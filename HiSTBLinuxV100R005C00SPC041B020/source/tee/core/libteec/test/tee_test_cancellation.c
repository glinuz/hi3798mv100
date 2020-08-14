#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <pthread.h>
#include "tee_client_api.h"
#include "tee_client_id.h"

void *cancel_func(TEEC_Operation *operation_test)
{
	printf("cancel funtion\n");
	sleep(3);
	printf("session id:%x,svc id : %x\n", operation_test->session->session_id, operation_test->session->service_id);
	//memset(&operation_test,0x00,sizeof(operation_test));
	operation_test->started = 1;
	operation_test->cancel_flag = 1;
	operation_test->paramTypes = TEEC_PARAM_TYPES(
					     TEEC_VALUE_INPUT,
					     TEEC_NONE,
					     TEEC_NONE,
					     TEEC_NONE);
	operation_test->params[0].value.a = 0xFFFF;
	operation_test->params[0].value.b = 0xFFFD;
	printf("before call cancel\n");
	TEEC_RequestCancellation(operation_test);
	printf("call cancle end\n");
	while (1);

}

int main(void)
{

	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_UUID svc_uuid[] = {TEE_SERVICE_GLOBAL, TEE_SERVICE_ECHO};
	TEEC_UUID global_uuid = TEE_SERVICE_GLOBAL;
	int err = 0;
	int i = 1;
	uint32_t test_count;
	bool is_global;
	TEEC_Operation operation , operation_test;
	uint32_t origin;
	uint32_t cmd;
	int ret;
	operation_test.session = &session;
	pthread_t ptid;

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		goto cleanup_1;
	}

	test_count = sizeof(svc_uuid) / sizeof(TEEC_UUID);
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
			 &(svc_uuid[i]),
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);

	if (result != TEEC_SUCCESS) {
		goto cleanup_2;
	}
	TEEC_Debug("session id 0x%x\n", session.session_id);
	printf("session id:%x,svc id : %x\n", operation_test.session->session_id, operation_test.session->service_id);
	err = pthread_create(&ptid, NULL, cancel_func, &operation_test); //创建线程
#if 0
	fpid = fork();
	if (fpid < 0)
		printf("fork error\n");
	if (fpid == 0) {
		sleep(3);
		printf("i am child \n");
		//while(1);
		printf("session id:%x,svc id : %x\n", operation_test.session->session_id, operation_test.session->service_id);
#if 0
		memset(&operation_test, 0x00, sizeof(operation_test));
		operation_test.started = 1;
		operation_test.paramTypes = TEEC_PARAM_TYPES(
						    TEEC_NONE,
						    TEEC_NONE,
						    TEEC_NONE,
						    TEEC_NONE);
		printf("before call cancel\n");
		TEEC_RequestCancellation(&operation_test);
		printf("call cancle end\n");
		while (1);
#endif
		printf("before invoke cmd\n");
		if (memcmp(&(svc_uuid[i]), &global_uuid, sizeof(global_uuid)))
			is_global = true;
		else
			is_global = false;
		printf("before invoke cmd\n");
		cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;
		memset(&operation, 0x00, sizeof(operation));
		operation.started = 1;
		operation.paramTypes = TEEC_PARAM_TYPES(
					       TEEC_VALUE_INPUT,
					       TEEC_NONE,
					       TEEC_NONE,
					       TEEC_NONE);

		operation.params[0].value.a = 0xFFFF;
		operation.params[0].value.b = 0xFFFD;
		printf("before invoke cmd\n");
		result = TEEC_InvokeCommand(
				 &session,
				 ECHO_CMD_ID_SEND_CMD,
				 &operation,
				 &origin);
		printf("end of invoke command\n");
		if (result != TEEC_SUCCESS) {
			TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			ret = -1;
			goto cleanup_3;
		} else {
			TEEC_Debug("[0]: %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
			TEEC_Debug("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
		}

	} else {
#endif

		if (memcmp(&(svc_uuid[i]), &global_uuid, sizeof(global_uuid)))
			is_global = true;
		else
			is_global = false;
		cmd = is_global ? GLOBAL_CMD_ID_BOOT_ACK : ECHO_CMD_ID_SEND_CMD;
		memset(&operation, 0x00, sizeof(operation));
		operation.started = 1;
		operation.paramTypes = TEEC_PARAM_TYPES(
					       TEEC_VALUE_INPUT,
					       TEEC_NONE,
					       TEEC_NONE,
					       TEEC_NONE);
		operation.params[0].value.a = 0xFFFF;
		operation.params[0].value.b = 0xFFFD;
		result = TEEC_InvokeCommand(
				 &session,
				 ECHO_CMD_ID_SEND_CMD,
				 &operation,
				 &origin);

		if (result != TEEC_SUCCESS) {
			TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			ret = -1;
			goto cleanup_3;
		} else {
			TEEC_Debug("[0]: %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
			TEEC_Debug("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
		}


		//}

		printf("hdcp key write test pass\n");
cleanup_3:
		TEEC_CloseSession(&session);
cleanup_2:
		TEEC_FinalizeContext(&context);
cleanup_1:
		if (err)
			while (1);
		return 0;
	}
