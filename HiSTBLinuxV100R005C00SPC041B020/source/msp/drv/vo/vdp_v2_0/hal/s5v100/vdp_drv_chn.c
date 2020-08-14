//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_drv_chn.cpp
// Author      : s173141
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


#include "vdp_drv_comm.h"
#include "vdp_drv_chn.h"
#include "vdp_define.h"
//#include "vdp_fun.h"

#define W_1080P 1920
#define H_1080P 1080
#define W_720P 1280
#define H_720P 720
#define W_D1 720
#define H_D1 576
#define W_2560P 3480
#define H_2560P 2160

extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_DISP_StartPos(HI_U32 u32hd_id, HI_U32 startpos)
{
    U_DHD0_START_POS DHD0_START_POS;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_START_POS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_START_POS.u32)+u32hd_id*CHN_OFFSET));
    DHD0_START_POS.bits.start_pos = startpos;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_START_POS.u32)+u32hd_id*CHN_OFFSET),DHD0_START_POS.u32);

    VDP_DISP_SetRegUp(u32hd_id);
}

HI_VOID VDP_DISP_TimingStartPos(HI_U32 u32hd_id, HI_U32 TimingStartPos)
{
    U_DHD0_START_POS DHD0_START_POS;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_START_POS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_START_POS.u32)+u32hd_id*CHN_OFFSET));
    DHD0_START_POS.bits.timing_start_pos = TimingStartPos;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_START_POS.u32)+u32hd_id*CHN_OFFSET),DHD0_START_POS.u32);

    VDP_DISP_SetRegUp(u32hd_id);
}

HI_VOID VDP_DISP_GetDispMode(HI_U32 u32hd_id, HI_U32 *p3DMode)
{
    U_DHD0_CTRL DHD0_CTRL;
    
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error, VDP_DISP_GetDispMode Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    *p3DMode = DHD0_CTRL.bits.disp_mode;
}

HI_VOID VDP_DISP_SetDispMode  (HI_U32 u32hd_id, HI_U32 u32DispMode)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetFramePackingEn Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.disp_mode = u32DispMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetHdmiMode  (HI_U32 u32hd_id, HI_U32 u32hdmi_md)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetHdmiMode Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.hdmi_mode = u32hdmi_md;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetRegUp (HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetIntfEnable(HI_U32 u32hd_id, HI_U32 bTrue)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.intf_en = bTrue;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);

    VDP_DISP_SetRegUp(u32hd_id);
}
HI_VOID VDP_DISP_GetIntfEnable(HI_U32 u32hd_id, HI_U32 *pbTrue)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_GetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    *pbTrue = DHD0_CTRL.bits.intf_en;
}


HI_VOID VDP_DISP_SetIntMask  (HI_U32 u32masktypeen)
{
    U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 | u32masktypeen;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOINTMSK.u32)), VOINTMSK.u32); 

    return ;
}

HI_VOID  VDP_DISP_SetIntDisable(HI_U32 u32masktypeen)
{
    U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 & (~u32masktypeen);
    VDP_RegWrite((HI_U32)&(pVdpReg->VOINTMSK.u32), VOINTMSK.u32);

    return ;
}

HI_VOID VDP_DISP_SetOflIntMask  (HI_U32 u32masktypeen)
{
    U_VOINTMSK_OFL VOINTMSK_OFL;

    /* Dispaly interrupt mask enable */
    VOINTMSK_OFL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK_OFL.u32)));
    VOINTMSK_OFL.u32 = VOINTMSK_OFL.u32 | u32masktypeen;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOINTMSK_OFL.u32)), VOINTMSK_OFL.u32); 

    return ;
}

HI_VOID  VDP_DISP_SetOflIntDisable(HI_U32 u32masktypeen)
{
    U_VOINTMSK_OFL VOINTMSK_OFL;

    /* Dispaly interrupt mask enable */
    VOINTMSK_OFL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK_OFL.u32)));
    VOINTMSK_OFL.u32 = VOINTMSK_OFL.u32 & (~u32masktypeen);
    VDP_RegWrite((HI_U32)&(pVdpReg->VOINTMSK_OFL.u32), VOINTMSK_OFL.u32);

    return ;
}
HI_VOID  VDP_DISP_BfmClkSel(HI_U32 u32Num)
{
    U_VODEBUG  VODEBUG ;
    /* Dispaly interrupt mask enable */
    VODEBUG.u32        = VDP_RegRead((HI_U32)(&(pVdpReg->VODEBUG.u32)));
    VODEBUG.bits.bfm_clk_sel = u32Num;
    VDP_RegWrite((HI_U32)&(pVdpReg->VODEBUG.u32), VODEBUG.u32);
    return ;
}
HI_VOID  VDP_DISP_OpenIntf(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S stSyncInfo)
{
    U_DHD0_CTRL DHD0_CTRL;
    U_DHD0_VSYNC DHD0_VSYNC;
    U_DHD0_VPLUS DHD0_VPLUS;
    U_DHD0_PWR DHD0_PWR;
    U_DHD0_HSYNC1 DHD0_HSYNC1;
    U_DHD0_HSYNC2 DHD0_HSYNC2;

#if 0
    //if((6.6 < clk_vo_hd_t< 6.8 ) & (6.6 < clk_vo_sd_t< 6.8))//hdsd:148.5M
    if(( clk_vo_hd_t == 6.73 ) & ( clk_vo_sd_t == 6.73 ))//hdsd:148.5M
    {
        VDP_DISP_BfmClkSel(0);
    }
    //else if((6.6 < clk_vo_hd_t< 6.8 ) & (36 < clk_vo_sd_t< 38))//hd:148.5  sd:27
    else if(( clk_vo_hd_t == 6.73 ) & ( clk_vo_sd_t == 37 ))//hdsd:148.5M
    {
        VDP_DISP_BfmClkSel(1);
    }
    else if(((6.6 < clk_vo_hd_t) & (clk_vo_hd_t < 6.8 )) & ((36*2 < clk_vo_sd_t ) & (clk_vo_sd_t  < 38*2)))//hd:148.5  sd:27/2=13.5
    {
        VDP_DISP_BfmClkSel(2);
    }
    else if(((6.6*2 < clk_vo_hd_t) & (clk_vo_hd_t < 6.8*2 )) & ((6.6*2 < clk_vo_sd_t ) & (clk_vo_sd_t  < 6.8*2)))//hdsd:74.25
    {
        VDP_DISP_BfmClkSel(3);
    }
    else if(((6.6*2 < clk_vo_hd_t) & (clk_vo_hd_t < 6.8*2 )) & ((36 < clk_vo_sd_t ) & (clk_vo_sd_t  < 38 )))//hdsd:74.25
    {
        VDP_DISP_BfmClkSel(4);
    }
    else if(((6.6*2 < clk_vo_hd_t) & (clk_vo_hd_t < 6.8*2 )) & ((36*2 < clk_vo_sd_t ) & (clk_vo_sd_t  < 38*2 )))//hd:74.25 sd:27/2=13.5M
    {
        VDP_DISP_BfmClkSel(5);
    }
    else //hd:148.5 sd:27/2=13.5M
    {
        VDP_DISP_BfmClkSel(6);
    }
#endif

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_OpenIntf Select Wrong CHANNEL ID\n");
        return ;
    }


    /*
    //VOU VHD CHANNEL enable 
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.iop   = stSyncInfo.bIop;// 
    //DHD0_CTRL.bits.intfb = //stSyncInfo.u32Intfb; in intf
    //DHD0_CTRL.bits.synm  = //stSyncInfo.bSynm;
    DHD0_CTRL.bits.idv   = stSyncInfo.bIdv;//DATA INV
    DHD0_CTRL.bits.ihs   = stSyncInfo.bIhs;// H PULSE INV
    DHD0_CTRL.bits.ivs   = stSyncInfo.bIvs;//V PULSE INV
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
     */
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.iop   = stSyncInfo.bIop;// 
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);


    DHD0_HSYNC1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC1.bits.hact = stSyncInfo.u32Hact -1;
    DHD0_HSYNC1.bits.hbb  = stSyncInfo.u32Hbb -1;
    DHD0_HSYNC2.bits.hfb  = stSyncInfo.u32Hfb -1;
    DHD0_HSYNC2.bits.hmid = (stSyncInfo.u32Hmid == 0) ? 0 : stSyncInfo.u32Hmid -1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC2.u32);

    //Config VHD interface veritical timming
    DHD0_VSYNC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VSYNC.bits.vact = stSyncInfo.u32Vact  -1;
    DHD0_VSYNC.bits.vbb = stSyncInfo.u32Vbb - 1;
    DHD0_VSYNC.bits.vfb =  stSyncInfo.u32Vfb - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET), DHD0_VSYNC.u32);

    //Config VHD interface veritical bottom timming,no use in progressive mode
    DHD0_VPLUS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VPLUS.bits.bvact = stSyncInfo.u32Bvact - 1;
    DHD0_VPLUS.bits.bvbb =  stSyncInfo.u32Bvbb - 1;
    DHD0_VPLUS.bits.bvfb =  stSyncInfo.u32Bvfb - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET), DHD0_VPLUS.u32);

    //Config VHD interface veritical bottom timming, 
    DHD0_PWR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET));        
    DHD0_PWR.bits.hpw = stSyncInfo.u32Hpw - 1;
    DHD0_PWR.bits.vpw = stSyncInfo.u32Vpw - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET), DHD0_PWR.u32);
}



HI_VOID  VDP_DISP_SetIntfMuxSel(HI_U32 u32hd_id,VDP_DISP_INTF_E enIntf)
{
    U_VO_MUX VO_MUX;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetIntfMuxSel Select Wrong CHANNEL ID\n");
        return ;
    }

    switch(enIntf)
    {
#if INTF_EN
        case VDP_DISP_INTF_LCD:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = 2; 
            VO_MUX.bits.lcd_sel = u32hd_id; 
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.bt1120_sel = u32hd_id;
            VO_MUX.bits.digital_sel = 0; 
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
#else
        case VDP_DISP_INTF_LCD:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = u32hd_id*2; 
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = 1+u32hd_id*2; 
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
#endif
        case VDP_DISP_INTF_HDMI:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.hdmi_sel = u32hd_id; 
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_VGA:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.vga_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_HDDATE:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.hddate_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_SDDATE:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.sddate_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        default:
        {
            HI_PRINT("Error! VDP_DISP_SetIntfMuxSel Wrong Select\n");
            return ;
        }
    }
    return ;
}

HI_U32 VDP_DISP_GetIntSta(HI_U32 u32intmask)
{
    U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTSTA.u32)));

    return (VOINTSTA.u32 & u32intmask);
}

HI_VOID  VDP_DISP_ClearIntSta(HI_U32 u32intmask)
{
    /* clear interrupt status */
    //U_VOMSKINTSTA VOMSKINTSTA;
    VDP_RegWrite((HI_U32)&(pVdpReg->VOMSKINTSTA.u32), u32intmask);
}


HI_U32 VDP_DISP_GetOflIntSta(HI_U32 u32intmask)
{
    U_VOINTSTA_OFL VOINTSTA_OFL;

    /* read interrupt status */
    VOINTSTA_OFL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTSTA_OFL.u32)));

    return (VOINTSTA_OFL.u32 & u32intmask);
}

HI_VOID  VDP_DISP_ClearOflIntSta(HI_U32 u32intmask)
{
    /* clear interrupt status */
    //U_VOMSKINTSTA VOMSKINTSTA;
    VDP_RegWrite((HI_U32)&(pVdpReg->VOMSKINTSTA_OFL.u32), u32intmask);
}

HI_VOID VDP_DISP_SetVtThdMode(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode)
{
    U_DHD0_VTTHD3 DHD0_VTTHD3;
    U_DHD0_VTTHD  DHD0_VTTHD;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetVtThdMode Select Wrong CHANNEL ID\n");
        return ;
    }

    if(u32uthdnum == 1)//threshold 1 int mode
    {
        DHD0_VTTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VTTHD.bits.thd1_mode = u32mode;// frame or field
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 2)//threshold 2 int mode
    {
        DHD0_VTTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD.bits.thd2_mode = u32mode;// frame or field
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 3)//threshold 3 int mode
    {
        DHD0_VTTHD3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD3.bits.thd3_mode = u32mode;// frame or field
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD3.u32);            

    }
}

HI_VOID VDP_DISP_SetVtThd(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd)
{

    U_DHD0_VTTHD DHD0_VTTHD;
    U_DHD0_VTTHD3 DHD0_VTTHD3;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetVtThd Select Wrong CHANNEL ID\n");
        return ;
    }

    if(u32uthdnum == 1)
    {
        DHD0_VTTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VTTHD.bits.vtmgthd1 = u32vtthd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 2)
    {
        DHD0_VTTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD.bits.vtmgthd2 = u32vtthd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum== 3)
    {
        DHD0_VTTHD3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD3.bits.vtmgthd3 = u32vtthd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD3.u32);            
    }
}

HI_VOID VDP_DISP_SetCbarEnable(HI_U32 u32hd_id,HI_U32 bTrue)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_SetCbarEnable Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.cbar_en = bTrue;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetCbarSel(HI_U32 u32hd_id,HI_U32 u32_cbar_sel)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_SetCbarSel Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.cbar_sel = u32_cbar_sel;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetDitherEnable(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, HI_U32 u32Enable)
{

    U_DHD0_DITHER_CTRL DHD0_DITHER_CTRL;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    U_DHD0_DITHER1_CTRL DHD0_DITHER1_CTRL;

    if(enIntf == VDP_DISP_INTF_LCD )
    {

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_en= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_en= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
        HI_PRINT("Error,VDP_DISP_SetDitherEnable  Select Wrong Interface Mode\n");
        return ;

    }

}


HI_VOID VDP_DISP_SetDitherRoundEnable(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, HI_U32 u32Enable)
{

    U_DHD0_DITHER_CTRL DHD0_DITHER_CTRL;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    U_DHD0_DITHER1_CTRL DHD0_DITHER1_CTRL;

    if(enIntf == VDP_DISP_INTF_LCD )
    {

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_round = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
        HI_PRINT("Error,VDP_DISP_SetDitherEnable  Select Wrong Interface Mode\n");
        return ;

    }

}
HI_VOID VDP_DISP_SetDitherMode  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_E enDitherMode)

{
    U_DHD0_DITHER_CTRL DHD0_DITHER_CTRL;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    U_DHD0_DITHER1_CTRL DHD0_DITHER1_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong CHANNEL ID\n");
        return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD)
    {

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_mode = enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
        HI_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong Interface Mode\n");
        return ;

    }

}

HI_VOID VDP_DISP_SetDitherCoef  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_COEF_S dither_coef)

{
    U_DHD0_DITHER0_COEF0 DHD0_DITHER0_COEF0;
    U_DHD0_DITHER_COEF0 DHD0_DITHER_COEF0;
    U_DHD0_DITHER0_COEF1 DHD0_DITHER0_COEF1;
    U_DHD0_DITHER_COEF1 DHD0_DITHER_COEF1;
    U_DHD0_DITHER1_COEF0 DHD0_DITHER1_COEF0;
    U_DHD0_DITHER1_COEF1 DHD0_DITHER1_COEF1;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD)
    {

        DHD0_DITHER0_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER0_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        DHD0_DITHER0_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        DHD0_DITHER0_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        DHD0_DITHER0_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER0_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_COEF0.u32);

        DHD0_DITHER0_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER0_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        DHD0_DITHER0_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        DHD0_DITHER0_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        DHD0_DITHER0_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER0_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_COEF1.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER1_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_COEF0.bits.dither_coef0 =dither_coef.dither_coef0 ;
        DHD0_DITHER1_COEF0.bits.dither_coef1 =dither_coef.dither_coef1 ;
        DHD0_DITHER1_COEF0.bits.dither_coef2 =dither_coef.dither_coef2 ;
        DHD0_DITHER1_COEF0.bits.dither_coef3 =dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER1_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_COEF0.u32);

        DHD0_DITHER1_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER1_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_COEF1.bits.dither_coef4 =dither_coef.dither_coef4 ;
        DHD0_DITHER1_COEF1.bits.dither_coef5 =dither_coef.dither_coef5 ;
        DHD0_DITHER1_COEF1.bits.dither_coef6 =dither_coef.dither_coef6 ;
        DHD0_DITHER1_COEF1.bits.dither_coef7 =dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER1_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_COEF1.u32);
    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {

        DHD0_DITHER_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        DHD0_DITHER_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        DHD0_DITHER_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        DHD0_DITHER_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_COEF0.u32);

        DHD0_DITHER_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DITHER_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        DHD0_DITHER_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        DHD0_DITHER_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        DHD0_DITHER_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DITHER_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_COEF1.u32);

    }
    else 
    {
        HI_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong Interface Mode\n");
        return ;

    }

}

HI_VOID  VDP_DISP_SetClipEnable  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf)
{
    HI_PRINT("Error,This Driver has been deleted ...\n");
}

HI_VOID  VDP_DISP_SetClipCoef (HI_U32 u32hd_id, VDP_DISP_INTF_E clipsel, VDP_DISP_CLIP_S stClipData)
{   

    U_DHD0_CLIP0_L       DHD0_CLIP0_L;
    U_DHD0_CLIP0_H       DHD0_CLIP0_H;
    U_DHD0_CLIP1_L       DHD0_CLIP1_L;
    U_DHD0_CLIP1_H       DHD0_CLIP1_H;
    U_DHD0_CLIP2_L       DHD0_CLIP2_L;
    U_DHD0_CLIP2_H       DHD0_CLIP2_H;
    U_DHD0_CLIP3_L       DHD0_CLIP3_L;
    U_DHD0_CLIP3_H       DHD0_CLIP3_H;
    U_DHD0_CLIP4_L       DHD0_CLIP4_L;
    U_DHD0_CLIP4_H       DHD0_CLIP4_H;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong CHANNEL ID\n");
        return ;
    }

    //BT1120
    if(clipsel == VDP_DISP_INTF_BT1120)
    {
        DHD0_CLIP0_L.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP0_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP0_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP0_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP0_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP0_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP0_L.u32);

        DHD0_CLIP0_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP0_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP0_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP0_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP0_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP0_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP0_H.u32); 
    }

    //DATE
    else if(clipsel == VDP_DISP_INTF_SDDATE)
    {

        DHD0_CLIP1_L.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP1_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP1_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP1_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP1_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP1_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP1_L.u32);

        DHD0_CLIP1_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP1_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP1_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP1_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP1_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP1_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP1_H.u32); 
    }

    //HDMI
    else if(clipsel == VDP_DISP_INTF_HDMI)
    {

        DHD0_CLIP2_L.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP2_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP2_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP2_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP2_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP2_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP2_L.u32);

        DHD0_CLIP2_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP2_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP2_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP2_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP2_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP2_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP2_H.u32); 
    }

    //VGA
    else if(clipsel == VDP_DISP_INTF_VGA)
    {

        DHD0_CLIP3_L.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP3_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP3_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP3_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP3_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP3_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP3_L.u32);

        DHD0_CLIP3_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP3_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP3_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP3_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP3_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP3_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP3_H.u32); 
    }

    //LCD
    else if(clipsel == VDP_DISP_INTF_LCD)
    {

        DHD0_CLIP4_L.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP4_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP4_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP4_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP4_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP4_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP4_L.u32);

        DHD0_CLIP4_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CLIP4_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP4_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP4_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP4_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CLIP4_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP4_H.u32); 
    }
    else
    {
        HI_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong Interface Mode\n");
    }
}


HI_VOID VDP_DISP_SetGammaEnable(HI_U32 u32hd_id, HI_U32 u32GmmEn)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetGammaEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.gmm_en = u32GmmEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}
HI_VOID VDP_DISP_SetMirrorEnable(HI_U32 u32hd_id, HI_U32 u32MirrEn)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetGammaEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.mirror_en = u32MirrEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetGammaAddr(HI_U32 u32hd_id, HI_U32 uGmmAddr)
{
    U_DHD0_GMM_COEFAD DHD0_GMM_COEFAD;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetGammaAddr Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_GMM_COEFAD.u32 = uGmmAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_GMM_COEFAD.u32)+u32hd_id*CHN_OFFSET), DHD0_GMM_COEFAD.u32);
}

HI_VOID VDP_DISP_GetDHDState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt,
        HI_U32 *pu32IntCnt, HI_BOOL *pbRight)
{        
    U_DHD0_STATE DHD0_STATE;  
         
    if(u32hd_id >= CHN_MAX)   
    {    
        HI_PRINT("Error, VDP_DISP_GetVactState Select Wrong CHANNEL ID\n");
        return ;
    }    
         
    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_STATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_STATE.u32)+u32hd_id*CHN_OFFSET));
    if (pbBtm)
        *pbBtm         = DHD0_STATE.bits.bottom_field;
    if (pu32Vcnt)
        *pu32Vcnt      = DHD0_STATE.bits.vcnt;
    if (pu32IntCnt)
        *pu32IntCnt    = DHD0_STATE.bits.count_int;
    if (pbRight)
        *pbRight       = DHD0_STATE.bits.right_field;
         
    return;
} 

//HI_VOID VDP_DISP_SetParaUpd(HI_U32 u32hd_id, VDP_DISP_PARA_E enPara)
//{
//    U_DHD0_PARAUP DHD0_PARAUP;
//
//    if(u32hd_id >= CHN_MAX)
//    {
//        HI_PRINT("Error,VDP_DISP_SetParaUp Select Wrong CHANNEL ID\n");
//        return ;
//    }
//
//    DHD0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32)+u32hd_id*CHN_OFFSET));
//    DHD0_PARAUP.bits.dhd0_gmm_upd = 0x1;//enPara;//0x1;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32)+u32hd_id*CHN_OFFSET), DHD0_PARAUP.u32);
//}



HI_VOID VDP_INTF_SetLvdsEnable(HI_U32 bEnable)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.lvds_en  = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32); 
}

HI_VOID VDP_INTF_SetLvdsMode(HI_U32 enMode) 
{
    U_LVDS_CTRL LVDS_CTRL;
    U_LVDS_PHYCTRL0 LVDS_PHYCTRL0;
    
#if 0

    HI_U32 lvds_mode;

    //for bfm_lvds_top.v
    if(enMode == VDP_LVDS_MODE_SINGLE)
        lvds_mode = 0;
    else if(enMode == VDP_LVDS_MODE_DUAL)
        lvds_mode = 0;
    else if(enMode == VDP_LVDS_MODE_QUAD)
        lvds_mode = 0;
#endif
    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.mode = enMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);

    LVDS_PHYCTRL0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)));
    LVDS_PHYCTRL0.bits.pon = enMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)), LVDS_PHYCTRL0.u32);
}

 HI_VOID VDP_INTF_SetLvdsBitMode(HI_U32 bBitSel)
{
    U_LVDS_CTRL LVDS_CTRL;
    U_LVDS_PHYCTRL0 LVDS_PHYCTRL0;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.bit_sel = bBitSel;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);

    LVDS_PHYCTRL0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)));
    LVDS_PHYCTRL0.bits.sel8_10 = ~bBitSel;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)), LVDS_PHYCTRL0.u32);
}

HI_VOID VDP_INTF_SetLvdsSyncOutEn(HI_U32 bSyncOutEn)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.sync_out = bSyncOutEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsSwap(HI_U32 u32Swap)
{
    U_LVDS_SWAP0 LVDS_SWAP0;

    LVDS_SWAP0.u32 = u32Swap;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SWAP0.u32)), LVDS_SWAP0.u32);
}

HI_VOID VDP_INTF_SetLvdsFormat(VDP_LVDS_FORMAT_E enFormat)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.lvds_map = enFormat;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsTestMode(VDP_LVDS_TEST_MODE_E enTestMode)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.test_mode = enTestMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsInvert(VDP_LVDS_INV_E enInv, HI_U32 bInvEn)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)));
    switch(enInv)
    {
        case VDP_LVDS_INV_DATA:
        {
            LVDS_CTRL.bits.data_inv = bInvEn;
            break;
        }
        case VDP_LVDS_INV_HSYNC:
        {
            LVDS_CTRL.bits.hsync_inv = bInvEn;
            break;
        }
        case VDP_LVDS_INV_VSYNC:
        {
            LVDS_CTRL.bits.vsync_inv = bInvEn;
            break;
        }
        case VDP_LVDS_INV_DE:
        {
            LVDS_CTRL.bits.de_inv = bInvEn;
            break;
        }
        case VDP_LVDS_INV_RESV:
        {
            LVDS_CTRL.bits.resv_inv = bInvEn;
            break;
        }
        default:
        {
            //HAL_PRINT("Error! VDP_LVDS_SetInvert() Select Wrong Sync Mode!\n");
            return ;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}


HI_VOID VDP_DISP_SetWcgEnable  (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_DHDWCG_CSC2CTRL  DHDWCG_CSC2CTRL;
    U_DHD0_CSC_ODC     DHD0_CSC_ODC;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DHD_SetWcgEnable() Select Wrong DHD ID\n");
        return ;
    }
    
    DHDWCG_CSC2CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET));
    DHDWCG_CSC2CTRL.bits.csc2_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2CTRL.u32);
      
    //set csc sign mode
    DHD0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_ODC.u32)+ u32Data * CHN_OFFSET));
    DHD0_CSC_ODC.bits.csc_sign_mode = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_ODC.u32)+ u32Data * CHN_OFFSET), DHD0_CSC_ODC.u32);

}



HI_VOID VDP_DISP_SetCoefReadEnable(HI_U32 u32Id, HI_U32 u32Para)
{

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARARD.u32) + u32Id * CHN_OFFSET), u32Para);
}

HI_VOID VDP_DISP_SetCoefReadDisable(HI_U32 u32Id, HI_U32 u32Para)
{

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARARD.u32) + u32Id * CHN_OFFSET), 0);
}


//------------------------------------------------
//SR_BEGING 
//-------------------------------------------------
HI_VOID VDP_DISP_SetSrEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_SR_CTRL  SR_CTRL;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
    SR_CTRL.bits.sr_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrMux (HI_U32 u32Data, VDP_SR_MUX_E SrMux)
{
    U_SR_CTRL  SR_CTRL;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrMux() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
    SR_CTRL.bits.sr_zme_en = SrMux;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrDemoMode(HI_U32 u32Data, SR_DEMO_MODE_E Democtrl)
{
    U_SR_CTRL  SR_CTRL;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("select wrong VP layer id\n");
        return ;
    }

    SR_CTRL.u32 = VDP_RegRead((HI_U32 )(&(pVdpReg->SR_CTRL.u32)+ u32Data * CHN_OFFSET));
    SR_CTRL.bits.demo_ctrl = Democtrl;
    VDP_RegWrite((HI_U32 )(&(pVdpReg->SR_CTRL.u32)+ u32Data * CHN_OFFSET), SR_CTRL.u32);
    return ;
}

HI_VOID VDP_DISP_SetSrServeMode (HI_U32 u32Data, VDP_SR_SERVE_MODE_E Mode)
{
    U_SR_CTRL  SR_CTRL;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrServeMode() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
    SR_CTRL.bits.serve_mode = Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrGrdSubRatio      (HI_U32 u32Data, HI_U32 u32GrdSubRatio)
{
    U_SR_LDIR_RATIO SR_LDIR_RATIO;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrGrdSubRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_LDIR_RATIO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET));
    SR_LDIR_RATIO.bits.grd_sub_ratio = u32GrdSubRatio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET), SR_LDIR_RATIO.u32); 
}

HI_VOID VDP_DISP_SetSrGrdThr (HI_U32 u32Data, HI_U32 u32LowThr, HI_U32 u32HighThr)
{
    U_SR_LTEX_THR SR_LTEX_THR;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrGrdThr() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_LTEX_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET));
    SR_LTEX_THR.bits.grd_thrh = u32HighThr;
    SR_LTEX_THR.bits.grd_thrl = u32LowThr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET), SR_LTEX_THR.u32); 
}

HI_VOID VDP_DISP_SetSrGrdThrSub (HI_U32 u32Data, HI_U32 u32GrdThrSub)
{
    U_SR_LTEX_THR SR_LTEX_THR;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrGrdThrSub() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_LTEX_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET));
    SR_LTEX_THR.bits.grd_thrh_subl = u32GrdThrSub;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET), SR_LTEX_THR.u32); 
}

HI_VOID VDP_DISP_SetSrLtexAdd (HI_U32 u32Data, HI_U32 u32LtexAdd)
{
    U_SR_LTEX_ADD  SR_LTEX_ADD;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrLtexAdd() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_LTEX_ADD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET));
    SR_LTEX_ADD.bits.grd_add_ctrl = u32LtexAdd;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET), SR_LTEX_ADD.u32); 
}

HI_VOID VDP_DISP_SetSrDiscDetailGain (HI_U32 u32Data, HI_U32 u32DetaiGain)
{
    U_SR_DIPL_DETAIL  SR_DIPL_DETAIL;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrDiscDetailGain() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_DIPL_DETAIL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_DIPL_DETAIL.u32) + u32Data * CHN_OFFSET));
    SR_DIPL_DETAIL.bits.disc_detail_gain = u32DetaiGain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_DIPL_DETAIL.u32) + u32Data * CHN_OFFSET), SR_DIPL_DETAIL.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_SR_SHARP_EN  SR_SHARP_EN; 

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSharpenEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_EN.bits.sharp_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenMode (HI_U32 u32Data, VDP_SR_SHARP_MODE_E Mode)
{
    U_SR_SHARP_EN  SR_SHARP_EN; 

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSharpenMode() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_EN.bits.hsh_nodir = Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}

HI_VOID VDP_DISP_SetSrEiscDetailGain (HI_U32 u32Data, HI_U32 u32DetaiGain)
{
    U_SR_SHARP_EN  SR_SHARP_EN; 

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrEiscDetailGain() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_EN.bits.ensc_detail_gain = u32DetaiGain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenCoring (HI_U32 u32Data, HI_U32 u32SharpCoring)
{
    U_SR_SHARP_THR  SR_SHARP_THR;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSharpenCoring() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_THR.bits.sharp_coring = u32SharpCoring;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenGain (HI_U32 u32Data, HI_U32 u32SharpGain)
{
    U_SR_SHARP_THR  SR_SHARP_THR;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSharpenGain() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_THR.bits.sharp_gain = u32SharpGain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenThrsh (HI_U32 u32Data, HI_U32 u32HighThr, HI_U32 u32LowThr)
{
    U_SR_SHARP_THR  SR_SHARP_THR;

    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSharpenGain() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SHARP_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
    SR_SHARP_THR.bits.over_thrsh = u32HighThr;
    SR_SHARP_THR.bits.under_thrsh = u32LowThr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}

HI_VOID VDP_DISP_SetSrSmoothEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_SR_SMOOTH  SR_SMOOTH;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSmoothEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SMOOTH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
    SR_SMOOTH.bits.smooth_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}

HI_VOID VDP_DISP_SetSrSmoothPlusEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_SR_SMOOTH  SR_SMOOTH;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSmoothPlusEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SMOOTH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
    SR_SMOOTH.bits.plus_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}

HI_VOID VDP_DISP_SetSrSmoothPlusCtrl (HI_U32 u32Data, HI_U32 u32PlusCtrl)
{
    U_SR_SMOOTH  SR_SMOOTH;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrSmoothPlusCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_SMOOTH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
    SR_SMOOTH.bits.plus_ctrl = u32PlusCtrl;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}



//SR_ZME
HI_VOID VDP_DISP_SetSrZmeEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hlmsc_en = u32bEnable;
        SR_HSP.bits.hlfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hchmsc_en = u32bEnable;
        SR_HSP.bits.hchfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vlmsc_en = u32bEnable;
        SR_VSP.bits.vlfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vchmsc_en = u32bEnable;
        SR_VSP.bits.vchfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }

    return ;
}

HI_VOID VDP_DISP_SetSrZmePhase (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase)
{
    U_SR_HLOFFSET  SR_HLOFFSET;
    U_SR_HCOFFSET  SR_HCOFFSET;
    U_SR_VOFFSET   SR_VOFFSET;
    U_SR_VBOFFSET  SR_VBOFFSET;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HLOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HLOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_HLOFFSET.bits.hor_loffset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HLOFFSET.u32) + u32Data * CHN_OFFSET), SR_HLOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HCOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HCOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_HCOFFSET.bits.hor_coffset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HCOFFSET.u32) + u32Data * CHN_OFFSET), SR_HCOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VOFFSET.bits.vluma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET), SR_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VOFFSET.bits.vchroma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET), SR_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VBOFFSET.bits.vbluma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET), SR_VBOFFSET.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VBOFFSET.bits.vbchroma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET), SR_VBOFFSET.u32); 
    }

    return ;
}
    

HI_VOID VDP_DISP_SetSrZmeFirEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }
            
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
         SR_HSP.bits.hlfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
     }
     
     if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
         SR_HSP.bits.hchfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
     }

     if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
         SR_VSP.bits.vlfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
     }
     
     if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
         SR_VSP.bits.vchfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
     }

    return ;
}

    
HI_VOID VDP_DISP_SetSrZmeMidEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }

    return ;
}

    
    
HI_VOID VDP_DISP_SetSrZmeHorRatio (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_HSP SR_HSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
    SR_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 

    return ;
}

    
HI_VOID VDP_DISP_SetSrZmeVerRatio (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_VSR SR_VSR;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSR.u32) + u32Data * CHN_OFFSET));
    SR_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSR.u32) + u32Data * CHN_OFFSET), SR_VSR.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrZmeInFmt (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
    SR_VSP.bits.zme_in_fmt = u32Fmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrZmeOutFmt (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeOutFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
    SR_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_SR_HSP SR_HSP;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
    SR_HSP.bits.hfir_order = u32HfirOrder;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 

    return ;
}
HI_VOID  VDP_DISP_SetParaRd (HI_U32 u32Data, VDP_DISP_PARA_E enMode)
{
    U_DHD0_PARARD DHD0_PARARD;
//        cout << "yyyyyyyyyyyyyy3"<< endl;
    DHD0_PARARD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PARARD.u32) + u32Data * CHN_OFFSET));
//        cout << "yyyyyyyyyyyyyy4"<< endl;
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("error,VDP_DISP_SetParaRd() select wrong video layer id\n");
        return ;
    }
//        cout << "yyyyyyyyyyyyyy5"<< endl;
    if(enMode == VDP_DISP_PARA_ZME_HOR)
    {
        DHD0_PARARD.bits.dhd_hlcoef_rd = 0X1;
        DHD0_PARARD.bits.dhd_hccoef_rd = 0X1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_VER)
    {
        DHD0_PARARD.bits.dhd_vlcoef_rd = 0X1;
        DHD0_PARARD.bits.dhd_vccoef_rd = 0X1;
    }
    else
    {
        HI_PRINT("error,VDP_DISP_SetParaRd() select wrong mode!\n");
    }
    
 //       cout << "yyyyyyyyyyyyyy6"<< endl;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARARD.u32) + u32Data * CHN_OFFSET), DHD0_PARARD.u32); 
 //       cout << "yyyyyyyyyyyyyy7"<< endl;
    return ;
}

HI_VOID  VDP_DISP_SetParaUpd (HI_U32 u32Data, VDP_DISP_PARA_E enMode)
{
    U_DHD0_PARAUP DHD0_PARAUP;
    DHD0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32) + u32Data * CHN_OFFSET));
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("error,VDP_DISP_SetParaUpd() select wrong video layer id\n");
        return ;
    }
    if(enMode == VDP_DISP_PARA_ZME_HORL)
    {
        DHD0_PARAUP.bits.sr_hlparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_HORC)
    {
        DHD0_PARAUP.bits.sr_hcparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_VERL)
    {
        DHD0_PARAUP.bits.sr_vlparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_VERC)
    {
        DHD0_PARAUP.bits.sr_vcparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_HOR)
    {
        DHD0_PARAUP.bits.sr_hlparaup = 0x1;
        DHD0_PARAUP.bits.sr_hcparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_ZME_VER)
    {
        DHD0_PARAUP.bits.sr_vlparaup = 0x1;
        DHD0_PARAUP.bits.sr_vcparaup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_GMM)
    {
        DHD0_PARAUP.bits.dhd0_gmm_upd= 0x1;
    }
    else if(enMode == VDP_DISP_PARA_DIM_LUT)
    {
        DHD0_PARAUP.bits.dim_lut_paraup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_DIM_HF)
    {
        DHD0_PARAUP.bits.dim_hf_paraup = 0x1;
    }
    else if(enMode == VDP_DISP_PARA_DIM_VF)
    {
        DHD0_PARAUP.bits.dim_vf_paraup = 0x1;
    }
    else
    {
        HI_PRINT("error,VDP_DISP_SetParaUpd() select wrong mode!\n");
    }
    
 //       cout << "yyyyyyyyyyyyyy6"<< endl;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32) + u32Data * CHN_OFFSET), DHD0_PARAUP.u32); 
 //       cout << "yyyyyyyyyyyyyy7"<< endl;
    return ;
}


HI_VOID VDP_DISP_SetSrZmeCoefAddr (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    U_DHDHLCOEFAD        DHDHLCOEFAD;
    U_DHDHCCOEFAD        DHDHCCOEFAD;
   
    U_DHDVLCOEFAD        DHDVLCOEFAD;
    U_DHDVCCOEFAD        DHDVCCOEFAD;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }
    
    if(u32Mode == VDP_GP_PARA_ZME_HORL)
    {
        DHDHLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHDHLCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDHLCOEFAD.bits.dhdhlcoefad = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHDHLCOEFAD.u32) + u32Data * CHN_OFFSET), DHDHLCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_HORC)
    {
        DHDHCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHDHCCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDHCCOEFAD.bits.dhdhccoefad = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHDHCCOEFAD.u32) + u32Data * CHN_OFFSET), DHDHCCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VERL)
    {
        DHDVLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHDVLCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDVLCOEFAD.bits.dhdvlcoefad = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHDVLCOEFAD.u32) + u32Data * CHN_OFFSET), DHDVLCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VERC)
    {
        DHDVCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHDVCCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDVCCOEFAD.bits.dhdvccoefad = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHDVCCOEFAD.u32) + u32Data * CHN_OFFSET), DHDVCCOEFAD.u32); 
    }
    else
    {
        HI_PRINT("Error,VDP_DISP_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}

HI_VOID VDP_DISP_SetSrInReso(HI_U32 u32Data, HI_U32 u32Width, HI_U32 u32Height)
{
    U_DHD0_SRIRESO DHD0_SRIRESO;
    
    if(u32Data >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSrInReso() Select Wrong Video Layer ID\n");
        return ;
    }
    
    DHD0_SRIRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_SRIRESO.u32) + u32Data * CHN_OFFSET));
    DHD0_SRIRESO.bits.sr_iw = u32Width - 1;
    DHD0_SRIRESO.bits.sr_ih = u32Height - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SRIRESO.u32) + u32Data * CHN_OFFSET), DHD0_SRIRESO.u32); 

    return ;
}


HI_VOID  VDP_DISP_SetSrCscEnable  (HI_U32 u32CscId, HI_U32 enCSC)
{   
    U_SR_CSC0_IDC SR_CSC0_IDC;
    U_SR_CSC1_IDC SR_CSC1_IDC;

    if(u32CscId == 0)
    {
        SR_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_IDC.u32)));        
        SR_CSC0_IDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_IDC.u32)), SR_CSC0_IDC.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_IDC.u32)));        
        SR_CSC1_IDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_IDC.u32)), SR_CSC1_IDC.u32);
    }
    else
    {
        HI_PRINT("Error, VDP_DISP_SetSrCscEnable() Select a Wrong CSC id!\n");
    }

}

HI_VOID   VDP_DISP_SetSrCscDcCoef(HI_U32 u32CscId,VDP_CSC_DC_COEF_S stCscCoef)
{   
    U_SR_CSC0_IDC  SR_CSC0_IDC;
    U_SR_CSC0_ODC  SR_CSC0_ODC;
    U_SR_CSC0_IODC SR_CSC0_IODC;

    U_SR_CSC1_IDC  SR_CSC1_IDC;
    U_SR_CSC1_ODC  SR_CSC1_ODC;
    U_SR_CSC1_IODC SR_CSC1_IODC;

    if(u32CscId == 0)
    {
        SR_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_IDC.u32)));
        SR_CSC0_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
        SR_CSC0_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_IDC.u32)), SR_CSC0_IDC.u32);

        SR_CSC0_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_ODC.u32)));
        SR_CSC0_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        SR_CSC0_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_ODC.u32)), SR_CSC0_ODC.u32);

        SR_CSC0_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_IODC.u32)));
        SR_CSC0_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        SR_CSC0_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_IODC.u32)), SR_CSC0_IODC.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_IDC.u32)));
        SR_CSC1_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
        SR_CSC1_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_IDC.u32)), SR_CSC1_IDC.u32);

        SR_CSC1_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_ODC.u32)));
        SR_CSC1_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        SR_CSC1_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_ODC.u32)), SR_CSC1_ODC.u32);

        SR_CSC1_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_IODC.u32)));
        SR_CSC1_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        SR_CSC1_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_IODC.u32)), SR_CSC1_IODC.u32);
    }
    else
    {
        HI_PRINT("Error, VDP_DISP_SetSrCscDcCoef() Select a Wrong CSC id!\n");
    }

}

HI_VOID VDP_DISP_SetSrCscCoef(HI_U32 u32CscId,VDP_CSC_COEF_S stCscCoef)
{   
    U_SR_CSC0_P0        SR_CSC0_P0;
    U_SR_CSC0_P1        SR_CSC0_P1;
    U_SR_CSC0_P2        SR_CSC0_P2;
    U_SR_CSC0_P3        SR_CSC0_P3;
    U_SR_CSC0_P4        SR_CSC0_P4;

    U_SR_CSC1_P0        SR_CSC1_P0;
    U_SR_CSC1_P1        SR_CSC1_P1;
    U_SR_CSC1_P2        SR_CSC1_P2;
    U_SR_CSC1_P3        SR_CSC1_P3;
    U_SR_CSC1_P4        SR_CSC1_P4;


    if(u32CscId == 0)
    {
        SR_CSC0_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_P0.u32)));
        SR_CSC0_P0.bits.cscp00 = stCscCoef.csc_coef00;
        SR_CSC0_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_P0.u32)), SR_CSC0_P0.u32);

        SR_CSC0_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_P1.u32)));
        SR_CSC0_P1.bits.cscp02 = stCscCoef.csc_coef02;
        SR_CSC0_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_P1.u32)), SR_CSC0_P1.u32);

        SR_CSC0_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_P2.u32)));
        SR_CSC0_P2.bits.cscp11 = stCscCoef.csc_coef11;
        SR_CSC0_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_P2.u32)), SR_CSC0_P2.u32);

        SR_CSC0_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_P3.u32)));
        SR_CSC0_P3.bits.cscp20 = stCscCoef.csc_coef20;
        SR_CSC0_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_P3.u32)), SR_CSC0_P3.u32);

        SR_CSC0_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC0_P4.u32)));
        SR_CSC0_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC0_P4.u32)), SR_CSC0_P4.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_P0.u32)));
        SR_CSC1_P0.bits.cscp00 = stCscCoef.csc_coef00;
        SR_CSC1_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_P0.u32)), SR_CSC1_P0.u32);

        SR_CSC1_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_P1.u32)));
        SR_CSC1_P1.bits.cscp02 = stCscCoef.csc_coef02;
        SR_CSC1_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_P1.u32)), SR_CSC1_P1.u32);

        SR_CSC1_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_P2.u32)));
        SR_CSC1_P2.bits.cscp11 = stCscCoef.csc_coef11;
        SR_CSC1_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_P2.u32)), SR_CSC1_P2.u32);

        SR_CSC1_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_P3.u32)));
        SR_CSC1_P3.bits.cscp20 = stCscCoef.csc_coef20;
        SR_CSC1_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_P3.u32)), SR_CSC1_P3.u32);

        SR_CSC1_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CSC1_P4.u32)));
        SR_CSC1_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CSC1_P4.u32)), SR_CSC1_P4.u32);
    }
    else
    {
        HI_PRINT("Error, VDP_DISP_SetSrCscCoef() Select a Wrong CSC id!\n");
    }

}

/*
typedef struct tagVDP_SR_PARA_INFO
{
    VDP_SR_MUX_E        sr_zme_en  ; //normal set as VDP_SR_AND_ZME_EN, 3d set as VDP_SR_ONLY_ZME_EN, other use in Demo Mode
    VDP_SR_SERVE_MODE_E serve_mode ; // H Scaler to 4k 1in2 out, other 1in1out, 2in2out no use
    VDP_SR_SHARP_MODE_E hsh_nodir  ; //don't care
    VDP_PROC_FMT_E      ifmt       ; // FI out 422, other 444
    HI_U32              sr_en      ; // enable
    VDP_SR_SHARP_E      sharp_en   ; //enable
    HI_U32              smooth_en  ; //enable
    HI_U32              plus_en    ; //enable
}VDP_SR_PARA_INFO;

VDP_VP_SR_MODE_E: we can chose one of this

*/

HI_VOID VDP_DISP_SetSrMode(VDP_SR_PARA_INFO SrParaInfo, VDP_VP_SR_MODE_E SrMode)
{
    VDP_SR_PARA_S sr_para;

    sr_para.SrParaInfo.sr_zme_en  = SrParaInfo.sr_zme_en          ; 
    //sr_para.SrParaInfo.serve_mode = SrParaInfo.serve_mode         ; 
    sr_para.SrParaInfo.hsh_nodir  = SrParaInfo.hsh_nodir          ; 
    //sr_para.SrParaInfo.ifmt       = SrParaInfo.ifmt               ; 
    //sr_para.SrParaInfo.sr_en      = SrParaInfo.sr_en              ; 
    sr_para.SrParaInfo.sharp_en   = SrParaInfo.sharp_en           ; 
    sr_para.SrParaInfo.smooth_en  = SrParaInfo.smooth_en          ; 
    sr_para.SrParaInfo.plus_en    = SrParaInfo.plus_en            ; 
    sr_para.hfir_order            = 1; 
    //sr_para.hratio                = IOReso.iw*ZME_HPREC/IOReso.ow ; 
    //sr_para.vratio                = IOReso.ih*ZME_VPREC/IOReso.oh ; 
    sr_para.zme_en                = 1                             ; 
    sr_para.offset                = 0                             ; 
    sr_para.fir_en                = 0                             ; 
    sr_para.mid_en                = 0                             ; 
    //:TODO: Will Implement in B030
    //sr_para.srzme_hladdr          = ADDR_COEF_DHD_SR_HL           ; 
    //sr_para.srzme_hcaddr          = ADDR_COEF_DHD_SR_HC           ; 
    //sr_para.srzme_vladdr          = ADDR_COEF_DHD_SR_VL           ; 
    //sr_para.srzme_vcaddr          = ADDR_COEF_DHD_SR_VC           ; 

    if (SrMode == VDP_VP_SR_TYP_1)
    {
        sr_para.grd_sub_ratio    = 14                  ; 
        sr_para.grd_thrl         = 0                   ; 
        sr_para.grd_thrh         = 4                   ; 
        sr_para.grd_thrh_subl    = 16                  ; 
        sr_para.grd_add_ctrl     = 4                   ; 
        sr_para.disc_detail_gain = 64                  ; 
        sr_para.ensc_detail_gain = 128                 ; 
        sr_para.sharp_coring     = 0                   ; 
        sr_para.sharp_gain       = 300                 ; 
        sr_para.over_thrsh       = 40                  ; 
        sr_para.under_thrsh      = 40                  ; 
        sr_para.plus_ctrl        = 35                  ; 
    }
    else if (SrMode == VDP_VP_SR_TYP_2)
    {
        sr_para.grd_sub_ratio    = 16                  ; 
        sr_para.grd_thrl         = -14                 ; 
        sr_para.grd_thrh         = 512                 ; 
        sr_para.grd_thrh_subl    = 128                 ; 
        sr_para.grd_add_ctrl     = 127                 ; 
        sr_para.disc_detail_gain = 0                   ; 
        sr_para.ensc_detail_gain = 0                   ; 
        sr_para.sharp_coring     = 8                   ; 
        sr_para.sharp_gain       = 512                 ; 
        sr_para.over_thrsh       = 80                  ; 
        sr_para.under_thrsh      = 80                  ; 
        sr_para.plus_ctrl        = 512                 ; 
    }
    else if (SrMode == VDP_VP_SR_TYP_3)
    {
        sr_para.grd_sub_ratio    = 8                   ; 
        sr_para.grd_thrl         = -1000               ; 
        sr_para.grd_thrh         = -500                ; 
        sr_para.grd_thrh_subl    = 100                 ; 
        sr_para.grd_add_ctrl     = 50                  ; 
        sr_para.disc_detail_gain = 1024                ; 
        sr_para.ensc_detail_gain = 400                 ; 
        sr_para.sharp_coring     = 3                   ; 
        sr_para.sharp_gain       = 800                 ; 
        sr_para.over_thrsh       = 128                 ; 
        sr_para.under_thrsh      = 128                 ; 
        sr_para.plus_ctrl        = 350                 ; 
    }
    else if (SrMode == VDP_VP_SR_TYP_4)
    {
        sr_para.grd_sub_ratio    = 1                   ; 
        sr_para.grd_thrl         = 50                  ; 
        sr_para.grd_thrh         = 360                 ; 
        sr_para.grd_thrh_subl    = 10                  ; 
        sr_para.grd_add_ctrl     = 90                  ; 
        sr_para.disc_detail_gain = 640                 ; 
        sr_para.ensc_detail_gain = 1280                ; 
        sr_para.sharp_coring     = 14                  ; 
        sr_para.sharp_gain       = 678                 ; 
        sr_para.over_thrsh       = 123                 ; 
        sr_para.under_thrsh      = 123                 ; 
        sr_para.plus_ctrl        = 358                 ; 
    }
    else if(SrMode == VDP_VP_SR_MAX)
    {
        sr_para.grd_sub_ratio    = 32                  ; 
        sr_para.grd_thrl         = 1023                ; 
        sr_para.grd_thrh         = 1023                ; 
        sr_para.grd_thrh_subl    = 255                 ; 
        sr_para.grd_add_ctrl     = 255                 ; 
        sr_para.disc_detail_gain = 4095                ; 
        sr_para.ensc_detail_gain = 4095                ; 
        sr_para.sharp_coring     = 15                  ; 
        sr_para.sharp_gain       = 1023                ; 
        sr_para.over_thrsh       = 255                 ; 
        sr_para.under_thrsh      = 255                 ; 
        sr_para.plus_ctrl        = 1023                ; 
    }
    else if(SrMode == VDP_VP_SR_MIN)
    {
        sr_para.grd_sub_ratio    = 0                   ; 
        sr_para.grd_thrl         = -1023               ; 
        sr_para.grd_thrh         = -1023               ; 
        sr_para.grd_thrh_subl    = 0                   ; 
        sr_para.grd_add_ctrl     = 0                   ; 
        sr_para.disc_detail_gain = 0                   ; 
        sr_para.ensc_detail_gain = 0                   ; 
        sr_para.sharp_coring     = 0                   ; 
        sr_para.sharp_gain       = 0                   ; 
        sr_para.over_thrsh       = 0                   ; 
        sr_para.under_thrsh      = 0                   ; 
        sr_para.plus_ctrl        = 0                   ; 
    }
    else if(SrMode == VDP_VP_SR_RAND)
    {
        #if 1
        sr_para.grd_sub_ratio    = 0                   ; 
        sr_para.grd_thrl         = -1023               ; 
        sr_para.grd_thrh         = -1023               ; 
        sr_para.grd_thrh_subl    = 0                   ; 
        sr_para.grd_add_ctrl     = 0                   ; 
        sr_para.disc_detail_gain = 0                   ; 
        sr_para.ensc_detail_gain = 0                   ; 
        sr_para.sharp_coring     = 0                   ; 
        sr_para.sharp_gain       = 0                   ; 
        sr_para.over_thrsh       = 0                   ; 
        sr_para.under_thrsh      = 0                   ; 
        sr_para.plus_ctrl        = 0                   ; 
        #else
        sr_para.grd_sub_ratio    = rand()%33           ; 
        sr_para.grd_thrl         = -1023 + rand()%2047 ; 
        sr_para.grd_thrh         = -1023 + rand()%2047 ; 
        sr_para.grd_thrh_subl    = rand()%256          ; 
        sr_para.grd_add_ctrl     = rand()%256          ; 
        sr_para.disc_detail_gain = rand()%4096         ; 
        sr_para.ensc_detail_gain = rand()%4096         ; 
        sr_para.sharp_coring     = rand()%16           ; 
        sr_para.sharp_gain       = rand()%1024         ; 
        sr_para.over_thrsh       = rand()%256          ; 
        sr_para.under_thrsh      = rand()%256          ; 
        sr_para.plus_ctrl        = rand()%1024         ; 
        #endif
    }
    else
    {
        sr_para.grd_sub_ratio    = 0                   ; 
        sr_para.grd_thrl         = -1023               ; 
        sr_para.grd_thrh         = -1023               ; 
        sr_para.grd_thrh_subl    = 0                   ; 
        sr_para.grd_add_ctrl     = 0                   ; 
        sr_para.disc_detail_gain = 0                   ; 
        sr_para.ensc_detail_gain = 0                   ; 
        sr_para.sharp_coring     = 0                   ; 
        sr_para.sharp_gain       = 0                   ; 
        sr_para.over_thrsh       = 0                   ; 
        sr_para.under_thrsh      = 0                   ; 
        sr_para.plus_ctrl        = 0                   ; 
    }
    VDP_DISP_SetSrMux              ( VDP_CHN_DHD0, sr_para.SrParaInfo.sr_zme_en);//VDP_SR_AND_ZME_EN         ; VDP_SR_AND_ZME_EN_L      ; VDP_SR_AND_ZME_EN_R ; VDP_SR_ONLY_ZME_EN ;
    //VDP_DISP_SetSrServeMode        ( VDP_CHN_DHD0, sr_para.SrParaInfo.serve_mode) ; //VDP_SR_2IN_2OUT           ; VDP_SR_1IN_2OUT          ; VDP_SR_1IN_1OUT     ;
    VDP_DISP_SetSrSharpenMode      ( VDP_CHN_DHD0, sr_para.SrParaInfo.hsh_nodir) ; //VDP_SR_SHARP_MODE_DETAIL  ; VDP_SR_SHARP_MODE_TRAD   ;
    //VDP_DISP_SetSrZmeInFmt         ( VDP_CHN_DHD0, sr_para.SrParaInfo.ifmt) ; //VDP_PROC_FMT_SP_444       ; VDP_PROC_FMT_SP_422      ;
    //VDP_DISP_SetSrEnable           ( VDP_CHN_DHD0, sr_para.SrParaInfo.sr_en) ;
    VDP_DISP_SetSrSmoothEnable     ( VDP_CHN_DHD0, sr_para.SrParaInfo.smooth_en) ;
    VDP_DISP_SetSrSmoothPlusEnable ( VDP_CHN_DHD0, sr_para.SrParaInfo.plus_en) ;
    VDP_DISP_SetSrSharpenEnable    ( VDP_CHN_DHD0, sr_para.SrParaInfo.sharp_en) ;
       
    //VDP_DISP_SetSrZmeOutFmt        ( VDP_CHN_DHD0, VDP_PROC_FMT_SP_444) ;
    VDP_DISP_SetSrGrdSubRatio      ( VDP_CHN_DHD0, sr_para.grd_sub_ratio) ;
    VDP_DISP_SetSrGrdThr           ( VDP_CHN_DHD0, sr_para.grd_thrl, sr_para.grd_thrh) ;
    VDP_DISP_SetSrGrdThrSub        ( VDP_CHN_DHD0, sr_para.grd_thrh_subl) ;
    VDP_DISP_SetSrLtexAdd          ( VDP_CHN_DHD0, sr_para.grd_add_ctrl) ;
    VDP_DISP_SetSrDiscDetailGain   ( VDP_CHN_DHD0, sr_para.disc_detail_gain) ;
    VDP_DISP_SetSrEiscDetailGain   ( VDP_CHN_DHD0, sr_para.ensc_detail_gain) ;
    VDP_DISP_SetSrSharpenCoring    ( VDP_CHN_DHD0, sr_para.sharp_coring) ;
    VDP_DISP_SetSrSharpenGain      ( VDP_CHN_DHD0, sr_para.sharp_gain) ;
    VDP_DISP_SetSrSharpenThrsh     ( VDP_CHN_DHD0, sr_para.over_thrsh, sr_para.under_thrsh) ;
    VDP_DISP_SetSrSmoothPlusCtrl   ( VDP_CHN_DHD0, sr_para.plus_ctrl) ;
    VDP_DISP_SetSrZmeEnable        ( VDP_CHN_DHD0, VDP_ZME_MODE_ALL, sr_para.zme_en) ;
    //VDP_DISP_SetSrZmePhase         ( VDP_CHN_DHD0, VDP_ZME_MODE_ALL, sr_para.offset) ;
    VDP_DISP_SetSrZmeFirEnable     ( VDP_CHN_DHD0, VDP_ZME_MODE_ALL, sr_para.fir_en) ;
    //VDP_DISP_SetSrZmeMidEnable     ( VDP_CHN_DHD0, VDP_ZME_MODE_ALL, sr_para.mid_en) ;
    VDP_DISP_SetSrHfirOrder        ( VDP_CHN_DHD0, sr_para.hfir_order) ;

}

HI_VOID VDP_DISP_SetHimaxIpSel(VDP_IP_SEL_E enConnect)
{
    U_IPSEL IPSEL;
    
    IPSEL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->IPSEL.u32)));
    IPSEL.bits.ip_2dto3d_sel = enConnect;
    VDP_RegWrite((HI_U32)(&(pVdpReg->IPSEL.u32)), IPSEL.u32);
}
