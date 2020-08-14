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
                        HI_RECT_S *pstOutRect,
                        HI_RECT_S stScreen,
                        HI_DRV_DISP_OFFSET_S stOffsetRect,
                        WIN_HAL_PARA_S *pstLayerPara);
#if defined (CHIP_HIFONEB02)

HI_BOOL Win_DcmpShouldOpen(HI_BOOL bDcmp, HI_BOOL bMultiMode);
HI_BOOL Win_MuteLayer_Policy(HI_RECT_S *pstExpectRect,
                             HI_RECT_S *pstActualRect,
                             HI_BOOL   bZmeCapbility,
                             HI_DRV_PIX_FORMAT_E  eActualPixFormat,
                             HI_BOOL   bDcmpShouldOpen,
                             HI_DRV_PIXEL_BITWIDTH_E  enActualBitWidth,
                             HI_U32 u32CurrentMaxBitWidth);
HI_U32 Win_SupportedBitWidth(HI_U32 u32SupportMaxbitWidth, HI_BOOL bMultimode);
#endif
HI_S32 Win_TestDci(HI_VOID);
HI_S32 Win_TestPost(HI_VOID);
HI_S32 Win_TestSR(HI_VOID);

#endif




