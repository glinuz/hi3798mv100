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

#ifndef __SPARSE_H
#define __SPARSE_H

#include <stddef.h>

typedef struct sparse_header {
	uint32 magic;           /* 0xed26ff3a */
	uint16 major_version;   /* (0x1) - reject images with higher major versions */
	uint16 minor_version;   /* (0x0) - allow images with higer minor versions */
	uint16 file_hdr_sz;     /* 28 bytes for first revision of the file format */
	uint16 chunk_hdr_sz;    /* 12 bytes for first revision of the file format */
	uint32 blk_sz;          /* block size in bytes, must be a multiple of 4 (4096) */
	uint32 total_blks;      /* total blocks in the non-sparse output image */
	uint32 total_chunks;    /* total chunks in the sparse input image */
	uint32 image_checksum;  /* CRC32 checksum of the original data, counting "don't care" */
				/* as 0. Standard 802.3 polynomial, use a Public Domain */
				/* table implementation */
} sparse_header_t;

#define SPARSE_HEADER_MAGIC     0xed26ff3a

#define CHUNK_TYPE_RAW          0xCAC1
#define CHUNK_TYPE_FILL         0xCAC2
#define CHUNK_TYPE_DONT_CARE    0xCAC3

typedef struct chunk_header {
	uint16 chunk_type;    /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
	uint16 reserved1;
	uint32 chunk_sz;      /* in blocks in output image */
	uint32 total_sz;      /* in bytes of chunk input file including chunk header and data */
} chunk_header_t;


struct sparse_ctrl_t {
	sparse_header_t sparse;
	chunk_header_t  chunk;
	uint64 chunk_offset;
	uint32 read_len;  /* data have read */
	uint32 chunk_count;
};

#define SPARSE_HEADER_MAJOR_VER               1

#define IS_SPARSE(_sparse_hdr) \
	(_sparse_hdr->magic == SPARSE_HEADER_MAGIC \
		&& _sparse_hdr->major_version == SPARSE_HEADER_MAJOR_VER \
		&& _sparse_hdr->file_hdr_sz == sizeof(sparse_header_t) \
		&& _sparse_hdr->chunk_hdr_sz == sizeof(chunk_header_t))

#endif /* __SPARSE_H */
