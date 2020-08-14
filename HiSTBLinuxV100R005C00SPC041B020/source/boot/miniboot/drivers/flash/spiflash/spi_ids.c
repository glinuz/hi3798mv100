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

#include <config.h>
#include <stdio.h>
#include <string.h>

#include <asm/io.h>

#include "spi_ids.h"

#define SPI_DRV_VERSION       "2.00"

#define DBG_MSG(_fmt, ...)
#define DBG_BUG(fmt, ...)

/*****************************************************************************/

struct spi_info *spi_serach_ids(struct spi_info *spi_info_table,
				unsigned char ids[8])
{
	struct spi_info *info;
	struct spi_info *fit_info = NULL;

	for (info = spi_info_table; info->name; info++) {
		if (memcmp(info->id, ids, info->id_len))
			continue;

		if (fit_info == NULL || fit_info->id_len < info->id_len)
			fit_info = info;
	}

	return fit_info;
}
/*****************************************************************************/
void spi_search_rw(struct spi_info *spiinfo, struct spi_operation *spiop_rw,
		   unsigned int iftype, unsigned int max_dummy, int is_read)
{
	int ix = 0;
	struct spi_operation **spiop, **fitspiop;

	for (fitspiop = spiop = (is_read ? spiinfo->read : spiinfo->write);
		(*spiop) && ix < MAX_SPI_OP; spiop++, ix++) {
		DBG_MSG("dump[%d] %s iftype:0x%02X\n", ix,
			(is_read ? "read" : "write"),
			(*spiop)->iftype);

		if (((*spiop)->iftype & iftype)
			&& ((*spiop)->dummy <= max_dummy)
			&& (*fitspiop)->iftype < (*spiop)->iftype)
			fitspiop = spiop;
	}
	memcpy(spiop_rw, (*fitspiop), sizeof(struct spi_operation));
}
/*****************************************************************************/
void spi_get_erase(struct spi_info *spiinfo, struct spi_operation *spiop_erase)
{
	int ix;

	spiop_erase->size = 0;
	for (ix = 0; ix < MAX_SPI_OP; ix++) {
		if (spiinfo->erase[ix] == NULL)
			break;
		if (spiinfo->erasesize == spiinfo->erase[ix]->size) {
			memcpy(spiop_erase, spiinfo->erase[ix],
				sizeof(struct spi_operation));
			break;
		}
	}
	if (!spiop_erase->size)
		DBG_BUG(("Spi erasesize error!"));
}

/*****************************************************************************/
void spi_get_erase_sfcv300(struct spi_info *spiinfo,
			   struct spi_operation *spiop_erase,
			   unsigned int *erasesize)
{
	int ix;

	(*erasesize) = spiinfo->erasesize;
	for (ix = 0; ix < MAX_SPI_OP; ix++) {
		if (spiinfo->erase[ix] == NULL)
			break;

		memcpy(&spiop_erase[ix], spiinfo->erase[ix],
			sizeof(struct spi_operation));

		switch (spiop_erase[ix].size) {
		case SPI_IF_ERASE_SECTOR:
			spiop_erase[ix].size = spiinfo->erasesize;
			break;
		case SPI_IF_ERASE_CHIP:
			spiop_erase[ix].size = spiinfo->chipsize;
			break;
		}

		if ((int)(spiop_erase[ix].size) < _2K) {
			DBG_BUG(("erase block size mistaken: "
				"spi->erase[%d].size:%s\n",
				ix, u32tohstr(spiop_erase[ix].size, NULL)));
		}

		if (spiop_erase[ix].size < (*erasesize)) {
			(*erasesize) = spiop_erase[ix].size;
		}
	}
}
