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

#ifndef SFINFH
#define SFINFH

#include <stddef.h>
#include <flash.h>

/*
 * parameter:
 *   offset or from : offset by spi flash start address 0.
 *   length or size : real operation length.
 *
 * return:
 *   0     : success;
 *  other  : fail.
 */
int spiflash_check_range(uint32 offset, uint32 *length);

int spiflash_check_align(uint32 offset, uint32 *length);

int spiflash_write(uint32 from, const char *buf, uint32 size);

int spiflash_read(uint32 from, char *buf, uint32 size);

int spiflash_erase(uint32 from, uint32 size);

struct flash_info_t *get_spiflash_info(void);

#endif /* SFINFH */
