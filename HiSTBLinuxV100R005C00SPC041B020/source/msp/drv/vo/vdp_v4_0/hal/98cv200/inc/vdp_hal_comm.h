#ifndef __HAL_COMM_H__
#define __HAL_COMM_H__
#include "vdp_define.h"

#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ahb_bus_sys.h"
#else
#include "hi_type.h"
#endif

#ifndef __DISP_PLATFORM_BOOT__

#if VDP_CBB_FPGA
#include "linux/kernel.h"
#endif
#endif
//#include "c_union_define.h"
#include "vdp_reg_define.h"


//HI_U32 VDP_RegRead(HI_U32 a);
//HI_VOID VDP_RegWrite(HI_U32 a, HI_U32 b);

HI_U32 VDP_RegRead( HI_ULONG a);
HI_VOID VDP_RegWrite( HI_ULONG  a, HI_U32 b);

HI_VOID vAxiRegSet(HI_U32 addr, HI_U32 val);

HI_U32 VDP_RegReadEx(HI_U32 a);
HI_VOID VDP_RegWriteEx(HI_U32 a, HI_U32 b);

#if (EDA_TEST & !BT_TEST)

HI_VOID vWrDis2Txt(char * DesFile, char *DesData);
HI_VOID vWrDat2Txt(char * DesFile, HI_U32 DesAddr, HI_U32 DesData);
HI_VOID vWrCoef2Txt(char * DesFile, HI_U32 DesAddr, HI_U32 DesData);
HI_VOID vWrDat2Bin(FILE* fpCfgOut, HI_U64  DesData);

//HI_U32 VDP_RegRead(HI_U32 * a);
//HI_VOID VDP_RegWrite(HI_U32 * a, HI_U32 b);

HI_U32 VDP_RegRead( HI_ULONG a);
HI_VOID VDP_RegWrite( HI_ULONG  a, HI_U32 b);

HI_U32  AhbRead                (HI_U32 a);
HI_VOID AhbWrite               (HI_U32 a, HI_U32 b);

#endif

//-------------------------------------------------------------------
//Bus Function BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_SetVDPMemLowPower (HI_VOID);
HI_VOID VDP_SetClkGateEn       ( HI_U32 u32Data);
HI_VOID VDP_SetWrOutStd        ( HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetRdOutStd        ( HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd);
HI_VOID VDP_SetArbMode         ( HI_U32 u32Data, HI_U32 u32bMode);
HI_VOID VDP_SetRdMultiIdEnable ( HI_U32 u32Data, HI_U32 u32Enable);
HI_VOID VDP_SetParaMasterSel   ( HI_U32 u32MasterNum);

HI_VOID VDP_SetM0AndM1Sel (HI_U32 u32Data, HI_U32 u32MasterNum);
HI_VOID VDP_SetM2AndM3Sel (HI_U32 u32Data, HI_U32 u32MasterNum);
HI_VOID VDP_SetRdBusId    (HI_U32 u32bMode);

HI_VOID VDP_SetAxiMidEnable    (HI_U32 u32Enable);
HI_VOID VDP_SetAxiEdgeMode     (VDP_AXI_EDGE_MODE_E EdgeMode);
HI_VOID VDP_VID_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl);
HI_VOID VDP_GFX_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl);


//-------------------------------------------------------------------
//Bus Function END
//-------------------------------------------------------------------

//HI_VOID VDP_HDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
//HI_VOID VDP_HDATE_SetVdacDetStart  ( HI_U32 u32Data) ;
////SDATE                                              ;
HI_VOID VDP_SDATE_SetVdacDetEnable ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetLine   ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetHigh   ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetWidth  ( HI_U32 u32Data) ;
//HI_VOID VDP_SDATE_SetVdacDetStart  ( HI_U32 u32Data) ;


//-------------------------------------------------------------------
//MIXER_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_CBM_SetMixerBkg              (VDP_CBM_MIX_E u32mixer_id, VDP_BKG_S stBkg);
HI_VOID VDP_CBM_Clear_MixvPrio           (HI_U32 u32layer_id);
HI_VOID VDP_CBM_SetMixerPrio             (VDP_CBM_MIX_E u32mixer_id, HI_U32 u32layer_id,HI_U32 u32prio);
HI_VOID VDP_CBM_ResetMixerPrio           (VDP_CBM_MIX_E u32mixer_id);
//-------------------------------------------------------------------
//MIXER_END
//-------------------------------------------------------------------


HI_VOID VDP_SetCheckSumEnable          (HI_U32 bEnable);
HI_VOID VDP_SetTwoChnEnable            (HI_U32 bEnable);
HI_VOID VDP_SetTwoChnDbgEnable         (HI_U32 bEnable);
HI_VOID VDP_SetTwoChnMode              (HI_U32 u32Data);

HI_VOID VDP_SetSrConnection            (HI_U32 u32Data);
HI_BOOL SDATE_SetSysMode(HI_U32 u32Data);

#endif//__HAL_COMM_H__
