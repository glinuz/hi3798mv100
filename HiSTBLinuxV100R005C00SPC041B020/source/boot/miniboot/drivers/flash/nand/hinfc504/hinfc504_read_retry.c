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

#include "hinfc504.h"
#include "hinfc504_gen.h"

extern struct read_retry_t hinfc504_hynix_bg_cdie_read_retry;
extern struct read_retry_t hinfc504_hynix_bg_bdie_read_retry;
extern struct read_retry_t hinfc504_hynix_cg_adie_read_retry;
extern struct read_retry_t hinfc504_micron_read_retry;
extern struct read_retry_t hinfc504_toshiba_24nm_read_retry;
extern struct read_retry_t hinfc504_samsung_read_retry;

static struct read_retry_t *hinfc504_read_retry[] = {
	&hinfc504_hynix_bg_bdie_read_retry,
	&hinfc504_hynix_bg_cdie_read_retry,
	&hinfc504_hynix_cg_adie_read_retry,
	&hinfc504_micron_read_retry,
	&hinfc504_toshiba_24nm_read_retry,
	&hinfc504_samsung_read_retry,
	NULL,
};

struct read_retry_t *hinfc504_get_read_retry(int type)
{
	struct read_retry_t **rr;

	for (rr = hinfc504_read_retry; rr; rr++) {
		if ((*rr)->type == type)
			return *rr;
	}
	return NULL;
}
