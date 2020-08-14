#include "vpss_img.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
HI_S32 VPSS_IMG_AddFulImage(VPSS_IMAGELIST_INFO_S *pstImageList,
                                VPSS_IMAGE_NODE_S *pstFulImage)
{
    VPSS_IMAGE_NODE_S *pstTarget;
    LIST *pstTarget_1;
    LIST *pos, *n;

    VPSS_OSAL_DownLock(&(pstImageList->stFulListLock));

   
    if (pstFulImage->stSrcImage.bProgressive == HI_FALSE)
    {
        /*
           *P -> I
           *Empty -> I
           *First Frame read filed
           */
        if (pstImageList->stFulImageList.next == &(pstImageList->stFulImageList))
        {
            pstFulImage->stSrcImage.bProgressive = HI_TRUE;
        }
    }
    else
    {
        /*
           *I -> P
           */
        if (pstImageList->stFulImageList.next != &(pstImageList->stFulImageList))
        {
            pstTarget_1 = pstImageList->pstTarget_1;
            if (pstTarget_1 != pstImageList->stFulImageList.next)
            {
                VPSS_ERROR("interlace to progressive Error\n");
            }
            for (pos = pstTarget_1->next, n = pos->next; 
                    pos != &(pstImageList->stFulImageList); 
		            pos = n, n = pos->next)
            {
                pstTarget = list_entry(pos, VPSS_IMAGE_NODE_S, node);
                pstTarget->stSrcImage.bProgressive = HI_TRUE;
            }
        }
    }
    
    list_add_tail(&(pstFulImage->node), &(pstImageList->stFulImageList));
    VPSS_OSAL_UpLock(&(pstImageList->stFulListLock));

    return HI_SUCCESS;
}
HI_U32 VPSS_IMG_AddEmptyImage(VPSS_IMAGELIST_INFO_S *pstImageList,
                                VPSS_IMAGE_NODE_S *pstUndoImage)
{
    VPSS_OSAL_DownLock(&(pstImageList->stEmptyListLock));
    list_add_tail(&(pstUndoImage->node), &(pstImageList->stEmptyImageList));
    VPSS_OSAL_UpLock(&(pstImageList->stEmptyListLock));

    return HI_SUCCESS;
}

VPSS_IMAGE_NODE_S* VPSS_IMG_GetEmptyImage(VPSS_IMAGELIST_INFO_S *pstImageList)
{
    VPSS_IMAGE_NODE_S *pstTarget;
    LIST *pos, *n;
    
    pstTarget = HI_NULL;

    VPSS_OSAL_DownLock(&(pstImageList->stEmptyListLock));
    list_for_each_safe(pos, n, &(pstImageList->stEmptyImageList))
    {
        pstTarget = list_entry(pos, VPSS_IMAGE_NODE_S, node);
        list_del_init(pos);
        break;
    }
    VPSS_OSAL_UpLock(&(pstImageList->stEmptyListLock));

    if(pstTarget)
    {
        memset(&(pstTarget->stSrcImage), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));
        return pstTarget;
    }
    else
    {
        return HI_NULL;
    }
}

HI_S32 VPSS_IMG_GetProcessImg(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                                   HI_DRV_VIDEO_FRAME_S **ppstImg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_IMAGE_NODE_S *pstImgNode;

    VPSS_OSAL_DownLock(&(pstImgInfo->stFulListLock));
    if((pstImgInfo->pstTarget_1)->next != &(pstImgInfo->stFulImageList))
    {
        pstImgNode = list_entry((pstImgInfo->pstTarget_1)->next, VPSS_IMAGE_NODE_S, node);

        VPSS_OSAL_UpLock(&(pstImgInfo->stFulListLock));
        
                    
        *ppstImg = &(pstImgNode->stSrcImage);
        s32Ret = HI_SUCCESS;
    }
    else
    {
        VPSS_OSAL_UpLock(&(pstImgInfo->stFulListLock));
        
        *ppstImg = HI_NULL;
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}
HI_BOOL VPSS_IMG_CheckEmptyNode(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    VPSS_IMAGE_NODE_S* pstEmpty1stImage;
    VPSS_IMAGE_NODE_S* pstEmpty2ndImage;
    
    /*get two image node to split interlace image*/
    pstEmpty1stImage = VPSS_IMG_GetEmptyImage(pstImgInfo);
    
    if(!pstEmpty1stImage)
    {
        return HI_FALSE;
    }
    pstEmpty2ndImage = VPSS_IMG_GetEmptyImage(pstImgInfo);

    if(!pstEmpty2ndImage)
    {
        VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty1stImage);
        return HI_FALSE;
    }

    VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty1stImage);
    VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty2ndImage);

    return HI_TRUE;
}
HI_BOOL VPSS_IMG_CheckImageList(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    HI_BOOL bReVal;
    VPSS_IMAGE_NODE_S *pstImgNode;
    LIST *pre;
    LIST *cur;
    LIST *next1;
    LIST *next2;
    LIST *next3;
    
    VPSS_OSAL_DownLock(&(pstImgInfo->stFulListLock));
    
    /*has undo image*/
    if ((pstImgInfo->pstTarget_1)->next != &(pstImgInfo->stFulImageList))
    {
        pstImgNode = list_entry((pstImgInfo->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
        /*undo image is progressive*/
        if(pstImgNode->stSrcImage.bProgressive == HI_TRUE)
        {
            bReVal = HI_TRUE;
        }
        /*undo image is interlace*/
        else
        {   
            pre = pstImgInfo->pstTarget_1;
            if(pre == &(pstImgInfo->stFulImageList))
            {
                bReVal = HI_FALSE;
                if (pre->next != &(pstImgInfo->stFulImageList))
                {
                    pstImgInfo->pstTarget_1 = pstImgInfo->pstTarget_1->next;
                    pre = pstImgInfo->pstTarget_1;
                }
                goto CHECKDONE;
            }
            cur = pre->next;
            if(cur == &(pstImgInfo->stFulImageList))
            {
                bReVal = HI_FALSE;
                goto CHECKDONE;
            }
            next1 = cur->next;
            if(next1 == &(pstImgInfo->stFulImageList))
            {
                bReVal = HI_FALSE;
                goto CHECKDONE;
            }
            next2 = next1->next;
            if(next2 == &(pstImgInfo->stFulImageList))
            {
                bReVal = HI_FALSE;
                goto CHECKDONE;
            }
            next3 = next2->next;
            if(next3 == &(pstImgInfo->stFulImageList))
            {
                bReVal = HI_FALSE;
                goto CHECKDONE;
            }
            
            bReVal = HI_TRUE;
        }
    }
    else
    {
        bReVal = HI_FALSE;
    }
    
CHECKDONE:
    VPSS_OSAL_UpLock(&(pstImgInfo->stFulListLock));
    return bReVal;
}
HI_BOOL VPSS_IMG_CheckNeedRelease(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                                    VPSS_IMAGE_NODE_S *pstDoneImageNode)
{
    VPSS_IMAGELIST_INFO_S *pstImageList;
    HI_BOOL bNeedRls = HI_FALSE;
    
    pstImageList = pstImgInfo;
    
    if(pstDoneImageNode->stSrcImage.bProgressive == HI_FALSE)
    {
        if(pstDoneImageNode->stSrcImage.enFieldMode == HI_DRV_FIELD_TOP
            && pstDoneImageNode->stSrcImage.bTopFieldFirst == HI_FALSE)
        {
            bNeedRls = HI_TRUE;
        }

        if(pstDoneImageNode->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM
            && pstDoneImageNode->stSrcImage.bTopFieldFirst == HI_TRUE)
        {
            bNeedRls = HI_TRUE;
        }
    }
    else
    {
        bNeedRls = HI_TRUE;
    }

    return bNeedRls;
}
HI_S32 VPSS_IMG_AddNewImg(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                            HI_DRV_VIDEO_FRAME_S *pstNewImg)
{
    VPSS_IMAGE_NODE_S* pstEmpty1stImage;
    VPSS_IMAGE_NODE_S* pstEmpty2ndImage;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_S32 s32Ret;
    
    /*get two image node to split interlace image*/
    pstEmpty1stImage = VPSS_IMG_GetEmptyImage(pstImgInfo);
    
    if(!pstEmpty1stImage)
    {
        return HI_FAILURE;
    }
    pstEmpty2ndImage = VPSS_IMG_GetEmptyImage(pstImgInfo);

    if(!pstEmpty2ndImage)
    {
        VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty1stImage);
        return HI_FAILURE;
    }
    
    VPSS_SAFE_MEMCPY(&(pstEmpty1stImage->stSrcImage),
            pstNewImg,
            sizeof(HI_DRV_VIDEO_FRAME_S));
            
    //VPSS_INST_CheckNeedRstDei(pstInstance,&(pstEmpty1stImage->stSrcImage));

    if (pstEmpty1stImage->stSrcImage.bProgressive == HI_FALSE
        && pstEmpty1stImage->stSrcImage.enFieldMode == HI_DRV_FIELD_ALL)
    {
        HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
        HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
        
        /*
        * split interlace image to top and bottom
        */
        VPSS_SAFE_MEMCPY(&(pstEmpty2ndImage->stSrcImage),&(pstEmpty1stImage->stSrcImage),
                sizeof(HI_DRV_VIDEO_FRAME_S));
                
        if(pstEmpty1stImage->stSrcImage.bTopFieldFirst == HI_TRUE)
        {
            pstEmpty1stImage->stSrcImage.enFieldMode = HI_DRV_FIELD_TOP;
            
            pstEmpty2ndImage->stSrcImage.enFieldMode = HI_DRV_FIELD_BOTTOM;
        }
        else
        {
            pstEmpty1stImage->stSrcImage.enFieldMode = HI_DRV_FIELD_BOTTOM;
            
            pstEmpty2ndImage->stSrcImage.enFieldMode = HI_DRV_FIELD_TOP;
        }
        pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);

        pstEmpty2ndImage->stSrcImage.u32Pts = pstEmpty1stImage->stSrcImage.u32Pts
                                             + pstVdecPriv->s32InterPtsDelta;
        pstEmpty2ndImage->stSrcImage.s64OmxPts = pstEmpty1stImage->stSrcImage.s64OmxPts
                                             + pstVdecPriv->s32InterPtsDelta*1000;
        pstEmpty1stImage->stSrcImage.u32FrameRate = 
                    pstEmpty1stImage->stSrcImage.u32FrameRate*2;
        pstEmpty2ndImage->stSrcImage.u32FrameRate = 
                    pstEmpty2ndImage->stSrcImage.u32FrameRate*2;


        #if 1
        if (pstEmpty1stImage->stSrcImage.bIsFirstIFrame)
        {
            HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
            HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;

            pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
            pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);

            //pstEmpty1stImage->stSrcImage.bProgressive = HI_TRUE;
            
            pstEmpty2ndImage->stSrcImage.bIsFirstIFrame = HI_FALSE;
        }
        #endif
        
        /*  VFMW last frame scheme
            *if the frame has last frame flag
            *progressive:bypass
            *interlace:change last three field info to progressive
            */
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
        
        if (pstPriv->u32LastFlag
                == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
        {
            /*1.change pre-frame's second field I->P*/
            VPSS_IMAGE_NODE_S* pstPreNode;
            if(pstImgInfo->stFulImageList.prev
                != &(pstImgInfo->stFulImageList))
            {
                pstPreNode = list_entry((pstImgInfo->stFulImageList.prev),
                        VPSS_IMAGE_NODE_S, node);
                pstPreNode->stSrcImage.bProgressive = HI_TRUE;
            }
            else
            {
                VPSS_FATAL("Last Field Error\n");
            }

            /*2.change last frame's two field I->P*/
            pstEmpty1stImage->stSrcImage.bProgressive = HI_TRUE;
            pstEmpty2ndImage->stSrcImage.bProgressive = HI_TRUE;
            /*3.set last flag to second field*/
            pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
            pstPriv->u32LastFlag = 0;

            pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty2ndImage->stSrcImage.u32Priv[0]);
            pstPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;

            s32Ret = VPSS_IMG_AddFulImage(pstImgInfo,pstEmpty1stImage);  
        
            s32Ret = VPSS_IMG_AddFulImage(pstImgInfo,pstEmpty2ndImage);  
        }
        else if(pstPriv->u32LastFlag
                == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
        {
            VPSS_IMAGE_NODE_S* pstPreNode;
            LIST * pstPre;
            
            HI_BOOL bPreProg;
            pstPre = pstImgInfo->stFulImageList.prev;
            bPreProg = HI_FALSE;

            /*DEF_HI_DRV_VPSS_LAST_ERROR_FLAG must be Interlace
                *we need to check whether pre stream is interlace
                */
            if (pstPre != &(pstImgInfo->stFulImageList))
            {
                pstPreNode = list_entry(pstPre,
                        VPSS_IMAGE_NODE_S, node);
                bPreProg = pstPreNode->stSrcImage.bProgressive;
            }

            if (bPreProg == HI_TRUE || pstPre == &(pstImgInfo->stFulImageList))
            {
                
            }
            else
            {
                if (pstPre != &(pstImgInfo->stFulImageList)) 
                {
                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;
                }
                
                pstPre = pstPre->prev;
                if (pstPre != &(pstImgInfo->stFulImageList)) 
                {
                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;
                }

                pstPre = pstPre->prev;
                if (pstPre != &(pstImgInfo->stFulImageList)) 
                {
                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;
                }
            }
            

            (HI_VOID)VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty1stImage);  
        
            (HI_VOID)VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty2ndImage);  
        }
        else
        {
            (HI_VOID)VPSS_IMG_AddFulImage(pstImgInfo,pstEmpty1stImage); 
            
            (HI_VOID)VPSS_IMG_AddFulImage(pstImgInfo,pstEmpty2ndImage);  
        }
        
    }
    else
    {
        //VPSS_INST_ChangeInRate(pstInstance,pstEmpty1stImage->stSrcImage.u32FrameRate);
            
        (HI_VOID)VPSS_IMG_AddFulImage(pstImgInfo,pstEmpty1stImage);
    
        (HI_VOID)VPSS_IMG_AddEmptyImage(pstImgInfo,pstEmpty2ndImage);
    }

    return HI_SUCCESS;
}       

HI_S32 VPSS_IMG_CopyAddr(LIST *pNode,
                                HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                                HI_DRV_BUF_ADDR_E eLReye)
{
    
    VPSS_IMAGE_NODE_S *pstTarget;
    HI_DRV_VIDEO_FRAME_S *pstImg;

    if (eLReye >= HI_DRV_BUF_ADDR_MAX)
    {
        VPSS_FATAL("Invalid eLReye:%d\n", eLReye);
        return HI_FAILURE;
    }
    
    pstTarget = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
    pstImg = &(pstTarget->stSrcImage);
    
    VPSS_SAFE_MEMCPY(pstFieldAddr,&(pstImg->stBufAddr[eLReye]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));
    if(pstTarget->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        if (pstFieldAddr->u32Stride_Y >= 256)
        {   
            if (pstTarget->stSrcImage.ePixFormat== HI_DRV_PIX_FMT_NV12_TILE
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
            {
                pstFieldAddr->u32PhyAddr_Y = pstFieldAddr->u32PhyAddr_Y + 256;
            }
            else
            {
                pstFieldAddr->u32PhyAddr_Y = pstFieldAddr->u32PhyAddr_Y + pstFieldAddr->u32Stride_Y;   
            }
        }
        else
        {
            pstFieldAddr->u32PhyAddr_Y = pstFieldAddr->u32PhyAddr_Y + pstFieldAddr->u32Stride_Y;
        }

        if (pstFieldAddr->u32Stride_C >= 256)

        {
            if (pstTarget->stSrcImage.ePixFormat== HI_DRV_PIX_FMT_NV12_TILE
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
            {
                pstFieldAddr->u32PhyAddr_C= pstFieldAddr->u32PhyAddr_C + 256;
            }
            else
            {
                pstFieldAddr->u32PhyAddr_C = pstFieldAddr->u32PhyAddr_C + pstFieldAddr->u32Stride_C;
            }
        }
        else
        {
            pstFieldAddr->u32PhyAddr_C = pstFieldAddr->u32PhyAddr_C + pstFieldAddr->u32Stride_C;
        }
        
        VPSS_INFO("BOTTOM FIELD PhyAddr_Y : %#x, PhyAddr_C : %#x, Stride_Y : %#x, Stride_C : %#x.\n",
            pstFieldAddr->u32PhyAddr_Y,pstFieldAddr->u32PhyAddr_C,pstFieldAddr->u32Stride_Y,pstFieldAddr->u32Stride_C);
        
    }
    else
    {
        VPSS_INFO("TOP FIELD PhyAddr_Y : %#x, PhyAddr_C : %#x, Stride_Y : %#x, Stride_C : %#x.\n",
            pstFieldAddr->u32PhyAddr_Y,pstFieldAddr->u32PhyAddr_C,pstFieldAddr->u32Stride_Y,pstFieldAddr->u32Stride_C);
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_IMG_GetFieldAddr(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                            HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                            HI_DRV_BUF_ADDR_E eLReye)
{
    LIST *pNode;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstTarget;
    HI_DRV_VIDEO_FRAME_S *pstImg;
    HI_DRV_VID_FRAME_ADDR_S stAddr[6];
    pstImageList = pstImgInfo;

    memset(stAddr,0,sizeof(HI_DRV_VID_FRAME_ADDR_S)*6);

    pNode = pstImageList->pstTarget_1;
    if (pNode != &(pstImageList->stFulImageList))
    {
        pstTarget = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        pstImg = &(pstTarget->stSrcImage);
    }
    else
    {
        VPSS_FATAL("Get Dei Addr Error\n");
        return HI_FAILURE;
    }
    
    pNode = pstImageList->pstTarget_1;
    (HI_VOID)VPSS_IMG_CopyAddr(pNode,&(stAddr[0]),eLReye);
                
    pNode = pNode->next->next;
    if(pNode == &(pstImageList->stFulImageList))
    {
        VPSS_FATAL("GetDeiAddr Error 3\n");
    }
    (HI_VOID)VPSS_IMG_CopyAddr(pNode,&(stAddr[3]),eLReye);
    
    pNode = pNode->next;
    (HI_VOID)VPSS_IMG_CopyAddr(pNode,&(stAddr[4]),eLReye);   
                
    pNode = pNode->next;
    (HI_VOID)VPSS_IMG_CopyAddr(pNode,&(stAddr[5]),eLReye);
    
    VPSS_SAFE_MEMCPY(pstFieldAddr,stAddr,sizeof(HI_DRV_VID_FRAME_ADDR_S)*6);
    
    return HI_SUCCESS;
}             

HI_S32 VPSS_IMG_Complete(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstDoneImageNode;
    VPSS_IMAGE_NODE_S *pstNextImageNode;
    HI_BOOL bProgressive;
    LIST *pos, *n, *head;
    HI_BOOL bNeedRls = HI_FALSE;
    pstImageList = pstImgInfo;
    pstDoneImageNode = HI_NULL;
    bProgressive = HI_FALSE;

    /*
      *  release done image according to fieldmode
      *  Progressive:not related to pre field ,target_1 can be released
      *  Interlace: related to pre field,target_1->pre can be released
     */
    if(pstImageList->pstTarget_1->next != &(pstImageList->stFulImageList))
    {
        pstDoneImageNode = list_entry((pstImageList->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
        bProgressive = pstDoneImageNode->stSrcImage.bProgressive;
    }
    else
    {
        VPSS_FATAL("RelDoneImage Error\n");
        return HI_FAILURE;
    }

    /*
      * progressive image process
     */
    pstDoneImageNode = HI_NULL;
    if (bProgressive == HI_TRUE)
    {
        head = &(pstImageList->stFulImageList);
            
        /*
           * if progressive,release all before image 
           */
        for (pos = (head)->next, n = pos->next; pos != (pstImageList->pstTarget_1)->next; 
	        pos = n, n = pos->next)
	    {
            pstDoneImageNode = list_entry(pos,
                            VPSS_IMAGE_NODE_S, node);
            if (pos == pstImageList->pstTarget_1)
            {
                pstImageList->pstTarget_1 = pstImageList->pstTarget_1->prev;
            }
            list_del_init(&(pstDoneImageNode->node));

            bNeedRls = VPSS_IMG_CheckNeedRelease(pstImgInfo,pstDoneImageNode);
            if (bNeedRls)
            {
                pstImageList->u32RelUsrTotal ++;
                pstImgInfo->pfnRlsImage(pstImgInfo->hSrc,
                                    &(pstDoneImageNode->stSrcImage));
                pstImageList->u32RelUsrSuccess++;
            }

            VPSS_IMG_AddEmptyImage(pstImgInfo,pstDoneImageNode);
	    }
	    
        if(pstImageList->pstTarget_1->next != &(pstImageList->stFulImageList))
        {
            pstDoneImageNode = list_entry((pstImageList->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
            
            /*P -> I*/
            if (pstImageList->pstTarget_1->next->next 
                    != &(pstImageList->stFulImageList))
            {
                pstNextImageNode = list_entry(
                            (pstImageList->pstTarget_1->next->next),
                            VPSS_IMAGE_NODE_S, node);
                if (pstNextImageNode->stSrcImage.u32FrameIndex
                    == pstDoneImageNode->stSrcImage.u32FrameIndex)
                {
                    pstDoneImageNode->stSrcImage.bProgressive = HI_FALSE;
                    pstImageList->pstTarget_1 = pstImageList->pstTarget_1->next;
                }
                else
                {
                    if (pstDoneImageNode->stSrcImage.bProgressive == HI_FALSE)
                    {
                        pstDoneImageNode->stSrcImage.bProgressive = HI_TRUE;
                    }
                    
                    list_del_init(&(pstDoneImageNode->node));
                    bNeedRls = VPSS_IMG_CheckNeedRelease(pstImgInfo,pstDoneImageNode);
                    if (bNeedRls)
                    {
                        pstImageList->u32RelUsrTotal ++;
                        pstImgInfo->pfnRlsImage(pstImgInfo->hSrc,
                                            &(pstDoneImageNode->stSrcImage));
                        pstImageList->u32RelUsrSuccess++;
                    }
                    
                    VPSS_IMG_AddEmptyImage(pstImgInfo,pstDoneImageNode);
                }
                
            }
            else
            {
                list_del_init(&(pstDoneImageNode->node));
                bNeedRls = VPSS_IMG_CheckNeedRelease(pstImgInfo,pstDoneImageNode);
                if (bNeedRls)
                {
                    pstImageList->u32RelUsrTotal ++;
                    pstImgInfo->pfnRlsImage(pstImgInfo->hSrc,
                                        &(pstDoneImageNode->stSrcImage));
                    pstImageList->u32RelUsrSuccess++;
                }

                VPSS_IMG_AddEmptyImage(pstImgInfo,pstDoneImageNode);
            }
        }
        else
        {

        }
    }
    else
    {
        /*
            *release target_1->prev
            */
        if(pstImageList->pstTarget_1 != &(pstImageList->stFulImageList))
        {
            pstDoneImageNode = list_entry((pstImageList->pstTarget_1),
                        VPSS_IMAGE_NODE_S, node);
            pstImageList->pstTarget_1 = pstImageList->pstTarget_1->prev;
            list_del_init(&(pstDoneImageNode->node));
         
            bNeedRls = VPSS_IMG_CheckNeedRelease(pstImgInfo,pstDoneImageNode);
            if (bNeedRls)
            {
                pstImageList->u32RelUsrTotal ++;
                pstImgInfo->pfnRlsImage(pstImgInfo->hSrc,
                                    &(pstDoneImageNode->stSrcImage));
                pstImageList->u32RelUsrSuccess++;
            }
            
            VPSS_IMG_AddEmptyImage(pstImgInfo,pstDoneImageNode);
            pstImageList->pstTarget_1 = pstImageList->pstTarget_1->next;
        }
        else
        {
            
        }
    }
   
    return HI_SUCCESS;
}


HI_S32 VPSS_IMG_Regist(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                    VPSS_IMG_CALLBACK_S stCallback)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if (pstImgInfo->hSrc == HI_INVALID_HANDLE)
    {
        pstImgInfo->hSrc = stCallback.hSrc;
        pstImgInfo->pfnAcqImage = stCallback.pfnAcqImage;
        pstImgInfo->pfnRlsImage = stCallback.pfnRlsImage;
        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}                    


HI_S32 VPSS_IMG_Init(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstImageNode[VPSS_SOURCE_MAX_NUMB];
    HI_U32 u32Count;
    
    pstImageList = pstImgInfo;

    pstImageList->hSrc = HI_INVALID_HANDLE;
    pstImageList->pfnAcqImage = HI_NULL;
    pstImageList->pfnRlsImage = HI_NULL;

    pstImageList->u32GetUsrTotal = 0 ;
    pstImageList->u32GetUsrSuccess = 0;
    pstImageList->u32RelUsrTotal = 0;
    pstImageList->u32RelUsrSuccess = 0;
    
    VPSS_OSAL_InitLOCK(&(pstImageList->stEmptyListLock), 1);
    VPSS_OSAL_InitLOCK(&(pstImageList->stFulListLock), 1);

    INIT_LIST_HEAD(&(pstImageList->stEmptyImageList));
    INIT_LIST_HEAD(&(pstImageList->stFulImageList));

    u32Count = 0;
    while(u32Count < VPSS_SOURCE_MAX_NUMB)
    {
        pstImageNode[u32Count] = (VPSS_IMAGE_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_IMAGE_NODE_S));
        if (pstImageNode[u32Count] == HI_NULL)
        {
            VPSS_FATAL("vmalloc imageNode failed\n");
            goto V1_VMALLOC_ERROR;
        }
        memset(&(pstImageNode[u32Count]->stSrcImage), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));
        
        list_add_tail(&(pstImageNode[u32Count]->node), 
                        &(pstImageList->stEmptyImageList));
        u32Count++;
    }

    pstImageList->pstTarget_1 = &(pstImageList->stFulImageList);

    return HI_SUCCESS;
    
V1_VMALLOC_ERROR:
    return HI_FAILURE;
}

HI_S32 VPSS_IMG_DeInit(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Count = 0;
    VPSS_IMAGE_NODE_S* pstImgNode;
    LIST *pos, *n;
    
    VPSS_IMAGELIST_INFO_S *pstImageList;
    
    pstImageList = pstImgInfo;

    list_for_each_safe(pos, n, &(pstImageList->stEmptyImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_VFREE(pstImgNode);
        u32Count++;
    }
    
    list_for_each_safe(pos, n, &(pstImageList->stFulImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_VFREE(pstImgNode);
        u32Count++;
    }

    if(u32Count != VPSS_SOURCE_MAX_NUMB)
    {
        VPSS_FATAL("free ImageList Error cnt = %d.\n",
                        u32Count);
        s32Ret = HI_FAILURE;
    }
    
    return s32Ret;
}

HI_S32 VPSS_IMG_CorrectListOrder(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                    HI_BOOL bTopFirst)
{
    LIST *pNode;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstPreFieldNode;
    VPSS_IMAGE_NODE_S *pstCurFieldNode;
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
            
        pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        if(pstCurFieldNode->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
    }
    /*top first -> bottom first */
    else
    {
        pNode = pstImageList->pstTarget_1->next;
            
        pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        if(pstCurFieldNode->stSrcImage.enFieldMode == HI_DRV_FIELD_TOP)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
    }

    for (pos = (pstImageList->stFulImageList).next, n = pos->next; 
            pos != &(pstImageList->stFulImageList);
            pos = pos->next, n = pos->next)
    {
        pPreNode = pos->prev;
        pNextNode = n->next;
        
        pstCurFieldNode = list_entry(n,VPSS_IMAGE_NODE_S, node);
        pstPreFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);
        
        if(pstCurFieldNode->stSrcImage.u32FrameIndex
           != pstPreFieldNode->stSrcImage.u32FrameIndex)
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
            HI_S64 s64Tmp;
            u32Tmp = pstCurFieldNode->stSrcImage.u32Pts;
            pstCurFieldNode->stSrcImage.u32Pts = 
                    pstPreFieldNode->stSrcImage.u32Pts;
            pstPreFieldNode->stSrcImage.u32Pts = u32Tmp;
            s64Tmp = pstCurFieldNode->stSrcImage.s64OmxPts;
            pstCurFieldNode->stSrcImage.s64OmxPts = 
                    pstPreFieldNode->stSrcImage.s64OmxPts;
            pstPreFieldNode->stSrcImage.s64OmxPts = s64Tmp;
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

HI_S32 VPSS_IMG_Reset(VPSS_IMAGELIST_INFO_S *pstImgInfo)
{
    VPSS_IMAGELIST_INFO_S*  pstImgList;
    LIST *pos, *n;
    VPSS_IMAGE_NODE_S* pstImgNode;


    /*reset image list*/
    pstImgList = pstImgInfo;
    
    list_for_each_safe(pos, n, &(pstImgList->stFulImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_IMG_AddEmptyImage(pstImgList,pstImgNode);
    }

    pstImgList->pstTarget_1 = &(pstImgList->stFulImageList);

    
	return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

