#ifndef _ROOT_CRYPTOWRAP_H_
#define _ROOT_CRYPTOWRAP_H_

#define ENCRYPT 1
#define DECRYPT 0

int do_aes256_cbc(u8 *output, const u8 *input,
		const u8 *iv, const u8 *key, int size, int encrypt);
#endif
