//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_driver.cpp
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
// $Log: vdp_driver.cpp,v $
//
//
//
//
//**********************************************************************

#include "vdp_define.h"
#include "vdp_hal_comm.h"
#include "KdmTypeFxp.h"
#if VDP_CBB_FPGA
#include "vdp_fpga_define.h"
#endif
#if ESL_TEST
    #define SYS_TEST       1
#else
    #define SYS_TEST       0
#endif

#if EDA_TEST
  #define CPP_MODE 1
#else
  #define CPP_MODE 0
#endif

#if EDA_TEST
#include "vdp_fun.h"
S_VDP_REGS_TYPE *pVdpReg = new S_VDP_REGS_TYPE;
extern Driver *u_drv;
  #if (!BT_TEST & !FPGA_TEST)
  extern ahb_bus_sys *u_ahb_drv;
  #endif

#else//FPGA_TEST

S_VDP_REGS_TYPE *pVdpReg;
  #if VDP_CBB_FPGA
 // #include "vdp_util.h"
  extern char cOfName[256];
  extern char cIfName[256];
  //extern  volatile HI_U32 g_pOptmRegVirAddrTnrSnrDbm ;
  extern  volatile HI_U32 g_pOptmRegVirAddrDci ;
  #endif

#endif//EDA_TEST


//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
#if  0
HI_U32 VDP_RegRead(volatile HI_U32* a)
{

#if EDA_TEST
    HI_U32 offset = (HI_U64)a - (HI_U64)pVdpReg;
       
#if SYS_TEST    
    cout << hex << "cpu_read(0x" << setw(8) << setfill('0') << offset << ");" << endl;
#endif
    return u_drv->apb_read(VDP_BASE_ADDR + offset);
#elif VDP_CBB_FPGA
   //for VPSS reg access
	//if((offset>= 0xd800) (HI_ULONG)&(HI_ULONG)& (offset < 0xf000))
	{
		//return *(g_pOptmRegVirAddrTnrSnrDbm +(offset-0xd800)) ;
	}
   // else
    {
        return (*(a));
    }
#else
    return (*(a));
#endif

}
#endif

HI_U32 VDP_RegRead(HI_ULONG a)
{
	return (*((volatile HI_U32 *)a));
}

#if  0
HI_VOID VDP_RegWrite(volatile HI_U32* a, HI_U32 b)
{
  //  HI_U32 offset = (HI_U64)a - (HI_U64)pVdpReg;
    HI_U32 *pu32Src;
    HI_U64 offset ;

    pu32Src = (HI_U32 *)pVdpReg;
    
    offset= (HI_U64)(a-pu32Src);
	
	offset= offset*4;
#if EDA_TEST
    u_drv->apb_write(VDP_BASE_ADDR + offset, b); // DUT cfg
#endif

#if VDP_CBB_FPGA
   //for VPSS reg access
	if((offset>= 0xd800) && (offset < 0xf000))
	{
		//*(g_pOptmRegVirAddrTnrSnrDbm +(offset-0xd800))  = b;
	}
    else
    {
        *a = b;
    }
#else
    *a = b;       // ENV cfg
#endif

#if SYS_TEST    
	cout << hex << "cpu_write( 0x" << setw(8) << setfill('0') << offset << ", 0x" << b << ");" << endl;
#endif
    return ;
}


#endif
HI_VOID VDP_RegWrite(HI_ULONG  a, HI_U32 b)
{	
    *((volatile HI_U32 *)a) = b;
    return ;
}     


HI_U32 VDP_RegReadEx(HI_U32 a)
{
    //a : physical address   
#if SYS_TEST    
	cout << hex << "cpu_read(0x" << setw(8) << setfill('0') << a << ");" << endl;
#endif    

#if EDA_TEST
    return u_drv->apb_read(a);
#else
    return 0;
#endif    

}

HI_VOID VDP_RegWriteEx(HI_U32 a, HI_U32 b)
{
    //a : physical address   
	//if((a (HI_ULONG)& 0xffff0000) != VDP_BASE_ADDR)
	//{
    //    VDP_PRINT("Error,Invalid VDP Addr : 0x%x!\n",a);
	//	return;
	//}

	HI_U32 * Reg;
	Reg = (HI_U32*)pVdpReg + (a & 0x1ffff);
#if EDA_TEST    
    u_drv->apb_write(a, b); //cfg to rtl with apb
    *Reg = b;       // ENV cfg
#else
    *Reg = b;       // ENV cfg
#endif

#if FPGA_AUTORUN_CFG
		vWrDat2Txt(cIfName,((HI_U32)(Reg)),b);
#endif

#if SYS_TEST    
	cout << hex << "cpu_write(0x" << setw(8) << setfill('0') << a << ", 0x" << b << ");" << endl;
#endif

    return ;
}     

#if FPGA_TEST
extern HI_VOID reg2txt(HI_U32* a, HI_U32 b,HI_UCHAR*Filename);
#endif


HI_U32 AhbRead(HI_U32 a)
{
#if( EDA_TEST & !BT_TEST  & !FPGA_TEST)
     HI_U32 Val = 0;
     u_ahb_drv->read_ahb(a, &Val);
 
     return Val;
 #else
     return 0;
 #endif
 
 }
 

HI_VOID AhbWrite(HI_U32 a, HI_U32 b)
{
 #if ( EDA_TEST & !BT_TEST & !FPGA_TEST) 
     u_ahb_drv->write_ahb(a, b);
     //cout << "WriteWord : AHB addr =" ;
     //cout << hex << setw(8) << setfill('0') << a ;
     //cout << " : WriteData = " ;
     //cout << hex << setw(8) << setfill('0') << b << endl;
     //cout << hex << "1W 0x" << (HI_U32)a << " 0x" << b << endl;
 #endif
     return ;
}


HI_VOID vAxiRegSet(HI_U32 addr, HI_U32 val)
{
#if (EDA_TEST & !BT_TEST)    
    Cell cell;
    cell.addr = addr;
    cell.data = val;
    //cell.pa_va_flag = VPSS_LIST_ADDR_PA_VA;
    //cout << hex << "addr=0x" << cell.addr << ", data=0x" << cell.data << endl; 
    HiMemWordWr(&cell);
#endif
}
//--------------------------------------------------------------------
// AXI BUS BEING
//--------------------------------------------------------------------

HI_VOID VDP_SetVDPMemLowPower(HI_VOID)
{
    U_VOLOWPOWER_CTRL VOLOWPOWER_CTRL;
    
    VOLOWPOWER_CTRL.u32 = 0x00005b2b;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOLOWPOWER_CTRL.u32)), VOLOWPOWER_CTRL.u32);
}

HI_VOID VDP_SetClkGateEn(HI_U32 u32Data)
{
    U_VOCTRL VOCTRL;
    
    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetWrOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
{
    U_VOAXICTRL  VOAXICTRL;
    U_VOAXICTRL1 VOAXICTRL1;

    if(u32Data == VDP_MASTER0)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_wr_ostd = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER1)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_wr_ostd = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER2)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_wr_ostd = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER3)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_wr_ostd = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else
    {
        VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
    }
    
    return ;
}

HI_VOID VDP_SetRdOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
{
    U_VOAXICTRL  VOAXICTRL;
    U_VOAXICTRL1 VOAXICTRL1;

    if(u32Data == VDP_MASTER0)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_outstd_rid0 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_outstd_rid1 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER1)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_outstd_rid0 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_outstd_rid1 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER2)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_outstd_rid0 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_outstd_rid1 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER3)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_outstd_rid0 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_outstd_rid1 = u32OutStd;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32); 
        }
        else
        {
            VDP_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else
    {
        VDP_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
    }
    
    return ;
}


HI_VOID VDP_SetArbMode(HI_U32 u32Data, HI_U32 u32bMode)
{
    U_VOCTRL VOCTRL;

    if(u32Data == VDP_MASTER0)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m0_arb_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32); 
    }
    else if(u32Data == VDP_MASTER1)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m1_arb_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32); 
    }
    else if(u32Data == VDP_MASTER2)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m2_arb_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32); 
    }
    else if(u32Data == VDP_MASTER3)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m3_arb_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_SetArbMode() Select Wrong Master!\n");
    }

    return ;
}

HI_VOID VDP_SetRdMultiIdEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_VOAXICTRL  VOAXICTRL;

    if(u32Data == VDP_MASTER0)
    {
        VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
        VOAXICTRL.bits.m0_mutli_id_o = u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
    }
    else if(u32Data == VDP_MASTER1)
    {
        VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
        VOAXICTRL.bits.m1_mutli_id_o = u32Enable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_SetRdMultiIdEnable() Select Wrong Master!\n");
    }
    
    return ;
}

HI_VOID VDP_SetParaMasterSel  (HI_U32 u32MasterNum)
{
	U_VOMASTERSEL   VOMASTERSEL;
	U_VOMASTERSEL1  VOMASTERSEL1;

	VOMASTERSEL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)));
	VOMASTERSEL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)));

    if(u32MasterNum == VDP_MASTER0)
    {
        VOMASTERSEL.bits.para_master_sel = 0;
    }
    else if(u32MasterNum == VDP_MASTER1)
    {
        VOMASTERSEL.bits.para_master_sel = 1;
    }
    else if(u32MasterNum == VDP_MASTER2)
    {
        VOMASTERSEL1.bits.para_master_sel = 0;
    }
    else if(u32MasterNum == VDP_MASTER3)
    {
        VOMASTERSEL1.bits.para_master_sel = 1;
    }

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32); 
    
    return ;
}

HI_VOID VDP_SetAxiMidEnable  (HI_U32 u32Enable)
{
    U_VOMIDORDER    VOMIDORDER;

	VOMIDORDER.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMIDORDER.u32)));
    VOMIDORDER.bits.mid_enable = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMIDORDER.u32)), VOMIDORDER.u32); 
    
    return ;
}

HI_VOID VDP_SetAxiEdgeMode  (VDP_AXI_EDGE_MODE_E EdgeMode)
{
    U_VOMIDORDER    VOMIDORDER;

	VOMIDORDER.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMIDORDER.u32)));
    VOMIDORDER.bits.edge_mode = EdgeMode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMIDORDER.u32)), VOMIDORDER.u32); 
    
    return ;
}

HI_VOID VDP_VID_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetReqCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.req_ctrl = u32ReqCtrl;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}
HI_VOID VDP_GFX_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetReqCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.req_ctrl = u32ReqCtrl;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 

    return ;
}

//--------------------------------------------------------------------
// AXI BUS END 
//--------------------------------------------------------------------

#if 0

HI_VOID VDP_HDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;
    
    HDATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetLine(HI_U32 u32Data)
{
    U_HDATE_DACDET1 HDATE_DACDET1 ;
    
    HDATE_DACDET1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_DACDET1.u32));
    HDATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_DACDET1.u32), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    U_HDATE_DACDET1 HDATE_DACDET1 ;
    
    HDATE_DACDET1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_DACDET1.u32));
    HDATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_DACDET1.u32), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;
    
    HDATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetStart(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;
    
    HDATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}
#endif

HI_VOID VDP_SDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;
    
    DATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

    return ;
}

#if 0
HI_VOID VDP_SDATE_SetVdacDetLine(HI_U32 u32Data)
{
    U_DATE_DACDET1 DATE_DACDET1 ;
    
    DATE_DACDET1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_DACDET1.u32));
    DATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_DACDET1.u32), DATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    U_DATE_DACDET1 DATE_DACDET1 ;
    
    DATE_DACDET1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_DACDET1.u32));
    DATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_DACDET1.u32), DATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;
    
    DATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetStart(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;
    
    DATE_DACDET2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

    return ;
}

#endif

HI_VOID VDP_CBM_SetMixerBkg(VDP_CBM_MIX_E u32mixer_id, VDP_BKG_S stBkg)
{
    U_CBM_BKG1 CBM_BKG1;
    U_CBM_BKG2 CBM_BKG2;
    U_CBM_BKG3 CBM_BKG3;

    if(u32mixer_id == VDP_CBM_MIX0)
    {
        /* DHD0  mixer link */
        CBM_BKG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_BKG1.u32)));
        CBM_BKG1.bits.cbm_bkgy1  = stBkg.u32BkgY;
        CBM_BKG1.bits.cbm_bkgcb1 = stBkg.u32BkgU;
        CBM_BKG1.bits.cbm_bkgcr1 = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->CBM_BKG1.u32), CBM_BKG1.u32); 
    }
    else if(u32mixer_id == VDP_CBM_MIX1)
    {
        CBM_BKG2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_BKG2.u32)));
        CBM_BKG2.bits.cbm_bkgy2  = stBkg.u32BkgY;
        CBM_BKG2.bits.cbm_bkgcb2 = stBkg.u32BkgU;
        CBM_BKG2.bits.cbm_bkgcr2 = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->CBM_BKG2.u32), CBM_BKG2.u32); 
    }
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        /* DHD1 mixer link */
        CBM_BKG3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_BKG3.u32)));
        CBM_BKG3.bits.cbm_bkgy3  = stBkg.u32BkgY;
        CBM_BKG3.bits.cbm_bkgcb3 = stBkg.u32BkgU;
        CBM_BKG3.bits.cbm_bkgcr3 = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->CBM_BKG3.u32), CBM_BKG3.u32); 
    }
#if SD0_EN
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        U_MIXDSD_BKG MIXDSD_BKG;
        /* DSD mixer link */
        MIXDSD_BKG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXDSD_BKG.u32)));
        MIXDSD_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXDSD_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXDSD_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->MIXDSD_BKG.u32), MIXDSD_BKG.u32); 


    }
#endif

#if MIXG_EN
    else if(u32mixer_id == VDP_CBM_MIXG0)
    {
        U_MIXG0_BKG MIXG0_BKG;
        U_MIXG0_BKALPHA MIXG0_BKALPHA;
        /* G0 mixer link */
        MIXG0_BKG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_BKG.u32)));
        MIXG0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXG0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXG0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->MIXG0_BKG.u32), MIXG0_BKG.u32); 

        MIXG0_BKALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_BKALPHA.u32)));
        MIXG0_BKALPHA.bits.mixer_alpha  = stBkg.u32BkgA;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->MIXG0_BKALPHA.u32), MIXG0_BKALPHA.u32); 
    }
    else if(u32mixer_id == VDP_CBM_MIXV0)
    {
         U_MIXV0_BKG MIXV0_BKG;
        MIXV0_BKG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXV0_BKG.u32)));
        MIXV0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXV0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXV0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->MIXV0_BKG.u32), MIXV0_BKG.u32); 
    }
#endif
    else
    {
        VDP_PRINT("Error! VDP_CBM_SetMixerBkg() Select Wrong mixer ID\n");
    }

    return ;
}

HI_VOID VDP_CBM_Clear_MixvPrio(HI_U32 u32layer_id)
{	
    U_MIXV0_MIX MIXV0_MIX;	
	MIXV0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)));

	if (MIXV0_MIX.bits.mixer_prio0 == (u32layer_id + 1))
	{
         MIXV0_MIX.bits.mixer_prio0 = 0;
	}
	
	if (MIXV0_MIX.bits.mixer_prio1 == (u32layer_id + 1))
	{
		MIXV0_MIX.bits.mixer_prio1 = 0;
	}
	
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);

	return;
}

HI_VOID VDP_CBM_SetMixerPrio(VDP_CBM_MIX_E u32mixer_id, HI_U32 u32layer_id,HI_U32 u32prio)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
    U_CBM_MIX3 CBM_MIX3;

	/* asic err code */
	if (u32layer_id == 4)
		return;

    if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            case 1:
            {
                CBM_MIX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
#if 0
//only 2 layers in mixer when vp (HI_ULONG)& gp is enabled
            case 2:
            {
                CBM_MIX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio2 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            case 3:
            {
                CBM_MIX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio3 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
#endif
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }

        }
    }
#if MIXG_EN
    else if (u32mixer_id == VDP_CBM_MIXG0)
    {
        U_MIXG0_MIX MIXG0_MIX;
        switch(u32prio)
        {
            case 0:
            {
                MIXG0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 1:
            {
                MIXG0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 2:
            {
                MIXG0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio2 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 3:
            {
                MIXG0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio3 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }

        }
    }
    else if(u32mixer_id == VDP_CBM_MIXG1)
        ;
    else if(u32mixer_id == VDP_CBM_MIXV0)
    {
       U_MIXV0_MIX MIXV0_MIX;
        switch(u32prio)
        {
            case 0:
            {
                MIXV0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)));
                MIXV0_MIX.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
                break;
            }
            case 1:
            {
                MIXV0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)));
                MIXV0_MIX.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
                break;
            }
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }

        }
    }
    else if(u32mixer_id == VDP_CBM_MIXV1)
        ;
#endif
#if 1//for STB/BVT
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            //mask by hyx
            //case 2:
            //{
			//	CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
			//	CBM_MIX2.bits.mixer_prio2 = u32layer_id;
			//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
			//	break;
            //}
            //case 3:
            //{
			//	CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
			//	CBM_MIX2.bits.mixer_prio3 = u32layer_id;
			//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
			//	break;
            //}
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }


        }
    }
#endif
    else if(u32mixer_id == VDP_CBM_MIX2)//DHD1
    {
        HI_U32  u32layer_id_use;
       // if(u32layer_id == VDP_CBM_GFX3)
       //     u32layer_id_use = 2;
       // else
            u32layer_id_use = u32layer_id;
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio0 = u32layer_id_use;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio1 = u32layer_id_use;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }
        }
    }
	
    else if(u32mixer_id == VDP_CBM_MIX3)//DHD1
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 1:
            {
                CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 2:
            {
				CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
				CBM_MIX3.bits.mixer_prio2 = u32layer_id;
				VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
				break;
            }
            //case 3:
            //{
			//	CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
			//	CBM_MIX3.bits.mixer_prio3 = u32layer_id;
			//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
			//	break;
            //}
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }


        }
    }
#if SD0_EN
    else if(u32mixer_id == VDP_CBM_MIX2)//DSD
    {
        U_MIXDSD_MIX MIXDSD_MIX;
        switch(u32prio)
        {
            case 0:
            {
                MIXDSD_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            case 1:
            {
                MIXDSD_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            case 2:
            {
                MIXDSD_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio2 = u32layer_id;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }
        }


    }
#endif
#if 0
    else if(u32mixer_id == VDP_CBM_MIX2)//MIX_4K
    {
        if(u32layer_id == VDP_CBM_CBM)
            u32Layer = 1;
        else if(u32layer_id == VDP_CBM_GFX3)//hc
            u32Layer = 2;
        else
            u32Layer = 0;

        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio0 = u32Layer;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 1:
            {
                CBM_MIX3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio1 = u32Layer;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            default:
            {
                VDP_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }


        }
    }
#endif


}

HI_VOID VDP_CBM_ResetMixerPrio(VDP_CBM_MIX_E u32mixer_id)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
    U_CBM_MIX3 CBM_MIX3;

    if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        CBM_MIX1.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
    }
#if MIXG_EN
    else if (u32mixer_id == VDP_CBM_MIXG0)
    {
        U_MIXG0_MIX MIXG0_MIX;
        MIXG0_MIX.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV0)
    {
        U_MIXV0_MIX MIXV0_MIX;
        MIXV0_MIX.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV1)
    {
        //reserved
    }
#endif
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        CBM_MIX2.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
    }
#if 1
    else if(u32mixer_id == VDP_CBM_MIX2)//MIX_4K
    {
        CBM_MIX3.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
    }
#endif
#if SD0_EN
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        U_MIXDSD_MIX MIXDSD_MIX;
        MIXDSD_MIX.u32 = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
    }
#endif
    else
    {
        VDP_PRINT("Error, VDP_CBM_ResetMixerPrio() Set mixer  select wrong layer ID\n");
    }

    return ;
}

HI_VOID VDP_SetCheckSumEnable(HI_U32  bEnable)
{
    U_VOCTRL  VOCTRL;
    
    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.chk_sum_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetTwoChnEnable(HI_U32 bEnable)
{
    U_VOCTRL  VOCTRL;
    
    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.twochn_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetTwoChnMode(HI_U32 u32Data)
{
    U_VOCTRL  VOCTRL;
    
    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.twochn_mode = 1;//u32Data;//in HiFoneB02 twochn_mode = 1 only
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);

}

HI_VOID VDP_SetTwoChnDbgEnable(HI_U32 bEnable)
{
    U_DHD0_CTRL  DHD0_CTRL;
    
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)));
    DHD0_CTRL.bits.twochn_debug = 1;//bEnable;//in HiFoneB02 twochn_mode = 1 only
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);

}

HI_VOID VDP_SetSrConnection(HI_U32 u32Data)
{
    U_DHD0_SRSEL  DHD0_SRSEL;
    
    DHD0_SRSEL.u32 = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SRSEL.u32)), DHD0_SRSEL.u32);
}

HI_BOOL SDATE_SetSysMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)));		
    DATE_COEFF0.bits.scanline = u32Data;     
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32); 
	
    return HI_TRUE;
}

