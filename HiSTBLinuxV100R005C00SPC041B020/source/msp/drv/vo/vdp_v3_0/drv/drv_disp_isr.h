
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_isr.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_ISR_H__
#define __DRV_DISP_ISR_H__

#include "drv_disp_com.h"
#include "drv_disp_hal.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define DEF_DISP_ISR_LIST_LENGTH 20


typedef struct tagDISP_ISR_C_LIST_S
{
    HI_U32 u32NodeFlag;
    HI_DRV_DISP_CALLBACK_S stNode[DEF_DISP_ISR_LIST_LENGTH];
}DISP_ISR_C_LIST_S;

//typedef HI_VOID (*PF_DISP_PRE_PROC)(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);

typedef struct tagDISP_ISR_CHN_S
{
    HI_DRV_DISPLAY_E enDisp;
    volatile HI_BOOL bEnable;

    volatile DISP_ISR_C_LIST_S stList[HI_DRV_DISP_C_TYPE_BUTT];
    HI_DRV_DISP_CALLBACK_INFO_S stCBInfo;

    //volatile HI_BOOL bDispInfoUp;
    //volatile HI_BOOL bDispInfoIsSet;
    //volatile HI_DISP_DISPLAY_INFO_S stDispInfoNew;
}DISP_ISR_CHN_S;

typedef struct tagDISP_ISR_M_S
{
    DISP_ISR_CHN_S stDispChn[HI_DRV_DISPLAY_BUTT+1];
    HI_U32 u32ChnNumber;
    HI_U32 u32IntMaskSave4Suspend;
}DISP_ISR_M_S;

HI_S32 DISP_ISR_Init(HI_VOID);
HI_S32 DISP_ISR_DeInit(HI_VOID);

HI_S32 DISP_ISR_Suspend(HI_VOID);
HI_S32 DISP_ISR_Resume(HI_VOID);

HI_S32 DISP_ISR_OpenChn(HI_DRV_DISPLAY_E enDisp);
HI_S32 DISP_ISR_CloseChn(HI_DRV_DISPLAY_E enDisp);

HI_S32 DISP_ISR_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCB);
HI_S32 DISP_ISR_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCB);

HI_S32 DISP_ISR_SetEvent(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_EVENT_E eEvent);
HI_S32 DISP_ISR_SetDispInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstDispInfo);

HI_S32 DISP_ISR_Main(HI_S32 irq, HI_VOID *dev_id);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_ISR_H__  */










