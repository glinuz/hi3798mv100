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

#ifndef NFC_IFH
#define NFC_IFH
/******************************************************************************/

#include <stddef.h>
#include <flash.h>

/*****************************************************************************/

struct flash_info_t *get_nand_info(void);

int nand_check_range(const uint64 *address, uint64 *size);

int nand_check_align(const uint64 *address, uint64 *size, int blockalign);

int nand_write(uint64 *address, char *buf, const uint64 *size, uint32 *oobsize,
	       int withoob, int skipempty);

int nand_read(uint64 *address, char *buf, const uint64 *size, int withoob,
	      int skipbad);

int nand_mark_bad(const uint64 *address);

int nand_bad_block(const uint64 *address);

int64 nand_erase_block(const uint64 *address, const uint64 *size, int force);

/******************************************************************************/
#endif /* NFC_IFH */
