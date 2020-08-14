/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/

#include <linux/string.h>

#include <match_table.h>

int reg2type(struct match_reg_type *table, int length, int reg, int def)
{
	while (length-- > 0) {
		if (table->reg == reg)
			return table->type;
		table++;
	}
	return def;
}

int type2reg(struct match_reg_type *table, int length, int type, int def)
{
	while (length-- > 0) {
		if (table->type == type)
			return table->reg;
		table++;
	}
	return def;
}

int str2type(struct match_type_str *table, int length, const char *str,
	     int size, int def)
{
	while (length-- > 0) {
		if (!strncmp(table->str, str, size))
			return table->type;
		table++;
	}
	return def;
}

const char *type2str(struct match_type_str *table, int length, int type,
		     const char *def)
{
	while (length-- > 0) {
		if (table->type == type)
			return table->str;
		table++;
	}
	return def;
}

