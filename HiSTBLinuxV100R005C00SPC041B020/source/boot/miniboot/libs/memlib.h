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

#ifndef MEMLIBH
#define MEMLIBH
/******************************************************************************/

struct mem_chunk_t {
	unsigned int prev_size;
	unsigned int size;
	struct mem_chunk_t *fd;
	struct mem_chunk_t *bk;
};

struct memctrl_t {
	uint32 start;
	uint32 end;
	uint32 pos;
	char *sbrk_base;
	struct mem_chunk_t *hdr[256 + 2];

	uint32 nr_malloc;
	uint32 nr_free;
};

struct memctrl_t *memlib_init(uint32 start, uint32 len);
void *memlib_malloc(struct memctrl_t *ctrl, uint32 bytes);
void memlib_free(struct memctrl_t *ctrl, void* mem);
void *memlib_memalign(struct memctrl_t *ctrl, uint32 alignment, uint32 bytes);
void dump_memlib(struct memctrl_t *ctrl);

/******************************************************************************/
#endif /* MEMLIBH */
