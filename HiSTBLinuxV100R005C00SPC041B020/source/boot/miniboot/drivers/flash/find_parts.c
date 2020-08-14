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

#include <stddef.h>
#include <string.h>
#include <debug.h>

/******************************************************************************/
/* 0 - not find, 1 - find, -1 - error. */
static int find_part(char *s, char **retptr, char *ptnname, 
		     uint64 *start, uint64 *length)
{
	int isfind = 0;
	char delim = 0;

	/* fetch the partition size */
	if (*s == '-') {
		/* assign all remaining space to this partition */
		(*length) = (int64)(-1);
		s++;
	} else {
		(*length) = (uint64)memparse(s, &s);
		if ((*length) < 8192) {
		pr_error("partition size too small (%llx)\n", (*length));
		return -1;
		}
	}

	/* check for offset */
	if (*s == '@') {
		s++;
		memparse(s, &s);
	}

	/* now look for name */
	if (*s == '(')
		delim = ')';

	if (delim) {
		char *p;
		char *name;

		name = ++s;
		p = strchr(name, delim);
		if (!p) {
			pr_error("no closing %c found in partition name\n", delim);
			return -1;
		}

		*p = '\0';
		if (!strncmp(name, ptnname, 
			     strlen(name) > strlen(ptnname) 
			     ? strlen(name) : strlen(ptnname)))
		    isfind = 1;
		*p = ')';
		s = p + 1;
	}

	/* test for options */
	if (strncmp(s, "ro", 2) == 0)
		s += 2;

	/* if lk is found do NOT unlock the MTD partition*/
	if (strncmp(s, "lk", 2) == 0)
		s += 2;

	/* test if more partitions are following */
	if (*s == ',') {
		int ret;

		if ((*length) == (uint64)(-1)) {
			pr_error("no partitions allowed after a "
				 "fill-up partition\n");
			return -1;
		}

		if (isfind)
			return 1;

		(*start) += (*length);
		ret = find_part(s + 1, &s, ptnname, start, length);
		*retptr = s;
		return ret;
	}

	*retptr = s;
	return isfind;
}

/* 0 - find, -1 - no find */
int find_flash_part(char *mtdparts, char *media_name, char *ptn_name, 
		    uint64 *start, uint64 *length)
{
	int rel;
	char *s = (char *)mtdparts;

	for( ; s != NULL; ) {
	        char *p;
		(*start) = 0;

		/* fetch <mtd-id> */
		if (0 == (p = strchr(s, ':'))) {
			pr_error("no mtd-id\n");
			return -1;
		} else {
			*p = '\0';
			rel = strncmp(s, media_name, 
				      strlen(s) > strlen(media_name) 
				      ? strlen(s) : strlen(media_name));
			*p = ':';
			if (rel) {
				if (0 == (p = strchr(s, ';')))
					return -1;
				s = p + 1;
				continue;
			}
		}

		/*
		* parse one mtd. have it reserve memory for the
		* struct cmdline_mtd_partition and the mtd-id string.
		*/
		rel =find_part(p + 1, &s, ptn_name, start, length);

		if (rel == 1)
			return 0;

		if (rel == -1)
			return -1;

		/* EOS - we're done */
		if (*s == 0)
	            break;

		/* does another spec follow? */
		if (*s != ';')
	            return -1;

		s++;
	}

	return -1;
}