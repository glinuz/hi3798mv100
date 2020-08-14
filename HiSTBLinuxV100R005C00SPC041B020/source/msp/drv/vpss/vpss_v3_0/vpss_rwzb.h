
#ifndef __VPSS_RWZB_H__
#define __VPSS_RWZB_H__

#include "vpss_alg_rwzb.h"
#include "hi_drv_video.h"
typedef struct hiVPSS_RWZB_S{
    HI_U32 u32Rwzb;
    HI_BOOL bInit;
    HI_U8 u8RwzbData[MAXSAMPNUM][PIX_NUM_IN_PATTERN];
    DET_INFO_S stDetInfo;
}VPSS_RWZB_S;

typedef struct hiVPSS_RWZB_IMG_S{
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_DRV_FIELD_MODE_E enFieldMode;
    HI_BOOL bProgressive;
}VPSS_RWZB_IMG_S;

HI_S32 VPSS_RWZB_Init(VPSS_RWZB_S *pstRwzb);
HI_S32 VPSS_RWZB_DeInit(VPSS_RWZB_S *pstRwzb);


HI_S32 VPSS_RWZB_GetRwzbData(VPSS_RWZB_S *pstRwzb,VPSS_RWZB_INFO_S *pstRwzbInfo);

HI_S32 VPSS_RWZB_GetRwzbInfo(VPSS_RWZB_S *pstRwzb,
                              VPSS_RWZB_INFO_S* pstRwzbInfo,
                              VPSS_RWZB_IMG_S* pstImage);

HI_S32 VPSS_RWZB_GetRwzbType(VPSS_RWZB_S *pstRwzb,HI_U32 *pu32Type);






#endif

