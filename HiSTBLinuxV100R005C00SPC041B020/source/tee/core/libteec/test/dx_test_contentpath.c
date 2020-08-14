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
#include "dx_client_api.h"

int test_global_with_operation_dx_content_path(Dx_Client_Tz_Session pSession)
{
	TEEC_Debug("%s start\n", __FUNCTION__);
	TEEC_Result ret;
	ret = DxTzClient_InitSecureContentPath(pSession);
	if (ret != TEEC_SUCCESS) {
		TEEC_Error("DxTzClient_InitSecureContentPath failed\n");
		return ret;
	}

	ret = DxTzClient_TerminateSecureContentPath(pSession);
	if (ret != TEEC_SUCCESS) {
		TEEC_Error("DxTzClient_TerminateSecureContentPath failed\n");
		return ret;
	}
	return ret;
}

int main(void)
{
	Dx_Client_Tz_Session_val dx_session;
	TEEC_Context context;
	TEEC_Session session;
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_ECHO;
	int err = 0;

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		err = 1;
		goto cleanup_1;
	}

	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &svc_uuid,
			 TEEC_LOGIN_PUBLIC,
			 NULL,
			 NULL,
			 NULL);
	if (result != TEEC_SUCCESS) {
		err = 1;
		goto cleanup_2;
	}
	dx_session.session = &session;
	dx_session.sharedMem = &sharedMem;
	dx_session.size = 1 * 0x8000;
	//dx_session.size = 0;
	//dx_session.size = 1;
	//dx_session.size = 65*1024;
	//dx_session.size = 129*1024;
	//dx_session.size = (4*1024-32)*1024;
	//dx_session.size = 4*1024*1024;
	if (test_global_with_operation_dx_content_path(&dx_session) != 0) {
		TEEC_Error("test_gloabl_with_operation_dx_content_path failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test_global_with_operation_dx_content_path success\n");
	}
cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (err)
		while (1);
	return 0;
}
