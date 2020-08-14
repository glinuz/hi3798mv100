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

#include <stdio.h>
#include <string.h>
#include <trace.h>

extern char __symbol_start[];
extern char __symbol_end[];

struct sym_t {
	uint32 entry;
	uint32 size;
#define SYMNAME(_sym)    (__symbol_start + (_sym)->offset)
	uint32 offset;
};

struct sym_ctrl_t {
	struct sym_t *start;
	struct sym_t *tail;
	int nr_syms;
};

static struct sym_ctrl_t symctrl = {
	.start = (struct sym_t *)__symbol_start,
	.tail = NULL,
	.nr_syms = 0,
};
/******************************************************************************/

void symbol_init(void)
{
	int nr_syms = 0;
	struct sym_t *tail = symctrl.start;

	while (tail->entry && tail->size && (char *)tail < __symbol_end) {
		tail++;
		nr_syms++;
	}
	symctrl.tail = tail - 1;
	symctrl.nr_syms = nr_syms;
}
/******************************************************************************/

static struct sym_t *find_entry(uint32 entry)
{
	int x0 = 0;
	int x1 = symctrl.nr_syms - 1;
	int x = (x1 + x0) >> 1;
	struct sym_t *sym;

	if (symctrl.nr_syms == 0)
		return NULL;

	if (entry < symctrl.start->entry ||
	    entry >= symctrl.tail->entry + symctrl.tail->size)
		return NULL;

	while (x != x0) {
		sym = &symctrl.start[x];
		if (entry < sym->entry)
			x1 = x;
		else if (entry > (sym->entry + sym->size))
			x0 = x;
		else
			return sym;
		x = (x1 + x0) >> 1;
	}

	return NULL;
}
/******************************************************************************/

char *find_symbol(uint32 addr, uint32 *entry, uint32 *size, char **name)
{
	struct sym_t *sym = find_entry(addr);

	if (entry)
		*entry = (sym == NULL ? 0 : sym->entry);
	if (size)
		*size = (sym == NULL ? 0 : sym->size);
	if (name)
		*name = (sym == NULL ? "" : SYMNAME(sym));

	return (sym == NULL ? NULL : SYMNAME(sym));
}
/******************************************************************************/

void dump_symbol(void)
{
	struct sym_t *sym = symctrl.start;

	if (symctrl.nr_syms == 0)
		return;

	printf("address    size        symbol name\n");
	while (sym->entry && sym->offset) {
		printf("0x%08x 0x%08x: %s\n",
		       sym->entry, sym->size, SYMNAME(sym));
		sym++;
	}
}
/******************************************************************************/

void dump_stack(void)
{
	printf("Exception Stack:\n");
	backtracking(0);
}
/******************************************************************************/

void dump_trace_symbol(unsigned long where, unsigned long from)
{
	char *sym0, *sym1;

	find_symbol(where, NULL, NULL, &sym0);
	find_symbol(from, NULL, NULL, &sym1);

	printf("[<%08lx>] %s  ->  [<%08lx>] %s\n", where, sym0, from, sym1);
}
