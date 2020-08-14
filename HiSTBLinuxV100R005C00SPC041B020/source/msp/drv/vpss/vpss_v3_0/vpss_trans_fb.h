#ifndef __DRV_TRANS_FB_H__
#define __DRV_TRANS_FB_H__
#include "vpss_common.h"
#include "hi_drv_vpss.h"

typedef struct hiVPSS_TRANS_FB_INFO_S{
    HI_BOOL bInit;
	HI_BOOL bNeedTrans;
    HI_VOID * pstInstance; 

}VPSS_TRANS_FB_INFO_S;

HI_S32 VPSS_TRANS_FB_Init(VPSS_TRANS_FB_INFO_S *pstTransFbList, HI_VOID* pstInstance);
HI_S32 VPSS_TRANS_FB_DeInit(VPSS_TRANS_FB_INFO_S *pstTransFbList);
HI_S32 VPSS_TRANS_FB_PutImage(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_TRANS_FB_RelImage(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_TRANS_FB_Reset(VPSS_TRANS_FB_INFO_S *pstTransFbList);
HI_BOOL VPSS_TRANS_FB_NeedTrans(VPSS_TRANS_FB_INFO_S *pstTransFbList,HI_DRV_VIDEO_FRAME_S *pstFrame);




#endif


