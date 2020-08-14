/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : unf_advca.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2013-08-28
Last Modified :
Description   : Hisilicon CA API definition
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <limits.h>
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "hi_drv_struct.h"
#include "hi_error_mpi.h"
#include "hi_drv_advca.h"
#include "drv_advca_ioctl.h"
#include "drv_otp_ioctl.h"
#include "drv_otp_ext.h"
#ifdef HI_PVR_SUPPORT
#include "hi_unf_pvr.h"
#endif
#include "hi_mpi_cipher.h"

#define INVALID_VALUE (0xffffffff)

/* for CodeCC, to limit the maxmun value of FPNum, Suppose: 3 FPNums comes per 20 seconds */
#define MAX_FP_NUM  (3*60*24*30)

static HI_S32 g_s32CaFd = -1;
static HI_S32 g_AdvcaInitCounter = -1;

static HI_UNF_ADVCA_KEYLADDER_LEV_E g_dvbLadder;
static HI_UNF_ADVCA_KEYLADDER_LEV_E g_r2rLadder;
static HI_UNF_ADVCA_VENDORID_E g_vendorType;

static HI_HANDLE  g_hCipher ;
static HI_BOOL g_bCryptologyConfig = HI_FALSE;

typedef struct
{
    HI_UNF_ADVCA_ALG_TYPE_E KeyladderAlg;
    HI_UNF_ADVCA_KEYLADDER_LEV_E KeyladderLevel;
    HI_U8 SessionKey1[16];
    HI_U8 SessionKey2[16];
    HI_HANDLE hCipherHandle ;
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
}HI_UNF_R2RKeyladder_Cipher_Info_S;

/*for PVR*/
#ifdef HI_PVR_SUPPORT
#define PVR_CA_MAX_CHANNEL 5
#define MAX_PVR_KEY_BLOCK_NUMBERS 5000

#define PVR_CA_VERIFY_LEN_DELTA 0x1400000   /*20M*/
//static HI_U64 PVR_BLOCK_SIZE = 0x100000000;//4G
static HI_U64 PVR_BLOCK_SIZE =  0x40000000; //1G

#define CA_PVR_MAGINNUMBER "HISI_ADVCA_PVR\0"

static sem_t g_PVRSem;

static sem_t g_PvrRecPlylock;
#define ADVCA_PVR_LOCK_INIT()  ((HI_VOID)sem_init(&g_PvrRecPlylock, 0, 1))
#define ADVCA_PVR_LOCK()       ((HI_VOID)sem_wait(&g_PvrRecPlylock))
#define ADVCA_PVR_UNLOCK()     ((HI_VOID)sem_post(&g_PvrRecPlylock))

typedef struct
{
    HI_U8 MagicNum[32];
    HI_UNF_ADVCA_Time_S Starttime;
    HI_U32 URINum;
    HI_U32 FPNum;
    HI_U32  u32UriStartIndex;   /*For rewind model, URI[] will be used as loop queue*/
    HI_UNF_PVR_URI_S URI[MAX_URI_NUM];        /* Maturity of this recorded stream*/
    HI_U8 MacKey[16];
} ADVCA_PVR_CA_PrivateFileHead_S;

typedef struct
{
    HI_U8 Mac[16];
    ADVCA_PVR_CA_PrivateFileHead_S stCAPrivateFileHead;
} ADVCA_PVR_CA_MacPrivateFileHead_S;

typedef struct
{
    HI_UNF_PVR_FP_INFO_S stPVRFPInfo;
    HI_U8 Mac[16];
} ADVCA_PVR_MAC_FP_INFO_S;

typedef struct
{
    HI_U32  u32BlockNum; /*Block key index, begin from 0*/
    HI_U32  u32Reserved;
    HI_U64  u64OffsetForKey;
    HI_U8   Cipherkey[16];
} HI_CA_PVR_KEY_INFO_S;

typedef struct
{
    HI_CHAR FileName[256];
    HI_CHAR CAPrivateFileName[128];
    HI_U64  u64CxVerifiedLen;
    HI_U32  u32TotalBlockNum;
    HI_UNF_R2RKeyladder_Cipher_Info_S stR2RkeyladderCipherInfo;
} HI_CA_PVR_CADATA_S;

typedef struct
{
    HI_U8 u8AES_CMAC[16];     /*The MAC of upper parameter of HI_CA_PVR_CADATA_S, use R2R-SWPK key-ladder */
    HI_CA_PVR_CADATA_S stPvrCaData;
} HI_CA_PVR_MAC_CADATA_S;

typedef struct
{
    HI_U32  u32ChnID;
    HI_BOOL bRewind;                /*Is rewind model?*/
    HI_BOOL bCADataSavedFlag;
    HI_BOOL bCAPrivateFileCreated;
    HI_U32  CurBlockNum;
    HI_U8   PvrRecTrackKey[16];
    HI_U64  PvrRecVerifyLenAcc;
    HI_U64  PvrRecBlockDataAcc;
    HI_BOOL PvrRecForceChangeKey;
    HI_U32  u32UriNumbers;
    HI_U32  u32FpNumbers;
    HI_CA_PVR_CADATA_S stPvrRecCaData;
} HI_CA_PVR_RECORD_CHANNEL_INFO_S;

typedef struct
{
    HI_U32  u32ChnID;
    HI_BOOL bIsTimeshiftPlay;
    HI_U32  u32RecChnID;      /*only valid when bIsTimeshiftPlay is true*/
    HI_BOOL bAlreadyGetPlayInfo;
    HI_U32  CurBlockNum;
    HI_U8   PvrPlayTrackKey[16];
    HI_U8   *pu8KeyBuffer;
    HI_CA_PVR_CADATA_S stPvrPlyCaData;
} HI_CA_PVR_PLAY_CHANNEL_INFO_S;

HI_CA_PVR_RECORD_CHANNEL_INFO_S PvrRecChannelInfo[PVR_CA_MAX_CHANNEL];
HI_CA_PVR_PLAY_CHANNEL_INFO_S   PvrPlayChannelInfo[PVR_CA_MAX_CHANNEL];
#endif

#define CA_CheckPointer(p) \
    do {  \
        if (HI_NULL == p)\
        {\
            HI_ERR_CA("pointer parameter is NULL.\n"); \
            return -1; \
        } \
    } while (0)

#define CA_ASSERT(api, ret) \
    do{ \
        ret = api; \
        if (ret != HI_SUCCESS) \
        { \
            HI_ERR_CA("run %s failed, ERRNO:%#x.\n", #api, ret); \
            return ret;\
        } \
        else\
        {\
        /*HI_INFO_CA("sample %s: run %s ok.\n", __FUNCTION__, #api);}*/   \
        }\
    }while(0)

extern HI_S32 HI_MPI_CIPHER_CbcMac_Auth(HI_U8 *pu8RefCbcMac, HI_U32 u32AppLen);

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_Init
*  Description:    open ca
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_Init(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 s32DevFd = 0;
    HI_U32 u32VendorType = 0;
#ifdef HI_PVR_SUPPORT
    HI_U8 i = 0;
#endif

    if (g_AdvcaInitCounter > 0)
    {
        g_AdvcaInitCounter++;
        return HI_SUCCESS;
    }

    s32DevFd = open("/dev/" UMAP_DEVNAME_CA, O_RDWR, 0);
    if (s32DevFd < 0)
    {
        HI_ERR_CA("ca open err. \n");
        return HI_ERR_CA_OPEN_ERR;
    }
    g_AdvcaInitCounter = 1;

    g_s32CaFd   = s32DevFd;
    ret = HI_UNF_ADVCA_GetDVBKeyLadderStage(&g_dvbLadder);
    ret |= HI_UNF_ADVCA_GetR2RKeyLadderStage(&g_r2rLadder);
    ret |= HI_UNF_ADVCA_GetVendorID(&u32VendorType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("get vendor type err. \n");
        (HI_VOID)HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

    g_vendorType = (HI_UNF_ADVCA_VENDORID_E)u32VendorType;
#ifdef HI_PVR_SUPPORT
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        memset(&PvrRecChannelInfo[i], 0, sizeof(HI_CA_PVR_RECORD_CHANNEL_INFO_S));
        memset(&PvrPlayChannelInfo[i], 0, sizeof(HI_CA_PVR_PLAY_CHANNEL_INFO_S));
        PvrRecChannelInfo[i].u32ChnID  = INVALID_VALUE;
        PvrPlayChannelInfo[i].u32ChnID  = INVALID_VALUE;
    }

    (HI_VOID)sem_init(&g_PVRSem, 0, 1);
    ADVCA_PVR_LOCK_INIT();
#endif

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DeInit
*  Description:    close ca
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:      ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DeInit(HI_VOID)
{
    if(g_AdvcaInitCounter > 0)
    {
        g_AdvcaInitCounter--;
    }

    if(g_AdvcaInitCounter != 0)
    {
        return HI_SUCCESS;
    }

    close(g_s32CaFd);
    g_s32CaFd   = -1;
    g_AdvcaInitCounter = -1;

    g_dvbLadder = HI_UNF_ADVCA_KEYLADDER_BUTT;
    g_r2rLadder = HI_UNF_ADVCA_KEYLADDER_BUTT;
    g_vendorType = HI_UNF_ADVCA_NULL;

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetChipId
*  Description:    get chip id
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetChipId(HI_U32 *pu32ChipId)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pu32ChipId)
    {
        HI_ERR_CA("pu32ChipId == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_CHIPID, pu32ChipId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_CHIPID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetMarketId
*  Description:    get market id
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetMarketId(HI_U8 u8MarketId[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8MarketId)
    {
        HI_ERR_CA("pu32MarketId == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_MARKETID, u8MarketId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_MARKETID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetMarketId
*  Description:    set market id
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetMarketId(HI_U8 u8MarketId[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }
    if (HI_NULL == u8MarketId)
    {
        HI_ERR_CA("pu32MarketId == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_MARKETID, u8MarketId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_MARKETID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetStbSn
*  Description:    get stb serial number
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetStbSn(HI_U8 u8StbSn[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8StbSn)
    {
        HI_ERR_CA("u8StbSn == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_STBSN, u8StbSn);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_STBSN err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetSerialNumber
*  Description:    set stb serial number
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetStbSn(HI_U8 u8StbSn[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8StbSn)
    {
        HI_ERR_CA("u8StbSn == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    ret = ioctl(g_s32CaFd, CMD_CA_SET_STBSN, u8StbSn);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_STBSN err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetVersionId
*  Description:    get version id
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetVersionId(HI_U8 u8VersionId[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8VersionId)
    {
        HI_ERR_CA("u8VersionId == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_VERSIONID, u8VersionId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_VERSIONID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetVersionId
*  Description:    set version id
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetVersionId(HI_U8 u8VersionId[4])
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8VersionId)
    {
        HI_ERR_CA("u8VersionId == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_VERSIONID, u8VersionId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_VERSIONID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_LockHardCwSel
*  Description:    Set hardware CW word only. Default, it depend on the configure of Demux.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_LockHardCwSel(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_LOCKHARDCWSEL,0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_LOCKHARDCWSEL err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_LockBootDecEn
*  Description:    Enable bootloader decryption
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_LockBootDecEn(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_LOCKBOOTDECEN, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_LOCKBOOTDECEN err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetBootDecEnStat
*  Description:    Get the decrypt flag of bootloader.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetBootDecEnStat(HI_U32 *pu32Stat)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pu32Stat)
    {
        HI_ERR_CA("pu32Stat == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_BLOAD_DEC_EN, pu32Stat);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_BLOAD_DEC_EN err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_LockR2RHardKey
*  Description:    Set to use the hardware key as R2R key.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_LockR2RHardKey(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_LOCKR2RHARDKEY,0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_LOCKR2RHARDKEY err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetHardCwSelStat
*  Description:    Check whether we use a static hardware CW word or not.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetHardCwSelStat(HI_BOOL *pbLock)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pbLock)
    {
        HI_ERR_CA("pbLock == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_STATHARDCWSEL,pbLock);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_STATHARDCWSEL err. \n");
        return ret;
    }

    return HI_SUCCESS;
}


/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetR2RHardKeyStat
*  Description:    To check the R2R Key is Hardware Key or not.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetR2RHardKeyStat(HI_BOOL *pbLock)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pbLock)
    {
        HI_ERR_CA("pbLock == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_STATR2RHARDKEY,pbLock);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_STATR2RHARDKEY err. \n");
        return ret;
    }

    return HI_SUCCESS;

}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DisableLinkProtection
*  Description:    Disable link protection
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DisableLinkProtection(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DISABLELPT,0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DISABLELP err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DisableSelfBoot
*  Description:    Disable self boot.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DisableSelfBoot(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DISABLESELFBOOT,0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DISABLESELFBOOT err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetSelfBootStat
*  Description:    Get the status of self boot, if it is close or not.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetSelfBootStat(HI_BOOL *pbDisable)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pbDisable)
    {
        HI_ERR_CA("pbDisable == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_SELFBOOT, pbDisable);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_SELFBOOT err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetSecBootStat
*  Description:    Get the status of secure boot, if it is enabled or not.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetSecBootStat(HI_BOOL *pbEnable,HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType)
{
    HI_S32 ret;
    HI_U32 val;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pbEnable)
    {
        HI_ERR_CA("pbEnable == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if (HI_NULL == penFlashType)
    {
        HI_ERR_CA("penFlashType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /**Get the status of secure boot, if it is enabled or not. CNcomment:获取安全启动使能状态 CNend */
    ret = ioctl(g_s32CaFd, CMD_CA_GET_SCSACTIVE, &val);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_SCSACTIVE err. \n");
        return ret;
    }

    if (val)
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    /**Get boot flash selection control flag. CNcomment:获取flash启动类型控制标志 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_GET_BOOTSEL_CTRL, &val);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_BOOTSEL_CTRL err. \n");
        return ret;
    }

    if(0 == val)
    {
        *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
        HI_INFO_CA("Boot mode is controlled by chipset pin. \n");
        return HI_SUCCESS;
    }

    /**Get the startup flash type. CNcomment:获取安全启动的Flash类型 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_GET_BOOTMODE, penFlashType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_BOOTMODE err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_EnableSecBoot
*  Description:    Enable secure boot and set the startup flash type.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_EnableSecBoot(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType)
{
    HI_S32 ret;
    HI_U32 u32BootselCtrl = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enFlashType >= HI_UNF_ADVCA_FLASH_TYPE_BUTT)
    {
        HI_ERR_CA("enFlashType >= HI_UNF_ADVCA_FLASH_TYPE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if(enFlashType == HI_UNF_ADVCA_FLASH_TYPE_NOR)
    {
        HI_ERR_CA("enFlashType set to HI_UNF_ADVCA_FLASH_TYPE_NOR, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /**Enable secure boot. CNcomment:使能安全启动 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_SET_SCSACTIVE,0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_SCSACTIVE err. \n");
        return ret;
    }

    /**Set the startup flash type.  CNcomment:设置安全启动的Flash类型 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_SET_BOOTMODE, &enFlashType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_BOOTMODE err. \n");
        return ret;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_BOOTSEL_CTRL, &u32BootselCtrl);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_BOOTSEL_CTRL err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_EnableSecBootEx(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

     /**Enable secure boot. CNcomment:使能安全启动 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_SET_SCSACTIVE, 0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_SCSACTIVE err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetFlashTypeEx(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType)
{
    HI_S32 ret;
    HI_U32 u32BootselCtrl = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enFlashType >= HI_UNF_ADVCA_FLASH_TYPE_BUTT)
    {
        HI_ERR_CA("enFlashType >=HI_UNF_ADVCA_FLASH_TYPE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if(enFlashType == HI_UNF_ADVCA_FLASH_TYPE_NOR)
    {
        HI_ERR_CA("enFlashType set to HI_UNF_ADVCA_FLASH_TYPE_NOR, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

     /**Set the startup flash type.  CNcomment:设置安全启动的Flash类型 CNend*/
    ret = ioctl(g_s32CaFd, CMD_CA_SET_BOOTMODE, &enFlashType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_BOOTMODE err. \n");
        return ret;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_BOOTSEL_CTRL, &u32BootselCtrl);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_BOOTSEL_CTRL err.\n");
        return ret;
    }

    return HI_SUCCESS;
}


/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetJtagMode
*  Description:    get jtag mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetJtagMode(HI_UNF_ADVCA_JTAG_MODE_E *penJtagMode)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (penJtagMode == NULL)
    {
        HI_ERR_CA("penJtagMode == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_JTAGPRTMODE, penJtagMode);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_JTAGPRTMODE err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetJtagMode
*  Description:    set jtag mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetJtagMode(HI_UNF_ADVCA_JTAG_MODE_E enJtagMode)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enJtagMode >= HI_UNF_ADVCA_JTAG_MODE_BUTT)
    {
        HI_ERR_CA("enJtagMode >=HI_UNF_ADVCA_JTAG_MODE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_JTAGPRTMODE, &enJtagMode);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_JTAGPRTMODE err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetR2RKeyLadderStage
*  Description:    get r2r ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (penStage == NULL)
    {
        HI_ERR_CA("penStage == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_R2R_GETLADDER, penStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_GETLADDER err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetR2RKeyLadderStage
*  Description:    set r2r ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((enStage >= HI_UNF_ADVCA_KEYLADDER_BUTT) || (enStage < HI_UNF_ADVCA_KEYLADDER_LEV2))
    {
        HI_ERR_CA("enStage = %d, invalid.\n", enStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_R2R_SETLADDER, &enStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_SETLADDER err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetDVBKeyLadderStage
*  Description:    get dvb ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetDVBKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (penStage == NULL)
    {
        HI_ERR_CA("penStage == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_GETLADDER, penStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CW_GETLADDER err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetDVBKeyLadderStage
*  Description:    set dvb ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetDVBKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((enStage < HI_UNF_ADVCA_KEYLADDER_LEV2) || (enStage >= HI_UNF_ADVCA_KEYLADDER_BUTT))
    {
        HI_ERR_CA("enStage = %d, invalid.\n", enStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_SETLADDER, &enStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DVB_SETLADDER err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetR2RAlg
*  Description:    set r2r alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_ALG_TYPE_BUTT)
    {
        HI_ERR_CA("enType >=  HI_UNF_ADVCA_ALG_TYPE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_R2R_SETALG, &enType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_SETALG err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetDVBAlg
*  Description:    set cw alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_ALG_TYPE_BUTT)
    {
        HI_ERR_CA("enType >=  HI_UNF_ADVCA_ALG_TYPE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_SETALG, &enType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CW_SETALG err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetR2RAlg
*  Description:    set cw alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pEnType == NULL)
    {
        HI_ERR_CA("pEnType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_R2R_GETALG, pEnType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_GETALG err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetDVBAlg
*  Description:    set cw alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pEnType == NULL)
    {
        HI_ERR_CA("pEnType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_GETALG, pEnType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DVB_GETALG err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetR2RSessionKey
*  Description:    set r2r session key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
/*lint -save -e818 ignored by m00190812, because these function prototypes could not be changed*/
HI_S32 HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret;
    CA_CRYPTPM_S cryptPm;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enTotalStage;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pu8Key == NULL)
    {
        HI_ERR_CA("pu8Key == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_R2R_GETLADDER, &enTotalStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_GETLADDER err. \n");
        return ret;
    }

    if (enStage > enTotalStage)
    {
        HI_ERR_CA("enStage(%d) > g_dvbLadder(%d), invalid.\n", enStage, enTotalStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    cryptPm.ladder = enStage;
    memcpy(cryptPm.pDin, pu8Key, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_R2R_CRYPT, &cryptPm);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_R2R_CRYPT err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetDVBSessionKey
*  Description:    set dvb session key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret;
    CA_CRYPTPM_S cryptPm;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pu8Key == NULL)
    {
        HI_ERR_CA("pu8Key == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (enStage >= g_dvbLadder)
    {
        HI_ERR_CA("enStage(%d) >= g_dvbLadder(%d), invalid.\n", enStage, g_dvbLadder);
        return HI_ERR_CA_INVALID_PARA;
    }

    cryptPm.ladder = enStage;
    memcpy(cryptPm.pDin, pu8Key, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_DVB_CRYPT, &cryptPm);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CW_CRYPT err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetGDRMSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                                    HI_HANDLE hCipherHandle,
                                    HI_U8 *pu8Input,
                                    HI_U32 u32InputLen,
                                    HI_U32 *pu32GdrmFlag)
{
    HI_S32 ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stTrans;
    CA_GDRM_CRYPTPM_S stGDRMCryptParam;

    if (g_AdvcaInitCounter <= 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((NULL == pu8Input) || (0 == u32InputLen) || (u32InputLen > 32))
    {
        HI_ERR_CA("Invalid params.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if ((HI_UNF_ADVCA_KEYLADDER_LEV3 == enLevel) && (NULL == pu32GdrmFlag))
    {
        HI_ERR_CA("Invalid level and enLevel, pu8GdrmFlag pointer.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stTrans, 0 , sizeof(stTrans));
    memset(&stGDRMCryptParam, 0, sizeof(stGDRMCryptParam));

    stGDRMCryptParam.stCACryptParams.ladder = enLevel;
    stGDRMCryptParam.stCACryptParams.enKlTarget = HI_UNF_ADVCA_CA_TARGET_MULTICIPHER;
    if (HI_UNF_ADVCA_KEYLADDER_LEV3 == enLevel)
    {
        stGDRMCryptParam.u32KeyAddr = hCipherHandle & 0xFF;
        stGDRMCryptParam.pu32GDRMFlag = pu32GdrmFlag;
    }
    stGDRMCryptParam.bIsDecrypt = HI_TRUE;
    memcpy(stGDRMCryptParam.stCACryptParams.pDin, pu8Input, u32InputLen);

    stTrans.enCmdChildID = CMD_CHILD_ID_CA_GDRM_CRYPT;
    memcpy(stTrans.pu8ParamBuf, &stGDRMCryptParam, sizeof(CA_GDRM_CRYPTPM_S));
    stTrans.u32ParamLen = sizeof(CA_GDRM_CRYPTPM_S);
    ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stTrans);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CHILD_ID_CA_GDRM_CRYPT err\n");
        return ret;
    }

    memcpy(&stGDRMCryptParam, stTrans.pu8ParamBuf, sizeof(stGDRMCryptParam));
    return HI_SUCCESS;
}

/**
\brief 加载LPK
*/
HI_S32 HI_UNF_ADVCA_LoadLpk(HI_U8 *pEncryptedLpk)
{
    HI_S32 ret;
    CA_LOADLPK_S stLoadLpk;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pEncryptedLpk)
    {
        HI_ERR_CA("HI_NULL == pEncryptLpk, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stLoadLpk.EncryptLpk, pEncryptedLpk, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_LPK_LOAD, &stLoadLpk);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DECRYPTLPKDATA err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_ADVCA_DecryptLptBlock
*  Description:    Decrypt the block data that the smart sdcard send to cpu with link protection.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
static HI_S32 HI_ADVCA_DecryptLptBlock(HI_U8 *pEncryptedBlock,HI_U8 *pPlainBlock)
{
    HI_S32 ret;
    CA_DECRYPTLPTDATA_S stDecryptLpData;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pEncryptedBlock)
    {
        HI_ERR_CA("HI_NULL == pEncryptData, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_NULL == pPlainBlock)
    {
        HI_ERR_CA("HI_NULL == pClearData, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stDecryptLpData.EncryptData, pEncryptedBlock, 8);
    ret = ioctl(g_s32CaFd, CMD_CA_LPK_DECRYPT, &stDecryptLpData);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DECRYPTLPKDATA err. \n");
        return ret;
    }

    memcpy(pPlainBlock,stDecryptLpData.ClearData,8);
    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DecryptLptParam
*  Description:    Decrypt the data that the smart sdcard send to cpu with link protection.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DecryptLptParam(HI_U8 *pCipherText,HI_S32 s32TextLen,HI_U8 *pPlainText)
{
    HI_S32 BlkNum, BlkIdx,s32BlkLen = 8;
    HI_S32 i,Ret;
    HI_S32 TailValidBytes;
    HI_U8 En1[8], Dn[8] = {0};
    HI_U8 IV[8]= {0};
    HI_U8 *pCipherBlk,*pPlainBlk;
    HI_U8 CN2[8] = {0},*pCN1,*pPN1,*pCN,*pPN;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pCipherText || HI_NULL == pPlainText || s32TextLen <= s32BlkLen)
    {
        HI_ERR_CA("invalid parameter\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    BlkNum = (s32TextLen + s32BlkLen - 1) / s32BlkLen;
    pCipherBlk = pCipherText;
    pPlainBlk = pPlainText;
    memset(IV, 0, (HI_U32)s32BlkLen);

    if (0 != s32TextLen%8)
    {
        /*1. Decrypt P[0] ~ P[n-2] */
        for (BlkIdx = 0; BlkIdx < BlkNum - 2; BlkIdx++)
        {
            /* ECB Decrypt */
            Ret = HI_ADVCA_DecryptLptBlock(pCipherBlk,pPlainBlk);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_CA("HI_ADVCA_DecryptLptBlock failed:%x\n",Ret);
                return Ret;
            }

            /* XOR */
            for (i = 0; i < s32BlkLen; i++)
            {
                pPlainBlk[i] = pPlainBlk[i] ^ IV[i];
            }

            /* Update */
            memcpy(IV, pCipherBlk, (HI_U32)s32BlkLen);
            pCipherBlk += s32BlkLen;
            pPlainBlk  += s32BlkLen;
        }

        /* Record C[n-2] C[n-1] P[n-1]*/
        memcpy(CN2, IV, (HI_U32)s32BlkLen);
        pCN1 = pCipherBlk;
        pPN1 = pPlainBlk;

        /*2. Decrypt P[n] */
        // D[n] = Decrypt (K, C[n-1]).
        Ret = HI_ADVCA_DecryptLptBlock(pCN1,Dn);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CA("HI_ADVCA_DecryptLptBlock failed:%x\n",Ret);
            return Ret;
        }

        // C = Cn || 0(B-M)
        memcpy(IV, pCipherText + (BlkNum-1) * s32BlkLen, (HI_U32)s32BlkLen);  /* IV = Cn */
        TailValidBytes = (0 == s32TextLen % s32BlkLen) ? (s32BlkLen) : (s32TextLen % s32BlkLen);
        for (i = TailValidBytes; i < s32BlkLen; i++)
        {
            IV[i] = 0;
        }

        // X[n] = D[n] XOR C
        for (i = 0; i < s32BlkLen; i++)
        {
            Dn[i] = Dn[i] ^ IV[i];
        }

        // Pn = Head(X[n],M)
        pPN = pPlainText + (BlkNum - 1) * s32BlkLen;
        memcpy(pPN, Dn, (HI_U32)TailValidBytes);

        /*3. Decrypt P[n-1] */
        // E[n-1] = Cn || Tail (Xn, B-M)
        pCN  = pCN1 + s32BlkLen;
        memcpy(En1, pCN, (HI_U32)TailValidBytes);

        if ( (s32BlkLen - TailValidBytes) <= (8 - TailValidBytes) )
        {
            memcpy(En1 + TailValidBytes, Dn + TailValidBytes, (HI_U32)(s32BlkLen - TailValidBytes));
        }
        else
        {
            HI_ERR_CA("Invalid TailValidBytes, Overrunning static array of size 8 bytes!\n");
            return HI_FAILURE;
        }

        // X[n-1] = Decrypt (K, E[n-1])
        pPN1 = pPlainText + (BlkNum - 2) * s32BlkLen;
        Ret = HI_ADVCA_DecryptLptBlock(En1,pPN1);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_CA("HI_ADVCA_DecryptLptBlock failed:%x\n",Ret);
            return Ret;
        }

        // P[n-1] = X[n-1] XOR C[n-2]
        for (i = 0; i < s32BlkLen; i++)
        {
            pPN1[i] = pPN1[i] ^ CN2[i];
        }

    }
    else
    {
        for (BlkIdx = 0; BlkIdx < BlkNum ; BlkIdx++)
        {
            /* ECB Decrypt */
            Ret = HI_ADVCA_DecryptLptBlock(pCipherBlk,pPlainBlk);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_CA("HI_ADVCA_DecryptLptBlock failed:%x\n",Ret);
                return Ret;
            }

            /* XOR */
            for (i = 0; i < s32BlkLen; i++)
            {
                pPlainBlk[i] = pPlainBlk[i] ^ IV[i];
            }

            /* Update */
            memcpy(IV, pCipherBlk, (HI_U32)s32BlkLen);
            pCipherBlk += s32BlkLen;
            pPlainBlk  += s32BlkLen;
        }
    }
    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_EncryptSWPK
*  Description:    Encrypt the key of software protetion.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_EncryptSWPK(HI_U8 *pPlainSwpk,HI_U8 *pEncryptedSwpk)
{
    HI_S32 ret;
    CA_ENCRYPTSWPK_S stEncryptSWPK;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pPlainSwpk)
    {
        HI_ERR_CA("HI_NULL == pClearSwpk, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_NULL == pEncryptedSwpk)
    {
        HI_ERR_CA("HI_NULL == pEncryptSwpk, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stEncryptSWPK.ClearSwpk, pPlainSwpk, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_SWPK_CRYPT, &stEncryptSWPK);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SWPK_CRYPT err. \n");
        return ret;
    }

    memcpy(pEncryptedSwpk,stEncryptSWPK.EncryptSwpk,16);

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetChipId
*  Description:    Set the chip id if it's not set.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         Be careful with the following functions!
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetChipId(HI_U32 Id)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_CHIPID, &Id);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_CHIPID err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetDVBRootKey(HI_U8 *pkey)
{
    HI_S32 ret;
    CA_KEY_S stCaKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stCaKey.KeyBuf, pkey, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_SET_DVB_ROOT_KEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_DVB_ROOT_KEY err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetR2RRootKey(HI_U8 *pkey)
{
    HI_S32 ret;
    CA_KEY_S stCaKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stCaKey.KeyBuf, pkey, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_SET_R2R_ROOT_KEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_R2R_ROOT_KEY err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetJtagKey(HI_U8 *pkey)
{
    HI_S32 ret;
    CA_KEY_S stCaKey;
    HI_U32 u32ChipID = 0;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_CHIPID, &u32ChipID);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_CHIPID err, ret = 0x%x. \n", ret);
        return ret;
    }

    if(0 == u32ChipID)
    {
        HI_ERR_CA("Error! Please set chipid before setting jtag key! \n");
        return HI_FAILURE;
    }

    memcpy(stCaKey.KeyBuf, pkey, 8);
    ret = ioctl(g_s32CaFd, CMD_CA_SET_JTAG_KEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_JTAG_KEY err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetJtagKey(HI_U8 *pkey)
{
    HI_S32 ret;
    CA_KEY_S stCaKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_JTAG_KEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_JTAG_KEY err. \n");
        return ret;
    }

    memcpy(pkey, stCaKey.KeyBuf, 8);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetSPRootKey(HI_U8 *pu8Key, HI_U32 u32Len)
{
    HI_S32 ret = HI_SUCCESS;
    CA_KEY_S stCaKey;
    if((NULL == pu8Key) || (16 != u32Len))
    {
        HI_ERR_CA("Invalid parameters!\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }
    memcpy(stCaKey.KeyBuf, pu8Key, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_SET_SP_ROOT_KEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_SP_ROOT_KEY err. \n");
        return ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetRSAKey(HI_U8 *pkey)
{
    HI_S32 ret = HI_SUCCESS;
    CA_KEY_S stCaKey, stRdKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memcpy(stCaKey.KeyBuf, pkey, 512);
    ret = ioctl(g_s32CaFd, CMD_CA_EXT1_SETRSAKEY, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_EXT1_SETRSAKEY err. \n");
        return ret;
    }

    memset(stRdKey.KeyBuf, 0, 512);
    ret = ioctl(g_s32CaFd, CMD_CA_EXT1_GETRSAKEY, &stRdKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_EXT1_GETRSAKEY err. \n");
        return ret;
    }

    if (memcmp(stCaKey.KeyBuf, stRdKey.KeyBuf, 512))
    {
        HI_ERR_CA("RSA Key Set err. \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetRSAKey(HI_U8 *pkey)
{
    HI_S32 ret = HI_SUCCESS;
    CA_KEY_S stRdKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pkey == NULL)
    {
        HI_ERR_CA("pkey == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(stRdKey.KeyBuf, 0, 512);
    ret = ioctl(g_s32CaFd, CMD_CA_EXT1_GETRSAKEY, &stRdKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_EXT1_GETRSAKEY err. \n");
        return ret;
    }

    memcpy(pkey, stRdKey.KeyBuf, 512);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_OtpReadByte(HI_U32 Addr, HI_U8 *pu8Value)
{
    HI_S32 ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if((Addr >= 0x800) || (NULL == pu8Value))
    {
        HI_ERR_CA("Invalid parameters!\n");
        return HI_FAILURE;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));

    stCmdParam.enCmdChildID = CMD_CHILD_ID_OTP_READ_BYTE;
    memcpy(stCmdParam.pu8ParamBuf, &Addr, sizeof(HI_U32));
    stCmdParam.u32ParamLen = 5;

    ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err. \n");
        return ret;
    }

    *pu8Value = stCmdParam.pu8ParamBuf[4];

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_OtpWriteByte(HI_U32 Addr,HI_U8 Value)
{
    HI_S32 ret;
    OTP_ENTRY_S stOtpEntry;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    stOtpEntry.Addr  = Addr;
    stOtpEntry.Value = Value;
    ret = ioctl(g_s32CaFd, CMD_CA_EXT1_OTPWRITEBYTE, &stOtpEntry);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_EXT1_OPTWRITE err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_OtpRead(HI_U32 Addr, HI_U32 *pValue)
{
    HI_S32 ret = HI_SUCCESS;
    OTP_ENTRY_S stOtpEntry;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pValue)
    {
        HI_ERR_CA("invalid param pValue\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (0 != (Addr % 4))
    {
        HI_ERR_CA("Addr is not aliged by 4\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    stOtpEntry.Addr = Addr;
    ret = ioctl(g_s32CaFd, CMD_CA_EXT1_OTPREAD, &stOtpEntry);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_EXT1_OTPREAD err. \n");
        return ret;
    }

    *pValue = stOtpEntry.Value;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_NovelCryptologyConfig(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32VendorType = 0;
    HI_U8  SessionKey1[16];
    HI_U8  SessionKey2[16];
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage = HI_UNF_ADVCA_KEYLADDER_BUTT;
    HI_U8 u8CipherInput[16] = {0x10, 0x6b, 0xe2, 0x28, 0xc6, 0x7b, 0x82, 0x09, 0x6a, 0x17, 0xc7, 0x25, 0xe5, 0xd5, 0x55, 0x4e};
    HI_U8 u8KeyladderInput1[16] = {0xb4, 0x8a, 0x34, 0xee, 0x36, 0x74, 0x8b, 0xf8, 0x98, 0x11, 0x10, 0x2c, 0x27, 0x8c, 0xe6, 0xdf};
    HI_U8 u8KeyladderInput2[16] = {0xd2, 0x7b, 0xfc, 0x9b, 0x68, 0x91, 0x99, 0x62, 0x77, 0x0f, 0x27, 0x3c, 0xc8, 0x45, 0xfa, 0xfb};
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if(HI_TRUE == g_bCryptologyConfig)
    {
        HI_INFO_CA("HI_UNF_ADVCA_NovelCryptologyConfig has already be called\n");
        return 0;
    }

    ret = HI_UNF_ADVCA_GetVendorID(&u32VendorType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("get vendor type err. \n");
        return HI_FAILURE;
    }
    g_vendorType = (HI_UNF_ADVCA_VENDORID_E)u32VendorType;

    if (HI_UNF_ADVCA_NOVEL != g_vendorType)
    {
        HI_ERR_CA("CA vendor NOT match! Permission denied:%d != %d!\n", HI_UNF_ADVCA_NOVEL, g_vendorType);
        return HI_ERR_CA_NOT_SUPPORT;
    }

    memset(SessionKey1,0,sizeof(SessionKey1));
    memset(SessionKey2,0,sizeof(SessionKey2));
    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));

    /*open and config keyladder*/
    ret = HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_ADVCA_SetR2RAlg failed:%#x\n", ret);
        return ret;
    }

    ret = HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_ADVCA_SetR2RSessionKey failed:%#x\n", ret);
        return ret;
    }
    memcpy(SessionKey1, u8KeyladderInput1, sizeof(SessionKey1));
    ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,SessionKey1);
    if(enStage == HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        memcpy(SessionKey2, u8KeyladderInput2, sizeof(SessionKey2));
        ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2,SessionKey2);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("HI_UNF_ADVCA_SetR2RSessionKey failed:%#x\n", ret);
            return ret;
        }
    }
    /*open and config Cipher*/
    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    memset(CipherCtrl.u32IV, 0, sizeof(CipherCtrl.u32IV));
    memcpy(CipherCtrl.u32Key, u8CipherInput, 16);
    ret = HI_UNF_CIPHER_Open();
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("HI_UNF_CIPHER_Open failed:%#x\n", ret);
        return ret;
    }

    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;

    ret = HI_UNF_CIPHER_CreateHandle(&g_hCipher, &stCipherAttr);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("HI_UNF_CIPHER_CreateHandle failed:%#x\n", ret);
        return ret;
    }

    ret = HI_UNF_CIPHER_ConfigHandle(g_hCipher, &CipherCtrl);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("HI_UNF_CIPHER_ConfigHandle failed:%#x\n", ret);
        (HI_VOID)HI_UNF_CIPHER_DestroyHandle(g_hCipher);
        return ret;
    }
    g_bCryptologyConfig = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_NovelDataEncrypt(HI_U8 *pPlainText, HI_U32 TextLen, HI_U8 *pCipherText)
{
    HI_S32 ret = HI_FAILURE;

    if (HI_FALSE == g_bCryptologyConfig)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_NovelCryptologyConfig first\n");
        return ret;
    }
    if ((HI_NULL == pPlainText) || (HI_NULL == pCipherText))
    {
        HI_ERR_CA("null pointer error\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if ((TextLen < 16) || (TextLen%16 != 0))
    {
        HI_ERR_CA("Data length must be times of 16BYTE\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_UNF_ADVCA_NOVEL != g_vendorType)
    {
        HI_ERR_CA("CA vendor NOT match! Permission denied!\n");
        return HI_ERR_CA_NOT_SUPPORT;
    }

    return HI_UNF_CIPHER_Encrypt(g_hCipher, (HI_U32)(HI_ULONG)pPlainText, (HI_U32)(HI_ULONG)pCipherText, TextLen);
}


HI_S32 HI_UNF_ADVCA_NovelDataDecrypt(HI_U8 *pCipherText, HI_U32 TextLen, HI_U8 *pPlainText)
{
    HI_S32 ret = HI_FAILURE;

    if(HI_FALSE == g_bCryptologyConfig)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_NovelCryptologyConfig first\n");
        return ret;
    }
    if ((HI_NULL == pPlainText) || (HI_NULL == pCipherText))
    {
        HI_ERR_CA("null pointer error\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    if ((TextLen < 16) || (TextLen%16 != 0))
    {
        HI_ERR_CA("Data length must be times of 16BYTE\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_UNF_ADVCA_NOVEL != g_vendorType)
    {
        HI_ERR_CA("CA vendor NOT match! Permission denied!\n");
        return HI_ERR_CA_NOT_SUPPORT;
    }

    return HI_UNF_CIPHER_Decrypt(g_hCipher, (HI_U32)(HI_ULONG)pCipherText, (HI_U32)(HI_ULONG)pPlainText, TextLen);
}

HI_S32 HI_UNF_ADVCA_IsMarketIdSet(HI_BOOL *pbIsMarketIdSet)
{
    HI_S32 ret = HI_SUCCESS;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (NULL == pbIsMarketIdSet)
    {
        HI_ERR_CA("NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_CHECK_MARKET_ID_SET, pbIsMarketIdSet);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CHECK_MARKET_ID_SET err.\n");
        return ret;
    }

    return HI_SUCCESS;
}



HI_S32 HI_UNF_ADVCA_GetVendorID(HI_U32 *pu32VendorID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pu32VendorID == NULL)
    {
        HI_ERR_CA("pu32VendorID == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_VENDOR_ID, pu32VendorID);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_VENDOR_ID err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetSPKeyLadderStage
*  Description:    get SP ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetSPKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (penStage == NULL)
    {
        HI_ERR_CA("penStage == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_GETLADDER, penStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_GETLADDER err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetSPKeyLadderStage
*  Description:    set SP ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetSPKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((enStage >= HI_UNF_ADVCA_KEYLADDER_BUTT) || (enStage < HI_UNF_ADVCA_KEYLADDER_LEV2))
    {
        HI_ERR_CA("enStage = %d, invalid.\n", enStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_SETLADDER, &enStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_SETLADDER err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetSPAlg
*  Description:    set SP alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetSPAlg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_ALG_TYPE_BUTT)
    {
        HI_ERR_CA("enType(0x%x) >=  HI_UNF_ADVCA_ALG_TYPE_BUTT(0x%x), invalid.\n", enType, HI_UNF_ADVCA_ALG_TYPE_BUTT);
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_SETALG, &enType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_SETALG err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetSPAlg
*  Description:    get SP alg
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetSPAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pEnType == NULL)
    {
        HI_ERR_CA("pEnType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_GETALG, pEnType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_GETALG err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetSPDscMode
*  Description:    set SP dsc mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E enType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_SP_DSC_MODE_BUTT)
    {
        HI_ERR_CA("enType(0x%x) >=  HI_UNF_ADVCA_SP_DSC_MODE_BUTT(0x%x), invalid.\n", enType, HI_UNF_ADVCA_SP_DSC_MODE_BUTT);
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_SET_DSC_MODE, &enType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_SET_DSC_MODE err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetSPDscMode
*  Description:    get SP dsc mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E *pEnType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pEnType == NULL)
    {
        HI_ERR_CA("pEnType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_GET_DSC_MODE, pEnType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_GET_DSC_MODE err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetSPSessionKey
*  Description:    set SP session key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetSPSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret;
    CA_CRYPTPM_S cryptPm;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enTotalStage;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pu8Key == NULL)
    {
        HI_ERR_CA("pu8Key == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SP_GETLADDER, &enTotalStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_GETLADDER err.\n");
        return ret;
    }

    if (enStage > enTotalStage)
    {
        HI_ERR_CA("enStage(%d) > enTotalStage(%d), invalid.\n", enStage, enTotalStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    cryptPm.ladder = enStage;
    memcpy(cryptPm.pDin, pu8Key, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_SP_CRYPT, &cryptPm);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_CRYPT err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetCSA3Alg
*  Description:    get the algorithm of CSA3 key ladder
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetCSA3Alg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pEnType == NULL)
    {
        HI_ERR_CA("pEnType == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_CSA3_GETALG, pEnType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DVB_CSA3_GETALG err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetCSA3Alg
*  Description:    set the algorithm of CSA3 key ladder
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetCSA3Alg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_ALG_TYPE_BUTT)
    {
        HI_ERR_CA("enType >=  HI_UNF_ADVCA_ALG_TYPE_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DVB_CSA3_SETALG, &enType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DVB_CSA3_SETALG err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetCSA3KeyLadderStage
*  Description:    get CSA3 ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetCSA3KeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (penStage == NULL)
    {
        HI_ERR_CA("penStage == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_CSA3_GETLADDER, penStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CSA3_GETLADDER err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetCSA3KeyLadderStage
*  Description:    set CSA3 ladder stage
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetCSA3KeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (HI_UNF_ADVCA_KEYLADDER_LEV2 != enStage) && (HI_UNF_ADVCA_KEYLADDER_LEV3 != enStage))
    {
        HI_ERR_CA("Invalid keyladder level select! Only supported 2 or 3 level\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_CSA3_SETLADDER, &enStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CSA3_SETLADDER err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetCSA3SessionKey
*  Description:    set CSA3 session key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetCSA3SessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret;
    CA_CRYPTPM_S cryptPm;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enTotalStage;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (pu8Key == NULL)
    {
        HI_ERR_CA("pu8Key == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_CSA3_GETLADDER, &enTotalStage);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SP_GETLADDER err.\n");
        return ret;
    }

    if (enStage > enTotalStage)
    {
        HI_ERR_CA("enStage(%d) > enTotalStage(%d), invalid.\n", enStage, enTotalStage);
        return HI_ERR_CA_INVALID_PARA;
    }

    cryptPm.ladder = enStage;
    memcpy(cryptPm.pDin, pu8Key, 16);
    ret = ioctl(g_s32CaFd, CMD_CA_CSA3_CRYPT, &cryptPm);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_CSA3_CRYPT err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_DisableDDRWakeup(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_LOWPOWER_DISABLE, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_LOWPOWER_DISABLE err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_SetDDRScramble(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Set = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_DDR_SCRAMBLE_EN, &u32Set);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_DDR_SCRAMBLE_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_GetDDRScrambleStat(HI_U32 *pu32Stat)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pu32Stat)
    {
        HI_ERR_CA("pu32Stat == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_DDR_SCRAMBLE_EN, pu32Stat);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_DDR_SCRAMBLE_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_LockCSA3HardCW(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_CSA3_HARDONLY_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_CSA3_HARDONLY_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_GetCSA3HardCWStat(HI_BOOL *pbLock)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pbLock)
    {
        HI_ERR_CA("pbLock == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_CSA3_HARDONLY_EN, pbLock);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_CSA3_HARDONLY_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_LockSPHardCW(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_SP_HARDONLY_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_SP_HARDONLY_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_DisableTsklDES(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_TSKL_DES_DISABLE, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_TSKL_DES_DISABLE err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_LockGlobalOTP(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_OTP_GLOBAL_LOCK_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_OTP_GLOBAL_LOCK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_DisableDCasKl(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_DCAS_KL_DISABLE, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_DCAS_KL_DISABLE err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_EnableRuntimeCheck(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_RUNTIME_CHECK_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_RUNTIME_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_EnableDDRWakeupCheck(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_WAKEUP_DDR_CHECK_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_WAKEUP_DDR_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_EnableVersionCheck(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_VERSION_CHECK_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_VERSION_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_GetVersionCheckStat(HI_U32 *pu32Stat)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pu32Stat)
    {
        HI_ERR_CA("pu32Stat == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_VERSION_CHECK_EN, pu32Stat);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_VERSION_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_EnableBootMSIDCheck(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_BL_MSID_CHECK_EN, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_BL_MSID_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_GetBootMSIDCheckStat(HI_U32 *pu32Stat)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == pu32Stat)
    {
        HI_ERR_CA("pu32Stat == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_BL_MSID_CHECK_EN, pu32Stat);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_BL_MSID_CHECK_EN err.\n");
        return ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_ADVCA_DisableJtagRead(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32Value = 1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SET_JTAG_READ_DISABLE, &u32Value);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_JTAG_READ_DISABLE err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_ConfigR2RKeyladderAndCipher(HI_UNF_R2RKeyladder_Cipher_Info_S * pstR2RkeyladderCipherInfo)
{
    HI_S32 ret = 0;

    CA_CheckPointer(pstR2RkeyladderCipherInfo);

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

#ifdef HI_PVR_SUPPORT
    (HI_VOID)sem_wait(&g_PVRSem);
#endif

    ret = HI_UNF_ADVCA_SetR2RAlg(pstR2RkeyladderCipherInfo->KeyladderAlg);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA(" call HI_UNF_ADVCA_SetR2RAlg failed\n ");
        goto ConfigErr;
    }

    ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, pstR2RkeyladderCipherInfo->SessionKey1);
    if (HI_UNF_ADVCA_KEYLADDER_LEV3 == pstR2RkeyladderCipherInfo->KeyladderLevel)
    {
        ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2, pstR2RkeyladderCipherInfo->SessionKey2);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CA(" call HI_UNF_ADVCA_SetR2RSessionKey failed\n ");
            goto ConfigErr;
        }
    }

    ret = HI_UNF_CIPHER_ConfigHandle(pstR2RkeyladderCipherInfo->hCipherHandle, &(pstR2RkeyladderCipherInfo->stCipherCtrl));
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA(" call HI_UNF_CIPHER_ConfigHandle failed, hCipherHandle = 0x%x\n ",pstR2RkeyladderCipherInfo->hCipherHandle);
        goto ConfigErr;
    }
#ifdef HI_PVR_SUPPORT
    (HI_VOID)sem_post(&g_PVRSem);
#endif
    return HI_SUCCESS;

ConfigErr:
#ifdef HI_PVR_SUPPORT
    (HI_VOID)sem_post(&g_PVRSem);
#endif
    return HI_FAILURE;
}

#ifdef HI_PVR_SUPPORT
/*the following APIs is for ADVCA_PVR*/
static unsigned char new_rand()
{
    HI_U8 n = 0;
    HI_U32 u32Number = 0;
    HI_S32 ret = HI_SUCCESS;

    ret = HI_MPI_CIPHER_Init();
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HI_MPI_CIPHER_Init failed, ret: 0x%x\n", ret);
        return 0;
    }

    ret = HI_MPI_CIPHER_GetRandomNumber(&u32Number, -1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HI_MPI_CIPHER_GetRandomNumber failed, ret: 0x%x\n", ret);
    }

    ret = HI_MPI_CIPHER_DeInit();
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HI_MPI_CIPHER_DeInit failed, ret: 0x%x\n", ret);
        return 0;
    }

    n = u32Number & 0XFF;

    return n;
}

static HI_S32 ADVCA_PVR_GetCADataFileName(const HI_CHAR *pRefFileName, HI_CHAR KeyFileName[128])
{
    HI_CHAR  IndexSuffix[5] = ".key";
    HI_CHAR  *p = HI_NULL;

    CA_CheckPointer(pRefFileName);
    CA_CheckPointer(KeyFileName);

    p = strrchr(pRefFileName,'.');
    if (NULL != p)
    {
        memset(KeyFileName, 0, 128);
        memcpy(KeyFileName,pRefFileName,(HI_U32)(p - pRefFileName));
        memcpy(KeyFileName + (HI_U32)(p - pRefFileName),IndexSuffix,4);
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

static HI_S32 ADVCA_PVR_SetCAData(const HI_CHAR *pFileName, HI_CA_PVR_CADATA_S* pstPvrCaData, HI_CA_PVR_KEY_INFO_S *pstNewKey)
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 fd;
    HI_S32 len;
    HI_U32 DataLen = 0;
    HI_U8 *buf = HI_NULL;
    HI_CHAR KeyFileName[128] = {0};
    HI_CA_PVR_MAC_CADATA_S *pMacCaData;

    CA_CheckPointer(pFileName);
    CA_CheckPointer(pstPvrCaData);

    if (pstNewKey != HI_NULL)
    {
        if (0 == pstPvrCaData->u32TotalBlockNum)
        {
            HI_ERR_CA("TotalBlockNum error!\n");
            return HI_FAILURE;
        }
    }

    if (pstPvrCaData->u32TotalBlockNum > MAX_PVR_KEY_BLOCK_NUMBERS)
    {
        HI_ERR_CA("u32TotalBlockNum: %d > %d\n", pstPvrCaData->u32TotalBlockNum, MAX_PVR_KEY_BLOCK_NUMBERS);
        return HI_FAILURE;
    }

    ret = ADVCA_PVR_GetCADataFileName(pFileName, KeyFileName);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("error, fail to get key file name!\n");
        return HI_FAILURE;
    }

    fd = open(KeyFileName, O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("error, open %s failed!\n", KeyFileName);

        return HI_FAILURE;
    }

    DataLen = sizeof(HI_CA_PVR_MAC_CADATA_S) + pstPvrCaData->u32TotalBlockNum * sizeof(HI_CA_PVR_KEY_INFO_S);

    buf = (HI_U8*)malloc(DataLen);
    if (buf == HI_NULL)
    {
        HI_ERR_CA("error, fail to malloc memory!\n");

        close(fd);

        return HI_FAILURE;
    }

    len = read(fd, buf, DataLen);
    if (len != DataLen)
    {
        ret = HI_FAILURE;

        goto RET;
    }

    pMacCaData = (HI_CA_PVR_MAC_CADATA_S*)buf;

    memset(pMacCaData->u8AES_CMAC, 0, sizeof(pMacCaData->u8AES_CMAC));
    memcpy(&pMacCaData->stPvrCaData, pstPvrCaData, sizeof(HI_CA_PVR_CADATA_S));

    if (pstNewKey != HI_NULL)
    {
       /*copy new key into buffer, because the latest key have no save into flash now! */
       memcpy(buf + DataLen - sizeof(HI_CA_PVR_KEY_INFO_S), pstNewKey, sizeof(HI_CA_PVR_KEY_INFO_S));
    }

    ret = HI_UNF_ADVCA_CalculteAES_CMAC(buf + sizeof(pMacCaData->u8AES_CMAC), DataLen - sizeof(pMacCaData->u8AES_CMAC),
        pstPvrCaData->stR2RkeyladderCipherInfo.SessionKey1, pMacCaData->u8AES_CMAC);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");
        pstPvrCaData->u32TotalBlockNum--;

        ret = HI_FAILURE;

        goto RET;
    }

    if (0 != lseek(fd, 0, SEEK_SET))
    {
        HI_ERR_CA("lseek error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    len = write(fd, buf, sizeof(HI_CA_PVR_MAC_CADATA_S));
    if (len != sizeof(HI_CA_PVR_MAC_CADATA_S))
    {
        HI_ERR_CA("write CA private file error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    if (pstNewKey != HI_NULL)
    {
        HI_U32 offset = DataLen - sizeof(HI_CA_PVR_KEY_INFO_S);

        if (offset != lseek(fd, offset, SEEK_SET))
        {
            HI_ERR_CA("lseek error\n");

            ret = HI_FAILURE;

            goto RET;
        }

        len = write(fd, pstNewKey, sizeof(HI_CA_PVR_KEY_INFO_S));
        if (len != sizeof(HI_CA_PVR_KEY_INFO_S))
        {
            HI_ERR_CA("write CA private file error\n");

            ret = HI_FAILURE;

            goto RET;
        }
    }

    ret = HI_SUCCESS;

RET:
    if (fd >= 0)
    {
        close(fd);
    }

    if (HI_NULL != buf)
    {
        free(buf);
    }

    return ret;
}

static HI_S32 ADVCA_PVR_GetCAData(const HI_CHAR *pFileName, HI_CA_PVR_CADATA_S *pstPvrCaData, HI_U8 *pu8Buffer)
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 fd;
    HI_S32 len;
    HI_U32 DataLen = 0;
    HI_U8  *pu8Buf;
    HI_U8 MacKey[16];
    HI_U8 Mac[16] = {0};
    HI_CHAR KeyFileName[128];
    HI_CA_PVR_MAC_CADATA_S stMacCaData;
    HI_CA_PVR_CADATA_S* pstPrivateInfo = &stMacCaData.stPvrCaData;

    CA_CheckPointer(pFileName);
    CA_CheckPointer(pstPvrCaData);

    ret = ADVCA_PVR_GetCADataFileName(pFileName, KeyFileName);
    if (ret != HI_SUCCESS)
    {
       HI_ERR_CA("error, fail to get key file name!\n");
       return HI_FAILURE;
    }

    HI_INFO_CA("KeyFileName=%s\n", KeyFileName);

    fd = open(KeyFileName, O_RDONLY);
    if (fd < 0)
    {
        HI_ERR_CA("open %s failed!\n", KeyFileName);
        return HI_FAILURE;
    }

    len = read(fd, &stMacCaData, sizeof(HI_CA_PVR_MAC_CADATA_S));
    if ((sizeof(HI_CA_PVR_MAC_CADATA_S) != len) || (0 == pstPrivateInfo->u32TotalBlockNum) || (pstPrivateInfo->u32TotalBlockNum > MAX_PVR_KEY_BLOCK_NUMBERS))
    {
        close(fd);
        HI_ERR_CA("read failed, len=%d\n", len);
        return HI_FAILURE;
    }

    HI_INFO_CA("TotalBlockNum=%u\n", pstPrivateInfo->u32TotalBlockNum);

    DataLen = pstPrivateInfo->u32TotalBlockNum * sizeof(HI_CA_PVR_KEY_INFO_S);

    pu8Buf = malloc(sizeof(HI_CA_PVR_CADATA_S) + DataLen);
    if (pu8Buf == HI_NULL)
    {
        close(fd);
        HI_ERR_CA("error, fail to malloc memory!\n");
        return HI_FAILURE;
    }

    memcpy(pu8Buf, &stMacCaData.stPvrCaData, sizeof(HI_CA_PVR_CADATA_S));

    len = read(fd, pu8Buf + sizeof(HI_CA_PVR_CADATA_S), DataLen);
    if (len != DataLen)
    {
        free(pu8Buf);

        close(fd);

        HI_ERR_CA("read failed, len=%d\n", len);

        return HI_FAILURE;
    }

    close(fd);

    memset(Mac, 0, sizeof(Mac));
    memcpy(MacKey, pstPrivateInfo->stR2RkeyladderCipherInfo.SessionKey1, 16);

    ret = HI_UNF_ADVCA_CalculteAES_CMAC(pu8Buf, sizeof(HI_CA_PVR_CADATA_S) + DataLen, MacKey, Mac);
    if (HI_SUCCESS != ret)
    {
        free(pu8Buf);

        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");

        return HI_FAILURE;
    }

    if (0 != memcmp(Mac, stMacCaData.u8AES_CMAC, 16))
    {
        free(pu8Buf);

        HI_ERR_CA("error, CAData Mac error\n");

        return HI_FAILURE;
    }

    memcpy(pstPvrCaData, &stMacCaData.stPvrCaData, sizeof(HI_CA_PVR_CADATA_S));

    if (pu8Buffer != HI_NULL && pstPrivateInfo->u32TotalBlockNum <= MAX_PVR_KEY_BLOCK_NUMBERS)
    {
        memcpy(pu8Buffer, pu8Buf + sizeof(HI_CA_PVR_CADATA_S), DataLen);
    }

    free(pu8Buf);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_RecOpen(HI_U32 u32RecChnID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0, RecChnIndex;
    HI_U8 SessionKey1[16];
    HI_U8 SessionKey2[16];
    HI_U8 CipherKey[16];
    HI_UNF_ADVCA_KEYLADDER_LEV_E KeyladderStage = HI_UNF_ADVCA_KEYLADDER_BUTT;
    HI_UNF_CIPHER_CTRL_S *pstCipherCtrl = NULL;
    HI_CA_PVR_CADATA_S * pstPvrRecCaData = NULL;
    HI_UNF_R2RKeyladder_Cipher_Info_S * pstR2RkeyladderCipherInfo = NULL;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
    HI_UNF_PVR_REC_ATTR_S recAttr;

    memset(&stCipherAttr, 0, sizeof(HI_UNF_CIPHER_ATTS_S));

    memset(SessionKey1, 0, sizeof(SessionKey1));
    memset(SessionKey2, 0, sizeof(SessionKey2));
    memset(CipherKey, 0, sizeof(CipherKey));
    memset(&recAttr, 0, sizeof(HI_UNF_PVR_REC_ATTR_S));

    CA_ASSERT(HI_UNF_ADVCA_Init(),ret);

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            HI_INFO_CA("this channel already opend\n");
            return HI_SUCCESS;
        }
    }

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if(INVALID_VALUE == PvrRecChannelInfo[i].u32ChnID)
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,not enough record channel to use\n");
        return HI_FAILURE;
    }

    RecChnIndex = i;

    memset(&PvrRecChannelInfo[RecChnIndex], 0x0, sizeof(HI_CA_PVR_RECORD_CHANNEL_INFO_S));

    PvrRecChannelInfo[RecChnIndex].u32ChnID = u32RecChnID;
    PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc = 0UL;
    PvrRecChannelInfo[RecChnIndex].PvrRecBlockDataAcc = 0UL;
    PvrRecChannelInfo[RecChnIndex].PvrRecForceChangeKey = HI_FALSE;
    PvrRecChannelInfo[RecChnIndex].bCADataSavedFlag = HI_FALSE;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;
    memset(pstPvrRecCaData,0,sizeof(HI_CA_PVR_CADATA_S));

#ifndef HI_LOADER_APPLOADER
    CA_ASSERT(HI_UNF_PVR_RecGetChn(u32RecChnID, &recAttr), ret);
#endif

    PvrRecChannelInfo[RecChnIndex].bRewind = recAttr.bRewind;

    pstR2RkeyladderCipherInfo = &(pstPvrRecCaData->stR2RkeyladderCipherInfo);
    /*config keyladder and init the CA info Data*/

    /*generate the random session key and cipher key*/
    for(i=0;i<16;i++)
    {
        SessionKey1[i] = new_rand();
        SessionKey2[i] = new_rand();
    }
    if(SessionKey1[0] == SessionKey1[8])
    {
        SessionKey1[0] = SessionKey1[8] + 1;
    }
    if(SessionKey2[0] == SessionKey2[8])
    {
        SessionKey2[0] = SessionKey2[8] + 1;
    }
    if(SessionKey1[0] == SessionKey2[0])
    {
        SessionKey1[0] = SessionKey2[0] + 1;
    }

    /*generate the random  cipher key*/
    for(i=0;i<16;i++)
    {
        CipherKey[i] = new_rand();
    }
    if (CipherKey[0] == CipherKey[8])
    {
        CipherKey[0] = CipherKey[8] + 1;
    }

    CA_ASSERT(HI_UNF_ADVCA_GetR2RKeyLadderStage(&KeyladderStage),ret);

    pstR2RkeyladderCipherInfo->KeyladderAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    pstR2RkeyladderCipherInfo->KeyladderLevel = KeyladderStage;
    memcpy(pstR2RkeyladderCipherInfo->SessionKey1,SessionKey1,sizeof(SessionKey1));
    memcpy(pstR2RkeyladderCipherInfo->SessionKey2,SessionKey2,sizeof(SessionKey2));

    /*open cipher*/
    CA_ASSERT(HI_UNF_CIPHER_Open(),ret);
    pstR2RkeyladderCipherInfo->hCipherHandle = INVALID_VALUE;

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    CA_ASSERT( HI_UNF_CIPHER_CreateHandle(&pstR2RkeyladderCipherInfo->hCipherHandle, &stCipherAttr),ret);

    PvrRecChannelInfo[RecChnIndex].CurBlockNum = 0;
    pstPvrRecCaData->u32TotalBlockNum++;
    pstCipherCtrl = &(pstR2RkeyladderCipherInfo->stCipherCtrl);
    pstCipherCtrl->bKeyByCA = HI_TRUE;
    pstCipherCtrl->enAlg = HI_UNF_CIPHER_ALG_AES;
    pstCipherCtrl->enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    pstCipherCtrl->enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    pstCipherCtrl->enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;

    memcpy(pstCipherCtrl->u32Key,CipherKey,16);

    CA_ASSERT(HI_UNF_ADVCA_ConfigR2RKeyladderAndCipher(pstR2RkeyladderCipherInfo),ret);
    /*update track key value to avoid config keyladder key more times*/
    memcpy(PvrRecChannelInfo[RecChnIndex].PvrRecTrackKey,CipherKey,16);
    return  HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_RecClose(HI_U32 u32RecChnID)
{
    HI_S32 ret;
    HI_U32 i = 0, RecChnIndex=0, PlyChnIndex=0;
    HI_HANDLE hCipher = INVALID_VALUE;
    HI_CA_PVR_CADATA_S * pstPvrRecCaData = NULL;

    /*find out the PVR CA infor by PVR channel ID*/
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("PVR channel ID error\n");
        return HI_FAILURE;
    }

    RecChnIndex = i;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;

    /*we need save remain recording data when close recording*/
    pstPvrRecCaData->u64CxVerifiedLen = pstPvrRecCaData->u64CxVerifiedLen + PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc;
    HI_INFO_CA("HI_UNF_ADVCA_PVR_RecClose: pstPvrRecCaData->u64CxVerifiedLen is %llu--->\n", pstPvrRecCaData->u64CxVerifiedLen);
    if(pstPvrRecCaData->FileName[0] != 0)
    {
        CA_ASSERT(ADVCA_PVR_SetCAData(pstPvrRecCaData->FileName, pstPvrRecCaData, HI_NULL),ret);
    }
    PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc = 0;

    hCipher = pstPvrRecCaData->stR2RkeyladderCipherInfo.hCipherHandle;

    ADVCA_PVR_LOCK();
    for (i = 0 ; i < PVR_CA_MAX_CHANNEL; i++)
    {
       if ((PvrPlayChannelInfo[i].u32ChnID != INVALID_VALUE)
           && (PvrPlayChannelInfo[i].bIsTimeshiftPlay == HI_TRUE)
           && (PvrPlayChannelInfo[i].u32RecChnID == u32RecChnID))
       {
          /*For timeshift, when stop timeshift recording, we must update related recording data in play channel
            For example, we must updade u64CxVerifiedLen in play channel.
          */
          PlyChnIndex = i;
          PvrPlayChannelInfo[PlyChnIndex].bIsTimeshiftPlay = HI_FALSE;
          PvrPlayChannelInfo[PlyChnIndex].stPvrPlyCaData.u64CxVerifiedLen = pstPvrRecCaData->u64CxVerifiedLen;
          HI_INFO_CA("HI_UNF_ADVCA_PVR_RecClose Set wait flag\n");
          break;
       }
    }
    memset(pstPvrRecCaData,0x00,sizeof(HI_CA_PVR_CADATA_S));
    PvrRecChannelInfo[RecChnIndex].u32ChnID = INVALID_VALUE;
    ADVCA_PVR_UNLOCK();

    if (INVALID_VALUE != hCipher)
    {
        CA_ASSERT(HI_UNF_CIPHER_DestroyHandle(hCipher),ret);
    }

    return  HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_PlayOpen(HI_U32 u32PlayChnID, HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPlayInfo)
{
    HI_S32 ret;
    HI_U32 i = 0, PlyChnIndex=0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    HI_CA_PVR_CADATA_S * pstPvrPlyCaData = NULL;

    CA_ASSERT(HI_UNF_ADVCA_Init(),ret);
    CA_ASSERT(HI_UNF_CIPHER_Open(),ret);

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32PlayChnID == PvrPlayChannelInfo[i].u32ChnID )
        {
            HI_INFO_CA("this channel already opened\n");
            return HI_SUCCESS;
        }
    }

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if(INVALID_VALUE == PvrPlayChannelInfo[i].u32ChnID)
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,not enough play channel to use\n");
        return HI_FAILURE;
    }

    PlyChnIndex = i;

    memset(&PvrPlayChannelInfo[PlyChnIndex], 0x0, sizeof(HI_CA_PVR_PLAY_CHANNEL_INFO_S));

    PvrPlayChannelInfo[PlyChnIndex].u32ChnID = u32PlayChnID;
    PvrPlayChannelInfo[PlyChnIndex].bIsTimeshiftPlay = stPlayInfo.bIsTimeshiftPlay;
    PvrPlayChannelInfo[PlyChnIndex].u32RecChnID = stPlayInfo.u32RecChnID;

    /*Every time start pvr play, we need reset the value of track key*/
    memset(PvrPlayChannelInfo[PlyChnIndex].PvrPlayTrackKey, 0x0, 16);

    PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer = malloc(MAX_PVR_KEY_BLOCK_NUMBERS*sizeof(HI_CA_PVR_KEY_INFO_S));
    if ( PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer == HI_NULL)
    {
       HI_ERR_CA("error,Fail to malloc memory for key\n");
       return HI_FAILURE;
    }

    pstPvrPlyCaData = &PvrPlayChannelInfo[PlyChnIndex].stPvrPlyCaData;
    memset(pstPvrPlyCaData,0,sizeof(HI_CA_PVR_CADATA_S));

    pstPvrPlyCaData->stR2RkeyladderCipherInfo.hCipherHandle = INVALID_VALUE;

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    CA_ASSERT(HI_UNF_CIPHER_CreateHandle(&pstPvrPlyCaData->stR2RkeyladderCipherInfo.hCipherHandle, &stCipherAttr),ret);

    PvrPlayChannelInfo[PlyChnIndex].bAlreadyGetPlayInfo = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_PlayClose(HI_U32 u32PlayChnID)
{
    HI_U8 i = 0,  PlyChnIndex=0;
    HI_S32 ret = 0;
    HI_CA_PVR_CADATA_S * pstPvrPlyCaData = NULL;
    HI_HANDLE hCipher = INVALID_VALUE;

    /*find out the PVR CA infor by PVR channel ID*/
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32PlayChnID == PvrPlayChannelInfo[i].u32ChnID )
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("PVR channel ID error\n");
        return HI_FAILURE;
    }
    PlyChnIndex = i;

    if (PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer != HI_NULL)
    {
       free(PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer);
       PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer = HI_NULL;
    }

    pstPvrPlyCaData = &PvrPlayChannelInfo[PlyChnIndex].stPvrPlyCaData;

    hCipher = pstPvrPlyCaData->stR2RkeyladderCipherInfo.hCipherHandle;
    memset(pstPvrPlyCaData,0x00,sizeof(HI_CA_PVR_CADATA_S));
    PvrPlayChannelInfo[PlyChnIndex].u32ChnID = INVALID_VALUE;
    PvrPlayChannelInfo[PlyChnIndex].bIsTimeshiftPlay = HI_FALSE;
    PvrPlayChannelInfo[PlyChnIndex].u32RecChnID = INVALID_VALUE;

    if (INVALID_VALUE != hCipher)
    {
        CA_ASSERT(HI_UNF_CIPHER_DestroyHandle(hCipher),ret);
    }

    return  HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_WriteCallBack(HI_UNF_PVR_CA_CALLBACK_ARGS_S* pstCAPVRArgs)
{
    HI_S32 ret = 0;
    HI_U32 i = 0, RecChnIndex=0, CurBlockNum=0;
    HI_BOOL isGenerateKey = HI_FALSE;
    HI_U8 CipherKey[16];
    HI_CA_PVR_KEY_INFO_S stNewKey;
    HI_U32 u32RecChnID = pstCAPVRArgs->u32ChnID;
    HI_CHAR * pFileName = pstCAPVRArgs->pFileName;
    HI_U64 u64DataStart = pstCAPVRArgs->u64GlobalOffset;
    HI_U32 u32PhyAddr = pstCAPVRArgs->u32PhyAddr;
    HI_U32 u32DataSize = pstCAPVRArgs->u32DataSize;
    HI_U64 u64DataEnd = u64DataStart + u32DataSize;
    HI_U64 u64BlockSize = PVR_BLOCK_SIZE;
    HI_UNF_CIPHER_CTRL_S *pstCipherCtrl = NULL;
    HI_CA_PVR_CADATA_S * pstPvrRecCaData = NULL;
    HI_UNF_R2RKeyladder_Cipher_Info_S * pstR2RkeyladderCipherInfo = NULL;
    HI_CHAR CaDataFileName[128] = {0};
    HI_S32 fd = -1;

    CA_CheckPointer(pstCAPVRArgs);
    CA_CheckPointer(pstCAPVRArgs->pFileName);

    if ((u32DataSize%16) != 0)
    {
        HI_INFO_CA("crypted data is not 16 bytes alignment\n");
    }

    /*find out the PVR CA infor by PVR channel ID*/
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            break;
        }
    }
    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("PVR channel ID error\n");
        return HI_FAILURE;
    }
    RecChnIndex = i;
    //HI_INFO_CA("HI_UNF_ADVCA_PVR_WriteCallBack: block size is lld------>\n", u64BlockSize);

    memset(CipherKey, 0, sizeof(CipherKey));
    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;

    pstR2RkeyladderCipherInfo = &(pstPvrRecCaData->stR2RkeyladderCipherInfo);
    pstCipherCtrl = &(pstR2RkeyladderCipherInfo->stCipherCtrl);

    if (INVALID_VALUE == pstR2RkeyladderCipherInfo->hCipherHandle)
    {
        HI_ERR_CA("hCipherHandle = -1 , call HI_UNF_ADVCA_PVR_Open first \n");
        return HI_FAILURE;
    }

    if (HI_FALSE == PvrRecChannelInfo[RecChnIndex].bCADataSavedFlag)
    {
        /*First, create key file*/
        memset(CaDataFileName, 0x0, sizeof(CaDataFileName));
        ret = ADVCA_PVR_GetCADataFileName(pFileName,CaDataFileName);
        if (HI_FAILURE == ret)
        {
            HI_ERR_CA("error, Get ca key file failed!\n");
            return HI_FAILURE;
        }

        fd = open(CaDataFileName,O_RDWR|O_CREAT, 0666);
        if (fd < 0)
        {
            HI_ERR_CA("error,open ca key file failed!\n");
            return HI_FAILURE;
        }
        close(fd);

        PvrRecChannelInfo[RecChnIndex].bCADataSavedFlag = HI_TRUE;
        memset(pstPvrRecCaData->FileName,0,sizeof(pstPvrRecCaData->FileName));
        memcpy(pstPvrRecCaData->FileName,pFileName,strlen(pFileName));

        stNewKey.u32BlockNum = 0;
        stNewKey.u64OffsetForKey = 0UL;
        memcpy(stNewKey.Cipherkey, pstCipherCtrl->u32Key, 16);
        CA_ASSERT(ADVCA_PVR_SetCAData(pFileName,pstPvrRecCaData, &stNewKey),ret);
        PvrRecChannelInfo[RecChnIndex].PvrRecBlockDataAcc = u64DataStart;
    }

    /*HI_INFO_CA("\n HI_UNF_ADVCA_PVR_WriteCallBack: u64DataStart=%lld u64DataEnd=%lld u32DataSize=%d current block number=%d------>\n", \
                  u64DataStart, \
                  u64DataEnd, \
                  u32DataSize, \
                  PvrRecChannelInfo[RecChnIndex].CurBlockNum);*/

    if (PvrRecChannelInfo[RecChnIndex].PvrRecForceChangeKey == HI_TRUE)
    {
        HI_INFO_CA("detect PvrRecForceChangeKey\n");
        isGenerateKey = HI_TRUE;
        PvrRecChannelInfo[RecChnIndex].PvrRecForceChangeKey = HI_FALSE;
    }
    else
    {
        PvrRecChannelInfo[RecChnIndex].PvrRecBlockDataAcc += u32DataSize;

        if (PvrRecChannelInfo[RecChnIndex].PvrRecBlockDataAcc > u64BlockSize)
        {
           isGenerateKey = HI_TRUE;
        }
    }

    CurBlockNum = PvrRecChannelInfo[RecChnIndex].CurBlockNum;

    if (isGenerateKey == HI_TRUE)
    {
        CurBlockNum = CurBlockNum+1;

        /*generate the random cipher key*/
        for(i=0;i<16;i++)
        {
            CipherKey[i] = new_rand();
        }
        if (CipherKey[0] == CipherKey[8])
        {
            CipherKey[0] = CipherKey[8] + 1;
        }

        memcpy(pstCipherCtrl->u32Key,CipherKey,16);
        HI_INFO_CA("config key ladder in block %d!\n", CurBlockNum);
        CA_ASSERT(HI_UNF_ADVCA_ConfigR2RKeyladderAndCipher(pstR2RkeyladderCipherInfo),ret);

        PvrRecChannelInfo[RecChnIndex].CurBlockNum = CurBlockNum;
        /*update track key value to avoid config keyladder key more times*/
        memcpy(PvrRecChannelInfo[RecChnIndex].PvrRecTrackKey, CipherKey,16);

        pstPvrRecCaData->u32TotalBlockNum++;
        stNewKey.u32BlockNum = CurBlockNum;
        stNewKey.u64OffsetForKey = u64DataStart;
        memcpy(stNewKey.Cipherkey,CipherKey,16);

        CA_ASSERT(ADVCA_PVR_SetCAData(pFileName,pstPvrRecCaData, &stNewKey),ret);

        PvrRecChannelInfo[RecChnIndex].PvrRecBlockDataAcc = 0;
    }

    if ((u32DataSize%16) != 0)
    {
       HI_INFO_CA("data is not 16bytes align\n");
       u32DataSize = u32DataSize - (u32DataSize%16);
    }

    CA_ASSERT(HI_UNF_CIPHER_Encrypt(pstR2RkeyladderCipherInfo->hCipherHandle, u32PhyAddr, u32PhyAddr, u32DataSize),ret);

    PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc = PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc + u32DataSize;
    if (PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc > PVR_CA_VERIFY_LEN_DELTA)
    {
        /*We save end data into u64CxVerifiedLen to be convient to calculate u64CxVerifiedLen when play back.*/
        pstPvrRecCaData->u64CxVerifiedLen = u64DataEnd;
        PvrRecChannelInfo[RecChnIndex].PvrRecVerifyLenAcc = 0;
        CA_ASSERT(ADVCA_PVR_SetCAData(pFileName,pstPvrRecCaData, HI_NULL),ret);
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_ReadCallBack(HI_UNF_PVR_CA_CALLBACK_ARGS_S* pstCAPVRArgs)
{
    HI_U32 i = 0, PlyChnIndex=0, CurBlockNum=0, NextBlockNum=0;
    HI_S32 ret = 0;
    HI_U32 u32PlayChnID = pstCAPVRArgs->u32ChnID;
    HI_CHAR *pFileName = pstCAPVRArgs->pFileName;
    HI_U64 u64DataStart = pstCAPVRArgs->u64GlobalOffset;
    HI_U32 u32PhyAddr = pstCAPVRArgs->u32PhyAddr;
    HI_U32 u32DataSize = pstCAPVRArgs->u32DataSize;
    HI_U64 u64DataEnd = u64DataStart + u32DataSize;
    HI_BOOL bIsCrossKeyBlock = HI_FALSE;
    HI_U32 u32DataSize1 = 0;
    HI_U32 u32DataSize2 = 0;
    HI_UNF_CIPHER_CTRL_S *pstCipherCtrl = NULL;
    HI_UNF_R2RKeyladder_Cipher_Info_S *pstR2RkeyladderCipherInfo = NULL;
    HI_CA_PVR_CADATA_S *pstPvrPlyCaData  = NULL;
    HI_CA_PVR_CADATA_S  stTmpPvrPlyPrivateInfo;
    HI_CA_PVR_KEY_INFO_S stBlockKey;
    HI_U8   *pu8KeyBuffer = HI_NULL;
    HI_U32  u32RecChnIndex = 0, u32TotalBlockNum = 0;
    HI_BOOL bIsTimeshiftPlay = HI_FALSE;

    CA_CheckPointer(pstCAPVRArgs);
    CA_CheckPointer(pstCAPVRArgs->pFileName);

    if ((u32DataSize%16) != 0)
    {
         HI_INFO_CA("HI_UNF_ADVCA_PVR_ReadCallBack: decrypted data is not 16 bytes alignment ----\n");
    }

    /*find out the PVR CA channel*/
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32PlayChnID == PvrPlayChannelInfo[i].u32ChnID )
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,not enough play channel to use \n");
        HI_ERR_CA("in HI_UNF_ADVCA_PVR_ReadCallBack u32PlayChnID = %d\n", u32PlayChnID);
        return HI_FAILURE;
    }

    PlyChnIndex = i;

    if (PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer == HI_NULL)
    {
        HI_ERR_CA("HI_UNF_ADVCA_PVR_ReadCallBack: key buffer is NULL\n");
        return HI_SUCCESS;
    }

    pstPvrPlyCaData = &PvrPlayChannelInfo[PlyChnIndex].stPvrPlyCaData;

    pstR2RkeyladderCipherInfo = &(pstPvrPlyCaData->stR2RkeyladderCipherInfo);
    pstCipherCtrl = &(pstR2RkeyladderCipherInfo->stCipherCtrl);

    memset(&stTmpPvrPlyPrivateInfo, 0, sizeof(HI_CA_PVR_CADATA_S));

    if (HI_FALSE == PvrPlayChannelInfo[PlyChnIndex].bAlreadyGetPlayInfo)
    {
        CA_ASSERT(ADVCA_PVR_GetCAData(pFileName,&stTmpPvrPlyPrivateInfo, PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer),ret);
        stTmpPvrPlyPrivateInfo.stR2RkeyladderCipherInfo.hCipherHandle = pstPvrPlyCaData->stR2RkeyladderCipherInfo.hCipherHandle;
        memcpy(pstPvrPlyCaData, &stTmpPvrPlyPrivateInfo, sizeof(HI_CA_PVR_CADATA_S));

        pu8KeyBuffer = PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer;
        memcpy(&stBlockKey, pu8KeyBuffer, sizeof(HI_CA_PVR_KEY_INFO_S));
        memcpy(pstCipherCtrl->u32Key, stBlockKey.Cipherkey,16);
        PvrPlayChannelInfo[PlyChnIndex].bAlreadyGetPlayInfo = HI_TRUE;
        PvrPlayChannelInfo[PlyChnIndex].CurBlockNum = 0;
    }

    /*HI_INFO_CA("\nHI_UNF_ADVCA_PVR_ReadCallBack: u64DataStart=%lld u64DataEnd=%lld u32DataSize=%d current BlockNum=%d------>\n", \
                   u64DataStart, \
                   u64DataEnd, \
                   u32DataSize, \
                   PvrPlayChannelInfo[PlyChnIndex].CurBlockNum);*/

    ADVCA_PVR_LOCK();
    if (PvrPlayChannelInfo[PlyChnIndex].bIsTimeshiftPlay == HI_TRUE)
    {
        for (i = 0; i < PVR_CA_MAX_CHANNEL; i++)
        {
            if (PvrPlayChannelInfo[PlyChnIndex].u32RecChnID == PvrRecChannelInfo[i].u32ChnID)
            {
                break;
            }
        }
        if (PVR_CA_MAX_CHANNEL == i)
        {
            HI_ERR_CA("PVR channel ID error\n");
            ADVCA_PVR_UNLOCK();
            return HI_FAILURE;
        }
        bIsTimeshiftPlay = HI_TRUE;
        u32RecChnIndex = i;
        u32TotalBlockNum = PvrRecChannelInfo[u32RecChnIndex].stPvrRecCaData.u32TotalBlockNum;
    }
    ADVCA_PVR_UNLOCK();

    if (bIsTimeshiftPlay == HI_TRUE)
    {
        if (pstPvrPlyCaData->u32TotalBlockNum < u32TotalBlockNum)
        {
           /*re-acquire key into buffer*/
           if (HI_SUCCESS != ADVCA_PVR_GetCAData(pFileName,&stTmpPvrPlyPrivateInfo, PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer))
           {
               HI_ERR_CA("HI_UNF_ADVCA_PVR_ReadCallBack: Fail to call ADVCA_PVR_GetCAData\n");
               return HI_FAILURE;
           }
           if (stTmpPvrPlyPrivateInfo.u32TotalBlockNum > MAX_PVR_KEY_BLOCK_NUMBERS)
           {
               HI_ERR_CA("HI_UNF_ADVCA_PVR_ReadCallBack: key block number exceed %d\n", MAX_PVR_KEY_BLOCK_NUMBERS);
               return HI_FAILURE;
           }

           stTmpPvrPlyPrivateInfo.stR2RkeyladderCipherInfo.hCipherHandle = pstPvrPlyCaData->stR2RkeyladderCipherInfo.hCipherHandle;
           memcpy(pstPvrPlyCaData, &stTmpPvrPlyPrivateInfo, sizeof(HI_CA_PVR_CADATA_S));
        }
    }
    else
    {
        if (u64DataEnd > pstPvrPlyCaData->u64CxVerifiedLen)
        {
            HI_ERR_CA("HI_UNF_ADVCA_PVR_ReadCallBack: play length is great than verify length, \
                      u64DataEnd=%llu u64CxVerifiedLen=%llu\n", u64DataEnd, pstPvrPlyCaData->u64CxVerifiedLen);
            return HI_SUCCESS;
        }
    }

    //HI_INFO_CA("HI_UNF_ADVCA_PVR_ReadCallBack: pstPvrPlyCaData->u32TotalBlockNum = %d \n", pstPvrPlyCaData->u32TotalBlockNum);
    for (i=0; i<pstPvrPlyCaData->u32TotalBlockNum; i++)
    {
        pu8KeyBuffer = PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer + i*sizeof(HI_CA_PVR_KEY_INFO_S);
        memcpy(&stBlockKey, pu8KeyBuffer, sizeof(HI_CA_PVR_KEY_INFO_S));
        //HI_INFO_CA("HI_UNF_ADVCA_PVR_ReadCallBack: u64DataStart = 0x%x stBlockKey.u64OffsetForKey=0x%x\n", u64DataStart, stBlockKey.u64OffsetForKey);
        if (u64DataStart < stBlockKey.u64OffsetForKey)
        {
            break;
        }
    }

    if (i == 0)
    {
        HI_ERR_CA("in HI_UNF_ADVCA_PVR_ReadCallBack: Fail to find key\n");
        return HI_FAILURE;
    }

    CurBlockNum = i-1;

    if (CurBlockNum < (pstPvrPlyCaData->u32TotalBlockNum-1)
       && u64DataEnd > stBlockKey.u64OffsetForKey)
    {
       HI_WARN_CA("HI_UNF_ADVCA_PVR_ReadCallBack: cross block in %d!!!!!!!!!!!!!!!!!!!!!!!!!!\n", CurBlockNum);
       bIsCrossKeyBlock = HI_TRUE;
    }

    PvrPlayChannelInfo[PlyChnIndex].CurBlockNum = CurBlockNum;

    pu8KeyBuffer = PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer + CurBlockNum*sizeof(HI_CA_PVR_KEY_INFO_S);
    memcpy(&stBlockKey, pu8KeyBuffer, sizeof(HI_CA_PVR_KEY_INFO_S));

    if (0 != memcmp(PvrPlayChannelInfo[PlyChnIndex].PvrPlayTrackKey, stBlockKey.Cipherkey,16))
    {
        HI_WARN_CA("HI_UNF_ADVCA_PVR_ReadCallBack: config_1 key ladder in block %d!!!!!!!!!!!!!!!!!!!!!!!!!!\n", CurBlockNum);
        memcpy(pstCipherCtrl->u32Key, stBlockKey.Cipherkey, 16);

        CA_ASSERT(HI_UNF_ADVCA_ConfigR2RKeyladderAndCipher(pstR2RkeyladderCipherInfo),ret);
        /*update track key value to avoid config keyladder key more times*/
        memcpy(PvrPlayChannelInfo[PlyChnIndex].PvrPlayTrackKey,stBlockKey.Cipherkey,16);
    }

    if(bIsCrossKeyBlock == HI_FALSE)
    {
        CA_ASSERT(HI_UNF_CIPHER_Decrypt(pstR2RkeyladderCipherInfo->hCipherHandle, u32PhyAddr, u32PhyAddr, u32DataSize),ret);
    }
    else
    {
        NextBlockNum = CurBlockNum+1;
        pu8KeyBuffer = PvrPlayChannelInfo[PlyChnIndex].pu8KeyBuffer + NextBlockNum*sizeof(HI_CA_PVR_KEY_INFO_S);
        memcpy(&stBlockKey, pu8KeyBuffer, sizeof(HI_CA_PVR_KEY_INFO_S));

        u32DataSize1 = (HI_U32)(stBlockKey.u64OffsetForKey - u64DataStart);
        if ((u32DataSize1%16) != 0)
        {
           HI_WARN_CA("HI_UNF_ADVCA_PVR_ReadCallBack: data 1 is not 16bytes align!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
           u32DataSize1 = u32DataSize1 + (16 - u32DataSize1%16);
        }
        CA_ASSERT(HI_UNF_CIPHER_Decrypt(pstR2RkeyladderCipherInfo->hCipherHandle, u32PhyAddr, u32PhyAddr, u32DataSize1),ret);

        /*Process part2 data*/
        PvrPlayChannelInfo[PlyChnIndex].CurBlockNum = NextBlockNum;

        memcpy(pstCipherCtrl->u32Key,stBlockKey.Cipherkey,16);
        /*update track key value to avoid config keyladder key more times*/
        memcpy(PvrPlayChannelInfo[PlyChnIndex].PvrPlayTrackKey, stBlockKey.Cipherkey,16);

        HI_WARN_CA("HI_UNF_ADVCA_PVR_ReadCallBack: config_2 key ladder in block %d!!!!!!!!!!!!!!!!!!!!!!!!!!\n", NextBlockNum);
        /*every time when cipher key changing, should use mutex and set keyladder and cipher again*/
        CA_ASSERT(HI_UNF_ADVCA_ConfigR2RKeyladderAndCipher(pstR2RkeyladderCipherInfo),ret);

        if ((u32DataSize1 + INT_MIN) > u32DataSize)
        {
            return HI_FAILURE;
        }
        else
        {
            u32DataSize2 = u32DataSize - u32DataSize1;
        }
        if ((u32DataSize2%16) != 0)
        {
           HI_WARN_CA("HI_UNF_ADVCA_PVR_ReadCallBack: data 2 is not 16bytes align!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
           u32DataSize2 = u32DataSize2 - (u32DataSize2%16);
        }

        if (u32DataSize2 > 0)
        {
            ret = HI_UNF_CIPHER_Decrypt(pstR2RkeyladderCipherInfo->hCipherHandle, u32PhyAddr + u32DataSize1, u32PhyAddr + u32DataSize1, u32DataSize2);
            if (ret)
            {
                HI_ERR_CA("error,HI_UNF_CIPHER_Decrypt failed ,hCipherHandle = 0x%x\n",pstR2RkeyladderCipherInfo->hCipherHandle);
                return HI_FAILURE;
            }
        }
    }

    return  HI_SUCCESS;
}
#endif

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DCASOpen
*  Description:    Open DCAS Mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_E enAlg)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((enAlg != HI_UNF_CIPHER_ALG_3DES) && (enAlg != HI_UNF_CIPHER_ALG_AES))
    {
        HI_ERR_CA("enAlg must be HI_UNF_CIPHER_ALG_3DES or HI_UNF_CIPHER_ALG_AES\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DCAS_OPEN, &enAlg);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DCAS_OPEN err. \n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetDCASChipId
*  Description:    Get DCAS CHIPID
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetDCASChipId(HI_U32 *pu32MSBID, HI_U32 *pu32LSBID)
{
    HI_S32 Ret = 0;
    HI_U32 u32DCASID_MSBAddr, u32DCASID_LSBAddr;
    HI_U32 u32StbSN0, u32StbSN1;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if((pu32MSBID == NULL) || (pu32LSBID == NULL))
    {
        HI_ERR_CA("Invalid parameter!\r\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    u32DCASID_MSBAddr = 0xF8AB00E8;
    u32DCASID_LSBAddr = 0xF8AB00EC;

    Ret = HI_SYS_ReadRegister(u32DCASID_MSBAddr, &u32StbSN0);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("Get Dcas chipid MSB failed\n");
        *pu32MSBID = 0X00;
        *pu32LSBID = 0X00;
        return HI_FAILURE;
    }

    Ret = HI_SYS_ReadRegister(u32DCASID_LSBAddr, &u32StbSN1);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("Get Dcas chipid LSB failed\n");
        *pu32MSBID = 0X00;
        *pu32LSBID = 0X00;
        return HI_FAILURE;
    }

    *pu32MSBID = u32StbSN0;
    *pu32LSBID = u32StbSN1;

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_DCASClose
*  Description:    Close DCAS Mode
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_DCASClose(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_DCAS_CLOSE, 0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DCAS_CLOSE err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetDCASSessionKey
*  Description:    set DCAS Session Key
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:         ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_LEV_E enDCASLevel, HI_U8 au8Key[16], HI_U8 au8Output[16])
{
    HI_S32 ret;
    CA_DCAS_PARAM_S DCASParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if ((au8Key == NULL)||(au8Output == NULL))
    {
        HI_ERR_CA("au8Key == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (enDCASLevel >= HI_UNF_ADVCA_DCAS_KEYLADDER_BUTT)
    {
        HI_ERR_CA("enDCASLevel >= HI_UNF_ADVCA_DCAS_KEYLADDER_BUTT, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&DCASParam, 0, sizeof(CA_DCAS_PARAM_S));
    DCASParam.level = enDCASLevel;
    memcpy(DCASParam.pDin, au8Key, 16);

    ret = ioctl(g_s32CaFd, CMD_CA_DCAS_PARAM_ID_SET, &DCASParam);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_DCAS_CRYPT err.\n");
        return ret;
    }

    memcpy(au8Output, DCASParam.pDout, 16);

    return HI_SUCCESS;
}
/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SWPKKeyLadderOpen
*  Description:    Open SWPK keyladder
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SWPKKeyLadderOpen(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SWPK_KEY_LADDER_OPEN, 0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SWPK_KEY_LADDER_OPEN err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SWPKKeyLadderClose
*  Description:   Close SWPK keyladder
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SWPKKeyLadderClose(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_SWPK_KEY_LADDER_CLOSE, 0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SWPK_KEY_LADDER_CLOSE err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_OtpReset
*  Description:   Reset OTP
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_OtpReset(HI_VOID)
{
    HI_S32 ret;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_OTP_RESET, NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_OTP_RESET err.\n");
        return ret;
    }

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_GetRevision
*  Description:   Get the software revision
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         NA
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_GetRevision(HI_U8 u8Revision[25])
{
    HI_S32 ret;
    CA_KEY_S stCaKey;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (HI_NULL == u8Revision)
    {
        HI_ERR_CA("u8Revision == NULL, invalid.\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    ret = ioctl(g_s32CaFd, CMD_CA_GET_REVISION, &stCaKey);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_REVISION err.\n");
        return ret;
    }

    memcpy(u8Revision, stCaKey.KeyBuf, 25);

    return HI_SUCCESS;
}

#ifdef HI_PVR_SUPPORT
HI_S32 HI_UNF_ADVCA_PVR_GetCAPrivateFileName(HI_CHAR * pIndexFileName,HI_CHAR CAPrivateFileName[128])
{
    HI_CHAR   CASuffix[4] = ".ca";
    HI_CHAR *p;

    CA_CheckPointer(pIndexFileName);
    CA_CheckPointer(CAPrivateFileName);

    p = strrchr(pIndexFileName,'.');
    if(NULL != p)
    {
        memset(CAPrivateFileName, 0, 128);
        memcpy(CAPrivateFileName,pIndexFileName,(HI_U32)(p - pIndexFileName));
        memcpy(CAPrivateFileName + (HI_U32)(p - pIndexFileName),CASuffix,3);
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}


HI_S32 HI_UNF_ADVCA_PVR_GetCAIndexFileName(HI_CHAR * pCAPrivateFileName,HI_CHAR IndexFileName[128])
{
    HI_CHAR   IndexSuffix[5] = ".idx";
    HI_CHAR *p;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(IndexFileName);

    p = strrchr(pCAPrivateFileName,'.');
    if(NULL != p)
    {
        memset(IndexFileName, 0, 128);
        memcpy(IndexFileName,pCAPrivateFileName,(HI_U32)(p - pCAPrivateFileName));
        memcpy(IndexFileName + (HI_U32)(p - pCAPrivateFileName),IndexSuffix,4);
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

HI_S32 HI_UNF_ADVCA_PVR_CreateCAPrivateFile(HI_U32 u32RecChnID,HI_CHAR * pCAPrivateFileName, HI_UNF_ADVCA_Time_S *pCurTime)
{
    HI_S32 ret = 0;
    HI_S32 fd = -1;
    HI_U8 i = 0, RecChnIndex=0;
    HI_U32 u32WriteCount = 0;
    HI_U8 MAC[16];
    HI_CA_PVR_CADATA_S* pstPvrRecCaData = NULL;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = NULL;
    ADVCA_PVR_CA_PrivateFileHead_S* pstPrivateFileHead = NULL;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pCurTime);

    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
        return HI_FAILURE;
    }

    memset(MAC, 0, sizeof(MAC));
    memset((HI_U8 *)pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    pstPrivateFileHead = &(pstMACPrivateFileHead->stCAPrivateFileHead);

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            break;
        }
    }
    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,u32RecChnID = 0x%x have not opened, please call the HI_UNF_ADVCA_PVR_RecOpen first\n", u32RecChnID);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }

    RecChnIndex = i;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;


    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName,O_RDWR|O_CREAT, 0666);
    if ( fd < 0 )
    {
        HI_ERR_CA("error,open pCAPrivateFileName failed!\n");
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }

    //Added ADVCA_PVR_CA_PrivateFileHead_S in front of ca privatedata file
    //Initiaze the header
    memset((HI_U8 *)pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    memcpy(pstPrivateFileHead->MagicNum, CA_PVR_MAGINNUMBER, strlen(CA_PVR_MAGINNUMBER));
    memcpy(&(pstPrivateFileHead->Starttime), pCurTime, sizeof(HI_UNF_ADVCA_Time_S));
    memcpy(pstPrivateFileHead->MacKey,pstPvrRecCaData->stR2RkeyladderCipherInfo.SessionKey1,16);

    ret = HI_UNF_ADVCA_CalculteAES_CMAC((HI_U8 *)pstPrivateFileHead, sizeof(ADVCA_PVR_CA_PrivateFileHead_S), pstPrivateFileHead->MacKey, MAC);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");
        close(fd);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    memcpy(pstMACPrivateFileHead->Mac, MAC, 16);
    u32WriteCount = write(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if(u32WriteCount != sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S))
    {
        HI_ERR_CA("write mac private file head error\n");
        close(fd);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    close(fd);

    memset(pstPvrRecCaData->CAPrivateFileName,0x0, sizeof(pstPvrRecCaData->CAPrivateFileName));
    memcpy(pstPvrRecCaData->CAPrivateFileName,pCAPrivateFileName, strlen(pCAPrivateFileName));

    PvrRecChannelInfo[RecChnIndex].bCAPrivateFileCreated = HI_TRUE;

    free(pstMACPrivateFileHead);

    return HI_SUCCESS;
}

static HI_S32 ADVCA_PVR_CheckCAPrivateFileCreated(HI_U32 u32RecChnID)
{
    HI_S32 fd = -1;
    HI_U8 i = 0, RecChnIndex=0;

    HI_CA_PVR_CADATA_S* pstPvrRecCaData = NULL;

    /*find out the PVR CA infor by PVR channel ID*/
    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            break;
        }
    }
    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,u32RecChnID = 0x%x have not opened, please call the HI_UNF_ADVCA_PVR_RecOpen first\n", u32RecChnID);
        return HI_FAILURE;
    }

    RecChnIndex = i;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;
    if (HI_FALSE == PvrRecChannelInfo[RecChnIndex].bCAPrivateFileCreated)
    {
       HI_ERR_CA("CA private file has not been created,error!\n");
       return HI_FAILURE;
    }

    /*read private file to get the file head*/
    CA_CheckPointer(pstPvrRecCaData);
    fd = open(pstPvrRecCaData->CAPrivateFileName,O_RDWR);
    if (-1 == fd)
    {
        HI_ERR_CA("CA private file open error!\n");
        return HI_FAILURE;
    }
    close(fd);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_CheckCAPrivateFileMAC( HI_CHAR * pCAPrivateFileName, HI_UNF_ADVCA_Time_S *pOutRecordStartTime)
{
    HI_S32 ret = 0;
    HI_S32 fd = -1;
    HI_U32 u32ReadCount = 0, u32DataOffset;
    HI_U8  *pu8Buf = NULL;
    HI_U32 u32DataLen=0;
    HI_U32 u32tmp = 0;
    HI_U32 u32Seek = 0;
    HI_U8 MAC[16] = {0};
    HI_CHAR IdxFileName[128] = {0};
    HI_U32 u32MacBufLen = 0;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = HI_NULL;
    HI_CA_PVR_CADATA_S   *pstTmpPvrPrivateInfo = HI_NULL;
    ADVCA_PVR_CA_PrivateFileHead_S* pstPrivateFileHead = HI_NULL;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pOutRecordStartTime);

    memset(MAC, 0, sizeof(MAC));
    memset(IdxFileName, 0, sizeof(IdxFileName));
    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
        ret = HI_FAILURE;
        goto RET;
    }
    memset(pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));

    pstTmpPvrPrivateInfo = (HI_CA_PVR_CADATA_S *)malloc(sizeof(HI_CA_PVR_CADATA_S));
    if (NULL == pstTmpPvrPrivateInfo)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(HI_CA_PVR_CADATA_S));
        ret = HI_FAILURE;
        goto RET;
    }
    memset(pstTmpPvrPrivateInfo, 0, sizeof(HI_CA_PVR_CADATA_S));

    ret = HI_UNF_ADVCA_PVR_GetCAIndexFileName(pCAPrivateFileName,IdxFileName);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("Failed to Get Ca Index File Name\n");
        ret = HI_FAILURE;
        goto RET;
    }

    ret = ADVCA_PVR_GetCAData(IdxFileName,pstTmpPvrPrivateInfo, NULL);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("Failed to Get Ca Data\n");
        ret = HI_FAILURE;
        goto RET;
    }

    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName,O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("%s is not exist\n",pCAPrivateFileName);
        ret = HI_FAILURE;
        goto RET;
    }

    u32ReadCount = (HI_U32)read(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (u32ReadCount != sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S))
    {
        HI_ERR_CA("get CAPrivatehead error\n");
        ret = HI_FAILURE;
        goto RET;
    }

    pstPrivateFileHead = &(pstMACPrivateFileHead->stCAPrivateFileHead);
    if (0 != memcmp(pstPrivateFileHead->MacKey, pstTmpPvrPrivateInfo->stR2RkeyladderCipherInfo.SessionKey1, 16))
    {
        HI_ERR_CA("error :CA Private file and index file not match\n");
        ret = HI_FAILURE;
        goto RET;
    }

    HI_INFO_CA("HI_UNF_ADVCA_PVR_CheckCAPrivateFileMAC:pstPrivateFileHead->FPNum is %d------>\n", pstPrivateFileHead->FPNum);

    if(pstPrivateFileHead->FPNum > MAX_FP_NUM)
    {
        HI_ERR_CA("error : pstPrivateFileHead->FPNum:0x%x > MAX_FP_NUM:0x%x\n", pstPrivateFileHead->FPNum, MAX_FP_NUM);
        ret = HI_FAILURE;
        goto RET;
    }

    u32tmp = pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S);

    u32MacBufLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + u32tmp;
    pu8Buf = (HI_U8 *)malloc(u32MacBufLen);
    if (pu8Buf == HI_NULL)
    {
        HI_ERR_CA("error, fail to malloc memory!\n");
        ret = HI_FAILURE;
        goto RET;
    }

    memcpy(pu8Buf, &(pstMACPrivateFileHead->stCAPrivateFileHead), sizeof(ADVCA_PVR_CA_PrivateFileHead_S));
    u32DataOffset = sizeof(ADVCA_PVR_CA_PrivateFileHead_S);

    u32DataLen = u32tmp;
    if (u32DataLen > 0)
    {
        u32Seek = sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S);
        if (u32Seek != lseek(fd, (long)u32Seek, SEEK_SET))
        {
            HI_ERR_CA("lseek error\n");
            ret = HI_FAILURE;
            goto RET;
        }

        if(u32DataLen > (u32MacBufLen - u32DataOffset))
        {
            HI_ERR_CA("error, u32DataLen:0x%x > u32MacBufLen:0x%x!\n", u32DataLen, u32MacBufLen);
            ret = HI_FAILURE;
            goto RET;
        }

        u32ReadCount = read(fd, &pu8Buf[u32DataOffset], u32DataLen);
        if (u32ReadCount != u32DataLen)
        {
            HI_ERR_CA("error, u32ReadCount:0x%x != u32DataLen:0x%x!\n", u32ReadCount, u32DataLen);
            ret = HI_FAILURE;
            goto RET;
        }
    }

    u32DataLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + u32DataLen;

    if(u32DataLen > u32MacBufLen)
    {
        HI_ERR_CA("Invalid data length, u32MacBufLen:0x%x < u32DataLen:0x%x\n", u32MacBufLen, u32DataLen);
        ret = HI_FAILURE;
        goto RET;
    }

    ret = HI_UNF_ADVCA_CalculteAES_CMAC(pu8Buf, u32DataLen, pstPrivateFileHead->MacKey, MAC);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");
        ret = HI_FAILURE;
        goto RET;
    }

    if (memcmp(MAC,pstMACPrivateFileHead->Mac,16))
    {
        HI_ERR_CA("call MACPrivateFileHead check mac error\n");
        ret = HI_FAILURE;
        goto RET;
    }

    HI_INFO_CA("CheckCAPrivateFileMAC OK\n");

    //URI will be check in upper lib
    //Get the RecordBegin Time
    memcpy(pOutRecordStartTime, &(pstPrivateFileHead->Starttime), sizeof(HI_UNF_ADVCA_Time_S));
    ret = HI_SUCCESS;

RET:
    if (fd != -1)
    {
        close(fd);
    }
    if (pu8Buf != HI_NULL)
    {
        free(pu8Buf);
    }
    if (pstTmpPvrPrivateInfo != HI_NULL)
    {
        free(pstTmpPvrPrivateInfo);
    }
    if (pstMACPrivateFileHead != HI_NULL)
    {
        free(pstMACPrivateFileHead);
    }
 
    return ret;
}

HI_S32 HI_UNF_ADVCA_PVR_SaveURI( HI_U32 u32RecChnID,HI_CHAR * pCAPrivateFileName ,HI_UNF_PVR_URI_S* pstPVRURI)
{
    HI_S32 ret = 0;
    HI_U32 i = 0, RecChnIndex=0;
    HI_S32 fd = -1;
    HI_S32 len;
    HI_U32 u32WriteCount = 0, u32DataOffset;
    HI_U8  *pu8Buf = NULL;
    HI_U32 u32DataLen=0;
    HI_U32 u32Seek = 0;
    HI_U8 MAC[16];
    HI_U32 u32MacBufLen = 0;
    HI_CA_PVR_CADATA_S* pstPvrRecCaData = NULL;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = HI_NULL;
    ADVCA_PVR_CA_PrivateFileHead_S* pstPrivateFileHead = HI_NULL;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pstPVRURI);

    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
        return HI_FAILURE;
    }
    pstPrivateFileHead = &(pstMACPrivateFileHead->stCAPrivateFileHead);

    /*find out the PVR CA infor by PVR channel ID*/
    for (i = 0; i < PVR_CA_MAX_CHANNEL; i++)
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID)
        {
            break;
        }
    }

    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("PVR channel ID error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    RecChnIndex = i;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;
    memset(MAC, 0, sizeof(MAC));
    memset(pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));

    ret = ADVCA_PVR_CheckCAPrivateFileCreated(u32RecChnID);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("error, Create CA Private File failed!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName, O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("error, open pCAPrivateFileName failed!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    len = read(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) != len)
    {
        ret = HI_FAILURE;

        goto RET;
    }

    if (pstPrivateFileHead->URINum >= MAX_URI_NUM)
    {
        ret = HI_FAILURE;

        goto RET;
    }

    HI_INFO_CA("FPNum is %d\n", pstPrivateFileHead->FPNum);
    if(pstPrivateFileHead->FPNum > MAX_FP_NUM)
    {
        ret = HI_FAILURE;

        goto RET;
    }

    u32MacBufLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S);
    pu8Buf = malloc(u32MacBufLen);
    if (pu8Buf == HI_NULL)
    {
        HI_ERR_CA("error, fail to malloc memory!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    /*Below code save URI and caculate the MAC*/

    memcpy((HI_U8*)&pstPrivateFileHead->URI[pstPrivateFileHead->URINum],pstPVRURI,sizeof(HI_UNF_PVR_URI_S));
    pstPrivateFileHead->URINum++;

    /*copy updated private head into buffer*/
    memcpy(pu8Buf, &(pstMACPrivateFileHead->stCAPrivateFileHead), sizeof(ADVCA_PVR_CA_PrivateFileHead_S));
    u32DataOffset = sizeof(ADVCA_PVR_CA_PrivateFileHead_S);

    /*read Fingerpint into buffer*/
    u32DataLen = (HI_U32)(pstPrivateFileHead->FPNum*sizeof(ADVCA_PVR_MAC_FP_INFO_S));
    if (u32DataLen > 0)
    {
       u32Seek = sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S);
       if (u32Seek != lseek(fd, (long)u32Seek, SEEK_SET))
       {
           HI_ERR_CA("lseek error\n");
       
           ret = HI_FAILURE;
       
           goto RET;
       }
       if (u32DataLen != read(fd, &pu8Buf[u32DataOffset], u32DataLen))
       {
           HI_ERR_CA("read error\n");
       
           ret = HI_FAILURE;
       
           goto RET;
       }
    }

    /*Now, we calculate CMAC involving private head and fingerprint*/
    u32DataLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + u32DataLen;

    if (u32DataLen > u32MacBufLen)
    {
        HI_ERR_CA("Error, u32DataLen:0x%x > u32MacBufLen:0x%x\n", u32DataLen, u32MacBufLen);

        ret = HI_FAILURE;

        goto RET;
    }

    ret = HI_UNF_ADVCA_CalculteAES_CMAC(pu8Buf, u32DataLen, pstPvrRecCaData->stR2RkeyladderCipherInfo.SessionKey1, MAC);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    memcpy(pstMACPrivateFileHead->Mac, MAC, 16);

    if (0 != lseek(fd, 0, SEEK_SET))
    {
        HI_ERR_CA("lseek error\n");
    
        ret = HI_FAILURE;
    
        goto RET;
    }

    u32WriteCount = (HI_U32)write(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) != u32WriteCount)
    {
        HI_ERR_CA("write CA private file error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    /*Only force change Record key from second URI*/
    if (pstPrivateFileHead->URINum > 1)
    {
       PvrRecChannelInfo[RecChnIndex].PvrRecForceChangeKey = HI_TRUE;
    }

    PvrRecChannelInfo[RecChnIndex].u32UriNumbers++;

    ret = HI_SUCCESS;

RET:
    if (fd >= 0)
    {
        close(fd);
    }

    if (HI_NULL != pu8Buf)
    {
        free(pu8Buf);
    }

    if (HI_NULL != pstMACPrivateFileHead)
    {
        free(pstMACPrivateFileHead);
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_PVR_SaveFP(HI_U32 u32RecChnID, HI_CHAR *pCAPrivateFileName, HI_UNF_PVR_FP_INFO_S *pstFPInfo)
{
    HI_S32 ret = 0;
    HI_U32 i = 0, RecChnIndex=0;
    HI_S32 fd = -1;
    HI_S32 len;
    HI_U32 u32Seek = 0;
    HI_U32 u32WriteCount = 0;
    HI_U32 u32DataLen = 0, u32DataOffset;
    HI_U8  *pu8Buf = HI_NULL;
    HI_U8  HeadMAC[16];
    HI_U8  FPMAC[16];
    HI_CA_PVR_CADATA_S* pstPvrRecCaData = NULL;
    HI_U32 u32MacBufLen = 0;
    ADVCA_PVR_MAC_FP_INFO_S *pstTmpMACFP = HI_NULL;
    HI_UNF_CIPHER_CTRL_S *pstCipherCtrl = NULL;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = HI_NULL;
    ADVCA_PVR_CA_PrivateFileHead_S* pstPrivateFileHead = HI_NULL;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pstFPInfo);

    pstTmpMACFP = (ADVCA_PVR_MAC_FP_INFO_S *)malloc(sizeof(ADVCA_PVR_MAC_FP_INFO_S));
    if (HI_NULL == pstTmpMACFP)
    {
        HI_ERR_CA("Failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_MAC_FP_INFO_S));
        return HI_FAILURE;
    }

    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (HI_NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("Failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));

        ret = HI_FAILURE;

        goto RET;
    }

    /*find out the PVR CA infor by PVR channel ID*/
    for (i = 0; i < PVR_CA_MAX_CHANNEL; i++)
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID)
        {
            break;
        }
    }
    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("PVR channel ID error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    RecChnIndex = i;

    pstPvrRecCaData = &PvrRecChannelInfo[RecChnIndex].stPvrRecCaData;

    memset(HeadMAC, 0, sizeof(HeadMAC));
    memset(FPMAC, 0, sizeof(FPMAC));
    memset(pstTmpMACFP, 0, sizeof(ADVCA_PVR_MAC_FP_INFO_S));
    memset(pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    pstPrivateFileHead = &pstMACPrivateFileHead->stCAPrivateFileHead;

    ret = ADVCA_PVR_CheckCAPrivateFileCreated(u32RecChnID);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("error, create Private File failed!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    fd = open(pCAPrivateFileName, O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("error, open pCAPrivateFileName failed!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    len = read(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) != len)
    {
        ret = HI_FAILURE;

        goto RET;
    }

    if (pstPrivateFileHead->FPNum > MAX_FP_NUM)
    {
        HI_ERR_CA("FPNum %d is error\n", pstPrivateFileHead->FPNum);

        ret = HI_FAILURE;

        goto RET;
    }

    u32MacBufLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + (pstPrivateFileHead->FPNum + 1) * sizeof(ADVCA_PVR_MAC_FP_INFO_S);

    pu8Buf = malloc(u32MacBufLen);
    if (pu8Buf == HI_NULL)
    {
        HI_ERR_CA("error, fail to malloc memory!\n");

        ret = HI_FAILURE;

        goto RET;
    }

    if (pstPrivateFileHead->FPNum > 0)
    {
        len = read(fd, pu8Buf + sizeof(ADVCA_PVR_CA_PrivateFileHead_S), pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S));
        if (len != pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S))
        {
            ret = HI_FAILURE;

            goto RET;
        }
    }

    u32DataOffset = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S);

    /*save FP and caculate the MAC*/
    memcpy(&pstTmpMACFP->stPVRFPInfo, pstFPInfo, sizeof(HI_UNF_PVR_FP_INFO_S));

    pstCipherCtrl = &pstPvrRecCaData->stR2RkeyladderCipherInfo.stCipherCtrl;

    ret = HI_UNF_ADVCA_CalculteAES_CMAC((HI_U8*)&pstTmpMACFP->stPVRFPInfo, sizeof(HI_UNF_PVR_FP_INFO_S), (HI_U8*)pstCipherCtrl->u32Key, FPMAC);

    memcpy(pstTmpMACFP->Mac, FPMAC, 16);

    memcpy(pu8Buf + u32DataOffset, pstTmpMACFP, sizeof(ADVCA_PVR_MAC_FP_INFO_S));

    pstPrivateFileHead->FPNum++;

    /*we should copy head after update pstPrivateFileHead->FPNum*/
    memcpy(&pu8Buf[0], &pstMACPrivateFileHead->stCAPrivateFileHead, sizeof(ADVCA_PVR_CA_PrivateFileHead_S));

    u32DataLen = sizeof(ADVCA_PVR_CA_PrivateFileHead_S) + pstPrivateFileHead->FPNum * sizeof(ADVCA_PVR_MAC_FP_INFO_S);
    if (u32DataLen > u32MacBufLen)
    {
        HI_ERR_CA("Error, u32DataLen:0x%x, u32MacBufLen:0x%x\n", u32DataLen, u32MacBufLen);
        pstPrivateFileHead->FPNum--;

        ret = HI_FAILURE;

        goto RET;
    }

    ret |= HI_UNF_ADVCA_CalculteAES_CMAC(pu8Buf, u32DataLen, pstPvrRecCaData->stR2RkeyladderCipherInfo.SessionKey1, HeadMAC);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("call HI_UNF_ADVCA_CalculteAES_CMAC error\n");
        pstPrivateFileHead->FPNum--;

        ret = HI_FAILURE;

        goto RET;
    }

    memcpy(pstMACPrivateFileHead->Mac,HeadMAC,16);

    /*save new Fingerpint*/
    u32Seek = sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) + (pstPrivateFileHead->FPNum - 1) * sizeof(ADVCA_PVR_MAC_FP_INFO_S);

    if (u32Seek != lseek(fd, (long)u32Seek, SEEK_SET))
    {
        HI_ERR_CA("lseek error\n");

        ret = HI_FAILURE;

        goto RET;
    }
    u32WriteCount = (HI_U32)write(fd, pstTmpMACFP,sizeof(ADVCA_PVR_MAC_FP_INFO_S));
    if (sizeof(ADVCA_PVR_MAC_FP_INFO_S) != u32WriteCount)
    {
        HI_ERR_CA("write CA private file error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    /*save private head*/
    if (0 != lseek(fd, 0, SEEK_SET))
    {
        HI_ERR_CA("lseek error\n");

        ret = HI_FAILURE;

        goto RET;
    }
    u32WriteCount = (HI_U32)write(fd,pstMACPrivateFileHead,sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) != u32WriteCount)
    {
        HI_ERR_CA("write CA private file error\n");

        ret = HI_FAILURE;

        goto RET;
    }

    PvrRecChannelInfo[RecChnIndex].u32FpNumbers++;

    ret = HI_SUCCESS;

RET:
    if (fd >= 0)
    {
        close(fd);
    }

    if (HI_NULL != pu8Buf)
    {
        free(pu8Buf);
    }

    if (HI_NULL != pstTmpMACFP)
    {
        free(pstTmpMACFP);
    }

    if (HI_NULL != pstMACPrivateFileHead)
    {
        free(pstMACPrivateFileHead);
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_PVR_GetURIAndFPNum(HI_CHAR *pCAPrivateFileName, HI_U32 *u32URINum, HI_U32 *u32FPNum)
{
    HI_S32 fd = -1;
    HI_U32 u32ReadCount = 0;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = HI_NULL;
    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(u32FPNum);
    CA_CheckPointer(u32URINum);

    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
        return HI_FAILURE;
    }

    memset(pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));

    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName,O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("%s is not exist\n",pCAPrivateFileName);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    u32ReadCount = (HI_U32)read(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (u32ReadCount != sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S))
    {
        HI_ERR_CA("get CAPrivatehead error\n");
        close(fd);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    *u32FPNum = pstMACPrivateFileHead->stCAPrivateFileHead.FPNum;
    *u32URINum = pstMACPrivateFileHead->stCAPrivateFileHead.URINum;

    close(fd);
    free(pstMACPrivateFileHead);
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_GetFP( HI_CHAR * pCAPrivateFileName,HI_U32 u32FPNum,HI_UNF_PVR_FP_INFO_S* pstFPInfo)
{
    HI_S32 ret = 0;
    HI_S32 fd = -1;
    HI_U32 u32ReadCount = 0;

    HI_U32 u32Seek = 0;

    HI_U32 u32FPTotalNum = 0;
    HI_U32 u32URITotalNum = 0;
    ADVCA_PVR_MAC_FP_INFO_S stTmpMacFPInfo;

    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pstFPInfo);

    memset(&stTmpMacFPInfo, 0, sizeof(ADVCA_PVR_MAC_FP_INFO_S));

    CA_ASSERT(HI_UNF_ADVCA_PVR_GetURIAndFPNum(pCAPrivateFileName,&u32URITotalNum,&u32FPTotalNum),ret);

    if (u32FPNum == 0)
    {
        HI_ERR_CA("u32FPNum = %d error,must > 0\n",u32FPNum);
        return HI_FAILURE;
    }
    if (u32FPNum > u32FPTotalNum)
    {
        HI_ERR_CA("u32FPNum = %d  big than u32FPTotalNum = %d,error\n",u32FPNum,u32FPTotalNum);
        return HI_FAILURE;
    }

    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName,O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("%s is not exist\n",pCAPrivateFileName);
        return HI_FAILURE;
    }


    u32Seek = sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S) + (u32FPNum - 1) * sizeof(ADVCA_PVR_MAC_FP_INFO_S);
    if (u32Seek != lseek(fd, (long)u32Seek, SEEK_SET))
    {
        HI_ERR_CA("lseek error\n");
        close(fd);
        return HI_FAILURE;
    }
    u32ReadCount = (HI_U32)read(fd,&stTmpMacFPInfo,sizeof(ADVCA_PVR_MAC_FP_INFO_S));
    if (u32ReadCount != sizeof(ADVCA_PVR_MAC_FP_INFO_S))
    {
        HI_ERR_CA("get FP error\n");
        close(fd);
        return HI_FAILURE;
    }
    memcpy(pstFPInfo,&stTmpMacFPInfo.stPVRFPInfo,sizeof(HI_UNF_PVR_FP_INFO_S));

    close(fd);
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_GetURI( HI_CHAR * pCAPrivateFileName ,HI_U32 u32URINum, HI_UNF_PVR_URI_S* pstURI)
{
    HI_S32 ret = 0;
    HI_S32 fd = -1;
    HI_U32 u32ReadCount = 0;
    HI_U32 u32FPTotalNum = 0;
    HI_U32 u32URITotalNum = 0;
    ADVCA_PVR_CA_MacPrivateFileHead_S *pstMACPrivateFileHead = HI_NULL;
    ADVCA_PVR_CA_PrivateFileHead_S* pstPrivateFileHead = HI_NULL;
    CA_CheckPointer(pCAPrivateFileName);
    CA_CheckPointer(pstURI);

    pstMACPrivateFileHead = (ADVCA_PVR_CA_MacPrivateFileHead_S *)malloc(sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (NULL == pstMACPrivateFileHead)
    {
        HI_ERR_CA("error: failed to malloc %d bytes memory\n", sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
        return HI_FAILURE;
    }
    memset(pstMACPrivateFileHead, 0, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    pstPrivateFileHead = &(pstMACPrivateFileHead->stCAPrivateFileHead);

    ret = HI_UNF_ADVCA_PVR_GetURIAndFPNum(pCAPrivateFileName,&u32URITotalNum,&u32FPTotalNum);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("Failed to Get URI.\n");
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }

    if (u32URINum == 0)
    {
        HI_ERR_CA("u32URINum = %d error,must > 0\n",u32URINum);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    if (u32URINum > u32URITotalNum)
    {
        HI_ERR_CA("u32URINum = %d  big than u32URITotalNum = %d,error\n",u32URINum,u32URITotalNum);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }


    /*read private file to get the file head*/
    fd = open(pCAPrivateFileName,O_RDWR);
    if (fd < 0)
    {
        HI_ERR_CA("%s is not exist\n",pCAPrivateFileName);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    u32ReadCount = (HI_U32)read(fd, pstMACPrivateFileHead, sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S));
    if (u32ReadCount != sizeof(ADVCA_PVR_CA_MacPrivateFileHead_S))
    {
        HI_ERR_CA("get CAPrivatehead error\n");
        close(fd);
        free(pstMACPrivateFileHead);
        return HI_FAILURE;
    }
    memcpy((HI_U8*)pstURI,(HI_U8*)&pstPrivateFileHead->URI[u32URINum - 1],sizeof(HI_UNF_PVR_URI_S));
    free(pstMACPrivateFileHead);
    close(fd);
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_PVR_GetRecStatus(HI_U32 u32RecChnID, HI_UNF_ADVCA_PVR_REC_STATUS_S *pstRecStatus)
{
    HI_U8 i = 0, RecChnIndex=0;

    CA_CheckPointer(pstRecStatus);

    for ( i = 0 ; i < PVR_CA_MAX_CHANNEL ; i++ )
    {
        if (u32RecChnID == PvrRecChannelInfo[i].u32ChnID )
        {
            break;
        }
    }
    if (PVR_CA_MAX_CHANNEL == i)
    {
        HI_ERR_CA("error,u32RecChnID = 0x%x have not opened, please call the HI_UNF_ADVCA_PVR_RecOpen first\n", u32RecChnID);
        return HI_FAILURE;
    }

    RecChnIndex = i;


    pstRecStatus->u32UriNumbers = PvrRecChannelInfo[RecChnIndex].u32UriNumbers;
    pstRecStatus->u32FpNumbers = PvrRecChannelInfo[RecChnIndex].u32FpNumbers;

    return HI_SUCCESS;
}
#endif

HI_S32 HI_UNF_ADVCA_CalculteAES_CMAC(HI_U8 *buffer, HI_U32 Length, HI_U8 Key[16], HI_U8 MAC[16])
{
    HI_S32 ret = 0;
    HI_HANDLE hCipher = 0xffffffff;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_U8 au8SessionInput1[16] = {0x23,0x34,0x34,0x52,0x34,0x55,0x45,0x84,0x54,0x54,0x84,0x53,0x34,0x47,0x34,0x47};
    HI_U8 au8SessionInput2[16] = {0x56,0x34,0x88,0x52,0x34,0x89,0x45,0xa0,0x54,0x54,0x77,0x53,0x34,0x47,0x34,0x91};
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    CA_CheckPointer(buffer);
    CA_CheckPointer(Key);
    CA_CheckPointer(MAC);

    /*open and config cipher*/
    CA_ASSERT(HI_UNF_ADVCA_Init(),ret);
    CA_ASSERT(HI_UNF_CIPHER_Init(),ret);
    CA_ASSERT(HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES), ret);
    CA_ASSERT(HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage), ret);

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    CA_ASSERT(HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAttr),ret);

#ifdef HI_PVR_SUPPORT
    (HI_VOID)sem_wait(&g_PVRSem);
#endif
    if (enStage >= HI_UNF_ADVCA_KEYLADDER_LEV2)
    {
        ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, au8SessionInput1);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CA("Fail to set R2R session key, level %d\n", HI_UNF_ADVCA_KEYLADDER_LEV1);
            goto ERROR_EXIT;
        }
    }

    if (enStage >= HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2, au8SessionInput2);
        if (ret != HI_SUCCESS)
        {
           HI_ERR_CA("Fail to set R2R session key, level %d\n", HI_UNF_ADVCA_KEYLADDER_LEV2);
           goto ERROR_EXIT;
        }
    }

    memset(&CipherCtrl,0x00,sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.stChangeFlags.bit1IV = 1;
    memcpy(CipherCtrl.u32Key, Key, 16);
    memset(CipherCtrl.u32IV, 0x0, 16);

    ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&CipherCtrl);
    if (ret != HI_SUCCESS)
    {
       HI_ERR_CA("Fail to config cipher\n");
       goto ERROR_EXIT;
    }

    ret = HI_UNF_CIPHER_CalcMAC(hCipher, buffer, Length, MAC, HI_TRUE);
    if (ret != HI_SUCCESS)
    {
       HI_ERR_CA("HI_UNF_CIPHER_CalcMAC failed, ret: 0x%x\n", ret);
       goto ERROR_EXIT;
    }

    ret = HI_SUCCESS;

ERROR_EXIT:
#ifdef HI_PVR_SUPPORT
    (HI_VOID)sem_post(&g_PVRSem);
#endif

    if (HI_SUCCESS != HI_UNF_CIPHER_DestroyHandle(hCipher))
    {
       HI_ERR_CA("Fail to destory cipher handle\n");
       return HI_FAILURE;
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_SetKlDPAClkSelEn(HI_BOOL bValue)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bTmp = bValue;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_KL_DPA_CLK_SEL_EN;
    memcpy(stCmdParam.pu8ParamBuf, &bTmp, sizeof(HI_BOOL));
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetMiscRootKey(HI_U8 *pu8Key, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (NULL == pu8Key) || (16 != u32Len) )
    {
        HI_ERR_CA("Invalid param!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_MISC_ROOT_KEY;
    memcpy(stCmdParam.pu8ParamBuf, pu8Key, u32Len);
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetESCK(HI_U8 *pu8Key, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (NULL == pu8Key) || (16 != u32Len) )
    {
        HI_ERR_CA("Invalid param!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_ESCK;
    memcpy(stCmdParam.pu8ParamBuf, pu8Key, u32Len);
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (HI_UNF_ADVCA_KEYLADDER_LEV2 != enLevel) && (HI_UNF_ADVCA_KEYLADDER_LEV3 != enLevel))
    {
        HI_ERR_CA("Invalid keyladder level select! Only supported 2 or 3 level\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_MISC_LV_SEL;
    memcpy(stCmdParam.pu8ParamBuf, &enLevel, sizeof(HI_UNF_ADVCA_KEYLADDER_LEV_E));
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}


HI_S32 HI_UNF_ADVCA_GetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penLevel)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( NULL == penLevel)
    {
        HI_ERR_CA("Invalid param, NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_GET_MISC_LV_SEL;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    memcpy(penLevel, stCmdParam.pu8ParamBuf, sizeof(HI_BOOL));

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetMiscAlg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enType >= HI_UNF_ADVCA_ALG_TYPE_BUTT)
    {
        HI_ERR_CA("Invalid alg type: %d\n", enType);
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_MISC_SETALG;
    memcpy(stCmdParam.pu8ParamBuf, &enType, sizeof(HI_UNF_ADVCA_ALG_TYPE_E));
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetMiscAlg(HI_UNF_ADVCA_ALG_TYPE_E *penType)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_ADVCA_ALG_TYPE_E enType = HI_UNF_ADVCA_ALG_TYPE_BUTT;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if(NULL == penType)
    {
        HI_ERR_CA("Invalid param input ,NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_MISC_GETALG;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    memcpy(&enType, stCmdParam.pu8ParamBuf, sizeof(HI_UNF_ADVCA_ALG_TYPE_E));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    *penType = enType;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetOEMRootKey(HI_U8 *pu8OEMRootKey, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (NULL == pu8OEMRootKey) || (16 != u32KeyLen))
    {
        HI_ERR_CA("Error! Invalid parameter input!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_OEM_ROOT_KEY;
    memcpy(stCmdParam.pu8ParamBuf, pu8OEMRootKey, u32KeyLen);
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetOEMRootKey(HI_U8 *pu8OEMRootKey, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( (NULL == pu8OEMRootKey) || (16 != u32KeyLen))
    {
        HI_ERR_CA("Error! Invalid parameter input!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_GET_OEM_ROOT_KEY;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err.\n");
        return s32Ret;
    }

    memcpy(pu8OEMRootKey, stCmdParam.pu8ParamBuf, u32KeyLen);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetOEMRootKeyLockFlag(HI_BOOL *pbLockFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( NULL == pbLockFlag)
    {
        HI_ERR_CA("Error! Invalid parameter input!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_GET_OEM_ROOT_KEY_LOCK;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err. \n");
        return s32Ret;
    }

    memcpy(pbLockFlag, stCmdParam.pu8ParamBuf, sizeof(HI_BOOL));

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetTZEnable(HI_BOOL bValue)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;
    HI_BOOL bTZEnable = bValue;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_SOC_OTP_TZ_EN;
    memcpy(stCmdParam.pu8ParamBuf, &bTZEnable, sizeof(HI_BOOL));

    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err. \n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetTZEnStatus(HI_BOOL *pbEn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( NULL == pbEn )
    {
        HI_ERR_CA("Invalid param ,NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_GET_SOC_OTP_TZ_EN;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err. \n");
        return s32Ret;
    }

    memcpy(pbEn, stCmdParam.pu8ParamBuf, sizeof(HI_BOOL));

    return HI_SUCCESS;
}

/***********************************************************************************
*  Function:       HI_UNF_ADVCA_SetTZOtp
*  Description:    Set the otp in Trustzone.
*  Calls:
*  Data Accessed:  NA
*  Data Updated:   NA
*  Input:          NA
*  Output:         NA
*  Return:          ErrorCode(reference to document)
*  Others:         The input address should be 16byte aligned!
***********************************************************************************/
HI_S32 HI_UNF_ADVCA_SetTZOtp(HI_U32 u32Addr, HI_U8 *pu8InData, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;
    CA_OTP_TZ_DATA_S stOtpTzInput;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( NULL == pu8InData )
    {
        HI_ERR_CA("Invalid param, null pointer!");
        return HI_FAILURE;
    }
    if((u32Addr % 16) != 0)
    {
        HI_ERR_CA("u32Addr(0x%x) is not aligned by 16!", u32Addr);
        return HI_FAILURE;
    }
    if(u32Len > 128)
    {
        HI_ERR_CA("Invalid param, u32Len(%d) > 128!", u32Len);
        return HI_FAILURE;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    memset(&stOtpTzInput, 0, sizeof(stOtpTzInput));
    stOtpTzInput.u32Addr = u32Addr;
    stOtpTzInput.u32Len = u32Len;
    memcpy(stOtpTzInput.u8Buf, pu8InData, u32Len);

    memcpy(stCmdParam.pu8ParamBuf, &stOtpTzInput, sizeof(CA_OTP_TZ_DATA_S));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_SET_OTP_TZ;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err:0x%x \n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetTZOtp(HI_U32 u32Addr, HI_U32 u32Len, HI_U8 *pu8OutData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stCmdParam;
    CA_OTP_TZ_DATA_S stOtpTzTrans;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if( NULL == pu8OutData )
    {
        HI_ERR_CA("Invalid param, null pointer!");
        return HI_FAILURE;
    }

    if((u32Addr % 16) != 0)
    {
        HI_ERR_CA("u32Addr(0x%x) is not aligned by 16!", u32Addr);
        return HI_FAILURE;
    }
    if(u32Len > 128)
    {
        HI_ERR_CA("Invalid param, u32Len(%d) > 128!", u32Len);
        return HI_FAILURE;
    }

    memset(&stCmdParam, 0, sizeof(stCmdParam));
    memset(&stOtpTzTrans, 0, sizeof(stOtpTzTrans));

    stOtpTzTrans.u32Addr = u32Addr;
    stOtpTzTrans.u32Len = u32Len;

    memcpy(stCmdParam.pu8ParamBuf, &stOtpTzTrans, sizeof(CA_OTP_TZ_DATA_S));
    stCmdParam.enCmdChildID = CMD_CHILD_ID_GET_OTP_TZ;
    s32Ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stCmdParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SUPPER_ID err. \n");
        return s32Ret;
    }

    memcpy(&stOtpTzTrans, stCmdParam.pu8ParamBuf, sizeof(CA_OTP_TZ_DATA_S));
    memcpy(pu8OutData, stOtpTzTrans.u8Buf, u32Len);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_TYPE_E enType, HI_U8 *pu8ParamIn, HI_U32 u32ParamLen)
{
    HI_S32 ret = HI_SUCCESS;
    CA_CMD_SUPPER_ID_S stParams;
    HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S *pstParams = NULL;

    if (g_s32CaFd <= 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    memset(&stParams, 0, sizeof(stParams));
    switch ( enType )
    {
        case HI_UNF_ADVCA_LOCK_RSA_KEY:
        {
            stParams.enCmdChildID = CMD_CHILD_ID_LOCK_RSA_KEY;
            ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stParams);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca ioctl  set rsa key lock flag err. \n");
                return ret;
            }
            break;
        }
        case HI_UNF_ADVCA_LOCK_TZ_OTP:
        {
            if ( (NULL == pu8ParamIn) || (0 == u32ParamLen) || (u32ParamLen != sizeof(HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S)) )
            {
                HI_ERR_CA("Invalid params input.\n");
                return HI_ERR_CA_INVALID_PARA;
            }

            pstParams = (HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S *)pu8ParamIn;

            /* tz otp : 0x100 ~ 0x17f , total length : 128 bytes*/
            if((0 != (pstParams->u32Addr%16)) || (pstParams->u32Addr < 0x100) || (pstParams->u32Addr > 0x170))
            {
                HI_ERR_CA("Invalid params, addr.\n");
                return HI_ERR_CA_INVALID_PARA;
            }

            if ( (0!=(pstParams->u32Len%16)) || (pstParams->u32Len > 128) )
            {
                HI_ERR_CA("Invalid params, u32Len.\n");
                return HI_ERR_CA_INVALID_PARA;
            }
            if ( ((pstParams->u32Addr + pstParams->u32Len) > 0x180) )
            {
                HI_ERR_CA("Invalid params, u32Len.\n");
                return HI_ERR_CA_INVALID_PARA;
            }

            stParams.enCmdChildID = CMD_CHILD_ID_LOCK_OTP_TZ;
            memcpy(stParams.pu8ParamBuf, (HI_VOID *)pstParams, sizeof(HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S));
            stParams.u32ParamLen = u32ParamLen;

            ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stParams);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca ioctl set tz otp lock flag err. \n");
                return ret;
            }
            break;
        }
        case HI_UNF_ADVCA_LOCK_MISC_KL_DISABLE:
        {
            stParams.enCmdChildID = CMD_CHILD_ID_LOCK_MISC_KL_DISABLE;
            ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stParams);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca ioctl lock misc kl disable err. \n");
                return ret;
            }
            break;
        }
        case HI_UNF_ADVCA_LOCK_GG_KL_DISABLE:
        {
            stParams.enCmdChildID = CMD_CHILD_ID_LOCK_GG_KL_DISABLE;
            ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stParams);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca ioctl lock gg kl disable err. \n");
                return ret;
            }
            break;
        }
        case HI_UNF_ADVCA_LOCK_TSKL_CSA3_DISABLE:
        {
            stParams.enCmdChildID = CMD_CHILD_ID_LOCK_TSKL_CSA3_DISABLE;
            ret = ioctl(g_s32CaFd, CMD_CA_SUPPER_ID, &stParams);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca ioctl lock tskl csa3 disable err. \n");
                return ret;
            }
            break;
        }
        default:
        {
            HI_ERR_CA("Not supported.\n");
            return HI_ERR_CA_NOT_SUPPORT;
        }
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_ATTR_S stOtpAttr;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if(enOtpFuse >= HI_UNF_ADVCA_OTP_FUSE_BUTT || enOtpFuse <= HI_UNF_ADVCA_OTP_NULL)
    {
        HI_ERR_CA("Invalid parameter, enOtpFuse = %d\n", enOtpFuse);
        return HI_ERR_CA_INVALID_PARA;
    }

    if(pstOtpFuseAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstOtpFuseAttr = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stOtpAttr, 0, sizeof(stOtpAttr));
    stOtpAttr.enOtpFuseId = enOtpFuse;
    memcpy(&stOtpAttr.stOtpAttr, pstOtpFuseAttr, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    ret = ioctl(g_s32CaFd, CMD_CA_SET_OTP_FUSE, &stOtpAttr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_OTP_FUSE err. \n");
        return ret;
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_ATTR_S stOtpAttr;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if(enOtpFuse >= HI_UNF_ADVCA_OTP_FUSE_BUTT || enOtpFuse <= HI_UNF_ADVCA_OTP_NULL)
    {
        HI_ERR_CA("Invalid parameter, enOtpFuse = %d\n", enOtpFuse);
        return HI_ERR_CA_INVALID_PARA;
    }

    if(pstOtpFuseAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstOtpFuseAttr == NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stOtpAttr, 0, sizeof(stOtpAttr));
    stOtpAttr.enOtpFuseId = enOtpFuse;
    ret = ioctl(g_s32CaFd, CMD_CA_GET_OTP_FUSE, &stOtpAttr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_GET_OTP_FUSE err. \n");
        return ret;
    }
    memcpy(pstOtpFuseAttr, &stOtpAttr.stOtpAttr, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));

    return ret;
}

HI_S32 HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType, HI_UNF_ADVCA_KEYLADDER_ATTR_S *pstKeyladderAttr)
{
    HI_S32 ret = HI_SUCCESS;
    CA_KEYLADDER_ATTR_S stKeyladderAttr;

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if(enKeyLadderType >= HI_UNF_ADVCA_KEYLADDER_TYPE_BUTT)
    {
        HI_ERR_CA("Invalid parameter, enKeyLadderType = %d\n", enKeyLadderType);
        return HI_ERR_CA_INVALID_PARA;
    }

    if(pstKeyladderAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstKeyladderAttr = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /**The LPK Protected Data is decrypted in the UNF layer**/
    if(enKeyLadderType == HI_UNF_ADVCA_KEYLADDER_LP &&
        pstKeyladderAttr->unKlAttr.stLpKlAttr.enLPKlAttr == HI_UNF_ADVCA_KEYLADDER_LP_ATTR_DECRYPT_PROTECTED_DATA)
    {
        ret = HI_UNF_ADVCA_DecryptLptParam(pstKeyladderAttr->unKlAttr.stLpKlAttr.pu8ProtectedData,
                                                                             pstKeyladderAttr->unKlAttr.stLpKlAttr.u32ProtectedDataLen,
                                                                             pstKeyladderAttr->unKlAttr.stLpKlAttr.pu8OutputProtectedData);
        return ret;
    }

    memset(&stKeyladderAttr, 0, sizeof(stKeyladderAttr));
    stKeyladderAttr.enKeyLadderType = enKeyLadderType;
    memcpy(&stKeyladderAttr.stKeyladderAttr, pstKeyladderAttr, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    ret = ioctl(g_s32CaFd, CMD_CA_SET_KEYLADDER_ATTR, &stKeyladderAttr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("ca ioctl CMD_CA_SET_KEYLADDER_ATTR err:0x%x\n", ret);
        return ret;
    }
#if !defined(CHIP_TYPE_hi3798mv100) \
 && !defined(CHIP_TYPE_hi3796mv100) \
 && !defined(CHIP_TYPE_hi3716dv100) \
 && defined(HI_ADVCA_TYPE_VERIMATRIX)
    if (HI_UNF_ADVCA_KEYLADDER_GDRM == enKeyLadderType)
    {
        if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT == pstKeyladderAttr->unKlAttr.stGDRMAttr.enGDRMKlAttr)
        {
            memcpy(pstKeyladderAttr->unKlAttr.stGDRMAttr.au8Output, stKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.au8Output, 16);
        }
        else if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_GETFLAG == pstKeyladderAttr->unKlAttr.stGDRMAttr.enGDRMKlAttr)
        {
            memcpy(pstKeyladderAttr->unKlAttr.stGDRMAttr.au8Output, stKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.au8Output, 4);
        }
    }
#endif
    return ret;
}


HI_S32 HI_UNF_ADVCA_CaVendorOperation(HI_UNF_ADVCA_VENDORID_E enCaVendor, HI_UNF_ADVCA_CA_VENDOR_OPT_S *pstCaVendorOpt)
{
    HI_S32 ret = HI_SUCCESS;
    CA_VENDOR_OPT_S CaVendorOpt;
    HI_UNF_ADVCA_CA_VENDOR_OPT_E enCaVendorOpt;
    HI_U8 *pu8RefCbcMac = NULL;
    HI_U32 u32AppLen;
    HI_UNF_CIPHER_HASH_ATTS_S stCipherHashAttr;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_U8 u8CbcMac[16] = {0};

    if (g_AdvcaInitCounter < 0)
    {
        HI_ERR_CA("ca not init\n");
        return HI_ERR_CA_NOT_INIT;
    }

    if (enCaVendor >= HI_UNF_ADVCA_VENDORIDE_BUTT)
    {
        HI_ERR_CA("Invalid parameter, enCaVendor = %d\n", enCaVendor);
        return HI_ERR_CA_INVALID_PARA;
    }

    if (pstCaVendorOpt == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstCaVendorOpt = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    enCaVendorOpt = pstCaVendorOpt->enCaVendorOpt;
    if (enCaVendor == HI_UNF_ADVCA_IRDETO && enCaVendorOpt == HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CBCMAC_CALC)
    {
        stCipherHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC;
        ret = HI_UNF_CIPHER_HashInit(&stCipherHashAttr, &hHandle);
        ret |= HI_UNF_CIPHER_HashUpdate(hHandle, pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.pu8InputData,
                                                                                    pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen);
        ret |= HI_UNF_CIPHER_HashFinal(hHandle, u8CbcMac);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("CBC-MAC calculation error!\n");
            return ret;
        }
        memcpy(pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.au8OutputCBCMAC, u8CbcMac,  16);
    }
    else if (enCaVendor == HI_UNF_ADVCA_IRDETO && enCaVendorOpt == HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CBCMAC_AUTH)
    {
        pu8RefCbcMac = pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.au8RefCBCMAC;
        u32AppLen = pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32AppLen;
        ret = HI_MPI_CIPHER_CbcMac_Auth(pu8RefCbcMac, u32AppLen);

        stCipherHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC;
        ret |= HI_UNF_CIPHER_HashInit(&stCipherHashAttr, &hHandle);
        ret |= HI_UNF_CIPHER_HashUpdate(hHandle, pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.pu8InputData,
                                                 pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen);
        ret |= HI_UNF_CIPHER_HashFinal(hHandle, u8CbcMac);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("CBC-MAC authentication error! ret = 0x%x\n", ret);
            return ret;
        }
    }
    else if(enCaVendor == HI_UNF_ADVCA_VERIMATRIX && enCaVendorOpt == HI_UNF_ADVCA_CA_VENDOR_OPT_VMX_GET_RNG)
    {
        HI_U32 u32RandomNumber = 0;

        ret = HI_UNF_CIPHER_Init();
        ret |= HI_MPI_CIPHER_GetRandomNumber(&u32RandomNumber, 0);
        if (HI_SUCCESS != ret && HI_ERR_CIPHER_NO_AVAILABLE_RNG != ret)
        {
            HI_ERR_CA("HI_MPI_CIPHER_GetRandomNumber error! ret = 0x%x\n", ret);
            return ret;
        }
        else if(HI_ERR_CIPHER_NO_AVAILABLE_RNG == ret)
        {
            return ret;
        }

        pstCaVendorOpt->unCaVendorOpt.stVMXRng.u32RNG = u32RandomNumber;
    }
    else
    {
        memset(&CaVendorOpt, 0, sizeof(CaVendorOpt));
        CaVendorOpt.enCaVendor = enCaVendor;
        memcpy(&CaVendorOpt.stCaVendorOpt, pstCaVendorOpt, sizeof(HI_UNF_ADVCA_CA_VENDOR_OPT_S));
        ret = ioctl(g_s32CaFd, CMD_CA_SET_CAVENDOR_OPT, &CaVendorOpt);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("ca ioctl CMD_CA_SET_CAVENDOR_OPT err. \n");
            return ret;
        }
        memcpy(pstCaVendorOpt, &CaVendorOpt.stCaVendorOpt, sizeof(HI_UNF_ADVCA_CA_VENDOR_OPT_S));
    }

    return ret;
}

/*--------------------------------END--------------------------------------*/

