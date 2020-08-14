#ifndef __VDP_SOFTWARE_SELFDEFINE_H_
#define __VDP_SOFTWARE_SELFDEFINE_H_
#define DHD_VTXTHD_FIELD_MODE 1
#define DHD_VTXTHD_FRAME_MODE 0

#include "hi_reg_common.h"
#include "vdp_define.h"
#include "vdp_fpga_define.h"

HI_VOID VDP_VID_SetRegionEnable_sdk(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable );
HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id);
HI_VOID VDP_CloseClkResetModule(HI_VOID);
HI_VOID VDP_VDAC_Reset(HI_VOID);

HI_VOID  VDP_DHD1_CBM_MIX_Init(HI_VOID);
HI_VOID  VDP_DHD1_CBM_MIX_DeInit(HI_VOID);
HI_VOID  VDP_DispMask(HI_DRV_DISPLAY_E eChn);
HI_VOID  VDP_DispUnMask(HI_DRV_DISPLAY_E eChn);


HI_VOID  VDP_DRIVER_Initial(HI_VOID);
HI_VOID  VDP_OpenClkAndInitCrgConfig(HI_VOID);
HI_VOID  VDP_DRIVER_SetVirtualAddr(S_VDP_REGS_TYPE *pstVdpReg);
HI_VOID VDP_VDAC_GetEnable(HI_U32 uVdac, HI_U32 *penable);
HI_VOID VDP_RegReStore(HI_U8 *pu8RegBackAddr);
HI_VOID VDP_RegSave(HI_U8 *pu8RegBackAddr);
HI_VOID VDP_VDAC_ResetFmt(DISP_VENC_E enVenc ,HI_U32 uVdac, HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk);
HI_VOID VDP_DISP_GetIntfEnable(HI_U32 u32hd_id, HI_U32 *pbTrue);
HI_U32 VDP_DISP_GetMaskIntSta(HI_U32 u32intmask);
HI_VOID VDP_DISP_SetHdmiClk  (HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv);
HI_VOID VDP_VDAC_SetClockEnable(HI_U32 uVdac, HI_U32 enable);
HI_VOID  VDP_WBC_SetZmeInFmt_Define1(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt);
HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH);
HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode);
HI_S32 VDP_DISP_SelectChanClkDiv(HI_DRV_DISPLAY_E eChn, HI_U32 u32Div);
HI_VOID VDP_VBI_SetCgmsTypeBEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeB);
HI_VOID VDP_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data);

HI_VOID VDP_MVCN_SetMcvnEnable(HI_BOOL bEnable);
HI_VOID VDP_MIXV_SetPrio(HI_U32 prioLayer);
HI_VOID VDP_MIXV_SetPrio1(HI_U32 prioLayer);
HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data);
HI_VOID VDP_DATE_SetHTiming(HI_U32 u32Chan, HI_U32 *pu32SrcCoef);
HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset);
HI_VOID VDP_DISP_GetIntMask (HI_U32 *pu32masktype);
HI_U32 VDP_WBC_GetAlwaysProFlag(HI_BOOL *bAlwaysPro);
HI_U32 VDP_DHD_GetDispMode(HI_U32 u32Data);
HI_VOID VDP_DISP_GetVtThd(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 *pu32vtthd);

HI_VOID  VDP_VID_GetInReso      (HI_U32 u32Data, HI_RECT_S *pstRect);
HI_VOID VDP_VID_GetInDataFmt (HI_U32 u32Data, HI_DRV_PIX_FORMAT_E *penDataFmt);
HI_VOID  VDP_VID_GetLayerAddrAndStride  (HI_U32 u32Data,
                                        HI_U32 *pu32LAddr,
                                        HI_U32 *pu32CAddr,
                                        HI_U32 *pu32LStr,
                                        HI_U32 *pu32CStr);
HI_VOID VDP_VID_GetLayerEnableFlag (HI_U32 u32Data, HI_BOOL *pbVideoLogoMode);
HI_VOID SDATE_VBI_CCLineConfig(HI_BOOL TopEnable, HI_BOOL BottomEnable, HI_U16 TopLine, HI_U16 BottomLine);
HI_VOID SDATE_VBI_CCDataConfig(HI_U16 TopData, HI_U16 BottomData);
HI_VOID SDATE_VBI_WSSConfig(HI_BOOL WssEnable, HI_U16 WssData);
HI_BOOL SDATE_VBI_IntStatus(HI_VOID);
HI_VOID SDATE_VBI_TtxIntClear(HI_VOID);
HI_VOID SDATE_VBI_TtxSetMode(HI_U32 TtxMode);
HI_VOID SDATE_VBI_TtxSetSeq(HI_BOOL TtxSeq);
HI_VOID SDATE_VBI_TtxLineEnable(HI_U32 WhichRow, HI_U32 WhichExtRow);
HI_VOID SDATE_VBI_TtxSetAddr(HI_U32 StartAddr, HI_U32 EndAddr);
HI_VOID SDATE_VBI_TtxSetReady(HI_VOID);
HI_VOID SDATE_VBI_TtxSetPackOff(HI_U32 PackOff);
HI_VOID SDATE_VBI_TtxPiorityHighest(HI_BOOL Highest);
HI_VOID SDATE_VBI_SetTtxFullPage(HI_BOOL Flag);
HI_VOID SDATE_VBI_VbiFilterEnable(HI_BOOL VbiFilterEnable);
HI_VOID SDATE_VBI_TtxEnableInt(HI_VOID);
HI_VOID SDATE_VBI_TtxDisableInt(HI_VOID);
HI_VOID SDATE_VBI_CCSetSeq(HI_BOOL CCSeq);
HI_VOID SDATE_HDATE_SetSrcCoef(DISP_VENC_E eDate, HI_U32 *pu32Src13);
HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data);
HI_VOID SDATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable);
HI_VOID SDATE_HDATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync);
HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data);
HI_VOID SDATE_SetHTiming(HI_U32 u32Chan, HI_U32 *pu32SrcCoef);
/* set length u32Data of one line */
HI_BOOL  SDATE_SetLenMode(HI_U32 u32Data);
HI_VOID SDATE_HDATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt);
HI_VOID SDATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta);
HI_VOID SDATE_VDAC_SetLink(DISP_VENC_E eDate, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal);
HI_VOID VDP_VDAC_SetEnable(HI_U32 uVdac, HI_U32 enable,HI_U32 u32DelayMs);
HI_VOID SDATE_HDATE_VBI_SetCgmsTypeAEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeA);
HI_VOID SDATE_HDATE_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data);
HI_VOID SDATE_HDATE_VBI_SetWssData(DISP_VENC_E enChanel,HI_U32 u32Data);
HI_VOID SDATE_HDATE_VBI_SetWssEn(DISP_VENC_E enChanel,HI_BOOL bEnWss);
HI_S32 SDATE_MVCN_DATE_SetMcvn(HI_U32 *pu32Value);
HI_VOID VDP_VDAC_SetGain(HI_U32 Gain);
HI_VOID SDATE_SYNC_SetSrcCoef(HI_U32 coef1,HI_U32 coef2,HI_U32 coef3,HI_U32 coef4);
HI_VOID SDATE_SCH_SetPhaseCoef(HI_U32 coef);
HI_VOID SDATE_CLIPS_SetEn(HI_U32 clips);
HI_VOID SDATE_CVBS_SetGain(HI_U32 cvbs_gain_en,HI_U32 ycvbs_gain,HI_U32 u_gain,HI_U32 v_gain);
HI_VOID SDATE_LUMA_SetDelay(HI_U32 delay);
#endif


