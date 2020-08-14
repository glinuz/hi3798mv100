#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "tee_client_api.h"
#include "tee_client_id.h"

TEEC_Result TEEC_GetTEETime(struct timeval *tv)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_UUID uuid = TEE_SERVICE_GLOBAL;
    TEEC_Operation operation = {0};

    result = TEEC_InitializeContext(
               NULL,
               &context);

    if(result != TEEC_SUCCESS) {
        return result;
    }

    result = TEEC_OpenSession(
                &context,
                &session,
                &uuid,
                TEEC_LOGIN_PUBLIC,
                NULL,
                NULL,
                NULL);

    if(result != TEEC_SUCCESS) {
        goto cleanup_1;
    }

    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_OUTPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);

    result = TEEC_InvokeCommand(
                 &session,
                 GLOBAL_CMD_ID_TEE_TIME,
                 &operation,
                 NULL);

    if(result != TEEC_SUCCESS){
        goto cleanup_2;
    }

    if(tv){
        tv->tv_sec = operation.params[0].value.a;
        tv->tv_usec = operation.params[0].value.b;
    }

cleanup_2:
    TEEC_CloseSession(&session);
cleanup_1:
    TEEC_FinalizeContext(&context);

    return result;
}
