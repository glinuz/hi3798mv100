#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/err.h>
#include "rootagent_common.h"

/* size of [iv] is 16 and [key] must be 32 bytes.
 * [size] is the size of [output] and [input].
 * [size] must be mutiple of 32. */
int do_aes256_cbc(u8 *output, const u8 *input,
	const u8 *iv, const u8 *key, int size, int encrypt)
{
	struct scatterlist src;
	struct scatterlist dst;
	struct blkcipher_desc desc;
	struct crypto_blkcipher *cipher;
	int ret;

	if ((!output) || (!input) || (!iv) || (!key)) {
		antiroot_error("rootagent: do_aes256_cbc invalid params!\n");
		return -EINVAL;
	}
	if (size <= 0 || size % CHALLENGE_KEY_LENGTH > 0) {
		antiroot_error("rootagent: do_aes256_cbc size=%d\n", size);
		return -EINVAL;
	}
	cipher = crypto_alloc_blkcipher("cbc(aes)", 0, 0);

	if (IS_ERR(cipher)) {
		antiroot_error(
			"rootagent:crypto_alloc_blkcipher() failed. %p\n",
			cipher);
		return -EFAULT;
	}

	ret = crypto_blkcipher_setkey(cipher, key, CHALLENGE_KEY_LENGTH);
	if (ret) {
		antiroot_error(
			"rootagent:crypto_blkcipher_setkey failed. %d\n",
			ret);
		crypto_free_blkcipher(cipher);
		return -EFAULT;
	}

	crypto_blkcipher_set_iv(cipher, iv, IV_SIZE);
	sg_init_table(&dst, 1);
	sg_init_table(&src, 1);
	sg_set_buf(&dst, output, size);
	sg_set_buf(&src, input, size);
	desc.tfm   = cipher;
	desc.flags = 0;

	if (encrypt)
		ret = crypto_blkcipher_encrypt(&desc, &dst, &src, size);
	else
		ret = crypto_blkcipher_decrypt(&desc, &dst, &src, size);

	crypto_free_blkcipher(cipher);
	return ret;
}
