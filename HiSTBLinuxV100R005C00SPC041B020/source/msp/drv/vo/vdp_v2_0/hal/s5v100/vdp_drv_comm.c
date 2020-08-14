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
#include "hi_debug.h"

volatile S_VDP_REGS_TYPE *pVdpReg = HI_NULL;

#if (!defined __VDP_ST__) && (!defined __DISP_PLATFORM_BOOT__)
#include <asm/io.h>
volatile HI_VOID __iomem *g_pv2dTo3dReg;
#else
#define iounmap         HI_MUNMAP
volatile HI_VOID *g_pv2dTo3dReg;
#endif

#define VDP_YCBCR_Y_MIN             (16 << 2)           /* 10bit Y最小值 */
#define VDP_YCBCR_Y_MAX             (235 << 2 | 0x3)    /* 10bit Y最大值 */
#define VDP_YCBCR_CBCR_MIN          (16 << 2)           /* 10bit Cb/Cr最小值 */
#define VDP_YCBCR_CBCR_MAX          (240 << 2 | 0x3)    /* 10bit Cb/Cr最大值 */


HI_U32 VDP_RegRead(volatile HI_U32 a)
{
    return (*((HI_U32 *)a));
}

HI_VOID VDP_RegWrite(volatile HI_U32 a, HI_U32 b)
{
    *((HI_U32 *)a) = b;

    return ;
}


HI_VOID VDP_REG_DeInit(HI_VOID)
{

#ifndef __DISP_PLATFORM_BOOT__

    if (pVdpReg)
    {
        iounmap(pVdpReg);
    }

    if (g_pv2dTo3dReg)
    {
        iounmap(g_pv2dTo3dReg);
    }

#else
    pVdpReg         = (S_VDP_REGS_TYPE *)OPTM_REGS_BASE_ADDR;
    g_pv2dTo3dReg   = (HI_VOID *)HIMAX_REGS_BASE_ADDR;
#endif

    pVdpReg = HI_NULL;
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
    U_VOAXICTRL VOAXICTRL;
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

HI_VOID VDP_SetM0AndM1Sel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
    U_VOMASTERSEL  VOMASTERSEL;

    VOMASTERSEL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)));

    if(u32Data == VDP_LAYER_MASTER_V0)
    {
        VOMASTERSEL.bits.v0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V1)
    {
        VOMASTERSEL.bits.v1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V2)
    {
        VOMASTERSEL.bits.v2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V3)
    {
        VOMASTERSEL.bits.v3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V4)
    {
        VOMASTERSEL.bits.v4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V5)
    {
        VOMASTERSEL.bits.v5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V6)
    {
        VOMASTERSEL.bits.v6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V7)
    {
        VOMASTERSEL.bits.v4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V8)
    {
        VOMASTERSEL.bits.v8_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V9)
    {
        VOMASTERSEL.bits.v9_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G0)
    {
        VOMASTERSEL.bits.g0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G1)
    {
        VOMASTERSEL.bits.g1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G2)
    {
        VOMASTERSEL.bits.g2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G3)
    {
        VOMASTERSEL.bits.g3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G4)
    {
        VOMASTERSEL.bits.g4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G5)
    {
        VOMASTERSEL.bits.g5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G6)
    {
        VOMASTERSEL.bits.g6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G7)
    {
        VOMASTERSEL.bits.g7_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC0)
    {
        VOMASTERSEL.bits.wbc0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC1)
    {
        VOMASTERSEL.bits.wbc1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC2)
    {
        VOMASTERSEL.bits.wbc2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC3)
    {
        VOMASTERSEL.bits.wbc3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC4)
    {
        VOMASTERSEL.bits.wbc4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC5)
    {
        VOMASTERSEL.bits.wbc5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC6)
    {
        VOMASTERSEL.bits.wbc6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC7)
    {
        VOMASTERSEL.bits.wbc7_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_PARA)
    {
        VOMASTERSEL.bits.para_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_SDDATE)
    {
        VOMASTERSEL.bits.sddate_master_sel = u32MasterNum;
    }
    else
    {
        HI_PRINT("Error,VDP_SetMasterSel() Select Wrong Master!\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32);

    return ;

}

HI_VOID VDP_SetM2AndM3Sel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
    U_VOMASTERSEL1  VOMASTERSEL1;

    VOMASTERSEL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)));

    if(u32Data == VDP_LAYER_MASTER_V0)
    {
        VOMASTERSEL1.bits.v0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V1)
    {
        VOMASTERSEL1.bits.v1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V2)
    {
        VOMASTERSEL1.bits.v2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V3)
    {
        VOMASTERSEL1.bits.v3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V4)
    {
        VOMASTERSEL1.bits.v4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V5)
    {
        VOMASTERSEL1.bits.v5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V6)
    {
        VOMASTERSEL1.bits.v6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V7)
    {
        VOMASTERSEL1.bits.v4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V8)
    {
        VOMASTERSEL1.bits.v8_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_V9)
    {
        VOMASTERSEL1.bits.v9_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G0)
    {
        VOMASTERSEL1.bits.g0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G1)
    {
        VOMASTERSEL1.bits.g1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G2)
    {
        VOMASTERSEL1.bits.g2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G3)
    {
        VOMASTERSEL1.bits.g3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G4)
    {
        VOMASTERSEL1.bits.g4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G5)
    {
        VOMASTERSEL1.bits.g5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G6)
    {
        VOMASTERSEL1.bits.g6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_G7)
    {
        VOMASTERSEL1.bits.g7_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC0)
    {
        VOMASTERSEL1.bits.wbc0_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC1)
    {
        VOMASTERSEL1.bits.wbc1_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC2)
    {
        VOMASTERSEL1.bits.wbc2_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC3)
    {
        VOMASTERSEL1.bits.wbc3_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC4)
    {
        VOMASTERSEL1.bits.wbc4_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC5)
    {
        VOMASTERSEL1.bits.wbc5_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC6)
    {
        VOMASTERSEL1.bits.wbc6_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_WBC7)
    {
        VOMASTERSEL1.bits.wbc7_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_PARA)
    {
        VOMASTERSEL1.bits.para_master_sel = u32MasterNum;
    }
    else if(u32Data == VDP_LAYER_MASTER_SDDATE)
    {
        VOMASTERSEL1.bits.sddate_master_sel = u32MasterNum;
    }
    else
    {
        HI_PRINT("Error,VDP_SetMasterSel() Select Wrong Master!\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32);

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
        /* DHD1 mixer link */
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

HI_VOID VDP_CBM_SetMixerPrio(VDP_CBM_MIX_E u32mixer_id,HI_U32 u32layer_id,HI_U32 u32prio)
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
//only 2 layer in mixer when vp & gp is enabled
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
                HI_PRINT("%s: Set mixer  select wrong layer ID\n", __func__);
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
                HI_PRINT("%s: Set mixer  select wrong layer ID\n", __func__);
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
                HI_PRINT(" %s: Set mixer  select wrong layer ID\n", __func__);
                return ;
            }

        }
    }
    else if(u32mixer_id == VDP_CBM_MIXV1)
        ;
#endif
#if 0//for STB/BVT
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
            //  CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
            //  CBM_MIX2.bits.mixer_prio2 = u32layer_id;
            //  VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
            //  break;
            //}
            //case 3:
            //{
            //  CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
            //  CBM_MIX2.bits.mixer_prio3 = u32layer_id;
            //  VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
            //  break;
            //}
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }


        }
    }
#endif
    else if(u32mixer_id == VDP_CBM_MIX2)//s5v1 mix2
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
                HI_PRINT("%s: Set mixer  select wrong layer ID\n", __func__);
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
                HI_PRINT("%s: Set mixer  select wrong layer ID\n", __func__);
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
    U_CBM_MIX2 CBM_MIX3;

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
    else if(u32mixer_id == VDP_CBM_MIX2)
    {
        CBM_MIX2.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
    }
#if 0
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
    else if(u32mixer_id == VDP_CBM_MIX3)//DHD1
    {
        CBM_MIX3.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX3.u32)), CBM_MIX3.u32);
    }
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

HI_VOID VDP_SetTwoChnDbgEnable(HI_U32 bEnable)
{
    U_DHD0_CTRL  DHD0_CTRL;

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)));
    DHD0_CTRL.bits.twochn_debug = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);
}
HI_VOID VDP_SetSrConnection(VDP_SR_CONN_E enConnectPosition)
{
    U_DHD0_SRSEL  DHD0_SRSEL;

    DHD0_SRSEL.u32 = enConnectPosition;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SRSEL.u32)), DHD0_SRSEL.u32);
}

HI_VOID VDP_CBM_SetTestPattern(VDP_BKG_S stVdpBg)
{
    U_CBM_BKG2 CBM_BKG2;

    CBM_BKG2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_BKG2.u32)));;
    CBM_BKG2.bits.cbm_bkgy2  = stVdpBg.u32BkgY;
    CBM_BKG2.bits.cbm_bkgcb2 = stVdpBg.u32BkgU;
    CBM_BKG2.bits.cbm_bkgcr2 = stVdpBg.u32BkgV;
    VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG2.u32), CBM_BKG2.u32);
}


HI_U32 VdpRgb2Yuv709Y(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B)
{
    HI_S32 r = (HI_S32)(u32R);
    HI_S32 g = (HI_S32)(u32G);
    HI_S32 b = (HI_S32)(u32B);
    HI_S32 y;

    y = ((182*r + 626*g + 63*b) / 1000 + 64);
    if (y < VDP_YCBCR_Y_MIN)
        y = VDP_YCBCR_Y_MIN;
    if (y > VDP_YCBCR_Y_MAX)
        y = VDP_YCBCR_Y_MAX;

    return (HI_U32)y;
}

HI_U32 VdpRgb2Yuv709Cb(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B)
{
    HI_S32 r = (HI_S32)(u32R);
    HI_S32 g = (HI_S32)(u32G);
    HI_S32 b = (HI_S32)(u32B);
    HI_S32 cb;

    cb = ((438*b - 338*g - 98*r) /1000 + 512);
    if (cb < VDP_YCBCR_CBCR_MIN)
        cb = VDP_YCBCR_CBCR_MIN;
    if (cb > VDP_YCBCR_CBCR_MAX)
        cb = VDP_YCBCR_CBCR_MAX;

    return (HI_U32)cb;
}

HI_U32 VdpRgb2Yuv709Cr(HI_U32 u32R, HI_U32 u32G, HI_U32 u32B)
{
    HI_S32 r = (HI_S32)(u32R);
    HI_S32 g = (HI_S32)(u32G);
    HI_S32 b = (HI_S32)(u32B);
    HI_S32 cr;

    cr = ((428*r - 397*g - 40*b) / 1000 + 512);
    if (cr < VDP_YCBCR_CBCR_MIN)
        cr = VDP_YCBCR_CBCR_MIN;
    if (cr > VDP_YCBCR_CBCR_MAX)
        cr = VDP_YCBCR_CBCR_MAX;

    return (HI_U32)cr;
}
