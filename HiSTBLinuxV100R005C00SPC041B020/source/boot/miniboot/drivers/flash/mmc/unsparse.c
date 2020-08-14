/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai Yingjun
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

#include <config.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "mmc.h"
#include "sparse.h"

/******************************************************************************/

#define EMMC_BLKSIZE_SHIFT           (9)
#define SZ_1M_SHIFT                  (20)

// #define DEBUG_EXT4

//TODO: XXX
extern uint64 __udivmoddi4(uint64 a, uint64 b, uint64* rem);
static inline uint64 lldiv(uint64 numerator, uint32 denominator)
{
	uint64 rem = 0;
	uint64 ret = __udivmoddi4(numerator,denominator, &rem);
	return ret;
}

/******************************************************************************/
void print_header_info(sparse_header_t *header)
{
#ifdef DEBUG_EXT4
	printf("sparse header info:\n");
	printf("magic: 0x%x\n",header->magic);
	printf("major_version: 0x%x\n",header->major_version);
	printf("minor_version: 0x%x\n",header->minor_version);
	printf("file_hdr_sz: %d\n",header->file_hdr_sz);
	printf("chunk_hdr_sz: %d\n",header->chunk_hdr_sz);
	printf("blk_sz: %d\n",header->blk_sz);
	printf("total_blks: %d\n",header->total_blks);
	printf("total_chunks: %d\n",header->total_chunks);
	printf("image_checksum: %d\n",header->image_checksum);
#endif
}

/******************************************************************************/
void print_chunk_info(chunk_header_t *chunk)
{
#ifdef DEBUG_EXT4
	printf("chunk header info:\n");
	printf("chunk_type: 0x%x\n",chunk->chunk_type);
	printf("chunk_sz: 0x%d\n",chunk->chunk_sz);
	printf("total_sz: 0x%d\n",chunk->total_sz);
#endif
}

/******************************************************************************/
int ext4_unsparse(struct mmc *mmc, uint32 dev, const uint8 *pbuf, uint32 blk, uint32 cnt)
{
	uint32 i, num;
	uint64 dense_len = 0;
	uint64 sparse_len = 0;
	uint64 img_size;
	uint32 chunk_len;
	int32 percent_complete = -1;
	chunk_header_t *chunk = NULL;
	sparse_header_t *header = (sparse_header_t *)pbuf;

	if (!IS_SPARSE(header)) {
		printf("Invalid sparse format.\n");
		return 1;
	}

	/* check header->blk_sz align to emmc block size */
	if (header->blk_sz & ((1 << EMMC_BLKSIZE_SHIFT) - 1)) {
		printf("image blk size %d is not aligned to 512Byte.\n",
			header->blk_sz);
		return 1;
	}

	/* check fs img's real size is larger than partition size */
	img_size = (uint64)(header->total_blks * header->blk_sz);
	if ((img_size >> EMMC_BLKSIZE_SHIFT) > (uint64)cnt) {
		printf("partition size %d MB not enough.\n",
			(int)(cnt >> (SZ_1M_SHIFT - EMMC_BLKSIZE_SHIFT)));
		print_header_info(header);
		return 1;
	}

	/* skip the sparse header,to visit first chunk */
	pbuf += header->file_hdr_sz;

	/* to visit each chunk */
	for (i = 0; i < header->total_chunks; i++) {

		/* here the chunk_header */
		chunk = (chunk_header_t *)pbuf;

		/* go to next chunk's data */
		pbuf += header->chunk_hdr_sz;

		switch (chunk->chunk_type) {
		case CHUNK_TYPE_RAW:

			/* to calculate the length of each chunk */
			chunk_len = chunk->chunk_sz * header->blk_sz;

			/* verify every chunk to asure it is valid */
			if (chunk->total_sz
			    != (chunk_len + header->chunk_hdr_sz)) {
				printf("No.%d chunk size error.\n", i);
				print_chunk_info(chunk);
				return 1;
			}

			dense_len += chunk_len;
			sparse_len += chunk_len;

			if (sparse_len > ((uint64)cnt << EMMC_BLKSIZE_SHIFT)) {
				printf("error: sparse size %d MB is "
				       "larger than partiton size %d MB.\n",
				       (int)(sparse_len >> SZ_1M_SHIFT),
				       (int)(cnt >> (SZ_1M_SHIFT - EMMC_BLKSIZE_SHIFT)));
				print_chunk_info(chunk);
				return 1;
			}

			num = mmc->block_dev.block_write(dev,
				blk,
				(chunk_len >> EMMC_BLKSIZE_SHIFT),
				pbuf);
			if (num != (chunk_len >> EMMC_BLKSIZE_SHIFT)) {
				printf("Raw data: No.%d blocks written: %s.\n",
					num, "ERROR");
				return 1;
			}

			pbuf += chunk_len;
			blk  += (chunk_len >> EMMC_BLKSIZE_SHIFT);
			break;

		case CHUNK_TYPE_DONT_CARE:
			if (chunk->total_sz != header->chunk_hdr_sz) {
				printf("No.%d chunk size error.\n", i);
				print_chunk_info(chunk);
				return 1;
			}

			chunk_len = chunk->chunk_sz * header->blk_sz;
			sparse_len += chunk_len;

			if (sparse_len > ((uint64)cnt << EMMC_BLKSIZE_SHIFT)) {
				printf("error: sparse size %d MB is "
				       "larger than partiton size %d MB.\n",
				       (int)(sparse_len >> SZ_1M_SHIFT),
				       (int)(cnt >> (SZ_1M_SHIFT - EMMC_BLKSIZE_SHIFT)));
				print_chunk_info(chunk);
				return 1;
			}

			blk  += (chunk_len >> EMMC_BLKSIZE_SHIFT);
			break;

		default:
			printf("sparse: unknow chunk type %04x.\n",
				chunk->chunk_type);
			return 1;
		}

		{
			uint64 n = (uint64)(sparse_len >> EMMC_BLKSIZE_SHIFT) * 100;
			int percent;
			n = lldiv(n, cnt);
			percent = (int)n;

			if (percent != percent_complete) {
				percent_complete = percent;
				printf("\rWriting at %d blk# -- %3d%% complete.",
					(int)(blk
					  + (sparse_len >> EMMC_BLKSIZE_SHIFT)),
					percent);
			}
		}

	}
	puts("\n");

	if (((uint64)cnt << EMMC_BLKSIZE_SHIFT) != sparse_len) {
		printf("error: partition size %d MB != ext4 image size %d MB\n",
			(int)(cnt >> (SZ_1M_SHIFT - EMMC_BLKSIZE_SHIFT)),
			(int)(sparse_len >> SZ_1M_SHIFT));
		return 1;
	}

	printf("sparse: %d MB / %d MB.\n", (int)(dense_len >> SZ_1M_SHIFT),
		(int)(sparse_len >> SZ_1M_SHIFT));

	return 0;
}
