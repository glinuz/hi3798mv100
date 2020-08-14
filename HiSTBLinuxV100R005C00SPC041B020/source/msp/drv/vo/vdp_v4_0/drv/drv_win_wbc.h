#ifndef __DRV_WIN_WBC_H__
#define __DRV_WIN_WBC_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_disp.h"


#include "drv_disp_osal.h"
#include "drv_win_buffer.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */



#define ISOGBNY_BUF_MULTIPLE_NUM       3
#define ISOGBNY_BUF_SINGL_NUM          1

#define ISOGBNY_BUFF_NUM_MAX     ISOGBNY_BUF_MULTIPLE_NUM

#define ISOGBNY_RECT_WIDTH       720
#define ISOGBNY_RECT_HEIGHT      576
#define ISOGBNY_MAX_STRIDE       720
#define WBC_MAX_STRIDE       720

typedef struct hiWBC_HD_WIN_INFOR_S{
    HI_BOOL         bV0Mute;
    HI_BOOL         bV0RegionEnable;
    HI_RECT_S       stV0InRect;/*video layer0 in Rect info */
    HI_RECT_S       stV0VideoRect;/*video layer0 Video Rect info */
    HI_RECT_S       stV0DispRect;/*video layer0 Disp Rect info */
    HI_DRV_COLOR_SPACE_E          enV0WinSrcColorSpace           ;
    HI_DRV_COLOR_SPACE_E          enV0WinDestColorSpace          ;

    HI_BOOL         bV0Secure;      /*video layer0 Secure info */

    HI_BOOL         bV1Secure;      /*video layer1 Secure info */
    HI_BOOL         bV1Mute;
    HI_BOOL         bV1RegionEnable;

    HI_U32 u32WinNum;       /*Disp1 Window Number */

}WBC_HD_WIN_INFOR_S;

typedef enum hiWBC_WB_FIELD_E
{
    WBC_WB_FIELD_AUTO = 0,
    WBC_WB_FIELD_FIELD,
    WBC_WB_FIELD_FRAME,
    WBC_WB_FIELD_BUTT
}WBC_WB_FIELD_E;

typedef enum hiWBC_WB_POINT_E
{
    WBC_WB_POINT_AUTO = 0,
    WBC_WB_POINT_VP,
    WBC_WB_POINT_ZME_F,
    WBC_WB_POINT_BUTT
}WBC_WB_POINT_E;

typedef enum hiISOGBNY_BUFFER_MODE_E
{
    ISOGBNY_BUFFER_MODE_SINGL_NONSEC = 0,
    ISOGBNY_BUFFER_MODE_SINGL_SEC,
    ISOGBNY_BUFFER_MODE_MULTIPLE_NONSEC,
    ISOGBNY_BUFFER_MODE_MULTIPLE_SEC,
    ISOGBNY_BUFFER_MODE_BUTT
}ISOGBNY_BUFFER_MODE_E;

typedef struct hi_ISOEBNY_THREAT_PROCESS_S
{
    WAIT_QUEUE_HAEAD    stWaitQueHead;
    THREAD  hThread;

    ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode;
    HI_BOOL bChangeProcess;
    HI_DRV_DISPLAY_E enDisp;

} ISOGBNY_THREAT_PROCESS_S;

typedef enum hiISOGBNY_BUFFER_STATUS_E
{
    ISOGBNY_BUFFER_STATUS_INIT = 0,
    ISOGBNY_BUFFER_STATUS_DEINIT,
    ISOGBNY_BUFFER_STATUS_BUTT
}ISOGBNY_BUFFER_STATUS_E;

typedef enum hiISOGBNY_FRAME_STATUS_E
{
    ISOGBNY_FRAME_STATUS_USED = 0,
    ISOGBNY_FRAME_STATUS_FREE,
    ISOGBNY_FRAME_STATUS_BUTT
}ISOGBNY_FRAME_STATUS_E;


typedef struct hi_ISOGBNY_BUFF_MANNGE_S
{
    HI_DRV_VIDEO_FRAME_S   *pstFrame;
    ISOGBNY_FRAME_STATUS_E enFrameStatus;

} ISOGBNY_BUFF_MANNGE_S;


typedef struct hi_ISOGBNY_SLAVE_INFO_S
{
    DISP_BUF_NODE_S *pstSalveNextNode;
    DISP_BUF_NODE_S *pstSalveCurrentNode;
    DISP_BUF_NODE_S *pstSalveDispNode;

    HI_DRV_VIDEO_FRAME_S *pstSalveCfgFrame;

    HI_U32 u32SalveCfgCount;
    HI_U32 u32SalveRecCount;
    HI_U32 u32SalveDisardCount;
    HI_U32 u32SalveRepeatCount;
    HI_U32 u32SalveReleaseCount;

} ISOGBNY_SLAVE_INFO_S;

typedef struct hi_ISOGBNY_SLAVE_CFG_S
{
    HI_BOOL bEnable;
    DISP_BUF_NODE_S *pstNextNode;
    DISP_BUF_NODE_S *pstCurrentNode;
    DISP_BUF_NODE_S *pstDispNode;

    HI_U32 u32CfgCount;
    HI_U32 u32RecCount;

    HI_U32 u32DisardCount;
    HI_U32 u32RepeatCount;
    HI_U32 u32ReleaseCount;

} ISOGBNY_SLAVE_CFG_S;


typedef struct hi_ISOGBNY_WBC0_INFO_S
{
    WBC_WB_POINT_E enWBCPoit;
    WBC_WB_FIELD_E enWBCField;
    HI_RECT_S   stWBCOutRect;

    HI_U32 u32WBCCfgCount;
    HI_U32 u32WBCFinishCount;
    HI_U32 u32WBCDisardCount;

} ISOGBNY_WBC0_INFO_S;

typedef struct hi_ISOGBNY_WBC_CFG_S
{

    HI_BOOL                        bWbcEnable ;
    HI_DRV_PIX_FORMAT_E           eOutPutPixelFmt;
    HI_DRV_FIELD_MODE_E           eFieldMode;
    HI_DRV_VID_FRAME_ADDR_S       stAddr;

    /**********************below is for  PQ's  policy.******************************/
    HI_RECT_S stHdWinInputSize;
    HI_RECT_S stHdWinOutputSize;
    HI_RECT_S stSdWinOutputSize;

    HI_DRV_COLOR_SPACE_E          enHdWinSrcColorSpace;
    HI_DRV_COLOR_SPACE_E          enHdWinDestColorSpace;


    HI_BOOL    bForceFieldMode; /*when lowdelay mode,  field mode is needed, or frame is needed.*/
    HI_BOOL    bComplexMode;/*means  should set wbc point to vp.*/

    HI_DRV_DISP_STEREO_E eDispMode;


    /**********************debug.******************************/
    WBC_WB_POINT_E enWBCPoit;
    //WBC_WB_FIELD_E enWBCField;
    HI_RECT_S   stWBCOutRect;
    DISP_BUF_NODE_S *pstWC0CfgNode;

    HI_U32 u32WBCCfgCount;
    HI_U32 u32WBCFinishCount;
    HI_U32 u32WBCDisardCount;

} ISOGBNY_WBC_CFG_S;

typedef struct hi_ISOGBNY_PROC_INFO_S
{
    HI_BOOL bChangeStatus;
    ISOGBNY_BUFFER_STATUS_E enIsogenyBuffStatus;
    ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode;
    WBC_WB_POINT_E enWBCForcePoint;     /*force WBC Point */
    WBC_WB_FIELD_E enWBForceField;  /*force WBC I/P/Auto */

    DISP_BUF_S *pstBuffer;
    ISOGBNY_WBC0_INFO_S stWBC0ProcInfo;
    ISOGBNY_SLAVE_INFO_S stSlaveProcInfo;
} ISOGBNY_PROC_INFO_S;

typedef struct hiISOGBNY_CFG_INFOR_S{
    HI_BOOL bIsogenyIint;
    HI_BOOL bChangeStatus;
    ISOGBNY_BUFFER_STATUS_E enIsogenyBuffStatus;
    ISOGBNY_BUFFER_MODE_E enIsogenyBuffMode;
    WBC_WB_POINT_E enWBCForcePoint;     /*force WBC Point */
    WBC_WB_FIELD_E enWBForceField;  /*force WBC I/P/Auto */

    ISOGBNY_THREAT_PROCESS_S stIsogenyThread;
    DISP_MMZ_BUF_S stIsogenyFrameMem;
    ISOGBNY_WBC_CFG_S stWBC0Cfg;

    ISOGBNY_SLAVE_CFG_S stSlaveCfg;
    DISP_BUF_S stBuffer;

    HI_DRV_VIDEO_FRAME_S   *pstBufArray[ISOGBNY_BUFF_NUM_MAX];
    HI_U32   u32FrameNum;
    ISOGBNY_BUFF_MANNGE_S  stFrameBuffStatus[ISOGBNY_BUF_MULTIPLE_NUM];
    ISOGBNY_PROC_INFO_S     stIsogenyProcInfo;

}ISOGBNY_CFG_INFOR_S;


HI_VOID ISOGENY_UpDateHDWinFramInfo(WBC_HD_WIN_INFOR_S *pstWinInfo,HI_U32 u32VideoLayer);

HI_S32 ISOGBNY_Init(HI_VOID);

HI_S32 ISOGBNY_DeInit(HI_VOID);

HI_VOID WIN_GetIsogenyPocInfor(ISOGBNY_PROC_INFO_S *pstIsogenyProcInfo);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
