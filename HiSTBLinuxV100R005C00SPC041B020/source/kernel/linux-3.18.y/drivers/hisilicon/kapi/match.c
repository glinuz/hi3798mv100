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

#include <linux/string.h>
#include <linux/hisilicon/himisc.h>

/*****************************************************************************/

int match_reg_to_type(struct match_t *table, int nr_table, int reg, int def)
{
	while (nr_table-- > 0) {
		if (table->reg == reg)
			return table->type;
		table++;
	}
	return def;
}

int match_type_to_reg(struct match_t *table, int nr_table, int type, int def)
{
	while (nr_table-- > 0) {
		if (table->type == type)
			return table->reg;
		table++;
	}
	return def;
}

int match_data_to_type(struct match_t *table, int nr_table, char *data, int size,
		      int def)
{
	while (nr_table-- > 0) {
		if (!memcmp(table->data, data, size))
			return table->type;
		table++;
	}
	return def;
}

void *match_type_to_data(struct match_t *table, int nr_table, int type,
			 void *def)
{
	while (nr_table-- > 0) {
		if (table->type == type)
			return table->data;
		table++;
	}
	return def;
}
