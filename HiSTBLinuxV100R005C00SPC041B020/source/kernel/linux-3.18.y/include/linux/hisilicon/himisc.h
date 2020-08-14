/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#ifndef __HISI_MISC__
#define __HISI_MISC__

#include <uapi/linux/types.h>

#ifdef CONFIG_BLK_DEV_RAM
void initmrd_reserve_memory(void);
#else
static inline void initmrd_reserve_memory(void) {}
#endif

#ifdef CONFIG_BLK_DEV_LOOP
void initfile_reserve_memory(void);
#else
static inline void initfile_reserve_memory(void) {}
#endif

#ifdef CONFIG_PCIE_HISILICON
void pcie_reserve_memory(void);
#else
static inline void pcie_reserve_memory(void) {}
#endif

void pdm_reserve_mem(void);

struct match_t {
	int type;
	int reg;
	void *data;
};

#define MATCH_SET_TYPE_REG(_type, _reg)   {(_type), (_reg), (void *)0}
#define MATCH_SET_TYPE_DATA(_type, _data) {(_type), 0, (void *)(_data)}
#define MATCH_SET(_type, _reg, _data)     {(_type), (_reg), (void *)(_data)}

int match_reg_to_type(struct match_t *table, int nr_table, int reg, int def);

int match_type_to_reg(struct match_t *table, int nr_table, int type, int def);

int match_data_to_type(struct match_t *table, int nr_table, char *data, int size,
		       int def);

void *match_type_to_data(struct match_t *table, int nr_table, int type,
			 void *def);

int find_param_tag(const char *name, char **data);

int fdt_add_memory_reserve(u64 base, u64 size);

#endif /* __HISI_MISC__ */
