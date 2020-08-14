/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_disp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_mpi_disp.h"

#include "hi_mpi_hdmi.h"

#include "mpi_disp_tran.h"
#include "hi_mpi_pq.h"
#include "hi_mpi_pq.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 HI_UNF_DISP_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_DISP_Init();

#ifndef HI_PQ_V1_0
    s32Ret |= HI_MPI_PQ_Init(HI_NULL);
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_DeInit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_DISP_DeInit();

#ifndef HI_PQ_V1_0
    s32Ret |= HI_MPI_PQ_DeInit();
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_Attach(HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp)
{
    HI_DRV_DISPLAY_E enMaster, enSlave;
    HI_S32 s32Ret;

    Transfer_DispID(&enDstDisp, &enSlave, HI_TRUE);
    Transfer_DispID(&enSrcDisp, &enMaster, HI_TRUE);
    s32Ret = HI_MPI_DISP_Attach(enMaster, enSlave);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_Detach(HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp)
{
    HI_DRV_DISPLAY_E enMaster, enSlave;
    HI_S32 s32Ret;

    Transfer_DispID(&enDstDisp, &enSlave, HI_TRUE);
    Transfer_DispID(&enSrcDisp, &enMaster, HI_TRUE);

    s32Ret = HI_MPI_DISP_Detach(enMaster, enSlave);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_Open(HI_UNF_DISP_E enDisp)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (enDisp >= HI_UNF_DISPLAY2)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    s32Ret = HI_MPI_DISP_Open(enD);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_Close(HI_UNF_DISP_E enDisp)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_Close(enD);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetFormat(HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E enEncodingFormat)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_FMT_E enF = HI_DRV_DISP_FMT_BUTT;
    HI_S32 s32Ret;

    if (  (enEncodingFormat >= HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING)
          && (enEncodingFormat <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
       )
    {
        HI_ERR_DISP("para enEncodingFormat is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    s32Ret = Transfer_EncFmt(&enEncodingFormat, &enF, HI_TRUE);

    if ( HI_SUCCESS != s32Ret)
        return s32Ret;

    s32Ret = HI_MPI_DISP_SetFormat(enD, HI_DRV_DISP_STEREO_NONE, enF);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetFormat(HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E* penEncodingFormat)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_FMT_E enF;
    HI_S32 s32Ret;
    HI_DRV_DISP_STEREO_MODE_E enDrv3D;

    if (!penEncodingFormat)
    {
        HI_ERR_DISP("para penEncodingFormat is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetFormat(enD, &enDrv3D, &enF);

    if (!s32Ret)
    {
        Transfer_EncFmt(penEncodingFormat, &enF, HI_FALSE);
    }

    return s32Ret;
}

#define HI_DISP_ASPECT_RATIO_MAX_WIDTH  0x0FFFFFFFUL
#define HI_DISP_ASPECT_RATIO_MAX_HEIGHT 0x0FFFFFFFUL

HI_S32 HI_UNF_DISP_SetAspectRatio(HI_UNF_DISP_E enDisp, HI_UNF_DISP_ASPECT_RATIO_S* pstDispAspectRatio)
{
    HI_DRV_DISPLAY_E enD;
    HI_U32 h, v;
    HI_S32 s32Ret;

    if (!pstDispAspectRatio)
    {
        HI_ERR_DISP("para aspect ratio is invalid.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (HI_UNF_DISP_ASPECT_RATIO_BUTT <= pstDispAspectRatio->enDispAspectRatio)
    {
        HI_ERR_DISP("para aspect ratio is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

   
    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    Transfer_AspectRatio(pstDispAspectRatio, &h, &v, HI_TRUE);

    s32Ret = HI_MPI_DISP_SetAspectRatio(enD, h, v);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetAspectRatio(HI_UNF_DISP_E enDisp, HI_UNF_DISP_ASPECT_RATIO_S* pstDispAspectRatio)
{
    HI_DRV_DISPLAY_E enD;
    HI_U32 h, v;
    HI_S32 s32Ret;

    if (!pstDispAspectRatio)
    {
        HI_ERR_DISP("para aspect ratio is invalid.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetAspectRatio(enD, &h, &v);
    if (!s32Ret)
    {
        Transfer_AspectRatio(pstDispAspectRatio, &h, &v, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 DISP_CheckIntf(HI_UNF_DISP_INTF_S* pstIntf, HI_U32 u32IntfNum)
{
    HI_U32 u;

    if (!pstIntf)
    {
        HI_ERR_DISP("para pstIntf is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check parameters */
    for (u = 0; u < u32IntfNum; u++)
    {
        if (pstIntf[u].enIntfType >= HI_UNF_DISP_INTF_TYPE_BUTT)
        {
            HI_ERR_DISP("Invalid interface type!\n");
            return HI_ERR_VO_INVALID_PARA;
        }

        if (HI_UNF_DISP_INTF_TYPE_YPBPR == pstIntf[u].enIntfType)
        {
            if (   (pstIntf[u].unIntf.stYPbPr.u8DacY  >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stYPbPr.u8DacPb >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stYPbPr.u8DacPr >= MAX_DAC_NUM)
               )
            {
                HI_ERR_DISP("Invalid YPBPR vdac number!\n");
                return HI_ERR_VO_INVALID_PARA;
            }
        }

        if (HI_UNF_DISP_INTF_TYPE_RGB == pstIntf[u].enIntfType)
        {
            if (   (pstIntf[u].unIntf.stRGB.u8DacR >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stRGB.u8DacG >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stRGB.u8DacB >= MAX_DAC_NUM)
               )
            {
                HI_ERR_DISP("Invalid RGB vdac number!\n");
                return HI_ERR_VO_INVALID_PARA;
            }
        }
        if (HI_UNF_DISP_INTF_TYPE_VGA == pstIntf[u].enIntfType)
        {
            if (   (pstIntf[u].unIntf.stVGA.u8DacR >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stVGA.u8DacG >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stVGA.u8DacB >= MAX_DAC_NUM)
               )
            {
                HI_ERR_DISP("Invalid VGA vdac number!\n");
                return HI_ERR_VO_INVALID_PARA;
            }
        }

        if (HI_UNF_DISP_INTF_TYPE_CVBS == pstIntf[u].enIntfType)
        {
            if (pstIntf[u].unIntf.stCVBS.u8Dac >= MAX_DAC_NUM)
            {
                HI_ERR_DISP("Invalid CVBS vdac number!\n");
                return HI_ERR_VO_INVALID_PARA;
            }
        }

        if (HI_UNF_DISP_INTF_TYPE_SVIDEO == pstIntf[u].enIntfType)
        {
            if (   (pstIntf[u].unIntf.stSVideo.u8DacY >= MAX_DAC_NUM)
                   || (pstIntf[u].unIntf.stSVideo.u8DacC >= MAX_DAC_NUM)
               )
            {
                HI_ERR_DISP("Invalid SVIDEO vdac number!\n");
                return HI_ERR_VO_INVALID_PARA;
            }
        }
    }

    return HI_SUCCESS;
}



HI_S32 HI_UNF_DISP_AttachIntf(HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_S* pstIntf, HI_U32 u32IntfNum)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_INTF_S stDrvIntf;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u;

    if (pstIntf == HI_NULL)
    {
        HI_ERR_DISP("para pstIntf is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (enDisp >= HI_UNF_DISPLAY_BUTT)
    {
        HI_ERR_DISP("Invalid interface parameters!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    /* check parameters */
    s32Ret = DISP_CheckIntf(pstIntf, u32IntfNum);
    if (s32Ret)
    {
        return s32Ret;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    for (u = 0; u < u32IntfNum; u++)
    {
        //stDrvIntf
        s32Ret = Transfer_Intf(&pstIntf[u], &stDrvIntf, HI_TRUE);
        if ( HI_SUCCESS != s32Ret)
        {
            HI_ERR_DISP("Transfer_Intf interface %d failed!\n", (HI_S32)pstIntf[u].enIntfType);
            break;
        }

        //HI_ERR_DISP("unf add intf Y=%d, pb=%d, pr=%d\n",  stDrvIntf.u8VDAC_Y_G, stDrvIntf.u8VDAC_Pb_B, stDrvIntf.u8VDAC_Pr_R);
        s32Ret = HI_MPI_DISP_AddIntf(enD, &stDrvIntf);
        if ( HI_SUCCESS != s32Ret)
        {
            HI_ERR_DISP("Attach interface %d failed!\n", (HI_S32)pstIntf[u].enIntfType);
            break;
        }
    }

    if ( HI_SUCCESS != s32Ret)
    {
        for (; u > 0; u--)
        {
            // delete inft from [u-1]
            s32Ret = Transfer_Intf(&pstIntf[u - 1], &stDrvIntf, HI_TRUE);
            s32Ret |= HI_MPI_DISP_DelIntf(enD, &stDrvIntf);
            if (s32Ret != HI_SUCCESS)
            { continue; }
        }
    }

    return s32Ret;
}

HI_S32 HI_UNF_DISP_DetachIntf(HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_S* pstIntf, HI_U32 u32IntfNum)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_INTF_S stDrvIntf;
    HI_U32 u;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstIntf)
    {
        HI_ERR_DISP("para pstIntf is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    /* check parameters */
    s32Ret = DISP_CheckIntf(pstIntf, u32IntfNum);
    if ( HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    for (u = 0; u < u32IntfNum; u++)
    {
        //stDrvIntf
        s32Ret = Transfer_Intf(&pstIntf[u], &stDrvIntf, HI_TRUE);
        if ( HI_SUCCESS != s32Ret)
        {
            HI_ERR_DISP("Transfer_Intf interface %d failed!\n", (HI_S32)pstIntf[u].enIntfType);
            break;
        }

        //HI_ERR_DISP("unf add intf Y=%d, pb=%d, pr=%d\n",  stDrvIntf.u8VDAC_Y_G, stDrvIntf.u8VDAC_Pb_B, stDrvIntf.u8VDAC_Pr_R);
        s32Ret = HI_MPI_DISP_DelIntf(enD, &stDrvIntf);
        if ( HI_SUCCESS != s32Ret)
        {
            HI_ERR_DISP("Attach interface %d failed!\n", (HI_S32)pstIntf[u].enIntfType);
            break;
        }
    }

    return s32Ret;
}
#if 0
HI_S32 HI_UNF_DISP_GetIntf(HI_UNF_DISP_E enDisp, HI_U32* pu32IntfNum,  HI_UNF_DISP_INTF_S* pstIntf)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_DISP_GetIntf(enDisp, pu32IntfNum,  pstIntf);
    return s32Ret;
}
#endif

HI_S32 HI_UNF_DISP_SetCustomTiming(HI_UNF_DISP_E enDisp,  HI_UNF_DISP_TIMING_S* pstTiming)
{
    HI_DRV_DISP_TIMING_S stTiming;
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (!pstTiming)
    {
        HI_ERR_DISP("para pstTiming is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    memset(&stTiming, 0, sizeof(HI_DRV_DISP_TIMING_S));
    Transfer_Timing(pstTiming, &stTiming, HI_TRUE);

    s32Ret = HI_MPI_DISP_SetTiming(enD, &stTiming);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetCustomTiming(HI_UNF_DISP_E enDisp, HI_UNF_DISP_TIMING_S* pstTiming)
{
    HI_DRV_DISP_TIMING_S stTiming;
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (!pstTiming)
    {
        HI_ERR_DISP("para pstTiming is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetTiming(enD, &stTiming);
    if (HI_SUCCESS == s32Ret)
    {
        Transfer_Timing(pstTiming, &stTiming, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_LAYER_ZORDER_E enZFlag)
{
#if 0
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_LAYER_E enYer = HI_DRV_DISP_LAYER_BUTT;
    HI_DRV_DISP_ZORDER_E enZ;
    HI_S32 s32Ret;


    if ((enLayer >= HI_UNF_DISP_LAYER_BUTT) || enZFlag  >= HI_LAYER_ZORDER_BUTT)
    {
        HI_ERR_DISP("para  is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    Transfer_LayerID(&enLayer, &enYer, HI_TRUE);

    Transfe_ZOrder(&enZFlag, &enZ, HI_TRUE);

    s32Ret = HI_MPI_DISP_SetLayerZorder(enD, enYer, enZ);
    return s32Ret;

#endif

    return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_UNF_DISP_GetLayerZorder(HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_U32* pu32Zorder)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_LAYER_E enYer = HI_DRV_DISP_LAYER_BUTT;
    HI_S32 s32Ret;

    if (!pu32Zorder)
    {
        HI_ERR_DISP("para pu32Zorder is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (enLayer >= HI_UNF_DISP_LAYER_BUTT)
    {
        HI_ERR_DISP("para  is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    Transfer_LayerID(&enLayer, &enYer, HI_TRUE);
    s32Ret = HI_MPI_DISP_GetLayerZorder(enD, enYer, pu32Zorder);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetBgColor(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_BG_COLOR_S* pstBgColor)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_COLOR_S stColor;
    HI_S32 s32Ret;

    if (!pstBgColor)
    {
        HI_ERR_DISP("para pstBgColor is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    Transfer_BGColor((HI_UNF_DISP_BG_COLOR_S*)pstBgColor, &stColor, HI_TRUE);
    s32Ret = HI_MPI_DISP_SetBGColor(enD, &stColor);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetBgColor(HI_UNF_DISP_E enDisp, HI_UNF_DISP_BG_COLOR_S* pstBgColor)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_COLOR_S stColor;
    HI_S32 s32Ret;

    if (!pstBgColor)
    {
        HI_ERR_DISP("para pstBgColor is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetBGColor(enD, &stColor);
    if (!s32Ret)
    {
        Transfer_BGColor(pstBgColor, &stColor, HI_FALSE);
    }

    return s32Ret;
}

#define PQ_PARA_MAX  100

HI_S32 HI_UNF_DISP_SetBrightness(HI_UNF_DISP_E enDisp, HI_U32 u32Brightness)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_SetBrightness(enD, u32Brightness);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    HI_MPI_PQ_SetBrightness(enDisp, u32Brightness);
#else

    if (u32Brightness > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Brightness is %d invalid.\n", u32Brightness);
        return HI_ERR_DISP_INVALID_PARA;
    }
	
	if (HI_UNF_DISPLAY0 == enDisp)
	{
		return HI_SUCCESS;
	}
	
	s32Ret = HI_MPI_PQ_SetBrightness(HI_UNF_DISPLAY0, u32Brightness);
	s32Ret |= HI_MPI_PQ_SetBrightness(HI_UNF_DISPLAY1, u32Brightness);
	
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetBrightness(HI_UNF_DISP_E enDisp, HI_U32* pu32Brightness)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (!pu32Brightness)
    {
        HI_ERR_DISP("para pu32Brightness is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_GetBrightness(enD, pu32Brightness);
#else
    s32Ret = HI_MPI_PQ_GetBrightness(enD, pu32Brightness);
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetContrast(HI_UNF_DISP_E enDisp, HI_U32 u32Contrast)
{
    HI_DRV_DISPLAY_E enD;

    HI_S32 s32Ret;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_SetContrast(enD, u32Contrast);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    HI_MPI_PQ_SetContrast(enDisp, u32Contrast);
#else

    if (u32Contrast > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Contrast is %d invalid.\n", u32Contrast);
        return HI_ERR_DISP_INVALID_PARA;
    }
	if (HI_UNF_DISPLAY0 == enDisp)
	{
		return HI_SUCCESS;
	}
	
	s32Ret = HI_MPI_PQ_SetContrast(HI_UNF_DISPLAY0, u32Contrast);
	s32Ret |= HI_MPI_PQ_SetContrast(HI_UNF_DISPLAY1, u32Contrast);

#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetContrast(HI_UNF_DISP_E enDisp, HI_U32* pu32Contrast)
{
    HI_DRV_DISPLAY_E enD;

    HI_S32 s32Ret;

    if (!pu32Contrast)
    {
        HI_ERR_DISP("para pu32Contrast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_GetContrast(enD, pu32Contrast);
#else
    s32Ret = HI_MPI_PQ_GetContrast(enD, pu32Contrast);
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetSaturation(HI_UNF_DISP_E enDisp, HI_U32 u32Saturation)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_SetSaturation(enD, u32Saturation);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    HI_MPI_PQ_SetSaturation(enDisp, u32Saturation);
#else
    if (u32Saturation > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Saturation is %d invalid.\n", u32Saturation);
        return HI_ERR_DISP_INVALID_PARA;
    }
	if (HI_UNF_DISPLAY0 == enDisp)
	{
		return HI_SUCCESS;
	}
	
	s32Ret = HI_MPI_PQ_SetSaturation(HI_UNF_DISPLAY0, u32Saturation);
	s32Ret |= HI_MPI_PQ_SetSaturation(HI_UNF_DISPLAY1, u32Saturation);

#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetSaturation(HI_UNF_DISP_E enDisp, HI_U32* pu32Saturation)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (!pu32Saturation)
    {
        HI_ERR_DISP("para pu32Saturation is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_GetSaturation(enD, pu32Saturation);
#else
    s32Ret = HI_MPI_PQ_GetSaturation(enD, pu32Saturation);
#endif

    return s32Ret;
}


HI_S32 HI_UNF_DISP_SetHuePlus(HI_UNF_DISP_E enDisp, HI_U32 u32HuePlus)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_SetHuePlus(enD, u32HuePlus);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    HI_MPI_PQ_SetHuePlus(enDisp, u32HuePlus);
#else
    if (u32HuePlus > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32HuePlus is %d invalid.\n", u32HuePlus);
        return HI_ERR_DISP_INVALID_PARA;
    }
	if (HI_UNF_DISPLAY0 == enDisp)
	{
		return HI_SUCCESS;
	}
	
	s32Ret = HI_MPI_PQ_SetHue(HI_UNF_DISPLAY0, u32HuePlus);
	s32Ret |= HI_MPI_PQ_SetHue(HI_UNF_DISPLAY1, u32HuePlus);

#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetHuePlus(HI_UNF_DISP_E enDisp, HI_U32* pu32HuePlus)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret;

    if (!pu32HuePlus)
    {
        HI_ERR_DISP("para pu32HuePlus is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

#ifdef HI_PQ_V1_0
    s32Ret = HI_MPI_DISP_GetHuePlus(enD, pu32HuePlus);
#else
    s32Ret = HI_MPI_PQ_GetHue(enD, pu32HuePlus);
#endif

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetAlgCfg(HI_UNF_DISP_E enDisp, HI_UNF_DISP_ALG_CFG_S* pstAlg)
{
    return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_UNF_DISP_GetAlgCfg(HI_UNF_DISP_E enDisp, HI_UNF_DISP_ALG_CFG_S* pstAlg)
{
    return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_UNF_DISP_CreateVBI(HI_UNF_DISP_E enDisp, HI_UNF_DISP_VBI_CFG_S* pstCfg, HI_HANDLE* phVbi)
{
	HI_DRV_DISPLAY_E enD;
	HI_DRV_DISP_VBI_CFG_S stVbiCfg;

    HI_S32 s32Ret = HI_SUCCESS;
    if (!pstCfg)
    {
        HI_ERR_DISP("para pstCfg is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    if (!phVbi)
    {
        HI_ERR_DISP("para phVbi is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    Transfer_VbiCfg(pstCfg,&stVbiCfg,HI_TRUE);

    s32Ret = HI_MPI_DISP_CreateVBI(enD, &stVbiCfg, phVbi);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_DestroyVBI(HI_HANDLE hVbi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_DISP_DestroyVBI(hVbi);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SendVBIData(HI_HANDLE hVbi, HI_UNF_DISP_VBI_DATA_S *pstVbiData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_DRV_DISP_VBI_DATA_S stVbiData;

    if (!pstVbiData)
    {
        HI_ERR_DISP("para pstVbiData is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_VbiData(pstVbiData, &stVbiData, HI_TRUE);

    s32Ret = HI_MPI_DISP_SendVBIData(hVbi, &stVbiData);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetWss(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_WSS_DATA_S* pstWssData)
{
	return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_UNF_DISP_SetMacrovision(HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E enMode, const HI_VOID* pData)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_SetMacrovision(enD, (HI_DRV_DISP_MACROVISION_E)enMode);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetMacrovision(HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E* penMode, const HI_VOID* pData)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!penMode)
    {
        HI_ERR_DISP("para penMode is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetMacrovision(enDisp, (HI_DRV_DISP_MACROVISION_E *)penMode);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetCgms(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_CGMS_CFG_S* pstCgmsCfg)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;
	HI_DRV_DISP_CGMSA_CFG_S stCgmsCgf;

    if (!pstCgmsCfg)
    {
        HI_ERR_DISP("para pstCgmsCfg is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    
    (HI_VOID)Transfer_DispID(&enDisp, &enD, HI_TRUE);
	(HI_VOID)Transfer_CgmsCfg(pstCgmsCfg, &stCgmsCgf, HI_TRUE);
	
    s32Ret = HI_MPI_DISP_SetCgms(enDisp, (const HI_DRV_DISP_CGMSA_CFG_S *)&stCgmsCgf);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_AcquireSnapshot(HI_UNF_DISP_E enDisp, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_VIDEO_FRAME_S stFrame;
    HI_S32 s32Ret = HI_SUCCESS;

    memset(&stFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    if (!pstFrameInfo)
    {
        HI_ERR_DISP("para pstSnapShotFrame is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    s32Ret =  HI_MPI_DISP_Snapshot_Acquire(enD, &stFrame);
    if (!s32Ret)
    {
        (HI_VOID)Transfer_Frame(pstFrameInfo, &stFrame, HI_FALSE);
        memcpy(&pstFrameInfo->u32Private[0], stFrame.u32Priv, 64 * sizeof(HI_U32));
    }

    return s32Ret;
}

HI_S32 HI_UNF_DISP_ReleaseSnapshot(HI_UNF_DISP_E enDisp, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_DRV_VIDEO_FRAME_S stFrame;

    if (!pstFrameInfo)
    {
        HI_ERR_DISP("para pstSnapShotFrame is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    memset(&stFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    memcpy(&stFrame.u32Priv[0], pstFrameInfo->u32Private, sizeof(pstFrameInfo->u32Private));
    s32Ret =  HI_MPI_DISP_Snapshot_Release(enD, &stFrame);

    return s32Ret;
}

/**Defines the default buffer number.*/
#define HI_DISP_CAST_BUFFER_DEF_NUMBER  ( 5 )

HI_S32 HI_UNF_DISP_GetDefaultCastAttr(HI_UNF_DISP_E enDisp, HI_UNF_DISP_CAST_ATTR_S* pstAttr)
{
    if ( !pstAttr )
    {
        HI_ERR_DISP("para pstAttr is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (enDisp >= HI_UNF_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    memset((void*)pstAttr, 0, sizeof(HI_UNF_DISP_CAST_ATTR_S));
    pstAttr->enFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    pstAttr->u32Width = 1280;
    pstAttr->u32Height = 720;
    pstAttr->u32BufNum = HI_DISP_CAST_BUFFER_DEF_NUMBER;
    pstAttr->bUserAlloc = HI_FALSE;
    pstAttr->bCrop = HI_FALSE;
    pstAttr->bLowDelay = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 HI_UNF_DISP_CreateCast(HI_UNF_DISP_E enDisp, HI_UNF_DISP_CAST_ATTR_S* pstAttr, HI_HANDLE* phCast)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_CAST_CFG_S stCfg;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstAttr)
    {
        HI_ERR_DISP("para pstAttr is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    if (!phCast)
    {
        HI_ERR_DISP("para phCast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    Transfer_CastCfg(pstAttr, &stCfg, HI_TRUE);

    s32Ret = HI_MPI_DISP_CreateCast(enD, &stCfg, phCast);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_DestroyCast(HI_HANDLE hCast)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_DISP_DestroyCast( hCast);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_DISP_SetCastEnable( hCast, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;

    if (!pbEnable)
    {
        HI_ERR_DISP("para pbEnable is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    s32Ret = HI_MPI_DISP_GetCastEnable(hCast, pbEnable);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_AcquireCastFrame( HI_HANDLE hCast, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo, HI_U32 u32TimeoutMs)
{
    HI_DRV_VIDEO_FRAME_S stFrame;
    HI_S32 s32Ret;
    HI_S32 s32TimeRet;
    HI_U32 u32OriTime = 0;
    HI_U32 u32Time = 0;
    HI_U32 u32Delta;

    if (!pstFrameInfo)
    {
        HI_ERR_DISP("para pstCastFrame is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    s32TimeRet = HI_SYS_GetTimeStampMs(&u32OriTime);
    if (s32TimeRet != HI_SUCCESS)
    {
        HI_ERR_DISP("GetTimeStampMs Failed\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    do
    {
        s32Ret = HI_MPI_DISP_AcquireCastFrame( hCast, &stFrame);
        if (!s32Ret)
        {
            Transfer_Frame(pstFrameInfo, &stFrame, HI_FALSE);
        }

        s32TimeRet = HI_SYS_GetTimeStampMs(&u32Time);
        if (s32TimeRet != HI_SUCCESS)
        {
            HI_ERR_DISP("GetTimeStampMs Failed\n");
            if (s32Ret != HI_SUCCESS)
            {
                return HI_ERR_DISP_INVALID_OPT;
            }
            else
            {
                return s32Ret;
            }
        }

        u32Delta = u32Time - u32OriTime;
        (HI_VOID)usleep(1 * 1000);

    }
    while (s32Ret == HI_FAILURE && u32Delta <= u32TimeoutMs);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo)
{
    HI_DRV_VIDEO_FRAME_S stFrame;
    HI_S32 s32Ret;

    if (!pstFrameInfo)
    {
        HI_ERR_DISP("para pstCastFrame is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    memset(&stFrame,0,sizeof(HI_DRV_VIDEO_FRAME_S));

    s32Ret = Transfer_Frame(pstFrameInfo, &stFrame, HI_TRUE);

    if (s32Ret == HI_SUCCESS)
    {
        s32Ret = HI_MPI_DISP_ReleaseCastFrame(hCast, &stFrame);
    }

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetColor(HI_UNF_DISP_E enDisp, HI_UNF_DISP_COLOR_SETTING_S* pstCS)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstCS)
    {
        HI_ERR_DISP("para pstCS is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    //s32Ret = HI_MPI_DISP_SetColor( enDisp, pstCS);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetColor(HI_UNF_DISP_E enDisp, HI_UNF_DISP_COLOR_SETTING_S* pstCS)
{
    HI_DRV_DISPLAY_E enD;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstCS)
    {
        HI_ERR_DISP("para pstCS is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    //s32Ret = HI_MPI_DISP_GetColor( enDisp, pstCS);

    return s32Ret;
}

HI_S32 HI_UNF_DISP_SetDefaultPara( HI_UNF_DISP_E enDisp)
{
    HI_DRV_DISPLAY_E enD;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    HI_ERR_DISP("Set DefaultPara is not support now.\n");

    return HI_ERR_DISP_INVALID_OPT;
}

HI_S32 HI_UNF_DISP_Set3DMode(HI_UNF_DISP_E enDisp, HI_UNF_DISP_3D_E en3D, HI_UNF_ENC_FMT_E enEncFormat)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_FMT_E enF = HI_DRV_DISP_FMT_BUTT;
    HI_DRV_DISP_STEREO_MODE_E enDrv3D;
    HI_S32 s32Ret;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (en3D > HI_UNF_DISP_3D_TOP_AND_BOTTOM)
    {
        HI_ERR_DISP("para en3D is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (en3D == HI_UNF_DISP_3D_NONE)
    {
        if (enEncFormat >= HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING )
        {
            HI_ERR_DISP("para enEncodingFormat is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    if (en3D == HI_UNF_DISP_3D_FRAME_PACKING)
    {
        if (   (enEncFormat < HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING )
               || (enEncFormat > HI_UNF_ENC_FMT_720P_50_FRAME_PACKING )
           )
        {
            HI_ERR_DISP("para enEncodingFormat is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    if (en3D == HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF)
    {
        if (   (enEncFormat != HI_UNF_ENC_FMT_1080i_60 )
               && (enEncFormat != HI_UNF_ENC_FMT_1080i_50 )
           )
        {
            HI_ERR_DISP("para enEncodingFormat is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    if (en3D == HI_UNF_DISP_3D_TOP_AND_BOTTOM)
    {
        if (   (enEncFormat != HI_UNF_ENC_FMT_1080P_24 )
               && (enEncFormat != HI_UNF_ENC_FMT_720P_60)
               && (enEncFormat != HI_UNF_ENC_FMT_720P_50)
           )
        {
            HI_ERR_DISP("para enEncodingFormat is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    Transfer_EncFmt(&enEncFormat, &enF, HI_TRUE);
    Transfer_Disp3DMode(&en3D, &enDrv3D, HI_TRUE);

    if (HI_UNF_DISPLAY1 == enDisp)
    {

        s32Ret = HI_MPI_DISP_SetFormat(enD, enDrv3D, enF);

    }
    else
    {
        s32Ret = HI_MPI_DISP_SetFormat(enD, enDrv3D, enF);
    }

    return s32Ret;
}


HI_S32 HI_UNF_DISP_Get3DMode(HI_UNF_DISP_E enDisp, HI_UNF_DISP_3D_E* pen3D, HI_UNF_ENC_FMT_E* penEncFormat)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_FMT_E enF;
    HI_DRV_DISP_STEREO_MODE_E enDrv3D;
    HI_S32 s32Ret;

    //CHECK_DISP_PTR(pen3D);
    //CHECK_DISP_PTR(penEncFormat);
    if (!pen3D || !penEncFormat)
    {
        HI_ERR_DISP("para is null ptr.\n");
        return HI_ERR_DISP_NULL_PTR;
    }


    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetFormat(enD, &enDrv3D, &enF);

    if (HI_SUCCESS == s32Ret)
    {
        Transfer_EncFmt(penEncFormat, &enF, HI_FALSE);
        Transfer_Disp3DMode(pen3D, &enDrv3D, HI_FALSE);
    }
    return s32Ret;
}


HI_S32 HI_UNF_DISP_SetRightEyeFirst(HI_UNF_DISP_E enDisp, HI_BOOL bEnable)
{
    HI_DRV_DISPLAY_E enD;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    if ((HI_TRUE != bEnable) && (HI_FALSE != bEnable))
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    Transfer_DispID(&enDisp, &enD, HI_TRUE);
    return HI_MPI_DISP_SetRightEyeFirst(enD, bEnable);
}

HI_S32 HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISP_E enDisp, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_DRV_DISPLAY_E enD;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    return  HI_MPI_DISP_SetVirtualScreen(enD, u32Width, u32Height);

}

HI_S32 HI_UNF_DISP_GetVirtualScreen(HI_UNF_DISP_E enDisp, HI_U32* u32Width, HI_U32* u32Height)
{
    HI_DRV_DISPLAY_E enD;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    return  HI_MPI_DISP_GetVirtualScreen(enD, u32Width, u32Height);

}

HI_S32 HI_UNF_DISP_SetScreenOffset(HI_UNF_DISP_E enDisp, HI_UNF_DISP_OFFSET_S* pstOffset)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_OFFSET_S drv_offset;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    if (!pstOffset)
    {
        HI_ERR_DISP("para pstOffset is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    Transfer_DispOffset(pstOffset, &drv_offset, HI_TRUE);

    return HI_MPI_DISP_SetScreenOffset(enD, &drv_offset);
}


HI_S32 HI_UNF_DISP_GetScreenOffset(HI_UNF_DISP_E enDisp, HI_UNF_DISP_OFFSET_S* pstOffset)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_OFFSET_S drv_offset;
    HI_S32 Ret = 0;

    if (enDisp > HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("para enDisp is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstOffset)
    {
        HI_ERR_DISP("para pstOffset is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    Ret = HI_MPI_DISP_GetScreenOffset(enD, &drv_offset);

    if (HI_SUCCESS != Ret)
    { return Ret; }

    Transfer_DispOffset(pstOffset, &drv_offset, HI_FALSE);

    return HI_SUCCESS;
}

#define DISP_CHANNEL_NUM_MAX  2

HI_S32 HI_UNF_DISP_SetIsogenyAttr(const HI_UNF_DISP_ISOGENY_ATTR_S *pstIsogeny, const HI_U32 u32ChannelNum)
{
    HI_DRV_DISP_ISOGENY_ATTR_S stMpiIsogeny[HI_UNF_DISPLAY_BUTT];
    HI_S32 Ret = 0;
	HI_U32 i = 0;

	/*only two channels supported now.*/
    if (u32ChannelNum != DISP_CHANNEL_NUM_MAX)
    {
        HI_ERR_DISP("channel num is invalid or not support now.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstIsogeny)
    {
        HI_ERR_DISP("para pstIsogeny is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
	

	for (i = 0; i < u32ChannelNum; i++)
	{
		if ((pstIsogeny[i].enDisp >= HI_UNF_DISPLAY_BUTT)
			 || (pstIsogeny[i].enFormat >= HI_UNF_ENC_FMT_BUTT)
			 || ( (pstIsogeny[i].enFormat >= HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING)
				   && (pstIsogeny[i].enFormat <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
				 ))
		{
			HI_ERR_DISP("para enDisp is invalid or not support now.\n");
			return HI_ERR_DISP_INVALID_PARA;
		}
		
		Ret = Transfer_DispID((HI_UNF_DISP_E *)&pstIsogeny[i].enDisp, &stMpiIsogeny[i].enDisp, HI_TRUE);
		Ret |= Transfer_EncFmt((HI_UNF_ENC_FMT_E *)&pstIsogeny[i].enFormat, &stMpiIsogeny[i].enFormat, HI_TRUE);
		
		if (HI_SUCCESS != Ret)
		{ 
			HI_ERR_DISP("param disp and fmt trans error.\n");
			return Ret;
		}
	}	


    Ret = HI_MPI_DISP_SetIsogenyAttr(stMpiIsogeny, u32ChannelNum);	
    if (HI_SUCCESS != Ret)
    { 
		return Ret;
	}	

    return HI_SUCCESS;	
}

HI_S32 HI_UNF_DISP_SetHDRType( HI_UNF_DISP_E enDisp, const HI_UNF_DISP_HDR_TYPE_E enHDRType)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_OUT_TYPE_E enMpiDispType;
    HI_UNF_DISP_HDR_TYPE_E enUnfHDRType;
    HI_S32 s32Ret = HI_SUCCESS;

    if(enDisp != HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("ONLY support HI_UNF_DISPLAY1!\n");
        return HI_ERR_DISP_NOT_SUPPORT;
    }
    
    if (enHDRType >= HI_UNF_DISP_HDR_TYPE_BUTT)
    {
        HI_ERR_DISP("para enUnfDispType is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    enUnfHDRType = enHDRType;
    Transfer_DispType(&enUnfHDRType, &enMpiDispType, HI_TRUE);
    
    s32Ret = HI_MPI_DISP_SetDispOutput( enDisp, enMpiDispType);
    return s32Ret;
}

HI_S32 HI_UNF_DISP_GetHDRType(HI_UNF_DISP_E enDisp, HI_UNF_DISP_HDR_TYPE_E *penHDRType)
{
    HI_DRV_DISPLAY_E enD;
    HI_DRV_DISP_OUT_TYPE_E enMpiDispType;
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(enDisp != HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("ONLY support HI_UNF_DISPLAY1!\n");
        return HI_ERR_DISP_NOT_SUPPORT;
    }

    if (!penHDRType)
    {
        HI_ERR_DISP("para penHDRType is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetDispOutput(enD, &enMpiDispType);
    if (HI_SUCCESS != s32Ret)
    { 
        return s32Ret; 
    }

    Transfer_DispType(penHDRType, &enMpiDispType, HI_FALSE);

    return HI_SUCCESS;
    
}

HI_S32 HI_UNF_DISP_SetBT2020Enable(HI_UNF_DISP_E enDisp, HI_BOOL bEnable)
{
    HI_DRV_DISPLAY_E enD;
   
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(enDisp != HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("ONLY support HI_UNF_DISPLAY1!\n");
        return HI_ERR_DISP_NOT_SUPPORT;
    }


    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_SetBT2020Enable(enD, bEnable);
    if (HI_SUCCESS != s32Ret)
    { 
        return s32Ret; 
    }

    return HI_SUCCESS;
    
}
HI_S32 HI_UNF_DISP_GetBT2020Enable(HI_UNF_DISP_E enDisp, HI_BOOL *pbEnable)
{
    HI_DRV_DISPLAY_E enD;

    HI_S32 s32Ret = HI_SUCCESS;
    
    if(enDisp != HI_UNF_DISPLAY1)
    {
        HI_ERR_DISP("ONLY support HI_UNF_DISPLAY1!\n");
        return HI_ERR_DISP_NOT_SUPPORT;
    }

    if (!pbEnable)
    {
        HI_ERR_DISP("para pbEnable is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    
    Transfer_DispID(&enDisp, &enD, HI_TRUE);

    s32Ret = HI_MPI_DISP_GetBT2020Enable(enD, pbEnable);
    if (HI_SUCCESS != s32Ret)
    { 
        return s32Ret; 
    }

    return HI_SUCCESS;
    
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

