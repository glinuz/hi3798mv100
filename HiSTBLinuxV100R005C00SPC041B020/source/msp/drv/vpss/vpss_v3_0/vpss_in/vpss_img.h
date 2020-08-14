
#ifndef __VPSS_IMG_H__
#define __VPSS_IMG_H__

#include "hi_drv_video.h"
#include "vpss_common.h"
#include "vpss_osal.h"

#define VPSS_SOURCE_MAX_NUMB 6

typedef HI_S32 (*PFN_IMG_FUNC)(HI_HANDLE hSrc,HI_DRV_VIDEO_FRAME_S *pstImage);

typedef struct hiVPSS_IMAGE_NODE_S{
    HI_DRV_VIDEO_FRAME_S stSrcImage;
    LIST node;
}VPSS_IMAGE_NODE_S;

typedef struct hiVPSS_IMAGELIST_STATE_S{
    HI_U32 u32TotalNumb;
    HI_U32 u32FulListNumb;
    HI_U32 u32EmptyListNumb;
    HI_U32 u32GetUsrTotal;
    HI_U32 u32GetUsrSuccess;
    
    HI_U32 u32RelUsrTotal;
    HI_U32 u32RelUsrSuccess;
    
    HI_U32 u32Target;
    HI_U32 u32FulList[VPSS_SOURCE_MAX_NUMB];
    HI_U32 u32EmptyList[VPSS_SOURCE_MAX_NUMB];
    HI_U32 u32List[VPSS_SOURCE_MAX_NUMB][2];
}VPSS_IMAGELIST_STATE_S;

typedef struct hiVPSS_IMAGELIST_INFO_S{
    VPSS_OSAL_LOCK stEmptyListLock;
    VPSS_OSAL_LOCK stFulListLock;
    HI_U32 u32GetUsrTotal;
    HI_U32 u32GetUsrSuccess;
    
    HI_U32 u32RelUsrTotal;
    HI_U32 u32RelUsrSuccess;
    
    LIST stEmptyImageList;
    LIST stFulImageList;
    LIST* pstTarget_1;

    HI_HANDLE hSrc;
    PFN_IMG_FUNC pfnRlsImage;
    PFN_IMG_FUNC pfnAcqImage;
}VPSS_IMAGELIST_INFO_S;

typedef struct hiVPSS_IMG_CALLBACK_S{
    HI_HANDLE hSrc;
    PFN_IMG_FUNC pfnRlsImage;
    PFN_IMG_FUNC pfnAcqImage;
}VPSS_IMG_CALLBACK_S;

HI_S32 VPSS_IMG_Init(VPSS_IMAGELIST_INFO_S *pstImgInfo);

HI_S32 VPSS_IMG_DeInit(VPSS_IMAGELIST_INFO_S *pstImgInfo);

HI_BOOL VPSS_IMG_CheckImageList(VPSS_IMAGELIST_INFO_S *pstImgInfo);
HI_BOOL VPSS_IMG_CheckEmptyNode(VPSS_IMAGELIST_INFO_S *pstImgInfo);

HI_S32 VPSS_IMG_GetProcessImg(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                                   HI_DRV_VIDEO_FRAME_S **ppstImg);

HI_S32 VPSS_IMG_AddNewImg(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                            HI_DRV_VIDEO_FRAME_S *pstNewImg);

HI_S32 VPSS_IMG_GetFieldAddr(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                            HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                            HI_DRV_BUF_ADDR_E eLReye); 

HI_S32 VPSS_IMG_Complete(VPSS_IMAGELIST_INFO_S *pstImgInfo);


HI_S32 VPSS_IMG_Regist(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                    VPSS_IMG_CALLBACK_S stCallback);      
                    
HI_S32 VPSS_IMG_CorrectListOrder(VPSS_IMAGELIST_INFO_S *pstImgInfo,
                    HI_BOOL bTopFirst);                  
                    
HI_S32 VPSS_IMG_Reset(VPSS_IMAGELIST_INFO_S *pstImgInfo);                    
#endif