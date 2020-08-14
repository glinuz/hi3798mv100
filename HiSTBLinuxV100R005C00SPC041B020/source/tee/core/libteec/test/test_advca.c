#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
//#include <sys/time.h>
//#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_id.h"

 /* example uuid */
static const TEEC_UUID stb_test_uuid =
{
    0x0E0E0E0E, \
    0x0E0E, \
    0x0E0E, \
        { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E }
};
#define  	TEST_TZ_ADVCA_GetOtpFuse        35
#define 	TEST_TZ_ADVCA_SetR2RAlg         36
#define 	TEST_TZ_ADVCA_SetR2RSessionKey  37
#define     TEST_TZ_ADVCA_R2R_CRYPTO        38
#define     TEST_TZ_ADVCA_DERIVE_KEY        39
#define     TEST_TZ_ADVCA_SP_CRYPTO         40
#define     TEST_TZ_ADVCA_MISC_CRYPTO       41
#define     TEST_TZ_ADVCA_GET_ANTIROLLBACK_VERSION  42
#define     TEST_TZ_ADVCA_SET_ANTIROLLBACK_VERSION  43
#define     TEST_TZ_ADVCA_DCAS_OPEN         44
#define     TEST_TZ_ADVCA_DCA_CLOSE         45
#define     TEST_TZ_ADVCA_DCAS_GETCHIPID    46
#define     TEE_ADVCA_CMD_DCAS_SET_SESSION_KEY 47


static unsigned char g_u8OutputBuf[16];
//static unsigned char g_u8ESCK[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//VendorSysId is a short integer, 2byte¡ê?should be extended to 16byte with 0x00.
//For example, the VendorSysId in this sample is 0x0a.
unsigned char g_u8Vendor_ID[16] = {0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

//TDES
unsigned char g_u8TdesEK2[16] = {0x9a,0x3a,0x13,0xa4,0x08,0x40,0x9e,0x62,0xd3,0x50,0x0d,0x06,0x35,0x36,0x9a,0x09};
unsigned char g_u8TdesEK1[16] = {0xdd,0x20,0x99,0xa6,0xc9,0x83,0x6a,0xf7,0xe9,0xe3,0xdd,0xdf,0xf8,0x33,0x26,0x0c};
//AES
//HI_U8 g_u8AesEK2[16] = {0x28,0x5b,0x79,0xe4,0x2b,0xb3,0x5d,0x6f,0x92,0x03,0x61,0x98,0xbf,0xad,0x1c,0x3a};
unsigned char g_u8AesEK2[16] = {0x00,0x5b,0x79,0xe4,0x2b,0xb3,0x5d,0x6f,0x92,0x03,0x61,0x98,0xbf,0xad,0x1c,0x3a};
unsigned char g_u8AesEK1[16] = {0x75,0xc4,0x41,0x3c,0xbc,0xfc,0x1e,0x64,0xa5,0xe7,0x6b,0x05,0x30,0xc6,0xc4,0xe2};



static int advca_getotpfuse(TEEC_Session session, unsigned int enFuse)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    uint32_t u32Value;

    operation.started = 1;
    operation.params[0].value.a = enFuse;
    operation.params[1].tmpref.buffer = &u32Value;
    operation.params[1].tmpref.size = sizeof (u32Value);

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_GetOtpFuse, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    if (1 == enFuse)
    {
        printf("R2R keyladder lev is :%d\n", u32Value);
    }
    else if (2 == enFuse)
    {
        printf("TZ area lock flag :%d\n", u32Value);
    }
    return 0;
}


static int advca_setr2ralg(TEEC_Session session, unsigned int enR2RAlg)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    operation.started = 1;
    operation.params[0].value.a = enR2RAlg;

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_SetR2RAlg, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}

static int advca_setr2rsessionkey(TEEC_Session session, unsigned int enLevel, unsigned char *pu8Key)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    operation.started = 1;
    operation.params[0].value.a = enLevel;
    operation.params[1].tmpref.buffer = pu8Key;
    operation.params[1].tmpref.size   = 16;

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_SetR2RSessionKey, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}


static int advca_r2rcrypto(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    operation.started = 1;
    operation.params[0].value.a = 0;


    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_R2R_CRYPTO, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}


static int advca_spcrypto(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    operation.started = 1;
    operation.params[0].value.a = 0;


    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_SP_CRYPTO, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}


static int advca_misccrypto(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    operation.started = 1;
    operation.params[0].value.a = 0;


    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_MISC_CRYPTO, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}

static int advca_antirollbackversion(TEEC_Session session, unsigned int select, unsigned int addr, unsigned int length)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;

    printf("addr:%x, length:%x\n", addr, length);

    operation.started = 1;
    operation.params[0].value.a = addr;
    operation.params[0].value.b = length;


    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    if (1 == select)
    {
        result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_GET_ANTIROLLBACK_VERSION, &operation, &returnOrigin);
    }
    else
    {
        result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_SET_ANTIROLLBACK_VERSION, &operation, &returnOrigin);
    }

    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }

    return 0;
}


static int advca_derivekey(TEEC_Session session)
{
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t cmd = 0;
    uint32_t returnOrigin;
    uint32_t i = 0;

    uint8_t dataIn[16] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
                    0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0};
    uint8_t dataOut[16] = {0};


    operation.started = 1;
    operation.params[0].tmpref.buffer = dataIn;
    operation.params[0].tmpref.size = 16;
    operation.params[1].tmpref.buffer = dataOut;
    operation.params[1].tmpref.size = 16;



    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);

    result = TEEC_InvokeCommand(&session, TEST_TZ_ADVCA_DERIVE_KEY, &operation, &returnOrigin);
    if (result != TEEC_SUCCESS) {
        printf("invoke failed, codes=0x%x, returnOrigin=0x%x\n", result, returnOrigin);
        return -1;
    }
    printf("outputkey:");
    for (i = 0; i < 16; i++)
    {
        printf("%2x", dataOut[i]);
    }
    printf("\n");

    return 0;
}


#if 0
typedef enum {
    TEEC_ADVCA_CMD_DCAS_OPEN            = 0xF0,
    TEEC_ADVCA_CMD_DCAS_CLOSE           = 0xF1,
    TEEC_ADVCA_CMD_DCAS_GET_CHIPID      = 0xF2,
    TEEC_ADVCA_CMD_DCAS_SET_SESSION_KEY = 0xF3,
} HI_ADVCA_DCAS_COMMON_ID_E;

int teec_dcas_open(TEEC_Session session, HI_TEE_ADVCA_ALG_TYPE_E alg)
{
    TEEC_Operation operation = {0};
    TEEC_Result result;
    uint32_t origin;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].value.a = alg;

    result = TEEC_InvokeCommand(
                 &session,
                 TEST_TZ_ADVCA_DCAS_OPEN,
                 &operation,
                 &origin);
    if (result != TEEC_SUCCESS) {
        printf("[DCAS test] invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        return -1;
    }
    return 0;
}

int teec_dcas_close(TEEC_Session session)
{
    TEEC_Operation operation = {0};
    TEEC_Result result;
    uint32_t origin;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    result = TEEC_InvokeCommand(
                 &session,
                 TEST_TZ_ADVCA_DCA_CLOSE,
                 &operation,
                 &origin);
    if (result != TEEC_SUCCESS)
	{
        printf("[DCAS test] invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        return -1;
    }
    return 0;
}

int teec_dcas_getchipid(TEEC_Session session, HI_U8 chipid[8])
{
	TEEC_Operation operation = {0};
    TEEC_Result result;
    uint32_t origin;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INOUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[0].tmpref.buffer = chipid;
    operation.params[0].tmpref.size = 8;

    result = TEEC_InvokeCommand(
                 &session,
                 TEST_TZ_ADVCA_DCAS_GETCHIPID,
                 &operation,
                 &origin);
    if (result != TEEC_SUCCESS)
	{
        printf("[DCAS test] invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        return -1;
    }
    return 0;
}

int teec_dcas_setDCASSesssionKey(TEEC_Session session, HI_TEE_ADVCA_DCAS_KEYLADDER_LEV_E enDCASLevel, HI_U8 au8Key[16], HI_U8 au8Output[16])
{
    TEEC_Operation operation = {0};
    TEEC_Result result;
    uint32_t origin;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_MEMREF_TEMP_INOUT,
        TEEC_MEMREF_TEMP_INOUT,
        TEEC_NONE);
    operation.params[0].value.a = enDCASLevel;
    operation.params[1].tmpref.buffer = au8Key;
    operation.params[1].tmpref.size = 16;
    operation.params[2].tmpref.buffer = au8Output;
    operation.params[2].tmpref.size = 16;
    result = TEEC_InvokeCommand(
                 &session,
                 TEE_ADVCA_CMD_DCAS_SET_SESSION_KEY,
                 &operation,
                 &origin);
    if (result != TEEC_SUCCESS)
	{
        printf("[DCAS test] invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
        return -1;
    }
    return 0;
}

int teec_dcas_decrypt_testR2R(TEEC_Session session, HI_TEE_ADVCA_ALG_TYPE_E s32KldAlg)
{
	if(s32KldAlg == 0)
	{
		//Set the algorithm to TDES
		teec_dcas_open(session, HI_TEE_ADVCA_ALG_TYPE_TDES);
		//Set Vendor ID
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
		//Set EK2
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_EK2, g_u8TdesEK2, g_u8OutputBuf);
		//Set EK1
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_EK1, g_u8TdesEK1, g_u8OutputBuf);
		//set E-KEY
		teec_dcas_close(session);
	}
	else
	{
		//Set the algorithm to AES
		teec_dcas_open(session, HI_TEE_ADVCA_ALG_TYPE_AES);
		//Set Vendor ID
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
		//Set EK2
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_EK2, g_u8AesEK2, g_u8OutputBuf);
		//Set EK1
		teec_dcas_setDCASSesssionKey(session, HI_TEE_ADVCA_DCAS_KEYLADDER_EK1, g_u8AesEK1, g_u8OutputBuf);
		//set E-KEY
		teec_dcas_close(session);
	}
	return 0;
}

static int advca_dcascrypto(TEEC_Session session)
{
    int ret;
    unsigned char u8chipid[8];
    int index;

    ret = teec_dcas_getchipid(session, u8chipid);
    if (0 == ret)
    {
		for (index = 0; index < 8; index++)
		{
	        printf("[DCAS test] chipid[%d] = 0x%02x \n", index, u8chipid[index]);
		}
    }

    ret = teec_dcas_decrypt_testR2R(session, HI_TEE_ADVCA_ALG_TYPE_AES);
    if (0 != ret)
    {
        printf("teec_dcas_decrypt_testR2R failed\n");
        return -1;
    }
    
	ret = teec_dcas_decrypt_testR2R(session, HI_TEE_ADVCA_ALG_TYPE_TDES);
    if (0 != ret)
    {
        printf("teec_dcas_decrypt_testR2R failed\n");
        return -1;
    }

ERR:
	if (0 == ret)
	{
	    printf("[DCAS test] test OK\n");
	}
    else
    {
		printf("[DCAS test] test no OK\n");
    }    

    return 0;
}

#endif

int main(int argc,char *argv[])
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    unsigned int mode = 0;
    unsigned int value = 0;
    unsigned int addr = 0;
    unsigned int length = 0;

    usleep(100*1000);

    if (argc <= 1)
    {
        printf("Usage: ./test_advca mode [value]\n"
               "       ./test_advca 0 1 to GetR2RkeyLadderLevel \n"
               "       ./test_advca 0 2 to GetTZAreaLockFlag\n"
               "       ./test_advca 1   to testR2R CRYPTO\n"
               "       ./test_advca 2   to test derive key api\n"
               "       ./test_advca 3   to test sp crypto\n"
               "       ./test_advca 4   to test misc crypto\n"
               "       ./test_advca 5 1 to test get antirollback version\n"
               "       ./test_advca 5 2 to test set antirollback version\n");
        return -1;
    }


    if (argc == 2)
    {
        mode = strtol(argv[1],NULL,0);
        printf("mode = %d\n",mode);
    }
    else if (argc == 3)
    {
        mode = strtol(argv[1],NULL,0);
        printf("mode = %d\n",mode);

        value = strtol(argv[2],NULL,0);
        printf("value = %d\n",value);
    }
    else if (argc == 5)
    {
        mode = strtol(argv[1],NULL,0);
        printf("mode = %d\n",mode);

        value = strtol(argv[2],NULL,0);
        printf("value = %d\n", value);

        addr = strtol(argv[3], NULL, 0);
        printf("addr = %x\n", addr);

        length = strtol(argv[4], NULL, 0);
        printf("length = %x\n", length);
    }


    usleep(100*1000);

    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        printf("teec initial failed\n");
        goto cleanup_1;
    }

    result = TEEC_OpenSession(&context, &session, &stb_test_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        printf("teec open session failed\n");
        goto cleanup_2;
    }

    switch(mode)
    {
        case 0:
            advca_getotpfuse(session, value);
            break;
        case 1:
            advca_r2rcrypto(session);
            break;
        case 2:
            advca_derivekey(session);
            break;
        case 3:
            advca_spcrypto(session);
            break;
        case 4:
            advca_misccrypto(session);
            break;
        case 5:
            advca_antirollbackversion(session, value, addr, length);
            break;
        case 6:
            //advca_dcascrypto(session);
            break;
        default:
            printf("invalid mode=%d\n", mode);
            break;
    }

    usleep(100*1000);

    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return 0;
}
