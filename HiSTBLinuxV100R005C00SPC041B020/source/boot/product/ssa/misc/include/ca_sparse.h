#ifndef __CA_SPARSE_H__
#define __CA_SPARSE_H__

//#include <linux/types.h>

typedef struct sparse_header {
	unsigned int magic;           /* 0xed26ff3a */
	unsigned short major_version;   /* (0x1) - reject images with higher major versions */
	unsigned short minor_version;   /* (0x0) - allow images with higer minor versions */
	unsigned short file_hdr_sz;     /* 28 bytes for first revision of the file format */
	unsigned short chunk_hdr_sz;    /* 12 bytes for first revision of the file format */
	unsigned int blk_sz;          /* block size in bytes, must be a multiple of 4 (4096) */
	unsigned int total_blks;      /* total blocks in the non-sparse output image */
	unsigned int total_chunks;    /* total chunks in the sparse input image */
	unsigned int image_checksum;  /* CRC32 checksum of the original data, counting "don't care" */
				/* as 0. Standard 802.3 polynomial, use a Public Domain */
				/* table implementation */
} sparse_header_t;

#define SPARSE_HEADER_MAGIC     0xed26ff3a

#define CHUNK_TYPE_RAW          0xCAC1
#define CHUNK_TYPE_FILL         0xCAC2
#define CHUNK_TYPE_DONT_CARE    0xCAC3

typedef struct chunk_header {
	unsigned short chunk_type;    /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
	unsigned short reserved1;
	unsigned int chunk_sz;      /* in blocks in output image */
	unsigned int total_sz;      /* in bytes of chunk input file including chunk header and data */
} chunk_header_t;

typedef struct sign_chunk_header {
	unsigned short chunk_type;  /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
	unsigned short num;       /* chunk块序号 */
	unsigned int chunk_sz;    /* 数据长度 */
	unsigned int offset;      /* 相对偏移位置 */
} sign_chunk_header_t;

#define SPARSE_HEADER_MAJOR_VER               1

#define IS_SPARSE(_sparse_hdr) \
	(_sparse_hdr->magic == SPARSE_HEADER_MAGIC \
		&& _sparse_hdr->major_version == SPARSE_HEADER_MAJOR_VER \
		&& _sparse_hdr->file_hdr_sz == sizeof(sparse_header_t))

#endif /* __SPARSE_H__ */
