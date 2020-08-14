#ifndef __HAL_CHN_H__
#define __HAL_CHN_H__

#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <vdp_assert.h>
#endif
#include "vdp_define.h"
#include "vdp_ip_define.h"

#if VDP_C_COMPILE
#endif

//-------------------------------------------------------------------
//DISP_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_DISP_ClearIntSta1(HI_U32 u32intmask );

HI_VOID VDP_DISP_SetUfOffineEn         (HI_U32 u32hd_id, HI_U32 u32UfOfflineEn);
HI_VOID VDP_DISP_SetDispMode           (HI_U32 u32hd_id, HI_U32 u32DispMode);
//HI_VOID VDP_DISP_SetFramePackingEn     (HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetHdmiMode           (HI_U32 u32hd_id, HI_U32 u32hdmi_md);
HI_VOID VDP_DISP_SetHdmi420Enable      (HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetHdmi420CSel        (HI_U32 u32hd_id, HI_U32 u32CSel); 
HI_VOID VDP_DISP_SetRegUp              (HI_U32 u32hd_id);
HI_VOID VDP_DISP_SetIntfEnable         (HI_U32 u32hd_id, HI_U32 bTrue);
HI_VOID VDP_DISP_SetIntMask            (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetIntDisable         (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetOflIntMask         (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetOflIntDisable      (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_BfmClkSel             (HI_U32 u32Num);
HI_U32 VDP_DISP_GetDispMode(HI_U32 u32hd_id);
HI_VOID VDP_DISP_OpenIntf              (HI_U32 u32hd_id, VDP_DISP_SYNCINFO_S stSyncInfo);
HI_VOID VDP_DISP_SetTrigger       (HI_U32 u32hd_id, HI_BOOL bEnable);
HI_VOID VDP_DISP_OpenTypIntf          (HI_U32 u32hd_id, VDP_DISP_DIGFMT_E enDigFmt);
HI_VOID VDP_DISP_SetVSync              (HI_U32 u32hd_id, HI_U32 u32vfb, HI_U32 u32vbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetVSyncPlus          (HI_U32 u32hd_id, HI_U32 u32bvfb, HI_U32 u32bvbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetHSync              (HI_U32 u32hd_id, HI_U32 u32hfb, HI_U32 u32hbb, HI_U32 u32hact);
HI_VOID VDP_DISP_SetSyncInv            (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf,  VDP_DISP_SYNCINV_S enInv);
HI_VOID VDP_DISP_SetPlusWidth          (HI_U32 u32hd_id, HI_U32 u32hpw, HI_U32 u32vpw);
HI_VOID VDP_DISP_SetPlusPhase          (HI_U32 u32hd_id, HI_U32 u32ihs, HI_U32 u32ivs, HI_U32 u32idv);
HI_VOID VDP_DISP_SetIntfMuxSel         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);

HI_U32  VDP_DISP_GetIntSta             (HI_U32 u32intmask);
HI_VOID VDP_DISP_ClearIntSta           (HI_U32 u32intmask);
HI_VOID VDP_DISP_SetVtThdMode          (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode);
HI_VOID VDP_DISP_SetVtThd              (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd);

HI_VOID VDP_DISP_SetWcgEnable         (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetWcgCoef           (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef);

HI_VOID VDP_DISP_SetHbiSyncEnable ( HI_U32 u32Data, HI_U32 u32HbiSyncEnable ); 
HI_VOID VDP_DISP_SetHbiHcEnable   ( HI_U32 u32Data, HI_U32 u32HbiHcEnable   ); 
HI_VOID VDP_DISP_SetHbiOsdEnable  ( HI_U32 u32Data, HI_U32 u32HbiOsdEnable  ); 
HI_VOID VDP_DISP_SetVbiEnable     ( HI_U32 u32Data, HI_U32 u32VbiEnable     ); 
HI_VOID VDP_DISP_SetVbiData       (HI_U32 u32Data, HI_U32 u32VbiAddr, HI_U32 u32VbiData);
HI_VOID VDP_DISP_SetHbiHcThd      ( HI_U32 u32Data, HI_U32 u32HbiHcThd);
HI_VOID VDP_DISP_SetHbiOsdThd     ( HI_U32 u32Data, HI_U32 u32HbiOsdThd);
HI_VOID VDP_DISP_ClearIntMask  (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetNxgDataSwapEnable(HI_U32 u32hd_id, HI_U32 enable);

//-------------------------------------------------------------------
//DISP_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//CBAR_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetCbarEnable         (HI_U32 u32hd_id,HI_U32 bTrue);
HI_VOID VDP_DISP_SetCbarSel            (HI_U32 u32hd_id,HI_U32 u32cbar_sel);

//-------------------------------------------------------------------
//CBAR_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//IPU_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDitherMode         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_E enDitherMode);
HI_VOID VDP_DISP_SetDitherRoundEnable  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetDitherEnable       (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetDitherCoef         (HI_U32 u32hd_id, VDP_DISP_INTF_E enChan, VDP_DITHER_COEF_S dither_coefs);

HI_VOID VDP_DISP_SetClipEnable         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);
HI_VOID VDP_DISP_SetClipCoef           (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DISP_CLIP_S stClipData);

HI_VOID VDP_DISP_SetCscEnable          (HI_U32 u32hd_id, HI_U32  enCSC);
HI_VOID VDP_DISP_SetCscDcCoef          (HI_U32 u32hd_id, VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetCscCoef            (HI_U32 u32hd_id, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetCscMode            (HI_U32 u32hd_id, VDP_CSC_MODE_E enCscMode);

HI_VOID VDP_DISP_SetGammaEnable        (HI_U32 u32hd_id, HI_U32 u32GmmEn);
HI_VOID VDP_DISP_SetGammaAddr          (HI_U32 u32hd_id, HI_U32 u32uGmmAddr);
HI_VOID VDP_DISP_SetMirrorEnable       (HI_U32 u32hd_id, HI_U32 u32MirrEn);
HI_VOID VDP_DISP_SetParaUpd            (HI_U32 u32hd_id, VDP_DISP_PARA_E enPara);
HI_VOID VDP_SetMirrorEnable            (HI_U32 u32hd_id,HI_U32 bTrue);
HI_VOID VDP_DISP_SetProToInterEnable   (HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetCoefReadEnable     (HI_U32 u32Id, HI_U32 u32Para);
HI_VOID VDP_DISP_SetCoefReadDisable    (HI_U32 u32Id, HI_U32 u32Para);
//-------------------------------------------------------------------
//IPU_END
//-------------------------------------------------------------------

#if INTF_EN
HI_VOID VDP_INTF_SetDitherMode         (HI_U32 u32Data, VDP_DITHER_E enDitherMode);
HI_VOID VDP_INTF_SetDitherCoef         (HI_U32 u32Data, VDP_DITHER_COEF_S dither_coefs);

HI_VOID VDP_INTF_SetClipEnable         (HI_U32 u32Data, VDP_DISP_INTF_E enIntf);
HI_VOID VDP_INTF_SetClipCoef           (HI_U32 u32Data, VDP_DISP_CLIP_S stClipData);

HI_VOID VDP_INTF_SetCscEnable          (HI_U32 u32Data, HI_U32  enCSC);
HI_VOID VDP_INTF_SetCscDcCoef          (HI_U32 u32Data, VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_INTF_SetCscCoef            (HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_INTF_SetCscMode            (HI_U32 u32Data, VDP_CSC_MODE_E enCscMode);
HI_VOID VDP_INTF_SetClipEnable         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);
HI_VOID VDP_INTF_SetClipCoef           (HI_U32 u32Data, VDP_DISP_INTF_E clipsel, VDP_DISP_CLIP_S stClipData);

HI_VOID VDP_INTF_SetHShrpEnable        (HI_U32 u32Data, HI_BOOL hsp_en);
HI_VOID VDP_INTF_SetHShrpF0coef        (HI_U32 u32Data, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3);
HI_VOID VDP_INTF_SetHShrpF1coef        (HI_U32 u32Data, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3);
HI_VOID VDP_INTF_SetHShrpCoring        (HI_U32 u32Data, HI_U8 hf0_coring, HI_U8 hf1_coring);
HI_VOID VDP_INTF_SetHShrpGain          (HI_U32 u32Data, HI_S16 hf0_gainneg, HI_S16 hf0_gainpos, HI_S16 hf1_gainneg, HI_S16 hf1_gainpos);
HI_VOID VDP_INTF_SetHShrpAdpEn         (HI_U32 u32Data, HI_BOOL hf0_adpshooten, HI_BOOL hf1_adpshooten);
HI_VOID VDP_INTF_SetHShrpWsize         (HI_U32 u32Data, HI_U8 hf0_winsize, HI_U8 hf1_winsize);
HI_VOID VDP_INTF_SetHShrpThrd          (HI_U32 u32Data, HI_U8 hf0_underth, HI_U8 hf0_overth, HI_U8 hf1_underth, HI_U8 hf1_overth);
HI_VOID VDP_INTF_SetHShrpMixratio      (HI_U32 u32Data, HI_U8 hf0_mixratio, HI_U8 hf1_mixratio);
HI_VOID VDP_INTF_SetHShrpEn            (HI_U32 u32Data, HI_BOOL h1_en, HI_BOOL h2_en, HI_BOOL ltih_en, HI_BOOL ctih_en);
HI_VOID VDP_INTF_SetHShrpXti           (HI_U32 u32Data, HI_U8 hf_shootdiv, HI_U8 lti_ratio, HI_U8 ldti_gain, HI_U8 cdti_gain);
HI_VOID VDP_INTF_SetHShrpGlbpara       (HI_U32 u32Data, HI_U8 peak_ratio, HI_U16 glb_overth, HI_U16 glb_underth);

HI_VOID VDP_INTF_SetRegUp              (HI_U32 u32Data);
HI_VOID VDP_INTF_SetHdmiMode           (HI_U32 u32Data, HI_U32 u32hdmi_md);
HI_VOID VDP_INTF_SetSyncInv            (HI_U32 u32Data, VDP_DISP_SYNCINV_S enInv);
#endif

#if SR_EN
//-------------------------------------------------------------------
//SR_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetSrEnable           (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSrDspSta           ( HI_U32 u32Data, VDP_SR_MODE_E enMd ) ;
//HI_VOID VDP_DISP_SetSrMux              (HI_U32 u32Data, VDP_SR_MUX_E SrMux);
//HI_VOID VDP_DISP_SetSrDemoCtrl         (HI_U32 u32Data, VDP_SR_DEMO_E DemoCtrl);
//HI_VOID VDP_DISP_SetSrServeMode        (HI_U32 u32Data, VDP_SR_SERVE_MODE_E Mode);
HI_VOID VDP_DISP_SetSrGrdSubRatio      ( HI_U32 u32Data, HI_U32 u32GrdSubRatio                                            ) ;
HI_VOID VDP_DISP_SetSrGrdHvNear        ( HI_U32 u32Data, HI_U32 u32GrdHvNear                                              ) ;
HI_VOID VDP_DISP_SetSrGrdThr           ( HI_U32 u32Data, HI_U32 u32HighThr, HI_U32 u32LowThr                              ) ;
HI_VOID VDP_DISP_SetSrGrdThrSub        ( HI_U32 u32Data, HI_U32 u32GrdThrSub                                              ) ;
HI_VOID VDP_DISP_SetSrLtexAdd          ( HI_U32 u32Data, HI_U32 u32DetaiGain                                              ) ;
HI_VOID VDP_DISP_SetSrDiscDetailGain   ( HI_U32 u32Data, HI_U32 u32DetaiGain                                              ) ;
HI_VOID VDP_DISP_SetSrSharpenEnable    ( HI_U32 u32Data, HI_U32 u32Enable                                                 ) ;
//HI_VOID VDP_DISP_SetSrSharpenMode    ( HI_U32 u32Data, VDP_SR_SHARP_MODE_E Mode                                         ) ;
//HI_VOID VDP_DISP_SetSrEiscDetailGain ( HI_U32 u32Data, HI_U32 u32DetaiGain                                              ) ;
HI_VOID VDP_DISP_SetSrSharpenCoring    ( HI_U32 u32Data, HI_U32 u32SharpCoring                                            ) ;
HI_VOID VDP_DISP_SetSrSharpenGain      ( HI_U32 u32Data, HI_U32 u32SharpGain                                              ) ;
//HI_VOID VDP_DISP_SetSrSharpenThrsh   ( HI_U32 u32Data, HI_U32 u32LowThr, HI_U32 u32HighThr                              ) ;
HI_VOID VDP_DISP_SetSrSmoothEnable     ( HI_U32 u32Data, HI_U32 u32Enable                                                 ) ;
HI_VOID VDP_DISP_SetSrSmoothPlusEnable ( HI_U32 u32Data, HI_U32 u32Enable                                                 ) ;
HI_VOID VDP_DISP_SetSrSmoothPlusCtrl   ( HI_U32 u32Data, HI_U32 u32PlusCtrl                                               ) ;
HI_VOID VDP_DISP_SetSrDirCtrl          ( HI_U32 u32Data, HI_U32 u32DirHvCtrl,HI_U32 u32DirDgCtrl                          ) ;

HI_VOID VDP_DISP_SetSrZmeEnable        ( HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable                         ) ;
HI_VOID VDP_DISP_SetSrZmePhase         ( HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase                           ) ;
HI_VOID VDP_DISP_SetSrZmeFirEnable     ( HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable                         ) ;
HI_VOID VDP_DISP_SetSrZmeMidEnable     ( HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable                         ) ;
HI_VOID VDP_DISP_SetSrZmeAlphaFirMode  (HI_U32 u32Data, HI_U32 u32HFirMode, HI_U32 u32VFirMode);
HI_VOID VDP_DISP_SetSrZmeHorRatio      ( HI_U32 u32Data, HI_U32 u32Ratio                                                  ) ;
HI_VOID VDP_DISP_SetSrZmeVerRatio      ( HI_U32 u32Data, HI_U32 u32Ratio                                                  ) ;
HI_VOID VDP_DISP_SetSrZmeInFmt         ( HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt                                            ) ;
HI_VOID VDP_DISP_SetSrZmeOutFmt        ( HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt                                            ) ;
HI_VOID VDP_DISP_SetSrHfirOrder        ( HI_U32 u32Data, HI_U32 u32HfirOrder                                              ) ;

HI_VOID VDP_DISP_SetSrGradThres        ( HI_U32 u32Data, HI_U32 u32GradThresH, HI_U32 u32GradThresL                       ) ;
HI_VOID VDP_DISP_SetSrGradFlatThr      ( HI_U32 u32Data, HI_U32 u32GradFlatThr                                            ) ;
HI_VOID VDP_DISP_SetSrRingdtEn         ( HI_U32 u32Data, HI_U32 u32RingdtEn                                               ) ;
HI_VOID VDP_DISP_SetSrGradFlatThr2     ( HI_U32 u32Data, HI_U32 u32GradFlatThr2                                           ) ;
HI_VOID VDP_DISP_SetSrGradSlop         ( HI_U32 u32Data, HI_U32 u32GradSlop                                               ) ;
HI_VOID VDP_DISP_SetSrSkindtEn         ( HI_U32 u32Data, HI_U32 u32SkindtEn                                               ) ;
HI_VOID VDP_DISP_SetSrClipData         ( HI_U32 u32Data, HI_U32 u32ClipData                                               ) ;
HI_VOID VDP_DISP_SetSrSkindtExp        ( HI_U32 u32Data, HI_U32 u32SkindtExpY, HI_U32 u32SkindtExpU, HI_U32 u32SkindtExpV ) ;
HI_VOID VDP_DISP_SetSrSkindtYthr       ( HI_U32 u32Data, HI_U32 u32YThresH, HI_U32 u32YThresL, HI_U32 u32YSlop            ) ;
HI_VOID VDP_DISP_SetSrSkindtUthr       ( HI_U32 u32Data, HI_U32 u32UThresH, HI_U32 u32UThresL, HI_U32 u32USlop            ) ;
HI_VOID VDP_DISP_SetSrSkindtVthr       ( HI_U32 u32Data, HI_U32 u32VThresH, HI_U32 u32VThresL, HI_U32 u32VSlop            ) ;
HI_VOID VDP_DISP_SetSrSharpThr         ( HI_U32 u32Data, HI_U32 u32SrSharpThrH, HI_U32 u32SrSharpThrL                     ) ;
HI_VOID VDP_DISP_SetSrHanceSlop        ( HI_U32 u32Data, HI_U32 u32HighSlop ,HI_U32 u32LowSlop                            ) ;
HI_VOID VDP_DISP_SetSrHanceLowCtrl     ( HI_U32 u32Data, HI_U32 u32LowCtrlH, HI_U32 u32LowCtrlL                           ) ;
HI_VOID VDP_DISP_SetSrHanceHighCtrl    ( HI_U32 u32Data, HI_U32 u32HighCtrlH, HI_U32 u32HighCtrlL                         ) ;
HI_VOID VDP_DISP_SetSrSharpStart       ( HI_U32 u32Data, HI_U32 u32SharpStartH, HI_U32 u32SharpStartL                     ) ;
HI_VOID VDP_DISP_SetSrSharpStrengh     ( HI_U32 u32Data, HI_U32 u32SharpStrengh                                           ) ;
HI_VOID VDP_DISP_SetSrEdgeFactorCtrl   ( HI_U32 u32Data, HI_U32 u32EdgeFactorCtrl                                         ) ;
HI_VOID VDP_DISP_SetSrSkinThres        ( HI_U32 u32Data, HI_U32 u32SkinThresH, HI_U32 u32SkinThresL                       ) ;
HI_VOID VDP_DISP_SetSrRingThres        ( HI_U32 u32Data, HI_U32 u32RingThresH, HI_U32 u32RingThresL                       ) ;
HI_VOID VDP_DISP_SetSrSharpHighCtrl    ( HI_U32 u32Data, HI_U32 u32SharpHighCtrl                                          ) ;
HI_VOID VDP_DISP_SetSrNolinearCtrl     ( HI_U32 u32Data, HI_U32 u32NolinearCtrl                                           ) ;
HI_VOID VDP_DISP_SetSrShootThrsh       ( HI_U32 u32Data, HI_U32 u32ShootThrsh                                             ) ;

//HI_VOID VDP_DISP_SetSrHorCoef          (HI_U32 u32Data, VDP_SR_HORCOEF_S stSrHorCoef);
//HI_VOID VDP_DISP_SetSrVerCoef          (HI_U32 u32Data, VDP_SR_VERCOEF_S stSrVerCoef);

HI_VOID VDP_DISP_SetParaUpd            (HI_U32 u32Data, VDP_DISP_PARA_E enMode);
HI_VOID VDP_DISP_SetSrZmeCoefAddr      (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_DISP_SetSrIReso            (HI_U32 u32Data, HI_U32 u32Sriw, HI_U32 u32Srih);


HI_VOID VDP_DISP_SetSrCscEnable        (HI_U32 u32CscId, HI_U32 enCSC);
HI_VOID VDP_DISP_SetSrCscDcCoef        (HI_U32 u32CscId,VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetSrCscCoef          (HI_U32 u32CscId,VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetSrCscMode          (HI_U32 u32CscId, VDP_CSC_MODE_E enCscMode);
//-------------------------------------------------------------------
//SR_END
//-------------------------------------------------------------------

#endif

//-------------------------------------------------------------------
//DIMMING_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDimmingSelUhd(HI_U32 u32Data, HI_U32 u32SelUhd);
HI_VOID VDP_DISP_SetDimVZmeCoefAddr      ( HI_U32 u32hd_id, HI_U32 u32CoefAddr);
HI_VOID VDP_DISP_SetDimHZmeCoefAddr      ( HI_U32 u32hd_id, HI_U32 u32CoefAddr);
HI_VOID VDP_DISP_SetLocalDimCoefAddr     ( HI_U32 u32hd_id, HI_U32 u32CoefAddr);
HI_VOID VDP_DISP_SetDimmingSclOffsetV    ( HI_U32 u32Data,VDP_ZME_MODE_E  enMode, HI_S32 s32Offset);
HI_VOID VDP_DISP_SetDimmingSclOffsetH    ( HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Offset);
HI_VOID VDP_DISP_SetDimmingSclFirEnable  ( HI_U32 u32Data, VDP_ZME_MODE_E  enMode, HI_U32 bEnable);
HI_VOID VDP_DISP_SetDimmingSclMidEnable  ( HI_U32 u32Data, VDP_ZME_MODE_E  enMode, HI_U32 bEnable);
HI_VOID VDP_DISP_SetDimmingSclEnable     ( HI_U32 u32Data, VDP_ZME_MODE_E  enMode, HI_U32 bEnable);
HI_VOID VDP_DISP_SetDimmingSclRatioV     ( HI_U32 u32Data, HI_U32 u32lvfir_scl_dec,HI_U32 u32lvfir_scl_int);
HI_VOID VDP_DISP_SetDimmingSclRatioH     ( HI_U32 u32Data, HI_U32 u32lhfir_scl_dec,HI_U32 u32lhfir_scl_int);
HI_VOID VDP_DISP_SetDimmingLed0D2D       ( HI_U32 u32Data,HI_U32 u32led2d_final,HI_U32 u32led0d_final);
HI_VOID VDP_DISP_SetDimmingLedPanel      ( HI_U32 u32Data, HI_U32 u32max_led_panel,HI_U32 u32k_led_panel);
HI_VOID VDP_DISP_SetDimmingLcdCompgain   ( HI_U32 u32Data, HI_U32 u32lcd_compgain);
HI_VOID VDP_DISP_SetDimming2DLcdComp     ( HI_U32 u32Data, HI_U32 u32lcd_k_led, HI_U32 u32lcd_core_led);
HI_VOID VDP_DISP_SetDimming0DLcdComp     ( HI_U32 u32Data, HI_U32 u32lcd_k_led, HI_U32 u32lcd_core_led);
HI_VOID VDP_DISP_SetDimmingPostFirGain   ( HI_U32 u32Data, HI_U32 u32k_led2d, HI_U32 u32core_led2d, HI_U32 u32gain_tf);
HI_VOID VDP_DISP_SetDimmingPostFirBld    ( HI_U32 u32Data, HI_U32 u32gain_hfir,HI_U32 u32gain_vfir);
HI_VOID VDP_DISP_SetDimmingPre0D2DBld    ( HI_U32 u32Data, HI_U32 u32core_led0d, HI_U32 u32k_led0d);
HI_VOID VDP_DISP_SetDimmingStatMetrics   ( HI_U32 u32Data,HI_U32 u32stat_height ,HI_U32 u32stat_width);
HI_VOID VDP_DISP_SetDimmingPreBld        ( HI_U32 u32Data, HI_U32 u32core_led, HI_U32 u32k_led);
HI_VOID VDP_DISP_SetDimming0DLedStatus   ( HI_U32 u32Data, HI_U32 u32led0d_sw, HI_U32 u32sc_0d_sw);
HI_VOID VDP_DISP_SetDimmingFinalClip2D   ( HI_U32 u32Data, HI_U32 u32clip_btm_final_2d,HI_U32 u32clip_up_final_2d);
HI_VOID VDP_DISP_SetDimmingFinalClip     ( HI_U32 u32Data, HI_U32 u32clip_btm_final_0d,HI_U32 u32clip_up_final_0d);
HI_VOID VDP_DISP_SetDimmingFinalGain2D   ( HI_U32 u32Data, HI_U32 u32gain_final_2d,HI_S32 u32offset_final_2d);
HI_VOID VDP_DISP_SetDimmingFinalGain     ( HI_U32 u32Data, HI_U32 u32gain_final_0d,HI_S32 u32offset_final_0d);
HI_VOID VDP_DISP_SetDimming2DLedGainMisc ( HI_U32 u32Data, HI_U32 u32gain_led2d,HI_U32 u32gain_bldmax2d,HI_U32 u32gain_bld_0d2d);
HI_VOID VDP_DISP_SetDimming0DLedGainMisc ( HI_U32 u32Data, HI_U32 u32gain_led0d ,HI_U32 u32gain_led0d_tf);
HI_VOID VDP_DISP_SetDimming0DLedCore     ( HI_U32 u32Data,HI_U32 u32core_dstat_high ,HI_U32 u32core_dstat_mid,HI_U32 u32core_dstat_low,HI_U32 u32k_dstat_0d);
HI_VOID VDP_DISP_SetDimmingfirCoef       ( HI_U32 u32Data, VDP_DISP_DIMMING_FIR_COEF_S stCoef );
HI_VOID VDP_DISP_SetDimming2DLedThd      ( HI_U32 u32Data,VDP_DISP_DIMMING_THD_S *pstThd2D);
HI_VOID VDP_DISP_SetDimming0DLedThd      ( HI_U32 u32Data, VDP_DISP_DIMMING_THD_S *pstThd);
HI_VOID VDP_DISP_SetDimmingHistStat      ( HI_U32 u32Data, HI_U32 u32thresh_md2 ,HI_U32 u32thresh_md0, HI_U32 u32bits_w_hist ,HI_U32 u32bits_b_hist);
HI_VOID VDP_DISP_SetDimmingNormUnit      ( HI_U32 u32Data, HI_U32 u32glb_norm_unit ,HI_U32 u32seg_norm_unit);
HI_VOID VDP_DISP_SetDimmingSegMetircs    ( HI_U32 u32Data,HI_U32 u32seg_height ,HI_U32 u32seg_width);
HI_VOID  VDP_DISP_SetDimmingStartPos     ( HI_U32 u32Data, HI_U32 u32hcnt_init ,HI_U32 u32vcnt_init);
HI_VOID VDP_DISP_SetDimmingLedNum        ( HI_U32 u32Data,HI_U32 u32lednumv,HI_U32 u32lednumh);
HI_VOID VDP_DISP_SetDimmingModeInitLed   ( HI_U32 u32Data, HI_U32 u32modeinitled);
HI_VOID VDP_DISP_SetDimmingdisable_2d    ( HI_U32 u32Data,HI_U32 disable_2d);
HI_VOID VDP_DISP_SetDimminglcdbypass     ( HI_U32 u32Data,HI_U32 bypasslcd);
HI_VOID VDP_DISP_SetDimmingledbypass     ( HI_U32 u32Data, HI_U32 bypassled);
HI_VOID VDP_DISP_SetDimmingbEn           ( HI_U32 u32Data, HI_U32 bEn);


//-------------------------------------------------------------------
//DIMMING_END
//-------------------------------------------------------------------



HI_VOID VDP_DISP_SetHShrpEnable        (VDP_CHN_E enLayer, HI_BOOL hsp_en);
HI_VOID VDP_DISP_SetHShrpF0coef        (VDP_CHN_E enLayer, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3);
HI_VOID VDP_DISP_SetHShrpF1coef        (VDP_CHN_E enLayer, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3);
HI_VOID VDP_DISP_SetHShrpCoring        (VDP_CHN_E enLayer, HI_U8 hf0_coring, HI_U8 hf1_coring);
HI_VOID VDP_DISP_SetHShrpGain          (VDP_CHN_E enLayer, HI_S16 hf0_gainneg, HI_S16 hf0_gainpos, HI_S16 hf1_gainneg, HI_S16 hf1_gainpos);
HI_VOID VDP_DISP_SetHShrpAdpEn         (VDP_CHN_E enLayer, HI_BOOL hf0_adpshooten, HI_BOOL hf1_adpshooten);
HI_VOID VDP_DISP_SetHShrpWsize         (VDP_CHN_E enLayer, HI_U8 hf0_winsize, HI_U8 hf1_winsize);
HI_VOID VDP_DISP_SetHShrpThrd          (VDP_CHN_E enLayer, HI_U8 hf0_underth, HI_U8 hf0_overth, HI_U8 hf1_underth, HI_U8 hf1_overth);
HI_VOID VDP_DISP_SetHShrpMixratio      (VDP_CHN_E enLayer, HI_U8 hf0_mixratio, HI_U8 hf1_mixratio);
HI_VOID VDP_DISP_SetHShrpEn            (VDP_CHN_E enLayer, HI_BOOL h1_en, HI_BOOL h2_en, HI_BOOL ltih_en, HI_BOOL ctih_en);
HI_VOID VDP_DISP_SetHShrpXti           (VDP_CHN_E enLayer, HI_U8 hf_shootdiv, HI_U8 lti_ratio, HI_U8 ldti_gain, HI_U8 cdti_gain);
HI_VOID VDP_DISP_SetHShrpGlbpara       (VDP_CHN_E enLayer, HI_U8 peak_ratio, HI_U16 glb_overth, HI_U16 glb_underth);

HI_VOID VDP_DISP_SetOneSyncTwoEnable   (HI_U32 u32Enable);
HI_VOID VDP_DISP_SetDhd1Select         (HI_U32 u32Data);


HI_VOID VDP_DISP_SetTriggerRow(HI_U32 u32Data, HI_U32 trigger_row);

HI_VOID VDP_DISP_SetTriggerEnable      (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSdSyncEnable         (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSdSyncFrmNum         (HI_U32 u32Data, HI_U32 frm_num);


//LVDS
HI_VOID VDP_INTF_SetLvdsEnable         (HI_U32 bEnable);
HI_VOID VDP_INTF_SetLvdsMode           (HI_U32 enMode);
HI_VOID VDP_INTF_SetLvdsBitMode        (HI_BOOL bBitSel);
HI_VOID VDP_INTF_SetLvdsSyncOutEn      (HI_U32 bSyncOutEn);
HI_VOID VDP_INTF_SetLvdsSwap           (HI_U32 u32Swap);
HI_VOID VDP_INTF_SetLvdsFormat         (VDP_LVDS_FORMAT_E enFormat);
HI_VOID VDP_INTF_SetLvdsTestMode       (VDP_LVDS_TEST_MODE_E enTestMode);
HI_VOID VDP_INTF_SetLvdsInvert         (VDP_LVDS_INV_E enInv, HI_U32 bInvEn);

HI_VOID VDP_DISP_SetStartPos (HI_U32 u32Data, HI_U32 u32StartPos);
HI_VOID VDP_DISP_SetParaLoadPos (HI_U32 u32Data, HI_U32 u32StartPos);

HI_VOID VDP_DISP_SetNxgDataSwapEnable(HI_U32 u32hd_id, HI_U32 enable);
HI_U32  VDP_DISP_SetReadCheckSumY  (HI_VOID);
HI_U32	VDP_DISP_SetReadCheckSumU  (HI_VOID);
HI_U32	VDP_DISP_SetReadCheckSumV  (HI_VOID);
HI_VOID VDP_DISP_GetVactState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt);

#endif
