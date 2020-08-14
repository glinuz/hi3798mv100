/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hifile_parse.c
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
#include "hifile_parse.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
#define HIFILE_D_MAGICNUM       0x4C4F4144
#define HIFILE_D_HEARDCRC_POS   4
#define HIFILE_D_HEARDCRC_LEN   4

/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/
HI_S32 HIFILE_ParseHeader
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_DOCUMENT_S*  pstDocument
)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U16 u16ManuNum = 0;
    HI_U32 u32TotalRead = 0;
    HI_U32 u32MagicNum = 0;
    HI_U32 u32HeaderCrc = 0;
    HI_U32 u32CalculateCRC = 0;
    HI_U16 u16NameLen = 0;
    HI_U16 u16ReserveLen = 0;
    HI_U8* pcBuffer = pu8FileData;
    HIFILE_MANUINFO_S  stManuInfo;
    HIFILE_FLASHMAP_S* pstFlashMap = HI_NULL_PTR;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8FileData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDocument);
    COM_CHECK_PARAM(0 == u32FileSize);

    HI_READ_32BIT(pcBuffer, u32MagicNum, u32TotalRead);

    if (HIFILE_D_MAGICNUM != u32MagicNum)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_32BIT(pcBuffer, u32HeaderCrc, u32TotalRead);
    HI_READ_32BIT(pcBuffer, pstDocument->u32HeaderLen, u32TotalRead);

    if (u32FileSize < pstDocument->u32HeaderLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_MemSet(pu8FileData + HIFILE_D_HEARDCRC_POS, 0x00, HIFILE_D_HEARDCRC_LEN);
    u32CalculateCRC = COM_CRC32(0xFFFFFFFFL, pu8FileData, pstDocument->u32HeaderLen);
    if (u32CalculateCRC != u32HeaderCrc)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

#if defined (HI_LOADER_PROTOCOL_VER2)
    HI_SKIP_BYTES(pcBuffer, 4, u32TotalRead);
#endif

    HI_READ_32BIT(pcBuffer, pstDocument->u32FileLen, u32TotalRead);
    HI_READ_16BIT(pcBuffer, pstDocument->u16ManuNum, u32TotalRead);

    if ((0 == pstDocument->u16ManuNum)
        || (HIFILE_D_MAXNUM_MANUFACTURER < pstDocument->u16ManuNum))
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    for (i = 0; i < pstDocument->u16ManuNum; ++i)
    {
        pstManuInfo = &pstDocument->astManuInfo[i];

        HI_READ_32BIT(pcBuffer, pstManuInfo->u32ManufacturerID, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32HardwareVersion, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32SoftwareVersion, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32SerialNumberStart, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32SerialNumberEnd, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32DownloadType, u32TotalRead);
        HI_READ_32BIT(pcBuffer, pstManuInfo->u32Reserved, u32TotalRead);
        HI_READ_16BIT(pcBuffer, pstManuInfo->u16FlashMapNum, u32TotalRead);

        if ((0 == pstManuInfo->u16FlashMapNum)
            || (HIFILE_D_MAXNUM_FLASHMAP < pstManuInfo->u16FlashMapNum))
        {
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        for (j = 0; j < pstManuInfo->u16FlashMapNum; ++j)
        {
            pstFlashMap = &pstManuInfo->astFlashMap[j];

            HI_READ_32BIT(pcBuffer, pstFlashMap->u32ImageLength, u32TotalRead);
            HI_READ_32BIT(pcBuffer, pstFlashMap->u32ImageOffset, u32TotalRead);
#if defined (HI_LOADER_PROTOCOL_VER3) || defined (HI_LOADER_PROTOCOL_VER2)
            HI_READ_64BIT(pcBuffer, pstFlashMap->u64PartitionStartAddr, u32TotalRead);
            HI_READ_64BIT(pcBuffer, pstFlashMap->u64PartitionEndAddr, u32TotalRead);
#elif defined(HI_LOADER_PROTOCOL_VER1)
            HI_READ_32BIT(pcBuffer, pstFlashMap->u64PartitionStartAddr, u32TotalRead);
            HI_READ_32BIT(pcBuffer, pstFlashMap->u64PartitionEndAddr, u32TotalRead);
#endif
            HI_READ_32BIT(pcBuffer, pstFlashMap->u32FlashType, u32TotalRead);
            HI_READ_32BIT(pcBuffer, pstFlashMap->u32FlashIndex, u32TotalRead);

#if defined (HI_LOADER_PROTOCOL_VER3)
            HI_READ_32BIT(pcBuffer, pstFlashMap->u32PartVersion, u32TotalRead);
            HI_READ_32BIT(pcBuffer, pstFlashMap->u32PartVerifyTag, u32TotalRead);
            HI_READ_16BIT(pcBuffer, u16NameLen, u32TotalRead);
            HI_READ_BYTES(pcBuffer, u16NameLen, pstFlashMap->au8PartName, HIFILE_D_MAXLEN_PARTNAME, u32TotalRead);
            HI_READ_16BIT(pcBuffer, u16ReserveLen, u32TotalRead);
            HI_SKIP_BYTES(pcBuffer, u16ReserveLen, u32TotalRead);
#endif
            pstManuInfo->u32ImageLength += pstFlashMap->u32ImageLength;
            pstFlashMap->u32ImageID = j;
        }
    }

    if (u32TotalRead != pstDocument->u32HeaderLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIFILE_ParseData
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_MANUINFO_S*  pstManuInfo
)
{
    HI_U16 i = 0;
    HI_U32 u32ImageLength = 0;
    HI_U32 u32CalculateCRC = 0;
    HI_U32 u32TotalRead = 0;
    HI_U8* pu8DataAddr = HI_NULL_PTR;
    HIFILE_FLASHMAP_S* pstFlashMap = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstManuInfo);
    COM_CHECK_PARAM(HI_NULL_PTR == pu8FileData);
    COM_CHECK_PARAM(0 == u32FileSize);

    for (i = 0; i < pstManuInfo->u16FlashMapNum; i++)
    {
        pstFlashMap = &pstManuInfo->astFlashMap[i];
        pu8DataAddr = pu8FileData + pstFlashMap->u32ImageOffset;

        HI_READ_32BIT(pu8DataAddr, u32ImageLength, u32TotalRead);

        if (u32ImageLength != pstFlashMap->u32ImageLength)
        {
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        HI_READ_32BIT(pu8DataAddr, pstFlashMap->u32ImageCRC, u32TotalRead);
        u32CalculateCRC = COM_CRC32(0xFFFFFFFFL, pu8DataAddr, u32ImageLength);
        if (u32CalculateCRC != pstFlashMap->u32ImageCRC)
        {
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        pstFlashMap->pu8ImageData = pu8DataAddr;
    }

    if (u32TotalRead > u32FileSize)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIFILE_ParseWholeFile
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_DOCUMENT_S*  pstDocument
)
{
    HI_U16 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8FileData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDocument);
    COM_CHECK_PARAM(0 == u32FileSize);

    s32Ret = HIFILE_ParseHeader(pu8FileData, u32FileSize, pstDocument);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ParseHeader, s32Ret);
        return s32Ret;
    }

    for (i = 0; i < pstDocument->u16ManuNum; i++)
    {
        s32Ret = HIFILE_ParseData(pu8FileData, u32FileSize, &pstDocument->astManuInfo[i]);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HIFILE_ParseData, s32Ret);
            return s32Ret;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


