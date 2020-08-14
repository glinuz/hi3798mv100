#ifndef __HAL_VID_H__
#define __HAL_VID_H__

#include "vdp_define.h"
#include "vdp_ip_define.h"

#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif

// #if VDP_C_COMPILE
// #include "vdp_alg.h"
// #endif
//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetReqCtrl             (HI_U32 u32Data, HI_U32 u32ReqCtrl);
HI_VOID VDP_VID_SetReqMacCtrl          (HI_U32 u32Data, HI_U32 u32ReqMacCtrl);
HI_VOID VDP_VID_SetVfirOneTapEnable    (HI_U32 u32Data, HI_U32 u32VfirOneTapEn);


HI_VOID VDP_VID_SetPreReadEnable       (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetReqLength           (HI_U32 u32Data, HI_U32 ReqLenght );
HI_VOID VDP_VID_SetNoSecFlag           (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetFiLayerEnable       (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetTileStride          (HI_U32 u32Data, HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_VID_SetLayerAddr           (HI_U32 u32Data, HI_U32 u32Chan, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_VID_SetHeadAddr            (HI_U32 u32Data, HI_U32 u32LHeadAddr,HI_U32 u32CHeadAddr);
HI_VOID VDP_VID_SetHeadSize            (HI_U32 u32Data, HI_U32 u32HeadSize);
HI_VOID VDP_VID_SetHeadStride          (HI_U32 u32Data, HI_U32 u32HeadStride);
HI_VOID VDP_VID_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VID_SetInDataFmt           (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt);
HI_VOID VDP_VID_SetDataWidth           (HI_U32 u32Data, HI_U32 DataWidth);
HI_VOID VDP_VID_SetReadMode            (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode);
HI_VOID VDP_VID_SetMuteEnable          (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VID_SetMuteBkg             (HI_U32 u32Data, VDP_BKG_S stMuteBkg);
HI_VOID VDP_VID_SetFlipEnable          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetOutReso             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetSrOutReso           (HI_U32 u32Data, VDP_RECT_S  stRect);

//U V Order
HI_VOID VDP_VID_SetUvorder             (HI_U32 u32Data, HI_U32 u32bUvorder);

//DCMP
HI_VOID VDP_VID_SetDcmpEnable          (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetTileDcmpEnable      (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetDcmpOffset          (HI_U32 u32Data, HI_U32 u32laddr_offset , HI_U32  u32caddr_offset );
HI_VOID VDP_VID_SetDcmpErrorClr        (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);
HI_VOID VDP_VID_SetDcmpHeadErrClr      (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);
HI_VOID VDP_VID_SetDcmpConsumeErrClr   (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);
HI_VOID VDP_VID_SetDcmpForgiveErrClr   (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);

HI_VOID VDP_VID_SetIfirMode            (HI_U32 u32Data, VDP_IFIRMODE_E enMode);
HI_VOID VDP_VID_SetIfirCoef            (HI_U32 u32Data, HI_S32 * s32Coef);

HI_VOID VDP_VID_SetLayerGalpha         (HI_U32 u32Data, HI_U32 u32Alpha0);

HI_VOID VDP_VID_SetCropReso            (HI_U32 u32Data, VDP_RECT_S stRect);

HI_VOID VDP_VID_SetLayerBkg            (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_SetParaUpMode              (HI_U32 u32Data, HI_U32 u32Mode);

HI_VOID VDP_VID_SetCscDcCoef           (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_VID_SetCscCoef             (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_VID_SetCscEnable           (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_VID_SetCscMode             (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_VID_SetDispMode            (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);


HI_VOID VDP_SetWbcMd                   (HI_U32 enMdSel);
HI_VOID VDP_SetInDataWidth             (HI_U32 u32Data, HI_U32 u32idatawth);
HI_VOID VDP_SetWbcOutMode              (HI_U32 u32Data, VDP_DATA_RMODE_E enRdMode);
//HI_VOID VDP_SetTimeOut                 (HI_U32 u32Data, HI_U32 u32TData);
HI_VOID VDP_VID_SetDrawMode            (HI_U32 u32Data, HI_U32 u32ModeLuma, HI_U32 u32ModeChroma);
HI_VOID VDP_VID_SetDrawPixelMode       (HI_U32 u32Data, HI_U32 u32DrawPixelMode);


HI_VOID VDP_VID_SetMultiModeEnable     (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetRegionEnable        (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetAllRegionDisable    (HI_U32 u32Data );
HI_BOOL VDP_VID_CheckRegionState(HI_U32 u32Data);
HI_VOID VDP_VID_SetRegionAddr          (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_VID_SetRegionReso          (HI_U32 u32Data, HI_U32 u32Num, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetRegionMuteEnable    (HI_U32 u32Data, HI_U32 u32Num, HI_U32 bEnable);

HI_VOID VDP_VID_SetRegUp               (HI_U32 u32Data);

//-------------------------------------------------------------------
//ZME_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetZmeEnable           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetVtapReduce          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmeShootCtrlEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable,HI_U32 u32bMode);
HI_VOID VDP_VID_SetZmeShootCtrlPara    (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32Flatdect_mode,HI_U32 u32Coringadj_en,HI_U32 u32Gain,HI_U32 u32Coring);
HI_VOID VDP_VID_SetZmePhase            (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_VID_SetZmeFirEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmeMidEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmeHorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_VID_SetZmeCoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_VID_SetZmeInFmt            (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_VID_SetZmeOutFmt           (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID  VDP_VID_SetHdrElZme           (HI_U32 u32Data, HI_U32 u32bEnable);
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZmeNonLnrDelta     (HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta);
HI_VOID  VDP_VID_SetZmeNonLnrZoneEnd   (HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End);

HI_VOID VDP_VID_SetParaUpd             (HI_U32 u32Data, VDP_VID_PARA_E enMode);

#if VID_ZME2_EN
//-------------------------------------------------------------------
//ZME2_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetZme2Enable           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZme2VtapReduce          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZme2ShootCtrlEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable,HI_U32 u32bMode);
HI_VOID VDP_VID_SetZme2ShootCtrlPara    (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32Flatdect_mode,HI_U32 u32Coringadj_en,HI_U32 u32Gain,HI_U32 u32Coring);
HI_VOID VDP_VID_SetZme2Phase            (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_VID_SetZme2FirEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZme2MidEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZme2HorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZme2VerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZme2HfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_VID_SetZme2CoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_VID_SetZme2InFmt            (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_VID_SetZme2OutFmt           (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZme2NonLnrDelta     (HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta);
HI_VOID  VDP_VID_SetZme2NonLnrZoneEnd   (HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End);
HI_VOID  VDP_VID_SetZme2HCoef           (HI_U32 u32Data,  VDP_VID_ZME2_HCOEF_S stZme2HCoef);
HI_VOID  VDP_VID_SetZme2VCoef           (HI_U32 u32Data,  VDP_VID_ZME2_VCOEF_S stZme2VCoef);
#endif

HI_VOID  VDP_VID_SetCvfirVCoef(HI_U32 u32Data,  VDP_VID_CVFIR_VCOEF_S stCvfirVCoef);


//-------------------------------------------------------------------
//VID_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//VP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VP_SetThreeDimDofStep      (HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep);
HI_VOID VDP_VP_SetThreeDimDofBkg       (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_VP_SetLayerReso            (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VP_SetVideoPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetDispPos              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetInReso               (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetRegUp                (HI_U32 u32Data);
HI_VOID VDP_VP_SetLayerGalpha          (HI_U32 u32Data, HI_U32 u32Alpha);
HI_VOID VDP_VP_SetLayerBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_VP_SetMuteEnable           (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VP_SetMuteBkg              (HI_U32 u32Data, VDP_VP_MUTE_BK_S stVpMuteBkg);
HI_VOID VDP_VP_SetDispMode             (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);

//HI_VOID VDP_VP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode, HI_U32 u32Num);

HI_VOID VDP_VP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_VP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_VP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_VP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);
HI_VOID VDP_VP_SetPositionSel          (HI_U32 u32Data, HI_U32 u32Sel);
//-------------------------------------------------------------------
//VP_END
//-------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// DCI_BEGIN
//-----------------------------------------------------------------------------------
HI_VOID VDP_VP_SetDciEnable            (HI_U32 u32Data, HI_U32 u32DciEn);
HI_VOID VDP_VP_SetDciDbgEn             (HI_U32 u32Data, HI_U32 u32DciDbgEn);
HI_VOID VDP_VP_SetDciDbgMode           (HI_U32 u32Data, HI_U32 u32DciDbgMode);
HI_VOID VDP_VP_SetDciSceneFlg          (HI_U32 u32Data, HI_U32 u32SceneFlg);
HI_VOID VDP_VP_SetDciManAdj            (HI_U32 u32Data, VDP_VP_DCI_ADJ_NUM_E Adjnum, VDP_VP_DCI_CTRL_MODE_E Manmode);
HI_VOID VDP_VP_SetDciCbCrcmpEn         (HI_U32 u32Data, HI_U32 u32ClorEn);
HI_VOID VDP_VP_SetDciCbCrstaEn         (HI_U32 u32Data, HI_U32 u32ClorstaEn);
HI_VOID VDP_VP_SetDciRange             (HI_U32 u32Data, VDP_DCI_RANGE_E Rangeinout , HI_U32 u32Rangemode);
HI_VOID VDP_VP_SetDciShiftCtrl         (HI_U32 u32Data, VDP_DCI_SHIFT_E enShiftCtrl);
HI_VOID VDP_VP_SetDciLpfEn             (HI_U32 u32Data, HI_U32 u32enLpf);
HI_VOID VDP_VP_SetDciSwcfgEn           (HI_U32 u32Data, HI_U32 u32enSwcfg);
HI_VOID VDP_VP_SetDciHistWgtEn         (HI_U32 u32Data, HI_U32 u32enSwcfg);
HI_VOID VDP_VP_SetDciStatCtrl          (HI_U32 u32Data, HI_U32 u32StatCtrl);
HI_VOID VDP_VP_SetDciYLinear           (HI_U32 u32Data, HI_U32 u32DciYLinear);//dci add y_linear

HI_VOID VDP_VP_SetDciPosition          (HI_U32 u32Data, VDP_DCI_POS_S stDciPos);
HI_VOID VDP_VP_SetDciHisBld            (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciBsLimit           (HI_U32 u32Data,HI_U32  u32BsLimit);
HI_VOID VDP_VP_SetDciHisOft            (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciLimitAblend       (HI_U32 u32Data, HI_U32 u32BsLimitAblend);
HI_VOID VDP_VP_SetDciHisCor            (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciMerBld            (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciAdjWgt            (HI_U32 u32Data, VDP_VP_DCI_ADJ_NUM_E Adjnum, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciClip              (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciGlbGain           (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);

HI_VOID VDP_VP_SetDciPos               (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);
HI_VOID VDP_VP_SetDciNeg               (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);

HI_VOID VDP_VP_SetDciAdjSW             (HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara);

HI_VOID VDP_VP_SetDciCoefAddr          (HI_U32 u32Data, HI_U32 u32CoefAddr);

HI_VOID VDP_VP_SetDciDcivsRdEn          ( HI_U32 u32Data, HI_U32 dcivs_rd_en);
HI_VOID VDP_VP_SetDciDcibsRdEn          ( HI_U32 u32Data, HI_U32 dcibs_rd_en);
HI_VOID VDP_VP_SetDciDcilutRdEn         ( HI_U32 u32Data, HI_U32 dcilut_rd_en);
HI_VOID VDP_VP_SetDciDcidivRdEn         ( HI_U32 u32Data, HI_U32 dcidiv_rd_en);
HI_VOID VDP_VP_SetDciDcihbwRdEn         ( HI_U32 u32Data, HI_U32 dcihbw_rd_en);
HI_VOID VDP_VP_SetDciParaData           ( HI_U32 u32Data, HI_U32 para_data);
HI_VOID VDP_VP_SetDciDciSplitPoint      ( HI_U32 u32Data, HI_U32 dci_split_point);
HI_VOID VDP_VP_SetDciDciSplitType       ( HI_U32 u32Data, HI_U32 dci_split_type);
HI_VOID VDP_VP_SetDciDciSplitHistMode   ( HI_U32 u32Data, HI_U32 dci_split_hist_mode);
HI_VOID VDP_VP_SetDciDciManAlgBlend     ( HI_U32 u32Data, HI_U32 dci_man_alg_blend);
HI_VOID VDP_VP_SetDciDciManAlgBlendEn   ( HI_U32 u32Data, HI_U32 dci_man_alg_blend_en);
HI_VOID VDP_VP_SetDciDciFlatThrsh       ( HI_U32 u32Data, HI_U32 dci_flat_thrsh);
HI_VOID VDP_VP_SetDciDciFlatMode        ( HI_U32 u32Data, HI_U32 dci_flat_mode);
HI_VOID VDP_VP_SetDciDciHistMapBlend    ( HI_U32 u32Data, HI_U32 dci_hist_map_blend);
HI_VOID VDP_VP_SetDciDciClipLimitR      ( HI_U32 u32Data, HI_U32 dci_clip_limit_r);
HI_VOID VDP_VP_SetDciDciClipLimitL      ( HI_U32 u32Data, HI_U32 dci_clip_limit_l);
HI_VOID VDP_VP_SetDciDciBsMinBlackThr   ( HI_U32 u32Data, HI_U32 dci_bs_min_black_thr);
HI_VOID VDP_VP_SetDciDciBsMinBlackBlend ( HI_U32 u32Data, HI_U32 dci_bs_min_black_blend);
HI_VOID VDP_VP_SetDciDciBsMaxLevel      ( HI_U32 u32Data, HI_U32 dci_bs_max_level);
HI_VOID VDP_VP_SetDciDciBsBlendingLevel ( HI_U32 u32Data, HI_U32 dci_bs_blending_level);
HI_VOID VDP_VP_SetDciDciBsEn            ( HI_U32 u32Data, HI_U32 dci_bs_en);
HI_VOID VDP_VP_SetDciDciBsDetaInv       ( HI_U32 u32Data, HI_U32 dci_bs_deta_inv);
HI_VOID VDP_VP_SetDciDciBsBlackWgtThr   ( HI_U32 u32Data, HI_U32 dci_bs_black_wgt_thr);
HI_VOID VDP_VP_SetDciDciBsLOffset       ( HI_U32 u32Data, HI_U32 dci_bs_l_offset);
HI_VOID VDP_VP_SetDciDciOutHstart       ( HI_U32 u32Data, HI_U32 dci_out_hstart);
HI_VOID VDP_VP_SetDciDciOutHend         ( HI_U32 u32Data, HI_U32 dci_out_hend);
HI_VOID VDP_VP_SetDciDciOutVstart       ( HI_U32 u32Data, HI_U32 dci_out_vstart);
HI_VOID VDP_VP_SetDciDciOutVend         ( HI_U32 u32Data, HI_U32 dci_out_vend);
HI_VOID VDP_VP_SetDciMeanValue          ( HI_U32 u32Data, HI_U32 mean_value);
HI_VOID VDP_VP_SetDciMaxBlack           ( HI_U32 u32Data, HI_U32 max_black);
HI_VOID VDP_VP_SetDciMinBlack           ( HI_U32 u32Data, HI_U32 min_black);
HI_VOID VDP_VP_SetDciDbgPos             (HI_U32 u32Data, HI_U32 dci_dbg_pos);
HI_VOID VDP_VP_SetDciDciTotalPixNum     (HI_U32 u32Data, HI_U32 dci_total_pix_num);
HI_VOID VDP_VP_SetDciDciTotalPixNumBs   (HI_U32 u32Data, HI_U32 dci_total_pix_num_bs);
//-----------------------------------------------------------------------------------
// DCI_END
//-----------------------------------------------------------------------------------


//-------------------------------------------------------------------
//ACM_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetAcmEnable            (HI_U32 u32Data, HI_U32 u32bAcmEn);
HI_VOID VDP_VP_SetAcmDbgEn             (HI_U32 u32Data, HI_U32 u32bAcmDbgEn);
HI_VOID VDP_VP_SetAcmDbgMode           (HI_U32 u32Data, HI_U32 u32bAcmDbgMode);
HI_VOID VDP_VP_SetAcmDbgPos            (HI_U32 u32Data, HI_U32 acm_dbg_pos);
HI_VOID VDP_VP_SetAcmCliporwrap        (HI_U32 u32Data, HI_U32 u32AcmCliporwrap);
HI_VOID VDP_VP_SetAcmStretch           (HI_U32 u32Data, HI_U32 u32AcmStrch);
HI_VOID VDP_VP_SetAcmClipRange         (HI_U32 u32Data, HI_U32 u32AcmClipRange);
HI_VOID VDP_VP_SetAcmChmThd            (HI_U32 u32Data, HI_U32 u32AcmCbcrthr);
HI_VOID VDP_VP_SetAcmGainLum           (HI_U32 u32Data, HI_U32 u32AcmGain0);
HI_VOID VDP_VP_SetAcmGainSat           (HI_U32 u32Data, HI_U32 u32AcmGain2);
HI_VOID VDP_VP_SetAcmGainHue           (HI_U32 u32Data, HI_U32 u32AcmGain1);
HI_VOID VDP_VP_SetAcmCoefAddr          (HI_U32 u32Data, HI_U32 u32CoefAddr);
//-------------------------------------------------------------------
//ACM_END
//-------------------------------------------------------------------



HI_VOID VDP_VP_SetAccThd               (HI_U32 u32Data, ACCTHD_S stAccThd);
HI_VOID VDP_VP_SetAccTab               (HI_U32 u32Data, HI_U32 *upTable);
HI_VOID VDP_VP_SetAccCtrl              (HI_U32 u32Data, HI_U32 uAccEn, HI_U32 uAccMode);
HI_VOID VDP_VP_SetAccWeightAddr        (HI_U32 u32Data,HI_U32 uAccAddr);
HI_VOID VDP_VP_SetAccRst               (HI_U32 u32Data,HI_U32 uAccRst);
//acc
HI_VOID vSetAccCoef                    (VDP_LAYER_VP_E enLayer);
HI_VOID VDP_VP0_SetParaUpd             (HI_U32 u32Data, VDP_VP_PARA_E enMode);
HI_VOID VDP_VP0_SetAccCad              (HI_U32 u32Data, HI_U32 u32addr);

HI_VOID VDP_VID_SetConnection          (HI_U32 u32LayerId, HI_U32 u32Data );
HI_U32  VDP_ReadCoefData(HI_VOID);
HI_VOID VDP_VID_SetCoefReadEnable      (HI_U32 u32Id, HI_U32 u32Para);
HI_VOID VDP_VID_SetCoefReadDisable     (HI_U32 u32Id, HI_U32 u32Para);

//wcg
HI_VOID VDP_VP_SetWcgEnable            (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_VP_SetWcgCoef              (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef);

HI_VOID VDP_VID_SetMasterSel           (HI_U32 u32Data, HI_U32 u32MasterNum);

HI_VOID VDP_VID_SetFrReqSize           (HI_U32 u32Data);
HI_VOID VDP_VID_SetDsReqSize           (HI_U32 u32Data);

HI_VOID VDP_VID_SetEsEnable           ( HI_U32 u32Data, HI_U32 u32Enable                                                                                   );
HI_VOID VDP_VID_SetESMarketMode       ( HI_U32 u32Data, HI_U32 mk_en, HI_U32 mk_mode                                                                       );
HI_VOID VDP_VID_SetESInterlacedProc   ( HI_U32 u32Data, HI_U32 interlaced_proc                                                                             );
HI_VOID VDP_VID_SetESDirMode          ( HI_U32 u32Data, HI_U32 dir_mode                                                                                    );
HI_VOID VDP_VID_SetESCheck1Enable     ( HI_U32 u32Data, HI_U32 check1_en                                                                                   );
HI_VOID VDP_VID_SetESCheck2Enable     ( HI_U32 u32Data, HI_U32 check2_en                                                                                   );
HI_VOID VDP_VID_SetESAlterDirEnable   ( HI_U32 u32Data, HI_U32 alter_dir_en                                                                                );
HI_VOID VDP_VID_SetESEdgeStrengthMode ( HI_U32 u32Data, HI_U32 edge_strength_mode                                                                          );
HI_VOID VDP_VID_SetESBlendMode        ( HI_U32 u32Data, HI_U32 blend_mode                                                                                  );
HI_VOID VDP_VID_SetESDirRatio         ( HI_U32 u32Data, HI_U32 dir_ratio[9]                                                                                );
HI_VOID VDP_VID_SetESNormalizedScale  ( HI_U32 u32Data, HI_U32 normalized_scale[9]                                                                         );
HI_VOID VDP_VID_SetESBC1CFG           ( HI_U32 u32Data, HI_U32 check1_deadzone, HI_U32 check1_autodz_gain, HI_U32 check1_gain, HI_U32 chech1_dir_diff_gain );
HI_VOID VDP_VID_SetESBC2CFG           ( HI_U32 u32Data, HI_U32 check2_deadzone, HI_U32 check2_autodz_gain, HI_U32 check2_gain                              );
HI_VOID VDP_VID_SetESBCCFG            ( HI_U32 u32Data, HI_U32 check_gain                                                                                  );
HI_VOID VDP_VID_SetESRatioLimitScale  ( HI_U32 u32Data, HI_U32 ratio_limit_scale                                                                           );
HI_VOID VDP_VID_SetESVerMinStrength   ( HI_U32 u32Data, HI_S32 ver_min_strength                                                                            );
HI_VOID VDP_VID_SetESEdgeSmoothGain   ( HI_U32 u32Data, HI_U32 edge_smooth_gain                                                                            );
HI_VOID VDP_VID_SetESEdgeThd          ( HI_U32 u32Data, HI_U32 edge_thd[4]                                                                                 );
HI_VOID VDP_VID_SetESEdgeSmoothRatio  ( HI_U32 u32Data, HI_U32 edge_smooth_ratio[5]                                                                        );
HI_VOID VDP_VID_SetESEdgeSlope        ( HI_U32 u32Data, HI_S32 edge_slope[5]                                                                               );
HI_VOID VDP_VP_SetDataMask            (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even );

//DCMP
HI_VOID VDP_VID_SetDcmpCtrl           (HI_U32 u32Data, HI_U32 u32LumLosslessEn, HI_U32 u32ChmLosslessEn );
HI_VOID VDP_VID_SetSrcCropReso        (HI_U32 u32Data, VDP_RECT_S stRect);


HI_VOID VDP_VID_SetTnrDcmpErrorClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);

//add for testpattern
HI_VOID VDP_VID_SetTestPatternEnable   (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID  VDP_VID_SetTestPatternMode    (HI_U32 u32Data, HI_U32 u32Mode );
HI_VOID  VDP_VID_SetTestPatternLWidth    (HI_U32 u32Data, HI_U32 u32Width );
HI_VOID  VDP_VID_SetTestPatternSpeed    (HI_U32 u32Data, HI_U32 u32Speed );
HI_VOID  VDP_VID_SetTestPatternSeed    (HI_U32 u32Data, HI_U32 u32Seed );

//add HZME
HI_VOID VDP_VID_SetHpzmeHpzmeEn(HI_U32 u32Data, HI_U32 hpzme_en);
//HI_VOID VDP_VID_SetHpzmeHpzmeMidEn(HI_U32 u32Data, HI_U32 hpzme_mid_en);
HI_VOID VDP_VID_SetHpzmeHpzmeMode(HI_U32 u32Data, HI_U32 hpzme_mode);
HI_VOID VDP_VID_SetHpzmeCoef2(HI_U32 u32Data, HI_U32 coef2);
HI_VOID VDP_VID_SetHpzmeCoef1(HI_U32 u32Data, HI_U32 coef1);
HI_VOID VDP_VID_SetHpzmeCoef0(HI_U32 u32Data, HI_U32 coef0);

HI_S32 VDP_VID_GetVidLayerEnable (HI_U32 u32Data);

#endif
