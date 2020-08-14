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
    #if VID_CROP_EN
    U_V0_CPOS          V0_CPOS;
    #endif
    //for multi region
    U_V0_P0RESO          V0_P0RESO;
    U_V0_P0VFPOS         V0_P0VFPOS;
    U_V0_P0VLPOS         V0_P0VLPOS;

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

HI_VOID  VDP_VID_SetDataWidth    (HI_U32 u32Data, HI_U32 u32DataWidth)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDataWidth() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.data_width = u32DataWidth;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetReadMode(HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode)
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

HI_VOID  VDP_VID_SetInReso(HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_IRESO V0_IRESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32Wth - 1;
    V0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32);


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

HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32Mode)
{
#if VID_DRAW_EN
    U_V0_DRAWMODE V0_DRAWMODE;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetDrawMode() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_DRAWMODE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET));
    V0_DRAWMODE.bits.draw_mode = u32Mode;
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET), V0_DRAWMODE.u32);

    return ;
#endif
}

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


#if 0
HI_VOID VDP_VID_SetRegionEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable )
{
    U_V0_16REGIONENL V0_16REGIONENL;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        HI_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
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

    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);

    return ;
}
#endif

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
    U_V0_UPD V0_UPD;

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
        V0_HSP.bits.non_lnr_en = 0;
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
#endif

HI_VOID VDP_VID_GetZmeHorRatio(HI_U32 u32Data, HI_U32 *u32Ratio)

{
    U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));

    (*u32Ratio) = V0_HSP.bits.hratio;

    return ;
}

#if 0
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
#endif

HI_VOID  VDP_VID_GetZmeVerRatio(HI_U32 u32Data, HI_U32 *u32Ratio)
{
    U_V0_VSR V0_VSR;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET));
    (*u32Ratio) = V0_VSR.bits.vratio;
    return ;
}


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

HI_VOID VDP_VID_SetZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32LAddr, HI_U32 u32CAddr)
{


    U_V0_HLCOEFAD V0_HLCOEFAD;
    U_V0_HCCOEFAD V0_HCCOEFAD;
    U_V0_VLCOEFAD V0_VLCOEFAD;
    U_V0_VCCOEFAD V0_VCCOEFAD;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_VID_PARA_ZME_HOR)
    {
        V0_HLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HLCOEFAD.bits.coef_addr = u32LAddr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET), V0_HLCOEFAD.u32);

        V0_HCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HCCOEFAD.bits.coef_addr = u32CAddr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET), V0_HCCOEFAD.u32);

    }
    else if(u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VLCOEFAD.bits.coef_addr = u32LAddr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET), V0_VLCOEFAD.u32);

        V0_VCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VCCOEFAD.bits.coef_addr = u32CAddr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET), V0_VCCOEFAD.u32);


    }
    else
    {
        HI_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}



HI_VOID  VDP_VID_SetZmeOutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

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

#if 1
// VID.ZME.NONLINE begin
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
#endif

#if 0
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

HI_VOID  VDP_VP_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
#if VP0_EN
    U_VP0_DOF_CTRL  VP0_DOF_CTRL;
    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetThreeDimDofEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_DOF_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DOF_CTRL.u32)));
    VP0_DOF_CTRL.bits.dof_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DOF_CTRL.u32)), VP0_DOF_CTRL.u32);
#endif
}

HI_VOID  VDP_VP_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
#if VP0_EN
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

#endif
    return ;
}

HI_VOID  VDP_VP_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetRegUp  (HI_U32 u32Data)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
#if VP0_EN
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
#endif
}

HI_VOID  VDP_VP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
#if VP0_EN
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
#endif
}

HI_VOID VDP_VP_SetMuteEnable     (HI_U32 u32Data, HI_U32 bEnable)
{
#if VP0_EN
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
#endif
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

#if VP0_CSC_EN
HI_VOID  VDP_VP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef, HI_U32 u32Num)
{
    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        U_VP0_CSC0_IDC  VP0_CSC0_IDC;
        U_VP0_CSC0_ODC  VP0_CSC0_ODC;
        U_VP0_CSC0_IODC VP0_CSC0_IODC;

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
        U_VP0_CSC1_IDC  VP0_CSC1_IDC;
        U_VP0_CSC1_ODC  VP0_CSC1_ODC;
        U_VP0_CSC1_IODC VP0_CSC1_IODC;

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
        U_VP0_CSC0_P0 VP0_CSC0_P0;
        U_VP0_CSC0_P1 VP0_CSC0_P1;
        U_VP0_CSC0_P2 VP0_CSC0_P2;
        U_VP0_CSC0_P3 VP0_CSC0_P3;
        U_VP0_CSC0_P4 VP0_CSC0_P4;

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
        U_VP0_CSC1_P0 VP0_CSC1_P0;
        U_VP0_CSC1_P1 VP0_CSC1_P1;
        U_VP0_CSC1_P2 VP0_CSC1_P2;
        U_VP0_CSC1_P3 VP0_CSC1_P3;
        U_VP0_CSC1_P4 VP0_CSC1_P4;

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
        U_VP0_CSC0_IDC VP0_CSC0_IDC;

        VP0_CSC0_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32);
    }
    else if(u32Num == 1)
    {
        U_VP0_CSC1_IDC VP0_CSC1_IDC;

        VP0_CSC1_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32);
    }

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

HI_VOID VDP_VP_SetVpSel     (HI_U32 u32Data,HI_U32 vp_sel)
{
    U_VP0_CTRL VP0_CTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetMuteEnable() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.vp_sel = vp_sel;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32);

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
    VP0_ACM_CTRL.bits.acm_en = u32bAcmEn;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32);

    return ;

}


HI_VOID VDP_VP_SetDciEnable(HI_U32 u32Data, HI_U32 u32DciEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetDciEnable() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_en = u32DciEn  ;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

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

HI_VOID VDP_VID_SetSharpEnable(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET));
    SPCTRL.bits.sharpen_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->SPCTRL.u32) + u32LayerId * VID_OFFSET), SPCTRL.u32);
}

HI_VOID VDP_VID_SetTwodt3dEn(HI_U32 u32Data, HI_U32 u32twodt3d_en)
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

