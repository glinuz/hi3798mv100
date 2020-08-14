#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "hi_unf_hdcp.h"

#define ENCRYPT_HDCP_KEY_SIZE 332

#define HI_HDCPKEY_EQUAL(api,rtn) \
do \
{ \
    s32Ret = (api); \
    if((rtn) != s32Ret) \
    { \
        goto ___EXIT; \
    } \
}while(0)




#define  HDCP_KEY_RAM_SIZE                      320

#define  HDCP_KEY_PRIME_SIZE                    320
#define  HDCP_KEY_TOOL_FILE_SIZE                384
#define  HDCP_KEY_CHIP_FILE_SIZE               (HDCP_KEY_RAM_SIZE + 12)

static HI_S32 CopyFromFileToBuffer(HI_CHAR *pFileName, HI_U8 *pu8Buffer)
{
    FILE *pBinFile;
    HI_U32 binfilelen;
    HI_U32 i = 0;

    if ( (NULL == pFileName) || (NULL == pu8Buffer) )
    {
        printf("Error! NULL Pointer Input!\n");
        return HI_FAILURE;
    }

    pBinFile = fopen(pFileName, "rb");

    fseek(pBinFile, 0, SEEK_END);
    binfilelen = ftell(pBinFile);
    printf("binfilelen=%d\n",binfilelen);
    fseek(pBinFile, 0, SEEK_SET);

    i = fread(pu8Buffer, 1, binfilelen, pBinFile);
    if(i != binfilelen)
    {
        printf("ERROR:FUNC- %sLINE-%d!\n", __FUNCTION__, __LINE__);
        fclose(pBinFile);
        return -1;
    }
    fclose(pBinFile);

    return HI_SUCCESS;
}

static HI_S32 CopyFromBufferToFile(HI_U8 *pu8Input, HI_U32 u32Length, HI_CHAR *pFileName)
{
    FILE *phFdEn = NULL;
    HI_S32 s32Ret = HI_SUCCESS;

    if ( (NULL == pFileName) || (NULL == pu8Input) )
    {
        printf("NULL pointer input ! error!\n");
        return  HI_FAILURE;
    }

    phFdEn = fopen(pFileName, "w+");
    if( NULL == phFdEn )
    {
         printf("Failed to fopen file %s.\n", pFileName);
         return HI_FAILURE;
    }

    printf("fopen FILE [%s] success.\n", pFileName);

    s32Ret = fwrite((HI_VOID *)pu8Input, (size_t)1, (size_t)u32Length, phFdEn);
    printf("fwrite length, s32ret = 0x%x\n", s32Ret);
    if( u32Length != s32Ret)
    {
        printf("fwrite to encrypt file failed.\n");
        fclose(phFdEn);
        return HI_FAILURE;
    }

    fclose(phFdEn);
    return HI_SUCCESS;
}

HI_S32 HI_PrintBuf(HI_CHAR *pString, HI_U8 *pu8Buf, HI_U32 u32Len)
{
    HI_U32 i;

    if ( (NULL == pu8Buf) || (NULL == pString) )
    {
        printf("null pointer input in function print_buf!\n");
        return HI_FAILURE;
    }

    printf("%s:\n    ", pString);
    for ( i = 0 ; i < u32Len ; i++ )
    {
        if ( ( i != 0) && ( (i% 16) == 0) )
        {
            printf("\n    ");
        }
        printf("%02x ", pu8Buf[i]);
    }
    printf("\n");

    return HI_SUCCESS;
}

typedef struct{
    HI_U32 num;
    HI_S32 (*pfunc)(HI_CHAR *pString);
}SAMPLE_TEST_STRUCTURE;



//###########################HDCP KEY TEST#############################
/* Encrypt HDCP Key using OTP Root Key */
HI_S32 sample_encryptHdcpKey_usingOtpRootkEY(HI_CHAR *pString)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_HDCP_HDCPKEY_S stHdcpKey;
    HI_U8 u8InEncryptKey[HDCP_KEY_TOOL_FILE_SIZE];
    HI_U8 u8OutEncryptKey[HDCP_KEY_CHIP_FILE_SIZE];

    if ( NULL == pString )
    {
        printf("NULL Pointer Input!ERROR!\n");
        return HI_FAILURE;
    }

    memset(u8InEncryptKey, 0x00, HDCP_KEY_TOOL_FILE_SIZE);
    memset(u8OutEncryptKey, 0x00, HDCP_KEY_CHIP_FILE_SIZE);
    memset(&stHdcpKey, 0x00, sizeof(HI_UNF_HDCP_HDCPKEY_S));

    HI_SYS_Init();
    HI_UNF_CIPHER_Init();

    stHdcpKey.EncryptionFlag = HI_TRUE;
    stHdcpKey.Reserved = 0;

    HI_HDCPKEY_EQUAL(CopyFromFileToBuffer(pString, u8InEncryptKey), HI_SUCCESS);
    HI_HDCPKEY_EQUAL(CopyFromBufferToFile(u8InEncryptKey + 48, 320, "input_320_of_384.bin"), HI_SUCCESS);
    memcpy(stHdcpKey.key.EncryptData.u8EncryptKey, u8InEncryptKey, HDCP_KEY_TOOL_FILE_SIZE);
    HI_HDCPKEY_EQUAL(HI_UNF_HDCP_EncryptHDCPKey(stHdcpKey, HI_TRUE, u8OutEncryptKey), HI_SUCCESS);
    HI_PrintBuf("Encrypted hdcp key [length 332 bytes]", u8OutEncryptKey, HDCP_KEY_CHIP_FILE_SIZE);

    HI_HDCPKEY_EQUAL(CopyFromBufferToFile(u8OutEncryptKey, HDCP_KEY_CHIP_FILE_SIZE, "EncryptedKey_332bytes.bin"), HI_SUCCESS);

___EXIT:
    if ( HI_SUCCESS != s32Ret )
    {
        printf("Run error.\n");
    }
    HI_UNF_CIPHER_DeInit();
    HI_SYS_DeInit();
    return s32Ret;
}

/* Encrypt HDCP Key using key defined by Hisilicon */
HI_S32 sample_encryptHdcpKey_usingKeyDefinedByHisilicon(HI_CHAR *pString)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_HDCP_HDCPKEY_S stHdcpKey;
    HI_U8 u8InEncryptKey[HDCP_KEY_TOOL_FILE_SIZE];
    HI_U8 u8OutEncryptKey[HDCP_KEY_CHIP_FILE_SIZE];

    if ( NULL == pString )
    {
        printf("NULL Pointer Input!ERROR!\n");
        return HI_FAILURE;
    }

    memset(u8InEncryptKey, 0x00, HDCP_KEY_TOOL_FILE_SIZE);
    memset(u8OutEncryptKey, 0x00, HDCP_KEY_CHIP_FILE_SIZE);
    memset(&stHdcpKey, 0x00, sizeof(HI_UNF_HDCP_HDCPKEY_S));

    HI_SYS_Init();
    HI_UNF_CIPHER_Init();

    stHdcpKey.EncryptionFlag = HI_TRUE;
    stHdcpKey.Reserved = 0;

    HI_HDCPKEY_EQUAL(CopyFromFileToBuffer(pString, u8InEncryptKey), HI_SUCCESS);
    HI_HDCPKEY_EQUAL(CopyFromBufferToFile(u8InEncryptKey + 48, 320, "input_320_of_384.bin"), HI_SUCCESS);
    memcpy(stHdcpKey.key.EncryptData.u8EncryptKey, u8InEncryptKey, HDCP_KEY_TOOL_FILE_SIZE);
    HI_HDCPKEY_EQUAL(HI_UNF_HDCP_EncryptHDCPKey(stHdcpKey, HI_FALSE, u8OutEncryptKey), HI_SUCCESS);
    HI_PrintBuf("Encrypted hdcp key [length 332 bytes]", u8OutEncryptKey, HDCP_KEY_CHIP_FILE_SIZE);
    HI_HDCPKEY_EQUAL(CopyFromBufferToFile(u8OutEncryptKey, HDCP_KEY_CHIP_FILE_SIZE, "EncryptedKey_332bytes.bin"), HI_SUCCESS);

___EXIT:
    if ( HI_SUCCESS != s32Ret )
    {
        printf("Run error.\n");
    }
    HI_UNF_CIPHER_DeInit();
    HI_SYS_DeInit();
    return s32Ret;
}


static const SAMPLE_TEST_STRUCTURE func_list[]={
    {1, sample_encryptHdcpKey_usingOtpRootkEY},
    {2, sample_encryptHdcpKey_usingKeyDefinedByHisilicon},
};


HI_S32 main(int argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 num, i;
    HI_U32 total;

    if ( argc != 3)
    {
        printf("Usage: %s num inputFilename\n\n", argv[0]);

        printf("#########HDCP SAMPLE##########\n"
        "[1] Encrypt HDCP Key using OTP Root key\n"
        "[2] Encrypt HDCP Key using key defined by Hisilicon\n" );
        return HI_SUCCESS;
    }

    num = strtol(argv[1],NULL,0);

    total = sizeof(func_list)/sizeof(SAMPLE_TEST_STRUCTURE);
    for(i = 0; i < total; i++)
    {
        if(func_list[i].num == num )
            break;
    }

    if(i >= total)
    {
        printf("The test case %d is not exist.\n", num);
        return (0);
    }

    s32Ret = func_list[i].pfunc(argv[2]);
    if(HI_SUCCESS != s32Ret)
    {
        printf("Execute sample %d failure.\n",num);
        return HI_FAILURE;
    }
    else
    {
        printf("Success!\n");
    }

    return HI_SUCCESS;
}

