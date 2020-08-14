/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : nasc1x_sbp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <stdarg.h>
#include "malloc.h"
#include "hi_type.h"
#include "nocs_bsd.h"
#include "hi_common.h"

#ifndef HI_MINIBOOT_SUPPORT
#include <linux/types.h>
#else
#include "app.h"
#endif

#ifdef HI_ADVCA_FUNCTION_RELEASE
#include "ssaflash.h"
#else
#include "ssaflad.h"
#endif

#include "hi_flash.h"
#include "loader_dbinfo.h"
#include "hi_drv_pdm.h"
#include "hi_unf_keyled.h"
#include "hi_unf_ir.h"

extern HI_S32 Logo_Main(HI_BOOL bNeedFastplay,HI_PDM_MEMINFO_S *pstMemInfo);

#ifdef HI_ANDROID_BOOT_SUPPORT
extern int set_default_ethaddr(void);
#endif

#ifdef HI_MINIBOOT_SUPPORT
extern int mmu_cache_enable(void);

extern int mmu_cache_disable(void);
#endif

typedef enum hiSBP_SIG_TYPE_E
{
    SBP_SIG_TYPE_NONE = 0,
    SBP_SIG_TYPE_SINGLE,
    SBP_SIG_TYPE_DOUBLE,
    SBP_SIG_TYPE_BUTT,
} HI_SBP_SIG_TYPE_E;

#define SBP_HEADER_SIZE     4
#define SBP_SIG_SIZE_SINGLE  128    /*single signature size*/
#define SBP_SIG_SIZE_DOUBLE  132    /*double signature size*/

#define SBP_HEADER_PART_SIZE    0x20000
#define SBP_LOGO_SIZE           0x800000
#define SBP_BASE_SIZE           0x400000
#define SBP_PQ_SIZE             0x400000

typedef enum hiSBP_DATA_TYPE_E
{
    SBP_DATA_DA1 = 0,
    SBP_DATA_DA2_FACTORY,
    SBP_DATA_PK,
    SBP_DATA_DA2,
    SBP_DATA_DA2_REDUNDANT,
    SBP_DATA_RECOVERY,
    SBP_DATA_BOOTARGS,
    SBP_DATA_LOADER,
    SBP_DATA_LOGO,
    SBP_DATA_BOOTVIDEO,
    SBP_DATA_KERNEL,
    SBP_DATA_SYSTEM,
    SBP_DATA_TYPE_BUTT,
} HI_SBP_DATA_TYPE_E;

#define SBP_MODULE_BOOTARGS         ICS_MODULE_1_INVALID
#define SBP_MODULE_LOGO             ICS_MODULE_2_INVALID
#define SBP_MODULE_KERNEL           ICS_MODULE_3_INVALID
#define SBP_MODULE_SYSTEM           ICS_MODULE_4_INVALID
#define SBP_MODULE_BOOTVIDEO        ICS_MODULE_5_INVALID
#define SBP_MODULE_RECOVERY         ICS_MODULE_6_INVALID
#define SBP_MODULE_LOADER           ICS_MODULE_7_INVALID

#define SBP_BEP_RAM_ADDR                0x800000
#define SBP_BOOT_CODE_ADDR              (SBP_BEP_RAM_ADDR + 0x2400)
#define SBP_BEP_MAX_SIZE                0x100
#define SBP_DA1_RAM_ORG_ADDR            (SBP_BEP_RAM_ADDR + SBP_BEP_MAX_SIZE)
#define SBP_DA1_MAX_SIZE                0x200
#define SBP_DA2_FAC_RAM_ADDR            (SBP_DA1_RAM_ORG_ADDR + SBP_DA1_MAX_SIZE)
#define SBP_DA2_MAX_SIZE                0x2000

#define SBP_ENV_MAX_SIZE                0x20000

/*Note : SBP_TMP_BUF should not cover other data area*/
#define SBP_TMP_BUF_ADDR                0x1000000
#define SBP_TMP_BUF_MAX_SIZE            0x10000000
#define SBP_DDR_START_ADDR              0x4000000       /*64M*/
#define SBP_DDR_END_ADDR                (0x2bc00000)    /*700M*/

#define SBP_INVALID_SIZE                0xFFFFFFFF
#define SBP_PARTITION_NAME_SIZE         32

typedef struct hiSBP_DATA_INFO_S
{
    HI_SBP_DATA_TYPE_E   enDataType;
    HI_FLASH_TYPE_E enFlashType;
    HI_CHAR szPartitionName[SBP_PARTITION_NAME_SIZE];
    HI_U32  u32Offset;
    HI_U32  u32RamAddr;
    HI_U32  u32Len;
} HI_SBP_DATA_INFO_S;

HI_SBP_DATA_INFO_S stSBPDataAll[SBP_DATA_TYPE_BUTT]={
    {SBP_DATA_DA1,              HI_FLASH_TYPE_SPI_0,    "boot",             0x2100,     0x3FF00000, SBP_DA1_MAX_SIZE},
    {SBP_DATA_DA2_FACTORY,      HI_FLASH_TYPE_SPI_0,    "boot",             0x2300,     0,          SBP_DA2_MAX_SIZE},
    {SBP_DATA_PK,               HI_FLASH_TYPE_SPI_0,    "boot",             0x1800,     0x007FF800, 600},
    {SBP_DATA_DA2,              HI_FLASH_TYPE_SPI_0,    "DA2",              0,          0x3FF10000, SBP_DA2_MAX_SIZE},
    {SBP_DATA_DA2_REDUNDANT,    HI_FLASH_TYPE_SPI_0,    "DA2_REDUNDANT",    0,          0x3FF20000, SBP_DA2_MAX_SIZE},
    {SBP_DATA_RECOVERY,         HI_FLASH_TYPE_SPI_0,    "recovery",         0,          0x04100000, SBP_INVALID_SIZE},
    {SBP_DATA_BOOTARGS,         HI_FLASH_TYPE_SPI_0,    "bootargs",         0x003C0000, 0x04000000, SBP_ENV_MAX_SIZE},
    {SBP_DATA_LOADER,           HI_FLASH_TYPE_EMMC_0,   "h_loader",         0,          0x04500000, SBP_INVALID_SIZE},
    {SBP_DATA_LOGO,             HI_FLASH_TYPE_EMMC_0,   "h_logo",           0,          0x06e00000, SBP_INVALID_SIZE},
    {SBP_DATA_BOOTVIDEO,        HI_FLASH_TYPE_EMMC_0,   "h_fastplay",       0,          0x07f00000, SBP_INVALID_SIZE},
    {SBP_DATA_KERNEL,           HI_FLASH_TYPE_EMMC_0,   "h_kernel",         0,          0x0a000000, SBP_INVALID_SIZE},
    {SBP_DATA_SYSTEM,           HI_FLASH_TYPE_EMMC_0,   "h_system",         0,          0x0b100000, SBP_INVALID_SIZE},
};

const HI_SBP_DATA_TYPE_E enDA2Module[]={
    SBP_DATA_BOOTARGS,
    SBP_DATA_LOGO,
    SBP_DATA_KERNEL,
    SBP_DATA_RECOVERY,
    SBP_DATA_LOADER,
};

#ifndef MAX_CMD_SIZE
#define MAX_CMD_SIZE              256
#endif

#ifndef MAX_BOOT_ARGS_SIZE
#define MAX_BOOT_ARGS_SIZE        512
#endif

#define FLASH_MAX_BLOCK_SIZE    0x80000     /*512k*/

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define HI_SBP_CHECK_DEBUG          /*do not process SBP check result*/
//#define HI_SBP_CHECK_BY_CRC         /*only for SBP test  by self*/
#endif

#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_SBP(format, arg...)
#define HI_INFO_SBP(format, arg...)
#else
#define HI_ERR_SBP(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_SBP(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif

HI_CHAR *SBP_SearchBootArgs(HI_CHAR *pSrc, HI_CHAR *pSubStr, HI_U32 len);

HI_U32 SBP_MSB2LSB_UINT(HI_U8 *pu8Data)
{
    if(HI_NULL == pu8Data)
        return 0;
    else
        return ((pu8Data[0]<<24)|(pu8Data[1]<<16)|(pu8Data[2]<<8)|pu8Data[3]);
}

#ifndef HI_MINIBOOT_SUPPORT
int reboot()
{
	do_shutdown();

	disable_interrupts();
	reset_cpu(0);
}
#endif

/*LED Show string, only for "boot" or "load"*/
static HI_VOID LED_ShowBLString(HI_CHAR *string)
{
#ifdef HI_BUILD_WITH_KEYLED
	HI_U32 aBLLedValue[2][5] =
	{
		/*boot value*/ {0xe1c5c5c1, 0x785c5c7c, 0x1e3a3a3e, 0x785c5c7c, 0x785c5c7c},
		/*load value*/ {0x1105c5e3, 0x5e775c38, 0x7aee3a1c, 0x5e775c38, 0x5e775c38}
	};

	HI_U8 u8Index = 0;

#if defined HI_BOOT_KEYLED_74HC164
	u8Index = 0;
#elif defined HI_BOOT_KEYLED_PT6961
	u8Index = 1;
#elif defined HI_BOOT_KEYLED_CT1642
	u8Index = 2;
#elif defined HI_BOOT_KEYLED_PT6964
	u8Index = 3;
#elif defined HI_BOOT_KEYLED_FD650
	u8Index = 4;
#endif

	if (0 == HI_OSAL_Strncmp(string, "boot", sizeof("boot") ))
		HI_UNF_LED_Display(aBLLedValue[0][u8Index]);
	else if (0 == HI_OSAL_Strncmp(string, "LoAd", sizeof("LoAd")))
		HI_UNF_LED_Display(aBLLedValue[1][u8Index]);
#endif

}

/*IR Get test*/
static HI_S32 IR_Test()
{
#ifdef HI_BUILD_WITH_IR
    int ret;
    char c;
    HI_U64 key;
    char name[64];
    enum KEY_STATUS status;
    unsigned int read_timeout = 10000;

    ret = HI_UNF_IR_Init();
    if (ret)
    {
        HI_ERR_SBP("Fail to open ir dev!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_IR_Enable(HI_TRUE);
    if (ret)
    {
        HI_ERR_SBP("Fail to set enable state of ir module!");
        return HI_FAILURE;
    }

    HI_INFO_SBP("Ready for receive the IR value!\n");

    ret = HI_UNF_IR_GetValueWithProtocol(&status, &key, name, sizeof(name), read_timeout);
    if (!ret)
    {
        HI_INFO_SBP("Received key: 0x%.08llx, %s,\tprotocol: %s.\n",
                key, status == KEY_STAT_DOWN ? "DOWN" :
                (status == KEY_STAT_UP ? "UP" : "HOLD"),
                name);
	}

    HI_UNF_IR_DeInit();
#else
    HI_INFO_SBP("HI_BUILD_WITH_IR is not set!\n");
#endif

    return HI_SUCCESS;
}


HI_SBP_DATA_TYPE_E SBP_Loader_Check(HI_VOID)
{
    // TODO: Implement the Check for booting
    return SBP_DATA_KERNEL;
}

HI_S32 SBP_CopyDataFromFlashToRAM(HI_SBP_DATA_TYPE_E enSrc, HI_BOOL bSignatured, HI_U8 **pu8DataAddr, HI_U32 *pu32DataLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_U8 *pu8Buf = HI_NULL;
    HI_U32 u32ReadAddr;
    HI_U32 u32ReadLen = FLASH_MAX_BLOCK_SIZE;
    HI_U32 u32Offset = 0;
    HI_U32 u32DataLen = 0;
    HI_U32 u32BlockSize = FLASH_MAX_BLOCK_SIZE;
    HI_Flash_InterInfo_S stFlashInfo;
    const HI_SBP_DATA_INFO_S *pstSbpDataInfo=HI_NULL;

    if((HI_NULL == pu8DataAddr)||(HI_NULL == pu32DataLen))
    {
        HI_ERR_SBP("Null Pointer\n");
        return HI_FAILURE;
    }

    if(enSrc >= SBP_DATA_TYPE_BUTT)
    {
        HI_ERR_SBP("Invalid data type, enSrc [%d].\n", enSrc);
        return HI_FAILURE;
    }
    pstSbpDataInfo = &stSBPDataAll[enSrc];

    *pu8DataAddr = HI_NULL;

    /*Open flash by partition*/
    hFlash = HI_Flash_Open(pstSbpDataInfo->enFlashType, (HI_CHAR *)pstSbpDataInfo->szPartitionName, 0, 0);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() %s error!\n", pstSbpDataInfo->szPartitionName);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_GetInfo() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    u32BlockSize = stFlashInfo.BlockSize;
    u32ReadAddr = (pstSbpDataInfo->u32Offset/u32BlockSize)*u32BlockSize;
    u32Offset = pstSbpDataInfo->u32Offset - u32ReadAddr;

    /*Get data and read length*/
    if(SBP_INVALID_SIZE == pstSbpDataInfo->u32Len)
    {
        if(pstSbpDataInfo->u32Offset >= stFlashInfo.PartSize)
        {
            HI_ERR_SBP("Over partition size!\n");
            (HI_VOID)HI_Flash_Close(hFlash);
            return HI_FAILURE;
        }
        u32DataLen = stFlashInfo.PartSize - pstSbpDataInfo->u32Offset;
    }
    else
    {
        u32DataLen = pstSbpDataInfo->u32Len;
    }

    /*Get signatured data length*/
    if (bSignatured)
    {
        HI_U32 u32SbpHeader = 0;
        pu8Buf = (HI_U8 *)SBP_TMP_BUF_ADDR;
        /*avoid sbp header cross the block*/
        u32ReadLen = u32Offset + SBP_HEADER_SIZE;
        if(u32ReadLen%u32BlockSize)
        {
            u32ReadLen = (u32ReadLen/u32BlockSize)*u32BlockSize + u32BlockSize;
        }
        ret = HI_Flash_Read(hFlash, u32ReadAddr, pu8Buf, u32ReadLen, HI_FLASH_RW_FLAG_RAW);
        if(ret <= 0)
        {
            HI_ERR_SBP("HI_Flash_Read() Error, result %d, Len [%x]\n", ret, u32ReadLen);
            HI_Flash_Close(hFlash);
            return HI_FAILURE;
        }

        /*Note : data length maybe bigger than opened partition, such as kernel and rootfs are signatured together*/
        u32SbpHeader = SBP_MSB2LSB_UINT((HI_U8 *)(pu8Buf+u32Offset));
        //HI_INFO_SBP("SBP Data Type [%d], SbpHeader [0x%x]\n", pstSbpDataInfo->enDataType, u32SbpHeader);
        if ((u32SbpHeader >= SBP_HEADER_SIZE)&&(u32SbpHeader < stFlashInfo.TotalSize))
        {
            u32DataLen = u32SbpHeader+ SBP_SIG_SIZE_DOUBLE;
        }
    }
    (HI_VOID)HI_Flash_Close(hFlash);

    u32ReadAddr = (HI_U32)(stFlashInfo.OpenAddr+u32ReadAddr);
    u32ReadLen = u32DataLen +u32Offset;
    if(u32ReadLen % u32BlockSize)
    {
        u32ReadLen = (u32ReadLen/u32BlockSize)*u32BlockSize + u32BlockSize;
    }

    HI_INFO_SBP("SBP Data Type [%d], Partition [%s], ReadAddr [%x], Offset [%x], ReadLen [%x], DataLen [%x]\n",
        pstSbpDataInfo->enDataType,
        pstSbpDataInfo->szPartitionName,
        u32ReadAddr,
        u32Offset,
        u32ReadLen,
        u32DataLen);

    /*Open and read by address and length*/
    hFlash = HI_Flash_Open(pstSbpDataInfo->enFlashType, HI_NULL, (HI_U64)u32ReadAddr, (HI_U64)stFlashInfo.TotalSize-u32ReadAddr);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() error!\n");
        return HI_FAILURE;
    }

    if((0 == u32Offset) && (0 != pstSbpDataInfo->u32RamAddr))
    {
        pu8Buf = (HI_U8 *)pstSbpDataInfo->u32RamAddr;
    }
    else if(u32ReadLen <= SBP_TMP_BUF_MAX_SIZE)
    {
        pu8Buf = (HI_U8 *)reserve_mem_alloc(u32ReadLen, &u32ReadAddr);
    }
    else
    {
        HI_ERR_SBP("Invalid length [%x], should less than [%x]\n",u32ReadLen,SBP_TMP_BUF_MAX_SIZE);
        HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    ret = HI_Flash_Read(hFlash, 0, pu8Buf, u32ReadLen, HI_FLASH_RW_FLAG_RAW);
    if(ret <= 0)
    {
        HI_ERR_SBP("HI_Flash_Read() Error, result %d, Len [%x]\n", ret, u32ReadLen);
        HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    if((SBP_TMP_BUF_ADDR == (HI_U32)pu8Buf) && (0 != pstSbpDataInfo->u32RamAddr))
    {
        memcpy((void *)pstSbpDataInfo->u32RamAddr, (void *)(pu8Buf+u32Offset), u32DataLen);
    }

    if(0 != pstSbpDataInfo->u32RamAddr)
    {
        *pu8DataAddr = (HI_U8 *)pstSbpDataInfo->u32RamAddr;
    }
    else
    {
        *pu8DataAddr = (HI_U8 *)(pu8Buf+u32Offset);
    }
    *pu32DataLen = u32DataLen;

#if 0
    {
        HI_U32 i;
        HI_U8 * pu8Data = *pu8DataAddr;

        HI_SIMPLEINFO_CA("Data address [0x%x] START:\n",(HI_U32)pu8Data);
        for(i=0;i<16;i++)
        {
            HI_SIMPLEINFO_CA("%02x ",pu8Data[i]);
        }
        HI_SIMPLEINFO_CA("\nData END:\n\n");
    }
#endif

    HI_Flash_Close(hFlash);
    return HI_SUCCESS;

}

HI_S32 SBP_CopyDataFromRAMToFlash(HI_U8 *pu8Src, HI_U32 u32DataLen, HI_SBP_DATA_TYPE_E enDst)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_U32 u32BlockSize = FLASH_MAX_BLOCK_SIZE;
    HI_Flash_InterInfo_S stFlashInfo;
    const HI_SBP_DATA_INFO_S *pstSbpDataInfo=HI_NULL;

    if(HI_NULL == pu8Src)
    {
        HI_ERR_SBP("Null Pointer\n");
        return HI_FAILURE;
    }

    if(enDst >= SBP_DATA_TYPE_BUTT)
    {
        HI_ERR_SBP("Invalid data type, enDst [%d].\n", enDst);
        return HI_FAILURE;
    }
    pstSbpDataInfo = &stSBPDataAll[enDst];


    /*Open flash by partition*/
    hFlash = HI_Flash_Open(pstSbpDataInfo->enFlashType, (HI_CHAR *)pstSbpDataInfo->szPartitionName, 0, 0);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() %s error!\n", pstSbpDataInfo->szPartitionName);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_GetInfo() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    u32BlockSize = stFlashInfo.BlockSize;

    if(pstSbpDataInfo->u32Offset % u32BlockSize)
    {
        HI_ERR_SBP("Invalid flash partition, not align block size\n");
        (HI_VOID)HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    if((0 == u32DataLen)||(u32DataLen > stFlashInfo.PartSize))
    {
        u32DataLen = stFlashInfo.PartSize;
    }

    if(u32DataLen%u32BlockSize)
    {
        u32DataLen = (u32DataLen/u32BlockSize)*u32BlockSize + u32BlockSize;
    }

	if((HI_FLASH_TYPE_SPI_0 == stFlashInfo.FlashType) || (HI_FLASH_TYPE_NAND_0 == stFlashInfo.FlashType))
	{
	    ret = HI_Flash_Erase(hFlash, 0, stFlashInfo.PartSize);
	    if(stFlashInfo.PartSize != ret)
	    {
	        HI_ERR_SBP("HI_Flash_Erase() error, result %x!\n",ret);
	        HI_Flash_Close(hFlash);
	        return HI_FAILURE;
	    }
	}

    ret = HI_Flash_Write(hFlash, 0, pu8Src, u32DataLen, HI_FLASH_RW_FLAG_RAW);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_Write() error, result %x!\n",ret);
        HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    if(HI_INVALID_HANDLE != hFlash)
    {
        HI_Flash_Close(hFlash);
    }
    return ret;

}

HI_S32 SBP_ReadFlash_ByAddress(HI_FLASH_TYPE_E enFlashType,
                                    HI_U32 u32Addr,
                                    HI_U32 u32MaxLen,
                                    HI_U8 *pu8Buf,
                                    HI_BOOL bSignatured,
                                    HI_U32 *pu32DataLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32BlockSize = FLASH_MAX_BLOCK_SIZE;
    HI_U32 u32ReadAddr;
    HI_U32 u32ReadLen;
    HI_U32 u32DataLen = u32MaxLen;
    HI_U32 u32Offset = 0;

    if((HI_NULL == pu8Buf)||(HI_NULL == pu32DataLen))
    {
        HI_ERR_SBP("Null Pointer\n");
        return HI_FAILURE;
    }

    HI_INFO_SBP("enFlashType [%d], u32Addr [0x%x], u32DataLen [0x%x], pu8Buf [0x%x], bSignatured [%d].\n",
            enFlashType,
            u32Addr,
            u32DataLen,
            (HI_U32)pu8Buf,
            bSignatured);

    u32ReadAddr = u32Addr & (~(FLASH_MAX_BLOCK_SIZE - 1));
    u32Offset = u32Addr - u32ReadAddr;
    u32ReadLen = u32Offset + SBP_HEADER_SIZE;
    if(u32ReadLen % FLASH_MAX_BLOCK_SIZE)
    {
        u32ReadLen = (u32ReadLen / FLASH_MAX_BLOCK_SIZE) * FLASH_MAX_BLOCK_SIZE + FLASH_MAX_BLOCK_SIZE;
    }

    hFlash = HI_Flash_Open(enFlashType, HI_NULL, u32ReadAddr, u32ReadLen);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() error, result %x!\n",ret);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_GetInfo() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    /*Get signatured data length*/
    if (bSignatured)
    {
        HI_U32 u32SbpHeader = 0;
        ret = HI_Flash_Read(hFlash, 0, pu8Buf, u32ReadLen, HI_FLASH_RW_FLAG_RAW);
        if(ret <= 0)
        {
            HI_ERR_SBP("HI_Flash_Read() Error, result %d, Len [%x]\n", ret, u32BlockSize);
            HI_Flash_Close(hFlash);
            return HI_FAILURE;
        }

        u32SbpHeader = SBP_MSB2LSB_UINT((HI_U8 *)(pu8Buf+u32Offset));
        if ((u32SbpHeader >= SBP_HEADER_SIZE)&&(u32SbpHeader < stFlashInfo.TotalSize))
        {
            u32DataLen = u32SbpHeader+ SBP_SIG_SIZE_DOUBLE;
        }
    }
    (HI_VOID)HI_Flash_Close(hFlash);

    if((0==u32DataLen)||(u32DataLen>stFlashInfo.TotalSize))
    {
        HI_ERR_SBP("Invalid data length [0x%x]\n", u32DataLen);
        (HI_VOID)HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    u32BlockSize = stFlashInfo.BlockSize;
    u32ReadAddr = u32Addr & (~(u32BlockSize-1));
    u32Offset = u32Addr - u32ReadAddr;
    u32ReadLen = u32DataLen +u32Offset;
    if(u32ReadLen%u32BlockSize)
    {
        u32ReadLen = (u32ReadLen/u32BlockSize)*u32BlockSize + u32BlockSize;
    }

    hFlash = HI_Flash_Open(enFlashType, HI_NULL, (HI_U64)u32ReadAddr, (HI_U64)stFlashInfo.TotalSize-u32ReadAddr);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() error!\n");
        return HI_FAILURE;
    }

    ret = HI_Flash_Read(hFlash, 0, pu8Buf, u32ReadLen, HI_FLASH_RW_FLAG_RAW);
    if(ret <= 0)
    {
        HI_ERR_SBP("HI_Flash_Read() Error, result %d, Len [%x]\n", ret, u32ReadLen);
        HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    if(0 != u32Offset)
    {
        memcpy((void *)pu8Buf, (void *)(pu8Buf+u32Offset), u32DataLen);
    }

    *pu32DataLen = u32DataLen;

    HI_Flash_Close(hFlash);

    return HI_SUCCESS;
}


HI_S32 SBP_WriteFlash_ByAddress(HI_FLASH_TYPE_E enFlashType,
                                    HI_U32 u32Addr,
                                    HI_U8 *pu8Buf,
                                    HI_U32 u32DataLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32BlockSize = FLASH_MAX_BLOCK_SIZE;
    HI_U32 u32WriteLen = 0;

    HI_INFO_SBP("enFlashType [%d], u32Addr [0x%x], u32DataLen [0x%x], pu8Buf [0x%x]\n",
            enFlashType,
            u32Addr,
            u32DataLen,
            (HI_U32)pu8Buf);

    if((HI_NULL == pu8Buf)||(0 == u32DataLen))
    {
        HI_ERR_SBP("Invalid Parameter\n");
        return HI_FAILURE;
    }

    if(u32DataLen % FLASH_MAX_BLOCK_SIZE)
    {
        u32WriteLen = (u32DataLen / FLASH_MAX_BLOCK_SIZE) * FLASH_MAX_BLOCK_SIZE + FLASH_MAX_BLOCK_SIZE;
    }
    else
    {
        u32WriteLen = u32DataLen;
    }

    hFlash = HI_Flash_Open(enFlashType, HI_NULL, (HI_U64)u32Addr, (HI_U64)u32WriteLen);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() error, result %x!\n",ret);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_GetInfo() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    u32BlockSize = stFlashInfo.BlockSize;

    if(u32DataLen % u32BlockSize)
    {
        u32WriteLen = (u32DataLen / u32BlockSize) * u32BlockSize + u32BlockSize;
    }
    else
    {
        u32WriteLen = u32DataLen;
    }

	if((HI_FLASH_TYPE_SPI_0 == stFlashInfo.FlashType) || (HI_FLASH_TYPE_NAND_0 == stFlashInfo.FlashType))
	{
	    ret = HI_Flash_Erase(hFlash, 0, u32WriteLen);
	    if(u32WriteLen != ret)
	    {
	        HI_ERR_SBP("HI_Flash_Erase() error, result %x!\n",ret);
	        HI_Flash_Close(hFlash);
	        return HI_FAILURE;
	    }
	}

    ret = HI_Flash_Write(hFlash, 0, pu8Buf, u32WriteLen, HI_FLASH_RW_FLAG_RAW);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_Write() error, result %x!\n",ret);
        HI_Flash_Close(hFlash);
        return HI_FAILURE;
    }

    HI_Flash_Close(hFlash);

    return HI_SUCCESS;
}

HI_VOID SBP_Reset(HI_VOID)
{
    HI_INFO_SBP(" \n");
    reboot ();
    reboot ();
    reboot ();
    while(1) { };
}

HI_VOID SBP_Infinite_loop(HI_VOID)
{
    HI_INFO_SBP(" \n");
    while(1) { };
    while(1) { };
    while(1) { };
    reboot ();
}

HI_S32 SBP_MemCmp(HI_U8 *pu8Dst, HI_U8 *pu8Src, HI_U32 u32Size, HI_BOOL bSignatured)
{
    HI_S32 ret;
    HI_U32 u32Len;

    if((NULL == pu8Dst)||(NULL == pu8Src)||(0 == u32Size))
    {
        HI_ERR_SBP("Invalid parameter.\n");
        return -1;
    }

    if(!bSignatured)
    {
        ret = memcmp((HI_VOID *)pu8Dst, (HI_VOID *)pu8Src, u32Size);
        return ret;
    }

    /*SBP double signatured, only compare valid data*/

    /*Check SBP header*/
    ret = memcmp((HI_VOID *)pu8Dst, (HI_VOID *)pu8Src, SBP_HEADER_SIZE);
    if(0 != ret)
    {
        HI_ERR_SBP("SBP header is not same.\n");
        HI_INFO_SBP("pu8Dst [0x%x], pu8Src [0x%x], u32Size [0x%x].\n",
            (HI_U32)pu8Dst, (HI_U32)pu8Src, u32Size);
        return ret;
    }

    /*Should notice the sbp header is 0xffffffff*/
    u32Len = SBP_MSB2LSB_UINT(pu8Dst);
    if((u32Len == 0 ) || (u32Len > u32Size) || ((u32Len + SBP_SIG_SIZE_DOUBLE) > u32Size))
    {
        u32Len = u32Size;
    }
    else
    {
        u32Len += SBP_SIG_SIZE_DOUBLE;
    }

    ret = memcmp((HI_VOID *)pu8Dst, (HI_VOID *)pu8Src, u32Len);

    HI_INFO_SBP("pu8Dst [0x%x], pu8Src [0x%x], u32Size [0x%x], u32Len [0x%x], ret [%d]\n",
        (HI_U32)pu8Dst, (HI_U32)pu8Src, u32Size, u32Len, ret);

    return ret;

}

HI_S32 SBP_Module_Sync(HI_SBP_DATA_TYPE_E enSrc, HI_SBP_DATA_TYPE_E enBak)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8BakData = HI_NULL;
    HI_U32 u32DataLen = 0;

    if((enBak == enSrc) || (enBak >= SBP_DATA_TYPE_BUTT) || (enSrc >= SBP_DATA_TYPE_BUTT))
    {
        HI_ERR_SBP("Invalid parameter, enBak [%d], enSrc [%d].\n", enBak, enSrc);
        return HI_FAILURE;
    }

    if(stSBPDataAll[enBak].u32Len != stSBPDataAll[enSrc].u32Len)
    {
        HI_ERR_SBP("Length is not same.\n");
        return HI_FAILURE;
    }

    if(stSBPDataAll[enBak].u32RamAddr == stSBPDataAll[enSrc].u32RamAddr)
    {
        HI_ERR_SBP("Ram address is same.\n");
        return HI_FAILURE;
    }

    ret = SBP_CopyDataFromFlashToRAM(enBak, HI_TRUE, &pu8BakData, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Copy bak module [%d] to RAM error, result %x.\n", enBak, ret);
        return ret;
    }

    u32DataLen = SBP_MSB2LSB_UINT((HI_U8 *)stSBPDataAll[enSrc].u32RamAddr) + SBP_SIG_SIZE_DOUBLE;
    if((u32DataLen < (SBP_HEADER_SIZE + SBP_SIG_SIZE_DOUBLE)) || (u32DataLen > stSBPDataAll[enSrc].u32Len))
    {
        u32DataLen = stSBPDataAll[enSrc].u32Len;
    }

    if(0 == SBP_MemCmp(pu8BakData, (HI_U8 *)stSBPDataAll[enSrc].u32RamAddr, u32DataLen, HI_TRUE))
    {
        return HI_SUCCESS;
    }

    ret = SBP_CopyDataFromRAMToFlash((HI_U8 *)stSBPDataAll[enSrc].u32RamAddr, u32DataLen, enBak);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Sync [%s]  to [%s] error, result %x.\n", stSBPDataAll[enSrc].szPartitionName, stSBPDataAll[enBak].szPartitionName, ret);
    }
    else
    {
        HI_INFO_SBP("Sync [%s]  to [%s] SUCCESS.\n", stSBPDataAll[enSrc].szPartitionName, stSBPDataAll[enBak].szPartitionName);
    }
    return ret;
}

HI_S32 SBP_Module_DA2_Recovery(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 * pu8DA2 = (HI_U8 *)stSBPDataAll[SBP_DATA_DA2].u32RamAddr;
    HI_U8 * pu8DA2Bak = HI_NULL;
    HI_U32 u32DataLen = SBP_DA2_MAX_SIZE;

    /*Compare DA2 with DA2_FACTORY*/
    if(0 == SBP_MemCmp(pu8DA2, (HI_U8 *)SBP_DA2_FAC_RAM_ADDR, SBP_DA2_MAX_SIZE, HI_TRUE))
    {
        /*DA2 == DA2_FACTORY*/
        SBP_Infinite_loop();
        return HI_FAILURE;
    }

    /*DA2 != DA2_FACTORY, Compare DA2 with DA2_REDUNDANT*/

    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_DA2_REDUNDANT, HI_FALSE, &pu8DA2Bak, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Copy DA2_REDUNDANT to RAM error, result %x.\n", ret);
        SBP_Reset();
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_SBP("Copy DA2_REDUNDANT to RAM SUCCESS, Address [0x%08x].\n", (HI_U32)pu8DA2Bak);
    }

    if(0 != SBP_MemCmp(pu8DA2, pu8DA2Bak, SBP_DA2_MAX_SIZE, HI_TRUE))
    {
         /*DA2 != DA2_REDUNDANT, copy DA2_REDUNDANT to DA2*/
        ret = SBP_CopyDataFromRAMToFlash(pu8DA2Bak, SBP_DA2_MAX_SIZE, SBP_DATA_DA2);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_SBP("Copy DA2_REDUNDANT to DA2 partition in flash error, result %x.\n", ret);
        }
        else
        {
            HI_INFO_SBP("Copy  DA2_REDUNDANT to DA2 partition in flash SUCCESS.\n");
        }
    }
    else
    {
       /*DA2 == DA2_REDUNDANT, copy DA2_FACTORY to DA2*/
        ret = SBP_CopyDataFromRAMToFlash((HI_U8 *)SBP_DA2_FAC_RAM_ADDR, SBP_DA2_MAX_SIZE, SBP_DATA_DA2);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_SBP("Copy  DA2_FACTORY to DA2 partition in flash error, result %x.\n", ret);
        }
        else
        {
            HI_INFO_SBP("Copy  DA2_FACTORY to DA2 partition in flash SUCCESS.\n");
        }
    }

    SBP_Reset();
    return HI_FAILURE;
}

HI_S32 SBP_Fault_Process(TIntegrityCheckStatus u32Result)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32GlobalStatus = 0;
    HI_U32 u32ModuleStatus = 0;

    u32GlobalStatus = u32Result & (~ICS_ALL_MODULES_INVALID);
    u32ModuleStatus = u32Result & ICS_ALL_MODULES_INVALID;

    switch(u32GlobalStatus)
    {
        case ICS_VALID_NO_ERROR:
        {
            return HI_SUCCESS;
        }
        case ICS_LOCKED_MEMORY_INVALID:
        {
            HI_ERR_SBP("ICS_LOCKED_MEMORY_INVALID\n");
            SBP_Infinite_loop();
            return HI_FAILURE;
        }
        case ICS_SSA_DA_2_INVALID:
        {
            HI_ERR_SBP("ICS_SSA_DA_2_INVALID\n");
            (HI_VOID)SBP_Module_DA2_Recovery();
            SBP_Reset();
            return HI_FAILURE;
        }
        case ICS_MODULES_INVALID:
        {
            if(0 == u32ModuleStatus)
            {
                return HI_SUCCESS;
            }
            else
            {
                // TODO: Set Manual upgrade
                //(HI_VOID)SBP_Loader_SetManualmode();
                return HI_FAILURE;
            }
        }
        default:
        {
            return HI_SUCCESS;
        }
    }

    return ret;
}

#ifdef  HI_SBP_CHECK_BY_CRC
HI_S32 SBP_SignatureCheck(HI_U8 *pu8Data, HI_U32 u32MaxLen)
{
    HI_U32 u32SbpHeader = 0;
    HI_U32 u32Crc = 0;

    if((NULL == pu8Data) || (0 == u32MaxLen ))
    {
        HI_ERR_SBP("Invalid parameter\n");
        return HI_FAILURE;
    }

    u32SbpHeader = SBP_MSB2LSB_UINT(pu8Data);
    if((u32SbpHeader < SBP_HEADER_SIZE) || (u32SbpHeader > u32MaxLen))
    {
        HI_ERR_SBP("Invalid sbp header [%08x]\n", u32SbpHeader);
        return HI_FAILURE;
    }

    u32Crc = crc32(0, pu8Data + SBP_HEADER_SIZE, u32SbpHeader - SBP_HEADER_SIZE);

    pu8Data += u32SbpHeader;
    if(u32SbpHeader != SBP_MSB2LSB_UINT(pu8Data))
    {
        HI_ERR_SBP("Length in signature %x is different from SBP header %x\n", SBP_MSB2LSB_UINT(pu8Data), u32SbpHeader);
        return HI_FAILURE;
    }

    pu8Data += SBP_HEADER_SIZE;
    if(u32Crc != SBP_MSB2LSB_UINT(pu8Data))
    {
        HI_ERR_SBP("crc32 in signature %x is different from crc32 checked %x\n", SBP_MSB2LSB_UINT(pu8Data), u32Crc);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

TIntegrityCheckStatus SBP_IntegrityCheck(	TUMCSSelector     xUMCS)
{
    HI_S32 ret = HI_SUCCESS;
    TIntegrityCheckStatus status = 0;

    {
        HI_U32 u32SbpHeader, u32LoaderAddr, u32RecoveryAddr, u32AppAddr;

        ret = SBP_SignatureCheck((HI_U8 *)(SBP_BOOT_CODE_ADDR - 4), 0x80000);
        if(HI_SUCCESS != ret)
        {
            return ICS_LOCKED_MEMORY_INVALID;
        }

        u32SbpHeader = SBP_MSB2LSB_UINT((HI_U8 *)stSBPDataAll[SBP_DATA_DA2].u32RamAddr);
        stSBPDataAll[SBP_DATA_KERNEL].u32RamAddr = SBP_MSB2LSB_UINT((HI_U8 *)stSBPDataAll[SBP_DATA_DA2].u32RamAddr + 12) - SBP_HEADER_PART_SIZE;
        stSBPDataAll[SBP_DATA_RECOVERY].u32RamAddr = SBP_MSB2LSB_UINT((HI_U8 *)stSBPDataAll[SBP_DATA_DA2].u32RamAddr + 16) - SBP_HEADER_SIZE;
        stSBPDataAll[SBP_DATA_LOADER].u32RamAddr = SBP_MSB2LSB_UINT((HI_U8 *)stSBPDataAll[SBP_DATA_DA2].u32RamAddr + 20) - SBP_HEADER_PART_SIZE;
        HI_INFO_SBP("DA2 u32SbpHeader [0x%x], Kernel Addr [0x%x], Recovery Addr [0x%x], Loader Addr [0x%x]\n",
                u32SbpHeader,
                stSBPDataAll[SBP_DATA_KERNEL].u32RamAddr,
                stSBPDataAll[SBP_DATA_RECOVERY].u32RamAddr,
                stSBPDataAll[SBP_DATA_LOADER].u32RamAddr);
    }

    if(SSA_UNLOCKED_MEMORY_CONTAINER_SET_1 == xUMCS)   /*bootargs*/
    {
        ret = SBP_SignatureCheck(stSBPDataAll[SBP_DATA_BOOTARGS].u32RamAddr, stSBPDataAll[SBP_DATA_BOOTARGS].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_BOOTARGS);
        }
    }

    if(SSA_UNLOCKED_MEMORY_CONTAINER_SET_2 == xUMCS)   /*baseparam, logo*/
    {
        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_LOGO].u32RamAddr, stSBPDataAll[SBP_DATA_LOGO].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_LOGO);
        }
    }

    if(SSA_UNLOCKED_MEMORY_CONTAINER_SET_3 == xUMCS)   /*kernel,system,bootvideo*/
    {
        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_BOOTVIDEO].u32RamAddr, stSBPDataAll[SBP_DATA_BOOTVIDEO].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_BOOTVIDEO);
        }

        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_KERNEL].u32RamAddr, stSBPDataAll[SBP_DATA_KERNEL].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_KERNEL);
        }

        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_SYSTEM].u32RamAddr, stSBPDataAll[SBP_DATA_SYSTEM].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_SYSTEM);
        }
    }
    else if(SSA_UNLOCKED_MEMORY_CONTAINER_SET_4 == xUMCS)   /*recovery*/
    {
        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_RECOVERY].u32RamAddr, stSBPDataAll[SBP_DATA_RECOVERY].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_RECOVERY);
        }
    }
    else if(SSA_UNLOCKED_MEMORY_CONTAINER_SET_5 == xUMCS)   /*loader*/
    {
        ret = SBP_SignatureCheck((HI_U8 *)stSBPDataAll[SBP_DATA_LOADER].u32RamAddr, stSBPDataAll[SBP_DATA_LOADER].u32Len);
        if(HI_SUCCESS != ret)
        {
            status |= (ICS_MODULES_INVALID | SBP_MODULE_LOADER);
        }
    }

    if(0 == status)
        return ICS_VALID_NO_ERROR;
    else
        return status;
}

#else

#ifndef HI_ADVCA_FUNCTION_RELEASE
HI_VOID SBP_Print_IntegrityCheckResult(TSsaDebugData  *pstSbpDbg)
{
        HI_U32 i;
        HI_U8 *pu8Data = HI_NULL;

        HI_INFO_SBP("\n ******************pxSsaDebugData Start******************* \n");
        HI_INFO_SBP("      globalResult:                0x%x\n", (HI_U32)pstSbpDbg->globalResult);
        HI_INFO_SBP("      ssaDataArrayResult:     0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayResult);
        HI_INFO_SBP("      bootCodeResult:           0x%x\n", (HI_U32)pstSbpDbg->bootCodeResult);
        HI_INFO_SBP("      pairingDataResult:        0x%x\n", (HI_U32)pstSbpDbg->pairingDataResult);
        HI_INFO_SBP("      jumpCodeResult:          0x%x\n", (HI_U32)pstSbpDbg->jumpCodeResult);
        HI_INFO_SBP("      moduleResult:              0x%x\n", (HI_U32)pstSbpDbg->moduleResult);

        HI_INFO_SBP("\n");
        HI_INFO_SBP("      ssaDataArrayStatus.startAddress:              0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.startAddress);
        HI_INFO_SBP("      ssaDataArrayStatus.length:                       0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.length);
        HI_INFO_SBP("      ssaDataArrayStatus.signatureAddress:       0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.signatureAddress);
        HI_INFO_SBP("      ssaDataArrayStatus.signatureLength:         0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.signatureLength);
        HI_INFO_SBP("      ssaDataArrayStatus.doubleSigned:             0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.doubleSigned);
        HI_INFO_SBP("      ssaDataArrayStatus.firstCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.firstCheckKeyAddress);
        HI_INFO_SBP("      ssaDataArrayStatus.secondCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.secondCheckKeyAddress);
        HI_INFO_SBP("      ssaDataArrayStatus.status:                        0x%x\n", (HI_U32)pstSbpDbg->ssaDataArrayStatus.status);

        HI_INFO_SBP("\n");
        HI_INFO_SBP("      bootCodeStatus.startAddress:              0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.startAddress);
        HI_INFO_SBP("      bootCodeStatus.length:                       0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.length);
        HI_INFO_SBP("      bootCodeStatus.signatureAddress:       0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.signatureAddress);
        HI_INFO_SBP("      bootCodeStatus.signatureLength:         0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.signatureLength);
        HI_INFO_SBP("      bootCodeStatus.doubleSigned:             0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.doubleSigned);
        HI_INFO_SBP("      bootCodeStatus.firstCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.firstCheckKeyAddress);
        HI_INFO_SBP("      bootCodeStatus.secondCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.secondCheckKeyAddress);
        HI_INFO_SBP("      bootCodeStatus.status:                        0x%x\n", (HI_U32)pstSbpDbg->bootCodeStatus.status);

        HI_INFO_SBP("\n");
        HI_INFO_SBP("      bepCodeStatus.startAddress:              0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.startAddress);
        HI_INFO_SBP("      bepCodeStatus.length:                       0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.length);
        HI_INFO_SBP("      bepCodeStatus.signatureAddress:       0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.signatureAddress);
        HI_INFO_SBP("      bepCodeStatus.signatureLength:         0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.signatureLength);
        HI_INFO_SBP("      bepCodeStatus.doubleSigned:             0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.doubleSigned);
        HI_INFO_SBP("      bepCodeStatus.firstCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.firstCheckKeyAddress);
        HI_INFO_SBP("      bepCodeStatus.secondCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.secondCheckKeyAddress);
        HI_INFO_SBP("      bepCodeStatus.status:                        0x%x\n", (HI_U32)pstSbpDbg->bepCodeStatus.status);

        HI_INFO_SBP("\n");
        HI_INFO_SBP("      pairingDataStatus.startAddress:              0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.startAddress);
        HI_INFO_SBP("      pairingDataStatus.length:                       0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.length);
        HI_INFO_SBP("      pairingDataStatus.signatureAddress:       0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.signatureAddress);
        HI_INFO_SBP("      pairingDataStatus.signatureLength:         0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.signatureLength);
        HI_INFO_SBP("      pairingDataStatus.doubleSigned:             0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.doubleSigned);
        HI_INFO_SBP("      pairingDataStatus.firstCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.firstCheckKeyAddress);
        HI_INFO_SBP("      pairingDataStatus.secondCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.secondCheckKeyAddress);
        HI_INFO_SBP("      pairingDataStatus.status:                        0x%x\n", (HI_U32)pstSbpDbg->pairingDataStatus.status);

        HI_INFO_SBP("\n");
        HI_INFO_SBP("      secSsaDataArrayStatus.startAddress:              0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.startAddress);
        HI_INFO_SBP("      secSsaDataArrayStatus.length:                       0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.length);
        HI_INFO_SBP("      secSsaDataArrayStatus.signatureAddress:       0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.signatureAddress);
        HI_INFO_SBP("      secSsaDataArrayStatus.signatureLength:         0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.signatureLength);
        HI_INFO_SBP("      secSsaDataArrayStatus.doubleSigned:             0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.doubleSigned);
        HI_INFO_SBP("      secSsaDataArrayStatus.firstCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.firstCheckKeyAddress);
        HI_INFO_SBP("      secSsaDataArrayStatus.secondCheckKeyAddress:             0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.secondCheckKeyAddress);
        HI_INFO_SBP("      secSsaDataArrayStatus.status:                        0x%x\n", (HI_U32)pstSbpDbg->secSsaDataArrayStatus.status);

        for(i=0;i<DEBUG_NUMBER_OF_APP_MODULES;i++)
        {
            HI_INFO_SBP("\n");
            HI_INFO_SBP("      moduleStatus[%d].startAddress:              0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].startAddress);
            HI_INFO_SBP("      moduleStatus[%d].length:                       0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].length);
            HI_INFO_SBP("      moduleStatus[%d].signatureAddress:       0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].signatureAddress);
            HI_INFO_SBP("      moduleStatus[%d].signatureLength:         0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].signatureLength);
            HI_INFO_SBP("      moduleStatus[%d].doubleSigned:             0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].doubleSigned);
            HI_INFO_SBP("      moduleStatus[%d].firstCheckKeyAddress:             0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].firstCheckKeyAddress);
            HI_INFO_SBP("      moduleStatus[%d].secondCheckKeyAddress:             0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].secondCheckKeyAddress);
            HI_INFO_SBP("      moduleStatus[%d].status:                        0x%x\n",i, (HI_U32)pstSbpDbg->moduleStatus[i].status);
        }

        pu8Data = (HI_U8*)pstSbpDbg;
        HI_INFO_SBP("\npxSsaDebugData:");
        for(i=0;i<sizeof(TSsaDebugData);i++)
        {
            if(0==(i%16))
                HI_INFO_SBP("\n");
            HI_INFO_SBP("%02x ",pu8Data[i]);
        }
        HI_INFO_SBP("\n ******************stSbpDbg End******************* \n\n");
}

#endif

#endif

HI_S32 SBP_SsaIntegrityCheck(TUMCSSelector   xUMCS)
{
    TIntegrityCheckStatus u32Result;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 umcs = 0, i = xUMCS;


    while(i>0)
    {
        i = i>>1;
        umcs++;
    }

#ifdef HI_MINIBOOT_SUPPORT
    mmu_cache_enable();
#endif

#ifdef HI_SBP_CHECK_BY_CRC
    u32Result = SBP_IntegrityCheck(xUMCS);
    HI_INFO_SBP("SBP_IntegrityCheck,  UMCS [%d] result: [0x%x]\n", umcs, (HI_U32)u32Result);
#else

    #ifdef HI_ADVCA_FUNCTION_RELEASE
    u32Result = ssaIntegrityCheck(xUMCS);
    HI_INFO_SBP("ssaIntegrityCheck,  UMCS [%d] result: [0x%x]\n", umcs, (HI_U32)u32Result);

    #else
    TSsaDebugData  stSbpDbg;

    memset(&stSbpDbg, 0, sizeof(TSsaDebugData));
    u32Result = ssaIntegrityCheck(xUMCS, &stSbpDbg);
    HI_INFO_SBP("ssaIntegrityCheck, UMCS [%d], result [0x%x]\n", umcs, (HI_U32)u32Result);
    if(ICS_VALID_NO_ERROR != u32Result)
    {
        SBP_Print_IntegrityCheckResult(&stSbpDbg);
    }
    #endif
#endif

#ifdef HI_MINIBOOT_SUPPORT
    mmu_cache_disable();
#endif

    ret = SBP_Fault_Process(u32Result);

    return ret;
}

HI_S32 SBP_PK_Process(HI_U8 *pu8PK, HI_U32 u32Size)
{
    HI_S32 ret = HI_SUCCESS;
    TBsd4BytesVector    stbCaSn;
    unsigned char u8Nuid[4];

    if(HI_NULL == pu8PK)
    {
        HI_ERR_SBP("Null Pointer.\n");
        return HI_FAILURE;
    }

    if(0 == u32Size)
    {
        HI_ERR_SBP("Invalid size.\n");
        return HI_FAILURE;
    }

    ret = bsdInitialize(NULL);
    if(BSD_NO_ERROR != ret)
    {
        HI_ERR_SBP("bsdInitialize() Error, result %d\n", ret);
        return HI_FAILURE;
    }

    ret = bsdGetNuid(u8Nuid);
    if(BSD_NO_ERROR != ret)
    {
        HI_ERR_SBP("Get NUID error, result %d\n", ret);
        (HI_VOID)bsdTerminate(NULL);
        return HI_FAILURE;
    }
    HI_INFO_SBP("    NUID:  %02x%02x%02x%02x\n", u8Nuid[0],u8Nuid[1],u8Nuid[2],u8Nuid[3]);

#if 0
    {
        int i;
        HI_SIMPLEINFO_CA("First 16 bytes of Encrypted PK, Address [%08x], Size [%d]:\n",pu8PK,u32Size);
        for(i=0;i<16;i++)
        {
            HI_SIMPLEINFO_CA("%02x ",pu8PK[i]);
        }
        HI_SIMPLEINFO_CA("\n",ret);
    }
#endif

    ret = bsdDecryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_TDES, pu8PK, pu8PK, (u32Size/8)*8);
    if(BSD_NO_ERROR != ret)
    {
        HI_ERR_SBP("Decrypt PK Error, result %d\n", ret);
        (HI_VOID)bsdTerminate(NULL);
        return HI_FAILURE;
    }

#if 0
    {
        int i;
        HI_SIMPLEINFO_CA("First 16 bytes of Decrypted PK:\n");
        for(i=0;i<16;i++)
        {
            HI_SIMPLEINFO_CA("%02x ",pu8PK[i]);
        }
        HI_SIMPLEINFO_CA("\n",ret);
    }
#endif

    ret = bsdGetStbCaSn(stbCaSn);
    if(BSD_NO_ERROR != ret)
    {
        HI_ERR_SBP("Get STB_CA_SN Error, result %d\n", ret);
        (HI_VOID)bsdTerminate(NULL);
        return HI_FAILURE;
    }

    HI_INFO_SBP("    STB_CA_SN:  %02x%02x%02x%02x\n", stbCaSn[0],stbCaSn[1],stbCaSn[2],stbCaSn[3]);

    if(0!=memcmp(&stbCaSn[0], &pu8PK[4], 4))
    {
        HI_ERR_SBP("STB_CA_SN is incorrect, STB_CA_SN_REF [%02x%02x%02x%02x], STB_CA_SN_PK [%02x%02x%02x%02x]\n",
            stbCaSn[0],stbCaSn[1],stbCaSn[2],stbCaSn[3],
            pu8PK[4],pu8PK[5],pu8PK[6],pu8PK[7]);

        (HI_VOID)bsdTerminate(NULL);
        return HI_FAILURE;
    }

    (HI_VOID)bsdTerminate(NULL);
    return HI_SUCCESS;
}

#ifndef HI_SBP_CHECK_DEBUG
HI_S32 load_direct_env(void *env, unsigned int size)
{
    HI_S32 ret = HI_SUCCESS;
    //HI_U32  u32Addr = CFG_ENV_ADDR & (~( CFG_ENV_SIZE - 1));
    HI_U8 *pu8Buf = (HI_U8 *)stSBPDataAll[SBP_DATA_BOOTARGS].u32RamAddr;
    HI_U32 u32DataLen;
    HI_CHAR *pcSrc = HI_NULL;
    HI_U8* pGlobalEnvPtr;
    HI_U32 u32EnvSize;

    ret = SBP_ReadFlash_ByAddress(stSBPDataAll[SBP_DATA_BOOTARGS].enFlashType,
                                                        stSBPDataAll[SBP_DATA_BOOTARGS].u32Offset,
                                                        stSBPDataAll[SBP_DATA_BOOTARGS].u32Len,
                                                        pu8Buf,
                                                        HI_TRUE,
                                                        &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    memcpy(env, pu8Buf + SBP_HEADER_SIZE, size);

    return HI_SUCCESS;
}

HI_S32 SBP_Authenticate_Prepare(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8Data = HI_NULL;
    HI_U32 u32DataLen = 0;

    /*Should notice the order to avoid cover other data*/
    memcpy((HI_VOID *)stSBPDataAll[SBP_DATA_DA1].u32RamAddr, (HI_VOID *)SBP_DA1_RAM_ORG_ADDR, SBP_DA1_MAX_SIZE);
    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_DA2, HI_FALSE, &pu8Data, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Read DA1, DA2,PK to RAM Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }
    ret = SBP_PK_Process((HI_U8 *)stSBPDataAll[SBP_DATA_PK].u32RamAddr,stSBPDataAll[SBP_DATA_PK].u32Len);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("PK process Error, result %d\n", ret);
        SBP_Infinite_loop();
        return ret;
    }

    ret = SBP_SsaIntegrityCheck(SSA_UNLOCKED_MEMORY_CONTAINER_SET_1);
    if(HI_SUCCESS == ret)
    {
        (HI_VOID)SBP_Module_Sync(SBP_DATA_DA2, SBP_DATA_DA2_REDUNDANT);
    }

    return ret;

}
#endif

HI_S32 SBP_Authenticate_LogoData(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8Data = HI_NULL;
    HI_U32 u32DataLen = 0;

    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_LOGO, HI_TRUE, &pu8Data, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Read BaseParam and Logo to RAM Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }

    stSBPDataAll[SBP_DATA_LOGO].u32RamAddr = (HI_U32)pu8Data;

    ret = SBP_SsaIntegrityCheck(SSA_UNLOCKED_MEMORY_CONTAINER_SET_2);
    if(HI_SUCCESS == ret)
    {
        HI_PDM_MEMINFO_S FastPlayMemInfo;

        memset(&FastPlayMemInfo,0,sizeof(HI_PDM_MEMINFO_S));
        FastPlayMemInfo.u32LogoAddr = (HI_U32)(pu8Data + SBP_HEADER_PART_SIZE);
        FastPlayMemInfo.u32BaseAddr = (HI_U32)(pu8Data + SBP_HEADER_PART_SIZE + SBP_LOGO_SIZE);

        ret = HI_DRV_PDM_SetSource(HI_PDM_SRCTYPE_DDR, &FastPlayMemInfo);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_SBP("HI_DRV_PDM_SetSource Error, result %d\n", ret);
            return ret;
        }

        HI_INFO_SBP("Show Logo!\n");

    	/* show logo */
        Logo_Main(HI_FALSE, HI_NULL);
    }
    return ret;

}

HI_S32 SBP_Authenticate_Loader(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8Data = HI_NULL;
    HI_U32 u32DataLen = 0;

    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_LOADER, HI_TRUE, &pu8Data, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Read Loader partition to RAM Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }

    ret = SBP_SsaIntegrityCheck(SSA_UNLOCKED_MEMORY_CONTAINER_SET_5);

    return ret;

}

HI_S32 SBP_Authenticate_Recovery(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8Data = HI_NULL;
    HI_U32 u32DataLen = 0;

    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_RECOVERY, HI_TRUE, &pu8Data, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Read Recovery partition to RAM Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }

    ret = SBP_SsaIntegrityCheck(SSA_UNLOCKED_MEMORY_CONTAINER_SET_4);

    return ret;

}

HI_S32 SBP_Authenticate_Application(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 *pu8Data = HI_NULL;
    HI_U32 u32DataLen = 0;
    HI_PDM_MEMINFO_S FastPlayMemInfo;

    ret = SBP_CopyDataFromFlashToRAM(SBP_DATA_BOOTVIDEO, HI_TRUE, &pu8Data, &u32DataLen);
    ret |= SBP_CopyDataFromFlashToRAM(SBP_DATA_KERNEL, HI_TRUE, &pu8Data, &u32DataLen);
    ret |= SBP_CopyDataFromFlashToRAM(SBP_DATA_SYSTEM, HI_TRUE, &pu8Data, &u32DataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("Read Bootvideo, kernel, system partition to RAM Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }

    ret = SBP_SsaIntegrityCheck(SSA_UNLOCKED_MEMORY_CONTAINER_SET_3);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("SBP_SsaIntegrityCheck Application Error, result %d\n", ret);
        SBP_Reset();
        return ret;
    }

    memset(&FastPlayMemInfo,0,sizeof(HI_PDM_MEMINFO_S));
    FastPlayMemInfo.u32PlayAddr = (HI_U8 *)(stSBPDataAll[SBP_DATA_BOOTVIDEO].u32RamAddr + SBP_HEADER_PART_SIZE);
    FastPlayMemInfo.u32LogoAddr = (HI_U8 *)(stSBPDataAll[SBP_DATA_LOGO].u32RamAddr + SBP_HEADER_PART_SIZE);
    FastPlayMemInfo.u32BaseAddr = (HI_U8 *)(stSBPDataAll[SBP_DATA_LOGO].u32RamAddr + SBP_HEADER_PART_SIZE + SBP_LOGO_SIZE);

    ret = HI_DRV_PDM_SetSource(HI_PDM_SRCTYPE_DDR, &FastPlayMemInfo);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_DRV_PDM_SetSource Error, result %d\n", ret);
    }

	/* Set fastplay */
#ifdef HI_MCE_SUPPORT
    {
        HI_MCE_PARAM_S              stMceParam;
        HI_U32                      u32PlayDataAddr;

        ret = HI_DRV_PDM_GetMceParam(&stMceParam);
        if (HI_SUCCESS == ret && 1 == stMceParam.u32CheckFlag)
        {
            if (0 != stMceParam.u32PlayDataLen)
            {
                ret = HI_DRV_PDM_GetMceData(stMceParam.u32PlayDataLen, &u32PlayDataAddr);
            }
        }
    }
#endif

    HI_DRV_PDM_SetTagData();

    return ret;
}

HI_CHAR *SBP_SearchBootArgs(HI_CHAR *pSrc, HI_CHAR *pSubStr, HI_U32 len)
{
    HI_U32 subLen = 0;

    if (pSrc == HI_NULL || pSubStr == HI_NULL ||(len == 0))
    {
        HI_ERR_SBP("Invalid parameter.\n");
        return HI_NULL;
    }

    subLen = strlen(pSubStr);

    while (len >= subLen)
    {
        if (!memcmp(pSrc, pSubStr, subLen))
        {
            return pSrc;
        }
        pSrc++;
        len--;
    }

    return HI_NULL;
}

HI_S32 SBP_Boot_Load_Image(HI_SBP_DATA_TYPE_E enImageType)
{
    HI_U32  u32ModAddr = (HI_U32)stSBPDataAll[enImageType].u32RamAddr + SBP_HEADER_PART_SIZE;
    HI_U32  u32ModId = 0;
    HI_U32  u32ModNum = sizeof(enDA2Module)/sizeof(HI_SBP_DATA_TYPE_E);
#ifndef HI_MINIBOOT_SUPPORT
    HI_CHAR cmdStr[MAX_CMD_SIZE];
#endif

    if ((SBP_DATA_LOADER != enImageType) && (SBP_DATA_KERNEL != enImageType) && (SBP_DATA_RECOVERY != enImageType))
    {
        HI_ERR_SBP("Loading invalid image.\n");
        SBP_Reset();
        return HI_FAILURE;
    }

    for ( u32ModId = 0 ; u32ModId < u32ModNum ; u32ModId++ )
    {
        if(enImageType == enDA2Module[u32ModId])
            break;
    }

    if(u32ModId >= u32ModNum)
    {
        HI_ERR_SBP("Module [%d] is not in DA2.\n",enImageType);
        SBP_Reset();
        return HI_FAILURE;
    }

    u32ModAddr = SBP_MSB2LSB_UINT((HI_U8 *)(stSBPDataAll[SBP_DATA_DA2].u32RamAddr + SBP_HEADER_SIZE + u32ModId*sizeof(HI_U32)));
    if((u32ModAddr < SBP_DDR_START_ADDR)||(u32ModAddr > SBP_DDR_END_ADDR))
    {
            HI_ERR_SBP("Moduld [%d] address [0x%x] in DA2 is invalid.\n",enImageType, u32ModAddr);
            SBP_Reset();
            return HI_FAILURE;
    }

    HI_INFO_SBP("Load address is %x\n", u32ModAddr);

#ifdef HI_MINIBOOT_SUPPORT
    /*launch the module*/
    kern_load((const char*)u32ModAddr);
    SBP_Reset();
#else
    memset(cmdStr, 0, MAX_CMD_SIZE);
    snprintf(cmdStr, MAX_CMD_SIZE, "bootm 0x%x", u32ModAddr);
    setenv("bootcmd", cmdStr);
    HI_INFO_SBP("Loading image, bootcmd=%s\n", cmdStr);
#endif

    return HI_FAILURE;
}


HI_S32 HI_SSA_Init(HI_VOID)
{
   //To be done
   return HI_SUCCESS;
}


#ifdef HI_SBP_CHECK_DEBUG

HI_S32 SBP_ReadPartition(HI_CHAR * pszPartition, HI_U8 *pu8Buf, HI_U32 * pu32Len)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U32 u32OpenSize;

    if((HI_NULL == pszPartition)||(HI_NULL == pu8Buf)||(HI_NULL == pu32Len))
    {
        HI_ERR_SBP("Null pointer.\n");
        return HI_FAILURE;
    }

    if(((HI_U32)pu8Buf < SBP_DDR_START_ADDR)||((HI_U32)pu8Buf > SBP_DDR_END_ADDR))
    {
        HI_ERR_SBP("Invalid buffer address [0x%x]\n", (HI_U32)pu8Buf );
        return HI_FAILURE;
    }

    hFlash = HI_Flash_Open(HI_FLASH_TYPE_BUTT, pszPartition, 0, 0);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() %s error, result %x!\n",pszPartition,ret);
        return HI_FAILURE;
    }

    ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("HI_Flash_GetInfo() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }
    HI_INFO_SBP("HI_Flash_GetInfo() , FlashType [%d], Partition [%s], Addr [0x%llx], Partition Size [0x%llx], Flash Size [0x%llx]\n", stFlashInfo.FlashType, pszPartition, stFlashInfo.OpenAddr, stFlashInfo.PartSize, stFlashInfo.TotalSize);

    (HI_VOID)HI_Flash_Close(hFlash);

    /*The opened size should larger then partition size to avoid read failure for  bad block */
    hFlash = HI_Flash_Open(stFlashInfo.FlashType, HI_NULL, stFlashInfo.OpenAddr, stFlashInfo.TotalSize-stFlashInfo.OpenAddr);
    if(HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_SBP("HI_Flash_Open() %s error!\n", pszPartition);
        return HI_FAILURE;
    }

    ret = HI_Flash_Read(hFlash, 0, pu8Buf, stFlashInfo.PartSize, HI_FLASH_RW_FLAG_RAW);
    if(ret <= 0)
    {
        HI_ERR_SBP("HI_Flash_Read() error, result %x!\n",ret);
        (HI_VOID)HI_Flash_Close(hFlash);
        return ret;
    }

    (HI_VOID)HI_Flash_Close(hFlash);

    *pu32Len = (HI_U32)stFlashInfo.PartSize;
    return HI_SUCCESS;
}


//HI_VOID HI_NASC_SBP_Process(HI_VOID)
HI_S32 HI_SSA_Entry()
{
    HI_S32 ret = HI_SUCCESS;
    HI_SBP_DATA_TYPE_E enImageType;
    HI_CHAR szPartitionName[32];
    HI_CHAR *pszBootargs = HI_NULL;
    HI_CHAR szStr[MAX_BOOT_ARGS_SIZE];
    HI_U8 * pu8Data = (HI_U8 *)SBP_TMP_BUF_ADDR;

    HI_INFO_SBP("************ Start ****************\n\n");

#ifdef HI_BUILD_WITH_KEYLED
    HI_UNF_KEYLED_Init();
    HI_UNF_LED_Open();
    LED_ShowBLString("boot");
#endif

    c51_loadCode();

#if 0
    SBP_Loader_ManualUpgradeCheck();
#endif

#ifdef HI_ANDROID_BOOT_SUPPORT
    set_default_ethaddr();
#endif

    Logo_Main(HI_TRUE, HI_NULL);

    HI_DRV_PDM_SetTagData();

    //IR_Test();

    enImageType = SBP_Loader_Check();
    memset(szPartitionName,0,sizeof(szPartitionName));
    if (SBP_DATA_LOADER == enImageType)
    {
        LED_ShowBLString("LoAd");
        snprintf(szPartitionName, 32, "loader");
    }
    else if (SBP_DATA_RECOVERY == enImageType)
    {
        LED_ShowBLString("LoAd");
        snprintf(szPartitionName, 32, "recovery");
    }
    else
    {
        snprintf(szPartitionName, 32, "kernel");
    }

    HI_INFO_SBP("Loading image [%s]\n", szPartitionName);
    pszBootargs = getenv("bootargs");
    HI_INFO_SBP("bootargs = %s\n",pszBootargs);

    {
        HI_U32 u32Len;
        HI_CHAR *pszStr = HI_NULL;

        ret = SBP_ReadPartition(szPartitionName, pu8Data, &u32Len);
        if(HI_SUCCESS != ret)
            return;

        memset(szStr, 0, MAX_BOOT_ARGS_SIZE);
        snprintf(szStr, MAX_BOOT_ARGS_SIZE, "bootm 0x%x", pu8Data);
        setenv("bootcmd", szStr);
    }

    HI_INFO_SBP("************ End ****************\n\n");
    return;
}

#else

//HI_VOID HI_NASC_SBP_Process(HI_VOID)
HI_S32 HI_SSA_Entry()
{
    HI_S32 ret = HI_SUCCESS;
    HI_SBP_DATA_TYPE_E enImageType;

    HI_INFO_SBP("************ Start ****************\n\n");

#ifdef HI_BUILD_WITH_KEYLED
    HI_UNF_KEYLED_Init();
    HI_UNF_LED_Open();
    LED_ShowBLString("boot");
#endif

    c51_loadCode();

#if 0
    SBP_Loader_ManualUpgradeCheck();
#endif

    ret = SBP_Authenticate_Prepare();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SBP("SBP_Authenticate_Prepare() Failure.\n");
        SBP_Reset();
        return;
    }

#ifdef HI_ANDROID_BOOT_SUPPORT
    set_default_ethaddr();
#endif

    (HI_VOID)SBP_Authenticate_LogoData();

    //IR_Test();

    enImageType = SBP_Loader_Check();

    if ((SBP_DATA_LOADER != enImageType) && (SBP_DATA_RECOVERY != enImageType))
    {
        /*There are three cases for SBP_Authenticate_Application() :
           1) HI_SUCCESS :  all modules are OK
           2) RESET : boot code or bootargs is corrupted.
           3) HI_FAILURE: some modules excpet  bootargs are corrupted, should recovery them by loader*/
        ret = SBP_Authenticate_Application();

        if(HI_SUCCESS == ret)
        {
            HI_INFO_SBP("Loading 'application' image.\n");
            ret = SBP_Boot_Load_Image(SBP_DATA_KERNEL);
            if(HI_SUCCESS == ret)
            {
                return;
            }
            else
            {
                HI_ERR_SBP("Load 'application' image Failure.\n");
                SBP_Reset();
                return;
            }
        }
        else
        {
            // TODO: Add Manual upgrade here
            //(HI_VOID)SBP_Loader_SetManualmode();
            HI_ERR_SBP("Authenticate 'application' image Failure.\n");
            SBP_Reset();
            return;
        }
    }

    if (SBP_DATA_LOADER == enImageType)
    {
        LED_ShowBLString("LoAd");
        /*There are three cases for SBP_Authenticate_Loader() :
           1) HI_SUCCESS :  all modules are OK
           2) RESET : boot code or bootargs, loader is corrupted.
           3) HI_FAILURE: some modules excpet loader and bootargs are corrupted, should recovery them by loader*/
        ret = SBP_Authenticate_Loader();
        if(HI_SUCCESS == ret)
        {
            HI_INFO_SBP("Loading 'loader' image.\n");
            ret = SBP_Boot_Load_Image(SBP_DATA_LOADER);
            if(HI_SUCCESS == ret)
            {
                return;
            }
        }
        else
        {
            HI_ERR_SBP("Authenticate 'loader' image Failure.\n");
            SBP_Reset();
            return;
        }
    }

    if (SBP_DATA_RECOVERY == enImageType)
    {
        LED_ShowBLString("LoAd");
        /*There are three cases for SBP_Authenticate_Loader() :
           1) HI_SUCCESS :  all modules are OK
           2) RESET : boot code or bootargs, loader is corrupted.
           3) HI_FAILURE: some modules excpet loader and bootargs are corrupted, should recovery them by loader*/
        ret = SBP_Authenticate_Recovery();
        if(HI_SUCCESS == ret)
        {
            HI_INFO_SBP("Loading 'recovery' image.\n");
            ret = SBP_Boot_Load_Image(SBP_DATA_RECOVERY);
            if(HI_SUCCESS == ret)
            {
                return;
            }
            else
            {
                HI_ERR_SBP("Load 'recovery' image Failure.\n");
                SBP_Reset();
                return;
            }
        }
        else
        {
            HI_ERR_SBP("SBP_Authenticate_Recovery Failure.\n");
            SBP_Reset();
        }
    }

    HI_INFO_SBP("************ End ****************\n\n");
    return;
}

#endif

