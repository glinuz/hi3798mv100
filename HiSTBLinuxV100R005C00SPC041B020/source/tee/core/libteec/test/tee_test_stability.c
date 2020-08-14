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
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_storage_api.h"

#define TEEC_DEBUG 1
#define TEE_TEST_STABILITY 0xffe1
#define TEE_TEST_MULI_SESSION 25
int main(int argc, char **argv)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result = TEEC_SUCCESS;
	TEEC_Operation operation;
	TEEC_UUID svc_id = TEE_SERVICE_ECHO;
	char tmp_buf[256] = {0};
	char *tmp;
	int input_value = 0;
	int muli_session = 0;

	if ((1 == argc) || (argc > 2)) {
		printf("Please input the image path and name .\n");
	} else if (2 == argc) {
		printf("%s %s", argv[0], argv[1]);
	}
	tmp = argv[1];
	input_value = atoi(tmp);
	printf("input_value is %d\n", input_value);
	if (TEE_TEST_MULI_SESSION == input_value){
		muli_session = 1;
	}
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
			 &svc_id,
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		goto cleanup_2;
	}
	TEEC_Debug("session id 0x%x\n", session.session_id);

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].value.a = 0xFFFF;
	operation.params[0].value.b = input_value;
	operation.params[1].tmpref.buffer = tmp_buf;
	operation.params[1].tmpref.size = 256;

	result = TEEC_InvokeCommand(
			 &session,
			 TEE_TEST_STABILITY,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed\n");
		goto cleanup_3;
	} else {
		TEEC_Debug("[0] : %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
	}
	printf("tee_test_stability caces end\n");
cleanup_3:
	while(muli_session); /*for test*/

	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	return result;
}
