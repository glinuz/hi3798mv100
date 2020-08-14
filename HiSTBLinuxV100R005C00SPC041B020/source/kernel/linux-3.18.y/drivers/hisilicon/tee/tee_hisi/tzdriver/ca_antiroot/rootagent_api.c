/*
 * FileName:      rootagent_api.c
 * Author:        z00321956
 * Description:   rootagent  API
 * Version:       0.0.1
 * Date:          2016-04-01
 */
#include <linux/mutex.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include "teek_client_api.h"
#include "teek_client_id.h"

#include "rootagent_mem.h"
#include "rootagent_api.h"
#include "rootagent_common.h"
#include "rootagent_crypto.h"

static TEEC_Context gContext;
static TEEC_Session gSession;
static int gInited; /*init flag*/
static int gCount; /*init count*/
static TEEC_UUID uuid = UUID_RM_TA; /*TA service ID*/
static TEEC_TempMemoryReference gMem;	/* for invokecmd params[0].tmpref.buffer */
static DEFINE_MUTEX(root_monitor_lock);

static TEEC_TempMemoryReference m_swap_mem;	/* for cbc crypto buffer: rm_command_in_out */
static uint8_t m_chal_req_key[CHALLENGE_REQ_KEY_LENGTH];
static uint8_t m_chal_key[CHALLENGE_KEY_LENGTH];
static uint8_t m_nounce[CHALLENGE_NOUNCE_LENGTH];

void root_monitor_finalize(void)
{
	 mutex_lock(&root_monitor_lock);
	 rm_mem_destroy();
	 TEEK_CloseSession(&gSession);
	 TEEK_FinalizeContext(&gContext);
	 gInited = 0;
	 mutex_unlock(&root_monitor_lock);
}

TEEC_Result root_monitor_tee_init(void)
{
	TEEC_Result result;
	uint32_t origin;
	TEEC_Operation operation = {0};
	u8 package_name[] = "antiroot-ca";
	u32 root_id = 0;

	mutex_lock(&root_monitor_lock);
	if (gInited) {
		antiroot_debug(ROOTAGENT_DEBUG_API,
				"RootAgent has already initialized");
		mutex_unlock(&root_monitor_lock);
		return TEEC_SUCCESS;
	}
	result = rm_mem_init();
	if (result) {
		antiroot_error("rm_mem_init failed\n");
		mutex_unlock(&root_monitor_lock);
		return TEEC_ERROR_GENERIC;
	}
	if (initial_rm_shared_mem(&gMem, &m_swap_mem)) {
		antiroot_error("Initial swap or share memory failed\n");
		mutex_unlock(&root_monitor_lock);
		return TEEC_ERROR_GENERIC;
	}
	result = TEEK_InitializeContext(NULL, &gContext);
	if (result != TEEC_SUCCESS) {
		antiroot_error("root_monitor_tee_init failed\n");
		goto cleanup_2;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_NONE,
			TEEC_NONE,
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_MEMREF_TEMP_INPUT);
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = strlen(package_name) + 1;
	result = TEEK_OpenSession(&gContext, &gSession,
		&uuid, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		antiroot_error("root agent open session failed\n");
		goto cleanup_1;
	}
	gInited = 1;
	gCount++;
	antiroot_debug(ROOTAGENT_DEBUG_API,
			"root_monitor_tee_init ok, initialized count: %d, gInited: %d\n",
			gCount, gInited);
	mutex_unlock(&root_monitor_lock);
	return TEEC_SUCCESS;

cleanup_1:
	TEEK_FinalizeContext(&gContext);
cleanup_2:
	rm_mem_destroy();
	mutex_unlock(&root_monitor_lock);
	return result;
}

TEEC_Result setting_config(struct RAGENT_CONFIG *config,
		struct RAGENT_ROOTPROC *root_proc)
{
	uint32_t origin;
	TEEC_Result result;
	TEEC_Operation operation;
	struct RAGENT_COMMAND *rm_command;
	int s_ret = 0;

	if (!gInited || !(gMem.buffer)) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC;
	}
	if (!config || !root_proc || !(root_proc->procid)) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	s_ret = memset_s(gMem.buffer, RM_PRE_ALLOCATE_SIZE,
			0x0, gMem.size);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(&operation, sizeof(TEEC_Operation),
			0, sizeof(TEEC_Operation));
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = gMem.buffer;
	operation.params[0].tmpref.size = gMem.size;
	rm_command = (struct RAGENT_COMMAND *)gMem.buffer;
	rm_command->magic = MAGIC;
	rm_command->version = VERSION;
	rm_command->interface = ROOTAGENT_CONFIG_ID;
	rm_command->content.config.white_list.dStatus =
		config->white_list.dStatus;
	rm_command->content.config.white_list.selinux =
		config->white_list.selinux;
	rm_command->content.config.white_list.proclength = root_proc->length;
	s_ret = memcpy_s(m_chal_req_key,
			CHALLENGE_REQ_KEY_LENGTH,
			config->cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rm_command->content.config.cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH,
			config->cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(m_chal_key, CHALLENGE_KEY_LENGTH,
			config->cipher_key.cha_key, CHALLENGE_KEY_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rm_command->content.config.cipher_key.cha_key,
			CHALLENGE_KEY_LENGTH,
			config->cipher_key.cha_key, CHALLENGE_KEY_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}

	s_ret = memcpy_s(rm_command->content.config.white_list.kcodes,
			KERNEL_CODE_LENGTH_SHA,
			config->white_list.kcodes,
			KERNEL_CODE_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rm_command->content.config.white_list.syscalls,
			SYSTEM_CALL_LENGTH_SHA,
			config->white_list.syscalls,
			SYSTEM_CALL_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rm_command->content.config.white_list.sehooks,
			SELINUX_HOOKS_LENGTH_SHA,
			config->white_list.sehooks,
			SELINUX_HOOKS_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}

	antiroot_debug(ROOTAGENT_DEBUG_API, "setting_config-------proclength:%d\n",
			rm_command->content.config.white_list.proclength);
	if ((root_proc->length > 0)
			&& (root_proc->length < ROOTAGENT_RPROCS_MAX_LENGTH)
			&& (NULL != root_proc->procid)) {
		s_ret = memcpy_s(gMem.buffer + sizeof(struct RAGENT_COMMAND),
				RM_PRE_ALLOCATE_SIZE - sizeof(struct RAGENT_COMMAND),
				root_proc->procid, root_proc->length);
		if (EOK != s_ret) {
			antiroot_error("setting_config _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return TEEC_ERROR_GENERIC;
		}
	} else {
		antiroot_error("root_proc is NULL!\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	result = TEEK_InvokeCommand(&gSession,
			ROOTAGENT_CONFIG_ID, &operation, &origin);
	/*
	 * if TA read rootstatus is already rooted, then TA
	 * will NOT setWhitelist, and return TEEC_SUCCESS directly,
	 * with value.a = TEE_ERROR_ANTIROOT_RSP_FAIL.
	 */
	if (result != TEEC_SUCCESS) {
		antiroot_error("Setting_config failed\n");
	} else if (REV_NOT_ROOT != operation.params[1].value.a) {
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"Setting_config failed due to R.\n");
		result = TEE_ERROR_ANTIROOT_RSP_FAIL;
	}

	return result;
}

TEEC_Result request_challenge(struct RAGENT_CHALLENGE *ichallenge)
{
	uint32_t origin;
	TEEC_Result result;
	TEEC_Operation operation;
	char *rm_command_out_in;
	char *rm_command_in_out;
	struct RAGENT_COMMAND *rm_command;
	int s_ret = 0;
	int ret;

	if (!gInited || !(gMem.buffer)) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC;
	}
	if (!ichallenge) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	s_ret = memset_s(gMem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, gMem.size);
	if (EOK != s_ret) {
		antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(m_swap_mem.buffer, RM_PRE_ALLOCATE_SIZE,
			0x0, m_swap_mem.size);
	if (EOK != s_ret) {
		antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}

	s_ret = memset_s(&operation, sizeof(TEEC_Operation),
			0, sizeof(TEEC_Operation));
	if (EOK != s_ret) {
		antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = gMem.buffer;
	operation.params[0].tmpref.size = gMem.size;

	get_random_bytes(gMem.buffer, IV_SIZE);
	rm_command_out_in = (char *)(gMem.buffer + IV_SIZE);
	rm_command_in_out = (char *)m_swap_mem.buffer;
	if (!rm_command_in_out) {
		antiroot_error("malloc failed!\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	rm_command = (struct RAGENT_COMMAND *)rm_command_in_out;
	rm_command->magic = MAGIC;
	rm_command->version = VERSION;
	rm_command->interface = ROOTAGENT_CHALLENGE_ID;
	rm_command->content.challenge.cpuload = ichallenge->cpuload;
	rm_command->content.challenge.battery = ichallenge->battery;
	rm_command->content.challenge.charging = ichallenge->charging;
	rm_command->content.challenge.time = ichallenge->time;
	rm_command->content.challenge.timezone = ichallenge->timezone;
	s_ret = memset_s(rm_command->content.challenge.nounce,
			CHALLENGE_NOUNCE_LENGTH,
			0, CHALLENGE_NOUNCE_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(m_nounce, CHALLENGE_NOUNCE_LENGTH,
			0, CHALLENGE_NOUNCE_LENGTH);
	if (EOK != s_ret) {
		antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
			__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	ret = do_aes256_cbc((u8 *)rm_command_out_in, (u8 *)rm_command_in_out,
		gMem.buffer, m_chal_req_key,
		ANTIROOT_SRC_LEN, ENCRYPT);
	if (ret) {
		antiroot_error("do_aes256_cbc failed, ret = %d.\n", ret);
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}
	result = TEEK_InvokeCommand(&gSession, ROOTAGENT_CHALLENGE_ID,
			&operation, &origin);
	if (result != TEEC_SUCCESS) {
		antiroot_error("Request_challenge failed!\n");
	} else if (REV_NOT_ROOT != operation.params[1].value.a) {
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"Request_challenge failed due to R!\n");
		result = TEE_ERROR_ANTIROOT_RSP_FAIL;
	} else {
		ret = do_aes256_cbc((u8 *)rm_command_in_out, (u8 *)rm_command_out_in,
			gMem.buffer, m_chal_key,
			ANTIROOT_SRC_LEN, DECRYPT);
		if (ret) {
			antiroot_error("do_aes256_cbc failed, ret = %d.\n", ret);
			return (TEEC_Result)TEEC_ERROR_GENERIC;
		}
		s_ret = memcpy_s(m_nounce, CHALLENGE_NOUNCE_LENGTH,
				rm_command->content.challenge.nounce,
				CHALLENGE_NOUNCE_LENGTH);
		if (EOK != s_ret) {
			antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return TEEC_ERROR_GENERIC;
		}
		s_ret = memcpy_s(ichallenge->challengeid,
				CHALLENGE_MAX_LENGTH*sizeof(uint32_t),
				rm_command->content.challenge.challengeid,
				CHALLENGE_MAX_LENGTH*sizeof(uint32_t));
		if (EOK != s_ret) {
			antiroot_error("request_challenge _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return TEEC_ERROR_GENERIC;
		}
		antiroot_debug(ROOTAGENT_DEBUG_API, "Request_challenge successful\n");
	}
	return result;
}
TEEC_Result feedback_response(struct RAGENT_RESPONSE *response,
		struct RAGENT_ROOTPROC *root_proc)
{
	uint32_t origin;
	int size;
	TEEC_Result result;
	TEEC_Operation operation;
	void *rm_command_out;
	void *rm_command_in;
	struct RAGENT_COMMAND *rm_command;
	struct RAGENT_RESPONSE *rsp;
	int s_ret = 0;
	int ret;

	if (!gInited || !(gMem.buffer)) {
		antiroot_error("Agent should be initialized first!\n");
		return TEEC_ERROR_GENERIC;
	}

	if (!response || !root_proc) {
		antiroot_error("Bad params!\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	s_ret = memset_s(gMem.buffer, RM_PRE_ALLOCATE_SIZE, 0x0, gMem.size);
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memset_s(m_swap_mem.buffer, RM_PRE_ALLOCATE_SIZE,
			0x0, m_swap_mem.size);
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}

	s_ret = memset_s(&operation, sizeof(TEEC_Operation),
			0, sizeof(TEEC_Operation));
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
			TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = gMem.buffer;
	operation.params[0].tmpref.size = gMem.size;
	get_random_bytes(gMem.buffer, IV_SIZE);
	rm_command_out = (void *)(gMem.buffer + IV_SIZE);
	rm_command_in = (void *)m_swap_mem.buffer;
	if (!rm_command_in) {
		antiroot_error("response kmalloc failed!\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	rm_command = (struct RAGENT_COMMAND *)rm_command_in;
	rsp = &(rm_command->content.response);
	rm_command->magic = MAGIC;
	rm_command->version = VERSION;
	rm_command->interface = ROOTAGENT_RESPONSE_ID;
	rsp->proc_integrated = response->proc_integrated;
	rsp->noop = response->noop;
	rsp->runtime_white_list.selinux = response->runtime_white_list.selinux;
	rsp->runtime_white_list.proclength = root_proc->length;
	s_ret = memcpy_s(rsp->runtime_white_list.kcodes,
			KERNEL_CODE_LENGTH_SHA,
			response->runtime_white_list.kcodes,
			KERNEL_CODE_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rsp->runtime_white_list.syscalls,
			SYSTEM_CALL_LENGTH_SHA,
			response->runtime_white_list.syscalls,
			SYSTEM_CALL_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	s_ret = memcpy_s(rsp->runtime_white_list.sehooks,
			SELINUX_HOOKS_LENGTH_SHA,
			response->runtime_white_list.sehooks,
			SELINUX_HOOKS_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return TEEC_ERROR_GENERIC;
	}
	if ((NULL != root_proc->procid)
			&& (root_proc->length > 0)
			&& (root_proc->length < ROOTAGENT_RPROCS_MAX_LENGTH)) {
		size = sizeof(struct RAGENT_COMMAND) + root_proc->length;
		if (size > (ANTIROOT_SRC_LEN)) {
			antiroot_error("response is oom!!\n");
			return TEEC_ERROR_OUT_OF_MEMORY;
		}
		s_ret = memcpy_s((char *)rm_command + sizeof(struct RAGENT_COMMAND),
				RM_PRE_ALLOCATE_SIZE - sizeof(struct RAGENT_COMMAND),
				root_proc->procid, root_proc->length);
		if (EOK != s_ret) {
			antiroot_error("feedback_response _s fail. line=%d, s_ret=%d.\n",
					__LINE__, s_ret);
			return TEEC_ERROR_GENERIC;
		}
	}
	ret = do_aes256_cbc(rm_command_out, rm_command_in, gMem.buffer,
		m_nounce, ANTIROOT_SRC_LEN, ENCRYPT);
	if (ret) {
		antiroot_error("do_aes256_cbc failed, ret = %d.\n", ret);
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}
	result = TEEK_InvokeCommand(&gSession,
		ROOTAGENT_RESPONSE_ID, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		antiroot_error("Feedback_response failed result = 0x%x\n",
				result);
	} else if (REV_NOT_ROOT != operation.params[1].value.a) {
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"feedback_response failed due to R!\n");
		result =  TEE_ERROR_ANTIROOT_RSP_FAIL;
	}
	return result;
}
