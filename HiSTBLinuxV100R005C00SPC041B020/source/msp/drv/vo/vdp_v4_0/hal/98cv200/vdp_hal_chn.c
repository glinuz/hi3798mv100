//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_hal_chn.cpp
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


#include "vdp_define.h"
#include "vdp_ip_define.h"
#include "vdp_hal_comm.h"
#include "vdp_hal_chn.h"

#if VDP_CBB_FPGA
#include "vdp_fpga_define.h"
//#include "sv_unf_osal.h"
#if FPGA_AUTORUN
#include "vdp_util.h"
#endif
//#include <linux/fcntl.h>
#endif



extern S_VDP_REGS_TYPE *pVdpReg;

HI_U32 gu32FrmCnt = 0;
HI_U32 gu32FrmCnt1 = 1;
extern char cOfName[256];	
extern char cIfName[256];
extern char cIfNamehdr[256];
extern struct file * fpCfgHdrOut;

#if VDP_CBB_FPGA
extern HI_U32 gu32SumOut[MAX_SUM][256];
//extern HI_U32 gu32_ret[MAX_CHK_CHN][MAX_CHK_FRM];
#endif
extern HI_U32  *gu32_ret;


HI_VOID VDP_DISP_SetUfOffineEn  (HI_U32 u32hd_id, HI_U32 u32UfOfflineEn)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error, VDP_DISP_SetUfOffineEn Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + u32hd_id*CHN_OFFSET);
    DHD0_CTRL.bits.uf_offline_en = u32UfOfflineEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetDispMode  (HI_U32 u32hd_id, HI_U32 u32DispMode)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetFramePackingEn Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.disp_mode = u32DispMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetHdmiMode  (HI_U32 u32hd_id, HI_U32 u32hdmi_md)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHdmiMode Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.hdmi_mode = u32hdmi_md;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetHdmi420Enable  (HI_U32 u32hd_id, HI_U32 u32Enable)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHdmi420Enable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.hdmi420_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetHdmi420CSel  (HI_U32 u32hd_id, HI_U32 u32Csel)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHdmi420CSel() Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.hdmi420c_sel = u32Csel;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetRegUp (HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetIntfEnable(HI_U32 u32hd_id, HI_U32 bTrue)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.intf_en = bTrue;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);

    VDP_DISP_SetRegUp(u32hd_id);
}

HI_VOID VDP_DISP_SetIntMask  (HI_U32 u32masktypeen)
{
    U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 | u32masktypeen;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOINTMSK.u32)), VOINTMSK.u32); 

    return ;
}

HI_VOID VDP_DISP_ClearIntMask  (HI_U32 u32masktypeen)
{
    U_VOINTMSK  VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = u32masktypeen;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOINTMSK.u32)), VOINTMSK.u32); 
    return ;
}

HI_VOID  VDP_DISP_SetIntDisable(HI_U32 u32masktypeen)
{
    U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 & (~u32masktypeen);
    VDP_RegWrite((HI_ULONG)&(pVdpReg->VOINTMSK.u32), VOINTMSK.u32);

    return ;
}

HI_VOID VDP_DISP_SetOflIntMask  (HI_U32 u32masktypeen)
{
    U_VOINTMSK_OFL VOINTMSK_OFL;

    /* Dispaly interrupt mask enable */
    VOINTMSK_OFL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTMSK_OFL.u32)));
    VOINTMSK_OFL.u32 = VOINTMSK_OFL.u32 | u32masktypeen;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOINTMSK_OFL.u32)), VOINTMSK_OFL.u32); 

    return ;
}

HI_VOID  VDP_DISP_SetOflIntDisable(HI_U32 u32masktypeen)
{
    U_VOINTMSK_OFL VOINTMSK_OFL;

    /* Dispaly interrupt mask enable */
    VOINTMSK_OFL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTMSK_OFL.u32)));
    VOINTMSK_OFL.u32 = VOINTMSK_OFL.u32 & (~u32masktypeen);
    VDP_RegWrite((HI_ULONG)&(pVdpReg->VOINTMSK_OFL.u32), VOINTMSK_OFL.u32);

    return ;
}

HI_VOID  VDP_DISP_BfmClkSel(HI_U32 u32Num)
{
    U_VODEBUG  VODEBUG ;
    /* Dispaly interrupt mask enable */
    VODEBUG.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->VODEBUG.u32)));
    VODEBUG.bits.bfm_clk_sel = u32Num;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->VODEBUG.u32), VODEBUG.u32);
    return ;
}

HI_VOID  VDP_DISP_SetTrigger(HI_U32 u32hd_id, HI_BOOL bEnable)
{
	U_DHD0_CTRL DHD0_CTRL;
	
	if (u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetTrigger Select Wrong CHANNEL ID\n");
        return ;
    }
	
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.trigger_en   = bEnable; 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);

	return;
}

HI_U32 VDP_DISP_GetDispMode(HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;
    DHD0_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + u32hd_id*CHN_OFFSET);

    return DHD0_CTRL.bits.iop;
}


HI_VOID  VDP_DISP_OpenIntf(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S stSyncInfo)
{
    U_DHD0_CTRL DHD0_CTRL;
    U_DHD0_VSYNC DHD0_VSYNC;
    U_DHD0_VPLUS DHD0_VPLUS;
    U_DHD0_PWR DHD0_PWR;
    U_DHD0_HSYNC1 DHD0_HSYNC1;
    U_DHD0_HSYNC2 DHD0_HSYNC2;
	HI_U32 u32TmpRation=1;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_OpenIntf Select Wrong CHANNEL ID\n");
        return ;
    }


    /*
    //VOU VHD CHANNEL enable 
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.iop   = stSyncInfo.bIop;// 
    //DHD0_CTRL.bits.intfb = //stSyncInfo.u32Intfb; in intf
    //DHD0_CTRL.bits.synm  = //stSyncInfo.bSynm;
    DHD0_CTRL.bits.idv   = stSyncInfo.bIdv;//DATA INV
    DHD0_CTRL.bits.ihs   = stSyncInfo.bIhs;// H PULSE INV
    DHD0_CTRL.bits.ivs   = stSyncInfo.bIvs;//V PULSE INV
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
     */
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.iop   = stSyncInfo.bIop;// 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);

	if(((u32hd_id == 0)&&(stSyncInfo.bIop	== 0)&&(stSyncInfo.u32Hact == 720 )&&(stSyncInfo.u32Vact== 288)))
	{
		u32TmpRation =2;
		stSyncInfo.u32Hmid = 600;
	}
	else if(((u32hd_id == 0)&&(stSyncInfo.bIop  == 0)&&(stSyncInfo.u32Hact == 720 )&&(stSyncInfo.u32Vact== 240)))
	{
		u32TmpRation =2;
		stSyncInfo.u32Hmid = 620;
	}
	else
	{
		u32TmpRation =1;
	}

    DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC1.bits.hact = stSyncInfo.u32Hact*u32TmpRation -1;
    DHD0_HSYNC1.bits.hbb  = stSyncInfo.u32Hbb*u32TmpRation -1;
    DHD0_HSYNC2.bits.hfb  = stSyncInfo.u32Hfb*u32TmpRation -1;

	// add for pal ntsc  timing ; dhd work on 27MHz pixclk
	if (u32hd_id == 0)
	{
		DHD0_HSYNC2.bits.hmid = (stSyncInfo.u32Hmid <= 1) ? 0 : (stSyncInfo.u32Hmid / 2 - 1);
	}
	else
	{
		DHD0_HSYNC2.bits.hmid = (stSyncInfo.u32Hmid <= 1) ? 0 : (stSyncInfo.u32Hmid - 1);
	}

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC2.u32);

    //Config VHD interface veritical timming
    DHD0_VSYNC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VSYNC.bits.vact = stSyncInfo.u32Vact  -1;
    DHD0_VSYNC.bits.vbb = stSyncInfo.u32Vbb - 1;
    DHD0_VSYNC.bits.vfb =  stSyncInfo.u32Vfb - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET), DHD0_VSYNC.u32);

    //Config VHD interface veritical bottom timming,no use in progressive mode
    DHD0_VPLUS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VPLUS.bits.bvact = stSyncInfo.u32Bvact - 1;
    DHD0_VPLUS.bits.bvbb =  stSyncInfo.u32Bvbb - 1;
    DHD0_VPLUS.bits.bvfb =  stSyncInfo.u32Bvfb - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET), DHD0_VPLUS.u32);

    //Config VHD interface veritical bottom timming, 
    DHD0_PWR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET));        
    DHD0_PWR.bits.hpw = stSyncInfo.u32Hpw*u32TmpRation - 1;
    DHD0_PWR.bits.vpw = stSyncInfo.u32Vpw - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET), DHD0_PWR.u32);
}

HI_VOID  VDP_DISP_OpenTypIntf(HI_U32 u32hd_id, VDP_DISP_DIGFMT_E enDigFmt)
{   
    VDP_DISP_SYNCINFO_S  stSyncInfo={0};

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_OpenIntf Select Wrong CHANNEL ID\n");
        return ;
    }

    switch(enDigFmt)
    {
        case VDP_DISP_DIGFMT_PAL:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = 288;
            stSyncInfo.u32Vfb   = 2;
            stSyncInfo.u32Vbb   = 22;

            stSyncInfo.u32Hact  = 720;
            stSyncInfo.u32Hfb   = 12;
            stSyncInfo.u32Hbb   = 132;

            stSyncInfo.u32Bvact = 288;
            stSyncInfo.u32Bvfb  = 2;
#if EDA_TEST
            stSyncInfo.u32Bvbb  = 22;
#else
            stSyncInfo.u32Bvbb  = 23;
#endif

            stSyncInfo.u32Hpw   = 63;
            stSyncInfo.u32Vpw   = 3;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_NTSC:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = 240;
            stSyncInfo.u32Vfb   = 4;
            stSyncInfo.u32Vbb   = 18;

            stSyncInfo.u32Hact  = 720;
            stSyncInfo.u32Hfb   = 19;
            stSyncInfo.u32Hbb   = 119;

            stSyncInfo.u32Bvact = 240;
            stSyncInfo.u32Bvfb  = 4;
#if EDA_TEST
            stSyncInfo.u32Bvbb  = 18;
#else
            stSyncInfo.u32Bvbb  = 19;
#endif

            stSyncInfo.u32Hpw   = 62;
            stSyncInfo.u32Vpw   = 3;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_1080P50:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M
#if 1
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_1080P;
            stSyncInfo.u32Vfb   = 4;
            stSyncInfo.u32Vbb   = 41;

            stSyncInfo.u32Hact  = W_1080P;
            stSyncInfo.u32Hfb   = 528;
            stSyncInfo.u32Hbb   = 192;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 5;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;
#else

            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_1080P;
            stSyncInfo.u32Vfb   = 10;
            stSyncInfo.u32Vbb   = 160;

            stSyncInfo.u32Hact  = W_1080P;
            stSyncInfo.u32Hfb   = 147;
            stSyncInfo.u32Hbb   = 309;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 5;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

#endif
            break;


        }
		case VDP_DISP_DIGFMT_1080P_24:
		{
		   
		   /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
		   /* Synm Iop	Itf  Vact Vbb Vfb  Hact  Hbb Hfb  Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
		   //{ {1, 1,  2,  1080, 41,  4,  1920, 192, 638,	 1, 1, 1,	44, 5, 1, 0, 0, 0}, /* 1080P@24Hz */
		  // printk("@@@@@@@@@  VDP_DISP_DIGFMT_1080P_24 !!\n");
		   //1080p@25Hz SMPTE 274M
		   //1080p@50Hz SMPTE 274M
		   stSyncInfo.bIop = 1;
		   stSyncInfo.u32Intfb = 2;
		   stSyncInfo.bSynm = 1;
		   
           stSyncInfo.u32Vact = 1080;
           stSyncInfo.u32Vfb  = 4;
           stSyncInfo.u32Vbb  = 41;

		


		  stSyncInfo.u32Hact = 1920;
		  stSyncInfo.u32Hfb  = 638;
		  stSyncInfo.u32Hbb  = 192;
		
		   stSyncInfo.u32Bvact = 0;
		   stSyncInfo.u32Bvfb = 0;
		   stSyncInfo.u32Bvbb = 0;
		
		   stSyncInfo.u32Hpw = 44;
		   stSyncInfo.u32Vpw = 5;
		   
		   stSyncInfo.bIdv = 0;
		   stSyncInfo.bIhs = 0;
		   stSyncInfo.bIvs = 0;
		
		   stSyncInfo.u32Hmid = 0;
		
		   break;
		
		  }
        case VDP_DISP_DIGFMT_1080P_24_FP:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M
            
            stSyncInfo.bIop = 1;
            //stSyncInfo.u32Intfb = 1;
            //stSyncInfo.bSynm = 0;
            
            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 1080;
            stSyncInfo.u32Vfb  = 4;
            stSyncInfo.u32Vbb  = 41;

            stSyncInfo.u32Hact = 1920;
            stSyncInfo.u32Hfb  = 638;
            stSyncInfo.u32Hbb  = 192;

            stSyncInfo.u32Bvact =0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  =0;

            stSyncInfo.u32Hpw = 44;
            stSyncInfo.u32Vpw = 5;
			
            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Hmid = 1;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;        

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1080P60:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M

            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_1080P;
            stSyncInfo.u32Vfb   = 4;
            stSyncInfo.u32Vbb   = 41;

            stSyncInfo.u32Hact  = W_1080P;
            stSyncInfo.u32Hfb   = 88;
            stSyncInfo.u32Hbb   = 192;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 5;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_1080P30:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M
            
            stSyncInfo.bIop = 1;
            
            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 1080;
            stSyncInfo.u32Vfb  = 4;
            stSyncInfo.u32Vbb  = 41;

            stSyncInfo.u32Hact = 1920;
            stSyncInfo.u32Hfb  = 88;
            stSyncInfo.u32Hbb  = 192;
            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw = 0x0;
            stSyncInfo.u32Vpw = 0x0;
            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Hmid = 0x0;
            break;
        }
        case VDP_DISP_DIGFMT_1080I50:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = 1080/2;
            stSyncInfo.u32Vfb   = 2;
            stSyncInfo.u32Vbb   = 20;

            stSyncInfo.u32Hact  = 1920;
            stSyncInfo.u32Hfb   = 528;
            stSyncInfo.u32Hbb   = 192;

            stSyncInfo.u32Bvact = 1080/2;
            stSyncInfo.u32Bvfb  = 2;
#if EDA_TEST
            stSyncInfo.u32Bvbb  = 20;
#else
            stSyncInfo.u32Bvbb  = 21;
#endif

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 5;

            stSyncInfo.u32Hmid  = 960;

            break;

        }
        case VDP_DISP_DIGFMT_1080I60:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_1080P/2;
            stSyncInfo.u32Vfb   = 2;
            stSyncInfo.u32Vbb   = 20;

            stSyncInfo.u32Hact  = W_1080P;
            stSyncInfo.u32Hfb   = 88;
            stSyncInfo.u32Hbb   = 192;

            stSyncInfo.u32Bvact = H_1080P/2;
            stSyncInfo.u32Bvfb  = 2;
            stSyncInfo.u32Bvbb  = 20;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 5;

            stSyncInfo.u32Hmid  = 960;

            break;

        }
        case VDP_DISP_DIGFMT_720P50:
        {

            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_720P;
            stSyncInfo.u32Vfb   = 5;
            stSyncInfo.u32Vbb   = 25;

            stSyncInfo.u32Hact  = W_720P;
            stSyncInfo.u32Hfb   = 440;
            stSyncInfo.u32Hbb   = 260;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 40;
            stSyncInfo.u32Vpw   = 5;

            stSyncInfo.u32Hmid  = 0;

            break;
        }
         case VDP_DISP_DIGFMT_720P60:
        {

            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_720P;
            stSyncInfo.u32Vfb   = 5;
            stSyncInfo.u32Vbb   = 25;

            stSyncInfo.u32Hact  = W_720P;
            stSyncInfo.u32Hfb   = 110;
            stSyncInfo.u32Hbb   = 260;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 40;
            stSyncInfo.u32Vpw   = 5;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_800x600:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = 600;
            stSyncInfo.u32Vfb   = 1;
            stSyncInfo.u32Vbb   = 27;

            stSyncInfo.u32Hact  = 800;
            stSyncInfo.u32Hfb   = 40;
            stSyncInfo.u32Hbb   = 216;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 0x0;
            stSyncInfo.u32Vpw   = 0x0;

            stSyncInfo.u32Hmid  = 0x0;

            break;

        }
        case VDP_DISP_DIGFMT_576P:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = 576;
            stSyncInfo.u32Vfb   = 5;
            stSyncInfo.u32Vbb   = 44;

            stSyncInfo.u32Hact  = 720;
            stSyncInfo.u32Hfb   = 12;
            stSyncInfo.u32Hbb   = 132;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 64;
            stSyncInfo.u32Vpw   = 5;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_576I:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_576P/2;
            stSyncInfo.u32Vfb   = 2;
            stSyncInfo.u32Vbb   = 22;

            stSyncInfo.u32Hact  = W_576P;
            stSyncInfo.u32Hfb   = 12;
            stSyncInfo.u32Hbb   = 132;

            stSyncInfo.u32Bvact = H_576P/2;
            stSyncInfo.u32Bvfb  = 2;
#if EDA_TEST
            stSyncInfo.u32Bvbb  = 22;
#else
            stSyncInfo.u32Bvbb  = 23;
#endif

            stSyncInfo.u32Hpw   = 63;
            stSyncInfo.u32Vpw   = 3;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_480P://@60
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            //stSyncInfo.u32Vact  = 480;
            //stSyncInfo.u32Vfb   = 9;
            //stSyncInfo.u32Vbb   = 36;
			
            stSyncInfo.u32Vact  = 480;
            stSyncInfo.u32Vfb   = 200;
            stSyncInfo.u32Vbb   = 200;
			
            stSyncInfo.u32Hact  = 720;
           stSyncInfo.u32Hfb   = 700;
           stSyncInfo.u32Hbb   = 500;

            //stSyncInfo.u32Hact  = 720;
            // stSyncInfo.u32Hfb   = 16;
            // stSyncInfo.u32Hbb   = 122;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 62;
            stSyncInfo.u32Vpw   = 6;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_480I:
        {

            stSyncInfo.bIop     = 0;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_D1/2;
            stSyncInfo.u32Vfb   = 4;
            stSyncInfo.u32Vbb   = 18;

            stSyncInfo.u32Hact  = W_D1;
            stSyncInfo.u32Hfb   = 20;
            stSyncInfo.u32Hbb   = 118;

            stSyncInfo.u32Bvact = H_D1/2;
            stSyncInfo.u32Bvfb  = 4;
#if EDA_TEST
            stSyncInfo.u32Bvbb  = 18;
#else
            stSyncInfo.u32Bvbb  = 19;
#endif

            stSyncInfo.u32Hpw   = 62;
            stSyncInfo.u32Vpw   = 3;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_1600P60:
        {
            stSyncInfo.bSynm    = 1;
            stSyncInfo.bIop     = 1;
            stSyncInfo.u32Intfb = 2;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_1600P;
            stSyncInfo.u32Vfb   = 3;
            stSyncInfo.u32Vbb   = 37;

            stSyncInfo.u32Hact  = W_1600P;
            stSyncInfo.u32Hfb   = 80;
            stSyncInfo.u32Hbb   = 48;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 32;
            stSyncInfo.u32Vpw   = 6;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_2160P60:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_2160P;
            stSyncInfo.u32Vfb   = 4*2; 
            stSyncInfo.u32Vbb   = 41*2; 

            stSyncInfo.u32Hact  = W_2160P;
            stSyncInfo.u32Hfb   = 88*2; 
            stSyncInfo.u32Hbb   = 192*2; 

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 8;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_2160P30:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_2160P;
            stSyncInfo.u32Vfb   = 4*2;
            stSyncInfo.u32Vbb   = 41*2;

            stSyncInfo.u32Hact  = W_2160P;
            stSyncInfo.u32Hfb   = 88*2;
            stSyncInfo.u32Hbb   = 192*2;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 8;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_4096x2160P60:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_2160P;
            stSyncInfo.u32Vfb   = 4*2;
            stSyncInfo.u32Vbb   = 41*2;

            stSyncInfo.u32Hact  = W_4096;
            stSyncInfo.u32Hfb   = 100;
            stSyncInfo.u32Hbb   = 204;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 8;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_2160P120:
        {
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_2160P/2;
            stSyncInfo.u32Vfb   = 4;
            stSyncInfo.u32Vbb   = 41;

            stSyncInfo.u32Hact  = W_2160P;
            stSyncInfo.u32Hfb   = 88*2;
            stSyncInfo.u32Hbb   = 192*2;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 8;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        case VDP_DISP_DIGFMT_TESTI:
        {
            VDP_PRINT("Now is TestMode I\n");

            stSyncInfo.bIop = 0;
            //stSyncInfo.u32Intfb = 1;
            //stSyncInfo.bSynm = 0;

            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 64;
            stSyncInfo.u32Vfb  = 6;
            stSyncInfo.u32Vbb  = 5;

            stSyncInfo.u32Hact = 720;
            stSyncInfo.u32Hfb  = 20;
            stSyncInfo.u32Hbb  = 10;

            stSyncInfo.u32Bvact = 64;
            stSyncInfo.u32Bvfb  = 6;
            stSyncInfo.u32Bvbb  = 5;

            stSyncInfo.u32Hpw = 0x0;
            stSyncInfo.u32Vpw = 0x0;

            stSyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;        

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_TESTP:
        {
            VDP_PRINT("Now is TestMode P\n");

            stSyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 2;
            //1stSyncInfo.bSynm = 0;

            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 128;
            stSyncInfo.u32Vfb  = 10;
            stSyncInfo.u32Vbb  = 10;

            stSyncInfo.u32Hact = 720;
            stSyncInfo.u32Hfb  = 16;
            stSyncInfo.u32Hbb  = 16;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw = 0x0;
            stSyncInfo.u32Vpw = 0x0;

            stSyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR444;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCBCR_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_TESTS:
        {
            VDP_PRINT("Now is TestMode S\n");

            stSyncInfo.bIop = 0;

            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 64;
            stSyncInfo.u32Vfb  = 6;
            stSyncInfo.u32Vbb  = 2;

            stSyncInfo.u32Hact = 720;
            stSyncInfo.u32Hfb  = 20;
            stSyncInfo.u32Hbb  = 10;

            stSyncInfo.u32Bvact = 64;
            stSyncInfo.u32Bvfb  = 6;
            stSyncInfo.u32Bvbb  = 2;

            stSyncInfo.u32Hpw = 0x0;
            stSyncInfo.u32Vpw = 0x0;

            stSyncInfo.u32Hmid = 0x0;

            break;

        }
        case VDP_DISP_DIGFMT_TESTUT:
        {
            VDP_PRINT("Now is TestMode UT\n");

            stSyncInfo.bIop = 1;

            stSyncInfo.bIdv = 0;
            stSyncInfo.bIhs = 0;
            stSyncInfo.bIvs = 0;

            stSyncInfo.u32Vact = 2048;
            stSyncInfo.u32Vfb  = 1;
            stSyncInfo.u32Vbb  = 1;

            stSyncInfo.u32Hact = 64;
            stSyncInfo.u32Hfb  = 8;
            stSyncInfo.u32Hbb  = 8;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw = 0x0;
            stSyncInfo.u32Vpw = 0x0;

            stSyncInfo.u32Hmid = 0x0;

            break;

        }
        case VDP_DISP_DIGFMT_2160P60_HBI:
        {
            VDP_PRINT("VDP_DISP_DIGFMT_2160P60_HBI\n");
            stSyncInfo.bIop     = 1;

            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Vact  = H_2160P;
            stSyncInfo.u32Vfb   = 4*2;
            stSyncInfo.u32Vbb   = 41*2;

            stSyncInfo.u32Hact  = W_2160P;
            stSyncInfo.u32Hfb   = 260*2;
            stSyncInfo.u32Hbb   = 40*2;

            stSyncInfo.u32Bvact = 0;
            stSyncInfo.u32Bvfb  = 0;
            stSyncInfo.u32Bvbb  = 0;

            stSyncInfo.u32Hpw   = 44;
            stSyncInfo.u32Vpw   = 8;
            stSyncInfo.bIdv     = 0;
            stSyncInfo.bIhs     = 0;
            stSyncInfo.bIvs     = 0;

            stSyncInfo.u32Hmid  = 0;

            break;

        }
        default:
        {
            VDP_PRINT("Error,VDP_DISP_OpenIntf() Wrong Default DIG FMT mode\n");
            return ;
        }
    }

    if(stSyncInfo.u32Hact <= 0)
    {
        stSyncInfo.u32Hact = 1;
    }
    if(stSyncInfo.u32Hbb <=0)
    {
        stSyncInfo.u32Hbb = 1;
    }
    if(stSyncInfo.u32Hfb <=0)
    {
        stSyncInfo.u32Hfb = 1;
    }

    if(stSyncInfo.u32Vact <= 0)
    {
        stSyncInfo.u32Vact = 1;
    }
    if(stSyncInfo.u32Vbb <=0)
    {
        stSyncInfo.u32Vbb = 1;
    }
    if(stSyncInfo.u32Vfb <=0)
    {
        stSyncInfo.u32Vfb = 1;
    }

    if(stSyncInfo.u32Bvact <= 0)
    {
        stSyncInfo.u32Bvact = 1;
    }
    if(stSyncInfo.u32Bvbb <=0)
    {
        stSyncInfo.u32Bvbb = 1;
    }
    if(stSyncInfo.u32Bvfb <=0)
    {
        stSyncInfo.u32Bvfb = 1;
    }

    if(stSyncInfo.u32Hpw <=0)
    {
        stSyncInfo.u32Hpw = 1;
    }
    if(stSyncInfo.u32Vpw <=0)
    {
        stSyncInfo.u32Vpw = 1;
    }

    // VDP_DISP_SetVSync(enChan,stSyncInfo);
    // VDP_DISP_SetHSync(enChan,stSyncInfo);
    // VDP_DISP_SetPlusWidth(enChan,stSyncInfo);
    // VDP_DISP_SetPlusPhase(enChan,stSyncInfo);
    // VDP_DISP_SetOutFmt(enChan,stIntfFmt);
    // VDP_DISP_SetVTThdMode(enChan, bool(!(stSyncInfo.bIop)));
    // VDP_DISP_SetIntfMode(enChan,stSyncInfo);
    VDP_DISP_OpenIntf(u32hd_id,stSyncInfo);

    return ;

}

#if 0
//mod me
HI_VOID VDP_DISP_SetVSync(HI_U32 u32hd_id, HI_U32 u32vfb, HI_U32 u32vbb, HI_U32 u32vact)
{
    U_DHD0_VSYNC DHD0_VSYNC;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVSync Select Wrong CHANNEL ID\n");
        return ;
    }
    //Config VHD interface veritical timming
    DHD0_VSYNC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VSYNC.bits.vact = u32vact;
    DHD0_VSYNC.bits.vbb = u32vbb;
    DHD0_VSYNC.bits.vfb = u32vfb;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET), DHD0_VSYNC.u32);
}

HI_VOID VDP_DISP_SetVSyncPlus(HI_U32 u32hd_id, HI_U32 u32bvfb, HI_U32 u32bvbb, HI_U32 u32vact)
{

    U_DHD0_VPLUS DHD0_VPLUS;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVSyncPlus Select Wrong CHANNEL ID\n");
        return ;
    }

    //Config VHD interface veritical bottom timming,no use in progressive mode
    DHD0_VPLUS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VPLUS.bits.bvact = u32vact;
    DHD0_VPLUS.bits.bvbb = u32bvbb;
    DHD0_VPLUS.bits.bvfb = u32bvfb;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET), DHD0_VPLUS.u32);


}

HI_VOID VDP_DISP_SetHSync(HI_U32 u32hd_id, HI_U32 u32hfb, HI_U32 u32hbb, HI_U32 u32hact)
{


    U_DHD0_HSYNC1 DHD0_HSYNC1;
    U_DHD0_HSYNC2 DHD0_HSYNC2;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHSync Select Wrong CHANNEL ID\n");
        return ;
    }

    //Config VHD interface horizontal timming
    DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC1.bits.hact = u32hact;
    DHD0_HSYNC1.bits.hbb = u32hbb;
    DHD0_HSYNC2.bits.hfb = u32hfb;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC2.u32);

}

HI_VOID  VDP_DISP_SetPlusWidth(HI_U32 u32hd_id, HI_U32 u32hpw, HI_U32 u32vpw)
{
    U_DHD0_PWR DHD0_PWR;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetPlusWidth Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_PWR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET));
    DHD0_PWR.bits.hpw = u32hpw;
    DHD0_PWR.bits.vpw = u32vpw;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET), DHD0_PWR.u32);
}

HI_VOID VDP_DISP_SetPlusPhase(HI_U32 u32hd_id, HI_U32 u32ihs, HI_U32 u32ivs, HI_U32 u32idv)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetPlusPhase Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.ihs = u32ihs;
    DHD0_CTRL.bits.ivs = u32ivs;
    DHD0_CTRL.bits.idv = u32idv;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}
#endif

#if 1
HI_VOID VDP_DISP_SetSyncInv(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DISP_SYNCINV_S enInv)
{
    U_DHD0_SYNC_INV DHD0_SYNC_INV;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSync_INV Select Wrong CHANNEL ID\n");
        return ;
    }
    
    switch(enIntf)
    {
        //case VDP_DISP_INTF_DATE:
        case VDP_DISP_INTF_HDDATE:
        case VDP_DISP_INTF_SDDATE:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET));
            DHD0_SYNC_INV.bits.date_f_inv  = enInv.u32FdInv;
            DHD0_SYNC_INV.bits.date_vs_inv = enInv.u32VsInv;
            DHD0_SYNC_INV.bits.date_hs_inv = enInv.u32HsInv;
            DHD0_SYNC_INV.bits.date_dv_inv = enInv.u32DvInv;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32); 

            break;
        }
        case VDP_DISP_INTF_HDMI:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET));
            DHD0_SYNC_INV.bits.hdmi_f_inv  = enInv.u32FdInv;
            DHD0_SYNC_INV.bits.hdmi_vs_inv = enInv.u32VsInv;
            DHD0_SYNC_INV.bits.hdmi_hs_inv = enInv.u32HsInv;
            DHD0_SYNC_INV.bits.hdmi_dv_inv = enInv.u32DvInv;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32); 

            break;
        }
        case VDP_DISP_INTF_VGA:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET));
            DHD0_SYNC_INV.bits.vga_vs_inv = enInv.u32VsInv;
            DHD0_SYNC_INV.bits.vga_hs_inv = enInv.u32HsInv;
            DHD0_SYNC_INV.bits.vga_dv_inv = enInv.u32DvInv;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32); 

            break;
        }
        case VDP_DISP_INTF_LCD:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET));
            DHD0_SYNC_INV.bits.lcd_vs_inv = enInv.u32VsInv;
            DHD0_SYNC_INV.bits.lcd_hs_inv = enInv.u32HsInv;
            DHD0_SYNC_INV.bits.lcd_dv_inv = enInv.u32DvInv;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32); 

            break;
        }
        default:
        {
            VDP_PRINT("Error! DP_DISP_SetSyncInv Wrong Select\n");
            return ;
        }
    }
    return ;
}
#endif


HI_VOID  VDP_DISP_SetIntfMuxSel(HI_U32 u32hd_id,VDP_DISP_INTF_E enIntf)
{
    U_VO_MUX VO_MUX;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetIntfMuxSel Select Wrong CHANNEL ID\n");
        return ;
    }

    switch(enIntf)
    {
#if INTF_EN
        case VDP_DISP_INTF_LCD:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = 2; 
            VO_MUX.bits.lcd_sel = u32hd_id; 
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.bt1120_sel = u32hd_id;
            VO_MUX.bits.digital_sel = 0; 
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
#else
        case VDP_DISP_INTF_LCD:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = u32hd_id*2; 
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.digital_sel = 1+u32hd_id*2; 
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
#endif
        case VDP_DISP_INTF_HDMI:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.hdmi_sel = u32hd_id; 
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_VGA:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.vga_sel = u32hd_id;
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        case VDP_DISP_INTF_HDDATE:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.hddate_sel = u32hd_id;
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
		case VDP_DISP_INTF_SDDATE:
        {
            VO_MUX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VO_MUX.u32)));
            VO_MUX.bits.sddate_sel = u32hd_id;
            VDP_RegWrite((HI_ULONG)&(pVdpReg->VO_MUX.u32), VO_MUX.u32); 

            break;
        }
        default:
        {
            VDP_PRINT("Error! VDP_DISP_SetIntfMuxSel Wrong Select\n");
            return ;
        }
    }
    return ;
}

HI_U32 VDP_DISP_GetIntSta(HI_U32 u32intmask)
{
    U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTSTA.u32)));

    return (VOINTSTA.u32 & u32intmask);
}

#if    1
HI_VOID VDP_RST_FILE(char * fpName)
{
#if  0
	char  fpSumRst[80];
	HI_U32 ii = 0;
	HI_U32 jj= 0;
	HI_U32	u32DataTmp = 0;
	HI_U32 u32Data = &u32DataTmp;

	strncpy(fpSumRst, FILE_PATH,sizoef(fpSumRst));
	strncat(fpSumRst, fpName,sizeof(fpSumRst));
	strncat(fpSumRst,".rst",sizeof(fpSumRst));
	
	VDP_PRINT("fpSumRst= %s\n",fpSumRst);
	
	for(  ii=0;ii<MAX_CHK_CHN;ii++)
	{
		for( jj=0;jj<MAX_CHK_FRM;jj++)
		{
			
			u32Data=gu32_ret[ii][jj];
			vWrDat2Bin(fpSumRst, u32Data);
			gu32_ret[ii][jj] =0;
		}
	}
#endif
}

HI_VOID  VDP_DISP_ClearIntSta1(HI_U32 u32intmask)
{
///* clear interrupt status */
//volatile U_VOMSKINTSTA VOMSKINTSTA;
////VDP_RegWrite((HI_ULONG)&(pVdpReg->VOMSKINTSTA.u32), u32intmask);
// VDP_RegWrite((HI_ULONG)&(pVdpReg->VOMSKINTSTA.u32), 0xffffffff);
//
////printk("pVdpReg->VOMSKINTSTA.u32 == %08x\n",VDP_RegRead((HI_ULONG)&(pVdpReg->VOMSKINTSTA.u32)));

}

HI_VOID  VDP_DISP_ClearIntSta(HI_U32 u32intmask )
{
#if FPGA_AUTORUN
	
	volatile U_DHD0_CTRL DHD0_CTRL;
	volatile U_DHD0_STATE DHD0_STATE;
	volatile U_VOINTSTA VOINTSTA;

	volatile U_VOCTRL  VOCTRL;

		
		HI_U32 ii = 0;
		HI_U32 jj= 0;
		HI_U32 u32TwoChnFlag;
		HI_U32 u32_dut_data_u ;
		HI_U32 u32_dut_data_v ;

		
		HI_U32 u32_dut_data_u_1 ;
		HI_U32 u32_dut_data_v_1 ;
		
		if((u32intmask & VDP_INTMSK_DHD0_VTTHD)  == VDP_INTMSK_DHD0_VTTHD)
		{
		
			// HI_U32 * sum_data = (HI_U32 *)IO_ADDRESS(0xf8ccc1c0);
			  HI_U32 u32_dut_data ;

			  HI_U32 u32_dut_data_H ;
			  HI_U32 u32_dut_data_L ;
			  HI_U32 u32_dut_data_u_H ;
			  HI_U32 u32_dut_data_v_H ;
			  HI_U32 u32_dut_data_u_L ;
			  HI_U32 u32_dut_data_v_L ;
			
			  HI_U32 u32_dut_data_1 ;

			  
			// HI_U32 u32_rm_data = gu32SumOut[ii][gu32FrmCnt]; 
			HI_U32 *sum_data ;

			
			HI_U32 *sum_data1 ;
			
			DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD0*CHN_OFFSET));
			DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD0*CHN_OFFSET));

			sum_data = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_DHD0);

#if   1
			if(DHD0_CTRL.bits.iop == 0)
			{

			if(gu32FrmCnt%2 == 0)
			{
				

			while(DHD0_STATE.bits.bottom_field != 1 || (DHD0_STATE.bits.vback_blank != 0) ||(DHD0_STATE.bits.vcnt > 200) )
			//while(DHD0_STATE.bits.bottom_field != 1 || (DHD0_STATE.bits.vback_blank != 0) )			
			{
				DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD0*CHN_OFFSET));
			}
			}
			else if(gu32FrmCnt%2 == 1)
			{
				
			while(DHD0_STATE.bits.bottom_field != 0 || (DHD0_STATE.bits.vblank != 0)||(DHD0_STATE.bits.vcnt > 200) )
				
			//while(DHD0_STATE.bits.bottom_field != 0 || (DHD0_STATE.bits.vblank != 0) )
			{
				DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD0*CHN_OFFSET)); 					 
			}
			}	  

			}
#endif
			u32_dut_data	 =	 *(sum_data+0);
			u32_dut_data_u	 =	 *(sum_data+1);
			u32_dut_data_v	 =	 *(sum_data+2);
			



			VDP_PRINT("dhd0 dut Y sum = 0x%0x\n",u32_dut_data);		
			VDP_PRINT("dhd0 dut U sum = 0x%0x\n",u32_dut_data_u); 		
			VDP_PRINT("dhd0 dut V sum = 0x%0x\n",u32_dut_data_v); 		

			static char cSrcFile[1024];
			static char cSrcTmp[1024];

			strncpy(cSrcFile , "[frame number :  ",sizeof(cSrcFile) );		
			snprintf(cSrcTmp, sizeof(cSrcTmp),"%x]",gu32FrmCnt+1);
			strncat(cSrcFile,cSrcTmp,sizeof(cSrcFile));
			
			vWrDis2TxtHdr(cIfNamehdr, cSrcFile);

			static char	bufY[256];
			static char	bufU[256];
			static char	bufV[256];


			snprintf(bufY, sizeof(bufY),"%x\n",u32_dut_data);	
			snprintf(bufU, sizeof(bufU),"%x\n",u32_dut_data_u);	
			snprintf(bufV, sizeof(bufV),"%x\n",u32_dut_data_v);	
			
			SV_klib_fwrite(bufY, strlen(bufY),fpCfgHdrOut);
			SV_klib_fwrite(bufU, strlen(bufU),fpCfgHdrOut);
			SV_klib_fwrite(bufV, strlen(bufV),fpCfgHdrOut);
			
			

			*(gu32_ret + 0*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data;
			*(gu32_ret + 1*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_u;
			*(gu32_ret + 2*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_v;
			
			
		}
	
		if((u32intmask & VDP_INTMSK_DHD1_VTTHD)  == VDP_INTMSK_DHD1_VTTHD)
		{
		
			DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD1*CHN_OFFSET));
			DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD1*CHN_OFFSET));
		
			HI_U32 u32_rm_data_Y ;	
			HI_U32 u32_rm_data_U ;	
			HI_U32 u32_rm_data_V ;	
			HI_U32 *sum_data ;
			HI_U32 u32_dut_data;
			
			HI_U32 *sum_data1 ;
			HI_U32 u32_dut_data1;
			HI_U32 *sum_data_B ;
			HI_U32 u32_dut_data_B;
			HI_U32 u32_dut_data_u_B ;
			HI_U32 u32_dut_data_v_B ;
			HI_U32 jj;			
#if   1
			if(DHD0_CTRL.bits.iop == 0)
			{
			
			 if(gu32FrmCnt%2 == 0)
			 {
				 while(DHD0_STATE.bits.bottom_field != 1 || (DHD0_STATE.bits.vblank != 0)||(DHD0_STATE.bits.vcnt > 40) )
				 {
					DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD1*CHN_OFFSET));
				 }
			 }
			else if(gu32FrmCnt%2 == 1)
			 {
				 while(DHD0_STATE.bits.bottom_field != 0 || (DHD0_STATE.bits.vblank != 0)||(DHD0_STATE.bits.vcnt > 40) )
				 {
					 DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD1*CHN_OFFSET));						 
				 }
			 }	  
			/*
			if(gu32FrmCnt == 6)
			{
				
				while(DHD0_STATE.bits.bottom_field != 0)
				{
				   DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD1*CHN_OFFSET));
				}
				sum_data_B = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_DHD1);
				u32_dut_data_B	  = *sum_data_B;
				u32_dut_data_u_B = *(sum_data_B+1);
				u32_dut_data_v_B = *(sum_data_B+2);
				  while(DHD0_STATE.bits.bottom_field != 1)
				  {
					  DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DHD1*CHN_OFFSET));						  
				  }
			}
			*/
			
			}
#endif
#if 0
for(  ii=0;ii<MAX_CHK_CHN;ii++)
{
	for( jj=0;jj<MAX_CHK_FRM;jj++)
	{
		gu32_ret[ii][jj] =0;
	}
}
#endif
			sum_data1 = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_DHD1);

        #if 1
			u32_dut_data1	    =   *sum_data1 ;
			u32_dut_data_u_1	=	*(sum_data1+1)  ; 
			u32_dut_data_v_1	=	*(sum_data1+2) ; 
        #else
			u32_dut_data1	   =	*(sum_data1+1);
			u32_dut_data_u_1	=	 *(sum_data1+3) ; 
			u32_dut_data_v_1	=	 *(sum_data1+5) ; 
        #endif

			*(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 0*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data1;
			*(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 1*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_u_1;
			*(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 2*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_v_1;


			//gu32_ret[1*ONE_CHK_CHN+0][gu32FrmCnt] = u32_dut_data;
			//gu32_ret[1*ONE_CHK_CHN+1][gu32FrmCnt] = u32_dut_data_u;
			//gu32_ret[1*ONE_CHK_CHN+2][gu32FrmCnt] = u32_dut_data_v;


		
	//	VDP_PRINT("dgu32_ret[1*ONE_CHK_CHN+0][gu32FrmCnt]  = 0x%0x\n",gu32_ret[1*ONE_CHK_CHN+0][gu32FrmCnt] );		
	//	VDP_PRINT("dgu32_ret[1*ONE_CHK_CHN+1][gu32FrmCnt]  = 0x%0x\n",gu32_ret[1*ONE_CHK_CHN+1][gu32FrmCnt] );	
	//	VDP_PRINT("dgu32_ret[1*ONE_CHK_CHN+2][gu32FrmCnt]  = 0x%0x\n",gu32_ret[1*ONE_CHK_CHN+2][gu32FrmCnt] );		

		VDP_PRINT("dut*********** sum_data1 = 0x%0x\n",sum_data1);		
		
		VDP_PRINT("dut* Y sum = 0x%0x\n",u32_dut_data1); 	  
		VDP_PRINT("dut* U sum = 0x%0x\n",u32_dut_data_u_1);		   
		VDP_PRINT("dut* V sum = 0x%0x\n",u32_dut_data_v_1);		   

        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);
        VDP_PRINT("u32_dut_data1 ======  %x\n",u32_dut_data1);


		}
	if((u32intmask & VDP_INTMSK_DSD0_VTTHD)  == VDP_INTMSK_DSD0_VTTHD)
	{	
		DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DSD0*CHN_OFFSET));
		DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DSD0*CHN_OFFSET));
		
		if(DHD0_CTRL.bits.iop == 0)
		{
			if(gu32FrmCnt%2 == 1)
			{
				while(DHD0_STATE.bits.bottom_field != 0 || (DHD0_STATE.bits.vblank != 0))//0 :top file	; 1 : bottom files;
				{
					DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DSD0*CHN_OFFSET));
				}
			}
		   else if(gu32FrmCnt%2 == 0 )
			{
				while(DHD0_STATE.bits.bottom_field != 1 || (DHD0_STATE.bits.vblank != 0))
				{
					DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+VDP_CHN_DSD0*CHN_OFFSET));
				}
			}			
		}
		HI_U32 * sum_data = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_DSD0);
		HI_U32 u32_dut_data = *sum_data;
		HI_U32 u32_rm_data = 0; 

		
		u32_dut_data	  =   *sum_data + *(sum_data+1);
		u32_dut_data_u	  =   *(sum_data+2) + *(sum_data+3) ; 
		u32_dut_data_v	  =   *(sum_data+4) + *(sum_data+5) ; 


		//why to minus 1 ???? why gu32FrmCnt == 1 at the  begin !!!
		*(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 0*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data;
		*(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 1*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_u;
		*(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 2*MAX_CHK_FRM + gu32FrmCnt) = u32_dut_data_v;

	   VDP_PRINT("dut Y sum	= 0x%0x\n", u32_dut_data);		   
	   VDP_PRINT("dut U sum	= 0x%0x\n",(u32_dut_data_u));		  
	   VDP_PRINT("dut V sum	= 0x%0x\n",(u32_dut_data_v));		  


	   VDP_PRINT("gu32FrmCnt  = %d\n",gu32FrmCnt);		  


	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));
	   printk("*(sum_data+1) ======  %x\n",*(sum_data+1));


		
	   // gu32_ret[2*ONE_CHK_CHN+0][gu32FrmCnt] = u32_dut_data;
	   // gu32_ret[2*ONE_CHK_CHN+1][gu32FrmCnt] = u32_dut_data_u;
	   // gu32_ret[2*ONE_CHK_CHN+2][gu32FrmCnt] = u32_dut_data_v;
		   

		 
	}
	
	if((u32intmask & VDP_INTMSK_WBC_HD0_INT)  == VDP_INTMSK_WBC_HD0_INT)
	{
		
		 HI_U32 * sum_data_wbc = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_WBCDHD0);
		 HI_U32 u32_dut_data_wbc_y = *sum_data_wbc;
		 HI_U32 u32_dut_data_wbc_c = *sum_data_wbc;
		 HI_U32 u32_rm_data = 0; 
		
		 
		 u32_dut_data_wbc_y 	 =	 *sum_data_wbc ;
		 u32_dut_data_wbc_c    =   *(sum_data_wbc+1); 
		
		
		 //why to minus 1 ???? why gu32FrmCnt == 1 at the  begin !!!
		 *(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 0*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_y;
		 *(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 1*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
		 *(gu32_ret + 2*ONE_CHK_CHN*MAX_CHK_FRM + 2*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
		
		VDP_PRINT("wbc dut Y sum  = 0x%0x\n", u32_dut_data_wbc_y);		  
		VDP_PRINT("wbc dut C sum  = 0x%0x\n",(u32_dut_data_wbc_c));		   
	   // VDP_PRINT("dut V sum  = 0x%0x\n",(u32_dut_data_v));		 
		
		
		VDP_PRINT("gu32FrmCnt  = %d\n",gu32FrmCnt);		   
		
	}

//wbc vp
	if((u32intmask & VDP_INTMSK_WBC_G4_INT)  == VDP_INTMSK_WBC_G4_INT)
	{
		printk("line :%d \n",__LINE__);
		 HI_U32 * sum_data_wbc = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_WBCVP0);
		 HI_U32 u32_dut_data_wbc_y = *sum_data_wbc;
		 HI_U32 u32_dut_data_wbc_c = *sum_data_wbc;
		 HI_U32 u32_rm_data = 0; 
		
		 
		 u32_dut_data_wbc_y 	 =	 *sum_data_wbc ;
		 u32_dut_data_wbc_c    =   *(sum_data_wbc+1); 
		
		
		 //why to minus 1 ???? why gu32FrmCnt == 1 at the  begin !!!
		 *(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 0*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_y;
		 *(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 1*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
		 *(gu32_ret + 1*ONE_CHK_CHN*MAX_CHK_FRM + 2*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
		
		VDP_PRINT("wbc dut Y sum  = 0x%0x\n", u32_dut_data_wbc_y);		  
		VDP_PRINT("wbc dut C sum  = 0x%0x\n",(u32_dut_data_wbc_c));		   

		//VDP_PRINT("dut V sum  = 0x%0x\n",(u32_dut_data_v));		 
		
		
		VDP_PRINT("gu32FrmCnt  = %d\n",gu32FrmCnt);		   
		
	}

if((u32intmask & VDP_INTMSK_WBC_GP0_INT)  == VDP_INTMSK_WBC_GP0_INT)
{
	
	 HI_U32 * sum_data_wbc = (HI_U32 *)IO_ADDRESS(VDP_CHK_ADDR_WBCGP0);
	 HI_U32 u32_dut_data_wbc_y = *sum_data_wbc;
	 HI_U32 u32_dut_data_wbc_c = *sum_data_wbc;
	 HI_U32 u32_rm_data = 0; 
	
	 
	 u32_dut_data_wbc_y 	 =	 *sum_data_wbc ;
	 u32_dut_data_wbc_c    =   *(sum_data_wbc+1); 
	
	
	 //why to minus 1 ???? why gu32FrmCnt == 1 at the  begin !!!
	 *(gu32_ret + 4*ONE_CHK_CHN*MAX_CHK_FRM + 0*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_y;
	 *(gu32_ret + 4*ONE_CHK_CHN*MAX_CHK_FRM + 1*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
	 *(gu32_ret + 4*ONE_CHK_CHN*MAX_CHK_FRM + 2*MAX_CHK_FRM + gu32FrmCnt-1) = u32_dut_data_wbc_c;
	
	VDP_PRINT("wbc dut Y sum  = 0x%0x\n", u32_dut_data_wbc_y);		  
	VDP_PRINT("wbc dut C sum  = 0x%0x\n",(u32_dut_data_wbc_c));		   

	//VDP_PRINT("dut V sum  = 0x%0x\n",(u32_dut_data_v));		 
	
	
	VDP_PRINT("gu32FrmCnt  = %d\n",gu32FrmCnt);		   
	
}

	
#endif

//volatile U_VOMSKINTSTA VOMSKINTSTA;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->VOMSKINTSTA.u32), u32intmask);

#if FPGA_AUTORUN_CFG
	if((u32intmask & VDP_INTMSK_DHD0_VTTHD)  == VDP_INTMSK_DHD0_VTTHD)
	{
		char cSrcFile[1024];
		char cSrcTmp[1024];

	HI_U32	intf0_en;
	HI_U32	intf1_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD0*CHN_OFFSET));
	  intf0_en = DHD0_CTRL.bits.intf_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD1*CHN_OFFSET));
	  intf1_en = DHD0_CTRL.bits.intf_en;
	
	if( (intf0_en == 1) &  (intf1_en == 1))
	 {
		 if(gu32FrmCnt1%2 == 1)
		{
		 }
		 
	}
	else
	{
	
			strncpy(cSrcFile , "[reg frame ",sizeof(cSrcFile));		
			snprintf(cSrcTmp,sizeof(cSrcTmp), "%d]",gu32FrmCnt+1);
			strncat(cSrcFile,cSrcTmp,sizeof(cSrcFile));
			
			vWrDis2Txt(cIfName, cSrcFile);
	}

  
	   
//		  VDP_PRINT("cSrc2File = %s\n",cSrcFile);
//		  VDP_PRINT("cSrcTmp = %s\n\n",cSrcTmp); 
	
	}
	else if((u32intmask & VDP_INTMSK_DHD1_VTTHD)  == VDP_INTMSK_DHD1_VTTHD)
	{
		char cSrcFile[1024];
		char cSrcTmp[1024];
		
	HI_U32	intf0_en;
	HI_U32	intf1_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD0*CHN_OFFSET));
	  intf0_en = DHD0_CTRL.bits.intf_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD1*CHN_OFFSET));
	  intf1_en = DHD0_CTRL.bits.intf_en;
	
	  VDP_PRINT("intf0_en = %d\n",intf0_en);

	  VDP_PRINT("intf1_en = %d\n",intf1_en);
	  
	 VDP_PRINT("gu32FrmCnt = %d\n",gu32FrmCnt);
	  
	if( intf0_en == 1  &  intf1_en == 1)
	 {
		 if(gu32FrmCnt1%2 == 0)
		{
			strncpy(cSrcFile , "[reg frame ",sizeof(cSrcFile));		
			snprintf(cSrcTmp, sizeof(cSrcTmp),"%d]",gu32FrmCnt +1);
			strncat(cSrcFile,cSrcTmp,sizeof(cSrcFile));
			
	  VDP_PRINT("cSrc2File = %s\n",cSrcFile);
	  VDP_PRINT("cSrcTmp = %s\n\n",cSrcTmp); 
			vWrDis2Txt(cIfName, cSrcFile);
			
		 }
	}
	else
	{
			strncpy(cSrcFile , "[reg frame ",sizeof(cSrcFile));		
			snprintf(cSrcTmp, sizeof(cSrcTmp),"%d]",gu32FrmCnt+1);
			strncat(cSrcFile,cSrcTmp);
			
			vWrDis2Txt(cIfName, cSrcFile,sizeof(cSrcFile));
	}
		
		//		  VDP_PRINT("cSrc2File = %s\n",cSrcFile);
		//		  VDP_PRINT("cSrcTmp = %s\n\n",cSrcTmp); 
		
	}
	else if((u32intmask & VDP_INTMSK_DSD0_VTTHD)  == VDP_INTMSK_DSD0_VTTHD)
	{
		char cSrcFile[1024];
		char cSrcTmp[1024];

		strncpy(cSrcFile , "[reg frame ",sizeof(cSrcFile));		
		snprintf(cSrcTmp, sizeof(cSrcTmp),"%d]",gu32FrmCnt+1);
	
		strncat(cSrcFile,cSrcTmp,sizeof(cSrcFile));
		
//		  VDP_PRINT("cSrc2File = %s\n",cSrcFile);
//		  VDP_PRINT("cSrcTmp = %s\n\n",cSrcTmp); 
	
		vWrDis2Txt(cIfName, cSrcFile);
	}
	/* clear interrupt status */

	
	/* read interrupt status */
	VOINTSTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTSTA.u32)));
	if((u32intmask & VDP_INTMSK_WBC_HD0_INT)  != VDP_INTMSK_WBC_HD0_INT)
	{
	HI_U32	intf0_en;
	HI_U32	intf1_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD0*CHN_OFFSET));
	intf0_en = DHD0_CTRL.bits.intf_en;
	DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+VDP_CHN_DHD1*CHN_OFFSET));
	intf1_en = DHD0_CTRL.bits.intf_en;

	if( (intf0_en == 1) &  (intf1_en == 1))
	{
	   printk("********two chn intf is	open !!!\n");
	   if(gu32FrmCnt1%2 ==0)
	   {   
		 gu32FrmCnt++;
	   }
	   
	   gu32FrmCnt1++;
	}
	else
	{
		gu32FrmCnt++;

	}
   }
	
#endif
  }


#endif



HI_U32 VDP_DISP_GetOflIntSta(HI_U32 u32intmask)
{
    U_VOINTSTA_OFL VOINTSTA_OFL;

    /* read interrupt status */
    VOINTSTA_OFL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOINTSTA_OFL.u32)));

    return (VOINTSTA_OFL.u32 & u32intmask);
}

HI_VOID VDP_DISP_SetVtThdMode(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode)
{
    U_DHD0_VTTHD3 DHD0_VTTHD3;
    U_DHD0_VTTHD  DHD0_VTTHD;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVtThdMode Select Wrong CHANNEL ID\n");
        return ;
    }

    if(u32uthdnum == 1)//threshold 1 int mode
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VTTHD.bits.thd1_mode = u32mode;// frame or field
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 2)//threshold 2 int mode
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD.bits.thd2_mode = u32mode;// frame or field
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 3)//threshold 3 int mode
    {
        DHD0_VTTHD3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD3.bits.thd3_mode = u32mode;// frame or field
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD3.u32);            

    }
}

HI_VOID VDP_DISP_SetVtThd(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd)
{

    U_DHD0_VTTHD DHD0_VTTHD;
    U_DHD0_VTTHD3 DHD0_VTTHD3;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVtThd Select Wrong CHANNEL ID\n");
        return ;
    }

    if(u32uthdnum == 1)
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VTTHD.bits.vtmgthd1 = u32vtthd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum == 2)
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD.bits.vtmgthd2 = u32vtthd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD.u32);
    }
    else if(u32uthdnum== 3)
    {
        DHD0_VTTHD3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_VTTHD3.bits.vtmgthd3 = u32vtthd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD3.u32);            
    }
	else if(u32uthdnum== 4)
    {
        DHD0_VTTHD3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VTTHD3.bits.vtmgthd4 = u32vtthd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET), DHD0_VTTHD3.u32);            
    }
}

HI_VOID VDP_DISP_SetCbarEnable(HI_U32 u32hd_id,HI_U32 bTrue)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_SetCbarEnable Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.cbar_en = bTrue;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetCbarSel(HI_U32 u32hd_id,HI_U32 u32_cbar_sel)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_SetCbarSel Select Wrong CHANNEL ID\n");
        return ;
    }
    VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.cbar_sel = u32_cbar_sel;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetDitherEnable(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, HI_U32 u32Enable)
{

    U_DHD0_DITHER_CTRL DHD0_DITHER_CTRL;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    U_DHD0_DITHER1_CTRL DHD0_DITHER1_CTRL;

    if(enIntf == VDP_DISP_INTF_LCD )
    {

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_en= u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_en= u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_en = u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
        VDP_PRINT("Error,VDP_DISP_SetDitherEnable  Select Wrong Interface Mode\n");
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

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_round = u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
        VDP_PRINT("Error,VDP_DISP_SetDitherEnable  Select Wrong Interface Mode\n");
        return ;

    }

}

HI_VOID VDP_DISP_SetDitherMode  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_E enDitherMode)
{
    U_DHD0_DITHER_CTRL DHD0_DITHER_CTRL;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    U_DHD0_DITHER1_CTRL DHD0_DITHER1_CTRL;


    if(enDitherMode == VDP_DITHER_ROUND_8 )
    {
        enDitherMode =VDP_DITHER_TMP_10 ;
    }
    else  if(enDitherMode == VDP_DITHER_ROUND_10)
    {
        enDitherMode =VDP_DITHER_DROP_10;
    }
    else
    {
       ;// VDP_PRINT("Error,VDP_WBC_SetDitherMode() select wrong Mode!!!!  \n");
    }

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong CHANNEL ID\n");
        return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD )
    {

        DHD0_DITHER0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_CTRL.bits.dither_mode = enDitherMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_CTRL.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {
        DHD0_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_CTRL.u32);
    }
    else 
    {
       // VDP_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong Interface Mode\n");
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
        VDP_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD)
    {

        DHD0_DITHER0_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        DHD0_DITHER0_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        DHD0_DITHER0_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        DHD0_DITHER0_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_COEF0.u32);

        DHD0_DITHER0_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER0_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        DHD0_DITHER0_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        DHD0_DITHER0_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        DHD0_DITHER0_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER0_COEF1.u32);

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
        DHD0_DITHER1_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_COEF0.bits.dither_coef0 =dither_coef.dither_coef0 ;
        DHD0_DITHER1_COEF0.bits.dither_coef1 =dither_coef.dither_coef1 ;
        DHD0_DITHER1_COEF0.bits.dither_coef2 =dither_coef.dither_coef2 ;
        DHD0_DITHER1_COEF0.bits.dither_coef3 =dither_coef.dither_coef3 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_COEF0.u32);

        DHD0_DITHER1_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER1_COEF1.bits.dither_coef4 =dither_coef.dither_coef4 ;
        DHD0_DITHER1_COEF1.bits.dither_coef5 =dither_coef.dither_coef5 ;
        DHD0_DITHER1_COEF1.bits.dither_coef6 =dither_coef.dither_coef6 ;
        DHD0_DITHER1_COEF1.bits.dither_coef7 =dither_coef.dither_coef7 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER1_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER1_COEF1.u32);

    }
    else if(enIntf == VDP_DISP_INTF_VGA)
    {

        DHD0_DITHER_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER_COEF0.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        DHD0_DITHER_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        DHD0_DITHER_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        DHD0_DITHER_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER_COEF0.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_COEF0.u32);

        DHD0_DITHER_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER_COEF1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_DITHER_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        DHD0_DITHER_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        DHD0_DITHER_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        DHD0_DITHER_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DITHER_COEF1.u32)+u32hd_id*CHN_OFFSET), DHD0_DITHER_COEF1.u32);

    }

    else 
    {
        VDP_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong Interface Mode\n");
        return ;

    }

}

HI_VOID  VDP_DISP_SetClipEnable  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf)
{
    VDP_PRINT("Error,This Driver has been deleted ...\n");
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
        VDP_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong CHANNEL ID\n");
        return ;
    }

    //BT1120
    if(clipsel == VDP_DISP_INTF_BT1120)
    {
        DHD0_CLIP0_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP0_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP0_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP0_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP0_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP0_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP0_L.u32);

        DHD0_CLIP0_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP0_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP0_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP0_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP0_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP0_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP0_H.u32); 
    }

    //DATE
    else if(clipsel == VDP_DISP_INTF_SDDATE)
    {

        DHD0_CLIP1_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP1_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP1_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP1_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP1_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP1_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP1_L.u32);

        DHD0_CLIP1_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP1_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP1_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP1_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP1_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP1_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP1_H.u32); 
    }

    //HDMI
    else if(clipsel == VDP_DISP_INTF_HDMI)
    {

        DHD0_CLIP2_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP2_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP2_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP2_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP2_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP2_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP2_L.u32);

        DHD0_CLIP2_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP2_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP2_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP2_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP2_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP2_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP2_H.u32); 
    }

    //VGA
    else if(clipsel == VDP_DISP_INTF_VGA)
    {

        DHD0_CLIP3_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP3_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP3_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP3_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP3_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP3_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP3_L.u32);

        DHD0_CLIP3_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP3_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP3_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP3_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP3_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP3_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP3_H.u32); 
    }

    //LCD
    else if(clipsel == VDP_DISP_INTF_LCD)
    {

        DHD0_CLIP4_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP4_L.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP4_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
        DHD0_CLIP4_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
        DHD0_CLIP4_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP4_L.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP4_L.u32);

        DHD0_CLIP4_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CLIP4_H.u32)+u32hd_id*CHN_OFFSET));        
        DHD0_CLIP4_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
        DHD0_CLIP4_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
        DHD0_CLIP4_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CLIP4_H.u32)+u32hd_id*CHN_OFFSET), DHD0_CLIP4_H.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong Interface Mode\n");
    }
}

HI_VOID  VDP_DISP_SetCscEnable  (HI_U32 u32hd_id, HI_U32 enCSC)
{   
    U_DHD0_CSC_IDC DHD0_CSC_IDC;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetCscEnable Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET));        
    DHD0_CSC_IDC.bits.csc_en = enCSC;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IDC.u32);

}

HI_VOID   VDP_DISP_SetCscDcCoef(HI_U32 u32hd_id,VDP_CSC_DC_COEF_S stCscCoef)
{   
    U_DHD0_CSC_IDC DHD0_CSC_IDC;
    U_DHD0_CSC_ODC DHD0_CSC_ODC;
    U_DHD0_CSC_IODC DHD0_CSC_IODC;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetCscDcCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
    DHD0_CSC_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IDC.u32);

    DHD0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_ODC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
    DHD0_CSC_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_ODC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_ODC.u32);

    DHD0_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_IODC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
    DHD0_CSC_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_IODC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IODC.u32);

}

HI_VOID   VDP_DISP_SetCscCoef(HI_U32 u32hd_id,VDP_CSC_COEF_S stCscCoef)
{   
    U_DHD0_CSC_P0        DHD0_CSC_P0;
    U_DHD0_CSC_P1        DHD0_CSC_P1;
    U_DHD0_CSC_P2        DHD0_CSC_P2;
    U_DHD0_CSC_P3        DHD0_CSC_P3;
    U_DHD0_CSC_P4        DHD0_CSC_P4;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetCscCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_P0.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    DHD0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_P0.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P0.u32);

    DHD0_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_P1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    DHD0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_P1.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P1.u32);

    DHD0_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_P2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    DHD0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_P2.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P2.u32);

    DHD0_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_P3.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    DHD0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_P3.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P3.u32);

    DHD0_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CSC_P4.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CSC_P4.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P4.u32);

}

HI_VOID VDP_DISP_SetCscMode(HI_U32 u32hd_id, VDP_CSC_MODE_E enCscMode)
{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;

    HI_S32 u32Pre   = 1 << 10;
    HI_S32 u32DcPre = 4;//1:8bit; 4:10bit

    if(enCscMode == VDP_CSC_RGB2YUV_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.299  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.587  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.114  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.172 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.339 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(0.511  * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(0.511  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.428 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.083 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.371  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.698 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.336 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.732  * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  0 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_RGB2YUV_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.213  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.715  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.072  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.117 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.394 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.511 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)( 0.511 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.464 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.047 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 16  * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.540  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.183 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.459 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.816  * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 0 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_709_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_601_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * u32DcPre;
    }
    else
    {
        st_csc_coef.csc_coef00     = 1 * u32Pre;
        st_csc_coef.csc_coef01     = 0 * u32Pre;
        st_csc_coef.csc_coef02     = 0 * u32Pre;

        st_csc_coef.csc_coef10     = 0 * u32Pre;
        st_csc_coef.csc_coef11     = 1 * u32Pre;
        st_csc_coef.csc_coef12     = 0 * u32Pre;

        st_csc_coef.csc_coef20     = 0 * u32Pre;
        st_csc_coef.csc_coef21     = 0 * u32Pre;
        st_csc_coef.csc_coef22     = 1 * u32Pre;

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }

    VDP_DISP_SetCscCoef  (u32hd_id,st_csc_coef);
    VDP_DISP_SetCscDcCoef(u32hd_id,st_csc_dc_coef);

    return ;
}

HI_VOID VDP_DISP_SetGammaEnable(HI_U32 u32hd_id, HI_U32 u32GmmEn)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetGammaEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.gmm_en = u32GmmEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}
HI_VOID VDP_DISP_SetMirrorEnable(HI_U32 u32hd_id, HI_U32 u32MirrEn)
{
    U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetGammaEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.mirror_en = 0;//u32MirrEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetGammaAddr(HI_U32 u32hd_id, HI_U32 uGmmAddr)
{
    U_DHD0_GMM_COEFAD DHD0_GMM_COEFAD;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetGammaAddr Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_GMM_COEFAD.u32 = uGmmAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_GMM_COEFAD.u32)+u32hd_id*CHN_OFFSET), DHD0_GMM_COEFAD.u32);

}

HI_VOID  VDP_DISP_SetParaUpd (HI_U32 u32Data, VDP_DISP_PARA_E enMode)
{
    U_DHD0_PARAUP DHD0_PARAUP;
    DHD0_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_PARAUP.u32) + u32Data * CHN_OFFSET));
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("error,VDP_DISP_SetParaUpd() select wrong video layer id\n");
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
        VDP_PRINT("error,VDP_DISP_SetParaUpd() select wrong mode!\n");
    }
    
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_PARAUP.u32) + u32Data * CHN_OFFSET), DHD0_PARAUP.u32); 
    return ;
}


//mod me
HI_VOID VDP_DISP_SetProToInterEnable   (HI_U32 u32hd_id, HI_U32 u32Enable)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetProToInterEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.p2i_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}


#if 0
HI_VOID VDP_INTF_SetDitherMode  (HI_U32 u32Data, VDP_DITHER_E enDitherMode)

{
    U_INTF_DITHER0_CTRL INTF_DITHER0_CTRL;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetDitherMode Select Wrong INTF ID\n");
        return ;
    }


    INTF_DITHER0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_DITHER0_CTRL.u32)+u32Data*INTF_OFFSET));
    INTF_DITHER0_CTRL.bits.dither_md = enDitherMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_DITHER0_CTRL.u32)+u32Data*INTF_OFFSET), INTF_DITHER0_CTRL.u32);
}

HI_VOID VDP_INTF_SetDitherCoef  (HI_U32 u32Data,  VDP_DITHER_COEF_S dither_coef)

{
    U_INTF_DITHER0_COEF0 INTF_DITHER0_COEF0;
    U_INTF_DITHER0_COEF1 INTF_DITHER0_COEF1;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetDitherCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    INTF_DITHER0_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_DITHER0_COEF0.u32)+u32Data*INTF_OFFSET));
    INTF_DITHER0_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
    INTF_DITHER0_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
    INTF_DITHER0_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
    INTF_DITHER0_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_DITHER0_COEF0.u32)+u32Data*INTF_OFFSET), INTF_DITHER0_COEF0.u32);

    INTF_DITHER0_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_DITHER0_COEF1.u32)+u32Data*INTF_OFFSET));
    INTF_DITHER0_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
    INTF_DITHER0_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
    INTF_DITHER0_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
    INTF_DITHER0_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_DITHER0_COEF1.u32)+u32Data*INTF_OFFSET), INTF_DITHER0_COEF1.u32);
}

HI_VOID  VDP_INTF_SetClipEnable  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf)
{
    VDP_PRINT("Error,This Driver has been deleted ...\n");
}



HI_VOID  VDP_INTF_SetCscEnable  (HI_U32 u32Data, HI_U32 enCSC)
{   
    U_INTF_CSC_IDC INTF_CSC_IDC;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetCscEnable Select Wrong CHANNEL ID\n");
        return ;
    }
    INTF_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_IDC.u32)+u32Data*INTF_OFFSET));        
    INTF_CSC_IDC.bits.csc_en = enCSC;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_IDC.u32)+u32Data*INTF_OFFSET), INTF_CSC_IDC.u32);

}

HI_VOID   VDP_INTF_SetCscDcCoef(HI_U32 u32Data,VDP_CSC_DC_COEF_S stCscCoef)
{   
    U_INTF_CSC_IDC  INTF_CSC_IDC;
    U_INTF_CSC_ODC  INTF_CSC_ODC;
    U_INTF_CSC_IODC INTF_CSC_IODC;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetCscDcCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    INTF_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_IDC.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
    INTF_CSC_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_IDC.u32)+u32Data*INTF_OFFSET), INTF_CSC_IDC.u32);

    INTF_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_ODC.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
    INTF_CSC_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_ODC.u32)+u32Data*INTF_OFFSET), INTF_CSC_ODC.u32);

    INTF_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_IODC.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
    INTF_CSC_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_IODC.u32)+u32Data*INTF_OFFSET), INTF_CSC_IODC.u32);

}

HI_VOID   VDP_INTF_SetCscCoef(HI_U32 u32Data,VDP_CSC_COEF_S stCscCoef)
{   
    U_INTF_CSC_P0        INTF_CSC_P0;
    U_INTF_CSC_P1        INTF_CSC_P1;
    U_INTF_CSC_P2        INTF_CSC_P2;
    U_INTF_CSC_P3        INTF_CSC_P3;
    U_INTF_CSC_P4        INTF_CSC_P4;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetCscCoef Select Wrong INTF ID\n");
        return ;
    }

    INTF_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_P0.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    INTF_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_P0.u32)+u32Data*INTF_OFFSET), INTF_CSC_P0.u32);

    INTF_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_P1.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    INTF_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_P1.u32)+u32Data*INTF_OFFSET), INTF_CSC_P1.u32);

    INTF_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_P2.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    INTF_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_P2.u32)+u32Data*INTF_OFFSET), INTF_CSC_P2.u32);

    INTF_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_P3.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    INTF_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_P3.u32)+u32Data*INTF_OFFSET), INTF_CSC_P3.u32);

    INTF_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CSC_P4.u32)+u32Data*INTF_OFFSET));
    INTF_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CSC_P4.u32)+u32Data*INTF_OFFSET), INTF_CSC_P4.u32);

}

HI_VOID VDP_INTF_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)

{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;

    HI_U32 u32Pre   = 1 << 10;
    HI_U32 u32DcPre = 4;//1:8bit; 4:10bit

    if(enCscMode == VDP_CSC_RGB2YUV_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.299  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.587  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.114  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.172 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.339 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(0.511  * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(0.511  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.428 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.083 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.371  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.698 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.336 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.732  * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  0 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_RGB2YUV_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.213  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.715  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.072  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.117 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.394 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.511 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)( 0.511 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.464 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.047 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 16  * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.540  * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.183 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.459 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.816  * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 0 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_709_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_601_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * u32DcPre;
    }
    else
    {
        st_csc_coef.csc_coef00     = 1 * u32Pre;
        st_csc_coef.csc_coef01     = 0 * u32Pre;
        st_csc_coef.csc_coef02     = 0 * u32Pre;

        st_csc_coef.csc_coef10     = 0 * u32Pre;
        st_csc_coef.csc_coef11     = 1 * u32Pre;
        st_csc_coef.csc_coef12     = 0 * u32Pre;

        st_csc_coef.csc_coef20     = 0 * u32Pre;
        st_csc_coef.csc_coef21     = 0 * u32Pre;
        st_csc_coef.csc_coef22     = 1 * u32Pre;

        st_csc_dc_coef.csc_in_dc2  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * u32DcPre;
    }

    VDP_INTF_SetCscCoef  (u32Data,st_csc_coef);
    VDP_INTF_SetCscDcCoef(u32Data,st_csc_dc_coef);

    return ;
}


HI_VOID  VDP_INTF_SetClipCoef (HI_U32 u32Data, VDP_DISP_CLIP_S stClipData)
{   

    U_INTF_CLIP0_L       INTF_CLIP0_L;
    U_INTF_CLIP0_H       INTF_CLIP0_H;
    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetClipVtt Select Wrong INTF ID\n");
        return ;
    }

    INTF_CLIP0_L.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CLIP0_L.u32)+u32Data*INTF_OFFSET));        
    INTF_CLIP0_L.bits.clip_cl2 = stClipData.u32ClipLow_y;
    INTF_CLIP0_L.bits.clip_cl1 = stClipData.u32ClipLow_cb;
    INTF_CLIP0_L.bits.clip_cl0 = stClipData.u32ClipLow_cr;        
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CLIP0_L.u32)+u32Data*INTF_OFFSET), INTF_CLIP0_L.u32);

    INTF_CLIP0_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CLIP0_H.u32)+u32Data*INTF_OFFSET));        
    INTF_CLIP0_H.bits.clip_ch2 = stClipData.u32ClipHigh_y;
    INTF_CLIP0_H.bits.clip_ch1 = stClipData.u32ClipHigh_cb;
    INTF_CLIP0_H.bits.clip_ch0 = stClipData.u32ClipHigh_cr;        
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CLIP0_H.u32)+u32Data*INTF_OFFSET), INTF_CLIP0_H.u32); 
}

HI_VOID VDP_INTF_SetHShrpEnable   (HI_U32 u32Data, HI_BOOL hsp_en)
{
    U_INTF_HSPCFG1 INTF_HSPCFG1;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpEnable() select wrong intf ID\n");
        return;
    }

    INTF_HSPCFG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG1.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG1.bits.hsp_en = hsp_en;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG1.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG1.u32);
}

HI_VOID VDP_INTF_SetHShrpF0coef(HI_U32 u32Data, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3)
{
    U_INTF_HSPCFG0 INTF_HSPCFG0;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpF0coef() select wrong intf ID\n");
        return;
    }

    INTF_HSPCFG0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG0.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG0.bits.hsp_hf0_tmp3 = tmp3;
    INTF_HSPCFG0.bits.hsp_hf0_tmp2 = tmp2;
    INTF_HSPCFG0.bits.hsp_hf0_tmp1 = tmp1;
    INTF_HSPCFG0.bits.hsp_hf0_tmp0 = tmp0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG0.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG0.u32);
}

HI_VOID VDP_INTF_SetHShrpF1coef(HI_U32 u32Data, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3)
{
    U_INTF_HSPCFG7 INTF_HSPCFG7;
    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpF1coef() select wrong intf ID\n");
        return;
    } 

    INTF_HSPCFG7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG7.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG7.bits.hsp_hf1_tmp3 = tmp3;
    INTF_HSPCFG7.bits.hsp_hf1_tmp2 = tmp2;
    INTF_HSPCFG7.bits.hsp_hf1_tmp1 = tmp1;
    INTF_HSPCFG7.bits.hsp_hf1_tmp0 = tmp0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG7.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG7.u32);
}

HI_VOID VDP_INTF_SetHShrpCoring(HI_U32 u32Data, HI_U8 hf0_coring, HI_U8 hf1_coring)
{
    U_INTF_HSPCFG1 INTF_HSPCFG1;
    U_INTF_HSPCFG8 INTF_HSPCFG8;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpCoring() select wrong intf ID\n");
        return;
    } 

    INTF_HSPCFG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG1.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG8.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG1.bits.hsp_hf0_coring = hf0_coring;
    INTF_HSPCFG8.bits.hsp_hf1_coring = hf1_coring;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG1.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG8.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG8.u32);
}

HI_VOID VDP_INTF_SetHShrpGain(HI_U32 u32Data, HI_S16 hf0_gainneg, HI_S16 hf0_gainpos, HI_S16 hf1_gainneg, HI_S16 hf1_gainpos)
{
    U_INTF_HSPCFG5  INTF_HSPCFG5;
    U_INTF_HSPCFG12 INTF_HSPCFG12;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpGain() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG5.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG12.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG5.bits.hsp_hf0_gainneg  = hf0_gainneg;
    INTF_HSPCFG5.bits.hsp_hf0_gainpos  = hf0_gainpos;
    INTF_HSPCFG12.bits.hsp_hf1_gainneg = hf1_gainneg;
    INTF_HSPCFG12.bits.hsp_hf1_gainpos = hf1_gainpos;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG5.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG5.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG12.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG12.u32);
}

HI_VOID VDP_INTF_SetHShrpAdpEn(HI_U32 u32Data, HI_BOOL hf0_adpshooten, HI_BOOL hf1_adpshooten)
{
    U_INTF_HSPCFG6 INTF_HSPCFG6;
    U_INTF_HSPCFG13 INTF_HSPCFG13;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpAdpEn() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG6.bits.hsp_hf0_adpshoot_en  = hf0_adpshooten;
    INTF_HSPCFG13.bits.hsp_hf1_adpshoot_en = hf1_adpshooten;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG6.u32);
}

HI_VOID VDP_INTF_SetHShrpWsize(HI_U32 u32Data, HI_U8 hf0_winsize, HI_U8 hf1_winsize)
{
    U_INTF_HSPCFG6 INTF_HSPCFG6;
    U_INTF_HSPCFG13 INTF_HSPCFG13;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpWsize() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG6.bits.hsp_hf0_winsize     = hf0_winsize;
    INTF_HSPCFG13.bits.hsp_hf1_winsize    = hf1_winsize;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET),INTF_HSPCFG6.u32);
}

HI_VOID VDP_INTF_SetHShrpThrd(HI_U32 u32Data, HI_U8 hf0_underth, HI_U8 hf0_overth, HI_U8 hf1_underth, HI_U8 hf1_overth)
{
    U_INTF_HSPCFG6 INTF_HSPCFG6;
    U_INTF_HSPCFG13 INTF_HSPCFG13;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpThrd() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG6.bits.hsp_hf0_underth = hf0_underth;
    INTF_HSPCFG6.bits.hsp_hf0_overth  = hf0_overth;
    INTF_HSPCFG13.bits.hsp_hf1_underth = hf1_underth;
    INTF_HSPCFG13.bits.hsp_hf1_overth  = hf1_overth;


    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG6.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG13.u32);
}

HI_VOID VDP_INTF_SetHShrpMixratio(HI_U32 u32Data, HI_U8 hf0_mixratio, HI_U8 hf1_mixratio)
{
    U_INTF_HSPCFG6 INTF_HSPCFG6;
    U_INTF_HSPCFG13 INTF_HSPCFG13;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error, VDP_INTF_SetHShrpMixratio() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG6.bits.hsp_hf0_mixratio    = hf0_mixratio;
    INTF_HSPCFG13.bits.hsp_hf1_mixratio    = hf1_mixratio;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG6.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG6.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG13.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG13.u32);
}

HI_VOID VDP_INTF_SetHShrpEn(HI_U32 u32Data, HI_BOOL h1_en, HI_BOOL h2_en, HI_BOOL ltih_en, HI_BOOL ctih_en)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetHShrpEn() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG14.u32)+u32Data*INTF_OFFSET));
    INTF_HSPCFG14.bits.hsp_h0_en = h1_en;
    INTF_HSPCFG14.bits.hsp_h1_en = h2_en;
    INTF_HSPCFG14.bits.hsp_ltih_en = ltih_en;
    INTF_HSPCFG14.bits.hsp_ctih_en = ctih_en;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG14.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG14.u32);
}

HI_VOID VDP_INTF_SetHShrpXti(HI_U32 u32Data, HI_U8 hf_shootdiv, HI_U8 lti_ratio, HI_U8 ldti_gain, HI_U8 cdti_gain)
{
    U_INTF_HSPCFG14 INTF_HSPCFG14;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetHShrpXti() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG14.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG14.bits.hsp_hf_shootdiv = hf_shootdiv;
    INTF_HSPCFG14.bits.hsp_lti_ratio = lti_ratio;
    INTF_HSPCFG14.bits.hsp_ldti_gain = ldti_gain;
    INTF_HSPCFG14.bits.hsp_cdti_gain = cdti_gain;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG14.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG14.u32);
}

HI_VOID VDP_INTF_SetHShrpGlbpara(HI_U32 u32Data, HI_U8 peak_ratio, HI_U16 glb_overth, HI_U16 glb_underth)
{
    U_INTF_HSPCFG15 INTF_HSPCFG15;

    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetHShrpXti() select wrong intf ID\n");
        return;
    } 
    INTF_HSPCFG15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_HSPCFG15.u32)+u32Data*INTF_OFFSET));

    INTF_HSPCFG15.bits.hsp_peak_ratio = peak_ratio;
    INTF_HSPCFG15.bits.hsp_glb_overth = glb_overth;
    INTF_HSPCFG15.bits.hsp_glb_underth = glb_underth;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_HSPCFG15.u32)+u32Data*INTF_OFFSET), INTF_HSPCFG15.u32);
}

HI_VOID VDP_INTF_SetRegUp (HI_U32 u32Data)
{
    U_INTF_UPD INTF_UPD;
    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetIntfEnable Select Wrong interface ID\n");
        return ;
    }

    INTF_UPD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_UPD.u32)+u32Data*INTF_OFFSET));
    INTF_UPD.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_UPD.u32)+u32Data*INTF_OFFSET),INTF_UPD.u32);
}

HI_VOID VDP_INTF_SetHdmiMode  (HI_U32 u32Data, HI_U32 u32hdmi_md)
{
    U_INTF_CTRL INTF_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHdmiMode Select Wrong CHANNEL ID\n");
        return ;
    }

    INTF_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_CTRL.u32)+u32Data*INTF_OFFSET));
    INTF_CTRL.bits.hdmi_mode = u32hdmi_md;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_CTRL.u32)+u32Data*INTF_OFFSET),INTF_CTRL.u32);
}

HI_VOID VDP_INTF_SetSyncInv(HI_U32 u32Data, VDP_DISP_SYNCINV_S enInv)
{
    U_INTF_SYNC_INV INTF_SYNC_INV;
    if(u32Data >= INTF_MAX)
    {
        VDP_PRINT("Error,VDP_INTF_SetSync_INV Select Wrong INTF ID\n");
        return ;
    }

    INTF_SYNC_INV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->INTF_SYNC_INV.u32)+u32Data*INTF_OFFSET));
    INTF_SYNC_INV.bits.f_inv  = enInv.u32FdInv;
    INTF_SYNC_INV.bits.vs_inv = enInv.u32VsInv;
    INTF_SYNC_INV.bits.hs_inv = enInv.u32HsInv;
    INTF_SYNC_INV.bits.dv_inv = enInv.u32DvInv;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->INTF_SYNC_INV.u32)+u32Data*INTF_OFFSET),INTF_SYNC_INV.u32); 
}

HI_VOID VDP_DISP_SetHShrpEnable   (VDP_CHN_E enLayer, HI_BOOL hsp_en)
{
    U_DHD0_HSPCFG1 DHD0_HSPCFG1;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG1.u32)+CHN_OFFSET));

        DHD0_HSPCFG1.bits.hsp_en = hsp_en;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG1.u32)+CHN_OFFSET), DHD0_HSPCFG1.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpEnable() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpF0coef(VDP_CHN_E enLayer, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3)
{
    U_DHD0_HSPCFG0 DHD0_HSPCFG0;


    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG0.u32)+CHN_OFFSET));

        DHD0_HSPCFG0.bits.hsp_hf0_tmp3 = tmp3;
        DHD0_HSPCFG0.bits.hsp_hf0_tmp2 = tmp2;
        DHD0_HSPCFG0.bits.hsp_hf0_tmp1 = tmp1;
        DHD0_HSPCFG0.bits.hsp_hf0_tmp0 = tmp0;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG0.u32)+CHN_OFFSET), DHD0_HSPCFG0.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpF0coef() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetHShrpF1coef(VDP_CHN_E enLayer, HI_S16 tmp0, HI_S16 tmp1, HI_S16 tmp2, HI_S16 tmp3)
{
    U_DHD0_HSPCFG7 DHD0_HSPCFG7;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG7.u32)+CHN_OFFSET));

        DHD0_HSPCFG7.bits.hsp_hf1_tmp3 = tmp3;
        DHD0_HSPCFG7.bits.hsp_hf1_tmp2 = tmp2;
        DHD0_HSPCFG7.bits.hsp_hf1_tmp1 = tmp1;
        DHD0_HSPCFG7.bits.hsp_hf1_tmp0 = tmp0;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG7.u32)+CHN_OFFSET), DHD0_HSPCFG7.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpF1coef() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetHShrpCoring(VDP_CHN_E enLayer, HI_U8 hf0_coring, HI_U8 hf1_coring)
{
    U_DHD0_HSPCFG1 DHD0_HSPCFG1;
    U_DHD0_HSPCFG8 DHD0_HSPCFG8;
    if(enLayer ==  VDP_CHN_DHD1)
    {
        DHD0_HSPCFG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG1.u32)+CHN_OFFSET));
        DHD0_HSPCFG8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG8.u32)+CHN_OFFSET));

        DHD0_HSPCFG1.bits.hsp_hf0_coring = hf0_coring;
        DHD0_HSPCFG8.bits.hsp_hf1_coring = hf1_coring;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG1.u32)+CHN_OFFSET), DHD0_HSPCFG1.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG8.u32)+CHN_OFFSET), DHD0_HSPCFG8.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpCoring() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpGain(VDP_CHN_E enLayer, HI_S16 hf0_gainneg, HI_S16 hf0_gainpos, HI_S16 hf1_gainneg, HI_S16 hf1_gainpos)
{
    U_DHD0_HSPCFG5 DHD0_HSPCFG5;
    U_DHD0_HSPCFG12 DHD0_HSPCFG12;

    if(enLayer == VDP_CHN_DHD1)
    {

        DHD0_HSPCFG5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG5.u32)+CHN_OFFSET));
        DHD0_HSPCFG12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG12.u32)+CHN_OFFSET));

        DHD0_HSPCFG5.bits.hsp_hf0_gainneg  = hf0_gainneg;
        DHD0_HSPCFG5.bits.hsp_hf0_gainpos  = hf0_gainpos;
        DHD0_HSPCFG12.bits.hsp_hf1_gainneg = hf1_gainneg;
        DHD0_HSPCFG12.bits.hsp_hf1_gainpos = hf1_gainpos;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG5.u32)+CHN_OFFSET), DHD0_HSPCFG5.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG12.u32)+CHN_OFFSET), DHD0_HSPCFG12.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpGain() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetHShrpAdpEn(VDP_CHN_E enLayer, HI_BOOL hf0_adpshooten, HI_BOOL hf1_adpshooten)
{
    U_DHD0_HSPCFG6 DHD0_HSPCFG6;
    U_DHD0_HSPCFG13 DHD0_HSPCFG13;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET));
        DHD0_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET));

        DHD0_HSPCFG6.bits.hsp_hf0_adpshoot_en  = hf0_adpshooten;
        DHD0_HSPCFG13.bits.hsp_hf1_adpshoot_en = hf1_adpshooten;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET), DHD0_HSPCFG6.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET), DHD0_HSPCFG13.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpAdpEn() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetHShrpWsize(VDP_CHN_E enLayer, HI_U8 hf0_winsize, HI_U8 hf1_winsize)
{
    U_DHD0_HSPCFG6 DHD0_HSPCFG6;
    U_DHD0_HSPCFG13 DHD0_HSPCFG13;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET));
        DHD0_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET));

        DHD0_HSPCFG6.bits.hsp_hf0_winsize     = hf0_winsize;
        DHD0_HSPCFG13.bits.hsp_hf1_winsize    = hf1_winsize;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET), DHD0_HSPCFG6.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET), DHD0_HSPCFG13.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpWsize() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpThrd(VDP_CHN_E enLayer, HI_U8 hf0_underth, HI_U8 hf0_overth, HI_U8 hf1_underth, HI_U8 hf1_overth)
{
    U_DHD0_HSPCFG6 DHD0_HSPCFG6;
    U_DHD0_HSPCFG13 DHD0_HSPCFG13;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET));
        DHD0_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET));

        DHD0_HSPCFG6.bits.hsp_hf0_underth = hf0_underth;
        DHD0_HSPCFG6.bits.hsp_hf0_overth  = hf0_overth;
        DHD0_HSPCFG13.bits.hsp_hf1_underth = hf1_underth;
        DHD0_HSPCFG13.bits.hsp_hf1_overth  = hf1_overth;


        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET), DHD0_HSPCFG6.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET), DHD0_HSPCFG13.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpThrd() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetHShrpMixratio(VDP_CHN_E enLayer, HI_U8 hf0_mixratio, HI_U8 hf1_mixratio)
{
    U_DHD0_HSPCFG6 DHD0_HSPCFG6;
    U_DHD0_HSPCFG13 DHD0_HSPCFG13;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET));
        DHD0_HSPCFG13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET));

        DHD0_HSPCFG6.bits.hsp_hf0_mixratio    = hf0_mixratio;
        DHD0_HSPCFG13.bits.hsp_hf1_mixratio    = hf1_mixratio;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG6.u32)+CHN_OFFSET), DHD0_HSPCFG6.u32);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG13.u32)+CHN_OFFSET), DHD0_HSPCFG13.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpParam() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpEn(VDP_CHN_E enLayer, HI_BOOL h1_en, HI_BOOL h2_en, HI_BOOL ltih_en, HI_BOOL ctih_en)
{
    U_DHD0_HSPCFG14 DHD0_HSPCFG14;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG14.u32)+CHN_OFFSET));

        DHD0_HSPCFG14.bits.hsp_h0_en = h1_en;
        DHD0_HSPCFG14.bits.hsp_h1_en = h2_en;
        DHD0_HSPCFG14.bits.hsp_ltih_en = ltih_en;
        DHD0_HSPCFG14.bits.hsp_ctih_en = ctih_en;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG14.u32)+CHN_OFFSET), DHD0_HSPCFG14.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpEn() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpXti(VDP_CHN_E enLayer, HI_U8 hf_shootdiv, HI_U8 lti_ratio, HI_U8 ldti_gain, HI_U8 cdti_gain)
{
    U_DHD0_HSPCFG14 DHD0_HSPCFG14;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG14.u32)+CHN_OFFSET));

        DHD0_HSPCFG14.bits.hsp_hf_shootdiv = hf_shootdiv;
        DHD0_HSPCFG14.bits.hsp_lti_ratio = lti_ratio;
        DHD0_HSPCFG14.bits.hsp_ldti_gain = ldti_gain;
        DHD0_HSPCFG14.bits.hsp_cdti_gain = cdti_gain;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG14.u32)+CHN_OFFSET), DHD0_HSPCFG14.u32);

    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpXti() select wrong layer ID\n");
    }

}

HI_VOID VDP_DISP_SetHShrpGlbpara(VDP_CHN_E enLayer, HI_U8 peak_ratio, HI_U16 glb_overth, HI_U16 glb_underth)
{
    U_DHD0_HSPCFG15 DHD0_HSPCFG15;

    if(enLayer == VDP_CHN_DHD1)
    {
        DHD0_HSPCFG15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG15.u32)+CHN_OFFSET));

        DHD0_HSPCFG15.bits.hsp_peak_ratio = peak_ratio;
        DHD0_HSPCFG15.bits.hsp_glb_overth = glb_overth;
        DHD0_HSPCFG15.bits.hsp_glb_underth = glb_underth;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HSPCFG15.u32)+CHN_OFFSET), DHD0_HSPCFG15.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetHShrpReg15() select wrong layer ID\n");
    }
}

HI_VOID VDP_DISP_SetOneSyncTwoEnable   (HI_U32 u32Enable)
{
    U_VOCTRL VOCTRL;

    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.one_sync2_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_DISP_SetDhd1Select         (HI_U32 u32Data)
{
    U_VOCTRL VOCTRL;

    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.chn2_select = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_DISP_SetTriggerEnable         (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_DHD0_CTRL DHD0_CTRL;

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+CHN_OFFSET));
    DHD0_CTRL.bits.trigger_en = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+CHN_OFFSET), DHD0_CTRL.u32);
}
#endif

HI_VOID VDP_INTF_SetLvdsEnable(HI_U32 bEnable)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.lvds_en  = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32); 
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

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.mode = enMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);

    LVDS_PHYCTRL0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL0.u32)));
    LVDS_PHYCTRL0.bits.pon = enMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL0.u32)), LVDS_PHYCTRL0.u32);
}

HI_VOID VDP_INTF_SetLvdsBitMode(HI_BOOL bBitSel)
{
    U_LVDS_CTRL LVDS_CTRL;
    U_LVDS_PHYCTRL0 LVDS_PHYCTRL0;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.bit_sel = bBitSel;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);

    LVDS_PHYCTRL0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL0.u32)));
    LVDS_PHYCTRL0.bits.sel8_10 = ~bBitSel;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL0.u32)), LVDS_PHYCTRL0.u32);
}

HI_VOID VDP_INTF_SetLvdsSyncOutEn(HI_U32 bSyncOutEn)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.sync_out = bSyncOutEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsSwap(HI_U32 u32Swap)
{
    U_LVDS_SWAP0 LVDS_SWAP0;

    LVDS_SWAP0.u32 = u32Swap;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_SWAP0.u32)), LVDS_SWAP0.u32);
}

HI_VOID VDP_INTF_SetLvdsFormat(VDP_LVDS_FORMAT_E enFormat)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.lvds_map = enFormat;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsTestMode(VDP_LVDS_TEST_MODE_E enTestMode)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
    LVDS_CTRL.bits.test_mode = enTestMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}

HI_VOID VDP_INTF_SetLvdsInvert(VDP_LVDS_INV_E enInv, HI_U32 bInvEn)
{
    U_LVDS_CTRL LVDS_CTRL;

    LVDS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)));
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
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_CTRL.u32)), LVDS_CTRL.u32);
}


//WCG_BEGING
HI_VOID VDP_DISP_SetWcgEnable  (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_DHDWCG_CSC2CTRL  DHDWCG_CSC2CTRL;
	U_SR_CSC1_ODC     SR_CSC1_ODC;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DHD_SetWcgEnable() Select Wrong DHD ID\n");
		return ;
	}
	
	DHDWCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2CTRL.bits.csc2_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2CTRL.u32);
	  
	//set csc sign mode
	SR_CSC1_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_ODC.u32)+ u32Data * CHN_OFFSET));
	SR_CSC1_ODC.bits.csc_sign_mode = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_ODC.u32)+ u32Data * CHN_OFFSET), SR_CSC1_ODC.u32);

}

HI_VOID VDP_DISP_SetWcgCoef  (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef)
{

	U_DHDWCG_NON2LCTRL     DHDWCG_NON2LCTRL;
	U_DHDWCG_NON2LCOEF0    DHDWCG_NON2LCOEF0;
	U_DHDWCG_NON2LCOEF1    DHDWCG_NON2LCOEF1;
	U_DHDWCG_NON2LCOEF2    DHDWCG_NON2LCOEF2;

	U_DHDWCG_CSC2CTRL      DHDWCG_CSC2CTRL;
	U_DHDWCG_CSC2COEF0     DHDWCG_CSC2COEF0;
	U_DHDWCG_CSC2COEF1     DHDWCG_CSC2COEF1;
	U_DHDWCG_CSC2COEF2     DHDWCG_CSC2COEF2;
	U_DHDWCG_CSC2COEF3     DHDWCG_CSC2COEF3;

	U_DHDWCG_L2NONTHD      DHDWCG_L2NONTHD;
	U_DHDWCG_L2NONCOEF0    DHDWCG_L2NONCOEF0;
	U_DHDWCG_L2NONCOEF1    DHDWCG_L2NONCOEF1;
	U_DHDWCG_L2NONCOEF2    DHDWCG_L2NONCOEF2;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DHD_SetWcgCoef() Select Wrong DHD ID\n");
		return ;
	}

	DHDWCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2CTRL.bits.csc2_scale_sel = pstWcgCoef->csc2_scale_sel;
	DHDWCG_CSC2CTRL.bits.csc2_r_r_coef  = (pstWcgCoef->csc2_r_r_coef);
	DHDWCG_CSC2CTRL.bits.csc2_r_g_coef  = (pstWcgCoef->csc2_r_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2CTRL.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2CTRL.u32);

	DHDWCG_CSC2COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF0.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2COEF0.bits.csc2_r_b_coef  = (pstWcgCoef->csc2_r_b_coef);
	DHDWCG_CSC2COEF0.bits.csc2_g_r_coef  = (pstWcgCoef->csc2_g_r_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF0.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2COEF0.u32);

	DHDWCG_CSC2COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF1.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2COEF1.bits.csc2_g_g_coef  = (pstWcgCoef->csc2_g_g_coef);
	DHDWCG_CSC2COEF1.bits.csc2_g_b_coef  = (pstWcgCoef->csc2_g_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF1.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2COEF1.u32);

	DHDWCG_CSC2COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF2.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2COEF2.bits.csc2_b_r_coef  = (pstWcgCoef->csc2_b_r_coef);
	DHDWCG_CSC2COEF2.bits.csc2_b_g_coef  = (pstWcgCoef->csc2_b_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF2.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2COEF2.u32);

	DHDWCG_CSC2COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF3.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_CSC2COEF3.bits.csc2_b_b_coef  = (pstWcgCoef->csc2_b_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_CSC2COEF3.u32)+ u32Data * CHN_OFFSET), DHDWCG_CSC2COEF3.u32);

	//-------------------------

	DHDWCG_NON2LCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCTRL.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_NON2LCTRL.bits.gma_lin_neg_scl  = pstWcgCoef->gma_lin_neg_scl;
	DHDWCG_NON2LCTRL.bits.gma_lin_init_slp = pstWcgCoef->gma_lin_init_slp;
	DHDWCG_NON2LCTRL.bits.gma_lin_thre_0   = pstWcgCoef->gma_lin_thre_0;
	DHDWCG_NON2LCTRL.bits.gma_lin_thre_1   = pstWcgCoef->gma_lin_thre_1;
	DHDWCG_NON2LCTRL.bits.gma_lin_thre_2   = pstWcgCoef->gma_lin_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCTRL.u32)+ u32Data * CHN_OFFSET), DHDWCG_NON2LCTRL.u32);

	DHDWCG_NON2LCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF0.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_NON2LCOEF0.bits.gma_lin_coef_a_0  = pstWcgCoef->gma_lin_coef_a_0;
	DHDWCG_NON2LCOEF0.bits.gma_lin_coef_b_0  = pstWcgCoef->gma_lin_coef_b_0;
	DHDWCG_NON2LCOEF0.bits.gma_lin_coef_c_0  = pstWcgCoef->gma_lin_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF0.u32)+ u32Data * CHN_OFFSET), DHDWCG_NON2LCOEF0.u32);

	DHDWCG_NON2LCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF1.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_NON2LCOEF1.bits.gma_lin_coef_a_1  = pstWcgCoef->gma_lin_coef_a_1;
	DHDWCG_NON2LCOEF1.bits.gma_lin_coef_b_1  = pstWcgCoef->gma_lin_coef_b_1;
	DHDWCG_NON2LCOEF1.bits.gma_lin_coef_c_1  = pstWcgCoef->gma_lin_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF1.u32)+ u32Data * CHN_OFFSET), DHDWCG_NON2LCOEF1.u32);

	DHDWCG_NON2LCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF2.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_NON2LCOEF2.bits.gma_lin_coef_a_2  = pstWcgCoef->gma_lin_coef_a_2;
	DHDWCG_NON2LCOEF2.bits.gma_lin_coef_b_2  = pstWcgCoef->gma_lin_coef_b_2;
	DHDWCG_NON2LCOEF2.bits.gma_lin_coef_c_2  = pstWcgCoef->gma_lin_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_NON2LCOEF2.u32)+ u32Data * CHN_OFFSET), DHDWCG_NON2LCOEF2.u32);

	DHDWCG_L2NONTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONTHD.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_L2NONTHD.bits.lin_gma_thre_0  = pstWcgCoef->lin_gma_thre_0;
	DHDWCG_L2NONTHD.bits.lin_gma_thre_1  = pstWcgCoef->lin_gma_thre_1;
	DHDWCG_L2NONTHD.bits.lin_gma_thre_2  = pstWcgCoef->lin_gma_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONTHD.u32)+ u32Data * CHN_OFFSET), DHDWCG_L2NONTHD.u32);

	DHDWCG_L2NONCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF0.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_L2NONCOEF0.bits.lin_gma_coef_a_0  = pstWcgCoef->lin_gma_coef_a_0;
	DHDWCG_L2NONCOEF0.bits.lin_gma_coef_b_0  = pstWcgCoef->lin_gma_coef_b_0;
	DHDWCG_L2NONCOEF0.bits.lin_gma_coef_c_0  = pstWcgCoef->lin_gma_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF0.u32)+ u32Data * CHN_OFFSET), DHDWCG_L2NONCOEF0.u32);

	DHDWCG_L2NONCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF1.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_L2NONCOEF1.bits.lin_gma_coef_a_1  = pstWcgCoef->lin_gma_coef_a_1;
	DHDWCG_L2NONCOEF1.bits.lin_gma_coef_b_1  = pstWcgCoef->lin_gma_coef_b_1;
	DHDWCG_L2NONCOEF1.bits.lin_gma_coef_c_1  = pstWcgCoef->lin_gma_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF1.u32)+ u32Data * CHN_OFFSET), DHDWCG_L2NONCOEF1.u32);

	DHDWCG_L2NONCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF2.u32)+ u32Data * CHN_OFFSET));
	DHDWCG_L2NONCOEF2.bits.lin_gma_coef_a_2  = pstWcgCoef->lin_gma_coef_a_2;
	DHDWCG_L2NONCOEF2.bits.lin_gma_coef_b_2  = pstWcgCoef->lin_gma_coef_b_2;
	DHDWCG_L2NONCOEF2.bits.lin_gma_coef_c_2  = pstWcgCoef->lin_gma_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDWCG_L2NONCOEF2.u32)+ u32Data * CHN_OFFSET), DHDWCG_L2NONCOEF2.u32);


}

#if SR_EN
//------------------------------------------------
//SR_BEGING 
//-------------------------------------------------
HI_VOID VDP_DISP_SetSrEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_SR_CTRL  SR_CTRL;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrEnable() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_CTRL.bits.sr_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrDspSta (HI_U32 u32Data, VDP_SR_MODE_E enMd)
{
	U_SR_CTRL  SR_CTRL;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrDspSta() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_CTRL.bits.sr_mode = enMd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}
#if 0
HI_VOID VDP_DISP_SetSrMux (HI_U32 u32Data, VDP_SR_MUX_E SrMux)
{
	U_SR_CTRL  SR_CTRL;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrMux() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_CTRL.bits.sr_zme_en = SrMux;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrDemoCtrl (HI_U32 u32Data, VDP_SR_DEMO_E DemoCtrl)
{
	U_SR_CTRL  SR_CTRL;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_VID_SetSrMux() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_CTRL.bits.demo_ctrl = DemoCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrServeMode (HI_U32 u32Data, VDP_SR_SERVE_MODE_E Mode)
{
	U_SR_CTRL  SR_CTRL;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrServeMode() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_CTRL.bits.serve_mode = Mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CTRL.u32) + u32Data * CHN_OFFSET), SR_CTRL.u32); 
}
#endif

HI_VOID VDP_DISP_SetSrGrdSubRatio      (HI_U32 u32Data, HI_U32 u32GrdSubRatio)
{
	U_SR_LDIR_RATIO SR_LDIR_RATIO;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGrdSubRatio() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_LDIR_RATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET));
	SR_LDIR_RATIO.bits.grd_sub_ratio = u32GrdSubRatio;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET), SR_LDIR_RATIO.u32); 
}

HI_VOID VDP_DISP_SetSrGrdHvNear      (HI_U32 u32Data, HI_U32 u32GrdHvNear)
{
	U_SR_LDIR_RATIO SR_LDIR_RATIO;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGrdSubRatio() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_LDIR_RATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET));
	SR_LDIR_RATIO.bits.grd_hv_near = u32GrdHvNear;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_LDIR_RATIO.u32) + u32Data * CHN_OFFSET), SR_LDIR_RATIO.u32); 
}


HI_VOID VDP_DISP_SetSrGrdThr (HI_U32 u32Data, HI_U32 u32LowThr, HI_U32 u32HighThr)
{
	U_SR_LTEX_THR SR_LTEX_THR;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGrdThr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_LTEX_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET));
	SR_LTEX_THR.bits.grd_thrh = u32HighThr;
	SR_LTEX_THR.bits.grd_thrl = u32LowThr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET), SR_LTEX_THR.u32); 
}

HI_VOID VDP_DISP_SetSrGrdThrSub (HI_U32 u32Data, HI_U32 u32GrdThrSub)
{
	U_SR_LTEX_ADD SR_LTEX_ADD;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGrdThrSub() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_LTEX_ADD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET));
	SR_LTEX_ADD.bits.grd_thrh_subl = u32GrdThrSub;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET), SR_LTEX_ADD.u32); 
}

HI_VOID VDP_DISP_SetSrLtexAdd (HI_U32 u32Data, HI_U32 u32LtexAdd)
{
	U_SR_LTEX_ADD  SR_LTEX_ADD;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrLtexAdd() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_LTEX_ADD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET));
	SR_LTEX_ADD.bits.grd_add_ctrl = u32LtexAdd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_LTEX_ADD.u32) + u32Data * CHN_OFFSET), SR_LTEX_ADD.u32); 
}

HI_VOID VDP_DISP_SetSrDiscDetailGain (HI_U32 u32Data, HI_U32 u32DetaiGain)
{
	U_SR_DIPL_DETAIL  SR_DIPL_DETAIL;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrDiscDetailGain() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_DIPL_DETAIL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_DIPL_DETAIL.u32) + u32Data * CHN_OFFSET));
	SR_DIPL_DETAIL.bits.disc_detail_gain = u32DetaiGain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_DIPL_DETAIL.u32) + u32Data * CHN_OFFSET), SR_DIPL_DETAIL.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_SR_SHARP_EN  SR_SHARP_EN;	

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpenEnable() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_EN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_EN.bits.sharp_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}
#if 0

HI_VOID VDP_DISP_SetSrSharpenMode (HI_U32 u32Data, VDP_SR_SHARP_MODE_E Mode)
{
	U_SR_SHARP_EN  SR_SHARP_EN;	

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpenMode() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_EN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_EN.bits.hsh_nodir = Mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}

HI_VOID VDP_DISP_SetSrEiscDetailGain (HI_U32 u32Data, HI_U32 u32DetaiGain)
{
	U_SR_SHARP_EN  SR_SHARP_EN;	

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrEiscDetailGain() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_EN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_EN.bits.ensc_detail_gain = u32DetaiGain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_EN.u32) + u32Data * CHN_OFFSET), SR_SHARP_EN.u32); 
}
#endif

HI_VOID VDP_DISP_SetSrSharpenCoring (HI_U32 u32Data, HI_U32 u32SharpCoring)
{
	U_SR_SHARP_THR  SR_SHARP_THR;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpenCoring() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_THR.bits.sharp_coring = u32SharpCoring;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}

HI_VOID VDP_DISP_SetSrSharpenGain (HI_U32 u32Data, HI_U32 u32SharpGain)
{
	U_SR_SHARP_THR  SR_SHARP_THR;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpenGain() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_THR.bits.sharp_gain = u32SharpGain;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}

#if 0
HI_VOID VDP_DISP_SetSrSharpenThrsh (HI_U32 u32Data, HI_U32 u32HighThr, HI_U32 u32LowThr)
{
	U_SR_SHARP_THR  SR_SHARP_THR;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpenGain() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SHARP_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET));
	SR_SHARP_THR.bits.over_thrsh = u32HighThr;
	SR_SHARP_THR.bits.under_thrsh = u32LowThr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_SHARP_THR.u32); 
}
#endif

HI_VOID VDP_DISP_SetSrSmoothEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_SR_SMOOTH  SR_SMOOTH;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSmoothEnable() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SMOOTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
	SR_SMOOTH.bits.smooth_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}

HI_VOID VDP_DISP_SetSrSmoothPlusEnable (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_SR_SMOOTH  SR_SMOOTH;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSmoothPlusEnable() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SMOOTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
	SR_SMOOTH.bits.plus_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}

HI_VOID VDP_DISP_SetSrSmoothPlusCtrl (HI_U32 u32Data, HI_U32 u32PlusCtrl)
{
	U_SR_SMOOTH  SR_SMOOTH;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSmoothPlusCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SMOOTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET));
	SR_SMOOTH.bits.plus_ctrl = u32PlusCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SMOOTH.u32) + u32Data * CHN_OFFSET), SR_SMOOTH.u32); 
}

HI_VOID VDP_DISP_SetSrDirCtrl (HI_U32 u32Data, HI_U32 u32DirHvCtrl,HI_U32 u32DirDgCtrl)
{
	U_SR_DIR_CTRL SR_DIR_CTRL ;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrDirCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_DIR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_DIR_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_DIR_CTRL.bits.dir_hv_ctrl = u32DirHvCtrl;
	SR_DIR_CTRL.bits.dir_dg_ctrl = u32DirDgCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_DIR_CTRL.u32) + u32Data * CHN_OFFSET), SR_DIR_CTRL.u32); 
}


//SR_ZME
HI_VOID VDP_DISP_SetSrZmeEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
	U_SR_VSP SR_VSP;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hlmsc_en = u32bEnable;
        SR_HSP.bits.hlfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hchmsc_en = u32bEnable;
        SR_HSP.bits.hchfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vlmsc_en = u32bEnable;
        SR_VSP.bits.vlfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vchmsc_en = u32bEnable;
        SR_VSP.bits.vchfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
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
        VDP_PRINT("Error,VDP_DISP_SetSrZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HLOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HLOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_HLOFFSET.bits.hor_loffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HLOFFSET.u32) + u32Data * CHN_OFFSET), SR_HLOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HCOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_HCOFFSET.bits.hor_coffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOFFSET.u32) + u32Data * CHN_OFFSET), SR_HCOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VOFFSET.bits.vluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET), SR_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VOFFSET.bits.vchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VOFFSET.u32) + u32Data * CHN_OFFSET), SR_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VBOFFSET.bits.vbluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET), SR_VBOFFSET.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET));
        SR_VBOFFSET.bits.vbchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VBOFFSET.u32) + u32Data * CHN_OFFSET), SR_VBOFFSET.u32); 
    }

    return ;
}
    

HI_VOID VDP_DISP_SetSrZmeFirEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }
            
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
         SR_HSP.bits.hlfir_en = u32bEnable;
         VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
     }
     
     if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
         SR_HSP.bits.hchfir_en = u32bEnable;
         VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
     }

     if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
         SR_VSP.bits.vlfir_en = u32bEnable;
         VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
     }
     
     if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
         SR_VSP.bits.vchfir_en = u32bEnable;
         VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
     }

    return ;
}

    
HI_VOID VDP_DISP_SetSrZmeMidEnable (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_SR_HSP SR_HSP;
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hlmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
        SR_HSP.bits.hchmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vlmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
        SR_VSP.bits.vchmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 
    }

    return ;
}

HI_VOID VDP_DISP_SetSrZmeAlphaFirMode (HI_U32 u32Data, HI_U32 u32HFirMode, HI_U32 u32VFirMode)
{
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeAlphaFirMode() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
    SR_VSP.bits.avfir_mode = u32VFirMode;
    SR_VSP.bits.ahfir_mode = u32HFirMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 

    return ;
}
    
    
HI_VOID VDP_DISP_SetSrZmeHorRatio (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_HSP SR_HSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
    SR_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 

    return ;
}

    
HI_VOID VDP_DISP_SetSrZmeVerRatio (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_SR_VSR SR_VSR;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_VSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSR.u32) + u32Data * CHN_OFFSET));
    SR_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSR.u32) + u32Data * CHN_OFFSET), SR_VSR.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrZmeInFmt (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
    SR_VSP.bits.zme_in_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrZmeOutFmt (HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_SR_VSP SR_VSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeOutFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    SR_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET));
    SR_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VSP.u32) + u32Data * CHN_OFFSET), SR_VSP.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_SR_HSP SR_HSP;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }
    
    SR_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET));
    SR_HSP.bits.hfir_order = u32HfirOrder;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HSP.u32) + u32Data * CHN_OFFSET), SR_HSP.u32); 

    return ;
}

HI_VOID VDP_DISP_SetSrGradThres (HI_U32 u32Data, HI_U32 u32GradThresH, HI_U32 u32GradThresL)
{
	U_SR_RINGDT0 SR_RINGDT0;
	
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGradThres() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_RINGDT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET));
	SR_RINGDT0.bits.grad_thres_h = u32GradThresH;
	SR_RINGDT0.bits.grad_thres_l = u32GradThresL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET), SR_RINGDT0.u32); 
}

HI_VOID VDP_DISP_SetSrGradFlatThr (HI_U32 u32Data, HI_U32 u32GradFlatThr)
{
	U_SR_RINGDT0 SR_RINGDT0;
	
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGradFlatThr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_RINGDT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET));
	SR_RINGDT0.bits.grad_flat_thr = u32GradFlatThr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET), SR_RINGDT0.u32); 
}

HI_VOID VDP_DISP_SetSrRingdtEn (HI_U32 u32Data, HI_U32 u32RingdtEn)
{
	U_SR_RINGDT0 SR_RINGDT0;
	
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrRingdtEn() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_RINGDT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET));
	SR_RINGDT0.bits.ringdt_en = u32RingdtEn;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_RINGDT0.u32) + u32Data * CHN_OFFSET), SR_RINGDT0.u32); 
}

HI_VOID VDP_DISP_SetSrGradFlatThr2 (HI_U32 u32Data, HI_U32 u32GradFlatThr2)
{
	U_SR_RINGDT1 SR_RINGDT1;
	
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGradFlatThr2() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_RINGDT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_RINGDT1.u32) + u32Data * CHN_OFFSET));
	SR_RINGDT1.bits.grad_flat_thr2 = u32GradFlatThr2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_RINGDT1.u32) + u32Data * CHN_OFFSET), SR_RINGDT1.u32); 
}

HI_VOID VDP_DISP_SetSrGradSlop (HI_U32 u32Data, HI_U32 u32GradSlop)
{
	U_SR_RINGDT1 SR_RINGDT1;
	
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrGradFlatThr2() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_RINGDT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_RINGDT1.u32) + u32Data * CHN_OFFSET));
	SR_RINGDT1.bits.grad_slop = u32GradSlop;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_RINGDT1.u32) + u32Data * CHN_OFFSET), SR_RINGDT1.u32); 
}

HI_VOID VDP_DISP_SetSrSkindtEn (HI_U32 u32Data, HI_U32 u32SkindtEn)
{
    U_SR_SKINDT_CTRL SR_SKINDT_CTRL;	
    
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkindtEn() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_CTRL.bits.skindt_en = u32SkindtEn;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_CTRL.u32) + u32Data * CHN_OFFSET), SR_SKINDT_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrClipData (HI_U32 u32Data, HI_U32 u32ClipData)
{
    U_SR_SKINDT_CTRL SR_SKINDT_CTRL;	
    
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrClipData() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_CTRL.bits.clip_data = u32ClipData;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_CTRL.u32) + u32Data * CHN_OFFSET), SR_SKINDT_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrSkindtExp (HI_U32 u32Data, HI_U32 u32SkindtExpY, HI_U32 u32SkindtExpU, HI_U32 u32SkindtExpV)
{
    U_SR_SKINDT_EXP SR_SKINDT_EXP;
    
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkindtExp() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_EXP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_EXP.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_EXP.bits.y_expect_value = u32SkindtExpY;
	SR_SKINDT_EXP.bits.u_expect_value = u32SkindtExpU;
	SR_SKINDT_EXP.bits.v_expect_value = u32SkindtExpV;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_EXP.u32) + u32Data * CHN_OFFSET), SR_SKINDT_EXP.u32); 
}

HI_VOID VDP_DISP_SetSrSkindtYthr (HI_U32 u32Data, HI_U32 u32YThresH, HI_U32 u32YThresL, HI_U32 u32YSlop)
{
    U_SR_SKINDT_YTHR SR_SKINDT_YTHR;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkindtYthr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_YTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_YTHR.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_YTHR.bits.y_thres_h = u32YThresH;
	SR_SKINDT_YTHR.bits.y_thres_l = u32YThresL;
	SR_SKINDT_YTHR.bits.y_slop = u32YSlop;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_YTHR.u32) + u32Data * CHN_OFFSET), SR_SKINDT_YTHR.u32); 
}

HI_VOID VDP_DISP_SetSrSkindtUthr (HI_U32 u32Data, HI_U32 u32UThresH, HI_U32 u32UThresL, HI_U32 u32USlop)
{
    U_SR_SKINDT_UTHR SR_SKINDT_UTHR;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkindtUthr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_UTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_UTHR.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_UTHR.bits.u_thres_h = u32UThresH;
	SR_SKINDT_UTHR.bits.u_thres_l = u32UThresL;
	SR_SKINDT_UTHR.bits.u_slop = u32USlop;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_UTHR.u32) + u32Data * CHN_OFFSET), SR_SKINDT_UTHR.u32); 
}

HI_VOID VDP_DISP_SetSrSkindtVthr (HI_U32 u32Data, HI_U32 u32VThresH, HI_U32 u32VThresL, HI_U32 u32VSlop)
{
    U_SR_SKINDT_VTHR SR_SKINDT_VTHR;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkindtVthr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_SKINDT_VTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_SKINDT_VTHR.u32) + u32Data * CHN_OFFSET));
	SR_SKINDT_VTHR.bits.v_thres_h = u32VThresH;
	SR_SKINDT_VTHR.bits.v_thres_l = u32VThresL;
	SR_SKINDT_VTHR.bits.v_slop = u32VSlop;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_SKINDT_VTHR.u32) + u32Data * CHN_OFFSET), SR_SKINDT_VTHR.u32); 
}

HI_VOID VDP_DISP_SetSrHanceSlop (HI_U32 u32Data, HI_U32 u32HighSlop, HI_U32 u32LowSlop)
{
    U_SR_HANCE_SLOP SR_HANCE_SLOP;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrHanceSlop() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_SLOP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_SLOP.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_SLOP.bits.sharp_high_slop   = u32HighSlop;
	SR_HANCE_SLOP.bits.sharp_low_slop    = u32LowSlop ;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_SLOP.u32) + u32Data * CHN_OFFSET), SR_HANCE_SLOP.u32); 
}

HI_VOID VDP_DISP_SetSrSharpThr (HI_U32 u32Data, HI_U32 u32SrSharpThrH, HI_U32 u32SrSharpThrL)
{
    U_SR_HANCE_SHARP_THR SR_HANCE_SHARP_THR;
    
    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpThr() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_SHARP_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_SHARP_THR.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_SHARP_THR.bits.sharp_thres_h = u32SrSharpThrH;
	SR_HANCE_SHARP_THR.bits.sharp_thres_l = u32SrSharpThrL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_SHARP_THR.u32) + u32Data * CHN_OFFSET), SR_HANCE_SHARP_THR.u32); 
}

HI_VOID VDP_DISP_SetSrHanceLowCtrl (HI_U32 u32Data, HI_U32 u32LowCtrlH, HI_U32 u32LowCtrlL)
{
    U_SR_HANCE_LOW_CTRL SR_HANCE_LOW_CTRL;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrHanceLowCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_LOW_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_LOW_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_LOW_CTRL.bits.sharp_low_ctrl_h = u32LowCtrlH;
	SR_HANCE_LOW_CTRL.bits.sharp_low_ctrl_l = u32LowCtrlL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_LOW_CTRL.u32) + u32Data * CHN_OFFSET), SR_HANCE_LOW_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrHanceHighCtrl (HI_U32 u32Data, HI_U32 u32HighCtrlH, HI_U32 u32HighCtrlL)
{
    U_SR_HANCE_HIGH_CTRL SR_HANCE_HIGH_CTRL;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrHanceHighCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_HIGH_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_HIGH_CTRL.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_HIGH_CTRL.bits.sharp_high_ctrl_h = u32HighCtrlH;
	SR_HANCE_HIGH_CTRL.bits.sharp_high_ctrl_l = u32HighCtrlL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_HIGH_CTRL.u32) + u32Data * CHN_OFFSET), SR_HANCE_HIGH_CTRL.u32); 
}

HI_VOID VDP_DISP_SetSrSharpStart (HI_U32 u32Data, HI_U32 u32SharpStartH, HI_U32 u32SharpStartL)
{
    U_SR_HANCE_THR0 SR_HANCE_THR0;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpStart() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR0.bits.sharp_high_start = u32SharpStartH;
	SR_HANCE_THR0.bits.sharp_low_start = u32SharpStartL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR0.u32); 
}

HI_VOID VDP_DISP_SetSrSharpStrengh (HI_U32 u32Data, HI_U32 u32SharpStrengh)
{
    U_SR_HANCE_THR0 SR_HANCE_THR0;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpStrengh() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR0.bits.strengh = u32SharpStrengh;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR0.u32); 
}

HI_VOID VDP_DISP_SetSrEdgeFactorCtrl (HI_U32 u32Data, HI_U32 u32EdgeFactorCtrl)
{
    U_SR_HANCE_THR0 SR_HANCE_THR0;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrEdgeFactorCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR0.bits.edge_factor_ctrl = u32EdgeFactorCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR0.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR0.u32); 
}

HI_VOID VDP_DISP_SetSrSkinThres (HI_U32 u32Data, HI_U32 u32SkinThresH, HI_U32 u32SkinThresL)
{
    U_SR_HANCE_THR1 SR_HANCE_THR1;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSkinThres() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR1.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR1.bits.skin_thres_h = u32SkinThresH;
	SR_HANCE_THR1.bits.skin_thres_l = u32SkinThresL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR1.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR1.u32); 
}

HI_VOID VDP_DISP_SetSrRingThres (HI_U32 u32Data, HI_U32 u32RingThresH, HI_U32 u32RingThresL)
{
    U_SR_HANCE_THR1 SR_HANCE_THR1;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrRingThres() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR1.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR1.bits.ring_thres_h = u32RingThresH;
	SR_HANCE_THR1.bits.ring_thres_l = u32RingThresL;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR1.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR1.u32); 
}

HI_VOID VDP_DISP_SetSrSharpHighCtrl (HI_U32 u32Data, HI_U32 u32SharpHighCtrl)
{
    U_SR_HANCE_THR2 SR_HANCE_THR2;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrSharpHighCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR2.bits.sharp_high_ctrl = u32SharpHighCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR2.u32); 
}

HI_VOID VDP_DISP_SetSrNolinearCtrl (HI_U32 u32Data, HI_U32 u32NolinearCtrl)
{
    U_SR_HANCE_THR2 SR_HANCE_THR2;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrNolinearCtrl() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR2.bits.nolinear_ctrl = u32NolinearCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR2.u32); 
}

HI_VOID VDP_DISP_SetSrShootThrsh (HI_U32 u32Data, HI_U32 u32ShootThrsh)
{
    U_SR_HANCE_THR2 SR_HANCE_THR2;

    if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrShootThrsh() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HANCE_THR2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET));
	SR_HANCE_THR2.bits.shoot_thrsh = u32ShootThrsh;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HANCE_THR2.u32) + u32Data * CHN_OFFSET), SR_HANCE_THR2.u32); 
}
#if 0
HI_VOID VDP_DISP_SetSrHorCoef (HI_U32 u32Data, VDP_SR_HORCOEF_S stSrHorCoef)
{
    U_SR_HCOEF0    SR_HCOEF0;
    U_SR_HCOEF1    SR_HCOEF1;
    U_SR_HCOEF2    SR_HCOEF2;
    U_SR_HCOEF3    SR_HCOEF3;
    U_SR_HCOEF4    SR_HCOEF4;
    U_SR_HCOEF5    SR_HCOEF5;
    U_SR_HCOEF6    SR_HCOEF6;
    U_SR_HCOEF7    SR_HCOEF7;
    U_SR_HCOEF8    SR_HCOEF8;
    U_SR_HCOEF9    SR_HCOEF9;
    U_SR_HCOEF10   SR_HCOEF10;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrHorCoef() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_HCOEF0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF0.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF1.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF2.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF2.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF3.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF3.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF4.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF4.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF5.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF5.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF6.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF6.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF7.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF7.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF8.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF8.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF9.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF9.u32)  + u32Data * CHN_OFFSET));
	SR_HCOEF10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_HCOEF10.u32) + u32Data * CHN_OFFSET));

	SR_HCOEF0.bits.hlcoef00 = stSrHorCoef.sr_hlcoef00;
	SR_HCOEF0.bits.hlcoef01 = stSrHorCoef.sr_hlcoef01;
	SR_HCOEF0.bits.hlcoef02 = stSrHorCoef.sr_hlcoef02;

	SR_HCOEF1.bits.hlcoef03 = stSrHorCoef.sr_hlcoef03;
	SR_HCOEF1.bits.hlcoef04 = stSrHorCoef.sr_hlcoef04;
	SR_HCOEF1.bits.hlcoef05 = stSrHorCoef.sr_hlcoef05;

	SR_HCOEF2.bits.hlcoef06 = stSrHorCoef.sr_hlcoef06;
	SR_HCOEF2.bits.hlcoef07 = stSrHorCoef.sr_hlcoef07;
	SR_HCOEF2.bits.hlcoef10 = stSrHorCoef.sr_hlcoef10;

	SR_HCOEF3.bits.hlcoef11 = stSrHorCoef.sr_hlcoef11;
	SR_HCOEF3.bits.hlcoef12 = stSrHorCoef.sr_hlcoef12;
	SR_HCOEF3.bits.hlcoef13 = stSrHorCoef.sr_hlcoef13;

	SR_HCOEF4.bits.hlcoef14 = stSrHorCoef.sr_hlcoef14;
	SR_HCOEF4.bits.hlcoef15 = stSrHorCoef.sr_hlcoef15;
	SR_HCOEF4.bits.hlcoef16 = stSrHorCoef.sr_hlcoef16;

	SR_HCOEF5.bits.hlcoef17 = stSrHorCoef.sr_hlcoef17;
	SR_HCOEF5.bits.hccoef00 = stSrHorCoef.sr_hccoef00;
	SR_HCOEF5.bits.hccoef01 = stSrHorCoef.sr_hccoef01;

	SR_HCOEF6.bits.hccoef02 = stSrHorCoef.sr_hccoef02;
	SR_HCOEF6.bits.hccoef03 = stSrHorCoef.sr_hccoef03;
	SR_HCOEF6.bits.hccoef04 = stSrHorCoef.sr_hccoef04;

	SR_HCOEF7.bits.hccoef05 = stSrHorCoef.sr_hccoef05;
	SR_HCOEF7.bits.hccoef06 = stSrHorCoef.sr_hccoef06;
	SR_HCOEF7.bits.hccoef07 = stSrHorCoef.sr_hccoef07;

	SR_HCOEF8.bits.hccoef10 = stSrHorCoef.sr_hccoef10;
	SR_HCOEF8.bits.hccoef11 = stSrHorCoef.sr_hccoef11;
	SR_HCOEF8.bits.hccoef12 = stSrHorCoef.sr_hccoef12;

	SR_HCOEF9.bits.hccoef13 = stSrHorCoef.sr_hccoef13;
	SR_HCOEF9.bits.hccoef14 = stSrHorCoef.sr_hccoef14;
	SR_HCOEF9.bits.hccoef15 = stSrHorCoef.sr_hccoef15;

	SR_HCOEF10.bits.hccoef16 = stSrHorCoef.sr_hccoef16;
	SR_HCOEF10.bits.hccoef17 = stSrHorCoef.sr_hccoef17;

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF0.u32)  + u32Data * CHN_OFFSET), SR_HCOEF0.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF1.u32)  + u32Data * CHN_OFFSET), SR_HCOEF1.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF2.u32)  + u32Data * CHN_OFFSET), SR_HCOEF2.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF3.u32)  + u32Data * CHN_OFFSET), SR_HCOEF3.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF4.u32)  + u32Data * CHN_OFFSET), SR_HCOEF4.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF5.u32)  + u32Data * CHN_OFFSET), SR_HCOEF5.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF6.u32)  + u32Data * CHN_OFFSET), SR_HCOEF6.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF7.u32)  + u32Data * CHN_OFFSET), SR_HCOEF7.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF8.u32)  + u32Data * CHN_OFFSET), SR_HCOEF8.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF9.u32)  + u32Data * CHN_OFFSET), SR_HCOEF9.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_HCOEF10.u32) + u32Data * CHN_OFFSET), SR_HCOEF10.u32); 

	return ;
}

HI_VOID VDP_DISP_SetSrVerCoef (HI_U32 u32Data, VDP_SR_VERCOEF_S  stSrVerCoef)
{
    U_SR_VCOEF0   SR_VCOEF0;
    U_SR_VCOEF1   SR_VCOEF1;
    U_SR_VCOEF2   SR_VCOEF2;
    U_SR_VCOEF3   SR_VCOEF3;
    U_SR_VCOEF4   SR_VCOEF4;
    U_SR_VCOEF5   SR_VCOEF5;
    U_SR_VCOEF6   SR_VCOEF6;
    U_SR_VCOEF7   SR_VCOEF7;
	
	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrVerCoef() Select Wrong Video Layer ID\n");
		return ;
	}

	SR_VCOEF0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF0.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF1.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF2.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF2.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF3.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF3.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF4.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF4.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF5.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF5.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF6.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF6.u32)  + u32Data * CHN_OFFSET));
	SR_VCOEF7.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_VCOEF7.u32)  + u32Data * CHN_OFFSET));

	SR_VCOEF0.bits.vlcoef00 = stSrVerCoef.sr_vlcoef00;
	SR_VCOEF0.bits.vlcoef01 = stSrVerCoef.sr_vlcoef01;
	SR_VCOEF0.bits.vlcoef02 = stSrVerCoef.sr_vlcoef02;

	SR_VCOEF1.bits.vlcoef03 = stSrVerCoef.sr_vlcoef03;
	SR_VCOEF1.bits.vlcoef04 = stSrVerCoef.sr_vlcoef04;
	SR_VCOEF1.bits.vlcoef05 = stSrVerCoef.sr_vlcoef05;

	SR_VCOEF2.bits.vlcoef10 = stSrVerCoef.sr_vlcoef10;
	SR_VCOEF2.bits.vlcoef11 = stSrVerCoef.sr_vlcoef11;
	SR_VCOEF2.bits.vlcoef12 = stSrVerCoef.sr_vlcoef12;

	SR_VCOEF3.bits.vlcoef13 = stSrVerCoef.sr_vlcoef13;
	SR_VCOEF3.bits.vlcoef14 = stSrVerCoef.sr_vlcoef14;
	SR_VCOEF3.bits.vlcoef15 = stSrVerCoef.sr_vlcoef15;

	SR_VCOEF4.bits.vccoef00 = stSrVerCoef.sr_vccoef00;
	SR_VCOEF4.bits.vccoef01 = stSrVerCoef.sr_vccoef01;
	SR_VCOEF4.bits.vccoef02 = stSrVerCoef.sr_vccoef02;

	SR_VCOEF5.bits.vccoef03 = stSrVerCoef.sr_vccoef03;
	SR_VCOEF5.bits.vccoef04 = stSrVerCoef.sr_vccoef04;
	SR_VCOEF5.bits.vccoef05 = stSrVerCoef.sr_vccoef05;

	SR_VCOEF6.bits.vccoef10 = stSrVerCoef.sr_vccoef10;
	SR_VCOEF6.bits.vccoef11 = stSrVerCoef.sr_vccoef11;
	SR_VCOEF6.bits.vccoef12 = stSrVerCoef.sr_vccoef12;

	SR_VCOEF7.bits.vccoef13 = stSrVerCoef.sr_vccoef13;
	SR_VCOEF7.bits.vccoef14 = stSrVerCoef.sr_vccoef14;
	SR_VCOEF7.bits.vccoef15 = stSrVerCoef.sr_vccoef15;

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF0.u32)  + u32Data * CHN_OFFSET), SR_VCOEF0.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF1.u32)  + u32Data * CHN_OFFSET), SR_VCOEF1.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF2.u32)  + u32Data * CHN_OFFSET), SR_VCOEF2.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF3.u32)  + u32Data * CHN_OFFSET), SR_VCOEF3.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF4.u32)  + u32Data * CHN_OFFSET), SR_VCOEF4.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF5.u32)  + u32Data * CHN_OFFSET), SR_VCOEF5.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF6.u32)  + u32Data * CHN_OFFSET), SR_VCOEF6.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_VCOEF7.u32)  + u32Data * CHN_OFFSET), SR_VCOEF7.u32); 

	return ;
}
#endif
#if 1
HI_VOID VDP_DISP_SetSrZmeCoefAddr (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    U_DHDHLCOEFAD        DHDHLCOEFAD;
    U_DHDHCCOEFAD        DHDHCCOEFAD;
   
    U_DHDVLCOEFAD        DHDVLCOEFAD;
    U_DHDVCCOEFAD        DHDVCCOEFAD;
    
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetSrZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }
    
    if(u32Mode == VDP_GP_PARA_ZME_HORL)
    {
        DHDHLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDHLCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDHLCOEFAD.bits.dhdhlcoefad = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDHLCOEFAD.u32) + u32Data * CHN_OFFSET), DHDHLCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_HORC)
    {
        DHDHCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDHCCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDHCCOEFAD.bits.dhdhccoefad = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDHCCOEFAD.u32) + u32Data * CHN_OFFSET), DHDHCCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VERL)
    {
        DHDVLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDVLCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDVLCOEFAD.bits.dhdvlcoefad = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDVLCOEFAD.u32) + u32Data * CHN_OFFSET), DHDVLCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VERC)
    {
        DHDVCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDVCCOEFAD.u32) + u32Data * CHN_OFFSET));
        DHDVCCOEFAD.bits.dhdvccoefad = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDVCCOEFAD.u32) + u32Data * CHN_OFFSET), DHDVCCOEFAD.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_DISP_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}
#endif
HI_VOID  VDP_DISP_SetSrCscEnable  (HI_U32 u32CscId, HI_U32 enCSC)
{   
    U_SR_CSC0_IDC SR_CSC0_IDC;
    U_SR_CSC1_IDC SR_CSC1_IDC;

    if(u32CscId == 0)
    {
        SR_CSC0_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_IDC.u32)));        
        SR_CSC0_IDC.bits.csc_en = enCSC;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_IDC.u32)), SR_CSC0_IDC.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_IDC.u32)));        
        SR_CSC1_IDC.bits.csc_en = enCSC;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_IDC.u32)), SR_CSC1_IDC.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetSrCscEnable() Select a Wrong CSC id!\n");
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
        SR_CSC0_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_IDC.u32)));
        SR_CSC0_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
        SR_CSC0_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_IDC.u32)), SR_CSC0_IDC.u32);

        SR_CSC0_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_ODC.u32)));
        SR_CSC0_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        SR_CSC0_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_ODC.u32)), SR_CSC0_ODC.u32);

        SR_CSC0_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_IODC.u32)));
        SR_CSC0_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        SR_CSC0_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_IODC.u32)), SR_CSC0_IODC.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_IDC.u32)));
        SR_CSC1_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
        SR_CSC1_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_IDC.u32)), SR_CSC1_IDC.u32);

        SR_CSC1_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_ODC.u32)));
        SR_CSC1_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        SR_CSC1_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_ODC.u32)), SR_CSC1_ODC.u32);

        SR_CSC1_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_IODC.u32)));
        SR_CSC1_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        SR_CSC1_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_IODC.u32)), SR_CSC1_IODC.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetSrCscDcCoef() Select a Wrong CSC id!\n");
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
        SR_CSC0_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_P0.u32)));
        SR_CSC0_P0.bits.cscp00 = stCscCoef.csc_coef00;
        SR_CSC0_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_P0.u32)), SR_CSC0_P0.u32);

        SR_CSC0_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_P1.u32)));
        SR_CSC0_P1.bits.cscp02 = stCscCoef.csc_coef02;
        SR_CSC0_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_P1.u32)), SR_CSC0_P1.u32);

        SR_CSC0_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_P2.u32)));
        SR_CSC0_P2.bits.cscp11 = stCscCoef.csc_coef11;
        SR_CSC0_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_P2.u32)), SR_CSC0_P2.u32);

        SR_CSC0_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_P3.u32)));
        SR_CSC0_P3.bits.cscp20 = stCscCoef.csc_coef20;
        SR_CSC0_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_P3.u32)), SR_CSC0_P3.u32);

        SR_CSC0_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC0_P4.u32)));
        SR_CSC0_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC0_P4.u32)), SR_CSC0_P4.u32);
    }
    else if(u32CscId == 1)
    {
        SR_CSC1_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_P0.u32)));
        SR_CSC1_P0.bits.cscp00 = stCscCoef.csc_coef00;
        SR_CSC1_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_P0.u32)), SR_CSC1_P0.u32);

        SR_CSC1_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_P1.u32)));
        SR_CSC1_P1.bits.cscp02 = stCscCoef.csc_coef02;
        SR_CSC1_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_P1.u32)), SR_CSC1_P1.u32);

        SR_CSC1_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_P2.u32)));
        SR_CSC1_P2.bits.cscp11 = stCscCoef.csc_coef11;
        SR_CSC1_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_P2.u32)), SR_CSC1_P2.u32);

        SR_CSC1_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_P3.u32)));
        SR_CSC1_P3.bits.cscp20 = stCscCoef.csc_coef20;
        SR_CSC1_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_P3.u32)), SR_CSC1_P3.u32);

        SR_CSC1_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_CSC1_P4.u32)));
        SR_CSC1_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->SR_CSC1_P4.u32)), SR_CSC1_P4.u32);
    }
    else
    {
        VDP_PRINT("Error, VDP_DISP_SetSrCscCoef() Select a Wrong CSC id!\n");
    }

}

HI_VOID VDP_DISP_SetSrCscMode(HI_U32 u32CscId, VDP_CSC_MODE_E enCscMode)
{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;

    HI_S32 s32Pre   = 1 << 10;
    HI_S32 s32DcPre = 4;//1:8bit; 4:10bit

    if(enCscMode == VDP_CSC_RGB2YUV_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.299  * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.587  * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.114  * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.172 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.339 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(0.511  * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(0.511  * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.428 * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.083 * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * s32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.371  * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.698 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.336 * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.732  * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  0 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  0 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  0 * s32DcPre;
    }
    else if(enCscMode == VDP_CSC_RGB2YUV_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(0.213  * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(0.715  * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(0.072  * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-0.117 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.394 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.511 * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)( 0.511 * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-0.464 * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-0.047 * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = 0 * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 0 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 0 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 16  * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * s32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2RGB_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(    1  * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(    0  * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(1.540  * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     1 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-0.183 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-0.459 * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(    1  * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(1.816  * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(    0  * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 0 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 0 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 0 * s32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_709_601)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * s32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_601_709)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(     1 * s32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-0.116 * s32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)( 0.208 * s32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(     0 * s32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)( 1.017 * s32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * s32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(     0 * s32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)( 0.075 * s32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * s32Pre);

        st_csc_dc_coef.csc_in_dc2  = -16  * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 =   16 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 =  128 * s32DcPre;
    }
    else
    {
        st_csc_coef.csc_coef00     = 1 * s32Pre;
        st_csc_coef.csc_coef01     = 0 * s32Pre;
        st_csc_coef.csc_coef02     = 0 * s32Pre;

        st_csc_coef.csc_coef10     = 0 * s32Pre;
        st_csc_coef.csc_coef11     = 1 * s32Pre;
        st_csc_coef.csc_coef12     = 0 * s32Pre;

        st_csc_coef.csc_coef20     = 0 * s32Pre;
        st_csc_coef.csc_coef21     = 0 * s32Pre;
        st_csc_coef.csc_coef22     = 1 * s32Pre;

        st_csc_dc_coef.csc_in_dc2  = -16  * s32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * s32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -128 * s32DcPre;

        st_csc_dc_coef.csc_out_dc2 =  16 * s32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * s32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 128 * s32DcPre;
    }

    VDP_DISP_SetSrCscCoef  (u32CscId,st_csc_coef);
    VDP_DISP_SetSrCscDcCoef(u32CscId,st_csc_dc_coef);

    return ;
}

HI_VOID VDP_DISP_SetSrIReso (HI_U32 u32Data, HI_U32 u32Sriw, HI_U32 u32Srih)
{
	U_DHD0_SRIRESO DHD0_SRIRESO;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_DISP_SetSrIReso() Select Wrong Video Layer ID\n");
		return ;
	}

	DHD0_SRIRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SR_LTEX_THR.u32) + u32Data * CHN_OFFSET));
	DHD0_SRIRESO.bits.sr_iw = u32Sriw - 1;
	DHD0_SRIRESO.bits.sr_ih = u32Srih - 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SRIRESO.u32) + u32Data * CHN_OFFSET), DHD0_SRIRESO.u32); 
}

//--------------------------------------------------------------------
//SR_END
//--------------------------------------------------------------------
#if 1
//--------------------------------------------------------------------
//Dimming begin
//--------------------------------------------------------------------

HI_VOID VDP_DISP_SetDimmingbEn(HI_U32 u32Data,HI_U32 bEn)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingbEn() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.diming_en = bEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 

}

HI_VOID VDP_DISP_SetDimmingledbypass(HI_U32 u32Data, HI_U32 bypassled)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingledbypass() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.bypass_led = bypassled;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 

}

HI_VOID VDP_DISP_SetDimminglcdbypass(HI_U32 u32Data, HI_U32 bypasslcd)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimminglcdbypass() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.bypass_lcd = bypasslcd;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 
}

HI_VOID VDP_DISP_SetDimmingdisable_2d(HI_U32 u32Data, HI_U32 disable_2d)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingdisable_2d() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.disable_2d = disable_2d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 
}

HI_VOID VDP_DISP_SetDimmingSelUhd(HI_U32 u32Data, HI_U32 u32SelUhd)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSelUhd() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.sel_uhd= u32SelUhd;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 
}


HI_VOID VDP_DISP_SetDimmingModeInitLed( HI_U32 u32Data, HI_U32 u32modeinitled)
{
    U_DIM_GLB_CTRL DIM_GLB_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingModeInitLed() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)));
    DIM_GLB_CTRL.bits.mode_init_led = u32modeinitled;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_CTRL.u32)), DIM_GLB_CTRL.u32); 
}

HI_VOID VDP_DISP_SetDimmingLedNum(HI_U32 u32Data,HI_U32 u32lednumv,HI_U32 u32lednumh)
{
    U_DIM_LED_NUM  DIM_LED_NUM;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingLedNum() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED_NUM.u32)));
    DIM_LED_NUM.bits.led_numv = u32lednumv-1;
    DIM_LED_NUM.bits.led_numh = u32lednumh-1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED_NUM.u32)), DIM_LED_NUM.u32); 
}

HI_VOID  VDP_DISP_SetDimmingStartPos ( HI_U32 u32Data,HI_U32 u32hcnt_init ,HI_U32 u32vcnt_init)
{
    U_DIM_START_POS   DIM_START_POS;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingStartPos() Select Wrong Video Layer ID\n");
        return ;
    }


    DIM_START_POS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_START_POS.u32)));
    DIM_START_POS.bits.hcnt_init= u32hcnt_init;
    DIM_START_POS.bits.vcnt_init= u32vcnt_init;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_START_POS.u32)), DIM_START_POS.u32); 

}

HI_VOID VDP_DISP_SetDimmingSegMetircs(HI_U32 u32Data,HI_U32 u32seg_height ,HI_U32 u32seg_width)
{
    U_DIM_SEG_METIRCS  DIM_SEG_METIRCS;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSegMetircs() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_SEG_METIRCS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SEG_METIRCS.u32)));
    DIM_SEG_METIRCS.bits.seg_height = u32seg_height;
    DIM_SEG_METIRCS.bits.seg_width  = u32seg_width;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SEG_METIRCS.u32)), DIM_SEG_METIRCS.u32); 
}

HI_VOID VDP_DISP_SetDimmingNormUnit        (HI_U32 u32Data, HI_U32 u32glb_norm_unit ,HI_U32 u32seg_norm_unit)
{
    U_DIM_GLB_NORM_UNIT   DIM_GLB_NORM_UNIT;
    U_DIM_SEG_NORM_UNIT   DIM_SEG_NORM_UNIT;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingNormUnit() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_GLB_NORM_UNIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_GLB_NORM_UNIT.u32)));
    DIM_SEG_NORM_UNIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SEG_NORM_UNIT.u32)));

    DIM_GLB_NORM_UNIT.bits.glb_norm_unit = u32glb_norm_unit;
    DIM_SEG_NORM_UNIT.bits.seg_norm_unit = u32seg_norm_unit;//lijincheng modify
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_GLB_NORM_UNIT.u32)), DIM_GLB_NORM_UNIT.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SEG_NORM_UNIT.u32)), DIM_SEG_NORM_UNIT.u32);

}

HI_VOID VDP_DISP_SetDimmingHistStat        ( HI_U32 u32Data,HI_U32 u32thresh_md2 ,HI_U32 u32thresh_md0, HI_U32 u32bits_w_hist ,HI_U32 u32bits_b_hist)
{
    U_DIM_HIST_STAT   DIM_HIST_STAT;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingHistStat() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_HIST_STAT.u32                                     = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_HIST_STAT.u32)));
    DIM_HIST_STAT.bits.thresh_md2                         = u32thresh_md2;
    DIM_HIST_STAT.bits.thresh_md0                         = u32thresh_md0;
    DIM_HIST_STAT.bits.bits_w_hist                        = u32bits_w_hist;
    DIM_HIST_STAT.bits.bits_b_hist                        = u32bits_b_hist;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_HIST_STAT.u32)), DIM_HIST_STAT.u32);
}

HI_VOID VDP_DISP_SetDimming0DLedThd        (HI_U32 u32Data, VDP_DISP_DIMMING_THD_S *pstThd)
{
    U_DIM_ADJ0D_CORE0  DIM_ADJ0D_CORE0 ;
    U_DIM_ADJ0D_CORE1  DIM_ADJ0D_CORE1 ;
    U_DIM_LED0D_LOW_THRESH0  DIM_LED0D_LOW_THRESH0 ;
    U_DIM_LED0D_LOW_THRESH1  DIM_LED0D_LOW_THRESH1 ;
    U_DIM_LED0D_LOW_OFFSET0  DIM_LED0D_LOW_OFFSET0 ;
    U_DIM_LED0D_LOW_OFFSET1  DIM_LED0D_LOW_OFFSET1 ;
    U_DIM_LED0D_LOW_GAIN0    DIM_LED0D_LOW_GAIN0   ;
    U_DIM_LED0D_LOW_GAIN1    DIM_LED0D_LOW_GAIN1   ;
    U_DIM_LED0D_MID_THRESH0  DIM_LED0D_MID_THRESH0 ;
    U_DIM_LED0D_MID_THRESH1  DIM_LED0D_MID_THRESH1 ;
    U_DIM_LED0D_MID_OFFSET0  DIM_LED0D_MID_OFFSET0 ;
    U_DIM_LED0D_MID_OFFSET1  DIM_LED0D_MID_OFFSET1 ;
    U_DIM_LED0D_MID_GAIN0    DIM_LED0D_MID_GAIN0   ;
    U_DIM_LED0D_MID_GAIN1    DIM_LED0D_MID_GAIN1   ;
    U_DIM_LED0D_HIGH_THRESH0 DIM_LED0D_HIGH_THRESH0;
    U_DIM_LED0D_HIGH_THRESH1 DIM_LED0D_HIGH_THRESH1;
    U_DIM_LED0D_HIGH_OFFSET0 DIM_LED0D_HIGH_OFFSET0;
    U_DIM_LED0D_HIGH_OFFSET1 DIM_LED0D_HIGH_OFFSET1;
    U_DIM_LED0D_HIGH_GAIN0   DIM_LED0D_HIGH_GAIN0  ;
    U_DIM_LED0D_HIGH_GAIN1   DIM_LED0D_HIGH_GAIN1  ;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming0DLedThd() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_ADJ0D_CORE0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_ADJ0D_CORE0.u32)));
    DIM_ADJ0D_CORE1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_ADJ0D_CORE1.u32)));
    DIM_LED0D_LOW_THRESH0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_THRESH0.u32)));
    DIM_LED0D_LOW_THRESH1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_THRESH1.u32)));
    DIM_LED0D_LOW_OFFSET0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_OFFSET0.u32)));
    DIM_LED0D_LOW_OFFSET1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_OFFSET1.u32)));
    DIM_LED0D_LOW_GAIN0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_GAIN0.u32)));
    DIM_LED0D_LOW_GAIN1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_GAIN1.u32)));
    DIM_LED0D_MID_THRESH0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_THRESH0.u32)));
    DIM_LED0D_MID_THRESH1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_THRESH1.u32)));
    DIM_LED0D_MID_OFFSET0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_OFFSET0.u32)));
    DIM_LED0D_MID_OFFSET1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_OFFSET1.u32)));
    DIM_LED0D_MID_GAIN0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_GAIN0.u32)));
    DIM_LED0D_MID_GAIN1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_GAIN1.u32)));
    DIM_LED0D_HIGH_THRESH0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_THRESH0.u32)));
    DIM_LED0D_HIGH_THRESH1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_THRESH1.u32)));
    DIM_LED0D_HIGH_OFFSET0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_OFFSET0.u32)));
    DIM_LED0D_HIGH_OFFSET1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_OFFSET1.u32)));
    DIM_LED0D_HIGH_GAIN0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_GAIN0.u32)));
    DIM_LED0D_HIGH_GAIN1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_GAIN1.u32)));

    DIM_ADJ0D_CORE0.bits.core_low_0d  = pstThd->u32CoreLow ;
    DIM_ADJ0D_CORE0.bits.core_mid_0d  = pstThd->u32CoreMid ;
    DIM_ADJ0D_CORE1.bits.core_high_0d  = pstThd->u32CoreHigh ;

    DIM_LED0D_LOW_THRESH0.bits.x0_low_0d    =   pstThd->u32X0Low;
    DIM_LED0D_LOW_THRESH0.bits.x1_low_0d    =   pstThd->u32X1Low;
    DIM_LED0D_LOW_THRESH1.bits.x2_low_0d    =   pstThd->u32X2Low;
    DIM_LED0D_LOW_OFFSET0.bits.g0_low_0d    =   pstThd->u32G0Low;
    DIM_LED0D_LOW_OFFSET0.bits.g1_low_0d    =   pstThd->u32G1Low;
    DIM_LED0D_LOW_OFFSET1.bits.g2_low_0d    =   pstThd->u32G2Low;
    DIM_LED0D_LOW_GAIN0.bits.k0_low_0d      =   pstThd->u32K0Low;
    DIM_LED0D_LOW_GAIN0.bits.k1_low_0d      =   pstThd->u32K1Low;
    DIM_LED0D_LOW_GAIN1.bits.k2_low_0d      =   pstThd->u32K2Low;

    DIM_LED0D_MID_THRESH0.bits.x0_mid_0d    =   pstThd->u32X0Mid;
    DIM_LED0D_MID_THRESH0.bits.x1_mid_0d    =   pstThd->u32X1Mid;
    DIM_LED0D_MID_THRESH1.bits.x2_mid_0d    =   pstThd->u32X2Mid;
    DIM_LED0D_MID_OFFSET0.bits.g0_mid_0d    =   pstThd->u32G0Mid;
    DIM_LED0D_MID_OFFSET0.bits.g1_mid_0d    =   pstThd->u32G1Mid;
    DIM_LED0D_MID_OFFSET1.bits.g2_mid_0d    =   pstThd->u32G2Mid;
    DIM_LED0D_MID_GAIN0.bits.k0_mid_0d      =   pstThd->u32K0Mid;
    DIM_LED0D_MID_GAIN0.bits.k1_mid_0d      =   pstThd->u32K1Mid;
    DIM_LED0D_MID_GAIN1.bits.k2_mid_0d      =   pstThd->u32K2Mid;

    DIM_LED0D_HIGH_THRESH0.bits.x0_high_0d    =   pstThd->u32X0High;
    DIM_LED0D_HIGH_THRESH0.bits.x1_high_0d    =   pstThd->u32X1High;
    DIM_LED0D_HIGH_THRESH1.bits.x2_high_0d    =   pstThd->u32X2High;
    DIM_LED0D_HIGH_OFFSET0.bits.g0_high_0d    =   pstThd->u32G0High;
    DIM_LED0D_HIGH_OFFSET0.bits.g1_high_0d    =   pstThd->u32G1High;
    DIM_LED0D_HIGH_OFFSET1.bits.g2_high_0d    =   pstThd->u32G2High;
    DIM_LED0D_HIGH_GAIN0.bits.k0_high_0d      =   pstThd->u32K0High;
    DIM_LED0D_HIGH_GAIN0.bits.k1_high_0d      =   pstThd->u32K1High;
    DIM_LED0D_HIGH_GAIN1.bits.k2_high_0d      =   pstThd->u32K2High;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_ADJ0D_CORE0.u32        )) ,DIM_ADJ0D_CORE0.u32        ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_ADJ0D_CORE1.u32        )) ,DIM_ADJ0D_CORE1.u32        ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_THRESH0.u32  )) ,DIM_LED0D_LOW_THRESH0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_THRESH1.u32  )) ,DIM_LED0D_LOW_THRESH1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_OFFSET0.u32  )) ,DIM_LED0D_LOW_OFFSET0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_OFFSET1.u32  )) ,DIM_LED0D_LOW_OFFSET1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_GAIN0.u32    )) ,DIM_LED0D_LOW_GAIN0.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_LOW_GAIN1.u32    )) ,DIM_LED0D_LOW_GAIN1.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_THRESH0.u32  )) ,DIM_LED0D_MID_THRESH0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_THRESH1.u32  )) ,DIM_LED0D_MID_THRESH1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_OFFSET0.u32  )) ,DIM_LED0D_MID_OFFSET0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_OFFSET1.u32  )) ,DIM_LED0D_MID_OFFSET1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_GAIN0.u32    )) ,DIM_LED0D_MID_GAIN0.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_MID_GAIN1.u32    )) ,DIM_LED0D_MID_GAIN1.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_THRESH0.u32 )) ,DIM_LED0D_HIGH_THRESH0.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_THRESH1.u32 )) ,DIM_LED0D_HIGH_THRESH1.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_OFFSET0.u32 )) ,DIM_LED0D_HIGH_OFFSET0.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_OFFSET1.u32 )) ,DIM_LED0D_HIGH_OFFSET1.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_GAIN0.u32   )) ,DIM_LED0D_HIGH_GAIN0.u32   ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_HIGH_GAIN1.u32   )) ,DIM_LED0D_HIGH_GAIN1.u32   ) ;

}

HI_VOID VDP_DISP_SetDimming2DLedThd(HI_U32 u32Data, VDP_DISP_DIMMING_THD_S *pstThd2D)
{
    U_DIM_ADJ2D_CORE0        DIM_ADJ2D_CORE0 ;
    U_DIM_ADJ2D_CORE1        DIM_ADJ2D_CORE1 ;
    U_DIM_LED2D_LOW_THRESH0  DIM_LED2D_LOW_THRESH0 ;
    U_DIM_LED2D_LOW_THRESH1  DIM_LED2D_LOW_THRESH1 ;
    U_DIM_LED2D_LOW_OFFSET0  DIM_LED2D_LOW_OFFSET0 ;
    U_DIM_LED2D_LOW_OFFSET1  DIM_LED2D_LOW_OFFSET1 ;
    U_DIM_LED2D_LOW_GAIN0    DIM_LED2D_LOW_GAIN0   ;
    U_DIM_LED2D_LOW_GAIN1    DIM_LED2D_LOW_GAIN1   ;
    U_DIM_LED2D_MID_THRESH0  DIM_LED2D_MID_THRESH0 ;
    U_DIM_LED2D_MID_THRESH1  DIM_LED2D_MID_THRESH1 ;
    U_DIM_LED2D_MID_OFFSET0  DIM_LED2D_MID_OFFSET0 ;
    U_DIM_LED2D_MID_OFFSET1  DIM_LED2D_MID_OFFSET1 ;
    U_DIM_LED2D_MID_GAIN0    DIM_LED2D_MID_GAIN0   ;
    U_DIM_LED2D_MID_GAIN1    DIM_LED2D_MID_GAIN1   ;
    U_DIM_LED2D_HIGH_THRESH0 DIM_LED2D_HIGH_THRESH0;
    U_DIM_LED2D_HIGH_THRESH1 DIM_LED2D_HIGH_THRESH1;
    U_DIM_LED2D_HIGH_OFFSET0 DIM_LED2D_HIGH_OFFSET0;
    U_DIM_LED2D_HIGH_OFFSET1 DIM_LED2D_HIGH_OFFSET1;
    U_DIM_LED2D_HIGH_GAIN0   DIM_LED2D_HIGH_GAIN0  ;
    U_DIM_LED2D_HIGH_GAIN1   DIM_LED2D_HIGH_GAIN1  ;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming2DLedThd() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_ADJ2D_CORE0.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_ADJ2D_CORE0.u32)));
    DIM_ADJ2D_CORE1.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_ADJ2D_CORE1.u32)));
    DIM_LED2D_LOW_THRESH0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_THRESH0.u32)));
    DIM_LED2D_LOW_THRESH1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_THRESH1.u32)));
    DIM_LED2D_LOW_OFFSET0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_OFFSET0.u32)));
    DIM_LED2D_LOW_OFFSET1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_OFFSET1.u32)));
    DIM_LED2D_LOW_GAIN0.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_GAIN0.u32)));
    DIM_LED2D_LOW_GAIN1.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_GAIN1.u32)));
    DIM_LED2D_MID_THRESH0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_THRESH0.u32)));
    DIM_LED2D_MID_THRESH1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_THRESH1.u32)));
    DIM_LED2D_MID_OFFSET0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_OFFSET0.u32)));
    DIM_LED2D_MID_OFFSET1.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_OFFSET1.u32)));
    DIM_LED2D_MID_GAIN0.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_GAIN0.u32)));
    DIM_LED2D_MID_GAIN1.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_GAIN1.u32)));
    DIM_LED2D_HIGH_THRESH0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_THRESH0.u32)));
    DIM_LED2D_HIGH_THRESH1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_THRESH1.u32)));
    DIM_LED2D_HIGH_OFFSET0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_OFFSET0.u32)));
    DIM_LED2D_HIGH_OFFSET1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_OFFSET1.u32)));
    DIM_LED2D_HIGH_GAIN0.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_GAIN0.u32)));
    DIM_LED2D_HIGH_GAIN1.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_GAIN1.u32)));

    DIM_ADJ2D_CORE0.bits.core_low_2d  = pstThd2D->u32CoreLow ;
    DIM_ADJ2D_CORE0.bits.core_mid_2d  = pstThd2D->u32CoreMid ;
    DIM_ADJ2D_CORE1.bits.core_high_2d = pstThd2D->u32CoreHigh ;

    DIM_LED2D_LOW_THRESH0.bits.x0_low_2d    =   pstThd2D->u32X0Low;
    DIM_LED2D_LOW_THRESH0.bits.x1_low_2d    =   pstThd2D->u32X1Low;
    DIM_LED2D_LOW_THRESH1.bits.x2_low_2d    =   pstThd2D->u32X2Low;
    DIM_LED2D_LOW_OFFSET0.bits.g0_low_2d    =   pstThd2D->u32G0Low;
    DIM_LED2D_LOW_OFFSET0.bits.g1_low_2d    =   pstThd2D->u32G1Low;
    DIM_LED2D_LOW_OFFSET1.bits.g2_low_2d    =   pstThd2D->u32G2Low;
    DIM_LED2D_LOW_GAIN0.bits.k0_low_2d      =   pstThd2D->u32K0Low;
    DIM_LED2D_LOW_GAIN0.bits.k1_low_2d      =   pstThd2D->u32K1Low;
    DIM_LED2D_LOW_GAIN1.bits.k2_low_2d      =   pstThd2D->u32K2Low;

    DIM_LED2D_MID_THRESH0.bits.x0_mid_2d    =   pstThd2D->u32X0Mid;
    DIM_LED2D_MID_THRESH0.bits.x1_mid_2d    =   pstThd2D->u32X1Mid;
    DIM_LED2D_MID_THRESH1.bits.x2_mid_2d    =   pstThd2D->u32X2Mid;
    DIM_LED2D_MID_OFFSET0.bits.g0_mid_2d    =   pstThd2D->u32G0Mid;
    DIM_LED2D_MID_OFFSET0.bits.g1_mid_2d    =   pstThd2D->u32G1Mid;
    DIM_LED2D_MID_OFFSET1.bits.g2_mid_2d    =   pstThd2D->u32G2Mid;
    DIM_LED2D_MID_GAIN0.bits.k0_mid_2d      =   pstThd2D->u32K0Mid;
    DIM_LED2D_MID_GAIN0.bits.k1_mid_2d      =   pstThd2D->u32K1Mid;
    DIM_LED2D_MID_GAIN1.bits.k2_mid_2d      =   pstThd2D->u32K2Mid;

    DIM_LED2D_HIGH_THRESH0.bits.x0_high_2d    =   pstThd2D->u32X0High;
    DIM_LED2D_HIGH_THRESH0.bits.x1_high_2d    =   pstThd2D->u32X1High;
    DIM_LED2D_HIGH_THRESH1.bits.x2_high_2d    =   pstThd2D->u32X2High;
    DIM_LED2D_HIGH_OFFSET0.bits.g0_high_2d    =   pstThd2D->u32G0High;
    DIM_LED2D_HIGH_OFFSET0.bits.g1_high_2d    =   pstThd2D->u32G1High;
    DIM_LED2D_HIGH_OFFSET1.bits.g2_high_2d    =   pstThd2D->u32G2High;
    DIM_LED2D_HIGH_GAIN0.bits.k0_high_2d      =   pstThd2D->u32K0High;
    DIM_LED2D_HIGH_GAIN0.bits.k1_high_2d      =   pstThd2D->u32K1High;
    DIM_LED2D_HIGH_GAIN1.bits.k2_high_2d      =   pstThd2D->u32K2High;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_ADJ2D_CORE0.u32        )) ,DIM_ADJ2D_CORE0.u32        ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_ADJ2D_CORE1.u32        )) ,DIM_ADJ2D_CORE1.u32        ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_THRESH0.u32  )) ,DIM_LED2D_LOW_THRESH0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_THRESH1.u32  )) ,DIM_LED2D_LOW_THRESH1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_OFFSET0.u32  )) ,DIM_LED2D_LOW_OFFSET0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_OFFSET1.u32  )) ,DIM_LED2D_LOW_OFFSET1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_GAIN0.u32    )) ,DIM_LED2D_LOW_GAIN0.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_LOW_GAIN1.u32    )) ,DIM_LED2D_LOW_GAIN1.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_THRESH0.u32  )) ,DIM_LED2D_MID_THRESH0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_THRESH1.u32  )) ,DIM_LED2D_MID_THRESH1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_OFFSET0.u32  )) ,DIM_LED2D_MID_OFFSET0.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_OFFSET1.u32  )) ,DIM_LED2D_MID_OFFSET1.u32  ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_GAIN0.u32    )) ,DIM_LED2D_MID_GAIN0.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_MID_GAIN1.u32    )) ,DIM_LED2D_MID_GAIN1.u32    ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_THRESH0.u32 )) ,DIM_LED2D_HIGH_THRESH0.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_THRESH1.u32 )) ,DIM_LED2D_HIGH_THRESH1.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_OFFSET0.u32 )) ,DIM_LED2D_HIGH_OFFSET0.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_OFFSET1.u32 )) ,DIM_LED2D_HIGH_OFFSET1.u32 ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_GAIN0.u32   )) ,DIM_LED2D_HIGH_GAIN0.u32   ) ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_HIGH_GAIN1.u32   )) ,DIM_LED2D_HIGH_GAIN1.u32   ) ;

}


HI_VOID VDP_DISP_SetDimmingfirCoef         (HI_U32 u32Data, VDP_DISP_DIMMING_FIR_COEF_S stCoef )
{  
    U_DIM_VFIR_COEF_0 DIM_VFIR_COEF_0;
    U_DIM_VFIR_COEF_1 DIM_VFIR_COEF_1;
    U_DIM_HFIR_COEF_0 DIM_HFIR_COEF_0;
    U_DIM_HFIR_COEF_1 DIM_HFIR_COEF_1;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingfirCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_VFIR_COEF_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_VFIR_COEF_0.u32)));
    DIM_VFIR_COEF_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_VFIR_COEF_1.u32)));
    DIM_HFIR_COEF_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_HFIR_COEF_0.u32)));
    DIM_HFIR_COEF_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_HFIR_COEF_1.u32)));

    DIM_HFIR_COEF_0.bits.hfir_tap0   =   stCoef.u32HFirTap0;
    DIM_HFIR_COEF_0.bits.hfir_tap1   =   stCoef.u32HFirTap1;
    DIM_HFIR_COEF_0.bits.hfir_tap2   =   stCoef.u32HFirTap2;
    DIM_HFIR_COEF_0.bits.hfir_tap3   =   stCoef.u32HFirTap3;
    DIM_HFIR_COEF_1.bits.hfir_tap4   =   stCoef.u32HFirTap4;
    DIM_VFIR_COEF_0.bits.vfir_tap0   =   stCoef.u32VFirTap0;
    DIM_VFIR_COEF_0.bits.vfir_tap1   =   stCoef.u32VFirTap1;
    DIM_VFIR_COEF_0.bits.vfir_tap2   =   stCoef.u32VFirTap2;
    DIM_VFIR_COEF_0.bits.vfir_tap3   =   stCoef.u32VFirTap3;
    DIM_VFIR_COEF_1.bits.vfir_tap4   =   stCoef.u32VFirTap4;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_VFIR_COEF_0.u32)) ,DIM_VFIR_COEF_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_VFIR_COEF_1.u32)) ,DIM_VFIR_COEF_1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_HFIR_COEF_0.u32)) ,DIM_HFIR_COEF_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_HFIR_COEF_1.u32)) ,DIM_HFIR_COEF_1.u32);


}

HI_VOID VDP_DISP_SetDimming0DLedCore       (HI_U32 u32Data, HI_U32 u32core_dstat_high ,HI_U32 u32core_dstat_mid,HI_U32 u32core_dstat_low,HI_U32 u32k_dstat_0d)
{
    U_DIM_LED0D_CORE  DIM_LED0D_CORE ;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming0DLedCore() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED0D_CORE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_CORE.u32)));

    DIM_LED0D_CORE.bits.core_dstat_low    = u32core_dstat_low ;
    DIM_LED0D_CORE.bits.core_dstat_mid    = u32core_dstat_mid;
    DIM_LED0D_CORE.bits.core_dstat_high   = u32core_dstat_high;
    DIM_LED0D_CORE.bits.k_dstat_0d        = u32k_dstat_0d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_CORE.u32)) ,DIM_LED0D_CORE.u32);

}

HI_VOID VDP_DISP_SetDimming0DLedGainMisc   (HI_U32 u32Data, HI_U32 u32gain_led0d ,HI_U32 u32gain_led0d_tf)
{
    U_DIM_LED0D_GAIN_MISC  DIM_LED0D_GAIN_MISC;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming0DLedGainMisc() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED0D_GAIN_MISC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_GAIN_MISC.u32)));
    DIM_LED0D_GAIN_MISC.bits.gain_led0d   =u32gain_led0d  ;
    DIM_LED0D_GAIN_MISC.bits.gain_led0d_tf=u32gain_led0d_tf;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D_GAIN_MISC.u32) ),DIM_LED0D_GAIN_MISC.u32)   ;

}

HI_VOID VDP_DISP_SetDimming2DLedGainMisc   ( HI_U32 u32Data,HI_U32 u32gain_led2d,HI_U32 u32gain_bldmax2d,HI_U32 u32gain_bld_0d2d)
{
    U_DIM_LED2D_GAIN_MISC DIM_LED2D_GAIN_MISC ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming2DLedGainMisc() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED2D_GAIN_MISC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED2D_GAIN_MISC.u32)));

    DIM_LED2D_GAIN_MISC.bits.gain_led2d   =u32gain_led2d  ;
    DIM_LED2D_GAIN_MISC.bits.gain_bldmax2d=u32gain_bldmax2d;
    DIM_LED2D_GAIN_MISC.bits.gain_bld_0d2d=u32gain_bld_0d2d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED2D_GAIN_MISC.u32) ),DIM_LED2D_GAIN_MISC.u32)    ;
}

HI_VOID VDP_DISP_SetDimmingFinalGain       (HI_U32 u32Data, HI_U32 u32gain_final_0d,HI_S32 u32offset_final_0d)
{
    U_DIM_FINAL_GAIN_0D  DIM_FINAL_GAIN_0D ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingFinalGain() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_FINAL_GAIN_0D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_FINAL_GAIN_0D.u32)));
    DIM_FINAL_GAIN_0D.bits.gain_final_0d  =u32gain_final_0d  ;
    DIM_FINAL_GAIN_0D.bits.offset_final_0d =u32offset_final_0d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_FINAL_GAIN_0D.u32)) ,DIM_FINAL_GAIN_0D.u32)    ;

}
HI_VOID VDP_DISP_SetDimmingFinalGain2D       (HI_U32 u32Data, HI_U32 u32gain_final_2d,HI_S32 u32offset_final_2d)
{
    U_DIM_FINAL_GAIN_2D  DIM_FINAL_GAIN_2D ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingFinalGain2D() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_FINAL_GAIN_2D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_FINAL_GAIN_2D.u32)));
    DIM_FINAL_GAIN_2D.bits.gain_final_2d  =u32gain_final_2d  ;
    DIM_FINAL_GAIN_2D.bits.offset_final_2d =u32offset_final_2d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_FINAL_GAIN_2D.u32)) ,DIM_FINAL_GAIN_2D.u32)    ;

}

HI_VOID VDP_DISP_SetDimmingFinalClip       ( HI_U32 u32Data,HI_U32 u32clip_btm_final_0d,HI_U32 u32clip_up_final_0d)
{
    U_DIM_FINAL_CLIP_0D  DIM_FINAL_CLIP_0D   ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingFinalClip() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_FINAL_CLIP_0D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_FINAL_CLIP_0D.u32)));
    DIM_FINAL_CLIP_0D.bits.clip_btm_final_0d =u32clip_btm_final_0d  ;
    DIM_FINAL_CLIP_0D.bits.clip_up_final_0d =u32clip_up_final_0d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_FINAL_CLIP_0D.u32)) ,DIM_FINAL_CLIP_0D.u32)    ;

}
HI_VOID VDP_DISP_SetDimmingFinalClip2D       (HI_U32 u32Data, HI_U32 u32clip_btm_final_2d,HI_U32 u32clip_up_final_2d)
{
    U_DIM_FINAL_CLIP_2D  DIM_FINAL_CLIP_2D   ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingFinalClip2D() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_FINAL_CLIP_2D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_FINAL_CLIP_2D.u32)));
    DIM_FINAL_CLIP_2D.bits.clip_btm_final_2d =u32clip_btm_final_2d  ;
    DIM_FINAL_CLIP_2D.bits.clip_up_final_2d =u32clip_up_final_2d;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_FINAL_CLIP_2D.u32)) ,DIM_FINAL_CLIP_2D.u32)    ;

}
HI_VOID VDP_DISP_SetDimming0DLedStatus     ( HI_U32 u32Data,HI_U32 u32led0d_sw, HI_U32 u32sc_0d_sw)
{
#if 0
    U_DIM_LED0D_STATUS  DIM_LED0D_STATUS ;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming0DLedStatus() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED0D_STATUS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D_STATUS.u32)));
#endif
}

HI_VOID VDP_DISP_SetDimmingPreBld          ( HI_U32 u32Data,HI_U32 u32core_led, HI_U32 u32k_led)
{
    U_DIM_LCD_COMP_0D  DIM_LCD_COMP_0D;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingPreBld() Select Wrong Video Layer ID\n");
        return ;
    }


    DIM_LCD_COMP_0D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_0D.u32)));
    DIM_LCD_COMP_0D.bits.lcd_core_led0d   = u32core_led  ;
    DIM_LCD_COMP_0D.bits.lcd_k_led0d   = u32k_led  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_0D.u32)) ,DIM_LCD_COMP_0D.u32)    ;

}

HI_VOID VDP_DISP_SetDimmingStatMetrics(HI_U32 u32Data,HI_U32 u32stat_height ,HI_U32 u32stat_width)
{
    U_DIM_STAT_METRICS DIM_STAT_METRICS ;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingStatMetrics() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_STAT_METRICS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_STAT_METRICS.u32)));
    DIM_STAT_METRICS.bits.stat_height = u32stat_height/2;  //lijincheng modify
    DIM_STAT_METRICS.bits.stat_width  = u32stat_width/2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_STAT_METRICS.u32)), DIM_STAT_METRICS.u32); 
}

HI_VOID VDP_DISP_SetDimmingPre0D2DBld          (HI_U32 u32Data, HI_U32 u32core_led0d, HI_U32 u32k_led0d)
{
    U_DIM_PRE_0D2D_BLD  DIM_PRE_0D2D_BLD;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingPre0D2DBld() Select Wrong Video Layer ID\n");
        return ;
    }


    DIM_PRE_0D2D_BLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_PRE_0D2D_BLD.u32)));
    DIM_PRE_0D2D_BLD.bits.core_led0d=u32core_led0d ;
    DIM_PRE_0D2D_BLD.bits.k_led0d=u32k_led0d ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_PRE_0D2D_BLD.u32)) ,DIM_PRE_0D2D_BLD.u32)    ;

}

HI_VOID VDP_DISP_SetDimmingPostFirBld      (HI_U32 u32Data, HI_U32 u32gain_hfir,HI_U32 u32gain_vfir)
{
    U_DIM_POST_FIR_BLD   DIM_POST_FIR_BLD;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingPostFirBld() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_POST_FIR_BLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_POST_FIR_BLD.u32)));
    DIM_POST_FIR_BLD.bits.gain_hfir      =u32gain_hfir   ;
    DIM_POST_FIR_BLD.bits.gain_vfir      =u32gain_vfir   ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_POST_FIR_BLD.u32)) ,DIM_POST_FIR_BLD.u32)    ;

}

HI_VOID VDP_DISP_SetDimmingPostFirGain     (HI_U32 u32Data, HI_U32 u32k_led2d, HI_U32 u32core_led2d, HI_U32 u32gain_tf)
{
    U_DIM_POST_FIR_GAIN  DIM_POST_FIR_GAIN;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingPostFirGain() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_POST_FIR_GAIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_POST_FIR_GAIN.u32)));
    DIM_POST_FIR_GAIN.bits.k_led2d    =  u32k_led2d;  
    DIM_POST_FIR_GAIN.bits.core_led2d =  u32core_led2d;
    DIM_POST_FIR_GAIN.bits.gain_tf    =  u32gain_tf;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_POST_FIR_GAIN.u32)) ,DIM_POST_FIR_GAIN.u32)    ;

}

HI_VOID VDP_DISP_SetDimming0DLcdComp       (HI_U32 u32Data, HI_U32 u32lcd_k_led, HI_U32 u32lcd_core_led)
{
    U_DIM_LCD_COMP_0D  DIM_LCD_COMP_0D;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming0DLcdComp() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LCD_COMP_0D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_0D.u32)));
    DIM_LCD_COMP_0D.bits.lcd_k_led0d       = u32lcd_k_led    ;
    DIM_LCD_COMP_0D.bits.lcd_core_led0d    = u32lcd_core_led ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_0D.u32)) ,DIM_LCD_COMP_0D.u32)    ;

}

HI_VOID VDP_DISP_SetDimming2DLcdComp       (HI_U32 u32Data, HI_U32 u32lcd_k_led, HI_U32 u32lcd_core_led)
{
    U_DIM_LCD_COMP_2D  DIM_LCD_COMP_2D;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimming2DLcdComp() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LCD_COMP_2D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_2D.u32)));
    DIM_LCD_COMP_2D.bits.lcd_k_led2d       = u32lcd_k_led    ;
    DIM_LCD_COMP_2D.bits.lcd_core_led2d    = u32lcd_core_led ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LCD_COMP_2D.u32)) ,DIM_LCD_COMP_2D.u32)    ;
}

HI_VOID VDP_DISP_SetDimmingLcdCompgain     (HI_U32 u32Data, HI_U32 u32lcd_compgain)
{
    U_DIM_LCD_COMPGAIN  DIM_LCD_COMPGAIN;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingLcdCompgain() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LCD_COMPGAIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LCD_COMPGAIN.u32)));
    DIM_LCD_COMPGAIN.bits.lcd_compgain_2d  =  u32lcd_compgain ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LCD_COMPGAIN.u32)) ,DIM_LCD_COMPGAIN.u32)    ;

}

HI_VOID VDP_DISP_SetDimmingLedPanel        (HI_U32 u32Data, HI_U32 u32max_led_panel,HI_U32 u32k_led_panel)
{
    U_DIM_LED_PANEL  DIM_LED_PANEL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingLedPanel() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED_PANEL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED_PANEL.u32)));
    DIM_LED_PANEL.bits.max_led_panel  = u32max_led_panel  ;
    DIM_LED_PANEL.bits.k_led_panel  = u32k_led_panel  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED_PANEL.u32)) ,DIM_LED_PANEL.u32)    ;
}
HI_VOID VDP_DISP_SetDimmingLed0D2D( HI_U32 u32Data,HI_U32 u32led2d_final,HI_U32 u32led0d_final)
{
    U_DIM_LED0D2D  DIM_LED0D2D;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingLed0D2D() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_LED0D2D.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_LED0D2D.u32)));
    DIM_LED0D2D.bits.led2d_final= u32led2d_final;
    DIM_LED0D2D.bits.led0d_final= u32led0d_final;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_LED0D2D.u32) ),DIM_LED0D2D.u32)    ;
}

HI_VOID VDP_DISP_SetDimmingSclRatioH(HI_U32 u32Data, HI_U32 u32lhfir_scl_dec,HI_U32 u32lhfir_scl_int)
{
	
    U_DIM_SCL_RATIO_H  DIM_SCL_RATIO_H;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclRatioH() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_SCL_RATIO_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_RATIO_H.u32)));
    DIM_SCL_RATIO_H.bits.lhfir_scl_dec= u32lhfir_scl_dec;
    DIM_SCL_RATIO_H.bits.lhfir_scl_int= u32lhfir_scl_int;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_RATIO_H.u32) ),DIM_SCL_RATIO_H.u32)    ;
}

HI_VOID VDP_DISP_SetDimmingSclRatioV( HI_U32 u32Data,HI_U32 u32lvfir_scl_dec,HI_U32 u32lvfir_scl_int)
{
	
    U_DIM_SCL_RATIO_V  DIM_SCL_RATIO_V;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclRatioV() Select Wrong Video Layer ID\n");
        return ;
    }

    DIM_SCL_RATIO_V.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_RATIO_V.u32)));
    DIM_SCL_RATIO_V.bits.lvfir_scl_dec= u32lvfir_scl_dec;
    DIM_SCL_RATIO_V.bits.lvfir_scl_int= u32lvfir_scl_int;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_RATIO_V.u32)) ,DIM_SCL_RATIO_V.u32)    ;
}

HI_VOID VDP_DISP_SetDimmingSclEnable( HI_U32 u32Data,VDP_ZME_MODE_E  enMode, HI_U32 bEnable)
{
	
    U_DIM_SCL_CTRL  DIM_SCL_CTRL;
    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclEnable() Select Wrong Video Layer ID\n");
        return ;
    }



    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lhfir_en= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lvfir_en= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32);  
    }


}
HI_VOID VDP_DISP_SetDimmingSclMidEnable(HI_U32 u32Data, VDP_ZME_MODE_E  enMode, HI_U32 bEnable)
{
    U_DIM_SCL_CTRL  DIM_SCL_CTRL;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }


    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lhfir_mid_en= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lvfir_mid_en= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32);  
    }


}

HI_VOID VDP_DISP_SetDimmingSclFirEnable( HI_U32 u32Data,VDP_ZME_MODE_E  enMode, HI_U32 bEnable)
{
    U_DIM_SCL_CTRL  DIM_SCL_CTRL;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclOffsetH() Select Wrong Video Layer ID\n");
        return ;
    }


    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lhfir_mode= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)));
        DIM_SCL_CTRL.bits.lvfir_mode= bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_CTRL.u32)), DIM_SCL_CTRL.u32);  
    }


}
HI_VOID VDP_DISP_SetDimmingSclOffsetH(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Offset)
{
    U_DIM_SCL_OFFSET_H   DIM_SCL_OFFSET_H;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclOffsetH() Select Wrong Video Layer ID\n");
        return ;
    }


    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_OFFSET_H.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_OFFSET_H.u32)));
        DIM_SCL_OFFSET_H.bits.lhfir_offset=s32Offset ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_OFFSET_H.u32)), DIM_SCL_OFFSET_H.u32); 
    }

}

HI_VOID VDP_DISP_SetDimmingSclOffsetV       (HI_U32 u32Data, VDP_ZME_MODE_E  enMode, HI_S32 s32Offset)
{
    U_DIM_SCL_OFFSET_V   DIM_SCL_OFFSET_V;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimmingSclOffsetV() Select Wrong Video Layer ID\n");
        return ;
    }



    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        DIM_SCL_OFFSET_V.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DIM_SCL_OFFSET_V.u32)));
        DIM_SCL_OFFSET_V.bits.lvfir_offset=s32Offset ;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DIM_SCL_OFFSET_V.u32)), DIM_SCL_OFFSET_V.u32);  
    }


}

HI_VOID VDP_DISP_SetLocalDimCoefAddr(HI_U32 u32hd_id, HI_U32 u32CoefAddr)
{
    U_DHDDIMLUTAD  DHDDIMLUTAD;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetLocalDimCoefAddrSelect Wrong CHANNEL ID\n");
        return ;
    }
    DHDDIMLUTAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDDIMLUTAD.u32)));
    DHDDIMLUTAD.bits.dhddimlutad= u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDDIMLUTAD.u32)), DHDDIMLUTAD.u32);
}

HI_VOID VDP_DISP_SetDimHZmeCoefAddr(HI_U32 u32hd_id, HI_U32 u32CoefAddr)
{
    U_DHDDIMHFAD  DHDDIMHFAD;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimHZmeCoefAddr  Select Wrong CHANNEL ID\n");
        return ;
    }

    DHDDIMHFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDDIMHFAD.u32)));
    DHDDIMHFAD.bits.dhddimhfad= u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDDIMHFAD.u32)), DHDDIMHFAD.u32);

}

HI_VOID VDP_DISP_SetDimVZmeCoefAddr(HI_U32 u32hd_id, HI_U32 u32CoefAddr)
{
    U_DHDDIMVFAD  DHDDIMVFAD ;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetDimHZmeCoefAddrSelect Wrong CHANNEL ID\n");
        return ;
    }

    DHDDIMVFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHDDIMVFAD.u32)));
    DHDDIMVFAD.bits.dhddimvfad= u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHDDIMVFAD.u32)), DHDDIMVFAD.u32);
}

#endif
//--------------------------------------------------------------------
//Dimming  end
//--------------------------------------------------------------------

HI_VOID VDP_DISP_SetStartPos (HI_U32 u32Data, HI_U32 u32StartPos)
{
    U_DHD0_START_POS   DHD0_START_POS;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetStartPos() Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_START_POS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_START_POS.u32)+u32Data*CHN_OFFSET));
    DHD0_START_POS.bits.start_pos = u32StartPos;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_START_POS.u32)+u32Data*CHN_OFFSET),DHD0_START_POS.u32);
}

HI_VOID VDP_DISP_SetParaLoadPos (HI_U32 u32Data, HI_U32 u32StartPos)
{
    U_DHD0_PARATHD   DHD0_PARATHD;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetStartPos() Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_PARATHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_PARATHD.u32)+u32Data*CHN_OFFSET));
    DHD0_PARATHD.bits.para_thd = u32StartPos;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_PARATHD.u32)+u32Data*CHN_OFFSET),DHD0_PARATHD.u32);
}

HI_VOID VDP_DISP_SetVbiEnable (HI_U32 u32Data, HI_U32 u32VbiEnable)
{
    U_DHD0_VBI   DHD0_VBI;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVbiEnable () Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_VBI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VBI.u32)+u32Data*CHN_OFFSET));
    DHD0_VBI.bits.vbi_en = u32VbiEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VBI.u32)+u32Data*CHN_OFFSET),DHD0_VBI.u32);
}

HI_VOID VDP_DISP_SetVbiData (HI_U32 u32Data, HI_U32 u32VbiAddr, HI_U32 u32VbiData)
{
    U_DHD0_VBI   DHD0_VBI;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetVbiData () Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_VBI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VBI.u32)+u32Data*CHN_OFFSET));
    DHD0_VBI.bits.vbi_addr = u32VbiAddr;
    DHD0_VBI.bits.vbi_data = u32VbiData;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_VBI.u32)+u32Data*CHN_OFFSET),DHD0_VBI.u32);
}

HI_VOID VDP_DISP_SetHbiOsdEnable (HI_U32 u32Data, HI_U32 u32HbiOsdEnable)
{
    U_DHD0_HBI   DHD0_HBI;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHbiOsdEnable () Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_HBI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET));
    DHD0_HBI.bits.hbi_gp0_en = u32HbiOsdEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET),DHD0_HBI.u32);
}

HI_VOID VDP_DISP_SetHbiHcEnable (HI_U32 u32Data, HI_U32 u32HbiHcEnable)
{
    U_DHD0_HBI   DHD0_HBI;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHbiHcEnable () Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_HBI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET));
    DHD0_HBI.bits.hbi_hc_en = u32HbiHcEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET),DHD0_HBI.u32);
}

HI_VOID VDP_DISP_SetHbiSyncEnable (HI_U32 u32Data, HI_U32 u32HbiSyncEnable)
{
    U_DHD0_HBI   DHD0_HBI;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHbiSyncEnable () Select Wrong Video Layer ID\n");
        return ;
    }

    DHD0_HBI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET));
    DHD0_HBI.bits.bitmap_sync_en = u32HbiSyncEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_HBI.u32)+u32Data*CHN_OFFSET),DHD0_HBI.u32);
}

HI_VOID VDP_DISP_SetHbiOsdThd (HI_U32 u32Data, HI_U32 u32HbiOsdThd)
{
    U_WBC_BMP_THD  WBC_BMP_THD;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHbiOsdThd () Select Wrong Video Layer ID\n");
        return ;
    }

    WBC_BMP_THD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_BMP_THD.u32)+u32Data*CHN_OFFSET));
    WBC_BMP_THD.bits.wbc_bmp_thd = u32HbiOsdThd;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->WBC_BMP_THD.u32)+u32Data*CHN_OFFSET),WBC_BMP_THD.u32);
}

HI_VOID VDP_DISP_SetHbiHcThd (HI_U32 u32Data, HI_U32 u32HbiHcThd)
{
    U_HC_BMP_THD  HC_BMP_THD;

    if(u32Data >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetHbiHcThd () Select Wrong Video Layer ID\n");
        return ;
    }

    HC_BMP_THD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->HC_BMP_THD.u32)+u32Data*CHN_OFFSET));
    HC_BMP_THD.bits.hc_bmp_thd = u32HbiHcThd;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->HC_BMP_THD.u32)+u32Data*CHN_OFFSET),HC_BMP_THD.u32);
}
#endif
HI_VOID VDP_DISP_SetNxgDataSwapEnable(HI_U32 u32hd_id, HI_U32 enable)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id>=CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_SetPlusPhase Select Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.fpga_lmt_en = enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_GetVactState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt)
{
    U_DHD0_STATE DHD0_STATE;

    if(u32hd_id >= CHN_MAX)
    {
        VDP_PRINT("Error,VDP_DISP_GetVactState Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_STATE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_STATE.u32)+u32hd_id*CHN_OFFSET));
    *pbBtm   = DHD0_STATE.bits.bottom_field;
    *pu32Vcnt = DHD0_STATE.bits.vcnt;

    return;
}


