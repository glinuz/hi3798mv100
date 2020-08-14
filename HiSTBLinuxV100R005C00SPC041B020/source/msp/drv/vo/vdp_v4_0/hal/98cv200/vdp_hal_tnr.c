//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_driver.cpp
// Author      : s173141it002-3002-01
// Data        : 2012/10/17
// Version     : v1.0
// Abstract    : header of vdp define
//                                                                         
// Modification history                                                    
//----------------------------------------------------------------------  
// Version       Data(yyyy/mm/dd)      name                                  
// Description                                                              
//                                                                          
//
//
//
//
//**********************************************************************


#include "vdp_hal_comm.h"
#include "vdp_hal_tnr.h"

extern S_VDP_REGS_TYPE *pVdpReg;


//TNR CFG START

///ref data_width
//
HI_VOID VDP_VID_SetTnrRefDataWidth(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrRefDataWidth() select wrong video layer ID\n");
        return ;
    }
    V0_VPSSIP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_VPSSIP_CTRL.bits.data_width = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTnrRefReqCtrl(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrRefReqCtrl() select wrong video layer ID\n");
        return ;
    }
    V0_VPSSIP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_VPSSIP_CTRL.bits.ref_req_ctrl = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return;
}
//mad
HI_VOID VDP_VID_SetTnrMadDbmReqCtrl(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrMadDbmReqCtrl() select wrong video layer ID\n");
        return ;
    }
    V0_VPSSIP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_VPSSIP_CTRL.bits.mad_dbm_req_ctrl = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTnrMadDataMode(HI_U32 u32Data, HI_U32 mad_data_mode)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrMadDataMode() select wrong video layer ID\n");
        return ;
    }
    V0_VPSSIP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_VPSSIP_CTRL.bits.mad_data_mode = mad_data_mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return;
}


//////////////////////

HI_VOID VDP_VID_SetTnrEnable(HI_U32 u32Data,HI_U32 tnr_en)
{
    U_V0_PRERD V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmLpfMode() select wrong video layer ID\n");
      return ;
    }
    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.tnr_en = tnr_en  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);
    return;
}
HI_VOID VDP_VID_SetTNRPgbmEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_PRERD V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetPgbmEnable() select wrong video layer ID\n");
      return ;
    }
    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.tnr_pgbm_en = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmLpfMode(HI_U32 u32Data,HI_U32 tm_lpf_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmLpfMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.tm_lpf_mode = tm_lpf_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCLpfMode(HI_U32 u32Data,HI_U32 te_ylpf_mode, HI_U32 te_clpf_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCLpfMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.te_ylpf_mode = te_ylpf_mode ;
    V0_TNR_MODE.bits.te_clpf_mode = te_clpf_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYCMtMeanMode(HI_U32 u32Data,HI_U32 tm_ymt_mean_mode, HI_U32 tm_cmt_mean_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYCMtMeanMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.tm_ymt_mean_mode = tm_ymt_mean_mode ;
    V0_TNR_MODE.bits.tm_cmt_mean_mode = tm_cmt_mean_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYCMtMode(HI_U32 u32Data,HI_U32 tm_ymt_mode, HI_U32 tm_cmt_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYCMtMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.tm_ymt_mode = tm_ymt_mode ;
    V0_TNR_MODE.bits.tm_cmt_mode = tm_cmt_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanMode(HI_U32 u32Data,HI_U32 te_yeg_mean_mode, HI_U32 te_ceg_mean_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.te_yeg_mean_mode = te_yeg_mean_mode ;
    V0_TNR_MODE.bits.te_ceg_mean_mode = te_ceg_mean_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMode(HI_U32 u32Data,HI_U32 te_yceg_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.te_yceg_mode = te_yceg_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmLpfEnable(HI_U32 u32Data,HI_U32 tfm_lpf_en)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmLpfEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.tfm_lpf_en = tfm_lpf_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNREdgeYCMovingLpfMode(HI_U32 u32Data,HI_U32 edge_ymoving_lpfmode, HI_U32 edge_cmoving_lpfmode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNREdgeYCMovingLpfMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.edge_ymoving_lpfmode = edge_ymoving_lpfmode ;
    V0_TNR_MODE.bits.edge_cmoving_lpfmode = edge_cmoving_lpfmode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRHistMotionEnable(HI_U32 u32Data,HI_U32 hist_motion_en)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRHistMotionEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.hist_motion_en = hist_motion_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRYCBypass(HI_U32 u32Data,HI_U32 tnr_y_bypass, HI_U32 tnr_c_bypass)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRYCBypass() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.tnr_y_bypass = tnr_y_bypass ;
    V0_TNR_MODE.bits.tnr_c_bypass = tnr_c_bypass ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRMarketMode(HI_U32 u32Data,HI_U32 market_mode)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRMarketMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.market_mode = market_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRMarketEnable(HI_U32 u32Data,HI_U32 market_en)
{
    U_V0_TNR_MODE V0_TNR_MODE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRMarketEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET));
    V0_TNR_MODE.bits.market_en = market_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MODE.u32) + u32Data * VID_OFFSET), V0_TNR_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMeanRatioEnable(HI_U32 u32Data,HI_U32 tm_ymt_meanratio_en)
{
    U_V0_TNR_YMOTION_MAP_CTRL V0_TNR_YMOTION_MAP_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMeanRatioEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MAP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_meanratio_en = tm_ymt_meanratio_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMapping(HI_U32 u32Data,HI_U32 tm_ymt_mapping_min, HI_U32 tm_ymt_mapping_max)
{
    U_V0_TNR_YMOTION_MAP_CTRL V0_TNR_YMOTION_MAP_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MAP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min = tm_ymt_mapping_min ;
    V0_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max = tm_ymt_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMappingR(HI_U32 u32Data,HI_U32 tm_ymt_mapping_r0, HI_U32 tm_ymt_mapping_r1, HI_U32 tm_ymt_mapping_r2, HI_U32 tm_ymt_mapping_r3, HI_U32 tm_ymt_mapping_r4)
{
    U_V0_TNR_YMOTION_MAP_R0 V0_TNR_YMOTION_MAP_R0;
    U_V0_TNR_YMOTION_MAP_R1 V0_TNR_YMOTION_MAP_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MAP_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0 = tm_ymt_mapping_r0 ;
    V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1 = tm_ymt_mapping_r1 ;
    V0_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2 = tm_ymt_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_R0.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_R0.u32);

    V0_TNR_YMOTION_MAP_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3 = tm_ymt_mapping_r3 ;
    V0_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4 = tm_ymt_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_R1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMappingT(HI_U32 u32Data,HI_U32 tm_ymt_mapping_t1, HI_U32 tm_ymt_mapping_t2, HI_U32 tm_ymt_mapping_t3, HI_U32 tm_ymt_mapping_t4, HI_U32 tm_ymt_mapping_t5, HI_U32 tm_ymt_mapping_t6)
{
    U_V0_TNR_YMOTION_MAP_T1 V0_TNR_YMOTION_MAP_T1;
    U_V0_TNR_YMOTION_MAP_T2 V0_TNR_YMOTION_MAP_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MAP_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1 = tm_ymt_mapping_t1 ;
    V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2 = tm_ymt_mapping_t2 ;
    V0_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3 = tm_ymt_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_T1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_T1.u32);

    V0_TNR_YMOTION_MAP_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4 = tm_ymt_mapping_t4 ;
    V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5 = tm_ymt_mapping_t5 ;
    V0_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6 = tm_ymt_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_T2.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMappingK(HI_U32 u32Data,HI_S32 tm_ymt_mapping_k1, HI_S32 tm_ymt_mapping_k2, HI_S32 tm_ymt_mapping_k3, HI_S32 tm_ymt_mapping_k4)
{
    U_V0_TNR_YMOTION_MAP_K1 V0_TNR_YMOTION_MAP_K1;
    U_V0_TNR_YMOTION_MAP_K2 V0_TNR_YMOTION_MAP_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MAP_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1 = tm_ymt_mapping_k1 ;
    V0_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2 = tm_ymt_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_K1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_K1.u32);

    V0_TNR_YMOTION_MAP_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3 = tm_ymt_mapping_k3 ;
    V0_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4 = tm_ymt_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MAP_K2.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MAP_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMeanRatioEnable(HI_U32 u32Data,HI_U32 tm_cmt_meanratio_en)
{
    U_V0_TNR_CMOTION_MAP_CTRL V0_TNR_CMOTION_MAP_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMeanRatioEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MAP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_meanratio_en = tm_cmt_meanratio_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMapping(HI_U32 u32Data,HI_U32 tm_cmt_mapping_min, HI_U32 tm_cmt_mapping_max)
{
    U_V0_TNR_CMOTION_MAP_CTRL V0_TNR_CMOTION_MAP_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MAP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min = tm_cmt_mapping_min ;
    V0_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max = tm_cmt_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMappingR(HI_U32 u32Data,HI_U32 tm_cmt_mapping_r0, HI_U32 tm_cmt_mapping_r1, HI_U32 tm_cmt_mapping_r2, HI_U32 tm_cmt_mapping_r3, HI_U32 tm_cmt_mapping_r4)
{
    U_V0_TNR_CMOTION_MAP_R0 V0_TNR_CMOTION_MAP_R0;
    U_V0_TNR_CMOTION_MAP_R1 V0_TNR_CMOTION_MAP_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MAP_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0 = tm_cmt_mapping_r0 ;
    V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1 = tm_cmt_mapping_r1 ;
    V0_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2 = tm_cmt_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_R0.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_R0.u32);

    V0_TNR_CMOTION_MAP_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3 = tm_cmt_mapping_r3 ;
    V0_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4 = tm_cmt_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_R1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMappingT(HI_U32 u32Data,HI_U32 tm_cmt_mapping_t1, HI_U32 tm_cmt_mapping_t2, HI_U32 tm_cmt_mapping_t3, HI_U32 tm_cmt_mapping_t4, HI_U32 tm_cmt_mapping_t5, HI_U32 tm_cmt_mapping_t6)
{
    U_V0_TNR_CMOTION_MAP_T1 V0_TNR_CMOTION_MAP_T1;
    U_V0_TNR_CMOTION_MAP_T2 V0_TNR_CMOTION_MAP_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MAP_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1 = tm_cmt_mapping_t1 ;
    V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2 = tm_cmt_mapping_t2 ;
    V0_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3 = tm_cmt_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_T1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_T1.u32);

    V0_TNR_CMOTION_MAP_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4 = tm_cmt_mapping_t4 ;
    V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5 = tm_cmt_mapping_t5 ;
    V0_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6 = tm_cmt_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_T2.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMappingK(HI_U32 u32Data,HI_S32 tm_cmt_mapping_k1, HI_S32 tm_cmt_mapping_k2, HI_S32 tm_cmt_mapping_k3, HI_S32 tm_cmt_mapping_k4)
{
    U_V0_TNR_CMOTION_MAP_K1 V0_TNR_CMOTION_MAP_K1;
    U_V0_TNR_CMOTION_MAP_K2 V0_TNR_CMOTION_MAP_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MAP_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1 = tm_cmt_mapping_k1 ;
    V0_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2 = tm_cmt_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_K1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_K1.u32);

    V0_TNR_CMOTION_MAP_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3 = tm_cmt_mapping_k3 ;
    V0_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4 = tm_cmt_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MAP_K2.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MAP_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMeanRatio(HI_U32 u32Data,HI_U32 tm_ymt_meanratio_min, HI_U32 tm_ymt_meanratio_max)
{
    U_V0_TNR_YMOTION_MEANRATIO_CTRL V0_TNR_YMOTION_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMeanRatio() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min = tm_ymt_meanratio_min ;
    V0_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max = tm_ymt_meanratio_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMeanRatioR(HI_U32 u32Data,HI_U32 tm_ymt_meanratio_r0, HI_U32 tm_ymt_meanratio_r1, HI_U32 tm_ymt_meanratio_r2, HI_U32 tm_ymt_meanratio_r3, HI_U32 tm_ymt_meanratio_r4)
{
    U_V0_TNR_YMOTION_MEANRATIO_R0 V0_TNR_YMOTION_MEANRATIO_R0;
    U_V0_TNR_YMOTION_MEANRATIO_R1 V0_TNR_YMOTION_MEANRATIO_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMeanRatioR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MEANRATIO_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0 = tm_ymt_meanratio_r0 ;
    V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1 = tm_ymt_meanratio_r1 ;
    V0_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2 = tm_ymt_meanratio_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_R0.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_R0.u32);

    V0_TNR_YMOTION_MEANRATIO_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3 = tm_ymt_meanratio_r3 ;
    V0_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4 = tm_ymt_meanratio_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_R1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMeanRatioT(HI_U32 u32Data,HI_U32 tm_ymt_meanratio_t1, HI_U32 tm_ymt_meanratio_t2, HI_U32 tm_ymt_meanratio_t3, HI_U32 tm_ymt_meanratio_t4, HI_U32 tm_ymt_meanratio_t5, HI_U32 tm_ymt_meanratio_t6)
{
    U_V0_TNR_YMOTION_MEANRATIO_T1 V0_TNR_YMOTION_MEANRATIO_T1;
    U_V0_TNR_YMOTION_MEANRATIO_T2 V0_TNR_YMOTION_MEANRATIO_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMeanRatioT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MEANRATIO_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1 = tm_ymt_meanratio_t1 ;
    V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2 = tm_ymt_meanratio_t2 ;
    V0_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3 = tm_ymt_meanratio_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_T1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_T1.u32);

    V0_TNR_YMOTION_MEANRATIO_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4 = tm_ymt_meanratio_t4 ;
    V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5 = tm_ymt_meanratio_t5 ;
    V0_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6 = tm_ymt_meanratio_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_T2.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMeanRatioK(HI_U32 u32Data,HI_S32 tm_ymt_meanratio_k1, HI_S32 tm_ymt_meanratio_k2, HI_S32 tm_ymt_meanratio_k3, HI_S32 tm_ymt_meanratio_k4)
{
    U_V0_TNR_YMOTION_MEANRATIO_K1 V0_TNR_YMOTION_MEANRATIO_K1;
    U_V0_TNR_YMOTION_MEANRATIO_K2 V0_TNR_YMOTION_MEANRATIO_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMeanRatioK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_YMOTION_MEANRATIO_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1 = tm_ymt_meanratio_k1 ;
    V0_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2 = tm_ymt_meanratio_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_K1.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_K1.u32);

    V0_TNR_YMOTION_MEANRATIO_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3 = tm_ymt_meanratio_k3 ;
    V0_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4 = tm_ymt_meanratio_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_YMOTION_MEANRATIO_K2.u32) + u32Data * VID_OFFSET), V0_TNR_YMOTION_MEANRATIO_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMeanRatio(HI_U32 u32Data,HI_U32 tm_cmt_meanratio_min, HI_U32 tm_cmt_meanratio_max)
{
    U_V0_TNR_CMOTION_MEANRATIO_CTRL V0_TNR_CMOTION_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMeanRatio() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min = tm_cmt_meanratio_min ;
    V0_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max = tm_cmt_meanratio_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMeanRatioR(HI_U32 u32Data,HI_U32 tm_cmt_meanratio_r0, HI_U32 tm_cmt_meanratio_r1, HI_U32 tm_cmt_meanratio_r2, HI_U32 tm_cmt_meanratio_r3, HI_U32 tm_cmt_meanratio_r4)
{
    U_V0_TNR_CMOTION_MEANRATIO_R0 V0_TNR_CMOTION_MEANRATIO_R0;
    U_V0_TNR_CMOTION_MEANRATIO_R1 V0_TNR_CMOTION_MEANRATIO_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMeanRatioR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MEANRATIO_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0 = tm_cmt_meanratio_r0 ;
    V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1 = tm_cmt_meanratio_r1 ;
    V0_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2 = tm_cmt_meanratio_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_R0.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_R0.u32);

    V0_TNR_CMOTION_MEANRATIO_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3 = tm_cmt_meanratio_r3 ;
    V0_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4 = tm_cmt_meanratio_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_R1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMeanRatioT(HI_U32 u32Data,HI_U32 tm_cmt_meanratio_t1, HI_U32 tm_cmt_meanratio_t2, HI_U32 tm_cmt_meanratio_t3, HI_U32 tm_cmt_meanratio_t4, HI_U32 tm_cmt_meanratio_t5, HI_U32 tm_cmt_meanratio_t6)
{
    U_V0_TNR_CMOTION_MEANRATIO_T1 V0_TNR_CMOTION_MEANRATIO_T1;
    U_V0_TNR_CMOTION_MEANRATIO_T2 V0_TNR_CMOTION_MEANRATIO_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMeanRatioT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MEANRATIO_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1 = tm_cmt_meanratio_t1 ;
    V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2 = tm_cmt_meanratio_t2 ;
    V0_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3 = tm_cmt_meanratio_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_T1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_T1.u32);

    V0_TNR_CMOTION_MEANRATIO_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4 = tm_cmt_meanratio_t4 ;
    V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5 = tm_cmt_meanratio_t5 ;
    V0_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6 = tm_cmt_meanratio_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_T2.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMeanRatioK(HI_U32 u32Data,HI_S32 tm_cmt_meanratio_k1, HI_S32 tm_cmt_meanratio_k2, HI_S32 tm_cmt_meanratio_k3, HI_S32 tm_cmt_meanratio_k4)
{
    U_V0_TNR_CMOTION_MEANRATIO_K1 V0_TNR_CMOTION_MEANRATIO_K1;
    U_V0_TNR_CMOTION_MEANRATIO_K2 V0_TNR_CMOTION_MEANRATIO_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMeanRatioK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_CMOTION_MEANRATIO_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1 = tm_cmt_meanratio_k1 ;
    V0_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2 = tm_cmt_meanratio_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_K1.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_K1.u32);

    V0_TNR_CMOTION_MEANRATIO_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3 = tm_cmt_meanratio_k3 ;
    V0_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4 = tm_cmt_meanratio_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CMOTION_MEANRATIO_K2.u32) + u32Data * VID_OFFSET), V0_TNR_CMOTION_MEANRATIO_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYCMtCoef(HI_U32 u32Data,HI_U32 tm_ycmtcoef0, HI_U32 tm_ycmtcoef1, HI_U32 tm_ycmtcoef2, HI_U32 tm_ycmtcoef3, HI_U32 tm_ycmtcoef4)
{
    U_V0_TNR_TM_YCMTCOEF V0_TNR_TM_YCMTCOEF;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYCMtCoef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_YCMTCOEF.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YCMTCOEF.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_YCMTCOEF.bits.tm_ycmtcoef0 = tm_ycmtcoef0 ;
    V0_TNR_TM_YCMTCOEF.bits.tm_ycmtcoef1 = tm_ycmtcoef1 ;
    V0_TNR_TM_YCMTCOEF.bits.tm_ycmtcoef2 = tm_ycmtcoef2 ;
    V0_TNR_TM_YCMTCOEF.bits.tm_ycmtcoef3 = tm_ycmtcoef3 ;
    V0_TNR_TM_YCMTCOEF.bits.tm_ycmtcoef4 = tm_ycmtcoef4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YCMTCOEF.u32) + u32Data * VID_OFFSET), V0_TNR_TM_YCMTCOEF.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYCLpfCoef(HI_U32 u32Data,HI_U32 tm_yclpfcoef0, HI_U32 tm_yclpfcoef1, HI_U32 tm_yclpfcoef2, HI_U32 tm_yclpfcoef3, HI_U32 tm_yclpfcoef4)
{
    U_V0_TNR_TM_YCLPFCOEF V0_TNR_TM_YCLPFCOEF;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYCLpfCoef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_YCLPFCOEF.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YCLPFCOEF.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_YCLPFCOEF.bits.tm_yclpfcoef0 = tm_yclpfcoef0 ;
    V0_TNR_TM_YCLPFCOEF.bits.tm_yclpfcoef1 = tm_yclpfcoef1 ;
    V0_TNR_TM_YCLPFCOEF.bits.tm_yclpfcoef2 = tm_yclpfcoef2 ;
    V0_TNR_TM_YCLPFCOEF.bits.tm_yclpfcoef3 = tm_yclpfcoef3 ;
    V0_TNR_TM_YCLPFCOEF.bits.tm_yclpfcoef4 = tm_yclpfcoef4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YCLPFCOEF.u32) + u32Data * VID_OFFSET), V0_TNR_TM_YCLPFCOEF.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYHpf1Coef(HI_U32 u32Data,HI_S32 te_yhpf1coef0, HI_S32 te_yhpf1coef1, HI_S32 te_yhpf1coef2, HI_S32 te_yhpf1coef3, HI_S32 te_yhpf1coef4, HI_S32 te_yhpf1coef5, HI_S32 te_yhpf1coef6, HI_S32 te_yhpf1coef7, HI_S32 te_yhpf1coef8, HI_U32 te_yhpf1lshiift)
{
    U_V0_TNR_TE_YHPF1_0 V0_TNR_TE_YHPF1_0;
    U_V0_TNR_TE_YHPF1_1 V0_TNR_TE_YHPF1_1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYHpf1Coef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YHPF1_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF1_0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef0 = te_yhpf1coef0 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef1 = te_yhpf1coef1 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef2 = te_yhpf1coef2 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef3 = te_yhpf1coef3 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef4 = te_yhpf1coef4 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef5 = te_yhpf1coef5 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef6 = te_yhpf1coef6 ;
    V0_TNR_TE_YHPF1_0.bits.te_yhpf1coef7 = te_yhpf1coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF1_0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF1_0.u32);

    V0_TNR_TE_YHPF1_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF1_1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF1_1.bits.te_yhpf1coef8   = te_yhpf1coef8 ;
    V0_TNR_TE_YHPF1_1.bits.te_yhpf1lshiift = te_yhpf1lshiift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF1_1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF1_1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYHpf2Coef(HI_U32 u32Data,HI_S32 te_yhpf2coef0, HI_S32 te_yhpf2coef1, HI_S32 te_yhpf2coef2, HI_S32 te_yhpf2coef3, HI_S32 te_yhpf2coef4, HI_S32 te_yhpf2coef5, HI_S32 te_yhpf2coef6, HI_S32 te_yhpf2coef7, HI_S32 te_yhpf2coef8, HI_U32 te_yhpf2lshiift)
{
    U_V0_TNR_TE_YHPF2_0 V0_TNR_TE_YHPF2_0;
    U_V0_TNR_TE_YHPF2_1 V0_TNR_TE_YHPF2_1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYHpf2Coef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YHPF2_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF2_0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef0 = te_yhpf2coef0 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef1 = te_yhpf2coef1 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef2 = te_yhpf2coef2 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef3 = te_yhpf2coef3 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef4 = te_yhpf2coef4 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef5 = te_yhpf2coef5 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef6 = te_yhpf2coef6 ;
    V0_TNR_TE_YHPF2_0.bits.te_yhpf2coef7 = te_yhpf2coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF2_0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF2_0.u32);

    V0_TNR_TE_YHPF2_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF2_1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF2_1.bits.te_yhpf2coef8   = te_yhpf2coef8 ;
    V0_TNR_TE_YHPF2_1.bits.te_yhpf2lshiift = te_yhpf2lshiift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF2_1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF2_1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYHpf3Coef(HI_U32 u32Data,HI_S32 te_yhpf3coef0, HI_S32 te_yhpf3coef1, HI_S32 te_yhpf3coef2, HI_S32 te_yhpf3coef3, HI_S32 te_yhpf3coef4, HI_S32 te_yhpf3coef5, HI_S32 te_yhpf3coef6, HI_S32 te_yhpf3coef7, HI_S32 te_yhpf3coef8, HI_U32 te_yhpf3lshiift)
{
    U_V0_TNR_TE_YHPF3_0 V0_TNR_TE_YHPF3_0;
    U_V0_TNR_TE_YHPF3_1 V0_TNR_TE_YHPF3_1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYHpf3Coef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YHPF3_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF3_0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef0 = te_yhpf3coef0 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef1 = te_yhpf3coef1 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef2 = te_yhpf3coef2 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef3 = te_yhpf3coef3 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef4 = te_yhpf3coef4 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef5 = te_yhpf3coef5 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef6 = te_yhpf3coef6 ;
    V0_TNR_TE_YHPF3_0.bits.te_yhpf3coef7 = te_yhpf3coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF3_0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF3_0.u32);

    V0_TNR_TE_YHPF3_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF3_1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF3_1.bits.te_yhpf3coef8   = te_yhpf3coef8 ;
    V0_TNR_TE_YHPF3_1.bits.te_yhpf3lshiift = te_yhpf3lshiift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF3_1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF3_1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYHpf4Coef(HI_U32 u32Data,HI_S32 te_yhpf4coef0, HI_S32 te_yhpf4coef1, HI_S32 te_yhpf4coef2, HI_S32 te_yhpf4coef3, HI_S32 te_yhpf4coef4, HI_S32 te_yhpf4coef5, HI_S32 te_yhpf4coef6, HI_S32 te_yhpf4coef7, HI_S32 te_yhpf4coef8, HI_U32 te_yhpf4lshiift)
{
    U_V0_TNR_TE_YHPF4_0 V0_TNR_TE_YHPF4_0;
    U_V0_TNR_TE_YHPF4_1 V0_TNR_TE_YHPF4_1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYHpf4Coef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YHPF4_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF4_0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef0 = te_yhpf4coef0 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef1 = te_yhpf4coef1 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef2 = te_yhpf4coef2 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef3 = te_yhpf4coef3 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef4 = te_yhpf4coef4 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef5 = te_yhpf4coef5 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef6 = te_yhpf4coef6 ;
    V0_TNR_TE_YHPF4_0.bits.te_yhpf4coef7 = te_yhpf4coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF4_0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF4_0.u32);

    V0_TNR_TE_YHPF4_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF4_1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YHPF4_1.bits.te_yhpf4coef8   = te_yhpf4coef8 ;
    V0_TNR_TE_YHPF4_1.bits.te_yhpf4lshiift = te_yhpf4lshiift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YHPF4_1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YHPF4_1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYMtMeanLpfCoef(HI_U32 u32Data,HI_U32 te_ymtmeanlpfcoef0, HI_U32 te_ymtmeanlpfcoef1, HI_U32 te_ymtmeanlpfcoef2, HI_U32 te_ymtmeanlpfcoef3, HI_U32 te_ymtmeanlpfcoef4, HI_U32 te_ymtmeanlpfcoef5, HI_U32 te_ymtmeanlpfcoef6, HI_U32 te_ymtmeanlpfcoef7, HI_U32 te_ymtmeanlpfcoef8)
{
    U_V0_TNR_TE_YMTMEAN_LPFCOEF_0 V0_TNR_TE_YMTMEAN_LPFCOEF_0;
    U_V0_TNR_TE_YMTMEAN_LPFCOEF_1 V0_TNR_TE_YMTMEAN_LPFCOEF_1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanLpfCoef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMTMEAN_LPFCOEF_0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.bits.te_ymtmeanlpfcoef0 = te_ymtmeanlpfcoef0 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.bits.te_ymtmeanlpfcoef1 = te_ymtmeanlpfcoef1 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.bits.te_ymtmeanlpfcoef2 = te_ymtmeanlpfcoef2 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.bits.te_ymtmeanlpfcoef3 = te_ymtmeanlpfcoef3 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_0.bits.te_ymtmeanlpfcoef4 = te_ymtmeanlpfcoef4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMTMEAN_LPFCOEF_0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMTMEAN_LPFCOEF_0.u32);

    V0_TNR_TE_YMTMEAN_LPFCOEF_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMTMEAN_LPFCOEF_1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMTMEAN_LPFCOEF_1.bits.te_ymtmeanlpfcoef5 = te_ymtmeanlpfcoef5 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_1.bits.te_ymtmeanlpfcoef6 = te_ymtmeanlpfcoef6 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_1.bits.te_ymtmeanlpfcoef7 = te_ymtmeanlpfcoef7 ;
    V0_TNR_TE_YMTMEAN_LPFCOEF_1.bits.te_ymtmeanlpfcoef8 = te_ymtmeanlpfcoef8 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMTMEAN_LPFCOEF_1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMTMEAN_LPFCOEF_1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanRatioEnable(HI_U32 u32Data,HI_U32 te_yceg_meanratio_en)
{
    U_V0_TNR_TE_YCEG_MEANRATIO_CTRL V0_TNR_TE_YCEG_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanRatioEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_CTRL.bits.te_yceg_meanratio_en = te_yceg_meanratio_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanRatio(HI_U32 u32Data,HI_U32 te_yceg_meanratio_min, HI_U32 te_yceg_meanratio_max)
{
    U_V0_TNR_TE_YCEG_MEANRATIO_CTRL V0_TNR_TE_YCEG_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanRatio() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_CTRL.bits.te_yceg_meanratio_min = te_yceg_meanratio_min ;
    V0_TNR_TE_YCEG_MEANRATIO_CTRL.bits.te_yceg_meanratio_max = te_yceg_meanratio_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanRatioR(HI_U32 u32Data,HI_U32 te_yceg_meanratio_r0, HI_U32 te_yceg_meanratio_r1, HI_U32 te_yceg_meanratio_r2, HI_U32 te_yceg_meanratio_r3, HI_U32 te_yceg_meanratio_r4)
{
    U_V0_TNR_TE_YCEG_MEANRATIO_R0 V0_TNR_TE_YCEG_MEANRATIO_R0;
    U_V0_TNR_TE_YCEG_MEANRATIO_R1 V0_TNR_TE_YCEG_MEANRATIO_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanRatioR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MEANRATIO_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_R0.bits.te_yceg_meanratio_r0 = te_yceg_meanratio_r0 ;
    V0_TNR_TE_YCEG_MEANRATIO_R0.bits.te_yceg_meanratio_r1 = te_yceg_meanratio_r1 ;
    V0_TNR_TE_YCEG_MEANRATIO_R0.bits.te_yceg_meanratio_r2 = te_yceg_meanratio_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_R0.u32);

    V0_TNR_TE_YCEG_MEANRATIO_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_R1.bits.te_yceg_meanratio_r3 = te_yceg_meanratio_r3 ;
    V0_TNR_TE_YCEG_MEANRATIO_R1.bits.te_yceg_meanratio_r4 = te_yceg_meanratio_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanRatioT(HI_U32 u32Data,HI_U32 te_yceg_meanratio_t1, HI_U32 te_yceg_meanratio_t2, HI_U32 te_yceg_meanratio_t3, HI_U32 te_yceg_meanratio_t4, HI_U32 te_yceg_meanratio_t5, HI_U32 te_yceg_meanratio_t6)
{
    U_V0_TNR_TE_YCEG_MEANRATIO_T1 V0_TNR_TE_YCEG_MEANRATIO_T1;
    U_V0_TNR_TE_YCEG_MEANRATIO_T2 V0_TNR_TE_YCEG_MEANRATIO_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanRatioT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MEANRATIO_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_T1.bits.te_yceg_meanratio_t1 = te_yceg_meanratio_t1 ;
    V0_TNR_TE_YCEG_MEANRATIO_T1.bits.te_yceg_meanratio_t2 = te_yceg_meanratio_t2 ;
    V0_TNR_TE_YCEG_MEANRATIO_T1.bits.te_yceg_meanratio_t3 = te_yceg_meanratio_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_T1.u32);

    V0_TNR_TE_YCEG_MEANRATIO_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_T2.bits.te_yceg_meanratio_t4 = te_yceg_meanratio_t4 ;
    V0_TNR_TE_YCEG_MEANRATIO_T2.bits.te_yceg_meanratio_t5 = te_yceg_meanratio_t5 ;
    V0_TNR_TE_YCEG_MEANRATIO_T2.bits.te_yceg_meanratio_t6 = te_yceg_meanratio_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMeanRatioK(HI_U32 u32Data,HI_S32 te_yceg_meanratio_k1, HI_S32 te_yceg_meanratio_k2, HI_S32 te_yceg_meanratio_k3, HI_S32 te_yceg_meanratio_k4)
{
    U_V0_TNR_TE_YCEG_MEANRATIO_K1 V0_TNR_TE_YCEG_MEANRATIO_K1;
    U_V0_TNR_TE_YCEG_MEANRATIO_K2 V0_TNR_TE_YCEG_MEANRATIO_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMeanRatioK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MEANRATIO_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_K1.bits.te_yceg_meanratio_k1 = te_yceg_meanratio_k1 ;
    V0_TNR_TE_YCEG_MEANRATIO_K1.bits.te_yceg_meanratio_k2 = te_yceg_meanratio_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_K1.u32);

    V0_TNR_TE_YCEG_MEANRATIO_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MEANRATIO_K2.bits.te_yceg_meanratio_k3 = te_yceg_meanratio_k3 ;
    V0_TNR_TE_YCEG_MEANRATIO_K2.bits.te_yceg_meanratio_k4 = te_yceg_meanratio_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MEANRATIO_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MEANRATIO_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtMeanRatioEnable(HI_U32 u32Data,HI_U32 te_ycmt_meanratio_en)
{
    U_V0_TNR_TE_CMT_MEANRATIO_CTRL V0_TNR_TE_CMT_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtMeanRatioEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_CTRL.bits.te_ycmt_meanratio_en = te_ycmt_meanratio_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtMeanRatio(HI_U32 u32Data,HI_U32 te_ycmt_meanratio_min, HI_U32 te_ycmt_meanratio_max)
{
    U_V0_TNR_TE_CMT_MEANRATIO_CTRL V0_TNR_TE_CMT_MEANRATIO_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtMeanRatio() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_CTRL.bits.te_ycmt_meanratio_min = te_ycmt_meanratio_min ;
    V0_TNR_TE_CMT_MEANRATIO_CTRL.bits.te_ycmt_meanratio_max = te_ycmt_meanratio_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtMeanRatioR(HI_U32 u32Data,HI_U32 te_ycmt_meanratio_r0, HI_U32 te_ycmt_meanratio_r1, HI_U32 te_ycmt_meanratio_r2, HI_U32 te_ycmt_meanratio_r3, HI_U32 te_ycmt_meanratio_r4)
{
    U_V0_TNR_TE_CMT_MEANRATIO_R0 V0_TNR_TE_CMT_MEANRATIO_R0;
    U_V0_TNR_TE_CMT_MEANRATIO_R1 V0_TNR_TE_CMT_MEANRATIO_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtMeanRatioR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_MEANRATIO_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_R0.bits.te_ycmt_meanratio_r0 = te_ycmt_meanratio_r0 ;
    V0_TNR_TE_CMT_MEANRATIO_R0.bits.te_ycmt_meanratio_r1 = te_ycmt_meanratio_r1 ;
    V0_TNR_TE_CMT_MEANRATIO_R0.bits.te_ycmt_meanratio_r2 = te_ycmt_meanratio_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_R0.u32);

    V0_TNR_TE_CMT_MEANRATIO_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_R1.bits.te_ycmt_meanratio_r3 = te_ycmt_meanratio_r3 ;
    V0_TNR_TE_CMT_MEANRATIO_R1.bits.te_ycmt_meanratio_r4 = te_ycmt_meanratio_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtMeanRatioT(HI_U32 u32Data,HI_U32 te_ycmt_meanratio_t1, HI_U32 te_ycmt_meanratio_t2, HI_U32 te_ycmt_meanratio_t3, HI_U32 te_ycmt_meanratio_t4, HI_U32 te_ycmt_meanratio_t5, HI_U32 te_ycmt_meanratio_t6)
{
    U_V0_TNR_TE_CMT_MEANRATIO_T1 V0_TNR_TE_CMT_MEANRATIO_T1;
    U_V0_TNR_TE_CMT_MEANRATIO_T2 V0_TNR_TE_CMT_MEANRATIO_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtMeanRatioT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_MEANRATIO_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_T1.bits.te_ycmt_meanratio_t1 = te_ycmt_meanratio_t1 ;
    V0_TNR_TE_CMT_MEANRATIO_T1.bits.te_ycmt_meanratio_t2 = te_ycmt_meanratio_t2 ;
    V0_TNR_TE_CMT_MEANRATIO_T1.bits.te_ycmt_meanratio_t3 = te_ycmt_meanratio_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_T1.u32);

    V0_TNR_TE_CMT_MEANRATIO_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_T2.bits.te_ycmt_meanratio_t4 = te_ycmt_meanratio_t4 ;
    V0_TNR_TE_CMT_MEANRATIO_T2.bits.te_ycmt_meanratio_t5 = te_ycmt_meanratio_t5 ;
    V0_TNR_TE_CMT_MEANRATIO_T2.bits.te_ycmt_meanratio_t6 = te_ycmt_meanratio_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtMeanRatioK(HI_U32 u32Data,HI_S32 te_ycmt_meanratio_k1, HI_S32 te_ycmt_meanratio_k2, HI_S32 te_ycmt_meanratio_k3, HI_S32 te_ycmt_meanratio_k4)
{
    U_V0_TNR_TE_CMT_MEANRATIO_K1 V0_TNR_TE_CMT_MEANRATIO_K1;
    U_V0_TNR_TE_CMT_MEANRATIO_K2 V0_TNR_TE_CMT_MEANRATIO_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtMeanRatioK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_MEANRATIO_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_K1.bits.te_ycmt_meanratio_k1 = te_ycmt_meanratio_k1 ;
    V0_TNR_TE_CMT_MEANRATIO_K1.bits.te_ycmt_meanratio_k2 = te_ycmt_meanratio_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_K1.u32);

    V0_TNR_TE_CMT_MEANRATIO_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_MEANRATIO_K2.bits.te_ycmt_meanratio_k3 = te_ycmt_meanratio_k3 ;
    V0_TNR_TE_CMT_MEANRATIO_K2.bits.te_ycmt_meanratio_k4 = te_ycmt_meanratio_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_MEANRATIO_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_MEANRATIO_K2.u32);
    return;
}

//HI_VOID VDP_VID_SetTNRTeYMtMeanRatioEnable(HI_U32 u32Data,HI_U32 te_ycmt_meanmtratio_en)
//{
//    U_V0_TNR_TE_YMT_MEANRATIO_CTRL V0_TNR_TE_YMT_MEANRATIO_CTRL;
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanRatioEnable() select wrong video layer ID\n");
//      return ;
//    }
//    V0_TNR_TE_YMT_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_CTRL.bits.te_ycmt_meanmtratio_en = te_ycmt_meanmtratio_en ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_CTRL.u32);
//    return;
//}
//
//HI_VOID VDP_VID_SetTNRTeYMtMeanRatio(HI_U32 u32Data,HI_U32 te_ycmt_meanmtratio_min, HI_U32 te_ycmt_meanmtratio_max)
//{
//    U_V0_TNR_TE_YMT_MEANRATIO_CTRL V0_TNR_TE_YMT_MEANRATIO_CTRL;
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanRatio() select wrong video layer ID\n");
//      return ;
//    }
//    V0_TNR_TE_YMT_MEANRATIO_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_CTRL.bits.te_ycmt_meanmtratio_min = te_ycmt_meanmtratio_min ;
//    V0_TNR_TE_YMT_MEANRATIO_CTRL.bits.te_ycmt_meanmtratio_max = te_ycmt_meanmtratio_max ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_CTRL.u32);
//    return;
//}
//
//HI_VOID VDP_VID_SetTNRTeYMtMeanRatioR(HI_U32 u32Data,HI_U32 te_ycmt_meanmtratio_r0, HI_U32 te_ycmt_meanmtratio_r1, HI_U32 te_ycmt_meanmtratio_r2, HI_U32 te_ycmt_meanmtratio_r3, HI_U32 te_ycmt_meanmtratio_r4)
//{
//    U_V0_TNR_TE_YMT_MEANRATIO_R0 V0_TNR_TE_YMT_MEANRATIO_R0;
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanRatioR() select wrong video layer ID\n");
//      return ;
//    }
//    V0_TNR_TE_YMT_MEANRATIO_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_R0.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_R0.bits.te_ycmt_meanmtratio_r0 = te_ycmt_meanmtratio_r0 ;
//    V0_TNR_TE_YMT_MEANRATIO_R0.bits.te_ycmt_meanmtratio_r1 = te_ycmt_meanmtratio_r1 ;
//    V0_TNR_TE_YMT_MEANRATIO_R0.bits.te_ycmt_meanmtratio_r2 = te_ycmt_meanmtratio_r2 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_R0.u32);
//
//    U_V0_TNR_TE_YMT_MEANRATIO_R1 V0_TNR_TE_YMT_MEANRATIO_R1;
//    V0_TNR_TE_YMT_MEANRATIO_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_R1.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_R1.bits.te_ycmt_meanmtratio_r3 = te_ycmt_meanmtratio_r3 ;
//    V0_TNR_TE_YMT_MEANRATIO_R1.bits.te_ycmt_meanmtratio_r4 = te_ycmt_meanmtratio_r4 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_R1.u32);
//    return;
//}
//
//HI_VOID VDP_VID_SetTNRTeYMtMeanRatioT(HI_U32 u32Data,HI_U32 te_ycmt_meanmtratio_t1, HI_U32 te_ycmt_meanmtratio_t2, HI_U32 te_ycmt_meanmtratio_t3, HI_U32 te_ycmt_meanmtratio_t4, HI_U32 te_ycmt_meanmtratio_t5, HI_U32 te_ycmt_meanmtratio_t6)
//{
//    U_V0_TNR_TE_YMT_MEANRATIO_T1 V0_TNR_TE_YMT_MEANRATIO_T1;
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanRatioT() select wrong video layer ID\n");
//      return ;
//    }
//    V0_TNR_TE_YMT_MEANRATIO_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_T1.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_T1.bits.te_ycmt_meanmtratio_t1 = te_ycmt_meanmtratio_t1 ;
//    V0_TNR_TE_YMT_MEANRATIO_T1.bits.te_ycmt_meanmtratio_t2 = te_ycmt_meanmtratio_t2 ;
//    V0_TNR_TE_YMT_MEANRATIO_T1.bits.te_ycmt_meanmtratio_t3 = te_ycmt_meanmtratio_t3 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_T1.u32);
//
//    U_V0_TNR_TE_YMT_MEANRATIO_T2 V0_TNR_TE_YMT_MEANRATIO_T2;
//    V0_TNR_TE_YMT_MEANRATIO_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_T2.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_T2.bits.te_ycmt_meanmtratio_t4 = te_ycmt_meanmtratio_t4 ;
//    V0_TNR_TE_YMT_MEANRATIO_T2.bits.te_ycmt_meanmtratio_t5 = te_ycmt_meanmtratio_t5 ;
//    V0_TNR_TE_YMT_MEANRATIO_T2.bits.te_ycmt_meanmtratio_t6 = te_ycmt_meanmtratio_t6 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_T2.u32);
//    return;
//}
//
//HI_VOID VDP_VID_SetTNRTeYMtMeanRatioK(HI_U32 u32Data,HI_U32 te_ycmt_meanmtratio_k1, HI_U32 te_ycmt_meanmtratio_k2, HI_U32 te_ycmt_meanmtratio_k3, HI_U32 te_ycmt_meanmtratio_k4)
//{
//    U_V0_TNR_TE_YMT_MEANRATIO_K1 V0_TNR_TE_YMT_MEANRATIO_K1;
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtMeanRatioK() select wrong video layer ID\n");
//      return ;
//    }
//    V0_TNR_TE_YMT_MEANRATIO_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_K1.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_K1.bits.te_ycmt_meanmtratio_k1 = te_ycmt_meanmtratio_k1 ;
//    V0_TNR_TE_YMT_MEANRATIO_K1.bits.te_ycmt_meanmtratio_k2 = te_ycmt_meanmtratio_k2 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_K1.u32);
//
//    U_V0_TNR_TE_YMT_MEANRATIO_K2 V0_TNR_TE_YMT_MEANRATIO_K2;
//    V0_TNR_TE_YMT_MEANRATIO_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_K2.u32) + u32Data * VID_OFFSET));
//    V0_TNR_TE_YMT_MEANRATIO_K2.bits.te_ycmt_meanmtratio_k3 = te_ycmt_meanmtratio_k3 ;
//    V0_TNR_TE_YMT_MEANRATIO_K2.bits.te_ycmt_meanmtratio_k4 = te_ycmt_meanmtratio_k4 ;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_MEANRATIO_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_MEANRATIO_K2.u32);
//    return;
//}

HI_VOID VDP_VID_SetTNRTeYCMtMapping(HI_U32 u32Data,HI_U32 te_ycmt_mapping_min, HI_U32 te_ycmt_mapping_max)
{
    U_V0_TNR_TE_YCMT_MAPPING_CTRL V0_TNR_TE_YCMT_MAPPING_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCMtMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCMT_MAPPING_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_CTRL.bits.te_ycmt_mapping_min = te_ycmt_mapping_min ;
    V0_TNR_TE_YCMT_MAPPING_CTRL.bits.te_ycmt_mapping_max = te_ycmt_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCMtMappingR(HI_U32 u32Data,HI_U32 te_ycmt_mapping_r0, HI_U32 te_ycmt_mapping_r1, HI_U32 te_ycmt_mapping_r2, HI_U32 te_ycmt_mapping_r3, HI_U32 te_ycmt_mapping_r4)
{
    U_V0_TNR_TE_YCMT_MAPPING_R0 V0_TNR_TE_YCMT_MAPPING_R0;
    U_V0_TNR_TE_YCMT_MAPPING_R1 V0_TNR_TE_YCMT_MAPPING_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCMtMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCMT_MAPPING_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_R0.bits.te_ycmt_mapping_r0 = te_ycmt_mapping_r0 ;
    V0_TNR_TE_YCMT_MAPPING_R0.bits.te_ycmt_mapping_r1 = te_ycmt_mapping_r1 ;
    V0_TNR_TE_YCMT_MAPPING_R0.bits.te_ycmt_mapping_r2 = te_ycmt_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_R0.u32);

    V0_TNR_TE_YCMT_MAPPING_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_R1.bits.te_ycmt_mapping_r3 = te_ycmt_mapping_r3 ;
    V0_TNR_TE_YCMT_MAPPING_R1.bits.te_ycmt_mapping_r4 = te_ycmt_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCMtMappingT(HI_U32 u32Data,HI_U32 te_ycmt_mapping_t1, HI_U32 te_ycmt_mapping_t2, HI_U32 te_ycmt_mapping_t3, HI_U32 te_ycmt_mapping_t4, HI_U32 te_ycmt_mapping_t5, HI_U32 te_ycmt_mapping_t6)
{
    U_V0_TNR_TE_YCMT_MAPPING_T1 V0_TNR_TE_YCMT_MAPPING_T1;
    U_V0_TNR_TE_YCMT_MAPPING_T2 V0_TNR_TE_YCMT_MAPPING_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCMtMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCMT_MAPPING_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_T1.bits.te_ycmt_mapping_t1 = te_ycmt_mapping_t1 ;
    V0_TNR_TE_YCMT_MAPPING_T1.bits.te_ycmt_mapping_t2 = te_ycmt_mapping_t2 ;
    V0_TNR_TE_YCMT_MAPPING_T1.bits.te_ycmt_mapping_t3 = te_ycmt_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_T1.u32);

    V0_TNR_TE_YCMT_MAPPING_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_T2.bits.te_ycmt_mapping_t4 = te_ycmt_mapping_t4 ;
    V0_TNR_TE_YCMT_MAPPING_T2.bits.te_ycmt_mapping_t5 = te_ycmt_mapping_t5 ;
    V0_TNR_TE_YCMT_MAPPING_T2.bits.te_ycmt_mapping_t6 = te_ycmt_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCMtMappingK(HI_U32 u32Data,HI_S32 te_ycmt_mapping_k1, HI_S32 te_ycmt_mapping_k2, HI_S32 te_ycmt_mapping_k3, HI_S32 te_ycmt_mapping_k4)
{
    U_V0_TNR_TE_YCMT_MAPPING_K1 V0_TNR_TE_YCMT_MAPPING_K1;
    U_V0_TNR_TE_YCMT_MAPPING_K2 V0_TNR_TE_YCMT_MAPPING_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCMtMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCMT_MAPPING_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_K1.bits.te_ycmt_mapping_k1 = te_ycmt_mapping_k1 ;
    V0_TNR_TE_YCMT_MAPPING_K1.bits.te_ycmt_mapping_k2 = te_ycmt_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_K1.u32);

    V0_TNR_TE_YCMT_MAPPING_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCMT_MAPPING_K2.bits.te_ycmt_mapping_k3 = te_ycmt_mapping_k3 ;
    V0_TNR_TE_YCMT_MAPPING_K2.bits.te_ycmt_mapping_k4 = te_ycmt_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCMT_MAPPING_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCMT_MAPPING_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMapping(HI_U32 u32Data,HI_U32 te_yceg_mapping_min, HI_U32 te_yceg_mapping_max)
{
    U_V0_TNR_TE_YCEG_MAPPING_CTRL V0_TNR_TE_YCEG_MAPPING_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MAPPING_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_CTRL.bits.te_yceg_mapping_min = te_yceg_mapping_min ;
    V0_TNR_TE_YCEG_MAPPING_CTRL.bits.te_yceg_mapping_max = te_yceg_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMappingR(HI_U32 u32Data,HI_U32 te_yceg_mapping_r0, HI_U32 te_yceg_mapping_r1, HI_U32 te_yceg_mapping_r2, HI_U32 te_yceg_mapping_r3, HI_U32 te_yceg_mapping_r4)
{
    U_V0_TNR_TE_YCEG_MAPPING_R0 V0_TNR_TE_YCEG_MAPPING_R0;
    U_V0_TNR_TE_YCEG_MAPPING_R1 V0_TNR_TE_YCEG_MAPPING_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MAPPING_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_R0.bits.te_yceg_mapping_r0 = te_yceg_mapping_r0 ;
    V0_TNR_TE_YCEG_MAPPING_R0.bits.te_yceg_mapping_r1 = te_yceg_mapping_r1 ;
    V0_TNR_TE_YCEG_MAPPING_R0.bits.te_yceg_mapping_r2 = te_yceg_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_R0.u32);

    V0_TNR_TE_YCEG_MAPPING_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_R1.bits.te_yceg_mapping_r3 = te_yceg_mapping_r3 ;
    V0_TNR_TE_YCEG_MAPPING_R1.bits.te_yceg_mapping_r4 = te_yceg_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMappingT(HI_U32 u32Data,HI_U32 te_yceg_mapping_t1, HI_U32 te_yceg_mapping_t2, HI_U32 te_yceg_mapping_t3, HI_U32 te_yceg_mapping_t4, HI_U32 te_yceg_mapping_t5, HI_U32 te_yceg_mapping_t6)
{
    U_V0_TNR_TE_YCEG_MAPPING_T1 V0_TNR_TE_YCEG_MAPPING_T1;
    U_V0_TNR_TE_YCEG_MAPPING_T2 V0_TNR_TE_YCEG_MAPPING_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MAPPING_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_T1.bits.te_yceg_mapping_t1 = te_yceg_mapping_t1 ;
    V0_TNR_TE_YCEG_MAPPING_T1.bits.te_yceg_mapping_t2 = te_yceg_mapping_t2 ;
    V0_TNR_TE_YCEG_MAPPING_T1.bits.te_yceg_mapping_t3 = te_yceg_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_T1.u32);

    V0_TNR_TE_YCEG_MAPPING_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_T2.bits.te_yceg_mapping_t4 = te_yceg_mapping_t4 ;
    V0_TNR_TE_YCEG_MAPPING_T2.bits.te_yceg_mapping_t5 = te_yceg_mapping_t5 ;
    V0_TNR_TE_YCEG_MAPPING_T2.bits.te_yceg_mapping_t6 = te_yceg_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYCEgMappingK(HI_U32 u32Data,HI_S32 te_yceg_mapping_k1, HI_S32 te_yceg_mapping_k2, HI_S32 te_yceg_mapping_k3, HI_S32 te_yceg_mapping_k4)
{
    U_V0_TNR_TE_YCEG_MAPPING_K1 V0_TNR_TE_YCEG_MAPPING_K1;
    U_V0_TNR_TE_YCEG_MAPPING_K2 V0_TNR_TE_YCEG_MAPPING_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYCEgMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YCEG_MAPPING_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_K1.bits.te_yceg_mapping_k1 = te_yceg_mapping_k1 ;
    V0_TNR_TE_YCEG_MAPPING_K1.bits.te_yceg_mapping_k2 = te_yceg_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_K1.u32);

    V0_TNR_TE_YCEG_MAPPING_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YCEG_MAPPING_K2.bits.te_yceg_mapping_k3 = te_yceg_mapping_k3 ;
    V0_TNR_TE_YCEG_MAPPING_K2.bits.te_yceg_mapping_k4 = te_yceg_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YCEG_MAPPING_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YCEG_MAPPING_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCEgCK(HI_U32 u32Data,HI_S32 te_ceg_core, HI_U32 te_ceg_k, HI_U32 te_crshift)
{
    U_V0_TNR_TE_CEG_CK V0_TNR_TE_CEG_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCEgCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CEG_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CEG_CK.bits.te_ceg_core = te_ceg_core ;
    V0_TNR_TE_CEG_CK.bits.te_ceg_k    = te_ceg_k    ;
    V0_TNR_TE_CEG_CK.bits.te_crshift  = te_crshift  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CEG_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCEgCK2(HI_U32 u32Data,HI_S32 te_ceg_core2, HI_U32 te_ceg_k2)
{
    U_V0_TNR_TE_CEG_CK2 V0_TNR_TE_CEG_CK2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCEgCK2() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CEG_CK2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_CK2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CEG_CK2.bits.te_ceg_core2 = te_ceg_core2 ;
    V0_TNR_TE_CEG_CK2.bits.te_ceg_k2    = te_ceg_k2    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_CK2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CEG_CK2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtCK1(HI_U32 u32Data,HI_S32 te_cmt_core1, HI_U32 te_cmt_k1)
{
    U_V0_TNR_TE_CMT_CK1 V0_TNR_TE_CMT_CK1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtCK1() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_CK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_CK1.bits.te_cmt_core1 = te_cmt_core1 ;
    V0_TNR_TE_CMT_CK1.bits.te_cmt_k1    = te_cmt_k1    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_CK1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtCK2(HI_U32 u32Data,HI_S32 te_cmt_core2, HI_U32 te_cmt_k2)
{
    U_V0_TNR_TE_CMT_CK2 V0_TNR_TE_CMT_CK2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtCK2() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_CK2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_CK2.bits.te_cmt_core2 = te_cmt_core2 ;
    V0_TNR_TE_CMT_CK2.bits.te_cmt_k2    = te_cmt_k2    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_CK2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMtCK3(HI_U32 u32Data,HI_S32 te_cmt_core3, HI_U32 te_cmt_k3)
{
    U_V0_TNR_TE_CMT_CK3 V0_TNR_TE_CMT_CK3;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMtCK3() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMT_CK3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK3.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMT_CK3.bits.te_cmt_core3 = te_cmt_core3 ;
    V0_TNR_TE_CMT_CK3.bits.te_cmt_k3    = te_cmt_k3    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMT_CK3.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMT_CK3.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCHpf4Coef(HI_U32 u32Data,HI_S32 te_chpf4coef0, HI_S32 te_chpf4coef1, HI_S32 te_chpf4coef2, HI_S32 te_chpf4coef3, HI_S32 te_chpf4coef4, HI_S32 te_chpf4coef5, HI_S32 te_chpf4coef6, HI_S32 te_chpf4coef7, HI_S32 te_chpf4coef8, HI_U32 te_chpf4lshiift)
{
    U_V0_TNR_TE_CHPF4_COEF0 V0_TNR_TE_CHPF4_COEF0;
    U_V0_TNR_TE_CHPF4_COEF1 V0_TNR_TE_CHPF4_COEF1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCHpf4Coef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CHPF4_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CHPF4_COEF0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef0 = te_chpf4coef0 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef1 = te_chpf4coef1 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef2 = te_chpf4coef2 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef3 = te_chpf4coef3 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef4 = te_chpf4coef4 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef5 = te_chpf4coef5 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef6 = te_chpf4coef6 ;
    V0_TNR_TE_CHPF4_COEF0.bits.te_chpf4coef7 = te_chpf4coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CHPF4_COEF0.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CHPF4_COEF0.u32);

    V0_TNR_TE_CHPF4_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CHPF4_COEF1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CHPF4_COEF1.bits.te_chpf4coef8   = te_chpf4coef8 ;
    V0_TNR_TE_CHPF4_COEF1.bits.te_chpf4lshiift = te_chpf4lshiift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CHPF4_COEF1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CHPF4_COEF1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYEgWeight(HI_U32 u32Data,HI_U32 te_yeg_weight1, HI_U32 te_yeg_weight2, HI_U32 te_yeg_weight3, HI_U32 te_yeg_weight4)
{
    U_V0_TNR_TE_YEG_WEIGHT V0_TNR_TE_YEG_WEIGHT;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYEgWeight() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YEG_WEIGHT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_WEIGHT.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YEG_WEIGHT.bits.te_yeg_weight1 = te_yeg_weight1 ;
    V0_TNR_TE_YEG_WEIGHT.bits.te_yeg_weight2 = te_yeg_weight2 ;
    V0_TNR_TE_YEG_WEIGHT.bits.te_yeg_weight3 = te_yeg_weight3 ;
    V0_TNR_TE_YEG_WEIGHT.bits.te_yeg_weight4 = te_yeg_weight4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_WEIGHT.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YEG_WEIGHT.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCEgWeight(HI_U32 u32Data,HI_U32 te_ceg_weight1, HI_U32 te_ceg_weight2, HI_U32 te_ceg_weight3, HI_U32 te_ceg_weight4)
{
    U_V0_TNR_TE_CEG_WEIGHT V0_TNR_TE_CEG_WEIGHT;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCEgWeight() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CEG_WEIGHT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_WEIGHT.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CEG_WEIGHT.bits.te_ceg_weight1 = te_ceg_weight1 ;
    V0_TNR_TE_CEG_WEIGHT.bits.te_ceg_weight2 = te_ceg_weight2 ;
    V0_TNR_TE_CEG_WEIGHT.bits.te_ceg_weight3 = te_ceg_weight3 ;
    V0_TNR_TE_CEG_WEIGHT.bits.te_ceg_weight4 = te_ceg_weight4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CEG_WEIGHT.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CEG_WEIGHT.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeCMeanLpfCoef(HI_U32 u32Data,HI_U32 te_cmean_lpfcoef0, HI_U32 te_cmean_lpfcoef1, HI_U32 te_cmean_lpfcoef2, HI_U32 te_cmean_lpfcoef3, HI_U32 te_cmean_lpfcoef4, HI_U32 te_cmean_lpfcoef5, HI_U32 te_cmean_lpfcoef6, HI_U32 te_cmean_lpfcoef7, HI_U32 te_cmean_lpfcoef8)
{
    U_V0_TNR_TE_CMEAN_LPFCOEF1 V0_TNR_TE_CMEAN_LPFCOEF1;
    U_V0_TNR_TE_CMEAN_LPFCOEF2 V0_TNR_TE_CMEAN_LPFCOEF2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeCMeanLpfCoef() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_CMEAN_LPFCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMEAN_LPFCOEF1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMEAN_LPFCOEF1.bits.te_cmean_lpfcoef0 = te_cmean_lpfcoef0 ;
    V0_TNR_TE_CMEAN_LPFCOEF1.bits.te_cmean_lpfcoef1 = te_cmean_lpfcoef1 ;
    V0_TNR_TE_CMEAN_LPFCOEF1.bits.te_cmean_lpfcoef2 = te_cmean_lpfcoef2 ;
    V0_TNR_TE_CMEAN_LPFCOEF1.bits.te_cmean_lpfcoef3 = te_cmean_lpfcoef3 ;
    V0_TNR_TE_CMEAN_LPFCOEF1.bits.te_cmean_lpfcoef4 = te_cmean_lpfcoef4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMEAN_LPFCOEF1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMEAN_LPFCOEF1.u32);

    V0_TNR_TE_CMEAN_LPFCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMEAN_LPFCOEF2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_CMEAN_LPFCOEF2.bits.te_cmean_lpfcoef5 = te_cmean_lpfcoef5 ;
    V0_TNR_TE_CMEAN_LPFCOEF2.bits.te_cmean_lpfcoef6 = te_cmean_lpfcoef6 ;
    V0_TNR_TE_CMEAN_LPFCOEF2.bits.te_cmean_lpfcoef7 = te_cmean_lpfcoef7 ;
    V0_TNR_TE_CMEAN_LPFCOEF2.bits.te_cmean_lpfcoef8 = te_cmean_lpfcoef8 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_CMEAN_LPFCOEF2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_CMEAN_LPFCOEF2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYMtCK1(HI_U32 u32Data,HI_S32 te_ymt_core1, HI_U32 te_ymt_k1)
{
    U_V0_TNR_TE_YMT_CK1 V0_TNR_TE_YMT_CK1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtCK1() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YMT_CK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMT_CK1.bits.te_ymt_core1 = te_ymt_core1 ;
    V0_TNR_TE_YMT_CK1.bits.te_ymt_k1    = te_ymt_k1    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_CK1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYMtCK2(HI_U32 u32Data,HI_S32 te_ymt_core2, HI_U32 te_ymt_k2)
{
    U_V0_TNR_TE_YMT_CK2 V0_TNR_TE_YMT_CK2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtCK2() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YMT_CK2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMT_CK2.bits.te_ymt_core2 = te_ymt_core2 ;
    V0_TNR_TE_YMT_CK2.bits.te_ymt_k2    = te_ymt_k2    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK2.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_CK2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYEgCK1(HI_U32 u32Data,HI_S32 te_yeg_core1, HI_U32 te_yeg_k1)
{
    U_V0_TNR_TE_YEG_CK1 V0_TNR_TE_YEG_CK1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYEgCK1() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YEG_CK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YEG_CK1.bits.te_yeg_core1 = te_yeg_core1 ;
    V0_TNR_TE_YEG_CK1.bits.te_yeg_k1    = te_yeg_k1    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK1.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YEG_CK1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYMtCK(HI_U32 u32Data,HI_S32 te_ymt_core, HI_U32 te_ymt_k)
{
    U_V0_TNR_TE_YMT_CK V0_TNR_TE_YMT_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYMtCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YMT_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMT_CK.bits.te_ymt_core = te_ymt_core ;
    V0_TNR_TE_YMT_CK.bits.te_ymt_k    = te_ymt_k    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMT_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMT_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYEgCK(HI_U32 u32Data,HI_S32 te_yeg_core, HI_U32 te_yeg_k)
{
    U_V0_TNR_TE_YEG_CK V0_TNR_TE_YEG_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYEgCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YEG_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YEG_CK.bits.te_yeg_core = te_yeg_core ;
    V0_TNR_TE_YEG_CK.bits.te_yeg_k    = te_yeg_k    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YEG_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYRShift(HI_U32 u32Data,HI_U32 te_yrshift)
{
    U_V0_TNR_TE_YEG_CK V0_TNR_TE_YEG_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYRShift() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YEG_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YEG_CK.bits.te_yrshift = te_yrshift ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YEG_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YEG_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTeYMovingCK(HI_U32 u32Data,HI_S32 te_ymoving_core, HI_U32 te_ymoving_k)
{
    U_V0_TNR_TE_YMOVING_CK V0_TNR_TE_YMOVING_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTeYMovingCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TE_YMOVING_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMOVING_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TE_YMOVING_CK.bits.te_ymoving_core = te_ymoving_core ;
    V0_TNR_TE_YMOVING_CK.bits.te_ymoving_k    = te_ymoving_k    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TE_YMOVING_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TE_YMOVING_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCMotionCK(HI_U32 u32Data,HI_S32 tfm_cmotion_core, HI_U32 tfm_cmotion_k)
{
    U_V0_TNR_TFM_CMOTION_CK V0_TNR_TFM_CMOTION_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCMotionCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_CMOTION_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CMOTION_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_core = tfm_cmotion_core ;
    V0_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k    = tfm_cmotion_k    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CMOTION_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_CMOTION_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCEdgeCK(HI_U32 u32Data,HI_S32 tfm_cedge_core, HI_U32 tfm_cedge_k)
{
    U_V0_TNR_TFM_CEDGE_CK V0_TNR_TFM_CEDGE_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCEdgeCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_CEDGE_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_CEDGE_CK.bits.tfm_cedge_core = tfm_cedge_core ;
    V0_TNR_TFM_CEDGE_CK.bits.tfm_cedge_k    = tfm_cedge_k    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_CEDGE_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfm2DUpdate(HI_U32 u32Data,HI_U32 tfm_2dupdate)
{
    U_V0_TNR_TFM_CEDGE_CK V0_TNR_TFM_CEDGE_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfm2DUpdate() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_CEDGE_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_CEDGE_CK.bits.tfm_2dupdate = tfm_2dupdate ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_CEDGE_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCbCrUpdate(HI_U32 u32Data,HI_U32 tfm_cbcrupdate)
{
    U_V0_TNR_TFM_CEDGE_CK V0_TNR_TFM_CEDGE_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCbCrUpdate() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_CEDGE_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_CEDGE_CK.bits.tfm_cbcrupdate = tfm_cbcrupdate ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_CEDGE_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_CEDGE_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYMapping(HI_U32 u32Data,HI_U32 tfm_y_mapping_min, HI_U32 tfm_y_mapping_max)
{
    U_V0_TNR_TFM_Y_MAPPING_CTRL V0_TNR_TFM_Y_MAPPING_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_Y_MAPPING_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min = tfm_y_mapping_min ;
    V0_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max = tfm_y_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYMappingR(HI_U32 u32Data,HI_U32 tfm_y_mapping_r0, HI_U32 tfm_y_mapping_r1, HI_U32 tfm_y_mapping_r2, HI_U32 tfm_y_mapping_r3, HI_U32 tfm_y_mapping_r4)
{
    U_V0_TNR_TFM_Y_MAPPING_R0 V0_TNR_TFM_Y_MAPPING_R0;
    U_V0_TNR_TFM_Y_MAPPING_R1 V0_TNR_TFM_Y_MAPPING_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_Y_MAPPING_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0 = tfm_y_mapping_r0 ;
    V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1 = tfm_y_mapping_r1 ;
    V0_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2 = tfm_y_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_R0.u32);

    V0_TNR_TFM_Y_MAPPING_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3 = tfm_y_mapping_r3 ;
    V0_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4 = tfm_y_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYMappingT(HI_U32 u32Data,HI_U32 tfm_y_mapping_t1, HI_U32 tfm_y_mapping_t2, HI_U32 tfm_y_mapping_t3, HI_U32 tfm_y_mapping_t4, HI_U32 tfm_y_mapping_t5, HI_U32 tfm_y_mapping_t6)
{
    U_V0_TNR_TFM_Y_MAPPING_T1 V0_TNR_TFM_Y_MAPPING_T1;
    U_V0_TNR_TFM_Y_MAPPING_T2 V0_TNR_TFM_Y_MAPPING_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_Y_MAPPING_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1 = tfm_y_mapping_t1 ;
    V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2 = tfm_y_mapping_t2 ;
    V0_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3 = tfm_y_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_T1.u32);

    V0_TNR_TFM_Y_MAPPING_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4 = tfm_y_mapping_t4 ;
    V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5 = tfm_y_mapping_t5 ;
    V0_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6 = tfm_y_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYMappingK(HI_U32 u32Data,HI_S32 tfm_y_mapping_k1, HI_S32 tfm_y_mapping_k2, HI_S32 tfm_y_mapping_k3, HI_S32 tfm_y_mapping_k4)
{
    U_V0_TNR_TFM_Y_MAPPING_K1 V0_TNR_TFM_Y_MAPPING_K1;
    U_V0_TNR_TFM_Y_MAPPING_K2 V0_TNR_TFM_Y_MAPPING_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_Y_MAPPING_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1 = tfm_y_mapping_k1 ;
    V0_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2 = tfm_y_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_K1.u32);

    V0_TNR_TFM_Y_MAPPING_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3 = tfm_y_mapping_k3 ;
    V0_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4 = tfm_y_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_Y_MAPPING_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_Y_MAPPING_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCMapping(HI_U32 u32Data,HI_U32 tfm_c_mapping_min, HI_U32 tfm_c_mapping_max)
{
    U_V0_TNR_TFM_C_MAPPING_CTRL V0_TNR_TFM_C_MAPPING_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCMapping() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_C_MAPPING_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min = tfm_c_mapping_min ;
    V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max = tfm_c_mapping_max ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCMappingR(HI_U32 u32Data,HI_U32 tfm_c_mapping_r0, HI_U32 tfm_c_mapping_r1, HI_U32 tfm_c_mapping_r2, HI_U32 tfm_c_mapping_r3, HI_U32 tfm_c_mapping_r4)
{
    U_V0_TNR_TFM_C_MAPPING_R0 V0_TNR_TFM_C_MAPPING_R0;
    U_V0_TNR_TFM_C_MAPPING_R1 V0_TNR_TFM_C_MAPPING_R1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCMappingR() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_C_MAPPING_R0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_R0.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0 = tfm_c_mapping_r0 ;
    V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1 = tfm_c_mapping_r1 ;
    V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2 = tfm_c_mapping_r2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_R0.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_R0.u32);

    V0_TNR_TFM_C_MAPPING_R1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_R1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3 = tfm_c_mapping_r3 ;
    V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4 = tfm_c_mapping_r4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_R1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_R1.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCMappingT(HI_U32 u32Data,HI_U32 tfm_c_mapping_t1, HI_U32 tfm_c_mapping_t2, HI_U32 tfm_c_mapping_t3, HI_U32 tfm_c_mapping_t4, HI_U32 tfm_c_mapping_t5, HI_U32 tfm_c_mapping_t6)
{
    U_V0_TNR_TFM_C_MAPPING_T1 V0_TNR_TFM_C_MAPPING_T1;
    U_V0_TNR_TFM_C_MAPPING_T2 V0_TNR_TFM_C_MAPPING_T2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCMappingT() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_C_MAPPING_T1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_T1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = tfm_c_mapping_t1 ;
    V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = tfm_c_mapping_t2 ;
    V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = tfm_c_mapping_t3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_T1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_T1.u32);

    V0_TNR_TFM_C_MAPPING_T2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_T2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = tfm_c_mapping_t4 ;
    V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = tfm_c_mapping_t5 ;
    V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = tfm_c_mapping_t6 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_T2.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_T2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCMappingK(HI_U32 u32Data,HI_S32 tfm_c_mapping_k1, HI_S32 tfm_c_mapping_k2, HI_S32 tfm_c_mapping_k3, HI_S32 tfm_c_mapping_k4)
{
    U_V0_TNR_TFM_C_MAPPING_K1 V0_TNR_TFM_C_MAPPING_K1;
    U_V0_TNR_TFM_C_MAPPING_K2 V0_TNR_TFM_C_MAPPING_K2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCMappingK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_C_MAPPING_K1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_K1.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = tfm_c_mapping_k1 ;
    V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = tfm_c_mapping_k2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_K1.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_K1.u32);

    V0_TNR_TFM_C_MAPPING_K2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_K2.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = tfm_c_mapping_k3 ;
    V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = tfm_c_mapping_k4 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_C_MAPPING_K2.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_C_MAPPING_K2.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmHismTerm(HI_U32 u32Data,HI_U32 tfm_hism_term1, HI_U32 tfm_hism_term2)
{
    U_V0_TNR_TFM_HISTMOTION_BLEND V0_TNR_TFM_HISTMOTION_BLEND;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmHismTerm() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_HISTMOTION_BLEND.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_HISTMOTION_BLEND.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_HISTMOTION_BLEND.bits.tfm_hism_term1 = tfm_hism_term1 ;
    V0_TNR_TFM_HISTMOTION_BLEND.bits.tfm_hism_term2 = tfm_hism_term2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_HISTMOTION_BLEND.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_HISTMOTION_BLEND.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmHismAlpha(HI_U32 u32Data,HI_U32 tfm_hism_alpha1, HI_U32 tfm_hism_alpha2)
{
    U_V0_TNR_TFM_HISTMOTION_BLEND V0_TNR_TFM_HISTMOTION_BLEND;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmHismAlpha() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_HISTMOTION_BLEND.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_HISTMOTION_BLEND.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_HISTMOTION_BLEND.bits.tfm_hism_alpha1 = tfm_hism_alpha1 ;
    V0_TNR_TFM_HISTMOTION_BLEND.bits.tfm_hism_alpha2 = tfm_hism_alpha2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_HISTMOTION_BLEND.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_HISTMOTION_BLEND.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmFilterMode(HI_U32 u32Data,HI_U32 tfm_filter_mode)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmFilterMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_filter_mode = tfm_filter_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCalcMode(HI_U32 u32Data,HI_U32 tfm_calc_mode)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCalcMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_calc_mode = tfm_calc_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmSceneChangeEnable(HI_U32 u32Data,HI_U32 tfm_scenechange_en)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmSceneChangeEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_scenechange_en = tfm_scenechange_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmSceneChangeForMotionEnable(HI_U32 u32Data,HI_U32 tfm_scenechangeformotion_en)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmSceneChangeForMotionEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_scenechangeformotion_en = tfm_scenechangeformotion_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmGlobalMotionEnable(HI_U32 u32Data,HI_U32 tfm_globalmotion_en)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmGlobalMotionEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_globalmotion_en = tfm_globalmotion_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmCbCrUpdataMode(HI_U32 u32Data,HI_U32 tfm_cbcrupdate_mode)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmCbCrUpdataMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_cbcrupdate_mode = tfm_cbcrupdate_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfm1DLpfMode(HI_U32 u32Data,HI_U32 tfm_1dlpf_mode)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfm1DLpfMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_1dlpf_mode = tfm_1dlpf_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRYCMotionMode(HI_U32 u32Data,HI_U32 ymotion_mode, HI_U32 cmotion_mode)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRYCMotionMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.ymotion_mode = ymotion_mode ;
    V0_TNR_TFM_PARA.bits.cmotion_mode = cmotion_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmGlobalMotion(HI_U32 u32Data,HI_U32 tfm_globalmotion)
{
    U_V0_TNR_TFM_PARA V0_TNR_TFM_PARA;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmGlobalMotion() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_PARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_PARA.bits.tfm_globalmotion = tfm_globalmotion ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_PARA.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_PARA.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYMotionCK(HI_U32 u32Data,HI_S32 tfm_ymotion_core, HI_U32 tfm_ymotion_k)
{
    U_V0_TNR_TFM_YMOTION_CK V0_TNR_TFM_YMOTION_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYMotionCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_YMOTION_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_YMOTION_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_core = tfm_ymotion_core;
    V0_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k    = tfm_ymotion_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_YMOTION_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_YMOTION_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTfmYEdgeCK(HI_U32 u32Data,HI_S32 tfm_yedge_core, HI_U32 tfm_yedge_k)
{
    U_V0_TNR_TFM_YEG_CK V0_TNR_TFM_YEG_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTfmYEdgeCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TFM_YEG_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_YEG_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TFM_YEG_CK.bits.tfm_yedge_core = tfm_yedge_core;
    V0_TNR_TFM_YEG_CK.bits.tfm_yedge_k    = tfm_yedge_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TFM_YEG_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TFM_YEG_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtMeanCK(HI_U32 u32Data,HI_S32 tm_ymtmean_core, HI_U32 tm_ymtmean_k)
{
    U_V0_TNR_TM_YMTMEAN_CK V0_TNR_TM_YMTMEAN_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtMeanCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_YMTMEAN_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YMTMEAN_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_YMTMEAN_CK.bits.tm_ymtmean_core = tm_ymtmean_core;
    V0_TNR_TM_YMTMEAN_CK.bits.tm_ymtmean_k    = tm_ymtmean_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YMTMEAN_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TM_YMTMEAN_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTYMYCK(HI_U32 u32Data,HI_S32 tymy_core, HI_U32 tymy_k)
{
    U_V0_TNR_TYMYCK V0_TNR_TYMYCK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTYMYCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TYMYCK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TYMYCK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TYMYCK.bits.tymy_core = tymy_core;
    V0_TNR_TYMYCK.bits.tymy_k    = tymy_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TYMYCK.u32) + u32Data * VID_OFFSET), V0_TNR_TYMYCK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTYMEYCK(HI_U32 u32Data,HI_S32 tymey_core, HI_U32 tymey_k)
{
    U_V0_TNR_TYMEYCK V0_TNR_TYMEYCK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTYMEYCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TYMEYCK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TYMEYCK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TYMEYCK.bits.tymey_core = tymey_core;
    V0_TNR_TYMEYCK.bits.tymey_k    = tymey_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TYMEYCK.u32) + u32Data * VID_OFFSET), V0_TNR_TYMEYCK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTCMYCK(HI_U32 u32Data,HI_S32 tcmy_core, HI_U32 tcmy_k)
{
    U_V0_TNR_TCMYCK V0_TNR_TCMYCK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTCMYCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TCMYCK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TCMYCK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TCMYCK.bits.tcmy_core = tcmy_core;
    V0_TNR_TCMYCK.bits.tcmy_k    = tcmy_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TCMYCK.u32) + u32Data * VID_OFFSET), V0_TNR_TCMYCK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTCMEYCK(HI_U32 u32Data,HI_S32 tcmey_core, HI_U32 tcmey_k)
{
    U_V0_TNR_TCMEYCK V0_TNR_TCMEYCK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTCMEYCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TCMEYCK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TCMEYCK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TCMEYCK.bits.tcmey_core = tcmey_core;
    V0_TNR_TCMEYCK.bits.tcmey_k    = tcmey_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TCMEYCK.u32) + u32Data * VID_OFFSET), V0_TNR_TCMEYCK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtCK(HI_U32 u32Data,HI_S32 tm_cmt_core, HI_U32 tm_cmt_k)
{
    U_V0_TNR_TM_CMT_CK V0_TNR_TM_CMT_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_CMT_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_CMT_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_CMT_CK.bits.tm_cmt_core = tm_cmt_core;
    V0_TNR_TM_CMT_CK.bits.tm_cmt_k    = tm_cmt_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_CMT_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TM_CMT_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmYMtCK(HI_U32 u32Data,HI_S32 tm_ymt_core, HI_U32 tm_ymt_k)
{
    U_V0_TNR_TM_YMT_CK V0_TNR_TM_YMT_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmYMtCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_YMT_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YMT_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_YMT_CK.bits.tm_ymt_core = tm_ymt_core;
    V0_TNR_TM_YMT_CK.bits.tm_ymt_k    = tm_ymt_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_YMT_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TM_YMT_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRTmCMtMeanCK(HI_U32 u32Data,HI_S32 tm_cmtmean_core, HI_U32 tm_cmtmean_k)
{
    U_V0_TNR_TM_CMTMEAN_CK V0_TNR_TM_CMTMEAN_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRTmCMtMeanCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_TM_CMTMEAN_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_TM_CMTMEAN_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_TM_CMTMEAN_CK.bits.tm_cmtmean_core = tm_cmtmean_core;
    V0_TNR_TM_CMTMEAN_CK.bits.tm_cmtmean_k    = tm_cmtmean_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_TM_CMTMEAN_CK.u32) + u32Data * VID_OFFSET), V0_TNR_TM_CMTMEAN_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRYCNr1dEnable(HI_U32 u32Data,HI_U32 ynr_1d_en, HI_U32 cnr_1d_en)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRYCNr1dEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.ynr_1d_en = ynr_1d_en;
    V0_TNR_SNR_CTRL.bits.cnr_1d_en = cnr_1d_en;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRSenceChange(HI_U32 u32Data,HI_U32 sencechange)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRSenceChange() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.sencechange  = sencechange ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRCTRandNoise(HI_U32 u32Data,HI_U32 ctrandnoise_en, HI_U32 ctrandnoise_bit)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRCTRandNoise() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.ctrandnoise_en  = ctrandnoise_en ;
    V0_TNR_SNR_CTRL.bits.ctrandnoise_bit = ctrandnoise_bit;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRYTRandNoise(HI_U32 u32Data,HI_U32 ytrandnoise_en, HI_U32 ytrandnoise_bit)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRYTRandNoise() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.ytrandnoise_en  = ytrandnoise_en ;
    V0_TNR_SNR_CTRL.bits.ytrandnoise_bit = ytrandnoise_bit;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DEdgeMode(HI_U32 u32Data,HI_U32 t2dedgemode)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DEdgeMode() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.t2dedgemode  = t2dedgemode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DMeanEnable(HI_U32 u32Data,HI_U32 t2dmeanen)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DMeanEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.t2dmeanen  = t2dmeanen ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DMedEnable(HI_U32 u32Data,HI_U32 t2dmeden)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DMedEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.t2dmeden  = t2dmeden ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DMMEnable(HI_U32 u32Data,HI_U32 t2dmmen)
{
    U_V0_TNR_SNR_CTRL V0_TNR_SNR_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DMMEnable() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_SNR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_SNR_CTRL.bits.t2dmmen  = t2dmmen ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_SNR_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_SNR_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRSChange(HI_U32 u32Data,HI_U32 s_change)
{
    U_V0_TNR_BLEND_CTRL V0_TNR_BLEND_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRSChange() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_BLEND_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_BLEND_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_BLEND_CTRL.bits.s_change = s_change;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_BLEND_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_BLEND_CTRL.u32);
    return;
}
HI_VOID VDP_VID_SetYCMotionStrengthEn(HI_U32 u32Data,HI_U32 ymotion_strength_en, HI_S32 cmotion_strength_en)
{
    U_V0_TNR_BLEND_CTRL V0_TNR_BLEND_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetYCMotionStrengthEn() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_BLEND_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_BLEND_CTRL.u32) + u32Data * VID_OFFSET));
    V0_TNR_BLEND_CTRL.bits.ymotion_strength_en = ymotion_strength_en;
    V0_TNR_BLEND_CTRL.bits.cmotion_strength_en = cmotion_strength_en;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_BLEND_CTRL.u32) + u32Data * VID_OFFSET), V0_TNR_BLEND_CTRL.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DEdgeCK(HI_U32 u32Data,HI_S32 t2d_edge_core, HI_U32 t2d_edge_k)
{
    U_V0_TNR_T2D_EDGE_CK V0_TNR_T2D_EDGE_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DEdgeCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_T2D_EDGE_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_T2D_EDGE_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_T2D_EDGE_CK.bits.t2d_edge_core = t2d_edge_core;
    V0_TNR_T2D_EDGE_CK.bits.t2d_edge_k    = t2d_edge_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_T2D_EDGE_CK.u32) + u32Data * VID_OFFSET), V0_TNR_T2D_EDGE_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRT2DNoiseCK(HI_U32 u32Data,HI_S32 t2d_noise_core, HI_U32 t2d_noise_k)
{
    U_V0_TNR_T2D_NOISE_CK V0_TNR_T2D_NOISE_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRT2DNoiseCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_T2D_NOISE_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_T2D_NOISE_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_T2D_NOISE_CK.bits.t2d_noise_core = t2d_noise_core;
    V0_TNR_T2D_NOISE_CK.bits.t2d_noise_k    = t2d_noise_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_T2D_NOISE_CK.u32) + u32Data * VID_OFFSET), V0_TNR_T2D_NOISE_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNREdgeCMovingCK(HI_U32 u32Data,HI_S32 edge_cmoving_core, HI_U32 edge_cmoving_k)
{
    U_V0_TNR_EDGE_CMOVING_CK V0_TNR_EDGE_CMOVING_CK;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNREdgeCMovingCK() select wrong video layer ID\n");
      return ;
    }
    V0_TNR_EDGE_CMOVING_CK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_EDGE_CMOVING_CK.u32) + u32Data * VID_OFFSET));
    V0_TNR_EDGE_CMOVING_CK.bits.edge_cmoving_core = edge_cmoving_core;
    V0_TNR_EDGE_CMOVING_CK.bits.edge_cmoving_k    = edge_cmoving_k   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_EDGE_CMOVING_CK.u32) + u32Data * VID_OFFSET), V0_TNR_EDGE_CMOVING_CK.u32);
    return;
}

HI_VOID VDP_VID_SetTNRMarketCoordinate(HI_U32 u32Data, HI_U32 market_coordinate)
{
	U_V0_TNR_MARKET V0_TNR_MARKET;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMarketCoordinate Select Wrong Layer ID\n");
		return ;
	}
	
	V0_TNR_MARKET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_MARKET.u32) + u32Data * VID_OFFSET));
	V0_TNR_MARKET.bits.market_coordinate = market_coordinate;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_MARKET.u32) + u32Data * VID_OFFSET),V0_TNR_MARKET.u32);
	
	return ;
}

HI_VOID VDP_VID_SetTNRMotionEdgeRatioLut(HI_U32 u32Data,HI_U32 motion_edge_ratio_lut[8][8])
{
    U_V0_TNR_CLUT10  V0_TNR_CLUT10 ;
    U_V0_TNR_CLUT11  V0_TNR_CLUT11 ;
    U_V0_TNR_CLUT12  V0_TNR_CLUT12 ;
    U_V0_TNR_CLUT13  V0_TNR_CLUT13 ;
    U_V0_TNR_CLUT14  V0_TNR_CLUT14 ;
    U_V0_TNR_CLUT15  V0_TNR_CLUT15 ;
    U_V0_TNR_CLUT16  V0_TNR_CLUT16 ;
    U_V0_TNR_CLUT17  V0_TNR_CLUT17 ;
    U_V0_TNR_CLUT18  V0_TNR_CLUT18 ;
    U_V0_TNR_CLUT19  V0_TNR_CLUT19 ;
    U_V0_TNR_CLUT110 V0_TNR_CLUT110;
    U_V0_TNR_CLUT111 V0_TNR_CLUT111;
    U_V0_TNR_CLUT112 V0_TNR_CLUT112;
    U_V0_TNR_CLUT113 V0_TNR_CLUT113;
    U_V0_TNR_CLUT114 V0_TNR_CLUT114;
    U_V0_TNR_CLUT115 V0_TNR_CLUT115;

    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRMotionEdgeRatioLut() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_CLUT10.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT10.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT11.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT11.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT12.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT12.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT13.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT13.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT14.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT14.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT15.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT15.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT16.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT16.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT17.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT17.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT18.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT18.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT19.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT19.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT110.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT110.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT111.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT111.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT112.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT112.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT113.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT113.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT114.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT114.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT115.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT115.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT10.bits.data0  = motion_edge_ratio_lut[0][0];
    V0_TNR_CLUT10.bits.data1  = motion_edge_ratio_lut[0][1];
    V0_TNR_CLUT10.bits.data2  = motion_edge_ratio_lut[0][2];
    V0_TNR_CLUT10.bits.data3  = motion_edge_ratio_lut[0][3];
    V0_TNR_CLUT11.bits.data0  = motion_edge_ratio_lut[0][4];
    V0_TNR_CLUT11.bits.data1  = motion_edge_ratio_lut[0][5];
    V0_TNR_CLUT11.bits.data2  = motion_edge_ratio_lut[0][6];
    V0_TNR_CLUT11.bits.data3  = motion_edge_ratio_lut[0][7];
    V0_TNR_CLUT12.bits.data0  = motion_edge_ratio_lut[1][0];
    V0_TNR_CLUT12.bits.data1  = motion_edge_ratio_lut[1][1];
    V0_TNR_CLUT12.bits.data2  = motion_edge_ratio_lut[1][2];
    V0_TNR_CLUT12.bits.data3  = motion_edge_ratio_lut[1][3];
    V0_TNR_CLUT13.bits.data0  = motion_edge_ratio_lut[1][4];
    V0_TNR_CLUT13.bits.data1  = motion_edge_ratio_lut[1][5];
    V0_TNR_CLUT13.bits.data2  = motion_edge_ratio_lut[1][6];
    V0_TNR_CLUT13.bits.data3  = motion_edge_ratio_lut[1][7];
    V0_TNR_CLUT14.bits.data0  = motion_edge_ratio_lut[2][0];
    V0_TNR_CLUT14.bits.data1  = motion_edge_ratio_lut[2][1];
    V0_TNR_CLUT14.bits.data2  = motion_edge_ratio_lut[2][2];
    V0_TNR_CLUT14.bits.data3  = motion_edge_ratio_lut[2][3];
    V0_TNR_CLUT15.bits.data0  = motion_edge_ratio_lut[2][4];
    V0_TNR_CLUT15.bits.data1  = motion_edge_ratio_lut[2][5];
    V0_TNR_CLUT15.bits.data2  = motion_edge_ratio_lut[2][6];
    V0_TNR_CLUT15.bits.data3  = motion_edge_ratio_lut[2][7];
    V0_TNR_CLUT16.bits.data0  = motion_edge_ratio_lut[3][0];
    V0_TNR_CLUT16.bits.data1  = motion_edge_ratio_lut[3][1];
    V0_TNR_CLUT16.bits.data2  = motion_edge_ratio_lut[3][2];
    V0_TNR_CLUT16.bits.data3  = motion_edge_ratio_lut[3][3];
    V0_TNR_CLUT17.bits.data0  = motion_edge_ratio_lut[3][4];
    V0_TNR_CLUT17.bits.data1  = motion_edge_ratio_lut[3][5];
    V0_TNR_CLUT17.bits.data2  = motion_edge_ratio_lut[3][6];
    V0_TNR_CLUT17.bits.data3  = motion_edge_ratio_lut[3][7];
    V0_TNR_CLUT18.bits.data0  = motion_edge_ratio_lut[4][0];
    V0_TNR_CLUT18.bits.data1  = motion_edge_ratio_lut[4][1];
    V0_TNR_CLUT18.bits.data2  = motion_edge_ratio_lut[4][2];
    V0_TNR_CLUT18.bits.data3  = motion_edge_ratio_lut[4][3];
    V0_TNR_CLUT19.bits.data0  = motion_edge_ratio_lut[4][4];
    V0_TNR_CLUT19.bits.data1  = motion_edge_ratio_lut[4][5];
    V0_TNR_CLUT19.bits.data2  = motion_edge_ratio_lut[4][6];
    V0_TNR_CLUT19.bits.data3  = motion_edge_ratio_lut[4][7];
    V0_TNR_CLUT110.bits.data0 = motion_edge_ratio_lut[5][0];
    V0_TNR_CLUT110.bits.data1 = motion_edge_ratio_lut[5][1];
    V0_TNR_CLUT110.bits.data2 = motion_edge_ratio_lut[5][2];
    V0_TNR_CLUT110.bits.data3 = motion_edge_ratio_lut[5][3];
    V0_TNR_CLUT111.bits.data0 = motion_edge_ratio_lut[5][4];
    V0_TNR_CLUT111.bits.data1 = motion_edge_ratio_lut[5][5];
    V0_TNR_CLUT111.bits.data2 = motion_edge_ratio_lut[5][6];
    V0_TNR_CLUT111.bits.data3 = motion_edge_ratio_lut[5][7];
    V0_TNR_CLUT112.bits.data0 = motion_edge_ratio_lut[6][0];
    V0_TNR_CLUT112.bits.data1 = motion_edge_ratio_lut[6][1];
    V0_TNR_CLUT112.bits.data2 = motion_edge_ratio_lut[6][2];
    V0_TNR_CLUT112.bits.data3 = motion_edge_ratio_lut[6][3];
    V0_TNR_CLUT113.bits.data0 = motion_edge_ratio_lut[6][4];
    V0_TNR_CLUT113.bits.data1 = motion_edge_ratio_lut[6][5];
    V0_TNR_CLUT113.bits.data2 = motion_edge_ratio_lut[6][6];
    V0_TNR_CLUT113.bits.data3 = motion_edge_ratio_lut[6][7];
    V0_TNR_CLUT114.bits.data0 = motion_edge_ratio_lut[7][0];
    V0_TNR_CLUT114.bits.data1 = motion_edge_ratio_lut[7][1];
    V0_TNR_CLUT114.bits.data2 = motion_edge_ratio_lut[7][2];
    V0_TNR_CLUT114.bits.data3 = motion_edge_ratio_lut[7][3];
    V0_TNR_CLUT115.bits.data0 = motion_edge_ratio_lut[7][4];
    V0_TNR_CLUT115.bits.data1 = motion_edge_ratio_lut[7][5];
    V0_TNR_CLUT115.bits.data2 = motion_edge_ratio_lut[7][6];
    V0_TNR_CLUT115.bits.data3 = motion_edge_ratio_lut[7][7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT10.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT10.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT11.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT11.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT12.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT12.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT13.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT13.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT14.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT14.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT15.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT15.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT16.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT16.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT17.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT17.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT18.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT18.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT19.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT19.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT110.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT110.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT111.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT111.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT112.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT112.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT113.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT113.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT114.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT114.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT115.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT115.u32);
    return;
}

HI_VOID VDP_VID_SetTNRCbCrLut(HI_U32 u32Data,HI_U32 cbcr_lut[16][32])
{
    U_V0_TNR_CLUT20  V0_TNR_CLUT20 ;
    U_V0_TNR_CLUT21  V0_TNR_CLUT21 ;
    U_V0_TNR_CLUT22  V0_TNR_CLUT22 ;
    U_V0_TNR_CLUT23  V0_TNR_CLUT23 ;
    U_V0_TNR_CLUT24  V0_TNR_CLUT24 ;
    U_V0_TNR_CLUT25  V0_TNR_CLUT25 ;
    U_V0_TNR_CLUT26  V0_TNR_CLUT26 ;
    U_V0_TNR_CLUT27  V0_TNR_CLUT27 ;
    U_V0_TNR_CLUT28  V0_TNR_CLUT28 ;
    U_V0_TNR_CLUT29  V0_TNR_CLUT29 ;
    U_V0_TNR_CLUT210 V0_TNR_CLUT210;
    U_V0_TNR_CLUT211 V0_TNR_CLUT211;
    U_V0_TNR_CLUT212 V0_TNR_CLUT212;
    U_V0_TNR_CLUT213 V0_TNR_CLUT213;
    U_V0_TNR_CLUT214 V0_TNR_CLUT214;
    U_V0_TNR_CLUT215 V0_TNR_CLUT215;
    U_V0_TNR_CLUT216 V0_TNR_CLUT216;
    U_V0_TNR_CLUT217 V0_TNR_CLUT217;
    U_V0_TNR_CLUT218 V0_TNR_CLUT218;
    U_V0_TNR_CLUT219 V0_TNR_CLUT219;
    U_V0_TNR_CLUT220 V0_TNR_CLUT220;
    U_V0_TNR_CLUT221 V0_TNR_CLUT221;
    U_V0_TNR_CLUT222 V0_TNR_CLUT222;
    U_V0_TNR_CLUT223 V0_TNR_CLUT223;
    U_V0_TNR_CLUT224 V0_TNR_CLUT224;
    U_V0_TNR_CLUT225 V0_TNR_CLUT225;
    U_V0_TNR_CLUT226 V0_TNR_CLUT226;
    U_V0_TNR_CLUT227 V0_TNR_CLUT227;
    U_V0_TNR_CLUT228 V0_TNR_CLUT228;
    U_V0_TNR_CLUT229 V0_TNR_CLUT229;
    U_V0_TNR_CLUT230 V0_TNR_CLUT230;
    U_V0_TNR_CLUT231 V0_TNR_CLUT231;
    U_V0_TNR_CLUT232 V0_TNR_CLUT232;
    U_V0_TNR_CLUT233 V0_TNR_CLUT233;
    U_V0_TNR_CLUT234 V0_TNR_CLUT234;
    U_V0_TNR_CLUT235 V0_TNR_CLUT235;
    U_V0_TNR_CLUT236 V0_TNR_CLUT236;
    U_V0_TNR_CLUT237 V0_TNR_CLUT237;
    U_V0_TNR_CLUT238 V0_TNR_CLUT238;
    U_V0_TNR_CLUT239 V0_TNR_CLUT239;
    U_V0_TNR_CLUT240 V0_TNR_CLUT240;
    U_V0_TNR_CLUT241 V0_TNR_CLUT241;
    U_V0_TNR_CLUT242 V0_TNR_CLUT242;
    U_V0_TNR_CLUT243 V0_TNR_CLUT243;
    U_V0_TNR_CLUT244 V0_TNR_CLUT244;
    U_V0_TNR_CLUT245 V0_TNR_CLUT245;
    U_V0_TNR_CLUT246 V0_TNR_CLUT246;
    U_V0_TNR_CLUT247 V0_TNR_CLUT247;
    U_V0_TNR_CLUT248 V0_TNR_CLUT248;
    U_V0_TNR_CLUT249 V0_TNR_CLUT249;
    U_V0_TNR_CLUT250 V0_TNR_CLUT250;
    U_V0_TNR_CLUT251 V0_TNR_CLUT251;
    U_V0_TNR_CLUT252 V0_TNR_CLUT252;
    U_V0_TNR_CLUT253 V0_TNR_CLUT253;
    U_V0_TNR_CLUT254 V0_TNR_CLUT254;
    U_V0_TNR_CLUT255 V0_TNR_CLUT255;
    U_V0_TNR_CLUT256 V0_TNR_CLUT256;
    U_V0_TNR_CLUT257 V0_TNR_CLUT257;
    U_V0_TNR_CLUT258 V0_TNR_CLUT258;
    U_V0_TNR_CLUT259 V0_TNR_CLUT259;
    U_V0_TNR_CLUT260 V0_TNR_CLUT260;
    U_V0_TNR_CLUT261 V0_TNR_CLUT261;
    U_V0_TNR_CLUT262 V0_TNR_CLUT262;
    U_V0_TNR_CLUT263 V0_TNR_CLUT263;
    U_V0_TNR_CLUT264 V0_TNR_CLUT264;
    U_V0_TNR_CLUT265 V0_TNR_CLUT265;
    U_V0_TNR_CLUT266 V0_TNR_CLUT266;
    U_V0_TNR_CLUT267 V0_TNR_CLUT267;
    U_V0_TNR_CLUT268 V0_TNR_CLUT268;
    U_V0_TNR_CLUT269 V0_TNR_CLUT269;
    U_V0_TNR_CLUT270 V0_TNR_CLUT270;
    U_V0_TNR_CLUT271 V0_TNR_CLUT271;
    U_V0_TNR_CLUT272 V0_TNR_CLUT272;
    U_V0_TNR_CLUT273 V0_TNR_CLUT273;
    U_V0_TNR_CLUT274 V0_TNR_CLUT274;
    U_V0_TNR_CLUT275 V0_TNR_CLUT275;
    U_V0_TNR_CLUT276 V0_TNR_CLUT276;
    U_V0_TNR_CLUT277 V0_TNR_CLUT277;
    U_V0_TNR_CLUT278 V0_TNR_CLUT278;
    U_V0_TNR_CLUT279 V0_TNR_CLUT279;
    U_V0_TNR_CLUT280 V0_TNR_CLUT280;
    U_V0_TNR_CLUT281 V0_TNR_CLUT281;
    U_V0_TNR_CLUT282 V0_TNR_CLUT282;
    U_V0_TNR_CLUT283 V0_TNR_CLUT283;
    U_V0_TNR_CLUT284 V0_TNR_CLUT284;
    U_V0_TNR_CLUT285 V0_TNR_CLUT285;
    U_V0_TNR_CLUT286 V0_TNR_CLUT286;
    U_V0_TNR_CLUT287 V0_TNR_CLUT287;
    U_V0_TNR_CLUT288 V0_TNR_CLUT288;
    U_V0_TNR_CLUT289 V0_TNR_CLUT289;
    U_V0_TNR_CLUT290 V0_TNR_CLUT290;
    U_V0_TNR_CLUT291 V0_TNR_CLUT291;
    U_V0_TNR_CLUT292 V0_TNR_CLUT292;
    U_V0_TNR_CLUT293 V0_TNR_CLUT293;
    U_V0_TNR_CLUT294 V0_TNR_CLUT294;
    U_V0_TNR_CLUT295 V0_TNR_CLUT295;
    U_V0_TNR_CLUT296 V0_TNR_CLUT296;
    U_V0_TNR_CLUT297 V0_TNR_CLUT297;
    U_V0_TNR_CLUT298 V0_TNR_CLUT298;
    U_V0_TNR_CLUT299 V0_TNR_CLUT299;
    U_V0_TNR_CLUT2100 V0_TNR_CLUT2100;
    U_V0_TNR_CLUT2101 V0_TNR_CLUT2101;
    U_V0_TNR_CLUT2102 V0_TNR_CLUT2102;
    U_V0_TNR_CLUT2103 V0_TNR_CLUT2103;
    U_V0_TNR_CLUT2104 V0_TNR_CLUT2104;
    U_V0_TNR_CLUT2105 V0_TNR_CLUT2105;
    U_V0_TNR_CLUT2106 V0_TNR_CLUT2106;
    U_V0_TNR_CLUT2107 V0_TNR_CLUT2107;
    U_V0_TNR_CLUT2108 V0_TNR_CLUT2108;
    U_V0_TNR_CLUT2109 V0_TNR_CLUT2109;
    U_V0_TNR_CLUT2110 V0_TNR_CLUT2110;
    U_V0_TNR_CLUT2111 V0_TNR_CLUT2111;
    U_V0_TNR_CLUT2112 V0_TNR_CLUT2112;
    U_V0_TNR_CLUT2113 V0_TNR_CLUT2113;
    U_V0_TNR_CLUT2114 V0_TNR_CLUT2114;
    U_V0_TNR_CLUT2115 V0_TNR_CLUT2115;
    U_V0_TNR_CLUT2116 V0_TNR_CLUT2116;
    U_V0_TNR_CLUT2117 V0_TNR_CLUT2117;
    U_V0_TNR_CLUT2118 V0_TNR_CLUT2118;
    U_V0_TNR_CLUT2119 V0_TNR_CLUT2119;
    U_V0_TNR_CLUT2120 V0_TNR_CLUT2120;
    U_V0_TNR_CLUT2121 V0_TNR_CLUT2121;
    U_V0_TNR_CLUT2122 V0_TNR_CLUT2122;
    U_V0_TNR_CLUT2123 V0_TNR_CLUT2123;
    U_V0_TNR_CLUT2124 V0_TNR_CLUT2124;
    U_V0_TNR_CLUT2125 V0_TNR_CLUT2125;
    U_V0_TNR_CLUT2126 V0_TNR_CLUT2126;
    U_V0_TNR_CLUT2127 V0_TNR_CLUT2127;

    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRCbCrLut() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_CLUT20.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT20.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT21.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT21.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT22.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT22.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT23.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT23.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT24.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT24.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT25.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT25.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT26.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT26.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT27.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT27.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT28.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT28.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT29.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT29.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT210.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT210.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT211.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT211.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT212.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT212.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT213.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT213.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT214.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT214.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT215.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT215.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT216.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT216.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT217.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT217.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT218.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT218.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT219.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT219.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT220.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT220.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT221.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT221.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT222.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT222.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT223.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT223.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT224.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT224.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT225.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT225.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT226.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT226.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT227.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT227.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT228.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT228.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT229.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT229.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT230.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT230.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT231.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT231.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT232.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT232.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT233.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT233.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT234.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT234.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT235.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT235.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT236.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT236.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT237.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT237.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT238.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT238.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT239.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT239.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT240.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT240.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT241.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT241.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT242.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT242.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT243.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT243.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT244.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT244.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT245.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT245.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT246.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT246.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT247.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT247.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT248.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT248.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT249.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT249.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT250.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT250.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT251.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT251.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT252.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT252.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT253.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT253.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT254.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT254.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT255.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT255.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT256.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT256.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT257.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT257.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT258.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT258.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT259.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT259.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT260.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT260.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT261.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT261.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT262.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT262.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT263.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT263.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT264.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT264.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT265.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT265.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT266.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT266.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT267.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT267.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT268.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT268.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT269.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT269.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT270.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT270.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT271.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT271.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT272.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT272.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT273.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT273.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT274.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT274.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT275.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT275.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT276.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT276.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT277.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT277.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT278.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT278.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT279.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT279.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT280.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT280.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT281.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT281.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT282.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT282.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT283.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT283.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT284.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT284.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT285.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT285.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT286.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT286.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT287.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT287.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT288.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT288.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT289.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT289.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT290.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT290.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT291.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT291.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT292.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT292.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT293.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT293.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT294.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT294.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT295.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT295.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT296.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT296.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT297.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT297.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT298.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT298.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT299.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT299.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2100.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2100.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2101.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2101.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2102.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2102.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2103.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2103.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2104.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2104.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2105.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2105.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2106.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2106.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2107.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2107.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2108.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2108.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2109.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2109.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2110.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2110.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2111.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2111.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2112.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2112.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2113.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2113.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2114.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2114.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2115.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2115.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2116.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2116.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2117.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2117.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2118.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2118.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2119.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2119.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2120.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2120.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2121.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2121.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2122.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2122.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2123.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2123.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2124.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2124.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2125.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2125.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2126.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2126.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT2127.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2127.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT20.bits.data0   = cbcr_lut[ 0][ 0];
    V0_TNR_CLUT20.bits.data1   = cbcr_lut[ 0][ 1];
    V0_TNR_CLUT20.bits.data2   = cbcr_lut[ 0][ 2];
    V0_TNR_CLUT20.bits.data3   = cbcr_lut[ 0][ 3];
    V0_TNR_CLUT21.bits.data0   = cbcr_lut[ 0][ 4];
    V0_TNR_CLUT21.bits.data1   = cbcr_lut[ 0][ 5];
    V0_TNR_CLUT21.bits.data2   = cbcr_lut[ 0][ 6];
    V0_TNR_CLUT21.bits.data3   = cbcr_lut[ 0][ 7];
    V0_TNR_CLUT22.bits.data0   = cbcr_lut[ 0][ 8];
    V0_TNR_CLUT22.bits.data1   = cbcr_lut[ 0][ 9];
    V0_TNR_CLUT22.bits.data2   = cbcr_lut[ 0][10];
    V0_TNR_CLUT22.bits.data3   = cbcr_lut[ 0][11];
    V0_TNR_CLUT23.bits.data0   = cbcr_lut[ 0][12];
    V0_TNR_CLUT23.bits.data1   = cbcr_lut[ 0][13];
    V0_TNR_CLUT23.bits.data2   = cbcr_lut[ 0][14];
    V0_TNR_CLUT23.bits.data3   = cbcr_lut[ 0][15];
    V0_TNR_CLUT24.bits.data0   = cbcr_lut[ 0][16];
    V0_TNR_CLUT24.bits.data1   = cbcr_lut[ 0][17];
    V0_TNR_CLUT24.bits.data2   = cbcr_lut[ 0][18];
    V0_TNR_CLUT24.bits.data3   = cbcr_lut[ 0][19];
    V0_TNR_CLUT25.bits.data0   = cbcr_lut[ 0][20];
    V0_TNR_CLUT25.bits.data1   = cbcr_lut[ 0][21];
    V0_TNR_CLUT25.bits.data2   = cbcr_lut[ 0][22];
    V0_TNR_CLUT25.bits.data3   = cbcr_lut[ 0][23];
    V0_TNR_CLUT26.bits.data0   = cbcr_lut[ 0][24];
    V0_TNR_CLUT26.bits.data1   = cbcr_lut[ 0][25];
    V0_TNR_CLUT26.bits.data2   = cbcr_lut[ 0][26];
    V0_TNR_CLUT26.bits.data3   = cbcr_lut[ 0][27];
    V0_TNR_CLUT27.bits.data0   = cbcr_lut[ 0][28];
    V0_TNR_CLUT27.bits.data1   = cbcr_lut[ 0][29];
    V0_TNR_CLUT27.bits.data2   = cbcr_lut[ 0][30];
    V0_TNR_CLUT27.bits.data3   = cbcr_lut[ 0][31];
    V0_TNR_CLUT28.bits.data0   = cbcr_lut[ 1][ 0];
    V0_TNR_CLUT28.bits.data1   = cbcr_lut[ 1][ 1];
    V0_TNR_CLUT28.bits.data2   = cbcr_lut[ 1][ 2];
    V0_TNR_CLUT28.bits.data3   = cbcr_lut[ 1][ 3];
    V0_TNR_CLUT29.bits.data0   = cbcr_lut[ 1][ 4];
    V0_TNR_CLUT29.bits.data1   = cbcr_lut[ 1][ 5];
    V0_TNR_CLUT29.bits.data2   = cbcr_lut[ 1][ 6];
    V0_TNR_CLUT29.bits.data3   = cbcr_lut[ 1][ 7];
    V0_TNR_CLUT210.bits.data0  = cbcr_lut[ 1][ 8];
    V0_TNR_CLUT210.bits.data1  = cbcr_lut[ 1][ 9];
    V0_TNR_CLUT210.bits.data2  = cbcr_lut[ 1][10];
    V0_TNR_CLUT210.bits.data3  = cbcr_lut[ 1][11];
    V0_TNR_CLUT211.bits.data0  = cbcr_lut[ 1][12];
    V0_TNR_CLUT211.bits.data1  = cbcr_lut[ 1][13];
    V0_TNR_CLUT211.bits.data2  = cbcr_lut[ 1][14];
    V0_TNR_CLUT211.bits.data3  = cbcr_lut[ 1][15];
    V0_TNR_CLUT212.bits.data0  = cbcr_lut[ 1][16];
    V0_TNR_CLUT212.bits.data1  = cbcr_lut[ 1][17];
    V0_TNR_CLUT212.bits.data2  = cbcr_lut[ 1][18];
    V0_TNR_CLUT212.bits.data3  = cbcr_lut[ 1][19];
    V0_TNR_CLUT213.bits.data0  = cbcr_lut[ 1][20];
    V0_TNR_CLUT213.bits.data1  = cbcr_lut[ 1][21];
    V0_TNR_CLUT213.bits.data2  = cbcr_lut[ 1][22];
    V0_TNR_CLUT213.bits.data3  = cbcr_lut[ 1][23];
    V0_TNR_CLUT214.bits.data0  = cbcr_lut[ 1][24];
    V0_TNR_CLUT214.bits.data1  = cbcr_lut[ 1][25];
    V0_TNR_CLUT214.bits.data2  = cbcr_lut[ 1][26];
    V0_TNR_CLUT214.bits.data3  = cbcr_lut[ 1][27];
    V0_TNR_CLUT215.bits.data0  = cbcr_lut[ 1][28];
    V0_TNR_CLUT215.bits.data1  = cbcr_lut[ 1][29];
    V0_TNR_CLUT215.bits.data2  = cbcr_lut[ 1][30];
    V0_TNR_CLUT215.bits.data3  = cbcr_lut[ 1][31];
    V0_TNR_CLUT216.bits.data0  = cbcr_lut[ 2][ 0];
    V0_TNR_CLUT216.bits.data1  = cbcr_lut[ 2][ 1];
    V0_TNR_CLUT216.bits.data2  = cbcr_lut[ 2][ 2];
    V0_TNR_CLUT216.bits.data3  = cbcr_lut[ 2][ 3];
    V0_TNR_CLUT217.bits.data0  = cbcr_lut[ 2][ 4];
    V0_TNR_CLUT217.bits.data1  = cbcr_lut[ 2][ 5];
    V0_TNR_CLUT217.bits.data2  = cbcr_lut[ 2][ 6];
    V0_TNR_CLUT217.bits.data3  = cbcr_lut[ 2][ 7];
    V0_TNR_CLUT218.bits.data0  = cbcr_lut[ 2][ 8];
    V0_TNR_CLUT218.bits.data1  = cbcr_lut[ 2][ 9];
    V0_TNR_CLUT218.bits.data2  = cbcr_lut[ 2][10];
    V0_TNR_CLUT218.bits.data3  = cbcr_lut[ 2][11];
    V0_TNR_CLUT219.bits.data0  = cbcr_lut[ 2][12];
    V0_TNR_CLUT219.bits.data1  = cbcr_lut[ 2][13];
    V0_TNR_CLUT219.bits.data2  = cbcr_lut[ 2][14];
    V0_TNR_CLUT219.bits.data3  = cbcr_lut[ 2][15];
    V0_TNR_CLUT220.bits.data0  = cbcr_lut[ 2][16];
    V0_TNR_CLUT220.bits.data1  = cbcr_lut[ 2][17];
    V0_TNR_CLUT220.bits.data2  = cbcr_lut[ 2][18];
    V0_TNR_CLUT220.bits.data3  = cbcr_lut[ 2][19];
    V0_TNR_CLUT221.bits.data0  = cbcr_lut[ 2][20];
    V0_TNR_CLUT221.bits.data1  = cbcr_lut[ 2][21];
    V0_TNR_CLUT221.bits.data2  = cbcr_lut[ 2][22];
    V0_TNR_CLUT221.bits.data3  = cbcr_lut[ 2][23];
    V0_TNR_CLUT222.bits.data0  = cbcr_lut[ 2][24];
    V0_TNR_CLUT222.bits.data1  = cbcr_lut[ 2][25];
    V0_TNR_CLUT222.bits.data2  = cbcr_lut[ 2][26];
    V0_TNR_CLUT222.bits.data3  = cbcr_lut[ 2][27];
    V0_TNR_CLUT223.bits.data0  = cbcr_lut[ 2][28];
    V0_TNR_CLUT223.bits.data1  = cbcr_lut[ 2][29];
    V0_TNR_CLUT223.bits.data2  = cbcr_lut[ 2][30];
    V0_TNR_CLUT223.bits.data3  = cbcr_lut[ 2][31];
    V0_TNR_CLUT224.bits.data0  = cbcr_lut[ 3][ 0];
    V0_TNR_CLUT224.bits.data1  = cbcr_lut[ 3][ 1];
    V0_TNR_CLUT224.bits.data2  = cbcr_lut[ 3][ 2];
    V0_TNR_CLUT224.bits.data3  = cbcr_lut[ 3][ 3];
    V0_TNR_CLUT225.bits.data0  = cbcr_lut[ 3][ 4];
    V0_TNR_CLUT225.bits.data1  = cbcr_lut[ 3][ 5];
    V0_TNR_CLUT225.bits.data2  = cbcr_lut[ 3][ 6];
    V0_TNR_CLUT225.bits.data3  = cbcr_lut[ 3][ 7];
    V0_TNR_CLUT226.bits.data0  = cbcr_lut[ 3][ 8];
    V0_TNR_CLUT226.bits.data1  = cbcr_lut[ 3][ 9];
    V0_TNR_CLUT226.bits.data2  = cbcr_lut[ 3][10];
    V0_TNR_CLUT226.bits.data3  = cbcr_lut[ 3][11];
    V0_TNR_CLUT227.bits.data0  = cbcr_lut[ 3][12];
    V0_TNR_CLUT227.bits.data1  = cbcr_lut[ 3][13];
    V0_TNR_CLUT227.bits.data2  = cbcr_lut[ 3][14];
    V0_TNR_CLUT227.bits.data3  = cbcr_lut[ 3][15];
    V0_TNR_CLUT228.bits.data0  = cbcr_lut[ 3][16];
    V0_TNR_CLUT228.bits.data1  = cbcr_lut[ 3][17];
    V0_TNR_CLUT228.bits.data2  = cbcr_lut[ 3][18];
    V0_TNR_CLUT228.bits.data3  = cbcr_lut[ 3][19];
    V0_TNR_CLUT229.bits.data0  = cbcr_lut[ 3][20];
    V0_TNR_CLUT229.bits.data1  = cbcr_lut[ 3][21];
    V0_TNR_CLUT229.bits.data2  = cbcr_lut[ 3][22];
    V0_TNR_CLUT229.bits.data3  = cbcr_lut[ 3][23];
    V0_TNR_CLUT230.bits.data0  = cbcr_lut[ 3][24];
    V0_TNR_CLUT230.bits.data1  = cbcr_lut[ 3][25];
    V0_TNR_CLUT230.bits.data2  = cbcr_lut[ 3][26];
    V0_TNR_CLUT230.bits.data3  = cbcr_lut[ 3][27];
    V0_TNR_CLUT231.bits.data0  = cbcr_lut[ 3][28];
    V0_TNR_CLUT231.bits.data1  = cbcr_lut[ 3][29];
    V0_TNR_CLUT231.bits.data2  = cbcr_lut[ 3][30];
    V0_TNR_CLUT231.bits.data3  = cbcr_lut[ 3][31];
    V0_TNR_CLUT232.bits.data0  = cbcr_lut[ 4][ 0];
    V0_TNR_CLUT232.bits.data1  = cbcr_lut[ 4][ 1];
    V0_TNR_CLUT232.bits.data2  = cbcr_lut[ 4][ 2];
    V0_TNR_CLUT232.bits.data3  = cbcr_lut[ 4][ 3];
    V0_TNR_CLUT233.bits.data0  = cbcr_lut[ 4][ 4];
    V0_TNR_CLUT233.bits.data1  = cbcr_lut[ 4][ 5];
    V0_TNR_CLUT233.bits.data2  = cbcr_lut[ 4][ 6];
    V0_TNR_CLUT233.bits.data3  = cbcr_lut[ 4][ 7];
    V0_TNR_CLUT234.bits.data0  = cbcr_lut[ 4][ 8];
    V0_TNR_CLUT234.bits.data1  = cbcr_lut[ 4][ 9];
    V0_TNR_CLUT234.bits.data2  = cbcr_lut[ 4][10];
    V0_TNR_CLUT234.bits.data3  = cbcr_lut[ 4][11];
    V0_TNR_CLUT235.bits.data0  = cbcr_lut[ 4][12];
    V0_TNR_CLUT235.bits.data1  = cbcr_lut[ 4][13];
    V0_TNR_CLUT235.bits.data2  = cbcr_lut[ 4][14];
    V0_TNR_CLUT235.bits.data3  = cbcr_lut[ 4][15];
    V0_TNR_CLUT236.bits.data0  = cbcr_lut[ 4][16];
    V0_TNR_CLUT236.bits.data1  = cbcr_lut[ 4][17];
    V0_TNR_CLUT236.bits.data2  = cbcr_lut[ 4][18];
    V0_TNR_CLUT236.bits.data3  = cbcr_lut[ 4][19];
    V0_TNR_CLUT237.bits.data0  = cbcr_lut[ 4][20];
    V0_TNR_CLUT237.bits.data1  = cbcr_lut[ 4][21];
    V0_TNR_CLUT237.bits.data2  = cbcr_lut[ 4][22];
    V0_TNR_CLUT237.bits.data3  = cbcr_lut[ 4][23];
    V0_TNR_CLUT238.bits.data0  = cbcr_lut[ 4][24];
    V0_TNR_CLUT238.bits.data1  = cbcr_lut[ 4][25];
    V0_TNR_CLUT238.bits.data2  = cbcr_lut[ 4][26];
    V0_TNR_CLUT238.bits.data3  = cbcr_lut[ 4][27];
    V0_TNR_CLUT239.bits.data0  = cbcr_lut[ 4][28];
    V0_TNR_CLUT239.bits.data1  = cbcr_lut[ 4][29];
    V0_TNR_CLUT239.bits.data2  = cbcr_lut[ 4][30];
    V0_TNR_CLUT239.bits.data3  = cbcr_lut[ 4][31];
    V0_TNR_CLUT240.bits.data0  = cbcr_lut[ 5][ 0];
    V0_TNR_CLUT240.bits.data1  = cbcr_lut[ 5][ 1];
    V0_TNR_CLUT240.bits.data2  = cbcr_lut[ 5][ 2];
    V0_TNR_CLUT240.bits.data3  = cbcr_lut[ 5][ 3];
    V0_TNR_CLUT241.bits.data0  = cbcr_lut[ 5][ 4];
    V0_TNR_CLUT241.bits.data1  = cbcr_lut[ 5][ 5];
    V0_TNR_CLUT241.bits.data2  = cbcr_lut[ 5][ 6];
    V0_TNR_CLUT241.bits.data3  = cbcr_lut[ 5][ 7];
    V0_TNR_CLUT242.bits.data0  = cbcr_lut[ 5][ 8];
    V0_TNR_CLUT242.bits.data1  = cbcr_lut[ 5][ 9];
    V0_TNR_CLUT242.bits.data2  = cbcr_lut[ 5][10];
    V0_TNR_CLUT242.bits.data3  = cbcr_lut[ 5][11];
    V0_TNR_CLUT243.bits.data0  = cbcr_lut[ 5][12];
    V0_TNR_CLUT243.bits.data1  = cbcr_lut[ 5][13];
    V0_TNR_CLUT243.bits.data2  = cbcr_lut[ 5][14];
    V0_TNR_CLUT243.bits.data3  = cbcr_lut[ 5][15];
    V0_TNR_CLUT244.bits.data0  = cbcr_lut[ 5][16];
    V0_TNR_CLUT244.bits.data1  = cbcr_lut[ 5][17];
    V0_TNR_CLUT244.bits.data2  = cbcr_lut[ 5][18];
    V0_TNR_CLUT244.bits.data3  = cbcr_lut[ 5][19];
    V0_TNR_CLUT245.bits.data0  = cbcr_lut[ 5][20];
    V0_TNR_CLUT245.bits.data1  = cbcr_lut[ 5][21];
    V0_TNR_CLUT245.bits.data2  = cbcr_lut[ 5][22];
    V0_TNR_CLUT245.bits.data3  = cbcr_lut[ 5][23];
    V0_TNR_CLUT246.bits.data0  = cbcr_lut[ 5][24];
    V0_TNR_CLUT246.bits.data1  = cbcr_lut[ 5][25];
    V0_TNR_CLUT246.bits.data2  = cbcr_lut[ 5][26];
    V0_TNR_CLUT246.bits.data3  = cbcr_lut[ 5][27];
    V0_TNR_CLUT247.bits.data0  = cbcr_lut[ 5][28];
    V0_TNR_CLUT247.bits.data1  = cbcr_lut[ 5][29];
    V0_TNR_CLUT247.bits.data2  = cbcr_lut[ 5][30];
    V0_TNR_CLUT247.bits.data3  = cbcr_lut[ 5][31];
    V0_TNR_CLUT248.bits.data0  = cbcr_lut[ 6][ 0];
    V0_TNR_CLUT248.bits.data1  = cbcr_lut[ 6][ 1];
    V0_TNR_CLUT248.bits.data2  = cbcr_lut[ 6][ 2];
    V0_TNR_CLUT248.bits.data3  = cbcr_lut[ 6][ 3];
    V0_TNR_CLUT249.bits.data0  = cbcr_lut[ 6][ 4];
    V0_TNR_CLUT249.bits.data1  = cbcr_lut[ 6][ 5];
    V0_TNR_CLUT249.bits.data2  = cbcr_lut[ 6][ 6];
    V0_TNR_CLUT249.bits.data3  = cbcr_lut[ 6][ 7];
    V0_TNR_CLUT250.bits.data0  = cbcr_lut[ 6][ 8];
    V0_TNR_CLUT250.bits.data1  = cbcr_lut[ 6][ 9];
    V0_TNR_CLUT250.bits.data2  = cbcr_lut[ 6][10];
    V0_TNR_CLUT250.bits.data3  = cbcr_lut[ 6][11];
    V0_TNR_CLUT251.bits.data0  = cbcr_lut[ 6][12];
    V0_TNR_CLUT251.bits.data1  = cbcr_lut[ 6][13];
    V0_TNR_CLUT251.bits.data2  = cbcr_lut[ 6][14];
    V0_TNR_CLUT251.bits.data3  = cbcr_lut[ 6][15];
    V0_TNR_CLUT252.bits.data0  = cbcr_lut[ 6][16];
    V0_TNR_CLUT252.bits.data1  = cbcr_lut[ 6][17];
    V0_TNR_CLUT252.bits.data2  = cbcr_lut[ 6][18];
    V0_TNR_CLUT252.bits.data3  = cbcr_lut[ 6][19];
    V0_TNR_CLUT253.bits.data0  = cbcr_lut[ 6][20];
    V0_TNR_CLUT253.bits.data1  = cbcr_lut[ 6][21];
    V0_TNR_CLUT253.bits.data2  = cbcr_lut[ 6][22];
    V0_TNR_CLUT253.bits.data3  = cbcr_lut[ 6][23];
    V0_TNR_CLUT254.bits.data0  = cbcr_lut[ 6][24];
    V0_TNR_CLUT254.bits.data1  = cbcr_lut[ 6][25];
    V0_TNR_CLUT254.bits.data2  = cbcr_lut[ 6][26];
    V0_TNR_CLUT254.bits.data3  = cbcr_lut[ 6][27];
    V0_TNR_CLUT255.bits.data0  = cbcr_lut[ 6][28];
    V0_TNR_CLUT255.bits.data1  = cbcr_lut[ 6][29];
    V0_TNR_CLUT255.bits.data2  = cbcr_lut[ 6][30];
    V0_TNR_CLUT255.bits.data3  = cbcr_lut[ 6][31];
    V0_TNR_CLUT256.bits.data0  = cbcr_lut[ 7][ 0];
    V0_TNR_CLUT256.bits.data1  = cbcr_lut[ 7][ 1];
    V0_TNR_CLUT256.bits.data2  = cbcr_lut[ 7][ 2];
    V0_TNR_CLUT256.bits.data3  = cbcr_lut[ 7][ 3];
    V0_TNR_CLUT257.bits.data0  = cbcr_lut[ 7][ 4];
    V0_TNR_CLUT257.bits.data1  = cbcr_lut[ 7][ 5];
    V0_TNR_CLUT257.bits.data2  = cbcr_lut[ 7][ 6];
    V0_TNR_CLUT257.bits.data3  = cbcr_lut[ 7][ 7];
    V0_TNR_CLUT258.bits.data0  = cbcr_lut[ 7][ 8];
    V0_TNR_CLUT258.bits.data1  = cbcr_lut[ 7][ 9];
    V0_TNR_CLUT258.bits.data2  = cbcr_lut[ 7][10];
    V0_TNR_CLUT258.bits.data3  = cbcr_lut[ 7][11];
    V0_TNR_CLUT259.bits.data0  = cbcr_lut[ 7][12];
    V0_TNR_CLUT259.bits.data1  = cbcr_lut[ 7][13];
    V0_TNR_CLUT259.bits.data2  = cbcr_lut[ 7][14];
    V0_TNR_CLUT259.bits.data3  = cbcr_lut[ 7][15];
    V0_TNR_CLUT260.bits.data0  = cbcr_lut[ 7][16];
    V0_TNR_CLUT260.bits.data1  = cbcr_lut[ 7][17];
    V0_TNR_CLUT260.bits.data2  = cbcr_lut[ 7][18];
    V0_TNR_CLUT260.bits.data3  = cbcr_lut[ 7][19];
    V0_TNR_CLUT261.bits.data0  = cbcr_lut[ 7][20];
    V0_TNR_CLUT261.bits.data1  = cbcr_lut[ 7][21];
    V0_TNR_CLUT261.bits.data2  = cbcr_lut[ 7][22];
    V0_TNR_CLUT261.bits.data3  = cbcr_lut[ 7][23];
    V0_TNR_CLUT262.bits.data0  = cbcr_lut[ 7][24];
    V0_TNR_CLUT262.bits.data1  = cbcr_lut[ 7][25];
    V0_TNR_CLUT262.bits.data2  = cbcr_lut[ 7][26];
    V0_TNR_CLUT262.bits.data3  = cbcr_lut[ 7][27];
    V0_TNR_CLUT263.bits.data0  = cbcr_lut[ 7][28];
    V0_TNR_CLUT263.bits.data1  = cbcr_lut[ 7][29];
    V0_TNR_CLUT263.bits.data2  = cbcr_lut[ 7][30];
    V0_TNR_CLUT263.bits.data3  = cbcr_lut[ 7][31];
    V0_TNR_CLUT264.bits.data0  = cbcr_lut[ 8][ 0];
    V0_TNR_CLUT264.bits.data1  = cbcr_lut[ 8][ 1];
    V0_TNR_CLUT264.bits.data2  = cbcr_lut[ 8][ 2];
    V0_TNR_CLUT264.bits.data3  = cbcr_lut[ 8][ 3];
    V0_TNR_CLUT265.bits.data0  = cbcr_lut[ 8][ 4];
    V0_TNR_CLUT265.bits.data1  = cbcr_lut[ 8][ 5];
    V0_TNR_CLUT265.bits.data2  = cbcr_lut[ 8][ 6];
    V0_TNR_CLUT265.bits.data3  = cbcr_lut[ 8][ 7];
    V0_TNR_CLUT266.bits.data0  = cbcr_lut[ 8][ 8];
    V0_TNR_CLUT266.bits.data1  = cbcr_lut[ 8][ 9];
    V0_TNR_CLUT266.bits.data2  = cbcr_lut[ 8][10];
    V0_TNR_CLUT266.bits.data3  = cbcr_lut[ 8][11];
    V0_TNR_CLUT267.bits.data0  = cbcr_lut[ 8][12];
    V0_TNR_CLUT267.bits.data1  = cbcr_lut[ 8][13];
    V0_TNR_CLUT267.bits.data2  = cbcr_lut[ 8][14];
    V0_TNR_CLUT267.bits.data3  = cbcr_lut[ 8][15];
    V0_TNR_CLUT268.bits.data0  = cbcr_lut[ 8][16];
    V0_TNR_CLUT268.bits.data1  = cbcr_lut[ 8][17];
    V0_TNR_CLUT268.bits.data2  = cbcr_lut[ 8][18];
    V0_TNR_CLUT268.bits.data3  = cbcr_lut[ 8][19];
    V0_TNR_CLUT269.bits.data0  = cbcr_lut[ 8][20];
    V0_TNR_CLUT269.bits.data1  = cbcr_lut[ 8][21];
    V0_TNR_CLUT269.bits.data2  = cbcr_lut[ 8][22];
    V0_TNR_CLUT269.bits.data3  = cbcr_lut[ 8][23];
    V0_TNR_CLUT270.bits.data0  = cbcr_lut[ 8][24];
    V0_TNR_CLUT270.bits.data1  = cbcr_lut[ 8][25];
    V0_TNR_CLUT270.bits.data2  = cbcr_lut[ 8][26];
    V0_TNR_CLUT270.bits.data3  = cbcr_lut[ 8][27];
    V0_TNR_CLUT271.bits.data0  = cbcr_lut[ 8][28];
    V0_TNR_CLUT271.bits.data1  = cbcr_lut[ 8][29];
    V0_TNR_CLUT271.bits.data2  = cbcr_lut[ 8][30];
    V0_TNR_CLUT271.bits.data3  = cbcr_lut[ 8][31];
    V0_TNR_CLUT272.bits.data0  = cbcr_lut[ 9][ 0];
    V0_TNR_CLUT272.bits.data1  = cbcr_lut[ 9][ 1];
    V0_TNR_CLUT272.bits.data2  = cbcr_lut[ 9][ 2];
    V0_TNR_CLUT272.bits.data3  = cbcr_lut[ 9][ 3];
    V0_TNR_CLUT273.bits.data0  = cbcr_lut[ 9][ 4];
    V0_TNR_CLUT273.bits.data1  = cbcr_lut[ 9][ 5];
    V0_TNR_CLUT273.bits.data2  = cbcr_lut[ 9][ 6];
    V0_TNR_CLUT273.bits.data3  = cbcr_lut[ 9][ 7];
    V0_TNR_CLUT274.bits.data0  = cbcr_lut[ 9][ 8];
    V0_TNR_CLUT274.bits.data1  = cbcr_lut[ 9][ 9];
    V0_TNR_CLUT274.bits.data2  = cbcr_lut[ 9][10];
    V0_TNR_CLUT274.bits.data3  = cbcr_lut[ 9][11];
    V0_TNR_CLUT275.bits.data0  = cbcr_lut[ 9][12];
    V0_TNR_CLUT275.bits.data1  = cbcr_lut[ 9][13];
    V0_TNR_CLUT275.bits.data2  = cbcr_lut[ 9][14];
    V0_TNR_CLUT275.bits.data3  = cbcr_lut[ 9][15];
    V0_TNR_CLUT276.bits.data0  = cbcr_lut[ 9][16];
    V0_TNR_CLUT276.bits.data1  = cbcr_lut[ 9][17];
    V0_TNR_CLUT276.bits.data2  = cbcr_lut[ 9][18];
    V0_TNR_CLUT276.bits.data3  = cbcr_lut[ 9][19];
    V0_TNR_CLUT277.bits.data0  = cbcr_lut[ 9][20];
    V0_TNR_CLUT277.bits.data1  = cbcr_lut[ 9][21];
    V0_TNR_CLUT277.bits.data2  = cbcr_lut[ 9][22];
    V0_TNR_CLUT277.bits.data3  = cbcr_lut[ 9][23];
    V0_TNR_CLUT278.bits.data0  = cbcr_lut[ 9][24];
    V0_TNR_CLUT278.bits.data1  = cbcr_lut[ 9][25];
    V0_TNR_CLUT278.bits.data2  = cbcr_lut[ 9][26];
    V0_TNR_CLUT278.bits.data3  = cbcr_lut[ 9][27];
    V0_TNR_CLUT279.bits.data0  = cbcr_lut[ 9][28];
    V0_TNR_CLUT279.bits.data1  = cbcr_lut[ 9][29];
    V0_TNR_CLUT279.bits.data2  = cbcr_lut[ 9][30];
    V0_TNR_CLUT279.bits.data3  = cbcr_lut[ 9][31];
    V0_TNR_CLUT280.bits.data0  = cbcr_lut[10][ 0];
    V0_TNR_CLUT280.bits.data1  = cbcr_lut[10][ 1];
    V0_TNR_CLUT280.bits.data2  = cbcr_lut[10][ 2];
    V0_TNR_CLUT280.bits.data3  = cbcr_lut[10][ 3];
    V0_TNR_CLUT281.bits.data0  = cbcr_lut[10][ 4];
    V0_TNR_CLUT281.bits.data1  = cbcr_lut[10][ 5];
    V0_TNR_CLUT281.bits.data2  = cbcr_lut[10][ 6];
    V0_TNR_CLUT281.bits.data3  = cbcr_lut[10][ 7];
    V0_TNR_CLUT282.bits.data0  = cbcr_lut[10][ 8];
    V0_TNR_CLUT282.bits.data1  = cbcr_lut[10][ 9];
    V0_TNR_CLUT282.bits.data2  = cbcr_lut[10][10];
    V0_TNR_CLUT282.bits.data3  = cbcr_lut[10][11];
    V0_TNR_CLUT283.bits.data0  = cbcr_lut[10][12];
    V0_TNR_CLUT283.bits.data1  = cbcr_lut[10][13];
    V0_TNR_CLUT283.bits.data2  = cbcr_lut[10][14];
    V0_TNR_CLUT283.bits.data3  = cbcr_lut[10][15];
    V0_TNR_CLUT284.bits.data0  = cbcr_lut[10][16];
    V0_TNR_CLUT284.bits.data1  = cbcr_lut[10][17];
    V0_TNR_CLUT284.bits.data2  = cbcr_lut[10][18];
    V0_TNR_CLUT284.bits.data3  = cbcr_lut[10][19];
    V0_TNR_CLUT285.bits.data0  = cbcr_lut[10][20];
    V0_TNR_CLUT285.bits.data1  = cbcr_lut[10][21];
    V0_TNR_CLUT285.bits.data2  = cbcr_lut[10][22];
    V0_TNR_CLUT285.bits.data3  = cbcr_lut[10][23];
    V0_TNR_CLUT286.bits.data0  = cbcr_lut[10][24];
    V0_TNR_CLUT286.bits.data1  = cbcr_lut[10][25];
    V0_TNR_CLUT286.bits.data2  = cbcr_lut[10][26];
    V0_TNR_CLUT286.bits.data3  = cbcr_lut[10][27];
    V0_TNR_CLUT287.bits.data0  = cbcr_lut[10][28];
    V0_TNR_CLUT287.bits.data1  = cbcr_lut[10][29];
    V0_TNR_CLUT287.bits.data2  = cbcr_lut[10][30];
    V0_TNR_CLUT287.bits.data3  = cbcr_lut[10][31];
    V0_TNR_CLUT288.bits.data0  = cbcr_lut[11][ 0];
    V0_TNR_CLUT288.bits.data1  = cbcr_lut[11][ 1];
    V0_TNR_CLUT288.bits.data2  = cbcr_lut[11][ 2];
    V0_TNR_CLUT288.bits.data3  = cbcr_lut[11][ 3];
    V0_TNR_CLUT289.bits.data0  = cbcr_lut[11][ 4];
    V0_TNR_CLUT289.bits.data1  = cbcr_lut[11][ 5];
    V0_TNR_CLUT289.bits.data2  = cbcr_lut[11][ 6];
    V0_TNR_CLUT289.bits.data3  = cbcr_lut[11][ 7];
    V0_TNR_CLUT290.bits.data0  = cbcr_lut[11][ 8];
    V0_TNR_CLUT290.bits.data1  = cbcr_lut[11][ 9];
    V0_TNR_CLUT290.bits.data2  = cbcr_lut[11][10];
    V0_TNR_CLUT290.bits.data3  = cbcr_lut[11][11];
    V0_TNR_CLUT291.bits.data0  = cbcr_lut[11][12];
    V0_TNR_CLUT291.bits.data1  = cbcr_lut[11][13];
    V0_TNR_CLUT291.bits.data2  = cbcr_lut[11][14];
    V0_TNR_CLUT291.bits.data3  = cbcr_lut[11][15];
    V0_TNR_CLUT292.bits.data0  = cbcr_lut[11][16];
    V0_TNR_CLUT292.bits.data1  = cbcr_lut[11][17];
    V0_TNR_CLUT292.bits.data2  = cbcr_lut[11][18];
    V0_TNR_CLUT292.bits.data3  = cbcr_lut[11][19];
    V0_TNR_CLUT293.bits.data0  = cbcr_lut[11][20];
    V0_TNR_CLUT293.bits.data1  = cbcr_lut[11][21];
    V0_TNR_CLUT293.bits.data2  = cbcr_lut[11][22];
    V0_TNR_CLUT293.bits.data3  = cbcr_lut[11][23];
    V0_TNR_CLUT294.bits.data0  = cbcr_lut[11][24];
    V0_TNR_CLUT294.bits.data1  = cbcr_lut[11][25];
    V0_TNR_CLUT294.bits.data2  = cbcr_lut[11][26];
    V0_TNR_CLUT294.bits.data3  = cbcr_lut[11][27];
    V0_TNR_CLUT295.bits.data0  = cbcr_lut[11][28];
    V0_TNR_CLUT295.bits.data1  = cbcr_lut[11][29];
    V0_TNR_CLUT295.bits.data2  = cbcr_lut[11][30];
    V0_TNR_CLUT295.bits.data3  = cbcr_lut[11][31];
    V0_TNR_CLUT296.bits.data0  = cbcr_lut[12][ 0];
    V0_TNR_CLUT296.bits.data1  = cbcr_lut[12][ 1];
    V0_TNR_CLUT296.bits.data2  = cbcr_lut[12][ 2];
    V0_TNR_CLUT296.bits.data3  = cbcr_lut[12][ 3];
    V0_TNR_CLUT297.bits.data0  = cbcr_lut[12][ 4];
    V0_TNR_CLUT297.bits.data1  = cbcr_lut[12][ 5];
    V0_TNR_CLUT297.bits.data2  = cbcr_lut[12][ 6];
    V0_TNR_CLUT297.bits.data3  = cbcr_lut[12][ 7];
    V0_TNR_CLUT298.bits.data0  = cbcr_lut[12][ 8];
    V0_TNR_CLUT298.bits.data1  = cbcr_lut[12][ 9];
    V0_TNR_CLUT298.bits.data2  = cbcr_lut[12][10];
    V0_TNR_CLUT298.bits.data3  = cbcr_lut[12][11];
    V0_TNR_CLUT299.bits.data0  = cbcr_lut[12][12];
    V0_TNR_CLUT299.bits.data1  = cbcr_lut[12][13];
    V0_TNR_CLUT299.bits.data2  = cbcr_lut[12][14];
    V0_TNR_CLUT299.bits.data3  = cbcr_lut[12][15];
    V0_TNR_CLUT2100.bits.data0 = cbcr_lut[12][16];
    V0_TNR_CLUT2100.bits.data1 = cbcr_lut[12][17];
    V0_TNR_CLUT2100.bits.data2 = cbcr_lut[12][18];
    V0_TNR_CLUT2100.bits.data3 = cbcr_lut[12][19];
    V0_TNR_CLUT2101.bits.data0 = cbcr_lut[12][20];
    V0_TNR_CLUT2101.bits.data1 = cbcr_lut[12][21];
    V0_TNR_CLUT2101.bits.data2 = cbcr_lut[12][22];
    V0_TNR_CLUT2101.bits.data3 = cbcr_lut[12][23];
    V0_TNR_CLUT2102.bits.data0 = cbcr_lut[12][24];
    V0_TNR_CLUT2102.bits.data1 = cbcr_lut[12][25];
    V0_TNR_CLUT2102.bits.data2 = cbcr_lut[12][26];
    V0_TNR_CLUT2102.bits.data3 = cbcr_lut[12][27];
    V0_TNR_CLUT2103.bits.data0 = cbcr_lut[12][28];
    V0_TNR_CLUT2103.bits.data1 = cbcr_lut[12][29];
    V0_TNR_CLUT2103.bits.data2 = cbcr_lut[12][30];
    V0_TNR_CLUT2103.bits.data3 = cbcr_lut[12][31];
    V0_TNR_CLUT2104.bits.data0 = cbcr_lut[13][ 0];
    V0_TNR_CLUT2104.bits.data1 = cbcr_lut[13][ 1];
    V0_TNR_CLUT2104.bits.data2 = cbcr_lut[13][ 2];
    V0_TNR_CLUT2104.bits.data3 = cbcr_lut[13][ 3];
    V0_TNR_CLUT2105.bits.data0 = cbcr_lut[13][ 4];
    V0_TNR_CLUT2105.bits.data1 = cbcr_lut[13][ 5];
    V0_TNR_CLUT2105.bits.data2 = cbcr_lut[13][ 6];
    V0_TNR_CLUT2105.bits.data3 = cbcr_lut[13][ 7];
    V0_TNR_CLUT2106.bits.data0 = cbcr_lut[13][ 8];
    V0_TNR_CLUT2106.bits.data1 = cbcr_lut[13][ 9];
    V0_TNR_CLUT2106.bits.data2 = cbcr_lut[13][10];
    V0_TNR_CLUT2106.bits.data3 = cbcr_lut[13][11];
    V0_TNR_CLUT2107.bits.data0 = cbcr_lut[13][12];
    V0_TNR_CLUT2107.bits.data1 = cbcr_lut[13][13];
    V0_TNR_CLUT2107.bits.data2 = cbcr_lut[13][14];
    V0_TNR_CLUT2107.bits.data3 = cbcr_lut[13][15];
    V0_TNR_CLUT2108.bits.data0 = cbcr_lut[13][16];
    V0_TNR_CLUT2108.bits.data1 = cbcr_lut[13][17];
    V0_TNR_CLUT2108.bits.data2 = cbcr_lut[13][18];
    V0_TNR_CLUT2108.bits.data3 = cbcr_lut[13][19];
    V0_TNR_CLUT2109.bits.data0 = cbcr_lut[13][20];
    V0_TNR_CLUT2109.bits.data1 = cbcr_lut[13][21];
    V0_TNR_CLUT2109.bits.data2 = cbcr_lut[13][22];
    V0_TNR_CLUT2109.bits.data3 = cbcr_lut[13][23];
    V0_TNR_CLUT2110.bits.data0 = cbcr_lut[13][24];
    V0_TNR_CLUT2110.bits.data1 = cbcr_lut[13][25];
    V0_TNR_CLUT2110.bits.data2 = cbcr_lut[13][26];
    V0_TNR_CLUT2110.bits.data3 = cbcr_lut[13][27];
    V0_TNR_CLUT2111.bits.data0 = cbcr_lut[13][28];
    V0_TNR_CLUT2111.bits.data1 = cbcr_lut[13][29];
    V0_TNR_CLUT2111.bits.data2 = cbcr_lut[13][30];
    V0_TNR_CLUT2111.bits.data3 = cbcr_lut[13][31];
    V0_TNR_CLUT2112.bits.data0 = cbcr_lut[14][ 0];
    V0_TNR_CLUT2112.bits.data1 = cbcr_lut[14][ 1];
    V0_TNR_CLUT2112.bits.data2 = cbcr_lut[14][ 2];
    V0_TNR_CLUT2112.bits.data3 = cbcr_lut[14][ 3];
    V0_TNR_CLUT2113.bits.data0 = cbcr_lut[14][ 4];
    V0_TNR_CLUT2113.bits.data1 = cbcr_lut[14][ 5];
    V0_TNR_CLUT2113.bits.data2 = cbcr_lut[14][ 6];
    V0_TNR_CLUT2113.bits.data3 = cbcr_lut[14][ 7];
    V0_TNR_CLUT2114.bits.data0 = cbcr_lut[14][ 8];
    V0_TNR_CLUT2114.bits.data1 = cbcr_lut[14][ 9];
    V0_TNR_CLUT2114.bits.data2 = cbcr_lut[14][10];
    V0_TNR_CLUT2114.bits.data3 = cbcr_lut[14][11];
    V0_TNR_CLUT2115.bits.data0 = cbcr_lut[14][12];
    V0_TNR_CLUT2115.bits.data1 = cbcr_lut[14][13];
    V0_TNR_CLUT2115.bits.data2 = cbcr_lut[14][14];
    V0_TNR_CLUT2115.bits.data3 = cbcr_lut[14][15];
    V0_TNR_CLUT2116.bits.data0 = cbcr_lut[14][16];
    V0_TNR_CLUT2116.bits.data1 = cbcr_lut[14][17];
    V0_TNR_CLUT2116.bits.data2 = cbcr_lut[14][18];
    V0_TNR_CLUT2116.bits.data3 = cbcr_lut[14][19];
    V0_TNR_CLUT2117.bits.data0 = cbcr_lut[14][20];
    V0_TNR_CLUT2117.bits.data1 = cbcr_lut[14][21];
    V0_TNR_CLUT2117.bits.data2 = cbcr_lut[14][22];
    V0_TNR_CLUT2117.bits.data3 = cbcr_lut[14][23];
    V0_TNR_CLUT2118.bits.data0 = cbcr_lut[14][24];
    V0_TNR_CLUT2118.bits.data1 = cbcr_lut[14][25];
    V0_TNR_CLUT2118.bits.data2 = cbcr_lut[14][26];
    V0_TNR_CLUT2118.bits.data3 = cbcr_lut[14][27];
    V0_TNR_CLUT2119.bits.data0 = cbcr_lut[14][28];
    V0_TNR_CLUT2119.bits.data1 = cbcr_lut[14][29];
    V0_TNR_CLUT2119.bits.data2 = cbcr_lut[14][30];
    V0_TNR_CLUT2119.bits.data3 = cbcr_lut[14][31];
    V0_TNR_CLUT2120.bits.data0 = cbcr_lut[15][ 0];
    V0_TNR_CLUT2120.bits.data1 = cbcr_lut[15][ 1];
    V0_TNR_CLUT2120.bits.data2 = cbcr_lut[15][ 2];
    V0_TNR_CLUT2120.bits.data3 = cbcr_lut[15][ 3];
    V0_TNR_CLUT2121.bits.data0 = cbcr_lut[15][ 4];
    V0_TNR_CLUT2121.bits.data1 = cbcr_lut[15][ 5];
    V0_TNR_CLUT2121.bits.data2 = cbcr_lut[15][ 6];
    V0_TNR_CLUT2121.bits.data3 = cbcr_lut[15][ 7];
    V0_TNR_CLUT2122.bits.data0 = cbcr_lut[15][ 8];
    V0_TNR_CLUT2122.bits.data1 = cbcr_lut[15][ 9];
    V0_TNR_CLUT2122.bits.data2 = cbcr_lut[15][10];
    V0_TNR_CLUT2122.bits.data3 = cbcr_lut[15][11];
    V0_TNR_CLUT2123.bits.data0 = cbcr_lut[15][12];
    V0_TNR_CLUT2123.bits.data1 = cbcr_lut[15][13];
    V0_TNR_CLUT2123.bits.data2 = cbcr_lut[15][14];
    V0_TNR_CLUT2123.bits.data3 = cbcr_lut[15][15];
    V0_TNR_CLUT2124.bits.data0 = cbcr_lut[15][16];
    V0_TNR_CLUT2124.bits.data1 = cbcr_lut[15][17];
    V0_TNR_CLUT2124.bits.data2 = cbcr_lut[15][18];
    V0_TNR_CLUT2124.bits.data3 = cbcr_lut[15][19];
    V0_TNR_CLUT2125.bits.data0 = cbcr_lut[15][20];
    V0_TNR_CLUT2125.bits.data1 = cbcr_lut[15][21];
    V0_TNR_CLUT2125.bits.data2 = cbcr_lut[15][22];
    V0_TNR_CLUT2125.bits.data3 = cbcr_lut[15][23];
    V0_TNR_CLUT2126.bits.data0 = cbcr_lut[15][24];
    V0_TNR_CLUT2126.bits.data1 = cbcr_lut[15][25];
    V0_TNR_CLUT2126.bits.data2 = cbcr_lut[15][26];
    V0_TNR_CLUT2126.bits.data3 = cbcr_lut[15][27];
    V0_TNR_CLUT2127.bits.data0 = cbcr_lut[15][28];
    V0_TNR_CLUT2127.bits.data1 = cbcr_lut[15][29];
    V0_TNR_CLUT2127.bits.data2 = cbcr_lut[15][30];
    V0_TNR_CLUT2127.bits.data3 = cbcr_lut[15][31];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT20.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT20.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT21.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT21.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT22.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT22.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT23.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT23.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT24.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT24.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT25.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT25.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT26.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT26.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT27.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT27.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT28.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT28.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT29.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT29.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT210.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT210.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT211.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT211.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT212.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT212.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT213.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT213.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT214.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT214.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT215.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT215.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT216.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT216.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT217.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT217.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT218.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT218.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT219.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT219.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT220.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT220.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT221.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT221.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT222.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT222.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT223.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT223.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT224.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT224.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT225.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT225.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT226.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT226.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT227.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT227.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT228.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT228.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT229.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT229.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT230.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT230.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT231.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT231.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT232.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT232.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT233.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT233.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT234.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT234.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT235.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT235.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT236.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT236.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT237.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT237.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT238.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT238.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT239.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT239.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT240.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT240.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT241.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT241.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT242.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT242.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT243.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT243.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT244.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT244.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT245.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT245.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT246.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT246.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT247.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT247.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT248.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT248.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT249.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT249.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT250.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT250.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT251.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT251.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT252.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT252.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT253.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT253.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT254.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT254.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT255.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT255.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT256.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT256.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT257.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT257.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT258.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT258.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT259.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT259.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT260.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT260.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT261.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT261.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT262.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT262.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT263.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT263.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT264.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT264.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT265.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT265.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT266.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT266.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT267.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT267.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT268.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT268.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT269.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT269.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT270.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT270.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT271.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT271.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT272.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT272.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT273.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT273.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT274.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT274.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT275.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT275.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT276.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT276.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT277.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT277.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT278.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT278.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT279.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT279.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT280.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT280.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT281.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT281.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT282.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT282.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT283.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT283.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT284.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT284.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT285.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT285.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT286.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT286.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT287.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT287.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT288.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT288.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT289.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT289.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT290.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT290.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT291.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT291.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT292.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT292.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT293.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT293.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT294.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT294.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT295.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT295.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT296.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT296.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT297.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT297.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT298.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT298.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT299.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT299.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2100.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2100.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2101.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2101.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2102.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2102.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2103.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2103.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2104.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2104.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2105.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2105.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2106.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2106.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2107.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2107.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2108.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2108.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2109.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2109.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2110.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2110.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2111.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2111.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2112.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2112.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2113.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2113.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2114.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2114.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2115.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2115.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2116.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2116.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2117.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2117.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2118.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2118.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2119.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2119.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2120.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2120.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2121.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2121.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2122.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2122.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2123.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2123.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2124.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2124.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2125.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2125.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2126.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2126.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT2127.u32) + u32Data * VID_OFFSET), V0_TNR_CLUT2127.u32);
    return;
}

HI_VOID VDP_VID_SetTNRMotionEdgeLut(HI_U32 u32Data,HI_U32 motion_edge_lut[16][8])
{
    U_V0_TNR_CLUT30  V0_TNR_CLUT30 ;
    U_V0_TNR_CLUT31  V0_TNR_CLUT31 ;
    U_V0_TNR_CLUT32  V0_TNR_CLUT32 ;
    U_V0_TNR_CLUT33  V0_TNR_CLUT33 ;
    U_V0_TNR_CLUT34  V0_TNR_CLUT34 ;
    U_V0_TNR_CLUT35  V0_TNR_CLUT35 ;
    U_V0_TNR_CLUT36  V0_TNR_CLUT36 ;
    U_V0_TNR_CLUT37  V0_TNR_CLUT37 ;
    U_V0_TNR_CLUT38  V0_TNR_CLUT38 ;
    U_V0_TNR_CLUT39  V0_TNR_CLUT39 ;
    U_V0_TNR_CLUT310 V0_TNR_CLUT310;
    U_V0_TNR_CLUT311 V0_TNR_CLUT311;
    U_V0_TNR_CLUT312 V0_TNR_CLUT312;
    U_V0_TNR_CLUT313 V0_TNR_CLUT313;
    U_V0_TNR_CLUT314 V0_TNR_CLUT314;
    U_V0_TNR_CLUT315 V0_TNR_CLUT315;
    U_V0_TNR_CLUT316 V0_TNR_CLUT316;
    U_V0_TNR_CLUT317 V0_TNR_CLUT317;
    U_V0_TNR_CLUT318 V0_TNR_CLUT318;
    U_V0_TNR_CLUT319 V0_TNR_CLUT319;
    U_V0_TNR_CLUT320 V0_TNR_CLUT320;
    U_V0_TNR_CLUT321 V0_TNR_CLUT321;
    U_V0_TNR_CLUT322 V0_TNR_CLUT322;
    U_V0_TNR_CLUT323 V0_TNR_CLUT323;
    U_V0_TNR_CLUT324 V0_TNR_CLUT324;
    U_V0_TNR_CLUT325 V0_TNR_CLUT325;
    U_V0_TNR_CLUT326 V0_TNR_CLUT326;
    U_V0_TNR_CLUT327 V0_TNR_CLUT327;
    U_V0_TNR_CLUT328 V0_TNR_CLUT328;
    U_V0_TNR_CLUT329 V0_TNR_CLUT329;
    U_V0_TNR_CLUT330 V0_TNR_CLUT330;
    U_V0_TNR_CLUT331 V0_TNR_CLUT331;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRMotionEdgeLut() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_CLUT30.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT30.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT31.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT31.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT32.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT32.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT33.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT33.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT34.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT34.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT35.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT35.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT36.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT36.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT37.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT37.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT38.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT38.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT39.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT39.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT310.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT310.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT311.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT311.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT312.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT312.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT313.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT313.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT314.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT314.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT315.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT315.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT316.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT316.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT317.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT317.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT318.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT318.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT319.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT319.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT320.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT320.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT321.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT321.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT322.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT322.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT323.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT323.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT324.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT324.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT325.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT325.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT326.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT326.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT327.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT327.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT328.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT328.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT329.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT329.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT330.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT330.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT331.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT331.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT30.bits.data0   = motion_edge_lut[ 0][0];
    V0_TNR_CLUT30.bits.data1   = motion_edge_lut[ 0][1];
    V0_TNR_CLUT30.bits.data2   = motion_edge_lut[ 0][2];
    V0_TNR_CLUT30.bits.data3   = motion_edge_lut[ 0][3];
    V0_TNR_CLUT31.bits.data0   = motion_edge_lut[ 0][4];
    V0_TNR_CLUT31.bits.data1   = motion_edge_lut[ 0][5];
    V0_TNR_CLUT31.bits.data2   = motion_edge_lut[ 0][6];
    V0_TNR_CLUT31.bits.data3   = motion_edge_lut[ 0][7];
    V0_TNR_CLUT32.bits.data0   = motion_edge_lut[ 1][0];
    V0_TNR_CLUT32.bits.data1   = motion_edge_lut[ 1][1];
    V0_TNR_CLUT32.bits.data2   = motion_edge_lut[ 1][2];
    V0_TNR_CLUT32.bits.data3   = motion_edge_lut[ 1][3];
    V0_TNR_CLUT33.bits.data0   = motion_edge_lut[ 1][4];
    V0_TNR_CLUT33.bits.data1   = motion_edge_lut[ 1][5];
    V0_TNR_CLUT33.bits.data2   = motion_edge_lut[ 1][6];
    V0_TNR_CLUT33.bits.data3   = motion_edge_lut[ 1][7];
    V0_TNR_CLUT34.bits.data0   = motion_edge_lut[ 2][0];
    V0_TNR_CLUT34.bits.data1   = motion_edge_lut[ 2][1];
    V0_TNR_CLUT34.bits.data2   = motion_edge_lut[ 2][2];
    V0_TNR_CLUT34.bits.data3   = motion_edge_lut[ 2][3];
    V0_TNR_CLUT35.bits.data0   = motion_edge_lut[ 2][4];
    V0_TNR_CLUT35.bits.data1   = motion_edge_lut[ 2][5];
    V0_TNR_CLUT35.bits.data2   = motion_edge_lut[ 2][6];
    V0_TNR_CLUT35.bits.data3   = motion_edge_lut[ 2][7];
    V0_TNR_CLUT36.bits.data0   = motion_edge_lut[ 3][0];
    V0_TNR_CLUT36.bits.data1   = motion_edge_lut[ 3][1];
    V0_TNR_CLUT36.bits.data2   = motion_edge_lut[ 3][2];
    V0_TNR_CLUT36.bits.data3   = motion_edge_lut[ 3][3];
    V0_TNR_CLUT37.bits.data0   = motion_edge_lut[ 3][4];
    V0_TNR_CLUT37.bits.data1   = motion_edge_lut[ 3][5];
    V0_TNR_CLUT37.bits.data2   = motion_edge_lut[ 3][6];
    V0_TNR_CLUT37.bits.data3   = motion_edge_lut[ 3][7];
    V0_TNR_CLUT38.bits.data0   = motion_edge_lut[ 4][0];
    V0_TNR_CLUT38.bits.data1   = motion_edge_lut[ 4][1];
    V0_TNR_CLUT38.bits.data2   = motion_edge_lut[ 4][2];
    V0_TNR_CLUT38.bits.data3   = motion_edge_lut[ 4][3];
    V0_TNR_CLUT39.bits.data0   = motion_edge_lut[ 4][4];
    V0_TNR_CLUT39.bits.data1   = motion_edge_lut[ 4][5];
    V0_TNR_CLUT39.bits.data2   = motion_edge_lut[ 4][6];
    V0_TNR_CLUT39.bits.data3   = motion_edge_lut[ 4][7];
    V0_TNR_CLUT310.bits.data0  = motion_edge_lut[ 5][0];
    V0_TNR_CLUT310.bits.data1  = motion_edge_lut[ 5][1];
    V0_TNR_CLUT310.bits.data2  = motion_edge_lut[ 5][2];
    V0_TNR_CLUT310.bits.data3  = motion_edge_lut[ 5][3];
    V0_TNR_CLUT311.bits.data0  = motion_edge_lut[ 5][4];
    V0_TNR_CLUT311.bits.data1  = motion_edge_lut[ 5][5];
    V0_TNR_CLUT311.bits.data2  = motion_edge_lut[ 5][6];
    V0_TNR_CLUT311.bits.data3  = motion_edge_lut[ 5][7];
    V0_TNR_CLUT312.bits.data0  = motion_edge_lut[ 6][0];
    V0_TNR_CLUT312.bits.data1  = motion_edge_lut[ 6][1];
    V0_TNR_CLUT312.bits.data2  = motion_edge_lut[ 6][2];
    V0_TNR_CLUT312.bits.data3  = motion_edge_lut[ 6][3];
    V0_TNR_CLUT313.bits.data0  = motion_edge_lut[ 6][4];
    V0_TNR_CLUT313.bits.data1  = motion_edge_lut[ 6][5];
    V0_TNR_CLUT313.bits.data2  = motion_edge_lut[ 6][6];
    V0_TNR_CLUT313.bits.data3  = motion_edge_lut[ 6][7];
    V0_TNR_CLUT314.bits.data0  = motion_edge_lut[ 7][0];
    V0_TNR_CLUT314.bits.data1  = motion_edge_lut[ 7][1];
    V0_TNR_CLUT314.bits.data2  = motion_edge_lut[ 7][2];
    V0_TNR_CLUT314.bits.data3  = motion_edge_lut[ 7][3];
    V0_TNR_CLUT315.bits.data0  = motion_edge_lut[ 7][4];
    V0_TNR_CLUT315.bits.data1  = motion_edge_lut[ 7][5];
    V0_TNR_CLUT315.bits.data2  = motion_edge_lut[ 7][6];
    V0_TNR_CLUT315.bits.data3  = motion_edge_lut[ 7][7];
    V0_TNR_CLUT316.bits.data0  = motion_edge_lut[ 8][0];
    V0_TNR_CLUT316.bits.data1  = motion_edge_lut[ 8][1];
    V0_TNR_CLUT316.bits.data2  = motion_edge_lut[ 8][2];
    V0_TNR_CLUT316.bits.data3  = motion_edge_lut[ 8][3];
    V0_TNR_CLUT317.bits.data0  = motion_edge_lut[ 8][4];
    V0_TNR_CLUT317.bits.data1  = motion_edge_lut[ 8][5];
    V0_TNR_CLUT317.bits.data2  = motion_edge_lut[ 8][6];
    V0_TNR_CLUT317.bits.data3  = motion_edge_lut[ 8][7];
    V0_TNR_CLUT318.bits.data0  = motion_edge_lut[ 9][0];
    V0_TNR_CLUT318.bits.data1  = motion_edge_lut[ 9][1];
    V0_TNR_CLUT318.bits.data2  = motion_edge_lut[ 9][2];
    V0_TNR_CLUT318.bits.data3  = motion_edge_lut[ 9][3];
    V0_TNR_CLUT319.bits.data0  = motion_edge_lut[ 9][4];
    V0_TNR_CLUT319.bits.data1  = motion_edge_lut[ 9][5];
    V0_TNR_CLUT319.bits.data2  = motion_edge_lut[ 9][6];
    V0_TNR_CLUT319.bits.data3  = motion_edge_lut[ 9][7];
    V0_TNR_CLUT320.bits.data0  = motion_edge_lut[10][0];
    V0_TNR_CLUT320.bits.data1  = motion_edge_lut[10][1];
    V0_TNR_CLUT320.bits.data2  = motion_edge_lut[10][2];
    V0_TNR_CLUT320.bits.data3  = motion_edge_lut[10][3];
    V0_TNR_CLUT321.bits.data0  = motion_edge_lut[10][4];
    V0_TNR_CLUT321.bits.data1  = motion_edge_lut[10][5];
    V0_TNR_CLUT321.bits.data2  = motion_edge_lut[10][6];
    V0_TNR_CLUT321.bits.data3  = motion_edge_lut[10][7];
    V0_TNR_CLUT322.bits.data0  = motion_edge_lut[11][0];
    V0_TNR_CLUT322.bits.data1  = motion_edge_lut[11][1];
    V0_TNR_CLUT322.bits.data2  = motion_edge_lut[11][2];
    V0_TNR_CLUT322.bits.data3  = motion_edge_lut[11][3];
    V0_TNR_CLUT323.bits.data0  = motion_edge_lut[11][4];
    V0_TNR_CLUT323.bits.data1  = motion_edge_lut[11][5];
    V0_TNR_CLUT323.bits.data2  = motion_edge_lut[11][6];
    V0_TNR_CLUT323.bits.data3  = motion_edge_lut[11][7];
    V0_TNR_CLUT324.bits.data0  = motion_edge_lut[12][0];
    V0_TNR_CLUT324.bits.data1  = motion_edge_lut[12][1];
    V0_TNR_CLUT324.bits.data2  = motion_edge_lut[12][2];
    V0_TNR_CLUT324.bits.data3  = motion_edge_lut[12][3];
    V0_TNR_CLUT325.bits.data0  = motion_edge_lut[12][4];
    V0_TNR_CLUT325.bits.data1  = motion_edge_lut[12][5];
    V0_TNR_CLUT325.bits.data2  = motion_edge_lut[12][6];
    V0_TNR_CLUT325.bits.data3  = motion_edge_lut[12][7];
    V0_TNR_CLUT326.bits.data0  = motion_edge_lut[13][0];
    V0_TNR_CLUT326.bits.data1  = motion_edge_lut[13][1];
    V0_TNR_CLUT326.bits.data2  = motion_edge_lut[13][2];
    V0_TNR_CLUT326.bits.data3  = motion_edge_lut[13][3];
    V0_TNR_CLUT327.bits.data0  = motion_edge_lut[13][4];
    V0_TNR_CLUT327.bits.data1  = motion_edge_lut[13][5];
    V0_TNR_CLUT327.bits.data2  = motion_edge_lut[13][6];
    V0_TNR_CLUT327.bits.data3  = motion_edge_lut[13][7];
    V0_TNR_CLUT328.bits.data0  = motion_edge_lut[14][0];
    V0_TNR_CLUT328.bits.data1  = motion_edge_lut[14][1];
    V0_TNR_CLUT328.bits.data2  = motion_edge_lut[14][2];
    V0_TNR_CLUT328.bits.data3  = motion_edge_lut[14][3];
    V0_TNR_CLUT329.bits.data0  = motion_edge_lut[14][4];
    V0_TNR_CLUT329.bits.data1  = motion_edge_lut[14][5];
    V0_TNR_CLUT329.bits.data2  = motion_edge_lut[14][6];
    V0_TNR_CLUT329.bits.data3  = motion_edge_lut[14][7];
    V0_TNR_CLUT330.bits.data0  = motion_edge_lut[15][0];
    V0_TNR_CLUT330.bits.data1  = motion_edge_lut[15][1];
    V0_TNR_CLUT330.bits.data2  = motion_edge_lut[15][2];
    V0_TNR_CLUT330.bits.data3  = motion_edge_lut[15][3];
    V0_TNR_CLUT331.bits.data0  = motion_edge_lut[15][4];
    V0_TNR_CLUT331.bits.data1  = motion_edge_lut[15][5];
    V0_TNR_CLUT331.bits.data2  = motion_edge_lut[15][6];
    V0_TNR_CLUT331.bits.data3  = motion_edge_lut[15][7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT30.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT30.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT31.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT31.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT32.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT32.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT33.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT33.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT34.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT34.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT35.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT35.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT36.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT36.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT37.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT37.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT38.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT38.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT39.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT39.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT310.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT310.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT311.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT311.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT312.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT312.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT313.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT313.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT314.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT314.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT315.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT315.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT316.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT316.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT317.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT317.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT318.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT318.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT319.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT319.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT320.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT320.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT321.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT321.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT322.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT322.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT323.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT323.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT324.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT324.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT325.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT325.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT326.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT326.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT327.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT327.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT328.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT328.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT329.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT329.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT330.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT330.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT331.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT331.u32);
    return;
}

HI_VOID VDP_VID_SetTNRMotionNoiseLut(HI_U32 u32Data,HI_U32 motion_noise_lut[16][8])
{
    U_V0_TNR_CLUT40  V0_TNR_CLUT40 ;
    U_V0_TNR_CLUT41  V0_TNR_CLUT41 ;
    U_V0_TNR_CLUT42  V0_TNR_CLUT42 ;
    U_V0_TNR_CLUT43  V0_TNR_CLUT43 ;
    U_V0_TNR_CLUT44  V0_TNR_CLUT44 ;
    U_V0_TNR_CLUT45  V0_TNR_CLUT45 ;
    U_V0_TNR_CLUT46  V0_TNR_CLUT46 ;
    U_V0_TNR_CLUT47  V0_TNR_CLUT47 ;
    U_V0_TNR_CLUT48  V0_TNR_CLUT48 ;
    U_V0_TNR_CLUT49  V0_TNR_CLUT49 ;
    U_V0_TNR_CLUT410 V0_TNR_CLUT410;
    U_V0_TNR_CLUT411 V0_TNR_CLUT411;
    U_V0_TNR_CLUT412 V0_TNR_CLUT412;
    U_V0_TNR_CLUT413 V0_TNR_CLUT413;
    U_V0_TNR_CLUT414 V0_TNR_CLUT414;
    U_V0_TNR_CLUT415 V0_TNR_CLUT415;
    U_V0_TNR_CLUT416 V0_TNR_CLUT416;
    U_V0_TNR_CLUT417 V0_TNR_CLUT417;
    U_V0_TNR_CLUT418 V0_TNR_CLUT418;
    U_V0_TNR_CLUT419 V0_TNR_CLUT419;
    U_V0_TNR_CLUT420 V0_TNR_CLUT420;
    U_V0_TNR_CLUT421 V0_TNR_CLUT421;
    U_V0_TNR_CLUT422 V0_TNR_CLUT422;
    U_V0_TNR_CLUT423 V0_TNR_CLUT423;
    U_V0_TNR_CLUT424 V0_TNR_CLUT424;
    U_V0_TNR_CLUT425 V0_TNR_CLUT425;
    U_V0_TNR_CLUT426 V0_TNR_CLUT426;
    U_V0_TNR_CLUT427 V0_TNR_CLUT427;
    U_V0_TNR_CLUT428 V0_TNR_CLUT428;
    U_V0_TNR_CLUT429 V0_TNR_CLUT429;
    U_V0_TNR_CLUT430 V0_TNR_CLUT430;
    U_V0_TNR_CLUT431 V0_TNR_CLUT431;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTNRMotionNoiseLut() select wrong video layer ID\n");
        return ;
    }

    V0_TNR_CLUT40.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT40.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT41.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT41.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT42.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT42.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT43.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT43.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT44.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT44.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT45.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT45.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT46.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT46.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT47.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT47.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT48.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT48.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT49.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT49.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT410.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT410.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT411.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT411.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT412.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT412.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT413.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT413.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT414.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT414.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT415.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT415.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT416.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT416.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT417.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT417.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT418.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT418.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT419.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT419.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT420.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT420.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT421.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT421.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT422.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT422.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT423.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT423.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT424.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT424.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT425.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT425.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT426.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT426.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT427.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT427.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT428.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT428.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT429.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT429.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT430.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT430.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT431.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT431.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT40.bits.data0   = motion_noise_lut[ 0][0];
    V0_TNR_CLUT40.bits.data1   = motion_noise_lut[ 0][1];
    V0_TNR_CLUT40.bits.data2   = motion_noise_lut[ 0][2];
    V0_TNR_CLUT40.bits.data3   = motion_noise_lut[ 0][3];
    V0_TNR_CLUT41.bits.data0   = motion_noise_lut[ 0][4];
    V0_TNR_CLUT41.bits.data1   = motion_noise_lut[ 0][5];
    V0_TNR_CLUT41.bits.data2   = motion_noise_lut[ 0][6];
    V0_TNR_CLUT41.bits.data3   = motion_noise_lut[ 0][7];
    V0_TNR_CLUT42.bits.data0   = motion_noise_lut[ 1][0];
    V0_TNR_CLUT42.bits.data1   = motion_noise_lut[ 1][1];
    V0_TNR_CLUT42.bits.data2   = motion_noise_lut[ 1][2];
    V0_TNR_CLUT42.bits.data3   = motion_noise_lut[ 1][3];
    V0_TNR_CLUT43.bits.data0   = motion_noise_lut[ 1][4];
    V0_TNR_CLUT43.bits.data1   = motion_noise_lut[ 1][5];
    V0_TNR_CLUT43.bits.data2   = motion_noise_lut[ 1][6];
    V0_TNR_CLUT43.bits.data3   = motion_noise_lut[ 1][7];
    V0_TNR_CLUT44.bits.data0   = motion_noise_lut[ 2][0];
    V0_TNR_CLUT44.bits.data1   = motion_noise_lut[ 2][1];
    V0_TNR_CLUT44.bits.data2   = motion_noise_lut[ 2][2];
    V0_TNR_CLUT44.bits.data3   = motion_noise_lut[ 2][3];
    V0_TNR_CLUT45.bits.data0   = motion_noise_lut[ 2][4];
    V0_TNR_CLUT45.bits.data1   = motion_noise_lut[ 2][5];
    V0_TNR_CLUT45.bits.data2   = motion_noise_lut[ 2][6];
    V0_TNR_CLUT45.bits.data3   = motion_noise_lut[ 2][7];
    V0_TNR_CLUT46.bits.data0   = motion_noise_lut[ 3][0];
    V0_TNR_CLUT46.bits.data1   = motion_noise_lut[ 3][1];
    V0_TNR_CLUT46.bits.data2   = motion_noise_lut[ 3][2];
    V0_TNR_CLUT46.bits.data3   = motion_noise_lut[ 3][3];
    V0_TNR_CLUT47.bits.data0   = motion_noise_lut[ 3][4];
    V0_TNR_CLUT47.bits.data1   = motion_noise_lut[ 3][5];
    V0_TNR_CLUT47.bits.data2   = motion_noise_lut[ 3][6];
    V0_TNR_CLUT47.bits.data3   = motion_noise_lut[ 3][7];
    V0_TNR_CLUT48.bits.data0   = motion_noise_lut[ 4][0];
    V0_TNR_CLUT48.bits.data1   = motion_noise_lut[ 4][1];
    V0_TNR_CLUT48.bits.data2   = motion_noise_lut[ 4][2];
    V0_TNR_CLUT48.bits.data3   = motion_noise_lut[ 4][3];
    V0_TNR_CLUT49.bits.data0   = motion_noise_lut[ 4][4];
    V0_TNR_CLUT49.bits.data1   = motion_noise_lut[ 4][5];
    V0_TNR_CLUT49.bits.data2   = motion_noise_lut[ 4][6];
    V0_TNR_CLUT49.bits.data3   = motion_noise_lut[ 4][7];
    V0_TNR_CLUT410.bits.data0  = motion_noise_lut[ 5][0];
    V0_TNR_CLUT410.bits.data1  = motion_noise_lut[ 5][1];
    V0_TNR_CLUT410.bits.data2  = motion_noise_lut[ 5][2];
    V0_TNR_CLUT410.bits.data3  = motion_noise_lut[ 5][3];
    V0_TNR_CLUT411.bits.data0  = motion_noise_lut[ 5][4];
    V0_TNR_CLUT411.bits.data1  = motion_noise_lut[ 5][5];
    V0_TNR_CLUT411.bits.data2  = motion_noise_lut[ 5][6];
    V0_TNR_CLUT411.bits.data3  = motion_noise_lut[ 5][7];
    V0_TNR_CLUT412.bits.data0  = motion_noise_lut[ 6][0];
    V0_TNR_CLUT412.bits.data1  = motion_noise_lut[ 6][1];
    V0_TNR_CLUT412.bits.data2  = motion_noise_lut[ 6][2];
    V0_TNR_CLUT412.bits.data3  = motion_noise_lut[ 6][3];
    V0_TNR_CLUT413.bits.data0  = motion_noise_lut[ 6][4];
    V0_TNR_CLUT413.bits.data1  = motion_noise_lut[ 6][5];
    V0_TNR_CLUT413.bits.data2  = motion_noise_lut[ 6][6];
    V0_TNR_CLUT413.bits.data3  = motion_noise_lut[ 6][7];
    V0_TNR_CLUT414.bits.data0  = motion_noise_lut[ 7][0];
    V0_TNR_CLUT414.bits.data1  = motion_noise_lut[ 7][1];
    V0_TNR_CLUT414.bits.data2  = motion_noise_lut[ 7][2];
    V0_TNR_CLUT414.bits.data3  = motion_noise_lut[ 7][3];
    V0_TNR_CLUT415.bits.data0  = motion_noise_lut[ 7][4];
    V0_TNR_CLUT415.bits.data1  = motion_noise_lut[ 7][5];
    V0_TNR_CLUT415.bits.data2  = motion_noise_lut[ 7][6];
    V0_TNR_CLUT415.bits.data3  = motion_noise_lut[ 7][7];
    V0_TNR_CLUT416.bits.data0  = motion_noise_lut[ 8][0];
    V0_TNR_CLUT416.bits.data1  = motion_noise_lut[ 8][1];
    V0_TNR_CLUT416.bits.data2  = motion_noise_lut[ 8][2];
    V0_TNR_CLUT416.bits.data3  = motion_noise_lut[ 8][3];
    V0_TNR_CLUT417.bits.data0  = motion_noise_lut[ 8][4];
    V0_TNR_CLUT417.bits.data1  = motion_noise_lut[ 8][5];
    V0_TNR_CLUT417.bits.data2  = motion_noise_lut[ 8][6];
    V0_TNR_CLUT417.bits.data3  = motion_noise_lut[ 8][7];
    V0_TNR_CLUT418.bits.data0  = motion_noise_lut[ 9][0];
    V0_TNR_CLUT418.bits.data1  = motion_noise_lut[ 9][1];
    V0_TNR_CLUT418.bits.data2  = motion_noise_lut[ 9][2];
    V0_TNR_CLUT418.bits.data3  = motion_noise_lut[ 9][3];
    V0_TNR_CLUT419.bits.data0  = motion_noise_lut[ 9][4];
    V0_TNR_CLUT419.bits.data1  = motion_noise_lut[ 9][5];
    V0_TNR_CLUT419.bits.data2  = motion_noise_lut[ 9][6];
    V0_TNR_CLUT419.bits.data3  = motion_noise_lut[ 9][7];
    V0_TNR_CLUT420.bits.data0  = motion_noise_lut[10][0];
    V0_TNR_CLUT420.bits.data1  = motion_noise_lut[10][1];
    V0_TNR_CLUT420.bits.data2  = motion_noise_lut[10][2];
    V0_TNR_CLUT420.bits.data3  = motion_noise_lut[10][3];
    V0_TNR_CLUT421.bits.data0  = motion_noise_lut[10][4];
    V0_TNR_CLUT421.bits.data1  = motion_noise_lut[10][5];
    V0_TNR_CLUT421.bits.data2  = motion_noise_lut[10][6];
    V0_TNR_CLUT421.bits.data3  = motion_noise_lut[10][7];
    V0_TNR_CLUT422.bits.data0  = motion_noise_lut[11][0];
    V0_TNR_CLUT422.bits.data1  = motion_noise_lut[11][1];
    V0_TNR_CLUT422.bits.data2  = motion_noise_lut[11][2];
    V0_TNR_CLUT422.bits.data3  = motion_noise_lut[11][3];
    V0_TNR_CLUT423.bits.data0  = motion_noise_lut[11][4];
    V0_TNR_CLUT423.bits.data1  = motion_noise_lut[11][5];
    V0_TNR_CLUT423.bits.data2  = motion_noise_lut[11][6];
    V0_TNR_CLUT423.bits.data3  = motion_noise_lut[11][7];
    V0_TNR_CLUT424.bits.data0  = motion_noise_lut[12][0];
    V0_TNR_CLUT424.bits.data1  = motion_noise_lut[12][1];
    V0_TNR_CLUT424.bits.data2  = motion_noise_lut[12][2];
    V0_TNR_CLUT424.bits.data3  = motion_noise_lut[12][3];
    V0_TNR_CLUT425.bits.data0  = motion_noise_lut[12][4];
    V0_TNR_CLUT425.bits.data1  = motion_noise_lut[12][5];
    V0_TNR_CLUT425.bits.data2  = motion_noise_lut[12][6];
    V0_TNR_CLUT425.bits.data3  = motion_noise_lut[12][7];
    V0_TNR_CLUT426.bits.data0  = motion_noise_lut[13][0];
    V0_TNR_CLUT426.bits.data1  = motion_noise_lut[13][1];
    V0_TNR_CLUT426.bits.data2  = motion_noise_lut[13][2];
    V0_TNR_CLUT426.bits.data3  = motion_noise_lut[13][3];
    V0_TNR_CLUT427.bits.data0  = motion_noise_lut[13][4];
    V0_TNR_CLUT427.bits.data1  = motion_noise_lut[13][5];
    V0_TNR_CLUT427.bits.data2  = motion_noise_lut[13][6];
    V0_TNR_CLUT427.bits.data3  = motion_noise_lut[13][7];
    V0_TNR_CLUT428.bits.data0  = motion_noise_lut[14][0];
    V0_TNR_CLUT428.bits.data1  = motion_noise_lut[14][1];
    V0_TNR_CLUT428.bits.data2  = motion_noise_lut[14][2];
    V0_TNR_CLUT428.bits.data3  = motion_noise_lut[14][3];
    V0_TNR_CLUT429.bits.data0  = motion_noise_lut[14][4];
    V0_TNR_CLUT429.bits.data1  = motion_noise_lut[14][5];
    V0_TNR_CLUT429.bits.data2  = motion_noise_lut[14][6];
    V0_TNR_CLUT429.bits.data3  = motion_noise_lut[14][7];
    V0_TNR_CLUT430.bits.data0  = motion_noise_lut[15][0];
    V0_TNR_CLUT430.bits.data1  = motion_noise_lut[15][1];
    V0_TNR_CLUT430.bits.data2  = motion_noise_lut[15][2];
    V0_TNR_CLUT430.bits.data3  = motion_noise_lut[15][3];
    V0_TNR_CLUT431.bits.data0  = motion_noise_lut[15][4];
    V0_TNR_CLUT431.bits.data1  = motion_noise_lut[15][5];
    V0_TNR_CLUT431.bits.data2  = motion_noise_lut[15][6];
    V0_TNR_CLUT431.bits.data3  = motion_noise_lut[15][7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT40.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT40.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT41.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT41.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT42.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT42.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT43.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT43.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT44.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT44.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT45.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT45.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT46.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT46.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT47.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT47.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT48.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT48.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT49.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT49.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT410.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT410.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT411.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT411.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT412.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT412.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT413.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT413.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT414.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT414.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT415.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT415.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT416.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT416.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT417.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT417.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT418.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT418.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT419.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT419.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT420.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT420.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT421.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT421.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT422.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT422.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT423.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT423.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT424.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT424.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT425.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT425.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT426.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT426.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT427.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT427.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT428.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT428.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT429.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT429.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT430.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT430.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT431.u32) + u32Data * VID_OFFSET),  V0_TNR_CLUT431.u32);
    return;
}
HI_VOID VDP_VID_SetTNRYMotionStrengthLut(HI_U32 u32Data,HI_U32 ymotion_strength_lut[32])
{
    U_V0_TNR_CLUT50  V0_TNR_CLUT50 ;
    U_V0_TNR_CLUT51  V0_TNR_CLUT51 ;
    U_V0_TNR_CLUT52  V0_TNR_CLUT52 ;
    U_V0_TNR_CLUT53  V0_TNR_CLUT53 ;
    U_V0_TNR_CLUT54  V0_TNR_CLUT54 ;
    U_V0_TNR_CLUT55  V0_TNR_CLUT55 ;
    U_V0_TNR_CLUT56  V0_TNR_CLUT56 ;
    U_V0_TNR_CLUT57  V0_TNR_CLUT57 ;

    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTNRYMotionStrengthLut() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_CLUT50.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT50.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT51.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT51.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT52.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT52.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT53.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT53.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT54.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT54.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT55.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT55.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT56.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT56.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT57.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT57.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT50.bits.data0   = ymotion_strength_lut[ 0];
    V0_TNR_CLUT50.bits.data1   = ymotion_strength_lut[ 1];
    V0_TNR_CLUT50.bits.data2   = ymotion_strength_lut[ 2];
    V0_TNR_CLUT50.bits.data3   = ymotion_strength_lut[ 3];
    V0_TNR_CLUT51.bits.data0   = ymotion_strength_lut[ 4];
    V0_TNR_CLUT51.bits.data1   = ymotion_strength_lut[ 5];
    V0_TNR_CLUT51.bits.data2   = ymotion_strength_lut[ 6];
    V0_TNR_CLUT51.bits.data3   = ymotion_strength_lut[ 7];
    V0_TNR_CLUT52.bits.data0   = ymotion_strength_lut[ 8];
    V0_TNR_CLUT52.bits.data1   = ymotion_strength_lut[ 9];
    V0_TNR_CLUT52.bits.data2   = ymotion_strength_lut[ 10];
    V0_TNR_CLUT52.bits.data3   = ymotion_strength_lut[ 11];
    V0_TNR_CLUT53.bits.data0   = ymotion_strength_lut[ 12];
    V0_TNR_CLUT53.bits.data1   = ymotion_strength_lut[ 13];
    V0_TNR_CLUT53.bits.data2   = ymotion_strength_lut[ 14];
    V0_TNR_CLUT53.bits.data3   = ymotion_strength_lut[ 15];
    V0_TNR_CLUT54.bits.data0   = ymotion_strength_lut[ 16];
    V0_TNR_CLUT54.bits.data1   = ymotion_strength_lut[ 17];
    V0_TNR_CLUT54.bits.data2   = ymotion_strength_lut[ 18];
    V0_TNR_CLUT54.bits.data3   = ymotion_strength_lut[ 19];
    V0_TNR_CLUT55.bits.data0   = ymotion_strength_lut[ 20];
    V0_TNR_CLUT55.bits.data1   = ymotion_strength_lut[ 21];
    V0_TNR_CLUT55.bits.data2   = ymotion_strength_lut[ 22];
    V0_TNR_CLUT55.bits.data3   = ymotion_strength_lut[ 23];
    V0_TNR_CLUT56.bits.data0   = ymotion_strength_lut[ 24];
    V0_TNR_CLUT56.bits.data1   = ymotion_strength_lut[ 25];
    V0_TNR_CLUT56.bits.data2   = ymotion_strength_lut[ 26];
    V0_TNR_CLUT56.bits.data3   = ymotion_strength_lut[ 27];
    V0_TNR_CLUT57.bits.data0   = ymotion_strength_lut[ 28];
    V0_TNR_CLUT57.bits.data1   = ymotion_strength_lut[ 29];
    V0_TNR_CLUT57.bits.data2   = ymotion_strength_lut[ 30];
    V0_TNR_CLUT57.bits.data3   = ymotion_strength_lut[ 31];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT50.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT50.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT51.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT51.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT52.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT52.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT53.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT53.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT54.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT54.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT55.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT55.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT56.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT56.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT57.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT57.u32);
    return;
}
HI_VOID VDP_VID_SetTNRCMotionStrengthLut(HI_U32 u32Data,HI_U32 cmotion_strength_lut[32])
{
    U_V0_TNR_CLUT60  V0_TNR_CLUT60 ;
    U_V0_TNR_CLUT61  V0_TNR_CLUT61 ;
    U_V0_TNR_CLUT62  V0_TNR_CLUT62 ;
    U_V0_TNR_CLUT63  V0_TNR_CLUT63 ;
    U_V0_TNR_CLUT64  V0_TNR_CLUT64 ;
    U_V0_TNR_CLUT65  V0_TNR_CLUT65 ;
    U_V0_TNR_CLUT66  V0_TNR_CLUT66 ;
    U_V0_TNR_CLUT67  V0_TNR_CLUT67 ;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTNRCMotionStrengthLut() select wrong video layer ID\n");
        return ;
    }

    V0_TNR_CLUT60.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT60.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT61.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT61.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT62.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT62.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT63.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT63.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT64.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT64.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT65.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT65.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT66.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT66.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT67.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT67.u32) + u32Data * VID_OFFSET));
    V0_TNR_CLUT60.bits.data0   = cmotion_strength_lut[ 0];
    V0_TNR_CLUT60.bits.data1   = cmotion_strength_lut[ 1];
    V0_TNR_CLUT60.bits.data2   = cmotion_strength_lut[ 2];
    V0_TNR_CLUT60.bits.data3   = cmotion_strength_lut[ 3];
    V0_TNR_CLUT61.bits.data0   = cmotion_strength_lut[ 4];
    V0_TNR_CLUT61.bits.data1   = cmotion_strength_lut[ 5];
    V0_TNR_CLUT61.bits.data2   = cmotion_strength_lut[ 6];
    V0_TNR_CLUT61.bits.data3   = cmotion_strength_lut[ 7];
    V0_TNR_CLUT62.bits.data0   = cmotion_strength_lut[ 8];
    V0_TNR_CLUT62.bits.data1   = cmotion_strength_lut[ 9];
    V0_TNR_CLUT62.bits.data2   = cmotion_strength_lut[ 10];
    V0_TNR_CLUT62.bits.data3   = cmotion_strength_lut[ 11];
    V0_TNR_CLUT63.bits.data0   = cmotion_strength_lut[ 12];
    V0_TNR_CLUT63.bits.data1   = cmotion_strength_lut[ 13];
    V0_TNR_CLUT63.bits.data2   = cmotion_strength_lut[ 14];
    V0_TNR_CLUT63.bits.data3   = cmotion_strength_lut[ 15];
    V0_TNR_CLUT64.bits.data0   = cmotion_strength_lut[ 16];
    V0_TNR_CLUT64.bits.data1   = cmotion_strength_lut[ 17];
    V0_TNR_CLUT64.bits.data2   = cmotion_strength_lut[ 18];
    V0_TNR_CLUT64.bits.data3   = cmotion_strength_lut[ 19];
    V0_TNR_CLUT65.bits.data0   = cmotion_strength_lut[ 20];
    V0_TNR_CLUT65.bits.data1   = cmotion_strength_lut[ 21];
    V0_TNR_CLUT65.bits.data2   = cmotion_strength_lut[ 22];
    V0_TNR_CLUT65.bits.data3   = cmotion_strength_lut[ 23];
    V0_TNR_CLUT66.bits.data0   = cmotion_strength_lut[ 24];
    V0_TNR_CLUT66.bits.data1   = cmotion_strength_lut[ 25];
    V0_TNR_CLUT66.bits.data2   = cmotion_strength_lut[ 26];
    V0_TNR_CLUT66.bits.data3   = cmotion_strength_lut[ 27];
    V0_TNR_CLUT67.bits.data0   = cmotion_strength_lut[ 28];
    V0_TNR_CLUT67.bits.data1   = cmotion_strength_lut[ 29];
    V0_TNR_CLUT67.bits.data2   = cmotion_strength_lut[ 30];
    V0_TNR_CLUT67.bits.data3   = cmotion_strength_lut[ 31];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT60.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT60.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT61.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT61.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT62.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT62.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT63.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT63.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT64.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT64.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT65.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT65.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT66.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT66.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CLUT67.u32) + u32Data * VID_OFFSET),   V0_TNR_CLUT67.u32);
    return;
}
//HI_VOID VDP_VID_SetMadRAddr(HI_U32 u32Data, HI_U32 u32Data1)
//{
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetMadRAddr() select wrong video layer ID\n");
//      return ;
//    }
//
//    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MAD_RADDR) + u32Data * VID_OFFSET), u32Data1); 
//    return ;
//}
//
//HI_VOID VDP_VID_SetMadWAddr(HI_U32 u32Data, HI_U32 u32Data1)
//{
//    if(u32Data >= VID_MAX)
//    {
//      VDP_PRINT("Error,VDP_VID_SetMadWAddr() select wrong video layer ID\n");
//      return ;
//    }
//
//    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MAD_WADDR) + u32Data * VID_OFFSET), u32Data1); 
//    return ;
//}
//
//HI_VOID VDP_VID_SetMadStride(HI_U32 u32Data, HI_U32 str)
//{
//
//    //U_V0_MADSTRIDE V0_MADSTRIDE;
//    //if(u32Data >= VID_MAX)
//    //{
//    //  VDP_PRINT("Error,VDP_VID_SetMadStride() select wrong video layer ID\n");
//    //  return ;
//    //}
//    //V0_MADSTRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MADSTRIDE.u32) + u32Data * VID_OFFSET));
//    //V0_MADSTRIDE.bits.mad_stride = str;
//    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MADSTRIDE.u32) + u32Data * VID_OFFSET), VPSS_MADSTRIDE.u32); 
//    return ;
//}
//HI_VOID VDP_VID_SetMadAttr(HI_U32 u32Data, HI_U32 r_val, HI_U32 w_val, HI_U32 str)
//{
//    VDP_VID_SetMadRAddr(u32Data, r_val);
//    VDP_VID_SetMadWAddr(u32Data, w_val);
//    VDP_VID_SetMadStride(u32Data, str);
//}

HI_VOID VDP_VID_SetTnrWbcEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcEnable() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_CTRL.bits.wbc_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcCEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcCEnable() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_CTRL.bits.wbc_c_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcRegUp(HI_U32 u32Data)
{
    U_WBC_FI_UPD   WBC_FI_UPD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcUpd() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_UPD.bits.regup = 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_UPD.u32) + u32Data * VID_OFFSET), WBC_FI_UPD.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr, HI_U32 u32CAddr)
{
    U_WBC_FI_YADDR   WBC_FI_YADDR;
    U_WBC_FI_CADDR   WBC_FI_CADDR;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcAddr() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_YADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_YADDR.u32) + u32Data * VID_OFFSET));
    WBC_FI_CADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_CADDR.u32) + u32Data * VID_OFFSET));

    WBC_FI_YADDR.bits.wbcaddr = u32YAddr;
    WBC_FI_CADDR.bits.wbcaddr = u32CAddr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_YADDR.u32) + u32Data * VID_OFFSET), WBC_FI_YADDR.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_CADDR.u32) + u32Data * VID_OFFSET), WBC_FI_CADDR.u32);
    return;
}

HI_VOID VDP_VID_SetTnrWbcStride(HI_U32 u32Data, HI_U32 u32YStride, HI_U32 u32CStride)
{
    U_WBC_FI_STRIDE   WBC_FI_STRIDE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcStride() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_STRIDE.u32) + u32Data * VID_OFFSET));

    WBC_FI_STRIDE.bits.wbclstride = u32YStride;
    WBC_FI_STRIDE.bits.wbccstride = u32CStride;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_STRIDE.u32) + u32Data * VID_OFFSET), WBC_FI_STRIDE.u32);
    return;
}

HI_VOID VDP_VID_SetMadWbcEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_ME_CTRL   WBC_ME_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetMadWbcEnable() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_ME_CTRL.bits.wbc_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_CTRL.u32) + u32Data * VID_OFFSET), WBC_ME_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetMadWbcDataMode(HI_U32 u32Data, HI_U32 mad_data_mode)
{
    U_WBC_ME_CTRL   WBC_ME_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetMadWbcDataMode() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_ME_CTRL.bits.mad_data_mode = mad_data_mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_CTRL.u32) + u32Data * VID_OFFSET), WBC_ME_CTRL.u32);

    return;
}

HI_VOID  VDP_VID_SetWbcTnrMadSmmuBypass   (HI_U32 u32Data, HI_U32 u32Bypass)
{
    U_WBC_ME_SMMU_BYPASS        WBC_ME_SMMU_BYPASS;


    WBC_ME_SMMU_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET));
    WBC_ME_SMMU_BYPASS.bits.l_bypass  = u32Bypass;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET), WBC_ME_SMMU_BYPASS.u32); 

    return ;
}

HI_VOID  VDP_VID_SetWbcTnrSttSmmuBypass   (HI_U32 u32Data, HI_U32 u32Bypass)
{
    U_WBC_ME_SMMU_BYPASS        WBC_ME_SMMU_BYPASS;


    WBC_ME_SMMU_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET));
    WBC_ME_SMMU_BYPASS.bits.c_bypass  = u32Bypass;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET), WBC_ME_SMMU_BYPASS.u32); 

    return ;
}

HI_VOID  VDP_VID_SetWbcTnrPreSmmuLBypass   (HI_U32 u32Data, HI_U32 u32Bypass)
{
    U_WBC_FI_SMMU_BYPASS        WBC_FI_SMMU_BYPASS;


    WBC_FI_SMMU_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET));
    WBC_FI_SMMU_BYPASS.bits.l_bypass  = u32Bypass;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET), WBC_FI_SMMU_BYPASS.u32); 

    return ;
}

HI_VOID  VDP_VID_SetWbcTnrPreSmmuCBypass   (HI_U32 u32Data, HI_U32 u32Bypass)
{
    U_WBC_FI_SMMU_BYPASS        WBC_FI_SMMU_BYPASS;


    WBC_FI_SMMU_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET));
    WBC_FI_SMMU_BYPASS.bits.c_bypass  = u32Bypass;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_SMMU_BYPASS.u32) + u32Data * WBC_OFFSET), WBC_FI_SMMU_BYPASS.u32); 

    return ;
}

HI_VOID VDP_VID_SetTnrCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_TNR_CFGAD         V0_TNR_CFGAD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrCfgAddr() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_CFGAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_CFGAD.u32) + u32Data * VID_OFFSET));

    V0_TNR_CFGAD.bits.coef_addr = u32Addr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_CFGAD.u32) + u32Data * VID_OFFSET), V0_TNR_CFGAD.u32);
    return;
}

HI_VOID VDP_VID_SetTnrLutAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_TNR_LUTAD         V0_TNR_LUTAD                      ; /* 0x81c */
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrLutAddr() select wrong video layer ID\n");
      return ;
    }

    V0_TNR_LUTAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_LUTAD.u32) + u32Data * VID_OFFSET));

    V0_TNR_LUTAD.bits.coef_addr = u32Addr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_LUTAD.u32) + u32Data * VID_OFFSET), V0_TNR_LUTAD.u32);
    return;
}

HI_VOID  VDP_VID_SetTnrPreAddr   (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32CAddr, HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_V0_P1STRIDE          V0_P1STRIDE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P1LADDR.u32) + u32Data * VID_OFFSET), u32LAddr); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P1CADDR.u32) + u32Data * VID_OFFSET), u32CAddr); 

    V0_P1STRIDE.bits.surface_stride = u32LStr;
    V0_P1STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P1STRIDE.u32) + u32Data * VID_OFFSET), V0_P1STRIDE.u32); 

    return ;
}

HI_VOID  VDP_VID_SetTnrMadAddr   (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32LStr)
{
    U_V0_P2STRIDE          V0_P2STRIDE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrMadAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P2LADDR.u32) + u32Data * VID_OFFSET), u32LAddr); 

    V0_P2STRIDE.bits.surface_stride = u32LStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P2STRIDE.u32) + u32Data * VID_OFFSET), V0_P2STRIDE.u32); 

    return ;
}

HI_VOID VDP_VID_SetMadWbcRegUp(HI_U32 u32Data)
{
    U_WBC_ME_UPD   WBC_ME_UPD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetMadWbcUpd() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_UPD.bits.regup = 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_UPD.u32) + u32Data * VID_OFFSET), WBC_ME_UPD.u32);

    return;
}

HI_VOID VDP_VID_SetMadWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr)
{
    U_WBC_ME_YADDR   WBC_ME_YADDR;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetMadWbcAddr() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_YADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_YADDR.u32) + u32Data * VID_OFFSET));

    WBC_ME_YADDR.bits.wbcaddr = u32YAddr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_YADDR.u32) + u32Data * VID_OFFSET), WBC_ME_YADDR.u32);
    return;
}

HI_VOID VDP_VID_SetMadWbcStride(HI_U32 u32Data, HI_U32 u32YStride)
{
    U_WBC_ME_STRIDE   WBC_ME_STRIDE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetMadWbcStride() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_STRIDE.u32) + u32Data * VID_OFFSET));

    WBC_ME_STRIDE.bits.wbclstride = u32YStride;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_STRIDE.u32) + u32Data * VID_OFFSET), WBC_ME_STRIDE.u32);
    return;
}

HI_VOID VDP_VID_SetTnrCEnable(HI_U32 u32Data, HI_U32 tnr_c_en)
{
    U_V0_PRERD   V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrCEnable() select wrong video layer ID\n");
      return ;
    }

    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));

    V0_PRERD.bits.tnr_c_en = tnr_c_en;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcDitherEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_FI_DITHER_CTRL   WBC_FI_DITHER_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcDitherEnable() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_DITHER_CTRL.bits.dither_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_DITHER_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcDitherMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    U_WBC_FI_DITHER_CTRL   WBC_FI_DITHER_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcDitherMode() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_DITHER_CTRL.bits.dither_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_DITHER_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcDitherRound(HI_U32 u32Data, HI_U32 u32Round)
{
    U_WBC_FI_DITHER_CTRL   WBC_FI_DITHER_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcDitherRound() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_DITHER_CTRL.bits.dither_round = u32Round;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_DITHER_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_DITHER_CTRL.u32);

    return;
}

HI_VOID VDP_VID_SetTnrWbcDataWidth(HI_U32 u32Data, HI_U32 u32DataWidth)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetTnrWbcDataWidth() select wrong video layer ID\n");
      return ;
    }

    WBC_FI_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_CTRL.bits.data_width = u32DataWidth;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_CTRL.u32);

    return;
}

///REF DCMP
HI_VOID  VDP_VID_SetRefDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRefDcmpEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VPSSIP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_VPSSIP_CTRL.bits.dcmp_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VPSSIP_CTRL.u32) + u32Data * VID_OFFSET), V0_VPSSIP_CTRL.u32);


    return ;
}

///WBC REF CMP
HI_VOID  VDP_VID_SetTnrWbcCmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTnrWbcCmpEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    WBC_FI_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET));
    WBC_FI_CTRL.bits.cmp_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_FI_CTRL.u32) + u32Data * VID_OFFSET), WBC_FI_CTRL.u32);


    return ;
}
//TNR CFG END


//SNR CFG START

HI_VOID VDP_VID_SetSnrCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_SNR_CFGAD         V0_SNR_CFGAD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetSnrCfgAddr() select wrong video layer ID\n");
      return ;
    }

    V0_SNR_CFGAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_CFGAD.u32) + u32Data * VID_OFFSET));

    V0_SNR_CFGAD.bits.coef_addr = u32Addr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_CFGAD.u32) + u32Data * VID_OFFSET), V0_SNR_CFGAD.u32);
    return;
}

HI_VOID VDP_VID_SetSnrEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_PRERD V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetNrEnable() select wrong video layer ID\n");
      return ;
    }
    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.snr_en = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);
    return;
}

HI_VOID VDP_VID_SetSnrstrength(HI_U32 u32Data, HI_U32 snrstrength)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSnrstrength Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.snrstrength = snrstrength;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSnrfiltermode(HI_U32 u32Data, HI_U32 snrfiltermode)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSnrfiltermode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.snrfiltermode = snrfiltermode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioen(HI_U32 u32Data, HI_U32 meanedgeratioen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.meanedgeratioen = meanedgeratioen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgelpfmode(HI_U32 u32Data, HI_U32 edgelpfmode)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgelpfmode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.edgelpfmode = edgelpfmode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDiredgesmoothen(HI_U32 u32Data, HI_U32 diredgesmoothen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDiredgesmoothen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.diredgesmoothen = diredgesmoothen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDebugmode(HI_U32 u32Data, HI_U32 debugmode)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDebugmode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.debugmode = debugmode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDebugmodeen(HI_U32 u32Data, HI_U32 debugmodeen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDebugmodeen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.debugmodeen = debugmodeen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDemomodelr(HI_U32 u32Data, HI_U32 demomodelr)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDemomodelr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.demomodelr = demomodelr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDemomodeen(HI_U32 u32Data, HI_U32 demomodeen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDemomodeen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.demomodeen = demomodeen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetBlendingmode(HI_U32 u32Data, HI_U32 blendingmode)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetBlendingmode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.blendingmode = blendingmode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgeonlyen(HI_U32 u32Data, HI_U32 edgeonlyen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgeonlyen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.edgeonlyen = edgeonlyen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMotionedgeen(HI_U32 u32Data, HI_U32 motionedgeen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMotionedgeen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.motionedgeen = motionedgeen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetColorweighten(HI_U32 u32Data, HI_U32 colorweighten)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetColorweighten Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.colorweighten = colorweighten;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgeprelpfen(HI_U32 u32Data, HI_U32 edgeprelpfen)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgeprelpfen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.edgeprelpfen = edgeprelpfen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetChromasnren(HI_U32 u32Data, HI_U32 chromasnren)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetChromasnren Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.chromasnren = chromasnren;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetLumasnren(HI_U32 u32Data, HI_U32 lumasnren)
{
	U_V0_SNR_ENABLE V0_SNR_ENABLE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetLumasnren Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ENABLE.bits.lumasnren = lumasnren;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ENABLE.u32) + u32Data * VID_OFFSET),V0_SNR_ENABLE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox2(HI_U32 u32Data, HI_U32 meanedgeratiox2)
{
	U_V0_SNR_EDGE_RATIOX0 V0_SNR_EDGE_RATIOX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox2 = meanedgeratiox2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox1(HI_U32 u32Data, HI_U32 meanedgeratiox1)
{
	U_V0_SNR_EDGE_RATIOX0 V0_SNR_EDGE_RATIOX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox1 = meanedgeratiox1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox0(HI_U32 u32Data, HI_U32 meanedgeratiox0)
{
	U_V0_SNR_EDGE_RATIOX0 V0_SNR_EDGE_RATIOX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox0 = meanedgeratiox0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox5(HI_U32 u32Data, HI_U32 meanedgeratiox5)
{
	U_V0_SNR_EDGE_RATIOX1 V0_SNR_EDGE_RATIOX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox5 = meanedgeratiox5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox4(HI_U32 u32Data, HI_U32 meanedgeratiox4)
{
	U_V0_SNR_EDGE_RATIOX1 V0_SNR_EDGE_RATIOX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox4 = meanedgeratiox4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiox3(HI_U32 u32Data, HI_U32 meanedgeratiox3)
{
	U_V0_SNR_EDGE_RATIOX1 V0_SNR_EDGE_RATIOX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiox3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox3 = meanedgeratiox3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioy2(HI_U32 u32Data, HI_U32 meanedgeratioy2)
{
	U_V0_SNR_EDGE_RATIOY0 V0_SNR_EDGE_RATIOY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioy2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy2 = meanedgeratioy2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioy1(HI_U32 u32Data, HI_U32 meanedgeratioy1)
{
	U_V0_SNR_EDGE_RATIOY0 V0_SNR_EDGE_RATIOY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioy1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy1 = meanedgeratioy1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioy0(HI_U32 u32Data, HI_U32 meanedgeratioy0)
{
	U_V0_SNR_EDGE_RATIOY0 V0_SNR_EDGE_RATIOY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioy0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy0 = meanedgeratioy0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioy4(HI_U32 u32Data, HI_U32 meanedgeratioy4)
{
	U_V0_SNR_EDGE_RATIOY1 V0_SNR_EDGE_RATIOY1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioy4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOY1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy4 = meanedgeratioy4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOY1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratioy3(HI_U32 u32Data, HI_U32 meanedgeratioy3)
{
	U_V0_SNR_EDGE_RATIOY1 V0_SNR_EDGE_RATIOY1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratioy3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOY1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy3 = meanedgeratioy3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOY1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOY1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiok1(HI_U32 u32Data, HI_U32 meanedgeratiok1)
{
	U_V0_SNR_EDGE_RATIOK0 V0_SNR_EDGE_RATIOK0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiok1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOK0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok1 = meanedgeratiok1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOK0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiok0(HI_U32 u32Data, HI_U32 meanedgeratiok0)
{
	U_V0_SNR_EDGE_RATIOK0 V0_SNR_EDGE_RATIOK0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiok0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOK0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok0 = meanedgeratiok0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOK0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiok3(HI_U32 u32Data, HI_U32 meanedgeratiok3)
{
	U_V0_SNR_EDGE_RATIOK1 V0_SNR_EDGE_RATIOK1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiok3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok3 = meanedgeratiok3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOK1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiok2(HI_U32 u32Data, HI_U32 meanedgeratiok2)
{
	U_V0_SNR_EDGE_RATIOK1 V0_SNR_EDGE_RATIOK1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiok2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIOK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok2 = meanedgeratiok2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIOK1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIOK1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiomin(HI_U32 u32Data, HI_U32 meanedgeratiomin)
{
	U_V0_SNR_EDGE_RATIO V0_SNR_EDGE_RATIO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiomin Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIO.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIO.bits.meanedgeratiomin = meanedgeratiomin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIO.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMeanedgeratiomax(HI_U32 u32Data, HI_U32 meanedgeratiomax)
{
	U_V0_SNR_EDGE_RATIO V0_SNR_EDGE_RATIO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMeanedgeratiomax Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_RATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIO.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_RATIO.bits.meanedgeratiomax = meanedgeratiomax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_RATIO.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_RATIO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx2(HI_U32 u32Data, HI_U32 edgestrmappingx2)
{
	U_V0_SNR_EDGE_STRX0 V0_SNR_EDGE_STRX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX0.bits.edgestrmappingx2 = edgestrmappingx2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx1(HI_U32 u32Data, HI_U32 edgestrmappingx1)
{
	U_V0_SNR_EDGE_STRX0 V0_SNR_EDGE_STRX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX0.bits.edgestrmappingx1 = edgestrmappingx1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx0(HI_U32 u32Data, HI_U32 edgestrmappingx0)
{
	U_V0_SNR_EDGE_STRX0 V0_SNR_EDGE_STRX0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX0.bits.edgestrmappingx0 = edgestrmappingx0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx5(HI_U32 u32Data, HI_U32 edgestrmappingx5)
{
	U_V0_SNR_EDGE_STRX1 V0_SNR_EDGE_STRX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX1.bits.edgestrmappingx5 = edgestrmappingx5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx4(HI_U32 u32Data, HI_U32 edgestrmappingx4)
{
	U_V0_SNR_EDGE_STRX1 V0_SNR_EDGE_STRX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX1.bits.edgestrmappingx4 = edgestrmappingx4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx3(HI_U32 u32Data, HI_U32 edgestrmappingx3)
{
	U_V0_SNR_EDGE_STRX1 V0_SNR_EDGE_STRX1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX1.bits.edgestrmappingx3 = edgestrmappingx3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx7(HI_U32 u32Data, HI_U32 edgestrmappingx7)
{
	U_V0_SNR_EDGE_STRX2 V0_SNR_EDGE_STRX2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX2.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX2.bits.edgestrmappingx7 = edgestrmappingx7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX2.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingx6(HI_U32 u32Data, HI_U32 edgestrmappingx6)
{
	U_V0_SNR_EDGE_STRX2 V0_SNR_EDGE_STRX2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingx6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX2.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRX2.bits.edgestrmappingx6 = edgestrmappingx6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRX2.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRX2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy3(HI_U32 u32Data, HI_U32 edgestrmappingy3)
{
	U_V0_SNR_EDGE_STRY0 V0_SNR_EDGE_STRY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY0.bits.edgestrmappingy3 = edgestrmappingy3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy2(HI_U32 u32Data, HI_U32 edgestrmappingy2)
{
	U_V0_SNR_EDGE_STRY0 V0_SNR_EDGE_STRY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY0.bits.edgestrmappingy2 = edgestrmappingy2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy1(HI_U32 u32Data, HI_U32 edgestrmappingy1)
{
	U_V0_SNR_EDGE_STRY0 V0_SNR_EDGE_STRY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY0.bits.edgestrmappingy1 = edgestrmappingy1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy0(HI_U32 u32Data, HI_U32 edgestrmappingy0)
{
	U_V0_SNR_EDGE_STRY0 V0_SNR_EDGE_STRY0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY0.bits.edgestrmappingy0 = edgestrmappingy0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy6(HI_U32 u32Data, HI_U32 edgestrmappingy6)
{
	U_V0_SNR_EDGE_STRY1 V0_SNR_EDGE_STRY1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY1.bits.edgestrmappingy6 = edgestrmappingy6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy5(HI_U32 u32Data, HI_U32 edgestrmappingy5)
{
	U_V0_SNR_EDGE_STRY1 V0_SNR_EDGE_STRY1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY1.bits.edgestrmappingy5 = edgestrmappingy5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingy4(HI_U32 u32Data, HI_U32 edgestrmappingy4)
{
	U_V0_SNR_EDGE_STRY1 V0_SNR_EDGE_STRY1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingy4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRY1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRY1.bits.edgestrmappingy4 = edgestrmappingy4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRY1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRY1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk2(HI_U32 u32Data, HI_U32 edgestrmappingk2)
{
	U_V0_SNR_EDGE_STRK0 V0_SNR_EDGE_STRK0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK0.bits.edgestrmappingk2 = edgestrmappingk2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk1(HI_U32 u32Data, HI_U32 edgestrmappingk1)
{
	U_V0_SNR_EDGE_STRK0 V0_SNR_EDGE_STRK0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK0.bits.edgestrmappingk1 = edgestrmappingk1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk0(HI_U32 u32Data, HI_U32 edgestrmappingk0)
{
	U_V0_SNR_EDGE_STRK0 V0_SNR_EDGE_STRK0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK0.bits.edgestrmappingk0 = edgestrmappingk0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK0.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk5(HI_U32 u32Data, HI_U32 edgestrmappingk5)
{
	U_V0_SNR_EDGE_STRK1 V0_SNR_EDGE_STRK1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK1.bits.edgestrmappingk5 = edgestrmappingk5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk4(HI_U32 u32Data, HI_U32 edgestrmappingk4)
{
	U_V0_SNR_EDGE_STRK1 V0_SNR_EDGE_STRK1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK1.bits.edgestrmappingk4 = edgestrmappingk4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingk3(HI_U32 u32Data, HI_U32 edgestrmappingk3)
{
	U_V0_SNR_EDGE_STRK1 V0_SNR_EDGE_STRK1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingk3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STRK1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STRK1.bits.edgestrmappingk3 = edgestrmappingk3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STRK1.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STRK1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingmax(HI_U32 u32Data, HI_U32 edgestrmappingmax)
{
	U_V0_SNR_EDGE_STR V0_SNR_EDGE_STR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingmax Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STR.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STR.bits.edgestrmappingmax = edgestrmappingmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STR.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgestrmappingmin(HI_U32 u32Data, HI_U32 edgestrmappingmin)
{
	U_V0_SNR_EDGE_STR V0_SNR_EDGE_STR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgestrmappingmin Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_EDGE_STR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STR.u32) + u32Data * VID_OFFSET));
	V0_SNR_EDGE_STR.bits.edgestrmappingmin = edgestrmappingmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_EDGE_STR.u32) + u32Data * VID_OFFSET),V0_SNR_EDGE_STR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgevalthd3(HI_U32 u32Data, HI_U32 sfymaxedgevalthd3)
{
	U_V0_SNR_SFYMAX_VALTH V0_SNR_SFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgevalthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_VALTH.bits.sfymaxedgevalthd3 = sfymaxedgevalthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgevalthd2(HI_U32 u32Data, HI_U32 sfymaxedgevalthd2)
{
	U_V0_SNR_SFYMAX_VALTH V0_SNR_SFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_VALTH.bits.sfymaxedgevalthd2 = sfymaxedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgevalthd1(HI_U32 u32Data, HI_U32 sfymaxedgevalthd1)
{
	U_V0_SNR_SFYMAX_VALTH V0_SNR_SFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_VALTH.bits.sfymaxedgevalthd1 = sfymaxedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgeidxthd3(HI_U32 u32Data, HI_U32 sfymaxedgeidxthd3)
{
	U_V0_SNR_SFYMAX_IDXTH V0_SNR_SFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgeidxthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_IDXTH.bits.sfymaxedgeidxthd3 = sfymaxedgeidxthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgeidxthd2(HI_U32 u32Data, HI_U32 sfymaxedgeidxthd2)
{
	U_V0_SNR_SFYMAX_IDXTH V0_SNR_SFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgeidxthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_IDXTH.bits.sfymaxedgeidxthd2 = sfymaxedgeidxthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymaxedgeidxthd1(HI_U32 u32Data, HI_U32 sfymaxedgeidxthd1)
{
	U_V0_SNR_SFYMAX_IDXTH V0_SNR_SFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymaxedgeidxthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMAX_IDXTH.bits.sfymaxedgeidxthd1 = sfymaxedgeidxthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymeanedgevalthd3(HI_U32 u32Data, HI_U32 sfymeanedgevalthd3)
{
	U_V0_SNR_SFYMEAN_VALTH V0_SNR_SFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymeanedgevalthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMEAN_VALTH.bits.sfymeanedgevalthd3 = sfymeanedgevalthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymeanedgevalthd2(HI_U32 u32Data, HI_U32 sfymeanedgevalthd2)
{
	U_V0_SNR_SFYMEAN_VALTH V0_SNR_SFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymeanedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMEAN_VALTH.bits.sfymeanedgevalthd2 = sfymeanedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfymeanedgevalthd1(HI_U32 u32Data, HI_U32 sfymeanedgevalthd1)
{
	U_V0_SNR_SFYMEAN_VALTH V0_SNR_SFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfymeanedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFYMEAN_VALTH.bits.sfymeanedgevalthd1 = sfymeanedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgevalthd3(HI_U32 u32Data, HI_U32 wfymaxedgevalthd3)
{
	U_V0_SNR_WFYMAX_VALTH V0_SNR_WFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgevalthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_VALTH.bits.wfymaxedgevalthd3 = wfymaxedgevalthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgevalthd2(HI_U32 u32Data, HI_U32 wfymaxedgevalthd2)
{
	U_V0_SNR_WFYMAX_VALTH V0_SNR_WFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_VALTH.bits.wfymaxedgevalthd2 = wfymaxedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgevalthd1(HI_U32 u32Data, HI_U32 wfymaxedgevalthd1)
{
	U_V0_SNR_WFYMAX_VALTH V0_SNR_WFYMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_VALTH.bits.wfymaxedgevalthd1 = wfymaxedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgeidxthd3(HI_U32 u32Data, HI_U32 wfymaxedgeidxthd3)
{
	U_V0_SNR_WFYMAX_IDXTH V0_SNR_WFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgeidxthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_IDXTH.bits.wfymaxedgeidxthd3 = wfymaxedgeidxthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgeidxthd2(HI_U32 u32Data, HI_U32 wfymaxedgeidxthd2)
{
	U_V0_SNR_WFYMAX_IDXTH V0_SNR_WFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgeidxthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_IDXTH.bits.wfymaxedgeidxthd2 = wfymaxedgeidxthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymaxedgeidxthd1(HI_U32 u32Data, HI_U32 wfymaxedgeidxthd1)
{
	U_V0_SNR_WFYMAX_IDXTH V0_SNR_WFYMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymaxedgeidxthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMAX_IDXTH.bits.wfymaxedgeidxthd1 = wfymaxedgeidxthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymeanedgevalthd3(HI_U32 u32Data, HI_U32 wfymeanedgevalthd3)
{
	U_V0_SNR_WFYMEAN_VALTH V0_SNR_WFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymeanedgevalthd3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMEAN_VALTH.bits.wfymeanedgevalthd3 = wfymeanedgevalthd3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymeanedgevalthd2(HI_U32 u32Data, HI_U32 wfymeanedgevalthd2)
{
	U_V0_SNR_WFYMEAN_VALTH V0_SNR_WFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymeanedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMEAN_VALTH.bits.wfymeanedgevalthd2 = wfymeanedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfymeanedgevalthd1(HI_U32 u32Data, HI_U32 wfymeanedgevalthd1)
{
	U_V0_SNR_WFYMEAN_VALTH V0_SNR_WFYMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfymeanedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFYMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFYMEAN_VALTH.bits.wfymeanedgevalthd1 = wfymeanedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFYMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFYMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmaxedgevalthd2(HI_U32 u32Data, HI_U32 sfcmaxedgevalthd2)
{
	U_V0_SNR_SFCMAX_VALTH V0_SNR_SFCMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmaxedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMAX_VALTH.bits.sfcmaxedgevalthd2 = sfcmaxedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmaxedgevalthd1(HI_U32 u32Data, HI_U32 sfcmaxedgevalthd1)
{
	U_V0_SNR_SFCMAX_VALTH V0_SNR_SFCMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmaxedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMAX_VALTH.bits.sfcmaxedgevalthd1 = sfcmaxedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmaxedgeidxthd2(HI_U32 u32Data, HI_U32 sfcmaxedgeidxthd2)
{
	U_V0_SNR_SFCMAX_IDXTH V0_SNR_SFCMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmaxedgeidxthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMAX_IDXTH.bits.sfcmaxedgeidxthd2 = sfcmaxedgeidxthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmaxedgeidxthd1(HI_U32 u32Data, HI_U32 sfcmaxedgeidxthd1)
{
	U_V0_SNR_SFCMAX_IDXTH V0_SNR_SFCMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmaxedgeidxthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMAX_IDXTH.bits.sfcmaxedgeidxthd1 = sfcmaxedgeidxthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmeanedgevalthd2(HI_U32 u32Data, HI_U32 sfcmeanedgevalthd2)
{
	U_V0_SNR_SFCMEAN_VALTH V0_SNR_SFCMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmeanedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMEAN_VALTH.bits.sfcmeanedgevalthd2 = sfcmeanedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSfcmeanedgevalthd1(HI_U32 u32Data, HI_U32 sfcmeanedgevalthd1)
{
	U_V0_SNR_SFCMEAN_VALTH V0_SNR_SFCMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSfcmeanedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_SFCMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_SFCMEAN_VALTH.bits.sfcmeanedgevalthd1 = sfcmeanedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_SFCMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_SFCMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmaxedgevalthd2(HI_U32 u32Data, HI_U32 wfcmaxedgevalthd2)
{
	U_V0_SNR_WFCMAX_VALTH V0_SNR_WFCMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmaxedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMAX_VALTH.bits.wfcmaxedgevalthd2 = wfcmaxedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmaxedgevalthd1(HI_U32 u32Data, HI_U32 wfcmaxedgevalthd1)
{
	U_V0_SNR_WFCMAX_VALTH V0_SNR_WFCMAX_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmaxedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMAX_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMAX_VALTH.bits.wfcmaxedgevalthd1 = wfcmaxedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMAX_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmaxedgeidxthd2(HI_U32 u32Data, HI_U32 wfcmaxedgeidxthd2)
{
	U_V0_SNR_WFCMAX_IDXTH V0_SNR_WFCMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmaxedgeidxthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMAX_IDXTH.bits.wfcmaxedgeidxthd2 = wfcmaxedgeidxthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmaxedgeidxthd1(HI_U32 u32Data, HI_U32 wfcmaxedgeidxthd1)
{
	U_V0_SNR_WFCMAX_IDXTH V0_SNR_WFCMAX_IDXTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmaxedgeidxthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMAX_IDXTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_IDXTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMAX_IDXTH.bits.wfcmaxedgeidxthd1 = wfcmaxedgeidxthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMAX_IDXTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMAX_IDXTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmeanedgevalthd2(HI_U32 u32Data, HI_U32 wfcmeanedgevalthd2)
{
	U_V0_SNR_WFCMEAN_VALTH V0_SNR_WFCMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmeanedgevalthd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMEAN_VALTH.bits.wfcmeanedgevalthd2 = wfcmeanedgevalthd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWfcmeanedgevalthd1(HI_U32 u32Data, HI_U32 wfcmeanedgevalthd1)
{
	U_V0_SNR_WFCMEAN_VALTH V0_SNR_WFCMEAN_VALTH;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWfcmeanedgevalthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WFCMEAN_VALTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMEAN_VALTH.u32) + u32Data * VID_OFFSET));
	V0_SNR_WFCMEAN_VALTH.bits.wfcmeanedgevalthd1 = wfcmeanedgevalthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WFCMEAN_VALTH.u32) + u32Data * VID_OFFSET),V0_SNR_WFCMEAN_VALTH.u32);
	
	return ;
}


HI_VOID VDP_VID_SetCdefaultfiltertype(HI_U32 u32Data, HI_U32 cdefaultfiltertype)
{
	U_V0_SNR_FILTER_TYPE V0_SNR_FILTER_TYPE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetCdefaultfiltertype Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_FILTER_TYPE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_FILTER_TYPE.u32) + u32Data * VID_OFFSET));
	V0_SNR_FILTER_TYPE.bits.cdefaultfiltertype = cdefaultfiltertype;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_FILTER_TYPE.u32) + u32Data * VID_OFFSET),V0_SNR_FILTER_TYPE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetYdefaultfiltertype(HI_U32 u32Data, HI_U32 ydefaultfiltertype)
{
	U_V0_SNR_FILTER_TYPE V0_SNR_FILTER_TYPE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetYdefaultfiltertype Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_FILTER_TYPE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_FILTER_TYPE.u32) + u32Data * VID_OFFSET));
	V0_SNR_FILTER_TYPE.bits.ydefaultfiltertype = ydefaultfiltertype;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_FILTER_TYPE.u32) + u32Data * VID_OFFSET),V0_SNR_FILTER_TYPE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetNonoisegain(HI_U32 u32Data, HI_U32 nonoisegain)
{
	U_V0_SNR_NOISE_CORE V0_SNR_NOISE_CORE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetNonoisegain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_NOISE_CORE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_NOISE_CORE.u32) + u32Data * VID_OFFSET));
	V0_SNR_NOISE_CORE.bits.nonoisegain = nonoisegain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_NOISE_CORE.u32) + u32Data * VID_OFFSET),V0_SNR_NOISE_CORE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetNoisecore(HI_U32 u32Data, HI_U32 noisecore)
{
	U_V0_SNR_NOISE_CORE V0_SNR_NOISE_CORE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetNoisecore Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_NOISE_CORE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_NOISE_CORE.u32) + u32Data * VID_OFFSET));
	V0_SNR_NOISE_CORE.bits.noisecore = noisecore;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_NOISE_CORE.u32) + u32Data * VID_OFFSET),V0_SNR_NOISE_CORE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetRandyinnoiselevel(HI_U32 u32Data, HI_U32 randyinnoiselevel)
{
	U_V0_SNR_ADD_NOISE V0_SNR_ADD_NOISE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetRandyinnoiselevel Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ADD_NOISE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ADD_NOISE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ADD_NOISE.bits.randyinnoiselevel = randyinnoiselevel;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ADD_NOISE.u32) + u32Data * VID_OFFSET),V0_SNR_ADD_NOISE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAddrandyinnoiseen(HI_U32 u32Data, HI_U32 addrandyinnoiseen)
{
	U_V0_SNR_ADD_NOISE V0_SNR_ADD_NOISE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAddrandyinnoiseen Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ADD_NOISE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ADD_NOISE.u32) + u32Data * VID_OFFSET));
	V0_SNR_ADD_NOISE.bits.addrandyinnoiseen = addrandyinnoiseen;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ADD_NOISE.u32) + u32Data * VID_OFFSET),V0_SNR_ADD_NOISE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDiffabssumofst(HI_U32 u32Data, HI_U32 diffabssumofst)
{
	U_V0_SNR_ABS_SUM V0_SNR_ABS_SUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDiffabssumofst Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ABS_SUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET));
	V0_SNR_ABS_SUM.bits.diffabssumofst = diffabssumofst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET),V0_SNR_ABS_SUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetSclabssum(HI_U32 u32Data, HI_U32 sclabssum)
{
	U_V0_SNR_ABS_SUM V0_SNR_ABS_SUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSclabssum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ABS_SUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET));
	V0_SNR_ABS_SUM.bits.sclabssum = sclabssum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET),V0_SNR_ABS_SUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetEdgeindicatorsel(HI_U32 u32Data, HI_U32 edgeindicatorsel)
{
	U_V0_SNR_ABS_SUM V0_SNR_ABS_SUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetEdgeindicatorsel Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_ABS_SUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET));
	V0_SNR_ABS_SUM.bits.edgeindicatorsel = edgeindicatorsel;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_ABS_SUM.u32) + u32Data * VID_OFFSET),V0_SNR_ABS_SUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinactdirdiffthr(HI_U32 u32Data, HI_U32 winactdirdiffthr)
{
	U_V0_SNR_WIN_ACT V0_SNR_WIN_ACT;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinactdirdiffthr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WIN_ACT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WIN_ACT.u32) + u32Data * VID_OFFSET));
	V0_SNR_WIN_ACT.bits.winactdirdiffthr = winactdirdiffthr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WIN_ACT.u32) + u32Data * VID_OFFSET),V0_SNR_WIN_ACT.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinactthr(HI_U32 u32Data, HI_U32 winactthr)
{
	U_V0_SNR_WIN_ACT V0_SNR_WIN_ACT;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinactthr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_WIN_ACT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_WIN_ACT.u32) + u32Data * VID_OFFSET));
	V0_SNR_WIN_ACT.bits.winactthr = winactthr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_WIN_ACT.u32) + u32Data * VID_OFFSET),V0_SNR_WIN_ACT.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr1(HI_U32 u32Data, HI_U32 pixdirstrthr1)
{
	U_V0_SNR_DIR_THR0 V0_SNR_DIR_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR0.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR0.bits.pixdirstrthr1 = pixdirstrthr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR0.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr0(HI_U32 u32Data, HI_U32 pixdirstrthr0)
{
	U_V0_SNR_DIR_THR0 V0_SNR_DIR_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR0.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR0.bits.pixdirstrthr0 = pixdirstrthr0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR0.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr3(HI_U32 u32Data, HI_U32 pixdirstrthr3)
{
	U_V0_SNR_DIR_THR1 V0_SNR_DIR_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR1.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR1.bits.pixdirstrthr3 = pixdirstrthr3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR1.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr2(HI_U32 u32Data, HI_U32 pixdirstrthr2)
{
	U_V0_SNR_DIR_THR1 V0_SNR_DIR_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR1.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR1.bits.pixdirstrthr2 = pixdirstrthr2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR1.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr5(HI_U32 u32Data, HI_U32 pixdirstrthr5)
{
	U_V0_SNR_DIR_THR2 V0_SNR_DIR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR2.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR2.bits.pixdirstrthr5 = pixdirstrthr5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR2.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr4(HI_U32 u32Data, HI_U32 pixdirstrthr4)
{
	U_V0_SNR_DIR_THR2 V0_SNR_DIR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR2.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR2.bits.pixdirstrthr4 = pixdirstrthr4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR2.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr7(HI_U32 u32Data, HI_U32 pixdirstrthr7)
{
	U_V0_SNR_DIR_THR3 V0_SNR_DIR_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR3.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR3.bits.pixdirstrthr7 = pixdirstrthr7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR3.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetPixdirstrthr6(HI_U32 u32Data, HI_U32 pixdirstrthr6)
{
	U_V0_SNR_DIR_THR3 V0_SNR_DIR_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetPixdirstrthr6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DIR_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR3.u32) + u32Data * VID_OFFSET));
	V0_SNR_DIR_THR3.bits.pixdirstrthr6 = pixdirstrthr6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DIR_THR3.u32) + u32Data * VID_OFFSET),V0_SNR_DIR_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir0(HI_U32 u32Data, HI_U32 mediff3thrdir0)
{
	U_V0_SNR_THR_DIR0A V0_SNR_THR_DIR0A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR0A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR0A.bits.mediff3thrdir0 = mediff3thrdir0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR0A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir0(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir0)
{
	U_V0_SNR_THR_DIR0A V0_SNR_THR_DIR0A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR0A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR0A.bits.mediffcntmintenthrdir0 = mediffcntmintenthrdir0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR0A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir0(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir0)
{
	U_V0_SNR_THR_DIR0B V0_SNR_THR_DIR0B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR0B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR0B.bits.winmeanhordiffthrdir0 = winmeanhordiffthrdir0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR0B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir0(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir0)
{
	U_V0_SNR_THR_DIR0B V0_SNR_THR_DIR0B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR0B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR0B.bits.mediffcntminnorthrdir0 = mediffcntminnorthrdir0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR0B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR0B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir1(HI_U32 u32Data, HI_U32 mediff3thrdir1)
{
	U_V0_SNR_THR_DIR1A V0_SNR_THR_DIR1A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR1A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR1A.bits.mediff3thrdir1 = mediff3thrdir1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR1A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir1(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir1)
{
	U_V0_SNR_THR_DIR1A V0_SNR_THR_DIR1A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR1A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR1A.bits.mediffcntmintenthrdir1 = mediffcntmintenthrdir1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR1A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir1(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir1)
{
	U_V0_SNR_THR_DIR1B V0_SNR_THR_DIR1B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR1B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR1B.bits.winmeanhordiffthrdir1 = winmeanhordiffthrdir1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR1B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir1(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir1)
{
	U_V0_SNR_THR_DIR1B V0_SNR_THR_DIR1B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR1B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR1B.bits.mediffcntminnorthrdir1 = mediffcntminnorthrdir1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR1B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR1B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir2(HI_U32 u32Data, HI_U32 mediff3thrdir2)
{
	U_V0_SNR_THR_DIR2A V0_SNR_THR_DIR2A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR2A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR2A.bits.mediff3thrdir2 = mediff3thrdir2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR2A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir2(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir2)
{
	U_V0_SNR_THR_DIR2A V0_SNR_THR_DIR2A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR2A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR2A.bits.mediffcntmintenthrdir2 = mediffcntmintenthrdir2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR2A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir2(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir2)
{
	U_V0_SNR_THR_DIR2B V0_SNR_THR_DIR2B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR2B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR2B.bits.winmeanhordiffthrdir2 = winmeanhordiffthrdir2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR2B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir2(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir2)
{
	U_V0_SNR_THR_DIR2B V0_SNR_THR_DIR2B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR2B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR2B.bits.mediffcntminnorthrdir2 = mediffcntminnorthrdir2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR2B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR2B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir3(HI_U32 u32Data, HI_U32 mediff3thrdir3)
{
	U_V0_SNR_THR_DIR3A V0_SNR_THR_DIR3A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR3A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR3A.bits.mediff3thrdir3 = mediff3thrdir3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR3A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir3(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir3)
{
	U_V0_SNR_THR_DIR3A V0_SNR_THR_DIR3A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR3A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR3A.bits.mediffcntmintenthrdir3 = mediffcntmintenthrdir3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR3A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir3(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir3)
{
	U_V0_SNR_THR_DIR3B V0_SNR_THR_DIR3B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR3B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR3B.bits.winmeanhordiffthrdir3 = winmeanhordiffthrdir3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR3B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir3(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir3)
{
	U_V0_SNR_THR_DIR3B V0_SNR_THR_DIR3B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR3B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR3B.bits.mediffcntminnorthrdir3 = mediffcntminnorthrdir3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR3B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR3B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir4(HI_U32 u32Data, HI_U32 mediff3thrdir4)
{
	U_V0_SNR_THR_DIR4A V0_SNR_THR_DIR4A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR4A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR4A.bits.mediff3thrdir4 = mediff3thrdir4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR4A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir4(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir4)
{
	U_V0_SNR_THR_DIR4A V0_SNR_THR_DIR4A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR4A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR4A.bits.mediffcntmintenthrdir4 = mediffcntmintenthrdir4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR4A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir4(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir4)
{
	U_V0_SNR_THR_DIR4B V0_SNR_THR_DIR4B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR4B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR4B.bits.winmeanhordiffthrdir4 = winmeanhordiffthrdir4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR4B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir4(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir4)
{
	U_V0_SNR_THR_DIR4B V0_SNR_THR_DIR4B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR4B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR4B.bits.mediffcntminnorthrdir4 = mediffcntminnorthrdir4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR4B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR4B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir5(HI_U32 u32Data, HI_U32 mediff3thrdir5)
{
	U_V0_SNR_THR_DIR5A V0_SNR_THR_DIR5A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR5A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR5A.bits.mediff3thrdir5 = mediff3thrdir5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR5A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir5(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir5)
{
	U_V0_SNR_THR_DIR5A V0_SNR_THR_DIR5A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR5A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR5A.bits.mediffcntmintenthrdir5 = mediffcntmintenthrdir5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR5A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir5(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir5)
{
	U_V0_SNR_THR_DIR5B V0_SNR_THR_DIR5B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR5B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR5B.bits.winmeanhordiffthrdir5 = winmeanhordiffthrdir5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR5B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir5(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir5)
{
	U_V0_SNR_THR_DIR5B V0_SNR_THR_DIR5B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR5B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR5B.bits.mediffcntminnorthrdir5 = mediffcntminnorthrdir5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR5B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR5B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir6(HI_U32 u32Data, HI_U32 mediff3thrdir6)
{
	U_V0_SNR_THR_DIR6A V0_SNR_THR_DIR6A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR6A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR6A.bits.mediff3thrdir6 = mediff3thrdir6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR6A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir6(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir6)
{
	U_V0_SNR_THR_DIR6A V0_SNR_THR_DIR6A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR6A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR6A.bits.mediffcntmintenthrdir6 = mediffcntmintenthrdir6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR6A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir6(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir6)
{
	U_V0_SNR_THR_DIR6B V0_SNR_THR_DIR6B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR6B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR6B.bits.winmeanhordiffthrdir6 = winmeanhordiffthrdir6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR6B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir6(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir6)
{
	U_V0_SNR_THR_DIR6B V0_SNR_THR_DIR6B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR6B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR6B.bits.mediffcntminnorthrdir6 = mediffcntminnorthrdir6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR6B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR6B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediff3thrdir7(HI_U32 u32Data, HI_U32 mediff3thrdir7)
{
	U_V0_SNR_THR_DIR7A V0_SNR_THR_DIR7A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediff3thrdir7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR7A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR7A.bits.mediff3thrdir7 = mediff3thrdir7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR7A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntmintenthrdir7(HI_U32 u32Data, HI_U32 mediffcntmintenthrdir7)
{
	U_V0_SNR_THR_DIR7A V0_SNR_THR_DIR7A;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntmintenthrdir7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR7A.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7A.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR7A.bits.mediffcntmintenthrdir7 = mediffcntmintenthrdir7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7A.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR7A.u32);
	
	return ;
}


HI_VOID VDP_VID_SetWinmeanhordiffthrdir7(HI_U32 u32Data, HI_U32 winmeanhordiffthrdir7)
{
	U_V0_SNR_THR_DIR7B V0_SNR_THR_DIR7B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetWinmeanhordiffthrdir7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR7B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR7B.bits.winmeanhordiffthrdir7 = winmeanhordiffthrdir7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR7B.u32);
	
	return ;
}


HI_VOID VDP_VID_SetMediffcntminnorthrdir7(HI_U32 u32Data, HI_U32 mediffcntminnorthrdir7)
{
	U_V0_SNR_THR_DIR7B V0_SNR_THR_DIR7B;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetMediffcntminnorthrdir7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_THR_DIR7B.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7B.u32) + u32Data * VID_OFFSET));
	V0_SNR_THR_DIR7B.bits.mediffcntminnorthrdir7 = mediffcntminnorthrdir7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_THR_DIR7B.u32) + u32Data * VID_OFFSET),V0_SNR_THR_DIR7B.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDemomodecoor(HI_U32 u32Data, HI_U32 demomodecoor)
{
	U_V0_SNR_DEMO V0_SNR_DEMO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDemomodecoor Select Wrong Layer ID\n");
		return ;
	}
	
	V0_SNR_DEMO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SNR_DEMO.u32) + u32Data * VID_OFFSET));
	V0_SNR_DEMO.bits.demomodecoor = demomodecoor;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SNR_DEMO.u32) + u32Data * VID_OFFSET),V0_SNR_DEMO.u32);
	
	return ;
}

HI_VOID VDP_VID_SetSnrMotionEdgeLut(HI_U32 u32Data, HI_U32 snrmotionedgelut[16][32])
{
    U_V0_MOTION_EDGE_LUT_01a V0_MOTION_EDGE_LUT_01a;
    U_V0_MOTION_EDGE_LUT_01b V0_MOTION_EDGE_LUT_01b;
    U_V0_MOTION_EDGE_LUT_01c V0_MOTION_EDGE_LUT_01c;
    U_V0_MOTION_EDGE_LUT_01d V0_MOTION_EDGE_LUT_01d;
    U_V0_MOTION_EDGE_LUT_01e V0_MOTION_EDGE_LUT_01e;
    U_V0_MOTION_EDGE_LUT_01f V0_MOTION_EDGE_LUT_01f;
    U_V0_MOTION_EDGE_LUT_01g V0_MOTION_EDGE_LUT_01g;
    U_V0_MOTION_EDGE_LUT_01h V0_MOTION_EDGE_LUT_01h;
    U_V0_MOTION_EDGE_LUT_02a V0_MOTION_EDGE_LUT_02a;
    U_V0_MOTION_EDGE_LUT_02b V0_MOTION_EDGE_LUT_02b;
    U_V0_MOTION_EDGE_LUT_02c V0_MOTION_EDGE_LUT_02c;
    U_V0_MOTION_EDGE_LUT_02d V0_MOTION_EDGE_LUT_02d;
    U_V0_MOTION_EDGE_LUT_02e V0_MOTION_EDGE_LUT_02e;
    U_V0_MOTION_EDGE_LUT_02f V0_MOTION_EDGE_LUT_02f;
    U_V0_MOTION_EDGE_LUT_02g V0_MOTION_EDGE_LUT_02g;
    U_V0_MOTION_EDGE_LUT_02h V0_MOTION_EDGE_LUT_02h;
    U_V0_MOTION_EDGE_LUT_03a V0_MOTION_EDGE_LUT_03a;
    U_V0_MOTION_EDGE_LUT_03b V0_MOTION_EDGE_LUT_03b;
    U_V0_MOTION_EDGE_LUT_03c V0_MOTION_EDGE_LUT_03c;
    U_V0_MOTION_EDGE_LUT_03d V0_MOTION_EDGE_LUT_03d;
    U_V0_MOTION_EDGE_LUT_03e V0_MOTION_EDGE_LUT_03e;
    U_V0_MOTION_EDGE_LUT_03f V0_MOTION_EDGE_LUT_03f;
    U_V0_MOTION_EDGE_LUT_03g V0_MOTION_EDGE_LUT_03g;
    U_V0_MOTION_EDGE_LUT_03h V0_MOTION_EDGE_LUT_03h;
    U_V0_MOTION_EDGE_LUT_04a V0_MOTION_EDGE_LUT_04a;
    U_V0_MOTION_EDGE_LUT_04b V0_MOTION_EDGE_LUT_04b;
    U_V0_MOTION_EDGE_LUT_04c V0_MOTION_EDGE_LUT_04c;
    U_V0_MOTION_EDGE_LUT_04d V0_MOTION_EDGE_LUT_04d;
    U_V0_MOTION_EDGE_LUT_04e V0_MOTION_EDGE_LUT_04e;
    U_V0_MOTION_EDGE_LUT_04f V0_MOTION_EDGE_LUT_04f;
    U_V0_MOTION_EDGE_LUT_04g V0_MOTION_EDGE_LUT_04g;
    U_V0_MOTION_EDGE_LUT_04h V0_MOTION_EDGE_LUT_04h;
    U_V0_MOTION_EDGE_LUT_05a V0_MOTION_EDGE_LUT_05a;
    U_V0_MOTION_EDGE_LUT_05b V0_MOTION_EDGE_LUT_05b;
    U_V0_MOTION_EDGE_LUT_05c V0_MOTION_EDGE_LUT_05c;
    U_V0_MOTION_EDGE_LUT_05d V0_MOTION_EDGE_LUT_05d;
    U_V0_MOTION_EDGE_LUT_05e V0_MOTION_EDGE_LUT_05e;
    U_V0_MOTION_EDGE_LUT_05f V0_MOTION_EDGE_LUT_05f;
    U_V0_MOTION_EDGE_LUT_05g V0_MOTION_EDGE_LUT_05g;
    U_V0_MOTION_EDGE_LUT_05h V0_MOTION_EDGE_LUT_05h;
    U_V0_MOTION_EDGE_LUT_06a V0_MOTION_EDGE_LUT_06a;
    U_V0_MOTION_EDGE_LUT_06b V0_MOTION_EDGE_LUT_06b;
    U_V0_MOTION_EDGE_LUT_06c V0_MOTION_EDGE_LUT_06c;
    U_V0_MOTION_EDGE_LUT_06d V0_MOTION_EDGE_LUT_06d;
    U_V0_MOTION_EDGE_LUT_06e V0_MOTION_EDGE_LUT_06e;
    U_V0_MOTION_EDGE_LUT_06f V0_MOTION_EDGE_LUT_06f;
    U_V0_MOTION_EDGE_LUT_06g V0_MOTION_EDGE_LUT_06g;
    U_V0_MOTION_EDGE_LUT_06h V0_MOTION_EDGE_LUT_06h;
    U_V0_MOTION_EDGE_LUT_07a V0_MOTION_EDGE_LUT_07a;
    U_V0_MOTION_EDGE_LUT_07b V0_MOTION_EDGE_LUT_07b;
    U_V0_MOTION_EDGE_LUT_07c V0_MOTION_EDGE_LUT_07c;
    U_V0_MOTION_EDGE_LUT_07d V0_MOTION_EDGE_LUT_07d;
    U_V0_MOTION_EDGE_LUT_07e V0_MOTION_EDGE_LUT_07e;
    U_V0_MOTION_EDGE_LUT_07f V0_MOTION_EDGE_LUT_07f;
    U_V0_MOTION_EDGE_LUT_07g V0_MOTION_EDGE_LUT_07g;
    U_V0_MOTION_EDGE_LUT_07h V0_MOTION_EDGE_LUT_07h;
    U_V0_MOTION_EDGE_LUT_08a V0_MOTION_EDGE_LUT_08a;
    U_V0_MOTION_EDGE_LUT_08b V0_MOTION_EDGE_LUT_08b;
    U_V0_MOTION_EDGE_LUT_08c V0_MOTION_EDGE_LUT_08c;
    U_V0_MOTION_EDGE_LUT_08d V0_MOTION_EDGE_LUT_08d;
    U_V0_MOTION_EDGE_LUT_08e V0_MOTION_EDGE_LUT_08e;
    U_V0_MOTION_EDGE_LUT_08f V0_MOTION_EDGE_LUT_08f;
    U_V0_MOTION_EDGE_LUT_08g V0_MOTION_EDGE_LUT_08g;
    U_V0_MOTION_EDGE_LUT_08h V0_MOTION_EDGE_LUT_08h;
    U_V0_MOTION_EDGE_LUT_09a V0_MOTION_EDGE_LUT_09a;
    U_V0_MOTION_EDGE_LUT_09b V0_MOTION_EDGE_LUT_09b;
    U_V0_MOTION_EDGE_LUT_09c V0_MOTION_EDGE_LUT_09c;
    U_V0_MOTION_EDGE_LUT_09d V0_MOTION_EDGE_LUT_09d;
    U_V0_MOTION_EDGE_LUT_09e V0_MOTION_EDGE_LUT_09e;
    U_V0_MOTION_EDGE_LUT_09f V0_MOTION_EDGE_LUT_09f;
    U_V0_MOTION_EDGE_LUT_09g V0_MOTION_EDGE_LUT_09g;
    U_V0_MOTION_EDGE_LUT_09h V0_MOTION_EDGE_LUT_09h;
    U_V0_MOTION_EDGE_LUT_10a V0_MOTION_EDGE_LUT_10a;
    U_V0_MOTION_EDGE_LUT_10b V0_MOTION_EDGE_LUT_10b;
    U_V0_MOTION_EDGE_LUT_10c V0_MOTION_EDGE_LUT_10c;
    U_V0_MOTION_EDGE_LUT_10d V0_MOTION_EDGE_LUT_10d;
    U_V0_MOTION_EDGE_LUT_10e V0_MOTION_EDGE_LUT_10e;
    U_V0_MOTION_EDGE_LUT_10f V0_MOTION_EDGE_LUT_10f;
    U_V0_MOTION_EDGE_LUT_10g V0_MOTION_EDGE_LUT_10g;
    U_V0_MOTION_EDGE_LUT_10h V0_MOTION_EDGE_LUT_10h;
    U_V0_MOTION_EDGE_LUT_11a V0_MOTION_EDGE_LUT_11a;
    U_V0_MOTION_EDGE_LUT_11b V0_MOTION_EDGE_LUT_11b;
    U_V0_MOTION_EDGE_LUT_11c V0_MOTION_EDGE_LUT_11c;
    U_V0_MOTION_EDGE_LUT_11d V0_MOTION_EDGE_LUT_11d;
    U_V0_MOTION_EDGE_LUT_11e V0_MOTION_EDGE_LUT_11e;
    U_V0_MOTION_EDGE_LUT_11f V0_MOTION_EDGE_LUT_11f;
    U_V0_MOTION_EDGE_LUT_11g V0_MOTION_EDGE_LUT_11g;
    U_V0_MOTION_EDGE_LUT_11h V0_MOTION_EDGE_LUT_11h;
    U_V0_MOTION_EDGE_LUT_12a V0_MOTION_EDGE_LUT_12a;
    U_V0_MOTION_EDGE_LUT_12b V0_MOTION_EDGE_LUT_12b;
    U_V0_MOTION_EDGE_LUT_12c V0_MOTION_EDGE_LUT_12c;
    U_V0_MOTION_EDGE_LUT_12d V0_MOTION_EDGE_LUT_12d;
    U_V0_MOTION_EDGE_LUT_12e V0_MOTION_EDGE_LUT_12e;
    U_V0_MOTION_EDGE_LUT_12f V0_MOTION_EDGE_LUT_12f;
    U_V0_MOTION_EDGE_LUT_12g V0_MOTION_EDGE_LUT_12g;
    U_V0_MOTION_EDGE_LUT_12h V0_MOTION_EDGE_LUT_12h;
    U_V0_MOTION_EDGE_LUT_13a V0_MOTION_EDGE_LUT_13a;
    U_V0_MOTION_EDGE_LUT_13b V0_MOTION_EDGE_LUT_13b;
    U_V0_MOTION_EDGE_LUT_13c V0_MOTION_EDGE_LUT_13c;
    U_V0_MOTION_EDGE_LUT_13d V0_MOTION_EDGE_LUT_13d;
    U_V0_MOTION_EDGE_LUT_13e V0_MOTION_EDGE_LUT_13e;
    U_V0_MOTION_EDGE_LUT_13f V0_MOTION_EDGE_LUT_13f;
    U_V0_MOTION_EDGE_LUT_13g V0_MOTION_EDGE_LUT_13g;
    U_V0_MOTION_EDGE_LUT_13h V0_MOTION_EDGE_LUT_13h;
    U_V0_MOTION_EDGE_LUT_14a V0_MOTION_EDGE_LUT_14a;
    U_V0_MOTION_EDGE_LUT_14b V0_MOTION_EDGE_LUT_14b;
    U_V0_MOTION_EDGE_LUT_14c V0_MOTION_EDGE_LUT_14c;
    U_V0_MOTION_EDGE_LUT_14d V0_MOTION_EDGE_LUT_14d;
    U_V0_MOTION_EDGE_LUT_14e V0_MOTION_EDGE_LUT_14e;
    U_V0_MOTION_EDGE_LUT_14f V0_MOTION_EDGE_LUT_14f;
    U_V0_MOTION_EDGE_LUT_14g V0_MOTION_EDGE_LUT_14g;
    U_V0_MOTION_EDGE_LUT_14h V0_MOTION_EDGE_LUT_14h;
    U_V0_MOTION_EDGE_LUT_15a V0_MOTION_EDGE_LUT_15a;
    U_V0_MOTION_EDGE_LUT_15b V0_MOTION_EDGE_LUT_15b;
    U_V0_MOTION_EDGE_LUT_15c V0_MOTION_EDGE_LUT_15c;
    U_V0_MOTION_EDGE_LUT_15d V0_MOTION_EDGE_LUT_15d;
    U_V0_MOTION_EDGE_LUT_15e V0_MOTION_EDGE_LUT_15e;
    U_V0_MOTION_EDGE_LUT_15f V0_MOTION_EDGE_LUT_15f;
    U_V0_MOTION_EDGE_LUT_15g V0_MOTION_EDGE_LUT_15g;
    U_V0_MOTION_EDGE_LUT_15h V0_MOTION_EDGE_LUT_15h;
    U_V0_MOTION_EDGE_LUT_16a V0_MOTION_EDGE_LUT_16a;
    U_V0_MOTION_EDGE_LUT_16b V0_MOTION_EDGE_LUT_16b;
    U_V0_MOTION_EDGE_LUT_16c V0_MOTION_EDGE_LUT_16c;
    U_V0_MOTION_EDGE_LUT_16d V0_MOTION_EDGE_LUT_16d;
    U_V0_MOTION_EDGE_LUT_16e V0_MOTION_EDGE_LUT_16e;
    U_V0_MOTION_EDGE_LUT_16f V0_MOTION_EDGE_LUT_16f;
    U_V0_MOTION_EDGE_LUT_16g V0_MOTION_EDGE_LUT_16g;
    U_V0_MOTION_EDGE_LUT_16h V0_MOTION_EDGE_LUT_16h;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetSnrMotionEdgeLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_MOTION_EDGE_LUT_16h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_16a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_15a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_14a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_13a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_12a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_11a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_10a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_09a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_08a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_07a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_06a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_05a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_04a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_03a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_02a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01h.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01h.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01g.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01g.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01f.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01f.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01e.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01e.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01d.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01c.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01b.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01a.u32) + u32Data * VID_OFFSET));
    V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__0_  = snrmotionedgelut[0][0];
    V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__1_  = snrmotionedgelut[0][1];
    V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__2_  = snrmotionedgelut[0][2];
    V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__3_  = snrmotionedgelut[0][3];
    V0_MOTION_EDGE_LUT_01b.bits.snrmotionedgelut_0__4_  = snrmotionedgelut[0][4];
    V0_MOTION_EDGE_LUT_01b.bits.snrmotionedgelut_0__5_  = snrmotionedgelut[0][5];
    V0_MOTION_EDGE_LUT_01b.bits.snrmotionedgelut_0__6_  = snrmotionedgelut[0][6];
    V0_MOTION_EDGE_LUT_01b.bits.snrmotionedgelut_0__7_  = snrmotionedgelut[0][7];
    V0_MOTION_EDGE_LUT_01c.bits.snrmotionedgelut_0__8_  = snrmotionedgelut[0][8];
    V0_MOTION_EDGE_LUT_01c.bits.snrmotionedgelut_0__9_  = snrmotionedgelut[0][9];
    V0_MOTION_EDGE_LUT_01c.bits.snrmotionedgelut_0__10_ = snrmotionedgelut[0][10];
    V0_MOTION_EDGE_LUT_01c.bits.snrmotionedgelut_0__11_ = snrmotionedgelut[0][11];
    V0_MOTION_EDGE_LUT_01d.bits.snrmotionedgelut_0__12_ = snrmotionedgelut[0][12];
    V0_MOTION_EDGE_LUT_01d.bits.snrmotionedgelut_0__13_ = snrmotionedgelut[0][13];
    V0_MOTION_EDGE_LUT_01d.bits.snrmotionedgelut_0__14_ = snrmotionedgelut[0][14];
    V0_MOTION_EDGE_LUT_01d.bits.snrmotionedgelut_0__15_ = snrmotionedgelut[0][15];
    V0_MOTION_EDGE_LUT_01e.bits.snrmotionedgelut_0__16_ = snrmotionedgelut[0][16];
    V0_MOTION_EDGE_LUT_01e.bits.snrmotionedgelut_0__17_ = snrmotionedgelut[0][17];
    V0_MOTION_EDGE_LUT_01e.bits.snrmotionedgelut_0__18_ = snrmotionedgelut[0][18];
    V0_MOTION_EDGE_LUT_01e.bits.snrmotionedgelut_0__19_ = snrmotionedgelut[0][19];
    V0_MOTION_EDGE_LUT_01f.bits.snrmotionedgelut_0__20_ = snrmotionedgelut[0][20];
    V0_MOTION_EDGE_LUT_01f.bits.snrmotionedgelut_0__21_ = snrmotionedgelut[0][21];
    V0_MOTION_EDGE_LUT_01f.bits.snrmotionedgelut_0__22_ = snrmotionedgelut[0][22];
    V0_MOTION_EDGE_LUT_01f.bits.snrmotionedgelut_0__23_ = snrmotionedgelut[0][23];
    V0_MOTION_EDGE_LUT_01g.bits.snrmotionedgelut_0__24_ = snrmotionedgelut[0][24];
    V0_MOTION_EDGE_LUT_01g.bits.snrmotionedgelut_0__25_ = snrmotionedgelut[0][25];
    V0_MOTION_EDGE_LUT_01g.bits.snrmotionedgelut_0__26_ = snrmotionedgelut[0][26];
    V0_MOTION_EDGE_LUT_01g.bits.snrmotionedgelut_0__27_ = snrmotionedgelut[0][27];
    V0_MOTION_EDGE_LUT_01h.bits.snrmotionedgelut_0__28_ = snrmotionedgelut[0][28];
    V0_MOTION_EDGE_LUT_01h.bits.snrmotionedgelut_0__29_ = snrmotionedgelut[0][29];
    V0_MOTION_EDGE_LUT_01h.bits.snrmotionedgelut_0__30_ = snrmotionedgelut[0][30];
    V0_MOTION_EDGE_LUT_01h.bits.snrmotionedgelut_0__31_ = snrmotionedgelut[0][31];
    V0_MOTION_EDGE_LUT_02a.bits.snrmotionedgelut_1__0_  = snrmotionedgelut[1][0];
    V0_MOTION_EDGE_LUT_02a.bits.snrmotionedgelut_1__1_  = snrmotionedgelut[1][1];
    V0_MOTION_EDGE_LUT_02a.bits.snrmotionedgelut_1__2_  = snrmotionedgelut[1][2];
    V0_MOTION_EDGE_LUT_02a.bits.snrmotionedgelut_1__3_  = snrmotionedgelut[1][3];
    V0_MOTION_EDGE_LUT_02b.bits.snrmotionedgelut_1__4_  = snrmotionedgelut[1][4];
    V0_MOTION_EDGE_LUT_02b.bits.snrmotionedgelut_1__5_  = snrmotionedgelut[1][5];
    V0_MOTION_EDGE_LUT_02b.bits.snrmotionedgelut_1__6_  = snrmotionedgelut[1][6];
    V0_MOTION_EDGE_LUT_02b.bits.snrmotionedgelut_1__7_  = snrmotionedgelut[1][7];
    V0_MOTION_EDGE_LUT_02c.bits.snrmotionedgelut_1__8_  = snrmotionedgelut[1][8];
    V0_MOTION_EDGE_LUT_02c.bits.snrmotionedgelut_1__9_  = snrmotionedgelut[1][9];
    V0_MOTION_EDGE_LUT_02c.bits.snrmotionedgelut_1__10_ = snrmotionedgelut[1][10];
    V0_MOTION_EDGE_LUT_02c.bits.snrmotionedgelut_1__11_ = snrmotionedgelut[1][11];
    V0_MOTION_EDGE_LUT_02d.bits.snrmotionedgelut_1__12_ = snrmotionedgelut[1][12];
    V0_MOTION_EDGE_LUT_02d.bits.snrmotionedgelut_1__13_ = snrmotionedgelut[1][13];
    V0_MOTION_EDGE_LUT_02d.bits.snrmotionedgelut_1__14_ = snrmotionedgelut[1][14];
    V0_MOTION_EDGE_LUT_02d.bits.snrmotionedgelut_1__15_ = snrmotionedgelut[1][15];
    V0_MOTION_EDGE_LUT_02e.bits.snrmotionedgelut_1__16_ = snrmotionedgelut[1][16];
    V0_MOTION_EDGE_LUT_02e.bits.snrmotionedgelut_1__17_ = snrmotionedgelut[1][17];
    V0_MOTION_EDGE_LUT_02e.bits.snrmotionedgelut_1__18_ = snrmotionedgelut[1][18];
    V0_MOTION_EDGE_LUT_02e.bits.snrmotionedgelut_1__19_ = snrmotionedgelut[1][19];
    V0_MOTION_EDGE_LUT_02f.bits.snrmotionedgelut_1__20_ = snrmotionedgelut[1][20];
    V0_MOTION_EDGE_LUT_02f.bits.snrmotionedgelut_1__21_ = snrmotionedgelut[1][21];
    V0_MOTION_EDGE_LUT_02f.bits.snrmotionedgelut_1__22_ = snrmotionedgelut[1][22];
    V0_MOTION_EDGE_LUT_02f.bits.snrmotionedgelut_1__23_ = snrmotionedgelut[1][23];
    V0_MOTION_EDGE_LUT_02g.bits.snrmotionedgelut_1__24_ = snrmotionedgelut[1][24];
    V0_MOTION_EDGE_LUT_02g.bits.snrmotionedgelut_1__25_ = snrmotionedgelut[1][25];
    V0_MOTION_EDGE_LUT_02g.bits.snrmotionedgelut_1__26_ = snrmotionedgelut[1][26];
    V0_MOTION_EDGE_LUT_02g.bits.snrmotionedgelut_1__27_ = snrmotionedgelut[1][27];
    V0_MOTION_EDGE_LUT_02h.bits.snrmotionedgelut_1__28_ = snrmotionedgelut[1][28];
    V0_MOTION_EDGE_LUT_02h.bits.snrmotionedgelut_1__29_ = snrmotionedgelut[1][29];
    V0_MOTION_EDGE_LUT_02h.bits.snrmotionedgelut_1__30_ = snrmotionedgelut[1][30];
    V0_MOTION_EDGE_LUT_02h.bits.snrmotionedgelut_1__31_ = snrmotionedgelut[1][31];
    V0_MOTION_EDGE_LUT_03a.bits.snrmotionedgelut_2__0_  = snrmotionedgelut[2][0];
    V0_MOTION_EDGE_LUT_03a.bits.snrmotionedgelut_2__1_  = snrmotionedgelut[2][1];
    V0_MOTION_EDGE_LUT_03a.bits.snrmotionedgelut_2__2_  = snrmotionedgelut[2][2];
    V0_MOTION_EDGE_LUT_03a.bits.snrmotionedgelut_2__3_  = snrmotionedgelut[2][3];
    V0_MOTION_EDGE_LUT_03b.bits.snrmotionedgelut_2__4_  = snrmotionedgelut[2][4];
    V0_MOTION_EDGE_LUT_03b.bits.snrmotionedgelut_2__5_  = snrmotionedgelut[2][5];
    V0_MOTION_EDGE_LUT_03b.bits.snrmotionedgelut_2__6_  = snrmotionedgelut[2][6];
    V0_MOTION_EDGE_LUT_03b.bits.snrmotionedgelut_2__7_  = snrmotionedgelut[2][7];
    V0_MOTION_EDGE_LUT_03c.bits.snrmotionedgelut_2__8_  = snrmotionedgelut[2][8];
    V0_MOTION_EDGE_LUT_03c.bits.snrmotionedgelut_2__9_  = snrmotionedgelut[2][9];
    V0_MOTION_EDGE_LUT_03c.bits.snrmotionedgelut_2__10_ = snrmotionedgelut[2][10];
    V0_MOTION_EDGE_LUT_03c.bits.snrmotionedgelut_2__11_ = snrmotionedgelut[2][11];
    V0_MOTION_EDGE_LUT_03d.bits.snrmotionedgelut_2__12_ = snrmotionedgelut[2][12];
    V0_MOTION_EDGE_LUT_03d.bits.snrmotionedgelut_2__13_ = snrmotionedgelut[2][13];
    V0_MOTION_EDGE_LUT_03d.bits.snrmotionedgelut_2__14_ = snrmotionedgelut[2][14];
    V0_MOTION_EDGE_LUT_03d.bits.snrmotionedgelut_2__15_ = snrmotionedgelut[2][15];
    V0_MOTION_EDGE_LUT_03e.bits.snrmotionedgelut_2__16_ = snrmotionedgelut[2][16];
    V0_MOTION_EDGE_LUT_03e.bits.snrmotionedgelut_2__17_ = snrmotionedgelut[2][17];
    V0_MOTION_EDGE_LUT_03e.bits.snrmotionedgelut_2__18_ = snrmotionedgelut[2][18];
    V0_MOTION_EDGE_LUT_03e.bits.snrmotionedgelut_2__19_ = snrmotionedgelut[2][19];
    V0_MOTION_EDGE_LUT_03f.bits.snrmotionedgelut_2__20_ = snrmotionedgelut[2][20];
    V0_MOTION_EDGE_LUT_03f.bits.snrmotionedgelut_2__21_ = snrmotionedgelut[2][21];
    V0_MOTION_EDGE_LUT_03f.bits.snrmotionedgelut_2__22_ = snrmotionedgelut[2][22];
    V0_MOTION_EDGE_LUT_03f.bits.snrmotionedgelut_2__23_ = snrmotionedgelut[2][23];
    V0_MOTION_EDGE_LUT_03g.bits.snrmotionedgelut_2__24_ = snrmotionedgelut[2][24];
    V0_MOTION_EDGE_LUT_03g.bits.snrmotionedgelut_2__25_ = snrmotionedgelut[2][25];
    V0_MOTION_EDGE_LUT_03g.bits.snrmotionedgelut_2__26_ = snrmotionedgelut[2][26];
    V0_MOTION_EDGE_LUT_03g.bits.snrmotionedgelut_2__27_ = snrmotionedgelut[2][27];
    V0_MOTION_EDGE_LUT_03h.bits.snrmotionedgelut_2__28_ = snrmotionedgelut[2][28];
    V0_MOTION_EDGE_LUT_03h.bits.snrmotionedgelut_2__29_ = snrmotionedgelut[2][29];
    V0_MOTION_EDGE_LUT_03h.bits.snrmotionedgelut_2__30_ = snrmotionedgelut[2][30];
    V0_MOTION_EDGE_LUT_03h.bits.snrmotionedgelut_2__31_ = snrmotionedgelut[2][31];
    V0_MOTION_EDGE_LUT_04a.bits.snrmotionedgelut_3__0_  = snrmotionedgelut[3][0];
    V0_MOTION_EDGE_LUT_04a.bits.snrmotionedgelut_3__1_  = snrmotionedgelut[3][1];
    V0_MOTION_EDGE_LUT_04a.bits.snrmotionedgelut_3__2_  = snrmotionedgelut[3][2];
    V0_MOTION_EDGE_LUT_04a.bits.snrmotionedgelut_3__3_  = snrmotionedgelut[3][3];
    V0_MOTION_EDGE_LUT_04b.bits.snrmotionedgelut_3__4_  = snrmotionedgelut[3][4];
    V0_MOTION_EDGE_LUT_04b.bits.snrmotionedgelut_3__5_  = snrmotionedgelut[3][5];
    V0_MOTION_EDGE_LUT_04b.bits.snrmotionedgelut_3__6_  = snrmotionedgelut[3][6];
    V0_MOTION_EDGE_LUT_04b.bits.snrmotionedgelut_3__7_  = snrmotionedgelut[3][7];
    V0_MOTION_EDGE_LUT_04c.bits.snrmotionedgelut_3__8_  = snrmotionedgelut[3][8];
    V0_MOTION_EDGE_LUT_04c.bits.snrmotionedgelut_3__9_  = snrmotionedgelut[3][9];
    V0_MOTION_EDGE_LUT_04c.bits.snrmotionedgelut_3__10_ = snrmotionedgelut[3][10];
    V0_MOTION_EDGE_LUT_04c.bits.snrmotionedgelut_3__11_ = snrmotionedgelut[3][11];
    V0_MOTION_EDGE_LUT_04d.bits.snrmotionedgelut_3__12_ = snrmotionedgelut[3][12];
    V0_MOTION_EDGE_LUT_04d.bits.snrmotionedgelut_3__13_ = snrmotionedgelut[3][13];
    V0_MOTION_EDGE_LUT_04d.bits.snrmotionedgelut_3__14_ = snrmotionedgelut[3][14];
    V0_MOTION_EDGE_LUT_04d.bits.snrmotionedgelut_3__15_ = snrmotionedgelut[3][15];
    V0_MOTION_EDGE_LUT_04e.bits.snrmotionedgelut_3__16_ = snrmotionedgelut[3][16];
    V0_MOTION_EDGE_LUT_04e.bits.snrmotionedgelut_3__17_ = snrmotionedgelut[3][17];
    V0_MOTION_EDGE_LUT_04e.bits.snrmotionedgelut_3__18_ = snrmotionedgelut[3][18];
    V0_MOTION_EDGE_LUT_04e.bits.snrmotionedgelut_3__19_ = snrmotionedgelut[3][19];
    V0_MOTION_EDGE_LUT_04f.bits.snrmotionedgelut_3__20_ = snrmotionedgelut[3][20];
    V0_MOTION_EDGE_LUT_04f.bits.snrmotionedgelut_3__21_ = snrmotionedgelut[3][21];
    V0_MOTION_EDGE_LUT_04f.bits.snrmotionedgelut_3__22_ = snrmotionedgelut[3][22];
    V0_MOTION_EDGE_LUT_04f.bits.snrmotionedgelut_3__23_ = snrmotionedgelut[3][23];
    V0_MOTION_EDGE_LUT_04g.bits.snrmotionedgelut_3__24_ = snrmotionedgelut[3][24];
    V0_MOTION_EDGE_LUT_04g.bits.snrmotionedgelut_3__25_ = snrmotionedgelut[3][25];
    V0_MOTION_EDGE_LUT_04g.bits.snrmotionedgelut_3__26_ = snrmotionedgelut[3][26];
    V0_MOTION_EDGE_LUT_04g.bits.snrmotionedgelut_3__27_ = snrmotionedgelut[3][27];
    V0_MOTION_EDGE_LUT_04h.bits.snrmotionedgelut_3__28_ = snrmotionedgelut[3][28];
    V0_MOTION_EDGE_LUT_04h.bits.snrmotionedgelut_3__29_ = snrmotionedgelut[3][29];
    V0_MOTION_EDGE_LUT_04h.bits.snrmotionedgelut_3__30_ = snrmotionedgelut[3][30];
    V0_MOTION_EDGE_LUT_04h.bits.snrmotionedgelut_3__31_ = snrmotionedgelut[3][31];
    V0_MOTION_EDGE_LUT_05a.bits.snrmotionedgelut_4__0_  = snrmotionedgelut[4][0];
    V0_MOTION_EDGE_LUT_05a.bits.snrmotionedgelut_4__1_  = snrmotionedgelut[4][1];
    V0_MOTION_EDGE_LUT_05a.bits.snrmotionedgelut_4__2_  = snrmotionedgelut[4][2];
    V0_MOTION_EDGE_LUT_05a.bits.snrmotionedgelut_4__3_  = snrmotionedgelut[4][3];
    V0_MOTION_EDGE_LUT_05b.bits.snrmotionedgelut_4__4_  = snrmotionedgelut[4][4];
    V0_MOTION_EDGE_LUT_05b.bits.snrmotionedgelut_4__5_  = snrmotionedgelut[4][5];
    V0_MOTION_EDGE_LUT_05b.bits.snrmotionedgelut_4__6_  = snrmotionedgelut[4][6];
    V0_MOTION_EDGE_LUT_05b.bits.snrmotionedgelut_4__7_  = snrmotionedgelut[4][7];
    V0_MOTION_EDGE_LUT_05c.bits.snrmotionedgelut_4__8_  = snrmotionedgelut[4][8];
    V0_MOTION_EDGE_LUT_05c.bits.snrmotionedgelut_4__9_  = snrmotionedgelut[4][9];
    V0_MOTION_EDGE_LUT_05c.bits.snrmotionedgelut_4__10_ = snrmotionedgelut[4][10];
    V0_MOTION_EDGE_LUT_05c.bits.snrmotionedgelut_4__11_ = snrmotionedgelut[4][11];
    V0_MOTION_EDGE_LUT_05d.bits.snrmotionedgelut_4__12_ = snrmotionedgelut[4][12];
    V0_MOTION_EDGE_LUT_05d.bits.snrmotionedgelut_4__13_ = snrmotionedgelut[4][13];
    V0_MOTION_EDGE_LUT_05d.bits.snrmotionedgelut_4__14_ = snrmotionedgelut[4][14];
    V0_MOTION_EDGE_LUT_05d.bits.snrmotionedgelut_4__15_ = snrmotionedgelut[4][15];
    V0_MOTION_EDGE_LUT_05e.bits.snrmotionedgelut_4__16_ = snrmotionedgelut[4][16];
    V0_MOTION_EDGE_LUT_05e.bits.snrmotionedgelut_4__17_ = snrmotionedgelut[4][17];
    V0_MOTION_EDGE_LUT_05e.bits.snrmotionedgelut_4__18_ = snrmotionedgelut[4][18];
    V0_MOTION_EDGE_LUT_05e.bits.snrmotionedgelut_4__19_ = snrmotionedgelut[4][19];
    V0_MOTION_EDGE_LUT_05f.bits.snrmotionedgelut_4__20_ = snrmotionedgelut[4][20];
    V0_MOTION_EDGE_LUT_05f.bits.snrmotionedgelut_4__21_ = snrmotionedgelut[4][21];
    V0_MOTION_EDGE_LUT_05f.bits.snrmotionedgelut_4__22_ = snrmotionedgelut[4][22];
    V0_MOTION_EDGE_LUT_05f.bits.snrmotionedgelut_4__23_ = snrmotionedgelut[4][23];
    V0_MOTION_EDGE_LUT_05g.bits.snrmotionedgelut_4__24_ = snrmotionedgelut[4][24];
    V0_MOTION_EDGE_LUT_05g.bits.snrmotionedgelut_4__25_ = snrmotionedgelut[4][25];
    V0_MOTION_EDGE_LUT_05g.bits.snrmotionedgelut_4__26_ = snrmotionedgelut[4][26];
    V0_MOTION_EDGE_LUT_05g.bits.snrmotionedgelut_4__27_ = snrmotionedgelut[4][27];
    V0_MOTION_EDGE_LUT_05h.bits.snrmotionedgelut_4__28_ = snrmotionedgelut[4][28];
    V0_MOTION_EDGE_LUT_05h.bits.snrmotionedgelut_4__29_ = snrmotionedgelut[4][29];
    V0_MOTION_EDGE_LUT_05h.bits.snrmotionedgelut_4__30_ = snrmotionedgelut[4][30];
    V0_MOTION_EDGE_LUT_05h.bits.snrmotionedgelut_4__31_ = snrmotionedgelut[4][31];
    V0_MOTION_EDGE_LUT_06a.bits.snrmotionedgelut_5__0_  = snrmotionedgelut[5][0];
    V0_MOTION_EDGE_LUT_06a.bits.snrmotionedgelut_5__1_  = snrmotionedgelut[5][1];
    V0_MOTION_EDGE_LUT_06a.bits.snrmotionedgelut_5__2_  = snrmotionedgelut[5][2];
    V0_MOTION_EDGE_LUT_06a.bits.snrmotionedgelut_5__3_  = snrmotionedgelut[5][3];
    V0_MOTION_EDGE_LUT_06b.bits.snrmotionedgelut_5__4_  = snrmotionedgelut[5][4];
    V0_MOTION_EDGE_LUT_06b.bits.snrmotionedgelut_5__5_  = snrmotionedgelut[5][5];
    V0_MOTION_EDGE_LUT_06b.bits.snrmotionedgelut_5__6_  = snrmotionedgelut[5][6];
    V0_MOTION_EDGE_LUT_06b.bits.snrmotionedgelut_5__7_  = snrmotionedgelut[5][7];
    V0_MOTION_EDGE_LUT_06c.bits.snrmotionedgelut_5__8_  = snrmotionedgelut[5][8];
    V0_MOTION_EDGE_LUT_06c.bits.snrmotionedgelut_5__9_  = snrmotionedgelut[5][9];
    V0_MOTION_EDGE_LUT_06c.bits.snrmotionedgelut_5__10_ = snrmotionedgelut[5][10];
    V0_MOTION_EDGE_LUT_06c.bits.snrmotionedgelut_5__11_ = snrmotionedgelut[5][11];
    V0_MOTION_EDGE_LUT_06d.bits.snrmotionedgelut_5__12_ = snrmotionedgelut[5][12];
    V0_MOTION_EDGE_LUT_06d.bits.snrmotionedgelut_5__13_ = snrmotionedgelut[5][13];
    V0_MOTION_EDGE_LUT_06d.bits.snrmotionedgelut_5__14_ = snrmotionedgelut[5][14];
    V0_MOTION_EDGE_LUT_06d.bits.snrmotionedgelut_5__15_ = snrmotionedgelut[5][15];
    V0_MOTION_EDGE_LUT_06e.bits.snrmotionedgelut_5__16_ = snrmotionedgelut[5][16];
    V0_MOTION_EDGE_LUT_06e.bits.snrmotionedgelut_5__17_ = snrmotionedgelut[5][17];
    V0_MOTION_EDGE_LUT_06e.bits.snrmotionedgelut_5__18_ = snrmotionedgelut[5][18];
    V0_MOTION_EDGE_LUT_06e.bits.snrmotionedgelut_5__19_ = snrmotionedgelut[5][19];
    V0_MOTION_EDGE_LUT_06f.bits.snrmotionedgelut_5__20_ = snrmotionedgelut[5][20];
    V0_MOTION_EDGE_LUT_06f.bits.snrmotionedgelut_5__21_ = snrmotionedgelut[5][21];
    V0_MOTION_EDGE_LUT_06f.bits.snrmotionedgelut_5__22_ = snrmotionedgelut[5][22];
    V0_MOTION_EDGE_LUT_06f.bits.snrmotionedgelut_5__23_ = snrmotionedgelut[5][23];
    V0_MOTION_EDGE_LUT_06g.bits.snrmotionedgelut_5__24_ = snrmotionedgelut[5][24];
    V0_MOTION_EDGE_LUT_06g.bits.snrmotionedgelut_5__25_ = snrmotionedgelut[5][25];
    V0_MOTION_EDGE_LUT_06g.bits.snrmotionedgelut_5__26_ = snrmotionedgelut[5][26];
    V0_MOTION_EDGE_LUT_06g.bits.snrmotionedgelut_5__27_ = snrmotionedgelut[5][27];
    V0_MOTION_EDGE_LUT_06h.bits.snrmotionedgelut_5__28_ = snrmotionedgelut[5][28];
    V0_MOTION_EDGE_LUT_06h.bits.snrmotionedgelut_5__29_ = snrmotionedgelut[5][29];
    V0_MOTION_EDGE_LUT_06h.bits.snrmotionedgelut_5__30_ = snrmotionedgelut[5][30];
    V0_MOTION_EDGE_LUT_06h.bits.snrmotionedgelut_5__31_ = snrmotionedgelut[5][31];
    V0_MOTION_EDGE_LUT_07a.bits.snrmotionedgelut_6__0_  = snrmotionedgelut[6][0];
    V0_MOTION_EDGE_LUT_07a.bits.snrmotionedgelut_6__1_  = snrmotionedgelut[6][1];
    V0_MOTION_EDGE_LUT_07a.bits.snrmotionedgelut_6__2_  = snrmotionedgelut[6][2];
    V0_MOTION_EDGE_LUT_07a.bits.snrmotionedgelut_6__3_  = snrmotionedgelut[6][3];
    V0_MOTION_EDGE_LUT_07b.bits.snrmotionedgelut_6__4_  = snrmotionedgelut[6][4];
    V0_MOTION_EDGE_LUT_07b.bits.snrmotionedgelut_6__5_  = snrmotionedgelut[6][5];
    V0_MOTION_EDGE_LUT_07b.bits.snrmotionedgelut_6__6_  = snrmotionedgelut[6][6];
    V0_MOTION_EDGE_LUT_07b.bits.snrmotionedgelut_6__7_  = snrmotionedgelut[6][7];
    V0_MOTION_EDGE_LUT_07c.bits.snrmotionedgelut_6__8_  = snrmotionedgelut[6][8];
    V0_MOTION_EDGE_LUT_07c.bits.snrmotionedgelut_6__9_  = snrmotionedgelut[6][9];
    V0_MOTION_EDGE_LUT_07c.bits.snrmotionedgelut_6__10_ = snrmotionedgelut[6][10];
    V0_MOTION_EDGE_LUT_07c.bits.snrmotionedgelut_6__11_ = snrmotionedgelut[6][11];
    V0_MOTION_EDGE_LUT_07d.bits.snrmotionedgelut_6__12_ = snrmotionedgelut[6][12];
    V0_MOTION_EDGE_LUT_07d.bits.snrmotionedgelut_6__13_ = snrmotionedgelut[6][13];
    V0_MOTION_EDGE_LUT_07d.bits.snrmotionedgelut_6__14_ = snrmotionedgelut[6][14];
    V0_MOTION_EDGE_LUT_07d.bits.snrmotionedgelut_6__15_ = snrmotionedgelut[6][15];
    V0_MOTION_EDGE_LUT_07e.bits.snrmotionedgelut_6__16_ = snrmotionedgelut[6][16];
    V0_MOTION_EDGE_LUT_07e.bits.snrmotionedgelut_6__17_ = snrmotionedgelut[6][17];
    V0_MOTION_EDGE_LUT_07e.bits.snrmotionedgelut_6__18_ = snrmotionedgelut[6][18];
    V0_MOTION_EDGE_LUT_07e.bits.snrmotionedgelut_6__19_ = snrmotionedgelut[6][19];
    V0_MOTION_EDGE_LUT_07f.bits.snrmotionedgelut_6__20_ = snrmotionedgelut[6][20];
    V0_MOTION_EDGE_LUT_07f.bits.snrmotionedgelut_6__21_ = snrmotionedgelut[6][21];
    V0_MOTION_EDGE_LUT_07f.bits.snrmotionedgelut_6__22_ = snrmotionedgelut[6][22];
    V0_MOTION_EDGE_LUT_07f.bits.snrmotionedgelut_6__23_ = snrmotionedgelut[6][23];
    V0_MOTION_EDGE_LUT_07g.bits.snrmotionedgelut_6__24_ = snrmotionedgelut[6][24];
    V0_MOTION_EDGE_LUT_07g.bits.snrmotionedgelut_6__25_ = snrmotionedgelut[6][25];
    V0_MOTION_EDGE_LUT_07g.bits.snrmotionedgelut_6__26_ = snrmotionedgelut[6][26];
    V0_MOTION_EDGE_LUT_07g.bits.snrmotionedgelut_6__27_ = snrmotionedgelut[6][27];
    V0_MOTION_EDGE_LUT_07h.bits.snrmotionedgelut_6__28_ = snrmotionedgelut[6][28];
    V0_MOTION_EDGE_LUT_07h.bits.snrmotionedgelut_6__29_ = snrmotionedgelut[6][29];
    V0_MOTION_EDGE_LUT_07h.bits.snrmotionedgelut_6__30_ = snrmotionedgelut[6][30];
    V0_MOTION_EDGE_LUT_07h.bits.snrmotionedgelut_6__31_ = snrmotionedgelut[6][31];
    V0_MOTION_EDGE_LUT_08a.bits.snrmotionedgelut_7__0_  = snrmotionedgelut[7][0];
    V0_MOTION_EDGE_LUT_08a.bits.snrmotionedgelut_7__1_  = snrmotionedgelut[7][1];
    V0_MOTION_EDGE_LUT_08a.bits.snrmotionedgelut_7__2_  = snrmotionedgelut[7][2];
    V0_MOTION_EDGE_LUT_08a.bits.snrmotionedgelut_7__3_  = snrmotionedgelut[7][3];
    V0_MOTION_EDGE_LUT_08b.bits.snrmotionedgelut_7__4_  = snrmotionedgelut[7][4];
    V0_MOTION_EDGE_LUT_08b.bits.snrmotionedgelut_7__5_  = snrmotionedgelut[7][5];
    V0_MOTION_EDGE_LUT_08b.bits.snrmotionedgelut_7__6_  = snrmotionedgelut[7][6];
    V0_MOTION_EDGE_LUT_08b.bits.snrmotionedgelut_7__7_  = snrmotionedgelut[7][7];
    V0_MOTION_EDGE_LUT_08c.bits.snrmotionedgelut_7__8_  = snrmotionedgelut[7][8];
    V0_MOTION_EDGE_LUT_08c.bits.snrmotionedgelut_7__9_  = snrmotionedgelut[7][9];
    V0_MOTION_EDGE_LUT_08c.bits.snrmotionedgelut_7__10_ = snrmotionedgelut[7][10];
    V0_MOTION_EDGE_LUT_08c.bits.snrmotionedgelut_7__11_ = snrmotionedgelut[7][11];
    V0_MOTION_EDGE_LUT_08d.bits.snrmotionedgelut_7__12_ = snrmotionedgelut[7][12];
    V0_MOTION_EDGE_LUT_08d.bits.snrmotionedgelut_7__13_ = snrmotionedgelut[7][13];
    V0_MOTION_EDGE_LUT_08d.bits.snrmotionedgelut_7__14_ = snrmotionedgelut[7][14];
    V0_MOTION_EDGE_LUT_08d.bits.snrmotionedgelut_7__15_ = snrmotionedgelut[7][15];
    V0_MOTION_EDGE_LUT_08e.bits.snrmotionedgelut_7__16_ = snrmotionedgelut[7][16];
    V0_MOTION_EDGE_LUT_08e.bits.snrmotionedgelut_7__17_ = snrmotionedgelut[7][17];
    V0_MOTION_EDGE_LUT_08e.bits.snrmotionedgelut_7__18_ = snrmotionedgelut[7][18];
    V0_MOTION_EDGE_LUT_08e.bits.snrmotionedgelut_7__19_ = snrmotionedgelut[7][19];
    V0_MOTION_EDGE_LUT_08f.bits.snrmotionedgelut_7__20_ = snrmotionedgelut[7][20];
    V0_MOTION_EDGE_LUT_08f.bits.snrmotionedgelut_7__21_ = snrmotionedgelut[7][21];
    V0_MOTION_EDGE_LUT_08f.bits.snrmotionedgelut_7__22_ = snrmotionedgelut[7][22];
    V0_MOTION_EDGE_LUT_08f.bits.snrmotionedgelut_7__23_ = snrmotionedgelut[7][23];
    V0_MOTION_EDGE_LUT_08g.bits.snrmotionedgelut_7__24_ = snrmotionedgelut[7][24];
    V0_MOTION_EDGE_LUT_08g.bits.snrmotionedgelut_7__25_ = snrmotionedgelut[7][25];
    V0_MOTION_EDGE_LUT_08g.bits.snrmotionedgelut_7__26_ = snrmotionedgelut[7][26];
    V0_MOTION_EDGE_LUT_08g.bits.snrmotionedgelut_7__27_ = snrmotionedgelut[7][27];
    V0_MOTION_EDGE_LUT_08h.bits.snrmotionedgelut_7__28_ = snrmotionedgelut[7][28];
    V0_MOTION_EDGE_LUT_08h.bits.snrmotionedgelut_7__29_ = snrmotionedgelut[7][29];
    V0_MOTION_EDGE_LUT_08h.bits.snrmotionedgelut_7__30_ = snrmotionedgelut[7][30];
    V0_MOTION_EDGE_LUT_08h.bits.snrmotionedgelut_7__31_ = snrmotionedgelut[7][31];
    V0_MOTION_EDGE_LUT_09a.bits.snrmotionedgelut_8__0_  = snrmotionedgelut[8][0];
    V0_MOTION_EDGE_LUT_09a.bits.snrmotionedgelut_8__1_  = snrmotionedgelut[8][1];
    V0_MOTION_EDGE_LUT_09a.bits.snrmotionedgelut_8__2_  = snrmotionedgelut[8][2];
    V0_MOTION_EDGE_LUT_09a.bits.snrmotionedgelut_8__3_  = snrmotionedgelut[8][3];
    V0_MOTION_EDGE_LUT_09b.bits.snrmotionedgelut_8__4_  = snrmotionedgelut[8][4];
    V0_MOTION_EDGE_LUT_09b.bits.snrmotionedgelut_8__5_  = snrmotionedgelut[8][5];
    V0_MOTION_EDGE_LUT_09b.bits.snrmotionedgelut_8__6_  = snrmotionedgelut[8][6];
    V0_MOTION_EDGE_LUT_09b.bits.snrmotionedgelut_8__7_  = snrmotionedgelut[8][7];
    V0_MOTION_EDGE_LUT_09c.bits.snrmotionedgelut_8__8_  = snrmotionedgelut[8][8];
    V0_MOTION_EDGE_LUT_09c.bits.snrmotionedgelut_8__9_  = snrmotionedgelut[8][9];
    V0_MOTION_EDGE_LUT_09c.bits.snrmotionedgelut_8__10_ = snrmotionedgelut[8][10];
    V0_MOTION_EDGE_LUT_09c.bits.snrmotionedgelut_8__11_ = snrmotionedgelut[8][11];
    V0_MOTION_EDGE_LUT_09d.bits.snrmotionedgelut_8__12_ = snrmotionedgelut[8][12];
    V0_MOTION_EDGE_LUT_09d.bits.snrmotionedgelut_8__13_ = snrmotionedgelut[8][13];
    V0_MOTION_EDGE_LUT_09d.bits.snrmotionedgelut_8__14_ = snrmotionedgelut[8][14];
    V0_MOTION_EDGE_LUT_09d.bits.snrmotionedgelut_8__15_ = snrmotionedgelut[8][15];
    V0_MOTION_EDGE_LUT_09e.bits.snrmotionedgelut_8__16_ = snrmotionedgelut[8][16];
    V0_MOTION_EDGE_LUT_09e.bits.snrmotionedgelut_8__17_ = snrmotionedgelut[8][17];
    V0_MOTION_EDGE_LUT_09e.bits.snrmotionedgelut_8__18_ = snrmotionedgelut[8][18];
    V0_MOTION_EDGE_LUT_09e.bits.snrmotionedgelut_8__19_ = snrmotionedgelut[8][19];
    V0_MOTION_EDGE_LUT_09f.bits.snrmotionedgelut_8__20_ = snrmotionedgelut[8][20];
    V0_MOTION_EDGE_LUT_09f.bits.snrmotionedgelut_8__21_ = snrmotionedgelut[8][21];
    V0_MOTION_EDGE_LUT_09f.bits.snrmotionedgelut_8__22_ = snrmotionedgelut[8][22];
    V0_MOTION_EDGE_LUT_09f.bits.snrmotionedgelut_8__23_ = snrmotionedgelut[8][23];
    V0_MOTION_EDGE_LUT_09g.bits.snrmotionedgelut_8__24_ = snrmotionedgelut[8][24];
    V0_MOTION_EDGE_LUT_09g.bits.snrmotionedgelut_8__25_ = snrmotionedgelut[8][25];
    V0_MOTION_EDGE_LUT_09g.bits.snrmotionedgelut_8__26_ = snrmotionedgelut[8][26];
    V0_MOTION_EDGE_LUT_09g.bits.snrmotionedgelut_8__27_ = snrmotionedgelut[8][27];
    V0_MOTION_EDGE_LUT_09h.bits.snrmotionedgelut_8__28_ = snrmotionedgelut[8][28];
    V0_MOTION_EDGE_LUT_09h.bits.snrmotionedgelut_8__29_ = snrmotionedgelut[8][29];
    V0_MOTION_EDGE_LUT_09h.bits.snrmotionedgelut_8__30_ = snrmotionedgelut[8][30];
    V0_MOTION_EDGE_LUT_09h.bits.snrmotionedgelut_8__31_ = snrmotionedgelut[8][31];
    V0_MOTION_EDGE_LUT_10a.bits.snrmotionedgelut_9__0_  = snrmotionedgelut[9][0];
    V0_MOTION_EDGE_LUT_10a.bits.snrmotionedgelut_9__1_  = snrmotionedgelut[9][1];
    V0_MOTION_EDGE_LUT_10a.bits.snrmotionedgelut_9__2_  = snrmotionedgelut[9][2];
    V0_MOTION_EDGE_LUT_10a.bits.snrmotionedgelut_9__3_  = snrmotionedgelut[9][3];
    V0_MOTION_EDGE_LUT_10b.bits.snrmotionedgelut_9__4_  = snrmotionedgelut[9][4];
    V0_MOTION_EDGE_LUT_10b.bits.snrmotionedgelut_9__5_  = snrmotionedgelut[9][5];
    V0_MOTION_EDGE_LUT_10b.bits.snrmotionedgelut_9__6_  = snrmotionedgelut[9][6];
    V0_MOTION_EDGE_LUT_10b.bits.snrmotionedgelut_9__7_  = snrmotionedgelut[9][7];
    V0_MOTION_EDGE_LUT_10c.bits.snrmotionedgelut_9__8_  = snrmotionedgelut[9][8];
    V0_MOTION_EDGE_LUT_10c.bits.snrmotionedgelut_9__9_  = snrmotionedgelut[9][9];
    V0_MOTION_EDGE_LUT_10c.bits.snrmotionedgelut_9__10_ = snrmotionedgelut[9][10];
    V0_MOTION_EDGE_LUT_10c.bits.snrmotionedgelut_9__11_ = snrmotionedgelut[9][11];
    V0_MOTION_EDGE_LUT_10d.bits.snrmotionedgelut_9__12_ = snrmotionedgelut[9][12];
    V0_MOTION_EDGE_LUT_10d.bits.snrmotionedgelut_9__13_ = snrmotionedgelut[9][13];
    V0_MOTION_EDGE_LUT_10d.bits.snrmotionedgelut_9__14_ = snrmotionedgelut[9][14];
    V0_MOTION_EDGE_LUT_10d.bits.snrmotionedgelut_9__15_ = snrmotionedgelut[9][15];
    V0_MOTION_EDGE_LUT_10e.bits.snrmotionedgelut_9__16_ = snrmotionedgelut[9][16];
    V0_MOTION_EDGE_LUT_10e.bits.snrmotionedgelut_9__17_ = snrmotionedgelut[9][17];
    V0_MOTION_EDGE_LUT_10e.bits.snrmotionedgelut_9__18_ = snrmotionedgelut[9][18];
    V0_MOTION_EDGE_LUT_10e.bits.snrmotionedgelut_9__19_ = snrmotionedgelut[9][19];
    V0_MOTION_EDGE_LUT_10f.bits.snrmotionedgelut_9__20_ = snrmotionedgelut[9][20];
    V0_MOTION_EDGE_LUT_10f.bits.snrmotionedgelut_9__21_ = snrmotionedgelut[9][21];
    V0_MOTION_EDGE_LUT_10f.bits.snrmotionedgelut_9__22_ = snrmotionedgelut[9][22];
    V0_MOTION_EDGE_LUT_10f.bits.snrmotionedgelut_9__23_ = snrmotionedgelut[9][23];
    V0_MOTION_EDGE_LUT_10g.bits.snrmotionedgelut_9__24_ = snrmotionedgelut[9][24];
    V0_MOTION_EDGE_LUT_10g.bits.snrmotionedgelut_9__25_ = snrmotionedgelut[9][25];
    V0_MOTION_EDGE_LUT_10g.bits.snrmotionedgelut_9__26_ = snrmotionedgelut[9][26];
    V0_MOTION_EDGE_LUT_10g.bits.snrmotionedgelut_9__27_ = snrmotionedgelut[9][27];
    V0_MOTION_EDGE_LUT_10h.bits.snrmotionedgelut_9__28_ = snrmotionedgelut[9][28];
    V0_MOTION_EDGE_LUT_10h.bits.snrmotionedgelut_9__29_ = snrmotionedgelut[9][29];
    V0_MOTION_EDGE_LUT_10h.bits.snrmotionedgelut_9__30_ = snrmotionedgelut[9][30];
    V0_MOTION_EDGE_LUT_10h.bits.snrmotionedgelut_9__31_ = snrmotionedgelut[9][31];
    V0_MOTION_EDGE_LUT_11a.bits.snrmotionedgelut_10__0_ = snrmotionedgelut[10][0];
    V0_MOTION_EDGE_LUT_11a.bits.snrmotionedgelut_10__1_ = snrmotionedgelut[10][1];
    V0_MOTION_EDGE_LUT_11a.bits.snrmotionedgelut_10__2_ = snrmotionedgelut[10][2];
    V0_MOTION_EDGE_LUT_11a.bits.snrmotionedgelut_10__3_ = snrmotionedgelut[10][3];
    V0_MOTION_EDGE_LUT_11b.bits.snrmotionedgelut_10__4_ = snrmotionedgelut[10][4];
    V0_MOTION_EDGE_LUT_11b.bits.snrmotionedgelut_10__5_ = snrmotionedgelut[10][5];
    V0_MOTION_EDGE_LUT_11b.bits.snrmotionedgelut_10__6_ = snrmotionedgelut[10][6];
    V0_MOTION_EDGE_LUT_11b.bits.snrmotionedgelut_10__7_ = snrmotionedgelut[10][7];
    V0_MOTION_EDGE_LUT_11c.bits.snrmotionedgelut_10__8_ = snrmotionedgelut[10][8];
    V0_MOTION_EDGE_LUT_11c.bits.snrmotionedgelut_10__9_ = snrmotionedgelut[10][9];
    V0_MOTION_EDGE_LUT_11c.bits.snrmotionedgelut_10__10_ = snrmotionedgelut[10][10];
    V0_MOTION_EDGE_LUT_11c.bits.snrmotionedgelut_10__11_ = snrmotionedgelut[10][11];
    V0_MOTION_EDGE_LUT_11d.bits.snrmotionedgelut_10__12_ = snrmotionedgelut[10][12];
    V0_MOTION_EDGE_LUT_11d.bits.snrmotionedgelut_10__13_ = snrmotionedgelut[10][13];
    V0_MOTION_EDGE_LUT_11d.bits.snrmotionedgelut_10__14_ = snrmotionedgelut[10][14];
    V0_MOTION_EDGE_LUT_11d.bits.snrmotionedgelut_10__15_ = snrmotionedgelut[10][15];
    V0_MOTION_EDGE_LUT_11e.bits.snrmotionedgelut_10__16_ = snrmotionedgelut[10][16];
    V0_MOTION_EDGE_LUT_11e.bits.snrmotionedgelut_10__17_ = snrmotionedgelut[10][17];
    V0_MOTION_EDGE_LUT_11e.bits.snrmotionedgelut_10__18_ = snrmotionedgelut[10][18];
    V0_MOTION_EDGE_LUT_11e.bits.snrmotionedgelut_10__19_ = snrmotionedgelut[10][19];
    V0_MOTION_EDGE_LUT_11f.bits.snrmotionedgelut_10__20_ = snrmotionedgelut[10][20];
    V0_MOTION_EDGE_LUT_11f.bits.snrmotionedgelut_10__21_ = snrmotionedgelut[10][21];
    V0_MOTION_EDGE_LUT_11f.bits.snrmotionedgelut_10__22_ = snrmotionedgelut[10][22];
    V0_MOTION_EDGE_LUT_11f.bits.snrmotionedgelut_10__23_ = snrmotionedgelut[10][23];
    V0_MOTION_EDGE_LUT_11g.bits.snrmotionedgelut_10__24_ = snrmotionedgelut[10][24];
    V0_MOTION_EDGE_LUT_11g.bits.snrmotionedgelut_10__25_ = snrmotionedgelut[10][25];
    V0_MOTION_EDGE_LUT_11g.bits.snrmotionedgelut_10__26_ = snrmotionedgelut[10][26];
    V0_MOTION_EDGE_LUT_11g.bits.snrmotionedgelut_10__27_ = snrmotionedgelut[10][27];
    V0_MOTION_EDGE_LUT_11h.bits.snrmotionedgelut_10__28_ = snrmotionedgelut[10][28];
    V0_MOTION_EDGE_LUT_11h.bits.snrmotionedgelut_10__29_ = snrmotionedgelut[10][29];
    V0_MOTION_EDGE_LUT_11h.bits.snrmotionedgelut_10__30_ = snrmotionedgelut[10][30];
    V0_MOTION_EDGE_LUT_11h.bits.snrmotionedgelut_10__31_ = snrmotionedgelut[10][31];
    V0_MOTION_EDGE_LUT_12a.bits.snrmotionedgelut_11__0_  = snrmotionedgelut[11][0];
    V0_MOTION_EDGE_LUT_12a.bits.snrmotionedgelut_11__1_  = snrmotionedgelut[11][1];
    V0_MOTION_EDGE_LUT_12a.bits.snrmotionedgelut_11__2_  = snrmotionedgelut[11][2];
    V0_MOTION_EDGE_LUT_12a.bits.snrmotionedgelut_11__3_  = snrmotionedgelut[11][3];
    V0_MOTION_EDGE_LUT_12b.bits.snrmotionedgelut_11__4_  = snrmotionedgelut[11][4];
    V0_MOTION_EDGE_LUT_12b.bits.snrmotionedgelut_11__5_  = snrmotionedgelut[11][5];
    V0_MOTION_EDGE_LUT_12b.bits.snrmotionedgelut_11__6_  = snrmotionedgelut[11][6];
    V0_MOTION_EDGE_LUT_12b.bits.snrmotionedgelut_11__7_  = snrmotionedgelut[11][7];
    V0_MOTION_EDGE_LUT_12c.bits.snrmotionedgelut_11__8_  = snrmotionedgelut[11][8];
    V0_MOTION_EDGE_LUT_12c.bits.snrmotionedgelut_11__9_  = snrmotionedgelut[11][9];
    V0_MOTION_EDGE_LUT_12c.bits.snrmotionedgelut_11__10_ = snrmotionedgelut[11][10];
    V0_MOTION_EDGE_LUT_12c.bits.snrmotionedgelut_11__11_ = snrmotionedgelut[11][11];
    V0_MOTION_EDGE_LUT_12d.bits.snrmotionedgelut_11__12_ = snrmotionedgelut[11][12];
    V0_MOTION_EDGE_LUT_12d.bits.snrmotionedgelut_11__13_ = snrmotionedgelut[11][13];
    V0_MOTION_EDGE_LUT_12d.bits.snrmotionedgelut_11__14_ = snrmotionedgelut[11][14];
    V0_MOTION_EDGE_LUT_12d.bits.snrmotionedgelut_11__15_ = snrmotionedgelut[11][15];
    V0_MOTION_EDGE_LUT_12e.bits.snrmotionedgelut_11__16_ = snrmotionedgelut[11][16];
    V0_MOTION_EDGE_LUT_12e.bits.snrmotionedgelut_11__17_ = snrmotionedgelut[11][17];
    V0_MOTION_EDGE_LUT_12e.bits.snrmotionedgelut_11__18_ = snrmotionedgelut[11][18];
    V0_MOTION_EDGE_LUT_12e.bits.snrmotionedgelut_11__19_ = snrmotionedgelut[11][19];
    V0_MOTION_EDGE_LUT_12f.bits.snrmotionedgelut_11__20_ = snrmotionedgelut[11][20];
    V0_MOTION_EDGE_LUT_12f.bits.snrmotionedgelut_11__21_ = snrmotionedgelut[11][21];
    V0_MOTION_EDGE_LUT_12f.bits.snrmotionedgelut_11__22_ = snrmotionedgelut[11][22];
    V0_MOTION_EDGE_LUT_12f.bits.snrmotionedgelut_11__23_ = snrmotionedgelut[11][23];
    V0_MOTION_EDGE_LUT_12g.bits.snrmotionedgelut_11__24_ = snrmotionedgelut[11][24];
    V0_MOTION_EDGE_LUT_12g.bits.snrmotionedgelut_11__25_ = snrmotionedgelut[11][25];
    V0_MOTION_EDGE_LUT_12g.bits.snrmotionedgelut_11__26_ = snrmotionedgelut[11][26];
    V0_MOTION_EDGE_LUT_12g.bits.snrmotionedgelut_11__27_ = snrmotionedgelut[11][27];
    V0_MOTION_EDGE_LUT_12h.bits.snrmotionedgelut_11__28_ = snrmotionedgelut[11][28];
    V0_MOTION_EDGE_LUT_12h.bits.snrmotionedgelut_11__29_ = snrmotionedgelut[11][29];
    V0_MOTION_EDGE_LUT_12h.bits.snrmotionedgelut_11__30_ = snrmotionedgelut[11][30];
    V0_MOTION_EDGE_LUT_12h.bits.snrmotionedgelut_11__31_ = snrmotionedgelut[11][31];
    V0_MOTION_EDGE_LUT_13a.bits.snrmotionedgelut_12__0_  = snrmotionedgelut[12][0];
    V0_MOTION_EDGE_LUT_13a.bits.snrmotionedgelut_12__1_  = snrmotionedgelut[12][1];
    V0_MOTION_EDGE_LUT_13a.bits.snrmotionedgelut_12__2_  = snrmotionedgelut[12][2];
    V0_MOTION_EDGE_LUT_13a.bits.snrmotionedgelut_12__3_  = snrmotionedgelut[12][3];
    V0_MOTION_EDGE_LUT_13b.bits.snrmotionedgelut_12__4_  = snrmotionedgelut[12][4];
    V0_MOTION_EDGE_LUT_13b.bits.snrmotionedgelut_12__5_  = snrmotionedgelut[12][5];
    V0_MOTION_EDGE_LUT_13b.bits.snrmotionedgelut_12__6_  = snrmotionedgelut[12][6];
    V0_MOTION_EDGE_LUT_13b.bits.snrmotionedgelut_12__7_  = snrmotionedgelut[12][7];
    V0_MOTION_EDGE_LUT_13c.bits.snrmotionedgelut_12__8_  = snrmotionedgelut[12][8];
    V0_MOTION_EDGE_LUT_13c.bits.snrmotionedgelut_12__9_  = snrmotionedgelut[12][9];
    V0_MOTION_EDGE_LUT_13c.bits.snrmotionedgelut_12__10_ = snrmotionedgelut[12][10];
    V0_MOTION_EDGE_LUT_13c.bits.snrmotionedgelut_12__11_ = snrmotionedgelut[12][11];
    V0_MOTION_EDGE_LUT_13d.bits.snrmotionedgelut_12__12_ = snrmotionedgelut[12][12];
    V0_MOTION_EDGE_LUT_13d.bits.snrmotionedgelut_12__13_ = snrmotionedgelut[12][13];
    V0_MOTION_EDGE_LUT_13d.bits.snrmotionedgelut_12__14_ = snrmotionedgelut[12][14];
    V0_MOTION_EDGE_LUT_13d.bits.snrmotionedgelut_12__15_ = snrmotionedgelut[12][15];
    V0_MOTION_EDGE_LUT_13e.bits.snrmotionedgelut_12__16_ = snrmotionedgelut[12][16];
    V0_MOTION_EDGE_LUT_13e.bits.snrmotionedgelut_12__17_ = snrmotionedgelut[12][17];
    V0_MOTION_EDGE_LUT_13e.bits.snrmotionedgelut_12__18_ = snrmotionedgelut[12][18];
    V0_MOTION_EDGE_LUT_13e.bits.snrmotionedgelut_12__19_ = snrmotionedgelut[12][19];
    V0_MOTION_EDGE_LUT_13f.bits.snrmotionedgelut_12__20_ = snrmotionedgelut[12][20];
    V0_MOTION_EDGE_LUT_13f.bits.snrmotionedgelut_12__21_ = snrmotionedgelut[12][21];
    V0_MOTION_EDGE_LUT_13f.bits.snrmotionedgelut_12__22_ = snrmotionedgelut[12][22];
    V0_MOTION_EDGE_LUT_13f.bits.snrmotionedgelut_12__23_ = snrmotionedgelut[12][23];
    V0_MOTION_EDGE_LUT_13g.bits.snrmotionedgelut_12__24_ = snrmotionedgelut[12][24];
    V0_MOTION_EDGE_LUT_13g.bits.snrmotionedgelut_12__25_ = snrmotionedgelut[12][25];
    V0_MOTION_EDGE_LUT_13g.bits.snrmotionedgelut_12__26_ = snrmotionedgelut[12][26];
    V0_MOTION_EDGE_LUT_13g.bits.snrmotionedgelut_12__27_ = snrmotionedgelut[12][27];
    V0_MOTION_EDGE_LUT_13h.bits.snrmotionedgelut_12__28_ = snrmotionedgelut[12][28];
    V0_MOTION_EDGE_LUT_13h.bits.snrmotionedgelut_12__29_ = snrmotionedgelut[12][29];
    V0_MOTION_EDGE_LUT_13h.bits.snrmotionedgelut_12__30_ = snrmotionedgelut[12][30];
    V0_MOTION_EDGE_LUT_13h.bits.snrmotionedgelut_12__31_ = snrmotionedgelut[12][31];
    V0_MOTION_EDGE_LUT_14a.bits.snrmotionedgelut_13__0_  = snrmotionedgelut[13][0];
    V0_MOTION_EDGE_LUT_14a.bits.snrmotionedgelut_13__1_  = snrmotionedgelut[13][1];
    V0_MOTION_EDGE_LUT_14a.bits.snrmotionedgelut_13__2_  = snrmotionedgelut[13][2];
    V0_MOTION_EDGE_LUT_14a.bits.snrmotionedgelut_13__3_  = snrmotionedgelut[13][3];
    V0_MOTION_EDGE_LUT_14b.bits.snrmotionedgelut_13__4_  = snrmotionedgelut[13][4];
    V0_MOTION_EDGE_LUT_14b.bits.snrmotionedgelut_13__5_  = snrmotionedgelut[13][5];
    V0_MOTION_EDGE_LUT_14b.bits.snrmotionedgelut_13__6_  = snrmotionedgelut[13][6];
    V0_MOTION_EDGE_LUT_14b.bits.snrmotionedgelut_13__7_  = snrmotionedgelut[13][7];
    V0_MOTION_EDGE_LUT_14c.bits.snrmotionedgelut_13__8_  = snrmotionedgelut[13][8];
    V0_MOTION_EDGE_LUT_14c.bits.snrmotionedgelut_13__9_  = snrmotionedgelut[13][9];
    V0_MOTION_EDGE_LUT_14c.bits.snrmotionedgelut_13__10_ = snrmotionedgelut[13][10];
    V0_MOTION_EDGE_LUT_14c.bits.snrmotionedgelut_13__11_ = snrmotionedgelut[13][11];
    V0_MOTION_EDGE_LUT_14d.bits.snrmotionedgelut_13__12_ = snrmotionedgelut[13][12];
    V0_MOTION_EDGE_LUT_14d.bits.snrmotionedgelut_13__13_ = snrmotionedgelut[13][13];
    V0_MOTION_EDGE_LUT_14d.bits.snrmotionedgelut_13__14_ = snrmotionedgelut[13][14];
    V0_MOTION_EDGE_LUT_14d.bits.snrmotionedgelut_13__15_ = snrmotionedgelut[13][15];
    V0_MOTION_EDGE_LUT_14e.bits.snrmotionedgelut_13__16_ = snrmotionedgelut[13][16];
    V0_MOTION_EDGE_LUT_14e.bits.snrmotionedgelut_13__17_ = snrmotionedgelut[13][17];
    V0_MOTION_EDGE_LUT_14e.bits.snrmotionedgelut_13__18_ = snrmotionedgelut[13][18];
    V0_MOTION_EDGE_LUT_14e.bits.snrmotionedgelut_13__19_ = snrmotionedgelut[13][19];
    V0_MOTION_EDGE_LUT_14f.bits.snrmotionedgelut_13__20_ = snrmotionedgelut[13][20];
    V0_MOTION_EDGE_LUT_14f.bits.snrmotionedgelut_13__21_ = snrmotionedgelut[13][21];
    V0_MOTION_EDGE_LUT_14f.bits.snrmotionedgelut_13__22_ = snrmotionedgelut[13][22];
    V0_MOTION_EDGE_LUT_14f.bits.snrmotionedgelut_13__23_ = snrmotionedgelut[13][23];
    V0_MOTION_EDGE_LUT_14g.bits.snrmotionedgelut_13__24_ = snrmotionedgelut[13][24];
    V0_MOTION_EDGE_LUT_14g.bits.snrmotionedgelut_13__25_ = snrmotionedgelut[13][25];
    V0_MOTION_EDGE_LUT_14g.bits.snrmotionedgelut_13__26_ = snrmotionedgelut[13][26];
    V0_MOTION_EDGE_LUT_14g.bits.snrmotionedgelut_13__27_ = snrmotionedgelut[13][27];
    V0_MOTION_EDGE_LUT_14h.bits.snrmotionedgelut_13__28_ = snrmotionedgelut[13][28];
    V0_MOTION_EDGE_LUT_14h.bits.snrmotionedgelut_13__29_ = snrmotionedgelut[13][29];
    V0_MOTION_EDGE_LUT_14h.bits.snrmotionedgelut_13__30_ = snrmotionedgelut[13][30];
    V0_MOTION_EDGE_LUT_14h.bits.snrmotionedgelut_13__31_ = snrmotionedgelut[13][31];
    V0_MOTION_EDGE_LUT_15a.bits.snrmotionedgelut_14__0_  = snrmotionedgelut[14][0];
    V0_MOTION_EDGE_LUT_15a.bits.snrmotionedgelut_14__1_  = snrmotionedgelut[14][1];
    V0_MOTION_EDGE_LUT_15a.bits.snrmotionedgelut_14__2_  = snrmotionedgelut[14][2];
    V0_MOTION_EDGE_LUT_15a.bits.snrmotionedgelut_14__3_  = snrmotionedgelut[14][3];
    V0_MOTION_EDGE_LUT_15b.bits.snrmotionedgelut_14__4_  = snrmotionedgelut[14][4];
    V0_MOTION_EDGE_LUT_15b.bits.snrmotionedgelut_14__5_  = snrmotionedgelut[14][5];
    V0_MOTION_EDGE_LUT_15b.bits.snrmotionedgelut_14__6_  = snrmotionedgelut[14][6];
    V0_MOTION_EDGE_LUT_15b.bits.snrmotionedgelut_14__7_  = snrmotionedgelut[14][7];
    V0_MOTION_EDGE_LUT_15c.bits.snrmotionedgelut_14__8_  = snrmotionedgelut[14][8];
    V0_MOTION_EDGE_LUT_15c.bits.snrmotionedgelut_14__9_  = snrmotionedgelut[14][9];
    V0_MOTION_EDGE_LUT_15c.bits.snrmotionedgelut_14__10_ = snrmotionedgelut[14][10];
    V0_MOTION_EDGE_LUT_15c.bits.snrmotionedgelut_14__11_ = snrmotionedgelut[14][11];
    V0_MOTION_EDGE_LUT_15d.bits.snrmotionedgelut_14__12_ = snrmotionedgelut[14][12];
    V0_MOTION_EDGE_LUT_15d.bits.snrmotionedgelut_14__13_ = snrmotionedgelut[14][13];
    V0_MOTION_EDGE_LUT_15d.bits.snrmotionedgelut_14__14_ = snrmotionedgelut[14][14];
    V0_MOTION_EDGE_LUT_15d.bits.snrmotionedgelut_14__15_ = snrmotionedgelut[14][15];
    V0_MOTION_EDGE_LUT_15e.bits.snrmotionedgelut_14__16_ = snrmotionedgelut[14][16];
    V0_MOTION_EDGE_LUT_15e.bits.snrmotionedgelut_14__17_ = snrmotionedgelut[14][17];
    V0_MOTION_EDGE_LUT_15e.bits.snrmotionedgelut_14__18_ = snrmotionedgelut[14][18];
    V0_MOTION_EDGE_LUT_15e.bits.snrmotionedgelut_14__19_ = snrmotionedgelut[14][19];
    V0_MOTION_EDGE_LUT_15f.bits.snrmotionedgelut_14__20_ = snrmotionedgelut[14][20];
    V0_MOTION_EDGE_LUT_15f.bits.snrmotionedgelut_14__21_ = snrmotionedgelut[14][21];
    V0_MOTION_EDGE_LUT_15f.bits.snrmotionedgelut_14__22_ = snrmotionedgelut[14][22];
    V0_MOTION_EDGE_LUT_15f.bits.snrmotionedgelut_14__23_ = snrmotionedgelut[14][23];
    V0_MOTION_EDGE_LUT_15g.bits.snrmotionedgelut_14__24_ = snrmotionedgelut[14][24];
    V0_MOTION_EDGE_LUT_15g.bits.snrmotionedgelut_14__25_ = snrmotionedgelut[14][25];
    V0_MOTION_EDGE_LUT_15g.bits.snrmotionedgelut_14__26_ = snrmotionedgelut[14][26];
    V0_MOTION_EDGE_LUT_15g.bits.snrmotionedgelut_14__27_ = snrmotionedgelut[14][27];
    V0_MOTION_EDGE_LUT_15h.bits.snrmotionedgelut_14__28_ = snrmotionedgelut[14][28];
    V0_MOTION_EDGE_LUT_15h.bits.snrmotionedgelut_14__29_ = snrmotionedgelut[14][29];
    V0_MOTION_EDGE_LUT_15h.bits.snrmotionedgelut_14__30_ = snrmotionedgelut[14][30];
    V0_MOTION_EDGE_LUT_15h.bits.snrmotionedgelut_14__31_ = snrmotionedgelut[14][31];
    V0_MOTION_EDGE_LUT_16a.bits.snrmotionedgelut_15__0_  = snrmotionedgelut[15][0];
    V0_MOTION_EDGE_LUT_16a.bits.snrmotionedgelut_15__1_  = snrmotionedgelut[15][1];
    V0_MOTION_EDGE_LUT_16a.bits.snrmotionedgelut_15__2_  = snrmotionedgelut[15][2];
    V0_MOTION_EDGE_LUT_16a.bits.snrmotionedgelut_15__3_  = snrmotionedgelut[15][3];
    V0_MOTION_EDGE_LUT_16b.bits.snrmotionedgelut_15__4_  = snrmotionedgelut[15][4];
    V0_MOTION_EDGE_LUT_16b.bits.snrmotionedgelut_15__5_  = snrmotionedgelut[15][5];
    V0_MOTION_EDGE_LUT_16b.bits.snrmotionedgelut_15__6_  = snrmotionedgelut[15][6];
    V0_MOTION_EDGE_LUT_16b.bits.snrmotionedgelut_15__7_  = snrmotionedgelut[15][7];
    V0_MOTION_EDGE_LUT_16c.bits.snrmotionedgelut_15__8_  = snrmotionedgelut[15][8];
    V0_MOTION_EDGE_LUT_16c.bits.snrmotionedgelut_15__9_  = snrmotionedgelut[15][9];
    V0_MOTION_EDGE_LUT_16c.bits.snrmotionedgelut_15__10_ = snrmotionedgelut[15][10];
    V0_MOTION_EDGE_LUT_16c.bits.snrmotionedgelut_15__11_ = snrmotionedgelut[15][11];
    V0_MOTION_EDGE_LUT_16d.bits.snrmotionedgelut_15__12_ = snrmotionedgelut[15][12];
    V0_MOTION_EDGE_LUT_16d.bits.snrmotionedgelut_15__13_ = snrmotionedgelut[15][13];
    V0_MOTION_EDGE_LUT_16d.bits.snrmotionedgelut_15__14_ = snrmotionedgelut[15][14];
    V0_MOTION_EDGE_LUT_16d.bits.snrmotionedgelut_15__15_ = snrmotionedgelut[15][15];
    V0_MOTION_EDGE_LUT_16e.bits.snrmotionedgelut_15__16_ = snrmotionedgelut[15][16];
    V0_MOTION_EDGE_LUT_16e.bits.snrmotionedgelut_15__17_ = snrmotionedgelut[15][17];
    V0_MOTION_EDGE_LUT_16e.bits.snrmotionedgelut_15__18_ = snrmotionedgelut[15][18];
    V0_MOTION_EDGE_LUT_16e.bits.snrmotionedgelut_15__19_ = snrmotionedgelut[15][19];
    V0_MOTION_EDGE_LUT_16f.bits.snrmotionedgelut_15__20_ = snrmotionedgelut[15][20];
    V0_MOTION_EDGE_LUT_16f.bits.snrmotionedgelut_15__21_ = snrmotionedgelut[15][21];
    V0_MOTION_EDGE_LUT_16f.bits.snrmotionedgelut_15__22_ = snrmotionedgelut[15][22];
    V0_MOTION_EDGE_LUT_16f.bits.snrmotionedgelut_15__23_ = snrmotionedgelut[15][23];
    V0_MOTION_EDGE_LUT_16g.bits.snrmotionedgelut_15__24_ = snrmotionedgelut[15][24];
    V0_MOTION_EDGE_LUT_16g.bits.snrmotionedgelut_15__25_ = snrmotionedgelut[15][25];
    V0_MOTION_EDGE_LUT_16g.bits.snrmotionedgelut_15__26_ = snrmotionedgelut[15][26];
    V0_MOTION_EDGE_LUT_16g.bits.snrmotionedgelut_15__27_ = snrmotionedgelut[15][27];
    V0_MOTION_EDGE_LUT_16h.bits.snrmotionedgelut_15__28_ = snrmotionedgelut[15][28];
    V0_MOTION_EDGE_LUT_16h.bits.snrmotionedgelut_15__29_ = snrmotionedgelut[15][29];
    V0_MOTION_EDGE_LUT_16h.bits.snrmotionedgelut_15__30_ = snrmotionedgelut[15][30];
    V0_MOTION_EDGE_LUT_16h.bits.snrmotionedgelut_15__31_ = snrmotionedgelut[15][31];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_16a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_16a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_15a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_15a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_14a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_14a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_13a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_13a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_12a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_12a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_11a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_11a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_10a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_10a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_09a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_09a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_08a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_08a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_07a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_07a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_06a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_06a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_05a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_05a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_04a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_04a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_03a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_03a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_02a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_02a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01h.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01h.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01g.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01g.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01f.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01f.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01e.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01e.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01d.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01c.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01b.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MOTION_EDGE_LUT_01a.u32) + u32Data * VID_OFFSET), V0_MOTION_EDGE_LUT_01a.u32);
    return;
}

HI_VOID VPSS_DRV_SetSnrColorBasedLut(HI_U32 u32Data, HI_U32 snrcbcrlut[16][16])
{
    U_V0_COLOR_BASED_LUT_01a V0_COLOR_BASED_LUT_01a;
    U_V0_COLOR_BASED_LUT_01b V0_COLOR_BASED_LUT_01b;
    U_V0_COLOR_BASED_LUT_01c V0_COLOR_BASED_LUT_01c;
    U_V0_COLOR_BASED_LUT_01d V0_COLOR_BASED_LUT_01d;
    U_V0_COLOR_BASED_LUT_02a V0_COLOR_BASED_LUT_02a;
    U_V0_COLOR_BASED_LUT_02b V0_COLOR_BASED_LUT_02b;
    U_V0_COLOR_BASED_LUT_02c V0_COLOR_BASED_LUT_02c;
    U_V0_COLOR_BASED_LUT_02d V0_COLOR_BASED_LUT_02d;
    U_V0_COLOR_BASED_LUT_03a V0_COLOR_BASED_LUT_03a;
    U_V0_COLOR_BASED_LUT_03b V0_COLOR_BASED_LUT_03b;
    U_V0_COLOR_BASED_LUT_03c V0_COLOR_BASED_LUT_03c;
    U_V0_COLOR_BASED_LUT_03d V0_COLOR_BASED_LUT_03d;
    U_V0_COLOR_BASED_LUT_04a V0_COLOR_BASED_LUT_04a;
    U_V0_COLOR_BASED_LUT_04b V0_COLOR_BASED_LUT_04b;
    U_V0_COLOR_BASED_LUT_04c V0_COLOR_BASED_LUT_04c;
    U_V0_COLOR_BASED_LUT_04d V0_COLOR_BASED_LUT_04d;
    U_V0_COLOR_BASED_LUT_05a V0_COLOR_BASED_LUT_05a;
    U_V0_COLOR_BASED_LUT_05b V0_COLOR_BASED_LUT_05b;
    U_V0_COLOR_BASED_LUT_05c V0_COLOR_BASED_LUT_05c;
    U_V0_COLOR_BASED_LUT_05d V0_COLOR_BASED_LUT_05d;
    U_V0_COLOR_BASED_LUT_06a V0_COLOR_BASED_LUT_06a;
    U_V0_COLOR_BASED_LUT_06b V0_COLOR_BASED_LUT_06b;
    U_V0_COLOR_BASED_LUT_06c V0_COLOR_BASED_LUT_06c;
    U_V0_COLOR_BASED_LUT_06d V0_COLOR_BASED_LUT_06d;
    U_V0_COLOR_BASED_LUT_07a V0_COLOR_BASED_LUT_07a;
    U_V0_COLOR_BASED_LUT_07b V0_COLOR_BASED_LUT_07b;
    U_V0_COLOR_BASED_LUT_07c V0_COLOR_BASED_LUT_07c;
    U_V0_COLOR_BASED_LUT_07d V0_COLOR_BASED_LUT_07d;
    U_V0_COLOR_BASED_LUT_08a V0_COLOR_BASED_LUT_08a;
    U_V0_COLOR_BASED_LUT_08b V0_COLOR_BASED_LUT_08b;
    U_V0_COLOR_BASED_LUT_08c V0_COLOR_BASED_LUT_08c;
    U_V0_COLOR_BASED_LUT_08d V0_COLOR_BASED_LUT_08d;
    U_V0_COLOR_BASED_LUT_09a V0_COLOR_BASED_LUT_09a;
    U_V0_COLOR_BASED_LUT_09b V0_COLOR_BASED_LUT_09b;
    U_V0_COLOR_BASED_LUT_09c V0_COLOR_BASED_LUT_09c;
    U_V0_COLOR_BASED_LUT_09d V0_COLOR_BASED_LUT_09d;
    U_V0_COLOR_BASED_LUT_10a V0_COLOR_BASED_LUT_10a;
    U_V0_COLOR_BASED_LUT_10b V0_COLOR_BASED_LUT_10b;
    U_V0_COLOR_BASED_LUT_10c V0_COLOR_BASED_LUT_10c;
    U_V0_COLOR_BASED_LUT_10d V0_COLOR_BASED_LUT_10d;
    U_V0_COLOR_BASED_LUT_11a V0_COLOR_BASED_LUT_11a;
    U_V0_COLOR_BASED_LUT_11b V0_COLOR_BASED_LUT_11b;
    U_V0_COLOR_BASED_LUT_11c V0_COLOR_BASED_LUT_11c;
    U_V0_COLOR_BASED_LUT_11d V0_COLOR_BASED_LUT_11d;
    U_V0_COLOR_BASED_LUT_12a V0_COLOR_BASED_LUT_12a;
    U_V0_COLOR_BASED_LUT_12b V0_COLOR_BASED_LUT_12b;
    U_V0_COLOR_BASED_LUT_12c V0_COLOR_BASED_LUT_12c;
    U_V0_COLOR_BASED_LUT_12d V0_COLOR_BASED_LUT_12d;
    U_V0_COLOR_BASED_LUT_13a V0_COLOR_BASED_LUT_13a;
    U_V0_COLOR_BASED_LUT_13b V0_COLOR_BASED_LUT_13b;
    U_V0_COLOR_BASED_LUT_13c V0_COLOR_BASED_LUT_13c;
    U_V0_COLOR_BASED_LUT_13d V0_COLOR_BASED_LUT_13d;
    U_V0_COLOR_BASED_LUT_14a V0_COLOR_BASED_LUT_14a;
    U_V0_COLOR_BASED_LUT_14b V0_COLOR_BASED_LUT_14b;
    U_V0_COLOR_BASED_LUT_14c V0_COLOR_BASED_LUT_14c;
    U_V0_COLOR_BASED_LUT_14d V0_COLOR_BASED_LUT_14d;
    U_V0_COLOR_BASED_LUT_15a V0_COLOR_BASED_LUT_15a;
    U_V0_COLOR_BASED_LUT_15b V0_COLOR_BASED_LUT_15b;
    U_V0_COLOR_BASED_LUT_15c V0_COLOR_BASED_LUT_15c;
    U_V0_COLOR_BASED_LUT_15d V0_COLOR_BASED_LUT_15d;
    U_V0_COLOR_BASED_LUT_16a V0_COLOR_BASED_LUT_16a;
    U_V0_COLOR_BASED_LUT_16b V0_COLOR_BASED_LUT_16b;
    U_V0_COLOR_BASED_LUT_16c V0_COLOR_BASED_LUT_16c;
    U_V0_COLOR_BASED_LUT_16d V0_COLOR_BASED_LUT_16d;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VPSS_DRV_SetSnrColorBasedLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_COLOR_BASED_LUT_16d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_16c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_16b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_16a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_15d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_15c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_15b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_15a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_14d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_14c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_14b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_14a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_13d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_13c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_13b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_13a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_12d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_12c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_12b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_12a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_11d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_11c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_11b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_11a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_10d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_10c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_10b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_10a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_09d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_09c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_09b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_09a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_08d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_08c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_08b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_08a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_07d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_07c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_07b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_07a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_06d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_06c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_06b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_06a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_05d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_05c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_05b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_05a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_04d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_04c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_04b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_04a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_03d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_03c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_03b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_03a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_02d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_02c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_02b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_02a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_01d.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01d.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_01c.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01c.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_01b.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01b.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_01a.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01a.u32) + u32Data * VID_OFFSET));
    V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__0_  = snrcbcrlut[0][0];
    V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__1_  = snrcbcrlut[0][1];
    V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__2_  = snrcbcrlut[0][2];
    V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__3_  = snrcbcrlut[0][3];
    V0_COLOR_BASED_LUT_01b.bits.snrcbcrlut_0__4_  = snrcbcrlut[0][4];
    V0_COLOR_BASED_LUT_01b.bits.snrcbcrlut_0__5_  = snrcbcrlut[0][5];
    V0_COLOR_BASED_LUT_01b.bits.snrcbcrlut_0__6_  = snrcbcrlut[0][6];
    V0_COLOR_BASED_LUT_01b.bits.snrcbcrlut_0__7_  = snrcbcrlut[0][7];
    V0_COLOR_BASED_LUT_01c.bits.snrcbcrlut_0__8_  = snrcbcrlut[0][8];
    V0_COLOR_BASED_LUT_01c.bits.snrcbcrlut_0__9_  = snrcbcrlut[0][9];
    V0_COLOR_BASED_LUT_01c.bits.snrcbcrlut_0__10_ = snrcbcrlut[0][10];
    V0_COLOR_BASED_LUT_01c.bits.snrcbcrlut_0__11_ = snrcbcrlut[0][11];
    V0_COLOR_BASED_LUT_01d.bits.snrcbcrlut_0__12_ = snrcbcrlut[0][12];
    V0_COLOR_BASED_LUT_01d.bits.snrcbcrlut_0__13_ = snrcbcrlut[0][13];
    V0_COLOR_BASED_LUT_01d.bits.snrcbcrlut_0__14_ = snrcbcrlut[0][14];
    V0_COLOR_BASED_LUT_01d.bits.snrcbcrlut_0__15_ = snrcbcrlut[0][15];
    V0_COLOR_BASED_LUT_02a.bits.snrcbcrlut_1__0_  = snrcbcrlut[1][0];
    V0_COLOR_BASED_LUT_02a.bits.snrcbcrlut_1__1_  = snrcbcrlut[1][1];
    V0_COLOR_BASED_LUT_02a.bits.snrcbcrlut_1__2_  = snrcbcrlut[1][2];
    V0_COLOR_BASED_LUT_02a.bits.snrcbcrlut_1__3_  = snrcbcrlut[1][3];
    V0_COLOR_BASED_LUT_02b.bits.snrcbcrlut_1__4_  = snrcbcrlut[1][4];
    V0_COLOR_BASED_LUT_02b.bits.snrcbcrlut_1__5_  = snrcbcrlut[1][5];
    V0_COLOR_BASED_LUT_02b.bits.snrcbcrlut_1__6_  = snrcbcrlut[1][6];
    V0_COLOR_BASED_LUT_02b.bits.snrcbcrlut_1__7_  = snrcbcrlut[1][7];
    V0_COLOR_BASED_LUT_02c.bits.snrcbcrlut_1__8_  = snrcbcrlut[1][8];
    V0_COLOR_BASED_LUT_02c.bits.snrcbcrlut_1__9_  = snrcbcrlut[1][9];
    V0_COLOR_BASED_LUT_02c.bits.snrcbcrlut_1__10_ = snrcbcrlut[1][10];
    V0_COLOR_BASED_LUT_02c.bits.snrcbcrlut_1__11_ = snrcbcrlut[1][11];
    V0_COLOR_BASED_LUT_02d.bits.snrcbcrlut_1__12_ = snrcbcrlut[1][12];
    V0_COLOR_BASED_LUT_02d.bits.snrcbcrlut_1__13_ = snrcbcrlut[1][13];
    V0_COLOR_BASED_LUT_02d.bits.snrcbcrlut_1__14_ = snrcbcrlut[1][14];
    V0_COLOR_BASED_LUT_02d.bits.snrcbcrlut_1__15_ = snrcbcrlut[1][15];
    V0_COLOR_BASED_LUT_03a.bits.snrcbcrlut_2__0_  = snrcbcrlut[2][0];
    V0_COLOR_BASED_LUT_03a.bits.snrcbcrlut_2__1_  = snrcbcrlut[2][1];
    V0_COLOR_BASED_LUT_03a.bits.snrcbcrlut_2__2_  = snrcbcrlut[2][2];
    V0_COLOR_BASED_LUT_03a.bits.snrcbcrlut_2__3_  = snrcbcrlut[2][3];
    V0_COLOR_BASED_LUT_03b.bits.snrcbcrlut_2__4_  = snrcbcrlut[2][4];
    V0_COLOR_BASED_LUT_03b.bits.snrcbcrlut_2__5_  = snrcbcrlut[2][5];
    V0_COLOR_BASED_LUT_03b.bits.snrcbcrlut_2__6_  = snrcbcrlut[2][6];
    V0_COLOR_BASED_LUT_03b.bits.snrcbcrlut_2__7_  = snrcbcrlut[2][7];
    V0_COLOR_BASED_LUT_03c.bits.snrcbcrlut_2__8_  = snrcbcrlut[2][8];
    V0_COLOR_BASED_LUT_03c.bits.snrcbcrlut_2__9_  = snrcbcrlut[2][9];
    V0_COLOR_BASED_LUT_03c.bits.snrcbcrlut_2__10_ = snrcbcrlut[2][10];
    V0_COLOR_BASED_LUT_03c.bits.snrcbcrlut_2__11_ = snrcbcrlut[2][11];
    V0_COLOR_BASED_LUT_03d.bits.snrcbcrlut_2__12_ = snrcbcrlut[2][12];
    V0_COLOR_BASED_LUT_03d.bits.snrcbcrlut_2__13_ = snrcbcrlut[2][13];
    V0_COLOR_BASED_LUT_03d.bits.snrcbcrlut_2__14_ = snrcbcrlut[2][14];
    V0_COLOR_BASED_LUT_03d.bits.snrcbcrlut_2__15_ = snrcbcrlut[2][15];
    V0_COLOR_BASED_LUT_04a.bits.snrcbcrlut_3__0_  = snrcbcrlut[3][0];
    V0_COLOR_BASED_LUT_04a.bits.snrcbcrlut_3__1_  = snrcbcrlut[3][1];
    V0_COLOR_BASED_LUT_04a.bits.snrcbcrlut_3__2_  = snrcbcrlut[3][2];
    V0_COLOR_BASED_LUT_04a.bits.snrcbcrlut_3__3_  = snrcbcrlut[3][3];
    V0_COLOR_BASED_LUT_04b.bits.snrcbcrlut_3__4_  = snrcbcrlut[3][4];
    V0_COLOR_BASED_LUT_04b.bits.snrcbcrlut_3__5_  = snrcbcrlut[3][5];
    V0_COLOR_BASED_LUT_04b.bits.snrcbcrlut_3__6_  = snrcbcrlut[3][6];
    V0_COLOR_BASED_LUT_04b.bits.snrcbcrlut_3__7_  = snrcbcrlut[3][7];
    V0_COLOR_BASED_LUT_04c.bits.snrcbcrlut_3__8_  = snrcbcrlut[3][8];
    V0_COLOR_BASED_LUT_04c.bits.snrcbcrlut_3__9_  = snrcbcrlut[3][9];
    V0_COLOR_BASED_LUT_04c.bits.snrcbcrlut_3__10_ = snrcbcrlut[3][10];
    V0_COLOR_BASED_LUT_04c.bits.snrcbcrlut_3__11_ = snrcbcrlut[3][11];
    V0_COLOR_BASED_LUT_04d.bits.snrcbcrlut_3__12_ = snrcbcrlut[3][12];
    V0_COLOR_BASED_LUT_04d.bits.snrcbcrlut_3__13_ = snrcbcrlut[3][13];
    V0_COLOR_BASED_LUT_04d.bits.snrcbcrlut_3__14_ = snrcbcrlut[3][14];
    V0_COLOR_BASED_LUT_04d.bits.snrcbcrlut_3__15_ = snrcbcrlut[3][15];
    V0_COLOR_BASED_LUT_05a.bits.snrcbcrlut_4__0_  = snrcbcrlut[4][0];
    V0_COLOR_BASED_LUT_05a.bits.snrcbcrlut_4__1_  = snrcbcrlut[4][1];
    V0_COLOR_BASED_LUT_05a.bits.snrcbcrlut_4__2_  = snrcbcrlut[4][2];
    V0_COLOR_BASED_LUT_05a.bits.snrcbcrlut_4__3_  = snrcbcrlut[4][3];
    V0_COLOR_BASED_LUT_05b.bits.snrcbcrlut_4__4_  = snrcbcrlut[4][4];
    V0_COLOR_BASED_LUT_05b.bits.snrcbcrlut_4__5_  = snrcbcrlut[4][5];
    V0_COLOR_BASED_LUT_05b.bits.snrcbcrlut_4__6_  = snrcbcrlut[4][6];
    V0_COLOR_BASED_LUT_05b.bits.snrcbcrlut_4__7_  = snrcbcrlut[4][7];
    V0_COLOR_BASED_LUT_05c.bits.snrcbcrlut_4__8_  = snrcbcrlut[4][8];
    V0_COLOR_BASED_LUT_05c.bits.snrcbcrlut_4__9_  = snrcbcrlut[4][9];
    V0_COLOR_BASED_LUT_05c.bits.snrcbcrlut_4__10_ = snrcbcrlut[4][10];
    V0_COLOR_BASED_LUT_05c.bits.snrcbcrlut_4__11_ = snrcbcrlut[4][11];
    V0_COLOR_BASED_LUT_05d.bits.snrcbcrlut_4__12_ = snrcbcrlut[4][12];
    V0_COLOR_BASED_LUT_05d.bits.snrcbcrlut_4__13_ = snrcbcrlut[4][13];
    V0_COLOR_BASED_LUT_05d.bits.snrcbcrlut_4__14_ = snrcbcrlut[4][14];
    V0_COLOR_BASED_LUT_05d.bits.snrcbcrlut_4__15_ = snrcbcrlut[4][15];
    V0_COLOR_BASED_LUT_06a.bits.snrcbcrlut_5__0_  = snrcbcrlut[5][0];
    V0_COLOR_BASED_LUT_06a.bits.snrcbcrlut_5__1_  = snrcbcrlut[5][1];
    V0_COLOR_BASED_LUT_06a.bits.snrcbcrlut_5__2_  = snrcbcrlut[5][2];
    V0_COLOR_BASED_LUT_06a.bits.snrcbcrlut_5__3_  = snrcbcrlut[5][3];
    V0_COLOR_BASED_LUT_06b.bits.snrcbcrlut_5__4_  = snrcbcrlut[5][4];
    V0_COLOR_BASED_LUT_06b.bits.snrcbcrlut_5__5_  = snrcbcrlut[5][5];
    V0_COLOR_BASED_LUT_06b.bits.snrcbcrlut_5__6_  = snrcbcrlut[5][6];
    V0_COLOR_BASED_LUT_06b.bits.snrcbcrlut_5__7_  = snrcbcrlut[5][7];
    V0_COLOR_BASED_LUT_06c.bits.snrcbcrlut_5__8_  = snrcbcrlut[5][8];
    V0_COLOR_BASED_LUT_06c.bits.snrcbcrlut_5__9_  = snrcbcrlut[5][9];
    V0_COLOR_BASED_LUT_06c.bits.snrcbcrlut_5__10_ = snrcbcrlut[5][10];
    V0_COLOR_BASED_LUT_06c.bits.snrcbcrlut_5__11_ = snrcbcrlut[5][11];
    V0_COLOR_BASED_LUT_06d.bits.snrcbcrlut_5__12_ = snrcbcrlut[5][12];
    V0_COLOR_BASED_LUT_06d.bits.snrcbcrlut_5__13_ = snrcbcrlut[5][13];
    V0_COLOR_BASED_LUT_06d.bits.snrcbcrlut_5__14_ = snrcbcrlut[5][14];
    V0_COLOR_BASED_LUT_06d.bits.snrcbcrlut_5__15_ = snrcbcrlut[5][15];
    V0_COLOR_BASED_LUT_07a.bits.snrcbcrlut_6__0_  = snrcbcrlut[6][0];
    V0_COLOR_BASED_LUT_07a.bits.snrcbcrlut_6__1_  = snrcbcrlut[6][1];
    V0_COLOR_BASED_LUT_07a.bits.snrcbcrlut_6__2_  = snrcbcrlut[6][2];
    V0_COLOR_BASED_LUT_07a.bits.snrcbcrlut_6__3_  = snrcbcrlut[6][3];
    V0_COLOR_BASED_LUT_07b.bits.snrcbcrlut_6__4_  = snrcbcrlut[6][4];
    V0_COLOR_BASED_LUT_07b.bits.snrcbcrlut_6__5_  = snrcbcrlut[6][5];
    V0_COLOR_BASED_LUT_07b.bits.snrcbcrlut_6__6_  = snrcbcrlut[6][6];
    V0_COLOR_BASED_LUT_07b.bits.snrcbcrlut_6__7_  = snrcbcrlut[6][7];
    V0_COLOR_BASED_LUT_07c.bits.snrcbcrlut_6__8_  = snrcbcrlut[6][8];
    V0_COLOR_BASED_LUT_07c.bits.snrcbcrlut_6__9_  = snrcbcrlut[6][9];
    V0_COLOR_BASED_LUT_07c.bits.snrcbcrlut_6__10_ = snrcbcrlut[6][10];
    V0_COLOR_BASED_LUT_07c.bits.snrcbcrlut_6__11_ = snrcbcrlut[6][11];
    V0_COLOR_BASED_LUT_07d.bits.snrcbcrlut_6__12_ = snrcbcrlut[6][12];
    V0_COLOR_BASED_LUT_07d.bits.snrcbcrlut_6__13_ = snrcbcrlut[6][13];
    V0_COLOR_BASED_LUT_07d.bits.snrcbcrlut_6__14_ = snrcbcrlut[6][14];
    V0_COLOR_BASED_LUT_07d.bits.snrcbcrlut_6__15_ = snrcbcrlut[6][15];
    V0_COLOR_BASED_LUT_08a.bits.snrcbcrlut_7__0_  = snrcbcrlut[7][0];
    V0_COLOR_BASED_LUT_08a.bits.snrcbcrlut_7__1_  = snrcbcrlut[7][1];
    V0_COLOR_BASED_LUT_08a.bits.snrcbcrlut_7__2_  = snrcbcrlut[7][2];
    V0_COLOR_BASED_LUT_08a.bits.snrcbcrlut_7__3_  = snrcbcrlut[7][3];
    V0_COLOR_BASED_LUT_08b.bits.snrcbcrlut_7__4_  = snrcbcrlut[7][4];
    V0_COLOR_BASED_LUT_08b.bits.snrcbcrlut_7__5_  = snrcbcrlut[7][5];
    V0_COLOR_BASED_LUT_08b.bits.snrcbcrlut_7__6_  = snrcbcrlut[7][6];
    V0_COLOR_BASED_LUT_08b.bits.snrcbcrlut_7__7_  = snrcbcrlut[7][7];
    V0_COLOR_BASED_LUT_08c.bits.snrcbcrlut_7__8_  = snrcbcrlut[7][8];
    V0_COLOR_BASED_LUT_08c.bits.snrcbcrlut_7__9_  = snrcbcrlut[7][9];
    V0_COLOR_BASED_LUT_08c.bits.snrcbcrlut_7__10_ = snrcbcrlut[7][10];
    V0_COLOR_BASED_LUT_08c.bits.snrcbcrlut_7__11_ = snrcbcrlut[7][11];
    V0_COLOR_BASED_LUT_08d.bits.snrcbcrlut_7__12_ = snrcbcrlut[7][12];
    V0_COLOR_BASED_LUT_08d.bits.snrcbcrlut_7__13_ = snrcbcrlut[7][13];
    V0_COLOR_BASED_LUT_08d.bits.snrcbcrlut_7__14_ = snrcbcrlut[7][14];
    V0_COLOR_BASED_LUT_08d.bits.snrcbcrlut_7__15_ = snrcbcrlut[7][15];
    V0_COLOR_BASED_LUT_09a.bits.snrcbcrlut_8__0_  = snrcbcrlut[8][0];
    V0_COLOR_BASED_LUT_09a.bits.snrcbcrlut_8__1_  = snrcbcrlut[8][1];
    V0_COLOR_BASED_LUT_09a.bits.snrcbcrlut_8__2_  = snrcbcrlut[8][2];
    V0_COLOR_BASED_LUT_09a.bits.snrcbcrlut_8__3_  = snrcbcrlut[8][3];
    V0_COLOR_BASED_LUT_09b.bits.snrcbcrlut_8__4_  = snrcbcrlut[8][4];
    V0_COLOR_BASED_LUT_09b.bits.snrcbcrlut_8__5_  = snrcbcrlut[8][5];
    V0_COLOR_BASED_LUT_09b.bits.snrcbcrlut_8__6_  = snrcbcrlut[8][6];
    V0_COLOR_BASED_LUT_09b.bits.snrcbcrlut_8__7_  = snrcbcrlut[8][7];
    V0_COLOR_BASED_LUT_09c.bits.snrcbcrlut_8__8_  = snrcbcrlut[8][8];
    V0_COLOR_BASED_LUT_09c.bits.snrcbcrlut_8__9_  = snrcbcrlut[8][9];
    V0_COLOR_BASED_LUT_09c.bits.snrcbcrlut_8__10_ = snrcbcrlut[8][10];
    V0_COLOR_BASED_LUT_09c.bits.snrcbcrlut_8__11_ = snrcbcrlut[8][11];
    V0_COLOR_BASED_LUT_09d.bits.snrcbcrlut_8__12_ = snrcbcrlut[8][12];
    V0_COLOR_BASED_LUT_09d.bits.snrcbcrlut_8__13_ = snrcbcrlut[8][13];
    V0_COLOR_BASED_LUT_09d.bits.snrcbcrlut_8__14_ = snrcbcrlut[8][14];
    V0_COLOR_BASED_LUT_09d.bits.snrcbcrlut_8__15_ = snrcbcrlut[8][15];
    V0_COLOR_BASED_LUT_10a.bits.snrcbcrlut_9__0_  = snrcbcrlut[9][0];
    V0_COLOR_BASED_LUT_10a.bits.snrcbcrlut_9__1_  = snrcbcrlut[9][1];
    V0_COLOR_BASED_LUT_10a.bits.snrcbcrlut_9__2_  = snrcbcrlut[9][2];
    V0_COLOR_BASED_LUT_10a.bits.snrcbcrlut_9__3_  = snrcbcrlut[9][3];
    V0_COLOR_BASED_LUT_10b.bits.snrcbcrlut_9__4_  = snrcbcrlut[9][4];
    V0_COLOR_BASED_LUT_10b.bits.snrcbcrlut_9__5_  = snrcbcrlut[9][5];
    V0_COLOR_BASED_LUT_10b.bits.snrcbcrlut_9__6_  = snrcbcrlut[9][6];
    V0_COLOR_BASED_LUT_10b.bits.snrcbcrlut_9__7_  = snrcbcrlut[9][7];
    V0_COLOR_BASED_LUT_10c.bits.snrcbcrlut_9__8_  = snrcbcrlut[9][8];
    V0_COLOR_BASED_LUT_10c.bits.snrcbcrlut_9__9_  = snrcbcrlut[9][9];
    V0_COLOR_BASED_LUT_10c.bits.snrcbcrlut_9__10_ = snrcbcrlut[9][10];
    V0_COLOR_BASED_LUT_10c.bits.snrcbcrlut_9__11_ = snrcbcrlut[9][11];
    V0_COLOR_BASED_LUT_10d.bits.snrcbcrlut_9__12_ = snrcbcrlut[9][12];
    V0_COLOR_BASED_LUT_10d.bits.snrcbcrlut_9__13_ = snrcbcrlut[9][13];
    V0_COLOR_BASED_LUT_10d.bits.snrcbcrlut_9__14_ = snrcbcrlut[9][14];
    V0_COLOR_BASED_LUT_10d.bits.snrcbcrlut_9__15_ = snrcbcrlut[9][15];
    V0_COLOR_BASED_LUT_11a.bits.snrcbcrlut_10__0_ = snrcbcrlut[10][0];
    V0_COLOR_BASED_LUT_11a.bits.snrcbcrlut_10__1_ = snrcbcrlut[10][1];
    V0_COLOR_BASED_LUT_11a.bits.snrcbcrlut_10__2_ = snrcbcrlut[10][2];
    V0_COLOR_BASED_LUT_11a.bits.snrcbcrlut_10__3_ = snrcbcrlut[10][3];
    V0_COLOR_BASED_LUT_11b.bits.snrcbcrlut_10__4_ = snrcbcrlut[10][4];
    V0_COLOR_BASED_LUT_11b.bits.snrcbcrlut_10__5_ = snrcbcrlut[10][5];
    V0_COLOR_BASED_LUT_11b.bits.snrcbcrlut_10__6_ = snrcbcrlut[10][6];
    V0_COLOR_BASED_LUT_11b.bits.snrcbcrlut_10__7_ = snrcbcrlut[10][7];
    V0_COLOR_BASED_LUT_11c.bits.snrcbcrlut_10__8_ = snrcbcrlut[10][8];
    V0_COLOR_BASED_LUT_11c.bits.snrcbcrlut_10__9_ = snrcbcrlut[10][9];
    V0_COLOR_BASED_LUT_11c.bits.snrcbcrlut_10__10_ = snrcbcrlut[10][10];
    V0_COLOR_BASED_LUT_11c.bits.snrcbcrlut_10__11_ = snrcbcrlut[10][11];
    V0_COLOR_BASED_LUT_11d.bits.snrcbcrlut_10__12_ = snrcbcrlut[10][12];
    V0_COLOR_BASED_LUT_11d.bits.snrcbcrlut_10__13_ = snrcbcrlut[10][13];
    V0_COLOR_BASED_LUT_11d.bits.snrcbcrlut_10__14_ = snrcbcrlut[10][14];
    V0_COLOR_BASED_LUT_11d.bits.snrcbcrlut_10__15_ = snrcbcrlut[10][15];
    V0_COLOR_BASED_LUT_12a.bits.snrcbcrlut_11__0_  = snrcbcrlut[11][0];
    V0_COLOR_BASED_LUT_12a.bits.snrcbcrlut_11__1_  = snrcbcrlut[11][1];
    V0_COLOR_BASED_LUT_12a.bits.snrcbcrlut_11__2_  = snrcbcrlut[11][2];
    V0_COLOR_BASED_LUT_12a.bits.snrcbcrlut_11__3_  = snrcbcrlut[11][3];
    V0_COLOR_BASED_LUT_12b.bits.snrcbcrlut_11__4_  = snrcbcrlut[11][4];
    V0_COLOR_BASED_LUT_12b.bits.snrcbcrlut_11__5_  = snrcbcrlut[11][5];
    V0_COLOR_BASED_LUT_12b.bits.snrcbcrlut_11__6_  = snrcbcrlut[11][6];
    V0_COLOR_BASED_LUT_12b.bits.snrcbcrlut_11__7_  = snrcbcrlut[11][7];
    V0_COLOR_BASED_LUT_12c.bits.snrcbcrlut_11__8_  = snrcbcrlut[11][8];
    V0_COLOR_BASED_LUT_12c.bits.snrcbcrlut_11__9_  = snrcbcrlut[11][9];
    V0_COLOR_BASED_LUT_12c.bits.snrcbcrlut_11__10_ = snrcbcrlut[11][10];
    V0_COLOR_BASED_LUT_12c.bits.snrcbcrlut_11__11_ = snrcbcrlut[11][11];
    V0_COLOR_BASED_LUT_12d.bits.snrcbcrlut_11__12_ = snrcbcrlut[11][12];
    V0_COLOR_BASED_LUT_12d.bits.snrcbcrlut_11__13_ = snrcbcrlut[11][13];
    V0_COLOR_BASED_LUT_12d.bits.snrcbcrlut_11__14_ = snrcbcrlut[11][14];
    V0_COLOR_BASED_LUT_12d.bits.snrcbcrlut_11__15_ = snrcbcrlut[11][15];
    V0_COLOR_BASED_LUT_13a.bits.snrcbcrlut_12__0_  = snrcbcrlut[12][0];
    V0_COLOR_BASED_LUT_13a.bits.snrcbcrlut_12__1_  = snrcbcrlut[12][1];
    V0_COLOR_BASED_LUT_13a.bits.snrcbcrlut_12__2_  = snrcbcrlut[12][2];
    V0_COLOR_BASED_LUT_13a.bits.snrcbcrlut_12__3_  = snrcbcrlut[12][3];
    V0_COLOR_BASED_LUT_13b.bits.snrcbcrlut_12__4_  = snrcbcrlut[12][4];
    V0_COLOR_BASED_LUT_13b.bits.snrcbcrlut_12__5_  = snrcbcrlut[12][5];
    V0_COLOR_BASED_LUT_13b.bits.snrcbcrlut_12__6_  = snrcbcrlut[12][6];
    V0_COLOR_BASED_LUT_13b.bits.snrcbcrlut_12__7_  = snrcbcrlut[12][7];
    V0_COLOR_BASED_LUT_13c.bits.snrcbcrlut_12__8_  = snrcbcrlut[12][8];
    V0_COLOR_BASED_LUT_13c.bits.snrcbcrlut_12__9_  = snrcbcrlut[12][9];
    V0_COLOR_BASED_LUT_13c.bits.snrcbcrlut_12__10_ = snrcbcrlut[12][10];
    V0_COLOR_BASED_LUT_13c.bits.snrcbcrlut_12__11_ = snrcbcrlut[12][11];
    V0_COLOR_BASED_LUT_13d.bits.snrcbcrlut_12__12_ = snrcbcrlut[12][12];
    V0_COLOR_BASED_LUT_13d.bits.snrcbcrlut_12__13_ = snrcbcrlut[12][13];
    V0_COLOR_BASED_LUT_13d.bits.snrcbcrlut_12__14_ = snrcbcrlut[12][14];
    V0_COLOR_BASED_LUT_13d.bits.snrcbcrlut_12__15_ = snrcbcrlut[12][15];
    V0_COLOR_BASED_LUT_14a.bits.snrcbcrlut_13__0_  = snrcbcrlut[13][0];
    V0_COLOR_BASED_LUT_14a.bits.snrcbcrlut_13__1_  = snrcbcrlut[13][1];
    V0_COLOR_BASED_LUT_14a.bits.snrcbcrlut_13__2_  = snrcbcrlut[13][2];
    V0_COLOR_BASED_LUT_14a.bits.snrcbcrlut_13__3_  = snrcbcrlut[13][3];
    V0_COLOR_BASED_LUT_14b.bits.snrcbcrlut_13__4_  = snrcbcrlut[13][4];
    V0_COLOR_BASED_LUT_14b.bits.snrcbcrlut_13__5_  = snrcbcrlut[13][5];
    V0_COLOR_BASED_LUT_14b.bits.snrcbcrlut_13__6_  = snrcbcrlut[13][6];
    V0_COLOR_BASED_LUT_14b.bits.snrcbcrlut_13__7_  = snrcbcrlut[13][7];
    V0_COLOR_BASED_LUT_14c.bits.snrcbcrlut_13__8_  = snrcbcrlut[13][8];
    V0_COLOR_BASED_LUT_14c.bits.snrcbcrlut_13__9_  = snrcbcrlut[13][9];
    V0_COLOR_BASED_LUT_14c.bits.snrcbcrlut_13__10_ = snrcbcrlut[13][10];
    V0_COLOR_BASED_LUT_14c.bits.snrcbcrlut_13__11_ = snrcbcrlut[13][11];
    V0_COLOR_BASED_LUT_14d.bits.snrcbcrlut_13__12_ = snrcbcrlut[13][12];
    V0_COLOR_BASED_LUT_14d.bits.snrcbcrlut_13__13_ = snrcbcrlut[13][13];
    V0_COLOR_BASED_LUT_14d.bits.snrcbcrlut_13__14_ = snrcbcrlut[13][14];
    V0_COLOR_BASED_LUT_14d.bits.snrcbcrlut_13__15_ = snrcbcrlut[13][15];
    V0_COLOR_BASED_LUT_15a.bits.snrcbcrlut_14__0_  = snrcbcrlut[14][0];
    V0_COLOR_BASED_LUT_15a.bits.snrcbcrlut_14__1_  = snrcbcrlut[14][1];
    V0_COLOR_BASED_LUT_15a.bits.snrcbcrlut_14__2_  = snrcbcrlut[14][2];
    V0_COLOR_BASED_LUT_15a.bits.snrcbcrlut_14__3_  = snrcbcrlut[14][3];
    V0_COLOR_BASED_LUT_15b.bits.snrcbcrlut_14__4_  = snrcbcrlut[14][4];
    V0_COLOR_BASED_LUT_15b.bits.snrcbcrlut_14__5_  = snrcbcrlut[14][5];
    V0_COLOR_BASED_LUT_15b.bits.snrcbcrlut_14__6_  = snrcbcrlut[14][6];
    V0_COLOR_BASED_LUT_15b.bits.snrcbcrlut_14__7_  = snrcbcrlut[14][7];
    V0_COLOR_BASED_LUT_15c.bits.snrcbcrlut_14__8_  = snrcbcrlut[14][8];
    V0_COLOR_BASED_LUT_15c.bits.snrcbcrlut_14__9_  = snrcbcrlut[14][9];
    V0_COLOR_BASED_LUT_15c.bits.snrcbcrlut_14__10_ = snrcbcrlut[14][10];
    V0_COLOR_BASED_LUT_15c.bits.snrcbcrlut_14__11_ = snrcbcrlut[14][11];
    V0_COLOR_BASED_LUT_15d.bits.snrcbcrlut_14__12_ = snrcbcrlut[14][12];
    V0_COLOR_BASED_LUT_15d.bits.snrcbcrlut_14__13_ = snrcbcrlut[14][13];
    V0_COLOR_BASED_LUT_15d.bits.snrcbcrlut_14__14_ = snrcbcrlut[14][14];
    V0_COLOR_BASED_LUT_15d.bits.snrcbcrlut_14__15_ = snrcbcrlut[14][15];
    V0_COLOR_BASED_LUT_16a.bits.snrcbcrlut_15__0_  = snrcbcrlut[15][0];
    V0_COLOR_BASED_LUT_16a.bits.snrcbcrlut_15__1_  = snrcbcrlut[15][1];
    V0_COLOR_BASED_LUT_16a.bits.snrcbcrlut_15__2_  = snrcbcrlut[15][2];
    V0_COLOR_BASED_LUT_16a.bits.snrcbcrlut_15__3_  = snrcbcrlut[15][3];
    V0_COLOR_BASED_LUT_16b.bits.snrcbcrlut_15__4_  = snrcbcrlut[15][4];
    V0_COLOR_BASED_LUT_16b.bits.snrcbcrlut_15__5_  = snrcbcrlut[15][5];
    V0_COLOR_BASED_LUT_16b.bits.snrcbcrlut_15__6_  = snrcbcrlut[15][6];
    V0_COLOR_BASED_LUT_16b.bits.snrcbcrlut_15__7_  = snrcbcrlut[15][7];
    V0_COLOR_BASED_LUT_16c.bits.snrcbcrlut_15__8_  = snrcbcrlut[15][8];
    V0_COLOR_BASED_LUT_16c.bits.snrcbcrlut_15__9_  = snrcbcrlut[15][9];
    V0_COLOR_BASED_LUT_16c.bits.snrcbcrlut_15__10_ = snrcbcrlut[15][10];
    V0_COLOR_BASED_LUT_16c.bits.snrcbcrlut_15__11_ = snrcbcrlut[15][11];
    V0_COLOR_BASED_LUT_16d.bits.snrcbcrlut_15__12_ = snrcbcrlut[15][12];
    V0_COLOR_BASED_LUT_16d.bits.snrcbcrlut_15__13_ = snrcbcrlut[15][13];
    V0_COLOR_BASED_LUT_16d.bits.snrcbcrlut_15__14_ = snrcbcrlut[15][14];
    V0_COLOR_BASED_LUT_16d.bits.snrcbcrlut_15__15_ = snrcbcrlut[15][15];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_16d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_16c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_16b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_16a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_16a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_15d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_15c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_15b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_15a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_15a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_14d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_14c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_14b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_14a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_14a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_13d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_13c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_13b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_13a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_13a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_12d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_12c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_12b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_12a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_12a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_11d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_11c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_11b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_11a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_11a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_10d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_10c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_10b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_10a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_10a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_09d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_09c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_09b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_09a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_09a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_08d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_08c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_08b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_08a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_08a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_07d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_07c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_07b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_07a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_07a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_06d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_06c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_06b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_06a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_06a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_05d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_05c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_05b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_05a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_05a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_04d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_04c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_04b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_04a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_04a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_03d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_03c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_03b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_03a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_03a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_02d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_02c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_02b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_02a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_02a.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01d.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_01d.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01c.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_01c.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01b.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_01b.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_COLOR_BASED_LUT_01a.u32) + u32Data * VID_OFFSET), V0_COLOR_BASED_LUT_01a.u32);
    return;
}

//SNR CFG END

//DBM CFG START

HI_VOID VDP_VID_SetDbmEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_PRERD V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDbmEnable() select wrong video layer ID\n");
      return ;
    }
    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.dbm_en = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);
    return;
}

HI_VOID VDP_VID_SetDbmDetEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_PRERD V0_PRERD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDbmDetEnable() select wrong video layer ID\n");
      return ;
    }
    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.dbm_det_en = u32Data1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);
    return;
}

HI_VOID VDP_VID_SetDbmProtocol(HI_U32 u32Data, HI_U32 dbm_protocol)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmProtocol Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dbm_protocol = dbm_protocol;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbmOutSelGain(HI_U32 u32Data, HI_U32 dbm_out_sel_gain)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmOutSelGain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dbm_out_sel_gain = dbm_out_sel_gain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbmOutMode(HI_U32 u32Data, HI_U32 dbm_out_mode)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmOutMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dbm_out_mode = dbm_out_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDetVyEn(HI_U32 u32Data, HI_U32 det_vy_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDetVyEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.det_vy_en = det_vy_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDetHcEn(HI_U32 u32Data, HI_U32 det_hc_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDetHcEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.det_hc_en = det_hc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDetHyEn(HI_U32 u32Data, HI_U32 det_hy_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDetHyEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.det_hy_en = det_hy_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbmDemoMode(HI_U32 u32Data, HI_U32 dbm_demo_mode)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmDemoMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dbm_demo_mode = dbm_demo_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbmDemoEn(HI_U32 u32Data, HI_U32 dbm_demo_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmDemoEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dbm_demo_en = dbm_demo_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


//HI_VOID VDP_VID_SetDbChrHorMidGreyEn(HI_U32 u32Data, HI_U32 db_chr_hor_mid_grey_en)
//{
//	U_V0_DB_CTRL V0_DB_CTRL;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbChrHorMidGreyEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL.bits.db_chr_hor_mid_grey_en = db_chr_hor_mid_grey_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbChrHorEn(HI_U32 u32Data, HI_U32 db_chr_hor_en)
//{
//	U_V0_DB_CTRL V0_DB_CTRL;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbChrHorEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL.bits.db_chr_hor_en = db_chr_hor_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbChrVerEn(HI_U32 u32Data, HI_U32 db_chr_ver_en)
//{
//	U_V0_DB_CTRL V0_DB_CTRL;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbChrVerEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL.bits.db_chr_ver_en = db_chr_ver_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
//	
//	return ;
//}


HI_VOID VDP_VID_SetDbLumHorEn(HI_U32 u32Data, HI_U32 db_lum_hor_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHorEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.db_lum_hor_en = db_lum_hor_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbLumVerEn(HI_U32 u32Data, HI_U32 db_lum_ver_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumVerEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.db_lum_ver_en = db_lum_ver_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEn(HI_U32 u32Data, HI_U32 dr_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dr_en = dr_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsEn(HI_U32 u32Data, HI_U32 ds_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.ds_en = ds_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmEn(HI_U32 u32Data, HI_U32 dm_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.dm_en = dm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbEn(HI_U32 u32Data, HI_U32 db_en)
{
	U_V0_DB_CTRL V0_DB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL.bits.db_en = db_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32) + u32Data * VID_OFFSET),V0_DB_CTRL.u32);
	
	return ;
}

#if 0
HI_VOID VDP_VID_SetDbLumHBorderStrLut(HI_U32 u32Data, HI_U32 db_lum_h_border_str_lut_p[136])
{
    U_V0_DB_LUT0 V0_DB_LUT0;
    U_V0_DB_LUT1 V0_DB_LUT1;
    U_V0_DB_LUT2 V0_DB_LUT2;
    U_V0_DB_LUT3 V0_DB_LUT3;
    U_V0_DB_LUT4 V0_DB_LUT4;
    U_V0_DB_LUT5 V0_DB_LUT5;
    U_V0_DB_LUT6 V0_DB_LUT6;
    U_V0_DB_LUT7 V0_DB_LUT7;
    U_V0_DB_LUT8 V0_DB_LUT8;
    U_V0_DB_LUT9 V0_DB_LUT9;
    U_V0_DB_LUT10 V0_DB_LUT10;
    U_V0_DB_LUT11 V0_DB_LUT11;
    U_V0_DB_LUT12 V0_DB_LUT12;
    U_V0_DB_LUT13 V0_DB_LUT13;
    U_V0_DB_LUT14 V0_DB_LUT14;
    U_V0_DB_LUT15 V0_DB_LUT15;
    U_V0_DB_LUT16 V0_DB_LUT16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHBorderStrLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT16.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT15.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT14.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT13.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT12.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT11.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT10.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT9.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT8.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT7.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT6.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT5.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT4.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT0.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT1.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT2.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT3.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p0  = db_lum_h_border_str_lut_p[0];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p1  = db_lum_h_border_str_lut_p[1];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p2  = db_lum_h_border_str_lut_p[2];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p3  = db_lum_h_border_str_lut_p[3];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p4  = db_lum_h_border_str_lut_p[4];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p5  = db_lum_h_border_str_lut_p[5];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p6  = db_lum_h_border_str_lut_p[6];
    V0_DB_LUT0.bits.db_lum_h_border_str_lut_p7  = db_lum_h_border_str_lut_p[7];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p8  = db_lum_h_border_str_lut_p[8];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p9  = db_lum_h_border_str_lut_p[9];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p10 = db_lum_h_border_str_lut_p[10];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p11 = db_lum_h_border_str_lut_p[11];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p12 = db_lum_h_border_str_lut_p[12];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p13 = db_lum_h_border_str_lut_p[13];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p14 = db_lum_h_border_str_lut_p[14];
    V0_DB_LUT1.bits.db_lum_h_border_str_lut_p15 = db_lum_h_border_str_lut_p[15];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p16 = db_lum_h_border_str_lut_p[16];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p17 = db_lum_h_border_str_lut_p[17];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p18 = db_lum_h_border_str_lut_p[18];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p19 = db_lum_h_border_str_lut_p[19];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p20 = db_lum_h_border_str_lut_p[20];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p21 = db_lum_h_border_str_lut_p[21];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p22 = db_lum_h_border_str_lut_p[22];
    V0_DB_LUT2.bits.db_lum_h_border_str_lut_p23 = db_lum_h_border_str_lut_p[23];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p24 = db_lum_h_border_str_lut_p[24];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p25 = db_lum_h_border_str_lut_p[25];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p26 = db_lum_h_border_str_lut_p[26];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p27 = db_lum_h_border_str_lut_p[27];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p28 = db_lum_h_border_str_lut_p[28];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p29 = db_lum_h_border_str_lut_p[29];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p30 = db_lum_h_border_str_lut_p[30];
    V0_DB_LUT3.bits.db_lum_h_border_str_lut_p31 = db_lum_h_border_str_lut_p[31];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p32 = db_lum_h_border_str_lut_p[32];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p33 = db_lum_h_border_str_lut_p[33];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p34 = db_lum_h_border_str_lut_p[34];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p35 = db_lum_h_border_str_lut_p[35];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p36 = db_lum_h_border_str_lut_p[36];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p37 = db_lum_h_border_str_lut_p[37];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p38 = db_lum_h_border_str_lut_p[38];
    V0_DB_LUT4.bits.db_lum_h_border_str_lut_p39 = db_lum_h_border_str_lut_p[39];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p40 = db_lum_h_border_str_lut_p[40];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p41 = db_lum_h_border_str_lut_p[41];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p42 = db_lum_h_border_str_lut_p[42];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p43 = db_lum_h_border_str_lut_p[43];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p44 = db_lum_h_border_str_lut_p[44];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p45 = db_lum_h_border_str_lut_p[45];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p46 = db_lum_h_border_str_lut_p[46];
    V0_DB_LUT5.bits.db_lum_h_border_str_lut_p47 = db_lum_h_border_str_lut_p[47];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p48 = db_lum_h_border_str_lut_p[48];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p49 = db_lum_h_border_str_lut_p[49];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p50 = db_lum_h_border_str_lut_p[50];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p51 = db_lum_h_border_str_lut_p[51];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p52 = db_lum_h_border_str_lut_p[52];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p53 = db_lum_h_border_str_lut_p[53];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p54 = db_lum_h_border_str_lut_p[54];
    V0_DB_LUT6.bits.db_lum_h_border_str_lut_p55 = db_lum_h_border_str_lut_p[55];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p56 = db_lum_h_border_str_lut_p[56];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p57 = db_lum_h_border_str_lut_p[57];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p58 = db_lum_h_border_str_lut_p[58];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p59 = db_lum_h_border_str_lut_p[59];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p60 = db_lum_h_border_str_lut_p[60];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p61 = db_lum_h_border_str_lut_p[61];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p62 = db_lum_h_border_str_lut_p[62];
    V0_DB_LUT7.bits.db_lum_h_border_str_lut_p63 = db_lum_h_border_str_lut_p[63];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p64 = db_lum_h_border_str_lut_p[64];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p65 = db_lum_h_border_str_lut_p[65];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p66 = db_lum_h_border_str_lut_p[66];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p67 = db_lum_h_border_str_lut_p[67];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p68 = db_lum_h_border_str_lut_p[68];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p69 = db_lum_h_border_str_lut_p[69];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p70 = db_lum_h_border_str_lut_p[70];
    V0_DB_LUT8.bits.db_lum_h_border_str_lut_p71 = db_lum_h_border_str_lut_p[71];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p72 = db_lum_h_border_str_lut_p[72];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p73 = db_lum_h_border_str_lut_p[73];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p74 = db_lum_h_border_str_lut_p[74];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p75 = db_lum_h_border_str_lut_p[75];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p76 = db_lum_h_border_str_lut_p[76];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p77 = db_lum_h_border_str_lut_p[77];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p78 = db_lum_h_border_str_lut_p[78];
    V0_DB_LUT9.bits.db_lum_h_border_str_lut_p79 = db_lum_h_border_str_lut_p[79];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p80  = db_lum_h_border_str_lut_p[80];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p81  = db_lum_h_border_str_lut_p[81];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p82  = db_lum_h_border_str_lut_p[82];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p83  = db_lum_h_border_str_lut_p[83];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p84  = db_lum_h_border_str_lut_p[84];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p85  = db_lum_h_border_str_lut_p[85];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p86  = db_lum_h_border_str_lut_p[86];
    V0_DB_LUT10.bits.db_lum_h_border_str_lut_p87  = db_lum_h_border_str_lut_p[87];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p88  = db_lum_h_border_str_lut_p[88];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p89  = db_lum_h_border_str_lut_p[89];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p90  = db_lum_h_border_str_lut_p[90];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p91  = db_lum_h_border_str_lut_p[91];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p92  = db_lum_h_border_str_lut_p[92];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p93  = db_lum_h_border_str_lut_p[93];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p94  = db_lum_h_border_str_lut_p[94];
    V0_DB_LUT11.bits.db_lum_h_border_str_lut_p95  = db_lum_h_border_str_lut_p[95];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p96  = db_lum_h_border_str_lut_p[96];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p97  = db_lum_h_border_str_lut_p[97];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p98  = db_lum_h_border_str_lut_p[98];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p99  = db_lum_h_border_str_lut_p[99];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p100 = db_lum_h_border_str_lut_p[100];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p101 = db_lum_h_border_str_lut_p[101];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p102 = db_lum_h_border_str_lut_p[102];
    V0_DB_LUT12.bits.db_lum_h_border_str_lut_p103 = db_lum_h_border_str_lut_p[103];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p104 = db_lum_h_border_str_lut_p[104];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p105 = db_lum_h_border_str_lut_p[105];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p106 = db_lum_h_border_str_lut_p[106];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p107 = db_lum_h_border_str_lut_p[107];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p108 = db_lum_h_border_str_lut_p[108];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p109 = db_lum_h_border_str_lut_p[109];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p110 = db_lum_h_border_str_lut_p[110];
    V0_DB_LUT13.bits.db_lum_h_border_str_lut_p111 = db_lum_h_border_str_lut_p[111];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p112 = db_lum_h_border_str_lut_p[112];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p113 = db_lum_h_border_str_lut_p[113];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p114 = db_lum_h_border_str_lut_p[114];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p115 = db_lum_h_border_str_lut_p[115];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p116 = db_lum_h_border_str_lut_p[116];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p117 = db_lum_h_border_str_lut_p[117];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p118 = db_lum_h_border_str_lut_p[118];
    V0_DB_LUT14.bits.db_lum_h_border_str_lut_p119 = db_lum_h_border_str_lut_p[119];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p120 = db_lum_h_border_str_lut_p[120];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p121 = db_lum_h_border_str_lut_p[121];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p122 = db_lum_h_border_str_lut_p[122];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p123 = db_lum_h_border_str_lut_p[123];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p124 = db_lum_h_border_str_lut_p[124];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p125 = db_lum_h_border_str_lut_p[125];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p126 = db_lum_h_border_str_lut_p[126];
    V0_DB_LUT15.bits.db_lum_h_border_str_lut_p127 = db_lum_h_border_str_lut_p[127];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p128 = db_lum_h_border_str_lut_p[128];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p129 = db_lum_h_border_str_lut_p[129];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p130 = db_lum_h_border_str_lut_p[130];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p131 = db_lum_h_border_str_lut_p[131];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p132 = db_lum_h_border_str_lut_p[132];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p133 = db_lum_h_border_str_lut_p[133];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p134 = db_lum_h_border_str_lut_p[134];
    V0_DB_LUT16.bits.db_lum_h_border_str_lut_p135 = db_lum_h_border_str_lut_p[135];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT16.u32) + u32Data * VID_OFFSET), V0_DB_LUT16.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT15.u32) + u32Data * VID_OFFSET), V0_DB_LUT15.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT14.u32) + u32Data * VID_OFFSET), V0_DB_LUT14.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT13.u32) + u32Data * VID_OFFSET), V0_DB_LUT13.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT12.u32) + u32Data * VID_OFFSET), V0_DB_LUT12.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT11.u32) + u32Data * VID_OFFSET), V0_DB_LUT11.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT10.u32) + u32Data * VID_OFFSET), V0_DB_LUT10.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT9.u32) + u32Data * VID_OFFSET), V0_DB_LUT9.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT8.u32) + u32Data * VID_OFFSET), V0_DB_LUT8.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT7.u32) + u32Data * VID_OFFSET), V0_DB_LUT7.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT6.u32) + u32Data * VID_OFFSET), V0_DB_LUT6.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT5.u32) + u32Data * VID_OFFSET), V0_DB_LUT5.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT4.u32) + u32Data * VID_OFFSET), V0_DB_LUT4.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT0.u32) + u32Data * VID_OFFSET), V0_DB_LUT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT1.u32) + u32Data * VID_OFFSET), V0_DB_LUT1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT2.u32) + u32Data * VID_OFFSET), V0_DB_LUT2.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT3.u32) + u32Data * VID_OFFSET), V0_DB_LUT3.u32);
    return;
}

HI_VOID VDP_VID_SetDbLumVBorderStrLut(HI_U32 u32Data, HI_U32 db_lum_v_border_str_lut_p[136])
{
    U_V0_DB_LUT17 V0_DB_LUT17;
    U_V0_DB_LUT18 V0_DB_LUT18;
    U_V0_DB_LUT19 V0_DB_LUT19;
    U_V0_DB_LUT20 V0_DB_LUT20;
    U_V0_DB_LUT21 V0_DB_LUT21;
    U_V0_DB_LUT22 V0_DB_LUT22;
    U_V0_DB_LUT23 V0_DB_LUT23;
    U_V0_DB_LUT24 V0_DB_LUT24;
    U_V0_DB_LUT25 V0_DB_LUT25;
    U_V0_DB_LUT26 V0_DB_LUT26;
    U_V0_DB_LUT27 V0_DB_LUT27;
    U_V0_DB_LUT28 V0_DB_LUT28;
    U_V0_DB_LUT29 V0_DB_LUT29;
    U_V0_DB_LUT30 V0_DB_LUT30;
    U_V0_DB_LUT31 V0_DB_LUT31;
    U_V0_DB_LUT32 V0_DB_LUT32;
    U_V0_DB_LUT33 V0_DB_LUT33;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumVBorderStrLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT33.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT33.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT32.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT32.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT31.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT31.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT30.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT30.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT29.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT29.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT28.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT28.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT27.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT27.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT26.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT26.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT25.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT24.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT24.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT23.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT23.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT22.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT22.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT21.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT21.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT20.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT19.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT18.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT17.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p0  = db_lum_v_border_str_lut_p[0];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p1  = db_lum_v_border_str_lut_p[1];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p2  = db_lum_v_border_str_lut_p[2];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p3  = db_lum_v_border_str_lut_p[3];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p4  = db_lum_v_border_str_lut_p[4];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p5  = db_lum_v_border_str_lut_p[5];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p6  = db_lum_v_border_str_lut_p[6];
    V0_DB_LUT17.bits.db_lum_v_border_str_lut_p7  = db_lum_v_border_str_lut_p[7];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p8  = db_lum_v_border_str_lut_p[8];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p9  = db_lum_v_border_str_lut_p[9];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p10 = db_lum_v_border_str_lut_p[10];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p11 = db_lum_v_border_str_lut_p[11];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p12 = db_lum_v_border_str_lut_p[12];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p13 = db_lum_v_border_str_lut_p[13];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p14 = db_lum_v_border_str_lut_p[14];
    V0_DB_LUT18.bits.db_lum_v_border_str_lut_p15 = db_lum_v_border_str_lut_p[15];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p16 = db_lum_v_border_str_lut_p[16];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p17 = db_lum_v_border_str_lut_p[17];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p18 = db_lum_v_border_str_lut_p[18];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p19 = db_lum_v_border_str_lut_p[19];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p20 = db_lum_v_border_str_lut_p[20];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p21 = db_lum_v_border_str_lut_p[21];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p22 = db_lum_v_border_str_lut_p[22];
    V0_DB_LUT19.bits.db_lum_v_border_str_lut_p23 = db_lum_v_border_str_lut_p[23];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p24 = db_lum_v_border_str_lut_p[24];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p25 = db_lum_v_border_str_lut_p[25];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p26 = db_lum_v_border_str_lut_p[26];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p27 = db_lum_v_border_str_lut_p[27];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p28 = db_lum_v_border_str_lut_p[28];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p29 = db_lum_v_border_str_lut_p[29];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p30 = db_lum_v_border_str_lut_p[30];
    V0_DB_LUT20.bits.db_lum_v_border_str_lut_p31 = db_lum_v_border_str_lut_p[31];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p32 = db_lum_v_border_str_lut_p[32];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p33 = db_lum_v_border_str_lut_p[33];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p34 = db_lum_v_border_str_lut_p[34];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p35 = db_lum_v_border_str_lut_p[35];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p36 = db_lum_v_border_str_lut_p[36];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p37 = db_lum_v_border_str_lut_p[37];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p38 = db_lum_v_border_str_lut_p[38];
    V0_DB_LUT21.bits.db_lum_v_border_str_lut_p39 = db_lum_v_border_str_lut_p[39];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p40 = db_lum_v_border_str_lut_p[40];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p41 = db_lum_v_border_str_lut_p[41];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p42 = db_lum_v_border_str_lut_p[42];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p43 = db_lum_v_border_str_lut_p[43];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p44 = db_lum_v_border_str_lut_p[44];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p45 = db_lum_v_border_str_lut_p[45];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p46 = db_lum_v_border_str_lut_p[46];
    V0_DB_LUT22.bits.db_lum_v_border_str_lut_p47 = db_lum_v_border_str_lut_p[47];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p48 = db_lum_v_border_str_lut_p[48];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p49 = db_lum_v_border_str_lut_p[49];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p50 = db_lum_v_border_str_lut_p[50];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p51 = db_lum_v_border_str_lut_p[51];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p52 = db_lum_v_border_str_lut_p[52];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p53 = db_lum_v_border_str_lut_p[53];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p54 = db_lum_v_border_str_lut_p[54];
    V0_DB_LUT23.bits.db_lum_v_border_str_lut_p55 = db_lum_v_border_str_lut_p[55];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p56 = db_lum_v_border_str_lut_p[56];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p57 = db_lum_v_border_str_lut_p[57];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p58 = db_lum_v_border_str_lut_p[58];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p59 = db_lum_v_border_str_lut_p[59];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p60 = db_lum_v_border_str_lut_p[60];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p61 = db_lum_v_border_str_lut_p[61];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p62 = db_lum_v_border_str_lut_p[62];
    V0_DB_LUT24.bits.db_lum_v_border_str_lut_p63 = db_lum_v_border_str_lut_p[63];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p64 = db_lum_v_border_str_lut_p[64];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p65 = db_lum_v_border_str_lut_p[65];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p66 = db_lum_v_border_str_lut_p[66];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p67 = db_lum_v_border_str_lut_p[67];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p68 = db_lum_v_border_str_lut_p[68];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p69 = db_lum_v_border_str_lut_p[69];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p70 = db_lum_v_border_str_lut_p[70];
    V0_DB_LUT25.bits.db_lum_v_border_str_lut_p71 = db_lum_v_border_str_lut_p[71];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p72 = db_lum_v_border_str_lut_p[72];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p73 = db_lum_v_border_str_lut_p[73];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p74 = db_lum_v_border_str_lut_p[74];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p75 = db_lum_v_border_str_lut_p[75];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p76 = db_lum_v_border_str_lut_p[76];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p77 = db_lum_v_border_str_lut_p[77];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p78 = db_lum_v_border_str_lut_p[78];
    V0_DB_LUT26.bits.db_lum_v_border_str_lut_p79 = db_lum_v_border_str_lut_p[79];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p80 = db_lum_v_border_str_lut_p[80];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p81 = db_lum_v_border_str_lut_p[81];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p82 = db_lum_v_border_str_lut_p[82];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p83 = db_lum_v_border_str_lut_p[83];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p84 = db_lum_v_border_str_lut_p[84];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p85 = db_lum_v_border_str_lut_p[85];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p86 = db_lum_v_border_str_lut_p[86];
    V0_DB_LUT27.bits.db_lum_v_border_str_lut_p87 = db_lum_v_border_str_lut_p[87];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p88 = db_lum_v_border_str_lut_p[88];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p89 = db_lum_v_border_str_lut_p[89];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p90 = db_lum_v_border_str_lut_p[90];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p91 = db_lum_v_border_str_lut_p[91];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p92 = db_lum_v_border_str_lut_p[92];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p93 = db_lum_v_border_str_lut_p[93];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p94 = db_lum_v_border_str_lut_p[94];
    V0_DB_LUT28.bits.db_lum_v_border_str_lut_p95 = db_lum_v_border_str_lut_p[95];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p96 = db_lum_v_border_str_lut_p[96];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p97 = db_lum_v_border_str_lut_p[97];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p98 = db_lum_v_border_str_lut_p[98];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p99 = db_lum_v_border_str_lut_p[99];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p100 = db_lum_v_border_str_lut_p[100];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p101 = db_lum_v_border_str_lut_p[101];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p102 = db_lum_v_border_str_lut_p[102];
    V0_DB_LUT29.bits.db_lum_v_border_str_lut_p103 = db_lum_v_border_str_lut_p[103];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p104 = db_lum_v_border_str_lut_p[104];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p105 = db_lum_v_border_str_lut_p[105];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p106 = db_lum_v_border_str_lut_p[106];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p107 = db_lum_v_border_str_lut_p[107];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p108 = db_lum_v_border_str_lut_p[108];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p109 = db_lum_v_border_str_lut_p[109];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p110 = db_lum_v_border_str_lut_p[110];
    V0_DB_LUT30.bits.db_lum_v_border_str_lut_p111 = db_lum_v_border_str_lut_p[111];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p112 = db_lum_v_border_str_lut_p[112];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p113 = db_lum_v_border_str_lut_p[113];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p114 = db_lum_v_border_str_lut_p[114];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p115 = db_lum_v_border_str_lut_p[115];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p116 = db_lum_v_border_str_lut_p[116];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p117 = db_lum_v_border_str_lut_p[117];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p118 = db_lum_v_border_str_lut_p[118];
    V0_DB_LUT31.bits.db_lum_v_border_str_lut_p119 = db_lum_v_border_str_lut_p[119];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p120 = db_lum_v_border_str_lut_p[120];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p121 = db_lum_v_border_str_lut_p[121];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p122 = db_lum_v_border_str_lut_p[122];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p123 = db_lum_v_border_str_lut_p[123];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p124 = db_lum_v_border_str_lut_p[124];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p125 = db_lum_v_border_str_lut_p[125];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p126 = db_lum_v_border_str_lut_p[126];
    V0_DB_LUT32.bits.db_lum_v_border_str_lut_p127 = db_lum_v_border_str_lut_p[127];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p128 = db_lum_v_border_str_lut_p[128];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p129 = db_lum_v_border_str_lut_p[129];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p130 = db_lum_v_border_str_lut_p[130];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p131 = db_lum_v_border_str_lut_p[131];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p132 = db_lum_v_border_str_lut_p[132];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p133 = db_lum_v_border_str_lut_p[133];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p134 = db_lum_v_border_str_lut_p[134];
    V0_DB_LUT33.bits.db_lum_v_border_str_lut_p135 = db_lum_v_border_str_lut_p[135];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT33.u32) + u32Data * VID_OFFSET), V0_DB_LUT33.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT32.u32) + u32Data * VID_OFFSET), V0_DB_LUT32.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT31.u32) + u32Data * VID_OFFSET), V0_DB_LUT31.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT30.u32) + u32Data * VID_OFFSET), V0_DB_LUT30.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT29.u32) + u32Data * VID_OFFSET), V0_DB_LUT29.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT28.u32) + u32Data * VID_OFFSET), V0_DB_LUT28.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT27.u32) + u32Data * VID_OFFSET), V0_DB_LUT27.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT26.u32) + u32Data * VID_OFFSET), V0_DB_LUT26.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT25.u32) + u32Data * VID_OFFSET), V0_DB_LUT25.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT24.u32) + u32Data * VID_OFFSET), V0_DB_LUT24.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT23.u32) + u32Data * VID_OFFSET), V0_DB_LUT23.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT22.u32) + u32Data * VID_OFFSET), V0_DB_LUT22.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT21.u32) + u32Data * VID_OFFSET), V0_DB_LUT21.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT20.u32) + u32Data * VID_OFFSET), V0_DB_LUT20.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT19.u32) + u32Data * VID_OFFSET), V0_DB_LUT19.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT18.u32) + u32Data * VID_OFFSET), V0_DB_LUT18.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT17.u32) + u32Data * VID_OFFSET), V0_DB_LUT17.u32);
    return;
}
#endif

HI_VOID VDP_VID_SetDbLumHStrFadeLut1(HI_U32 u32Data, HI_U32 db_lum_h_str_fade_lut1_p[2])
{
    U_V0_DB_LUT34 V0_DB_LUT34;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHStrFadeLut1 Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT34.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT34.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT34.bits.db_lum_h_str_fade_lut1_p0 = db_lum_h_str_fade_lut1_p[0];
    V0_DB_LUT34.bits.db_lum_h_str_fade_lut1_p1 = db_lum_h_str_fade_lut1_p[1];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT34.u32) + u32Data * VID_OFFSET), V0_DB_LUT34.u32);
    return;
}

HI_VOID VDP_VID_SetDbLumHStrFadeLut2(HI_U32 u32Data, HI_U32 db_lum_h_str_fade_lut2_p[4])
{
    U_V0_DB_LUT35 V0_DB_LUT35;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHStrFadeLut2 Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT35.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT35.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p0 = db_lum_h_str_fade_lut2_p[0];
    V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p1 = db_lum_h_str_fade_lut2_p[1];
    V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p2 = db_lum_h_str_fade_lut2_p[2];
    V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p3 = db_lum_h_str_fade_lut2_p[3];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT35.u32) + u32Data * VID_OFFSET), V0_DB_LUT35.u32);
    return;
}

HI_VOID VDP_VID_SetDbLumHStrFadeLut3(HI_U32 u32Data, HI_U32 db_lum_h_str_fade_lut3_p[8])
{
    U_V0_DB_LUT36 V0_DB_LUT36;
    U_V0_DB_LUT37 V0_DB_LUT37;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHStrFadeLut3 Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT37.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT37.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT36.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT36.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p0 = db_lum_h_str_fade_lut3_p[0];
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p1 = db_lum_h_str_fade_lut3_p[1];
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p2 = db_lum_h_str_fade_lut3_p[2];
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p3 = db_lum_h_str_fade_lut3_p[3];
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p4 = db_lum_h_str_fade_lut3_p[4];
    V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p5 = db_lum_h_str_fade_lut3_p[5];
    V0_DB_LUT37.bits.db_lum_h_str_fade_lut3_p6 = db_lum_h_str_fade_lut3_p[6];
    V0_DB_LUT37.bits.db_lum_h_str_fade_lut3_p7 = db_lum_h_str_fade_lut3_p[7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT37.u32) + u32Data * VID_OFFSET), V0_DB_LUT37.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT36.u32) + u32Data * VID_OFFSET), V0_DB_LUT36.u32);
    return;
}

HI_VOID VDP_VID_SetDbLumHStrFageLut4(HI_U32 u32Data, HI_U32 db_lum_h_str_fade_lut4_p[12])
{
    U_V0_DB_LUT38 V0_DB_LUT38;
    U_V0_DB_LUT39 V0_DB_LUT39;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHStrFageLut4 Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT39.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT39.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT38.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT38.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p0  = db_lum_h_str_fade_lut4_p[0];
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p1  = db_lum_h_str_fade_lut4_p[1];
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p2  = db_lum_h_str_fade_lut4_p[2];
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p3  = db_lum_h_str_fade_lut4_p[3];
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p4  = db_lum_h_str_fade_lut4_p[4];
    V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p5  = db_lum_h_str_fade_lut4_p[5];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p6  = db_lum_h_str_fade_lut4_p[6];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p7  = db_lum_h_str_fade_lut4_p[7];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p8  = db_lum_h_str_fade_lut4_p[8];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p9  = db_lum_h_str_fade_lut4_p[9];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p10 = db_lum_h_str_fade_lut4_p[10];
    V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p11 = db_lum_h_str_fade_lut4_p[11];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT39.u32) + u32Data * VID_OFFSET), V0_DB_LUT39.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT38.u32) + u32Data * VID_OFFSET), V0_DB_LUT38.u32);
    return;
}

HI_VOID VDP_VID_SetDbLumVStrFadeLut(HI_U32 u32Data, HI_U32 db_lum_v_str_fade_lut_p[4])
{
    U_V0_DB_LUT40 V0_DB_LUT40;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumVStrFadeLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT40.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT40.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p0 = db_lum_v_str_fade_lut_p[0];
    V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p1 = db_lum_v_str_fade_lut_p[1];
    //V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p2 = db_lum_v_str_fade_lut_p[2];
    //V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p3 = db_lum_v_str_fade_lut_p[3];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT40.u32) + u32Data * VID_OFFSET), V0_DB_LUT40.u32);
    return;
}

//HI_VOID VDP_VID_SetDbQpStrLut(HI_U32 u32Data, HI_U32 db_qp_str_lut_p[16])
//{
//    U_V0_DB_LUT41 V0_DB_LUT41;
//    U_V0_DB_LUT42 V0_DB_LUT42;
//    U_V0_DB_LUT43 V0_DB_LUT43;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbQpStrLut Select Wrong Layer ID\n");
//		return ;
//	}
//	
//    V0_DB_LUT43.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT43.u32) + u32Data * VID_OFFSET));
//    V0_DB_LUT42.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT42.u32) + u32Data * VID_OFFSET));
//    V0_DB_LUT41.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT41.u32) + u32Data * VID_OFFSET));
//    V0_DB_LUT41.bits.db_qp_str_lut_p0  = db_qp_str_lut_p[0];
//    V0_DB_LUT41.bits.db_qp_str_lut_p1  = db_qp_str_lut_p[1];
//    V0_DB_LUT41.bits.db_qp_str_lut_p2  = db_qp_str_lut_p[2];
//    V0_DB_LUT41.bits.db_qp_str_lut_p3  = db_qp_str_lut_p[3];
//    V0_DB_LUT41.bits.db_qp_str_lut_p4  = db_qp_str_lut_p[4];
//    V0_DB_LUT41.bits.db_qp_str_lut_p5  = db_qp_str_lut_p[5];
//    V0_DB_LUT42.bits.db_qp_str_lut_p6  = db_qp_str_lut_p[6];
//    V0_DB_LUT42.bits.db_qp_str_lut_p7  = db_qp_str_lut_p[7];
//    V0_DB_LUT42.bits.db_qp_str_lut_p8  = db_qp_str_lut_p[8];
//    V0_DB_LUT42.bits.db_qp_str_lut_p9  = db_qp_str_lut_p[9];
//    V0_DB_LUT42.bits.db_qp_str_lut_p10 = db_qp_str_lut_p[10];
//    V0_DB_LUT42.bits.db_qp_str_lut_p11 = db_qp_str_lut_p[11];
//    V0_DB_LUT43.bits.db_qp_str_lut_p12 = db_qp_str_lut_p[12];
//    V0_DB_LUT43.bits.db_qp_str_lut_p13 = db_qp_str_lut_p[13];
//    V0_DB_LUT43.bits.db_qp_str_lut_p14 = db_qp_str_lut_p[14];
//    V0_DB_LUT43.bits.db_qp_str_lut_p15 = db_qp_str_lut_p[15];
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT43.u32) + u32Data * VID_OFFSET), V0_DB_LUT43.u32);
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT42.u32) + u32Data * VID_OFFSET), V0_DB_LUT42.u32);
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT41.u32) + u32Data * VID_OFFSET), V0_DB_LUT41.u32);
//    return;
//}

HI_VOID VDP_VID_SetDbSobelStrLut(HI_U32 u32Data, HI_U32 db_sobel_str_lut_p[26])
{
    U_V0_DB_LUT44 V0_DB_LUT44;
    U_V0_DB_LUT45 V0_DB_LUT45;
    U_V0_DB_LUT46 V0_DB_LUT46;
    U_V0_DB_LUT47 V0_DB_LUT47;
    U_V0_DB_LUT48 V0_DB_LUT48;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbSobelStrLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT48.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT48.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT47.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT47.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT46.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT46.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT45.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT45.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT44.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT44.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT44.bits.db_sobel_str_lut_p0  = db_sobel_str_lut_p[0];
    V0_DB_LUT44.bits.db_sobel_str_lut_p1  = db_sobel_str_lut_p[1];
    V0_DB_LUT44.bits.db_sobel_str_lut_p2  = db_sobel_str_lut_p[2];
    V0_DB_LUT44.bits.db_sobel_str_lut_p3  = db_sobel_str_lut_p[3];
    V0_DB_LUT44.bits.db_sobel_str_lut_p4  = db_sobel_str_lut_p[4];
    V0_DB_LUT44.bits.db_sobel_str_lut_p5  = db_sobel_str_lut_p[5];
    V0_DB_LUT45.bits.db_sobel_str_lut_p6  = db_sobel_str_lut_p[6];
    V0_DB_LUT45.bits.db_sobel_str_lut_p7  = db_sobel_str_lut_p[7];
    V0_DB_LUT45.bits.db_sobel_str_lut_p8  = db_sobel_str_lut_p[8];
    V0_DB_LUT45.bits.db_sobel_str_lut_p9  = db_sobel_str_lut_p[9];
    V0_DB_LUT45.bits.db_sobel_str_lut_p10 = db_sobel_str_lut_p[10];
    V0_DB_LUT45.bits.db_sobel_str_lut_p11 = db_sobel_str_lut_p[11];
    V0_DB_LUT46.bits.db_sobel_str_lut_p12 = db_sobel_str_lut_p[12];
    V0_DB_LUT46.bits.db_sobel_str_lut_p13 = db_sobel_str_lut_p[13];
    V0_DB_LUT46.bits.db_sobel_str_lut_p14 = db_sobel_str_lut_p[14];
    V0_DB_LUT46.bits.db_sobel_str_lut_p15 = db_sobel_str_lut_p[15];
    V0_DB_LUT46.bits.db_sobel_str_lut_p16 = db_sobel_str_lut_p[16];
    V0_DB_LUT46.bits.db_sobel_str_lut_p17 = db_sobel_str_lut_p[17];
    V0_DB_LUT47.bits.db_sobel_str_lut_p18 = db_sobel_str_lut_p[18];
    V0_DB_LUT47.bits.db_sobel_str_lut_p19 = db_sobel_str_lut_p[19];
    V0_DB_LUT47.bits.db_sobel_str_lut_p20 = db_sobel_str_lut_p[20];
    V0_DB_LUT47.bits.db_sobel_str_lut_p21 = db_sobel_str_lut_p[21];
    V0_DB_LUT47.bits.db_sobel_str_lut_p22 = db_sobel_str_lut_p[22];
    V0_DB_LUT47.bits.db_sobel_str_lut_p23 = db_sobel_str_lut_p[23];
    V0_DB_LUT48.bits.db_sobel_str_lut_p24 = db_sobel_str_lut_p[24];
    V0_DB_LUT48.bits.db_sobel_str_lut_p25 = db_sobel_str_lut_p[25];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT48.u32) + u32Data * VID_OFFSET), V0_DB_LUT48.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT47.u32) + u32Data * VID_OFFSET), V0_DB_LUT47.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT46.u32) + u32Data * VID_OFFSET), V0_DB_LUT46.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT45.u32) + u32Data * VID_OFFSET), V0_DB_LUT45.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT44.u32) + u32Data * VID_OFFSET), V0_DB_LUT44.u32);
    return;
}

HI_VOID VDP_VID_SetDbDirStrLut(HI_U32 u32Data, HI_U32 db_dir_str_lut_p[16])
{
    U_V0_DB_LUT49 V0_DB_LUT49;
    U_V0_DB_LUT50 V0_DB_LUT50;
    U_V0_DB_LUT51 V0_DB_LUT51;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDirStrLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT51.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT51.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT50.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT50.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT49.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT49.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT49.bits.db_dir_str_lut_p0  = db_dir_str_lut_p[0];
    V0_DB_LUT49.bits.db_dir_str_lut_p1  = db_dir_str_lut_p[1];
    V0_DB_LUT49.bits.db_dir_str_lut_p2  = db_dir_str_lut_p[2];
    V0_DB_LUT49.bits.db_dir_str_lut_p3  = db_dir_str_lut_p[3];
    V0_DB_LUT49.bits.db_dir_str_lut_p4  = db_dir_str_lut_p[4];
    V0_DB_LUT49.bits.db_dir_str_lut_p5  = db_dir_str_lut_p[5];
    V0_DB_LUT50.bits.db_dir_str_lut_p6  = db_dir_str_lut_p[6];
    V0_DB_LUT50.bits.db_dir_str_lut_p7  = db_dir_str_lut_p[7];
    V0_DB_LUT50.bits.db_dir_str_lut_p8  = db_dir_str_lut_p[8];
    V0_DB_LUT50.bits.db_dir_str_lut_p9  = db_dir_str_lut_p[9];
    V0_DB_LUT50.bits.db_dir_str_lut_p10 = db_dir_str_lut_p[10];
    V0_DB_LUT50.bits.db_dir_str_lut_p11 = db_dir_str_lut_p[11];
    V0_DB_LUT51.bits.db_dir_str_lut_p12 = db_dir_str_lut_p[12];
    V0_DB_LUT51.bits.db_dir_str_lut_p13 = db_dir_str_lut_p[13];
    V0_DB_LUT51.bits.db_dir_str_lut_p14 = db_dir_str_lut_p[14];
    V0_DB_LUT51.bits.db_dir_str_lut_p15 = db_dir_str_lut_p[15];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT51.u32) + u32Data * VID_OFFSET), V0_DB_LUT51.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT50.u32) + u32Data * VID_OFFSET), V0_DB_LUT50.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT49.u32) + u32Data * VID_OFFSET), V0_DB_LUT49.u32);
    return;
}

HI_VOID VDP_VID_SetDbDirStrGainLut(HI_U32 u32Data, HI_U32 db_dir_str_gain_lut_p[10])
{
    U_V0_DB_LUT52 V0_DB_LUT52;
    U_V0_DB_LUT53 V0_DB_LUT53;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDirStrGainLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT53.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT53.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT52.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT52.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p0 = db_dir_str_gain_lut_p[0];
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p1 = db_dir_str_gain_lut_p[1];
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p2 = db_dir_str_gain_lut_p[2];
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p3 = db_dir_str_gain_lut_p[3];
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p4 = db_dir_str_gain_lut_p[4];
    V0_DB_LUT52.bits.db_dir_str_gain_lut_p5 = db_dir_str_gain_lut_p[5];
    V0_DB_LUT53.bits.db_dir_str_gain_lut_p6 = db_dir_str_gain_lut_p[6];
    V0_DB_LUT53.bits.db_dir_str_gain_lut_p7 = db_dir_str_gain_lut_p[7];
    V0_DB_LUT53.bits.db_dir_str_gain_lut_p8 = db_dir_str_gain_lut_p[8];
    V0_DB_LUT53.bits.db_dir_str_gain_lut_p9 = db_dir_str_gain_lut_p[9];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT53.u32) + u32Data * VID_OFFSET), V0_DB_LUT53.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT52.u32) + u32Data * VID_OFFSET), V0_DB_LUT52.u32);
    return;
}

HI_VOID VDP_VID_SetDbDiffMapLut(HI_U32 u32Data, HI_U32 db_diff_map_lut_p[8])
{
    U_V0_DB_LUT54 V0_DB_LUT54;
    U_V0_DB_LUT55 V0_DB_LUT55;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDiffMapLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT55.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT55.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT54.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT54.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT54.bits.db_diff_map_lut_p0 = db_diff_map_lut_p[0];
    V0_DB_LUT54.bits.db_diff_map_lut_p1 = db_diff_map_lut_p[1];
    V0_DB_LUT54.bits.db_diff_map_lut_p2 = db_diff_map_lut_p[2];
    V0_DB_LUT54.bits.db_diff_map_lut_p3 = db_diff_map_lut_p[3];
    V0_DB_LUT55.bits.db_diff_map_lut_p4 = db_diff_map_lut_p[4];
    V0_DB_LUT55.bits.db_diff_map_lut_p5 = db_diff_map_lut_p[5];
    V0_DB_LUT55.bits.db_diff_map_lut_p6 = db_diff_map_lut_p[6];
    V0_DB_LUT55.bits.db_diff_map_lut_p7 = db_diff_map_lut_p[7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT55.u32) + u32Data * VID_OFFSET), V0_DB_LUT55.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT54.u32) + u32Data * VID_OFFSET), V0_DB_LUT54.u32);
    return;
}

HI_VOID VDP_VID_SetDbDiffWeightLut(HI_U32 u32Data, HI_U32 db_diff_weight_lut_p[8])
{
    U_V0_DB_LUT56 V0_DB_LUT56;
    U_V0_DB_LUT57 V0_DB_LUT57;
    //U_V0_DB_LUT58 V0_DB_LUT58;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDiffWeightLut Select Wrong Layer ID\n");
		return ;
	}
	
    //V0_DB_LUT58.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT58.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT57.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT57.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT56.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT56.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT56.bits.db_diff_weight_lut_p0 = db_diff_weight_lut_p[0];
    V0_DB_LUT56.bits.db_diff_weight_lut_p1 = db_diff_weight_lut_p[1];
    V0_DB_LUT56.bits.db_diff_weight_lut_p2 = db_diff_weight_lut_p[2];
    V0_DB_LUT57.bits.db_diff_weight_lut_p3 = db_diff_weight_lut_p[3];
    V0_DB_LUT57.bits.db_diff_weight_lut_p4 = db_diff_weight_lut_p[4];
    V0_DB_LUT57.bits.db_diff_weight_lut_p5 = db_diff_weight_lut_p[5];
    //V0_DB_LUT58.bits.db_diff_weight_lut_p6 = db_diff_weight_lut_p[6];
    //V0_DB_LUT58.bits.db_diff_weight_lut_p7 = db_diff_weight_lut_p[7];
    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT58.u32) + u32Data * VID_OFFSET), V0_DB_LUT58.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT57.u32) + u32Data * VID_OFFSET), V0_DB_LUT57.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT56.u32) + u32Data * VID_OFFSET), V0_DB_LUT56.u32);
    return;
}

HI_VOID VDP_VID_SetDbDiffStrLut(HI_U32 u32Data, HI_U32 db_diff_str_lut_p[8])
{
    U_V0_DB_LUT59 V0_DB_LUT59;
    U_V0_DB_LUT60 V0_DB_LUT60;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDiffStrLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT60.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT60.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT59.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT59.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT59.bits.db_diff_str_lut_p0 = db_diff_str_lut_p[0];
    V0_DB_LUT59.bits.db_diff_str_lut_p1 = db_diff_str_lut_p[1];
    V0_DB_LUT59.bits.db_diff_str_lut_p2 = db_diff_str_lut_p[2];
    V0_DB_LUT59.bits.db_diff_str_lut_p3 = db_diff_str_lut_p[3];
    V0_DB_LUT59.bits.db_diff_str_lut_p4 = db_diff_str_lut_p[4];
    V0_DB_LUT60.bits.db_diff_str_lut_p5 = db_diff_str_lut_p[5];
    V0_DB_LUT60.bits.db_diff_str_lut_p6 = db_diff_str_lut_p[6];
    //V0_DB_LUT60.bits.db_diff_str_lut_p7 = db_diff_str_lut_p[7];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT60.u32) + u32Data * VID_OFFSET), V0_DB_LUT60.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT59.u32) + u32Data * VID_OFFSET), V0_DB_LUT59.u32);
    return;
}

HI_VOID VDP_VID_SetDbStrethEn(HI_U32 u32Data, HI_U32 db_streth_en)
{
	U_V0_DB_CTRL1 V0_DB_CTRL1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbStrethEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL1.bits.db_streth_en = db_streth_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbBlkPosMode(HI_U32 u32Data, HI_U32 db_blk_pos_mode)
{
	U_V0_DB_CTRL1 V0_DB_CTRL1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbBlkPosMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL1.bits.db_blk_pos_mode = db_blk_pos_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbBlkMode(HI_U32 u32Data, HI_U32 db_blk_mode)
{
	U_V0_DB_CTRL1 V0_DB_CTRL1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbBlkMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL1.bits.db_blk_mode = db_blk_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
	
	return ;
}


//HI_VOID VDP_VID_SetDbFltStrCalcMode(HI_U32 u32Data, HI_U32 db_flt_str_calc_mode)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbFltStrCalcMode Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_flt_str_calc_mode = db_flt_str_calc_mode;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbImageStruct(HI_U32 u32Data, HI_U32 db_image_struct)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbImageStruct Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_image_struct = db_image_struct;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbMp2QScaleType(HI_U32 u32Data, HI_U32 db_mp2_q_scale_type)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbMp2QScaleType Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_mp2_q_scale_type = db_mp2_q_scale_type;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbTopOrBot(HI_U32 u32Data, HI_U32 db_top_or_bot)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbTopOrBot Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_top_or_bot = db_top_or_bot;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}


HI_VOID VDP_VID_SetDbVdhTransInfoEn(HI_U32 u32Data, HI_U32 db_vdh_trans_info_en)
{
	U_V0_DB_CTRL1 V0_DB_CTRL1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbVdhTransInfoEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL1.bits.db_vdh_trans_info_en = db_vdh_trans_info_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbDetEn(HI_U32 u32Data, HI_U32 db_det_en)
{
	U_V0_DB_CTRL1 V0_DB_CTRL1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDetEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
	V0_DB_CTRL1.bits.db_det_en = db_det_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
	
	return ;
}


//HI_VOID VDP_VID_SetDbSingleEn(HI_U32 u32Data, HI_U32 db_single_en)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbSingleEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_single_en = db_single_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbDeiEn(HI_U32 u32Data, HI_U32 db_dei_en)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbDeiEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_dei_en = db_dei_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//HI_VOID VDP_VID_SetDbLumVbsCalcMode(HI_U32 u32Data, HI_U32 db_lum_vbs_calc_mode)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbLumVbsCalcMode Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_lum_vbs_calc_mode = db_lum_vbs_calc_mode;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbLumHbsCalcMode(HI_U32 u32Data, HI_U32 db_lum_hbs_calc_mode)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbLumHbsCalcMode Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_lum_hbs_calc_mode = db_lum_hbs_calc_mode;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbQpEn(HI_U32 u32Data, HI_U32 db_qp_en)
//{
//	U_V0_DB_CTRL1 V0_DB_CTRL1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbQpEn Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_CTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET));
//	V0_DB_CTRL1.bits.db_qp_en = db_qp_en;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32) + u32Data * VID_OFFSET),V0_DB_CTRL1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbAvgQp(HI_U32 u32Data, HI_U32 db_avg_qp)
//{
//	U_V0_DB_THR0 V0_DB_THR0;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbAvgQp Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR0.bits.db_avg_qp = db_avg_qp;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
//	
//	return ;
//}


HI_VOID VDP_VID_SetDbCtrstThresh(HI_U32 u32Data, HI_U32 db_ctrst_thresh)
{
	U_V0_DB_THR0 V0_DB_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbCtrstThresh Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
	V0_DB_THR0.bits.db_ctrst_thresh = db_ctrst_thresh;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbGradSubRatio(HI_U32 u32Data, HI_U32 db_grad_sub_ratio)
{
	U_V0_DB_THR0 V0_DB_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbGradSubRatio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
	V0_DB_THR0.bits.db_grad_sub_ratio = db_grad_sub_ratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbDirSmoothMode(HI_U32 u32Data, HI_U32 db_dir_smooth_mode)
{
	U_V0_DB_THR0 V0_DB_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDirSmoothMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
	V0_DB_THR0.bits.db_dir_smooth_mode = db_dir_smooth_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbLumVBorderStrTh(HI_U32 u32Data, HI_U32 db_lum_v_border_str_th)
{
	U_V0_DB_THR0 V0_DB_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumVBorderStrTh Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
	V0_DB_THR0.bits.db_lum_v_border_str_th = db_lum_v_border_str_th;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbLumHBorderStrTh(HI_U32 u32Data, HI_U32 db_lum_h_border_str_th)
{
	U_V0_DB_THR0 V0_DB_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHBorderStrTh Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET));
	V0_DB_THR0.bits.db_lum_h_border_str_th = db_lum_h_border_str_th;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR0.u32) + u32Data * VID_OFFSET),V0_DB_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbDetectRatioV(HI_U32 u32Data, HI_U32 db_detect_ratio_v)
{
	U_V0_DB_THR1 V0_DB_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDetectRatioV Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
	V0_DB_THR1.bits.db_detect_ratio_v = db_detect_ratio_v;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbDetectRatioH(HI_U32 u32Data, HI_U32 db_detect_ratio_h)
{
	U_V0_DB_THR1 V0_DB_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDetectRatioH Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
	V0_DB_THR1.bits.db_detect_ratio_h = db_detect_ratio_h;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbTextStrGain(HI_U32 u32Data, HI_U32 db_text_str_gain)
{
	U_V0_DB_THR1 V0_DB_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbTextStrGain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
	V0_DB_THR1.bits.db_text_str_gain = db_text_str_gain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
	
	return ;
}


//HI_VOID VDP_VID_SetDbTextAdpGain(HI_U32 u32Data, HI_U32 db_text_adp_gain)
//{
//	U_V0_DB_THR1 V0_DB_THR1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbTextAdpGain Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR1.bits.db_text_adp_gain = db_text_adp_gain;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbSkipGain(HI_U32 u32Data, HI_U32 db_skip_gain)
//{
//	U_V0_DB_THR1 V0_DB_THR1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbSkipGain Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR1.bits.db_skip_gain = db_skip_gain;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbMaxLumHorDbDist(HI_U32 u32Data, HI_U32 db_max_lum_hor_db_dist)
//{
//	U_V0_DB_THR1 V0_DB_THR1;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbMaxLumHorDbDist Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR1.bits.db_max_lum_hor_db_dist = db_max_lum_hor_db_dist;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR1.u32) + u32Data * VID_OFFSET),V0_DB_THR1.u32);
//	
//	return ;
//}


HI_VOID VDP_VID_SetDbSttOffsetY(HI_U32 u32Data, HI_U32 db_stt_offset_y)
{
	U_V0_DB_OST0 V0_DB_OST0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbSttOffsetY Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_OST0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_OST0.u32) + u32Data * VID_OFFSET));
	V0_DB_OST0.bits.db_stt_offset_y = db_stt_offset_y;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_OST0.u32) + u32Data * VID_OFFSET),V0_DB_OST0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbSttOffsetX(HI_U32 u32Data, HI_U32 db_stt_offset_x)
{
	U_V0_DB_OST0 V0_DB_OST0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbSttOffsetX Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_OST0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_OST0.u32) + u32Data * VID_OFFSET));
	V0_DB_OST0.bits.db_stt_offset_x = db_stt_offset_x;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_OST0.u32) + u32Data * VID_OFFSET),V0_DB_OST0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbEndOffsetY(HI_U32 u32Data, HI_U32 db_end_offset_y)
{
	U_V0_DB_OST1 V0_DB_OST1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbEndOffsetY Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_OST1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_OST1.u32) + u32Data * VID_OFFSET));
	V0_DB_OST1.bits.db_end_offset_y = db_end_offset_y;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_OST1.u32) + u32Data * VID_OFFSET),V0_DB_OST1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbEndOffsetX(HI_U32 u32Data, HI_U32 db_end_offset_x)
{
	U_V0_DB_OST1 V0_DB_OST1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbEndOffsetX Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_OST1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_OST1.u32) + u32Data * VID_OFFSET));
	V0_DB_OST1.bits.db_end_offset_x = db_end_offset_x;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_OST1.u32) + u32Data * VID_OFFSET),V0_DB_OST1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbDemoPosX(HI_U32 u32Data, HI_U32 db_demo_pos_x)
{
	U_V0_DB_DEMO V0_DB_DEMO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbDemoPosX Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_DEMO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_DEMO.u32) + u32Data * VID_OFFSET));
	V0_DB_DEMO.bits.db_demo_pos_x = db_demo_pos_x;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_DEMO.u32) + u32Data * VID_OFFSET),V0_DB_DEMO.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbLumHorDeltaLut(HI_U32 u32Data,HI_U32 db_lum_hor_delta_lut_p[16])
{
    U_V0_DB_LUT61 V0_DB_LUT61;
    U_V0_DB_LUT62 V0_DB_LUT62;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_DRV_SetDbLumHorDeltaLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT61.u32 =VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT61.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT62.u32 =VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT62.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p0 = db_lum_hor_delta_lut_p[0];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p1 = db_lum_hor_delta_lut_p[1];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p2 = db_lum_hor_delta_lut_p[2];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p3 = db_lum_hor_delta_lut_p[3];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p4 = db_lum_hor_delta_lut_p[4];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p5 = db_lum_hor_delta_lut_p[5];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p6 = db_lum_hor_delta_lut_p[6];
    V0_DB_LUT61.bits.db_lum_hor_delta_lut_p7 = db_lum_hor_delta_lut_p[7];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p8 = db_lum_hor_delta_lut_p[8];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p9 = db_lum_hor_delta_lut_p[9];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p10 = db_lum_hor_delta_lut_p[10];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p11 = db_lum_hor_delta_lut_p[11];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p12 = db_lum_hor_delta_lut_p[12];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p13 = db_lum_hor_delta_lut_p[13];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p14 = db_lum_hor_delta_lut_p[14];
    V0_DB_LUT62.bits.db_lum_hor_delta_lut_p15 = db_lum_hor_delta_lut_p[15];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT61.u32) + u32Data * VID_OFFSET), V0_DB_LUT61.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT62.u32) + u32Data * VID_OFFSET), V0_DB_LUT62.u32);
return;
}
HI_VOID VDP_VID_SetDbLumVerDeltaLut(HI_U32 u32Data,HI_U32 db_lum_ver_delta_lut_p[16])
{
    U_V0_DB_LUT63 V0_DB_LUT63;
    U_V0_DB_LUT64 V0_DB_LUT64;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_DRV_SetDbLumVerDeltaLut Select Wrong Layer ID\n");
		return ;
	}
	
    V0_DB_LUT63.u32 =VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT63.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT64.u32 =VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_LUT64.u32) + u32Data * VID_OFFSET));
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p0 = db_lum_ver_delta_lut_p[0];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p1 = db_lum_ver_delta_lut_p[1];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p2 = db_lum_ver_delta_lut_p[2];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p3 = db_lum_ver_delta_lut_p[3];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p4 = db_lum_ver_delta_lut_p[4];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p5 = db_lum_ver_delta_lut_p[5];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p6 = db_lum_ver_delta_lut_p[6];
    V0_DB_LUT63.bits.db_lum_ver_delta_lut_p7 = db_lum_ver_delta_lut_p[7];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p8 = db_lum_ver_delta_lut_p[8];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p9 = db_lum_ver_delta_lut_p[9];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p10 = db_lum_ver_delta_lut_p[10];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p11 = db_lum_ver_delta_lut_p[11];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p12 = db_lum_ver_delta_lut_p[12];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p13 = db_lum_ver_delta_lut_p[13];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p14 = db_lum_ver_delta_lut_p[14];
    V0_DB_LUT64.bits.db_lum_ver_delta_lut_p15 = db_lum_ver_delta_lut_p[15];
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT63.u32) + u32Data * VID_OFFSET), V0_DB_LUT63.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_LUT64.u32) + u32Data * VID_OFFSET), V0_DB_LUT64.u32);
return;
}

//HI_VOID VDP_VID_SetDbTextAdpGain2(HI_U32 u32Data, HI_U32 db_text_adp_gain2)
//{
//	U_V0_DB_THR2 V0_DB_THR2;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbTextAdpGain2 Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR2.bits.db_text_adp_gain2 = db_text_adp_gain2;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbTextAdpGain1(HI_U32 u32Data, HI_U32 db_text_adp_gain1)
//{
//	U_V0_DB_THR2 V0_DB_THR2;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbTextAdpGain1 Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR2.bits.db_text_adp_gain1 = db_text_adp_gain1;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbFltAdpGain2(HI_U32 u32Data, HI_U32 db_flt_adp_gain2)
//{
//	U_V0_DB_THR2 V0_DB_THR2;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbFltAdpGain2 Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR2.bits.db_flt_adp_gain2 = db_flt_adp_gain2;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbFltAdpGain1(HI_U32 u32Data, HI_U32 db_flt_adp_gain1)
//{
//	U_V0_DB_THR2 V0_DB_THR2;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbFltAdpGain1 Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR2.bits.db_flt_adp_gain1 = db_flt_adp_gain1;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
//	
//	return ;
//}
//
//
//HI_VOID VDP_VID_SetDbFltAdpGain(HI_U32 u32Data, HI_U32 db_flt_adp_gain)
//{
//	U_V0_DB_THR2 V0_DB_THR2;
//	
//	if(u32Data >= VID_MAX)
//	{
//		VDP_PRINT("Error, VDP_VID_SetDbFltAdpGain Select Wrong Layer ID\n");
//		return ;
//	}
//	
//	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
//	V0_DB_THR2.bits.db_flt_adp_gain = db_flt_adp_gain;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
//	
//	return ;
//}


HI_VOID VDP_VID_SetDbGlobalDbStrengthLum(HI_U32 u32Data, HI_U32 db_global_db_strength_lum)
{
	U_V0_DB_THR2 V0_DB_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbGlobalDbStrengthLum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET));
	V0_DB_THR2.bits.db_global_db_strength_lum = db_global_db_strength_lum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR2.u32) + u32Data * VID_OFFSET),V0_DB_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbBlkHeight(HI_U32 u32Data, HI_U32 db_blk_height)
{
	U_V0_DB_INFO V0_DB_INFO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbBlkHeight Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_INFO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_INFO.u32) + u32Data * VID_OFFSET));
	V0_DB_INFO.bits.db_blk_height = db_blk_height-1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_INFO.u32) + u32Data * VID_OFFSET),V0_DB_INFO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDbBlkWidth(HI_U32 u32Data, HI_U32 db_blk_width)
{
	U_V0_DB_INFO V0_DB_INFO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbBlkWidth Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_INFO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_INFO.u32) + u32Data * VID_OFFSET));
	V0_DB_INFO.bits.db_blk_width = db_blk_width-1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_INFO.u32) + u32Data * VID_OFFSET),V0_DB_INFO.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbLumHorGain2(HI_U32 u32Data, HI_U32 db_lum_hor_gain2)
{
	U_V0_DB_THR3 V0_DB_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHorGain2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET));
	V0_DB_THR3.bits.db_lum_hor_gain2 = db_lum_hor_gain2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET),V0_DB_THR3.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbLumHorGain1(HI_U32 u32Data, HI_U32 db_lum_hor_gain1)
{
	U_V0_DB_THR3 V0_DB_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHorGain1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET));
	V0_DB_THR3.bits.db_lum_hor_gain1 = db_lum_hor_gain1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET),V0_DB_THR3.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDbLumHorVarCore(HI_U32 u32Data, HI_U32 db_lum_hor_var_core)
{
	U_V0_DB_THR3 V0_DB_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbLumHorVarCore Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DB_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET));
	V0_DB_THR3.bits.db_lum_hor_var_core = db_lum_hor_var_core;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_THR3.u32) + u32Data * VID_OFFSET),V0_DB_THR3.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDBDThdEdge(HI_U32 u32Data,HI_U32 hy_thd_edge,HI_U32 hc_thd_edge, HI_U32 vy_thd_edge)
{
    U_V0_DBD_THDEDGE   V0_DBD_THDEDGE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDThdEdge() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_THDEDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_THDEDGE.u32) + u32Data * VID_OFFSET));
    V0_DBD_THDEDGE.bits.dbd_hy_thd_edge = hy_thd_edge ;
    V0_DBD_THDEDGE.bits.dbd_hc_thd_edge = hc_thd_edge ;
    V0_DBD_THDEDGE.bits.dbd_vy_thd_edge = vy_thd_edge ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_THDEDGE.u32) + u32Data * VID_OFFSET), V0_DBD_THDEDGE.u32);
    return;
}
HI_VOID VDP_VID_SetDBDThdTxt(HI_U32 u32Data,HI_U32 hy_thd_txt,HI_U32 hc_thd_txt, HI_U32 vy_thd_txt)
{
    U_V0_DBD_THDTXT V0_DBD_THDTXT;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDThdTxt() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_THDTXT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_THDTXT.u32) + u32Data * VID_OFFSET));
    V0_DBD_THDTXT.bits.dbd_hy_thd_txt = hy_thd_txt ;
    V0_DBD_THDTXT.bits.dbd_hc_thd_txt = hc_thd_txt ;
    V0_DBD_THDTXT.bits.dbd_vy_thd_txt = vy_thd_txt ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_THDTXT.u32) + u32Data * VID_OFFSET), V0_DBD_THDTXT.u32);
    return;
}
HI_VOID VDP_VID_SetDBDDetWgtLut(HI_U32 u32Data,HI_U32 det_lut_wgt[16])
{
    U_V0_DBD_LUTWGT0  V0_DBD_LUTWGT0;
    U_V0_DBD_LUTWGT1  V0_DBD_LUTWGT1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDDetWgtLut() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_LUTWGT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT0.u32) + u32Data * VID_OFFSET));
    V0_DBD_LUTWGT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT1.u32) + u32Data * VID_OFFSET));
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt0  = det_lut_wgt[0] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt1  = det_lut_wgt[1] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt2  = det_lut_wgt[2] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt3  = det_lut_wgt[3] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt4  = det_lut_wgt[4] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt5  = det_lut_wgt[5] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt6  = det_lut_wgt[6] ;
    V0_DBD_LUTWGT0.bits.dbd_det_lut_wgt7  = det_lut_wgt[7] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt8  = det_lut_wgt[8] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt9  = det_lut_wgt[9] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt10 = det_lut_wgt[10] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt11 = det_lut_wgt[11] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt12 = det_lut_wgt[12] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt13 = det_lut_wgt[13] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt14 = det_lut_wgt[14] ;
    V0_DBD_LUTWGT1.bits.dbd_det_lut_wgt15 = det_lut_wgt[15] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT0.u32) + u32Data * VID_OFFSET), V0_DBD_LUTWGT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT1.u32) + u32Data * VID_OFFSET), V0_DBD_LUTWGT1.u32);
    return;
}
HI_VOID VDP_VID_SetDBDRtnBdTxt(HI_U32 u32Data,HI_U32 hy_rtn_bd_txt,HI_U32 hc_rtn_bd_txt, HI_U32 vy_rtn_bd_txt)
{
    U_V0_DBD_RTNBDTXT V0_DBD_RTNBDTXT;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDRtnBdTxt() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_RTNBDTXT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_RTNBDTXT.u32) + u32Data * VID_OFFSET));
    V0_DBD_RTNBDTXT.bits.dbd_hy_rtn_bd_txt = hy_rtn_bd_txt ;
    V0_DBD_RTNBDTXT.bits.dbd_hc_rtn_bd_txt = hc_rtn_bd_txt ;
    V0_DBD_RTNBDTXT.bits.dbd_vy_rtn_bd_txt = vy_rtn_bd_txt ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_RTNBDTXT.u32) + u32Data * VID_OFFSET), V0_DBD_RTNBDTXT.u32);
    return;
}
HI_VOID VDP_VID_SetDBDTstBlkNum(HI_U32 u32Data,HI_U32 hy_tst_blk_num,HI_U32 hc_tst_blk_num, HI_U32 vy_tst_blk_num)
{
    U_V0_DBD_TSTBLKNUM  V0_DBD_TSTBLKNUM;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDTstBlkNum() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_TSTBLKNUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_TSTBLKNUM.u32) + u32Data * VID_OFFSET));
    V0_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num = hy_tst_blk_num ;
    V0_DBD_TSTBLKNUM.bits.dbd_hc_tst_blk_num = hc_tst_blk_num ;
    V0_DBD_TSTBLKNUM.bits.dbd_vy_tst_blk_num = vy_tst_blk_num ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_TSTBLKNUM.u32) + u32Data * VID_OFFSET), V0_DBD_TSTBLKNUM.u32);
    return;
}
HI_VOID VDP_VID_SetDBDHorLumaRdx8BinLut(HI_U32 u32Data,HI_U32 hy_rdx8bin_lut[7])
{
    U_V0_DBD_HYRDX8BINLUT0 V0_DBD_HYRDX8BINLUT0;
    U_V0_DBD_HYRDX8BINLUT1 V0_DBD_HYRDX8BINLUT1;
    U_V0_DBD_HYRDX8BINLUT2 V0_DBD_HYRDX8BINLUT2;
    U_V0_DBD_HYRDX8BINLUT3 V0_DBD_HYRDX8BINLUT3;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDHorLumaRdx8BinLut() select wrong video layer ID\n");
      return ;
    }
   
    V0_DBD_HYRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT0.u32) + u32Data * VID_OFFSET));
    V0_DBD_HYRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT1.u32) + u32Data * VID_OFFSET));
    V0_DBD_HYRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT2.u32) + u32Data * VID_OFFSET));
    V0_DBD_HYRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT3.u32) + u32Data * VID_OFFSET));
    V0_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut0 = hy_rdx8bin_lut[0] ;
    V0_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut1 = hy_rdx8bin_lut[1] ;
    V0_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut2 = hy_rdx8bin_lut[2] ;
    V0_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut3 = hy_rdx8bin_lut[3] ;
    V0_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut4 = hy_rdx8bin_lut[4] ;
    V0_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut5 = hy_rdx8bin_lut[5] ;
    V0_DBD_HYRDX8BINLUT3.bits.dbd_hy_rdx8bin_lut6 = hy_rdx8bin_lut[6] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT0.u32) + u32Data * VID_OFFSET), V0_DBD_HYRDX8BINLUT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT1.u32) + u32Data * VID_OFFSET), V0_DBD_HYRDX8BINLUT1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT2.u32) + u32Data * VID_OFFSET), V0_DBD_HYRDX8BINLUT2.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT3.u32) + u32Data * VID_OFFSET), V0_DBD_HYRDX8BINLUT3.u32);
    return;
}
HI_VOID VDP_VID_SetDBDHorChromRdx8BinLut(HI_U32 u32Data,HI_U32 hc_rdx8bin_lut[7])
{
    U_V0_DBD_HCRDX8BINLUT0 V0_DBD_HCRDX8BINLUT0;
    U_V0_DBD_HCRDX8BINLUT1 V0_DBD_HCRDX8BINLUT1;
    U_V0_DBD_HCRDX8BINLUT2 V0_DBD_HCRDX8BINLUT2;
    U_V0_DBD_HCRDX8BINLUT3 V0_DBD_HCRDX8BINLUT3;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDHorChromRdx8BinLut() select wrong video layer ID\n");
      return ;
    }
   
    V0_DBD_HCRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT0.u32) + u32Data * VID_OFFSET));
    V0_DBD_HCRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT1.u32) + u32Data * VID_OFFSET));
    V0_DBD_HCRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT2.u32) + u32Data * VID_OFFSET));
    V0_DBD_HCRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT3.u32) + u32Data * VID_OFFSET));
    V0_DBD_HCRDX8BINLUT0.bits.dbd_hc_rdx8bin_lut0 = hc_rdx8bin_lut[0] ;
    V0_DBD_HCRDX8BINLUT0.bits.dbd_hc_rdx8bin_lut1 = hc_rdx8bin_lut[1] ;
    V0_DBD_HCRDX8BINLUT1.bits.dbd_hc_rdx8bin_lut2 = hc_rdx8bin_lut[2] ;
    V0_DBD_HCRDX8BINLUT1.bits.dbd_hc_rdx8bin_lut3 = hc_rdx8bin_lut[3] ;
    V0_DBD_HCRDX8BINLUT2.bits.dbd_hc_rdx8bin_lut4 = hc_rdx8bin_lut[4] ;
    V0_DBD_HCRDX8BINLUT2.bits.dbd_hc_rdx8bin_lut5 = hc_rdx8bin_lut[5] ;
    V0_DBD_HCRDX8BINLUT3.bits.dbd_hc_rdx8bin_lut6 = hc_rdx8bin_lut[6] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT0.u32) + u32Data * VID_OFFSET), V0_DBD_HCRDX8BINLUT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT1.u32) + u32Data * VID_OFFSET), V0_DBD_HCRDX8BINLUT1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT2.u32) + u32Data * VID_OFFSET), V0_DBD_HCRDX8BINLUT2.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT3.u32) + u32Data * VID_OFFSET), V0_DBD_HCRDX8BINLUT3.u32);
    return;
}
HI_VOID VDP_VID_SetDBDVerLumaRdx8BinLut(HI_U32 u32Data,HI_U32 vy_rdx8bin_lut[7])
{
    U_V0_DBD_VYRDX8BINLUT0 V0_DBD_VYRDX8BINLUT0;
    U_V0_DBD_VYRDX8BINLUT1 V0_DBD_VYRDX8BINLUT1;
    U_V0_DBD_VYRDX8BINLUT2 V0_DBD_VYRDX8BINLUT2;
    U_V0_DBD_VYRDX8BINLUT3 V0_DBD_VYRDX8BINLUT3;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDVerLumaRdx8BinLut() select wrong video layer ID\n");
      return ;
    }
   
    V0_DBD_VYRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT0.u32) + u32Data * VID_OFFSET));
    V0_DBD_VYRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT1.u32) + u32Data * VID_OFFSET));
    V0_DBD_VYRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT2.u32) + u32Data * VID_OFFSET));
    V0_DBD_VYRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT3.u32) + u32Data * VID_OFFSET));
    V0_DBD_VYRDX8BINLUT0.bits.dbd_vy_rdx8bin_lut0 = vy_rdx8bin_lut[0] ;
    V0_DBD_VYRDX8BINLUT0.bits.dbd_vy_rdx8bin_lut1 = vy_rdx8bin_lut[1] ;
    V0_DBD_VYRDX8BINLUT1.bits.dbd_vy_rdx8bin_lut2 = vy_rdx8bin_lut[2] ;
    V0_DBD_VYRDX8BINLUT1.bits.dbd_vy_rdx8bin_lut3 = vy_rdx8bin_lut[3] ;
    V0_DBD_VYRDX8BINLUT2.bits.dbd_vy_rdx8bin_lut4 = vy_rdx8bin_lut[4] ;
    V0_DBD_VYRDX8BINLUT2.bits.dbd_vy_rdx8bin_lut5 = vy_rdx8bin_lut[5] ;
    V0_DBD_VYRDX8BINLUT3.bits.dbd_vy_rdx8bin_lut6 = vy_rdx8bin_lut[6] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT0.u32) + u32Data * VID_OFFSET), V0_DBD_VYRDX8BINLUT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT1.u32) + u32Data * VID_OFFSET), V0_DBD_VYRDX8BINLUT1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT2.u32) + u32Data * VID_OFFSET), V0_DBD_VYRDX8BINLUT2.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT3.u32) + u32Data * VID_OFFSET), V0_DBD_VYRDX8BINLUT3.u32);
    return;
}
HI_VOID VDP_VID_SetDBDMinBlkSize(HI_U32 u32Data,HI_U32 min_blk_size)
{
    U_V0_DBD_BLKSIZE V0_DBD_BLKSIZE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDMinBlkSize() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_BLKSIZE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_BLKSIZE.u32) + u32Data * VID_OFFSET));
    V0_DBD_BLKSIZE.bits.dbd_min_blk_size = min_blk_size ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_BLKSIZE.u32) + u32Data * VID_OFFSET), V0_DBD_BLKSIZE.u32);
    return;
}
HI_VOID VDP_VID_SetDBDMaxBlkSize(HI_U32 u32Data,HI_U32 hy_max_blk_size,HI_U32 hc_max_blk_size,HI_U32 vy_max_blk_size)
{
    U_V0_DBD_BLKSIZE V0_DBD_BLKSIZE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBDMaxBlkSize() select wrong video layer ID\n");
      return ;
    }
    V0_DBD_BLKSIZE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_BLKSIZE.u32) + u32Data * VID_OFFSET));
    V0_DBD_BLKSIZE.bits.dbd_hy_max_blk_size = hy_max_blk_size ;
    V0_DBD_BLKSIZE.bits.dbd_hc_max_blk_size = hc_max_blk_size ;
    V0_DBD_BLKSIZE.bits.dbd_vy_max_blk_size = vy_max_blk_size ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBD_BLKSIZE.u32) + u32Data * VID_OFFSET), V0_DBD_BLKSIZE.u32);
    return;
}

HI_VOID VDP_VID_SetDmOppAngCtrstDiv(HI_U32 u32Data, HI_U32 dm_opp_ang_ctrst_div)
{
	U_V0_DM_DIR V0_DM_DIR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmDmOppAngCtrstDiv Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIR.bits.dm_opp_ang_ctrst_div = dm_opp_ang_ctrst_div;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET),V0_DM_DIR.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmOppAngCtrstT(HI_U32 u32Data, HI_U32 dm_opp_ang_ctrst_t)
{
	U_V0_DM_DIR V0_DM_DIR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDbmDmOppAngCtrstT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIR.bits.dm_opp_ang_ctrst_t = dm_opp_ang_ctrst_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET),V0_DM_DIR.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmCtrstThresh(HI_U32 u32Data, HI_U32 dm_ctrst_thresh)
{
	U_V0_DM_DIR V0_DM_DIR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmCtrstThresh Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIR.bits.dm_ctrst_thresh = dm_ctrst_thresh;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET),V0_DM_DIR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmGradSubRatio(HI_U32 u32Data, HI_U32 dm_grad_sub_ratio)
{
	U_V0_DM_DIR V0_DM_DIR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmGradSubRatio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIR.bits.dm_grad_sub_ratio = dm_grad_sub_ratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIR.u32) + u32Data * VID_OFFSET),V0_DM_DIR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmStrIdxFlag(HI_U32 u32Data, HI_U32 dm_str_idx_flag)
{
	U_V0_DM_EDGE V0_DM_EDGE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmStrIdxFlag Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
	V0_DM_EDGE.bits.dm_str_idx_flag = dm_str_idx_flag;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET),V0_DM_EDGE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmGlobalStr(HI_U32 u32Data, HI_U32 dm_global_str)
{
	U_V0_DM_EDGE V0_DM_EDGE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmGlobalStr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
	V0_DM_EDGE.bits.dm_global_str = dm_global_str;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET),V0_DM_EDGE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmSwThrGain(HI_U32 u32Data, HI_U32 dm_sw_thr_gain)
{
	U_V0_DM_EDGE V0_DM_EDGE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmSwThrGain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
	V0_DM_EDGE.bits.dm_sw_thr_gain = dm_sw_thr_gain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET),V0_DM_EDGE.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmEdgeGain(HI_U32 u32Data,HI_U32 dm_edge_gain1,HI_U32 dm_edge_gain2 )
{
    U_V0_DM_EDGE V0_DM_EDGE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDmEdgeGain() select wrong video layer ID\n");
      return ;
    }
    V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
    V0_DM_EDGE.bits.dm_edge_gain1 = dm_edge_gain1 ;
    V0_DM_EDGE.bits.dm_edge_gain2 = dm_edge_gain2 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET), V0_DM_EDGE.u32);
    return;
}

HI_VOID VDP_VID_SetDmAsymtrcGain(HI_U32 u32Data, HI_U32 dm_asymtrc_gain)
{
	U_V0_DM_EDGE V0_DM_EDGE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmAsymtrcGain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
	V0_DM_EDGE.bits.dm_asymtrc_gain = dm_asymtrc_gain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET),V0_DM_EDGE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmEdgeThr(HI_U32 u32Data, HI_U32 dm_edge_thr)
{
	U_V0_DM_EDGE V0_DM_EDGE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmEdgeThr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_EDGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET));
	V0_DM_EDGE.bits.dm_edge_thr = dm_edge_thr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_EDGE.u32) + u32Data * VID_OFFSET),V0_DM_EDGE.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmSwWhtLut(HI_U32 u32Data,HI_U32 dm_sw_wht_lut[5])
{
    U_V0_DM_LUT0 V0_DM_LUT0;
    U_V0_DM_LUT1 V0_DM_LUT1;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDmSwWhtLut() select wrong video layer ID\n");
      return ;
    }
    V0_DM_LUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT0.u32) + u32Data * VID_OFFSET));
    V0_DM_LUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET));
    V0_DM_LUT0.bits.dm_sw_wht_lut_p0  = dm_sw_wht_lut[0] ;
    V0_DM_LUT0.bits.dm_sw_wht_lut_p1  = dm_sw_wht_lut[1] ;
    V0_DM_LUT0.bits.dm_sw_wht_lut_p2  = dm_sw_wht_lut[2] ;
    V0_DM_LUT0.bits.dm_sw_wht_lut_p3  = dm_sw_wht_lut[3] ;
    V0_DM_LUT1.bits.dm_sw_wht_lut_p4  = dm_sw_wht_lut[4] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT0.u32) + u32Data * VID_OFFSET), V0_DM_LUT0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET), V0_DM_LUT1.u32);
    return;
}

HI_VOID VDP_VID_SetDmLimitT10(HI_U32 u32Data, HI_U32 dm_limit_t_10)
{
	U_V0_DM_LUT1 V0_DM_LUT1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimitT10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET));
	V0_DM_LUT1.bits.dm_limit_t_10 = dm_limit_t_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET),V0_DM_LUT1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLimitT(HI_U32 u32Data, HI_U32 dm_limit_t)
{
	U_V0_DM_LUT1 V0_DM_LUT1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimitT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET));
	V0_DM_LUT1.bits.dm_limit_t = dm_limit_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT1.u32) + u32Data * VID_OFFSET),V0_DM_LUT1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmDirStrGainLut(HI_U32 u32Data,HI_U32 dm_dir_str_gain_lut[8])
{
    U_V0_DM_LUT2 V0_DM_LUT2;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDmDirStrGainLut() select wrong video layer ID\n");
      return ;
    }
    V0_DM_LUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT2.u32) + u32Data * VID_OFFSET));
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p0  = dm_dir_str_gain_lut[0] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p1  = dm_dir_str_gain_lut[1] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p2  = dm_dir_str_gain_lut[2] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p3  = dm_dir_str_gain_lut[3] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p4  = dm_dir_str_gain_lut[4] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p5  = dm_dir_str_gain_lut[5] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p6  = dm_dir_str_gain_lut[6] ;
    V0_DM_LUT2.bits.dm_dir_str_gain_lut_p7  = dm_dir_str_gain_lut[7] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT2.u32) + u32Data * VID_OFFSET), V0_DM_LUT2.u32);
    return;
}

HI_VOID VDP_VID_SetDmDirStrLut(HI_U32 u32Data,HI_U32 dm_dir_str_lut[16])
{
    U_V0_DM_LUT3 V0_DM_LUT3;
    U_V0_DM_LUT4 V0_DM_LUT4;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDmDirStrLut() select wrong video layer ID\n");
      return ;
    }
    V0_DM_LUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT3.u32) + u32Data * VID_OFFSET));
    V0_DM_LUT4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LUT4.u32) + u32Data * VID_OFFSET));
    V0_DM_LUT3.bits.dm_dir_str_lut_p0   = dm_dir_str_lut[0 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p1   = dm_dir_str_lut[1 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p2   = dm_dir_str_lut[2 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p3   = dm_dir_str_lut[3 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p4   = dm_dir_str_lut[4 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p5   = dm_dir_str_lut[5 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p6   = dm_dir_str_lut[6 ] ;
    V0_DM_LUT3.bits.dm_dir_str_lut_p7   = dm_dir_str_lut[7 ] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p8   = dm_dir_str_lut[8 ] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p9   = dm_dir_str_lut[9 ] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p10  = dm_dir_str_lut[10] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p11  = dm_dir_str_lut[11] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p12  = dm_dir_str_lut[12] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p13  = dm_dir_str_lut[13] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p14  = dm_dir_str_lut[14] ;
    V0_DM_LUT4.bits.dm_dir_str_lut_p15  = dm_dir_str_lut[15] ;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT3.u32) + u32Data * VID_OFFSET), V0_DM_LUT3.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LUT4.u32) + u32Data * VID_OFFSET), V0_DM_LUT4.u32);
    return;
}

HI_VOID VDP_VID_SetDmMmfLimitEn(HI_U32 u32Data, HI_U32 dm_mmf_limit_en)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMmfLimitEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_mmf_limit_en = dm_mmf_limit_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirMmfEn(HI_U32 u32Data, HI_U32 dm_dir_mmf_en)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirMmfEn Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_dir_mmf_en = dm_dir_mmf_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmSimDirPtCountT(HI_U32 u32Data, HI_U32 dm_sim_dir_pt_count_t)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmSimDirPtCountT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_sim_dir_pt_count_t = dm_sim_dir_pt_count_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMndirOppCtrstT(HI_U32 u32Data, HI_U32 dm_mndir_opp_ctrst_t)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMndirOppCtrstT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_mndir_opp_ctrst_t = dm_mndir_opp_ctrst_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirCtrstT10(HI_U32 u32Data, HI_U32 dm_dir_ctrst_t_10)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirCtrstT10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_dir_ctrst_t_10 = dm_dir_ctrst_t_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirCtrstT(HI_U32 u32Data, HI_U32 dm_dir_ctrst_t)
{
	U_V0_DM_DIRC V0_DM_DIRC;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirCtrstT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRC.bits.dm_dir_ctrst_t = dm_dir_ctrst_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRC.u32) + u32Data * VID_OFFSET),V0_DM_DIRC.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmCswTrsntSt10(HI_U32 u32Data, HI_U32 dm_csw_trsnt_st_10)
{
	U_V0_DM_DIRO V0_DM_DIRO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmCswTrsntSt10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRO.bits.dm_csw_trsnt_st_10 = dm_csw_trsnt_st_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET),V0_DM_DIRO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirStrenShiftFac(HI_U32 u32Data, HI_U32 dm_dir_stren_shift_fac)
{
	U_V0_DM_DIRO V0_DM_DIRO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirStrenShiftFac Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRO.bits.dm_dir_stren_shift_fac = dm_dir_stren_shift_fac;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET),V0_DM_DIRO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirOppCtrstT10(HI_U32 u32Data, HI_U32 dm_dir_opp_ctrst_t_10)
{
	U_V0_DM_DIRO V0_DM_DIRO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirOppCtrstT10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRO.bits.dm_dir_opp_ctrst_t_10 = dm_dir_opp_ctrst_t_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET),V0_DM_DIRO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirOppCtrstT(HI_U32 u32Data, HI_U32 dm_dir_opp_ctrst_t)
{
	U_V0_DM_DIRO V0_DM_DIRO;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirOppCtrstT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRO.bits.dm_dir_opp_ctrst_t = dm_dir_opp_ctrst_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRO.u32) + u32Data * VID_OFFSET),V0_DM_DIRO.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLswRatio(HI_U32 u32Data, HI_U32 dm_lsw_ratio)
{
	U_V0_DM_LSW V0_DM_LSW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLswRatio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LSW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET));
	V0_DM_LSW.bits.dm_lsw_ratio = dm_lsw_ratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET),V0_DM_LSW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmSdbwMode(HI_U32 u32Data, HI_U32 dm_sdbw_mode)
{
	U_V0_DM_LSW V0_DM_LSW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmSdbwMode Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LSW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET));
	V0_DM_LSW.bits.dm_sdbw_mode = dm_sdbw_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET),V0_DM_LSW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmCswTrsntSt(HI_U32 u32Data, HI_U32 dm_csw_trsnt_st)
{
	U_V0_DM_LSW V0_DM_LSW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmCswTrsntSt Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LSW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET));
	V0_DM_LSW.bits.dm_csw_trsnt_st = dm_csw_trsnt_st;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET),V0_DM_LSW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLwCtrstT10(HI_U32 u32Data, HI_U32 dm_lw_ctrst_t_10)
{
	U_V0_DM_LSW V0_DM_LSW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLwCtrstT10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LSW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET));
	V0_DM_LSW.bits.dm_lw_ctrst_t_10 = dm_lw_ctrst_t_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET),V0_DM_LSW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLwCtrstT(HI_U32 u32Data, HI_U32 dm_lw_ctrst_t)
{
	U_V0_DM_LSW V0_DM_LSW;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLwCtrstT Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_LSW.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET));
	V0_DM_LSW.bits.dm_lw_ctrst_t = dm_lw_ctrst_t;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_LSW.u32) + u32Data * VID_OFFSET),V0_DM_LSW.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmCswTrsntLt(HI_U32 u32Data, HI_U32 dm_csw_trsnt_lt)
{
	U_V0_DM_MMFLR V0_DM_MMFLR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmCswTrsntLt Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFLR.bits.dm_csw_trsnt_lt = dm_csw_trsnt_lt;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET),V0_DM_MMFLR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMmfLr10(HI_U32 u32Data, HI_U32 dm_mmf_lr_10)
{
	U_V0_DM_MMFLR V0_DM_MMFLR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMmfLr10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFLR.bits.dm_mmf_lr_10 = dm_mmf_lr_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET),V0_DM_MMFLR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMmfLr(HI_U32 u32Data, HI_U32 dm_mmf_lr)
{
	U_V0_DM_MMFLR V0_DM_MMFLR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMmfLr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFLR.bits.dm_mmf_lr = dm_mmf_lr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFLR.u32) + u32Data * VID_OFFSET),V0_DM_MMFLR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmCswTrsntLt10(HI_U32 u32Data, HI_U32 dm_csw_trsnt_lt_10)
{
	U_V0_DM_MMFSR V0_DM_MMFSR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmCswTrsntLt10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFSR.bits.dm_csw_trsnt_lt_10 = dm_csw_trsnt_lt_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET),V0_DM_MMFSR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMmfSr10(HI_U32 u32Data, HI_U32 dm_mmf_sr_10)
{
	U_V0_DM_MMFSR V0_DM_MMFSR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMmfSr10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFSR.bits.dm_mmf_sr_10 = dm_mmf_sr_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET),V0_DM_MMFSR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMmfSr(HI_U32 u32Data, HI_U32 dm_mmf_sr)
{
	U_V0_DM_MMFSR V0_DM_MMFSR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMmfSr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_MMFSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET));
	V0_DM_MMFSR.bits.dm_mmf_sr = dm_mmf_sr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_MMFSR.u32) + u32Data * VID_OFFSET),V0_DM_MMFSR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirMmfLr10(HI_U32 u32Data, HI_U32 dm_dir_mmf_lr_10)
{
	U_V0_DM_DIRLR V0_DM_DIRLR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirMmfLr10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRLR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRLR.bits.dm_dir_mmf_lr_10 = dm_dir_mmf_lr_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRLR.u32) + u32Data * VID_OFFSET),V0_DM_DIRLR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirMmfLr(HI_U32 u32Data, HI_U32 dm_dir_mmf_lr)
{
	U_V0_DM_DIRLR V0_DM_DIRLR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirMmfLr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRLR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRLR.bits.dm_dir_mmf_lr = dm_dir_mmf_lr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRLR.u32) + u32Data * VID_OFFSET),V0_DM_DIRLR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirMmfSr10(HI_U32 u32Data, HI_U32 dm_dir_mmf_sr_10)
{
	U_V0_DM_DIRSR V0_DM_DIRSR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirMmfSr10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRSR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRSR.bits.dm_dir_mmf_sr_10 = dm_dir_mmf_sr_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRSR.u32) + u32Data * VID_OFFSET),V0_DM_DIRSR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmDirMmfSr(HI_U32 u32Data, HI_U32 dm_dir_mmf_sr)
{
	U_V0_DM_DIRSR V0_DM_DIRSR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirMmfSr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_DIRSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_DIRSR.u32) + u32Data * VID_OFFSET));
	V0_DM_DIRSR.bits.dm_dir_mmf_sr = dm_dir_mmf_sr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_DIRSR.u32) + u32Data * VID_OFFSET),V0_DM_DIRSR.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmDirBlendStr(HI_U32 u32Data, HI_U32 dm_dir_blend_str)
{
	U_V0_DM_THR0 V0_DM_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmDirBlendStr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET));
	V0_DM_THR0.bits.dm_dir_blend_str = dm_dir_blend_str;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET),V0_DM_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLimResBlendStr2(HI_U32 u32Data, HI_U32 dm_lim_res_blend_str2)
{
	U_V0_DM_THR0 V0_DM_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimResBlendStr2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET));
	V0_DM_THR0.bits.dm_lim_res_blend_str2 = dm_lim_res_blend_str2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET),V0_DM_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLimResBlendStr1(HI_U32 u32Data, HI_U32 dm_lim_res_blend_str1)
{
	U_V0_DM_THR0 V0_DM_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimResBlendStr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET));
	V0_DM_THR0.bits.dm_lim_res_blend_str1 = dm_lim_res_blend_str1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET),V0_DM_THR0.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmLimitLswRatio(HI_U32 u32Data, HI_U32 dm_limit_lsw_ratio)
{
	U_V0_DM_THR0 V0_DM_THR0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimitLswRatio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET));
	V0_DM_THR0.bits.dm_limit_lsw_ratio = dm_limit_lsw_ratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR0.u32) + u32Data * VID_OFFSET),V0_DM_THR0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmLimTransWid(HI_U32 u32Data, HI_U32 dm_lim_trans_wid)
{
	U_V0_DM_THR1 V0_DM_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimTransWid Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET));
	V0_DM_THR1.bits.dm_lim_trans_wid = dm_lim_trans_wid;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET),V0_DM_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMinLimValTrans(HI_U32 u32Data, HI_U32 dm_min_lim_val_trans)
{
	U_V0_DM_THR1 V0_DM_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMinLimValTrans Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET));
	V0_DM_THR1.bits.dm_min_lim_val_trans = dm_min_lim_val_trans;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET),V0_DM_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMinLimTransWid(HI_U32 u32Data, HI_U32 dm_min_lim_trans_wid)
{
	U_V0_DM_THR1 V0_DM_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMinLimTransWid Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET));
	V0_DM_THR1.bits.dm_min_lim_trans_wid = dm_min_lim_trans_wid;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR1.u32) + u32Data * VID_OFFSET),V0_DM_THR1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetDmLimTransWid10(HI_U32 u32Data, HI_U32 dm_lim_trans_wid_10)
{
	U_V0_DM_THR2 V0_DM_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmLimTransWid10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET));
	V0_DM_THR2.bits.dm_lim_trans_wid_10 = dm_lim_trans_wid_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET),V0_DM_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMinLimValTrans10(HI_U32 u32Data, HI_U32 dm_min_lim_val_trans_10)
{
	U_V0_DM_THR2 V0_DM_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMinLimValTrans10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET));
	V0_DM_THR2.bits.dm_min_lim_val_trans_10 = dm_min_lim_val_trans_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET),V0_DM_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDmMinLimTransWid10(HI_U32 u32Data, HI_U32 dm_min_lim_trans_wid_10)
{
	U_V0_DM_THR2 V0_DM_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDmMinLimTransWid10 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DM_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET));
	V0_DM_THR2.bits.dm_min_lim_trans_wid_10 = dm_min_lim_trans_wid_10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DM_THR2.u32) + u32Data * VID_OFFSET),V0_DM_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsFlatthrscl(HI_U32 u32Data, HI_U32 ds_flatthrscl)
{
	U_V0_DS_CTRL V0_DS_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsFlatthrscl Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DS_CTRL.bits.ds_flatthrscl = ds_flatthrscl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET),V0_DS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsCordirthr(HI_U32 u32Data, HI_U32 ds_cordirthr)
{
	U_V0_DS_CTRL V0_DS_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsCordirthr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DS_CTRL.bits.ds_cordirthr = ds_cordirthr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET),V0_DS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsBlendgain(HI_U32 u32Data, HI_U32 ds_blendgain)
{
	U_V0_DS_CTRL V0_DS_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsBlendgain Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DS_CTRL.bits.ds_blendgain = ds_blendgain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_CTRL.u32) + u32Data * VID_OFFSET),V0_DS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond3thr3(HI_U32 u32Data, HI_U32 ds_patcond3thr3)
{
	U_V0_DS_THR1 V0_DS_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond3thr3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET));
	V0_DS_THR1.bits.ds_patcond3thr3 = ds_patcond3thr3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET),V0_DS_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond5thr5(HI_U32 u32Data, HI_U32 ds_patcond5thr5)
{
	U_V0_DS_THR1 V0_DS_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond5thr5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET));
	V0_DS_THR1.bits.ds_patcond5thr5 = ds_patcond5thr5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET),V0_DS_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl61(HI_U32 u32Data, HI_U32 ds_patcond6scl61)
{
	U_V0_DS_THR1 V0_DS_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl61 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET));
	V0_DS_THR1.bits.ds_patcond6scl61 = ds_patcond6scl61;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET),V0_DS_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl62(HI_U32 u32Data, HI_U32 ds_patcond6scl62)
{
	U_V0_DS_THR1 V0_DS_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl62 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET));
	V0_DS_THR1.bits.ds_patcond6scl62 = ds_patcond6scl62;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR1.u32) + u32Data * VID_OFFSET),V0_DS_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl63(HI_U32 u32Data, HI_U32 ds_patcond6scl63)
{
	U_V0_DS_THR2 V0_DS_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl63 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET));
	V0_DS_THR2.bits.ds_patcond6scl63 = ds_patcond6scl63;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET),V0_DS_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl64(HI_U32 u32Data, HI_U32 ds_patcond6scl64)
{
	U_V0_DS_THR2 V0_DS_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl64 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET));
	V0_DS_THR2.bits.ds_patcond6scl64 = ds_patcond6scl64;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET),V0_DS_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl65(HI_U32 u32Data, HI_U32 ds_patcond6scl65)
{
	U_V0_DS_THR2 V0_DS_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl65 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET));
	V0_DS_THR2.bits.ds_patcond6scl65 = ds_patcond6scl65;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET),V0_DS_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl66(HI_U32 u32Data, HI_U32 ds_patcond6scl66)
{
	U_V0_DS_THR2 V0_DS_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl66 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET));
	V0_DS_THR2.bits.ds_patcond6scl66 = ds_patcond6scl66;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR2.u32) + u32Data * VID_OFFSET),V0_DS_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond1thr1(HI_U32 u32Data, HI_U32 ds_wincond1thr1)
{
	U_V0_DS_THR3 V0_DS_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond1thr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET));
	V0_DS_THR3.bits.ds_wincond1thr1 = ds_wincond1thr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET),V0_DS_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond1val1(HI_U32 u32Data, HI_U32 ds_wincond1val1)
{
	U_V0_DS_THR3 V0_DS_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond1val1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET));
	V0_DS_THR3.bits.ds_wincond1val1 = ds_wincond1val1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET),V0_DS_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond2thr1(HI_U32 u32Data, HI_U32 ds_wincond2thr1)
{
	U_V0_DS_THR3 V0_DS_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond2thr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET));
	V0_DS_THR3.bits.ds_wincond2thr1 = ds_wincond2thr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR3.u32) + u32Data * VID_OFFSET),V0_DS_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond2val1(HI_U32 u32Data, HI_U32 ds_wincond2val1)
{
	U_V0_DS_THR4 V0_DS_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond2val1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET));
	V0_DS_THR4.bits.ds_wincond2val1 = ds_wincond2val1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET),V0_DS_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond2thr2(HI_U32 u32Data, HI_U32 ds_wincond2thr2)
{
	U_V0_DS_THR4 V0_DS_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond2thr2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET));
	V0_DS_THR4.bits.ds_wincond2thr2 = ds_wincond2thr2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET),V0_DS_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond2val2(HI_U32 u32Data, HI_U32 ds_wincond2val2)
{
	U_V0_DS_THR4 V0_DS_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond2val2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET));
	V0_DS_THR4.bits.ds_wincond2val2 = ds_wincond2val2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR4.u32) + u32Data * VID_OFFSET),V0_DS_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3thr1(HI_U32 u32Data, HI_U32 ds_wincond3thr1)
{
	U_V0_DS_THR5 V0_DS_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3thr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET));
	V0_DS_THR5.bits.ds_wincond3thr1 = ds_wincond3thr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET),V0_DS_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3val1(HI_U32 u32Data, HI_U32 ds_wincond3val1)
{
	U_V0_DS_THR5 V0_DS_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3val1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET));
	V0_DS_THR5.bits.ds_wincond3val1 = ds_wincond3val1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET),V0_DS_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3val2(HI_U32 u32Data, HI_U32 ds_wincond3val2)
{
	U_V0_DS_THR5 V0_DS_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3val2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET));
	V0_DS_THR5.bits.ds_wincond3val2 = ds_wincond3val2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR5.u32) + u32Data * VID_OFFSET),V0_DS_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3thr2(HI_U32 u32Data, HI_U32 ds_wincond3thr2)
{
	U_V0_DS_THR6 V0_DS_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3thr2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET));
	V0_DS_THR6.bits.ds_wincond3thr2 = ds_wincond3thr2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET),V0_DS_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3val3(HI_U32 u32Data, HI_U32 ds_wincond3val3)
{
	U_V0_DS_THR6 V0_DS_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3val3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET));
	V0_DS_THR6.bits.ds_wincond3val3 = ds_wincond3val3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET),V0_DS_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3val4(HI_U32 u32Data, HI_U32 ds_wincond3val4)
{
	U_V0_DS_THR6 V0_DS_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3val4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET));
	V0_DS_THR6.bits.ds_wincond3val4 = ds_wincond3val4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR6.u32) + u32Data * VID_OFFSET),V0_DS_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond3scl1(HI_U32 u32Data, HI_U32 ds_wincond3scl1)
{
	U_V0_DS_THR7 V0_DS_THR7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond3scl1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET));
	V0_DS_THR7.bits.ds_wincond3scl1 = ds_wincond3scl1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET),V0_DS_THR7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4thr1(HI_U32 u32Data, HI_U32 ds_wincond4thr1)
{
	U_V0_DS_THR7 V0_DS_THR7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4thr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET));
	V0_DS_THR7.bits.ds_wincond4thr1 = ds_wincond4thr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET),V0_DS_THR7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4thr2(HI_U32 u32Data, HI_U32 ds_wincond4thr2)
{
	U_V0_DS_THR7 V0_DS_THR7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4thr2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET));
	V0_DS_THR7.bits.ds_wincond4thr2 = ds_wincond4thr2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR7.u32) + u32Data * VID_OFFSET),V0_DS_THR7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4val1(HI_U32 u32Data, HI_U32 ds_wincond4val1)
{
	U_V0_DS_THR8 V0_DS_THR8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4val1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET));
	V0_DS_THR8.bits.ds_wincond4val1 = ds_wincond4val1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET),V0_DS_THR8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4val2(HI_U32 u32Data, HI_U32 ds_wincond4val2)
{
	U_V0_DS_THR8 V0_DS_THR8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4val2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET));
	V0_DS_THR8.bits.ds_wincond4val2 = ds_wincond4val2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET),V0_DS_THR8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4val3(HI_U32 u32Data, HI_U32 ds_wincond4val3)
{
	U_V0_DS_THR8 V0_DS_THR8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4val3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET));
	V0_DS_THR8.bits.ds_wincond4val3 = ds_wincond4val3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR8.u32) + u32Data * VID_OFFSET),V0_DS_THR8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4val4(HI_U32 u32Data, HI_U32 ds_wincond4val4)
{
	U_V0_DS_THR9 V0_DS_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4val4 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET));
	V0_DS_THR9.bits.ds_wincond4val4 = ds_wincond4val4;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET),V0_DS_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4val5(HI_U32 u32Data, HI_U32 ds_wincond4val5)
{
	U_V0_DS_THR9 V0_DS_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4val5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET));
	V0_DS_THR9.bits.ds_wincond4val5 = ds_wincond4val5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET),V0_DS_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond4scl1(HI_U32 u32Data, HI_U32 ds_wincond4scl1)
{
	U_V0_DS_THR9 V0_DS_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond4scl1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET));
	V0_DS_THR9.bits.ds_wincond4scl1 = ds_wincond4scl1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR9.u32) + u32Data * VID_OFFSET),V0_DS_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5thr1(HI_U32 u32Data, HI_U32 ds_wincond5thr1)
{
	U_V0_DS_THR10 V0_DS_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5thr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET));
	V0_DS_THR10.bits.ds_wincond5thr1 = ds_wincond5thr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET),V0_DS_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5val1(HI_U32 u32Data, HI_U32 ds_wincond5val1)
{
	U_V0_DS_THR10 V0_DS_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5val1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET));
	V0_DS_THR10.bits.ds_wincond5val1 = ds_wincond5val1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET),V0_DS_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5val2(HI_U32 u32Data, HI_U32 ds_wincond5val2)
{
	U_V0_DS_THR10 V0_DS_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5val2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET));
	V0_DS_THR10.bits.ds_wincond5val2 = ds_wincond5val2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR10.u32) + u32Data * VID_OFFSET),V0_DS_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5val3(HI_U32 u32Data, HI_U32 ds_wincond5val3)
{
	U_V0_DS_THR11 V0_DS_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5val3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET));
	V0_DS_THR11.bits.ds_wincond5val3 = ds_wincond5val3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET),V0_DS_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5scl1(HI_U32 u32Data, HI_U32 ds_wincond5scl1)
{
	U_V0_DS_THR11 V0_DS_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5scl1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET));
	V0_DS_THR11.bits.ds_wincond5scl1 = ds_wincond5scl1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET),V0_DS_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWincond5scl2(HI_U32 u32Data, HI_U32 ds_wincond5scl2)
{
	U_V0_DS_THR11 V0_DS_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWincond5scl2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET));
	V0_DS_THR11.bits.ds_wincond5scl2 = ds_wincond5scl2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR11.u32) + u32Data * VID_OFFSET),V0_DS_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPicresscl0(HI_U32 u32Data, HI_U32 ds_picresscl0)
{
	U_V0_DS_THR12 V0_DS_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPicresscl0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET));
	V0_DS_THR12.bits.ds_picresscl0 = ds_picresscl0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET),V0_DS_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPicresscl1(HI_U32 u32Data, HI_U32 ds_picresscl1)
{
	U_V0_DS_THR12 V0_DS_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPicresscl1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET));
	V0_DS_THR12.bits.ds_picresscl1 = ds_picresscl1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET),V0_DS_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPicresscl2(HI_U32 u32Data, HI_U32 ds_picresscl2)
{
	U_V0_DS_THR12 V0_DS_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPicresscl2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET));
	V0_DS_THR12.bits.ds_picresscl2 = ds_picresscl2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR12.u32) + u32Data * VID_OFFSET),V0_DS_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsWinactthr(HI_U32 u32Data, HI_U32 ds_winactthr)
{
	U_V0_DS_THR13 V0_DS_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsWinactthr Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR13.u32) + u32Data * VID_OFFSET));
	V0_DS_THR13.bits.ds_winactthr = ds_winactthr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR13.u32) + u32Data * VID_OFFSET),V0_DS_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPicresofst(HI_U32 u32Data, HI_U32 ds_picresofst)
{
	U_V0_DS_THR13 V0_DS_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPicresofst Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR13.u32) + u32Data * VID_OFFSET));
	V0_DS_THR13.bits.ds_picresofst = ds_picresofst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR13.u32) + u32Data * VID_OFFSET),V0_DS_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6norm61(HI_U32 u32Data, HI_U32 ds_patcond6norm61)
{
	U_V0_DS_THR14 V0_DS_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6norm61 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET));
	V0_DS_THR14.bits.ds_patcond6norm61 = ds_patcond6norm61;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET),V0_DS_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6norm62(HI_U32 u32Data, HI_U32 ds_patcond6norm62)
{
	U_V0_DS_THR14 V0_DS_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6norm62 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET));
	V0_DS_THR14.bits.ds_patcond6norm62 = ds_patcond6norm62;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET),V0_DS_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6norm63(HI_U32 u32Data, HI_U32 ds_patcond6norm63)
{
	U_V0_DS_THR14 V0_DS_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6norm63 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET));
	V0_DS_THR14.bits.ds_patcond6norm63 = ds_patcond6norm63;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET),V0_DS_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6normofst(HI_U32 u32Data, HI_U32 ds_patcond6normofst)
{
	U_V0_DS_THR14 V0_DS_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6normofst Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET));
	V0_DS_THR14.bits.ds_patcond6normofst = ds_patcond6normofst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR14.u32) + u32Data * VID_OFFSET),V0_DS_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6norm5(HI_U32 u32Data, HI_U32 ds_patcond6norm5)
{
	U_V0_DS_THR15 V0_DS_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6norm5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET));
	V0_DS_THR15.bits.ds_patcond6norm5 = ds_patcond6norm5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET),V0_DS_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6norm6(HI_U32 u32Data, HI_U32 ds_patcond6norm6)
{
	U_V0_DS_THR15 V0_DS_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6norm6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET));
	V0_DS_THR15.bits.ds_patcond6norm6 = ds_patcond6norm6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET),V0_DS_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6clipscl6(HI_U32 u32Data, HI_U32 ds_patcond6clipscl6)
{
	U_V0_DS_THR15 V0_DS_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6clipscl6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET));
	V0_DS_THR15.bits.ds_patcond6clipscl6 = ds_patcond6clipscl6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR15.u32) + u32Data * VID_OFFSET),V0_DS_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst(HI_U32 u32Data, HI_U32 ds_patcond6ofst)
{
	U_V0_DS_THR16 V0_DS_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET));
	V0_DS_THR16.bits.ds_patcond6ofst = ds_patcond6ofst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET),V0_DS_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsSclctrst(HI_U32 u32Data, HI_U32 ds_sclctrst)
{
	U_V0_DS_THR16 V0_DS_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsSclctrst Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET));
	V0_DS_THR16.bits.ds_sclctrst = ds_sclctrst;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET),V0_DS_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsResadpten(HI_U32 u32Data, HI_U32 ds_resadpten)
{
	U_V0_DS_THR16 V0_DS_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsResadpten Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET));
	V0_DS_THR16.bits.ds_resadpten = ds_resadpten;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR16.u32) + u32Data * VID_OFFSET),V0_DS_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsShtctrstscl1(HI_U32 u32Data, HI_U32 ds_shtctrstscl1)
{
	U_V0_DS_THR17 V0_DS_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsShtctrstscl1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR17.u32) + u32Data * VID_OFFSET));
	V0_DS_THR17.bits.ds_shtctrstscl1 = ds_shtctrstscl1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR17.u32) + u32Data * VID_OFFSET),V0_DS_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsShtctrstscl2(HI_U32 u32Data, HI_U32 ds_shtctrstscl2)
{
	U_V0_DS_THR17 V0_DS_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsShtctrstscl2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR17.u32) + u32Data * VID_OFFSET));
	V0_DS_THR17.bits.ds_shtctrstscl2 = ds_shtctrstscl2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR17.u32) + u32Data * VID_OFFSET),V0_DS_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsCormmthr0(HI_U32 u32Data, HI_U32 ds_cormmthr0)
{
	U_V0_DS_THR18 V0_DS_THR18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsCormmthr0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR18.u32) + u32Data * VID_OFFSET));
	V0_DS_THR18.bits.ds_cormmthr0 = ds_cormmthr0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR18.u32) + u32Data * VID_OFFSET),V0_DS_THR18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsCormmthr1(HI_U32 u32Data, HI_U32 ds_cormmthr1)
{
	U_V0_DS_THR18 V0_DS_THR18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsCormmthr1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR18.u32) + u32Data * VID_OFFSET));
	V0_DS_THR18.bits.ds_cormmthr1 = ds_cormmthr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR18.u32) + u32Data * VID_OFFSET),V0_DS_THR18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPixeldiff2p(HI_U32 u32Data, HI_U32 ds_pixeldiff_2p)
{
	U_V0_DS_READSTATE0 V0_DS_READSTATE0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPixeldiff2p Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_READSTATE0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE0.u32) + u32Data * VID_OFFSET));
	V0_DS_READSTATE0.bits.ds_pixeldiff_2p = ds_pixeldiff_2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE0.u32) + u32Data * VID_OFFSET),V0_DS_READSTATE0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsFlag2p(HI_U32 u32Data, HI_U32 ds_flag_2p)
{
	U_V0_DS_READSTATE0 V0_DS_READSTATE0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsFlag2p Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_READSTATE0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE0.u32) + u32Data * VID_OFFSET));
	V0_DS_READSTATE0.bits.ds_flag_2p = ds_flag_2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE0.u32) + u32Data * VID_OFFSET),V0_DS_READSTATE0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsStrctrast2p(HI_U32 u32Data, HI_U32 ds_strctrast_2p)
{
	U_V0_DS_READSTATE1 V0_DS_READSTATE1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsStrctrast2p Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_READSTATE1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE1.u32) + u32Data * VID_OFFSET));
	V0_DS_READSTATE1.bits.ds_strctrast_2p = ds_strctrast_2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_READSTATE1.u32) + u32Data * VID_OFFSET),V0_DS_READSTATE1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst5(HI_U32 u32Data, HI_U32 ds_patcond6ofst5)
{
	U_V0_DS_THR19 V0_DS_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst5 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET));
	V0_DS_THR19.bits.ds_patcond6ofst5 = ds_patcond6ofst5;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET),V0_DS_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst6(HI_U32 u32Data, HI_U32 ds_patcond6ofst6)
{
	U_V0_DS_THR19 V0_DS_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst6 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET));
	V0_DS_THR19.bits.ds_patcond6ofst6 = ds_patcond6ofst6;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET),V0_DS_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst61(HI_U32 u32Data, HI_U32 ds_patcond6ofst61)
{
	U_V0_DS_THR19 V0_DS_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst61 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET));
	V0_DS_THR19.bits.ds_patcond6ofst61 = ds_patcond6ofst61;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR19.u32) + u32Data * VID_OFFSET),V0_DS_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6scl7(HI_U32 u32Data, HI_U32 ds_patcond6scl7)
{
	U_V0_DS_THR20 V0_DS_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6scl7 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET));
	V0_DS_THR20.bits.ds_patcond6scl7 = ds_patcond6scl7;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET),V0_DS_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst63(HI_U32 u32Data, HI_U32 ds_patcond6ofst63)
{
	U_V0_DS_THR20 V0_DS_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst63 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET));
	V0_DS_THR20.bits.ds_patcond6ofst63 = ds_patcond6ofst63;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET),V0_DS_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsPatcond6ofst62(HI_U32 u32Data, HI_U32 ds_patcond6ofst62)
{
	U_V0_DS_THR20 V0_DS_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsPatcond6ofst62 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET));
	V0_DS_THR20.bits.ds_patcond6ofst62 = ds_patcond6ofst62;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET),V0_DS_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDsOvershootflag(HI_U32 u32Data, HI_U32 ds_overshootflag)
{
	U_V0_DS_THR20 V0_DS_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDsOvershootflag Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DS_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET));
	V0_DS_THR20.bits.ds_overshootflag = ds_overshootflag;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DS_THR20.u32) + u32Data * VID_OFFSET),V0_DS_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEnnosmoothwavepro(HI_U32 u32Data, HI_U32 dr_ennosmoothwavepro)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEnnosmoothwavepro Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_ennosmoothwavepro = dr_ennosmoothwavepro;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEn3x3filter(HI_U32 u32Data, HI_U32 dr_en3x3filter)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEn3x3filter Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_en3x3filter = dr_en3x3filter;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEnfarthestsmooth(HI_U32 u32Data, HI_U32 dr_enfarthestsmooth)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEnfarthestsmooth Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_enfarthestsmooth = dr_enfarthestsmooth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEnlimitpro(HI_U32 u32Data, HI_U32 dr_enlimitpro)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEnlimitpro Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_enlimitpro = dr_enlimitpro;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEnunderpro(HI_U32 u32Data, HI_U32 dr_enunderpro)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEnunderpro Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_enunderpro = dr_enunderpro;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEncondarkpro(HI_U32 u32Data, HI_U32 dr_encondarkpro)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEncondarkpro Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_encondarkpro = dr_encondarkpro;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEnconpro(HI_U32 u32Data, HI_U32 dr_enconpro)
{
	U_V0_DR_CTRL V0_DR_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEnconpro Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET));
	V0_DR_CTRL.bits.dr_enconpro = dr_enconpro;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_CTRL.u32) + u32Data * VID_OFFSET),V0_DR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrGradmaxthd(HI_U32 u32Data, HI_U32 dr_gradmaxthd)
{
	U_V0_DR_THR1 V0_DR_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrGradmaxthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET));
	V0_DR_THR1.bits.dr_gradmaxthd = dr_gradmaxthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET),V0_DR_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShootmaxthd(HI_U32 u32Data, HI_U32 dr_shootmaxthd)
{
	U_V0_DR_THR1 V0_DR_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShootmaxthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET));
	V0_DR_THR1.bits.dr_shootmaxthd = dr_shootmaxthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET),V0_DR_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrGradratio(HI_U32 u32Data, HI_U32 dr_gradratio)
{
	U_V0_DR_THR1 V0_DR_THR1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrGradratio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET));
	V0_DR_THR1.bits.dr_gradratio = dr_gradratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR1.u32) + u32Data * VID_OFFSET),V0_DR_THR1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrUnderratio(HI_U32 u32Data, HI_U32 dr_underratio)
{
	U_V0_DR_THR2 V0_DR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrUnderratio Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET));
	V0_DR_THR2.bits.dr_underratio = dr_underratio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET),V0_DR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSrhtrsngradalpha(HI_U32 u32Data, HI_U32 dr_srhtrsngradalpha)
{
	U_V0_DR_THR2 V0_DR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSrhtrsngradalpha Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET));
	V0_DR_THR2.bits.dr_srhtrsngradalpha = dr_srhtrsngradalpha;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET),V0_DR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSrhtrsngradbeta(HI_U32 u32Data, HI_U32 dr_srhtrsngradbeta)
{
	U_V0_DR_THR2 V0_DR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSrhtrsngradbeta Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET));
	V0_DR_THR2.bits.dr_srhtrsngradbeta = dr_srhtrsngradbeta;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET),V0_DR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrHighfrequent(HI_U32 u32Data, HI_U32 dr_highfrequent)
{
	U_V0_DR_THR2 V0_DR_THR2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrHighfrequent Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET));
	V0_DR_THR2.bits.dr_highfrequent = dr_highfrequent;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR2.u32) + u32Data * VID_OFFSET),V0_DR_THR2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingx0(HI_U32 u32Data, HI_U32 dr_ringx0)
{
	U_V0_DR_THR3 V0_DR_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingx0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET));
	V0_DR_THR3.bits.dr_ringx0 = dr_ringx0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET),V0_DR_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingx1(HI_U32 u32Data, HI_U32 dr_ringx1)
{
	U_V0_DR_THR3 V0_DR_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingx1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET));
	V0_DR_THR3.bits.dr_ringx1 = dr_ringx1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET),V0_DR_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingx2(HI_U32 u32Data, HI_U32 dr_ringx2)
{
	U_V0_DR_THR3 V0_DR_THR3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingx2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET));
	V0_DR_THR3.bits.dr_ringx2 = dr_ringx2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR3.u32) + u32Data * VID_OFFSET),V0_DR_THR3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingx3(HI_U32 u32Data, HI_U32 dr_ringx3)
{
	U_V0_DR_THR4 V0_DR_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingx3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET));
	V0_DR_THR4.bits.dr_ringx3 = dr_ringx3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET),V0_DR_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingy0(HI_U32 u32Data, HI_U32 dr_ringy0)
{
	U_V0_DR_THR4 V0_DR_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingy0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET));
	V0_DR_THR4.bits.dr_ringy0 = dr_ringy0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET),V0_DR_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingy1(HI_U32 u32Data, HI_U32 dr_ringy1)
{
	U_V0_DR_THR4 V0_DR_THR4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingy1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET));
	V0_DR_THR4.bits.dr_ringy1 = dr_ringy1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR4.u32) + u32Data * VID_OFFSET),V0_DR_THR4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingy2(HI_U32 u32Data, HI_U32 dr_ringy2)
{
	U_V0_DR_THR5 V0_DR_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingy2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET));
	V0_DR_THR5.bits.dr_ringy2 = dr_ringy2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET),V0_DR_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingy3(HI_U32 u32Data, HI_U32 dr_ringy3)
{
	U_V0_DR_THR5 V0_DR_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingy3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET));
	V0_DR_THR5.bits.dr_ringy3 = dr_ringy3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET),V0_DR_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShttornum0(HI_U32 u32Data, HI_U32 dr_shttornum0)
{
	U_V0_DR_THR5 V0_DR_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShttornum0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET));
	V0_DR_THR5.bits.dr_shttornum0 = dr_shttornum0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET),V0_DR_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShttornum1(HI_U32 u32Data, HI_U32 dr_shttornum1)
{
	U_V0_DR_THR5 V0_DR_THR5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShttornum1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET));
	V0_DR_THR5.bits.dr_shttornum1 = dr_shttornum1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR5.u32) + u32Data * VID_OFFSET),V0_DR_THR5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingk0(HI_U32 u32Data, HI_U32 dr_ringk0)
{
	U_V0_DR_THR6 V0_DR_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingk0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET));
	V0_DR_THR6.bits.dr_ringk0 = dr_ringk0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET),V0_DR_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShttornum2(HI_U32 u32Data, HI_U32 dr_shttornum2)
{
	U_V0_DR_THR6 V0_DR_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShttornum2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET));
	V0_DR_THR6.bits.dr_shttornum2 = dr_shttornum2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET),V0_DR_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShttornum3(HI_U32 u32Data, HI_U32 dr_shttornum3)
{
	U_V0_DR_THR6 V0_DR_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShttornum3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET));
	V0_DR_THR6.bits.dr_shttornum3 = dr_shttornum3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET),V0_DR_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRestornum(HI_U32 u32Data, HI_U32 dr_restornum)
{
	U_V0_DR_THR6 V0_DR_THR6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRestornum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET));
	V0_DR_THR6.bits.dr_restornum = dr_restornum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR6.u32) + u32Data * VID_OFFSET),V0_DR_THR6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingk1(HI_U32 u32Data, HI_U32 dr_ringk1)
{
	U_V0_DR_THR7 V0_DR_THR7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingk1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR7.u32) + u32Data * VID_OFFSET));
	V0_DR_THR7.bits.dr_ringk1 = dr_ringk1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR7.u32) + u32Data * VID_OFFSET),V0_DR_THR7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingmax(HI_U32 u32Data, HI_U32 dr_ringmax)
{
	U_V0_DR_THR7 V0_DR_THR7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingmax Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR7.u32) + u32Data * VID_OFFSET));
	V0_DR_THR7.bits.dr_ringmax = dr_ringmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR7.u32) + u32Data * VID_OFFSET),V0_DR_THR7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingk2(HI_U32 u32Data, HI_U32 dr_ringk2)
{
	U_V0_DR_THR8 V0_DR_THR8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingk2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR8.u32) + u32Data * VID_OFFSET));
	V0_DR_THR8.bits.dr_ringk2 = dr_ringk2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR8.u32) + u32Data * VID_OFFSET),V0_DR_THR8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrRingmin(HI_U32 u32Data, HI_U32 dr_ringmin)
{
	U_V0_DR_THR8 V0_DR_THR8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrRingmin Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR8.u32) + u32Data * VID_OFFSET));
	V0_DR_THR8.bits.dr_ringmin = dr_ringmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR8.u32) + u32Data * VID_OFFSET),V0_DR_THR8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstresalterthd(HI_U32 u32Data, HI_U32 dr_estresalterthd)
{
	U_V0_DR_THR9 V0_DR_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstresalterthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET));
	V0_DR_THR9.bits.dr_estresalterthd = dr_estresalterthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET),V0_DR_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstresaltertmp(HI_U32 u32Data, HI_U32 dr_estresaltertmp)
{
	U_V0_DR_THR9 V0_DR_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstresaltertmp Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET));
	V0_DR_THR9.bits.dr_estresaltertmp = dr_estresaltertmp;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET),V0_DR_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstproringthd(HI_U32 u32Data, HI_U32 dr_estproringthd)
{
	U_V0_DR_THR9 V0_DR_THR9;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstproringthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET));
	V0_DR_THR9.bits.dr_estproringthd = dr_estproringthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR9.u32) + u32Data * VID_OFFSET),V0_DR_THR9.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmooththd0(HI_U32 u32Data, HI_U32 dr_shtsmooththd0)
{
	U_V0_DR_THR10 V0_DR_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmooththd0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET));
	V0_DR_THR10.bits.dr_shtsmooththd0 = dr_shtsmooththd0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET),V0_DR_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmooththd1(HI_U32 u32Data, HI_U32 dr_shtsmooththd1)
{
	U_V0_DR_THR10 V0_DR_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmooththd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET));
	V0_DR_THR10.bits.dr_shtsmooththd1 = dr_shtsmooththd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET),V0_DR_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmooththd2(HI_U32 u32Data, HI_U32 dr_shtsmooththd2)
{
	U_V0_DR_THR10 V0_DR_THR10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmooththd2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET));
	V0_DR_THR10.bits.dr_shtsmooththd2 = dr_shtsmooththd2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR10.u32) + u32Data * VID_OFFSET),V0_DR_THR10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmoothvalue0(HI_U32 u32Data, HI_U32 dr_shtsmoothvalue0)
{
	U_V0_DR_THR11 V0_DR_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmoothvalue0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET));
	V0_DR_THR11.bits.dr_shtsmoothvalue0 = dr_shtsmoothvalue0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET),V0_DR_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmoothvalue1(HI_U32 u32Data, HI_U32 dr_shtsmoothvalue1)
{
	U_V0_DR_THR11 V0_DR_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmoothvalue1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET));
	V0_DR_THR11.bits.dr_shtsmoothvalue1 = dr_shtsmoothvalue1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET),V0_DR_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShtsmoothvalue2(HI_U32 u32Data, HI_U32 dr_shtsmoothvalue2)
{
	U_V0_DR_THR11 V0_DR_THR11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShtsmoothvalue2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET));
	V0_DR_THR11.bits.dr_shtsmoothvalue2 = dr_shtsmoothvalue2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR11.u32) + u32Data * VID_OFFSET),V0_DR_THR11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstprogradthd1(HI_U32 u32Data, HI_U32 dr_estprogradthd1)
{
	U_V0_DR_THR12 V0_DR_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstprogradthd1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET));
	V0_DR_THR12.bits.dr_estprogradthd1 = dr_estprogradthd1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET),V0_DR_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstprogradthd0(HI_U32 u32Data, HI_U32 dr_estprogradthd0)
{
	U_V0_DR_THR12 V0_DR_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstprogradthd0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET));
	V0_DR_THR12.bits.dr_estprogradthd0 = dr_estprogradthd0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET),V0_DR_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstprogradcoef(HI_U32 u32Data, HI_U32 dr_estprogradcoef)
{
	U_V0_DR_THR12 V0_DR_THR12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstprogradcoef Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET));
	V0_DR_THR12.bits.dr_estprogradcoef = dr_estprogradcoef;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR12.u32) + u32Data * VID_OFFSET),V0_DR_THR12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstcomthd(HI_U32 u32Data, HI_U32 dr_estcomthd)
{
	U_V0_DR_THR13 V0_DR_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstcomthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET));
	V0_DR_THR13.bits.dr_estcomthd = dr_estcomthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET),V0_DR_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstcomratio1(HI_U32 u32Data, HI_U32 dr_estcomratio1)
{
	U_V0_DR_THR13 V0_DR_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstcomratio1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET));
	V0_DR_THR13.bits.dr_estcomratio1 = dr_estcomratio1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET),V0_DR_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstcomratio2(HI_U32 u32Data, HI_U32 dr_estcomratio2)
{
	U_V0_DR_THR13 V0_DR_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstcomratio2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET));
	V0_DR_THR13.bits.dr_estcomratio2 = dr_estcomratio2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET),V0_DR_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstcomtmp(HI_U32 u32Data, HI_U32 dr_estcomtmp)
{
	U_V0_DR_THR13 V0_DR_THR13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstcomtmp Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET));
	V0_DR_THR13.bits.dr_estcomtmp = dr_estcomtmp;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR13.u32) + u32Data * VID_OFFSET),V0_DR_THR13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrEstpixeldiff(HI_U32 u32Data, HI_U32 dr_estpixeldiff)
{
	U_V0_DR_THR14 V0_DR_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrEstpixeldiff Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET));
	V0_DR_THR14.bits.dr_estpixeldiff = dr_estpixeldiff;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET),V0_DR_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavescope0(HI_U32 u32Data, HI_U32 dr_limitwavescope0)
{
	U_V0_DR_THR14 V0_DR_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavescope0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET));
	V0_DR_THR14.bits.dr_limitwavescope0 = dr_limitwavescope0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET),V0_DR_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavescope1(HI_U32 u32Data, HI_U32 dr_limitwavescope1)
{
	U_V0_DR_THR14 V0_DR_THR14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavescope1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET));
	V0_DR_THR14.bits.dr_limitwavescope1 = dr_limitwavescope1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR14.u32) + u32Data * VID_OFFSET),V0_DR_THR14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavenum0(HI_U32 u32Data, HI_U32 dr_limitwavenum0)
{
	U_V0_DR_THR15 V0_DR_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavenum0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET));
	V0_DR_THR15.bits.dr_limitwavenum0 = dr_limitwavenum0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET),V0_DR_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavenum1(HI_U32 u32Data, HI_U32 dr_limitwavenum1)
{
	U_V0_DR_THR15 V0_DR_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavenum1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET));
	V0_DR_THR15.bits.dr_limitwavenum1 = dr_limitwavenum1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET),V0_DR_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavenum2(HI_U32 u32Data, HI_U32 dr_limitwavenum2)
{
	U_V0_DR_THR15 V0_DR_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavenum2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET));
	V0_DR_THR15.bits.dr_limitwavenum2 = dr_limitwavenum2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET),V0_DR_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitgrad(HI_U32 u32Data, HI_U32 dr_limitgrad)
{
	U_V0_DR_THR15 V0_DR_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitgrad Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET));
	V0_DR_THR15.bits.dr_limitgrad = dr_limitgrad;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET),V0_DR_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitscope(HI_U32 u32Data, HI_U32 dr_limitscope)
{
	U_V0_DR_THR15 V0_DR_THR15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitscope Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET));
	V0_DR_THR15.bits.dr_limitscope = dr_limitscope;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR15.u32) + u32Data * VID_OFFSET),V0_DR_THR15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrCalcsum5error(HI_U32 u32Data, HI_U32 dr_calcsum5error)
{
	U_V0_DR_THR16 V0_DR_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrCalcsum5error Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET));
	V0_DR_THR16.bits.dr_calcsum5error = dr_calcsum5error;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET),V0_DR_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLargewaveblend(HI_U32 u32Data, HI_U32 dr_largewaveblend)
{
	U_V0_DR_THR16 V0_DR_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLargewaveblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET));
	V0_DR_THR16.bits.dr_largewaveblend = dr_largewaveblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET),V0_DR_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLargewavesumblend(HI_U32 u32Data, HI_U32 dr_largewavesumblend)
{
	U_V0_DR_THR16 V0_DR_THR16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLargewavesumblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET));
	V0_DR_THR16.bits.dr_largewavesumblend = dr_largewavesumblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR16.u32) + u32Data * VID_OFFSET),V0_DR_THR16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrPixelrange(HI_U32 u32Data, HI_U32 dr_pixelrange)
{
	U_V0_DR_THR17 V0_DR_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrPixelrange Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET));
	V0_DR_THR17.bits.dr_pixelrange = dr_pixelrange;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET),V0_DR_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrCalcwaverange(HI_U32 u32Data, HI_U32 dr_calcwaverange)
{
	U_V0_DR_THR17 V0_DR_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrCalcwaverange Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET));
	V0_DR_THR17.bits.dr_calcwaverange = dr_calcwaverange;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET),V0_DR_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLargewaveminthd(HI_U32 u32Data, HI_U32 dr_largewaveminthd)
{
	U_V0_DR_THR17 V0_DR_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLargewaveminthd Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET));
	V0_DR_THR17.bits.dr_largewaveminthd = dr_largewaveminthd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET),V0_DR_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrWavepronumlim(HI_U32 u32Data, HI_U32 dr_wavepronumlim)
{
	U_V0_DR_THR17 V0_DR_THR17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrWavepronumlim Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET));
	V0_DR_THR17.bits.dr_wavepronumlim = dr_wavepronumlim;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR17.u32) + u32Data * VID_OFFSET),V0_DR_THR17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendx0(HI_U32 u32Data, HI_U32 dr_smoothblendx0)
{
	U_V0_DR_THR18 V0_DR_THR18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendx0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET));
	V0_DR_THR18.bits.dr_smoothblendx0 = dr_smoothblendx0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET),V0_DR_THR18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendx1(HI_U32 u32Data, HI_U32 dr_smoothblendx1)
{
	U_V0_DR_THR18 V0_DR_THR18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendx1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET));
	V0_DR_THR18.bits.dr_smoothblendx1 = dr_smoothblendx1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET),V0_DR_THR18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendx2(HI_U32 u32Data, HI_U32 dr_smoothblendx2)
{
	U_V0_DR_THR18 V0_DR_THR18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendx2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET));
	V0_DR_THR18.bits.dr_smoothblendx2 = dr_smoothblendx2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR18.u32) + u32Data * VID_OFFSET),V0_DR_THR18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendx3(HI_U32 u32Data, HI_U32 dr_smoothblendx3)
{
	U_V0_DR_THR19 V0_DR_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendx3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET));
	V0_DR_THR19.bits.dr_smoothblendx3 = dr_smoothblendx3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET),V0_DR_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendy0(HI_U32 u32Data, HI_U32 dr_smoothblendy0)
{
	U_V0_DR_THR19 V0_DR_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendy0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET));
	V0_DR_THR19.bits.dr_smoothblendy0 = dr_smoothblendy0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET),V0_DR_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendy1(HI_U32 u32Data, HI_U32 dr_smoothblendy1)
{
	U_V0_DR_THR19 V0_DR_THR19;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendy1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR19.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET));
	V0_DR_THR19.bits.dr_smoothblendy1 = dr_smoothblendy1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR19.u32) + u32Data * VID_OFFSET),V0_DR_THR19.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendy2(HI_U32 u32Data, HI_U32 dr_smoothblendy2)
{
	U_V0_DR_THR20 V0_DR_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendy2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET));
	V0_DR_THR20.bits.dr_smoothblendy2 = dr_smoothblendy2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET),V0_DR_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendy3(HI_U32 u32Data, HI_U32 dr_smoothblendy3)
{
	U_V0_DR_THR20 V0_DR_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendy3 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET));
	V0_DR_THR20.bits.dr_smoothblendy3 = dr_smoothblendy3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET),V0_DR_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendmax(HI_U32 u32Data, HI_U32 dr_smoothblendmax)
{
	U_V0_DR_THR20 V0_DR_THR20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendmax Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET));
	V0_DR_THR20.bits.dr_smoothblendmax = dr_smoothblendmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR20.u32) + u32Data * VID_OFFSET),V0_DR_THR20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendk0(HI_U32 u32Data, HI_U32 dr_smoothblendk0)
{
	U_V0_DR_THR21 V0_DR_THR21;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendk0 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR21.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR21.u32) + u32Data * VID_OFFSET));
	V0_DR_THR21.bits.dr_smoothblendk0 = dr_smoothblendk0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR21.u32) + u32Data * VID_OFFSET),V0_DR_THR21.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendmin(HI_U32 u32Data, HI_U32 dr_smoothblendmin)
{
	U_V0_DR_THR21 V0_DR_THR21;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendmin Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR21.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR21.u32) + u32Data * VID_OFFSET));
	V0_DR_THR21.bits.dr_smoothblendmin = dr_smoothblendmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR21.u32) + u32Data * VID_OFFSET),V0_DR_THR21.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendk1(HI_U32 u32Data, HI_U32 dr_smoothblendk1)
{
	U_V0_DR_THR22 V0_DR_THR22;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendk1 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR22.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR22.u32) + u32Data * VID_OFFSET));
	V0_DR_THR22.bits.dr_smoothblendk1 = dr_smoothblendk1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR22.u32) + u32Data * VID_OFFSET),V0_DR_THR22.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrNoflagareablend(HI_U32 u32Data, HI_U32 dr_noflagareablend)
{
	U_V0_DR_THR22 V0_DR_THR22;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrNoflagareablend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR22.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR22.u32) + u32Data * VID_OFFSET));
	V0_DR_THR22.bits.dr_noflagareablend = dr_noflagareablend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR22.u32) + u32Data * VID_OFFSET),V0_DR_THR22.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrSmoothblendk2(HI_U32 u32Data, HI_U32 dr_smoothblendk2)
{
	U_V0_DR_THR23 V0_DR_THR23;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrSmoothblendk2 Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR23.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR23.u32) + u32Data * VID_OFFSET));
	V0_DR_THR23.bits.dr_smoothblendk2 = dr_smoothblendk2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR23.u32) + u32Data * VID_OFFSET),V0_DR_THR23.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrPixellargeerror(HI_U32 u32Data, HI_U32 dr_pixellargeerror)
{
	U_V0_DR_THR23 V0_DR_THR23;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrPixellargeerror Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR23.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR23.u32) + u32Data * VID_OFFSET));
	V0_DR_THR23.bits.dr_pixellargeerror = dr_pixellargeerror;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR23.u32) + u32Data * VID_OFFSET),V0_DR_THR23.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrConpixelerror(HI_U32 u32Data, HI_U32 dr_conpixelerror)
{
	U_V0_DR_THR24 V0_DR_THR24;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrConpixelerror Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR24.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET));
	V0_DR_THR24.bits.dr_conpixelerror = dr_conpixelerror;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET),V0_DR_THR24.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrCon5pixelerror(HI_U32 u32Data, HI_U32 dr_con5pixelerror)
{
	U_V0_DR_THR24 V0_DR_THR24;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrCon5pixelerror Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR24.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET));
	V0_DR_THR24.bits.dr_con5pixelerror = dr_con5pixelerror;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET),V0_DR_THR24.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrConallpixelerror(HI_U32 u32Data, HI_U32 dr_conallpixelerror)
{
	U_V0_DR_THR24 V0_DR_THR24;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrConallpixelerror Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR24.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET));
	V0_DR_THR24.bits.dr_conallpixelerror = dr_conallpixelerror;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR24.u32) + u32Data * VID_OFFSET),V0_DR_THR24.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrReturnblend(HI_U32 u32Data, HI_U32 dr_returnblend)
{
	U_V0_DR_THR25 V0_DR_THR25;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrReturnblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET));
	V0_DR_THR25.bits.dr_returnblend = dr_returnblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET),V0_DR_THR25.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrConsumblend(HI_U32 u32Data, HI_U32 dr_consumblend)
{
	U_V0_DR_THR25 V0_DR_THR25;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrConsumblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET));
	V0_DR_THR25.bits.dr_consumblend = dr_consumblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET),V0_DR_THR25.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitpixelnum(HI_U32 u32Data, HI_U32 dr_limitpixelnum)
{
	U_V0_DR_THR25 V0_DR_THR25;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitpixelnum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET));
	V0_DR_THR25.bits.dr_limitpixelnum = dr_limitpixelnum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET),V0_DR_THR25.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitsmoothpronum(HI_U32 u32Data, HI_U32 dr_limitsmoothpronum)
{
	U_V0_DR_THR25 V0_DR_THR25;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitsmoothpronum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET));
	V0_DR_THR25.bits.dr_limitsmoothpronum = dr_limitsmoothpronum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET),V0_DR_THR25.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrLimitwavepronum(HI_U32 u32Data, HI_U32 dr_limitwavepronum)
{
	U_V0_DR_THR25 V0_DR_THR25;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrLimitwavepronum Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR25.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET));
	V0_DR_THR25.bits.dr_limitwavepronum = dr_limitwavepronum;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR25.u32) + u32Data * VID_OFFSET),V0_DR_THR25.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrCrossblend(HI_U32 u32Data, HI_U32 dr_crossblend)
{
	U_V0_DR_THR26 V0_DR_THR26;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrCrossblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR26.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET));
	V0_DR_THR26.bits.dr_crossblend = dr_crossblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET),V0_DR_THR26.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrShootblend(HI_U32 u32Data, HI_U32 dr_shootblend)
{
	U_V0_DR_THR26 V0_DR_THR26;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrShootblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR26.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET));
	V0_DR_THR26.bits.dr_shootblend = dr_shootblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET),V0_DR_THR26.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrBorderprcwidth(HI_U32 u32Data, HI_U32 dr_borderprcwidth)
{
	U_V0_DR_THR26 V0_DR_THR26;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrBorderprcwidth Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR26.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET));
	V0_DR_THR26.bits.dr_borderprcwidth = dr_borderprcwidth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET),V0_DR_THR26.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDrResblend(HI_U32 u32Data, HI_U32 dr_resblend)
{
	U_V0_DR_THR26 V0_DR_THR26;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetDrResblend Select Wrong Layer ID\n");
		return ;
	}
	
	V0_DR_THR26.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET));
	V0_DR_THR26.bits.dr_resblend = dr_resblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DR_THR26.u32) + u32Data * VID_OFFSET),V0_DR_THR26.u32);
	
	return ;
}


HI_VOID VDP_VID_SetDBBordFlag(HI_U32 u32Data,HI_U32 bordflag128[128])
{
    //U_V0_DB_BORD_FLAG      V0_DB_BORD_FLAG[128];
	int i=0;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDBBordFlag() select wrong video layer ID\n");
      return ;
    }

    for( i=0; i<128;i++)
    {
      VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DB_BORD_FLAG[i].u32)  + u32Data * VID_OFFSET), bordflag128[i]);
    }
    return;
}
 
HI_VOID VDP_VID_SetDbmCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_DBM_CFGAD         V0_DBM_CFGAD;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetDbmCfgAddr() select wrong video layer ID\n");
      return ;
    }

    V0_DBM_CFGAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBM_CFGAD.u32) + u32Data * VID_OFFSET));

    V0_DBM_CFGAD.bits.coef_addr = u32Addr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBM_CFGAD.u32) + u32Data * VID_OFFSET), V0_DBM_CFGAD.u32);
    return;
}


HI_VOID  VDP_VID_SetDbmAddr   (HI_U32 u32Data, HI_U32 u32CAddr, HI_U32 u32CStr)
{
    U_V0_P2STRIDE          V0_P2STRIDE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDbmAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P2CADDR.u32) + u32Data * VID_OFFSET), u32CAddr); 


    V0_P2STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P2STRIDE.u32) + u32Data * VID_OFFSET));
    V0_P2STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P2STRIDE.u32) + u32Data * VID_OFFSET), V0_P2STRIDE.u32); 

    return ;
}

HI_VOID  VDP_VID_SetDbmInfoReso(HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_DBM_RESO        V0_DBM_RESO;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDbmInfoReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DBM_RESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBM_RESO.u32) + u32Data * VID_OFFSET));
    V0_DBM_RESO.bits.info_width   = stRect.u32Wth - 1;
    V0_DBM_RESO.bits.info_height  = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DBM_RESO.u32) + u32Data * VID_OFFSET), V0_DBM_RESO.u32); 

    return ;
}

HI_VOID  VDP_VID_SetDbmInfoEnable(HI_U32 u32Data, HI_U32  dbm_info_en)
{
    U_V0_PRERD        V0_PRERD;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDbmInfoEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.dbm_info_en   = dbm_info_en;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32); 

    return ;
}
//DBM CFG END

//STT CFG
HI_VOID VDP_VID_SetSttWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr)
{
    U_WBC_ME_CADDR   WBC_ME_CADDR;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetSttWbcAddr() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_CADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_CADDR.u32) + u32Data * VID_OFFSET));

    WBC_ME_CADDR.bits.wbccaddr = u32YAddr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_CADDR.u32) + u32Data * VID_OFFSET), WBC_ME_CADDR.u32);
    return;
}

HI_VOID VDP_VID_SetSttWbcStride(HI_U32 u32Data, HI_U32 u32YStride)
{
    U_WBC_ME_STRIDE   WBC_ME_STRIDE;
    if(u32Data >= VID_MAX)
    {
      VDP_PRINT("Error,VDP_VID_SetSttWbcStride() select wrong video layer ID\n");
      return ;
    }

    WBC_ME_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_ME_STRIDE.u32) + u32Data * VID_OFFSET));

    WBC_ME_STRIDE.bits.wbccstride = u32YStride;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_ME_STRIDE.u32) + u32Data * VID_OFFSET), WBC_ME_STRIDE.u32);
    return;
}
//STT CFG END

