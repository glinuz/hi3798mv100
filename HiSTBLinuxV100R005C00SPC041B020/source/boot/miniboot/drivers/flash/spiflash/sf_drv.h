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

#ifndef SPIFLASHH
#define SPIFLASHH

#include <stddef.h>
#include <flash.h>
#include "spi_ids.h"

/* spiflash driver interface. */
struct spiflash_dev_t {

	unsigned int size;

	const char *name;

	int (*read)(struct spiflash_dev_t *flash, uint32, size_t, void *);
	int (*write)(struct spiflash_dev_t *flash, uint32, size_t, const void *);
	int (*erase)(struct spiflash_dev_t *flash, uint32, size_t);
};

typedef struct spiflash_dev_t *(*spiflash_driver_init_fn)(void);

extern spiflash_driver_init_fn spiflash_driver_init;

int set_spiflash_info(uint32 chipsize, uint32 blocksize, uint32 nr_chips,
		      const char *name, u_char *ids, uint32 nr_ids);

#endif /* SPIFLASHH */
