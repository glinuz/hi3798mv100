#ifndef __WIN_POLICY_H_
#define __WIN_POLICY_H_

#include "drv_display.h"

#include "drv_window.h"
#include "drv_win_policy.h"
#include "drv_win_priv.h"
#include "hi_drv_sys.h"
#include "hi_drv_stat.h"
#include "drv_vdec_ext.h"
#include "drv_disp_hal.h"
#include "drv_disp_alg_service.h"
#include "hi_drv_module.h"
#include "drv_pq_ext.h"


int Win_Pre_ScalerDistribute(WINDOW_S *pstWin,
                             HI_RECT_S *pstVpssZmeSize,
                             HI_RECT_S *pstFinalZmeSize,
                             HI_U32 u32WinNum,
                             HI_BOOL *pbHorSrEnable,
                             HI_BOOL *pbVerSrEnable,
                             HI_RECT_S *pstFmtResolution,
                             HI_DRV_DISP_STEREO_E enStereo);
int Win_Post_ScalerProcess( WINDOW_S *pstWin,
                            HI_RECT_S *pstFinalDisPosition,
                            HI_RECT_S *pstV0DisPosition,
                            HI_BOOL    *pbHorSrEnable,
                            HI_BOOL    *pbVerSrEnable,
                            HI_U32 u32WinNum,
                            HI_RECT_S *pstSourceFrameRect,
                            const HI_RECT_S *pstFmtResolution,
                            HI_DRV_DISP_STEREO_E enStereo);

HI_VOID Win_DciEnable_Policy(WINDOW_S *pstWin,
                         HI_RECT_S *pstOriginDciArea,/*vpss's effective area.*/
                         HI_RECT_S *pstV0DisPosition,/*secondary coordinate*/
                         HI_RECT_S *pstVpssGive,/*the original rect give by vpss.*/
                         HI_U32    u32WinNum);

HI_S32 Win_Revise_OutOfScreenWin_OutRect(HI_RECT_S *pstInRect,
                        HI_RECT_S *pstVideoRect, HI_RECT_S *pstDispRect,
                        HI_RECT_S stScreen,
                        HI_DRV_DISP_OFFSET_S stOffsetRect,
                        WIN_HAL_PARA_S *pstLayerPara);

HI_S32  Win_Get_WbcPointAndOutSize(HI_RECT_S *pstInputFrameSize,
                                HI_RECT_S *pstOutPutFrameSize,
                                HI_RECT_S *pstHdFmtRect,
                                HI_RECT_S *pstSdFmtRect,
                                HI_RECT_S *pstSdOutRect,
								HI_U32   u32WinNum,
                                HI_U32   *pu32WbcPoint,
								HI_BOOL bFieldRead,
								HI_BOOL bFieldOutput);

HI_S32 Win_Decide_WbcPoint(HI_BOOL  bHDVertZoomIn,/*whether exist horizical scale down.*/
		                            HI_BOOL  bHDHorZoomIn,                            
		                            HI_BOOL  bSdVertZoomIn,/*the same with hd.*/
		                            HI_BOOL  bSdHorZoomIn,                                     
		                            HI_U32  u32SDVertZoomScale,
		                            HI_U32  u32SDHorZoomScale,
		                            HI_U32   u32WinNum,
		                            HI_U32   *pu32WbcPoint);
HI_BOOL Win_Decide_LayerP2iOpenOrNot(HI_RECT_S *pstFrameRectSize,
                               HI_RECT_S *pstOutPutSize,
                               HI_BOOL  bInputFrameMode);

HI_BOOL Win_Decide_WbcP2iOpenOrNot(HI_RECT_S *pstFrameRectSize,
                                           HI_RECT_S *pstOutPutSize,
                                           HI_BOOL  bInputFrameMode,
                                           HI_DRV_FIELD_MODE_E eWbcInFieldMode_Out);


HI_S32 Win_TestDci(HI_VOID);
HI_S32 Win_TestPost(HI_VOID);
HI_S32 Win_TestSR(HI_VOID);

#endif




