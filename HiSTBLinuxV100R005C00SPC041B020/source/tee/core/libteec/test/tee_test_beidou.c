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
#include <sys/mman.h>

#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_app_load.h"


#define TEST_TZ_BEIDOU 49


static const TEEC_UUID stb_test_uuid =
{
    0x0E0E0E0E, \
    0x0E0E, \
    0x0E0E, \
        { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E }
};

static TEEC_Context stb_test_context;
static TEEC_Session stb_test_session;

int test_tee_beidou()
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t origin;
    TEEC_Operation sess_op;

    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_3;
    }

	memset(&sess_op, 0, sizeof(TEEC_Operation));
	sess_op.started = 1;
	sess_op.paramTypes = TEEC_PARAM_TYPES(
						TEEC_NONE,
						TEEC_NONE,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_INPUT);
    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_IDENTIFY, NULL, &sess_op, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }
    
    result = TEEC_InvokeCommand(&session, TEST_TZ_BEIDOU, NULL, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }
    
cleanup_1:
    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_3:
    if (result)
    {
        TEEC_Error("test_tee_beidou case failed\n");
    }
    else
    {
        printf("test_tee_beidou case pass!!\n");
    }

    return result;
}


int main(int argc, char *argv[])
{
    test_tee_beidou();
    return 0;
}
