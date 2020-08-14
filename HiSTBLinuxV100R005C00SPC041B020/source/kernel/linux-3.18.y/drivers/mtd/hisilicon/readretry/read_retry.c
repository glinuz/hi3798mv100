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
 * Create By Cai Zhiying 2015.7.27
 *
******************************************************************************/

#include <hiflash.h>

#include "read_retry.h"

/******************************************************************************/

static struct nand_read_retry *nand_read_retry[] = {
	&nand_read_retry_micron,
	&nand_read_retry_samsung,
	&nand_read_retry_toshiba_v2012,
	&nand_read_retry_toshiba_v2013,
	&nand_read_retry_hynix_cg_adie,
	&nand_read_retry_hynix_bc_cdie,
	NULL,
};
/******************************************************************************/

struct nand_read_retry *nand_get_read_retry(int read_retry_type)
{
	int ix;

	for (ix = 0; nand_read_retry[ix]; ix++) {
		if (nand_read_retry[ix]->type == read_retry_type)
			return nand_read_retry[ix];
	}

	return NULL;
}
