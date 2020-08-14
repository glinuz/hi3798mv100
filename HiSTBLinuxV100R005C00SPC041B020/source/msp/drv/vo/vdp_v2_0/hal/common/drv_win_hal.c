
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

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 s_bVideoSurfaceFlag = -1;
static VIDEO_LAYER_FUNCTIONG_S s_stVieoLayerFunc;
static VIDEO_LAYER_S s_stVideoLayer[DEF_VIDEO_LAYER_MAX_NUMBER];
static VIDEO_LAYER_CAPABILITY_S s_stVideoLayerCap[DEF_VIDEO_LAYER_MAX_NUMBER];
#if defined (CHIP_HIFONEB02)
static VIDEO_LAYER_STATUS_S   s_stVideoLayerStatus[DEF_VIDEO_LAYER_MAX_NUMBER];
#endif
static MMZ_BUFFER_S g_stBlackMMZ_HAL;
static HI_DRV_VIDEO_FRAME_S g_stBlackFrame_HAL;
#define DEF_BLACK_H 4
#define DEF_BLACK_W 4096

static HI_S32 HAL_SetDispMode(HI_U32 u32id, HI_DRV_DISP_STEREO_MODE_E eMode)
{
    return Chip_Specific_SetDispMode(u32id, eMode);
}

HI_S32 GetCapability(HI_U32 eLayer, VIDEO_LAYER_CAPABILITY_S *pstSurf)
{
    if ( (eLayer < DEF_VIDEO_LAYER_MAX_NUMBER) && pstSurf)
    {
        *pstSurf = s_stVideoLayerCap[eLayer];
		
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}
#if defined (CHIP_HIFONEB02)
HI_S32 GetLayerAttr(HI_U32 eLayer, VIDEO_LAYER_STATUS_S *pstSurf)
{
    if ( (eLayer < DEF_VIDEO_LAYER_MAX_NUMBER) && pstSurf)
    {
        *pstSurf = s_stVideoLayerStatus[eLayer];
		
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}
#endif

/*to be deleted, seems useless.*/
HI_S32 AcquireLayerByDisplay(HI_DRV_DISPLAY_E eDisp, HI_U32 *peLayer)
{
    HI_U32 eId;

    if (HI_DRV_DISPLAY_1 == eDisp)
    {
        for(eId = VDP_RM_LAYER_VID0; eId < VDP_RM_LAYER_VID2; eId++)
        {
            if( s_stVideoLayerCap[eId].bSupport && !s_stVideoLayer[eId].bWorking)
            {
                *peLayer = eId;
                s_stVideoLayer[eId].bWorking = HI_TRUE;
                return HI_SUCCESS;
            }
        }

        /*in multi-area mode, the third and following window can be attached to V1.
         * but it should confirm to the rules of V1. if not satisfied,
         * error  should be return back, just only in another func,such as enable.
         */
        if (eId == VDP_RM_LAYER_VID2)
            *peLayer = VDP_RM_LAYER_VID1;
    }

    if (HI_DRV_DISPLAY_0 == eDisp)
    {
        for(eId = VDP_RM_LAYER_VID3; eId <=  VDP_RM_LAYER_VID4; eId++)
        {
            if( s_stVideoLayerCap[eId].bSupport && !s_stVideoLayer[eId].bWorking)
            {
                *peLayer = eId;
                s_stVideoLayer[eId].bWorking = HI_TRUE;
                return HI_SUCCESS;
            }
        }

        /* the same as display1*/
        if (eId >  VDP_RM_LAYER_VID4)
            *peLayer = VDP_RM_LAYER_VID4;
    }

    return HI_SUCCESS;
}


HI_S32 ReleaseLayer(HI_U32 eLayer)
{
    if (eLayer  >= VDP_LAYER_VID_BUTT)
    {
        return HI_FAILURE;
    }

    if (!s_stVideoLayerCap[eLayer].bSupport)
    {
        return HI_FAILURE;
    }

    s_stVideoLayer[eLayer].bWorking = HI_FALSE;
    s_stVideoLayer[eLayer].bInitial=  HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 SetEnable(HI_U32 eLayer, HI_U32 u32RegionNum, HI_BOOL bEnable)
{
    VDP_VID_SetRegionEnable(eLayer, u32RegionNum, bEnable);
    return HI_SUCCESS;
}

HI_S32 VP0ParaUpd(HI_U32 eLayer)
{
    if ( (eLayer == VDP_RM_LAYER_VID0) || (eLayer == VDP_RM_LAYER_VID1) )
    {

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)
        VDP_VP0_SetParaUpd(VDP_LAYER_VP0,VDP_DISP_COEFMODE_ACM);
        VDP_VP0_SetParaUpd(VDP_LAYER_VP0,VDP_DISP_COEFMODE_ACC);
#endif
        VDP_VP_SetRegUp(VDP_LAYER_VP0);
    }
    return HI_SUCCESS;
}

HI_S32 Update(HI_U32 eLayer)
{
    VDP_VID_SetRegUp(eLayer);
    return HI_SUCCESS;
}


HI_S32 ChckLayerInit(HI_U32 eLayer)
{
    return s_stVideoLayer[eLayer].bInitial;
}
HI_S32 SetDefault(HI_U32 eLayer)
{
    HI_U32 uHid;
    VDP_BKG_S vidBkg;
    VIDEO_LAYER_CAPABILITY_S video_capbility;

    memset((void*)&video_capbility, 0, sizeof(VIDEO_LAYER_CAPABILITY_S));
    memset((void*)&vidBkg, 0, sizeof(VDP_BKG_S));

    uHid = eLayer;
    VDP_VID_SetLayerEnable(uHid, HI_FALSE);
    VDP_VID_SetInDataFmt(uHid, VDP_VID_IFMT_SP_420);

    /*no matter which mode the dhd is in ,progressive or interlace,
     * we should set the updating  period of param coef to field mode, not frame mode.
     * this is the same with mv300, s40/cv200 was not set, so cause two problems
     * DTS2013091206325/DTS2013091209440.
     * added by z56248;
     */
    VDP_SetParaUpMode(uHid, 1);

    (HI_VOID)GetCapability(eLayer, &video_capbility);

    if (video_capbility.bZme)
        VDP_VID_SetReadMode(uHid, VDP_RMODE_PROGRESSIVE, VDP_RMODE_PROGRESSIVE);
    else
        VDP_VID_SetReadMode(uHid, VDP_RMODE_INTERFACE, VDP_RMODE_INTERFACE);

    VDP_VID_SetMuteEnable(uHid, HI_FALSE);
    VDP_VID_SetFlipEnable(uHid, HI_FALSE);
    VDP_VID_SetIfirMode(uHid, VDP_IFIRMODE_6TAPFIR);
    VDP_VID_SetLayerGalpha(uHid, 0xff);

    if (video_capbility.u32BitWidth == 10)
    {
        /*if bitwidth is 10bit. should 2 bit left shift.*/
        vidBkg.u32BkgU = 0x80 << 2;
        vidBkg.u32BkgV = 0x80 << 2;
        vidBkg.u32BkgY = 0x10 << 2;
        vidBkg.u32BkgA = 0xff;
    }
    else if (video_capbility.u32BitWidth == 8)
    {
        vidBkg.u32BkgU = 0x80;
        vidBkg.u32BkgV = 0x80;
        vidBkg.u32BkgY = 0x10;
        vidBkg.u32BkgA = 0xff;
    }

    VDP_VID_SetLayerBkg(uHid, vidBkg);

    /*now v1 and v4 we fixed it to multi-area mode.*/
    if ((eLayer == VDP_RM_LAYER_VID1)
        ||(eLayer == VDP_RM_LAYER_VID4))
    {
        VDP_VID_SetMultiModeEnable(eLayer,1);
    }
    else
    {
        VDP_VID_SetMultiModeEnable(eLayer,0);
    }

    s_stVideoLayer[eLayer].bInitial = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 SetAllLayerDefault(HI_VOID)
{
    SetDefault(VDP_RM_LAYER_VID0);
    SetDefault(VDP_RM_LAYER_VID1);
    //SetDefault(VDP_RM_LAYER_VID2);
    SetDefault(VDP_RM_LAYER_VID3);
    SetDefault(VDP_RM_LAYER_VID4);

    return HI_SUCCESS;
}

HI_S32 WinHalSetColor_MPW(HI_U32 u32LayerId, HI_DRV_DISP_COLOR_SETTING_S *pstColor)
{
    HI_PQ_CSC_MODE_E enCSCMode = HI_PQ_CSC_BUTT;
    HI_PQ_PICTURE_SETTING_S stPictureSetting;
    HI_PQ_VDP_CSC_S stCscMode;

    memset((void*)&stPictureSetting, 0, sizeof(HI_PQ_PICTURE_SETTING_S));
    memset((void*)&stCscMode, 0, sizeof(HI_PQ_VDP_CSC_S));

   switch(pstColor->enInCS)
   {
    case HI_DRV_CS_BT601_YUV_LIMITED:
        switch (pstColor->enOutCS)
        {
            case HI_DRV_CS_BT709_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_YUV2YUV_601_709;
                break;
            case HI_DRV_CS_BT601_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_YUV2YUV;
                break;
            case HI_DRV_CS_BT709_RGB_FULL:
                enCSCMode = HI_PQ_CSC_YUV2RGB_601;
                break;
            default:
                enCSCMode = HI_PQ_CSC_BUTT;
                break;
        }
        break;
    case HI_DRV_CS_BT601_YUV_FULL:
        switch (pstColor->enOutCS)
        {
            case HI_DRV_CS_BT709_RGB_FULL:
                enCSCMode = HI_PQ_CSC_YUV2RGB_601;
                break;
            default:
                enCSCMode = HI_PQ_CSC_BUTT;
                break;
        }
        break;
     case HI_DRV_CS_BT709_YUV_LIMITED:
        switch (pstColor->enOutCS)
        {
            case HI_DRV_CS_BT709_RGB_FULL:
                enCSCMode = HI_PQ_CSC_YUV2RGB_709;
                break;
            case HI_DRV_CS_BT601_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_YUV2YUV_709_601;
                break;
            case HI_DRV_CS_BT709_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_YUV2YUV;
                break;
            default:
                enCSCMode = HI_PQ_CSC_BUTT;
                break;
        }
        break;
    case HI_DRV_CS_BT709_YUV_FULL:
        switch (pstColor->enOutCS)
        {
            case HI_DRV_CS_BT709_RGB_FULL:
                enCSCMode = HI_PQ_CSC_YUV2RGB_709_FULL;
                break;
            default:
                enCSCMode = HI_PQ_CSC_BUTT;
                break;
        }
        break;
    case HI_DRV_CS_BT709_RGB_FULL:
        switch (pstColor->enOutCS)
        {
            case HI_DRV_CS_BT601_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_RGB2YUV_601;
                break;
            case HI_DRV_CS_BT601_YUV_FULL:
                enCSCMode = HI_PQ_CSC_RGB2YUV_601_FULL;
                break;
            case HI_DRV_CS_BT709_YUV_LIMITED:
                enCSCMode = HI_PQ_CSC_RGB2YUV_709;
                break;
            case HI_DRV_CS_BT709_YUV_FULL:
                enCSCMode = HI_PQ_CSC_RGB2YUV_709_FULL;
                break;
            case HI_DRV_CS_BT709_RGB_FULL:
                enCSCMode = HI_PQ_CSC_RGB2RGB;
                break;
            default:
                enCSCMode = HI_PQ_CSC_BUTT;
                break;
        }
        break;
     default:
        break;
    }

    stPictureSetting.u16Brightness = pstColor->u32Bright;
    stPictureSetting.u16Contrast   = pstColor->u32Contrst;
    stPictureSetting.u16Hue        = pstColor->u32Hue;
    stPictureSetting.u16Saturation = pstColor->u32Satur;

    stCscMode.bCSCEn = HI_TRUE;
    stCscMode.enCSC = enCSCMode;

    DRV_PQ_UpdateVdpCSC(u32LayerId , &stCscMode);

    if ((u32LayerId == 0) || (u32LayerId == 1))
    {
        /*this  setting change the 4 params*/
        DRV_PQ_SetHDVideoSetting(&stPictureSetting);
    }
    else if ((u32LayerId == 3) || (u32LayerId == 4))
    {
        /*this  setting change the 4 params*/
        DRV_PQ_SetSDVideoSetting(&stPictureSetting);
    }

    return HI_SUCCESS;
}

HI_S32 WinHalSetAddr_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara, HI_S32 s32exl)
{
    return Chip_Specific_WinHalSetAddr(u32LayerId, pstPara, s32exl);
}

HI_S32 WinHalSetRegionMute(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    return Chip_Specific_WinHalSetRegionMute(u32LayerId, pstPara);
}


HI_S32 WinHalSetPixFmt_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_DRV_PIX_FORMAT_E eFmt = HI_DRV_PIX_BUTT;

    if (!pstPara)
    {
        DISP_FATAL_RETURN();
    }

    /*s40v2 defined null. i don't know is this a better way or not?
     * i think capablity-callback supplied for drv-level,not hal-level,
     * so i give a different implement and same interface instead of  capability,
     * to be reviewed by others.*/

    /*since dcmp treated as pixel fmt, so dcmp set here.
      closed first,because compressed stream and non-compressed will switch sometimes.*/
    VDP_VID_SetDcmpEnable(u32LayerId, 0);
	
#if defined (CHIP_HIFONEB02)
    VDP_VID_SetDataWidth(u32LayerId, pstPara->pstFrame->enBitWidth);
#endif
    eFmt =  pstPara->pstFrame->ePixFormat;
    if (eFmt == HI_DRV_PIX_FMT_NV12)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV21)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV16_2X1)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV61_2X1)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YUYV)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YUYV);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YVYU)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YVYU);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_UYVY)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_UYVY);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV21_CMP)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_420);
#if defined (CHIP_HIFONEB02)
        /*turn on lossless dcmp.*/
        VDP_VID_SetDcmpCtrl(u32LayerId,1,1);
#endif		
        /*turn on dcmp.*/
        VDP_VID_SetDcmpEnable(u32LayerId, 1);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV12_CMP)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt_Define1(u32LayerId, VDP_PROC_FMT_SP_420);
#if defined (CHIP_HIFONEB02)
        /*turn on lossless dcmp.*/
        VDP_VID_SetDcmpCtrl(u32LayerId,1,1);
#endif		
        /*turn on dcmp.*/
        VDP_VID_SetDcmpEnable(u32LayerId, 1);
    }
    else {
        WIN_FATAL(">>>>>>>>>>>>> Error! not support vid format!\n");
    }


    return HI_SUCCESS;
}

HI_S32 TranPixFmtToAlg(HI_DRV_PIX_FORMAT_E enFmt)
{
    switch (enFmt)
    {
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_YVU420:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            return 1;

        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YYUV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
        case HI_DRV_PIX_FMT_VYUY:
        case HI_DRV_PIX_FMT_YUV422P:
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
            return 0;

        case HI_DRV_PIX_FMT_NV24_CMP:
        case HI_DRV_PIX_FMT_NV42_CMP:
        case HI_DRV_PIX_FMT_NV24:
        case HI_DRV_PIX_FMT_NV42:
            return 2;

        default:
            return 1;
    }

}


HI_BOOL GetLayerRevisedPixelFmt(HI_U32 u32LayerId,
										 HI_RECT_S *pstOutRect,
										 HI_DRV_PIX_FORMAT_E *penFmt,
										 HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
	VIDEO_LAYER_CAPABILITY_S stSurfCapbility;

	memset((HI_VOID*)&stSurfCapbility, 0, sizeof(VIDEO_LAYER_CAPABILITY_S));
	GetCapability(u32LayerId, &stSurfCapbility);
	
	return Chip_Specific_WinGetRevisedPixelFmt(stSurfCapbility.bZme, pstOutRect, penFmt,pstDispInfo);
}

HI_S32 Get3DOutRect(HI_DRV_DISP_STEREO_E en3DMode, HI_RECT_S *pstOutRect, HI_RECT_S *pstReviseOutRect)
{
    *pstReviseOutRect = *pstOutRect;
    if(en3DMode >= DISP_STEREO_BUTT)
    {
        WIN_ERROR("3d mode error!\n");
        return HI_FAILURE;
    }

    if (en3DMode == DISP_STEREO_FPK)
    {
      //  *pstReviseOutRect = stOutRect;
    }
    else if (en3DMode == DISP_STEREO_SBS_HALF)
    {
        pstReviseOutRect->s32Width = (pstOutRect->s32Width/2) & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    }
    else if (en3DMode == DISP_STEREO_TAB)
    {
        pstReviseOutRect->s32Height = (pstOutRect->s32Height/2) & HI_WIN_OUT_RECT_HEIGHT_ALIGN;
    }
    else //no 3d mode
    {
        //WIN_ERROR("3d mode no support!\n");
    }

    return HI_SUCCESS;
}

HI_S32 WinHalSetRect_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara, HI_S32 s32exl)
{
    HI_PQ_ZME_PARA_IN_S stZmeI;
    HI_RECT_S stIntmp,stIntmp1, stVideo, stDisp,stOutRect, stVideo_tmp;
    HI_RECT_S   stRect, stRect1;
    HI_U32     bFirlterEnable = HI_FALSE;
    VIDEO_LAYER_CAPABILITY_S stCapability;
    HI_CHIP_TYPE_E   enChipType;
    HI_CHIP_VERSION_E enChipVersion;

    HI_DRV_VIDEO_PRIVATE_S *pFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&pstPara->pstFrame->u32Priv[0];

    memset((void*)&stRect, 0, sizeof(HI_RECT_S));
    memset((void*)&stZmeI, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
    memset((void*)&stCapability, 0, sizeof(VIDEO_LAYER_CAPABILITY_S));

    stRect = pstPara->pstDispInfo->stFmtResolution;

    stIntmp = pstPara->stIn;
    stIntmp.s32Height = stIntmp.s32Height/s32exl;

    /*since cv200 and s40v2 will accept differe width when crop
     *(orginal width should be set in cv200, but cropped width will be set in s40v2.)
     *,so we pass  both stInOrigin and stIn to VDP_VID_SetInReso2, make a choice by hal.
     */
    stIntmp1 = pstPara->stInOrigin;
    stVideo = pstPara->stVideo;
    stDisp = pstPara->stDisp;

    /*when pal and ntsc 1440 width, all the zme width set should be 2 size.*/
    if (pstPara->pstDispInfo->stPixelFmtResolution.s32Width
        == pstPara->pstDispInfo->stFmtResolution.s32Width *2)
    {
        stVideo.s32Width *=2;
        stVideo.s32X *=2;
        stDisp.s32Width  *=2;
        stDisp.s32X  *=2;
    }

    stVideo_tmp = stVideo;

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);
    if (pstPara->bSecondHorZmeEnable && pstPara->bSecondVerZmeEnable)
    {

#if defined (CHIP_HIFONEB02)
		{
            /*In 98m, the output rect is zme's output, if sr enable, so the out rect should be 1/2 of final size.
             * if sr not enabled, should be full size.
             */
            stVideo_tmp.s32Width  /= 2;
            stVideo_tmp.s32Height /= 2;
        }
#else

        if ((HI_CHIP_TYPE_HI3798M == enChipType) || (HI_CHIP_TYPE_HI3796M == enChipType))
        {
            /*In 98m, the output rect is zme's output, if sr enable, so the out rect should be 1/2 of final size.
             * if sr not enabled, should be full size.
             */
            stVideo_tmp.s32Width  /= 2;
            stVideo_tmp.s32Height /= 2;
        }
#endif
    }


    if ((u32LayerId == VDP_LAYER_VID0)
        || (u32LayerId == VDP_LAYER_VID3))
    {
        VDP_VID_SetInReso2(u32LayerId, &stIntmp, &stIntmp1);

        VDP_VID_SetOutReso2(u32LayerId,  &stVideo_tmp);
        VDP_VID_SetVideoPos2(u32LayerId, &stVideo);
        VDP_VID_SetDispPos2(u32LayerId,  &stDisp);
    }
    else
    {
        stRect1 = stDisp;
        VDP_VID_MultiSetRegionReso (u32LayerId, pstPara->u32RegionNum, stRect1);
        /*in cv200, in v1/v4, we must set these value to vp0*/
        VDP_VID_SetInReso2(u32LayerId, &stRect, &stRect);
        VDP_VID_SetOutReso2(u32LayerId,  &stRect);
        VDP_VID_SetVideoPos2(u32LayerId, &stRect);
        VDP_VID_SetDispPos2(u32LayerId,  &stRect);
    }

    stZmeI.u32ZmeFrmWIn = stIntmp.s32Width;
    stZmeI.u32ZmeFrmHIn = stIntmp.s32Height;

    Get3DOutRect(pstPara->en3Dmode,&stVideo, &stOutRect);



    if (pstPara->bSecondHorZmeEnable && pstPara->bSecondVerZmeEnable)
    {
#if defined (CHIP_HIFONEB02)
		{
            /*since sr in v0 in 98m, so the v0's zme output size is 1/2 of v0's output size.
             * because the sr is behind zme.
             */
            stOutRect.s32Width  /= 2;
            stOutRect.s32Height /= 2;
        }
#else
        if ((HI_CHIP_TYPE_HI3798M == enChipType) || (HI_CHIP_TYPE_HI3796M == enChipType))
        {
            /*since sr in v0 in 98m, so the v0's zme output size is 1/2 of v0's output size.
             * because the sr is behind zme.
             */
            stOutRect.s32Width  /= 2;
            stOutRect.s32Height /= 2;
        }
#endif
    }

    stZmeI.u32ZmeFrmWOut = stOutRect.s32Width;
    stZmeI.u32ZmeFrmHOut = stOutRect.s32Height;

    stZmeI.u8ZmeYCFmtIn  = TranPixFmtToAlg(pstPara->pstFrame->ePixFormat);

    stZmeI.u8ZmeYCFmtOut = 0; // X5HD2 MPW FIX '0'(422)
    stZmeI.bZmeFrmFmtIn  = 1;
    stZmeI.bZmeFrmFmtOut = (pstPara->pstDispInfo->bInterlace == HI_TRUE) ? 0 : 1;

    stZmeI.bZmeBFIn  = 0;
    stZmeI.bZmeBFOut = 0;
    stZmeI.u32Fidelity = pstPara->u32Fidelity;
    stZmeI.stOriRect = pstPara->stOriRect;
    stZmeI.u32InRate = pstPara->pstFrame->u32FrameRate;
    stZmeI.u32OutRate = pstPara->pstDispInfo->u32RefreshRate*10;
    stZmeI.bDispProgressive = !pstPara->pstDispInfo->bInterlace;

    #if 0
    HI_PRINT("Ori H %d W %d InRate %d OutRate %d disp %d\n",
            stZmeI.stOriRect.s32Height,
            stZmeI.stOriRect.s32Width,
            stZmeI.u32InRate,
            stZmeI.u32OutRate,
            stZmeI.bDispProgressive);
    #endif

    if ((pstPara->pstDispInfo->stPixelFmtResolution.s32Width  == stVideo.s32Width)
        &&(pstPara->pstDispInfo->stPixelFmtResolution.s32Height == stVideo.s32Height)
        &&(pFrmPriv->u32Fidelity > 0)
       )
    {
        bFirlterEnable = HI_FALSE;
    }
    else
    {
        bFirlterEnable = HI_TRUE;
    }

    #if 0
    HI_PRINT("111:%d, %d, %d, %d,%d!\n",
            stZmeI.u32ZmeFrmWIn,
            stZmeI.u32ZmeFrmHIn,
            stZmeI.u32ZmeFrmWOut,
            stZmeI.u32ZmeFrmHOut,
            stZmeI.u8ZmeYCFmtIn);
    #endif

    if (HI_SUCCESS != Chip_Specific_LayerZmeFunc((HI_PQ_ZME_LAYER_E)u32LayerId, &stZmeI, bFirlterEnable))
        return HI_FAILURE;

    if (HI_SUCCESS != Chip_Specific_WinDoFurtherZmeProcess(u32LayerId,pstPara))
        return HI_FAILURE;

    return HI_SUCCESS;
}

HI_S32 WinHalGetExtrLineParam(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_S32 s32exl = 1;
    HI_S32 s32HeightIn, s32HeightOut;

    s32HeightIn = pstPara->stIn.s32Height;
    s32HeightOut = pstPara->stVideo.s32Height;

    while((s32HeightIn / s32exl)  > (s32HeightOut * VIDEO_ZOOM_IN_VERTICAL_MAX))
    {
        s32exl = s32exl * 2;
    }

    return s32exl;
}

HI_S32 WinHalSetSecure_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
	if (!pstPara)
	{
		DISP_FATAL_RETURN();
	}

	VDP_VID_SetNoSecFlag(u32LayerId,!pstPara->bSecure);

	return HI_SUCCESS;
}
HI_S32 WinHalSetFrame_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_S32 s32exl;

    WinHalSetRegionMute(u32LayerId, pstPara);

    if (pstPara->bRegionMute == HI_TRUE)
    {
        pstPara->pstFrame = &g_stBlackFrame_HAL;
        pstPara->stIn.s32Width = pstPara->stVideo.s32Width;
        pstPara->stIn.s32Height = pstPara->stVideo.s32Height;
        pstPara->stIn.s32X = 0;
        pstPara->stIn.s32Y = 0;
		
		pstPara->stInOrigin.s32Width = pstPara->stVideo.s32Width;
        pstPara->stInOrigin.s32Height = pstPara->stVideo.s32Height;
        pstPara->stInOrigin.s32X = 0;
        pstPara->stInOrigin.s32Y = 0;
		
        pstPara->stOriRect.s32Width = pstPara->stVideo.s32Width;
        pstPara->stOriRect.s32Height = pstPara->stVideo.s32Height;
        pstPara->stOriRect.s32X = 0;
        pstPara->stOriRect.s32Y = 0;
    }

    pstPara->stDisp = pstPara->stVideo;
    if(HAL_SetDispMode(u32LayerId, pstPara->en3Dmode) )
    {
        return HI_FAILURE;
    }

    s32exl = WinHalGetExtrLineParam(u32LayerId, pstPara);
    if (WinHalSetAddr_MPW(u32LayerId, pstPara, s32exl))
    {
        return HI_FAILURE;
    }

	if(WinHalSetSecure_MPW(u32LayerId,pstPara))
	{
        return HI_FAILURE;
	}
	
    if( WinHalSetPixFmt_MPW(u32LayerId, pstPara) )
    {
        return HI_FAILURE;
    }

    if (pstPara->bZmeUpdate)
    {
        if( WinHalSetRect_MPW(u32LayerId, pstPara, s32exl) )
        {
            return HI_FAILURE;
        }
    }
    else
    {
          //WIN_ERROR("no zme\n");
    }

    return HI_SUCCESS;
}

VDP_CBM_MIX_E GetMixerID(VDP_LAYER_VID_E eLayer)
{
    if (eLayer <= VDP_LAYER_VID1)
    {
        return VDP_CBM_MIXV0;
    }
    else if ( (VDP_LAYER_VID3 <= eLayer) && (VDP_LAYER_VID4 >= eLayer))
    {
        return VDP_CBM_MIXV1;
    }
    else
    {
        return VDP_CBM_MIX_BUTT;
    }
}



HI_S32 MovUp(HI_U32 eLayer)
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


//WIN_ERROR("in id=%d, halid=%d\n", eLayer, eLayerHalId);
    // get mixv setting
    nMaxLayer = Chip_Specific_GetMixvMaxNumvber(eMixId);

//WIN_ERROR("eMixId=%d, nMaxLayer=%d\n", eMixId, nMaxLayer);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        Chip_Specific_CBM_GetMixvPrio(eMixId, i, &MixArray[i]);
        //WIN_ERROR("prio=%d, id=%d\n", i, MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        //WIN_ERROR("i=%d, id=%d\n", i, MixArray[i]);
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    //WIN_ERROR("index=%d\n", index);

    // not found or just single layer work
    if (index >= (nMaxLayer-1))
    {
        return HI_SUCCESS;
    }

    // change mixv order
    MixArray[index]= MixArray[index+1];
    MixArray[index+1] = eLayerHalId;

    // set mixv setting
    Chip_Specific_CBM_SetMixvPrio(eMixId, MixArray, nMaxLayer);

    return HI_SUCCESS;
}


HI_S32 MovTop(HI_U32 eLayer)
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
    nMaxLayer = Chip_Specific_GetMixvMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        Chip_Specific_CBM_GetMixvPrio(eMixId, i, &MixArray[i]);
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

    // set mixv setting
    Chip_Specific_CBM_SetMixvPrio(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}


HI_S32 MovDown(HI_U32 eLayer)
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
    nMaxLayer = Chip_Specific_GetMixvMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        Chip_Specific_CBM_GetMixvPrio(eMixId, i, &MixArray[i]);
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
    MixArray[index]= MixArray[index-1];
    MixArray[index - 1] = eLayerHalId;

    // set mixv setting
    Chip_Specific_CBM_SetMixvPrio(eMixId, MixArray, nMaxLayer);
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
    nMaxLayer = Chip_Specific_GetMixvMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        Chip_Specific_CBM_GetMixvPrio(eMixId, i, &MixArray[i]);
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

    // set mixv setting
    Chip_Specific_CBM_SetMixvPrio(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}

HI_S32 GetZorder(HI_U32 eLayer, HI_U32 *pZOrder)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

//WIN_ERROR("in id=%d, halid=%d\n", eLayer, eLayerHalId);

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = Chip_Specific_GetMixvMaxNumvber(eMixId);

//WIN_ERROR("eMixId=%d, nMaxLayer=%d\n", eMixId, nMaxLayer);
    if (nMaxLayer <= 1)
    {
        *pZOrder = 0;
        return HI_SUCCESS;
    }


    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        Chip_Specific_CBM_GetMixvPrio(eMixId, i, &MixArray[i]);
        //WIN_ERROR("prio=%d, id=%d\n", i, MixArray[i]);
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
        *pZOrder = 0xfffffffful;
    }
    else
    {
        *pZOrder = index;
    }

    return HI_SUCCESS;
}

/* to resolve rwzb problem */
HI_S32 SetCSCReg(HI_U32 u32Layer, HI_U32 *pdata)
{
    VDP_VID_SetCscReg(u32Layer, pdata);
    return HI_SUCCESS;
}


HI_S32 GetCSCReg(HI_U32 u32Layer, HI_U32 *pdata)
{
    VDP_VID_GetCscReg(u32Layer, pdata);
    return HI_SUCCESS;
}

HI_S32  SetZMEPhase(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    VDP_VID_SetZmePhaseV_Define1(u32Data, s32PhaseL, s32PhaseC);
    return HI_SUCCESS;
}

HI_S32 VideoLayer_CreateBlackFrame(HI_VOID)
{
    HI_U32 u32Size;
    HI_U8  *pY;
    HI_U8  *pUV;
    HI_S32 s32Ret = HI_SUCCESS;
	
    u32Size = DEF_BLACK_H*DEF_BLACK_W*2;
	
    s32Ret = HI_DRV_MMZ_AllocAndMap("VDP_BlackFrm_HAL", "VO", u32Size, 0, &g_stBlackMMZ_HAL);
    if (s32Ret != HI_SUCCESS)
    {
        WIN_ERROR("Alloc HAL Black Frame Buffer Failed");

        return HI_FAILURE;
    }

    g_stBlackFrame_HAL.ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
    g_stBlackFrame_HAL.u32Height = DEF_BLACK_H;
    g_stBlackFrame_HAL.u32Width = DEF_BLACK_W;
	
    g_stBlackFrame_HAL.stBufAddr[0].u32PhyAddr_Y = g_stBlackMMZ_HAL.u32StartPhyAddr;
    g_stBlackFrame_HAL.stBufAddr[0].u32PhyAddr_C = g_stBlackFrame_HAL.stBufAddr[0].u32PhyAddr_Y
                                                + g_stBlackFrame_HAL.u32Width*g_stBlackFrame_HAL.u32Height;
    g_stBlackFrame_HAL.stBufAddr[0].u32Stride_Y = 0;
    g_stBlackFrame_HAL.stBufAddr[0].u32Stride_C = 0;	
    pY  = (HI_U8 *)(g_stBlackMMZ_HAL.pu8StartVirAddr);
    DISP_MEMSET(pY, 0x10, u32Size/2);
    pUV = (HI_U8 *)(g_stBlackMMZ_HAL.pu8StartVirAddr + DEF_BLACK_H*DEF_BLACK_W);
    DISP_MEMSET(pUV, 0x80, u32Size/2);

    return HI_SUCCESS;
}

HI_S32 VideoLayer_DestoryBlackFrame(HI_VOID)
{
    HI_DRV_MMZ_UnmapAndRelease(&g_stBlackMMZ_HAL);

    return HI_SUCCESS;
}

HI_S32 VideoLayer_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if (s_bVideoSurfaceFlag >= 0)
    {
        return HI_SUCCESS;
    }

    // s1 init videolayer
    DISP_MEMSET(&s_stVieoLayerFunc, 0, sizeof(VIDEO_LAYER_FUNCTIONG_S));
    DISP_MEMSET(&s_stVideoLayer, 0, sizeof(VIDEO_LAYER_S) * DEF_VIDEO_LAYER_MAX_NUMBER);

    // s2.1 set function pointer
    s_stVieoLayerFunc.PF_GetCapability  = GetCapability;
#if defined (CHIP_HIFONEB02)
	 s_stVieoLayerFunc.PF_GetLayerAttr  = GetLayerAttr;
#endif
    s_stVieoLayerFunc.PF_AcquireLayerByDisplay = AcquireLayerByDisplay;
    s_stVieoLayerFunc.PF_ReleaseLayer   = ReleaseLayer;
    s_stVieoLayerFunc.PF_SetEnable      = SetEnable;

    s_stVieoLayerFunc.PF_VP0ParaUpd         = VP0ParaUpd;
    s_stVieoLayerFunc.PF_Update         = Update;
    s_stVieoLayerFunc.PF_SetDefault     = SetDefault;
    s_stVieoLayerFunc.PF_ChckLayerInit   = ChckLayerInit;
    s_stVieoLayerFunc.PF_SetAllLayerDefault = SetAllLayerDefault;
    //s_stVieoLayerFunc.PF_SetFramePara   = SetFramePara;
    s_stVieoLayerFunc.PF_SetDispMode    = HAL_SetDispMode;
    s_stVieoLayerFunc.PF_SetColor  = WinHalSetColor_MPW;
    s_stVieoLayerFunc.PF_MovUp          = MovUp;
    s_stVieoLayerFunc.PF_MovTop         = MovTop;
    s_stVieoLayerFunc.PF_MovDown        = MovDown;
    s_stVieoLayerFunc.PF_MovBottom      = MovBottom;
    s_stVieoLayerFunc.PF_GetZorder      = GetZorder;



    s_stVieoLayerFunc.PF_SetFramePara   = WinHalSetFrame_MPW;
    s_stVieoLayerFunc.PF_Get3DOutRect   = Get3DOutRect;

	s_stVieoLayerFunc.PF_GetLayerRevisedPixelFmt   = GetLayerRevisedPixelFmt;
    s_stVieoLayerFunc.PF_GetCSCReg  =  GetCSCReg;
    s_stVieoLayerFunc.PF_SetCSCReg  =  SetCSCReg;

    /* to resolve pal rwzb */
    s_stVieoLayerFunc.PF_SetZMEPhase  =  SetZMEPhase;
#if defined (CHIP_HIFONEB02)
    (HI_VOID)Chip_Specific_SetLayerCapability(s_stVideoLayerCap);
    (HI_VOID)Chip_Specific_SetLayerAttr(s_stVideoLayerStatus);
#else
    // s2.2 init videolayer capbility
    Chip_Specific_SetLayerCapability(s_stVideoLayerCap);
#endif

    s32Ret = VideoLayer_CreateBlackFrame();
    if (HI_FAILURE == s32Ret)
    {
        WIN_ERROR("Alloc Black Frame Buffer Failed");

        return HI_FAILURE;
    }

    s_bVideoSurfaceFlag++;

    return HI_SUCCESS;
}


HI_S32 VideoLayer_DeInit(HI_VOID)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        return HI_SUCCESS;
    }

    (HI_VOID)VideoLayer_DestoryBlackFrame();

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
