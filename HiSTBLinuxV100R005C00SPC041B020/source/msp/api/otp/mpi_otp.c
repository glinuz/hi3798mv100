/******************************************************************************

  Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.

******************************************************************************
    File Name   : mpi_otp.c
    Version       : Initial Draft
    Author        : Hisilicon multimedia software group
    Created      : 2012/02/29
    Description :
    History        :
    1.Date        : 2012/02/29
        Author    : c00209458
        Modification: Created file
******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#if defined(CHIP_TYPE_hi3716mv310)
#include "drv_struct_ext.h"
#else
#include "hi_drv_struct.h"
#endif
#include "drv_otp_ioctl.h"
#include "hi_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

static HI_S32 g_OtpDevFd    =   -1;
static HI_S32 g_OtpOpenCnt  =   0;
static pthread_mutex_t   g_OtpMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_OTP_LOCK()    (HI_VOID)pthread_mutex_lock(&g_OtpMutex);
#define HI_OTP_UNLOCK()  (HI_VOID)pthread_mutex_unlock(&g_OtpMutex);

#define CHECK_OTP_INIT()\
do{\
    HI_OTP_LOCK();\
    if (g_OtpDevFd < 0)\
    {\
        HI_ERR_OTP("OTP is not init.\n");\
        HI_OTP_UNLOCK();\
        return HI_FAILURE;\
    }\
    HI_OTP_UNLOCK();\
}while(0)

HI_S32 HI_MPI_OTP_Init(HI_VOID)
{
    HI_OTP_LOCK();

    if (-1 != g_OtpDevFd)
    {
        g_OtpOpenCnt++;
        HI_OTP_UNLOCK();
        return HI_SUCCESS;
    }

    g_OtpDevFd = open("/dev/"UMAP_DEVNAME_OTP, O_RDWR, 0);
    if ( g_OtpDevFd < 0)
    {
        HI_FATAL_OTP("Open OTP ERROR.\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    g_OtpOpenCnt++;
    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_DeInit(HI_VOID)
{
    HI_OTP_LOCK();

    if ( g_OtpDevFd < 0)
    {
        HI_OTP_UNLOCK();
        return HI_SUCCESS;
    }

    g_OtpOpenCnt--;
    if ( 0 == g_OtpOpenCnt)
    {
        close(g_OtpDevFd);
        g_OtpDevFd = -1;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_SetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen)
{
    HI_S32 Ret = HI_SUCCESS;
    OTP_CUSTOMER_KEY_TRANSTER_S stCustomerKeyTransfer;

    if (HI_NULL == pKey)
    {
        HI_ERR_OTP("Null ptr for otp writes\n");
        return HI_FAILURE;
    }

    if( OTP_CUSTOMER_KEY_LEN !=  u32KeyLen )
    {
       HI_ERR_OTP("Invalid customer key length!\n");
       return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    memset(&stCustomerKeyTransfer, 0x00, sizeof(stCustomerKeyTransfer));
    memcpy((HI_U8 *)stCustomerKeyTransfer.stkey.u32CustomerKey, pKey, u32KeyLen);
    stCustomerKeyTransfer.u32KeyLen = u32KeyLen;

    /* Write Customer Key */
    Ret = ioctl(g_OtpDevFd, CMD_OTP_SETCUSTOMERKEY, &stCustomerKeyTransfer);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("Failed to write otp\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_GetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen)
{
    HI_S32 Ret = HI_SUCCESS;
    OTP_CUSTOMER_KEY_TRANSTER_S stCustomerKeyTransfer;

    if (HI_NULL == pKey)
    {
        HI_ERR_OTP("Null ptr for otp read\n");
        return HI_FAILURE;
    }

    if( OTP_CUSTOMER_KEY_LEN !=  u32KeyLen )
    {
       HI_ERR_OTP("Invalid customer key length!\n");
       return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    memset(&stCustomerKeyTransfer, 0x00, sizeof(stCustomerKeyTransfer));
    stCustomerKeyTransfer.u32KeyLen = u32KeyLen;

    Ret = ioctl(g_OtpDevFd, CMD_OTP_GETCUSTOMERKEY, &stCustomerKeyTransfer);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("Failed to read customer key\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    memcpy(pKey, stCustomerKeyTransfer.stkey.u32CustomerKey, u32KeyLen);

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_IsDDPLUSSupport(HI_BOOL* pbDDPLUSFlag)
{
    HI_S32 Ret = HI_SUCCESS;

    if(HI_NULL == pbDDPLUSFlag)
    {
        HI_ERR_OTP("NULL pointer!\n");
        return HI_FAILURE;
    }

    *pbDDPLUSFlag = HI_FALSE;

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    Ret = ioctl(g_OtpDevFd, CMD_OTP_GETDDPLUSFLAG, pbDDPLUSFlag);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("Failed to get dolby_en flag in otp!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_IsDTSSupport(HI_BOOL* pbDTSFlag)
{
    HI_S32 Ret = HI_SUCCESS;

    if(HI_NULL == pbDTSFlag)
    {
        HI_ERR_OTP("NULL pointer!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    *pbDTSFlag = HI_FALSE;

    /* Read Audio support flag */
    Ret = ioctl(g_OtpDevFd, CMD_OTP_GETDTSFLAG, pbDTSFlag);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("failed to get dts_en flag!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data)
{
    HI_S32 Ret = HI_SUCCESS;
    OTP_STB_PRIV_DATA_S OtpStbPrivData;

    if (u32Offset >= 16) //should be less than 16
    {
        HI_ERR_OTP("u32Offset (%d) invalid!\n", u32Offset);
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&OtpStbPrivData, 0, sizeof(OTP_STB_PRIV_DATA_S));
    OtpStbPrivData.u32Offset = u32Offset;
    OtpStbPrivData.u8Data = u8Data;

    Ret = ioctl(g_OtpDevFd, CMD_OTP_SETSTBPRIVDATA, &OtpStbPrivData);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("Failed to write stb private data\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data)
{
    HI_S32 Ret = HI_SUCCESS;
    OTP_STB_PRIV_DATA_S OtpStbPrivData;

    if (HI_NULL == pu8Data)
    {
        HI_ERR_OTP("Null ptr for otp read\n");
        return HI_FAILURE;
    }

    if (u32Offset >= 16) //should be less than 16
    {
        HI_ERR_OTP("u32Offset (%d) invalid!\n", u32Offset);
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&OtpStbPrivData, 0, sizeof(OTP_STB_PRIV_DATA_S));
    OtpStbPrivData.u32Offset = u32Offset;

    Ret = ioctl(g_OtpDevFd, CMD_OTP_GETSTBPRIVDATA, &OtpStbPrivData);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_OTP("Failed to read stb private data\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }
    *pu8Data = OtpStbPrivData.u8Data;

    HI_OTP_UNLOCK();
    return Ret;
}

HI_S32 HI_MPI_OTP_WriteHdcpRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_HDCP_ROOT_KEY_S stHdcpRootKey;


    if ( NULL == pu8Key)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    if ( OTP_HDCP_ROOT_KEY_LEN != u32KeyLen )
    {
        HI_ERR_OTP("Invalid Input Key Length!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&stHdcpRootKey, 0, sizeof(OTP_HDCP_ROOT_KEY_S));
    memcpy(stHdcpRootKey.u8Key, pu8Key, OTP_HDCP_ROOT_KEY_LEN);
    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_WRITEHDCPROOTKEY, &stHdcpRootKey);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to burn hdcp root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_ReadHdcpRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_HDCP_ROOT_KEY_S stOtpHdcpRootKey;

    if ( NULL == pu8Key)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    if ( OTP_HDCP_ROOT_KEY_LEN != u32KeyLen )
    {
        HI_ERR_OTP("Invalid Input Key Length!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    memset(&stOtpHdcpRootKey, 0, sizeof(OTP_HDCP_ROOT_KEY_S));
    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_READHDCPROOTKEY, &stOtpHdcpRootKey);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read hdcp root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    memcpy(pu8Key, stOtpHdcpRootKey.u8Key, OTP_HDCP_ROOT_KEY_LEN);

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_LockHdcpRootKey(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_LOCKHDCPROOTKEY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to lock hdcp root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_GetHdcpRootKeyLockFlag(HI_BOOL *pbKeyLockFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if ( NULL == pbKeyLockFlag)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_GETHDCPROOTKEYLOCKFLAG, pbKeyLockFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to get hdcp root key lock flag!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_WriteStbRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_STB_ROOT_KEY_S stStbRootKey;

    if ( NULL == pu8Key)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    if ( OTP_STB_ROOT_KEY_LEN != u32KeyLen )
    {
        HI_ERR_OTP("Invalid Input Key Length!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&stStbRootKey, 0, sizeof(OTP_STB_ROOT_KEY_S));
    memcpy(stStbRootKey.u8Key, pu8Key, OTP_STB_ROOT_KEY_LEN);

    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_WRITESTBROOTKEY, &stStbRootKey);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to burn STB root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_ReadStbRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_STB_ROOT_KEY_S stStbRootKey;

    if ( NULL == pu8Key)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    if ( OTP_STB_ROOT_KEY_LEN != u32KeyLen )
    {
        HI_ERR_OTP("Invalid Input Key Length!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    HI_OTP_LOCK();

    memset(&stStbRootKey, 0, sizeof(OTP_STB_ROOT_KEY_S));
    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_READSTBROOTKEY, &stStbRootKey);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read stb root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    memcpy(pu8Key, stStbRootKey.u8Key, OTP_STB_ROOT_KEY_LEN);

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_LockStbRootKey(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_LOCKSTBROOTKEY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to lock stb root key!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_GetStbRootKeyLockFlag(HI_BOOL *pbKeyLockFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if ( NULL == pbKeyLockFlag)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_GETSTBROOTKEYLOCKFLAG, pbKeyLockFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to get stb root key lock flag!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_WriteByte(HI_U32 u32Addr, HI_U8 u8Value)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_ENTRY_S stOtpEntry;

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&stOtpEntry, 0, sizeof(OTP_ENTRY_S));
    stOtpEntry.Addr = u32Addr;
    stOtpEntry.Value = (HI_U32)u8Value;
    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_WRITE_BYTE, &stOtpEntry);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to write OTP!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_Read(HI_U32 u32Addr, HI_U32 *pu32Value)
{
    HI_S32 s32Ret = HI_SUCCESS;
    OTP_ENTRY_S stOtpEntry;

    if ( NULL == pu32Value)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();
    HI_OTP_LOCK();

    memset(&stOtpEntry, 0, sizeof(OTP_ENTRY_S));
    stOtpEntry.Addr = u32Addr;
    s32Ret = ioctl(g_OtpDevFd, CMD_OTP_READ, &stOtpEntry);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read OTP!\n");
        HI_OTP_UNLOCK();
        return HI_FAILURE;
    }

    *pu32Value = stOtpEntry.Value;

    HI_OTP_UNLOCK();
    return HI_SUCCESS;
}


#define OTP_INTERNAL_DATALOCK_0         (0x10)
#define OTP_INTERNAL_PVLOCK_1           (0x0C)
#define OTP_ADVCA_ID_WORD_ADDR          (0xa8)
#define ADVCA_ID_WORD                   (0x6EDBE953)
#define OTP_INTERNAL_PV_1               (0x04)
#define OTP_RIGHT_CTRL_EN_ADDR          (0x19)
#define OTP_SCS_EN_BAK_ADDR             (0xad)
#define OTP_JTAG_MODE_BAK_ADDR          (0xae)
#define OTP_RIGHT_CTRL_EN_BAK_ADDR      (0xaf)
#define OTP_SELF_BOOT_DIABLE_BAK_ADDR   (0x1c)

HI_S32 HI_MPI_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U32 u32Value = 0;

    if (NULL == pbLockFlag)
    {
        HI_ERR_OTP("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    CHECK_OTP_INIT();

    s32Ret = HI_MPI_OTP_Read(OTP_INTERNAL_DATALOCK_0, &u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read otp!\n");
        return HI_FAILURE;
    }

    if (1 == ((u32Value >> 10) & 0x1))
    {
        *pbLockFlag = HI_TRUE;
    }
    else
    {
        *pbLockFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_LockIDWord(HI_VOID)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_U8 u8Value   = 0;

    CHECK_OTP_INIT();

    s32Ret = HI_MPI_OTP_Read(OTP_INTERNAL_DATALOCK_0, &u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read otp!\n");
        return HI_FAILURE;
    }

    /* Check ID_WORD locked or not */
    u8Value = (u32Value >> 8) & 0x04;
    if (0 == u8Value)
    {
        /* Lock ID_WORD */
        u8Value = ((u32Value >> 8) | 0x04) & 0xff;
        s32Ret = HI_MPI_OTP_WriteByte(OTP_INTERNAL_DATALOCK_0 + 1, u8Value);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OTP("Failed to Lock IDWord!\n");
            return HI_FAILURE;
        }
    }

    /* secure_chip_flag_lock */
    s32Ret = HI_MPI_OTP_Read(OTP_INTERNAL_PVLOCK_1, &u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Failed to read otp!\n");
        return HI_FAILURE;
    }

    /* Check secure_chip_flag locked or not */
    u8Value = (u32Value & 0x1);
    if (0 == u8Value)
    {
        /* Lock secure_chip_flag */
        u8Value = (u32Value | 0x1) & 0xff;
        s32Ret = HI_MPI_OTP_WriteByte(OTP_INTERNAL_PVLOCK_1, u8Value);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_OTP("Write secure_chip_flag_lock failed!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_OTP_BurnToNormalChipset(HI_VOID)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_BOOL bIsIDWordLocked = HI_FALSE;

    CHECK_OTP_INIT();

    s32Ret = HI_MPI_OTP_Read(OTP_ADVCA_ID_WORD_ADDR, &u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Get ADVCA_ID_WORD Error!\n");
        return HI_FAILURE;
    }

    if(ADVCA_ID_WORD == u32Value)
    {
        HI_ERR_OTP("It's secure chipset already, can not burn to normal!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_OTP_GetIDWordLockFlag(&bIsIDWordLocked);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Get ADVCA_ID_WORD Lock Flag Error!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == bIsIDWordLocked)
    {
        return HI_SUCCESS;
    }

    return HI_MPI_OTP_LockIDWord();
}

HI_S32 HI_MPI_OTP_BurnToSecureChipset(HI_VOID)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_BOOL bIsIDWordLocked = HI_FALSE;

    CHECK_OTP_INIT();
    s32Ret = HI_MPI_OTP_GetIDWordLockFlag(&bIsIDWordLocked);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Get ADVCA_ID_WORD Lock Flag Error!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_OTP_Read(OTP_ADVCA_ID_WORD_ADDR, &u32Value);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Get ADVCA_ID_WORD Error!\n");
        return HI_FAILURE;
    }

    if ((HI_TRUE == bIsIDWordLocked) && (ADVCA_ID_WORD == u32Value))
    {
        return HI_SUCCESS;
    }

    if ((HI_TRUE != bIsIDWordLocked) && (ADVCA_ID_WORD == u32Value))
    {
        return HI_MPI_OTP_LockIDWord();
    }

    if ((HI_TRUE == bIsIDWordLocked) && (ADVCA_ID_WORD != u32Value))
    {
        HI_ERR_OTP("ID_WORD has already been loacked to noaml chipset\n");
        return HI_FAILURE;
    }

    /* echo write 0x04 0x01 > /proc/msp/otp //secure_chip_flag */
    s32Ret  = HI_MPI_OTP_WriteByte(OTP_INTERNAL_PV_1, 0x01);
    /* echo write 0x19 0x08 > /proc/msp/otp  //right_ctrl_en */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_RIGHT_CTRL_EN_ADDR, 0x08);

    /*
    echo write 0xa8 0x53 > /proc/msp/otp  //Advca ID_WORD
    echo write 0xa9 0xe9 > /proc/msp/otp  //Advca ID_WORD
    echo write 0xaa 0xdb > /proc/msp/otp  //Advca ID_WORD
    echo write 0xab 0x6e > /proc/msp/otp  //Advca ID_WORD
    */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR,     0x53);
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 1, 0xe9);
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 2, 0xdb);
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 3, 0x6e);

    /* echo write 0xad 0x81 > /proc/msp/otp  //scs_en_bak */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_SCS_EN_BAK_ADDR, 0x81);

    /* echo write 0xae 0x42 > /proc/msp/otp  //jtag_mode_bak */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_JTAG_MODE_BAK_ADDR, 0x42);

    /* echo write 0xaf 0xff > /proc/msp/otp  //right_ctrl_en_bak */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_RIGHT_CTRL_EN_BAK_ADDR, 0xff);

    /* echo write 0x1c 0x11 > /proc/msp/otp  //self_boot_disable_bak */
    s32Ret |= HI_MPI_OTP_WriteByte(OTP_SELF_BOOT_DIABLE_BAK_ADDR, 0x11);

    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("BurnToSecureChipset failed!\n");
        return HI_FAILURE;
    }

    return HI_MPI_OTP_LockIDWord();
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

