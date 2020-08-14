/*
 * (C) Copyright 2011
 * Texas Instruments, <www.ti.com>
 * Author: Vikram Pandita <vikram.pandita@ti.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __SPARSE_H
#define __SPARSE_H

#include <linux/types.h>

typedef struct sparse_header {
	u32 magic;           /* 0xed26ff3a */
	u16 major_version;   /* (0x1) - reject images with higher major versions */
	u16 minor_version;   /* (0x0) - allow images with higer minor versions */
	u16 file_hdr_sz;     /* 28 bytes for first revision of the file format */
	u16 chunk_hdr_sz;    /* 12 bytes for first revision of the file format */
	u32 blk_sz;          /* block size in bytes, must be a multiple of 4 (4096) */
	u32 total_blks;      /* total blocks in the non-sparse output image */
	u32 total_chunks;    /* total chunks in the sparse input image */
	u32 image_checksum;  /* CRC32 checksum of the original data, counting "don't care" */
				/* as 0. Standard 802.3 polynomial, use a Public Domain */
				/* table implementation */
} sparse_header_t;

#define SPARSE_HEADER_MAGIC     0xed26ff3a

#define CHUNK_TYPE_RAW          0xCAC1
#define CHUNK_TYPE_FILL         0xCAC2
#define CHUNK_TYPE_DONT_CARE    0xCAC3

typedef struct chunk_header {
	u16 chunk_type;    /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
	u16 reserved1;
	u32 chunk_sz;      /* in blocks in output image */
	u32 total_sz;      /* in bytes of chunk input file including chunk header and data */
} chunk_header_t;


struct sparse_ctrl_t {
	sparse_header_t sparse;
	chunk_header_t  chunk;
	u64 chunk_offset;
	u32 read_len;  /* data have read */
	u32 chunk_count;
};

#define SPARSE_HEADER_MAJOR_VER               1

#define IS_SPARSE(_sparse_hdr) \
	(_sparse_hdr->magic == SPARSE_HEADER_MAGIC \
		&& _sparse_hdr->major_version == SPARSE_HEADER_MAJOR_VER \
		&& _sparse_hdr->file_hdr_sz == sizeof(sparse_header_t) \
		&& _sparse_hdr->chunk_hdr_sz == sizeof(chunk_header_t))

#endif /* __SPARSE_H */
