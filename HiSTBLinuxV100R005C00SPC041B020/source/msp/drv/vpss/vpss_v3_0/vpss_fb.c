#include "vpss_fb.h"
#include "hi_drv_stat.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
HI_S32 VPSS_FB_Init(VPSS_FB_INFO_S *pstFrameList,
                HI_DRV_VPSS_BUFLIST_CFG_S *pstBufListCfg)
{
    HI_U32 u32Count;
    HI_S32 s32Ret;
    VPSS_FB_NODE_S* pstNode;
    VPSS_BUFFER_S* pstBuf;
    VPSS_MEM_S* pstMemBuf;
    MMZ_BUFFER_S* pstPrivDataBuf;
    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg;

    pstBufCfg = &( pstFrameList->stBufListCfg );

    memcpy( pstBufCfg, pstBufListCfg,
            sizeof( HI_DRV_VPSS_BUFLIST_CFG_S ) );

    INIT_LIST_HEAD( &( pstFrameList->stEmptyFrmList ) );
    INIT_LIST_HEAD( &( pstFrameList->stFulFrmList ) );
    INIT_LIST_HEAD( &( pstFrameList->stExtFrmList ) );


    VPSS_OSAL_InitSpin(&(pstFrameList->stFulBufSpin ));
    VPSS_OSAL_InitSpin(&(pstFrameList->stEmptyBufSpin));
    VPSS_OSAL_InitSpin(&(pstFrameList->stExtBufSpin));

    pstFrameList->u32ExtCnt = 0;
    pstFrameList->u32ExtNumb = 0;
    
    pstFrameList->u32GetTotal = 0;
    pstFrameList->u32GetSuccess = 0;
    pstFrameList->u32RelTotal = 0;
    pstFrameList->u32RelSuccess= 0;
    pstFrameList->u32ListFul = 0;
    pstFrameList->ulStart = jiffies;
    pstFrameList->u32GetHZ = 0;
    pstFrameList->u32GetLast = 0;
    
    for ( u32Count = 0; u32Count < pstBufListCfg->u32BufNumber; u32Count ++ )
    {
        pstNode = ( VPSS_FB_NODE_S* )VPSS_VMALLOC( sizeof( VPSS_FB_NODE_S ));
        if (pstNode != HI_NULL)
        {
            memset( &( pstNode->stOutFrame ), 0, sizeof( HI_DRV_VIDEO_FRAME_S ) );
			pstBuf = &( pstNode->stBuffer );

			pstBuf->u32Stride = 0;

			pstMemBuf = &( pstBuf->stMemBuf );
			pstPrivDataBuf = &(pstBuf->stPrivDataBuf);

			switch(pstBufCfg->eBufType)
			{
				case HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE:
                    pstMemBuf->u32StartPhyAddr = 0;
                    pstMemBuf->pu8StartVirAddr = 0;
                    pstMemBuf->u32Size = 0;
                    pstMemBuf->u8flag = VPSS_MEM_FLAG_NORMAL;
                    pstPrivDataBuf->u32StartPhyAddr = 0;

					pstPrivDataBuf->pu8StartVirAddr = 0;
					pstPrivDataBuf->u32Size = 0;
					s32Ret = HI_SUCCESS;
                    break;
                case HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE:
                    pstMemBuf->u32StartPhyAddr = 0;
                    pstMemBuf->pu8StartVirAddr = 0;
					pstMemBuf->u32Size = pstBufListCfg->u32BufSize;
                    pstMemBuf->u8flag = VPSS_MEM_FLAG_NORMAL;
                    pstPrivDataBuf->u32StartPhyAddr = 0;

					pstPrivDataBuf->pu8StartVirAddr = 0;

                    pstPrivDataBuf->u32Size = 0;
                    s32Ret = HI_SUCCESS;
                    break;
                case HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE:
                    VPSS_FATAL("Buffer Type HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE can't support now.\n");                   
                    s32Ret = HI_FAILURE;
                    #if 0
                    pstMMZBuf->u32StartPhyAddr = pstBufListCfg->u32BufPhyAddr[u32Count];
                    pstMMZBuf->u32StartVirAddr = 0;
                    pstMMZBuf->u32Size = pstBufListCfg->u32BufSize;
                    s32Ret = HI_SUCCESS;
                    #endif
                    break;
                default:
                    VPSS_FATAL("Invalid Buffer Type.\n");
                    s32Ret = HI_FAILURE;
                    break;
			}
        }
		else
		{
			VPSS_FATAL("Vpss Fb Vmalloc Fail\n");
			s32Ret = HI_FAILURE;
		}
        
        if ( HI_SUCCESS != s32Ret )
        {
			goto FB_Init_Failed;
        }                   

        list_add_tail( &( pstNode->node ), &( pstFrameList->stEmptyFrmList ) );

    }

    pstFrameList->pstTarget_1 = &( pstFrameList->stFulFrmList );
    return HI_SUCCESS;

FB_Init_Failed:
    if (pstNode != HI_NULL)
	{
		VPSS_VFREE(pstNode);
	}
	VPSS_FATAL( "Vpss Alloc Buffer failed,already alloc %d total %d.\n",
                u32Count,pstBufListCfg->u32BufNumber);
    VPSS_FB_DelInit(pstFrameList);
    return HI_FAILURE;    

}
HI_S32 VPSS_FB_DelInit(VPSS_FB_INFO_S *pstFrameList)
{
    HI_DRV_VPSS_BUFLIST_CFG_S *pstBufCfg;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    HI_U32 u32DelCount;
    VPSS_MEM_S *pstMemBuf;
    pstBufCfg = &(pstFrameList->stBufListCfg);

    u32DelCount = 0;
    
    //VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
        
		if (pstBufCfg->eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
		{    
			if (pstMemBuf->u32StartPhyAddr != 0 || pstMemBuf->pu8StartVirAddr!= 0)
			{
				VPSS_OSAL_FreeMem(pstMemBuf);
			}
		}
        list_del_init(pos);

        VPSS_VFREE(pstTarget);
        u32DelCount++;
    }
    //VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin));

    
    //VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stFulFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
        if (pstBufCfg->eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
		{    
			if (pstMemBuf->u32StartPhyAddr != 0 || pstMemBuf->pu8StartVirAddr!= 0)
			{
				VPSS_OSAL_FreeMem(pstMemBuf);
			}
		}

        list_del_init(pos);

        VPSS_VFREE(pstTarget);
        u32DelCount++;
    }
    //VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));

    
    //VPSS_OSAL_DownSpin(&(pstFrameList->stExtBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stExtFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
        if (pstBufCfg->eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
		{    
			if (pstMemBuf->u32StartPhyAddr != 0 || pstMemBuf->pu8StartVirAddr!= 0)
			{
				VPSS_OSAL_FreeMem(pstMemBuf);
			}
		}

        list_del_init(pos);

        VPSS_VFREE(pstTarget);
        u32DelCount++;
    }
    //VPSS_OSAL_UpSpin(&(pstFrameList->stExtBufSpin));
    
    if (u32DelCount != pstBufCfg->u32BufNumber + pstFrameList->u32ExtCnt)
    {
        VPSS_FATAL("Vpss buffer destory error already delete %d total %d+%d(%d).\n",
                    u32DelCount,
                    pstBufCfg->u32BufNumber,
                    pstFrameList->u32ExtNumb,
                    pstFrameList->u32ExtCnt);
    }

    return HI_SUCCESS;
}
//消费者INSTANCE
HI_S32 VPSS_FB_GetFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile)
{
    VPSS_FB_NODE_S *pstFrmNode;
    HI_DRV_VIDEO_FRAME_S *pstFrm;
    LIST *pstNextNode;
    VPSS_FB_NODE_S *pstLeftNode;
    VPSS_FB_NODE_S *pstRightNode;
    unsigned long flags;
    HI_S32 s32Ret = HI_FAILURE;

    
    pstFrameList->u32GetTotal++;
    
    if(jiffies - pstFrameList->ulStart >= HZ)
    {
        pstFrameList->ulStart = jiffies;
        pstFrameList->u32GetHZ = pstFrameList->u32GetTotal - pstFrameList->u32GetLast;
        pstFrameList->u32GetLast = pstFrameList->u32GetTotal;
    }
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    
    pstNextNode = (pstFrameList->pstTarget_1)->next;
    
    if(pstNextNode != &(pstFrameList->stFulFrmList))
    {
        pstFrmNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);

        pstFrm = &(pstFrmNode->stOutFrame);
        
        if(pstFrm->eFrmType == HI_DRV_FT_NOT_STEREO)
        {
            memcpy(pstFrame,pstFrm,sizeof(HI_DRV_VIDEO_FRAME_S));
            
            pstFrameList->pstTarget_1 =  pstNextNode;
            pstFrameList->u32GetSuccess++;
			s32Ret = HI_SUCCESS;
        }
        else
        {
			if(pstNextNode->next != &(pstFrameList->stFulFrmList))
            {
				pstLeftNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);
				pstNextNode = pstNextNode->next;
				pstRightNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);
				pstFrmNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);

				pstFrm = &(pstFrmNode->stOutFrame);
            
				if (pstLeftNode->stOutFrame.u32FrameIndex 
					!= pstRightNode->stOutFrame.u32FrameIndex )
				{
					VPSS_FATAL("GetFulFrmBuf 3D Error.\n");
				}

                /* 将左眼的输出地址拷贝到右眼 */
                memcpy(&pstFrm->stBufAddr[0], &(pstLeftNode->stOutFrame.stBufAddr[0]), sizeof(HI_DRV_VID_FRAME_ADDR_S));
            
				memcpy(pstFrame,pstFrm,sizeof(HI_DRV_VIDEO_FRAME_S));
        
				pstFrameList->pstTarget_1 =  pstNextNode;
				s32Ret = HI_SUCCESS;
				pstFrameList->u32GetSuccess++;
            }
            else
            {
                s32Ret = HI_FAILURE;
            }
        }
    }
    else
    {
        s32Ret = HI_FAILURE;
    }
    
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);

    return s32Ret;
}

HI_S32 VPSS_FB_RelFulFrmBufNoMmz(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{

    HI_U32 u32RelCount = 0;    
    VPSS_FB_NODE_S *pstTarget = HI_NULL;
    LIST *pos, *n;
    unsigned long flags;
    
    pstFrameList->u32RelTotal++;
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    for (pos = (pstFrameList->stFulFrmList).next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList) && pos != (pstFrameList->pstTarget_1)->next;
		pos = n, n = pos->next)
	{
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);  

        if((pstTarget->stOutFrame.stBufAddr[0].u32PhyAddr_Y
           == pstFrame->stBufAddr[0].u32PhyAddr_Y)
            /*FOR 3D FRM,4K may be 3D*/
           && (pstTarget->stOutFrame.u32FrameIndex
           == pstFrame->u32FrameIndex))
        {
            u32RelCount++;
            if(pstFrameList->pstTarget_1 == pos)
            {
                pstFrameList->pstTarget_1 = (pstFrameList->pstTarget_1)->prev;
            }

            list_del_init(pos);
            break;
        }
    }

    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);

    if(u32RelCount != 1)
    {
        VPSS_FATAL("Rel Error ,Rel %d buffer\n",u32RelCount);
        return HI_FAILURE;
    }
    else
    {   
        pstFrameList->u32RelSuccess++; 
		pstTarget->stOutFrame.u32FrameIndex = 0xffffffff;
        VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstTarget,VPSS_FB_TYPE_NORMAL);
    }    
   
    return HI_SUCCESS;
}

HI_S32 VPSS_FB_RelFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VPSS_FB_NODE_S *pstTarget = HI_NULL;
    VPSS_FB_NODE_S *pstRelTarget_1 = HI_NULL;
    VPSS_FB_NODE_S *pstRelTarget_2 = HI_NULL;
    HI_U32 u32RelCount = 0;
    VPSS_BUFFER_S *pstBuf;
    LIST *pos, *n;
    HI_S32 s32GetFrm = HI_FAILURE;
    HI_U32 u32Count = 0;
    unsigned long flags;
    
    pstFrameList->u32RelTotal++;

    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    for (pos = (pstFrameList->stFulFrmList).next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList) && pos != (pstFrameList->pstTarget_1)->next;
		pos = n, n = pos->next)
    {
        if ( u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("\n RelFulFrmBuf Error\n");
        }
        u32Count ++;
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstBuf = &(pstTarget->stBuffer);

        if((pstBuf->stMemBuf.u32StartPhyAddr
           == pstFrame->stBufAddr[0].u32PhyAddr_Y
            /*FOR 3D FRM*/
           || pstBuf->stMemBuf.u32StartPhyAddr
           == pstFrame->stBufAddr[1].u32PhyAddr_Y)
		   &&(pstBuf->stMemBuf.u32StartPhyAddr != 0)) 
        {
            if(pstTarget->stOutFrame.u32FrameIndex != pstFrame->u32FrameIndex)
            {
               VPSS_WARN("\nRel Error BufferId %d FrmId %d\n",
                    pstTarget->stOutFrame.u32FrameIndex,
                    pstFrame->u32FrameIndex);
               s32GetFrm = HI_FAILURE;
               break;
            }
            
            
            if(pstFrameList->pstTarget_1 != pos)
            {

            }
            else
            {
                pstFrameList->pstTarget_1 = (pstFrameList->pstTarget_1)->prev;
            }
            
                
            list_del_init(pos);

            if(u32RelCount == 0)
            {
                pstRelTarget_1 = pstTarget;
                u32RelCount ++;
            }
            else if (u32RelCount == 1)
            {
                pstRelTarget_2 = pstTarget;
                u32RelCount ++;
            }
            else
            {
                VPSS_FATAL("Rel Error too many buffer\n");
            }
            
            s32GetFrm = HI_SUCCESS;
        }
    }
     VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);
    if(s32GetFrm == HI_FAILURE)
    {
        VPSS_WARN("Can't Get RelFrm %d\n",pstFrame->u32FrameIndex);
    }
    else
    {
        pstFrameList->u32RelSuccess++;
        if(u32RelCount == 0)
        {
            VPSS_FATAL("Rel Error no buffer\n");
        }
        else if (u32RelCount == 1)
        {
			pstRelTarget_1->stOutFrame.u32FrameIndex = 0xffffffff;
            VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstRelTarget_1,VPSS_FB_TYPE_NORMAL);
        }
        else if (u32RelCount == 2)
        {
			pstRelTarget_1->stOutFrame.u32FrameIndex = 0xffffffff;
			pstRelTarget_2->stOutFrame.u32FrameIndex = 0xffffffff;
            VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstRelTarget_1,VPSS_FB_TYPE_NORMAL);
            VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstRelTarget_2,VPSS_FB_TYPE_NORMAL);
        }
        else
        {
            VPSS_FATAL("Rel Error too many buffer\n");
        }
        
    }
   
    return s32GetFrm;
}

VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBufNoMmz(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    unsigned long flags;
    
    pstTarget = HI_NULL;
    
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        list_del_init(pos);
        break;
    }    
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    
    if (pstTarget)
    {    
        if((pstTarget->stBuffer.stMemBuf.u32StartPhyAddr != 0))
        {
			VPSS_MEM_S *pstMemBuf;
            pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
					VPSS_OSAL_FreeMem(pstMemBuf);
            pstMemBuf->u32Size = 0;
            pstTarget->stBuffer.u32Stride = 0;
			pstMemBuf->u32StartPhyAddr = 0;
			pstMemBuf->pu8StartVirAddr = 0;
        }
        memcpy(&(pstTarget->stOutFrame), pstFrame, 
                sizeof(HI_DRV_VIDEO_FRAME_S));

        return pstTarget;
    }
    else
    {
        return HI_NULL;
    }
}

VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,
                            HI_U32 u32Height,HI_U32 u32Width,
                            HI_DRV_PIX_FORMAT_E ePixFormat,
							HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth,
							HI_BOOL bSecure)
{
    HI_S32 s32Ret;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg;
    VPSS_MEM_S *pstMemBuf;
    unsigned long flags;
    pstBufCfg = &( pstFrameList->stBufListCfg );

    pstTarget = HI_NULL;
    
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        list_del_init(pos);
        break;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);

    if (pstTarget)
    { 
        memset(&(pstTarget->stOutFrame), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));

        if (pstFrameList->stBufListCfg.eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
        {
            HI_U32 u32BufSize = 0;
            HI_U32 u32BufStride = 0;

            pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
            
            VPSS_OSAL_CalBufSize(&u32BufSize, &u32BufStride, 
                        u32Height, u32Width, ePixFormat,enOutBitWidth);
                      
            if (pstMemBuf->u32Size == 0 
               || pstTarget->stBuffer.u32Stride == 0
               || u32BufSize != pstMemBuf->u32Size
               || u32BufStride != pstTarget->stBuffer.u32Stride)
            {
                if (pstMemBuf->u32StartPhyAddr != 0)
                {
					VPSS_OSAL_FreeMem(pstMemBuf);
                    pstMemBuf->u32Size = 0;
					pstMemBuf->u32StartPhyAddr = 0;
					pstMemBuf->pu8StartVirAddr = 0;
                    pstTarget->stBuffer.u32Stride = 0;
                }
				
				if (!bSecure)
				{
					s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,u32BufSize,
							"VPSS_FrmBuf", 
							"VPSS", 
							pstMemBuf);
				}
				else
				{
					s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,u32BufSize,
							"VPSS_FrmBuf", 
							"VPSS", 
							pstMemBuf);
				}

                if (s32Ret == HI_FAILURE)
                {
                    pstMemBuf->u32Size = 0;
                    pstTarget->stBuffer.u32Stride = 0;
                    pstMemBuf->u32StartPhyAddr = 0;
                    pstMemBuf->pu8StartVirAddr = 0;
                    VPSS_FB_AddEmptyFrmBuf(pstFrameList, pstTarget,VPSS_FB_TYPE_NORMAL);
                    VPSS_FATAL("Dynamic Alloc Buffer Failed.BufSize %#x\n",u32BufSize);
                    return HI_NULL;
                }
                pstTarget->stBuffer.u32Stride = u32BufStride;

                
            }
        }
        return pstTarget;
    }
    else
    {
        return HI_NULL;
    }
}

HI_S32 VPSS_FB_AddFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode)
{
    unsigned long flags;
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    list_add_tail(&(pstFBNode->node), &(pstFrameList->stFulFrmList));

    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);
    if (pstFBNode->stOutFrame.bIsFirstIFrame)
    {
        HI_DRV_STAT_Event(STAT_EVENT_VPSSOUTFRM, 0);
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_FB_AddEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,
                                VPSS_FB_NODE_S *pstFBNode,
                                VPSS_FB_TYPE_E enType)
{
    HI_BOOL bFree = HI_FALSE;
    unsigned long flags;
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    if (enType == VPSS_FB_TYPE_NORMAL)
    {
        if (pstFrameList->u32ExtCnt <= pstFrameList->u32ExtNumb)
        {
            list_add(&(pstFBNode->node), &(pstFrameList->stEmptyFrmList));
        }
        else
        {
            bFree = HI_TRUE;
            pstFrameList->u32ExtCnt--;
        }
    }
    else if(enType == VPSS_FB_TYPE_EXTERN)
    {
        list_add_tail(&(pstFBNode->node), &(pstFrameList->stEmptyFrmList));
        pstFrameList->u32ExtCnt++;
    }
    else
    {
        VPSS_VFREE(pstFBNode);
        VPSS_FATAL("Invalid VPSS Fb Type %d\n",enType);
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);

    if (bFree == HI_TRUE)
    {
        VPSS_OSAL_DownSpin(&(pstFrameList->stExtBufSpin),&flags);
        
        list_add_tail(&(pstFBNode->node), &(pstFrameList->stExtFrmList));
        VPSS_OSAL_UpSpin(&(pstFrameList->stExtBufSpin),&flags);
    }
    return HI_SUCCESS;
    
}

HI_BOOL VPSS_FB_CheckIsAvailable(VPSS_FB_INFO_S *pstFrameList,HI_U32  u32AvailableNum)
{
    LIST* pstEmptyList;
    unsigned long flags;
    pstEmptyList = &(pstFrameList->stEmptyFrmList);
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);

    if((DEF_VPSS_2D_BUFFER_DEPEND_NUM == u32AvailableNum)
        && (pstEmptyList->next != pstEmptyList))
    {
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        return HI_TRUE;
    }
    else
    if((DEF_VPSS_3D_BUFFER_DEPEND_NUM == u32AvailableNum)
      && (pstEmptyList->next->next != pstEmptyList))
    {
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        return HI_TRUE;
    }
    else
    {   
        pstFrameList->u32ListFul++;
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        return HI_FALSE;
    }
}

HI_S32 VPSS_FB_Reset(VPSS_FB_INFO_S *pstFrameList)
{
    HI_DRV_VPSS_BUFLIST_CFG_S *pstBufCfg;
    VPSS_FB_NODE_S *pstTarget;
    VPSS_FB_NODE_S *pstRelTarget[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER];
    LIST *pos, *n;
    HI_U32 u32Count;
    HI_U32 u32RelCount;
    unsigned long flags;
    pstBufCfg = &(pstFrameList->stBufListCfg);

    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    u32Count = 0;
    u32RelCount = 0;
    for (pos = (pstFrameList->pstTarget_1)->next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList);
		pos = n, n = pos->next)
    {
        if ( u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("Reset Error\n");
        }
        
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        list_del_init(pos);
        

        pstRelTarget[u32RelCount] = pstTarget;
        
        u32Count ++;
        u32RelCount++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);

    if ( u32RelCount >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
    {
       VPSS_FATAL("Reset Error\n");
    }
    
    for(u32Count = 0; u32Count < u32RelCount;u32Count ++)
    {
        VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstRelTarget[u32Count],VPSS_FB_TYPE_NORMAL);
    }
    
    pstFrameList->u32GetTotal = 0 ;
    pstFrameList->u32GetSuccess = 0 ;
    pstFrameList->u32RelTotal = 0 ;
    pstFrameList->u32RelSuccess = 0 ;
    pstFrameList->u32GetHZ = 0 ;
    pstFrameList->u32GetLast = 0 ;
    
    return HI_SUCCESS;
}
HI_S32 VPSS_FB_GetState(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_STATE_S *pstFbState)
{
    HI_U32 u32Count;
    VPSS_FB_NODE_S *pstFbNode;
    LIST *pos, *n;
    HI_U32 u32Total = 0;
    HI_U32 u32DoneFlag;
    unsigned long flags;

    
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin),&flags);
    if (pstFrameList->pstTarget_1 != &(pstFrameList->stFulFrmList))
    {
        pstFbState->u32Target_1 = (HI_U32)list_entry(pstFrameList->pstTarget_1, VPSS_FB_NODE_S, node);
        pstFbNode = list_entry(pstFrameList->pstTarget_1, VPSS_FB_NODE_S, node);
        pstFbState->u32OutRate = pstFbNode->stOutFrame.u32FrameRate;
    }
    else
    {
        pstFbState->u32Target_1 = (HI_U32)&(pstFrameList->stFulFrmList);
    }
        
    if (pstFrameList->pstTarget_1 == &(pstFrameList->stFulFrmList))
    {
        u32DoneFlag = 2;
    }
    else
    {
        u32DoneFlag = 1;
    }    
    
    pstFbState->u32WaitSinkRlsNumb = 0;
    u32Count = 0;
    list_for_each_safe(pos, n, &(pstFrameList->stFulFrmList))
    {
        if (u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("Get FbList Error\n");
            break;
        }
        #if FB_DBG
        pstFbNode = list_entry(pos, VPSS_FB_NODE_S, node);
        
        pstFbState->u32FulList[u32Count] = (HI_U32)pstFbNode;
        
        pstFbState->u32List[u32Total][0] = pstFbNode->stOutFrame.u32FrameIndex;
        pstFbState->u32List[u32Total][1] = u32DoneFlag;

        if (pstFbState->u32Target_1 == (HI_U32)pstFbNode)
        {
            u32DoneFlag = 2;
        }
        #endif
        if(pstFrameList->pstTarget_1 == pos)
        {
            pstFbState->u32WaitSinkRlsNumb = u32Count + 1;
        }
        u32Count++;
        u32Total++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin),&flags);
    pstFbState->u32FulListNumb = u32Count;
    
    u32Count = 0;
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        if (u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("Get FbList Error\n");
            break;
        }
        #if FB_DBG
        pstFbNode = list_entry(pos, VPSS_FB_NODE_S, node);
        pstFbState->u32EmptyList[u32Count] = (HI_U32)pstFbNode;

        pstFbState->u32List[u32Total][0] = -1;
        pstFbState->u32List[u32Total][1] = 0;
        #endif
        
        u32Count++;
        u32Total++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
    pstFbState->u32EmptyListNumb = u32Count;

    #if FB_DBG
    while(u32Total < DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
    {
        pstFbState->u32List[u32Total][0] = -1;
        pstFbState->u32List[u32Total][1] = 3;
        u32Total++;
    }
    #endif
    
    pstFbState->u32ExtListNumb = pstFrameList->u32ExtNumb;
    pstFbState->u32TotalNumb = pstFrameList->stBufListCfg.u32BufNumber;

    pstFbState->u32GetHZ = pstFrameList->u32GetHZ;
    pstFbState->u32GetTotal = pstFrameList->u32GetTotal;
    pstFbState->u32GetSuccess = pstFrameList->u32GetSuccess;
    
    pstFbState->u32RelTotal = pstFrameList->u32RelTotal;
    pstFbState->u32RelSuccess = pstFrameList->u32RelSuccess;
    
    pstFbState->u32ListFul = pstFrameList->u32ListFul;
    
    return HI_SUCCESS;                            
}


HI_S32 VPSS_FB_AllocExtBuffer(VPSS_FB_INFO_S *pstFrameList,HI_U32 u32ExtNumb)
{
    HI_U32 i;
    VPSS_FB_NODE_S* pstNode;
    VPSS_BUFFER_S* pstBuf;
    VPSS_MEM_S* pstMemBuf;
    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg;
    HI_S32 s32Ret;
    unsigned long flags;
    
    pstBufCfg = &( pstFrameList->stBufListCfg );
    if (u32ExtNumb != pstFrameList->stBufListCfg.u32BufNumber
        && u32ExtNumb != 0)
    {
        VPSS_FATAL("Alloc Extern 3D Buffer number %d Attr %d Failed\n",
            u32ExtNumb,pstFrameList->stBufListCfg.u32BufNumber);
        return HI_FAILURE;
    }

    /*Cnt > numb means:3D->2D buffer decrease,if numb != 0, it is an error*/
    if (pstFrameList->u32ExtCnt > u32ExtNumb)
    {
        if (u32ExtNumb != 0)
        {
            VPSS_FATAL("Release Extern 3D Buffer number %d Cnt %d Failed\n",
                u32ExtNumb,pstFrameList->u32ExtCnt);
        }
        VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        pstFrameList->u32ExtNumb = u32ExtNumb;
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        return HI_SUCCESS;
    }
    else
    {
        VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        pstFrameList->u32ExtNumb = u32ExtNumb;
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin),&flags);
        for (i = pstFrameList->u32ExtCnt;i < u32ExtNumb;i ++)
        {
            
            pstNode = ( VPSS_FB_NODE_S* )VPSS_VMALLOC( sizeof( VPSS_FB_NODE_S ));
            if (pstNode != HI_NULL)
            {
                memset( &( pstNode->stOutFrame ), 0, sizeof( HI_DRV_VIDEO_FRAME_S ) );
                pstBuf = &( pstNode->stBuffer );

                pstBuf->u32Stride = 0;

                pstMemBuf = &(pstBuf->stMemBuf);
                pstMemBuf->u32StartPhyAddr = 0;
                pstMemBuf->pu8StartVirAddr = 0;
                pstMemBuf->u32Size = 0;
                
                VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstNode,VPSS_FB_TYPE_EXTERN);
                
				s32Ret = HI_SUCCESS;
            }
            else
            {
                
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_FB_RlsExtBuffer(VPSS_FB_INFO_S *pstFrameList)
{
    HI_DRV_VPSS_BUFLIST_CFG_S *pstBufCfg;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    VPSS_MEM_S *pstMemBuf;
    HI_U32 u32RlsCnt;
    HI_U32 i;
    unsigned long flags;
    
    VPSS_FB_NODE_S *pstRlsNode[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER] = {0};
    
    pstBufCfg = &(pstFrameList->stBufListCfg);
    
    VPSS_OSAL_DownSpin(&(pstFrameList->stExtBufSpin),&flags);
    u32RlsCnt= 0;
    list_for_each_safe(pos, n, &(pstFrameList->stExtFrmList))
    {
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstRlsNode[u32RlsCnt] = pstTarget;
        list_del_init(pos);
        u32RlsCnt++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stExtBufSpin),&flags);
    
    for (i = 0; i < DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER; i++)
    {
        if (pstRlsNode[i] != HI_NULL)
        {
            pstTarget = pstRlsNode[i];
            pstMemBuf = &(pstTarget->stBuffer.stMemBuf);
            if (pstBufCfg->eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
			{    
				if (pstMemBuf->u32StartPhyAddr != 0 || pstMemBuf->pu8StartVirAddr != 0)
				{
					VPSS_OSAL_FreeMem(pstMemBuf);
				}
			}
            VPSS_VFREE(pstTarget);
        }
    }
    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
