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


#include "vdp_drv_comm.h"
#include "vdp_define.h"


extern char cOfName[256];
extern char cIfName[256];


S_VDP_REGS_TYPE *pVdpReg;



HI_U32 VDP_RegRead(HI_U32 a)
{
    //a : addr of reg struct
    //HI_U32 addr = VDP_BASE_ADDR + ((HI_U32)a - (HI_U32)pVdpReg);

    return (*((HI_U32 *)a));

}

extern HI_VOID reg2txt(HI_U32* a, HI_U32 b,HI_UCHAR*Filename);

HI_VOID VDP_RegWrite(HI_U32 a, HI_U32 b)
{
    *((HI_U32 *)a) = b;
#if FPGA_AUTORUN_CFG
    HI_U32 u32_det_addr = a;
    HI_U32 u32_ori_addr = (&(pVdpReg->VOCTRL.u32));
    vWrDat2Txt(cIfName,(u32_det_addr-u32_ori_addr),b);
#endif

#if REG_LOG
    	reg2txt((HI_U32 *) ((HI_U32)a&0xffff), b, "reg_vdp.txt");
#endif


    return ;
}

HI_U32 VDP_RegReadEx(HI_U32 a)
{
    //a : physical address
    return 0;
}

HI_VOID VDP_RegWriteEx(HI_U32 a, HI_U32 b)
{
	HI_U32 * Reg;
	Reg = (HI_U32 *)((HI_U32)pVdpReg + (a & 0xffff));
    *Reg = b;       // ENV cfg
#if FPGA_AUTORUN_CFG
		vWrDat2Txt(cIfName,((HI_U32)(Reg)),b);
#endif


    return ;
}

HI_VOID VDP_SetClkGateEn(HI_U32 u32Data)
{
    U_VOCTRL VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetWrOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
{
    U_VOAXICTRL VOAXICTRL;
    U_VOAXICTRL1 VOAXICTRL1;

    if(u32Data == VDP_MASTER0)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_wr_ostd = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER1)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_wr_ostd = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER2)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_wr_ostd = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else if(u32Data == VDP_MASTER3)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_wr_ostd = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
        }
    }
    else
    {
        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
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
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_outstd_rid0 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m0_outstd_rid1 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER1)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_outstd_rid0 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL.u32)));
            VOAXICTRL.bits.m1_outstd_rid1 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL.u32)), VOAXICTRL.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER2)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_outstd_rid0 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m2_outstd_rid1 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else if(u32Data == VDP_MASTER3)
    {
        if(u32BusId == 0)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_outstd_rid0 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else if(u32BusId == 1)
        {
            VOAXICTRL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)));
            VOAXICTRL1.bits.m3_outstd_rid1 = u32OutStd;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VOAXICTRL1.u32)), VOAXICTRL1.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
        }
    }
    else
    {
        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
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
        VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m0_arb_mode = u32bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
    }
    else if(u32Data == VDP_MASTER1)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m1_arb_mode = u32bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
    }
    else if(u32Data == VDP_MASTER2)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m2_arb_mode = u32bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
    }
    else if(u32Data == VDP_MASTER3)
    {
        //set vdp arbitration mode
        //0:Poll Mode,1:GFX First Mode
        VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
        VOCTRL.bits.m3_arb_mode = u32bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_SetArbMode() Select Wrong Master!\n");
    }

    return ;
}

#if 0

HI_VOID VDP_HDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite(&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetLine(HI_U32 u32Data)
{
    U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead(&(pVdpReg->HDATE_DACDET1.u32));
    HDATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite(&(pVdpReg->HDATE_DACDET1.u32), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead(&(pVdpReg->HDATE_DACDET1.u32));
    HDATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite(&(pVdpReg->HDATE_DACDET1.u32), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite(&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetStart(HI_U32 u32Data)
{
    U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->HDATE_DACDET2.u32));
    HDATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite(&(pVdpReg->HDATE_DACDET2.u32), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite(&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetLine(HI_U32 u32Data)
{
    U_DATE_DACDET1 DATE_DACDET1 ;

    DATE_DACDET1.u32 = VDP_RegRead(&(pVdpReg->DATE_DACDET1.u32));
    DATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite(&(pVdpReg->DATE_DACDET1.u32), DATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    U_DATE_DACDET1 DATE_DACDET1 ;

    DATE_DACDET1.u32 = VDP_RegRead(&(pVdpReg->DATE_DACDET1.u32));
    DATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite(&(pVdpReg->DATE_DACDET1.u32), DATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite(&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetStart(HI_U32 u32Data)
{
    U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead(&(pVdpReg->DATE_DACDET2.u32));
    DATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite(&(pVdpReg->DATE_DACDET2.u32), DATE_DACDET2.u32);

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
        CBM_BKG1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_BKG1.u32)));;
        CBM_BKG1.bits.cbm_bkgy1  = stBkg.u32BkgY;
        CBM_BKG1.bits.cbm_bkgcb1 = stBkg.u32BkgU;
        CBM_BKG1.bits.cbm_bkgcr1 = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG1.u32), CBM_BKG1.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIX1)
    {
        CBM_BKG2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_BKG2.u32)));;
        CBM_BKG2.bits.cbm_bkgy2  = stBkg.u32BkgY;
        CBM_BKG2.bits.cbm_bkgcb2 = stBkg.u32BkgU;
        CBM_BKG2.bits.cbm_bkgcr2 = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG2.u32), CBM_BKG2.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        /* DHD1 mixer link */
        CBM_BKG3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_BKG3.u32)));;
        CBM_BKG3.bits.cbm_bkgy3  = stBkg.u32BkgY;
        CBM_BKG3.bits.cbm_bkgcb3 = stBkg.u32BkgU;
        CBM_BKG3.bits.cbm_bkgcr3 = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG3.u32), CBM_BKG3.u32);
    }
#if SD0_EN
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        U_MIXDSD_BKG MIXDSD_BKG;
        /* DSD mixer link */
        MIXDSD_BKG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXDSD_BKG.u32)));;
        MIXDSD_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXDSD_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXDSD_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->MIXDSD_BKG.u32), MIXDSD_BKG.u32);


    }
#endif

#if MIXG_EN
    else if(u32mixer_id == VDP_CBM_MIXG0)
    {
        U_MIXG0_BKG MIXG0_BKG;
        U_MIXG0_BKALPHA MIXG0_BKALPHA;
        /* G0 mixer link */
        MIXG0_BKG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_BKG.u32)));;
        MIXG0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXG0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXG0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->MIXG0_BKG.u32), MIXG0_BKG.u32);

        MIXG0_BKALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_BKALPHA.u32)));;
        MIXG0_BKALPHA.bits.mixer_alpha  = stBkg.u32BkgA;
        VDP_RegWrite((HI_U32)&(pVdpReg->MIXG0_BKALPHA.u32), MIXG0_BKALPHA.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIXV0)
    {
         U_MIXV0_BKG MIXV0_BKG;
        MIXV0_BKG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_BKG.u32)));;
        MIXV0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXV0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXV0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->MIXV0_BKG.u32), MIXV0_BKG.u32);
    }
#endif
    else
    {
        HI_PRINT("Error! VDP_CBM_SetMixerBkg() Select Wrong mixer ID\n");
    }

    return ;
}

HI_VOID VDP_CBM_SetMixerPrio(VDP_CBM_MIX_E u32mixer_id, HI_U32 u32layer_id,HI_U32 u32prio)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
    U_CBM_MIX3 CBM_MIX3;


    if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            case 1:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
#if 0
//only 2 layers in mixer when vp & gp is enabled
            case 2:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio2 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            case 3:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio3 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
#endif
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
                MIXG0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 1:
            {
                MIXG0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 2:
            {
                MIXG0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio2 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 3:
            {
                MIXG0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio3 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)));
                MIXV0_MIX.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
                break;
            }
            case 1:
            {
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)));
                MIXV0_MIX.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            //mask by hyx
            //case 2:
            //{
			//	CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
			//	CBM_MIX2.bits.mixer_prio2 = u32layer_id;
			//	VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
			//	break;
            //}
            //case 3:
            //{
			//	CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
			//	CBM_MIX2.bits.mixer_prio3 = u32layer_id;
			//	VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
			//	break;
            //}
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }


        }
    }
#endif
    else if(u32mixer_id == VDP_CBM_MIX2)//DHD1
    {
        HI_U32  u32layer_id_use;
        if(u32layer_id == VDP_CBM_GFX3)
            u32layer_id_use = 2;
        else
            u32layer_id_use = u32layer_id;

        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio0 = u32layer_id_use;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio1 = u32layer_id_use;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
                CBM_MIX3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 1:
            {
                CBM_MIX3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 2:
            {
                CBM_MIX3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio2 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            default:
            {
                HI_PRINT("%s: Set mixer  select wrong layer ID\n", __func__);
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
                MIXDSD_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio0 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            case 1:
            {
                MIXDSD_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio1 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            case 2:
            {
                MIXDSD_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)));
                MIXDSD_MIX.bits.mixer_prio2 = u32layer_id;
                VDP_RegWrite((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
                CBM_MIX3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio0 = u32Layer;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            case 1:
            {
                CBM_MIX3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX3.u32)));
                CBM_MIX3.bits.mixer_prio1 = u32Layer;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
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
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
    }
#if MIXG_EN
    else if (u32mixer_id == VDP_CBM_MIXG0)
    {
        U_MIXG0_MIX MIXG0_MIX;
        MIXG0_MIX.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV0)
    {
        U_MIXV0_MIX MIXV0_MIX;
        MIXV0_MIX.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV1)
    {
        //reserved
    }
#endif
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        CBM_MIX2.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
    }
#if 1
    else if(u32mixer_id == VDP_CBM_MIX2)//MIX_4K
    {
        CBM_MIX3.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
    }
#endif
#if SD0_EN
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        U_MIXDSD_MIX MIXDSD_MIX;
        MIXDSD_MIX.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXDSD_MIX.u32)), MIXDSD_MIX.u32);
    }
#endif
    else
    {
        HI_PRINT("Error, VDP_CBM_ResetMixerPrio() Set mixer  select wrong layer ID\n");
    }

    return ;
}

HI_VOID VDP_SetCheckSumEnable(HI_U32  bEnable)
{
    U_VOCTRL  VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.chk_sum_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetTwoChnEnable(HI_U32 bEnable)
{
    U_VOCTRL  VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.twochn_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}
#if  1
HI_VOID VDP_SetTwoChnMode(HI_U32 bMode)
{
    U_VOCTRL  VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.twochn_mode = bMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}
#endif
HI_VOID VDP_SetTwoChnDbgEnable(HI_U32 bEnable)
{
    U_DHD0_CTRL  DHD0_CTRL;

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)));

    DHD0_CTRL.bits.twochn_debug = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);
}

HI_VOID VDP_SetSrConnection(HI_U32 u32Data)
{
    U_DHD0_SRSEL  DHD0_SRSEL;

    DHD0_SRSEL.u32 = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SRSEL.u32)), DHD0_SRSEL.u32);
}
