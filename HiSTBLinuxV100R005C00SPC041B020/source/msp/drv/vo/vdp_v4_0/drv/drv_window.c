
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name      : drv_window.h
Version          : Initial Draft
Author          : Hisilicon multimedia software group
Created          : 2012/12/30
Last Modified :
Description   :
Function List :
History          :
******************************************************************************/
#include "drv_display.h"
#include "drv_window.h"
#include "drv_win_frc.h"
#include "drv_win_policy.h"
#include "drv_win_priv.h"
#include "hi_drv_sys.h"
#include "hi_drv_stat.h"
#include "drv_vdec_ext.h"
#include "drv_disp_hal.h"
#include "drv_disp_alg_service.h"
#include "hi_drv_module.h"
#include "drv_pq_ext.h"
#include "drv_win_alg_ratio.h"

#include "drv_win_hdr.h"
#include "drv_vdp_hdr_com.h"

#include "decompress_api.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_HANDLE g_hSlaveWin = 0;

DEFINE_SPINLOCK(g_threadIsr_Lock);


/******************************************************************************
    global object
******************************************************************************/
volatile HI_S32 s_s32WindowGlobalFlag = WIN_DEVICE_STATE_CLOSE;
static DISPLAY_WINDOW_S stDispWindow;
static VIRTUAL_WINDOW_S stVirWindow;

static HI_U32 WinGetHDWinNum(HI_VOID)
{
    return stDispWindow.u32WinNumber[HI_DRV_DISPLAY_1];
}

HI_BOOL WinCheckWhetherWbcIsogenyMode(HI_VOID)
{
    if ((stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
         && (stDispWindow.bIsogeny == HI_TRUE))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_BOOL WinCheckWhetherCanToCreate(HI_HANDLE hWin)
{
    WINDOW_S *pstWin =  HI_NULL;
    HI_BOOL bVirtual = HI_FALSE;

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        pstWin = WinGetWindow(hWin);
        if (!pstWin)
        {
            WIN_WARN("WIN is not exist! %#x\n", hWin);
            return HI_FALSE;
        }

        /*in isogeny wbc mode, the sd window is a singleton mode.*/
        if (WinCheckWhetherWbcIsogenyMode()
            && (stDispWindow.u32CurrentHdWindowCounts > 1)
            && (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE))
        {
            return  HI_FALSE;
        }
    }

    return HI_TRUE;
}



HI_BOOL WinCheckProcCanBeRemoved(HI_HANDLE hWin)
{
    WINDOW_S *pstWin =  HI_NULL;

    HI_BOOL bVirtual = HI_FALSE;

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        pstWin = WinGetWindow(hWin);

        /*when remove slave window from proc, there exists one hd window.
            *since remove proc infor should go first.*/
        if ((HI_NULL != pstWin)
             && WinCheckWhetherWbcIsogenyMode()
             && (stDispWindow.u32CurrentHdWindowCounts != 1)
             && (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE))
        {
            return HI_FALSE;
        }
        else
        {
            return HI_TRUE;
        }
    }

    return HI_TRUE;
}

HI_BOOL WinCheckWhetherCanToDestroy(HI_HANDLE hWin)
{
    WINDOW_S *pstWin =  HI_NULL;
    HI_BOOL bVirtual = HI_FALSE;

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        pstWin = WinGetWindow(hWin);
        if(HI_NULL == pstWin)
        {
            return HI_FALSE;
        }

        /*only when  the last master win destroyed, the  slave window can be destroyed.*/
        if (WinCheckWhetherWbcIsogenyMode()
               && (stDispWindow.u32CurrentHdWindowCounts != 0)
               && (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE))
        {
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}


HI_S32 WinQueueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo, HI_DRV_DOLBY_META_INFO_S *pstDolbyMdInfo);
HI_S32 WinBufferPutULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 WinBufferGetULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame);

#if defined(DAC_TYPE_SYNOPSYS)

typedef struct hiDRV_GAMMA_S
{
   HI_U32 u32Gamma;
   HI_U32 u32ParaIndex;
   HI_BOOL bGammaChange;
}HI_DRV_GAMMA_S;

#define  GAMMA_UP_DELAY  4
#define  GAMMA_PARA_NO_USE  3
HI_DRV_GAMMA_S g_stGamma[HI_DRV_DISPLAY_BUTT] =
{
    {0,GAMMA_PARA_NO_USE,HI_FALSE},
    {0,GAMMA_PARA_NO_USE,HI_FALSE},
    {0,GAMMA_PARA_NO_USE,HI_FALSE}
};
#endif

HI_BOOL   WinGetRegWinManageStatus(HI_VOID)
{
    return stDispWindow.bWinManageStatus;
}

HI_VOID WinSetRegWinManageStatus(HI_BOOL bRegStatus)
{
    stDispWindow.bWinManageStatus = bRegStatus;
    return ;
}
/*
  *this func is for  queue/dequeue  function pairs in UNF level.
  *vdp  should store the displayed frame to useless array, and wait dequeue
  * unf func to retrieve the frame.
  */
HI_S32 Win_ReleaseDisplayedFrame_ForDequeue(HI_HANDLE hpstWin, HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 nRet = 0;
    WINDOW_S *pstWin = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

    WinCheckWindow(hpstWin, pstWin);
    WinCheckNullPointer(pstFrm);

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S*)&(pstFrm->u32Priv[0]);

    /*Win_ReleaseDisplayedFrame_ForDequeue is for Scenes under which
      vdec and window not attached. in non-attached mode,  there are 2 ways for frame operation:
      1) user can call queue/dequeue;
      2) user queue frame into window ,  window release by fence.
            when frame released  by fence, we should not put the frames into a list to wait for user calling dequeue,
         because  in this condition, user will never call dequeue to get their frames, because they use fence to release.
      */
    if (pstPriv->bForFenceUse != HI_TRUE)
    {
        nRet = WinBufferPutULSFrame(&pstWin->stBuffer, pstFrm);
    }
    else
    {
        pstPriv->bForFenceUse = HI_FALSE;
    }

    return nRet;
}

/*
  * when UNF DEQUEUE func works, this func should be called, retrieve the frames.
  */
HI_S32 Win_Dequeue_DisplayedFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 nRet = 0;
    WINDOW_S *pstWin = HI_NULL;

    WinCheckWindow(hWin, pstWin);
    nRet = WinBufferGetULSFrame(&pstWin->stBuffer, pstFrm);

    return nRet;
}
VIRTUAL_S *WinGetVirWindow(HI_U32 u32WinIndex)
{

    if (!stVirWindow.u32WinNumber)
    {
        WIN_WARN("Not found this window!\n");
        return HI_NULL;
    }

    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if (  WinGetDispId(u32WinIndex) != WIN_INDEX_VIRTUAL_CHANNEL)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if ( WinGetId(u32WinIndex) >= WIN_VIRTAUL_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    return stVirWindow.pstWinArray[WinGetId(u32WinIndex)];
}

HI_VOID ISR_CallbackForDispModeChange(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID ISR_CallbackForWinProcess(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID WinUpdateDispInfo(WINDOW_S *pstWin, HI_DISP_DISPLAY_INFO_S * pstDsipInfo);

/******************************************************************************
    Win device function
******************************************************************************/

HI_S32 Check_WinFunc(WINDOW_S *pWin)
{
    if (!pWin)
        return HI_ERR_VO_NULL_PTR;

    if ((!pWin->stVLayerFunc.PF_ReleaseLayer)
        ||(!pWin->stVLayerFunc.PF_SetEnable)
        ||(!pWin->stVLayerFunc.PF_AcquireLayerByDisplay)
        ||(!pWin->stVLayerFunc.PF_MovBottom)
        ||(!pWin->stVLayerFunc.PF_MovTop)
        ||(!pWin->stVLayerFunc.PF_AcquireLayerByDisplay)
        )
    {
        return HI_ERR_VO_NO_INIT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

HI_S32 Win_WaitForLayerClosed(WINDOW_S *pstWin, WINDOW_S *pstSlaveWin)
{
    HI_U32 u32ChnHd_time = 0,  u32Chnsd_time = 0, u32timeout = 0;
    HI_DRV_DISP_CALLBACK_INFO_S stInfo;
    HI_SIZE_T irqflag = 0;

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    /*Config regitster to close  hd layer.*/
    if (HI_NULL != pstWin)
    {
        memset((HI_VOID*)&stInfo, 0, sizeof(HI_DRV_DISP_CALLBACK_INFO_S));
        (HI_VOID)DISP_GetDisplayInfo(pstWin->enDisp, &stInfo.stDispInfo);

        Win_UpdateVideoLayerSetting(pstWin->enDisp, &stInfo);
        (HI_VOID)DISP_GetVbiTimeInfo(pstWin->enDisp, HI_NULL, &u32ChnHd_time);
    }

    /*Config regitster to close sd layer.*/
    if ((HI_NULL != pstSlaveWin) && (HI_FALSE == pstSlaveWin->bEnable))
    {
        memset((HI_VOID*)&stInfo, 0, sizeof(HI_DRV_DISP_CALLBACK_INFO_S));
        (HI_VOID)DISP_GetDisplayInfo(pstSlaveWin->enDisp, &stInfo.stDispInfo);
        Win_UpdateVideoLayerSetting(pstSlaveWin->enDisp, &stInfo);
        (HI_VOID)DISP_GetVbiTimeInfo(pstSlaveWin->enDisp, HI_NULL, &u32Chnsd_time);
    }

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);

    if (u32Chnsd_time >= u32ChnHd_time)
    {
        u32timeout  = u32Chnsd_time;
    }
    else
    {
        u32timeout  = u32ChnHd_time;
    }

    msleep(u32timeout);
    return  HI_SUCCESS;
}

HI_VOID Win_UpdateHdFramInfo(WIN_HAL_PARA_S *pstLayerPara,WIN_CONFIG_S *pstCfg,HI_BOOL bSecure);

HI_VOID WinDisableSingleLayerRegion(WINDOW_S *pstWin, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    WIN_HAL_PARA_S stParam;

    memset(&stParam,0,sizeof(WIN_HAL_PARA_S));

    if (pstWin->u32VideoLayer != VDP_LAYER_VID_BUTT)
    {
        stParam.bRegionEnable = HI_FALSE;
        stParam.u32LayerNO    = pstWin->u32VideoLayer;
        stParam.u32RegionNum  = pstWin->u32VideoRegionNo;
        stParam.pstDispInfo   = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);
        stParam.eOutPutColorSpace = pstInfo->stDispInfo.eColorSpace;
        //printk("disable,layer%d,time:%lx!\n",pstWin->u32VideoLayer, jiffies);
        Win_UpdateHdFramInfo(&stParam,&pstWin->stCfg,HI_FALSE);
        pstWin->stVLayerFunc.PF_SetFramePara(pstWin->u32VideoLayer, &stParam);
    }

    return;
}

HI_VOID WinDisableAllLayerRegion(HI_DRV_DISPLAY_E enDisp,const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_U32 i = 0;
    WINDOW_S *pstWin = HI_NULL;

    /*because video-window remmaping, so make a status resetting.*/
    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        pstWin = stDispWindow.pstWinArray[enDisp][i];
        if (pstWin)
        {
            WinDisableSingleLayerRegion(pstWin, pstInfo);
            pstWin->u32VideoLayer = VDP_LAYER_VID_BUTT;
        }
    }

    return;
}

HI_VOID WinLoadNewMapping(HI_DRV_DISPLAY_E enDisp)
{
    HI_U32 i = 0;

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[enDisp][i]
            && (stDispWindow.pstWinArray[enDisp][i]->bEnable))
        {
            stDispWindow.pstWinArray[enDisp][i]->u32VideoLayer
                 = stDispWindow.pstWinArray[enDisp][i]->u32VideoLayerNew;

            stDispWindow.pstWinArray[enDisp][i]->u32VideoRegionNo
                 = stDispWindow.pstWinArray[enDisp][i]->u32VideoRegionNoNew;
        }
        else if (stDispWindow.pstWinArray[enDisp][i]
                && (!stDispWindow.pstWinArray[enDisp][i]->bEnable))
        {
            stDispWindow.pstWinArray[enDisp][i]->u32VideoLayer
                 = VDP_LAYER_VID_BUTT;
            stDispWindow.pstWinArray[enDisp][i]->u32VideoRegionNo
                 = VDP_LAYER_VID_BUTT;
        }

        if (stDispWindow.pstWinArray[enDisp][i])
        {
            stDispWindow.pstWinArray[enDisp][i]->bEnable_Using = stDispWindow.pstWinArray[enDisp][i]->bEnable;
        }
    }

    return ;
}

HI_VOID Win_UpdateVideoLayerSetting(HI_DRV_DISPLAY_E enDstDisp,
                                    const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_U32 i = 0;
    WINDOW_S *pWin = HI_NULL;

    /*judge whether the internal hareware-cfg is changed*/
    if (atomic_read(&stDispWindow.bWindowSytemUpdate[enDstDisp]))
    {
        /*video-window remmaping may occurs, so make a status resetting.
                 *and load new mapping.*/
        WinDisableAllLayerRegion(enDstDisp, pstInfo);
        WinLoadNewMapping(enDstDisp);

        /*to adjust the order, becasue win-layermapping changed.*/
        for (i = 0; i < WINDOW_MAX_NUMBER; i++)
        {
            /*check wether the layer is set  or not.*/
            pWin = stDispWindow.pstWinArray[enDstDisp][i];
            if (pWin)
            {
                atomic_set(&pWin->stCfg.bNewAttrFlag, 1);
                pWin->bDispInfoChange = HI_TRUE;
            }

            if ((pWin) && pWin->u32VideoLayer == VDP_LAYER_VID_BUTT)
            {
                continue;
            }

            if( (pWin) && (!pWin->stVLayerFunc.PF_CheckLayerInit(pWin->u32VideoLayer)))
            {
                (HI_VOID)pWin->stVLayerFunc.PF_InitialLayer(pWin->u32VideoLayer);
            }

            if ((pWin) && (pWin->stWinLayerOpt.bEffective)
                    && (pWin->stWinLayerOpt.layerOptType == LAYER_OPT_ZORDER_ADJUST))
            {
                if (((HI_U32)pWin->stWinLayerOpt.enLayerOpt) == HI_DRV_DISP_ZORDER_MOVETOP)
                {
                    pWin->stVLayerFunc.PF_MovTop(pWin->u32VideoLayer);
                }
                else if (((HI_U32)pWin->stWinLayerOpt.enLayerOpt) == HI_DRV_DISP_ZORDER_MOVEBOTTOM)
                {
                    pWin->stVLayerFunc.PF_MovBottom(pWin->u32VideoLayer);
                }

                /*clear the flag, to avoid taking effect next period.*/
                pWin->stWinLayerOpt.bEffective = HI_FALSE;
                pWin->stWinLayerOpt.layerOptType = LAYER_OPT_BUTT;
                pWin->stWinLayerOpt.enLayerOpt = HI_DRV_DISP_ZORDER_BUTT;
            }
        }

        atomic_set(&stDispWindow.bWindowSytemUpdate[enDstDisp], 0);
    }

}


HI_VOID ISR_CallbackForWinManage(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_DISPLAY_E enDstDisp = (HI_DRV_DISPLAY_E)hDst;
    HI_SIZE_T irqflag = 0;

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    Win_UpdateVideoLayerSetting(enDstDisp, pstInfo);
    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    return ;
}

HI_S32 WinRegWinManageCallback(HI_DRV_DISPLAY_E enDisp)
{
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet= HI_SUCCESS;

    stCB.hDst  = (HI_VOID*)(unsigned long)enDisp;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_HIGH;
    stCB.pfDISP_Callback = ISR_CallbackForWinManage;
    nRet = DISP_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
    if (nRet)
    {
        WIN_WARN("WIN register callback failed in %s!\n", __FUNCTION__);
    }

    return nRet;
}

HI_S32 WinUnRegWinManageCallback(HI_DRV_DISPLAY_E enDisp)
{
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet= HI_SUCCESS;

    stCB.hDst  = (HI_VOID*)(unsigned long)enDisp;
    stCB.pfDISP_Callback = ISR_CallbackForWinManage;
    stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_HIGH;
    nRet = DISP_UnRegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
    if (nRet)
    {
        WIN_ERROR("WIN unregister callback failed in %s!\n", __FUNCTION__);
    }
    return nRet;
}

HI_S32 WIN_DestroyStillFrame(HI_DRV_VIDEO_FRAME_S *pstReleaseFrame)
{
    HI_U32 i;
    WIN_RELEASE_FRM_S* pstWinRelFrame = &stDispWindow.stWinRelFrame;

    for ( i =0; i < MAX_RELEASE_NO; i++)
    {
        if (!pstWinRelFrame->pstNeedRelFrmNode[i] )
        {
            pstWinRelFrame->pstNeedRelFrmNode[i] = pstReleaseFrame;
            pstWinRelFrame->enThreadEvent= EVENT_RELEASE;
            wake_up(&pstWinRelFrame->stWaitQueHead);
            return HI_SUCCESS;
        }
    }

    WIN_ERROR("Release still frame failed ,buff is full!\n");
    return HI_FAILURE;
}

HI_S32 WinReleaseFrameThreadProcess(HI_VOID* pArg)
{
    HI_U32 i;
    WIN_RELEASE_FRM_S* pstWinRelFrame = pArg;

    /*if stop refush release frame buffer*/
    while(!kthread_should_stop() )
    {
        for ( i =0; i < MAX_RELEASE_NO; i++)
        {
            if (pstWinRelFrame->pstNeedRelFrmNode[i])
            {
                if ( pstWinRelFrame->pstNeedRelFrmNode[i]->bStillFrame)
                {
                    Win_DestroyStillFrame(pstWinRelFrame->pstNeedRelFrmNode[i]);
                    pstWinRelFrame->pstNeedRelFrmNode[i] = HI_NULL;
                }
            }
        }

        pstWinRelFrame->enThreadEvent = EVENT_BUTT;

        wait_event_timeout(pstWinRelFrame->stWaitQueHead, (EVENT_RELEASE == pstWinRelFrame->enThreadEvent),HZ);
    }

    return HI_SUCCESS;
}

HI_S32 WinCreatReleaseFrameThread(HI_VOID )
{
    WIN_RELEASE_FRM_S* pstWinRelFrame = &stDispWindow.stWinRelFrame;

    memset(pstWinRelFrame,0,sizeof(WIN_RELEASE_FRM_S));
    pstWinRelFrame->hThread =
               kthread_create(WinReleaseFrameThreadProcess, (HI_VOID *)(&stDispWindow.stWinRelFrame), "HI_WIN_ReleaseFrameProcess");

    init_waitqueue_head( &(pstWinRelFrame->stWaitQueHead) );
    if (IS_ERR(pstWinRelFrame->hThread))
    {
        WIN_FATAL("Can not create thread.\n");
        return HI_ERR_VO_CREATE_ERR;
    }

    wake_up_process(pstWinRelFrame->hThread);
    return HI_SUCCESS;
}

HI_S32 WinDestroyReleaseFrameThread(HI_VOID )
{
    HI_S32 s32Ret;
    HI_S32 s32Times = 0;
    WIN_RELEASE_FRM_S* pstWinRelFrame = &stDispWindow.stWinRelFrame;

    /*reflush release buffer */
    pstWinRelFrame->enThreadEvent = EVENT_RELEASE;
    wake_up(&pstWinRelFrame->stWaitQueHead);

    for (s32Times = 0 ; s32Times < 10; s32Times++)
    {
         if (EVENT_BUTT == pstWinRelFrame->enThreadEvent)
         {
            pstWinRelFrame->enThreadEvent = EVENT_RELEASE;
            s32Ret = kthread_stop(pstWinRelFrame->hThread);

            if (s32Ret != HI_SUCCESS)
            {
                WIN_FATAL("Destory Thread Error.\n");
            }
            return HI_SUCCESS;
        }
         msleep(100);
    }

    return HI_FAILURE;
}

HI_S32 WIN_Init(HI_VOID)
{
    HI_BOOL bDispInitFlag;
    HI_S32 s32Ret;
    DISP_INTF_OPERATION_S stFunction = {0};

    if (WIN_DEVICE_STATE_CLOSE != s_s32WindowGlobalFlag)
    {
        WIN_INFO("VO has been inited!\n");
        return HI_SUCCESS;
    }

    DISP_GetInitFlag(&bDispInitFlag);
    if (HI_TRUE != bDispInitFlag)
    {
        WIN_ERROR("Display is not inited!\n");
        return HI_ERR_VO_DEPEND_DEVICE_NOT_READY;
    }

    if (HI_SUCCESS != BP_CreateBlackFrame())
    {
        WIN_ERROR("Create Black Frame failed!\n");
        return HI_ERR_VO_MALLOC_FAILED;
    }

    DISP_MEMSET(&stDispWindow, 0, sizeof(DISPLAY_WINDOW_S));
    VideoLayer_Init();

    (HI_VOID)DISP_HAL_GetOperation(&stFunction);
    if(HI_NULL == stFunction.PF_GetIsogenyMode)
    {
        WIN_ERROR("PF_GetIsogenyMode is NULL!\n");
        return HI_ERR_VO_NULL_PTR;
    }
    (HI_VOID)stFunction.PF_GetIsogenyMode(&stDispWindow.eIsogeny_mode);

    /*first, judge whether iso-geny or not.*/
    if (DISP_IsFollowed(HI_DRV_DISPLAY_0)
        || DISP_IsFollowed(HI_DRV_DISPLAY_1))
    {
        stDispWindow.bIsogeny = HI_TRUE;
    }
    else
    {
        stDispWindow.bIsogeny = HI_FALSE;
    }

    if ( HI_TRUE == WinCheckWhetherWbcIsogenyMode())
    {
        s32Ret = ISOGBNY_Init();
        if ( HI_SUCCESS != s32Ret )
        {
            WIN_ERROR("WIN ISOGBNY_Init failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }
    }

    DISP_MEMSET(&stVirWindow, 0, sizeof(VIRTUAL_WINDOW_S));
    s32Ret = WinCreatReleaseFrameThread();
    if (HI_SUCCESS != s32Ret)
    {
        WIN_ERROR("win Create Release Frame Thread failed!\n");
        return HI_ERR_VO_MALLOC_FAILED;
    }
    ResetWinDoviRefInfo();
    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_OPEN;

    return HI_SUCCESS;
}

HI_S32 WIN_DeInit(HI_VOID)
{
    HI_S32 i,j,s32Ret;

    if (WIN_DEVICE_STATE_CLOSE == s_s32WindowGlobalFlag)
    {
        WIN_INFO("VO is not inited!\n");
        return HI_SUCCESS;
    }

    /*close all the windows, since slave channel is not visible to  upper level
        * all the things done by lower driver.
        */
    for(i = HI_DRV_DISPLAY_1; i < HI_DRV_DISPLAY_BUTT; i++)
    {
        stDispWindow.u32WinNumber[i] = 0;

        for(j = 0; j < WINDOW_MAX_NUMBER; j++)
        {
            if (stDispWindow.pstWinArray[i][j])
            {
                (HI_VOID)WIN_Destroy(stDispWindow.pstWinArray[i][j]->u32Index);
                stDispWindow.pstWinArray[i][j] = HI_NULL;
            }
        }
    }

    /*close all the virtual windows.*/
    for(i = 0; i < WIN_VIRTAUL_MAX_NUMBER; i++)
    {
        if (stVirWindow.pstWinArray[i])
        {
            WIN_VIR_Destroy(stVirWindow.pstWinArray[i]);
            stVirWindow.pstWinArray[i] = HI_NULL;
        }
    }

    stVirWindow.u32WinNumber = 0;

    if (WinGetRegWinManageStatus())
    {
        WinUnRegWinManageCallback(HI_DRV_DISPLAY_1);
        WinUnRegWinManageCallback(HI_DRV_DISPLAY_0);
        WinSetRegWinManageStatus(HI_FALSE);
    }

    s32Ret = WinDestroyReleaseFrameThread();
    if (HI_SUCCESS != s32Ret)
    {
        WIN_ERROR("win Destroy Release Frame Thread failed!\n");
    }


    if ( HI_TRUE == WinCheckWhetherWbcIsogenyMode())
    {
        s32Ret = ISOGBNY_DeInit();
        if ( HI_SUCCESS != s32Ret )
        {
            WIN_ERROR("WIN ISOGBNY_DeInit failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }
    }

    ResetWinDoviRefInfo();
    VideoLayer_DeInit();
    BP_DestroyBlackFrame();
    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_CLOSE;

    WIN_INFO("VO has been DEinited!\n");
    return HI_SUCCESS;
}

WINDOW_S *WinGetWindow(HI_U32 u32WinIndex);

HI_S32 WIN_Suspend(HI_VOID)
{
    WinCheckDeviceOpen();

    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_SUSPEND;

    return HI_SUCCESS;
}

HI_S32 WIN_Resume(HI_VOID)
{
    if (s_s32WindowGlobalFlag == WIN_DEVICE_STATE_SUSPEND)
    {
        VIDEO_LAYER_FUNCTIONG_S *pF = VideoLayer_GetFunctionPtr();

        s_s32WindowGlobalFlag = WIN_DEVICE_STATE_OPEN;
        if (pF)
            pF->PF_SetAllLayerDefault();
    }

    return HI_SUCCESS;
}


HI_BOOL WinCheckHdrMode(HI_VOID)
{
    HI_S32 s32Index = 0;
    HI_U32 u32HDWinNum = 0;
    HI_BOOL bExistHdrMode = HI_FALSE;
    WINDOW_S* pstWin = HI_NULL;

    u32HDWinNum = WinGetHDWinNum();

    if (u32HDWinNum != 1)
    {
        bExistHdrMode = HI_FALSE;
    }
    else
    {
        for (s32Index = 0; s32Index < WINDOW_MAX_NUMBER; s32Index++)
        {
            pstWin = stDispWindow.pstWinArray[HI_DRV_DISPLAY_1][s32Index];
            if ((HI_NULL != pstWin)
                && ((HI_DRV_WINDOW_MODE_DOLBY == pstWin->enWinMode)
                    ||(HI_DRV_WINDOW_MODE_HDR == pstWin->enWinMode)))
            {
                bExistHdrMode = HI_TRUE;
                break;
            }
        }
    }
    return bExistHdrMode;
}

HI_S32 WinTestAddWindow(HI_DRV_DISPLAY_E  enDisp)
{
    HI_U32 u32WinNumber = 0;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        WIN_ERROR("Invalid Disp = 0x%x\n", enDisp);
        return HI_ERR_VO_INVALID_PARA;
    }
    if (WinCheckHdrMode())
    {
        HI_ERR_WIN("Exist HDR mode window!Can not create new window anymore!\n");
        return HI_ERR_VO_CREATE_ERR;
    }

#ifdef HI_GFX_USE_G3

    /*DTS2013110807439
        3716cv200 :g3 and v4 can not be used at the same time.
        Attach mode : GFX DISP0 channel is writeback form DISP1,so  G3 is not use.
        Detach mode : GFX DISP0  G3 is  used.
        */
    if (!DISP_IsSameSource(enDisp) && HI_DRV_DISPLAY_0 == enDisp)
    {
        if (stDispWindow.u32WinNumber[HI_DRV_DISPLAY_0] >= 1)
            return HI_ERR_VO_WIN_UNSUPPORT;
    }
#endif

    if (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
    {
        u32WinNumber = (enDisp == HI_DRV_DISPLAY_1)? WINDOW_MAX_NUMBER:1;
    }
    else
    {
        u32WinNumber = WINDOW_MAX_NUMBER;
    }

    if ( stDispWindow.u32WinNumber[enDisp] < u32WinNumber)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_VO_INVALID_OPT;
    }
}

HI_U32 WinGetPrefix(HI_U32 u32WinIndex)
{
    return (HI_U32)(u32WinIndex & WIN_INDEX_PREFIX_MASK);
}

HI_U32 WinGetDispId(HI_U32 u32WinIndex)
{
    return (HI_U32)((u32WinIndex >> WIN_INDEX_DISPID_SHIFT_NUMBER) & WIN_INDEX_DISPID_MASK);
}

HI_U32 WinGetId(HI_U32 u32WinIndex)
{
    return (HI_U32)(u32WinIndex & WINDOW_INDEX_NUMBER_MASK);
}

HI_U32 WinMakeVirIndex(HI_U32 u32WinIndex)
{
    /*
     *    0x12               34                         56                78
     *         WIN_INDEX_PREFIX  WIN_INDEX_VIRTUAL_CHANNEL   u32WinIndex
     *
     */
    return (HI_U32)(   WIN_INDEX_PREFIX
                     | ( ( WIN_INDEX_VIRTUAL_CHANNEL& WIN_INDEX_DISPID_MASK) \
                          << WIN_INDEX_DISPID_SHIFT_NUMBER
                       )
                     |(u32WinIndex& WINDOW_INDEX_NUMBER_MASK)
                    );
}

HI_U32 WinMakeIndex(HI_DRV_DISPLAY_E enDisp, HI_U32 u32WinIndex)
{
    return (HI_U32)(   (WIN_INDEX_PREFIX)
                     | ( ( (HI_U32)enDisp & WIN_INDEX_DISPID_MASK) \
                          << WIN_INDEX_DISPID_SHIFT_NUMBER
                       )
                     |(u32WinIndex& WINDOW_INDEX_NUMBER_MASK)
                    );
}

HI_U32 WinGetIndex(HI_HANDLE hWin, HI_DRV_DISPLAY_E *enDisp, HI_U32 *u32WinIndex)
{

    *enDisp = (hWin & 0xff00) >> WIN_INDEX_DISPID_SHIFT_NUMBER;
    *u32WinIndex = hWin & 0xff;

    return HI_SUCCESS;
}

HI_S32 WinAddVirWindow(VIRTUAL_S *pstWin)
{
    HI_S32 i;

    for(i=0; i<WIN_VIRTAUL_MAX_NUMBER; i++)
    {
        if (!stVirWindow.pstWinArray[i])
        {
            pstWin->u32Index =    WinMakeVirIndex((HI_U32)i);
            stVirWindow.pstWinArray[i] = pstWin;
            stVirWindow.u32WinNumber++;

            return HI_SUCCESS;
        }
    }
    return HI_ERR_VO_CREATE_ERR;
}
HI_S32 WinDelVirWindow(HI_U32 u32WinIndex)
{
    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if ( WinGetDispId(u32WinIndex) != WIN_INDEX_VIRTUAL_CHANNEL)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if ( WinGetId(u32WinIndex) >= WIN_VIRTAUL_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if (!stVirWindow.u32WinNumber)
    {
        WIN_ERROR("Not found this window!\n");
        return HI_FAILURE;
    }

    if (stVirWindow.pstWinArray[WinGetId(u32WinIndex)])
    {
        stVirWindow.pstWinArray[WinGetId(u32WinIndex)] = HI_NULL;
        stVirWindow.u32WinNumber--;
    }
    else
    {
        WIN_ERROR("Not found this window!\n");
    }

    return HI_SUCCESS;
}


HI_S32 WinAddWindow(HI_DRV_DISPLAY_E enDisp, WINDOW_S *pstWin)
{
    HI_S32 i;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        return HI_ERR_VO_CREATE_ERR;
    }

    for(i=0; i<WINDOW_MAX_NUMBER; i++)
    {
        if (!stDispWindow.pstWinArray[(HI_U32)enDisp][i])
        {
            pstWin->u32Index =    WinMakeIndex(enDisp, (HI_U32)i);

            stDispWindow.pstWinArray[(HI_U32)enDisp][i] = pstWin;
            stDispWindow.u32WinNumber[(HI_U32)enDisp]++;

            //update HD win num for Dovi hdr
            UpdateHDWinNum(WinGetHDWinNum());
            return HI_SUCCESS;
        }
    }

    return HI_ERR_VO_CREATE_ERR;
}



HI_S32 WinDelWindow(HI_U32 u32WinIndex)
{
    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if (  WinGetDispId(u32WinIndex) >= HI_DRV_DISPLAY_BUTT)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if ( WinGetId(u32WinIndex) >= WINDOW_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if (!stDispWindow.u32WinNumber[WinGetDispId(u32WinIndex)])
    {
        WIN_ERROR("Not found this window!\n");
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if (stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)])
    {
        stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)] = HI_NULL;
        stDispWindow.u32WinNumber[WinGetDispId(u32WinIndex)]--;
        //update HD win num for Dovi hdr
        UpdateHDWinNum(WinGetHDWinNum());
    }
    else
    {
        WIN_ERROR("Not found this window!\n");
    }

    return HI_SUCCESS;
}

WINDOW_S *WinGetWindow(HI_U32 u32WinIndex)
{
    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if (  WinGetDispId(u32WinIndex) >= HI_DRV_DISPLAY_BUTT)
    {
        WIN_WARN("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if (!stDispWindow.u32WinNumber[WinGetDispId(u32WinIndex)])
    {
        WIN_WARN("Not found this window!\n");
        return HI_NULL;
    }
    if ( WinGetId(u32WinIndex) >= WINDOW_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    return stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)];
}



/******************************************************************************
    internal function
******************************************************************************/
HI_U32 WinParamAlignUp(HI_U32 x, HI_U32 a)
{
    if (!a)
    {
        return x;
    }
    else
    {
        return ( (( x + (a-1) ) / a ) * a);
    }
}

HI_U32 WinParamAlignDown(HI_U32 x, HI_U32 a)
{
    if (!a)
    {
        return x;
    }
    else
    {
        return (( x / a ) * a);
    }
}

HI_VOID window_revise(HI_RECT_S *pstToBeRevisedRect_tmp,
                     const HI_RECT_S *ptmp_virtscreen)
{
    /*give a basic    process of width and height.*/
    if (pstToBeRevisedRect_tmp->s32Width < WIN_OUTRECT_MIN_WIDTH)
    {
        pstToBeRevisedRect_tmp->s32Width = WIN_OUTRECT_MIN_WIDTH;
    }

    /*here we delete the width max branch, because we support tc 2-screen,
        *even 3-screen display.
        * here we do not make a limit,  all the behavior depends on the zme performance.
        */

    if (pstToBeRevisedRect_tmp->s32Height < WIN_OUTRECT_MIN_HEIGHT)
    {
        pstToBeRevisedRect_tmp->s32Height = WIN_OUTRECT_MIN_HEIGHT;
    }

    return ;
}

/*previously, we do division first, then do multiplication
 *so we lose precision. now we invert the  order.
 */
HI_S32 Win_Caculate(HI_S32 ratioA, HI_S32 ratioB,HI_S32 virtualValue)
{
    return ((virtualValue * ratioA) * 100) / ratioB;
}

/*this func is for both graphics and video    virtual screen deal, it's a common function.*/
HI_S32 WinOutRectSizeConversion(const HI_RECT_S *pstCanvas,
                         const HI_DRV_DISP_OFFSET_S *pstOffsetInfo,
                         const HI_RECT_S *pstFmtResolution,
                         HI_RECT_S *pstToBeRevisedRect,
                         HI_RECT_S *pstRevisedRect)
{
    HI_U32 zmeDestWidth = 0, zmeDestHeight = 0;
    HI_DRV_DISP_OFFSET_S tmp_offsetInfo;
    HI_RECT_S stToBeRevisedRect_tmp = *pstToBeRevisedRect;

    tmp_offsetInfo = *pstOffsetInfo;

    /*for browse mode, revise in virt screen .*/
    window_revise(&stToBeRevisedRect_tmp,pstCanvas);

    zmeDestWidth = (pstFmtResolution->s32Width - tmp_offsetInfo.u32Left - tmp_offsetInfo.u32Right);
    zmeDestHeight = (pstFmtResolution->s32Height - tmp_offsetInfo.u32Top - tmp_offsetInfo.u32Bottom);

    if (pstCanvas->s32Width != stToBeRevisedRect_tmp.s32Width)
    {
        pstRevisedRect->s32Width = Win_Caculate(zmeDestWidth,
                                               pstCanvas->s32Width,
                                               stToBeRevisedRect_tmp.s32Width) / 100;
    } else {
        pstRevisedRect->s32Width = zmeDestWidth;
    }

    if (pstCanvas->s32Height != stToBeRevisedRect_tmp.s32Height)
    {
        pstRevisedRect->s32Height = Win_Caculate(zmeDestHeight,
                                                pstCanvas->s32Height,
                                                stToBeRevisedRect_tmp.s32Height)/ 100;
    } else {
        pstRevisedRect->s32Height = zmeDestHeight;
    }


    pstRevisedRect->s32X =Win_Caculate(zmeDestWidth,
                                      pstCanvas->s32Width,
                                      stToBeRevisedRect_tmp.s32X) /100 + tmp_offsetInfo.u32Left;
    pstRevisedRect->s32Y= Win_Caculate(zmeDestHeight,
                                      pstCanvas->s32Height,
                                      stToBeRevisedRect_tmp.s32Y) /100 + tmp_offsetInfo.u32Top;

    pstRevisedRect->s32X = WinParamAlignUp(pstRevisedRect->s32X, 2);
    pstRevisedRect->s32Y = WinParamAlignUp(pstRevisedRect->s32Y, 2);

    //here need to align down to avoid out of screen.
    pstRevisedRect->s32Width  = WinParamAlignDown(pstRevisedRect->s32Width, 4);
    pstRevisedRect->s32Height = WinParamAlignDown(pstRevisedRect->s32Height, 4);

    /*for browse mode, revise in virt screen .*/
    window_revise(pstRevisedRect,pstFmtResolution);
    return HI_SUCCESS;
}

HI_S32 WinOutRectSizeConversionByType(const HI_DISP_DISPLAY_INFO_S  *pstInfo,
                                                     HI_RECT_S *pstRevisedRect,
                                                     WINDOW_S *pstWin)
{
    HI_RECT_S stCanvas, stOutRectOrigin;
    HI_DRV_DISP_OFFSET_S stOffsetInfo;

    memset((void*)&stCanvas, 0, sizeof(HI_RECT_S));
    memset((void*)&stOffsetInfo, 0, sizeof(HI_DRV_DISP_OFFSET_S));

    stOutRectOrigin = pstWin->stCfg.stAttrBuf.stOutRect;

    if (!stOutRectOrigin.s32Width || !stOutRectOrigin.s32Height)
    {
        if (pstWin->bVirtScreenMode)
            stOutRectOrigin = pstInfo->stVirtaulScreen;
        else
            stOutRectOrigin = pstInfo->stFmtResolution;
    }

    if (pstWin->bVirtScreenMode)
    {
        (HI_VOID)DISP_GetVirtScreen(pstWin->enDisp, &stCanvas);
        stOffsetInfo = pstInfo->stOffsetInfo;

#ifndef HI_VO_OFFSET_EFFECTIVE_WHEN_WIN_FULL
        if (   ((pstWin->stCfg.stAttrBuf.stOutRect.s32Width == 0)
                    || (pstWin->stCfg.stAttrBuf.stOutRect.s32Height== 0))
            || ((pstWin->stCfg.stAttrBuf.stOutRect.s32Width == stCanvas.s32Width)
                    &&(pstWin->stCfg.stAttrBuf.stOutRect.s32Height == stCanvas.s32Height))
           )
        {
            memset((void*)&stOffsetInfo, 0, sizeof(HI_DRV_DISP_OFFSET_S));
        }
#endif
    }
    else
    {
        stOffsetInfo.u32Left = 0;
        stOffsetInfo.u32Top = 0;
        stOffsetInfo.u32Right = 0;
        stOffsetInfo.u32Bottom = 0;
        stCanvas = pstWin->stDispInfo.stWinCurrentFmt;
    }

    (HI_VOID)WinOutRectSizeConversion(&stCanvas,
                              &stOffsetInfo,
                              &pstInfo->stFmtResolution,
                              &stOutRectOrigin,
                              pstRevisedRect);

    return HI_SUCCESS;
}


HI_S32 WindowRedistributeProcess(WINDOW_S *tmpWindow);
HI_S32 WindowRedistributeProcess_Wait(WINDOW_S *tmpWindow, HI_BOOL bWait);

#if 0
static HI_S32 WinZorderAdd(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0, i = 0;

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]) {
            i++;
        }
    }

    pstWin->u32Zorder = i;
    return WindowRedistributeProcess(pstWin);
}
#endif


static HI_VOID WinZorderSave(HI_U32 *pU32Array, HI_DRV_DISPLAY_E enDisp)
{
    HI_U32 u32Cnt = 0, i = 0;

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if (stDispWindow.pstWinArray[enDisp][u32Cnt]) {
            i++;
        }
    }

    for (u32Cnt = 0; u32Cnt < i; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[enDisp][u32Cnt]))
        {
            pU32Array[u32Cnt] = stDispWindow.pstWinArray[enDisp][u32Cnt]->u32Zorder;
        }
    }

    return ;
}

static HI_VOID WinZorderRestore(HI_U32 *pU32Array, HI_DRV_DISPLAY_E enDisp)
{
    HI_U32 u32Cnt = 0, i = 0;

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if (stDispWindow.pstWinArray[enDisp][u32Cnt]) {
            i++;
        }
    }

    for (u32Cnt = 0; u32Cnt < i; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[enDisp][u32Cnt]))
        {
            stDispWindow.pstWinArray[enDisp][u32Cnt]->u32Zorder = pU32Array[u32Cnt];
        }
    }

    return;
}
static HI_S32 WinZorderDelete(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0;

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt])
            && (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder > pstWin->u32Zorder))
        {
            stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder --;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 WinZorderMoveTop(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0, i = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32ZorderBak[WINDOW_MAX_NUMBER] = {0};
    HI_SIZE_T irqflag = 0;

    WinZorderSave(u32ZorderBak, pstWin->enDisp);

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]) {
            i++;
        }
    }

    /*for existing window's move top.*/
    if (pstWin->u32Index)
    {
        for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
        {
            if ((stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt])
                && (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder > pstWin->u32Zorder))
            {
                stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder --;
            }
        }

        pstWin->u32Zorder = i - 1;
    }
    else
    {
        /*for new window's move top.*/
        pstWin->u32Zorder = i;
    }

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    ret = WindowRedistributeProcess(pstWin);
    if (ret)
    {
        WinZorderRestore(u32ZorderBak, pstWin->enDisp);
    }

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    (HI_VOID)WindowRedistributeProcess_Wait(pstWin,HI_FALSE);

    return ret;
}

static HI_S32 WinZorderMoveBottom(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0, i = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32ZorderBak[WINDOW_MAX_NUMBER] = {0};
    HI_SIZE_T irqflag = 0;

    WinZorderSave(u32ZorderBak, pstWin->enDisp);
    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]) {
            i++;
        }
    }

    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt])
            && (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder < pstWin->u32Zorder))
        {
            stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder ++;
        }
    }

    pstWin->u32Zorder = 0;

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    ret =  WindowRedistributeProcess(pstWin);
    if (ret)
        WinZorderRestore(u32ZorderBak, pstWin->enDisp);

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    (HI_VOID)WindowRedistributeProcess_Wait(pstWin, HI_FALSE);




    return ret;
}

static HI_S32 WinZorderMoveUp(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32ZorderBak[WINDOW_MAX_NUMBER] = {0};
    HI_SIZE_T irqflag = 0;

    WinZorderSave(u32ZorderBak, pstWin->enDisp);
    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt])
            && (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder == (pstWin->u32Zorder + 1)))
        {
            pstWin->u32Zorder ++;
            stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder --;
            break;
        }
    }

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);

    ret =  WindowRedistributeProcess(pstWin);
    if (ret)
        WinZorderRestore(u32ZorderBak, pstWin->enDisp);

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    (HI_VOID)WindowRedistributeProcess_Wait(pstWin, HI_FALSE);

    return ret;
}

static HI_S32 WinZorderMoveDown(WINDOW_S *pstWin)
{
    HI_U32 u32Cnt = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32ZorderBak[WINDOW_MAX_NUMBER] = {0};
    HI_SIZE_T irqflag = 0;

    if (pstWin->u32Zorder == 0)
        return HI_SUCCESS;

    WinZorderSave(u32ZorderBak, pstWin->enDisp);
    for (u32Cnt = 0; u32Cnt < WINDOW_MAX_NUMBER; u32Cnt++)
    {
        if ((stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt])
            && (stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder == (pstWin->u32Zorder - 1)))
        {
            pstWin->u32Zorder --;
            stDispWindow.pstWinArray[pstWin->enDisp][u32Cnt]->u32Zorder ++;
            break;
        }
    }

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    ret = WindowRedistributeProcess(pstWin);
    if (ret)
        WinZorderRestore(u32ZorderBak, pstWin->enDisp);

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    (HI_VOID)WindowRedistributeProcess_Wait(pstWin, HI_FALSE);


    return ret;
}


/*check whether the layout is valid.*/
HI_VOID WinSetLocationInfor(WINDOW_S **pstWin,
                           WindowInfor_S *pstWinInfo,
                           HI_U32 numofWindow)
{
    HI_U32 i = 0;
    HI_RECT_S stCanvas, stTmpRect;
    HI_DISP_DISPLAY_INFO_S stDispInfo;

    memset(&stCanvas,0x0,sizeof(HI_RECT_S));
    memset(&stTmpRect,0x0,sizeof(HI_RECT_S));
    memset(&stDispInfo,0x0,sizeof(HI_DISP_DISPLAY_INFO_S));

    for (i = 0; i < numofWindow; i++)
    {
        stCanvas = pstWin[i]->stDispInfo.stWinCurrentFmt;

        (HI_VOID)DISP_GetDisplayInfo(pstWin[i]->enDisp, &stDispInfo);
        WinOutRectSizeConversionByType(&stDispInfo, &stTmpRect, pstWin[i]);

        /*if the window 's outrect is zero, just set it to vscreen size.*/
        if (!stTmpRect.s32Width || !stTmpRect.s32Height)
        {
            stTmpRect.s32Width  =  stCanvas.s32Width;
            stTmpRect.s32Height =  stCanvas.s32Height;
        }

        pstWinInfo[i].x_start = stTmpRect.s32X;

        /*if window is outof screen, just pull it in.*/
        if (pstWinInfo[i].x_start < 0)
        {
            pstWinInfo[i].x_start = 0;
        }
        else if (pstWinInfo[i].x_start >  stCanvas.s32Width)
        {
            pstWinInfo[i].x_start = stCanvas.s32Width;
        }

        /*compare x_end.*/
        pstWinInfo[i].x_end = stTmpRect.s32X +  stTmpRect.s32Width;
        if (pstWinInfo[i].x_end < 0)
        {
            pstWinInfo[i].x_end = 0;
        }
        else if (pstWinInfo[i].x_end > stCanvas.s32Width)
        {
            pstWinInfo[i].x_end = stCanvas.s32Width;
        }

        /*compare y_start*/
        pstWinInfo[i].y_start = stTmpRect.s32Y;

        if (pstWinInfo[i].y_start < 0)
        {
            pstWinInfo[i].y_start = 0;
        }
        else if (pstWinInfo[i].y_start >  stCanvas.s32Height)
        {
            pstWinInfo[i].y_start = stCanvas.s32Height;
        }

        /*compare y_end.*/
        pstWinInfo[i].y_end = stTmpRect.s32Y  +  stTmpRect.s32Height;

        if (pstWinInfo[i].y_end < 0)
        {
            pstWinInfo[i].y_end = 0;
        }
        else if (pstWinInfo[i].y_end > stCanvas.s32Height)
        {
            pstWinInfo[i].y_end = stCanvas.s32Height;
        }

        /*set the window zorder.*/
        pstWinInfo[i].zorder = pstWin[i]->u32Zorder;
    }

    return ;
}

HI_VOID Win_GetAllEnabledWindows(WINDOW_S **ppstWin, HI_U32 *WinNum, WINDOW_S *tmpWindow)
{
    HI_U32 k = 0, i = 0;

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[tmpWindow->enDisp][i])
        {
            if ((stDispWindow.pstWinArray[tmpWindow->enDisp][i]->u32Index
                            != tmpWindow->u32Index)
                    && (stDispWindow.pstWinArray[tmpWindow->enDisp][i]->bEnable == HI_TRUE))
            {
                ppstWin[k] = stDispWindow.pstWinArray[tmpWindow->enDisp][i];
                k++;
            }
        }
    }

   *WinNum = k;
    return ;
}

HI_VOID Win_InsertNodeIntoEnabledWindows(WINDOW_S **ppstWin, HI_U32 *WinNum, WINDOW_S *tmpWindow)
{
    HI_U32 i = 0;

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[tmpWindow->enDisp][i])
        {
            /*select enabled window which does not equal to tmpWindow
                     and insert them to the window ptr array.*/
            if ((stDispWindow.pstWinArray[tmpWindow->enDisp][i]->u32Index
                            == tmpWindow->u32Index)
                    && (tmpWindow->bEnable == HI_TRUE))
            {
                ppstWin[*WinNum] = tmpWindow;
                *WinNum = *WinNum + 1;
            }
        }
    }

    return ;
}

/*update the new layer and region number to window.*/
HI_S32 WinUpdateMappingInfor(WINDOW_S *pstWin, WindowMappingResult_S *pstWinMapResult, HI_DRV_DISPLAY_E enDisp)
{
    HI_U32 i = 0;

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        if ((stDispWindow.pstWinArray[pstWin->enDisp][i])
            && (stDispWindow.pstWinArray[pstWin->enDisp][i]->u32Index
                            == pstWin->u32Index))
        {
             if (enDisp == HI_DRV_DISPLAY_1)
             {
                 stDispWindow.pstWinArray[pstWin->enDisp][i]->u32VideoLayerNew = pstWinMapResult->videoLayer;
             }
             else if (enDisp == HI_DRV_DISPLAY_0)
             {
                 /*the sd channel is v3/v4*/
                 stDispWindow.pstWinArray[pstWin->enDisp][i]->u32VideoLayerNew = pstWinMapResult->videoLayer + 3;
             }

             stDispWindow.pstWinArray[pstWin->enDisp][i]->u32VideoRegionNoNew = pstWinMapResult->regionNo;

        }
    }

    if (enDisp == HI_DRV_DISPLAY_1)
        pstWin->u32VideoLayerNew = pstWinMapResult->videoLayer;
    else if (enDisp == HI_DRV_DISPLAY_0)
        /*the sd channel is v3/v4*/
        pstWin->u32VideoLayerNew = pstWinMapResult->videoLayer + 3;

    pstWin->u32VideoRegionNoNew = pstWinMapResult->regionNo;
    return HI_SUCCESS;
}

/*window adjust, move/zme/create/destroy may cause the layout
 *change. because the  adjust in thread-suituation may be half complete,
 *and then interruptted by isr, so the change should be located in    isr func.
*/
HI_S32 WindowRedistributeProcess(WINDOW_S *tmpWindow)
{
    WINDOW_S *pstWin[WINDOW_MAX_NUMBER] = {HI_NULL}, *pstWinTmp = HI_NULL;
    WindowInfor_S winLocationInfor[WINDOW_MAX_NUMBER] = {{0}};
    WindowMappingResult_S winMappingResult[WINDOW_MAX_NUMBER] = {{0}};
    WindowZorderInfor_S  winZorderInfor[WINDOW_MAX_NUMBER] = {{0}};

    HI_U32    numofWindow = 0, i = 0, k = 0, overlapWinCnt = 0;
    HI_S32    ret = HI_SUCCESS;
    HI_DRV_DISPLAY_E enDisp  = HI_DRV_DISPLAY_BUTT;

    /*first,get all the enabled window ptrs.*/
    Win_GetAllEnabledWindows(pstWin, &numofWindow, tmpWindow);
    Win_InsertNodeIntoEnabledWindows(pstWin, &numofWindow, tmpWindow);

    if (numofWindow == 0)
    {
        DISP_WARN("No enabled window exist.!\n");
        goto _REDIS_SUCCESS;
    }

    /*Second,  set the coordinate and locations value from windows.*/
    WinSetLocationInfor(pstWin, winLocationInfor, numofWindow);

    /*third:  generate win-layer remapping.*/
    ret =  WindowAndLayerMapping(winLocationInfor, winMappingResult, numofWindow);
    if (ret)
    {
        WIN_ERROR("UI disobey the windows strict, the coordinate is as follows:!\n");
        for (i = 0; i < numofWindow; i++)
        {
            WIN_ERROR("win x:%d, y:%d, x_end:%d, y_end:%d, zorder %d !\n",
                winLocationInfor[i].x_start,
                winLocationInfor[i].y_start,
                winLocationInfor[i].x_end,
                winLocationInfor[i].y_end,
                winLocationInfor[i].zorder);
        }

        return HI_ERR_VO_OPERATION_DENIED;
    }

    /*update new window-layer mapping.*/
    enDisp = tmpWindow->enDisp;
    for (i = 0 ; i < numofWindow; i++)
    {
        WinUpdateMappingInfor(pstWin[i], &winMappingResult[i], enDisp);
    }

    for (i = 0; i < numofWindow; i++)
    {
        if (winMappingResult[i].u32OverlapCnt)
        {
            winZorderInfor[overlapWinCnt].layerNumber = pstWin[i]->u32VideoLayerNew;
            winZorderInfor[overlapWinCnt].winZorder =    pstWin[i]->u32Zorder;

            (HI_VOID)pstWin[i]->stVLayerFunc.PF_GetZorder(pstWin[i]->u32VideoLayerNew,
                                                &winZorderInfor[overlapWinCnt].layerZorder);


            overlapWinCnt ++;
        }
    }

    /*check wether the window's order matches the layer's order.*/
    ret = layerZorderMatch(winZorderInfor, overlapWinCnt);
    if (ret)
    {
        WIN_ERROR(" the windows zorder is not right, and can't allocate layers.!\n");
        for (i = 0; i < overlapWinCnt; i++)
        {
            WIN_ERROR("win zorder:%d, layerorder:%d, allocated layer:%d !\n",
                winZorderInfor[i].winZorder,
                winZorderInfor[i].layerZorder,
                winZorderInfor[i].layerNumber);
        }
        return HI_ERR_VO_OPERATION_DENIED;
    }

    for (i = 0; i < overlapWinCnt; i++)
    {
        if ((winZorderInfor[i].layerNumber == VDP_RM_LAYER_VID0)
            || (winZorderInfor[i].layerNumber == VDP_RM_LAYER_VID3))
        {
            k = i;
            break;
        }
    }

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        pstWinTmp  = stDispWindow.pstWinArray[tmpWindow->enDisp][i];

        if (pstWinTmp
            && ((pstWinTmp->u32VideoLayerNew == VDP_RM_LAYER_VID3)
            || (pstWinTmp->u32VideoLayerNew == VDP_RM_LAYER_VID0)))
        {
             pstWinTmp->stWinLayerOpt.layerOptType = LAYER_OPT_ZORDER_ADJUST;

            if (HI_DRV_DISP_ZORDER_BUTT == winZorderInfor[k].matchOperation)
            {
                pstWinTmp->stWinLayerOpt.bEffective = HI_FALSE;
            }
            else
            {
                pstWinTmp->stWinLayerOpt.bEffective = HI_TRUE;
            }

            pstWinTmp->stWinLayerOpt.enLayerOpt = winZorderInfor[k].matchOperation;
        }
    }

_REDIS_SUCCESS:
    /*this setting is  atomic, and update the flag, so isr func can do something.*/
    return HI_SUCCESS;
}

HI_S32 WindowRedistributeProcess_Wait(WINDOW_S *tmpWindow, HI_BOOL bWait)
{
    unsigned int t = 0;
    atomic_set(&stDispWindow.bWindowSytemUpdate[tmpWindow->enDisp], 1);

    if (HI_TRUE == bWait)
    {
        while(atomic_read(&stDispWindow.bWindowSytemUpdate[tmpWindow->enDisp]))
        {
            DISP_MSLEEP(5);
            t ++;

            if (t > 10)
            {
                WIN_WARN("wait for redistribtue end time out!\n");
                break;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinBufferReset(WIN_BUFFER_S *pstBuffer);
HI_VOID ISR_WinReleaseUSLFrame(WINDOW_S *pstWin);

HI_S32 WinCreateDisplayWindow(HI_DRV_WIN_ATTR_S *pWinAttr, WINDOW_S **ppstWin, HI_U32 u32BufNum)
{
    WINDOW_S *pstWin;
    HI_S32 nRet;

    if(WinTestAddWindow(pWinAttr->enDisp))
    {
        WIN_ERROR("Reach max window number,can not create!\n");
        return HI_ERR_VO_CREATE_ERR;
    }

    pstWin = (WINDOW_S *)DISP_MALLOC(sizeof(WINDOW_S));
    if (!pstWin)
    {
        WIN_ERROR("Malloc WINDOW_S failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    DISP_MEMSET(pstWin, 0, sizeof(WINDOW_S));

    /* attribute */
    pstWin->bEnable = HI_FALSE;
    pstWin->bEnable_Using = HI_FALSE;
    pstWin->bMasked = HI_FALSE;

    pstWin->enState = WIN_STATE_WORK;
    pstWin->bUpState      = HI_FALSE;

    pstWin->enDisp = pWinAttr->enDisp;
    pstWin->enType = HI_DRV_WIN_ACTIVE_SINGLE;

    /* pay attention: when virtualscreen,it is the use's settting.
      *if non-virt mode ,it is physical cooridinate.
      */
    pstWin->stCfg.stAttrBuf = *pWinAttr;
    pstWin->stCfg.stAttr    = *pWinAttr;

#if 0
    /* default Hisi-self hdr currently. */
    pstWin->stCfg.stAttrBuf.bHisiHdr = HI_TRUE;
#endif
    atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);

    pstWin->stCfg.enFrameCS = HI_DRV_CS_UNKNOWN;
    pstWin->stCfg.u32Fidelity = 0;
    pstWin->stCfg.enOutCS    = HI_DRV_CS_UNKNOWN;

    pstWin->stCfg.stSource.hSrc = HI_INVALID_HANDLE;
    pstWin->stCfg.stSource.hSecondSrc = HI_INVALID_HANDLE;
    pstWin->stCfg.stSource.pfAcqFrame = HI_NULL;
    pstWin->stCfg.stSource.pfRlsFrame = HI_NULL;
    pstWin->stCfg.stSource.pfSendWinInfo = HI_NULL;
    nRet = VideoLayer_GetFunction(&pstWin->stVLayerFunc);
    if (nRet)
    {
        WIN_ERROR("VideoLayer_GetFunction failed!\n");
        goto __ERR_GET_FUNC__;
    }

    pstWin->u32VideoLayer = VDP_LAYER_VID_BUTT;
    pstWin->u32VideoLayerNew = VDP_LAYER_VID_BUTT;

    pstWin->u32VideoRegionNo    = 0xffffffff;
    pstWin->u32VideoRegionNoNew = 0xffffffff;

    pstWin->u32LastInLowDelayIdx = 0xffffffff;
    pstWin->u32LastOutLowDelayIdx = 0xffffffff;

    /*Pay attention please: here  the  func of zorder adjust should be modified.
        * do not forget it.
        */
    WinZorderMoveTop(pstWin);

    nRet = WinBuf_Create(u32BufNum, WIN_BUF_MEM_SRC_SUPPLY, HI_NULL, &pstWin->stBuffer.stWinBP);
    if (nRet)
    {
        WIN_ERROR("Create buffer pool failed\n");
        goto __ERR_GET_FUNC__;
    }
    spin_lock_init(&(pstWin->stBuffer.stUlsLock));
    WinBufferReset(&pstWin->stBuffer);

    DRV_WIN_FRC_Create(&pstWin->hFrc);

    // initial reset
    pstWin->bReset = HI_FALSE;
    pstWin->bConfigedBlackFrame = HI_FALSE;

    // initial quickmode
    pstWin->bQuickMode = HI_FALSE;

    // initial stepmode flag
    pstWin->bStepMode = HI_FALSE;

    //init delay info
    pstWin->stDelayInfo.u32DispRate = 5000;
    pstWin->stDelayInfo.bTBMatch = HI_TRUE;
    pstWin->stDelayInfo.u32DisplayTime = 20;
    pstWin->bInInterrupt = HI_FALSE;

    //init rotation
    pstWin->enRotation = HI_DRV_ROT_ANGLE_0;
    //init flip
    pstWin->bVertFlip = HI_FALSE;
    pstWin->bHoriFlip = HI_FALSE;
    *ppstWin = pstWin;

    return HI_SUCCESS;


__ERR_GET_FUNC__:

    DISP_FREE(pstWin);

    return HI_ERR_VO_CREATE_ERR;
}

HI_S32 WinDestroyDisplayWindow(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame;

    /*when destroy window, clear the dolby vision information.*/
    ClearWinDoviRefInfo(pstWin->enWinMode);

    WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
    ISR_WinReleaseUSLFrame(pstWin);

    // flush frame in full buffer pool
    pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
    WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);

    // release current frame
    WinBuf_ForceReleaseFrame(&pstWin->stBuffer.stWinBP, pstFrame);

    // s1 derstoy buffer
    WinBuf_Destroy(&pstWin->stBuffer.stWinBP);

    /*pay attention please, here release layer is wrong.
        *when in v1, layer shoulde not be released.*/
    pstWin->stVLayerFunc.PF_ReleaseLayer(pstWin->u32VideoLayer);

    /*when delete the win, adjust the zorder.*/
    WinZorderDelete(pstWin);

    DRV_WIN_FRC_Destroy(pstWin->hFrc);

    DISP_FREE(pstWin);
    pstWin = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 WinRegCallback(WINDOW_S *pstWin)
{
    HI_DRV_DISP_CALLBACK_S stCB;

    HI_S32 nRet= HI_SUCCESS;

    /*only hd register win process.*/
    if (WinGetDispID(pstWin) == HI_DRV_DISPLAY_1)
    {
        stCB.hDst  = (HI_VOID*)pstWin;
        stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
        stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;

        nRet = DISP_RegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
        if (nRet)
        {
            WIN_ERROR("WIN register callback failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }


    }
    else
    {
        if (!WinCheckWhetherWbcIsogenyMode())
        {
            stCB.hDst  = (HI_VOID*)pstWin;
            stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
            stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;

            nRet = DISP_RegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
            if (nRet)
            {
                WIN_ERROR("WIN register callback failed in %s!\n", __FUNCTION__);
                return HI_ERR_VO_CREATE_ERR;
            }
        }

    }

    return HI_SUCCESS;
}

HI_S32 WinUnRegCallback(WINDOW_S *pstWin)
{
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet= HI_SUCCESS;

    if (WinGetDispID(pstWin) == HI_DRV_DISPLAY_1)
    {
        stCB.hDst  = (HI_VOID*)pstWin;
        stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
        stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
        nRet = DISP_UnRegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

        if ( nRet!= HI_SUCCESS)
        {
            WIN_ERROR("WIN unregister callback failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_DESTROY_ERR;
        }



    }
    else
    {
        if (!WinCheckWhetherWbcIsogenyMode())
        {
            stCB.hDst  = (HI_VOID*)pstWin;
            stCB.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
            stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
            nRet = DISP_UnRegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
        }

    }

    return HI_SUCCESS;
}


HI_VOID WinSetBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        pstWin->bConfigedBlackFrame = HI_TRUE;
    }
}

HI_VOID WinClearBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        pstWin->bConfigedBlackFrame = HI_FALSE;
    }
}

HI_BOOL WinTestBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        return pstWin->bConfigedBlackFrame;
    }
    return HI_FALSE;
}

HI_S32 WinCheckFixedAttr(HI_DRV_WIN_ATTR_S *pOldAttr, HI_DRV_WIN_ATTR_S *pNewAttr)
{
    if (  (pOldAttr->enDisp != pNewAttr->enDisp)
        ||(pOldAttr->bVirtual != pNewAttr->bVirtual)
        )
    {
        WIN_ERROR("the fixed attr is not right.!\n");
        return HI_ERR_VO_OPERATION_DENIED;
    }

    if (pOldAttr->bVirtual)
    {
        if (  (pOldAttr->bUserAllocBuffer != pNewAttr->bUserAllocBuffer)
            ||(pOldAttr->u32BufNumber != pNewAttr->u32BufNumber)
            )
        {
            WIN_ERROR("the fixed attr of virtual window error!\n");
            return HI_ERR_VO_OPERATION_DENIED;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinCheckAttr(HI_DRV_WIN_ATTR_S *pstAttr, HI_BOOL bVirtScreen)
{
    HI_RECT_S drv_resolution;
    HI_S32 ret = 0;
    HI_DISP_DISPLAY_INFO_S  stInfo;

    if (pstAttr->enDisp > HI_DRV_DISPLAY_1)
    {
        WIN_FATAL("WIN only support HI_DRV_DISPLAY_0!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    memset((void*)&drv_resolution, 0, sizeof(HI_RECT_S));
    memset((void*)&stInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));

    if (bVirtScreen)
    {
       ret = DISP_GetVirtScreen(pstAttr->enDisp, &drv_resolution);
    }
    else
    {
        ret = DISP_GetDisplayInfo(pstAttr->enDisp, &stInfo);
        drv_resolution = stInfo.stFmtResolution;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_WIN("Get Virtual SCREEN error!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    /*delete the window size limit.*/
#if 0
    if (pstAttr->bVirtual == HI_TRUE)
    {
        if (   (pstAttr->stOutRect.s32Height > WIN_VIRTUAL_OUTRECT_MAX_HEIGHT)
            || (pstAttr->stOutRect.s32Width  > WIN_VIRTUAL_OUTRECT_MAX_WIDTH))
        {
            HI_ERR_WIN("Virtual win outrect is larger max width and height!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }
    else
    {
        if (   (pstAttr->stOutRect.s32Height > drv_resolution.s32Height)
            || (pstAttr->stOutRect.s32Width > drv_resolution.s32Width))
        {
            HI_ERR_WIN("Win outrect is larger than virtual screen!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }
#endif

    if ( ((pstAttr->stOutRect.s32Height == 0)
              && (pstAttr->stOutRect.s32Width != 0))
         || ((pstAttr->stOutRect.s32Height != 0)
              && (pstAttr->stOutRect.s32Width == 0)))
    {
        HI_ERR_WIN("win outrect error, one of w/h is zero.!\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    else if ((!(pstAttr->stOutRect.s32Height | pstAttr->stOutRect.s32Width))
         && (pstAttr->stOutRect.s32X | pstAttr->stOutRect.s32Y))
    {
        HI_ERR_WIN("when w/h is zero, x/y should be zero too.!\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    else if((pstAttr->stOutRect.s32Height | pstAttr->stOutRect.s32Width)
        && ((pstAttr->stOutRect.s32Width   < WIN_OUTRECT_MIN_WIDTH)
            || (pstAttr->stOutRect.s32Height  < WIN_OUTRECT_MIN_HEIGHT))
            )
    {
        WIN_FATAL("The Min WIN OutRect supported is 64*64 !\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (   ( pstAttr->stCustmAR.u32ARw > (pstAttr->stCustmAR.u32ARh * WIN_MAX_ASPECT_RATIO) )
        || ( (pstAttr->stCustmAR.u32ARw * WIN_MAX_ASPECT_RATIO) < pstAttr->stCustmAR.u32ARh)
        )
    {
        HI_ERR_WIN("bUserDefAspectRatio  error!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if ( pstAttr->bUseCropRect)
    {
        if(    (pstAttr->stCropRect.u32TopOffset    > WIN_CROPRECT_MAX_OFFSET_TOP)
            || (pstAttr->stCropRect.u32LeftOffset    > WIN_CROPRECT_MAX_OFFSET_LEFT)
            || (pstAttr->stCropRect.u32BottomOffset > WIN_CROPRECT_MAX_OFFSET_BOTTOM)
            || (pstAttr->stCropRect.u32RightOffset    > WIN_CROPRECT_MAX_OFFSET_RIGHT)
           )
        {
            WIN_FATAL("WIN CropRec support less than 128!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }
    else
    {
        if ( !pstAttr->stInRect.s32Height || !pstAttr->stInRect.s32Width)
        {
            DISP_MEMSET(&pstAttr->stInRect, 0, sizeof(HI_RECT_S));
        }
        else if(   (pstAttr->stInRect.s32Width     < WIN_FRAME_MIN_WIDTH)
                || (pstAttr->stInRect.s32Height  < WIN_FRAME_MIN_HEIGHT)
                || (pstAttr->stInRect.s32Width     > WIN_FRAME_MAX_WIDTH)
                || (pstAttr->stInRect.s32Height  > WIN_FRAME_MAX_HEIGHT)
                )
        {
            WIN_FATAL("WIN InRect support 64*64 ~ 2560*1600!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
        else if( (pstAttr->stInRect.s32X  < 0) || (pstAttr->stInRect.s32Y  < 0))
        {
            WIN_FATAL("WIN InRect  invalid para!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }


    /* may change when window lives */
    pstAttr->stInRect.s32X = pstAttr->stInRect.s32X & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stInRect.s32Y = pstAttr->stInRect.s32Y & HI_WIN_IN_RECT_Y_ALIGN;
    pstAttr->stInRect.s32Width = pstAttr->stInRect.s32Width & HI_WIN_IN_RECT_WIDTH_ALIGN;
    pstAttr->stInRect.s32Height = pstAttr->stInRect.s32Height & HI_WIN_IN_RECT_HEIGHT_ALIGN;

    pstAttr->stCropRect.u32LeftOffset    = pstAttr->stCropRect.u32LeftOffset   & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stCropRect.u32RightOffset    = pstAttr->stCropRect.u32RightOffset  & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stCropRect.u32TopOffset    = pstAttr->stCropRect.u32TopOffset      & HI_WIN_IN_RECT_Y_ALIGN;
    pstAttr->stCropRect.u32BottomOffset = pstAttr->stCropRect.u32BottomOffset & HI_WIN_IN_RECT_Y_ALIGN;

    pstAttr->stOutRect.s32X = pstAttr->stOutRect.s32X & HI_WIN_OUT_RECT_X_ALIGN;
    pstAttr->stOutRect.s32Y = pstAttr->stOutRect.s32Y & HI_WIN_OUT_RECT_Y_ALIGN;
    pstAttr->stOutRect.s32Width  = pstAttr->stOutRect.s32Width    & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstAttr->stOutRect.s32Height = pstAttr->stOutRect.s32Height & HI_WIN_OUT_RECT_HEIGHT_ALIGN;

    return HI_SUCCESS;
}


HI_S32 WinCheckSourceInfo(HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    if (!pstSrc->pfAcqFrame && !pstSrc->pfRlsFrame && !pstSrc->pfSendWinInfo)
    {
        return HI_ERR_VO_NULL_PTR;
    }
    return HI_SUCCESS;
}

static HI_VOID Win_Calculate_New_DeviceAspectRatio(HI_DRV_ASPECT_RATIO_S *pstDeviceAspectRatio,
                                           HI_BOOL bHorSrEnable,
                                           HI_BOOL bVerSrEnable,
                                           HI_DRV_ASPECT_RATIO_S *pstNewDeviceAspectRatio)
{
    HI_U32 tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;

    if  (bHorSrEnable)
    {
        tmp1 = 2;
        tmp2 = 1;
    }
    else
    {
        tmp1 = 1;
        tmp2 = 1;
    }

    if  (bVerSrEnable)
    {
        tmp3 = 1;
        tmp4 = 2;
    }
    else
    {
        tmp3= 1;
        tmp4 = 1;
    }

    pstNewDeviceAspectRatio->u32ARw = pstDeviceAspectRatio->u32ARw * tmp1 * tmp3;
    pstNewDeviceAspectRatio->u32ARh = pstDeviceAspectRatio->u32ARh * tmp2 * tmp4;

    return;
}

HI_S32 Get3DOutRect(HI_DRV_DISP_STEREO_E en3DMode, HI_RECT_S *pstOutRect, HI_RECT_S *pstReviseOutRect)
{
    *pstReviseOutRect = *pstOutRect;
    if(en3DMode >= DISP_STEREO_BUTT)
    {
        WIN_ERROR("3d mode error!\n");
        return HI_FAILURE;
    }

    if (en3DMode == DISP_STEREO_NONE)
    {
        return HI_SUCCESS;
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
    else
    {
        WIN_ERROR("3d mode no support!\n");
    }

    return HI_SUCCESS;
}
static HI_S32 WinCalDobySourceInfo(WINDOW_S *pstWin, HI_U32 u32VideoLayer, HI_DISP_DISPLAY_INFO_S *pstDispInfo, HI_DRV_WIN_PRIV_INFO_S *pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    HI_DRV_WIN_ATTR_S*          pstAttr = HI_NULL;
    HI_DRV_ASPECT_RATIO_S       stScreenARTmp = {0};
    HI_DRV_VIDEO_FRAME_S *      pstElFrm = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S *    pstPrivInfo = HI_NULL;
    memset(&stVideoLayerCap,0x0,sizeof(VIDEO_LAYER_CAPABILITY_S));

    pstAttr = &pstWin->stUsingAttr;

    pstInfo->ePixFmt        = HI_DRV_PIX_FMT_NV21;
    pstInfo->bUseCropRect   = HI_FALSE;

    pstInfo->stInRect.s32Width = 0;
    pstInfo->stInRect.s32Height = 0;
    pstInfo->stInRect.s32X = 0;
    pstInfo->stInRect.s32Y = 0;

    pstInfo->stCropRect.u32LeftOffset = 0;
    pstInfo->stCropRect.u32TopOffset = 0;
    pstInfo->stCropRect.u32RightOffset = 0;
    pstInfo->stCropRect.u32BottomOffset = 0;


    if (!pstAttr->stOutRect.s32Width || !pstAttr->stOutRect.s32Height)
    {
        pstInfo->stOutRect = pstDispInfo->stFmtResolution;
    }
    else
    {
        pstInfo->stOutRect = pstAttr->stOutRect;
    }

    if (u32VideoLayer  == VDP_LAYER_VID1)
    {

        if ((pstDispInfo->eFmt == HI_DRV_DISP_FMT_3840X2160_50)
            || (pstDispInfo->eFmt == HI_DRV_DISP_FMT_3840X2160_60)
            || (pstDispInfo->eFmt == HI_DRV_DISP_FMT_4096X2160_50)
            || (pstDispInfo->eFmt == HI_DRV_DISP_FMT_4096X2160_60)
           )
        {

            /*when sr enable,but 1/2 size  larger than max, the vpss zme size should be calculated again.*/
            if ( (pstInfo->stOutRect.s32Height > 1080) ||  (pstInfo->stOutRect.s32Width > 1920))
            {
                pstInfo->stOutRect.s32Width = pstInfo->stOutRect.s32Width / 2;
                pstInfo->stOutRect.s32Height = pstInfo->stOutRect.s32Height / 2 ;

                pstInfo->stOutRect.s32Width &= HI_WIN_OUT_RECT_WIDTH_ALIGN;
                pstInfo->stOutRect.s32Height &= HI_WIN_OUT_RECT_HEIGHT_ALIGN;

            }
        }
        else
        {
            if (pstWin->stBuffer.stWinBP.pstDisplay)
            {
                pstElFrm = (HI_DRV_VIDEO_FRAME_S*)pstWin->stBuffer.stWinBP.pstDisplay->u32Data2;
                /* when EL frm is 1/2 size of outRect,let vpss output this size. */
                if (pstElFrm)
                {
                    pstPrivInfo = (HI_DRV_VIDEO_PRIVATE_S*)(pstElFrm->u32Priv);
                    if ( (pstPrivInfo->stVideoOriginalInfo.u32Width == pstInfo->stOutRect.s32Width / 2)
                         && (pstPrivInfo->stVideoOriginalInfo.u32Height == pstInfo->stOutRect.s32Height / 2))
                    {
                        pstInfo->stOutRect.s32Width = pstInfo->stOutRect.s32Width / 2;
                        pstInfo->stOutRect.s32Height = pstInfo->stOutRect.s32Height / 2 ;

                        pstInfo->stOutRect.s32Width &= HI_WIN_OUT_RECT_WIDTH_ALIGN;
                        pstInfo->stOutRect.s32Height &= HI_WIN_OUT_RECT_HEIGHT_ALIGN;
                    }
                }
            }
        }
    }

    pstInfo->stScreenAR   = pstDispInfo->stAR;
    pstInfo->stCustmAR    = pstAttr->stCustmAR;
    pstInfo->enARCvrs     = HI_DRV_ASP_RAT_MODE_FULL;
    pstInfo->bUseExtBuf   = pstAttr->bUserAllocBuffer;
    pstInfo->u32MaxRate   = (pstDispInfo->u32RefreshRate > WIN_TRANSFER_CODE_MAX_FRAME_RATE) ?
                            (pstDispInfo->u32RefreshRate / 2 ) : pstDispInfo->u32RefreshRate;

    pstInfo->bInterlaced = HI_TRUE;
    pstInfo->enRotation  = HI_DRV_ROT_ANGLE_0;
    pstInfo->bHoriFlip   = HI_FALSE;
    pstInfo->bVertFlip   = HI_FALSE;

    pstInfo->bIn3DMode = HI_FALSE;
    /*displaying window does not support tunnel mode.*/
    pstInfo->bTunnelSupport = HI_FALSE;


    if (!pstWin->stVLayerFunc.PF_GetCapability)
    {
        WIN_FATAL("PF_GetCapability is null\n");
        return HI_SUCCESS;//here should return failure NOT success!
    }

    if ((s32Ret = pstWin->stVLayerFunc.PF_GetCapability(u32VideoLayer, &stVideoLayerCap)))
    {
        return s32Ret;
    }

    /*in most condition, v1 will be set to mutilarea mode except in dolby mode,
      *under which v1 will be set single mode.  v1 only support 8 bit in multiarea
      * mode and 10 bit in singlearea mode.
      *  so when the layer support 10bit or the window in dolby mode , we acquire 10 bit from vpss.
      *  VPSS will give a result according the stream source.
      */
    if ((stVideoLayerCap.u32BitWidth == 10)
        || (pstWin->enWinMode == HI_DRV_WINDOW_MODE_DOLBY))
    {
        pstInfo->enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
    }
    else
    {
        pstInfo->enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    }


    pstInfo->stScreenAR = stScreenARTmp;
      /*as a result of winlayermapping, we switch vpss  compress on or off according
          the layer capability.*/
    if (stVideoLayerCap.bDcmp == HI_TRUE)
    {
        pstInfo->ePixFmt = HI_DRV_PIX_FMT_NV21_CMP;
    }
    else
    {
        pstInfo->ePixFmt = HI_DRV_PIX_FMT_NV21;
    }

    /*if the layer has zme,it can bypass vpss*/
    if (stVideoLayerCap.bZme == HI_TRUE)
    {
        pstInfo->bPassThrough = HI_TRUE;
    }
    else
    {
        pstInfo->bPassThrough = HI_FALSE;
    }

    DISP_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>will send info to source .............\n");

    //pro info below need to modify when it comes to Dolby frame. currently just mantain it.
    pstWin->stWinInfoForDeveloper.u32WinNum = stDispWindow.u32WinNumber[pstWin->enDisp];
    pstWin->stWinInfoForDeveloper.bIn3DMode = pstInfo->bIn3DMode;

    pstWin->stWinInfoForDeveloper.bHorSrOpenInPreProcess = HI_FALSE;
    pstWin->stWinInfoForDeveloper.bVerSrOpenInPreProcess = HI_FALSE;

    pstWin->stWinInfoForDeveloper.stVdpRequire = pstInfo->stOutRect;
    pstWin->stWinInfoForDeveloper.eCurrentFmt = pstDispInfo->stFmtResolution;
    pstWin->stWinInfoForDeveloper.stFinalWinOutputSize = pstInfo->stOutRect;


    return s32Ret;

}


static HI_S32 WinCalcuInfo(WINDOW_S *pstWin, HI_U32 u32VideoLayer, HI_DISP_DISPLAY_INFO_S *pstDispInfo, HI_DRV_WIN_PRIV_INFO_S *pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    HI_BOOL bHorSrEnable = HI_FALSE;
    HI_BOOL bVerSrEnable = HI_FALSE;
    HI_DRV_DISP_STEREO_E enStereo = pstDispInfo->eDispMode;

    HI_DRV_WIN_ATTR_S*          pstAttr;
    HI_DRV_ASPECT_RATIO_S       stScreenARTmp;

    pstAttr = &pstWin->stUsingAttr;

    pstInfo->ePixFmt        = HI_DRV_PIX_FMT_NV21;
    pstInfo->bUseCropRect   = pstAttr->bUseCropRect;
    pstInfo->stInRect       = pstAttr->stInRect;
    pstInfo->stCropRect     = pstAttr->stCropRect;

    if (!pstAttr->stOutRect.s32Width || !pstAttr->stOutRect.s32Height)
    {
        pstInfo->stOutRect = pstDispInfo->stFmtResolution;
    }
    else
    {
        pstInfo->stOutRect = pstAttr->stOutRect;
    }

    pstInfo->stScreenAR   = pstDispInfo->stAR;
    pstInfo->stCustmAR    = pstAttr->stCustmAR;
    pstInfo->enARCvrs     = pstAttr->enARCvrs;
    pstInfo->bUseExtBuf   = pstAttr->bUserAllocBuffer;
    pstInfo->u32MaxRate   = (pstDispInfo->u32RefreshRate > WIN_TRANSFER_CODE_MAX_FRAME_RATE) ?
                            (pstDispInfo->u32RefreshRate / 2 ) : pstDispInfo->u32RefreshRate;

    pstInfo->bInterlaced = pstDispInfo->bInterlace;
    pstInfo->enRotation  = pstWin->enRotation;
    pstInfo->bHoriFlip   = pstWin->bHoriFlip;
    pstInfo->bVertFlip   = pstWin->bVertFlip;

    // pstInfo->stScreen     = pstDispInfo->stFmtResolution;
    if ((pstDispInfo->eDispMode != DISP_STEREO_NONE)
        && (pstDispInfo->eDispMode < DISP_STEREO_BUTT))
    {
        pstInfo->bIn3DMode = HI_TRUE;
    }
    else
    {
        pstInfo->bIn3DMode = HI_FALSE;
    }

    /*displaying window does not support tunnel mode.*/
    pstInfo->bTunnelSupport = HI_FALSE;


    Get3DOutRect(pstDispInfo->eDispMode, &pstDispInfo->stFmtResolution, &pstInfo->stScreen);
    Get3DOutRect(pstDispInfo->eDispMode, &pstInfo->stOutRect, &pstInfo->stOutRect);
    pstWin->stWinInfoForDeveloper.stFinalWinOutputSize = pstInfo->stOutRect;


    if (!pstWin->stVLayerFunc.PF_GetCapability)
    {
        WIN_FATAL("PF_GetCapability is null\n");
        return HI_SUCCESS;//here should return failure NOT success!
    }

    if ((s32Ret = pstWin->stVLayerFunc.PF_GetCapability(u32VideoLayer, &stVideoLayerCap)))
    {
        return s32Ret;
    }

    if (pstWin->bMCE && (pstDispInfo->stFmtResolution.s32Width <= FHD_WIDTH_VALUE))
    {
        stVideoLayerCap.bZme = HI_FALSE;
    }

    /*  Window  process in VDP */
    if ((stVideoLayerCap.bZmePrior == HI_TRUE)
        && (stVideoLayerCap.bZme == HI_TRUE))
    {
        pstInfo->enARCvrs  = HI_DRV_ASP_RAT_MODE_FULL;

        pstInfo->bUseCropRect = HI_FALSE;

        DISP_MEMSET(&pstInfo->stCropRect, 0, sizeof(HI_DRV_CROP_RECT_S));
        DISP_MEMSET(&pstInfo->stInRect, 0, sizeof(HI_RECT_S));

    }
    else
    {
        pstInfo->enARCvrs  = pstAttr->enARCvrs;
    }


    /*in most condition, v1 will be set to mutilarea mode except in dolby mode,
      *under which v1 will be set single mode.  v1 only support 8 bit in multiarea
      * mode and 10 bit in singlearea mode.
      *  so when the layer support 10bit or the window in dolby mode , we acquire 10 bit from vpss.
      *  VPSS will give a result according the stream source.
      */

    if ((stVideoLayerCap.u32BitWidth == 10)
        || (pstWin->enWinMode == HI_DRV_WINDOW_MODE_DOLBY))
    {
        pstInfo->enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
    }
    else
    {
        pstInfo->enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    }

    /*If we need a 2-class zme, to avoid the distortion of content,
          we should let every zme do the same aspect ratio of zoom in or out.*/
    Win_Pre_ScalerDistribute(pstWin,
                             &pstInfo->stOutRect,
                             &pstInfo->stOutRect,
                             stDispWindow.u32WinNumber[pstWin->enDisp],
                             &bHorSrEnable,
                             &bVerSrEnable,
                             &pstDispInfo->stFmtResolution,
                             enStereo,
                             &stVideoLayerCap);

    Win_Calculate_New_DeviceAspectRatio(&pstInfo->stScreenAR, bHorSrEnable, bVerSrEnable, &stScreenARTmp);

    pstInfo->stScreenAR = stScreenARTmp;
    DISP_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>will send info to source .............\n");

    //pro info below need to modify when it comes to Dolby frame. currently just mantain it.
    pstWin->stWinInfoForDeveloper.u32WinNum = stDispWindow.u32WinNumber[pstWin->enDisp];
    pstWin->stWinInfoForDeveloper.bIn3DMode = pstInfo->bIn3DMode;

    pstWin->stWinInfoForDeveloper.bHorSrOpenInPreProcess = bHorSrEnable;
    pstWin->stWinInfoForDeveloper.bVerSrOpenInPreProcess = bVerSrEnable;

    pstWin->stWinInfoForDeveloper.stVdpRequire = pstInfo->stOutRect;
    pstWin->stWinInfoForDeveloper.eCurrentFmt = pstDispInfo->stFmtResolution;

    /*as a result of winlayermapping, we switch vpss  compress on or off according
          the layer capability.*/
    if (stVideoLayerCap.bDcmp == HI_TRUE)
    {
        pstInfo->ePixFmt = HI_DRV_PIX_FMT_NV21_CMP;
    }
    else
    {
        pstInfo->ePixFmt = HI_DRV_PIX_FMT_NV21;
    }

    if (stVideoLayerCap.bZme == HI_TRUE && pstInfo->enRotation == HI_DRV_ROT_ANGLE_0)
    {
        pstInfo->bPassThrough = HI_TRUE;
    }
    else
    {
        pstInfo->bPassThrough = HI_FALSE;
    }

    return s32Ret;

}

HI_S32 WinSendAttrToSource(WINDOW_S *pstWin, HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
    HI_DRV_WIN_PRIV_INFO_S      stInfo;
    PFN_GET_WIN_INFO_CALLBACK   pfTmpSendWinInfo = HI_NULL;

    DISP_MEMSET(&stInfo, 0, sizeof(HI_DRV_WIN_PRIV_INFO_S));

    if (HI_NULL == pstWin->stCfg.stSource.pfSendWinInfo)
    {
        return HI_FAILURE;
    }

    pfTmpSendWinInfo = pstWin->stCfg.stSource.pfSendWinInfo;

    /* hSrc valid and hSecondSrc invalid,there's ONLY one VPSS:normal frame or
        * (BL+metadata)Dolby frame will enter this branch.
        */
    if ((pstWin->stCfg.stSource.hSrc != HI_INVALID_HANDLE) &&
        (pstWin->stCfg.stSource.hSecondSrc == HI_INVALID_HANDLE))
    {
        WinCalcuInfo(pstWin, pstWin->u32VideoLayer, pstDispInfo, &stInfo);
        (pfTmpSendWinInfo)(pstWin->stCfg.stSource.hSrc, &stInfo);

    }

    /* Both hSrc and hSecondSrc are valid indicate there're two VPSSes:
        * BL+(EL+metadata)Dolby frame will enter this branch.
        */
    else if ((pstWin->stCfg.stSource.hSrc != HI_INVALID_HANDLE) &&
             (pstWin->stCfg.stSource.hSecondSrc != HI_INVALID_HANDLE))
    {
        //set the first VPSS output attributes.Calculate V0 videoLayer's info.
        WinCalDobySourceInfo(pstWin, VDP_LAYER_VID0, pstDispInfo, &stInfo);
        (pfTmpSendWinInfo)(pstWin->stCfg.stSource.hSrc, &stInfo);


        //printk("Bl->(%d,%d,%d,%d)",stInfo.stOutRect.s32X,stInfo.stOutRect.s32Y,stInfo.stOutRect.s32Width,stInfo.stOutRect.s32Height);
        //set the second VPSS output attributes.Calulate V1 videoLayer's info.
        DISP_MEMSET(&stInfo, 0, sizeof(HI_DRV_WIN_PRIV_INFO_S));
        WinCalDobySourceInfo(pstWin, VDP_LAYER_VID1, pstDispInfo, &stInfo);
       // printk("El->(%d,%d,%d,%d)\n",stInfo.stOutRect.s32X,stInfo.stOutRect.s32Y,stInfo.stOutRect.s32Width,stInfo.stOutRect.s32Height);

        (pfTmpSendWinInfo)(pstWin->stCfg.stSource.hSecondSrc, &stInfo);
    }
    else
    {
        HI_ERR_WIN("hSrc is invalid!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 WinCalcDispRectBaseRefandRel(HI_RECT_S *pRef, HI_RECT_S *pRel,
                                             HI_RECT_S *pI, HI_RECT_S *pO)
{
    pO->s32X      = (pI->s32X * pRel->s32Width) / pRef->s32Width;
    pO->s32Width  = (pI->s32Width * pRel->s32Width) / pRef->s32Width;
    pO->s32Y      = (pI->s32Y* pRel->s32Height) / pRef->s32Height;
    pO->s32Height = (pI->s32Height* pRel->s32Height) / pRef->s32Height;

    pO->s32X      = pO->s32X & HI_WIN_OUT_RECT_X_ALIGN;
    pO->s32Width  = pO->s32Width & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pO->s32Y      = pO->s32Y & HI_WIN_OUT_RECT_Y_ALIGN;
    pO->s32Height = pO->s32Height & HI_WIN_OUT_RECT_HEIGHT_ALIGN;

    if ( pO->s32Width && (pO->s32Width < WIN_OUTRECT_MIN_WIDTH))
    {
        pO->s32Width = WIN_OUTRECT_MIN_WIDTH;
    }

    if ( pO->s32Height && (pO->s32Height < WIN_OUTRECT_MIN_HEIGHT))
    {
        pO->s32Height = WIN_OUTRECT_MIN_HEIGHT;
    }

    return HI_SUCCESS;
}

HI_S32 WinCalcCropRectBaseRefandRel(HI_RECT_S *pRef, HI_RECT_S *pRel,
                                              HI_DRV_CROP_RECT_S *pI,
                                              HI_DRV_CROP_RECT_S *pO)
{
    pO->u32LeftOffset    = (pI->u32LeftOffset * pRel->s32Width) / pRef->s32Width;
    pO->u32RightOffset    = (pI->u32RightOffset * pRel->s32Width) / pRef->s32Width;
    pO->u32TopOffset    = (pI->u32TopOffset * pRel->s32Height) / pRef->s32Height;
    pO->u32BottomOffset = (pI->u32BottomOffset * pRel->s32Height) / pRef->s32Height;

    pO->u32LeftOffset    =  pO->u32LeftOffset   & HI_WIN_IN_RECT_X_ALIGN;
    pO->u32RightOffset    = pO->u32RightOffset  & HI_WIN_IN_RECT_X_ALIGN;
    pO->u32TopOffset    = pO->u32TopOffset      & HI_WIN_IN_RECT_Y_ALIGN;
    pO->u32BottomOffset = pO->u32BottomOffset & HI_WIN_IN_RECT_Y_ALIGN;

    return HI_SUCCESS;
}

HI_S32 WinGetSlaveWinAttr(HI_DRV_WIN_ATTR_S *pWinAttr,
                          HI_DRV_WIN_ATTR_S *pSlvWinAttr,
                          HI_BOOL bVirtScreen)
{
    HI_S32 nRet;
    HI_DISP_DISPLAY_INFO_S stDispInfo,stSlaveDispInfo;
    HI_RECT_S stCanvas;
    HI_RECT_S stMastOutRect;
    HI_DRV_DISP_OFFSET_S stOffsetInfo;

    DISP_MEMSET(pSlvWinAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));
    DISP_MEMSET((void*)&stDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));
    DISP_MEMSET((void*)&stSlaveDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));

    // s1 get slave display
    nRet = DISP_GetSlave(pWinAttr->enDisp, &pSlvWinAttr->enDisp);
    if (nRet)
    {
        WIN_ERROR("Get slave Display failed\n");
        return nRet;
    }

    if (!DISP_IsOpened(pSlvWinAttr->enDisp) )
    {
        WIN_ERROR("Slave Display is not open\n");
        return HI_ERR_DISP_NOT_OPEN;
    }

    // s2 get master and slave display info
    pSlvWinAttr->bVirtual = HI_FALSE;

    /* may change when window lives */
    pSlvWinAttr->stCustmAR = pWinAttr->stCustmAR;
    pSlvWinAttr->enARCvrs  = pWinAttr->enARCvrs;

    /*now sd's crop is done by hd(vpss cropped),
       so in isogeny-wbc mode, sd's inrect and croprect should  re-calculated.
      */
    pSlvWinAttr->bUseCropRect = pWinAttr->bUseCropRect;
    pSlvWinAttr->stInRect    = pWinAttr->stInRect;
    pSlvWinAttr->stCropRect = pWinAttr->stCropRect;

    if (!bVirtScreen)
    {
        (HI_VOID)DISP_GetDisplayInfo(pWinAttr->enDisp, &stDispInfo);
        (HI_VOID)DISP_GetDisplayInfo(pSlvWinAttr->enDisp, &stSlaveDispInfo);

        stCanvas = stDispInfo.stFmtResolution;
        stOffsetInfo.u32Left = 0;
        stOffsetInfo.u32Top  = 0;
        stOffsetInfo.u32Right = 0;
        stOffsetInfo.u32Bottom = 0;

        stMastOutRect = pWinAttr->stOutRect;
        if ((stMastOutRect.s32Width == 0) || (stMastOutRect.s32Height == 0))
        {
            stMastOutRect.s32Width  = stCanvas.s32Width;
            stMastOutRect.s32Height = stCanvas.s32Height;
        }

        (HI_VOID)WinOutRectSizeConversion(&stCanvas,
                                  &stOffsetInfo,
                                  &stSlaveDispInfo.stFmtResolution,
                                  &stMastOutRect,
                                  &pSlvWinAttr->stOutRect);
    }
    else
    {
        pSlvWinAttr->stOutRect    = pWinAttr->stOutRect;
    }

    return HI_SUCCESS;
}

HI_S32 WinGetSlaveWinAttr2(WINDOW_S *pstWin,
                                 HI_DRV_WIN_ATTR_S *pWinAttr,
                                 HI_DRV_WIN_ATTR_S *pSlvWinAttr)
{
    HI_DISP_DISPLAY_INFO_S stDispInfo,stSlaveDispInfo;
    HI_RECT_S stCanvas;
    HI_RECT_S stMastOutRect;
    HI_DRV_DISP_OFFSET_S stOffsetInfo;

    memset((void*)&stDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));
    memset((void*)&stSlaveDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));

    /* may change when window lives */
    pSlvWinAttr->stCustmAR = pWinAttr->stCustmAR;
    pSlvWinAttr->enARCvrs  = pWinAttr->enARCvrs;

    pSlvWinAttr->bUseCropRect = pWinAttr->bUseCropRect;
    pSlvWinAttr->stInRect    = pWinAttr->stInRect;
    pSlvWinAttr->stCropRect = pWinAttr->stCropRect;

    if (pstWin->bVirtScreenMode)
    {
        pSlvWinAttr->stOutRect  = pWinAttr->stOutRect;
    }
    else
    {
        (HI_VOID)DISP_GetDisplayInfo(pWinAttr->enDisp, &stDispInfo);
        (HI_VOID)DISP_GetDisplayInfo(pSlvWinAttr->enDisp, &stSlaveDispInfo);

        stCanvas = stDispInfo.stFmtResolution;
        stOffsetInfo.u32Left = 0;
        stOffsetInfo.u32Top  = 0;
        stOffsetInfo.u32Right = 0;
        stOffsetInfo.u32Bottom = 0;

        stMastOutRect = pWinAttr->stOutRect;
        if ((stMastOutRect.s32Width == 0) || (stMastOutRect.s32Height == 0))
        {
            stMastOutRect.s32Width  = stCanvas.s32Width;
            stMastOutRect.s32Height = stCanvas.s32Height;
        }

        (HI_VOID)WinOutRectSizeConversion(&stCanvas,
                                  &stOffsetInfo,
                                  &stSlaveDispInfo.stFmtResolution,
                                  &stMastOutRect,
                                  &pSlvWinAttr->stOutRect);
    }

    return HI_SUCCESS;
}

HI_S32 WinTestZero(volatile HI_U32 *pLock, HI_U32 u32MaxTimeIn10ms)
{
    volatile HI_U32 nLockState;
    HI_U32 u = 0;

    while(u < u32MaxTimeIn10ms)
    {
        nLockState = *pLock;
        if (!nLockState)
        {
            return HI_SUCCESS;
        }

        DISP_MSLEEP(10);
        u++;
    }

    return HI_ERR_VO_TIMEOUT;
}


HI_S32 WinCheckFrame(HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = (HI_DRV_VIDEO_PRIVATE_S*)&(pFrameInfo->u32Priv[0]);

    pstPriv->u32PlayTime = 1;

    if (pFrameInfo->eFrmType >    HI_DRV_FT_BUTT)
    {
        WIN_FATAL("Q Frame type error : %d\n", pFrameInfo->eFrmType);
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!( (HI_DRV_PIX_FMT_NV12 == pFrameInfo->ePixFormat)
            || (HI_DRV_PIX_FMT_NV21 == pFrameInfo->ePixFormat)
            || (HI_DRV_PIX_FMT_NV21_CMP  == pFrameInfo->ePixFormat)
            || (HI_DRV_PIX_FMT_NV21_TILE == pFrameInfo->ePixFormat)
            || (HI_DRV_PIX_FMT_NV21_TILE_CMP == pFrameInfo->ePixFormat)
            || (HI_DRV_PIX_FMT_NV61_2X1 == pFrameInfo->ePixFormat)
          )
       )
    {
        WIN_FATAL("Q Frame pixformat error : %d\n", pFrameInfo->ePixFormat);
        return HI_ERR_VO_INVALID_PARA;
    }

    if (    (pFrameInfo->u32Width < WIN_FRAME_MIN_WIDTH)
         || (pFrameInfo->u32Width > WIN_FRAME_MAX_WIDTH)
         || (pFrameInfo->u32Height < WIN_FRAME_MIN_HEIGHT)
         || (pFrameInfo->u32Height > WIN_FRAME_MAX_HEIGHT)
        )
    {
        WIN_FATAL("Q Frame resolution error : w=%d,h=%d\n",
                     pFrameInfo->u32Width, pFrameInfo->u32Height);
        return HI_ERR_VO_INVALID_PARA;
    }

    pFrameInfo->stDispRect.s32X = 0;
    pFrameInfo->stDispRect.s32Y = 0;
    pFrameInfo->stDispRect.s32Width  = pFrameInfo->u32Width;
    pFrameInfo->stDispRect.s32Height = pFrameInfo->u32Height;

/*
    if (   (pFrameInfo->stDispRect.s32X < 0)
        || (pFrameInfo->stDispRect.s32Width <  0)
        || ((pFrameInfo->stDispRect.s32Width + pFrameInfo->stDispRect.s32X) >  pFrameInfo->u32Width)
        || (pFrameInfo->stDispRect.s32Y < 0)
        || (pFrameInfo->stDispRect.s32Height < 0)
        || ((pFrameInfo->stDispRect.s32Height + pFrameInfo->stDispRect.s32Y) >    pFrameInfo->u32Height)
        )
    {
        return HI_ERR_VO_INVALID_PARA;
    }

    if (  (pFrameInfo->stDispAR.u8ARh > (pFrameInfo->stDispAR.u8ARw * WIN_MAX_ASPECT_RATIO))
        ||(pFrameInfo->stDispAR.u8ARw > (pFrameInfo->stDispAR.u8ARh  * WIN_MAX_ASPECT_RATIO))
        )
    {
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->u32FrameRate >    WIN_MAX_FRAME_RATE)
    {
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->eColorSpace >  HI_DRV_CS_SMPT240M)
    {
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->u32PlayTime >  WIN_MAX_FRAME_PLAY_TIME)
    {
        return HI_ERR_VO_INVALID_PARA;
    }

    // stBufAddr[1] is right eye for stereo video
    if (    (pFrameInfo->stBufAddr[0].u32Stride_Y < pFrameInfo->stDispRect.s32Width)
        ||    (pFrameInfo->stBufAddr[0].u32Stride_C < pFrameInfo->stDispRect.s32Width)
        )
    {
        return HI_ERR_VO_INVALID_PARA;
    }
*/

    return HI_SUCCESS;
}

/* window buffer manager */
HI_S32 WinBufferReset(WIN_BUFFER_S *pstBuffer)
{

    DISP_MEMSET(&pstBuffer->stUselessFrame, 0,
                sizeof(HI_DRV_VIDEO_FRAME_S)*WIN_USELESS_FRAME_MAX_NUMBER);

    pstBuffer->u32ULSRdPtr = 0;
    pstBuffer->u32ULSWtPtr = 0;

    pstBuffer->u32ULSIn  = 0;
    pstBuffer->u32ULSOut = 0;
    pstBuffer->u32UnderLoad= 0;

    return HI_SUCCESS;
}

HI_S32 WinBufferPutULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 WP1;
    HI_SIZE_T irqflag = 0;

    spin_lock_irqsave(&pstBuffer->stUlsLock, irqflag);
    WP1 = (pstBuffer->u32ULSWtPtr + 1) % WIN_USELESS_FRAME_MAX_NUMBER;

    if (WP1 == pstBuffer->u32ULSRdPtr)
    {
        WIN_ERROR("usl full\n");
        spin_unlock_irqrestore(&pstBuffer->stUlsLock, irqflag);
        return HI_ERR_VO_BUFQUE_FULL;
    }

    pstBuffer->stUselessFrame[pstBuffer->u32ULSWtPtr] = *pstFrame;

    pstBuffer->u32ULSWtPtr = WP1;
    pstBuffer->u32ULSIn++;
    spin_unlock_irqrestore(&pstBuffer->stUlsLock, irqflag);

    return HI_SUCCESS;
}

HI_S32 WinBufferGetULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if (pstBuffer->u32ULSWtPtr == pstBuffer->u32ULSRdPtr)
    {
        return HI_FAILURE;
    }

    *pstFrame = pstBuffer->stUselessFrame[pstBuffer->u32ULSRdPtr];
    pstBuffer->u32ULSRdPtr = (pstBuffer->u32ULSRdPtr + 1) % WIN_USELESS_FRAME_MAX_NUMBER;
    pstBuffer->u32ULSOut++;

    return HI_SUCCESS;
}

HI_S32 WinAttachDefaultSource(WINDOW_S *pstWin)
{
    WB_SOURCE_INFO_S stSrc2Buf;
    HI_S32 nRet;

    memset(&stSrc2Buf,0x0,sizeof(WB_SOURCE_INFO_S));
    /*when create window, default set the attach obj to vo it self.*/
    pstWin->stCfg.stSource.hSrc = pstWin->u32Index;
    pstWin->stCfg.stSource.pfAcqFrame = HI_NULL;
    pstWin->stCfg.stSource.pfRlsFrame = Win_ReleaseDisplayedFrame_ForDequeue;
    pstWin->stCfg.stSource.pfSendWinInfo = HI_NULL;

    /*and set the window's buffer  attach source.*/
    stSrc2Buf.hSrc = pstWin->u32Index;
    stSrc2Buf.hSecondSrc = HI_INVALID_HANDLE;
    stSrc2Buf.pfAcqFrame = HI_NULL;
    stSrc2Buf.pfRlsFrame = Win_ReleaseDisplayedFrame_ForDequeue;
    nRet =  WinBuf_SetSource(&pstWin->stBuffer.stWinBP, &stSrc2Buf);

    return nRet;
}

static HI_S32 WIN_Create_Seperate(HI_DRV_WIN_ATTR_S *pWinAttr,
                                          HI_HANDLE phWin,
                                          HI_U32    u32BufNum,
                                          WINDOW_S **pWindowReturn)
{
    WINDOW_S *pWindow = HI_NULL;
    HI_S32 nRet = HI_SUCCESS;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_U32 i = 0;

    if (DISP_IsOpened(pWinAttr->enDisp) != HI_TRUE)
    {
        WIN_ERROR("DISP is not opened!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = WinCreateDisplayWindow(pWinAttr, &pWindow, u32BufNum);
    if (nRet)
    {
        return nRet;
    }

    (HI_VOID)DISP_GetDisplayInfo(pWinAttr->enDisp, &stDispInfo);
    WinUpdateDispInfo(pWindow, &stDispInfo);



    for (i = 0; i < WIN_ALG_MMZ_NUM; i++)
    {
        nRet = DISP_OS_MMZ_AllocAndMap((const char *)"VDP_DBM_MEM",
                                        VDP_MEM_TYPE_PHY,
                                        WIN_ALG_MMZ_SIZE,
                                        VDP_MEM_ALIGN,
                                        &pWindow->stWinAlgMMZ[i].stVideoAlgMmz);
        if (nRet)
        {
           goto __ERR_RET_DESTROY__;
        }

        pWindow->stWinAlgMMZ[i].bReady = HI_TRUE;
    }

    /*before regcallback, create wbc first.*/
    nRet = WinRegCallback(pWindow);
    if (nRet)
    {
        goto __ERR_RET_DESTROY__;
    }

    nRet = WinAddWindow(pWindow->enDisp, pWindow);
    if (nRet)
    {
        goto __ERR_RET_UNREG_;
    }

    nRet = WinAttachDefaultSource(pWindow);
    if (nRet)
    {
        goto __ERR_RET_DELWIN;
    }

    /*this branch is for  main window,  only create wbc in  the first main window.*/
    if ((stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
        && DISP_IsFollowed(pWindow->enDisp))
    {
        stDispWindow.u32CurrentHdWindowCounts ++;
    }

    *pWindowReturn  = pWindow;
    return HI_SUCCESS;

 __ERR_RET_DELWIN:
    WinDelWindow(pWindow->u32Index);

 __ERR_RET_UNREG_:
    WinUnRegCallback(pWindow);

__ERR_RET_DESTROY__:

    while (i > 0)
    {
        if (pWindow->stWinAlgMMZ[i - 1].bReady == HI_TRUE)
        {
            DISP_OS_MMZ_UnmapAndRelease(&pWindow->stWinAlgMMZ[i - 1].stVideoAlgMmz);
        }

        pWindow->stWinAlgMMZ[i - 1].bReady = HI_FALSE;
        i --;
    }


    WinDestroyDisplayWindow(pWindow);
    return nRet;
}

HI_S32 WIN_Sync_DeInit(WINDOW_S *pstWindow);

HI_S32 WIN_Destroy_Seperate(HI_HANDLE hWin)
{
    WINDOW_S *pstWin =  HI_NULL;
    HI_S32 nRet = HI_SUCCESS;
    HI_U32 i = 0;
    WinCheckWindow(hWin, pstWin);

    /*check whether slave window can be destroy.*/
    if (HI_FALSE == WinCheckWhetherCanToDestroy(hWin))
    {
        return HI_SUCCESS;
    }

    /*disable  hardware first, after that  the software can be released.*/
    if (pstWin->bEnable == HI_TRUE)
    {
        nRet = WIN_SetEnable(hWin, HI_FALSE);
    }

    /* stop isr secondly.*/
    WinUnRegCallback(pstWin);

    #ifdef HI_VO_WIN_SYNC_SUPPORT
    (HI_VOID)WIN_Sync_DeInit(pstWin);
    #endif

     WinDelWindow(pstWin->u32Index);

    for (i = 0; i < WIN_ALG_MMZ_NUM; i++)
    {
        DISP_OS_MMZ_UnmapAndRelease(&pstWin->stWinAlgMMZ[i].stVideoAlgMmz);
        pstWin->stWinAlgMMZ[i].bReady = HI_FALSE;
    }

    if (WinCheckWhetherWbcIsogenyMode()
        && (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE))
    {
        stDispWindow.u32CurrentHdWindowCounts --;
    }


    WinDestroyDisplayWindow(pstWin);

    return nRet;
}

HI_S32 WIN_Sync_Init(WINDOW_S *pstWindow)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    WIN_SYNC_INFO_S *pstSyncInfo;
    HI_U32 u32Index;
    HI_S32 nRet = 0;

    if (pstWindow->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
    {
        pstSyncInfo = &(pstWindow->stSyncInfo);
        u32Index = pstWindow->u32Index;
    }
    else if (pstWindow->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        pstSyncInfo = &(pstWindow->stSyncInfo);
        u32Index = pstWindow->u32Index;
    }
    else
    {
        return HI_SUCCESS;
    }

    nRet = DRV_SYNC_Init(pstSyncInfo,u32Index);
    if (nRet)
    {
        WIN_ERROR("win %#x Sync Init Failed\n",u32Index);
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 WIN_Sync_DeInit(WINDOW_S *pstWindow)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    WIN_SYNC_INFO_S *pstSyncInfo;
    HI_S32 nRet = 0;

    if (pstWindow->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
    {
        pstSyncInfo = &(pstWindow->stSyncInfo);
    }
    else if (pstWindow->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        pstSyncInfo = &(pstWindow->stSyncInfo);
    }
    else
    {
        return HI_SUCCESS;
    }

    nRet = DRV_SYNC_DeInit(pstSyncInfo);
    if (nRet)
    {
        WIN_ERROR("win %#x Sync DeInit Failed\n",pstWindow->u32Index);
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 WIN_Sync_QueueFrame(HI_HANDLE hWin,HI_DRV_VIDEO_FRAME_S *pFrameInfo,HI_U32 *pu32FenceFd)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    WINDOW_S *pstWin;
    HI_S32 s32Ret = 0;

    WinCheckWindow(hWin, pstWin);
    if ((pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
        || (pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE))
    {
        s32Ret = WinQueueFrame(hWin,pFrameInfo,HI_NULL);
        if (s32Ret != HI_SUCCESS)
        {
            WIN_ERROR("win %#x QueueFrame Failed\n",hWin);
            return HI_FAILURE;
        }
        if (*pu32FenceFd != 0x0)
        {
            *pu32FenceFd = DRV_SYNC_CreateFence(&pstWin->stSyncInfo,
                                    pFrameInfo->u32FrameIndex,
                                    pFrameInfo->stBufAddr[0].u32PhyAddr_Y);
                                    }
        else
        {
            DRV_SYNC_AddData(&pstWin->stSyncInfo,
                    pFrameInfo->u32FrameIndex,
                    pFrameInfo->stBufAddr[0].u32PhyAddr_Y);
        }
    }
    else
    {
        WIN_ERROR("slave window does not support queue in wbc-isogeny mode.\n",hWin);
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 WIN_Sync_Reset(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E enRst,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    WINDOW_S *pstWin = HI_NULL;
    WIN_SYNC_INFO_S *pstSyncInfo;

    WinCheckDeviceOpen();
    WinCheckWindow(hWin, pstWin);

    if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE
        || pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        pstSyncInfo = &(pstWin->stSyncInfo);
    }
    else
    {
        return HI_SUCCESS;
    }

    if (enRst == HI_DRV_WIN_SWITCH_BLACK)
    {
        DRV_SYNC_Flush(pstSyncInfo);
    }
    else
    {
        if (pstFrame)
        {
            DRV_SYNC_Signal(pstSyncInfo,
                    pstFrame->u32FrameIndex,
                    pstFrame->stBufAddr[0].u32PhyAddr_Y);
        }
        else
        {
            DRV_SYNC_Flush(pstSyncInfo);
        }
    }
#endif
    return HI_SUCCESS;
}

HI_S32 WIN_Sync_Flush(WINDOW_S *pstWin)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    WIN_SYNC_INFO_S *pstSyncInfo;

    WinCheckDeviceOpen();

    if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE
        || pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        pstSyncInfo = &(pstWin->stSyncInfo);
    }
    else
    {
        return HI_SUCCESS;
    }

    DRV_SYNC_Flush(pstSyncInfo);
#endif
    return HI_SUCCESS;
}

HI_S32 WIN_Sync_Signal(WINDOW_S *pstWin,HI_U32 u32Index,HI_U32 u32Addr)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT

    WIN_SYNC_INFO_S *pstSyncInfo;
    WinCheckDeviceOpen();

    if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE
            || pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        pstSyncInfo = &(pstWin->stSyncInfo);
    }
    else
    {
        return HI_SUCCESS;
    }

    (HI_VOID)DRV_SYNC_Signal(pstSyncInfo,u32Index,u32Addr);
#endif
    return HI_SUCCESS;
}

HI_S32 WIN_Create_SlaveWin(HI_DRV_WIN_ATTR_S *pWinAttr,
                                    HI_BOOL bVirtScreen,
                                    HI_HANDLE *phSlvWin,
                                    HI_VOID* pstMasterWin)
{
    HI_DRV_WIN_ATTR_S stSlvWinAttr;
    WINDOW_S *pSlaveWindow = HI_NULL;
    HI_U32 phWin = 0;
    HI_S32 nRet = 0;

    /*in isogeny wbc mode, the sd window is a singleton mode.*/
    if ((stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
        && (stDispWindow.u32CurrentHdWindowCounts > 1))
    {
        *phSlvWin = g_hSlaveWin;
        return  HI_SUCCESS;
    }

    /*in fact, in isogeny-wbc mode,  slave's winattr partly is generated
      * according to the wbc's output.
      */
    nRet = WinGetSlaveWinAttr(pWinAttr, &stSlvWinAttr, bVirtScreen);
    if (nRet)
    {
        WIN_ERROR("Get Slave WinAttr is invalid!\n");
        return HI_FAILURE;
    }

    nRet = WIN_Create_Seperate(&stSlvWinAttr,
                                phWin,
                                WIN_IN_FB_DEFAULT_NUMBER,
                                &pSlaveWindow);
    if (nRet)
    {
        WIN_ERROR("Creat slave window failed!\n");
        return HI_FAILURE;
    }

    /*pointer to each other.*/
    *phSlvWin    =  (HI_HANDLE)(pSlaveWindow->u32Index);
    g_hSlaveWin  =  (HI_HANDLE)(pSlaveWindow->u32Index);

    pSlaveWindow->pstMstWin = (HI_VOID*)pstMasterWin;
    pSlaveWindow->enType = HI_DRV_WIN_ACTIVE_SLAVE ;
    pSlaveWindow->bVirtScreenMode = bVirtScreen;

    /*in this func,  means this is  iso-geny already.*/
    if (stDispWindow.eIsogeny_mode != ISOGENY_WBC_MODE)
    {
        #ifdef HI_VO_WIN_SYNC_SUPPORT
        /*in isogeny mode,  the sync mode  is not neccesary.*/
        (HI_VOID)WIN_Sync_Init(pSlaveWindow);
        #endif
    }

    return HI_SUCCESS;
}


/******************************************************************************
    apply function
******************************************************************************/
HI_S32 WIN_Create_MCE(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWin,HI_BOOL bVirtScreen)
{

    WINDOW_S *pstWin = HI_NULL;
    HI_S32 nRet = HI_SUCCESS;

    nRet = WIN_Create(pWinAttr,phWin,bVirtScreen);

    if (HI_SUCCESS == nRet)
    {
        WinCheckWindow(*phWin, pstWin);
        pstWin->bMCE = HI_TRUE;
    }

    return nRet;
}
HI_S32 WIN_Create(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWin,HI_BOOL bVirtScreen)
{
    HI_S32 nRet = HI_SUCCESS;
    WINDOW_S *pWindow = HI_NULL;
    HI_HANDLE hSlaveWin = 0;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);
    WinCheckNullPointer(phWin);

    // s1 check attribute
    nRet = WinCheckAttr(pWinAttr, bVirtScreen);
    if (nRet)
    {
        WIN_ERROR("WinAttr is invalid!\n");
        return nRet;
    }

    if (!WinGetRegWinManageStatus())
    {
        WinRegWinManageCallback(HI_DRV_DISPLAY_0);
        WinRegWinManageCallback(HI_DRV_DISPLAY_1);
        WinSetRegWinManageStatus(HI_TRUE);
    }

    if (pWinAttr->bVirtual != HI_TRUE)
    {
        nRet = WIN_Create_Seperate(pWinAttr, *phWin, WIN_IN_FB_DEFAULT_NUMBER,&pWindow);
        if (nRet)
        {
            return nRet;
        }

        pWindow->bVirtScreenMode = bVirtScreen;
        *phWin = (HI_HANDLE)(pWindow->u32Index);

        /*if master mode, create the slave window.*/
        if (DISP_IsFollowed(pWinAttr->enDisp) && ( HI_TRUE != WinCheckWhetherWbcIsogenyMode()))
        {
            nRet = WIN_Create_SlaveWin(pWinAttr,
                                     bVirtScreen,
                                     &hSlaveWin,
                                     pWindow);
            if (nRet)
                goto __ERR_RET_UNREG_CB__;

            pWindow->enType = HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE;
            pWindow->hSlvWin = hSlaveWin;

        }
        else
        {
            pWindow->enType = HI_DRV_WIN_ACTIVE_SINGLE;
        }

#ifdef HI_VO_WIN_SYNC_SUPPORT
        (HI_VOID)WIN_Sync_Init(pWindow);
#endif
        return HI_SUCCESS;

__ERR_RET_UNREG_CB__:
        WIN_Destroy_Seperate(pWindow->u32Index);
        return nRet;
    }
    else
    {
        VIRTUAL_S *pstVirWindow = HI_NULL;
        nRet = WIN_VIR_Create(pWinAttr, &pstVirWindow);
        if (nRet)
            return nRet;

        pstVirWindow->enType = HI_DRV_WIN_VITUAL_SINGLE;

        // add to virtual window array, if win_deinit, auto destory it
        nRet = WinAddVirWindow(pstVirWindow);
        if (nRet)
        {
            WIN_VIR_Destroy(pstVirWindow);
            return nRet;
        }
        else
        {
            *phWin = (HI_HANDLE)(pstVirWindow->u32Index);
            return HI_SUCCESS;
        }
    }

    return HI_SUCCESS;
}


/*slave's  enable and create/destroy opts , are invisible above.**/
HI_S32 WIN_Destroy(HI_HANDLE hWin)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;
    HI_HANDLE hSlaveWin = HI_INVALID_HANDLE;

    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        if (pstWin->hSlvWin)
        {
            hSlaveWin = pstWin->hSlvWin;
        }

        nRet = WIN_Destroy_Seperate(hWin);
        if (HI_SUCCESS != nRet)
        {
            return nRet;
        }

        /*destroy slave window first, then wbc.  because  if wbc destroyed first
          *, mem released either. slave window will have no mem to use.*/
        if (HI_INVALID_HANDLE != hSlaveWin)
        {
            nRet = WIN_Destroy_Seperate(hSlaveWin);
            if (HI_SUCCESS != nRet)
            {
                return nRet;
            }
        }
    }
    else
    {
        VIRTUAL_S *pstVirWin;

        WinCheckVirWindow(hWin, pstVirWin);
        WinDelVirWindow(hWin);
        nRet = WIN_VIR_Destroy(pstVirWin);
    }

    return HI_SUCCESS;
}

HI_S32 WIN_CheckAttachState(HI_HANDLE hWin,HI_BOOL *pbSrcAttached,HI_BOOL *pbSinkAttached)
{
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WINDOW_S *pstWin;

        WinCheckWindow(hWin, pstWin);

        if ((pstWin->stCfg.stSource.hSrc == HI_INVALID_HANDLE)
            || (pstWin->stCfg.stSource.hSrc == hWin))
        {
            *pbSrcAttached = HI_FALSE;
        }
        else
        {
            *pbSrcAttached = HI_TRUE;
        }

        *pbSinkAttached = HI_FALSE;
    }
    else
    {
        VIRTUAL_S *pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);

        if (pstVirWin->hSink == HI_INVALID_HANDLE)
        {
            *pbSinkAttached = HI_FALSE;
        }
        else
        {
            *pbSinkAttached = HI_TRUE;
        }

        if (pstVirWin->stSrcInfo.hSrc == HI_INVALID_HANDLE)
        {
            *pbSrcAttached = HI_FALSE;
        }
        else
        {
            *pbSrcAttached = HI_TRUE;
        }
    }

    return HI_SUCCESS;
}

/*get current number.*/
HI_U32 WIN_GetDisp1CurrentWinCnt(HI_VOID)
{
    return  stDispWindow.u32WinNumber[HI_DRV_DISPLAY_1];
}

HI_S32 WIN_SetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    WINDOW_S *pstWin;
    HI_DRV_WIN_ATTR_S stSlvWinAttr;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;
    WINDOW_S *pstWinTmp = HI_NULL;
    HI_U32  u32Wait = 0;
    HI_SIZE_T irqflag = 0;
    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);

    memset((void*)&stSlvWinAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));
    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        nRet = WinCheckFixedAttr(&pstWin->stCfg.stAttrBuf, pWinAttr);
        if (nRet)
        {
            return nRet;
        }

        nRet = WinCheckAttr(pWinAttr,pstWin->bVirtScreenMode);
        if (nRet)
        {
            return nRet;
        }

        nRet = DISP_GetDisplayInfo(pstWin->enDisp, &stDispInfo);
        if (nRet)
        {
            WIN_ERROR("DISP_GetDisplayInfo failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }

        pstWinTmp = (WINDOW_S *) DISP_MALLOC(sizeof(WINDOW_S));
        if (!pstWinTmp)
            return HI_ERR_VO_MALLOC_FAILED;

        *pstWinTmp = *pstWin;
        pstWinTmp->stCfg.stAttrBuf = *pWinAttr;
        WinUpdateDispInfo(pstWinTmp, &stDispInfo);

        /*to judge the layout valid or not.*/
        spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
        nRet =    WindowRedistributeProcess(pstWinTmp);
        if (nRet)
        {
            spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
            DISP_FREE(pstWinTmp);
            WIN_ERROR("reallocate video layer failed in %s!\n", __FUNCTION__);
            return nRet;
        }

        spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);

        /*update the formal window's initial fmt.*/
        WinUpdateDispInfo(pstWin, &stDispInfo);

        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
        pstWin->stCfg.stAttrBuf = *pWinAttr;
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);

        /*wait for the hal resources:such as physical layers, zorder to take effect.*/
        if (pstWin->bEnable)
        {
            (HI_VOID)WindowRedistributeProcess_Wait(pstWin, HI_TRUE);
            /* HSCP201602290020 WIN_SetAttr and Win's interrupt process function may run in
                    * different core, when interrupt process run after WIN_SetAttr,it will cause TIME-OUT.
                    * therefore,add atomic_read NewAttrFlag here to avoid this problem.
                    * Testing result shows that it takes about 1ms to run over all interrupt process functions
                    * in DISP1(0% location),here max sleep 5ms to ensure Win's interrupt function be processed.
                    */
            while (atomic_read(&pstWin->stCfg.bNewAttrFlag))
            {
                DISP_MSLEEP(1);
                u32Wait++;
                if (u32Wait > 5)
                {
                    break;
                }
            }

            if (atomic_read(&pstWin->stCfg.bNewAttrFlag) )
            {
                atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
                WIN_ERROR("WIN Set Attr timeout in %s\n", __FUNCTION__);
                DISP_FREE(pstWinTmp);
                return HI_ERR_VO_TIMEOUT;
            }
        }
        else
        {
            (HI_VOID)WindowRedistributeProcess_Wait(pstWin, HI_FALSE);
        }

        if (pstWin->hSlvWin)
        {
            /*only non-wbc mode , or 1 window in wbc-mode, can slave's attr be set. */
            if ((stDispWindow.eIsogeny_mode != ISOGENY_WBC_MODE) ||  (WIN_GetDisp1CurrentWinCnt() == 1))
            {
                WIN_GetAttr(pstWin->hSlvWin, &stSlvWinAttr);
                WinGetSlaveWinAttr2(pstWin, pWinAttr, &stSlvWinAttr);
                nRet = WIN_SetAttr(pstWin->hSlvWin, &stSlvWinAttr);
            }
        }

        DISP_FREE(pstWinTmp);
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        WinCheckVirWindow(hWin, pstVirWindow);
        nRet = WIN_VIR_SetAttr(pstVirWindow, pWinAttr);

        return nRet;
    }

    return nRet;
}



HI_S32 WIN_GetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);
    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WINDOW_S *pstWin;

        WinCheckWindow(hWin, pstWin);
        *pWinAttr = pstWin->stCfg.stAttr;
    }
    else
    {
        VIRTUAL_S *pstVirWindow;

        WinCheckVirWindow(hWin, pstVirWindow);
        *pWinAttr = pstVirWindow->stAttrBuf;
    }

    return HI_SUCCESS;
}


//get info for source
HI_S32 WIN_GetInfo(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstInfo);

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        pstInfo->eType = WinGetType(pstWin);
        pstInfo->hPrim = (HI_HANDLE)(pstWin->u32Index);
        pstInfo->hSec  = (HI_HANDLE)(pstWin->hSlvWin);
    }
    else
    {
        VIRTUAL_S *pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);

        pstInfo->eType = pstVirWin->enType;
        pstInfo->hPrim = (HI_HANDLE)(pstVirWin->u32Index);
        pstInfo->hSec  = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

static HI_S32 WIN_ResetWithBlkFrm(HI_HANDLE hWin, HI_BOOL *pbEnable)
{
    HI_S32  nRet = HI_SUCCESS;
    (HI_VOID)WIN_GetEnable(hWin, pbEnable);
    if (HI_TRUE == *pbEnable)
        {
            WIN_ERROR("Window is still working,can't be detached,please disable it first\n");
            nRet = HI_ERR_VO_INVALID_OPT;
        }
        else
        {
            nRet = WIN_Reset(hWin, HI_DRV_WIN_SWITCH_BLACK);
            if (HI_SUCCESS != nRet)
            {
                WIN_ERROR("Reset Window Failed\n");
            }
            //WIN_ERROR("Reset win with black frame!\n");
        }
    return nRet;
}

HI_S32 WIN_SetSdrSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_DISP_DISPLAY_INFO_S stDispInfo = {0};
    WB_SOURCE_INFO_S stSrc2Buf = {0};
    WINDOW_S *pstWin = HI_NULL;
    HI_BOOL bVirtual = HI_FALSE;

    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstSrc);

    if (pstSrc->hSrc == HI_INVALID_HANDLE)
    {
        HI_BOOL bEnable = HI_FALSE;
        (HI_VOID)WIN_GetEnable(hWin,&bEnable);
        if (bEnable == HI_TRUE)
        {
            DISP_ERROR("Window is still working,can't be detached,please disable it first\n");
            return HI_ERR_VO_INVALID_OPT;
        }
        else
        {
            nRet = WIN_Reset(hWin,HI_DRV_WIN_SWITCH_BLACK);
            if (HI_SUCCESS != nRet)
            {
                DISP_ERROR("Reset Window Failed\n");
                return nRet;
            }
        }
    }

    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        nRet = DISP_GetDisplayInfo(WinGetDispID(pstWin), &stDispInfo);
        if (nRet)
        {
            return nRet;
        }

        if (pstWin->stCfg.stSource.hSrc == pstSrc->hSrc)
        {
            if (pstSrc->hSrc)
                WIN_ERROR("Attach repeately!\n");
            else
                WIN_ERROR("Detach repeately!\n");

            return HI_ERR_VO_OPERATION_DENIED;
        }

        pstWin->stCfg.stSource = *pstSrc;

        stSrc2Buf.hSrc = pstSrc->hSrc;
        stSrc2Buf.pfAcqFrame = pstSrc->pfAcqFrame;
        stSrc2Buf.pfRlsFrame = pstSrc->pfRlsFrame;
        nRet =  WinBuf_SetSource(&pstWin->stBuffer.stWinBP, &stSrc2Buf);

        if (nRet)
        {
            return nRet;
        }

        // send attr to source
        WinSendAttrToSource(pstWin, &stDispInfo);
    }
    else
    {
        WIN_ERROR("Virtual Window doesn't support SDR/HDR mode\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_SetHdrSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    WB_SOURCE_INFO_S stSrc2Buf;
    WINDOW_S* pstWin;
    HI_BOOL bEnable = HI_FALSE;
    HI_BOOL bVirtual;

    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstSrc);

    if ((HI_INVALID_HANDLE == pstSrc->hSrc) &&
        (HI_INVALID_HANDLE == pstSrc->hSecondSrc))
    {
        nRet = WIN_ResetWithBlkFrm(hWin, &bEnable);
        if (HI_SUCCESS != nRet)
        {
            return nRet;
        }
    }
    if ((HI_INVALID_HANDLE == pstSrc->hSrc) &&
        (pstSrc->hSecondSrc != HI_INVALID_HANDLE))
    {
        WIN_ERROR("Not support,BL is invalid while EL is valid!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    if ((HI_INVALID_HANDLE != pstSrc->hSrc) &&
        (HI_INVALID_HANDLE == pstSrc->hSecondSrc))
    {
        nRet = WIN_ResetWithBlkFrm(hWin, &bEnable);
        if (HI_SUCCESS != nRet)
        {
            return nRet;
        }
    }


    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        nRet = DISP_GetDisplayInfo(WinGetDispID(pstWin), &stDispInfo);
        if (HI_SUCCESS != nRet)
        {
            return nRet;
        }
        if (HI_INVALID_HANDLE == pstSrc->hSrc)
        {
            if ((pstWin->stCfg.stSource.hSrc == pstSrc->hSrc) &&
                (pstWin->stCfg.stSource.hSecondSrc == pstSrc->hSecondSrc))
            {
                WIN_ERROR("Detach repeately!\n");
                return HI_ERR_VO_OPERATION_DENIED;
            }
        }
        else
        {
            if ((pstWin->stCfg.stSource.hSrc == pstSrc->hSrc) &&
                (pstWin->stCfg.stSource.hSecondSrc == pstSrc->hSecondSrc))
            {
                WIN_ERROR("Attach repeately!\n");
                return HI_ERR_VO_OPERATION_DENIED;
            }
        }
        pstWin->stCfg.stSource = *pstSrc;

        stSrc2Buf.hSrc = pstSrc->hSrc;
        stSrc2Buf.hSecondSrc = pstSrc->hSecondSrc;
        stSrc2Buf.pfAcqFrame = pstSrc->pfAcqFrame;
        stSrc2Buf.pfRlsFrame = pstSrc->pfRlsFrame;
        nRet =  WinBuf_SetSource(&pstWin->stBuffer.stWinBP, &stSrc2Buf);

        if (HI_SUCCESS != nRet)
        {
            return nRet;
        }

        // send attr to source
        WinSendAttrToSource(pstWin, &stDispInfo);

    }
    else
    {
        VIRTUAL_S* pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);

        if (pstVirWin->stSrcInfo.hSrc == pstSrc->hSrc)
        {
            if (pstSrc->hSrc)
            { WIN_ERROR("Attach repeately!\n"); }
            else
            { WIN_ERROR("Detach repeately!\n"); }

            return HI_ERR_VO_OPERATION_DENIED;
        }

        pstVirWin->stSrcInfo = *pstSrc;

        WIN_VIR_SendAttrToSource(pstVirWin);
    }
    return HI_SUCCESS;
}

HI_S32 WIN_SetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_S32    s32Ret;
    WINDOW_S* pstWin;
    HI_BOOL bVirtual;

    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        if((HI_DRV_WINDOW_MODE_NORMAL == pstWin->enWinMode)
            || (HI_DRV_WINDOW_MODE_HDR == pstWin->enWinMode))
        {
            pstSrc->hSecondSrc = HI_INVALID_HANDLE;
            s32Ret = WIN_SetSdrSource(hWin,pstSrc);
        }
        else if(HI_DRV_WINDOW_MODE_DOLBY == pstWin->enWinMode)
        {
            s32Ret = WIN_SetHdrSource(hWin,pstSrc);
        }
        else
        {
            return HI_ERR_VO_WIN_UNSUPPORT;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);

        if (pstVirWin->stSrcInfo.hSrc == pstSrc->hSrc)
        {
            if (pstSrc->hSrc)
                WIN_ERROR("Attach repeately!\n");
            else
                WIN_ERROR("Detach repeately!\n");

            return HI_ERR_VO_OPERATION_DENIED;
        }

        pstVirWin->stSrcInfo = *pstSrc;

        s32Ret = WIN_VIR_SendAttrToSource(pstVirWin);
    }
    return s32Ret;
}
HI_S32 WIN_GetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstSrc);
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        *pstSrc = pstWin->stCfg.stSource;
    }
    else
    {
        VIRTUAL_S *pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);
        *pstSrc = pstVirWin->stSrcInfo;
    }


    return HI_SUCCESS;
}

/*
1) whether salve;
2) now window's num;
3) wbc mode or not?
3) enable or not.
4) if enable == pstwin's enable status, return;
   output: whether allow to set enable or not.
*/
HI_BOOL WIN_Judge_WhetherCanSetEnable(WINDOW_S *pstWin, HI_BOOL bEnable)
{
    HI_U32 i = 0, u32WinEnableNum = 0;

    /*if  flag is the same with previous setting, do nothing.*/
    if (bEnable == pstWin->bEnable)
    {
        WIN_WARN("Set the same enable Flag.!\n");
        return HI_FALSE;
    }

    for (i = 0; i < WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[HI_DRV_DISPLAY_1][i]
            && (stDispWindow.pstWinArray[HI_DRV_DISPLAY_1][i]->bEnable))
        {
            u32WinEnableNum ++;
        }
    }

     if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin)
         || HI_DRV_WIN_ACTIVE_SINGLE == WinGetType(pstWin)
         || HI_DRV_WIN_VITUAL_SINGLE == WinGetType(pstWin))
     {
        /*if master,  do not  make limit.*/
        return HI_TRUE;
     }
     else
     {
        /*only  slave window not in wbc mode can do freeze.*/
        if (stDispWindow.eIsogeny_mode != ISOGENY_WBC_MODE)
        {
            /*if non-wbc isogeny mode, enable  do not make limit.*/
            return HI_TRUE;
        }
        else
        {
            if ((bEnable) && (u32WinEnableNum > 0))
            {
                /*if enable, only in condition:  more than 1 window enabled. */
                return HI_TRUE;
            }
            else if ((!bEnable) && (u32WinEnableNum == 0))
            {
                /*if disabled, only in condition:  all the window disabled. */
                return HI_TRUE;
            }

            return HI_FALSE;
        }
     }

}

HI_S32 WIN_SetEnable_Seperate(WINDOW_S *pstWin, HI_BOOL bEnable)
{
    HI_S32 ret = HI_SUCCESS;
    WINDOW_S *pstWinTmp = HI_NULL;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_SIZE_T irqflag = 0;

    /*if isogeny wbc mode, and slave window, we should give a refcnt increase.*/
    if (HI_FALSE == WIN_Judge_WhetherCanSetEnable(pstWin, bEnable))
    {
        return HI_SUCCESS;
    }

    memset((void*)&stDispInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));
    pstWinTmp = (WINDOW_S *) DISP_MALLOC(sizeof(WINDOW_S));
    if (!pstWinTmp)
        return HI_FAILURE;

    *pstWinTmp = *pstWin;
    pstWinTmp->bEnable =  bEnable;

    ret = DISP_GetDisplayInfo(pstWin->enDisp, &stDispInfo);
    if (ret)
    {
            stDispInfo.stFmtResolution.s32X = 0;
            stDispInfo.stFmtResolution.s32Y = 0;
            stDispInfo.stFmtResolution.s32Width = 1280;
            stDispInfo.stFmtResolution.s32Height = 720;
            memset((void*)&stDispInfo.stOffsetInfo, 0, sizeof(HI_DRV_DISP_OFFSET_S));
            WIN_WARN("when call set_enable, may ctrl+c, display closed\n");
    }

    /* when enable,may cause win-layer remapping.*/
    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);
    ret =  WindowRedistributeProcess(pstWinTmp);
    if (ret) {
        spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
        DISP_FREE(pstWinTmp);
        return ret;
    }

    pstWin->bEnable = bEnable;
    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);

    (HI_VOID)WindowRedistributeProcess_Wait(pstWinTmp, HI_FALSE);

    DISP_FREE(pstWinTmp);
    return HI_SUCCESS;
}

HI_S32 WIN_SetEnable(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin = HI_NULL, *pstSlvWin = HI_NULL;
    HI_BOOL bVirtual;
    HI_S32 ret = HI_SUCCESS;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        /*if window has opened, return hi_success.*/
        if (pstWin->bEnable == bEnable)
            return HI_SUCCESS;

        if ((ret = WIN_SetEnable_Seperate(pstWin, bEnable)))
        {
            WIN_ERROR("main window enable error:%x\n",ret);
            return ret;
        }

        if ((HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
            && (pstWin->hSlvWin))
        {
            WinCheckWindow(pstWin->hSlvWin, pstSlvWin);
            if ((ret = WIN_SetEnable_Seperate(pstSlvWin, bEnable)))
            {
                WIN_ERROR("slave window enable error:%x\n",ret);
                return ret;
            }
        }

        /*wait for v0/v3/wbc closed indeed.*/
        if (bEnable == HI_FALSE)
        {
            (HI_VOID)Win_WaitForLayerClosed(pstWin, pstSlvWin);
        }
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        WinCheckVirWindow(hWin, pstVirWindow);
        pstVirWindow->bEnable = bEnable;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_GetEnable(HI_HANDLE hWin, HI_BOOL *pbEnable)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pbEnable);

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        *pbEnable = pstWin->bEnable;
    }
    else
    {
        VIRTUAL_S *pstVirWindow;

        WinCheckVirWindow(hWin, pstVirWindow);

        *pbEnable = pstVirWindow->bEnable;

    }
    return HI_SUCCESS;
}



#define WIN_PROCESS_CALC_TIME_THRESHOLD 10
#define WIN_DELAY_TIME_MAX_CIRCLE 50
HI_S32 WIN_CalcDelayTime(WINDOW_S *pstWin, HI_U32 *pu32BufNum, HI_U32 *pu32DelayMs)
{
    HI_U32 u32Num, u32NumNew, T, Dt, Ct, LstCt, Delay;
    HI_U32 L = 0;

__WIN_CALC_DELAY__:
    L++;
    if (L > WIN_PROCESS_CALC_TIME_THRESHOLD)
    {
        goto __WIN_CALC_DELAY_ERROR__;
    }

    LstCt = pstWin->stDelayInfo.u32CfgTime;
    T = pstWin->stDelayInfo.T;

    WinBuf_GetFullBufNum(&pstWin->stBuffer.stWinBP, &u32Num);
    Dt = pstWin->stDelayInfo.u32DisplayTime;

    HI_DRV_SYS_GetTimeStampMs(&Ct);

    if (Ct >= LstCt)
    {
        Delay = Ct - LstCt;
    }
    else
    {
        // circle happen, Ct across zero
        Delay = 0xFFFFFFFFul - LstCt + Ct;
    }

#if 0
    if (Delay > ((T*3)/2) )
    {
        WIN_ERROR("Delay=%d, T = %d\n", Delay, T);
        goto __WIN_CALC_DELAY__;
    }
#endif

    if (T <= (Dt + Delay))
    {
        if (((u32Num + 1) * T) < (Dt + Delay - T))
        {
            /*when fmt changed, may come into this branch .
              *since  isr func not executed, the delay time may be too long.
              * so we just give a basic value,  not  accurate.
              */
            Delay = (u32Num + 1) * T;
        }
        else
        {
            Delay = (u32Num + 1) * T - (Dt + Delay - T) ;
        }
    }
    else
    {
        Delay = T - Dt - Delay;
        Delay = (u32Num + 1) * T + Delay;
    }


    WinBuf_GetFullBufNum(&pstWin->stBuffer.stWinBP, &u32NumNew);
    if (  (LstCt != pstWin->stDelayInfo.u32CfgTime)
        ||(u32NumNew != u32Num)
        ||(HI_TRUE == pstWin->bInInterrupt)
       )
    {
        udelay(100);
        goto __WIN_CALC_DELAY__;
    }

    if (Delay > (T * WIN_DELAY_TIME_MAX_CIRCLE))
    {
        DISP_ASSERT(!Delay);
        goto __WIN_CALC_DELAY_ERROR__;
    }

    *pu32BufNum = u32Num;
    *pu32DelayMs= Delay;
    return HI_SUCCESS;

__WIN_CALC_DELAY_ERROR__:

    *pu32BufNum = 0;
    *pu32DelayMs= 0;

    return HI_SUCCESS;
}

HI_BOOL WinGetTBMatchInfo(HI_HANDLE hWin)
{
    WINDOW_S *pstWin;

    WinCheckWindow(hWin, pstWin);

    return pstWin->stDelayInfo.bTBMatch;
}

HI_S32 WIN_GetPlayInfo(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstInfo);
    WinCheckWindow(hWin, pstWin);

    if (!pstWin->bEnable || !pstWin->stDelayInfo.u32DispRate)
    {
        WIN_WARN("window is not ready! state:%d dispRate:%d\n",
                        pstWin->bEnable,
                        pstWin->stDelayInfo.u32DispRate);
        return HI_ERR_VO_INVALID_OPT;
    }


    WIN_CalcDelayTime(pstWin, &(pstInfo->u32FrameNumInBufQn), &(pstInfo->u32DelayTime));
    pstInfo->u32DispRate = pstWin->stDelayInfo.u32DispRate;

    return HI_SUCCESS;
}

HI_S32 WinPutNewFrameIntoBufNode(WB_POOL_S *pstWinBP,
                                           HI_BOOL bHdrFrame,
                                           HI_DRV_VIDEO_FRAME_S *pstFrame,
                                           HI_DRV_DOLBY_META_INFO_S *pstDolbyMdInfo,
                                           HI_BOOL bHisiHdr,
                                           HI_U32  u32PlayCnt)
{
    HI_S32  nRet = HI_SUCCESS;
    HI_BOOL bDolbyProcess = HI_FALSE;

    WinCheckNullPointer(pstWinBP);
    WinCheckNullPointer(pstFrame);

    if (HI_FALSE == bHdrFrame)
    {
        nRet = WinBuf_PutNewFrame(pstWinBP, pstFrame, u32PlayCnt);
    }
    else
    {
        if (HI_NULL == pstDolbyMdInfo)
        {
            bDolbyProcess = HI_FALSE;
        }
        else
        {
            WIN_Hdr10ProcessPolicy(pstFrame->enSrcFrameType,
                                     pstDolbyMdInfo->enDispOutType,
                                     bHisiHdr,
                                     &bDolbyProcess);
        }

        nRet = WinBuf_PutNewHdrFrame(pstWinBP,
                                     pstFrame,
                                     bDolbyProcess,
                                     pstDolbyMdInfo,
                                     u32PlayCnt);
    }

    return nRet;

}


HI_S32 WinQueueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo, HI_DRV_DOLBY_META_INFO_S *pstDolbyMdInfo)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;
    HI_BOOL bHdrFrame = HI_FALSE;
    HI_DRV_WINDOW_MODE_E enWinMode = HI_DRV_WINDOW_MODE_BUTT;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pFrameInfo);

    if (HI_DRV_VIDEO_FRAME_TYPE_BUTT <= pFrameInfo->enSrcFrameType)
    {
        WIN_ERROR("Invalid Frame Type!\n");
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    nRet = WinCheckFrame(pFrameInfo);
    if (nRet)
    {
        WIN_ERROR("win frame parameters invalid\n");
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    /* judge frame type. */
    WIN_JudgeHdrFrame(pFrameInfo->enSrcFrameType, &bHdrFrame);

    enWinMode = (HI_FALSE == bHdrFrame) ? HI_DRV_WINDOW_MODE_NORMAL : HI_DRV_WINDOW_MODE_HDR;
    nRet = WIN_SetMode(hWin, enWinMode);
    if(HI_SUCCESS != nRet)
    {
        WIN_ERROR("Set WinMode Fail!\n");
        return nRet;
    }

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        if ((pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE
            || pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
            && (pFrameInfo->u32FrameIndex != pstWin->u32LastInLowDelayIdx)
            )
        {
            HI_LD_Event_S evt;
            HI_U32 TmpTime = 0;
            HI_DRV_SYS_GetTimeStampMs(&TmpTime);
            evt.evt_id = EVENT_VO_FRM_IN;
            evt.frame = pFrameInfo->u32FrameIndex;
            evt.handle = pFrameInfo->hTunnelSrc;
            evt.time = TmpTime;
            HI_DRV_LD_Notify_Event(&evt);
            pstWin->u32LastInLowDelayIdx = pFrameInfo->u32FrameIndex;
        }

        if (pstWin->bEnable == HI_TRUE)
        {
            HI_U32 u32PlayCnt = 0;

            DRV_WIN_FRC_Calculate(pstWin->hFrc,
                pFrameInfo->u32FrameRate/10,
                pstWin->stDispInfo.u32RefreshRate,
                &u32PlayCnt);

            if (pFrameInfo->u32FrameRate/10 != pstWin->stDispInfo.u32RefreshRate)
            {
                pFrameInfo->u32OriFrameRate = pFrameInfo->u32FrameRate;
                pFrameInfo->u32FrameRate = pstWin->stDispInfo.u32RefreshRate*10;
            }

            if (u32PlayCnt)
            {
                nRet = WinPutNewFrameIntoBufNode(&pstWin->stBuffer.stWinBP,
                                                 bHdrFrame,
                                                 pFrameInfo,
                                                 pstDolbyMdInfo,
                                                 pstWin->stUsingAttr.bHisiHdr,
                                                 u32PlayCnt);
            }
            else
            {
                nRet = WinBufferPutULSFrame(&pstWin->stBuffer, pFrameInfo);
            }

            if (nRet)
            {
                return HI_ERR_VO_BUFQUE_FULL;
            }
        }
        else
        {
            WIN_WARN("Window is disabled\n");
            return HI_ERR_VO_WIN_NOT_ENABLE;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWin;
        WinCheckVirWindow(hWin, pstVirWin);

        // update sink acquire frame count
        pstVirWin->stFrameStat.u32SrcQTry++;

        nRet = WIN_VIR_AddNewFrm(pstVirWin, pFrameInfo);
        if (nRet)
        {
            return HI_ERR_VO_BUFQUE_FULL;
        }

        pstVirWin->stFrameStat.u32SrcQOK++;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_QueueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo, HI_DRV_DOLBY_META_INFO_S *pstDolbyMdInfo)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

    WinCheckNullPointer(pFrameInfo);
    pFrameInfo->bStillFrame =  HI_FALSE;

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pFrameInfo->u32Priv;
    /*if come into this func , means non-fence mode.*/
    pstPriv->bForFenceUse = HI_FALSE;

    return WinQueueFrame(hWin, pFrameInfo,pstDolbyMdInfo);
}

HI_S32 WIN_QueueSyncFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo, HI_U32 *pu32FenceFd)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

    WinCheckNullPointer(pFrameInfo);

    WinCheckNullPointer(pu32FenceFd);

    pFrameInfo->bStillFrame =  HI_FALSE;

    pFrameInfo->enFieldMode = HI_DRV_FIELD_ALL;

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pFrameInfo->u32Priv;

    pstPriv->eOriginField = HI_DRV_FIELD_ALL;

    /*if come into this func , means non-fence mode.*/
    pstPriv->bForFenceUse = HI_TRUE;

#ifdef HI_VO_WIN_SYNC_SUPPORT
    s32Ret = WIN_Sync_QueueFrame(hWin,pFrameInfo,pu32FenceFd);
#endif

    return s32Ret;
}

HI_S32 WIN_QueueUselessFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;


    WinCheckDeviceOpen();

    WinCheckNullPointer(pFrameInfo);

    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        nRet = WinCheckFrame(pFrameInfo);
        if (nRet)
        {
            WIN_ERROR("win frame parameters invalid\n");
            return HI_ERR_VO_FRAME_INFO_ERROR;
        }

        if (pstWin->bEnable == HI_TRUE)
        {
            nRet = WinBufferPutULSFrame(&pstWin->stBuffer, pFrameInfo);
            if (nRet)
            {
                WIN_WARN("quls failed\n");
                return HI_ERR_VO_BUFQUE_FULL;
            }
        }
        else
        {
            WIN_ERROR("Window is disabled\n");
            return HI_ERR_VO_INVALID_OPT;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        WinCheckVirWindow(hWin, pstVirWindow);

        nRet = WinCheckFrame(pFrameInfo);
        if (nRet)
        {
            WIN_ERROR("win frame parameters invalid\n");
            return HI_ERR_VO_FRAME_INFO_ERROR;
        }

        nRet = WIN_VIR_AddUlsFrm(pstVirWindow, pFrameInfo);
        if (nRet)
        {
            WIN_WARN("quls failed\n");
            return HI_ERR_VO_BUFQUE_FULL;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WIN_DequeueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    HI_S32 nRet = HI_FAILURE;
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pFrameInfo);
    WinCheckWindow(hWin, pstWin);

    nRet = Win_Dequeue_DisplayedFrame(hWin, pFrameInfo);

    return nRet;
}

HI_S32 WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E enZFlag)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();

    if (enZFlag >= HI_DRV_DISP_ZORDER_BUTT)
    {
        WIN_FATAL("HI_DRV_DISP_ZORDER_E invalid!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);

        switch(enZFlag)
        {
            case HI_DRV_DISP_ZORDER_MOVETOP:
                nRet = WinZorderMoveTop(pstWin);
                break;
            case HI_DRV_DISP_ZORDER_MOVEUP:
                nRet = WinZorderMoveUp(pstWin);
                break;
            case HI_DRV_DISP_ZORDER_MOVEBOTTOM:
                nRet = WinZorderMoveBottom(pstWin);
                break;
            case HI_DRV_DISP_ZORDER_MOVEDOWN:
                nRet = WinZorderMoveDown(pstWin);
                break;
            default :
                nRet = HI_ERR_VO_INVALID_OPT;
                break;
        }

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
        {
            if (pstWin->hSlvWin)
            {
                WIN_SetZorder(pstWin->hSlvWin, enZFlag);
            }
        }
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return nRet;
}

HI_S32 WIN_GetZorder(HI_HANDLE hWin, HI_U32 *pu32Zorder)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    WinCheckNullPointer(pu32Zorder);

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        *pu32Zorder = pstWin->u32Zorder;
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
   return nRet;
}

/*check whether  window is ready for changing?*/
HI_S32 WIN_CheckWinStatus(WINDOW_S *pstWin, HI_BOOL bBlock)
{
    HI_U32 i = 0;

    if (bBlock)
    {
        while ((pstWin->bUpState || pstWin->bReset) && (i < 10))
        {
            msleep(5);
            i ++;
        }
    }

    if (pstWin->bUpState || pstWin->bReset)
    {
         WIN_ERROR("Window is in updating status, can't be changed now!\n");
         return HI_ERR_VO_INVALID_OPT;
    }
    else
    {
        return HI_SUCCESS;
    }

}

HI_S32 WIN_Freeze(HI_HANDLE hWin, HI_BOOL bEnable, HI_DRV_WIN_SWITCH_E enFrz)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;
    HI_U32    u = 0;


    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);
        //WinCheckSlaveWindow(pstWin);

        if (enFrz >= HI_DRV_WIN_SWITCH_BUTT)
        {
            WIN_ERROR("Freeze mode is invalid!\n");
            return HI_ERR_VO_INVALID_PARA;
        }

        if (HI_SUCCESS != WIN_CheckWinStatus(pstWin, HI_TRUE))
        {
            return HI_ERR_VO_INVALID_OPT;
        }

        pstWin->bUpState = HI_FALSE;

        pstWin->enStateNew = bEnable ? WIN_STATE_FREEZE : WIN_STATE_UNFREEZE;
        if (bEnable)
            pstWin->stFrz.enFreezeMode = enFrz;
        else
            pstWin->stFrz.enFreezeMode = HI_DRV_WIN_SWITCH_BUTT;


        pstWin->bUpState = HI_TRUE;

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
        {
            /*only  slave window not in wbc mode can do freeze.*/
            if ((pstWin->hSlvWin) && (stDispWindow.eIsogeny_mode != ISOGENY_WBC_MODE))
            {
                WIN_Freeze(pstWin->hSlvWin, bEnable, enFrz);
            }
        }

        u = 0;
        while(pstWin->bUpState && (u<10))
        {
            DISP_MSLEEP(5);
            u++;
        }

        if (u >= 10)
        {
            DISP_WARN("############ freeze TIMEOUT#########\n");
        }
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return HI_SUCCESS;
}

HI_S32 WIN_GetFreezeStatus(HI_HANDLE hWin, HI_BOOL *pbEnable, HI_DRV_WIN_SWITCH_E *penFrz)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    WinCheckNullPointer(penFrz);

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        *pbEnable = (pstWin->enStateNew == WIN_STATE_FREEZE) ? HI_TRUE : HI_FALSE;
        *penFrz = pstWin->stFrz.enFreezeMode;
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return HI_SUCCESS;
}


HI_S32 WIN_ResetSeperate(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E enRst)
{
    WINDOW_S *pstWin = HI_NULL;
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    HI_BOOL bVirtual;

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);

        /*in wbc mode, the slave window does not need to be reset.*/
        if ((pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
                && (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE))
        {
            return HI_SUCCESS;
        }

        if (pstWin->bEnable)
        {
            pstWin->stRst.enResetMode = enRst;
            pstWin->bReset = HI_TRUE;
        }
        else
        {
            WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
            ISR_WinReleaseUSLFrame(pstWin);
            WinBuf_DiscardDisplayedFrame(&pstWin->stBuffer.stWinBP);

            /* flush frame in full buffer pool */
            pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);

#ifdef HI_VO_WIN_SYNC_SUPPORT
            (HI_VOID)WIN_Sync_Reset(hWin,enRst,pstFrame);
#endif

            WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);
            pstWin->bReset = HI_FALSE;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        HI_S32 s32Ret;

        WinCheckVirWindow(hWin, pstVirWindow);

        s32Ret = WIN_VIR_Reset(pstVirWindow);
        if (HI_SUCCESS != s32Ret)
        {
            DISP_ERROR("Reset Virtual Window Failed\n");
            return s32Ret;
        }
    }
    return HI_SUCCESS;
}


HI_S32 WIN_Reset(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E enRst)
{
    WINDOW_S *pstWin = HI_NULL, *pstWinSlv = HI_NULL;
    HI_BOOL bVirtual;
    HI_S32  ret = 0;

    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        if (enRst >= HI_DRV_WIN_SWITCH_BUTT)
        {
            WIN_ERROR("Reset mode is invalid!\n");
            return HI_ERR_VO_INVALID_PARA;
        }

        if (pstWin->bUpState)
        {
            WIN_WARN("Window may be in changing status!\n");
            return HI_ERR_VO_INVALID_OPT;
        }

        if ((ret = WIN_ResetSeperate(hWin, enRst)))
            return ret;

        /*now we do main and slave window reset in drv, not in mpi/unf level.*/
        if (pstWin->hSlvWin)
        {
            WinCheckWindow(pstWin->hSlvWin, pstWinSlv);
            if ((ret = WIN_ResetSeperate(pstWin->hSlvWin, enRst)))
                return ret;
        }
    }
    else
    {
        return WIN_ResetSeperate(hWin, enRst);
    }

    return HI_SUCCESS;
}


HI_S32 WIN_Pause(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin;
    HI_U32 u;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);

        /*if  slave window in wbc mode,  does not allow pause.  slave win's pause is realized by wbc.*/
        if ((pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE) && (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE))
        {
            return HI_SUCCESS;
        }

        if (HI_SUCCESS != WIN_CheckWinStatus(pstWin, HI_TRUE))
        {
            return HI_ERR_VO_INVALID_OPT;
        }

        pstWin->bUpState = HI_FALSE;
        pstWin->enStateNew = bEnable ? WIN_STATE_PAUSE : WIN_STATE_RESUME;
        pstWin->bUpState = HI_TRUE;

        u = 0;
        while(pstWin->bUpState && (u<10))
        {
            DISP_MSLEEP(5);
            u++;
        }

        if (u >= 10)
        {
            DISP_WARN("############ PAUSE TIMEOUT#########\n");
        }
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return HI_SUCCESS;
}

HI_S32 WIN_SetStepMode(HI_HANDLE hWin, HI_BOOL bStepMode)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);

        // s2 set enable
        if (pstWin->bUpState && pstWin->bEnable)
        {
            WIN_ERROR("Window is changing, can't set pause now!\n");
            return HI_ERR_VO_INVALID_OPT;
        }

        // set stepmode flag
        pstWin->bStepMode = bStepMode;

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
        {
            if (pstWin->hSlvWin)
            {
                WIN_SetStepMode(pstWin->hSlvWin, bStepMode);
            }
        }
    }
    else
    {
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return HI_SUCCESS;
}

HI_S32 WIN_SetStepPlay(HI_HANDLE hWin)
{
    WINDOW_S* pstWin;
    HI_BOOL bVirtual;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);

    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);

        // play next frame
        pstWin->bStepPlayNext = HI_TRUE;

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
        {
            if (pstWin->hSlvWin)
            {
                WIN_SetStepPlay(pstWin->hSlvWin);
            }
        }
    }
    else
    {
        WIN_ERROR("StepPlay Unsupport virtual window!\n");
        return HI_ERR_VO_WIN_UNSUPPORT;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_SetQuick(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin;
    HI_BOOL bVirtual = HI_FALSE;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        // s1 检查句柄合法性
        WinCheckWindow(hWin, pstWin);

        // s2 set enable
        if (pstWin->bUpState && pstWin->bEnable)
        {
            WIN_ERROR("Window is changing, can't set pause now!\n");
            return HI_ERR_VO_INVALID_OPT;
        }

        if ((pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
             && (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE))
        {
            return HI_SUCCESS;
        }

        // initial quickmode
        pstWin->bQuickMode = bEnable;
        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
        {
            if (pstWin->hSlvWin)
            {
                WIN_SetQuick(pstWin->hSlvWin, bEnable);
            }
        }
    }
    else
    {
        WIN_ERROR("SetQuick unsupported for virtual window!\n");
        return HI_ERR_VO_WIN_UNSUPPORT;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_GetQuick(HI_HANDLE hWin, HI_BOOL *pbEnable)
{
    WINDOW_S *pstWin;
    HI_BOOL  bVirtual = HI_FALSE;

    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckDeviceOpen();
        WinCheckWindow(hWin, pstWin);

        // initial quickmode
        *pbEnable = pstWin->bQuickMode;
    }
    else
    {
        WIN_ERROR("GetQuick unsupported for virtual window!\n");
        return HI_ERR_VO_WIN_UNSUPPORT;
    }

    return HI_SUCCESS;
}

/* only for virtual window */
HI_S32 WIN_SetExtBuffer(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf)
{
    return HI_ERR_VO_WIN_UNSUPPORT;
}
HI_S32 WIN_AttachSink(HI_HANDLE hWin, HI_HANDLE hSink)
{
    VIRTUAL_S *pstVirWin;
    HI_S32 s32Ret;

    WinCheckVirWindow(hWin, pstVirWin);

    s32Ret = WIN_VIR_AttachSink(pstVirWin, hSink);

    return s32Ret;
}

HI_S32 WIN_DetachSink(HI_HANDLE hWin, HI_HANDLE hSink)
{
    VIRTUAL_S *pstVirWin;
    HI_S32 s32Ret;

    WinCheckVirWindow(hWin, pstVirWin);

    s32Ret = WIN_VIR_DetachSink(pstVirWin, hSink);

    return s32Ret;
}

HI_S32 WIN_SetVirtualAttr(HI_HANDLE hWin,
                        HI_U32 u32Width,
                        HI_U32 u32Height,
                        HI_U32 u32FrmRate)
{
    VIRTUAL_S *pstVirWin;
    HI_S32 s32Ret;

    WinCheckVirWindow(hWin, pstVirWin);

    s32Ret = WIN_VIR_SetSize(pstVirWin, u32Width,u32Height);


    s32Ret |= WIN_VIR_SetFrmRate(pstVirWin, u32FrmRate);

    return s32Ret;
}
HI_S32 WIN_AcquireFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    VIRTUAL_S *pstVirWin;
    HI_S32 s32Ret;

    WinCheckVirWindow(hWin, pstVirWin);

    s32Ret = WIN_VIR_GetFrm(pstVirWin, pFrameinfo);

    // update sink acquire frame count
    pstVirWin->stFrameStat.u32SinkAcqTry++;
    if (s32Ret == HI_SUCCESS)
    {
        pstVirWin->stFrameStat.u32SinkAcqOK++;
    }

    return s32Ret;
}

HI_S32 WIN_ReleaseFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    VIRTUAL_S *pstVirWin;
    HI_S32 s32Ret;

    WinCheckVirWindow(hWin, pstVirWin);
    WinCheckNullPointer(pFrameinfo);

    s32Ret = WIN_VIR_RelFrm(pstVirWin, pFrameinfo);

    // update sink release frame count
    pstVirWin->stFrameStat.u32SinkRlsTry++;
    if (s32Ret == HI_SUCCESS)
    {
        pstVirWin->stFrameStat.u32SinkRlsOK++;
    }

    return s32Ret;
}

HI_S32 WIN_CreatStillFrame(HI_DRV_VIDEO_FRAME_S *pFrameinfo,HI_DRV_VIDEO_FRAME_S *pStillFrameInfo)
{
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = HI_NULL;
    HI_U32 datalen = 0, y_stride = 0, height = 0 ;
    DISP_MMZ_BUF_S    stMMZ_StillFrame;
    DISP_MMZ_BUF_S    stMMZ_Frame;
    HI_S32            nRet = 0;

    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(pFrameinfo->u32Priv);

    memset(&stMMZ_StillFrame,0,sizeof(DISP_MMZ_BUF_S));
    memset(&stMMZ_Frame,0,sizeof(DISP_MMZ_BUF_S));

    /*1:calculate alloc mem*/
    y_stride = pFrameinfo->stBufAddr[0].u32Stride_Y;
    height     = (HI_TRUE == pstPrivInfo->stCompressInfo.u32CompressFlag)
                     ? pstPrivInfo->stCompressInfo.s32CompFrameHeight : pFrameinfo->u32Height;

    if ( HI_DRV_PIX_FMT_NV21 == pFrameinfo->ePixFormat)
        datalen = height * y_stride * 3 / 2 ;
    else
        datalen = height * y_stride * 2 ;


    if(HI_SUCCESS != DISP_OS_MMZ_Alloc("VDP_StillFrame", VDP_MEM_TYPE_MMZ, datalen, VDP_MEM_ALIGN, &stMMZ_StillFrame))
    {
        WIN_ERROR(" Alloc StillFrame  failid(%x)\n",datalen);
        return HI_ERR_VO_MALLOC_FAILED;
    }

    /*2: creat still frame*/
    /*not support  Compress info*/
    stMMZ_Frame.u32StartPhyAddr = pFrameinfo->stBufAddr[0].u32PhyAddr_Y;
    stMMZ_Frame.bSmmu = HI_TRUE;
    stMMZ_Frame.u32Size= datalen;

    stMMZ_StillFrame.bSmmu = HI_TRUE;
    stMMZ_StillFrame.u32Size= datalen;

    nRet = DISP_OS_MMZ_Map(&stMMZ_StillFrame);
    if (HI_SUCCESS != nRet)
        goto __FAILURE_RELEASE;

    nRet = DISP_OS_MMZ_Map(&stMMZ_Frame);
    if (HI_SUCCESS != nRet)
        goto __EXIT_UNMAP;


    memcpy(pStillFrameInfo,pFrameinfo,sizeof(HI_DRV_VIDEO_FRAME_S));
    memcpy((void *)stMMZ_StillFrame.pu8StartVirAddr, (void *)stMMZ_Frame.pu8StartVirAddr,datalen);


    /*3: calculate still frame addr*/
    pStillFrameInfo->stBufAddr[0].u32PhyAddr_YHead = stMMZ_StillFrame.u32StartPhyAddr +(pFrameinfo->stBufAddr[0].u32PhyAddr_YHead - stMMZ_Frame.u32StartPhyAddr );
    pStillFrameInfo->stBufAddr[0].u32Stride_Y =  pFrameinfo->stBufAddr[0].u32Stride_Y;

    pStillFrameInfo->stBufAddr[0].u32PhyAddr_Y = stMMZ_StillFrame.u32StartPhyAddr +(stMMZ_Frame.u32StartPhyAddr - pFrameinfo->stBufAddr[0].u32PhyAddr_Y );

    pStillFrameInfo->stBufAddr[0].u32PhyAddr_C = stMMZ_StillFrame.u32StartPhyAddr + (pStillFrameInfo->u32Height*pStillFrameInfo->stBufAddr[0].u32Stride_Y);

    pStillFrameInfo->stBufAddr[0].u32PhyAddr_CrHead = stMMZ_StillFrame.u32StartPhyAddr +( pFrameinfo->stBufAddr[0].u32PhyAddr_CrHead - stMMZ_Frame.u32StartPhyAddr);
    pStillFrameInfo->stBufAddr[0].u32PhyAddr_Cr = stMMZ_StillFrame.u32StartPhyAddr +( pFrameinfo->stBufAddr[0].u32PhyAddr_Cr - stMMZ_Frame.u32StartPhyAddr );
    pStillFrameInfo->stBufAddr[0].u32Stride_Cr = pFrameinfo->stBufAddr[0].u32Stride_Cr;

    pStillFrameInfo->bStillFrame = HI_TRUE;

    DISP_OS_MMZ_UnMap(&stMMZ_StillFrame);
    DISP_OS_MMZ_UnMap(&stMMZ_Frame);

    return HI_SUCCESS;

__EXIT_UNMAP:
    DISP_OS_MMZ_UnMap(&stMMZ_StillFrame);

__FAILURE_RELEASE:
    DISP_OS_MMZ_Release(&stMMZ_StillFrame);
    return nRet;
}

HI_S32 Win_DestroyStillFrame(HI_DRV_VIDEO_FRAME_S *pStillFrameInfo)
{
    DISP_MMZ_BUF_S    stMMZ_StillFrame;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = HI_NULL;

    WinCheckNullPointer(pStillFrameInfo);
    if (pStillFrameInfo->bStillFrame)
    {
        memset((void*)&stMMZ_StillFrame, 0, sizeof(DISP_MMZ_BUF_S));
        pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(pStillFrameInfo->u32Priv);
        if (HI_TRUE == pstPrivInfo->stCompressInfo.u32CompressFlag)
        {
            stMMZ_StillFrame.u32StartPhyAddr = pStillFrameInfo->stBufAddr[0].u32PhyAddr_YHead;
        }
        else
        {
            stMMZ_StillFrame.u32StartPhyAddr = pStillFrameInfo->stBufAddr[0].u32PhyAddr_Y;
        }
        //printk("release 0x%x\n",stMMZ_StillFrame.u32StartPhyAddr);
         DISP_OS_MMZ_Release(&stMMZ_StillFrame);
         return HI_SUCCESS;
    }

    return HI_FAILURE;
}



HI_S32 WIN_SendFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    HI_S32 nRet;
    HI_DRV_VIDEO_FRAME_S StillFrameInfo;
    WinCheckNullPointer(pFrameInfo);

    nRet = WIN_CreatStillFrame(pFrameInfo,&StillFrameInfo);
    if ( HI_SUCCESS != nRet )
    {
        WIN_ERROR(" WIN_CreatStillFrame  failid(%x)\n",nRet);
        return nRet;
    }

    return WinQueueFrame( hWin, &StillFrameInfo, HI_NULL);
}

HI_S32 Win_DebugGetHandle(HI_DRV_DISPLAY_E enDisp, WIN_HANDLE_ARRAY_S *pstWin)
{
    HI_S32 i;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckNullPointer(pstWin);

    DISP_MEMSET(pstWin, 0, sizeof(WIN_HANDLE_ARRAY_S));

    pstWin->u32WinNumber = 0;

    for(i=0; i<WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[(HI_U32)enDisp][i])
        {
            pstWin->ahWinHandle[pstWin->u32WinNumber] = (HI_HANDLE)(stDispWindow.pstWinArray[(HI_U32)enDisp][i]->u32Index);
            pstWin->u32WinNumber++;
        }
    }

    return HI_SUCCESS;
}


/*
HI_S32 WinUpdatePlayInfo(HI_DRV_WIN_PLAY_INFO_S *ptPlay, HI_U32 u32Rate)
{
    ptPlay->u32DispRate = u32Rate;
    return HI_SUCCESS;
}
*/



HI_S32 WinAcquireFrame(WINDOW_S *pstWin)
{
#if 0
    HI_DRV_VIDEO_FRAME_S stNewFrame;
    HI_U32 u32BufId;
    HI_S32 nRet = HI_SUCCESS;

    nRet = BQ_GetWriteNode(&pstWin->stBuffer.stBP, &u32BufId);
    if (nRet)
    {
        return HI_ERR_VO_BUFQUE_FULL;
    }

    if (pstWin->stSource.pfAcqFrame)
    {
        nRet = pstWin->stSource.pfAcqFrame(pstWin->stSource.hSrc, &stNewFrame);
        if (nRet)
        {
            WIN_ERROR("WIN Release Frame failid\n");
            return HI_ERR_VO_FRAME_RELEASE_FAILED;
        }
    }

    // s2 get state
    nRet = WinCheckFrame(&stNewFrame);
    if (nRet)
    {
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    if (!stNewFrame.u32PlayTime)
    {
        if (pstWin->stSource.pfRlsFrame && stNewFrame.bToRelease)
        {
            nRet = pstWin->stSource.pfRlsFrame(pstWin->stSource.hSrc, &stNewFrame);
            if (nRet)
            {
                WIN_ERROR("WIN Release Frame failid\n");
            }

            BQ_ReleaseRecoder(&pstWin->stBufQue, BQ_RELEASE_DISCARD);
        }
        else
        {
            // todo
        }
    }

    nRet = BQ_PutWriteNode(&pstWin->stBufQue, &stNewFrame, BUF_FRAME_SRC_NORMAL);
#endif

    return HI_SUCCESS;
}


//#define WIN_DEBUG_PRINT_RELEASE 1
HI_S32 s_ResetPrint = 0;

HI_VOID ISR_WinReleaseUSLFrame(WINDOW_S *pstWin)
{
    HI_DRV_WIN_SRC_INFO_S *pstSource = &pstWin->stCfg.stSource;
    HI_DRV_VIDEO_FRAME_S stRlsFrm;
    HI_S32 nRet;

    if (pstWin->stCfg.stSource.hSrc == (HI_HANDLE)pstWin->u32Index)
        return;

     /* release useless frame */
    nRet = WinBufferGetULSFrame(&pstWin->stBuffer, &stRlsFrm);
    while(!nRet)
    {
        if (stRlsFrm.bStillFrame)
        {
            WIN_DestroyStillFrame(&stRlsFrm);
        }
        else if (pstSource->pfRlsFrame)
        {
            if ((pstSource->hSecondSrc != HI_INVALID_HANDLE)
                && (HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL == stRlsFrm.enSrcFrameType))
            {
                pstSource->pfRlsFrame(pstSource->hSecondSrc, &stRlsFrm);
            }
            else if(pstSource->hSrc != HI_INVALID_HANDLE)
            {
                pstSource->pfRlsFrame(pstSource->hSrc, &stRlsFrm);
            }
            else
            {
                WIN_ERROR("Cannot Release USL frame,invalid hSrc or FrameType! FrameType %d hSrc %#x\n",
                                stRlsFrm.enSrcFrameType,
                                pstSource->hSrc);
            }
#ifdef WIN_DEBUG_PRINT_RELEASE
            if (s_ResetPrint)
            {
                printk("Rel 006 fid=%d, addr=0x%x\n",
                        stRlsFrm.u32FrameIndex,
                        stRlsFrm.stBufAddr[0].u32PhyAddr_Y);
            }
#endif
        }

        nRet = WinBufferGetULSFrame(&pstWin->stBuffer, &stRlsFrm);
    }

    return;
}

HI_DRV_VIDEO_FRAME_S *ISR_SlaveWinGetConfigFrame(WINDOW_S *pstWin)
{
    WIN_ERROR("Invalid Code branch\n");

    return HI_NULL;
#if 0
    WINDOW_S *pstMstWin = (WINDOW_S *)(pstWin->pstMstWin);
    HI_DRV_VIDEO_FRAME_S *pstDstF, *pstRefF, *pstNew;

    // get master window crrent configed frame
    pstDstF = WinBuf_GetConfigedFrame(&pstMstWin->stBuffer.stWinBP);
    if (!pstDstF)
    {
        return HI_NULL;
    }

    // get slave window displayed frame
    pstRefF = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);

    //pstNew = WinBuf_GetFrameByMaxID(&pstWin->stBuffer.stWinBP, pstRefF, RefID, enDstField);
    pstNew = WinBuf_GetFrameByDstFrame(&pstWin->stBuffer.stWinBP, pstDstF, pstRefF);
#endif
}


HI_VOID WinUpdateDispInfo(WINDOW_S *pstWin, HI_DISP_DISPLAY_INFO_S * pstDsipInfo)
{
    pstWin->stDispInfo.u32RefreshRate = pstDsipInfo->u32RefreshRate;
    pstWin->stDispInfo.bIsInterlace   = pstDsipInfo->bInterlace;
    pstWin->stDispInfo.bIsBtm          = pstDsipInfo->bIsBottomField;
    pstWin->stDispInfo.stWinCurrentFmt = pstDsipInfo->stFmtResolution;

    return;
}

WIN_DISP_INFO_S *WinGetDispInfoByHandle(HI_HANDLE hWin)
{
    WINDOW_S *pstWin;

    pstWin = WinGetWindow(hWin);

    if (pstWin)
    {
        return &pstWin->stDispInfo;
    }

    return HI_NULL;
}

HI_VOID WinTestFrameMatch(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_DISP_DISPLAY_INFO_S * pstDsipInfo)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrame->u32Priv[0]);

    //if display work at interlace mode and frame rate equale to display refresh rate,
    // dectect whether top-field video frame output at top-field time
    if (pstDsipInfo->bInterlace == HI_TRUE)
    {
         if ( ((pstPriv->eOriginField == HI_DRV_FIELD_TOP)
                &&(pstDsipInfo->bIsBottomField == HI_FALSE) )
            || ((pstPriv->eOriginField == HI_DRV_FIELD_BOTTOM)
                &&(pstDsipInfo->bIsBottomField == HI_TRUE) )
            )
        {
            pstWin->u32TBNotMatchCount++;
        }
    }

    return;
}
HI_VOID Win_UpdateHdFramInfo(WIN_HAL_PARA_S *pstLayerPara,WIN_CONFIG_S *pstCfg,HI_BOOL bSecure)
{
    WBC_HD_WIN_INFOR_S  stHDWinInfo;

    memset((HI_VOID*)&stHDWinInfo, 0, sizeof(WBC_HD_WIN_INFOR_S));

    if ( VDP_LAYER_VID0 == pstLayerPara->u32LayerNO )
    {
        stHDWinInfo.bV0Secure = bSecure;
        stHDWinInfo.bV0Mute = pstLayerPara->bRegionMute;
        stHDWinInfo.bV0RegionEnable = pstLayerPara->bRegionEnable;
        stHDWinInfo.stV0DispRect = pstLayerPara->stDisp;
        stHDWinInfo.stV0VideoRect = pstLayerPara->stVideo;
        stHDWinInfo.stV0InRect = pstLayerPara->stIn;

        stHDWinInfo.enV0WinSrcColorSpace = pstCfg->enFrameCS;
        stHDWinInfo.enV0WinDestColorSpace =  pstCfg->enOutCS;
    }
    else if ( VDP_LAYER_VID1 == pstLayerPara->u32LayerNO )
    {
        stHDWinInfo.bV1Secure = bSecure;
        stHDWinInfo.bV1Mute = pstLayerPara->bRegionMute;
        stHDWinInfo.bV1RegionEnable = pstLayerPara->bRegionEnable;
    }
    else
    {
        /*do nothing\n*/
    }
    stHDWinInfo.u32WinNum = WinGetHDWinNum();

    ISOGENY_UpDateHDWinFramInfo(&stHDWinInfo,pstLayerPara->u32LayerNO);

}


HI_VOID WinGetActualOffsetAccordingUserSetting(HI_DRV_DISPLAY_E enDisp,
                                                        HI_DRV_DISP_OFFSET_S  *pstUserSettingOffset,
                                                        HI_DRV_DISP_OFFSET_S  *pstActualOffset,
                                                        HI_RECT_S *pstOutRect)
{

#ifndef HI_VO_OFFSET_EFFECTIVE_WHEN_WIN_FULL
    HI_RECT_S  stCanvas = {0};

    (HI_VOID)DISP_GetVirtScreen(enDisp, &stCanvas);

    if (((pstOutRect->s32Width == 0)
        || (pstOutRect->s32Height== 0))
        || ((pstOutRect->s32Width == stCanvas.s32Width)
        &&(pstOutRect->s32Height == stCanvas.s32Height)))
    {
        memset((void*)pstActualOffset, 0, sizeof(HI_DRV_DISP_OFFSET_S));
    }
    else
    {
        *pstActualOffset = *pstUserSettingOffset;
    }

#else

    *pstActualOffset = *pstUserSettingOffset;

#endif

    return;
}

HI_VOID ISR_WinConfigFrameMute(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrame, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
    WIN_HAL_PARA_S stLayerPara;
    HI_DRV_DISP_OFFSET_S stOffsetRect = {0};

    memset((void*)&stLayerPara, 0, sizeof(WIN_HAL_PARA_S));

    stLayerPara.en3Dmode = pstInfo->stDispInfo.eDispMode;
    stLayerPara.bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;

    memset((HI_VOID*)&stLayerPara.stHdcpIntermediateRect, 0, sizeof(HI_RECT_S));
    stLayerPara.bHdcpDegradePQ = HI_FALSE;

    pstWin->stCfg.enOutCS = pstInfo->stDispInfo.eColorSpace;
    stLayerPara.eOutPutColorSpace = pstWin->stCfg.enOutCS;

    /*stOutRect may equal to stIn in most situation.*/
    stLayerPara.stDisp    = pstAttr->stOutRect;
    stLayerPara.stVideo = pstAttr->stOutRect;

    /*add for mute area  8 align*/
    stLayerPara.stVideo.s32X &= HI_WIN_IN_RECT_X_ALIGN;
    stLayerPara.stVideo.s32Y &= HI_WIN_IN_RECT_Y_ALIGN;
    stLayerPara.stVideo.s32Width &= HI_WIN_IN_RECT_WIDTH_ALIGN;
    stLayerPara.stVideo.s32Height &= HI_WIN_IN_RECT_HEIGHT_ALIGN;

    stLayerPara.stIn.s32Width = stLayerPara.stVideo.s32Width;
    stLayerPara.stIn.s32Height = stLayerPara.stVideo.s32Height;

    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);
    stLayerPara.u32RegionNum =    pstWin->u32VideoRegionNo;
    stLayerPara.bRegionMute = HI_TRUE;
    stLayerPara.u32LayerNO =    pstWin->u32VideoLayer;
    stLayerPara.bRegionEnable = HI_TRUE;
    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);


    if (pstWin->bVirtScreenMode == HI_TRUE)
    {
        WinGetActualOffsetAccordingUserSetting(pstWin->enDisp,
                                               &stLayerPara.pstDispInfo->stOffsetInfo,
                                               &stOffsetRect,
                                               &pstWin->stCfg.stAttrBuf.stOutRect);
    }
    else
    {
        memset((void*)&stOffsetRect, 0, sizeof(HI_DRV_DISP_OFFSET_S));
    }


    /*deal with out of  screen scene.*/
    (HI_VOID)Win_Revise_OutOfScreenWin_OutRect(&stLayerPara.stIn,
                                               &stLayerPara.stVideo,
                                               &stLayerPara.stDisp,
                                               stLayerPara.pstDispInfo->stFmtResolution,
                                               stOffsetRect,
                                               &stLayerPara);

     /*get intermediate Rect to degrade the PQ  as a matter of TEE monitor.*/
    Win_Policy_GetPQDegradingConfigFromTEE(&stLayerPara.stIn,
                                           &stLayerPara.stVideo,
                                           pstWin->u32VideoLayer,
                                           &stLayerPara.bHdcpDegradePQ,
                                           &stLayerPara.stHdcpIntermediateRect);

    stLayerPara.enLayerFrostMode = HI_DRV_WINDOW_FROST_CLOSE;

    Win_PqProcess(pstWin, pstFrame,&stLayerPara.stVideo, &pstInfo->stDispInfo, HI_FALSE);

    (HI_VOID)pstWin->stVLayerFunc.PF_SetFramePara(pstWin->u32VideoLayer, &stLayerPara);

    Win_UpdateHdFramInfo(&stLayerPara,&pstWin->stCfg,HI_FALSE);

    return;
}

HI_S32  RWZB_GetDATEParaIndex(HI_S32 u32RwzbType)
{
    HI_U32 u32Index;

    switch (u32RwzbType)
    {
        case FIDELITY_480I_YPBPR:
        case FIDELITY_576I_YPBPR:
            u32Index = 1;
            break;

        case FIDELITY_SKN:
        case FIDELITY_ZDN:
            u32Index = 2;
            break;
        case FIDELITY_MOTO_CVBS:
            u32Index = 3;
            break;
        case FIDELITY_033:
            u32Index = 4;
            break;
        case FIDELITY_MATRIX525:
            u32Index = 5;
            break;


        default:
            u32Index = 0;
            break;
    }
    return u32Index;
}

extern HI_BOOL DISP_IsFollowed(HI_DRV_DISPLAY_E enDisp);

HI_VOID Win_ReviseInRect(HI_RECT_S *pstDstRect,HI_RECT_S *pstSrctRect)
{

    if ((0 == pstDstRect->s32Width) || (0 == pstDstRect->s32Height))
    {
        pstDstRect->s32Width = pstSrctRect->s32Width;
        pstDstRect->s32Height = pstSrctRect->s32Height;
    }


     if ( pstDstRect->s32Width > pstSrctRect->s32Width)
    {
        pstDstRect->s32Width =  pstSrctRect->s32Width;
        //WIN_ERROR("inrect s32Width outof srource rect!");
    }

     if ( pstDstRect->s32Height > pstSrctRect->s32Height)
    {
        pstDstRect->s32Height =  pstSrctRect->s32Height;
        //WIN_ERROR("inrect s32Height outof srource rect!");
    }

     if ( pstDstRect->s32Y > pstSrctRect->s32Height -64 )
    {
        pstDstRect->s32Y =    pstSrctRect->s32Height -64;

    }
      if ( pstDstRect->s32X > pstSrctRect->s32Width -64 )
     {
         pstDstRect->s32X =  pstSrctRect->s32Width -64;

     }
      if ( (pstDstRect->s32Y + pstDstRect->s32Height) > pstSrctRect->s32Height )
      {
          pstDstRect->s32Height = pstSrctRect->s32Height - pstDstRect->s32Y;
          //WIN_ERROR("Invalid  inrect!\n");
      }


    if ( (pstDstRect->s32X + pstDstRect->s32Width) > pstSrctRect->s32Width )
    {
        pstDstRect->s32Width = pstSrctRect->s32Width - pstDstRect->s32X;
        //WIN_ERROR("Invalid  inrect!\n");
    }

    if ( pstDstRect->s32Width < 64)
       pstDstRect->s32Width = 64;

     if ( pstDstRect->s32Height < 64)
       pstDstRect->s32Height = 64;
}

HI_VOID Win_AspRatioProcess(VDP_ALG_RATIO_DRV_PARA_S *pstAspDrvPara,VDP_ALG_RATIO_OUT_PARA_S *pstAspCfg)
{

    VDP_ALG_RATIO_DRV_PARA_S  stAspCalPara = *pstAspDrvPara;
    HI_S32 sr_w, sr_h;
    HI_S32 in_w, in_h;

    if (HI_TRUE == pstAspDrvPara->stUsrAsp.bUserDefAspectRatio)
    {
        sr_w = pstAspDrvPara->stUsrAsp.u32UserAspectWidth;
        sr_h = pstAspDrvPara->stUsrAsp.u32UserAspectHeight;
    }
    else
    {
        sr_w = stAspCalPara.AspectWidth;
        sr_h = stAspCalPara.AspectHeight;
    }

    in_h =  (pstAspDrvPara->stInWnd.s32Height * sr_h)/pstAspDrvPara->stSrcRect.s32Height;
    in_w =  (pstAspDrvPara->stInWnd.s32Width * sr_w)/pstAspDrvPara->stSrcRect.s32Width;

    /*cal src crop rect aspratio!*/
    stAspCalPara.AspectWidth = in_w ;
    stAspCalPara.AspectHeight = in_h;

    VDP_ALG_RATIO_RatioProcess(pstAspDrvPara,pstAspCfg);

    if (pstAspCfg->bEnCrop)
    {
        /*cal inRect */
        pstAspDrvPara->stInWnd.s32X = (pstAspCfg->stCropWnd.s32X * pstAspDrvPara->stSrcRect.s32Width / pstAspCfg->u32ZmeW);
        pstAspDrvPara->stInWnd.s32Width =(pstAspCfg->stCropWnd.s32Width * pstAspDrvPara->stSrcRect.s32Width /pstAspCfg->u32ZmeW);
        pstAspDrvPara->stInWnd.s32Y = (pstAspCfg->stCropWnd.s32Y * pstAspDrvPara->stSrcRect.s32Height / pstAspCfg->u32ZmeH);
        pstAspDrvPara->stInWnd.s32Height = (pstAspCfg->stCropWnd.s32Height * pstAspDrvPara->stSrcRect.s32Height / pstAspCfg->u32ZmeH);

    }

}

extern  S_VDP_REGS_TYPE *g_pstVdpBaseAddr;
HI_VOID Win_PqProcess(WINDOW_S *pstWin,
                           HI_DRV_VIDEO_FRAME_S *pstFrame,
                           HI_RECT_S *pstVideoRect,
                           const HI_DISP_DISPLAY_INFO_S *pstDispInfo,
                           HI_BOOL bHdrMode)
{
    HI_VDP_PQ_INFO_S stTimingInfo;
    HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    DISP_INTF_OPERATION_S stFunction = {0};
    HI_U32 u32PixelRepeateTimes = 0;

    (HI_VOID)pstWin->stVLayerFunc.PF_GetCapability(pstWin->u32VideoLayer,&stVideoLayerCap);

    /*this is the current value.*/
    stTimingInfo.enLayerId  = pstWin->u32VideoLayer;
    if (HI_NULL == pstFrame)
    {
        stTimingInfo.u32Width = pstVideoRect->s32Width;
        stTimingInfo.u32Height = pstVideoRect->s32Height;
    }
    else
    {
        stTimingInfo.u32Width = ((HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0])->stVideoOriginalInfo.u32Width;
        stTimingInfo.u32Height = ((HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0])->stVideoOriginalInfo.u32Height;
    }

    stTimingInfo.bSRState     = pstWin->stMiscInfor.bWinSrEnableCurrent;
    stTimingInfo.bPartUpdate = HI_TRUE;

    stTimingInfo.bIsogenyMode = DISP_Check_IsogenyMode();
    stTimingInfo.bHdrMode     = bHdrMode;

    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)pstWin->enDisp].stFmtRect = pstDispInfo->stFmtResolution;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)pstWin->enDisp].u32RefreshRate = pstDispInfo->u32RefreshRate;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)pstWin->enDisp].bProgressive = pstDispInfo->bInterlace ? HI_FALSE:HI_TRUE;
    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)pstWin->enDisp].enOutColorSpace = pstDispInfo->eColorSpace;

    (HI_VOID)DISP_HAL_GetOperation(&stFunction);
    DISP_ASSERT(HI_NULL != stFunction.PF_GetPixelRepeatTimesAccordingFmt);

    stFunction.PF_GetPixelRepeatTimesAccordingFmt(pstWin->enDisp,
                                                  pstDispInfo->eFmt,
                                                  &u32PixelRepeateTimes);

    stTimingInfo.stVideo.s32X      = pstVideoRect->s32X * u32PixelRepeateTimes;
    stTimingInfo.stVideo.s32Y      = pstVideoRect->s32Y;
    stTimingInfo.stVideo.s32Width  = pstVideoRect->s32Width * u32PixelRepeateTimes;
    stTimingInfo.stVideo.s32Height = pstVideoRect->s32Height;

    stTimingInfo.stChannelTiming[(HI_PQ_DISPLAY_E)pstWin->enDisp].stFmtRect.s32Width *= u32PixelRepeateTimes;

    if (DISP_STEREO_NONE == pstDispInfo->eDispMode)
    {
        stTimingInfo.b3dType = HI_FALSE;
    }
    else
    {
        stTimingInfo.b3dType = HI_TRUE;
    }

    DRV_PQ_UpdateVdpPQ((HI_PQ_DISPLAY_E)pstWin->enDisp, &stTimingInfo, (S_VDP_REGS_TYPE *)g_pstVdpBaseAddr);

    /*save the value ,for next judgement.*/
    pstWin->stMiscInfor.stFrameOriginalRect.s32Width  = stTimingInfo.u32Width;
    pstWin->stMiscInfor.stFrameOriginalRect.s32Height  = stTimingInfo.u32Height;
    pstWin->stMiscInfor.stWinOutRect.s32Width  =  pstAttr->stOutRect.s32Width;
    pstWin->stMiscInfor.stWinOutRect.s32Height = pstAttr->stOutRect.s32Height;
    pstWin->stMiscInfor.bWinSrEnableLast       =  stTimingInfo.bSRState;

    return;
    /*delete dci policy--20160222.*/
}


HI_VOID WinCropRect2InRect (HI_DRV_CROP_RECT_S *pstCropRect,HI_RECT_S *pstInRect,HI_U32 u32SrcW ,HI_U32 u32SrcH)
{
    if ((0 == u32SrcW) || (0 == u32SrcH))
        return ;

    if(((pstCropRect->u32LeftOffset + pstCropRect->u32RightOffset) + WIN_FRAME_MIN_WIDTH > u32SrcW)
        ||((pstCropRect->u32TopOffset + pstCropRect->u32BottomOffset)  + WIN_FRAME_MIN_HEIGHT > u32SrcH))
    {
        WIN_ERROR("u32LeftOffset(%d) add u32RightOffset(%d) is too large(%d)\n"
            "Or TopOffset(%d) add u32BottomOffset(%d) is too large(%d)\n",
            pstCropRect->u32LeftOffset, pstCropRect->u32RightOffset, u32SrcW,
            pstCropRect->u32TopOffset, pstCropRect->u32BottomOffset, u32SrcH);
        return;
    }

    pstInRect->s32X = pstCropRect->u32LeftOffset;
    pstInRect->s32Y = pstCropRect->u32TopOffset;
    pstInRect->s32Width  = u32SrcW - (pstCropRect->u32LeftOffset + pstCropRect->u32RightOffset);
    pstInRect->s32Height = u32SrcH - (pstCropRect->u32TopOffset + pstCropRect->u32BottomOffset);

}

HI_VOID Win_GenerateConfigInfo(WINDOW_S *pstWin,
                               HI_DRV_VIDEO_FRAME_S *pstFrame,
                               const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo,
                               WIN_HAL_PARA_S *pstLayerPara)
{
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap = {0};
    HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
    HI_RECT_S stSourceFrameRect = {0};
    HI_CHIP_TYPE_E   enChipType = HI_CHIP_TYPE_BUTT;
    HI_CHIP_VERSION_E enChipVersion = HI_CHIP_VERSION_BUTT;
    DISP_INTF_OPERATION_S stFunction = {0};

    /*field read for v0 or v3.*/
    HI_BOOL bHdInterfaceInterleave = HI_FALSE;
    HI_BOOL bBtm  =    HI_FALSE;
    HI_U32  u32Vcnt = 0;
    HI_RECT_S  stFinalVideoPosition;/*out out video Rect*/
    HI_RECT_S  stFinalDisPosition;/*out out win Rect*/

    VDP_ALG_RATIO_DRV_PARA_S  stDrvPara;
    VDP_ALG_RATIO_OUT_PARA_S  stOutPara;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

    HI_DRV_DISP_OFFSET_S  stOffsetTmp = {0};
    HI_RECT_S stFmtResolution = {0};

    memset(&stDrvPara,0,sizeof(VDP_ALG_RATIO_DRV_PARA_S));
    memset(&stOutPara,0,sizeof(VDP_ALG_RATIO_OUT_PARA_S));
    memset(&stFinalDisPosition,0,sizeof(HI_RECT_S));
    memset(&stFinalVideoPosition,0,sizeof(HI_RECT_S));

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);
    (HI_VOID)pstWin->stVLayerFunc.PF_GetCapability(pstWin->u32VideoLayer,&stVideoLayerCap);

    /* first  get the basic setting of layerparam. */
    memset(pstLayerPara,0x0,sizeof(WIN_HAL_PARA_S));

    /*tnr alg need 2 buffers, we make a alternating use.*/
    pstLayerPara->bTnrOnVdp = Win_CheckTnrLocateOnVdp(pstWin, &pstFrame->stDispRect);
    if (pstWin->stWinAlgMMZ[0].bReady == HI_TRUE)
    {
        pstLayerPara->stLayerAlgMmz = pstWin->stWinAlgMMZ[0].stVideoAlgMmz;
        pstWin->stWinAlgMMZ[0].bReady = HI_FALSE;
    }
    else
    {
        pstLayerPara->stLayerAlgMmz = pstWin->stWinAlgMMZ[1].stVideoAlgMmz;
        pstWin->stWinAlgMMZ[0].bReady = HI_TRUE;
    }

    pstLayerPara->bDolbyMode = (pstWin->enWinMode == HI_DRV_WINDOW_MODE_DOLBY) ? HI_TRUE:HI_FALSE;
    pstLayerPara->en3Dmode = pstInfo->stDispInfo.eDispMode;
    pstLayerPara->bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;
    pstLayerPara->pstFrame = pstFrame;

    /*stOutRect may equal to stIn in most situation.*/
    pstLayerPara->pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);

    pstLayerPara->u32RegionNum =    pstWin->u32VideoRegionNo;
    pstLayerPara->u32LayerNO =    pstWin->u32VideoLayer;
    pstLayerPara->bRegionMute  = 0;
    pstLayerPara->bRegionEnable = HI_TRUE;

    if (pstWin->u32VideoLayer == VDP_LAYER_VID0)
    {
        if (HI_TRUE == pstWin->stUsingAttr.bUseCropRect)
        {
            WinCropRect2InRect( &pstWin->stUsingAttr.stCropRect,&pstLayerPara->stIn,pstFrame->u32Width, pstFrame->u32Height);
        }
        else
        {
            pstLayerPara->stIn = pstWin->stUsingAttr.stInRect;
            Win_ReviseInRect(&pstLayerPara->stIn,&pstFrame->stDispRect);
        }
    }
    else
    {
        pstLayerPara->stIn = pstFrame->stDispRect;
    }

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstFrame->u32Priv;
    if ((!pstPriv->bAspectProcessed) )
    {
        /*window phy OutRect*/
        stFinalDisPosition= pstWin->stUsingAttr.stOutRect;

        /*need VDP AspRatio process*/
        /*out out win Rect*/
        stDrvPara.AspectWidth= pstFrame->u32AspectWidth;
        stDrvPara.AspectHeight = pstFrame->u32AspectHeight;
        stDrvPara.DeviceHeight = pstInfo->stDispInfo.stAR.u32ARh;
        stDrvPara.DeviceWidth= pstInfo->stDispInfo.stAR.u32ARw;
        stDrvPara.eAspMode = pstWin->stUsingAttr.enARCvrs;

        if ((!pstWin->stUsingAttr.stCustmAR.u32ARw ) || (!pstWin->stUsingAttr.stCustmAR.u32ARh))
        {
            stDrvPara.stUsrAsp.bUserDefAspectRatio = HI_FALSE;
        }
        else
        {
            stDrvPara.stUsrAsp.bUserDefAspectRatio = HI_TRUE;
        }

        stDrvPara.stUsrAsp.u32UserAspectWidth = pstWin->stUsingAttr.stCustmAR.u32ARw;
        stDrvPara.stUsrAsp.u32UserAspectHeight = pstWin->stUsingAttr.stCustmAR.u32ARh;

        stDrvPara.stSrcRect = pstFrame->stDispRect;
        stDrvPara.stInWnd= pstLayerPara->stIn;
        stDrvPara.stOutWnd = stFinalDisPosition;
        stDrvPara.stScreen = pstInfo->stDispInfo.stFmtResolution;

        if ((0 == stDrvPara.stOutWnd.s32Width) || (0 == stDrvPara.stOutWnd.s32Height))
        {
            stDrvPara.stOutWnd.s32Width = stDrvPara.stScreen.s32Width;
            stDrvPara.stOutWnd.s32Height = stDrvPara.stScreen.s32Height;
        }

        if ((stDrvPara.eAspMode == HI_DRV_ASP_RAT_MODE_FULL
                || stDrvPara.eAspMode == HI_DRV_ASP_RAT_MODE_BUTT)
            && stDrvPara.stOutWnd.s32Width == 720
            && stDrvPara.stInWnd.s32Width== 704
            && (stDrvPara.stInWnd.s32Height == 576
                || stDrvPara.stInWnd.s32Height == 480)
            && stDrvPara.stInWnd.s32Height
                == stDrvPara.stOutWnd.s32Height)
        {
            stDrvPara.stOutWnd.s32X = 8;
            stDrvPara.stOutWnd.s32Y = 0;
            stDrvPara.stOutWnd.s32Width = stDrvPara.stInWnd.s32Width;
            stDrvPara.stOutWnd.s32Height = stDrvPara.stInWnd.s32Height;
        }
        else
        {
            Win_AspRatioProcess(&stDrvPara,&stOutPara);
        }

        stFinalVideoPosition = stDrvPara.stOutWnd;
        pstLayerPara->stIn = stDrvPara.stInWnd;
    }
    else
    {
        if ( VDP_LAYER_VID3 == pstWin->u32VideoLayer)
        {
            HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)(&(pstFrame->u32Priv[0]));

            stFinalDisPosition = pstFramePrivate->stWBC2SDDispRect;
        }
        else
        {
            stFinalDisPosition= pstWin->stUsingAttr.stOutRect;
        }


        stFinalVideoPosition = pstFrame->stDispRect;
        stFinalVideoPosition.s32X = stFinalDisPosition.s32X;
        stFinalVideoPosition.s32Y = stFinalDisPosition.s32Y;
        stFinalDisPosition = stFinalVideoPosition;

    }

    /*this rect just for post scaler dealer.*/
    stSourceFrameRect.s32X = 0;
    stSourceFrameRect.s32Y= 0;
    stSourceFrameRect.s32Width  = pstFrame->u32Width;
    stSourceFrameRect.s32Height = pstFrame->u32Height;

    if (!pstWin->enDisp)
    {
        pstAttr->stOutRect  = pstFrame->stDispRect;
    }

    pstLayerPara->stDisp   = stFinalDisPosition;
    pstLayerPara->stVideo  = stFinalVideoPosition;

    if ((pstFrame->stDispRect.s32Width != pstLayerPara->stVideo.s32Width)
         || (pstFrame->stDispRect.s32Height!= pstLayerPara->stVideo.s32Height))
    {
        if (!stVideoLayerCap.bZme)
        {
            /*we must confirm that v1, should not configure the zme,
                    but the ifir is needed ,so we have to configure it or not? */
                 pstLayerPara->bRegionMute = 1;

        }
    }

    if (((pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP)
                || (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP))
            && (stVideoLayerCap.bDcmp != HI_TRUE))
    {
        /*frame is compressed but the layer has no capability*/
       pstLayerPara->bRegionMute = 1;
    }

    {

        pstLayerPara->stOriRect.s32Width = pstPriv->stVideoOriginalInfo.u32Width;
        pstLayerPara->stOriRect.s32Height = pstPriv->stVideoOriginalInfo.u32Height;
        pstLayerPara->u32Fidelity = pstPriv->u32Fidelity;
    }

    /*get current hd's p/I  attribution.*/
    bHdInterfaceInterleave = pstInfo->stDispInfo.bInterlace;

    (HI_VOID)DISP_HAL_GetOperation(&stFunction);
    if(HI_NULL == stFunction.FP_GetChnBottomFlag)
    {
        WIN_INFO("stFunction.FP_GetChnBottomFlag is NULL!\n");
        return ;
    }
    stFunction.FP_GetChnBottomFlag(pstWin->enDisp, &bBtm, &u32Vcnt);

    if (pstWin->bVirtScreenMode == HI_TRUE)
    {
        WinGetActualOffsetAccordingUserSetting(pstWin->enDisp,
                                               &pstLayerPara->pstDispInfo->stOffsetInfo,
                                               &stOffsetTmp,
                                               &pstWin->stCfg.stAttrBuf.stOutRect);
    }
    else
    {
        memset((void*)&stOffsetTmp, 0, sizeof(HI_DRV_DISP_OFFSET_S));
    }

     stFmtResolution = pstLayerPara->pstDispInfo->stFmtResolution;

    /*
        *since we support picture moved out of screen, so we should give a revise to the window ,
        *both inrect and outrect.
        */
    (HI_VOID)Win_Revise_OutOfScreenWin_OutRect(&pstLayerPara->stIn, &pstLayerPara->stVideo,&pstLayerPara->stDisp,
                               stFmtResolution,
                               stOffsetTmp,
                               pstLayerPara);

    /*get intermediate Rect to degrade the PQ  as a matter of TEE monitor.*/
    Win_Policy_GetPQDegradingConfigFromTEE(&pstLayerPara->stIn,
                                           &pstLayerPara->stVideo,
                                           pstWin->u32VideoLayer,
                                           &pstLayerPara->bHdcpDegradePQ,
                                           &pstLayerPara->stHdcpIntermediateRect);

    pstLayerPara->enLayerFrostMode = HI_DRV_WINDOW_FROST_CLOSE;
    return;
}

HI_VOID ISR_WinConfigFrameNormal(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrame, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
#if defined(DAC_TYPE_SYNOPSYS)
    HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
#endif

    WIN_HAL_PARA_S stLayerPara;
    HI_DRV_DISP_COLOR_SETTING_S stColor;
    HI_U32 u32Fidelity;
    DISP_INTF_OPERATION_S *pstDispOpt;
    HI_RECT_S stCanvas;
    WBC_HD_WIN_INFOR_S stHDWinInfo;

    memset(&stHDWinInfo,0,sizeof(WBC_HD_WIN_INFOR_S));

    memset(&stCanvas,0,sizeof(HI_RECT_S));
    memset(&stColor,0,sizeof(HI_DRV_DISP_COLOR_SETTING_S));

    pstDispOpt = DISP_HAL_GetOperationPtr();

    pstWin->stCfg.eDispMode = pstInfo->stDispInfo.eDispMode;
    pstWin->stCfg.bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;

    (HI_VOID) Win_GenerateConfigInfo(pstWin, pstFrame, pstInfo, &stLayerPara);

    stColor.enInCS  = ((HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0])->eColorSpace;
    stColor.enOutCS = pstInfo->stDispInfo.eColorSpace;
    u32Fidelity     = ((HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0])->u32Fidelity;


    if (stColor.enInCS == HI_DRV_CS_UNKNOWN)
    {
        stColor.enInCS = HI_DRV_CS_BT709_YUV_LIMITED;
    }

    if (stColor.enOutCS == HI_DRV_CS_UNKNOWN)
    {
        stColor.enOutCS = HI_DRV_CS_BT709_YUV_LIMITED;
    }

    pstWin->stCfg.enFrameCS = stColor.enInCS;
    pstWin->stCfg.enOutCS    = stColor.enOutCS;
    pstWin->stCfg.u32Fidelity = u32Fidelity;


    if (pstWin->bDispInfoChange)
    {
        pstWin->bDispInfoChange = HI_FALSE;
    }

    /* here we delete all the rwzb content, just rebuild it  in 98cv200 .*/

    if (pstWin->enDisp == HI_DRV_DISPLAY_1)
    {
        if (HI_NULL != pstDispOpt)
        {
            (HI_VOID)pstDispOpt->PF_DATE_SetCoef(HI_DRV_DISPLAY_0, RWZB_GetDATEParaIndex(u32Fidelity));
            (HI_VOID)pstDispOpt->PF_DATE_SetIRE(pstWin->enDisp, 0);
        }
    }

    stLayerPara.eOutPutColorSpace = pstWin->stCfg.enOutCS;
    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);

    Win_PqProcess(pstWin, pstFrame,&stLayerPara.stVideo, &pstInfo->stDispInfo, HI_FALSE);

    (HI_VOID)pstWin->stVLayerFunc.PF_SetFramePara(pstWin->u32VideoLayer, &stLayerPara);

    Win_UpdateHdFramInfo(&stLayerPara,&pstWin->stCfg,pstFrame->bSecure);
    WinTestFrameMatch(pstWin, pstFrame, (HI_DISP_DISPLAY_INFO_S *)&pstInfo->stDispInfo);

    return;
}

HI_VOID ISR_WinConfigFrame(WINDOW_S *pstWin,
                                    HI_DRV_VIDEO_FRAME_S *pstFrame,
                                    const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_BOOL bHdrFrame = HI_FALSE;
    HI_BOOL bSdrChoice = HI_FALSE;
    HI_RECT_S stVideoRect = {0};

    if(HI_DRV_DISPLAY_1 == pstWin->enDisp)
    {
        SetWinDoviRefInfo(WinGetHDWinNum(),
                      pstFrame,
                      WinBuf_GetNextCfgFrm(&pstWin->stBuffer.stWinBP),
                      pstWin->stUsingAttr.stOutRect);
        SetHDRPathStatus(pstWin->stUsingAttr.bCloseHdrPath);
    }

    if ((pstInfo->stDispInfo.enDispProcessMode == HI_DRV_DISP_PROCESS_MODE_SDR)
        && (HI_NULL == pstFrame))
    {
        /* when last work mode is HDR,go through HDR mute path; otherwise,just go through normal mute path.*/
        bSdrChoice = HI_TRUE;
    }

    if (HI_NULL != pstFrame)
    {
        WIN_JudgeHdrFrame(pstFrame->enSrcFrameType, &bHdrFrame);
        if ((HI_FALSE == bHdrFrame) || (pstWin->stUsingAttr.bCloseHdrPath))
        {
            bSdrChoice = HI_TRUE;
        }
        else
        {
#ifndef VDP_DOLBY_HDR_SUPPORT
            bSdrChoice = HI_TRUE;
#else
            bSdrChoice = HI_FALSE;
#endif
        }
    }

    if (HI_FALSE == bSdrChoice)
    {
        stVideoRect = pstWin->stUsingAttr.stOutRect;
        (HI_VOID)Win_PqProcess(pstWin, pstFrame, &stVideoRect, &pstInfo->stDispInfo, HI_TRUE);
    }
    else
    {
        if (HI_NULL == pstFrame)
        {
            ISR_WinConfigFrameMute(pstWin, pstFrame, pstInfo);
        }
        else
        {
            ISR_WinConfigFrameNormal(pstWin, pstFrame, pstInfo);
        }
    }

    WinUpdateDispInfo(pstWin, (HI_DISP_DISPLAY_INFO_S *)&pstInfo->stDispInfo);
    return ;
}

HI_VOID ISR_WinUpdatePlayInfo(WINDOW_S *pstWin, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    // calc delay time in buffer queue
    HI_U32 T;

    DISP_ASSERT(pstInfo->stDispInfo.u32RefreshRate);
    DISP_ASSERT(pstInfo->stDispInfo.stFmtResolution.s32Height);

    pstWin->stDelayInfo.u32DispRate = pstInfo->stDispInfo.u32RefreshRate;

    /*in fact DISP_ASSERT has solves this ,this is for codecc warning.*/
    if (pstInfo->stDispInfo.u32RefreshRate)
    {
        pstWin->stDelayInfo.T = (1*1000*100)/pstInfo->stDispInfo.u32RefreshRate;
    }
    else
    {
        WIN_ERROR("Invalid display fresh rate:[%d]!\n", __LINE__);
    }

    pstWin->stDelayInfo.bInterlace = pstInfo->stDispInfo.bInterlace;
    T = pstWin->stDelayInfo.T;

    if (pstInfo->stDispInfo.bInterlace)
    {
        pstWin->stDelayInfo.u32DisplayTime = (pstInfo->stDispInfo.u32Vline *2*T)/pstInfo->stDispInfo.stPixelFmtResolution.s32Height;
    }
    else
    {
        pstWin->stDelayInfo.u32DisplayTime = (pstInfo->stDispInfo.u32Vline *T)/pstInfo->stDispInfo.stPixelFmtResolution.s32Height;
    }

    HI_DRV_SYS_GetTimeStampMs((HI_U32 *)&pstWin->stDelayInfo.u32CfgTime);
    return;
}


HI_VOID ISR_WinStateTransfer(WINDOW_S *pstWin)
{
    if (pstWin->enState == WIN_STATE_WORK)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_PAUSE:
            case WIN_STATE_FREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_WORK:
            case WIN_STATE_RESUME:
            case WIN_STATE_UNFREEZE:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }
    else if(pstWin->enState == WIN_STATE_PAUSE)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_RESUME:
            case WIN_STATE_FREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_PAUSE:
            case WIN_STATE_WORK:
            case WIN_STATE_UNFREEZE:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }
    else if(pstWin->enState == WIN_STATE_FREEZE)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_UNFREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_PAUSE:
            case WIN_STATE_FREEZE:
            case WIN_STATE_WORK:
            case WIN_STATE_RESUME:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }

    return;
}

HI_DRV_VIDEO_FRAME_S *WinSearchMatchFrame(WINDOW_S *pstWin)
{
    WIN_DISP_INFO_S *pstDispInfo;
    HI_DRV_FIELD_MODE_E enDstField;
    HI_DRV_VIDEO_FRAME_S *pstDispFrame, *pstNewFrame;

    // if display work at interlace mode
    pstDispInfo = &pstWin->stDispInfo;
    if(!pstDispInfo)
    {
         WIN_ERROR("WIN  null pointer in %s!\n", __FUNCTION__);
         return HI_NULL;
    }

#if 0
    if (pstDispInfo->bIsInterlace != HI_TRUE)
    {
        if(pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
        {
            pstDispInfo = WinGetDispInfoByHandle(pstWin->hSlvWin);
        }
    }
#else

    if (pstDispInfo->bIsInterlace != HI_TRUE)
    {
        // if master window output progressive frame, get slave window info
        if(pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
        {
            WIN_DISP_INFO_S *pstSlvDispInfo;

            pstSlvDispInfo = WinGetDispInfoByHandle(pstWin->hSlvWin);
            if (pstSlvDispInfo)
            {
                if (   (pstSlvDispInfo->bIsInterlace == HI_TRUE)
                     &&(pstDispInfo->u32RefreshRate == pstSlvDispInfo->u32RefreshRate)
                    )
                {
                    pstDispInfo = pstSlvDispInfo;
                }
            }
         }
    }
#endif

    if (!pstDispInfo || (pstDispInfo->bIsInterlace != HI_TRUE) )
    {
        //if output progressive picture, need not to search top/bottom field
        return WinBuf_GetConfigFrame(&pstWin->stBuffer.stWinBP);
    }

    // if display work at interlace mode and frame rate equale to display refresh rate,
    // dectect whether top-field video frame output at top-field time
    pstDispFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
    enDstField = pstDispInfo->bIsBtm ? HI_DRV_FIELD_BOTTOM : HI_DRV_FIELD_TOP;


    pstNewFrame = WinBuf_GetFrameByDisplayInfo(&pstWin->stBuffer.stWinBP,
                                            pstDispFrame,
                                            pstDispInfo->u32RefreshRate,
                                            enDstField);
    return pstNewFrame;
}


HI_DRV_VIDEO_FRAME_S * WinGetFrameToConfig(WINDOW_S *pstWin, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;

    if (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
    {
        pstFrame = ISR_SlaveWinGetConfigFrame(pstWin);
        if (pstFrame == HI_NULL && pstWin->bQuickMode)
        {
            HI_DRV_VIDEO_FRAME_S *pstDispFrame;

            pstDispFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);

            WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstDispFrame);
        }
    }
    else
    {
        if (pstWin->bQuickMode)
        {
            HI_DRV_VIDEO_FRAME_S *pstDispFrame;

            pstDispFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
            pstFrame = WinBuf_GetNewestFrame(&pstWin->stBuffer.stWinBP, pstDispFrame);
        }
        else
        {
            if(!pstWin->bStepMode)
            {
                pstFrame = WinSearchMatchFrame(pstWin);
            }
            else
            {
                if(pstWin->bStepPlayNext)
                {
                    pstFrame = WinSearchMatchFrame(pstWin);
                }
            }
        }
    }


    if (!pstFrame)
    {
        /*if not  receive frame*/
        pstWin->stBuffer.u32UnderLoad++;

        if (WinTestBlackFrameFlag(pstWin) != HI_TRUE)
        {
            /*
                    1 :  if first config frame ,config nothing.
                    2 :  if last config is not black frame, repeat last config frame*/
            WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
            pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);
        }
        else
        {
            /*if last config is black frame, repeat black frame*/
            WinSetBlackFrameFlag(pstWin);
        }
    }
    else
    {
        /*if received frame*/
        WinClearBlackFrameFlag(pstWin);
        //wait for next frame to play.
        pstWin->bStepPlayNext = HI_FALSE;
    }

    return pstFrame;
}



HI_VOID ISR_CallbackForWinProcess(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;
    HI_BOOL bUpDispInof = HI_FALSE;
    HI_U32  u32HdrWinHandle = HI_INVALID_HANDLE;
    HI_SIZE_T irqflag = 0;

    if (!hDst || !pstInfo )
    {
        WIN_ERROR("Input null pointer\n");
        return;
    }

    spin_lock_irqsave(&g_threadIsr_Lock, irqflag);

    pstWin = (WINDOW_S *)hDst;

    if (pstInfo->eEventType != HI_DRV_DISP_C_VT_INT)
    {
        DISP_PRINT("@@@@@@@ DISP HI_DRV_DISP_C_event= %d, disp=%d\n", pstInfo->eEventType, pstWin->enDisp);
    }

    if (  (WIN_DEVICE_STATE_SUSPEND == s_s32WindowGlobalFlag)
            ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
            ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND)
       )
    {
        DISP_PRINT(">>>>>>>>> mask\n");
        pstWin->bMasked = HI_TRUE;
    }
    else
    {
        pstWin->bMasked = HI_FALSE;
    }

    if ( (pstInfo->eEventType == HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE)
            || (pstInfo->eEventType == HI_DRV_DISP_C_OPEN)
            || (pstInfo->eEventType == HI_DRV_DISP_C_RESUME))
    {
        pstWin->bDispInfoChange = HI_TRUE;
        bUpDispInof = HI_TRUE;
    }

    /*judge whether the window cfg is changed.
         * and reload the attr buffer to the buffer using.*/
    if (atomic_read(&pstWin->stCfg.bNewAttrFlag))
    {
        pstWin->stCfg.stAttr = pstWin->stCfg.stAttrBuf;
        pstWin->stUsingAttr  = pstWin->stCfg.stAttrBuf;
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
        bUpDispInof = HI_TRUE;
        pstWin->bDispInfoChange = HI_TRUE;
    }

    if (bUpDispInof)
    {
        HI_RECT_S stOutRectRevised;
        memset((void*)&stOutRectRevised, 0, sizeof(HI_RECT_S));

        /*give a size conversion from:
             * 1) user setting(virtscreen)-----> virtual screen---->actual format;
             * 2) user setting(physical coordinate)--------> actual format
           * in fact, you can choose only one through the flag pstWin->bVirtScreenMode.
             */
        WinOutRectSizeConversionByType(&pstInfo->stDispInfo,
                                        &stOutRectRevised,
                                        pstWin);

        pstWin->stUsingAttr.stOutRect = stOutRectRevised;

        /*when not virtual screen mode, the cfg's attr must be the actual physical fmt.*/
        if (!pstWin->bVirtScreenMode)
        {
            pstWin->stCfg.stAttr.stOutRect =  pstWin->stUsingAttr.stOutRect;
        }

    }

    WinSendAttrToSource(pstWin, (HI_DISP_DISPLAY_INFO_S *)&pstInfo->stDispInfo);

    DISP_PRINT("Display info>> M=%d,S=%d,att=%d, 3d=%d, R=%d, I=%d, w=%d, h=%d, %dvs%d, rate=%d, cs=%d\n",
            pstInfo->stDispInfo.bIsMaster,
            pstInfo->stDispInfo.bIsSlave,
            pstInfo->stDispInfo.enAttachedDisp,
            pstInfo->stDispInfo.eDispMode,
            pstInfo->stDispInfo.bRightEyeFirst,
            pstInfo->stDispInfo.bInterlace,
            pstInfo->stDispInfo.stPixelFmtResolution.s32Width,
            pstInfo->stDispInfo.stPixelFmtResolution.s32Height,
            pstInfo->stDispInfo.stAR.u32ARw,
            pstInfo->stDispInfo.stAR.u32ARh,
            pstInfo->stDispInfo.u32RefreshRate,
            pstInfo->stDispInfo.eColorSpace);

     /*set dolby win status*/
     GetHdrWinHandle(&u32HdrWinHandle);
    if (u32HdrWinHandle == pstWin->u32Index)
    {
        SetHdrWinStatus(pstWin->bEnable_Using);
        if (pstWin->stUsingAttr.bCloseHdrPath)
        {
            WIN_HAL_PARA_S stV1LayerPara;
            memset(&stV1LayerPara,0,sizeof(WIN_HAL_PARA_S));

            stV1LayerPara.bRegionEnable = HI_FALSE;
            stV1LayerPara.u32LayerNO = VDP_LAYER_VID1;
            stV1LayerPara.u32RegionNum = pstWin->u32VideoRegionNo;
            stV1LayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);
            stV1LayerPara.eOutPutColorSpace = pstWin->stCfg.enOutCS;
            (HI_VOID)pstWin->stVLayerFunc.PF_SetFramePara(pstWin->u32VideoLayer, &stV1LayerPara);
        }
    }

    if (!pstWin->bEnable_Using || pstWin->bMasked)
    {
        WinDisableSingleLayerRegion(pstWin,pstInfo);
        if(HI_DRV_DISPLAY_1 == pstWin->enDisp)
        {
           SetWinDoviRefInfo(WinGetHDWinNum(),
                             HI_NULL,
                             HI_NULL,
                             pstWin->stUsingAttr.stOutRect);
        }
        spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
        return;
    }

    pstWin->bInInterrupt = HI_TRUE;

    // window process
    if ( (HI_DRV_DISP_C_VT_INT == pstInfo->eEventType)
            || (HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE == pstInfo->eEventType))
    {
        HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;

        if (pstWin->bReset)
        {
            // release displayed and configed frame
            s_ResetPrint = 1;

            //ISR_WinReleaseDisplayedFrame(pstWin);
            WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);

            ISR_WinReleaseUSLFrame(pstWin);

            // flush frame in full buffer pool
            //ISR_WinReleaseFullFrame(pstWin);
            pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
            WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);

            if (pstWin->stRst.enResetMode == HI_DRV_WIN_SWITCH_BLACK)
            {
                /*do not use blackframe,just set the flag and mute the layer.*/
                WinSetBlackFrameFlag(pstWin);

                /*pass NULL frame-ptr into the func, just mute it internally.*/
                ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
#ifdef HI_VO_WIN_SYNC_SUPPORT
                (HI_VOID)WIN_Sync_Flush(pstWin);
#endif
            }
            else
            {
                WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
                pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);

                if (!pstFrame)
                {
                    /*no frame to display, mute it.*/
                    WinSetBlackFrameFlag(pstWin);
                    ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                }
                else
                {
                    ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                    WinClearBlackFrameFlag(pstWin);
#ifdef HI_VO_WIN_SYNC_SUPPORT
                    WIN_Sync_Signal(pstWin,
                            pstFrame->u32FrameIndex,
                            pstFrame->stBufAddr[0].u32PhyAddr_Y);
#endif
                }
            }
            //printk("-----reset in winprocess %d----\n",__LINE__);
            s_ResetPrint = 0;
            pstWin->enState = WIN_STATE_WORK;
            pstWin->bReset  = HI_FALSE;
            pstWin->stRst.enResetMode  = HI_DRV_WIN_SWITCH_BUTT;
        }
        else
        {
            // window state transfer
            if (pstWin->bUpState)
            {
                ISR_WinStateTransfer(pstWin);
            }

            switch(pstWin->enState)
            {
                case WIN_STATE_RESUME:
                case WIN_STATE_UNFREEZE:
                    {
                        pstWin->enState = WIN_STATE_WORK;
                        // no break, enter case 'WIN_STATE_WORK'
                    }
                case WIN_STATE_WORK:
                    {
                        //ISR_WinReleaseDisplayedFrame(pstWin);
                        WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);

                        ISR_WinReleaseUSLFrame(pstWin);

                        pstFrame = WinGetFrameToConfig(pstWin, pstInfo);

                        if(pstFrame)
                        {
                            if ((pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE
                                        || pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
                                    && (pstFrame->u32FrameIndex != pstWin->u32LastOutLowDelayIdx)
                               )
                            {
                                HI_LD_Event_S evt;
                                HI_U32 TmpTime = 0;
                                HI_DRV_SYS_GetTimeStampMs(&TmpTime);
                                evt.evt_id = EVENT_VO_FRM_OUT;
                                evt.frame = pstFrame->u32FrameIndex;
                                evt.handle = pstFrame->hTunnelSrc;
                                evt.time = TmpTime;
                                HI_DRV_LD_Notify_Event(&evt);
                                pstWin->u32LastOutLowDelayIdx = pstFrame->u32FrameIndex;
                            }

                            ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                            WinClearBlackFrameFlag(pstWin);

#ifdef HI_VO_WIN_SYNC_SUPPORT
                            (HI_VOID)WIN_Sync_Signal(pstWin,
                                    pstFrame->u32FrameIndex,
                                    pstFrame->stBufAddr[0].u32PhyAddr_Y);
#endif

                        }
                        else if (WinTestBlackFrameFlag(pstWin) == HI_TRUE)
                        {
                            /*no frame to display, just mute the layer.*/
                            ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                        }
                        else
                        {
                            ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                        }

                        /* last branch , no frame and it's the start of playing,
                         * just return.
                         */

                        break;
                    }
                case WIN_STATE_PAUSE:
                    {
                        WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
                        ISR_WinReleaseUSLFrame(pstWin);

                        WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);

                        pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);
                        if (!pstFrame)
                        {
                            WinSetBlackFrameFlag(pstWin);
                            ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                        }
                        else if (pstFrame)
                        {
                            ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                            WinClearBlackFrameFlag(pstWin);
                        }

                        break;
                    }
                case WIN_STATE_FREEZE:
                    {
                        HI_S32 nRet;

                        WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
                        ISR_WinReleaseUSLFrame(pstWin);

                        /*freeze mode, the buffer should flows as normal,
                                         so we should let the buffer rounds.*/
                        WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
                        pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);

                        nRet = WinBuf_ReleaseOneFrame(&pstWin->stBuffer.stWinBP, pstFrame);
                        if (nRet != HI_SUCCESS)
                        {
                            /* u32UnderLoad happened */
                            pstWin->stBuffer.u32UnderLoad++;
                        }

                        if (pstWin->stFrz.enFreezeMode == HI_DRV_WIN_SWITCH_BLACK)
                        {
                            WinSetBlackFrameFlag(pstWin);
                            ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                        }
                        else if (pstFrame)
                        {
                            ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                            WinClearBlackFrameFlag(pstWin);
                        }
                        else
                        {
                            ISR_WinConfigFrame(pstWin, HI_NULL, pstInfo);
                        }

                        break;
                    }
                default:
                    break;
            }
        }
    }

    ISR_WinUpdatePlayInfo(pstWin, pstInfo);
    pstWin->bInInterrupt = HI_FALSE;

    spin_unlock_irqrestore(&g_threadIsr_Lock, irqflag);
    return;
}


HI_S32 WinGetProcIndex(HI_HANDLE hWin, HI_U32 *p32Index)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(p32Index);

    // s1 get window pointer
    pstWin = WinGetWindow(hWin);
    if (pstWin)
    {
        // return active windwo index
        *p32Index = pstWin->u32Index;
    }
    else
    {
        VIRTUAL_S *pstVirWin;

        pstVirWin = WinGetVirWindow(hWin);
        if (pstVirWin)
        {
            //return virtual window index
            *p32Index = pstVirWin->u32Index;
        }
        else
        {
            return HI_ERR_VO_WIN_NOT_EXIST;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinGetProcInfo(HI_HANDLE hWin, WIN_PROC_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pstInfo);

    DISP_MEMSET(pstInfo, 0, sizeof(WIN_PROC_INFO_S));

    // s1 get active window pointer
    pstWin = WinGetWindow(hWin);
    if (pstWin)
    {
        // get window proc info
        pstInfo->enType        = pstWin->enType;
        pstInfo->u32Index    = pstWin->u32Index;

        pstInfo->u32LayerId = (HI_U32)pstWin->u32VideoLayer;
        pstInfo->u32LayerRegionNo  = (HI_U32)pstWin->u32VideoRegionNo;

        pstInfo->u32Zorder  = pstWin->u32Zorder;
        pstInfo->bEnable   =  (HI_U32)pstWin->bEnable;
        pstInfo->bMasked   =  (HI_U32)pstWin->bMasked;
        pstInfo->u32WinState = (HI_U32)pstWin->enState;

        pstInfo->bReset = pstWin->bReset;
        pstInfo->enResetMode = pstWin->stRst.enResetMode;
        pstInfo->enFreezeMode = pstWin->stFrz.enFreezeMode;

        pstInfo->bQuickMode = pstWin->bQuickMode;
        pstInfo->bStepMode    = pstWin->bStepMode;
        pstInfo->bVirtualCoordinate = pstWin->bVirtScreenMode;

        pstInfo->hSrc           = (HI_U32)pstWin->stCfg.stSource.hSrc;
        pstInfo->hSecondSrc    = (HI_U32)pstWin->stCfg.stSource.hSecondSrc;
        pstInfo->pfAcqFrame    = (HI_VOID*)pstWin->stCfg.stSource.pfAcqFrame;
        pstInfo->pfRlsFrame    = (HI_VOID*)pstWin->stCfg.stSource.pfRlsFrame;
        pstInfo->pfSendWinInfo = (HI_VOID*)pstWin->stCfg.stSource.pfSendWinInfo;

        pstInfo->stAttr  = pstWin->stCfg.stAttr;

        pstInfo->u32TBNotMatchCount = pstWin->u32TBNotMatchCount;

        pstInfo->eDispMode = pstWin->stCfg.eDispMode;
        pstInfo->bRightEyeFirst = pstWin->stCfg.bRightEyeFirst;

        pstInfo->hSlvWin  = (HI_U32)pstWin->hSlvWin;
        pstInfo->bDebugEn = (HI_U32)pstWin->bDebugEn;

        pstInfo->u32ULSIn  = pstWin->stBuffer.u32ULSIn;
        pstInfo->u32ULSOut = pstWin->stBuffer.u32ULSOut;
        pstInfo->u32UnderLoad = pstWin->stBuffer.u32UnderLoad;

        pstInfo->stWinInfoForDeveloper  = pstWin->stWinInfoForDeveloper;

        WIN_GetIsogenyPocInfor(&pstInfo->stIsogenyProcInfo);
        WinBuf_GetStateInfo(&pstWin->stBuffer.stWinBP, (WB_STATE_S *)&(pstInfo->stBufState));

        pstInfo->enWinMode  = pstWin->enWinMode;
    }
    else
    {
        VIRTUAL_S *pstVirWin;

        pstVirWin = WinGetVirWindow(hWin);
        if (pstVirWin)
        {
            // get virtual proc info
            pstInfo->enType        = pstVirWin->enType;
            pstInfo->u32Index    = pstVirWin->u32Index;
            pstInfo->u32LayerId = 0xFFFFul;  // not use video layer
            pstInfo->bEnable   = (HI_U32)pstVirWin->bEnable;
            pstInfo->bMasked   = (HI_U32)pstVirWin->bMasked;
            pstInfo->u32WinState = 0;  // 0 means work

            pstInfo->hSrc          = (HI_U32)pstVirWin->stSrcInfo.hSrc;
            pstInfo->pfAcqFrame    = (HI_VOID*)pstVirWin->stSrcInfo.pfAcqFrame;
            pstInfo->pfRlsFrame    = (HI_VOID*)pstVirWin->stSrcInfo.pfRlsFrame;
            pstInfo->pfSendWinInfo = (HI_VOID*)pstVirWin->stSrcInfo.pfSendWinInfo;

            pstInfo->stAttr = pstVirWin->stAttrBuf;


            pstInfo->stBufState.stRecord.u32TryQueueFrame = pstVirWin->stFrameStat.u32SrcQTry;
            pstInfo->stBufState.stRecord.u32QueueFrame      = pstVirWin->stFrameStat.u32SrcQOK;

            pstInfo->stBufState.stRecord.u32Release = pstVirWin->stFrameStat.u32SinkRlsOK;
        }
        else
        {
            WIN_ERROR("WIN is not exist!\n");
            return HI_ERR_VO_WIN_NOT_EXIST;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinGetCurrentImg(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    WINDOW_S *pstWin;
    HI_DRV_VIDEO_FRAME_S *pstFrmTmp = NULL;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstFrame);
    WinCheckWindow(hWin, pstWin);
    DISP_MEMSET(pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    pstFrmTmp = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
    if (NULL == pstFrmTmp)
    {
        if (pstWin->latest_display_frame_valid)
        {
            *pstFrame = pstWin->latest_display_frame;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else
    {
         *pstFrame = *(pstFrmTmp);
         pstWin->latest_display_frame = *(pstFrmTmp);
         pstWin->latest_display_frame_valid = 1;
    }

    return HI_SUCCESS;
}

HI_S32 WinCaptureFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U32 *stMMZPhyAddr, HI_U32 *stMMZlen)
{
    WINDOW_S *pstWin;
    HI_DRV_VIDEO_FRAME_S *pstFrmTmp = NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = HI_NULL;
    HI_U32 datalen = 0, y_stride = 0, c_stride = 0, height = 0, u32width = 0;
    SMMU_BUFFER_S stDstMMU;
    HI_S32 s32Ret;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstFrame);
    WinCheckNullPointer(stMMZPhyAddr);
    WinCheckNullPointer(stMMZlen);
    WinCheckWindow(hWin, pstWin);
    DISP_MEMSET(pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    if (pstWin->enType == HI_DRV_WIN_VITUAL_SINGLE)
        return HI_ERR_VO_WIN_UNSUPPORT;

    if ( ((WIN_STATE_FREEZE == pstWin->enState)
             && (HI_DRV_WIN_SWITCH_BLACK == pstWin->stFrz.enFreezeMode))
      || ((HI_TRUE == pstWin->bReset)
             && (HI_DRV_WIN_SWITCH_BLACK == pstWin->stRst.enResetMode)))
    {
        pstFrmTmp = BP_GetBlackFrameInfo();
    }
    else
    {
        if(HI_SUCCESS != WinBuf_SetCaptureFrame(&pstWin->stBuffer.stWinBP, pstWin->u32WinCapMMZvalid))
            return HI_ERR_VO_INVALID_OPT;

        pstFrmTmp = WinBuf_GetCapturedFrame(&pstWin->stBuffer.stWinBP);
        if (HI_NULL == pstFrmTmp)
        {
            pstFrmTmp = BP_GetBlackFrameInfo();
        }
    }

    if (HI_NULL == pstFrmTmp)
    {
        WIN_ERROR("get null frame ptr\n");
        return HI_ERR_VO_BUFQUE_EMPTY;
    }

    *pstFrame = *(pstFrmTmp);
    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(pstFrame->u32Priv);

    y_stride = pstFrame->stBufAddr[0].u32Stride_Y;
    c_stride = pstFrame->stBufAddr[0].u32Stride_C;
    height   = pstFrame->u32Height;
    u32width   = pstFrame->u32Width;

    /*don't know why there exists these two branches, do we condidered gstreamer?*/
    if ( HI_DRV_PIX_FMT_NV21 == pstFrame->ePixFormat
        || HI_DRV_PIX_FMT_NV12_TILE_CMP == pstFrame->ePixFormat
        || HI_DRV_PIX_FMT_NV21_TILE_CMP == pstFrame->ePixFormat)
    {
        datalen = height * y_stride * 3 / 2 + height * 4;
    }
    else if ( HI_DRV_PIX_FMT_NV21_CMP == pstFrame->ePixFormat)
    {
        datalen = height * y_stride * 3 / 2 + 16 * height * 3/2;
    }
    else if ( HI_DRV_PIX_FMT_NV61_2X1 == pstFrame->ePixFormat)
    {
        datalen = height * y_stride * 2 + height * 4;
    }
    else
    {
        WIN_ERROR("Invalid Format %d!\n",pstFrame->ePixFormat);
        return HI_FAILURE;
    }

//  printk("---capture-----str(%d)---(%d,%d)--------\n",y_stride,u32width,height);
    if(HI_SUCCESS != DISP_OS_MMZ_Alloc("VDP_Cpefun", VDP_MEM_TYPE_MMZ, datalen, VDP_MEM_ALIGN, &pstWin->stWinCaptureMMZ))
    {
        WinBuf_ReleaseCaptureFrame(&pstWin->stBuffer.stWinBP,pstFrame,HI_FALSE);
        return HI_ERR_VO_MALLOC_FAILED;
    }

    pstWin->u32WinCapMMZvalid  = 1;
    *stMMZPhyAddr = pstWin->stWinCaptureMMZ.u32StartPhyAddr;
    *stMMZlen      = pstWin->stWinCaptureMMZ.u32Size;
    stDstMMU.u32StartSmmuAddr = pstWin->stWinCaptureMMZ.u32StartPhyAddr;
    stDstMMU.u32Size = pstWin->stWinCaptureMMZ.u32Size;
    s32Ret = HI_DRV_SMMU_Map(&stDstMMU);
    if (s32Ret != HI_SUCCESS)
    {
        DISP_OS_MMZ_Release(&pstWin->stWinCaptureMMZ);
        WIN_ERROR("Map VDP_Cpefun Failed\n");
        return HI_FAILURE;
    }

    if (HI_DRV_PIX_FMT_NV12_TILE_CMP == pstFrame->ePixFormat
        || HI_DRV_PIX_FMT_NV21_TILE_CMP == pstFrame->ePixFormat)
    {
        s32Ret = VO_ALG_Decompress(pstFrame,
            stDstMMU.pu8StartVirAddr,
            stDstMMU.u32Size);
        if (s32Ret != HI_SUCCESS)
        {
            WIN_ERROR("Decompress Failed\n");
            HI_DRV_SMMU_Unmap(&stDstMMU);
            DISP_OS_MMZ_Release(&pstWin->stWinCaptureMMZ);
            return HI_FAILURE;
        }
    }
    else
    {
        SMMU_BUFFER_S stSrcMMU;
        stSrcMMU.u32StartSmmuAddr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
        s32Ret = HI_DRV_SMMU_Map(&stSrcMMU);
        if (s32Ret != HI_SUCCESS)
        {
            WIN_ERROR("Map Src Buffer Failed\n");
            HI_DRV_SMMU_Unmap(&stDstMMU);
            DISP_OS_MMZ_Release(&pstWin->stWinCaptureMMZ);
            return HI_FAILURE;
        }
        memcpy(stDstMMU.pu8StartVirAddr,
            stSrcMMU.pu8StartVirAddr,
            height * y_stride * 3 / 2);
        HI_DRV_SMMU_Unmap(&stSrcMMU);
    }

    HI_DRV_SMMU_Unmap(&stDstMMU);
    return HI_SUCCESS;
}

HI_S32 WinReleaseCaptureFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstFrame);
    WinCheckWindow(hWin, pstWin);

    if (pstWin->enType == HI_DRV_WIN_VITUAL_SINGLE)
        return HI_ERR_VO_WIN_UNSUPPORT;

    if(HI_SUCCESS != WinBuf_ReleaseCaptureFrame(&pstWin->stBuffer.stWinBP,pstFrame, HI_FALSE))
        return HI_ERR_VO_INVALID_OPT;

    return HI_SUCCESS;
}

HI_S32 WinFreeCaptureMMZ(WINDOW_S *pstWin)
{

    if (pstWin->u32WinCapMMZvalid == 1) {
        DISP_OS_MMZ_Release(&pstWin->stWinCaptureMMZ);
        pstWin->stWinCaptureMMZ.u32StartPhyAddr = 0;
        pstWin->u32WinCapMMZvalid = 0;
    }
    else
    {
        WIN_WARN("warning: when free mmz, null refcnt occurs.!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 WinFreeCaptureMMZBuf(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *cap_frame)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();
    WinCheckWindow(hWin, pstWin);

    if (cap_frame->stBufAddr[0].u32PhyAddr_Y
        != pstWin->stWinCaptureMMZ.u32StartPhyAddr)
    {
        WIN_ERROR("capture release addr:%x,%x!\n", cap_frame->stBufAddr[0].u32PhyAddr_Y,
            pstWin->stWinCaptureMMZ.u32StartPhyAddr);
        return HI_ERR_VO_INVALID_OPT;
    }

    return WinFreeCaptureMMZ(pstWin);

}


HI_S32 WinForceClearCapture(HI_HANDLE hWin)
{
    HI_DRV_VIDEO_FRAME_S *pstFrmTmp = NULL;
    WINDOW_S *pstWin = HI_NULL;

    WinCheckDeviceOpen();
    WinCheckWindow(hWin, pstWin);

    pstFrmTmp = WinBuf_GetCapturedFrame(&pstWin->stBuffer.stWinBP);
    if (NULL != pstFrmTmp)
    {
        if(HI_SUCCESS != WinBuf_ReleaseCaptureFrame(&pstWin->stBuffer.stWinBP,pstFrmTmp, HI_TRUE))
            return HI_ERR_VO_INVALID_OPT;
    }
    if( HI_SUCCESS != WinFreeCaptureMMZ(pstWin))
        return HI_FAILURE;

    return HI_SUCCESS;
}

HI_S32 WinCapturePause(HI_HANDLE hWin, HI_BOOL bCaptureStart)
{
    WINDOW_S *pstWin = HI_NULL, *pstWinAttach = HI_NULL;
    HI_S32 Ret = 0;
    HI_HANDLE  attach_handle = 0;

    pstWin = WinGetWindow(hWin);
    if (!pstWin)
    {
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if (bCaptureStart && ((pstWin->enState == WIN_STATE_PAUSE)
        || (pstWin->enState == WIN_STATE_FREEZE)))
    {
        pstWin->bRestoreFlag = 0;
        return HI_SUCCESS;
    }

    if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
    {
        /*attach mode , need  pasue salve window too*/
        attach_handle = pstWin->hSlvWin;
        pstWinAttach = WinGetWindow(attach_handle);

        if (!pstWinAttach)
        {
            attach_handle = 0;
        }
    }
    else if (pstWin->enType == HI_DRV_WIN_ACTIVE_SINGLE)
    {
        /*not attach mode only need  pasue self window*/
        attach_handle = 0;
    }
    else if (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
    {
        /*attach mode only    mce capture slave window*/
        attach_handle = 0;
    }

    if (bCaptureStart)
    {
        Ret = WIN_Pause(hWin, bCaptureStart);
        if (Ret != HI_SUCCESS)
            return Ret;

        if (attach_handle)
        {
            Ret = WIN_Pause(attach_handle, bCaptureStart);
            if (Ret != HI_SUCCESS)
            {
                (HI_VOID)WIN_Pause(hWin, !bCaptureStart);
                return Ret;
            }
        }

        pstWin->bRestoreFlag = 1;
    }
    else
    {
        if (pstWin->bRestoreFlag) {

            Ret = WIN_Pause(hWin, 0);
            if (attach_handle)
            {
                Ret |= WIN_Pause(attach_handle, 0);
            }
            pstWin->bRestoreFlag = 0;

            if (Ret != HI_SUCCESS)
                return Ret;

        }
    }

    return HI_SUCCESS;
}


HI_S32 WIN_SetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E enRotation)
{
    WINDOW_S *pstWin;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_S32 nRet = HI_SUCCESS;
    HI_S32 t;
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        nRet = DISP_GetDisplayInfo(pstWin->enDisp, &stDispInfo);
        if (nRet)
        {
            WIN_ERROR("DISP_GetDisplayInfo failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }

        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
        pstWin->enRotation = enRotation;
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);

        t = 0;
        while( atomic_read(&pstWin->stCfg.bNewAttrFlag) )
        {
            DISP_MSLEEP(5);
            t++;
            if (t > 10)
            {
                break;
            }
        }
        if (pstWin->hSlvWin)
        {
            if (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
            {
                nRet = WIN_SetRotation(pstWin->hSlvWin, enRotation);
            }
        }
        if (atomic_read(&pstWin->stCfg.bNewAttrFlag) )
        {
            atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
            WIN_ERROR("WIN Set Attr timeout in %s\n", __FUNCTION__);
            return HI_ERR_VO_TIMEOUT;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWindow;

        WinCheckVirWindow(hWin, pstVirWindow);

        pstVirWindow->enRotation = enRotation;

        WIN_VIR_SendAttrToSource(pstVirWindow);

    }
    return HI_SUCCESS;
}

HI_S32 WIN_GetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E *penRotation)
{
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WINDOW_S *pstWin;
        WinCheckWindow(hWin, pstWin);
        *penRotation = pstWin->enRotation;
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        WinCheckVirWindow(hWin, pstVirWindow);
        *penRotation = pstVirWindow->enRotation;
    }
    return HI_SUCCESS;
}

HI_S32 WIN_SetFlip(HI_HANDLE hWin, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    WINDOW_S *pstWin;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_S32 nRet = HI_SUCCESS;
    HI_S32 t;
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WinCheckWindow(hWin, pstWin);
        nRet = DISP_GetDisplayInfo(pstWin->enDisp, &stDispInfo);
        if (nRet)
        {
            WIN_ERROR("DISP_GetDisplayInfo failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }

        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
        pstWin->bHoriFlip = bHoriFlip;
        pstWin->bVertFlip = bVertFlip;
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);


        t = 0;
        while( atomic_read(&pstWin->stCfg.bNewAttrFlag) )
        {
            DISP_MSLEEP(5);
            t++;
            if (t > 10)
            {
                break;
            }
        }

        if (pstWin->hSlvWin)
        {
            if (stDispWindow.eIsogeny_mode == ISOGENY_WBC_MODE)
            {
                nRet = WIN_SetFlip(pstWin->hSlvWin, bHoriFlip,bVertFlip);
            }
        }

        if (atomic_read(&pstWin->stCfg.bNewAttrFlag) )
        {
            atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
            WIN_ERROR("WIN Set Attr timeout in %s\n", __FUNCTION__);
            return HI_ERR_VO_TIMEOUT;
        }
    }
    else
    {
        VIRTUAL_S *pstVirWindow;

        WinCheckVirWindow(hWin, pstVirWindow);

        pstVirWindow->bHoriFlip = bHoriFlip;
        pstVirWindow->bVertFlip = bVertFlip;

        WIN_VIR_SendAttrToSource(pstVirWindow);
    }
    return HI_SUCCESS;
}

HI_S32 WIN_GetFlip(HI_HANDLE hWin, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip)
{
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WINDOW_S *pstWin;
        WinCheckWindow(hWin, pstWin);
        *pbHoriFlip = pstWin->bHoriFlip;
        *pbVertFlip = pstWin->bVertFlip;
    }
    else
    {
        VIRTUAL_S *pstVirWindow;
        WinCheckVirWindow(hWin, pstVirWindow);
        *pbHoriFlip = pstVirWindow->bHoriFlip;
        *pbVertFlip = pstVirWindow->bVertFlip;
    }
    return HI_SUCCESS;
}

HI_S32 WIN_SetFrostMode(HI_HANDLE hWin, HI_DRV_WINDOW_FROST_E eWinFrostMode)
{
    HI_BOOL bVirtual;
    WINDOW_S *pstWin = HI_NULL;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (HI_TRUE == bVirtual)
    {
        return HI_ERR_VO_INVALID_OPT;
    }

    WinCheckWindow(hWin, pstWin);

    if (HI_DRV_DISPLAY_0 == pstWin->enDisp)
    {
        pstWin->enWinFrostMode = HI_DRV_WINDOW_FROST_CLOSE;
    }
    else
    {
        pstWin->enWinFrostMode = eWinFrostMode;

        if ((HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
            && (pstWin->hSlvWin))
        {
            WINDOW_S *pstSlaveWin = HI_NULL;
            WinCheckWindow(pstWin->hSlvWin, pstSlaveWin);
            pstSlaveWin->enWinFrostMode = HI_DRV_WINDOW_FROST_CLOSE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WIN_GetFrostMode(HI_HANDLE hWin, HI_DRV_WINDOW_FROST_E *peWinFrostMode)
{
    HI_BOOL bVirtual;
    WINDOW_S *pstWin = HI_NULL;

    WinCheckDeviceOpen();

    bVirtual = WinCheckVirtual(hWin);
    if (HI_TRUE == bVirtual)
    {
        return HI_ERR_VO_INVALID_OPT;
    }

    WinCheckWindow(hWin, pstWin);
    *peWinFrostMode = pstWin->enWinFrostMode;

    return HI_SUCCESS;
}

HI_S32 WIN_Test(HI_HANDLE hWin)
{
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
    }

    return HI_SUCCESS;
}

HI_S32 WIN_GetUnload(HI_HANDLE hWin, HI_U32 *pu32Times)
{
    HI_BOOL bVirtual;
    WinCheckDeviceOpen();
    bVirtual = WinCheckVirtual(hWin);
    if (!bVirtual)
    {
        WINDOW_S *pstWin;
        WinCheckWindow(hWin, pstWin);
        *pu32Times = pstWin->stBuffer.u32UnderLoad;
    }
    else
    {
        *pu32Times = 0;
        return HI_ERR_VO_WIN_UNSUPPORT;
    }
    return HI_SUCCESS;
}

HI_S32 WinDebugCmdProcess(WIN_DEBUG_CMD_AND_ARGS_S * pstDebugCmd)
{
    HI_S32 s32Ret = HI_SUCCESS;

    WinCheckNullPointer(pstDebugCmd);
#ifdef VDP_DOLBY_HDR_SUPPORT
    switch (pstDebugCmd->enDebugCmd)
    {
        case WIN_DEBUG_SET_HISI_HDR_PATH:
        {
            HI_BOOL *pbEnable = (HI_BOOL*)pstDebugCmd->u8Args;
            s32Ret = WIN_SetHisiPath(pstDebugCmd->hWindow,*pbEnable);
            break;
        }
        default:
        {
            s32Ret = HI_ERR_VO_WIN_UNSUPPORT;
            break;
        }
    }
#endif
    return s32Ret;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




