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
//
//
//
//
//**********************************************************************


#include "vdp_drv_comm.h"
#include "vdp_drv_vid.h"
//#include "vdp_fun.h"


#if EDA_TEST
extern Driver *u_drv;
#endif

extern S_VDP_REGS_TYPE *pVdpReg;


HI_VOID  VDP_VID_SetPreReadEnable    (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_PRERD     V0_PRERD;

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

HI_VOID  VDP_VID_SetUvorder(HI_U32 u32Data, HI_U32 u32bUvorder)
{
    U_V0_CTRL V0_CTRL;

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


HI_VOID  VDP_VID_SetTwodt3dEn(HI_U32 u32Data, HI_U32 u32twodt3d_en)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetUvorder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.twodt3d_en= u32twodt3d_en;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}


HI_VOID  VDP_VID_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_CTRL  V0_CTRL;

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
    U_V0_CTRL V0_CTRL;
    U_V0_16REGIONENL V0_16REGIONENL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.surface_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    V0_16REGIONENL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));
    V0_16REGIONENL.u32 = 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);

    return ;
}

HI_VOID  VDP_VID_SetFiLayerEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetFiLayerEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.surface_c_en = u32bEnable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetFiVscale    (HI_U32 u32Data, HI_U32 u32bFiVscale )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error, VDP_VID_SetFiVscale() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.fi_vscale = u32bFiVscale ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    U_V0_VFPOS V0_VFPOS;
    U_V0_VLPOS V0_VLPOS;
    U_V0_DFPOS V0_DFPOS;
    U_V0_DLPOS V0_DLPOS;
    U_V0_IRESO V0_IRESO;
    U_V0_ORESO V0_ORESO;

    //for multi region
    U_V0_P0RESO          V0_P0RESO;
    U_V0_P0VFPOS         V0_P0VFPOS;
    U_V0_P0VLPOS         V0_P0VLPOS;
#if VID_CROP_EN
    U_V0_CPOS          V0_CPOS;
#endif

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
#if VID_CROP_EN

    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = 0;
    V0_CPOS.bits.src_xlpos = stRect.u32IWth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);
#endif

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
    V0_P0RESO.bits.w = stRect.u32OWth- 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET), V0_P0RESO.u32);
    return ;
}

HI_VOID  VDP_VID_SetInDataFmt       (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt)
{
    U_V0_CTRL V0_CTRL;

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

HI_VOID  VDP_VID_SetDataWidth (HI_U32 u32Data, HI_U32 DataWidth)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDataWidth() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.data_width = DataWidth;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetReadMode    (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode)
{
    U_V0_CTRL V0_CTRL;

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

HI_VOID  VDP_VID_SetMuteEnable   (HI_U32 u32Data, HI_U32 bEnable)
{
    U_V0_CTRL V0_CTRL;

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
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetFlipEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET));
    V0_CTRL.bits.flip_en = u32bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET), V0_CTRL.u32);
}

HI_VOID  VDP_VID_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_IRESO V0_IRESO;

#if VID_CROP_EN
    U_V0_CPOS  V0_CPOS;
#endif

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32Wth - 1;
    V0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);

#if VID_CROP_EN
    V0_CPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = 0;
    V0_CPOS.bits.src_xlpos = stRect.u32Wth - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32);
#endif

    return ;
}

HI_VOID  VDP_VID_SetOutReso     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_ORESO V0_ORESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
    V0_ORESO.bits.ow = stRect.u32Wth - 1;
    V0_ORESO.bits.oh = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32);

    return ;
}

HI_VOID  VDP_VID_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
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
    U_V0_DFPOS V0_DFPOS;
    U_V0_DLPOS V0_DLPOS;

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

//#if DCMP_EN
HI_VOID  VDP_VID_SetDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;

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



HI_VOID  VDP_VID_SetDcmpIsLosslessYC(HI_U32 u32Data, HI_U32 u32Y_Lossless, HI_U32 u32C_Lossless)
{
    U_V0_DCMP_CTRL V0_DCMP_CTRL;


    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDcmpOffset() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMP_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_DCMP_CTRL.bits.is_lossless_l = u32Y_Lossless ;
    V0_DCMP_CTRL.bits.is_lossless_c = u32C_Lossless ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET), V0_DCMP_CTRL.u32);
    return ;
}


HI_VOID  VDP_VID_SetDcmpOffset    (HI_U32 u32Data,HI_U32 u32laddr_offset,HI_U32  u32caddr_offset )
{
    U_V0_LADDROFFSET V0_LADDROFFSET;
    U_V0_CADDROFFSET V0_CADDROFFSET;


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
	U_V0_DCMPERRCLR  V0_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDcmpErrorClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMPERRCLR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.dcmp_l_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);

    V0_DCMPERRCLR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.dcmp_c_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);


    return ;
}
//#endif

HI_VOID VDP_VID_SetIfirMode(HI_U32 u32Data, VDP_IFIRMODE_E enMode)
{
    U_V0_CTRL V0_CTRL;

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
    U_V0_IFIRCOEF01 V0_IFIRCOEF01;
    U_V0_IFIRCOEF23 V0_IFIRCOEF23;
    U_V0_IFIRCOEF45 V0_IFIRCOEF45;
    U_V0_IFIRCOEF67 V0_IFIRCOEF67;

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

HI_VOID  VDP_VID_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    U_V0_CBMPARA V0_CBMPARA;
    U_V0_ALPHA V0_ALPHA;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error, VDP_VID_SetLayerGalpha() Select Wrong Video Layer ID\n");
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
#if VID_CROP_EN
    U_V0_CPOS V0_CPOS;

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
#endif
}

HI_VOID  VDP_VID_SetLayerBkg    (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_V0_BK    V0_BK;
    //U_V0_ALPHA V0_ALPHA;

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

    //V0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET));
    //V0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET), V0_ALPHA.u32);

    return ;
}

//modify add
HI_VOID  VDP_VID_SetMuteBkg    (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_V0_MUTE_BK    V0_MUTE_BK;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetMuteBkg() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_MUTE_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_MUTE_BK.u32) + u32Data * VID_OFFSET));
    V0_MUTE_BK.bits.mute_y  = stBkg.u32BkgY;
    V0_MUTE_BK.bits.mute_cb = stBkg.u32BkgU;
    V0_MUTE_BK.bits.mute_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_MUTE_BK.u32) + u32Data * VID_OFFSET), V0_MUTE_BK.u32);

    return ;
}

HI_VOID  VDP_SetParaUpMode(HI_U32 u32Data,HI_U32 u32Mode)
{
    U_V0_CTRL V0_CTRL;

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

HI_VOID  VDP_VID_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    U_V0_CSC_IDC  V0_CSC_IDC;
    U_V0_CSC_ODC  V0_CSC_ODC;
    U_V0_CSC_IODC V0_CSC_IODC;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

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

    return ;
}

HI_VOID   VDP_VID_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{
    U_V0_CSC_P0 V0_CSC_P0;
    U_V0_CSC_P1 V0_CSC_P1;
    U_V0_CSC_P2 V0_CSC_P2;
    U_V0_CSC_P3 V0_CSC_P3;
    U_V0_CSC_P4 V0_CSC_P4;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscCoef Select Wrong video ID\n");
        return ;
    }


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

HI_VOID  VDP_VID_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    U_V0_CSC_IDC V0_CSC_IDC;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
    V0_CSC_IDC.bits.csc_en = u32bCscEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), V0_CSC_IDC.u32);

    return ;
}

HI_VOID VDP_VID_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)

{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;

    HI_U32 u32Pre   = 1 << 10;
    HI_U32 u32DcPre = 4;//1:8bit; 4:10bit

   // if(u32Data == VDP_LAYER_VID3)
   // {
    //    U_V0_CSC_IDC  V0_CSC_IDC;
//
    //    V0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET ));
    //    V0_CSC_IDC.bits.csc_mode = enCscMode;
    //    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET ), V0_CSC_IDC.u32);
    //}

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

            st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
            st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
            st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;

            st_csc_dc_coef.csc_out_dc0 =  0 * u32DcPre;
            st_csc_dc_coef.csc_out_dc1 =  0 * u32DcPre;
            st_csc_dc_coef.csc_out_dc2 =  0 * u32DcPre;
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

            st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
            st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
            st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;

            st_csc_dc_coef.csc_out_dc0 = 0 * u32DcPre;
            st_csc_dc_coef.csc_out_dc1 = 0 * u32DcPre;
            st_csc_dc_coef.csc_out_dc2 = 0 * u32DcPre;
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

            st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
            st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
            st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;

            st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
            st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
            st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre;
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

        VDP_VID_SetCscCoef  (u32Data,st_csc_coef);
        VDP_VID_SetCscDcCoef(u32Data,st_csc_dc_coef);
    return ;
}

HI_VOID VDP_VID_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    //U_V0_CTRL V0_CTRL;
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

HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32ModeLuma, HI_U32 u32ModeChroma)
{
#if VID_DRAW_EN
    U_V0_DRAWMODE V0_DRAWMODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDrawMode() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_DRAWMODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET));
    V0_DRAWMODE.bits.draw_mode   = u32ModeLuma;
    V0_DRAWMODE.bits.draw_mode_c = u32ModeChroma;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET), V0_DRAWMODE.u32);

    return ;
#endif
}
#if 0 //delte in s5v2
HI_VOID VDP_SetTimeOut(HI_U32 u32Data, HI_U32 u32TData)
{
    U_V0_CTRL V0_CTRL;

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
#endif

HI_VOID VDP_VID_SetReqCtrl(HI_U32 u32Data, HI_U32 u32ReqCtrl)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetReqCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.req_ctrl = u32ReqCtrl;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetMultiModeEnable(HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_MULTI_MODE      V0_MULTI_MODE;

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
        VDP_VID_SetLayerEnable(u32Data, 1);
    }
    else
    {
        VDP_VID_SetLayerEnable(u32Data, 0);
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
    U_V0_P0STRIDE V0_P0STRIDE;

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
    U_V0_P0RESO          V0_P0RESO;
    U_V0_P0VFPOS         V0_P0VFPOS;
    U_V0_P0VLPOS         V0_P0VLPOS;

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

HI_VOID VDP_VID_SetRegionMuteEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 bEnable)
{
    U_V0_16MUTE V0_16MUTE;

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
    U_V0_UPD V0_UPD;

    /* VHD layer register update */
    if(u32Data >= VID_MAX)
    {
        //HI_PRINT("Error,VDP_VID_SetRegup() Select Wrong Video Layer ID:%d\n", __LINE__);
        //dump_stack();
        return ;
    }

    V0_UPD.bits.regup = 0x1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_UPD.u32) + u32Data * VID_OFFSET), V0_UPD.u32);

    return ;
}

HI_VOID  VDP_VID_SetZmeEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

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
#if 1
    if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
    {
		V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32)));
        V0_HSP.bits.non_lnr_en = u32bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HSP.u32)), V0_HSP.u32);
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

HI_VOID  VDP_VID_SetZmePhase    (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    U_V0_HLOFFSET  V0_HLOFFSET;
    U_V0_HCOFFSET  V0_HCOFFSET;
    U_V0_VOFFSET   V0_VOFFSET;
    U_V0_VBOFFSET  V0_VBOFFSET;

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
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

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
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

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
HI_VOID VDP_VID_SetZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_HSP V0_HSP;

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


HI_VOID  VDP_VID_SetZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_VSR V0_VSR;

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

HI_VOID VDP_VID_SetZmeHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_V0_HSP V0_HSP;

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

HI_VOID VDP_VID_SetZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    U_V0_HLCOEFAD V0_HLCOEFAD;
    U_V0_HCCOEFAD V0_HCCOEFAD;
    U_V0_VLCOEFAD V0_VLCOEFAD;
    U_V0_VCCOEFAD V0_VCCOEFAD;

    HI_U32 ADDR_OFFSET= 0x100 ;
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
        V0_HCCOEFAD.bits.coef_addr = (u32Addr +ADDR_OFFSET) ;//& 0xfffffff0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET), V0_HCCOEFAD.u32);

    }
    else if(u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET), V0_VLCOEFAD.u32);

        V0_VCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VCCOEFAD.bits.coef_addr = (u32Addr + ADDR_OFFSET) ;// & 0xfffffff0;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET), V0_VCCOEFAD.u32);


    }
    else
    {
        HI_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}
#endif

HI_VOID  VDP_VID_SetZmeInFmt_Define1(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

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
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32);

    return ;
}

HI_VOID  VDP_VID_SetZmeNonLnrDelta(HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta)
{
    U_V0_HZONE0DELTA     V0_HZONE0DELTA;
    U_V0_HZONE2DELTA     V0_HZONE2DELTA;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeNonLnrDelta() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HZONE0DELTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HZONE0DELTA.u32) + u32Data * VID_OFFSET));
    V0_HZONE0DELTA.bits.zone0_delta = s32Zone0Delta;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HZONE0DELTA.u32) + u32Data * VID_OFFSET), V0_HZONE0DELTA.u32);

    V0_HZONE2DELTA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HZONE2DELTA.u32) + u32Data * VID_OFFSET));
    V0_HZONE2DELTA.bits.zone2_delta = s32Zone2Delta;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HZONE2DELTA.u32) + u32Data * VID_OFFSET), V0_HZONE2DELTA.u32);

    return ;
}


HI_VOID  VDP_VID_SetZmeNonLnrZoneEnd(HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End)
{
    U_V0_HZONEEND        V0_HZONEEND;

	if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeNonLnrZoneEnd() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HZONEEND.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HZONEEND.u32) + u32Data * VID_OFFSET));
    V0_HZONEEND.bits.zone0_end = u32Zone0End-1;
    V0_HZONEEND.bits.zone1_end = u32Zone1End-1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HZONEEND.u32) + u32Data * VID_OFFSET), V0_HZONEEND.u32);

    return ;
}




HI_VOID  VDP_VID_SetParaUpd       (HI_U32 u32Data, VDP_VID_PARA_E enMode)
{
    U_V0_PARAUP V0_PARAUP;
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

HI_VOID VDP_VID_SetCoefReadEnable(HI_U32 u32Id, HI_U32 u32Para)
{
    HI_U32 u32WrData;
    u32WrData = 1 << u32Para;

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_PARARD.u32) + u32Id ), u32WrData);
}

HI_VOID VDP_VID_SetCoefReadDisable(HI_U32 u32Id, HI_U32 u32Para)
{
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_PARARD.u32) + u32Id ), 0);
}

//------------------------------------------------------------------
//driver for VP layer
//------------------------------------------------------------------

#if VP0_EN
HI_VOID  VDP_VP_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
    U_VP0_DOF_CTRL  VP0_DOF_CTRL;

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
    U_VP0_DOF_STEP  VP0_DOF_STEP;

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

HI_VOID  VDP_VP_SetThreeDimDofBkg   (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_VP0_DOF_BKG  VP0_DOF_BKG;
    //U_G0_DOF_ALPHA G0_DOF_ALPHA;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetThreeDimDofBkg() Select Wrong Graph Layer ID\n");
        return ;
    }

    VP0_DOF_BKG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DOF_BKG.u32)));
    VP0_DOF_BKG.bits.dof_bk_y  = stBkg.u32BkgY ;
    VP0_DOF_BKG.bits.dof_bk_cb = stBkg.u32BkgU;
    VP0_DOF_BKG.bits.dof_bk_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DOF_BKG.u32)), VP0_DOF_BKG.u32);

    return ;
}


HI_VOID  VDP_VP_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;
    U_VP0_DFPOS VP0_DFPOS;
    U_VP0_DLPOS VP0_DLPOS;
    U_VP0_IRESO VP0_IRESO;

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

    VP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32IWth - 1;
    VP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32);

    return ;
}

HI_VOID  VDP_VP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;

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
    U_VP0_DFPOS VP0_DFPOS;
    U_VP0_DLPOS VP0_DLPOS;

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
    U_VP0_IRESO VP0_IRESO;

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

HI_VOID  VDP_VP_SetRegUp  (HI_U32 u32Data)
{
    U_VP0_UPD VP0_UPD;

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

HI_VOID  VDP_VP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    U_VP0_GALPHA VP0_GALPHA;

    //special for bk alpha = video alpha
    U_VP0_ALPHA     VP0_ALPHA;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetLayerGalpha() Select Wrong vp Layer ID\n");
        return ;
    }


    VP0_GALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_GALPHA.u32) + u32Data * VP_OFFSET));
    VP0_GALPHA.bits.galpha = u32Alpha;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_GALPHA.u32) + u32Data * VP_OFFSET), VP0_GALPHA.u32);


    VP0_ALPHA.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ALPHA.u32)));
    VP0_ALPHA.bits.vbk_alpha = u32Alpha;
    VDP_RegWrite((HI_U32)&(pVdpReg->VP0_ALPHA.u32), VP0_ALPHA.u32);

    return ;
}

HI_VOID  VDP_VP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_VP0_BK VP0_BK;
    //U_VP0_ALPHA     VP0_ALPHA;

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

HI_VOID VDP_VP_SetMuteEnable     (HI_U32 u32Data, HI_U32 bEnable)
{
    U_VP0_CTRL VP0_CTRL;

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

HI_VOID  VDP_VP_SetMuteBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_VP0_MUTE_BK VP0_MUTE_BK;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetMuteBkg() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_MUTE_BK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_MUTE_BK.u32) + u32Data * VP_OFFSET));
    VP0_MUTE_BK.bits.mute_y  = stBkg.u32BkgY;
    VP0_MUTE_BK.bits.mute_cb = stBkg.u32BkgU;
    VP0_MUTE_BK.bits.mute_cr = stBkg.u32BkgV;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_MUTE_BK.u32) + u32Data * VP_OFFSET), VP0_MUTE_BK.u32);

    return ;
}

HI_VOID VDP_VP_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    //U_VP0_CTRL VP0_CTRL;
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


//#if VP0_CSC_EN
#if 0
HI_VOID  VDP_VP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num)
{
    U_VP0_CSC0_IDC  VP0_CSC0_IDC;
    U_VP0_CSC0_ODC  VP0_CSC0_ODC;
    U_VP0_CSC0_IODC VP0_CSC0_IODC;

    U_VP0_CSC1_IDC  VP0_CSC1_IDC;
    U_VP0_CSC1_ODC  VP0_CSC1_ODC;
    U_VP0_CSC1_IODC VP0_CSC1_IODC;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
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
    U_VP0_CSC0_P0 VP0_CSC0_P0;
    U_VP0_CSC0_P1 VP0_CSC0_P1;
    U_VP0_CSC0_P2 VP0_CSC0_P2;
    U_VP0_CSC0_P3 VP0_CSC0_P3;
    U_VP0_CSC0_P4 VP0_CSC0_P4;

    U_VP0_CSC1_P0 VP0_CSC1_P0;
    U_VP0_CSC1_P1 VP0_CSC1_P1;
    U_VP0_CSC1_P2 VP0_CSC1_P2;
    U_VP0_CSC1_P3 VP0_CSC1_P3;
    U_VP0_CSC1_P4 VP0_CSC1_P4;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscCoef Select Wrong video ID\n");
        return ;
    }

    if(u32Num == 0)
    {
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
    U_VP0_CSC0_IDC VP0_CSC0_IDC;
    U_VP0_CSC1_IDC VP0_CSC1_IDC;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        VP0_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32);
    }
    else if(u32Num == 1)
    {
        VP0_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32);
    }

    return ;
}

HI_VOID VDP_VP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode, HI_U32 u32Num)
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

    VDP_VP_SetCscCoef  (u32Data,st_csc_coef, u32Num);
    VDP_VP_SetCscDcCoef(u32Data,st_csc_dc_coef, u32Num);

    return ;
}
#endif

HI_VOID  VDP_VP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
	U_VP0_CSC_IDC  VP0_CSC_IDC;
	U_VP0_CSC_ODC  VP0_CSC_ODC;
	U_VP0_CSC_IODC VP0_CSC_IODC;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

	VP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
	VP0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC_IDC.u32);

	VP0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_ODC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
	VP0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC_ODC.u32);

	VP0_CSC_IODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_IODC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
	VP0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC_IODC.u32);


    return ;
}

HI_VOID   VDP_VP_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{
	U_VP0_CSC_P0 VP0_CSC_P0;
	U_VP0_CSC_P1 VP0_CSC_P1;
	U_VP0_CSC_P2 VP0_CSC_P2;
	U_VP0_CSC_P3 VP0_CSC_P3;
	U_VP0_CSC_P4 VP0_CSC_P4;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscCoef Select Wrong video ID\n");
        return ;
	}

	VP0_CSC_P0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_P0.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
	VP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_P0.u32)+u32Data*VP_OFFSET), VP0_CSC_P0.u32);

	VP0_CSC_P1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_P1.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
	VP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_P1.u32)+u32Data*VP_OFFSET), VP0_CSC_P1.u32);

	VP0_CSC_P2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_P2.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
	VP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_P2.u32)+u32Data*VP_OFFSET), VP0_CSC_P2.u32);

	VP0_CSC_P3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_P3.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
	VP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_P3.u32)+u32Data*VP_OFFSET), VP0_CSC_P3.u32);

	VP0_CSC_P4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_P4.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_P4.u32)+u32Data*VP_OFFSET), VP0_CSC_P4.u32);

}

HI_VOID  VDP_VP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
	U_VP0_CSC_IDC VP0_CSC_IDC;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

	VP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IDC.bits.csc_en = u32bCscEn;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC_IDC.u32);

	return ;
}

HI_VOID VDP_VP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)
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

        st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc0 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 0 * u32DcPre;
        st_csc_dc_coef.csc_out_dc2 = 0 * u32DcPre;
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

        st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
        st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;

        st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre;
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

    VDP_VP_SetCscCoef  (u32Data, st_csc_coef);
    VDP_VP_SetCscDcCoef(u32Data, st_csc_dc_coef);

    return ;
}

HI_VOID VDP_VP_SetAcmEnable (HI_U32 u32Data, HI_U32 u32bAcmEn)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmEnable() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    //modify en
    VP0_ACM_CTRL.bits.acm_en = u32bAcmEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;

}


HI_VOID VDP_VP_SetAcmDbgEn(HI_U32 u32Data , HI_U32 u32bAcmDbgEn)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

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

HI_VOID VDP_VP_SetAcmDbgMode(HI_U32 u32Data , HI_U32 u32bAcmDbgMode)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmDbgEn() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_dbg_mode = u32bAcmDbgMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}
HI_VOID VDP_VP_SetAcmCliporwrap(HI_U32 u32Data, HI_U32 u32AcmCliporwrap)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

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

HI_VOID VDP_VP_SetAcmStretch(HI_U32 u32Data, HI_U32 u32AcmStrch)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAcmStretch() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_stretch = u32AcmStrch;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetAcmClipRange(HI_U32 u32Data, HI_U32 u32AcmClipRange)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

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

HI_VOID VDP_VP_SetAcmChmThd(HI_U32 u32Data, HI_U32 u32AcmCbcrthr)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

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
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

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
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

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
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

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
    U_VP0_ACM_CAD VP0_ACM_CAD;

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

#if 0
HI_VOID  VDP_VP_SetAbcEnable      (HI_U32 u32Data, HI_U32 u32Enable)
{
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_CTRL VP0_ABC_CTRL;

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
    U_VP0_ABC_THRE1 VP0_ABC_THRE1;
    U_VP0_ABC_THRE2 VP0_ABC_THRE2;

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
    U_VP0_ABC_BRIGTHRE1  VP0_ABC_BRIGTHRE1;
    U_VP0_ABC_DARKTHRE1  VP0_ABC_DARKTHRE1;

    U_VP0_ABC_BRIGTHRE2  VP0_ABC_BRIGTHRE2;
    U_VP0_ABC_DARKTHRE2  VP0_ABC_DARKTHRE2;

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
    U_VP0_ABC_PARAADJ  VP0_ABC_PARAADJ;

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
    U_VP0_AB_CCAD  VP0_AB_CCAD;

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
    U_VP0_ABC_HBWCOEF VP0_ABC_HBWCOEF;

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
    U_VP0_ABC_HBWCOEF VP0_ABC_HBWCOEF;

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
    U_VP0_ABC_HBWOFFSET  VP0_ABC_HBWOFFSET ;

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
    U_VP0_ABC_LUMATHR0 VP0_ABC_LUMATHR0;
    U_VP0_ABC_LUMATHR1 VP0_ABC_LUMATHR1;
    U_VP0_ABC_LUMATHR2 VP0_ABC_LUMATHR2;
    U_VP0_ABC_LUMATHR3 VP0_ABC_LUMATHR3;

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
    U_VP0_ABC_LUMACOEF0 VP0_ABC_LUMACOEF0;
    U_VP0_ABC_LUMACOEF1 VP0_ABC_LUMACOEF1;
    U_VP0_ABC_LUMACOEF2 VP0_ABC_LUMACOEF2;

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
    U_VP0_ABC_CLIP       VP0_ABC_CLIP;
    U_VP0_ABC_CLIPNEG    VP0_ABC_CLIPNEG;

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
    U_VP0_UPD VP0_UPD ;

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
#endif


#if VP0_ACC_EN
HI_VOID VDP_VP_SetAccThd( HI_U32 u32Data,ACCTHD_S stAccThd)
{
    U_VP0_ACCTHD1 VP0_ACCTHD1;
    U_VP0_ACCTHD2 VP0_ACCTHD2;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAccThd() Select Wrong VP ID\n");
        return ;
    }

    VP0_ACCTHD1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.bits.acc_multiple = stAccThd.acc_multi   ;
    VP0_ACCTHD2.bits.thd_med_high = stAccThd.thd_med_high;
    VP0_ACCTHD1.bits.thd_med_low  = stAccThd.thd_med_low ;
    VP0_ACCTHD1.bits.thd_high     = stAccThd.thd_high    ;
    VP0_ACCTHD1.bits.thd_low      = stAccThd.thd_low     ;

    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD2.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCTHD1.u32) + u32Data * VP_OFFSET), VP0_ACCTHD1.u32);

}

HI_VOID VDP_VP_SetAccTab(HI_U32 u32Data,HI_U32 *upTable)
{
    U_VP0_ACCLOWN  VP0_ACCLOW[3];
    U_VP0_ACCMEDN  VP0_ACCMED[3];
    U_VP0_ACCHIGHN VP0_ACCHIGH[3];
    U_VP0_ACCMLN   VP0_ACCML[3];
    U_VP0_ACCMHN   VP0_ACCMH[3];

    HI_U32 ii = 0;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAccTab() Select Wrong VP ID\n");
        return ;
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCLOW[ii].bits.table_data1n = upTable[0 + ii*3];
        VP0_ACCLOW[ii].bits.table_data2n = upTable[1 + ii*3];
        VP0_ACCLOW[ii].bits.table_data3n = upTable[2 + ii*3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCLOWN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCLOW[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCMED[ii].bits.table_data1n = upTable[9 + 0 + ii*3];
        VP0_ACCMED[ii].bits.table_data2n = upTable[9 + 1 + ii*3];
        VP0_ACCMED[ii].bits.table_data3n = upTable[9 + 2 + ii*3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCMEDN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCMED[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCHIGH[ii].bits.table_data1n = upTable[18 + 0 + ii*3];
        VP0_ACCHIGH[ii].bits.table_data2n = upTable[18 + 1 + ii*3];
        VP0_ACCHIGH[ii].bits.table_data3n = upTable[18 + 2 + ii*3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCHIGHN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCHIGH[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCML[ii].bits.table_data1n = upTable[27 + 0 + ii*3];
        VP0_ACCML[ii].bits.table_data2n = upTable[27 + 1 + ii*3];
        VP0_ACCML[ii].bits.table_data3n = upTable[27 + 2 + ii*3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCMLN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCML[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCMH[ii].bits.table_data1n = upTable[36 + 0 + ii*3];
        VP0_ACCMH[ii].bits.table_data2n = upTable[36 + 1 + ii*3];
        VP0_ACCMH[ii].bits.table_data3n = upTable[36 + 2 + ii*3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCMHN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCMH[ii].u32);
    }

}

HI_VOID VDP_VP_SetAccCtrl(HI_U32 u32Data,HI_U32 uAccEn, HI_U32 uAccMode)
{
    U_VP0_ACCTHD1  VP0_ACCTHD1;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAccCtrl() Select Wrong VP ID\n");
        return ;
    }

    VP0_ACCTHD1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD1.bits.acc_en   = uAccEn  ;
    VP0_ACCTHD1.bits.acc_mode = uAccMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD1.u32);
}


HI_VOID VDP_VP_SetAccRst(HI_U32 u32Data,HI_U32 uAccRst)
{
    U_VP0_ACCTHD2 VP0_ACCTHD2;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetAccRst() Select Wrong VP ID\n");
        return ;
    }


    VP0_ACCTHD2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.bits.acc_rst   = uAccRst  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD2.u32);
}

HI_VOID  VDP_VP0_SetAccCad(HI_U32 u32Data, HI_U32 u32addr)
{
    U_VP0_ACC_CAD VP0_ACC_CAD;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("error,VDP_VP0_SetAccCad() select wrong video layer id\n");
        return ;
    }

    VP0_ACC_CAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_ACC_CAD.u32) + u32Data * VID_OFFSET));

    VP0_ACC_CAD.bits.coef_addr= u32addr;

    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACC_CAD.u32) + u32Data * VID_OFFSET), VP0_ACC_CAD.u32);
    return ;
}
#endif

HI_VOID  VDP_VP0_SetParaUpd       (HI_U32 u32Data, VDP_VP_PARA_E  enMode)
{
    U_VP0_PARAUP  VP0_PARAUP;
    VP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET));

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("error,VDP_VP0_SetParaUpd() select wrong video layer id\n");
        return ;
    }

#if 0
    //mod me!
    if(enMode == VDP_DISP_COEFMODE_ACC)
    {
        VP0_PARAUP.bits.v0_acccoef_upd= 0x1;
    }

    if(enMode == VDP_DISP_COEFMODE_ACM)
    {
        VP0_PARAUP.bits.v0_acmcoef_upd= 0x1;
    }
#endif


    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET), VP0_PARAUP.u32);
    return ;
}


//DCI_BEING
HI_VOID VDP_VP_SetDciEnable(HI_U32 u32Data, HI_U32 u32DciEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciEnable() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_en = u32DciEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciDbgEn(HI_U32 u32Data, HI_U32 u32DciDbgEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciDbgEn() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_dbg_en = u32DciDbgEn;

    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciDbgMode(HI_U32 u32Data, HI_U32 u32DciDbgMode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciDbgMode() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_dbg_mode = u32DciDbgMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciSceneFlg(HI_U32 u32Data, HI_U32 u32SceneFlg)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciSceneFlg() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_scene_flg = u32SceneFlg;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciManAdj(HI_U32 u32Data, VDP_VP_DCI_ADJ_NUM_E Adjnum, VDP_VP_DCI_CTRL_MODE_E Manmode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciManAdj() Select Wrong VP ID\n");
        return ;
    }

	if(Adjnum == VDP_VP_DCI_ADJ0 )
	{
		VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj0 = Manmode;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ1)
	{
		VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj1 = Manmode;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ2)
	{
		VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj2 = Manmode;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}

}

HI_VOID VDP_VP_SetDciCbCrcmpEn(HI_U32 u32Data, HI_U32 u32ClorEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciCbCrcmpEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_cbcrcmp_en = u32ClorEn;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciCbCrstaEn(HI_U32 u32Data, HI_U32 u32ClorstaEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciCbCrstaEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_cbcrsta_en = u32ClorstaEn;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciRange(HI_U32 u32Data, VDP_DCI_RANGE_E Rangeinout , HI_U32 u32Rangemode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciRange() Select Wrong VP ID\n");
        return ;
    }

	if(Rangeinout == IN_RANGE )
	{
		VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_in_range = u32Rangemode;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else
	{
		VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_out_range = u32Rangemode;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
}

HI_VOID VDP_VP_SetDciShiftCtrl(HI_U32 u32Data, VDP_DCI_SHIFT_E enShiftCtrl)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciShiftCtrl() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_shift_ctrl = enShiftCtrl;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciLpfEn(HI_U32 u32Data, HI_U32 u32enLpf)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciLpfEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_histlpf_en = u32enLpf;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciSwcfgEn(HI_U32 u32Data, HI_U32 u32enSwcfg)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciSwcfgEn() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_swcfg_en = u32enSwcfg;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciStatCtrl(HI_U32 u32Data, HI_U32 u32StatCtrl)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciStatCtrl() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_stat_ctrl = u32StatCtrl;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciPosition(HI_U32 u32Data, VDP_DCI_POS_S stDciPos)
{
	U_VP0_DCIHPOS VP0_DCIHPOS;
	U_VP0_DCIVPOS VP0_DCIVPOS;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciPosition() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIHPOS.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHPOS.bits.dci_hstart = stDciPos.hstart;
	VP0_DCIHPOS.bits.dci_hend = stDciPos.hend;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIHPOS.u32)+ u32Data * VP_OFFSET), VP0_DCIHPOS.u32);

	VP0_DCIVPOS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIVPOS.u32)+ u32Data * VP_OFFSET));
	VP0_DCIVPOS.bits.dci_vstart = stDciPos.vstart;
	VP0_DCIVPOS.bits.dci_vend = stDciPos.vend;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIVPOS.u32)+ u32Data * VP_OFFSET), VP0_DCIVPOS.u32);
}

HI_VOID VDP_VP_SetDciHisBld(HI_U32 u32Data, VDP_DCI_PARA_S stDciPara)
{
    U_VP0_DCIHISBLD VP0_DCIHISBLD;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciHisBld() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISBLD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISBLD.bits.dci_cbcrsta_y = stDciPara.s32CbCrSta_Y;
	VP0_DCIHISBLD.bits.dci_cbcrsta_cb = stDciPara.s32CbCrSta_Cb;
	VP0_DCIHISBLD.bits.dci_cbcrsta_cr = stDciPara.s32CbCrSta_Cr;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET), VP0_DCIHISBLD.u32);
}

HI_VOID VDP_VP_SetDciHisOft(HI_U32 u32Data, VDP_DCI_PARA_S stDciPara)
{
    U_VP0_DCIHISOFT VP0_DCIHISOFT ;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciHisOft() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISOFT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISOFT.bits.dci_cbcrsta_oft = stDciPara.s32CbCrSta_Offset;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET), VP0_DCIHISOFT.u32);
}

HI_VOID VDP_VP_SetDciHisCor(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara)
{
    U_VP0_DCIHISCOR VP0_DCIHISCOR;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciHisCor() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISCOR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIHISCOR.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISCOR.bits.dci_histcor_thr0 = stDciPara.s32HistCoring[0];
	VP0_DCIHISCOR.bits.dci_histcor_thr1 = stDciPara.s32HistCoring[1];
	VP0_DCIHISCOR.bits.dci_histcor_thr2 = stDciPara.s32HistCoring[2];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIHISCOR.u32)+ u32Data * VP_OFFSET), VP0_DCIHISCOR.u32);

}

HI_VOID VDP_VP_SetDciMerBld(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara)
{
    U_VP0_DCIMERBLD VP0_DCIMERBLD;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciMerBld() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIMERBLD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIMERBLD.u32)+ u32Data * VP_OFFSET));
	VP0_DCIMERBLD.bits.dci_metrc_abld0 = stDciPara.s32MetricAlpha[0];
	VP0_DCIMERBLD.bits.dci_metrc_abld1 = stDciPara.s32MetricAlpha[1];
	VP0_DCIMERBLD.bits.dci_metrc_abld2 = stDciPara.s32MetricAlpha[2];
	VP0_DCIMERBLD.bits.dci_hist_abld = stDciPara.s32HistABlend;
	VP0_DCIMERBLD.bits.dci_org_abld  = stDciPara.s32OrgABlend;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIMERBLD.u32)+ u32Data * VP_OFFSET), VP0_DCIMERBLD.u32);

}

HI_VOID VDP_VP_SetDciAdjWgt(HI_U32 u32Data,VDP_VP_DCI_ADJ_NUM_E Adjnum,VDP_DCI_PARA_S stDciPara)
{
    U_VP0_DCIADJWGT VP0_DCIADJWGT;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciAdjWgt() Select Wrong VP ID\n");
		return ;
	}

	if(Adjnum == VDP_VP_DCI_ADJ0)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt0 = stDciPara.s32ManAdjWgt[0];
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ1)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt1 = stDciPara.s32ManAdjWgt[1];
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ2)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt2 = stDciPara.s32ManAdjWgt[2];
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
}

HI_VOID VDP_VP_SetDciClip(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara )
{
    U_VP0_DCICLIP0 VP0_DCICLIP0;
    U_VP0_DCICLIP1 VP0_DCICLIP1;
    U_VP0_DCICLIP2 VP0_DCICLIP2;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciClip() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICLIP0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICLIP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP0.bits.dci_wgt_cliplow0  = stDciPara.s32AdjGain_ClipL[0];
	VP0_DCICLIP0.bits.dci_wgt_cliphigh0  = stDciPara.s32AdjGain_ClipH[0];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICLIP0.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP0.u32);

	VP0_DCICLIP1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICLIP1.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP1.bits.dci_wgt_cliplow1  = stDciPara.s32AdjGain_ClipL[1];
	VP0_DCICLIP1.bits.dci_wgt_cliphigh1  = stDciPara.s32AdjGain_ClipH[1];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICLIP1.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP1.u32);

	VP0_DCICLIP2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICLIP2.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP2.bits.dci_wgt_cliplow2  = stDciPara.s32AdjGain_ClipL[2];
	VP0_DCICLIP2.bits.dci_wgt_cliphigh2  = stDciPara.s32AdjGain_ClipH[2];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICLIP2.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP2.u32);

}

HI_VOID VDP_VP_SetDciGlbGain(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara )
{
    U_VP0_DCIGLBGAIN VP0_DCIGLBGAIN;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciGlbGain() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIGLBGAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIGLBGAIN.u32)+ u32Data * VP_OFFSET));
	VP0_DCIGLBGAIN.bits.dci_glb_gain0 = stDciPara.s32GlbGain0;
	VP0_DCIGLBGAIN.bits.dci_glb_gain1 = stDciPara.s32GlbGain1;
	VP0_DCIGLBGAIN.bits.dci_glb_gain2 = stDciPara.s32GlbGain2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIGLBGAIN.u32)+ u32Data * VP_OFFSET), VP0_DCIGLBGAIN.u32);
}

HI_VOID VDP_VP_SetDciPos(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara )
{
    U_VP0_DCIPOSTHR0 VP0_DCIPOSTHR0;
    U_VP0_DCIPOSTHR1 VP0_DCIPOSTHR1;

    U_VP0_DCIPOSGAIN0 VP0_DCIPOSGAIN0;
    U_VP0_DCIPOSGAIN1 VP0_DCIPOSGAIN1;

    U_VP0_DCIPOSSLP0 VP0_DCIPOSSLP0;
    U_VP0_DCIPOSSLP1 VP0_DCIPOSSLP1;
    U_VP0_DCIPOSSLP2 VP0_DCIPOSSLP2;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciPos() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIPOSTHR0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSTHR0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr1  = stDciPara.s32CbCrGainPos_Ythrsh[0];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr2  = stDciPara.s32CbCrGainPos_Ythrsh[1];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr3  = stDciPara.s32CbCrGainPos_Ythrsh[2];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr4  = stDciPara.s32CbCrGainPos_Ythrsh[3];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSTHR0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSTHR0.u32);

	VP0_DCIPOSTHR1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSTHR1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr5  = stDciPara.s32CbCrGainPos_Ythrsh[4];
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr6  = stDciPara.s32CbCrGainPos_Ythrsh[5];
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr7  = stDciPara.s32CbCrGainPos_Ythrsh[6];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSTHR1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSTHR1.u32);


	VP0_DCIPOSGAIN0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSGAIN0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSGAIN1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSGAIN1.u32)+ u32Data * VP_OFFSET));

	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr0 = stDciPara.s32CbCrGainPos[0];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr1 = stDciPara.s32CbCrGainPos[1];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr2 = stDciPara.s32CbCrGainPos[2];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr3 = stDciPara.s32CbCrGainPos[3];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr4 = stDciPara.s32CbCrGainPos[4];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr5 = stDciPara.s32CbCrGainPos[5];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr6 = stDciPara.s32CbCrGainPos[6];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr7 = stDciPara.s32CbCrGainPos[7];

	VP0_DCIPOSGAIN1.bits.dci_gainpos_cbcr8 = stDciPara.s32CbCrGainPos[8];

	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSGAIN0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSGAIN0.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSGAIN1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSGAIN1.u32);


	VP0_DCIPOSSLP0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSSLP1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSSLP2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP2.u32)+ u32Data * VP_OFFSET));

	VP0_DCIPOSSLP0.bits.dci_gainpos_slp0 = stDciPara.s32CbCrGainPos_Slp[0];
	VP0_DCIPOSSLP0.bits.dci_gainpos_slp1 = stDciPara.s32CbCrGainPos_Slp[1];
	VP0_DCIPOSSLP0.bits.dci_gainpos_slp2 = stDciPara.s32CbCrGainPos_Slp[2];

	VP0_DCIPOSSLP1.bits.dci_gainpos_slp3 = stDciPara.s32CbCrGainPos_Slp[3];
	VP0_DCIPOSSLP1.bits.dci_gainpos_slp4 = stDciPara.s32CbCrGainPos_Slp[4];
	VP0_DCIPOSSLP1.bits.dci_gainpos_slp5 = stDciPara.s32CbCrGainPos_Slp[5];

	VP0_DCIPOSSLP2.bits.dci_gainpos_slp6 = stDciPara.s32CbCrGainPos_Slp[6];
	VP0_DCIPOSSLP2.bits.dci_gainpos_slp7 = stDciPara.s32CbCrGainPos_Slp[7];

	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP0.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP1.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIPOSSLP2.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP2.u32);
}

HI_VOID VDP_VP_SetDciNeg(HI_U32 u32Data,VDP_DCI_PARA_S stDciPara )
{
    U_VP0_DCINEGTHR0 VP0_DCINEGTHR0;
    U_VP0_DCINEGTHR1 VP0_DCINEGTHR1;

    U_VP0_DCINEGGAIN0 VP0_DCINEGGAIN0;
    U_VP0_DCINEGGAIN1 VP0_DCINEGGAIN1;

    U_VP0_DCINEGSLP0 VP0_DCINEGSLP0;
    U_VP0_DCINEGSLP1 VP0_DCINEGSLP1;
    U_VP0_DCINEGSLP2 VP0_DCINEGSLP2;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciNeg() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCINEGTHR0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGTHR0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGTHR0.bits.dci_gainneg_thr1  = stDciPara.s32CbCrGainNeg_Ythrsh[0];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr2  = stDciPara.s32CbCrGainNeg_Ythrsh[1];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr3  = stDciPara.s32CbCrGainNeg_Ythrsh[2];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr4  = stDciPara.s32CbCrGainNeg_Ythrsh[3];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGTHR0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGTHR0.u32);

	VP0_DCINEGTHR1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGTHR1.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGTHR1.bits.dci_gainneg_thr5  = stDciPara.s32CbCrGainNeg_Ythrsh[4];
	VP0_DCINEGTHR1.bits.dci_gainneg_thr6  = stDciPara.s32CbCrGainNeg_Ythrsh[5];
	VP0_DCINEGTHR1.bits.dci_gainneg_thr7  = stDciPara.s32CbCrGainNeg_Ythrsh[6];
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGTHR1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGTHR1.u32);


	VP0_DCINEGGAIN0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGGAIN0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGGAIN1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGGAIN1.u32)+ u32Data * VP_OFFSET));

	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr0 = stDciPara.s32CbCrGainNeg[0];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr1 = stDciPara.s32CbCrGainNeg[1];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr2 = stDciPara.s32CbCrGainNeg[2];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr3 = stDciPara.s32CbCrGainNeg[3];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr4 = stDciPara.s32CbCrGainNeg[4];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr5 = stDciPara.s32CbCrGainNeg[5];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr6 = stDciPara.s32CbCrGainNeg[6];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr7 = stDciPara.s32CbCrGainNeg[7];

	VP0_DCINEGGAIN1.bits.dci_gainneg_cbcr8 = stDciPara.s32CbCrGainNeg[8];

	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGGAIN0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGGAIN0.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGGAIN1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGGAIN1.u32);


	VP0_DCINEGSLP0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGSLP0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGSLP2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCINEGSLP2.u32)+ u32Data * VP_OFFSET));

	VP0_DCINEGSLP0.bits.dci_gainneg_slp0 = stDciPara.s32CbCrGainNeg_Slp[0];
	VP0_DCINEGSLP0.bits.dci_gainneg_slp1 = stDciPara.s32CbCrGainNeg_Slp[1];
	VP0_DCINEGSLP0.bits.dci_gainneg_slp2 = stDciPara.s32CbCrGainNeg_Slp[2];


	VP0_DCINEGSLP1.bits.dci_gainneg_slp3 = stDciPara.s32CbCrGainNeg_Slp[3];
	VP0_DCINEGSLP1.bits.dci_gainneg_slp4 = stDciPara.s32CbCrGainNeg_Slp[4];
	VP0_DCINEGSLP1.bits.dci_gainneg_slp5 = stDciPara.s32CbCrGainNeg_Slp[5];

	VP0_DCINEGSLP2.bits.dci_gainneg_slp6 = stDciPara.s32CbCrGainNeg_Slp[6];
	VP0_DCINEGSLP2.bits.dci_gainneg_slp7 = stDciPara.s32CbCrGainNeg_Slp[7];

	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP0.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGSLP1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP1.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCINEGSLP2.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP2.u32);

}

HI_VOID VDP_VP_SetDciAdjSW(HI_U32 u32Data, VDP_DCI_PARA_S stDciPara )
{
    U_VP0_DCIADJSW0 VP0_DCIADJSW0 ;
    U_VP0_DCIADJSW1 VP0_DCIADJSW1 ;
    U_VP0_DCIADJSW2 VP0_DCIADJSW2 ;
    U_VP0_DCIADJSW3 VP0_DCIADJSW3 ;
    U_VP0_DCIADJSW4 VP0_DCIADJSW4 ;
    U_VP0_DCIADJSW5 VP0_DCIADJSW5 ;
    U_VP0_DCIADJSW6 VP0_DCIADJSW6 ;
    U_VP0_DCIADJSW7 VP0_DCIADJSW7 ;
    U_VP0_DCIADJSW8 VP0_DCIADJSW8 ;
    U_VP0_DCIADJSW9 VP0_DCIADJSW9 ;
    U_VP0_DCIADJSW10 VP0_DCIADJSW10 ;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciAdjSW() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIADJSW0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW2.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW3.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW4.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW5.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW5.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW6.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW6.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW7.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW7.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW8.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW8.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW9.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW9.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW10.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCIADJSW10.u32)+ u32Data * VP_OFFSET));

	VP0_DCIADJSW0.bits.dci_adj_sw0 = stDciPara.ps32AdjLUT_SW[0];
	VP0_DCIADJSW0.bits.dci_adj_sw1 = stDciPara.ps32AdjLUT_SW[1];
	VP0_DCIADJSW0.bits.dci_adj_sw2 = stDciPara.ps32AdjLUT_SW[2];
	VP0_DCIADJSW1.bits.dci_adj_sw3 = stDciPara.ps32AdjLUT_SW[3];
	VP0_DCIADJSW1.bits.dci_adj_sw4 = stDciPara.ps32AdjLUT_SW[4];
	VP0_DCIADJSW1.bits.dci_adj_sw5 = stDciPara.ps32AdjLUT_SW[5];
	VP0_DCIADJSW2.bits.dci_adj_sw6 = stDciPara.ps32AdjLUT_SW[6];
	VP0_DCIADJSW2.bits.dci_adj_sw7 = stDciPara.ps32AdjLUT_SW[7];
	VP0_DCIADJSW2.bits.dci_adj_sw8 = stDciPara.ps32AdjLUT_SW[8];
	VP0_DCIADJSW3.bits.dci_adj_sw9 = stDciPara.ps32AdjLUT_SW[9];
	VP0_DCIADJSW3.bits.dci_adj_sw10 = stDciPara.ps32AdjLUT_SW[10];
	VP0_DCIADJSW3.bits.dci_adj_sw11 = stDciPara.ps32AdjLUT_SW[11];
	VP0_DCIADJSW4.bits.dci_adj_sw12 = stDciPara.ps32AdjLUT_SW[12];
	VP0_DCIADJSW4.bits.dci_adj_sw13 = stDciPara.ps32AdjLUT_SW[13];
	VP0_DCIADJSW4.bits.dci_adj_sw14 = stDciPara.ps32AdjLUT_SW[14];
	VP0_DCIADJSW5.bits.dci_adj_sw15 = stDciPara.ps32AdjLUT_SW[15];
	VP0_DCIADJSW5.bits.dci_adj_sw16 = stDciPara.ps32AdjLUT_SW[16];
	VP0_DCIADJSW5.bits.dci_adj_sw17 = stDciPara.ps32AdjLUT_SW[17];
	VP0_DCIADJSW6.bits.dci_adj_sw18 = stDciPara.ps32AdjLUT_SW[18];
	VP0_DCIADJSW6.bits.dci_adj_sw19 = stDciPara.ps32AdjLUT_SW[19];
	VP0_DCIADJSW6.bits.dci_adj_sw20 = stDciPara.ps32AdjLUT_SW[20];
	VP0_DCIADJSW7.bits.dci_adj_sw21 = stDciPara.ps32AdjLUT_SW[21];
	VP0_DCIADJSW7.bits.dci_adj_sw22 = stDciPara.ps32AdjLUT_SW[22];
	VP0_DCIADJSW7.bits.dci_adj_sw23 = stDciPara.ps32AdjLUT_SW[23];
	VP0_DCIADJSW8.bits.dci_adj_sw24 = stDciPara.ps32AdjLUT_SW[24];
	VP0_DCIADJSW8.bits.dci_adj_sw25 = stDciPara.ps32AdjLUT_SW[25];
	VP0_DCIADJSW8.bits.dci_adj_sw26 = stDciPara.ps32AdjLUT_SW[26];
	VP0_DCIADJSW9.bits.dci_adj_sw27 = stDciPara.ps32AdjLUT_SW[27];
	VP0_DCIADJSW9.bits.dci_adj_sw28 = stDciPara.ps32AdjLUT_SW[28];
	VP0_DCIADJSW9.bits.dci_adj_sw29 = stDciPara.ps32AdjLUT_SW[29];
	VP0_DCIADJSW10.bits.dci_adj_sw30 = stDciPara.ps32AdjLUT_SW[30];
	VP0_DCIADJSW10.bits.dci_adj_sw31 = stDciPara.ps32AdjLUT_SW[31];
	VP0_DCIADJSW10.bits.dci_adj_sw32 = stDciPara.ps32AdjLUT_SW[32];

	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW0.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW0.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW1.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW1.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW2.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW2.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW3.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW3.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW4.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW4.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW5.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW5.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW6.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW6.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW7.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW7.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW8.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW8.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW9.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW9.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCIADJSW10.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW10.u32);


}

HI_VOID VDP_VP_SetDciCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr)
{
    U_VP0_DCI_CAD VP0_DCI_CAD;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciCoefAddr() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCI_CAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCI_CAD.u32)+ u32Data * VP_OFFSET));
	VP0_DCI_CAD.bits.coef_addr = u32CoefAddr;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCI_CAD.u32)+ u32Data * VP_OFFSET), VP0_DCI_CAD.u32);
}


#if 0
HI_VOID VDP_VP_SetDciVpRegUp (HI_U32 u32Data, HI_U32 VpRegUpEn)
{
    U_VP0_UPD VP0_UPD;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetDciCoefAddr() Select Wrong VP ID\n");
		return ;
	}

	VP0_UPD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_UPD.u32)+ u32Data * VP_OFFSET));
	VP0_UPD.bits.regup = VpRegUpEn;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_UPD.u32)+ u32Data * VP_OFFSET), VP0_UPD.u32);
}
#endif

HI_VOID VDP_VP_SetParaUpd (HI_U32 u32Data, VDP_VP_PARA_E VpPara)
{
    U_VP0_PARAUP   VP0_PARAUP;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetParaUpd() Select Wrong VP ID\n");
		return ;
	}

	if(VpPara == VDP_VP_PARA_ACM)
	{
		VP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET));
		VP0_PARAUP.bits.vp0_acmcoef_upd = 1;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET), VP0_PARAUP.u32);
	}
	else if(VpPara == VDP_VP_PARA_DCI)
	{
		VP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET));
		VP0_PARAUP.bits.vp0_acccoef_upd = 1;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET), VP0_PARAUP.u32);
	}
	else
	{
		VP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET));
		VP0_PARAUP.bits.vp0_sharpcoef_upd = 1;
		VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_PARAUP.u32)+ u32Data * VP_OFFSET), VP0_PARAUP.u32);
	}

}
//Dci end


//WCG_BEGING
HI_VOID VDP_VP_SetWcgEnable  (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_VPWCG_CSC2CTRL  VPWCG_CSC2CTRL;
	U_VP0_CSC_ODC     VP0_CSC_ODC;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetWcgEnable() Select Wrong VP ID\n");
		return ;
	}

	VPWCG_CSC2CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2CTRL.bits.csc2_en = u32Enable;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2CTRL.u32);

	//set csc sign mode
	VP0_CSC_ODC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC_ODC.u32)+ u32Data * VP_OFFSET));
	VP0_CSC_ODC.bits.csc_sign_mode = u32Enable;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC_ODC.u32)+ u32Data * VP_OFFSET), VP0_CSC_ODC.u32);

}

HI_VOID VDP_VP_SetWcgCoef  (HI_U32 u32Data, VDP_WCG_COEF_S stWcgCoef)
{
	U_VPWCG_NON2LCTRL     VPWCG_NON2LCTRL;
	U_VPWCG_NON2LCOEF0    VPWCG_NON2LCOEF0;
	U_VPWCG_NON2LCOEF1    VPWCG_NON2LCOEF1;
	U_VPWCG_NON2LCOEF2    VPWCG_NON2LCOEF2;

	U_VPWCG_CSC2CTRL      VPWCG_CSC2CTRL;
	U_VPWCG_CSC2COEF0     VPWCG_CSC2COEF0;
	U_VPWCG_CSC2COEF1     VPWCG_CSC2COEF1;
	U_VPWCG_CSC2COEF2     VPWCG_CSC2COEF2;
	U_VPWCG_CSC2COEF3     VPWCG_CSC2COEF3;

	U_VPWCG_L2NONTHD      VPWCG_L2NONTHD;
	U_VPWCG_L2NONCOEF0    VPWCG_L2NONCOEF0;
	U_VPWCG_L2NONCOEF1    VPWCG_L2NONCOEF1;
	U_VPWCG_L2NONCOEF2    VPWCG_L2NONCOEF2;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetWcgCoef() Select Wrong VP ID\n");
		return ;
	}

	VPWCG_CSC2CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2CTRL.bits.csc2_scale_sel = stWcgCoef.csc2_scale_sel;
	VPWCG_CSC2CTRL.bits.csc2_r_r_coef  = (stWcgCoef.csc2_r_r_coef);
	VPWCG_CSC2CTRL.bits.csc2_r_g_coef  = (stWcgCoef.csc2_r_g_coef);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2CTRL.u32);

	VPWCG_CSC2COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF0.bits.csc2_r_b_coef  = (stWcgCoef.csc2_r_b_coef);
	VPWCG_CSC2COEF0.bits.csc2_g_r_coef  = (stWcgCoef.csc2_g_r_coef);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF0.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF0.u32);

	VPWCG_CSC2COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF1.bits.csc2_g_g_coef  = (stWcgCoef.csc2_g_g_coef);
	VPWCG_CSC2COEF1.bits.csc2_g_b_coef  = (stWcgCoef.csc2_g_b_coef);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF1.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF1.u32);

	VPWCG_CSC2COEF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF2.bits.csc2_b_r_coef  = (stWcgCoef.csc2_b_r_coef);
	VPWCG_CSC2COEF2.bits.csc2_b_g_coef  = (stWcgCoef.csc2_b_g_coef);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF2.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF2.u32);

	VPWCG_CSC2COEF3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF3.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF3.bits.csc2_b_b_coef  = (stWcgCoef.csc2_b_b_coef);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_CSC2COEF3.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF3.u32);

	//-------------------------

	VPWCG_NON2LCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_NON2LCTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCTRL.bits.gma_lin_neg_scl  = stWcgCoef.gma_lin_neg_scl;
	VPWCG_NON2LCTRL.bits.gma_lin_init_slp = stWcgCoef.gma_lin_init_slp;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_0   = stWcgCoef.gma_lin_thre_0;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_1   = stWcgCoef.gma_lin_thre_1;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_2   = stWcgCoef.gma_lin_thre_2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_NON2LCTRL.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCTRL.u32);

	VPWCG_NON2LCOEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_a_0  = stWcgCoef.gma_lin_coef_a_0;
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_b_0  = stWcgCoef.gma_lin_coef_b_0;
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_c_0  = stWcgCoef.gma_lin_coef_c_0;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF0.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF0.u32);

	VPWCG_NON2LCOEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_a_1  = stWcgCoef.gma_lin_coef_a_1;
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_b_1  = stWcgCoef.gma_lin_coef_b_1;
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_c_1  = stWcgCoef.gma_lin_coef_c_1;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF1.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF1.u32);

	VPWCG_NON2LCOEF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_a_2  = stWcgCoef.gma_lin_coef_a_2;
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_b_2  = stWcgCoef.gma_lin_coef_b_2;
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_c_2  = stWcgCoef.gma_lin_coef_c_2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_NON2LCOEF2.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF2.u32);

	VPWCG_L2NONTHD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_L2NONTHD.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONTHD.bits.lin_gma_thre_0  = stWcgCoef.lin_gma_thre_0;
	VPWCG_L2NONTHD.bits.lin_gma_thre_1  = stWcgCoef.lin_gma_thre_1;
	VPWCG_L2NONTHD.bits.lin_gma_thre_2  = stWcgCoef.lin_gma_thre_2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_L2NONTHD.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONTHD.u32);

	VPWCG_L2NONCOEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_a_0  = stWcgCoef.lin_gma_coef_a_0;
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_b_0  = stWcgCoef.lin_gma_coef_b_0;
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_c_0  = stWcgCoef.lin_gma_coef_c_0;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF0.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF0.u32);

	VPWCG_L2NONCOEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_a_1  = stWcgCoef.lin_gma_coef_a_1;
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_b_1  = stWcgCoef.lin_gma_coef_b_1;
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_c_1  = stWcgCoef.lin_gma_coef_c_1;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF1.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF1.u32);

	VPWCG_L2NONCOEF2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_a_2  = stWcgCoef.lin_gma_coef_a_2;
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_b_2  = stWcgCoef.lin_gma_coef_b_2;
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_c_2  = stWcgCoef.lin_gma_coef_c_2;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VPWCG_L2NONCOEF2.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF2.u32);


}

HI_VOID VDP_VP_SetPositionSel  (HI_U32 u32Data, HI_U32 u32Sel)
{
    U_VP0_CTRL  VP0_CTRL;

	if(u32Data >= VP_MAX)
	{
		HI_PRINT("Error,VDP_VP_SetPositionSel() Select Wrong VP ID\n");
		return ;
	}

	VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32)+ u32Data * VP_OFFSET));
	VP0_CTRL.bits.vp_sel = u32Sel;
	VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32)+ u32Data * VP_OFFSET), VP0_CTRL.u32);

}

#endif //end VP0_EN


HI_VOID VDP_VID_SetMasterSel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
	U_VOMASTERSEL   VOMASTERSEL;
	U_VOMASTERSEL1  VOMASTERSEL1;

    U_V0_CTRL       V0_CTRL;

	VOMASTERSEL.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)));
	VOMASTERSEL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)));

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));

    if(u32MasterNum == VDP_MASTER0)
    {
        V0_CTRL.bits.ofl_master = 0;
        if(u32Data == VDP_LAYER_VID0)
        {
            VOMASTERSEL.bits.v0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID1)
        {
            VOMASTERSEL.bits.v1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID2)
        {
            VOMASTERSEL.bits.v2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID3)
        {
            VOMASTERSEL.bits.v3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID4)
        {
            VOMASTERSEL.bits.v4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID6)
        {
            VOMASTERSEL.bits.v5_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID7)
        {
            VOMASTERSEL.bits.v6_master_sel = 0;
        }
        else
        {
            HI_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER1)
    {
        V0_CTRL.bits.ofl_master = 0;
        if(u32Data == VDP_LAYER_VID0)
        {
            VOMASTERSEL.bits.v0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID1)
        {
            VOMASTERSEL.bits.v1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID2)
        {
            VOMASTERSEL.bits.v2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID3)
        {
            VOMASTERSEL.bits.v3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID4)
        {
            VOMASTERSEL.bits.v4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID6)
        {
            VOMASTERSEL.bits.v5_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID7)
        {
            VOMASTERSEL.bits.v6_master_sel = 1;
        }
        else
        {
            HI_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER2)
    {
        V0_CTRL.bits.ofl_master = 1;
        if(u32Data == VDP_LAYER_VID0)
        {
            VOMASTERSEL1.bits.v0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID1)
        {
            VOMASTERSEL1.bits.v1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID2)
        {
            VOMASTERSEL1.bits.v2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID3)
        {
            VOMASTERSEL1.bits.v3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID4)
        {
            VOMASTERSEL1.bits.v4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_VID5)
        {
            VOMASTERSEL1.bits.v5_master_sel = 0;
        }
        else
        {
            HI_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER3)
    {
        V0_CTRL.bits.ofl_master = 1;
        if(u32Data == VDP_LAYER_VID0)
        {
            VOMASTERSEL1.bits.v0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID1)
        {
            VOMASTERSEL1.bits.v1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID2)
        {
            VOMASTERSEL1.bits.v2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID3)
        {
            VOMASTERSEL1.bits.v3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID4)
        {
            VOMASTERSEL1.bits.v4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_VID5)
        {
            VOMASTERSEL1.bits.v5_master_sel = 1;
        }
        else
        {
            HI_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
        }
    }


	VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32);

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;

}

HI_VOID  VDP_VID_SetFrReqSize    (HI_U32 u32Data)
{
    U_V0_FI_REQW_0 V0_FI_REQW_0;

    V0_FI_REQW_0.u32 = u32Data ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_FI_REQW_0.u32) + VDP_LAYER_VID6 * VID_OFFSET), V0_FI_REQW_0.u32);

    return ;
}

HI_VOID  VDP_VID_SetDsReqSize    (HI_U32 u32Data)
{
    U_V0_FI_REQW_1 V0_FI_REQW_1;

    V0_FI_REQW_1.u32 = u32Data ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_FI_REQW_1.u32) + VDP_LAYER_VID6 * VID_OFFSET), V0_FI_REQW_1.u32);

    return ;
}
HI_VOID  VDP_VID_SetEsEnable (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_CTRL  V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetEsEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.es_en = u32Enable ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetESMarketMode(HI_U32 u32Data, HI_U32 mk_en, HI_U32 mk_mode)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESMarketMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.mk_en   = mk_en;
    ES_MODE.bits.mk_mode = mk_mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESInterlacedProc(HI_U32 u32Data, HI_U32 interlaced_proc)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESInterlacedProc() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.interlaced_proc = interlaced_proc ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESDirMode(HI_U32 u32Data, HI_U32 dir_mode)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESDirMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.dir_mode = dir_mode ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESCheck1Enable(HI_U32 u32Data, HI_U32 check1_en)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESCheck1Enable() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.check1_en = check1_en ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESCheck2Enable(HI_U32 u32Data, HI_U32 check2_en)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESCheck2Enable() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.check2_en = check2_en ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESAlterDirEnable(HI_U32 u32Data, HI_U32 alter_dir_en)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESAlterDirEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.alter_dir_en = alter_dir_en ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeStrengthMode(HI_U32 u32Data, HI_U32 edge_strength_mode)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESEdgeStrengthMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.edge_strength_mode = edge_strength_mode ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESBlendMode(HI_U32 u32Data, HI_U32 blend_mode)
{
    U_ES_MODE ES_MODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESBlendMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.blend_mode = blend_mode ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESDirRatio(HI_U32 u32Data, HI_U32 dir_ratio[9])
{
    U_ES_DIR_RATIO_0 ES_DIR_RATIO_0;
    U_ES_DIR_RATIO_1 ES_DIR_RATIO_1;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESDirRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_DIR_RATIO_0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_DIR_RATIO_0.u32) + u32Data * VID_OFFSET));
    ES_DIR_RATIO_1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_DIR_RATIO_1.u32) + u32Data * VID_OFFSET));
    ES_DIR_RATIO_0.bits.dir_ratio_0 = dir_ratio[0] ;
    ES_DIR_RATIO_0.bits.dir_ratio_1 = dir_ratio[1] ;
    ES_DIR_RATIO_0.bits.dir_ratio_2 = dir_ratio[2] ;
    ES_DIR_RATIO_0.bits.dir_ratio_3 = dir_ratio[3] ;
    ES_DIR_RATIO_0.bits.dir_ratio_4 = dir_ratio[4] ;
    ES_DIR_RATIO_0.bits.dir_ratio_5 = dir_ratio[5] ;
    ES_DIR_RATIO_0.bits.dir_ratio_6 = dir_ratio[6] ;
    ES_DIR_RATIO_0.bits.dir_ratio_7 = dir_ratio[7] ;
    ES_DIR_RATIO_1.bits.dir_ratio_8 = dir_ratio[8] ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_DIR_RATIO_0.u32) + u32Data * VID_OFFSET), ES_DIR_RATIO_0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_DIR_RATIO_1.u32) + u32Data * VID_OFFSET), ES_DIR_RATIO_1.u32);
    return;
}

HI_VOID VDP_VID_SetESNormalizedScale(HI_U32 u32Data, HI_U32 normalized_scale[9])
{
    U_ES_NORMALIZED_SCALE_0 ES_NORMALIZED_SCALE_0;
    U_ES_NORMALIZED_SCALE_1 ES_NORMALIZED_SCALE_1;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESNormalizedScale() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_NORMALIZED_SCALE_0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_NORMALIZED_SCALE_0.u32) + u32Data * VID_OFFSET));
    ES_NORMALIZED_SCALE_1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_NORMALIZED_SCALE_1.u32) + u32Data * VID_OFFSET));
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_0 = normalized_scale[0] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_1 = normalized_scale[1] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_2 = normalized_scale[2] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_3 = normalized_scale[3] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_4 = normalized_scale[4] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_5 = normalized_scale[5] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_6 = normalized_scale[6] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_7 = normalized_scale[7] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_8 = normalized_scale[8] ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_NORMALIZED_SCALE_0.u32) + u32Data * VID_OFFSET), ES_NORMALIZED_SCALE_0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_NORMALIZED_SCALE_1.u32) + u32Data * VID_OFFSET), ES_NORMALIZED_SCALE_1.u32);
    return;
}

HI_VOID VDP_VID_SetESBC1CFG(HI_U32 u32Data, HI_U32 check1_deadzone, HI_U32 check1_autodz_gain, HI_U32 check1_gain, HI_U32 chech1_dir_diff_gain)
{
    U_ES_BC1_CFG ES_BC1_CFG;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESBC1CFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC1_CFG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_BC1_CFG.u32) + u32Data * VID_OFFSET));
    //ES_BC1_CFG.bits.check1_deadzone      = check1_deadzone <<2 ;//z221213
    ES_BC1_CFG.bits.check1_deadzone      = check1_deadzone ;
    ES_BC1_CFG.bits.check1_autodz_gain   = check1_autodz_gain   ;
    ES_BC1_CFG.bits.check1_gain          = check1_gain          ;
    ES_BC1_CFG.bits.chech1_dir_diff_gain = chech1_dir_diff_gain ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_BC1_CFG.u32) + u32Data * VID_OFFSET), ES_BC1_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESBC2CFG(HI_U32 u32Data, HI_U32 check2_deadzone, HI_U32 check2_autodz_gain, HI_U32 check2_gain)
{
    U_ES_BC2_CFG ES_BC2_CFG;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESBC2CFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC2_CFG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_BC2_CFG.u32) + u32Data * VID_OFFSET));
    //ES_BC2_CFG.bits.check2_deadzone      = check2_deadzone <<2 ;//z221213
    ES_BC2_CFG.bits.check2_deadzone      = check2_deadzone  ;
    ES_BC2_CFG.bits.check2_autodz_gain   = check2_autodz_gain   ;
    ES_BC2_CFG.bits.check2_gain          = check2_gain          ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_BC2_CFG.u32) + u32Data * VID_OFFSET), ES_BC2_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESBCCFG(HI_U32 u32Data, HI_U32 check_gain)
{
    U_ES_BC_CFG ES_BC_CFG;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESBCCFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC_CFG.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_BC_CFG.u32) + u32Data * VID_OFFSET));
    ES_BC_CFG.bits.check_gain   = check_gain    ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_BC_CFG.u32) + u32Data * VID_OFFSET), ES_BC_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESRatioLimitScale(HI_U32 u32Data, HI_U32 ratio_limit_scale)
{
    U_ES_RATIO_LIMIT_SCALE ES_RATIO_LIMIT_SCALE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESRatioLimitScale() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_RATIO_LIMIT_SCALE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_RATIO_LIMIT_SCALE.u32) + u32Data * VID_OFFSET));
    ES_RATIO_LIMIT_SCALE.bits.ratio_limit_scale   = ratio_limit_scale  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_RATIO_LIMIT_SCALE.u32) + u32Data * VID_OFFSET), ES_RATIO_LIMIT_SCALE.u32);
    return;
}

HI_VOID VDP_VID_SetESVerMinStrength(HI_U32 u32Data, HI_S32 ver_min_strength)
{
    U_ES_VER_MIN_STRENGTH ES_VER_MIN_STRENGTH;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESVerMinStrength() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_VER_MIN_STRENGTH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_VER_MIN_STRENGTH.u32) + u32Data * VID_OFFSET));
    //ES_VER_MIN_STRENGTH.bits.ver_min_strength   = ver_min_strength <<2 ;//z221213
    ES_VER_MIN_STRENGTH.bits.ver_min_strength   = ver_min_strength ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_VER_MIN_STRENGTH.u32) + u32Data * VID_OFFSET), ES_VER_MIN_STRENGTH.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSmoothGain(HI_U32 u32Data, HI_U32 edge_smooth_gain)
{
    U_ES_EDGE_SMOOTH_GAIN ES_EDGE_SMOOTH_GAIN;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESEdgeSmoothGain() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SMOOTH_GAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_GAIN.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_GAIN.bits.edge_smooth_gain   = edge_smooth_gain  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_GAIN.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_GAIN.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeThd(HI_U32 u32Data, HI_U32 edge_thd[4])
{
    U_ES_EDGE_THD ES_EDGE_THD;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESEdgeThd() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_THD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_THD.u32) + u32Data * VID_OFFSET));
    //ES_EDGE_THD.bits.edge_thd_0   = edge_thd[0] <<2  ; //z221213
    //ES_EDGE_THD.bits.edge_thd_1   = edge_thd[1] <<2  ;
    //ES_EDGE_THD.bits.edge_thd_2   = edge_thd[2] <<2  ;
    //ES_EDGE_THD.bits.edge_thd_3   = edge_thd[3] <<2  ;
    ES_EDGE_THD.bits.edge_thd_0   = edge_thd[0]  ;
    ES_EDGE_THD.bits.edge_thd_1   = edge_thd[1]  ;
    ES_EDGE_THD.bits.edge_thd_2   = edge_thd[2]  ;
    ES_EDGE_THD.bits.edge_thd_3   = edge_thd[3]  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_THD.u32) + u32Data * VID_OFFSET), ES_EDGE_THD.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSmoothRatio(HI_U32 u32Data, HI_U32 edge_smooth_ratio[5])
{
    U_ES_EDGE_SMOOTH_RATIO_0 ES_EDGE_SMOOTH_RATIO_0;
    U_ES_EDGE_SMOOTH_RATIO_1 ES_EDGE_SMOOTH_RATIO_1;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESEdgeSmoothRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SMOOTH_RATIO_0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_RATIO_0.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_RATIO_1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_RATIO_1.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_0   = edge_smooth_ratio[0]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_1   = edge_smooth_ratio[1]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_2   = edge_smooth_ratio[2]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_3   = edge_smooth_ratio[3]  ;
    ES_EDGE_SMOOTH_RATIO_1.bits.edge_smooth_ratio_4   = edge_smooth_ratio[4]  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_RATIO_0.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_RATIO_0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SMOOTH_RATIO_1.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_RATIO_1.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSlope(HI_U32 u32Data, HI_S32 edge_slope[5])
{
    U_ES_EDGE_SLOPE_0 ES_EDGE_SLOPE_0;
    U_ES_EDGE_SLOPE_1 ES_EDGE_SLOPE_1;
    U_ES_EDGE_SLOPE_2 ES_EDGE_SLOPE_2;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetESEdgeSlope() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SLOPE_0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_0.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SLOPE_1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_1.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SLOPE_2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_2.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SLOPE_0.bits.edge_slope_0   = edge_slope[0]  ;
    ES_EDGE_SLOPE_0.bits.edge_slope_1   = edge_slope[1]  ;
    ES_EDGE_SLOPE_1.bits.edge_slope_2   = edge_slope[2]  ;
    ES_EDGE_SLOPE_1.bits.edge_slope_3   = edge_slope[3]  ;
    ES_EDGE_SLOPE_2.bits.edge_slope_4   = edge_slope[4]  ;
    //ES_EDGE_SLOPE_0.bits.edge_slope_0   = ((edge_smooth_ratio[0]                       ) <<(10 - 2)) / (edge_thd[0]              )  ;
    //ES_EDGE_SLOPE_0.bits.edge_slope_1   = ((edge_smooth_ratio[1] - edge_smooth_ratio[0]) <<(10 - 2)) / (edge_thd[1] - edge_thd[0])  ;
    //ES_EDGE_SLOPE_1.bits.edge_slope_2   = ((edge_smooth_ratio[2] - edge_smooth_ratio[1]) <<(10 - 2)) / (edge_thd[2] - edge_thd[1])  ;
    //ES_EDGE_SLOPE_1.bits.edge_slope_3   = ((edge_smooth_ratio[3] - edge_smooth_ratio[2]) <<(10 - 2)) / (edge_thd[3] - edge_thd[2])  ;
    //ES_EDGE_SLOPE_2.bits.edge_slope_4   = ((edge_smooth_ratio[4] - edge_smooth_ratio[3]) <<(10 - 2)) / (edge_thd[4] - edge_thd[3])  ;

    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_0.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_0.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_1.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_1.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->ES_EDGE_SLOPE_2.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_2.u32);
    return;
}
//-----------------------------------------------------------------------------------
//SHARPEN_BEGIN
//-----------------------------------------------------------------------------------
#if 1
//sharpen cfg
HI_VOID VDP_VID_SetSharpDbgEn(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPCTRL SPCTRL;

    if(u32LayerId >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.debug_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetSharpDemoMode(HI_U32 u32LayerId, HI_U32 DemoMode)
{
    U_SPCTRL SPCTRL;

    if(u32LayerId >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.demo_mode = DemoMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetSharpEnable(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    //temp verision
    SPCTRL.bits.sharpen_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}


HI_VOID VDP_VID_SetSharpDispMode(HI_U32 u32LayerId, HI_U32 u32DispMode)
{
    U_SPCTRL SPCTRL;

    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.disp_mode = u32DispMode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetSharpGlbThd(HI_U32 u32LayerId, HI_U32 u32UnderThd, HI_U32 u32OverThd)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.glb_undersh_thd = u32UnderThd;
    SPCTRL.bits.glb_oversh_thd  = u32OverThd;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetSharpMergeMode(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.merge_mode = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetSharpDetailAdj(HI_U32 u32LayerId, HI_U32 bEnable, HI_U32 u32Thd)
{
    U_SPDETAILADJ SPDETAILADJ;
    SPDETAILADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDETAILADJ.u32) + u32LayerId * VID_OFFSET));
    SPDETAILADJ.bits.detail_adj_en = bEnable;
    SPDETAILADJ.bits.detail_adj_thd = u32Thd;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDETAILADJ.u32) + u32LayerId * VID_OFFSET), SPDETAILADJ.u32);
}

HI_VOID VDP_VID_SetSharpPeakRatio(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPRATIO SPRATIO;
    SPRATIO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET));
    SPRATIO.bits.peak_ratio = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET), SPRATIO.u32);
}

HI_VOID VDP_VID_SetSharpLtiRatio(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPRATIO SPRATIO;
    SPRATIO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET));
    SPRATIO.bits.lti_ratio = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET), SPRATIO.u32);
}

HI_VOID VDP_VID_SetSharpShootDiv(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPRATIO SPRATIO;
    SPRATIO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET));
    SPRATIO.bits.adp_shoot_div = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET), SPRATIO.u32);
}

HI_VOID VDP_VID_SetSharpLutDiv(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPRATIO SPRATIO;
    SPRATIO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET));
    SPRATIO.bits.color_adj_div = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIO.u32) + u32LayerId * VID_OFFSET), SPRATIO.u32);
}

HI_VOID VDP_VID_SetSharpColorAdj(HI_U32 u32LayerId, HI_U32 bEnable, HI_U32 u32Low, HI_U32 u32High)
{
    U_SPCOLORADJ SPCOLORADJ;
    SPCOLORADJ.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCOLORADJ.u32) + u32LayerId * VID_OFFSET));
    SPCOLORADJ.bits.color_adj_en   = bEnable;
    SPCOLORADJ.bits.color_adj_low  = u32Low;
    SPCOLORADJ.bits.color_adj_high = u32High;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCOLORADJ.u32) + u32LayerId * VID_OFFSET), SPCOLORADJ.u32);
}

HI_VOID VDP_VID_SetSharpHighFreq(HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, VDP_SHARP_GROUP_S * stCfg)
{
    U_SPRATIOH           SPRATIOH;
    U_SPRATIOV           SPRATIOV;
    U_SPRATIOD           SPRATIOD;
    U_SPCORINGH          SPCORINGH;
    U_SPCORINGV          SPCORINGV;
    U_SPCORINGD          SPCORINGD;
    U_SPWINSIZEHV        SPWINSIZEHV;
    U_SPWINSIZED         SPWINSIZED;
    U_SPGRDH11           SPGRDH11;
    U_SPGRDH12           SPGRDH12;
    U_SPGRDH21           SPGRDH21;
    U_SPGRDH22           SPGRDH22;
    U_SPGRDH31           SPGRDH31;
    U_SPGRDH32           SPGRDH32;
    U_SPGRDH41           SPGRDH41;
    U_SPGRDH42           SPGRDH42;
    U_SPGRDV0            SPGRDV0;
    U_SPGRDV1            SPGRDV1;
    U_SPGRDV2            SPGRDV2;
    U_SPGRDV3            SPGRDV3;
    U_SPGRDD0            SPGRDD0;
    U_SPGRDD1            SPGRDD1;
    U_SPGRDD2            SPGRDD2;
    U_SPGRDD3            SPGRDD3;
    U_SPGAINEN           SPGAINEN;
    U_SPSLPH1            SPSLPH1;
    U_SPSLPH2            SPSLPH2;
    U_SPSLPH3            SPSLPH3;
    U_SPSLPH4            SPSLPH4;
    U_SPSLPV1            SPSLPV1;
    U_SPSLPV2            SPSLPV2;
    U_SPSLPD1            SPSLPD1;
    U_SPSLPD2            SPSLPD2;
    U_SPGAINMINLH1       SPGAINMINLH1;
    U_SPGAINMINLH2       SPGAINMINLH2;
    U_SPGAINMINLV        SPGAINMINLV;
    U_SPGAINMINLD        SPGAINMINLD;
    U_SPGAINMINHH1       SPGAINMINHH1;
    U_SPGAINMINHH2       SPGAINMINHH2;
    U_SPGAINMINHV        SPGAINMINHV;
   // U_SPGAINMINHD        SPGAINMINHD;
    U_SPGAINMAXH1        SPGAINMAXH1;
    U_SPGAINMAXH2        SPGAINMAXH2;
    U_SPGAINMAXV         SPGAINMAXV;
    U_SPGAINMAXD         SPGAINMAXD;
    U_SPGAINPOSH1        SPGAINPOSH1;
    U_SPGAINPOSH2        SPGAINPOSH2;
    U_SPGAINPOSV         SPGAINPOSV;
    U_SPGAINPOSD         SPGAINPOSD;
    U_SPGAINNEGH1        SPGAINNEGH1;
    U_SPGAINNEGH2        SPGAINNEGH2;
    U_SPGAINNEGV         SPGAINNEGV;
    U_SPGAINNEGD         SPGAINNEGD;
    U_SPOVERTHDH         SPOVERTHDH;
    U_SPOVERTHDV         SPOVERTHDV;
    U_SPOVERTHDD         SPOVERTHDD;
    U_SPUNDERTHDH        SPUNDERTHDH;
    U_SPUNDERTHDV        SPUNDERTHDV;
    U_SPUNDERTHDD        SPUNDERTHDD;
    U_SPTMPH1            SPTMPH1;
    U_SPTMPH2            SPTMPH2;
    U_SPTMPH3            SPTMPH3;
    U_SPTMPH4            SPTMPH4;
    U_SPTMPV1            SPTMPV1;
    U_SPTMPV2            SPTMPV2;
    U_SPTMPV3            SPTMPV3;
    U_SPTMPD1            SPTMPD1;
    U_SPTMPD2            SPTMPD2;
    U_SPTMPD3            SPTMPD3;
//    U_SPTIGAIN           SPTIGAIN;
    U_SPENABLE           SPENABLE;
    U_SPSHOOTEN          SPSHOOTEN;
    switch(enDir)
    {
        case VDP_SHARP_DIR_H:
        {
            //bEnable
            SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
            SPENABLE.bits.hfreq_en_h1 = stCfg->stSharpU1.bEnable;
            SPENABLE.bits.hfreq_en_h2 = stCfg->stSharpU2.bEnable;
            SPENABLE.bits.hfreq_en_h3 = stCfg->stSharpU3.bEnable;
            SPENABLE.bits.hfreq_en_h4 = stCfg->stSharpU4.bEnable;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET), SPENABLE.u32);

           //u8MixRatio
           SPRATIOH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIOH.u32) + u32LayerId * VID_OFFSET));
           SPRATIOH.bits.mixratio_h1 = stCfg->stSharpU1.u8MixRatio;
           SPRATIOH.bits.mixratio_h2 = stCfg->stSharpU2.u8MixRatio;
           SPRATIOH.bits.mixratio_h3 = stCfg->stSharpU3.u8MixRatio;
           SPRATIOH.bits.mixratio_h4 = stCfg->stSharpU4.u8MixRatio;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIOH.u32) + u32LayerId * VID_OFFSET), SPRATIOH.u32);

           //u8Coring
           SPCORINGH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCORINGH.u32) + u32LayerId * VID_OFFSET));
           SPCORINGH.bits.coring_h1 = stCfg->stSharpU1.u8Coring;
           SPCORINGH.bits.coring_h2 = stCfg->stSharpU2.u8Coring;
           SPCORINGH.bits.coring_h3 = stCfg->stSharpU3.u8Coring;
           SPCORINGH.bits.coring_h4 = stCfg->stSharpU4.u8Coring;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPCORINGH.u32) + u32LayerId * VID_OFFSET), SPCORINGH.u32);

           //u8WinSize
           SPWINSIZEHV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPWINSIZEHV.u32) + u32LayerId * VID_OFFSET));
           SPWINSIZEHV.bits.winsize_h1 = stCfg->stSharpU1.u8WinSize;
           SPWINSIZEHV.bits.winsize_h2 = stCfg->stSharpU2.u8WinSize;
           SPWINSIZEHV.bits.winsize_h3 = stCfg->stSharpU3.u8WinSize;
           SPWINSIZEHV.bits.winsize_h4 = stCfg->stSharpU4.u8WinSize;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPWINSIZEHV.u32) + u32LayerId * VID_OFFSET), SPWINSIZEHV.u32);

           //u16Grd0,u16Grd1
           SPGRDH11.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH11.u32) + u32LayerId * VID_OFFSET));
           SPGRDH11.bits.grd0_h1 = stCfg->stSharpU1.u16Grd0;
           SPGRDH11.bits.grd1_h1 = stCfg->stSharpU1.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH11.u32) + u32LayerId * VID_OFFSET), SPGRDH11.u32);

           SPGRDH21.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH21.u32) + u32LayerId * VID_OFFSET));
           SPGRDH21.bits.grd0_h2 = stCfg->stSharpU2.u16Grd0;
           SPGRDH21.bits.grd1_h2 = stCfg->stSharpU2.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH21.u32) + u32LayerId * VID_OFFSET), SPGRDH21.u32);

           SPGRDH31.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH31.u32) + u32LayerId * VID_OFFSET));
           SPGRDH31.bits.grd0_h3 = stCfg->stSharpU3.u16Grd0;
           SPGRDH31.bits.grd1_h3 = stCfg->stSharpU3.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH31.u32) + u32LayerId * VID_OFFSET), SPGRDH31.u32);

           SPGRDH41.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH41.u32) + u32LayerId * VID_OFFSET));
           SPGRDH41.bits.grd0_h4 = stCfg->stSharpU4.u16Grd0;
           SPGRDH41.bits.grd1_h4 = stCfg->stSharpU4.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH41.u32) + u32LayerId * VID_OFFSET), SPGRDH41.u32);

           //u16Grd2,u16Grd3
           SPGRDH12.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH12.u32) + u32LayerId * VID_OFFSET));
           SPGRDH12.bits.grd2_h1 = stCfg->stSharpU1.u16Grd2;
           SPGRDH12.bits.grd3_h1 = stCfg->stSharpU1.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH12.u32) + u32LayerId * VID_OFFSET), SPGRDH12.u32);

           SPGRDH22.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH22.u32) + u32LayerId * VID_OFFSET));
           SPGRDH22.bits.grd2_h2 = stCfg->stSharpU2.u16Grd2;
           SPGRDH22.bits.grd3_h2 = stCfg->stSharpU2.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH22.u32) + u32LayerId * VID_OFFSET), SPGRDH22.u32);

           SPGRDH32.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH32.u32) + u32LayerId * VID_OFFSET));
           SPGRDH32.bits.grd2_h3 = stCfg->stSharpU3.u16Grd2;
           SPGRDH32.bits.grd3_h3 = stCfg->stSharpU3.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH32.u32) + u32LayerId * VID_OFFSET), SPGRDH32.u32);

           SPGRDH42.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDH42.u32) + u32LayerId * VID_OFFSET));
           SPGRDH42.bits.grd2_h4 = stCfg->stSharpU4.u16Grd2;
           SPGRDH42.bits.grd3_h4 = stCfg->stSharpU4.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDH42.u32) + u32LayerId * VID_OFFSET), SPGRDH42.u32);

           //bEnAdpGain
           SPGAINEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET));
           SPGAINEN.bits.h1_enable = stCfg->stSharpU1.bEnAdpGain;
           SPGAINEN.bits.h2_enable = stCfg->stSharpU2.bEnAdpGain;
           SPGAINEN.bits.h3_enable = stCfg->stSharpU3.bEnAdpGain;
           SPGAINEN.bits.h4_enable = stCfg->stSharpU4.bEnAdpGain;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET), SPGAINEN.u32);

           //s16Slp1,s16Slp2
           SPSLPH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPH1.u32) + u32LayerId * VID_OFFSET));
           SPSLPH1.bits.slp1_h1 = stCfg->stSharpU1.s16Slp1;
           SPSLPH1.bits.slp2_h1 = stCfg->stSharpU1.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPH1.u32) + u32LayerId * VID_OFFSET), SPSLPH1.u32);

           SPSLPH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPH2.u32) + u32LayerId * VID_OFFSET));
           SPSLPH2.bits.slp1_h2 = stCfg->stSharpU2.s16Slp1;
           SPSLPH2.bits.slp2_h2 = stCfg->stSharpU2.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPH2.u32) + u32LayerId * VID_OFFSET), SPSLPH2.u32);

           SPSLPH3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPH3.u32) + u32LayerId * VID_OFFSET));
           SPSLPH3.bits.slp1_h3 = stCfg->stSharpU3.s16Slp1;
           SPSLPH3.bits.slp2_h3 = stCfg->stSharpU3.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPH3.u32) + u32LayerId * VID_OFFSET), SPSLPH3.u32);

           SPSLPH4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPH4.u32) + u32LayerId * VID_OFFSET));
           SPSLPH4.bits.slp1_h4 = stCfg->stSharpU4.s16Slp1;
           SPSLPH4.bits.slp2_h4 = stCfg->stSharpU4.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPH4.u32) + u32LayerId * VID_OFFSET), SPSLPH4.u32);

           //s16GainMinL
           SPGAINMINLH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINLH1.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINLH1.bits.gain_minl_h2 = stCfg->stSharpU2.s16GainMinL;
           SPGAINMINLH1.bits.gain_minl_h3 = stCfg->stSharpU3.s16GainMinL;
           SPGAINMINLH1.bits.gain_minl_h4 = stCfg->stSharpU4.s16GainMinL;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINLH1.u32) + u32LayerId * VID_OFFSET), SPGAINMINLH1.u32);

           SPGAINMINLH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINLH2.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINLH2.bits.gain_minl_h1 = stCfg->stSharpU1.s16GainMinL;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINLH2.u32) + u32LayerId * VID_OFFSET), SPGAINMINLH2.u32);

           //s16GainMinH
           SPGAINMINHH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINHH1.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINHH1.bits.gain_minh_h2 = stCfg->stSharpU2.s16GainMinH;
           SPGAINMINHH1.bits.gain_minh_h3 = stCfg->stSharpU3.s16GainMinH;
           SPGAINMINHH1.bits.gain_minh_h4 = stCfg->stSharpU4.s16GainMinH;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINHH1.u32) + u32LayerId * VID_OFFSET), SPGAINMINHH1.u32);

           SPGAINMINHH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINHH2.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINHH2.bits.gain_minh_h1 = stCfg->stSharpU1.s16GainMinH;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINHH2.u32) + u32LayerId * VID_OFFSET), SPGAINMINHH2.u32);

           //s16GainMax
           SPGAINMAXH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMAXH1.u32) + u32LayerId * VID_OFFSET));
           SPGAINMAXH1.bits.gain_max_h2 = stCfg->stSharpU2.s16GainMax;
           SPGAINMAXH1.bits.gain_max_h3 = stCfg->stSharpU3.s16GainMax;
           SPGAINMAXH1.bits.gain_max_h4 = stCfg->stSharpU4.s16GainMax;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMAXH1.u32) + u32LayerId * VID_OFFSET), SPGAINMAXH1.u32);

           SPGAINMAXH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMAXH2.u32) + u32LayerId * VID_OFFSET));
           SPGAINMAXH2.bits.gain_max_h1 = stCfg->stSharpU1.s16GainMax;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMAXH2.u32) + u32LayerId * VID_OFFSET), SPGAINMAXH2.u32);

           //s16GainPos
           SPGAINPOSH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINPOSH1.u32) + u32LayerId * VID_OFFSET));
           SPGAINPOSH1.bits.gain_pos_h2 = stCfg->stSharpU2.s16GainPos;
           SPGAINPOSH1.bits.gain_pos_h3 = stCfg->stSharpU3.s16GainPos;
           SPGAINPOSH1.bits.gain_pos_h4 = stCfg->stSharpU4.s16GainPos;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINPOSH1.u32) + u32LayerId * VID_OFFSET), SPGAINPOSH1.u32);

           SPGAINPOSH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINPOSH2.u32) + u32LayerId * VID_OFFSET));
           SPGAINPOSH2.bits.gain_pos_h1 = stCfg->stSharpU1.s16GainPos;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINPOSH2.u32) + u32LayerId * VID_OFFSET), SPGAINPOSH2.u32);

           //s16GainNeg
           SPGAINNEGH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINNEGH1.u32) + u32LayerId * VID_OFFSET));
           SPGAINNEGH1.bits.gain_neg_h2 = stCfg->stSharpU2.s16GainNeg;
           SPGAINNEGH1.bits.gain_neg_h3 = stCfg->stSharpU3.s16GainNeg;
           SPGAINNEGH1.bits.gain_neg_h4 = stCfg->stSharpU4.s16GainNeg;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINNEGH1.u32) + u32LayerId * VID_OFFSET), SPGAINNEGH1.u32);

           SPGAINNEGH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINNEGH2.u32) + u32LayerId * VID_OFFSET));
           SPGAINNEGH2.bits.gain_neg_h1 = stCfg->stSharpU1.s16GainNeg;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINNEGH2.u32) + u32LayerId * VID_OFFSET), SPGAINNEGH2.u32);

           //u8OverThrsh
           SPOVERTHDH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPOVERTHDH.u32) + u32LayerId * VID_OFFSET));
           SPOVERTHDH.bits.over_thd_h1 = stCfg->stSharpU1.u8OverThrsh;
           SPOVERTHDH.bits.over_thd_h2 = stCfg->stSharpU2.u8OverThrsh;
           SPOVERTHDH.bits.over_thd_h3 = stCfg->stSharpU3.u8OverThrsh;
           SPOVERTHDH.bits.over_thd_h4 = stCfg->stSharpU4.u8OverThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPOVERTHDH.u32) + u32LayerId * VID_OFFSET), SPOVERTHDH.u32);

           //u8UnderThrsh
           SPUNDERTHDH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPUNDERTHDH.u32) + u32LayerId * VID_OFFSET));
           SPUNDERTHDH.bits.under_thd_h1 = stCfg->stSharpU1.u8UnderThrsh;
           SPUNDERTHDH.bits.under_thd_h2 = stCfg->stSharpU2.u8UnderThrsh;
           SPUNDERTHDH.bits.under_thd_h3 = stCfg->stSharpU3.u8UnderThrsh;
           SPUNDERTHDH.bits.under_thd_h4 = stCfg->stSharpU4.u8UnderThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPUNDERTHDH.u32) + u32LayerId * VID_OFFSET), SPUNDERTHDH.u32);

           //s16Tmp0,s16Tmp1,s16Tmp2,s16Tmp3
           SPTMPH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPH1.u32) + u32LayerId * VID_OFFSET));
           SPTMPH1.bits.tmp0_h1 = stCfg->stSharpU1.s16Tmp0;
           SPTMPH1.bits.tmp1_h1 = stCfg->stSharpU1.s16Tmp1;
           SPTMPH1.bits.tmp2_h1 = stCfg->stSharpU1.s16Tmp2;
           SPTMPH1.bits.tmp3_h1 = stCfg->stSharpU1.s16Tmp3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPH1.u32) + u32LayerId * VID_OFFSET), SPTMPH1.u32);

           SPTMPH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPH2.u32) + u32LayerId * VID_OFFSET));
           SPTMPH2.bits.tmp0_h2 = stCfg->stSharpU2.s16Tmp0;
           SPTMPH2.bits.tmp1_h2 = stCfg->stSharpU2.s16Tmp1;
           SPTMPH2.bits.tmp2_h2 = stCfg->stSharpU2.s16Tmp2;
           SPTMPH2.bits.tmp3_h2 = stCfg->stSharpU2.s16Tmp3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPH2.u32) + u32LayerId * VID_OFFSET), SPTMPH2.u32);

           SPTMPH3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPH3.u32) + u32LayerId * VID_OFFSET));
           SPTMPH3.bits.tmp0_h3 = stCfg->stSharpU3.s16Tmp0;
           SPTMPH3.bits.tmp1_h3 = stCfg->stSharpU3.s16Tmp1;
           SPTMPH3.bits.tmp2_h3 = stCfg->stSharpU3.s16Tmp2;
           SPTMPH3.bits.tmp3_h3 = stCfg->stSharpU3.s16Tmp3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPH3.u32) + u32LayerId * VID_OFFSET), SPTMPH3.u32);

           SPTMPH4.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPH4.u32) + u32LayerId * VID_OFFSET));
           SPTMPH4.bits.tmp0_h4 = stCfg->stSharpU4.s16Tmp0;
           SPTMPH4.bits.tmp1_h4 = stCfg->stSharpU4.s16Tmp1;
           SPTMPH4.bits.tmp2_h4 = stCfg->stSharpU4.s16Tmp2;
           SPTMPH4.bits.tmp3_h4 = stCfg->stSharpU4.s16Tmp3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPH4.u32) + u32LayerId * VID_OFFSET), SPTMPH4.u32);

           //bEnAdpShoot
           SPSHOOTEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET));
           SPSHOOTEN.bits.shoot_en_h1 = stCfg->stSharpU1.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_h2 = stCfg->stSharpU2.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_h3 = stCfg->stSharpU3.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_h4 = stCfg->stSharpU4.bEnAdpShoot;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET), SPSHOOTEN.u32);


           break;
       }
       case VDP_SHARP_DIR_V:
       {
           //bEnable
           SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
           SPENABLE.bits.hfreq_en_v1 = stCfg->stSharpU1.bEnable;
           SPENABLE.bits.hfreq_en_v2 = stCfg->stSharpU2.bEnable;
           SPENABLE.bits.hfreq_en_v3 = stCfg->stSharpU3.bEnable;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET), SPENABLE.u32);

           //u8MixRatio
           SPRATIOV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIOV.u32) + u32LayerId * VID_OFFSET));
           SPRATIOV.bits.mixratio_v1 = stCfg->stSharpU1.u8MixRatio;
           SPRATIOV.bits.mixratio_v2 = stCfg->stSharpU2.u8MixRatio;
           SPRATIOV.bits.mixratio_v3 = stCfg->stSharpU3.u8MixRatio;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIOV.u32) + u32LayerId * VID_OFFSET), SPRATIOV.u32);

           //u8Coring
           SPCORINGV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCORINGV.u32) + u32LayerId * VID_OFFSET));
           SPCORINGV.bits.coring_v1 = stCfg->stSharpU1.u8Coring;
           SPCORINGV.bits.coring_v2 = stCfg->stSharpU2.u8Coring;
           SPCORINGV.bits.coring_v3 = stCfg->stSharpU3.u8Coring;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPCORINGV.u32) + u32LayerId * VID_OFFSET), SPCORINGV.u32);

           //u8WinSize
           SPWINSIZEHV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPWINSIZEHV.u32) + u32LayerId * VID_OFFSET));
           SPWINSIZEHV.bits.winsize_v1 = stCfg->stSharpU1.u8WinSize;
           SPWINSIZEHV.bits.winsize_v2 = stCfg->stSharpU2.u8WinSize;
           SPWINSIZEHV.bits.winsize_v3 = stCfg->stSharpU3.u8WinSize;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPWINSIZEHV.u32) + u32LayerId * VID_OFFSET), SPWINSIZEHV.u32);

           //u16Grd0
           SPGRDV0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDV0.u32) + u32LayerId * VID_OFFSET));
           SPGRDV0.bits.grd0_v1 = stCfg->stSharpU1.u16Grd0;
           SPGRDV0.bits.grd0_v2 = stCfg->stSharpU2.u16Grd0;
           SPGRDV0.bits.grd0_v3 = stCfg->stSharpU3.u16Grd0;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDV0.u32) + u32LayerId * VID_OFFSET), SPGRDV0.u32);

           //u16Grd1
           SPGRDV1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDV1.u32) + u32LayerId * VID_OFFSET));
           SPGRDV1.bits.grd1_v1 = stCfg->stSharpU1.u16Grd1;
           SPGRDV1.bits.grd1_v2 = stCfg->stSharpU2.u16Grd1;
           SPGRDV1.bits.grd1_v3 = stCfg->stSharpU3.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDV1.u32) + u32LayerId * VID_OFFSET), SPGRDV1.u32);

           //u16Grd2
           SPGRDV2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDV2.u32) + u32LayerId * VID_OFFSET));
           SPGRDV2.bits.grd2_v1 = stCfg->stSharpU1.u16Grd2;
           SPGRDV2.bits.grd2_v2 = stCfg->stSharpU2.u16Grd2;
           SPGRDV2.bits.grd2_v3 = stCfg->stSharpU3.u16Grd2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDV2.u32) + u32LayerId * VID_OFFSET), SPGRDV2.u32);

           //u16Grd3
           SPGRDV3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDV3.u32) + u32LayerId * VID_OFFSET));
           SPGRDV3.bits.grd3_v1 = stCfg->stSharpU1.u16Grd3;
           SPGRDV3.bits.grd3_v2 = stCfg->stSharpU2.u16Grd3;
           SPGRDV3.bits.grd3_v3 = stCfg->stSharpU3.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDV3.u32) + u32LayerId * VID_OFFSET), SPGRDV3.u32);

           //bEnAdpGain
           SPGAINEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET));
           SPGAINEN.bits.v1_enable = stCfg->stSharpU1.bEnAdpGain;
           SPGAINEN.bits.v2_enable = stCfg->stSharpU2.bEnAdpGain;
           SPGAINEN.bits.v3_enable = stCfg->stSharpU3.bEnAdpGain;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET), SPGAINEN.u32);

           //s16Slp1
           SPSLPV1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPV1.u32) + u32LayerId * VID_OFFSET));
           SPSLPV1.bits.slp1_v1 = stCfg->stSharpU1.s16Slp1;
           SPSLPV1.bits.slp1_v2 = stCfg->stSharpU2.s16Slp1;
           SPSLPV1.bits.slp1_v3 = stCfg->stSharpU3.s16Slp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPV1.u32) + u32LayerId * VID_OFFSET), SPSLPV1.u32);

           //s16Slp1
           SPSLPV2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPV2.u32) + u32LayerId * VID_OFFSET));
           SPSLPV2.bits.slp2_v1 = stCfg->stSharpU1.s16Slp2;
           SPSLPV2.bits.slp2_v2 = stCfg->stSharpU2.s16Slp2;
           SPSLPV2.bits.slp2_v3 = stCfg->stSharpU3.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPV2.u32) + u32LayerId * VID_OFFSET), SPSLPV2.u32);

           //s16GainMinL
           SPGAINMINLV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINLV.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINLV.bits.gain_minl_v1 = stCfg->stSharpU1.s16GainMinL;
           SPGAINMINLV.bits.gain_minl_v2 = stCfg->stSharpU2.s16GainMinL;
           SPGAINMINLV.bits.gain_minl_v3 = stCfg->stSharpU3.s16GainMinL;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINLV.u32) + u32LayerId * VID_OFFSET), SPGAINMINLV.u32);

           //s16GainMinH
           SPGAINMINHV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINHV.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINHV.bits.gain_minh_v1 = stCfg->stSharpU1.s16GainMinH;
           SPGAINMINHV.bits.gain_minh_v2 = stCfg->stSharpU2.s16GainMinH;
           SPGAINMINHV.bits.gain_minh_v3 = stCfg->stSharpU3.s16GainMinH;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINHV.u32) + u32LayerId * VID_OFFSET), SPGAINMINHV.u32);

           //s16GainMax
           SPGAINMAXV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMAXV.u32) + u32LayerId * VID_OFFSET));
           SPGAINMAXV.bits.gain_max_v1 = stCfg->stSharpU1.s16GainMax;
           SPGAINMAXV.bits.gain_max_v2 = stCfg->stSharpU2.s16GainMax;
           SPGAINMAXV.bits.gain_max_v3 = stCfg->stSharpU3.s16GainMax;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMAXV.u32) + u32LayerId * VID_OFFSET), SPGAINMAXV.u32);

           //s16GainPos
           SPGAINPOSV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINPOSV.u32) + u32LayerId * VID_OFFSET));
           SPGAINPOSV.bits.gain_pos_v1 = stCfg->stSharpU1.s16GainPos;
           SPGAINPOSV.bits.gain_pos_v2 = stCfg->stSharpU2.s16GainPos;
           SPGAINPOSV.bits.gain_pos_v3 = stCfg->stSharpU3.s16GainPos;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINPOSV.u32) + u32LayerId * VID_OFFSET), SPGAINPOSV.u32);

           //s16GainNeg
           SPGAINNEGV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINNEGV.u32) + u32LayerId * VID_OFFSET));
           SPGAINNEGV.bits.gain_neg_v1 = stCfg->stSharpU1.s16GainNeg;
           SPGAINNEGV.bits.gain_neg_v2 = stCfg->stSharpU2.s16GainNeg;
           SPGAINNEGV.bits.gain_neg_v3 = stCfg->stSharpU3.s16GainNeg;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINNEGV.u32) + u32LayerId * VID_OFFSET), SPGAINNEGV.u32);


           //u8OverThrsh
           SPOVERTHDV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPOVERTHDV.u32) + u32LayerId * VID_OFFSET));
           SPOVERTHDV.bits.over_thd_v1 = stCfg->stSharpU1.u8OverThrsh;
           SPOVERTHDV.bits.over_thd_v2 = stCfg->stSharpU2.u8OverThrsh;
           SPOVERTHDV.bits.over_thd_v3 = stCfg->stSharpU3.u8OverThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPOVERTHDV.u32) + u32LayerId * VID_OFFSET), SPOVERTHDV.u32);

           //u8UnderThrsh
           SPUNDERTHDV.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPUNDERTHDV.u32) + u32LayerId * VID_OFFSET));
           SPUNDERTHDV.bits.under_thd_v1 = stCfg->stSharpU1.u8UnderThrsh;
           SPUNDERTHDV.bits.under_thd_v2 = stCfg->stSharpU2.u8UnderThrsh;
           SPUNDERTHDV.bits.under_thd_v3 = stCfg->stSharpU3.u8UnderThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPUNDERTHDV.u32) + u32LayerId * VID_OFFSET), SPUNDERTHDV.u32);

           //s16Tmp0,s16Tmp1,s16Tmp2,s16Tmp3
           SPTMPV1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPV1.u32) + u32LayerId * VID_OFFSET));
           SPTMPV1.bits.tmp0_v1 = stCfg->stSharpU1.s16Tmp0;
           SPTMPV1.bits.tmp1_v1 = stCfg->stSharpU1.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPV1.u32) + u32LayerId * VID_OFFSET), SPTMPV1.u32);

           SPTMPV2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPV2.u32) + u32LayerId * VID_OFFSET));
           SPTMPV2.bits.tmp0_v2 = stCfg->stSharpU2.s16Tmp0;
           SPTMPV2.bits.tmp1_v2 = stCfg->stSharpU2.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPV2.u32) + u32LayerId * VID_OFFSET), SPTMPV2.u32);

           SPTMPV3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPV3.u32) + u32LayerId * VID_OFFSET));
           SPTMPV3.bits.tmp0_v3 = stCfg->stSharpU3.s16Tmp0;
           SPTMPV3.bits.tmp1_v3 = stCfg->stSharpU3.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPV3.u32) + u32LayerId * VID_OFFSET), SPTMPV3.u32);

           //bEnAdpShoot
           SPSHOOTEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET));
           SPSHOOTEN.bits.shoot_en_v1 = stCfg->stSharpU1.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_v2 = stCfg->stSharpU2.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_v3 = stCfg->stSharpU3.bEnAdpShoot;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET), SPSHOOTEN.u32);


           break;
       }
       case VDP_SHARP_DIR_D:
       {
           //bEnable
           SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
           SPENABLE.bits.hfreq_en_d1 = stCfg->stSharpU1.bEnable;
           SPENABLE.bits.hfreq_en_d2 = stCfg->stSharpU2.bEnable;
           SPENABLE.bits.hfreq_en_d3 = stCfg->stSharpU3.bEnable;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET), SPENABLE.u32);

           //u8MixRatio
           SPRATIOD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPRATIOD.u32) + u32LayerId * VID_OFFSET));
           SPRATIOD.bits.mixratio_d1 = stCfg->stSharpU1.u8MixRatio;
           SPRATIOD.bits.mixratio_d2 = stCfg->stSharpU2.u8MixRatio;
           SPRATIOD.bits.mixratio_d3 = stCfg->stSharpU3.u8MixRatio;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPRATIOD.u32) + u32LayerId * VID_OFFSET), SPRATIOD.u32);

           //u8Coring
           SPCORINGD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCORINGD.u32) + u32LayerId * VID_OFFSET));
           SPCORINGD.bits.coring_d1 = stCfg->stSharpU1.u8Coring;
           SPCORINGD.bits.coring_d2 = stCfg->stSharpU2.u8Coring;
           SPCORINGD.bits.coring_d3 = stCfg->stSharpU3.u8Coring;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPCORINGD.u32) + u32LayerId * VID_OFFSET), SPCORINGD.u32);

           //u8WinSize
           SPWINSIZED.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPWINSIZED.u32) + u32LayerId * VID_OFFSET));
           SPWINSIZED.bits.winsize_d1 = stCfg->stSharpU1.u8WinSize;
           SPWINSIZED.bits.winsize_d2 = stCfg->stSharpU2.u8WinSize;
           SPWINSIZED.bits.winsize_d3 = stCfg->stSharpU3.u8WinSize;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPWINSIZED.u32) + u32LayerId * VID_OFFSET), SPWINSIZED.u32);

           //u16Grd0
           SPGRDD0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDD0.u32) + u32LayerId * VID_OFFSET));
           SPGRDD0.bits.grd0_d1 = stCfg->stSharpU1.u16Grd0;
           SPGRDD0.bits.grd0_d2 = stCfg->stSharpU2.u16Grd0;
           SPGRDD0.bits.grd0_d3 = stCfg->stSharpU3.u16Grd0;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDD0.u32) + u32LayerId * VID_OFFSET), SPGRDD0.u32);

           //u16Grd1
           SPGRDD1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDD1.u32) + u32LayerId * VID_OFFSET));
           SPGRDD1.bits.grd1_d1 = stCfg->stSharpU1.u16Grd1;
           SPGRDD1.bits.grd1_d2 = stCfg->stSharpU2.u16Grd1;
           SPGRDD1.bits.grd1_d3 = stCfg->stSharpU3.u16Grd1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDD1.u32) + u32LayerId * VID_OFFSET), SPGRDD1.u32);

           //u16Grd2
           SPGRDD2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDD2.u32) + u32LayerId * VID_OFFSET));
           SPGRDD2.bits.grd2_d1 = stCfg->stSharpU1.u16Grd2;
           SPGRDD2.bits.grd2_d2 = stCfg->stSharpU2.u16Grd2;
           SPGRDD2.bits.grd2_d3 = stCfg->stSharpU3.u16Grd2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDD2.u32) + u32LayerId * VID_OFFSET), SPGRDD2.u32);

           //u16Grd3
           SPGRDD3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRDD3.u32) + u32LayerId * VID_OFFSET));
           SPGRDD3.bits.grd3_d1 = stCfg->stSharpU1.u16Grd3;
           SPGRDD3.bits.grd3_d2 = stCfg->stSharpU2.u16Grd3;
           SPGRDD3.bits.grd3_d3 = stCfg->stSharpU3.u16Grd3;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRDD3.u32) + u32LayerId * VID_OFFSET), SPGRDD3.u32);

           //bEnAdpGain
           SPGAINEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET));
           SPGAINEN.bits.d1_enable = stCfg->stSharpU1.bEnAdpGain;
           SPGAINEN.bits.d2_enable = stCfg->stSharpU2.bEnAdpGain;
           SPGAINEN.bits.d3_enable = stCfg->stSharpU3.bEnAdpGain;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINEN.u32) + u32LayerId * VID_OFFSET), SPGAINEN.u32);

           //s16Slp1
           SPSLPD1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPD1.u32) + u32LayerId * VID_OFFSET));
           SPSLPD1.bits.slp1_d1 = stCfg->stSharpU1.s16Slp1;
           SPSLPD1.bits.slp1_d2 = stCfg->stSharpU2.s16Slp1;
           SPSLPD1.bits.slp1_d3 = stCfg->stSharpU3.s16Slp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPD1.u32) + u32LayerId * VID_OFFSET), SPSLPD1.u32);

           //s16Slp2
           SPSLPD2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSLPD2.u32) + u32LayerId * VID_OFFSET));
           SPSLPD2.bits.slp2_d1 = stCfg->stSharpU1.s16Slp2;
           SPSLPD2.bits.slp2_d2 = stCfg->stSharpU2.s16Slp2;
           SPSLPD2.bits.slp2_d3 = stCfg->stSharpU3.s16Slp2;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSLPD2.u32) + u32LayerId * VID_OFFSET), SPSLPD2.u32);

           //s16GainMinL
           SPGAINMINLD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMINLD.u32) + u32LayerId * VID_OFFSET));
           SPGAINMINLD.bits.gain_minl_d1 = stCfg->stSharpU1.s16GainMinL;
           SPGAINMINLD.bits.gain_minl_d2 = stCfg->stSharpU2.s16GainMinL;
           SPGAINMINLD.bits.gain_minl_d3 = stCfg->stSharpU3.s16GainMinL;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMINLD.u32) + u32LayerId * VID_OFFSET), SPGAINMINLD.u32);

           //s16GainMax
           SPGAINMAXD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINMAXD.u32) + u32LayerId * VID_OFFSET));
           SPGAINMAXD.bits.gain_max_d1 = stCfg->stSharpU1.s16GainMax;
           SPGAINMAXD.bits.gain_max_d2 = stCfg->stSharpU2.s16GainMax;
           SPGAINMAXD.bits.gain_max_d3 = stCfg->stSharpU3.s16GainMax;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINMAXD.u32) + u32LayerId * VID_OFFSET), SPGAINMAXD.u32);

           //s16GainPos
           SPGAINPOSD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINPOSD.u32) + u32LayerId * VID_OFFSET));
           SPGAINPOSD.bits.gain_pos_d1 = stCfg->stSharpU1.s16GainPos;
           SPGAINPOSD.bits.gain_pos_d2 = stCfg->stSharpU2.s16GainPos;
           SPGAINPOSD.bits.gain_pos_d3 = stCfg->stSharpU3.s16GainPos;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINPOSD.u32) + u32LayerId * VID_OFFSET), SPGAINPOSD.u32);

           //s16GainNeg
           SPGAINNEGD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGAINNEGD.u32) + u32LayerId * VID_OFFSET));
           SPGAINNEGD.bits.gain_neg_d1 = stCfg->stSharpU1.s16GainNeg;
           SPGAINNEGD.bits.gain_neg_d2 = stCfg->stSharpU2.s16GainNeg;
           SPGAINNEGD.bits.gain_neg_d3 = stCfg->stSharpU3.s16GainNeg;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPGAINNEGD.u32) + u32LayerId * VID_OFFSET), SPGAINNEGD.u32);

           //u8OverThrsh
           SPOVERTHDD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPOVERTHDD.u32) + u32LayerId * VID_OFFSET));
           SPOVERTHDD.bits.over_thd_d1 = stCfg->stSharpU1.u8OverThrsh;
           SPOVERTHDD.bits.over_thd_d2 = stCfg->stSharpU2.u8OverThrsh;
           SPOVERTHDD.bits.over_thd_d3 = stCfg->stSharpU3.u8OverThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPOVERTHDD.u32) + u32LayerId * VID_OFFSET), SPOVERTHDD.u32);

           //u8UnderThrsh
           SPUNDERTHDD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPUNDERTHDD.u32) + u32LayerId * VID_OFFSET));
           SPUNDERTHDD.bits.under_thd_d1 = stCfg->stSharpU1.u8UnderThrsh;
           SPUNDERTHDD.bits.under_thd_d2 = stCfg->stSharpU2.u8UnderThrsh;
           SPUNDERTHDD.bits.under_thd_d3 = stCfg->stSharpU3.u8UnderThrsh;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPUNDERTHDD.u32) + u32LayerId * VID_OFFSET), SPUNDERTHDD.u32);

           //s16Tmp0,s16Tmp1,s16Tmp2,s16Tmp3
           SPTMPD1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPD1.u32) + u32LayerId * VID_OFFSET));
           SPTMPD1.bits.tmp0_d1 = stCfg->stSharpU1.s16Tmp0;
           SPTMPD1.bits.tmp1_d1 = stCfg->stSharpU1.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPD1.u32) + u32LayerId * VID_OFFSET), SPTMPD1.u32);

           SPTMPD2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPD2.u32) + u32LayerId * VID_OFFSET));
           SPTMPD2.bits.tmp0_d2 = stCfg->stSharpU2.s16Tmp0;
           SPTMPD2.bits.tmp1_d2 = stCfg->stSharpU2.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPD2.u32) + u32LayerId * VID_OFFSET), SPTMPD2.u32);

           SPTMPD3.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTMPD3.u32) + u32LayerId * VID_OFFSET));
           SPTMPD3.bits.tmp0_d3 = stCfg->stSharpU3.s16Tmp0;
           SPTMPD3.bits.tmp1_d3 = stCfg->stSharpU3.s16Tmp1;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPTMPD3.u32) + u32LayerId * VID_OFFSET), SPTMPD3.u32);

           //bEnAdpShoot
           SPSHOOTEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET));
           SPSHOOTEN.bits.shoot_en_d1 = stCfg->stSharpU1.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_d2 = stCfg->stSharpU2.bEnAdpShoot;
           SPSHOOTEN.bits.shoot_en_d3 = stCfg->stSharpU3.bEnAdpShoot;
           VDP_RegWrite((HI_U32)(&(pVdpReg->SPSHOOTEN.u32) + u32LayerId * VID_OFFSET), SPSHOOTEN.u32);


           break;
       }
       default:
       {
           HI_PRINT("Error,VDP_SetSharpHighFreq() select Wrong direction\n");
           return ;
       }
    }

}
HI_VOID VDP_VID_SetSharpLti(HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir,HI_U32 bEnable, HI_U32 u32Gain)
{
    U_SPENABLE SPENABLE;
    U_SPTIGAIN SPTIGAIN;


    SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
    SPENABLE.bits.lti_en_h = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET), SPENABLE.u32);

    SPTIGAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTIGAIN.u32) + u32LayerId * VID_OFFSET));
    SPTIGAIN.bits.lti_gain_h = u32Gain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPTIGAIN.u32) + u32LayerId * VID_OFFSET), SPTIGAIN.u32);
}

HI_VOID VDP_VID_SetSharpCti(HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir,HI_U32 bEnable, HI_U32 u32Gain)
{
    U_SPENABLE SPENABLE;
    U_SPTIGAIN SPTIGAIN;

    SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
    SPENABLE.bits.cti_en_h = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET), SPENABLE.u32);

    SPTIGAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPTIGAIN.u32) + u32LayerId * VID_OFFSET));
    SPTIGAIN.bits.cti_gain_h = u32Gain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPTIGAIN.u32) + u32LayerId * VID_OFFSET), SPTIGAIN.u32);
}

HI_VOID VDP_VID_SetSharpCoefAddr(HI_U32 u32LayerId, HI_U32 u32CoefAddr)
{
    U_VP0_SHARP_CAD VP0_SHARP_CAD;

    VP0_SHARP_CAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_SHARP_CAD.u32) + u32LayerId * VID_OFFSET));
    VP0_SHARP_CAD.bits.coef_addr = u32CoefAddr;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_SHARP_CAD.u32) + u32LayerId * VID_OFFSET), VP0_SHARP_CAD.u32);
}

//HI_VOID VDP_VID_SetSharpDrEn(HI_U32 u32LayerId, HI_U32 DrEn , HI_U32 MaxdiffEn)
//{
//    U_SPEN SPEN;
//
//    SPEN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPEN.u32) + u32LayerId * VID_OFFSET));
//    SPEN.bits.dr_en      = DrEn;
//    SPEN.bits.maxdiff_en = MaxdiffEn;
//    VDP_RegWrite((HI_U32)(&(pVdpReg->SPEN.u32) + u32LayerId * VID_OFFSET), SPEN.u32);
//}

HI_VOID VDP_VID_SetSharpLtiCor(HI_U32 u32LayerId, HI_U32 u32LtiCor)
{
    U_SPLTICOR SPLTICOR;

    SPLTICOR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPLTICOR.u32) + u32LayerId * VID_OFFSET));
    SPLTICOR.bits.lti_cor = u32LtiCor;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPLTICOR.u32) + u32LayerId * VID_OFFSET), SPLTICOR.u32);
}

HI_VOID VDP_VID_SetSharpCtiA1Th(HI_U32 u32LayerId, HI_U32 u32CtiA1Th0,HI_U32 u32CtiA1Th1)
{
    U_SPCTIA1TH SPCTIA1TH;

    SPCTIA1TH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA1TH.u32) + u32LayerId * VID_OFFSET));
    SPCTIA1TH.bits.cti_a1_th0 = u32CtiA1Th0;
    SPCTIA1TH.bits.cti_a1_th1 = u32CtiA1Th1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA1TH.u32) + u32LayerId * VID_OFFSET), SPCTIA1TH.u32);
}

HI_VOID VDP_VID_SetSharpCtiA2Th(HI_U32 u32LayerId, HI_U32 u32CtiA2Th0,HI_U32 u32CtiA2Th1,HI_U32 u32CtiA2Win)
{
    U_SPCTIA2TH SPCTIA2TH;

    SPCTIA2TH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA2TH.u32) + u32LayerId * VID_OFFSET));
    SPCTIA2TH.bits.cti_a2_th0 = u32CtiA2Th0;
    SPCTIA2TH.bits.cti_a2_th1 = u32CtiA2Th1;
    SPCTIA2TH.bits.cti_a2_win = u32CtiA2Win;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA2TH.u32) + u32LayerId * VID_OFFSET), SPCTIA2TH.u32);
}

HI_VOID VDP_VID_SetSharpCtiA3Th(HI_U32 u32LayerId, HI_U32 u32CtiA3Th0,HI_U32 u32CtiA3Th1)
{
    U_SPCTIA3TH SPCTIA3TH;

    SPCTIA3TH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA3TH.u32) + u32LayerId * VID_OFFSET));
    SPCTIA3TH.bits.cti_a3_th0 = u32CtiA3Th0;
    SPCTIA3TH.bits.cti_a3_th1 = u32CtiA3Th1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA3TH.u32) + u32LayerId * VID_OFFSET), SPCTIA3TH.u32);
}

HI_VOID VDP_VID_SetSharpCtiA4Gain(HI_U32 u32LayerId, HI_U32 u32CtiA4Cor,HI_U32 u32CtiA4Gain)
{
    U_SPCTIA4GAIN SPCTIA4GAIN;

    SPCTIA4GAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA4GAIN.u32) + u32LayerId * VID_OFFSET));
    SPCTIA4GAIN.bits.cti_a4_cor  = u32CtiA4Cor ;
    SPCTIA4GAIN.bits.cti_a4_gain = u32CtiA4Gain;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA4GAIN.u32) + u32LayerId * VID_OFFSET), SPCTIA4GAIN.u32);
}
#if 0
HI_VOID VDP_VID_SetSharpDiffWin(HI_U32 u32LayerId, HI_U32 u32DiffWin)
{
    U_SPDRWIN SPDRWIN;

    SPDRWIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRWIN.u32) + u32LayerId * VID_OFFSET));
    SPDRWIN.bits.diff_win  = u32DiffWin ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRWIN.u32) + u32LayerId * VID_OFFSET), SPDRWIN.u32);
}

HI_VOID VDP_VID_SetSharpDrOvlp(HI_U32 u32LayerId, HI_U32 u32DrHOvlp, HI_U32 u32DrVOvlp, HI_U32 u32DrDOvlp)
{
    U_SPDROVLP SPDROVLP;

    SPDROVLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDROVLP.u32) + u32LayerId * VID_OFFSET));
    SPDROVLP.bits.diff_h_ovlp  = u32DrHOvlp ;
    SPDROVLP.bits.diff_v_ovlp  = u32DrVOvlp ;
    SPDROVLP.bits.diff_d_ovlp  = u32DrDOvlp ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDROVLP.u32) + u32LayerId * VID_OFFSET), SPDROVLP.u32);
}

HI_VOID VDP_VID_SetSharpDrTex(HI_U32 u32LayerId, HI_U32 u32DrTexTh, HI_U32 u32DrTexSlop)
{
    U_SPDRTEX SPDRTEX;

    SPDRTEX.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRTEX.u32) + u32LayerId * VID_OFFSET));
    SPDRTEX.bits.tex_th    = u32DrTexTh   ;
    SPDRTEX.bits.tex_slop  = u32DrTexSlop ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRTEX.u32) + u32LayerId * VID_OFFSET), SPDRTEX.u32);
}

HI_VOID VDP_VID_SetSharpDrGain(HI_U32 u32LayerId, HI_U32 u32DrHDiffGain, HI_U32 u32DrVDiffGain, HI_U32 u32DrDDiffGain)
{
    U_SPDRGAIN SPDRGAIN;

    SPDRGAIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRGAIN.u32) + u32LayerId * VID_OFFSET));
    SPDRGAIN.bits.h_diff_gain  =  u32DrHDiffGain ;
    SPDRGAIN.bits.v_diff_gain  =  u32DrVDiffGain ;
    SPDRGAIN.bits.d_diff_gain  =  u32DrDDiffGain ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRGAIN.u32) + u32LayerId * VID_OFFSET), SPDRGAIN.u32);
}

HI_VOID VDP_VID_SetSharpDrShift(HI_U32 u32LayerId, HI_U32 u32DrDiffShift)
{
    U_SPDRSHIFT SPDRSHIFT;

    SPDRSHIFT.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRSHIFT.u32) + u32LayerId * VID_OFFSET));
    SPDRSHIFT.bits.diff_shift  =  u32DrDiffShift ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRSHIFT.u32) + u32LayerId * VID_OFFSET), SPDRSHIFT.u32);
}

HI_VOID VDP_VID_SetSharpDrBriTh1(HI_U32 u32LayerId, HI_U32 u32DrBriTh0,HI_U32 u32DrBriTh1)
{
    U_SPDRBRITH1 SPDRBRITH1;

    SPDRBRITH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRBRITH1.u32) + u32LayerId * VID_OFFSET));
    SPDRBRITH1.bits.bri_th0 = u32DrBriTh0;
    SPDRBRITH1.bits.bri_th1 = u32DrBriTh1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRBRITH1.u32) + u32LayerId * VID_OFFSET), SPDRBRITH1.u32);
}
HI_VOID VDP_VID_SetSharpDrBriTh2(HI_U32 u32LayerId, HI_U32 u32DrBriTh2,HI_U32 u32DrBriTh3)
{
    U_SPDRBRITH2 SPDRBRITH2;

    SPDRBRITH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRBRITH2.u32) + u32LayerId * VID_OFFSET));
    SPDRBRITH2.bits.bri_th2 = u32DrBriTh2;
    SPDRBRITH2.bits.bri_th3 = u32DrBriTh3;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRBRITH2.u32) + u32LayerId * VID_OFFSET), SPDRBRITH2.u32);
}

HI_VOID VDP_VID_SetSharpDrBriSlp(HI_U32 u32LayerId, HI_U32 u32DrBriSlp0,HI_U32 u32DrBriSlp1,HI_U32 u32DrBriSlp2,HI_U32 u32DrBriSlp3)
{
    U_SPDRBRISLP SPDRBRISLP;

    SPDRBRISLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRBRISLP.u32) + u32LayerId * VID_OFFSET));
    SPDRBRISLP.bits.bri_slp0 = u32DrBriSlp0;
    SPDRBRISLP.bits.bri_slp1 = u32DrBriSlp1;
    SPDRBRISLP.bits.bri_slp2 = u32DrBriSlp2;
    SPDRBRISLP.bits.bri_slp3 = u32DrBriSlp3;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRBRISLP.u32) + u32LayerId * VID_OFFSET), SPDRBRISLP.u32);
}

HI_VOID VDP_VID_SetSharpDrBriRatio0(HI_U32 u32LayerId, HI_U32 u32DrBriRatio0,HI_U32 u32DrBriRatio1,HI_U32 u32DrBriRatio2,HI_U32 u32DrBriRatio3)
{
    U_SPDRBRIRA0 SPDRBRIRA0;

    SPDRBRIRA0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRBRIRA0.u32) + u32LayerId * VID_OFFSET));
    SPDRBRIRA0.bits.bri_ratio0 = u32DrBriRatio0;
    SPDRBRIRA0.bits.bri_ratio1 = u32DrBriRatio1;
    SPDRBRIRA0.bits.bri_ratio2 = u32DrBriRatio2;
    SPDRBRIRA0.bits.bri_ratio3 = u32DrBriRatio3;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRBRIRA0.u32) + u32LayerId * VID_OFFSET), SPDRBRIRA0.u32);
}
HI_VOID VDP_VID_SetSharpDrBriRatio1(HI_U32 u32LayerId, HI_U32 u32DrOriBriRatio ,HI_U32 u32DrMinBriRatio ,HI_U32 u32DrMaxBriRatio)
{
    U_SPDRBRIRA1 SPDRBRIRA1;

    SPDRBRIRA1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPDRBRIRA1.u32) + u32LayerId * VID_OFFSET));
    SPDRBRIRA1.bits.ori_bri_ratio = u32DrOriBriRatio;
    SPDRBRIRA1.bits.min_bri_ratio = u32DrMinBriRatio;
    SPDRBRIRA1.bits.max_bri_ratio = u32DrMaxBriRatio;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPDRBRIRA1.u32) + u32LayerId * VID_OFFSET), SPDRBRIRA1.u32);
}
#endif

HI_VOID VDP_VID_SetSharpCtiA1Slp(HI_U32 u32LayerId, HI_U32 u32CtiA1Slp)
{
    U_SPCTIA1SLP SPCTIA1SLP;

    SPCTIA1SLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA1SLP.u32) + u32LayerId * VID_OFFSET));
    SPCTIA1SLP.bits.cti_a1_slp = u32CtiA1Slp;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA1SLP.u32) + u32LayerId * VID_OFFSET), SPCTIA1SLP.u32);
}
HI_VOID VDP_VID_SetSharpCtiA2Slp(HI_U32 u32LayerId,  HI_U32 u32CtiA2Slp)
{
    U_SPCTIA2SLP SPCTIA2SLP;

    SPCTIA2SLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA2SLP.u32) + u32LayerId * VID_OFFSET));
    SPCTIA2SLP.bits.cti_a2_slp = u32CtiA2Slp;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA2SLP.u32) + u32LayerId * VID_OFFSET), SPCTIA2SLP.u32);
}
HI_VOID VDP_VID_SetSharpCtiA3Slp(HI_U32 u32LayerId, HI_U32 u32CtiA3Slp)
{
    U_SPCTIA3SLP SPCTIA3SLP;

    SPCTIA3SLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA3SLP.u32) + u32LayerId * VID_OFFSET));
    SPCTIA3SLP.bits.cti_a3_slp = u32CtiA3Slp;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA3SLP.u32) + u32LayerId * VID_OFFSET), SPCTIA3SLP.u32);
}
//sharpen add
HI_VOID VDP_VID_SetSharpYValueEn(HI_U32 u32LayerId, HI_U32 u32YValueEn)
{
    U_SPENABLE SPENABLE;

    SPENABLE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET));
    SPENABLE.bits.yvalue_en = u32YValueEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPENABLE.u32) + u32LayerId * VID_OFFSET),SPENABLE.u32);
}
HI_VOID VDP_VID_SetSharpYValueTh(HI_U32 u32LayerId, HI_U32 u32YValueTh0,HI_U32 u32YValueTh1,HI_U32 u32YValueTh2,HI_U32 u32YValueTh3)
{
    U_SPYVALUETH1 SPYVALUETH1;
    U_SPYVALUETH2 SPYVALUETH2;

    SPYVALUETH1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPYVALUETH1.u32) + u32LayerId * VID_OFFSET));
    SPYVALUETH1.bits.yvalue_th0 = u32YValueTh0;
    SPYVALUETH1.bits.yvalue_th1 = u32YValueTh1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPYVALUETH1.u32) + u32LayerId * VID_OFFSET),SPYVALUETH1.u32);

    SPYVALUETH2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPYVALUETH2.u32) + u32LayerId * VID_OFFSET));
    SPYVALUETH2.bits.yvalue_th2 = u32YValueTh2;
    SPYVALUETH2.bits.yvalue_th3 = u32YValueTh3;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPYVALUETH2.u32) + u32LayerId * VID_OFFSET),SPYVALUETH2.u32);
}
HI_VOID VDP_VID_SetSharpYValueSlp(HI_U32 u32LayerId, HI_U32 u32YValueSlp1,HI_U32 u32YValueSlp2)
{
    U_SPYVALUESLP1 SPYVALUESLP1;
    U_SPYVALUESLP2 SPYVALUESLP2;

    SPYVALUESLP1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPYVALUESLP1.u32) + u32LayerId * VID_OFFSET));
    SPYVALUESLP1.bits.yvalue_slp1 = u32YValueSlp1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPYVALUESLP1.u32) + u32LayerId * VID_OFFSET),SPYVALUESLP1.u32);

    SPYVALUESLP2.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPYVALUESLP2.u32) + u32LayerId * VID_OFFSET));
    SPYVALUESLP2.bits.yvalue_slp2 = u32YValueSlp2;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPYVALUESLP2.u32) + u32LayerId * VID_OFFSET),SPYVALUESLP2.u32);

}
HI_VOID VDP_VID_SetSharpCtiA4Th(HI_U32 u32LayerId, HI_U32 u32CtiA4Th0,HI_U32 u32CtiA4Th1)
{
    U_SPCTIA4TH SPCTIA4TH;

    SPCTIA4TH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA4TH.u32) + u32LayerId * VID_OFFSET));
    SPCTIA4TH.bits.cti_a4_th0 = u32CtiA4Th0;
    SPCTIA4TH.bits.cti_a4_th1 = u32CtiA4Th1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA4TH.u32) + u32LayerId * VID_OFFSET),SPCTIA4TH.u32);
}
HI_VOID VDP_VID_SetSharpCtiA4Slp      (HI_U32 u32LayerId, HI_U32 u32CtiA4Slp)
{
    U_SPCTIA4SLP SPCTIA4SLP;

    SPCTIA4SLP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTIA4SLP.u32) + u32LayerId * VID_OFFSET));
    SPCTIA4SLP.bits.cti_a4_slp = u32CtiA4Slp;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTIA4SLP.u32) + u32LayerId * VID_OFFSET),SPCTIA4SLP.u32);

}
HI_VOID VDP_VID_SetSharpFlatSatur     (HI_U32 u32LayerId, HI_U32 u32Satur,HI_U32 u32YFlatNum)
{
    U_SPFLATSATUR SPFLATSATUR;

    SPFLATSATUR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPFLATSATUR.u32) + u32LayerId * VID_OFFSET));
    SPFLATSATUR.bits.saturation = u32Satur ;
    SPFLATSATUR.bits.yflat_num  = u32YFlatNum;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPFLATSATUR.u32) + u32LayerId * VID_OFFSET),SPFLATSATUR.u32);
}
HI_VOID VDP_VID_SetSharpGradSumTh(HI_U32 u32LayerId, HI_U32 u32CGradSumLow,HI_U32 u32CGradSumHigh,HI_U32 u32YGradSumThr)
{
    U_SPGRADSUMTH SPGRADSUMTH;

    SPGRADSUMTH.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPGRADSUMTH.u32) + u32LayerId * VID_OFFSET));
    SPGRADSUMTH.bits.cgrad_sum_low  = u32CGradSumLow;
    SPGRADSUMTH.bits.cgrad_sum_high = u32CGradSumHigh;
    SPGRADSUMTH.bits.ygrad_sum_thr  = u32YGradSumThr ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPGRADSUMTH.u32) + u32LayerId * VID_OFFSET),SPGRADSUMTH.u32);

}
HI_VOID VDP_VID_SetSharpGradThr(HI_U32 u32LayerId, HI_U32 u32CGradThr0,HI_U32 u32CGradThr1,HI_U32 u32CGradDecThr0,HI_U32 u32CGradDecThr1)
{
    U_SPCGRADTHR SPCGRADTHR;

    SPCGRADTHR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCGRADTHR.u32) + u32LayerId * VID_OFFSET));
    SPCGRADTHR.bits.cgrad_thr0  = u32CGradThr0;
    SPCGRADTHR.bits.cgrad_thr1  = u32CGradThr1;
    SPCGRADTHR.bits.cgrad_dec_thr0  = u32CGradDecThr0;
    SPCGRADTHR.bits.cgrad_dec_thr1  = u32CGradDecThr1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCGRADTHR.u32) + u32LayerId * VID_OFFSET),SPCGRADTHR.u32);

}
HI_VOID VDP_VID_SetSharpGradDiffThr(HI_U32 u32LayerId, HI_U32 u32GradDiffThr0,HI_U32 u32GradDiffThr1)
{
    U_SPCGRADDIFFTHR SPCGRADDIFFTHR;

    SPCGRADDIFFTHR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCGRADDIFFTHR.u32) + u32LayerId * VID_OFFSET));
    SPCGRADDIFFTHR.bits.cgrad_diff_thr0  = u32GradDiffThr0;
    SPCGRADDIFFTHR.bits.cgrad_diff_thr1  = u32GradDiffThr1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCGRADDIFFTHR.u32) + u32LayerId * VID_OFFSET),SPCGRADDIFFTHR.u32);
}


//-----------------------------------------------------------------------------------
//SHARPEN_END
//-----------------------------------------------------------------------------------
#endif
HI_VOID VDP_VP_SetDataMask    (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even )
{
    U_VP0_CTRL VP0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error, VDP_VID_SetDataMask() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.odd_data_mute  = u32Odd ;
    VP0_CTRL.bits.even_data_mute = u32Even ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32);

    return ;
}


HI_VOID VDP_VID_SetDcmpCtrl   (HI_U32 u32Data, HI_U32 u32LumLosslessEn, HI_U32 u32ChmLosslessEn )
{
    U_V0_DCMP_CTRL V0_DCMP_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error, VDP_V0_SetDcmpCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMP_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_DCMP_CTRL.bits.is_lossless_l = u32LumLosslessEn ;
    V0_DCMP_CTRL.bits.is_lossless_c = u32ChmLosslessEn ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET), V0_DCMP_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetTileCrop(HI_U32 u32Data,HI_U32 x_offset,HI_U32 l_y_offset,HI_U32 l_c_offset)
{
     U_V0_TILE_LOFFSET V0_TILE_LOFFSET;
     U_V0_TILE_COFFSET V0_TILE_COFFSET;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetTileCrop() Select Wrong Video Layer ID\n");
        return ;
    }
    V0_TILE_LOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_TILE_LOFFSET.u32) + u32Data * VID_OFFSET));
    V0_TILE_LOFFSET.bits.l_x_offset = x_offset;//16BYTE¶ÔÆë
    V0_TILE_LOFFSET.bits.l_y_offset = l_y_offset;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_TILE_LOFFSET.u32) + u32Data * VID_OFFSET), V0_TILE_LOFFSET.u32);
    V0_TILE_COFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_TILE_COFFSET.u32) + u32Data * VID_OFFSET));
    V0_TILE_COFFSET.bits.c_x_offset = x_offset;
    V0_TILE_COFFSET.bits.c_y_offset = l_c_offset;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_TILE_COFFSET.u32) + u32Data * VID_OFFSET), V0_TILE_COFFSET.u32);
    return ;
}



