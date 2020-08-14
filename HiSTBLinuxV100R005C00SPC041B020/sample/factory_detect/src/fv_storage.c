#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#include "fv_face.h"
#include "fv_config.h"
#include "fv.h"
#include "fv_threads.h"
#include "fv_dbg.h"
struct partition {
        unsigned char boot_ind;         /* 0x80 - active */
        unsigned char head;             /* starting head */
        unsigned char sector;           /* starting sector */
        unsigned char cyl;              /* starting cylinder */
        unsigned char sys_ind;          /* what partition type */
        unsigned char end_head;         /* end head */
        unsigned char end_sector;       /* end sector */
        unsigned char end_cyl;          /* end cylinder */
        unsigned char start4[4];        /* starting sector counting from 0 */
        unsigned char size4[4];         /* nr of sectors in partition */
} PACKED;
/*
 * per partition table entry data
 *
 * The four primary partitions have the same sectorbuffer (MBRbuffer)
 * and have NULL ext_pointer.
 * Each logical partition table entry has two pointers, one for the
 * partition and one link to the next one.
 */
struct pte {
        struct partition *part_table;   /* points into sectorbuffer */
        struct partition *ext_pointer;  /* points into sectorbuffer */
        unsigned int offset;                /* disk sector number */
        char *sectorbuffer;             /* disk sector contents */
};
static const char *const i386_sys_types[] = {
        "\x00" "Empty",
        "\x01" "FAT12",
        "\x04" "FAT16 <32M",
        "\x05" "Extended",         /* DOS 3.3+ extended partition */
        "\x06" "FAT16",            /* DOS 16-bit >=32M */
        "\x07" "HPFS/NTFS",        /* OS/2 IFS, eg, HPFS or NTFS or QNX */
        "\x0a" "OS/2 Boot Manager",/* OS/2 Boot Manager */
        "\x0b" "Win95 FAT32",
        "\x0c" "Win95 FAT32 (LBA)",/* LBA really is 'Extended Int 13h' */
        "\x0e" "Win95 FAT16 (LBA)",
        "\x0f" "Win95 Ext'd (LBA)",
        "\x11" "Hidden FAT12",
        "\x12" "Compaq diagnostics",
        "\x14" "Hidden FAT16 <32M",
        "\x16" "Hidden FAT16",
        "\x17" "Hidden HPFS/NTFS",
        "\x1b" "Hidden Win95 FAT32",
        "\x1c" "Hidden W95 FAT32 (LBA)",
        "\x1e" "Hidden W95 FAT16 (LBA)",
        "\x3c" "Part.Magic recovery",
        "\x41" "PPC PReP Boot",
        "\x42" "SFS",
        "\x63" "GNU HURD or SysV", /* GNU HURD or Mach or Sys V/386 (such as ISC UNIX) */
        "\x80" "Old Minix",        /* Minix 1.4a and earlier */
        "\x81" "Minix / old Linux",/* Minix 1.4b and later */
        "\x82" "Linux swap",       /* also Solaris */
        "\x83" "Linux",
        "\x84" "OS/2 hidden C: drive",
        "\x85" "Linux extended",
        "\x86" "NTFS volume set",
        "\x87" "NTFS volume set",
        "\x8e" "Linux LVM",
        "\x9f" "BSD/OS",           /* BSDI */
        "\xa0" "Thinkpad hibernation",
        "\xa5" "FreeBSD",          /* various BSD flavours */
        "\xa6" "OpenBSD",
        "\xa8" "Darwin UFS",
        "\xa9" "NetBSD",
        "\xab" "Darwin boot",
        "\xb7" "BSDI fs",
        "\xb8" "BSDI swap",
        "\xbe" "Solaris boot",
        "\xeb" "BeOS fs",
        "\xee" "EFI GPT",                    /* Intel EFI GUID Partition Table */
        "\xef" "EFI (FAT-12/16/32)",         /* Intel EFI System Partition */
        "\xf0" "Linux/PA-RISC boot",         /* Linux/PA-RISC boot loader */
        "\xf2" "DOS secondary",              /* DOS 3.3+ secondary */
        "\xfd" "Linux raid autodetect",      /* New (2.2.x) raid partition with
                                                autodetect using persistent
                                                superblock */
	NULL
};
#define pt_offset(b, n) \
        ((struct partition *)((b) + 0x1be + (n) * sizeof(struct partition)))

#define MAX_STR_SZ 100
#define MAX_MBR_SZ 2048
#define MAX_MAIN_PARTITIONS_NR	4

#if MAX_MAIN_PARTITIONS_NR > 4
#error MAX_MAIN_PARTITIONS_NR > 4
#endif

#define MAX_STORAGE_TXT_BUF_SZ 100

#define VIDEO_MOUNT_POINT UDISK_MOUNT_POINT 
#define STORAGE_NOT_EXIST FV_ITEM_NULL_NAME

static char storage_txt_buffer[MAX_STORAGE_TXT_BUF_SZ];
static char* storage_txt_pos;
static int storage_txt_free_buf;

static const char* get_partition_type(unsigned char type)
{
	int j;	
	for (j = 0; i386_sys_types[j]; j++) {
		if (type == (unsigned char)i386_sys_types[j][0]) {
			return i386_sys_types[j] + 1;
		}
	}
	return "Unkown";
}
static void fv_storage_get_part_info(int fd, struct pte *pes, int nr)
{
	int i;
	const char *p;
	int  len;
	
	for (i = 0; i < nr; i++) {
		if (storage_txt_free_buf <= 0)
			break;
		if (!pes->part_table[i].sys_ind)
			continue;
		p = get_partition_type(pes->part_table[i].sys_ind);
		fv_debug("p:%s\n",p);
		len = strlen(p) + 5;
		len = len > storage_txt_free_buf ? storage_txt_free_buf : len;
		snprintf(storage_txt_pos, len, "%d(%s),", i, p);
		storage_txt_pos += len - 1;
		storage_txt_free_buf -= len - 1;	
		fv_debug("storage_txt_buffer:%s\n",storage_txt_buffer);
	}
}
static void fv_storage_update_face(int item_type, int sn)
{
	enum fv_item_list i;
    
    if (1 == item_type){
    	i = FV_ITEM_USB1_TXT + sn * 2 - 2;
    	if (i <= FV_ITEM_USB2_TXT)
    		(void)HI_FV_DrawText(i, storage_txt_buffer, MAX_STORAGE_TXT_BUF_SZ - storage_txt_free_buf);
    }
    else{
        (void)HI_FV_DrawText(FV_ITEM_SD_TXT, storage_txt_buffer, MAX_STORAGE_TXT_BUF_SZ - storage_txt_free_buf);
    }
}

static void fv_storage_umount_part(char *dev_path)
{	
	char cmd_buf[128];
	
	snprintf(cmd_buf, sizeof(cmd_buf),
			 "umount %s &>/dev/null", dev_path);
	
	system(cmd_buf);
}

static int fv_storage_mount_part(char *dev_path, char *mount_point)
{
	char cmd_buf[128];
	int ret;

	/* FAT */
	fv_storage_umount_part(dev_path);
	
	snprintf(cmd_buf, sizeof(cmd_buf),
			 "mount -t vfat %s %s", dev_path, mount_point);
	
	fv_debug(cmd_buf);

	ret = system(cmd_buf);
	if (WIFEXITED(ret) && 0 == (WEXITSTATUS(ret)))
		return 0;

	/* add another filesystem support */

	return -1;
}



static int video_bind;
char bind_dev_path[1024];
static int fv_storage_find_video(int fd, struct pte *pes, char *path, char *video)
{
	int i, tfd,tfd2;
	char cmd_buf[1024];

	if (video_bind) {
		int exit = 0;
		tfd2 = open(bind_dev_path, O_RDONLY);
		if (tfd2 >= 0) {
			close(tfd2);
			tfd = open(VIDEO_MOUNT_POINT "/" VIDEO_FILE_NAME, O_RDONLY);
			if (tfd >= 0) {
				close(tfd);
				return 1;
			}
		}
		video_bind = 0;
		/* FIXME: notify tsplay that file disappeared. */
		isTsFileExist(0);
		getTsFileState(&exit);
		while(exit >= 0) {
			sleep(1);
			getTsFileState(&exit);
		}

		snprintf(cmd_buf, strlen("umount ") + strlen(VIDEO_MOUNT_POINT) + 1,
			"umount %s", VIDEO_MOUNT_POINT);
		fv_debug(cmd_buf);
		system(cmd_buf);
		memset(bind_dev_path, 0, sizeof(bind_dev_path));
	}
	
	for (i = 0; i < MAX_MAIN_PARTITIONS_NR; i++) {
		snprintf(cmd_buf, sizeof(cmd_buf),
			 "%s%d", path, i + 1);
		
		if (0 != fv_storage_mount_part(cmd_buf, UDISK_MOUNT_POINT))
			continue;

		tfd = open(VIDEO_MOUNT_POINT "/" VIDEO_FILE_NAME, O_RDONLY);
		if (tfd >= 0) {
			video_bind = 1;
			close(tfd);
			/* FIXME: notify tsplay that file appeared. */	
			isTsFileExist(1);
			strcpy(bind_dev_path, path);
			return 0;
		}

		fv_debug("No video file found at %s\n", cmd_buf);

		fv_storage_umount_part(cmd_buf);
	}

	return -1;	
}
static void fv_storage_process_dev(char *path, int sn)
{
	char mbrbuf[MAX_MBR_SZ];
	int fd, i;
	struct pte *pe;
	struct pte pes[MAX_MAIN_PARTITIONS_NR];
	
	fv_debug("%s: path:%s.\n", __func__, path);
	
	/* get mbr info from device. */
	for (i = 0; i < MAX_MAIN_PARTITIONS_NR; i++) {
		pe = &pes[i];
		pe->part_table = pt_offset(mbrbuf, i);
		pe->ext_pointer = NULL;
		pe->offset = 0;
		pe->sectorbuffer = mbrbuf;
	}

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fv_error("Fail to open dev %s, ignore it\n",path);
		goto out;
	}

	if (512 != read(fd, mbrbuf, 512)) {
		fv_error("Fail to read 512 byte from the head of %s, ignore dev\n", path);
		goto err_read_mbr;
	}
	
	fv_storage_get_part_info(fd, pes, MAX_MAIN_PARTITIONS_NR);

       /* only find ts video file in usb device */
       if (!strncmp(path, "/dev/sd", strlen("/dev/sd")))
	       fv_storage_find_video(fd, pes, path, VIDEO_FILE_NAME);

	close(fd);
	return;
err_read_mbr:
	close(fd);
out:
	return;
}

void fv_storage_detect(int usb_nr, int sd_nr)
{
	FILE *f;
	int mi, ma,  sz;
	char buf[MAX_STR_SZ], ptname[MAX_STR_SZ], devname[MAX_STR_SZ];
	char *s;
	struct stat statbuf;
	int usbsn = 0, sdsn = 0, i, bits = 0;
	int storage_type = 0;

	if ((usb_nr <= 0) || (sd_nr <= 0))
		return;

	f = fopen("/proc/partitions", "r");
	if (!f) {
		fv_error("Fail to open /proc/partitions\n");
		goto out;
	}

	storage_txt_pos = storage_txt_buffer;	
	storage_txt_free_buf = MAX_STORAGE_TXT_BUF_SZ;

        /*read first two invalid line*/
        fgets(buf, sizeof(buf), f);
        fgets(buf, sizeof(buf), f);

	while (fgets(buf, sizeof(buf), f)) {
		if (sscanf(buf, " %u %u %u %[^\n ]",
					&ma, &mi, &sz, ptname) != 4)
			continue;
		for (s = ptname; *s; s++)
			continue;
		if (isdigit(s[-1]) && strncmp(ptname, "mmcblk", strlen("mmcblk")))
			continue;

		if (strlen(ptname) + strlen("/dev/") + 1 >= MAX_STR_SZ) {
			fv_error("dev %s's name too long, ignore it!\n", ptname);
			continue;
		}

		sprintf(devname, "/dev/%s", ptname);

		if (stat(devname, &statbuf) == 0 && ((statbuf.st_mode & 0222) == 0)) {
			fv_error("dev %s is readonly, ignore it!\n", devname);
			continue;
		}

		fv_debug("device :%s\n", ptname);

		if (storage_txt_free_buf > strlen(ptname) + 2) {
			snprintf(storage_txt_pos, strlen(ptname) + 2, "%s:", ptname);
			storage_txt_free_buf -= strlen(ptname) + 2;
			/* reserve pos for ';' */
			storage_txt_pos += strlen(ptname) + 1;

			if (storage_txt_free_buf > 0) {
				if (sdsn < sd_nr && !strncmp(ptname, "mmcblk", strlen("mmcblk")))
				{
					fv_storage_process_dev(devname, sdsn);
					bits |= (1 << sdsn);
					sdsn++;
					storage_type = 0;
				}

                            if (usbsn < usb_nr && !strncmp(ptname, "sd", strlen("sd")))
				{
					fv_storage_process_dev(devname, usbsn);
					bits |= (1 << (usbsn+16));
					usbsn++;
					storage_type =1;
				}
			}
			fv_debug("%s->%d,usb txt buffer:%s\n", __func__,__LINE__,storage_txt_buffer);

			//strcat(storage_txt_buffer, ";");
			storage_txt_buffer[strlen(storage_txt_buffer) - 1] = '\0';
			fv_debug("%s->%d,usb txt buffer:%s\n", __func__,__LINE__,storage_txt_buffer);
		}

              if (sdsn > sd_nr && usbsn > usb_nr)
                break;
              
		fv_debug("%s->%d,storage_type:%d,bits:0x%x,usb txt buffer:%s\n", __func__,__LINE__,
				storage_type, bits,storage_txt_buffer);

		fv_storage_update_face(storage_type, (0 == storage_type)?sdsn:usbsn);	

		storage_txt_pos = storage_txt_buffer;	
		storage_txt_free_buf = MAX_STORAGE_TXT_BUF_SZ;
		memset(storage_txt_buffer, 0, MAX_STORAGE_TXT_BUF_SZ);
		strcpy(storage_txt_buffer, STORAGE_NOT_EXIST);
     
	}
	
	/*fv_debug("one turn done, sn:%d !\n", sn);  */
	fclose(f);
	if (video_bind && !usbsn) {
		char cmd_buf[1024];
		int exit;

		/* FIXME: notify tsplay that file disappeared. */

		isTsFileExist(0);
		getTsFileState(&exit);
		while(exit >= 0) {
			sleep(1);
			getTsFileState(&exit);
		}

		snprintf(cmd_buf, strlen("umount ") + strlen(VIDEO_MOUNT_POINT) + 1,
				"umount %s", VIDEO_MOUNT_POINT);
		fv_debug(cmd_buf);
		system(cmd_buf);
		video_bind = 0;
	}

	for (i = 0; i < sd_nr; i++) {
		if (bits & (1 << i))
			continue;
		storage_txt_pos = storage_txt_buffer;	
		storage_txt_free_buf = MAX_STORAGE_TXT_BUF_SZ;
		memset(storage_txt_buffer, 0, MAX_STORAGE_TXT_BUF_SZ);
		strcpy(storage_txt_buffer, STORAGE_NOT_EXIST);
		fv_storage_update_face(0, i + 1);	
	}
	fv_debug("%s->%d,storage_type:%d,bits:0x%x,usb txt buffer:%s\n", __func__,__LINE__,
			storage_type, bits,storage_txt_buffer);
	for (i = 0; i < usb_nr; i++) {
		if (bits & (1 << (i + 16)))
			continue;
		storage_txt_pos = storage_txt_buffer;	
		storage_txt_free_buf = MAX_STORAGE_TXT_BUF_SZ;
		memset(storage_txt_buffer, 0, MAX_STORAGE_TXT_BUF_SZ);
		strcpy(storage_txt_buffer, STORAGE_NOT_EXIST);
		fv_storage_update_face(1, i + 1);	
	}
out:
	return;
}
