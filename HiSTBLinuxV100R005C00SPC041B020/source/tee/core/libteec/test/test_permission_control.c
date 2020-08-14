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


 /* example uuid */
static const TEEC_UUID EXAMPLE_uuid =
{
    0xF1AE5991, 0xF36A, 0x84A4,
    {0xEE, 0x9F, 0x23, 0x4B, 0x37, 0xFB,0xBE, 0x69}
};

int main(void)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t cmd = 0;
    uint32_t origin;

    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed\n");
        goto cleanup_1;
    }
    memset(&operation, 0, sizeof(TEEC_Operation));
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
					    TEEC_NONE,
					    TEEC_NONE,
					    TEEC_MEMREF_TEMP_INPUT,
					    TEEC_MEMREF_TEMP_INPUT);
    result = TEEC_OpenSession(&context, &session, &EXAMPLE_uuid,
            TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("teec open session failed\n");
        goto cleanup_2;
    }

    result = TEEC_InvokeCommand(&session, cmd, NULL, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }else{
	printf("Succeed to load and excute dynamic TA!\n");
    }

    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return 0;
}
