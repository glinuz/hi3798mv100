/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/
#ifndef MATCH_TABLEH
#define MATCH_TABLEH
/******************************************************************************/

struct match_reg_type {
	int reg;
	int type;
};

struct match_type_str {
	int type;
	const char *str;
};

int reg2type(struct match_reg_type *table, int length, int reg, int def);

int type2reg(struct match_reg_type *table, int length, int type, int def);

int str2type(struct match_type_str *table, int length, const char *str,
	     int size, int def);

const char *type2str(struct match_type_str *table, int length, int type,
		     const char *def);

/******************************************************************************/
#endif /* MATCH_TABLEH */
