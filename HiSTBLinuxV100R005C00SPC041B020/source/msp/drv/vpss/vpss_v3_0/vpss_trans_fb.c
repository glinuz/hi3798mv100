#include "vpss_trans_fb.h"
#include "vpss_instance.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_BOOL VPSS_TRANS_FB_CheckTileFmt(HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if((HI_DRV_PIX_FMT_NV12_TILE == pstFrame->ePixFormat)
        || (HI_DRV_PIX_FMT_NV21_TILE == pstFrame->ePixFormat))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_BOOL VPSS_TRANS_FB_NeedTrans(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 u32Width;
    HI_U32 u32FrameRate;  
    HI_DRV_SOURCE_E  enInputSrc;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv; 
    VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_INSTANCE_S* pstInstance;
	HI_BOOL bProgressive;

    if(!pstTransFbList->bInit)
    {
        VPSS_FATAL("Trans fb not init.\n");        
        return HI_FALSE;
    }

    pstInstance = (VPSS_INSTANCE_S *)(pstTransFbList->pstInstance);
    u32Width = pstFrame->u32Width;
    u32FrameRate = pstFrame->u32FrameRate; 
	bProgressive = pstFrame->bProgressive;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrame->u32Priv[0]);
    enInputSrc = pstPriv->stVideoOriginalInfo.enSource;
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);
	
	if(pstInstance->stDbgCtrl.stInstDbg.unInfo.bits.vpssbypass)
    {
        return HI_TRUE;
    }
	
#if 1
#if defined(CHIP_TYPE_hi3716mv410)
    if((HI_DRV_SOURCE_DTV == enInputSrc) 
			&& (VPSS_TRANS_FB_CheckTileFmt(pstFrame))
			&& (u32Width >= 1920) 
			&& (u32FrameRate >= 50000)
			&& (bProgressive == HI_TRUE) 
			&& (VPSS_INST_CheckPassThrough(pstInstance,pstFrame)))
    {
        pstStreamInfo->u32StreamInRate = pstFrame->u32FrameRate;
        pstStreamInfo->u32StreamTopFirst = pstFrame->bTopFieldFirst;
        pstStreamInfo->u32StreamProg = pstFrame->bProgressive;
        pstStreamInfo->u32StreamH = pstFrame->u32Height;
        pstStreamInfo->u32StreamW = pstFrame->u32Width;
        pstStreamInfo->eStreamFrmType = pstFrame->eFrmType;
        pstStreamInfo->u32FrameIndex = pstFrame->u32FrameIndex;
        pstStreamInfo->u32Pts = pstFrame->u32Pts;        
		pstStreamInfo->bStreamByPass = HI_TRUE;

        return HI_TRUE;
    }
#elif defined(CHIP_TYPE_hi3716mv420)

#elif defined(CHIP_TYPE_hi3798cv200_a) || defined (CHIP_TYPE_hi3798cv200_b)
#if 0
	if((HI_DRV_SOURCE_DTV == enInputSrc) && (VPSS_TRANS_FB_CheckTileFmt(pstFrame))
		&& (u32Width > 1920) && (u32FrameRate >= 50000))
	{
		pstStreamInfo->u32StreamInRate = pstFrame->u32FrameRate;
		pstStreamInfo->u32StreamTopFirst = pstFrame->bTopFieldFirst;
		pstStreamInfo->u32StreamProg = pstFrame->bProgressive;
		pstStreamInfo->u32StreamH = pstFrame->u32Height;
		pstStreamInfo->u32StreamW = pstFrame->u32Width;
		pstStreamInfo->eStreamFrmType = pstFrame->eFrmType;
		pstStreamInfo->u32FrameIndex = pstFrame->u32FrameIndex;
		pstStreamInfo->u32Pts = pstFrame->u32Pts;	
		
		pstTransFbList->bNeedTrans = HI_TRUE;	 
		return HI_TRUE;
	}
#endif
#endif
#endif
 //   if(u32Width > 1920)
    if(u32Width > 4096)
    {
        pstStreamInfo->u32StreamInRate = pstFrame->u32FrameRate;
        pstStreamInfo->u32StreamTopFirst = pstFrame->bTopFieldFirst;
        pstStreamInfo->u32StreamProg = pstFrame->bProgressive;
        pstStreamInfo->u32StreamH = pstFrame->u32Height;
        pstStreamInfo->u32StreamW = pstFrame->u32Width;
        pstStreamInfo->eStreamFrmType = pstFrame->eFrmType;
        pstStreamInfo->u32FrameIndex = pstFrame->u32FrameIndex;
        pstStreamInfo->u32Pts = pstFrame->u32Pts;    

		pstTransFbList->bNeedTrans = HI_TRUE;
		
        return HI_TRUE;
    }
    else
    {
		pstTransFbList->bNeedTrans = HI_FALSE;
        return HI_FALSE;
    }
}

HI_S32 VPSS_TRANS_FB_Init(VPSS_TRANS_FB_INFO_S *pstTransFbList, HI_VOID* pstInstance)
{
    if(HI_NULL == pstInstance)
    {
        VPSS_FATAL("Invalid Instance pointer.\n");        
        return HI_FAILURE;
    }

    pstTransFbList->bInit = HI_TRUE;
    pstTransFbList->pstInstance = pstInstance;
    
    return HI_SUCCESS;
}
HI_S32 VPSS_TRANS_FB_DeInit(VPSS_TRANS_FB_INFO_S *pstTransFbList)
{

    pstTransFbList->bInit = HI_FALSE;
    pstTransFbList->pstInstance = HI_NULL;
    
    return HI_SUCCESS;
}    
HI_S32 VPSS_TRANS_FB_PutImage(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
	HI_U32 u32PortId;
	HI_U32 u32Count;
	VPSS_PORT_S* pstPort;    
	VPSS_FB_NODE_S* pstFbNode;
	VPSS_INSTANCE_S* pstInstance;
	HI_DRV_VIDEO_PRIVATE_S *pstPriv;
	
	if(!pstTransFbList->bInit)
	{
		VPSS_FATAL("Trans fb not init.\n");        
		return HI_FAILURE;
	}
	
	pstInstance = (VPSS_INSTANCE_S *)(pstTransFbList->pstInstance);
	
	//DTV 4K@60,VPSS trans it to vdp
	u32Count = 0;
	for (u32PortId = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
	{
		pstPort = &(pstInstance->stPort[u32PortId]);
		if ((pstPort->s32PortId == VPSS_INVALID_HANDLE)
				|| (pstPort->bEnble == HI_FALSE))
		{
			continue;
		}

		pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstFrame->u32Priv;

		pstPriv->eOriginField = HI_DRV_FIELD_ALL;	

		pstFbNode = VPSS_FB_GetEmptyFrmBufNoMmz(&(pstPort->stFrmInfo),pstFrame);

		if(HI_NULL == pstFbNode)
		{
			VPSS_FATAL("Trans fb error,port%d buffer is full.\n",u32PortId);                 
			continue;
		}



		VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo), pstFbNode);           
		u32Count++;
	}

	if(0 == u32Count)
	{
		return HI_FAILURE;
	}
	else if(1 < u32Count)
	{
		VPSS_FATAL("Trans fb warning,use multi port.\n");
	}
	
	return HI_SUCCESS;
}

HI_S32 VPSS_TRANS_FB_RelImage(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    
    VPSS_INSTANCE_S* pstInstance;
	HI_S32 s32Ret;
    
    if(!pstTransFbList->bInit)
    {
        VPSS_FATAL("Trans fb not init.\n");        
        return HI_FAILURE;
    }

    pstInstance = (VPSS_INSTANCE_S *)(pstTransFbList->pstInstance);
    //DTV 4K@60,VPSS trans it to vdp
	s32Ret = pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE(pstInstance->ID,pstFrame);
	if (s32Ret != HI_SUCCESS)
	{
		VPSS_ERROR("Trans fb rls image failed.\n"); 
	}     
    return HI_SUCCESS;
}    
HI_S32 VPSS_TRANS_FB_ResetPort(VPSS_TRANS_FB_INFO_S *pstTransFbList,VPSS_FB_INFO_S *pstFrameList)
{
    unsigned long flags;    
    LIST *pos, *n;
    VPSS_FB_NODE_S *pstTarget;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = (VPSS_INSTANCE_S *)(pstTransFbList->pstInstance);    
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);

    for (pos = (pstFrameList->pstTarget_1)->next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList);
        pos = n, n = pos->next)
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        if(VPSS_TRANS_FB_NeedTrans(pstTransFbList,&(pstTarget->stOutFrame)))
        {
            pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE(pstInstance->ID,&(pstTarget->stOutFrame));
        }        
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);

    return HI_SUCCESS;
}

HI_S32 VPSS_TRANS_FB_Reset(VPSS_TRANS_FB_INFO_S *pstTransFbList)
{
    HI_U32 u32Count;    
    VPSS_PORT_S *pstPort;
    VPSS_INSTANCE_S* pstInstance;
    if(!pstTransFbList->bInit)
    {
        VPSS_FATAL("Trans fb not init.\n");        
        return HI_FAILURE;
    }
    pstInstance = (VPSS_INSTANCE_S *)(pstTransFbList->pstInstance);
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId != VPSS_INVALID_HANDLE)
        {
            VPSS_TRANS_FB_ResetPort(pstTransFbList,&(pstPort->stFrmInfo));           
        }
    }
    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
#endif
#endif /* __cplusplus */


