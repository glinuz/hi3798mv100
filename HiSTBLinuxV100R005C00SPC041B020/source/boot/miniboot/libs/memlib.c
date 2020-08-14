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
 * modify base on http://g.oswego.edu/dl/html/malloc.html
******************************************************************************/

#include <stddef.h>
#include <string.h>
#include "memlib.h"

//#define MODULE_DEBUG
#include <debug.h>

#define EFAIL                -1

#define MALLOC_ALIGN_MASK      ((sizeof(uint32) << 1) - 1)
#define MINSIZE                (sizeof(struct mem_chunk_t))
#define MAX_SMALLBIN_SIZE      512
#define SMALLBIN_WIDTH         8
#define PREV_INUSE             0x1
#define MALLOC_PAGESIZE        4096
#define SIZE_BITS              (PREV_INUSE)
#define MEMLIB_TRIM_THRESHOLD  (128 * 1024)

/* pad request bytes into a usable size */
#define request2size(req) \
	(((long)((req) + (sizeof(uint32) + MALLOC_ALIGN_MASK)) < (long)(MINSIZE + MALLOC_ALIGN_MASK)) ? MINSIZE : (((req) + (sizeof(uint32) + MALLOC_ALIGN_MASK)) & ~(MALLOC_ALIGN_MASK)))

#define is_small_request(nb) (nb < MAX_SMALLBIN_SIZE - SMALLBIN_WIDTH)
#define smallbin_index(sz)   (((unsigned long)(sz)) >> 3)

#define first(b)          ((b)->fd)
#define last(b)           ((b)->bk)
#define chunksize(p)      ((p)->size & ~(SIZE_BITS))
#define chunk2mem(p)      ((void*)((char*)(p) + (sizeof(uint32) << 1)))
#define mem2chunk(mem)    ((struct mem_chunk_t *)((char*)(mem) - (sizeof(uint32) << 1)))

#define bin_at(_ctrl, i)  ((struct mem_chunk_t *)((char*)&(_ctrl->hdr[(i << 1) + 2]) - (sizeof(uint32) << 1)))
#define next_bin(b)       ((struct mem_chunk_t *)((char*)(b) + (sizeof(struct mem_chunk_t *) << 1)))
#define prev_bin(b)       ((struct mem_chunk_t *)((char*)(b) - (sizeof(struct mem_chunk_t *) << 1)))

#define set_head(p, s)      ((p)->size = (s))
#define set_foot(p, s)      (((struct mem_chunk_t *)((char*)(p) + (s)))->prev_size = (s))
#define set_head_size(p, s) ((p)->size = (((p)->size & PREV_INUSE) | (s)))

#define set_inuse_bit_at_offset(p, s) \
	(((struct mem_chunk_t *)(((char*)(p)) + (s)))->size |= PREV_INUSE)
#define inuse_bit_at_offset(p, s)\
	(((struct mem_chunk_t *)(((char*)(p)) + (s)))->size & PREV_INUSE)

#define chunk_at_offset(p, s)  ((struct mem_chunk_t *)(((char*)(p)) + (s)))

#define binblocks_w(_ctrl)     (_ctrl->hdr[1])
#define binblocks_r(_ctrl)     ((unsigned int)_ctrl->hdr[1]) /* bitvector of nonempty blocks */

#define mark_binblock(_ctrl, ii)   (binblocks_w(_ctrl) = (struct mem_chunk_t *)(binblocks_r(_ctrl) | idx2binblock(ii)))

#define BINBLOCKWIDTH   4   /* bins per block */

/* bin<->block macros */
#define idx2binblock(ix)    ((unsigned)1 << (ix / BINBLOCKWIDTH))

#define link_last_remainder(_ctrl, P) { \
	last_remainder(_ctrl)->fd = last_remainder(_ctrl)->bk =  P; \
	P->fd = P->bk = last_remainder(_ctrl); \
}

#define clear_last_remainder(_ctrl) \
	(last_remainder(_ctrl)->fd = last_remainder(_ctrl)->bk = last_remainder(_ctrl))

#define bin_index(sz)  \
	(((((unsigned long)(sz)) >> 9) == 0) ? (((unsigned long)(sz)) >>  3): \
	((((unsigned long)(sz)) >> 9) <=    4) ?  56 + (((unsigned long)(sz)) >>  6): \
	((((unsigned long)(sz)) >> 9) <=   20) ?  91 + (((unsigned long)(sz)) >>  9): \
	((((unsigned long)(sz)) >> 9) <=   84) ? 110 + (((unsigned long)(sz)) >> 12): \
	((((unsigned long)(sz)) >> 9) <=  340) ? 119 + (((unsigned long)(sz)) >> 15): \
	((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18): \
	126)

#define initial_top(_ctrl)       ((struct mem_chunk_t *)(bin_at(_ctrl, 0)))
#define topmost_chunk(_ctrl)     ((_ctrl)->hdr[2])  /* The topmost chunk */
#define last_remainder(_ctrl)    (bin_at(_ctrl, 1)) /* remainder from last split */

/* take a chunk off a list */
#define unlink(P, BK, FD) { \
	BK = P->bk;  \
	FD = P->fd;  \
	FD->bk = BK; \
	BK->fd = FD; \
}

#define frontlink(_ctrl, P, S, IDX, BK, FD) { \
	if (S < MAX_SMALLBIN_SIZE) {  \
		IDX = smallbin_index(S);   \
		mark_binblock(_ctrl, IDX);  \
		BK = bin_at(_ctrl, IDX);  \
		FD = BK->fd;  \
		P->bk = BK;  \
		P->fd = FD;  \
		FD->bk = BK->fd = P; \
	} else  {  \
		IDX = bin_index(S);  \
		BK = bin_at(_ctrl, IDX);    \
		FD = BK->fd;         \
		if (FD == BK) \
			mark_binblock(_ctrl, IDX);  \
		else { \
			while (FD != BK && S < chunksize(FD)) \
				FD = FD->fd; \
			BK = FD->bk; \
		} \
		P->bk = BK; \
		P->fd = FD; \
		FD->bk = BK->fd = P; \
	}  \
}
/*****************************************************************************/

static void *memlib_sbrk(struct memctrl_t *ctrl, int32 increment)
{
	uint32 old = ctrl->pos;
	uint32 new_pos = old + increment;

	if ((new_pos < ctrl->start) || (new_pos > ctrl->end))
		return (void *)EFAIL;

	ctrl->pos = new_pos;

	return ((void *)old);
}
/*****************************************************************************/

static void memlib_extend_top(struct memctrl_t *ctrl, unsigned int nb)
{
	char *brk;                   /* return value from dlmalloc_sbrk */
	unsigned int front_misalign; /* unusable bytes at front of sbrked space */
	unsigned int correction;     /* bytes for 2nd dlmalloc_sbrk call */
	char *new_brk;         /* return of 2nd dlmalloc_sbrk call */
	unsigned int top_size; /* new size of topmost_chunk chunk */

	struct mem_chunk_t *old_top = topmost_chunk(ctrl);  /* Record state of old topmost_chunk */
	unsigned int old_top_size = chunksize(old_top);
	char *old_end = (char*)(chunk_at_offset(old_top, old_top_size));

	/* Pad request with top_pad plus minimal overhead */

	unsigned int sbrk_size = nb + MINSIZE;
	unsigned long pagesz = MALLOC_PAGESIZE;

	/* If not the first time through, round to preserve page boundary */
	/* Otherwise, we need to correct to a page size below anyway. */
	/* (We also correct below if an intervening foreign dlmalloc_sbrk call.) */

	if (ctrl->sbrk_base != (char*)(-1))
		sbrk_size = (sbrk_size + (pagesz - 1)) & ~(pagesz - 1);

	brk = (char*)memlib_sbrk(ctrl, sbrk_size);

	/* Fail if dlmalloc_sbrk failed or if a foreign dlmalloc_sbrk call killed our space */
	if (brk == (char*)(EFAIL) ||
		(brk < old_end && old_top != initial_top(ctrl)))
		return;

	if (brk == old_end) {
		/* can just add bytes to current topmost_chunk */
		top_size = sbrk_size + old_top_size;
		set_head(topmost_chunk(ctrl), top_size | PREV_INUSE);
	} else {
		if (ctrl->sbrk_base == (char*)(-1))  /* First time through. Record base */
			ctrl->sbrk_base = brk;

		/* Guarantee alignment of first new chunk made from this space */
		front_misalign = (unsigned long)chunk2mem(brk) & MALLOC_ALIGN_MASK;
		if (front_misalign > 0) {
			correction = ((sizeof(uint32) << 1)) - front_misalign;
			brk += correction;
		} else
			correction = 0;

		/* Guarantee the next brk will be at a page boundary */
		correction += ((((unsigned long)(brk + sbrk_size))+(pagesz-1)) &
			~(pagesz - 1)) - ((unsigned long)(brk + sbrk_size));

		/* Allocate correction */
		new_brk = (char*)memlib_sbrk(ctrl, correction);
		if (new_brk == (char*)(EFAIL))
			return;

		topmost_chunk(ctrl) = (struct mem_chunk_t *)brk;
		top_size = new_brk - brk + correction;
		set_head(topmost_chunk(ctrl), top_size | PREV_INUSE);

		if (old_top != initial_top(ctrl)) {
			/* There must have been an intervening foreign dlmalloc_sbrk call. */
			/* A double fencepost is necessary to prevent consolidation */

			/* If not enough space to do this, then user did something very wrong */
			if (old_top_size < MINSIZE) {
				set_head(topmost_chunk(ctrl), PREV_INUSE); /* will force null return from malloc */
				return;
			}

			/* Also keep size a multiple of (sizeof(uint32) << 1) */
			old_top_size = (old_top_size - 3*sizeof(uint32)) & ~MALLOC_ALIGN_MASK;
			set_head_size(old_top, old_top_size);
			chunk_at_offset(old_top, old_top_size)->size
				= sizeof(uint32)|PREV_INUSE;
			chunk_at_offset(old_top, old_top_size + sizeof(uint32))->size
				= sizeof(uint32)|PREV_INUSE;
			/* If possible, release the rest. */
			if (old_top_size >= MINSIZE)
				memlib_free(ctrl, chunk2mem(old_top));
		}
	}
}
/*****************************************************************************/

static int memlib_trim(struct memctrl_t *ctrl, uint32 pad)
{
	long  top_size;        /* Amount of topmost_chunk-most memory */
	long  extra;           /* Amount to release */
	char *current_brk;     /* address returned by pre-check dlmalloc_sbrk call */
	char *new_brk;         /* address returned by negative dlmalloc_sbrk call */
	unsigned long pagesz = MALLOC_PAGESIZE;

	top_size = chunksize(topmost_chunk(ctrl));
	extra = ((top_size - pad - MINSIZE + (pagesz-1)) / pagesz - 1) * pagesz;

	if (extra < (long)pagesz) {
		/* Not enough memory to release */
		return 0;
	} else {
		/* Test to make sure no one else called dlmalloc_sbrk */
		current_brk = (char*)memlib_sbrk(ctrl, 0);

		if (current_brk != (char*)(topmost_chunk(ctrl)) + top_size) {
			return 0;     /* Apparently we don't own memory; must fail */
		} else {
			new_brk = (char*)memlib_sbrk(ctrl, -extra);

			if (new_brk == (char*)(EFAIL)) {
				/* Try to figure out what we have */
				current_brk = (char*)memlib_sbrk(ctrl, 0);
				top_size = current_brk - (char*)topmost_chunk(ctrl);
				if (top_size >= (long)MINSIZE) /* if not, we are very very dead! */
					set_head(topmost_chunk(ctrl), top_size | PREV_INUSE);
				return 0;
			} else {
				/* Success. Adjust topmost_chunk accordingly. */
				set_head(topmost_chunk(ctrl), (top_size - extra) | PREV_INUSE);
				return 1;
			}
		}
	}
}
/*****************************************************************************/

struct memctrl_t *memlib_init(uint32 start, uint32 len)
{
	int ix;
	struct memctrl_t *ctrl = (struct memctrl_t *)start;

	ASSERT(!(start & 0x03));

	if (len <= sizeof(struct memctrl_t) + 4096)
		return NULL;

	start += sizeof(struct memctrl_t);
	len   -= sizeof(struct memctrl_t);

	memset(ctrl, 0, sizeof(struct memctrl_t));

	ctrl->start = start;
	ctrl->end = start + len;
	ctrl->pos = start;
	ctrl->sbrk_base = (char *)(-1);

	ctrl->hdr[0] = 0;
	ctrl->hdr[1] = 0;

	for (ix = 0; ix < 128; ix ++) {
		int pos = (ix + 1) << 1;
		ctrl->hdr[pos] = bin_at(ctrl, ix);
		ctrl->hdr[pos+1] = bin_at(ctrl, ix);
	}

	return ctrl;
}
/*****************************************************************************/

void dump_memlib(struct memctrl_t *ctrl)
{
	printf("start:       0x%08x\n", ctrl->start);
	printf("end:         0x%08x\n", ctrl->end);
	printf("pos:         0x%08x\n", ctrl->pos);
	printf("malloc:      %d\n", ctrl->nr_malloc);
	printf("free:        %d\n", ctrl->nr_free);
}
/*****************************************************************************/

void *memlib_malloc(struct memctrl_t *ctrl, uint32 bytes)
{
	struct mem_chunk_t *victim; /* inspected/selected chunk */
	unsigned int victim_size; /* its size */
	int idx; /* index for bin traversal */
	struct mem_chunk_t *bin;  /* associated bin */
	struct mem_chunk_t *remainder;  /* remainder from a split */
	long remainder_size;  /* its size */
	int remainder_index;  /* its bin index */
	unsigned long block;  /* block traverser bit */
	int startidx;  /* first bin of a traversed block */
	struct mem_chunk_t *fwd;  /* misc temp for linking */
	struct mem_chunk_t *bck;  /* misc temp for linking */
	struct mem_chunk_t *q;  /* misc temp */
	unsigned int roud_size;

	ctrl->nr_malloc++;

	if ((long)bytes < 0) {
		fdbgout("malloc size is zero\n");
		return NULL;
	}

	roud_size = request2size(bytes);  /* padded request size; */

	/* Check for exact match in a bin */

	if (is_small_request(roud_size)) {
		/* Faster version for small requests */
		idx = smallbin_index(roud_size);

		/* No traversal or size check necessary for small bins.  */
		q = bin_at(ctrl, idx);
		victim = last(q);

		/* Also scan the next one, since it would have a remainder < MINSIZE */
		if (victim == q) {
			q = next_bin(q);
			victim = last(q);
		}

		if (victim != q) {
			victim_size = chunksize(victim);
			unlink(victim, bck, fwd);
			set_inuse_bit_at_offset(victim, victim_size);

			return chunk2mem(victim);
		}
		idx += 2; /* Set for bin scan below. We've already scanned 2 bins. */
	} else {
		idx = bin_index(roud_size);
		bin = bin_at(ctrl, idx);

		for (victim = last(bin); victim != bin; victim = victim->bk) {
			victim_size = chunksize(victim);
			remainder_size = victim_size - roud_size;

			if (remainder_size >= (long)MINSIZE) {
				/* too big */
				--idx; /* adjust to rescan below after checking last remainder */
				break;
			} else if (remainder_size >= 0) {
				 /* exact fit */
				unlink(victim, bck, fwd);
				set_inuse_bit_at_offset(victim, victim_size);
				return chunk2mem(victim);
			}
		}
		++idx;
	}

	/* Try to use the last split-off remainder */
	if ((victim = last_remainder(ctrl)->fd) != last_remainder(ctrl)) {
		victim_size = chunksize(victim);
		remainder_size = victim_size - roud_size;

		if (remainder_size >= (long)MINSIZE) {
			remainder = chunk_at_offset(victim, roud_size);
			set_head(victim, roud_size | PREV_INUSE);
			link_last_remainder(ctrl, remainder);
			set_head(remainder, remainder_size | PREV_INUSE);
			set_foot(remainder, remainder_size);
			return chunk2mem(victim);
		}

		clear_last_remainder(ctrl);

		if (remainder_size >= 0) {
			set_inuse_bit_at_offset(victim, victim_size);
			return chunk2mem(victim);
		}

		/* Else place in bin */
		frontlink(ctrl, victim, victim_size, remainder_index, bck, fwd);
	}

	/*
	 * If there are any possibly nonempty big-enough blocks,
	 * search for best fitting chunk by scanning bins in block width units.
	 */

	if ((block = idx2binblock(idx)) <= binblocks_r(ctrl)) {
		/* Get to the first marked block */

		if ( (block & binblocks_r(ctrl)) == 0) {
			/* force to an even block boundary */
			idx = (idx & ~(BINBLOCKWIDTH - 1)) + BINBLOCKWIDTH;
			block <<= 1;
			while ((block & binblocks_r(ctrl)) == 0) {
				idx += BINBLOCKWIDTH;
				block <<= 1;
			}
		}

		/* For each possibly nonempty block ... */
		for (;;) {
			startidx = idx;          /* (track incomplete blocks) */
			q = bin = bin_at(ctrl, idx);

			/* For each bin in this block ... */
			do {
				/* Find and use first big enough chunk ... */
				for (victim = last(bin); victim != bin;
				     victim = victim->bk) {
					victim_size = chunksize(victim);
					remainder_size = victim_size - roud_size;

					if (remainder_size >= (long)MINSIZE) {
						remainder = chunk_at_offset(victim, roud_size);
						set_head(victim, roud_size | PREV_INUSE);
						unlink(victim, bck, fwd);
						link_last_remainder(ctrl, remainder);
						set_head(remainder, remainder_size | PREV_INUSE);
						set_foot(remainder, remainder_size);
						return chunk2mem(victim);
					} else if (remainder_size >= 0) {
						set_inuse_bit_at_offset(victim, victim_size);
						unlink(victim, bck, fwd);
						return chunk2mem(victim);
					}
				}
				bin = next_bin(bin);
			} while ((++idx & (BINBLOCKWIDTH - 1)) != 0);

			/* Clear out the block bit. */
			do {
				/* Possibly backtrack to try to clear a partial block */
				if ((startidx & (BINBLOCKWIDTH - 1)) == 0) {
					ctrl->hdr[1] = (struct mem_chunk_t *)(binblocks_r(ctrl) & ~block);
					break;
				}
				--startidx;
				q = prev_bin(q);
			} while (first(q) == q);

			/* Get to the next possibly nonempty block */
			if ((block <<= 1) <= binblocks_r(ctrl) && (block != 0)) {
				while ((block & binblocks_r(ctrl)) == 0) {
					idx += BINBLOCKWIDTH;
					block <<= 1;
				}
			} else
				break;
		}
	}

	/* Try to use topmost_chunk chunk */
	/* Require that there be a remainder, ensuring topmost_chunk always exists  */
	if ((remainder_size = chunksize(topmost_chunk(ctrl)) - roud_size) < (long)MINSIZE) {
		/* Try to extend */
		memlib_extend_top(ctrl, roud_size);
		if ((remainder_size = chunksize(topmost_chunk(ctrl)) - roud_size) < (long)MINSIZE) {
			fdbgout("out of memory.\n");
			return NULL;
		}
	}

	victim = topmost_chunk(ctrl);
	set_head(victim, roud_size | PREV_INUSE);
	topmost_chunk(ctrl) = chunk_at_offset(victim, roud_size);
	set_head(topmost_chunk(ctrl), remainder_size | PREV_INUSE);
	return chunk2mem(victim);
}
/*****************************************************************************/

void memlib_free(struct memctrl_t *ctrl, void *mem)
{
	struct mem_chunk_t *p; /* chunk corresponding to mem */
	unsigned int this_status;  /* its head field */
	unsigned int this_size;  /* its size */
	int idx;  /* its bin index */
	struct mem_chunk_t *_next_chunk; /* next contiguous chunk */
	unsigned int _next_size; /* its size */
	unsigned int prevsz; /* size of previous contiguous chunk */
	struct mem_chunk_t *bck;  /* misc temp for linking */
	struct mem_chunk_t *fwd;  /* misc temp for linking */
	int islr;  /* track whether merging with last_remainder */

	ctrl->nr_free++;

	if (mem == 0)  /* free(0) has no effect */
		return;

	p = mem2chunk(mem);
	this_status = p->size;

	this_size = this_status & ~PREV_INUSE;
	_next_chunk = chunk_at_offset(p, this_size);
	_next_size = chunksize(_next_chunk);

	if (_next_chunk == topmost_chunk(ctrl)) { /* merge with topmost_chunk */
		this_size += _next_size;

		if (!(this_status & PREV_INUSE)) { /* consolidate backward */
			prevsz = p->prev_size;
			p = chunk_at_offset(p, -((long) prevsz));
			this_size += prevsz;
			unlink(p, bck, fwd);
		}

		set_head(p, this_size | PREV_INUSE);
		topmost_chunk(ctrl) = p;
		if (this_size >= MEMLIB_TRIM_THRESHOLD)
			memlib_trim(ctrl, 0);
		return;
	}

	set_head(_next_chunk, _next_size); /* clear inuse bit */

	islr = 0;

	if (!(this_status & PREV_INUSE)) { /* consolidate backward */
		prevsz = p->prev_size;
		p = chunk_at_offset(p, -((long) prevsz));
		this_size += prevsz;

		if (p->fd == last_remainder(ctrl)) /* keep as last_remainder */
			islr = 1;
		else
			unlink(p, bck, fwd);
	}

	if (!(inuse_bit_at_offset(_next_chunk, _next_size))) { /* consolidate forward */
		this_size += _next_size;

		if (!islr && _next_chunk->fd == last_remainder(ctrl)) { /* re-insert last_remainder */
			islr = 1;
			link_last_remainder(ctrl, p);
		} else
			unlink(_next_chunk, bck, fwd);
	}

	set_head(p, this_size | PREV_INUSE);
	set_foot(p, this_size);

	if (!islr)
		frontlink(ctrl, p, this_size, idx, bck, fwd);
}
/*****************************************************************************/

void *memlib_memalign(struct memctrl_t *ctrl, uint32 alignment, uint32 bytes)
{
	unsigned int nb;  /* padded  request size */
	char *m;          /* memory returned by malloc call */
	struct mem_chunk_t *p;    /* corresponding chunk */
	char *brk;                /* alignment point within p */
	struct mem_chunk_t *newp; /* chunk to return */
	unsigned int newsize;     /* its size */
	unsigned int leadsize;    /* leading space before alignment point */
	struct mem_chunk_t *remainder; /* spare room at end to split off */
	long remainder_size;           /* its size */

	ctrl->nr_malloc++;

	if ((long)bytes < 0)
		return NULL;

	/* If need less alignment than we give anyway, just relay to malloc */
	if (alignment <= (sizeof(uint32) << 1))
		return memlib_malloc(ctrl, bytes);

	/* Otherwise, ensure that it is at least a minimum chunk size */
	if (alignment <  MINSIZE)
		alignment = MINSIZE;

	/* Call malloc with worst case padding to hit alignment. */
	nb = request2size(bytes);
	m  = (char*)memlib_malloc(ctrl, nb + alignment + MINSIZE);

	if (m == 0)
		return NULL; /* propagate failure */

	p = mem2chunk(m);

	if ((((unsigned long)(m)) % alignment) == 0) {
		/* aligned */
	} else {
		/* misaligned */
		/*
		 * Find an aligned spot inside chunk.
		 * Since we need to give back leading space in a chunk of at
		 * least MINSIZE, if the first calculation places us at
		 * a spot with less than MINSIZE leader, we can move to the
		 * next aligned spot -- we've allocated enough total room so that
		 * this is always possible.
		 */

		brk = (char*)mem2chunk(((unsigned long)(m + alignment - 1)) & -((signed) alignment));
		if ((long)(brk - (char*)(p)) < MINSIZE)
			brk = brk + alignment;

		newp = (struct mem_chunk_t *)brk;
		leadsize = brk - (char*)(p);
		newsize = chunksize(p) - leadsize;

		/* give back leader, use the rest */
		set_head(newp, newsize | PREV_INUSE);
		set_inuse_bit_at_offset(newp, newsize);
		set_head_size(p, leadsize);
		memlib_free(ctrl, chunk2mem(p));
		p = newp;
	}

	/* Also give back spare room at the end */
	remainder_size = chunksize(p) - nb;

	if (remainder_size >= (long)MINSIZE) {
		remainder = chunk_at_offset(p, nb);
		set_head(remainder, remainder_size | PREV_INUSE);
		set_head_size(p, nb);
		memlib_free(ctrl, chunk2mem(remainder));
	}

	return chunk2mem(p);
}
