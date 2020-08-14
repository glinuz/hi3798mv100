#ifndef __VDP_SOFTWARE_SELFDEFINE_H_
#define __VDP_SOFTWARE_SELFDEFINE_H_

#include "vdp_define.h"
#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_disp.h"
#include "drv_disp_osal.h"

typedef enum tagDISP_VENC_E
{
    DISP_VENC_HDATE0 = 0,
    DISP_VENC_SDATE0,
    DISP_VENC_VGA0,
    DISP_VENC_MAX
}DISP_VENC_E;

typedef struct tagVDP_DISP_SYNCINV_E
{
    HI_U32  u32Dinv;
    HI_U32  u32DatHinv;
    HI_U32  u32DatVinv;

    HI_U32  u32Hdfinv;
    HI_U32  u32Hdvinv;
    HI_U32  u32Hdhinv;
    HI_U32  u32Hddinv;

    HI_U32  u32Vgavinv;
    HI_U32  u32Vgahinv;
    HI_U32  u32Vgadinv;

    HI_U32  u32Lcdvinv;
    HI_U32  u32Lcdhinv;
    HI_U32  u32Lcddinv;

}VDP_DISP_SYNCINV_E;


#define DHD_VTXTHD_FIELD_MODE 1
#define DHD_VTXTHD_FRAME_MODE 0
#define DHD_YUV_TO_HDMI 0
#define DHD_RGB_TO_HDMI 1

HI_VOID  VDP_DRIVER_SetVirtualAddr(HI_U32 virAddr);
HI_VOID  VDP_DRIVER_Initial(HI_VOID);
HI_VOID  VDAC_DRIVER_Initial(HI_VOID);
HI_VOID  VDP_VID_Layer4LinkToMix2( HI_BOOL bEnable );
HI_VOID  VDP_VID_MultiSetRegionReso(HI_U32 u32Data, HI_U32 u32RegionNo,HI_RECT_S stRect);
HI_VOID  VDP_VP_GetCscReg(HI_U32 u32Data, HI_U32 *pdata, HI_U32 u32Num);
HI_VOID  VDP_VID_GetCscReg(HI_U32 u32Data, HI_U32 *pdata);
HI_VOID  VDP_VP_SetCscReg(HI_U32 u32Data, HI_U32 *pdata, HI_U32 u32Num);
HI_VOID  VDP_VID_SetCscReg(HI_U32 u32Data, HI_U32 *pdata);
HI_VOID  VDP_VP_GetLayerReso(HI_U32 u32Data, HI_RECT_S *stRect);
HI_VOID  VDP_VP0_SetAccCad(HI_U32 u32Data, HI_U32 u32addr);
HI_VOID VDP_VP_SetAccThd( HI_U32 u32Data,ACCTHD_S stAccThd);
HI_VOID VDP_VP_SetAccCtrl(HI_U32 u32Data,HI_U32 uAccEn, HI_U32 uAccMode);
HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id);
HI_VOID VDP_DISP_SetFramePackingEn(HI_U32 u32hd_id, HI_U32 u32Enable);
HI_VOID VDP_DISP_SetHdmiClk(HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv);
HI_VOID VDP_DISP_GetIntMask(HI_U32 *pu32masktype);

HI_VOID VDP_DISP_SetSyncInv(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DISP_SYNCINV_E enInv);


HI_VOID  VDP_DISP_SetCscEnable(HI_U32 u32hd_id, HI_U32 enCSC);

HI_VOID VDP_DISP_SetDataSel(HI_U32 u32hd_id, HI_U32 u32DataSel);
HI_VOID   VDP_DISP_SetCscDcCoef(HI_U32 u32hd_id,VDP_CSC_DC_COEF_S stCscCoef);
HI_VOID   VDP_DISP_SetCscCoef(HI_U32 u32hd_id,VDP_CSC_COEF_S stCscCoef);
HI_VOID VDP_DISP_SetGammaMode(HI_U32 u32hd_id, HI_U32 u32GmmEn);
HI_VOID VDP_WBC_SetOutFmtUVOrder(VDP_LAYER_WBC_E enLayer, HI_U32 uvOrder);
HI_VOID VDP_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID VDP_WBC_SetZmePhaseV(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID VDP_VP_SetAcmDbgEn(HI_U32 u32Data , HI_U32 u32bAcmDbgEn);

HI_VOID VDP_VP_SetAcmStretch(HI_U32 u32Data, HI_U32 u32AcmStrch);

HI_VOID VDP_VP_SetAcmClipRange(HI_U32 u32Data, HI_U32 u32AcmClipRange);

HI_VOID VDP_VP_SetAcmCliporwrap(HI_U32 u32Data, HI_U32 u32AcmCliporwrap);
HI_VOID VDP_VP_SetAcmChmThd(HI_U32 u32Data, HI_U32 u32AcmCbcrthr);


HI_VOID VDP_VP_SetAcmGainLum(HI_U32 u32Data, HI_U32 u32AcmGain0);
HI_VOID VDP_VP_SetAcmGainSat(HI_U32 u32Data, HI_U32 u32AcmGain2);
HI_VOID VDP_VP_SetAcmGainHue(HI_U32 u32Data, HI_U32 u32AcmGain1);
HI_VOID VDP_VP_SetAcmCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr);

HI_VOID VDP_DHD_DEFAULT(HI_VOID);

HI_VOID VDP_DSD_DEFAULT(HI_VOID);

HI_VOID VDP_MIXV_SetPrio(HI_U32 prioLayer);
HI_VOID VDP_MIXV_SetPrio1(HI_U32 prioLayer);

HI_U32 VDP_DHD_GetDispMode(HI_U32 u32Data);


HI_U32 VDP_WBC_GetAlwaysProFlag(/*VDP_LAYER_WBC_E enLayer,*/ HI_BOOL *bAlwaysPro);

HI_U32 VDP_DISP_GetMaskIntSta(HI_U32 u32intmask);

HI_VOID  VDP_VID_SetInDataUVOrder(HI_U32 u32Data, HI_U32  VFirst);

HI_VOID VDP_DATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta);

HI_VOID VDP_DATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable);
HI_VOID VDP_VDAC_GetEnable(HI_U32 uVdac, HI_U32 *penable);


HI_VOID VDP_DATE_SetSrcCoef(DISP_VENC_E eDate, HI_U32 *pu32Src13);
HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data);
HI_VOID VDP_DATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt);
HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data);
HI_VOID VDP_VDAC_ResetCRG(HI_VOID);
HI_VOID VDP_VDAC_Reset(HI_VOID);
HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset);
HI_U32 VDP_GetVGADacReviseValue(HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk);
HI_VOID VDP_VDAC_ResetFmt(DISP_VENC_E enVenc ,HI_U32 uVdac, HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk);
HI_VOID VDP_DATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync);
HI_VOID VDP_VDAC_SetLink(DISP_VENC_E eDate, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal);

HI_VOID VDP_VDAC_SetEnable(HI_U32 uVdac, HI_U32 enable,HI_U32 u32DelayMs);

HI_VOID VDP_VDAC_SetClockEnable(HI_U32 uVdac, HI_U32 enable);

HI_VOID  VDP_VID_SetInReso2(HI_U32 u32Data, HI_RECT_S *pstRect, HI_RECT_S *pstRectOrigin);

HI_VOID  VDP_VID_SetInCrop(HI_U32 u32Data, HI_U32 xstartposition, HI_U32 xstopposition);

HI_VOID  VDP_VID_SetOutReso2(HI_U32 u32Data, HI_RECT_S *pstRect);

HI_VOID  VDP_VID_SetVideoPos2(HI_U32 u32Data, HI_RECT_S *pstRect);

HI_VOID  VDP_VID_SetDispPos2(HI_U32 u32Data, HI_RECT_S *pstRect);

HI_VOID  VDP_VID_SetZmeEnable2(HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID  VDP_VID_SetZmePhaseH(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID  VDP_VID_SetZmePhaseV_Define1(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID  VDP_VID_SetZmePhaseVB(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
HI_VOID  VDP_VID_SetZmeFirEnable2(HI_U32 u32Data,HI_U32 u32bEnableHl,HI_U32 u32bEnableHc,HI_U32 u32bEnableVl,HI_U32 u32bEnableVc);
HI_VOID  VDP_VID_SetZmeMidEnable2(HI_U32 u32Data, HI_U32 u32bEnable);
HI_VOID  VDP_VID_SetZmeVchTap(HI_U32 u32Data, HI_U32 u32VscTap);
HI_VOID  VDP_VID_SetLayerAddr(HI_U32 u32Data, HI_U32 u32Chan, HI_U32 regionNo, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr);

HI_VOID  VDP_DISP_SetTiming(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S *pstSyncInfo);

HI_VOID VDP_DISP_GetVactState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt);

HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode);
HI_S32 VDP_DISP_SelectChanClkDiv(HI_DRV_DISPLAY_E eChn, HI_U32 u32Div);
HI_VOID DISP_ResetCRG(HI_VOID);
HI_VOID VDP_RegSave(HI_U32 u32RegBackAddr);
HI_VOID VDP_RegReStore(HI_U32 u32RegBackAddr);
HI_VOID VDP_CloseClkResetModule(HI_VOID);
HI_VOID VDP_DISP_SetProToInterEnable(HI_U32 u32hd_id, HI_U32 u32Enable);
HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH);
HI_VOID VDP_FUNC_SetIntf(VDP_INTF_MODE_E enVdpIntfMode);
HI_VOID  VDP_VID_SetSrOutReso(HI_U32 u32Data, HI_RECT_S stRect);

HI_VOID  VDP_VP_SetLayer_InReso(HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID  VDP_VP_SetLayer_OutReso(HI_U32 u32Data, VDP_DISP_RECT_S  stRect);
HI_VOID VDP_DISP_GetIntfEnable(HI_U32 u32hd_id, HI_U32 *pbTrue);

extern HI_S32 VDP_MVCN_DATE_SetMcvn(HI_U32 *pu32Value);


extern HI_VOID VDP_MVCN_SetMcvnEnable(HI_BOOL bEnable);
extern HI_VOID VDP_VBI_SetCgmsTypeAEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeA);
extern HI_VOID VDP_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data);

extern HI_VOID VDP_VBI_SetCgmsTypeBEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeB);
extern HI_VOID VDP_VBI_SetCgmsTypeB(DISP_VENC_E enChanel,HI_U32 u32Data);
#endif


