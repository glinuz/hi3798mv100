#ifndef __DRV_VID_H__
#define __DRV_VID_H__
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
#include "vdp_define.h"
//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl);


HI_VOID VDP_VID_SetPreReadEnable       (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetNoSecFlag           (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetFiLayerEnable       (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VID_SetInDataFmt           (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt);
HI_VOID VDP_VID_SetDataWidth           (HI_U32 u32Data, HI_U32 DataWidth);
HI_VOID VDP_VID_SetReadMode            (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode);
HI_VOID VDP_VID_SetMuteEnable          (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VID_SetMuteBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_VID_SetFlipEnable          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetOutReso             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID  VDP_VID_SetFiVscale           (HI_U32 u32Data, HI_U32 u32bFiVscale );
HI_VOID  VDP_VID_SetDcmpIsLosslessYC(HI_U32 u32Data, HI_U32 u32Y_Lossless, HI_U32 u32C_Lossless);

//U V Order
HI_VOID VDP_VID_SetUvorder           ( HI_U32 u32Data, HI_U32 u32bUvorder);

HI_VOID  VDP_VID_SetTwodt3dEn         (HI_U32 u32Data, HI_U32 u32twodt3d_en);


//DCMP
HI_VOID VDP_VID_SetDcmpEnable          (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetDcmpOffset          (HI_U32 u32Data, HI_U32 u32laddr_offset , HI_U32  u32caddr_offset );
HI_VOID VDP_VID_SetDcmpErrorClr        (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);

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

HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32ModeLuma, HI_U32 u32ModeChroma);

HI_VOID VDP_SetWbcMd                   (HI_U32 enMdSel);
HI_VOID VDP_SetInDataWidth             (HI_U32 u32Data, HI_U32 u32idatawth);
HI_VOID VDP_SetWbcOutMode              (HI_U32 u32Data, VDP_DATA_RMODE_E enRdMode);
//HI_VOID VDP_SetTimeOut                 (HI_U32 u32Data, HI_U32 u32TData);


HI_VOID VDP_VID_SetMultiModeEnable     (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetRegionEnable        (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetAllRegionDisable    (HI_U32 u32Data );
HI_VOID VDP_VID_SetRegionAddr          (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_VID_SetRegionReso          (HI_U32 u32Data, HI_U32 u32Num, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetRegionMuteEnable    (HI_U32 u32Data, HI_U32 u32Num, HI_U32 bEnable);

HI_VOID VDP_VID_SetRegUp               (HI_U32 u32Data);

//-------------------------------------------------------------------
//ZME_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetZmeEnable           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmePhase            (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_VID_SetZmeFirEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmeMidEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetZmeHorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_VID_SetZmeCoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID  VDP_VID_SetZmeInFmt_Define1(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_VID_SetZmeOutFmt           (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZmeNonLnrDelta     (HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta);
HI_VOID  VDP_VID_SetZmeNonLnrZoneEnd   (HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End);

HI_VOID VDP_VID_SetParaUpd             (HI_U32 u32Data, VDP_VID_PARA_E enMode);

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
HI_VOID  VDP_VP_SetMuteBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);
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
HI_VOID VDP_VP_SetDciStatCtrl          (HI_U32 u32Data, HI_U32 u32StatCtrl);

HI_VOID VDP_VP_SetDciPosition          (HI_U32 u32Data, VDP_DCI_POS_S stDciPos);
HI_VOID VDP_VP_SetDciHisBld            (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciHisOft            (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciHisCor            (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciMerBld            (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciAdjWgt            (HI_U32 u32Data, VDP_VP_DCI_ADJ_NUM_E Adjnum, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciClip              (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciGlbGain           (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);

HI_VOID VDP_VP_SetDciPos               (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);
HI_VOID VDP_VP_SetDciNeg               (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);

HI_VOID VDP_VP_SetDciAdjSW             (HI_U32 u32Data, VDP_DCI_PARA_S stDciPara);

HI_VOID VDP_VP_SetDciCoefAddr          (HI_U32 u32Data, HI_U32 u32CoefAddr);
//HI_VOID VDP_VP_SetDciVpRegUp           (HI_U32 u32Data, HI_U32 VpRegUpEn);
HI_VOID VDP_VP_SetParaUpd              (HI_U32 u32Data, VDP_VP_PARA_E VpPara);
//-----------------------------------------------------------------------------------
// DCI_END
//-----------------------------------------------------------------------------------


//-------------------------------------------------------------------
//ACM_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetAcmEnable            (HI_U32 u32Data, HI_U32 u32bAcmEn);
HI_VOID VDP_VP_SetAcmDbgEn             (HI_U32 u32Data, HI_U32 u32bAcmDbgEn);
HI_VOID VDP_VP_SetAcmDbgMode           (HI_U32 u32Data , HI_U32 u32bAcmDbgMode);
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


//-------------------------------------------------------------------
//ABC_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetAbcEnable            (HI_U32 u32Data,  HI_U32 u32Enable);
HI_VOID VDP_VP_SetAbcDbgEn             (HI_U32 u32hd_id, HI_U32 u32bAbcDbgEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcUpdEn             (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bAbcUpdEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcAutoMan           (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_AUTOMAN_E enAutoMan,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcReveEn            (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bReveEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcNegaEn            (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bNegaEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcInputAdjEn        (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32InputAdj,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcShiftCtrl         (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_SHIFT_E enShiftCtrl,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcAdjWrite          (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_S32 s32AdjWrite,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcRangeMode         (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32range,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcChromaEn          (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32chromaEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHistThd           (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_HIST_THD_S stHistThd,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcThd               (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_THD_S stThd,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcParaAdj           (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_ADJ_S stParaAdj,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcCoefAddr          (HI_U32 u32hd_id, HI_U32 u32CoefAddr,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcMixEn             (HI_U32 u32hd_id, HI_U32 u32mixEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHbwCoef           (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHbwOffset         (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcLumaThro          (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcLumaCoef          (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcClip              (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcVpRegUp           (HI_U32 u32hd_id, HI_U32 u32VpRegUpEn,HI_U32 u32Data);

//-------------------------------------------------------------------
//ABC_END
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
HI_VOID VDP_VP_SetWcgCoef              (HI_U32 u32Data, VDP_WCG_COEF_S stWcgCoef);

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
//-----------------------------------------------------------------------------------
//SHARPEN_BEGIN
//-----------------------------------------------------------------------------------
HI_VOID VDP_VID_SetSharpDbgEn         (HI_U32 u32LayerId, HI_U32 bEnable);
HI_VOID VDP_VID_SetSharpDemoMode      (HI_U32 u32LayerId, HI_U32 DemoMode);
HI_VOID VDP_VID_SetSharpEnable        (HI_U32 u32LayerId, HI_U32 bEnable);
HI_VOID VDP_VID_SetSharpDispMode      (HI_U32 u32LayerId, HI_U32 u32DispMode);
HI_VOID VDP_VID_SetSharpGlbThd        (HI_U32 u32LayerId, HI_U32 u32UnderThd, HI_U32 u32OverThd);
HI_VOID VDP_VID_SetSharpMergeMode     (HI_U32 u32LayerId, HI_U32 u32Data);
HI_VOID VDP_VID_SetSharpDetailAdj     (HI_U32 u32LayerId, HI_U32 bEnable, HI_U32 u32Thd);
HI_VOID VDP_VID_SetSharpPeakRatio     (HI_U32 u32LayerId, HI_U32 u32Data);
HI_VOID VDP_VID_SetSharpLtiRatio      (HI_U32 u32LayerId, HI_U32 u32Data);
HI_VOID VDP_VID_SetSharpShootDiv      (HI_U32 u32LayerId, HI_U32 u32Data);
HI_VOID VDP_VID_SetSharpLutDiv        (HI_U32 u32LayerId, HI_U32 u32Data);
HI_VOID VDP_VID_SetSharpColorAdj      (HI_U32 u32LayerId, HI_U32 bEnable, HI_U32 u32Low, HI_U32 u32High);
HI_VOID VDP_VID_SetSharpHighFreq      (HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, VDP_SHARP_GROUP_S * stCfg);
HI_VOID VDP_VID_SetSharpLti           (HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, HI_U32 bEnable, HI_U32 u32Gain);
HI_VOID VDP_VID_SetSharpCti           (HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, HI_U32 bEnable, HI_U32 u32Gain);
HI_VOID VDP_VID_SetSharpCoefAddr      (HI_U32 u32LayerId, HI_U32 u32CoefAddr);
//HI_VOID VDP_VID_SetSharpDispMode      (HI_U32 u32LayerId, HI_U32 u32DispMode);
//HI_VOID VDP_VID_SetSharpDrEn          (HI_U32 u32LayerId, HI_U32 DrEn , HI_U32 MaxdiffEn);
HI_VOID VDP_VID_SetSharpLtiCor        (HI_U32 u32LayerId, HI_U32 u32LtiCor);
HI_VOID VDP_VID_SetSharpCtiA1Th       (HI_U32 u32LayerId, HI_U32 u32CtiA1Th0,HI_U32 u32CtiA1Th1);
HI_VOID VDP_VID_SetSharpCtiA2Th       (HI_U32 u32LayerId, HI_U32 u32CtiA2Th0,HI_U32 u32CtiA2Th1,HI_U32 u32CtiA2Win);
HI_VOID VDP_VID_SetSharpCtiA3Th       (HI_U32 u32LayerId, HI_U32 u32CtiA3Th0,HI_U32 u32CtiA3Th1);
HI_VOID VDP_VID_SetSharpCtiA4Gain     (HI_U32 u32LayerId, HI_U32 u32CtiA4Cor,HI_U32 u32CtiA4Gain);
//HI_VOID VDP_VID_SetSharpDiffWin       (HI_U32 u32LayerId, HI_U32 u32DiffWin);
//HI_VOID VDP_VID_SetSharpDrOvlp        (HI_U32 u32LayerId, HI_U32 u32DrHOvlp, HI_U32 u32DrVOvlp, HI_U32 u32DrDOvlp);
//HI_VOID VDP_VID_SetSharpDrTex         (HI_U32 u32LayerId, HI_U32 u32DrTexTh, HI_U32 u32DrTexSlop);
//HI_VOID VDP_VID_SetSharpDrGain        (HI_U32 u32LayerId, HI_U32 u32DrHDiffGain, HI_U32 u32DrVDiffGain, HI_U32 u32DrDDiffGain);
//HI_VOID VDP_VID_SetSharpDrShift       (HI_U32 u32LayerId, HI_U32 u32DrDiffShift);
//HI_VOID VDP_VID_SetSharpDrBriTh1      (HI_U32 u32LayerId, HI_U32 u32DrBriTh0,HI_U32 u32DrBriTh1);
//HI_VOID VDP_VID_SetSharpDrBriTh2      (HI_U32 u32LayerId, HI_U32 u32DrBriTh2,HI_U32 u32DrBriTh3);
//HI_VOID VDP_VID_SetSharpDrBriSlp      (HI_U32 u32LayerId, HI_U32 u32DrBriSlp0,HI_U32 u32DrBriSlp1,HI_U32 u32DrBriSlp2,HI_U32 u32DrBriSlp3);
//HI_VOID VDP_VID_SetSharpDrBriRatio0   (HI_U32 u32LayerId, HI_U32 u32DrBriRatio0,HI_U32 u32DrBriRatio1,HI_U32 u32DrBriRatio2,HI_U32 u32DrBriRatio3);
//HI_VOID VDP_VID_SetSharpDrBriRatio1   (HI_U32 u32LayerId, HI_U32 u32DrOriBriRatio,HI_U32 u32DrMinBriRatio,HI_U32 u32DrMaxBriRatio);
HI_VOID VDP_VID_SetSharpCtiA1Slp      (HI_U32 u32LayerId, HI_U32 u32CtiA1Slp);
HI_VOID VDP_VID_SetSharpCtiA2Slp      (HI_U32 u32LayerId, HI_U32 u32CtiA2Slp);
HI_VOID VDP_VID_SetSharpCtiA3Slp      (HI_U32 u32LayerId, HI_U32 u32CtiA3Slp);
//sharpen add
HI_VOID VDP_VID_SetSharpYValueEn      (HI_U32 u32LayerId, HI_U32 u32YValueEn);
HI_VOID VDP_VID_SetSharpYValueTh      (HI_U32 u32LayerId, HI_U32 u32YValueTh0    , HI_U32 u32YValueTh1      , HI_U32 u32YValueTh2     , HI_U32 u32YValueTh3);
HI_VOID VDP_VID_SetSharpYValueSlp     (HI_U32 u32LayerId, HI_U32 u32YValueSlp1   , HI_U32 u32YValueSlp2);
HI_VOID VDP_VID_SetSharpCtiA4Th       (HI_U32 u32LayerId, HI_U32 u32CtiA4Th0     , HI_U32 u32CtiA4Th1);
HI_VOID VDP_VID_SetSharpCtiA4Slp      (HI_U32 u32LayerId, HI_U32 u32CtiA4Slp);
HI_VOID VDP_VID_SetSharpFlatSatur     (HI_U32 u32LayerId, HI_U32 u32Satur        , HI_U32 u32YFlatNum);
HI_VOID VDP_VID_SetSharpGradSumTh     (HI_U32 u32LayerId, HI_U32 u32CGradSumLow  , HI_U32 u32CGradSumHigh   , HI_U32 u32YGradSumThr);
HI_VOID VDP_VID_SetSharpGradThr       (HI_U32 u32LayerId, HI_U32 u32CGradThr0    , HI_U32 u32CGradThr1      , HI_U32 u32CGradDecThr0  , HI_U32 u32CGradDecThr1);
HI_VOID VDP_VID_SetSharpGradDiffThr   (HI_U32 u32LayerId, HI_U32 u32GradDiffThr0 , HI_U32 u32GradDiffThr1);
//HI_VOID VDP_VID_SetSharpVp0ParaRd     (HI_U32 u32LayerId , HI_U32 u32Vp0ParaRd);

//-----------------------------------------------------------------------------------
//SHARPEN_END
//-----------------------------------------------------------------------------------

HI_VOID VDP_VP_SetDataMask            (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even );

//DCMP
HI_VOID VDP_VID_SetDcmpCtrl   (HI_U32 u32Data, HI_U32 u32LumLosslessEn, HI_U32 u32ChmLosslessEn );
//tile format !!!
HI_VOID  VDP_VID_SetTileCrop(HI_U32 u32Data,HI_U32 x_offset,HI_U32 l_y_offset,HI_U32 l_c_offset);

#endif
