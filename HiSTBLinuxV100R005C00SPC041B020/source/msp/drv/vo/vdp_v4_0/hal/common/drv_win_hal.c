/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_hal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software  group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_win_hal.h"
#include "drv_disp_da.h"
#include "drv_win_priv.h"
#include "drv_win_hal_adp.h"
#include "drv_pq_ext.h"
#include "vdp_hal_intf.h"
#include "vdp_software_selfdefine.h"
#include "drv_pq_ext.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#if 0
typedef enum tagVDP_CBM_MIX_E
{
    VDP_CBM_MIXV0 = 0,
    VDP_CBM_MIXV1 = 1,
    VDP_CBM_MIX_BUTT
}VDP_CBM_MIX_E;
#endif

#define VDP_MIXv0_LayerCNT    2
#define VDP_MIXv1_LayerCNT    1

static HI_S32 s_bVideoSurfaceFlag = -1;
static VIDEO_LAYER_FUNCTIONG_S s_stVieoLayerFunc;
static VIDEO_LAYER_S s_stVideoLayer[DEF_VIDEO_LAYER_MAX_NUMBER];

static VDP_LAYER_VID_E s_MIXv0_Zorder[VDP_LAYER_VID_BUTT]=
{
    VDP_LAYER_VID0,
    VDP_LAYER_VID1
};

/*mixv1  and mixg1 be control by CMB*/
static VDP_LAYER_VID_E s_MIXv1_Zorder[VDP_LAYER_VID_BUTT]=
{
    VDP_LAYER_VID3,
};

HI_U32 GetMixvLayerCounts(VDP_CBM_MIX_E eM)
{
    switch(eM)
    {
        case VDP_CBM_MIXV0:
            return VDP_MIXv0_LayerCNT;
        case VDP_CBM_MIXV1:
            return VDP_MIXv1_LayerCNT;
        default:
            return 0;
    }
}

HI_VOID GetMixvPriors(VDP_CBM_MIX_E enMixer, HI_U32 u32prio, HI_U32 *pu32layer_id)
{
    if (enMixer == VDP_CBM_MIXV0)
        *pu32layer_id = s_MIXv0_Zorder[u32prio];
    else
        *pu32layer_id = s_MIXv1_Zorder[u32prio];

    return ;
}

HI_VOID GetSpecialLayerPriorty(HI_U32 u32layer_id,HI_U32 *pu32prio)
{
    HI_U32 i = 0;

    if ((u32layer_id == VDP_LAYER_VID0)
        || (u32layer_id == VDP_LAYER_VID1))
    {
        for (i =0 ; i < VDP_MIXv0_LayerCNT; i++)
        {
            if (s_MIXv0_Zorder[i] == u32layer_id)
            {
                *pu32prio = i;
                break;
            }
        }

        if (i == VDP_MIXv0_LayerCNT)
        {
            WIN_ERROR("Getlayer's priorty failed,layerid:%d!\n", u32layer_id);
            return ;
        }
    }
    else if (u32layer_id == VDP_LAYER_VID3)
    {
        for (i =0 ; i < VDP_MIXv1_LayerCNT; i++)
        {
            if (s_MIXv1_Zorder[i] == u32layer_id)
            {
                *pu32prio = i;
                break;
            }
        }

        if (i == VDP_MIXv1_LayerCNT)
        {
            WIN_ERROR("Getlayer's priorty failed,layerid:%d!\n", u32layer_id);
            return ;
        }
    }

    return;

}


HI_VOID SetMixvPrios(VDP_CBM_MIX_E enMixer,HI_U32 *pMixArray,HI_U32 u32prio)
{
    HI_U32 i = 0;

    if (enMixer == VDP_CBM_MIXV0)
    {
        for (i =0 ; i< VDP_MIXv0_LayerCNT; i++)
        {
            s_MIXv0_Zorder[i] = pMixArray[i];
        }
    }
    else
    {
        for (i =0 ; i< VDP_MIXv0_LayerCNT; i++)
        {
            s_MIXv1_Zorder[i] = pMixArray[i];
        }
    }

    return;
}

static HI_VOID TransvertAddr2CBB(CBB_ADDRESS_S *pstCbbAddr, HI_DRV_VID_FRAME_ADDR_S *pstAddr)
{
    pstCbbAddr->u32PhyAddr_YHead = pstAddr->u32PhyAddr_YHead;
    pstCbbAddr->u32PhyAddr_Y = pstAddr->u32PhyAddr_Y;
    pstCbbAddr->u32Stride_Y = pstAddr->u32Stride_Y;

    pstCbbAddr->u32PhyAddr_CHead = pstAddr->u32PhyAddr_CHead;
    pstCbbAddr->u32PhyAddr_C = pstAddr->u32PhyAddr_C;
    pstCbbAddr->u32Stride_C = pstAddr->u32Stride_C;

    pstCbbAddr->u32PhyAddr_CrHead = pstAddr->u32PhyAddr_CrHead;
    pstCbbAddr->u32PhyAddr_Cr = pstAddr->u32PhyAddr_Cr;
    pstCbbAddr->u32Stride_Cr = pstAddr->u32Stride_Cr;


    pstCbbAddr->u32Head_Stride = pstAddr->u32Head_Stride;
    pstCbbAddr->u32Head_Size = pstAddr->u32Head_Size;

    return;
}

static HI_VOID TransvertCapability(CBB_VIDEO_LAYER_CAPABILITY_S *pstCbbLayer,
                                         VIDEO_LAYER_CAPABILITY_S *pstLayerCap)
{
    pstLayerCap->bZme = pstCbbLayer->bZme;
    pstLayerCap->bZmePrior = pstCbbLayer->bZmePrior;
    pstLayerCap->bDcmp = pstCbbLayer->bDcmp;
    pstLayerCap->bTnrSnrDbDMDrDs = pstCbbLayer->bTnrSnrDbDMDrDs;
    pstLayerCap->u32BitWidth = pstCbbLayer->u32BitWidth;

    memcpy((HI_VOID*)&pstLayerCap->stVideoReadSize,
            (HI_VOID*)&pstCbbLayer->stVideoMaxReadSize,
            sizeof(CBB_RECT_S));

    return;
}


HI_VOID TransvertPixelFmt2CBB(HI_DRV_PIX_FORMAT_E *peInputPixelFmt,CBB_PIX_FORMAT_E  *peOutPixelFmt)
{
    switch(*peInputPixelFmt)
    {
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
            *peOutPixelFmt = CBB_VID_IFMT_SP_420;
            break;
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
            *peOutPixelFmt = CBB_VID_IFMT_SP_422;
            break;
        case HI_DRV_PIX_FMT_YUYV:
            *peOutPixelFmt = CBB_VID_IFMT_PKG_YUYV;
            break;
        case HI_DRV_PIX_FMT_YVYU:
            *peOutPixelFmt = CBB_VID_IFMT_PKG_YVYU;
            break;
        case HI_DRV_PIX_FMT_UYVY:
            *peOutPixelFmt = CBB_VID_IFMT_PKG_UYVY;
            break;
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            *peOutPixelFmt = CBB_VID_IFMT_SP_TILE_64;
            break;
        default:
            WIN_ERROR("Invalid pixel fmt.\n");
            break;
    }

    return ;
}

HI_VOID TransvertCmpUvFmt2CBB(WIN_HAL_PARA_S *pstPara,CBB_LAYER_CFG_INFO_S *pstCbb_Layer)
{
    /*since the cbb  has 3 params to describe:  cmp/uvorder/pixelfmt,  so we should
          *convert the enum value to the struct members.
          */
        if ((pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
            || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1)
            || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP))

        {
            pstCbb_Layer->stFrame.bUvorder  = HI_TRUE;
        }
        else  if ((pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP))
        {
            pstCbb_Layer->stFrame.bUvorder  = HI_FALSE;
        }
        else
        {
            pstCbb_Layer->stFrame.bUvorder  = HI_FALSE;
        }


        /*since the cbb  has 3 params to describe:  cmp/uvorder/pixelfmt,  so we should
          *convert the enum value to the struct members.
          */

        if ((pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV16_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
                || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP))
        {
            /*should open dcmp function in video layer.*/
            pstCbb_Layer->stFrame.bCmpFmt   = HI_TRUE;
            pstCbb_Layer->stFrame.bLostCmp  = HI_FALSE;
        }
        else
        {
            /*not compressed fmt.*/
            pstCbb_Layer->stFrame.bCmpFmt   = HI_FALSE;
            pstCbb_Layer->stFrame.bLostCmp  = HI_FALSE;
        }

}



HI_VOID TransvertSdkDispInfo2Cbb(HI_DISP_DISPLAY_INFO_S *pstSDKDispInfo,
                                     CBB_DISPLAY_INFO_S *pstCBBDispInfo)
{
    pstCBBDispInfo->bInterlace = pstSDKDispInfo->bInterlace;
    pstCBBDispInfo->stFmtResolution.s32X= pstSDKDispInfo->stFmtResolution.s32X;
    pstCBBDispInfo->stFmtResolution.s32Y= pstSDKDispInfo->stFmtResolution.s32Y;
    pstCBBDispInfo->stFmtResolution.s32Width= pstSDKDispInfo->stFmtResolution.s32Width;
    pstCBBDispInfo->stFmtResolution.s32Height= pstSDKDispInfo->stFmtResolution.s32Height;
    pstCBBDispInfo->bInterlace = pstSDKDispInfo->bInterlace;


    switch(pstSDKDispInfo->eDispMode)
    {
        case DISP_STEREO_NONE:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_NONE;
            break;
        case DISP_STEREO_FPK:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_FPK;
            break;
         case DISP_STEREO_SBS_HALF:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_SBS_HALF;
            break;
        case DISP_STEREO_TAB:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_TAB;
            break;
        case DISP_STEREO_FIELD_ALTE:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_FIELD_ALTE;
            break;
        case DISP_STEREO_LINE_ALTE:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_LINE_ALTE;
            break;
        case DISP_STEREO_SBS_FULL:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_SBS_FULL;
            break;
        case DISP_STEREO_L_DEPT:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_L_DEPT;
            break;
        case DISP_STEREO_L_DEPT_G_DEPT:
            pstCBBDispInfo->eDispMode = CBB_DISP_STEREO_L_DEPT_G_DEPT;
            break;
         default:
            WIN_ERROR("Invalid eDispMode type.\n");
            break;
    }

    return;
}



HI_VOID TransvertSdkCsc2Cbb(HI_DRV_COLOR_SPACE_E eColorSpace,
                                     CBB_CSC_MODE_E *penOutColorSpace)
{
    switch(eColorSpace)
    {
        case HI_DRV_CS_BT601_YUV_LIMITED:
            *penOutColorSpace = CBB_CSC_BT601_YUV_LIMITED;
            break;
        case HI_DRV_CS_BT601_YUV_FULL:
            *penOutColorSpace = CBB_CSC_BT601_YUV_FULL;
            break;
         case HI_DRV_CS_BT709_YUV_LIMITED:

            *penOutColorSpace = CBB_CSC_BT709_YUV_LIMITED;
            break;
        case HI_DRV_CS_BT709_YUV_FULL:

            *penOutColorSpace = CBB_CSC_BT709_YUV_FULL;
            break;
        case HI_DRV_CS_BT709_RGB_FULL:
            *penOutColorSpace = CBB_CSC_BT709_RGB_FULL;
            break;
        case HI_DRV_CS_BT2020_YUV_LIMITED:
            *penOutColorSpace = CBB_CSC_BT2020_YUV_LIMITED;
            break;
        case HI_DRV_CS_BT2020_YUV_FULL:
            *penOutColorSpace = CBB_CSC_BT2020_YUV_LIMITED;
            break;
         case HI_DRV_CS_BT2020_RGB_LIMITED:

            *penOutColorSpace = CBB_CSC_BT2020_RGB_FULL;
            break;
        case HI_DRV_CS_BT2020_RGB_FULL:

            *penOutColorSpace = CBB_CSC_BT2020_RGB_FULL;
            break;
         default:
            WIN_ERROR("Invalid color space type--%d.\n",eColorSpace);
            break;
    }

    return;
}

HI_VOID TransvertCbbCsc2Sdk(CBB_CSC_MODE_E eColorSpace,
                                     HI_DRV_COLOR_SPACE_E *penOutColorSpace)
{
    switch(eColorSpace)
    {
        case CBB_CSC_BT601_YUV_LIMITED:

            *penOutColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
            break;
        case CBB_CSC_BT601_YUV_FULL:
            *penOutColorSpace = HI_DRV_CS_BT601_YUV_FULL;
            break;
         case CBB_CSC_BT709_YUV_LIMITED:
            *penOutColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
            break;
        case CBB_CSC_BT709_YUV_FULL:
            *penOutColorSpace = HI_DRV_CS_BT709_YUV_FULL;
            break;
        case CBB_CSC_BT709_RGB_FULL:
            *penOutColorSpace = HI_DRV_CS_BT709_RGB_FULL;
            break;

        case CBB_CSC_BT2020_YUV_LIMITED:
            *penOutColorSpace = HI_DRV_CS_BT2020_YUV_LIMITED;
            break;

         case CBB_CSC_BT2020_RGB_FULL:
            *penOutColorSpace = HI_DRV_CS_BT2020_RGB_FULL;
            break;

         default:
            WIN_ERROR("Invalid color space type.\n");
            break;
    }

    return;
}

/*transvert  sdk's bitwidth definition to cbb's*/
HI_VOID TransvertSdkBitWidth2Cbb(WIN_HAL_PARA_S *pstPara,CBB_LAYER_CFG_INFO_S *pstCbb_Layer)
{
    switch(pstPara->pstFrame->enBitWidth)
    {
        case HI_DRV_PIXEL_BITWIDTH_8BIT:
            pstCbb_Layer->stFrame.enSrcBitWidth = CBB_PIXEL_BITWIDTH_8BIT;
            break;
        case HI_DRV_PIXEL_BITWIDTH_10BIT:
            pstCbb_Layer->stFrame.enSrcBitWidth = CBB_PIXEL_BITWIDTH_10BIT;
            break;
        case HI_DRV_PIXEL_BITWIDTH_10BIT_CTS:
            pstCbb_Layer->stFrame.enSrcBitWidth = CBB_PIXEL_BITWIDTH_10BIT_CTS;
            break;
        case HI_DRV_PIXEL_BITWIDTH_12BIT:
            pstCbb_Layer->stFrame.enSrcBitWidth = CBB_PIXEL_BITWIDTH_12BIT;
            break;
        default:
            WIN_ERROR("Invalid bit width.\n");
            break;
    }

    return;
}



HI_BOOL  CheckFrameIs3D( HI_DRV_FRAME_TYPE_E  eFrmType)
{
    if (( HI_DRV_FT_SBS == eFrmType )
        || ( HI_DRV_FT_TAB == eFrmType )
        || ( HI_DRV_FT_FPK == eFrmType )
        || ( HI_DRV_FT_TILE == eFrmType ))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_VOID TransvertSdkFrameGeneralInfor2Cbb(WIN_HAL_PARA_S *pstPara,CBB_LAYER_CFG_INFO_S *pstCbb_Layer)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

#if 0
    if ((pstPara->pstFrame->bProgressive == HI_FALSE)
        && (pstPara->pstFrame->enFieldMode >= HI_DRV_FIELD_ALL))
    {
        WIN_ERROR("Frame interleave and field mode does not match.\n");
    }
#endif

    pstCbb_Layer->stFrame.bFrameInterlace = pstPara->pstFrame->bProgressive?HI_FALSE:HI_TRUE;

    switch (pstPara->pstFrame->enFieldMode)
    {
        case  HI_DRV_FIELD_TOP:
            pstCbb_Layer->stFrame.eFieldMode = CBB_FIELD_TOP;
            break;
        case  HI_DRV_FIELD_BOTTOM:
            pstCbb_Layer->stFrame.eFieldMode = CBB_FIELD_BOTTOM;
            break;
        case  HI_DRV_FIELD_ALL:
            pstCbb_Layer->stFrame.eFieldMode = CBB_FIELD_ALL;
            break;
        case  HI_DRV_FIELD_BUTT:
        default:
            pstCbb_Layer->stFrame.eFieldMode = CBB_FIELD_BUTT;
            break;
    }

    if ( HI_TRUE == CheckFrameIs3D(pstPara->pstFrame->eFrmType))
    {
        if ( pstPara->bRightEyeFirst )
        {
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress[0],  &pstPara->pstFrame->stBufAddr[1]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress_lb[0], &pstPara->pstFrame->stBufAddr_LB[1]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress[1],  &pstPara->pstFrame->stBufAddr[0]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress_lb[1], &pstPara->pstFrame->stBufAddr_LB[0]);
        }
        else
        {
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress[0],  &pstPara->pstFrame->stBufAddr[0]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress_lb[0], &pstPara->pstFrame->stBufAddr_LB[0]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress[1],  &pstPara->pstFrame->stBufAddr[1]);
            TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress_lb[1], &pstPara->pstFrame->stBufAddr_LB[1]);
        }
    }
    else
    {
        TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress[0],  &pstPara->pstFrame->stBufAddr[0]);
        TransvertAddr2CBB(&pstCbb_Layer->stFrame.stAddress_lb[0], &pstPara->pstFrame->stBufAddr_LB[0]);
    }



    pstCbb_Layer->stFrame.bSecure = pstPara->pstFrame->bSecure;

    #ifdef  CFG_VDP_MMU_SUPPORT
    pstCbb_Layer->stFrame.bSmmu   = HI_TRUE;
    #else
    pstCbb_Layer->stFrame.bSmmu   = HI_FALSE;
    #endif

    /*set the frame size.*/
    pstCbb_Layer->stFrame.stSrcRect.s32X      = 0;
    pstCbb_Layer->stFrame.stSrcRect.s32Y      = 0;
    pstCbb_Layer->stFrame.stSrcRect.s32Width  = pstPara->pstFrame->u32Width;
    pstCbb_Layer->stFrame.stSrcRect.s32Height = pstPara->pstFrame->u32Height;

    switch(pstPara->pstFrame->enBitWidth)
    {
        case HI_DRV_PIXEL_BITWIDTH_8BIT:
            pstCbb_Layer->stAlgWbcInfor.u32BitDepth = 8;
            break;
        case HI_DRV_PIXEL_BITWIDTH_10BIT:
            pstCbb_Layer->stAlgWbcInfor.u32BitDepth = 10;
            break;
        case HI_DRV_PIXEL_BITWIDTH_12BIT:
            pstCbb_Layer->stAlgWbcInfor.u32BitDepth = 12;
            break;
        case HI_DRV_PIXEL_BITWIDTH_BUTT:
        default:
            break;
    }

    pstCbb_Layer->stAlgWbcInfor.stOriCoordinate  = pstPara->pstFrame->stDispRect;
    pstCbb_Layer->stAlgWbcInfor.stCropCoordinate = pstPara->stIn;

    if (pstPara->bTnrOnVdp)
    {
        pstCbb_Layer->stAlgWbcInfor.enAlgIPSel       = HI_PQ_ALGIP_SEL_VDP;
    }
    else
    {
        pstCbb_Layer->stAlgWbcInfor.enAlgIPSel       = HI_PQ_ALGIP_SEL_VPSS;
    }

    pstCbb_Layer->stAlgWbcInfor.bCropBeforeDB   = HI_TRUE;
    pstCbb_Layer->stAlgWbcInfor.u32SingleModeFlag = 1;
    pstCbb_Layer->stAlgWbcInfor.u32ImageStruct  = 0;

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstPara->pstFrame->u32Priv;

    pstCbb_Layer->stAlgWbcInfor.u32ImageAvgQP = pstPriv->stDNRInfo.u32ImageAvgQP;
    pstCbb_Layer->stAlgWbcInfor.u32VdhInfoAddr = pstPriv->stDNRInfo.u32VdhInfoAddr;
    pstCbb_Layer->stAlgWbcInfor.u32VdhInfoStride = pstPriv->stDNRInfo.u32VdhInfoStride;

    pstCbb_Layer->stAlgWbcInfor.pstVdpWbcRegCpuAddr = pstPara->stLayerAlgMmz.pu8StartVirAddr;
    pstCbb_Layer->stAlgWbcInfor.u32VdpWbcRegPhyAddr = pstPara->stLayerAlgMmz.u32StartPhyAddr;

    pstCbb_Layer->stAlgWbcInfor.bSmmu = pstPara->stLayerAlgMmz.bSmmu;
    pstCbb_Layer->stAlgWbcInfor.bSecure = HI_FALSE;

    pstCbb_Layer->stAlgWbcInfor.enVdecProtocol = HI_PQ_VDEC_PROTOCOL_H264;

    return;
}


HI_S32 TranHalParam2CBB(WIN_HAL_PARA_S *pstPara,
                                 CBB_LAYER_CFG_INFO_S *pstCbb_Layer,
                                 CBB_CFG_EXT_S *pstVDPCfgExtPara)
{
    memset((HI_VOID *)pstCbb_Layer, 0, sizeof(CBB_LAYER_CFG_INFO_S));

    /*pass the layer and region number to cbb.*/
    pstCbb_Layer->u32LayerNO = pstPara->u32LayerNO;
    pstCbb_Layer->u32RegionNO = pstPara->u32RegionNum;

    GetSpecialLayerPriorty(pstPara->u32LayerNO, &pstCbb_Layer->u32LayerZorder);

    if ((pstPara->u32LayerNO == VDP_LAYER_VID1)
        && (pstPara->bDolbyMode != HI_TRUE))
    {
        pstCbb_Layer->bRegionModeEn = HI_TRUE;
    }
    else
    {
        pstCbb_Layer->bRegionModeEn = HI_FALSE;
    }

    /*we only care the  region disable or not.*/
    pstCbb_Layer->bRegionEn = pstPara->bRegionEnable;

    pstCbb_Layer->bMuteEn = pstPara->bRegionMute;
    pstCbb_Layer->bFlipEn = pstPara->bFlipEn;

    /*init the bg color.*/
    pstCbb_Layer->stMuteColor.bYuvColorSpace = HI_TRUE;
    pstCbb_Layer->stMuteColor.u32BitWidth = 8;
    pstCbb_Layer->stMuteColor.u32YorRValue = 0x10;
    pstCbb_Layer->stMuteColor.u32UorGValue = 0x80;
    pstCbb_Layer->stMuteColor.u32VorBValue = 0x80;
    pstCbb_Layer->stLetterBoxColor = pstCbb_Layer->stMuteColor;
    pstCbb_Layer->stVideoBGColor = pstCbb_Layer->stMuteColor;

    memcpy((HI_VOID*)&pstCbb_Layer->stCropRect, (HI_VOID*)&pstPara->stIn, sizeof(HI_RECT_S));
    memcpy((HI_VOID*)&pstCbb_Layer->stVideoRect, (HI_VOID*)&pstPara->stVideo, sizeof(HI_RECT_S));
    memcpy((HI_VOID*)&pstCbb_Layer->stDispRect, (HI_VOID*)&pstPara->stDisp, sizeof(HI_RECT_S));

    memcpy((HI_VOID*)&pstCbb_Layer->stHdcpIntermediateRect, (HI_VOID*)&pstPara->stHdcpIntermediateRect, sizeof(HI_RECT_S));
    pstCbb_Layer->bHdcpDegradePQ = pstPara->bHdcpDegradePQ;
    pstCbb_Layer->enLayerFrostMode = (CBB_FROST_MODE_E)pstPara->enLayerFrostMode;

    if ((!pstPara->pstFrame) || (pstPara->bRegionMute) || (pstPara->bRegionEnable == HI_FALSE))
    {
        pstCbb_Layer->stFrame.bFrameUseFul = HI_FALSE;
        pstCbb_Layer->stFrame.enSrcColorSpace = CBB_CSC_BT709_YUV_LIMITED;
    }
    else
    {
        pstCbb_Layer->stFrame.bFrameUseFul = HI_TRUE;
        #if 0
        TransvertSdkCsc2Cbb(((HI_DRV_VIDEO_PRIVATE_S *)&pstPara->pstFrame->u32Priv[0])->eColorSpace,
                             &pstCbb_Layer->enOutColorSpace);
        TransvertSdkCsc2Cbb(((HI_DRV_VIDEO_PRIVATE_S *)&pstPara->pstFrame->u32Priv[0])->eColorSpace,
                             &pstCbb_Layer->stFrame.enSrcColorSpace);
        #else
        TransvertSdkCsc2Cbb(((HI_DRV_VIDEO_PRIVATE_S *)&pstPara->pstFrame->u32Priv[0])->eColorSpace,&pstCbb_Layer->stFrame.enSrcColorSpace);

        /* for HDR show.if HDR10 or Dolby frame go through normal path,just set inCS and outCS as bt709L. */
        if((HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstPara->pstFrame->enSrcFrameType)
            || (HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL == pstPara->pstFrame->enSrcFrameType))
        {
            pstCbb_Layer->stFrame.enSrcColorSpace = CBB_CSC_BT709_YUV_LIMITED;
            pstCbb_Layer->enOutColorSpace = CBB_CSC_BT709_YUV_LIMITED;
        }

        #endif
        /*transvert the sdk's frame infor to cbb's*/
        pstCbb_Layer->stFrame.en3dSrcFormat  = (CBB_FRAME_TYPE_E)pstPara->pstFrame->eFrmType;
        pstCbb_Layer->stFrame.u32DepthLevel  = 0;
        pstCbb_Layer->stFrame.u32ViewLevel   = 0;

        TransvertPixelFmt2CBB(&pstPara->pstFrame->ePixFormat, &pstCbb_Layer->stFrame.eSrcFmt);
        TransvertCmpUvFmt2CBB(pstPara, pstCbb_Layer);


        TransvertSdkBitWidth2Cbb(pstPara, pstCbb_Layer);
        TransvertSdkFrameGeneralInfor2Cbb(pstPara, pstCbb_Layer);
    }

    TransvertSdkCsc2Cbb(pstPara->eOutPutColorSpace,&pstCbb_Layer->enOutColorSpace);
    TransvertSdkDispInfo2Cbb(pstPara->pstDispInfo,&pstCbb_Layer->stDispInfo);

    return HI_SUCCESS;
}


HI_S32 TransWbcInfor2CBB(WIN_WBC_CONFIG_S *pstPara,
                                 CBB_WBC_CFG_INFO_S *pCbbLayerBasePara,
                                 CBB_CFG_EXT_S *pVDPCfgExtPara)
{
    if (pstPara->bWbcEnable == HI_FALSE)
    {
        pCbbLayerBasePara->enWBC                  = DRV_VDP_WBC_WIN;
        pCbbLayerBasePara->bWbcEnable             = pstPara->bWbcEnable;
    }
    else
    {
        /*when in complex condition,  such as watermark.*/
        pCbbLayerBasePara->bForceWbcPoint2Vp           = pstPara->bComplexMode;
        pCbbLayerBasePara->bForceWbcFieldMode        = pstPara->bForceFieldMode;

        pCbbLayerBasePara->stSdWinOutputSize      = pstPara->stSdWinOutputSize;
        pCbbLayerBasePara->stHdWinOutputSize      = pstPara->stHdWinOutputSize;
        pCbbLayerBasePara->stHdWinInputSize       = pstPara->stHdWinInputSize;

        pCbbLayerBasePara->enWBC                  = DRV_VDP_WBC_WIN;
        pCbbLayerBasePara->bWbcEnable             = pstPara->bWbcEnable;

        TransvertPixelFmt2CBB(&pstPara->eOutPutPixelFmt, &pCbbLayerBasePara->eOutPutPixelFmt);

        pCbbLayerBasePara->eFieldMode             = (CBB_FIELD_MODE_E)pstPara->eFieldMode;

        TransvertSdkCsc2Cbb(pstPara->enHdWinSrcColorSpace, &pCbbLayerBasePara->enHdWinSrcColorSpace);
        TransvertSdkCsc2Cbb(pstPara->enHdWinDestColorSpace, &pCbbLayerBasePara->enHdWinDestColorSpace);
        TransvertAddr2CBB(&pCbbLayerBasePara->stAddr, &pstPara->stAddr);

        pCbbLayerBasePara->bSecure = HI_FALSE;

        #ifdef  CFG_VDP_MMU_SUPPORT
        pCbbLayerBasePara->bSmmu   = HI_TRUE;
        #else
        pCbbLayerBasePara->bSmmu   = HI_FALSE;
        #endif

    }

    return 0;
}


HI_S32 TransCbbWbcPolicy2SDK(WIN_WBC_POLICY_S *pstPolicy,
                                         CBB_WBC_POLICY_S *pCbbPolicy)
{
    pstPolicy->bWbcOutputFieldMode = pCbbPolicy->bWbcOutputFieldMode;

    pstPolicy->stWbcOutputSize.s32X = pCbbPolicy->stSdFinaleDispRect.s32X;
    pstPolicy->stWbcOutputSize.s32Y = pCbbPolicy->stSdFinaleDispRect.s32Y;
    pstPolicy->stWbcOutputSize.s32Width = pCbbPolicy->stSdFinaleDispRect.s32Width;
    pstPolicy->stWbcOutputSize.s32Height = pCbbPolicy->stSdFinaleDispRect.s32Height;

    TransvertCbbCsc2Sdk(pCbbPolicy->enDestColorSpace,&pstPolicy->enDestColorSpace);
    TransvertCbbCsc2Sdk(pCbbPolicy->enSrcColorSpace,&pstPolicy->enSrcColorSpace);
    return 0;
}

HI_S32 PF_AllocateMemForCbb(CBB_REQUIRE_BUF_INFO_S *pstMemInfor)
{
    HI_U8  u8MemName[20];
    HI_S32 Ret = HI_FAILURE;
    DISP_MMZ_BUF_S stMBuf;

    (HI_VOID)snprintf(u8MemName, 20, "VDP_CBB_%s", pstMemInfor->u8BufName);
    Ret = DISP_OS_MMZ_AllocAndMap(u8MemName, HI_NULL, pstMemInfor->u32BufSize, 16, &stMBuf);
    if (Ret)
    {
        WIN_ERROR("Allocate mem for cbb failed:%x.\n", Ret);
        return Ret;
    }

    pstMemInfor->u32BufAddr = stMBuf.u32StartPhyAddr;
    pstMemInfor->pu8BufCpuAddr = stMBuf.pu8StartVirAddr;
    pstMemInfor->u32BufSize = stMBuf.u32Size;
    pstMemInfor->bSmmu      = stMBuf.bSmmu;

    return HI_SUCCESS;
}

HI_S32 PF_FreeMemForCbb(CBB_REQUIRE_BUF_INFO_S *pstMemInfor)
{
    DISP_MMZ_BUF_S stMBuf;

    if ((!pstMemInfor)
        || (!pstMemInfor->u32BufAddr)
        || (!pstMemInfor->pu8BufCpuAddr)
        || (!pstMemInfor->u32BufSize))
    {
        WIN_ERROR("Free mem for cbb failed.\n");
        return HI_FAILURE;
    }

    stMBuf.u32StartPhyAddr = pstMemInfor->u32BufAddr;
    stMBuf.pu8StartVirAddr    = pstMemInfor->pu8BufCpuAddr;
    stMBuf.u32Size         = pstMemInfor->u32BufSize;
    stMBuf.bSmmu           = pstMemInfor->bSmmu;

    (HI_VOID)DISP_OS_MMZ_UnmapAndRelease(&stMBuf);

    return HI_SUCCESS;
}


VDP_CBM_MIX_E GetMixerID(VDP_LAYER_VID_E eLayer)
{
    if (eLayer <= VDP_LAYER_VID1)
    {
        return VDP_CBM_MIXV0;
    }
    else if ( (VDP_LAYER_VID3 == eLayer))
    {
        return VDP_CBM_MIXV1;
    }
    else
    {
        WIN_ERROR("Pass invalid layer !\n");
        return VDP_CBM_BUTT;
    }
}

HI_S32 MovTop(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    /*get eLayer Id and mix id. */
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;
    eMixId = GetMixerID(eLayerHalId);

    /*get the layer counts in the given mixid. */
    nMaxLayer = GetMixvLayerCounts(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    /*traversing  the array, get eLayer prios*/
    for(i=0; i < nMaxLayer; i++)
    {
        /*pay attention:  the index "i" means priorty,  the value MixArray[i] means the layerid. */
        GetMixvPriors(eMixId, i, &MixArray[i]);
    }

    /* get eLayer index, */
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found or just single layer work
    if (index >= (nMaxLayer-1))
    {
        return HI_SUCCESS;
    }

    // change mixv order
    for(i=index; i<(nMaxLayer-1); i++)
    {
        MixArray[i]= MixArray[i+1];
    }

    MixArray[i] = eLayerHalId;
    SetMixvPrios(eMixId, MixArray, nMaxLayer);

    return HI_SUCCESS;
}

HI_S32 MovBottom(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = GetMixvLayerCounts(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        GetMixvPriors(eMixId, i, &MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found
    if (index >= nMaxLayer)
    {
        return HI_SUCCESS;
    }

    // layer at bottom
    if (!index)
    {
        return HI_SUCCESS;
    }

    // change mixv order
    for(i=index; i>0; i--)
    {
        MixArray[i]= MixArray[i-1];
    }
    MixArray[0] = eLayerHalId;
    SetMixvPrios(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}


/*to be deleted, seems useless.*/
HI_S32 HAL_Video_AcquireLayerByDisplay(HI_DRV_DISPLAY_E eDisp, HI_U32 *peLayer)
{
    return HI_SUCCESS;
}

HI_S32 HAL_Video_ReleaseLayer(HI_U32 eLayer)
{
    return HI_SUCCESS;
}

/*set layer default.*/
HI_S32 HAL_Video_InitSingleLayer(HI_U32 eLayer)
{
    CBB_Video_SetLayerInit(eLayer);
    return HI_SUCCESS;
}

/*call cbb's func to init all the video layers and etc.*/
HI_S32 HAL_Video_Init(HI_VOID)
{
    /*when suspend resume. when call this do what?*/
    return HI_SUCCESS;
}

HI_S32 HAL_Video_SetLayerParam(HI_U32 u32LayerId,
                                          WIN_HAL_PARA_S *pstPara)
{
    CBB_LAYER_CFG_INFO_S stLayerCfgBasePara;
    CBB_CFG_EXT_S stVDPCfgExtPara;
    CBB_SYS_INFO_S stSysInfo;

    stSysInfo.u32LayerZorder[0] = s_MIXv0_Zorder[0];
    stSysInfo.u32LayerZorder[1] = s_MIXv0_Zorder[1];

#if 0
    stSysInfo.stDispInfo.bInterlace     = pstPara->pstDispInfo->bInterlace;
    stSysInfo.stDispInfo.bIsBottomField = pstPara->pstDispInfo->bIsBottomField;
    stSysInfo.stDispInfo.u32Vline = pstPara->pstDispInfo->u32Vline;
    stSysInfo.stDispInfo.eDispMode = (CBB_DISP_STEREO_E)pstPara->pstDispInfo->eDispMode;
    stSysInfo.stDispInfo.stFmtResolution = pstPara->pstDispInfo->stFmtResolution;
    stSysInfo.stDispInfo.u32RefreshRate = pstPara->pstDispInfo->u32RefreshRate;
#endif

    stSysInfo.bWaterMarkMode =  HI_FALSE;
    stSysInfo.u32WinNum      =  1;
    stSysInfo.bFrostMode     =  HI_FALSE;


    CBB_Video_Set_SysInfo(&stSysInfo);
    TranHalParam2CBB(pstPara,&stLayerCfgBasePara,&stVDPCfgExtPara);

    return CBB_Video_SetLayerParam(&stLayerCfgBasePara, &stVDPCfgExtPara);
}

HI_S32  HAL_Video_GetLayerCapability(HI_U32 eLayer, VIDEO_LAYER_CAPABILITY_S *pstLayerCap)
{
    CBB_VIDEO_LAYER_CAPABILITY_S stLayerCapability;
    memset(&stLayerCapability,0x0,sizeof(CBB_VIDEO_LAYER_CAPABILITY_S));

    stLayerCapability.eId = eLayer;
    CBB_Video_GetLayersCapability(&stLayerCapability);
    TransvertCapability(&stLayerCapability, pstLayerCap);

    return HI_SUCCESS;
}

/*check whether the layer is opened or not?*/
HI_S32  HAL_Video_GetLayerStatus(HI_U32 eLayer)
{
    return  CBB_Video_GetLayerStatus(eLayer);
}

HI_S32 HAL_Video_MovTop(HI_U32 eLayer)
{
    MovTop(eLayer);
    return HI_SUCCESS;
}

HI_S32 HAL_Video_MovBottom(HI_U32 eLayer)
{
    MovBottom(eLayer);
    return HI_SUCCESS;
}

HI_S32 HAL_Video_GetZorder(HI_U32 eLayer, HI_U32 *pZOrder)
{
    GetSpecialLayerPriorty(eLayer, pZOrder);
    return HI_SUCCESS;
}

HI_S32 HAL_SetEnable(HI_U32 eLayer, HI_U32 u32RegionNum, HI_BOOL bEnable)
{
    VDP_VID_SetRegionEnable_sdk(eLayer, u32RegionNum, bEnable);
    return HI_SUCCESS;
}


/*get the wbc policy to  sdk for v3's config . */
HI_S32 HAL_GetWbcPolicy(WIN_WBC_CONFIG_S *pstPara,
                                WIN_WBC_POLICY_S *pstWbcPolicy)
{
    CBB_WBC_POLICY_S stCbbWbcPolicy;
    CBB_WBC_CFG_INFO_S stWbcCfgInfor;

    TransWbcInfor2CBB(pstPara, &stWbcCfgInfor, HI_NULL);
    (HI_VOID)CBB_Video_GetWbcPolicy(&stWbcCfgInfor, HI_NULL, &stCbbWbcPolicy);
    (HI_VOID)TransCbbWbcPolicy2SDK(pstWbcPolicy, &stCbbWbcPolicy);

    return HI_SUCCESS;
}

HI_S32 HAL_SetWbcParam(WIN_WBC_CONFIG_S *pstPara)
{
    CBB_WBC_CFG_INFO_S stCbbInfor;

    TransWbcInfor2CBB(pstPara, &stCbbInfor, HI_NULL);
    (HI_VOID)CBB_Video_SetWbcParam(&stCbbInfor,
                          HI_NULL);
    return HI_SUCCESS;
}

HI_S32 VideoLayer_Init(HI_VOID)
{
    if (s_bVideoSurfaceFlag >= 0)
    {
        return HI_SUCCESS;
    }

    // s1 init videolayer
    DISP_MEMSET(&s_stVieoLayerFunc, 0, sizeof(VIDEO_LAYER_FUNCTIONG_S));
    DISP_MEMSET(&s_stVideoLayer, 0, sizeof(VIDEO_LAYER_S) * DEF_VIDEO_LAYER_MAX_NUMBER);

    // s2.1 set function pointer
    s_stVieoLayerFunc.PF_GetCapability  = HAL_Video_GetLayerCapability;
    s_stVieoLayerFunc.PF_AcquireLayerByDisplay = HAL_Video_AcquireLayerByDisplay;
    s_stVieoLayerFunc.PF_ReleaseLayer   = HAL_Video_ReleaseLayer;
    s_stVieoLayerFunc.PF_SetEnable   = HAL_SetEnable;

    s_stVieoLayerFunc.PF_InitialLayer     = HAL_Video_InitSingleLayer;
    s_stVieoLayerFunc.PF_CheckLayerInit   = HAL_Video_GetLayerStatus;
    s_stVieoLayerFunc.PF_SetAllLayerDefault = HAL_Video_Init;

#if 1
    //s_stVieoLayerFunc.PF_MovUp          = MovUp;
    s_stVieoLayerFunc.PF_MovTop         = HAL_Video_MovTop;
    //s_stVieoLayerFunc.PF_MovDown        = MovDown;
    s_stVieoLayerFunc.PF_MovBottom      = HAL_Video_MovBottom;
    s_stVieoLayerFunc.PF_GetZorder      = HAL_Video_GetZorder;
#endif
    s_stVieoLayerFunc.PF_SetFramePara   = HAL_Video_SetLayerParam;
    s_stVieoLayerFunc.PF_GetWbcPolicy = HAL_GetWbcPolicy;
    s_stVieoLayerFunc.PF_SetWbcParam = HAL_SetWbcParam;

    CBB_Video_Init(PF_AllocateMemForCbb, PF_FreeMemForCbb);

    s_bVideoSurfaceFlag++;

    return HI_SUCCESS;
}


HI_S32 VideoLayer_DeInit(HI_VOID)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        return HI_SUCCESS;
    }

    /*cbb should release all the resources.*/
    CBB_Video_DeInit();

    s_bVideoSurfaceFlag--;
    return HI_SUCCESS;
}

HI_S32 VideoLayer_GetFunction(VIDEO_LAYER_FUNCTIONG_S *pstFunc)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        WIN_WARN("Video layer NOT INIT\n");
        return HI_FAILURE;
    }

    if (!pstFunc)
    {
        WIN_WARN("NULL Pointer\n");
        return HI_FAILURE;
    }

    *pstFunc = s_stVieoLayerFunc;

    return HI_SUCCESS;
}

VIDEO_LAYER_FUNCTIONG_S *VideoLayer_GetFunctionPtr(HI_VOID)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        WIN_ERROR("Video layer NOT INIT\n");
        return HI_NULL;
    }

    return &s_stVieoLayerFunc;
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
