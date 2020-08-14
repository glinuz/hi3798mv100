/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>

#include <sparse/sparse.h>

#include "ext4_utils.h"
#include "make_ext4fs.h"
#include "allocate.h"

#if defined(__APPLE__) && defined(__MACH__)
#define off64_t off_t
#endif

#ifndef USE_MINGW /* O_BINARY is windows-specific flag */
#define O_BINARY 0
#endif

extern struct fs_info info;

static int verbose = 0;

static void usage(char *path)
{
	fprintf(stderr, "%s [ options ] <image or block device> <output image>\n", path);
	fprintf(stderr, "\n");
	fprintf(stderr, "  -c include CRC block\n");
	fprintf(stderr, "  -v verbose output\n");
	fprintf(stderr, "  -z gzip output\n");
	fprintf(stderr, "  -S don't use sparse output format\n");
}

static int read_ext(int fd)
{
	off64_t ret;
	struct ext4_super_block sb;

	ret = lseek64(fd, 1024, SEEK_SET);
	if (ret < 0)
		critical_error_errno("failed to seek to superblock");

	ret = read(fd, &sb, sizeof(sb));
	if (ret < 0)
		critical_error_errno("failed to read superblock");
	if (ret != sizeof(sb))
		critical_error("failed to read all of superblock");

	ext4_parse_sb(&sb);

	ret = lseek64(fd, info.len, SEEK_SET);
	if (ret < 0)
		critical_error_errno("failed to seek to end of input image");

	ret = lseek64(fd, info.block_size * (aux_info.first_data_block + 1), SEEK_SET);
	if (ret < 0)
		critical_error_errno("failed to seek to block group descriptors");

	ret = read(fd, aux_info.bg_desc, info.block_size * aux_info.bg_desc_blocks);
	if (ret < 0)
		critical_error_errno("failed to read block group descriptors");
	if (ret != (int)info.block_size * (int)aux_info.bg_desc_blocks)
		critical_error("failed to read all of block group descriptors");

	if (verbose) {
		printf("Found filesystem with parameters:\n");
		printf("    Size: %llu\n", info.len);
		printf("    Block size: %d\n", info.block_size);
		printf("    Blocks per group: %d\n", info.blocks_per_group);
		printf("    Inodes per group: %d\n", info.inodes_per_group);
		printf("    Inode size: %d\n", info.inode_size);
		printf("    Label: %s\n", info.label);
		printf("    Blocks: %llu\n", aux_info.len_blocks);
		printf("    Block groups: %d\n", aux_info.groups);
		printf("    Reserved block group size: %d\n", info.bg_desc_reserve_blocks);
		printf("    Used %d/%d inodes and %d/%d blocks\n",
				aux_info.sb->s_inodes_count - aux_info.sb->s_free_inodes_count,
				aux_info.sb->s_inodes_count,
				aux_info.sb->s_blocks_count_lo - aux_info.sb->s_free_blocks_count_lo,
				aux_info.sb->s_blocks_count_lo);
	}

	return 0;
}

static int bitmap_get_bit(u8 *bitmap, u32 bit)
{
	if (bitmap[bit / 8] & 1 << (bit % 8))
		return 1;

	return 0;
}

static int build_sparse_ext(int fd, const char *filename)
{
	unsigned int i;
	unsigned int block;
	int start_contiguous_block;
	u8 *block_bitmap;
	off64_t ret;

	block_bitmap = malloc(info.block_size);
	if (!block_bitmap)
		critical_error("failed to allocate block bitmap");

	if (aux_info.first_data_block > 0)
		sparse_file_add_file(info.sparse_file, filename, 0,
				info.block_size * aux_info.first_data_block, 0);

	for (i = 0; i < aux_info.groups; i++) {
		u32 first_block = aux_info.first_data_block + i * info.blocks_per_group;
		u32 last_block = min(info.blocks_per_group, aux_info.len_blocks - first_block);

		ret = lseek64(fd, (u64)info.block_size * aux_info.bg_desc[i].bg_block_bitmap,
				SEEK_SET);
		if (ret < 0)
			critical_error_errno("failed to seek to block group bitmap %d", i);

		ret = read(fd, block_bitmap, info.block_size);
		if (ret < 0)
			critical_error_errno("failed to read block group bitmap %d", i);
		if (ret != (int)info.block_size)
			critical_error("failed to read all of block group bitmap %d", i);

		start_contiguous_block = -1;
		for (block = 0; block < last_block; block++) {
			if (start_contiguous_block >= 0) {
				if (!bitmap_get_bit(block_bitmap, block)) {
					u32 start_block = first_block + start_contiguous_block;
					u32 len_blocks = block - start_contiguous_block;

					sparse_file_add_file(info.sparse_file, filename,
							(u64)info.block_size * start_block,
							info.block_size * len_blocks, start_block);
					start_contiguous_block = -1;
				}
			} else {
				if (bitmap_get_bit(block_bitmap, block))
					start_contiguous_block = block;
			}
		}

		if (start_contiguous_block >= 0) {
			u32 start_block = first_block + start_contiguous_block;
			u32 len_blocks = last_block - start_contiguous_block;
			sparse_file_add_file(info.sparse_file, filename,
					(u64)info.block_size * start_block,
					info.block_size * len_blocks, start_block);
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	int opt;
	const char *in = NULL;
	const char *out = NULL;
	int gzip = 0;
	int sparse = 1;
	int infd, outfd;
	int crc = 0;

	while ((opt = getopt(argc, argv, "cvzS")) != -1) {
		switch (opt) {
		case 'c':
			crc = 1;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'z':
			gzip = 1;
			break;
		case 'S':
			sparse = 0;
			break;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "Expected image or block device after options\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	in = argv[optind++];

	if (optind >= argc) {
		fprintf(stderr, "Expected output image after input image\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	out = argv[optind++];

	if (optind < argc) {
		fprintf(stderr, "Unexpected argument: %s\n", argv[optind]);
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	infd = open(in, O_RDONLY);

	if (infd < 0)
		critical_error_errno("failed to open input image");

	read_ext(infd);

	info.sparse_file = sparse_file_new(info.block_size, info.len);

	build_sparse_ext(infd, in);

	close(infd);

	if (strcmp(out, "-")) {
		outfd = open(out, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
		if (outfd < 0) {
			error_errno("open");
			return EXIT_FAILURE;
		}
	} else {
		outfd = STDOUT_FILENO;
	}

	write_ext4_image(outfd, gzip, sparse, crc);
	close(outfd);

	sparse_file_destroy(info.sparse_file);

	return 0;
}
