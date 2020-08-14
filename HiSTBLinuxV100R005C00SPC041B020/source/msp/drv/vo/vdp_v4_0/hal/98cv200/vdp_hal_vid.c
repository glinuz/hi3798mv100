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


#include "vdp_hal_comm.h"

#ifndef __DISP_PLATFORM_BOOT__
#include "vdp_hal_vid.h"
#endif

extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID  VDP_VP_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;
    U_VP0_DFPOS VP0_DFPOS;
    U_VP0_DLPOS VP0_DLPOS;
    U_VP0_IRESO VP0_IRESO;
	
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    VP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VID_OFFSET));
    VP0_VFPOS.bits.video_xfpos = stRect.u32VX;
    VP0_VFPOS.bits.video_yfpos = stRect.u32VY;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VID_OFFSET), VP0_VFPOS.u32); 

    VP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VID_OFFSET));
    VP0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    VP0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VID_OFFSET), VP0_VLPOS.u32); 

    VP0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32) + u32Data * VID_OFFSET));
    VP0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    VP0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32) + u32Data * VID_OFFSET), VP0_DFPOS.u32); 

    VP0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32) + u32Data * VID_OFFSET));
    VP0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
    VP0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32) + u32Data * VID_OFFSET), VP0_DLPOS.u32); 

    VP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32IWth - 1;
    VP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32); 

    return ;
} 

HI_VOID  VDP_VP_SetRegUp  (HI_U32 u32Data)
{
    U_VP0_UPD VP0_UPD;

    /* VP layer register update */
    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetRegUp() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_UPD.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_UPD.u32) + u32Data * VP_OFFSET), VP0_UPD.u32); 

    return ;
}



HI_VOID  VDP_VID_SetPreReadEnable    (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_PRERD     V0_PRERD;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetPreReadEnable() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.pre_rd_en = u32Enable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);

    return ;
}
#ifndef __DISP_PLATFORM_BOOT__

HI_VOID  VDP_VID_SetReqLength    (HI_U32 u32Data, HI_U32 ReqLenght )
{
    U_V0_PRERD     V0_PRERD;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetReqLength() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET));
    V0_PRERD.bits.req_len = ReqLenght ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET), V0_PRERD.u32);

    return ;
}

HI_VOID  VDP_VID_SetUvorder(HI_U32 u32Data, HI_U32 u32bUvorder)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetUvorder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.uv_order= u32bUvorder;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}

HI_VOID  VDP_VID_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_CTRL  V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetNoSecFlag() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.nosec_flag= u32Enable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetLayerEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;
    U_V0_16REGIONENL V0_16REGIONENL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.surface_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    V0_16REGIONENL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));
    V0_16REGIONENL.u32 = 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);

    return ;
}

HI_VOID  VDP_VID_SetFiLayerEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetFiLayerEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.surface_c_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetFiVscale    (HI_U32 u32Data, HI_U32 u32bFiVscale )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error, VDP_VID_SetFiVscale() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.fi_vscale = u32bFiVscale ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetTileStride (HI_U32 u32Data, HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_V0_TILE_STRIDE V0_TILE_STRIDE;

    if(u32Data != VDP_LAYER_VID0)
    {
        VDP_PRINT("Error, VDP_VID_SetTileStride() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_TILE_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TILE_STRIDE.u32) + u32Data * VID_OFFSET));
    V0_TILE_STRIDE.bits.tile_stride = u32LStr;
    V0_TILE_STRIDE.bits.tile_cstride = u32CStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TILE_STRIDE.u32) + u32Data * VID_OFFSET), V0_TILE_STRIDE.u32);

    return ;
}

HI_VOID  VDP_VID_SetLayerAddr   (HI_U32 u32Data, HI_U32 u32Chan, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_V0_P0STRIDE V0_P0STRIDE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Chan == 0)
    {
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET), u32LAddr); 
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET), u32CAddr); 
    }
    else if(u32Chan == 1)
    {
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_NADDR.u32) + u32Data * VID_OFFSET), u32LAddr); 
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_NCADDR.u32) + u32Data * VID_OFFSET), u32CAddr); 
    }
    else
    {
        VDP_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Addr ID\n");
    }

    V0_P0STRIDE.bits.surface_stride = u32LStr;
    V0_P0STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET), V0_P0STRIDE.u32); 

    return ;
}

HI_VOID  VDP_VID_SetHeadAddr   (HI_U32 u32Data, HI_U32 u32LHeadAddr,HI_U32 u32CHeadAddr)
{
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetHeadAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_LHEAD_ADDR.u32) + u32Data * VID_OFFSET), u32LHeadAddr); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CHEAD_ADDR.u32) + u32Data * VID_OFFSET), u32CHeadAddr); 

    return ;
}

HI_VOID  VDP_VID_SetHeadSize   (HI_U32 u32Data, HI_U32 u32HeadSize)
{
    U_V0_HEAD_SIZE V0_HEAD_SIZE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetHeadSize() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HEAD_SIZE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HEAD_SIZE.u32) + u32Data * VID_OFFSET));
    V0_HEAD_SIZE.bits.head_size = u32HeadSize;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HEAD_SIZE.u32) + u32Data * VID_OFFSET), V0_HEAD_SIZE.u32); 

    return ;
}

HI_VOID  VDP_VID_SetHeadStride  (HI_U32 u32Data, HI_U32 u32HeadStride)
{
    U_V0_HEAD_STRIDE       V0_HEAD_STRIDE                    ; /* 0xf38 */

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetHeadStride() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HEAD_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HEAD_STRIDE.u32) + u32Data * VID_OFFSET));
    V0_HEAD_STRIDE.bits.head_stride = u32HeadStride;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HEAD_STRIDE.u32) + u32Data * VID_OFFSET), V0_HEAD_STRIDE.u32); 

    return ;
}

//HI_VOID  VDP_VID_SetLayerReso     (HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
//{
//    U_V0_VFPOS V0_VFPOS;
//    U_V0_VLPOS V0_VLPOS;
//    U_V0_DFPOS V0_DFPOS;
//    U_V0_DLPOS V0_DLPOS;
//    U_V0_IRESO V0_IRESO;
//    U_V0_ORESO V0_ORESO;
//
//    //for multi region
//    U_V0_P0RESO          V0_P0RESO;
//    U_V0_P0VFPOS         V0_P0VFPOS;
//    U_V0_P0VLPOS         V0_P0VLPOS;
//#if VID_CROP_EN
//    U_V0_CPOS          V0_CPOS;
//#endif
//
//    if(u32Data >= VID_MAX)
//    {
//        VDP_PRINT("Error,VDP_VID_SetLayerReso() Select Wrong Video Layer ID\n");
//        return ;
//    }
//
//    /*video position */ 
//    V0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
//    V0_VFPOS.bits.video_xfpos = stRect.u32VX;
//    V0_VFPOS.bits.video_yfpos = stRect.u32VY;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET), V0_VFPOS.u32); 
//
//    V0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
//    V0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
//    V0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET), V0_VLPOS.u32); 
//
//    V0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
//    V0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
//    V0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET), V0_DFPOS.u32); 
//
//    V0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
//    V0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
//    V0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET), V0_DLPOS.u32); 
//
//    V0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
//    V0_IRESO.bits.iw = stRect.u32IWth - 1;
//    V0_IRESO.bits.ih = stRect.u32IHgt - 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32); 
//
//    V0_ORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
//    V0_ORESO.bits.ow = stRect.u32OWth - 1;
//    V0_ORESO.bits.oh = stRect.u32OHgt - 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32); 
//#if VID_CROP_EN
//
//    V0_CPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
//    V0_CPOS.bits.src_xfpos = 0;
//    V0_CPOS.bits.src_xlpos = stRect.u32IWth - 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32); 
//#endif
//
//    /*video position */ 
//    V0_P0VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET));
//    V0_P0VFPOS.bits.video_xfpos = stRect.u32VX;
//    V0_P0VFPOS.bits.video_yfpos = stRect.u32VY;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET), V0_P0VFPOS.u32); 
//
//    V0_P0VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET));
//    V0_P0VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
//    V0_P0VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET), V0_P0VLPOS.u32); 
//
//    V0_P0RESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET));
//    V0_P0RESO.bits.w = stRect.u32OWth- 1;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET), V0_P0RESO.u32); 
//    return ;
//}   

HI_S32  VDP_VID_GetInDataFmt       (HI_U32 u32Data)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetInDataFmt() Select Wrong Video Layer ID\n");
        return  HI_FAILURE;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));



    return (V0_CTRL.u32 & 0xf);
}

HI_VOID  VDP_VID_SetInDataFmt       (HI_U32 u32Data, VDP_VID_IFMT_E  enDataFmt)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetInDataFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));

#ifdef VDP_HI3798CV200
    if(u32Data == VDP_LAYER_VID3)
        V0_CTRL.bits.ifmt = VDP_VID_IFMT_SP_422;
    else
#endif
        V0_CTRL.bits.ifmt = enDataFmt;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VID_SetDataWidth    (HI_U32 u32Data, HI_U32 DataWidth)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDataWidth() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.data_width = DataWidth;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VID_SetReadMode    (HI_U32 u32Data, VDP_DATA_RMODE_E enLRMode,VDP_DATA_RMODE_E enCRMode)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetReadMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.lm_rmode = enLRMode;
    V0_CTRL.bits.chm_rmode = enCRMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VID_SetMuteEnable   (HI_U32 u32Data, HI_U32 bEnable)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_LayerMuteEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VID_SetFlipEnable(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetFlipEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET));
    V0_CTRL.bits.flip_en = u32bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32)+ u32Data * VID_OFFSET), V0_CTRL.u32); 
}

HI_VOID  VDP_VID_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_IRESO V0_IRESO;

#if VID_CROP_EN
    U_V0_CPOS  V0_CPOS;
#endif

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
    V0_IRESO.bits.iw = stRect.u32Wth - 1;
    V0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET), V0_IRESO.u32); 

#if VID_CROP_EN
    V0_CPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = 0;
    V0_CPOS.bits.src_xlpos = stRect.u32Wth - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32); 
#endif

    return ;
}

HI_VOID  VDP_VID_SetOutReso     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_ORESO V0_ORESO;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_ORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));
    V0_ORESO.bits.ow = stRect.u32Wth - 1;
    V0_ORESO.bits.oh = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET), V0_ORESO.u32); 

    return ;
}   

HI_VOID  VDP_VID_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_VFPOS V0_VFPOS;
    U_V0_VLPOS V0_VLPOS;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetVideoPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    V0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
    V0_VFPOS.bits.video_xfpos = stRect.u32X;
    V0_VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET), V0_VFPOS.u32); 

    V0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
    V0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    V0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET), V0_VLPOS.u32); 
    return ;
}   

HI_VOID  VDP_VID_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_V0_DFPOS V0_DFPOS;
    U_V0_DLPOS V0_DLPOS;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDispPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    V0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
    V0_DFPOS.bits.disp_xfpos = stRect.u32X;
    V0_DFPOS.bits.disp_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET), V0_DFPOS.u32); 

    V0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
    V0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
    V0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET), V0_DLPOS.u32); 
    return ;
}   

HI_VOID VDP_VID_SetSrOutReso (HI_U32 u32Data, VDP_RECT_S stRect)
{
	U_V0_SRORESO  V0_SRORESO;

	if(u32Data >= CHN_MAX)
	{
		VDP_PRINT("Error,VDP_VID_SetSrOutReso() Select Wrong Video Layer ID\n");
		return ;
	}

	V0_SRORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SRORESO.u32) + u32Data * VID_OFFSET));
	V0_SRORESO.bits.ow = stRect.u32Wth - 1;
	V0_SRORESO.bits.oh = stRect.u32Hgt - 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_SRORESO.u32) + u32Data * VID_OFFSET), V0_SRORESO.u32); 
}

//#if DCMP_EN
HI_VOID  VDP_VID_SetDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
#ifdef VDP_HI3798CV200
    V0_CTRL.bits.dcmp_en = 0 ;
#else
    V0_CTRL.bits.dcmp_en = u32bEnable ;
#endif
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}

HI_VOID  VDP_VID_SetTileDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTileDcmpEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.dcmp_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);


    return ;
}
//del
#if 0
HI_VOID  VDP_VID_SetDcmpOffset    (HI_U32 u32Data,HI_U32 u32laddr_offset,HI_U32  u32caddr_offset )
{
    U_V0_LADDROFFSET V0_LADDROFFSET;
    U_V0_CADDROFFSET V0_CADDROFFSET;


    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpOffset() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_LADDROFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_LADDROFFSET.u32) + u32Data * VID_OFFSET));
    V0_LADDROFFSET.u32 = u32laddr_offset ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_LADDROFFSET.u32) + u32Data * VID_OFFSET), V0_LADDROFFSET.u32);

    V0_CADDROFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CADDROFFSET.u32) + u32Data * VID_OFFSET));
    V0_CADDROFFSET.u32 = u32caddr_offset ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CADDROFFSET.u32) + u32Data * VID_OFFSET), V0_CADDROFFSET.u32);


    return ;
}
#endif
HI_VOID  VDP_VID_SetDcmpErrorClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
	U_V0_DCMPERRCLR  V0_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpErrorClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.dcmp_l_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.dcmp_c_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);


    return ;
}

HI_VOID  VDP_VID_SetDcmpHeadErrClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
	U_V0_DCMPERRCLR  V0_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpHeadErrClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.l_head_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.c_head_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);


    return ;
}

HI_VOID  VDP_VID_SetDcmpConsumeErrClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
	U_V0_DCMPERRCLR  V0_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpConsumeErrClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.l_consume_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.c_consume_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);


    return ;
}

HI_VOID  VDP_VID_SetDcmpForgiveErrClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
	U_V0_DCMPERRCLR  V0_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpForgiveErrClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.l_forgive_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);

    V0_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_DCMPERRCLR.bits.c_forgive_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_DCMPERRCLR.u32);


    return ;
}
//#endif

HI_VOID VDP_VID_SetIfirMode(HI_U32 u32Data, VDP_IFIRMODE_E enMode)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_SetIfirMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.ifir_mode = enMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

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
        VDP_PRINT("Error,VDP_VID_SetIfirCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_IFIRCOEF01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF01.u32) + u32Data * VID_OFFSET));
    V0_IFIRCOEF23.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF23.u32) + u32Data * VID_OFFSET));
    V0_IFIRCOEF45.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF45.u32) + u32Data * VID_OFFSET));
    V0_IFIRCOEF67.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF67.u32) + u32Data * VID_OFFSET));

    V0_IFIRCOEF01.bits.coef0 = s32Coef[0];
    V0_IFIRCOEF01.bits.coef1 = s32Coef[1];
    V0_IFIRCOEF23.bits.coef2 = s32Coef[2];
    V0_IFIRCOEF23.bits.coef3 = s32Coef[3];
    V0_IFIRCOEF45.bits.coef4 = s32Coef[4];
    V0_IFIRCOEF45.bits.coef5 = s32Coef[5];
    V0_IFIRCOEF67.bits.coef6 = s32Coef[6];
    V0_IFIRCOEF67.bits.coef7 = s32Coef[7];

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF01.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF01.u32); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF23.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF23.u32); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF45.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF45.u32); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_IFIRCOEF67.u32) + u32Data * VID_OFFSET), V0_IFIRCOEF67.u32); 

    return ;
}

HI_VOID  VDP_VID_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    U_V0_CBMPARA V0_CBMPARA;
    U_V0_ALPHA V0_ALPHA;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error, VDP_VID_SetLayerGalpha() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CBMPARA.u32) + u32Data * VID_OFFSET));
    V0_CBMPARA.bits.galpha = u32Alpha0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CBMPARA.u32) + u32Data * VID_OFFSET), V0_CBMPARA.u32); 

    V0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET));
    V0_ALPHA.bits.vbk_alpha = u32Alpha0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET), V0_ALPHA.u32); 
    return ;
}

HI_VOID  VDP_VID_SetCropReso    (HI_U32 u32Data, VDP_RECT_S stRect)
{
#if VID_CROP_EN
    U_V0_CPOS V0_CPOS;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetCropReso() Select Wrong Video Layer ID\n");
        return ;
    }

    /* crop position */
    V0_CPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));
    V0_CPOS.bits.src_xfpos = stRect.u32X;
    V0_CPOS.bits.src_xlpos = stRect.u32X+stRect.u32Wth-1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET), V0_CPOS.u32); 

    return ;
#endif
}

HI_VOID  VDP_VID_SetLayerBkg    (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_V0_BK    V0_BK;
    U_V0_ALPHA V0_ALPHA;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerBkg() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_BK.u32) + u32Data * VID_OFFSET));
    V0_BK.bits.vbk_y  = stBkg.u32BkgY;
    V0_BK.bits.vbk_cb = stBkg.u32BkgU;
    V0_BK.bits.vbk_cr = stBkg.u32BkgV;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_BK.u32) + u32Data * VID_OFFSET), V0_BK.u32); 

    V0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET));
    V0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ALPHA.u32) + u32Data * VID_OFFSET), V0_ALPHA.u32); 

    return;
}

HI_VOID  VDP_VID_SetMuteBkg    (HI_U32 u32Data, VDP_BKG_S stMuteBkg)
{
    U_V0_MUTE_BK    V0_MUTE_BK;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetMuteBkg() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_MUTE_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MUTE_BK.u32) + u32Data * VID_OFFSET));
    V0_MUTE_BK.bits.mute_y  = stMuteBkg.u32BkgY;
    V0_MUTE_BK.bits.mute_cb = stMuteBkg.u32BkgU;
    V0_MUTE_BK.bits.mute_cr = stMuteBkg.u32BkgV;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MUTE_BK.u32) + u32Data * VID_OFFSET), V0_MUTE_BK.u32); 

    return ;
}

HI_VOID  VDP_SetParaUpMode(HI_U32 u32Data,HI_U32 u32Mode)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_SetParaUpMode() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.vup_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VID_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    U_V0_CSC_IDC  V0_CSC_IDC;
    U_V0_CSC_ODC  V0_CSC_ODC;
    U_V0_CSC_IODC V0_CSC_IODC;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
    V0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
    V0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), V0_CSC_IDC.u32); 

    V0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET));
    V0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
    V0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET), V0_CSC_ODC.u32); 

    V0_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET));
    V0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
    V0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET), V0_CSC_IODC.u32); 

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
        VDP_PRINT("Error,VDP_VID_SetCscCoef Select Wrong video ID\n");
        return ;
    }


    V0_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_P0.u32)+u32Data*VID_OFFSET));
    V0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    V0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_P0.u32)+u32Data*VID_OFFSET), V0_CSC_P0.u32);

    V0_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_P1.u32)+u32Data*VID_OFFSET));
    V0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    V0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_P1.u32)+u32Data*VID_OFFSET), V0_CSC_P1.u32);

    V0_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_P2.u32)+u32Data*VID_OFFSET));
    V0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    V0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_P2.u32)+u32Data*VID_OFFSET), V0_CSC_P2.u32);

    V0_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_P3.u32)+u32Data*VID_OFFSET));
    V0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    V0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_P3.u32)+u32Data*VID_OFFSET), V0_CSC_P3.u32);

    V0_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_P4.u32)+u32Data*VID_OFFSET));
    V0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_P4.u32)+u32Data*VID_OFFSET), V0_CSC_P4.u32);

}

HI_VOID  VDP_VID_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    U_V0_CSC_IDC V0_CSC_IDC;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
    V0_CSC_IDC.bits.csc_en = u32bCscEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), V0_CSC_IDC.u32); 

    return ;
}

HI_VOID VDP_VID_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)

{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;

    HI_S32 u32Pre   = 1 << 10;
    HI_S32 u32DcPre = 4;//1:8bit; 4:10bit

    //if(u32Data == VDP_LAYER_VID3)  //for vid3 csc coef fixed !!!!
    //{
    //    U_V0_CSC_IDC  V0_CSC_IDC;

    //    V0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET ));        
    //    V0_CSC_IDC.bits.csc_mode = enCscMode;
    //    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET ), V0_CSC_IDC.u32);
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
    //    VDP_PRINT("Error,VDP_VID_SetDispMode() Select Wrong Video Layer ID\n");
    //    return ;
    //}

    //
    //V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    //V0_CTRL.bits.disp_mode = enDispMode;
    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}

HI_VOID VDP_VID_SetDrawMode (HI_U32 u32Data, HI_U32 u32ModeLuma, HI_U32 u32ModeChroma)
{
#if VID_DRAW_EN
    U_V0_DRAWMODE V0_DRAWMODE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDrawMode() Select Wrong Video Layer ID\n");
        return ;
    }


    V0_DRAWMODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET));
    V0_DRAWMODE.bits.draw_mode   = u32ModeLuma;
    V0_DRAWMODE.bits.draw_mode_c = u32ModeChroma;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET), V0_DRAWMODE.u32); 

    return ;
#endif
}

#if 0 //delte in s5v2
HI_VOID VDP_SetTimeOut(HI_U32 u32Data, HI_U32 u32TData)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetTimeOut() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.time_out = u32TData;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32); 

    return ;
}
#endif

/*move to hal common*/
#if 0
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
#endif
HI_VOID VDP_VID_SetMultiModeEnable(HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_MULTI_MODE      V0_MULTI_MODE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetMultiModeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_MULTI_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MULTI_MODE.u32) + u32Data * VID_OFFSET));
    V0_MULTI_MODE.bits.mrg_mode = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_MULTI_MODE.u32) + u32Data * VID_OFFSET), V0_MULTI_MODE.u32);

    return ;
}

HI_VOID VDP_VID_SetRegionEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable )
{
    U_V0_16REGIONENL V0_16REGIONENL;
    U_V0_P0CTRL      V0_P0CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionEnable() Select Wrong region ID\n");
        return ;
    }

    V0_16REGIONENL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));
    if(u32bEnable)
    {
        V0_16REGIONENL.u32 = (1 << u32Num) | V0_16REGIONENL.u32;
    }
    else
    {
        V0_16REGIONENL.u32 = (~(1 << u32Num)) & V0_16REGIONENL.u32;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);


    V0_P0CTRL.u32   =   VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0CTRL.bits.region_en    =   u32bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetAllRegionDisable(HI_U32 u32Data )
{
    HI_U32           u32Num;
    U_V0_P0CTRL      V0_P0CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetAllRegionDisable() Select Wrong Video Layer ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), 0); 

    for(u32Num=0; u32Num<16; u32Num++)
    {
        V0_P0CTRL.u32   =   VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
        V0_P0CTRL.bits.region_en    =   HI_FALSE;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0CTRL.u32);
    }

    return ;
}

HI_BOOL VDP_VID_CheckRegionState(HI_U32 u32Data)
{
	HI_U32           u32Num = 0, u32RegValue = 0;
    U_V0_P0CTRL      V0_P0CTRL;
	
    for(u32Num = 0; u32Num < 16; u32Num++)
    {
        V0_P0CTRL.u32   =   VDP_RegRead((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET);
		u32RegValue     |= V0_P0CTRL.bits.region_en;
    }

	if (u32RegValue)
	{
		return HI_TRUE;
	}
	else
	{
		return HI_FALSE;
	}
		
}

HI_VOID VDP_VID_SetRegionAddr(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_V0_P0STRIDE V0_P0STRIDE;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionAddr() Select Wrong region ID\n");
        return ;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), u32LAddr); 
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), u32CAddr); 

    V0_P0STRIDE.bits.surface_stride = u32LStr;
    V0_P0STRIDE.bits.surface_cstride = u32CStr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0STRIDE.u32); 

    return ;
}

HI_VOID VDP_VID_SetRegionReso(HI_U32 u32Data, HI_U32 u32Num, VDP_RECT_S  stRect)
{
    U_V0_P0RESO          V0_P0RESO;
    U_V0_P0VFPOS         V0_P0VFPOS;
    U_V0_P0VLPOS         V0_P0VLPOS;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionReso() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionReso() Select Wrong region ID\n");
        return ;
    }

    /*video position */ 
    V0_P0VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0VFPOS.bits.video_xfpos = stRect.u32X;
    V0_P0VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VFPOS.u32); 

    V0_P0VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    V0_P0VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0VLPOS.u32); 

    V0_P0RESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0RESO.bits.w = stRect.u32Wth - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0RESO.u32); 

    return ;
}

HI_VOID VDP_VID_SetRegionMuteEnable(HI_U32 u32Data, HI_U32 u32Num, HI_U32 bEnable)
{
    U_V0_16MUTE V0_16MUTE;
    U_V0_P0CTRL V0_P0CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionMuteEnable() Select Wrong VID Layer ID\n");
        return ;
    }

    if(u32Num >= VID_REGION_NUM)
    {
        VDP_PRINT("Error,VDP_VID_SetRegionMuteEnable() Select Wrong region ID\n");
        return ;
    }

    V0_16MUTE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16MUTE.u32) + u32Data * VID_OFFSET));
    if(bEnable)
    {
        V0_16MUTE.u32 = (1 << u32Num) | V0_16MUTE.u32;
    }
    else
    {
        V0_16MUTE.u32 = (~(1 << u32Num)) & V0_16MUTE.u32;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_16MUTE.u32) + u32Data * VID_OFFSET), V0_16MUTE.u32); 

    V0_P0CTRL.u32               =   VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
    V0_P0CTRL.bits.mute_en      =   bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET), V0_P0CTRL.u32);

    return ;
}

HI_VOID  VDP_VID_SetRegUp       (HI_U32 u32Data)
{
    U_V0_UPD V0_UPD;

    /* VHD layer register update */
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetRegup() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_UPD.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_UPD.u32) + u32Data * VID_OFFSET), V0_UPD.u32); 

    return ;
}

///////////////////////////////////
//ZME BEGIN
///////////////////////////////////
HI_VOID  VDP_VID_SetZmeEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hlmsc_en = u32bEnable;
        V0_HSP.bits.hlfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hchmsc_en = u32bEnable;
        V0_HSP.bits.hchfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
    }
#if 1
    if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
    {
		V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32)));
        V0_HSP.bits.non_lnr_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32)), V0_HSP.u32);
    }
#endif

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vlmsc_en = u32bEnable;
        V0_VSP.bits.vlfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vchmsc_en = u32bEnable;
        V0_VSP.bits.vchfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
    }

#ifdef VDP_HI3798CV200
    if(u32Data == VDP_LAYER_VID3)
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hlmsc_en = 0;
        V0_HSP.bits.hlfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 

        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hchmsc_en = 0;
        V0_HSP.bits.hchfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 

        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vlmsc_en = 0;
        V0_VSP.bits.vlfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vchmsc_en = 0;
        V0_VSP.bits.vchfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
    }
#endif

    return;
}

HI_VOID  VDP_VID_SetVtapReduce   (HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_VSP V0_VSP;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetVtapReduce() Select Wrong Video Layer ID\n");
        return ;
    }
    
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vtap_reduce = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

    return ;
}    

HI_VOID  VDP_VID_SetZmeShootCtrlEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable,HI_U32 u32bMode)
{
    U_V0_ZME_SHOOTCTRL_HL V0_ZME_SHOOTCTRL_HL;
    U_V0_ZME_SHOOTCTRL_HC V0_ZME_SHOOTCTRL_HC;
    U_V0_ZME_SHOOTCTRL_VL V0_ZME_SHOOTCTRL_VL;
    U_V0_ZME_SHOOTCTRL_VC V0_ZME_SHOOTCTRL_VC;

    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data != VDP_LAYER_VID0)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlEnable() Select Wrong Video Layer ID\n");
        return;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_HL.bits.hl_shootctrl_en   = u32bEnable;
        V0_ZME_SHOOTCTRL_HL.bits.hl_shootctrl_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_HL.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_en   = u32bEnable;
//         V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_mode = u32bMode;
        V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_mode = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_HC.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_VL.bits.vl_shootctrl_en   = u32bEnable;
        V0_ZME_SHOOTCTRL_VL.bits.vl_shootctrl_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_VL.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_en   = u32bEnable;
//         V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_mode = u32bMode;
        V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_mode = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_VC.u32);      
    }

    return ;
}


HI_VOID  VDP_VID_SetZmeShootCtrlPara  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32Flatdect_mode,HI_U32 u32Coringadj_en,HI_U32 u32Gain,HI_U32 u32Coring)
{
    U_V0_ZME_SHOOTCTRL_HL V0_ZME_SHOOTCTRL_HL;
    U_V0_ZME_SHOOTCTRL_HC V0_ZME_SHOOTCTRL_HC;
    U_V0_ZME_SHOOTCTRL_VL V0_ZME_SHOOTCTRL_VL;
    U_V0_ZME_SHOOTCTRL_VC V0_ZME_SHOOTCTRL_VC;   

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data != VDP_LAYER_VID0)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlPara() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_HL.bits.hl_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_HL.bits.hl_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_HL.bits.hl_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_HL.bits.hl_coring         = u32Coring;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_HL.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_HC.bits.hc_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_HC.bits.hc_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_HC.bits.hc_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_HC.bits.hc_coring         = u32Coring;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_HC.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_VL.bits.vl_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_VL.bits.vl_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_VL.bits.vl_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_VL.bits.vl_coring         = u32Coring;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_VL.u32);     
    }
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + u32Data * VID_OFFSET));
        V0_ZME_SHOOTCTRL_VC.bits.vc_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_VC.bits.vc_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_VC.bits.vc_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_VC.bits.vc_coring         = u32Coring;          
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + u32Data * VID_OFFSET), V0_ZME_SHOOTCTRL_VC.u32);      
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
        VDP_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HLOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET));
        V0_HLOFFSET.bits.hor_loffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HLOFFSET.u32) + u32Data * VID_OFFSET), V0_HLOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HCOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET));
        V0_HCOFFSET.bits.hor_coffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOFFSET.u32) + u32Data * VID_OFFSET), V0_HCOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VOFFSET.bits.vluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VOFFSET.bits.vchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + u32Data * VID_OFFSET), V0_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERBL) ||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VBOFFSET.bits.vbluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERBC) ||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET));
        V0_VBOFFSET.bits.vbchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + u32Data * VID_OFFSET), V0_VBOFFSET.u32); 
    }

    return ;
}

HI_VOID  VDP_VID_SetHdrElZme(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_V0_VSP V0_VSP;

    if(u32Data != VDP_LAYER_VID1)
    {
        VDP_PRINT("Error,VDP_VID_SetHdrElZme() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.hdr_el_zme = u32bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

    return ;
}




HI_VOID  VDP_VID_SetZmeFirEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hlfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
        V0_HSP.bits.hchfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vlfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
        V0_VSP.bits.vchfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
    }

    return ;
}

HI_VOID  VDP_VID_SetZmeMidEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data == VDP_LAYER_VID0)
    {
        /* VHD layer zoom enable */
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
//             V0_HSP.bits.hlmid_en = u32bEnable;
            V0_HSP.bits.hlmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
//             V0_HSP.bits.hchmid_en = u32bEnable;
            V0_HSP.bits.hchmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
//             V0_VSP.bits.vlmid_en = u32bEnable;
            V0_VSP.bits.vlmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
//             V0_VSP.bits.vchmid_en = u32bEnable;
            V0_VSP.bits.vchmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
        }
    }
    else
    {
        /* VHD layer zoom enable */
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
            V0_HSP.bits.hlmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
            V0_HSP.bits.hchmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
            V0_VSP.bits.vlmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
            V0_VSP.bits.vchmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 
        }
    }

    return ;
}

HI_VOID VDP_VID_SetZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 

    return ;
}

HI_VOID  VDP_VID_SetZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_VSR V0_VSR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET));
    V0_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET), V0_VSR.u32); 

    return ;
}

HI_VOID  VDP_VID_SetVfirOneTapEnable(HI_U32 u32Data, HI_U32 u32VfirOneTapEn)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetVfirOneTapEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.vfir_1tap_en = u32VfirOneTapEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

    return ;
}

HI_VOID VDP_VID_SetZmeHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
#ifdef VDP_HI3798CV200
    V0_HSP.bits.hfir_order = 1;     //u32HfirOrder;
#else
    V0_HSP.bits.hfir_order = u32HfirOrder;
#endif
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET), V0_HSP.u32); 

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
        VDP_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_VID_PARA_ZME_HOR)
    {
        V0_HLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HLCOEFAD.u32) + u32Data * VID_OFFSET), V0_HLCOEFAD.u32); 

        V0_HCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_HCCOEFAD.bits.coef_addr = (u32Addr +ADDR_OFFSET) & 0xfffffff0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCCOEFAD.u32) + u32Data * VID_OFFSET), V0_HCCOEFAD.u32); 

    }
    else if(u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VLCOEFAD.u32) + u32Data * VID_OFFSET), V0_VLCOEFAD.u32); 

        V0_VCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET));
        V0_VCCOEFAD.bits.coef_addr = (u32Addr + ADDR_OFFSET)  & 0xfffffff0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCCOEFAD.u32) + u32Data * VID_OFFSET), V0_VCCOEFAD.u32); 


    }
    else
    {
        VDP_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}

HI_VOID  VDP_VID_SetZmeInFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.zme_in_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

    return ;
}

HI_VOID  VDP_VID_SetZmeOutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
    V0_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET), V0_VSP.u32); 

    return ;
}

#if 1
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZmeNonLnrDelta(HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta)
{
    U_V0_HZONE0DELTA     V0_HZONE0DELTA;
    U_V0_HZONE2DELTA     V0_HZONE2DELTA;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeNonLnrDelta() Select Wrong Video Layer ID\n");
        return ;
    }
	
    V0_HZONE0DELTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONE0DELTA.u32) + u32Data * VID_OFFSET));
    V0_HZONE0DELTA.bits.zone0_delta = s32Zone0Delta;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONE0DELTA.u32) + u32Data * VID_OFFSET), V0_HZONE0DELTA.u32); 
	
    V0_HZONE2DELTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONE2DELTA.u32) + u32Data * VID_OFFSET));
    V0_HZONE2DELTA.bits.zone2_delta = s32Zone2Delta;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONE2DELTA.u32) + u32Data * VID_OFFSET), V0_HZONE2DELTA.u32); 
	
    return ;
}

HI_VOID  VDP_VID_SetZmeNonLnrZoneEnd(HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End)
{
    U_V0_HZONEEND        V0_HZONEEND;
    
	if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeNonLnrZoneEnd() Select Wrong Video Layer ID\n");
        return ;
    }
	
    V0_HZONEEND.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONEEND.u32) + u32Data * VID_OFFSET));
    V0_HZONEEND.bits.zone0_end = u32Zone0End-1;
    V0_HZONEEND.bits.zone1_end = u32Zone1End-1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONEEND.u32) + u32Data * VID_OFFSET), V0_HZONEEND.u32); 

    return ;
}
#endif
////////////////////////////////////
//ZME END
////////////////////////////////////

#if VID_ZME2_EN
///////////////////////////////////
//ZME2 BEGIN
///////////////////////////////////
HI_VOID  VDP_VID_SetZme2Enable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
        V0_HSP.bits.hlmsc_en = u32bEnable;
        V0_HSP.bits.hlfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
        V0_HSP.bits.hchmsc_en = u32bEnable;
        V0_HSP.bits.hchfir_en = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
    }
#if 1
    if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
    {
		V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32)));
        V0_HSP.bits.non_lnr_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32)), V0_HSP.u32);
    }
#endif

    if(u32Data == VDP_LAYER_VID1 || u32Data == VDP_LAYER_VID4)
    {
        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vlmsc_en = 0;//u32bEnable;
            V0_VSP.bits.vlfir_en = 1;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vchmsc_en = u32bEnable;
            V0_VSP.bits.vchfir_en = 1;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }
    }
    else
    {
        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vlmsc_en = u32bEnable;
            V0_VSP.bits.vlfir_en = 1;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vchmsc_en = u32bEnable;
            V0_VSP.bits.vchfir_en = 1;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }
    }

#ifdef HI3798CV200
    if(u32Data == VDP_LAYER_VID3)
    {

        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
        V0_VSP.bits.vlmsc_en = 0;//u32bEnable;
        V0_VSP.bits.vlfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 

        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
        V0_VSP.bits.vchmsc_en = 0;
        V0_VSP.bits.vchfir_en = 0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
    }
#endif

    return;
}

HI_VOID  VDP_VID_SetZme2ShootCtrlEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable,HI_U32 u32bMode)
{
    U_V0_ZME_SHOOTCTRL_HL V0_ZME_SHOOTCTRL_HL;
    U_V0_ZME_SHOOTCTRL_HC V0_ZME_SHOOTCTRL_HC;
    U_V0_ZME_SHOOTCTRL_VL V0_ZME_SHOOTCTRL_VL;
    U_V0_ZME_SHOOTCTRL_VC V0_ZME_SHOOTCTRL_VC;

    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data != VDP_LAYER_VID0)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlEnable() Select Wrong Video Layer ID\n");
        return;
    }
    
    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_HL.bits.hl_shootctrl_en   = u32bEnable;
        V0_ZME_SHOOTCTRL_HL.bits.hl_shootctrl_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_HL.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_en   = u32bEnable;
//         V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_mode = u32bMode;
        V0_ZME_SHOOTCTRL_HC.bits.hc_shootctrl_mode = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_HC.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_VL.bits.vl_shootctrl_en   = u32bEnable;
        V0_ZME_SHOOTCTRL_VL.bits.vl_shootctrl_mode = u32bMode;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_VL.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_en   = u32bEnable;
//         V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_mode = u32bMode;
        V0_ZME_SHOOTCTRL_VC.bits.vc_shootctrl_mode = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_VC.u32);      
    }

    return ;
}


HI_VOID  VDP_VID_SetZme2ShootCtrlPara  (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32Flatdect_mode,HI_U32 u32Coringadj_en,HI_U32 u32Gain,HI_U32 u32Coring)
{
    U_V0_ZME_SHOOTCTRL_HL V0_ZME_SHOOTCTRL_HL;
    U_V0_ZME_SHOOTCTRL_HC V0_ZME_SHOOTCTRL_HC;
    U_V0_ZME_SHOOTCTRL_VL V0_ZME_SHOOTCTRL_VL;
    U_V0_ZME_SHOOTCTRL_VC V0_ZME_SHOOTCTRL_VC;   

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data != VDP_LAYER_VID0)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeShootCtrlPara() Select Wrong Video Layer ID\n");
        return;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_HL.bits.hl_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_HL.bits.hl_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_HL.bits.hl_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_HL.bits.hl_coring         = u32Coring;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HL.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_HL.u32); 
    }
    
    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_HC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_HC.bits.hc_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_HC.bits.hc_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_HC.bits.hc_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_HC.bits.hc_coring         = u32Coring;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_HC.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_HC.u32);     
    }
    
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_VL.bits.vl_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_VL.bits.vl_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_VL.bits.vl_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_VL.bits.vl_coring         = u32Coring;        
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VL.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_VL.u32);     
    }
    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_ZME_SHOOTCTRL_VC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + VID_ZME_OFFSET));
        V0_ZME_SHOOTCTRL_VC.bits.vc_flatdect_mode  = u32Flatdect_mode;
        V0_ZME_SHOOTCTRL_VC.bits.vc_coringadj_en   = u32Coringadj_en;
        V0_ZME_SHOOTCTRL_VC.bits.vc_gain           = u32Gain;
        V0_ZME_SHOOTCTRL_VC.bits.vc_coring         = u32Coring;          
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_ZME_SHOOTCTRL_VC.u32) + VID_ZME_OFFSET), V0_ZME_SHOOTCTRL_VC.u32);      
    }
    return ;
}
HI_VOID  VDP_VID_SetZme2Phase    (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    U_V0_HLOFFSET  V0_HLOFFSET;
    U_V0_HCOFFSET  V0_HCOFFSET;
    U_V0_VOFFSET   V0_VOFFSET;
    U_V0_VBOFFSET  V0_VBOFFSET;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HLOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HLOFFSET.u32) + VID_ZME_OFFSET));
        V0_HLOFFSET.bits.hor_loffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HLOFFSET.u32) + VID_ZME_OFFSET), V0_HLOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HCOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HCOFFSET.u32) + VID_ZME_OFFSET));
        V0_HCOFFSET.bits.hor_coffset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOFFSET.u32) + VID_ZME_OFFSET), V0_HCOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + VID_ZME_OFFSET));
        V0_VOFFSET.bits.vluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + VID_ZME_OFFSET), V0_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + VID_ZME_OFFSET));
        V0_VOFFSET.bits.vchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VOFFSET.u32) + VID_ZME_OFFSET), V0_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + VID_ZME_OFFSET));
        V0_VBOFFSET.bits.vbluma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + VID_ZME_OFFSET), V0_VBOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VBOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + VID_ZME_OFFSET));
        V0_VBOFFSET.bits.vbchroma_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VBOFFSET.u32) + VID_ZME_OFFSET), V0_VBOFFSET.u32); 
    }

    return ;
}

HI_VOID  VDP_VID_SetZme2FirEnable(HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
        V0_HSP.bits.hlfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
        V0_HSP.bits.hchfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
        V0_VSP.bits.vlfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
        V0_VSP.bits.vchfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
    }

    return ;
}

HI_VOID  VDP_VID_SetZme2MidEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode,HI_U32 u32bEnable)
{
    U_V0_HSP V0_HSP;
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data == VDP_LAYER_VID0)
    {
        /* VHD layer zoom enable */
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
//             V0_HSP.bits.hlmid_en = u32bEnable;
            V0_HSP.bits.hlmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
//             V0_HSP.bits.hchmid_en = u32bEnable;
            V0_HSP.bits.hchmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
//             V0_VSP.bits.vlmid_en = u32bEnable;
            V0_VSP.bits.vlmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
//             V0_VSP.bits.vchmid_en = u32bEnable;
            V0_VSP.bits.vchmid_en = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }
    }
    else
    {
        /* VHD layer zoom enable */
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
            V0_HSP.bits.hlmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
            V0_HSP.bits.hchmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vlmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
            V0_VSP.bits.vchmid_en = u32bEnable;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 
        }
    }

    return ;
}

HI_VOID VDP_VID_SetZme2HorRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
    V0_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 

    return ;
}

HI_VOID  VDP_VID_SetZme2VerRatio(HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_V0_VSR V0_VSR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSR.u32) + VID_ZME_OFFSET));
    V0_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSR.u32) + VID_ZME_OFFSET), V0_VSR.u32); 

    return ;
}

HI_VOID  VDP_VID_SetZme2VfirOneTapEnable(HI_U32 u32Data, HI_U32 u32VfirOneTapEn)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetVfirOneTapEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
    V0_VSP.bits.vfir_1tap_en = u32VfirOneTapEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 

    return ;
}

HI_VOID VDP_VID_SetZme2HfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_V0_HSP V0_HSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET));
#ifdef VDP_HI3798CV200
    V0_HSP.bits.hfir_order = 1;     //u32HfirOrder;
#else
    V0_HSP.bits.hfir_order = u32HfirOrder;
#endif
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VID_ZME_OFFSET), V0_HSP.u32); 

    return ;
}

HI_VOID VDP_VID_SetZme2CoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    U_V0_HLCOEFAD V0_HLCOEFAD;
    U_V0_HCCOEFAD V0_HCCOEFAD;
    U_V0_VLCOEFAD V0_VLCOEFAD;
    U_V0_VCCOEFAD V0_VCCOEFAD;

    HI_U32 ADDR_OFFSET= 0x100 ;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_VID_PARA_ZME_HOR)
    {
        V0_HLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HLCOEFAD.u32) + VID_ZME_OFFSET));
        V0_HLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HLCOEFAD.u32) + VID_ZME_OFFSET), V0_HLCOEFAD.u32); 

        V0_HCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HCCOEFAD.u32) + VID_ZME_OFFSET));
        V0_HCCOEFAD.bits.coef_addr = (u32Addr +ADDR_OFFSET) & 0xfffffff0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCCOEFAD.u32) + VID_ZME_OFFSET), V0_HCCOEFAD.u32); 

    }
    else if(u32Mode == VDP_VID_PARA_ZME_VER)
    {
        V0_VLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLCOEFAD.u32) + VID_ZME_OFFSET));
        V0_VLCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VLCOEFAD.u32) + VID_ZME_OFFSET), V0_VLCOEFAD.u32); 

        V0_VCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VCCOEFAD.u32) + VID_ZME_OFFSET));
        V0_VCCOEFAD.bits.coef_addr = (u32Addr + ADDR_OFFSET)  & 0xfffffff0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCCOEFAD.u32) + VID_ZME_OFFSET), V0_VCCOEFAD.u32); 


    }
    else
    {
        VDP_PRINT("Error,VDP_VID_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}

HI_VOID  VDP_VID_SetZme2InFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
    V0_VSP.bits.zme_in_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 

    return ;
}

HI_VOID  VDP_VID_SetZme2OutFmt(HI_U32 u32Data, VDP_PROC_FMT_E u32Fmt)
{
    U_V0_VSP V0_VSP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeInFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET));
    V0_VSP.bits.zme_out_fmt = u32Fmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VID_ZME_OFFSET), V0_VSP.u32); 

    return ;
}

#if 1
// VID.ZME.NONLINE begin
HI_VOID  VDP_VID_SetZme2NonLnrDelta(HI_U32 u32Data, HI_S32 s32Zone0Delta, HI_S32 s32Zone2Delta)
{
    U_V0_HZONE0DELTA     V0_HZONE0DELTA;
    U_V0_HZONE2DELTA     V0_HZONE2DELTA;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeNonLnrDelta() Select Wrong Video Layer ID\n");
        return ;
    }
	
    V0_HZONE0DELTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONE0DELTA.u32) + VID_ZME_OFFSET));
    V0_HZONE0DELTA.bits.zone0_delta = s32Zone0Delta;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONE0DELTA.u32) + VID_ZME_OFFSET), V0_HZONE0DELTA.u32); 
	
    V0_HZONE2DELTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONE2DELTA.u32) + VID_ZME_OFFSET));
    V0_HZONE2DELTA.bits.zone2_delta = s32Zone2Delta;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONE2DELTA.u32) + VID_ZME_OFFSET), V0_HZONE2DELTA.u32); 
	
    return ;
}

HI_VOID  VDP_VID_SetZme2NonLnrZoneEnd(HI_U32 u32Data, HI_U32 u32Zone0End, HI_U32 u32Zone1End)
{
    U_V0_HZONEEND        V0_HZONEEND;
    
	if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZmeNonLnrZoneEnd() Select Wrong Video Layer ID\n");
        return ;
    }
	
    V0_HZONEEND.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HZONEEND.u32) + VID_ZME_OFFSET));
    V0_HZONEEND.bits.zone0_end = u32Zone0End-1;
    V0_HZONEEND.bits.zone1_end = u32Zone1End-1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HZONEEND.u32) + VID_ZME_OFFSET), V0_HZONEEND.u32); 

    return ;
}
#endif

//ZME2 coef sent drv
HI_VOID  VDP_VID_SetZme2HCoef(HI_U32 u32Data,  VDP_VID_ZME2_HCOEF_S stZme2HCoef)
{
    U_V0_HCOEF0  st_v0_hcoef0 ;
    U_V0_HCOEF1  st_v0_hcoef1 ;
    U_V0_HCOEF2  st_v0_hcoef2 ;
    U_V0_HCOEF3  st_v0_hcoef3 ;
    U_V0_HCOEF4  st_v0_hcoef4 ;
    U_V0_HCOEF5  st_v0_hcoef5 ;
    U_V0_HCOEF6  st_v0_hcoef6 ;
    U_V0_HCOEF7  st_v0_hcoef7 ;
    U_V0_HCOEF8  st_v0_hcoef8 ;

	if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZme2HCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data == VDP_LAYER_VID0)
    {
        st_v0_hcoef0.bits.hlcoef00 = stZme2HCoef.zme2_hlcoef00; 
        st_v0_hcoef0.bits.hlcoef01 = stZme2HCoef.zme2_hlcoef01; 
        st_v0_hcoef0.bits.hlcoef02 = stZme2HCoef.zme2_hlcoef02; 
        st_v0_hcoef1.bits.hlcoef03 = stZme2HCoef.zme2_hlcoef03; 
        st_v0_hcoef1.bits.hlcoef04 = stZme2HCoef.zme2_hlcoef04; 
        st_v0_hcoef1.bits.hlcoef05 = stZme2HCoef.zme2_hlcoef05; 
        st_v0_hcoef2.bits.hlcoef06 = stZme2HCoef.zme2_hlcoef06; 
        st_v0_hcoef2.bits.hlcoef07 = stZme2HCoef.zme2_hlcoef07; 
        st_v0_hcoef2.bits.hlcoef10 = stZme2HCoef.zme2_hlcoef10; 
        st_v0_hcoef3.bits.hlcoef11 = stZme2HCoef.zme2_hlcoef11; 
        st_v0_hcoef3.bits.hlcoef12 = stZme2HCoef.zme2_hlcoef12; 
        st_v0_hcoef3.bits.hlcoef13 = stZme2HCoef.zme2_hlcoef13; 
        st_v0_hcoef4.bits.hlcoef14 = stZme2HCoef.zme2_hlcoef14; 
        st_v0_hcoef4.bits.hlcoef15 = stZme2HCoef.zme2_hlcoef15; 
        st_v0_hcoef4.bits.hlcoef16 = stZme2HCoef.zme2_hlcoef16; 
        st_v0_hcoef5.bits.hlcoef17 = stZme2HCoef.zme2_hlcoef17; 

        st_v0_hcoef6.bits.hccoef00 = stZme2HCoef.zme2_hccoef00; 
        st_v0_hcoef6.bits.hccoef01 = stZme2HCoef.zme2_hccoef01; 
        st_v0_hcoef6.bits.hccoef02 = stZme2HCoef.zme2_hccoef02; 
        st_v0_hcoef7.bits.hccoef03 = stZme2HCoef.zme2_hccoef03; 
        st_v0_hcoef7.bits.hccoef10 = stZme2HCoef.zme2_hccoef10; 
        st_v0_hcoef7.bits.hccoef11 = stZme2HCoef.zme2_hccoef11; 
        st_v0_hcoef8.bits.hccoef12 = stZme2HCoef.zme2_hccoef12; 
        st_v0_hcoef8.bits.hccoef13 = stZme2HCoef.zme2_hccoef13; 

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF0.u32) + VID_ZME_OFFSET), st_v0_hcoef0.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF1.u32) + VID_ZME_OFFSET), st_v0_hcoef1.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF3.u32) + VID_ZME_OFFSET), st_v0_hcoef3.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF4.u32) + VID_ZME_OFFSET), st_v0_hcoef4.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF5.u32) + VID_ZME_OFFSET), st_v0_hcoef5.u32 );   
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF6.u32) + VID_ZME_OFFSET), st_v0_hcoef6.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF7.u32) + VID_ZME_OFFSET), st_v0_hcoef7.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_HCOEF8.u32) + VID_ZME_OFFSET), st_v0_hcoef8.u32 );  
    }
    return ;
}

HI_VOID  VDP_VID_SetZme2VCoef(HI_U32 u32Data,  VDP_VID_ZME2_VCOEF_S stZme2VCoef)
{
    U_V0_VCOEF0  st_v0_vcoef0 ;
    U_V0_VCOEF1  st_v0_vcoef1 ;
    U_V0_VCOEF2  st_v0_vcoef2 ;
    U_V0_VCOEF3  st_v0_vcoef3 ;
    U_V0_VCOEF4  st_v0_vcoef4 ;
    U_V0_VCOEF5  st_v0_vcoef5 ;
    U_V0_VCOEF6  st_v0_vcoef6 ;

	if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetZme2VCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Data == VDP_LAYER_VID0)
    {
        st_v0_vcoef0.bits.vlcoef00 = stZme2VCoef.zme2_vlcoef00; 
        st_v0_vcoef0.bits.vlcoef01 = stZme2VCoef.zme2_vlcoef01; 
        st_v0_vcoef0.bits.vlcoef02 = stZme2VCoef.zme2_vlcoef02; 
        st_v0_vcoef1.bits.vlcoef03 = stZme2VCoef.zme2_vlcoef03; 
        st_v0_vcoef1.bits.vlcoef04 = stZme2VCoef.zme2_vlcoef04; 
        st_v0_vcoef1.bits.vlcoef05 = stZme2VCoef.zme2_vlcoef05; 

        st_v0_vcoef2.bits.vlcoef10 = stZme2VCoef.zme2_vlcoef10; 
        st_v0_vcoef2.bits.vlcoef11 = stZme2VCoef.zme2_vlcoef11; 
        st_v0_vcoef2.bits.vlcoef12 = stZme2VCoef.zme2_vlcoef12; 
        st_v0_vcoef3.bits.vlcoef13 = stZme2VCoef.zme2_vlcoef13; 
        st_v0_vcoef3.bits.vlcoef14 = stZme2VCoef.zme2_vlcoef14; 
        st_v0_vcoef3.bits.vlcoef15 = stZme2VCoef.zme2_vlcoef15; 

        st_v0_vcoef4.bits.vccoef00 = stZme2VCoef.zme2_vccoef00; 
        st_v0_vcoef4.bits.vccoef01 = stZme2VCoef.zme2_vccoef01; 
        st_v0_vcoef4.bits.vccoef02 = stZme2VCoef.zme2_vccoef02; 
        st_v0_vcoef5.bits.vccoef03 = stZme2VCoef.zme2_vccoef03; 
        st_v0_vcoef5.bits.vccoef10 = stZme2VCoef.zme2_vccoef10; 
        st_v0_vcoef5.bits.vccoef11 = stZme2VCoef.zme2_vccoef11; 
        st_v0_vcoef6.bits.vccoef12 = stZme2VCoef.zme2_vccoef12; 
        st_v0_vcoef6.bits.vccoef13 = stZme2VCoef.zme2_vccoef13; 

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF0.u32) + VID_ZME_OFFSET), st_v0_vcoef0.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF1.u32) + VID_ZME_OFFSET), st_v0_vcoef1.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF3.u32) + VID_ZME_OFFSET), st_v0_vcoef3.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF4.u32) + VID_ZME_OFFSET), st_v0_vcoef4.u32 );
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF5.u32) + VID_ZME_OFFSET), st_v0_vcoef5.u32 );   
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_VCOEF6.u32) + VID_ZME_OFFSET), st_v0_vcoef6.u32 );
    }
    return ;
}
////////////////////////////////////
//ZME2 END
////////////////////////////////////
#endif

HI_VOID  VDP_VID_SetParaUpd       (HI_U32 u32Data, VDP_VID_PARA_E enMode)
{
    U_V0_PARAUP V0_PARAUP;
    U_V0_DWM_SYB_PARAUP V0_DWM_SYB_PARAUP ;
    if(enMode == VDP_VID_PARA_VMX)
    {
        V0_DWM_SYB_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DWM_SYB_PARAUP.u32) + u32Data * VID_VMX_OFFSET));
    }
    else 
    {
        V0_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET));
    }

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("error,VDP_VID_SetParaUpd() select wrong video layer id\n");
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
    else if(enMode == VDP_VID_PARA_DCI)
    {
        V0_PARAUP.bits.v0_dcicoef_upd= 0x1;
    }
    else if(enMode == VDP_VID_PARA_TNR)
    {
        V0_PARAUP.bits.tnr_upd= 0x1;
    }
    else if(enMode == VDP_VID_PARA_TNR_LUT)
    {
        V0_PARAUP.bits.tnr_lut_upd= 0x1;
    }
    else if(enMode == VDP_VID_PARA_SNR)
    {
        V0_PARAUP.bits.snr_upd= 0x1;
    }
    else if(enMode == VDP_VID_PARA_DBM)
    {
        V0_PARAUP.bits.dbm_upd= 0x1;
    }
    else if(enMode == VDP_VID_PARA_VMX)
    {
        V0_DWM_SYB_PARAUP.bits.para_up= 0x1;
    }
    else
    {
        VDP_PRINT("error,VDP_VID_SetParaUpd() select wrong mode!\n");
    }

    if(enMode == VDP_VID_PARA_VMX)
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DWM_SYB_PARAUP.u32) + u32Data * VID_VMX_OFFSET), V0_DWM_SYB_PARAUP.u32); 
    else    
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PARAUP.u32) + u32Data * VID_OFFSET), V0_PARAUP.u32); 
    
    return ;
}

HI_VOID VDP_VID_SetCoefReadEnable(HI_U32 u32Id, HI_U32 u32Para)
{
    HI_U32 u32WrData;
    u32WrData = 1 << u32Para; 

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PARARD.u32) + u32Id*4), u32WrData);
}

HI_VOID VDP_VID_SetCoefReadDisable(HI_U32 u32Id, HI_U32 u32Para)
{
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_PARARD.u32) + u32Id*4), 0);
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
        VDP_PRINT("Error,VDP_VP_SetThreeDimDofEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_DOF_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DOF_CTRL.u32)));
    VP0_DOF_CTRL.bits.dof_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DOF_CTRL.u32)), VP0_DOF_CTRL.u32);
}

HI_VOID  VDP_VP_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
    U_VP0_DOF_STEP  VP0_DOF_STEP;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetThreeDimDofStep() Select Wrong Video Layer ID\n");
        return ;
    }
    VP0_DOF_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DOF_STEP.u32)));
    VP0_DOF_STEP.bits.right_step= s32RStep;
    VP0_DOF_STEP.bits.left_step = s32LStep;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DOF_STEP.u32)), VP0_DOF_STEP.u32);
}

HI_VOID  VDP_VP_SetThreeDimDofBkg   (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_VP0_DOF_BKG  VP0_DOF_BKG; 
    //U_G0_DOF_ALPHA G0_DOF_ALPHA; 

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetThreeDimDofBkg() Select Wrong Graph Layer ID\n");
        return ;
    }

    VP0_DOF_BKG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DOF_BKG.u32)));
    VP0_DOF_BKG.bits.dof_bk_y  = stBkg.u32BkgY ;
    VP0_DOF_BKG.bits.dof_bk_cb = stBkg.u32BkgU;
    VP0_DOF_BKG.bits.dof_bk_cr = stBkg.u32BkgV;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DOF_BKG.u32)), VP0_DOF_BKG.u32); 

    return ;
}



HI_VOID  VDP_VP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetVideoPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    VP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET));
    VP0_VFPOS.bits.video_xfpos = stRect.u32X;
    VP0_VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET), VP0_VFPOS.u32); 

    VP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET));
    VP0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    VP0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET), VP0_VLPOS.u32); 

    return ;
}


HI_VOID  VDP_VP_GetVideoPos(HI_U32 u32Data, VDP_RECT_S  *pstRect)
{
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetVideoPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    VP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET));
    pstRect->u32X = VP0_VFPOS.bits.video_xfpos;
    pstRect->u32Y = VP0_VFPOS.bits.video_yfpos;

    VP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET));
    pstRect->u32Wth = VP0_VLPOS.bits.video_xlpos + 1 -  pstRect->u32X;
    pstRect->u32Hgt = VP0_VLPOS.bits.video_ylpos + 1 - pstRect->u32Y;
    return ;
}


HI_VOID  VDP_VP_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_VP0_DFPOS VP0_DFPOS;
    U_VP0_DLPOS VP0_DLPOS;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDispPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    VP0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32) + u32Data * VP_OFFSET));
    VP0_DFPOS.bits.disp_xfpos = stRect.u32X;
    VP0_DFPOS.bits.disp_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32) + u32Data * VP_OFFSET), VP0_DFPOS.u32); 

    VP0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32) + u32Data * VP_OFFSET));
    VP0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
    VP0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32) + u32Data * VP_OFFSET), VP0_DLPOS.u32); 
    return ;
}   

HI_VOID  VDP_VP_SetInReso      (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_VP0_IRESO VP0_IRESO;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetInReso() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32) + u32Data * VP_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32Wth - 1;
    VP0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32) + u32Data * VP_OFFSET), VP0_IRESO.u32); 

    return ;
}

HI_VOID  VDP_VP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    U_VP0_GALPHA VP0_GALPHA;

    //special for bk alpha = video alpha
    U_VP0_ALPHA     VP0_ALPHA;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetLayerGalpha() Select Wrong vp Layer ID\n");
        return ;
    }


    VP0_GALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_GALPHA.u32) + u32Data * VP_OFFSET));
    VP0_GALPHA.bits.galpha = u32Alpha;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_GALPHA.u32) + u32Data * VP_OFFSET), VP0_GALPHA.u32); 


    VP0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ALPHA.u32)));
    VP0_ALPHA.bits.vbk_alpha = u32Alpha;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->VP0_ALPHA.u32), VP0_ALPHA.u32); 

    return ;
}

HI_VOID  VDP_VP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_VP0_BK VP0_BK;
    //U_VP0_ALPHA     VP0_ALPHA;

    if(u32Data == VDP_LAYER_VP0)
    {
        VP0_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_BK.u32)));
        VP0_BK.bits.vbk_y  = stBkg.u32BkgY;
        VP0_BK.bits.vbk_cb = stBkg.u32BkgU;
        VP0_BK.bits.vbk_cr = stBkg.u32BkgV;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->VP0_BK.u32), VP0_BK.u32); 

        //        VP0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ALPHA.u32)));
        //        VP0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
        //        VDP_RegWrite((HI_ULONG)&(pVdpReg->VP0_ALPHA.u32), VP0_ALPHA.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_VP_SetLayerBkg() Select Wrong VP Layer ID\n");
    }

    return ;
}

HI_VOID VDP_VP_SetMuteEnable     (HI_U32 u32Data, HI_U32 bEnable)
{
    U_VP0_CTRL VP0_CTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetMuteEnable() Select Wrong VP Layer ID\n");
        return ;
    }

    VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32); 

    return ;
}

HI_VOID  VDP_VP_SetMuteBkg(HI_U32 u32Data, VDP_VP_MUTE_BK_S stVpMuteBkg)
{
    U_VP0_MUTE_BK VP0_MUTE_BK;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetMuteBkg() Select Wrong VP Layer ID\n");
        return ;
    }
    
    VP0_MUTE_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_MUTE_BK.u32) + u32Data * VP_OFFSET));
    VP0_MUTE_BK.bits.mute_y  = stVpMuteBkg.u32VpMuteBkgY;
    VP0_MUTE_BK.bits.mute_cb = stVpMuteBkg.u32VpMuteBkgU;
    VP0_MUTE_BK.bits.mute_cr = stVpMuteBkg.u32VpMuteBkgV;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_MUTE_BK.u32) + u32Data * VP_OFFSET), VP0_MUTE_BK.u32); 

    return ;
}

HI_VOID VDP_VP_SetDispMode(HI_U32 u32Data, VDP_DISP_MODE_E enDispMode)
{
    //U_VP0_CTRL VP0_CTRL;
    //
    //if(u32Data >= VP_MAX)
    //{
    //    VDP_PRINT("Error,VDP_VP_SetDispMode() Select Wrong Video Layer ID\n");
    //    return ;
    //}

    //
    //VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    //VP0_CTRL.bits.disp_mode = enDispMode;
    //VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VID_OFFSET), VP0_CTRL.u32); 

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
        VDP_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        VP0_CSC0_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
        VP0_CSC0_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32); 

        VP0_CSC0_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
        VP0_CSC0_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC0_ODC.u32); 

        VP0_CSC0_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
        VP0_CSC0_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IODC.u32); 
    }
    else if(u32Num == 1)
    {
        VP0_CSC1_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
        VP0_CSC1_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32); 

        VP0_CSC1_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
        VP0_CSC1_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC1_ODC.u32); 

        VP0_CSC1_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
        VP0_CSC1_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IODC.u32); 
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
        VDP_PRINT("Error,VDP_VP_SetCscCoef Select Wrong video ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        VP0_CSC0_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_P0.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P0.bits.cscp00 = stCscCoef.csc_coef00;
        VP0_CSC0_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_P0.u32)+u32Data*VP_OFFSET), VP0_CSC0_P0.u32);

        VP0_CSC0_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_P1.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P1.bits.cscp02 = stCscCoef.csc_coef02;
        VP0_CSC0_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_P1.u32)+u32Data*VP_OFFSET), VP0_CSC0_P1.u32);

        VP0_CSC0_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_P2.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P2.bits.cscp11 = stCscCoef.csc_coef11;
        VP0_CSC0_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_P2.u32)+u32Data*VP_OFFSET), VP0_CSC0_P2.u32);

        VP0_CSC0_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_P3.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P3.bits.cscp20 = stCscCoef.csc_coef20;
        VP0_CSC0_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_P3.u32)+u32Data*VP_OFFSET), VP0_CSC0_P3.u32);

        VP0_CSC0_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_P4.u32)+u32Data*VP_OFFSET));
        VP0_CSC0_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_P4.u32)+u32Data*VP_OFFSET), VP0_CSC0_P4.u32);
    }
    else if(u32Num == 1)
    {
        VP0_CSC1_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_P0.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P0.bits.cscp00 = stCscCoef.csc_coef00;
        VP0_CSC1_P0.bits.cscp01 = stCscCoef.csc_coef01;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_P0.u32)+u32Data*VP_OFFSET), VP0_CSC1_P0.u32);

        VP0_CSC1_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_P1.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P1.bits.cscp02 = stCscCoef.csc_coef02;
        VP0_CSC1_P1.bits.cscp10 = stCscCoef.csc_coef10;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_P1.u32)+u32Data*VP_OFFSET), VP0_CSC1_P1.u32);

        VP0_CSC1_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_P2.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P2.bits.cscp11 = stCscCoef.csc_coef11;
        VP0_CSC1_P2.bits.cscp12 = stCscCoef.csc_coef12;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_P2.u32)+u32Data*VP_OFFSET), VP0_CSC1_P2.u32);

        VP0_CSC1_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_P3.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P3.bits.cscp20 = stCscCoef.csc_coef20;
        VP0_CSC1_P3.bits.cscp21 = stCscCoef.csc_coef21;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_P3.u32)+u32Data*VP_OFFSET), VP0_CSC1_P3.u32);

        VP0_CSC1_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_P4.u32)+u32Data*VP_OFFSET));
        VP0_CSC1_P4.bits.cscp22 = stCscCoef.csc_coef22;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_P4.u32)+u32Data*VP_OFFSET), VP0_CSC1_P4.u32);
    }

}

HI_VOID  VDP_VP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn, HI_U32 u32Num)
{
    U_VP0_CSC0_IDC VP0_CSC0_IDC;
    U_VP0_CSC1_IDC VP0_CSC1_IDC;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Num == 0)
    {
        VP0_CSC0_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC0_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC0_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC0_IDC.u32); 
    }
    else if(u32Num == 1)
    {
        VP0_CSC1_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET));
        VP0_CSC1_IDC.bits.csc_en = u32bCscEn;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC1_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC1_IDC.u32); 
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
        VDP_PRINT("Error,VDP_VP_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

	VP0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
	VP0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC_IDC.u32); 

	VP0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_ODC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
	VP0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_ODC.u32) + u32Data * VP_OFFSET), VP0_CSC_ODC.u32); 

	VP0_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_IODC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
	VP0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_IODC.u32) + u32Data * VP_OFFSET), VP0_CSC_IODC.u32); 


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
        VDP_PRINT("Error,VDP_VP_SetCscCoef Select Wrong video ID\n");
        return ;
	}

	VP0_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_P0.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
	VP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_P0.u32)+u32Data*VP_OFFSET), VP0_CSC_P0.u32);

	VP0_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_P1.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
	VP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_P1.u32)+u32Data*VP_OFFSET), VP0_CSC_P1.u32);

	VP0_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_P2.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
	VP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_P2.u32)+u32Data*VP_OFFSET), VP0_CSC_P2.u32);

	VP0_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_P3.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
	VP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_P3.u32)+u32Data*VP_OFFSET), VP0_CSC_P3.u32);

	VP0_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_P4.u32)+u32Data*VP_OFFSET));
	VP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_P4.u32)+u32Data*VP_OFFSET), VP0_CSC_P4.u32);

}

HI_VOID  VDP_VP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
	U_VP0_CSC_IDC VP0_CSC_IDC;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetCscEnable() Select Wrong Video Layer ID\n");
        return ;
    }

	VP0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET));
	VP0_CSC_IDC.bits.csc_en = u32bCscEn;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_IDC.u32) + u32Data * VP_OFFSET), VP0_CSC_IDC.u32); 
    
	return ;
}

HI_VOID VDP_VP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)
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

    VDP_VP_SetCscCoef  (u32Data, st_csc_coef);
    VDP_VP_SetCscDcCoef(u32Data, st_csc_dc_coef);

    return ;
}    

HI_VOID VDP_VP_SetAcmEnable (HI_U32 u32Data, HI_U32 u32bAcmEn)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmEnable() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_en = u32bAcmEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;

}


HI_VOID VDP_VP_SetAcmDbgEn(HI_U32 u32Data , HI_U32 u32bAcmDbgEn)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmDbgEn() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_dbg_en = u32bAcmDbgEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmDbgMode(HI_U32 u32Data , HI_U32 u32bAcmDbgMode)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmDbgEn() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_dbg_mode = u32bAcmDbgMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmDbgPos(HI_U32 u32Data, HI_U32 acm_dbg_pos)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetAcmDbgPos Select Wrong Layer ID\n");
	}
	
	VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
#ifdef VDP_HI3798CV200
	VP0_ACM_CTRL.bits.acm_dbg_pos = acm_dbg_pos/2*2;
#else
	VP0_ACM_CTRL.bits.acm_dbg_pos = acm_dbg_pos;
#endif
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET),VP0_ACM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VP_SetAcmCliporwrap(HI_U32 u32Data, HI_U32 u32AcmCliporwrap)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmCliporWrap() Select Wrong CHANNEL ID\n");
        return ;
    }
    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cliporwrap= u32AcmCliporwrap;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmStretch(HI_U32 u32Data, HI_U32 u32AcmStrch)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmStretch() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_stretch = u32AcmStrch;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmClipRange(HI_U32 u32Data, HI_U32 u32AcmClipRange)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmClipRange() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cliprange = u32AcmClipRange;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmChmThd(HI_U32 u32Data, HI_U32 u32AcmCbcrthr)
{
    U_VP0_ACM_CTRL  VP0_ACM_CTRL;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmChmThd() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CTRL.bits.acm_cbcrthr = u32AcmCbcrthr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CTRL.u32) + u32Data * VP_OFFSET), VP0_ACM_CTRL.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmGainLum(HI_U32 u32Data, HI_U32 u32AcmGain0)
{
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmGainLum() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain0 = u32AcmGain0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmGainSat(HI_U32 u32Data, HI_U32 u32AcmGain2)
{
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmGainSat() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain2 = u32AcmGain2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmGainHue(HI_U32 u32Data, HI_U32 u32AcmGain1)
{
    U_VP0_ACM_ADJ  VP0_ACM_ADJ;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmGainHue() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_ADJ.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET));
    VP0_ACM_ADJ.bits.acm_gain1 = u32AcmGain1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_ADJ.u32) + u32Data * VP_OFFSET), VP0_ACM_ADJ.u32); 

    return ;
}

HI_VOID VDP_VP_SetAcmCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr)
{
    U_VP0_ACM_CAD VP0_ACM_CAD;

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAcmGainHue() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_ACM_CAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACM_CAD.u32) + u32Data * VP_OFFSET));
    VP0_ACM_CAD.bits.coef_addr = u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACM_CAD.u32) + u32Data * VP_OFFSET), VP0_ACM_CAD.u32); 

    return ;
}

#if VP0_ACC_EN
HI_VOID VDP_VP_SetAccThd( HI_U32 u32Data,ACCTHD_S stAccThd)
{
    U_VP0_ACCTHD1 VP0_ACCTHD1;
    U_VP0_ACCTHD2 VP0_ACCTHD2;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAccThd() Select Wrong VP ID\n");
        return ;
    }

    VP0_ACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.bits.acc_multiple = stAccThd.acc_multi   ;
    VP0_ACCTHD2.bits.thd_med_high = stAccThd.thd_med_high;
    VP0_ACCTHD1.bits.thd_med_low  = stAccThd.thd_med_low ;
    VP0_ACCTHD1.bits.thd_high     = stAccThd.thd_high    ;
    VP0_ACCTHD1.bits.thd_low      = stAccThd.thd_low     ;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD2.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCTHD1.u32) + u32Data * VP_OFFSET), VP0_ACCTHD1.u32);

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
        VDP_PRINT("Error,VDP_VP_SetAccTab() Select Wrong VP ID\n");
        return ;
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCLOW[ii].bits.table_data1n = upTable[0 + ii*3];
        VP0_ACCLOW[ii].bits.table_data2n = upTable[1 + ii*3];
        VP0_ACCLOW[ii].bits.table_data3n = upTable[2 + ii*3];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCLOWN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCLOW[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCMED[ii].bits.table_data1n = upTable[9 + 0 + ii*3];
        VP0_ACCMED[ii].bits.table_data2n = upTable[9 + 1 + ii*3];
        VP0_ACCMED[ii].bits.table_data3n = upTable[9 + 2 + ii*3];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCMEDN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCMED[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCHIGH[ii].bits.table_data1n = upTable[18 + 0 + ii*3];
        VP0_ACCHIGH[ii].bits.table_data2n = upTable[18 + 1 + ii*3];
        VP0_ACCHIGH[ii].bits.table_data3n = upTable[18 + 2 + ii*3];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCHIGHN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCHIGH[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCML[ii].bits.table_data1n = upTable[27 + 0 + ii*3];
        VP0_ACCML[ii].bits.table_data2n = upTable[27 + 1 + ii*3];
        VP0_ACCML[ii].bits.table_data3n = upTable[27 + 2 + ii*3];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCMLN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCML[ii].u32);
    }

    for (ii = 0; ii < 3; ii++)
    {
        VP0_ACCMH[ii].bits.table_data1n = upTable[36 + 0 + ii*3];
        VP0_ACCMH[ii].bits.table_data2n = upTable[36 + 1 + ii*3];
        VP0_ACCMH[ii].bits.table_data3n = upTable[36 + 2 + ii*3];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCMHN[ii].u32)+ u32Data * VP_OFFSET), VP0_ACCMH[ii].u32);
    }

}

HI_VOID VDP_VP_SetAccCtrl(HI_U32 u32Data,HI_U32 uAccEn, HI_U32 uAccMode)
{
    U_VP0_ACCTHD1  VP0_ACCTHD1;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAccCtrl() Select Wrong VP ID\n");
        return ;
    }

    VP0_ACCTHD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD1.bits.acc_en   = uAccEn  ;
    VP0_ACCTHD1.bits.acc_mode = uAccMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCTHD1.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD1.u32);
}


HI_VOID VDP_VP_SetAccRst(HI_U32 u32Data,HI_U32 uAccRst)
{
    U_VP0_ACCTHD2 VP0_ACCTHD2;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetAccRst() Select Wrong VP ID\n");
        return ;
    }


    VP0_ACCTHD2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET));
    VP0_ACCTHD2.bits.acc_rst   = uAccRst  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACCTHD2.u32)+ u32Data * VP_OFFSET), VP0_ACCTHD2.u32);
}

HI_VOID  VDP_VP0_SetAccCad(HI_U32 u32Data, HI_U32 u32addr)
{
    U_VP0_ACC_CAD VP0_ACC_CAD;
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("error,VDP_VP0_SetAccCad() select wrong video layer id\n");
        return ;
    }

    VP0_ACC_CAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_ACC_CAD.u32) + u32Data * VID_OFFSET));

    VP0_ACC_CAD.bits.coef_addr= u32addr;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_ACC_CAD.u32) + u32Data * VID_OFFSET), VP0_ACC_CAD.u32); 
    return ;
}
#endif


HI_VOID  VDP_VP0_SetParaUpd       (HI_U32 u32Data, VDP_VP_PARA_E  enMode)
{
    U_VP0_PARAUP  VP0_PARAUP;
    U_V0_DWM_SYB_PARAUP V0_DWM_SYB_PARAUP ;
    if(enMode == VDP_VP_PARA_VMX)
        V0_DWM_SYB_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DWM_SYB_PARAUP.u32) + 2 * VID_VMX_OFFSET));
    else
        VP0_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET));

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("error,VDP_VP0_SetParaUpd() select wrong video layer id\n");
        return ;
    }

#if 0
    //mod me!
    if(enMode == VDP_DISP_COEFMODE_ACC)
    {
        VP0_PARAUP.bits.v0_acccoef_upd= 0x1;
    }
#endif

    if(enMode == VDP_VP_PARA_ACM)
    {
        VP0_PARAUP.bits.vp0_acmcoef_upd= 0x1;
    }
    else if(enMode == VDP_VP_PARA_VMX)
    {
        V0_DWM_SYB_PARAUP.bits.para_up= 0x1;
    }


    if(enMode == VDP_VP_PARA_VMX)
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DWM_SYB_PARAUP.u32) + 2 * VID_VMX_OFFSET), V0_DWM_SYB_PARAUP.u32); 
    else
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_PARAUP.u32) + u32Data * VID_OFFSET), VP0_PARAUP.u32); 
    return ;
}


//DCI_BEING
//-------------------------------------------------
//DCI drivers 
//-------------------------------------------------
HI_VOID VDP_VP_SetDciCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr)
{
    U_VP0_DCI_CAD   VP0_DCI_CAD;
#ifdef VDP_HI3798CV200
    U_V0_DCICOEFAD  V0_DCICOEFAD;
#endif

    if(u32Data>= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciCoefAddr() Select Wrong CHANNEL ID\n");
        return ;
    }

    VP0_DCI_CAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_CAD.u32) + u32Data * VP_OFFSET));
    VP0_DCI_CAD.bits.coef_addr = u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_CAD.u32) + u32Data * VP_OFFSET), VP0_DCI_CAD.u32); 

#ifdef VDP_HI3798CV200
    V0_DCICOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCICOEFAD.u32) + u32Data * VID_OFFSET));
    V0_DCICOEFAD.bits.dci_coef_addr = u32CoefAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCICOEFAD.u32) + u32Data * VID_OFFSET), V0_DCICOEFAD.u32); 
#endif

    return ;
}

HI_VOID VDP_VP_SetDciEnable(HI_U32 u32Data, HI_U32 u32DciEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciEnable() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_en = u32DciEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	
}

HI_VOID VDP_VP_SetDciDbgEn(HI_U32 u32Data, HI_U32 u32DciDbgEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciDbgEn() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_dbg_en = u32DciDbgEn;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
    
}

HI_VOID VDP_VP_SetDciDbgMode(HI_U32 u32Data, HI_U32 u32DciDbgMode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciDbgMode() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_dbg_mode = u32DciDbgMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
    
}

HI_VOID VDP_VP_SetDciSceneFlg(HI_U32 u32Data, HI_U32 u32SceneFlg)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciSceneFlg() Select Wrong VP ID\n");
        return ;
    }

    VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DCICTRL.bits.dci_scene_flg = u32SceneFlg;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
    
}

HI_VOID VDP_VP_SetDciManAdj(HI_U32 u32Data, VDP_VP_DCI_ADJ_NUM_E Adjnum, VDP_VP_DCI_CTRL_MODE_E Manmode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

    if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciManAdj() Select Wrong VP ID\n");
        return ;
    }

	if(Adjnum == VDP_VP_DCI_ADJ0 )
	{
		VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj0 = Manmode;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ1)
	{
		VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj1 = Manmode;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ2)
	{
		VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_man_adj2 = Manmode;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}

}

HI_VOID VDP_VP_SetDciCbCrcmpEn(HI_U32 u32Data, HI_U32 u32ClorEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciCbCrcmpEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_cbcrcmp_en = u32ClorEn;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciCbCrstaEn(HI_U32 u32Data, HI_U32 u32ClorstaEn)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciCbCrstaEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_cbcrsta_en = u32ClorstaEn;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciRange(HI_U32 u32Data, VDP_DCI_RANGE_E Rangeinout , HI_U32 u32Rangemode)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciRange() Select Wrong VP ID\n");
        return ;
    }

	if(Rangeinout == IN_RANGE )
	{
		VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_in_range = u32Rangemode;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
	else
	{
		VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
		VP0_DCICTRL.bits.dci_out_range = u32Rangemode;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
	}
}

HI_VOID VDP_VP_SetDciShiftCtrl(HI_U32 u32Data, VDP_DCI_SHIFT_E enShiftCtrl)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciShiftCtrl() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_shift_ctrl = enShiftCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciLpfEn(HI_U32 u32Data, HI_U32 u32enLpf)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
    {
        VDP_PRINT("Error,VDP_VP_SetDciLpfEn() Select Wrong VP ID\n");
        return ;
    }

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_histlpf_en = u32enLpf;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);

}

HI_VOID VDP_VP_SetDciSwcfgEn(HI_U32 u32Data, HI_U32 u32enSwcfg)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciSwcfgEn() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_swcfg_en = u32enSwcfg;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciHistWgtEn(HI_U32 u32Data, HI_U32 u32enSwcfg)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciSwcfgEn() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_hist_wgt_en = u32enSwcfg;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciYLinear(HI_U32 u32Data, HI_U32 u32DciYLinear)//dci add y_linear
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciYLinear() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_y_linear = u32DciYLinear;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciStatCtrl(HI_U32 u32Data, HI_U32 u32StatCtrl)
{
    U_VP0_DCICTRL VP0_DCICTRL;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciStatCtrl() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET));
	VP0_DCICTRL.bits.dci_stat_ctrl = u32StatCtrl;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32)+ u32Data * VP_OFFSET), VP0_DCICTRL.u32);
}

HI_VOID VDP_VP_SetDciPosition(HI_U32 u32Data, VDP_DCI_POS_S stDciPos)
{
	U_VP0_DCIHPOS VP0_DCIHPOS;
	U_VP0_DCIVPOS VP0_DCIVPOS;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciPosition() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHPOS.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHPOS.bits.dci_hstart = stDciPos.hstart;
	VP0_DCIHPOS.bits.dci_hend = stDciPos.hend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHPOS.u32)+ u32Data * VP_OFFSET), VP0_DCIHPOS.u32);

	VP0_DCIVPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIVPOS.u32)+ u32Data * VP_OFFSET));
	VP0_DCIVPOS.bits.dci_vstart = stDciPos.vstart;
	VP0_DCIVPOS.bits.dci_vend = stDciPos.vend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIVPOS.u32)+ u32Data * VP_OFFSET), VP0_DCIVPOS.u32);
}

HI_VOID VDP_VP_SetDciHisBld(HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara)
{
    U_VP0_DCIHISBLD VP0_DCIHISBLD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciHisBld() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISBLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISBLD.bits.dci_cbcrsta_y = pstDciPara->s32CbCrSta_Y;       
	VP0_DCIHISBLD.bits.dci_cbcrsta_cb = pstDciPara->s32CbCrSta_Cb;       
	VP0_DCIHISBLD.bits.dci_cbcrsta_cr = pstDciPara->s32CbCrSta_Cr;      
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET), VP0_DCIHISBLD.u32);
}

HI_VOID VDP_VP_SetDciBsLimit(HI_U32 u32Data,HI_U32  u32BsLimit)
{
    U_VP0_DCIHISBLD VP0_DCIHISBLD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciHisBld() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISBLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISBLD.bits.bs_limit_thr = u32BsLimit;      
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISBLD.u32)+ u32Data * VP_OFFSET), VP0_DCIHISBLD.u32);
}

HI_VOID VDP_VP_SetDciHisOft(HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara)
{
    U_VP0_DCIHISOFT VP0_DCIHISOFT ;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciHisOft() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISOFT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISOFT.bits.dci_cbcrsta_oft = pstDciPara->s32CbCrSta_Offset;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET), VP0_DCIHISOFT.u32);
}

HI_VOID VDP_VP_SetDciLimitAblend(HI_U32 u32Data, HI_U32 u32BsLimitAblend)
{
    U_VP0_DCIHISOFT VP0_DCIHISOFT ;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciHisOft() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISOFT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISOFT.bits.bs_limit_ablend = 0;//u32BsLimitAblend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32)+ u32Data * VP_OFFSET), VP0_DCIHISOFT.u32);
}

HI_VOID VDP_VP_SetDciHisCor(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara)
{
    U_VP0_DCIHISCOR VP0_DCIHISCOR;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciHisCor() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIHISCOR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISCOR.u32)+ u32Data * VP_OFFSET));
	VP0_DCIHISCOR.bits.dci_histcor_thr0 = pstDciPara->s32HistCoring[0];
	VP0_DCIHISCOR.bits.dci_histcor_thr1 = pstDciPara->s32HistCoring[1];
	VP0_DCIHISCOR.bits.dci_histcor_thr2 = pstDciPara->s32HistCoring[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISCOR.u32)+ u32Data * VP_OFFSET), VP0_DCIHISCOR.u32);

}

HI_VOID VDP_VP_SetDciMerBld(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara)
{
    U_VP0_DCIMERBLD VP0_DCIMERBLD;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciMerBld() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIMERBLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIMERBLD.u32)+ u32Data * VP_OFFSET));
	VP0_DCIMERBLD.bits.dci_metrc_abld0 = pstDciPara->s32MetricAlpha[0];
	VP0_DCIMERBLD.bits.dci_metrc_abld1 = pstDciPara->s32MetricAlpha[1];
	VP0_DCIMERBLD.bits.dci_metrc_abld2 = pstDciPara->s32MetricAlpha[2];
	VP0_DCIMERBLD.bits.dci_hist_abld = pstDciPara->s32HistABlend;
	VP0_DCIMERBLD.bits.dci_org_abld  = pstDciPara->s32OrgABlend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIMERBLD.u32)+ u32Data * VP_OFFSET), VP0_DCIMERBLD.u32);

}

HI_VOID VDP_VP_SetDciAdjWgt(HI_U32 u32Data,VDP_VP_DCI_ADJ_NUM_E Adjnum,VDP_DCI_PARA_S *pstDciPara)
{
    U_VP0_DCIADJWGT VP0_DCIADJWGT;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciAdjWgt() Select Wrong VP ID\n");
		return ;
	}

	if(Adjnum == VDP_VP_DCI_ADJ0)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt0 = pstDciPara->s32ManAdjWgt[0];
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ1)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt1 = pstDciPara->s32ManAdjWgt[1];
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
	else if(Adjnum == VDP_VP_DCI_ADJ2)
	{
		VP0_DCIADJWGT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET));
		VP0_DCIADJWGT.bits.dci_man_adjwgt2 = pstDciPara->s32ManAdjWgt[2];
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJWGT.u32)+ u32Data * VP_OFFSET), VP0_DCIADJWGT.u32);
	}
}

HI_VOID VDP_VP_SetDciClip(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara )
{
    U_VP0_DCICLIP0 VP0_DCICLIP0;
    U_VP0_DCICLIP1 VP0_DCICLIP1;
    U_VP0_DCICLIP2 VP0_DCICLIP2;

	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciClip() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCICLIP0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICLIP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP0.bits.dci_wgt_cliplow0  = pstDciPara->s32AdjGain_ClipL[0];
	VP0_DCICLIP0.bits.dci_wgt_cliphigh0  = pstDciPara->s32AdjGain_ClipH[0];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICLIP0.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP0.u32);

	VP0_DCICLIP1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICLIP1.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP1.bits.dci_wgt_cliplow1  = pstDciPara->s32AdjGain_ClipL[1];
	VP0_DCICLIP1.bits.dci_wgt_cliphigh1  = pstDciPara->s32AdjGain_ClipH[1];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICLIP1.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP1.u32);

	VP0_DCICLIP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICLIP2.u32)+ u32Data * VP_OFFSET));
	VP0_DCICLIP2.bits.dci_wgt_cliplow2  = pstDciPara->s32AdjGain_ClipL[2];
	VP0_DCICLIP2.bits.dci_wgt_cliphigh2  = pstDciPara->s32AdjGain_ClipH[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCICLIP2.u32)+ u32Data * VP_OFFSET), VP0_DCICLIP2.u32);

}

HI_VOID VDP_VP_SetDciGlbGain(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara )
{
    U_VP0_DCIGLBGAIN VP0_DCIGLBGAIN;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetDciGlbGain() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIGLBGAIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIGLBGAIN.u32)+ u32Data * VP_OFFSET));
	VP0_DCIGLBGAIN.bits.dci_glb_gain0 = pstDciPara->s32GlbGain0;
	VP0_DCIGLBGAIN.bits.dci_glb_gain1 = pstDciPara->s32GlbGain1;
	VP0_DCIGLBGAIN.bits.dci_glb_gain2 = pstDciPara->s32GlbGain2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIGLBGAIN.u32)+ u32Data * VP_OFFSET), VP0_DCIGLBGAIN.u32);
}

HI_VOID VDP_VP_SetDciPos(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara )
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
		VDP_PRINT("Error,VDP_VP_SetDciPos() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIPOSTHR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSTHR0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr1  = pstDciPara->s32CbCrGainPos_Ythrsh[0];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr2  = pstDciPara->s32CbCrGainPos_Ythrsh[1];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr3  = pstDciPara->s32CbCrGainPos_Ythrsh[2];
	VP0_DCIPOSTHR0.bits.dci_gainpos_thr4  = pstDciPara->s32CbCrGainPos_Ythrsh[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSTHR0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSTHR0.u32);

	VP0_DCIPOSTHR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSTHR1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr5  = pstDciPara->s32CbCrGainPos_Ythrsh[4];
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr6  = pstDciPara->s32CbCrGainPos_Ythrsh[5];
	VP0_DCIPOSTHR1.bits.dci_gainpos_thr7  = pstDciPara->s32CbCrGainPos_Ythrsh[6];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSTHR1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSTHR1.u32);
       

	VP0_DCIPOSGAIN0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSGAIN0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSGAIN1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSGAIN1.u32)+ u32Data * VP_OFFSET));

	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr0 = pstDciPara->s32CbCrGainPos[0];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr1 = pstDciPara->s32CbCrGainPos[1];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr2 = pstDciPara->s32CbCrGainPos[2];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr3 = pstDciPara->s32CbCrGainPos[3];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr4 = pstDciPara->s32CbCrGainPos[4];
	VP0_DCIPOSGAIN0.bits.dci_gainpos_cbcr5 = pstDciPara->s32CbCrGainPos[5];
	VP0_DCIPOSGAIN1.bits.dci_gainpos_cbcr6 = pstDciPara->s32CbCrGainPos[6];
	VP0_DCIPOSGAIN1.bits.dci_gainpos_cbcr7 = pstDciPara->s32CbCrGainPos[7];

	VP0_DCIPOSGAIN1.bits.dci_gainpos_cbcr8 = pstDciPara->s32CbCrGainPos[8];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSGAIN0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSGAIN0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSGAIN1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSGAIN1.u32);


	VP0_DCIPOSSLP0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSSLP1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIPOSSLP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP2.u32)+ u32Data * VP_OFFSET));

	VP0_DCIPOSSLP0.bits.dci_gainpos_slp0 = pstDciPara->s32CbCrGainPos_Slp[0];
	VP0_DCIPOSSLP0.bits.dci_gainpos_slp1 = pstDciPara->s32CbCrGainPos_Slp[1];
	VP0_DCIPOSSLP0.bits.dci_gainpos_slp2 = pstDciPara->s32CbCrGainPos_Slp[2];

	VP0_DCIPOSSLP1.bits.dci_gainpos_slp3 = pstDciPara->s32CbCrGainPos_Slp[3];
	VP0_DCIPOSSLP1.bits.dci_gainpos_slp4 = pstDciPara->s32CbCrGainPos_Slp[4];
	VP0_DCIPOSSLP1.bits.dci_gainpos_slp5 = pstDciPara->s32CbCrGainPos_Slp[5];

	VP0_DCIPOSSLP2.bits.dci_gainpos_slp6 = pstDciPara->s32CbCrGainPos_Slp[6];
	VP0_DCIPOSSLP2.bits.dci_gainpos_slp7 = pstDciPara->s32CbCrGainPos_Slp[7];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP0.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP1.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIPOSSLP2.u32)+ u32Data * VP_OFFSET), VP0_DCIPOSSLP2.u32);
}

HI_VOID VDP_VP_SetDciNeg(HI_U32 u32Data,VDP_DCI_PARA_S *pstDciPara )
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
		VDP_PRINT("Error,VDP_VP_SetDciNeg() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCINEGTHR0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGTHR0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGTHR0.bits.dci_gainneg_thr1  = pstDciPara->s32CbCrGainNeg_Ythrsh[0];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr2  = pstDciPara->s32CbCrGainNeg_Ythrsh[1];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr3  = pstDciPara->s32CbCrGainNeg_Ythrsh[2];
	VP0_DCINEGTHR0.bits.dci_gainneg_thr4  = pstDciPara->s32CbCrGainNeg_Ythrsh[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGTHR0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGTHR0.u32);

	VP0_DCINEGTHR1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGTHR1.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGTHR1.bits.dci_gainneg_thr5  = pstDciPara->s32CbCrGainNeg_Ythrsh[4];
	VP0_DCINEGTHR1.bits.dci_gainneg_thr6  = pstDciPara->s32CbCrGainNeg_Ythrsh[5];
	VP0_DCINEGTHR1.bits.dci_gainneg_thr7  = pstDciPara->s32CbCrGainNeg_Ythrsh[6];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGTHR1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGTHR1.u32);


	VP0_DCINEGGAIN0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGGAIN0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGGAIN1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGGAIN1.u32)+ u32Data * VP_OFFSET));

	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr0 = pstDciPara->s32CbCrGainNeg[0];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr1 = pstDciPara->s32CbCrGainNeg[1];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr2 = pstDciPara->s32CbCrGainNeg[2];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr3 = pstDciPara->s32CbCrGainNeg[3];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr4 = pstDciPara->s32CbCrGainNeg[4];
	VP0_DCINEGGAIN0.bits.dci_gainneg_cbcr5 = pstDciPara->s32CbCrGainNeg[5];
	VP0_DCINEGGAIN1.bits.dci_gainneg_cbcr6 = pstDciPara->s32CbCrGainNeg[6];
	VP0_DCINEGGAIN1.bits.dci_gainneg_cbcr7 = pstDciPara->s32CbCrGainNeg[7];

	VP0_DCINEGGAIN1.bits.dci_gainneg_cbcr8 = pstDciPara->s32CbCrGainNeg[8];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGGAIN0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGGAIN0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGGAIN1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGGAIN1.u32);


	VP0_DCINEGSLP0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGSLP0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET));
	VP0_DCINEGSLP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP2.u32)+ u32Data * VP_OFFSET));

	VP0_DCINEGSLP0.bits.dci_gainneg_slp0 = pstDciPara->s32CbCrGainNeg_Slp[0];
	VP0_DCINEGSLP0.bits.dci_gainneg_slp1 = pstDciPara->s32CbCrGainNeg_Slp[1];
	VP0_DCINEGSLP0.bits.dci_gainneg_slp2 = pstDciPara->s32CbCrGainNeg_Slp[2];


	VP0_DCINEGSLP1.bits.dci_gainneg_slp3 = pstDciPara->s32CbCrGainNeg_Slp[3];
	VP0_DCINEGSLP1.bits.dci_gainneg_slp4 = pstDciPara->s32CbCrGainNeg_Slp[4];
	VP0_DCINEGSLP1.bits.dci_gainneg_slp5 = pstDciPara->s32CbCrGainNeg_Slp[5];

	VP0_DCINEGSLP2.bits.dci_gainneg_slp6 = pstDciPara->s32CbCrGainNeg_Slp[6];
	VP0_DCINEGSLP2.bits.dci_gainneg_slp7 = pstDciPara->s32CbCrGainNeg_Slp[7];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP0.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP1.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCINEGSLP2.u32)+ u32Data * VP_OFFSET), VP0_DCINEGSLP2.u32);

}

HI_VOID VDP_VP_SetDciAdjSW(HI_U32 u32Data, VDP_DCI_PARA_S *pstDciPara )
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
		VDP_PRINT("Error,VDP_VP_SetDciAdjSW() Select Wrong VP ID\n");
		return ;
	}

	VP0_DCIADJSW0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW0.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW1.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW2.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW3.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW4.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW5.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW6.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW7.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW8.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW9.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW9.u32)+ u32Data * VP_OFFSET));
	VP0_DCIADJSW10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW10.u32)+ u32Data * VP_OFFSET));
	
	VP0_DCIADJSW0.bits.dci_adj_sw0 = pstDciPara->ps32AdjLUT_SW[0];
	VP0_DCIADJSW0.bits.dci_adj_sw1 = pstDciPara->ps32AdjLUT_SW[1];
	VP0_DCIADJSW0.bits.dci_adj_sw2 = pstDciPara->ps32AdjLUT_SW[2];
	VP0_DCIADJSW1.bits.dci_adj_sw3 = pstDciPara->ps32AdjLUT_SW[3];
	VP0_DCIADJSW1.bits.dci_adj_sw4 = pstDciPara->ps32AdjLUT_SW[4];
	VP0_DCIADJSW1.bits.dci_adj_sw5 = pstDciPara->ps32AdjLUT_SW[5];
	VP0_DCIADJSW2.bits.dci_adj_sw6 = pstDciPara->ps32AdjLUT_SW[6];
	VP0_DCIADJSW2.bits.dci_adj_sw7 = pstDciPara->ps32AdjLUT_SW[7];
	VP0_DCIADJSW2.bits.dci_adj_sw8 = pstDciPara->ps32AdjLUT_SW[8];
	VP0_DCIADJSW3.bits.dci_adj_sw9 = pstDciPara->ps32AdjLUT_SW[9];
	VP0_DCIADJSW3.bits.dci_adj_sw10 = pstDciPara->ps32AdjLUT_SW[10];
	VP0_DCIADJSW3.bits.dci_adj_sw11 = pstDciPara->ps32AdjLUT_SW[11];
	VP0_DCIADJSW4.bits.dci_adj_sw12 = pstDciPara->ps32AdjLUT_SW[12];
	VP0_DCIADJSW4.bits.dci_adj_sw13 = pstDciPara->ps32AdjLUT_SW[13];
	VP0_DCIADJSW4.bits.dci_adj_sw14 = pstDciPara->ps32AdjLUT_SW[14];
	VP0_DCIADJSW5.bits.dci_adj_sw15 = pstDciPara->ps32AdjLUT_SW[15];
	VP0_DCIADJSW5.bits.dci_adj_sw16 = pstDciPara->ps32AdjLUT_SW[16];
	VP0_DCIADJSW5.bits.dci_adj_sw17 = pstDciPara->ps32AdjLUT_SW[17];
	VP0_DCIADJSW6.bits.dci_adj_sw18 = pstDciPara->ps32AdjLUT_SW[18];
	VP0_DCIADJSW6.bits.dci_adj_sw19 = pstDciPara->ps32AdjLUT_SW[19];
	VP0_DCIADJSW6.bits.dci_adj_sw20 = pstDciPara->ps32AdjLUT_SW[20];
	VP0_DCIADJSW7.bits.dci_adj_sw21 = pstDciPara->ps32AdjLUT_SW[21];
	VP0_DCIADJSW7.bits.dci_adj_sw22 = pstDciPara->ps32AdjLUT_SW[22];
	VP0_DCIADJSW7.bits.dci_adj_sw23 = pstDciPara->ps32AdjLUT_SW[23];
	VP0_DCIADJSW8.bits.dci_adj_sw24 = pstDciPara->ps32AdjLUT_SW[24];
	VP0_DCIADJSW8.bits.dci_adj_sw25 = pstDciPara->ps32AdjLUT_SW[25];
	VP0_DCIADJSW8.bits.dci_adj_sw26 = pstDciPara->ps32AdjLUT_SW[26];
	VP0_DCIADJSW9.bits.dci_adj_sw27 = pstDciPara->ps32AdjLUT_SW[27];
	VP0_DCIADJSW9.bits.dci_adj_sw28 = pstDciPara->ps32AdjLUT_SW[28];
	VP0_DCIADJSW9.bits.dci_adj_sw29 = pstDciPara->ps32AdjLUT_SW[29];
	VP0_DCIADJSW10.bits.dci_adj_sw30 = pstDciPara->ps32AdjLUT_SW[30];
	VP0_DCIADJSW10.bits.dci_adj_sw31 = pstDciPara->ps32AdjLUT_SW[31];
	VP0_DCIADJSW10.bits.dci_adj_sw32 = pstDciPara->ps32AdjLUT_SW[32];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW0.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW1.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW2.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW3.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW4.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW5.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW6.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW7.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW8.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW8.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW9.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW9.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIADJSW10.u32)+ u32Data * VP_OFFSET), VP0_DCIADJSW10.u32);


}

HI_VOID VDP_VP_SetDciDcivsRdEn(HI_U32 u32Data, HI_U32 dcivs_rd_en)
{
	U_VP0_DCI_PARARD VP0_DCI_PARARD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDcivsRdEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARARD.bits.dcivs_rd_en = dcivs_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET),VP0_DCI_PARARD.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDcibsRdEn(HI_U32 u32Data, HI_U32 dcibs_rd_en)
{
	U_VP0_DCI_PARARD VP0_DCI_PARARD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDcibsRdEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARARD.bits.dcibs_rd_en = dcibs_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET),VP0_DCI_PARARD.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDcilutRdEn(HI_U32 u32Data, HI_U32 dcilut_rd_en)
{
	U_VP0_DCI_PARARD VP0_DCI_PARARD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDcilutRdEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARARD.bits.dcilut_rd_en = dcilut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET),VP0_DCI_PARARD.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDcidivRdEn(HI_U32 u32Data, HI_U32 dcidiv_rd_en)
{
	U_VP0_DCI_PARARD VP0_DCI_PARARD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDcidivRdEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARARD.bits.dcidiv_rd_en = dcidiv_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET),VP0_DCI_PARARD.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDcihbwRdEn(HI_U32 u32Data, HI_U32 dcihbw_rd_en)
{
	U_VP0_DCI_PARARD VP0_DCI_PARARD;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDcihbwRdEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARARD.bits.dcihbw_rd_en = dcihbw_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARARD.u32) + u32Data * VP_OFFSET),VP0_DCI_PARARD.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciParaData(HI_U32 u32Data, HI_U32 para_data)
{
	U_VP0_DCI_PARA_DATA VP0_DCI_PARA_DATA;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciParaData Select Wrong Layer ID\n");
	}
	
	VP0_DCI_PARA_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_PARA_DATA.u32) + u32Data * VP_OFFSET));
	VP0_DCI_PARA_DATA.bits.para_data = para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_PARA_DATA.u32) + u32Data * VP_OFFSET),VP0_DCI_PARA_DATA.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciSplitPoint(HI_U32 u32Data, HI_U32 dci_split_point)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciSplitPoint Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_split_point = dci_split_point;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}

HI_VOID VDP_VP_SetDciDciSplitType(HI_U32 u32Data, HI_U32 dci_split_type)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciSplitType Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_split_type = dci_split_type;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciSplitHistMode(HI_U32 u32Data, HI_U32 dci_split_hist_mode)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciSplitHistMode Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_split_hist_mode = dci_split_hist_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciManAlgBlend(HI_U32 u32Data, HI_U32 dci_man_alg_blend)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciManAlgBlend Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_man_alg_blend = dci_man_alg_blend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciManAlgBlendEn(HI_U32 u32Data, HI_U32 dci_man_alg_blend_en)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciManAlgBlendEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_man_alg_blend_en = dci_man_alg_blend_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciFlatThrsh(HI_U32 u32Data, HI_U32 dci_flat_thrsh)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciFlatThrsh Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_flat_thrsh = dci_flat_thrsh;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciFlatMode(HI_U32 u32Data, HI_U32 dci_flat_mode)
{
	U_VP0_DCI_SPLIT VP0_DCI_SPLIT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciFlatMode Select Wrong Layer ID\n");
	}
	
	VP0_DCI_SPLIT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET));
	VP0_DCI_SPLIT.bits.dci_flat_mode = dci_flat_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_SPLIT.u32) + u32Data * VP_OFFSET),VP0_DCI_SPLIT.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciHistMapBlend(HI_U32 u32Data, HI_U32 dci_hist_map_blend)
{
	U_VP0_DCI_HIST_CLIP VP0_DCI_HIST_CLIP;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciHistMapBlend Select Wrong Layer ID\n");
	}
	
	VP0_DCI_HIST_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET));
	VP0_DCI_HIST_CLIP.bits.dci_hist_map_blend = dci_hist_map_blend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET),VP0_DCI_HIST_CLIP.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciClipLimitR(HI_U32 u32Data, HI_U32 dci_clip_limit_r)
{
	U_VP0_DCI_HIST_CLIP VP0_DCI_HIST_CLIP;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciClipLimitR Select Wrong Layer ID\n");
	}
	
	VP0_DCI_HIST_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET));
	VP0_DCI_HIST_CLIP.bits.dci_clip_limit_r = dci_clip_limit_r;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET),VP0_DCI_HIST_CLIP.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciClipLimitL(HI_U32 u32Data, HI_U32 dci_clip_limit_l)
{
	U_VP0_DCI_HIST_CLIP VP0_DCI_HIST_CLIP;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciClipLimitL Select Wrong Layer ID\n");
	}
	
	VP0_DCI_HIST_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET));
	VP0_DCI_HIST_CLIP.bits.dci_clip_limit_l = dci_clip_limit_l;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_HIST_CLIP.u32) + u32Data * VP_OFFSET),VP0_DCI_HIST_CLIP.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsMinBlackThr(HI_U32 u32Data, HI_U32 dci_bs_min_black_thr)
{
	U_VP0_DCI_BLACK0_STRETCH VP0_DCI_BLACK0_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsMinBlackThr Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK0_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK0_STRETCH.bits.dci_bs_min_black_thr = dci_bs_min_black_thr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK0_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsMinBlackBlend(HI_U32 u32Data, HI_U32 dci_bs_min_black_blend)
{
	U_VP0_DCI_BLACK0_STRETCH VP0_DCI_BLACK0_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsMinBlackBlend Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK0_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK0_STRETCH.bits.dci_bs_min_black_blend = dci_bs_min_black_blend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK0_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsMaxLevel(HI_U32 u32Data, HI_U32 dci_bs_max_level)
{
	U_VP0_DCI_BLACK0_STRETCH VP0_DCI_BLACK0_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsMaxLevel Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK0_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK0_STRETCH.bits.dci_bs_max_level = dci_bs_max_level;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK0_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsBlendingLevel(HI_U32 u32Data, HI_U32 dci_bs_blending_level)
{
	U_VP0_DCI_BLACK0_STRETCH VP0_DCI_BLACK0_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsBlendingLevel Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK0_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK0_STRETCH.bits.dci_bs_blending_level = dci_bs_blending_level;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK0_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsEn(HI_U32 u32Data, HI_U32 dci_bs_en)
{
	U_VP0_DCI_BLACK0_STRETCH VP0_DCI_BLACK0_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsEn Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK0_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK0_STRETCH.bits.dci_bs_en = dci_bs_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK0_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK0_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsDetaInv(HI_U32 u32Data, HI_U32 dci_bs_deta_inv)
{
	U_VP0_DCI_BLACK1_STRETCH VP0_DCI_BLACK1_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsDetaInv Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK1_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK1_STRETCH.bits.dci_bs_deta_inv = dci_bs_deta_inv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK1_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsBlackWgtThr(HI_U32 u32Data, HI_U32 dci_bs_black_wgt_thr)
{
	U_VP0_DCI_BLACK1_STRETCH VP0_DCI_BLACK1_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsBlackWgtThr Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK1_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK1_STRETCH.bits.dci_bs_black_wgt_thr = dci_bs_black_wgt_thr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK1_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciBsLOffset(HI_U32 u32Data, HI_U32 dci_bs_l_offset)
{
	U_VP0_DCI_BLACK1_STRETCH VP0_DCI_BLACK1_STRETCH;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciBsLOffset Select Wrong Layer ID\n");
	}
	
	VP0_DCI_BLACK1_STRETCH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET));
	VP0_DCI_BLACK1_STRETCH.bits.dci_bs_l_offset = dci_bs_l_offset;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_BLACK1_STRETCH.u32) + u32Data * VP_OFFSET),VP0_DCI_BLACK1_STRETCH.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciOutHstart(HI_U32 u32Data, HI_U32 dci_out_hstart)
{
	U_VP0_DCI_OUT_HREGION VP0_DCI_OUT_HREGION;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciOutHstart Select Wrong Layer ID\n");
	}
	
	VP0_DCI_OUT_HREGION.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_HREGION.u32) + u32Data * VP_OFFSET));
	VP0_DCI_OUT_HREGION.bits.dci_out_hstart = dci_out_hstart;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_HREGION.u32) + u32Data * VP_OFFSET),VP0_DCI_OUT_HREGION.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciOutHend(HI_U32 u32Data, HI_U32 dci_out_hend)
{
	U_VP0_DCI_OUT_HREGION VP0_DCI_OUT_HREGION;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciOutHend Select Wrong Layer ID\n");
	}
	
	VP0_DCI_OUT_HREGION.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_HREGION.u32) + u32Data * VP_OFFSET));
	VP0_DCI_OUT_HREGION.bits.dci_out_hend = dci_out_hend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_HREGION.u32) + u32Data * VP_OFFSET),VP0_DCI_OUT_HREGION.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciOutVstart(HI_U32 u32Data, HI_U32 dci_out_vstart)
{
	U_VP0_DCI_OUT_VREGION VP0_DCI_OUT_VREGION;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciOutVstart Select Wrong Layer ID\n");
	}
	
	VP0_DCI_OUT_VREGION.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_VREGION.u32) + u32Data * VP_OFFSET));
	VP0_DCI_OUT_VREGION.bits.dci_out_vstart = dci_out_vstart;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_VREGION.u32) + u32Data * VP_OFFSET),VP0_DCI_OUT_VREGION.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciOutVend(HI_U32 u32Data, HI_U32 dci_out_vend)
{
	U_VP0_DCI_OUT_VREGION VP0_DCI_OUT_VREGION;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciOutVend Select Wrong Layer ID\n");
	}
	
	VP0_DCI_OUT_VREGION.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_VREGION.u32) + u32Data * VP_OFFSET));
	VP0_DCI_OUT_VREGION.bits.dci_out_vend = dci_out_vend;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_OUT_VREGION.u32) + u32Data * VP_OFFSET),VP0_DCI_OUT_VREGION.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciMeanValue(HI_U32 u32Data, HI_U32 mean_value)
{
	U_VP0_DCI_STATICS_DATA VP0_DCI_STATICS_DATA;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciMeanValue Select Wrong Layer ID\n");
	}
	
	VP0_DCI_STATICS_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET));
	VP0_DCI_STATICS_DATA.bits.mean_value = mean_value;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET),VP0_DCI_STATICS_DATA.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciMaxBlack(HI_U32 u32Data, HI_U32 max_black)
{
	U_VP0_DCI_STATICS_DATA VP0_DCI_STATICS_DATA;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciMaxBlack Select Wrong Layer ID\n");
	}
	
	VP0_DCI_STATICS_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET));
	VP0_DCI_STATICS_DATA.bits.max_black = max_black;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET),VP0_DCI_STATICS_DATA.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciMinBlack(HI_U32 u32Data, HI_U32 min_black)
{
	U_VP0_DCI_STATICS_DATA VP0_DCI_STATICS_DATA;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciMinBlack Select Wrong Layer ID\n");
	}
	
	VP0_DCI_STATICS_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET));
	VP0_DCI_STATICS_DATA.bits.min_black = min_black;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_STATICS_DATA.u32) + u32Data * VP_OFFSET),VP0_DCI_STATICS_DATA.u32);
	
	return ;
}

HI_VOID VDP_VP_SetDciDciTotalPixNum(HI_U32 u32Data, HI_U32 dci_total_pix_num)
{
	U_VP0_DCI_TOTAL_PIX VP0_DCI_TOTAL_PIX;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciTotalPixNum Select Wrong Layer ID\n");
	}
	
	VP0_DCI_TOTAL_PIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_TOTAL_PIX.u32) + u32Data * VP_OFFSET));
	VP0_DCI_TOTAL_PIX.bits.dci_total_pix_num = dci_total_pix_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_TOTAL_PIX.u32) + u32Data * VP_OFFSET),VP0_DCI_TOTAL_PIX.u32);
	
	return ;
}


HI_VOID VDP_VP_SetDciDciTotalPixNumBs(HI_U32 u32Data, HI_U32 dci_total_pix_num_bs)
{
	U_VP0_DCI_TOTAL_PIX_BS VP0_DCI_TOTAL_PIX_BS;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDciTotalPixNumBs Select Wrong Layer ID\n");
	}
	
	VP0_DCI_TOTAL_PIX_BS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCI_TOTAL_PIX_BS.u32) + u32Data * VP_OFFSET));
	VP0_DCI_TOTAL_PIX_BS.bits.dci_total_pix_num_bs = dci_total_pix_num_bs;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCI_TOTAL_PIX_BS.u32) + u32Data * VP_OFFSET),VP0_DCI_TOTAL_PIX_BS.u32);
	
	return ;
}

HI_VOID VDP_VP_SetDciDbgPos(HI_U32 u32Data, HI_U32 dci_dbg_pos)
{
	U_VP0_DCIHISOFT VP0_DCIHISOFT;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error, VDP_VP_SetDciDbgPos Select Wrong Layer ID\n");
	}
	
	VP0_DCIHISOFT.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32) + u32Data * VP_OFFSET));
#ifdef VDP_HI3798CV200
	VP0_DCIHISOFT.bits.dci_dbg_pos = dci_dbg_pos/2*2;
#else
	VP0_DCIHISOFT.bits.dci_dbg_pos = dci_dbg_pos;
#endif
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_DCIHISOFT.u32) + u32Data * VP_OFFSET),VP0_DCIHISOFT.u32);
	
	return ;
}


//Dci end


//WCG_BEGING
HI_VOID VDP_VP_SetWcgEnable  (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_VPWCG_CSC2CTRL  VPWCG_CSC2CTRL;
	U_VP0_CSC_ODC     VP0_CSC_ODC;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetWcgEnable() Select Wrong VP ID\n");
		return ;
	}
	
	VPWCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2CTRL.bits.csc2_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2CTRL.u32);
	  
	//set csc sign mode
	VP0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_ODC.u32)+ u32Data * VP_OFFSET));
	VP0_CSC_ODC.bits.csc_sign_mode = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CSC_ODC.u32)+ u32Data * VP_OFFSET), VP0_CSC_ODC.u32);

}

HI_VOID VDP_VP_SetWcgCoef  (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef)
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
		VDP_PRINT("Error,VDP_VP_SetWcgCoef() Select Wrong VP ID\n");
		return ;
	}

	VPWCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2CTRL.bits.csc2_scale_sel = pstWcgCoef->csc2_scale_sel;
	VPWCG_CSC2CTRL.bits.csc2_r_r_coef  = (pstWcgCoef->csc2_r_r_coef);
	VPWCG_CSC2CTRL.bits.csc2_r_g_coef  = (pstWcgCoef->csc2_r_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2CTRL.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2CTRL.u32);

	VPWCG_CSC2COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF0.bits.csc2_r_b_coef  = (pstWcgCoef->csc2_r_b_coef);
	VPWCG_CSC2COEF0.bits.csc2_g_r_coef  = (pstWcgCoef->csc2_g_r_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF0.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF0.u32);

	VPWCG_CSC2COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF1.bits.csc2_g_g_coef  = (pstWcgCoef->csc2_g_g_coef);
	VPWCG_CSC2COEF1.bits.csc2_g_b_coef  = (pstWcgCoef->csc2_g_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF1.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF1.u32);

	VPWCG_CSC2COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF2.bits.csc2_b_r_coef  = (pstWcgCoef->csc2_b_r_coef);
	VPWCG_CSC2COEF2.bits.csc2_b_g_coef  = (pstWcgCoef->csc2_b_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF2.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF2.u32);

	VPWCG_CSC2COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF3.u32)+ u32Data * VP_OFFSET));
	VPWCG_CSC2COEF3.bits.csc2_b_b_coef  = (pstWcgCoef->csc2_b_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_CSC2COEF3.u32)+ u32Data * VP_OFFSET), VPWCG_CSC2COEF3.u32);

	//-------------------------

	VPWCG_NON2LCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCTRL.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCTRL.bits.gma_lin_neg_scl  = pstWcgCoef->gma_lin_neg_scl;
	VPWCG_NON2LCTRL.bits.gma_lin_init_slp = pstWcgCoef->gma_lin_init_slp;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_0   = pstWcgCoef->gma_lin_thre_0;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_1   = pstWcgCoef->gma_lin_thre_1;
	VPWCG_NON2LCTRL.bits.gma_lin_thre_2   = pstWcgCoef->gma_lin_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCTRL.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCTRL.u32);

	VPWCG_NON2LCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_a_0  = pstWcgCoef->gma_lin_coef_a_0;
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_b_0  = pstWcgCoef->gma_lin_coef_b_0;
	VPWCG_NON2LCOEF0.bits.gma_lin_coef_c_0  = pstWcgCoef->gma_lin_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF0.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF0.u32);

	VPWCG_NON2LCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_a_1  = pstWcgCoef->gma_lin_coef_a_1;
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_b_1  = pstWcgCoef->gma_lin_coef_b_1;
	VPWCG_NON2LCOEF1.bits.gma_lin_coef_c_1  = pstWcgCoef->gma_lin_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF1.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF1.u32);

	VPWCG_NON2LCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_a_2  = pstWcgCoef->gma_lin_coef_a_2;
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_b_2  = pstWcgCoef->gma_lin_coef_b_2;
	VPWCG_NON2LCOEF2.bits.gma_lin_coef_c_2  = pstWcgCoef->gma_lin_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_NON2LCOEF2.u32)+ u32Data * VP_OFFSET), VPWCG_NON2LCOEF2.u32);

	VPWCG_L2NONTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_L2NONTHD.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONTHD.bits.lin_gma_thre_0  = pstWcgCoef->lin_gma_thre_0;
	VPWCG_L2NONTHD.bits.lin_gma_thre_1  = pstWcgCoef->lin_gma_thre_1;
	VPWCG_L2NONTHD.bits.lin_gma_thre_2  = pstWcgCoef->lin_gma_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_L2NONTHD.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONTHD.u32);

	VPWCG_L2NONCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF0.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_a_0  = pstWcgCoef->lin_gma_coef_a_0;
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_b_0  = pstWcgCoef->lin_gma_coef_b_0;
	VPWCG_L2NONCOEF0.bits.lin_gma_coef_c_0  = pstWcgCoef->lin_gma_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF0.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF0.u32);

	VPWCG_L2NONCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF1.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_a_1  = pstWcgCoef->lin_gma_coef_a_1;
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_b_1  = pstWcgCoef->lin_gma_coef_b_1;
	VPWCG_L2NONCOEF1.bits.lin_gma_coef_c_1  = pstWcgCoef->lin_gma_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF1.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF1.u32);

	VPWCG_L2NONCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF2.u32)+ u32Data * VP_OFFSET));
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_a_2  = pstWcgCoef->lin_gma_coef_a_2;
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_b_2  = pstWcgCoef->lin_gma_coef_b_2;
	VPWCG_L2NONCOEF2.bits.lin_gma_coef_c_2  = pstWcgCoef->lin_gma_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VPWCG_L2NONCOEF2.u32)+ u32Data * VP_OFFSET), VPWCG_L2NONCOEF2.u32);


}

HI_VOID VDP_VP_SetPositionSel  (HI_U32 u32Data, HI_U32 u32Sel)
{
    U_VP0_CTRL  VP0_CTRL;
	
	if(u32Data >= VP_MAX)
	{
		VDP_PRINT("Error,VDP_VP_SetPositionSel() Select Wrong VP ID\n");
		return ;
	}
	
	VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)+ u32Data * VP_OFFSET));
	VP0_CTRL.bits.vp_sel = 0;//u32Sel;//DCI conncet to VP in HiFoneB02
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)+ u32Data * VP_OFFSET), VP0_CTRL.u32);
	  
}

#endif //end VP0_EN


HI_VOID VDP_VID_SetMasterSel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
	U_VOMASTERSEL   VOMASTERSEL;
	U_VOMASTERSEL1  VOMASTERSEL1;

    U_V0_CTRL       V0_CTRL;
    
	VOMASTERSEL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)));
	VOMASTERSEL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)));
	
    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));

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
            VDP_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
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
            VDP_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
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
            VDP_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
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
            VDP_PRINT("Error! VDP_VID_SetMasterSel() Select Wrong layer ID\n");
        }
    }


	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32); 

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);
    
    return ;

}

HI_VOID  VDP_VID_SetFrReqSize    (HI_U32 u32Data)
{
    U_V0_FI_REQW_0 V0_FI_REQW_0;

    V0_FI_REQW_0.u32 = u32Data ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_FI_REQW_0.u32) + VDP_LAYER_VID6 * VID_OFFSET), V0_FI_REQW_0.u32);

    return ;
}

HI_VOID  VDP_VID_SetDsReqSize    (HI_U32 u32Data)
{
    U_V0_FI_REQW_1 V0_FI_REQW_1;

    V0_FI_REQW_1.u32 = u32Data ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_FI_REQW_1.u32) + VDP_LAYER_VID6 * VID_OFFSET), V0_FI_REQW_1.u32);

    return ;
}
HI_VOID  VDP_VID_SetEsEnable (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_V0_CTRL  V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetEsEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
    V0_CTRL.bits.es_en = u32Enable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET), V0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetESMarketMode(HI_U32 u32Data, HI_U32 mk_en, HI_U32 mk_mode)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESMarketMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.mk_en   = mk_en;
    ES_MODE.bits.mk_mode = mk_mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESInterlacedProc(HI_U32 u32Data, HI_U32 interlaced_proc)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESInterlacedProc() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.interlaced_proc = interlaced_proc ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESDirMode(HI_U32 u32Data, HI_U32 dir_mode)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESDirMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.dir_mode = dir_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESCheck1Enable(HI_U32 u32Data, HI_U32 check1_en)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESCheck1Enable() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.check1_en = check1_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESCheck2Enable(HI_U32 u32Data, HI_U32 check2_en)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESCheck2Enable() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.check2_en = check2_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESAlterDirEnable(HI_U32 u32Data, HI_U32 alter_dir_en)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESAlterDirEnable() Select Wrong Video Layer ID\n");
        return ;
    }
    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.alter_dir_en = alter_dir_en ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeStrengthMode(HI_U32 u32Data, HI_U32 edge_strength_mode)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESEdgeStrengthMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.edge_strength_mode = edge_strength_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESBlendMode(HI_U32 u32Data, HI_U32 blend_mode)
{
    U_ES_MODE ES_MODE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESBlendMode() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET));
    ES_MODE.bits.blend_mode = blend_mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_MODE.u32) + u32Data * VID_OFFSET), ES_MODE.u32);
    return;
}

HI_VOID VDP_VID_SetESDirRatio(HI_U32 u32Data, HI_U32 dir_ratio[9])
{
    U_ES_DIR_RATIO_0 ES_DIR_RATIO_0;
    U_ES_DIR_RATIO_1 ES_DIR_RATIO_1;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESDirRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_DIR_RATIO_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_DIR_RATIO_0.u32) + u32Data * VID_OFFSET));
    ES_DIR_RATIO_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_DIR_RATIO_1.u32) + u32Data * VID_OFFSET));
    ES_DIR_RATIO_0.bits.dir_ratio_0 = dir_ratio[0] ;
    ES_DIR_RATIO_0.bits.dir_ratio_1 = dir_ratio[1] ;
    ES_DIR_RATIO_0.bits.dir_ratio_2 = dir_ratio[2] ;
    ES_DIR_RATIO_0.bits.dir_ratio_3 = dir_ratio[3] ;
    ES_DIR_RATIO_0.bits.dir_ratio_4 = dir_ratio[4] ;
    ES_DIR_RATIO_0.bits.dir_ratio_5 = dir_ratio[5] ;
    ES_DIR_RATIO_0.bits.dir_ratio_6 = dir_ratio[6] ;
    ES_DIR_RATIO_0.bits.dir_ratio_7 = dir_ratio[7] ;
    ES_DIR_RATIO_1.bits.dir_ratio_8 = dir_ratio[8] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_DIR_RATIO_0.u32) + u32Data * VID_OFFSET), ES_DIR_RATIO_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_DIR_RATIO_1.u32) + u32Data * VID_OFFSET), ES_DIR_RATIO_1.u32);
    return;
}

HI_VOID VDP_VID_SetESNormalizedScale(HI_U32 u32Data, HI_U32 normalized_scale[9])
{
    U_ES_NORMALIZED_SCALE_0 ES_NORMALIZED_SCALE_0;
    U_ES_NORMALIZED_SCALE_1 ES_NORMALIZED_SCALE_1;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESNormalizedScale() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_NORMALIZED_SCALE_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_NORMALIZED_SCALE_0.u32) + u32Data * VID_OFFSET));
    ES_NORMALIZED_SCALE_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_NORMALIZED_SCALE_1.u32) + u32Data * VID_OFFSET));
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_0 = normalized_scale[0] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_1 = normalized_scale[1] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_2 = normalized_scale[2] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_3 = normalized_scale[3] ;
    ES_NORMALIZED_SCALE_0.bits.normalized_scale_4 = normalized_scale[4] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_5 = normalized_scale[5] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_6 = normalized_scale[6] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_7 = normalized_scale[7] ;
    ES_NORMALIZED_SCALE_1.bits.normalized_scale_8 = normalized_scale[8] ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_NORMALIZED_SCALE_0.u32) + u32Data * VID_OFFSET), ES_NORMALIZED_SCALE_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_NORMALIZED_SCALE_1.u32) + u32Data * VID_OFFSET), ES_NORMALIZED_SCALE_1.u32);
    return;
}

HI_VOID VDP_VID_SetESBC1CFG(HI_U32 u32Data, HI_U32 check1_deadzone, HI_U32 check1_autodz_gain, HI_U32 check1_gain, HI_U32 chech1_dir_diff_gain)
{
    U_ES_BC1_CFG ES_BC1_CFG;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESBC1CFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC1_CFG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_BC1_CFG.u32) + u32Data * VID_OFFSET));
    //ES_BC1_CFG.bits.check1_deadzone      = check1_deadzone <<2 ;//z221213
    ES_BC1_CFG.bits.check1_deadzone      = check1_deadzone ;
    ES_BC1_CFG.bits.check1_autodz_gain   = check1_autodz_gain   ;
    ES_BC1_CFG.bits.check1_gain          = check1_gain          ;
    ES_BC1_CFG.bits.chech1_dir_diff_gain = chech1_dir_diff_gain ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_BC1_CFG.u32) + u32Data * VID_OFFSET), ES_BC1_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESBC2CFG(HI_U32 u32Data, HI_U32 check2_deadzone, HI_U32 check2_autodz_gain, HI_U32 check2_gain)
{
    U_ES_BC2_CFG ES_BC2_CFG;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESBC2CFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC2_CFG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_BC2_CFG.u32) + u32Data * VID_OFFSET));
    //ES_BC2_CFG.bits.check2_deadzone      = check2_deadzone <<2 ;//z221213
    ES_BC2_CFG.bits.check2_deadzone      = check2_deadzone  ;
    ES_BC2_CFG.bits.check2_autodz_gain   = check2_autodz_gain   ;
    ES_BC2_CFG.bits.check2_gain          = check2_gain          ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_BC2_CFG.u32) + u32Data * VID_OFFSET), ES_BC2_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESBCCFG(HI_U32 u32Data, HI_U32 check_gain)
{
    U_ES_BC_CFG ES_BC_CFG;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESBCCFG() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_BC_CFG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_BC_CFG.u32) + u32Data * VID_OFFSET));
    ES_BC_CFG.bits.check_gain   = check_gain    ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_BC_CFG.u32) + u32Data * VID_OFFSET), ES_BC_CFG.u32);
    return;
}

HI_VOID VDP_VID_SetESRatioLimitScale(HI_U32 u32Data, HI_U32 ratio_limit_scale)
{
    U_ES_RATIO_LIMIT_SCALE ES_RATIO_LIMIT_SCALE;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESRatioLimitScale() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_RATIO_LIMIT_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_RATIO_LIMIT_SCALE.u32) + u32Data * VID_OFFSET));
    ES_RATIO_LIMIT_SCALE.bits.ratio_limit_scale   = ratio_limit_scale  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_RATIO_LIMIT_SCALE.u32) + u32Data * VID_OFFSET), ES_RATIO_LIMIT_SCALE.u32);
    return;
}

HI_VOID VDP_VID_SetESVerMinStrength(HI_U32 u32Data, HI_S32 ver_min_strength)
{
    U_ES_VER_MIN_STRENGTH ES_VER_MIN_STRENGTH;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESVerMinStrength() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_VER_MIN_STRENGTH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_VER_MIN_STRENGTH.u32) + u32Data * VID_OFFSET));
    //ES_VER_MIN_STRENGTH.bits.ver_min_strength   = ver_min_strength <<2 ;//z221213
    ES_VER_MIN_STRENGTH.bits.ver_min_strength   = ver_min_strength ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_VER_MIN_STRENGTH.u32) + u32Data * VID_OFFSET), ES_VER_MIN_STRENGTH.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSmoothGain(HI_U32 u32Data, HI_U32 edge_smooth_gain)
{
    U_ES_EDGE_SMOOTH_GAIN ES_EDGE_SMOOTH_GAIN;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESEdgeSmoothGain() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SMOOTH_GAIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_GAIN.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_GAIN.bits.edge_smooth_gain   = edge_smooth_gain  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_GAIN.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_GAIN.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeThd(HI_U32 u32Data, HI_U32 edge_thd[4])
{
    U_ES_EDGE_THD ES_EDGE_THD;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESEdgeThd() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_THD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_THD.u32) + u32Data * VID_OFFSET));
    //ES_EDGE_THD.bits.edge_thd_0   = edge_thd[0] <<2  ; //z221213
    //ES_EDGE_THD.bits.edge_thd_1   = edge_thd[1] <<2  ;
    //ES_EDGE_THD.bits.edge_thd_2   = edge_thd[2] <<2  ;
    //ES_EDGE_THD.bits.edge_thd_3   = edge_thd[3] <<2  ;
    ES_EDGE_THD.bits.edge_thd_0   = edge_thd[0]  ;
    ES_EDGE_THD.bits.edge_thd_1   = edge_thd[1]  ;
    ES_EDGE_THD.bits.edge_thd_2   = edge_thd[2]  ;
    ES_EDGE_THD.bits.edge_thd_3   = edge_thd[3]  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_THD.u32) + u32Data * VID_OFFSET), ES_EDGE_THD.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSmoothRatio(HI_U32 u32Data, HI_U32 edge_smooth_ratio[5])
{
    U_ES_EDGE_SMOOTH_RATIO_0 ES_EDGE_SMOOTH_RATIO_0;
    U_ES_EDGE_SMOOTH_RATIO_1 ES_EDGE_SMOOTH_RATIO_1;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESEdgeSmoothRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SMOOTH_RATIO_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_RATIO_0.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_RATIO_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_RATIO_1.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_0   = edge_smooth_ratio[0]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_1   = edge_smooth_ratio[1]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_2   = edge_smooth_ratio[2]  ;
    ES_EDGE_SMOOTH_RATIO_0.bits.edge_smooth_ratio_3   = edge_smooth_ratio[3]  ;
    ES_EDGE_SMOOTH_RATIO_1.bits.edge_smooth_ratio_4   = edge_smooth_ratio[4]  ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_RATIO_0.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_RATIO_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SMOOTH_RATIO_1.u32) + u32Data * VID_OFFSET), ES_EDGE_SMOOTH_RATIO_1.u32);
    return;
}

HI_VOID VDP_VID_SetESEdgeSlope(HI_U32 u32Data, HI_S32 edge_slope[5])
{
    U_ES_EDGE_SLOPE_0 ES_EDGE_SLOPE_0;
    U_ES_EDGE_SLOPE_1 ES_EDGE_SLOPE_1;
    U_ES_EDGE_SLOPE_2 ES_EDGE_SLOPE_2;
    
    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetESEdgeSlope() Select Wrong Video Layer ID\n");
        return ;
    }

    ES_EDGE_SLOPE_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_0.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SLOPE_1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_1.u32) + u32Data * VID_OFFSET));
    ES_EDGE_SLOPE_2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_2.u32) + u32Data * VID_OFFSET));
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

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_0.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_0.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_1.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_1.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->ES_EDGE_SLOPE_2.u32) + u32Data * VID_OFFSET), ES_EDGE_SLOPE_2.u32);
    return;
}
//-----------------------------------------------------------------------------------
//SHARPEN_BEGIN  
//-----------------------------------------------------------------------------------
#if VID_SHARP_EN
HI_VOID VDP_VID_SetSharpDbgEn(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPCTRL SPCTRL;

    if(u32LayerId >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPCTRL.bits.debug_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32); 
}


HI_VOID VDP_VID_SetSharpDemoMode(HI_U32 u32LayerId, HI_U32 DemoMode)
{
    U_SPCTRL SPCTRL;

    if(u32LayerId >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetLayerAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPCTRL.bits.demo_mode = DemoMode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32); 
}

HI_VOID VDP_VID_SetSharpEnable(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPCTRL.bits.sharpen_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32); 
}

HI_VOID VDP_VID_SetSharpDemoPos( HI_U32 u32LayerId, HI_U32 u32DemoPos)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
#ifdef VDP_HI3798CV200
    SPCTRL.bits.demo_pos = u32DemoPos/2*2;
#else
    SPCTRL.bits.demo_pos = u32DemoPos;
#endif
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32);
}

//-------------------------------------------------------------
//HI_VOID VDP_VID_SetSharpDmoPos( HI_U32 u32LayerId, HI_U32 u32DbgPos)
//{
//    U_SPCTRL SPCTRL;
//    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
//    SPCTRL.bits.dbg_pos = u32DbgPos;
//    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32);
//}

HI_VOID VDP_VID_SetSharpEnsctrl( HI_U32 u32LayerId, HI_U32 u32Ensctrl)
{
    U_SPCTRL SPCTRL;
    SPCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPCTRL.bits.ensctrl = u32Ensctrl;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCTRL.u32) + u32LayerId * SHARPEN_OFFSET), SPCTRL.u32);
}
//------------------------------------------------------------

HI_VOID VDP_VID_SetSharpDetecEnable(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPENABLE SPENABLE;
    SPENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET));
    SPENABLE.bits.detec_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET), SPENABLE.u32); 
}
HI_VOID VDP_VID_SetSharpPeakEnable(HI_U32 u32LayerId, HI_U32 bEnable)
{
    U_SPENABLE SPENABLE;
    SPENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET));
    SPENABLE.bits.peak_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET), SPENABLE.u32); 
}

HI_VOID VDP_VID_SetSharpGain    ( HI_U32 u32LayerId, HI_U32 u32Gain1,HI_U32 u32Gain2,HI_U32 u32Gain3)
{
    U_SPGAIN SPGAIN;
    SPGAIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAIN.u32) + u32LayerId * SHARPEN_OFFSET));
    SPGAIN.bits.dir_gain  = u32Gain1;
    SPGAIN.bits.edge_gain = u32Gain2;
    SPGAIN.bits.peak_gain = u32Gain3;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAIN.u32) + u32LayerId * SHARPEN_OFFSET), SPGAIN.u32); 

}

HI_VOID VDP_VID_SetSharpPeakCoring(HI_U32 u32LayerId, HI_U32 u32Data)
{
    U_SPCORING SPCORING;
    SPCORING.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORING.u32) + u32LayerId * SHARPEN_OFFSET));
    SPCORING.bits.peak_coring = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORING.u32) + u32LayerId * SHARPEN_OFFSET), SPCORING.u32); 
}


HI_VOID VDP_VID_SetSharpPeakShootCtrl( HI_U32 u32LayerId, HI_U32 u32Over, HI_U32 u32Under,HI_U32 u32Mix)
{
    U_SPPEAKSHOOTCTL SPPEAKSHOOTCTL;
    SPPEAKSHOOTCTL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPPEAKSHOOTCTL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPPEAKSHOOTCTL.bits.peak_mixratio   = u32Mix;
    SPPEAKSHOOTCTL.bits.peak_under_thr  = u32Under;
    SPPEAKSHOOTCTL.bits.peak_over_thr   = u32Over;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPPEAKSHOOTCTL.u32) + u32LayerId * SHARPEN_OFFSET), SPPEAKSHOOTCTL.u32); 
}

HI_VOID VDP_VID_SetSharpEdgeShootCtrl( HI_U32 u32LayerId, HI_U32 u32Over, HI_U32 u32Under,HI_U32 u32Mix)
{
    U_SPEDGESHOOTCTL SPEDGESHOOTCTL;
    SPEDGESHOOTCTL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPEDGESHOOTCTL.u32) + u32LayerId * SHARPEN_OFFSET));
    SPEDGESHOOTCTL.bits.edge_mixratio   = u32Mix;
    SPEDGESHOOTCTL.bits.edge_under_thr  = u32Under;
    SPEDGESHOOTCTL.bits.edge_over_thr   = u32Over;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPEDGESHOOTCTL.u32) + u32LayerId * SHARPEN_OFFSET), SPEDGESHOOTCTL.u32); 
}
HI_VOID VDP_VID_SetSharpTmpH1( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPTMPH1 SPTMPH1;
    SPTMPH1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPTMPH1.u32) + u32LayerId * SHARPEN_OFFSET));
    SPTMPH1.bits.tmp0_h1  = u32Tmp0;
    SPTMPH1.bits.tmp1_h1  = u32Tmp1;
    SPTMPH1.bits.tmp2_h1  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPTMPH1.u32) + u32LayerId * SHARPEN_OFFSET), SPTMPH1.u32); 
}
HI_VOID VDP_VID_SetSharpTmpH2( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPTMPH2 SPTMPH2;
    SPTMPH2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPTMPH2.u32) + u32LayerId * SHARPEN_OFFSET));
    SPTMPH2.bits.tmp0_h2  = u32Tmp0;
    SPTMPH2.bits.tmp1_h2  = u32Tmp1;
    SPTMPH2.bits.tmp2_h2  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPTMPH2.u32) + u32LayerId * SHARPEN_OFFSET), SPTMPH2.u32); 
}
HI_VOID VDP_VID_SetSharpTmpV1( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPTMPV1 SPTMPV1;
    SPTMPV1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPTMPV1.u32) + u32LayerId * SHARPEN_OFFSET));
    SPTMPV1.bits.tmp0_v1  = u32Tmp0;
    SPTMPV1.bits.tmp1_v1  = u32Tmp1;
    SPTMPV1.bits.tmp2_v1  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPTMPV1.u32) + u32LayerId * SHARPEN_OFFSET), SPTMPV1.u32); 
}
HI_VOID VDP_VID_SetSharpTmpV2( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPTMPV2 SPTMPV2;
    SPTMPV2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPTMPV2.u32) + u32LayerId * SHARPEN_OFFSET));
    SPTMPV2.bits.tmp0_v2  = u32Tmp0;
    SPTMPV2.bits.tmp1_v2  = u32Tmp1;
    SPTMPV2.bits.tmp2_v2  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPTMPV2.u32) + u32LayerId * SHARPEN_OFFSET), SPTMPV2.u32); 
}

HI_VOID VDP_VID_SetSharpEdgeTmpH( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPEDGETMPH SPEDGETMPH;
    SPEDGETMPH.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPEDGETMPH.u32) + u32LayerId * SHARPEN_OFFSET));
    SPEDGETMPH.bits.tmp0_sx  = u32Tmp0;
    SPEDGETMPH.bits.tmp1_sx  = u32Tmp1;
    SPEDGETMPH.bits.tmp2_sx  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPEDGETMPH.u32) + u32LayerId * SHARPEN_OFFSET), SPEDGETMPH.u32); 
}
HI_VOID VDP_VID_SetSharpEdgeTmpV( HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)
{
    U_SPEDGETMPV SPEDGETMPV;
    SPEDGETMPV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPEDGETMPV.u32) + u32LayerId * SHARPEN_OFFSET));
    SPEDGETMPV.bits.tmp0_sy  = u32Tmp0;
    SPEDGETMPV.bits.tmp1_sy  = u32Tmp1;
    SPEDGETMPV.bits.tmp2_sy  = u32Tmp2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPEDGETMPV.u32) + u32LayerId * SHARPEN_OFFSET), SPEDGETMPV.u32); 
}

HI_VOID VDP_VID_SetSharpEdgeEi( HI_U32 u32LayerId, HI_U32 u32EiRatio, HI_U32 u32Ei1,HI_U32 u32Ei0)
{
    U_SPEDGEEI SPEDGEEI;
    SPEDGEEI.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPEDGEEI.u32) + u32LayerId * SHARPEN_OFFSET));
    SPEDGEEI.bits.edge_eiratio = u32EiRatio;
    SPEDGEEI.bits.edge_ei1     = u32Ei1;
    SPEDGEEI.bits.edge_ei0     = u32Ei0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPEDGEEI.u32) + u32LayerId * SHARPEN_OFFSET), SPEDGEEI.u32); 
}

//---------------------------------------------------------------------------------------------------
HI_VOID VDP_VID_SetSharpGradMin( HI_U32 u32LayerId, HI_U32 u32GradMinSlop, HI_U32 u32GradMinThrh,HI_U32 u32GradMinThrl)
{
    U_SPGRADMIN SPGRADMIN;
    SPGRADMIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGRADMIN.u32) + u32LayerId * SHARPEN_OFFSET));
    SPGRADMIN.bits.gradminslop     = u32GradMinSlop;
    SPGRADMIN.bits.gradminthrh     = u32GradMinThrh;
    SPGRADMIN.bits.gradminthrl     = u32GradMinThrl;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGRADMIN.u32) + u32LayerId * SHARPEN_OFFSET), SPGRADMIN.u32); 
}

HI_VOID VDP_VID_SetSharpGradCtrl( HI_U32 u32LayerId, HI_U32 u32ChessBoardThr, HI_U32 u32GradMinThr)
{
    U_SPGRADCTR SPGRADCTR;
    SPGRADCTR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGRADCTR.u32) + u32LayerId * SHARPEN_OFFSET));
    SPGRADCTR.bits.chessboardthr   = u32ChessBoardThr;
    SPGRADCTR.bits.gradminthr      = u32GradMinThr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGRADCTR.u32) + u32LayerId * SHARPEN_OFFSET), SPGRADCTR.u32); 
}

HI_VOID VDP_VID_SetSharpShootRatio( HI_U32 u32LayerId, HI_U32 u32OshootRatiot, HI_U32 u32OshootRatioe,HI_U32 u32UshootRatiot,HI_U32 u32UshootRatioe)
{
    U_SPSHOOTRATIO SPSHOOTRATIO;
    SPSHOOTRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPSHOOTRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
    SPSHOOTRATIO.bits.oshootratiot   = u32OshootRatiot;
    SPSHOOTRATIO.bits.oshootratioe   = u32OshootRatioe;
    SPSHOOTRATIO.bits.ushootratiot   = u32UshootRatiot;
    SPSHOOTRATIO.bits.ushootratioe   = u32UshootRatioe;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPSHOOTRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPSHOOTRATIO.u32); 
}

HI_VOID VDP_VID_SetSharpGradContrast( HI_U32 u32LayerId, HI_U32 u32GradContrastThr, HI_U32 u32GradContrastSlop)
{
    U_SPGRADCONTRAST SPGRADCONTRAST;
    SPGRADCONTRAST.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGRADCONTRAST.u32) + u32LayerId * SHARPEN_OFFSET));
    SPGRADCONTRAST.bits.gradcontrastthr = u32GradContrastThr;
    SPGRADCONTRAST.bits.gradcontrastslop= u32GradContrastSlop;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGRADCONTRAST.u32) + u32LayerId * SHARPEN_OFFSET), SPGRADCONTRAST.u32); 
}


//--------------------------------------------------------------------------------------------------

HI_VOID VDP_VID_SetSharpHighFreq(HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, VDP_SHARP_GROUP_S * stCfg)
{
    U_SPENABLE           SPENABLE      ; 
    U_SPWINSIZEHVD       SPWINSIZEHVD  ; 
    U_SPGAINPOS          SPGAINPOS     ; 
    U_SPGAINNEG          SPGAINNEG     ; 
    U_SPOVERTHR          SPOVERTHR     ; 
    U_SPUNDERTHR         SPUNDERTHR    ; 
    U_SPMIXRATIO         SPMIXRATIO    ; 
    
    U_SPCORINGZERO       SPCORINGZERO  ; 
    U_SPCORINGRATIO      SPCORINGRATIO ; 
    U_SPCORINGTHR        SPCORINGTHR   ; 
    
    U_SPLMTRATIO         SPLMTRATIO    ; 
    U_SPLMTPOS0          SPLMTPOS0     ; 
    U_SPLMTPOS1          SPLMTPOS1     ; 
    U_SPLMTNEG0          SPLMTNEG0     ; 
    U_SPLMTNEG1          SPLMTNEG1     ; 
    U_SPBOUNDPOS         SPBOUNDPOS    ; 
    U_SPBOUNDNEG         SPBOUNDNEG    ; 
   
    switch(enDir)
    {
        case VDP_SHARP_DIR_H:
        {
            //bEnable
           SPENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET));
           SPENABLE.bits.hfreq_en_h = stCfg->stSharpU1.bEnable;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET), SPENABLE.u32); 
          
           //u8WinSize  SPWINSIZEHVD
           SPWINSIZEHVD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET));
           SPWINSIZEHVD.bits.winsize_h = stCfg->stSharpU1.u8WinSize;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET), SPWINSIZEHVD.u32); 
           //s16GainPos 
           SPGAINPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINPOS.bits.gain_pos_h = stCfg->stSharpU1.s16GainPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINPOS.u32); 

           //s16GainNeg 
           SPGAINNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINNEG.bits.gain_neg_h = stCfg->stSharpU1.s16GainNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINNEG.u32); 

           //u8OverThrsh 
           SPOVERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPOVERTHR.bits.over_thr_h = stCfg->stSharpU1.u8OverThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPOVERTHR.u32); 

           //u8UnderThrsh 
           SPUNDERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPUNDERTHR.bits.under_thr_h = stCfg->stSharpU1.u8UnderThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPUNDERTHR.u32); 

           //u8MixRatio
           SPMIXRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPMIXRATIO.bits.mixratio_h = stCfg->stSharpU1.u8MixRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPMIXRATIO.u32); 

           //-------------------------------------------- SPCORINGHTHR
           SPCORINGTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGTHR.bits.coringthr_h = stCfg->stSharpU1.u8Coringthr;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGTHR.u32); 
           //-------------------------------------------- SPCORINGHZERO
           SPCORINGZERO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGZERO.bits.coringzero_h = stCfg->stSharpU1.u8Coringzero;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGZERO.u32); 
           //-------------------------------------------- SPCORINGHRATIO
           SPCORINGRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGRATIO.bits.coringratio_h = stCfg->stSharpU1.u8Coringratio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGRATIO.u32); 
           
   
           SPLMTRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTRATIO.bits.limit_ratio_h = stCfg->stSharpU1.u16LimitRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTRATIO.u32); 
           
           SPLMTPOS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS0.bits.limitpos0_h = stCfg->stSharpU1.u16LimitPos0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS0.u32); 
           
           SPLMTPOS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS1.bits.limitpos1_h = stCfg->stSharpU1.u16LimitPos1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS1.u32); 
    
           SPLMTNEG0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG0.bits.limitneg0_h = stCfg->stSharpU1.s16LimitNeg0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG0.u32); 
           
           SPLMTNEG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG1.bits.limitneg1_h = stCfg->stSharpU1.s16LimitNeg1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG1.u32); 
    
           SPBOUNDPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDPOS.bits.boundpos_h = stCfg->stSharpU1.u16BoundPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDPOS.u32); 
    
           SPBOUNDNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDNEG.bits.boundneg_h = stCfg->stSharpU1.s16BoundNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDNEG.u32); 
   
           break;
       }
       case VDP_SHARP_DIR_V:
       {
            //bEnable
           SPENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET));
           SPENABLE.bits.hfreq_en_v = stCfg->stSharpU1.bEnable;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET), SPENABLE.u32); 
          
           //u8WinSize  SPWINSIZEHVD
           SPWINSIZEHVD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET));
           SPWINSIZEHVD.bits.winsize_v = stCfg->stSharpU1.u8WinSize;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET), SPWINSIZEHVD.u32); 
           //s16GainPos 
           SPGAINPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINPOS.bits.gain_pos_v = stCfg->stSharpU1.s16GainPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINPOS.u32); 

           //s16GainNeg 
           SPGAINNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINNEG.bits.gain_neg_v = stCfg->stSharpU1.s16GainNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINNEG.u32); 

           //u8OverThrsh 
           SPOVERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPOVERTHR.bits.over_thr_v = stCfg->stSharpU1.u8OverThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPOVERTHR.u32); 

           //u8UnderThrsh 
           SPUNDERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPUNDERTHR.bits.under_thr_v = stCfg->stSharpU1.u8UnderThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPUNDERTHR.u32); 

           //u8MixRatio
           SPMIXRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPMIXRATIO.bits.mixratio_v = stCfg->stSharpU1.u8MixRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPMIXRATIO.u32); 

           //-------------------------------------------- SPCORINGHTHR
           SPCORINGTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGTHR.bits.coringthr_v = stCfg->stSharpU1.u8Coringthr;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGTHR.u32); 
           //-------------------------------------------- SPCORINGHZERO
           SPCORINGZERO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGZERO.bits.coringzero_v = stCfg->stSharpU1.u8Coringzero;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGZERO.u32); 
           //-------------------------------------------- SPCORINGHRATIO
           SPCORINGRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGRATIO.bits.coringratio_v = stCfg->stSharpU1.u8Coringratio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGRATIO.u32); 
           
   
           SPLMTRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTRATIO.bits.limit_ratio_v = stCfg->stSharpU1.u16LimitRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTRATIO.u32); 
           
           SPLMTPOS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS0.bits.limitpos0_v = stCfg->stSharpU1.u16LimitPos0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS0.u32); 
           
           SPLMTPOS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS1.bits.limitpos1_v = stCfg->stSharpU1.u16LimitPos1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS1.u32); 
    
           SPLMTNEG0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG0.bits.limitneg0_v = stCfg->stSharpU1.s16LimitNeg0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG0.u32); 
           
           SPLMTNEG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG1.bits.limitneg1_v = stCfg->stSharpU1.s16LimitNeg1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG1.u32); 
    
           SPBOUNDPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDPOS.bits.boundpos_v = stCfg->stSharpU1.u16BoundPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDPOS.u32); 
    
           SPBOUNDNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDNEG.bits.boundneg_v = stCfg->stSharpU1.s16BoundNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDNEG.u32); 

   
           break;
       }
       case VDP_SHARP_DIR_D:
       {
            //bEnable
           SPENABLE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET));
           SPENABLE.bits.hfreq_en_d = stCfg->stSharpU1.bEnable;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32LayerId * SHARPEN_OFFSET), SPENABLE.u32); 
          
           //u8WinSize  SPWINSIZEHVD
           SPWINSIZEHVD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET));
           SPWINSIZEHVD.bits.winsize_d = stCfg->stSharpU1.u8WinSize;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPWINSIZEHVD.u32) + u32LayerId * SHARPEN_OFFSET), SPWINSIZEHVD.u32); 
           //s16GainPos 
           SPGAINPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINPOS.bits.gain_pos_d = stCfg->stSharpU1.s16GainPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINPOS.u32); 

           //s16GainNeg 
           SPGAINNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPGAINNEG.bits.gain_neg_d = stCfg->stSharpU1.s16GainNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPGAINNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPGAINNEG.u32); 

           //u8OverThrsh 
           SPOVERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPOVERTHR.bits.over_thr_d = stCfg->stSharpU1.u8OverThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPOVERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPOVERTHR.u32); 

           //u8UnderThrsh 
           SPUNDERTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPUNDERTHR.bits.under_thr_d = stCfg->stSharpU1.u8UnderThrsh;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPUNDERTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPUNDERTHR.u32); 

           //u8MixRatio
           SPMIXRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPMIXRATIO.bits.mixratio_d = stCfg->stSharpU1.u8MixRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPMIXRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPMIXRATIO.u32); 

           //-------------------------------------------- SPCORINGHTHR
           SPCORINGTHR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGTHR.bits.coringthr_d = stCfg->stSharpU1.u8Coringthr;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGTHR.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGTHR.u32); 
           //-------------------------------------------- SPCORINGHZERO
           SPCORINGZERO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGZERO.bits.coringzero_d = stCfg->stSharpU1.u8Coringzero;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGZERO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGZERO.u32); 
           //-------------------------------------------- SPCORINGHRATIO
           SPCORINGRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPCORINGRATIO.bits.coringratio_d = stCfg->stSharpU1.u8Coringratio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPCORINGRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPCORINGRATIO.u32); 
           
   
           SPLMTRATIO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTRATIO.bits.limit_ratio_d = stCfg->stSharpU1.u16LimitRatio;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTRATIO.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTRATIO.u32); 
           
           SPLMTPOS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS0.bits.limitpos0_d = stCfg->stSharpU1.u16LimitPos0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS0.u32); 
           
           SPLMTPOS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTPOS1.bits.limitpos1_d = stCfg->stSharpU1.u16LimitPos1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTPOS1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTPOS1.u32); 
    
           SPLMTNEG0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG0.bits.limitneg0_d = stCfg->stSharpU1.s16LimitNeg0;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG0.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG0.u32); 
           
           SPLMTNEG1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET));
           SPLMTNEG1.bits.limitneg1_d = stCfg->stSharpU1.s16LimitNeg1;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPLMTNEG1.u32) + u32LayerId * SHARPEN_OFFSET), SPLMTNEG1.u32); 
    
           SPBOUNDPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDPOS.bits.boundpos_d = stCfg->stSharpU1.u16BoundPos;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDPOS.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDPOS.u32); 
    
           SPBOUNDNEG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET));
           SPBOUNDNEG.bits.boundneg_d = stCfg->stSharpU1.s16BoundNeg;
           VDP_RegWrite(((HI_ULONG)&(pVdpReg->SPBOUNDNEG.u32) + u32LayerId * SHARPEN_OFFSET), SPBOUNDNEG.u32); 
   
           break;
       }
       default:
       {
           VDP_PRINT("Error,VDP_SetSharpHighFreq() select Wrong direction\n");
           return ;
       }
    }
   
}
#endif

HI_VOID VDP_VP_SetDataMask    (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even )
{
    U_VP0_CTRL VP0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error, VDP_VID_SetDataMask() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_CTRL.bits.odd_data_mute  = u32Odd ;
    VP0_CTRL.bits.even_data_mute = u32Even ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET), VP0_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetDcmpCtrl   (HI_U32 u32Data, HI_U32 u32LumLosslessEn, HI_U32 u32ChmLosslessEn )
{
    U_V0_DCMP_CTRL V0_DCMP_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error, VDP_V0_SetDcmpCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_DCMP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET));
    V0_DCMP_CTRL.bits.is_lossless_l = u32LumLosslessEn ;
    V0_DCMP_CTRL.bits.is_lossless_c = u32ChmLosslessEn ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET), V0_DCMP_CTRL.u32);

    return ;
}

HI_VOID VDP_VID_SetSrcCropReso    (HI_U32 u32Data, VDP_RECT_S stRect)
{
    U_V0_TILE_LOFFSET V0_TILE_LOFFSET;
    U_V0_TILE_COFFSET V0_TILE_COFFSET;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetSrcCropReso() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_TILE_LOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TILE_LOFFSET.u32) + u32Data * VID_OFFSET));
    V0_TILE_LOFFSET.bits.l_x_offset = stRect.u32X;
    V0_TILE_LOFFSET.bits.l_y_offset = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TILE_LOFFSET.u32) + u32Data * VID_OFFSET), V0_TILE_LOFFSET.u32); 

    V0_TILE_COFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TILE_COFFSET.u32) + u32Data * VID_OFFSET));
    V0_TILE_COFFSET.bits.c_x_offset = stRect.u32X;
    V0_TILE_COFFSET.bits.c_y_offset = stRect.u32Y/2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TILE_COFFSET.u32) + u32Data * VID_OFFSET), V0_TILE_COFFSET.u32); 
    return ;
}

HI_VOID  VDP_VID_SetTnrDcmpErrorClr    (HI_U32 u32Data,HI_U32 u32dcmp_lerror_clr,HI_U32  u32dcmp_cerror_clr)
{
	U_V0_TNR_DCMPERRCLR  V0_TNR_DCMPERRCLR;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetDcmpErrorClr() Select Wrong Video Layer ID\n");
        return ;
    }

    V0_TNR_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_TNR_DCMPERRCLR.bits.dcmp_l_errclr= u32dcmp_lerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_TNR_DCMPERRCLR.u32);

    V0_TNR_DCMPERRCLR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TNR_DCMPERRCLR.u32) + u32Data * VID_OFFSET));
    V0_TNR_DCMPERRCLR.bits.dcmp_c_errclr= u32dcmp_cerror_clr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_TNR_DCMPERRCLR.u32) + u32Data * VID_OFFSET), V0_TNR_DCMPERRCLR.u32);

    return ;
}


HI_S32 VDP_VID_GetVidLayerEnable (HI_U32 u32Data)
{
    U_V0_CTRL V0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error,VDP_VID_SetInDataFmt() Select Wrong Video Layer ID\n");
        return HI_FAILURE;
    }

    V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));

    return (V0_CTRL.u32 & 0x80000000);
}


#endif


