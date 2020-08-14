//**********************************************************************
//
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd
// All rights reserved.
//
// File Name   : vdp_drv_wbc.cpp
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
#include "vdp_drv_wbc.h"

#if EDA_TEST
extern Driver *u_drv;
#endif

extern S_VDP_REGS_TYPE *pVdpReg;



HI_VOID VDP_WBC_SetCmpEnable( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    U_WBC_G0_CMP WBC_G0_CMP;
    U_WBC_FI_CTRL  WBC_FI_CTRL;

    if(enLayer == VDP_LAYER_WBC_G0
	|| enLayer == VDP_LAYER_WBC_G4
	)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET), WBC_G0_CMP.u32);
    }
    else if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CTRL.u32)));
        WBC_FI_CTRL.bits.cmp_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CTRL.u32)), WBC_FI_CTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetCmpEnable() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_SetCmpDrr( VDP_LAYER_WBC_E enLayer, HI_U32 u32CmpDrr)
{
    U_WBC_G0_CMP WBC_G0_CMP;

    if(enLayer == VDP_LAYER_WBC_G0
	|| enLayer == VDP_LAYER_WBC_G4
	)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_drr = u32CmpDrr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET), WBC_G0_CMP.u32);

    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetCmpDrr() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_SetLossy( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    U_WBC_G0_CMP WBC_G0_CMP;

    if(enLayer == VDP_LAYER_WBC_G0
	|| enLayer == VDP_LAYER_WBC_G4
	)
    {
        WBC_G0_CMP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_lossy_en = bEnable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_CMP.u32) + (enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET), WBC_G0_CMP.u32);

    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetLossy() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_SetOffSet( VDP_LAYER_WBC_E enLayer, HI_U32 u32Offset)
{

    U_WBC_G0_OFFSET WBC_G0_OFFSET;

    if(enLayer == VDP_LAYER_WBC_G0
	|| enLayer == VDP_LAYER_WBC_G4
	)
    {
        WBC_G0_OFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)+(enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET));
        WBC_G0_OFFSET.bits.wbcoffset = u32Offset;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_G0_OFFSET.u32)+(enLayer - VDP_LAYER_WBC_G0) * WBC_OFFSET), WBC_G0_OFFSET.u32);

    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetOffSet() select wrong layer id\n");
	}
}

HI_VOID VDP_WBC_SetTunlCellAddr(VDP_LAYER_WBC_E enLayer,  HI_U32 u32Addr)
{
    U_WBC_DHD0_TUNLADDR     WBC_DHD0_TUNLADDR;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_TUNLADDR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_TUNLADDR.u32)));
        WBC_DHD0_TUNLADDR.bits.tunl_cell_addr =  u32Addr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_TUNLADDR.u32)), WBC_DHD0_TUNLADDR.u32);
    }
	else
    {
        HI_PRINT("Error,VDP_WBC_SetTunlCellAddr() select wrong layer id\n");
    }

}

HI_VOID VDP_WBC_SetLowdlyEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.lowdly_en =  u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }
	else
    {
        HI_PRINT("Error,VDP_WBC_SetLowdlyEnable() select wrong layer id\n");
    }

}

HI_VOID VDP_WBC_SetLowdlyTestEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_LOWDLYCTRL  WBC_DHD0_LOWDLYCTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
		WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
		WBC_DHD0_LOWDLYCTRL.bits.lowdly_test = u32Enable;
		VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetLowdlyTestEnable() select wrong layer id\n");
	}
}

HI_VOID VDP_WBC_SetWbPerLineNum(VDP_LAYER_WBC_E enLayer,  HI_U32 u32Num)
{
    U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.wb_per_line_num =  u32Num - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetWbPerLineNum() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_SetPartfnsLineNum(VDP_LAYER_WBC_E enLayer,HI_U32 u32Num)
{
    U_WBC_DHD0_LOWDLYCTRL    WBC_DHD0_LOWDLYCTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_LOWDLYCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)));
        WBC_DHD0_LOWDLYCTRL.bits.partfns_line_num = u32Num - 1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LOWDLYCTRL.u32)), WBC_DHD0_LOWDLYCTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetPartfnsLineNum() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_HistogramEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.histogam_en =  u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_SetPartfnsLineNum() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_VtthdMode(VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_vtthd_mode)
{
    U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
        WBC_DHD0_CTRL.bits.wbc_vtthd_mode =  u32wbc_vtthd_mode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
    }
	else
	{
        HI_PRINT("Error,VDP_WBC_VtthdMode() select wrong layer id\n");
	}

}

HI_VOID VDP_WBC_SetOflEnable(HI_U32 bEnable)
{

	U_WBC_OFL_EN WBC_OFL_EN;

    WBC_OFL_EN.u32 = (VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32))) & 0xefffffff);
    WBC_OFL_EN.bits.wbc_ofl_en = bEnable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
}

HI_VOID VDP_WBC_SetOflPro(HI_U32 u32Data)
{

	U_WBC_OFL_EN WBC_OFL_EN;

    WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
    WBC_OFL_EN.bits.wbc_ofl_pro = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
}

HI_VOID VDP_WBC_SetOflDispMode(HI_U32 u32Data)
{
#if  0
	U_WBC_OFL_EN WBC_OFL_EN;

    WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
    WBC_OFL_EN.bits.ofl_disp_mode = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
    #endif
}

HI_VOID VDP_WBC_SetOflMixerReso(VDP_RECT_S stRect)
{

	U_WBC_OFL_RESO WBC_OFL_RESO;

    WBC_OFL_RESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_RESO.u32)));
    WBC_OFL_RESO.bits.w = stRect.u32Wth - 1;
    WBC_OFL_RESO.bits.h = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_RESO.u32)), WBC_OFL_RESO.u32);
}
#if  0

HI_VOID VDP_WBC_SetOflStart()
{

	U_WBC_OFL_RESO WBC_OFL_RESO;

	U_WBC_OFL_EN WBC_OFL_EN;

    WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
    WBC_OFL_EN.bits.ofl_start = 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
}
#endif

HI_VOID VDP_WBC_SetCropPos (VDP_LAYER_WBC_E enLayer, VDP_RECT_S stRect)
{
	U_WBC_DHD0_FCROP WBC_DHD0_FCROP;
	U_WBC_DHD0_LCROP WBC_DHD0_LCROP;

    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_GP0
	|| enLayer == VDP_LAYER_WBC_G0
	|| enLayer == VDP_LAYER_WBC_G4
	)
    {
        WBC_DHD0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_FCROP.bits.wfcrop = stRect.u32X;
        WBC_DHD0_FCROP.bits.hfcrop = stRect.u32Y;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_FCROP.u32);

        WBC_DHD0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_LCROP.bits.wlcrop = stRect.u32Wth-1;
        WBC_DHD0_LCROP.bits.hlcrop = stRect.u32Hgt-1;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_LCROP.u32);

    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCropPos() select wrong layer id\n");
    }
    return ;
}

HI_VOID  VDP_WBC_SetOutReso (VDP_LAYER_WBC_E enLayer, VDP_RECT_S stRect)
{
	U_WBC_DHD0_ORESO WBC_DHD0_ORESO;

    WBC_DHD0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32) + enLayer * WBC_OFFSET));
    WBC_DHD0_ORESO.bits.ow = stRect.u32Wth - 1;
    WBC_DHD0_ORESO.bits.oh = stRect.u32Hgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ORESO.u32);

    return ;
}

HI_VOID VDP_WBC_SetEnable( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{

	U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

	WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_CTRL.bits.wbc_en = bEnable;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
}

HI_VOID VDP_WBC_SetDataWidth( VDP_LAYER_WBC_E enLayer, HI_U32 u32DataWidth)
{

	U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

	WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_CTRL.bits.data_width = u32DataWidth;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
}


HI_VOID VDP_WBC_SetOutIntf (VDP_LAYER_WBC_E enLayer, VDP_DATA_RMODE_E u32RdMode)
{
	U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_GP0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_CTRL.bits.mode_out = u32RdMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetOutIntf() select wrong layer id\n");
    }
}

HI_VOID VDP_WBC_SetOutFmt(VDP_LAYER_WBC_E enLayer, VDP_WBC_OFMT_E stIntfFmt)

{
    U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

	WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_CTRL.bits.format_out = stIntfFmt;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);

}

HI_VOID VDP_WBC_SetSpd(VDP_LAYER_WBC_E enLayer, HI_U32 u32ReqSpd)
{
    U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

	WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_CTRL.bits.req_interval = u32ReqSpd;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
}

HI_VOID  VDP_WBC_SetLayerAddr   (VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_WBC_DHD0_STRIDE WBC_DHD0_STRIDE;

	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_YADDR.u32) + enLayer * WBC_OFFSET), u32LAddr);
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CADDR.u32) + enLayer * WBC_OFFSET), u32CAddr);
	WBC_DHD0_STRIDE.bits.wbclstride = u32LStr;
	WBC_DHD0_STRIDE.bits.wbccstride = u32CStr;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_STRIDE.u32) + enLayer * WBC_OFFSET), WBC_DHD0_STRIDE.u32);

	return ;
}

HI_VOID  VDP_WBC_SetLayerReso     (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S  stRect)
{
    U_WBC_DHD0_ORESO WBC_DHD0_ORESO;

	WBC_DHD0_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_ORESO.bits.ow = stRect.u32OWth - 1;
	WBC_DHD0_ORESO.bits.oh = stRect.u32OHgt - 1;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ORESO.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ORESO.u32);

}

HI_VOID VDP_WBC_SetBmp_Thd( VDP_LAYER_WBC_E enLayer, HI_U32 u32wbc_bmp_thd)
{

	U_WBC_BMP_THD WBC_BMP_THD;

	WBC_BMP_THD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_BMP_THD.u32) ));
	WBC_BMP_THD.bits.wbc_bmp_thd= u32wbc_bmp_thd;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_BMP_THD.u32) ), WBC_BMP_THD.u32);
}

HI_VOID VDP_WBC_SetDitherEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)

{
    U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
    U_WBC_GP0_DITHER_CTRL  WBC_GP0_DITHER_CTRL;
    U_WBC_ME_DITHER_CTRL   WBC_ME_DITHER_CTRL;


    if(enLayer ==  VDP_LAYER_WBC_HD0)
    {
        //DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)  ));
        WBC_DHD0_DITHER_CTRL.bits.dither_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) ), WBC_DHD0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32)  ));
        WBC_GP0_DITHER_CTRL.bits.dither_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32) ), WBC_GP0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_ME)
    {
        WBC_ME_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32)  ));
        WBC_ME_DITHER_CTRL.bits.dither_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32) ), WBC_ME_DITHER_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetDitherEnable() select wrong layer id\n");
    }


}
HI_VOID VDP_WBC_SetDitherRoundEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)

{
    U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
    U_WBC_GP0_DITHER_CTRL  WBC_GP0_DITHER_CTRL;
    U_WBC_ME_DITHER_CTRL   WBC_ME_DITHER_CTRL;

    if(enLayer ==  VDP_LAYER_WBC_HD0)
    {
        //DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)  ));
        WBC_DHD0_DITHER_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) ), WBC_DHD0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32)  ));
        WBC_GP0_DITHER_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32) ), WBC_GP0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_ME)
    {
        WBC_ME_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32)  ));
        WBC_ME_DITHER_CTRL.bits.dither_round= u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32) ), WBC_ME_DITHER_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetDitherRoundEnable() select wrong layer id\n");
    }


}


HI_VOID VDP_WBC_SetDitherMode  (VDP_LAYER_WBC_E enLayer, VDP_DITHER_E enDitherMode)
{

    U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
    U_WBC_GP0_DITHER_CTRL  WBC_GP0_DITHER_CTRL;
    U_WBC_ME_DITHER_CTRL   WBC_ME_DITHER_CTRL;
    if(enDitherMode == VDP_DITHER_ROUND_8 )
    {
    enDitherMode =VDP_DITHER_DROP_10;
    }
    else  if(enDitherMode == VDP_DITHER_ROUND_10)
    {
    enDitherMode =VDP_DITHER_TMP_10;
    }
    else
    {
       ;// HI_PRINT("Error,VDP_WBC_SetDitherMode() select wrong Mode!!!!  \n");
    }


    if(enLayer ==  VDP_LAYER_WBC_HD0)
    {
        //DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)  ));
        WBC_DHD0_DITHER_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) ), WBC_DHD0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32)  ));
        WBC_GP0_DITHER_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_DITHER_CTRL.u32) ), WBC_GP0_DITHER_CTRL.u32);
    }
    else  if(enLayer ==  VDP_LAYER_WBC_ME)
    {
        WBC_ME_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32)  ));
        WBC_ME_DITHER_CTRL.bits.dither_mode= enDitherMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_ME_DITHER_CTRL.u32) ), WBC_ME_DITHER_CTRL.u32);
    }
    else
    {
      ;//  HI_PRINT("Error,VDP_WBC_SetDitherMode() select wrong layer id\n");
    }


    // U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;

    // if(enLayer == VDP_LAYER_WBC_HD0
    // || enLayer == VDP_LAYER_WBC_GP0
    // || enLayer == VDP_LAYER_WBC_ME
    // )
    // {
    //     WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) + enLayer * WBC_OFFSET));
    //     WBC_DHD0_DITHER_CTRL.bits.dither_md = enDitherMode;
    //     VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_DITHER_CTRL.u32);

    // }

}

HI_VOID VDP_WBC_SetDitherCoef  (VDP_LAYER_WBC_E enLayer, VDP_DITHER_COEF_S dither_coef)

{

    U_WBC_DHD0_DITHER_COEF0 WBC_DHD0_DITHER_COEF0;
    U_WBC_DHD0_DITHER_COEF1 WBC_DHD0_DITHER_COEF1;

    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_GP0
	|| enLayer == VDP_LAYER_WBC_ME
	)
	{
        WBC_DHD0_DITHER_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF0.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_DITHER_COEF0.bits.dither_coef0 = dither_coef.dither_coef0 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef1 = dither_coef.dither_coef1 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef2 = dither_coef.dither_coef2 ;
        WBC_DHD0_DITHER_COEF0.bits.dither_coef3 = dither_coef.dither_coef3 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF0.u32) + enLayer * WBC_OFFSET), WBC_DHD0_DITHER_COEF0.u32);

        WBC_DHD0_DITHER_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF1.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_DITHER_COEF1.bits.dither_coef4 = dither_coef.dither_coef4 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef5 = dither_coef.dither_coef5 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef6 = dither_coef.dither_coef6 ;
        WBC_DHD0_DITHER_COEF1.bits.dither_coef7 = dither_coef.dither_coef7 ;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_DITHER_COEF1.u32) + enLayer * WBC_OFFSET), WBC_DHD0_DITHER_COEF1.u32);
    }
}

HI_VOID  VDP_WBC_SetCropReso (VDP_LAYER_WBC_E enLayer, VDP_DISP_RECT_S stRect)
{
	U_WBC_DHD0_FCROP WBC_DHD0_FCROP;
	U_WBC_DHD0_LCROP WBC_DHD0_LCROP;

    U_WBC_GP0_FCROP WBC_GP0_FCROP;
    U_WBC_GP0_LCROP WBC_GP0_LCROP;

    U_WBC_G0_FCROP WBC_G0_FCROP;
    U_WBC_G0_LCROP WBC_G0_LCROP;


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

    //if(enLayer == VDP_LAYER_WBC_HD0
	//|| enLayer == VDP_LAYER_WBC_GP0
	//|| enLayer == VDP_LAYER_WBC_G0
	//|| enLayer == VDP_LAYER_WBC_G4
	//)
    //{
    //    WBC_DHD0_FCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32) + enLayer * WBC_OFFSET));
    //    WBC_DHD0_FCROP.bits.wfcrop = stRect.u32DXS;
    //    WBC_DHD0_FCROP.bits.hfcrop = stRect.u32DYS;
    //    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_FCROP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_FCROP.u32);

    //    WBC_DHD0_LCROP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32) + enLayer * WBC_OFFSET));
    //    WBC_DHD0_LCROP.bits.wlcrop = stRect.u32DXL-1;
    //    WBC_DHD0_LCROP.bits.hlcrop = stRect.u32DYL-1;
    //    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_LCROP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_LCROP.u32);

    //}
    return ;
}

HI_VOID  VDP_WBC_SetRegUp (VDP_LAYER_WBC_E enLayer)
{
	U_WBC_DHD0_UPD WBC_DHD0_UPD;
	WBC_DHD0_UPD.bits.regup = 0x1;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_UPD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_UPD.u32);

	return ;
}

#if 0
HI_VOID VDP_WBC_SetZmeCoefAddr(VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E u32Mode, HI_U32 u32Addr)
{
    U_WBC_DHD0_HLCOEFAD WBC_DHD0_HLCOEFAD;
    U_WBC_DHD0_HCCOEFAD WBC_DHD0_HCCOEFAD;
    U_WBC_DHD0_VLCOEFAD WBC_DHD0_VLCOEFAD;
    U_WBC_DHD0_VCCOEFAD WBC_DHD0_VCCOEFAD;

    HI_U32 ADDR_OFFSET= 0x100 ;
#if WBC_GP0_EN
	U_GP0_HCOEFAD      GP0_HCOEFAD;
	U_GP0_VCOEFAD      GP0_VCOEFAD;
#endif

    if( enLayer == VDP_LAYER_WBC_HD0 ||  enLayer == VDP_LAYER_WBC_ME  )
    {
        if(u32Mode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_DHD0_HLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HLCOEFAD.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_HLCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HLCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HLCOEFAD.u32);

            WBC_DHD0_HCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCCOEFAD.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_HCCOEFAD.bits.coef_addr = (u32Addr + ADDR_OFFSET) & 0xfffffff0;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCCOEFAD.u32);

        }
        else if(u32Mode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_DHD0_VLCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_VLCOEFAD.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_VLCOEFAD.bits.coef_addr = u32Addr;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_VLCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_VLCOEFAD.u32);

            WBC_DHD0_VCCOEFAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_VCCOEFAD.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_VCCOEFAD.bits.coef_addr = (u32Addr + ADDR_OFFSET) & 0xfffffff0;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_VCCOEFAD.u32) + enLayer * WBC_OFFSET), WBC_DHD0_VCCOEFAD.u32);

        }
        else
        {
            HI_PRINT("Error,VDP_WBC_SetZmeCoefAddr() Select a Wrong Mode!\n");
        }

    }



#if WBC_GP0_EN
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
#endif
    return ;

}

HI_VOID  VDP_WBC_SetParaUpd (VDP_LAYER_WBC_E enLayer, VDP_WBC_PARA_E enMode)
{
    U_WBC_DHD0_PARAUP WBC_DHD0_PARAUP;
    U_WBC_ME_PARAUP WBC_ME_PARAUP;
#if WBC_GP0_EN
	U_GP0_PARAUP GP0_PARAUP;
	GP0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_PARAUP.u32) + WBC_GP0_SEL * GP_OFFSET));
#endif


    if( enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_DHD0_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_PARAUP.u32)));
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
#if WBC_GP0_EN
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
#endif
    else if( enLayer == VDP_LAYER_WBC_ME)
    {

        WBC_ME_PARAUP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_ME_PARAUP.u32)));
        if(enMode == VDP_WBC_PARA_ZME_HOR)
        {
            WBC_ME_PARAUP.bits.wbc_hlcoef_upd= 0x1;
            WBC_ME_PARAUP.bits.wbc_hccoef_upd= 0x1;
        }
        else if(enMode == VDP_WBC_PARA_ZME_VER)
        {
            WBC_ME_PARAUP.bits.wbc_vlcoef_upd= 0x1;
            WBC_ME_PARAUP.bits.wbc_vccoef_upd= 0x1;
        }
        else
        {
            HI_PRINT("error,VDP_WBC_DHD0_SetParaUpd() select wrong mode!\n");
        }

        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_ME_PARAUP.u32)), WBC_ME_PARAUP.u32);
        return ;
    }
}



HI_VOID VDP_WBC_SetZmeEnable  (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

#if WBC_GP0_EN
	U_GP0_ZME_HSP      GP0_ZME_HSP;
	U_GP0_ZME_VSP      GP0_ZME_VSP;
#endif

    /*WBC zoom enable */
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hlmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hlfir_en = 1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hchmsc_en = u32bEnable;
            WBC_DHD0_ZME_HSP.bits.hchfir_en = 1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        //if((enMode == VDP_ZME_MODE_NONL)||(enMode == VDP_ZME_MODE_ALL))
        //{
        //    WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)));
        //    WBC_DHD0_ZME_HSP.bits.non_lnr_en = u32bEnable;
        //    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32)), WBC_DHD0_ZME_HSP.u32);
        //}


        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vlmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vlfir_en = 1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vchmsc_en = u32bEnable;
            WBC_DHD0_ZME_VSP.bits.vchfir_en = 1;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

    }
#if WBC_GP0_EN
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
#endif
    return ;

}


HI_VOID  VDP_WBC_SetMidEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_U32 bEnable)
{

    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
#if WBC_GP0_EN
	U_GP0_ZME_HSP      GP0_ZME_HSP;
	U_GP0_ZME_VSP      GP0_ZME_VSP;
#endif
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);

        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vlmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vchmid_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }
    }
#if WBC_GP0_EN
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
#endif
    return;

}

#endif

HI_VOID VDP_WBC_SetFirEnable(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 bEnable)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
#if WBC_GP0_EN
	U_GP0_ZME_HSP      GP0_ZME_HSP;
	U_GP0_ZME_VSP      GP0_ZME_VSP;
#endif
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hlfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);

        }

        if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_HSP.bits.hchfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vlfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VSP.bits.vchfir_en = bEnable;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
        }
    }
#if WBC_GP0_EN
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
#endif

    return ;

}

#if 0
HI_VOID VDP_WBC_SetZmeVerTap(VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode, HI_U32 u32VerTap)
{
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        /*
           if ((enMode == VDP_ZME_MODE_VERL) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
           {
           WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
           WBC_DHD0_ZME_VSP.bits.vsc_luma_tap = u32VerTap;
           VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
           }
         */


        if ((enMode == VDP_ZME_MODE_VERC) || (enMode == VDP_ZME_MODE_VER) || (enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)));
            WBC_DHD0_ZME_VSP.bits.vsc_chroma_tap = u32VerTap;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32)), WBC_DHD0_ZME_VSP.u32);
        }
    }

}


HI_VOID VDP_WBC_SetZmeHfirOrder(VDP_LAYER_WBC_E enLayer, HI_U32 u32HfirOrder)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
#if WBC_GP0_EN
        U_GP0_ZME_HSP      GP0_ZME_HSP;
#endif
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {

        WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }
#if WBC_GP0_EN
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
    }
#endif
    return ;
}

HI_VOID VDP_WBC_SetZmeHorRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
#if WBC_GP0_EN
	U_GP0_ZME_HSP      GP0_ZME_HSP;
#endif
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_ZME_HSP.bits.hratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_HSP.u32);
    }
#if WBC_GP0_EN
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_HSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_HSP.bits.hratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HSP.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HSP.u32);
    }
#endif
    return ;
}

#endif

HI_VOID  VDP_WBC_SetZmeInFmt_Define1(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = u32Fmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
    }

    return ;
}

HI_VOID  VDP_WBC_SetZmeOutFmt(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_ZME_VSP.bits.zme_out_fmt = u32Fmt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSP.u32);
    }

    return ;
}

#if 0
HI_VOID  VDP_WBC_SetZmeVerRatio(VDP_LAYER_WBC_E enLayer, HI_U32 u32Ratio)
{
    U_WBC_DHD0_ZME_VSR WBC_DHD0_ZME_VSR;
#if WBC_GP0_EN
	U_GP0_ZME_VSR        GP0_ZME_VSR;
#endif

    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {
        WBC_DHD0_ZME_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_ZME_VSR.bits.vratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VSR.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VSR.u32);
    }
#if WBC_GP0_EN
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_ZME_VSR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_ZME_VSR.bits.vratio = u32Ratio;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_VSR.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_VSR.u32);
    }
#endif
    return ;
}

HI_VOID  VDP_WBC_SetZmePhase    (VDP_LAYER_WBC_E enLayer, VDP_ZME_MODE_E enMode,HI_S32 s32Phase)
{
    U_WBC_DHD0_ZME_VOFFSET   WBC_DHD0_ZME_VOFFSET;
    U_WBC_DHD0_ZME_VBOFFSET  WBC_DHD0_ZME_VBOFFSET;
#if WBC_GP0_EN
    U_GP0_ZME_HLOFFSET  GP0_ZME_HLOFFSET;
    U_GP0_ZME_HCOFFSET  GP0_ZME_HCOFFSET;
	U_GP0_ZME_VOFFSET        GP0_ZME_VOFFSET;
#endif

    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_ME)
    {

        if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VOFFSET.bits.vluma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VOFFSET.bits.vchroma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VOFFSET.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VBOFFSET.bits.vbluma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VBOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_VERB)||(enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_ALL))
        {
            WBC_DHD0_ZME_VBOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32) + enLayer * WBC_OFFSET));
            WBC_DHD0_ZME_VBOFFSET.bits.vbchroma_offset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_ZME_VBOFFSET.u32) + enLayer * WBC_OFFSET), WBC_DHD0_ZME_VBOFFSET.u32);
        }
    }
#if WBC_GP0_EN
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HLOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HLOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HLOFFSET.bits.hor_loffset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HLOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HLOFFSET.u32);
        }

        if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
        {
            GP0_ZME_HCOFFSET.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_ZME_HCOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET));
            GP0_ZME_HCOFFSET.bits.hor_coffset = s32Phase;
            VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_ZME_HCOFFSET.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_ZME_HCOFFSET.u32);
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
#endif

    return ;
}

#endif

HI_VOID  VDP_WBC_SetCscEnable  (VDP_LAYER_WBC_E enLayer, HI_U32 enCSC)
{
    U_WBC_DHD0_CSCIDC WBC_DHD0_CSCIDC;
#if WBC_GP0_EN
	U_GP0_CSC_IDC     GP0_CSC_IDC;
#endif

    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_DHD0_CSCIDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)));
        WBC_DHD0_CSCIDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CSCIDC.u32)), WBC_DHD0_CSCIDC.u32);
    }
#if WBC_GP0_EN
    else if(enLayer == VDP_LAYER_WBC_GP0)
    {
        GP0_CSC_IDC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET));
        GP0_CSC_IDC.bits.csc_en = enCSC;
        VDP_RegWrite((HI_U32)(&(pVdpReg->GP0_CSC_IDC.u32) + WBC_GP0_SEL * GP_OFFSET), GP0_CSC_IDC.u32);

    }
#endif
    return ;
}

HI_VOID VDP_WBC_SetCscDcCoef(VDP_LAYER_WBC_E enLayer,VDP_CSC_DC_COEF_S stCscCoef)
{
    U_WBC_DHD0_CSCIDC WBC_DHD0_CSCIDC;
    U_WBC_DHD0_CSCODC WBC_DHD0_CSCODC;
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_FI)
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
        U_GP0_CSC_IDC  GP0_CSC_IDC;
        U_GP0_CSC_ODC  GP0_CSC_ODC;
        U_GP0_CSC_IODC GP0_CSC_IODC;
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

HI_VOID VDP_WBC_SetCscCoef(VDP_LAYER_WBC_E enLayer,VDP_CSC_COEF_S stCscCoef)
{
    U_WBC_DHD0_CSCP0        WBC_DHD0_CSCP0;
    U_WBC_DHD0_CSCP1        WBC_DHD0_CSCP1;
    U_WBC_DHD0_CSCP2        WBC_DHD0_CSCP2;
    U_WBC_DHD0_CSCP3        WBC_DHD0_CSCP3;
    U_WBC_DHD0_CSCP4        WBC_DHD0_CSCP4;
    if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_FI)
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
        U_GP0_CSC_P0            GP0_CSC_P0;
        U_GP0_CSC_P1            GP0_CSC_P1;
        U_GP0_CSC_P2            GP0_CSC_P2;
        U_GP0_CSC_P3            GP0_CSC_P3;
        U_GP0_CSC_P4            GP0_CSC_P4;

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
        //csc config mode
        VDP_CSC_COEF_S    st_csc_coef;
        VDP_CSC_DC_COEF_S st_csc_dc_coef;

        HI_U32 u32Pre   ;//= 1 << 10;
        HI_U32 u32DcPre ;//= 4;//1:8bit; 4:10bit

        if(enLayer == VDP_LAYER_WBC_HD0 || enLayer == VDP_LAYER_WBC_FI)
        {
            u32Pre   = 1 << 10;
            u32DcPre = 1;//1:8bit; 4:10bit
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

            VDP_WBC_SetCscCoef  ( enLayer,st_csc_coef);
            VDP_WBC_SetCscDcCoef( enLayer,st_csc_dc_coef);
        }
        else if(enLayer == VDP_LAYER_WBC_GP0)
        {
            u32Pre   = 1 << 10;
            u32DcPre = 4;//1:8bit; 4:10bit
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
        }
    }

    return ;
}

HI_VOID VDP_WBC_SetAutoSt( VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
	U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;

	WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
	WBC_DHD0_CTRL.bits.auto_stop_en=  bEnable;
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
}

HI_VOID VDP_WBC_SetThreeMd( VDP_LAYER_WBC_E enLayer, HI_U32 bMode)
{
	U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;

    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_GP0
	)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_CTRL.bits.three_d_mode =  bMode;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + enLayer * WBC_OFFSET), WBC_DHD0_CTRL.u32);
    }
}

#if CDS_EN
HI_VOID VDP_WBC_SetCdsEnable(VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HCDS WBC_DHD0_HCDS;
    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_FI
	)
    {
        WBC_DHD0_HCDS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_HCDS.bits.hcds_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCDS.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCdsEnable() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCdsFirEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HCDS WBC_DHD0_HCDS;
    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_FI
	)
    {
        WBC_DHD0_HCDS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_HCDS.bits.hchfir_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCDS.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCdsFirEnable() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCdsMidEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HCDS WBC_DHD0_HCDS;
    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_FI
	)
    {
        WBC_DHD0_HCDS.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_HCDS.bits.hchmid_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCDS.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCdsMidEnable() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCdsCoef             (VDP_LAYER_WBC_E enLayer, HI_S32 * s32Coef)
{
    U_WBC_DHD0_HCDS_COEF0 WBC_DHD0_HCDS_COEF0;
    U_WBC_DHD0_HCDS_COEF1 WBC_DHD0_HCDS_COEF1;

    if(enLayer == VDP_LAYER_WBC_HD0
	|| enLayer == VDP_LAYER_WBC_FI
	)
    {
        WBC_DHD0_HCDS_COEF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS_COEF0.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_HCDS_COEF0.bits.coef0 = s32Coef[0];
        WBC_DHD0_HCDS_COEF0.bits.coef1 = s32Coef[1];
        WBC_DHD0_HCDS_COEF0.bits.coef2 = s32Coef[2];
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS_COEF0.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCDS_COEF0.u32);

        WBC_DHD0_HCDS_COEF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS_COEF1.u32) + enLayer * WBC_OFFSET));
        WBC_DHD0_HCDS_COEF1.bits.coef3 = s32Coef[3];
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HCDS_COEF1.u32) + enLayer * WBC_OFFSET), WBC_DHD0_HCDS_COEF1.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCdsCoef() Select a Wrong layer!\n");
    }
}
#endif

#if 0
//mod me
HI_VOID VDP_WBC_SetDhdLocate(HI_U32 u32Id, VDP_WBC_LOCATE_E   u32Data)

{
    if(u32Id>= CHN_MAX)
    {
        HI_PRINT("error,VDP_WBC_SetDhdLocate() select wrong layer id\n");
        return ;
    }

    U_WBC_DHD_LOCATE  WBC_DHD_LOCATE;

    WBC_DHD_LOCATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)));
    WBC_DHD_LOCATE.bits.wbc_dhd_locate= (HI_U32) u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)), WBC_DHD_LOCATE.u32);
}


HI_VOID VDP_WBC_SetDhdOflEn(HI_U32 u32Id, HI_U32  u32Data)
{
    if(u32Id>= CHN_MAX)
    {
        HI_PRINT("error,VDP_WBC_SetDhdLocate() select wrong layer id\n");
        return ;
    }

    U_WBC_OFL_EN  WBC_OFL_EN;

    WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
    WBC_OFL_EN.bits.wbc_ofl_en= u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);
}
#endif

#if 0
//new_add wbc_dhd
HI_VOID VDP_WBC_SetDhdOflEn(VDP_LAYER_WBC_E enLayer, HI_U32 u32Data)
{
    U_WBC_OFL_EN WBC_OFL_EN;
    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
        WBC_OFL_EN.bits.wbc0_ofl_en = u32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_HD1)
    {

        WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
        WBC_OFL_EN.bits.wbc1_ofl_en = u32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);

    }
    else if(enLayer == VDP_LAYER_WBC_DSD)
    {

        WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
        WBC_OFL_EN.bits.wbc2_ofl_en = u32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);

    }


}
#endif

#if 0
//mod me
HI_VOID VDP_WBC_SetDhdOflMode(VDP_LAYER_WBC_E enLayer, HI_U32 u32Data)
{
    U_WBC_OFL_EN WBC_OFL_EN;
    if(enLayer == VDP_LAYER_WBC_HD0)
    {

        WBC_OFL_EN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)));
        WBC_OFL_EN.bits.wbc0_ofl_pro = u32Data;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_OFL_EN.u32)), WBC_OFL_EN.u32);

    }

}




#endif
HI_VOID VDP_WBC_SetConnection(HI_U32 u32LayerId, HI_U32 u32Data )
{

    U_WBC_DHD_LOCATE  WBC_DHD_LOCATE;
    if(u32LayerId != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("Error, VDP_WBC_SetConnection() select wrong dhd layer id\n");
        return ;

    }


    WBC_DHD_LOCATE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)));
    WBC_DHD_LOCATE.bits.wbc_dhd_locate= (HI_U32) u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD_LOCATE.u32)), WBC_DHD_LOCATE.u32);

    //U_WBC_CORRESP  WBC_CORRESP;

    //WBC_CORRESP.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_CORRESP.u32)));
    //WBC_CORRESP.bits.wbc_corresp = u32Data;
    //VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_CORRESP.u32)), WBC_CORRESP.u32);

}

HI_VOID VDP_WBC_SetCoefReadEnable(HI_U32 u32Id, HI_U32 u32Para)
{
    HI_U32 u32WrData;
    u32WrData = 1 << u32Para;

    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_PARARD.u32) + u32Id ), u32WrData);
}

HI_VOID VDP_WBC_SetCoefReadDisable(HI_U32 u32Id, HI_U32 u32Para)
{

    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_PARARD.u32) + u32Id ), 0);
}

HI_VOID VDP_WBC_SetCmpMb(VDP_LAYER_WBC_E enLayer, HI_U32 u32MbBits)
{
    U_WBC_FI_CMP_MB  WBC_FI_CMP_MB;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_MB.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_MB.u32)));
        WBC_FI_CMP_MB.bits.mb_bits= u32MbBits;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_MB.u32)), WBC_FI_CMP_MB.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpMb() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCmpMaxMin(VDP_LAYER_WBC_E enLayer, HI_U32 u32MinBitsCnt, HI_U32 u32MaxBitsCnt)
{
    U_WBC_FI_CMP_MAX_MIN   WBC_FI_CMP_MAX_MIN;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_MAX_MIN.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_MAX_MIN.u32)));
        WBC_FI_CMP_MAX_MIN.bits.min_bits_cnt= u32MinBitsCnt;
        WBC_FI_CMP_MAX_MIN.bits.max_bits_cnt= u32MaxBitsCnt;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_MAX_MIN.u32)), WBC_FI_CMP_MAX_MIN.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpMaxMin() Select a Wrong layer!\n");
    }
}
HI_VOID VDP_WBC_SetCmpAdjThr(VDP_LAYER_WBC_E enLayer, HI_U32 u32AdjSadThr, HI_U32 u32AdjSadBitThr,HI_U32 u32AdjSpecBitThr)
{
    U_WBC_FI_CMP_ADJ_THR WBC_FI_CMP_ADJ_THR;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_ADJ_THR.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_ADJ_THR.u32)));
        WBC_FI_CMP_ADJ_THR.bits.adj_sad_thr= u32AdjSadThr;
        WBC_FI_CMP_ADJ_THR.bits.adj_sad_bit_thr= u32AdjSadBitThr;
        WBC_FI_CMP_ADJ_THR.bits.adj_spec_bit_thr= u32AdjSpecBitThr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_ADJ_THR.u32)), WBC_FI_CMP_ADJ_THR.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpAdjThr() Select a Wrong layer!\n");
    }
}
HI_VOID VDP_WBC_SetCmpBigGrad(VDP_LAYER_WBC_E enLayer, HI_U32 u32BigGradThr, HI_U32 u32BigGradNumThr)
{
    U_WBC_FI_CMP_BIG_GRAD WBC_FI_CMP_BIG_GRAD;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_BIG_GRAD.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_BIG_GRAD.u32)));
        WBC_FI_CMP_BIG_GRAD.bits.big_grad_thr= u32BigGradThr;
        WBC_FI_CMP_BIG_GRAD.bits.big_grad_num_thr= u32BigGradNumThr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_BIG_GRAD.u32)), WBC_FI_CMP_BIG_GRAD.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpBigGrad() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCmpBlk(VDP_LAYER_WBC_E enLayer, HI_U32 u32SmthThr, HI_U32 u32BlkCompThr)
{
    U_WBC_FI_CMP_BLK WBC_FI_CMP_BLK;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_BLK.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_BLK.u32)));
        WBC_FI_CMP_BLK.bits.smth_thr= u32SmthThr;
        WBC_FI_CMP_BLK.bits.blk_comp_thr= u32BlkCompThr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_BLK.u32)), WBC_FI_CMP_BLK.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpBlk() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCmpGraphic(VDP_LAYER_WBC_E enLayer, HI_U32 u32GrcEn, HI_U32 u32VideoSadThr)
{
    U_WBC_FI_CMP_GRAPHIC_JUDGE  WBC_FI_CMP_GRAPHIC_JUDGE;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_GRAPHIC_JUDGE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_GRAPHIC_JUDGE.u32)));
        WBC_FI_CMP_GRAPHIC_JUDGE.bits.graphic_en = u32GrcEn;
        WBC_FI_CMP_GRAPHIC_JUDGE.bits.video_sad_thr= u32VideoSadThr;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_GRAPHIC_JUDGE.u32)), WBC_FI_CMP_GRAPHIC_JUDGE.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpGraphic() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCmpRateCtrl(VDP_LAYER_WBC_E enLayer, HI_U32 u32MaxTrow, HI_U32 u32SmthGain, HI_U32 u32SadBitNgain)
{
    U_WBC_FI_CMP_RC  WBC_FI_CMP_RC;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_RC.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_RC.u32)));
        WBC_FI_CMP_RC.bits.max_trow_bits = u32MaxTrow;
        WBC_FI_CMP_RC.bits.rc_smth_gain  = u32SmthGain;
        WBC_FI_CMP_RC.bits.sadbits_ngain = u32SadBitNgain;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_RC.u32)), WBC_FI_CMP_RC.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpRateCtrl() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetCmpFrameSize(VDP_LAYER_WBC_E enLayer, HI_U32 u32FrameSize)
{
    U_WBC_FI_CMP_FRAME_SIZE  WBC_FI_CMP_FRAME_SIZE;
    if(enLayer == VDP_LAYER_WBC_FI)
    {
        WBC_FI_CMP_FRAME_SIZE.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CMP_FRAME_SIZE.u32)));
        WBC_FI_CMP_FRAME_SIZE.bits.frame_size= u32FrameSize;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CMP_FRAME_SIZE.u32)), WBC_FI_CMP_FRAME_SIZE.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetCmpFrameSize() Select a Wrong layer!\n");
    }
}

HI_VOID VDP_WBC_SetMasterSel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
	U_VOMASTERSEL    VOMASTERSEL;
	U_VOMASTERSEL1   VOMASTERSEL1;
    U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    U_WBC_ME_CTRL    WBC_ME_CTRL;
    U_WBC_FI_CTRL    WBC_FI_CTRL;
    U_WBC_BMP_CTRL   WBC_BMP_CTRL;

	VOMASTERSEL.u32  = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)));
	VOMASTERSEL1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)));

    WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
    WBC_ME_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_ME_CTRL.u32)));
    WBC_FI_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_FI_CTRL.u32)));
    WBC_BMP_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_BMP_CTRL.u32)));


    if(u32MasterNum == VDP_MASTER0)
    {

        if(u32Data == VDP_LAYER_WBC_HD0)
        {
            WBC_DHD0_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_GP0)
        {
            VOMASTERSEL.bits.wbc1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_G0)
        {
            VOMASTERSEL.bits.wbc2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_G4)
        {
            VOMASTERSEL.bits.wbc3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_ME)
        {
            WBC_ME_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_FI)
        {
            WBC_FI_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc5_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_BMP)
        {
            WBC_BMP_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc6_master_sel = 0;
        }
        else
        {
            HI_PRINT("Error! VDP_WBC_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER1)
    {
        if(u32Data == VDP_LAYER_WBC_HD0)
        {
            WBC_DHD0_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_GP0)
        {
            VOMASTERSEL.bits.wbc1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_G0)
        {
            VOMASTERSEL.bits.wbc2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_G4)
        {
            VOMASTERSEL.bits.wbc3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_ME)
        {
            WBC_ME_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_FI)
        {
            WBC_FI_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc5_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_BMP)
        {
            WBC_BMP_CTRL.bits.ofl_master    = 0;
            VOMASTERSEL.bits.wbc6_master_sel = 1;
        }
        else
        {
            HI_PRINT("Error! VDP_WBC_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER2)
    {
        if(u32Data == VDP_LAYER_WBC_HD0)
        {
            WBC_DHD0_CTRL.bits.ofl_master    = 1;
            VOMASTERSEL1.bits.wbc0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_GP0)
        {
            VOMASTERSEL1.bits.wbc1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_G0)
        {
            VOMASTERSEL1.bits.wbc2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_G4)
        {
            VOMASTERSEL1.bits.wbc3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_ME)
        {
            WBC_ME_CTRL.bits.ofl_master    = 1;

            VOMASTERSEL1.bits.wbc4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_FI)
        {
            WBC_FI_CTRL.bits.ofl_master    = 1;

            VOMASTERSEL1.bits.wbc5_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_WBC_BMP)
        {
            WBC_BMP_CTRL.bits.ofl_master    = 1;
            VOMASTERSEL1.bits.wbc6_master_sel = 0;
        }
        else
        {
            HI_PRINT("Error! VDP_WBC_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER3)
    {
        if(u32Data == VDP_LAYER_WBC_HD0)
        {
            WBC_DHD0_CTRL.bits.ofl_master    = 1;
            VOMASTERSEL1.bits.wbc0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_GP0)
        {
            VOMASTERSEL1.bits.wbc1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_G0)
        {
            VOMASTERSEL1.bits.wbc2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_G4)
        {
            VOMASTERSEL1.bits.wbc3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_ME)
        {
            WBC_ME_CTRL.bits.ofl_master       = 1;
            VOMASTERSEL1.bits.wbc4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_FI)
        {
            WBC_FI_CTRL.bits.ofl_master       = 1;
            VOMASTERSEL1.bits.wbc5_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_WBC_BMP)
        {
            WBC_BMP_CTRL.bits.ofl_master      = 1;
            VOMASTERSEL1.bits.wbc6_master_sel = 1;
        }
        else
        {
            HI_PRINT("Error! VDP_WBC_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }


	VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32);

    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_ME_CTRL.u32)), WBC_ME_CTRL.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_BMP_CTRL.u32)), WBC_BMP_CTRL.u32);
	VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_FI_CTRL.u32)), WBC_FI_CTRL.u32);

    return ;

}


//wbc flip
HI_VOID VDP_WBC_SetFlipEnable(HI_U32 u32LayerId, HI_U32 u32Enable)
{
    U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    if(u32LayerId == VDP_LAYER_WBC_HD0
	|| u32LayerId == VDP_LAYER_WBC_GP0
	)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + u32LayerId * WBC_OFFSET));
        WBC_DHD0_CTRL.bits.flip_en = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + u32LayerId * WBC_OFFSET), WBC_DHD0_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetFlipEnable() Select a Wrong layer!\n");
    }

	return ;

}

HI_VOID VDP_WBC_SetUVOrder(HI_U32 u32LayerId, HI_U32 u32Enable)
{
    U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    if(u32LayerId == VDP_LAYER_WBC_HD0)
    {
        WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + u32LayerId * WBC_OFFSET));
        WBC_DHD0_CTRL.bits.uv_order = u32Enable;
        VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32) + u32LayerId * WBC_OFFSET), WBC_DHD0_CTRL.u32);
    }
    else
    {
        HI_PRINT("Error,VDP_WBC_SetUVOrder() Select a Wrong layer!\n");
    }

	return ;

}

HI_VOID VDP_WBC_SetDhdFlip(HI_U32 u32LayerId, HI_U32 u32Data )
{

    U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    if(u32LayerId != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("Error, VDP_WBC_SetDhdFlip() select wrong dhd layer id\n");
        return ;

    }
    WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)));
    WBC_DHD0_CTRL.bits.flip_en= (HI_U32) u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_CTRL.u32)), WBC_DHD0_CTRL.u32);

}
HI_VOID VDP_WBC_SetGpFlip(HI_U32 u32LayerId, HI_U32 u32Data )
{

    U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    if(u32LayerId != VDP_LAYER_WBC_GP0)
    {
        HI_PRINT("Error, VDP_WBC_SetGpFlip() select wrong dhd layer id\n");
        return ;

    }
    WBC_GP0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)));
    WBC_GP0_CTRL.bits.flip_en= (HI_U32) u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);

}
HI_VOID VDP_WBC_CDS_init(VDP_LAYER_WBC_E wbc_enlayer)
{
    HI_S32 s32CdsCoef[4] = {103,335,103,-29};
	VDP_WBC_SetCdsEnable(wbc_enlayer, HI_TRUE);
	VDP_WBC_SetCdsFirEnable(wbc_enlayer, HI_TRUE);
	VDP_WBC_SetCdsMidEnable(wbc_enlayer, HI_TRUE);
	VDP_WBC_SetCdsCoef(wbc_enlayer, s32CdsCoef);
}


HI_VOID VDP_WBC_SetHorZmeEnable        (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HPZME  WBC_DHD0_HPZME;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("Error, VDP_WBC_SetHorZmeEnable() select wrong dhd layer id\n");
        return ;
    }

    WBC_DHD0_HPZME.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)));
    WBC_DHD0_HPZME.bits.hpzme_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)), WBC_DHD0_HPZME.u32);

}

HI_VOID VDP_WBC_SetHorZmeMidEnable     (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HPZME  WBC_DHD0_HPZME;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("Error, VDP_WBC_SetHorZmeMidEnable() select wrong dhd layer id\n");
        return ;
    }

    WBC_DHD0_HPZME.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)));
    WBC_DHD0_HPZME.bits.hpzme_mid_en = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)), WBC_DHD0_HPZME.u32);

}

HI_VOID VDP_WBC_SetHorZmeFirEnable     (VDP_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    U_WBC_DHD0_HPZME  WBC_DHD0_HPZME;

    if(enLayer != VDP_LAYER_WBC_HD0)
    {
        HI_PRINT("Error, VDP_WBC_SetHorZmeFirEnable() select wrong dhd layer id\n");
        return ;
    }

    WBC_DHD0_HPZME.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)));
    WBC_DHD0_HPZME.bits.hpzme_mode = u32Enable;
    VDP_RegWrite((HI_U32)(&(pVdpReg->WBC_DHD0_HPZME.u32)), WBC_DHD0_HPZME.u32);

}
