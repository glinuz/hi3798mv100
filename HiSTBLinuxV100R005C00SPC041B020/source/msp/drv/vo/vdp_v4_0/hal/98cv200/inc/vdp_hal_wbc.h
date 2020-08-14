#ifndef __HAL_WBC_H__
#define __HAL_WBC_H__
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif
#include "vdp_fpga_define.h"
#include "vdp_define.h"
#include "vdp_ip_define.h"
#include "vdp_hal_comm.h"
#if (FPGA_TEST&VDP_CBB_FPGA)
#if FPGA_AUTORUN
#include "vdp_alg.h"
#endif
#endif

//WBC_G0_BEGIN
HI_VOID VDP_WBC_SetCmpEnable           (VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetCmpDrr              (VDP_LAYER_WBC_E enLayer, HI_U32 u32CmpDrr);
HI_VOID VDP_WBC_SetLossy               (VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetOffSet              (VDP_LAYER_WBC_E enLayer, HI_U32 u32Offset);

//-------------------------------------------------------------------
//WBC_BMP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_WBC_SetBmp_Thd             (VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_bmp_thd);

//-------------------------------------------------------------------
//WBC_DHD0_BEGIN
//-------------------------------------------------------------------

HI_VOID VDP_WBC_ConfigSmmuBypass   (VDP_LAYER_WBC_E enLayer,  HI_BOOL bBypass);
HI_VOID VDP_WBC_SetTunlCellAddr        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Addr);
HI_VOID VDP_WBC_SetLowdlyEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetLowdlyTestEnable    (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetWbPerLineNum        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Num);
HI_VOID VDP_WBC_SetPartfnsLineNum      (VDP_LAYER_WBC_E enLayer, HI_U32 u32Num);
HI_VOID VDP_WBC_HistogramEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_VtthdMode              (VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_vtthd_mode);

HI_VOID VDP_WBC_SetOflEnable           (HI_U32 bEnable);
HI_VOID VDP_WBC_SetOflPro              (HI_U32 u32Data);
HI_VOID VDP_WBC_SetOflDispMode         (HI_U32 u32Data);
HI_VOID VDP_WBC_SetOflMixerReso        (VDP_RECT_S stRect);
HI_VOID VDP_WBC_SetOflStart            (HI_VOID);
//new function for setting
HI_VOID VDP_WBC_SetCropPos             (VDP_LAYER_WBC_E enLayer, VDP_RECT_S stRect);
HI_VOID VDP_WBC_SetOutReso             (VDP_LAYER_WBC_E enLayer, VDP_RECT_S stRect);

HI_VOID VDP_WBC_SetEnable              (VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetDataWidth           (VDP_LAYER_WBC_E enLayer, HI_U32 u32DataWidth);
HI_VOID VDP_WBC_SetOutIntf             (VDP_LAYER_WBC_E enLayer, VDP_DATA_RMODE_E u32RdMode);
HI_VOID VDP_WBC_SetOutFmt              (VDP_LAYER_WBC_E enLayer, VDP_WBC_OFMT_E stIntfFmt);
HI_VOID VDP_WBC_SetSpd                 (VDP_LAYER_WBC_E enLayer, HI_U32 u32ReqSpd);
HI_VOID VDP_WBC_SetWlen                (VDP_LAYER_WBC_E enLayer, VDP_WBC_WLEN_E stWlen);
HI_VOID VDP_WBC_SetLayerAddr           (VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_WBC_SetLayerReso           (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_WBC_SetDitherMode          (VDP_LAYER_WBC_E enLayer, VDP_DITHER_E enDitherMode);
HI_VOID VDP_WBC_SetDitherRoundEnable   (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetDitherEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetDitherCoef          (VDP_LAYER_WBC_E enLayer, VDP_DITHER_COEF_S dither_coef);
HI_VOID VDP_WBC_SetCropReso            (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S stRect);
HI_VOID VDP_WBC_SetRegUp               (VDP_LAYER_WBC_E enLayer);
HI_VOID  VDP_WBC_SetRegUpAuto (VDP_LAYER_WBC_E enLayer,HI_U32 mm,VDP_CHN_E enChn);

HI_VOID VDP_WBC_SetZmeCoefAddr         (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr);
HI_VOID VDP_WBC_SetParaUpd             (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E enMode);
HI_VOID VDP_WBC_SetSfifo               (VDP_LAYER_WBC_E enLayer, HI_U32 u32Data );
HI_VOID VDP_WBC_SetZmeEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_WBC_SetMidEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
HI_VOID VDP_WBC_SetFirEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
HI_VOID VDP_WBC_SetZmeVerTap           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap);
HI_VOID VDP_WBC_SetZmeHfirOrder        (VDP_LAYER_WBC_E enLayer, HI_U32 u32HfirOrder);
HI_VOID VDP_WBC_SetZmeHorRatio         (VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio);
HI_VOID VDP_WBC_SetZmeInFmt            (VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_WBC_SetZmeOutFmt           (VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_WBC_SetZmeVerRatio         (VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio);
HI_VOID VDP_WBC_SetZmePhase            (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_S32 s32Phase);

HI_VOID VDP_WBC_SetCscEnable           (VDP_LAYER_WBC_E enLayer, HI_U32 enCSC);
HI_VOID VDP_WBC_SetCscDcCoef           (VDP_LAYER_WBC_E enLayer,VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_WBC_SetCscCoef             (VDP_LAYER_WBC_E enLayer,VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_WBC_SetCscMode             (VDP_LAYER_WBC_E enLayer, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_WBC_SetAutoSt              (VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetThreeMd             (VDP_LAYER_WBC_E enLayer, HI_U32 bMode);

HI_VOID VDP_WBC_SetCdsEnable           (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetCdsFirEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetCdsMidEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);

HI_VOID VDP_WBC_SetCdsCoef             (VDP_LAYER_WBC_E enLayer, HI_S32 * s32Coef);

HI_VOID VDP_WBC_SetFlipEnable          (HI_U32 u32LayerId, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetUVOrder             (HI_U32 u32LayerId, HI_U32 u32Enable);

HI_VOID VDP_WBC_SetDhdFlip             (HI_U32 u32LayerId, HI_U32 u32Data );
HI_VOID VDP_WBC_SetGpFlip              (HI_U32 u32LayerId, HI_U32 u32Data );

HI_VOID VDP_WBC_SetHorZmeEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetHorZmeMidEnable     (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetHorZmeFirEnable     (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);

HI_VOID VDP_WBC_SetProToInterEnable    (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable) ;
HI_VOID VDP_WBC_SetWbcVpConnection     (HI_U32 enLayer, VDP_LAYER_CONN_E WbcVpConnect);

//-------------------------------------------------------------------
//WBC_DHD0_END
//-------------------------------------------------------------------

//wbc loacate 
//HI_VOID VDP_WBC_SetDhdLocate           (HI_U32 u32Id, VDP_WBC_LOCATE_E  u32Data);
HI_VOID VDP_WBC_SetDhdOflEn           (HI_U32 u32Id, HI_U32  u32Data);


//3535 new add wbc 
HI_VOID VDP_WBC_SetDhdOflEn            (VDP_LAYER_WBC_E enLayer, HI_U32 u32Data);
HI_VOID VDP_WBC_SetDhdOflMode          (VDP_LAYER_WBC_E enLayer, HI_U32 u32Data);
HI_VOID VDP_VID_SetConnection          (HI_U32 u32LayerId, HI_U32 u32Data );
HI_VOID VDP_GFX_SetConnection          (HI_U32 u32LayerId, HI_U32 u32Data );
HI_VOID VDP_WBC_SetConnection          (HI_U32 u32LayerId, HI_U32 u32Data );
HI_VOID VDP_WBC_SetCoefReadEnable      (HI_U32 u32Id, HI_U32 u32Para);
HI_VOID VDP_WBC_SetCoefReadDisable     (HI_U32 u32Id, HI_U32 u32Para);

HI_VOID VDP_WBC_SetCmpMb               (VDP_LAYER_WBC_E enLayer, HI_U32 u32MbBits);
HI_VOID VDP_WBC_SetCmpMaxMin           (VDP_LAYER_WBC_E enLayer, HI_U32 u32MinBitsCnt, HI_U32 u32MaxBitsCnt);
HI_VOID VDP_WBC_SetCmpAdjThr           (VDP_LAYER_WBC_E enLayer, HI_U32 u32AdjSadThr, HI_U32 u32AdjSadBitThr,HI_U32 u32AdjSpecBitThr);
HI_VOID VDP_WBC_SetCmpBigGrad          (VDP_LAYER_WBC_E enLayer, HI_U32 u32BigGradThr, HI_U32 u32BigGradNumThr);
HI_VOID VDP_WBC_SetCmpBlk              (VDP_LAYER_WBC_E enLayer, HI_U32 u32SmthThr, HI_U32 u32BlkCompThr);
HI_VOID VDP_WBC_SetCmpGraphic          (VDP_LAYER_WBC_E enLayer, HI_U32 u32GrcEn, HI_U32 u32VideoSadThr);
HI_VOID VDP_WBC_SetCmpRateCtrl         (VDP_LAYER_WBC_E enLayer, HI_U32 u32MaxTrow, HI_U32 u32SmthGain, HI_U32 u32SadBitNgain);
HI_VOID VDP_WBC_SetCmpFrameSize        (VDP_LAYER_WBC_E enLayer, HI_U32 u32FrameSize);

HI_VOID VDP_WBC_SetMasterSel  (VDP_LAYER_WBC_E enVdpLayerWbc, VDP_MASTER_E enVdpMaster);
HI_VOID VDP_WBC_SetBtDebug     (VDP_LAYER_WBC_E enLayer, HI_U32 u32FmtSel);

#endif
