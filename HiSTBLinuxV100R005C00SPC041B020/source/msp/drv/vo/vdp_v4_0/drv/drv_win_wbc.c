/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name      : drv_win_wbc.c
Version          : Initial Draft
Author          : Hisilicon multimedia software group
Created          : 2015/03/11
Last Modified :  z286221
Description   :
Function List :
History          :
******************************************************************************/

#include "drv_disp_com.h"
#include "drv_win_priv.h"
#include "drv_win_hal.h"
#include "hi_drv_disp.h"
#include "drv_display.h"
#include "drv_disp_bufcore.h"
#include "hi_osal.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"



extern spinlock_t g_threadIsr_Lock;
extern S_VDP_REGS_TYPE* g_pstVdpBaseAddr;
WBC_HD_WIN_INFOR_S stWnHDFrameInfor;
ISOGBNY_CFG_INFOR_S stIsogenyInfo;



HI_VOID ISOGENY_UpDateHDWinFramInfo(WBC_HD_WIN_INFOR_S* pstHDWinInfo, HI_U32 u32VideoLayer)
{
    if ( 0 == u32VideoLayer )
    {
        stWnHDFrameInfor.bV0Mute = pstHDWinInfo->bV0Mute;
        stWnHDFrameInfor.bV0RegionEnable = pstHDWinInfo->bV0RegionEnable;

        stWnHDFrameInfor.stV0InRect = pstHDWinInfo->stV0InRect;
        stWnHDFrameInfor.stV0VideoRect = pstHDWinInfo->stV0VideoRect;
        stWnHDFrameInfor.stV0DispRect = pstHDWinInfo->stV0DispRect;

        stWnHDFrameInfor.enV0WinSrcColorSpace = pstHDWinInfo->enV0WinSrcColorSpace;
        stWnHDFrameInfor.enV0WinDestColorSpace = pstHDWinInfo->enV0WinDestColorSpace;
        stWnHDFrameInfor.bV0Secure = pstHDWinInfo->bV0Secure;
    }
    else
    {
        stWnHDFrameInfor.bV1Mute &= pstHDWinInfo->bV1Mute;
        stWnHDFrameInfor.bV1RegionEnable |= pstHDWinInfo->bV1RegionEnable;
        stWnHDFrameInfor.bV1Secure |= pstHDWinInfo->bV1Secure;

    }

    stWnHDFrameInfor.u32WinNum = pstHDWinInfo->u32WinNum;
}


HI_VOID ISOGENY_GetHDWinFrameInfo(WBC_HD_WIN_INFOR_S* pstHDWinInfo)
{
    *pstHDWinInfo = stWnHDFrameInfor;
}

HI_BOOL  ISOGENY_GetHDWinEnable(HI_VOID)
{
    ISOGENY_GetHDWinFrameInfo(&stWnHDFrameInfor);

    if ( 0 == stWnHDFrameInfor.u32WinNum )
    {
        return HI_FALSE;
    }

    if ((( HI_TRUE == stWnHDFrameInfor.bV0RegionEnable) )
        || (( HI_TRUE == stWnHDFrameInfor.bV1RegionEnable)  ))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_VOID ISOGENY_CleanHDWinFrameInfo(HI_VOID)
{
    memset(&stWnHDFrameInfor, 0x0, sizeof(WBC_HD_WIN_INFOR_S));
}

HI_VOID  ISOGENY_GetIsogenyInfo(ISOGBNY_CFG_INFOR_S* pstIsogenyInfo)
{
    pstIsogenyInfo = &stIsogenyInfo;
}

HI_VOID  ISOGENY_GetWBC0Cfg(ISOGBNY_WBC_CFG_S* pstWB0Cfg)
{
    pstWB0Cfg = &stIsogenyInfo.stWBC0Cfg;
}

HI_S32  ISOGBNY_GetWBCOutSize(HI_RECT_S* pstHDOutPutFrameSize,
                              HI_RECT_S* pstHdFmtRect,
                              HI_RECT_S* pstSdFmtRect,
                              HI_RECT_S* pstSdOutRect)
{
    HI_RECT_S  stSdOutputRectSize;

    /*do correspoding tranlation according the hd and sd fmt.*/
    stSdOutputRectSize.s32X = (pstHDOutPutFrameSize->s32X * pstSdFmtRect->s32Width) / pstHdFmtRect->s32Width;
    stSdOutputRectSize.s32Width = (pstHDOutPutFrameSize->s32Width * pstSdFmtRect->s32Width) / pstHdFmtRect->s32Width;

    stSdOutputRectSize.s32Y = (pstHDOutPutFrameSize->s32Y * pstSdFmtRect->s32Height) / pstHdFmtRect->s32Height;
    stSdOutputRectSize.s32Height = (pstHDOutPutFrameSize->s32Height * pstSdFmtRect->s32Height) / pstHdFmtRect->s32Height;

    if (stSdOutputRectSize.s32Height < 64)
    {
        stSdOutputRectSize.s32Height = 64;
    }

    if (stSdOutputRectSize.s32Width < 64)
    {
        stSdOutputRectSize.s32Width = 64;
    }

    if (stSdOutputRectSize.s32Width > 720)
    {
        WIN_WARN("Sd OutputSize out of range.\n");
        stSdOutputRectSize.s32Width = 720;
    }

    if (stSdOutputRectSize.s32Height > 576)
    {
        WIN_WARN("Sd OutputSize out of range.\n");
        stSdOutputRectSize.s32Height = 576;
    }

    if ((stSdOutputRectSize.s32Width + stSdOutputRectSize.s32X) > 720)
    {
        if (stSdOutputRectSize.s32Width <= 64)
        {
            stSdOutputRectSize.s32X = 720 - stSdOutputRectSize.s32Width ;
        }

    }

    stSdOutputRectSize.s32X &= HI_WIN_IN_RECT_X_ALIGN;
    stSdOutputRectSize.s32Width &= HI_WIN_IN_RECT_X_ALIGN;
    stSdOutputRectSize.s32Y &= HI_WIN_IN_RECT_X_ALIGN;
    stSdOutputRectSize.s32Height &= HI_WIN_IN_RECT_X_ALIGN;

    *pstSdOutRect = stSdOutputRectSize;

    return HI_SUCCESS;
}

HI_BOOL ISOGBNY_CheckFrameAddrValid(HI_U32 u32Yaddr,
                                    HI_U32 u32YStride,
                                    HI_U32 u32Caddr,
                                    HI_U32 u32CStride,
                                    HI_U32 u32Height)
{
    HI_U32 u32StartAddr = 0, u32EndAddr = 0;

    u32StartAddr = stIsogenyInfo.stIsogenyFrameMem.u32StartPhyAddr;
    u32EndAddr = (stIsogenyInfo.stIsogenyFrameMem.u32StartPhyAddr + stIsogenyInfo.stIsogenyFrameMem.u32Size);

    if ((u32Yaddr < u32StartAddr)
        || (u32Yaddr > u32EndAddr)
        || ((u32Yaddr + u32YStride * u32Height) > u32EndAddr)
        || (u32Caddr < u32StartAddr)
        || (u32Caddr > u32EndAddr)
        || ((u32Caddr + u32CStride * u32Height) > u32EndAddr))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}



HI_S32 ISOGBNY_GenarateFrameInfor(HI_DRV_VIDEO_FRAME_S* pstConfigFrame,
                                  HI_RECT_S* pstSdDispRect,
                                  HI_DRV_FIELD_MODE_E eWbcInFieldMode_Out,

                                  HI_BOOL bSecure,
                                  HI_DRV_COLOR_SPACE_E enDestColorSpace)
{
    HI_DRV_VIDEO_PRIVATE_S* pstFramePrivate = HI_NULL;



    pstConfigFrame->bSecure = bSecure;
    pstConfigFrame->bCompressd = HI_FALSE;

    /*must pay attention for debug.*/
    pstConfigFrame->enFieldMode = eWbcInFieldMode_Out;
    pstConfigFrame->enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    pstConfigFrame->stDispRect.s32Width = pstSdDispRect->s32Width;
    pstConfigFrame->stDispRect.s32Height = pstSdDispRect->s32Height;
    pstConfigFrame->stDispRect.s32X = pstSdDispRect->s32X;
    pstConfigFrame->stDispRect.s32Y = pstSdDispRect->s32Y;

    /*set the w/h of frame.*/
    pstConfigFrame->u32Width = pstSdDispRect->s32Width;
    pstConfigFrame->u32Height = pstSdDispRect->s32Height;
    pstConfigFrame->enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;

    pstConfigFrame->stBufAddr[0].u32Stride_Y = 720;
    pstConfigFrame->stBufAddr[0].u32Stride_C = 720;

    pstConfigFrame->stBufAddr[0].u32PhyAddr_C = pstConfigFrame->stBufAddr[0].u32PhyAddr_Y
            + pstConfigFrame->stBufAddr[0].u32Stride_Y * pstConfigFrame->u32Height;

    if (HI_FALSE == ISOGBNY_CheckFrameAddrValid(pstConfigFrame->stBufAddr[0].u32PhyAddr_Y,
            pstConfigFrame->stBufAddr[0].u32Stride_Y,
            pstConfigFrame->stBufAddr[0].u32PhyAddr_C,
            pstConfigFrame->stBufAddr[0].u32Stride_C,
            pstConfigFrame->u32Height))
    {
        WIN_FATAL("Wbc write out of range,W/H:%d,%d.\n", pstConfigFrame->u32Width, pstConfigFrame->u32Height);
        return HI_FAILURE;
    }


    pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)(&pstConfigFrame->u32Priv[0]);


    pstFramePrivate->eColorSpace = enDestColorSpace;
    pstFramePrivate->eOriginField = eWbcInFieldMode_Out;
    pstFramePrivate->stVideoOriginalInfo.u32Width = pstSdDispRect->s32Width;
    pstFramePrivate->stVideoOriginalInfo.u32Height = pstSdDispRect->s32Height;
    pstFramePrivate->stWBC2SDDispRect = *pstSdDispRect;

    pstFramePrivate->bAspectProcessed = HI_TRUE;

    return HI_SUCCESS;
}

HI_VOID ISOGBNY_PqProcess(HI_DRV_VIDEO_FRAME_S* pstFrame,
                          WIN_HAL_PARA_S* pstLayerPara,
                          const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{
    HI_VDP_PQ_INFO_S stTimingInfo;

    /*this is the current value.*/
    stTimingInfo.enLayerId  = VDP_LAYER_VID3;
    stTimingInfo.u32Width = ((HI_DRV_VIDEO_PRIVATE_S*)&pstFrame->u32Priv[0])->stVideoOriginalInfo.u32Width;
    stTimingInfo.u32Height = ((HI_DRV_VIDEO_PRIVATE_S*)&pstFrame->u32Priv[0])->stVideoOriginalInfo.u32Height;

    stTimingInfo.bSRState     = HI_FALSE;
    stTimingInfo.bPartUpdate = HI_TRUE;
    stTimingInfo.bIsogenyMode = DISP_Check_IsogenyMode();

    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)HI_DRV_DISPLAY_0].stFmtRect = pstInfo->stDispInfo.stFmtResolution;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)HI_DRV_DISPLAY_0].u32RefreshRate = pstInfo->stDispInfo.u32RefreshRate;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)HI_DRV_DISPLAY_0].bProgressive = pstInfo->stDispInfo.bInterlace ? HI_FALSE : HI_TRUE;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)HI_DRV_DISPLAY_0].enOutColorSpace = pstInfo->stDispInfo.eColorSpace;


    stTimingInfo.stVideo.s32X      = pstLayerPara->stVideo.s32X ;
    stTimingInfo.stVideo.s32Y      = pstLayerPara->stVideo.s32Y;
    stTimingInfo.stVideo.s32Width  = pstLayerPara->stVideo.s32Width;
    stTimingInfo.stVideo.s32Height = pstLayerPara->stVideo.s32Height;


    if (DISP_STEREO_NONE == pstInfo->stDispInfo.eDispMode)
    {
        stTimingInfo.b3dType = HI_FALSE;
    }
    else
    {
        stTimingInfo.b3dType = HI_TRUE;
    }

    DRV_PQ_UpdateVdpPQ((HI_PQ_DISPLAY_E)HI_DRV_DISPLAY_0, &stTimingInfo, (S_VDP_REGS_TYPE*)g_pstVdpBaseAddr);

}


HI_VOID ISR_SlaveWinConfigFrame( HI_DRV_VIDEO_FRAME_S* pstFrame, const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{

    WIN_HAL_PARA_S stLayerPara;
    VIDEO_LAYER_FUNCTIONG_S* pstFunc = HI_NULL;
    /*here we delete all the rwzb content, just rebuild it  in 98cv200 .*/
    memset(&stLayerPara, 0, sizeof(WIN_HAL_PARA_S));

    pstFunc = VideoLayer_GetFunctionPtr();

    if (HI_NULL == pstFunc)
    {
        WIN_INFO("pstFunc is NULL!\n");
        return ;
    }

    stLayerPara.bDolbyMode = HI_FALSE;
    stLayerPara.eOutPutColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    stLayerPara.u32RegionNum =    0;
    stLayerPara.u32LayerNO =   VDP_LAYER_VID3;
    stLayerPara.bRegionMute  = 0;
    stLayerPara.bRegionEnable = HI_TRUE;

    stLayerPara.en3Dmode = pstInfo->stDispInfo.eDispMode;
    stLayerPara.bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;

    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S*) & (pstInfo->stDispInfo);

    /*stOutRect may equal to stIn in most situation.*/
    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S*) & (pstInfo->stDispInfo);
    stLayerPara.pstFrame = pstFrame;

    if (HI_NULL != pstFrame)
    {
        stLayerPara.stIn   = pstFrame->stDispRect;
        stLayerPara.stIn.s32X = 0;
        stLayerPara.stIn.s32Y = 0;
        stLayerPara.stIn.s32Width = pstFrame->stDispRect.s32Width;
        stLayerPara.stIn.s32Height = pstFrame->stDispRect.s32Height;
        stLayerPara.stVideo      = pstFrame->stDispRect;
        stLayerPara.stDisp      = pstFrame->stDispRect;
        (HI_VOID)ISOGBNY_PqProcess(pstFrame, &stLayerPara, pstInfo);
    }
    else
    {
        stLayerPara.bRegionMute  = HI_TRUE;
        stLayerPara.bRegionEnable = HI_FALSE;
    }


    if (HI_NULL == pstFunc->PF_SetFramePara)
    {
        WIN_INFO("pstFunc->PF_SetWbcParam is NULL!\n");
        return ;
    }

    (HI_VOID)pstFunc->PF_SetFramePara(VDP_LAYER_VID3, &stLayerPara);

    return;
}


HI_DRV_FIELD_MODE_E  ISOGBNY_GetWbcOutputFieldMode(HI_VOID)
{
    DISP_INTF_OPERATION_S stFunction = {0};
    HI_BOOL bBtm = HI_FALSE;
    HI_U32  u32Vcnt = 0;

    (HI_VOID)DISP_HAL_GetOperation(&stFunction);

    DISP_ASSERT(HI_NULL != stFunction.FP_GetChnBottomFlag);

    stFunction.FP_GetChnBottomFlag(HI_DRV_DISPLAY_0, &bBtm, &u32Vcnt);

    return (1 - bBtm);
}

HI_S32 ISOGBNY_ConfigWbc0(ISOGBNY_WBC_CFG_S* pstWBC0Cfg, HI_DRV_VIDEO_FRAME_S* pstCfgFrame)
{
    DISP_INTF_OPERATION_S stFunction = {0};

    VIDEO_LAYER_FUNCTIONG_S* pstFunc = HI_NULL;

    WBC_HD_WIN_INFOR_S stHDWinInfo;
    HI_RECT_S stSdWinOutputSize;
    HI_DISP_DISPLAY_INFO_S*	pstDispHd = HI_NULL, *pstDispSd = HI_NULL;
    WIN_WBC_POLICY_S stWbcPolicy;


    (HI_VOID)DISP_HAL_GetOperation(&stFunction);
    pstFunc = VideoLayer_GetFunctionPtr();

    if (HI_NULL == pstFunc)
    {
        WIN_INFO("pstFunc is NULL!\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (HI_NULL == pstFunc->PF_SetWbcParam)
    {
        WIN_INFO("pstFunc->PF_SetWbcParam is NULL!\n");
        return HI_ERR_VO_NULL_PTR;
    }

    ISOGENY_GetHDWinFrameInfo(&stHDWinInfo);

    if ( HI_FALSE == ISOGENY_GetHDWinEnable())
    {
        pstWBC0Cfg->bWbcEnable = HI_FALSE;
    }

    if ((pstWBC0Cfg->bWbcEnable == HI_FALSE)  || (HI_NULL == pstCfgFrame))
    {
        WIN_WBC_CONFIG_S st_tmpWbcConfig;

        memcpy((HI_VOID*)&st_tmpWbcConfig, pstWBC0Cfg, sizeof(WIN_WBC_CONFIG_S));
        st_tmpWbcConfig.bWbcEnable = HI_FALSE;

        pstFunc->PF_SetWbcParam(&st_tmpWbcConfig);
        return  HI_FAILURE;
    }

    pstDispHd = DISP_GetDispBasicInfor(HI_DRV_DISPLAY_1);
    pstDispSd = DISP_GetDispBasicInfor(HI_DRV_DISPLAY_0);

    if ( WBC_WB_FIELD_FIELD == stIsogenyInfo.enWBForceField )
    {
        pstWBC0Cfg->bForceFieldMode = HI_TRUE;
    }
    else
    {
        pstWBC0Cfg->bForceFieldMode = HI_FALSE;
    }

    if ((WBC_WB_POINT_VP == stIsogenyInfo.enWBCForcePoint)
        || (stHDWinInfo.u32WinNum > 1))
    {
        pstWBC0Cfg->bComplexMode = HI_TRUE;
    }
    else
    {
        pstWBC0Cfg->bComplexMode = HI_FALSE;
    }

    pstWBC0Cfg->stHdWinInputSize = stHDWinInfo.stV0InRect;
    pstWBC0Cfg->stHdWinOutputSize = stHDWinInfo.stV0VideoRect;;

    (HI_VOID)ISOGBNY_GetWBCOutSize(&stHDWinInfo.stV0VideoRect,
                                   &pstDispHd->stFmtResolution,
                                   &pstDispSd->stFmtResolution,
                                   &stSdWinOutputSize);

    pstWBC0Cfg->stSdWinOutputSize = stSdWinOutputSize;
    pstWBC0Cfg->eOutPutPixelFmt = HI_DRV_PIX_FMT_NV61_2X1;

    if (HI_DRV_CS_UNKNOWN == stHDWinInfo.enV0WinSrcColorSpace)
    {
        pstWBC0Cfg->enHdWinSrcColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
    }
    else
    {
        pstWBC0Cfg->enHdWinSrcColorSpace = stHDWinInfo.enV0WinSrcColorSpace;
    }

    if (HI_DRV_CS_UNKNOWN == stHDWinInfo.enV0WinDestColorSpace)
    {
        pstWBC0Cfg->enHdWinDestColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
    }
    else
    {
        pstWBC0Cfg->enHdWinDestColorSpace = stHDWinInfo.enV0WinDestColorSpace;
    }

    pstFunc->PF_GetWbcPolicy((WIN_WBC_CONFIG_S*)pstWBC0Cfg, &stWbcPolicy);

    if (stWbcPolicy.bWbcOutputFieldMode)
    {
        pstWBC0Cfg->eFieldMode = ISOGBNY_GetWbcOutputFieldMode();
    }
    else
    {
        pstWBC0Cfg->eFieldMode = HI_DRV_FIELD_ALL;
    }


#if 0
    printk("s-(%d,%d,%d,%d)", stSdWinOutputSize.s32X, stSdWinOutputSize.s32Y,
           stSdWinOutputSize.s32Width, stSdWinOutputSize.s32Height);

    printk("p-(%d,%d,%d,%d)\n", stWbcPolicy.stWbcOutputSize.s32X, stWbcPolicy.stWbcOutputSize.s32Y,
           stWbcPolicy.stWbcOutputSize.s32Width, stWbcPolicy.stWbcOutputSize.s32Height);
#endif
    ISOGBNY_GenarateFrameInfor(pstCfgFrame,
                               &stWbcPolicy.stWbcOutputSize,
                               pstWBC0Cfg->eFieldMode ,
                               pstCfgFrame->bSecure,
                               stWbcPolicy.enDestColorSpace);



    pstWBC0Cfg->stAddr.u32PhyAddr_Y  =  pstCfgFrame->stBufAddr[0].u32PhyAddr_Y;
    pstWBC0Cfg->stAddr.u32Stride_Y   =  pstCfgFrame->stBufAddr[0].u32Stride_Y;
    pstWBC0Cfg->stAddr.u32PhyAddr_C  =  pstCfgFrame->stBufAddr[0].u32PhyAddr_C;
    pstWBC0Cfg->stAddr.u32Stride_C   =  pstCfgFrame->stBufAddr[0].u32Stride_C;

    pstWBC0Cfg->eDispMode = pstDispHd->eDispMode;

    pstFunc->PF_SetWbcParam((WIN_WBC_CONFIG_S*)pstWBC0Cfg);


    return HI_SUCCESS;
}

HI_U32 ISOGBNY_GetBuffNum(ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode)
{
    HI_U32 u32BuffNum = 0;

    if (( ISOGBNY_BUFFER_MODE_MULTIPLE_SEC == enIsogenyBuffMode)
        || ( ISOGBNY_BUFFER_MODE_MULTIPLE_NONSEC == enIsogenyBuffMode)
       )
    {
        u32BuffNum = ISOGBNY_BUF_MULTIPLE_NUM;
    }
    else
    {
        u32BuffNum = ISOGBNY_BUF_SINGL_NUM;
    }

    return u32BuffNum;
}

HI_VOID ISOGBNY_WBC0StatusReset(ISOGBNY_WBC_CFG_S* pstWBC0Cfg)
{
    pstWBC0Cfg->pstWC0CfgNode = HI_NULL;

    pstWBC0Cfg->u32WBCCfgCount = 0;
    pstWBC0Cfg->u32WBCFinishCount = 0;
    pstWBC0Cfg->u32WBCDisardCount = 0;

}

HI_VOID ISOGBNY_SlaveStatusReset(ISOGBNY_SLAVE_CFG_S* pstSlaveCfg)
{
    pstSlaveCfg->pstNextNode = HI_NULL;
    pstSlaveCfg->pstCurrentNode = HI_NULL;
    pstSlaveCfg->pstDispNode = HI_NULL;

    pstSlaveCfg->u32CfgCount = 0;
    pstSlaveCfg->u32RecCount = 0;

    pstSlaveCfg->u32DisardCount = 0;
    pstSlaveCfg->u32RepeatCount = 0;
    pstSlaveCfg->u32ReleaseCount = 0;

}

HI_S32 ISOGBNY_CreatFrameMem( DISP_MMZ_BUF_S* pstMBuf_WBC, HI_U32 u32AllocateSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_TRUE == pstMBuf_WBC->bSecure)
    {
        s32Ret = DISP_OS_SECSMMU_Alloc("VDP_WBC_V3_SE", u32AllocateSize, VDP_MEM_ALIGN, pstMBuf_WBC);
    }
    else
    {
        s32Ret = DISP_OS_MMZ_Alloc("VDP_WBC_V3", VDP_MEM_TYPE_MMZ, u32AllocateSize, VDP_MEM_ALIGN, pstMBuf_WBC);
    }

    return  s32Ret;
}
HI_VOID ISOGBNY_ReleaseFrameInfo( HI_DRV_VIDEO_FRAME_S* pstBufArray , HI_U32 u32Num)
{

    HI_U32 i;

    for (i = 0; i < u32Num; i++)
    {
        if ( HI_NULL != stIsogenyInfo.stFrameBuffStatus[i].pstFrame)
        {
            memset((HI_VOID*)stIsogenyInfo.stFrameBuffStatus[i].pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
        }

        stIsogenyInfo.stFrameBuffStatus[i].pstFrame = HI_NULL;
        stIsogenyInfo.stFrameBuffStatus[i].enFrameStatus = ISOGBNY_FRAME_STATUS_BUTT;
    }

    if ( HI_NULL != pstBufArray)
    {
        DISP_FREE(pstBufArray);
        pstBufArray = HI_NULL;
    }


}

HI_VOID ISOGBNY_ReleaseFrameMem( DISP_MMZ_BUF_S* pstMBuf_WBC )
{

    if ( HI_TRUE == pstMBuf_WBC->bSecure)
    {
        DISP_OS_SECSMMU_Release(pstMBuf_WBC);
    }
    else
    {
        DISP_OS_MMZ_Release(pstMBuf_WBC);
    }
}
HI_VOID ISOGBNY_ReleaseFrameBuf( DISP_MMZ_BUF_S* pstMBuf_WBC )
{
    ISOGBNY_ReleaseFrameMem(pstMBuf_WBC);
    ISOGBNY_ReleaseFrameInfo(stIsogenyInfo.pstBufArray[0], stIsogenyInfo.u32FrameNum);
}

HI_S32 ISOGBNY_CreatFrameBuf( DISP_MMZ_BUF_S* pstMBuf_WBC, ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32AllocateSize = 0;
    HI_U32 u32YSize = 0;
    HI_U32 u32CSize = 0;
    HI_U32 u32BuffNum = 0;
    HI_U32 i;
    HI_DRV_VIDEO_FRAME_S* pstNewFrame = HI_NULL;

    u32BuffNum = ISOGBNY_GetBuffNum(enIsogenyBuffMode);

    if ((u32BuffNum == 0) || (u32BuffNum > ISOGBNY_BUFF_NUM_MAX))
    {
        WIN_ERROR("Isogeny buffer out of max number!\n");
        return HI_FAILURE;
    }

    u32YSize = ISOGBNY_MAX_STRIDE * ISOGBNY_RECT_HEIGHT ;
    u32CSize = u32YSize;/*422 */
    u32AllocateSize = (u32YSize + u32CSize) * u32BuffNum;

    s32Ret = ISOGBNY_CreatFrameMem(pstMBuf_WBC, u32AllocateSize);
    if (HI_SUCCESS != s32Ret)
    {
        WIN_ERROR("CreatFrameBuf malloc frame info memory err!\n");
        return HI_FAILURE;
    }

    /*frame info init*/
    pstNewFrame = (HI_DRV_VIDEO_FRAME_S*)DISP_MALLOC(sizeof(HI_DRV_VIDEO_FRAME_S) * u32BuffNum);
    if (!pstNewFrame)
    {
        ISOGBNY_ReleaseFrameMem(pstMBuf_WBC);
        WIN_ERROR("CreatFrameBuf malloc frame info memory err!\n");
        return HI_FAILURE;
    }

    stIsogenyInfo.pstBufArray[0] = pstNewFrame;
    stIsogenyInfo.u32FrameNum = u32BuffNum;

    for (i = 0; i < u32BuffNum; i++)
    {
        stIsogenyInfo.pstBufArray[i] = pstNewFrame++;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame = stIsogenyInfo.pstBufArray[i];

        if ( !stIsogenyInfo.stFrameBuffStatus[i].pstFrame )
        {
            ISOGBNY_ReleaseFrameMem(pstMBuf_WBC);
            ISOGBNY_ReleaseFrameInfo( stIsogenyInfo.pstBufArray[0], u32BuffNum);
            return HI_ERR_VO_MALLOC_FAILED;
        }

        memset((HI_VOID*)stIsogenyInfo.stFrameBuffStatus[i].pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->u32FrameIndex =  i;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->bProgressive = HI_TRUE;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->stBufAddr[0].u32PhyAddr_Y =  pstMBuf_WBC->u32StartPhyAddr + i * (u32YSize + u32CSize);
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->stBufAddr[0].u32Stride_Y  =  ISOGBNY_MAX_STRIDE;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->stBufAddr[0].u32PhyAddr_C =  stIsogenyInfo.stFrameBuffStatus[i].pstFrame->stBufAddr[0].u32Stride_Y + u32YSize;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->stBufAddr[0].u32Stride_C =  ISOGBNY_MAX_STRIDE;
        stIsogenyInfo.stFrameBuffStatus[i].pstFrame->bSecure = pstMBuf_WBC->bSecure;
        stIsogenyInfo.stFrameBuffStatus[i].enFrameStatus = ISOGBNY_FRAME_STATUS_FREE;

    }

    return HI_SUCCESS;
}


HI_S32 ISOGBNY_FrameBuf_Init(ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode)
{
    HI_BOOL bSecure = HI_FALSE;
    HI_S32 s32Ret;

    if (ISOGBNY_BUFFER_STATUS_INIT == stIsogenyInfo.enIsogenyBuffStatus)
    {
        return HI_SUCCESS;
    }

    if (( ISOGBNY_BUFFER_MODE_MULTIPLE_SEC == enIsogenyBuffMode)
        || ( ISOGBNY_BUFFER_MODE_SINGL_SEC == enIsogenyBuffMode)
       )
    {
        bSecure = HI_TRUE;
    }

    stIsogenyInfo.stIsogenyFrameMem.bSecure = bSecure;

    s32Ret = ISOGBNY_CreatFrameBuf(&stIsogenyInfo.stIsogenyFrameMem, enIsogenyBuffMode);
    if (s32Ret != HI_SUCCESS)
    {
        WIN_FATAL("isogeny create node failed\n");
        return HI_FAILURE;
    }

    s32Ret = DispBuf_Create(&stIsogenyInfo.stBuffer, ISOGBNY_BUFF_NUM_MAX);
    if (s32Ret != HI_SUCCESS)
    {
        WIN_FATAL("isogeny create node failed\n");
        ISOGBNY_ReleaseFrameBuf(&stIsogenyInfo.stIsogenyFrameMem);
        return HI_FAILURE;
    }

    (HI_VOID)DispBuf_Reset(&stIsogenyInfo.stBuffer);

    /*status init*/
    ISOGBNY_WBC0StatusReset(&stIsogenyInfo.stWBC0Cfg);
    ISOGBNY_SlaveStatusReset(&stIsogenyInfo.stSlaveCfg);

    stIsogenyInfo.enIsogenyBuffMode = enIsogenyBuffMode;
    stIsogenyInfo.enIsogenyBuffStatus = ISOGBNY_BUFFER_STATUS_INIT;

    return s32Ret;
}


HI_VOID ISOGBNY_FrameBuf_DeInit(HI_VOID)
{

    if (ISOGBNY_BUFFER_STATUS_DEINIT == stIsogenyInfo.enIsogenyBuffStatus)
    {
        return ;
    }

    DispBuf_Reset(&stIsogenyInfo.stBuffer);
    DispBuf_Destoy(&stIsogenyInfo.stBuffer);

    ISOGBNY_ReleaseFrameBuf(&stIsogenyInfo.stIsogenyFrameMem);
    stIsogenyInfo.enIsogenyBuffMode = ISOGBNY_BUFFER_MODE_BUTT;
    stIsogenyInfo.enIsogenyBuffStatus = ISOGBNY_BUFFER_STATUS_DEINIT;

}


HI_S32 ISOGBNY_ThreadProcess(HI_VOID* pArg)
{
    HI_S32 s32Ret;

    ISOGBNY_THREAT_PROCESS_S* pstIsogenyThread = (ISOGBNY_THREAT_PROCESS_S*) pArg;

    /*if stop refush release frame buffer*/
    while (!kthread_should_stop() )
    {
        if (HI_TRUE == pstIsogenyThread->bChangeProcess)
        {
            pstIsogenyThread->bChangeProcess = HI_FALSE;

            msleep(50);
            ISOGBNY_FrameBuf_DeInit();
            s32Ret = ISOGBNY_FrameBuf_Init(pstIsogenyThread->enIsogenyBuffMode);

            if (HI_SUCCESS != s32Ret)
            {
                WIN_FATAL("isogeny FrameBuf init failed\n");
                return HI_FAILURE;
            }

            stIsogenyInfo.bChangeStatus = HI_FALSE;
        }

        wait_event_timeout(pstIsogenyThread->stWaitQueHead, (HI_TRUE == pstIsogenyThread->bChangeProcess), HZ);
    }

    return HI_SUCCESS;
}

HI_S32 ISOGBNY_CreatProcessThread(HI_VOID )
{
#ifndef __DISP_PLATFORM_BOOT__
    ISOGBNY_THREAT_PROCESS_S* pstIsogenyThread = &stIsogenyInfo.stIsogenyThread;

    init_waitqueue_head( &(pstIsogenyThread->stWaitQueHead));
    pstIsogenyThread->hThread =
        kthread_create(ISOGBNY_ThreadProcess, (HI_VOID*)(&stIsogenyInfo.stIsogenyThread), "IsogenyProcess");

    if (IS_ERR(pstIsogenyThread->hThread))
    {
        DISP_FATAL("Can not create process thread .\n");
        return HI_ERR_DISP_CREATE_ERR;
    }

    wake_up_process(pstIsogenyThread->hThread);
#endif
    return HI_SUCCESS;
}

HI_S32 ISOGBNY_DestroyProcessThread(HI_VOID )
{
#ifndef __DISP_PLATFORM_BOOT__
    HI_S32 s32Ret;
    ISOGBNY_THREAT_PROCESS_S* pstIsogenyThread  = &stIsogenyInfo.stIsogenyThread;

    s32Ret = kthread_stop(pstIsogenyThread->hThread);

    return s32Ret;
#else
    return HI_SUCCESS;
#endif
}

ISOGBNY_BUFFER_MODE_E ISOGENY_JudgeBufferMode(HI_U32 u32HDDispFreq, HI_U32 u32SDDispFreq, HI_BOOL bSecureV0, HI_BOOL bSecureV1)
{

    if ( ( HI_TRUE == bSecureV0) || ( HI_TRUE == bSecureV1))
    {
        if ( u32HDDispFreq != u32SDDispFreq)
        {
            return ISOGBNY_BUFFER_MODE_MULTIPLE_SEC;
        }
        else
        {
            return ISOGBNY_BUFFER_MODE_SINGL_SEC;
        }
    }
    else
    {
        if ( u32HDDispFreq != u32SDDispFreq)
        {
            return ISOGBNY_BUFFER_MODE_MULTIPLE_NONSEC;
        }
        else
        {
            return ISOGBNY_BUFFER_MODE_SINGL_NONSEC;
        }
    }

}



HI_DRV_VIDEO_FRAME_S* ISOGBNY_GetFreeFrame(ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode)
{
    HI_U32 i;
    ISOGBNY_BUFF_MANNGE_S*  pstFrameBuffStatus = HI_NULL ;

    if (ISOGBNY_BUF_SINGL_NUM == ISOGBNY_GetBuffNum(enIsogenyBuffMode))
    {
        return stIsogenyInfo.stFrameBuffStatus[0].pstFrame;
    }

    for (i = 0; i < ISOGBNY_GetBuffNum(stIsogenyInfo.enIsogenyBuffMode); i++)
    {
        pstFrameBuffStatus = &stIsogenyInfo.stFrameBuffStatus[i];

        if (ISOGBNY_FRAME_STATUS_FREE == pstFrameBuffStatus->enFrameStatus )
        {
            pstFrameBuffStatus->enFrameStatus = ISOGBNY_FRAME_STATUS_USED;

            return pstFrameBuffStatus->pstFrame;
        }
    }

    return HI_NULL;
}

HI_S32 ISOGBNY_PutFrame(HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_U32 i;

    for (i = 0; i < sizeof(stIsogenyInfo.stFrameBuffStatus) / sizeof(ISOGBNY_BUFF_MANNGE_S); i++)
    {
        if (pstFrame->u32FrameIndex == stIsogenyInfo.stFrameBuffStatus[i].pstFrame->u32FrameIndex )
        {
            stIsogenyInfo.stFrameBuffStatus[i].enFrameStatus = ISOGBNY_FRAME_STATUS_FREE;
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}


HI_S32 ISOGBNY_ReleaseNode( DISP_BUF_S* pstBuffer, DISP_BUF_NODE_S* pstNode)
{
    HI_S32 nRet;
    HI_DRV_VIDEO_FRAME_S* pstDispFrame;

    if (HI_NULL == pstNode)
    {
        return HI_FAILURE;
    }

    nRet = DispBuf_AddEmptyNode(pstBuffer, pstNode);

    if (HI_SUCCESS != nRet)
    {
        WIN_ERROR("DispBuf_AddEmptyNode err(0x%x).\n", pstNode->u32ID);
        return HI_FAILURE;
    }

    pstDispFrame = (HI_DRV_VIDEO_FRAME_S*)&pstNode->u32Data;

    nRet = ISOGBNY_PutFrame(pstDispFrame);

    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("ISOGBNY_PutFrame err(%d).\n", nRet);
    }

    stIsogenyInfo.stSlaveCfg.u32ReleaseCount++;
    return nRet;
}

HI_VOID ISR_CallbackForWbc0Finish(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{
    HI_S32 nRet;

    if ((HI_TRUE == stIsogenyInfo.bChangeStatus) || (ISOGBNY_BUFFER_STATUS_INIT != stIsogenyInfo.enIsogenyBuffStatus ))
    {
        return;
    }

    if (HI_NULL == stIsogenyInfo.stWBC0Cfg.pstWC0CfgNode )
    {
        return;
    }

    nRet = DispBuf_AddFullNode(&stIsogenyInfo.stBuffer, stIsogenyInfo.stWBC0Cfg.pstWC0CfgNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    stIsogenyInfo.stWBC0Cfg.pstWC0CfgNode = HI_NULL;

    return ;
}


HI_VOID ISR_CallbackIsogenyProcess(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{
    WBC_HD_WIN_INFOR_S stWnHDFrameInfor;
    HI_S32 nRet;
    DISP_BUF_NODE_S* pstNode;
    HI_DISP_DISPLAY_INFO_S stHdDispInfo;
    HI_DISP_DISPLAY_INFO_S stSdDispInfo;
    HI_DRV_VIDEO_FRAME_S*  psCfgtFrame = HI_NULL ;
    ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode = ISOGBNY_BUFFER_MODE_BUTT;
    HI_SIZE_T irqflag = 0;
	
	memset((HI_VOID*)&stHdDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));
    memset((HI_VOID*)&stSdDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);

    if ((HI_TRUE == stIsogenyInfo.bChangeStatus) || (ISOGBNY_BUFFER_STATUS_INIT != stIsogenyInfo.enIsogenyBuffStatus  ))
    {
        goto _ISOGENY_EXIT;
    }

    ISOGENY_GetHDWinFrameInfo(&stWnHDFrameInfor);
    (HI_VOID) DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &stSdDispInfo);
    (HI_VOID) DISP_GetDisplayInfo(HI_DRV_DISPLAY_1, &stHdDispInfo);

    /*judge mode*/
    enIsogenyBuffMode = ISOGENY_JudgeBufferMode(stHdDispInfo.u32RefreshRate, stSdDispInfo.u32RefreshRate, stWnHDFrameInfor.bV0Secure, stWnHDFrameInfor.bV1Secure);

    if ( enIsogenyBuffMode != stIsogenyInfo.enIsogenyBuffMode )
    {
        /*wake up ISOgeny thread !*/
        stIsogenyInfo.stIsogenyThread.enIsogenyBuffMode = enIsogenyBuffMode;
        stIsogenyInfo.bChangeStatus = HI_TRUE;
        stIsogenyInfo.stIsogenyThread.bChangeProcess = HI_TRUE;
        //debug_print("from %d  change to %d.\n",stIsogenyInfo.enIsogenyBuffMode,enIsogenyBuffMode);
        wake_up(&stIsogenyInfo.stIsogenyThread.stWaitQueHead);
        goto _ISOGENY_EXIT;
    }

    /*config WBC0, and get free Buffer */
    if ( HI_FALSE == ISOGENY_GetHDWinEnable())
    {
        stIsogenyInfo.stSlaveCfg.bEnable = HI_FALSE;
        goto _ISOGENY_EXIT;
    }

    stIsogenyInfo.stSlaveCfg.bEnable = HI_TRUE;

    psCfgtFrame = ISOGBNY_GetFreeFrame(stIsogenyInfo.enIsogenyBuffMode);
    if ((HI_NULL == psCfgtFrame))
    {
        goto _ISOGENY_EXIT;
    }

    nRet = DispBuf_GetEmptyNode(&stIsogenyInfo.stBuffer, &pstNode);
    if (nRet != HI_SUCCESS)
    {
        goto _ISOGENY_EXIT;
    }

    nRet = DispBuf_DelEmptyNode(&stIsogenyInfo.stBuffer, pstNode);

    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("DispBuf_DelEmptyNode failed, ID=0x%x\n", pstNode->u32ID);
        goto _ISOGENY_EXIT;
    }

    stIsogenyInfo.stWBC0Cfg.bWbcEnable = 1;

    nRet = ISOGBNY_ConfigWbc0(&stIsogenyInfo.stWBC0Cfg, psCfgtFrame);
    if (HI_SUCCESS != nRet  )
    {
        WIN_ERROR("ISOGBNY_ConfigWbc0 failed, ret(%d)\n", nRet);

        nRet = DispBuf_AddEmptyNode(&stIsogenyInfo.stBuffer, pstNode);
        if (HI_SUCCESS != nRet  )
        {
            WIN_ERROR("add  node to empty err, ret(%d)\n", nRet);
        }

        goto _ISOGENY_EXIT;
    }

    memcpy(&pstNode->u32Data, psCfgtFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
    pstNode->u32PlayCnt = 1;
    stIsogenyInfo.stWBC0Cfg.pstWC0CfgNode = pstNode;

    if (ISOGBNY_BUF_SINGL_NUM == ISOGBNY_GetBuffNum(stIsogenyInfo.enIsogenyBuffMode))
    {
        ISR_CallbackForWbc0Finish(HI_NULL, HI_NULL);
    }

_ISOGENY_EXIT:
    ISOGENY_CleanHDWinFrameInfo();
    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    return ;
}

DISP_BUF_NODE_S* ISOGBNY_SlaveGetCfgNode( DISP_BUF_S* pstBuffer, ISOGBNY_SLAVE_CFG_S* pstSlaveCfg)
{
    HI_S32 nRet;
    DISP_BUF_NODE_S* pstCfgNode;
    DISP_BUF_NODE_S* pstNextCfgNode;


    nRet = DispBuf_GetFullNode(pstBuffer, &pstCfgNode);

    if (nRet != HI_SUCCESS)
    {
        return HI_NULL;
    }

    /*up data*/
    nRet = DispBuf_DelFullNode(pstBuffer, pstCfgNode);

    if (nRet != HI_SUCCESS)
    {
        WIN_INFO("DelFullNode err(%d).\n", nRet);
        return HI_NULL;
    }

    /*get newer*/
    nRet = DispBuf_GetFullNode(pstBuffer, &pstNextCfgNode);

    if (nRet != HI_SUCCESS)
    {
        return pstCfgNode;
    }

    nRet = DispBuf_DelFullNode(pstBuffer, pstNextCfgNode);

    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("DelFullNode err(%d).\n", nRet);
        return pstCfgNode;
    }

    nRet = ISOGBNY_ReleaseNode(pstBuffer, pstCfgNode);

    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("ReleaseNode err(%d).\n", nRet);
        return pstNextCfgNode;
    }

    pstSlaveCfg->u32DisardCount++;

    return pstNextCfgNode;

}


HI_VOID ISOGBNY_SlaveNoFrameCfg( ISOGBNY_SLAVE_CFG_S* pstSlaveCfg, DISP_BUF_NODE_S* pstNewNode)
{

    if ( HI_NULL == pstNewNode)
    {
        pstSlaveCfg->pstDispNode = HI_NULL;
        pstSlaveCfg->pstCurrentNode = HI_NULL;
        pstSlaveCfg->pstNextNode = HI_NULL;
    }
    else
    {
        pstSlaveCfg->pstDispNode = HI_NULL;
        pstSlaveCfg->pstCurrentNode = pstNewNode;
        pstSlaveCfg->pstNextNode = HI_NULL;
    }

}


HI_VOID ISOGBNY_SlaveNormalCfg( ISOGBNY_SLAVE_CFG_S* pstSlaveCfg, DISP_BUF_NODE_S* pstNewNode)
{
    HI_S32 nRet;

    nRet = ISOGBNY_ReleaseNode(&stIsogenyInfo.stBuffer, pstSlaveCfg->pstDispNode);

    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("ISOGBNY_ReleaseNode err(%d).\n", nRet);
        return ;
    }

    if ( HI_NULL == pstNewNode)
    {
        if ( HI_NULL == pstSlaveCfg->pstNextNode)
        {
            pstSlaveCfg->pstDispNode = HI_NULL;
            pstSlaveCfg->pstCurrentNode = pstSlaveCfg->pstCurrentNode;
            pstSlaveCfg->pstNextNode = HI_NULL;
        }
        else
        {
            pstSlaveCfg->pstDispNode = pstSlaveCfg->pstCurrentNode;
            pstSlaveCfg->pstCurrentNode = pstSlaveCfg->pstNextNode;
            pstSlaveCfg->pstNextNode = HI_NULL;
        }
    }
    else
    {
        if ( HI_NULL == pstSlaveCfg->pstNextNode)
        {
            pstSlaveCfg->pstDispNode = pstSlaveCfg->pstCurrentNode;
            pstSlaveCfg->pstCurrentNode = pstNewNode;
            pstSlaveCfg->pstNextNode = HI_NULL;
        }
        else
        {
            pstSlaveCfg->pstDispNode = pstSlaveCfg->pstCurrentNode;
            pstSlaveCfg->pstCurrentNode = pstSlaveCfg->pstNextNode;
            pstSlaveCfg->pstNextNode = pstNewNode;
        }
    }
}


HI_VOID  ISOGBNY_SlaveOneFrameCfg( ISOGBNY_SLAVE_CFG_S* pstSlaveCfg, DISP_BUF_NODE_S* pstNewNode)
{

    if ( HI_NULL == pstNewNode)
    {
        pstSlaveCfg->pstDispNode = HI_NULL;
        pstSlaveCfg->pstCurrentNode = pstSlaveCfg->pstCurrentNode;
        pstSlaveCfg->pstNextNode = HI_NULL;
        pstSlaveCfg->u32RepeatCount++;
    }
    else
    {
        pstSlaveCfg->pstDispNode = pstSlaveCfg->pstCurrentNode;
        pstSlaveCfg->pstCurrentNode = pstNewNode;
        pstSlaveCfg->pstNextNode = HI_NULL;
    }
}

HI_VOID  ISOGBNY_SlaveSingleCfg( ISOGBNY_SLAVE_CFG_S* pstSlaveCfg, DISP_BUF_NODE_S* pstNewNode)
{
    HI_S32 nRet;

    if ( HI_NULL == pstNewNode)
    {
        pstSlaveCfg->pstDispNode = HI_NULL;
        pstSlaveCfg->pstCurrentNode = pstSlaveCfg->pstCurrentNode;
        pstSlaveCfg->pstNextNode = HI_NULL;
        pstSlaveCfg->u32RepeatCount++;
    }
    else
    {
        if ( HI_NULL != pstSlaveCfg->pstDispNode)
        {
            nRet = ISOGBNY_ReleaseNode(&stIsogenyInfo.stBuffer, pstSlaveCfg->pstDispNode);

            if (nRet != HI_SUCCESS)
            {
                return ;
            }
        }

        if (HI_NULL != pstSlaveCfg->pstCurrentNode)
        {
            nRet = ISOGBNY_ReleaseNode(&stIsogenyInfo.stBuffer, pstSlaveCfg->pstCurrentNode);

            if (nRet != HI_SUCCESS)
            {
                return ;
            }
        }

        pstSlaveCfg->pstDispNode = HI_NULL;
        pstSlaveCfg->pstCurrentNode = pstNewNode;
        pstSlaveCfg->pstNextNode = HI_NULL;

    }

}




HI_VOID ISR_CallbackForSlaveProcess(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{
    HI_DRV_VIDEO_FRAME_S* pstDispFrame = HI_NULL;
    DISP_BUF_NODE_S* pstNeWNode = HI_NULL;
    ISOGBNY_SLAVE_CFG_S* pstSlaveCfg = HI_NULL;
    HI_SIZE_T irqflag = 0;

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);

    /* put config frame to buffer list */
    if ((HI_TRUE == stIsogenyInfo.bChangeStatus) || (ISOGBNY_BUFFER_STATUS_INIT != stIsogenyInfo.enIsogenyBuffStatus ))
    {
        ISR_SlaveWinConfigFrame(HI_NULL, pstInfo);
        goto ISR_CallbackForSlaveProcess_EXIT;
    }


    pstSlaveCfg = &stIsogenyInfo.stSlaveCfg;

    if (HI_FALSE == pstSlaveCfg->bEnable)
    {
       ISR_SlaveWinConfigFrame(HI_NULL, pstInfo);
       goto ISR_CallbackForSlaveProcess_EXIT;
    }

    pstNeWNode = ISOGBNY_SlaveGetCfgNode(&stIsogenyInfo.stBuffer, pstSlaveCfg);
    pstSlaveCfg->u32CfgCount++;
    if ( HI_NULL != pstNeWNode)
    {
        pstSlaveCfg->u32RecCount++;
    }

    if (ISOGBNY_BUF_SINGL_NUM == ISOGBNY_GetBuffNum(stIsogenyInfo.enIsogenyBuffMode))
    {
        /*signle buff mode */
        if (HI_NULL == pstSlaveCfg->pstCurrentNode)
        {
            /*last cfg	has no frame*/
            ISOGBNY_SlaveNoFrameCfg(pstSlaveCfg, pstNeWNode);
        }
        else
        {
            ISOGBNY_SlaveSingleCfg(pstSlaveCfg, pstNeWNode);
        }
    }
    else
    {
        if (HI_NULL == pstSlaveCfg->pstCurrentNode)
        {
            /*last cfg  has no frame*/
            ISOGBNY_SlaveNoFrameCfg(pstSlaveCfg, pstNeWNode);
        }
        else if (HI_NULL == pstSlaveCfg->pstDispNode)
        {
            /*last cfg has 1 frame*/
            ISOGBNY_SlaveOneFrameCfg(pstSlaveCfg, pstNeWNode);
        }
        else
        {
            /*last cfg has more than 2 frame*/
            ISOGBNY_SlaveNormalCfg(pstSlaveCfg, pstNeWNode);
        }
    }


    if ( HI_NULL != pstSlaveCfg->pstCurrentNode)
    {
        pstDispFrame = (HI_DRV_VIDEO_FRAME_S*) & (pstSlaveCfg->pstCurrentNode->u32Data[0]);
    }
    else
    {
        pstDispFrame = HI_NULL;
    }

    ISR_SlaveWinConfigFrame(pstDispFrame, pstInfo);

ISR_CallbackForSlaveProcess_EXIT:
    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    return ;

}


HI_VOID ISOGBNY_GetWBC0ProcInfor(ISOGBNY_WBC0_INFO_S* pstWBC0ProcInfo)
{

    pstWBC0ProcInfo =  &stIsogenyInfo.stIsogenyProcInfo.stWBC0ProcInfo;
}

HI_VOID ISOGBNY_GetSlaveProcInfor(ISOGBNY_SLAVE_INFO_S* pstSlaveProcInfo)
{
    pstSlaveProcInfo = &stIsogenyInfo.stIsogenyProcInfo.stSlaveProcInfo;
}

HI_VOID WIN_GetIsogenyPocInfor(ISOGBNY_PROC_INFO_S* pstIsogenyProcInfo)
{
    pstIsogenyProcInfo->bChangeStatus = stIsogenyInfo.bChangeStatus;
    pstIsogenyProcInfo->enIsogenyBuffStatus = stIsogenyInfo.enIsogenyBuffStatus;
    pstIsogenyProcInfo->enIsogenyBuffMode = stIsogenyInfo.enIsogenyBuffMode;
    pstIsogenyProcInfo->enWBCForcePoint = stIsogenyInfo.enWBCForcePoint;
    pstIsogenyProcInfo->enWBForceField = stIsogenyInfo.enWBForceField;

    pstIsogenyProcInfo->pstBuffer = &stIsogenyInfo.stBuffer;

    pstIsogenyProcInfo->stSlaveProcInfo.pstSalveCurrentNode = stIsogenyInfo.stSlaveCfg.pstCurrentNode;
    pstIsogenyProcInfo->stSlaveProcInfo.pstSalveDispNode = stIsogenyInfo.stSlaveCfg.pstDispNode;
    pstIsogenyProcInfo->stSlaveProcInfo.pstSalveNextNode = stIsogenyInfo.stSlaveCfg.pstNextNode;

    pstIsogenyProcInfo->stSlaveProcInfo.pstSalveCfgFrame = (HI_DRV_VIDEO_FRAME_S*)&stIsogenyInfo.stSlaveCfg.pstCurrentNode->u32Data;

    pstIsogenyProcInfo->stSlaveProcInfo.u32SalveCfgCount = stIsogenyInfo.stSlaveCfg.u32CfgCount;
    pstIsogenyProcInfo->stSlaveProcInfo.u32SalveRecCount = stIsogenyInfo.stSlaveCfg.u32RecCount;
    pstIsogenyProcInfo->stSlaveProcInfo.u32SalveDisardCount = stIsogenyInfo.stSlaveCfg.u32DisardCount;
    pstIsogenyProcInfo->stSlaveProcInfo.u32SalveRepeatCount = stIsogenyInfo.stSlaveCfg.u32RepeatCount;
    pstIsogenyProcInfo->stSlaveProcInfo.u32SalveReleaseCount = stIsogenyInfo.stSlaveCfg.u32ReleaseCount;

    return ;
}



/***************************************************************/

extern HI_U8* g_pVDPColorSpaceString[HI_DRV_CS_BUTT];
extern HI_U8* g_pWinTrueString[2];
extern HI_U8* g_pWinFrameTypetring[HI_DRV_FT_BUTT];
extern HI_U8* g_pWinAspectCvrsString[HI_DRV_ASP_RAT_MODE_BUTT] ;
extern HI_U8* g_pWinFieldTypeString[HI_DRV_FIELD_BUTT + 1];
extern HI_U8* g_pWinYNString[2];
extern HI_U8* g_pWinFieldModeString[HI_DRV_FIELD_BUTT];


HI_S32 ISOGENY_ProcRead(struct seq_file* p, HI_VOID* v)
{

    HI_DRV_VIDEO_FRAME_S* pstNewFrame;
    HI_DRV_VIDEO_PRIVATE_S* pstPriv;
    HI_U32 i;
    ISOGBNY_PROC_INFO_S stIsogenyProcInfo;

    WIN_GetIsogenyPocInfor(&stIsogenyProcInfo);

    PROC_PRINT(p, "-----------------Isogeny  frame--------------------------------|\n");
    pstNewFrame = stIsogenyProcInfo.stSlaveProcInfo.pstSalveCfgFrame;
    if (pstNewFrame)
    {
        pstPriv 	= (HI_DRV_VIDEO_PRIVATE_S*)(&pstNewFrame->u32Priv[0]);
        PROC_PRINT(p, "%-19s:%-20s\n",	"wbc field", g_pWinFieldTypeString[pstNewFrame->enFieldMode]);
        PROC_PRINT(p, "%-19s:0x%x\n", "|FrameIndex",  pstNewFrame->u32FrameIndex);
        PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|SrcPTS/PTS",  pstNewFrame->u32SrcPts, pstNewFrame->u32Pts);
        PROC_PRINT(p, "%-19s:%d\n", "|PlayTime", pstPriv->u32PlayTime);
        PROC_PRINT(p, "%-19s:%-20s\n", "|FieldMode", g_pWinFieldModeString[pstNewFrame->enFieldMode]);
        PROC_PRINT(p, "%-19s:%d\n", "|Fidelity", pstPriv->u32Fidelity);
        PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|H8:YAddr/YStride", pstNewFrame->stBufAddr[0].u32PhyAddr_Y, pstNewFrame->stBufAddr[0].u32Stride_Y);
        PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|H8:CAddr/CStride", pstNewFrame->stBufAddr[0].u32PhyAddr_C, pstNewFrame->stBufAddr[0].u32Stride_C);
        PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|L2:YAddr/YStride", pstNewFrame->stBufAddr_LB[0].u32PhyAddr_Y, pstNewFrame->stBufAddr_LB[0].u32Stride_Y);
        PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|L2:CAddr/CStride", pstNewFrame->stBufAddr_LB[0].u32PhyAddr_C, pstNewFrame->stBufAddr_LB[0].u32Stride_C);
        PROC_PRINT(p, "%-19s:%-20d\n", "Secure", pstNewFrame->bSecure);
    }

    PROC_PRINT(p, "-----------------Isogeny  infor--------------------------------|\n");
    PROC_PRINT(p, "%-19s:%-20d\n",	"wbc point", stIsogenyProcInfo.enWBCForcePoint);
    PROC_PRINT(p, "%-19s:%-20d\n",	"enIsogenyBuffMode:", stIsogenyProcInfo.enIsogenyBuffMode);
    PROC_PRINT(p, "%-19s:%-20d\n",	"enIsogenyBuffStatus:", stIsogenyProcInfo.enIsogenyBuffStatus);
    PROC_PRINT(p, "%-19s:%-20d\n",	"enWBCForcePoint:", stIsogenyProcInfo.enWBCForcePoint);
    PROC_PRINT(p, "%-19s:%-20d\n",	"enWBForceField:", stIsogenyProcInfo.enWBForceField);
    PROC_PRINT(p, "%-19s:%-20d\n",	"enWBForceField:", stIsogenyProcInfo.enWBForceField);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32WBCFinishCount:", stIsogenyProcInfo.stWBC0ProcInfo.u32WBCFinishCount);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32WBCDisardCount:", stIsogenyProcInfo.stWBC0ProcInfo.u32WBCDisardCount);

    if (HI_NULL != stIsogenyProcInfo.stSlaveProcInfo.pstSalveNextNode)
    {
        PROC_PRINT(p, "%-19s:0x%-20x\n",	"SalveNextNode:ID",  stIsogenyProcInfo.stSlaveProcInfo.pstSalveNextNode->u32ID);
    }
    else
    {
        PROC_PRINT(p, "%-19s:%-20s\n",	"SalveNextNode:", "Nothing");
    }

    if (HI_NULL != stIsogenyProcInfo.stSlaveProcInfo.pstSalveCurrentNode)
    {
        PROC_PRINT(p, "%-19s:0x%-20x\n",	"SalveCurrentNode:ID",	stIsogenyProcInfo.stSlaveProcInfo.pstSalveCurrentNode->u32ID);
        PROC_PRINT(p, "%-19s:%-20d\n",	"Config FrameIndx",  stIsogenyProcInfo.stSlaveProcInfo.pstSalveCfgFrame->u32FrameIndex);
    }
    else
    {
        PROC_PRINT(p, "%-19s:%-20s\n",	"SalveCurrentNode:", "Nothing");
    }

    if (HI_NULL != stIsogenyProcInfo.stSlaveProcInfo.pstSalveDispNode)
    {
        PROC_PRINT(p, "%-19s:0x%-20x\n",	"SalveDispNode:ID",  stIsogenyProcInfo.stSlaveProcInfo.pstSalveDispNode->u32ID);
    }
    else
    {
        PROC_PRINT(p, "%-19s:%-20s\n",	"SalveDispNode:", "Nothing");
    }

    PROC_PRINT(p, "%-19s:%-20d\n",	"u32SalveCfgCount:", stIsogenyProcInfo.stSlaveProcInfo.u32SalveCfgCount);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32SalveRecCount:", stIsogenyProcInfo.stSlaveProcInfo.u32SalveRecCount);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32SalveDisardCount:", stIsogenyProcInfo.stSlaveProcInfo.u32SalveDisardCount);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32SalveRepeatCount:", stIsogenyProcInfo.stSlaveProcInfo.u32SalveRepeatCount);
    PROC_PRINT(p, "%-19s:%-20d\n",	"u32SalveReleaseCount:", stIsogenyProcInfo.stSlaveProcInfo.u32SalveReleaseCount);

    for (i = 0; i < stIsogenyProcInfo.pstBuffer->u32Number;)
    {
        pstNewFrame = (HI_DRV_VIDEO_FRAME_S*) & (stIsogenyProcInfo.pstBuffer->pstBufArray[i]->u32Data);
        PROC_PRINT(p, "[%d,0x%x,%d,0x%x] ",
                   stIsogenyProcInfo.pstBuffer->pstBufArray[i]->u32ID,
                   stIsogenyProcInfo.pstBuffer->pstBufArray[i]->u32State,
                   pstNewFrame->u32FrameIndex,
                   pstNewFrame->stBufAddr[0].u32PhyAddr_Y
                  );

        i++;

        if ( (i % 4) == 0)
        {
            PROC_PRINT(p, "\n");
        }
    }

    PROC_PRINT(p, "\n");

    PROC_PRINT(p, "Empty:read->%d,Write->%d \n", stIsogenyProcInfo.pstBuffer->u32EmptyReadPos, stIsogenyProcInfo.pstBuffer->u32EmptyWritePos);

    for (i = 0; i < stIsogenyProcInfo.pstBuffer->u32Number; i++)
    {
        PROC_PRINT(p, "[%d],", stIsogenyProcInfo.pstBuffer->u32EmptyArray[i]);

    }

    PROC_PRINT(p, "\n");

    PROC_PRINT(p, "Full:read->%d,Write->%d \n", stIsogenyProcInfo.pstBuffer->u32FullReaddPos, stIsogenyProcInfo.pstBuffer->u32FullWritePos);

    for (i = 0; i < stIsogenyProcInfo.pstBuffer->u32Number; i++)
    {
        PROC_PRINT(p, "[%d],", stIsogenyProcInfo.pstBuffer->u32FullArray[i]);

    }

    PROC_PRINT(p, "\n");

    return HI_SUCCESS;
}

HI_S32 ISOGENY_ProcWrite(struct file* file,
                         const char __user* buf, size_t count, loff_t* ppos)
{


    return count;

}

HI_S32 ISOGENY_AddToProc(HI_VOID)
{
    DRV_PROC_ITEM_S*  pProcItem;
    HI_CHAR           ProcName[12];
    HI_S32 s32Ret;

    s32Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "isogeny");

    if (0 == s32Ret)
    {
        HI_ERR_WIN("HI_OSAL_Snprintf failed!\n");
        return HI_FAILURE;
    }

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);

    if (!pProcItem)
    {
        HI_ERR_WIN("Isogeny add proc failed!\n");
        return HI_ERR_VO_ADD_PROC_ERR;
    }

    pProcItem->data  = &stIsogenyInfo;
    pProcItem->read  = ISOGENY_ProcRead;
    pProcItem->write = ISOGENY_ProcWrite;

    return HI_SUCCESS;
}

HI_VOID ISOGENY_DelFromProc(HI_VOID)
{
    HI_CHAR           ProcName[12];
    HI_S32 s32Ret;

    s32Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "isogeny");

    if (0 == s32Ret)
    {
        HI_ERR_WIN("HI_OSAL_Snprintf failed!\n");
        return;
    }

    HI_DRV_PROC_RemoveModule(ProcName);

    return ;
}


HI_S32 ISOGBNY_Init(HI_VOID)
{
    HI_S32 nRet;
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_DRV_DISP_CALLBACK_S stCB1;

    memset(&stIsogenyInfo , 0x0, sizeof(ISOGBNY_CFG_INFOR_S));

    stIsogenyInfo.bChangeStatus = HI_FALSE;
    stIsogenyInfo.enIsogenyBuffMode = ISOGBNY_BUFFER_MODE_BUTT;
    stIsogenyInfo.enIsogenyBuffStatus = ISOGBNY_BUFFER_STATUS_DEINIT;

    stIsogenyInfo.enWBCForcePoint = WBC_WB_POINT_AUTO;
    stIsogenyInfo.enWBForceField = WBC_WB_FIELD_AUTO;

    nRet = ISOGBNY_CreatProcessThread();

    if (nRet)
    {
        WIN_ERROR("ISOGBNY Creat callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stCB.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_LOW;
    stCB.pfDISP_Callback = ISR_CallbackIsogenyProcess;
    nRet = DISP_RegCallback(HI_DRV_DISPLAY_1, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stCB1.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB1.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_LOW;
    stCB1.pfDISP_Callback = ISR_CallbackForWbc0Finish;
    nRet = DISP_RegCallback(HI_DRV_DISPLAY_1, HI_DRV_DISP_C_VID_WBC, &stCB1);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stCB.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB.pfDISP_Callback = ISR_CallbackForSlaveProcess;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;

    nRet = DISP_RegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    if (VDP_NORMAL_STAGE == DRV_WIN_GetSftwareStage())
    {
        nRet = ISOGENY_AddToProc();
         if (nRet)
        {
            WIN_ERROR("ISOGENY AddToProc failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }
    }

    /*set default mode */
    stIsogenyInfo.bChangeStatus = HI_TRUE;
    ISOGBNY_FrameBuf_Init(ISOGBNY_BUFFER_MODE_SINGL_NONSEC);
    stIsogenyInfo.bChangeStatus = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 ISOGBNY_DeInit(HI_VOID)
{
    HI_S32 nRet;
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_DRV_DISP_CALLBACK_S stCB1;

    stIsogenyInfo.bChangeStatus = HI_TRUE;
    stIsogenyInfo.enWBCForcePoint = WBC_WB_POINT_AUTO;
    stIsogenyInfo.enWBForceField = WBC_WB_FIELD_AUTO;

    msleep(50);
    nRet = ISOGBNY_DestroyProcessThread();

    if (nRet)
    {
        WIN_ERROR("ISOGBNY Creat callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stIsogenyInfo.enIsogenyBuffMode = ISOGBNY_BUFFER_MODE_BUTT;

    /*register in hd, for configure the wbc.*/
    stCB.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_LOW;
    stCB.pfDISP_Callback = ISR_CallbackIsogenyProcess;

    nRet = DISP_UnRegCallback(HI_DRV_DISPLAY_1, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stCB1.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB1.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_LOW;
    stCB1.pfDISP_Callback = ISR_CallbackForWbc0Finish;
    nRet = DISP_UnRegCallback(HI_DRV_DISPLAY_1, HI_DRV_DISP_C_VID_WBC, &stCB1);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return nRet;
    }

    stCB1.hDst  = (HI_VOID*)(unsigned long)WIN_WBC_VP_HANDLE;
    stCB.pfDISP_Callback = ISR_CallbackForSlaveProcess;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    nRet = DISP_UnRegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

    if (nRet)
    {
        WIN_ERROR("ISOGENY register callback failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    if (VDP_NORMAL_STAGE == DRV_WIN_GetSftwareStage())
    {
        ISOGENY_DelFromProc();
    }

    if (ISOGBNY_BUFFER_STATUS_INIT == stIsogenyInfo.enIsogenyBuffStatus )
    {
        (HI_VOID) ISOGBNY_FrameBuf_DeInit();
    }

    return HI_SUCCESS;
}


