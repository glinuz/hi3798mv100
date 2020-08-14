/******************************************************************************
 *  Copyright (C) 2014 Cai Zhiyong
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
 * Create By Cai Zhiying 2014.11.17
 *
******************************************************************************/


#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"
#include "hinfc610_cache.h"

/******************************************************************************/

static int hinfc610_cache_enable(struct dentry *root, struct hinfc_host *host)
{
	hinfc_cache_enable(host->cache, 1);
	return 0;
}
/******************************************************************************/

static int hinfc610_cache_disable(struct hinfc_host *host)
{
	hinfc_cache_enable(host->cache, 0);
	return 0;
}
/******************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_if_cache_enable = {
	"cache", 1,
	hinfc610_cache_enable,
	hinfc610_cache_disable,
	NULL,
	NULL,
	NULL,
	NULL,
};
/******************************************************************************/

static int hinfc610_cache_crc_enable(struct dentry *root, struct hinfc_host *host)
{
	hinfc_cache_crc_enable(host->cache, 1);
	return 0;
}
/******************************************************************************/

static int hinfc610_cache_crc_disable(struct hinfc_host *host)
{
	hinfc_cache_crc_enable(host->cache, 0);
	return 0;
}
/******************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_if_cache_crc = {
	"cache_crc", 0,
	hinfc610_cache_crc_enable,
	hinfc610_cache_crc_disable,
	NULL,
	NULL,
	NULL,
	NULL,
};
