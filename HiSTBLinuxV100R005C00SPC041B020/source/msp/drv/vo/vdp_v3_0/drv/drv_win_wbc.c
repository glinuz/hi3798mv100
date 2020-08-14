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
#include "drv_display.h"
#include "drv_win_priv.h"
#include "drv_window.h"
#include "drv_win_policy.h"
#include "hi_drv_sys.h"
#include "hi_drv_stat.h"
#include "drv_vdec_ext.h"
#include "drv_disp_hal.h"
#include "drv_disp_alg_service.h"
#include "hi_drv_module.h"
#include "drv_pq_ext.h"
#include "drv_win_wbc.h"
#include <linux/hrtimer.h>


HI_BOOL g_bWbcTimerRunFlag =  HI_FALSE;
HI_VOID * pWbc_hrtimer = NULL;

#ifdef WBC_FRAME_CREATE_FRAMEMODE
HI_BOOL bAllocateFrameSize  = HI_TRUE;
#else
HI_BOOL bAllocateFrameSize  = HI_FALSE;
#endif

HI_U8 *u32Partfinish_UsedArray[DISP_BUF_NODE_MAX_NUMBER];
HI_U32 u32UsedArray_ReadPos = 0;
HI_U32 u32UsedArray_WritePos = 0;

HI_HANDLE g_hSlaveWin = 0;
HI_U32 u32WbcFrameCnt = 0;
HI_DRV_WIN_WBC_Linker_S  stWbcConfigInfor;
HI_U32 u32WbcPartFinishCnt = 0;
HI_DISP_WBC_S stWbcInfor;
DISP_MMZ_BUF_S stMBuf_WBC;
Linker_INFOR_S g_stLinker;

/*default  non-lowdelay mode.*/
HI_BOOL bLowdelayMode = HI_TRUE;
HI_BOOL bLowdelayModeChange = HI_TRUE;

HI_U32 jiffies_now_wbc = 0;
HI_U32 jiffies_old_wbc = 0;

HI_DRV_VIDEO_FRAME_S *pstFrame_wbc[WBC_BUF_NUM] = {HI_NULL};
WIN_WBCISOGENY_PROC_INFOR_S  stWbcIsogenyProcInfor;


HI_S32 WinSd_Dequeue_DisplayedFrame(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrm);


HI_VOID * HrTimer_Init(HI_VOID *pTimerProc)
{
    struct hrtimer *pstWbcTimer = NULL;
	
    pstWbcTimer = (struct hrtimer *)HI_KMALLOC(HI_ID_KEYLED, sizeof(struct hrtimer),GFP_ATOMIC);
    if (NULL != pstWbcTimer)
    {
        hrtimer_init(pstWbcTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        pstWbcTimer->function = pTimerProc;
        WIN_INFO("hrtimer_init OK! \n");
        return (void *)pstWbcTimer;
    }
	
    return NULL;
}

HI_S32 HrTimer_Start(HI_VOID *handle)
{
    if(handle == NULL)
    {
        WIN_ERROR("HI_HIGH_RES_TimerStart handle invalid \n");
        return HI_FAILURE;
    }
	
    hrtimer_start((struct hrtimer *)handle,ktime_set(0,WBC_TIMER_INTERVAL_MS*1000000),HRTIMER_MODE_REL);

    return HI_SUCCESS;
}

HI_S32 HrTimer_Update(HI_VOID *handle)
{
    if(handle == NULL)
    {
        WIN_ERROR("HI_HIGH_RES_TimerUpdate handle invalid \n");
        return HI_FAILURE;
    }
	
    hrtimer_forward((struct hrtimer *)handle, ktime_get(), ktime_set(0,WBC_TIMER_INTERVAL_MS*1000000));
    return HI_SUCCESS;
}

HI_S32 HrTimer_Destroy(HI_VOID *handle)
{
    if(handle == NULL)
    {
        WIN_ERROR("HI_HIGH_RES_TimerDestroy handle invalid \n");
        return HI_FAILURE;
    }
	
    hrtimer_cancel((struct hrtimer *)handle);	
    HI_KFREE(HI_ID_KEYLED, handle);
    return HI_SUCCESS;
}

HI_VOID Wbc_PartFinish_TimerProc(HI_VOID)
{
    if (HI_SUCCESS != HrTimer_Update(pWbc_hrtimer))
    {
        WIN_ERROR("Err in HI_HIGH_RES_TimerUpdate!\n");
        return;
    }
	
	//WbcLinker_Proc();
	
    if (HI_SUCCESS != HrTimer_Start(pWbc_hrtimer))
    {
        WIN_ERROR("Err in HI_HIGH_RES_TimerStart!\n");
        return;
    }
	
    return;
}

HI_S32 Wbc_PartFinish_InitTimer(HI_VOID)
{
    if (NULL != pWbc_hrtimer)
    {
        WIN_ERROR("LED_InitTimer pWbc_hrtimer is not NULL\n");
        return HI_FAILURE;
    }

    pWbc_hrtimer = HrTimer_Init(Wbc_PartFinish_TimerProc);

    if (HI_SUCCESS != HrTimer_Start(pWbc_hrtimer))
    {
        WIN_ERROR("HI_HIGH_RES_TimerStart start failure \n");
        return HI_FAILURE;
    }
	
    g_bWbcTimerRunFlag = HI_TRUE;
	
    return HI_SUCCESS;
}

HI_VOID Wbc_PartFinish_DeleteTimer(HI_VOID)
{
    HrTimer_Destroy(pWbc_hrtimer);
	
    pWbc_hrtimer = NULL;
    g_bWbcTimerRunFlag = HI_FALSE;

    WIN_INFO("DeleteTimer g_bWbcTimerRunFlag =  %d\n", g_bWbcTimerRunFlag);
    return;
}

HI_BOOL Wbc_PartFinish_GetTimerStatus(HI_VOID)
{
	return g_bWbcTimerRunFlag;
}

/*when dobly or av-sync  test, set the lowdelay mode,
  * In this mode,  t/b filed-match is not done!*/
HI_VOID WinSd_Chn_Set_LowdelayMode(HI_BOOL bLowdelayModeTmp)
{
	if (bLowdelayModeTmp != bLowdelayMode)
		bLowdelayModeChange = HI_TRUE;
	else
		bLowdelayModeChange = HI_FALSE;
	
	bLowdelayMode = bLowdelayModeTmp;
}

HI_BOOL WinSd_Chn_Get_LowdelayMode(HI_VOID)
{
     return bLowdelayMode;
}

HI_BOOL WinSd_Chn_Get_LowdelayChangeFlag(HI_VOID)
{
     return bLowdelayModeChange;
}

HI_VOID WinSd_Chn_Clear_LowdelayModeChange(HI_VOID)
{
     bLowdelayModeChange  = HI_FALSE;
}

HI_BOOL Win_checkFrameAddrValid(HI_U32 u32Yaddr,
										   HI_U32 u32YStride,
										   HI_U32 u32Caddr,							   
										   HI_U32 u32CStride,
										   HI_U32 u32Height)
{
	HI_U32 u32StartAddr = 0, u32EndAddr = 0;
	
	u32StartAddr = stMBuf_WBC.u32StartPhyAddr;
	u32EndAddr = (stMBuf_WBC.u32StartPhyAddr + stMBuf_WBC.u32Size);

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

HI_S32 Win_AllocateFrameBuf(HI_BOOL bAllocateFrameSize)
{
	HI_U32 u32AllocateSize = 0;
	WIN_BUF_ALLOC_PARA_S stAlloc;

	/*create buffers for wbc to write to.*/
	stAlloc.bFbAllocMem	 = HI_TRUE;
	stAlloc.u32BufWidth  = WBC_RECT_WIDTH;
	stAlloc.u32BufHeight = WBC_RECT_HEIGHT;
	stAlloc.u32BufStride = WBC_MAX_STRIDE;
	stAlloc.eDataFormat  = HI_DRV_PIX_FMT_NV61_2X1;

	u32AllocateSize = stAlloc.u32BufStride * stAlloc.u32BufHeight * WBC_BUF_NUM * WBC_PERPIXEL_BYTES;
	u32AllocateSize = bAllocateFrameSize?u32AllocateSize:u32AllocateSize/2;
	
	if (DISP_OS_MMZ_AllocAndMap("VDP_WBC_V3",
					  HI_NULL,
					  u32AllocateSize,
					  16,
					  &stMBuf_WBC))
	{
		return HI_ERR_VO_MALLOC_FAILED;
	}
	
	return HI_SUCCESS;
}

HI_S32 WbcInit_Insert_Empty_Node(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 nRet;
    DISP_BUF_NODE_S *pstNode;
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = 0;
	HI_DRV_VIDEO_FRAME_S *frame_tmp = 0;
	WBC_FRAME_PRIVATE_S  *pstWbcFramePrivate = HI_NULL;

    WIN_CHECK_NULL_RETURN(pstWinBP);
    WIN_CHECK_NULL_RETURN(pstFrame);
	
    nRet = DispBuf_GetEmptyNode(&pstWinBP->stBuffer, &pstNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

     /*set a barrier bettween DispBuf_GetEmptyNode and DispBuf_DelEmptyNode,
     * del oper will change the state of node in empty list, but get oper
     * will check it. so a barrier is neccessary. see DTS2014080502661.
     */
    barrier();

    nRet = DispBuf_DelEmptyNode(&pstWinBP->stBuffer, pstNode);
    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("DispBuf_DelEmptyNode failed, ID=0x%x\n", pstNode->u32ID);
        return HI_FAILURE;
    }

	
    memcpy(pstNode->u32Data, pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
	
	pstNode->u32State = DISP_BUFN_STATE_READING;

	frame_tmp = (HI_DRV_VIDEO_FRAME_S*)pstNode->u32Data;
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&frame_tmp->u32Priv[0];

	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
	
	pstWbcFramePrivate->pstFrameAddr = (HI_VOID*)frame_tmp;
    nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstNode);
    DISP_ASSERT(nRet == HI_SUCCESS);
    return HI_SUCCESS;
}


/* 
  *there are two lists:    wbc's and sd window's.  
  * wbc's buffer list is initialized here, sd window's is done in window create func.
  * Pay attention: 
  * 1)  frame's infor: bProgressive/u32PhyAddr_Y/u32PhyAddr_C  is just initialized here.
  *      it will be changed according the fmt, wbc point etc. 
  */
HI_S32 WinWbc_CreateBuffer(HI_VOID)
{
	HI_U32 i = 0;	
	memset((HI_VOID*)&stWbcIsogenyProcInfor, 0, sizeof(WIN_WBCISOGENY_PROC_INFOR_S));
	
	Win_AllocateFrameBuf(HI_TRUE);
 
	/*2.memset wbcinfor structure.*/
	memset((HI_VOID*)&stWbcConfigInfor, 0, sizeof(HI_DRV_WIN_WBC_Linker_S));
	
	/*3. create the wbc's  buffer list.*/
	(HI_VOID)WinBuf_Create(WBC_BUF_NUM,
						   WIN_BUF_MEM_SRC_SUPPLY,
						   HI_NULL,
						   &stWbcConfigInfor.stWinBP);
	
	/*4.this func is useless for wbc.*/
	//WinBufferReset(&stWbcConfigInfor.stWinBP);
	
	/*assemble all the mems to  frame array.*/
	for (i = 0;i < WBC_BUF_NUM; i++)
	{
		pstFrame_wbc[i] = (HI_DRV_VIDEO_FRAME_S *)DISP_MALLOC(sizeof(HI_DRV_VIDEO_FRAME_S));
		if (!pstFrame_wbc[i])
		{	
			return HI_ERR_VO_MALLOC_FAILED;
		}
		
		memset((HI_VOID*)pstFrame_wbc[i], 0, sizeof(HI_DRV_VIDEO_FRAME_S));
		
		pstFrame_wbc[i]->bProgressive = HI_TRUE;
		pstFrame_wbc[i]->ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
		
		pstFrame_wbc[i]->stBufAddr[0].u32PhyAddr_Y =  stMBuf_WBC.u32StartPhyAddr 
								+ i * WBC_MAX_STRIDE * WBC_RECT_HEIGHT * 2;		
		pstFrame_wbc[i]->stBufAddr[0].u32Stride_Y  =  WBC_MAX_STRIDE;
		pstFrame_wbc[i]->stBufAddr[0].u32PhyAddr_C =  pstFrame_wbc[i]->stBufAddr[0].u32PhyAddr_Y
												  + WBC_MAX_STRIDE * WBC_RECT_HEIGHT;
		
		pstFrame_wbc[i]->stBufAddr[0].u32Stride_C =  WBC_MAX_STRIDE;
		
		/*insert the frame to  wbc's fullist, notice, i think this is should be full state. */
		WbcInit_Insert_Empty_Node(&stWbcConfigInfor.stWinBP,pstFrame_wbc[i]); 
	}	
	
	stWbcConfigInfor.eInitalizeFlag = WBC_INITFLAG;
	stWbcConfigInfor.u32StartTime = jiffies;

	stWbcConfigInfor.eWbcFieldMode_In = HI_DRV_FIELD_BUTT;
	stWbcConfigInfor.eWbcFieldMode_Out = HI_DRV_FIELD_BUTT;
	
	
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcBufferStatus.u32WbcTotalNodeCounts  = WBC_BUF_NUM;		
	
	/*init the spinlock.*/
	spin_lock_init(&stWbcConfigInfor.stWinBP.spinlock_BufPool);	
	
	return HI_SUCCESS;
}

/*reset the winbp 's  pointer member.*/
HI_VOID WinWbc_ResetBuffer(HI_VOID)
{
	(HI_VOID)WinBuf_Reset(&stWbcConfigInfor.stWinBP);
	return ;
}

/*destroy buffer when close the window.*/
HI_VOID WinWbc_DestroyBuffer(HI_VOID)
{
	HI_U32 i = 0;

	stWbcConfigInfor.eInitalizeFlag = WBC_NOT_INIT_FLAG;
	/*release the real frame mem.*/
	DISP_OS_MMZ_UnmapAndRelease(&stMBuf_WBC);

	/*release the  frame struct node.*/
	for (i = 0; i < WBC_BUF_NUM; i++)
	{	
		DISP_FREE(pstFrame_wbc[i]);
	}
	
	/*release the  wbc's buffer node.*/
	WinBuf_Destroy(&stWbcConfigInfor.stWinBP);
	memset((HI_VOID*)&stWbcConfigInfor, 0, sizeof(HI_DRV_WIN_WBC_Linker_S));
}

struct task_struct **WinWbc_GetKthreadPtr(HI_VOID)
{
	return &stWbcConfigInfor.kThreadWbcPartFinish;	
}


HI_BOOL Win_GetWbcInitFlag(HI_VOID)
{
	return (stWbcConfigInfor.eInitalizeFlag == WBC_INITFLAG);
}

/*according the  lowdelay */
HI_VOID Win_SetLowdelayMode_AndSwitch(HI_U32 u32HdFreshRate,
									  HI_U32 u32SdFreshRate)
{	
	HI_BOOL bLowdelayChange = HI_FALSE;
	
	/*means not-lowdelay Circumstance.*/
	if (u32HdFreshRate == u32SdFreshRate)
	{		
		/*update  the lowdelay mode, according the refresh rate.*/
		WinSd_Chn_Set_LowdelayMode(HI_TRUE);		
	}
	else
	{
		/*update  the lowdelay mode, according the refresh rate.*/
		WinSd_Chn_Set_LowdelayMode(HI_TRUE);		
	}

	bLowdelayChange  = WinSd_Chn_Get_LowdelayChangeFlag();
	if (bLowdelayChange)
	{
		/*if  lowdelay mode changed ,clear the flag. and do some switch work.*/
		WinSd_Chn_Clear_LowdelayModeChange();
		/*todo: do some work change.*/
	}
	
	return;
}

HI_VOID Win_SetSingleBufMode_WbcEffectiveFlag(HI_BOOL bWbcTakeEffect)
{
	stWbcConfigInfor.bWbcTakeEffect = bWbcTakeEffect;
}

HI_BOOL Win_GetSingleBufMode_WbcEffectiveFlag(HI_VOID)
{
	return  stWbcConfigInfor.bWbcTakeEffect;
}

/*check whether the frame can be pushed to sd's window.*/
HI_BOOL Win_CheckWbcPartFinish(HI_DRV_VIDEO_FRAME_S *pstFrameInfor)
{
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = HI_NULL;
	WBC_FRAME_PRIVATE_S  *pstWbcFramePrivate = HI_NULL;
	
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&pstFrameInfor->u32Priv[0];
	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
	
	if (pstWbcFramePrivate->ullExpectWbcFinishTimeMs <= get_jiffies_64())
	{
		pstWbcFramePrivate->ullLinkerProcessTimeMs = get_jiffies_64();
		return HI_TRUE;
	}
	else
	{
		return HI_FALSE;
	}	
}


/*for wbc producer.*/
HI_VOID WinLinker_GetPreviousWbcField(HI_DRV_WIN_WBC_Linker_S  *pstWbcConfigInfor, HI_DRV_FIELD_MODE_E *peFieldNode)
{	
	*peFieldNode = pstWbcConfigInfor->eWbcFieldMode_Out;
	return ;	
}

/*for wbc producer.*/
HI_S32 WinLinker_GetNewNodeToWrite(HI_BOOL bLowDelayMode)
{
	DISP_BUF_NODE_S *pstNode = HI_NULL;
	HI_U32 s32Ret = 0;
	
    /*when lowdelay mode, we fix use one buffer: the first one.*/
	if (bLowDelayMode)
	{			
		DispBuf_GetEmptyNode(&stWbcConfigInfor.stWinBP.stBuffer, &pstNode);
		if (!pstNode)
		{
			WIN_ERROR("Get First wbc node faile!\n");
			return HI_FAILURE;
		}

        stWbcConfigInfor.stWinBP.pstConfig =  pstNode;		
		if (!stWbcConfigInfor.stWinBP.pstConfig)
		{	
			WIN_WARN("Get First wbc node failed!\n");
			return HI_FAILURE;
		}
		else
		{
			return HI_SUCCESS;
		}
	}
	else
	{
		/*for wbc producer,  insert a  frame into it.*/
		if (0 != (s32Ret = DispBuf_GetEmptyNode(&stWbcConfigInfor.stWinBP.stBuffer,
												&stWbcConfigInfor.stWinBP.pstConfig)))
		{		
			goto __exit1111;
		}
	
		if (0 != (s32Ret = DispBuf_DelEmptyNode(&stWbcConfigInfor.stWinBP.stBuffer,
												  stWbcConfigInfor.stWinBP.pstConfig)))
		{
			goto __exit1111;
		}	
		s32Ret = HI_SUCCESS;	
		return s32Ret;
	
		__exit1111:	
			return s32Ret;
	}
	
}

HI_VOID Win_GetWbcinputSize(VDP_LAYER_CONN_E eWbcConnection,
						   HI_RECT_S *pstInputFrameSize,
						   HI_RECT_S *pstHdFmtRect,
						   HI_RECT_S *pstSdWbcInputRect)
{
	/*when wbc is located in vp after csc. the wbc's input
	 will be the whole size described by format size.
	 */
	if (eWbcConnection == VDP_LAYER_CONN_VPACSC)
	{
		*pstSdWbcInputRect = *pstHdFmtRect;					 
	}
	else if (eWbcConnection == VDP_LAYER_CONN_VIDBZME)
	{
		*pstSdWbcInputRect = *pstInputFrameSize;
	}	
}

HI_VOID  WinHd_Decide_WbcInputOutput_FieldMode(WINDOW_S *pstWin,
											   WINDOW_S *pstSlvWin,
											   HI_BOOL  bWbcInputFrameMode,
											   const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo,
											   HI_DRV_FIELD_MODE_E *peWbcInFieldMode_Out,
											   HI_DRV_FIELD_MODE_E *peWbcInFieldMode_In)
{
	DISP_INTF_OPERATION_S stFunction;
	HI_BOOL bBtm;
	HI_U32  u32Vcnt = 0;	

	if (bWbcInputFrameMode == HI_FALSE)
	{
		/*when wbc's input is field mode, the wbc's out is completely decided by
		  * hd channel and should be equal to it .  not by sd's anything.
		  */
		(HI_VOID)DISP_HAL_GetOperation(&stFunction);
		stFunction.FP_GetChnBottomFlag(pstWin->enDisp, &bBtm, &u32Vcnt);
		*peWbcInFieldMode_In = 1 - bBtm;
	}
	else
	{
		*peWbcInFieldMode_In = HI_DRV_FIELD_ALL;
	}

	/*if  is lowdelay mode,  the field mode is decided by sd-channel.*/
	if (WinSd_Chn_Get_LowdelayMode())
	{		
		(HI_VOID)DISP_HAL_GetOperation(&stFunction);
		stFunction.FP_GetChnBottomFlag(0, &bBtm, &u32Vcnt);
		*peWbcInFieldMode_Out  = 1 - bBtm;
	}
	else
	{
		/*not lowdelay mode,  hd and sd is not coupled, sd is adjusted by hd.*/
		/*TODO:   relealized.*/		
	}
	
    return;
}


HI_U32 u32DhdM_Intr_Line = 0;

HI_VOID  WinHd_GenarateFrameInfor(HI_DRV_VIDEO_FRAME_S **pstConfigFrame,
											  HI_RECT_S *pstSdDispRect,
											  HI_DRV_FIELD_MODE_E eWbcInFieldMode_Out,
											  HI_U32 u32Fidelity,
											  HI_BOOL bLowDelayMode)
{
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = HI_NULL;
	WBC_FRAME_PRIVATE_S  *pstWbcFramePrivate = HI_NULL;	
	
	/*here pay attention, the frame is for sd output.
	 * not for wbc but as a result of wbc.
	 */
	*pstConfigFrame = (HI_DRV_VIDEO_FRAME_S *)&stWbcConfigInfor.stWinBP.pstConfig->u32Data[0];
	(*pstConfigFrame)->bCompressd = HI_FALSE;
	(*pstConfigFrame)->u32FrameIndex = u32WbcFrameCnt ++;
	
	/*must pay attention for debug.*/
	(*pstConfigFrame)->enFieldMode = eWbcInFieldMode_Out;
	(*pstConfigFrame)->enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
	(*pstConfigFrame)->stDispRect.s32Width = pstSdDispRect->s32Width;
	(*pstConfigFrame)->stDispRect.s32Height = pstSdDispRect->s32Height;
	(*pstConfigFrame)->stDispRect.s32X = 0;
	(*pstConfigFrame)->stDispRect.s32Y = 0;
	
	/*set the w/h of frame.*/
	(*pstConfigFrame)->u32Width = pstSdDispRect->s32Width;
	(*pstConfigFrame)->u32Height = pstSdDispRect->s32Height;
	
	(*pstConfigFrame)->stBufAddr[0].u32Stride_Y = 720;
	(*pstConfigFrame)->stBufAddr[0].u32Stride_C = 720;
	
	(*pstConfigFrame)->stBufAddr[0].u32PhyAddr_C = (*pstConfigFrame)->stBufAddr[0].u32PhyAddr_Y
												+ (*pstConfigFrame)->stBufAddr[0].u32Stride_Y * (*pstConfigFrame)->u32Height;

	if (HI_FALSE == Win_checkFrameAddrValid((*pstConfigFrame)->stBufAddr[0].u32PhyAddr_Y,
											(*pstConfigFrame)->stBufAddr[0].u32Stride_Y,
											(*pstConfigFrame)->stBufAddr[0].u32PhyAddr_C,
											(*pstConfigFrame)->stBufAddr[0].u32Stride_C,
											(*pstConfigFrame)->u32Height))
	{
		WIN_FATAL("Wbc write out of range,W/H:%d,%d.\n",(*pstConfigFrame)->u32Width, (*pstConfigFrame)->u32Height);
		return;
	}
	

	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)(&(*pstConfigFrame)->u32Priv[0]);
	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
	
	pstFramePrivate->eColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
	pstFramePrivate->eOriginField = eWbcInFieldMode_Out;
	pstFramePrivate->u32Fidelity = u32Fidelity;
	pstFramePrivate->stVideoOriginalInfo.u32Width = pstSdDispRect->s32Width;
	pstFramePrivate->stVideoOriginalInfo.u32Height = pstSdDispRect->s32Height;
	pstFramePrivate->stWBC2SDDispRect = *pstSdDispRect;
	pstWbcFramePrivate->u32FrameStatus = FRAME_IN_USE;
	pstWbcFramePrivate->bFrameValid = HI_TRUE;
	
	pstFramePrivate->bAspectProcessed = HI_TRUE;

    /*set the lowdelay mode to wbcframe node.*/
	pstWbcFramePrivate->bLowDelay  = bLowDelayMode;
	
#if 0
	HI_U32 u32vtthd = 0, bBtm = 0, bBtm1 =0, u32Vcnt = 0, u32Vcnt1 = 0;
	HI_U32 u32current_line = 0, u32current_line1 = 0;	
	DISP_INTF_OPERATION_S stFunction;
	
	HI_DRV_DISP_STEREO_MODE_E  enStereo;
	HI_DRV_DISP_FMT_E enEncFmt, enEncFmt1; 
		
	DISP_GetFormat(HI_DRV_DISPLAY_1, &enStereo, &enEncFmt1);
	DISP_GetFormat(HI_DRV_DISPLAY_0, &enStereo, &enEncFmt);
	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);	
	stFunction.FP_GetChnBottomFlag(0, &bBtm, &u32Vcnt);
    stFunction.FP_GetChnBottomFlag(1, &bBtm1, &u32Vcnt1);
	
	u32current_line = stFunction.PF_GetAccurateLinePosition(HI_DRV_DISPLAY_0, enEncFmt);
	u32current_line1 = stFunction.PF_GetAccurateLinePosition(HI_DRV_DISPLAY_1, enEncFmt1);

	WIN_ERROR("genarate:%x,%d,time:%llx, hd currline:%d,sd line:%d, hd/sd:%d,%d\n", 
		  (*pstConfigFrame)->stBufAddr[0].u32PhyAddr_Y,
		  eWbcInFieldMode_Out,
		  get_jiffies_64(),
		  u32current_line1,
		  u32current_line,
		  bBtm1,
		  bBtm);

	 u32DhdM_Intr_Line = u32current_line1;
#endif
	
	return;
}

HI_S32 Wbc_ReleaseOneFrame(WB_POOL_S *pstWinBP, DISP_BUF_NODE_S *pstWBNode)
{
    HI_S32 nRet;

    WIN_CHECK_NULL_RETURN(pstWinBP);	
    nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);
	
    return HI_SUCCESS;
}

/*called by V0's ISR process.*/
HI_S32  WinHd_GenarateWbcInfo(WINDOW_S *pstWin,
										HI_RECT_S *pstHdWinInputSize,
										HI_RECT_S *pstHdWinOutputSize,
										HI_DRV_DISP_CALLBACK_INFO_S *pstInfo,
										HI_U32 u32WinNum,
										HI_BOOL bFieldRead,
										HI_BOOL bFieldOutput,
										HI_U32 u32Fidelity)
{
	HI_BOOL bHdInterfaceInterleave = HI_FALSE;
	HI_BOOL  bWbcInputFrameMode = HI_FALSE;
	HI_DRV_FIELD_MODE_E  eWbcFieldMode_In = HI_DRV_FIELD_BUTT, eWbcFieldMode_Out = HI_DRV_FIELD_BUTT;
	HI_BOOL bP2IOpened = HI_FALSE, bLowdelay = HI_TRUE;
	VDP_LAYER_CONN_E eWbcConnection = VDP_LAYER_CONN_VIDBZME;
	HI_RECT_S stSdDispRect;	
	HI_RECT_S stWbcInputDispRect;	
	HI_RECT_S stSdFmtRect;
	HI_DISP_DISPLAY_INFO_S  *pstDispHd = HI_NULL, *pstDispSd = HI_NULL;
	HI_RECT_S stHdWinInputSize = *pstHdWinInputSize;	
	
	/*get slave window.*/
	WINDOW_S *pstSlvWin;
	HI_DRV_VIDEO_FRAME_S *pstConfigFrame = HI_NULL;
	
	Win_SetSingleBufMode_WbcEffectiveFlag (HI_FALSE);
	
	WinCheckWindow(pstWin->hSlvWin, pstSlvWin);
	
	/*if wbc not initialized or wbc is working, just return.*/
	if (!Win_GetWbcInitFlag())
	{
		return HI_SUCCESS;
	}
	
	/*if slave window not enabled, wbc should not work. */
	if(!pstSlvWin->bEnable)
	{		
		stWbcConfigInfor.bEnable  = HI_FALSE;
		return HI_SUCCESS;
	}
	
	stWbcConfigInfor.pstCurrentFrame     = HI_NULL;	
	/*get current hd's p/I  attribution.*/	
	pstDispHd = DISP_GetDispBasicInfor(HI_DRV_DISPLAY_1);
	pstDispSd = DISP_GetDispBasicInfor(HI_DRV_DISPLAY_0);	
	stSdFmtRect = pstDispSd->stFmtResolution;
	
	/*1  decide the wbc point and  output size. */
	Win_Get_WbcPointAndOutSize(&stHdWinInputSize,
								pstHdWinOutputSize,
								(HI_RECT_S *)&pstInfo->stDispInfo.stFmtResolution,								
								&stSdFmtRect,								
								&stSdDispRect,
								u32WinNum,
								&eWbcConnection,
								bFieldRead,
								bFieldOutput);
	
	/*2 decide the wbc's input frame/field mode. */
	bHdInterfaceInterleave = pstInfo->stDispInfo.bInterlace;
	if (!bHdInterfaceInterleave
		|| (!bFieldRead && (eWbcConnection >= VDP_LAYER_CONN_VIDBZME)))
	{	
		 bWbcInputFrameMode = HI_TRUE;
	}
	else		
	{
		bWbcInputFrameMode = HI_FALSE;
	}
	
	/*3  decide the wbc's input size according: 1) wbc point*/
	Win_GetWbcinputSize(eWbcConnection,
						&stHdWinInputSize,
						&pstInfo->stDispInfo.stFmtResolution,
						&stWbcInputDispRect);	

	/*4 decide input and output field mode.*/
	(HI_VOID)WinHd_Decide_WbcInputOutput_FieldMode(pstWin,
										  pstSlvWin,
										  bWbcInputFrameMode,
										  pstInfo,
    									  &eWbcFieldMode_Out,
										  &eWbcFieldMode_In);

	
	/*5 whether the P2I opened or not.*/
	bP2IOpened = Win_Decide_WbcP2iOpenOrNot(&stWbcInputDispRect,
										    &stSdDispRect,
										    bWbcInputFrameMode,
										    eWbcFieldMode_Out);
	
	/*set the lowdelay  flag.  when the refresh rate is the same, lowdelay needed */	
	(HI_VOID)Win_SetLowdelayMode_AndSwitch(pstDispHd->u32RefreshRate, pstDispSd->u32RefreshRate);
	bLowdelay = WinSd_Chn_Get_LowdelayMode();
	
	/*get empty node from wbc's list,  when wbc finished, insert it to sd's buffer.*/
	if (WinLinker_GetNewNodeToWrite(bLowdelay))
	{
		WIN_WARN("No frame to genarate wbcconfig!\n");
		return HI_FAILURE;
	}
	
	/*genarate the frame infor.*/
	WinHd_GenarateFrameInfor(&pstConfigFrame,
							 &stSdDispRect,
							 eWbcFieldMode_Out,
							 u32Fidelity, 
							 bLowdelay);
	
	stWbcConfigInfor.bOpenP2I = bP2IOpened;
	if (stWbcConfigInfor.bOpenP2I)
	{
		stWbcConfigInfor.eWbcFieldMode_In = eWbcFieldMode_Out;
	}
	else
	{
		stWbcConfigInfor.eWbcFieldMode_In = eWbcFieldMode_In;
	}
	
	stWbcConfigInfor.eWbcFieldMode_Out = eWbcFieldMode_Out;	
	
	stWbcConfigInfor.eWbcConnection = eWbcConnection;	
	
	/* PAY ATTENTION please:
	  *the input size is not frame size, because v0 may do some crop. */
	/*stWbcInputDispRect means the input of whole wbc, not through p2i */
	stWbcConfigInfor.stinputSize = stWbcInputDispRect;

	/*update the pstSlvWin's Attr.*/
	pstSlvWin->stUsingAttr.stOutRect = stSdDispRect;
	stWbcConfigInfor.stUsingAttr = pstSlvWin->stUsingAttr;
	stWbcConfigInfor.bEnable = HI_TRUE;
	
	stWbcConfigInfor.pstCurrentFrame = pstConfigFrame;
	stWbcConfigInfor.stDispInfo = pstInfo->stDispInfo;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcGenarateSuccessCounts++;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcP2IOpenedOrNot = stWbcConfigInfor.bOpenP2I;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.eCurrentWbcPoint = stWbcConfigInfor.eWbcConnection;	
	return HI_SUCCESS;
	
}


HI_VOID Win_GetWbcInfor(HI_DRV_PIX_FORMAT_E *pewbcInputPixel,
	HI_DRV_PIX_FORMAT_E *pewbcOutputPixel,
	HI_DRV_FIELD_MODE_E  *peWbcFieldMode_In,
	HI_DRV_FIELD_MODE_E  *peWbcFieldMode_Out)
{
	if (stWbcInfor.ewbcInputPixel == HI_DRV_PIX_FMT_NV61_2X1)
	{
		*pewbcInputPixel  = 0;
	}
	else if(stWbcInfor.ewbcInputPixel == HI_DRV_PIX_FMT_NV21)
	{
		*pewbcInputPixel  = 1;
	}
	else
	{
		*pewbcInputPixel  = 2;
	}
	
	//*pewbcOutputPixel = stWbcInfor.ewbcOutputPixel ;
	*pewbcOutputPixel = 0;	
	*peWbcFieldMode_In = stWbcInfor.eWbcFieldMode_In;
	*peWbcFieldMode_Out = stWbcInfor.eWbcFieldMode_Out;

	return ;
}


/* We advanced a conception:linker,  which links win_hd and win_sd.  
  * write stream from hd and to sd.    
  * registered in the same isr point with v0.  
  */
HI_S32 Win_ConfigWbc(HI_DRV_WIN_WBC_Linker_S *pstWbcLinker)
{
	DISP_INTF_OPERATION_S stFunction;
	HI_DRV_VID_FRAME_ADDR_S stAddr;
	HI_BOOL bBtm;
	HI_U32  u32Vcnt;	
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = HI_NULL;
	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);

	if ((pstWbcLinker->bEnable == HI_FALSE)  || (!pstWbcLinker->pstCurrentFrame))
	{		
		stFunction.PF_SetWbcEnable(DISP_WBC_Video, HI_FALSE);
		stFunction.PF_UpdateWbc(DISP_WBC_Video);		
		Win_SetSingleBufMode_WbcEffectiveFlag (HI_FALSE);
		return  HI_FAILURE;
	}
	
	Win_SetSingleBufMode_WbcEffectiveFlag (HI_TRUE);	
	
	if (pstWbcLinker->eWbcConnection == VDP_LAYER_CONN_VIDBZME)
	{
		stWbcInfor.ewbcInputPixel = HI_DRV_PIX_FMT_NV21;
	}
	else if (pstWbcLinker->eWbcConnection == VDP_LAYER_CONN_VIDAZME)
	{
		stWbcInfor.ewbcInputPixel = HI_DRV_PIX_FMT_NV61_2X1;
	}
	else if (pstWbcLinker->eWbcConnection == VDP_LAYER_CONN_VPACSC)
	{
		stWbcInfor.ewbcInputPixel = HI_DRV_PIX_FMT_NV61_2X1;
	}
	
	/*wbc's out pixel fmt is fixed.*/
	stWbcInfor.ewbcOutputPixel = HI_DRV_PIX_FMT_NV61_2X1;
	stWbcInfor.eWbcFieldMode_In = pstWbcLinker->eWbcFieldMode_In;
	stWbcInfor.eWbcFieldMode_Out = pstWbcLinker->eWbcFieldMode_Out;
	pstWbcLinker->pstCurrentFrame->enFieldMode =  stWbcInfor.eWbcFieldMode_Out;
	
	VDP_WBC_SetProToInterEnable(VDP_LAYER_WBC_HD0, pstWbcLinker->bOpenP2I);
	
	/*config the pixel fmt.*/
	stFunction.PF_SetWbcPixFmt(DISP_WBC_Video, HI_DRV_PIX_FMT_NV61_2X1);

	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&pstWbcLinker->pstCurrentFrame->u32Priv[0];
	
	/*all the size not think about field or frame.*/
	stFunction.PF_SetWbcIORect(DISP_WBC_Video, 
					&pstWbcLinker->stDispInfo,
					&pstWbcLinker->stinputSize,
					&pstWbcLinker->stUsingAttr.stOutRect,
					pstFramePrivate->u32Fidelity);
	
	/*1 when wbc in v0, before zme or after zme,
	  *  all the content the wbc see is effective.
	  * 2 but in vp ,the wbc sees  is the whole size equal to the fmt. 
	  ***/
	if (pstWbcLinker->eWbcConnection >= VDP_LAYER_CONN_VIDAZME)
	{
		stFunction.PF_SetWbc3DInfo(DISP_WBC_Video, &pstWbcLinker->stDispInfo,&pstWbcLinker->stinputSize);
	}
	else
	{
		/*wbc located on v0.*/
		stFunction.PF_SetWbc3DInfo(DISP_WBC_Video, &pstWbcLinker->stDispInfo,&pstWbcLinker->stDispInfo.stFmtResolution);
	}
	
	VDP_WBC_SetConnection(VDP_LAYER_WBC_HD0, pstWbcLinker->eWbcConnection);

	/*configure wbc csc.*/
	stFunction.PF_SetWbcColorSpace(DISP_WBC_Video, pstWbcLinker->eSrcCS , pstWbcLinker->eDstCS);

	/*fill the frame's addr to wbc, to write stream into it.*/
	stAddr.u32PhyAddr_Y  =  pstWbcLinker->pstCurrentFrame->stBufAddr[0].u32PhyAddr_Y;
	stAddr.u32Stride_Y   =  pstWbcLinker->pstCurrentFrame->stBufAddr[0].u32Stride_Y;	
	stAddr.u32PhyAddr_C  =  pstWbcLinker->pstCurrentFrame->stBufAddr[0].u32PhyAddr_C;
	stAddr.u32Stride_C   =  pstWbcLinker->pstCurrentFrame->stBufAddr[0].u32Stride_C;
	
	/*set the  wbc's output addr.*/
	stFunction.PF_SetWbcAddr(DISP_WBC_Video, &stAddr);
	stFunction.PF_SetWbcEnable(DISP_WBC_Video, HI_TRUE);
	stFunction.PF_UpdateWbc(DISP_WBC_Video);
	stFunction.FP_GetChnBottomFlag(HI_DRV_DISPLAY_1, &bBtm, &u32Vcnt);
	
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcInputFiledMode  = stWbcInfor.eWbcFieldMode_In;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcOutputFiledMode  = stWbcInfor.eWbcFieldMode_Out;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32DhdFieldModeWhenConfigWbc  = bBtm;
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32DhdCurrentLineWhenConfigWbc  = u32Vcnt;
	
	return HI_SUCCESS;
}

HI_VOID WIN_Stop_Wbc(HI_VOID)
{
	return;
}

/*called by wbc's 10% isr.*/
HI_DRV_WIN_WBC_Linker_S  *WinHd_GetWbcInfo(HI_VOID)
{
	return &stWbcConfigInfor;	
}

HI_VOID Wbc_InsertNode_ToFullList(const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
	DISP_INTF_OPERATION_S stFunction;	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);
	
#if 0
	HI_DRV_VIDEO_FRAME_S  *pstFrame = HI_NULL;
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = HI_NULL;
	HI_U32 u32PartTime = 0, u32FinishTime = 0;
	WBC_FRAME_PRIVATE_S  *pstWbcFramePrivate = HI_NULL;	
	/*insert the node to full list.*/
	DispBuf_AddFullNode(&stWbcConfigInfor.stWinBP.stBuffer, stWbcConfigInfor.stWinBP.pstConfig);

	pstFrame = (HI_DRV_VIDEO_FRAME_S *)stWbcConfigInfor.stWinBP.pstConfig->u32Data;	
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0];	
	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);	
	pstWbcFramePrivate->ullExpectWbcFinishTimeMs = (unsigned long long)0xffffffffffffffff;

	(HI_VOID)stFunction.PF_GetIsoWbcPartFinishPoint(HI_DRV_DISPLAY_1,
													pstInfo->stDispInfo.eFmt,
													WBC_PARTFINISH_PERCENT_0,
													&u32PartTime,
													&u32FinishTime);
	
	if (pstWbcFramePrivate->bLowDelay)
	{
		pstWbcFramePrivate->ullExpectWbcFinishTimeMs = (unsigned long long)u32PartTime + get_jiffies_64();
	}
	else
	{
		pstWbcFramePrivate->ullExpectWbcFinishTimeMs = (unsigned long long)u32FinishTime + get_jiffies_64();
	}
#endif

	return ;
}

HI_VOID ISR_CallbackForConfigWbc(HI_VOID* hDst, 
											const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{	
	HI_DRV_WIN_WBC_Linker_S *pstWbcLinkerInfo;
	
	if ((pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
            ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND))
    {
		return;
    }

	if (!Win_GetWbcInitFlag())
		return;
	
	stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigTryCounts++;	
	
    /*1 generate the config infor*/
    pstWbcLinkerInfo = WinHd_GetWbcInfo();
	
    /*2 config the wbc hardware*/
	if (!Win_ConfigWbc(pstWbcLinkerInfo))
	{
#if 0		
		HI_U32 u32vtthd = 0;
		HI_U32 u32current_line = 0;		
		DISP_INTF_OPERATION_S stFunction;
		HI_DRV_DISP_STEREO_MODE_E  enStereo;
		HI_DRV_DISP_FMT_E enEncFmt; 
		
		(HI_VOID)DISP_HAL_GetOperation(&stFunction);		
		VDP_DISP_GetVtThd(0, 1, &u32vtthd);

		DISP_GetFormat(HI_DRV_DISPLAY_1, &enStereo, &enEncFmt);
		
		u32current_line = stFunction.PF_GetAccurateLinePosition(1,enEncFmt);
		WIN_ERROR("--hd fmt:%d, vtthd:%d,current line:%d, jif:%llx!\n", enEncFmt,u32vtthd, u32current_line, get_jiffies_64());
#endif
		Wbc_InsertNode_ToFullList(pstInfo);		

		stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigOkCounts++;
		stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigSpeed =  
			 stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigOkCounts * 1000 / (jiffies - stWbcConfigInfor.u32StartTime);
	}
	
	pstWbcLinkerInfo->pstCurrentFrame = HI_NULL;	
   return ;
}


static HI_VOID Wbc_CalculateSpeed(HI_VOID)
{
	HI_U32 u32TotalWbcCounts = stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishPushFrameCnts;
	
	stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishSpeed = (u32TotalWbcCounts * 1000) / (jiffies - stWbcConfigInfor.u32StartTime);
	stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcStartTime = stWbcConfigInfor.u32StartTime;
    stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcCurrentTime = jiffies;
	
	return ;
}

/**********************************partfinish********************************************************/
HI_S32 LinkerBuf_Init(HI_VOID)
{
	HI_U32 i = 0;
	
	for(i = 0; i < DISP_BUF_NODE_MAX_NUMBER; i++)
	{
		u32Partfinish_UsedArray[i] = HI_NULL;
	}
	
    u32UsedArray_ReadPos = 0;
    u32UsedArray_WritePos = 0;

	return HI_SUCCESS;
}

HI_S32 LinkerBuf_GetUsedNode(HI_U8 **pu8NodePtr)
{
    if (u32Partfinish_UsedArray[u32UsedArray_ReadPos] != HI_NULL)
    {
		*pu8NodePtr = u32Partfinish_UsedArray[u32UsedArray_ReadPos];
        return HI_SUCCESS;
    }
	
	*pu8NodePtr = 0;	
    return HI_FAILURE;
}

// take node away from empty array and set Writing State
HI_S32 LinkerBuf_DelUsedNode(HI_U8 *pu8NodePtr)
{
    HI_U8 *pu8NodeID;
	
    DISP_ASSERT(u32Partfinish_UsedArray[u32UsedArray_ReadPos] != HI_NULL);	
    pu8NodeID = u32Partfinish_UsedArray[u32UsedArray_ReadPos];
	
    DISP_ASSERT(pu8NodePtr == pu8NodeID);
	
    u32Partfinish_UsedArray[u32UsedArray_ReadPos] = HI_NULL;
    u32UsedArray_ReadPos = (u32UsedArray_ReadPos + 1)%DISP_BUF_NODE_MAX_NUMBER;
	
    return HI_SUCCESS;
}

// put node into empry array and set empty state
HI_S32 LinkerBuf_AddUsedNode(HI_U8 *pu8NodePtr)
{
    DISP_ASSERT(u32Partfinish_UsedArray[u32UsedArray_WritePos] == HI_NULL);
	
    u32Partfinish_UsedArray[u32UsedArray_WritePos] = pu8NodePtr; 
    u32UsedArray_WritePos = (u32UsedArray_WritePos + 1)%DISP_BUF_NODE_MAX_NUMBER;	
    return HI_SUCCESS;
}

HI_VOID Linker_RetriveFrameFromWinSd_ReleaseToWbc(WINDOW_S *pstWin)
{
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = 0;
	WBC_FRAME_PRIVATE_S    *pstWbcFramePrivate = HI_NULL;
	HI_DRV_VIDEO_FRAME_S   stWinsdFrm;
	HI_DRV_VIDEO_FRAME_S   *pstWbcFrm = HI_NULL, *pstFrame_tmp = HI_NULL;
	DISP_BUF_NODE_S *pstNode = HI_NULL;
	HI_U8  *pu8Node = 0;
	
	/*win sd has no frame node.*/
	if (WinSd_Dequeue_DisplayedFrame(pstWin, &stWinsdFrm))
		return;

	/*get the wbc's frame addr.*/
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)&stWinsdFrm.u32Priv[0];
	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
	pstWbcFrm = (HI_DRV_VIDEO_FRAME_S *)pstWbcFramePrivate->pstFrameAddr;
	


	/*set the wbc's frame flag. indicates that the frame is free. wbc can use it.*/
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)&pstWbcFrm->u32Priv[0];
	pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
	pstWbcFramePrivate->u32FrameStatus  = FRAME_FREE;
	
	while(1)
	{
		/*return the node to wbc*/
		if (!LinkerBuf_GetUsedNode(&pu8Node))
		{			
			pstNode = (DISP_BUF_NODE_S *)pu8Node;			
			pstFrame_tmp = (HI_DRV_VIDEO_FRAME_S *)pstNode->u32Data;
			
			/*get the frame's  private area.*/
			pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S*)&pstFrame_tmp->u32Priv[0];
			pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);
			
			/*check the frame's free flag.*/
			if (pstWbcFramePrivate->u32FrameStatus == FRAME_FREE)
			{
				/*delete from linker's list.*/
				LinkerBuf_DelUsedNode((HI_U8*)pstNode);
				stWbcIsogenyProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcRetriveFromSdNodeCounts++;
				Wbc_ReleaseOneFrame(&stWbcConfigInfor.stWinBP, pstNode);
			}
			else
			{
				break;
			}			
		}
		else
		{
			break;
		}
	}

	return;
}


HI_VOID WbcLinker_Proc(HI_VOID)
{	
	HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;
	WINDOW_S *pstWin = HI_NULL;
	Linker_INFOR_S *pstLinkerInfor = HI_NULL;
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = HI_NULL;
	HI_DRV_VIDEO_FRAME_S stWinsdFrm;

	if (!Win_GetWbcInitFlag())
		return;
	
	
	jiffies_now_wbc =  jiffies;
	
	if ((jiffies_now_wbc - jiffies_old_wbc) > 5)
		stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcHighLantencyTimes ++;
	
	jiffies_old_wbc =  jiffies;
	
	pstLinkerInfor = Thread_Get_LinkerStatus();
	if (!pstLinkerInfor->bEnable)
		return;

	pstWin = (WINDOW_S *)pstLinkerInfor->hSdWinHanle;
	if (!pstWin)
		return;
	
	stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishTrycounts ++;	
		
	while(1)
	{	/*dequeue frame sd's window.*/	
		if (WinSd_Dequeue_DisplayedFrame(pstWin, &stWinsdFrm))
			break;
	}
	
	{
	#if 0
		HI_S32 s32Ret = 0;
		DISP_BUF_NODE_S *pstNode = HI_NULL;	
		/*when in non-lowdelay mode,  we should get the full node from wbc's list.*/
		s32Ret = DispBuf_GetFullNode(&stWbcConfigInfor.stWinBP.stBuffer, &pstNode);
		if (s32Ret)
		{
			WIN_WARN("there is no fram in wbc buffer!\n");
			return;
		}
		
		/*if finished,  delete it from wbc.*/
		s32Ret = DispBuf_DelFullNode(&stWbcConfigInfor.stWinBP.stBuffer, pstNode);
		if (s32Ret)
		{
			WIN_FATAL("return to used list error!\n");
			return;
		}
		
		(HI_VOID)DispBuf_AddEmptyNode(&stWbcConfigInfor.stWinBP.stBuffer, pstNode);
	#endif

		if (Win_GetSingleBufMode_WbcEffectiveFlag() != HI_TRUE)
		{
			return;
		}
	
		if (stWbcConfigInfor.stWinBP.pstConfig)
		{
			WBC_FRAME_PRIVATE_S *pstWbcFramePrivate = HI_NULL;
			/*check wbc's begin to write or not.*/
			pstFrame = (HI_DRV_VIDEO_FRAME_S *)stWbcConfigInfor.stWinBP.pstConfig->u32Data;		
			pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0];
			pstWbcFramePrivate =  (WBC_FRAME_PRIVATE_S  *)(&pstFramePrivate->u32Reserve[0]);			
			if (pstWbcFramePrivate->bFrameValid == HI_TRUE)
			{				
				pstFramePrivate->u32PlayTime = 1;	
				pstFrame->u32FrameIndex = stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishPushFrameCnts;
				pstFrame->u32SrcPts = 0xffffffff;
				pstFrame->u32Pts	= jiffies;
				
				WinBuf_PutNewFrame(&pstWin->stBuffer.stWinBP,pstFrame); 
				Wbc_CalculateSpeed();
				
				stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishPushFrameCnts ++;
				stWbcIsogenyProcInfor.stPartFinishThreadProcInfor.ePartFinishThreadState = PARTFINISH_THREAD_STATUS_PUSH_FRAME;
				pstWbcFramePrivate->bFrameValid = HI_FALSE;
			}
			
			Win_SetSingleBufMode_WbcEffectiveFlag (HI_FALSE);
		}		
	}
	
	return;
}

HI_BOOL Thread_Get_WbcEnableStatus(HI_VOID)
{
	return stWbcConfigInfor.bEnable;
}

HI_BOOL Thread_Get_SdWinEnableStatus(WINDOW_S *pstWin)
{
	return pstWin->bEnable;
}

HI_VOID Thread_Init_Linker(HI_VOID)
{
	memset((HI_VOID*)&g_stLinker, 0, sizeof(Linker_INFOR_S));
	return;
}

HI_VOID Thread_Set_LinkerStatus(WINDOW_S *pstWin)
{
	if (Thread_Get_WbcEnableStatus() && Thread_Get_SdWinEnableStatus(pstWin))
	{
		g_stLinker.bEnable =  HI_TRUE;
	}
	else
	{
		g_stLinker.bEnable =  HI_FALSE;
	}
	
	g_stLinker.hSdWinHanle = (HI_VOID*)pstWin;	
	return;
}

Linker_INFOR_S *Thread_Get_LinkerStatus(HI_VOID)
{
	return &g_stLinker;
}

/*isr for wbc part finish process.*/
HI_S32 Thread_CallbackForLinker(HI_VOID* hDst)
{
    WINDOW_S *pstWin;
	
    if (!hDst)
    {
        WIN_ERROR("WIN Input Null pointer in %s!\n", __FUNCTION__);
        return HI_FAILURE;
    }
	
	pstWin = (WINDOW_S *)hDst;
	LinkerBuf_Init();
	Thread_Init_Linker();

	(HI_VOID)Wbc_PartFinish_InitTimer();
	
	while (1)
    {
        if (kthread_should_stop())
            break;
		
		Thread_Set_LinkerStatus(pstWin);		
		msleep(5);
	}

	/*hrtime cancel  is synchronous and blocked func. when return,
	   means the timer is deleted completely.*/
	Wbc_PartFinish_DeleteTimer();	
	 return HI_SUCCESS;
}

/****************************sd's v3 window*******************************************************/

/*
  * dequeue func works, this func should be called, retrieve the frames.
  */
HI_S32 WinSd_Dequeue_DisplayedFrame(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 nRet = 0;
    nRet = WinBufferGetULSFrame(&pstWin->stBuffer, pstFrm); 
    return nRet;    
}

HI_S32 WinSd_ReleaseFrameToUlsList(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 nRet = 0;
	
    nRet = WinBufferPutULSFrame(&pstWin->stBuffer, pstFrm); 
    return nRet;
}


/*for v3 consumer, return displayed frame to list.*/
HI_VOID WinSd_ReturnFrame(WINDOW_S *pstWin,WB_POOL_S *pstWinBP)
{
	HI_S32 s32Ret = -1;
	HI_DRV_VIDEO_FRAME_S *pstCurrFrame  = HI_NULL;	
	
	if (!pstWinBP->pstDisplay)
		return;
	
	/*when release frame, add release refcnt for proc information.*/
	pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstDisplay->u32Data;
	
	/*set the frame's state, can be used by wbc.*/
	(HI_VOID)WinSd_ReleaseFrameToUlsList(pstWin, pstCurrFrame);
	
	/* release disp buffer*/
    s32Ret =  DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWinBP->pstDisplay);
	if (HI_SUCCESS == s32Ret)
	{
		pstWinBP->pstDisplay = HI_NULL;
	}

	stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdReturnBackFrameCounts ++;
	WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
	
	return;
}


/*for v3, to get new frame displayed*/
HI_DRV_VIDEO_FRAME_S * WinSd_GetConfigFrame(WB_POOL_S *pstWinBP)
{	
	HI_DRV_VIDEO_FRAME_S *pstCurrFrame = HI_NULL;
	
	/*set display frame  to last configured frame.*/
	pstWinBP->pstDisplay = pstWinBP->pstConfig;	
	pstWinBP->pstConfig = HI_NULL;
	
	/*for wbc producer,  insert a  frame into it.*/
	DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstWinBP->pstConfig);
	
	if ((pstWinBP->pstConfig == HI_NULL) && (pstWinBP->pstDisplay == HI_NULL))
	{
		return HI_NULL;		
	}
	else if ((pstWinBP->pstConfig == HI_NULL) && (pstWinBP->pstDisplay != HI_NULL))
	{
		pstWinBP->pstConfig = pstWinBP->pstDisplay;		
		pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;	
		if (pstCurrFrame == HI_NULL)
			return HI_NULL;
	}
	else
	{		
		pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;	
		if (pstCurrFrame == HI_NULL)
			return HI_NULL;
		
		WinBuf_DebugAddCfg(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);	
		DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWinBP->pstConfig);
	}
	
	return pstCurrFrame;
}

HI_DRV_VIDEO_FRAME_S * WinSd_GetMatchedFrame(WINDOW_S *pstWin,
											 WB_POOL_S *pstWinBP,
											 HI_BOOL bBtm)
{	
	HI_DRV_VIDEO_FRAME_S *pstCurrFrame = HI_NULL;
	DISP_BUF_NODE_S *pstCurrentNode  = HI_NULL;
	DISP_BUF_NODE_S *pstFieldMatch = HI_NULL;
	HI_U32 i = 0;
	HI_S32 s32Ret = 0;
		
	/*set display frame  to last configured frame.*/
	pstWinBP->pstDisplay = pstWinBP->pstConfig;
	pstWinBP->pstConfig = HI_NULL;
		
	/*check whether there is a frame matched the current field.*/
	while (1)
	{
		pstCurrentNode = HI_NULL;		
		if (HI_SUCCESS == DispBuf_GetFullNodeByIndex(&pstWinBP->stBuffer,i, &pstCurrentNode))
		{
			pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstCurrentNode->u32Data;
			if (pstCurrFrame->enFieldMode == (1 - bBtm))
			{
				pstFieldMatch = pstCurrentNode;
			}
		}
		else
		{
			break;
		}
		
		i ++;
	}

	/*the full list has no node match the current dhd filed, so check the node we have disaplayed.*/
	if (pstFieldMatch == HI_NULL)
	{
		if (pstWinBP->pstDisplay)
		{
			pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstDisplay->u32Data;
			if (pstCurrFrame->enFieldMode == (1 - bBtm))
			{
				/*we shoule play the played frame again for match.*/
				pstWinBP->pstConfig = pstWinBP->pstDisplay;
				return pstCurrFrame;
			}
		}
	}
	else if (pstFieldMatch)
	{
		pstCurrentNode = HI_NULL;
		
		while (!DispBuf_GetFullNode(&pstWinBP->stBuffer,&pstCurrentNode))
		{

			DispBuf_DelFullNode(&pstWinBP->stBuffer, pstCurrentNode);
			if (pstCurrentNode != pstFieldMatch)
			{	
				/*when release frame, add release refcnt for proc information.*/
				pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstCurrentNode->u32Data;
				
				/*set the frame's state, can be used by wbc.*/
				(HI_VOID)WinSd_ReleaseFrameToUlsList(pstWin, pstCurrFrame);
				
				/* release disp buffer*/
				s32Ret =  DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstCurrentNode);
				if (s32Ret)
				{
					WIN_FATAL("add empty node failed!\n");
				}
				
				stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdReturnBackFrameCounts ++;
				WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);				
			}
			else
			{
				pstWinBP->pstConfig = pstCurrentNode;
				pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;
				return pstCurrFrame;
			}

			pstCurrentNode = HI_NULL;			
		}
		
		if (pstWinBP->pstConfig == HI_NULL)
		{
			WIN_FATAL("The matched frame not found.");
		}
	}


	/*when come here, we did not find a proper frame, so we just ge the  nearest frame.*/
	if(!DispBuf_GetFullNode(&pstWinBP->stBuffer,&pstCurrentNode))
	{
		DispBuf_DelFullNode(&pstWinBP->stBuffer, pstCurrentNode);
		pstWinBP->pstConfig = pstCurrentNode;
		pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;
		return pstCurrFrame;
	}
	else
	{
		pstWinBP->pstConfig = pstWinBP->pstDisplay;
		if (pstWinBP->pstConfig)
		{
			pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;
			return pstCurrFrame;
		}
		else
		{
			return HI_NULL;
		}		
	}
	
	return HI_NULL;
}


extern HI_S32  WinUpdatPara(WINDOW_S *pWin);
HI_DRV_VIDEO_FRAME_S * WinSd_Search_FieldMatchFrame(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;
	HI_BOOL bBtm = 0;
	HI_U32 u32Vcnt= 0;
	DISP_INTF_OPERATION_S stFunction;	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);
	stFunction.FP_GetChnBottomFlag(0, &bBtm, &u32Vcnt);
	
	pstFrame  = WinSd_GetMatchedFrame(pstWin, &pstWin->stBuffer.stWinBP, bBtm);
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32DsdFieldMode     = bBtm;
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32DsdCurrentLine   = u32Vcnt;
	
	return pstFrame;
}

HI_DRV_VIDEO_FRAME_S * WinSd_Search_NewestFrame(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame = HI_NULL;
	HI_BOOL bBtm = 0;
	HI_U32 u32Vcnt = 0;	
	HI_S32 s32Ret  = 0;
	DISP_INTF_OPERATION_S stFunction;
	DISP_BUF_NODE_S *pstCurrentNode  = HI_NULL;
	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);
	stFunction.FP_GetChnBottomFlag(0, &bBtm, &u32Vcnt);
	
	while (!DispBuf_GetFullNode(&pstWin->stBuffer.stWinBP.stBuffer,&pstCurrentNode))
	{		
		DispBuf_DelFullNode(&pstWin->stBuffer.stWinBP.stBuffer, pstCurrentNode);		
		/* release disp buffer*/
		s32Ret =  DispBuf_AddEmptyNode(&pstWin->stBuffer.stWinBP.stBuffer, pstCurrentNode);
		if (s32Ret)
		{
			WIN_FATAL("add empty node failed!\n");
		}
		
		/*when release frame, add release refcnt for proc information.*/
		pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstCurrentNode->u32Data;
		
		/*set the frame's state, can be used by wbc.*/
		(HI_VOID)WinSd_ReleaseFrameToUlsList(pstWin, pstCurrFrame);
		stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdReturnBackFrameCounts ++;
		WinBuf_DebugAddRls(pstWin->stBuffer.stWinBP.pstDebugInfo, pstCurrFrame->u32FrameIndex);
		
#if  0
		HI_BOOL bBtm1 = 0;
		HI_U32 u32Vcnt1= 0;
		
		stFunction.FP_GetChnBottomFlag(0, &bBtm, &u32Vcnt);
        stFunction.FP_GetChnBottomFlag(1, &bBtm1, &u32Vcnt1);
		pstCurrFrame->enFieldMode = 1 - bBtm;
		
		WIN_ERROR("search newest frame hd/sd: %d/%d,diff:%d, frame:%x, field:%d\n", bBtm1,bBtm, u32Vcnt1 - u32Vcnt,
				pstCurrFrame->stBufAddr[0].u32PhyAddr_Y,
				pstCurrFrame->enFieldMode);
#endif

		pstCurrentNode = HI_NULL;			
	}
	
	return pstCurrFrame;	
}

HI_VOID  WinSd_ResetBuffer(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame = HI_NULL;
	HI_S32 s32Ret  = 0;
	DISP_BUF_NODE_S *pstCurrentNode  = HI_NULL;	
	
	while (!DispBuf_GetFullNode(&pstWin->stBuffer.stWinBP.stBuffer,&pstCurrentNode))
	{		
		DispBuf_DelFullNode(&pstWin->stBuffer.stWinBP.stBuffer, pstCurrentNode);		
		/* release disp buffer*/
	    s32Ret =  DispBuf_AddEmptyNode(&pstWin->stBuffer.stWinBP.stBuffer, pstCurrentNode);
		if (s32Ret)
		{
			WIN_FATAL("add empty node failed!\n");
		}
		
		/*when release frame, add release refcnt for proc information.*/
		pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstCurrentNode->u32Data;
		
		/*set the frame's state, can be used by wbc.*/
		(HI_VOID)WinSd_ReleaseFrameToUlsList(pstWin, pstCurrFrame);
		stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdReturnBackFrameCounts ++;
		WinBuf_DebugAddRls(pstWin->stBuffer.stWinBP.pstDebugInfo, pstCurrFrame->u32FrameIndex);
		pstCurrentNode = HI_NULL;			
	}
	
	return;	
}
HI_VOID ISR_CallbackForSd_V3_Output(HI_VOID* hDst, 
												const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;
	HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;
	HI_BOOL bLowdelayModeTmp = HI_FALSE;
	DISP_BUF_NODE_S * pstConfig_tmp = HI_NULL;
	
	HI_DRV_VIDEO_PRIVATE_S *pstFramePrivate = 0;
	WBC_FRAME_PRIVATE_S    *pstWbcFramePrivate = HI_NULL;
	HI_U32 u32PartTime = 0, u32FinishTime = 0;
	DISP_INTF_OPERATION_S stFunction;	
	(HI_VOID)DISP_HAL_GetOperation(&stFunction);	
	
	if (!hDst || !pstInfo )
	{
	   return;
	}

	if ( (pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
            ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND))
    {
		return;
    }

	
#if 0	
	HI_U32 u32vtthd = 0;
	HI_U32 u32current_lineM = 0, u32current_lineS = 0;
	HI_DRV_DISP_STEREO_MODE_E  enStereo;
	HI_DRV_DISP_FMT_E enEncFmtM, enEncFmtS;
	HI_U32 u32TriggerTime = 0;

	DISP_GetFormat(HI_DRV_DISPLAY_1, &enStereo, &enEncFmtM);
	DISP_GetFormat(HI_DRV_DISPLAY_0, &enStereo, &enEncFmtS);
	VDP_DISP_GetVtThd(0, 1, &u32vtthd);
	
	u32current_lineM = stFunction.PF_GetAccurateLinePosition(HI_DRV_DISPLAY_1, enEncFmtM);
	WIN_ERROR("v3--hd fmt:%d,vtthd:%d,current line:%d, jif:%llx!\n", enEncFmtM, u32vtthd, u32current_lineM, get_jiffies_64());

	
	VDP_DISP_GetVtThd(1, 1, &u32vtthd);
	u32current_lineS = stFunction.PF_GetAccurateLinePosition(HI_DRV_DISPLAY_0, pstInfo->stDispInfo.eFmt);
	WIN_ERROR("v3--sd fmt:%d, vtthd:%d,current line:%d, jif:%llx!\n", pstInfo->stDispInfo.eFmt, u32vtthd, u32current_lineS, get_jiffies_64());
	
	u32TriggerTime = stFunction.PF_Calculate_TriggerTime(u32DhdM_Intr_Line, u32current_lineM, u32current_lineS, enEncFmtM, enEncFmtS);
	WIN_ERROR("---trigger time:%d ms!\n", u32TriggerTime);
#endif

	WbcLinker_Proc();
	
    pstWin = (WINDOW_S *)hDst;			

	
	if (atomic_read(&pstWin->stCfg.bNewAttrFlag))
    {
        pstWin->stCfg.stAttr = pstWin->stCfg.stAttrBuf;
        pstWin->stUsingAttr  = pstWin->stCfg.stAttrBuf;
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
    }
	
	if (!pstWin->bEnable || pstWin->bMasked)
    {	
	    /* when disable, we should reset the sd's  buffer**/
        WinSd_ResetBuffer(pstWin);
		if (pstWin->u32VideoLayer != VDP_LAYER_VID_BUTT)
	    {
		    pstWin->stVLayerFunc.PF_SetEnable(pstWin->u32VideoLayer, pstWin->u32VideoRegionNo, HI_FALSE);     
		    WinUpdatPara(pstWin);
	    }
		
        return;
    }
	
	bLowdelayModeTmp  = WinSd_Chn_Get_LowdelayMode();
	
	/*just keep the newest frame,drop others.*/	
	if (bLowdelayModeTmp)
	{	
		pstFrame = WinSd_Search_NewestFrame(pstWin);
	}
	else
	{	
		/*remember the last config ptr,  to  compare with the  newest config.*/
		pstConfig_tmp = pstWin->stBuffer.stWinBP.pstConfig;		
		
		/*from second isr peroid, we begin to try to release the displayed frame.*/
		if ((pstWin->stBuffer.stWinBP.pstDisplay != pstWin->stBuffer.stWinBP.pstConfig)
			&& (pstWin->stBuffer.stWinBP.pstDisplay))		
		{
			WinSd_ReturnFrame(pstWin, &pstWin->stBuffer.stWinBP);
		}
		
		/*do field-match operation.*/
		pstFrame = WinSd_Search_FieldMatchFrame(pstWin);
		if (!pstFrame)
		{
			WIN_FATAL("Sd Channel has no frame!\n");
			return;
		}
		
		stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdFrameFieldMode = pstFrame->enFieldMode; 
		if (pstConfig_tmp == pstWin->stBuffer.stWinBP.pstConfig)
		{
			stWbcIsogenyProcInfor.stSdWinProcInfor.u32SdUnderloadCounts ++;
			pstWin->stBuffer.u32UnderLoad++;
		}
	}

	
	if (!pstFrame)
	{
		WIN_WARN("Sd Channel has no frame!\n");
		return;
	}
	
	(HI_VOID)stFunction.PF_GetIsoWbcPartFinishPoint(HI_DRV_DISPLAY_0,
													pstInfo->stDispInfo.eFmt,
													WBC_PARTFINISH_PERCENT_0,													
													&u32PartTime,
													&u32FinishTime);
	
	/*just for debug.*/
	pstFramePrivate = (HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0];
	pstWbcFramePrivate = (WBC_FRAME_PRIVATE_S *)&pstFramePrivate->u32Reserve[0];
	pstWbcFramePrivate->ullSdReceiveTimeMs = get_jiffies_64();
	pstWbcFramePrivate->ullSdDisplayTimeMs = get_jiffies_64()  + u32PartTime;
	
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32FrameLantencyTimeMs =  get_jiffies_64()
																	  + u32PartTime
																	  - pstWbcFramePrivate->ullExpectWbcFinishTimeMs;

	stWbcIsogenyProcInfor.stSdWinProcInfor.u32FrameExpectWbcTimeMs =  pstWbcFramePrivate->ullExpectWbcFinishTimeMs;
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32FrameLinkerProcessTimeMs =  pstWbcFramePrivate->ullLinkerProcessTimeMs;
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32FrameSdReceiveTimeMs =  pstWbcFramePrivate->ullSdReceiveTimeMs;
	stWbcIsogenyProcInfor.stSdWinProcInfor.u32FrameSdDisplayTimeMs =  pstWbcFramePrivate->ullSdDisplayTimeMs;
	
	/*configure the frame to v3.*/
	if (pstFrame)
		ISR_WinConfigFrame(pstWin, pstFrame,pstInfo);

	return;
}

WIN_WBCISOGENY_PROC_INFOR_S WIN_GetWbcIsoInfor(HI_VOID)
{
	stWbcIsogenyProcInfor.bWbcInit = Win_GetWbcInitFlag();
	return stWbcIsogenyProcInfor;
}
