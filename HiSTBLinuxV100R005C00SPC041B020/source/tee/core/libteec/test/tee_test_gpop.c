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

int main(void)
{
	TEEC_Context context;
	TEEC_Session session;

	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_ECHO;
	uint32_t buf123_len, buf4_len, origin;
	char *buf1, *buf2, *buf3, *buf4;
	uint32_t cmd;
	int ret = 0;


	/*
	 * this cmd do nothing and return TEEC_SUCCESS.
	 * #define TEE_TEST_CA_MUTI_THREAD 0xFFE0
	 */
	cmd = 0xFFE0;

	printf("tee_test_gpop 1.\n");

	buf123_len = 1000000;
	buf4_len =   1050000;	//MAX_SHARED_SIZE 0x100000 = 1048576
	buf1 = (char *)malloc(buf123_len);
	buf2 = (char *)malloc(buf123_len);
	buf3 = (char *)malloc(buf123_len);
	buf4 = (char *)malloc(buf4_len);

	if (!buf1 || !buf2 || !buf3 || !buf4) {
		printf("test gpop malloc failed.\n");
		ret = -1;
		goto cleanup_1;
	}
	printf("tee_test_gpop 2.\n");

	TEEC_Operation operation;
	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		ret = -10;
		goto cleanup_1;
	}
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);/*lint !e835 !e845*/
	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &svc_uuid,
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);

	if (result != TEEC_SUCCESS) {
		ret = -22;
		goto cleanup_2;
	}
	TEEC_Debug("session id 0x%x\n", session.session_id);
	printf("tee_test_gpop 3.\n");

	/*------invokecmd-------*/
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT);




	operation.params[0].tmpref.buffer = buf1;
	operation.params[0].tmpref.size = buf123_len;
	operation.params[1].tmpref.buffer = buf2;
	operation.params[1].tmpref.size = buf123_len;
	operation.params[2].tmpref.buffer = buf3;
	operation.params[2].tmpref.size = buf123_len;
	operation.params[3].tmpref.buffer = buf4;
	operation.params[3].tmpref.size = buf4_len;

	result = TEEC_InvokeCommand(
			&session,
			cmd,
			&operation,
			&origin);
	if (result != TEEC_ERROR_OUT_OF_MEMORY) {
		printf("gpop test, codes=0x%x, origin=0x%x\n", result, origin);
		ret = 0;
	} else {
		printf("gpop test , TEEC_ERROR_OUT_OF_MEMORY!\n");
		ret = -35;
		goto cleanup_3;
	}

	printf("tee_test_gpop 4.\n");

cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (!buf1)
		free(buf1);
	if (!buf2)
		free(buf2);
	if (!buf3)
		free(buf3);
	if (!buf4)
		free(buf4);

	printf("tee_test_gpop 5.\n");
	return ret;/*lint !e593*/
}
