#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_id.h"

uint8_t *tee_output = "MEMREF_OUTPUT";
uint8_t *tee_inout = "the paramType is MEMREF_INOUT";
uint32_t tee_output_len;
uint32_t tee_inout_len;

uint32_t tee_output_a = 0xddd;
uint32_t tee_output_b = 0xccc;
uint32_t tee_inout_a = 0x555;
uint32_t tee_inout_b = 0x666;

int test_invoke_without_operation(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 NULL,
			 &origin);

	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	}

	return ret;
}

int test_invoke_with_operation_none(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	}

	return ret;
}

int test_invoke_with_operation_value(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_VALUE_INOUT,
				       TEEC_VALUE_INOUT);
	operation.params[0].value.a = 0x111;
	operation.params[0].value.b = 0x222;
	operation.params[1].value.a = 0x333;
	operation.params[1].value.b = 0x444;
	operation.params[2].value.a = 0x555;
	operation.params[2].value.b = 0x666;
	operation.params[3].value.a = 0x777;
	operation.params[3].value.b = 0x888;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[1]: %x %x\n", operation.params[1].value.a, operation.params[1].value.b);
		TEEC_Debug("[2]: %x %x\n", operation.params[2].value.a, operation.params[2].value.b);
		TEEC_Debug("[3]: %x %x\n", operation.params[3].value.a, operation.params[3].value.b);
	}
	return ret;
}

int test_invoke_with_operation_value_none(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_NONE,
				       TEEC_VALUE_INOUT);
	operation.params[0].value.a = 0x111;
	operation.params[0].value.b = 0x222;
	operation.params[1].value.a = 0x333;
	operation.params[1].value.b = 0x444;
	operation.params[3].value.a = 0x777;
	operation.params[3].value.b = 0x888;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[1]: %x %x\n", operation.params[1].value.a, operation.params[1].value.b);
		TEEC_Debug("[3]: %x %x\n", operation.params[3].value.a, operation.params[3].value.b);
	}
	return ret;
}

int test_invoke_with_operation_temp(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	uint32_t len0, len1, len2, len3;
	char    testData0[256], testData1[256], testData2[256], testData3[256];

	memset(testData0, 0x00, 256);
	memset(testData1, 0x00, 256);
	memset(testData2, 0x00, 256);
	memset(testData3, 0x00, 256);
	strcpy(testData0, "Hello");
	len0 = strlen(testData0) + 1;
	strcpy(testData1, "abcdefgh");
	len1 = strlen(testData1) + 1;
	strcpy(testData2, "qwertyuiop");
	len2 = strlen(testData2) + 1;
	strcpy(testData3, "aaaabbbbccccd");
	len3 = strlen(testData3) + 1;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT);
	operation.params[0].tmpref.buffer = testData0;
	operation.params[0].tmpref.size = len0;
	operation.params[1].tmpref.buffer = testData1;
	operation.params[1].tmpref.size = tee_output_len; //len1;
	operation.params[2].tmpref.buffer = testData2;
	operation.params[2].tmpref.size = tee_inout_len; //len2;
	operation.params[3].tmpref.buffer = testData3;
	operation.params[3].tmpref.size = tee_inout_len; //len3;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
		TEEC_Debug("[2]: %s\n", (char *)operation.params[2].tmpref.buffer);
		TEEC_Debug("[3]: %s\n", (char *)operation.params[3].tmpref.buffer);
	}
	return ret;
}

int test_invoke_with_operation_temp_none(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	uint32_t len0, len1, len3;
	char    testData0[256], testData1[256], testData3[256];

	memset(testData0, 0x00, 256);
	memset(testData1, 0x00, 256);
	memset(testData3, 0x00, 256);
	strcpy(testData0, "Hello");
	len0 = strlen(testData0) + 1;
	strcpy(testData1, "abcdefgh");
	len1 = strlen(testData1) + 1;
	strcpy(testData3, "qwertyuiop");
	len3 = strlen(testData3) + 1;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INOUT);
	operation.params[0].tmpref.buffer = testData0;
	operation.params[0].tmpref.size = len0;
	operation.params[1].tmpref.buffer = testData1;
	operation.params[1].tmpref.size = tee_output_len; //len1;
	operation.params[3].tmpref.buffer = testData3;
	operation.params[3].tmpref.size = tee_inout_len; //len3;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
		TEEC_Debug("[3]: %s\n", (char *)operation.params[3].tmpref.buffer);
	}
	return ret;
}

int test_invoke_with_operation_value_temp(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	uint32_t len0, len1;
	char    testData0[256], testData1[256];

	memset(testData0, 0x00, 256);
	memset(testData1, 0x00, 256);
	strcpy(testData0, "Hello");
	len0 = strlen(testData0) + 1;
	strcpy(testData1, "abcdefgh");
	len1 = strlen(testData1) + 1;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_MEMREF_TEMP_OUTPUT);
	operation.params[0].value.a = 0xABC;
	operation.params[0].value.b = 0xDEF;
	operation.params[1].tmpref.buffer = testData0;
	operation.params[1].tmpref.size = len0;
	operation.params[2].value.a = 0x123;
	operation.params[2].value.b = 0x234;
	operation.params[3].tmpref.buffer = testData1;
	operation.params[3].tmpref.size = tee_output_len; //len1;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[2]: %x %x\n", operation.params[2].value.a, operation.params[2].value.b);
		TEEC_Debug("[3]: %s\n", (char *)operation.params[3].tmpref.buffer);
	}
	return ret;
}

int test_invoke_with_operation_value_temp_none(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;

	uint32_t len0;
	char    testData0[256];

	memset(testData0, 0x00, 256);
	strcpy(testData0, "Hello");
	len0 = strlen(testData0) + 1;

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INOUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].value.a = 0xaaa;
	operation.params[0].value.b = 0xbbb;
	operation.params[1].tmpref.buffer = testData0;
	operation.params[1].tmpref.size = tee_inout_len; //len0;

	result = TEEC_InvokeCommand(
			 session,
			 cmd,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		TEEC_Debug("[0]: %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
		TEEC_Debug("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
	}
	return ret;
}

int main(void)
{
	tee_output_len = strlen(tee_output);
	tee_inout_len = strlen(tee_inout);

	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result;
	TEEC_UUID svc_uuid[] = {TEE_SERVICE_ECHO};
	TEEC_UUID global_uuid = TEE_SERVICE_GLOBAL;
	TEEC_UUID ut_svc_id = TEE_SERVICE_UT;
	int err = 0;
	int i;
	uint32_t test_count;
	bool is_global;

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		goto cleanup_1;
	}
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);
	//do tee_test_ut, so ut can keep alive, this will make sure than in tee_test_invoke ta2ta will pass.
	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &ut_svc_id,
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		printf("TEEC_OpenSession failed\n");
		err = 1;
		goto cleanup_2;
	}
	TEEC_CloseSession(&session);

	test_count = sizeof(svc_uuid) / sizeof(TEEC_UUID);

	for (i = 0; i < test_count; i++) {
		result = TEEC_OpenSession(
				 &context,
				 &session,
				 &(svc_uuid[i]),
				 TEEC_LOGIN_IDENTIFY,
				 NULL,
				 &operation,
				 NULL);

		if (result != TEEC_SUCCESS) {
			goto cleanup_2;
		}
		TEEC_Debug("session id 0x%x\n", session.session_id);

		if (memcmp(&(svc_uuid[i]), &global_uuid, sizeof(global_uuid)))
			is_global = true;
		else
			is_global = false;

		if (test_invoke_without_operation(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_without_operation failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_without_operation success\n");
		}

		if (test_invoke_with_operation_none(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_none failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_none success\n");
		}

		if (test_invoke_with_operation_value(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_value failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_value success\n");
		}

		if (test_invoke_with_operation_value_none(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_value_none failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_value_none success\n");
		}

		if (test_invoke_with_operation_temp(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_temp failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_temp success\n");
		}

		if (test_invoke_with_operation_temp_none(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_temp_none failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_temp_none success\n");
		}

		if (test_invoke_with_operation_value_temp(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_value_temp failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_value_temp success\n");
		}

		if (test_invoke_with_operation_value_temp_none(&session, is_global) != 0) {
			TEEC_Error("test test_invoke_with_operation_value_temp_none failed\n");
			err = 1;
			goto cleanup_3;
		} else {
			TEEC_Debug("test test_invoke_with_operation_value_temp_none success\n");
		}

		if (i < (test_count - 1))
			TEEC_CloseSession(&session);
	}

	printf("ALL test case are PASS\n");

cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (err)
		while (1);
	return 0;
}
