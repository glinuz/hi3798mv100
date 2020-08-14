#ifndef __VDP_DRIVER_H__
#define __VDP_DRIVER_H__

#include "vdp_define.h"
#include "hi_drv_disp.h"
#include "drv_disp_osal.h"
#include "vdp_software_selfdefine.h"
//#include "driver_define.h"

//#include <linux/kernel.h>

//HI_U32   VDP_RegRead(volatile HI_U32 *a);
//HI_VOID  VDP_RegWrite(volatile HI_U32 *a, HI_U32 b);
HI_U32   VDP_RegRead(HI_U32 a);
HI_VOID  VDP_RegWrite(HI_U32 a, HI_U32 b);


// VDP INIT
HI_VOID  VDP_DRIVER_SetVirtualAddr(HI_U32 virAddr);
HI_VOID  VDP_DRIVER_Initial(HI_VOID);

// VDAC INIT
HI_VOID  VDAC_DRIVER_SetVirtualAddr(HI_U32 virAddr);
HI_VOID  VDAC_DRIVER_Initial(HI_VOID);

HI_VOID  VDPSYSCTRL_DRIVER_SetVirtualAddr(HI_U32 virAddr);



//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetPreReadEnable      (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetNoSecFlag           (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_VID_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID  VDP_VID_Layer4LinkToMix2         ( HI_BOOL bEnable );
HI_VOID  VDP_VID_SetLayerAddr   (HI_U32 u32Data, HI_U32 u32Chan, HI_U32 regionNo, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_VID_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_VID_SetInDataFmt           (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt);
HI_VOID VDP_VID_SetReadMode            (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode);
HI_VOID VDP_VID_SetMuteEnable          (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID VDP_VID_SetFlipEnable          (HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID VDP_SetFieldOrder              (HI_U32 u32Data, HI_U32 u32FieldOrder);
HI_VOID VDP_VID_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetOutReso             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_VID_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID  VDP_VID_SetUvorder(HI_U32 u32Data, HI_U32 u32bUvorder);
HI_VOID VDP_VID_SetMultiModeEnable(HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID  VDP_VID_MultiSetRegionReso (HI_U32 u32Data, HI_U32 u32RegionNo,HI_RECT_S  stRect);


//DCMP
HI_VOID VDP_VID_SetDcmpEnable                  (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_VID_SetDcmpOffset                  (HI_U32 u32Data, HI_U32 u32laddr_offset , HI_U32  u32caddr_offset );
HI_VOID VDP_VID_SetDcmpErrorClr                (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr);


HI_VOID VDP_VID_SetIfirMode            (HI_U32 u32Data, VDP_IFIRMODE_E enMode);
HI_VOID  VDP_VID_SetIfirCoef           (HI_U32 u32Data, HI_S32 * s32Coef);

HI_VOID VDP_VID_SetLayerGalpha         (HI_U32 u32Data, HI_U32 u32Alpha0);
HI_VOID  VDP_VID_SetCropReso    (HI_U32 u32Data, VDP_RECT_S stRect);
HI_VOID VDP_VID_SetLayerBkg            (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID VDP_SetParaUpMode              (HI_U32 u32Data, HI_U32 u32Mode);

HI_VOID VDP_VID_SetCscDcCoef           (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_VID_SetCscCoef             (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_VID_SetCscEnable           (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_VID_SetCscMode             (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_VID_SetDispMode            (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);
HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32Mode);


HI_VOID VDP_VID_SetPreReadEnable      (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_SetWbcMd                   (HI_U32 enMdSel);
HI_VOID VDP_SetInDataWidth             (HI_U32 u32Data, HI_U32 u32idatawth);
HI_VOID VDP_SetWbcOutMode              (HI_U32 u32Data, VDP_DATA_RMODE_E enRdMode);
HI_VOID VDP_SetTimeOut                 (HI_U32 u32Data, HI_U32 u32TData);

//HI_VOID VDP_VID_SetMultiModeEnable     (HI_U32 u32Data, HI_U32 u32Enable );
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
//HI_VOID VDP_VID_SetZmeHorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
//HI_VOID VDP_VID_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
//HI_VOID VDP_VID_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
//HI_VOID VDP_VID_SetZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr, HI_U32 u32AddrChr);
HI_VOID VDP_VID_SetZmeInFmt_Define1            (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
//HI_VOID VDP_VID_SetZmeOutFmt           (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);

//HI_VOID VDP_VID_SetParaUpd             (HI_U32 u32Data, VDP_VID_PARA_E enMode);
//-------------------------------------------------------------------
//ZME_END
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//VID_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//GFX_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_GFX_SetLayerReso           (HI_U32 u32Data, VDP_DISP_RECT_S stRect);
HI_VOID VDP_GFX_SetVideoPos            (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetDispPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetInReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GFX_SetLayerEnable         (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID  VDP_GFX_SetNoSecFlag          (HI_U32 u32Data, HI_U32 u32Enable );
HI_VOID VDP_GFX_SetLayerAddr           (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32Stride);
HI_VOID VDP_GFX_SetInDataFmt           (HI_U32 u32Data, VDP_GFX_IFMT_E  enDataFmt);
HI_VOID VDP_GFX_SetReadMode            (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID VDP_GFX_SetBitExtend           (HI_U32 u32Data, VDP_GFX_BITEXTEND_E u32mode);
HI_VOID VDP_GFX_SetColorKey            (HI_U32 u32Data, HI_U32  bkeyEn,VDP_GFX_CKEY_S stKey );
HI_VOID VDP_GFX_SetKeyMask             (HI_U32 u32Data, VDP_GFX_MASK_S stMsk);
//lut read update
HI_VOID  VDP_GFX_SetParaUpd            (HI_U32 u32Data, VDP_DISP_COEFMODE_E enMode );
HI_VOID VDP_GFX_SetLutAddr             (HI_U32 u32Data, HI_U32 u32LutAddr);

//DCMP
HI_VOID VDP_GFX_SetDcmpEnable          (HI_U32 u32Data, HI_U32 u32bEnable );
HI_VOID VDP_GFX_SetDcmpAddr            (HI_U32 u32Data, HI_U32 u32LAddr);
HI_VOID VDP_GFX_SetDcmpNAddr           (HI_U32 u32Data, HI_U32 u32NAddr);
HI_VOID VDP_GFX_SetDcmpOffset          (HI_U32 u32Data, HI_U32 u32Offset);
HI_VOID  VDP_VID_SetUvorder           ( HI_U32 u32Data, HI_U32 u32bUvorder);
HI_VOID  VDP_GFX_SetThreeDimDofStep   ( HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep);
HI_VOID  VDP_GFX_SetThreeDimDofEnable ( HI_U32 u32Data, HI_U32 bEnable);

//3D
//HI_VOID VDP_GFX_SetThreeDimEnable      (HI_U32 u32Data, HI_U32 bTrue);
HI_VOID VDP_GFX_SetSrcMode             (HI_U32 u32Data, HI_U32 u32SrcMode);

//HI_VOID VDP_GFX_SetDofEnable           (HI_U32 u32Data, HI_U32 bTrue);
//HI_VOID VDP_GFX_SetDofFmt              (HI_U32 u32Data, HI_U32 u32DataFmt);
//HI_VOID VDP_GFX_SetDofStep             (HI_U32 u32Data, HI_U32 u32eye_sel,HI_U32 u32step);
//HI_VOID VDP_GFX_SetDispMode            (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);

HI_VOID VDP_GFX_SetGammaEnable         (HI_U32 u32Data, HI_U32 u32GmmEn);


HI_VOID  VDP_GFX_SetFlipEnable         (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID  VDP_GFX_SetPreMultEnable      (HI_U32 u32Data, HI_U32 bEnable);

HI_VOID  VDP_GFX_SetLayerBkg           (HI_U32 u32Data, VDP_BKG_S stBkg);
HI_VOID  VDP_GFX_SetLayerGalpha        (HI_U32 u32Data, HI_U32 u32Alpha0);
HI_VOID  VDP_GFX_SetPalpha             (HI_U32 u32Data, HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1);

//HI_VOID  VDP_GFX_SetCmpAddr            (HI_U32 u32Data, HI_U32 u32CmpAddr);
HI_VOID  VDP_GFX_SetLayerNAddr         (HI_U32 u32Data, HI_U32 u32NAddr);
HI_VOID  VDP_GFX_SetMuteEnable         (HI_U32 u32Data, HI_U32 bEnable);
HI_VOID  VDP_GFX_SetUpdMode            (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID  VDP_GFX_SetDeCmpEnable        (HI_U32 u32Data, HI_U32 bEnable);

HI_VOID  VDP_GFX_SetRegUp              (HI_U32 u32Data);
HI_VOID VDP_GFX_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);


////Zme
//HI_VOID  VDP_GFX_SetZmeEnable          (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID  VDP_GFX_SetZmeFirEnable       (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID  VDP_GFX_SetZmeMidEnable       (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID  VDP_GFX_SetZmeHfirOrder       (HI_U32 u32Data, HI_U32 uHfirOrder);
//HI_VOID  VDP_GFX_SetZmeVerTap          (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap);
//HI_VOID  VDP_GFX_SetZmePhase           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);

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

HI_VOID VDP_VP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num);
HI_VOID VDP_VP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef, HI_U32 u32Num);
HI_VOID VDP_VP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn, HI_U32 u32Num);
HI_VOID VDP_VP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode, HI_U32 u32Num);


HI_VOID VDP_VID_SetCscReg(HI_U32 u32Data, HI_U32 *pdata);
HI_VOID VDP_VID_GetCscReg(HI_U32 u32Data, HI_U32 *pdata);
//-------------------------------------------------------------------
//GFX_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//GP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_GP_SetLayerReso            (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_GP_SetVideoPos             (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetDispPos              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetInReso               (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetOutReso              (HI_U32 u32Data, VDP_RECT_S  stRect);
HI_VOID VDP_GP_SetIpOrder              (HI_U32 u32Data, HI_U32 u32Chn, VDP_GP_ORDER_E enIpOrder);
HI_VOID VDP_GP_SetReadMode             (HI_U32 u32Data, HI_U32 u32Mode);
HI_VOID VDP_GP_SetParaUpd              (HI_U32 u32Data, VDP_GP_PARA_E enMode);
HI_VOID VDP_GP_SetRect                 (HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_GP_SetRegUp                (HI_U32 u32Data);
HI_VOID VDP_GP_SetLayerGalpha          (HI_U32 u32Data, HI_U32 u32Alpha);
HI_VOID VDP_GP_SetLayerBkg             (HI_U32 u32Data, VDP_BKG_S stBkg);

//GP_CSC_BEGIN
HI_VOID VDP_GP_SetCscDcCoef            (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef);
HI_VOID VDP_GP_SetCscCoef              (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_GP_SetCscEnable            (HI_U32 u32Data, HI_U32 u32bCscEn);
HI_VOID VDP_GP_SetCscMode              (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);
HI_VOID VDP_GP_SetDispMode             (HI_U32 u32Data, VDP_DISP_MODE_E enDispMode);



HI_VOID VDP_GP_SetZmeEnable           (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmePhase            (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_GP_SetZmeFirEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmeMidEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_GP_SetZmeHorRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_GP_SetZmeVerRatio         (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_GP_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID VDP_GP_SetZmeCoefAddr         (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_GP_SetParaRd(HI_U32 u32Data, VDP_GP_PARA_E enMode);

//GTI
HI_VOID  VDP_GP_SetTiEnable           (HI_U32 u32Data, HI_U32 u32Md,HI_U32 u32Data1);
HI_VOID  VDP_GP_SetTiGainRatio        (HI_U32 u32Data, HI_U32 u32Md, HI_S32 s32Data);
HI_VOID  VDP_GP_SetTiMixRatio         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32mixing_ratio);
HI_VOID  VDP_GP_SetTiHfThd            (HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32TiHfThd);
HI_VOID  VDP_GP_SetTiHpCoef           (HI_U32 u32Data, HI_U32 u32Md, HI_S32 * s32Data);
HI_VOID  VDP_GP_SetTiCoringThd        (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd);
HI_VOID  VDP_GP_SetTiSwingThd         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd, HI_U32 u32thd1);
HI_VOID  VDP_GP_SetTiGainCoef         (HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32coef);
HI_VOID  VDP_GP_SetTiDefThd           (HI_U32 u32Data, HI_U32 u32Md);

//-------------------------------------------------------------------
//GPX_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//Bus Function BEGIN
HI_VOID VDP_SetClkGateEn  (HI_U32 u32Data);
HI_VOID VDP_SetWrOutStd   (HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetRdOutStd   (HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetArbMode    (HI_U32 u32Data, HI_U32 u32bMode);
HI_VOID VDP_SetRdBusId    (HI_U32 u32bMode);
//Bus Function END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//DISP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDispMode           (HI_U32 u32Data, HI_U32 u32DispMode);
HI_VOID VDP_DISP_SetFramePackingEn     (HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetHdmiMode           (HI_U32 u32hd_id, HI_U32 u32hdmi_md);
HI_VOID VDP_DISP_SetHdmiClk  (HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv);
HI_VOID VDP_DISP_SetRegUp              (HI_U32 u32hd_id);
HI_VOID VDP_DISP_SetIntfEnable         (HI_U32 u32hd_id, HI_U32 bTrue);
HI_VOID VDP_DISP_SetIntMask            (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_GetIntMask (HI_U32 *pu32masktype);
HI_VOID VDP_DISP_SetIntDisable         (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_OpenIntf              (HI_U32 u32hd_id, VDP_DISP_SYNCINFO_S stSyncInfo);
HI_VOID VDP_DISP_OpenTypIntf           (HI_U32 u32hd_id, VDP_DISP_DIGFMT_E enDigFmt);
HI_VOID VDP_DISP_SetVSync              (HI_U32 u32hd_id, HI_U32 u32vfb, HI_U32 u32vbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetVSyncPlus          (HI_U32 u32hd_id, HI_U32 u32bvfb, HI_U32 u32bvbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetHSync              (HI_U32 u32hd_id, HI_U32 u32hfb, HI_U32 u32hbb, HI_U32 u32hact);
HI_VOID VDP_DISP_SetSyncInv            (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf,  VDP_DISP_SYNCINV_E enInv);
HI_VOID VDP_DISP_SetPlusWidth          (HI_U32 u32hd_id, HI_U32 u32hpw, HI_U32 u32vpw);
HI_VOID VDP_DISP_SetPlusPhase          (HI_U32 u32hd_id, HI_U32 u32ihs, HI_U32 u32ivs, HI_U32 u32idv);
HI_VOID VDP_DISP_SetIntfMuxSel         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);

HI_U32  VDP_DISP_GetIntSta             (HI_U32 u32intmask);
HI_U32  VDP_DISP_GetIntMskStat             (HI_U32 u32intmask);
HI_VOID VDP_DISP_ClearIntSta           (HI_U32 u32intmask);
HI_VOID VDP_DISP_SetVtThdMode          (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode);
HI_VOID VDP_DISP_SetVtThd              (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd);

#define DHD_VTXTHD_FIELD_MODE 1
#define DHD_VTXTHD_FRAME_MODE 0
#define DHD_YUV_TO_HDMI 0
#define DHD_RGB_TO_HDMI 1
HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id);
HI_VOID VDP_DISP_SetTiming(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S *pstSyncInfo);
HI_VOID VDP_DISP_GetIntfEnable(HI_U32 u32hd_id, HI_U32 *pbTrue);
//-------------------------------------------------------------------
//DISP_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//MIXER_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_CBM_SetMixerBkg              (VDP_CBM_MIX_E u32mixer_id, VDP_BKG_S stBkg);
HI_VOID VDP_CBM_SetMixerPrio             (VDP_CBM_MIX_E u32mixer_id, HI_U32 u32layer_id,HI_U32 u32prio);
HI_U32 VDP_CBM_GetMAXLayer(VDP_CBM_MIX_E eMixId);

HI_VOID VDP_CBM_ResetMixerPrio           (VDP_CBM_MIX_E u32mixer_id);
HI_VOID VDP_CBM_GetMixerPrio(VDP_CBM_MIX_E u32mixer_id, HI_U32 u32prio, HI_U32 *pu32layer_id);
HI_VOID VDP_CBM_SetMixerPrioQuick(VDP_CBM_MIX_E u32mixer_id,HI_U32 *pu32layer_id, HI_U32 u32Number);

//-------------------------------------------------------------------
//MIXER_END
//-------------------------------------------------------------------
////-------------------------------------------------------------------
//CBAR_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetCbarEnable           (HI_U32 u32hd_id,HI_U32 bTrue);
HI_VOID VDP_DISP_SetCbarSel              (HI_U32 u32hd_id,HI_U32 u32cbar_sel);

//-------------------------------------------------------------------
//CBAR_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//IPU_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDitherMode         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_E enDitherMode);
HI_VOID VDP_DISP_SetDitherCoef         (HI_U32 u32hd_id, VDP_DISP_INTF_E enChan, VDP_DITHER_COEF_S dither_coefs);

HI_VOID VDP_DISP_SetClipEnable         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);
HI_VOID VDP_DISP_SetClipCoef           (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DISP_CLIP_S stClipData);

HI_VOID VDP_DISP_SetCscEnable          (HI_U32 u32hd_id, HI_U32  enCSC);
HI_VOID VDP_DISP_SetCscDcCoef          (HI_U32 u32hd_id, VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetCscCoef            (HI_U32 u32hd_id, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetCscMode            (HI_U32 u32hd_id, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_DISP_SetGammaEnable        (HI_U32 u32hd_id, HI_U32 u32GmmEn);
HI_VOID VDP_DISP_SetGammaMode(HI_U32 u32hd_id, HI_U32 u32GmmEn);
HI_VOID VDP_DISP_SetGammaAddr          (HI_U32 u32hd_id, HI_U32 u32uGmmAddr);
HI_VOID VDP_DISP_SetParaUpd            (HI_U32 u32hd_id, VDP_DISP_PARA_E enPara);
HI_VOID VDP_SetMirrorEnable            (HI_U32 u32hd_id,HI_U32 bTrue);
HI_VOID VDP_DISP_SetProToInterEnable   (HI_U32 u32hd_id, HI_U32 u32Enable);
//-------------------------------------------------------------------
//IPU_END
//-------------------------------------------------------------------

//WBC_G0_BEGIN
HI_VOID VDP_WBC_SetCmpEnable( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetCmpDrr( VDP_LAYER_WBC_E enLayer, HI_U32 u32CmpDrr);
HI_VOID VDP_WBC_SetLossy( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetOffSet( VDP_LAYER_WBC_E enLayer, HI_U32 u32Offset);




//-------------------------------------------------------------------
//WBC_DHD0_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_WBC_SetTunlCellAddr        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Addr);
HI_VOID VDP_WBC_SetLowdlyEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetLowdlyTestEnable    (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_SetWbPerLineNum        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Num);
HI_VOID VDP_WBC_SetPartfnsLineNum      (VDP_LAYER_WBC_E enLayer, HI_U32 u32Num);
HI_VOID VDP_WBC_HistogramEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID VDP_WBC_VtthdMode              (VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_vtthd_mode);
HI_VOID VDP_WBC_SetEnable              (VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetOutIntf             (VDP_LAYER_WBC_E enLayer, VDP_DATA_RMODE_E u32RdMode);
HI_VOID VDP_WBC_SetOutFmt              (VDP_LAYER_WBC_E enLayer, VDP_WBC_OFMT_E stIntfFmt);
HI_VOID VDP_WBC_SetSpd                 (VDP_LAYER_WBC_E enLayer, HI_U32 u32ReqSpd);
HI_VOID VDP_WBC_SetLayerAddr           (VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);
HI_VOID VDP_WBC_SetLayerAddr_R         (VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr);
HI_VOID VDP_WBC_SetLayerReso           (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_WBC_SetDitherMode          (VDP_LAYER_WBC_E enLayer, VDP_DITHER_E enDitherMode);
HI_VOID VDP_WBC_SetDitherCoef          (VDP_LAYER_WBC_E enLayer, VDP_DITHER_COEF_S dither_coef);
HI_VOID VDP_WBC_SetCropReso            (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S stRect);
HI_VOID VDP_WBC_SetRegUp               (VDP_LAYER_WBC_E enLayer);

//HI_VOID VDP_WBC_SetZmeCoefAddr         (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr,HI_U32 u32Addrchr);
//HI_VOID VDP_WBC_SetParaUpd             (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E enMode);
HI_VOID VDP_WBC_SetSfifo               (VDP_LAYER_WBC_E enLayer, HI_U32 u32Data );
//HI_VOID VDP_WBC_SetZmeEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable, HI_U32 u32firMode);
//HI_VOID VDP_WBC_SetMidEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
HI_VOID VDP_WBC_SetFirEnable           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable);
//HI_VOID VDP_WBC_SetZmeVerTap           (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap);
//HI_VOID VDP_WBC_SetZmeHfirOrder        (VDP_LAYER_WBC_E enLayer, HI_U32 u32HfirOrder);
//HI_VOID VDP_WBC_SetZmeHorRatio         (VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio);
HI_VOID VDP_WBC_SetZmeInFmt_Define1            (VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_WBC_SetZmeOutFmt           (VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt);
//HI_VOID VDP_WBC_SetZmeVerRatio         (VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio);
HI_VOID VDP_WBC_SetZmePhase            (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_S32 s32Phase);
HI_VOID VDP_WBC_SetCscEnable           (VDP_LAYER_WBC_E enLayer, HI_U32 enCSC);
HI_VOID VDP_WBC_SetCscDcCoef           (VDP_LAYER_WBC_E enLayer,VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_WBC_SetCscCoef             (VDP_LAYER_WBC_E enLayer,VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_WBC_SetCscMode             (VDP_LAYER_WBC_E enLayer, VDP_CSC_MODE_E enCscMode);
HI_VOID VDP_WBC_SetAutoSt              ( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);
HI_VOID VDP_WBC_SetThreeMd             ( VDP_LAYER_WBC_E enLayer, HI_U32 bMode);
//-------------------------------------------------------------------
//WBC_DHD0_END
//-------------------------------------------------------------------

HI_VOID VDP_WBC_SetOutFmtUVOrder(VDP_LAYER_WBC_E enLayer, HI_U32 uvOrder);
//-------------------------------------------------------------------
//ACM_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetAcmEnable           ( HI_U32 u32Data, HI_U32 u32bAcmEn);
HI_VOID VDP_VP_SetAcmDbgEn            ( HI_U32 u32Data, HI_U32 u32bAcmDbgEn);
HI_VOID VDP_VP_SetAcmCliporwrap       ( HI_U32 u32Data, HI_U32 u32AcmCliporwrap);
HI_VOID VDP_VP_SetAcmStretch          ( HI_U32 u32Data, HI_U32 u32AcmStrch);
HI_VOID VDP_VP_SetAcmClipRange        ( HI_U32 u32Data, HI_U32 u32AcmClipRange);
HI_VOID VDP_VP_SetAcmChmThd           ( HI_U32 u32Data, HI_U32 u32AcmCbcrthr);
HI_VOID VDP_VP_SetAcmGainLum          ( HI_U32 u32Data, HI_U32 u32AcmGain0);
HI_VOID VDP_VP_SetAcmGainSat          ( HI_U32 u32Data, HI_U32 u32AcmGain2);
HI_VOID VDP_VP_SetAcmGainHue          ( HI_U32 u32Data, HI_U32 u32AcmGain1);
HI_VOID VDP_VP_SetAcmCoefAddr         ( HI_U32 u32Data, HI_U32 u32CoefAddr);
//-------------------------------------------------------------------
//ACM_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//ABC_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_VP_SetAbcEnable        (HI_U32 u32Data,  HI_U32 u32Enable);
HI_VOID VDP_VP_SetAbcDbgEn          (HI_U32 u32hd_id, HI_U32 u32bAbcDbgEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcUpdEn          (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bAbcUpdEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcAutoMan        (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_AUTOMAN_E enAutoMan,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcReveEn         (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bReveEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcNegaEn         (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bNegaEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcInputAdjEn     (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32InputAdj,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcShiftCtrl      (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_SHIFT_E enShiftCtrl,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcAdjWrite       (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_S32 s32AdjWrite,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcRangeMode      (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32range,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcChromaEn       (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32chromaEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHistThd        (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_HIST_THD_S stHistThd,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcThd            (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_THD_S stThd,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcParaAdj        (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_ADJ_S stParaAdj,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcCoefAddr       (HI_U32 u32hd_id, HI_U32 u32CoefAddr,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcMixEn          (HI_U32 u32hd_id, HI_U32 u32mixEn,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHbwCoef        (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcHbwOffset      (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcLumaThro       (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcLumaCoef       (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcClip           (HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data);
HI_VOID VDP_VP_SetAbcVpRegUp        (HI_U32 u32hd_id, HI_U32 u32VpRegUpEn,HI_U32 u32Data);

//WBC_DHD0_END

HI_VOID VDP_HDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
HI_VOID VDP_HDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
HI_VOID VDP_HDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
HI_VOID VDP_HDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
HI_VOID VDP_HDATE_SetVdacDetStart  ( HI_U32 u32Data) ;
//SDATE                                              ;
HI_VOID VDP_SDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
HI_VOID VDP_SDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
HI_VOID VDP_SDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
HI_VOID VDP_SDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
HI_VOID VDP_SDATE_SetVdacDetStart  ( HI_U32 u32Data) ;

HI_VOID VDP_SDATE_SetVdacDetStart(HI_U32 u32Data);
HI_VOID VDP_VP_SetAccThd(HI_U32 u32Data, ACCTHD_S stAccThd);
HI_VOID VDP_VP_SetAccTab(HI_U32 u32Data, HI_U16 *upTable);
HI_VOID VDP_VP_SetAccCtrl(HI_U32 u32Data, HI_U32 uAccEn, HI_U32 uAccMode);
HI_VOID VDP_VP_SetAccWeightAddr(HI_U32 u32Data,HI_U32 uAccAddr);
HI_VOID VDP_VP_SetAccRst(HI_U32 u32Data,HI_U32 uAccRst);
//  acc
HI_VOID vSetAccCoef(VDP_LAYER_VP_E enLayer);
HI_VOID VDP_VP0_SetParaUpd(HI_U32 u32Data,VDP_DISP_COEFMODE_E  enMode);

HI_VOID  VDP_VP0_SetAccCad(HI_U32 u32Data, HI_U32 u32addr);

//wbc loacate 
HI_VOID VDP_WBC_SetDhdLocate(HI_U32 u32Id, VDP_WBC_LOCATE_E  u32Data);
HI_VOID VDP_WBC_SetDhdOflEn(HI_U32 u32Id, HI_U32  u32Data);



HI_VOID VDP_WBC_SetThreeMd(VDP_LAYER_WBC_E  VDP_LAYER_WBC_GP0, HI_U32 u32Data);


HI_VOID VDP_WBC_SetAutoSt( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable);

HI_VOID VDP_DHD_DEFAULT(HI_VOID);
HI_VOID VDP_DSD_DEFAULT(HI_VOID);
HI_VOID VDP_DHD1_DEFAULT(HI_VOID);
HI_VOID VDP_SDATE_DEFAULT(VDP_DISP_DIGFMT_E enDigFmt);
HI_VOID VDP_HDATE_DEFAULT(VDP_DISP_DIGFMT_E enDigFmt);
HI_VOID VDP_HDATE_MODE(HI_U32 inft_mode);
HI_VOID VDP_INTF_DEFAULT(HI_VOID);
HI_VOID VDP_VGA_DEFAULT(HI_VOID);

HI_VOID VDP_VID_ZME_DEFAULT(HI_VOID);
HI_VOID VDP_DHD_DEBUG_DEFAULT(HI_U32 width);
HI_VOID VDP_DHD_SetDispMode(HI_U32 u32Data,VDP_DATA_RMODE_E dispMode);
HI_VOID VDP_MIXV_SetPrio(HI_U32 prioLayer);
HI_VOID VDP_MIXV_SetPrio1(HI_U32 prioLayer);
HI_VOID VDP_VID_SetLayerBkgColor    (HI_U32 u32Data, HI_U32 stBkg);
HI_VOID VDP_MIXV2_DEFAULT(HI_VOID);
HI_U32  VDP_DHD_GetDispMode(HI_U32 u32Data);
HI_U32 VDP_WBC_GetAlwaysProFlag(/*VDP_LAYER_WBC_E enLayer,*/ HI_BOOL *bAlwaysPro);

HI_VOID vou_RegRwTest(HI_U32 mask[], HI_U32 size, HI_U32 *testaddr, HI_U32 testdata);
HI_VOID vou_RegRTest(HI_U32 mask [],HI_U32 size,HI_U32 * testaddr,HI_U32 regdef [ ]);
HI_U32 VDP_DISP_GetMaskIntSta(HI_U32 u32intmask);
HI_VOID VDP_VID_SetInDataUVOrder(HI_U32 u32Data, HI_U32  VFirst);

HI_VOID VDP_DATE_SetEnable(DISP_VENC_E eDate, HI_U32 enable);

HI_S32 SocVouIrqInit(HI_VOID);
HI_S32 SocVouIrqDeInit(HI_VOID);
HI_S32 SocVouGetIntNumber(VDP_INTMSK_E enInt);
#ifndef __DISP_PLATFORM_BOOT__
irqreturn_t SocVouIsrFunc(HI_S32 irq, HI_VOID *dev_id);
#endif
HI_VOID VDP_DATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt);

HI_VOID VDP_SetLayerConnect(HI_U32 u32Data);
HI_VOID VDP_VDAC_Reset(HI_VOID);
HI_VOID VDP_VDAC_ResetFmt(DISP_VENC_E enVenc ,HI_U32 uVdac, HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk);

HI_VOID VDP_VDAC_SetLink(DISP_VENC_E eDate, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal);
HI_VOID VDP_VDAC_SetEnable(HI_U32 uVdac, HI_U32 enable,HI_U32 u32DelayMs);
HI_VOID VDP_VDAC_SetClockEnable(HI_U32 uVdac, HI_U32 enable);

// VIDEO LAYER CONFIG
HI_VOID VDP_VID_SetInReso2(HI_U32 u32Data, HI_RECT_S *pstRect, HI_RECT_S *pstRectOrigin);
HI_VOID  VDP_VID_SetInCrop(HI_U32 u32Data, HI_U32 xstartposition, HI_U32 xstopposition);
HI_VOID VDP_VID_SetOutReso2(HI_U32 u32Data, HI_RECT_S *pstRect);
HI_VOID VDP_VID_SetVideoPos2(HI_U32 u32Data, HI_RECT_S *pstRect);
HI_VOID VDP_VID_SetDispPos2(HI_U32 u32Data, HI_RECT_S *pstRect);
//HI_VOID  VDP_VID_SetZmePhaseH(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID  VDP_VID_SetZmePhaseV_Define1(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
//HI_VOID  VDP_VID_SetZmePhaseVB(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
//HI_VOID  VDP_VID_SetZmeFirEnable2(HI_U32 u32Data, HI_U32 u32bEnableHl,HI_U32 u32bEnableHc,HI_U32 u32bEnableVl,HI_U32 u32bEnableVc);
//HI_VOID  VDP_VID_SetZmeMidEnable2(HI_U32 u32Data, HI_U32 u32bEnable);
//HI_VOID  VDP_VID_SetZmeEnable2(HI_U32 u32Data, HI_U32 u32bEnable);
//HI_VOID  VDP_VID_SetZmeVchTap(HI_U32 u32Data, HI_U32 u32VscTap);

//HI_VOID  VDP_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
//HI_VOID VDP_WBC_SetZmePhaseV(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID  VDP_VP_GetLayerReso (HI_U32 u32Data, HI_RECT_S *stRect);
HI_VOID VDP_DISP_SetDataSel  (HI_U32 u32hd_id, HI_U32 u32DataSel);
HI_VOID VDP_DATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync);




/******************about date cfg******************/
HI_BOOL  SDATE_SetIntEn(HI_U32 u32Data);
/* get SDATE interrupt status  */
HI_BOOL  SDATE_GetIntSta(HI_U32 u32Data);
/* get SDATE interrupt */
HI_BOOL SDATE_SetOutMode(HI_U32 u32Data, HI_U32 u32Data1);
HI_BOOL SDATE_SetSysMode(HI_U32 u32Data);
HI_BOOL SDATE_SetMduMode(HI_U32 u32Data);
HI_BOOL SDATE_SetSysTyp(HI_U32 u32Data);
/* select chrmoa lpf coeff */
HI_BOOL  SDATE_SetChmCoeff(HI_U32 u32Data);
/* 7.5 IRE enable */
HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data);
/* set RGB output enable */
HI_BOOL  SDATE_SetCompTypMode(HI_U32 u32Data);
/* set sync u32Data for component output  */
HI_BOOL  SDATE_SetCompSynMode(HI_U32 u32Data);
/* set blanking base for RGB output */
HI_BOOL  SDATE_SetBlnkMode(HI_U32 u32Data);
/* set length u32Data of one line */
HI_BOOL  SDATE_SetLenMode(HI_U32 u32Data);
/* set generate u32Data of AGC pulse  */
HI_BOOL  SDATE_SetGenAgcMode(HI_U32 u32Data);
/* set delay u32Data bwtween lum and chroma */
HI_BOOL  SDATE_SetDlyMode(HI_U32 u32Data);
/* set lum over-sample enable */
HI_BOOL  SDATE_SetLumOsEn(HI_U32  u32Data,HI_U32 u32Data1);
/* set chroma over-sample enable */
HI_BOOL  SDATE_SetChmOsEn(HI_U32 u32Data);
/* set lum notch enable */
HI_BOOL  SDATE_SetLumNthEnable(HI_U32 u32Data);
/* set chroma lowpass filtering enable */
HI_BOOL  SDATE_SetChmLpEn(HI_U32 u32Data);
/* set synchronization lowpass filtering enable */
HI_BOOL  SDATE_SetSyncLpEn(HI_U32 u32Data);
/* set chroma gain enable */
HI_VOID VDP_SetCheckSumEnable(HI_U32  bEnable);
/* set teletext send sequence order*/
HI_BOOL  SDATE_SetTtSeq(HI_U32 u32Data);
/* set cc send sequence order*/
HI_BOOL  SDATE_SetCcSeq(HI_U32 u32Data);
/* set cgms send sequence order*/
HI_BOOL  SDATE_SetCgmsSeq(HI_U32  u32Data);
/* set vps send sequence order*/
HI_BOOL  SDATE_SetVpsSeq(HI_U32 u32Data);
/* set wss send sequence order*/
HI_BOOL  SDATE_SetWssSeq(HI_U32 u32Data);
HI_BOOL  SDATE_SetChmLmtEn(HI_U32 u32Data);
HI_BOOL  SDATE_SetCvbsLmtEn(HI_U32 u32Data);
/* set test u32Data enable */
HI_BOOL  SDATE_SetTestEn(HI_U32 u32Data);
/* set test u32Data */
HI_BOOL  SDATE_SetTestMode(HI_U32 u32Data);
/* set dac test input*/
HI_BOOL  SDATE_SetTestData(HI_U32 u32Data);
/* set outside AGC pulse amplification  */
HI_BOOL  SDATE_SetOutAgcAmp(HI_U32 u32Data);
/* set macrovision coefficient */
HI_BOOL  SDATE_SetMvNxValue(HI_U32 u32Data, HI_U32 u32Data1);
/* set tt line enable */
HI_BOOL  SDATE_SetTtLineEn(HI_U32 u32Data, HI_U32 u32Data1);
/* set TT address */ 
HI_BOOL  SDATE_SetTtAddr(HI_U32 u32Data, HI_U32 u32Data1);
/*  get TT ready */
HI_U32 SDATE_GetTtRdy(HI_VOID);
/*  set TT ready */
HI_BOOL  SDATE_SetTtRdy(HI_VOID);
/*  set TT u32Data */
HI_BOOL  SDATE_SetTtMode(HI_U32 u32Data);
/*  set TT package offset address */
HI_BOOL  SDATE_SetTtPktOff(HI_U32 u32Data);
/* set closecaption enable */
HI_BOOL  SDATE_SetCcEn(HI_U32 u32Data, HI_U32 u32Data1);
/*  set CloseCaption line configuration */
HI_BOOL  SDATE_SetCcLineCfg(HI_U32 u32Data, HI_U32 u32Data1);
/*  set CloseCaption data */
HI_BOOL  SDATE_SetCcData(HI_U32 u32Data, HI_U32 u32Data1);
/* set CGMS enable */
HI_BOOL  SDATE_SetCgmsEn(HI_U32 u32Data, HI_U32 u32Data1);
/* set CGMS data */
HI_BOOL  SDATE_SetCgmsData(HI_U32 u32Data, HI_U32 u32Data1);
/*  set WSS enable */
HI_BOOL  SDATE_SetWssEn(HI_U32 u32Data);
/*  set WSS data */
HI_BOOL  SDATE_SetWssData(HI_U32 u32Data);
/*  set VPS enable */
HI_BOOL  SDATE_SetVpsEn(HI_U32 u32Data);
/*  set VPS data */
HI_BOOL  SDATE_SetVpsData(HI_U32 u32Data, HI_U32 u32Data1);
HI_BOOL SDATE_SetVbiLpfEn(HI_U32  u32Data);
HI_BOOL SDATE_SetFmMode(HI_U32  u32Data);
HI_BOOL SDATE_SetTtFullPage(HI_U32  u32Data);
HI_BOOL SDATE_SetTtPrio(HI_U32  u32Data);
HI_BOOL SDATE_SetNabts(HI_U32  u32Data);
HI_BOOL  SDATE_SetOvsCoef(HI_U32 u32Data, HI_S32 * psts32Data);
HI_VOID VDP_DISP_GetVactState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt);

HI_BOOL HDATE_SetOutEn(HI_U32  u32Data);
HI_BOOL HDATE_SetSyncPola(HI_U32  u32Data, HI_U32 u32Data1);
HI_BOOL HDATE_SetSyncMode(HI_U32  u32Data);
HI_BOOL HDATE_SetSyncChn(HI_U32  u32Data, HI_U32 u32Data1);
HI_BOOL HDATE_SetOutFmt(HI_U32  u32Data);
HI_BOOL HDATE_SetCscMode(HI_U32  u32Data);
HI_BOOL HDATE_SetCscRodEn(HI_U32  u32Data);
HI_BOOL HDATE_SetCscCoeff(HI_U32 * psts32Data);
HI_BOOL HDATE_SetOutSel(HI_U32  u32Data,HI_U32 u32Data1);
HI_BOOL HDATE_SetInSrcMode(HI_U32  u32Data);
HI_BOOL HDATE_SetInSrcOsMode(HI_U32  u32Data);
HI_BOOL HDATE_SetLpfEn(HI_U32  u32Data);
HI_BOOL HDATE_SetTestData(HI_U32  u32Data);
HI_BOOL HDATE_SetVbiLpfEn(HI_U32  u32Data);
HI_BOOL HDATE_SetCgmsEn(HI_U32  u32Data,HI_U32 u32Data1);
HI_BOOL HDATE_SetCgmsData(HI_U32  u32Data,HI_U32 u32Data1);
HI_BOOL HDATE_SetMvEn(HI_U32  u32Data);
HI_BOOL HDATE_SetOvsCoef(HI_S32 * psts32Data);

HI_VOID VDP_DATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta);
HI_VOID VDP_DATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable);
HI_VOID VDP_VDAC_GetEnable(HI_U32 uVdac, HI_U32 *penable);
HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode);
HI_S32 VDP_DISP_SelectChanClkDiv(HI_DRV_DISPLAY_E eChn, HI_U32 u32Div);

HI_VOID DISP_ResetCRG(HI_VOID);

HI_VOID VDP_RegSave(HI_U32 u32RegBackAddrf);

HI_VOID VDP_RegReStore(HI_U32 u32RegBackAddrf);
HI_VOID VDP_CloseClkResetModule(HI_VOID);

//extern HI_U32 u32aSDDATEDefSrcCoef[];
HI_VOID VDP_DATE_SetSrcCoef(DISP_VENC_E eDate, HI_U32 *pu32Src13);
HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH);
extern HI_S32 VDP_MVCN_DATE_SetMcvn(HI_U32 *pu32Value);


extern HI_VOID VDP_MVCN_SetMcvnEnable(HI_BOOL bEnable);
extern HI_VOID VDP_VBI_SetCgmsTypeAEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeA);
extern HI_VOID VDP_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data);

extern HI_VOID VDP_VBI_SetCgmsTypeBEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeB);
extern HI_VOID VDP_VBI_SetCgmsTypeB(DISP_VENC_E enChanel,HI_U32 u32Data);
#endif
