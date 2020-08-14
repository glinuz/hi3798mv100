#ifndef __VDP_DRV_CHN_H__
#define __VDP_DRV_CHN_H__

//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
#include "vdp_define.h"
#include "hi_type.h"


//-------------------------------------------------------------------
//DISP_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDispMode           (HI_U32 u32Data, HI_U32 u32DispMode);
HI_VOID VDP_DISP_GetDispMode        (HI_U32 u32hd_id, HI_U32 *p3DMode);
//HI_VOID VDP_DISP_SetFramePackingEn     (HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetHdmiMode           (HI_U32 u32hd_id, HI_U32 u32hdmi_md);
HI_VOID VDP_DISP_SetRegUp              (HI_U32 u32hd_id);
HI_VOID VDP_DISP_SetIntfEnable         (HI_U32 u32hd_id, HI_U32 bTrue);
HI_VOID VDP_DISP_GetIntfEnable         (HI_U32 u32hd_id, HI_U32 *pbTrue);
HI_VOID VDP_DISP_SetIntMask            (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetIntDisable         (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetOflIntMask         (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_SetOflIntDisable      (HI_U32 u32masktypeen);
HI_VOID VDP_DISP_BfmClkSel             (HI_U32 u32Num);
HI_VOID VDP_DISP_OpenIntf              (HI_U32 u32hd_id, VDP_DISP_SYNCINFO_S stSyncInfo);
HI_VOID VDP_DISP_SetVSync              (HI_U32 u32hd_id, HI_U32 u32vfb, HI_U32 u32vbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetVSyncPlus          (HI_U32 u32hd_id, HI_U32 u32bvfb, HI_U32 u32bvbb, HI_U32 u32vact);
HI_VOID VDP_DISP_SetHSync              (HI_U32 u32hd_id, HI_U32 u32hfb, HI_U32 u32hbb, HI_U32 u32hact);
HI_VOID VDP_DISP_SetPlusWidth          (HI_U32 u32hd_id, HI_U32 u32hpw, HI_U32 u32vpw);
HI_VOID VDP_DISP_SetPlusPhase          (HI_U32 u32hd_id, HI_U32 u32ihs, HI_U32 u32ivs, HI_U32 u32idv);
HI_VOID VDP_DISP_SetIntfMuxSel         (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf);

HI_U32  VDP_DISP_GetIntSta             (HI_U32 u32intmask);
HI_VOID VDP_DISP_ClearIntSta           (HI_U32 u32intmask);
HI_VOID VDP_DISP_SetSrMode(VDP_SR_PARA_INFO SrParaInfo, VDP_VP_SR_MODE_E SrMode);

HI_VOID VDP_DISP_SetVtThdMode          (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode);
HI_VOID VDP_DISP_SetVtThd              (HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd);
HI_VOID VDP_DISP_SetWcgEnable         (HI_U32 u32Data, HI_U32 u32Enable);

HI_U32  VDP_DISP_GetOflIntSta(HI_U32 u32intmask);


HI_VOID  VDP_DISP_ClearOflIntSta(HI_U32 u32intmask);
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


HI_VOID VDP_DISP_SetGammaEnable        (HI_U32 u32hd_id, HI_U32 u32GmmEn);
HI_VOID VDP_DISP_SetGammaAddr          (HI_U32 u32hd_id, HI_U32 u32uGmmAddr);
HI_VOID VDP_DISP_SetMirrorEnable       (HI_U32 u32hd_id, HI_U32 u32MirrEn);
HI_VOID VDP_DISP_SetParaUpd            (HI_U32 u32hd_id, VDP_DISP_PARA_E enPara);
HI_VOID VDP_SetMirrorEnable            (HI_U32 u32hd_id,HI_U32 bTrue);
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
HI_VOID VDP_INTF_SetSyncInv(HI_U32 u32Data, VDP_DISP_SYNCINV_S enInv);
#endif

HI_VOID VDP_DISP_SetSrEnable           (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSrMux              (HI_U32 u32Data, VDP_SR_MUX_E SrMux);
HI_VOID VDP_DISP_SetSrDemoMode         (HI_U32 u32Data, SR_DEMO_MODE_E Democtrl);
HI_VOID VDP_DISP_SetSrDemoCtrl         (HI_U32 u32Data, VDP_SR_DEMO_E DemoCtrl);
HI_VOID VDP_DISP_SetSrServeMode        (HI_U32 u32Data, VDP_SR_SERVE_MODE_E Mode);
HI_VOID VDP_DISP_SetSrGrdSubRatio      (HI_U32 u32Data, HI_U32 u32GrdSubRatio);
HI_VOID VDP_DISP_SetSrGrdThr           (HI_U32 u32Data, HI_U32 u32HighThr, HI_U32 u32LowThr);
HI_VOID VDP_DISP_SetSrGrdThrSub        (HI_U32 u32Data, HI_U32 u32GrdThrSub);
HI_VOID VDP_DISP_SetSrLtexAdd          (HI_U32 u32Data, HI_U32 u32DetaiGain);
HI_VOID VDP_DISP_SetSrDiscDetailGain   (HI_U32 u32Data, HI_U32 u32DetaiGain);
HI_VOID VDP_DISP_SetSrSharpenEnable    (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSrSharpenMode      (HI_U32 u32Data, VDP_SR_SHARP_MODE_E Mode);
HI_VOID VDP_DISP_SetSrEiscDetailGain   (HI_U32 u32Data, HI_U32 u32DetaiGain);
HI_VOID VDP_DISP_SetSrSharpenCoring    (HI_U32 u32Data, HI_U32 u32SharpCoring);
HI_VOID VDP_DISP_SetSrSharpenGain      (HI_U32 u32Data, HI_U32 u32SharpGain);
HI_VOID VDP_DISP_SetSrSharpenThrsh     (HI_U32 u32Data, HI_U32 u32LowThr, HI_U32 u32HighThr);
HI_VOID VDP_DISP_SetSrSmoothEnable     (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSrSmoothPlusEnable (HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetSrSmoothPlusCtrl   (HI_U32 u32Data, HI_U32 u32PlusCtrl);

HI_VOID VDP_DISP_SetSrZmeEnable        (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_DISP_SetSrZmePhase         (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID VDP_DISP_SetSrZmeFirEnable     (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_DISP_SetSrZmeMidEnable     (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID VDP_DISP_SetSrZmeHorRatio      (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_DISP_SetSrZmeVerRatio      (HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID VDP_DISP_SetSrZmeCoefAddr      (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_DISP_SetSrZmeInFmt         (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_DISP_SetSrZmeOutFmt        (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt);
HI_VOID VDP_DISP_SetSrHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder);

HI_VOID  VDP_DISP_SetParaRd (HI_U32 u32Data, VDP_DISP_PARA_E enMode);
HI_VOID VDP_DISP_SetParaUp            (HI_U32 u32Data, VDP_DISP_PARA_E enMode);
HI_VOID VDP_DISP_SetSrZmeCoefAddr      (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID VDP_DISP_SetSrIReso            (HI_U32 u32Data, HI_U32 u32Sriw, HI_U32 u32Srih);

HI_VOID VDP_DISP_SetSrInReso           (HI_U32 u32Data, HI_U32 u32Width, HI_U32 u32Height);
HI_VOID VDP_DISP_SetSrCscEnable        (HI_U32 u32CscId, HI_U32 enCSC);
HI_VOID VDP_DISP_SetSrCscDcCoef        (HI_U32 u32CscId,VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetSrCscCoef          (HI_U32 u32CscId,VDP_CSC_COEF_S stCscCoef);

HI_VOID VDP_DISP_GetDHDState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt,
        HI_U32 *pu32IntCnt, HI_BOOL *pbRight);

//LVDS
HI_VOID VDP_INTF_SetLvdsEnable         (HI_U32 bEnable);
HI_VOID VDP_INTF_SetLvdsBitMode         (HI_U32 bBitSel);
HI_VOID VDP_INTF_SetLvdsMode           (HI_U32 enMode);
HI_VOID VDP_INTF_SetLvdsSyncOutEn      (HI_U32 bSyncOutEn);
HI_VOID VDP_INTF_SetLvdsSwap           (HI_U32 u32Swap);
HI_VOID VDP_INTF_SetLvdsFormat         (VDP_LVDS_FORMAT_E enFormat);
HI_VOID VDP_INTF_SetLvdsTestMode       (VDP_LVDS_TEST_MODE_E enTestMode);
HI_VOID VDP_INTF_SetLvdsInvert         (VDP_LVDS_INV_E enInv, HI_U32 bInvEn);

HI_VOID VDP_DISP_StartPos(HI_U32 u32hd_id, HI_U32 startpos);
HI_VOID VDP_DISP_TimingStartPos(HI_U32 u32hd_id, HI_U32 TimingStartPos);

/*3D  */
HI_VOID VDP_DISP_SetHimaxIpSel(VDP_IP_SEL_E enConnect);
#endif


