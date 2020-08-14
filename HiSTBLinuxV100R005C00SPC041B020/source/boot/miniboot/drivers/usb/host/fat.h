/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#ifndef _FAT_H_
#define _FAT_H_

#include <stddef.h>
#include "part.h"
/* Maximum Long File Name length supported here is 128 UTF-16 code units */
#define VFAT_MAXLEN_BYTES	256 /* Maximum LFN buffer in bytes */
#define VFAT_MAXSEQ		9   /* Up to 9 of 13 2-byte UTF-16 entries */
#define PREFETCH_BLOCKS		2

#define MAX_CLUSTSIZE	16*1024
#define DIRENTSPERBLOCK	(mydata->sect_size / sizeof(dir_entry))
#define DIRENTSPERCLUST	((mydata->clust_size * mydata->sect_size) / \
			 sizeof(dir_entry))

#define FATBUFBLOCKS	6
#define FATBUFSIZE	(mydata->sect_size * FATBUFBLOCKS)
#define FAT32BUFSIZE	(FATBUFSIZE/4)

/* support max usb partition num ,start with 1 */
#define USB_MAX_PART_NUM  16

/* File attributes */
#define ATTR_VOLUME	8

#define DELETED_FLAG	((char)0xe5) /* Marks deleted files when in name[0] */
#define aRING		0x05	     /* Used as special character in name[0] */

/*
 * Indicates that the entry is the last long entry in a set of long
 * dir entries
 */
#define LAST_LONG_ENTRY_MASK	0x40

#define TOLOWER(c)	if((c) >= 'A' && (c) <= 'Z'){(c)+=('a' - 'A');}
#define START(dent)	((dent)->start + ((dent)->starthi << 16))
#define CHECK_CLUST(x) ((x) <= 1 ||(x) >= 0xffffff0)

typedef struct boot_sector {
	u8	ignored[3];	/* Bootstrap code */
	char	system_id[8];	/* Name of fs */
	u8	sector_size[2];	/* Bytes/sector */
	u8	cluster_size;	/* Sectors/cluster */
	u16	reserved;	/* Number of reserved sectors */
	u8	fats;		/* Number of FATs */
	u8	dir_entries[2];	/* Number of root directory entries */
	u8	sectors[2];	/* Number of sectors */
	u8	media;		/* Media code */
	u16	fat_length;	/* Sectors/FAT */
	u16	secs_track;	/* Sectors/track */
	u16	heads;		/* Number of heads */
	u32	hidden;		/* Number of hidden sectors */
	u32	total_sect;	/* Number of sectors (if sectors == 0) */

	/* FAT32 only */
	u32	fat32_length;	/* Sectors/FAT */
	u16	flags;		/* Bit 8: fat mirroring, low 4: active fat */
	u8	version[2];	/* Filesystem version */
	u32	root_cluster;	/* First cluster in root directory */
	u16	info_sector;	/* Filesystem info sector */
	u16	backup_boot;	/* Backup boot sector */
	u16	reserved2[6];	/* Unused */
} boot_sector;


typedef struct dir_entry {
	char	name[8],ext[3];	/* Name and extension */
	u8	attr;		/* Attribute bits */
	u32	reserved[2];
	u16	starthi;	/* High 16 bits of cluster in FAT32 */
	u16	time,date,start;/* Time, date and first cluster */
	u32	size;		/* File size in bytes */
} dir_entry;

/*
 * Private filesystem parameters
 *
 * Note: FAT buffer has to be 32 bit aligned
 * (see FAT32 accesses)
 */
typedef struct {
	u8	*fatbuf;	/* Current FAT buffer */
	u32	fatlength;	/* Length of FAT in sectors */
	u16	fat_sect;	/* Starting sector of the FAT */
	u32	rootdir_sect;	/* Start sector of root directory */
	u16	sect_size;	/* Size of sectors in bytes */
	u16	clust_size;	/* Size of clusters in sectors */
	int	data_begin;	/* The sector of the first cluster, can be negative */
	int	fatbufnum;	/* Used by get_fatent, init to -1 */
} fsdata;

#define DOS_FS32_TYPE_OFFSET	0x52
#define DEFAULT_SECTOR_SIZE	512
#define DOS_PART_TBL_OFFSET	0x1be
#define DOS_PART_MAGIC_OFFSET	0x1fe

typedef struct dos_partition {
	unsigned char boot_ind;		/* 0x80 - active			*/
	unsigned char reserved[3];	/* reserved				*/
	unsigned char sys_ind;		/* What partition type			*/
	unsigned char reserved2[3];	/* reserved2				*/
	unsigned char start4[4];	/* starting sector counting from 0	*/
	unsigned char size4[4];		/* nr of sectors in partition		*/
} dos_partition_t;


/* Currently this doesn't check if the dir exists or is valid... */
int file_fat_read(const char *filename, unsigned int addr, unsigned int maxsize);
int fat_register_device(block_dev_desc_t *dev_desc, int part_no);
#endif /* _FAT_H_ */
