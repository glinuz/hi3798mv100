#include "vpss_src.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

static inline HI_BOOL VPSS_SRC_IsProgressive(VPSS_SRC_DATA_S *pstData)
{
	return pstData->bProgressive;
}

static inline HI_BOOL VPSS_SRC_IsFieldAll(VPSS_SRC_DATA_S *pstData)
{
	return (HI_DRV_FIELD_ALL == pstData->enFieldMode) ;
}

static inline HI_BOOL VPSS_SRC_IsTopFirst(VPSS_SRC_DATA_S *pstData)
{
	return pstData->bTopFieldFirst;
}

static inline HI_BOOL VPSS_SRC_IsFull(VPSS_SRC_S *pstSrc)
{
    return list_empty_careful(&(pstSrc->stEmptySrcList));
}

static inline HI_BOOL VPSS_SRC_IsEmpty(VPSS_SRC_S *pstSrc)
{
    return list_empty_careful(&(pstSrc->stFulSrcList));
}

static inline HI_BOOL VPSS_SRC_IsSomeSpaceLeft(VPSS_SRC_S *pstSrc,HI_U32 u32Numb)
{
    HI_U32 u32Cnt = 0;
    LIST *pos, *n;
    list_for_each_safe(pos, n, &(pstSrc->stEmptySrcList))
    {
        u32Cnt++;
    }
    return (u32Cnt >= u32Numb);
}
static inline HI_U32 VPSS_SRC_FulListLenth(VPSS_SRC_S *pstSrc)
{
    HI_U32 u32Cnt = 0;
    LIST *pos, *n;
    list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
    {
        u32Cnt++;
    }
    return u32Cnt;
}
static inline HI_BOOL VPSS_SRC_IsUndoDataLeft(VPSS_SRC_S *pstSrc)
{
    return (pstSrc->pstTarget_1->next != &(pstSrc->stFulSrcList));
}
static inline VPSS_SRC_DATA_S *VPSS_SRC_GetData(LIST *pstNodePtr)
{
    VPSS_SRC_NODE_S *pstNode;
    pstNode = list_entry(pstNodePtr, VPSS_SRC_NODE_S, node);
    return &(pstNode->stSrcData);
}
static inline HI_VOID VPSS_SRC_AddFulNode(VPSS_SRC_S *pstSrc,VPSS_SRC_NODE_S* pstDataNode)
{
    list_add_tail(&(pstDataNode->node), &(pstSrc->stFulSrcList));
}
static inline HI_VOID VPSS_SRC_AddEmptyNode(VPSS_SRC_S *pstSrc,VPSS_SRC_NODE_S* pstDataNode)
{
    list_add_tail(&(pstDataNode->node), &(pstSrc->stEmptySrcList));
}

static VPSS_SRC_NODE_S* VPSS_SRC_DelEmptyNode(VPSS_SRC_S *pstSrc)
{
    LIST *pos, *n;
    VPSS_SRC_NODE_S *pstTarget = HI_NULL;
    list_for_each_safe(pos, n, &(pstSrc->stEmptySrcList))
    {
        pstTarget = list_entry(pos, VPSS_SRC_NODE_S, node);
        list_del_init(pos);
        break;
    }
    
    if(pstTarget)
    {
        memset(&(pstTarget->stSrcData), 0, 
                sizeof(VPSS_SRC_DATA_S));
        return pstTarget;
    }
    else
    {
        return HI_NULL;
    }
}
#if 1
HI_S32 VPSS_SRC_RlsDoneFullNode(VPSS_SRC_S *pstSrc)
{
    LIST *pos, *n, *head, *ListNewFirstNode;
    VPSS_SRC_DATA_S *pstFstData;
    VPSS_SRC_DATA_S *pstSndData;
    VPSS_SRC_NODE_S *pstNode;
    
    head = &(pstSrc->stFulSrcList);

    if (pstSrc->enMode == SRC_MODE_FIELD
        || pstSrc->enMode == SRC_MODE_NTSC
        || pstSrc->enMode == SRC_MODE_PAL)
    {
        pos = (head)->next;
        n = pos->next;
        ListNewFirstNode = n->next;
        if (pos == head || n == head)
        {
            VPSS_ERROR("Can't get first two released nodes\n");
            return HI_FAILURE;
        }
        pstFstData = VPSS_SRC_GetData(pstSrc->stFulSrcList.next);
        pstSndData = VPSS_SRC_GetData(pstSrc->stFulSrcList.next->next);
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
#else
		if (pstFstData->u32FrameIndex != pstSndData->u32FrameIndex)
        {
            VPSS_ERROR("not same frame(%d), (%d)!\n", 
                pstFstData->u32FrameIndex, pstSndData->u32FrameIndex);
            return HI_FAILURE;
        }
#endif

        if(pstFstData->enFieldMode == HI_DRV_FIELD_TOP)
        {
			HI_DRV_VIDEO_PRIVATE_S *pstPriv;
			pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstFstData->u32Priv;
			if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstSrc->bEnding = HI_TRUE;	
				pstSrc->bEndingCompleteCnt = 2;
			}
            pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
			//hevc+interlace, top&bottom field are not in the same frame.
			if (pstSndData->enBufValidMode == HI_DRV_FIELD_ALL)
			{
				pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstSndData);
			}
#endif
        }
        else
        {
			HI_DRV_VIDEO_PRIVATE_S *pstPriv;
			pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstSndData->u32Priv;
			if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstSrc->bEnding = HI_TRUE;	
				pstSrc->bEndingCompleteCnt = 2;
			}
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
			//hevc+interlace, top&bottom field are not in the same frame.
			if (pstFstData->enBufValidMode == HI_DRV_FIELD_ALL)
			{
				pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
			}
#endif
            pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstSndData);
			
        }
    }
    else
	{
		pos = (head)->next;
		ListNewFirstNode = pos->next;
		if (pos == head)
		{
			VPSS_ERROR("Can't get first one released nodes\n");
			return HI_FAILURE;
		}
		pstFstData = VPSS_SRC_GetData(pstSrc->stFulSrcList.next);

		pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
	}
    
    for (pos = (head)->next, n = pos->next; 
         pos != ListNewFirstNode
         && pos != head; 
         pos = n, n = pos->next)
    {
        pstNode = list_entry(pos,
                        VPSS_SRC_NODE_S, node);
        if (pos == pstSrc->pstTarget_1)
        {
            pstSrc->pstTarget_1 = pstSrc->pstTarget_1->prev;
        }
        list_del_init(&(pstNode->node));

        VPSS_SRC_AddEmptyNode(pstSrc,pstNode);
    }
	
    pstSrc->u32ReleaseSrcCount++;   
    return HI_SUCCESS;
}
#endif
HI_S32 VPSS_SRC_Init(VPSS_SRC_S *pstSrc,VPSS_SRC_ATTR_S stAttr)
{
    HI_U32 u32Cnt = 0;
    VPSS_SRC_NODE_S *pstSrcNode;
    LIST *pos, *n;
    
    VPSS_CHECK_NULL(pstSrc);
    
    if (stAttr.enMode != SRC_MODE_FRAME
        && stAttr.enMode != SRC_MODE_FIELD
        && stAttr.enMode != SRC_MODE_NTSC
        && stAttr.enMode != SRC_MODE_PAL)
    {
        VPSS_ERROR("Para enMode %#x is Invalid.\n",stAttr.enMode);
        return HI_FAILURE;
    }
    
    VPSS_CHECK_NULL(stAttr.pfnRlsImage);

    if(pstSrc->bInit)
    {
        (HI_VOID)VPSS_SRC_DeInit(pstSrc);
    }
    
    memset(pstSrc,0,sizeof(VPSS_SRC_S));
    
    switch(stAttr.enMode)
    {
        case SRC_MODE_FRAME:
            pstSrc->u32ListLenth = DEF_SRC_MODE_FRAME_NUMB;  
            break;
        case SRC_MODE_FIELD:
            pstSrc->u32ListLenth = DEF_SRC_MODE_FIELD_NUMB;        
            break;
        case SRC_MODE_NTSC:
            pstSrc->u32ListLenth = DEF_SRC_MODE_NTSC_NUMB;          
            break;
        case SRC_MODE_PAL: 
            pstSrc->u32ListLenth = DEF_SRC_MODE_PAL_NUMB;          
            break;
        default:
            VPSS_ERROR("Para enMode %#x is Invalid.\n",stAttr.enMode);
            return HI_FAILURE;
    }

    pstSrc->enMode  = stAttr.enMode;
    
    INIT_LIST_HEAD(&(pstSrc->stEmptySrcList));
    INIT_LIST_HEAD(&(pstSrc->stFulSrcList));
    
    for (u32Cnt = 0; u32Cnt < pstSrc->u32ListLenth; u32Cnt++)
    {
        pstSrcNode = (VPSS_SRC_NODE_S*)VPSS_KMALLOC(sizeof(VPSS_SRC_NODE_S),GFP_ATOMIC);
        if (pstSrcNode == HI_NULL)
        {
            VPSS_ERROR("vmalloc SrcNode failed\n");
            goto SRC_Init_Failed;
        }

        memset(&(pstSrcNode->stSrcData), 0, 
                sizeof(VPSS_SRC_DATA_S));
        
        list_add_tail(&(pstSrcNode->node), 
                        &(pstSrc->stEmptySrcList));
    }

    pstSrc->pstTarget_1 = &(pstSrc->stFulSrcList);
    
    pstSrc->pfnRlsImage = stAttr.pfnRlsImage;
    pstSrc->hSrcModule = stAttr.hSrcModule;

    pstSrc->bInit = HI_TRUE;
	pstSrc->bEnding = HI_FALSE;
	pstSrc->bEndingCompleteCnt = 0;
    
    return HI_SUCCESS;
    
SRC_Init_Failed:
    list_for_each_safe(pos, n, &(pstSrc->stEmptySrcList))
    {
        pstSrcNode = list_entry(pos, VPSS_SRC_NODE_S, node);

        list_del_init(pos);
        VPSS_KFREE(pstSrcNode);
    }
    
    return HI_FAILURE;
}

HI_S32 VPSS_SRC_DeInit(VPSS_SRC_S* pstSrc)
{
    VPSS_SRC_NODE_S *pstFstNode;
    VPSS_SRC_NODE_S *pstSndNode;    
    VPSS_SRC_DATA_S *pstFstData;
    VPSS_SRC_DATA_S *pstSndData;    

    LIST *pos, *n,*tmp;
    
    VPSS_CHECK_NULL(pstSrc);

    if(!pstSrc->bInit)
    {
        VPSS_WARN("Delint.\n");      
        return HI_FAILURE;
    }   
    
    //release source buffer
    list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
    {
        if(SRC_MODE_FRAME == pstSrc->enMode)
        {
            pstFstNode = list_entry(pos, VPSS_SRC_NODE_S, node);
            pstFstData = VPSS_SRC_GetData(pos);        
            pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
            list_del_init(pos);     
            VPSS_SRC_AddEmptyNode(pstSrc,pstFstNode);
        }
        else
        {
            if (n == &(pstSrc->stFulSrcList))
            {
            pstFstNode = list_entry(pos, VPSS_SRC_NODE_S, node);
                pstFstData = VPSS_SRC_GetData(pos);
                pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
                list_del_init(pos);            
                VPSS_SRC_AddEmptyNode(pstSrc,pstFstNode);   
            }
            else
            {
                pstFstNode = list_entry(pos, VPSS_SRC_NODE_S, node);
            pstSndNode = list_entry(n, VPSS_SRC_NODE_S, node);
            pstFstData = VPSS_SRC_GetData(pos);
            pstSndData = VPSS_SRC_GetData(n);
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
            if (pstFstData->enBufValidMode != HI_DRV_FIELD_ALL)
            {
		        if (pstFstData->u32FrameIndex != pstSndData->u32FrameIndex)
		        {
		            VPSS_ERROR("not same frame(%d), (%d)!\n", 
		                pstFstData->u32FrameIndex, pstSndData->u32FrameIndex);
		            return HI_FAILURE;
		        }

		        if(pstFstData->enFieldMode == HI_DRV_FIELD_TOP)
		        {
		            pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
		        }
		        else
		        {
		            pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstSndData);
		        }
            }
            else
#endif
            {
                pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstFstData);
                pstSrc->pfnRlsImage(pstSrc->hSrcModule,pstSndData);
            }
            tmp = n->next;
            list_del_init(pos);            
            list_del_init(n);
            VPSS_SRC_AddEmptyNode(pstSrc,pstFstNode);            
            VPSS_SRC_AddEmptyNode(pstSrc,pstSndNode);             
            n = tmp;            
        }
        }
    }

    list_for_each_safe(pos, n, &(pstSrc->stEmptySrcList))
    {
        pstFstNode = list_entry(pos, VPSS_SRC_NODE_S, node);

        list_del_init(pos);

        VPSS_KFREE(pstFstNode);
    }

    memset(pstSrc,0,sizeof(VPSS_SRC_S));

    return HI_SUCCESS;
}
HI_S32 VPSS_SRC_Reset(VPSS_SRC_S* pstSrc)
{
    LIST *pos, *n;  
    VPSS_SRC_NODE_S *pstSrcNode;    
    VPSS_CHECK_NULL(pstSrc);
    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS src reset failed(not init).\n");
        return HI_FAILURE;
    }

    list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
    {
        pstSrcNode = list_entry(pos, VPSS_SRC_NODE_S, node);

        list_del_init(pos);

        VPSS_SRC_AddEmptyNode(pstSrc,pstSrcNode);
    }
    
    pstSrc->pstTarget_1 = &(pstSrc->stFulSrcList);
    pstSrc->u32PutSrcCount = 0;
    pstSrc->u32CompleteSrcCount = 0;
    pstSrc->u32ReleaseSrcCount = 0;
	pstSrc->bEnding = HI_FALSE;
	pstSrc->bEndingCompleteCnt = 0;
    return HI_SUCCESS;
}

#define DEF_SRC_FIELD_SPACE_NUMB 2
#define DEF_SRC_FRAME_SPACE_NUMB 1

HI_S32 VPSS_SRC_PutImage(VPSS_SRC_S *pstSrc,VPSS_SRC_DATA_S *pstData)
{
    HI_BOOL bProgressive;
    HI_BOOL bFieldAll;
    HI_BOOL bTopfirst;
    HI_BOOL bEnoughSpace;
    VPSS_SRC_NODE_S *pstDstNode;
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv; 
    HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv; 
    
    VPSS_CHECK_NULL(pstSrc);

    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS_SRC_PutImage Delint.\n");
        return HI_FAILURE;
    }

    pstSrc->u32PutSrcCount++;
    bProgressive = VPSS_SRC_IsProgressive(pstData);
    bFieldAll = VPSS_SRC_IsFieldAll(pstData);
    bTopfirst = VPSS_SRC_IsTopFirst(pstData);

    if (bProgressive)// && bFieldAll
    {
        bEnoughSpace = VPSS_SRC_IsSomeSpaceLeft(pstSrc,DEF_SRC_FRAME_SPACE_NUMB);
    }
    else
    {
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
		if (bFieldAll)
        {
            bEnoughSpace = VPSS_SRC_IsSomeSpaceLeft(pstSrc,DEF_SRC_FIELD_SPACE_NUMB);
        }
        else
        {
            bEnoughSpace = VPSS_SRC_IsSomeSpaceLeft(pstSrc,DEF_SRC_FRAME_SPACE_NUMB);
        }
#else
        bEnoughSpace = VPSS_SRC_IsSomeSpaceLeft(pstSrc,DEF_SRC_FIELD_SPACE_NUMB);
#endif
    }

    if (!bEnoughSpace)
    {
        VPSS_ERROR("there is not enough space.\n");
        return HI_FAILURE;
    }

    if (bProgressive && bFieldAll)
    {
        pstDstNode = VPSS_SRC_DelEmptyNode(pstSrc);
        if(HI_NULL == pstDstNode)
        {
            return HI_FAILURE;
        }        
        memcpy(&(pstDstNode->stSrcData),pstData,sizeof(VPSS_SRC_DATA_S));
        VPSS_SRC_AddFulNode(pstSrc,pstDstNode);
    }
    else if (!bProgressive && bFieldAll)
    {
        VPSS_SRC_NODE_S *pstFstDstNode;
        VPSS_SRC_NODE_S *pstSndDstNode;
        VPSS_SRC_NODE_S *pstReviseNode;
		HI_DRV_VIDEO_PRIVATE_S *pstFstPriv;
		HI_DRV_VIDEO_PRIVATE_S *pstSndPriv;
        HI_U32 u32BaseAddr;
        HI_U32 u32BaseStride;

        pstFstDstNode = VPSS_SRC_DelEmptyNode(pstSrc);
        if(HI_NULL == pstFstDstNode)
        {
            return HI_FAILURE;
        }   
        
        memcpy(&(pstFstDstNode->stSrcData),pstData,sizeof(VPSS_SRC_DATA_S));
        VPSS_SRC_AddFulNode(pstSrc,pstFstDstNode);

        pstSndDstNode = VPSS_SRC_DelEmptyNode(pstSrc);
        if(HI_NULL == pstSndDstNode)
        {
            return HI_FAILURE;
        }
        
        memcpy(&(pstSndDstNode->stSrcData),pstData,sizeof(VPSS_SRC_DATA_S));
        VPSS_SRC_AddFulNode(pstSrc,pstSndDstNode);
        
		pstFstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstFstDstNode->stSrcData.u32Priv;
		pstSndPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstSndDstNode->stSrcData.u32Priv;

        if (bTopfirst)
        {
            pstFstDstNode->stSrcData.bProgressive = HI_FALSE;
            pstFstDstNode->stSrcData.enFieldMode = HI_DRV_FIELD_TOP;
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420) ||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
            pstFstDstNode->stSrcData.enBufValidMode = HI_DRV_FIELD_TOP;
#endif
            pstSndDstNode->stSrcData.bProgressive = HI_FALSE;
            pstSndDstNode->stSrcData.enFieldMode = HI_DRV_FIELD_BOTTOM;
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
            pstSndDstNode->stSrcData.enBufValidMode = HI_DRV_FIELD_BOTTOM;
#endif
            /*Revise field addr*/
            pstReviseNode = pstSndDstNode;

			if (pstFstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG
					&& pstSndPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstFstPriv->u32LastFlag = 0x0;
			}
        }
        else
        {
            pstFstDstNode->stSrcData.bProgressive = HI_FALSE;
            pstFstDstNode->stSrcData.enFieldMode = HI_DRV_FIELD_BOTTOM;
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420) ||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
            pstFstDstNode->stSrcData.enBufValidMode = HI_DRV_FIELD_BOTTOM;
#endif
            pstSndDstNode->stSrcData.bProgressive = HI_FALSE;
            pstSndDstNode->stSrcData.enFieldMode = HI_DRV_FIELD_TOP;			
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420) ||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
            pstSndDstNode->stSrcData.enBufValidMode = HI_DRV_FIELD_TOP;
#endif
            /*Revise field addr*/
            pstReviseNode = pstFstDstNode;

			if (pstFstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG
					&& pstSndPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstFstPriv->u32LastFlag = 0x0;				
			}
        }

        pstFstDstNode->stSrcData.u32FrameRate *= 2;
        pstSndDstNode->stSrcData.u32FrameRate *= 2;
        pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFstDstNode->stSrcData.u32Priv[0]);
        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);        
        pstSndDstNode->stSrcData.u32Pts = pstFstDstNode->stSrcData.u32Pts + pstVdecPriv->s32InterPtsDelta; 
            
        //Is available?
        //:TODO:后面处理3D隔行的情况/宽度小于256时的情况暂时没有处理
        switch (pstData->ePixFormat)
        {
            case HI_DRV_PIX_FMT_NV12_TILE:
            case HI_DRV_PIX_FMT_NV21_TILE:
            case HI_DRV_PIX_FMT_NV12_TILE_CMP:
            case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            {
                u32BaseAddr = pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_Y;
                u32BaseStride = 256;//pstReviseNode->stSrcData.stBufAddr[0].u32Stride_Y;
                
                pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_Y =
                                    u32BaseAddr + u32BaseStride;
                                    
                u32BaseAddr = pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_C;
                u32BaseStride = 256;//pstReviseNode->stSrcData.stBufAddr[0].u32Stride_C;
                
                pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_C =
                                    u32BaseAddr + u32BaseStride;
                break;
            }

            default:
            {
                u32BaseAddr = pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_Y;
                u32BaseStride =  pstReviseNode->stSrcData.stBufAddr[0].u32Stride_Y;
                
                pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_Y =
                                    u32BaseAddr + u32BaseStride;
                                    
                u32BaseAddr = pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_C;
                u32BaseStride =  pstReviseNode->stSrcData.stBufAddr[0].u32Stride_C;
                
                pstReviseNode->stSrcData.stBufAddr[0].u32PhyAddr_C =
                                    u32BaseAddr + u32BaseStride;
            }
        }
    }
    else //(!bProgressive && !bFieldAll)
    {
    
		HI_DRV_VIDEO_PRIVATE_S *pPriInfo;

		VPSS_DBG_INFO("(!bProgressive && !bFieldAll) \n");
    
        pstDstNode = VPSS_SRC_DelEmptyNode(pstSrc);
        if(HI_NULL == pstDstNode)
        {
            return HI_FAILURE;
        }        
        memcpy(&(pstDstNode->stSrcData),pstData,sizeof(VPSS_SRC_DATA_S));

        pPriInfo = (HI_DRV_VIDEO_PRIVATE_S *)&(pstDstNode->stSrcData.u32Priv[0]);
        VPSS_SRC_AddFulNode(pstSrc,pstDstNode);
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_SRC_GetProcessImage(VPSS_SRC_S *pstSrc,VPSS_SRC_DATA_S **pstData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_CHECK_NULL(pstSrc);
    
    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS_SRC_GetProcessImage Delint.\n");   
        return HI_FAILURE;
    }
    
    if (VPSS_SRC_IsUndoDataLeft(pstSrc))
    {
        *pstData = VPSS_SRC_GetData(pstSrc->pstTarget_1->next);
        s32Ret = HI_SUCCESS;
    }
    else
    {
        *pstData = HI_NULL;
        s32Ret = HI_FAILURE;
    }
      
    return s32Ret;
}

HI_S32 VPSS_SRC_GetPreImgInfo(VPSS_SRC_S *pstSrc,
                               VPSS_SRC_DATA_S **pPtrPreData,
                               VPSS_SRC_DATA_S **pPtrPpreData
                               )
{
    HI_U32 u32Cnt = 0;
    LIST *pos, *n;
    VPSS_SRC_DATA_S *pstData;
    VPSS_SRC_DATA_S *pstDataArray[SRC_MODE_PAL];
    VPSS_SRC_DATA_S *pstCurData;
	VPSS_SRC_MODE_E enMode;
	
    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS_SRC_GetPreImgInfo Delint.\n");
        return HI_FAILURE;
    }

	enMode = pstSrc->enMode;
	if((SRC_MODE_NTSC != enMode) && (SRC_MODE_PAL != enMode))
	{
        *pPtrPreData = HI_NULL;
        *pPtrPpreData = HI_NULL;
		return HI_SUCCESS;
	}
	
    if (pstSrc->pstTarget_1->next != &(pstSrc->stFulSrcList))
    {
        pstCurData = VPSS_SRC_GetData(pstSrc->pstTarget_1->next);
        list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
        {
            pstData = VPSS_SRC_GetData(pos);
            pstDataArray[u32Cnt] = pstData;
            u32Cnt++;			
            if (pstData == pstCurData)
                break;     
        }
    }
    else
    {
        *pPtrPreData = HI_NULL;
        *pPtrPpreData = HI_NULL;
		return HI_FAILURE;
    }

	if(SRC_MODE_NTSC == enMode)
	{
		if(5 <= u32Cnt)
		{
			*pPtrPreData = pstDataArray[u32Cnt-1-2];	
			*pPtrPpreData = pstDataArray[u32Cnt-1-4];			
		}
		else
		{
			*pPtrPreData = HI_NULL;
			*pPtrPpreData = HI_NULL;
		}
	}
	else
	{
		if(9 <= u32Cnt)
		{
			*pPtrPreData = pstDataArray[u32Cnt-1-4];	
			*pPtrPpreData = pstDataArray[u32Cnt-1-8];			
		}
		else
		{
			*pPtrPreData = HI_NULL;
			*pPtrPpreData = HI_NULL;
		}		
	}
    
    
    return HI_SUCCESS;
}      

HI_S32 VPSS_SRC_MoveNext(VPSS_SRC_S* pstSrc,HI_U32 u32NextCount)
{
    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS_SRC_GetPreImgInfo Delint.\n");
        return HI_FAILURE;
    }

    if((&(pstSrc->stFulSrcList) == pstSrc->pstTarget_1->next)
        || (&(pstSrc->stFulSrcList) == pstSrc->pstTarget_1->next->next))
    {
        VPSS_INFO("VPSS_SRC_MoveNext error.\n");
        return HI_FAILURE;
    }
    
    pstSrc->pstTarget_1 = pstSrc->pstTarget_1->next;

    return HI_SUCCESS;

}


HI_S32 VPSS_SRC_CompleteImage(VPSS_SRC_S* pstSrc,HI_VOID *arg)
{   
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstSrc);
    VPSS_CHECK_NULL(pstSrc->pfnRlsImage);
    
    if(!pstSrc->bInit)
    {
        VPSS_INFO("VPSS_SRC_CompleteImage Delint.\n");
        return HI_FAILURE;
    }

	if (pstSrc->bEnding == HI_TRUE)
	{
		pstSrc->bEndingCompleteCnt--;

		if (pstSrc->bEndingCompleteCnt == 0)
		{
			pstSrc->pstTarget_1 = pstSrc->pstTarget_1->next;
			pstSrc->bEnding = HI_FALSE;	
		}

		if (!VPSS_SRC_IsEmpty(pstSrc))
		{
			pstSrc->u32CompleteSrcCount++;
			s32Ret = VPSS_SRC_RlsDoneFullNode(pstSrc);
		}
		return HI_SUCCESS;	
	}
    
    if (VPSS_SRC_IsEmpty(pstSrc))
    {
        VPSS_ERROR("Srclist is Empty\n");
        return HI_FAILURE;
    }

    pstSrc->u32CompleteSrcCount++;
    pstSrc->pstTarget_1 = pstSrc->pstTarget_1->next;
    
    if (!VPSS_SRC_IsUndoDataLeft(pstSrc))
    {
        if (VPSS_SRC_IsFull(pstSrc))
        {
            s32Ret = VPSS_SRC_RlsDoneFullNode(pstSrc);
        }
    }

    if (HI_FAILURE == s32Ret)
    {
        VPSS_ERROR("CompleteImage Failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_SRC_CorrectListOrder(VPSS_SRC_S *pstImgInfo, HI_BOOL bTopFirst)
{
    LIST *pNode;
    VPSS_SRC_S *pstImageList;
    VPSS_SRC_NODE_S *pstPreFieldNode;
    VPSS_SRC_NODE_S *pstCurFieldNode;
    LIST *pPreNode;
    LIST *pNextNode;
    LIST *pos, *n;
    
    pstImageList = pstImgInfo;
    
    /*bottom first  -> top first */
    /*
	 *   B T  B T B T
	 *     | 
	 *   T B (T B T B)
	 *
      */
    if(bTopFirst == HI_TRUE)
    {
        pNode = pstImageList->pstTarget_1->next;

		pstCurFieldNode = list_entry(pNode, VPSS_SRC_NODE_S, node);
            
        if(pstCurFieldNode->stSrcData.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
    }
    /*top first -> bottom first */
    else
    {
        pNode = pstImageList->pstTarget_1->next;
            
        pstCurFieldNode = list_entry(pNode,VPSS_SRC_NODE_S, node);
        if(pstCurFieldNode->stSrcData.enFieldMode == HI_DRV_FIELD_TOP)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
    }

    for (pos = (pstImageList->stFulSrcList).next, n = pos->next; 
            pos != &(pstImageList->stFulSrcList);
            pos = pos->next, n = pos->next)
    {
        pPreNode = pos->prev;
        pNextNode = n->next;
        
        pstCurFieldNode = list_entry(n,VPSS_SRC_NODE_S, node);
        pstPreFieldNode = list_entry(pos,VPSS_SRC_NODE_S, node);
        
        if(pstCurFieldNode->stSrcData.u32FrameIndex
           != pstPreFieldNode->stSrcData.u32FrameIndex)
        {
            VPSS_FATAL("Field Order Detect Error\n");
        }
        
        if(pstImageList->pstTarget_1 == n)
        {
            VPSS_FATAL("Field Order Detect Error\n");
        }

         /*ori Pts:0 20 40 60 80 100
           after correct:20 0 60 40 100 80
           so we need reviese PTS too.
          */
        if (pstImageList->pstTarget_1 != pos)
        {
            HI_U32 u32Tmp;
            u32Tmp = pstCurFieldNode->stSrcData.u32Pts;
            pstCurFieldNode->stSrcData.u32Pts = 
                    pstPreFieldNode->stSrcData.u32Pts;
            pstPreFieldNode->stSrcData.u32Pts = u32Tmp;
        }
        
        if(pstImageList->pstTarget_1 == pos)
        {
            pstImageList->pstTarget_1 = pos->next;
        }
        
        list_del_init(pos);
        list_del_init(n);

        n->next = pos;
        n->prev = pPreNode;
        pos->next = pNextNode;
        pos->prev = n;

        pPreNode->next = n;
        pNextNode->prev = pos;
    }
    
    return HI_SUCCESS;
} 

HI_S32 VPSS_SRCIN_Init(VPSS_SRCIN_S *pSrcIn)
{
    HI_U32 i;

    VPSS_CHECK_NULL(pSrcIn);
    
    if(pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been Inited\n");
        return HI_SUCCESS;
    }

    VPSS_OSAL_InitSpin(&pSrcIn->stSrcInLock);
    
    INIT_LIST_HEAD(&pSrcIn->stBusyList);
    INIT_LIST_HEAD(&pSrcIn->stFreeList);
    INIT_LIST_HEAD(&pSrcIn->stReleaseList);

    /* 空闲buffer初始化时空闲buffer全部插入到free队列 */
    for (i = 0; i < SRCIN_NODE_NUM; i++)
    {
        list_add_tail(&pSrcIn->astSrcNode[i].node, &pSrcIn->stFreeList);
    }

    pSrcIn->bInit = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 VPSS_SRCIN_DeInit(VPSS_SRCIN_S *pSrcIn)
{
    VPSS_CHECK_NULL(pSrcIn);
    
    if(!pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been DeInited\n");
        return HI_SUCCESS;
    }

    INIT_LIST_HEAD(&pSrcIn->stBusyList);
    INIT_LIST_HEAD(&pSrcIn->stFreeList);
    INIT_LIST_HEAD(&pSrcIn->stReleaseList);

    pSrcIn->bInit = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 VPSS_SRCIN_SendImage(VPSS_SRCIN_S *pSrcIn, VPSS_SRC_DATA_S *pstData)
{
    unsigned long u32Flag;
    VPSS_SRC_NODE_S *pstDateTmp;
    LIST *pstListTmp;

    VPSS_CHECK_NULL(pSrcIn);
    VPSS_CHECK_NULL(pstData);

    if(!pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been DeInited\n");
        return HI_FAILURE;
    }

    VPSS_OSAL_DownSpin(&pSrcIn->stSrcInLock, &u32Flag);

    if(HI_TRUE == list_empty(&pSrcIn->stFreeList))
    {
        VPSS_ERROR("SRCIN No Node to Fill\n");
        VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);
        return HI_FAILURE;
    }

	pstListTmp = pSrcIn->stFreeList.next;
	list_del(pstListTmp);
	pstDateTmp = (VPSS_SRC_NODE_S *)list_entry(pstListTmp, VPSS_SRC_NODE_S, node);
    memcpy(&pstDateTmp->stSrcData, pstData, sizeof(VPSS_SRC_DATA_S));
    list_add_tail(&pstDateTmp->node, &pSrcIn->stBusyList);

    VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);

    return HI_SUCCESS;
    
}

HI_S32 VPSS_SRCIN_CallImage(VPSS_SRCIN_S *pSrcIn, VPSS_SRC_DATA_S *pstData)
{
    unsigned long u32Flag;
    VPSS_SRC_NODE_S *pstDateTmp;
    LIST *pstListTmp;

    VPSS_CHECK_NULL(pSrcIn);
    VPSS_CHECK_NULL(pstData);

    if(!pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been DeInited\n");
        return HI_FAILURE;
    }

    VPSS_OSAL_DownSpin(&pSrcIn->stSrcInLock, &u32Flag);

    if(HI_TRUE == list_empty(&pSrcIn->stReleaseList))
    {
        VPSS_INFO("SRCIN No Node to Call\n");
        VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);
        return HI_FAILURE;
    }

    pstListTmp = pSrcIn->stReleaseList.next;
	list_del(pstListTmp);
	pstDateTmp = (VPSS_SRC_NODE_S *)list_entry(pstListTmp, VPSS_SRC_NODE_S, node);
    memcpy(pstData, &pstDateTmp->stSrcData, sizeof(VPSS_SRC_DATA_S));
    list_add_tail(&pstDateTmp->node, &pSrcIn->stFreeList);

    VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);

    return HI_SUCCESS;
}

HI_S32 VPSS_SRCIN_GetImage(VPSS_SRCIN_S *pSrcIn, VPSS_SRC_DATA_S *pstData)
{
    unsigned long u32Flag;
    VPSS_SRC_NODE_S *pstDateTmp;
    LIST *pstListTmp;

    VPSS_CHECK_NULL(pSrcIn);
    VPSS_CHECK_NULL(pstData);

    if(!pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been DeInited\n");
        return HI_FAILURE;
    }

    VPSS_OSAL_DownSpin(&pSrcIn->stSrcInLock, &u32Flag);

    if(HI_TRUE == list_empty(&pSrcIn->stBusyList))
    {
        VPSS_INFO("SRCIN No Node to Get\n");
        VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);
        return HI_FAILURE;
    }

    pstListTmp = pSrcIn->stBusyList.next;
    list_del(pstListTmp);
    pstDateTmp = (VPSS_SRC_NODE_S *)list_entry(pstListTmp, VPSS_SRC_NODE_S, node);
    memcpy(pstData, &pstDateTmp->stSrcData, sizeof(VPSS_SRC_DATA_S));
    //list_add_tail(&pstDateTmp->node, &pSrcIn->stFreeList);
    pSrcIn->pstNodeUsed = pstDateTmp;

    VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);

    return HI_SUCCESS;
}

HI_S32 VPSS_SRCIN_RelImage(VPSS_SRCIN_S *pSrcIn, VPSS_SRC_DATA_S *pstData)
{
    unsigned long u32Flag;
    VPSS_SRC_NODE_S *pstDateTmp;

    VPSS_CHECK_NULL(pSrcIn);
    VPSS_CHECK_NULL(pstData);
    VPSS_CHECK_NULL(pSrcIn->pstNodeUsed);

    if(!pSrcIn->bInit)
    {
        VPSS_INFO("SRCIN has been DeInited\n");
        return HI_FAILURE;
    }

    VPSS_OSAL_DownSpin(&pSrcIn->stSrcInLock, &u32Flag);

    pstDateTmp = pSrcIn->pstNodeUsed;

    if(pstDateTmp->stSrcData.u32FrameIndex != pstData->u32FrameIndex)
    {
        VPSS_ERROR("this Node is invaild\n");
        VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);
        return HI_FAILURE;
    }

    list_add_tail(&pstDateTmp->node, &pSrcIn->stReleaseList);

    VPSS_OSAL_UpSpin(&pSrcIn->stSrcInLock, &u32Flag);

    return HI_SUCCESS;
}


HI_S32 VPSS_SRC_Flush(VPSS_SRC_S* pstSrc,HI_BOOL bProgressive)
{
	if (pstSrc->bInit)
	{
		if (!VPSS_SRC_IsEmpty(pstSrc))
		{
			if (bProgressive == HI_TRUE)
			{
				//		pstSrc->bEnding = HI_TRUE;	
				//		pstSrc->bEndingCompleteCnt = 1;
			}
			else
			{
				pstSrc->bEnding = HI_TRUE;	
				pstSrc->bEndingCompleteCnt = 2;
			}
		}
	}

	return HI_SUCCESS;
}

HI_S32 VPSS_SRC_GetLastImageInfo(VPSS_SRC_S* pstSrc,HI_DRV_VIDEO_FRAME_S** ppstLastImage)
{
	HI_U32 u32Cnt = 0;
	LIST *pos, *n;
	VPSS_SRC_DATA_S *pstData;
	VPSS_SRC_DATA_S *pstDataArray[SRC_MODE_PAL]={0};
	VPSS_SRC_DATA_S *pstCurData;
	VPSS_SRC_MODE_E enMode;

	if(!pstSrc->bInit)
	{
		VPSS_INFO("VPSS_SRC_GetPreImgInfo Delint.\n");
		return HI_FAILURE;
	}

	enMode = pstSrc->enMode;

	if(SRC_MODE_NTSC != enMode)
	{
		*ppstLastImage = HI_NULL;
		return HI_SUCCESS;
	}

	if (pstSrc->pstTarget_1 != &(pstSrc->stFulSrcList))
	{
		pstCurData = VPSS_SRC_GetData(pstSrc->pstTarget_1);
		list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
		{
			pstData = VPSS_SRC_GetData(pos);
			pstDataArray[u32Cnt] = pstData;
			u32Cnt++;			
			if (pstData == pstCurData)
				break;     
		}
	}
	else
	{
		*ppstLastImage = HI_NULL;
		VPSS_ERROR("Can't Get Last Image \n");
		return HI_FAILURE;
	}

		//VPSS_ERROR("cnt %d\n",u32Cnt);
	if(SRC_MODE_NTSC == enMode)
	{
		switch(u32Cnt)
		{
			case 0:
				*ppstLastImage = pstCurData;
				break;
			case 1:
				*ppstLastImage = pstDataArray[u32Cnt - 1];
				break;
			case 2:
				*ppstLastImage = pstDataArray[u32Cnt - 2];
				break;
			case 3:
				*ppstLastImage = pstDataArray[u32Cnt - 3];
				break;
			default:
				*ppstLastImage = pstDataArray[u32Cnt - 4];
				break;
		}
	}
	else
	{
		VPSS_ERROR("Can't Get Last Image \n");
		*ppstLastImage = HI_NULL;
		return HI_FAILURE;
	}


	return HI_SUCCESS;
}

HI_S32 VPSS_SRC_GetRefInfo(VPSS_SRC_S* pstSrc,HI_DRV_VIDEO_FRAME_S** pstDataList)
{
	HI_U32 u32Cnt = 0;
	LIST *pos, *n;
	VPSS_SRC_DATA_S *pstData;
	VPSS_SRC_DATA_S *pstDataArray[SRC_MODE_PAL]={0};
	VPSS_SRC_DATA_S *pstCurData;
	VPSS_SRC_MODE_E enMode;

	if(!pstSrc->bInit)
	{
		VPSS_INFO("VPSS_SRC_GetPreImgInfo Delint.\n");
		return HI_FAILURE;
	}

	enMode = pstSrc->enMode;
	
	if(SRC_MODE_NTSC != enMode
		&& SRC_MODE_FRAME != enMode)
	{
		pstDataList[0] = HI_NULL;
		pstDataList[1] = HI_NULL;
		pstDataList[2] = HI_NULL;
		return HI_SUCCESS;
	}
		
	if (SRC_MODE_FRAME == enMode)
	{
		if (pstSrc->pstTarget_1->next != &(pstSrc->stFulSrcList))
		{
			pstCurData = VPSS_SRC_GetData(pstSrc->pstTarget_1->next);
			list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
			{
				pstData = VPSS_SRC_GetData(pos);
				pstDataArray[u32Cnt] = pstData;
				u32Cnt++;			
				if (pstData == pstCurData)
					break;     
			}

			pstDataList[0] = pstCurData;
			pstDataList[1] = pstCurData;
			pstDataList[2] = pstCurData;

			return HI_SUCCESS;
		}
		else
		{
			VPSS_ERROR("get ref failed\n");
			return HI_FAILURE;
		}
	}
	
	if (pstSrc->pstTarget_1->next != &(pstSrc->stFulSrcList))
	{
		pstCurData = VPSS_SRC_GetData(pstSrc->pstTarget_1->next);
		list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
		{
			pstData = VPSS_SRC_GetData(pos);
			pstDataArray[u32Cnt] = pstData;
			u32Cnt++;			
			if (pstData == pstCurData)
				break;     
		}
	}
	else
	{
		if (pstSrc->pstTarget_1 != &(pstSrc->stFulSrcList))
		{
			pstCurData = VPSS_SRC_GetData(pstSrc->pstTarget_1);

			list_for_each_safe(pos, n, &(pstSrc->stFulSrcList))
			{
				pstData = VPSS_SRC_GetData(pos);
				pstDataArray[u32Cnt] = pstData;
				u32Cnt++;			
				if (pstData == pstCurData)
					break;     
			}
		}
		else
		{
			VPSS_ERROR("get ref failed mode=%d\n",enMode);
			return HI_FAILURE;
		}
	}

	if(SRC_MODE_NTSC == enMode
			|| SRC_MODE_FRAME == enMode)
	{
		switch(u32Cnt)
			{
				case 0:
				case 1:
					pstDataList[0] = pstCurData;
					pstDataList[1] = pstCurData;
					pstDataList[2] = pstCurData;
					break;
				case 2:
					pstDataList[0] = pstDataArray[0];
					pstDataList[1] = pstDataArray[0];
					pstDataList[2] = pstDataArray[1];
					break;
				case 3:
					pstDataList[0] = pstDataArray[0];
					pstDataList[1] = pstDataArray[1];
					pstDataList[2] = pstDataArray[2];
					break;
				default:
					pstDataList[0] = pstDataArray[u32Cnt-4];	
					pstDataList[1] = pstDataArray[u32Cnt-3];
					pstDataList[2] = pstDataArray[u32Cnt-2];
					break;
			}
	}
	else
	{
			VPSS_ERROR("get ref failed mode=%d\n",enMode);
			return HI_FAILURE;
	}
	return HI_SUCCESS;
}

HI_BOOL VPSS_SRC_CheckIsEmpty(VPSS_SRC_S* pstSrc)
{
	return VPSS_SRC_IsEmpty(pstSrc);
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
