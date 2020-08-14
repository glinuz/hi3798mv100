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
 * base on http://miniz.googlecode.com/files/miniz_v115_r4.7z.
 * The miniz is unlicense, For more information refer to <http://unlicense.org/>.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#ifndef NULL
#  define NULL       (void *)0
#endif

typedef signed char                   int8;
typedef unsigned char                 uint8;

typedef signed short                  int16;
typedef unsigned short                uint16;

typedef signed int                    int32;
typedef unsigned int                  uint32;

#define ESTREAM              -2
#define EMEM                 -4
#define EOK                  0
#define ESTREAM_END          1
#define EDATA                -3
#define EBUF                 -5
#define EPARAM               -10000
#define FALSE                (0)
#define TRUE                 (1)


enum flush_t {
	FLUSH_NO = 0,
	FLUSH_FULL = 3,
	FLUSH_FINISH = 4
};

enum status_t {
	STATUS_BAD_PARAM = -2,
	STATUS_OKAY = 0,
	STATUS_DONE = 1,
};

#define _MAX_HUFF_TABLES       3
#define _MAX_HUFF_SYMBOLS_0    288
#define _MAX_HUFF_SYMBOLS_1    32
#define _MAX_HUFF_SYMBOLS_2    19
#define _LZ_DICT_SIZE          32768
#define _LZ_DICT_SIZE_MASK     (_LZ_DICT_SIZE - 1)
#define _MIN_MATCH_LEN         3
#define _MAX_MATCH_LEN         258
#define _LZ_CODE_BUF_SIZE      64 * 1024
#define _OUT_BUF_SIZE          ((_LZ_CODE_BUF_SIZE * 13 ) / 10)
#define _MAX_HUFF_SYMBOLS      288
#define _LZ_HASH_BITS          15
#define _LEVEL1_HASH_SIZE_MASK 4095;
#define _LZ_HASH_SHIFT         ((_LZ_HASH_BITS + 2) / 3)
#define _LZ_HASH_SIZE          (1 << _LZ_HASH_BITS)

#define _WRITE_ZLIB_HEADER             0x01000
#define _COMPUTE_ADLER32               0x02000
#define _GREEDY_PARSING_FLAG           0x04000
#define _NONDETERMINISTIC_PARSING_FLAG 0x08000
#define _RLE_MATCHES                   0x10000
#define _FILTER_MATCHES                0x20000
#define _FORCE_ALL_STATIC_BLOCKS       0x40000
#define _FORCE_ALL_RAW_BLOCKS          0x80000

#define _MAX_SUPPORTED_HUFF_CODESIZE   32

typedef void *(*fn_os_alloc)(void *opaque, size_t items, size_t size);
typedef void (*fn_os_free)(void *opaque, void *address);

struct zstream_t {
	const unsigned char *next_in;     // pointer to next byte to read
	unsigned int avail_in;            // number of bytes available at next_in
	uint32 total_in;                // total number of bytes consumed so far

	unsigned char *next_out;          // pointer to next byte to write
	unsigned int avail_out;           // number of bytes that can be written to next_out
	uint32 total_out;               // total number of bytes produced so far

	char *msg;                        // error msg (unused)
	void *state;  // internal state, allocated by zalloc/zfree

	fn_os_alloc zalloc;             // optional heap allocation function (defaults to malloc)
	fn_os_free zfree;               // optional heap free function (defaults to free)
	void *opaque;                     // heap alloc function user pointer

	int data_type;                    // data_type (unused)
	uint32 adler;                   // adler32 of the source or uncompressed data
	uint32 reserved;                // not used
};

struct compressor_t {
	uint32 m_flags;
	uint32 m_max_probes[2];
	int m_greedy_parsing;
	uint32 m_adler32;
	uint32 m_lookahead_pos;
	uint32 m_lookahead_size;
	uint32 m_dict_size;
	uint8 *m_pLZ_code_buf;
	uint8 *m_pLZ_flags;
	uint8 *m_pOutput_buf;
	uint8 *m_pOutput_buf_end;
	uint32 m_num_flags_left;
	uint32 m_total_lz_bytes;
	uint32 m_lz_code_buf_dict_pos;
	uint32 m_bits_in;
	uint32 m_bit_buffer;
	uint32 m_saved_match_dist;
	uint32 m_saved_match_len;
	uint32 m_saved_lit;
	uint32 m_output_flush_ofs;
	uint32 m_output_flush_remaining;
	uint32 m_finished;
	uint32 m_block_index;
	uint32 m_wants_to_finish;
	enum status_t m_prev_return_status;
	const void *m_pIn_buf;
	void *m_pOut_buf;
	size_t *m_pIn_buf_size;
	size_t *m_pOut_buf_size;
	enum flush_t m_flush;
	const uint8 *m_pSrc;
	size_t m_src_buf_left, m_out_buf_ofs;
	uint8 m_dict[_LZ_DICT_SIZE + _MAX_MATCH_LEN - 1];
	uint16 m_huff_count[_MAX_HUFF_TABLES][_MAX_HUFF_SYMBOLS];
	uint16 m_huff_codes[_MAX_HUFF_TABLES][_MAX_HUFF_SYMBOLS];
	uint8 m_huff_code_sizes[_MAX_HUFF_TABLES][_MAX_HUFF_SYMBOLS];
	uint8 m_lz_code_buf[_LZ_CODE_BUF_SIZE];
	uint16 m_next[_LZ_DICT_SIZE];
	uint16 m_hash[_LZ_HASH_SIZE];
	uint8 m_output_buf[_OUT_BUF_SIZE];
};
/*****************************************************************************/

static void *os_malloc(void *opaque, size_t items, size_t size) 
{
	(void)opaque, (void)items, (void)size;
	return malloc(items * size);
}

static void os_free(void *opaque, void *address)
{
	(void)opaque, (void)address;
	free(address);
}

static void *os_memset(void *s, int n, size_t size)
{
	return memset(s, n, size);
}

static void *os_memcpy(void *dst, const void *src, size_t size)
{
	return memcpy(dst, src, size);
}

#define OBJ_ZERO(obj)        os_memset(&(obj), 0, sizeof(obj))
#define _MIN(a,b)            (((a)<(b))?(a):(b))
#define _MAX(a,b)            (((a)>(b))?(a):(b))

/*****************************************************************************/

static enum status_t _init(struct compressor_t *pcomp, int flags)
{
	pcomp->m_flags = (uint32)(flags);
	pcomp->m_max_probes[0] = 1 + ((flags & 0xFFF) + 2) / 3;
	pcomp->m_greedy_parsing = (flags & _GREEDY_PARSING_FLAG) != 0;
	pcomp->m_max_probes[1] = 1 + (((flags & 0xFFF) >> 2) + 2) / 3;

	if (!(flags & _NONDETERMINISTIC_PARSING_FLAG))
		OBJ_ZERO(pcomp->m_hash);

	pcomp->m_lookahead_pos = pcomp->m_lookahead_size = pcomp->m_dict_size =
		pcomp->m_total_lz_bytes = pcomp->m_lz_code_buf_dict_pos = pcomp->m_bits_in = 0;
	pcomp->m_output_flush_ofs = pcomp->m_output_flush_remaining = pcomp->m_finished =
		pcomp->m_block_index = pcomp->m_bit_buffer = pcomp->m_wants_to_finish = 0;
	pcomp->m_pLZ_code_buf = pcomp->m_lz_code_buf + 1; pcomp->m_pLZ_flags =
		pcomp->m_lz_code_buf; pcomp->m_num_flags_left = 8;
	pcomp->m_pOutput_buf = pcomp->m_output_buf; pcomp->m_pOutput_buf_end =
		pcomp->m_output_buf;
	pcomp->m_prev_return_status = STATUS_OKAY;
	pcomp->m_saved_match_dist = pcomp->m_saved_match_len = pcomp->m_saved_lit = 0;
	pcomp->m_adler32 = 1;
	pcomp->m_pIn_buf = NULL;
	pcomp->m_pOut_buf = NULL;
	pcomp->m_pIn_buf_size = NULL;
	pcomp->m_pOut_buf_size = NULL;
	pcomp->m_flush = FLUSH_NO;
	pcomp->m_pSrc = NULL;
	pcomp->m_src_buf_left = 0;
	pcomp->m_out_buf_ofs = 0;
	os_memset(&pcomp->m_huff_count[0][0], 0,
		  sizeof(pcomp->m_huff_count[0][0]) * _MAX_HUFF_SYMBOLS_0);
	os_memset(&pcomp->m_huff_count[1][0], 0,
		  sizeof(pcomp->m_huff_count[1][0]) * _MAX_HUFF_SYMBOLS_1);

	return STATUS_OKAY;
}

#define ADLER32_INIT                   1

static uint32 adler32(uint32 adler, const unsigned char *ptr, size_t buf_len)
{
	uint32 i;
	uint32 s1 = adler & 0xffff;
	uint32 s2 = adler >> 16;
	size_t block_len = buf_len % 5552;

	if (!ptr)
		return ADLER32_INIT;

	while (buf_len) {
		for (i = 0; i + 7 < block_len; i += 8, ptr += 8) {
			s1 += ptr[0], s2 += s1; s1 += ptr[1], s2 += s1;
			s1 += ptr[2], s2 += s1; s1 += ptr[3], s2 += s1;
			s1 += ptr[4], s2 += s1; s1 += ptr[5], s2 += s1;
			s1 += ptr[6], s2 += s1; s1 += ptr[7], s2 += s1;
		}
		for ( ; i < block_len; ++i) {
			s1 += *ptr++;
			s2 += s1;
		}
		s1 %= 65521U;
		s2 %= 65521U;
		buf_len -= block_len;
		block_len = 5552;
	}
	return (s2 << 16) + s1;
}

static enum status_t _flush_output_buffer(struct compressor_t *pcomp)
{
	if (pcomp->m_pIn_buf_size)
		*pcomp->m_pIn_buf_size = pcomp->m_pSrc - (const uint8 *)pcomp->m_pIn_buf;

	if (pcomp->m_pOut_buf_size) {
		size_t n = _MIN(*pcomp->m_pOut_buf_size - pcomp->m_out_buf_ofs,
			       pcomp->m_output_flush_remaining);
		os_memcpy((uint8 *)pcomp->m_pOut_buf + pcomp->m_out_buf_ofs,
			   pcomp->m_output_buf + pcomp->m_output_flush_ofs, n);
		pcomp->m_output_flush_ofs += (uint32)n;
		pcomp->m_output_flush_remaining -= (uint32)n;
		pcomp->m_out_buf_ofs += n;

		*pcomp->m_pOut_buf_size = pcomp->m_out_buf_ofs;
	}

	return (pcomp->m_finished && !pcomp->m_output_flush_remaining) ? STATUS_DONE : STATUS_OKAY;
}

#define _PUT_BITS(b, l) do { \
	uint32 bits = b; \
	uint32 len = l; \
	pcomp->m_bit_buffer |= (bits << pcomp->m_bits_in); \
	pcomp->m_bits_in += len; \
	while (pcomp->m_bits_in >= 8) { \
	if (pcomp->m_pOutput_buf < pcomp->m_pOutput_buf_end) \
	*pcomp->m_pOutput_buf++ = (uint8)(pcomp->m_bit_buffer); \
	pcomp->m_bit_buffer >>= 8; \
	pcomp->m_bits_in -= 8; \
	} \
} while (0)

#define _RLE_PREV_CODE_SIZE() { \
	if (rle_repeat_count) { \
		if (rle_repeat_count < 3) { \
			pcomp->m_huff_count[2][prev_code_size] = (uint16)(pcomp->m_huff_count[2][prev_code_size] + rle_repeat_count); \
			while (rle_repeat_count--) \
				packed_code_sizes[num_packed_code_sizes++] = prev_code_size; \
		} else { \
			pcomp->m_huff_count[2][16] = (uint16)(pcomp->m_huff_count[2][16] + 1);\
			packed_code_sizes[num_packed_code_sizes++] = 16; \
			packed_code_sizes[num_packed_code_sizes++] = (uint8)(rle_repeat_count - 3); \
		} \
		rle_repeat_count = 0;\
	} \
}

#define _RLE_ZERO_CODE_SIZE() { \
	if (rle_z_count) { \
		if (rle_z_count < 3) { \
			pcomp->m_huff_count[2][0] = (uint16)(pcomp->m_huff_count[2][0] + rle_z_count); \
			while (rle_z_count--) \
				packed_code_sizes[num_packed_code_sizes++] = 0; \
		} else if (rle_z_count <= 10) { \
			pcomp->m_huff_count[2][17] = (uint16)(pcomp->m_huff_count[2][17] + 1); \
			packed_code_sizes[num_packed_code_sizes++] = 17; \
			packed_code_sizes[num_packed_code_sizes++] = (uint8)(rle_z_count - 3); \
		} else { \
			pcomp->m_huff_count[2][18] = (uint16)(pcomp->m_huff_count[2][18] + 1); \
			packed_code_sizes[num_packed_code_sizes++] = 18; \
			packed_code_sizes[num_packed_code_sizes++] = (uint8)(rle_z_count - 11); \
		} \
		rle_z_count = 0; \
	} \
}

static const uint32 __bitmasks[17] = {
	0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF,
		0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
};

static const uint8 __large_dist_extra[128] = {
	0,0,8,8,9,9,9,9,10,10,10,10,10,10,10,10,
		11,11,11,11,11,11,11,11,11,11,11, 11,11,11,11,11,
		12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12, 12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
		13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
		13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13
};

static const uint8 __large_dist_sym[128] = {
	0,0,18,19,20,20,21,21,22,22,22,22,23,23,23,23,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,26,26,
		26,26,26,26,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
		28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29
};

static const uint8 __len_extra[256] = {
	0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0
};

static const uint16 __len_sym[256] = {
	257,258,259,260,261,262,263,264,265,265,266,266,267,267,268,268,269,269,269,269,270,270,270,270,271,271,271,271,272,272,272,272,
		273,273,273,273,273,273,273,273,274,274,274,274,274,274,274,274,275,275,275,275,275,275,275,275,276,276,276,276,276,276,276,276,
		277,277,277,277,277,277,277,277,277,277,277,277,277,277,277,277,278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,278,
		279,279,279,279,279,279,279,279,279,279,279,279,279,279,279,279,280,280,280,280,280,280,280,280,280,280,280,280,280,280,280,280,
		281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,281,
		282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,282,
		283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,283,
		284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,284,285
};

static uint8 __packed_code_size_syms_swizzle[] = {
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static const uint8 __small_dist_sym[512] = {
	0,1,2,3,4,4,5,5,6,6,6,6,7,7,7,7,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,
		11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,
		13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
		14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
		17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
		17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
		17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17
};

static const uint8 __small_dist_extra[512] = {
	0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7
};

struct sym_freq_t{
	uint16 m_key;
	uint16 m_sym_index;
};

static struct sym_freq_t *_radix_sort_syms(uint32 num_syms, struct sym_freq_t* pSyms0,
					   struct sym_freq_t* pSyms1)
{
	uint32 total_passes = 2;
	uint32 pass_shift;
	uint32 pass;
	uint32 i;
	uint32 hist[256 * 2];
	struct sym_freq_t* pCur_syms = pSyms0;
	struct sym_freq_t *pNew_syms = pSyms1;
	
	OBJ_ZERO(hist);

	for (i = 0; i < num_syms; i++) {
		uint32 freq = pSyms0[i].m_key;
		hist[freq & 0xFF]++;
		hist[256 + ((freq >> 8) & 0xFF)]++;
	}

	while ((total_passes > 1) && (num_syms == hist[(total_passes - 1) * 256]))
		total_passes--;

	for (pass_shift = 0, pass = 0; pass < total_passes; pass++, pass_shift += 8) {
		const uint32* pHist = &hist[pass << 8];
		uint32 offsets[256], cur_ofs = 0;

		for (i = 0; i < 256; i++) {
			offsets[i] = cur_ofs;
			cur_ofs += pHist[i];
		}
		for (i = 0; i < num_syms; i++)
			pNew_syms[offsets[(pCur_syms[i].m_key >> pass_shift) & 0xFF]++] = pCur_syms[i];
		{
			struct sym_freq_t* t = pCur_syms;
			pCur_syms = pNew_syms;
			pNew_syms = t;
		}
	}
	return pCur_syms;
}

static void _calculate_minimum_redundancy(struct sym_freq_t *sym, int n)
{
	int root, leaf, next, avbl, used, dpth;

	if (n==0)
		return;
	else if (n==1) {
		sym[0].m_key = 1;
		return;
	}

	sym[0].m_key += sym[1].m_key; root = 0;
	leaf = 2;

	for (next=1; next < n-1; next++) {
		if (leaf>=n || sym[root].m_key<sym[leaf].m_key) {
			sym[next].m_key = sym[root].m_key;
			sym[root++].m_key = (uint16)next;
		} else
			sym[next].m_key = sym[leaf++].m_key;
		if (leaf>=n || (root<next && sym[root].m_key<sym[leaf].m_key)) {
			sym[next].m_key = (uint16)(sym[next].m_key + sym[root].m_key);
			sym[root++].m_key = (uint16)next;
		} else
			sym[next].m_key = (uint16)(sym[next].m_key + sym[leaf++].m_key);
	}

	sym[n-2].m_key = 0;
	for (next=n-3; next>=0; next--)
		sym[next].m_key = sym[sym[next].m_key].m_key+1;

	avbl = 1;
	used = dpth = 0;
	root = n-2;
	next = n-1;

	while (avbl>0) {
		while (root>=0 && (int)sym[root].m_key==dpth) {
			used++;
			root--;
		}
		while (avbl>used) {
			sym[next--].m_key = (uint16)(dpth);
			avbl--;
		}
		avbl = 2*used;
		dpth++;
		used = 0;
	}
}

static void _huffman_enforce_max_code_size(int *pNum_codes, int code_list_len,
					   int max_code_size)
{
	int i;
	uint32 total = 0;
	
	if (code_list_len <= 1)
		return;

	for (i = max_code_size + 1; i <= _MAX_SUPPORTED_HUFF_CODESIZE; i++)
		pNum_codes[max_code_size] += pNum_codes[i];

	for (i = max_code_size; i > 0; i--)
		total += (((uint32)pNum_codes[i]) << (max_code_size - i));

	while (total != (1UL << max_code_size)) {
		pNum_codes[max_code_size]--;
		for (i = max_code_size - 1; i > 0; i--) 
			if (pNum_codes[i]) {
				pNum_codes[i]--;
				pNum_codes[i + 1] += 2;
				break;
			}
		total--;
	}
}

static void _optimize_huffman_table(struct compressor_t *pcomp, int table_num,
				    int table_len, int code_size_limit,
				    int static_table)
{
	int i, j, l;
	int num_codes[1 + _MAX_SUPPORTED_HUFF_CODESIZE];
	uint32 next_code[_MAX_SUPPORTED_HUFF_CODESIZE + 1];
	
	OBJ_ZERO(num_codes);

	if (static_table) {
		for (i = 0; i < table_len; i++)
			num_codes[pcomp->m_huff_code_sizes[table_num][i]]++;
	} else {
		struct sym_freq_t syms0[_MAX_HUFF_SYMBOLS];
		struct sym_freq_t syms1[_MAX_HUFF_SYMBOLS];
		struct sym_freq_t *pSyms;
		int num_used_syms = 0;
		const uint16 *pSym_count = &pcomp->m_huff_count[table_num][0];

		for (i = 0; i < table_len; i++) {
			if (pSym_count[i]) { 
				syms0[num_used_syms].m_key = (uint16)pSym_count[i];
				syms0[num_used_syms++].m_sym_index = (uint16)i;
			}
		}

		pSyms = _radix_sort_syms(num_used_syms, syms0, syms1);

		_calculate_minimum_redundancy(pSyms, num_used_syms);

		for (i = 0; i < num_used_syms; i++)
			num_codes[pSyms[i].m_key]++;

		_huffman_enforce_max_code_size(num_codes, num_used_syms, code_size_limit);

		OBJ_ZERO(pcomp->m_huff_code_sizes[table_num]);
		OBJ_ZERO(pcomp->m_huff_codes[table_num]);

		for (i = 1, j = num_used_syms; i <= code_size_limit; i++)
			for (l = num_codes[i]; l > 0; l--)
				pcomp->m_huff_code_sizes[table_num][pSyms[--j].m_sym_index] = (uint8)(i);
	}

	next_code[1] = 0;
	
	for (j = 0, i = 2; i <= code_size_limit; i++)
		next_code[i] = j = ((j + num_codes[i - 1]) << 1);

	for (i = 0; i < table_len; i++) {
		uint32 rev_code = 0;
		uint32 code;
		uint32 code_size;
		
		if ((code_size = pcomp->m_huff_code_sizes[table_num][i]) == 0)
			continue;

		code = next_code[code_size]++;

		for (l = code_size; l > 0; l--, code >>= 1)
			rev_code = (rev_code << 1) | (code & 1);

		pcomp->m_huff_codes[table_num][i] = (uint16)rev_code;
	}
}

static void _start_dynamic_block(struct compressor_t *pcomp)
{
	int num_lit_codes;
	int num_dist_codes;
	int num_bit_lengths;
	uint32 i;
	uint32 total_code_sizes_to_pack;
	uint32 num_packed_code_sizes;
	uint32 rle_z_count;
	uint32 rle_repeat_count;
	uint32 packed_code_sizes_index;
	uint8 code_sizes_to_pack[_MAX_HUFF_SYMBOLS_0 + _MAX_HUFF_SYMBOLS_1];
	uint8 packed_code_sizes[_MAX_HUFF_SYMBOLS_0 + _MAX_HUFF_SYMBOLS_1];
	uint8 prev_code_size = 0xFF;

	pcomp->m_huff_count[0][256] = 1;

	_optimize_huffman_table(pcomp, 0, _MAX_HUFF_SYMBOLS_0, 15, FALSE);
	_optimize_huffman_table(pcomp, 1, _MAX_HUFF_SYMBOLS_1, 15, FALSE);

	for (num_lit_codes = 286; num_lit_codes > 257; num_lit_codes--)
		if (pcomp->m_huff_code_sizes[0][num_lit_codes - 1])
			break;
	for (num_dist_codes = 30; num_dist_codes > 1; num_dist_codes--)
		if (pcomp->m_huff_code_sizes[1][num_dist_codes - 1])
			break;

	os_memcpy(code_sizes_to_pack, &pcomp->m_huff_code_sizes[0][0],
		  num_lit_codes);
	os_memcpy(code_sizes_to_pack + num_lit_codes,
		  &pcomp->m_huff_code_sizes[1][0], num_dist_codes);

	total_code_sizes_to_pack = num_lit_codes + num_dist_codes;
	num_packed_code_sizes = 0;
	rle_z_count = 0;
	rle_repeat_count = 0;

	os_memset(&pcomp->m_huff_count[2][0], 0,
		  sizeof(pcomp->m_huff_count[2][0]) * _MAX_HUFF_SYMBOLS_2);

	for (i = 0; i < total_code_sizes_to_pack; i++) {
		uint8 code_size = code_sizes_to_pack[i];
		if (!code_size) {
			_RLE_PREV_CODE_SIZE();
			if (++rle_z_count == 138) {
				_RLE_ZERO_CODE_SIZE();
			}
		} else {
			_RLE_ZERO_CODE_SIZE();
			if (code_size != prev_code_size) {
				_RLE_PREV_CODE_SIZE();
				pcomp->m_huff_count[2][code_size] = (uint16)(pcomp->m_huff_count[2][code_size] + 1);
				packed_code_sizes[num_packed_code_sizes++] = code_size;
			} else if (++rle_repeat_count == 6) {
				_RLE_PREV_CODE_SIZE();
			}
		}
		prev_code_size = code_size;
	}
	if (rle_repeat_count) {
		_RLE_PREV_CODE_SIZE();
	} else { 
		_RLE_ZERO_CODE_SIZE();
	}

	_optimize_huffman_table(pcomp, 2, _MAX_HUFF_SYMBOLS_2, 7, FALSE);

	_PUT_BITS(2, 2);

	_PUT_BITS(num_lit_codes - 257, 5);
	_PUT_BITS(num_dist_codes - 1, 5);

	for (num_bit_lengths = 18; num_bit_lengths >= 0; num_bit_lengths--)
		if (pcomp->m_huff_code_sizes[2][__packed_code_size_syms_swizzle[num_bit_lengths]])
			break;
	num_bit_lengths = _MAX(4, (num_bit_lengths + 1));
	_PUT_BITS(num_bit_lengths - 4, 4);
	for (i = 0; (int)i < num_bit_lengths; i++)
		_PUT_BITS(pcomp->m_huff_code_sizes[2][__packed_code_size_syms_swizzle[i]], 3);

	for (packed_code_sizes_index = 0; packed_code_sizes_index < num_packed_code_sizes; ) {
		uint32 code = packed_code_sizes[packed_code_sizes_index++];
		_PUT_BITS(pcomp->m_huff_codes[2][code], pcomp->m_huff_code_sizes[2][code]);
		if (code >= 16)
			_PUT_BITS(packed_code_sizes[packed_code_sizes_index++], "\02\03\07"[code - 16]);
	}
}

static int32 _compress_lz_codes(struct compressor_t *pcomp)
{
	uint32 flags = 1;
	uint8 *pLZ_codes;

	for (pLZ_codes = pcomp->m_lz_code_buf; pLZ_codes < pcomp->m_pLZ_code_buf; flags >>= 1) {
		if (flags == 1)
			flags = *pLZ_codes++ | 0x100;
		if (flags & 1) {
			uint32 sym;
			uint32 num_extra_bits;
			uint32 match_len = pLZ_codes[0];
			uint32 match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
			
			pLZ_codes += 3;

			_PUT_BITS(pcomp->m_huff_codes[0][__len_sym[match_len]],
				       pcomp->m_huff_code_sizes[0][__len_sym[match_len]]);
			_PUT_BITS(match_len & __bitmasks[__len_extra[match_len]],
				       __len_extra[match_len]);

			if (match_dist < 512) {
				sym = __small_dist_sym[match_dist];
				num_extra_bits = __small_dist_extra[match_dist];
			} else {
				sym = __large_dist_sym[match_dist >> 8];
				num_extra_bits = __large_dist_extra[match_dist >> 8];
			}
			_PUT_BITS(pcomp->m_huff_codes[1][sym],
				       pcomp->m_huff_code_sizes[1][sym]);
			_PUT_BITS(match_dist & __bitmasks[num_extra_bits],
				       num_extra_bits);
		} else {
			uint32 lit = *pLZ_codes++;
			_PUT_BITS(pcomp->m_huff_codes[0][lit],
				       pcomp->m_huff_code_sizes[0][lit]);
		}
	}

	_PUT_BITS(pcomp->m_huff_codes[0][256], pcomp->m_huff_code_sizes[0][256]);

	return (pcomp->m_pOutput_buf < pcomp->m_pOutput_buf_end);
}

static int32 _compress_block(struct compressor_t *pcomp, int32 static_block)
{
	if (static_block) {
		uint32 i;
		uint8 *p = &pcomp->m_huff_code_sizes[0][0];

		for (i = 0; i <= 143; ++i)
			*p++ = 8;
		for ( ; i <= 255; ++i)
			*p++ = 9;
		for ( ; i <= 279; ++i)
			*p++ = 7;
		for ( ; i <= 287; ++i)
			*p++ = 8;

		os_memset(pcomp->m_huff_code_sizes[1], 5, 32);

		_optimize_huffman_table(pcomp, 0, 288, 15, TRUE);
		_optimize_huffman_table(pcomp, 1, 32, 15, TRUE);

		_PUT_BITS(1, 2);
	} else
		_start_dynamic_block(pcomp);
	return _compress_lz_codes(pcomp);
}

static int _flush_block(struct compressor_t *pcomp, int flush)
{
	uint32 saved_bit_buf;
	uint32 saved_bits_in;
	uint8 *pSaved_output_buf;
	int32 comp_block_succeeded = FALSE;
	int n;
	int use_raw_block = ((pcomp->m_flags & _FORCE_ALL_RAW_BLOCKS) != 0) && (pcomp->m_lookahead_pos - pcomp->m_lz_code_buf_dict_pos) <= pcomp->m_dict_size;
	uint8 *pOutput_buf_start = (((*pcomp->m_pOut_buf_size - pcomp->m_out_buf_ofs) >= _OUT_BUF_SIZE)) ? ((uint8 *)pcomp->m_pOut_buf + pcomp->m_out_buf_ofs) : pcomp->m_output_buf;

	pcomp->m_pOutput_buf = pOutput_buf_start;
	pcomp->m_pOutput_buf_end = pcomp->m_pOutput_buf + _OUT_BUF_SIZE - 16;

	pcomp->m_output_flush_ofs = 0;
	pcomp->m_output_flush_remaining = 0;

	*pcomp->m_pLZ_flags = (uint8)(*pcomp->m_pLZ_flags >> pcomp->m_num_flags_left);
	pcomp->m_pLZ_code_buf -= (pcomp->m_num_flags_left == 8);

	if ((pcomp->m_flags & _WRITE_ZLIB_HEADER) && (!pcomp->m_block_index)) {
		_PUT_BITS(0x78, 8);
		_PUT_BITS(0x01, 8);
	}

	_PUT_BITS(flush == FLUSH_FINISH, 1);

	pSaved_output_buf = pcomp->m_pOutput_buf;
	saved_bit_buf = pcomp->m_bit_buffer;
	saved_bits_in = pcomp->m_bits_in;

	if (!use_raw_block) {
		comp_block_succeeded = _compress_block(pcomp,
			(pcomp->m_flags & _FORCE_ALL_STATIC_BLOCKS) || (pcomp->m_total_lz_bytes < 48));
	}

	if ( ((use_raw_block) || ((pcomp->m_total_lz_bytes) && ((pcomp->m_pOutput_buf - pSaved_output_buf + 1U) >= pcomp->m_total_lz_bytes))) &&
	     ((pcomp->m_lookahead_pos - pcomp->m_lz_code_buf_dict_pos) <= pcomp->m_dict_size) ) {
		uint32 i;
		
		pcomp->m_pOutput_buf = pSaved_output_buf;
		pcomp->m_bit_buffer = saved_bit_buf;
		pcomp->m_bits_in = saved_bits_in;

		_PUT_BITS(0, 2);

		if (pcomp->m_bits_in) {
			_PUT_BITS(0, 8 - pcomp->m_bits_in);
		}

		for (i = 2; i; --i, pcomp->m_total_lz_bytes ^= 0xFFFF) {
			_PUT_BITS(pcomp->m_total_lz_bytes & 0xFFFF, 16);
		}

		for (i = 0; i < pcomp->m_total_lz_bytes; ++i) {
			_PUT_BITS(pcomp->m_dict[(pcomp->m_lz_code_buf_dict_pos + i) & _LZ_DICT_SIZE_MASK], 8);
		}
	} else if (!comp_block_succeeded) {
		pcomp->m_pOutput_buf = pSaved_output_buf;
		pcomp->m_bit_buffer = saved_bit_buf;
		pcomp->m_bits_in = saved_bits_in;
		_compress_block(pcomp, TRUE);
	}

	if (flush) {
		if (flush == FLUSH_FINISH) {
			if (pcomp->m_bits_in) {
				_PUT_BITS(0, 8 - pcomp->m_bits_in);
			}
			if (pcomp->m_flags & _WRITE_ZLIB_HEADER) { 
				uint32 i;
				uint32 a = pcomp->m_adler32;
				
				for (i = 0; i < 4; i++) {
					_PUT_BITS((a >> 24) & 0xFF, 8);
					a <<= 8;
				}
			}
		} else {
			uint32 i;
			uint32 z = 0;
			
			_PUT_BITS(0, 3);
			
			if (pcomp->m_bits_in) {
				_PUT_BITS(0, 8 - pcomp->m_bits_in);
			}
			
			for (i = 2; i; --i, z ^= 0xFFFF) { 
				_PUT_BITS(z & 0xFFFF, 16);
			}
		}
	}

	os_memset(&pcomp->m_huff_count[0][0], 0,
		  sizeof(pcomp->m_huff_count[0][0]) * _MAX_HUFF_SYMBOLS_0);
	os_memset(&pcomp->m_huff_count[1][0], 0,
		  sizeof(pcomp->m_huff_count[1][0]) * _MAX_HUFF_SYMBOLS_1);

	pcomp->m_pLZ_code_buf = pcomp->m_lz_code_buf + 1;
	pcomp->m_pLZ_flags = pcomp->m_lz_code_buf;
	pcomp->m_num_flags_left = 8;
	pcomp->m_lz_code_buf_dict_pos += pcomp->m_total_lz_bytes;
	pcomp->m_total_lz_bytes = 0;
	pcomp->m_block_index++;

	if ((n = (int)(pcomp->m_pOutput_buf - pOutput_buf_start)) != 0) {
		if (pOutput_buf_start == pcomp->m_output_buf) {
			int bytes_to_copy = (int)_MIN((size_t)n,
				(size_t)(*pcomp->m_pOut_buf_size - pcomp->m_out_buf_ofs));

			os_memcpy((uint8 *)pcomp->m_pOut_buf + pcomp->m_out_buf_ofs,
				  pcomp->m_output_buf, bytes_to_copy);

			pcomp->m_out_buf_ofs += bytes_to_copy;

			if ((n -= bytes_to_copy) != 0) {
				pcomp->m_output_flush_ofs = bytes_to_copy;
				pcomp->m_output_flush_remaining = n;
			}
		} else {
			pcomp->m_out_buf_ofs += n;
		}
	}

	return pcomp->m_output_flush_remaining;
}

static void _find_match(struct compressor_t *pcomp, uint32 lookahead_pos,
			uint32 max_dist, uint32 max_match_len,
			uint32 *pMatch_dist, uint32 *pMatch_len)
{
	uint32 dist;
	uint32 pos = lookahead_pos & _LZ_DICT_SIZE_MASK;
	uint32 match_len = *pMatch_len;
	uint32 probe_pos = pos;
	uint32 next_probe_pos;
	uint32 probe_len;
	uint32 num_probes_left = pcomp->m_max_probes[match_len >= 32];
	const uint8 *s = pcomp->m_dict + pos, *p, *q;
	uint8 c0 = pcomp->m_dict[pos + match_len], c1 = pcomp->m_dict[pos + match_len - 1];

#define _PROBE \
	next_probe_pos = pcomp->m_next[probe_pos]; \
	if ((!next_probe_pos) || ((dist = (uint16)(lookahead_pos - next_probe_pos)) > max_dist)) \
	return; \
	probe_pos = next_probe_pos & _LZ_DICT_SIZE_MASK; \
	if ((pcomp->m_dict[probe_pos + match_len] == c0) && (pcomp->m_dict[probe_pos + match_len - 1] == c1)) \
	break;

	if (max_match_len <= match_len)
		return;

	while (1) {
		while (1) {
			if (--num_probes_left == 0)
				return;
			_PROBE; _PROBE; _PROBE;
		}

		if (!dist)
			break;
		p = s;
		q = pcomp->m_dict + probe_pos;
		for (probe_len = 0; probe_len < max_match_len; probe_len++)
			if (*p++ != *q++)
				break;
		if (probe_len > match_len) {
			*pMatch_dist = dist;
			if ((*pMatch_len = match_len = probe_len) == max_match_len)
				return;
			c0 = pcomp->m_dict[pos + match_len];
			c1 = pcomp->m_dict[pos + match_len - 1];
		}
	}
#undef _PROBE
}

static  void _record_literal(struct compressor_t *pcomp, uint8 lit)
{
	pcomp->m_total_lz_bytes++;
	*pcomp->m_pLZ_code_buf++ = lit;
	*pcomp->m_pLZ_flags = (uint8)(*pcomp->m_pLZ_flags >> 1);
	if (--pcomp->m_num_flags_left == 0) {
		pcomp->m_num_flags_left = 8;
		pcomp->m_pLZ_flags = pcomp->m_pLZ_code_buf++;
	}
	pcomp->m_huff_count[0][lit]++;
}

static void _record_match(struct compressor_t *pcomp, uint32 match_len, uint32 match_dist)
{
	uint32 s0, s1;

	pcomp->m_total_lz_bytes += match_len;
	pcomp->m_pLZ_code_buf[0] = (uint8)(match_len - _MIN_MATCH_LEN);
	match_dist -= 1;
	pcomp->m_pLZ_code_buf[1] = (uint8)(match_dist & 0xFF);
	pcomp->m_pLZ_code_buf[2] = (uint8)(match_dist >> 8); pcomp->m_pLZ_code_buf += 3;
	*pcomp->m_pLZ_flags = (uint8)((*pcomp->m_pLZ_flags >> 1) | 0x80);
	
	if (--pcomp->m_num_flags_left == 0) {
		pcomp->m_num_flags_left = 8;
		pcomp->m_pLZ_flags = pcomp->m_pLZ_code_buf++;
	}

	s0 = __small_dist_sym[match_dist & 511];
	s1 = __large_dist_sym[(match_dist >> 8) & 127];
	pcomp->m_huff_count[1][(match_dist < 512) ? s0 : s1]++;

	if (match_len >= _MIN_MATCH_LEN)
		pcomp->m_huff_count[0][__len_sym[match_len - _MIN_MATCH_LEN]]++;
}

static int32 _compress_normal(struct compressor_t *pcomp)
{
	const uint8 *pSrc = pcomp->m_pSrc;
	size_t src_buf_left = pcomp->m_src_buf_left;
	enum flush_t flush = pcomp->m_flush;

	while ((src_buf_left) || ((flush) && (pcomp->m_lookahead_size))) {
		uint32 len_to_move;
		uint32 cur_match_dist;
		uint32 cur_match_len;
		uint32 cur_pos;

		if ((pcomp->m_lookahead_size + pcomp->m_dict_size) >= (_MIN_MATCH_LEN - 1)) {
			uint32 dst_pos = (pcomp->m_lookahead_pos + pcomp->m_lookahead_size) & _LZ_DICT_SIZE_MASK;
			uint32 ins_pos = pcomp->m_lookahead_pos + pcomp->m_lookahead_size - 2;
			uint32 hash = (pcomp->m_dict[ins_pos & _LZ_DICT_SIZE_MASK] << _LZ_HASH_SHIFT) ^ pcomp->m_dict[(ins_pos + 1) & _LZ_DICT_SIZE_MASK];
			uint32 num_bytes_to_process = (uint32)_MIN(src_buf_left, _MAX_MATCH_LEN - pcomp->m_lookahead_size);
			const uint8 *pSrc_end = pSrc + num_bytes_to_process;

			src_buf_left -= num_bytes_to_process;
			pcomp->m_lookahead_size += num_bytes_to_process;
			while (pSrc != pSrc_end) {
				uint8 c = *pSrc++; pcomp->m_dict[dst_pos] = c;
				if (dst_pos < (_MAX_MATCH_LEN - 1))
					pcomp->m_dict[_LZ_DICT_SIZE + dst_pos] = c;
				hash = ((hash << _LZ_HASH_SHIFT) ^ c) & (_LZ_HASH_SIZE - 1);
				pcomp->m_next[ins_pos & _LZ_DICT_SIZE_MASK] = pcomp->m_hash[hash];
				pcomp->m_hash[hash] = (uint16)(ins_pos);
				dst_pos = (dst_pos + 1) & _LZ_DICT_SIZE_MASK;
				ins_pos++;
			}
		} else {
			while ((src_buf_left) && (pcomp->m_lookahead_size < _MAX_MATCH_LEN)) {
				uint8 c = *pSrc++;
				uint32 dst_pos = (pcomp->m_lookahead_pos + pcomp->m_lookahead_size) & _LZ_DICT_SIZE_MASK;

				src_buf_left--;
				pcomp->m_dict[dst_pos] = c;
				if (dst_pos < (_MAX_MATCH_LEN - 1))
					pcomp->m_dict[_LZ_DICT_SIZE + dst_pos] = c;
				if ((++pcomp->m_lookahead_size + pcomp->m_dict_size) >= _MIN_MATCH_LEN) {
					uint32 ins_pos = pcomp->m_lookahead_pos + (pcomp->m_lookahead_size - 1) - 2;
					uint32 hash = ((pcomp->m_dict[ins_pos & _LZ_DICT_SIZE_MASK] << (_LZ_HASH_SHIFT * 2)) ^ (pcomp->m_dict[(ins_pos + 1) & _LZ_DICT_SIZE_MASK] << _LZ_HASH_SHIFT) ^ c) & (_LZ_HASH_SIZE - 1);
					pcomp->m_next[ins_pos & _LZ_DICT_SIZE_MASK] = pcomp->m_hash[hash];
					pcomp->m_hash[hash] = (uint16)(ins_pos);
				}
			}
		}
		pcomp->m_dict_size = _MIN(_LZ_DICT_SIZE - pcomp->m_lookahead_size, pcomp->m_dict_size);
		if ((!flush) && (pcomp->m_lookahead_size < _MAX_MATCH_LEN))
			break;

		// Simple lazy/greedy parsing state machine.
		len_to_move = 1;
		cur_match_dist = 0;
		cur_match_len = pcomp->m_saved_match_len ? pcomp->m_saved_match_len : (_MIN_MATCH_LEN - 1);
		cur_pos = pcomp->m_lookahead_pos & _LZ_DICT_SIZE_MASK;
		if (pcomp->m_flags & (_RLE_MATCHES | _FORCE_ALL_RAW_BLOCKS)) {
			if ((pcomp->m_dict_size) && (!(pcomp->m_flags & _FORCE_ALL_RAW_BLOCKS))) {
				uint8 c = pcomp->m_dict[(cur_pos - 1) & _LZ_DICT_SIZE_MASK];
				cur_match_len = 0;
				while (cur_match_len < pcomp->m_lookahead_size) {
					if (pcomp->m_dict[cur_pos + cur_match_len] != c)
						break;
					cur_match_len++;
				}
				if (cur_match_len < _MIN_MATCH_LEN)
					cur_match_len = 0;
				else
					cur_match_dist = 1;
			}
		} else {
			_find_match(pcomp, pcomp->m_lookahead_pos,
					 pcomp->m_dict_size, pcomp->m_lookahead_size,
					 &cur_match_dist, &cur_match_len);
		}

		if (((cur_match_len == _MIN_MATCH_LEN) && (cur_match_dist >= 8U*1024U)) ||
		    (cur_pos == cur_match_dist) ||
		    ((pcomp->m_flags & _FILTER_MATCHES) && (cur_match_len <= 5)))
			cur_match_dist = cur_match_len = 0;

		if (pcomp->m_saved_match_len) {
			if (cur_match_len > pcomp->m_saved_match_len) {
				_record_literal(pcomp, (uint8)pcomp->m_saved_lit);
				if (cur_match_len >= 128) {
					_record_match(pcomp, cur_match_len, cur_match_dist);
					pcomp->m_saved_match_len = 0;
					len_to_move = cur_match_len;
				} else {
					pcomp->m_saved_lit = pcomp->m_dict[cur_pos];
					pcomp->m_saved_match_dist = cur_match_dist;
					pcomp->m_saved_match_len = cur_match_len;
				}
			} else {
				_record_match(pcomp, pcomp->m_saved_match_len,
						   pcomp->m_saved_match_dist);
				len_to_move = pcomp->m_saved_match_len - 1;
				pcomp->m_saved_match_len = 0;
			}
		}
		else if (!cur_match_dist)
			_record_literal(pcomp, pcomp->m_dict[_MIN(cur_pos, sizeof(pcomp->m_dict) - 1)]);
		else if ((pcomp->m_greedy_parsing) || (pcomp->m_flags & _RLE_MATCHES) || (cur_match_len >= 128)) {
			_record_match(pcomp, cur_match_len, cur_match_dist);
			len_to_move = cur_match_len;
		} else {
			pcomp->m_saved_lit = pcomp->m_dict[_MIN(cur_pos, sizeof(pcomp->m_dict) - 1)];
			pcomp->m_saved_match_dist = cur_match_dist;
			pcomp->m_saved_match_len = cur_match_len;
		}
		pcomp->m_lookahead_pos += len_to_move;
		pcomp->m_lookahead_size -= len_to_move;
		pcomp->m_dict_size = _MIN(pcomp->m_dict_size + len_to_move, _LZ_DICT_SIZE);

		if ( (pcomp->m_pLZ_code_buf > &pcomp->m_lz_code_buf[_LZ_CODE_BUF_SIZE - 8]) ||
		     ( (pcomp->m_total_lz_bytes > 31*1024) &&
		       (((((uint32)(pcomp->m_pLZ_code_buf - pcomp->m_lz_code_buf) * 115) >> 7) >= pcomp->m_total_lz_bytes) ||
		        (pcomp->m_flags & _FORCE_ALL_RAW_BLOCKS))) ) {
			int n;
			pcomp->m_pSrc = pSrc; pcomp->m_src_buf_left = src_buf_left;
			if ((n = _flush_block(pcomp, 0)) != 0)
				return (n < 0) ? FALSE : TRUE;
		}
	}

	pcomp->m_pSrc = pSrc;
	pcomp->m_src_buf_left = src_buf_left;
	return TRUE;
}

enum status_t _compress(struct compressor_t *pcomp, const void *pIn_buf,
			size_t *pIn_buf_size, void *pOut_buf,
			size_t *pOut_buf_size, enum flush_t flush)
{
	if (!pcomp) {
		if (pIn_buf_size)
			*pIn_buf_size = 0;
		if (pOut_buf_size)
			*pOut_buf_size = 0;
		return STATUS_BAD_PARAM;
	}

	pcomp->m_pIn_buf = pIn_buf;
	pcomp->m_pIn_buf_size = pIn_buf_size;
	pcomp->m_pOut_buf = pOut_buf;
	pcomp->m_pOut_buf_size = pOut_buf_size;
	pcomp->m_pSrc = (const uint8 *)(pIn_buf);
	pcomp->m_src_buf_left = pIn_buf_size ? *pIn_buf_size : 0;
	pcomp->m_out_buf_ofs = 0;
	pcomp->m_flush = flush;

	if ((0 == ((pOut_buf != NULL) || (pOut_buf_size != NULL))) ||
	    (pcomp->m_prev_return_status != STATUS_OKAY) ||
	    (pcomp->m_wants_to_finish && (flush != FLUSH_FINISH)) ||
	    (pIn_buf_size && *pIn_buf_size && !pIn_buf) ||
	    (pOut_buf_size && *pOut_buf_size && !pOut_buf) ) {
		if (pIn_buf_size)
			*pIn_buf_size = 0;
		if (pOut_buf_size)
			*pOut_buf_size = 0;
		return (pcomp->m_prev_return_status = STATUS_BAD_PARAM);
	}
	pcomp->m_wants_to_finish |= (flush == FLUSH_FINISH);

	if ((pcomp->m_output_flush_remaining) || (pcomp->m_finished))
		return (pcomp->m_prev_return_status = _flush_output_buffer(pcomp));

	if (!_compress_normal(pcomp))
		return pcomp->m_prev_return_status;

	if ((pcomp->m_flags & (_WRITE_ZLIB_HEADER | _COMPUTE_ADLER32)) && (pIn_buf))
		pcomp->m_adler32 = (uint32)adler32(pcomp->m_adler32, (const uint8 *)pIn_buf,
						   pcomp->m_pSrc - (const uint8 *)pIn_buf);

	if ((flush) && (!pcomp->m_lookahead_size) &&
	    (!pcomp->m_src_buf_left) && (!pcomp->m_output_flush_remaining)) {
		if (_flush_block(pcomp, flush) < 0)
			return pcomp->m_prev_return_status;
		pcomp->m_finished = (flush == FLUSH_FINISH);
		if (flush == FLUSH_FULL) {
			OBJ_ZERO(pcomp->m_hash);
			OBJ_ZERO(pcomp->m_next);
			pcomp->m_dict_size = 0;
		}
	}

	return (pcomp->m_prev_return_status = _flush_output_buffer(pcomp));
}

static int deflate_end(struct zstream_t * pstream)
{
	if (!pstream)
		return ESTREAM;
	if (pstream->state) {
		pstream->zfree(pstream->opaque, pstream->state);
		pstream->state = NULL;
	}
	return EOK;
}

static int deflate_init(struct zstream_t *pstream)
{
	struct compressor_t *pcomp;

	if (!pstream)
		return ESTREAM;

	pstream->data_type = 0;
	pstream->adler = ADLER32_INIT;
	pstream->msg = NULL;
	pstream->reserved = 0;
	pstream->total_in = 0;
	pstream->total_out = 0;

	if (!pstream->zalloc)
		pstream->zalloc = os_malloc;
	if (!pstream->zfree)
		pstream->zfree = os_free;

	pcomp = (struct compressor_t *)pstream->zalloc(pstream->opaque, 1,
		sizeof(struct compressor_t));
	if (!pcomp)
		return EMEM;

	pstream->state = pcomp;

	if (_init(pcomp, 0x7080) != STATUS_OKAY) {
		deflate_end(pstream);
		return EPARAM;
	}

	return EOK;
}

static int deflate(struct zstream_t *pstream)
{
	size_t in_bytes;
	size_t out_bytes;
	uint32 orig_total_in;
	uint32 orig_total_out;
	struct compressor_t *pcomp = (struct compressor_t *)pstream->state;

	if (!pstream || !pcomp || !pstream->next_out)
		return ESTREAM;

	if (!pstream->avail_out)
		return EBUF;

	if (pcomp->m_prev_return_status == STATUS_DONE)
		return ESTREAM_END;

	orig_total_in = pstream->total_in;
	orig_total_out = pstream->total_out;
	while (1) {
		enum status_t defl_status;

		in_bytes = pstream->avail_in;
		out_bytes = pstream->avail_out;

		defl_status = _compress(pcomp, pstream->next_in,
					     &in_bytes, pstream->next_out,
					     &out_bytes, FLUSH_FINISH);
		pstream->next_in += (uint32)in_bytes;
		pstream->avail_in -= (uint32)in_bytes;
		pstream->total_in += (uint32)in_bytes;
		pstream->adler = pcomp->m_adler32;

		pstream->next_out += (uint32)out_bytes;
		pstream->avail_out -= (uint32)out_bytes;
		pstream->total_out += (uint32)out_bytes;

		if (defl_status < 0) {
			return ESTREAM;
		} else if (defl_status == STATUS_DONE) {
			return ESTREAM_END;
		} else if (!pstream->avail_out) {
			break;
		}
	}
	return EOK;
}

int compress(unsigned char *compbuf, uint32 *sz_compbuf,
	     const unsigned char *uncompbuf, uint32 sz_uncompbuf)
{
	int status;
	struct zstream_t stream;

	os_memset(&stream, 0, sizeof(stream));

	if ((sz_uncompbuf | *sz_compbuf) > 0xFFFFFFFFU)
		return EPARAM;

	stream.next_in = uncompbuf;
	stream.avail_in = sz_uncompbuf;
	stream.next_out = compbuf;
	stream.avail_out = *sz_compbuf;

	status = deflate_init(&stream);
	if (status != EOK)
		return status;

	status = deflate(&stream);
	if (status != ESTREAM_END) {
		deflate_end(&stream);
		return (status == EOK) ? EBUF : status;
	}

	*sz_compbuf = stream.total_out;
	return deflate_end(&stream);
}
/*****************************************************************************/

int main(int argc, char *argv[])
{
	int ret = -1;
	FILE *file_uncomp = NULL;
	FILE *file_comp = NULL;
	const char *name_uncomp = NULL;
	const char *name_comp = NULL;

	int sz_uncomp;
	char *buf_uncomp = NULL;
	int sz_comp;
	char *buf_comp;

	if (argc < 3) {
		fprintf(stderr, "usage: deflate [uncompress file] [compress file]\n");
		return 0;
	}

	name_uncomp = argv[1];
	name_comp = argv[2];

	file_uncomp = fopen(name_uncomp, "rb");
	if (!file_uncomp) {
		fprintf(stderr, "open uncompress file `%s` fail. errno(%d):%s\n",
			name_uncomp, errno, strerror(errno));
		ret = -errno;
		goto fail;
	}

	file_comp = fopen(name_comp, "wb");
	if (!file_comp) {
		fprintf(stderr, "open compress file `%s` fail. errno(%d):%s\n",
			name_comp, errno, strerror(errno));
		ret = -errno;
		goto fail;
	}

	fseek(file_uncomp, 0, SEEK_END);
	sz_uncomp = ftell(file_uncomp);
	if (sz_uncomp < 0) {
		fprintf(stderr, "get uncompress file `%s` size fail. errno(%d):%s\n",
			name_uncomp, errno, strerror(errno));
		ret = -errno;
		goto fail;
	}
	sz_comp = sz_uncomp;

	buf_uncomp = (char *)malloc(sz_uncomp + sz_comp);
	if (!buf_uncomp) {
		fprintf(stderr, "can't malloc memory.\n");
		ret = -1;
		goto fail;
	}
	buf_comp = buf_uncomp + sz_uncomp;

	fseek(file_uncomp, 0, SEEK_SET);
	ret = (int)fread(buf_uncomp, 1, sz_uncomp, file_uncomp);
	if (ret != sz_uncomp) {
		fprintf(stderr, "read uncompress file `%s` fail. errno(%d):%s\n",
			name_uncomp, errno, strerror(errno));
		ret = -errno;
		goto fail;
	}

	ret = compress(buf_comp, (unsigned int *)&sz_comp, buf_uncomp,
		       (unsigned int)sz_uncomp);
	if (ret != 0) {
		fprintf(stderr, "compress fail.\n");
		ret = -1;
		goto fail;
	}

	ret = (int)fwrite(buf_comp, 1, sz_comp, file_comp);
	if (ret != sz_comp) {
		fprintf(stderr, "write compress file `%s` fail. errno(%d):%s\n",
			name_comp, errno, strerror(errno));
		ret = -errno;
		goto fail;
	}

	ret = 0;
fail:
	if (file_uncomp)
		fclose(file_uncomp);
	if (file_comp)
		fclose(file_comp);
	if (buf_uncomp)
		free(buf_uncomp);

	return ret;
}
