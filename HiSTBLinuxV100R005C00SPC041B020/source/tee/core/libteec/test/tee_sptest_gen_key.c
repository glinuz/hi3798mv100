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

int main(void)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result = TEEC_SUCCESS;
	TEEC_Operation operation;
	TEEC_UUID svc_id = TEE_SERVICE_ECHO;

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
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].value.a = 0xFFFF;
	operation.params[0].value.b = 0xFFF9;

	result = TEEC_InvokeCommand(
			 &session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed\n");
		goto cleanup_3;
	} else {
		TEEC_Debug("[0] : %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
	}

cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	return result;
}
