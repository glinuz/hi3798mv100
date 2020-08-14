/*
 * fat.c
 *
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to uboot
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <exports.h>
#include <malloc.h>
#include "part.h"
#include "fat.h"
#include "usb.h"


static block_dev_desc_t *cur_dev;
static disk_partition_t cur_part_info;
extern int usb_max_devs;
extern int usbhost_init(void);
extern block_dev_desc_t usb_dev_desc[USB_MAX_STOR_DEV];


/* Convert char[4] in little endian format to the host format integer
 */
static inline int le32_to_int(unsigned char *le32)
{
    return ((le32[3] << 24) + (le32[2] << 16) + (le32[1] << 8) + le32[0]);
}

static inline int is_extended(int part_type)
{
    return (part_type == 0x5 ||part_type == 0xf || part_type == 0x85);
}

/*  Print a partition that is relative to its Extended partition table
 */
static int get_partition_info_extended (block_dev_desc_t *dev_desc, int ext_part_sector,
				 int relative, int part_num,
				 int which_part, disk_partition_t *info)
{
	unsigned char buffer[DEFAULT_SECTOR_SIZE];
	dos_partition_t *pt;
	int i;

	if (dev_desc->block_read (dev_desc->dev, ext_part_sector, 1, (u32 *) buffer) != 1) {
		printf ("** Can't read partition table on %d:%d **\n",
			dev_desc->dev, ext_part_sector);
		return -1;
	}
	if (buffer[DOS_PART_MAGIC_OFFSET] != 0x55 ||
		buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) {
		return -1;
	}

	/* Print all primary/logical partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		/*
		 * fdisk does not show the extended partitions that
		 * are not in the MBR
		 */
		if (((pt->boot_ind & ~0x80) == 0) &&(pt->sys_ind != 0) &&
		    (part_num == which_part) &&(is_extended(pt->sys_ind) == 0)) {
			info->blksz = 512;
			info->start = ext_part_sector + le32_to_int (pt->start4);
			info->size  = le32_to_int (pt->size4);
			return 0;
		}

		/* Reverse engr the fdisk part# assignment rule! */
		if ((ext_part_sector == 0) ||
		    (pt->sys_ind != 0 && !is_extended (pt->sys_ind)) ) {
			part_num++;
		}
	}

	/* Follows the extended partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		if (is_extended (pt->sys_ind)) {
			int lba_start = le32_to_int (pt->start4) + relative;

			return get_partition_info_extended (dev_desc, lba_start,
				 ext_part_sector == 0 ? lba_start : relative,
				 part_num, which_part, info);
		}
	}
	return -1;
}

/*
 * Convert a string to lowercase.
 */
static void downcase (char *str)
{
	while (*str != '\0') {
		TOLOWER(*str);
		str++;
	}
}

static int disk_read(u32 block, u32 nr_blocks, void *buf)
{
	if (!cur_dev || !cur_dev->block_read)
		return -1;

	return cur_dev->block_read(cur_dev->dev,cur_part_info.start + block, nr_blocks, buf);
}

int fat_register_device (block_dev_desc_t * dev_desc, int part_no)
{
	unsigned char buffer[dev_desc->blksz];
	/* First close any currently found FAT filesystem */
	cur_dev = NULL;

	/* Read the partition table, if present */
	if (!get_partition_info_extended (dev_desc, 0, 0, 1, part_no, &cur_part_info)){
		cur_dev = dev_desc;
	}

	if ((!cur_dev) ||(disk_read(0, 1, buffer) != 1)
		||(memcmp(buffer + DOS_PART_MAGIC_OFFSET, "\x55\xAA", 2))
		||(memcmp(buffer + DOS_FS32_TYPE_OFFSET, "FAT32", 5))){

		cur_dev = NULL;
		return -1;
	}

	return 0;
}


/*
 * Extract zero terminated short name from a directory entry.
 */
static void get_name (dir_entry *dirent, char *s_name)
{
	char *ptr;

	memcpy(s_name, dirent->name, 8);
	s_name[8] = '\0';
	ptr = s_name;
	while (*ptr && *ptr != ' ')
		ptr++;
	if (dirent->ext[0] && dirent->ext[0] != ' ') {
		*ptr = '.';
		ptr++;
		memcpy(ptr, dirent->ext, 3);
		ptr[3] = '\0';
		while (*ptr && *ptr != ' ')
			ptr++;
	}
	*ptr = '\0';
	if (*s_name == DELETED_FLAG)
		*s_name = '\0';
	else if (*s_name == aRING)
		*s_name = DELETED_FLAG;
	downcase(s_name);
}

/*
 * Get the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static u32 get_fatent (fsdata *mydata, u32 entry)
{
	u32 bufnum;
	u32  offset;
	u32 ret = 0x00;

	bufnum = entry / FAT32BUFSIZE;
	offset = entry - bufnum * FAT32BUFSIZE;


	/* Read a new block of FAT entries into the cache. */
	if (bufnum != mydata->fatbufnum) {
		u32 getsize = FATBUFBLOCKS;
		u8 *bufptr = mydata->fatbuf;
		u32 fatlength = mydata->fatlength;
		u32 startblock = bufnum * FATBUFBLOCKS;

		if (getsize > fatlength)
			getsize = fatlength;

		fatlength *= mydata->sect_size;	/* We want it in bytes now */
		startblock += mydata->fat_sect;	/* Offset from start of disk */

		if (disk_read(startblock, getsize, bufptr) < 0) {
			printf("Error reading FAT blocks\n");
			return ret;
		}
		mydata->fatbufnum = bufnum;
	}

	/* Get the actual entry from the table */
	ret = ((u32 *) mydata->fatbuf)[offset];

	return ret;
}

/*
 * Read at most 'size' bytes from the specified cluster into 'buffer'.
 * Return 0 on success, -1 otherwise.
 */
static int
get_cluster (fsdata *mydata, u32 clustnum, u8 *buffer,
	     unsigned long size)
{
	u32 idx = 0;
	u32 startsect;
	u32 nr_sect;
	int ret;

	if (clustnum > 0) {
		startsect = mydata->data_begin +
				clustnum * mydata->clust_size;
	} else {
		startsect = mydata->rootdir_sect;
	}


	nr_sect = size / mydata->sect_size;
	ret = disk_read(startsect, nr_sect, buffer);
	if (ret != nr_sect) {
		printf("Error reading data (got %d)\n", ret);
		return -1;
	}
	if (size > (mydata->sect_size*nr_sect)) {
		u8 tmpbuf[mydata->sect_size];
		idx = size / mydata->sect_size;
		ret = disk_read(startsect + idx, 1, tmpbuf);
		if (ret != 1) {
			printf("Error reading data (got %d)\n", ret);
			return -1;
		}
		buffer += idx * mydata->sect_size;

		memcpy(buffer, tmpbuf, size - idx *mydata->sect_size);
		return 0;
	}

	return 0;
}


/*
 * Read at most 'maxsize' bytes from 'pos' in the file associated with 'dentptr'
 * into 'buffer'.
 * Return the number of bytes read or -1 on fatal errors.
 */
static long
get_contents (fsdata *mydata, dir_entry *dentptr, 
	      u8 *buffer, unsigned long maxsize)

{
	unsigned long filesize = dentptr->size, gotsize = 0;
	unsigned int bytesperclust = mydata->clust_size * mydata->sect_size;
	u32 curclust = START(dentptr);
	u32 endclust, newclust;
	unsigned long actsize;

	if (maxsize > 0 && filesize > maxsize)
		filesize = maxsize;

	actsize = bytesperclust;
	endclust = curclust;

	do {
		/* search for consecutive clusters */
		while (actsize < filesize) {
			newclust = get_fatent(mydata, endclust);
			if ((newclust - 1) != endclust)
				goto getit;
			if (CHECK_CLUST(newclust)) {
				printf("curclust: 0x%x\n", newclust);
				printf("Invalid FAT entry\n");
				return gotsize;
			}
			endclust = newclust;
			actsize += bytesperclust;
		}

		/* actsize >= file size */
		actsize -= bytesperclust;

		/* get remaining clusters */
		if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
			printf("Error reading cluster\n");
			return -1;
		}

		/* get remaining bytes */
		gotsize += (int)actsize;
		filesize -= actsize;
		buffer += actsize;
		actsize = filesize;
		if (get_cluster(mydata, endclust, buffer, (int)actsize) != 0) {
			printf("Error reading cluster\n");
			return -1;
		}
		gotsize += actsize;
		return gotsize;
getit:
		if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
			printf("Error reading cluster\n");
			return -1;
		}
		gotsize += (int)actsize;
		filesize -= actsize;
		buffer += actsize;

		curclust = get_fatent(mydata, endclust);
		if (CHECK_CLUST(curclust)) {
			printf("curclust: 0x%x\n", curclust);
			printf("Invalid FAT entry\n");
			return gotsize;
		}
		actsize = bytesperclust;
		endclust = curclust;
	} while (1);
}



long
do_fat_read_at (const char *filename, void *buffer,
		unsigned long maxsize)

{
	boot_sector bs;
	u8 do_fat_read_at_block[2048];
	fsdata datablock;
	fsdata *mydata = &datablock;
	dir_entry *dentptr;
	u32 cursect;
	long ret = -1;
	u32 root_cluster = 0;
	int j;

	if (disk_read(0, 1, do_fat_read_at_block) < 0) {
		printf("Error: reading boot sector\n");
		return -1;
	}
	memcpy(&bs, do_fat_read_at_block, sizeof(boot_sector));

	root_cluster = bs.root_cluster;
	mydata->fatlength = bs.fat32_length;
	mydata->fat_sect = bs.reserved;
	cursect = mydata->rootdir_sect = mydata->fat_sect + mydata->fatlength * bs.fats;
	mydata->sect_size = (bs.sector_size[1] << 8) + bs.sector_size[0];
	mydata->clust_size = bs.cluster_size;
	if (mydata->sect_size != cur_part_info.blksz) {
		printf("Error: FAT sector size mismatch (fs=%hu, dev=%lu)\n",
				mydata->sect_size, cur_part_info.blksz);
		return -1;
	}
	mydata->data_begin = mydata->rootdir_sect -(mydata->clust_size * 2);
	mydata->fatbufnum = -1;
	mydata->fatbuf = malloc(FATBUFSIZE);
	if (mydata->fatbuf == NULL) {
		printf("Error: allocating memory\n");
		return -1;
	}
	

	j = 0;
	while (1) {

		int i;
		if (disk_read(cursect,1,do_fat_read_at_block) < 0) {
			printf("Error: reading rootdir block\n");
			goto exit;
		}

		dentptr = (dir_entry *) do_fat_read_at_block;

		for (i = 0; i < DIRENTSPERBLOCK; i++) {
			char s_name[14];

			if (dentptr->name[0] == DELETED_FLAG) {
				dentptr++;
				continue;
			}
			if ((dentptr->attr & ATTR_VOLUME)) {
					/* Volume label or VFAT entry */
					dentptr++;
					continue;
			} else if (dentptr->name[0] == 0) {
				goto exit;
			}
			
			get_name(dentptr, s_name);

			if (strcmp(filename, s_name)) {
				dentptr++;
				continue;
			}

			goto rootdir_done;	/* We got a match */
		}

		/*
		 * On FAT32 we must fetch the FAT entries for the next
		 * root directory clusters when a cluster has been
		 * completely processed.
		 */
		++j;
		int fat32_end = 0;
		if (j == mydata->clust_size) {
			int nxtsect = 0;
			int nxt_clust = 0;

			nxt_clust = get_fatent(mydata, root_cluster);
			fat32_end = CHECK_CLUST(nxt_clust);

			nxtsect = mydata->data_begin + (nxt_clust * mydata->clust_size);

			root_cluster = nxt_clust;

			cursect = nxtsect;
			j = 0;
		} else {
			cursect++;
		}

		/* If end of rootdir reached */
		if (fat32_end) {
			goto exit;
		}
	}
	
rootdir_done:

	ret = get_contents(mydata, dentptr, buffer, maxsize);

exit:
	free(mydata->fatbuf);
	return ret;
}



/******************************************************************************/
int file_fat_read(const char *filename, unsigned int addr, unsigned int size)
{
	int ix_dev;
	int ix_part;
	block_dev_desc_t *dev_desc = NULL;

	for (ix_dev = 0 ; ix_dev < usb_max_devs; ix_dev++) {
		dev_desc = &usb_dev_desc[ix_dev];
		if (!dev_desc)
			continue;
		for (ix_part = 1; ix_part < USB_MAX_PART_NUM;ix_part++) {
			if (!fat_register_device(dev_desc,ix_part)) {
				size = do_fat_read_at(filename, (unsigned char *)addr, size);
				if (size < 0)
					continue;
				printf("\n%ld bytes read\n",size);
				return 0;
			}
		}
	}

	printf("not find file\n");
	return 1;
}
