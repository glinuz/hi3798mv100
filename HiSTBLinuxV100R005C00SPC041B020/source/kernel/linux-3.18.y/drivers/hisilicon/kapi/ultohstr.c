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

#include <linux/kernel.h>

char *ultohstr(u64 size, char *str, int len)
{
	char *unit = "";
	int rr = 0;
	unsigned int mm = 0;
	const char *c;
	const char fmt[] = " KMGTPE";
	unsigned int nn = (unsigned int)(size & 0x3FF);

	if (size < 1024ULL) {
		snprintf(str, len, "%u%s", nn, unit);
		return str;
	}

	for (c = fmt; *c && (size & ~0x3FFULL); c++) {
		mm = nn;
		if (!rr && mm)
			rr = 1;
		size = (size >> 10);
		nn = (unsigned int)(size & 0x3FF);
	}

	if (!rr) {
		snprintf(str, len, "%u%c%s", nn, *c, unit);
		return str;
	}

	if (nn > 512) {
		mm = nn;
		nn = 0;
		c++;
	}

	mm = ((mm * 100) >> 10);

	snprintf(str, len, "%u.%u%c%s", nn, mm, *c, unit);

	return str;
}
EXPORT_SYMBOL(ultohstr);