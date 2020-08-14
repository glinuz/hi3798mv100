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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int                   uint32;

struct sym_t {
	uint32 entry;
	uint32 size;
	char *name;
};

struct symtbl_t {
	struct sym_t *syms;
	uint32 max_sym;
	uint32 of_sym;

	char *buf;
	uint32 max_buf;
	uint32 of_buf;
};

#define MAX_SYMBOL_NAME                  512

/*****************************************************************************/

static int read_symbol(FILE *fin, struct symtbl_t *symtbl)
{
	int ret;
	char type;
	struct sym_t *sym;
/*
8050b694 00000014 T udp_send
8050b6a8 00000328 T udp_input
8050b9d0 0000006c t etharp_free_entry
8050ba3c 00000248 t etharp_find_entry
8050bc84 0000009c T etharp_find_addr
8050bd20 00000050 T etharp_cleanup_netif
 */
	sym = &symtbl->syms[symtbl->of_sym];
	sym->name = symtbl->buf + symtbl->of_buf;

	ret = fscanf(fin, "%x %x %c %511s\n", &sym->entry, &sym->size, &type, sym->name);
	if (ret != 4) {
		if (ret != EOF && fgets(sym->name, MAX_SYMBOL_NAME, fin) == NULL)
			fprintf(stderr, "Read error or end of file.\n");
		return -1;
	}

	if (type != 'T' && type != 't')
		return 1;

	symtbl->of_sym++;
	symtbl->of_buf += strlen(sym->name) + 1;

	return 0;
}
/*****************************************************************************/

static int read_map(FILE *fin, struct symtbl_t *symtbl)
{
	struct sym_t *sym;

	while (!feof(fin)) {
		if (symtbl->of_sym >= symtbl->max_sym) {
			symtbl->max_sym += 100;
			symtbl->syms = (struct sym_t *)realloc(symtbl->syms,
				symtbl->max_sym * sizeof(struct sym_t));
			if (!symtbl->syms) {
				fprintf(stderr, "out of memory.\n");
				return -1;
			}
		}

		if (symtbl->of_buf + MAX_SYMBOL_NAME > symtbl->max_buf) {
			symtbl->max_buf += MAX_SYMBOL_NAME * 100;
			symtbl->buf = (char *)realloc(symtbl->buf, symtbl->max_buf);
			if (!symtbl->buf) {
				fprintf(stderr, "out of memory.\n");
				return -1;
			}
		}

		read_symbol(fin, symtbl);
	}
	return 0;
}
/*****************************************************************************/

static void export_symbol(char *buf, uint32 sz_buf)
{
	uint32 *ptr = (uint32 *)buf;

	sz_buf += 3;

	printf(".section .symbol,#alloc\n");
	printf(".global __symbol_start; __symbol_start:\n");

	while (sz_buf >= 4) {
		printf(".word 0x%08x\n", *ptr);
		sz_buf -= 4;
		ptr++;
	}

	printf(".global __symbol_end; __symbol_end:\n");
}
/*****************************************************************************/

static void dump_symbol(FILE *fout, struct symtbl_t *symtbl)
{
	int ix;
	uint32 *paddr;
	char *pbuf;
	char *data;
	uint32 sz_data;
	struct sym_t *sym;

	sz_data = (symtbl->of_sym + 1) * sizeof(struct sym_t) + symtbl->of_buf;
	data = (char *)malloc(sz_data + 4);
	if (!data) {
		fprintf(stderr, "out of memory.\n");
		exit(1);
	}
	memset(data, 0, sz_data + 4);

	paddr = (uint32 *)data;
	pbuf = data + (symtbl->of_sym + 1) * sizeof(struct sym_t);

	for (ix = 0; ix < symtbl->of_sym; ix++) {
		sym = &symtbl->syms[ix];

		*paddr++ = sym->entry;
		*paddr++ = sym->size;
		*paddr++ = (uint32)(pbuf - data);
		pbuf += sprintf(pbuf, "%s", sym->name) + 1;
	}

	export_symbol(data, sz_data);

	free(data);
}
/*****************************************************************************/

int main(int argc, char *argv[])
{
	struct symtbl_t symtbl = {0};

	read_map(stdin, &symtbl);

	dump_symbol(stdout, &symtbl);

	return 0;
}
