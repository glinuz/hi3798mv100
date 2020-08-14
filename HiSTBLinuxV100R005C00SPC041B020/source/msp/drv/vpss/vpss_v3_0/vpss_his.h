
#ifndef __VPSS_HIS_H__
#define __VPSS_HIS_H__

#include "hi_drv_video.h"
#include "vpss_common.h"
#include "vpss_osal.h"


typedef struct
{
    VPSS_MEM_S stMemBuf;
    HI_U32 u32MadMvAddr[3];
}HIS_MAD_MEM_S;

typedef struct
{
    HIS_MAD_MEM_S stMadHisInfo;
}VPSS_HIS_INFO_S;

typedef struct
{
    HI_U32 u32WPhyAddr;
    HI_U32 u32RPhyAddr;
    HI_U32 u32Stride;
}VPSS_HIS_ADDR_S;

HI_S32 VPSS_HIS_Init(VPSS_HIS_INFO_S *pstHisInfo,HI_BOOL bSecure);

HI_S32 VPSS_HIS_DeInit(VPSS_HIS_INFO_S *pstHisInfo);

HI_S32 VPSS_HIS_GetAddr(VPSS_HIS_INFO_S *pstHisInfo,VPSS_HIS_ADDR_S *pstAddr);
#endif
