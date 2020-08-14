#include "vpss_wbc_mcdei.h"
#include "vpss_wbc.h"
#include "vpss_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
HI_S32 VPSS_RGME_CalBufSize(HI_U32* pSize, HI_U32* pStride, VPSS_RGME_ATTR_S* pstAttr)
{
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32Stride;
    u32Width = pstAttr->u32Width;
    u32Height = pstAttr->u32Height;
    
    u32Stride = (u32Width + 15) / 16 * 16;
    u32Stride = HI_ALIGN_8BIT_YSTRIDE(u32Stride);
    *pSize = u32Stride * u32Height / 2;
    *pStride = u32Stride;
    
    return HI_SUCCESS;    

}

HI_S32 VPSS_RGME_Init(VPSS_RGME_S *pstRgme,VPSS_RGME_ATTR_S *pstAttr)
{  
    HI_U32 i;    
    HI_S32 s32Ret = HI_FAILURE;    
    HI_U32 u32Stride = 0;
    HI_U32 u32NodeBuffSize = 0; 
    HI_U32 u32TotalBuffSize = 0;
    HI_U32 u32PhyAddr = 0;
    LIST* pListHead = HI_NULL; 
    
    if (HI_NULL == pstRgme)
    {
        VPSS_ERROR("Vpss rgme wbc Init error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == pstRgme->bInit)
    {
        (HI_VOID)VPSS_RGME_DeInit(pstRgme);
    }

    memset(pstRgme, 0, sizeof(VPSS_RGME_S));
    VPSS_SAFE_MEMCPY(&pstRgme->stRgmeAttr, pstAttr, sizeof(VPSS_RGME_ATTR_S));


    s32Ret = VPSS_RGME_CalBufSize(&u32NodeBuffSize, &u32Stride, pstAttr);
    if (HI_FAILURE == s32Ret)
    {
        return HI_FAILURE;
    }

    u32TotalBuffSize = u32NodeBuffSize * VPSS_RGME_WBC_MAX_NODE;
#ifdef HI_VPSS_SMMU_SUPPORT
    if (pstAttr->bSecure)
    {
#ifdef HI_TEE_SUPPORT
        s32Ret = HI_DRV_SECSMMU_Alloc("VPSS_RgmeWbcBuf", u32TotalBuffSize,0, &pstRgme->stTEEBuf);
#else
        s32Ret = HI_DRV_SMMU_Alloc( "VPSS_RgmeWbcBuf", u32TotalBuffSize, 0, &(pstRgme->stTEEBuf));
#endif
        u32PhyAddr = pstRgme->stTEEBuf.u32StartSmmuAddr;
    }
    else
    {
        s32Ret = HI_DRV_SMMU_Alloc( "VPSS_RgmeWbcBuf", u32TotalBuffSize, 0, &(pstRgme->stMMUBuf));
        u32PhyAddr = pstRgme->stMMUBuf.u32StartSmmuAddr;
    }
    
    if (HI_FAILURE == s32Ret)
    {
        VPSS_FATAL("VPSS RGME WBC Alloc memory failed.\n");
        return HI_FAILURE;
    }
	
#else
    s32Ret = HI_DRV_MMZ_Alloc( "VPSS_RgmeWbcBuf", HI_NULL, u32TotalBuffSize, 0, &(pstRgme->stMMZBuf));
    if (HI_FAILURE == s32Ret)
    {
        VPSS_FATAL("VPSS RGME WBC Alloc memory failed.\n");
        return HI_FAILURE;
    }
    u32PhyAddr = pstRgme->stMMZBuf.u32StartPhyAddr;
#endif
    pListHead = &(pstRgme->stDataList[0].node);
    INIT_LIST_HEAD(pListHead);
    pstRgme->pstFirstRef = pListHead;

    for (i = 1 ; i < VPSS_RGME_WBC_MAX_NODE; i++)
    {
        list_add_tail(&(pstRgme->stDataList[i].node), pListHead);
    }

    for (i = 0 ; i < VPSS_RGME_WBC_MAX_NODE; i++)
    {
        pstRgme->stDataList[i].stWbcFrame.stBufAddr[0].u32PhyAddr_Y = u32PhyAddr;
        pstRgme->stDataList[i].stWbcFrame.stBufAddr[1].u32PhyAddr_Y = u32PhyAddr;
        pstRgme->stDataList[i].stWbcFrame.stBufAddr[0].u32Stride_Y = u32Stride;
        pstRgme->stDataList[i].stWbcFrame.stBufAddr[1].u32Stride_Y = u32Stride;
        u32PhyAddr += u32NodeBuffSize;
    }   

    pstRgme->bInit = HI_TRUE;

    return HI_SUCCESS;
}
HI_S32 VPSS_RGME_DeInit(VPSS_RGME_S* pstRgme)
{
    if (HI_NULL == pstRgme)
    {
        VPSS_ERROR("Vpss rgme wbc DeInit error(null pointer).\n");
        return HI_FAILURE;
    }
    if (HI_FALSE == pstRgme->bInit)
    {
        VPSS_WARN("Vpss rgme wbc DeInit error(not init).\n");
        //return HI_SUCCESS;
    }    
    
    if (0 != pstRgme->stTEEBuf.u32StartSmmuAddr)
    {
#ifdef HI_TEE_SUPPORT
        (HI_VOID)HI_DRV_SECSMMU_Release(&pstRgme->stTEEBuf);
#else
        HI_DRV_SMMU_Release(&(pstRgme->stTEEBuf));
#endif
    }

#ifdef HI_VPSS_SMMU_SUPPORT
    if (0 != pstRgme->stMMUBuf.u32StartSmmuAddr)
    {
        HI_DRV_SMMU_Release(&(pstRgme->stMMUBuf));
    }
#else
    if (0 != pstRgme->stMMZBuf.u32StartPhyAddr)
    {
        HI_DRV_MMZ_Release(&(pstRgme->stMMZBuf));
    }
#endif
    memset(pstRgme,0,sizeof(VPSS_RGME_S));
    
    return HI_SUCCESS;
}
//获取回写队列中的参考帧
HI_S32 VPSS_RGME_GetRefInfo(VPSS_RGME_S *pstRgme,
                               HI_DRV_VIDEO_FRAME_S **pstDataList)
{    
    HI_U32 i;
    VPSS_WBC_MCDEI_DATA_S* pstRefNodeData = HI_NULL;
    LIST* pstRefNode = HI_NULL;
    VPSS_RGME_PREIMG_MODE_E enRefMode;
    HI_U32 u32NodeCount;

    if ((HI_NULL == pstRgme) || (HI_NULL == pstDataList))
    {
        VPSS_ERROR("Vpss rgme wbc GetRefInfo error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_FALSE == pstRgme->bInit)
    {
        VPSS_ERROR("Vpss rgme wbc GetRefInfo error(not init).\n");
        return HI_FAILURE;
    }

    enRefMode = pstRgme->stRgmeAttr.enRefMode;
    u32NodeCount = VPSS_RGME_WBC_MAX_NODE - 1;
    pstRefNode = pstRgme->pstFirstRef;

    for (i = 0; i < u32NodeCount; i++)
    {
        pstRefNodeData = list_entry(pstRefNode, VPSS_WBC_MCDEI_DATA_S, node);
        pstDataList[i] = &(pstRefNodeData->stWbcFrame);

        pstRefNode = pstRefNode->next;
    }
#if 0
    //first frame we use mmz info,we init buffer zero
    for (i = pstRgme->u32CompleteCount; i < u32NodeCount; i++)
    {
        if (VPSS_RGME_PREIMG_MODE_INIT == enRefMode)
        {
            pstDataList[u32NodeCount - i - 1] = HI_NULL;
        }
    }
#endif
    return HI_SUCCESS;

}

//获取回写队列中的回写帧
HI_S32 VPSS_RGME_GetWbcInfo(VPSS_RGME_S *pstRgme,HI_DRV_VIDEO_FRAME_S **pstData)
{
    VPSS_WBC_MCDEI_DATA_S* pstWbcNodeData = HI_NULL;
    LIST* pstWbcNode = HI_NULL;

    if ((HI_NULL == pstRgme) || (HI_NULL == pstData))
    {
        VPSS_ERROR("Vpss rgme wbc GetWbcInfo error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_FALSE == pstRgme->bInit)
    {
        VPSS_ERROR("Vpss rgme wbc GetWbcInfo error(not init).\n");
        return HI_FAILURE;
    }

    pstWbcNode = pstRgme->pstFirstRef->prev;
    pstWbcNodeData = list_entry(pstWbcNode, VPSS_WBC_MCDEI_DATA_S, node);
    *pstData = &(pstWbcNodeData->stWbcFrame);

    return HI_SUCCESS;
}
//设置回写队列轮转
HI_S32 VPSS_RGME_CompleteImage(VPSS_RGME_S* pstRgme)
{
    if (HI_NULL == pstRgme)
    {
        VPSS_ERROR("Vpss rgme wbc complete error(null pointer).\n");
        return HI_FAILURE;
    }
    
    if (HI_FALSE == pstRgme->bInit)
    {
        VPSS_ERROR("Vpss rgme wbc complete error(not init).\n");
        return HI_FAILURE;
    }

    if (0 == (pstRgme->u32CompleteCount + 1))
    {
        pstRgme->u32CompleteCount = VPSS_RGME_WBC_MAX_NODE;
    }

    pstRgme->u32CompleteCount++;
    pstRgme->pstFirstRef = pstRgme->pstFirstRef->next;

    return HI_SUCCESS;

}
HI_S32 VPSS_RGME_Reset(VPSS_RGME_S* pstRgme)
{
    if (HI_NULL == pstRgme)
    {
        VPSS_WARN("Vpss rgme wbc reset error(null pointer).\n");
        return HI_FAILURE;
    }
    
    if (HI_FALSE == pstRgme->bInit)
    {
        VPSS_WARN("Vpss rgme wbc reset error(not init).\n");
        return HI_FAILURE;
    }
#ifdef HI_VPSS_SMMU_SUPPORT
    if(0 != pstRgme->stMMUBuf.pu8StartVirAddr)
    {
        memset((HI_U8*)pstRgme->stMMUBuf.pu8StartVirAddr,0,pstRgme->stMMUBuf.u32Size);
    }
#else
    if(0 != pstRgme->stMMZBuf.pu8StartVirAddr)
    {
        memset((HI_U8*)pstRgme->stMMZBuf.pu8StartVirAddr,0,pstRgme->stMMZBuf.u32Size);
    }
#endif
    pstRgme->u32CompleteCount = 0;

    return HI_SUCCESS;
}


///////////////////////////////////////////////////////////////////





HI_S32 VPSS_BLEND_Init(VPSS_BLEND_S *pstBlend,VPSS_BLEND_ATTR_S *pstAttr)
{    
    HI_S32 s32Ret = HI_FAILURE;    
    HI_U32 u32Stride = 0;
    HI_U32 u32NodeBuffSize = 0;
    HI_U32 u32TotalBuffSize = 0;
    HI_U32 u32PhyAddr = 0;
    LIST* pListHead = HI_NULL; 
    VPSS_WBC_ATTR_S stCalc;
    
    if (HI_NULL == pstBlend)
    {
        VPSS_ERROR("Vpss blend wbc Init error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == pstBlend->bInit)
    {
        (HI_VOID)VPSS_BLEND_DeInit(pstBlend);
    }

    memset(pstBlend, 0, sizeof(VPSS_BLEND_S));
    VPSS_SAFE_MEMCPY(&pstBlend->stBlendAttr, pstAttr, sizeof(VPSS_BLEND_ATTR_S));    
    memset(&stCalc, 0x0, sizeof(stCalc));

    stCalc.u32Width = pstAttr->u32Width;
    stCalc.u32Height = pstAttr->u32Height;
    stCalc.enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
    stCalc.ePixFormat = pstAttr->ePixFormat;

    (HI_VOID)VPSS_WBC_CalBufSize(&u32NodeBuffSize, &u32Stride, &stCalc);

#if 1 //kill codecc warning
    u32TotalBuffSize = u32NodeBuffSize;
#else
    if(VPSS_BLEND_BUFMODE_LOWMEM == pstAttr->enBufMode)
    {
        u32TotalBuffSize = u32NodeBuffSize;
    }
    else
    {
        u32TotalBuffSize = u32NodeBuffSize;
    }
#endif

#ifdef HI_VPSS_SMMU_SUPPORT
    if (pstAttr->bSecure)
    {
#ifdef HI_TEE_SUPPORT
        s32Ret = HI_DRV_SECSMMU_Alloc("VPSS_BlendWbcBuf", u32TotalBuffSize,0, &pstBlend->stTEEBuf);
#else
        s32Ret = HI_DRV_SMMU_Alloc("VPSS_BlendWbcBuf",u32TotalBuffSize, 0, &(pstBlend->stTEEBuf));
#endif		
        if (HI_FAILURE == s32Ret)
        {
            VPSS_FATAL("VPSS blend WBC Alloc memory failed.\n");
            return HI_FAILURE;
        }
        u32PhyAddr = pstBlend->stTEEBuf.u32StartSmmuAddr;    
    }
    else
    {
        s32Ret = HI_DRV_SMMU_Alloc("VPSS_BlendWbcBuf",u32TotalBuffSize, 0, &(pstBlend->stMMUBuf));
        if (HI_FAILURE == s32Ret)
        {
            VPSS_FATAL("VPSS blend WBC Alloc memory failed.\n");
            return HI_FAILURE;
        }
        u32PhyAddr = pstBlend->stMMUBuf.u32StartSmmuAddr;    
    }
    
#else
    s32Ret = HI_DRV_MMZ_Alloc( "VPSS_BlendWbcBuf", HI_NULL, u32TotalBuffSize, 0, &(pstBlend->stMMZBuf));
    if (HI_FAILURE == s32Ret)
    {
        VPSS_FATAL("VPSS blend WBC Alloc memory failed.\n");
        return HI_FAILURE;
    }
    u32PhyAddr = pstBlend->stMMZBuf.u32StartPhyAddr;
#endif

    pListHead = &(pstBlend->stDataList[0].node);
    INIT_LIST_HEAD(pListHead);
    pstBlend->pstFirstRef = pListHead;

    if(VPSS_BLEND_BUFMODE_LOWMEM != pstAttr->enBufMode)
    {
        list_add_tail(&(pstBlend->stDataList[1].node), pListHead);
    }
    
    VPSS_WBC_InitFrame(&(pstBlend->stDataList[0].stWbcFrame), &stCalc, u32PhyAddr);
    VPSS_WBC_InitFrame(&(pstBlend->stDataList[1].stWbcFrame), &stCalc, u32PhyAddr + u32Stride);

    
    pstBlend->bInit = HI_TRUE;
    
    return HI_SUCCESS;
}

HI_S32 VPSS_BLEND_DeInit(VPSS_BLEND_S* pstBlend)
{    
    if (HI_NULL == pstBlend)
    {
        VPSS_ERROR("Vpss blend wbc DeInit error(null pointer).\n");
        return HI_FAILURE;
    }
    if (HI_FALSE == pstBlend->bInit)
    {
        VPSS_WARN("Vpss blend wbc DeInit error(not init).\n");
        //return HI_SUCCESS;
    }    
    
    if (0 != pstBlend->stTEEBuf.u32StartSmmuAddr)
    {
#ifdef HI_TEE_SUPPORT
        (HI_VOID)HI_DRV_SECSMMU_Release(&pstBlend->stTEEBuf);
#else
        HI_DRV_SMMU_Release(&(pstBlend->stTEEBuf));
#endif
    }

#ifdef HI_VPSS_SMMU_SUPPORT
    if (0 != pstBlend->stMMUBuf.u32StartSmmuAddr)
    {
        HI_DRV_SMMU_Release(&(pstBlend->stMMUBuf));
    }
#else
    if (0 != pstBlend->stMMZBuf.u32StartPhyAddr)
    {
        HI_DRV_MMZ_Release(&(pstBlend->stMMZBuf));
    }
    
#endif

    memset(pstBlend,0,sizeof(VPSS_BLEND_S));
    
    return HI_SUCCESS;

}

//获取回写队列中的参考帧
HI_S32 VPSS_BLEND_GetRefInfo(VPSS_BLEND_S *pstBlend,
                              HI_DRV_VIDEO_FRAME_S **pstDataList)
{
    HI_U32 i;    
    VPSS_WBC_MCDEI_DATA_S* pstRefNodeData = HI_NULL;
    LIST* pstRefNode = HI_NULL;
    HI_U32 u32NodeCount;

    if ((HI_NULL == pstBlend) || (HI_NULL == pstDataList))
    {
        VPSS_ERROR("Vpss blend wbc GetRefInfo error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_FALSE == pstBlend->bInit)
    {
        VPSS_ERROR("Vpss blend wbc GetRefInfo error(not init).\n");
        return HI_FAILURE;
    }
    
    u32NodeCount = VPSS_BLEND_WBC_MAX_NODE - 1;
    pstRefNode = pstBlend->pstFirstRef;

    for (i = 0; i < u32NodeCount; i++)
    {
        pstRefNodeData = list_entry(pstRefNode, VPSS_WBC_MCDEI_DATA_S, node);
        pstDataList[i] = &(pstRefNodeData->stWbcFrame);

        pstRefNode = pstRefNode->next;
    }

    for (i = pstBlend->u32CompleteCount; i < u32NodeCount; i++)
    {
        //pstDataList[u32NodeCount - i - 1] = HI_NULL;
    }

    return HI_SUCCESS;

}

//获取回写队列中的回写帧
HI_S32 VPSS_BLEND_GetWbcInfo(VPSS_BLEND_S *pstBlend,HI_DRV_VIDEO_FRAME_S **pstData)
{
    VPSS_WBC_MCDEI_DATA_S* pstWbcNodeData = HI_NULL;
    LIST* pstWbcNode = HI_NULL;

    if ((HI_NULL == pstBlend) || (HI_NULL == pstData))
    {
        VPSS_ERROR("Vpss blend wbc GetWbcInfo error(null pointer).\n");
        return HI_FAILURE;
    }

    if (HI_FALSE == pstBlend->bInit)
    {
        VPSS_ERROR("Vpss blend wbc GetWbcInfo error(not init).\n");
        return HI_FAILURE;
    }

    pstWbcNode = pstBlend->pstFirstRef->prev;
    pstWbcNodeData = list_entry(pstWbcNode, VPSS_WBC_MCDEI_DATA_S, node);
    *pstData = &(pstWbcNodeData->stWbcFrame);

    return HI_SUCCESS;
}
//设置回写队列轮转
HI_S32 VPSS_BLEND_CompleteImage(VPSS_BLEND_S* pstBlend)
{    
    if (HI_NULL == pstBlend)
    {
        VPSS_ERROR("Vpss blend wbc complete error(null pointer).\n");
        return HI_FAILURE;
    }
    
    if (HI_FALSE == pstBlend->bInit)
    {
        VPSS_ERROR("Vpss blend wbc complete error(not init).\n");
        return HI_FAILURE;
    }

    if (0 == (pstBlend->u32CompleteCount + 1))
    {
        pstBlend->u32CompleteCount = VPSS_BLEND_WBC_MAX_NODE;
    }

    pstBlend->u32CompleteCount++;
    pstBlend->pstFirstRef = pstBlend->pstFirstRef->next;

    return HI_SUCCESS;
}
HI_S32 VPSS_BLEND_Reset(VPSS_BLEND_S* pstBlend)
{
    if (HI_NULL == pstBlend)
    {
        VPSS_WARN("Vpss blend wbc reset error(null pointer).\n");
        return HI_FAILURE;
    }
    
    if (HI_FALSE == pstBlend->bInit)
    {
        VPSS_WARN("Vpss blend wbc reset error(not init).\n");
        return HI_FAILURE;
    }
    pstBlend->u32CompleteCount = 0;

    return HI_SUCCESS;
}

///////////////////////////////////////////////////////////////////






#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

