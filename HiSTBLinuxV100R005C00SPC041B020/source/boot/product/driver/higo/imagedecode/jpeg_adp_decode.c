/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_adp_decode.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/

#include "jpeg_adp_decode.h"
#include "hi_drv_jpeg.h"


/***************************** Macro Definition ********************************/

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/
static HI_DRV_JPEG_INMSG_S  gs_stInMsg;
static HI_DRV_JPEG_OUTMSG_S gs_stOutMsg;

/***************************** API forward declarations ************************/



/***************************** API realization *********************************/


HI_S32 JPEG_ADP_CreateDecode(HI_U32 *pDecHandle,HI_CHAR* pSrcBuf,HI_U32 u32SrcLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 DecHandle = 0;

    memset(&gs_stInMsg, 0,sizeof(HI_DRV_JPEG_INMSG_S));
    memset(&gs_stOutMsg,0,sizeof(HI_DRV_JPEG_OUTMSG_S));
    gs_stInMsg.u32SavePhy    = (HI_U32)pSrcBuf;
    gs_stInMsg.u64SaveVir[0] = (HI_U64)(unsigned long)pSrcBuf;
    gs_stInMsg.u32SaveLen[0] = u32SrcLen;
    gs_stInMsg.bUserPhyMem   = HI_TRUE;

    s32Ret = HI_DRV_JPEG_CreateDec(&DecHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    *pDecHandle   = (unsigned long)DecHandle;

    return HI_SUCCESS;
}



HI_S32 JPEG_ADP_GetImgInfo(HI_U32 DecHandle,HI_HANDLE hSurface)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HI_GO_SURFACE_S *pstSurface = (HI_GO_SURFACE_S*)hSurface;

    if (NULL == pstSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = HI_DRV_JPEG_DecInfo(DecHandle,&gs_stInMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if( (gs_stInMsg.s32InWidth >= CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH * 2) || (gs_stInMsg.s32InHeight >= CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT * 2))
    {
        gs_stOutMsg.s32Scale = 8;
    }
    else if ( (gs_stInMsg.s32InWidth > CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH) || (gs_stInMsg.s32InHeight > CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT))
    {
        gs_stOutMsg.s32Scale = 4;
    }
    else
    {
        gs_stOutMsg.s32Scale = 1;
    }


    s32Ret = HI_DRV_JPEG_DecOutInfo(DecHandle,&gs_stOutMsg);
    if (HI_SUCCESS != s32Ret){
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pstSurface->u32Width[0]  = gs_stOutMsg.u32OutWidth[0];
    pstSurface->u32Height[0] = gs_stOutMsg.u32OutHeight[0];
    pstSurface->u32Stride[0] = gs_stOutMsg.u32OutStride[0];
    pstSurface->u32Size[0]   = gs_stOutMsg.u32OutSize[0];
    pstSurface->u32Width[1]  = gs_stOutMsg.u32OutWidth[1];
    pstSurface->u32Height[1] = gs_stOutMsg.u32OutHeight[1];
    pstSurface->u32Stride[1] = gs_stOutMsg.u32OutStride[1];
    pstSurface->u32Size[1]   = gs_stOutMsg.u32OutSize[1];
    switch (gs_stOutMsg.enOutFmt)
    {
        case JPG_FMT_YUV400:
            pstSurface->enPixelFormat = HIGO_PF_YUV400;
            break;
        case JPG_FMT_YUV420:
            pstSurface->enPixelFormat = HIGO_PF_YUV420;
            break;
        case JPG_FMT_YUV444:
            pstSurface->enPixelFormat = HIGO_PF_YUV444;
            break;
        case JPG_FMT_YUV422_12:
            pstSurface->enPixelFormat = HIGO_PF_YUV422_12;
            break;
        case JPG_FMT_YUV422_21:
            pstSurface->enPixelFormat = HIGO_PF_YUV422_21;
            break;
        default:
            HI_GFX_Log();
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 JPEG_ADP_StartDecode(HI_U32 DecHandle, HI_HANDLE hSurface)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_GO_SURFACE_S *pstSurface = (HI_GO_SURFACE_S*)hSurface;
    HI_JPEG_DECINFO_S stDecInfo;

    if (NULL == pstSurface)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    memset(&stDecInfo,0,sizeof(HI_JPEG_DECINFO_S));

    gs_stOutMsg.u32OutPhy[0]   = (HI_U32)pstSurface->pPhyAddr[0];
    gs_stOutMsg.u32OutPhy[1]   = (HI_U32)pstSurface->pPhyAddr[1];

#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("gs_stOutMsg.pOutPhy[0]    = 0x%x\n",gs_stOutMsg.pOutPhy[0]);
    HI_GFX_Print("gs_stOutMsg.pOutPhy[1]    = 0x%x\n",gs_stOutMsg.pOutPhy[1]);
    HI_GFX_Print("gs_stOutMsg.u32OutWidth[0]     = %d\n",gs_stOutMsg.u32OutWidth[0]);
    HI_GFX_Print("gs_stOutMsg.u32OutHeight[0]    = %d\n",gs_stOutMsg.u32OutHeight[0]);
    HI_GFX_Print("gs_stOutMsg.u32OutStride[0]    = %d\n",gs_stOutMsg.u32OutStride[0]);
    HI_GFX_Print("gs_stOutMsg.u32OutWidth[1]     = %d\n",gs_stOutMsg.u32OutWidth[1]);
    HI_GFX_Print("gs_stOutMsg.u32OutHeight[1]    = %d\n",gs_stOutMsg.u32OutHeight[1]);
    HI_GFX_Print("gs_stOutMsg.u32OutStride[1]    = %d\n",gs_stOutMsg.u32OutStride[1]);
    HI_GFX_Print("gs_stOutMsg.u32OutSize[0]      = %d\n",gs_stOutMsg.u32OutSize[0]);
    HI_GFX_Print("gs_stOutMsg.u32OutSize[1]      = %d\n",gs_stOutMsg.u32OutSize[1]);
    HI_GFX_Print("gs_stOutMsg.enOutFmt           = %d\n",gs_stOutMsg.enOutFmt);
    HI_GFX_Print("=============================================\n");
#endif

    stDecInfo.stInMsg   = gs_stInMsg;
    stDecInfo.stOutMsg  = gs_stOutMsg;
    s32Ret = HI_DRV_JPEG_DecFrame(DecHandle,&stDecInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 JPEG_ADP_DestroyDecode(HI_U32 DecHandle)
{
    HI_DRV_JPEG_DestoryDec(DecHandle);
    return HI_SUCCESS;
}
