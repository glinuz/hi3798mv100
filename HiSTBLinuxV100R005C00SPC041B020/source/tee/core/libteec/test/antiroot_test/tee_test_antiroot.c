#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <cutils/process_name.h>
#include <getopt.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/cipher.h>

#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_test_antiroot.h"

#define TEEC_DEBUG

#define BIG_SIZE 1024
#define DS_BIG_SIZE 4096

#define TEE_TEST_ANTIROOT_VERSION     "V1.00 - AntiRoot_Test"
#define NAME_FOR_LOGIN		"test_antiroot-ca"

//TEE_AntiRoot  e7ed1f64-4687-41da-96dc-cbe4f27c838f
static const TEEC_UUID AntiRoot_uuid =
{
	0xE7ED1F64, 0x4687, 0x41DA,
	{ 0x96, 0xDC, 0xCB, 0xE4, 0xF2, 0x7C, 0x83, 0x8F }
};

static TEEC_Context gContext;
static int gInited;
static TEEC_Session gSession;
static uint8_t m_nounce[CHALLENGE_NOUNCE_LENGTH];

struct RAGENT_COMMAND whitelist_cmd;
struct RAGENT_COMMAND challenge_cmd;
struct RAGENT_COMMAND response_cmd;

char *tmp_rootproc =
		"/system/bin/mpdecision:/system/bin/thermal-engine:/sbin/ueventd:/sbin/healthd:/system/bin/lmkd:/system/bin/vold:/system/bin/epmd:/system/bin/netd:/system/bin/debuggerd:/system/bin/app_process";



/*#define TEST_AR_DEBUG*/
#ifdef TEST_AR_DEBUG
static void dump_hex(unsigned char* hex, uint32_t len, unsigned char* name)
{
	uint32_t i;
	if (NULL == hex) {
		printf("Bad expected parameter.\n");
		return;
	}
	printf("dump_hex name = [%s], len = %d.\n", name, len);

	for (i = 0; i < (len - len%16); i += 16) {
		printf(
			"     %02x %02x %02x %02x   %02x %02x %02x %02x   %02x %02x %02x %02x   %02x %02x %02x %02x  \n",
			hex[i + 0], hex[i + 1], hex[i + 2], hex[i + 3],
			hex[i + 4], hex[i + 5], hex[i + 6], hex[i + 7],
			hex[i + 8], hex[i + 9], hex[i + 10], hex[i + 11],
			hex[i + 12], hex[i + 13], hex[i + 14], hex[i + 15]);
	}

	for (i = (len - len%16); i < len; i++)
		printf("     hex[%d] = %02x\n", i, hex[i]);
}
#else
#define dump_hex(/*unsigned char* */ hex, /*uint32_t */ len, /*unsigned char* */ name)
#endif


void aes256_cbc_encrypt(unsigned char *output, unsigned char *input,
			unsigned char *iv, unsigned char *key, int size)
{
	EVP_CIPHER_CTX ctx;
	int outl, tmp, i;

	int rv;

	OpenSSL_add_all_algorithms();
	EVP_CIPHER_CTX_init(&ctx);

	rv = EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(),NULL, key, iv);
	if(rv != 1)
	{
		printf("aes256_cbc_encrypt init Error");
		return;
	}

	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	if(rv != 1)
	{
		printf("aes256_cbc_encrypt set padding Error");
		return;
	}

	outl = 0;
	rv = EVP_EncryptUpdate(&ctx, output, &outl, input, size);
	if(rv != 1)
	{
		printf("aes encrypt Error");
		return;
	}
	rv = EVP_EncryptFinal_ex(&ctx, output + outl, &tmp);
	outl = outl + tmp;
	/*printf("--aes256_cbc_encrypt is AES256 cipher_mode is CBC\n");
	dump_hex(output, outli, "aes256_cbc_encrypt");*/
}

void aes256_cbc_decrypt(unsigned char *output, unsigned char *input,
		           unsigned char *iv, unsigned char *key, int size){

	EVP_CIPHER_CTX ctx;
	int outl, tmp, i;

	int rv;
	OpenSSL_add_all_algorithms();
	EVP_CIPHER_CTX_init(&ctx);

	rv = EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(),NULL, key, iv);
	if(rv != 1)
	{
		printf("aes256_cbc_decrypt init Error");
		return;
	}

	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	if(rv != 1)
	{
		printf("aes256_cbc_decrypt set padding Error");
		return;
	}

	outl = 0;
	rv = EVP_DecryptUpdate(&ctx, output, &outl, input, size);
	if(rv != 1)
	{
		printf("aes decrypt Error");
		return;
	}
	rv = EVP_DecryptFinal_ex(&ctx, output + outl, &tmp);
	outl = outl + tmp;
	/*printf("----aes256_cbc_decrypt is AES256 cipher_mode is CBC\n");
	dump_hex(output, outli, "aes256_cbc_decrypt");*/
}

static void get_rand_bytes(char *output, uint32_t total)
{
	uint32_t len;

	for (len = 0; len < total; len++) {
		*(output + len) = rand()%256;
	}
}


static void wlist_cmdbuff_init(unsigned char* cmd_buff)
{
	struct RAGENT_CONFIG *config;
	struct RAGENT_CIPHER_KEY *white_key;
	struct RAGENT_WHITE_LIST *white_list;

	whitelist_cmd.magic = 0x53525748;
	whitelist_cmd.version = 1;
	whitelist_cmd.interface = 1;
	config = (struct RAGENT_CONFIG *)&(whitelist_cmd.content.config);

	white_list = (struct RAGENT_WHITE_LIST	*)&(config->white_list);
	white_key = (struct RAGENT_CIPHER_KEY *)&(config->cipher_key);

	white_list->dStatus = 0;

	get_rand_bytes((char*)white_list->kcodes, KERNEL_CODE_LENGTH_SHA);

	get_rand_bytes((char*)white_list->syscalls, SYSTEM_CALL_LENGTH_SHA);

	white_list->selinux = 0;

	get_rand_bytes((char*)white_list->sehooks, SELINUX_HOOKS_LENGTH_SHA);

	white_list->proclength = strlen(tmp_rootproc);

	get_rand_bytes((char*)white_key->cha_req_key, CHALLENGE_REQ_KEY_LENGTH);

	get_rand_bytes((char*)white_key->cha_key, CHALLENGE_KEY_LENGTH);

	memcpy(cmd_buff, &whitelist_cmd, sizeof(struct RAGENT_COMMAND));
	memcpy(cmd_buff + sizeof(struct RAGENT_COMMAND),
			tmp_rootproc, white_list->proclength);

	dump_hex((char*)white_list, sizeof(struct RAGENT_WHITE_LIST), "white_list");
}


static void chall_cmdbuf_init(unsigned char* input_buff)
{
	struct RAGENT_CHALLENGE *chg;

	challenge_cmd.magic = 0x53525748;
	challenge_cmd.version = 1;
	challenge_cmd.interface = 2;
	chg = (struct RAGENT_CHALLENGE *)&(challenge_cmd.content);
	chg->cpuload = rand()%100;
	chg->battery = rand()%100;
	chg->charging = rand()%100;
	chg->time = 1453680000 + 60*(rand()%14400);
	chg->timezone = 60*(rand()%24 - 12);
	memcpy(input_buff, &challenge_cmd, sizeof(struct RAGENT_COMMAND));
}
/*
static void resp_cmdbuf_init(unsigned char* input_buff)
{
	struct RAGENT_RESPONSE *rsp;
	struct RAGENT_WHITE_LIST *rspcon;

	response_cmd.magic = 0x53525748;
	response_cmd.version = 1;
	response_cmd.interface = 3;
	rsp = (struct RAGENT_RESPONSE *)&(response_cmd.content);
	rsp->proc_integrated = 1;
	rspcon = &(rsp->runtime_white_list);

	rspcon->dStatus = 0;

	get_rand_bytes((char*)rspcon->kcodes, KERNEL_CODE_LENGTH_SHA);

	get_rand_bytes((char*)rspcon->syscalls, SYSTEM_CALL_LENGTH_SHA);

	rspcon->selinux = 1;

	get_rand_bytes((char*)rspcon->sehooks, SELINUX_HOOKS_LENGTH_SHA);

	rspcon->proclength = strlen(tmp_rootproc);

	memcpy(input_buff, &response_cmd, sizeof(struct RAGENT_COMMAND));
}
*/
static TEEC_Result test_invokecmd_cmdbuff_fuzz(uint32_t cmdid)
{
	TEEC_Result result;
	uint32_t origin;
	TEEC_Operation operation;
	unsigned char *buff = NULL;

	printf(
		"----test_invokecmd_cmdbuff_fuzz start cmdid = %d ----\n",
		cmdid);

	buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == buff) {
		TEEC_Error("test_invokecmd_cmdbuff_fuzz malloc buffer failed.\n");
		return -1;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.started = 1;
	operation.params[0].tmpref.buffer = buff;
	operation.params[0].tmpref.size = ANTIROOT_DST_LEN;

	get_rand_bytes((char*)buff, ANTIROOT_DST_LEN);

	result = TEEC_InvokeCommand(&gSession, cmdid, &operation, &origin);

	TEEC_Error("invoke cmdid = %d, result = 0x%x.\n",
			cmdid, result);

	printf(
		"====test_invokecmd_cmdbuff_fuzz end cmdid = %d, result = 0x%x. ====\n\n\n",
		cmdid, result);

	free(buff);

	return result;
}

static void antiroot_set_whitelist(unsigned char* cmd_buff, uint32_t buff_size, uint32_t haskey)
{
	TEEC_Result result;
	uint32_t origin;
	TEEC_Operation operation;

	printf(
		"\n---- antiroot_set_whitelist start buff_size = %d ----\n",
		buff_size);
	TEEC_Debug("set WhiteList entry");
	wlist_cmdbuff_init(cmd_buff);

	if (0 == haskey) {
		memset(whitelist_cmd.content.config.cipher_key.cha_req_key,
				0x00, CHALLENGE_REQ_KEY_LENGTH);
		memset(whitelist_cmd.content.config.cipher_key.cha_key,
				0x00, CHALLENGE_KEY_LENGTH);
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.started = 1;
	operation.params[0].tmpref.buffer = cmd_buff;
	operation.params[0].tmpref.size = buff_size;
	/*
		sizeof(struct RAGENT_COMMAND)
		+ whitelist_cmd.content.config.white_list.proclength;*/

	dump_hex(cmd_buff, 40, "antiroot_set_whitelist cmd_buff");

	result = TEEC_InvokeCommand(&gSession,
			ROOTAGENT_CONFIG_ID, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error(
			"invoke failed, whitelist codes=0x%x, origin=0x%x\n",
			result, origin);
	} else {
		TEEC_Debug("set WhiteList success\n");
		printf("##set WhiteList success\n");
	}

	printf(
		"==== antiroot_set_whitelist end buff_size = %d, result = 0x%x. ====\n\n",
		buff_size, result);
}

static void antiroot_get_challenge(unsigned char* cmd_buff,
		unsigned char* input_buff, uint32_t buff_size)
{
	TEEC_Result result;
	uint32_t origin;
	int i =0;
	TEEC_Operation operation;
	unsigned char res[ANTIROOT_DST_LEN];
	struct RAGENT_CHALLENGE *tmp_challenge;
	unsigned char iv[IV_SIZE] =
		"\x56\x2e\x17\x99\x6d\x09\x3d\x28\xdd\xb3\xba\x69\x5a\x2e\x6f\x58";

	printf(
		"\n---- antiroot_get_challenge start buff_size = %d ----\n",
		buff_size);
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.started = 1;
	operation.params[0].tmpref.buffer = cmd_buff;
	operation.params[0].tmpref.size = buff_size;

	chall_cmdbuf_init(input_buff);

	memcpy(cmd_buff, iv, IV_SIZE);
	aes256_cbc_encrypt(cmd_buff + IV_SIZE, input_buff, iv,
			whitelist_cmd.content.config.cipher_key.cha_req_key,
			ANTIROOT_SRC_LEN);

	result = TEEC_InvokeCommand(&gSession,
			ROOTAGENT_CHALLENGE_ID, &operation, &origin);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error(
			"invoke failed,  command id = ROOTAGENT_CHALLENGE_ID, codes=0x%x, origin=0x%x\n",
			result, origin);
		return;
	}

	memset(input_buff, 0x00, ANTIROOT_DST_LEN);
	aes256_cbc_decrypt((unsigned char *)input_buff,
			(unsigned char*)cmd_buff + IV_SIZE,
			(unsigned char *)cmd_buff,
			whitelist_cmd.content.config.cipher_key.cha_key,
			ANTIROOT_SRC_LEN);

	tmp_challenge =
		(struct RAGENT_CHALLENGE *)&(((struct RAGENT_COMMAND *)input_buff)->content);

	memcpy(m_nounce, tmp_challenge->nounce, CHALLENGE_KEY_LENGTH);

	dump_hex(m_nounce, CHALLENGE_KEY_LENGTH, "m_nounce");

	printf(
		"==== antiroot_get_challenge end buff_size = %d, result = 0x%x. ====\n\n",
		buff_size, result);
}

static int teec_open_session()
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;

	char version_buf[256] = {0};
	unsigned int buf_len = 256;

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_NONE,
			TEEC_NONE,
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_MEMREF_TEMP_INPUT);
	result = TEEC_OpenSession(&gContext, &gSession, &AntiRoot_uuid,
			TEEC_LOGIN_IDENTIFY, NULL,  &operation, &origin);

	if(result != TEEC_SUCCESS)
	{
		printf("teec_init open session failed: result:%d orgi: %d\n",
				(int)result, origin);
	}
	else
	{
		TEEC_Error("teec_init open session OK\n");
	}

	return result;
}

static TEEC_Result teec_init()
{
	TEEC_Result result;
	uint32_t origin;
	TEEC_Operation operation;

	if (gInited)
	{
		printf("teec_init OK. had init before. return success\n");
		return TEEC_SUCCESS;
	}

	result = TEEC_InitializeContext(NULL, &gContext);
	if(result != TEEC_SUCCESS) {
		printf("teec initial failed, result:%d\n", result);
		return -1;
	}

	result = teec_open_session();
	if (result != TEEC_SUCCESS) {
		TEEC_Error("teec_init OpenSession failed\n");
		goto cleanup_1;
	}

	gInited = 1;

	printf("teec_init init OK\n");
	return TEEC_SUCCESS;

cleanup_1:
	TEEC_FinalizeContext(&gContext);
	return result;
}

static int teec_uninit()
{
	if (!gInited)
	{
		printf("gInited is not inited!\n");
		return -1;
	}

	TEEC_CloseSession(&gSession);

	TEEC_FinalizeContext(&gContext);

	gInited = 0;

	return 0;
}

static void test_invokecmd_SetWhiteList(uint32_t times)
{
	unsigned char* cmd_buff;
	uint32_t i;

	printf(
		"\n\n###########test_invokecmd_SetWhiteList start ###########\n\n");
	cmd_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == cmd_buff) {
		TEEC_Error("test_invokecmd_SetWhiteList malloc cmd_buff failed.\n");
		return;
	}
	printf("test_invokecmd_SetWhiteList  random buffersize.\n");
	for (i = 0; i < times; i++) {
		printf("test time = %d.\n", i );
		if (TEEC_SUCCESS != teec_init()) {
			printf("teec_init failed. leaving...\n");
			return;
		}
		antiroot_set_whitelist(cmd_buff, rand()%4096, 1);
		teec_uninit();
	}

	printf("test_invokecmd_SetWhiteList  random buffer.\n");
	for (i = 0; i < times; i++) {
		printf("test time = %d.\n", i );
		if (TEEC_SUCCESS != teec_init()) {
			printf("teec_init failed. leaving...\n");
			return;
		}
		test_invokecmd_cmdbuff_fuzz(ROOTAGENT_CONFIG_ID);
		teec_uninit();
	}

	printf(
		"###########test_invokecmd_SetWhiteList end ##########\n\n\n");
	free(cmd_buff);
}

static void test_invokecmd_Challenge(uint32_t times)
{
	unsigned char* cmd_buff;
	unsigned char* input_buff;
	uint32_t i;
	uint32_t bufsize = sizeof(struct RAGENT_COMMAND)
		+ whitelist_cmd.content.config.white_list.proclength;

	printf(
		"\n\n###########test_invokecmd_Challenge start ###########\n");
	cmd_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == cmd_buff) {
		TEEC_Error("test_invokecmd_Challenge malloc cmd_buff failed.\n");
		return;
	}
	input_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == input_buff) {
		TEEC_Error("test_invokecmd_Challenge malloc input_buff failed.\n");
		free(cmd_buff);
		return;
	}

	memset(cmd_buff, 0x00, ANTIROOT_DST_LEN);

	printf("test_invokecmd_Challenge  key is NULL test.\n");
	if (TEEC_SUCCESS != teec_init()) {
		printf("teec_init failed. leaving...\n");
		return;
	}
	antiroot_set_whitelist(cmd_buff, bufsize, 0);
	antiroot_get_challenge(cmd_buff, input_buff, ANTIROOT_DST_LEN);
	teec_uninit();


	printf("test_invokecmd_Challenge  random buffersize.\n");
	for (i = 0; i < times; i++) {
		printf("test time = %d.\n", i );
		if (TEEC_SUCCESS != teec_init()) {
			printf("teec_init failed. leaving...\n");
			return;
		}
		antiroot_set_whitelist(cmd_buff, bufsize, 1);
		antiroot_get_challenge(cmd_buff, input_buff, rand()%4096);
		teec_uninit();
	}

	printf("test_invokecmd_Challenge  random buffer.\n");
	for (i = 0; i < times; i++) {
		printf("test time = %d.\n", i );
		if (TEEC_SUCCESS != teec_init()) {
			printf("teec_init failed. leaving...\n");
			return;
		}
		antiroot_set_whitelist(cmd_buff, bufsize, 1);
		test_invokecmd_cmdbuff_fuzz(ROOTAGENT_CHALLENGE_ID);
		teec_uninit();
	}
	printf(
		"###########test_invokecmd_Challenge end ##########\n\n\n");

	free(cmd_buff);
	free(input_buff);
}

static void test_invokecmd_Response(uint32_t times)
{
	unsigned char* cmd_buff;
	unsigned char* input_buff;
	TEEC_Result result = TEEC_SUCCESS;
	uint32_t i;
	uint32_t bufsize = sizeof(struct RAGENT_COMMAND)
		+ whitelist_cmd.content.config.white_list.proclength;

	printf(
		"\n\n########### test_invokecmd_Response start ###########\n");
	cmd_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == cmd_buff) {
		TEEC_Error("test_invokecmd_Response malloc cmd_buff failed.\n");
		return;
	}

	input_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == input_buff) {
		TEEC_Error("test_invokecmd_Response malloc input_buff failed.\n");
		free(cmd_buff);
		return;
	}

	memset(cmd_buff, 0x00, ANTIROOT_DST_LEN);
	memset(input_buff, 0x00, ANTIROOT_DST_LEN);

	/*
	 * TODO:
	 *	Add rsp buffsize test.
	 */

	for (i = 0; i < times; i++) {
		printf("test time = %d.\n", i );
		if (TEEC_SUCCESS != teec_init()) {
			printf("teec_init failed. leaving...\n");
			return;
		}
		antiroot_set_whitelist(cmd_buff, bufsize, 1);
		antiroot_get_challenge(cmd_buff, input_buff, ANTIROOT_DST_LEN);
		result = test_invokecmd_cmdbuff_fuzz(ROOTAGENT_RESPONSE_ID);
		teec_uninit();
	}

	free(cmd_buff);
	free(input_buff);

	printf(
		"########### test_invokecmd_Response end ##########\n\n");
}

void random_cmdid_test(uint32_t times)
{
	uint32_t cmdid, i;
	unsigned char* cmd_buff;
	unsigned char* input_buff;
	uint32_t bufsize = sizeof(struct RAGENT_COMMAND)
		+ whitelist_cmd.content.config.white_list.proclength;

	printf(
		"\n\n########### random_cmdid_test start ###########\n");
	cmd_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == cmd_buff) {
		TEEC_Error("test_invokecmd_Response malloc cmd_buff failed.\n");
		return;
	}

	input_buff = (unsigned char*)malloc(ANTIROOT_DST_LEN);
	if (NULL == input_buff) {
		TEEC_Error("test_invokecmd_Response malloc input_buff failed.\n");
		free(cmd_buff);
		return;
	}

	memset(cmd_buff, 0x00, ANTIROOT_DST_LEN);
	memset(input_buff, 0x00, ANTIROOT_DST_LEN);

	i = times;
	printf("test time = %d.\n", i );
	/*
	 * TODO:
	 *	Add result verify.
	 *	use normal resp.
	 */
	if (TEEC_SUCCESS != teec_init()) {
		printf("teec_init failed. leaving...\n");
		return;
	}

	antiroot_get_challenge(cmd_buff, input_buff, ANTIROOT_DST_LEN);
	teec_uninit();

	if (TEEC_SUCCESS != teec_init()) {
		printf("teec_init failed. leaving...\n");
		return;
	}
	test_invokecmd_cmdbuff_fuzz(ROOTAGENT_RESPONSE_ID);
	teec_uninit();

	if (TEEC_SUCCESS != teec_init()) {
		printf("teec_init failed. leaving...\n");
		return;
	}
	antiroot_set_whitelist(cmd_buff, bufsize, 1);
	test_invokecmd_cmdbuff_fuzz(ROOTAGENT_RESPONSE_ID);
	teec_uninit();


	free(cmd_buff);
	free(input_buff);

	printf(
		"########### test_invokecmd_Response end ##########\n\n");
}

static void close_session_test(void)
{
	unsigned char *cmd_buff;
	uint32_t i;
	uint32_t bufsize = sizeof(struct RAGENT_COMMAND)
		+ whitelist_cmd.content.config.white_list.proclength;

	printf(
		"\n\n###########test_invokecmd_Challenge start ###########\n");
	cmd_buff = (unsigned char *)malloc(ANTIROOT_DST_LEN);
	if (NULL == cmd_buff) {
		TEEC_Error("test_invokecmd_Challenge malloc cmd_buff failed.\n");
		return;
	}

	memset(cmd_buff, 0x00, ANTIROOT_DST_LEN);

	printf("test_invokecmd_Challenge  key is NULL test.\n");
	if (TEEC_SUCCESS != teec_init()) {
		printf("teec_init failed. leaving...\n");
		return;
	}
	antiroot_set_whitelist(cmd_buff, bufsize, 1);
	teec_uninit();
	/*
	 * TODO: need to test under rootless device, or change TA to do this test.
	 *	 need to verify the return value.*/
}

static TEEC_Result test(void)
{
	TEEC_SharedMemory shared_mem;
	TEEC_Result result;
	uint32_t mem_input = (uint32_t)TEEC_MEM_INPUT;
	uint32_t mem_output = (uint32_t)TEEC_MEM_OUTPUT;
	TEEC_Operation operation;
	uint32_t origin;
	char *buff = "xxxxxxxxxxxyyyyyyyyyy";

	shared_mem.size = strlen(buff) + 1;
	shared_mem.flags = mem_input | mem_output;

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_INOUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].memref.parent = &shared_mem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = shared_mem.size;



	teec_init();
	result = TEEC_AllocateSharedMemory(&gContext, &shared_mem);
	if (result != TEEC_SUCCESS) {
		printf("test allocate failed, %x\n", result);
		return result;
	}
	result = TEEC_InvokeCommand(&gSession, 2, &operation, &origin);
	teec_uninit();

	return result;
}

void print_usage(void)
{
	printf("\ntee_test_antiroot  version 1.0\n");
	printf("usage: antiroot-ca [-n times]\n");
	printf("Options and arguments:\n");
	printf("On default test will repeat 10 times.\n");
	printf("-h	: print this usage.\n");
	printf("-n times: test will repeat the times that user input.\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	int cmdid;
	int ch, in;
	uint32_t fuzztimes = 10;

	printf("tee_test_antiroot is running, version is %s.\n",
			TEE_TEST_ANTIROOT_VERSION);

	while (-1 != (ch = getopt(argc, argv, "hn:"))) {
		printf("optind: %d,argc:%d,argv[%d]:%s\n", optind, argc, optind, argv[optind]);
		switch (ch) {
		case 'h':
			print_usage();
			break;
		case 'n':
			fuzztimes = atoi(optarg);
			printf("test fuzz times = %u.\n", fuzztimes);
			break;
		default:
			fuzztimes = 10;
			break;
		}
	}

	random_cmdid_test(1);
	close_session_test();
	/*
	 * invoke cmd buff fuzz test:
	 *	CA/TA should not crash.  TEE should not be affected at all.
	 */
	test_invokecmd_SetWhiteList(fuzztimes);
	test_invokecmd_Challenge(fuzztimes);
	test_invokecmd_Response(fuzztimes);

	return 0;
}
