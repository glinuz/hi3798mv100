#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#include <string.h>
#include <stdint.h>

#include <fcntl.h>

#define ECC_MAX_COORD_SIZE 66
#define ECC_COORD_SIZE     32
#define ECC_POINT_SIZE     65

#define EIMG_IV_SIZE (16)
#define EIMG_KEYIV_SIZE (64)

#define EIMG_RESERVED1_SIZE (6)     /* magic and version part size total 32 bytes */
#define EIMG_RESERVED2_SIZE (3)     /* kecc2_pubkey 4 bytes aligned */

#define EIMG_MAGIC_NUM (0x5A5AA5A5)
#define EIMG_FORMAT_VERSION (0x100)  /* used to update image encryption schem */
#define EIMG_AES_KEY_LEN (32)
#define VAL_LEN 129
#define BUF_LEN (VAL_LEN * 2 + 1)

#define STORE32H(x, y)                                                                     \
     { (y)[0] = (uint8_t)(((x)>>24)&255); (y)[1] = (uint8_t)(((x)>>16)&255);   \
       (y)[2] = (uint8_t)(((x)>>8)&255); (y)[3] = (uint8_t)((x)&255); }



typedef struct {
    uint32_t magic_num;
    uint32_t version;
    uint32_t reserved[EIMG_RESERVED1_SIZE];
    uint8_t kecc2_pubkey[ECC_POINT_SIZE + EIMG_RESERVED2_SIZE];
    uint8_t kecdhIV[EIMG_IV_SIZE];
    uint8_t enc_krndIV[EIMG_KEYIV_SIZE];
}enc_header_part1_t;

/*
 * use ECC 256 to ecdh share secret: share secret key len is 32bytes
 * plain_key: output
 *				secret key
 * plain_key_len: input
 *				plain_key buffer len, must be 32 bytes
 * kecc2_pubkey :output
 *				generate kecc2 keypair and output the kecc2_pub.buffer
 *				size must be 65 bytes(ECC256)
 * kecc2_pubkey_len: input
 *				must be 65 bytes
 * pub: input
 *		input kecc1 pub key buffer, buffer size must be 65 bytes(ECC256)
 * pub_len: input
 *			must be 65 bytes
 * single_hash_mode: input
 * */
static int32_t ecies_kem_encrypt(uint8_t *plain_key, uint32_t plain_key_len,
		uint8_t *kecc2_pubkey, uint32_t kecc2_pubkey_len, uint8_t *pub,
		uint32_t pub_len, uint32_t single_hash_mode)
{

	EC_KEY    *key = NULL, *ec_eph = NULL;
	EC_POINT *pub_key;
	EC_GROUP *group;
	uint8_t secret[3*ECC_MAX_COORD_SIZE];
	uint32_t secret_len;
	uint8_t peh[67];
	uint32_t size = ECC_COORD_SIZE;
	size_t peh_len;

	/* input and output params check */
	if (!plain_key || !kecc2_pubkey || !pub) {
		printf("Invalid input buffer\n");
		return -1;
	}
	if (ECC_POINT_SIZE != kecc2_pubkey_len) {
		printf("Invalid input param:kecc2_pubkey_len %d\n", kecc2_pubkey_len);
		return -1;
	}
	if (ECC_POINT_SIZE != pub_len) {
		printf("Invalid input param:kecc1_pubkey_len %d\n", pub_len);
		return -1;
	}
	if (EIMG_AES_KEY_LEN != plain_key_len) {
		printf("Invalid output param:kecc1_plain_key_len %d\n", plain_key_len);
		return -1;
	}

	/* pack KECC1 key:only have pub key */
	key = EC_KEY_new();
	if (NULL == key) {
		printf("ECC1 key alloc failed\n");
		return -1;
	}

	group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
	if (NULL == group) {
		printf("Failed to do EC_GROUP_new_by_curve_name\n");
		EC_KEY_free(key);
		return -1;
	}

	if (!EC_KEY_set_group(key, group)) {
		printf("Failed to set group for ecc1 key\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		return -1;
	}

	pub_key = EC_POINT_new(group);
	if (NULL == pub_key) {
		printf("Failed to alloc group for ecc1\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		return -1;
	}

	if(!EC_POINT_oct2point(group, pub_key, pub, pub_len, NULL))
	{
		printf("Failed to setup point for ecc1\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		return -1;
	}

	if (!EC_POINT_is_on_curve(group, pub_key, NULL))
	{
		printf("ECC1 is not in curve\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		return -1;
	}
	if(!EC_KEY_set_public_key(key, pub_key))
	{
		printf("Failed to set ecc1 pubkey\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		return -1;
	}

	if (!EC_KEY_check_key(key))
	{
		printf("EC_KEY_check_key failed:\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		return -1;
	}
	else
	{
		printf("Public key verified OK\n");
	}
	EC_KEY_free(key);

	/* Generate ECC2 key pair */
	ec_eph = EC_KEY_new();
	if (NULL == ec_eph) {
		printf("Failed to malloc for ecc2 keypair\n");
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		return -1;
	}
	if (!EC_KEY_set_group(ec_eph, group)) {
		printf("Failed to set group for ecc2 keypair\n");
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}
	if (!EC_KEY_generate_key(ec_eph)) {
		printf("Failed to generate key for ecc2 keypair\n");
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}

	if (!EC_KEY_check_key(ec_eph))
	{
		printf("EC_KEY_check_key failed:\n");
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}
	else
	{
		printf("Public key verified OK\n");
	}
	/*C0 = pub_eph, which is encrypted key*/
	if (!EC_POINT_point2oct(group, EC_KEY_get0_public_key(ec_eph),
				POINT_CONVERSION_UNCOMPRESSED,
				kecc2_pubkey, kecc2_pubkey_len, NULL)) {
		printf("Failed to get kecc2pubkey\n");
		EC_GROUP_free(group);
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}
	EC_GROUP_free(group);
	/*ECDH result is PEH */
	peh_len = ECDH_compute_key(peh, sizeof(peh), pub_key, ec_eph, NULL);
	if (!peh_len) {
		printf("Failed to do ecdh compute key\n");
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}

	if(single_hash_mode)
	{
		/*We use secret = PEH*/
		if (sizeof(secret) < size || sizeof(peh) < size) {
			printf("Memcpy failed, size(%d) not correct\n", size);
			EC_POINT_free(pub_key);
			EC_KEY_free(ec_eph);
			return -1;
		}
		memcpy(secret, peh, size);
		secret_len = size;
	}
	else
	{
		/*We use secret = C0|PEH */
		if (sizeof(secret) < ECC_POINT_SIZE ||
				sizeof(secret) < (ECC_POINT_SIZE + ECC_COORD_SIZE)) {
			printf("Failed to copy ecdh key to secret\n");
			EC_POINT_free(pub_key);
			EC_KEY_free(ec_eph);
			return -1;
		}
		memcpy(secret, kecc2_pubkey, ECC_POINT_SIZE);
		memcpy(secret + ECC_POINT_SIZE, peh, ECC_COORD_SIZE);
		secret_len = ECC_POINT_SIZE + ECC_COORD_SIZE;
	}

	if (0 != PKCS1_MGF1(plain_key, plain_key_len, secret, secret_len, EVP_sha256())) {
		printf("Failed to do PKCS1_MGF1\n");
		EC_POINT_free(pub_key);
		EC_KEY_free(ec_eph);
		return -1;
	}

	EC_POINT_free(pub_key);
	EC_KEY_free(ec_eph);
	return 0;
}

static int str2hex(char *dest, const char *src, uint32_t count)
{
	uint8_t hb, lb;
	uint32_t i;
	for (i = 0; i < count; i++) {
		hb = src[i];
		if (hb >= 0x30 && hb <= 0x39)
			hb -= 0x30;
		else if (hb >= 'a' && hb <='f')
			hb = (hb - 'a') + 0x0A;
		else
			return -1;
		i++;
		lb = src[i];
		if (lb >= 0x30 && lb <= 0x39)
			lb -= 0x30;
		else if (lb >= 'a' && lb <='f')
			lb = (lb - 'a') + 0x0A;
		else
			return -1;
		dest[i/2] = ((hb<<4) + lb);
	}
	return 0;
}
static int get_plain_text(char **input, uint32_t *input_len, char *file_name)
{
	uint8_t *buf;
	uint32_t size;
	int ret;

	FILE* fp = fopen(file_name, "rb");
	if (!fp) {
		printf("Cannot open %s\n", file_name);
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	if (!size) {
		printf("Error:Input file %s len is zero\n", file_name);
		fclose(fp);
		return -1;
	}
	buf = (uint8_t *)malloc(size);
	if (!buf) {
		printf("Error:Failed to malloc size(%d) mem\n", size);
		fclose(fp);
		return -1;
	}
	fseek(fp, 0L, SEEK_SET);

	ret= fread(buf, size, 1, fp);
	if (!ret) {
		fclose(fp);
		free(buf);
		printf("failed to read from %s\n", file_name);
		return -1;
	}

	fclose(fp);
	*input = buf;
	*input_len = size;
	return 0;
}

static int32_t aes_cbc_encrypt(uint8_t *out, uint8_t *in, uint32_t inlen, uint8_t *key, uint8_t *iv)
{
	EVP_CIPHER_CTX ctx;
	uint32_t len, len2;

	EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
	EVP_EncryptUpdate(&ctx, out, &len, in, inlen);
	EVP_EncryptFinal(&ctx, out + len, &len2);

	return len + len2;
}
/*
 * argv[1] input Krnd and IV for encrypt
 * argv[2] input fastboot ecc pubkey
 * argv[3] output encrypt image header file
 * wraptool krndIV.bin pub
 * */
int main(int argc, char* argv[])
{
	uint8_t buf[BUF_LEN] = { 0 };
	uint8_t *file_name;
	int32_t plain_key_len, kecc2_pubkey_len, single_hash_mode = 0;
	uint8_t kecc1_pubkey[ECC_POINT_SIZE];
	uint8_t kecdh_key[EIMG_AES_KEY_LEN];
	int len = 0;
	FILE * fp;
	uint8_t *krnd_iv;
	uint32_t krnd_iv_len;
	int fd;
	int i;
	uint32_t input_text_len;
	enc_header_part1_t *img_header;
	size_t write_len;

	if (4 != argc) {
		printf("Usage: %s krndIV_key_file ecies_pub_file header_file \n", argv[0]);
		return 1;
	}
	/* Get input Krnd and KrndIV */
	printf("Start get Krnd and IV.\n");
	file_name = argv[1];
	if (get_plain_text((char **)&krnd_iv, &krnd_iv_len, file_name)) {
		printf("Failed to get plain data from %s\n", file_name);
		return -1;
	}
	printf("krnd_iv_len is %d\n", krnd_iv_len);

	/* Get Board ECIES public key */
	printf("Start get public key.\n");
	file_name = argv[2];
	fp = fopen(file_name, "r");
	if (!fp)
	{
		printf("Cannot open %s\n", file_name);
		free(krnd_iv);
		return -1;
	}
	if (!fgets(buf, ECC_POINT_SIZE*2+1, fp) || ECC_POINT_SIZE*2 != strlen(buf))
	{
		printf("Cannot read kecc1_pubkey from %s\n", file_name);
		fclose(fp);
		free(krnd_iv);
		return -1;
	}
	if (str2hex(kecc1_pubkey, buf, ECC_POINT_SIZE*2)) {
		printf("Cannot do str2hex, invalid ecc pubkey\n");
		fclose(fp);
		free(krnd_iv);
		return -1;
    }
	fclose(fp);

	/* Generate img header part1 */
	img_header = (enc_header_part1_t *)malloc(sizeof(enc_header_part1_t));
	if (!img_header) {
		printf("Failed to malloc memory for img_header\n");
		free(krnd_iv);
		return -1;
	}
	memset((void *)img_header, 0, sizeof(enc_header_part1_t));
	img_header->magic_num = EIMG_MAGIC_NUM;
	img_header->version = EIMG_FORMAT_VERSION;

	/* Using share secret to encrypt Krnd and KrndIV */
	printf("Start to encrypt Krnd and IV.\n");
	if (krnd_iv_len%16 == 0) {
		input_text_len = krnd_iv_len + 1;
		krnd_iv_len += 16;
	}
	else {
		input_text_len = krnd_iv_len;
		krnd_iv_len = (input_text_len + (16 - (krnd_iv_len%16)));
	}
	OpenSSL_add_all_algorithms();

	/* Generate kecc2_pubkey and get share secret kecdh_key*/
	kecc2_pubkey_len = ECC_POINT_SIZE;
	if (0 != ecies_kem_encrypt(kecdh_key, sizeof(kecdh_key), img_header->kecc2_pubkey,
				kecc2_pubkey_len, kecc1_pubkey,
				sizeof(kecc1_pubkey), single_hash_mode)) {
		printf("Failed to generate kecdh_key\n");
		free(krnd_iv);
		free(img_header);
		return -1;
	}

	/* Generate kecdhIV */
	fd = open("/dev/random", O_RDONLY);
	if (fd > 0) {
		read(fd, img_header->kecdhIV, sizeof(img_header->kecdhIV));
	}
	else {
		printf("Failed to generate rand IV\n");
		free(krnd_iv);
		free(img_header);
		return -1;
	}
	for(i = 0; i < sizeof(img_header->kecdhIV); i++) {
		printf("rand img_header->kecdhIV[%d] = 0x%x\n", i, img_header->kecdhIV[i]);
	}

	/* Generate encKrndIV */
	printf("input_text_len is %d\n", input_text_len);
	if((len = aes_cbc_encrypt(img_header->enc_krndIV, krnd_iv, input_text_len,
					kecdh_key, img_header->kecdhIV)) != EIMG_KEYIV_SIZE)
	{
		printf("Failed to do aes_cbc_encrypt:len is %d, krnd_iv_len is %d\n",
				len, krnd_iv_len);
		free(krnd_iv);
		free(img_header);
		close(fd);
		return -1;
	}
	close(fd);

	/* Pack the Kecc2pub,KecdhIV, En_Krnd and En_KrndIV to trustedcore header */
	printf("Start to pack header.\n");
	fp = fopen(argv[3], "wb+");
	if (!fp) {
		printf("Failed to open %s\n", argv[3]);
		free(krnd_iv);
		free(img_header);
		return -1;
	}
	write_len = fwrite(img_header, sizeof(enc_header_part1_t), 1, fp);
	if (!write_len) {
		printf("Failed to write to file %s\n", argv[3]);
		free(krnd_iv);
		free(img_header);
		fclose(fp);
		return -1;
	}
	fclose(fp);
	free(img_header);
	free(krnd_iv);
	printf("succeed to generate enc trustedcore.img header\n");
	return 0;
}
