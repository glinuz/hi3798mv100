#ifndef __VDP_DRV_COMM_H__
#define __VDP_DRV_COMM_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "vdp_define.h"
#include "vdp_reg.h"



HI_U32 VDP_RegRead(HI_U32 a);
HI_VOID VDP_RegWrite(HI_U32 a, HI_U32 b);

HI_U32 VDP_REG_Init(HI_VOID **ppvVdpRegVir);
HI_VOID VDP_REG_DeInit(HI_VOID);


//-------------------------------------------------------------------
//Bus Function BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_SetClkGateEn  (HI_U32 u32Data);
HI_VOID VDP_SetWrOutStd   (HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetRdOutStd   (HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetM0AndM1Sel (HI_U32 u32Data, HI_U32 u32MasterNum);
HI_VOID VDP_SetM2AndM3Sel (HI_U32 u32Data, HI_U32 u32MasterNum);
HI_VOID VDP_SetArbMode    (HI_U32 u32Data, HI_U32 u32bMode);
HI_VOID VDP_SetRdBusId    (HI_U32 u32bMode);
//-------------------------------------------------------------------
//Bus Function END
//-------------------------------------------------------------------

//HI_VOID VDP_HDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetStart  ( HI_U32 u32Data) ;
////SDATE                                              ;
//HI_VOID VDP_SDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetStart  ( HI_U32 u32Data) ;


//-------------------------------------------------------------------
//MIXER_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_CBM_SetMixerBkg              (VDP_CBM_MIX_E u32mixer_id, VDP_BKG_S stBkg);
HI_VOID VDP_CBM_SetMixerPrio             (VDP_CBM_MIX_E u32mixer_id, HI_U32 u32layer_id,HI_U32 u32prio);
HI_VOID VDP_CBM_ResetMixerPrio           (VDP_CBM_MIX_E u32mixer_id);
//-------------------------------------------------------------------
//MIXER_END
//-------------------------------------------------------------------
HI_VOID VDP_SetCheckSumEnable          (HI_U32  bEnable);
HI_VOID VDP_SetTwoChnEnable            (HI_U32 bEnable);
HI_VOID VDP_SetTwoChnDbgEnable         (HI_U32 bEnable);
HI_VOID VDP_SetSrConnection            (VDP_SR_CONN_E enConnectPosition);


HI_VOID VDP_CBM_SetTestPattern(VDP_BKG_S stVdpBg);

HI_U32 VdpRgb2Yuv709Y(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B);
HI_U32 VdpRgb2Yuv709Cb(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B);
HI_U32 VdpRgb2Yuv709Cr(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B);
#endif
