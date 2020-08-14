#ifndef __HAL_GFX_H__
#define __HAL_GFX_H__
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif
#include "vdp_define.h"
#include "vdp_ip_define.h"


//-------------------------------------------------------------------
//GFX_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_GFX_SetGfxArbMode  (HI_U32 u32GfxLayer, HI_U32 u32Mode);

HI_VOID VDP_GFX_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl);
//HI_VOID VDP_GFX_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S stRect);
HI_VOID VDP_GFX_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetSrcPos              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_GFX_SetNoSecFlag           (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_GFX_SetLayerAddr           (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32Stride);
HI_VOID VDP_GFX_SetInDataFmt           (HI_U32 u32Data, VDP_GFX_IFMT_E  enDataFmt);
HI_VOID VDP_GFX_SetReadMode            (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID VDP_GFX_SetBitExtend           (HI_U32 u32Data, VDP_GFX_BITEXTEND_E u32mode);
HI_VOID VDP_GFX_SetColorKey            (HI_U32 u32Data, HI_U32  bkeyEn,VDP_GFX_CKEY_S stKey );
HI_VOID VDP_GFX_SetKeyMask             (HI_U32 u32Data, VDP_GFX_MASK_S stMsk);
HI_VOID VDP_GP_SetVfirOneTapEnable     (HI_U32 u32Data, HI_U32 u32VfirOneTapEn);
HI_VOID VDP_GP_SetVfirTwoTapEnable     (HI_U32 u32Data, HI_U32 u32VfirTwoTapEn);
//lut read update
HI_VOID VDP_GFX_SetParaUpd             (HI_U32 u32Data, VDP_GFX_PARA_E enMode );
HI_VOID VDP_GFX_SetLutAddr             (HI_U32 u32Data, HI_U32 u32LutAddr);

//DCMP
HI_VOID VDP_GFX_SetDcmpEnable          (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_GFX_SetDcmpAddr            (HI_U32 u32Data, HI_U32 u32LAddr);
HI_VOID VDP_GFX_SetDcmpNAddr           (HI_U32 u32Data, HI_U32 u32NAddr);
HI_VOID VDP_GFX_SetDcmpOffset         ( HI_U32 u32Data, HI_U32 u32Offset);

HI_VOID VDP_GFX_SetDcmpLossLess        (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_GFX_SetDcmpAlphaLossLess   (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_GFX_SetDcmpCmpMode         (HI_U32 u32Data, HI_U32 u32Mode);

//DOF
HI_VOID VDP_GFX_SetThreeDimDofStep   ( HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep);
HI_VOID VDP_GFX_SetThreeDimDofEnable ( HI_U32 u32Data, HI_U32 bEnable);
HI_VOID  VDP_GFX_SetThreeDimDofBkg   (HI_U32 u32Data, VDP_BKG_S stBkg);
//HI_VOID VDP_GFX_SetThreeDimIsRData   ( HI_U32 u32Data, HI_U32 u32IsRData);
// burst length choice 
HI_VOID  VDP_GFX_SetReqLength           (HI_U32 u32Data, HI_U32 u32RegLen);

//3D
//HI_VOID VDP_GFX_SetThreeDimEnable      (HI_U32 u32Data, HI_U32 bTrue);
HI_VOID VDP_GFX_SetSrcMode             (HI_U32 u32Data, HI_U32 u32SrcMode);

//HI_VOID VDP_GFX_SetDofEnable           (HI_U32 u32Data, HI_U32 bTrue);
//HI_VOID VDP_GFX_SetDofFmt              (HI_U32 u32Data, HI_U32 u32DataFmt);
//HI_VOID VDP_GFX_SetDofStep             (HI_U32 u32Data, HI_U32 u32eye_sel,HI_U32 u32step);
//HI_VOID VDP_GFX_SetDispMode            (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);

HI_VOID VDP_GFX_SetGammaEnable         (HI_U32 u32Data, HI_U32 u32GmmEn);


HI_VOID VDP_GFX_SetFlipEnable         (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_GFX_SetPreMultEnable      (HI_U32 u32Data, HI_U32 bEnable);

HI_VOID VDP_GFX_SetLayerBkg           (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_GFX_SetLayerGalpha        (HI_U32 u32Data, HI_U32 u32Alpha0);
HI_VOID VDP_GFX_SetPalpha             (HI_U32 u32Data, HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1);

//HI_VOID  VDP_GFX_SetCmpAddr            (HI_U32 u32Data, HI_U32 u32CmpAddr);
HI_VOID VDP_GFX_SetLayerNAddr         (HI_U32 u32Data, HI_U32 u32NAddr);
HI_VOID  VDP_GFX_GetMuteEnable(HI_U32 u32Data, HI_U32 *pbEnable);

HI_VOID VDP_GFX_SetMuteEnable         (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_GFX_SetUpdMode            (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID VDP_GFX_SetDeCmpEnable        (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_GFX_MixvBypassEn (HI_U32 u32En);
HI_VOID VDP_GFX_MixvPremultEn (HI_U32 u32En);

HI_VOID VDP_DRV_GetGfxState(HI_BOOL *pbNeedProcessGfx);


HI_VOID VDP_GFX_SetRegUp              (HI_U32 u32Data);


////Zme
//HI_VOID VDP_GFX_SetZmeEnable          (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID VDP_GFX_SetZmeFirEnable       (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID VDP_GFX_SetZmeMidEnable       (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID VDP_GFX_SetZmeHfirOrder       (HI_U32 u32Data, HI_U32 uHfirOrder);
//HI_VOID VDP_GFX_SetZmeVerTap          (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap);
//HI_VOID VDP_GFX_SetZmePhase           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);

//-------------------------------------------------------------------
//GFX_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//GP_BEGIN
//-------------------------------------------------------------------
//HI_VOID VDP_GP_SetLayerReso            (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_GP_SetVideoPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetDispPos              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetInReso               (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetOutReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetIpOrder              (HI_U32 u32Data, HI_U32 u32Chn, VDP_GP_ORDER_E enIpOrder);
HI_VOID VDP_GP_SetReadMode             (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID VDP_GP_SetParaUpd              (HI_U32 u32Data, VDP_GP_PARA_E enMode);
//HI_VOID VDP_GP_SetRect                 (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_GP_SetRegUp                (HI_U32 u32Data);

HI_VOID  VDP_GP_GetLayerGalpha (HI_U32 u32Data, HI_U32 *pu32Alpha);

HI_VOID VDP_GP_SetLayerGalpha          (HI_U32 u32Data, HI_U32 u32Alpha);
HI_VOID VDP_GP_SetLayerBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);

//GP_CSC_BEGIN
HI_VOID VDP_GP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_GP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_GP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_GP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);
//HI_VOID VDP_GP_SetDispMode             (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);


HI_VOID VDP_GP_SetZmeEnable           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmePhase            (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_GP_SetZmeFirEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmeMidEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmeHorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_GP_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_GP_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_GP_SetZmeCoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_GP_SetParaRd              (HI_U32 u32Data, VDP_GP_PARA_E enMode);

//GTI
HI_VOID VDP_GP_SetTiEnable           (HI_U32 u32Data, HI_U32 u32Md,HI_U32 u32Data1);
HI_VOID VDP_GP_SetTiGainRatio        (HI_U32 u32Data, HI_U32 u32Md, HI_S32 s32Data);
HI_VOID VDP_GP_SetTiMixRatio         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32mixing_ratio);
HI_VOID VDP_GP_SetTiHfThd            (HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32TiHfThd);
HI_VOID VDP_GP_SetTiHpCoef           (HI_U32 u32Data, HI_U32 u32Md, HI_S32 * s32Data);
HI_VOID VDP_GP_SetTiCoringThd        (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd);
HI_VOID VDP_GP_SetTiSwingThd         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd, HI_U32 u32thd1);
HI_VOID VDP_GP_SetTiGainCoef         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32coef);
HI_VOID VDP_GP_SetTiDefThd           (HI_U32 u32Data, HI_U32 u32Md);

//WCG
HI_VOID VDP_GP_SetWcgEnable            (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_GP_SetWcgCoef              (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef);

//-------------------------------------------------------------------
//GPX_END
//-------------------------------------------------------------------


HI_VOID VDP_GFX_SetConnection(HI_U32 u32LayerId, HI_U32 u32Data );
//3535 new add gfx csc
HI_VOID VDP_GFX_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_GFX_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_GFX_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_GFX_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_GP_SetCoefReadEnable       (HI_U32 u32Id, HI_U32 u32Para);
HI_VOID VDP_GP_SetCoefReadDisable      (HI_U32 u32Id, HI_U32 u32Para);

HI_VOID VDP_GFX_SetMasterSel           (HI_U32 u32Data, HI_U32 u32MasterNum);
HI_VOID VDP_GFX_SetDataMask            (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even);
HI_VOID VDP_GP_SetTwoChnEnable         (HI_U32 u32Enable);
HI_VOID VDP_GP_SetOutputMode           (HI_U32 u32Mode);
#endif
