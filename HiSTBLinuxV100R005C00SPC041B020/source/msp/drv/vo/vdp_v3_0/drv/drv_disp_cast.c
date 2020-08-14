
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_cast.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_com.h"
#include "drv_disp_cast.h"
#include "drv_venc_ext.h"
#include "hi_drv_module.h"
#include "drv_disp_priv.h"
#include "drv_display.h"
#include "hi_drv_stat.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

CAST_RELEASE_PTR_S g_stReleasePtrArray[DISP_CAST_BUFFER_MAX_NUMBER];


static HI_S32 CastCheckCfg(HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    if (  (pstCfg->u32Width < DISP_CAST_MIN_W)
        ||(pstCfg->u32Width > DISP_CAST_MAX_W)
        ||(pstCfg->u32Height < DISP_CAST_MIN_H)
        ||(pstCfg->u32Height > DISP_CAST_MAX_H)
        ||( (pstCfg->u32Width & 0x1)  != 0)
        ||( (pstCfg->u32Height & 0x3) != 0)
        )
    {
        DISP_ERROR("Cast w= %d or h=%d invalid\n", pstCfg->u32Width, pstCfg->u32Height);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (  (pstCfg->eFormat != HI_DRV_PIX_FMT_NV21)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_NV12)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_NV16_2X1)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_NV61_2X1)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_YUYV)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_YVYU)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_UYVY)
        )
    {
        DISP_ERROR("Cast pixfmt = %d invalid\n", pstCfg->eFormat);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (  (pstCfg->u32BufNumber < DISP_CAST_BUFFER_MIN_NUMBER)
        ||(pstCfg->u32BufNumber > DISP_CAST_BUFFER_MAX_NUMBER) )
    {
        DISP_ERROR("Cast u32BufNumber =%d invalid\n", pstCfg->u32BufNumber);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstCfg->bUserAlloc > HI_TRUE)
        ||(pstCfg->bLowDelay > HI_TRUE))
    {
        DISP_ERROR("bUserAlloc: %d or bLowDelay =%d invalid.\n", pstCfg->bUserAlloc, pstCfg->bLowDelay);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if(pstCfg->bUserAlloc)
    {
        DISP_ERROR("Cast not support User Alloc memory\n");
        return HI_ERR_DISP_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}

static HI_S32 CastSetFrameDemoPartA(DISP_CAST_S *cast_ptr,
                                    HI_DRV_DISP_CAST_CFG_S *pstCfg,
                                    DISP_CAST_ATTR_S *pstAttr)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    DISP_CAST_PRIV_FRAME_S *pstPriv;

    // set frame demo
    DISP_MEMSET(pstAttr, 0, sizeof(DISP_CAST_ATTR_S));

    pstAttr->stOut.s32Width  = (HI_S32)pstCfg->u32Width;
    pstAttr->stOut.s32Height = (HI_S32)pstCfg->u32Height;

    pstFrame = &pstAttr->stFrameDemo;
    pstFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
    pstFrame->ePixFormat = pstCfg->eFormat;

    pstFrame->bProgressive = HI_TRUE;
    pstFrame->u32Width  = pstCfg->u32Width;
    pstFrame->u32Height = pstCfg->u32Height;
    pstFrame->stDispRect = pstAttr->stOut;

    pstPriv = (DISP_CAST_PRIV_FRAME_S *)&(pstFrame->u32Priv[0]);

    pstPriv->cast_ptr = cast_ptr;
    pstPriv->stPrivInfo.u32PlayTime = 1;

    return HI_SUCCESS;
}

#define Cast_ERROR_DEAL(a, flag)  do{                    \
        if (HI_SUCCESS !=   (a))                    \
        {               \
            return HI_FAILURE;\
        }\
    }while(0);

static HI_S32 Cast_GetFrame(DISP_CAST_S *pstCast,
                     HI_U32 *u32BufId,
                     HI_U32 buf_type,
                     HI_DRV_VIDEO_FRAME_S *frameInfo)
{
    if(buf_type == 1)
    {
        Cast_ERROR_DEAL(BP_GetFullBuf(&pstCast->stBP, u32BufId), 0);
        Cast_ERROR_DEAL(BP_DelFullBuf(&pstCast->stBP, *u32BufId), 1);
        Cast_ERROR_DEAL(BP_GetFrame(&pstCast->stBP, *u32BufId, frameInfo), 1);
    } else {
        Cast_ERROR_DEAL(BP_GetEmptyBuf(&pstCast->stBP, u32BufId), 0);
        Cast_ERROR_DEAL(BP_DelEmptyBuf(&pstCast->stBP, *u32BufId), 1);
        Cast_ERROR_DEAL(BP_GetFrame(&pstCast->stBP, *u32BufId, frameInfo), 1);
    }
    return HI_SUCCESS;
}

static HI_BOOL Cast_CheckOutputSizeChange(DISP_CAST_S *pstCast,
                                          HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if ((pstFrame->u32Width == pstCast->stAttr.stOut.s32Width)
        && (pstFrame->u32Height == pstCast->stAttr.stOut.s32Height))
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}

static HI_S32 Cast_KThread_ReAllocate(HI_VOID *pstCastmp)
{
    HI_U32 i = 0;
    HI_DRV_VIDEO_FRAME_S stFrame;
    DISP_CAST_S *pstCast = HI_NULL;

    memset((void*)&stFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    pstCast = (DISP_CAST_S *)pstCastmp;

    while (1)
    {
        if (kthread_should_stop())
            break;

        for (i = 0; i < DISP_CAST_BUFFER_MAX_NUMBER; i++)
        {
            /*need to reallocate.*/
            if (CAST_RETRIVE_NODE_REALLOCATE == atomic_read(&g_stReleasePtrArray[i].atReleaseNodeStatus))
            {
                if (BP_ReAllocBuf(&pstCast->stBP, g_stReleasePtrArray[i].u32BufID))
                    break;

                /*set the node to normal status.*/
                atomic_set(&g_stReleasePtrArray[i].atReleaseNodeStatus, CAST_RETRIVE_NODE_READY_TO_RETURN);
            }
        }

        msleep(10);
    }

    return HI_SUCCESS;
}

static HI_S32 Cast_ReturnFrameToList(DISP_CAST_S *pstCast)
{
    HI_U32 i = 0;
    HI_S32 nRet = HI_SUCCESS;

    for (i = 0; i < DISP_CAST_BUFFER_MAX_NUMBER; i++)
    {
        if (CAST_RETRIVE_NODE_READY_TO_RETURN == atomic_read(&g_stReleasePtrArray[i].atReleaseNodeStatus))
        {
            nRet = BP_SetBufReading(&pstCast->stBP, g_stReleasePtrArray[i].u32BufID);
            if (nRet)
                break;

             nRet = BP_AddEmptyBuf(&pstCast->stBP, g_stReleasePtrArray[i].u32BufID);
             if (nRet)
                break;

             nRet = BP_SetBufEmpty(&pstCast->stBP, g_stReleasePtrArray[i].u32BufID);
             if (nRet)
                break;

            if (!g_stReleasePtrArray[i].bInternalRelease)
               pstCast->u32CastReleaseOkCnt ++;

            atomic_set(&g_stReleasePtrArray[i].atReleaseNodeStatus, CAST_RETRIVE_NODE_EMPTY);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 Cast_CreateKthread(DISP_CAST_S *pstCast)
{
    pstCast->kThreadReleaseFrame = kthread_create(Cast_KThread_ReAllocate, pstCast, "HI_DISP_CastRelease");

    if (IS_ERR(pstCast->kThreadReleaseFrame))
    {
        DISP_ERROR("Cast create release thread failed!\n");
        return HI_ERR_DISP_CREATE_ERR;
    }
    else
    {
        wake_up_process(pstCast->kThreadReleaseFrame);
    }

    return HI_SUCCESS;
}

HI_S32 DISP_CastCreate(HI_DRV_DISPLAY_E enDisp,
                       HI_DISP_DISPLAY_INFO_S *pstInfo,
                       HI_DRV_DISP_CAST_CFG_S *pstCfg,
                       DISP_CAST_S** cast_ptr)
{
    HI_DRV_DISP_CALLBACK_TYPE_E eCallType;
    HI_DRV_DISP_CALLBACK_S stCB1, stCB2;
    DISP_CAST_S *pstCast;
    BUF_ALLOC_S stAlloc;
    HI_S32 nRet;

    // check cfg
    if( CastCheckCfg(pstCfg, pstInfo) )
    {
        DISP_ERROR("Cast config invalid!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    // alloc mem
    pstCast = (DISP_CAST_S *)DISP_MALLOC( sizeof(DISP_CAST_S) );
    if (!pstCast)
    {
        DISP_ERROR("Cast malloc failed!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    DISP_MEMSET(pstCast, 0, sizeof(DISP_CAST_S));

    // set attr
    CastSetFrameDemoPartA(pstCast, pstCfg, &pstCast->stAttr);

    // get hal operation
    nRet = DISP_HAL_GetOperation(&pstCast->stIntfOpt);
    if (nRet)
    {
        DISP_ERROR("Cast get hal operation failed!\n");
        goto _ERR_MALLOC;
    }

    // get wbclayer
    nRet = pstCast->stIntfOpt.PF_AcquireWbcByChn(enDisp, &pstCast->eWBC);
    if (nRet)
    {
        DISP_ERROR("Cast get wbc layer failed!\n");
        goto _ERR_MALLOC;
    }

    // create buffer
    stAlloc.bFbAllocMem  = !pstCfg->bUserAlloc;
    stAlloc.eDataFormat  = pstCfg->eFormat;
    stAlloc.u32BufWidth  = pstCfg->u32Width;
    stAlloc.u32BufHeight = pstCfg->u32Height;
    stAlloc.u32BufStride = pstCfg->u32BufStride;
    stAlloc.u32BufSize = pstCfg->u32BufSize;
    nRet = BP_Create(pstCfg->u32BufNumber, &stAlloc, &pstCast->stBP);
    if (nRet)
    {
        DISP_ERROR("Cast alloc buffer failed!\n");
        goto _ERR_MALLOC;
    }

    // register callback
    stCB1.hDst = (HI_VOID *)pstCast;
    stCB1.pfDISP_Callback = DISP_CastCB_GenarateFrame;
	stCB1.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    nRet = DISP_ISR_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);
    if (nRet)
    {
        DISP_ERROR("Cast register work callback failed!\n");
        goto _ERR_BP;
    }

    stCB2.hDst = (HI_VOID*)pstCast;
    stCB2.pfDISP_Callback = DISP_CastPushFrame;
	stCB2.eCallBackPrior  =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    if (pstCfg->bLowDelay)
    {
        eCallType = HI_DRV_DISP_C_DHD0_WBC;
    }
    else
    {
        eCallType = HI_DRV_DISP_C_INTPOS_0_PERCENT;
    }

    nRet = DISP_ISR_RegCallback(enDisp, eCallType, &stCB2);
    if (nRet)
    {
        DISP_ERROR("Cast register work callback failed!\n");
        goto __ERR_REG_CALLBACK1;
    }

    pstCast->u32LastCfgBufId= 0;
    pstCast->u32LastFrameBufId = 0;


    pstCast->bToGetDispInfo = HI_TRUE;
    pstCast->bOpen = HI_TRUE;
    pstCast->bLowDelay = pstCfg->bLowDelay;
    pstCast->eDisp = enDisp;
    pstCast->u32Ref = 1;
    *cast_ptr = pstCast;


    nRet = Cast_CreateKthread(pstCast);
    if (nRet)
        goto __ERR_REG_CALLBACK2;

    memset((void*)g_stReleasePtrArray,
            0,
            sizeof(CAST_RELEASE_PTR_S )* DISP_CAST_BUFFER_MAX_NUMBER);

    DISP_PRINT("DISP_CastCreate ok\n");
    return HI_SUCCESS;

__ERR_REG_CALLBACK2:
    DISP_ISR_UnRegCallback(enDisp, eCallType, &stCB2);
__ERR_REG_CALLBACK1:
    DISP_ISR_UnRegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);
 _ERR_BP:
    BP_Destroy(&pstCast->stBP);
_ERR_MALLOC:
    DISP_FREE(pstCast);
    return nRet;
}

HI_S32 DISP_CastDestroy(HI_VOID *cast_ptr)
{
    HI_DRV_DISP_CALLBACK_TYPE_E eCallType;
    HI_DRV_DISP_CALLBACK_S stCB1,stCB2;
    DISP_CAST_S *pstCast;

    pstCast = (DISP_CAST_S *)cast_ptr;

    if (pstCast->kThreadReleaseFrame)
    {
        kthread_stop(pstCast->kThreadReleaseFrame);
        pstCast->kThreadReleaseFrame = NULL;
    }

    // set disable
    pstCast->bEnable = HI_FALSE;
    /*release  buffer and stop intr func is asynchronus,
      so we should wait. after intr stop, then release the mmz mem,
       to avoid writing after release.*/
    msleep(100);

    stCB1.hDst = (HI_VOID*)cast_ptr;
	stCB1.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;	
    stCB1.pfDISP_Callback = DISP_CastCB_GenarateFrame;
    (HI_VOID)DISP_ISR_UnRegCallback(pstCast->eDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);

    if (pstCast->bLowDelay)
    {
        eCallType = HI_DRV_DISP_C_DHD0_WBC;
    }
    else
    {
        eCallType = HI_DRV_DISP_C_INTPOS_0_PERCENT;
    }

    stCB2.hDst = (HI_VOID *)cast_ptr;
	stCB2.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    stCB2.pfDISP_Callback = DISP_CastPushFrame;
    (HI_VOID)DISP_ISR_UnRegCallback(pstCast->eDisp, eCallType, &stCB2);
    msleep(60);

    // destroy buffer
    BP_Destroy(&pstCast->stBP);

    pstCast->stIntfOpt.PF_ReleaseWbc(pstCast->eWBC);

    // free mem
    DISP_FREE(pstCast);

    return HI_SUCCESS;
}


HI_S32 DISP_CastSetEnable(HI_VOID* cast_ptr, HI_BOOL bEnable)
{
    DISP_CAST_S *pstCast;

    pstCast = (DISP_CAST_S *)cast_ptr;

    pstCast->bEnable = bEnable;

    DISP_PRINT("DISP_CastSetEnable  bEnable = 0x%x\n", (HI_U32)bEnable);

    return HI_SUCCESS;
}

HI_S32 DISP_CastGetEnable(HI_VOID* cast_ptr, HI_BOOL *pbEnable)
{
    DISP_CAST_S *pstCast;

    pstCast = (DISP_CAST_S *)cast_ptr;

    *pbEnable = pstCast->bEnable;

    return HI_SUCCESS;
}

HI_S32	DispCastSetFramePTS(HI_DRV_VIDEO_FRAME_S *pstCastFrame,MIRA_GET_PTS_E pts_flag)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    if(!pts_flag) {
        DISP_OS_GetTime(&pstPrivFrame->u32Pts0);
        pstCastFrame->u32Pts    = pstPrivFrame->u32Pts0;
        pstCastFrame->u32SrcPts = pstPrivFrame->u32Pts0;
    }
    else {
        DISP_OS_GetTime(&pstPrivFrame->u32Pts1);
    }


    return HI_SUCCESS;}

static HI_S32 Cast_AcquireFrame(DISP_CAST_S *pstCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    HI_S32 nRet;
    HI_U32 u32BufId = 0;
    nRet = Cast_GetFrame(pstCast, &u32BufId, 1, pstCastFrame);
    if (nRet)
    {
        DISP_WARN("Cast get id failed!\n");
        return nRet;
    }

    DispCastSetFramePTS(pstCastFrame, MIRA_SET_AQUIRE_PTS);
    return HI_SUCCESS;
}

HI_VOID Cast_SendTimeStamps(DISP_CAST_S *pstCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame, HI_LD_Event_ID_E eEvent)
{
    HI_LD_Event_S evt;
    HI_U32 TmpTime = 0;

    HI_DRV_SYS_GetTimeStampMs(&TmpTime);
    evt.evt_id = eEvent;
    evt.frame = pstCastFrame->u32FrameIndex;
    evt.handle = (HI_ID_DISP << 16) | pstCast->eDisp;
    evt.time = TmpTime;
    HI_DRV_LD_Notify_Event(&evt);

    return;
}

static HI_S32 Cast_PushFrameToBackMod(DISP_CAST_S *pstCast)
{
    HI_S32 nRet = HI_SUCCESS;
    HI_DRV_VIDEO_FRAME_S stCastFrame;
    FN_VENC_PUT_FRAME  venc_queue_pfn  = NULL;
    HI_U32 u32BufId = 0;

    /*if this is null, means get frame though unf api, not attach mode.*/
    if (pstCast->attach_pairs[0].pfnQueueFrm)
    {
        while(1)
        {
            pstCast->u32CastAcquireTryCnt ++;

            Cast_ERROR_DEAL(BP_GetFullBuf(&pstCast->stBP, &u32BufId), 0);
            Cast_ERROR_DEAL(BP_DelFullBuf(&pstCast->stBP, u32BufId), 1);
            Cast_ERROR_DEAL(BP_GetFrame(&pstCast->stBP, u32BufId, &stCastFrame), 1);

            venc_queue_pfn = pstCast->attach_pairs[0].pfnQueueFrm;

            nRet = venc_queue_pfn(pstCast->attach_pairs[0].hSink, &stCastFrame);
            if (nRet == HI_SUCCESS)
            {
                Cast_SendTimeStamps(pstCast, &stCastFrame, EVENT_CAST_FRM_OUT);
                pstCast->u32CastAcquireOkCnt ++;
            }
            else
            {
                DISP_CastReleaseFrame(pstCast, &stCastFrame);
                break;
            }
        }
    }

    return nRet;
}

HI_S32 DISP_CastAcquireFrame(DISP_CAST_S *cast_ptr, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_S *pstCast;
    HI_S32 Ret = 0;

    pstCast = (DISP_CAST_S *)cast_ptr;

    if(pstCast->attach_pairs[0].pfnQueueFrm)
    {
        DISP_ERROR("attach mode is going on ,you can't acquire through unf api.");
        return HI_FAILURE;
    }

    Ret = Cast_AcquireFrame(pstCast, pstCastFrame);

    pstCast->u32CastAcquireTryCnt ++;
    if (HI_SUCCESS == Ret)
    {
        Cast_SendTimeStamps(pstCast, pstCastFrame, EVENT_CAST_FRM_OUT);
        pstCast->u32CastAcquireOkCnt ++;
    }

#if 0
       DISP_ERROR("cast acquire bufid=0x%x, w=%d,h=%d,index=%d, y=0x%x, c=0x%x\n",
       u32BufId,
       pstCastFrame->u32Width,
       pstCastFrame->u32Height,
       pstCastFrame->u32FrmCnt,
       pstCastFrame->stBufAddr[0].u32PhyAddr_Y,
       pstCastFrame->stBufAddr[0].u32PhyAddr_C
       );
#endif

   return Ret;
}


HI_S32 DispCastCheckFrame(DISP_CAST_S *cast_ptr, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    if (pstPrivFrame->cast_ptr == cast_ptr)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_S32 DispCastGetBufId(DISP_CAST_S *cast_ptr,
                        HI_DRV_VIDEO_FRAME_S *pstCastFrame,
                        HI_U32 *pu32BufId)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    if (pstPrivFrame->cast_ptr != cast_ptr)
    {
        DISP_PRINT("PRIV=[%x][%x]\n",
            pstPrivFrame->cast_ptr,
            pstPrivFrame->stPrivInfo.u32BufferID);

        return HI_FAILURE;
    }

    *pu32BufId = pstPrivFrame->stPrivInfo.u32BufferID;

    return HI_SUCCESS;
}


HI_S32 Cast_AddNode2ReallocateList(HI_U32 u32BufId,
                                   HI_BOOL bInternalRelease,
                                   HI_BOOL bNeedReAllocate)
{
    HI_U32 i = 0;

    for (i = 0; i < DISP_CAST_BUFFER_MAX_NUMBER; i++)
    {
        /*search the empty node.*/
        if (!atomic_read(&g_stReleasePtrArray[i].atReleaseNodeStatus))
        {
            g_stReleasePtrArray[i].u32BufID = u32BufId;
            g_stReleasePtrArray[i].bInternalRelease = bInternalRelease;

            if (bNeedReAllocate)
            {
                //need to reallocate.
                atomic_set(&g_stReleasePtrArray[i].atReleaseNodeStatus, CAST_RETRIVE_NODE_REALLOCATE);
            }
            else
            {
                /*set the flag, just put it to nomal empty list, no
                 *necessary to reallocate.
                 */
                atomic_set(&g_stReleasePtrArray[i].atReleaseNodeStatus, CAST_RETRIVE_NODE_READY_TO_RETURN);
            }

            break;
        }
    }
    return HI_SUCCESS;
}

HI_S32 DISP_CastReleaseFrame(DISP_CAST_S *cast_ptr, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_S *pstCast;
    HI_U32 u32BufId;
    HI_S32 nRet = HI_SUCCESS;
    HI_BOOL bNeedReAllocate = HI_FALSE;

    pstCast = (DISP_CAST_S *)cast_ptr;
    pstCast->u32CastReleaseTryCnt ++;

    nRet = DispCastGetBufId(cast_ptr, pstCastFrame, &u32BufId);
    if (nRet)
    {
        DISP_WARN("Cast release frame invalid!\n");
        return nRet;
    }

    if (Cast_CheckOutputSizeChange(pstCast, pstCastFrame))
    {
        bNeedReAllocate = HI_TRUE;
    }
    else
    {
        bNeedReAllocate = HI_FALSE;
    }

    Cast_AddNode2ReallocateList(u32BufId, HI_FALSE, bNeedReAllocate);

    return HI_SUCCESS;
}

HI_S32 DispSetFrameDemoPartB(DISP_CAST_S *cast_ptr,
                             HI_U32 u32Rate,
                             HI_DRV_COLOR_SPACE_E eColorSpace,
                             HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    DISP_CAST_S *pstCast;
    DISP_CAST_PRIV_FRAME_S *pstPriv;

    pstCast = (DISP_CAST_S *)cast_ptr;
    pstPriv = (DISP_CAST_PRIV_FRAME_S *)&(pstFrame->u32Priv[0]);

    //venc and vpss and others,they want a rate * 1000;
    pstFrame->u32FrameRate = u32Rate * 10;
    pstPriv->stPrivInfo.eColorSpace = eColorSpace;

    return HI_SUCCESS;
}

HI_VOID DISP_CastCBSetDispMode(DISP_CAST_S *cast_ptr,
                               const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    DISP_CAST_S *pstCast;
    HI_U32 Rate;

    pstCast = (DISP_CAST_S *)cast_ptr;

    // set display info
    pstCast->stDispInfo = pstInfo->stDispInfo;

    pstCast->stAttr.stIn = pstInfo->stDispInfo.stPixelFmtResolution;
    pstCast->stAttr.bInterlace = pstInfo->stDispInfo.bInterlace;
    pstCast->stAttr.eInColorSpace = pstInfo->stDispInfo.eColorSpace;
    //Todo
    if (pstCast->stAttr.stOut.s32Height >= 720
        && pstCast->stAttr.stOut.s32Width >= 1280)
    {
        pstCast->stAttr.eOutColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
    }
    else
    {
        pstCast->stAttr.eOutColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    }

    Rate = pstInfo->stDispInfo.u32RefreshRate;
    pstCast->stAttr.u32InRate = Rate;

    pstCast->u32Periods = 1;
    while(Rate > DISP_CAST_MAX_FRAME_RATE)
    {
        pstCast->u32Periods = pstCast->u32Periods << 1;
        Rate = Rate >> 1;
    }

    pstCast->stAttr.u32OutRate = Rate;
    DispSetFrameDemoPartB(cast_ptr, Rate, pstCast->stAttr.eOutColorSpace, &pstCast->stAttr.stFrameDemo);

    DISP_PRINT("CAST: iw=%d, ih=%d, ow=%d, oh=%d, or=%d\n ",
                pstCast->stAttr.stIn.s32Width,
                pstCast->stAttr.stIn.s32Height,
                pstCast->stAttr.stOut.s32Width,
                pstCast->stAttr.stOut.s32Height,
                Rate);
    return ;
}


HI_S32 DispCastSetFrameInfo(DISP_CAST_S *pstCast, HI_DRV_VIDEO_FRAME_S *pstCurFrame)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;

    pstCast->stAttr.stFrameDemo.u32FrameIndex = pstCast->u32FrameCnt;
    pstCast->stAttr.stFrameDemo.stBufAddr[0] = pstCurFrame->stBufAddr[0];

    pstFrame = &pstCast->stAttr.stFrameDemo;
    pstFrame->u32Width  = pstCurFrame->u32Width;
    pstFrame->u32Height = pstCurFrame->u32Height;
    pstFrame->stDispRect = pstCurFrame->stDispRect;

    pstFrame->enFieldMode = HI_DRV_FIELD_ALL;
    pstFrame->hTunnelSrc = (HI_ID_DISP << 16) | pstCast->eDisp;

    if (pstCast->bLowDelay)
    {
        pstFrame->u32TunnelPhyAddr = pstCurFrame->u32TunnelPhyAddr;
    }
    else
    {
        pstFrame->u32TunnelPhyAddr = 0;
    }

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCast->stAttr.stFrameDemo.u32Priv[0]);
    pstPrivFrame->stPrivInfo.u32FrmCnt   = pstCast->u32FrameCnt;
    pstPrivFrame->stPrivInfo.u32BufferID = pstCast->u32LastCfgBufId;

    DISP_OS_GetTime(&pstCast->stAttr.stFrameDemo.u32Pts);
    return HI_SUCCESS;
}


HI_S32 DispCastSendTask(DISP_CAST_S *pstCast)
{
    DISP_CAST_ATTR_S *pstAttr;
    HI_RECT_S stOut;

    pstAttr = &pstCast->stAttr;

    stOut.s32Width = pstAttr->stFrameDemo.u32Width;
    stOut.s32Height = pstAttr->stFrameDemo.u32Height;
    stOut.s32X  = 0;
    stOut.s32Y  = 0;

    // config pixformat
    pstCast->stIntfOpt.PF_SetWbcPixFmt(pstCast->eWBC, pstAttr->stFrameDemo.ePixFormat);
    pstCast->stIntfOpt.PF_SetWbcIORect(pstCast->eWBC, &pstCast->stDispInfo, &pstAttr->stIn, &stOut, 0);

    pstCast->stIntfOpt.PF_SetWbc3DInfo(pstCast->eWBC, &pstCast->stDispInfo, &pstAttr->stIn);
    // config csc
    pstCast->stIntfOpt.PF_SetWbcColorSpace(pstCast->eWBC, pstAttr->eInColorSpace, pstAttr->eOutColorSpace);

    // config addr
    pstCast->stIntfOpt.PF_SetWbcAddr(pstCast->eWBC, &(pstAttr->stFrameDemo.stBufAddr[0]));
    pstCast->stIntfOpt.PF_SetWbcLowDlyEnable(pstCast->eWBC, pstCast->bLowDelay);

    if (pstCast->bLowDelay)
    {
        pstCast->stIntfOpt.PF_SetWbcPartfnsLineNum(pstCast->eWBC,
                                    (stOut.s32Height *
                                    DISP_CAST_LOWDLY_THRESHOLD_NUMERATOR) / DISP_CAST_LOWDLY_THRESHOLD_DENOMINATOR);


        pstCast->stIntfOpt.PF_SetWbcLineNumInterval(pstCast->eWBC, DISP_CAST_LOWDLY_LINENUM_INTERVAL);
        pstCast->stIntfOpt.PF_SetWbcLineAddr(pstCast->eWBC, pstAttr->stFrameDemo.u32TunnelPhyAddr);
    }
    // set enable
    pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_TRUE);
    pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);

    return HI_SUCCESS;
}

HI_VOID DISP_CastPushFrame(HI_VOID *cast_ptr, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    DISP_CAST_S *pstCast;
    HI_U32 u32BufId, u32BufState = 0;
    HI_S32 nRet;
    HI_U32 u32FrameWbcStage = 0;

    pstCast = (DISP_CAST_S *)cast_ptr;

    if ( (pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND)
       )
    {
        pstCast->bMasked  = HI_TRUE;
    }
    else
    {
        pstCast->bMasked  = HI_FALSE;
    }

    if(!pstCast->bEnable || pstCast->bMasked)
    {
        return;
    }

    if (atomic_read(&pstCast->bBufBusy))
    {
        return;
    }
    else
    {
        atomic_set(&pstCast->bBufBusy, HI_TRUE);
    }

    {
        pstCast->u32CastIntrCnt++;
        while (1)
        {
            u32BufState = pstCast->bLowDelay ? CAST_BUFFER_STATE_IN_CFGLIST_WRITING:CAST_BUFFER_STATE_IN_CFGLIST_WRITE_FINISH;

            nRet = BP_GetCfgWritingBuf(&pstCast->stBP,
                                       &u32BufId,
                                       u32BufState,
                                       &u32FrameWbcStage);
            if (nRet)
            {
                DISP_WARN("Cast Get cfg  buf failed!\n");
                break;
            }

            (HI_VOID)BP_DelCfgWritingBuf(&pstCast->stBP,u32BufId);
            nRet = BP_AddFullBuf(&pstCast->stBP, u32BufId);
            if(nRet)
            {
                (HI_VOID)BP_SetBufReading(&pstCast->stBP, u32BufId);
                (HI_VOID)BP_AddEmptyBuf(&pstCast->stBP, u32BufId);
                DISP_ERROR("Cast ADD buf failed!\n");
                break;
            }
        }

        Cast_PushFrameToBackMod(pstCast);
    }

    atomic_set(&pstCast->bBufBusy, HI_FALSE);

    return;
}

HI_VOID DISP_CastCB_GenarateFrame(HI_VOID *cast_ptr, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_VIDEO_FRAME_S stCurFrame;
    DISP_CAST_S *pstCast;
    HI_S32 nRet;
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame = NULL;

    pstCast = (DISP_CAST_S *)cast_ptr;

    if ( (pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND)
       )
    {
        pstCast->bMasked  = HI_TRUE;
        pstCast->bToGetDispInfo = HI_TRUE;
    }
    else
    {
        pstCast->bMasked  = HI_FALSE;
    }

    if (pstInfo->eEventType == HI_DRV_DISP_C_OPEN)
    {
        pstCast->bToGetDispInfo = HI_TRUE;
    }

    if (pstCast->bToGetDispInfo)
    {
        DISP_CastCBSetDispMode(cast_ptr, pstInfo);

        pstCast->bToGetDispInfo = HI_FALSE;
    }

	if (pstCast->bScheduleWbc)
    {
        pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_FALSE);
        pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);
		pstCast->bScheduleWbcStatus = HI_TRUE;
		return ;
	}

    // check state
    if(!pstCast->bEnable || pstCast->bMasked)
    {
        pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_FALSE);
        pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);
        return;
    }


    /*when isr and process conflict, try to lock.*/
    if (atomic_read(&pstCast->bBufBusy))
    {
        return;
    }
    else
    {
        atomic_set(&pstCast->bBufBusy, HI_TRUE);
    }

    if (pstInfo->eEventType == HI_DRV_DISP_C_VT_INT)
    {
        HI_U32 u32BufId;

        /*stop wbc first, because in 50hz condition. every 2 times, one wbc occurs.*/
        pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_FALSE);
        pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);

        /*retrieve all the frame release by venc or reallocated frame.*/
        Cast_ReturnFrameToList(pstCast);

        /*all the frame node's stage ++ */
        BP_IncreaseAllCfgWritingState(&pstCast->stBP, HI_NULL);

        /*get the frame writing now, and send time stamp for stastics.*/
        nRet = BP_GetCfgWritingBuf_JustWriting(&pstCast->stBP, &u32BufId, CAST_BUFFER_STATE_IN_CFGLIST_WRITING);
        if ((!nRet) && (!BP_GetFrame(&pstCast->stBP, u32BufId, &stCurFrame)))
        {
                Cast_SendTimeStamps(pstCast, &stCurFrame, EVENT_CAST_FRM_BEGIN);
        }

        /*since we may get 25pfs in 50hz fmt, so a interval is necessary.*/
        pstCast->u32TaskCount++;
        if ((pstCast->u32TaskCount % pstCast->u32Periods) != 0)
        {
            atomic_set(&pstCast->bBufBusy, HI_FALSE);
            return;
        }

        nRet = Cast_GetFrame(pstCast, &u32BufId, 0, &stCurFrame);
        if (nRet)
        {
            DISP_WARN("Cast get empty id failed!\n");
            atomic_set(&pstCast->bBufBusy, HI_FALSE);
            return;
        }

        while(Cast_CheckOutputSizeChange(pstCast, &stCurFrame))
        {
            Cast_AddNode2ReallocateList(u32BufId, HI_TRUE, HI_TRUE);
            nRet = Cast_GetFrame(pstCast, &u32BufId, 0, &stCurFrame);
            if (nRet)
            {
                atomic_set(&pstCast->bBufBusy, HI_FALSE);
                DISP_WARN("Cast get empty id failed!\n");
                return;
            }
        }

        nRet = BP_AddCfgWritingBuf(&pstCast->stBP, u32BufId);
        if (nRet)
        {
            atomic_set(&pstCast->bBufBusy, HI_FALSE);
            DISP_WARN("Cast add to cfg list  failed!\n");
            return;
        }

        pstCast->u32LastCfgBufId = u32BufId;
        pstCast->u32FrameCnt++;

        DispCastSetFrameInfo(pstCast, &stCurFrame);

        DispCastSetFramePTS(&pstCast->stAttr.stFrameDemo, MIRA_SET_CREATE_PTS);

        pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCast->stAttr.stFrameDemo.u32Priv[0]);
        pstPrivFrame->u32Pts0 += 100000 / pstCast->stDispInfo.u32RefreshRate;

        pstCast->stAttr.stFrameDemo.u32Pts    = pstPrivFrame->u32Pts0;
        pstCast->stAttr.stFrameDemo.u32SrcPts = pstPrivFrame->u32Pts0;

        DispCastSendTask(pstCast);

        (HI_VOID) BP_SetFrame(&pstCast->stBP, pstCast->u32LastCfgBufId, &pstCast->stAttr.stFrameDemo);
    }

    atomic_set(&pstCast->bBufBusy, HI_FALSE);
    return;
}

HI_S32 DISP_Cast_AttachSink(DISP_CAST_S *cast_ptr, HI_HANDLE hSink)
{
    HI_MOD_ID_E enModID;
    DISP_CAST_S *pstCast;
    HI_S32 s32Ret;
    VENC_EXPORT_FUNC_S *pstVenFunc = HI_NULL;
    HI_U32  attach_index  = 0, u32Cnt = 0;

    pstCast = (DISP_CAST_S *)cast_ptr;
    enModID = (HI_MOD_ID_E)((hSink & 0xff0000) >> 16);

    while (atomic_read(&pstCast->bBufBusy))
    {
        msleep(10);
        u32Cnt ++;
        if (u32Cnt > 200)
        {
            DISP_ERROR("DISP_Cast_AttachSink timeout.\n");
            return HI_ERR_DISP_TIMEOUT;
        }
    }
    atomic_set(&pstCast->bBufBusy, HI_TRUE);

    if ( HI_ID_VENC == enModID ) {

        s32Ret = HI_DRV_MODULE_GetFunction(enModID,(HI_VOID**)&(pstVenFunc));
        if (HI_SUCCESS != s32Ret) {
            DISP_ERROR("Get null venc ptr when cast.\n");
            atomic_set(&pstCast->bBufBusy, HI_FALSE);
            return HI_ERR_DISP_NULL_PTR;
        }

        for (attach_index = 0; attach_index < DISPLAY_ATTACH_CNT_MAX; attach_index++) {
            if (pstCast->attach_pairs[attach_index].hSink == hSink) {
                pstCast->bAttached = HI_TRUE;
                atomic_set(&pstCast->bBufBusy, HI_FALSE);
                return HI_SUCCESS;
            }
        }

        for (attach_index = 0; attach_index < DISPLAY_ATTACH_CNT_MAX; attach_index++) {
            if (pstCast->attach_pairs[attach_index].hSink == 0) {
                pstCast->attach_pairs[attach_index].hSink = hSink;
                pstCast->attach_pairs[attach_index].pfnQueueFrm = pstVenFunc->pfnVencQueueFrame;
                pstCast->attach_pairs[attach_index].pfnDequeueFrame = HI_NULL;
                break;
            }
        }

        if (attach_index == DISPLAY_ATTACH_CNT_MAX) {
            s32Ret = HI_FAILURE;
        }
        else{
            s32Ret = HI_SUCCESS;
            pstCast->bAttached = HI_TRUE;
        }
    } else {
       s32Ret = HI_FAILURE;
    }

    /*we support dynamic attach and dettach without disabling cast(as a producer),
      when attach again,  we should reset all the buffer, or else
      there is no empty buffer node, because cast enabled and full buf full.*/
    if (s32Ret == HI_SUCCESS)
    {
		/*msleep 60 ms, for when reset we should wait until wbc stop to write back.*/
		msleep(60);

        BP_Reset(&pstCast->stBP);
        /*because in the first, venc and cast output resolutin
         *may not consistent.
         */
        (HI_VOID)BP_ReAllocAllBuf(&pstCast->stBP);
        memset((void*)g_stReleasePtrArray,
        0,
        sizeof(CAST_RELEASE_PTR_S) * DISP_CAST_BUFFER_MAX_NUMBER);
    }

    atomic_set(&pstCast->bBufBusy, HI_FALSE);
    return s32Ret;
}

HI_S32 DISP_Cast_DeAttachSink(DISP_CAST_S *cast_ptr, HI_HANDLE hSink)
{
    HI_MOD_ID_E enModID;
    DISP_CAST_S *pstCast;
    HI_S32 s32Ret;
    HI_U32  attach_index  = 0, u32Cnt = 0;

    pstCast = (DISP_CAST_S *)cast_ptr;
    enModID = (HI_MOD_ID_E)((hSink & 0xff0000) >> 16);

    while (atomic_read(&pstCast->bBufBusy))
    {
        msleep(10);
        u32Cnt ++;
        if (u32Cnt > 200)
        {
            DISP_ERROR("DISP_Cast_AttachSink timeout.\n");
            return HI_ERR_DISP_TIMEOUT;
        }
    }
    atomic_set(&pstCast->bBufBusy, HI_TRUE);

    if ( HI_ID_VENC == enModID ) {

        for (attach_index = 0; attach_index < DISPLAY_ATTACH_CNT_MAX; attach_index++) {
            if (pstCast->attach_pairs[attach_index].hSink == hSink) {
                pstCast->attach_pairs[attach_index].hSink = 0;
                pstCast->attach_pairs[attach_index].pfnQueueFrm = HI_NULL;
                pstCast->attach_pairs[attach_index].pfnDequeueFrame = HI_NULL;
                 break;
            }
        }

        if (attach_index == DISPLAY_ATTACH_CNT_MAX)
        {
            s32Ret = HI_FAILURE;
        }
        else
        {
            s32Ret = HI_SUCCESS;
            pstCast->bAttached = HI_FALSE;
        }

    } else {
       s32Ret = HI_FAILURE;
    }

    /*as a result of asynchrounous bettween Cast and venc,
     *we should wait ,because if venc detach and return, cast may keep
     * writing  to venc.
     */
    msleep(40);
    atomic_set(&pstCast->bBufBusy, HI_FALSE);

    return s32Ret;
}

static HI_VOID Cast_UpdateAttr(DISP_CAST_S *pstCast)
{
    pstCast->stBP.stAlloc.u32BufWidth = pstCast->stAttr.stOut.s32Width;
    pstCast->stBP.stAlloc.u32BufHeight = pstCast->stAttr.stOut.s32Height;

    return;
}

HI_S32 DISP_Cast_SetAttr(DISP_CAST_S *cast_ptr, HI_DRV_DISP_Cast_Attr_S *castAttr)
{
    DISP_CAST_S *pstCast = HI_NULL;

    pstCast = (DISP_CAST_S *)cast_ptr;

    pstCast->stAttr.stOut.s32Width  = castAttr->s32Width;
    pstCast->stAttr.stOut.s32Height = castAttr->s32Height;

    Cast_UpdateAttr(pstCast);

    return HI_SUCCESS;
}

HI_S32 DISP_Cast_GetAttr(DISP_CAST_S *cast_ptr, HI_DRV_DISP_Cast_Attr_S *castAttr)
{
    DISP_CAST_S *pstCast = HI_NULL;

    pstCast = (DISP_CAST_S *)cast_ptr;

    castAttr->s32Width = pstCast->stAttr.stOut.s32Width;
    castAttr->s32Height = pstCast->stAttr.stOut.s32Height;

    return HI_SUCCESS;
}

#define SNAPSHOT_MAGIC (0x534e4150uL) /* ASCII code of "SNAP" */

static HI_U32 s_u32FrameCnt = 1;

HI_S32 DISP_Acquire_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_VOID **snapshotHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    BUF_ALLOC_S stAlloc;
    HI_S32 ret;
    HI_DRV_VIDEO_FRAME_S* pstVideoFrame = pstFrame;
    DISP_INTF_OPERATION_S stFunc;
    DISP_WBC_E eWBC;
    HI_DISP_DISPLAY_INFO_S stInfo;
    HI_U32 u32BufId = 0;
    DISP_SNAPSHOT_PRIV_FRAME_S *pstPriv;
    HI_U32 u32Pts;
    DISP_SNAPSHOT_S *pstSnapshot = HI_NULL;
    HI_BOOL  bBufAlloc = 0;

    *snapshotHandle  = 0;

    pstSnapshot = (DISP_SNAPSHOT_S*)DISP_MALLOC(sizeof(DISP_SNAPSHOT_S));
    if (!pstSnapshot)
        return HI_FAILURE;

    DISP_MEMSET(pstSnapshot, 0, sizeof(DISP_SNAPSHOT_S));
    ret = DISP_HAL_GetOperation(&stFunc);
    if (ret) {
        goto __ERR_EXIT__;
    }

    ret = stFunc.PF_AcquireWbcByChn(enDisp, &eWBC);
    if (ret) {
        if (eWBC >= DISP_WBC_BUTT)
            goto __ERR_EXIT__;
    }

    (HI_VOID)DISP_GetDisplayInfo( enDisp, &stInfo);
    stAlloc.bFbAllocMem = HI_TRUE;
    stAlloc.eDataFormat = HI_DRV_PIX_FMT_NV21;
    stAlloc.u32BufHeight = stInfo.stFmtResolution.s32Height;
    stAlloc.u32BufWidth = stInfo.stFmtResolution.s32Width;
    stAlloc.u32BufStride = 0;
    ret = BP_Create(1, &stAlloc, &pstSnapshot->stBP);
    if (ret)
    {
        goto __ERR_EXIT__;
    }

    bBufAlloc = 1;
    ret = BP_GetEmptyBuf(&pstSnapshot->stBP, &u32BufId);
    if (ret)
    {
        goto __ERR_EXIT__;
    }

    ret = BP_DelEmptyBuf(&pstSnapshot->stBP, u32BufId);
    if (ret)
    {
        goto __ERR_EXIT__;
    }

    ret = BP_GetFrame(&pstSnapshot->stBP, u32BufId, pstVideoFrame);
    if (ret)
    {
        goto __ERR_EXIT__;
    }

    pstVideoFrame->u32Width  = stInfo.stFmtResolution.s32Width;
    pstVideoFrame->u32Height = stInfo.stFmtResolution.s32Height;
    pstVideoFrame->u32AspectWidth = 16;
    pstVideoFrame->u32AspectHeight = 9;
    pstVideoFrame->u32FrameRate = 0;
    pstVideoFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
    pstVideoFrame->bProgressive = HI_TRUE;
    pstVideoFrame->enFieldMode = HI_DRV_FIELD_ALL;
    pstVideoFrame->bTopFieldFirst = 0;
    pstVideoFrame->stDispRect = stInfo.stFmtResolution;
    pstVideoFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
    pstVideoFrame->u32FrameIndex = 0;
    memset(pstVideoFrame->u32Priv, 0, sizeof(pstVideoFrame->u32Priv));
    // config pixformat
    stFunc.PF_SetWbcPixFmt(eWBC, HI_DRV_PIX_FMT_NV21);

    /*FIXME:  ourrect  why be stInfo.stFmtResolution? error?*/
    stFunc.PF_SetWbcIORect(eWBC, &stInfo, &stInfo.stPixelFmtResolution, &stInfo.stFmtResolution, 0);

    stFunc.PF_SetWbc3DInfo(eWBC, &stInfo, &stInfo.stPixelFmtResolution);

    // config csc
    stFunc.PF_SetWbcColorSpace(eWBC, stInfo.eColorSpace, stInfo.eColorSpace);

    // config addr
    stFunc.PF_SetWbcAddr(eWBC, &(pstVideoFrame->stBufAddr[0]));

    // set enable FIXME!!!
    stFunc.PF_SetWbcEnable(eWBC, HI_TRUE);
    stFunc.PF_UpdateWbc(eWBC);

    /*we should wait util wbc finished ,else acquire api may get useless data.*/
    if (!stInfo.u32RefreshRate)
    {
        msleep(60);
    }
    else
    {
        msleep((100000/stInfo.u32RefreshRate + 1) * 2);
    }

    /* set pts. */
    DISP_OS_GetTime(&u32Pts);
    pstFrame->u32Pts    = u32Pts;
    pstFrame->u32SrcPts = u32Pts;

    /* construct some private info. */
    pstPriv = (DISP_SNAPSHOT_PRIV_FRAME_S*)&(pstFrame->u32Priv[0]);
    pstPriv->stPrivInfo.u32FrmCnt = s_u32FrameCnt ++;
    pstPriv->u32BPAddr = &(pstSnapshot->stBP);
    pstPriv->u32Magic = SNAPSHOT_MAGIC;

    pstSnapshot->bWork = 1;

    *snapshotHandle  = (HI_VOID*)pstSnapshot;
    return HI_SUCCESS;

__ERR_EXIT__:
    if (bBufAlloc)
        BP_Destroy(&pstSnapshot->stBP);

    DISP_FREE(pstSnapshot);
    return HI_FAILURE;
}

HI_S32 DISP_Release_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_VOID* snapshotHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    DISP_SNAPSHOT_PRIV_FRAME_S *pstPriv;
    DISP_SNAPSHOT_S *pstSnapshot = HI_NULL;

    pstPriv = (DISP_SNAPSHOT_PRIV_FRAME_S *)&(pstFrame->u32Priv[0]);
    if (pstPriv->u32Magic != SNAPSHOT_MAGIC)
        return HI_ERR_DISP_INVALID_PARA;

    if (!snapshotHandle)
        return HI_ERR_DISP_NULL_PTR;

    pstSnapshot = (DISP_SNAPSHOT_S *)snapshotHandle;

     if (pstSnapshot->bWork == 0)
        return HI_SUCCESS;

    pstSnapshot->bWork = 0;
    BP_Destroy(&pstSnapshot->stBP);
    DISP_FREE(pstSnapshot);
    return 0;
}


HI_S32 DISP_SnapshotDestroy(HI_VOID* snapshot_ptr)
{
    DISP_SNAPSHOT_S *pstSnapshot = HI_NULL;



    pstSnapshot = (DISP_SNAPSHOT_S *)snapshot_ptr;
    if (!pstSnapshot)
        return HI_ERR_DISP_NULL_PTR;

    if (pstSnapshot->bWork == 0)
        return HI_SUCCESS;

    /*when we destroy, wbc may be going on,so we wait.*/
    msleep(40);

    pstSnapshot->bWork = 0;
    BP_Destroy(&pstSnapshot->stBP);
    DISP_FREE(pstSnapshot);

    return HI_SUCCESS;
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
