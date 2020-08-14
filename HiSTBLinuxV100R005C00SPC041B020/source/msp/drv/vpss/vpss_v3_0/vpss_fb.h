#ifndef __VPSS_BUFFER_H__
#define __VPSS_BUFFER_H__


#include <linux/list.h>
#include "hi_drv_mmz.h"
#include"drv_vpss_ext.h"
#include "vpss_osal.h"
#include "vpss_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define DEF_VPSS_2D_BUFFER_DEPEND_NUM 1
#define DEF_VPSS_3D_BUFFER_DEPEND_NUM 2


typedef struct hiVPSS_BUFFER_S{
    VPSS_MEM_S stMemBuf;
    HI_U32 u32Stride;
    MMZ_BUFFER_S stPrivDataBuf;
}VPSS_BUFFER_S;

typedef struct hiVPSS_FRAME_NODE_S{
    HI_DRV_VIDEO_FRAME_S stOutFrame;
    VPSS_BUFFER_S stBuffer;
    LIST node;
}VPSS_FB_NODE_S;

typedef struct hiVPSS_FB_INFO_S{
    HI_DRV_VPSS_BUFLIST_CFG_S  stBufListCfg;

    HI_U32 u32ExtNumb;
    HI_U32 u32ExtCnt;
    
    HI_U32 u32GetTotal;
    HI_U32 u32GetSuccess;
    
    HI_U32 u32RelTotal;
    HI_U32 u32RelSuccess;
    
    unsigned long ulStart; 
    HI_U32 u32GetHZ;
    HI_U32 u32GetLast;
    
    HI_U32 u32ListFul;
    VPSS_OSAL_SPIN stFulBufSpin;
    VPSS_OSAL_SPIN stEmptyBufSpin;
    LIST stEmptyFrmList;
    LIST stFulFrmList;

    
    VPSS_OSAL_SPIN stExtBufSpin;
    LIST stExtFrmList;

    LIST* pstTarget_1;
}VPSS_FB_INFO_S;

typedef struct hiVPSS_FB_STATE_S{
    HI_U32 u32TotalNumb;
    HI_U32 u32EmptyListNumb;
    HI_U32 u32FulListNumb;
    HI_U32 u32ExtListNumb;
    HI_U32 u32WaitSinkRlsNumb;
    
    HI_U32 u32GetTotal;
    HI_U32 u32GetSuccess;
    HI_U32 u32RelTotal;
    HI_U32 u32RelSuccess;
    HI_U32 u32GetHZ;
    
    HI_U32 u32ListFul;
    HI_U32 u32Target_1;
    HI_U32 u32OutRate;
    #if FB_DBG
    HI_U32 u32List[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER][2];
    HI_U32 u32FulList[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER];
    HI_U32 u32EmptyList[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER];
    #endif
}VPSS_FB_STATE_S;
typedef enum {
    VPSS_FB_TYPE_NORMAL = 0,
    VPSS_FB_TYPE_EXTERN,
    VPSS_FB_TYPE_BUTT
}VPSS_FB_TYPE_E;

HI_S32 VPSS_FB_Init(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VPSS_BUFLIST_CFG_S *pstBufListCfg);
HI_S32 VPSS_FB_DelInit(VPSS_FB_INFO_S *pstFrameList);

/*Consumer:Port*/
HI_S32 VPSS_FB_GetFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile);
HI_S32 VPSS_FB_RelFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_FB_RelFulFrmBufNoMmz(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame);

/*Producers:TASK*/
VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBufNoMmz(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame);
VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,
                            HI_U32 u32Height,HI_U32 u32Width,
                            HI_DRV_PIX_FORMAT_E ePixFormat,
							HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth,
							HI_BOOL bSecure);
HI_S32 VPSS_FB_AddFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode);
HI_S32 VPSS_FB_AddEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,
                                VPSS_FB_NODE_S *pstFBNode,
                                VPSS_FB_TYPE_E enType);

HI_BOOL VPSS_FB_CheckIsAvailable(VPSS_FB_INFO_S *pstFrameList,HI_U32  u32AvailableNum);


HI_S32 VPSS_FB_GetState(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_STATE_S *pstFbState);


HI_S32 VPSS_FB_Reset(VPSS_FB_INFO_S *pstFrameList);


HI_S32 VPSS_FB_AllocExtBuffer(VPSS_FB_INFO_S *pstFrameList,HI_U32 u32ExtNumb);   
HI_S32 VPSS_FB_RlsExtBuffer(VPSS_FB_INFO_S *pstFrameList);                               

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /* __VO_EXT_H__ */
