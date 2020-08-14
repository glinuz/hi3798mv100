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
 * Create By Cai Zhiying 2015.8.10
 *
******************************************************************************/

#ifndef FLASH_STATS_H
#define FLASH_STATS_H

/******************************************************************************/

struct flash_stats {
	void (*erase)(struct flash_stats *stats, u64 addr, u64 len);
	void (*read)(struct flash_stats *stats, u64 addr, u64 len, u8 *ecc);
	void (*write)(struct flash_stats *stats, u64 addr, u64 len);
	void (*read_retry)(struct flash_stats *stats, u64 addr, int index);
};

struct flash_stats *flash_stats_create(char *name, int pagesize, int blocksize,
	u64 totalsize, int nr_ecc_sect, int read_retries);

void flash_stats_destory(struct flash_stats *stats);

/******************************************************************************/

#endif /* FLASH_STATS_H */

