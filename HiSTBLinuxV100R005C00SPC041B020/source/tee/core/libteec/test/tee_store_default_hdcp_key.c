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
#include <cutils/log.h>
#include <cutils/properties.h>
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_storage_api.h"

#define TEEC_DEBUG 1
#define LOG_TAG "storedefaultkey"
#define SFS_CHECK_MAX_TIME (100)
int main(void)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result = TEEC_SUCCESS;
	TEEC_Operation operation;
	TEEC_UUID svc_id = TEE_SERVICE_HDCP;
	unsigned int check_times = 0;
	char sfs_property[PROPERTY_VALUE_MAX];

	do {
		if (property_get(SFS_PROPERTY, sfs_property, NULL)) {
			ALOGD("Get sfs property is %s\n", sfs_property);
			break;
		} else {
			ALOGD("Can not get sfs property:check_times=%d\n", check_times);
		}
		sleep(1);
		check_times++;
	} while (check_times <= SFS_CHECK_MAX_TIME);

	if (check_times > SFS_CHECK_MAX_TIME) {
		TEEC_Error("Cannot get sfs property!!!\n");
		return -1;
	}

	result = TEEC_InitializeContext(
			 NULL,
			 &context);
	if (result != TEEC_SUCCESS) {
		goto cleanup_1;
	}

	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &svc_id,
			 TEEC_LOGIN_PUBLIC,
			 NULL,
			 NULL,
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
	operation.params[0].value.b = 0xFFFB;

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
