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
#include "tee_client_random_api.h"

static TEEC_Context g_context;
static TEEC_Session g_session;

static void TEEC_InitGVar()
{
    memset(&g_context, 0x00, sizeof(TEEC_Context));
    memset(&g_session, 0x00, sizeof(TEEC_Session));
}

TEEC_Result TEEC_InitCryptoService()
{
    TEEC_Result result;
    TEEC_UUID svc_id = TEE_SERVICE_CRYPTO;
    TEEC_Operation operation;

    TEEC_InitGVar();

    /* 1. Initialize Context */
    result = TEEC_InitializeContext(
        NULL,
        &g_context);

    if(result != TEEC_SUCCESS) {
        TEEC_Error("TEEC_InitializeContext failed\n");
        goto cleanup_1;
    }
    /* 2. Open Session */
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    result = TEEC_OpenSession(
        &g_context,
        &g_session,
        &svc_id,
        TEEC_LOGIN_IDENTIFY,
        NULL,
        &operation,
        NULL);

    if(result != TEEC_SUCCESS) {
        TEEC_Error("TEEC_OpenSession failed\n");
        goto cleanup_2;
    }
    return result;

cleanup_2:
    TEEC_FinalizeContext(&g_context);
    TEEC_InitGVar();
cleanup_1:
    return result;
}

TEEC_Result TEEC_UninitCryptoService()
{
    if ((g_context.fd == 0) || (g_session.session_id == 0))
        return (TEEC_Result)TEEC_ERROR_GENERIC;

    TEEC_CloseSession(&g_session);
    TEEC_FinalizeContext(&g_context);
    TEEC_InitGVar();

    return TEEC_SUCCESS;
}

TEEC_Result TEEC_Random(uint8_t *output, uint32_t outsize)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (output == NULL) {
        TEEC_Error("TEEC_Random failed, output is null\n");
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    }
    if (outsize == 0) {
        TEEC_Error("TEEC_Random failed, outsize is zero\n");
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    }

    /* 1. Check Init is ok or not */
    if ((g_context.fd == 0) || (g_session.session_id == 0)) {
        TEEC_Error("TEEC_Random failed, Please Init Crypto Service\n");
        return (TEEC_Result)TEEC_ERROR_BAD_STATE;
    }

    /* 2. Invoke Command */
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_OUTPUT);
    operation.params[3].tmpref.buffer = output;
    operation.params[3].tmpref.size = outsize;

    result = TEEC_InvokeCommand(
        &g_session,
        CRYPT_CMD_ID_CIPHER_RND,
        &operation,
        &origin);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("---->TEEC_Random Failed:0x%x, origin:0x%x\n", result, origin);
    } else {
        TEEC_Debug("---->TEEC_Random Success\n");
    }
    return result;
}
