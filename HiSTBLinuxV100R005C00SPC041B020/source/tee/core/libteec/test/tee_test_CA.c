#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
//#define TEEC_DEBUG
#include "tee_client_api.h"
#include "tee_client_id.h"
#ifdef WITH_DEVCHIP_PLATFORM
#define SIGKILL 9
#endif
uint8_t *tee_output = "MEMREF_OUTPUT";
uint8_t *tee_inout = "the paramType is MEMREF_INOUT";
uint32_t tee_output_len;
uint32_t tee_inout_len;

uint32_t tee_output_a = 0xddd;
uint32_t tee_output_b = 0xccc;
uint32_t tee_inout_a = 0x555;
uint32_t tee_inout_b = 0x666;
TEEC_Context context;
TEEC_Session session;

int test_invoke_with_operation_value(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation = {0};
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

int do_tee_fs_test(TEEC_Session *session, bool is_global)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Operation operation = {0};
	TEEC_Result result;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = 0xFFE0;// cmd id do create_fs_isolation_test_file in echo task
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_VALUE_INOUT,
				       TEEC_VALUE_INOUT);
	operation.params[0].value.a = 0x111;// cmd id set:in TA do create_fs_isolation_test_file
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
pthread_t main_tid;
pthread_t main_tid2;
void *thread1_func(void *arg)
{
	TEEC_Result ret;
	printf("come into pthread func\n");
	while (1) {
		ret = do_tee_fs_test(&session, false);
		if (ret != TEEC_SUCCESS)
			break;
	}
	printf("failed to do thread_func ret is 0x%x\n", ret);
	pthread_kill(main_tid2, SIGKILL);
	return (void *)0;
}
void *thread2_func(void *arg)
{
	printf("come into pthread func\n");
	while (1) {
		if (test_invoke_with_operation_value(&session, false) != 0)
			break;
	}
	printf("failed to do thread_func\n");
	pthread_kill(main_tid, SIGKILL);
	return (void *)0;
}
int main(void)
{
	tee_output_len = strlen(tee_output);
	tee_inout_len = strlen(tee_inout);
	void *status;
	TEEC_Result result;
	TEEC_UUID svc_uuid[] = {TEE_SERVICE_GLOBAL, TEE_SERVICE_ECHO};
	TEEC_UUID global_uuid = TEE_SERVICE_GLOBAL;
	int err = 0;
	int i;
	uint32_t test_count;
	bool is_global;
	TEEC_Operation operation;
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
	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &(svc_uuid[1]),
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);

	if (result != TEEC_SUCCESS) {
		goto cleanup_2;
	}
	TEEC_Debug("session id 0x%x\n", session.session_id);
	err = pthread_create(&main_tid, NULL, thread1_func, NULL);
	if (err != 0) {
		printf("create thread error: %s/n", strerror(err));
		return 1;
	}
	err = pthread_create(&main_tid2, NULL, thread2_func, NULL);
	if (err != 0) {
		printf("create thread2 error: %s/n", strerror(err));
		return 1;
	}
	pthread_join(main_tid, &status);
	pthread_join(main_tid2, &status);
	printf("tee_test_CA failed\n");
cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (err)
		while (1);
	return 0;
}
