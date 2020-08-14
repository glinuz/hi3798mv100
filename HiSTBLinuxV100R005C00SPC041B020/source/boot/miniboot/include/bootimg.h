/* tools/mkbootimg/bootimg.h
**
** Copyright 2007, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

#ifndef _BOOT_IMAGE_H_
#define _BOOT_IMAGE_H_

struct img_hdr_t {
#define UIMG_MAGIC        0x27051956
	uint32 magic;
	uint32 hcrc;
	uint32 time;
	uint32 size;
	uint32 load;
	uint32 entry;
	uint32 dcrc;
	uint8 os;
	uint8 arch;
	uint8 type;
	uint8 comp;
	char name[32];
};

typedef struct boot_img_hdr boot_img_hdr;

#define BOOT_MAGIC              "ANDROID!"
#define BOOT_MAGIC_SIZE         8
#define BOOT_NAME_SIZE          16
#define BOOT_ARGS_SIZE          512
#define HI_ADVCA_MAGIC          "Hisilicon_ADVCA_ImgHead_MagicNum"
#define HI_ADVCA_MAGIC_SIZE     32
#define MAX_BOOTIMG_LEN         0x4000000  /* 64M */

struct boot_img_hdr {
	char magic[BOOT_MAGIC_SIZE];

	uint32 kernel_size; /* kernel size in bytes */
	uint32 kernel_addr; /* physical load addr */

	uint32 ramdisk_size; /* ramdisk size in bytes */
	uint32 ramdisk_addr; /* physical load addr */

	uint32 second_size; /* size in bytes */
	uint32 second_addr; /* physical load addr */

	uint32 tags_addr; /* physical addr for kernel tags */
	uint32 page_size; /* flash page size we assume */
	uint32 unused[2]; /* future expansion: should be 0 */

	char name[BOOT_NAME_SIZE]; /* asciiz product name */

	char cmdline[BOOT_ARGS_SIZE];

	uint32 id[8]; /* timestamp / checksum / sha1 / etc */
};

typedef void (*kernel_fn)(int flags, int mach, uint32 tags);

/******************************************************************************/

/*
** +-----------------+ 
** | boot header     | 1 page
** +-----------------+
** | kernel          | n pages  
** +-----------------+
** | ramdisk         | m pages  
** +-----------------+
** | second stage    | o pages
** +-----------------+
**
** n = (kernel_size + page_size - 1) / page_size
** m = (ramdisk_size + page_size - 1) / page_size
** o = (second_size + page_size - 1) / page_size
**
** 0. all entities are page_size aligned in flash
** 1. kernel and ramdisk are required (size != 0)
** 2. second is optional (second_size == 0 -> no second)
** 3. load each element (kernel, ramdisk, second) at
**    the specified physical address (kernel_addr, etc)
** 4. prepare tags at tag_addr.  kernel_args[] is
**    appended to the kernel commandline in the tags.
** 5. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
** 6. if second_size != 0: jump to second_addr
**    else: jump to kernel_addr
*/

void show_bootimg_header(void *buf);

int check_bootimg(char *buf, unsigned int len);

int bootimg_get_kernel(char *buf, unsigned int len, unsigned int *entry);

int bootimg_get_ramfs(char *buf, unsigned int len, unsigned int *initrd_start,
		      unsigned int *initrd_size);

#endif
