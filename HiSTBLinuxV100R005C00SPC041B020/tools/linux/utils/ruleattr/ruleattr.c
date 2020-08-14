/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
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
 * Create By Cai Zhiyong 2015.2.13
 *
******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "capability.h"

#ifdef WIN32
#  include <io.h>
#  include <direct.h>
#  define PATH_MAX  4096
#  pragma warning(disable : 4996)
#  define lstat stat
#else
#  include <unistd.h>
#  include <sys/param.h>
#endif /* WIN32 */

#include "ruleattr.h"

/******************************************************************************/

struct match {
	int num;
	const char *str;
};

#define FA_MODE_MASK  (S_IRWXO | \
                       S_IRWXG | \
                       S_IRWXU | \
                       S_ISVTX | S_ISGID | S_ISUID)

struct fileattr {
	unsigned short mode;
	short gid;
	short uid;

#define FA_MODE      0x01
#define FA_GID       0x02
#define FA_UID       0x04
#define FA_CAP       0x08
	unsigned short fa_flags;

	struct vfs_cap_data cap;
};

struct fileprefix {
	char *name;
	int sz_name;

	/* if it has escaped characters, alias is malloc, else alias is name, not include `*` */
	char *alias;
	int sz_alias;

/* name strings has escaped character, such as a**b, it means "a*b" */
#define FP_ESCAPED        0x02
/* name strings has wild-card character, such as /bin* */
#define FP_WILDCARD       0x01
	short flags;
};
#define IS_PREFIX_WILDCARD(_prefix) ((_prefix)->flags & FP_WILDCARD)
#define IS_PREFIX_ESCAPED(_prefix)  ((_prefix)->flags & FP_ESCAPED)

struct filepath {
	struct fileprefix prefix[PATH_MAX];
	int nr_prefix;

	char buf[PATH_MAX];
	char *pfree;
};

struct filenode {
	struct fileprefix prefix;

	struct filenode *attrnode;
	struct fileattr *attr;
/*
 * assign  - attrnode = NULL, attr,rule,linenum != NULL
 * wildcard|inheritable - attrnode != NULL, attr,rule,linenum = NULL
 */
#define FN_ASSIGN         0x01
#define FN_WILDCARD       0x02
#define FN_INHERITABLE    0x03
	int flags;

	char *rule;
	int linenum;

	struct filenode *child, *sibling, *parent;
};
/******************************************************************************/

#define RA_CAP_MASK0           0xFFFFFFFFU
#define RA_CAP_MASK1           0x1FU

#define RA_CAP_LAST_CAP        CAP_BLOCK_SUSPEND
/* select all capability */
#define RA_CAP_FULL            (RA_CAP_LAST_CAP + 1)
/* do not select any capability */
#define RA_CAP_NONE            (RA_CAP_LAST_CAP + 2)
/* use file host capability */
#define RA_CAP_HOST            (RA_CAP_LAST_CAP + 3)

/* cat /proc/sys/kernel/cap_last_cap */
static struct match captable[] = {
	{RA_CAP_FULL,          "full"},
	{RA_CAP_NONE,          "none"},
	{RA_CAP_HOST,          "host"},

	{CAP_CHOWN,            "chown"},
	{CAP_DAC_OVERRIDE,     "dac_override"},
	{CAP_DAC_READ_SEARCH,  "dac_read_search"},
	{CAP_FOWNER,           "fowner"},
	{CAP_FSETID,           "fsetid"},
	{CAP_KILL,             "kill"},
	{CAP_SETGID,           "setgid"},
	{CAP_SETUID,           "setuid"},
	{CAP_SETPCAP,          "setpcap"},
	{CAP_LINUX_IMMUTABLE,  "linux_immutable"},
	{CAP_NET_BIND_SERVICE, "net_bind_service"},
	{CAP_NET_BROADCAST,    "net_broadcast"},
	{CAP_NET_ADMIN,        "net_admin"},
	{CAP_NET_RAW,          "net_raw"},
	{CAP_IPC_LOCK,         "ipc_lock"},
	{CAP_IPC_OWNER,        "ipc_owner"},
	{CAP_SYS_MODULE,       "sys_module"},
	{CAP_SYS_RAWIO,        "sys_rawio"},
	{CAP_SYS_CHROOT,       "sys_chroot"},
	{CAP_SYS_PTRACE,       "sys_ptrace"},
	{CAP_SYS_PACCT,        "sys_psacct"},
	{CAP_SYS_ADMIN,        "sys_admin"},
	{CAP_SYS_BOOT,         "sys_boot"},
	{CAP_SYS_NICE,         "sys_nice"},
	{CAP_SYS_RESOURCE,     "sys_resource"},
	{CAP_SYS_TIME,         "sys_time"},
	{CAP_SYS_TTY_CONFIG,   "sys_tty_config"},
	{CAP_MKNOD,            "mknod"},
	{CAP_LEASE,            "lease"},
	{CAP_AUDIT_WRITE,      "audit_write"},
	{CAP_AUDIT_CONTROL,    "audit_control"},
	{CAP_SETFCAP,          "setfcap"},
	{CAP_MAC_OVERRIDE,     "mac_override"},
	{CAP_MAC_ADMIN,        "mac_admin"},
	{CAP_SYSLOG,           "syslog"},
	{CAP_WAKE_ALARM,       "wake_alarm"},
	{CAP_BLOCK_SUSPEND,    "block_suspend"},
	{-1, NULL},
};

struct ruleattr_gobal {
	char *rulefile;

	char *rulelist;
	int sz_rulelist;

	struct filenode root;

	char rootdir[PATH_MAX];
	int sz_rootdir;

	FILE *fl_show_warning;
	FILE *fl_dump_rules;
	FILE *fl_dump_nodes;
	FILE *fl_dump_match;
};

static struct ruleattr_gobal g = {0};

/******************************************************************************/

#define IS_INIT()        (g.rulelist != NULL)

#define SKIP_ONE_PREFIX(_next, _first) \
	for (_next = (_first); *_next && *_next != '\\' && *_next != '/'; _next++);

#define SKIP_ONE_N_PREFIX(_next, _first, _tail) \
	for (_next = (_first); _next < (_tail) && *_next != '\\' && *_next != '/'; _next++);

#define SKIP_N_SPACE(_next, _first, _tail) \
	for (_next = (_first); _next < (_tail) && isspace(*_next); _next++);

#define GOTO_NEXT_PREFIX(_next, _first) \
	for (_next = (_first); *_next && (*_next == '\\' || *_next == '/'); _next++);

#define GOTO_NEXT_N_PREFIX(_next, _first, _tail) \
	for (_next = (_first); _next < (_tail) && (*_next == '\\' || *_next == '/'); _next++);

/******************************************************************************/

#if 1

static int nr_malloc = 0;
static void _dbg_free(void *ptr)
{
	nr_malloc--;
	free(ptr);
}

static void *_dbg_malloc(unsigned int size)
{
	nr_malloc++;
	return malloc(size);
}

char *_dbg_strdup(const char *str)
{
	nr_malloc++;
	return strdup(str);
}

#  define CHECK_MALLOC    ASSERT(nr_malloc == 0)
#  define ASSERT(_p) if (!(_p)) { \
	printf("%s(%d): assert: (%s)\n", __FILE__, __LINE__, #_p); while (1);}
#else
#  define _dbg_free free
#  define _dbg_malloc malloc
#  define _dbg_strdup strdup
#  define CHECK_MALLOC
#  define ASSERT(_p)
#endif

/******************************************************************************/

static char *cap2hstr(int type)
{
	struct match *cap = captable;

	while (cap->str) {
		if (cap->num == type)
			return (char *)cap->str;
		cap++;
	}

	return NULL;
}
/******************************************************************************/

static int hstr2cap(const char *str, int len)
{
	struct match *cap = captable;

	if (!*str || !len)
		return -EINVAL;

	while (cap->str) {
		if (!strncmp(cap->str, str, len))
			return cap->num;
		cap++;
	}

	return -ENOSPC;
}
/******************************************************************************/

static int have_permitted(struct vfs_cap_data *capdata, unsigned int captype)
{
	unsigned int idx;

	if (captype > 31) {
		idx = captype >> 5;
		captype %= 32;
	} else
		idx = 0;

	return ((1 << captype) & capdata->data[idx].permitted) ? 1 : 0;
}
/******************************************************************************/

static int insert_permitted(struct vfs_cap_data *cap_data, unsigned int captype)
{
	unsigned int idx;

	if (captype > 31) {
		idx = captype >> 5;
		captype %= 32;
	} else
		idx = 0;

	cap_data->data[idx].permitted |= 1 << captype;

	return 0;
}
/******************************************************************************/

static void fill_permitted(struct vfs_cap_data *capdata)
{
	capdata->data[0].permitted = RA_CAP_MASK0;
	capdata->data[1].permitted = RA_CAP_MASK1;
}
/******************************************************************************/
/*
 *  0 - none
 *  1 - full
 * -1 - partial
 */
static int permitted_status(struct vfs_cap_data *capdata)
{
	unsigned int permitted0 = capdata->data[0].permitted;
	unsigned int permitted1 = capdata->data[1].permitted;

	if (!(permitted0 | permitted1))
		return 0;

	if (permitted0 == RA_CAP_MASK0 && permitted1 == RA_CAP_MASK1)
		return 1;

	return -1;
}
/******************************************************************************/

static int read_rulefile(const char *rulefile)
{
	int ret = -EBADF;
	int fd;

	fd = open(rulefile, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open `%s` fail. %s\n", rulefile, strerror(errno));
		return -EBADF;
	}

	g.sz_rulelist = read(fd, g.rulelist, g.sz_rulelist);
	if (g.sz_rulelist < 0) {
		fprintf(stderr, "read `%s` fail. %s\n", rulefile, strerror(errno));
		goto out;
	}

	g.rulelist[g.sz_rulelist++] = 0x0A;
	ret = 0;

out:
	close(fd);
	return ret;
}
/******************************************************************************/

static char *goto_next_mark(const char *str, const char *ptail, char mark)
{
	char *s = (char *)str;

	while (s < ptail && *s != mark)
		s++;

	return s;
}
/******************************************************************************/

static void pr_warning(int linenum, int column, char *rule, const char *fmt,
		       ...)
{
	va_list ap;

	if (!g.fl_show_warning)
		return;

	fprintf(g.fl_show_warning, "\n%s:%d: warning: ", g.rulefile, linenum);

	va_start(ap, fmt);
	vfprintf(g.fl_show_warning, fmt, ap);
	va_end(ap);

	fprintf(g.fl_show_warning, "\n");

	if (column >= 0) {
		fprintf(g.fl_show_warning, "  %s\n", rule);
		while (column-- > 0)
			fputc(' ', g.fl_show_warning);
		fprintf(g.fl_show_warning, "  ^\n");
	}
}
/******************************************************************************/

static char *skip_one_capability(const char *str, const char *ptail)
{
	char *s = (char *)str;

	while (s < ptail && (isalpha(*s) || *s == '_'))
		s++;

	return s;
}
/******************************************************************************/

static char *goto_next_capability(const char *str, const char *ptail)
{
	char *s = (char *)str;

	while (s < ptail && (*s == ',' || *s == ';' || isspace(*s)))
		s++;

	return s;
}
/******************************************************************************/

static int resolve_filecap(char *rule, int capoff, int sz_cap, int linenum,
			   struct fileattr *attr)
{
	int nr_cap = 0;
	int capstat = 0;
	struct vfs_cap_data *cap = &attr->cap;
	char *ptr = rule + capoff;
	char *ptail = ptr + sz_cap;

	attr->fa_flags |= FA_CAP;
	memset(cap, 0, sizeof(struct vfs_cap_data));

	ptr = goto_next_capability(ptr, ptail);

	while (ptr < ptail) {
		int captype;
		char *chr, oldchr;

		chr = skip_one_capability(ptr, ptail);

		oldchr = *chr;
		*chr = 0;
		captype = hstr2cap(ptr, chr - ptr);
		*chr = oldchr;

		switch (captype) {
		case -ENOSPC:
			pr_warning(linenum, ptr - rule, rule, "unknown capability");

			break;

		case -EINVAL:
			pr_warning(linenum, ptr - rule, rule, "found unexpected character `%c`", oldchr);
			chr++;

			break;

		case RA_CAP_FULL:
			if (nr_cap || capstat)
				pr_warning(linenum, ptr - rule, rule,
					   "`%s` can't merge with any other capability", cap2hstr(RA_CAP_FULL));

			capstat = RA_CAP_FULL;
			fill_permitted(cap);

			break;

		case RA_CAP_NONE:
			if (nr_cap || capstat)
				pr_warning(linenum, ptr - rule, rule,
					   "`%s` can't merge with any other capability", cap2hstr(RA_CAP_NONE));

			capstat = RA_CAP_NONE;
			memset(cap, 0, sizeof(struct vfs_cap_data));

			break;

		case RA_CAP_HOST:
			if (nr_cap || capstat)
				pr_warning(linenum, ptr - rule, rule,
					   "`%s` can't merge with any other capability", cap2hstr(RA_CAP_HOST));

			capstat = RA_CAP_HOST;
			attr->fa_flags &= ~FA_CAP;

			break;

		default:
			if (capstat)
				pr_warning(linenum, ptr - rule, rule,
					   "`%s` can't merge with any other capability", cap2hstr(capstat));

			nr_cap++;

			insert_permitted(cap, captype);

			break;
		}

		ptr = goto_next_capability(chr, ptail);
	}

	cap->magic_etc = VFS_CAP_REVISION | VFS_CAP_FLAGS_EFFECTIVE;

	return 0;
}
/******************************************************************************/

static char *skip_one_rule_prefix(const char *str, const char *ptail,
				  char **pwildcard, int *nr_escaped)
{
	char *s = (char *)str;

	*pwildcard = NULL;
	*nr_escaped = 0;

	while (s < ptail && *s != '\\' && *s != '/') {
		if (s[0] == '*') {
			if (s + 1 == ptail || s[1] != '*') {
				if (!*pwildcard)
					*pwildcard  = s;
			} else {
				(*nr_escaped)++;
				s++;
			}
		}
		s++;
	}

	return s;
}
/******************************************************************************/

static char *normalize_perfix(struct filepath *path, struct fileprefix *prefix)
{
	char *s1;
	char *s0 = prefix->name;
	char *ptail0 = prefix->name + prefix->sz_name;

	prefix->alias = s1 = path->pfree;

	while (s0 < ptail0) {
		ASSERT(s1 < path->buf + PATH_MAX);

		if (*s0 != '*') {
			*s1++ = *s0++;
		} else {
			if ((s0 + 1) == ptail0) {
				*s1++ = *s0++;
			} else {
				ASSERT(s0[1] == '*');

				*s1++ = '*';
				s0 += 2;
			}
		}
	}

	prefix->sz_alias = s1 - path->pfree;
	path->pfree = s1;

	return prefix->alias;
}
/******************************************************************************/

static int resolve_rule_filepath(char *rule, int nameoff, int sz_name,
				 int linenum, struct filepath *path)
{
	char *chr;
	char *pwildcard = NULL;
	int nr_escaped = 0;
	int nr_prefix = 0;
	struct fileprefix *prefix = NULL;
	char *ppre_wildcard = NULL;
	char *ptr = rule + nameoff;
	char *ptail = ptr + sz_name;

	if (sz_name >= PATH_MAX) {
		pr_warning(linenum, ptr - rule, rule, "filename is too long");
		goto fail;
	}

	GOTO_NEXT_N_PREFIX(ptr, ptr, ptail);

	path->pfree = path->buf; /* for alias used. */

	while (ptr < ptail) {
		chr = skip_one_rule_prefix(ptr, ptail, &pwildcard, &nr_escaped);

		ASSERT(chr != ptr);

		if (nr_prefix >= PATH_MAX) {
			pr_warning(linenum, ptr - rule, rule, "too many directory");
			goto fail;
		}

		if (ppre_wildcard) {
			pr_warning(linenum, ppre_wildcard - rule, rule,
				"`*` should not in the middle of path field");
			goto fail;
		}

		prefix = &path->prefix[nr_prefix++];
		prefix->flags = 0;

		if (pwildcard) {
			ppre_wildcard = pwildcard;

			if (pwildcard +1 != chr) {
				pr_warning(linenum, pwildcard - rule, rule,
					"`*` can't in the middle of filename word, it should in the tail of filename");
				goto fail;
			}

			prefix->flags |= FP_WILDCARD;
		}

		prefix->name = ptr;
		prefix->sz_name = chr - ptr;

		if (nr_escaped) {
			prefix->flags |= FP_ESCAPED;
			normalize_perfix(path, prefix);
		} else {
			prefix->alias = prefix->name;
			prefix->sz_alias = prefix->sz_name;
		}

		if (IS_PREFIX_WILDCARD(prefix))
			prefix->sz_alias--;

		GOTO_NEXT_N_PREFIX(ptr, chr, ptail);
	}

	if (!nr_prefix) {
		pr_warning(linenum, ptr - rule, rule, "filename is invalid");
		goto fail;
	}

	path->nr_prefix = nr_prefix;

	return 0;
fail:
	return -EINVAL;
}
/******************************************************************************/

static int parse_mode(const char *s, char **next, unsigned short *mode,
		      char *rule, int sz_rule, int linenum)
{
	char *ptail;
	unsigned long tmp;

	if ((unsigned char)(*s - '0') >= 8) {
		pr_warning(linenum, s - rule, rule, "invalid mode");
		return -EINVAL;
	}

	tmp = strtoul(s, &ptail, 8);
	if (*ptail && !isspace(*ptail)) {
		pr_warning(linenum, s - rule, rule, "invalid mode");
		return -EINVAL;
	}

	if (tmp > 07777U) {
		pr_warning(linenum, s - rule, rule, "mode out of range");
		return -EINVAL;
	}

	*mode = (unsigned short)tmp;
	*next = ptail;

	return 0;
}
/******************************************************************************/

static int read_one_rule(char *rule, int sz_rule, int linenum,
			 struct fileattr *attr, struct filepath *path)
{
	int ret;
	char *ptr;
	char mark;
	char *chr = rule;

	attr->fa_flags = 0;

	ret = parse_mode(chr, &chr, &attr->mode, rule, sz_rule, linenum);
	if (ret)
		return ret;
	attr->fa_flags |= FA_MODE;

	/* deal uid */
	SKIP_N_SPACE(chr, chr, rule + sz_rule);
	if (*chr < '0' || *chr > '9') {
		pr_warning(linenum, chr - rule, rule, "uid invalid");
		return -EINVAL;
	}
	attr->uid = (short)strtol(chr, &chr, 10);
	attr->fa_flags |= FA_UID;

	/* deal ':' */
	if (*chr != ':') {
		pr_warning(linenum, chr - rule, rule, "need `:` between uid and gid");
		return -EINVAL;
	}
	chr++;

	/* deal gid */
	if (*chr < '0' || *chr > '9') {
		pr_warning(linenum, chr - rule, rule, "gid invalid");
		return -EINVAL;
	}
	attr->gid = (short)strtol(chr, &chr, 10);
	attr->fa_flags |= FA_GID;

	/* deal file capability */
	SKIP_N_SPACE(chr, chr, rule + sz_rule);
	mark = *chr;
	if (mark != '"' && mark != '\'') {
		pr_warning(linenum, chr - rule, rule, "file capability should start with `\"` or `'`");
		return -EINVAL;
	}

	ptr = ++chr;

	chr = goto_next_mark(chr, rule + sz_rule, mark);
	if (*chr != mark) {
		pr_warning(linenum, chr - rule, rule, "file capability should ending with `%c`", mark);
		return -EINVAL;
	}

	if (resolve_filecap(rule, ptr - rule, chr - ptr, linenum, attr))
		return -EINVAL;

	ptr = ++chr;

	/* deal file prefix */
	SKIP_N_SPACE(chr, chr, rule + sz_rule);
	mark = *chr;
	if (mark != '"' && mark != '\'') {
		pr_warning(linenum, chr - rule, rule, "filename should start with `\"` or `'`");

		return -EINVAL;
	}

	ptr = ++chr;

	chr = goto_next_mark(chr, rule + sz_rule, mark);
	if (*chr != mark) {
		pr_warning(linenum, chr - rule, rule, "filename should ending with `%c`", mark);

		return -EINVAL;
	}

	if (resolve_rule_filepath(rule, ptr - rule, chr - ptr, linenum, path))
		return -EINVAL;

	return 0;
}
/******************************************************************************/

static void pr_fileattr(FILE *file, struct fileattr *attr, int capwidth)
{
	int ret;
	int width;

	if (!attr) {
		width = 0;
		capwidth += 16;
		goto exit;
	}

	if (attr->fa_flags & FA_MODE)
		fprintf(file, "0%04o ", attr->mode);
	else
		fprintf(file, "host  ");

	if (attr->fa_flags & FA_UID)
		fprintf(file, "%4d:", attr->uid);
	else
		fprintf(file, "host:");

	if (attr->fa_flags & FA_GID)
		fprintf(file, "%-4d ", attr->gid);
	else
		fprintf(file, "host ");

	if (!(attr->fa_flags & FA_MODE)) {
		width = fprintf(file, "\"host\"");
		goto exit;
	}

	ret = permitted_status(&attr->cap);
	if (ret == 1)
		width = fprintf(file, "\"full\"");
	else if (ret == 0)
		width = fprintf(file, "\"none\"");
	else {
		int ix;
		int first = 0;

		width = fprintf(file, "\"");

		for (ix = 0; ix <= RA_CAP_LAST_CAP; ix++) {
			if (!have_permitted(&attr->cap, ix))
				continue;

			if (first)
				width += fprintf(file, ",");

			width += fprintf(file, "%s", cap2hstr(ix));
			first = 1;
		}

		width += fprintf(file, "\" ");
	}
exit:
	for (; width < capwidth; width++)
		fputc(' ', file);
}
/******************************************************************************/

static void pr_filepath(FILE *file, struct filepath *path)
{
	int ix;
	char *ptail = NULL;
	char oldchr;
	int wildcard = 0;
	struct fileprefix *prefix;

	fprintf(file, "\"");
	prefix = path->prefix;
	for (ix = 0; ix < path->nr_prefix; ix++, prefix++) {
		ptail = prefix->name + prefix->sz_name;

		oldchr = *ptail;
		*ptail = 0;
		fprintf(file, "/%s", prefix->name);
		*ptail = oldchr;

		if (IS_PREFIX_WILDCARD(prefix))
			wildcard++;
	}
	fprintf(file, "\" ");

	if (!wildcard)
		return;

	fprintf(file, "    [\"");
	prefix = path->prefix;
	for (ix = 0; ix < path->nr_prefix; ix++, prefix++) {
		ptail = prefix->alias + prefix->sz_alias;

		oldchr = *ptail;
		*ptail = 0;
		fprintf(file, "/%s", prefix->alias);
		*ptail = oldchr;
	}
	fprintf(file, "\"] ");

	if (wildcard)
		fprintf(file, "wildcard:%d ", wildcard);
}
/******************************************************************************/

static void pr_one_rule(FILE *file, struct fileattr *attr,
			struct filepath *path)
{
	pr_fileattr(file, attr, 12);
	pr_filepath(file, path);

	fprintf(file, "\n");
}
/******************************************************************************/
#define PFC_LARGE          1
#define PFC_LARGE_EQUATE   2
#define PFC_EQUATE         0
#define PFC_SMALL         -1
#define PFC_SMALL_EQUATE  -2

static int prefixcmp(const struct fileprefix *src, const struct fileprefix *dst,
		     int *nr_match)
{
	char *s0 = src->alias;
	char *s1 = dst->alias;
	int sz_src = src->sz_alias;
	int sz_dst = dst->sz_alias;
	int sz_alias;
	int count;

	sz_alias = sz_src;
	if (sz_src > sz_dst)
		sz_alias = sz_dst;

	while (sz_alias && *s0 == *s1) {
		sz_alias--;
		s0++;
		s1++;
	}

	count = s0 - src->alias;

	if (nr_match)
		*nr_match = count;

	if (sz_alias)
		return *s0 > *s1 ? PFC_LARGE : PFC_SMALL;

	sz_src -= count;
	sz_dst -= count;

	if (!sz_src) {
		if (!sz_dst) { /* dst = "*" or dst = "" */
			if (IS_PREFIX_WILDCARD(src)) { /* src = "*" */
				return IS_PREFIX_WILDCARD(dst) ? PFC_EQUATE : PFC_SMALL_EQUATE;
			} else { /* src = "" */
				return IS_PREFIX_WILDCARD(dst) ? PFC_LARGE_EQUATE : PFC_EQUATE;
			}
		} else {
			/* "*" <= "a", "" < "a" */
			return IS_PREFIX_WILDCARD(src) ? PFC_SMALL_EQUATE : PFC_SMALL;
		}
	}

	if (!sz_dst)
		return IS_PREFIX_WILDCARD(dst) ? PFC_LARGE_EQUATE : PFC_LARGE;

	ASSERT(0);
	return 0;
}
/******************************************************************************/
/*
 * return:
 *   0 - no found
 *   1 - found
 * node:
 *   return = 0: node which will be inserted next by prefix, or NULL it is the first node.
 *   return = 1: the found node
 * wcnode:
 *   return = 0 or 1, wcnode which wild-card match with prefix.
 *
 */
static int find_filenode(const struct filenode *parent,
			 const struct fileprefix *prefix,
			 struct filenode **node, struct filenode **wcnode)
{
	int max_match = -1;
	struct filenode *next;

	ASSERT(parent != NULL && prefix != NULL && node != NULL && wcnode != NULL);

	*node = NULL;
	*wcnode = NULL;

	for (next = parent->child; next; next = next->sibling) {
		int nr_match;

		switch (prefixcmp(&next->prefix, prefix, &nr_match)) {
		default:
			return 0;

		case PFC_SMALL:
			*node = next;
			break;

		case PFC_SMALL_EQUATE:
			*node = next;

			if (nr_match > max_match) {
				*wcnode = next;
				max_match = nr_match;
			}
			break;

		case PFC_EQUATE:
			*node = next;
			return 1;
		}
	}

	return 0;
}
/******************************************************************************/

static struct filenode *new_filenode(struct fileprefix *prefix,
				     struct filenode *parent,
				     struct filenode *prev)
{
	struct filenode *newnode;
	struct fileprefix *newprefix;
	int size = sizeof(struct filenode);

	if (IS_PREFIX_ESCAPED(prefix))
		size += prefix->sz_alias + 1;

	newnode = (struct filenode *)_dbg_malloc(size);
	if (!newnode) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return NULL;
	}

	newnode->child = newnode->sibling = NULL;
	newnode->parent = parent;

	/* new node inheritable parent node attribute */
	newnode->attrnode = parent;
	newnode->attr = NULL;
	newnode->flags = FN_INHERITABLE;
	newnode->linenum = -1;
	newnode->rule = NULL;

	newprefix = &newnode->prefix;

	newprefix->name = prefix->name;
	newprefix->sz_name = prefix->sz_name;
	newprefix->flags = prefix->flags;
	newprefix->sz_alias = prefix->sz_alias;

	if (IS_PREFIX_ESCAPED(prefix)) {
		newprefix->alias = (char *)&newnode[1];
		memcpy(newprefix->alias, prefix->alias, prefix->sz_alias);
	} else
		newprefix->alias = newprefix->name;

	if (prev) {
		newnode->sibling = prev->sibling;
		prev->sibling = newnode;
	} else {
		newnode->sibling = parent->child;
		parent->child = newnode;
	}

	return newnode;
}
/******************************************************************************/

static struct fileattr *new_fileattr(struct fileattr *attr)
{
	struct fileattr *newattr;

	newattr = (struct fileattr *)_dbg_malloc(sizeof(struct fileattr));
	if (!newattr) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return NULL;
	}

	memcpy(newattr, attr, sizeof(struct fileattr));

	return newattr;
}
/******************************************************************************/

static int insert_new_fileattr(char *rule, int linenum, struct filenode *node,
			       struct fileattr *attr)
{
	ASSERT(node->flags != 0);

	if (node->flags == FN_ASSIGN) {
		ASSERT(node->attrnode == NULL && node->attr != NULL && node->rule != NULL);

		pr_warning(linenum, -1, rule,
			"this rule conflict with previous rule, ignore\n  line %d: %s\n  line %d: %s",
			linenum, rule, node->linenum, node->rule);

		return -EEXIST;
	}

	ASSERT(node->attrnode != NULL && node->attr == NULL && node->rule == NULL);

	node->attr = new_fileattr(attr);
	if (!node->attr)
		return -ENOMEM;

	node->flags = FN_ASSIGN;
	node->rule = rule;
	node->linenum = linenum;
	node->attrnode = NULL;

	return 0;
}
/******************************************************************************/
/*
 * if next match prefix, modify next attr
 */
static void fix_sibling_attr(struct filenode *node, struct filenode *next)
{
	int ret;
	struct fileprefix *attr_prefix;
	struct fileprefix *src = &node->prefix;

	for (; next; next = next->sibling) {
		ret = prefixcmp(src, &next->prefix, NULL);
		if (ret != PFC_SMALL_EQUATE)
			return;

		if (next->flags == FN_ASSIGN)
			continue;

		if (next->flags == FN_INHERITABLE) {
			next->flags = FN_WILDCARD;
			next->attrnode = node;
			continue;
		}

		ASSERT(next->flags == FN_WILDCARD && next->attrnode != NULL);

		attr_prefix = &next->attrnode->prefix;

		if (attr_prefix->sz_alias > src->sz_alias)
			continue;

		next->attrnode = node;
		next->flags = FN_WILDCARD;
	}
}
/******************************************************************************/

static int insert_one_rule(char *rule, int linenum, struct fileattr *attr,
			   struct filepath *path)
{
	int ix;
	int ret;
	struct fileprefix *prefix;
	struct filenode *node, *wcnode;
	struct filenode *newnode;
	struct filenode *parent = &g.root;

	for (ix = 0; ix < path->nr_prefix; ix++) {
		prefix = &path->prefix[ix];

		ret = find_filenode(parent, prefix, &node, &wcnode);
		if (ret) {
			/* prefix is not the last field. */
			if (ix + 1 != path->nr_prefix) {
				parent = node;
				continue;
			}

			/* prefix is the last node */
			ret = insert_new_fileattr(rule, linenum, node, attr);
			if (ret == -EEXIST)
				ret = 0;

			return ret;
		}

		/* no found match node */
		newnode = new_filenode(prefix, parent, node);
		if (!newnode)
			return -ENOMEM;

		/* prefix is the last node */
		if (ix + 1 == path->nr_prefix) {
			ret = insert_new_fileattr(rule, linenum, newnode, attr);
			if (ret == -EEXIST)
				ret = 0;

			if (IS_PREFIX_WILDCARD(prefix) && newnode->sibling)
				fix_sibling_attr(newnode, newnode->sibling);

			return ret;
		}

		if (wcnode) {
			newnode->flags = FN_WILDCARD;
			newnode->attrnode = wcnode;
		}

		parent = newnode;
	}

	return 0;
}
/******************************************************************************/
/*
 * 0755 0:0 "none" "/usr/sbin"
 */
static int resolve_one_rule(char *rule, int sz_rule, int linenum,
			    struct fileattr *attr, struct filepath *path)
{
	int ret;

	if (!sz_rule || !*rule || *rule == '#')
		return 0;

	ret = read_one_rule(rule, sz_rule, linenum, attr, path);
	if (ret)
		return 0;

	if (g.fl_dump_rules)
		pr_one_rule(g.fl_dump_rules, attr, path);

	ret = insert_one_rule(rule, linenum, attr, path);
	if (ret)
		return ret;

	return 0;
}
/******************************************************************************/

static char *skip_lfcr(const char *str, const char *ptail, int *nr_line)
{
	char *s = (char *)str;

	while (s < ptail && (*s == 0x0D || *s == 0x0A)) {
		if (*s == 0x0A)
			(*nr_line)++;
		*s++ = 0;
	}

	return s;
}
/******************************************************************************/

static int resolve_rulelist(const char *rulelist, int sz_rulelist)
{
	int ret = 0;
	char *rule = NULL;
	char *chr = NULL;
	int linenum = 1;
	struct fileattr attr = {0};
	struct filepath *path = NULL;
	char *ptail = (char *)rulelist + sz_rulelist;

	path = (struct filepath *)_dbg_malloc(sizeof(struct filepath));
	if (!path) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return -ENOMEM;
	}

	for (rule = chr = (char *)rulelist; chr < ptail; chr++) {
		char *next;
		int nr_line = 0;

		if (*chr != 0x0D && *chr != 0x0A)
			continue;

		next = skip_lfcr(chr, ptail, &nr_line);

		SKIP_N_SPACE(rule, rule, chr);

		ret = resolve_one_rule(rule, chr - rule, linenum, &attr, path);
		if (ret)
			goto out;

		rule = chr = next;
		linenum += nr_line;
	}
	ret = 0;
out:
	_dbg_free(path);

	return ret;
}
/******************************************************************************/

static struct filenode *get_attrnode(struct filenode *node)
{
	while (node && node->flags != FN_ASSIGN)
		node = node->attrnode;

	return node;
}
/******************************************************************************/

static void pr_node_path(FILE *file, struct filenode **nodes, int level)
{
	int ix;

	for (ix = 0; ix <= level; ix++) {
		char oldchr, *ptail;
		struct fileprefix *prefix = &nodes[ix]->prefix;

		ptail = prefix->name + prefix->sz_name;

		oldchr = *ptail;
		*ptail = 0;
		fprintf(file, "/%s", prefix->name);
		*ptail = oldchr;
	}
}
/******************************************************************************/

static int pr_one_node(struct filenode **nodes, int level)
{
	int flags;
	const char *attrstr;
	struct filenode *node = nodes[level];
	struct filenode *attrnode = get_attrnode(node);
	FILE *file = g.fl_dump_nodes;

	if (attrnode)
		pr_fileattr(file, attrnode->attr, 12);
	else
		pr_fileattr(file, NULL, 12);

	flags = node->flags;
	switch (flags) {
	case FN_ASSIGN:
		attrstr = "assign";
		break;
	case FN_WILDCARD:
		attrstr = "wildcard";
		break;
	case FN_INHERITABLE:
		attrstr = "inheritable";
		break;
	default:
		attrstr = "unknown";
		break;
	}
	fprintf(file, "%-11s ", attrstr);

	fprintf(file, "\"");
	pr_node_path(file, nodes, level);
	fprintf(file, "\" ");

	fprintf(file, "\n");

	return 0;
}
/******************************************************************************/

static int enum_all_filenode(struct filenode *root,
			     int (*func)(struct filenode **, int))
{
	int level = -1;
	struct filenode **stacks;
	struct filenode *node = root;

	stacks = (struct filenode **)
		_dbg_malloc(sizeof(struct filenode *) * PATH_MAX);
	if (!stacks) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return -ENOMEM;
	}

	while (node) {
		if (node->child) {
			node = node->child;
			stacks[++level] = node;

			if (func(stacks, level))
				return -1;

			continue;
		}
next_sibling:
		if (node->sibling) {
			node = node->sibling;
			stacks[level] = node;

			if (func(stacks, level))
				return -1;

			continue;
		}

		if (!node->parent)
			break;

		node = node->parent;
		level--;

		goto next_sibling;
	}

	_dbg_free(stacks);

	return 0;
}
/******************************************************************************/

static int new_rulelist(const char *rulefile, int size)
{
	g.sz_rulelist = size;
	g.rulelist = (char *)_dbg_malloc(g.sz_rulelist + 2);
	if (!g.rulelist) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return -ENOMEM;
	}

	g.rulelist[g.sz_rulelist] = 0x0A;

	g.rulefile = _dbg_strdup(rulefile);
	if (!g.rulefile) {
		fprintf(stderr, "strdup fail. %s\n", strerror(errno));

		_dbg_free(g.rulelist);
		g.rulelist = NULL;

		return -ENOMEM;
	}

	return 0;
}
/******************************************************************************/

int ra_init_file(const char *rulefile, FILE *flwarning, FILE *flattrpath,
		 FILE *flnode, FILE *flmatch)
{
	int ret;
	struct stat buf;

	if (IS_INIT()) {
		fprintf(stderr, "rule_attr module was initiated.\n");
		return -EINVAL;
	}

	g.fl_show_warning = flwarning;
	g.fl_dump_rules = flattrpath;
	g.fl_dump_nodes = flnode;
	g.fl_dump_match = flmatch;

	ret = stat(rulefile, &buf);
	if (ret) {
		fprintf(stderr, "stat `%s` fail. %s\n", rulefile, strerror(errno));
		return ret;
	}

	if (buf.st_size < 4) {
		fprintf(stderr, "file `%s` is too short.\n", rulefile);
		return -1;
	}

	ret = new_rulelist(rulefile, buf.st_size);
	if (ret)
		return ret;

	ret = read_rulefile(rulefile);
	if (ret) {
		ra_free();
		return ret;
	}

	ret = resolve_rulelist(g.rulelist, g.sz_rulelist);

	if (g.fl_dump_nodes) {
		fprintf(g.fl_dump_nodes, "\nall nodes:\n");
		enum_all_filenode(&g.root, pr_one_node);
		fprintf(g.fl_dump_nodes, "\n");
	}

	return 0;
}
/******************************************************************************/

int ra_init_file_debuglevel(const char *rulefile, int debuglevel)
{
	FILE *flwarning = NULL;
	FILE *flattrpath = NULL;
	FILE *flnode = NULL;
	FILE *flmatch = NULL;

	if (debuglevel & 0x01)
		flwarning = stderr;
	if (debuglevel & 0x02)
		flmatch = stderr;
	if (debuglevel & 0x04)
		flattrpath = stderr;
	if (debuglevel & 0x08)
		flnode = stderr;

	return ra_init_file(rulefile, flwarning, flattrpath, flnode, flmatch);
}
/******************************************************************************/

int ra_init_data(const char *rulelist, int sz_rulelist, FILE *flwarning,
		 FILE *flattrpath, FILE *flnode, FILE *flmatch)
{
	int ret;

	if (IS_INIT()) {
		fprintf(stderr, "rule_attr module was initiated.\n");
		return -EINVAL;
	}

	g.fl_show_warning = flwarning;
	g.fl_dump_rules = flattrpath;
	g.fl_dump_nodes = flnode;
	g.fl_dump_match = flmatch;

	ret = new_rulelist("<rulelist>", sz_rulelist);
	if (ret)
		return ret;

	memcpy(g.rulelist, rulelist, sz_rulelist);
	g.rulelist[g.sz_rulelist++] = 0x0A;

	ret = resolve_rulelist(g.rulelist, g.sz_rulelist);

	if (g.fl_dump_nodes) {
		fprintf(g.fl_dump_nodes, "\nall nodes:\n");
		enum_all_filenode(&g.root, pr_one_node);
		fprintf(g.fl_dump_nodes, "\n");
	}

	return 0;
}
/******************************************************************************/

static struct fileattr *__find_fileattr(const char *path, int sz_path)
{
	int ret;
	char *ptr, *ptail;
	struct filenode *node;
	struct filenode *parent = &g.root;

	ret = 0;
	node = NULL;

	ptail = (char *)path + sz_path;
	GOTO_NEXT_N_PREFIX(ptr, (char *)path, ptail);

	while (ptr < ptail) {
		char *chr;
		struct filenode *wcnode;
		struct fileprefix prefix;

		SKIP_ONE_N_PREFIX(chr, ptr, ptail);

		prefix.flags = 0;
		prefix.name = prefix.alias = ptr;
		prefix.sz_name = prefix.sz_alias = chr - ptr;

		ret = find_filenode(parent, &prefix, &node, &wcnode);
		if (!ret) {
			node = wcnode == NULL ? parent : wcnode;
			break;
		}

		parent = node;
		GOTO_NEXT_N_PREFIX(ptr, chr, ptail);
	}

	if (g.fl_dump_match) {
		for (ptr = (char *)path; ptr < ptail; ptr++)
			fputc(*ptr, g.fl_dump_match);
		fputc('\n', g.fl_dump_match);
	}

	node = get_attrnode(node);
	if (!node) {
		if (g.fl_dump_match)
			fprintf(g.fl_dump_match, "    match: none\n");

		return NULL;
	} else {
		if (g.fl_dump_match)
			fprintf(g.fl_dump_match, "    match: (%d): %s\n", node->linenum, node->rule);

		return node->attr;
	}
}
/******************************************************************************/

static void free_one_filenode(struct filenode *node)
{
	if (node->attr)
		_dbg_free(node->attr);

	_dbg_free(node);
}
/******************************************************************************/

static void free_all_filenode(struct filenode *root)
{
	struct filenode *del;
	struct filenode *parent = root;
	struct filenode *node = parent->child;

	while (parent && node) {
		if (node->child) {
			parent = node;
			node = parent->child;
			continue;
		}
		del = node;

		parent->child = node->sibling;
		node = del->sibling;

		free_one_filenode(del);

		if (node)
			continue;

		node = parent;
		parent = node->parent;
	}
}
/******************************************************************************/

int ra_free(void)
{
	free_all_filenode(&g.root);

	if (g.rulelist) {
		_dbg_free(g.rulelist);
		g.rulelist = NULL;
	}

	if (g.rulefile) {
		_dbg_free(g.rulefile);
		g.rulefile = NULL;
	}

	CHECK_MALLOC;

	return 0;
}
/******************************************************************************/

static int resolve_path(const char *str, struct filepath *path)
{
	char *ptr;

	GOTO_NEXT_PREFIX(ptr, (char *)str);

	while (*ptr) {
		char *chr;
		struct fileprefix *prefix;

		SKIP_ONE_PREFIX(chr, ptr);

		if (path->nr_prefix >= PATH_MAX)
			return -EINVAL;

		if (ptr[0] != '.')
			goto save;

		if (chr == ptr + 1)
			goto next;

		if (chr == ptr + 2 && ptr[1] == '.') {
			if (path->nr_prefix)
				path->nr_prefix--;
			goto next;
		}

save:
		prefix = &path->prefix[path->nr_prefix++];
		prefix->name = ptr;
		prefix->sz_name = chr - ptr;

next:
		if (!*chr)
			break;

		GOTO_NEXT_PREFIX(ptr, chr);
	}

	return 0;
}
/******************************************************************************/

static int ra_path_normalize(const char *path, char *npath, int sz_npath)
{
	int ix;
	int ret;
	char *ptr = npath;
	struct filepath *filepath;

	filepath = (struct filepath *)_dbg_malloc(sizeof(struct filepath));
	if (!filepath) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return -ENOMEM;
	}

	filepath->nr_prefix = 0;

	if (path[0] != '\\' && path[0] != '/') {
		if (!getcwd(filepath->buf, sizeof(filepath->buf))) {
			fprintf(stderr, "getcwd fail. %s\n", strerror(errno));
			ret = -EINVAL;
			goto fail;
		}
		ret = resolve_path(filepath->buf, filepath);
		if (ret < 0)
			goto fail;
	}

	ret = resolve_path(path, filepath);
	if (ret < 0)
		goto fail;

	sz_npath--; /* for ending with '\0' */

	for (ix = 0; ix < filepath->nr_prefix; ix++) {
		struct fileprefix *prefix = &filepath->prefix[ix];

		if (sz_npath < prefix->sz_name + 1) {
			ret = -ERANGE;
			goto fail;
		}

		*ptr++ = '/';
		memcpy(ptr, prefix->name, prefix->sz_name);

		ptr += prefix->sz_name;
		sz_npath -= prefix->sz_name + 1;
	}

	*ptr = '\0';
	ret = ptr - npath;
fail:
	_dbg_free(filepath);
	return ret;
}
/******************************************************************************/

static int in_rootdir(const char *path, int sz_path)
{
	const char *ptr;

	if (sz_path <= g.sz_rootdir)
		return 1;

	ptr = path + g.sz_rootdir;

	if (*ptr != '\\' && *ptr != '/')
		return 1;

	if (memcmp(path, g.rootdir, g.sz_rootdir))
		return 1;

	return 0;
}
/******************************************************************************/

int ra_set_rootdir(const char *path)
{
	g.sz_rootdir = ra_path_normalize(path, g.rootdir, sizeof(g.rootdir));
	if (g.sz_rootdir < 0)
		g.sz_rootdir = 0;

	return !g.sz_rootdir;
}
/******************************************************************************/

static struct fileattr *find_fileattr(const char *path)
{
	char *npath;
	int sz_npath = PATH_MAX;
	struct fileattr *attr = NULL;

	npath = (char *)_dbg_malloc(PATH_MAX);
	if (!npath) {
		fprintf(stderr, "malloc fail. %s\n", strerror(errno));
		return NULL;
	}

	sz_npath = ra_path_normalize(path, npath, PATH_MAX);
	if (sz_npath < 0)
		goto fail;

	if (in_rootdir(npath, sz_npath))
		goto fail;

	attr = __find_fileattr(npath + g.sz_rootdir, sz_npath - g.sz_rootdir);
fail:
	_dbg_free(npath);

	return attr;
}
/******************************************************************************/

int ra_host_attr(const char *path, void **attr)
{
	struct fileattr *_attr;

	*attr = NULL;

	if (!IS_INIT() || !path)
		return -1;

	_attr = find_fileattr(path);
	if (!_attr)
		return -2;

	*attr = (void *)_attr;

	return 0;
}
/******************************************************************************/

int ra_sys_lstat(const char *path, struct stat *buf)
{
	int ret;
	void *attr;

	ret = lstat(path, buf);
	if (ret)
		return ret;

	ret = ra_host_attr(path, &attr);
	if (!ret)
		ra_attr_stat(attr, buf);

	return 0;
}
/******************************************************************************/

int ra_target_attr(const char *path, int sz_path, void **attr)
{
	struct fileattr *_attr;

	*attr = NULL;

	if (!IS_INIT() || !path)
		return -1;

	_attr = __find_fileattr(path, sz_path);
	if (!_attr)
		return -2;

	*attr = (void *)_attr;

	return 0;
}
/******************************************************************************/

int ra_attr_cap(void *attr, void *data)
{
	struct fileattr *_attr = (struct fileattr *)attr;

	if (!_attr || !(_attr->fa_flags & FA_CAP))
		return -1;

	if (data)
		memcpy(data, &_attr->cap, sizeof(struct vfs_cap_data));

	return sizeof(struct vfs_cap_data);
}
/******************************************************************************/

int ra_attr_stat(void *attr, struct stat *buf)
{
	struct fileattr *_attr = (struct fileattr *)attr;

	if (!_attr)
		return -1;

	if (_attr->fa_flags & FA_GID)
		buf->st_gid = _attr->gid;
	if (_attr->fa_flags & FA_UID)
		buf->st_uid = _attr->uid;
	if ((_attr->fa_flags & FA_MODE) && (!S_ISLNK(buf->st_mode))) {
		buf->st_mode &= ~FA_MODE_MASK;
		buf->st_mode |= (_attr->mode & FA_MODE_MASK);
	}

	return 0;
}
/******************************************************************************/

int ra_target_stat(const char *path, int sz_path, struct stat *buf)
{
	int ret;
	void *attr;

	ret = ra_target_attr(path, sz_path, &attr);
	if (ret)
		return ret;

	return ra_attr_stat(attr, buf);
}
/******************************************************************************/

int ra_attr_stat_cap(void *attr, short *gid, short *uid, unsigned short *mode,
		     unsigned int permitted[2])
{
	struct fileattr *_attr = (struct fileattr *)attr;

	if (!_attr)
		return -1;

	if (gid && (_attr->fa_flags & FA_GID))
		*gid = _attr->gid;

	if (uid && (_attr->fa_flags & FA_UID))
		*uid = _attr->uid;

	if (mode && (_attr->fa_flags & FA_MODE)) {
		*mode &= ~FA_MODE_MASK;
		*mode |= (_attr->mode & FA_MODE_MASK);
	}

	if (permitted && (_attr->fa_flags & FA_CAP)) {
		permitted[0] = _attr->cap.data[0].permitted;
		permitted[1] = _attr->cap.data[1].permitted;
	}

	return 0;
}
