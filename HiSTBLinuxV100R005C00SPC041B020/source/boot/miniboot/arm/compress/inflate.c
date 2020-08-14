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
 * base on http://miniz.googlecode.com/files/miniz_v115_r4.7z.
 * The miniz is unlicense, For more information refer to <http://unlicense.org/>.
******************************************************************************/

#define NULL       (void *)0

typedef signed char                   int8;
typedef signed short                  int16;
typedef signed int                    int32;

typedef unsigned char                 uint8;
typedef unsigned short                uint16;
typedef unsigned int                  uint32;
typedef uint32                        size_t;

#define ESTREAM              -2
#define EMEM                 -4
#define EOK                  0
#define ESTREAM_END          1
#define EDATA                -3
#define EBUF                 -5
#define EPARAM               -10000
#define FALSE                (0)
#define TRUE                 (1)

#define OBJ_ZERO(obj)        os_memset(&(obj), 0, sizeof(obj))
#define _MIN(a,b)            (((a)<(b))?(a):(b))
#define _MAX(a,b)            (((a)>(b))?(a):(b))

#define _FLAG_PARSE_ZLIB_HEADER                  1
#define _FLAG_HAS_MORE_INPUT                     2
#define _FLAG_USING_NON_WRAPPING_OUTPUT_BUF      4
#define _FLAG_COMPUTE_ADLER32                    8

#define _MAX_HUFF_TABLES                         3
#define _MAX_HUFF_SYMBOLS_0                      288
#define _MAX_HUFF_SYMBOLS_1                      32
#define _FAST_LOOKUP_BITS                        10
#define _FAST_LOOKUP_SIZE                        ( 1 << _FAST_LOOKUP_BITS)

#define _LZ_DICT_SIZE                            32768


enum status_t {
	STATUS_BAD_PARAM        = -3,
	STATUS_ADLER32_MISMATCH = -2,
	STATUS_FAILED           = -1,
	STATUS_DONE             = 0,
	STATUS_NEEDS_MORE_INPUT = 1,
	STATUS_HAS_MORE_OUTPUT  = 2
};


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

struct huff_table_t {
	uint8 m_code_size[_MAX_HUFF_SYMBOLS_0];
	int16 m_look_up[_FAST_LOOKUP_SIZE];
	int16 m_tree[_MAX_HUFF_SYMBOLS_0 * 2];
};

struct decompressor_t {
	uint32 m_state;
	uint32 m_num_bits;
	uint32 m_zhdr0;
	uint32 m_zhdr1;
	uint32 m_z_adler32;
	uint32 m_final;
	uint32 m_type;
	uint32 m_check_adler32;
	uint32 m_dist;
	uint32 m_counter;
	uint32 m_num_extra;
	uint32 m_table_sizes[_MAX_HUFF_TABLES];
	uint32 m_bit_buf;
	size_t m_dist_from_out_buf_start;
	struct huff_table_t m_tables[_MAX_HUFF_TABLES];
	uint8 m_raw_header[4];
	uint8 m_len_codes[_MAX_HUFF_SYMBOLS_0 + _MAX_HUFF_SYMBOLS_1 + 137];
};

struct state_t {
	struct decompressor_t m_decomp;
	uint32 m_dict_ofs;
	uint32 m_dict_avail;
	uint32 m_first_call;
	uint32 m_has_flushed;
	int m_window_bits;
	uint8 m_dict[_LZ_DICT_SIZE];
	enum status_t m_last_status;
};

enum status_t _decompress(struct decompressor_t *r, const uint8 *pIn_buf_next,
			  size_t *pIn_buf_size, uint8 *pOut_buf_start,
			  uint8 *pOut_buf_next, size_t *pOut_buf_size,
			  const uint32 decomp_flags);

extern void *memcpy(void *dst, const void *src, size_t len);
extern void *memset(void *dst, int n, size_t len);
extern void *malloc(size_t size);
extern void free(void *ptr);
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
/*****************************************************************************/

int inflate_init(struct zstream_t *pstream)
{
	struct state_t *pDecomp;

	if (!pstream)
		return ESTREAM;

	pstream->data_type = 0;
	pstream->adler = 0;
	pstream->msg = NULL;
	pstream->total_in = 0;
	pstream->total_out = 0;
	pstream->reserved = 0;
	if (!pstream->zalloc)
		pstream->zalloc = os_malloc;
	if (!pstream->zfree)
		pstream->zfree = os_free;

	pDecomp = (struct state_t*)pstream->zalloc(pstream->opaque, 1, sizeof(struct state_t));
	if (!pDecomp)
		return EMEM;

	pstream->state = pDecomp;

	pDecomp->m_decomp.m_state = 0;
	pDecomp->m_dict_ofs = 0;
	pDecomp->m_dict_avail = 0;
	pDecomp->m_last_status = STATUS_NEEDS_MORE_INPUT;
	pDecomp->m_first_call = 1;
	pDecomp->m_has_flushed = 0;
	pDecomp->m_window_bits = 15;

	return EOK;
}
/*****************************************************************************/

static int inflate(struct zstream_t * pstream)
{
	struct state_t *pState;
	uint32 n;
	uint32 first_call;
	uint32 decomp_flags = _FLAG_COMPUTE_ADLER32;
	size_t in_bytes;
	size_t out_bytes;
	size_t orig_avail_in;
	enum status_t status;

	if ((!pstream) || (!pstream->state))
		return ESTREAM;

	pState = (struct state_t*)pstream->state;

	if (pState->m_window_bits > 0)
		decomp_flags |= _FLAG_PARSE_ZLIB_HEADER;

	orig_avail_in = pstream->avail_in;

	first_call = pState->m_first_call;
	pState->m_first_call = 0;

	if (pState->m_last_status < 0)
		return EDATA;

	pState->m_has_flushed |= 1;

	if (first_call) {
		decomp_flags |= _FLAG_USING_NON_WRAPPING_OUTPUT_BUF;
		in_bytes = pstream->avail_in;
		out_bytes = pstream->avail_out;

		status = _decompress(&pState->m_decomp, pstream->next_in,
			&in_bytes, pstream->next_out,
			pstream->next_out, &out_bytes, decomp_flags);
		
		pState->m_last_status = status;
		pstream->next_in += (uint32)in_bytes;
		pstream->avail_in -= (uint32)in_bytes;
		pstream->total_in += (uint32)in_bytes;
		pstream->adler = pState->m_decomp.m_check_adler32;
		pstream->next_out += (uint32)out_bytes;
		pstream->avail_out -= (uint32)out_bytes;
		pstream->total_out += (uint32)out_bytes;

		if (status < 0)
			return EDATA;
		else if (status != STATUS_DONE) {
			pState->m_last_status = STATUS_FAILED;
			return EBUF;
		}
		return ESTREAM_END;
	}

	if (pState->m_dict_avail) {
		n = _MIN(pState->m_dict_avail, pstream->avail_out);
		os_memcpy(pstream->next_out, pState->m_dict + pState->m_dict_ofs, n);
		pstream->next_out += n;
		pstream->avail_out -= n;
		pstream->total_out += n;
		pState->m_dict_avail -= n;
		pState->m_dict_ofs = (pState->m_dict_ofs + n) & (_LZ_DICT_SIZE - 1);

		return ((pState->m_last_status == STATUS_DONE) && (!pState->m_dict_avail)) ? ESTREAM_END : EOK;
	}

	while (1) {
		in_bytes = pstream->avail_in;
		out_bytes = _LZ_DICT_SIZE - pState->m_dict_ofs;

		status = _decompress(&pState->m_decomp, pstream->next_in,
			&in_bytes, pState->m_dict,
			pState->m_dict + pState->m_dict_ofs, &out_bytes, decomp_flags);

		pState->m_last_status = status;

		pstream->next_in += (uint32)in_bytes;
		pstream->avail_in -= (uint32)in_bytes;
		pstream->total_in += (uint32)in_bytes;
		pstream->adler = pState->m_decomp.m_check_adler32;

		pState->m_dict_avail = (uint32)out_bytes;

		n = _MIN(pState->m_dict_avail, pstream->avail_out);

		os_memcpy(pstream->next_out, pState->m_dict + pState->m_dict_ofs, n);

		pstream->next_out += n;
		pstream->avail_out -= n;
		pstream->total_out += n;

		pState->m_dict_avail -= n;
		pState->m_dict_ofs = (pState->m_dict_ofs + n) & (_LZ_DICT_SIZE - 1);

		if (status < 0)
			return EDATA;
		else if ((status == STATUS_NEEDS_MORE_INPUT) && (!orig_avail_in))
			return EBUF;
		else {
			if (status == STATUS_DONE)
				return pState->m_dict_avail ? EBUF : ESTREAM_END;
			else if (!pstream->avail_out)
				return EBUF;
		}
	}

	return ((status == STATUS_DONE) && (!pState->m_dict_avail)) ? ESTREAM_END : EOK;
}
/*****************************************************************************/

static int inflate_end(struct zstream_t *pstream)
{
	if (!pstream)
		return ESTREAM;
	if (pstream->state) {
		pstream->zfree(pstream->opaque, pstream->state);
		pstream->state = NULL;
	}
	return EOK;
}
/*****************************************************************************/

int uncompress(unsigned char *uncompbuf, unsigned int *sz_uncompbuf,
	       const unsigned char *compbuf, unsigned int sz_compbuf)
{
	struct zstream_t stream;
	int status;

	os_memset(&stream, 0, sizeof(stream));

	if ((sz_compbuf | *sz_uncompbuf) > 0xFFFFFFFFU)
		return EPARAM;

	stream.next_in = compbuf;
	stream.avail_in = (uint32)sz_compbuf;
	stream.next_out = uncompbuf;
	stream.avail_out = (uint32)*sz_uncompbuf;

	status = inflate_init(&stream);
	if (status != EOK)
		return status;

	status = inflate(&stream);
	if (status != ESTREAM_END)
	{
		inflate_end(&stream);
		return ((status == EBUF) && (!stream.avail_in)) ? EDATA : status;
	}
	*sz_uncompbuf = (unsigned int)stream.total_out;

	return inflate_end(&stream);
}
/*****************************************************************************/


#define _CR_RETURN(state_index, result)  do { \
	status = result;\
	r->m_state = state_index; \
	goto common_exit; \
	case state_index:; \
} while (0)

#define _CR_RETURN_FOREVER(state_index, result) do { \
	for ( ; ; ) { \
		_CR_RETURN(state_index, result); \
	} \
} while (0)

#define _GET_BITS(state_index, b, n) do { \
	if (num_bits < (uint32)(n)) { \
		_NEED_BITS(state_index, n); \
	} \
	b = bit_buf & ((1 << (n)) - 1); \
	bit_buf >>= (n); \
	num_bits -= (n); \
} while (0)

#define _GET_BYTE(state_index, c) do { \
	if (pIn_buf_cur >= pIn_buf_end) { \
		while (1) { \
			if (decomp_flags & _FLAG_HAS_MORE_INPUT) { \
				_CR_RETURN(state_index, STATUS_NEEDS_MORE_INPUT); \
				if (pIn_buf_cur < pIn_buf_end) { \
					c = *pIn_buf_cur++; \
					break; \
				} \
			} else { \
				c = 0; \
				break; \
			} \
		} \
	} else \
		c = *pIn_buf_cur++; \
} while (0)

#define _SKIP_BITS(state_index, n) do { \
	if (num_bits < (uint32)(n)) { \
		_NEED_BITS(state_index, n); \
	} \
	bit_buf >>= (n); \
	num_bits -= (n); \
} while (0)

#define _NEED_BITS(state_index, n) do { \
	uint32 c; \
	_GET_BYTE(state_index, c);\
	bit_buf |= (((uint32)c) << num_bits);\
	num_bits += 8;\
} while (num_bits < (uint32)(n))

#define _HUFF_DECODE(state_index, sym, pHuff) do { \
	int temp; uint32 code_len, c; \
	if (num_bits < 15) { \
		if ((pIn_buf_end - pIn_buf_cur) < 2) { \
			_HUFF_BITBUF_FILL(state_index, pHuff); \
		} else { \
			bit_buf |= (((uint32)pIn_buf_cur[0]) << num_bits) | (((uint32)pIn_buf_cur[1]) << (num_bits + 8)); \
			pIn_buf_cur += 2; num_bits += 16; \
		} \
	} \
	if ((temp = (pHuff)->m_look_up[bit_buf & (_FAST_LOOKUP_SIZE - 1)]) >= 0) \
		code_len = temp >> 9, temp &= 511; \
	else { \
		code_len = _FAST_LOOKUP_BITS; \
		do { \
			temp = (pHuff)->m_tree[~temp + ((bit_buf >> code_len++) & 1)]; \
		} while (temp < 0); \
	} \
	sym = temp; \
	bit_buf >>= code_len; \
	num_bits -= code_len; \
} while (0)

#define _HUFF_BITBUF_FILL(state_index, pHuff) do { \
	temp = (pHuff)->m_look_up[bit_buf & (_FAST_LOOKUP_SIZE - 1)]; \
	if (temp >= 0) { \
		code_len = temp >> 9; \
		if ((code_len) && (num_bits >= code_len)) \
			break; \
	} else if (num_bits > _FAST_LOOKUP_BITS) { \
		code_len = _FAST_LOOKUP_BITS; \
		do { \
			temp = (pHuff)->m_tree[~temp + ((bit_buf >> code_len++) & 1)]; \
		} while ((temp < 0) && (num_bits >= (code_len + 1))); if (temp >= 0) break; \
	} \
	_GET_BYTE(state_index, c); \
	bit_buf |= (((uint32)c) << num_bits); \
	num_bits += 8; \
} while (num_bits < 15);

#define _READ_LE16(p) ((uint32)(((const uint8 *)(p))[0]) | ((uint32)(((const uint8 *)(p))[1]) << 8U))

enum status_t _decompress(struct decompressor_t *r, const uint8 *pIn_buf_next,
			  size_t *pIn_buf_size, uint8 *pOut_buf_start,
			  uint8 *pOut_buf_next, size_t *pOut_buf_size,
			  const uint32 decomp_flags)
{
	static const int s_length_base[31] = {
		3,4,5,6,7,8,9,10,11,13,
			15,17,19,23,27,31,35,43,51,59,
			67,83,99,115,131,163,195,227,258,0,0
	};
	static const int s_length_extra[31]= {
		0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0
	};
	static const int s_dist_base[32] = {
		1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
			257,385,513,769,1025,1537,2049,3073,4097,
			6145,8193,12289,16385,24577,0,0
	};
	static const int s_dist_extra[32] = {
		0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,
			8,8,9,9,10,10,11,11,12,12,13,13
	};
	static const uint8 s_length_dezigzag[19] = {
		16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
	};
	static const int s_min_table_sizes[3] = {
		257, 1, 4
	};

	enum status_t status = STATUS_FAILED;
	uint32 num_bits;
	uint32 dist;
	uint32 counter;
	uint32 num_extra;
	uint32 bit_buf;
	const uint8 *pIn_buf_cur = pIn_buf_next;
	const uint8 *pIn_buf_end = pIn_buf_next + *pIn_buf_size;
	uint8 *pOut_buf_cur = pOut_buf_next;
	const uint8 *pOut_buf_end = pOut_buf_next + *pOut_buf_size;
	size_t out_buf_size_mask = (decomp_flags & _FLAG_USING_NON_WRAPPING_OUTPUT_BUF) ? (size_t)-1 : ((pOut_buf_next - pOut_buf_start) + *pOut_buf_size) - 1, dist_from_out_buf_start;

	if (((out_buf_size_mask + 1) & out_buf_size_mask) ||
	    (pOut_buf_next < pOut_buf_start)) {
		*pIn_buf_size = *pOut_buf_size = 0;
		return STATUS_BAD_PARAM;
	}

	num_bits = r->m_num_bits;
	bit_buf = r->m_bit_buf;
	dist = r->m_dist;
	counter = r->m_counter;
	num_extra = r->m_num_extra;
	dist_from_out_buf_start = r->m_dist_from_out_buf_start;

	switch(r->m_state) {
	case 0:
		bit_buf = num_bits = dist = counter = num_extra = r->m_zhdr0 = r->m_zhdr1 = 0;
		r->m_z_adler32 = r->m_check_adler32 = 1;
		if (decomp_flags & _FLAG_PARSE_ZLIB_HEADER)  {
			_GET_BYTE(1, r->m_zhdr0); _GET_BYTE(2, r->m_zhdr1);
			counter = (((r->m_zhdr0 * 256 + r->m_zhdr1) % 31 != 0) || (r->m_zhdr1 & 32) || ((r->m_zhdr0 & 15) != 8));
			if (!(decomp_flags & _FLAG_USING_NON_WRAPPING_OUTPUT_BUF))
				counter |= (((1U << (8U + (r->m_zhdr0 >> 4))) > 32768U) || ((out_buf_size_mask + 1) < (size_t)(1U << (8U + (r->m_zhdr0 >> 4)))));
			if (counter) {
				_CR_RETURN_FOREVER(36, STATUS_FAILED);
			}
		}

		do {
			_GET_BITS(3, r->m_final, 3); r->m_type = r->m_final >> 1;
			if (r->m_type == 0) {
				_SKIP_BITS(5, num_bits & 7);
				for (counter = 0; counter < 4; ++counter) {
					if (num_bits)
						_GET_BITS(6, r->m_raw_header[counter], 8);
					else
						_GET_BYTE(7, r->m_raw_header[counter]);
				}
				if ((counter = (r->m_raw_header[0] | (r->m_raw_header[1] << 8))) != (uint32)(0xFFFF ^ (r->m_raw_header[2] | (r->m_raw_header[3] << 8)))) {
					_CR_RETURN_FOREVER(39, STATUS_FAILED);
				}
				while ((counter) && (num_bits)) {
					_GET_BITS(51, dist, 8);
					while (pOut_buf_cur >= pOut_buf_end) {
						_CR_RETURN(52, STATUS_HAS_MORE_OUTPUT);
					}
					*pOut_buf_cur++ = (uint8)dist;
					counter--;
				}
				while (counter) {
					size_t n;
					while (pOut_buf_cur >= pOut_buf_end) {
						_CR_RETURN(9, STATUS_HAS_MORE_OUTPUT);
					}
					while (pIn_buf_cur >= pIn_buf_end) {
						if (decomp_flags & _FLAG_HAS_MORE_INPUT) {
							_CR_RETURN(38, STATUS_NEEDS_MORE_INPUT);
						} else {
							_CR_RETURN_FOREVER(40, STATUS_FAILED);
						}
					}
					n = _MIN(_MIN((size_t)(pOut_buf_end - pOut_buf_cur), (size_t)(pIn_buf_end - pIn_buf_cur)), counter);
					os_memcpy(pOut_buf_cur, pIn_buf_cur, n); pIn_buf_cur += n; pOut_buf_cur += n; counter -= (uint32)n;
				}
			} else if (r->m_type == 3) {
				_CR_RETURN_FOREVER(10, STATUS_FAILED);
			} else {
				if (r->m_type == 1) {
					uint8 *p = r->m_tables[0].m_code_size; uint32 i;
					r->m_table_sizes[0] = 288;
					r->m_table_sizes[1] = 32;
					os_memset(r->m_tables[1].m_code_size, 5, 32);
					for ( i = 0; i <= 143; ++i)
						*p++ = 8;
					for ( ; i <= 255; ++i)
						*p++ = 9;
					for ( ; i <= 279; ++i)
						*p++ = 7;
					for ( ; i <= 287; ++i)
						*p++ = 8;
				} else 	{
					for (counter = 0; counter < 3; counter++) {
						_GET_BITS(11, r->m_table_sizes[counter], "\05\05\04"[counter]);
						r->m_table_sizes[counter] += s_min_table_sizes[counter];
					}
					OBJ_ZERO(r->m_tables[2].m_code_size);
					for (counter = 0; counter < r->m_table_sizes[2]; counter++) {
						uint32 s;
						_GET_BITS(14, s, 3);
						r->m_tables[2].m_code_size[s_length_dezigzag[counter]] = (uint8)s;
					}
					r->m_table_sizes[2] = 19;
				}
				for ( ; (int)r->m_type >= 0; r->m_type--) {
					int tree_next, tree_cur; struct huff_table_t *pTable;
					uint32 i, j, used_syms, total, sym_index, next_code[17], total_syms[16]; pTable = &r->m_tables[r->m_type]; OBJ_ZERO(total_syms); OBJ_ZERO(pTable->m_look_up); OBJ_ZERO(pTable->m_tree);
					
					for (i = 0; i < r->m_table_sizes[r->m_type]; ++i)
						total_syms[pTable->m_code_size[i]]++;

					used_syms = 0, total = 0; next_code[0] = next_code[1] = 0;

					for (i = 1; i <= 15; ++i) {
						used_syms += total_syms[i];
						next_code[i + 1] = (total = ((total + total_syms[i]) << 1));
					}

					if ((65536 != total) && (used_syms > 1)) {
						_CR_RETURN_FOREVER(35, STATUS_FAILED);
					}

					for (tree_next = -1, sym_index = 0; sym_index < r->m_table_sizes[r->m_type]; ++sym_index) {
						uint32 rev_code = 0, l, cur_code, code_size = pTable->m_code_size[sym_index]; if (!code_size) continue;
						cur_code = next_code[code_size]++;
						
						for (l = code_size; l > 0; l--, cur_code >>= 1)
							rev_code = (rev_code << 1) | (cur_code & 1);

						if (code_size <= _FAST_LOOKUP_BITS) {
							int16 k = (int16)((code_size << 9) | sym_index);
							while (rev_code < _FAST_LOOKUP_SIZE) {
								pTable->m_look_up[rev_code] = k;
								rev_code += (1 << code_size);
							}
							continue;
						}
						if (0 == (tree_cur = pTable->m_look_up[rev_code & (_FAST_LOOKUP_SIZE - 1)])) {
							pTable->m_look_up[rev_code & (_FAST_LOOKUP_SIZE - 1)] = (int16)tree_next;
							tree_cur = tree_next;
							tree_next -= 2;
						}
						rev_code >>= (_FAST_LOOKUP_BITS - 1);
						for (j = code_size; j > (_FAST_LOOKUP_BITS + 1); j--) {
							tree_cur -= ((rev_code >>= 1) & 1);
							if (!pTable->m_tree[-tree_cur - 1]) {
								pTable->m_tree[-tree_cur - 1] = (int16)tree_next;
								tree_cur = tree_next; tree_next -= 2;
							} else
								tree_cur = pTable->m_tree[-tree_cur - 1];
						}
						tree_cur -= ((rev_code >>= 1) & 1);
						pTable->m_tree[-tree_cur - 1] = (int16)sym_index;
					}
					if (r->m_type == 2) {
						for (counter = 0; counter < (r->m_table_sizes[0] + r->m_table_sizes[1]); ) {
							uint32 s;
							_HUFF_DECODE(16, dist, &r->m_tables[2]);
							if (dist < 16) {
								r->m_len_codes[counter++] = (uint8)dist;
								continue;
							}
							if ((dist == 16) && (!counter)) {
								_CR_RETURN_FOREVER(17, STATUS_FAILED);
							}
							num_extra = "\02\03\07"[dist - 16];
							_GET_BITS(18, s, num_extra);
							s += "\03\03\013"[dist - 16];
							os_memset(r->m_len_codes + counter,
								  (dist == 16) ? r->m_len_codes[counter - 1] : 0, s);
							counter += s;
						}
						if ((r->m_table_sizes[0] + r->m_table_sizes[1]) != counter) {
							_CR_RETURN_FOREVER(21, STATUS_FAILED);
						}
						os_memcpy(r->m_tables[0].m_code_size, r->m_len_codes,
							  r->m_table_sizes[0]);
						os_memcpy(r->m_tables[1].m_code_size, r->m_len_codes + r->m_table_sizes[0],
							  r->m_table_sizes[1]);
					}
				}
				for ( ; ; ) {
					uint8 *pSrc;
					for ( ; ; ) {
						if (((pIn_buf_end - pIn_buf_cur) < 4) || ((pOut_buf_end - pOut_buf_cur) < 2)) {
							_HUFF_DECODE(23, counter, &r->m_tables[0]);
							if (counter >= 256)
								break;
							while (pOut_buf_cur >= pOut_buf_end) {
								_CR_RETURN(24, STATUS_HAS_MORE_OUTPUT);
							}
							*pOut_buf_cur++ = (uint8)counter;
						} else {
							int sym2; uint32 code_len;

							if (num_bits < 15) {
								bit_buf |= (((uint32)_READ_LE16(pIn_buf_cur)) << num_bits);
								pIn_buf_cur += 2;
								num_bits += 16;
							}
							if ((sym2 = r->m_tables[0].m_look_up[bit_buf & (_FAST_LOOKUP_SIZE - 1)]) >= 0)
								code_len = sym2 >> 9;
							else {
								code_len = _FAST_LOOKUP_BITS;
								do {
									sym2 = r->m_tables[0].m_tree[~sym2 + ((bit_buf >> code_len++) & 1)];
								} while (sym2 < 0);
							}
							counter = sym2; bit_buf >>= code_len; num_bits -= code_len;
							if (counter & 256)
								break;

							if (num_bits < 15) {
								bit_buf |= (((uint32)_READ_LE16(pIn_buf_cur)) << num_bits);
								pIn_buf_cur += 2;
								num_bits += 16;
							}
							if ((sym2 = r->m_tables[0].m_look_up[bit_buf & (_FAST_LOOKUP_SIZE - 1)]) >= 0)
								code_len = sym2 >> 9;
							else {
								code_len = _FAST_LOOKUP_BITS;
								do {
									sym2 = r->m_tables[0].m_tree[~sym2 + ((bit_buf >> code_len++) & 1)];
								} while (sym2 < 0);
							}
							bit_buf >>= code_len; num_bits -= code_len;

							pOut_buf_cur[0] = (uint8)counter;
							if (sym2 & 256) {
								pOut_buf_cur++;
								counter = sym2;
								break;
							}
							pOut_buf_cur[1] = (uint8)sym2;
							pOut_buf_cur += 2;
						}
					}
					if ((counter &= 511) == 256)
						break;

					num_extra = s_length_extra[counter - 257];
					counter = s_length_base[counter - 257];

					if (num_extra) {
						uint32 extra_bits;
						_GET_BITS(25, extra_bits, num_extra);
						counter += extra_bits;
					}

					_HUFF_DECODE(26, dist, &r->m_tables[1]);
					num_extra = s_dist_extra[dist]; dist = s_dist_base[dist];
					if (num_extra) {
						uint32 extra_bits;
						_GET_BITS(27, extra_bits, num_extra);
						dist += extra_bits;
					}

					dist_from_out_buf_start = pOut_buf_cur - pOut_buf_start;
					if ((dist > dist_from_out_buf_start) && (decomp_flags & _FLAG_USING_NON_WRAPPING_OUTPUT_BUF)) {
						_CR_RETURN_FOREVER(37, STATUS_FAILED);
					}

					pSrc = pOut_buf_start + ((dist_from_out_buf_start - dist) & out_buf_size_mask);

					if ((_MAX(pOut_buf_cur, pSrc) + counter) > pOut_buf_end) {
						while (counter--) {
							while (pOut_buf_cur >= pOut_buf_end) {
								_CR_RETURN(53, STATUS_HAS_MORE_OUTPUT);
							}
							*pOut_buf_cur++ = pOut_buf_start[(dist_from_out_buf_start++ - dist) & out_buf_size_mask];
						}
						continue;
					}
					do {
						pOut_buf_cur[0] = pSrc[0];
						pOut_buf_cur[1] = pSrc[1];
						pOut_buf_cur[2] = pSrc[2];
						pOut_buf_cur += 3; pSrc += 3;
					} while ((int)(counter -= 3) > 2);
					if ((int)counter > 0) {
						pOut_buf_cur[0] = pSrc[0];
						if ((int)counter > 1)
							pOut_buf_cur[1] = pSrc[1];
						pOut_buf_cur += counter;
					}
				}
			}
		} while (!(r->m_final & 1));

		if (decomp_flags & _FLAG_PARSE_ZLIB_HEADER) {
			_SKIP_BITS(32, num_bits & 7);
			for (counter = 0; counter < 4; ++counter) {
				uint32 s;
				if (num_bits)
					_GET_BITS(41, s, 8);
				else _GET_BYTE(42, s);
				r->m_z_adler32 = (r->m_z_adler32 << 8) | s; 
			}
		}
		_CR_RETURN_FOREVER(34, STATUS_DONE);
	}

common_exit:
	r->m_num_bits = num_bits;
	r->m_bit_buf = bit_buf;
	r->m_dist = dist;
	r->m_counter = counter;
	r->m_num_extra = num_extra;
	r->m_dist_from_out_buf_start = dist_from_out_buf_start;

	*pIn_buf_size = pIn_buf_cur - pIn_buf_next;
	*pOut_buf_size = pOut_buf_cur - pOut_buf_next;

	if ((decomp_flags & (_FLAG_PARSE_ZLIB_HEADER | _FLAG_COMPUTE_ADLER32)) && (status >= 0)) {
		const uint8 *ptr = pOut_buf_next;
		size_t buf_len = *pOut_buf_size;

		uint32 i;
		uint32 s1 = r->m_check_adler32 & 0xffff;
		uint32 s2 = r->m_check_adler32 >> 16;
		size_t block_len = buf_len % 5552;

		while (buf_len) {
			for (i = 0; i + 7 < block_len; i += 8, ptr += 8) {
				s1 += ptr[0], s2 += s1; s1 += ptr[1], s2 += s1;
				s1 += ptr[2], s2 += s1; s1 += ptr[3], s2 += s1;
				s1 += ptr[4], s2 += s1; s1 += ptr[5], s2 += s1;
				s1 += ptr[6], s2 += s1; s1 += ptr[7], s2 += s1;
			}
			for ( ; i < block_len; ++i)
				s1 += *ptr++, s2 += s1;

			s1 %= 65521U;
			s2 %= 65521U;
			buf_len -= block_len;
			block_len = 5552;
		}
		r->m_check_adler32 = (s2 << 16) + s1;
		if ((status == STATUS_DONE) && (decomp_flags & _FLAG_PARSE_ZLIB_HEADER) && (r->m_check_adler32 != r->m_z_adler32))
			status = STATUS_ADLER32_MISMATCH;
	}
	return status;
}
