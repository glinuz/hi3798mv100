#ifndef __DRV_VIRTUAL_H__
#define __DRV_VIRTUAL_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_video.h"
#include "hi_drv_win.h"
#include "drv_disp_osal.h"
#include "drv_window.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define DEF_VIR_BUFFER_LENGTH 10

#define WinCheckVirWindow(hWin, pstWin) \
{                                    \
    pstWin = WinGetVirWindow(hWin);     \
    if (!pstWin)                      \
    {                                \
        WIN_ERROR("WIN is not exist!\n"); \
        return HI_ERR_VO_WIN_NOT_EXIST; \
    }  \
}
typedef enum tagMUTUAL_TYPE_E
{
    MUTUAL_TYPE_SRCACTIVE = 0, 
    MUTUAL_TYPE_SINKACTIVE,    
    MUTUAL_TYPE_BUTT
}MUTUAL_TYPE_E;
 
typedef struct tagVIR_BUFFER_S
{
    MUTUAL_TYPE_E enType;
    HI_DRV_VIDEO_FRAME_S stBufArray[DEF_VIR_BUFFER_LENGTH];
    HI_U32 u32Head;
    HI_U32 u32Tail;
}VIR_BUFFER_S;

typedef struct tagWIN_VIRTUAL_STAT_S
{
    HI_U32 u32SrcQTry;
    HI_U32 u32SrcQOK;
    HI_U32 u32SrcDQTry;
    HI_U32 u32SrcDQOK;

    HI_U32 u32SinkAcqTry;
    HI_U32 u32SinkAcqOK;
    HI_U32 u32SinkRlsTry;
    HI_U32 u32SinkRlsOK;
}WIN_VIRTUAL_STAT_S;

typedef struct tagVIRTUAL_S
{
    HI_U32 u32Index;

    /* state */
    HI_BOOL bEnable;
    HI_BOOL bMasked;

    HI_DRV_WIN_TYPE_E enType;
    
    MUTUAL_TYPE_E enBufType;
    VIR_BUFFER_S stBuffer;

    /*as display window ,store usrset attr*/
    HI_DRV_WIN_ATTR_S stAttrBuf;
    atomic_t bNewAttrFlag;

    /*sink module can change it*/
    HI_DRV_PIX_FORMAT_E ePixFormat;
    HI_U32 u32Height;
    HI_U32 u32Width;
    HI_U32 u32FrmRate;
    
    HI_DRV_ROT_ANGLE_E enRotation;
    HI_BOOL bHoriFlip;
    HI_BOOL bVertFlip;
    
    HI_HANDLE hSink;
    HI_VOID* pfnQueueFrm;
    HI_VOID* pfnDequeueFrame;
    
    HI_DRV_WIN_SRC_INFO_S stSrcInfo;

    WIN_VIRTUAL_STAT_S stFrameStat;
}VIRTUAL_S;



HI_S32 VIR_BUFFER_Init(VIR_BUFFER_S *pstBuffer);
HI_S32 VIR_BUFFER_DeInit(VIR_BUFFER_S *pstBuffer);
HI_S32 VIR_BUFFER_Reset(VIR_BUFFER_S *pstBuffer);
/*consumer*/
HI_S32 VIR_BUFFER_GetFrm(VIR_BUFFER_S *pstBuffer,HI_DRV_VIDEO_FRAME_S *pstFrm);
/*productor*/
HI_S32 VIR_BUFFER_AddFrm(VIR_BUFFER_S *pstBuffer,HI_DRV_VIDEO_FRAME_S *pstFrm);

HI_S32 WIN_VIR_Create(HI_DRV_WIN_ATTR_S *pWinAttr, VIRTUAL_S **ppstVirWin);
HI_S32 WIN_VIR_Destroy(VIRTUAL_S *pstVirWin);
HI_S32 WIN_VIR_Reset(VIRTUAL_S *pstVirWin);

HI_S32 WIN_VIR_GetFrm(VIRTUAL_S *pstVirWin,HI_DRV_VIDEO_FRAME_S *pstFrm);
HI_S32 WIN_VIR_RelFrm(VIRTUAL_S *pstVirWin,HI_DRV_VIDEO_FRAME_S *pstFrm);

HI_S32 WIN_VIR_AddNewFrm(VIRTUAL_S *pstVirWin,HI_DRV_VIDEO_FRAME_S *pstFrm);
HI_S32 WIN_VIR_AddUlsFrm(VIRTUAL_S *pstVirWin,HI_DRV_VIDEO_FRAME_S *pstFrm);

HI_S32 WIN_VIR_SendAttrToSource(VIRTUAL_S *pstVirWin);
HI_S32 WIN_VIR_SetSize(VIRTUAL_S *pstVirWin,HI_U32 u32Width,HI_U32 u32Height);
HI_S32 WIN_VIR_SetFrmRate(VIRTUAL_S *pstVirWin,HI_U32 u32FrmRate);
HI_S32 WIN_VIR_SetAttr(VIRTUAL_S *pstVirWin,HI_DRV_WIN_ATTR_S *pWinAttr);

HI_S32 WIN_VIR_AttachSink(VIRTUAL_S *pstVirWin,HI_HANDLE hSink);
HI_S32 WIN_VIR_DetachSink(VIRTUAL_S *pstVirWin,HI_HANDLE hSink);

HI_BOOL WinCheckVirtual(HI_U32 u32WinIndex);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_WINDOW_H__  */