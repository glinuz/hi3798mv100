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
//#include <linux/delay.h>
//#include <linux/timer.h>
//#include <stdio.h>
#ifdef HI_DISP_BUILD_FULL
#include <linux/kernel.h>
#endif

#include "hi_reg_common.h"
#include "vdp_driver.h"
//#include "hi_reg_vdp.h"
#include "drv_disp_rwzb.h"
#include "hi_reg_common.h"


volatile S_VOU_CV200_REGS_TYPE *pVdpReg = HI_NULL;
#if 0
static VDP_DISP_SYNCINFO_S s_stSyncTiming[] =
{
/* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
/* Synm,Iop, Itf, Vact,Vbb,Vfb,  Hact, Hbb,Hfb,Hmid,  Bvact,Bvbb,Bvfb, Hpw,Vpw, Idv,Ihs,Ivs */
  //0 HI_UNF_ENC_FMT_PAL
  {0,   0,   0,   288,  22,  2,  720, 132, 12,     288,  23,  2,    126, 3, 0, 0,  0,  0},/* 576I(PAL) */
  //576I: HDMI输出要求hmid=300, 而YPbPr要求hmid=0,
  //考虑一般用户不会使用HDMI输出576I，所以不支持HDMI_567I输出，选择hmid=0
  //1 HI_UNF_ENC_FMT_NTSC
  {0,   0,   0,   240,  18,  4,   720, 119, 19,     240,  19,  4,    124, 3,  0, 0, 0,  0},/* 480I(NTSC) */
  //480I: HDMI输出要求hmid=310, 而YPbPr要求hmid=0,
  //考虑一般用户不会使用HDMI输出480I，所以不支持HDMI_480I输出，选择hmid=0
  //2 HI_UNF_ENC_FMT_720P_60
  {1,   1,   2,   720,  25,  5,  1280, 260,110,      1,   1,  1,    40,  5,  1, 0,  0,  0}, /* 720P@60Hz */
  //3 HI_UNF_ENC_FMT_1080i_60
  {1,   0,   2,   540,  20,  2,  1920, 192, 88,    21,  2,    44,  5, 908, 540, 0,  0,  0}, /* 1080I@60Hz */
  //4 HI_UNF_ENC_FMT_1080P_30
  {1,   1,   2,  1080,  41,  4,  1920, 192, 88,       1,   1,  1,    44,  5, 1, 0,  0,  0}, /* 1080P@30Hz */
  //5~9, LCD
  {1,   1,   2,   600,  27,  1,   800, 216, 40,       1,   1,  1,    128, 4, 1, 0,  0,  0}, /* 800*600@60Hz */
  {1,   1,   2,   768,  35,  3,  1024, 296, 24,      1,   1,  1,    136, 6, 1,  0,  0,  0}, /* 1024x768@60Hz */
  {1,   1,   2,  1024,  41,  1,  1280, 360, 48,      1,   1,  1,    112, 3, 1,  0,  0,  0}, /* 1280x1024@60Hz */
  {1,   1,   2,   768,  27,  3,  1366, 356, 70,      1,   1,  1,    143, 3, 1,  0,  0,  0}, /* 1366x768@60Hz */
  {1,   1,   2,   900,  23,  3,  1440, 112, 48,     1,   1,  1,    32, 6,   1, 0,  0,  0}, /* 1440x900@60Hz@88.75MHz */
  //10 HI_UNF_ENC_FMT_720P_50
  {1,   1,   2,   720,  25,  5,  1280, 260,440,     1,   1,  1,     40, 5,  1,  0,  0,  0},  /* 720P@50Hz */
  //11 HI_UNF_ENC_FMT_1080i_50
  {1,   0,   2,   540,  20,  2,  1920, 192,528, 540,  21,  2,     44, 5, 1128,  0,  0,  0}, /* 1080I@50Hz */
  //12 HI_UNF_ENC_FMT_1080P_60,
  {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
  //13 HI_UNF_ENC_FMT_1080P_50,
  {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,     44, 5, 1, 0,  0,  0}, /* 1080P@50Hz */
  //14 HI_UNF_ENC_FMT_1080P_25,
  {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,    44, 5, 1,  0,  0,  0}, /* 1080P@25Hz */
  //15 HI_UNF_ENC_FMT_1080P_24 @74.25MHz,
  {1,   1,   2,  1080,  41,  4,  1920, 192, 638,       1,   1,  1,    44, 5, 1, 0,  0,  0}, /* 1080P@24Hz */
  /* Synm,Iop, Itf,Vact,Vbb,Vfb,Hact, Hbb,Hfb, Bvact,Bvbb,Bvfb, Hpw,Vpw, Idv,Ihs,Ivs */
  //16 HI_UNF_ENC_FMT_576P_50,
  {1,  1,   2,   576,   44,  5,   720, 132, 12,     1,   1,  1,     64, 5,  1,  0,  0,  0}, /* 576P@50Hz */
    /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
  /*Synm,Iop, Itf, Vact,Vbb,Vfb,  Hact, Hbb,Hfb,Hmid,  Bvact,Bvbb,Bvfb, Hpw,Vpw, Idv,Ihs,Ivs */
  //17 HI_UNF_ENC_FMT_480P_60,
  {1,  1,   2,   480,   36,  9,   720, 122, 16,     1,   1,  1,     62, 6,  1,  0,  0,  0}, /* 480P@60Hz */

  //18~20
  {1,   1,   2,   900,  31,  3,  1440, 384, 80,    1,   1,  1,    152, 6,   1,  0,  0,  0}, /* 1440x900@60Hz@106.5MHz */
  {1,   1,   2,   480,  35,  10,  640, 144, 16,       1,   1,  1,      96, 2,  1, 0,  0,  0}, /* 640*480@60Hz */
  {1,   1,   2,   1200,  49,  1, 1600, 496, 64,       1,   1,  1,     192, 3, 1, 0,  0,  0}, /* 1600*12000@60Hz */

  //21 HI_UNF_ENC_FMT_PAL_TEST
  {0,   0,   2,   288,  22,  2,  1440, 132, 12,    288,  23,  2,    126, 3,  0, 0,  0,  0},/* 576I(PAL) */

};
#endif

//-------------------------------------------------
//
//-------------------------------------------------

//S_VOU_V400_REGS_TYPE g_VDPDebugDevice;
HI_VOID  VDP_DRIVER_SetVirtualAddr(HI_U32 virAddr)
{
    pVdpReg = (S_VOU_CV200_REGS_TYPE *)virAddr;
}

HI_VOID  VDP_DRIVER_Initial()
{
    U_CBM_ATTR CBM_ATTR;
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
	U_VOCTRL VOCTRL;

    /* initail layer link */
    CBM_ATTR.u32 = pVdpReg->CBM_ATTR.u32;
    //CBM_ATTR.bits.sur_attr5 = 0;  /*g3 to mixer1*/
    CBM_ATTR.bits.sur_attr4 = 0;  /*v4 to mixer1  free mode */
    CBM_ATTR.bits.sur_attr3 = 1;  /*gb to mixer2*/
    CBM_ATTR.bits.sur_attr2 = 1;  /*v3 to mixer2*/
    CBM_ATTR.bits.sur_attr1 = 0;  /*ga to mixer1*/
    CBM_ATTR.bits.sur_attr0 = 0;  /*ga to mixer1*/
    pVdpReg->CBM_ATTR.u32 = CBM_ATTR.u32;

    VDP_SetRdOutStd(VDP_MASTER0, 0, 7);
    VDP_SetRdOutStd(VDP_MASTER0, 1, 7);
    VDP_SetWrOutStd(VDP_MASTER0, 0, 4);

    /* initail video mixer */
    VDP_MIXV_SetPrio(VDP_LAYER_VID0);
    VDP_MIXV_SetPrio1(VDP_LAYER_VID1);

    /* initail mixer1, clear all layer */
    CBM_MIX1.u32 = pVdpReg->CBM_MIX1.u32;
    CBM_MIX1.bits.mixer_prio1 = 0; /* no layer */
    CBM_MIX1.bits.mixer_prio0 = 0; /* no layer */
    pVdpReg->CBM_MIX1.u32 = CBM_MIX1.u32;

    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_VP0, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_GP0, 1);

    /* initail mixer2, clear all layer */
    CBM_MIX2.u32 = pVdpReg->CBM_MIX2.u32;
    CBM_MIX2.bits.mixer_prio3 = 0; /* no layer */
    CBM_MIX2.bits.mixer_prio2 = 0; /* no layer */
    CBM_MIX2.bits.mixer_prio1 = 0; /* no layer */
    CBM_MIX2.bits.mixer_prio0 = 0; /* no layer */
    pVdpReg->CBM_MIX2.u32 = CBM_MIX2.u32;

    VDP_CBM_SetMixerPrio(VDP_CBM_MIX1, VDP_CBM_VID3, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX1, VDP_CBM_VID4, 1);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX1, VDP_CBM_GP1, 2);

	VOCTRL.u32 = pVdpReg->VOCTRL.u32;
	VOCTRL.bits.vo_ck_gt_en = 1;
	pVdpReg->VOCTRL.u32 = VOCTRL.u32;


    //VDP_INTF_DEFAULT();
    VDP_DHD_DEFAULT();
    VDP_DSD_DEFAULT();

    return;
}

#ifndef __DISP_PLATFORM_BOOT__

HI_VOID  VDAC_DRIVER_Initial()
{
    // set vdac reset value
    //g_pstRegCrg->PERI_CRG71.u32 = VDAC_SYSCTRL_RESET_VALUE;

    VDP_VDAC_Reset();

    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX_DAC.u32)), 0);

    DISP_WARN("=========VDAC_DRIVER_Initial====\n");

    return;
}
#endif

HI_U32 VDP_RegRead(HI_U32 a)
{
   //msleep(5);
   //HI_PRINT("\nread---addr = 0x%x,date = 0x%x",a,*a);
   return (*((HI_U32 *)a));
}

HI_VOID VDP_RegWrite(HI_U32 a, HI_U32 b)
{
    //msleep(5);
    //HI_PRINT("\nwrite---addr = 0x%x,date = 0x%x",a,b);
    *(HI_U32 *)a = b;       // ENV cfg
}

#ifndef __DISP_PLATFORM_BOOT__


//--------------------------------------------------------------------
// Video Layer
//--------------------------------------------------------------------
HI_VOID  VDP_VID_SetPreReadEnable    (HI_U32 u32Data, HI_U32 u32Enable )
{
    volatile U_V0_PRERD     V0_PRERD;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetPreReadEnable() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_PRERD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.pre_rd_en = u32Enable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);

    return ;
}
HI_VOID  VDP_VID_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    volatile U_V0_CTRL  V0_CTRL;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetNoSecFlag() Select Wrong Video Layer ID\n");
        return ;
    }
    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.nosec_flag= u32Enable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
    return ;
}
HI_VOID  VDP_VID_SetLayerEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    volatile U_V0_CTRL V0_CTRL;

//    U_V0_16REGIONENL V0_16REGIONENL;

    if(u32Data >= VID_MAX)
    {
        //HI_PRINT("Error,VDP_VID_SetLayerEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.surface_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}

HI_VOID  VDP_VID_Layer4LinkToMix2( HI_BOOL bEnable )
{
    U_CBM_ATTR CBM_ATTR;

     /*if VID4 is enable ,should link to Mix2,or link to Mix1*/
     CBM_ATTR.u32 = pVdpReg->CBM_ATTR.u32;
     CBM_ATTR.bits.sur_attr4 = bEnable?HI_TRUE:HI_FALSE;  /*true means v4 to mixer2*/
     pVdpReg->CBM_ATTR.u32 = CBM_ATTR.u32  ;

    return ;
}


HI_VOID  VDP_VID_SetDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    volatile     U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDcmpEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.dcmp_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}
HI_VOID  VDP_VID_SetUvorder(HI_U32 u32Data, HI_U32 u32bUvorder)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetUvorder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.uv_order= u32bUvorder;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}

HI_VOID  VDP_VID_SetDcmpOffset    (HI_U32 u32Data,HI_U32 u32laddr_offset,HI_U32  u32caddr_offset )
{
    volatile     U_V0_LADDROFFSET V0_LADDROFFSET;
    volatile     U_V0_CADDROFFSET V0_CADDROFFSET;


    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDcmpOffset() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_LADDROFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_LADDROFFSET.u32) + u32Data * VID_OFFSET));
    V0_LADDROFFSET.u32 = u32laddr_offset ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_LADDROFFSET.u32) + u32Data * VID_OFFSET), V0_LADDROFFSET.u32);

    V0_CADDROFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CADDROFFSET.u32) + u32Data * VID_OFFSET));
    V0_CADDROFFSET.u32 = u32caddr_offset ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CADDROFFSET.u32) + u32Data * VID_OFFSET), V0_CADDROFFSET.u32);


    return ;
}

HI_VOID  VDP_VID_SetDcmpErrorClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
    volatile U_VO_DCMPERRCLR  VO_DCMPERRCLR;


    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDcmpErrorClr() Select Wrong Video Layer ID\n");
        return ;
    }

    VO_DCMPERRCLR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    VO_DCMPERRCLR.bits.dcmp_l_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DCMPERRCLR.u32) + u32Data * VID_OFFSET), VO_DCMPERRCLR.u32);

    VO_DCMPERRCLR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    VO_DCMPERRCLR.bits.dcmp_c_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DCMPERRCLR.u32) + u32Data * VID_OFFSET), VO_DCMPERRCLR.u32);


    return ;
}




HI_VOID  VDP_VID_SetInDataFmt       (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInDataFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.ifmt = enDataFmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetReadMode    (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetReadMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.lm_rmode = enLRMode;
    V0_CTRL.bits.chm_rmode = enCRMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetIfirMode(HI_U32 u32Data, VDP_IFIRMODE_E enMode)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_SetIfirMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.ifir_mode = enMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetIfirCoef    (HI_U32 u32Data, HI_S32 * s32Coef)
{
    volatile U_V0_IFIRCOEF01 V0_IFIRCOEF01;
    volatile U_V0_IFIRCOEF23 V0_IFIRCOEF23;
    volatile U_V0_IFIRCOEF45 V0_IFIRCOEF45;
    volatile U_V0_IFIRCOEF67 V0_IFIRCOEF67;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetIfirCoef() Select Wrong Video Layer ID\n");
        return ;
    }

       V0_IFIRCOEF01.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IFIRCOEF01.u32) + u32Data * VID_OFFSET));
       V0_IFIRCOEF23.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IFIRCOEF23.u32) + u32Data * VID_OFFSET));
       V0_IFIRCOEF45.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IFIRCOEF45.u32) + u32Data * VID_OFFSET));
       V0_IFIRCOEF67.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IFIRCOEF67.u32) + u32Data * VID_OFFSET));

       V0_IFIRCOEF01.bits.coef0 = s32Coef[0];
       V0_IFIRCOEF01.bits.coef1 = s32Coef[1];
       V0_IFIRCOEF23.bits.coef2 = s32Coef[2];
       V0_IFIRCOEF23.bits.coef3 = s32Coef[3];
       V0_IFIRCOEF45.bits.coef4 = s32Coef[4];
       V0_IFIRCOEF45.bits.coef5 = s32Coef[5];
       V0_IFIRCOEF67.bits.coef6 = s32Coef[6];
       V0_IFIRCOEF67.bits.coef7 = s32Coef[7];

        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IFIRCOEF01.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF01.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IFIRCOEF23.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF23.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IFIRCOEF45.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF45.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IFIRCOEF67.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF67.u32);

    return ;
}
HI_VOID  VDP_VID_SetMuteEnable   (HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_LayerMuteEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetFlipEnable(HI_U32 u32Data, HI_U32 u32bEnable)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetFlipEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET));
    V0_CTRL.bits.flip_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET), V0_CTRL.u32);
}


//HI_VOID VDP_SetFieldOrder(HI_U32 u32Data, HI_U32 u32FieldOrder)
//{
//    volatile U_V0_CTRL V0_CTRL;
//
//    if(u32Data >= VID_MAX)
//    {
//        HI_PRINT("Error,VDP_SetFieldOrder() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
//    V0_CTRL.bits.bfield_first = u32FieldOrder;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
//
//    return ;
//}



HI_VOID  VDP_SetParaUpMode(HI_U32 u32Data,HI_U32 u32Mode)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_SetParaUpMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.vup_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}


HI_VOID VDP_SetTimeOut(HI_U32 u32Data, HI_U32 u32TData)
{
    volatile U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetTimeOut() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.time_out = u32TData;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}


//HI_VOID VDP_SetWbcOutMode(HI_U32 u32Data, VDP_DATA_RMODE_E enRdMode)
//{
//    volatile U_V0_CTRL V0_CTRL;
//
//    if(u32Data >= VID_MAX)
//    {
//        HI_PRINT("Error,VDP_SeWbcOutMode() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
//
//    if(enRdMode == VDP_RMODE_PROGRESSIVE)
//        V0_CTRL.bits.ofl_inter = 0;
//    else
//    {
//        V0_CTRL.bits.ofl_inter = 1;
//        if(enRdMode == VDP_RMODE_TOP)
//            V0_CTRL.bits.ofl_btm = 0;
//        else if (enRdMode == VDP_RMODE_BOTTOM)
//            V0_CTRL.bits.ofl_btm = 1;
//        else
//            HI_PRINT("Error! Vou_SetWbcOutIntf enRdMode error!\n");
//    }
//    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
//
//    return ;
//}
HI_VOID  VDP_VID_SetLayerAddr(HI_U32 u32Data, HI_U32 u32Chan, HI_U32 regionNo, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    volatile U_V0_P0STRIDE V0_P0STRIDE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Chan == 0)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET + regionNo  * 8), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET + regionNo  * 8), u32CAddr);
    }
    else if(u32Chan == 1)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_NADDR.u32) + u32Data * VID_OFFSET + regionNo  * 8), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_NCADDR.u32) + u32Data * VID_OFFSET + regionNo  * 8), u32CAddr);
    }
    else
    {
        HI_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Addr ID\n");
    }

    V0_P0STRIDE.bits.surface_stride = u32LStr;
    V0_P0STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET + regionNo  * 8), V0_P0STRIDE.u32);

    return ;
}
HI_VOID  VDP_VID_MultiSetRegionReso (HI_U32 u32Data, HI_U32 u32RegionNo,HI_RECT_S stRect)
{
    volatile U_V0_P0RESO          V0_P0RESO;
    volatile U_V0_P0VFPOS         V0_P0VFPOS;
    volatile U_V0_P0VLPOS         V0_P0VLPOS;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }
    V0_P0VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32RegionNo*8));
    V0_P0VFPOS.bits.video_xfpos = stRect.s32X;
    V0_P0VFPOS.bits.video_yfpos = stRect.s32Y;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32RegionNo*8), V0_P0VFPOS.u32);
    V0_P0VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32RegionNo*8));
    V0_P0VLPOS.bits.video_xlpos = stRect.s32X + stRect.s32Width - 1;
    V0_P0VLPOS.bits.video_ylpos = stRect.s32Y + stRect.s32Height - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32RegionNo*8), V0_P0VLPOS.u32);
    V0_P0RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32RegionNo*8));
    V0_P0RESO.bits.w = stRect.s32Width - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32RegionNo*8), V0_P0RESO.u32);
    return ;
}

HI_VOID  VDP_VID_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    volatile U_V0_VFPOS V0_VFPOS;
    volatile U_V0_VLPOS V0_VLPOS;
    volatile U_V0_DFPOS V0_DFPOS;
    volatile U_V0_DLPOS V0_DLPOS;
    volatile U_V0_IRESO V0_IRESO;
    volatile U_V0_ORESO V0_ORESO;
    //for multi region
    volatile U_V0_P0RESO          V0_P0RESO;
    volatile U_V0_P0VFPOS         V0_P0VFPOS;
    volatile U_V0_P0VLPOS         V0_P0VLPOS;
    //for crop
    volatile U_V0_CPOS          V0_CPOS;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }


    /*video position */
    V0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
    V0_VFPOS.bits.video_xfpos = stRect.u32VX;
    V0_VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET), V0_VFPOS.u32);

    V0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
    V0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    V0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET), V0_VLPOS.u32);

    V0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
    V0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    V0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET), V0_DFPOS.u32);

    V0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
    V0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
    V0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET), V0_DLPOS.u32);

    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32IWth - 1;
    V0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);

    V0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
    V0_ORESO.bits.ow = stRect.u32OWth - 1;
    V0_ORESO.bits.oh = stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);


    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = 0;
    V0_CPOS.bits.src_xlpos = stRect.u32IWth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);



    /*video position */
    V0_P0VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET));
    V0_P0VFPOS.bits.video_xfpos = stRect.u32VX;
    V0_P0VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET), V0_P0VFPOS.u32);

    V0_P0VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET));
    V0_P0VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    V0_P0VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET), V0_P0VLPOS.u32);

    V0_P0RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET));
    V0_P0RESO.bits.w = stRect.u32OWth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET), V0_P0RESO.u32);
   return ;
}

HI_VOID  VDP_VID_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_V0_IRESO V0_IRESO;
    //for crop
    volatile U_V0_CPOS          V0_CPOS;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32Wth - 1;
    V0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);


    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = 0;
    V0_CPOS.bits.src_xlpos = stRect.u32Wth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);

    //for multi region

    return ;
}

HI_VOID  VDP_VID_SetOutReso     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_V0_ORESO V0_ORESO;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
       return ;
   }

   //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);
   V0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
   V0_ORESO.bits.ow = stRect.u32Wth - 1;
   V0_ORESO.bits.oh = stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);

   return ;
}

HI_VOID  VDP_VID_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_V0_VFPOS V0_VFPOS;
   volatile U_V0_VLPOS V0_VLPOS;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetVideoPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   V0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
   V0_VFPOS.bits.video_xfpos = stRect.u32X;
   V0_VFPOS.bits.video_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET), V0_VFPOS.u32);

   V0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
   V0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
   V0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET), V0_VLPOS.u32);
   return ;
}

HI_VOID  VDP_VID_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_V0_DFPOS V0_DFPOS;
   volatile U_V0_DLPOS V0_DLPOS;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetDispPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   V0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
   V0_DFPOS.bits.disp_xfpos = stRect.u32X;
   V0_DFPOS.bits.disp_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET), V0_DFPOS.u32);

   V0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
   V0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
   V0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET), V0_DLPOS.u32);
   return ;
}

HI_VOID VDP_VID_SetMultiModeEnable(HI_U32 u32Data, HI_U32 u32Enable )
{
    volatile U_V0_MULTI_MODE      V0_MULTI_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetMultiModeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_MULTI_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_MULTI_MODE.u32) + u32Data * VID_OFFSET));
    V0_MULTI_MODE.bits.mrg_mode = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_MULTI_MODE.u32) + u32Data * VID_OFFSET), V0_MULTI_MODE.u32);

    return ;
}

HI_VOID VDP_VID_SetRegionEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable )
{
    volatile U_V0_16REGIONENL V0_16REGIONENL;

    if(u32Data >= VID_MAX)
    {
        //HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        //HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    V0_16REGIONENL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));
    if(u32bEnable)
    {
        V0_16REGIONENL.u32 = (1 << u32Num) | V0_16REGIONENL.u32;
    }
    else
    {
        V0_16REGIONENL.u32 = (~(1 << u32Num)) & V0_16REGIONENL.u32;
    }

    if (V0_16REGIONENL.u32)
    {
        if ( VDP_RM_LAYER_VID4 == u32Data )
            VDP_VID_Layer4LinkToMix2(HI_TRUE);

        VDP_VID_SetLayerEnable(u32Data, 1);
    }
    else
    {
        VDP_VID_SetLayerEnable(u32Data, 0);

        if ( VDP_RM_LAYER_VID4 == u32Data )
            VDP_VID_Layer4LinkToMix2(HI_TRUE);
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);

    return ;
}

HI_VOID VDP_VID_SetAllRegionDisable(HI_U32 u32Data )
{
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetAllRegionDisable() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), 0);

    return ;
}

HI_VOID VDP_VID_SetRegionAddr(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    volatile U_V0_P0STRIDE V0_P0STRIDE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetRegionAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), u32LAddr);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), u32CAddr);

    V0_P0STRIDE.bits.surface_stride = u32LStr;
    V0_P0STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0STRIDE.u32);

    return ;
}

HI_VOID VDP_VID_SetRegionReso(HI_U32 u32Data, HI_U32 u32Num, VDP_RECT_S  stRect)
{
    volatile U_V0_P0RESO          V0_P0RESO;
    volatile U_V0_P0VFPOS         V0_P0VFPOS;
    volatile U_V0_P0VLPOS         V0_P0VLPOS;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetRegionReso() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    /*video position */
    V0_P0VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0VFPOS.bits.video_xfpos = stRect.u32X;
    V0_P0VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VFPOS.u32);

    V0_P0VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    V0_P0VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VLPOS.u32);

    V0_P0RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0RESO.bits.w = stRect.u32Wth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0RESO.u32);

   return ;
}

#if 0
HI_VOID VDP_VID_SetRegionReso(HI_U32 u32Data, HI_U32 u32Num, VDP_DISP_RECT_S  stRect)
{
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetRegionReso() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    volatile U_V0_P0RESO          V0_P0RESO;
    volatile U_V0_P0VFPOS         V0_P0VFPOS;
    volatile U_V0_P0VLPOS         V0_P0VLPOS;
    volatile U_V0_IRESO           V0_IRESO;
    volatile U_V0_ORESO           V0_ORESO;

    /*video position */
    //V0_P0VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    //V0_P0VFPOS.bits.video_xfpos = stRect.u32X;
    //V0_P0VFPOS.bits.video_yfpos = stRect.u32Y;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VFPOS.u32);

    //V0_P0VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    //V0_P0VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    //V0_P0VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VLPOS.u32);

    //V0_P0RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    //V0_P0RESO.bits.w = stRect.u32Wth - 1;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0RESO.u32);

    V0_P0VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET));
    V0_P0VFPOS.bits.video_xfpos = stRect.u32VX;
    V0_P0VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET), V0_P0VFPOS.u32);

    V0_P0VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET));
    V0_P0VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    V0_P0VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET), V0_P0VLPOS.u32);

    V0_P0RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0RESO.bits.w = stRect.u32OWth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0RESO.u32);

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);
    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32IWth - 1;
    V0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);
    V0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
    V0_ORESO.bits.ow = stRect.u32OWth - 1;
    V0_ORESO.bits.oh = stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);
   return ;
}
#endif

HI_VOID VDP_VID_SetRegionMuteEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 bEnable)
{
    volatile U_V0_16MUTE V0_16MUTE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetRegionMuteEnable() Select Wrong VID Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    if ((u32Data == VDP_LAYER_VID0)
        || (u32Data == VDP_LAYER_VID3))
        VDP_VID_SetMuteEnable(u32Data, bEnable);
    V0_16MUTE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_16MUTE.u32) + u32Data * VID_OFFSET));
    if(bEnable)
    {
        V0_16MUTE.u32 = (1 << u32Num) | V0_16MUTE.u32;
    }
    else
    {
        V0_16MUTE.u32 = (~(1 << u32Num)) & V0_16MUTE.u32;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_16MUTE.u32) + u32Data * VID_OFFSET), V0_16MUTE.u32);

    return ;
}

HI_VOID  VDP_VID_SetRegUp       (HI_U32 u32Data)
{
    volatile U_V0_UPD V0_UPD;

    /* VHD layer register update */
    if(u32Data >= VID_MAX)
    {
        //HI_PRINT("Error,VDP_VID_SetRegup() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_UPD.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_UPD.u32) + u32Data * VID_OFFSET), V0_UPD.u32);

    return ;
}



HI_VOID  VDP_VID_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    volatile U_V0_CBMPARA V0_CBMPARA;
    volatile U_V0_ALPHA V0_ALPHA;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerGalpha() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CBMPARA.u32) + u32Data * VID_OFFSET));
    V0_CBMPARA.bits.galpha = u32Alpha0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CBMPARA.u32) + u32Data * VID_OFFSET), V0_CBMPARA.u32);

    V0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET));
    V0_ALPHA.bits.vbk_alpha = u32Alpha0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET), V0_ALPHA.u32);
    return ;
}

HI_VOID  VDP_VID_SetCropReso    (HI_U32 u32Data, VDP_RECT_S stRect)
{
    volatile  U_V0_CPOS V0_CPOS;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCropReso() Select Wrong Video Layer ID\n");
        return ;
    }

    /* crop position */
    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = stRect.u32X;
    V0_CPOS.bits.src_xlpos = stRect.u32X+stRect.u32Wth-1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);

    return ;
}

HI_VOID  VDP_VID_SetLayerBkg    (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    volatile U_V0_BK    V0_BK;
    volatile U_V0_ALPHA V0_ALPHA;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerBkg() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_BK.u32) + u32Data * VID_OFFSET));
    V0_BK.bits.vbk_y  = stBkg.u32BkgY;
    V0_BK.bits.vbk_cb = stBkg.u32BkgU;
    V0_BK.bits.vbk_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_BK.u32) + u32Data * VID_OFFSET), V0_BK.u32);

    V0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET));
    V0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET), V0_ALPHA.u32);

    return ;
}

HI_VOID  VDP_VP_GetCscReg(HI_U32 u32Data, HI_U32 *pdata, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_GetCscCoef() Select Wrong Video Layer ID\n");
        return ;
    }
    if(u32Num == 0)
    {
        pdata[0] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        pdata[1] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET));
        pdata[2] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET));
        pdata[3] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P0.u32) + u32Data * VP_OFFSET));
        pdata[4] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P1.u32) + u32Data * VP_OFFSET));
        pdata[5] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P2.u32) + u32Data * VP_OFFSET));
        pdata[6] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P3.u32) + u32Data * VP_OFFSET));
        pdata[7] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P4.u32) + u32Data * VP_OFFSET));
    }
    else if(u32Num == 1)
    {
        pdata[0] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        pdata[1] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET));
        pdata[2] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET));
        pdata[3] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P0.u32) + u32Data * VP_OFFSET));
        pdata[4] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P1.u32) + u32Data * VP_OFFSET));
        pdata[5] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P2.u32) + u32Data * VP_OFFSET));
        pdata[6] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P3.u32) + u32Data * VP_OFFSET));
        pdata[7] = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P4.u32) + u32Data * VP_OFFSET));
    }
    else
    {
    }

    return ;
}

HI_VOID  VDP_VID_GetCscReg(HI_U32 u32Data, HI_U32 *pdata)
{
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }
    if (u32Data == 0) {
        VDP_VP_GetCscReg(0, pdata, 0);
    }
    else if (u32Data == 1) {
        VDP_VP_GetCscReg(0, pdata, 1);
    }
    else {
        pdata[0] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
        pdata[1] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET));
        pdata[2] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET));
        pdata[3] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P0.u32) + u32Data * VID_OFFSET));
        pdata[4] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P1.u32) + u32Data * VID_OFFSET));
        pdata[5] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P2.u32) + u32Data * VID_OFFSET));
        pdata[6] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P3.u32) + u32Data * VID_OFFSET));
        pdata[7] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P4.u32) + u32Data * VID_OFFSET));
    }

    return ;
}
HI_VOID  VDP_VP_SetCscReg(HI_U32 u32Data, HI_U32 *pdata, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_GetCscCoef() Select Wrong Video Layer ID\n");
        return ;
    }
    if(u32Num == 0)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET),  pdata[0]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET),  pdata[1]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET),  pdata[2]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P0.u32) + u32Data * VP_OFFSET),  pdata[3]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P1.u32) + u32Data * VP_OFFSET),  pdata[4]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P2.u32) + u32Data * VP_OFFSET),  pdata[5]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P3.u32) + u32Data * VP_OFFSET),  pdata[6]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P4.u32) + u32Data * VP_OFFSET),  pdata[7]);
    }
    else if(u32Num == 1)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), pdata[0]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET), pdata[1]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET), pdata[2]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P0.u32) + u32Data * VP_OFFSET), pdata[3]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P1.u32) + u32Data * VP_OFFSET), pdata[4]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P2.u32) + u32Data * VP_OFFSET), pdata[5]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P3.u32) + u32Data * VP_OFFSET), pdata[6]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P4.u32) + u32Data * VP_OFFSET), pdata[7]);
    }
    else
    {
    }
    return ;
}
HI_VOID  VDP_VID_SetCscReg(HI_U32 u32Data, HI_U32 *pdata)
{
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }
    if (u32Data == 0) {
        VDP_VP_SetCscReg(0, pdata, 0);
    }
    else if (u32Data == 1) {
        VDP_VP_SetCscReg(0, pdata, 1);
    }
    else {
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), pdata[0]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET), pdata[1]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET), pdata[2]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P0.u32) + u32Data * VID_OFFSET), pdata[3]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P1.u32) + u32Data * VID_OFFSET), pdata[4]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P2.u32) + u32Data * VID_OFFSET), pdata[5]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P3.u32) + u32Data * VID_OFFSET), pdata[6]);
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P4.u32) + u32Data * VID_OFFSET), pdata[7]);
    }
    return ;
}

HI_VOID  VDP_VID_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    volatile U_V0_CSC_IDC  V0_CSC_IDC;
    volatile U_V0_CSC_ODC  V0_CSC_ODC;
    volatile U_V0_CSC_IODC V0_CSC_IODC;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if (u32Data == 0) {
        VDP_VP_SetCscDcCoef(0, pstCscCoef, 0);
    }
    else if (u32Data == 1) {
        VDP_VP_SetCscDcCoef(0, pstCscCoef, 1);
    }
    else {

        V0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
        V0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
        V0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), V0_CSC_IDC.u32);

        V0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET));
        V0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
        V0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET), V0_CSC_ODC.u32);

        V0_CSC_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET));
        V0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
        V0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET), V0_CSC_IODC.u32);
    }

    return ;
}

HI_VOID   VDP_VID_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{
    volatile U_V0_CSC_P0 V0_CSC_P0;
    volatile U_V0_CSC_P1 V0_CSC_P1;
    volatile U_V0_CSC_P2 V0_CSC_P2;
    volatile U_V0_CSC_P3 V0_CSC_P3;
    volatile U_V0_CSC_P4 V0_CSC_P4;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscCoef Select Wrong video ID\n");
        return ;
    }


    if (u32Data == 0) {
        VDP_VP_SetCscCoef(0, stCscCoef, 0);
    }
    else if (u32Data == 1) {
        VDP_VP_SetCscCoef(0, stCscCoef, 1);
    }
    else {
    V0_CSC_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P0.u32)+u32Data*VID_OFFSET));
    V0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    V0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P0.u32)+u32Data*VID_OFFSET), V0_CSC_P0.u32);

    V0_CSC_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P1.u32)+u32Data*VID_OFFSET));
    V0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    V0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P1.u32)+u32Data*VID_OFFSET), V0_CSC_P1.u32);

    V0_CSC_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P2.u32)+u32Data*VID_OFFSET));
    V0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    V0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P2.u32)+u32Data*VID_OFFSET), V0_CSC_P2.u32);

    V0_CSC_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P3.u32)+u32Data*VID_OFFSET));
    V0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    V0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P3.u32)+u32Data*VID_OFFSET), V0_CSC_P3.u32);

    V0_CSC_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P4.u32)+u32Data*VID_OFFSET));
    V0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P4.u32)+u32Data*VID_OFFSET), V0_CSC_P4.u32);
    }

}




HI_VOID  VDP_VID_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    volatile U_V0_CSC_IDC V0_CSC_IDC;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if (u32Data == 0) {
       VDP_VP_SetCscEnable(0, u32bCscEn, 0);
    }
    else if (u32Data == 1) {
       VDP_VP_SetCscEnable(0, u32bCscEn, 1);
    }
    else {
        V0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
        V0_CSC_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), V0_CSC_IDC.u32);
    }

    return ;
}

HI_VOID VDP_VID_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)

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

    VDP_VID_SetCscCoef  (u32Data,st_csc_coef);
    VDP_VID_SetCscDcCoef(u32Data,st_csc_dc_coef);

    return ;
}

HI_VOID VDP_VID_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    //volatile U_V0_CTRL V0_CTRL;
    //
    //if(u32Data >= VID_MAX)
    //{
    //    HI_PRINT("Error,VDP_VID_SetDispMode() Select Wrong Video Layer ID\n");
    //    return ;
    //}

    //
    //V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    //V0_CTRL.bits.disp_mode = enDispMode;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_V0_DRAWMODE V0_DRAWMODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDrawMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DRAWMODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET));
    V0_DRAWMODE.bits.draw_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET), V0_DRAWMODE.u32);

    return ;
}

HI_VOID  VDP_VID_SetZmeEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    volatile U_V0_HSP V0_HSP;
    volatile U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hlmsc_en = u32bEnable;
        V0_HSP.bits.hlfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hchmsc_en = u32bEnable;
        V0_HSP.bits.hchfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
    }
#if 0
    if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
    {
        VHDHSP.u32 = VDP_RegRead(&(pVdpReg->VHDHSP.u32));
//        VHDHSP.bits.non_lnr_en = bEnable;
        VHDHSP.bits.non_lnr_en = 0;
        VDP_RegWrite(&(pVdpReg->VHDHSP.u32), VHDHSP.u32);
    }
#endif
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vlmsc_en = u32bEnable;
        V0_VSP.bits.vlfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vchmsc_en = u32bEnable;
        V0_VSP.bits.vchfir_en = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
    }

    return ;
}



/* Vou set zoom inital phase */

HI_VOID  VDP_VID_SetZmePhase    (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    volatile U_V0_HLOFFSET  V0_HLOFFSET;
    volatile U_V0_HCOFFSET  V0_HCOFFSET;
    volatile U_V0_VOFFSET   V0_VOFFSET;
    volatile U_V0_VBOFFSET  V0_VBOFFSET;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HLOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET));
        V0_HLOFFSET.bits.hor_loffset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET), V0_HLOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HCOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET));
        V0_HCOFFSET.bits.hor_coffset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET), V0_HCOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VOFFSET.bits.vluma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VOFFSET.bits.vchroma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VBOFFSET.bits.vbluma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VBOFFSET.bits.vbchroma_offset = s32Phase;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32);
    }

    return ;
}


HI_VOID  VDP_VID_SetZmeFirEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_V0_HSP V0_HSP;
    volatile U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
         V0_HSP.bits.hlfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
     }

     if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
     {
         V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
         V0_HSP.bits.hchfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
     }

     if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
         V0_VSP.bits.vlfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
     }

     if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
     {
         V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
         V0_VSP.bits.vchfir_en = u32bEnable;
         VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
     }

    return ;
}



HI_VOID  VDP_VID_SetZmeMidEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    volatile U_V0_HSP V0_HSP;
    volatile U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);
    }

    return ;
}

#if 0
HI_VOID  VDP_VID_SetZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_V0_VSR V0_VSR;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET));
    V0_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET), V0_VSR.u32);

    return ;
}
#endif
HI_VOID  VDP_VID_SetZmeInFmt_Define1(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    volatile U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeInFmt_Define1() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.zme_in_fmt = u32Fmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}
#if 0
HI_VOID  VDP_VID_SetZmeOutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    volatile U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeInFmt_Define1() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}
#endif
#if 0
// Vou set coef or lut read update
HI_VOID  VDP_VID_SetParaUpd       (HI_U32 u32Data, VDP_VID_PARA_E enMode)
{
    volatile U_V0_PARAUP V0_PARAUP;
    V0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET));

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("error,VDP_VID_SetParaUpd() select wrong video layer id\n");
        return ;
    }
    if(enMode == VDP_VID_PARA_ZME_HOR)
    {

        V0_PARAUP.bits.v0_hlcoef_upd= 0x1;
        V0_PARAUP.bits.v0_hccoef_upd= 0x1;

    }
    else if(enMode == VDP_VID_PARA_ZME_VER)
    {
        V0_PARAUP.bits.v0_vlcoef_upd= 0x1;
        V0_PARAUP.bits.v0_vccoef_upd= 0x1;
    }
    else
    {
        HI_PRINT("error,VDP_VID_SetParaUpd() select wrong mode!\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET), V0_PARAUP.u32);
    return ;
}
#endif
#if 0
HI_VOID VDP_VID_SetZmeHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    volatile U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_HSP.bits.hfir_order = u32HfirOrder;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return ;
}
#endif
#if 0
HI_VOID VDP_VID_SetZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr, HI_U32 u32AddrChr)
{


    volatile U_V0_HLCOEFAD V0_HLCOEFAD;
    volatile U_V0_HCCOEFAD V0_HCCOEFAD;
    volatile U_V0_VLCOEFAD V0_VLCOEFAD;
    volatile U_V0_VCCOEFAD V0_VCCOEFAD;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_VID_PARA_ZME_HOR)
    {
        V0_HLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET), V0_HLCOEFAD.u32);

        V0_HCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HCCOEFAD.bits.coef_addr = u32AddrChr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET), V0_HCCOEFAD.u32);

    }
    else if(u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET), V0_VLCOEFAD.u32);

        V0_VCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VCCOEFAD.bits.coef_addr = u32AddrChr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET), V0_VCCOEFAD.u32);


    }
    else
    {
        HI_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}
#endif
#if 0
HI_VOID VDP_VID_SetZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio)

{
    volatile U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    return ;
}
#endif
//-------------------------------------------------------------------
//VP_BEGINING
//-------------------------------------------------------------------
HI_VOID  VDP_VP_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_VP0_DOF_CTRL  VP0_DOF_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetThreeDimDofEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_DOF_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DOF_CTRL.u32)));
    VP0_DOF_CTRL.bits.dof_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DOF_CTRL.u32)), VP0_DOF_CTRL.u32);
}

HI_VOID  VDP_VP_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
    volatile U_VP0_DOF_STEP  VP0_DOF_STEP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetThreeDimDofStep() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_DOF_STEP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DOF_STEP.u32)));
    VP0_DOF_STEP.bits.right_step= s32RStep;
    VP0_DOF_STEP.bits.left_step = s32LStep;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DOF_STEP.u32)), VP0_DOF_STEP.u32);
}






HI_VOID  VDP_VP_GetLayerReso (HI_U32 u32Data, HI_RECT_S *stRect)
{
    volatile U_VP0_IRESO VP0_IRESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }
    VP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    stRect->s32Width  = VP0_IRESO.bits.iw + 1;
    stRect->s32Height = VP0_IRESO.bits.ih + 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32);
   return ;
}
HI_VOID  VDP_VP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_VP0_VFPOS VP0_VFPOS;
   volatile U_VP0_VLPOS VP0_VLPOS;

   if(u32Data >= VP_MAX)
   {
       HI_PRINT("Error,VDP_VP_SetVideoPos() Select Wrong Video Layer ID\n");
       return ;
   }



   /*video position */
   VP0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET));
   VP0_VFPOS.bits.video_xfpos = stRect.u32X;
   VP0_VFPOS.bits.video_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET), VP0_VFPOS.u32);

   VP0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET));
   VP0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
   VP0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET), VP0_VLPOS.u32);
   return ;
}

HI_VOID  VDP_VP_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_VP0_DFPOS VP0_DFPOS;
   volatile U_VP0_DLPOS VP0_DLPOS;

   if(u32Data >= VP_MAX)
   {
       HI_PRINT("Error,VDP_VP_SetDispPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   VP0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DFPOS.u32) + u32Data * VP_OFFSET));
   VP0_DFPOS.bits.disp_xfpos = stRect.u32X;
   VP0_DFPOS.bits.disp_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DFPOS.u32) + u32Data * VP_OFFSET), VP0_DFPOS.u32);

   VP0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DLPOS.u32) + u32Data * VP_OFFSET));
   VP0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
   VP0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DLPOS.u32) + u32Data * VP_OFFSET), VP0_DLPOS.u32);
   return ;
}

HI_VOID  VDP_VP_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_VP0_IRESO VP0_IRESO;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VP_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32Wth - 1;
    VP0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VP_OFFSET), VP0_IRESO.u32);

    return ;
}


HI_VOID  VDP_VP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    volatile U_VP0_CTRL VP0_CTRL;

    //special for bk alpha = video alpha
    volatile U_VP0_ALPHA     VP0_ALPHA;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetLayerGalpha() Select Wrong vp Layer ID\n");
        return ;
    }


    VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.vp_galpha = u32Alpha;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32);


    VP0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ALPHA.u32)));
    VP0_ALPHA.bits.vbk_alpha = u32Alpha;
    VDP_RegWrite((HI_U32)&(pVdpReg->VP0_ALPHA.u32), VP0_ALPHA.u32);

    return ;
}

HI_VOID  VDP_VP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    volatile U_VP0_BK VP0_BK;

    if(u32Data == VDP_LAYER_VP0)
    {
        VP0_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_BK.u32)));
        VP0_BK.bits.vbk_y  = stBkg.u32BkgY;
        VP0_BK.bits.vbk_cb = stBkg.u32BkgU;
        VP0_BK.bits.vbk_cr = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->VP0_BK.u32), VP0_BK.u32);

//        VP0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ALPHA.u32)));
//        VP0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
//        VDP_RegWrite((HI_U32)&(pVdpReg->VP0_ALPHA.u32), VP0_ALPHA.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_VP_SetLayerBkg() Select Wrong VP Layer ID\n");
    }

    return ;
}

HI_VOID VDP_VP_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    //volatile U_VP0_CTRL VP0_CTRL;
    //
    //if(u32Data >= VP_MAX)
    //{
    //    HI_PRINT("Error,VDP_VP_SetDispMode() Select Wrong Video Layer ID\n");
    //    return ;
    //}

    //
    //VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    //VP0_CTRL.bits.disp_mode = enDispMode;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VID_OFFSET), VP0_CTRL.u32);

    //return ;
}



HI_VOID VDP_VP_SetMuteEnable     (HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_VP0_CTRL VP0_CTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetMuteEnable() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VP0_SetParaUpd       (HI_U32 u32Data, VDP_DISP_COEFMODE_E  enMode)
{
    volatile U_VP0_PARAUP  VP0_PARAUP;
    VP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET));

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("error,VDP_VP0_SetParaUpd() select wrong video layer id\n");
        return ;
    }

    if(enMode == VDP_DISP_COEFMODE_ACC)
    {
        VP0_PARAUP.bits.vp0_acccoef_upd= 0x1;
    }

    if(enMode == VDP_DISP_COEFMODE_ACM)
    {
        VP0_PARAUP.bits.vp0_acmcoef_upd= 0x1;
    }


    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET), VP0_PARAUP.u32);
    return ;
}


//-------------------------------------------------------------------
//VP_END
//-------------------------------------------------------------------

///HI_VOID VDP_SetLnkWbEnable    (HI_U32 u32Data ,HI_U32 u32Enable)
///{
///    volatile U_V0_CTRL V0_CTRL;
///
///    if(u32Data >= VID_MAX)
///    {
///        HI_PRINT("Error,VDP_SetLnkWbEnable() Select Wrong Video Layer ID\n");
///        return ;
///    }
///
///    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
///    V0_CTRL.bits.v0_sta_wr_en = u32Enable;
///    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
///
///    return;
///}

//HI_VOID VDP_SetWbcMd    (HI_U32 u32Data,HI_U32 enMdSel)
//{
//    volatile U_V0_CTRL V0_CTRL;
//
//    if(u32Data >= VID_MAX)
//    {
//        HI_PRINT("Error,VDP_VID_SetWbcaMd() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
//    V0_CTRL.bits.ofl_mode = enMdSel;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
//
//    return ;
//}

//HI_VOID VDP_SetInDataWidth    (HI_U32 u32Data, HI_U32 u32idatawth)
//{
//    volatile U_V0_CTRL V0_CTRL;
//
//    if(u32Data >= VID_MAX)
//    {
//        HI_PRINT("Error,VDP_SetInDataWidth() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
//    V0_CTRL.bits.data_width = u32idatawth;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
//
//    return ;
//}

HI_VOID VDP_SetOutMode   (HI_U32 u32Data, HI_U32 u32outmode)
{
    //volatile U_V0_CTRL V0_CTRL;
    //
    //if(u32Data >= VID_MAX)
    //{
    //    HI_PRINT("Error,VDP_SetInDataWidth() Select Wrong Video Layer ID\n");
    //    return ;
    //}

    //V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    //V0_CTRL.bits.disp_mode = u32outmode;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
    //
    //return ;
}





//--------------------------------------------------------------------
// AXI BUS BEING
//--------------------------------------------------------------------

HI_VOID VDP_SetClkGateEn(HI_U32 u32Data)
{
    volatile U_VOCTRL VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

///HI_VOID VDP_SetWrOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
///{
///    volatile U_VOAXICTRL VOAXICTRL;
///
///    if(u32Data == VDP_MASTER0)
///    {
///        if(u32BusId == 0)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m0_wr_ostd = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///        else
///        {
///            HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
///        }
///    }
///    else if(u32Data == VDP_MASTER1)
///    {
///        if(u32BusId == 0)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m1_wr_ostd = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///    return ;
///}
#endif

HI_VOID  VDP_VP_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    volatile U_VP0_VFPOS VP0_VFPOS;
    volatile U_VP0_VLPOS VP0_VLPOS;
    volatile U_VP0_DFPOS VP0_DFPOS;
    volatile U_VP0_DLPOS VP0_DLPOS;
    volatile U_VP0_IRESO VP0_IRESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }



    /*video position */
    VP0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_VFPOS.u32) + u32Data * VID_OFFSET));
    VP0_VFPOS.bits.video_xfpos = stRect.u32VX;
    VP0_VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_VFPOS.u32) + u32Data * VID_OFFSET), VP0_VFPOS.u32);

    VP0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_VLPOS.u32) + u32Data * VID_OFFSET));
    VP0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    VP0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_VLPOS.u32) + u32Data * VID_OFFSET), VP0_VLPOS.u32);

    VP0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DFPOS.u32) + u32Data * VID_OFFSET));
    VP0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    VP0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DFPOS.u32) + u32Data * VID_OFFSET), VP0_DFPOS.u32);

    VP0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DLPOS.u32) + u32Data * VID_OFFSET));
    VP0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
    VP0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DLPOS.u32) + u32Data * VID_OFFSET), VP0_DLPOS.u32);

    //VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32);
    VP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32IWth - 1;
    VP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32);

   return ;
}

HI_VOID  VDP_VP_SetRegUp  (HI_U32 u32Data)
{
    volatile U_VP0_UPD VP0_UPD;

    /* VP layer register update */
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetRegUp() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_UPD.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_UPD.u32) + u32Data * VP_OFFSET), VP0_UPD.u32);

    return ;
}

///HI_VOID VDP_SetRdOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
///{
///    volatile U_VOAXICTRL VOAXICTRL;
///
///    if(u32Data == VDP_MASTER0)
///    {
///        if(u32BusId == 0)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m0_outstd_rid0 = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///        else if(u32BusId == 1)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m0_outstd_rid1 = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///        else
///        {
///            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
///        }
///    }
///    else if(u32Data == VDP_MASTER1)
///    {
///        if(u32BusId == 0)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m1_outstd_rid0 = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///        else if(u32BusId == 1)
///        {
///            VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
///            VOAXICTRL.bits.m1_outstd_rid1 = u32OutStd;
///            VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
///        }
///        else
///        {
///            HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
///        }
///    }
///    else
///    {
///        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
///    }
///
///    return ;
///}

//HI_VOID VDP_SetArbMode(HI_U32 u32Data, HI_U32 u32bMode)
//{
//    volatile U_VOCTRL VOCTRL;
//
//    if(u32Data == VDP_MASTER0)
//    {
//        //set vdp arbitration mode
//        //0:Poll Mode,1:GFX First Mode
//        VOCTRL.u32 = VDP_RegRead(&(pVdpReg->VOCTRL.u32));
//        VOCTRL.bits.m0_arb_mode = u32bMode;
//        VDP_RegWrite(&(pVdpReg->VOCTRL.u32), VOCTRL.u32);
//    }
//    else if(u32Data == VDP_MASTER1)
//    {
//        //set vdp arbitration mode
//        //0:Poll Mode,1:GFX First Mode
//        VOCTRL.u32 = VDP_RegRead(&(pVdpReg->VOCTRL.u32));
//        VOCTRL.bits.m1_arb_mode = u32bMode;
//        VDP_RegWrite(&(pVdpReg->VOCTRL.u32), VOCTRL.u32);
//    }
//    else
//    {
//        HI_PRINT("Error,VDP_SetArbMode() Select Wrong Master!\n");
//    }
//
//    return ;
//}
//
//HI_VOID VDP_SetBusRdId(HI_U32 u32Data, HI_U32 u32bMode)
//{
//    volatile U_VOAXICTRL VOAXICTRL;
//
//    if(u32Data == VDP_MASTER0)
//    {
//        //set vdp read bus ID
//        //0:V0 ID0, 1:V0 ID1
//        VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
//        VOAXICTRL.bits.m0_id_sel = u32bMode;
//        VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
//    }
//    else if(u32Data == VDP_MASTER1)
//    {
//        //set vdp read bus ID
//        //0:V0 ID0, 1:V0 ID1
//        VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
//        VOAXICTRL.bits.m1_id_sel = u32bMode;
//        VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
//    }
//    else
//    {
//        HI_PRINT("Error,VDP_SetBusRdId() Select Wrong Master!\n");
//    }
//
//    return ;
//}
//
////--------------------------------------------------------------------
// AXI BUS END
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// DHD Channel
//--------------------------------------------------------------------
//HI_VOID VDP_DISP_SetFramePackingEn  (HI_U32 u32hd_id, HI_U32 u32Enable)
//{
//    volatile U_DHD0_CTRL DHD0_CTRL;
//    if(u32hd_id >= CHN_MAX)
//    {
//        HI_PRINT("Error,VDP_DISP_SetFramePackingEn Select Wrong CHANNEL ID\n");
//        return ;
//    }
//
//    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
//    DHD0_CTRL.bits.fp_en = u32Enable;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
//}

HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;

/*
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_330          : 2   ; // [2..1]
        unsigned int    fp_en                 : 1   ; // [3]
        unsigned int    iop                   : 1   ; // [4]
        unsigned int    Reserved_329          : 7   ; // [11..5]
        unsigned int    gmm_mode              : 1   ; // [12]
        unsigned int    gmm_en                : 1   ; // [13]
        unsigned int    hdmi_mode             : 1   ; // [14]
        unsigned int    Reserved_328          : 5   ; // [19..15]
        unsigned int    fpga_lmt_width        : 7   ; // [26..20]
        unsigned int    fpga_lmt_en           : 1   ; // [27]
        unsigned int    Reserved_327          : 1   ; // [28]
        unsigned int    cbar_sel              : 1   ; // [29]
        unsigned int    cbar_en               : 1   ; // [30]
        unsigned int    intf_en               : 1   ; // [31]
    } bits;
*/
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.intf_en = 0;
    DHD0_CTRL.bits.cbar_en = 0;
    DHD0_CTRL.bits.fpga_lmt_en = 0;
    DHD0_CTRL.bits.hdmi_mode = DHD_YUV_TO_HDMI;
    DHD0_CTRL.bits.gmm_en = 0;
    DHD0_CTRL.bits.disp_mode = 0;
    DHD0_CTRL.bits.regup = 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

#ifndef __DISP_PLATFORM_BOOT__

HI_VOID VDP_DISP_SetFramePackingEn  (HI_U32 u32hd_id, HI_U32 u32Enable)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetFramePackingEn Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));

    if(u32Enable == 1)
    {
        /*set it to 3d framepacking.*/
        DHD0_CTRL.bits.disp_mode = 5;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
    } else {
        /*set it to 2d */
        DHD0_CTRL.bits.disp_mode = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
    }
}
#endif

HI_VOID VDP_DISP_SetProToInterEnable(HI_U32 u32hd_id, HI_U32 u32Enable)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetProToInterEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.p2i_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}


HI_VOID VDP_DISP_SetDispMode  (HI_U32 u32hd_id, HI_U32 u32DispMode)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
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
    volatile U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetHdmiMode Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.hdmi_mode = u32hdmi_md;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetHdmiClk  (HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    if ( u32hd_id)
    {
        /*SD*/

        /*chan select :      0:sd ,1: hd*/
        PERI_CRG54Tmp.bits.hdmi_clk_sel = 0;

        /* clk div select :      0 : 1:1      1:  1:2*/
        PERI_CRG54Tmp.bits.vo_sd_hdmi_clk_sel = u32hdmi_clkdiv;
    }
    else
    {
        /*chan select :      0:sd ,1: hd*/
        PERI_CRG54Tmp.bits.hdmi_clk_sel = 1;

        /* clk div select :      0 : 1:1      1:  1:2*/
        PERI_CRG54Tmp.bits.vo_hd_hdmi_clk_sel = u32hdmi_clkdiv;
    }
    g_pstRegCrg->PERI_CRG54.u32= PERI_CRG54Tmp.u32;
}
HI_VOID VDP_DISP_SetRegUp (HI_U32 u32hd_id)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
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
    volatile U_DHD0_CTRL DHD0_CTRL;
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
        HI_PRINT("Error,VDP_DISP_SetIntfEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    *pbTrue = DHD0_CTRL.bits.intf_en;
}
#ifndef __DISP_PLATFORM_BOOT__
HI_VOID  VDP_DISP_OpenIntf(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S stSyncInfo)

{
     volatile U_DHD0_CTRL DHD0_CTRL;
     volatile U_DHD0_VSYNC DHD0_VSYNC;
     volatile U_DHD0_VPLUS DHD0_VPLUS;
     volatile U_DHD0_PWR DHD0_PWR;
     volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
     volatile U_DHD0_HSYNC2 DHD0_HSYNC2;


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

HI_VOID VDP_DISP_SetCbarEnable(HI_U32 u32hd_id,HI_U32 bTrue)
{
    volatile U_DHD0_CTRL DHD0_CTRL;

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
    volatile U_DHD0_CTRL DHD0_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_SetCbarSel Select Wrong CHANNEL ID\n");
        return ;
    }


    VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.cbar_sel = u32_cbar_sel;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

#endif

HI_VOID VDP_DISP_SetVtThd(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32vtthd)
{

    volatile U_DHD0_VTTHD DHD0_VTTHD;
    volatile U_DHD0_VTTHD3 DHD0_VTTHD3;
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

HI_VOID VDP_DISP_SetIntMask  (HI_U32 u32masktypeen)
{
    volatile U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 | u32masktypeen;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOINTMSK.u32)), VOINTMSK.u32);

    return ;
}

HI_VOID VDP_DISP_GetIntMask (HI_U32 *pu32masktype)
{
    /* Dispaly interrupt mask enable */
    *pu32masktype = VDP_RegRead((HI_U32)&(pVdpReg->VOINTMSK.u32));

    return ;
}

#ifndef __DISP_PLATFORM_BOOT__

// VDP_DISP_OpenIntf FOR INTF TEST
HI_VOID  VDP_DISP_OpenTypIntf(HI_U32 u32hd_id, VDP_DISP_DIGFMT_E enDigFmt)
{
    VDP_DISP_SYNCINFO_S  st1SyncInfo;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_OpenIntf Select Wrong CHANNEL ID\n");
        return ;
    }

    memset(&st1SyncInfo, 0, sizeof(VDP_DISP_SYNCINFO_S));

    switch(enDigFmt)
    {
        case VDP_DISP_DIGFMT_PAL:
        {

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 0;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 0x120;
            st1SyncInfo.u32Vfb  = 0x2;
            st1SyncInfo.u32Vbb  = 0x16;

            st1SyncInfo.u32Hact = 0x2d0;
            st1SyncInfo.u32Hfb  = 0xc;
            st1SyncInfo.u32Hbb  = 0x84;

            st1SyncInfo.u32Bvact = 0x120;
            st1SyncInfo.u32Bvfb = 0x02;
            st1SyncInfo.u32Bvbb = 0x17;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_SINGL_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_NTSC:
        {

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 0;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 0xf0;
            st1SyncInfo.u32Vfb  = 0x2;
            st1SyncInfo.u32Vbb  = 0x13;

            st1SyncInfo.u32Hact = 0x2d0;
            st1SyncInfo.u32Hfb  = 0x10;
            st1SyncInfo.u32Hbb  = 0x7a;

            st1SyncInfo.u32Bvact = 0xf0;
            st1SyncInfo.u32Bvfb = 0x02;
            st1SyncInfo.u32Bvbb = 0x14;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_SINGL_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1080P:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M

            st1SyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 1080;
            st1SyncInfo.u32Vfb  = 4;
            st1SyncInfo.u32Vbb  = 41;

            st1SyncInfo.u32Hact = 1920;
            st1SyncInfo.u32Hfb  = 528;
            st1SyncInfo.u32Hbb  = 192;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 44;
            st1SyncInfo.u32Vpw = 5;
            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1080P_24_FP:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M

            st1SyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 1080;
            st1SyncInfo.u32Vfb  = 4;
            st1SyncInfo.u32Vbb  = 41;

            st1SyncInfo.u32Hact = 1920;
            st1SyncInfo.u32Hfb  = 638;
            st1SyncInfo.u32Hbb  = 192;

            st1SyncInfo.u32Bvact =0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  =0;

            st1SyncInfo.u32Hpw = 44;
            st1SyncInfo.u32Vpw = 5;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Hmid = 1;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1080I_50_FP:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 540;
            st1SyncInfo.u32Vfb  = 2;
            st1SyncInfo.u32Vbb  = 20;

            st1SyncInfo.u32Hact = 1920;
            st1SyncInfo.u32Hfb  = 528;
            st1SyncInfo.u32Hbb  = 192;

            st1SyncInfo.u32Bvact =540;
            st1SyncInfo.u32Bvfb  = 3;
            st1SyncInfo.u32Bvbb  =20;

            st1SyncInfo.u32Hpw = 44;
            st1SyncInfo.u32Vpw = 5;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Hmid = 1129;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }

        case VDP_DISP_DIGFMT_1080P30:
        {
            //1080p@25Hz SMPTE 274M
            //1080p@50Hz SMPTE 274M

            st1SyncInfo.bIop = 1;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 1080;
            st1SyncInfo.u32Vfb  = 4;
            st1SyncInfo.u32Vbb  = 41;

            st1SyncInfo.u32Hact = 1920;
            st1SyncInfo.u32Hfb  = 88;
            st1SyncInfo.u32Hbb  = 192;
            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;
            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Hmid = 0x0;
            break;
        }
        case VDP_DISP_DIGFMT_1080I:
        {

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 540;
            st1SyncInfo.u32Vfb  = 3;
            st1SyncInfo.u32Vbb  = 20;

            st1SyncInfo.u32Hact = 1920;
            st1SyncInfo.u32Hfb  = 528;
            st1SyncInfo.u32Hbb  = 192;

            st1SyncInfo.u32Bvact = 540;
            st1SyncInfo.u32Bvfb  = 2;
            st1SyncInfo.u32Bvbb  = 20;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_720P:
        {

            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 720;
            st1SyncInfo.u32Vfb  = 5;
            st1SyncInfo.u32Vbb  = 25;

            st1SyncInfo.u32Hact = 1280;
            st1SyncInfo.u32Hfb  = 440;
            st1SyncInfo.u32Hbb  = 260;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 40;
            st1SyncInfo.u32Vpw = 5;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);
            break;

        }

        case VDP_DISP_DIGFMT_800x600:
        {

            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 600;
            st1SyncInfo.u32Vfb  = 1;
            st1SyncInfo.u32Vbb  = 27;

            st1SyncInfo.u32Hact = 800;
            st1SyncInfo.u32Hfb  =  40;
            st1SyncInfo.u32Hbb  = 216;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 128;
            st1SyncInfo.u32Vpw = 4;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1280x1024:
        {

            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 1024;
            st1SyncInfo.u32Vfb  = 1;
            st1SyncInfo.u32Vbb  = 41;

            st1SyncInfo.u32Hact = 1280;
            st1SyncInfo.u32Hfb  =  48;
            st1SyncInfo.u32Hbb  = 296;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 112;
            st1SyncInfo.u32Vpw = 3;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_1024x768:
        {

            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 768;
            st1SyncInfo.u32Vfb  = 3;
            st1SyncInfo.u32Vbb  = 35;

            st1SyncInfo.u32Hact = 1024;
            st1SyncInfo.u32Hfb  =  24;
            st1SyncInfo.u32Hbb  = 280;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 136;
            st1SyncInfo.u32Vpw = 6;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_640x480_72:
        {

            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 480;
            st1SyncInfo.u32Vfb  = 1;
            st1SyncInfo.u32Vbb  = 23;

            st1SyncInfo.u32Hact = 640;
            st1SyncInfo.u32Hfb  =  16;
            st1SyncInfo.u32Hbb  = 160;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 40;
            st1SyncInfo.u32Vpw = 3;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_576P:
        {
            st1SyncInfo.bIop = 1;
            //s1tSyncInfo.u32Intfb = 1;
            //s1tSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact =  576;
            st1SyncInfo.u32Vfb  =  0x2;
            st1SyncInfo.u32Vbb  = 0x16;

            st1SyncInfo.u32Hact =  720;
            st1SyncInfo.u32Hfb  =  0xc;
            st1SyncInfo.u32Hbb  = 0x84;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }

        case VDP_DISP_DIGFMT_576I:
        {

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact =   288;
            st1SyncInfo.u32Vfb  =   0x2;
            st1SyncInfo.u32Vbb  =  0x16;

            st1SyncInfo.u32Hact =   720;
            st1SyncInfo.u32Hfb  =   0xc;
            st1SyncInfo.u32Hbb  =  0x84;

            st1SyncInfo.u32Bvact =  288;
            st1SyncInfo.u32Bvfb  =  0x2;
            st1SyncInfo.u32Bvbb  = 0x17;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }

        case VDP_DISP_DIGFMT_480P:
        {
            st1SyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact =  480;
            st1SyncInfo.u32Vfb  =    2;
            st1SyncInfo.u32Vbb  = 0x10;

            st1SyncInfo.u32Hact =  720;
            st1SyncInfo.u32Hfb  = 0x10;
            st1SyncInfo.u32Hbb  = 0x7a;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_480I:
        {

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact =   240;
            st1SyncInfo.u32Vfb  =   0x2;
            st1SyncInfo.u32Vbb  =  0x10;

            st1SyncInfo.u32Hact =   720;
            st1SyncInfo.u32Hfb  =  0x10;
            st1SyncInfo.u32Hbb  =  0x7a;

            st1SyncInfo.u32Bvact =  240;
            st1SyncInfo.u32Bvfb  = 0x03;
            st1SyncInfo.u32Bvbb  = 0x11;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

           // Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }

        case VDP_DISP_DIGFMT_TESTI:
        {
            HI_PRINT("Now is TestMode I\n");

            st1SyncInfo.bIop = 0;
            //stSyncInfo.u32Intfb = 1;
            //stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 64;
            st1SyncInfo.u32Vfb  = 6;
            st1SyncInfo.u32Vbb  = 5;

            st1SyncInfo.u32Hact = 720;
            st1SyncInfo.u32Hfb  = 20;
            st1SyncInfo.u32Hbb  = 10;

            st1SyncInfo.u32Bvact = 64;
            st1SyncInfo.u32Bvfb  = 6;
            st1SyncInfo.u32Bvbb  = 5;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCMUX_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_TESTP:
        {
            HI_PRINT("Now is TestMode P\n");

            st1SyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 2;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 128;
            st1SyncInfo.u32Vfb  = 10;
            st1SyncInfo.u32Vbb  = 10;

            st1SyncInfo.u32Hact = 720;
            st1SyncInfo.u32Hfb  = 16;
            st1SyncInfo.u32Hbb  = 16;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR444;

            //Vou_SetIntfDigSel(VDP_DIGSEL_YCBCR_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_TESTS:
        {
            HI_PRINT("Now is TestMode S\n");

            st1SyncInfo.bIop = 0;
            //1stSyncInfo.u32Intfb = 0;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 64;
            st1SyncInfo.u32Vfb  = 6;
            st1SyncInfo.u32Vbb  = 2;

            st1SyncInfo.u32Hact = 720;
            st1SyncInfo.u32Hfb  = 20;
            st1SyncInfo.u32Hbb  = 10;

            st1SyncInfo.u32Bvact = 64;
            st1SyncInfo.u32Bvfb  = 6;
            st1SyncInfo.u32Bvbb  = 2;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            //Vou_SetIntfDigSel(VDP_DIGSEL_SINGL_EMB);

            break;

        }
        case VDP_DISP_DIGFMT_TESTUT:
        {
            HI_PRINT("Now is TestMode UT\n");

            st1SyncInfo.bIop = 1;
            //1stSyncInfo.u32Intfb = 1;
            //1stSyncInfo.bSynm = 0;

            st1SyncInfo.bIdv = 0;
            st1SyncInfo.bIhs = 0;
            st1SyncInfo.bIvs = 0;

            st1SyncInfo.u32Vact = 2048;
            st1SyncInfo.u32Vfb  = 1;
            st1SyncInfo.u32Vbb  = 1;

            st1SyncInfo.u32Hact = 64;
            st1SyncInfo.u32Hfb  = 8;
            st1SyncInfo.u32Hbb  = 8;

            st1SyncInfo.u32Bvact = 0;
            st1SyncInfo.u32Bvfb  = 0;
            st1SyncInfo.u32Bvbb  = 0;

            st1SyncInfo.u32Hpw = 0x0;
            st1SyncInfo.u32Vpw = 0x0;

            st1SyncInfo.u32Hmid = 0x0;
            //stIntfFmt = VDP_DISP_INTFDATAFMT_YCBCR422;

            break;

        }
        default:
        {
            HI_PRINT("Error,VDP_DISP_OpenIntf() Wrong Default DIG FMT mode\n");
            return ;
        }
    }

    if(st1SyncInfo.u32Hact <= 0)
    {
        st1SyncInfo.u32Hact = 1;
    }
    if(st1SyncInfo.u32Hbb <=0)
    {
        st1SyncInfo.u32Hbb = 1;
    }
    if(st1SyncInfo.u32Hfb <=0)
    {
        st1SyncInfo.u32Hfb = 1;
    }

    if(st1SyncInfo.u32Vact <= 0)
    {
        st1SyncInfo.u32Vact = 1;
    }
    if(st1SyncInfo.u32Vbb <=0)
    {
        st1SyncInfo.u32Vbb = 1;
    }
    if(st1SyncInfo.u32Vfb <=0)
    {
        st1SyncInfo.u32Vfb = 1;
    }

    if(st1SyncInfo.u32Bvact <= 0)
    {
        st1SyncInfo.u32Bvact = 1;
    }
    if(st1SyncInfo.u32Bvbb <=0)
    {
        st1SyncInfo.u32Bvbb = 1;
    }
    if(st1SyncInfo.u32Bvfb <=0)
    {
        st1SyncInfo.u32Bvfb = 1;
    }

    if(st1SyncInfo.u32Hpw <=0)
    {
        st1SyncInfo.u32Hpw = 1;
    }
    if(st1SyncInfo.u32Vpw <=0)
    {
        st1SyncInfo.u32Vpw = 1;
    }

   // VDP_DISP_SetVSync(enChan,stSyncInfo);
   // VDP_DISP_SetHSync(enChan,stSyncInfo);
   // VDP_DISP_SetPlusWidth(enChan,stSyncInfo);
   // VDP_DISP_SetPlusPhase(enChan,stSyncInfo);
   // VDP_DISP_SetOutFmt(enChan,stIntfFmt);
   // VDP_DISP_SetVTThdMode(enChan, bool(!(stSyncInfo.bIop)));
   // VDP_DISP_SetIntfMode(enChan,stSyncInfo);
   VDP_DISP_OpenIntf(u32hd_id,st1SyncInfo);

    return ;

}
# if 0

HI_VOID VDP_DISP_SetVSync(HI_U32 u32hd_id, HI_U32 u32vfb, HI_U32 u32vbb, HI_U32 u32vact)
{
    volatile U_DHD0_VSYNC DHD0_VSYNC;
    if(u32hd_id>=CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetVSync Select Wrong CHANNEL ID\n");
        return ;
    }
    //Config VHD interface veritical timming
    DHD0_VSYNC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_VSYNC.bits.vact = u32vact;
    DHD0_VSYNC.bits.vbb = u32vbb;
    DHD0_VSYNC.bits.vfb = u32vfb;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET), DHD0_VSYNC.u32);
}



HI_VOID VDP_DISP_SetVSyncPlus(HI_U32 u32hd_id, HI_U32 u32bvfb, HI_U32 u32bvbb, HI_U32 u32vact)
{

     volatile U_DHD0_VPLUS DHD0_VPLUS;
    if(u32hd_id>=CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetVSyncPlus Select Wrong CHANNEL ID\n");
        return ;
    }

     //Config VHD interface veritical bottom timming,no use in progressive mode
     DHD0_VPLUS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET));
     DHD0_VPLUS.bits.bvact = bvfb;
     DHD0_VPLUS.bits.bvbb = bvbb;
     DHD0_VPLUS.bits.bvfb = vact;
     VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET), DHD0_VPLUS.u32);


}
HI_VOID VDP_DISP_SetHSync(HI_U32 u32hd_id, HI_U32 u32hfb, HI_U32 u32hbb, HI_U32 u32hact)
{


    volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2;
    if(u32hd_id>=CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetHSync Select Wrong CHANNEL ID\n");
        return ;
    }

        //Config VHD interface horizontal timming
    DHD0_HSYNC1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_HSYNC1.bits.hact = u32hact;
    DHD0_HSYNC1.bits.hbb = u32hbb;
    DHD0_HSYNC2.bits.hfb = u32hfb;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC2.u32);

}


HI_VOID  VDP_DISP_SetPlusWidth(HI_U32 u32hd_id, HI_U32 u32hpw, HI_U32 u32vpw)
{
     volatile U_DHD0_PWR DHD0_PWR;
     if(u32hd_id>=CHN_MAX)
     {
          HI_PRINT("Error,VDP_DISP_SetPlusWidth Select Wrong CHANNEL ID\n");
          return ;
     }
     DHD0_PWR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET));
     DHD0_PWR.bits.hpw = u32hpw;
     DHD0_PWR.bits.vpw = u32vpw;
     VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET), DHD0_PWR.u32);
}

HI_VOID VDP_DISP_SetPlusPhase(HI_U32 u32hd_id, HI_U32 u32ihs, HI_U32 u32ivs, HI_U32 u32idv)
{
     volatile U_DHD0_CTRL DHD0_CTRL;
     if(u32hd_id>=CHN_MAX)
     {
          HI_PRINT("Error,VDP_DISP_SetPlusPhase Select Wrong CHANNEL ID\n");
          return ;
     }
     DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
     DHD0_CTRL.bits.ihs = u32ihs;
     DHD0_CTRL.bits.ivs = u32ivs;
     DHD0_CTRL.bits.idv = u32idv;
     VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET), DHD0_CTRL.u32);
}

#endif

#endif
HI_VOID VDP_DISP_SetSyncInv(HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DISP_SYNCINV_E enInv)
{
    volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetSync_INV Select Wrong CHANNEL ID\n");
        return ;
    }

    switch(enIntf)
    {
        case VDP_DISP_INTF_DATE:
        {
#if 0
            DHD0_SYNC_INV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)));
            DHD0_SYNC_INV.bits.date_dv_inv = enInv.u32DatDinv;
            DHD0_SYNC_INV.bits.date_hs_inv = enInv.u32DatHinv;
            DHD0_SYNC_INV.bits.date_vs_inv = enInv.u32DatVinv;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32);
#endif
            break;
        }
        case VDP_DISP_INTF_HDMI:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)));
            DHD0_SYNC_INV.bits.hdmi_f_inv = enInv.u32Hdfinv;
            DHD0_SYNC_INV.bits.hdmi_vs_inv = enInv.u32Hdvinv;
            DHD0_SYNC_INV.bits.hdmi_hs_inv = enInv.u32Hdhinv;
            DHD0_SYNC_INV.bits.hdmi_dv_inv = enInv.u32Hddinv;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32);

            break;
        }
        case VDP_DISP_INTF_VGA:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)));
            DHD0_SYNC_INV.bits.vga_vs_inv = enInv.u32Vgavinv;
            DHD0_SYNC_INV.bits.vga_hs_inv = enInv.u32Vgahinv;
            DHD0_SYNC_INV.bits.vga_dv_inv = enInv.u32Vgadinv;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32);

            break;
        }
        case VDP_DISP_INTF_LCD:
        {
            DHD0_SYNC_INV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)));
            DHD0_SYNC_INV.bits.lcd_vs_inv = enInv.u32Lcdvinv;
            DHD0_SYNC_INV.bits.lcd_hs_inv = enInv.u32Lcdhinv;
            DHD0_SYNC_INV.bits.lcd_dv_inv = enInv.u32Lcddinv;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+u32hd_id*CHN_OFFSET),DHD0_SYNC_INV.u32);

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            break;
        }
        default:
        {
            HI_PRINT("Error! DP_DISP_SetSyncInv Wrong Select\n");
            return ;
        }
    }
    return ;
}


HI_VOID  VDP_DISP_SetIntfMuxSel(HI_U32 u32hd_id,VDP_DISP_INTF_E enIntf)
{
    volatile U_VO_MUX VO_MUX;

    if(u32hd_id >= CHN_MAX)
     {
          HI_PRINT("Error,VDP_DISP_SetIntfMuxSel Select Wrong CHANNEL ID\n");
          return ;
     }

    switch(enIntf)
    {
        case VDP_DISP_INTF_LCD:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.digital_sel = u32hd_id*2;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }
        case VDP_DISP_INTF_BT1120:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.digital_sel = 1+u32hd_id*2;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }
        case VDP_DISP_INTF_HDMI:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.hdmi_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }
        case VDP_DISP_INTF_VGA:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.vga_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }
        case VDP_DISP_INTF_HDDATE:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.hddate_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }

        case VDP_DISP_INTF_SDDATE:
        {
            VO_MUX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VO_MUX.u32)));;
            VO_MUX.bits.sddate_sel = u32hd_id;
            VDP_RegWrite((HI_U32)&(pVdpReg->VO_MUX.u32), VO_MUX.u32);

            break;
        }

        default:
        {
            HI_PRINT("Error! DP_DISP_SetIntfMuxSel Wrong Select\n");
            return ;
        }
    }
    return ;
}

#ifndef __DISP_PLATFORM_BOOT__
//-------------------------------------------------------------------
//VDP_DISP_DITHER BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_DISP_SetDitherMode  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_E enDitherMode)

{

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong CHANNEL ID\n");
          return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD)
    {

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
    }
    else
    {
         HI_PRINT("Error,VDP_DISP_SetDitherMode Select Wrong Interface Mode\n");
         return ;

    }

}

HI_VOID VDP_DISP_SetDitherCoef  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf, VDP_DITHER_COEF_S dither_coef)

{

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong CHANNEL ID\n");
          return ;
    }

    if(enIntf == VDP_DISP_INTF_LCD)
    {

    }
    else if(enIntf == VDP_DISP_INTF_BT1120)
    {
    }
    else
    {
         HI_PRINT("Error,VDP_DISP_SetDitherCoef Select Wrong Interface Mode\n");
         return ;

    }

}
//-------------------------------------------------------------------
//VDP_DISP_DITHER END
//-------------------------------------------------------------------
#endif


// INT SET
HI_VOID  VDP_DISP_SetIntDisable(HI_U32 u32masktypeen)
{
    volatile U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTMSK.u32)));
    VOINTMSK.u32 = VOINTMSK.u32 & (~u32masktypeen);
    VDP_RegWrite((HI_U32)&(pVdpReg->VOINTMSK.u32), VOINTMSK.u32);

    return ;
}

HI_U32 VDP_DISP_GetIntSta(HI_U32 u32intmask)
{
    volatile U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTSTA.u32)));

    return (VOINTSTA.u32 & u32intmask);
}
HI_U32 VDP_DISP_GetIntMskStat(HI_U32 u32intmask)
{
    volatile U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOINTSTA.u32)));;

    return (VOINTSTA.u32 & u32intmask);
}

HI_VOID  VDP_DISP_ClearIntSta(HI_U32 u32intmask)
{
    /* clear interrupt status */
    VDP_RegWrite((HI_U32)&(pVdpReg->VOMSKINTSTA.u32), u32intmask);
}

///VTT INT
HI_VOID VDP_DISP_SetVtThdMode(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 u32mode)
{
    volatile U_DHD0_VTTHD3 DHD0_VTTHD3;
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
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

#ifndef __DISP_PLATFORM_BOOT__
//-------------------------------------------------------------------
//VDP_DISP_CLIP_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_DISP_SetClipEnable  (HI_U32 u32hd_id, VDP_DISP_INTF_E enIntf)
{
    HI_PRINT("Error,This Driver has been deleted ...\n");
}
#endif

HI_VOID  VDP_DISP_SetClipCoef (HI_U32 u32hd_id, VDP_DISP_INTF_E clipsel, VDP_DISP_CLIP_S stClipData)
{
    volatile U_DHD0_CLIP1_L       DHD0_CLIP1_L;
    volatile U_DHD0_CLIP1_H       DHD0_CLIP1_H;
    volatile U_DHD0_CLIP2_L       DHD0_CLIP2_L;
    volatile U_DHD0_CLIP2_H       DHD0_CLIP2_H;
    volatile U_DHD0_CLIP3_L       DHD0_CLIP3_L;
    volatile U_DHD0_CLIP3_H       DHD0_CLIP3_H;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong CHANNEL ID\n");
        return ;
    }

    //BT1120
    if(clipsel == VDP_DISP_INTF_BT1120)
    {
    }
    //DATE
    else if(clipsel == VDP_DISP_INTF_DATE)
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
    }
    else
    {
        HI_PRINT("Error,VDP_DISP_SetClipVtt Select Wrong Interface Mode\n");
    }
}

//-------------------------------------------------------------------
//VDP_DISP_CLIP_END
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//VDP_DISP_CSC_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_DISP_SetCscEnable  (HI_U32 u32hd_id, HI_U32 enCSC)
{
        volatile U_DHD0_CSC_IDC DHD0_CSC_IDC;

        if(u32hd_id >= CHN_MAX)
        {
            HI_PRINT("Error,VDP_DISP_SetCscEnable Select Wrong CHANNEL ID\n");
            return ;
        }

        DHD0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET));
        DHD0_CSC_IDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IDC.u32);

}
HI_VOID VDP_DISP_SetDataSel  (HI_U32 u32hd_id, HI_U32 u32DataSel)
{
  volatile  U_DHD0_DATA_SEL   DHD0_DATA_SEL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetDataSelSelect Wrong CHANNEL ID\n");
        return ;
    }
    DHD0_DATA_SEL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_DATA_SEL.bits.hdmi_data_sel = u32DataSel;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32)+u32hd_id*CHN_OFFSET),DHD0_DATA_SEL.u32);
}

HI_VOID   VDP_DISP_SetCscDcCoef(HI_U32 u32hd_id,VDP_CSC_DC_COEF_S stCscCoef)
{
    volatile U_DHD0_CSC_IDC DHD0_CSC_IDC;
    volatile U_DHD0_CSC_ODC DHD0_CSC_ODC;
    volatile U_DHD0_CSC_IODC DHD0_CSC_IODC;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetCscDcCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_IDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
    DHD0_CSC_IDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_IDC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IDC.u32);

    DHD0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_ODC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
    DHD0_CSC_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_ODC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_ODC.u32);

    DHD0_CSC_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_IODC.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
    DHD0_CSC_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_IODC.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_IODC.u32);

}

HI_VOID   VDP_DISP_SetCscCoef(HI_U32 u32hd_id,VDP_CSC_COEF_S stCscCoef)
{
    volatile U_DHD0_CSC_P0        DHD0_CSC_P0;
    volatile U_DHD0_CSC_P1        DHD0_CSC_P1;
    volatile U_DHD0_CSC_P2        DHD0_CSC_P2;
    volatile U_DHD0_CSC_P3        DHD0_CSC_P3;
    volatile U_DHD0_CSC_P4        DHD0_CSC_P4;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetCscCoef Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CSC_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_P0.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    DHD0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_P0.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P0.u32);

    DHD0_CSC_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_P1.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    DHD0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_P1.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P1.u32);

    DHD0_CSC_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_P2.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    DHD0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_P2.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P2.u32);

    DHD0_CSC_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_P3.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    DHD0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_P3.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P3.u32);

    DHD0_CSC_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CSC_P4.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CSC_P4.u32)+u32hd_id*CHN_OFFSET), DHD0_CSC_P4.u32);

}

#ifndef __DISP_PLATFORM_BOOT__
HI_VOID VDP_DISP_SetCscMode(HI_U32 u32hd_id, VDP_CSC_MODE_E enCscMode)

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

    VDP_DISP_SetCscCoef  (u32hd_id,st_csc_coef);
    VDP_DISP_SetCscDcCoef(u32hd_id,st_csc_dc_coef);

    return ;
}

HI_VOID VDP_DISP_SetGammaEnable(HI_U32 u32hd_id, HI_U32 u32GmmEn)
{
    volatile U_DHD0_CTRL DHD0_CTRL;

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

HI_VOID VDP_DISP_SetGammaMode(HI_U32 u32hd_id, HI_U32 u32GmmEn)
{
    volatile U_DHD0_CTRL DHD0_CTRL;

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

HI_VOID VDP_DISP_SetGammaAddr(HI_U32 u32hd_id, HI_U32 uGmmAddr)
{
    volatile U_DHD0_GMM_COEFAD DHD0_GMM_COEFAD;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetGammaAddr Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_GMM_COEFAD.u32 = uGmmAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_GMM_COEFAD.u32)+u32hd_id*CHN_OFFSET), DHD0_GMM_COEFAD.u32);

}

HI_VOID VDP_DISP_SetParaUpd(HI_U32 u32hd_id, VDP_DISP_PARA_E enPara)
{
    volatile U_DHD0_PARAUP DHD0_PARAUP;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetParaUp Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_PARAUP.bits.dhd0_gmm_upd = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32)+u32hd_id*CHN_OFFSET), DHD0_PARAUP.u32);
}


//-------------------------------------------------------------------
//VDP_DISP_CSC_END
//-------------------------------------------------------------------

#if 0

//GMMA
HI_VOID VDP_SetGammaUp(HI_U32 u32hd_id)
{
    volatile U_DHD0_PARAUP DHD0_PARAUP;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_SetGammaUp Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32)+u32hd_id*CHN_OFFSET));
    DHD0_PARAUP.bits.dhd0_gamm_upd = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PARAUP.u32)+u32hd_id*CHN_OFFSET),DHD0_PARAUP.u32);
}

HI_VOID VDP_SetGammaCoef(HI_U32 u32hd_id, HI_U32 *upTable)/// FOR TC GMMA TABLE MUST GIVE
{
    volatile U_DHDGAMMAN DHDGAMMAN[33];
    HI_U32 ii = 0;
    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_SetGammaCoef Select Wrong CHANNEL ID\n");
        return ;
    }


     for (ii = 0; ii < 33; ii++)
     {
         DHDGAMMAN[ii].bits.gamma_datarn = upTable[ii + 33*0];
         DHDGAMMAN[ii].bits.gamma_datagn = upTable[ii + 33*1];
         DHDGAMMAN[ii].bits.gamma_databn = upTable[ii + 33*2];
         VDP_RegWrite((HI_U32)(&(pVdpReg->DHDGAMMAN[ii].u32)+u32hd_id*CHN_OFFSET), DHDGAMMAN[ii].u32);
     }

}

#endif
#endif

///GF AND VIDEO MIX CBM
HI_VOID VDP_CBM_SetMixerBkg(VDP_CBM_MIX_E u32mixer_id, VDP_BKG_S stBkg)
{
    volatile U_CBM_BKG1 CBM_BKG1;
    volatile U_CBM_BKG2 CBM_BKG2;
    volatile U_MIXG0_BKG MIXG0_BKG;
    volatile U_MIXG0_BKALPHA MIXG0_BKALPHA;
    volatile U_MIXV0_BKG MIXV0_BKG;

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
        /* DHD1(DSD) mixer link */
        CBM_BKG2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_BKG2.u32)));;
        CBM_BKG2.bits.cbm_bkgy2  = stBkg.u32BkgY;
        CBM_BKG2.bits.cbm_bkgcb2 = stBkg.u32BkgU;
        CBM_BKG2.bits.cbm_bkgcr2 = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG2.u32), CBM_BKG2.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIXG0)
    {
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
        MIXV0_BKG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_BKG.u32)));;
        MIXV0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXV0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXV0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        VDP_RegWrite((HI_U32)&(pVdpReg->MIXV0_BKG.u32), MIXV0_BKG.u32);
    }
    else
    {
        HI_PRINT("Error! VDP_CBM_SetMixerBkg() Select Wrong mixer ID\n");
    }

    return ;
}

#define VDP_CBM_MIX0_LayerNO    2
#define VDP_CBM_MIX1_LayerNO    3

#ifndef __DISP_PLATFORM_BOOT__
HI_U32 VDP_CBM_GetMAXLayer(VDP_CBM_MIX_E eMixId)
{
    if (VDP_CBM_MIX0 == eMixId)
        return VDP_CBM_MIX0_LayerNO;
    else
        return VDP_CBM_MIX1_LayerNO;
}


HI_VOID VDP_CBM_ResetMixerPrio(VDP_CBM_MIX_E u32mixer_id)
{
    volatile U_MIXG0_MIX MIXG0_MIX;
    volatile U_MIXV0_MIX MIXV0_MIX;
    volatile U_CBM_MIX1 CBM_MIX1;
    volatile U_CBM_MIX2 CBM_MIX2;

    if (u32mixer_id == VDP_CBM_MIXG0)
    {
        MIXG0_MIX.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV0)
    {
        MIXV0_MIX.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
    }
    else if (u32mixer_id == VDP_CBM_MIXV1)
    {
        //reserved
    }
    else if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        CBM_MIX1.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        CBM_MIX2.u32 = 0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
    }
    else
    {
        HI_PRINT("Error, VDP_CBM_ResetMixerPrio() Set mixer  select wrong layer ID\n");
    }

    return ;
}

#endif
HI_VOID VDP_CBM_SetMixerPrio(VDP_CBM_MIX_E u32mixer_id,HI_U32 u32layer_id,HI_U32 u32prio)
{
    volatile U_MIXG0_MIX MIXG0_MIX;
    volatile U_MIXV0_MIX MIXV0_MIX;
    volatile U_CBM_MIX1 CBM_MIX1;
    volatile U_CBM_MIX2 CBM_MIX2;

    if (u32mixer_id == VDP_CBM_MIXG0)
    {
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
    else if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            case 1:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                CBM_MIX1.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);
                break;
            }
            default:
            {
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
                return ;
            }

        }
    }
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio0 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio1 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 2:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio2 = u32layer_id+1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);
                break;
            }
            case 3:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                CBM_MIX2.bits.mixer_prio3 = u32layer_id+1;
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
}
#ifndef __DISP_PLATFORM_BOOT__
//-------------------------------------------------------------------
//GFX_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_GFX_SetLayerEnable(HI_U32 u32Data, HI_U32 u32bEnable )
{
    volatile U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerEnable Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.surface_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);

    return ;
}
HI_VOID  VDP_GFX_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    volatile U_G0_CTRL  G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFx_SetNoSecFlag() Select Wrong Video Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.nosec_flag= u32Enable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);
    return ;
}
HI_VOID  VDP_GFX_SetDcmpEnable(HI_U32 u32Data, HI_U32 u32bEnable )
{
    volatile U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerEnable Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.dcmp_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);

    return ;
}

HI_VOID VDP_GFX_SetDcmpAddr(HI_U32 u32Data, HI_U32 u32LAddr)
{
    volatile U_G0_DCMP_ADDR G0_DCMP_ADDR;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_ADDR.u32 = u32LAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DCMP_ADDR.u32)+ u32Data * GFX_OFFSET), G0_DCMP_ADDR.u32);
}

HI_VOID VDP_GFX_SetDcmpNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    volatile U_G0_DCMP_NADDR G0_DCMP_NADDR;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerNAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_NADDR.u32 = u32NAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DCMP_NADDR.u32)+ u32Data * GFX_OFFSET), G0_DCMP_NADDR.u32);
}

HI_VOID VDP_GFX_SetDcmpOffset(HI_U32 u32Data, HI_U32 u32Offset)
{
    volatile U_G0_DCMP_OFFSET G0_DCMP_OFFSET;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetDcmpOffset Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_OFFSET.u32 = u32Offset;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DCMP_OFFSET.u32)+ u32Data * GFX_OFFSET), G0_DCMP_OFFSET.u32);
}






HI_VOID VDP_GFX_SetLayerAddr(HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32Stride)
{
    volatile U_G0_ADDR G0_ADDR;
    volatile U_G0_STRIDE G0_STRIDE;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_ADDR.u32 = u32LAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_ADDR.u32)+ u32Data * GFX_OFFSET), G0_ADDR.u32);

    G0_STRIDE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_STRIDE.u32)+ u32Data * GFX_OFFSET));
    G0_STRIDE.bits.surface_stride = u32Stride;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_STRIDE.u32)+ u32Data * GFX_OFFSET), G0_STRIDE.u32);
}

HI_VOID VDP_GFX_SetLutAddr(HI_U32 u32Data, HI_U32 u32LutAddr)
{
    volatile U_G0_PARAADDR G0_PARAADDR;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLutAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_PARAADDR.u32 = u32LutAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_PARAADDR.u32)+ u32Data * GFX_OFFSET), G0_PARAADDR.u32);
}

//HI_VOID VDP_GFX_SetGammaEnable(HI_U32 u32Data, HI_U32 u32GmmEn)
//{
//    volatile U_G0_CTRL G0_CTRL;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetGammaEnable Select Wrong Graph Layer ID\n");
//        return ;
//    }
//
//    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+u32Data*GFX_MAX));
//    G0_CTRL.bits.gmm_en = u32GmmEn;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+u32Data*GFX_MAX), G0_CTRL.u32);
//}



HI_VOID  VDP_GFX_SetInDataFmt(HI_U32 u32Data, VDP_GFX_IFMT_E  enDataFmt)
{
    volatile U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetDataFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.ifmt = enDataFmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);

    return ;
}


HI_VOID VDP_GFX_SetBitExtend(HI_U32 u32Data, VDP_GFX_BITEXTEND_E u32mode)
{
    volatile U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetBitExtend Select Wrong Graph Layer ID\n");
        return ;
    }


    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.bitext = u32mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);


}

//HI_VOID VDP_SetGfxPalpha(VDP_DISP_LAYER_E enLayer,HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1)//CBM

HI_VOID VDP_GFX_SetColorKey(HI_U32 u32Data,HI_U32  bkeyEn,VDP_GFX_CKEY_S stKey )
{
    volatile U_G0_CKEYMAX G0_CKEYMAX;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CBMPARA G0_CBMPARA;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetColorKey Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CKEYMAX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMAX.bits.keyr_max = stKey.u32Key_r_max;
    G0_CKEYMAX.bits.keyg_max = stKey.u32Key_g_max;
    G0_CKEYMAX.bits.keyb_max = stKey.u32Key_b_max;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET), G0_CKEYMAX.u32);

    G0_CKEYMIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMIN.bits.keyr_min = stKey.u32Key_r_min;
    G0_CKEYMIN.bits.keyg_min = stKey.u32Key_g_min;
    G0_CKEYMIN.bits.keyb_min = stKey.u32Key_b_min;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET), G0_CKEYMIN.u32);

    G0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.key_en = bkeyEn;
    G0_CBMPARA.bits.key_mode = stKey.bKeyMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);

}

HI_VOID VDP_GFX_SetKeyMask(HI_U32 u32Data, VDP_GFX_MASK_S stMsk)
{
    volatile U_G0_CMASK G0_CMASK;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetKeyMask Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CMASK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CMASK.u32)+ u32Data * GFX_OFFSET));
    G0_CMASK.bits.kmsk_r = stMsk.u32Mask_r;
    G0_CMASK.bits.kmsk_g = stMsk.u32Mask_g;
    G0_CMASK.bits.kmsk_b = stMsk.u32Mask_b;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CMASK.u32)+ u32Data * GFX_OFFSET), G0_CMASK.u32);

}


HI_VOID VDP_GFX_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetReadMode Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.read_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);


}

HI_VOID  VDP_GFX_SetParaUpd  (HI_U32 u32Data, VDP_DISP_COEFMODE_E enMode )
{
    volatile U_G0_PARAUP G0_PARAUP;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetParaUpd Select Wrong Graph Layer ID\n");
        return ;
    }

    if (enMode == VDP_DISP_COEFMODE_LUT || enMode == VDP_DISP_COEFMODE_ALL)
    {
        G0_PARAUP.bits.gdc_paraup = 0x1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_PARAUP.u32)+ u32Data * GFX_OFFSET), G0_PARAUP.u32);
    }



}
HI_VOID  VDP_GFX_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_G0_DOF_CTRL  G0_DOF_CTRL;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetThreeDimDofEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_DOF_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOF_CTRL.u32))+ u32Data * GFX_OFFSET);
    G0_DOF_CTRL.bits.dof_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOF_CTRL.u32)+ u32Data * GFX_OFFSET), G0_DOF_CTRL.u32);
}
//HI_VOID  VDP_GFX_SetThreeDimIsRData(HI_U32 u32Data, HI_U32 u32IsRData)
HI_VOID  VDP_GFX_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
    volatile U_G0_DOF_STEP  G0_DOF_STEP;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetThreeDimDofStep() Select Wrong Video Layer ID\n");
        return;
    }

    G0_DOF_STEP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOF_STEP.u32))+ u32Data * GFX_OFFSET);
    G0_DOF_STEP.bits.right_step= s32RStep;
    G0_DOF_STEP.bits.left_step = s32LStep;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOF_STEP.u32)+ u32Data * GFX_OFFSET), G0_DOF_STEP.u32);
}

//3D MODE
//HI_VOID VDP_GFX_SetThreeDimEnable(HI_U32 u32Data,HI_U32 bTrue)
//{
//
//    volatile U_G0_CTRL G0_CTRL;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetThreeDimEnable Select Wrong Graph Layer ID\n");
//        return ;
//    }
//
//    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
//    G0_CTRL.bits.trid_en = bTrue ;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);
//}
//
//HI_VOID VDP_GFX_SetDofEnable(HI_U32 u32Data,HI_U32 bTrue)
//{
//
//    volatile U_G0_DOFCTRL G0_DOFCTRL;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetDofEnable Select Wrong Graph Layer ID\n");
//        return ;
//    }
//
//    G0_DOFCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOFCTRL.u32)+ u32Data * GFX_OFFSET));
//    G0_DOFCTRL.bits.dof_en = bTrue ;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOFCTRL.u32)+ u32Data * GFX_OFFSET), G0_DOFCTRL.u32);
//}
//
//HI_VOID VDP_GFX_SetDofFmt ( HI_U32 u32Data, HI_U32 u32DataFmt)
//{
//    volatile U_G0_DOFCTRL G0_DOFCTRL;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetDofFmt Select Wrong Graph Layer ID\n");
//        return ;
//    }
//
//    G0_DOFCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOFCTRL.u32)+ u32Data * GFX_OFFSET));
//    G0_DOFCTRL.bits.dof_format = u32DataFmt ;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOFCTRL.u32)+ u32Data * GFX_OFFSET), G0_DOFCTRL.u32);
//
//}
//
//HI_VOID VDP_GFX_SetDofStep ( HI_U32 u32Data, HI_U32 u32eye_sel,HI_U32 u32step)
//{
//    volatile U_G0_DOFSTEP G0_DOFSTEP;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetDofStep Select Wrong Graph Layer ID\n");
//        return ;
//    }
//    if(u32eye_sel== 0)
//    {
//
//        G0_DOFSTEP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOFSTEP.u32)+ u32Data * GFX_OFFSET));
//
//        G0_DOFSTEP.bits.left_eye = 0 ;
//        G0_DOFSTEP.bits.left_step = u32step ;
//        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOFSTEP.u32)+ u32Data * GFX_OFFSET), G0_DOFSTEP.u32);
//
//    }
//    if(u32eye_sel== 1)
//    {
//
//        G0_DOFSTEP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DOFSTEP.u32)+ u32Data * GFX_OFFSET));
//
//        G0_DOFSTEP.bits.left_eye = 1 ;
//        G0_DOFSTEP.bits.right_step = u32step ;
//        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DOFSTEP.u32)+ u32Data * GFX_OFFSET), G0_DOFSTEP.u32);
//
//    }
//
//}
//
//HI_VOID VDP_GFX_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
//{
//    volatile U_G0_CTRL G0_CTRL;
//
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetDispMode() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//
//    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
//    G0_CTRL.bits.disp_mode = enDispMode;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * VID_OFFSET), G0_CTRL.u32);
//
//    return ;
//}

HI_VOID VDP_GFX_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    return;
}


HI_VOID  VDP_GFX_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{

    volatile U_G0_SFPOS G0_SFPOS;
    volatile U_G0_VFPOS G0_VFPOS;
    volatile U_G0_VLPOS G0_VLPOS;
    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;
    volatile U_G0_IRESO G0_IRESO;
    //volatile U_G0_ORESO G0_ORESO;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }



    // Read source position
    G0_SFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_SFPOS.u32)+ u32Data * GFX_OFFSET));
    G0_SFPOS.bits.src_xfpos = stRect.u32SX;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_SFPOS.u32)+ u32Data * GFX_OFFSET), G0_SFPOS.u32);

    G0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VFPOS.u32) + u32Data * VID_OFFSET));
    G0_VFPOS.bits.video_xfpos = stRect.u32VX;
    G0_VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VFPOS.u32) + u32Data * VID_OFFSET), G0_VFPOS.u32);

    G0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VLPOS.u32) + u32Data * VID_OFFSET));
    G0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    G0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VLPOS.u32) + u32Data * VID_OFFSET), G0_VLPOS.u32);

    // display position
    G0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DFPOS.u32)+ u32Data * GFX_OFFSET));
    G0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    G0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DFPOS.u32)+ u32Data * GFX_OFFSET), G0_DFPOS.u32);

    G0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DLPOS.u32)+ u32Data * GFX_OFFSET));
    G0_DLPOS.bits.disp_xlpos = stRect.u32DXL - 1;
    G0_DLPOS.bits.disp_ylpos = stRect.u32DYL - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DLPOS.u32)+ u32Data * GFX_OFFSET), G0_DLPOS.u32);

    // input width and height
    G0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_IRESO.u32)+ u32Data * GFX_OFFSET));
    G0_IRESO.bits.iw = stRect.u32IWth - 1;
    G0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_IRESO.u32)+ u32Data * GFX_OFFSET), G0_IRESO.u32);

    // output width and height
    //G0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_ORESO.u32)+ u32Data * GFX_OFFSET));
    //G0_ORESO.bits.ow = stRect.u32OWth - 1;
    //G0_ORESO.bits.oh = stRect.u32OHgt - 1;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->G0_ORESO.u32)+ u32Data * GFX_OFFSET), G0_ORESO.u32);

    return ;
}

HI_VOID  VDP_GFX_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_G0_VFPOS G0_VFPOS;
   volatile U_G0_VLPOS G0_VLPOS;

   if(u32Data >= GFX_MAX)
   {
       HI_PRINT("Error,VDP_GFX_SetVideoPos() Select Wrong Video Layer ID\n");
       return ;
   }


   /*video position */
   G0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VFPOS.u32) + u32Data * GFX_OFFSET));
   G0_VFPOS.bits.video_xfpos = stRect.u32X;
   G0_VFPOS.bits.video_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VFPOS.u32) + u32Data * GFX_OFFSET), G0_VFPOS.u32);

   G0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VLPOS.u32) + u32Data * GFX_OFFSET));
   G0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
   G0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VLPOS.u32) + u32Data * GFX_OFFSET), G0_VLPOS.u32);
   return ;
}

HI_VOID  VDP_GFX_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_G0_DFPOS G0_DFPOS;
   volatile U_G0_DLPOS G0_DLPOS;

   if(u32Data >= GFX_MAX)
   {
       HI_PRINT("Error,VDP_GFX_SetDispPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   G0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DFPOS.u32) + u32Data * GFX_OFFSET));
   G0_DFPOS.bits.disp_xfpos = stRect.u32X;
   G0_DFPOS.bits.disp_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DFPOS.u32) + u32Data * GFX_OFFSET), G0_DFPOS.u32);

   G0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_DLPOS.u32) + u32Data * GFX_OFFSET));
   G0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
   G0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->G0_DLPOS.u32) + u32Data * GFX_OFFSET), G0_DLPOS.u32);
   return ;
}



HI_VOID  VDP_GFX_SetInReso(HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_G0_IRESO G0_IRESO;

   if(u32Data >= GFX_MAX)
   {
       HI_PRINT("Error,VDP_GFX_SetInReso Select Wrong Graph Layer ID\n");
       return ;
   }



   //grap position
   G0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_IRESO.u32) + u32Data * GFX_OFFSET));
   G0_IRESO.bits.iw = stRect.u32Wth - 1;
   G0_IRESO.bits.ih = stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->G0_IRESO.u32) + u32Data * GFX_OFFSET), G0_IRESO.u32);
   return ;
}


//HI_VOID VDP_GFX_SetCmpAddr(HI_U32 u32Data, HI_U32 u32CmpAddr)
//{
//    volatile U_G0_CMPADDR G0_CMPADDR;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetCmpAddr() Select Wrong Graph Layer ID\n");
//        return ;
//    }
//    G0_CMPADDR.u32 = u32CmpAddr;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CMPADDR.u32)+ u32Data * GFX_OFFSET), G0_CMPADDR.u32);
//
//}
//
HI_VOID  VDP_GFX_SetRegUp (HI_U32 u32Data)
{
    volatile U_G0_UPD G0_UPD;

    /* GO layer register update */
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetRegup() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_UPD.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_UPD.u32) + u32Data * GFX_OFFSET), G0_UPD.u32);

    return ;
}

HI_VOID  VDP_GFX_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    volatile U_G0_BK    G0_BK;
    volatile U_G0_ALPHA G0_ALPHA;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerBkg() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_BK.u32) + u32Data * GFX_OFFSET));
    G0_BK.bits.vbk_y  = stBkg.u32BkgY ;
    G0_BK.bits.vbk_cb = stBkg.u32BkgU;
    G0_BK.bits.vbk_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_BK.u32) + u32Data * GFX_OFFSET), G0_BK.u32);

    G0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_ALPHA.u32) + u32Data * GFX_OFFSET));
    G0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_ALPHA.u32) + u32Data * GFX_OFFSET), G0_ALPHA.u32);

    return ;
}

HI_VOID  VDP_GFX_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    volatile U_G0_CBMPARA G0_CBMPARA;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerGalpha() Select Wrong Video Layer ID\n");
        return ;
    }


    G0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CBMPARA.u32) + u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.galpha = u32Alpha0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CBMPARA.u32) + u32Data * GFX_OFFSET), G0_CBMPARA.u32);

    return ;
}


HI_VOID VDP_GFX_SetPalpha(HI_U32 u32Data, HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1)
{


    volatile U_G0_CBMPARA G0_CBMPARA;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CKEYMAX G0_CKEYMAX;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetPalpha() Select Wrong Graph Layer ID\n");
        return ;
    }


    G0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.palpha_en = bAlphaEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);

    G0_CKEYMIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMIN.bits.va1 = u32Alpha1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET), G0_CKEYMIN.u32);

    G0_CKEYMAX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMAX.bits.va0 = u32Alpha0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET), G0_CKEYMAX.u32);

    G0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.palpha_range = bArange;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);

}



HI_VOID VDP_GFX_SetLayerNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    volatile U_G0_NADDR G0_NADDR;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetLayerNAddr() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_NADDR.u32 = u32NAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_NADDR.u32)+ u32Data * GFX_OFFSET), G0_NADDR.u32);

}

HI_VOID  VDP_GFX_SetMuteEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetMuteEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);
}

HI_VOID VDP_GFX_SetPreMultEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_G0_CBMPARA G0_CBMPARA;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_SetGfxPreMultEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CBMPARA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.premult_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);
}


HI_VOID  VDP_GFX_SetUpdMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_Gfx_SetUpdMode() Select Wrong Graph Layer ID\n");
        return ;
    }

    /* G0 layer register update mode */
    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.upd_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);
}

//HI_VOID  VDP_GFX_SetDeCmpEnable(HI_U32 u32Data, HI_U32 bEnable)
//{
//    volatile U_G0_CTRL G0_CTRL;
//    if(u32Data >= GFX_MAX)
//    {
//        HI_PRINT("Error,VDP_GFX_SetDeCmpEnable() Select Wrong Graph Layer ID\n");
//        return ;
//    }
//    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
//    G0_CTRL.bits.decmp_en = bEnable;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);
//}

HI_VOID  VDP_GFX_SetFlipEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetFlipEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.flip_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32);
}




//-------------------------------------------------------------------
//GP_BEGIN
//-------------------------------------------------------------------
//
HI_VOID  VDP_GP_SetParaUpd       (HI_U32 u32Data, VDP_GP_PARA_E enMode)
{
    volatile U_GP0_PARAUP GP0_PARAUP;
    GP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_PARAUP.u32) + u32Data * GP_OFFSET));

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("error,VDP_GP_SetParaUpd() select wrong GP layer id\n");
        return ;
    }
    if(enMode == VDP_GP_PARA_ZME_HOR)
    {
        GP0_PARAUP.bits.gp0_hcoef_upd = 0x1;
    }
    else if(enMode == VDP_GP_PARA_ZME_VER)
    {
        GP0_PARAUP.bits.gp0_vcoef_upd = 0x1;
    }
    else
    {
        HI_PRINT("error,VDP_GP_SetParaUpd() select wrong mode!\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_PARAUP.u32) + u32Data * GP_OFFSET), GP0_PARAUP.u32);
    return ;
}


HI_VOID VDP_GP_SetIpOrder (HI_U32 u32Data, HI_U32 u32Chn, VDP_GP_ORDER_E enIpOrder)
{
    volatile U_GP0_CTRL GP0_CTRL ;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetIpOrder() Select Wrong GP Layer ID\n");
        return ;
    }
    if((u32Data == VDP_LAYER_GP0) && (u32Chn == 1))
    {
        HI_PRINT("Error,VDP_LAYER_GP0 have no channel 1\n");
        return;
    }

    GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET));

    if(u32Chn == 0)//channel A
    {
        if(u32Data == VDP_LAYER_GP1)
        {
            //for GP1
            GP0_CTRL.bits.ffc_sel = 0;
        }
        switch(enIpOrder)
        {
            case VDP_GP_ORDER_NULL:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case VDP_GP_ORDER_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 0;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 2;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case VDP_GP_ORDER_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case VDP_GP_ORDER_CSC_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 0;
                GP0_CTRL.bits.mux2_sel = 2;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case VDP_GP_ORDER_ZME_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 3;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 2;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            default://null
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
        }
    }
    else if(u32Chn == 1)//channel B
    {
        if(u32Data == VDP_LAYER_GP1)
        {
            //for WBC_GP0
            GP0_CTRL.bits.ffc_sel = 1;
        }
        switch(enIpOrder)
        {
            case VDP_GP_ORDER_NULL:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case VDP_GP_ORDER_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }
            case VDP_GP_ORDER_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 1;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 3;

                break;
            }
            case VDP_GP_ORDER_CSC_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 2;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 3;

                break;
            }
            case VDP_GP_ORDER_ZME_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 3;
                GP0_CTRL.bits.mux2_sel = 1;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }
            default://null
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
        }
    }
    else
    {
        HI_PRINT("Error,VDP_GP_SetIpOrder() Select Wrong GP Channel\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET), GP0_CTRL.u32);
}

HI_VOID VDP_GP_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_GP0_CTRL GP0_CTRL;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetReadMode Select Wrong Graph Layer ID\n");
        return ;
    }

    GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET));
    GP0_CTRL.bits.read_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET), GP0_CTRL.u32);


}


HI_VOID  VDP_GP_SetRect  (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    volatile U_GP0_IRESO GP0_IRESO;
    volatile U_GP0_ORESO GP0_ORESO;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetRect() Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET));
    GP0_IRESO.bits.iw = stRect.u32IWth - 1;
    GP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET), GP0_IRESO.u32);

    GP0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET));
    GP0_ORESO.bits.ow = stRect.u32OWth - 1;
    GP0_ORESO.bits.oh = stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET), GP0_ORESO.u32);

    return ;
}

HI_VOID  VDP_GP_SetLayerReso (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{

    volatile U_GP0_VFPOS GP0_VFPOS;
    volatile U_GP0_VLPOS GP0_VLPOS;
    volatile U_GP0_DFPOS GP0_DFPOS;
    volatile U_GP0_DLPOS GP0_DLPOS;
    volatile U_GP0_IRESO GP0_IRESO;
    volatile U_GP0_ORESO GP0_ORESO;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetLayerReso() Select Wrong GP Layer ID\n");
        return ;
    }

    /*video position */
    GP0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET));
    GP0_VFPOS.bits.video_xfpos = stRect.u32VX;
    GP0_VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET), GP0_VFPOS.u32);

    GP0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET));
    GP0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    GP0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET), GP0_VLPOS.u32);

    GP0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET));
    GP0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    GP0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET), GP0_DFPOS.u32);

    GP0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET));
    GP0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
    GP0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET), GP0_DLPOS.u32);

    GP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET));
    GP0_IRESO.bits.iw = stRect.u32IWth - 1;
    GP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET), GP0_IRESO.u32);

    GP0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET));
    GP0_ORESO.bits.ow = stRect.u32OWth - 1;
    GP0_ORESO.bits.oh = stRect.u32OHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET), GP0_ORESO.u32);

    return ;
}

HI_VOID  VDP_GP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
   volatile U_GP0_VFPOS GP0_VFPOS;
   volatile U_GP0_VLPOS GP0_VLPOS;

   if(u32Data >= GP_MAX)
   {
       HI_PRINT("Error,VDP_GP_SetVideoPos() Select Wrong Video Layer ID\n");
       return ;
   }



   /*video position */
   GP0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET));
   GP0_VFPOS.bits.video_xfpos = stRect.u32X;
   GP0_VFPOS.bits.video_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET), GP0_VFPOS.u32);

   GP0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET));
   GP0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
   GP0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET), GP0_VLPOS.u32);
   return ;
}

HI_VOID  VDP_GP_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{

   volatile U_GP0_DFPOS GP0_DFPOS;
   volatile U_GP0_DLPOS GP0_DLPOS;

   if(u32Data >= GP_MAX)
   {
       HI_PRINT("Error,VDP_GP_SetDispPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   GP0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET));
   GP0_DFPOS.bits.disp_xfpos = stRect.u32X;
   GP0_DFPOS.bits.disp_yfpos = stRect.u32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET), GP0_DFPOS.u32);

   GP0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET));
   GP0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
   GP0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET), GP0_DLPOS.u32);
   return ;
}

HI_VOID  VDP_GP_SetInReso (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_GP0_IRESO GP0_IRESO;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetInReso() Select Wrong GP Layer ID\n");
        return ;
    }


    GP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET));
    GP0_IRESO.bits.iw = stRect.u32Wth - 1;
    GP0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET), GP0_IRESO.u32);

    return ;
}

HI_VOID  VDP_GP_SetOutReso (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    volatile U_GP0_ORESO GP0_ORESO;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetOutReso() Select Wrong GP Layer ID\n");
        return ;
    }


    GP0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET));
    GP0_ORESO.bits.ow = stRect.u32Wth - 1;
    GP0_ORESO.bits.oh = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET), GP0_ORESO.u32);

    return ;
}

HI_VOID  VDP_GP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    volatile U_GP0_GALPHA GP0_GALPHA;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetLayerGalpha() Select Wrong gp Layer ID\n");
        return ;
    }


    GP0_GALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_GALPHA.u32) + u32Data * GP_OFFSET));
    GP0_GALPHA.bits.galpha = u32Alpha;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_GALPHA.u32) + u32Data * GP_OFFSET), GP0_GALPHA.u32);

    return ;
}

HI_VOID  VDP_GP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    volatile U_GP0_BK GP0_BK;
    volatile U_GP0_ALPHA     GP0_ALPHA;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetLayerBkg() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_BK.u32) + u32Data * GP_OFFSET));
    GP0_BK.bits.vbk_y  = stBkg.u32BkgY;
    GP0_BK.bits.vbk_cb = stBkg.u32BkgU;
    GP0_BK.bits.vbk_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_BK.u32) + u32Data * GP_OFFSET), GP0_BK.u32);

    GP0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ALPHA.u32) + u32Data * GP_OFFSET));
    GP0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ALPHA.u32) + u32Data * GP_OFFSET), GP0_ALPHA.u32);

    return ;
}

HI_VOID  VDP_GP_SetRegUp  (HI_U32 u32Data)
{
    volatile U_GP0_UPD GP0_UPD;

    /* GP layer register update */
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetRegUp() Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_UPD.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_UPD.u32) + u32Data * GP_OFFSET), GP0_UPD.u32);

    return ;
}

//-------------------------------------------------------------------
// GP.CSC begin
//-------------------------------------------------------------------

HI_VOID  VDP_GP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    volatile U_GP0_CSC_IDC  GP0_CSC_IDC;
    volatile U_GP0_CSC_ODC  GP0_CSC_ODC;
    volatile U_GP0_CSC_IODC GP0_CSC_IODC;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetCscDcCoef() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
    GP0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET), GP0_CSC_IDC.u32);

    GP0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_ODC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
    GP0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_ODC.u32) + u32Data * GP_OFFSET), GP0_CSC_ODC.u32);

    GP0_CSC_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IODC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
    GP0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IODC.u32) + u32Data * GP_OFFSET), GP0_CSC_IODC.u32);

    return ;
}

HI_VOID   VDP_GP_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{
    volatile U_GP0_CSC_P0 GP0_CSC_P0;
    volatile U_GP0_CSC_P1 GP0_CSC_P1;
    volatile U_GP0_CSC_P2 GP0_CSC_P2;
    volatile U_GP0_CSC_P3 GP0_CSC_P3;
    volatile U_GP0_CSC_P4 GP0_CSC_P4;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetCscCoef Select Wrong GPeo ID\n");
        return ;
    }


    GP0_CSC_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P0.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    GP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P0.u32)+u32Data*GP_OFFSET), GP0_CSC_P0.u32);

    GP0_CSC_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P1.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    GP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P1.u32)+u32Data*GP_OFFSET), GP0_CSC_P1.u32);

    GP0_CSC_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P2.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    GP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P2.u32)+u32Data*GP_OFFSET), GP0_CSC_P2.u32);

    GP0_CSC_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P3.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    GP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P3.u32)+u32Data*GP_OFFSET), GP0_CSC_P3.u32);

    GP0_CSC_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P4.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P4.u32)+u32Data*GP_OFFSET), GP0_CSC_P4.u32);

}


HI_VOID  VDP_GP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    volatile U_GP0_CSC_IDC GP0_CSC_IDC;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetCscEnable() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IDC.bits.csc_en = u32bCscEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET), GP0_CSC_IDC.u32);

    return ;
}

HI_VOID VDP_GP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)

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

    VDP_GP_SetCscCoef(u32Data,st_csc_coef);
    VDP_GP_SetCscDcCoef(u32Data,st_csc_dc_coef);

    return ;
}


HI_VOID VDP_GP_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    return;
}

//HI_VOID VDP_GP_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
//{
//    volatile U_GP0_CTRL GP0_CTRL;
//
//    if(u32Data >= GP_MAX)
//    {
//        HI_PRINT("Error,VDP_GP_SetDispMode() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//
//    GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CTRL.u32) + u32Data * GP_OFFSET));
//    GP0_CTRL.bits.disp_mode = enDispMode;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CTRL.u32) + u32Data * VID_OFFSET), GP0_CTRL.u32);
//
//    return ;
//}

//-------------------------------------------------------------------
// GP.ZME begin
//-------------------------------------------------------------------

HI_VOID VDP_GP_SetZmeEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /*GP zoom enable */
    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hsc_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vsc_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}

HI_VOID VDP_GP_SetZmeFirEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hafir_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vafir_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vfir_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}

HI_VOID VDP_GP_SetZmeMidEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hamid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vamid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vlmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vchmid_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID VDP_GP_SetZmeHorRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID VDP_GP_SetZmeVerRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_VSR        GP0_ZME_VSR;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + u32Data * GP_OFFSET));
    GP0_ZME_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + u32Data * GP_OFFSET), GP0_ZME_VSR.u32);

    return ;
}



HI_VOID VDP_GP_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID VDP_GP_SetZmeCoefAddr  (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    volatile U_GP0_HCOEFAD    GP0_HCOEFAD;
    volatile U_GP0_VCOEFAD    GP0_VCOEFAD;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_GP_PARA_ZME_HOR)
    {
        GP0_HCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_HCOEFAD.u32) + u32Data * GP_OFFSET));
        GP0_HCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_HCOEFAD.u32) + u32Data * GP_OFFSET), GP0_HCOEFAD.u32);
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VER)
    {
        GP0_VCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VCOEFAD.u32) + u32Data * GP_OFFSET));
        GP0_VCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VCOEFAD.u32) + u32Data * GP_OFFSET), GP0_VCOEFAD.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_GP_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}


HI_VOID VDP_GP_SetParaRd(HI_U32 u32Data, VDP_GP_PARA_E enMode)
{
    volatile U_GP0_PARARD GP0_PARARD;

    GP0_PARARD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_PARARD.u32) + u32Data * GP_OFFSET));

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetParaRd() Select Wrong Video Layer ID\n");
        return ;
    }
    if(enMode == VDP_GP_PARA_ZME_HORL)
    {
        GP0_PARARD.bits.gp0_hlcoef_rd = 0x1;
    }

    if(enMode == VDP_GP_PARA_ZME_HORC)
    {
        GP0_PARARD.bits.gp0_hccoef_rd = 0x1;
    }

    if(enMode == VDP_GP_PARA_ZME_VERL)
    {
        GP0_PARARD.bits.gp0_vlcoef_rd = 0x1;
    }

    if(enMode == VDP_GP_PARA_ZME_VERC)
    {
        GP0_PARARD.bits.gp0_vccoef_rd = 0x1;
    }

    if(enMode == VDP_GP_GTI_PARA_ZME_HORL)
    {
        GP0_PARARD.bits.gp0_gti_hlcoef_rd = 0x1;
    }

    if(enMode == VDP_GP_GTI_PARA_ZME_HORC)
    {
        GP0_PARARD.bits.gp0_gti_hccoef_rd = 0x1;
    }

    if(enMode == VDP_GP_GTI_PARA_ZME_VERL)
    {
        GP0_PARARD.bits.gp0_gti_vlcoef_rd = 0x1;
    }

    if(enMode == VDP_GP_PARA_ZME_VERC)
    {
        GP0_PARARD.bits.gp0_gti_vccoef_rd = 0x1;
    }

    else
    {
        HI_PRINT("error,VDP_VID_SetParaUpd() select wrong mode!\n");
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_PARARD.u32) + u32Data * GP_OFFSET), GP0_PARARD.u32);
    return ;
}

//-------------------------------------------------------------------
// GP.ZME.GTI(LTI/CTI) begin
//-------------------------------------------------------------------

HI_VOID  VDP_GP_SetTiEnable(HI_U32 u32Data, HI_U32 u32Md,HI_U32 u32Data1)
{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lti_en = u32Data1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32);

    }
    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cti_en = u32Data1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  VDP_GP_SetTiGainRatio(HI_U32 u32Data, HI_U32 u32Md, HI_S32 s32Data)

{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_SetTiGainRatio() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lgain_ratio = s32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cgain_ratio = s32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  VDP_GP_SetTiMixRatio(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32mixing_ratio)

{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiMixRatio() Select Wrong Graph Layer ID\n");
        return ;
    }


    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lmixing_ratio = u32mixing_ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cmixing_ratio = u32mixing_ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  VDP_GP_SetTiHfThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32TiHfThd)
{
    volatile U_GP0_ZME_LHFREQTHD GP0_ZME_LHFREQTHD;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiHfThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LHFREQTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LHFREQTHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd0 = u32TiHfThd[0];
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd1 = u32TiHfThd[1];
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LHFREQTHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LHFREQTHD.u32);
    }


}

HI_VOID  VDP_GP_SetTiHpCoef(HI_U32 u32Data, HI_U32 u32Md, HI_S32 * s32Data)
{

    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    volatile U_GP0_ZME_LHPASSCOEF GP0_ZME_LHPASSCOEF;
    volatile U_GP0_ZME_CHPASSCOEF GP0_ZME_CHPASSCOEF;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiHpCoef() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHPASSCOEF.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LHPASSCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef0 = s32Data[0];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef1 = s32Data[1];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef2 = s32Data[2];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef3 = s32Data[3];
        GP0_ZME_LTICTRL.bits.lhpass_coef4    = s32Data[4];
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LHPASSCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_LHPASSCOEF.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CHPASSCOEF.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CHPASSCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CHPASSCOEF.bits.chpass_coef0 = s32Data[0];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef1 = s32Data[1];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef2 = s32Data[2];
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CHPASSCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_CHPASSCOEF.u32);
    }
}

HI_VOID  VDP_GP_SetTiCoringThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd)
{
    volatile U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    volatile U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiCoringThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTITHD.bits.lcoring_thd = u32thd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTITHD.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTITHD.bits.ccoring_thd = u32thd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTITHD.u32);
    }

}

//***********************have problem !!!*****************************************************
HI_VOID  VDP_GP_SetTiSwingThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd, HI_U32 u32thd1)//have problem
{
    volatile U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    volatile U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,Vou_SetTiSwingThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTITHD.bits.lover_swing  = u32thd;
        GP0_ZME_LTITHD.bits.lunder_swing = u32thd1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTITHD.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTITHD.bits.cover_swing  = u32thd;
        GP0_ZME_CTITHD.bits.cunder_swing = u32thd1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTITHD.u32);
    }

}

//***********************************************************************************************
HI_VOID  VDP_GP_SetTiGainCoef(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32coef)
{
    volatile U_GP0_ZME_LGAINCOEF GP0_ZME_LGAINCOEF;
    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiGainCoef() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LGAINCOEF.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_LGAINCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LGAINCOEF.bits.lgain_coef0 = u32coef[0];
        GP0_ZME_LGAINCOEF.bits.lgain_coef1 = u32coef[1];
        GP0_ZME_LGAINCOEF.bits.lgain_coef2 = u32coef[2];
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_LGAINCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_LGAINCOEF.u32);
    }
}

HI_VOID  VDP_GP_SetTiDefThd(HI_U32 u32Data, HI_U32 u32Md)
{
    HI_U32 u32HfThd[2]    = {20,512};
    HI_U32 u32GainCoef[3] = {152,118,108};
//    HI_S32  s32LtiHpCoef[5] = {-19,-8,-2,-1,-1};
//    HI_S32  s32CtiHpCoef[5] = {-21,-21,-11,0,0};

    HI_S32  s32CtiHpCoef[5] = {-21,-21,-11,0,0};
    HI_S32  s32LtiHpCoef[5] = {-21,-21,-11,0,0};

    if(u32Data >= GP_MAX)
    {
        HI_PRINT("Error,VDP_GP_SetTiDefThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if(u32Md == VDP_TI_MODE_LUM)
    {
        VDP_GP_SetTiGainRatio(u32Data, u32Md, 255);
        VDP_GP_SetTiMixRatio (u32Data, u32Md, 127);
        VDP_GP_SetTiCoringThd(u32Data, u32Md, 0);
        VDP_GP_SetTiSwingThd (u32Data, u32Md, 6, 6);
        VDP_GP_SetTiHfThd    (u32Data, u32Md, u32HfThd);
        VDP_GP_SetTiGainCoef (u32Data, u32Md, u32GainCoef);
        VDP_GP_SetTiHpCoef   (u32Data, u32Md, s32LtiHpCoef);
    }

    if(u32Md == VDP_TI_MODE_CHM)
    {
        VDP_GP_SetTiGainRatio(u32Data, u32Md, 255);
        VDP_GP_SetTiMixRatio (u32Data, u32Md, 127);
        VDP_GP_SetTiCoringThd(u32Data, u32Md, 4);
        VDP_GP_SetTiSwingThd (u32Data, u32Md, 6, 6);
        VDP_GP_SetTiHfThd    (u32Data, u32Md, u32HfThd);
        VDP_GP_SetTiGainCoef (u32Data, u32Md, u32GainCoef);
        VDP_GP_SetTiHpCoef   (u32Data, u32Md, s32CtiHpCoef);
    }
}



#if 0
///////////////////////GO ZME  BEGIN///
/* graphic0 layer zoom enable */
HI_VOID  VDP_GFX_SetZmeEnable(HI_U32 u32Data,VDP_ZME_MODE_E enMode,HI_U32 bEnable)
{
    volatile U_G0_HSP G0_HSP;
    volatile U_G0_VSP G0_VSP;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetZmeEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {


        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hsc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vsc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }
}

HI_VOID VDP_GFX_SetZmeFirEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 bEnable)
{

    volatile U_G0_VSP G0_VSP;
    volatile U_G0_HSP G0_HSP;


    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_SetZmeFirEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

    /* g0 layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hfir_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hafir_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vfir_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vafir_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }

}

/* Vou set Median filter enable */
HI_VOID  VDP_GFX_SetZmeMidEnable(HI_U32 u32Data,VDP_ZME_MODE_E enMode, HI_U32 bEnable)
{
    volatile U_G0_HSP G0_HSP;
    volatile U_G0_VSP G0_VSP;

    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetZmeMidEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    /* G0 layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hlmid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }
    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hchmid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }
    if((enMode == VDP_ZME_MODE_ALPHA) || (enMode == VDP_ZME_MODE_HOR) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
        G0_HSP.bits.hamid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);
    }
    if((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vlmid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }
    if((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vchmid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }
    if((enMode == VDP_ZME_MODE_ALPHAV) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
    {
        G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
        G0_VSP.bits.vamid_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
    }



}

HI_VOID VDP_GFX_SetZmeHfirOrder(HI_U32 u32Data, HI_U32 uHfirOrder)
{
    volatile U_G0_HSP G0_HSP;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetZmeHfirOrder() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET));
    G0_HSP.bits.hfir_order = uHfirOrder;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HSP.u32)+ u32Data * GFX_OFFSET), G0_HSP.u32);


}
HI_VOID VDP_GFX_SetZmeVerTap(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)
{
    volatile U_G0_VSP G0_VSP;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetZmeVerTap() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET));
    G0_VSP.bits.vsc_luma_tap = u32VerTap;
    VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VSP.u32)+ u32Data * GFX_OFFSET), G0_VSP.u32);
}

/* Vou set zoom inital phase */
HI_VOID  VDP_GFX_SetZmePhase(HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    volatile U_G0_HOFFSET G0_HOFFSET;
    volatile U_G0_VOFFSET G0_VOFFSET;
    if(u32Data >= GFX_MAX)
    {
        HI_PRINT("Error,VDP_GFX_SetZmePhase() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_HOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HOFFSET.u32)+ u32Data * GFX_OFFSET));
        G0_HOFFSET.bits.hor_loffset = (s32Phase);
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HOFFSET.u32)+ u32Data * GFX_OFFSET), G0_HOFFSET.u32);
    }
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_HOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HOFFSET.u32)+ u32Data * GFX_OFFSET));
        G0_HOFFSET.bits.hor_coffset = (s32Phase);
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_HOFFSET.u32)+ u32Data * GFX_OFFSET), G0_HOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERT)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_HOFFSET.u32)+ u32Data * GFX_OFFSET));
        G0_VOFFSET.bits.vtp_offset = (s32Phase);
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VOFFSET.u32)+ u32Data * GFX_OFFSET), G0_VOFFSET.u32);
    }
    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        G0_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->G0_VOFFSET.u32)+ u32Data * GFX_OFFSET));
        G0_VOFFSET.bits.vbtm_offset = (s32Phase);
        VDP_RegWrite((HI_U32)(&(pVdpReg->G0_VOFFSET.u32)+ u32Data * GFX_OFFSET), G0_VOFFSET.u32);
    }


}
//GFX END
#endif
//-------------------------------------------------------------------
//WBC_GFX_BEGIN
//-------------------------------------------------------------------
//
HI_VOID VDP_WBC_SetCmpEnable( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET), WBC_G0_CMP.u32);

    }

}
HI_VOID VDP_WBC_SetCmpDrr( VDP_LAYER_WBC_E enLayer, HI_U32 u32CmpDrr)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_drr = u32CmpDrr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_drr = u32CmpDrr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET), WBC_G0_CMP.u32);

    }

}
HI_VOID VDP_WBC_SetLossy( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_lossy_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_lossy_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32)+WBC_OFFSET), WBC_G0_CMP.u32);

    }


}




//-------------------------------------------------------------------
//WBC_DHD0_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_WBC_SetLowdlyEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    volatile U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("error,VDP_WBC_SetLowdlyEnable() select wrong video layer id\n");
        return ;
    }


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.lowdly_en =  u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }

}

HI_VOID VDP_WBC_SetLowdlyTestEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    volatile U_WBC_DHD0_LOWDLYCTRL  WBC_DHD0_LOWDLYCTRL;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("error,VDP_WBC_SetLowDlyTestEnable() select wrong video layer id\n");
        return ;
    }


    WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
    WBC_DHD0_LOWDLYCTRL.bits.lowdly_test = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
}

HI_VOID VDP_WBC_SetWbPerLineNum(VDP_LAYER_WBC_E enLayer,  HI_U32 u32Num)
{
    volatile U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("error,VDP_WBC_SetWbPerLineNum() select wrong video layer id\n");
        return ;
    }


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.wb_per_line_num =  u32Num - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }

}

HI_VOID VDP_WBC_SetPartfnsLineNum(VDP_LAYER_WBC_E enLayer,HI_U32 u32Num)
{
    volatile U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("error,VDP_WBC_SetPartfnsLineNum() select wrong video layer id\n");
        return ;
    }


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.partfns_line_num = u32Num - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }

}

HI_VOID VDP_WBC_SetTunlCellAddr(VDP_LAYER_WBC_E enLayer,  HI_U32 u32Addr)
{
    volatile U_WBC_DHD0_TUNLADDR     WBC_DHD0_TUNLADDR;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("error,VDP_WBC_SetTunlCellAddr() select wrong video layer id\n");
        return ;
    }


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_TUNLADDR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_TUNLADDR.u32)));
        WBC_DHD0_TUNLADDR.bits.tunl_cell_addr =  u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_TUNLADDR.u32)), WBC_DHD0_TUNLADDR.u32);
    }

}

HI_VOID VDP_WBC_HistogramEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    volatile U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.histogam_en =  u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }

}

HI_VOID VDP_WBC_VtthdMode(VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_vtthd_mode)
{
    volatile U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.wbc_vtthd_mode =  u32wbc_vtthd_mode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }

}
HI_VOID VDP_WBC_SetAutoSt( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;

    if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.auto_stop_en=  bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }

}
HI_VOID VDP_WBC_SetThreeMd( VDP_LAYER_WBC_E enLayer, HI_U32 bMode)
{
    volatile    U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    volatile    U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.three_d_mode =  bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.three_d_mode =  bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }

}
HI_VOID VDP_WBC_SetEnable( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    volatile U_WBC_G0_CTRL   WBC_G0_CTRL;


    if( enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.wbc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.wbc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);


    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.wbc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET));
        WBC_G0_CTRL.bits.wbc_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET), WBC_G0_CTRL.u32);

    }



}

HI_VOID VDP_WBC_SetOffSet( VDP_LAYER_WBC_E enLayer, HI_U32 u32Offset)
{

    volatile U_WBC_G0_OFFSET WBC_G0_OFFSET;
    if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_OFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)));
        WBC_G0_OFFSET.bits.wbcoffset = u32Offset;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)), WBC_G0_OFFSET.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_OFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)+WBC_OFFSET));
        WBC_G0_OFFSET.bits.wbcoffset = u32Offset;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)+WBC_OFFSET), WBC_G0_OFFSET.u32);

    }



}


HI_VOID VDP_WBC_SetOutIntf (VDP_LAYER_WBC_E enLayer, VDP_DATA_RMODE_E u32RdMode)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    if( enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.mode_out = u32RdMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
    else if( enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.mode_out = u32RdMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error! VDP_WBC_SetOutIntf enRdMode error!\n");
    }
}

HI_VOID  VDP_WBC_SetRegUp (VDP_LAYER_WBC_E enLayer)
{
    volatile U_WBC_DHD0_UPD WBC_DHD0_UPD;
    volatile U_WBC_GP0_UPD WBC_GP0_UPD;
    volatile U_WBC_G0_UPD WBC_G0_UPD;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_UPD.bits.regup = 0x1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_UPD.u32)), WBC_DHD0_UPD.u32);

        return ;
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_UPD.bits.regup = 0x1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_UPD.u32)), WBC_GP0_UPD.u32);

        return ;
    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_UPD.bits.regup = 0x1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_UPD.u32)), WBC_G0_UPD.u32);

        return ;
    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_UPD.bits.regup = 0x1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_UPD.u32)+WBC_OFFSET), WBC_G0_UPD.u32);

        return ;
    }





}

HI_VOID VDP_WBC_SetOutFmt(VDP_LAYER_WBC_E enLayer, VDP_WBC_OFMT_E stIntfFmt)

{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile U_WBC_GP0_CTRL   WBC_GP0_CTRL;
    volatile U_WBC_G0_CTRL   WBC_G0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.format_out = stIntfFmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.format_out = stIntfFmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.format_out = stIntfFmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);
    }

    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET));
        WBC_G0_CTRL.bits.format_out = stIntfFmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET), WBC_G0_CTRL.u32);
    }




}

HI_VOID VDP_WBC_SetOutFmtUVOrder(VDP_LAYER_WBC_E enLayer, HI_U32 uvOrder)
{
    U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    U_WBC_GP0_CTRL   WBC_GP0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.uv_order = uvOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.uv_order = uvOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
}

HI_VOID VDP_WBC_SetSpd(VDP_LAYER_WBC_E enLayer, HI_U32 u32ReqSpd)
{
    volatile U_WBC_DHD0_CTRL WBC_DHD0_CTRL;
    volatile U_WBC_GP0_CTRL WBC_GP0_CTRL;
    volatile U_WBC_G0_CTRL WBC_G0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.req_interval = u32ReqSpd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.req_interval = u32ReqSpd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.req_interval = u32ReqSpd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET));
        WBC_G0_CTRL.bits.req_interval = u32ReqSpd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CTRL.u32)+WBC_OFFSET), WBC_G0_CTRL.u32);
    }




}

HI_VOID  VDP_WBC_SetLayerAddr   (VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    volatile U_WBC_DHD0_STRIDE WBC_DHD0_STRIDE;
    volatile U_WBC_GP0_STRIDE WBC_GP0_STRIDE;
    volatile U_WBC_G0_STRIDE  WBC_G0_STRIDE;

    if (enLayer == VDP_LAYER_WBC_HD0)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_YADDR.u32)), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CADDR.u32)), u32CAddr);
        WBC_DHD0_STRIDE.bits.wbclstride = u32LStr;
        WBC_DHD0_STRIDE.bits.wbccstride = u32CStr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_STRIDE.u32)), WBC_DHD0_STRIDE.u32);

        return ;
    }

    else if (enLayer == VDP_LAYER_WBC_GP0)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_YADDR.u32)), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CADDR.u32)), u32CAddr);
        WBC_GP0_STRIDE.bits.wbclstride = u32LStr;
        WBC_GP0_STRIDE.bits.wbccstride = u32CStr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_STRIDE.u32)), WBC_GP0_STRIDE.u32);

        return ;
    }
    else if (enLayer == VDP_LAYER_WBC_G0)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_AR_ADDR.u32)), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_GB_ADDR.u32)), u32CAddr);
        WBC_G0_STRIDE.bits.wbcstride = u32LStr;
        //WBC_G0_STRIDE.bits.wbccstride = u32CStr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_STRIDE.u32)), WBC_G0_STRIDE.u32);

        return ;
    }
    else if (enLayer == VDP_LAYER_WBC_G4)
    {
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_AR_ADDR.u32)+WBC_OFFSET), u32LAddr);
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_GB_ADDR.u32)+WBC_OFFSET), u32CAddr);
        WBC_G0_STRIDE.bits.wbcstride = u32LStr;
        //WBC_G0_STRIDE.bits.wbccstride = u32CStr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_STRIDE.u32)+WBC_OFFSET), WBC_G0_STRIDE.u32);

        return ;
    }



}

HI_VOID  VDP_WBC_SetLayerReso     (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S  stRect)
{
    volatile U_WBC_DHD0_ORESO WBC_DHD0_ORESO;
   // volatile U_WBC_GP0_IRESO WBC_GP0_IRESO;//mask by h00226871
    volatile U_WBC_GP0_ORESO WBC_GP0_ORESO;
    volatile U_WBC_G0_ORESO  WBC_G0_ORESO;
    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32)));
        WBC_DHD0_ORESO.bits.ow = stRect.u32OWth - 1;
        WBC_DHD0_ORESO.bits.oh = stRect.u32OHgt - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32)), WBC_DHD0_ORESO.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
       // mask by h00226871 WBC_GP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_IRESO.u32)));
       // mask by h00226871 WBC_GP0_IRESO.bits.iw = stRect.u32IWth - 1;
       // mask by h00226871 WBC_GP0_IRESO.bits.ih = stRect.u32IHgt - 1;
       // mask by h00226871 VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_IRESO.u32)), WBC_GP0_IRESO.u32);

        WBC_GP0_ORESO.u32 = VDP_RegRead(   (HI_U32)(&(pVdpReg->WBC_GP0_ORESO.u32)));
        WBC_GP0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_ORESO.u32)));
        WBC_GP0_ORESO.bits.ow = stRect.u32OWth - 1;
        WBC_GP0_ORESO.bits.oh = stRect.u32OHgt - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_ORESO.u32)), WBC_GP0_ORESO.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.bits.ow = stRect.u32OWth - 1;
        WBC_G0_ORESO.bits.oh = stRect.u32OHgt - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)), WBC_G0_ORESO.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)+WBC_OFFSET));
        WBC_G0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)+WBC_OFFSET));
        WBC_G0_ORESO.bits.ow = stRect.u32OWth - 1;
        WBC_G0_ORESO.bits.oh = stRect.u32OHgt - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_ORESO.u32)+WBC_OFFSET), WBC_G0_ORESO.u32);
    }
}

HI_VOID VDP_WBC_SetDitherMode  (VDP_LAYER_WBC_E enLayer, VDP_DITHER_E enDitherMode)

{
    volatile U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
    volatile U_WBC_GP0_DITHER_CTRL WBC_GP0_DITHER_CTRL;


    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)));
        WBC_DHD0_DITHER_CTRL.bits.dither_md = enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)), WBC_DHD0_DITHER_CTRL.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {

        WBC_GP0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32)));
        WBC_GP0_DITHER_CTRL.bits.dither_md = enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32)), WBC_GP0_DITHER_CTRL.u32);

    }


}

HI_VOID VDP_WBC_SetDitherCoef  (VDP_LAYER_WBC_E enLayer, VDP_DITHER_COEF_S dither_coef)

{

    volatile U_WBC_DHD0_DITHER_COEF0 WBC_DHD0_DITHER_COEF0;
    volatile U_WBC_DHD0_DITHER_COEF1 WBC_DHD0_DITHER_COEF1;

    volatile U_WBC_GP0_DITHER_COEF0 WBC_GP0_DITHER_COEF0;
    volatile U_WBC_GP0_DITHER_COEF1 WBC_GP0_DITHER_COEF1;



    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_DHD0_DITHER_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF0.u32)));
        WBC_DHD0_DITHER_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF0.u32)), WBC_DHD0_DITHER_COEF0.u32);

        WBC_DHD0_DITHER_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF1.u32)));
        WBC_DHD0_DITHER_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF1.u32)), WBC_DHD0_DITHER_COEF1.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {

        WBC_GP0_DITHER_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_COEF0.u32)));
        WBC_GP0_DITHER_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        WBC_GP0_DITHER_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        WBC_GP0_DITHER_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        WBC_GP0_DITHER_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_COEF0.u32)), WBC_GP0_DITHER_COEF0.u32);

        WBC_GP0_DITHER_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_COEF1.u32)));
        WBC_GP0_DITHER_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        WBC_GP0_DITHER_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        WBC_GP0_DITHER_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        WBC_GP0_DITHER_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_COEF1.u32)), WBC_GP0_DITHER_COEF1.u32);

    }


}

HI_VOID  VDP_WBC_SetCropReso (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S stRect)
{
    volatile U_WBC_DHD0_FCROP WBC_DHD0_FCROP;
    volatile U_WBC_DHD0_LCROP WBC_DHD0_LCROP;

    volatile U_WBC_GP0_FCROP WBC_GP0_FCROP;
    volatile U_WBC_GP0_LCROP WBC_GP0_LCROP;
    volatile U_WBC_G0_FCROP WBC_G0_FCROP;
    volatile U_WBC_G0_LCROP WBC_G0_LCROP;


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32)));
        WBC_DHD0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_DHD0_FCROP.bits.hfcrop = stRect.u32DYS;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32)), WBC_DHD0_FCROP.u32);

        WBC_DHD0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32)));
        WBC_DHD0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_DHD0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32)), WBC_DHD0_LCROP.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_FCROP.u32)));
        WBC_GP0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_GP0_FCROP.bits.hfcrop = stRect.u32DYS;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_FCROP.u32)), WBC_GP0_FCROP.u32);

        WBC_GP0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_LCROP.u32)));
        WBC_GP0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_GP0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_LCROP.u32)), WBC_GP0_LCROP.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_G0)
    {
        WBC_G0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_FCROP.u32)));
        WBC_G0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_G0_FCROP.bits.hfcrop = stRect.u32DYS;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_FCROP.u32)), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_LCROP.u32)));
        WBC_G0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_G0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_LCROP.u32)), WBC_G0_LCROP.u32);


    }
    else if(enLayer == VDP_LAYER_WBC_G4)
    {
        WBC_G0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_FCROP.u32)+WBC_OFFSET));
        WBC_G0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_G0_FCROP.bits.hfcrop = stRect.u32DYS;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_FCROP.u32)+WBC_OFFSET), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_LCROP.u32)+WBC_OFFSET));
        WBC_G0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_G0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_LCROP.u32)+WBC_OFFSET), WBC_G0_LCROP.u32);
    }







    return ;
}
#if 0
HI_VOID VDP_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr,HI_U32 u32Addrchr)
{
    volatile U_WBC_DHD0_HLCOEFAD WBC_DHD0_HLCOEFAD;
    volatile U_WBC_DHD0_HCCOEFAD WBC_DHD0_HCCOEFAD;
    volatile U_WBC_DHD0_VLCOEFAD WBC_DHD0_VLCOEFAD;
    volatile U_WBC_DHD0_VCCOEFAD WBC_DHD0_VCCOEFAD;
    volatile U_GP0_HCOEFAD      GP0_HCOEFAD;
    volatile U_GP0_VCOEFAD      GP0_VCOEFAD;

    if( enLayer == VDP_LAYER_WBC_HD0)
    {
        if(u32Mode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_DHD0_HLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HLCOEFAD.u32)));
            WBC_DHD0_HLCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HLCOEFAD.u32)), WBC_DHD0_HLCOEFAD.u32);

            WBC_DHD0_HCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCCOEFAD.u32)));
            WBC_DHD0_HCCOEFAD.bits.coef_addr = u32Addrchr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCCOEFAD.u32)), WBC_DHD0_HCCOEFAD.u32);

        }
        else if(u32Mode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_DHD0_VLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_VLCOEFAD.u32)));
            WBC_DHD0_VLCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_VLCOEFAD.u32)), WBC_DHD0_VLCOEFAD.u32);

            WBC_DHD0_VCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_VCCOEFAD.u32)));
            WBC_DHD0_VCCOEFAD.bits.coef_addr = u32Addrchr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_VCCOEFAD.u32)), WBC_DHD0_VCCOEFAD.u32);

        }
        else
        {
            HI_PRINT("Error,VDP_WBC_DHD0_SetZmeCoefAddr() Select a Wrong Mode!\n");
        }

    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {

        if(u32Mode == VDP_WBC_PARA_ZME_HOR)
        {
            GP0_HCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_HCOEFAD.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_HCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_HCOEFAD.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_HCOEFAD.u32);
        }
        else if(u32Mode == VDP_WBC_PARA_ZME_VER)
        {
            GP0_VCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_VCOEFAD.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_VCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_VCOEFAD.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_VCOEFAD.u32);
        }
        else
        {
            HI_PRINT("Error,VDP_GP_SetZmeCoefAddr() Select a Wrong Mode!\n");
        }
    }
    return ;

}
#endif
#if 0
HI_VOID  VDP_WBC_SetParaUpd (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E enMode)
{
    volatile U_WBC_DHD0_PARAUP WBC_DHD0_PARAUP;
    volatile U_GP0_PARAUP GP0_PARAUP;

    WBC_DHD0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_PARAUP.u32)));
    GP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_PARAUP.u32) + WBC_GP0_SEL * GP_OFFSET));

    if( enLayer == VDP_LAYER_WBC_HD0)
    {

        if(enMode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_DHD0_PARAUP.bits.wbc_hlcoef_upd= 0x1;
            WBC_DHD0_PARAUP.bits.wbc_hccoef_upd= 0x1;
        }
        else if(enMode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_DHD0_PARAUP.bits.wbc_vlcoef_upd= 0x1;
            WBC_DHD0_PARAUP.bits.wbc_vccoef_upd= 0x1;
        }
        else
        {
            HI_PRINT("error,VDP_WBC_DHD0_SetParaUpd() select wrong mode!\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_PARAUP.u32)), WBC_DHD0_PARAUP.u32);
        return ;
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if(enMode == VDP_WBC_PARA_ZME_HOR)
        {
            GP0_PARAUP.bits.gp0_hcoef_upd = 0x1;
        }
        else if(enMode == VDP_WBC_PARA_ZME_VER)
        {
            GP0_PARAUP.bits.gp0_vcoef_upd = 0x1;
        }
        else
        {
            HI_PRINT("error,VDP_GP_SetParaUpd() select wrong mode!\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_PARAUP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_PARAUP.u32);
        return ;
    }
}
#endif

//HI_VOID  VDP_WBC_SetSfifo (VDP_LAYER_WBC_E enLayer, HI_U32 u32Data )
//{
//    volatile U_WBC_DHD0_SFIFO_CTRL WBC_DHD0_SFIFO_CTRL;
//    WBC_DHD0_SFIFO_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_SFIFO_CTRL.u32)));
//    if( enLayer == VDP_LAYER_WBC_HD0)
//    {
//    WBC_DHD0_SFIFO_CTRL.bits.sfifo_wr_thd = u32Data;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_SFIFO_CTRL.u32)), WBC_DHD0_SFIFO_CTRL.u32);
//
//    }
//
//}
#if 0
HI_VOID VDP_WBC_SetZmeEnable  (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable, HI_U32 u32firMode)
{
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    volatile U_GP0_ZME_HSP      GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP      GP0_ZME_VSP;

    /*WBC zoom enable */
    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hlfir_en = u32firMode ;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hchfir_en = u32firMode;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }

#if 0
        if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.non_lnr_en = u32bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }

#endif
        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vlfir_en = u32firMode;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vchfir_en = u32firMode;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }

    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hsc_en = u32bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vsc_en = u32bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }

        return ;
    }

    return ;

}
#endif
#if 0
HI_VOID VDP_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_WBC_DHD0_ZME_HLOFFSET WBC_DHD0_ZME_HLOFFSET;
    U_WBC_DHD0_ZME_HCOFFSET WBC_DHD0_ZME_HCOFFSET;


    if (enLayer == VDP_LAYER_WBC_HD0 ) {
        /* VHD layer zoom enable */
        WBC_DHD0_ZME_HLOFFSET.u32 = VDP_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32));
        WBC_DHD0_ZME_HLOFFSET.bits.hor_loffset = s32PhaseL;
        VDP_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HLOFFSET.u32), WBC_DHD0_ZME_HLOFFSET.u32);

        WBC_DHD0_ZME_HCOFFSET.u32 = VDP_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32));
        WBC_DHD0_ZME_HCOFFSET.bits.hor_coffset = s32PhaseC;
        VDP_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_HCOFFSET.u32), WBC_DHD0_ZME_HCOFFSET.u32);

    } else if(enLayer == VDP_LAYER_WBC_GP0){

    }
    return ;
}
#endif
#if 0
HI_VOID VDP_WBC_SetZmePhaseV(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_WBC_DHD0_ZME_VOFFSET WBC_DHD0_ZME_VOFFSET;

    if (enLayer == VDP_LAYER_WBC_HD0 ) {
        /* VHD layer zoom enable */
        WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32));
        WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32PhaseL;
        VDP_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32), WBC_DHD0_ZME_VOFFSET.u32);

        WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32));
        WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32PhaseC;
        VDP_RegWrite((HI_U32)(&pVdpReg->WBC_DHD0_ZME_VOFFSET.u32), WBC_DHD0_ZME_VOFFSET.u32);

    } else if(enLayer == VDP_LAYER_WBC_GP0){


    }

    return ;
}
#endif

#if 0
/* WBC set Median filter enable */
HI_VOID  VDP_WBC_SetMidEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_U32 bEnable)

{

    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    volatile U_GP0_ZME_HSP      GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP      GP0_ZME_VSP;
    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);

        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hamid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vamid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }
        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }
    }
    return;

}
#endif
HI_VOID VDP_WBC_SetFirEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable)
{
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    volatile U_GP0_ZME_HSP      GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP      GP0_ZME_VSP;

    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hlfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);

        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
            WBC_DHD0_ZME_HSP.bits.hchfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vlfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vchfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hafir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HSP.bits.hfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vafir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VSP.bits.vfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSP.u32);
        }
    }


    return ;

}

#if 0
HI_VOID VDP_WBC_SetZmeVerTap(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)

{
    return;
}
#endif
#if 0
HI_VOID VDP_WBC_SetZmeHfirOrder(VDP_LAYER_WBC_E enLayer, HI_U32 u32HfirOrder)
{
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_GP0_ZME_HSP      GP0_ZME_HSP;
    if(enLayer == VDP_LAYER_WBC_HD0 )
    {

        WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
        WBC_DHD0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
    }
    return ;
}
#endif
#if 0
HI_VOID VDP_WBC_SetZmeHorRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)

{
    volatile U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    volatile U_GP0_ZME_HSP      GP0_ZME_HSP;
    if(enLayer == VDP_LAYER_WBC_HD0 )
    {


        WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
        WBC_DHD0_ZME_HSP.bits.hratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_HSP.bits.hratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
    }
    return ;
}
#endif
HI_VOID  VDP_WBC_SetZmeInFmt_Define1(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
    volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    if(enLayer == VDP_LAYER_WBC_HD0 )
    {

        WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = u32Fmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID  VDP_WBC_SetZmeOutFmt(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
   volatile U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
        WBC_DHD0_ZME_VSP.bits.zme_out_fmt = u32Fmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
    }

    return ;
}
#if 0
HI_VOID  VDP_WBC_SetZmeVerRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    volatile U_WBC_DHD0_ZME_VSR WBC_DHD0_ZME_VSR;
    volatile U_GP0_ZME_VSR        GP0_ZME_VSR;


    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        WBC_DHD0_ZME_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32)));
        WBC_DHD0_ZME_VSR.bits.vratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32)), WBC_DHD0_ZME_VSR.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_VSR.bits.vratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSR.u32);
    }
    return ;
}

HI_VOID  VDP_WBC_SetZmePhase    (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    volatile U_WBC_DHD0_ZME_VOFFSET   WBC_DHD0_ZME_VOFFSET;
    volatile U_WBC_DHD0_ZME_VBOFFSET  WBC_DHD0_ZME_VBOFFSET;
    volatile U_GP0_ZME_HOFFSET        GP0_ZME_HOFFSET;
    volatile U_GP0_ZME_VOFFSET        GP0_ZME_VOFFSET;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)));
            WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)), WBC_DHD0_ZME_VOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)));
            WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32)), WBC_DHD0_ZME_VOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32)));
            WBC_DHD0_ZME_VBOFFSET.bits.vbluma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32)), WBC_DHD0_ZME_VBOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32)));
            WBC_DHD0_ZME_VBOFFSET.bits.vbchroma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32)), WBC_DHD0_ZME_VBOFFSET.u32);
        }
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HOFFSET.bits.hor_loffset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HOFFSET.bits.hor_coffset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VOFFSET.bits.vbtm_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_VOFFSET.bits.vtp_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VOFFSET.u32);
        }
    }

    return ;
}

#endif

HI_VOID  VDP_WBC_SetCscEnable  (VDP_LAYER_WBC_E enLayer, HI_U32 enCSC)
{
    volatile U_WBC_DHD0_CSCIDC WBC_DHD0_CSCIDC;
    volatile U_GP0_CSC_IDC     GP0_CSC_IDC;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_DHD0_CSCIDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)));
        WBC_DHD0_CSCIDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)), WBC_DHD0_CSCIDC.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {

        GP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_CSC_IDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_CSC_IDC.u32);

    }
    return ;
}

HI_VOID   VDP_WBC_SetCscDcCoef(VDP_LAYER_WBC_E enLayer,VDP_CSC_DC_COEF_S stCscCoef)
{
    volatile U_WBC_DHD0_CSCIDC WBC_DHD0_CSCIDC;
    volatile U_WBC_DHD0_CSCODC WBC_DHD0_CSCODC;
    volatile U_GP0_CSC_IDC  GP0_CSC_IDC;
    volatile U_GP0_CSC_ODC  GP0_CSC_ODC;
    volatile U_GP0_CSC_IODC GP0_CSC_IODC;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CSCIDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)));

        WBC_DHD0_CSCIDC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        WBC_DHD0_CSCIDC.bits.cscidc1 = stCscCoef.csc_in_dc1;
        WBC_DHD0_CSCIDC.bits.cscidc0 = stCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)), WBC_DHD0_CSCIDC.u32);

        WBC_DHD0_CSCODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCODC.u32)));
        WBC_DHD0_CSCODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        WBC_DHD0_CSCODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        WBC_DHD0_CSCODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCODC.u32)), WBC_DHD0_CSCODC.u32);
    }

    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_CSC_IDC.bits.cscidc1  = stCscCoef.csc_in_dc1;
        GP0_CSC_IDC.bits.cscidc0  = stCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_CSC_IDC.u32);

        GP0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_ODC.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_CSC_ODC.bits.cscodc1 = stCscCoef.csc_out_dc1;
        GP0_CSC_ODC.bits.cscodc0 = stCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_ODC.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_CSC_ODC.u32);

        GP0_CSC_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IODC.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_CSC_IODC.bits.cscodc2 = stCscCoef.csc_out_dc2;
        GP0_CSC_IODC.bits.cscidc2 = stCscCoef.csc_in_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IODC.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_CSC_IODC.u32);
    }
    return ;
}

HI_VOID   VDP_WBC_SetCscCoef(VDP_LAYER_WBC_E enLayer,VDP_CSC_COEF_S stCscCoef)
{
    volatile U_WBC_DHD0_CSCP0        WBC_DHD0_CSCP0;
    volatile U_WBC_DHD0_CSCP1        WBC_DHD0_CSCP1;
    volatile U_WBC_DHD0_CSCP2        WBC_DHD0_CSCP2;
    volatile U_WBC_DHD0_CSCP3        WBC_DHD0_CSCP3;
    volatile U_WBC_DHD0_CSCP4        WBC_DHD0_CSCP4;
    volatile U_GP0_CSC_P0            GP0_CSC_P0;
    volatile U_GP0_CSC_P1            GP0_CSC_P1;
    volatile U_GP0_CSC_P2            GP0_CSC_P2;
    volatile U_GP0_CSC_P3            GP0_CSC_P3;
    volatile U_GP0_CSC_P4            GP0_CSC_P4;


    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CSCP0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP0.u32)));
        WBC_DHD0_CSCP0.bits.cscp00 = stCscCoef.csc_coef00;
        WBC_DHD0_CSCP0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP0.u32)), WBC_DHD0_CSCP0.u32);

        WBC_DHD0_CSCP1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP1.u32)));
        WBC_DHD0_CSCP1.bits.cscp02 = stCscCoef.csc_coef02;
        WBC_DHD0_CSCP1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP1.u32)), WBC_DHD0_CSCP1.u32);

        WBC_DHD0_CSCP2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP2.u32)));
        WBC_DHD0_CSCP2.bits.cscp11 = stCscCoef.csc_coef11;
        WBC_DHD0_CSCP2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP2.u32)), WBC_DHD0_CSCP2.u32);

        WBC_DHD0_CSCP3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP3.u32)));
        WBC_DHD0_CSCP3.bits.cscp20 = stCscCoef.csc_coef20;
        WBC_DHD0_CSCP3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP3.u32)), WBC_DHD0_CSCP3.u32);

        WBC_DHD0_CSCP4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP4.u32)));
        WBC_DHD0_CSCP4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCP4.u32)), WBC_DHD0_CSCP4.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_CSC_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P0.u32)+WBC_GP0_SEL*GP_OFFSET));
        GP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
        GP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P0.u32)+WBC_GP0_SEL*GP_OFFSET), GP0_CSC_P0.u32);

        GP0_CSC_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P1.u32)+WBC_GP0_SEL*GP_OFFSET));
        GP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
        GP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P1.u32)+WBC_GP0_SEL*GP_OFFSET), GP0_CSC_P1.u32);

        GP0_CSC_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P2.u32)+WBC_GP0_SEL*GP_OFFSET));
        GP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
        GP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P2.u32)+WBC_GP0_SEL*GP_OFFSET), GP0_CSC_P2.u32);

        GP0_CSC_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P3.u32)+WBC_GP0_SEL*GP_OFFSET));
        GP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
        GP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P3.u32)+WBC_GP0_SEL*GP_OFFSET), GP0_CSC_P3.u32);

        GP0_CSC_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_P4.u32)+WBC_GP0_SEL*GP_OFFSET));
        GP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_P4.u32)+WBC_GP0_SEL*GP_OFFSET), GP0_CSC_P4.u32);
    }

}

HI_VOID VDP_WBC_SetCscMode( VDP_LAYER_WBC_E enLayer, VDP_CSC_MODE_E enCscMode)
{
    //csc coef solid mode
    //if(enLayer == VDP_LAYER_WBC_HD0)
    //{
    //    U_WBC_DHD0_CSCIDC WBC_DHD0_CSCIDC;

    //    WBC_DHD0_CSCIDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)));
    //    WBC_DHD0_CSCIDC.bits.csc_mode = enCscMode;
    //    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)), WBC_DHD0_CSCIDC.u32);
    //}
    //else if(enLayer == VDP_LAYER_WBC_GP0)
    {
#if 1
         //csc config mode
         volatile VDP_CSC_COEF_S    st_csc_coef;
         volatile VDP_CSC_DC_COEF_S st_csc_dc_coef;

         HI_S32 s32Pre   ;//= 1 << 10;
         HI_S32 s32DcPre ;//= 4;//1:8bit; 4:10bit

         if(enLayer == VDP_LAYER_WBC_HD0)
         {
             s32Pre   = 1 << 10;// 1 << 8;
             s32DcPre = 1;//1:8bit; 4:10bit
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
             VDP_WBC_SetCscCoef  ( enLayer,st_csc_coef);
             VDP_WBC_SetCscDcCoef( enLayer,st_csc_dc_coef);
         }
         else if(enLayer == VDP_LAYER_WBC_GP0)
         {
             s32Pre   = 1 << 10;
             s32DcPre = 4;//1:8bit; 4:10bit
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
             VDP_GP_SetCscCoef  (WBC_GP0_SEL,st_csc_coef);
             VDP_GP_SetCscDcCoef(WBC_GP0_SEL,st_csc_dc_coef);
         }
#endif
    }

    return ;
}

HI_VOID  VDP_VP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        volatile U_VP0_CSC0_IDC  VP0_CSC0_IDC;
        volatile U_VP0_CSC0_ODC  VP0_CSC0_ODC;
        volatile U_VP0_CSC0_IODC VP0_CSC0_IODC;

        VP0_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
        VP0_CSC0_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32);

        VP0_CSC0_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
        VP0_CSC0_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC0_ODC.u32);

        VP0_CSC0_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
        VP0_CSC0_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IODC.u32);
    }
    else if(u32Num == 1)
    {
        volatile U_VP0_CSC1_IDC  VP0_CSC1_IDC;
        volatile U_VP0_CSC1_ODC  VP0_CSC1_ODC;
        volatile U_VP0_CSC1_IODC VP0_CSC1_IODC;

        VP0_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
        VP0_CSC1_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32);

        VP0_CSC1_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
        VP0_CSC1_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC1_ODC.u32);

        VP0_CSC1_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
        VP0_CSC1_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IODC.u32);
    }

    return ;
}

HI_VOID   VDP_VP_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscCoef Select Wrong video ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        volatile U_VP0_CSC0_P0 VP0_CSC0_P0;
        volatile U_VP0_CSC0_P1 VP0_CSC0_P1;
        volatile U_VP0_CSC0_P2 VP0_CSC0_P2;
        volatile U_VP0_CSC0_P3 VP0_CSC0_P3;
        volatile U_VP0_CSC0_P4 VP0_CSC0_P4;

        VP0_CSC0_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P0.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P0.bits.cscp00 = stCscCoef.csc_coef00;
        VP0_CSC0_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P0.u32)+u32Data*VP_OFFSET), VP0_CSC0_P0.u32);

        VP0_CSC0_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P1.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P1.bits.cscp02 = stCscCoef.csc_coef02;
        VP0_CSC0_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P1.u32)+u32Data*VP_OFFSET), VP0_CSC0_P1.u32);

        VP0_CSC0_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P2.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P2.bits.cscp11 = stCscCoef.csc_coef11;
        VP0_CSC0_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P2.u32)+u32Data*VP_OFFSET), VP0_CSC0_P2.u32);

        VP0_CSC0_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P3.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P3.bits.cscp20 = stCscCoef.csc_coef20;
        VP0_CSC0_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P3.u32)+u32Data*VP_OFFSET), VP0_CSC0_P3.u32);

        VP0_CSC0_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_P4.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_P4.u32)+u32Data*VP_OFFSET), VP0_CSC0_P4.u32);
    }
    else if(u32Num == 1)
    {
        volatile U_VP0_CSC1_P0 VP0_CSC1_P0;
        volatile U_VP0_CSC1_P1 VP0_CSC1_P1;
        volatile U_VP0_CSC1_P2 VP0_CSC1_P2;
        volatile U_VP0_CSC1_P3 VP0_CSC1_P3;
        volatile U_VP0_CSC1_P4 VP0_CSC1_P4;

        VP0_CSC1_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P0.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P0.bits.cscp00 = stCscCoef.csc_coef00;
        VP0_CSC1_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P0.u32)+u32Data*VP_OFFSET), VP0_CSC1_P0.u32);

        VP0_CSC1_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P1.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P1.bits.cscp02 = stCscCoef.csc_coef02;
        VP0_CSC1_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P1.u32)+u32Data*VP_OFFSET), VP0_CSC1_P1.u32);

        VP0_CSC1_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P2.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P2.bits.cscp11 = stCscCoef.csc_coef11;
        VP0_CSC1_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P2.u32)+u32Data*VP_OFFSET), VP0_CSC1_P2.u32);

        VP0_CSC1_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P3.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P3.bits.cscp20 = stCscCoef.csc_coef20;
        VP0_CSC1_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P3.u32)+u32Data*VP_OFFSET), VP0_CSC1_P3.u32);

        VP0_CSC1_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_P4.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_P4.u32)+u32Data*VP_OFFSET), VP0_CSC1_P4.u32);
    }

}

HI_VOID  VDP_VP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        volatile U_VP0_CSC0_IDC VP0_CSC0_IDC;

        VP0_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32);
    }
    else if(u32Num == 1)
    {
        volatile U_VP0_CSC1_IDC VP0_CSC1_IDC;

        VP0_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode, HI_U32 u32Num)

{
    volatile VDP_CSC_COEF_S    st_csc_coef;
    volatile VDP_CSC_DC_COEF_S st_csc_dc_coef;

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

    VDP_VP_SetCscCoef  (u32Data,st_csc_coef, u32Num);
    VDP_VP_SetCscDcCoef(u32Data,st_csc_dc_coef, u32Num);

    return ;
}


//-------------------------------------------------------------------
//WBC_DHD0_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//ACM BEGING
//-------------------------------------------------------------------
HI_VOID VDP_VP_SetAcmEnable (HI_U32 u32Data, HI_U32 u32bAcmEn)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmEnable() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_en = u32bAcmEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;

}


HI_VOID VDP_VP_SetAcmDbgEn(HI_U32 u32Data , HI_U32 u32bAcmDbgEn)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmDbgEn() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_dbg_en = u32bAcmDbgEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmStretch(HI_U32 u32Data, HI_U32 u32AcmStrch)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmStretch() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_stretch = u32AcmStrch;
    //cout << "VP0_ACM_CTRL.bits.acm_stretch == " <<  VP0_ACM_CTRL.bits.acm_stretch << endl;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmClipRange(HI_U32 u32Data, HI_U32 u32AcmClipRange)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmClipRange() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cliprange = u32AcmClipRange;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}
HI_VOID VDP_VP_SetAcmCliporwrap(HI_U32 u32Data, HI_U32 u32AcmCliporwrap)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;
    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmCliporWrap() Select Wrong CHANNEL ID\n");
        return ;
    }
    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cliporwrap= u32AcmCliporwrap;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);
    return ;
}
HI_VOID VDP_VP_SetAcmChmThd(HI_U32 u32Data, HI_U32 u32AcmCbcrthr)
{
    volatile U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmChmThd() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cbcrthr = u32AcmCbcrthr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmGainLum(HI_U32 u32Data, HI_U32 u32AcmGain0)
{
    volatile U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmGainLum() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain0 = u32AcmGain0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmGainSat(HI_U32 u32Data, HI_U32 u32AcmGain2)
{
    volatile U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmGainSat() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain2 = u32AcmGain2;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmGainHue(HI_U32 u32Data, HI_U32 u32AcmGain1)
{
    volatile U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmGainHue() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain1 = u32AcmGain1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr)
{
    volatile U_VP0_ACM_CAD VP0_ACM_CAD;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmGainHue() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CAD.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CAD.bits.coef_addr = u32CoefAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CAD.u32) + u32Data * VP_OFFSET), VP0_ACM_CAD.u32);

    return ;
}
//-------------------------------------------------------------------
//ACM_END
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/*
//ABC_BEGIN
//-------------------------------------------------------------------
HI_VOID  VDP_VP_SetAbcEnable      (HI_U32 u32Data, HI_U32 u32Enable)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CTRL.bits.abc_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);

    return ;
}


HI_VOID VDP_VP_SetAbcDbgEn(HI_U32 u32hd_id ,  HI_U32 u32bAbcDbgEn,HI_U32 u32data,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcDbgEn() Select Wrong CHANNEL ID\n");
          return ;
    }

    VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CTRL.bits.abc_dbg_en = u32bAbcDbgEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);

    return ;
}


HI_VOID VDP_VP_SetAbcUpdEn(HI_U32 u32hd_id , VDP_ABC_UNIT_E enUnit, HI_U32 u32bAbcUpdEn,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcUpdEn() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_update_en1 = u32bAbcUpdEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_update_en2 = u32bAbcUpdEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}


HI_VOID VDP_VP_SetAbcAutoMan(HI_U32 u32hd_id , VDP_ABC_UNIT_E enUnit, VDP_ABC_AUTOMAN_E enAutoMan,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcUpdEn() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_auto_man1 = enAutoMan;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_auto_man2 = enAutoMan;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}


HI_VOID VDP_VP_SetAbcReveEn(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bReveEn,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcReveEn() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_reve_en1 = u32bReveEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_reve_en2 = u32bReveEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetAbcNegaEn(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32bNegaEn,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VVDP_VP_SetAbcNegaEn() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_nega_en1 = u32bNegaEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_nega_en2 = u32bNegaEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetAbcInputAdjEn(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32InputAdj,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcInputAdjEn() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_input_adj1 = u32InputAdj;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_input_adj2 = u32InputAdj;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}


HI_VOID VDP_VP_SetAbcShiftCtrl(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_SHIFT_E enShiftCtrl,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcShiftCtrl() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_shift_ctrl1 = enShiftCtrl;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_shift_ctrl2 = enShiftCtrl;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}


HI_VOID VDP_VP_SetAbcAdjWrite(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_S32 s32AdjWrite,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcAdjWrite() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_adj_write1 = s32AdjWrite;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
        VP0_ABC_CTRL.bits.abc_adj_write2 = s32AdjWrite;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);
    }

    return ;
}


HI_VOID VDP_VP_SetAbcRangeMode(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32range,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcRangeMode() Select Wrong CHANNEL ID\n");
          return ;
    }

    VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CTRL.bits.abc_range_mode = u32range;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcChromaEn(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, HI_U32 u32chromaEn,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CTRL VP0_ABC_CTRL;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcChromaEn() Select Wrong CHANNEL ID\n");
          return ;
    }

    VP0_ABC_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CTRL.bits.abc_chroma_en = u32chromaEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CTRL.u32) + u32Data * VP_OFFSET), VP0_ABC_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcHistThd(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_HIST_THD_S stHistThd,HI_U32 u32Data)
{
    volatile U_VP0_ABC_THRE1 VP0_ABC_THRE1;
    volatile U_VP0_ABC_THRE2 VP0_ABC_THRE2;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcHistThd() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_THRE1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_THRE1.u32) + u32Data * VP_OFFSET));
        VP0_ABC_THRE1.bits.abc_brig_thre1 = stHistThd.abc_hist_brig_thd;
        VP0_ABC_THRE1.bits.abc_dark_thre1 = stHistThd.abc_hist_dark_thd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_THRE1.u32) + u32Data * VP_OFFSET), VP0_ABC_THRE1.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_THRE2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_THRE2.u32) + u32Data * VP_OFFSET));
        VP0_ABC_THRE2.bits.abc_brig_thre2 = stHistThd.abc_hist_brig_thd;
        VP0_ABC_THRE2.bits.abc_dark_thre2 = stHistThd.abc_hist_dark_thd;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_THRE2.u32) + u32Data * VP_OFFSET), VP0_ABC_THRE2.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetAbcThd(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_THD_S stThd,HI_U32 u32Data)
{
    volatile U_VP0_ABC_BRIGTHRE1  VP0_ABC_BRIGTHRE1;
    volatile U_VP0_ABC_DARKTHRE1  VP0_ABC_DARKTHRE1;

    volatile U_VP0_ABC_BRIGTHRE2  VP0_ABC_BRIGTHRE2;
    volatile U_VP0_ABC_DARKTHRE2  VP0_ABC_DARKTHRE2;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcThd() Select Wrong CHANNEL ID\n");
        return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_BRIGTHRE1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE1.u32) + u32Data * VP_OFFSET));
        VP0_ABC_BRIGTHRE1.bits.abc_brig_low1  = stThd.abc_brig_thd_low;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE1.u32) + u32Data * VP_OFFSET), VP0_ABC_BRIGTHRE1.u32);

        VP0_ABC_BRIGTHRE1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE1.u32) + u32Data * VP_OFFSET));
        VP0_ABC_BRIGTHRE1.bits.abc_brig_high1 = stThd.abc_brig_thd_high;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE1.u32) + u32Data * VP_OFFSET), VP0_ABC_BRIGTHRE1.u32);

        VP0_ABC_DARKTHRE1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE1.u32) + u32Data * VP_OFFSET));
        VP0_ABC_DARKTHRE1.bits.abc_dark_high1 = stThd.abc_dark_thd_high;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE1.u32) + u32Data * VP_OFFSET), VP0_ABC_DARKTHRE1.u32);

        VP0_ABC_DARKTHRE1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE1.u32) + u32Data * VP_OFFSET));
        VP0_ABC_DARKTHRE1.bits.abc_dark_low1  = stThd.abc_dark_thd_low;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE1.u32) + u32Data * VP_OFFSET), VP0_ABC_DARKTHRE1.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_BRIGTHRE2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE2.u32) + u32Data * VP_OFFSET));
        VP0_ABC_BRIGTHRE2.bits.abc_brig_low2  = stThd.abc_brig_thd_low;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE2.u32) + u32Data * VP_OFFSET), VP0_ABC_BRIGTHRE2.u32);

        VP0_ABC_BRIGTHRE2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE2.u32) + u32Data * VP_OFFSET));
        VP0_ABC_BRIGTHRE2.bits.abc_brig_high2 = stThd.abc_brig_thd_high;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_BRIGTHRE2.u32) + u32Data * VP_OFFSET), VP0_ABC_BRIGTHRE2.u32);

        VP0_ABC_DARKTHRE2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE2.u32) + u32Data * VP_OFFSET));
        VP0_ABC_DARKTHRE2.bits.abc_dark_high2 = stThd.abc_dark_thd_high;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE2.u32) + u32Data * VP_OFFSET), VP0_ABC_DARKTHRE2.u32);

        VP0_ABC_DARKTHRE2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE2.u32) + u32Data * VP_OFFSET));
        VP0_ABC_DARKTHRE2.bits.abc_dark_low2  = stThd.abc_dark_thd_low;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_DARKTHRE2.u32) + u32Data * VP_OFFSET), VP0_ABC_DARKTHRE2.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetAbcParaAdj(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_ADJ_S stParaAdj,HI_U32 u32Data)
{
    volatile U_VP0_ABC_PARAADJ  VP0_ABC_PARAADJ;

    if(u32hd_id >= CHN_MAX)
    {
          HI_PRINT("Error,VDP_VP_SetAbcParaAdj() Select Wrong CHANNEL ID\n");
          return ;
    }
    else if(enUnit == VDP_ABC_UNIT_LUM)
    {
        VP0_ABC_PARAADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_PARAADJ.u32) + u32Data * VP_OFFSET));
        VP0_ABC_PARAADJ.bits.abc_adj_dn1 = stParaAdj.abc_adj_dn;
        VP0_ABC_PARAADJ.bits.abc_adj_up1 = stParaAdj.abc_adj_up;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_PARAADJ.u32) + u32Data * VP_OFFSET), VP0_ABC_PARAADJ.u32);
    }
    else if(enUnit == VDP_ABC_UNIT_CONTRAST)
    {
        VP0_ABC_PARAADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_PARAADJ.u32) + u32Data * VP_OFFSET));
        VP0_ABC_PARAADJ.bits.abc_adj_dn2 = stParaAdj.abc_adj_dn;
        VP0_ABC_PARAADJ.bits.abc_adj_up2 = stParaAdj.abc_adj_up;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_PARAADJ.u32) + u32Data * VP_OFFSET), VP0_ABC_PARAADJ.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetAbcCoefAddr(HI_U32 u32hd_id, HI_U32 u32CoefAddr,HI_U32 u32Data)
{
    volatile U_VP0_AB_CCAD  VP0_AB_CCAD;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcCoefAddr() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_AB_CCAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_AB_CCAD.u32) + u32Data * VP_OFFSET));
    VP0_AB_CCAD.bits.coef_addr = u32CoefAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_AB_CCAD.u32) + u32Data * VP_OFFSET), VP0_AB_CCAD.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcMixEn(HI_U32 u32hd_id, HI_U32 u32mixEn,HI_U32 u32Data)
{
    volatile U_VP0_ABC_HBWCOEF VP0_ABC_HBWCOEF;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcMixEn() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_HBWCOEF.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_HBWCOEF.u32) + u32Data * VP_OFFSET));
    VP0_ABC_HBWCOEF.bits.abc_mix_en = u32mixEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_HBWCOEF.u32) + u32Data * VP_OFFSET), VP0_ABC_HBWCOEF.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcHbwCoef(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data)
{
    volatile U_VP0_ABC_HBWCOEF VP0_ABC_HBWCOEF;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcHbwCoef() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_HBWCOEF.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_HBWCOEF.u32) + u32Data * VP_OFFSET));
    VP0_ABC_HBWCOEF.bits.abc_cr_coef = stAbcPara.s32CbCrSta_Cr ;
    VP0_ABC_HBWCOEF.bits.abc_cb_coef = stAbcPara.s32CbCrSta_Cb ;
    VP0_ABC_HBWCOEF.bits.abc_y_coef  = stAbcPara.s32CbCrSta_Y  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_HBWCOEF.u32) + u32Data * VP_OFFSET), VP0_ABC_HBWCOEF.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcHbwOffset(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data)
{
    volatile U_VP0_ABC_HBWOFFSET  VP0_ABC_HBWOFFSET ;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcHbwOffset() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_HBWOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_HBWOFFSET.u32) + u32Data * VP_OFFSET));
    VP0_ABC_HBWOFFSET.bits.abc_offset = stAbcPara.s32CbCrSta_Offset;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_HBWOFFSET.u32) + u32Data * VP_OFFSET), VP0_ABC_HBWOFFSET.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcLumaThro(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data)
{
    volatile U_VP0_ABC_LUMATHR0 VP0_ABC_LUMATHR0;
    volatile U_VP0_ABC_LUMATHR1 VP0_ABC_LUMATHR1;
    volatile U_VP0_ABC_LUMATHR2 VP0_ABC_LUMATHR2;
    volatile U_VP0_ABC_LUMATHR3 VP0_ABC_LUMATHR3;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcLumaThro() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_LUMATHR0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR0.u32) + u32Data * VP_OFFSET));
    VP0_ABC_LUMATHR1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR1.u32) + u32Data * VP_OFFSET));
    VP0_ABC_LUMATHR2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR2.u32) + u32Data * VP_OFFSET));
    VP0_ABC_LUMATHR3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR3.u32) + u32Data * VP_OFFSET));

    VP0_ABC_LUMATHR0.bits.abc_gain1_y0 = stAbcPara.s32CbCrGain1_Y_THR[0];
    VP0_ABC_LUMATHR0.bits.abc_gain1_y1 = stAbcPara.s32CbCrGain1_Y_THR[1];
    VP0_ABC_LUMATHR0.bits.abc_gain1_y2 = stAbcPara.s32CbCrGain1_Y_THR[2];
    VP0_ABC_LUMATHR0.bits.abc_gain1_y3 = stAbcPara.s32CbCrGain1_Y_THR[3];
    VP0_ABC_LUMATHR1.bits.abc_gain1_y4 = stAbcPara.s32CbCrGain1_Y_THR[4];
    VP0_ABC_LUMATHR1.bits.abc_gain1_y5 = stAbcPara.s32CbCrGain1_Y_THR[5];
    VP0_ABC_LUMATHR1.bits.abc_gain1_y6 = stAbcPara.s32CbCrGain1_Y_THR[6];
    VP0_ABC_LUMATHR1.bits.abc_gain1_y7 = stAbcPara.s32CbCrGain1_Y_THR[7];

    VP0_ABC_LUMATHR2.bits.abc_gain2_y0 = stAbcPara.s32CbCrGain2_Y_THR[0];
    VP0_ABC_LUMATHR2.bits.abc_gain2_y1 = stAbcPara.s32CbCrGain2_Y_THR[1];
    VP0_ABC_LUMATHR2.bits.abc_gain2_y2 = stAbcPara.s32CbCrGain2_Y_THR[2];
    VP0_ABC_LUMATHR2.bits.abc_gain2_y3 = stAbcPara.s32CbCrGain2_Y_THR[3];
    VP0_ABC_LUMATHR3.bits.abc_gain2_y4 = stAbcPara.s32CbCrGain2_Y_THR[4];
    VP0_ABC_LUMATHR3.bits.abc_gain2_y5 = stAbcPara.s32CbCrGain2_Y_THR[5];
    VP0_ABC_LUMATHR3.bits.abc_gain2_y6 = stAbcPara.s32CbCrGain2_Y_THR[6];
    VP0_ABC_LUMATHR3.bits.abc_gain2_y7 = stAbcPara.s32CbCrGain2_Y_THR[7];

    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR0.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMATHR0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR1.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMATHR1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR2.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMATHR2.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMATHR3.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMATHR3.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcLumaCoef(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data)
{
    volatile U_VP0_ABC_LUMACOEF0 VP0_ABC_LUMACOEF0;
    volatile U_VP0_ABC_LUMACOEF1 VP0_ABC_LUMACOEF1;
    volatile U_VP0_ABC_LUMACOEF2 VP0_ABC_LUMACOEF2;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcLumaThro() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_LUMACOEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF0.u32) + u32Data * VP_OFFSET));
    VP0_ABC_LUMACOEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF1.u32) + u32Data * VP_OFFSET));
    VP0_ABC_LUMACOEF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF2.u32) + u32Data * VP_OFFSET));

    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj0 = stAbcPara.s32CbCrGain1[0];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj1 = stAbcPara.s32CbCrGain1[1];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj2 = stAbcPara.s32CbCrGain1[2];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj3 = stAbcPara.s32CbCrGain1[3];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj4 = stAbcPara.s32CbCrGain1[4];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj5 = stAbcPara.s32CbCrGain1[5];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj6 = stAbcPara.s32CbCrGain1[6];
    VP0_ABC_LUMACOEF0.bits.abc_gain1_adj7 = stAbcPara.s32CbCrGain1[7];

    VP0_ABC_LUMACOEF1.bits.abc_gain1_adj8 = stAbcPara.s32CbCrGain1[8];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj0 = stAbcPara.s32CbCrGain2[0];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj1 = stAbcPara.s32CbCrGain2[1];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj2 = stAbcPara.s32CbCrGain2[2];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj3 = stAbcPara.s32CbCrGain2[3];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj4 = stAbcPara.s32CbCrGain2[4];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj5 = stAbcPara.s32CbCrGain2[5];
    VP0_ABC_LUMACOEF1.bits.abc_gain2_adj6 = stAbcPara.s32CbCrGain2[6];

    VP0_ABC_LUMACOEF2.bits.abc_gain2_adj7 = stAbcPara.s32CbCrGain2[7];
    VP0_ABC_LUMACOEF2.bits.abc_gain2_adj8 = stAbcPara.s32CbCrGain2[8];


    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF0.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMACOEF0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF1.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMACOEF1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_LUMACOEF2.u32) + u32Data * VP_OFFSET), VP0_ABC_LUMACOEF2.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcClip(HI_U32 u32hd_id, VDP_ABC_UNIT_E enUnit, VDP_ABC_PARA_S stAbcPara,HI_U32 u32Data)
{
    volatile U_VP0_ABC_CLIP       VP0_ABC_CLIP;
    volatile U_VP0_ABC_CLIPNEG    VP0_ABC_CLIPNEG;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcClip() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ABC_CLIP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CLIP.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CLIP.bits.abc_clip_range = stAbcPara.s32ClipRange               ;
    VP0_ABC_CLIP.bits.abc_clip_high  = stAbcPara.s32CorrParam_ClipHigh_pos  ;
    VP0_ABC_CLIP.bits.abc_clip_low   = stAbcPara.s32CorrParam_ClipLow_pos   ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CLIP.u32) + u32Data * VP_OFFSET), VP0_ABC_CLIP.u32);

    VP0_ABC_CLIPNEG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ABC_CLIPNEG.u32) + u32Data * VP_OFFSET));
    VP0_ABC_CLIPNEG.bits.abc_clip_highneg  = stAbcPara.s32CorrParam_ClipHigh_neg  ;
    VP0_ABC_CLIPNEG.bits.abc_clip_lowneg   = stAbcPara.s32CorrParam_ClipLow_neg  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ABC_CLIPNEG.u32) + u32Data * VP_OFFSET), VP0_ABC_CLIPNEG.u32);

    return ;
}

HI_VOID VDP_VP_SetAbcVpRegUp(HI_U32 u32hd_id, HI_U32 u32VpRegUpEn,HI_U32 u32Data)
{
    volatile U_VP0_UPD VP0_UPD ;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAbcVpRegUp() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_UPD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_UPD.u32) + u32Data * VP_OFFSET));
    VP0_UPD.bits.regup = u32VpRegUpEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_UPD.u32) + u32Data * VP_OFFSET), VP0_UPD.u32);

    return ;
}

//-------------------------------------------------------------------
//ABC_END
//-------------------------------------------------------------------
*/
#endif
#ifndef __DISP_PLATFORM_BOOT__
HI_VOID VDP_HDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetLine(HI_U32 u32Data)
{
    volatile U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_DACDET1.u32)));
    HDATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_DACDET1.u32)), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    volatile U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_DACDET1.u32)));
    HDATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_DACDET1.u32)), HDATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_HDATE_SetVdacDetStart(HI_U32 u32Data)
{
    volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

//SDATE
HI_VOID VDP_SDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    volatile U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)));
    DATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)), DATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetLine(HI_U32 u32Data)
{
    volatile U_DATE_DACDET1 DATE_DACDET1 ;

    DATE_DACDET1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_DACDET1.u32)));
    DATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_DACDET1.u32)), DATE_DACDET1.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    volatile U_DATE_DACDET1 DATE_DACDET1 ;

    DATE_DACDET1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_DACDET1.u32)));
    DATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_DACDET1.u32)), DATE_DACDET1.u32);
    return ;
}

HI_VOID VDP_SDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    volatile U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)));
    DATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)), DATE_DACDET2.u32);

    return ;
}

HI_VOID VDP_SDATE_SetVdacDetStart(HI_U32 u32Data)
{
    volatile U_DATE_DACDET2 DATE_DACDET2 ;

    DATE_DACDET2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)));
    DATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_DACDET2.u32)), DATE_DACDET2.u32);

    return ;
}


HI_VOID VDP_WBC_SetDhdLocate(HI_U32 u32Id, VDP_WBC_LOCATE_E   u32Data)

{
    volatile U_WBC_DHD_LOCATE  WBC_DHD_LOCATE;
    if(u32Id>= CHN_MAX)
    {
        HI_PRINT("error,VDP_WBC_SetDhdLocate() select wrong video layer id\n");
        return ;
    }



    WBC_DHD_LOCATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)));
    WBC_DHD_LOCATE.bits.wbc_dhd_locate= (HI_U32) u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)), WBC_DHD_LOCATE.u32);
}

HI_VOID VDP_WBC_SetDhdOflEn(HI_U32 u32Id, HI_U32  u32Data)
{
    volatile U_WBC_OFL_EN  WBC_OFL_EN;
    if(u32Id>= CHN_MAX)
    {
        HI_PRINT("error,VDP_WBC_SetDhdLocate() select wrong video layer id\n");
        return ;
    }



    WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
    WBC_OFL_EN.bits.wbc_ofl_en= u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
}

HI_VOID VDP_SetCheckSumEnable(HI_U32  bEnable)
{
    U_VOCTRL  VOCTRL;
    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.chk_sum_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}

HI_VOID VDP_SetLayerConnect(HI_U32 u32Data)
{
    volatile U_VOCTRL VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));

    if(u32Data == VDP_CONNECT_G3_DHD0)
        VOCTRL.bits.g3_dhd1_sel = 0;
    else if(u32Data == VDP_CONNECT_G3_DHD1)
        VOCTRL.bits.g3_dhd1_sel = 1;
    else
        HI_PRINT("error,VDP_SetLayerConnect() select wrong connect\n");

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);
}
#endif

HI_VOID VDP_DHD_DEFAULT( )
{
   volatile U_DHD0_CTRL DHD0_CTRL;
   volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
   volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;
#endif

    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)), DHD0_SYNC_INV.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);


#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32)), DHD0_DATA_SEL.u32);
#endif
}

HI_VOID VDP_DSD_DEFAULT( )
{
   volatile U_DHD0_CTRL DHD0_CTRL;
   volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
   volatile U_VO_MUX VO_MUX;

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
   volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;
#endif

    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;
    VO_MUX.u32= 0x1;


    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+ 1*CHN_OFFSET), DHD0_SYNC_INV.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+ 1*CHN_OFFSET), DHD0_CTRL.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX.u32)), VO_MUX.u32);

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32) + 1*CHN_OFFSET), DHD0_DATA_SEL.u32);
#endif

}

#ifndef __DISP_PLATFORM_BOOT__
HI_VOID VDP_DHD1_DEFAULT()
{
   volatile U_DHD0_CTRL DHD1_CTRL;
   volatile U_DHD0_SYNC_INV DHD1_SYNC_INV;


    DHD1_SYNC_INV.u32 = 0x6000;
    DHD1_CTRL.u32 = 0x00000001;//0x8ad20011;

//    VDP_RegWrite((&(pVdpReg->DHD1_SYNC_INV.u32)), DHD1_SYNC_INV.u32);
//    VDP_RegWrite((&(pVdpReg->DHD1_CTRL.u32)), DHD1_CTRL.u32);

}


HI_VOID VDP_HDATE_DEFAULT(VDP_DISP_DIGFMT_E enDigFmt)
{

   volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
   volatile U_HDATE_POLA_CTRL HDATE_POLA_CTRL;
   volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;


    if(enDigFmt == VDP_DISP_DIGFMT_1080P30)
    {
        HDATE_POLA_CTRL.u32 = 0x3;
    HDATE_VIDEO_FORMAT.u32 = 0x000000a3;
    HDATE_OUT_CTRL.u32 = 0x000001b0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_POLA_CTRL.u32)), HDATE_POLA_CTRL.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);
    }
    else
    {
        if(enDigFmt == VDP_DISP_DIGFMT_720P)
            HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
        else
            HDATE_VIDEO_FORMAT.u32 = 0x000000a4;

        VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
    }

}

HI_VOID VDP_SDATE_DEFAULT(VDP_DISP_DIGFMT_E enDigFmt)
{
    VDP_DISP_CLIP_S ClipData;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), 0x628412dc);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF21.u32)), 0x111111);

    ClipData.bClipEn = 1;
    ClipData.u32ClipLow_y = 0x40;
    ClipData.u32ClipLow_cb = 0x40;
    ClipData.u32ClipLow_cr = 0x40;
    ClipData.u32ClipHigh_y = 0x3ac;
    ClipData.u32ClipHigh_cb= 0x3c0;
    ClipData.u32ClipHigh_cr= 0x3c0;

    VDP_DISP_SetClipCoef (VDP_CHN_DHD1, VDP_DISP_INTF_DATE, ClipData);


}

HI_VOID VDP_HDATE_MODE(HI_U32 inft_mode)
{

  volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(inft_mode == VDP_DISP_DIGFMT_720P)
        HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
    else if(inft_mode == VDP_DISP_DIGFMT_1080I)
         HDATE_VIDEO_FORMAT.u32 = 0x000000a4;

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

}

HI_VOID VDP_INTF_DEFAULT()
{

  volatile U_VO_MUX VO_MUX;

    VO_MUX.u32 = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX.u32)), VO_MUX.u32);
    //VO_MUX_DAC.u32 = 0x654;
    //VDP_RegWrite((&(pVdpReg->VO_MUX_DAC.u32)), VO_MUX_DAC.u32);

    VDP_RegWrite((HI_U32)((&(pVdpReg->VOCTRL.u32)) + 0xb440/4), 0x1c);

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOMSKINTSTA.u32)), 0xffffffff);

    VDP_SetRdOutStd(VDP_MASTER0, 0, 4);

    VDP_SetWrOutStd(VDP_MASTER0, 0, 3);

}

HI_VOID VDP_VGA_DEFAULT()
{
  volatile U_VO_MUX_DAC VO_MUX_DAC;
  volatile U_VO_MUX VO_MUX;

    VO_MUX.u32 = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX.u32)), VO_MUX.u32);
    VO_MUX_DAC.u32 = 0x9870;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX_DAC.u32)), VO_MUX_DAC.u32);

    VDP_RegWrite((HI_U32)((&(pVdpReg->VOCTRL.u32)) + 0xb440/4), 0x1c);

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOMSKINTSTA.u32)), 0xffffffff);

    VDP_SetRdOutStd(VDP_MASTER0, 0, 4);

    VDP_SetWrOutStd(VDP_MASTER0, 0, 3);

}

HI_VOID VDP_VID_ZME_DEFAULT()
{
   volatile U_V0_VSP V0_VSP;
    V0_VSP.u32 = 0xc0080000;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32)), V0_VSP.u32);
}

HI_VOID VDP_DHD_DEBUG_DEFAULT(HI_U32 width)
{
   volatile U_DHD0_CTRL DHD0_CTRL;
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)));
    DHD0_CTRL.bits.fpga_lmt_en = 0x1;
    DHD0_CTRL.bits.fpga_lmt_width = width /16;
    //DHD0_CTRL.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);
}


//#define   RegWrite  VDP_RegWriteEx
//#define   RegRead   VDP_RegReadEx

#if 0
HI_VOID vou_RegRwTest(HI_U32 mask[], HI_U32 size, HI_U32 *testaddr, HI_U32 testdata)
{
    HI_U32 regsize = 0;
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 counter = 0;

    HI_U32 rdata = 0;
    HI_U32 wdata = 0;
    HI_U32 cdata = 0;

    /* read register test array */
    regsize = size;

   HI_PRINT("regsize = %d\n",regsize);
    for(i=0;i<regsize;i++)
    {
        if(mask[i] != 0x12345678)
        {
            wdata = testdata;
            RegWrite(testaddr[i],wdata);
       //     HI_PRINT("********))))))addr = %x\n",(HI_U32*)(testaddr + i*4));
        }

    }

    HI_PRINT ("the register has been writen now !\n\n" );
    i = 0;

    msleep(1);

    for(j=0;j<regsize;j++)
    {
        if(mask[j] == 0x12345678)
        {
            cdata = 0x0;
            rdata  = 0x0;
        }
        else if(mask[j] == 0x0)
        {
            cdata = 0x0;
       //     rdata = RegRead(testaddr[j]); //because in DUT, reserved bit is writeable
            rdata = 0x0;

        }
        else
        {
            cdata = mask[j] & testdata;
            rdata = RegRead(testaddr[j])& mask[j]; //because in DUT, reserved bit is writeable
        }

        if (rdata == cdata)
        {
        //    HI_PRINT ("Right! Register Address %x is right! :)\n", (testaddr[j]));
        //    HI_PRINT ("Register Data is 0x %x\n", rdata);
        }
        else
        {
            counter ++ ;
        //    HI_PRINT ("Error! Register Address  %x is wrong! :(\n", (testaddr[j]));
        //    HI_PRINT ("the right Data is %x\n", cdata);
        //    HI_PRINT ("the wrong Data is %x\n", rdata);

        }
    }

    if(counter == 0)
    {
        HI_PRINT ("Right! All the register are right!\n");
    //    cout << "Right! All the register are right!" << endl;
    }
    else
    {
        HI_PRINT ("Error! We find %d wrong register. Please check it!\n\n",counter);
    }

    // clear register
    for(i=0;i<regsize;i++)
    {
        if(mask[i] != 0x12345678)
        {
            wdata =0;
            RegWrite(testaddr[i],wdata);
        }

    }

    return;
}

HI_VOID vou_RegRTest(HI_U32 mask[], HI_U32 size, HI_U32 *testaddr, HI_U32 regdef[])
{
    HI_U32 regsize = 0;
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 counter = 0;

    HI_U32 rdata = 0;
    HI_U32 wdata = 0;
    HI_U32 cdata = 0;

    /* read register test array */
    regsize = size;

   HI_PRINT("regsize = %d\n",regsize);

    for(j=0;j<regsize;j++)
    {
        cdata = regdef[j];
        rdata = RegRead(testaddr[j]);

        if (rdata == cdata)
        {
        //    HI_PRINT ("Right! Register Address %x is right! :)\n", (testaddr[j]));
        //    HI_PRINT ("Register Data is 0x %x\n", rdata);
        }
        else
        {
            counter ++ ;
            HI_PRINT ("Error! Register Address  %x is wrong! :(\n", (testaddr[j]));
            HI_PRINT ("the Reg def Data is %x\n", cdata);
            HI_PRINT ("the Actual reg Data is %x\n", rdata);

        }
    }

    if(counter == 0)
    {
        HI_PRINT ("Right! All the register are right!\n");
    //    cout << "Right! All the register are right!" << endl;
    }
    else
    {
        HI_PRINT ("Error! We find %d wrong register. Please check it!\n\n",counter);
    }

    return;
}
#endif
#endif



HI_VOID VDP_MIXV_SetPrio(HI_U32 prioLayer)
{
    U_MIXV0_MIX          MIXV0_MIX;
    MIXV0_MIX.u32 = VDP_RegRead((HI_U32)&(pVdpReg->MIXV0_MIX.u32));
    if(prioLayer == VDP_LAYER_VID0)
    {
        MIXV0_MIX.bits.mixer_prio0= 0x1;
    }
    else
    {
        MIXV0_MIX.bits.mixer_prio0= 0x2;
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
}

HI_VOID VDP_MIXV_SetPrio1(HI_U32 prioLayer)
{
    U_MIXV0_MIX          MIXV0_MIX;
    MIXV0_MIX.u32 = VDP_RegRead((HI_U32)&(pVdpReg->MIXV0_MIX.u32));
    if(prioLayer == VDP_LAYER_VID0)
    {
        MIXV0_MIX.bits.mixer_prio1= 0x1;
    }
    else
    {
        MIXV0_MIX.bits.mixer_prio1= 0x2;
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);

}

#ifndef __DISP_PLATFORM_BOOT__
HI_U32 VDP_DHD_GetDispMode(HI_U32 u32Data)
{
    U_DHD0_CTRL DHD0_CTRL;
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DHD0_CTRL.u32));

    return DHD0_CTRL.bits.iop;
}

HI_U32 VDP_WBC_GetAlwaysProFlag(/*VDP_LAYER_WBC_E enLayer,*/ HI_BOOL *bAlwaysPro)
{
    *bAlwaysPro = 1;
    return HI_SUCCESS;
}


HI_VOID  VDP_VID_SetLayerBkgColor    (HI_U32 u32Data, HI_U32 stBkg)
{
   volatile U_V0_BK    V0_BK;

    V0_BK.u32 = stBkg;
    switch(u32Data)
    {
        case  1://VDP_LAYER_VID0
            VDP_RegWrite((HI_U32)(&(pVdpReg->V0_BK.u32)), V0_BK.u32);
            break;
        case  2://VDP_LAYER_VP0
            VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_BK.u32)), V0_BK.u32);
            break;
        case  3://VDP_CBM_MIX0
            VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG1.u32), V0_BK.u32);
            break;
        case  4://MIXV0
            VDP_RegWrite((HI_U32)&(pVdpReg->MIXV0_BKG.u32), V0_BK.u32);
            break;
        case 5://VDP_LAYER_VID1
            VDP_RegWrite((HI_U32)(&(pVdpReg->V0_BK.u32)+VID_OFFSET), V0_BK.u32);
            break;
        case 6://VDP_LAYER_VID3
            VDP_RegWrite((HI_U32)(&(pVdpReg->V0_BK.u32)+3*VID_OFFSET), V0_BK.u32);
            break;
        case 7:
            VDP_RegWrite((HI_U32)&(pVdpReg->CBM_BKG2.u32), V0_BK.u32);
            break;
        default:
            return;
    }
    return ;
}

#endif
HI_U32 VDP_DISP_GetMaskIntSta(HI_U32 u32intmask)
{
    U_VOMSKINTSTA VOMSKINTSTA;

    if (!pVdpReg)
    {
        HI_PRINT("pVdpReg is is a null\n");
        VOMSKINTSTA.u32 = 0;
    }
    else
    {
        /* read interrupt status */
        VOMSKINTSTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOMSKINTSTA.u32)));;
    }

    return (VOMSKINTSTA.u32 & u32intmask);
}

HI_VOID  VDP_VID_SetInDataUVOrder(HI_U32 u32Data, HI_U32  VFirst)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInDataFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.uv_order = VFirst;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_SetWrOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
{
    volatile U_VOAXICTRL VOAXICTRL;

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
    //else if(u32Data == VDP_MASTER1)
    //{
    //    if(u32BusId == 0)
    //    {
    //        VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
    //        VOAXICTRL.bits.m1_wr_ostd = u32OutStd;
    //        VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
    //    }
    //    else
    //    {
    //        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Bus Id,Wr Support one id!\n");
    //    }
    //}
    else
    {
        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
    }

    return ;
}

HI_VOID VDP_SetRdOutStd(HI_U32 u32Data, HI_U32 u32BusId, HI_U32 u32OutStd)
{
    volatile U_VOAXICTRL VOAXICTRL;

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
    //else if(u32Data == VDP_MASTER1)
    //{
    //    if(u32BusId == 0)
    //    {
    //        VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
    //        VOAXICTRL.bits.m1_outstd_rid0 = u32OutStd;
    //        VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
    //    }
    //    else if(u32BusId == 1)
    //    {
    //        VOAXICTRL.u32 = VDP_RegRead(&(pVdpReg->VOAXICTRL.u32));
    //        VOAXICTRL.bits.m1_outstd_rid1 = u32OutStd;
    //        VDP_RegWrite(&(pVdpReg->VOAXICTRL.u32), VOAXICTRL.u32);
    //    }
    //    else
    //    {
    //        HI_PRINT("Error,VDP_SetRdOutStd() Select Wrong Bus Id,Rd Support two id!\n");
    //    }
    //}
    else
    {
        HI_PRINT("Error,VDP_SetWrOutStd() Select Wrong Master!\n");
    }

    return ;
}

HI_VOID VDP_DATE_SetEnable(DISP_VENC_E eDate,HI_U32 enable)
{
    if (DISP_VENC_HDATE0 == eDate)
    {
        U_HDATE_EN HDATE_EN;

        HDATE_EN.u32 = 0;
        HDATE_EN.bits.hd_en = enable;
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_EN.u32), HDATE_EN.u32);
    }

    if (DISP_VENC_SDATE0 == eDate)
    {

    }
}

HI_VOID VDP_DATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta)
{
    return ;
}

HI_VOID VDP_DATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable)
{
    return;
}



HI_VOID VDP_VDAC_GetEnable(HI_U32 uVdac, HI_U32 *penable)
{
    U_VO_DAC_C_CTRL VO_DAC_C_CTRL;
    U_VO_DAC_R_CTRL VO_DAC_R_CTRL;
    U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    switch (uVdac)
    {
        case 3:
            VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_C_CTRL.u32));
            *penable = VO_DAC_C_CTRL.bits.en_c;
            break;
        case 0:

            VO_DAC_R_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_R_CTRL.u32));
            *penable = VO_DAC_R_CTRL.bits.en_r;
            break;
        case 1:
            VO_DAC_G_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_G_CTRL.u32));
            *penable = VO_DAC_G_CTRL.bits.en_g;
            break;
        case 2:
            VO_DAC_B_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_B_CTRL.u32));
            *penable = VO_DAC_B_CTRL.bits.en_b;
            break;
        default :
            break;
    }
    return;
}

#define DISP_DATE_SRC_COEF_INDEX_DEFAULT 0
#define DISP_DATE_SRC_COEF_INDEX_480P  1
#define DISP_DATE_SRC_COEF_INDEX_576P  1
#define DISP_DATE_SRC_COEF_INDEX_720P  2
#define DISP_DATE_SRC_COEF_INDEX_1080I 2
#define DISP_DATE_SRC_COEF_INDEX_1080P 3

HI_VOID VDP_DATE_SetSrcCoef(DISP_VENC_E eDate, HI_U32 *pu32Src13)
{
    HI_U32 *pu32Src13tmp = pu32Src13;

    if (DISP_VENC_HDATE0 == eDate)
    {
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF1.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF2.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF3.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF4.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF5.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF6.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF7.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF8.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF9.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF10.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF11.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF12.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF13.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF14.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF15.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF16.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF17.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->HDATE_SRC_13_COEF18.u32), *pu32Src13tmp++);
    }
    else if (DISP_VENC_SDATE0 == eDate)
    {
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF50.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF51.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF52.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF53.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF54.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF55.u32), *pu32Src13tmp++);

#if  !(defined(CHIP_TYPE_hi3716cv200)   || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3716mv400))
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF56.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF57.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF58.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF59.u32), *pu32Src13tmp++);
#endif
    }
}


#ifndef __DISP_PLATFORM_BOOT__

HI_VOID  VDP_MIXV2_DEFAULT()
{
   volatile U_CBM_MIX2 CBM_MIX2;//b428
   volatile U_CBM_ATTR CBM_ATTR;//b440


    CBM_MIX2.u32 = 0x3;
    CBM_ATTR.u32 = 0x1c;
    VDP_RegWrite((HI_U32)&(pVdpReg->CBM_MIX2.u32), CBM_MIX2.u32);
    VDP_RegWrite((HI_U32)&(pVdpReg->CBM_ATTR.u32), CBM_ATTR.u32);
}

/***********************************SDATE*******************************************/
/* set SDATE interrupt u32Data */
HI_BOOL  SDATE_SetIntEn(HI_U32 u32Data)
{
    volatile U_DATE_ISRMASK DATE_ISRMASK;

    DATE_ISRMASK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_ISRMASK.u32)));
    DATE_ISRMASK.bits.tt_mask = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_ISRMASK.u32)), DATE_ISRMASK.u32);

    return HI_TRUE;
}

/* get SDATE interrupt status  */
HI_BOOL  SDATE_GetIntSta(HI_U32 u32Data)
{
    volatile U_DATE_ISRSTATE DATE_ISRSTATE;

    DATE_ISRSTATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_ISRMASK.u32)));

    if(DATE_ISRSTATE.bits.tt_status == u32Data)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }

}

#endif
HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.c_gain = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}

HI_VOID VDP_DATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt)
{
    if (DISP_VENC_HDATE0 == eDate)
    {
        U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
        U_HDATE_OUT_CTRL     HDATE_OUT_CTRL;

        VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x03ul);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)), 0x02000ul);

        switch(enFmt)
        {
            case HI_DRV_DISP_FMT_1080P_60:
            case HI_DRV_DISP_FMT_1080P_50:
            case HI_DRV_DISP_FMT_1080P_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a3;
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_1080P));
                break;
            case HI_DRV_DISP_FMT_1080i_50:
                /* to resolve vsync for 1080i_50 */
                VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x05ul);
            case HI_DRV_DISP_FMT_1080i_60:
            case HI_DRV_DISP_FMT_1080i_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a4;
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_1080I));
                break;
            case HI_DRV_DISP_FMT_720P_60:
            case HI_DRV_DISP_FMT_720P_50:
            case HI_DRV_DISP_FMT_720P_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_720P));
                break;
            case HI_DRV_DISP_FMT_576P_50:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a1;
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_576P));
                break;
            case HI_DRV_DISP_FMT_480P_60:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a0;
                /* to resolve rwzb for 480p vsync */
                VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)), 0x06000ul);
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_480P));
                break;
            case HI_DRV_DISP_FMT_3840X2160_24:
            case HI_DRV_DISP_FMT_3840X2160_25:
            case HI_DRV_DISP_FMT_3840X2160_30:
            case HI_DRV_DISP_FMT_3840X2160_50:
            case HI_DRV_DISP_FMT_3840X2160_60:
				
            case HI_DRV_DISP_FMT_4096X2160_24:
			case HI_DRV_DISP_FMT_4096X2160_25:
			case HI_DRV_DISP_FMT_4096X2160_30:
			case HI_DRV_DISP_FMT_4096X2160_50:
			case HI_DRV_DISP_FMT_4096X2160_60:
				
            case HI_DRV_DISP_FMT_3840X2160_23_976:
            case HI_DRV_DISP_FMT_3840X2160_29_97:
                 /*4k*2k do nothing. to be confirm*/
                HDATE_VIDEO_FORMAT.u32 =  VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));
                break;
            default :
                HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
                VDP_DATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_DEFAULT));

                break;
        }
        VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);


        HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_OUT_CTRL.u32));

        HDATE_OUT_CTRL.bits.src_ctrl = 2;
        HDATE_OUT_CTRL.bits.sync_lpf_en = 1;

        //HDATE_OUT_CTRL.u32 = HDATE_OUT_CTRL.u32 | 0x1000;
        HDATE_OUT_CTRL.bits.video1_sel = 2;
        HDATE_OUT_CTRL.bits.video2_sel = 3;
        HDATE_OUT_CTRL.bits.video3_sel = 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);


        // DTS2012080100525 : some sony46 TV can not accept 576P50 siganl correctly
        {
            U_HDATE_CLIP HDATE_CLIP;  // new add

            HDATE_CLIP.u32 = 0;

            // 20 x clk_pix + 4 x clk_dis, now clk_pix = 2*clk_dis for HDATE_OUT_CTRL.bits.src_ctrl = 2
            HDATE_CLIP.bits.clip_bb = 80;
            HDATE_CLIP.bits.clip_fb = 8;
            HDATE_CLIP.bits.clip_disable = 0;
            HDATE_CLIP.bits.clip_thdl = 251;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CLIP.u32)), HDATE_CLIP.u32);
        }
    }

    if (DISP_VENC_SDATE0 == eDate)
    {
        U_DATE_COEFF0 DATE_COEFF0;
        U_DATE_COEFF21 DATE_COEFF21;

        switch(enFmt)
        {
            case HI_DRV_DISP_FMT_PAL:
            case HI_DRV_DISP_FMT_PAL_B:
            case HI_DRV_DISP_FMT_PAL_B1:
            case HI_DRV_DISP_FMT_PAL_D:
            case HI_DRV_DISP_FMT_PAL_D1:
            case HI_DRV_DISP_FMT_PAL_G:
            case HI_DRV_DISP_FMT_PAL_H:
            case HI_DRV_DISP_FMT_PAL_K:
            case HI_DRV_DISP_FMT_PAL_I:
                //DATE_COEFF0.u32  = 0x628412dc;
                DATE_COEFF0.u32  = 0x6a8452dc;
                break;
            case HI_DRV_DISP_FMT_PAL_N:
                DATE_COEFF0.u32  = 0x6a8852dc;
                break;

            case HI_DRV_DISP_FMT_PAL_Nc:
                DATE_COEFF0.u32  = 0x6a9052dc;
                break;

            case HI_DRV_DISP_FMT_SECAM_SIN:      /**< SECAM_SIN*/
                DATE_COEFF0.u32  = 0x6aa052dc;
                break;

            case HI_DRV_DISP_FMT_SECAM_COS:      /**< SECAM_COS*/
                DATE_COEFF0.u32  = 0x6ae052dc;
                break;

            //NTSC
            case HI_DRV_DISP_FMT_PAL_M:
                DATE_COEFF0.u32  = 0x449050dc;
                break;

            case HI_DRV_DISP_FMT_PAL_60:
                DATE_COEFF0.u32  = 0x44a050dc;
                break;

            case HI_DRV_DISP_FMT_NTSC:
                DATE_COEFF0.u32  = 0x048450dc;
                break;
            case HI_DRV_DISP_FMT_NTSC_J:
                DATE_COEFF0.u32  = 0x248450dc;
                break;

            case HI_DRV_DISP_FMT_NTSC_443:
                DATE_COEFF0.u32  = 0x04a450dc;
                break;
            default :
                DATE_COEFF0.u32  = 0x628412dc;
                break;
        }

        /*zkun_confirm:*/
        VDP_DATE_SetSrcCoef(eDate, VDP_Get_SDateSrcCoef(0));
        // delay time of luma and chroma is too much, set luma_dl tobe zero.
        DATE_COEFF0.bits.luma_dl = 0;
        // close pbpr low pass filter for standard test
        DATE_COEFF0.bits.pbpr_lpf_en = 0;

        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
        SDATE_Setc_gain(1);

        // to get good SCH parameters, set video_phase_delta as 0x9A
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF22.u32)), SCH_PHASE);


        DATE_COEFF21.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF21.u32));
        DATE_COEFF21.bits.dac0_in_sel = 1;
        DATE_COEFF21.bits.dac1_in_sel = 2;
        DATE_COEFF21.bits.dac2_in_sel = 4;
        DATE_COEFF21.bits.dac3_in_sel = 3;
        DATE_COEFF21.bits.dac4_in_sel = 5;
        DATE_COEFF21.bits.dac5_in_sel = 6;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF21.u32)), DATE_COEFF21.u32);

    }
}
HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.dis_ire = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

/*zkun_confirm:*/
HI_VOID VDP_DATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync)
{
    //TODO SELECT RGB or YPbPr   and sync mode
    if (HI_DRV_DISP_INTF_YPBPR0 == enIntfId)
    {
        if (DISP_VENC_HDATE0 ==  eDate )
        {
            U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
            /**/
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));

            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 0;/*YCbCr－>YPbPr*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 1;/*YPbPr insight sync*/
            HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;

            DATE_COEFF0.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF0.u32));
            /*0: yuv   1: rgb*/
            DATE_COEFF0.bits.rgb_en = 0;/* yuv*/

            DATE_COEFF0.bits.sync_mode_sel =   0;
            /*1:no sync
                0:sync_mode_sel is
            */
            DATE_COEFF0.bits.sync_mode_scart = 0;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
        }
        else
        {/*VGA do nothing
           */
        }

    }
    if (HI_DRV_DISP_INTF_RGB0 == enIntfId)
    {
        if (DISP_VENC_HDATE0 == eDate )
        {
            U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
            /**/
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));
            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 1;/*YCbCr—>RGB ITU-R BT709*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 0;/*RGB sync in*/

            if (!bRGBSync)
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 0; /*no sync*/
            else
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;
            DATE_COEFF0.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF0.u32));
            /*0: yuv   1: rgb*/
            DATE_COEFF0.bits.rgb_en = 1;

             if (!bRGBSync)
            {
                DATE_COEFF0.bits.sync_mode_scart = 1;
                DATE_COEFF0.bits.sync_mode_sel =   0;
            }
            else
            {
                DATE_COEFF0.bits.sync_mode_scart = 0;
                DATE_COEFF0.bits.sync_mode_sel =   1;
            }
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

        }
    }
}

/* 7.5 IRE enable */

#ifndef __DISP_PLATFORM_BOOT__

/* get SDATE interrupt */
HI_BOOL  SDATE_ClrIntSta(HI_U32 u32Data)
{
    volatile U_DATE_ISR DATE_ISR;

    DATE_ISR.bits.tt_int = true;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_ISR.u32)), DATE_ISR.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetOutMode(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF21 DATE_COEFF21;

    DATE_COEFF21.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF21.u32)));

    switch(u32Data)
    {
        case SDATE_DACMODE_DAC0:
        {
            DATE_COEFF21.bits.dac0_in_sel = u32Data1;
            break;
        }
        case SDATE_DACMODE_DAC1:
        {
            DATE_COEFF21.bits.dac1_in_sel = u32Data1;
            break;
        }
        case SDATE_DACMODE_DAC2:
        {
            DATE_COEFF21.bits.dac2_in_sel = u32Data1;
            break;
        }
        case SDATE_DACMODE_DAC3:
        {
            DATE_COEFF21.bits.dac3_in_sel = u32Data1;
            break;
        }
        case SDATE_DACMODE_DAC4:
        {
            DATE_COEFF21.bits.dac4_in_sel = u32Data1;
            break;
        }
        case SDATE_DACMODE_DAC5:
        {
            DATE_COEFF21.bits.dac5_in_sel = u32Data1;
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetOutMode() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF21.u32)), DATE_COEFF21.u32);

    return HI_TRUE;
}


HI_BOOL SDATE_SetSysMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.scanline = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetMduMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.style_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetSysTyp(HI_U32 u32Data)
{
#if 0
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));

    if(u32Data > SDATE_MDUMD_625)
    {
        DATE_COEFF0.bits.scanline =  SDATE_MDUMD_625;
    }
    else
    {
        DATE_COEFF0.bits.scanline =  SDATE_MDUMD_525;
    }

    DATE_COEFF0.bits.style_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
#endif
    return HI_TRUE;
}


/* select chrmoa lpf coeff */
HI_BOOL  SDATE_SetChmCoeff(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.clpf_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}




/* set RGB output enable */
HI_BOOL  SDATE_SetCompTypMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.rgb_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

 /* set sync u32Data for component output  */
HI_BOOL  SDATE_SetCompSynMode(HI_U32 u32Data)
{
    HI_U32 value = 0;
    volatile U_DATE_COEFF0 DATE_COEFF0;
    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));


    if(u32Data != SDATE_SYNCMOD_NULL)
    {
        value = DATE_COEFF0.bits.sync_mode_sel;
        value = value & 0x01;
        value = value + (u32Data << 1);
        DATE_COEFF0.bits.sync_mode_sel = value;
    }
    else
    {
        DATE_COEFF0.bits.sync_mode_sel = 0;
        DATE_COEFF0.bits.sync_mode_scart = 1;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

/* set blanking base for RGB output */
HI_BOOL  SDATE_SetBlnkMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;
    HI_U32 value = 0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));

    value = DATE_COEFF0.bits.sync_mode_sel ;
    value = (value & 0x02) + u32Data;
    DATE_COEFF0.bits.sync_mode_sel = value;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set length u32Data of one line */
HI_BOOL  SDATE_SetLenMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.length_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set generate u32Data of AGC pulse  */
HI_BOOL  SDATE_SetGenAgcMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.agc_amp_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set delay u32Data bwtween lum and chroma */
HI_BOOL  SDATE_SetDlyMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.luma_dl = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

/* set lum over-sample enable */
HI_BOOL  SDATE_SetLumOsEn(HI_U32  u32Data,HI_U32 u32Data1)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;
    HI_U32 value = 0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));

    switch(u32Data)
    {
        case SDATE_LUMOS_TAP1:
        {
            value = DATE_COEFF0.bits.oversam_en ;
            value = (value & 0x02) + u32Data1;
            DATE_COEFF0.bits.oversam_en = value;

            break;
        }
        case SDATE_LUMOS_TAP2:
        {
            DATE_COEFF0.bits.oversam2_en = u32Data1;
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetLumOsEn() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

/* set chroma over-sample enable */
HI_BOOL  SDATE_SetChmOsEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;
    HI_U32 value = 0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));

    value = DATE_COEFF0.bits.oversam_en ;
    value = (value & 0x01) + u32Data;
    DATE_COEFF0.bits.oversam_en = value;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set lum notch enable */
HI_BOOL  SDATE_SetLumNthEnable(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.lunt_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set chroma lowpass filtering enable */
HI_BOOL  SDATE_SetChmLpEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.chlp_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set synchronization lowpass filtering enable */
HI_BOOL  SDATE_SetSyncLpEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.sylp_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set chroma gain enable */
HI_BOOL  SDATE_SetChmGainEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.chgain_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


/* set teletext send sequence order*/
HI_BOOL  SDATE_SetTtSeq(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.tt_seq = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}
#endif
/*zkun_confirm:*/
#define VDAC_SYSCTRL_RESET_VALUE 0x000010F3ul
#define VDAC_CTRL_ISO_RCT_VALUE 0x00004000ul
#define VDAC_CTRL_EN_RCT_VALUE  0x00008000ul


HI_VOID VDP_VDAC_ResetCRG(HI_VOID)
{
    U_PERI_CRG71 PERI_CRG71TMP;

    PERI_CRG71TMP.u32 = g_pstRegCrg->PERI_CRG71.u32;

    PERI_CRG71TMP.bits.vdac_chop_cken = 0;
    PERI_CRG71TMP.bits.vdac_c_clk_pctrl = 0;

#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
    PERI_CRG71TMP.bits.vdac_r_clk_pctrl = 0;
    PERI_CRG71TMP.bits.vdac_g_clk_pctrl = 0;
    PERI_CRG71TMP.bits.vdac_b_clk_pctrl = 0;
#endif

    g_pstRegCrg->PERI_CRG71.u32 = PERI_CRG71TMP.u32;
}

#define VDAC_CTRL_ENBG_AND_TRIM_RESET_VALUE1  0x80c00000ul

HI_VOID VDP_VDAC_Reset(HI_VOID)
{
#if !(defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100))
    HI_U32 ctrlv;

    VDP_VDAC_ResetCRG();
    /*
    0xF8CC0120  0x00004000 //common block(29\14) iso控制寄存器默认为0；
    //0xF8CC0130  0x10000000 //c channel (28)
    //0xF8CC0134  0x10000000 //r channel (28)
    //0xF8CC0138  0x10000000 //g channel (28)
    //0xF8CC013c  0x10000000 //b channel (28)
    */

    ctrlv = VDAC_CTRL_ISO_RCT_VALUE;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), ctrlv);

    /*
    0xF8CC0120   0xa0004000 // enable bg (31\29\14)
   // 0xF8CC0120   0x80004000 //release iso (31\14)  delay more than 10ms
    0xF8CC0120   0x8000c000 // enable rct (31\15\14)
    0xF8CC0120   0x80008000 //release iso (31\15) delay 1us
    */

    /*zkun_confirm:*/
    ctrlv = VDAC_CTRL_ISO_RCT_VALUE | VDAC_CTRL_ENBG_AND_TRIM_RESET_VALUE1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), ctrlv);

    DISP_MSLEEP(10);

    ctrlv = VDAC_CTRL_ISO_RCT_VALUE | VDAC_CTRL_EN_RCT_VALUE | VDAC_CTRL_ENBG_AND_TRIM_RESET_VALUE1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), ctrlv);

    ctrlv = VDAC_CTRL_EN_RCT_VALUE | VDAC_CTRL_ENBG_AND_TRIM_RESET_VALUE1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), ctrlv);
#else
    VDP_VDAC_ResetCRG();
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), 0x80000000);
#endif

    DISP_MSLEEP(1);

    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX_DAC.u32)), 0);
    DISP_WARN("=========VDAC_DRIVER_Initial====\n");

    return;
}

#ifndef __DISP_PLATFORM_BOOT__
/* set cc send sequence order*/
HI_BOOL  SDATE_SetCcSeq(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.cc_seq = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}

#endif

#define X5HD2_VDAC0_MASK_RESET_OVER   0xFFE3ul
#define X5HD2_VDAC0_MASK_RESET        0x10ul

#define X5HD2_VDAC1_MASK_RESET_OVER   0xFFD3ul
#define X5HD2_VDAC1_MASK_RESET        0x20ul

#define X5HD2_VDAC2_MASK_RESET_OVER   0xFFB3ul
#define X5HD2_VDAC2_MASK_RESET        0x40ul

#define X5HD2_VDAC3_MASK_RESET_OVER   0xFF73ul
#define X5HD2_VDAC3_MASK_RESET        0x80ul

#define FILTER_100M    0x0
#define FILTER_50M    0x2
#define FILTER_30M    0x3

#define X5HD2_VDACX_FILTER_OFFSET   25

#define VDECX_MODE_TV    0x0
#define VDECX_MODE_VGA    0x1

#define X5HD2_VDACX_MODE_C_OFFSET   27

HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset)
{
    #if 0
    U_PERI_CRG71 PERI_CRG71Tmp;

    PERI_CRG71Tmp.u32 = g_pstRegCrg->PERI_CRG71.u32;
    #endif
    
    /*pay attention, no reset?*/
    return;
}


typedef enum tagVGA_PIX_RANG_E
{
    VGA_PIX_LESS_65M = 0,
    VGA_PIX_65M_74_25M,
    VGA_PIX_74_25M_85_75M,
    VGA_PIX_87_75M_108M,
    VGA_PIX_MORE_108M,
    VGA_PIX_BUT
}VGA_PIX_RANG_E;
HI_U32 u32VGADacRevise[VGA_PIX_BUT]=
{
    0xcffa0000,/*<65M*/
    0xcbfa0000,/*65M~74.25M*/
    0xc8010000,/*74.25M~85.75M*/
    0xc19a0000,/*85.75M~108M*/
    0xc8000000,/*>108M*/
};
HI_U32 VDP_GetVGADacReviseValue(HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk)
{
    VGA_PIX_RANG_E enVGAPixRang;
    if (HI_DRV_DISP_FMT_CUSTOM == enFmt)
    {
        if (u32CustomPixClk < 65000)
            enVGAPixRang = VGA_PIX_LESS_65M;
        else if (u32CustomPixClk < 74250)
            enVGAPixRang = VGA_PIX_65M_74_25M;
        else if (u32CustomPixClk < 85750)
            enVGAPixRang = VGA_PIX_74_25M_85_75M;
        else if (u32CustomPixClk < 108000)
            enVGAPixRang = VGA_PIX_87_75M_108M;
        else
            enVGAPixRang = VGA_PIX_MORE_108M;
    }else if ((HI_DRV_DISP_FMT_861D_640X480_60  == enFmt)
        ||(HI_DRV_DISP_FMT_VESA_800X600_60  == enFmt)
        )
    {
        enVGAPixRang = VGA_PIX_LESS_65M;
    }
    else if (HI_DRV_DISP_FMT_VESA_1024X768_60  == enFmt)
    {
        enVGAPixRang = VGA_PIX_65M_74_25M;
    }else if ((HI_DRV_DISP_FMT_VESA_1280X720_60  == enFmt)
        ||(HI_DRV_DISP_FMT_VESA_1280X800_60  == enFmt)
        ||(HI_DRV_DISP_FMT_VESA_1360X768_60  == enFmt)
        )
    {
        enVGAPixRang = VGA_PIX_74_25M_85_75M;
    }
    else
    {
        enVGAPixRang = VGA_PIX_87_75M_108M;
    }
    return u32VGADacRevise[enVGAPixRang];
}


HI_VOID VDP_VDAC_ResetFmt(DISP_VENC_E enVenc ,HI_U32 uVdac, HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk)
{
    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);

    switch(uVdac)
    {
        case 3:
            VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_C_CTRL.u32)), VDAC_DEFAULT_VALUE);
            break;

        case 0:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC1_MASK_RESET;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_R_CTRL.u32)), VDAC_DEFAULT_VALUE);
            break;

        case 1:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC2_MASK_RESET;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_G_CTRL.u32)), VDAC_DEFAULT_VALUE);

            break;
        case 2:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC3_MASK_RESET;
            VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_B_CTRL.u32)), VDAC_DEFAULT_VALUE);

            break;
        default:
            return;
    }
    return;
}


#ifndef __DISP_PLATFORM_BOOT__
/* set cgms send sequence order*/
HI_BOOL  SDATE_SetCgmsSeq(HI_U32  u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.cgms_seq = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


/* set vps send sequence order*/
HI_BOOL  SDATE_SetVpsSeq(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.vps_seq = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


/* set wss send sequence order*/
HI_BOOL  SDATE_SetWssSeq(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.wss_seq = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


HI_BOOL  SDATE_SetChmLmtEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.c_limit_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}

HI_BOOL  SDATE_SetCvbsLmtEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.cvbs_limit_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}

/* set test u32Data enable */
HI_BOOL  SDATE_SetTestEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.date_test_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


/* set test u32Data */
HI_BOOL  SDATE_SetTestMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.date_test_mode = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


/* set dac test input*/
HI_BOOL  SDATE_SetTestData(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.dac_test = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


/* set outside AGC pulse amplification  */
HI_BOOL  SDATE_SetOutAgcAmp(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.amp_outside = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}
#endif

/*zkun_confirm:*/
/*
    DAC （ 0x0）。

    0x0： （SDATE）；
    0x1： Y/R （SDATE）；
    0x2： Cb/G （SDATE）；
    0x3： Cr/B （SDATE）；
    0x4： Y/R （HDATE）；
    0x5： Cb/G （HDATE）；
    0x6： Cr/B （HDATE）；
    0x7：VGA   R；
    0x8：VGA   G；
    0x9：VGA   B。
*/
static HI_U32 s_VDACLink[3][HI_DRV_DISP_VDAC_SIGNAL_BUTT] =
{/*0  1  2  3  4     5  6  7  8  9    10*/
    {4, 4, 4, 5, 6,    4, 4, 4, 5, 6,    4},/*HDATE0*/
    {0, 0, 1, 2, 3,    0, 0, 1, 2, 3,    0},/*SDATE0*/
    {7, 7, 7, 7, 7,    7, 7, 7, 8, 9,    7},/*VGA0*/
};


HI_VOID VDP_VDAC_SetLink(DISP_VENC_E eDate, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal)
{
    U_VO_MUX_DAC VO_MUX_DAC;

    //HI_PRINT("VDP_VDAC_SetLink date=%d, dac=%d, sig=%d\n", eDate, uVdac, signal);

    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);

    VO_MUX_DAC.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_MUX_DAC.u32));
    switch (uVdac)
    {
        case 0:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC0_MASK_RESET;
            VO_MUX_DAC.bits.dac0_sel = s_VDACLink[eDate][signal];
            break;
        case 1:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC1_MASK_RESET;
            VO_MUX_DAC.bits.dac1_sel = s_VDACLink[eDate][signal];
            break;

        case 2:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC2_MASK_RESET;
            VO_MUX_DAC.bits.dac2_sel = s_VDACLink[eDate][signal];
            break;

        case 3:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC3_MASK_RESET;
            VO_MUX_DAC.bits.dac3_sel = s_VDACLink[eDate][signal];
            break;

        default :
            return;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX_DAC.u32)), VO_MUX_DAC.u32);

    //TODO SELECT CLK
    return;
}

HI_VOID VDP_VDAC_SetEnable(HI_U32 uVdac, HI_U32 enable,HI_U32 u32DelayMs)
{
    U_VO_DAC_C_CTRL VO_DAC_C_CTRL;
    U_VO_DAC_R_CTRL VO_DAC_R_CTRL;
    U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);
    switch (uVdac)
    {
        case 3:
            if (enable)
            {
                VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_C_CTRL.u32));
                VO_DAC_C_CTRL.bits.en_c = 1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_C_CTRL.u32)), VO_DAC_C_CTRL.u32);
            }
            else
            {
                VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_C_CTRL.u32));
                VO_DAC_C_CTRL.bits.en_c = 0;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_C_CTRL.u32)), VO_DAC_C_CTRL.u32);
            }
            break;
        case 0:
            if (enable)
            {
                VO_DAC_R_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_R_CTRL.u32));
                VO_DAC_R_CTRL.bits.en_r = 1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_R_CTRL.u32)), VO_DAC_R_CTRL.u32);
            }
            else
            {
                VO_DAC_R_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_R_CTRL.u32));
                VO_DAC_R_CTRL.bits.en_r = 0;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_R_CTRL.u32)), VO_DAC_R_CTRL.u32);
            }
            break;
        case 1:
            if (enable)
            {
                VO_DAC_G_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_G_CTRL.u32));
                VO_DAC_G_CTRL.bits.en_g = 1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_G_CTRL.u32)), VO_DAC_G_CTRL.u32);
            }
            else
            {
                VO_DAC_G_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_G_CTRL.u32));
                VO_DAC_G_CTRL.bits.en_g = 0;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_G_CTRL.u32)), VO_DAC_G_CTRL.u32);
            }
            break;
        case 2:
            if (enable)
            {
                VO_DAC_B_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_B_CTRL.u32));
                VO_DAC_B_CTRL.bits.en_b = 1;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_B_CTRL.u32)), VO_DAC_B_CTRL.u32);
            }
            else
            {
                VO_DAC_B_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->VO_DAC_B_CTRL.u32));
                VO_DAC_B_CTRL.bits.en_b = 0;
                VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_B_CTRL.u32)), VO_DAC_B_CTRL.u32);
            }
            break;
        default :
            break;

    }

    if (enable)
    {
        VDP_VDAC_SetReset(uVdac, HI_FALSE);
   }

    return;
}

HI_VOID VDP_VDAC_SetClockEnable(HI_U32 uVdac, HI_U32 enable)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    switch(uVdac)
    {
        case 0:
            PERI_CRG54Tmp.bits.vdac_ch0_cken = (enable == HI_TRUE)? 1 : 0;
            break;
#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
        case 1:
            PERI_CRG54Tmp.bits.vdac_ch1_cken = (enable == HI_TRUE)? 1 : 0;
            break;
        case 2:
            PERI_CRG54Tmp.bits.vdac_ch2_cken = (enable == HI_TRUE)? 1 : 0;
            break;
        case 3:
            PERI_CRG54Tmp.bits.vdac_ch3_cken = (enable == HI_TRUE)? 1 : 0;
            break;
#endif
        default:
            break;
    }

    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;

    return;
}

#ifndef __DISP_PLATFORM_BOOT__

//-------------------------------------------------------------------
// VIDEO LAYER CONFIG
HI_VOID  VDP_VID_SetInReso2(HI_U32 u32Data, HI_RECT_S *pstRect, HI_RECT_S *pstRectOrigin)
{
    U_V0_IRESO V0_IRESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));

    /*we should set original widht to register
     *and set height undergoing extract line to reg.
     */
    V0_IRESO.bits.iw = pstRectOrigin->s32Width - 1;
    V0_IRESO.bits.ih = pstRect->s32Height - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);

    return ;
}

HI_VOID  VDP_VID_SetInCrop(HI_U32 u32Data, HI_U32 xstartposition, HI_U32 xstopposition)
{
    U_V0_CPOS  V0_CPOS;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = xstartposition;
    V0_CPOS.bits.src_xlpos = xstopposition - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);

    return ;
}

HI_VOID  VDP_VID_SetOutReso2(HI_U32 u32Data, HI_RECT_S *pstRect)
{
    U_V0_ORESO V0_ORESO;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
       return ;
   }

   //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);
   V0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
   V0_ORESO.bits.ow = pstRect->s32Width - 1;
   V0_ORESO.bits.oh = pstRect->s32Height - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);

   return ;
}

HI_VOID  VDP_VID_SetVideoPos2(HI_U32 u32Data, HI_RECT_S *pstRect)
{
   U_V0_VFPOS V0_VFPOS;
   U_V0_VLPOS V0_VLPOS;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetVideoPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   V0_VFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
   V0_VFPOS.bits.video_xfpos = pstRect->s32X;
   V0_VFPOS.bits.video_yfpos = pstRect->s32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET), V0_VFPOS.u32);

   V0_VLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
   V0_VLPOS.bits.video_xlpos = pstRect->s32X + pstRect->s32Width - 1;
   V0_VLPOS.bits.video_ylpos = pstRect->s32Y + pstRect->s32Height - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET), V0_VLPOS.u32);
   return ;
}

HI_VOID  VDP_VID_SetDispPos2(HI_U32 u32Data, HI_RECT_S *pstRect)
{
   U_V0_DFPOS V0_DFPOS;
   U_V0_DLPOS V0_DLPOS;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetDispPos() Select Wrong Video Layer ID\n");
       return ;
   }

   /*video position */
   V0_DFPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
   V0_DFPOS.bits.disp_xfpos = pstRect->s32X;
   V0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET), V0_DFPOS.u32);

   V0_DLPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
   V0_DLPOS.bits.disp_xlpos = pstRect->s32X + pstRect->s32Width - 1;
   V0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->s32Height - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET), V0_DLPOS.u32);
   return ;
}

#if 0
HI_VOID  VDP_VID_SetZmeEnable2(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_HSP.bits.hlmsc_en  = u32bEnable;
    V0_HSP.bits.hchmsc_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.vlmsc_en  = u32bEnable;
    V0_VSP.bits.vchmsc_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}
#endif
#if 0
/* Vou set zoom inital phase */
HI_VOID  VDP_VID_SetZmePhaseH(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_HLOFFSET  V0_HLOFFSET;
    U_V0_HCOFFSET  V0_HCOFFSET;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    V0_HLOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET));
    V0_HLOFFSET.bits.hor_loffset = s32PhaseL;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET), V0_HLOFFSET.u32);

    V0_HCOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET));
    V0_HCOFFSET.bits.hor_coffset = s32PhaseC;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET), V0_HCOFFSET.u32);


    return ;
}
#endif

HI_VOID  VDP_VID_SetZmePhaseV_Define1(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_VOFFSET   V0_VOFFSET;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET));
    V0_VOFFSET.bits.vluma_offset   = s32PhaseL;
    V0_VOFFSET.bits.vchroma_offset = s32PhaseC;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32);

    return ;
}
#if 0
HI_VOID  VDP_VID_SetZmePhaseVB(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
    U_V0_VBOFFSET  V0_VBOFFSET;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET));
    V0_VBOFFSET.bits.vbluma_offset   = s32PhaseL;
    V0_VBOFFSET.bits.vbchroma_offset = s32PhaseC;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32);

    return ;
}
#endif
#if 0
HI_VOID  VDP_VID_SetZmeFirEnable2(HI_U32 u32Data,
                                  HI_U32 u32bEnableHl,
                                  HI_U32 u32bEnableHc,
                                  HI_U32 u32bEnableVl,
                                  HI_U32 u32bEnableVc)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

     V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
     V0_HSP.bits.hlfir_en  = u32bEnableHl;
     V0_HSP.bits.hchfir_en = u32bEnableHc;
     VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

     V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
     V0_VSP.bits.vlfir_en  = u32bEnableVl;
     V0_VSP.bits.vchfir_en = u32bEnableVc;
     VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}

#endif
#if 0
HI_VOID  VDP_VID_SetZmeMidEnable2(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_HSP.bits.hlmid_en = u32bEnable;
    V0_HSP.bits.hchmid_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32);

    V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.vlmid_en  = u32bEnable;
    V0_VSP.bits.vchmid_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}
#endif
HI_VOID VDP_CBM_GetMixerPrio(VDP_CBM_MIX_E u32mixer_id, HI_U32 u32prio, HI_U32 *pu32layer_id)
{
    U_MIXV0_MIX MIXV0_MIX;
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
    HI_U32 id;

    if(u32mixer_id == VDP_CBM_MIXV0)
    {
        switch(u32prio)
        {
            case 0:
            {
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)));
                id = MIXV0_MIX.bits.mixer_prio0;
                break;
            }
            case 1:
            {
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)));
                id = MIXV0_MIX.bits.mixer_prio1;
                break;
            }
            default:
            {
                id = 0;
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
            }
        }

        if (id > 0)
        {
            *pu32layer_id = id - 1;
        }
        else
        {
            *pu32layer_id = VDP_LAYER_VID_BUTT;
        }

    }
    else if(u32mixer_id == VDP_CBM_MIXV1)
    {
#if 0
        switch(u32prio)
        {
            case 0:
            {
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV1_MIX.u32)));
                id = MIXV0_MIX.bits.mixer_prio0;
                break;
            }
            case 1:
            {
                MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV1_MIX.u32)));
                id = MIXV0_MIX.bits.mixer_prio1;
                break;
            }
            default:
            {
                id = 0;
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
            }
            if (id > 0)
            {
                *pu32layer_id = id - 1;
            }
            else
            {
                *pu32layer_id = VDP_LAYER_VID_BUTT;
            }
        }
#endif
        *pu32layer_id = VDP_LAYER_VID_BUTT;
    }
    else if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                id = CBM_MIX1.bits.mixer_prio0;
                break;
            }
            case 1:
            {
                CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));
                id = CBM_MIX1.bits.mixer_prio1;
                break;
            }
            default:
            {
                id = 0;
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
            }
        }

        if (id > 0)
        {
            *pu32layer_id = id - 1;
        }
        else
        {
            *pu32layer_id = VDP_CBM_BUTT;
        }
    }
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        switch(u32prio)
        {
            case 0:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                id = CBM_MIX2.bits.mixer_prio0;
                break;
            }
            case 1:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                id = CBM_MIX2.bits.mixer_prio1;
                break;
            }
            case 2:
            {
                CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));
                id = CBM_MIX2.bits.mixer_prio2;
                break;
            }
            default:
            {
                id = 0;
                HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
            }
        }

        if (id > 0)
        {
            *pu32layer_id = id - 1;
        }
        else
        {
            *pu32layer_id = VDP_CBM_BUTT;
        }

    }
}

HI_VOID VDP_CBM_SetMixerPrioQuick(VDP_CBM_MIX_E u32mixer_id,HI_U32 *pu32layer_id, HI_U32 u32Number)
{
    U_MIXV0_MIX MIXV0_MIX;
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;

    if(u32mixer_id == VDP_CBM_MIXV0)
    {
        MIXV0_MIX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)));

        MIXV0_MIX.bits.mixer_prio0 = 0;
        MIXV0_MIX.bits.mixer_prio1 = 0;

        if (u32Number > 0)
        {
            if (pu32layer_id[0] < VDP_LAYER_VID2)
            {
                MIXV0_MIX.bits.mixer_prio0 = pu32layer_id[0]+1;
            }
        }

        if (u32Number > 1)
        {
            if (pu32layer_id[1] < VDP_LAYER_VID2)
            {
                MIXV0_MIX.bits.mixer_prio1 = pu32layer_id[1]+1;
            }
        }

        if (u32Number > 2)
        {
            HI_PRINT("Error, Vou_SetCbmMixerPrioQuickly() Two many number\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIXV1)
        ;
    else if(u32mixer_id == VDP_CBM_MIX0)//DHD0
    {
        CBM_MIX1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX1.u32)));

        CBM_MIX1.bits.mixer_prio0 = 0;
        CBM_MIX1.bits.mixer_prio1 = 0;

        if (u32Number > 0)
        {
            if (pu32layer_id[0] < VDP_CBM_VP1)
            {
                CBM_MIX1.bits.mixer_prio0 = pu32layer_id[0]+1;
            }
        }

        if (u32Number > 1)
        {
            if (pu32layer_id[1] < VDP_CBM_VP1)
            {
                CBM_MIX1.bits.mixer_prio1 = pu32layer_id[1]+1;
            }
        }

        if (u32Number > 2)
        {
            HI_PRINT("Error, Vou_SetCbmMixerPrioQuickly() Two many number\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX1.u32)), CBM_MIX1.u32);

    }
    else if(u32mixer_id == VDP_CBM_MIX1)//DHD1
    {
        CBM_MIX2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->CBM_MIX2.u32)));

        CBM_MIX2.bits.mixer_prio0 = 0;
        CBM_MIX2.bits.mixer_prio1 = 0;

        if (u32Number > 0)
        {
            if ( (pu32layer_id[0] < VDP_CBM_BUTT) && (pu32layer_id[0] > VDP_CBM_GP0))
            {
                CBM_MIX2.bits.mixer_prio0 = pu32layer_id[0]+1;
            }
        }

        if (u32Number > 1)
        {
            if ( (pu32layer_id[1] < VDP_CBM_BUTT) && (pu32layer_id[1] > VDP_CBM_GP0))
            {
                CBM_MIX2.bits.mixer_prio1 = pu32layer_id[1]+1;
            }
        }

        if (u32Number > 2)
        {
            HI_PRINT("Error, Vou_SetCbmMixerPrioQuickly() Two many number\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->CBM_MIX2.u32)), CBM_MIX2.u32);

    }
}

#if 0
HI_VOID  VDP_VID_SetZmeVchTap(HI_U32 u32Data, HI_U32 u32VscTap)
{

    return ;
}
#endif

#if 0
/* set macrovision coefficient */
HI_BOOL  SDATE_SetMvNxValue(HI_U32 u32Data, HI_U32 u32Data1)
{
    switch(u32Data)
    {
        case SDATE_MV_N0:
        {
            volatile U_DATE_COEFF2 DATE_COEFF2;

            DATE_COEFF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)));
            DATE_COEFF2.bits.N0 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)), DATE_COEFF2.u32);

            break;
        }
        case SDATE_MV_N1:
        {
            volatile U_DATE_COEFF2 DATE_COEFF2;

            DATE_COEFF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)));
            DATE_COEFF2.bits.N1 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)), DATE_COEFF2.u32);

            break;
        }
        case SDATE_MV_N2:
        {
            volatile U_DATE_COEFF2 DATE_COEFF2;

            DATE_COEFF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)));
            DATE_COEFF2.bits.N2 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)), DATE_COEFF2.u32);

            break;
        }
        case SDATE_MV_N3:
        {
            volatile U_DATE_COEFF2 DATE_COEFF2;

            DATE_COEFF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)));
            DATE_COEFF2.bits.N3 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)), DATE_COEFF2.u32);

            break;
        }
        case SDATE_MV_N4:
        {
            volatile U_DATE_COEFF2 DATE_COEFF2;

            DATE_COEFF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)));
            DATE_COEFF2.bits.N4 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF2.u32)), DATE_COEFF2.u32);

            break;
        }
        case SDATE_MV_N5:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N5 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N6:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N6 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N7:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N7 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N8:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N8 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N9:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N9 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N10:
        {
            volatile U_DATE_COEFF3 DATE_COEFF3;

            DATE_COEFF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)));
            DATE_COEFF3.bits.N10 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF3.u32)), DATE_COEFF3.u32);

            break;
        }
        case SDATE_MV_N11:
        {
            volatile U_DATE_COEFF4 DATE_COEFF4;

            DATE_COEFF4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF4.u32)));
            DATE_COEFF4.bits.N11 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF4.u32)), DATE_COEFF4.u32);

            break;
        }
        case SDATE_MV_N12:
        {
            volatile U_DATE_COEFF4 DATE_COEFF4;

            DATE_COEFF4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF4.u32)));
            DATE_COEFF4.bits.N12 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF4.u32)), DATE_COEFF4.u32);

            break;
        }
        case SDATE_MV_N13:
        {
            volatile U_DATE_COEFF5 DATE_COEFF5;

            DATE_COEFF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)));
            DATE_COEFF5.bits.N13 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)), DATE_COEFF5.u32);

            break;
        }
        case SDATE_MV_N14:
        {
            volatile U_DATE_COEFF5 DATE_COEFF5;

            DATE_COEFF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)));
            DATE_COEFF5.bits.N14 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)), DATE_COEFF5.u32);

            break;
        }
        case SDATE_MV_N15:
        {
            volatile U_DATE_COEFF5 DATE_COEFF5;

            DATE_COEFF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)));
            DATE_COEFF5.bits.N15 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)), DATE_COEFF5.u32);

            break;
        }
        case SDATE_MV_N16:
        {
            volatile U_DATE_COEFF5 DATE_COEFF5;

            DATE_COEFF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)));
            DATE_COEFF5.bits.N16 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)), DATE_COEFF5.u32);

            break;
        }
        case SDATE_MV_N17:
        {
            volatile U_DATE_COEFF5 DATE_COEFF5;

            DATE_COEFF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)));
            DATE_COEFF5.bits.N17 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF5.u32)), DATE_COEFF5.u32);

            break;
        }
        case SDATE_MV_N18:
        {
            volatile U_DATE_COEFF6 DATE_COEFF6;

            DATE_COEFF6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)));
            DATE_COEFF6.bits.N18 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)), DATE_COEFF6.u32);

            break;
        }
        case SDATE_MV_N19:
        {
            volatile U_DATE_COEFF6 DATE_COEFF6;

            DATE_COEFF6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)));
            DATE_COEFF6.bits.N19 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)), DATE_COEFF6.u32);

            break;
        }
        case SDATE_MV_N20:
        {
            volatile U_DATE_COEFF6 DATE_COEFF6;

            DATE_COEFF6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)));
            DATE_COEFF6.bits.N20 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)), DATE_COEFF6.u32);

            break;
        }
        case SDATE_MV_N21:
        {
            volatile U_DATE_COEFF6 DATE_COEFF6;

            DATE_COEFF6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)));
            DATE_COEFF6.bits.N21 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)), DATE_COEFF6.u32);

            break;
        }
        case SDATE_MV_N22:
        {
            volatile U_DATE_COEFF6 DATE_COEFF6;

            DATE_COEFF6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)));
            DATE_COEFF6.bits.N22 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF6.u32)), DATE_COEFF6.u32);

            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetMvNxValue(), Select Wrong Number of Macrovision Coefficient!\n");

            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

#endif
#ifndef __DISP_PLATFORM_BOOT__

/* set tt line enable */
HI_BOOL  SDATE_SetTtLineEn(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF7 DATE_COEFF7;

    if(u32Data > SDATE_TTLINE_NULL)
    {
        HI_PRINT("SDATE_SetTtLineEn(), Select Wrong Field!\n");

        return HI_FALSE;
    }



    DATE_COEFF7.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF7.u32)));

    if(true == u32Data1)
    {
        DATE_COEFF7.u32 =   DATE_COEFF7.u32  || u32Data;
    }
    else if(false == u32Data1)
    {
        DATE_COEFF7.u32 =   DATE_COEFF7.u32 && (~u32Data);
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF7.u32)), DATE_COEFF7.u32);

    return HI_TRUE;
}

#endif
/* set TT address */
HI_BOOL  SDATE_SetTtAddr(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF8 DATE_COEFF8;
    volatile U_DATE_COEFF9 DATE_COEFF9;

    // set start address of TT
    DATE_COEFF8.u32 = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF8.u32)), DATE_COEFF8.u32);

    // set end address of TT
    DATE_COEFF9.u32 = u32Data1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF9.u32)), DATE_COEFF9.u32);

    return HI_TRUE;
}


/*  get TT ready */
HI_U32 SDATE_GetTtRdy()
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    return ((HI_U32)(DATE_COEFF10.bits.tt_ready));
}

/*  set TT ready */
HI_BOOL  SDATE_SetTtRdy()
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.tt_ready = 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}


/*  set TT u32Data */
HI_BOOL  SDATE_SetTtMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.tt_mode = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}


/*  set TT package offset address */
HI_BOOL  SDATE_SetTtPktOff(HI_U32 u32Data)
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.tt_pktoff = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}


/* set closecaption enable */
HI_BOOL  SDATE_SetCcEn(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF11 DATE_COEFF11;

    DATE_COEFF11.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF11.u32)));
    switch(u32Data)
    {
        case SDATE_ODD_FIELD:
        {
            DATE_COEFF11.bits.cc_enf1 = u32Data1;
            break;
        }
        case SDATE_EVEN_FIELD:
        {
            DATE_COEFF11.bits.cc_enf2 = u32Data1;
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetCcEn() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF11.u32)), DATE_COEFF11.u32);

    return HI_TRUE;
}


/*  set CloseCaption line configuration */
HI_BOOL  SDATE_SetCcLineCfg(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF11 DATE_COEFF11;

    DATE_COEFF11.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF11.u32)));

    switch(u32Data)
    {
        case SDATE_ODD_FIELD:
        {
            DATE_COEFF11.bits.date_clf1 = u32Data1;
            break;
        }
        case SDATE_EVEN_FIELD:
        {
            DATE_COEFF11.bits.date_clf2 = u32Data1;
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetCcLineCfg() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF11.u32)), DATE_COEFF11.u32);

    return HI_TRUE;
}


/*  set CloseCaption data */
HI_BOOL  SDATE_SetCcData(HI_U32 u32Data, HI_U32 u32Data1)
{
    volatile U_DATE_COEFF12 DATE_COEFF12;

    DATE_COEFF12.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF12.u32)));

    switch(u32Data)
    {
        case SDATE_ODD_FIELD:
        {
            DATE_COEFF12.bits.cc_f1data = u32Data1;
            break;
        }
        case SDATE_EVEN_FIELD:
        {
            DATE_COEFF12.bits.cc_f2data = u32Data1;
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetCcData() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF12.u32)), DATE_COEFF12.u32);

    return HI_TRUE;
}



/* set CGMS enable */
HI_BOOL  SDATE_SetCgmsEn(HI_U32 u32Data, HI_U32 u32Data1)
{

    switch(u32Data)
    {
        case SDATE_ODD_FIELD:
        {
            volatile U_DATE_COEFF13 DATE_COEFF13;

            DATE_COEFF13.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)));

            DATE_COEFF13.bits.cg_enf1 = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)), DATE_COEFF13.u32);
            break;
        }
        case SDATE_EVEN_FIELD:
        {
            volatile U_DATE_COEFF13 DATE_COEFF13;

            DATE_COEFF13.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)));

            DATE_COEFF13.bits.cg_enf2 = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)), DATE_COEFF13.u32);
            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetCgmsEn() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }


    return HI_TRUE;
}

/* set CGMS data */
HI_BOOL  SDATE_SetCgmsData(HI_U32 u32Data, HI_U32 u32Data1)
{
    switch(u32Data)
    {
        case SDATE_ODD_FIELD:
        {
          volatile U_DATE_COEFF13 DATE_COEFF13;

            DATE_COEFF13.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)));

            DATE_COEFF13.bits.cg_f1data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF13.u32)), DATE_COEFF13.u32);

            break;
        }
        case SDATE_EVEN_FIELD:
        {
          volatile U_DATE_COEFF14 DATE_COEFF14;

            DATE_COEFF14.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF14.u32)));

            DATE_COEFF14.bits.cg_f2data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF14.u32)), DATE_COEFF14.u32);

            break;
        }
        default:
        {
            HI_PRINT("SDATE_SetCgmsEn() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }


    return HI_TRUE;
}

/*  set WSS enable */
HI_BOOL  SDATE_SetWssEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF15 DATE_COEFF15;

    DATE_COEFF15.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF15.u32)));
    DATE_COEFF15.bits.wss_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF15.u32)), DATE_COEFF15.u32);

    return HI_TRUE;
}


/*  set WSS data */
HI_BOOL  SDATE_SetWssData(HI_U32 u32Data)
{
    volatile U_DATE_COEFF15 DATE_COEFF15;

    DATE_COEFF15.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF15.u32)));
    DATE_COEFF15.bits.wss_data = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF15.u32)), DATE_COEFF15.u32);

    return HI_TRUE;
}


/*  set VPS enable */
HI_BOOL  SDATE_SetVpsEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF16 DATE_COEFF16;

    DATE_COEFF16.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF16.u32)));
    DATE_COEFF16.bits.vps_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF16.u32)), DATE_COEFF16.u32);

    return HI_TRUE;
}


/*  set VPS data */
HI_BOOL  SDATE_SetVpsData(HI_U32 u32Data, HI_U32 u32Data1)
{
    switch(u32Data)
    {
        case SDATE_VPS_DATA_0:
        {
            volatile U_DATE_COEFF16 DATE_COEFF16;

            DATE_COEFF16.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF16.u32)));
            DATE_COEFF16.bits.vps_data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF16.u32)), DATE_COEFF16.u32);

            break;
        }
        case SDATE_VPS_DATA_1:
        {
            volatile U_DATE_COEFF17 DATE_COEFF17;

            DATE_COEFF17.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF17.u32)));
            DATE_COEFF17.bits.vps_data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF17.u32)), DATE_COEFF17.u32);

            break;
        }
        case SDATE_VPS_DATA_2:
        {
            volatile U_DATE_COEFF18 DATE_COEFF18;

            DATE_COEFF18.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF18.u32)));
            DATE_COEFF18.bits.vps_data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF18.u32)), DATE_COEFF18.u32);

            break;
        }
        case SDATE_VPS_DATA_3:
        {
            volatile U_DATE_COEFF19 DATE_COEFF19;

            DATE_COEFF19.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF19.u32)));
            DATE_COEFF19.bits.vps_data = u32Data1;

            VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF19.u32)), DATE_COEFF19.u32);

            break;
        }

        default:
        {
            HI_PRINT("SDATE_SetVpsData() Set Wrong Field!\n");

            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


//add by l67259
HI_BOOL SDATE_SetVbiLpfEn(HI_U32  u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.vbi_lpf_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetFmMode(HI_U32  u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.fm_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetTtFullPage(HI_U32  u32Data)
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.full_page = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}

HI_BOOL SDATE_SetTtPrio(HI_U32  u32Data)
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.tt_highest = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}


HI_BOOL SDATE_SetNabts(HI_U32  u32Data)
{
    volatile U_DATE_COEFF10 DATE_COEFF10;

    DATE_COEFF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)));
    DATE_COEFF10.bits.nabts_100ire = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF10.u32)), DATE_COEFF10.u32);

    return HI_TRUE;
}


HI_BOOL SDATE_SetOvsCoef(HI_U32 u32Data, HI_S32 * psts32Data)
{
    if(u32Data == 0)
    {
        volatile U_DATE_COEFF50 DATE_COEFF50;
        volatile U_DATE_COEFF51 DATE_COEFF51;
        volatile U_DATE_COEFF52 DATE_COEFF52;

        DATE_COEFF50.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF50.u32)));
        DATE_COEFF51.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF51.u32)));
        DATE_COEFF52.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF52.u32)));

        DATE_COEFF50.bits.ovs_coeff0 = psts32Data[0];
        DATE_COEFF50.bits.ovs_coeff1 = psts32Data[1];
        DATE_COEFF51.bits.ovs_coeff0 = psts32Data[2];
        DATE_COEFF51.bits.ovs_coeff1 = psts32Data[3];
        DATE_COEFF52.bits.ovs_coeff0 = psts32Data[4];
        DATE_COEFF52.bits.ovs_coeff1 = psts32Data[5];

        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF50.u32)), DATE_COEFF50.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF51.u32)), DATE_COEFF51.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF52.u32)), DATE_COEFF52.u32);
    }
    else
    {
        volatile U_DATE_COEFF53 DATE_COEFF53;
        volatile U_DATE_COEFF54 DATE_COEFF54;
        volatile U_DATE_COEFF55 DATE_COEFF55;

        DATE_COEFF53.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF53.u32)));
        DATE_COEFF54.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF54.u32)));
        DATE_COEFF55.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF55.u32)));

        DATE_COEFF53.bits.ovs_coeff0 = psts32Data[0];
        DATE_COEFF53.bits.ovs_coeff1 = psts32Data[1];
        DATE_COEFF54.bits.ovs_coeff0 = psts32Data[2];
        DATE_COEFF54.bits.ovs_coeff1 = psts32Data[3];
        DATE_COEFF55.bits.ovs_coeff0 = psts32Data[4];
        DATE_COEFF55.bits.ovs_coeff1 = psts32Data[5];

        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF53.u32)), DATE_COEFF53.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF54.u32)), DATE_COEFF54.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF55.u32)), DATE_COEFF55.u32);
    }

    return HI_TRUE  ;
}

/***********************************SDATE END*******************************************/

/***********************************HDATE*******************************************/
HI_BOOL HDATE_SetOutEn(HI_U32  u32Data)
{
    volatile U_HDATE_EN HDATE_EN;

    HDATE_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_EN.u32)));
    HDATE_EN.bits.hd_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_EN.u32)), HDATE_EN.u32);

    return HI_TRUE;
}


HI_BOOL HDATE_SetSyncPola(HI_U32  u32Data, HI_U32 u32Data1)
{
    volatile U_HDATE_POLA_CTRL HDATE_POLA_CTRL;
    if(u32Data >= HDATE_SYNCMODE_NULL)
    {
        HI_PRINT("HDATE_SetSyncSel() , Select Wrong Type!\n");
    }



    HDATE_POLA_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_POLA_CTRL.u32)));

    switch(u32Data)
    {
        case HDATE_SYNCMODE_HSYNC_IN:
        {
            HDATE_POLA_CTRL.bits.hsync_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_HSYNC_OUT:
        {
            HDATE_POLA_CTRL.bits.hsync_out_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_VSYNC_IN:
        {
            HDATE_POLA_CTRL.bits.vsync_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_VSYNC_OUT:
        {
            HDATE_POLA_CTRL.bits.vsync_out_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_FID_IN:
        {
            HDATE_POLA_CTRL.bits.fid_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_FID_OUT:
        {
            HDATE_POLA_CTRL.bits.fid_out_pola = u32Data1;
            break;
        }
    default:
        {
        HI_PRINT("HDATE_SetSyncPola() , Select Wrong Type!\n");
        }
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_POLA_CTRL.u32)), HDATE_POLA_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetSyncMode(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
    if(u32Data < HDATE_SYNCCHN_RGB)
    {
        HI_PRINT("HDATE_SetSyncMode() , Select Wrong Type!\n");
    }



    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));

    HDATE_VIDEO_FORMAT.bits.video_out_ctrl = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}
#ifndef __DISP_PLATFORM_BOOT__

HI_BOOL HDATE_SetSyncChn(HI_U32  u32Data, HI_U32 u32Data1)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(u32Data >= HDATE_SYNCCHN_RGB)
    {
        HI_PRINT("HDATE_SetSyncChn() , Select Wrong Type!\n");
    }


    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));

    if(false == u32Data1)
    {
        HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = HDATE_VIDEO_FORMAT.bits.sync_add_ctrl || u32Data;
    }
    else if(true == u32Data1)
    {
        HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = HDATE_VIDEO_FORMAT.bits.sync_add_ctrl && (~u32Data);
    }


    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}
#endif
HI_BOOL HDATE_SetOutFmt(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.video_ft = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscMode(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(u32Data >= HDATE_CSCMODE_NULL)
    {
        HI_PRINT("HDATE_SetCscMode() , Select Wrong Type!\n");
    }


    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.csc_ctrl = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscRodEn(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.csc_round_disable = ~u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscCoeff(HI_U32 * psts32Data)
{
    volatile U_HDATE_CSC_COEF1 HDATE_CSC_COEF1;
    volatile U_HDATE_CSC_COEF2 HDATE_CSC_COEF2;
    volatile U_HDATE_CSC_COEF3 HDATE_CSC_COEF3;
    volatile U_HDATE_CSC_COEF4 HDATE_CSC_COEF4;
    volatile U_HDATE_CSC_COEF5 HDATE_CSC_COEF5;

    HDATE_CSC_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CSC_COEF1.u32)));
    HDATE_CSC_COEF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CSC_COEF2.u32)));
    HDATE_CSC_COEF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CSC_COEF3.u32)));
    HDATE_CSC_COEF4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CSC_COEF4.u32)));
    HDATE_CSC_COEF5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CSC_COEF5.u32)));

    HDATE_CSC_COEF1.bits.csc_coef_r_y  = psts32Data[0];
    HDATE_CSC_COEF1.bits.csc_coef_r_cb = psts32Data[1];
    HDATE_CSC_COEF2.bits.csc_coef_r_cr = psts32Data[2];

    HDATE_CSC_COEF2.bits.csc_coef_g_y  = psts32Data[3];
    HDATE_CSC_COEF3.bits.csc_coef_g_cb = psts32Data[4];
    HDATE_CSC_COEF3.bits.csc_coef_g_cr = psts32Data[5];

    HDATE_CSC_COEF4.bits.csc_coef_b_y  = psts32Data[6];
    HDATE_CSC_COEF4.bits.csc_coef_b_cb = psts32Data[7];
    HDATE_CSC_COEF5.bits.csc_coef_b_cr = psts32Data[8];

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CSC_COEF1.u32)), HDATE_CSC_COEF1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CSC_COEF2.u32)), HDATE_CSC_COEF2.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CSC_COEF3.u32)), HDATE_CSC_COEF3.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CSC_COEF4.u32)), HDATE_CSC_COEF4.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CSC_COEF5.u32)), HDATE_CSC_COEF5.u32);

    return HI_TRUE;
}


HI_BOOL HDATE_SetOutSel(HI_U32  u32Data,HI_U32 u32Data1)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)));

    switch(u32Data)
    {
        case HDATE_OUTCHN_VID1:
        {
            HDATE_OUT_CTRL.bits.video1_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VID2:
        {
            HDATE_OUT_CTRL.bits.video2_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VID3:
        {
            HDATE_OUT_CTRL.bits.video3_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_HSYNC:
        {
            HDATE_OUT_CTRL.bits.hsync_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VSYNC:
        {
            HDATE_OUT_CTRL.bits.vsync_sel = u32Data1;
            break;
        }
        default:
        {
        HI_PRINT("HDATE_SetOutSel() , Select Wrong Type!\n");
        }
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}
#endif

HI_VOID  VDP_DISP_SetTiming(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S *pstSyncInfo)

{
     U_DHD0_CTRL DHD0_CTRL;
     U_DHD0_VSYNC DHD0_VSYNC;
     U_DHD0_VPLUS DHD0_VPLUS;
     U_DHD0_PWR DHD0_PWR;
     U_DHD0_HSYNC1 DHD0_HSYNC1;
     U_DHD0_HSYNC2 DHD0_HSYNC2;


    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_OpenIntf Select Wrong CHANNEL ID\n");
        return ;
    }


        //VOU VHD CHANNEL enable
        DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
        DHD0_CTRL.bits.iop   = pstSyncInfo->bIop;//
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);


        DHD0_HSYNC1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET));
        DHD0_HSYNC2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET));
/*
        DHD0_HSYNC1.bits.hact = pstSyncInfo->u32Hact -1;
        DHD0_HSYNC1.bits.hbb  = pstSyncInfo->u32Hbb -1;
        DHD0_HSYNC2.bits.hfb  = pstSyncInfo->u32Hfb -1;
*/
        //if (pstSyncInfo->u32Intfb)
        {
            DHD0_HSYNC1.bits.hact = pstSyncInfo->u32Hact -1;
            DHD0_HSYNC1.bits.hbb  = pstSyncInfo->u32Hbb -1;
            DHD0_HSYNC2.bits.hfb  = pstSyncInfo->u32Hfb -1;
            DHD0_HSYNC2.bits.hmid = pstSyncInfo->u32Hmid -1;
        }
#if 0
        else
        {
            DHD0_HSYNC1.bits.hact = pstSyncInfo->u32Hact * 2 -1;
            DHD0_HSYNC1.bits.hbb  = pstSyncInfo->u32Hbb * 2 -1;
            DHD0_HSYNC2.bits.hfb  = pstSyncInfo->u32Hfb * 2 -1;
            DHD0_HSYNC2.bits.hmid = pstSyncInfo->u32Hmid * 2 -1;
        }
#endif
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC1.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC1.u32);
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_HSYNC2.u32)+u32hd_id*CHN_OFFSET), DHD0_HSYNC2.u32);

        //Config VHD interface veritical timming
        DHD0_VSYNC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VSYNC.bits.vact = pstSyncInfo->u32Vact  -1;
        DHD0_VSYNC.bits.vbb = pstSyncInfo->u32Vbb - 1;
        DHD0_VSYNC.bits.vfb =  pstSyncInfo->u32Vfb - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VSYNC.u32)+u32hd_id*CHN_OFFSET), DHD0_VSYNC.u32);

        //Config VHD interface veritical bottom timming,no use in progressive mode
        DHD0_VPLUS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET));
        DHD0_VPLUS.bits.bvact = pstSyncInfo->u32Bvact - 1;
        DHD0_VPLUS.bits.bvbb =  pstSyncInfo->u32Bvbb - 1;
        DHD0_VPLUS.bits.bvfb =  pstSyncInfo->u32Bvfb - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_VPLUS.u32)+u32hd_id*CHN_OFFSET), DHD0_VPLUS.u32);

        //Config VHD interface veritical bottom timming,
        DHD0_PWR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET));
        DHD0_PWR.bits.hpw = pstSyncInfo->u32Hpw - 1;
        DHD0_PWR.bits.vpw = pstSyncInfo->u32Vpw - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_PWR.u32)+u32hd_id*CHN_OFFSET), DHD0_PWR.u32);
}

HI_VOID VDP_DISP_GetVactState(HI_U32 u32hd_id, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt)
{
    U_DHD0_STATE DHD0_STATE;

    if(u32hd_id >= CHN_MAX)
    {
        HI_PRINT("Error,VDP_DISP_SetGammaEnable Select Wrong CHANNEL ID\n");
        return ;
    }

    //Set Vou Dhd Channel Gamma Correct Enable
    DHD0_STATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_STATE.u32)+u32hd_id*CHN_OFFSET));
    *pbBtm   = DHD0_STATE.bits.bottom_field;
    *pu32Vcnt = DHD0_STATE.bits.vcnt;

    return;
}

#ifndef __DISP_PLATFORM_BOOT__
HI_BOOL HDATE_SetInSrcMode(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.sd_sel = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetInSrcOsMode(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.src_ctrl = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetLpfEn(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.sync_lpf_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetTestData(HI_U32  u32Data)
{
    volatile U_HDATE_TEST HDATE_TEST;

    HDATE_TEST.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_TEST.u32)));
    HDATE_TEST.bits.test_data = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_TEST.u32)), HDATE_TEST.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetVbiLpfEn(HI_U32  u32Data)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)));
    HDATE_VBI_CTRL.bits.vbi_lpf_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCgmsEn(HI_U32  u32Data,HI_U32 u32Data1)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)));

    if(u32Data  == HDATE_CGMS_TYPEA)
    {
        HDATE_VBI_CTRL.bits.cgmsa_add_en = u32Data1;
    }
    else
    {
        HDATE_VBI_CTRL.bits.cgmsb_add_en = u32Data1;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCgmsData(HI_U32  u32Data,HI_U32 u32Data1)
{
    switch(u32Data)
    {
        case HDATE_CGMSA_DATA:
        {
            volatile U_HDATE_CGMSA_DATA HDATE_CGMSA_DATA;
            HDATE_CGMSA_DATA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSA_DATA.u32)));

            HDATE_CGMSA_DATA.bits.cgmsa_data = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSA_DATA.u32)), HDATE_CGMSA_DATA.u32);

            break;
        }
        case HDATE_CGMSB_HEAD:
        {
            volatile U_HDATE_CGMSB_H HDATE_CGMSB_H;
            HDATE_CGMSB_H.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSB_H.u32)));

            HDATE_CGMSB_H.bits.hdate_cgmsb_h = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSB_H.u32)), HDATE_CGMSB_H.u32);

            break;
        }
        case HDATE_CGMSB_DATA1:
        {
            volatile U_HDATE_CGMSB_DATA1 HDATE_CGMSB_DATA1;
            HDATE_CGMSB_DATA1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA1.u32)));

            HDATE_CGMSB_DATA1.bits.cgmsb_data1 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA1.u32)), HDATE_CGMSB_DATA1.u32);

            break;
        }
        case HDATE_CGMSB_DATA2:
        {
            volatile U_HDATE_CGMSB_DATA2 HDATE_CGMSB_DATA2;
            HDATE_CGMSB_DATA2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA2.u32)));

            HDATE_CGMSB_DATA2.bits.cgmsb_data2 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA2.u32)), HDATE_CGMSB_DATA2.u32);

            break;
        }
        case HDATE_CGMSB_DATA3:
        {
            volatile U_HDATE_CGMSB_DATA3 HDATE_CGMSB_DATA3;
            HDATE_CGMSB_DATA3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA3.u32)));

            HDATE_CGMSB_DATA3.bits.cgmsb_data3 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA3.u32)), HDATE_CGMSB_DATA3.u32);

            break;
        }
        case HDATE_CGMSB_DATA4:
        {
            volatile U_HDATE_CGMSB_DATA4 HDATE_CGMSB_DATA4;
            HDATE_CGMSB_DATA4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA4.u32)));

            HDATE_CGMSB_DATA4.bits.cgmsb_data4 = u32Data1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_CGMSB_DATA4.u32)), HDATE_CGMSB_DATA4.u32);

            break;
        }
    default:
        {
        HI_PRINT("HDATE_SetCgmsData() , Select Wrong Type!\n");
        }
    }



    return HI_TRUE;
}

HI_BOOL HDATE_SetMvEn(HI_U32  u32Data)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)));
    HDATE_VBI_CTRL.bits.mv_en = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetOvsCoef(HI_S32 * psts32Data)
{
    volatile U_HDATE_SRC_13_COEF1   HDATE_SRC_13_COEF1;
    volatile U_HDATE_SRC_13_COEF2   HDATE_SRC_13_COEF2;
    volatile U_HDATE_SRC_13_COEF3   HDATE_SRC_13_COEF3;
    volatile U_HDATE_SRC_13_COEF4   HDATE_SRC_13_COEF4;
    volatile U_HDATE_SRC_13_COEF5   HDATE_SRC_13_COEF5;
    volatile U_HDATE_SRC_13_COEF6   HDATE_SRC_13_COEF6;
    volatile U_HDATE_SRC_13_COEF7   HDATE_SRC_13_COEF7;
    volatile U_HDATE_SRC_13_COEF8   HDATE_SRC_13_COEF8;
    volatile U_HDATE_SRC_13_COEF9   HDATE_SRC_13_COEF9;
    volatile U_HDATE_SRC_13_COEF10  HDATE_SRC_13_COEF10;
    volatile U_HDATE_SRC_13_COEF11  HDATE_SRC_13_COEF11;
    volatile U_HDATE_SRC_13_COEF12  HDATE_SRC_13_COEF12;
    volatile U_HDATE_SRC_13_COEF13  HDATE_SRC_13_COEF13;
    volatile U_HDATE_SRC_13_COEF14  HDATE_SRC_13_COEF14;
    volatile U_HDATE_SRC_13_COEF15  HDATE_SRC_13_COEF15;
    volatile U_HDATE_SRC_13_COEF16  HDATE_SRC_13_COEF16;
    volatile U_HDATE_SRC_13_COEF17  HDATE_SRC_13_COEF17;
    volatile U_HDATE_SRC_13_COEF18  HDATE_SRC_13_COEF18;

    volatile U_HDATE_SRC_24_COEF1   HDATE_SRC_24_COEF1;
    volatile U_HDATE_SRC_24_COEF2   HDATE_SRC_24_COEF2;
    volatile U_HDATE_SRC_24_COEF3   HDATE_SRC_24_COEF3;
    volatile U_HDATE_SRC_24_COEF4   HDATE_SRC_24_COEF4;
    volatile U_HDATE_SRC_24_COEF5   HDATE_SRC_24_COEF5;
    volatile U_HDATE_SRC_24_COEF6   HDATE_SRC_24_COEF6;
    volatile U_HDATE_SRC_24_COEF7   HDATE_SRC_24_COEF7;
    volatile U_HDATE_SRC_24_COEF8   HDATE_SRC_24_COEF8;
    volatile U_HDATE_SRC_24_COEF9   HDATE_SRC_24_COEF9;
    volatile U_HDATE_SRC_24_COEF10  HDATE_SRC_24_COEF10;
    volatile U_HDATE_SRC_24_COEF11  HDATE_SRC_24_COEF11;
    volatile U_HDATE_SRC_24_COEF12  HDATE_SRC_24_COEF12;
    volatile U_HDATE_SRC_24_COEF13  HDATE_SRC_24_COEF13;
    volatile U_HDATE_SRC_24_COEF14  HDATE_SRC_24_COEF14;
    volatile U_HDATE_SRC_24_COEF15  HDATE_SRC_24_COEF15;
    volatile U_HDATE_SRC_24_COEF16  HDATE_SRC_24_COEF16;
    volatile U_HDATE_SRC_24_COEF17  HDATE_SRC_24_COEF17;
    volatile U_HDATE_SRC_24_COEF18  HDATE_SRC_24_COEF18;

    HDATE_SRC_13_COEF1.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF1.u32 )));
    HDATE_SRC_13_COEF2.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF2.u32 )));
    HDATE_SRC_13_COEF3.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF3.u32 )));
    HDATE_SRC_13_COEF4.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF4.u32 )));
    HDATE_SRC_13_COEF5.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF5.u32 )));
    HDATE_SRC_13_COEF6.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF6.u32 )));
    HDATE_SRC_13_COEF7.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF7.u32 )));
    HDATE_SRC_13_COEF8.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF8.u32 )));
    HDATE_SRC_13_COEF9.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF9.u32 )));
    HDATE_SRC_13_COEF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF10.u32)));
    HDATE_SRC_13_COEF11.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF11.u32)));
    HDATE_SRC_13_COEF12.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF12.u32)));
    HDATE_SRC_13_COEF13.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF13.u32)));
    HDATE_SRC_13_COEF14.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF14.u32)));
    HDATE_SRC_13_COEF15.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF15.u32)));
    HDATE_SRC_13_COEF16.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF16.u32)));
    HDATE_SRC_13_COEF17.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF17.u32)));
    HDATE_SRC_13_COEF18.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF18.u32)));

    HDATE_SRC_24_COEF1.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF1.u32 )));
    HDATE_SRC_24_COEF2.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF2.u32 )));
    HDATE_SRC_24_COEF3.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF3.u32 )));
    HDATE_SRC_24_COEF4.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF4.u32 )));
    HDATE_SRC_24_COEF5.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF5.u32 )));
    HDATE_SRC_24_COEF6.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF6.u32 )));
    HDATE_SRC_24_COEF7.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF7.u32 )));
    HDATE_SRC_24_COEF8.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF8.u32 )));
    HDATE_SRC_24_COEF9.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF9.u32 )));
    HDATE_SRC_24_COEF10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF10.u32)));
    HDATE_SRC_24_COEF11.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF11.u32)));
    HDATE_SRC_24_COEF12.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF12.u32)));
    HDATE_SRC_24_COEF13.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF13.u32)));
    HDATE_SRC_24_COEF14.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF14.u32)));
    HDATE_SRC_24_COEF15.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF15.u32)));
    HDATE_SRC_24_COEF16.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF16.u32)));
    HDATE_SRC_24_COEF17.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF17.u32)));
    HDATE_SRC_24_COEF18.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF18.u32)));

    HDATE_SRC_13_COEF1.bits.coef_tap1_1   = psts32Data[( 1-1)*4+0];
    HDATE_SRC_13_COEF2.bits.coef_tap2_1   = psts32Data[( 2-1)*4+0];
    HDATE_SRC_13_COEF3.bits.coef_tap3_1   = psts32Data[( 3-1)*4+0];
    HDATE_SRC_13_COEF4.bits.coef_tap4_1   = psts32Data[( 4-1)*4+0];
    HDATE_SRC_13_COEF5.bits.coef_tap5_1   = psts32Data[( 5-1)*4+0];
    HDATE_SRC_13_COEF6.bits.coef_tap6_1   = psts32Data[( 6-1)*4+0];
    HDATE_SRC_13_COEF7.bits.coef_tap7_1   = psts32Data[( 7-1)*4+0];
    HDATE_SRC_13_COEF8.bits.coef_tap8_1   = psts32Data[( 8-1)*4+0];
    HDATE_SRC_13_COEF9.bits.coef_tap9_1   = psts32Data[( 9-1)*4+0];
    HDATE_SRC_13_COEF10.bits.coef_tap10_1 = psts32Data[(10-1)*4+0];
    HDATE_SRC_13_COEF11.bits.coef_tap11_1 = psts32Data[(11-1)*4+0];
    HDATE_SRC_13_COEF12.bits.coef_tap12_1 = psts32Data[(12-1)*4+0];
    HDATE_SRC_13_COEF13.bits.coef_tap13_1 = psts32Data[(13-1)*4+0];
    HDATE_SRC_13_COEF14.bits.coef_tap14_1 = psts32Data[(14-1)*4+0];
    HDATE_SRC_13_COEF15.bits.coef_tap15_1 = psts32Data[(15-1)*4+0];
    HDATE_SRC_13_COEF16.bits.coef_tap16_1 = psts32Data[(16-1)*4+0];
    HDATE_SRC_13_COEF17.bits.coef_tap17_1 = psts32Data[(17-1)*4+0];
    HDATE_SRC_13_COEF18.bits.coef_tap18_1 = psts32Data[(18-1)*4+0];

    HDATE_SRC_24_COEF1.bits.coef_tap1_2   = psts32Data[( 1-1)*4+1];
    HDATE_SRC_24_COEF2.bits.coef_tap2_2   = psts32Data[( 2-1)*4+1];
    HDATE_SRC_24_COEF3.bits.coef_tap3_2   = psts32Data[( 3-1)*4+1];
    HDATE_SRC_24_COEF4.bits.coef_tap4_2   = psts32Data[( 4-1)*4+1];
    HDATE_SRC_24_COEF5.bits.coef_tap5_2   = psts32Data[( 5-1)*4+1];
    HDATE_SRC_24_COEF6.bits.coef_tap6_2   = psts32Data[( 6-1)*4+1];
    HDATE_SRC_24_COEF7.bits.coef_tap7_2   = psts32Data[( 7-1)*4+1];
    HDATE_SRC_24_COEF8.bits.coef_tap8_2   = psts32Data[( 8-1)*4+1];
    HDATE_SRC_24_COEF9.bits.coef_tap9_2   = psts32Data[( 9-1)*4+1];
    HDATE_SRC_24_COEF10.bits.coef_tap10_2 = psts32Data[(10-1)*4+1];
    HDATE_SRC_24_COEF11.bits.coef_tap11_2 = psts32Data[(11-1)*4+1];
    HDATE_SRC_24_COEF12.bits.coef_tap12_2 = psts32Data[(12-1)*4+1];
    HDATE_SRC_24_COEF13.bits.coef_tap13_2 = psts32Data[(13-1)*4+1];
    HDATE_SRC_24_COEF14.bits.coef_tap14_2 = psts32Data[(14-1)*4+1];
    HDATE_SRC_24_COEF15.bits.coef_tap15_2 = psts32Data[(15-1)*4+1];
    HDATE_SRC_24_COEF16.bits.coef_tap16_2 = psts32Data[(16-1)*4+1];
    HDATE_SRC_24_COEF17.bits.coef_tap17_2 = psts32Data[(17-1)*4+1];
    HDATE_SRC_24_COEF18.bits.coef_tap18_2 = psts32Data[(18-1)*4+1];

    HDATE_SRC_13_COEF1.bits.coef_tap1_3   = psts32Data[( 1-1)*4+2];
    HDATE_SRC_13_COEF2.bits.coef_tap2_3   = psts32Data[( 2-1)*4+2];
    HDATE_SRC_13_COEF3.bits.coef_tap3_3   = psts32Data[( 3-1)*4+2];
    HDATE_SRC_13_COEF4.bits.coef_tap4_3   = psts32Data[( 4-1)*4+2];
    HDATE_SRC_13_COEF5.bits.coef_tap5_3   = psts32Data[( 5-1)*4+2];
    HDATE_SRC_13_COEF6.bits.coef_tap6_3   = psts32Data[( 6-1)*4+2];
    HDATE_SRC_13_COEF7.bits.coef_tap7_3   = psts32Data[( 7-1)*4+2];
    HDATE_SRC_13_COEF8.bits.coef_tap8_3   = psts32Data[( 8-1)*4+2];
    HDATE_SRC_13_COEF9.bits.coef_tap9_3   = psts32Data[( 9-1)*4+2];
    HDATE_SRC_13_COEF10.bits.coef_tap10_3 = psts32Data[(10-1)*4+2];
    HDATE_SRC_13_COEF11.bits.coef_tap11_3 = psts32Data[(11-1)*4+2];
    HDATE_SRC_13_COEF12.bits.coef_tap12_3 = psts32Data[(12-1)*4+2];
    HDATE_SRC_13_COEF13.bits.coef_tap13_3 = psts32Data[(13-1)*4+2];
    HDATE_SRC_13_COEF14.bits.coef_tap14_3 = psts32Data[(14-1)*4+2];
    HDATE_SRC_13_COEF15.bits.coef_tap15_3 = psts32Data[(15-1)*4+2];
    HDATE_SRC_13_COEF16.bits.coef_tap16_3 = psts32Data[(16-1)*4+2];
    HDATE_SRC_13_COEF17.bits.coef_tap17_3 = psts32Data[(17-1)*4+2];
    HDATE_SRC_13_COEF18.bits.coef_tap18_3 = psts32Data[(18-1)*4+2];

    HDATE_SRC_24_COEF1.bits.coef_tap1_4   = psts32Data[( 1-1)*4+3];
    HDATE_SRC_24_COEF2.bits.coef_tap2_4   = psts32Data[( 2-1)*4+3];
    HDATE_SRC_24_COEF3.bits.coef_tap3_4   = psts32Data[( 3-1)*4+3];
    HDATE_SRC_24_COEF4.bits.coef_tap4_4   = psts32Data[( 4-1)*4+3];
    HDATE_SRC_24_COEF5.bits.coef_tap5_4   = psts32Data[( 5-1)*4+3];
    HDATE_SRC_24_COEF6.bits.coef_tap6_4   = psts32Data[( 6-1)*4+3];
    HDATE_SRC_24_COEF7.bits.coef_tap7_4   = psts32Data[( 7-1)*4+3];
    HDATE_SRC_24_COEF8.bits.coef_tap8_4   = psts32Data[( 8-1)*4+3];
    HDATE_SRC_24_COEF9.bits.coef_tap9_4   = psts32Data[( 9-1)*4+3];
    HDATE_SRC_24_COEF10.bits.coef_tap10_4 = psts32Data[(10-1)*4+3];
    HDATE_SRC_24_COEF11.bits.coef_tap11_4 = psts32Data[(11-1)*4+3];
    HDATE_SRC_24_COEF12.bits.coef_tap12_4 = psts32Data[(12-1)*4+3];
    HDATE_SRC_24_COEF13.bits.coef_tap13_4 = psts32Data[(13-1)*4+3];
    HDATE_SRC_24_COEF14.bits.coef_tap14_4 = psts32Data[(14-1)*4+3];
    HDATE_SRC_24_COEF15.bits.coef_tap15_4 = psts32Data[(15-1)*4+3];
    HDATE_SRC_24_COEF16.bits.coef_tap16_4 = psts32Data[(16-1)*4+3];
    HDATE_SRC_24_COEF17.bits.coef_tap17_4 = psts32Data[(17-1)*4+3];
    HDATE_SRC_24_COEF18.bits.coef_tap18_4 = psts32Data[(18-1)*4+3];


    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF1.u32 )), HDATE_SRC_13_COEF1.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF2.u32 )), HDATE_SRC_13_COEF2.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF3.u32 )), HDATE_SRC_13_COEF3.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF4.u32 )), HDATE_SRC_13_COEF4.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF5.u32 )), HDATE_SRC_13_COEF5.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF6.u32 )), HDATE_SRC_13_COEF6.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF7.u32 )), HDATE_SRC_13_COEF7.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF8.u32 )), HDATE_SRC_13_COEF8.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF9.u32 )), HDATE_SRC_13_COEF9.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF10.u32)), HDATE_SRC_13_COEF10.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF11.u32)), HDATE_SRC_13_COEF11.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF12.u32)), HDATE_SRC_13_COEF12.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF13.u32)), HDATE_SRC_13_COEF13.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF14.u32)), HDATE_SRC_13_COEF14.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF15.u32)), HDATE_SRC_13_COEF15.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF16.u32)), HDATE_SRC_13_COEF16.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF17.u32)), HDATE_SRC_13_COEF17.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_13_COEF18.u32)), HDATE_SRC_13_COEF18.u32);

    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF1.u32 )), HDATE_SRC_24_COEF1.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF2.u32 )), HDATE_SRC_24_COEF2.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF3.u32 )), HDATE_SRC_24_COEF3.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF4.u32 )), HDATE_SRC_24_COEF4.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF5.u32 )), HDATE_SRC_24_COEF5.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF6.u32 )), HDATE_SRC_24_COEF6.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF7.u32 )), HDATE_SRC_24_COEF7.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF8.u32 )), HDATE_SRC_24_COEF8.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF9.u32 )), HDATE_SRC_24_COEF9.u32 );
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF10.u32)), HDATE_SRC_24_COEF10.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF11.u32)), HDATE_SRC_24_COEF11.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF12.u32)), HDATE_SRC_24_COEF12.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF13.u32)), HDATE_SRC_24_COEF13.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF14.u32)), HDATE_SRC_24_COEF14.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF15.u32)), HDATE_SRC_24_COEF15.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF16.u32)), HDATE_SRC_24_COEF16.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF17.u32)), HDATE_SRC_24_COEF17.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->HDATE_SRC_24_COEF18.u32)), HDATE_SRC_24_COEF18.u32);


    return  HI_TRUE;
}
#endif

HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    PERI_CRG54Tmp.bits.vdp_clk_sel = u32VDPClkMode;
    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;
}

HI_S32 VDP_DISP_SelectChanClkDiv(HI_DRV_DISPLAY_E eChn, HI_U32 u32Div)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;

    if (HI_DRV_DISPLAY_1 == eChn)
    {
        PERI_CRG54Tmp.bits.vo_hd_clk_div = u32Div;
    }
    else
    {
        PERI_CRG54Tmp.bits.vo_sd_clk_div = u32Div;
    }

    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;

    return HI_SUCCESS;
}

HI_VOID DISP_ResetCRG(HI_VOID)
{
    U_PERI_CRG54 unTmpValue;

/*=============================================================================
BIT[   31] = reserved0         <=
BIT[   30] = vou_srst_req      <= 1, soft reset
BIT[   29] = vo_hd_hdmi_clk_sel<= 0,1; 1,1/2
BIT[   28] = vdp_clk_sel       <= 0,300MHZ; 1, 345.6MHZ
BIT[   27] = vo_sd_hdmi_clk_sel<= 0,1; 1,1/2
BIT[   26] = hdmi_clk_sel      <= 0, vo_sd; 1, vo_hd;
BIT[   25] = vo_clkout_pctrl   <= 0, +; 1, -
BIT[   24] = vo_clkout_sel     <= 0, sd; 1, hd
BIT[   23] = vdac_ch3_clk_sel  <= 0, sd; 1, hd
BIT[   22] = vdac_ch2_clk_sel  <= 0, sd; 1, hd
BIT[   21] = vdac_ch1_clk_sel  <= 0, sd; 1, hd
BIT[   20] = vdac_ch0_clk_sel  <= 0, sd; 1, hd
BIT[19-18] = vo_hd_clk_div     <= 00, 1/2; 01, 1/4; 1X, 1
BIT[17-16] = vo_hd_clk_sel     <= 00：sd_ini；01：hd0_ini；10：hd1_ini；11：reserved
BIT[15-14] = vo_sd_clk_div     <= 00, 1/2; 01, 1/4; 1X, 1
BIT[13-12] = vo_sd_clk_sel     <= 00：sd_ini；01：hd0_ini；10：hd1_ini；11：reserved
BIT[   11] = reserved          <=
BIT[   10] = vo_clkout_cken    <= 0,dis; 1, en;
BIT[    9] = vdac_ch3_cken     <= 0,dis; 1, en;
BIT[    8] = vdac_ch2_cken     <= 0,dis; 1, en;
BIT[    7] = vdac_ch1_cken     <= 0,dis; 1, en;
BIT[    6] = vdac_ch0_cken     <= 0,dis; 1, en;
BIT[    5] = vo_hdate_cken     <= 0,dis; 1, en;
BIT[    4] = vo_hd_cken        <= 0,dis; 1, en;
BIT[    3] = vo_sdate_cken     <= 0,dis; 1, en;
BIT[    2] = vo_sd_cken        <= 0,dis; 1, en;
BIT[    1] = vo_cken           <= 0,dis; 1, en;
BIT[    0] = vo_bus_cken       <= 0,dis; 1, en;
=============================================================================*/
    /*open and set clock  reset */
    unTmpValue.u32 = g_pstRegCrg->PERI_CRG54.u32;

    //#define DISP_CV200_ES_SYSCTRL_RESET_VALUE 0x05F147FFUL
    unTmpValue.bits.vo_bus_cken      = 1;
    unTmpValue.bits.vo_cken          = 1;
    unTmpValue.bits.vo_sd_cken       = 1;
    unTmpValue.bits.vo_sdate_cken    = 1;
    unTmpValue.bits.vo_hd_cken       = 1;
    unTmpValue.bits.vo_hdate_cken    = 1;

    unTmpValue.bits.vdac_ch0_cken    = 1;
#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
    unTmpValue.bits.vdac_ch1_cken    = 1;
    unTmpValue.bits.vdac_ch2_cken    = 1;
    unTmpValue.bits.vdac_ch3_cken    = 1;
#endif

    unTmpValue.bits.vo_sd_clk_sel    = 0;
    unTmpValue.bits.vo_sd_clk_div    = 2;
    unTmpValue.bits.vo_hd_clk_sel    = 1;
    unTmpValue.bits.vo_hd_clk_div    = 0;
    unTmpValue.bits.vdac_ch0_clk_sel = 1;
#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
    unTmpValue.bits.vdac_ch1_clk_sel = 1;
    unTmpValue.bits.vdac_ch2_clk_sel = 1;
    unTmpValue.bits.vdac_ch3_clk_sel = 1;
#endif

    unTmpValue.bits.hdmi_clk_sel     = 1;
    unTmpValue.bits.vo_sd_hdmi_clk_sel = 0;

    #if !(defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100))
    unTmpValue.bits.vdp_clk_sel        = 0;
    #else
    unTmpValue.bits.vdp_clk_sel        = 1;
    #endif

    unTmpValue.bits.vo_hd_hdmi_clk_sel = 0;
    unTmpValue.bits.vou_srst_req       = 1;


    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    DISP_MSLEEP(5);
    unTmpValue.bits.vou_srst_req = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    DISP_MSLEEP(1);

    return;
}

HI_VOID VDP_RegSave(HI_U32 u32RegBackAddr)
{
    S_VOU_CV200_REGS_TYPE  *pVdpBackReg;
    pVdpBackReg = (S_VOU_CV200_REGS_TYPE *)u32RegBackAddr;

    /* save Reg */
    memcpy((HI_VOID *)(&(pVdpBackReg->VOCTRL)), (HI_VOID *)(&(pVdpReg->VOCTRL)), 0x100*2);
    memcpy((HI_VOID *)(&(pVdpBackReg->WBC_DHD_LOCATE)), (HI_VOID *)(&(pVdpReg->WBC_DHD_LOCATE)), 0x100);
    memcpy((HI_VOID *)(&(pVdpBackReg->COEF_DATA)), (HI_VOID *)(&(pVdpReg->COEF_DATA)), 0x100);

    /*video layer */
    memcpy((HI_VOID *)(&(pVdpBackReg->V0_CTRL)), (HI_VOID *)(&(pVdpReg->V0_CTRL)), 0x800*5);

    /*VP0 VP1*/
    memcpy((HI_VOID *)(&(pVdpBackReg->VP0_CTRL)), (HI_VOID *)(&(pVdpReg->VP0_CTRL)), 0x800*2);

    /*DWBC0*/
    memcpy((HI_VOID *)(&(pVdpBackReg->WBC_DHD0_CTRL)), (HI_VOID *)(&(pVdpReg->WBC_DHD0_CTRL)), 0x400);

    /*MIXER*/
    memcpy((HI_VOID *)(&(pVdpBackReg->MIXV0_BKG.u32)), (HI_VOID *)(&(pVdpReg->MIXV0_BKG.u32)), 0x500);

    /*DHDx*/
    memcpy((HI_VOID *)(&(pVdpBackReg->DHD0_CTRL)), (HI_VOID *)(&(pVdpReg->DHD0_CTRL)), 0x400*2);

    /*DATE*/
    memcpy((HI_VOID *)(&(pVdpBackReg->HDATE_VERSION)), (HI_VOID *)(&(pVdpReg->HDATE_VERSION)), 0x2dc);


}

#define DISP_FASTBOOTUP_FLAG       0xdeadbeaf
extern HI_U32 Disp_GetFastbootupFlag(HI_VOID);
HI_VOID VDP_RegReStore(HI_U32 u32RegBackAddr)
{

    S_VOU_CV200_REGS_TYPE  *pVdpBackReg;

    pVdpBackReg = (S_VOU_CV200_REGS_TYPE *)u32RegBackAddr;
    /*because we will call DISP_ISR_Resume to resume the isr,se here we do not restore it.*/
    pVdpBackReg->VOINTMSK.u32 = 0;
    /* save Reg */

    if (Disp_GetFastbootupFlag() !=  DISP_FASTBOOTUP_FLAG)
    {
        memcpy((HI_VOID *)(&(pVdpReg->VOCTRL)), (HI_VOID *)(&(pVdpBackReg->VOCTRL)), 0x100*2);
    }
    else
    {
        memcpy((HI_VOID *)(&(pVdpReg->VOCTRL)), (HI_VOID *)(&(pVdpBackReg->VOCTRL)), 0x130);
    }

    memcpy((HI_VOID *)(&(pVdpReg->WBC_DHD_LOCATE)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD_LOCATE)), 0x100);
    memcpy((HI_VOID *)(&(pVdpReg->COEF_DATA)), (HI_VOID *)(&(pVdpBackReg->COEF_DATA)), 0x100);
    /*video layer */
    memcpy((HI_VOID *)(&(pVdpReg->V0_CTRL)), (HI_VOID *)(&(pVdpBackReg->V0_CTRL)), 0x800*5);

    /*VP0 VP1*/
    memcpy((HI_VOID *)(&(pVdpReg->VP0_CTRL)), (HI_VOID *)(&(pVdpBackReg->VP0_CTRL)), 0x800*2);

    /*DWBC0*/
    memcpy((HI_VOID *)(&(pVdpReg->WBC_DHD0_CTRL)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD0_CTRL)), 0x400);

    /*MIXER*/
    memcpy((HI_VOID *)(&(pVdpReg->MIXV0_BKG)), (HI_VOID *)(&(pVdpBackReg->MIXV0_BKG)), 0x500);

    if (Disp_GetFastbootupFlag() !=  DISP_FASTBOOTUP_FLAG)
    {
        /*DHDx*/
        memcpy((HI_VOID *)(&(pVdpReg->DHD0_CTRL)), (HI_VOID *)(&(pVdpBackReg->DHD0_CTRL)), 0x400*2);
    }

    /*DATE*/
    memcpy((HI_VOID *)(&(pVdpReg->HDATE_VERSION)), (HI_VOID *)(&(pVdpBackReg->HDATE_VERSION)), 0x2dc);

}

HI_VOID VDP_CloseClkResetModule(HI_VOID)
{
    U_PERI_CRG54 unTmpValue1;

    g_pstRegCrg->PERI_CRG71.bits.vdac_chop_cken = 0;

    unTmpValue1.u32 = g_pstRegCrg->PERI_CRG54.u32;
    unTmpValue1.bits.vou_srst_req = 1;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;

#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
    unTmpValue1.bits.vdac_ch3_cken = 0;
    unTmpValue1.bits.vdac_ch2_cken = 0;
    unTmpValue1.bits.vdac_ch1_cken = 0;
#endif

    unTmpValue1.bits.vdac_ch0_cken = 0;

    unTmpValue1.bits.vo_hdate_cken = 0;
    unTmpValue1.bits.vo_hd_cken = 0;
    unTmpValue1.bits.vo_sdate_cken = 0;
    unTmpValue1.bits.vo_sd_cken = 0;
    unTmpValue1.bits.vo_cken = 0;
    unTmpValue1.bits.vo_bus_cken = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;
    return ;
}
/***********************************HDATE END*******************************************/

HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH)
{
    *pu32VersionL =  0x76756F76UL;
    *pu32VersionH =  0x30313134UL;
   return HI_SUCCESS;
}

HI_S32 VDP_MVCN_DATE_SetMcvn(HI_U32 *pu32Value)
{
    pVdpReg->DATE_COEFF2.u32 = pu32Value[0];
    pVdpReg->DATE_COEFF3.u32 = pu32Value[1];
    pVdpReg->DATE_COEFF4.u32 = pu32Value[2];
    pVdpReg->DATE_COEFF5.u32 = pu32Value[3];
    pVdpReg->DATE_COEFF6.u32 = pu32Value[4];

    return HI_SUCCESS;
}

HI_VOID VDP_MVCN_SetMcvnEnable(HI_BOOL bEnable)
{
    U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
    HDATE_VBI_CTRL.bits.mv_en = bEnable;
    pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
}

HI_VOID VDP_VBI_SetCgmsTypeAEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeA)
{
	U_HDATE_VBI_CTRL HDATE_VBI_CTRL; 

	U_DATE_COEFF13 DATE_COEFF13;

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
		HDATE_VBI_CTRL.bits.cgmsa_add_en = bEnTypeA;
		pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF13.u32 = pVdpReg->DATE_COEFF13.u32;
		DATE_COEFF13.bits.cg_enf1 = bEnTypeA;
		DATE_COEFF13.bits.cg_enf2 = bEnTypeA;
		pVdpReg->DATE_COEFF13.u32 = DATE_COEFF13.u32;
	}
	else
	{

	}
}

HI_VOID VDP_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data)
{
	U_HDATE_CGMSA_DATA HDATE_CGMSA_DATA;

	U_DATE_COEFF13	DATE_COEFF13;

	U_DATE_COEFF14	DATE_COEFF14;

	VDP_VBI_SetCgmsTypeAEn(enChanel,HI_FALSE);

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_CGMSA_DATA.u32 = pVdpReg->HDATE_CGMSA_DATA.u32;
		HDATE_CGMSA_DATA.bits.cgmsa_data = u32Data;
		pVdpReg->HDATE_CGMSA_DATA.u32 = HDATE_CGMSA_DATA.u32;
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF13.u32 = pVdpReg->DATE_COEFF13.u32;
		DATE_COEFF13.bits.cg_f1data = u32Data;
		pVdpReg->DATE_COEFF13.u32 = DATE_COEFF13.u32;

		DATE_COEFF14.u32 = pVdpReg->DATE_COEFF14.u32;
		DATE_COEFF14.bits.cg_f2data = u32Data;
		pVdpReg->DATE_COEFF14.u32 = DATE_COEFF14.u32;
	}

	VDP_VBI_SetCgmsTypeAEn(enChanel,HI_TRUE);
}

HI_VOID VDP_VBI_SetCgmsTypeBEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeB)
{
	U_HDATE_VBI_CTRL HDATE_VBI_CTRL; 

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
		HDATE_VBI_CTRL.bits.cgmsb_add_en = bEnTypeB;
		pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		HI_PRINT("error\n");
	}
	else
	{
		HI_PRINT("error\n");
	}

}
HI_VOID VDP_VBI_SetCgmsTypeB(DISP_VENC_E enChanel,HI_U32 u32Data)
{

	if (enChanel == DISP_VENC_HDATE0)
	{
	
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
	
	}
	else
	{
	
	}

	HI_PRINT("error\n");
}
