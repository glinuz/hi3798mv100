/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : drv_tde_ext.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/
#ifndef  __DRV_TDE_EXT_H__
#define  __DRV_TDE_EXT_H__


#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */


#include "hi_type.h"
#include "hi_tde_type.h"
#include "hi_drv_dev.h"

typedef HI_S32  (*FN_TDE_Open)(HI_VOID);
typedef HI_S32  (*FN_TDE_Close)(HI_VOID);
typedef HI_S32  (*FN_TDE_BeginJob)(TDE_HANDLE *);
typedef HI_S32  (*FN_TDE_EndJob)(TDE_HANDLE, HI_BOOL, HI_U32, HI_BOOL, TDE_FUNC_CB, HI_VOID*);
typedef HI_S32  (*FN_TDE_CancelJob)(TDE_HANDLE);
typedef HI_S32  (*FN_TDE_WaitForDone)(TDE_HANDLE, HI_U32);
typedef HI_S32  (*FN_TDE_WaitAllDone)(HI_BOOL);
typedef HI_S32  (*FN_TDE_QuickCopy)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*);
typedef HI_S32  (*FN_TDE_QuickCopyEx)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*, HI_BOOL,HI_BOOL);
typedef HI_S32  (*FN_TDE_SingleBlitEx)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*,TDE2_OPT_S*,HI_BOOL,HI_BOOL);
typedef HI_S32  (*FN_TDE_QuickFill)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, HI_U32);
typedef HI_S32  (*FN_TDE_QuickResize)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*);
typedef HI_S32  (*FN_TDE_QuickFlicker)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*);
typedef HI_S32  (*FN_TDE_Blit)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*,
                          TDE2_RECT_S*, TDE2_OPT_S*);
typedef HI_S32  (*FN_TDE_MbBlit)(TDE_HANDLE, TDE2_MB_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_MBOPT_S*);
typedef HI_S32  (*FN_TDE_SolidDraw)(TDE_HANDLE, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_SURFACE_S*, TDE2_RECT_S*, TDE2_FILLCOLOR_S*, TDE2_OPT_S*);
typedef HI_S32  (*FN_TDE_SetDeflickerLevel)(TDE_DEFLICKER_LEVEL_E);
typedef HI_S32  (*FN_TDE_EnableRegionDeflicker)(HI_BOOL);
typedef HI_S32	(*FN_TDE_SetAlphaThresholdValue)(HI_U8 u8ThresholdValue);
typedef HI_S32  (*FN_TDE_SetAlphaThresholdState)(HI_BOOL bEnAlphaThreshold);
typedef HI_S32  (*FN_TDE_GetAlphaThresholdState)(HI_BOOL *pbEnAlphaThreshold);
typedef HI_S32  (*FN_TDE_CalScaleRect)(const TDE2_RECT_S*, const TDE2_RECT_S*, TDE2_RECT_S*, TDE2_RECT_S*);
typedef HI_S32  (*FN_TDE_Suspend)(PM_BASEDEV_S *, pm_message_t);
typedef HI_S32  (*FN_TDE_Resume)(PM_BASEDEV_S *);
typedef HI_S32  (*FN_TDE_LockWorkingFlag)(HI_SIZE_T *);
typedef HI_S32  (*FN_TDE_UnlockWorkingFlag)(HI_SIZE_T *);
typedef HI_S32  (*FN_TDE_GetWorkingFlag)(HI_BOOL *);

typedef struct
{
    FN_TDE_Open             pfnTdeOpen;
    FN_TDE_Close            pfnTdeClose;
    FN_TDE_BeginJob         pfnTdeBeginJob;
    FN_TDE_EndJob           pfnTdeEndJob;
    FN_TDE_CancelJob        pfnTdeCancelJob;
    FN_TDE_WaitForDone      pfnTdeWaitForDone;
    FN_TDE_WaitAllDone      pfnTdeWaitAllDone;
    FN_TDE_QuickCopy        pfnTdeQuickCopy;    
    FN_TDE_QuickFill        pfnTdeQuickFill;
    FN_TDE_QuickResize      pfnTdeQuickResize;
    FN_TDE_QuickFlicker     pfnTdeQuickFlicker;
    FN_TDE_Blit             pfnTdeBlit;
    FN_TDE_MbBlit           pfnTdeMbBlit;
    FN_TDE_SolidDraw        pfnTdeSolidDraw;
    FN_TDE_SetDeflickerLevel        pfnTdeSetDeflickerLevel;
    FN_TDE_EnableRegionDeflicker    pfnTdeEnableRegionDeflicker;
	FN_TDE_SetAlphaThresholdValue   pfnTdeSetAlphaThresholdValue;
	FN_TDE_SetAlphaThresholdState   pfnTdeSetAlphaThresholdState;
	FN_TDE_GetAlphaThresholdState   pfnTdeGetAlphaThresholdState;
    FN_TDE_CalScaleRect     pfnTdeCalScaleRect;
    FN_TDE_Suspend  pfnTdeSuspend;
    FN_TDE_Resume   pfnTdeResume;
    FN_TDE_QuickCopyEx  pfnTdeQuickCopyEx;
    FN_TDE_SingleBlitEx pfnTdeSingleBlitEx;
    FN_TDE_LockWorkingFlag pfnTdeLockWorkingFlag;
    FN_TDE_UnlockWorkingFlag pfnTdeUnlockWorkingFlag;
    FN_TDE_GetWorkingFlag pfnTdeGetWorkingFlag;
} TDE_EXPORT_FUNC_S;

HI_S32 TDE_DRV_ModInit(HI_VOID);
HI_VOID  TDE_DRV_ModExit(HI_VOID);
HI_S32 tde_init_module_k(HI_VOID);
HI_VOID tde_cleanup_module_k(HI_VOID);


 #ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__DRV_TDE_EXT_H__ */
