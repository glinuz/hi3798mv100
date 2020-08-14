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
#include "tee_client_storage_api.h"

static TEEC_Context g_context;
static TEEC_Session g_session;
static TEEC_Result g_errno;

static void TEEC_InitGVar()
{
	memset(&g_context, 0x00, sizeof(TEEC_Context));
	memset(&g_session, 0x00, sizeof(TEEC_Session));
	g_errno = TEEC_SUCCESS;
}

int32_t main()
{
	printf("------%s------\n", __FUNCTION__);
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result;
	TEEC_UUID svc_id = TEE_SERVICE_STORAGE;
	uint32_t origin;
	int ret = 0;
	int connectdata = 1234;
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

	result = TEEC_InitializeContext(
			 NULL,
			 &context);
	if (result != TEEC_SUCCESS) {
		ret = -1;
		goto cleanup_1;
	}

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT);
	//operation.params[0].tmpref.buffer = testData0;
	//operation.params[0].tmpref.size = len0;
	//operation.params[1].tmpref.buffer = testData1;
	//operation.params[1].tmpref.size = len1;
	//operation.params[3].tmpref.buffer = testData3;
	//operation.params[3].tmpref.size = len3;

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
		printf("open session failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto cleanup_2;
	}
	printf("session id 0x%x\n", session.session_id);
	printf("[1]: %s\n", (char *)operation.params[1].tmpref.buffer);
	printf("[3]: %s\n", (char *)operation.params[3].tmpref.buffer);

	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	return ret;
}
