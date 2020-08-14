#ifndef __VDP_DRV_VID_H__
#define __VDP_DRV_VID_H__
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
#include "vdp_define.h"
//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetPreReadEnable       (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetNoSecFlag           (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID  VDP_VID_SetFiLayerEnable    (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID  VDP_VID_SetFiVscale    (HI_U32 u32Data, HI_U32 u32bFiVscale );
HI_VOID VDP_VID_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VID_SetInDataFmt           (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt);
HI_VOID VDP_VID_SetDataWidth           (HI_U32 u32Data, HI_U32 DataWidth);
HI_VOID VDP_VID_SetReadMode            (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode);
HI_VOID VDP_VID_SetMuteEnable          (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VID_SetFlipEnable          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_VID_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetOutReso             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
//U V Order
HI_VOID VDP_VID_SetUvorder           ( HI_U32 u32Data, HI_U32 u32bUvorder);

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

HI_VOID VDP_VID_SetDrawMode            (HI_U32 u32Data, HI_U32 u32Mode);

HI_VOID VDP_SetWbcMd                   (HI_U32 enMdSel);
HI_VOID VDP_SetInDataWidth             (HI_U32 u32Data, HI_U32 u32idatawth);
HI_VOID VDP_SetWbcOutMode              (HI_U32 u32Data, VDP_DATA_RMODE_E enRdMode);
HI_VOID VDP_SetTimeOut                 (HI_U32 u32Data, HI_U32 u32TData);


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
HI_VOID VDP_VID_GetZmeHorRatio         (HI_U32 u32Data, HI_U32 *u32Ratio);
HI_VOID VDP_VID_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_VID_GetZmeVerRatio         (HI_U32 u32Data, HI_U32 *u32Ratio);
HI_VOID VDP_VID_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_VID_SetZmeCoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32LAddr, HI_U32 u32CAddr);
HI_VOID VDP_VID_SetZmeInFmt_Define1            (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_VID_SetZmeOutFmt           (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZmeNonLnrDelta     (HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta);
HI_VOID  VDP_VID_SetZmeNonLnrZoneEnd   (HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End);

HI_VOID VDP_VID_SetParaUpd             (HI_U32 u32Data, VDP_VID_PARA_E enMode);
HI_VOID  VDP_VID_SetFrReqSize    (HI_U32 u32Data);
HI_VOID  VDP_VID_SetDsReqSize    (HI_U32 u32Data);


//-------------------------------------------------------------------
//VID_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//VP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VP_SetThreeDimDofStep      (HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep);
HI_VOID VDP_VP_SetLayerReso            (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VP_SetVideoPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetDispPos              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetInReso               (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VP_SetRegUp                (HI_U32 u32Data);
HI_VOID VDP_VP_SetLayerGalpha          (HI_U32 u32Data, HI_U32 u32Alpha);
HI_VOID VDP_VP_SetLayerBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_VP_SetMuteEnable           (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VP_SetDispMode             (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);

//HI_VOID VDP_VP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn, HI_U32 u32Num);
//HI_VOID VDP_VP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode, HI_U32 u32Num);

HI_VOID VDP_VP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_VP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_VP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn);
//-------------------------------------------------------------------
//VP_END
//-------------------------------------------------------------------

HI_VOID VDP_VP_SetVpSel     (HI_U32 u32Data,HI_U32 vp_sel);
HI_VOID VDP_VP_SetAcmEnable (HI_U32 u32Data, HI_U32 u32bAcmEn);
HI_VOID VDP_VP_SetDciEnable(HI_U32 u32Data, HI_U32 u32DciEn);
HI_VOID VDP_VP_SetWcgEnable  (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID  VDP_VID_SetEsEnable (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetSharpEnable(HI_U32 u32LayerId, HI_U32 bEnable);
/*2d ----> 3d*/
HI_VOID VDP_VID_SetTwodt3dEn(HI_U32 u32Data, HI_U32 u32twodt3d_en);
#endif
