/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssd_parse.c
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
#include "com_parse.h"
#include "com_debug.h"
#include "ssd_router.h"
#include "ssd_parse.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/

/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/




HI_S32 ADVCA_ParseFileData
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_DOCUMENT_S*  pstDocument
)
{
    HI_U16  i = 0;
    HI_U32  u32ImageSize = 0;
    HI_U8*  pu8ImageData = HI_NULL_PTR;
    HIFILE_FLASHMAP_S* pstFlashMap = HI_NULL_PTR;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK_PARAM(HI_NULL_PTR == pu8FileData);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDocument);
    COM_CHECK_PARAM(0 == u32FileSize);

    s32Ret = ADVCA_SSD_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ADVCA_SSD_Init, s32Ret);
        return s32Ret;
    }

    s32Ret = ADVCA_SSD_WholeImageVerify(pu8FileData, u32FileSize, &pu8ImageData, &u32ImageSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ADVCA_SSD_WholeImageVerify, s32Ret);

        COM_CHECK(ADVCA_SSD_DeInit());
        return s32Ret;
    }

    if ((HI_NULL_PTR == pu8ImageData)
        || (0 == u32ImageSize))
    {
        COM_CHECK(ADVCA_SSD_DeInit());
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    s32Ret = HIFILE_ParseWholeFile(pu8ImageData, u32ImageSize, pstDocument);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ParseWholeFile, s32Ret);
        COM_CHECK(ADVCA_SSD_DeInit());
        return s32Ret;
    }

    pstManuInfo = &pstDocument->astManuInfo[0];

    for (i = 0; i < pstManuInfo->u16FlashMapNum; i++)
    {
        pstFlashMap = &pstManuInfo->astFlashMap[i];

        s32Ret = ADVCA_SSD_PartitionImageVerify(HI_NULL_PTR,
                                                pstFlashMap->u64PartitionStartAddr,
                                                pstFlashMap->pu8ImageData,
                                                pstFlashMap->u32ImageLength,
                                                &pstFlashMap->pu8ImageData,
                                                &pstFlashMap->u32ImageLength,
                                                &pstFlashMap->u32PartVersion,
                                                &pstFlashMap->bNeedUpgrade);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ADVCA_SSD_PartitionImageVerify, s32Ret);
            COM_CHECK(ADVCA_SSD_DeInit());
            return s32Ret;
        }
    }

    COM_CHECK(ADVCA_SSD_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
