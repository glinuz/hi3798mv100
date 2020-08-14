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

#ifndef FLASHH
#define FLASHH

#include <stddef.h>

enum flash_type_t {
	FT_SPIFLASH = 0,
	FT_NAND     = 1,
	FT_EMMC     = 2,
	FT_NONE     = 3,
} ;

struct flash_info_t {
	enum flash_type_t type;
	char name[32];
	u_char ids[8];
	int nr_ids;
	uint32 pagesize;  /* page size */
	uint32 blocksize; /* block size */
	uint32 oobsize;   /* controller used oobsize */
	uint32 oobused;   /* yaffs2 oobsize used */
	uint64 chipsize;
	uint32 nr_chips;
};

struct flash_ops_t;

struct flash_intf_t {
	uint64 from;
	uint64 length;
	const struct flash_info_t *info;
	struct flash_ops_t *ops;
};

/*
 * parameter:
 *   from   : offset by flash start address "0";
 *   length : open flash size, or real operation size;
 *   offset : offset by partition start address "from"
 *            if read partition start address, this parameter is "0";
 * return:
 *    <= 0  : fail;
 *    > 0   : return real erase/write/read length;
 */
struct flash_ops_t {
	const char *devname;

	int (*open)(uint64 from, uint64 length);

	void (*close)(struct flash_intf_t *intf);

	int64 (*erase)(struct flash_intf_t *intf, uint64 offset, uint64 length);

	int64 (*write)(struct flash_intf_t *intf, uint64 offset, uint64 length,
			char *buf, int withoob);

	int64 (*read)(struct flash_intf_t *intf, uint64 offset, uint64 length,
		       char *buf, int withoob);

	struct flash_info_t *(*get_info)(void);
};

int flash_register(enum flash_type_t type, struct flash_ops_t *ops);

/*****************************************************************************/

void *flash_open_range(uint32 flash, uint64 from, uint64 length);

void *flash_open_part(uint32 flash, const char *partname);

void flash_close(void *intf);

int64 flash_erase(void *intf, uint64 offset, uint64 length);

int64 flash_write(void *intf, uint64 offset, uint64 length,
		  char *buf, int withoob);

int64 flash_read(void *intf, uint64 offset, uint64 length,
		 char *buf, int withoob);

const char *flash_get_name(uint32 type);

void flash_show_info(struct flash_info_t *info);

const void *flash_get_info(uint32 flash);

/*****************************************************************************/
#endif
