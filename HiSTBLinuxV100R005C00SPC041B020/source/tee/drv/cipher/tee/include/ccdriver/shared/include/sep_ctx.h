/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/


#ifndef _SEP_CTX_H_
#define _SEP_CTX_H_
#ifdef __KERNEL__
//#include <linux/types.h>
#define INT32_MAX 0x7FFFFFFFL
#else
//#include <stdint.h>
#endif
#define INT32_MAX 2147483647


#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

/* SeP context size */
#ifndef SEP_CTX_SIZE_LOG2
#if (SEP_SUPPORT_SHA > 256)
#define SEP_CTX_SIZE_LOG2 8
#else
#define SEP_CTX_SIZE_LOG2 7
#endif
#endif
#define SEP_CTX_SIZE (1<<SEP_CTX_SIZE_LOG2)
#define SEP_CTX_SIZE_WORDS (SEP_CTX_SIZE >> 2)

#define SEP_DES_IV_SIZE 8
#define SEP_DES_BLOCK_SIZE 8

#define SEP_DES_ONE_KEY_SIZE 8
#define SEP_DES_DOUBLE_KEY_SIZE 16
#define SEP_DES_TRIPLE_KEY_SIZE 24
#define SEP_DES_KEY_SIZE_MAX SEP_DES_TRIPLE_KEY_SIZE

#define SEP_AES_IV_SIZE 16
#define SEP_AES_IV_SIZE_WORDS (SEP_AES_IV_SIZE >> 2)

#define SEP_AES_BLOCK_SIZE 16
#define SEP_AES_BLOCK_SIZE_WORDS 4

#define SEP_AES_128_BIT_KEY_SIZE 16
#define SEP_AES_128_BIT_KEY_SIZE_WORDS	(SEP_AES_128_BIT_KEY_SIZE >> 2)
#define SEP_AES_192_BIT_KEY_SIZE 24
#define SEP_AES_192_BIT_KEY_SIZE_WORDS	(SEP_AES_192_BIT_KEY_SIZE >> 2)
#define SEP_AES_256_BIT_KEY_SIZE 32
#define SEP_AES_256_BIT_KEY_SIZE_WORDS	(SEP_AES_256_BIT_KEY_SIZE >> 2)
#define SEP_AES_KEY_SIZE_MAX			SEP_AES_256_BIT_KEY_SIZE
#define SEP_AES_KEY_SIZE_WORDS_MAX		(SEP_AES_KEY_SIZE_MAX >> 2)

#define SEP_MD5_DIGEST_SIZE 16
#define SEP_SHA1_DIGEST_SIZE 20
#define SEP_SHA224_DIGEST_SIZE 28
#define SEP_SHA256_DIGEST_SIZE 32
#define SEP_SHA256_DIGEST_SIZE_IN_WORDS 8
#define SEP_SHA384_DIGEST_SIZE 48
#define SEP_SHA512_DIGEST_SIZE 64

#define SEP_SHA1_BLOCK_SIZE 64
#define SEP_SHA1_BLOCK_SIZE_IN_WORDS 16
#define SEP_MD5_BLOCK_SIZE 64
#define SEP_MD5_BLOCK_SIZE_IN_WORDS 16
#define SEP_SHA224_BLOCK_SIZE 64
#define SEP_SHA256_BLOCK_SIZE 64
#define SEP_SHA256_BLOCK_SIZE_IN_WORDS 16
#define SEP_SHA1_224_256_BLOCK_SIZE 64
#define SEP_SHA384_BLOCK_SIZE 128
#define SEP_SHA512_BLOCK_SIZE 128

#if (SEP_SUPPORT_SHA > 256)
#define SEP_DIGEST_SIZE_MAX SEP_SHA512_DIGEST_SIZE
#define SEP_HASH_BLOCK_SIZE_MAX SEP_SHA512_BLOCK_SIZE /*1024b*/
#else /* Only up to SHA256 */
#define SEP_DIGEST_SIZE_MAX SEP_SHA256_DIGEST_SIZE
#define SEP_HASH_BLOCK_SIZE_MAX SEP_SHA256_BLOCK_SIZE /*512b*/
#endif

#define SEP_HMAC_BLOCK_SIZE_MAX SEP_HASH_BLOCK_SIZE_MAX

#define SEP_RC4_KEY_SIZE_MIN 1
#define SEP_RC4_KEY_SIZE_MAX 20
#define SEP_RC4_STATE_SIZE 264

#define SEP_C2_KEY_SIZE_MAX 16
#define SEP_C2_BLOCK_SIZE 8

#define SEP_MULTI2_SYSTEM_KEY_SIZE 		32
#define SEP_MULTI2_DATA_KEY_SIZE 		8
#define SEP_MULTI2_SYSTEM_N_DATA_KEY_SIZE 	(SEP_MULTI2_SYSTEM_KEY_SIZE + SEP_MULTI2_DATA_KEY_SIZE)
#define	SEP_MULTI2_BLOCK_SIZE					8
#define	SEP_MULTI2_IV_SIZE					8
#define	SEP_MULTI2_MIN_NUM_ROUNDS				8
#define	SEP_MULTI2_MAX_NUM_ROUNDS				128


#define SEP_ALG_MAX_BLOCK_SIZE SEP_HASH_BLOCK_SIZE_MAX

#define SEP_MAX_COMBINED_ENGINES 4

#define SEP_MAX_CTX_SIZE (max(sizeof(struct sep_ctx_rc4), \
				sizeof(struct sep_ctx_cache_entry)))
enum sep_engine_type {
	SEP_ENGINE_NULL = 0,
	SEP_ENGINE_AES = 1,
	SEP_ENGINE_DES = 2,
	SEP_ENGINE_HASH = 3,
	SEP_ENGINE_RC4 = 4,
	SEP_ENGINE_DOUT = 5,
	SEP_ENGINE_RESERVE32B = INT32_MAX,
};

enum sep_crypto_alg {
	SEP_CRYPTO_ALG_NULL = -1,
	SEP_CRYPTO_ALG_AES  = 0,
	SEP_CRYPTO_ALG_DES  = 1,
	SEP_CRYPTO_ALG_HASH = 2,
	SEP_CRYPTO_ALG_RC4  = 3,
	SEP_CRYPTO_ALG_C2   = 4,
	SEP_CRYPTO_ALG_HMAC = 5,
	SEP_CRYPTO_ALG_AEAD = 6,
	SEP_CRYPTO_ALG_BYPASS = 7,
	SEP_CRYPTO_ALG_COMBINED = 8,
	SEP_CRYPTO_ALG_NUM = 9,
	SEP_CRYPTO_ALG_RESERVE32B = INT32_MAX
};

enum sep_crypto_direction {
	SEP_CRYPTO_DIRECTION_NULL = -1,
	SEP_CRYPTO_DIRECTION_ENCRYPT = 0,
	SEP_CRYPTO_DIRECTION_DECRYPT = 1,
	SEP_CRYPTO_DIRECTION_DECRYPT_ENCRYPT = 3,
	SEP_CRYPTO_DIRECTION_RESERVE32B = INT32_MAX
};

enum sep_cipher_mode {
	SEP_CIPHER_NULL_MODE = -1,
	SEP_CIPHER_ECB = 0,
	SEP_CIPHER_CBC = 1,
	SEP_CIPHER_CTR = 2,
	SEP_CIPHER_CBC_MAC = 3,
	SEP_CIPHER_XTS = 4,
	SEP_CIPHER_XCBC_MAC = 5,
	SEP_CIPHER_OFB = 6,
	SEP_CIPHER_CMAC = 7,
	SEP_CIPHER_CCM = 8,
	SEP_CIPHER_CBC_CTS = 11,
	SEP_CIPHER_GCTR = 12,
	SEP_CIPHER_RESERVE32B = INT32_MAX
};

enum sep_hash_mode {
	SEP_HASH_NULL = -1,
	SEP_HASH_SHA1 = 0,
	SEP_HASH_SHA256 = 1,
	SEP_HASH_SHA224 = 2,
	SEP_HASH_SHA512 = 3,
	SEP_HASH_SHA384 = 4,
	SEP_HASH_MD5 = 5,
	SEP_HASH_CBC_MAC = 6, 
	SEP_HASH_XCBC_MAC = 7,
	SEP_HASH_CMAC = 8,
	SEP_HASH_MODE_NUM = 9,
	SEP_HASH_RESERVE32B = INT32_MAX
};

enum sep_hash_hw_mode {
	SEP_HASH_HW_MD5 = 0,
	SEP_HASH_HW_SHA1 = 1,
	SEP_HASH_HW_SHA256 = 2,
	SEP_HASH_HW_SHA224 = 10,
	SEP_HASH_HW_SHA512 = 4,
	SEP_HASH_HW_SHA384 = 12,
	SEP_HASH_HW_GHASH = 6,
	SEP_HASH_HW_RESERVE32B = INT32_MAX
};

enum sep_c2_mode {
	SEP_C2_NULL = -1,
	SEP_C2_ECB = 0,
	SEP_C2_CBC = 1,
	SEP_C2_RESERVE32B = INT32_MAX
};

enum sep_multi2_mode {
	SEP_MULTI2_NULL = -1,
	SEP_MULTI2_ECB = 0,
	SEP_MULTI2_CBC = 1,
	SEP_MULTI2_OFB = 2,
	SEP_MULTI2_RESERVE32B = INT32_MAX
};

enum sep_crypto_key_type {
	SEP_USER_KEY = 0,
	SEP_ROOT_KEY = 1,
	SEP_PROVISIONING_KEY = 2,
	SEP_SESSION_KEY = 3,
	SEP_APPLET_KEY = 4,
	SEP_END_OF_KEYS = INT32_MAX,
};

/*******************************************************************/
/***************** DESCRIPTOR BASED CONTEXTS ***********************/
/*******************************************************************/

 /* Generic context ("super-class") */
struct sep_ctx_generic {
	enum sep_crypto_alg alg;
} __attribute__((__may_alias__));

/* Cache context entry ("sub-class") */
struct sep_ctx_cache_entry {
	enum sep_crypto_alg alg;
	uint32_t reserved[SEP_CTX_SIZE_WORDS - 1];
};

struct sep_ctx_c2 {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_C2 */
	enum sep_c2_mode mode;
	enum sep_crypto_direction direction;
	/* reserve to end of allocated context size */
	uint32_t key_size; /* numeric value in bytes */
	uint8_t key[SEP_C2_KEY_SIZE_MAX];
	uint8_t reserved[SEP_CTX_SIZE - 4 * sizeof(uint32_t) -
			SEP_C2_KEY_SIZE_MAX];
};

struct sep_ctx_hash {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_HASH */
	enum sep_hash_mode mode;
	uint8_t digest[SEP_DIGEST_SIZE_MAX];
	/* reserve to end of allocated context size */
	uint8_t reserved[SEP_CTX_SIZE - 2 * sizeof(uint32_t) -
			SEP_DIGEST_SIZE_MAX];
};

/* !!!! sep_ctx_hmac should have the same structure as sep_ctx_hash except
   k0, k0_size fields */
struct sep_ctx_hmac {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_HMAC */
	enum sep_hash_mode mode;
	uint8_t digest[SEP_DIGEST_SIZE_MAX];
	uint32_t k0[SEP_HMAC_BLOCK_SIZE_MAX/sizeof(uint32_t)];
	uint32_t k0_size;
	/* reserve to end of allocated context size */
	uint8_t reserved[SEP_CTX_SIZE - 3 * sizeof(uint32_t) -
			SEP_DIGEST_SIZE_MAX - SEP_HMAC_BLOCK_SIZE_MAX];
};

struct sep_ctx_cipher {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_AES */
	enum sep_cipher_mode mode;
	enum sep_crypto_direction direction;
	enum sep_crypto_key_type crypto_key_type;
	uint32_t key_size; /* numeric value in bytes   */
	uint32_t data_unit_size; /* required for XTS */
	/* block_state is the AES engine block state.
	*  It is used by the host to pass IV or counter at initialization.
	*  It is used by SeP for intermediate block chaining state and for
	*  returning MAC algorithms results.           */
	uint8_t block_state[SEP_AES_BLOCK_SIZE];
	uint8_t key[SEP_AES_KEY_SIZE_MAX];
	uint8_t xex_key[SEP_AES_KEY_SIZE_MAX];
	/* reserve to end of allocated context size */
	uint32_t reserved[SEP_CTX_SIZE_WORDS - 6 -
		SEP_AES_BLOCK_SIZE/sizeof(uint32_t) - 2 *
		(SEP_AES_KEY_SIZE_MAX/sizeof(uint32_t))];
};

/* authentication and encryption with associated data class */
struct sep_ctx_aead {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_AES */
	enum sep_cipher_mode mode;
	enum sep_crypto_direction direction;
	uint32_t key_size; /* numeric value in bytes   */
	uint32_t nonce_size; /* nonce size (octets) */
	uint32_t header_size; /* finit additional data size (octets) */
	uint32_t text_size; /* finit text data size (octets) */
	uint32_t tag_size; /* mac size, element of {4, 6, 8, 10, 12, 14, 16} */
	/* block_state1/2 is the AES engine block state */
	uint8_t block_state[SEP_AES_BLOCK_SIZE];
	uint8_t mac_state[SEP_AES_BLOCK_SIZE]; /* MAC result */
	uint8_t nonce[SEP_AES_BLOCK_SIZE]; /* nonce buffer */
	uint8_t key[SEP_AES_KEY_SIZE_MAX];
	/* reserve to end of allocated context size */
	uint32_t reserved[SEP_CTX_SIZE_WORDS - 8 -
		3 * (SEP_AES_BLOCK_SIZE/sizeof(uint32_t)) -
		SEP_AES_KEY_SIZE_MAX/sizeof(uint32_t)];
};

/* crys combined context */
struct sep_ctx_combined {
	enum sep_crypto_alg alg;
	uint32_t mode;
	/* array of sub contexts used for the combined operation      *
	*  according to the given mode				      */
	struct sep_ctx_cache_entry *sub_ctx[SEP_MAX_COMBINED_ENGINES];
	/* store the host contexts addresses (optimization) */
	struct sep_ctx_cache_entry *hostAddr[SEP_MAX_COMBINED_ENGINES];
};

/*******************************************************************/
/***************** MESSAGE BASED CONTEXTS **************************/
/*******************************************************************/

struct sep_ctx_rc4 {
	enum sep_crypto_alg alg; /* SEP_CRYPTO_ALG_RC4 */
	uint32_t key_size; /* numeric value in bytes */
	uint8_t key[SEP_RC4_KEY_SIZE_MAX];
	uint8_t state[SEP_RC4_STATE_SIZE];
};

/* Get the address of a @member within a given @ctx address
   @ctx: The context address
   @type: Type of context structure
   @member: Associated context field */
#define GET_CTX_FIELD_ADDR(ctx, type, member) (ctx + offsetof(type, member))

#endif /* _SEP_CTX_H_ */

