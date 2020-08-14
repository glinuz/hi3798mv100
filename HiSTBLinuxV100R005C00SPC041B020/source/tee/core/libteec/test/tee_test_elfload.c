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

#define file_open open
#define file_close close
#define file_seek lseek
#define file_read read
#define FILE_SEEK_SET            0
#define FILE_SEEK_CUR            1
#define FILE_SEEK_END            2

#define TEE_SERVICE_GLOBAL \
{ \
        0x00000000, \
        0x0000, \
        0x0000, \
        { \
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
        } \
}

/**
 *  * @ingroup  TEE_ID
 *   *
 *    * 安全服务Echo
 *     */
#define TEE_SERVICE_ECHO \
{ \
        0x01010101, \
        0x0101, \
        0x0101, \
        { \
                    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 \
        } \
}

/**
 *  * @ingroup  TEE_ID
 *   *
 *    * 安全存储服务
 *     */
#define TEE_SERVICE_STORAGE \
{ \
        0x02020202, \
        0x0202, \
        0x0202, \
        { \
                    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 \
        } \
}

/**
 *  * @ingroup  TEE_ID
 *   *
 *    * 安全服务UT测试
 *     */
#define TEE_SERVICE_UT \
{ \
        0x03030303, \
        0x0303, \
        0x0303, \
        { \
                    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 \
        } \
}

/**
 *  * @ingroup  TEE_ID
 *   *
 *    * 加解密服务
 *     */
#define TEE_SERVICE_CRYPT \
{ \
        0x04040404, \
        0x0404, \
        0x0404, \
        { \
                    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 \
        } \
}



 /* file exits */
static const TEEC_UUID UUID01 =
{
    0x79b77788, 0x9789, 0x4a7a,
    { 0xa2, 0xbe, 0xb6, 0x1, 0x55, 0xee, 0xf5, 0xf3 }
};

/* file exits, but file read will erro */
static const TEEC_UUID UUID02 =
{
    0x55555555, 0x9789, 0x4a7a,
    { 0xa2, 0xbe, 0xb6, 0x1, 0x55, 0xee, 0xf5, 0xf3 }
};

/* file not exits */
static const TEEC_UUID UUID03 =
{
    0x0, 0x0, 0x4aaa,
    { 0xa2, 0xbe, 0x0, 0x1, 0x55, 0x0, 0xf5, 0xf3 }
};

/* file exits, but elf hash verify erro */
static const TEEC_UUID UUID09 =
{
    0x44444444, 0x9789, 0x4a7a,
    { 0xa2, 0xbe, 0xb6, 0x1, 0x55, 0xee, 0xf5, 0xf3 }
};

static const TEEC_UUID UUID04 = TEE_SERVICE_GLOBAL;
static const TEEC_UUID UUID05 = TEE_SERVICE_ECHO;
static const TEEC_UUID UUID06 = TEE_SERVICE_STORAGE;
static const TEEC_UUID UUID07 = TEE_SERVICE_UT;
static const TEEC_UUID UUID08 = TEE_SERVICE_CRYPT;

int test_elf_load(TEEC_UUID* uuid);

int main(void)
{
    int ret;
    int i;
    printf("--------- test trusted app load --------\n");

    for (i = 0; i < 1000; i++) {

        printf(" ------------test times: %d -----------------\n", i);
        /* will ok */
        if (i == 0) {
        ret = test_elf_load(&UUID01);
        if (ret < 0) {
            printf(" test case 1 error\n");
            return 0;
        }
        printf("test case 1 passs\n");
        }

        /* will fail file read error */
        ret = test_elf_load(&UUID02);
        if (ret == 0) {
            printf(" test case 2 error\n");
            return 0;
        }
        printf("test case 2 passs\n");

        /* will fail : file open err */
        ret = test_elf_load(&UUID03);
        if (ret == 0) {
            printf(" test case 3 error\n");
            return 0;
        }
        printf("test case 3 passs\n");

#if 1
        /* will ok */
        ret = test_elf_load(&UUID04);
        if (ret < 0) {
            printf(" test case 4 error\n");
            return 0;
        }
        printf("test case 4 passs\n");
#endif
        /* will ok */
        ret = test_elf_load(&UUID05);
        if (ret < 0) {
            printf(" test case 5 error\n");
            return 0;
        }
        printf("test case 5 passs\n");

        /* will ok */
        ret = test_elf_load(&UUID06);
        if (ret < 0) {
            printf(" test case 6 error\n");
            return 0;
        }
        printf("test case 6 passs\n");

        /* will ok */
        ret = test_elf_load(&UUID07);
        if (ret < 0) {
            printf(" test case 7 error\n");
            return 0;
        }
        printf("test case 7 passs\n");

        /* will ok */
        ret = test_elf_load(&UUID08);
        if (ret < 0) {
            printf(" test case 8 error\n");
            return 0;
        }
        printf("test case 8 passs\n");

        /* will fail, hash verify error */
        ret = test_elf_load(&UUID09);
        if (ret == 0) {
            printf(" test case 9 error\n");
            return 0;
        }
        printf("test case 9 passs\n");

    }

    return 0;
}

int test_elf_load(TEEC_UUID* uuid)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Operation operation;
    TEEC_Result result;

    TEEC_Debug("elf load begin\n");
    result = TEEC_InitializeContext(
               NULL,
               &context);

    TEEC_Debug("elf load open session\n");

    if(result != TEEC_SUCCESS) {
       TEEC_Error("teec initial failed\n");
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
                uuid,
                TEEC_LOGIN_IDENTIFY,
                NULL,
                &operation,
                NULL);

    if(result == TEEC_ERROR_TRUSTED_APP_LOAD_ERROR) {
        TEEC_Error("teec opensession start app error\n");
        TEEC_FinalizeContext (&context);
        return -1;
    }

    TEEC_Debug("app load ok\n");
    TEEC_FinalizeContext (&context);

    return 0;
}
