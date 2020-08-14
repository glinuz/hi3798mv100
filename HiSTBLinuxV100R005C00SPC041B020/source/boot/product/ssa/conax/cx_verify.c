/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_verify.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-15
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "boot.h"
#include "app.h"
  #ifdef reserve_mem_alloc
  #undef resmemalign
  #undef reserve_mem_alloc
  #endif
#else
#include "exports.h"
#endif
//#include "crc32.h"
#include "hi_drv_pdm.h"
#include "cx_debug.h"
#include "cx_adp.h"
#include "loader_dbinfo.h"
#include "cx_flash.h"
#include "cx_verify_api.h"


#define BOOT_BASIC          "boot"
#define STBID_BASIC         "deviceinfo"
#define LOADER_BASIC        "loader"
#define SYSTEM_BASIC        "system"
#define BASE_BASIC          "baseparam"
#define PQ_BASIC            "pqparam"
#define LOGO_BASIC          "logo"
#define LOADER_INFOBASIC    "loaderdb"

#define CMAC_VALUE_LEN 16
typedef struct hi_CaCMACDataHead_S
{
    HI_U8  au8MagicNumber[32];
    HI_U32 u32Version;
    HI_U32 u32CMACDataLen;                       //Data length
    HI_U32 u32Reserve[6];                        //Reserve for forture
    HI_U8  u8CMACValue[CMAC_VALUE_LEN];
    HI_U8  u8HeaderCMACValue[CMAC_VALUE_LEN];
} HI_CaCMACDataHead_S;


static HI_U32 u32ConfigEnvSize = CONFIG_ENV_SIZE;   //CFG_ENV_SIZE;

extern HI_U8 CA_VENDOR[];
extern HI_U8 *env_ptr;
extern HI_U32 g_EnvBakFlashAddr;
extern HI_U32 g_EnvFlashAddr;

#ifdef HI_MINIBOOT_SUPPORT
extern int run_cmd(char *cmdline, int length);
#else
extern int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
extern void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize);
#endif
extern unsigned long DVB_CRC32(unsigned char *buf, int length);
extern HI_S32 Logo_Main(HI_BOOL bNeedFastplay,HI_PDM_MEMINFO_S *pstMemInfo);


HI_VOID CX_Reset(HI_VOID)
{
#ifndef HI_MINIBOOT_SUPPORT
    HI_INFO_CA(" \n");
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);
    while(1) { };
#else
    reboot();
#endif
}


/*void showtime(HI_U32 n,HI_U32*  oldtime,HI_U32* newtime)
{
    HI_U32 timeuse = 0;
    *newtime = get_timer(0);
    timeuse= (*newtime - *oldtime)/get_tbclk();
    HI_DEBUG_CA("%d\n", n);
    HI_DEBUG_CA(" Time use :%d s\n",timeuse);
    *oldtime = *newtime;
}*/


#ifdef HI_MINIBOOT_SUPPORT
HI_U8* CX_GetEnvPtr(HI_VOID)
{
    char *pEnvbuf;

    getenv_buf(&pEnvbuf, HI_NULL);
    return (HI_U8*)pEnvbuf;
}
#else
HI_U8* CX_GetEnvPtr(HI_VOID)
{
    return (HI_U8*)env_ptr;/*UBOOT use this address as env addr */
}
#endif

HI_VOID CX_SetEnv(HI_CHAR* args)
{
    HI_CHAR* s;
    HI_CHAR buf[4096];
    memset(buf,0,sizeof(buf));
    if((s = getenv(args)) != NULL)
    {
        HI_DEBUG_CA("%s = %s\n",args,s);
        snprintf(buf, sizeof(buf), "%s", s);
        setenv(args,buf);
    }
    return;
}

HI_S32 CX_ReadUpgrdFlag(HI_BOOL *pbNeedUpgrd)
{
    LDDB_LOADER_INFO_S stInfoParam;
    HI_S32 s32Ret = HI_SUCCESS;

    CA_CheckPointer(pbNeedUpgrd);

	memset(&stInfoParam, 0x00, sizeof(stInfoParam));
    s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_CA("Failed to get upgrade parameter info.");
		return s32Ret;
	}

    *pbNeedUpgrd = stInfoParam.stTAGParamData.bNeedUpgrade;


    return s32Ret;
}

HI_S32 CX_nand_env_relocate_spec(HI_U8 *u8EnvAddr)
{
    HI_U8* pGlobalEnvPtr = NULL;
    CA_CheckPointer(u8EnvAddr);

    pGlobalEnvPtr = CX_GetEnvPtr();

    memset(pGlobalEnvPtr, 0, u32ConfigEnvSize);
    memcpy(pGlobalEnvPtr, u8EnvAddr, u32ConfigEnvSize);

    CX_SetEnv("bootargs");
    CX_SetEnv("loaderargs");

#ifndef  HI_ADVCA_FUNCTION_RELEASE
//    CX_SetEnv("bootcmd");
    CX_SetEnv("ipaddr");
    CX_SetEnv("serverip");
    CX_SetEnv("gatewayip");
    CX_SetEnv("netmask");
    CX_SetEnv("ethaddr");
#endif

    return 0;
}


HI_S32 CX_AuthenticateBootPara(HI_VOID)
{
    HI_S32 ret;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_U32 u32EnvRamAddr = 0;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;

    HI_SIMPLEINFO_CA("%s: g_EnvFlashAddr:0x%x\n", __FUNCTION__, g_EnvFlashAddr);

    ret = HI_CX_VerifyFlashPartitionByAddr(g_EnvFlashAddr, u32RamAddr, &stAuthInfo, &u32EnvRamAddr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Authenticate bootargs failed\n");
        return HI_FAILURE;
    }

    CX_nand_env_relocate_spec((HI_U8*)u32EnvRamAddr);

    return HI_SUCCESS;
}


static HI_S32 CX_AuthenticateLogo(HI_U32 StartAddr, HI_U32 *ImgInDDRAddress)
{
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;
    return HI_CX_VerifyFlashPartitionByName(PDM_LOGO_DEF_NAME, StartAddr, &stAuthInfo, ImgInDDRAddress);
}

static HI_S32 CX_AuthenticateBase(HI_U32 StartAddr, HI_U32 *ImgInDDRAddress)
{
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;

    return HI_CX_VerifyFlashPartitionByName(PDM_BASE_DEF_NAME, StartAddr, &stAuthInfo, ImgInDDRAddress);
}

HI_S32 CX_AuthAndShowLogo(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32LogoSize = 0;
    HI_U32 u32BaseSize = 0;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_PDM_MEMINFO_S stPdmInfo;
    HI_Flash_InterInfo_S stLogoInfo;
    HI_Flash_InterInfo_S stBaseInfo;
    HI_U32 u32LogoRamAddress = 0;
    HI_U32 u32BaseRamAddress = 0;
    HI_U32 u32LogoLoc = 0;
    HI_U32 u32BaseLoc = 0;
    HI_U32 u32AllocSize = 0;

    /*Read logo partition*/
    hFlash = HI_Flash_Open(HI_FLASH_TYPE_BUTT, PDM_LOGO_DEF_NAME, 0, 0);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_CA("HI_Flash_Open() %s error, result %x!\n",PDM_LOGO_DEF_NAME, ret);
        return HI_FAILURE;
    }
    ret = HI_Flash_GetInfo(hFlash, &stLogoInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_Flash_GetInfo() error, result %x!\n", ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    u32LogoSize = (HI_U32)stLogoInfo.PartSize;

    u32LogoRamAddress = (HI_U32)reserve_mem_alloc(u32LogoSize, &u32AllocSize);
    if (0 == u32LogoRamAddress || u32AllocSize < u32LogoSize)
    {
        HI_ERR_CA("Alloc Reserve Mem err!, Ret = %#x\n", ret);
        return HI_FAILURE;
    }
    HI_ERR_CA("logo part size = %llx, logo ram address = %x\n", stLogoInfo.PartSize, u32LogoRamAddress);

    HI_Flash_Close(hFlash);

    /*Read base partition*/
    hFlash = HI_Flash_Open(HI_FLASH_TYPE_BUTT, PDM_BASE_DEF_NAME, 0, 0);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_CA("HI_Flash_Open() %s error, result %x!\n",PDM_BASE_DEF_NAME, ret);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stBaseInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_Flash_GetInfo() error, result %x!\n", ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    u32BaseSize = (HI_U32)stBaseInfo.PartSize;
    /*LOGO_DATA_ADDR - baseparam size = 0x8FDFE000*/
    u32BaseRamAddress = (HI_U32) reserve_mem_alloc(u32BaseSize, &u32AllocSize);
    if (0 == u32BaseRamAddress || u32AllocSize < u32BaseSize)
    {
        HI_ERR_CA("Alloc Reserve Mem err!, Ret = %#x\n", ret);
        return HI_FAILURE;
    }

    HI_Flash_Close(hFlash);

    //verify logo
    CA_ASSERT(CX_AuthenticateLogo(u32LogoRamAddress, &u32LogoLoc), ret);
    //verify baseparam
    CA_ASSERT(CX_AuthenticateBase(u32BaseRamAddress, &u32BaseLoc), ret);

    memset(&stPdmInfo, 0, sizeof(HI_PDM_MEMINFO_S));
    stPdmInfo.u32BaseAddr = u32BaseLoc;
    stPdmInfo.u32LogoAddr = u32LogoLoc;

    /* show logo */
    Logo_Main(HI_TRUE, &stPdmInfo);

    /**just protect the logo reserve memory*/
    HI_DRV_PDM_SetTagData();

    return HI_SUCCESS;
}

int load_direct_env(void *env, unsigned int offset, unsigned int size)
{
#ifdef HI_ADVCA_VERIFY_ENABLE

#ifdef CONFIG_ENV_BACKUP
    static HI_U32 s_u32LoadEnvLoop = 2;
#else
    static HI_U32 s_u32LoadEnvLoop = 1;
#endif

    HI_S32 ret;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_U32 u32EnvRamAddr = 0;
    HI_CA_AuthInfo_S stAuthInfo;

    HI_INFO_CA("Enter Conax load_direct_env offset:%x, size:%x\n", offset, size);

    if(s_u32LoadEnvLoop > 0)
    {
        s_u32LoadEnvLoop--;
    }
    else
    {
        return HI_FAILURE;
    }

    memset(&stAuthInfo, 0x0, sizeof (stAuthInfo));
    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;

    ret = HI_CX_VerifyFlashPartitionByAddr(offset, u32RamAddr, &stAuthInfo, &u32EnvRamAddr);
    if ((HI_SUCCESS != ret) && (0 == s_u32LoadEnvLoop))
    {
        HI_ERR_CA("Authenticate bootargs failed, s_u32LoadEnvLoop:%d\n", s_u32LoadEnvLoop);
        CX_Reset();
    }

    if (HI_SUCCESS == ret)
    {
        memset(env, 0, size);
        memcpy(env, (HI_U8 *)(u32EnvRamAddr), size);

        HI_ERR_CA("Authenticate bootargs success\n");

    }

    return ret;
#else
   /*return Failure and fastboot will continue to read env by normal way*/
   return -1;
#endif
}


HI_S32 HI_SSA_Init(HI_VOID)
{
   //HI_DRV_PDM_RegDecryptCallBack(CX_PDM_DataDecrypt);
   HI_CX_VerifyInit();
   return HI_SUCCESS;
}

/*
The entry for security system authenticate
*/
HI_S32 HI_SSA_Entry(HI_VOID)
{
    HI_S32 ret;
    HI_BOOL bNeedUpgrd = HI_FALSE;
    HI_CHAR chBootCmd[64] = {0};
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_U32 u32LaunchAddr = 0;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;

    CX_AuthAndShowLogo();

    ret = HI_CX_VerifyFlashPartitionByName((HI_CHAR *)STBID_BASIC, u32RamAddr, &stAuthInfo, NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("fail to verify stbid\n");
        CX_Reset();
    }

    ret = HI_CX_VerifyFlashPartitionByName((HI_CHAR *)LOADER_BASIC, u32RamAddr, &stAuthInfo, &u32LaunchAddr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("fail to verify loader\n");
        CX_Reset();
    }

    ret = CX_ReadUpgrdFlag(&bNeedUpgrd);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("fail to verify loader db\n");
        CX_Reset();
    }

    if (0x01 != bNeedUpgrd)
    {
        ret = HI_CX_VerifyFlashPartitionByName((HI_CHAR *)SYSTEM_BASIC, u32RamAddr, &stAuthInfo, &u32LaunchAddr);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("fail to verify system\n");
            CX_Reset();
        }
    }

    snprintf(chBootCmd, sizeof(chBootCmd), "bootm 0x%x", u32LaunchAddr);

#ifndef HI_MINIBOOT_SUPPORT
    run_command(chBootCmd, 0);
#else
    run_cmd(chBootCmd, strlen(chBootCmd));
#endif
    return ret;
}

