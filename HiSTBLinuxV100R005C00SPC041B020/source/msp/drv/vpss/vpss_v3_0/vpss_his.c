#include "vpss_his.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define HIS_MAD_MAX_WIDTH 1920
#define HIS_MAD_MAX_HEIGHT 1088


HI_S32 VPSS_HIS_FLUSHDATA(VPSS_MEM_S* pstMemBuf, HI_U32 u32Data)
{
    HI_U32 u32Numb;
    HI_U32 u32Count;
    HI_U32* pu32Pos;
    u32Numb = (pstMemBuf->u32Size + 3) / 4;

    pu32Pos = (HI_U32*)pstMemBuf->pu8StartVirAddr;

    for (u32Count = 0; u32Count < u32Numb; u32Count ++)
    {
        *pu32Pos = u32Data;
        pu32Pos = pu32Pos + 1;
    }

    return HI_SUCCESS;

}
HI_S32 VPSS_HIS_Init(VPSS_HIS_INFO_S *pstHisInfo,HI_BOOL bSecure)
{
    VPSS_MEM_S *pstMemBuf;
    HI_S32 nRet;
    HI_U32 ii;
    HI_U32 u32InfoSize;

    //128bit align, 4pixels occpy 2bytes.
    u32InfoSize = (((HIS_MAD_MAX_WIDTH + 31) & 0xffffffe0L) /2) * HIS_MAD_MAX_HEIGHT / 2;

    //apply memory for MAD motion-infomation, and get the address.
    pstMemBuf = &(pstHisInfo->stMadHisInfo.stMemBuf);
	
	if (!bSecure)
	{
		nRet = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,
				u32InfoSize*3,
				"VPSS",
				"VPSS_MADMotionInfoBuf",
				pstMemBuf);
	}
	else
	{
		nRet = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,
				u32InfoSize*3,
				"VPSS",
				"VPSS_MADMotionInfoBuf",
				pstMemBuf);
	}
    if (nRet != HI_SUCCESS)
    {
		VPSS_FATAL("Get VPSS_MADMotionInfoBuf failed.\n");
		return HI_FAILURE;
    }

	if (!bSecure)
	{
    	VPSS_HIS_FLUSHDATA(pstMemBuf, 0x007f007f);
	}

    for (ii = 0; ii < 3; ii++)
    {
        pstHisInfo->stMadHisInfo.u32MadMvAddr[ii] 
                    = pstMemBuf->u32StartPhyAddr + (u32InfoSize * ii);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HIS_DeInit(VPSS_HIS_INFO_S *pstHisInfo)
{
    HIS_MAD_MEM_S *pstMadMem;

    pstMadMem = &(pstHisInfo->stMadHisInfo);
    
    //release MAD motion-infomation memory 
    if (pstMadMem->stMemBuf.pu8StartVirAddr != 0) 
    {
		VPSS_OSAL_FreeMem(&(pstMadMem->stMemBuf));
        pstMadMem->stMemBuf.pu8StartVirAddr = 0;
        pstMadMem->stMemBuf.u32Size = 0;
    }
    else
    {
        VPSS_FATAL("Release MadBuf Error\n");
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HIS_GetAddr(VPSS_HIS_INFO_S *pstHisInfo,VPSS_HIS_ADDR_S *pstAddr)
{
    HIS_MAD_MEM_S *pstMadMem;
    
    HI_U32 *pu32Addr;
    HI_U32 i;
    
    HI_U32 u32Addrtmp;
    
    pstMadMem = &(pstHisInfo->stMadHisInfo);
    
    pstAddr->u32RPhyAddr = pstMadMem->u32MadMvAddr[2];
    pstAddr->u32WPhyAddr = pstMadMem->u32MadMvAddr[0];
    pstAddr->u32Stride = ((HIS_MAD_MAX_WIDTH + 31) & 0xffffffe0L) /2;

    //motion infor address
    pu32Addr = pstMadMem->u32MadMvAddr;
    
    u32Addrtmp = pu32Addr[2];
    for(i=2;i>0;i--)
    {
        pu32Addr[i] = pu32Addr[i-1];
    }
    pu32Addr[0] = u32Addrtmp;

    return HI_SUCCESS;
}                        
#ifdef __cplusplus
 #if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

