/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_wrap_hisi_hal_vo.cpp
 * @brief   android hisi37xx vdec&vdisppipe
 * @author  lidanjing(l00346954)
 * @date    2015/12/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/12/4
 * Author : lidanjing(l00346954)
 * Desc   : Created file
 *
******************************************************************************/
//#include <pthread.h>
//#include <binder/Parcel.h>
//#include <gui/Surface.h>

//lint -e1784
//lint -e717

#include "osal_type.h"
#include "sme_macro.h"
#include "sme_log.h"
#include "osal_mem.h"
#include "sme_media_type.h"

#ifndef LINUX_OS
#include "hardware.h"
#endif
#include "sme_wrap_hisi_hal_vo.h"

#define VIRTUAL_SCREEN_WIDTH 1280
#define VIRTUAL_SCREEN_HEIGHT 720

#define DEQUEUE_TIMEOUT 3   //ms
#define HI_ERR_VO_BUFQUE_FULL               (HI_S32)(0x80110052)

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif


SME_HAL_VDISP_PIPE_HDL SME_Hal_VDisp_Create()
{
    ST_SME_HAL_HI_VDISP_PIPE * pstPipe;
//lint -e10
    pstPipe = (ST_SME_HAL_HI_VDISP_PIPE*)malloc(sizeof(ST_SME_HAL_HI_VDISP_PIPE));
//lint +e10
    if(pstPipe == NULL)
    {
        ICS_LOGE("malloc ST_SME_HAL_HI_VDISP_PIPE failed");
        return NULL;
    }
    pstPipe->phVout = HI_INVALID_HANDLE;
    pstPipe->phVoutWindow = HI_INVALID_HANDLE;
    pstPipe->pstVoutDev = NULL;
    //pstPipe->pstVoutDev = (VOUT_DEVICE_S*)malloc(sizeof(VOUT_DEVICE_S));
    //if(pstPipe->pstVoutDev == NULL)
    //{
    //    ICS_LOGE("malloc VOUT_DEVICE_S failed");
    //    free(pstPipe);
    //    return NULL;
    //}

    return (SME_HAL_VDISP_PIPE_HDL)pstPipe;

}

V_VOID SME_Hal_VDisp_Destory(IN SME_HAL_VDISP_PIPE_HDL hdlPipe)
{
    ST_SME_HAL_HI_VDISP_PIPE* pstPipe  = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    if(pstPipe == NULL)
        return;

    //if(pstPipe->pstVoutDev != NULL)
    //{
    //    free(pstPipe->pstVoutDev);
    //    pstPipe->pstVoutDev = NULL;
    //}

    free(pstPipe);

    return;

}

V_UINT32 SME_Hal_VDisp_Init(IN SME_HAL_VDISP_PIPE_HDL hdlPipe)
{
    V_UINT32 u32Ret                     = ICS_SUCCESS;
    ST_SME_HAL_HI_VDISP_PIPE* pstPipe  = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
#ifndef LINUX_OS
    hw_module_t const* module;
#endif
    S32 s32Ret;
    ICS_LOGW("%s:VDisp_Init in...", SMEPLAYER_STAT_TAG_TIME);

    do
    {
        UTILS_MLOGW_BRK_VAL_IF(NULL == pstPipe , u32Ret, ICS_FAIL, ("invalid hdl!"));

        HI_SYS_Init();

#ifndef LINUX_OS

        if (hw_get_module(VOUT_HARDWARE_MODULE_ID, &module) != 0)
        {
            ICS_LOGE("%s module not found!\n",  VOUT_HARDWARE_MODULE_ID);
            return ICS_FAIL;
        }

        /* HAL视频输出模块初始化 */
        s32Ret = vout_open(module, &pstPipe->pstVoutDev);
        if(s32Ret != SUCCESS)
#else
        pstPipe->pstVoutDev = getVoutDevice();
        if (NULL == pstPipe->pstVoutDev)
#endif
        {
            HI_SYS_DeInit();
            ICS_LOGE("open VoutDev failed");
            return ICS_FAIL;
        }

        /*init video output device. ps:VOUT_INIT_PARAMS_S not used,so set NULL is ok*/
        s32Ret = pstPipe->pstVoutDev->vout_init(pstPipe->pstVoutDev, NULL);
        if(s32Ret != SUCCESS)
        {
#ifndef LINUX_OS
            vout_close(pstPipe->pstVoutDev);
#endif
            HI_SYS_DeInit();
            ICS_LOGE("init VoutDev failed");
            return ICS_FAIL;
        }

        /* 打开一个视频输出实例,VOUT_OPEN_PARAMS_S not used ,so set NULL is OK  */
        s32Ret = pstPipe->pstVoutDev->vout_open_channel(pstPipe->pstVoutDev, &pstPipe->phVout, NULL);
        if(s32Ret != SUCCESS)
        {
#ifndef LINUX_OS
            vout_close(pstPipe->pstVoutDev);
#endif
            HI_SYS_DeInit();
            ICS_LOGE("open VoutDev instance failed");
            return ICS_FAIL;
        }

        /* 创建一个窗口 */
        s32Ret = pstPipe->pstVoutDev->vout_window_create(pstPipe->pstVoutDev, pstPipe->phVout, &pstPipe->phVoutWindow, NULL);
        if(s32Ret != SUCCESS)
        {
            pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, pstPipe->phVout, NULL);
#ifndef LINUX_OS
            vout_close(pstPipe->pstVoutDev);
#endif
            HI_SYS_DeInit();
            ICS_LOGE("create VoutDev windows failed");
            return ICS_FAIL;
        }

#ifndef LINUX_OS
        U32 hSrc = (U32)SME_ID_USER << 16;
        s32Ret = pstPipe->pstVoutDev->vout_window_attach_input(pstPipe->pstVoutDev, pstPipe->phVoutWindow, hSrc);
#else
        /* 将窗口enable */
        s32Ret = pstPipe->pstVoutDev->vout_window_unmute(pstPipe->pstVoutDev, pstPipe->phVoutWindow);
#endif
        if(s32Ret != SUCCESS)
        {
            pstPipe->pstVoutDev->vout_window_destroy(pstPipe->pstVoutDev, pstPipe->phVout, pstPipe->phVoutWindow, NULL);
            pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, pstPipe->phVout, NULL);
#ifndef LINUX_OS
            vout_close(pstPipe->pstVoutDev);
#endif
            HI_SYS_DeInit();
            ICS_LOGE("enable VoutDev windows failed");
            return ICS_FAIL;
        }

#ifndef LINUX_OS
        s32Ret = pstPipe->pstVoutDev->vout_window_unmute(pstPipe->pstVoutDev, pstPipe->phVoutWindow);
        if(s32Ret != SUCCESS)
        {
            ICS_LOGE("SME_Hal_VDisp_Init vout_window_unmute failed");
        }
#endif

        /* 与资源无关的初始化操作 */
        if(ICS_SUCCESS != VOS_InitThdMutex(&pstPipe->stMutex, NULL))
        {
            ICS_LOGE("init pstPipe->stMutex failed, it may cause SME_Hal_VDisp_Init failed");
            if(pstPipe->phVoutWindow)
            {
                /*disable windows*/
#ifndef LINUX_OS
                U32 hSrc = (U32)SME_ID_USER << 16;
                s32Ret = pstPipe->pstVoutDev->vout_window_detach_input(pstPipe->pstVoutDev, pstPipe->phVoutWindow, hSrc);
                if(s32Ret == FAILURE)
                {
                    ICS_LOGE("vout_window_detach_input failed");
                    //return ICS_FAIL;
                }
#else
                ICS_LOGE("disable vout_windows");
                s32Ret = pstPipe->pstVoutDev->vout_window_mute(pstPipe->pstVoutDev, pstPipe->phVoutWindow);
                if(s32Ret == FAILURE)
                {
                    ICS_LOGE("vout_window_mute failed");
                    return ICS_FAIL;
                }
#endif

                ICS_LOGE("destory vout_windows");
                /*destory windows*/
                s32Ret = pstPipe->pstVoutDev->vout_window_destroy(pstPipe->pstVoutDev, 0, pstPipe->phVoutWindow, NULL);
                if(s32Ret == FAILURE)
                {
                    ICS_LOGE("vout_window_destroy failed,maybe not use vout_window_detach_input");
                    return ICS_FAIL;
                }
            }
#if 1 //#ifdef LINUX_OS
            ICS_LOGE("close channel in");
            if(pstPipe->phVout != (HANDLE)NULL)
            {
                /* close channel */
                ICS_LOGE("real close channel in");
                s32Ret = pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, pstPipe->phVout, NULL);
                //s32Ret = pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, NULL, NULL);
                if(s32Ret == FAILURE)
                {
                    ICS_LOGE("vout_close_channel failed");
                    return ICS_FAIL;
                }
            }

            ICS_LOGE("vout_close in");
#endif
#ifndef LINUX_OS
            s32Ret = vout_close(pstPipe->pstVoutDev);
            if(s32Ret == FAILURE)
            {
                ICS_LOGE("vout_close failed");
                return ICS_FAIL;
            }
#endif
            HI_SYS_DeInit();
            return ICS_FAIL;
        }

        pstPipe->u64FrameIdx = 0;
        pstPipe->u64DeqFailNum = 0;

    }while(FALSE);

    ICS_LOGW("%s:VDisp_Init out(%d)", SMEPLAYER_STAT_TAG_TIME, u32Ret);
    return u32Ret;
}

V_UINT32 SME_Hal_VDisp_DeInit(IN SME_HAL_VDISP_PIPE_HDL hdlPipe)
{
    V_UINT32 u32Ret                     = ICS_SUCCESS;
    ST_SME_HAL_HI_VDISP_PIPE* pstPipe  = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    S32 s32Ret;

    ICS_LOGW("%s:VDisp_Init in...", SMEPLAYER_STAT_TAG_TIME);

    if(pstPipe == NULL || pstPipe->pstVoutDev== NULL)
        return u32Ret;

    VOS_DeInitThdMutex(&pstPipe->stMutex);

    ICS_LOGE("SME_Hal_VDisp_DeInit in...");
    if(pstPipe->phVoutWindow)
    {
        /*disable windows*/
#ifndef LINUX_OS
        U32 hSrc = (U32)SME_ID_USER << 16;
        s32Ret = pstPipe->pstVoutDev->vout_window_detach_input(pstPipe->pstVoutDev, pstPipe->phVoutWindow, hSrc);
        if(s32Ret == FAILURE)
        {
            ICS_LOGE("vout_window_detach_input failed");
            //return ICS_FAIL;
        }
#else
        ICS_LOGE("disable vout_windows");
        s32Ret = pstPipe->pstVoutDev->vout_window_mute(pstPipe->pstVoutDev, pstPipe->phVoutWindow);
        if(s32Ret == FAILURE)
        {
            ICS_LOGE("vout_window_mute failed");
            return ICS_FAIL;
        }
#endif

        ICS_LOGE("destory vout_windows");
        /*destory windows*/
        s32Ret = pstPipe->pstVoutDev->vout_window_destroy(pstPipe->pstVoutDev, 0, pstPipe->phVoutWindow, NULL);
        if(s32Ret == FAILURE)
        {
            ICS_LOGE("vout_window_destroy failed,maybe not use vout_window_detach_input");
            return ICS_FAIL;
        }
    }
#if 1 //#ifdef LINUX_OS
    ICS_LOGE("close channel in");
    if(pstPipe->phVout != (HANDLE)NULL)
    {
        /* close channel */
        ICS_LOGE("real close channel in");
        s32Ret = pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, pstPipe->phVout, NULL);
        //s32Ret = pstPipe->pstVoutDev->vout_close_channel(pstPipe->pstVoutDev, NULL, NULL);
        if(s32Ret == FAILURE)
        {
            ICS_LOGE("vout_close_channel failed");
            return ICS_FAIL;
        }
    }

    ICS_LOGE("vout_close in");
#endif
#ifndef LINUX_OS
    s32Ret = vout_close(pstPipe->pstVoutDev);
    if(s32Ret == FAILURE)
    {
        ICS_LOGE("vout_close failed");
        return ICS_FAIL;
    }
#endif

    ICS_LOGE("HI_SYS_DeInit in");
    HI_SYS_DeInit();
    ICS_LOGE("HI_SYS_DeInit out");

    ICS_LOGW("%s:VDisp_Init out(%d)", SMEPLAYER_STAT_TAG_TIME, u32Ret);

    return u32Ret;

}
//lint -e58
V_UINT32 SME_Hal_VDisp_Updata_OutRect(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, const HI_RECT_S * pstRect)
{
    ST_SME_HAL_HI_VDISP_PIPE* pstPipe  = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    S32 s32Ret;

    UTILS_MLOGW_RET_VAL_IF(NULL == pstPipe
        || (HANDLE)NULL == pstPipe->phVoutWindow
        || (HANDLE)NULL == pstPipe->pstVoutDev || NULL == pstRect,
        ICS_FAIL, ("invalid hdl!"));

    ICS_LOGW("%s:set_output_rect begin...:x=%d，y=%d, width =%d, height=%d",
        SMEPLAYER_STAT_TAG_TIME, pstRect->s32X, pstRect->s32Y,
        pstRect->s32Width, pstRect->s32Height);
    s32Ret = pstPipe->pstVoutDev->vout_window_set_output_rect(pstPipe->pstVoutDev, pstPipe->phVoutWindow,
        (V_UINT32)pstRect->s32X, (V_UINT32)pstRect->s32Y, (V_UINT32)pstRect->s32Width, (V_UINT32)pstRect->s32Height);
    ICS_LOGW("%s:set_output_rect, end(%x)", SMEPLAYER_STAT_TAG_TIME, s32Ret);

    if(SUCCESS == s32Ret)
    {
        ICS_LOGW("set vout_window_set_output_rect:[%d %d %d %d] succeed",pstRect->s32X, pstRect->s32Y,
            pstRect->s32Width, pstRect->s32Height);
        return ICS_SUCCESS;
    }
    else
    {
        ICS_LOGW("set vout_window_set_output_rect:[%d %d %d %d] failed,err:%d",pstRect->s32X, pstRect->s32Y,
            pstRect->s32Width, pstRect->s32Height,s32Ret);
        return ICS_FAIL;
    }

}
//lint +e58



static V_VOID SME_SetVoutFrameInfoDefaultValue(const ST_SME_HAL_HI_VDISP_PIPE* pstHalPipe,
    VOUT_FRAME_INFO_S* pstFrame, V_UINT32 u32W, V_UINT32 u32H,
        V_UINT32 u32PhyAddr, V_UINT32 u32Duration)
{
    V_UINT32 u32StrW = (u32W + 0xF) & 0xFFFFFFF0UL;
    E_SME_MEM_SECURE_RET eMemSecRet;

    eMemSecRet = VOS_Memset_S(pstFrame, sizeof(VOUT_FRAME_INFO_S), 0, sizeof(VOUT_FRAME_INFO_S));
    if(eMemSecRet != E_SME_MEM_OK){
        ICS_LOGE("init pstFrame failed,ret:%d", eMemSecRet);
        return;
    }

    pstFrame->u32FrameIndex             = (U32)pstHalPipe->u64FrameIdx;
    pstFrame->stVideoFrameAddr[0].u32YAddr = u32PhyAddr;
    pstFrame->stVideoFrameAddr[0].u32CAddr = u32PhyAddr + u32StrW * u32H;
    //pstFrame->stVideoFrameAddr[0].u32CrAddr = u32PhyAddr + u32StrW * u32H;

    pstFrame->stVideoFrameAddr[0].u32YStride = u32StrW;
    pstFrame->stVideoFrameAddr[0].u32CStride = u32StrW;
    pstFrame->stVideoFrameAddr[0].u32CrStride = u32StrW;
    pstFrame->u32Width                  = u32W;
    pstFrame->u32Height                 = u32H;
#ifndef LINUX_OS
    pstFrame->u32SrcPts                 = (U32) - 1;
    pstFrame->u32Pts                    = (U32) - 1;
#else
    pstFrame->s64SrcPts                 = - 1;
    pstFrame->s64Pts                    = - 1;
#endif
    pstFrame->u32AspectWidth            = 0;
    pstFrame->u32AspectHeight           = 0;
    if(u32Duration != 0)
    {
        pstFrame->stFrameRate.u32fpsInteger = M_SME_SEC_TO_MS / u32Duration;
        pstFrame->stFrameRate.u32fpsDecimal
            = (M_SME_SEC_TO_MS * M_SME_MS_TO_US ) / u32Duration
            - pstFrame->stFrameRate.u32fpsInteger * M_SME_MS_TO_US;
    }
    else
    {
        pstFrame->stFrameRate.u32fpsInteger = 30;
        pstFrame->stFrameRate.u32fpsDecimal = 0;
        ICS_LOGW("use default fps:30.0");
    }
    //ICS_LOGW("use1 fps:%u:%u",
    //    pstFrame->stFrameRate.u32fpsInteger, pstFrame->stFrameRate.u32fpsDecimal);

    //pstFrame->stFrameRate.u32fpsInteger = 50;
    //pstFrame->stFrameRate.u32fpsDecimal = 0;
    //ICS_LOGW("use fps2:%u:%u",
    //    pstFrame->stFrameRate.u32fpsInteger, pstFrame->stFrameRate.u32fpsDecimal);

    //pstFrame->enVideoFormat             = VOUT_FORMAT_YUV_SEMIPLANAR_420;
    pstFrame->enVideoFormat             = VOUT_FORMAT_YUV_SEMIPLANAR_420_UV;

    pstFrame->bProgressive              = FALSE;//HI_TRUE;
    pstFrame->enFieldMode               = VOUT_VIDEO_FIELD_TOP;//HI_UNF_VIDEO_FIELD_BUTT;
    pstFrame->bTopFieldFirst            = FALSE;//HI_TRUE;
    pstFrame->enFramePackingType        = VOUT_FRAME_PACKING_TYPE_NONE;
    pstFrame->u32Circumrotate           = 0;
    pstFrame->bVerticalMirror           = FALSE;
    pstFrame->bHorizontalMirror         = FALSE;

    pstFrame->u32DisplayCenterX         = (U32)(u32W >> 1);
    pstFrame->u32DisplayCenterY         = (U32)(u32H >> 1);
    pstFrame->u32DisplayWidth           = (U32)u32W;
    pstFrame->u32DisplayHeight          = (U32)u32H;
    pstFrame->u32ErrorLevel             = 0;
    /* 当前非安全帧 */
    pstFrame->bSecurityFrame            = FALSE;
    //pstFrame->u32Private[0] = 0xf3f3f3f3;

    return;
}

E_SME_HIVO_ERR SME_Hal_VDisp_QueueFrame(IN SME_HAL_VDISP_PIPE_HDL hdlPipe,
        IN const ST_SME_VIDEO_FRAME* pstVideoFrame)
{
    E_SME_HIVO_ERR                  eRet          = E_SME_HIVO_ERR_NONE;
    ST_SME_HAL_HI_VDISP_PIPE*       pstPipe        = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    V_UINT32                        u32PhyAddr      = 0;
    S32                             s32Err;
    HI_S32                          i32GetPhyErr;
    V_UINT32                        u32Len          = 0;
    VOUT_FRAME_INFO_S               stVoutFrame;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstPipe || NULL == pstVideoFrame
                           || NULL == pstVideoFrame->pvAddr[0], E_SME_HIVO_ERR_INVALID_ARGS,
                           ("SME_HiVO_QueueFrame:invalid args!"));

    i32GetPhyErr = HI_MMZ_GetPhyaddr(pstVideoFrame->pvAddr[0], &u32PhyAddr, &u32Len);
    UTILS_MLOGE_RET_VAL_IF(HI_SUCCESS != i32GetPhyErr, E_SME_HIVO_ERR_FATAL,
       ("HI_MMZ_GetPhyaddr:pvAddr=%p, u32Len=%u", pstVideoFrame->pvAddr[0], u32Len));

    pstPipe->u64FrameIdx++;

    SME_SetVoutFrameInfoDefaultValue(pstPipe, &stVoutFrame, pstVideoFrame->u32Width,
         pstVideoFrame->u32Height, u32PhyAddr, pstVideoFrame->u32Duration);

    s32Err = pstPipe->pstVoutDev->vout_window_queue_frame(pstPipe->pstVoutDev, pstPipe->phVoutWindow, &stVoutFrame);

    if (SUCCESS == s32Err)
    {
        eRet = E_SME_HIVO_ERR_NONE;
    }
    else if(HI_ERR_VO_BUFQUE_FULL == s32Err)
    {
        ICS_LOGE("buf que full");
        eRet = E_SME_HIVO_ERR_BUF_FULL;
    }
    else
    {
        ICS_LOGE("hal queue failed");
        eRet = E_SME_HIVO_ERR_FATAL;
    }

    return eRet;
}

E_SME_HIVO_ERR SME_Hal_VDisp_DeQueueFrame(IN SME_HAL_VDISP_PIPE_HDL hdlPipe,
   OUT ST_SME_VIDEO_FRAME* pstVideoFrame)
{
    E_SME_HIVO_ERR                  eRet          = E_SME_HIVO_ERR_NONE;
    ST_SME_HAL_HI_VDISP_PIPE*       pstPipe        = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    S32                             s32Err;
    VOUT_FRAME_INFO_S               stVoutFrame;
    E_SME_MEM_SECURE_RET            eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstPipe || NULL == pstVideoFrame,
    E_SME_HIVO_ERR_INVALID_ARGS, ("SME_HiVO_DeQueueFrame:invalid args!"));



    eMemSecRet = VOS_Memset_S(&stVoutFrame, sizeof(VOUT_FRAME_INFO_S), 0, sizeof(VOUT_FRAME_INFO_S));
    if(eMemSecRet != E_SME_MEM_OK){
        ICS_LOGE("init stVoutFrame failed,ret:%d", eMemSecRet);
        return E_SME_HIVO_ERR_FATAL;
    }

    s32Err = pstPipe->pstVoutDev->vout_window_dequeue_frame(pstPipe->pstVoutDev, pstPipe->phVoutWindow, &stVoutFrame, DEQUEUE_TIMEOUT);

    if (SUCCESS == s32Err)
    {
        pstVideoFrame->pvAddr[1] = (V_VOID*)stVoutFrame.stVideoFrameAddr[0].u32YAddr;
        eRet = E_SME_HIVO_ERR_NONE;
    }
    else
    {
        pstPipe->u64DeqFailNum++;
        eRet = E_SME_HIVO_ERR_FATAL;
        if((pstPipe->u64DeqFailNum - 1)%100 == 0)
        {
            ICS_LOGD("SME_Hal_VDisp_DeQueueFrame failed, faile number is %lld", pstPipe->u64DeqFailNum);
        }

    }

    return eRet;
}

E_SME_HIVO_ERR SME_Hal_VDisp_GetRenderDelay(IN SME_HAL_VDISP_PIPE_HDL hldPipe, OUT V_UINT64 *delay )
{
    E_SME_HIVO_ERR              eRet                = E_SME_HIVO_ERR_NONE;
    ST_SME_HAL_HI_VDISP_PIPE*   pstPipe             = (ST_SME_HAL_HI_VDISP_PIPE*)hldPipe;
    V_INT32                     s32Err;
    V_UINT32                    u32DelayTime        = 0;
    V_UINT32                    u32DispRate         = 0;
    V_UINT32                    u32FrameNumInBufQn  = 0;

    UTILS_MLOGE_RET_VAL_IF( NULL == pstPipe || NULL == delay,
        E_SME_HIVO_ERR_INVALID_ARGS, ("SME_Hal_VDisp_GetRenderDelay: invalid args!"));

#ifdef DEBUG
    ICS_LOGE("@cat_clock@video@vsink@vo@in: SME_Hal_VDisp_GetRenderLatency");
#endif

    s32Err = pstPipe->pstVoutDev->vout_window_get_playinfo(
                                                         pstPipe->pstVoutDev,
                                                         pstPipe->phVoutWindow,
                                                        &u32DelayTime,
                                                        &u32DispRate,
                                                        &u32FrameNumInBufQn );
    if ( SUCCESS == s32Err )
    {
        *delay = u32DelayTime * 1000000ULL;
        eRet     = E_SME_HIVO_ERR_NONE;
    }
    else
    {
        eRet     = E_SME_HIVO_ERR_FATAL;
        ICS_LOGW("SME_Hal_VDisp_DeQueueFrame failed");
    }

    ICS_LOGD("@-cat_clock@video@vsink@vo@ playinfo : u32DelayTime:%d,"
        "u32DispRate:%u,u32FrameNumInBufQn:%u",
        u32DelayTime, u32DispRate, u32FrameNumInBufQn);

    return eRet;
}

//lint -e58
V_UINT32 SME_Hal_VDisp_Reset(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, IN E_SME_HIVO_SWITCH_TYPE eSwitchMode)
{
    ST_SME_HAL_HI_VDISP_PIPE*       pstPipe        = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    S32                             sRet;
    VOUT_WINDOW_SWITCH_MODE_E       eVoutSwitchMode;
    if(NULL == pstPipe || (HANDLE)NULL == pstPipe->pstVoutDev
        || (HANDLE)NULL == pstPipe->phVoutWindow
        || (eSwitchMode != E_SME_HIVO_SWITCH_TYPE_LAST
            && eSwitchMode != E_SME_HIVO_SWITCH_TYPE_BLACK))
    {
        ICS_LOGE("input para is error");
        return ICS_FAIL;
    }

    eVoutSwitchMode = (eSwitchMode == E_SME_HIVO_SWITCH_TYPE_LAST) ? VOUT_WINDOW_SWITCH_MODE_FREEZE : VOUT_WINDOW_SWITCH_MODE_BLACK;
    sRet = pstPipe->pstVoutDev->vout_window_reset(pstPipe->pstVoutDev, pstPipe->phVoutWindow, eVoutSwitchMode);
    if(sRet == SUCCESS)
    {
        ICS_LOGE("reset switch mode : %u succeed", eVoutSwitchMode);
        return ICS_SUCCESS;
    }
    else
    {
        ICS_LOGE("reset switch mode : %u failed", eVoutSwitchMode);
        return ICS_FAIL;
    }
}
//lint +e58




V_VOID SME_Disp_Full(IN const ST_OUT_RECT * pstDispRect, OUT ST_OUT_RECT * pstOutRect)
{
    printf("full IN pstDispRect(%u,%u,%u,%u)\n", pstDispRect->u32OutX, pstDispRect->u32OutY, pstDispRect->u32OutWidth, pstDispRect->u32OutHeight);
    pstOutRect->u32OutWidth = (V_UINT32)(pstDispRect->u32OutWidth &~ 15);
    pstOutRect->u32OutHeight = (V_UINT32)(pstDispRect->u32OutHeight &~ 15);
    pstOutRect->u32OutX = (V_UINT32)pstDispRect->u32OutX + (V_UINT32)(pstDispRect->u32OutWidth - (V_UINT32)pstOutRect->u32OutWidth)/2;
    pstOutRect->u32OutY = (V_UINT32)pstDispRect->u32OutY + (V_UINT32)(pstDispRect->u32OutHeight - (V_UINT32)pstOutRect->u32OutHeight)/2;
    printf("full OUT stOutRect:(%u,%u,%u,%u)\n",pstOutRect->u32OutX, pstOutRect->u32OutY, pstOutRect->u32OutWidth, pstOutRect->u32OutHeight);

    return ;
}

V_VOID SME_Disp_Fitin(SME_HAL_VDISP_PIPE_HDL hdlPipe, const ST_DISP_HAL_RATIO* p_disp_radio, const ST_OUT_RECT* p_win_rect, ST_OUT_RECT* p_disp_win)
{
    ST_DISP_SCREEN stVirScreen;
    SME_Get_Virtual_Screen(hdlPipe, &stVirScreen);
    if(ICS_FAIL == SME_Calcu_Win_Rect(*p_disp_radio, stVirScreen, p_win_rect, p_disp_win))
    {
        /* if calc filed, maybe the input rect is invaild, so we send input rect to HISI directly*/
        ICS_LOGW("SME_Calcu_Win_Rect failed");
        p_disp_win->u32OutX = p_win_rect->u32OutX;
        p_disp_win->u32OutY = p_win_rect->u32OutY;
        p_disp_win->u32OutWidth = p_win_rect->u32OutWidth;
        p_disp_win->u32OutHeight = p_win_rect->u32OutHeight;
    }
    return ;
}

//lint -e529
//lint -e438
V_VOID SME_Get_Virtual_Screen(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, OUT ST_DISP_SCREEN * pstVirScreen)
{
    ST_SME_HAL_HI_VDISP_PIPE* pstPipe = (ST_SME_HAL_HI_VDISP_PIPE*)hdlPipe;
    HI_S32  i32GetVWinSize = HI_SUCCESS;
    HI_U32  u32VirScreenW = VIRTUAL_SCREEN_WIDTH;
    HI_U32  u32VirScreenH = VIRTUAL_SCREEN_HEIGHT;

    if(NULL == pstPipe || NULL == pstPipe->pstVoutDev)
    {
        ICS_LOGE("hdlPipe IS NULL, maybe init Vdisp error, pstPipe:%p", pstPipe);
        pstVirScreen->u32DispWidth  = VIRTUAL_SCREEN_WIDTH;
        pstVirScreen->u32DispHeight = VIRTUAL_SCREEN_HEIGHT;
        return;
    }
#ifdef LINUX_OS
    pstVirScreen->u32DispWidth  = VIRTUAL_SCREEN_WIDTH;
    pstVirScreen->u32DispHeight = VIRTUAL_SCREEN_HEIGHT;
#else
    if(pstPipe->pstVoutDev->vout_window_get_virtual_size)
    {
        i32GetVWinSize = pstPipe->pstVoutDev->vout_window_get_virtual_size(pstPipe->pstVoutDev, pstPipe->phVoutWindow, &u32VirScreenW, &u32VirScreenH);
    }
    else
    {
        ICS_LOGE("pstPipe->pstVoutDev->vout_window_get_virtual_size is NULL ,maybe hisi SDK has something wrong");
    }
    if(i32GetVWinSize == HI_SUCCESS)
    {
        pstVirScreen->u32DispWidth = u32VirScreenW;
        pstVirScreen->u32DispHeight = u32VirScreenH;
    }
    else
    {
        pstVirScreen->u32DispWidth  = VIRTUAL_SCREEN_WIDTH;
        pstVirScreen->u32DispHeight = VIRTUAL_SCREEN_HEIGHT;
        ICS_LOGE("get virtual screen failed");
    }
#endif

    return;
}
//lint +e529
//lint +e438

//lint -e1746
V_UINT32 SME_Calcu_Win_Rect(IN const ST_DISP_HAL_RATIO stUI, IN  ST_DISP_SCREEN stScreen,
    const ST_OUT_RECT* pstDispRect, ST_OUT_RECT* pOutRect)
{
    //UI 宽高比
    V_FLOAT fUIRatioWidth = (V_FLOAT)stUI.u32DispRatioW;
    V_FLOAT fUIRatioHeight= (V_FLOAT)stUI.u32DispRatioH;
    //虚拟屏幕宽高
    V_FLOAT fVirScreenWidth = (V_FLOAT)stScreen.u32DispWidth;
    V_FLOAT fVirScreenHeight= (V_FLOAT)stScreen.u32DispHeight;

    //小窗位置、大小
    V_FLOAT fWinWidth ;
    V_FLOAT fWinHeight;
    V_UINT32 u32StartX;
    V_UINT32 u32StartY;
    if(pstDispRect != NULL)
    {
        fWinWidth = (V_FLOAT)pstDispRect->u32OutWidth;
        fWinHeight= (V_FLOAT)pstDispRect->u32OutHeight;
        u32StartX = pstDispRect->u32OutX;
        u32StartY = pstDispRect->u32OutY;
    }
    else
    {
        fWinWidth = 0;
        fWinHeight = 0;
        u32StartX = 0;
        u32StartY = 0;
    }

    //小窗显示比例，UI显示比例
    V_FLOAT fWinRatio;
    V_FLOAT fUIRatio;
    ST_OUT_RECT stOutRectTmp;
    E_SME_MEM_SECURE_RET            eMemSecRet;
    eMemSecRet = VOS_Memset_S((void*)&stOutRectTmp, sizeof(ST_OUT_RECT), 0x0, sizeof(ST_OUT_RECT));
    if(eMemSecRet != E_SME_MEM_OK){
        ICS_LOGE("init stOutRectTmp failed,ret:%d", eMemSecRet);
        return ICS_FAIL;
    }

    if(fUIRatioWidth == 0 || fUIRatioHeight == 0 || fVirScreenWidth == 0 ||
        fVirScreenHeight == 0)
    {
        ICS_LOGW("the resolution of UI , VirScreen or WinScreen contains zero:[%f %f %f %f]",
            fUIRatioWidth, fUIRatioHeight, fVirScreenWidth, fVirScreenHeight);
        return ICS_FAIL;
    }
    if(fWinWidth == 0 && fWinHeight == 0){
        fWinWidth = fVirScreenWidth;
        fWinHeight = fVirScreenHeight;
    }
    else if(fWinWidth == 0 || fWinHeight == 0){
        ICS_LOGW("window size contains zero");
        return ICS_FAIL;
    }

    fWinRatio = fWinWidth / fWinHeight;
    fUIRatio = fUIRatioWidth / fUIRatioHeight;

    /* Caculate the real W&H and the start of X,Y */
    if(fWinRatio > fUIRatio)
    {
        /* 宽加黑边 */
        stOutRectTmp.u32OutHeight = ((V_UINT32)(fWinHeight)) &~ 15;
        stOutRectTmp.u32OutWidth = (V_UINT32)( fUIRatio * fWinHeight);
        /* 16字节对齐 */
        stOutRectTmp.u32OutWidth = (stOutRectTmp.u32OutWidth) &~ 15;
        stOutRectTmp.u32OutX = (V_UINT32)((fWinWidth - stOutRectTmp.u32OutWidth)/2 + u32StartX);
        stOutRectTmp.u32OutY = u32StartY + (V_UINT32)((fWinHeight - stOutRectTmp.u32OutHeight)/2);

    }
    else if(fWinRatio < fUIRatio)
    {
        /* 高加黑边 */
        stOutRectTmp.u32OutHeight = (V_UINT32)(fWinWidth / fUIRatio);
        stOutRectTmp.u32OutWidth = ((V_UINT32)(fWinWidth)) &~ 15;
        /* 16字节对齐 */
        stOutRectTmp.u32OutHeight = (stOutRectTmp.u32OutHeight) &~ 15;
        stOutRectTmp.u32OutX = u32StartX + (V_UINT32)((fWinWidth - stOutRectTmp.u32OutWidth)/2);
        stOutRectTmp.u32OutY = (V_UINT32)((fWinHeight - stOutRectTmp.u32OutHeight)/2 + u32StartY);
    }
    else
    {
        /* ui比例和Screen一致，无需加黑边 */
        stOutRectTmp.u32OutWidth = ((V_UINT32)(fWinWidth)) &~ 15;
        stOutRectTmp.u32OutHeight = ((V_UINT32)(fWinHeight)) &~ 15;
        stOutRectTmp.u32OutX = u32StartX + (V_UINT32)((fWinWidth - stOutRectTmp.u32OutWidth)/2);
        stOutRectTmp.u32OutY = u32StartY + (V_UINT32)((fWinHeight - stOutRectTmp.u32OutHeight)/2);
    }
    if(pstDispRect != NULL)
    {
        ICS_LOGW("fitin IN pstDispRect(%u,%u,%u,%u)\n", pstDispRect->u32OutX, pstDispRect->u32OutY, pstDispRect->u32OutWidth, pstDispRect->u32OutHeight);
    }
    if(pOutRect != NULL)
    {
        pOutRect->u32OutX = stOutRectTmp.u32OutX;
        pOutRect->u32OutY = stOutRectTmp.u32OutY;
        pOutRect->u32OutHeight = stOutRectTmp.u32OutHeight;
        pOutRect->u32OutWidth = stOutRectTmp.u32OutWidth;
        ICS_LOGW("fitin OUT stOutRect:(%u,%u,%u,%u)\n",pOutRect->u32OutX, pOutRect->u32OutY, pOutRect->u32OutWidth, pOutRect->u32OutHeight);
    }

    return ICS_SUCCESS;

}
//lint +e1746
