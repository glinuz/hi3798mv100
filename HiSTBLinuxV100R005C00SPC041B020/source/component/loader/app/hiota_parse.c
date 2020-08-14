/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hiota_parse_v3.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/
#include "com_osal.h"
#include "com_debug.h"
#include "com_parse.h"
#include "hiota_parse.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/

#define HIOTA_D_DATAGRAM_MAX_SECTION_NUM    (8)
#define HIOTA_D_FLASHTYPE_DEFAULT           (OTA_FLASHTYPE_NAND)
#define HIOTA_D_FLASHINDEX_DEFAULT          (0)
#define HIOTA_D_DLMODE_PARTITION            (0)
#define HIOTA_D_DLMODE_FILE                 (1)


/*************************** Structure Definition *****************************/


/*************************** API Declaration **********************************/


static HI_S32 hiotaParseDownloadInfo
(
    HI_U8*                  pu8DownloadInfoData,
    HI_U32                  u32DownLoadInfoLen,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
)
{
    HI_U32 u32TotalRead = 0;
    HI_U32 u32HardwareStringLen = 0;
    HI_U8* pu8Data = pu8DownloadInfoData;

    COM_DBG_FuncEnter();

    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32ManufacturerID,    u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32HardwareVersion,   u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32SoftwareVersion,   u32TotalRead);
    HI_READ_08BIT(pu8Data, pstUpgradeInfo->u8DownloadTableId,    u32TotalRead);
    HI_READ_08BIT(pu8Data, pstUpgradeInfo->u8KeyContorl,         u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32SerialNumberStart, u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32SerialNumberEnd,   u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32DownloadDate,      u32TotalRead);

    if (0 != (HIOTA_PARTITION_VERSION_APP & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32APPVersion, u32TotalRead);
    }

    if (0 != (HIOTA_PARTITION_VERSION_KERNEL & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32KernelVersion, u32TotalRead);
    }

    if (0 != (HIOTA_PARTITION_VERSION_CA & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32CAVersion, u32TotalRead);
    }

    if (0 != (HIOTA_PARTITION_VERSION_BOOTLOADER & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32BootLoaderVersion, u32TotalRead);
    }

    if (0 != (HIOTA_PARTITION_VERSION_LOADER & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32LoaderVersion, u32TotalRead);
    }

    if (0 != (HIOTA_PARTITION_VERSION_LOGO & pstUpgradeInfo->u8KeyContorl))
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32LogoVersion, u32TotalRead);
    }

    HI_READ_08BIT(pu8Data, u32HardwareStringLen, u32TotalRead);
    HI_READ_BYTES(pu8Data, u32HardwareStringLen, pstUpgradeInfo->acHardwareString, HIOTA_D_LEN_HWSTRING, u32TotalRead);

    if (u32TotalRead != u32DownLoadInfoLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 hiotaParseDownloadPartInfo
(
    HI_U8*                  pu8DownloadPartData,
    HI_U32                  u32DownLoadPartLen,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
)
{
    HI_U8  i = 0;
    HI_U8  u8Reserved = 0;
    HI_U8  u8DownloadModeDataLen = 0;
    HI_U8  u8DownloadStringLen = 0;
    HI_U16 u16ReservedDataLen = 0;
    HI_U16 u16PartDescritptionLength = 0;
    HI_U8  u8DownloadDescriptionLen = 0;
    HI_U32 u32TotalRead = 0;
    HI_U8* pu8Data = pu8DownloadPartData;

    HIOTA_PARTITION_S* pstTPartInfo = HI_NULL_PTR;

    COM_DBG_FuncEnter();

#if defined (HI_LOADER_PROTOCOL_VER2)
    HI_SKIP_BYTES(pu8Data, 4, u32TotalRead);
#endif

    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32DownloadDataTotalsize, u32TotalRead);
    HI_READ_08BIT(pu8Data, pstUpgradeInfo->u8PartitionCount, u32TotalRead);

    if (0 == pstUpgradeInfo->u8PartitionCount)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8Data, u8Reserved, u32TotalRead)
    HI_READ_16BIT(pu8Data, u16PartDescritptionLength, u32TotalRead);

    if (u16PartDescritptionLength + u32TotalRead > u32DownLoadPartLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    for (i = 0; i < pstUpgradeInfo->u8PartitionCount; i++)
    {
        pstTPartInfo = &pstUpgradeInfo->astPartition[i];
        pstTPartInfo->u8PartitionNumber = HIOTA_GetPartitionNumber(i);

        HI_READ_08BIT(pu8Data, pstTPartInfo->u8DownloadMode, u32TotalRead);

        if ((HIOTA_D_DLMODE_PARTITION != pstTPartInfo->u8DownloadMode)
            && (HIOTA_D_DLMODE_FILE != pstTPartInfo->u8DownloadMode))
        {
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        HI_READ_08BIT(pu8Data, u8DownloadModeDataLen, u32TotalRead);

        if (HIOTA_D_DLMODE_PARTITION == pstTPartInfo->u8DownloadMode)
        {
#if defined (HI_LOADER_PROTOCOL_VER3)
            HI_READ_64BIT(pu8Data, pstTPartInfo->u64DownloadAddr,  u32TotalRead);
#elif defined(HI_LOADER_PROTOCOL_VER2)
            HI_READ_32BIT(pu8Data, pstTPartInfo->u64DownloadAddr,  u32TotalRead);
#elif defined(HI_LOADER_PROTOCOL_VER1)
            HI_READ_32BIT(pu8Data, pstTPartInfo->u64DownloadAddr,  u32TotalRead);
#endif
            HI_READ_32BIT(pu8Data, pstTPartInfo->u32DownloadSize,  u32TotalRead);
            HI_READ_32BIT(pu8Data, pstTPartInfo->u32DownloadCRC32, u32TotalRead);
        }
        else
        {
            HI_READ_08BIT(pu8Data, u8DownloadStringLen, u32TotalRead);
            HI_READ_BYTES(pu8Data, u8DownloadStringLen, pstTPartInfo->acDownLoadString, HIOTA_D_LEN_DOWNLOADSTRING, u32TotalRead);
            HI_READ_32BIT(pu8Data, pstTPartInfo->u32DownloadSize,  u32TotalRead);
            HI_READ_32BIT(pu8Data, pstTPartInfo->u32DownloadCRC32, u32TotalRead);
        }
    }

    HI_READ_08BIT(pu8Data, u8DownloadDescriptionLen, u32TotalRead);

    if (u8DownloadDescriptionLen + u32TotalRead > u32DownLoadPartLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_BYTES(pu8Data, u8DownloadDescriptionLen, pstUpgradeInfo->acDownloadDescription, HIOTA_D_LEN_DLDESCRIPTION, u32TotalRead);
    HI_READ_08BIT(pu8Data, pstUpgradeInfo->u8ReservedTag, u32TotalRead);
    HI_READ_16BIT(pu8Data, u16ReservedDataLen, u32TotalRead);

    if (1 == pstUpgradeInfo->u8ReservedTag)
    {
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32FullCRC32, u32TotalRead);
        HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32MagicNum,  u32TotalRead);
    }

    if (u32TotalRead != u32DownLoadPartLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 hiotaParseDownloadControl
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
)
{
    HI_U8  u8Tag   = 0;
    HI_U32 u32TotalRead = 0;
    HI_U32 u32DownLoadInfoLen = 0;
    HI_U32 u32DownloadPartInfoLen = 0;
    HI_U8* pu8Data = pu8SectionData;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    HI_READ_08BIT(pu8Data, u8Tag, u32TotalRead);

    if (HIOTA_D_DOWNLOAD_INFOTAG != u8Tag)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8Data, u32DownLoadInfoLen, u32TotalRead);

    if (0 == u32DownLoadInfoLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    if (u32DownLoadInfoLen + u32TotalRead > u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    s32Ret = hiotaParseDownloadInfo(pu8Data, u32DownLoadInfoLen, pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hiotaParseDownloadInfo, s32Ret);
        return s32Ret;
    }

    HI_SKIP_BYTES(pu8Data, u32DownLoadInfoLen, u32TotalRead);
    HI_READ_08BIT(pu8Data, u8Tag, u32TotalRead);

    if (HIOTA_D_DOWNLOAD_PARTINFOTAG != u8Tag)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8Data, u32DownloadPartInfoLen, u32TotalRead);

    if (0 == u32DownloadPartInfoLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    if (u32DownloadPartInfoLen + u32TotalRead > u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    s32Ret = hiotaParseDownloadPartInfo(pu8Data, u32DownloadPartInfoLen, pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hiotaParseDownloadPartInfo, s32Ret);
        return s32Ret;
    }

    HI_SKIP_BYTES(pu8Data, u32DownloadPartInfoLen, u32TotalRead);
    HI_READ_32BIT(pu8Data, pstUpgradeInfo->u32CRC32, u32TotalRead);

    if (u32TotalRead != u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_BOOL hiotaParsePartitionControl
(
    HI_U8*              pu8SecData,
    HI_U32              u32SectionDataLen,
    HIOTA_PARTITION_S*  pstPartition
)
{
    HI_U8  u8PartHeadTag = 0;
    HI_U8  u8PartHeadLen = 0;
    HI_U8  u8ReservedTag = 0;
    HI_U8  u8StartStringLen = 0;
    HI_U16 u16NameLen = 0;
    HI_U16 u16ReservedDataLen = 0;
    HI_U32 u32TotalRead = 0;
    HI_U8* pu8Data = pu8SecData;

    COM_DBG_FuncEnter();

    HI_READ_08BIT(pu8Data, u8PartHeadTag, u32TotalRead);

    if (HIOTA_D_PARTITION_HEADTAG != u8PartHeadTag)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8Data, u8PartHeadLen, u32TotalRead);

    if (u8PartHeadLen + u32TotalRead != u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8Data, pstPartition->u8DownLoadType,        u32TotalRead);
    HI_READ_08BIT(pu8Data, pstPartition->u8Reserved,            u32TotalRead);
    HI_READ_16BIT(pu8Data, pstPartition->u16PartDatagramNumber, u32TotalRead);
    HI_READ_32BIT(pu8Data, pstPartition->u32PartTotalSize,      u32TotalRead);
    HI_READ_32BIT(pu8Data, pstPartition->u32PartOriSize,        u32TotalRead);
    HI_READ_32BIT(pu8Data, pstPartition->u32PartOldVerStart,    u32TotalRead);
    HI_READ_32BIT(pu8Data, pstPartition->u32PartOldVerEnd,      u32TotalRead);
    HI_READ_32BIT(pu8Data, pstPartition->u32PartNewVer,         u32TotalRead);

    if (HIOTA_D_DLMODE_PARTITION == pstPartition->u8DownLoadType)
    {
#if defined (HI_LOADER_PROTOCOL_VER3) || defined (HI_LOADER_PROTOCOL_VER2)
        HI_READ_64BIT(pu8Data, pstPartition->u64FlashStartAddr, u32TotalRead);
#elif defined(HI_LOADER_PROTOCOL_VER1)
        HI_READ_32BIT(pu8Data, pstPartition->u64FlashStartAddr, u32TotalRead);
#endif
    }
    else
    {
        HI_READ_08BIT(pu8Data, u8StartStringLen, u32TotalRead);
        HI_READ_BYTES(pu8Data, u8StartStringLen, pstPartition->acStartString, HIOTA_D_LEN_STARTSTRING, u32TotalRead);
    }

    HI_READ_16BIT(pu8Data, u16NameLen, u32TotalRead);
    HI_READ_BYTES(pu8Data, u16NameLen, pstPartition->au8PartName, HIOTA_D_MAXLEN_PARTNAME, u32TotalRead);

    HI_READ_08BIT(pu8Data, u8ReservedTag, u32TotalRead);
    HI_READ_16BIT(pu8Data, u16ReservedDataLen, u32TotalRead);

    if (1 == u8ReservedTag)
    {
        HI_READ_32BIT(pu8Data, pstPartition->u32FlashType, u32TotalRead);
        HI_READ_32BIT(pu8Data, pstPartition->u32FlashIndex, u32TotalRead);
#if defined (HI_LOADER_PROTOCOL_VER3)
        HI_READ_64BIT(pu8Data, pstPartition->u64FlashEndAddr, u32TotalRead);
        HI_READ_32BIT(pu8Data, pstPartition->u32PartVerifyTag, u32TotalRead);
#elif defined (HI_LOADER_PROTOCOL_VER2)
        HI_READ_64BIT(pu8Data, pstPartition->u64FlashEndAddr, u32TotalRead);
#elif defined (HI_LOADER_PROTOCOL_VER1)
        HI_READ_32BIT(pu8Data, pstPartition->u64FlashEndAddr, u32TotalRead);
#endif
    }
    else
    {
        HI_SKIP_BYTES(pu8Data, u16ReservedDataLen, u32TotalRead);
        pstPartition->u32FlashType  = HIOTA_D_FLASHTYPE_DEFAULT;
        pstPartition->u32FlashIndex = HIOTA_D_FLASHINDEX_DEFAULT;
    }

    if (u32TotalRead != u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_BOOL hiotaParseDatagram
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_DATAGRAM_INFO_S*  pstDatagram
)
{
    HI_U32 u32TotalRead = 0;
    HI_U32 u32CRC32 = 0;
    HI_U32 u32CalculateCRC = 0;
    HI_U32 u32MagicNum = 0;
    HI_U8  u8SectionNumber = 0;
    HI_U16 u16ReservedDataLength = 0;

    COM_DBG_FuncEnter();

    HI_READ_32BIT(pu8SectionData, u32MagicNum, u32TotalRead);
    HI_READ_16BIT(pu8SectionData, u16ReservedDataLength, u32TotalRead);
    HI_SKIP_BYTES(pu8SectionData, u16ReservedDataLength, u32TotalRead);
    HI_READ_08BIT(pu8SectionData, u8SectionNumber, u32TotalRead);

    if (0 == (u8SectionNumber & 0x80))
    {
        pstDatagram->u32SectionIndex = pstDatagram->u16DatagramNum - 1;
    }
    else
    {
        pstDatagram->u32SectionIndex = ((pstDatagram->u16DatagramNum - 1) * HIOTA_D_DATAGRAM_MAX_SECTION_NUM)
                                     + ((u8SectionNumber & 0x0f) - 1);
    }


    HI_READ_16BIT(pu8SectionData, pstDatagram->u16DataLength, u32TotalRead);

    if ((HI_U32)pstDatagram->u16DataLength + u32TotalRead >= u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pstDatagram->pu8Data = pu8SectionData;
    HI_SKIP_BYTES(pu8SectionData, pstDatagram->u16DataLength, u32TotalRead);
    HI_READ_32BIT(pu8SectionData, u32CRC32, u32TotalRead);

    if (u32TotalRead != u32SectionDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    u32CalculateCRC = COM_CRC32(0xFFFFFFFFL, pstDatagram->pu8Data, pstDatagram->u16DataLength);
    if (u32CalculateCRC != u32CRC32)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }


    COM_DBG_FuncExit();
    return HI_SUCCESS;

}


HI_S32 HIOTA_ParseDatagramSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_DATAGRAM_INFO_S*  pstDatagramInfo
)
{
    COM_SECTION_S stSection;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8SectionData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDatagramInfo);
    COM_CHECK_PARAM(0 == u32SectionDataLen);

    COM_MemSet(&stSection, 0x00, sizeof(stSection));
    s32Ret = COM_ParseSection(pu8SectionData, u32SectionDataLen, &stSection);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_ParseSection, s32Ret);
        return s32Ret;
    }

    pstDatagramInfo->u16DatagramNum = stSection.u16TableIDExtension;
    pstDatagramInfo->u8PartitionNumber = stSection.u8SectionNumber;

    s32Ret = hiotaParseDatagram(stSection.pu8PrivateData,
                                stSection.u32PrivateDataLen,
                                pstDatagramInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hiotaParseDatagram, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ParseDownloadControlSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
)
{
    HI_S32 s32Ret = HI_FAILURE;
    COM_SECTION_S stSection;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8SectionData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstUpgradeInfo);
    COM_CHECK_PARAM(0 == u32SectionDataLen);

    COM_MemSet(&stSection, 0x00, sizeof(stSection));
    s32Ret = COM_ParseSection(pu8SectionData, u32SectionDataLen, &stSection);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_ParseSection, s32Ret);
        return s32Ret;
    }

    if (HIOTA_D_DOWNLOAD_TABID != stSection.u8TableID)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    if (0 != stSection.u8SectionNumber)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    if (HIOTA_D_DOWNLOAD_TABEXTID != stSection.u16TableIDExtension)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    s32Ret = hiotaParseDownloadControl(stSection.pu8PrivateData,
                                       stSection.u32PrivateDataLen,
                                       pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hiotaParseDownloadControl, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ParsePartitionControlSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_PARTITION_S*      pstPartition
)
{
    HI_S32 s32Ret = HI_FAILURE;
    COM_SECTION_S stSection;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8SectionData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstPartition);
    COM_CHECK_PARAM(0 == u32SectionDataLen);

    COM_MemSet(&stSection, 0x00, sizeof(stSection));
    s32Ret = COM_ParseSection(pu8SectionData, u32SectionDataLen, &stSection);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_ParseSection, s32Ret);
        return s32Ret;
    }

    s32Ret = hiotaParsePartitionControl(stSection.pu8PrivateData,
                                        stSection.u32PrivateDataLen,
                                        pstPartition);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hiotaParsePartitionControl, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
