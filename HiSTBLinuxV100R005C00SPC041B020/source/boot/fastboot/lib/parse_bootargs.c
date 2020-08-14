/******************************************************************************
 *    Reserve memory manager
 *    Copyright (c) 2013-2014 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Lai Yingjun.
 *
 ******************************************************************************/

#include <config.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <asm/types.h>
#include <asm/sizes.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 *	memparse - parse a string with mem suffixes into a number
 *	@ptr: Where parse begins
 *	@retptr: (output) Optional pointer to next char after parse completes
 *
 *	Parses a string into a number.  The number stored at @ptr is
 *	potentially suffixed with %K (for kilobytes, or 1024 bytes),
 *	%M (for megabytes, or 1048576 bytes), or %G (for gigabytes, or
 *	1073741824).  If the number is suffixed with K, M, or G, then
 *	the return value is the number multiplied by one kilobyte, one
 *	megabyte, or one gigabyte, respectively.
 */

unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long long ret =
		(unsigned long long)simple_strtoul(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G':
	case 'g':
		ret <<= 10;
	case 'M':
	case 'm':
		ret <<= 10;
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

/**
 * skip_spaces - Removes leading whitespace from @str.
 * @str: The string to be stripped.
 *
 * Returns a pointer to the first non-whitespace character in @str.
 */
char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}

static char dash2underscore(char c)
{
	if (c == '-')
		return '_';
	return c;
}

int parameqn(const char *a, const char *b, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (dash2underscore(a[i]) != dash2underscore(b[i]))
			return 0;
	}
	return 1;
}

int parameq(const char *a, const char *b)
{
	return parameqn(a, b, strlen(a) + 1);
}

/* You can use " around spaces, but can't escape ". */
/* Hyphens and underscores equivalent in parameter names. */
static char *next_arg(char *args, char **param, char **val)
{
	unsigned int i, equals = 0;
	int in_quote = 0, quoted = 0;
	char *next;

	if (*args == '"') {
		args++;
		in_quote = 1;
		quoted = 1;
	}

	for (i = 0; args[i]; i++) {
		if (isspace(args[i]) && !in_quote)
			break;
		if (equals == 0) {
			if (args[i] == '=')
				equals = i;
		}
		if (args[i] == '"')
			in_quote = !in_quote;
	}

	*param = args;
	if (!equals)
		*val = NULL;
	else {
		args[equals] = '\0';
		*val = args + equals + 1;

		/* Don't include quotes in value. */
		if (**val == '"') {
			(*val)++;
			if (args[i - 1] == '"')
				args[i - 1] = '\0';
		}
		if (quoted && args[i - 1] == '"')
			args[i - 1] = '\0';
	}

	if (args[i]) {
		args[i] = '\0';
		next = args + i + 1;
	} else
		next = args + i;

	/* Chew up trailing spaces. */
	return skip_spaces(next);
}
char *get_mem_val(char *bootargs_env)
{
	char *args = NULL;
	static char bootargs[1024] = { 0 };
	char *param = NULL, *val = NULL;
	char *env = NULL;

	if (NULL == bootargs_env)
		goto error_out;

	env = getenv(bootargs_env);
	if (!env)
		goto error_out;

	memcpy(bootargs, env, sizeof(bootargs));
	bootargs[sizeof(bootargs) - 1] = '\0';
	args = bootargs;

	/* Chew leading spaces */
	args = skip_spaces(args);

	while (*args) {
		args = next_arg(args, &param, &val);

		if (parameq("mem", param))
			break;
	}

	if (NULL == param)
		goto error_out;

	if (parameq("mem", param) == 0)
		goto error_out;

	return val;
error_out:
	return NULL;

}
extern unsigned int get_ddr_size(void);
/* Args looks like "foo=bar,bar2 baz=fuz wiz". */
int parse_mem_args(unsigned long *ret_start_addr, unsigned long *ret_size)
{
	char *val = NULL;
	char *endp;
	unsigned long start;
	unsigned long long size;
	char bootargs_ddr[32];

	if (NULL == ret_start_addr || NULL == ret_size)
		goto error_out;

	val = get_mem_val("bootargs");
	if (!val) {
		size = get_ddr_size();
		snprintf(bootargs_ddr, sizeof(bootargs_ddr), "bootargs_%s",
		 	ultohstr((unsigned long long)size));
		val = get_mem_val(bootargs_ddr);
		if (!val)
			val = get_mem_val("bootargs");

		if (!val)
			goto error_out;
	}

	start = CFG_DDR_PHYS_OFFSET;
	size = memparse(val, &endp);
	if (*endp == '@')
		start = memparse(endp + 1, NULL);

	*ret_start_addr = start;
	*ret_size = size;

	return 0;
error_out:
	return -1;
}
