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


#define TEE_TEST_HWI_IPC  0x66

int main(int argc, char *argv[])
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result;
	TEEC_UUID svc_id = TEE_SERVICE_ECHO;
	uint32_t origin;
	uint32_t hwi_num = 12;
	int ret = 0;

	TEEC_EXT_TEEInfo tee_info;
	uint32_t len = sizeof(tee_info);
	TEEC_EXT_GetTEEInfo(&tee_info, &len);
	//printf("tee_info.version=%d, %x, %x, %x\n", tee_info.version, tee_info.reserve1, tee_info.reserve2, tee_info.reserve3);
	if (argc > 1)
		hwi_num = atoi(argv[1]);
	if (hwi_num > 15) {
		printf("hwi not support fiq bigger than 15\n");
		while (1);
	}
	TEEC_Debug("------%s------\n", __FUNCTION__);

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		ret = -1;
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
			 &origin);

	if (result != TEEC_SUCCESS) {
		ret = -1;
		TEEC_Error("open session failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto cleanup_2;
	}

	TEEC_Debug("session id 0x%x\n", session.session_id);

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INOUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].value.a = hwi_num;
	operation.params[0].value.b = 0;

	result = TEEC_InvokeCommand(
			 &session,
			 TEE_TEST_HWI_IPC,
			 &operation,
			 &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = -1;
	} else {
		printf("successfully to trigger fiq %d and schedule TA to response!\n", hwi_num);
	}

	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	return ret;
}
