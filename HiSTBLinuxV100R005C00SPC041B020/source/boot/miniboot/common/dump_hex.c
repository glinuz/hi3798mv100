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
#include <stdio.h>
#include <string.h>
#include <debug.h>

#define SZ_LINE    16

/*****************************************************************************/

static void pr_hex(uint8 *to, char *from, uint32 width)
{
	uint32 n = 0;
	char fmt[16] = {0};
	memcpy(&n, from, width);
	memcpy(to, from, width);
	snprintf(fmt, sizeof(fmt), " %%0%dx", (width << 1));
	printf(fmt, n);
}
/*****************************************************************************/

static void pr_space(uint32 width)
{
	width = (width << 1) + 1;
	while (width--)
		printf(" ");
}
/*****************************************************************************/

int dump_hex(uint32 addr, char *buf, uint32 sz_buf, uint32 width)
{
	uint32 ix, skip;
	uint8 data[SZ_LINE];
	uint32 align = ((uint32)buf) & (width - 1);

	if (align)
		align = width - align;

	ASSERT(sz_buf >= 4);
	ASSERT1(width == 1 || width == 2 || width == 4, "width:%d\n", width);

	buf += align;
	addr += align;
	sz_buf -= align;
	sz_buf = sz_buf & ~(width - 1);

	skip = addr % SZ_LINE;

	if (skip) {
		uint32 min_ix = min(sz_buf, SZ_LINE - skip) + skip;

		printf("%08lx:", (addr - skip));

		for (ix = 0; ix < skip; ix += width)
			pr_space(width);

		for (ix = skip; ix < min_ix; ix += width, buf += width)
			pr_hex(data + ix, buf, width);

		addr += (min_ix - skip);

		for (ix = min_ix; ix < SZ_LINE; ix += width)
			pr_space(width);

		printf("%s", "    ");

		for (ix = 0; ix < skip; ix++)
			printf("%s", " ");

		for (ix = skip; ix < min_ix; ix++)
			printf("%c", isprint(data[ix]) && (data[ix] < 0x80) ? data[ix] : '.');

		sz_buf -= (min_ix - skip);

		printf("\n");
	}

	while (sz_buf) {
		uint32 steplen = SZ_LINE;

		printf("%08lx:", addr);

		if (steplen > sz_buf)
			steplen = sz_buf;

		for (ix = 0; ix < steplen; ix += width, buf += width)
			pr_hex(data + ix, buf, width);

		for (ix = steplen; ix < SZ_LINE; ix += width)
			pr_space(width);

		printf("%s", "    ");

		for (ix = 0; ix < steplen; ix++)
			printf("%c", isprint(data[ix]) && (data[ix] < 0x80) ? data[ix] : '.');

		sz_buf -= steplen;
		addr += steplen;

		printf("\n");
	}

	return 0;
}
