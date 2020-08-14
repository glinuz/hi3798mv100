#include "vpss_rwzb.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


HI_S32 VPSS_RWZB_Init(VPSS_RWZB_S *pstRwzb)
{
    if (pstRwzb->bInit == HI_TRUE)
    {
        (HI_VOID)ALG_DeInitRwzbInfo(&(pstRwzb->stDetInfo));
        pstRwzb->u32Rwzb = PAT_UNKNOWN;
        pstRwzb->bInit = HI_FALSE;
        memset(pstRwzb->u8RwzbData,0,
                sizeof(HI_U8)*MAXSAMPNUM*PIX_NUM_IN_PATTERN);
    }
    
    (HI_VOID)ALG_InitRwzbInfo(&(pstRwzb->stDetInfo));
    pstRwzb->bInit = HI_TRUE;
    return HI_SUCCESS;
}
HI_S32 VPSS_RWZB_DeInit(VPSS_RWZB_S *pstRwzb)
{
    if (pstRwzb->bInit == HI_TRUE)
    {
        (HI_VOID)ALG_DeInitRwzbInfo(&(pstRwzb->stDetInfo));
        pstRwzb->u32Rwzb = PAT_UNKNOWN;
        pstRwzb->bInit = HI_FALSE;
    }
    

    return HI_SUCCESS;
}

HI_S32 VPSS_RWZB_GetRwzbData(VPSS_RWZB_S *pstRwzb,VPSS_RWZB_INFO_S *pstRwzbInfo)
{
    HI_U32 u32Count;
    
    for(u32Count = 0; u32Count < 6 ; u32Count ++)
    {
       memcpy(&(pstRwzbInfo->u8Data[u32Count][0]),
               &(pstRwzb->u8RwzbData[u32Count][0]),
               sizeof(HI_U8)*8);
       VPSS_INFO("dat%d0=%d dat%d1=%d dat%d2=%d dat%d3=%d dat%d4=%d dat%d5=%d dat%d6=%d dat%d7=%d\n",
       u32Count,pstRwzbInfo->u8Data[u32Count][0],
       u32Count,pstRwzbInfo->u8Data[u32Count][1],
       u32Count,pstRwzbInfo->u8Data[u32Count][2],
       u32Count,pstRwzbInfo->u8Data[u32Count][3],
       u32Count,pstRwzbInfo->u8Data[u32Count][4],
       u32Count,pstRwzbInfo->u8Data[u32Count][5],
       u32Count,pstRwzbInfo->u8Data[u32Count][6],
       u32Count,pstRwzbInfo->u8Data[u32Count][7]);
                
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_RWZB_GetRwzbInfo(VPSS_RWZB_S *pstRwzb,
                              VPSS_RWZB_INFO_S* pstRwzbInfo,
                              VPSS_RWZB_IMG_S* pstImage)
{
    if(pstImage->enFieldMode == HI_DRV_FIELD_TOP
       || pstImage->bProgressive == HI_TRUE)
    {
        pstRwzbInfo->u32EnRwzb = 0x1;
        pstRwzbInfo->u32Mode = 0x0;
        pstRwzbInfo->u32Width = pstImage->u32Width;
        pstRwzbInfo->u32Height = pstImage->u32Height;
		pstRwzbInfo->u32IsRwzb = pstRwzb->stDetInfo.isRWZB;
        
        ALG_DetPic(&(pstRwzb->stDetInfo),pstRwzbInfo);
        pstRwzb->u32Rwzb = pstRwzb->stDetInfo.isRWZB;
    }
    else
    {
        pstRwzbInfo->u32EnRwzb = 0x0;
        pstRwzbInfo->u32Mode = 0x0;
		pstRwzbInfo->u32IsRwzb = pstRwzb->stDetInfo.isRWZB;
    }
   
    return HI_SUCCESS;
}


HI_S32 VPSS_RWZB_GetRwzbType(VPSS_RWZB_S *pstRwzb,HI_U32 *pu32Type)
{
    *pu32Type = pstRwzb->u32Rwzb;
    //VPSS_INFO("---->type %d\n",pstRwzb->u32Rwzb);
    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

