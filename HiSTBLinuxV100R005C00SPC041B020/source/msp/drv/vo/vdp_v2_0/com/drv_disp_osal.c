
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_osal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


HI_VOID DISP_OS_GetTime(HI_U32 *t_ms)
{
#ifdef __DISP_PLATFORM_SDK__

    HI_DRV_SYS_GetTimeStampMs(t_ms);
	return;
#else

	return;
#endif


}

HI_S32  DISP_OS_MMZ_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;
    HI_S32 nRet;

    nRet = HI_DRV_MMZ_Alloc(bufname, zone_name, size, align, &stMMZ);
    if (!nRet)
    {
        pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->u32StartVirAddr = (HI_U32)stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size = stMMZ.u32Size;
    }

    //HI_PRINT("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return nRet;
#else

    return HI_FAILURE;
#endif
}

HI_S32  DISP_OS_MMZ_Map( DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;
    HI_S32 nRet;

    memset((void*)&stMMZ, 0, sizeof(MMZ_BUFFER_S));
    
    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr ;
        
    nRet =HI_DRV_MMZ_Map(&stMMZ);
    if (!nRet)
    {
        pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->u32StartVirAddr = (HI_U32)stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size = stMMZ.u32Size;
    }

    //HI_PRINT("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return nRet;
#else

    return HI_FAILURE;
#endif
}

HI_S32  DISP_OS_MMZ_UnMap( DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;
    HI_S32 nRet = HI_SUCCESS;

    stMMZ.pu8StartVirAddr = (HI_U8 *)pstMBuf->u32StartVirAddr ;
        
    HI_DRV_MMZ_Unmap(&stMMZ);


    //HI_PRINT("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return nRet;
#else

    return HI_FAILURE;
#endif
}
HI_VOID DISP_OS_MMZ_Release(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;

    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
    stMMZ.pu8StartVirAddr =	(HI_U8 *)pstMBuf->u32StartVirAddr;
    stMMZ.u32Size = pstMBuf->u32Size;

    HI_DRV_MMZ_Release(&stMMZ);	

    return;
#else

    return;
#endif
}


HI_S32 DISP_OS_MMZ_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;
    HI_S32 nRet;

    nRet = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, size, align, &stMMZ);
    if (!nRet)
    {
        pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->u32StartVirAddr = (HI_U32)stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size = stMMZ.u32Size;
    }

    //HI_PRINT("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return nRet;
#else

    return HI_FAILURE;
#endif
}

HI_VOID DISP_OS_MMZ_UnmapAndRelease(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    MMZ_BUFFER_S stMMZ;

    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
    stMMZ.pu8StartVirAddr =	(HI_U8 *)pstMBuf->u32StartVirAddr;
    stMMZ.u32Size = pstMBuf->u32Size;

    HI_DRV_MMZ_UnmapAndRelease(&stMMZ);	

    return;
#else

    return;
#endif
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




