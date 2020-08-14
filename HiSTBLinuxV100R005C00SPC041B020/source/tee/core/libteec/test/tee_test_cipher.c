#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "hi_tee_cipher.h"
#ifndef HI_SMMU_SUPPORT
#include "sec_mmz.h"
#endif
#ifdef HI_TEE_SUPPORT
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "mpi_mmz.h"
#endif 

int  CIPHER_SEC_PHY_print(TEEC_Session session, const char *name, unsigned int u32Pha, unsigned int u32Size, HI_BOOL bIsSMMu);
int  CIPHER_SEC_VIA_print(TEEC_Session session, const char *name, unsigned int u32SecVia, unsigned int u32Size);
int  CIPHER_SEC_memset(TEEC_Session session, unsigned int u32SecVia, unsigned char val, unsigned int u32Size);
int  CIPHER_SEC_memcmp(TEEC_Session session, unsigned int u32SecVia, const unsigned char *pu8Check, unsigned char u8Size);
int  CIPHER_SEC_memcpy(TEEC_Session session, unsigned int u32SecPhy, const unsigned char *pu8Data, unsigned char u8Size);
int  CIPHER_SEC_REG_read(TEEC_Session session, unsigned int u32RegAddr, unsigned int u32Size );
int  CIPHER_SEC_REG_write(TEEC_Session session, unsigned int u32RegAddr, unsigned int u32Val);

#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

typedef enum {
	TEST_SELF_TEST                   = 0,
	TEST_SEC_MMZ                     = 1,
	TEST_CIPHER                      = 2,
	TEST_VFMW                        = 3,
	TEST_CRYPTO                      = 4,
	TEST_CANAL                       = 5,
	TEST_TZ_HDCP_SETHDMISTATUS        = 6,
	TEST_TZ_CIPHER_INIT               = 7,
	TEST_TZ_CIPHER_DEINIT             = 8,
	TEST_TZ_CIPHER_ENCRYPT            = 9,
	TEST_TZ_CIPHER_ENCRYPTMULTI       = 10,
	TEST_TZ_CIPHER_DECRYPT            = 11,
	TEST_TZ_CIPHER_DECRYPTMULTI       = 12,
	TEST_TZ_CIPHER_CONFIGHANDLE       = 13,
	TEST_TZ_CIPHER_CREATEHANDLE       = 14,
	TEST_TZ_CIPHER_DESTROYHANDLE      = 15,
	TEST_TZ_CIPHER_GETHANDLECONFIG    = 16,
	TEST_TZ_CIPHER_GETRANDOMNUMBER    = 17,
	TEST_TZ_CIPHER_ENCRYPTHDCPKEY     = 18,
	TEST_TZ_CIPHER_LOADHDCPKEY        = 19,
	TEST_TZ_CIPHER_HASHINIT           = 20,
	TEST_TZ_CIPHER_HASHUPDATE         = 21,
	TEST_TZ_CIPHER_HASHFINAL          = 22,
	TEST_TZ_OTP_TEST                  = 23,
	TEST_TZ_CIPHER_TEST               = 24,
	TEST_TZ_ADVCA_GDRMENCRYPT         = 25,
	TEST_TZ_TEST                      = 26,
	TEST_TZ_OTP_INIT                  = 27,
	TEST_TZ_OTP_DEINIT                = 28,
	TEST_TZ_OTP_SetReset              = 29,
	TEST_TZ_OTP_GetChipID             = 30,
	TEST_TZ_OTP_ReadWord              = 31,
	TEST_TZ_OTP_ReadByte              = 32,
	TEST_TZ_OTP_WriteWord             = 33,
	TEST_TZ_OTP_WriteByte             = 34,
	TEST_TZ_ADVCA_GetOtpFuse          = 35,
	TEST_TZ_ADVCA_SetR2RAlg           = 36,
	TEST_TZ_ADVCA_SetR2RSessionKey    = 37,
	TEST_TZ_ADVCA_R2R_CRYPTO          = 38,
	TEST_TZ_ADVCA_DERIVE_KEY          = 39,
	TEST_SRAM                         = 40,
	TEST_TZ_BUTT                      ,
}stb_test_cmd_t;

static const TEEC_UUID s_stTeecCipherUUID =
{
    0x0E0E0E0E, \
    0x0E0E, \
    0x0E0E, \
    { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E }
};

#ifdef HI_TEE_SUPPORT
/*#######################MPI API#######################################*/
static int g_CipherDevFd = -1;
static HI_TEE_CIPHER_HASH_TYPE_E s_eShaType = HI_TEE_CIPHER_HASH_TYPE_BUTT;
static pthread_mutex_t   g_CipherMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()  	     (void)pthread_mutex_lock(&g_CipherMutex);
#define HI_CIPHER_UNLOCK()  	 (void)pthread_mutex_unlock(&g_CipherMutex);

#define CHECK_CIPHER_INIT()\
do{\
    HI_CIPHER_LOCK();\
    if (g_CipherDevFd < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        HI_CIPHER_UNLOCK();\
        return HI_ERR_CIPHER_NOT_INIT;\
    }\
    HI_CIPHER_UNLOCK();\
}while(0)

#define HASH_BLOCK_SIZE		(64)
#define HASH_PAD_MAX_LEN	(64)
#define HASH1_SIGNATURE_SIZE		(20)
#define HASH256_SIGNATURE_SIZE		(32)
#define HASH_MMZ_BUF_LEN	(10*1024*1024)		//10M
#define HASH_MMZ_TAIL_LEN	(8*1024)			//8K

#ifndef SEC_MMZ_ZONE
#define SEC_MMZ_ZONE        "SEC-MMZ"
#endif

static unsigned char g_au8HmacKey[HASH_BLOCK_SIZE];
static unsigned int u32HmacKeyLen = 0;
static	TEEC_Context s_stTeecContext;
static	TEEC_Session s_stTeecSession;

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/*********************************************************
 * The function below is added for AES CBC-MAC
 *
 *********************************************************/
#define MAX_DATA_LEN    (0x2000) //the max data length for encryption / decryption is  8k one time

static int CIPHER_HashInit(HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_HASHINIT;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_TEE_CIPHER_HASH_TYPE_BUTT))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
    							        TEEC_MEMREF_TEMP_INPUT,
    							        TEEC_VALUE_INOUT,
    							        TEEC_NONE,
    							        TEEC_NONE);
    stTeecOperation.params[0].tmpref.buffer = (void *)pstHashAttr;
    stTeecOperation.params[0].tmpref.size = sizeof(HI_TEE_CIPHER_HASH_ATTS_S);
    stTeecOperation.params[1].value.a = *pHashHandle;
    
    stTeecOperation.started = 1;
	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
	}

    *pHashHandle = stTeecOperation.params[1].value.a;
        
    return 0;
}

static int CIPHER_HashUpdate(HI_HANDLE hHashHandle, unsigned char *pu8InputData, unsigned int u32InputDataLen)
{
    TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_HASHUPDATE;

    if(NULL == pu8InputData)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_MEMREF_TEMP_INPUT,
										        TEEC_NONE,
										        TEEC_NONE);

    stTeecOperation.params[0].value.a = hHashHandle;
    stTeecOperation.params[1].tmpref.buffer = pu8InputData;
    stTeecOperation.params[1].tmpref.size = u32InputDataLen;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( 0 != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return  -1;
    }

    return 0;
}

static int CIPHER_HashFinal(HI_HANDLE hHashHandle, unsigned char *pu8OutputHash)
{
    TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd;
    unsigned int u32ShaLen;

    if(NULL == pu8OutputHash)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return -1;
    }

    CHECK_CIPHER_INIT();

    if ((s_eShaType == HI_TEE_CIPHER_HASH_TYPE_SHA1) || (s_eShaType == HI_TEE_CIPHER_HASH_TYPE_HMAC_SHA1))
    {
        u32ShaLen = 20;
    }
    else
    {
        u32ShaLen = 32;
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    u32TeeCmd = TEST_TZ_CIPHER_HASHFINAL;
	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_MEMREF_TEMP_OUTPUT,
										        TEEC_NONE,
										        TEEC_NONE);

    stTeecOperation.params[0].value.a = hHashHandle;
    stTeecOperation.params[1].tmpref.buffer = pu8OutputHash;
    stTeecOperation.params[1].tmpref.size = u32ShaLen;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( 0 != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return  -1;
    }
    memcpy(pu8OutputHash, stTeecOperation.params[1].tmpref.buffer, u32ShaLen);


    return 0;
}

static int CIPHER_HmacKeyInit(HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr, unsigned char au8Hmackey[HASH_BLOCK_SIZE])
{
    int ret = 0;
    HI_HANDLE hHash = 0;

    if((pstHashAttr == NULL)||(pstHashAttr->pu8HMACKey == NULL)||(pstHashAttr->u32HMACKeyLen == 0))
    {
        HI_ERR_CIPHER("Invalid parameters.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*key length is less than 64bytes, copy directly*/
    if(pstHashAttr->u32HMACKeyLen <= HASH_BLOCK_SIZE)
    {
        memcpy(au8Hmackey, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
		u32HmacKeyLen = pstHashAttr->u32HMACKeyLen;
        return 0;
    }

    /*key length more than 64bytes, calcute the hash result*/
    ret = CIPHER_HashInit(pstHashAttr, &hHash);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Cipher_HashInit failure, ret=%d\n", ret);
        return -1;
    }

    ret = CIPHER_HashUpdate(hHash, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Cipher_HashUpdate failure, ret=%d\n", ret);
        return -1;
    }

    ret = CIPHER_HashFinal(hHash,au8Hmackey);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Cipher_HashFinal failure, ret=%d\n", ret);
        return -1;
    }

	if ( HI_TEE_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType )
	{
		u32HmacKeyLen = 20;
	}
	else
	{
		u32HmacKeyLen = 32;
	}

    return 0;
}

static int CIPHER_HmacInit(HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    int ret = 0;
    unsigned char i_key_pad[HASH_BLOCK_SIZE];
	unsigned int i = 0;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_TEE_CIPHER_HASH_TYPE_BUTT)
     || (NULL == pstHashAttr->pu8HMACKey)
     || (0 == pstHashAttr->u32HMACKeyLen))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

	/* Init hmac key */
    memset(g_au8HmacKey, 0, sizeof(g_au8HmacKey));
    ret = CIPHER_HmacKeyInit(pstHashAttr, g_au8HmacKey);
	if ( 0 != ret )
	{
	    HI_ERR_CIPHER("Hmac key initial failed!\n");
		return -1;
	}

	/* hash i_key_pad and message start */
	ret = CIPHER_HashInit(pstHashAttr, pHashHandle);
	if ( 0 != ret )
	{
	    HI_ERR_CIPHER("hash i_key_pad and message start failed!\n");
		return -1;
	}

	/* generate i_key_pad */
	memset(i_key_pad, 0x36, HASH_BLOCK_SIZE);
    for(i=0; i < u32HmacKeyLen; i++)
    {
        i_key_pad[i] = i_key_pad[i] ^ g_au8HmacKey[i];
    }

	/* hash i_key_pad update */
	ret = CIPHER_HashUpdate(*pHashHandle, i_key_pad, sizeof(i_key_pad));
	if ( 0 != ret )
	{
	    HI_ERR_CIPHER("hash i_key_pad and message update failed!\n");
		return -1;
	}

    return 0;
}

static int CIPHER_HmacUpdate(HI_HANDLE hHashHandle, unsigned char *pu8InputData, unsigned int u32InputDataLen)
{
    int ret = 0;

	ret = CIPHER_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
	if ( 0 != ret )
	{
	    HI_ERR_CIPHER("hmac message update failed!\n");
		return -1;
	}

    return 0;
}

static int CIPHER_HmacFinal(HI_HANDLE hHashHandle, unsigned char *pu8Output)
{
    int ret = 0;
    HI_HANDLE hHash = HI_INVALID_HANDLE;
	HI_TEE_CIPHER_HASH_ATTS_S stHashAttr;
    unsigned char hash_sum_1[HASH256_SIGNATURE_SIZE];
    unsigned char o_key_pad[HASH_BLOCK_SIZE];
    unsigned int i;

	if ( NULL == pu8Output )
	{
   	    HI_ERR_CIPHER("Invalid param input, hmac final failed!\n");
		return HI_ERR_CIPHER_INVALID_POINT;
	}

    /* hash i_key_pad+message finished */
    memset(hash_sum_1, 0, sizeof(hash_sum_1));
    ret = CIPHER_HashFinal(hHashHandle, hash_sum_1);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Hash Final i_key_pad+message failure, ret=%d\n", ret);
        return -1;
    }

    /* generate o_key_pad */
    memset(o_key_pad, 0x5c, HASH_BLOCK_SIZE);
    for(i=0; i < u32HmacKeyLen; i++)
    {
        o_key_pad[i] = o_key_pad[i] ^ g_au8HmacKey[i];
    }

    /* hash o_key_pad+hash_sum_1 start */
	memset(&stHashAttr, 0, sizeof(stHashAttr));
    ret = CIPHER_HashInit(&stHashAttr, &hHash);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Hash Init o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return -1;
    }

    /* hash o_key_pad */
    ret = CIPHER_HashUpdate(hHash, (unsigned char *)o_key_pad, HASH_BLOCK_SIZE);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Hash Update o_key_pad failure, ret=%d\n", ret);
        return -1;
    }

    /* hash hash_sum_1 */
	if (s_eShaType == HI_TEE_CIPHER_HASH_TYPE_SHA1)
	{
	    ret = CIPHER_HashUpdate(hHash, (unsigned char *)hash_sum_1, HASH1_SIGNATURE_SIZE);
	}
	else
	{
	    ret = CIPHER_HashUpdate(hHash, (unsigned char *)hash_sum_1, HASH256_SIGNATURE_SIZE);
	}

    if(ret != 0)
    {
        HI_ERR_CIPHER("Hash Update hash_sum_1 failure, ret=%d\n", ret);
        return -1;
    }

    /* hash o_key_pad+hash_sum_1 finished */
    ret = CIPHER_HashFinal(hHash, pu8Output);
    if(ret != 0)
    {
        HI_ERR_CIPHER("Hash Final o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return -1;
    }

    return 0;
}

static int TEE_CIPHER_Init(void)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_INIT;
	TEEC_Operation sess_op;

    HI_CIPHER_LOCK();

	memset(&s_stTeecContext, 0, sizeof(s_stTeecContext));
	memset(&s_stTeecSession, 0, sizeof(s_stTeecSession));
	memset(&stTeecResult, 0, sizeof(stTeecResult));

    if (g_CipherDevFd > 0)
    {
        HI_CIPHER_UNLOCK();
        return 0;
    }

	stTeecResult = TEEC_InitializeContext(NULL, &s_stTeecContext);
    if(TEEC_SUCCESS != stTeecResult)
	{
		HI_ERR_CIPHER("Teec Initialize context failed!\n");
	    HI_CIPHER_UNLOCK();
        return -1;;
    }

	memset(&sess_op, 0, sizeof(TEEC_Operation));
	sess_op.started = 1;
	sess_op.paramTypes = TEEC_PARAM_TYPES(
					    TEEC_NONE,
					    TEEC_NONE,
					    TEEC_MEMREF_TEMP_INPUT,
					    TEEC_MEMREF_TEMP_INPUT);
	stTeecResult = TEEC_OpenSession(&s_stTeecContext,
				                    &s_stTeecSession,
				                    &s_stTeecCipherUUID,
				                    TEEC_LOGIN_IDENTIFY,
				                    NULL,
				                    &sess_op,
				                    NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec open session failed!\n");
		(void)TEEC_FinalizeContext(&s_stTeecContext);
		HI_CIPHER_UNLOCK();
		return -1;
	}

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE);

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        HI_CIPHER_UNLOCK();
		return  -1;
	}

    g_CipherDevFd = 1;

    HI_CIPHER_UNLOCK();
    
    return 0;
}

static int TEE_CIPHER_DeInit(void)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_DEINIT;

    HI_CIPHER_LOCK();

    if (g_CipherDevFd <= 0)
    {
        HI_CIPHER_UNLOCK();
        return 0;
    }

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE);

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        HI_CIPHER_UNLOCK();
		return  -1;
	}

	(void)TEEC_CloseSession(&s_stTeecSession);
	(void)TEEC_FinalizeContext(&s_stTeecContext);

    g_CipherDevFd = -1;

    HI_CIPHER_UNLOCK();

    return 0;
}

static int TEE_CIPHER_CreateHandle(HI_HANDLE* phCipher, const HI_TEE_CIPHER_ATTS_S *pstCipherAttr)
{
    int ret = 0;
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_CREATEHANDLE;

	if ( ( NULL == phCipher ) || ( NULL == pstCipherAttr ) )
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INOUT,
										        TEEC_MEMREF_TEMP_INPUT,
										        TEEC_NONE,
										        TEEC_NONE);
	stTeecOperation.params[0].value.a = *phCipher;
	stTeecOperation.params[1].tmpref.buffer = (void *)pstCipherAttr;
	stTeecOperation.params[1].tmpref.size = sizeof(HI_TEE_CIPHER_ATTS_S);

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		ret =  -1;
	}

    *phCipher = stTeecOperation.params[0].value.a;

    return ret;
}

static int TEE_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_DESTROYHANDLE;

    CHECK_CIPHER_INIT();

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE);
	stTeecOperation.params[0].value.a = hCipher;
	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
	}

    return 0;
}


static int TEE_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_TEE_CIPHER_CTRL_S* pstCtrl)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_CONFIGHANDLE;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
#if 0
	if(HI_TRUE == pstCtrl->bKeyByCA)
	{
		ret = HI_MPI_ADVCA_SetKeyladderSessionKey(pstCtrl, hCipher);
		if(0 != ret)
		{
	        HI_ERR_CIPHER("mpi set keyladder session key failed!\n");
	        return -1;
		}
	}
#endif

	CHECK_CIPHER_INIT();

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_MEMREF_TEMP_INPUT,
										        TEEC_NONE,
										        TEEC_NONE);
	stTeecOperation.params[0].value.a = (unsigned int)hCipher;
	stTeecOperation.params[1].tmpref.buffer = (void *)pstCtrl;
	stTeecOperation.params[1].tmpref.size = sizeof(HI_TEE_CIPHER_CTRL_S);

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
	}

    return 0;
}

static int TEE_CIPHER_Encrypt(HI_HANDLE hCipher, unsigned int u32SrcPhyAddr, unsigned int u32DestPhyAddr, unsigned int u32ByteLength)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_ENCRYPT;

	CHECK_CIPHER_INIT();

    if(0 == (hCipher & 0x00ff))
    {
        if ( 16 != u32ByteLength )
        {
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    if ( u32ByteLength < HI_TEE_CIPHER_MIN_CRYPT_LEN || u32ByteLength > HI_TEE_CIPHER_MAX_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if((0 == u32SrcPhyAddr) || (0 == u32DestPhyAddr))
	{
		HI_ERR_CIPHER("Invali parameter, null address input!\n");
		return -1;
	}

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_VALUE_INPUT,
										        TEEC_NONE,
										        TEEC_NONE);
	stTeecOperation.params[0].value.a = (unsigned int)hCipher;
	stTeecOperation.params[0].value.b= u32SrcPhyAddr;
	stTeecOperation.params[1].value.a = u32DestPhyAddr;
	stTeecOperation.params[1].value.b = u32ByteLength;
	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
    {
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
    }

    return 0;
}

static int TEE_CIPHER_Decrypt(HI_HANDLE hCipher, unsigned int u32SrcPhyAddr, unsigned int u32DestPhyAddr, unsigned int u32ByteLength)
{
    TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_DECRYPT;

	CHECK_CIPHER_INIT();

    if ( 0 == (hCipher & 0x00ff) )
    {
        if ( 16 != u32ByteLength )
        {
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    if (u32ByteLength < HI_TEE_CIPHER_MIN_CRYPT_LEN || u32ByteLength > HI_TEE_CIPHER_MAX_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }
	if((0 == u32SrcPhyAddr) || (0 == u32DestPhyAddr))
	{
		HI_ERR_CIPHER("Invali parameter, null address input!\n");
		return -1;
	}
	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_INPUT,
										        TEEC_VALUE_INPUT,
										        TEEC_NONE,
										        TEEC_NONE);
	stTeecOperation.params[0].value.a = (unsigned int)hCipher;
	stTeecOperation.params[0].value.b= u32SrcPhyAddr;
	stTeecOperation.params[1].value.a = u32DestPhyAddr;
	stTeecOperation.params[1].value.b = u32ByteLength;

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
    {
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
    }

    return 0;
}

static int TEE_CIPHER_HashInit(HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    s_eShaType = pstHashAttr->eShaType;

	if ( (HI_TEE_CIPHER_HASH_TYPE_SHA1 == s_eShaType) || (HI_TEE_CIPHER_HASH_TYPE_SHA256 == s_eShaType) )
	{
	    return CIPHER_HashInit(pstHashAttr, pHashHandle);
	}
	else
	{
	    return CIPHER_HmacInit(pstHashAttr, pHashHandle);
	}
}

static int TEE_CIPHER_HashUpdate(HI_HANDLE hHashHandle, unsigned char *pu8InputData, unsigned int u32InputDataLen)
{
	if ( (HI_TEE_CIPHER_HASH_TYPE_SHA1 == s_eShaType) || (HI_TEE_CIPHER_HASH_TYPE_SHA256 == s_eShaType) )
	{
		return CIPHER_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
	}
	else
	{
	    return CIPHER_HmacUpdate(hHashHandle, pu8InputData, u32InputDataLen);
	}
}

static int TEE_CIPHER_HashFinal(HI_HANDLE hHashHandle, unsigned char *pu8OutputHash)
{
	if ( (HI_TEE_CIPHER_HASH_TYPE_SHA1 == s_eShaType) || (HI_TEE_CIPHER_HASH_TYPE_SHA256 == s_eShaType) )
	{
		return CIPHER_HashFinal(hHashHandle, pu8OutputHash);
	}
	else
	{
	    return CIPHER_HmacFinal(hHashHandle, pu8OutputHash);
	}
}

static int TEE_CIPHER_GetRandomNumber(HI_U32 *pu32Rnd)
{
	TEEC_Result stTeecResult;
	TEEC_Operation stTeecOperation;
	unsigned int u32TeeCmd = TEST_TZ_CIPHER_GETRANDOMNUMBER;

	CHECK_CIPHER_INIT();

	memset(&stTeecResult, 0, sizeof(stTeecResult));
	memset(&stTeecOperation, 0, sizeof(stTeecOperation));

	stTeecOperation.started = 1;
	stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
										        TEEC_VALUE_OUTPUT,
										        TEEC_NONE,
										        TEEC_NONE,
										        TEEC_NONE);

	stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
	if( 0 != stTeecResult)
    {
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
		return -1;
    }

    *pu32Rnd = stTeecOperation.params[0].value.a;

    return 0;
}


/*#######################Cipher Test sample#######################################*/
#define HI_TVP_CIPHER_TEST

typedef int (*list_func)();

#define TEST_DATA_KEY_LEN   16
#define TEST_CIPHER_CNT     1
#define SEC_MMZ_ZONE        "SEC-MMZ"

static int printBuffer(const HI_CHAR *string, const unsigned char *pu8Input, unsigned int u32Length)
{
    unsigned int i = 0;

    if ( NULL != string )
    {
        printf("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) printf("\n");
        printf("0x%02x ", pu8Input[i]);
    }
    printf("\n");

    return 0;
}

int Setconfiginfo(HI_HANDLE chnHandle, HI_BOOL isKeyByCA, HI_TEE_CIPHER_ALG_E alg, HI_TEE_CIPHER_WORK_MODE_E mode, HI_TEE_CIPHER_KEY_LENGTH_E keyLen,
                                                const unsigned char u8KeyBuf[16], const unsigned char u8IVBuf[16])
{
	int s32Ret = 0;
    HI_TEE_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_TEE_CIPHER_CTRL_S));
    CipherCtrl.bKeyByCA = isKeyByCA;
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_TEE_CIPHER_BIT_WIDTH_64BIT;
    CipherCtrl.enKeyLen = keyLen;
    if(CipherCtrl.enWorkMode != HI_TEE_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    if ( HI_FALSE == isKeyByCA )
    {
        memcpy(CipherCtrl.u32Key, u8KeyBuf, 32);
    }

    s32Ret = TEE_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(0 != s32Ret)
	{
		return -1;
	}
    return 0;
}

#ifdef HI_SMMU_SUPPORT //SMMU
static int AES(HI_TEE_CIPHER_WORK_MODE_E mode,
           const unsigned char u8Key[16], const unsigned char u8IV[16],
           const unsigned char u8Src[16], const unsigned char u8Dst[16])
{
	int s32Ret = 0;
    HI_HANDLE hTestchnid[TEST_CIPHER_CNT] = {0};
    unsigned char *pInputAddrVir = HI_NULL;
    unsigned int u32InputAddrPhy = 0;
    unsigned int u32OutPhy = 0;
    unsigned int u32Testcached = 0;
    HI_TEE_CIPHER_ATTS_S stCipherAttr = {0};
    unsigned int i = 0;
	HI_MPI_SMMU_SEC_ATTR_S SecAttr;
    HI_MPI_SMMU_SEC_BUF_S SecBuf;
	
    stCipherAttr.enCipherType = HI_TEE_CIPHER_TYPE_NORMAL;

    for(i=0; i<TEST_CIPHER_CNT; i++)
    {
        printf("===================%d====================\r\n", i);

        s32Ret = TEE_CIPHER_CreateHandle(&hTestchnid[i], &stCipherAttr);
        if(0 != s32Ret)
    	{
    	    HI_ERR_CIPHER("Error: CreateHandle failed!\n");
    		continue;
    	}

        memset(&SecAttr, 0, sizeof(HI_MPI_SMMU_SEC_ATTR_S));
        memset(&SecBuf, 0, sizeof(HI_MPI_SMMU_SEC_BUF_S));
        
    	strcpy(SecAttr.bufname, "CIPHER_INPUT");
		SecAttr.bufsize = MAX_DATA_LEN;
//		SecAttr.auth_flag=  1;
//		SecAttr.sec_map_flag = 1;

//        SecBuf.auth_flag = 1;
//        SecBuf.sec_map_flag = 1;
        SecBuf.smmu_size = MAX_DATA_LEN;
        
		s32Ret = HI_MPI_SMMU_SecMemAlloc(&SecAttr, &SecBuf);
		if (s32Ret != 0)
		{
			HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
			HI_MMZ_Unmap(u32InputAddrPhy);
			HI_MMZ_Delete(u32InputAddrPhy);
			return -1;;
		}
		u32OutPhy = SecBuf.smmu_addr;
//        printf("MMU: 0x%x, VIA: 0x%x\n", SecBuf.smmu_addr, SecBuf.sec_virt);

        u32InputAddrPhy = (unsigned int)HI_MMZ_New(TEST_DATA_KEY_LEN, 0, NULL, "CIPHER_BufIn");
        if (0 == u32InputAddrPhy)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            goto __CIPHER_EXIT__;
        }
        pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);

    	/* For encrypt */
        s32Ret = Setconfiginfo(hTestchnid[i],
                                HI_FALSE,
                                HI_TEE_CIPHER_ALG_AES,
                                mode,
                                HI_TEE_CIPHER_KEY_AES_128BIT,
                                u8Key,
                                u8IV);
    	if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Set config info failed.\n");
    		goto __CIPHER_EXIT__;
    	}

        memcpy(pInputAddrVir, u8Src, TEST_DATA_KEY_LEN);
        usleep(100000);
        s32Ret = TEE_CIPHER_Encrypt(hTestchnid[i], u32InputAddrPhy, u32OutPhy, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        usleep(100000);
        CIPHER_SEC_PHY_print(s_stTeecSession, "ENCRYPT", SecBuf.smmu_addr, TEST_DATA_KEY_LEN, HI_TRUE);
        s32Ret = CIPHER_SEC_memcmp(s_stTeecSession, SecBuf.smmu_addr, u8Dst, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        
    	/* For decrypt */
    	s32Ret = Setconfiginfo(hTestchnid[i],
                                        HI_FALSE,
                                        HI_TEE_CIPHER_ALG_AES,
                                        mode,
                                        HI_TEE_CIPHER_KEY_AES_128BIT,
                                        u8Key,
                                        u8IV);
    	if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Set config info failed.\n");
    		goto __CIPHER_EXIT__;
    	}

        memcpy(pInputAddrVir, u8Dst, TEST_DATA_KEY_LEN);
        usleep(100000);
        s32Ret = TEE_CIPHER_Decrypt(hTestchnid[i], u32InputAddrPhy, u32OutPhy, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher decrypt failed.\n");
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        usleep(100000);
        CIPHER_SEC_PHY_print(s_stTeecSession, "DECRYPT", SecBuf.smmu_addr, TEST_DATA_KEY_LEN, HI_TRUE);
        s32Ret = CIPHER_SEC_memcmp(s_stTeecSession, SecBuf.smmu_addr, u8Src, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher decrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}

        printf("\033[0;32m""sample %s run successfully!\n""\033[0m",  __FUNCTION__);
        
__CIPHER_EXIT__:

        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        HI_MPI_SMMU_SecMemFree(&SecBuf);
    }

    for(i=0; i<TEST_CIPHER_CNT; i++)
    {
        if (hTestchnid[i])
        {
            TEE_CIPHER_DestroyHandle(hTestchnid[i]);
        }
    }

    return s32Ret;
}
#else //MMZ
static int AES(HI_TEE_CIPHER_WORK_MODE_E mode,
           const unsigned char u8Key[16], const unsigned char u8IV[16],
           const unsigned char u8Src[16], const unsigned char u8Dst[16])
{
	int s32Ret = 0;
    HI_HANDLE hTestchnid[TEST_CIPHER_CNT] = {0};
    HI_MMZ_BUF_S stInBuf;
    HI_TEE_CIPHER_ATTS_S stCipherAttr = {0};
    HI_U32 datalen = 0xFFFF0;
    unsigned int i = 0;
    HI_U32 u32SecOutAddrPhy = 0;
    
	HI_SEC_MMZ_Init();
	
    stCipherAttr.enCipherType = HI_TEE_CIPHER_TYPE_NORMAL;

    for(i=0; i<TEST_CIPHER_CNT; i++)
    {
        printf("===================%d====================\r\n", i);

        s32Ret = TEE_CIPHER_CreateHandle(&hTestchnid[i], &stCipherAttr);
        if(0 != s32Ret)
    	{
    	    HI_ERR_CIPHER("Error: CreateHandle failed!\n");
    		continue;
    	}

        strncpy(stInBuf.bufname, "CIPHER_DATA_IN", MAX_BUFFER_NAME_SIZE);
        stInBuf.bufsize = datalen;
        s32Ret = HI_MPI_MMZ_Malloc(&stInBuf);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            return s32Ret;
        }
        printf("IN MMZ: 0x%x, VIA: %p\n", stInBuf.phyaddr, stInBuf.user_viraddr);

		u32SecOutAddrPhy = (HI_U32)HI_SEC_MMZ_New(TEST_DATA_KEY_LEN, SEC_MMZ_ZONE, "CIPHER_BufOut");
        if (0 == u32SecOutAddrPhy)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
            return s32Ret;
        }
		printf("SEC MMZ: 0x%x\n", u32SecOutAddrPhy);
        
    	/* For encrypt */
        s32Ret = Setconfiginfo(hTestchnid[i],
                                HI_FALSE,
                                HI_TEE_CIPHER_ALG_AES,
                                mode,
                                HI_TEE_CIPHER_KEY_AES_128BIT,
                                u8Key,
                                u8IV);
    	if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Set config info failed.\n");
    		goto __CIPHER_EXIT__;
    	}

        HI_PRINT("CIPHER_Encrypt...\n");
        usleep(100000);

        memcpy(stInBuf.user_viraddr, u8Src, TEST_DATA_KEY_LEN);
        s32Ret = TEE_CIPHER_Encrypt(hTestchnid[i], stInBuf.phyaddr, u32SecOutAddrPhy, datalen);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        usleep(100000);
        CIPHER_SEC_PHY_print(s_stTeecSession, "encrypt", u32SecOutAddrPhy, TEST_DATA_KEY_LEN, HI_FALSE);
        s32Ret = CIPHER_SEC_memcmp(s_stTeecSession, u32SecOutAddrPhy, u8Dst, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        
    	/* For decrypt */
    	s32Ret = Setconfiginfo(hTestchnid[i],
                                        HI_FALSE,
                                        HI_TEE_CIPHER_ALG_AES,
                                        mode,
                                        HI_TEE_CIPHER_KEY_AES_128BIT,
                                        u8Key,
                                        u8IV);
    	if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Set config info failed.\n");
    		goto __CIPHER_EXIT__;
    	}

        HI_PRINT("CIPHER_Decrypt...\n");
        usleep(100000);

        memcpy(stInBuf.user_viraddr, u8Dst, TEST_DATA_KEY_LEN);
        s32Ret = TEE_CIPHER_Decrypt(hTestchnid[i], stInBuf.phyaddr, u32SecOutAddrPhy, datalen);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher decrypt failed.\n");
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        usleep(100000);
        CIPHER_SEC_PHY_print(s_stTeecSession, "decrypt", u32SecOutAddrPhy, TEST_DATA_KEY_LEN, HI_FALSE);
        s32Ret = CIPHER_SEC_memcmp(s_stTeecSession, u32SecOutAddrPhy, u8Src, TEST_DATA_KEY_LEN);
        if(0 != s32Ret)
    	{
    		HI_ERR_CIPHER("Cipher decrypt failed. hTestchnid=0x%x\n", hTestchnid[i]);
    		s32Ret = -1;
    		goto __CIPHER_EXIT__;
    	}
        
        HI_PRINT("Please compare the result carefully!\r\n");

__CIPHER_EXIT__:

        HI_MPI_MMZ_Free(&stInBuf);
        HI_SEC_MMZ_Delete(u32SecOutAddrPhy);
    }

	HI_SEC_MMZ_DeInit();
	
    for(i=0; i<TEST_CIPHER_CNT; i++)
    {
        if (hTestchnid[i])
        {
            TEE_CIPHER_DestroyHandle(hTestchnid[i]);
        }
    }

    return s32Ret;
}
#endif

/* encrypt data using special chn*/
static int AES_CBC128(int argc, char* argv[])
{
    unsigned char u8Key[TEST_DATA_KEY_LEN] = {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char u8IV[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
    unsigned char u8Src[TEST_DATA_KEY_LEN] = {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char u8Dst[TEST_DATA_KEY_LEN] = {0x1e, 0x07, 0x63, 0x70, 0x20, 0x29, 0x01, 0xdf,0x6e, 0xc8, 0x4f, 0x5e, 0x42, 0x49, 0xb7, 0x2b};

    HI_PRINT("\nAES-128-CBC TEST\n");

    return AES(HI_TEE_CIPHER_WORK_MODE_CBC, u8Key, u8IV, u8Src, u8Dst);
}

/* encrypt data using special chn*/
static int AES_CTR128(int argc, char* argv[])
{
    unsigned char u8Key[16] = {"\x7E\x24\x06\x78\x17\xFA\xE0\xD7\x43\xD6\xCE\x1F\x32\x53\x91\x63"};
    unsigned char u8IV[16]  = {"\x00\x6C\xB6\xDB\xC0\x54\x3B\x59\xDA\x48\xD9\x0B\x00\x00\x00\x01"};
    unsigned char u8Src[32] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"};
    unsigned char u8Dst[32] = {"\x51\x04\xA1\x06\x16\x8A\x72\xD9\x79\x0D\x41\xEE\x8E\xDA\xD3\x88\xEB\x2E\x1E\xFC\x46\xDA\x57\xC8\xFC\xE6\x30\xDF\x91\x41\xBE\x28"};

    HI_PRINT("\nAES-128-CTR TEST\n");

	return AES(HI_TEE_CIPHER_WORK_MODE_CTR, u8Key, u8IV, u8Src, u8Dst);
}

/* encrypt data using special chn*/
static int AES_CFB128(int argc, char* argv[])
{
    unsigned char u8Key[16] = {"\x2B\x7E\x15\x16\x28\xAE\xD2\xA6\xAB\xF7\x15\x88\x09\xCF\x4F\x3C"};
    unsigned char u8IV[16]  = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};
    unsigned char u8Src[32] = {"\x6B\xC1\xBE\xE2\x2E\x40\x9F\x96\xE9\x3D\x7E\x11\x73\x93\x17\x2A\xAE\x2D\x8A\x57\x1E\x03\xAC\x9C\x9E\xB7\x6F\xAC\x45\xAF\x8E\x51"};
    unsigned char u8Dst[32] = {"\x3B\x3F\xD9\x2E\xB7\x2D\xAD\x20\x33\x34\x49\xF8\xE8\x3C\xFB\x4A\xC8\xA6\x45\x37\xA0\xB3\xA9\x3F\xCD\xE3\xCD\xAD\x9F\x1C\xE5\x8B"};

    HI_PRINT("\nAES-128-CFB TEST\n");

	return AES(HI_TEE_CIPHER_WORK_MODE_CFB, u8Key, u8IV, u8Src, u8Dst);
}

static int cipher_main(int argc, char* argv[])
{
    int  s32Ret;

    (void)HI_SYS_Init();

    s32Ret = TEE_CIPHER_Init();
    if(0 != s32Ret)
	{
        (void)HI_SYS_DeInit();
		return -1;
	}

    AES_CBC128(argc, argv);
    AES_CTR128(argc, argv);
    AES_CFB128(argc, argv);

    TEE_CIPHER_DeInit();
    (void)HI_SYS_DeInit();

    return 0;
}


/*##########################HASH Sample###############################*/
#if defined(CHIP_TYPE_hi3798cv200)
#define MAX_HASH_HANDLE 4
#else
#define MAX_HASH_HANDLE 1
#endif
#define LONG_DATA_SIZE 10000

static unsigned char sha1_buf[3][128] = {
    {"abc"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}
};
static const int sha1_buflen[4] ={3, 56, 112, 1000000};
static const unsigned char sha1_sum[5][20] =
{
    {0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D},
    {0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1},
    {0xaf, 0xc5, 0x3a, 0x4e, 0xa2, 0x08, 0x56, 0xf9, 0x8e, 0x08, 0xdc, 0x6f, 0x3a, 0x5c, 0x98, 0x33, 0x13, 0x77, 0x68, 0xed},
    {0x34, 0xaa, 0x97, 0x3c, 0xd4, 0xc4, 0xda, 0xa4, 0xf6, 0x1e, 0xeb, 0x2b, 0xdb, 0xad, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6f},
    {0x7d, 0xf9, 0x62, 0x1f, 0x17, 0xad, 0x18, 0xc5, 0x8a, 0x5a, 0xf7, 0x99, 0x1d, 0x12, 0x62, 0x20, 0x0f, 0xaf, 0xa9, 0x0f},
};

static const unsigned char sha2_sum[4][32] ={
    /** SHA-256 sample vectors*/
    {0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD},
    {0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1},
    {0x59, 0xf1, 0x09, 0xd9, 0x53, 0x3b, 0x2b, 0x70, 0xe7, 0xc3, 0xb8, 0x14, 0xa2, 0xbd, 0x21, 0x8f, 0x78, 0xea, 0x5d, 0x37, 0x14, 0x45, 0x5b, 0xc6, 0x79, 0x87, 0xcf, 0x0d, 0x66, 0x43, 0x99 ,0xcf},
    {0xcd, 0xc7, 0x6e, 0x5c, 0x99, 0x14, 0xfb, 0x92, 0x81, 0xa1, 0xc7, 0xe2, 0x84, 0xd7, 0x3e, 0x67, 0xf1, 0x80, 0x9a, 0x48, 0xa4, 0x97, 0x20, 0x0e, 0x04, 0x6d, 0x39, 0xcc, 0xc7, 0x11, 0x2c, 0xd0},
};

static unsigned char au8Buf[LONG_DATA_SIZE];

int SHA1()
{
	int ret = 0;
    unsigned char u8Hash[20];
    unsigned int i = 0,j = 0;
    HI_HANDLE hHandle[MAX_HASH_HANDLE];
    HI_TEE_CIPHER_HASH_ATTS_S stHashAttr;

	ret = TEE_CIPHER_Init();
	if ( 0 != ret )
	{
		return -1;
	}

    memset(u8Hash, 0, 20);

    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        memset(&stHashAttr, 0, sizeof(HI_TEE_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = HI_TEE_CIPHER_HASH_TYPE_SHA1;

        ret = TEE_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
		if ( 0 != ret )
		{
			TEE_CIPHER_DeInit();
			return -1;
		}
     }
     for(i = 0; i < MAX_HASH_HANDLE; i++)
     {
        if(i == 3)
        {
            memset(au8Buf, 'a', LONG_DATA_SIZE);
            for(j=0; j<1000000/LONG_DATA_SIZE; j++)
            {
                ret = TEE_CIPHER_HashUpdate(hHandle[i], au8Buf, LONG_DATA_SIZE);
        		if ( 0 != ret )
        		{   
        			TEE_CIPHER_DeInit();
        			return -1;
        		}
            }  
        }
        else
        {
            ret = TEE_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
    		if ( 0 != ret )
    		{
    			TEE_CIPHER_DeInit();
    			return -1;
    		}
        }
     }

     for(i = 0; i < MAX_HASH_HANDLE; i++)
     {
        ret = TEE_CIPHER_HashFinal(hHandle[i], u8Hash);
		if ( 0 != ret )
		{
			TEE_CIPHER_DeInit();
			return -1;
		}
        if(memcmp(u8Hash, sha1_sum[i], 20) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA1 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("Sha1 result:", u8Hash, 20);
            printBuffer("golden data:", sha1_sum[i], 20);
            TEE_CIPHER_DeInit();
            return -1;
        }
        printf("SHA1 run success, sample %d!\n", i);
     }
    
    HI_ERR_CIPHER("sample SHA1 run successfully!\n");
    TEE_CIPHER_DeInit();

    return 0;
}
int SHA256()
{
	int ret = 0;
    unsigned char u8Hash[32];
    unsigned int i = 0, j = 0;
    HI_HANDLE hHandle[MAX_HASH_HANDLE];
    HI_TEE_CIPHER_HASH_ATTS_S stHashAttr;

	ret = TEE_CIPHER_Init();
	if ( 0 != ret )
	{
		return -1;
	}

    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        memset(&stHashAttr, 0, sizeof(HI_TEE_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = HI_TEE_CIPHER_HASH_TYPE_SHA256;

        ret = TEE_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
		if ( 0 != ret )
		{
			TEE_CIPHER_DeInit();
			return -1;
		}
    }
    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        if(i == 3)
        {
            memset(au8Buf, 'a', LONG_DATA_SIZE);
            for(j=0; j<1000000/LONG_DATA_SIZE; j++)
            {
                ret = TEE_CIPHER_HashUpdate(hHandle[i], au8Buf, LONG_DATA_SIZE);
        		if ( 0 != ret )
        		{   
        			TEE_CIPHER_DeInit();
        			return -1;
        		}
            }  
        }
        else
        {
            ret = TEE_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
    		if ( 0 != ret )
    		{
    			TEE_CIPHER_DeInit();
    			return -1;
    		}
        }
    }
    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        ret = TEE_CIPHER_HashFinal(hHandle[i], u8Hash);
		if ( 0 != ret )
		{
			TEE_CIPHER_DeInit();
			return -1;
		}

        if(memcmp(u8Hash, sha2_sum[i], 32) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA256 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("Sha256 result:", u8Hash, 32);
            printBuffer("golden data:", sha2_sum[i], 20);
			TEE_CIPHER_DeInit();
            return -1;
        }
        printf("SHA256 run success, sample %d!\n", i);
    }

    HI_ERR_CIPHER("sample SHA256 run successfully!\n");
    TEE_CIPHER_DeInit();
    return 0;
}
#endif

/*##########################TEST##################################*/
typedef enum {
    CIPHER_TEST_PRINT_PHY = 0x01,
    CIPHER_TEST_PRINT_VIA,
    CIPHER_TEST_MEMSET,
    CIPHER_TEST_MEMCMP,
    CIPHER_TEST_MEMCPY,
    CIPHER_TEST_MEMCMP_PHY,
    CIPHER_TEST_READ_REG,
    CIPHER_TEST_WRITE_REG,
    CIPHER_TEST_AES  = 0x10,
    CIPHER_TEST_HMAC,
    CIPHER_TEST_RSA,
    CIPHER_TEST_HASH,
    CIPHER_TEST_DES,
    CIPHER_TEST_RSA_PRIM,
    CIPHER_TEST_RSA_KG,
    CIPHER_TEST_RND,  
    CIPHER_TEST_BUTT,
}CIPHER_TEST;

typedef struct hi_CIPHER_TEST_MEMCMP_S
{
    HI_U8 au8Data[256];
    HI_U8 u8Size;
    HI_U32 u32SecPhy;
}CIPHER_TEST_MEMCMP_S;

typedef struct hi_CIPHER_TEST_MEMSET_S
{
    HI_U32 u32SecPhy;
    HI_U32 u32Size;
    HI_U8 u8Val;
}CIPHER_TEST_MEMSET_S;

typedef struct hi_CIPHER_TEST_MEMCPY_S
{
    HI_U32 u32SecPhy;
    HI_U8 u8Size;
    HI_U8 au8Data[256];
}CIPHER_TEST_MEMCPY_S;

typedef struct hi_CIPHER_TEST_PRINT_S
{
    HI_U32 u32SecPhy;
    HI_U32 u32Size;
    HI_BOOL bIsSMMu;
    char uName[32];
}CIPHER_TEST_PRINT_S;

static int cipher_cmd(TEEC_Session session, unsigned int u32Cmd, void *pu8Param, unsigned int u32Size)
{
    TEEC_Operation stTeecOperation;
    TEEC_Result stTeecResult;

    stTeecOperation.started = 1;
    stTeecOperation.params[0].value.a = u32Cmd;

    if (u32Size > 0)
    {
        stTeecOperation.params[1].tmpref.buffer = pu8Param;
        stTeecOperation.params[1].tmpref.size= u32Size;

        stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    }
    else
    {
        stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                        TEEC_NONE,
                                        TEEC_NONE,
                                        TEEC_NONE);
    }
    stTeecResult = TEEC_InvokeCommand(&session, TEST_TZ_CIPHER_TEST, &stTeecOperation, NULL);
    if (stTeecResult != TEEC_SUCCESS)
	{
		HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", TEST_TZ_CIPHER_TEST);
		return -1;
	}
    
    memcpy(pu8Param,  stTeecOperation.params[1].tmpref.buffer, u32Size);

    return 0;
}

int CIPHER_SEC_VIA_print(TEEC_Session session, const char *name, unsigned int u32SecVia, unsigned int u32Size)
{
    int  ret;
    CIPHER_TEST_PRINT_S print;

 //   usleep(10000);
    print.u32SecPhy = u32SecVia;
    print.u32Size = u32Size;
    strncpy(print.uName, name, sizeof(print.uName));

    ret = cipher_cmd(session, CIPHER_TEST_PRINT_VIA, &print, sizeof(CIPHER_TEST_PRINT_S));

    return ret;
}

int CIPHER_SEC_PHY_print(TEEC_Session session, const char *name, unsigned int u32Pha, unsigned int u32Size, HI_BOOL bIsSMMu)
{
    CIPHER_TEST_PRINT_S print;

    print.u32SecPhy = u32Pha;
    print.u32Size = u32Size;
    print.bIsSMMu = bIsSMMu;
    strncpy(print.uName, name, sizeof(print.uName));

    return cipher_cmd(session, CIPHER_TEST_PRINT_PHY, &print, sizeof(CIPHER_TEST_PRINT_S));
}

int CIPHER_SEC_memset(TEEC_Session session, unsigned int u32SecVia, unsigned char val, unsigned int u32Size)
{
    CIPHER_TEST_MEMSET_S set;

    set.u32SecPhy = u32SecVia;
    set.u8Val = val;
    set.u32Size = u32Size;

    return cipher_cmd(session, CIPHER_TEST_MEMSET, &set, sizeof(CIPHER_TEST_MEMSET_S));
}

int CIPHER_SEC_memcmp(TEEC_Session session, unsigned int u32SecPhy, const unsigned char *pu8Check, unsigned char u8Size)
{
    CIPHER_TEST_MEMCMP_S cmp;

    cmp.u32SecPhy = u32SecPhy;
    cmp.u8Size = u8Size;
    memcpy(cmp.au8Data, pu8Check, u8Size);

    return cipher_cmd(session, CIPHER_TEST_MEMCMP, &cmp, sizeof(CIPHER_TEST_MEMCMP_S));
}

int CIPHER_SEC_memcpy(TEEC_Session session, unsigned int u32SecPhy, const unsigned char *pu8Data, unsigned char u8Size)
{
    CIPHER_TEST_MEMCPY_S cpy;

    cpy.u32SecPhy = u32SecPhy;
    cpy.u8Size = u8Size;
    memcpy(cpy.au8Data, pu8Data, u8Size);

    return cipher_cmd(session, CIPHER_TEST_MEMCMP, &cpy, sizeof(CIPHER_TEST_MEMCMP_S));
}

int CIPHER_SEC_REG_read(TEEC_Session session, unsigned int u32RegAddr, unsigned int u32Size)
{
    CIPHER_TEST_PRINT_S print;

    print.u32SecPhy = u32RegAddr;
    print.u32Size = u32Size;

    return cipher_cmd(session, CIPHER_TEST_READ_REG, &print, sizeof(CIPHER_TEST_PRINT_S));
}

int CIPHER_SEC_REG_write(TEEC_Session session, unsigned int u32RegAddr, unsigned int u32Val)
{
    CIPHER_TEST_MEMSET_S set;

    set.u32SecPhy = u32RegAddr;
    set.u8Val = u32Val;
    set.u32Size = 1;
    
    return cipher_cmd(session, CIPHER_TEST_WRITE_REG, &set, sizeof(CIPHER_TEST_MEMSET_S));
}

static int test_main(int argc,char *argv[])
{
    TEEC_Context context;
    TEEC_Session session;
    unsigned int result;
    unsigned int mode = 0;
    unsigned int u32Addr, u32Val = 0;
    HI_U32 i;
	TEEC_Operation sess_op;

    if (argc <= 1)
    {
        printf("Usage: teec_cipher mode \n"
               "       mode: 0--AES Test\n"
               "             1--HAMC Test\n"
               "             2--RSA Test\n"
               "             3--HASH Test\n"
               "             4--DES Test\n"
               "             5--RSA PRIM Test\n"
               "             6--RSA KG Test\n"
               "             7--RND Test\n"						
               "             8 Read Secure Register\n"
			   "             9 Write Secure Register\n"
			   "             10 RNG Test\n"
			   "             11 Read Phy, 1:MMU 0:MMZ\n"
			   "             12 Read VIA\n");
        
        printf("Example:./teec_cipher 0\n");
        printf("Example:./teec_cipher 5 0\n");
        printf("Example:./teec_cipher 5 8 0xf9a01200\n");
        printf("Example:./teec_cipher 5 9 0xf9a01200 0xff\n");
        
        return -1;
    }

    printf("argc = %d\n",argc);
    
    if (argc >= 2)
    {
        mode = strtol(argv[1],NULL,0);
        printf("mode = %d\n",mode);
    }

    usleep(100*1000);

    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        printf("teec initial failed\n");
        goto cleanup_1;
    }

	memset(&sess_op, 0, sizeof(TEEC_Operation));
	sess_op.started = 1;
	sess_op.paramTypes = TEEC_PARAM_TYPES(
						TEEC_NONE,
						TEEC_NONE,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_INPUT);
    result = TEEC_OpenSession(&context, &session, &s_stTeecCipherUUID,
            TEEC_LOGIN_IDENTIFY, NULL, &sess_op, NULL);
    if(result != TEEC_SUCCESS) {
        printf("teec open session failed\n");
        goto cleanup_2;
    }

    switch(mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        cipher_cmd(session, mode + CIPHER_TEST_AES, 0, 0);
        break;
    case 8:
        if (argc >= 3)
        {
            u32Addr = strtoul(argv[2], NULL, 16);
            CIPHER_SEC_REG_read(session, u32Addr, 64);
        }
        else
        {
            printf("Error, Invalid arg, argc = 0x%x, mode = 0x%x\n", argc, mode);
            printf("./tee_test_cipher 5 8 0xf9a00000\n");
        }
        break;
    case 9:
        if (argc >= 4)
        {
            u32Addr = strtoul(argv[2],NULL, 16);
            u32Val= strtoul(argv[3],NULL, 16);
            CIPHER_SEC_REG_write(session, u32Addr, u32Val);
        } 
        else
        {
            printf("Error, Invalid arg, argc = 0x%x, mode = 0x%x\n", argc, mode);
            printf("./tee_test_cipher 5 9 0xf9a00000 0x01\n");
        }
        break;
    case 10:
#ifdef HI_TEE_SUPPORT
        TEE_CIPHER_Init();
        for(i=0; i<32; i++)
        {
            TEE_CIPHER_GetRandomNumber(&u32Val);
            printf("RNG: 0x%x\n", u32Val);
        }
        TEE_CIPHER_DeInit();
#endif
        break;
    case 11:
        if (argc >= 4)
        {
            u32Addr = strtoul(argv[2], NULL, 16);
            u32Val= strtoul(argv[3],NULL, 10);
            CIPHER_SEC_PHY_print(session, "MEM", u32Addr, 256, u32Val);
        }
        else
        {
            printf("Error, Invalid arg, argc = 0x%x, mode = 0x%x\n", argc, mode);
            printf("./tee_test_cipher 5 11 0x7d0000000 0  #mmz\n");
            printf("./tee_test_cipher 5 11 0x800000000 1  #mmu\n");
        }
        break;
    case 12:
        if (argc >= 3)
        {
            u32Addr = strtoul(argv[2], NULL, 16);
            CIPHER_SEC_VIA_print(session, "MEM", u32Addr, 256);
        }
        else
        {
            printf("Error, Invalid arg, argc = 0x%x, mode = 0x%x\n", argc, mode);
            printf("./tee_test_cipher 5 12 0xa0100800\n");
        }
        break;
    default:
        printf("Error, Invalid command, argc = 0x%x, mode = 0x%x\n", argc, mode);
        break;
    }

    printf("result: 0x%x\n", result);

    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return 0;
}

/////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    int s32Ret = 0;

#ifdef HI_TEE_SUPPORT
    unsigned int funcNumber = 0;

    (void)HI_SYS_Init();

    if ( 2 > argc )
    {
        HI_PRINT("Usage: %s funcNumber\n\n", argv[0]);
        HI_PRINT("#########cipher sample##########\n"
                        "[0] SHA1\n"
                        "[1] SHA256\n"
						"[3] Cipher\n"
						"[5] Test\n");
	    (void)HI_SYS_DeInit();
        return 0;
    }
    funcNumber = strtoul(argv[1],NULL, 0);

	if ( 0 == funcNumber )
    {
        s32Ret = SHA1();
    }
    else if ( 1 == funcNumber )
    {
        s32Ret = SHA256();
    }
	else if ( 3 == funcNumber )
    {
        s32Ret = cipher_main(argc, argv);
    }
	else if ( 5 == funcNumber )
    {
        s32Ret = test_main(argc - 1, argv+1);
    }
    else
    {
        HI_INFO_CIPHER("funcNumber %d do not exist!\n",funcNumber);
    }

    (void)HI_SYS_DeInit();

    if(0 != s32Ret)
    {
        HI_INFO_CIPHER("funcNumber %d run failure.\n",funcNumber);
        return -1;
    }
#else
    s32Ret = test_main(argc, argv);
#endif

    return s32Ret;
}
