/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Liu Hui 2016.4.16
 *
******************************************************************************/
#include <config.h>
#include <common.h>
#include <mmc.h>


/******************************************************************************/
struct emmc_dev emmc_ids_hisi[] = {

	/* sample */
	{
		.mid  = 0x45,
		.pnm  = "DS2008",
		.caps = EXT_CSD_CARD_TYPE_DDR_52,
	},
	/* ALL TOSHIBA run in HS200 or DDR50*/
	{
		.mid  = 0x11,
		.pnm  = NULL,
		.caps = EXT_CSD_CARD_TYPE_DDR_52
			| EXT_CSD_CARD_TYPE_SDR_200,
	},
	{NULL}
};

struct emmc_dev *mmc_get_ids_dev(uint * cid)
{
	struct emmc_dev *mmc_dev;
	char buf[7];

	for (mmc_dev = emmc_ids_hisi; mmc_dev->mid; mmc_dev++) {

		if ((char)(cid[0] >> 24) != mmc_dev->mid)
			continue;

		snprintf(buf, 7, "%c%c%c%c%c%c",
	       cid[0] & 0xff,
	       (cid[1] >> 24),
	       (cid[1] >> 16) & 0xff,
	       (cid[1] >> 8) & 0xff,
	       cid[1] & 0xff,
	       cid[2] >> 24);
		if ((mmc_dev->pnm) && strncmp(buf, mmc_dev->pnm, 6))
			continue;

		return mmc_dev;
	}

	return NULL;
}
