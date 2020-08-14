//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_drv_gfx.cpp
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
#include "vdp_hal_gfx.h"

extern S_VDP_REGS_TYPE *pVdpReg;

extern HI_S32 sGetRand(HI_S32 max, HI_S32 min);

HI_VOID VDP_GFX_SetGfxArbMode  (HI_U32 u32GfxLayer, HI_U32 u32Mode)
{
    U_VOCTRL VOCTRL;
    if(u32GfxLayer >= GFX_MAX)
    {
        VDP_PRINT("Error, VDP_GFX_SetGfxArbMode ID\n");
        return ;
    }

    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.grc_arb_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)),VOCTRL.u32);
}

HI_VOID  VDP_GFX_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_G0_VFPOS G0_VFPOS;
    U_G0_VLPOS G0_VLPOS;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetVideoPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    G0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_VFPOS.u32) + u32Data * GFX_OFFSET));
    G0_VFPOS.bits.video_xfpos = stRect.u32X;
    G0_VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_VFPOS.u32) + u32Data * GFX_OFFSET), G0_VFPOS.u32); 

    G0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_VLPOS.u32) + u32Data * GFX_OFFSET));
    G0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    G0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_VLPOS.u32) + u32Data * GFX_OFFSET), G0_VLPOS.u32); 
    return ;
}   

HI_VOID  VDP_GFX_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_G0_DFPOS G0_DFPOS;
    U_G0_DLPOS G0_DLPOS;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDispPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    G0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DFPOS.u32) + u32Data * GFX_OFFSET));
    G0_DFPOS.bits.disp_xfpos = stRect.u32X;
    G0_DFPOS.bits.disp_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DFPOS.u32) + u32Data * GFX_OFFSET), G0_DFPOS.u32); 

    G0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DLPOS.u32) + u32Data * GFX_OFFSET));
    G0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
    G0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DLPOS.u32) + u32Data * GFX_OFFSET), G0_DLPOS.u32); 
    return ;
}   

HI_VOID  VDP_GFX_SetInReso(HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_G0_IRESO G0_IRESO;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetInReso Select Wrong Graph Layer ID\n");
        return ;
    }

    //grap position 
    G0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_IRESO.u32) + u32Data * GFX_OFFSET));
    G0_IRESO.bits.iw = stRect.u32Wth - 1;
    G0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_IRESO.u32) + u32Data * GFX_OFFSET), G0_IRESO.u32); 
    return ;
}

HI_VOID  VDP_GFX_SetSrcPos (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_G0_SFPOS G0_SFPOS;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetSrcPos Select Wrong Graph Layer ID\n");
        return ;
    }
    
    // Read source position
    G0_SFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_SFPOS.u32)+ u32Data * GFX_OFFSET));
    G0_SFPOS.bits.src_xfpos = stRect.u32SX;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_SFPOS.u32)+ u32Data * GFX_OFFSET), G0_SFPOS.u32);

    return ;
}

HI_VOID  VDP_GFX_SetLayerEnable(HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerEnable Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.surface_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 

    return ;
}

/*move to hal common*/
#if 0
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
#endif
HI_VOID VDP_GFX_SetReqLength(HI_U32 u32Data, HI_U32 u32RegLen)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetReqCtrl() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.rlen_sel = u32RegLen;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 

    return ;
}


HI_VOID  VDP_GFX_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    U_G0_CTRL  G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFx_SetNoSecFlag() Select Wrong Video Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.nosec_flag= u32Enable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);

    return ;
}

HI_VOID VDP_GFX_SetLayerAddr(HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32Stride)
{
    U_G0_ADDR G0_ADDR;
    U_G0_STRIDE G0_STRIDE;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_ADDR.u32 = u32LAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_ADDR.u32)+ u32Data * GFX_OFFSET), G0_ADDR.u32);

    G0_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_STRIDE.u32)+ u32Data * GFX_OFFSET));
    G0_STRIDE.bits.surface_stride = u32Stride;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_STRIDE.u32)+ u32Data * GFX_OFFSET), G0_STRIDE.u32);
}

HI_VOID  VDP_GFX_SetInDataFmt(HI_U32 u32Data, VDP_GFX_IFMT_E  enDataFmt)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDataFmt() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.ifmt = enDataFmt;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 

    return ;
}

HI_VOID VDP_GFX_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetReadMode Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.read_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32); 


}

HI_VOID VDP_GFX_SetBitExtend(HI_U32 u32Data, VDP_GFX_BITEXTEND_E u32mode)
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetBitExtend Select Wrong Graph Layer ID\n");
        return ;
    }


    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.bitext = u32mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32); 


}

HI_VOID VDP_GFX_SetColorKey(HI_U32 u32Data, HI_U32 u32keyEn,VDP_GFX_CKEY_S stKey )
{
    U_G0_CKEYMAX G0_CKEYMAX;
    U_G0_CKEYMIN G0_CKEYMIN;
    U_G0_CBMPARA G0_CBMPARA;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetColorKey Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CKEYMAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMAX.bits.keyr_max = stKey.u32Key_r_max;
    G0_CKEYMAX.bits.keyg_max = stKey.u32Key_g_max;
    G0_CKEYMAX.bits.keyb_max = stKey.u32Key_b_max;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET), G0_CKEYMAX.u32);

    G0_CKEYMIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET));
    G0_CKEYMIN.bits.keyr_min = stKey.u32Key_r_min;
    G0_CKEYMIN.bits.keyg_min = stKey.u32Key_g_min;
    G0_CKEYMIN.bits.keyb_min = stKey.u32Key_b_min;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET), G0_CKEYMIN.u32);

    G0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.key_en = u32keyEn;
    G0_CBMPARA.bits.key_mode = stKey.bKeyMode;            
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32); 

}

HI_VOID VDP_GFX_SetKeyMask(HI_U32 u32Data, VDP_GFX_MASK_S stMsk)
{
    U_G0_CMASK G0_CMASK;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetKeyMask Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CMASK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CMASK.u32)+ u32Data * GFX_OFFSET));
    G0_CMASK.bits.kmsk_r = stMsk.u32Mask_r;
    G0_CMASK.bits.kmsk_g = stMsk.u32Mask_g;
    G0_CMASK.bits.kmsk_b = stMsk.u32Mask_b;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CMASK.u32)+ u32Data * GFX_OFFSET), G0_CMASK.u32);

}

HI_VOID  VDP_GFX_SetParaUpd  (HI_U32 u32Data, VDP_GFX_PARA_E enMode )
{
    U_G0_PARAUP G0_PARAUP;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetParaUpd Select Wrong Graph Layer ID\n");
        return ;
    }

    if (enMode == VDP_GFX_PARA_LUT)
    {
        G0_PARAUP.bits.gdc_paraup = 0x1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_PARAUP.u32)+ u32Data * GFX_OFFSET), G0_PARAUP.u32);
    }



}

HI_VOID VDP_GFX_SetLutAddr(HI_U32 u32Data, HI_U32 u32LutAddr)
{
    U_G0_PARAADDR G0_PARAADDR;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLutAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_PARAADDR.u32 = u32LutAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_PARAADDR.u32)+ u32Data * GFX_OFFSET), G0_PARAADDR.u32);
}

HI_VOID  VDP_GFX_SetDcmpEnable(HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerEnable Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.dcmp_en = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 

    return ;
}

HI_VOID VDP_GFX_SetDcmpAddr(HI_U32 u32Data, HI_U32 u32LAddr)
{
    U_G0_DCMP_ADDR G0_DCMP_ADDR;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_ADDR.u32 = u32LAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DCMP_ADDR.u32)+ u32Data * GFX_OFFSET), G0_DCMP_ADDR.u32);
}

HI_VOID VDP_GFX_SetDcmpNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    U_G0_DCMP_NADDR G0_DCMP_NADDR;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerNAddr Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_NADDR.u32 = u32NAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DCMP_NADDR.u32)+ u32Data * GFX_OFFSET), G0_DCMP_NADDR.u32);
}

HI_VOID VDP_GFX_SetDcmpOffset(HI_U32 u32Data, HI_U32 u32Offset)
{
    U_G0_DCMP_OFFSET G0_DCMP_OFFSET;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDcmpOffset Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_DCMP_OFFSET.u32 = u32Offset;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DCMP_OFFSET.u32)+ u32Data * GFX_OFFSET), G0_DCMP_OFFSET.u32);
}

HI_VOID  VDP_GFX_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
    U_G0_DOF_STEP  G0_DOF_STEP;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetThreeDimDofStep() Select Wrong Video Layer ID\n");
        return ;
    }
    G0_DOF_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_STEP.u32))+ u32Data * GFX_OFFSET);
    G0_DOF_STEP.bits.right_step= s32RStep;
    G0_DOF_STEP.bits.left_step = s32LStep;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DOF_STEP.u32)+ u32Data * GFX_OFFSET), G0_DOF_STEP.u32);
}

HI_VOID  VDP_GFX_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
    U_G0_DOF_CTRL  G0_DOF_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetThreeDimDofEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_DOF_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_CTRL.u32))+ u32Data * GFX_OFFSET);
    G0_DOF_CTRL.bits.dof_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DOF_CTRL.u32)+ u32Data * GFX_OFFSET), G0_DOF_CTRL.u32);
}

HI_VOID  VDP_GFX_SetThreeDimDofBkg   (HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_G0_DOF_BKG   G0_DOF_BKG  ; 
    U_G0_DOF_ALPHA G0_DOF_ALPHA; 

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetThreeDimDofBkg() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_DOF_BKG.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_BKG.u32) + u32Data * GFX_OFFSET));
    G0_DOF_BKG.bits.dof_bk_y  = stBkg.u32BkgY << 2;
    G0_DOF_BKG.bits.dof_bk_cb = stBkg.u32BkgU << 2;
    G0_DOF_BKG.bits.dof_bk_cr = stBkg.u32BkgV << 2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DOF_BKG.u32) + u32Data * GFX_OFFSET), G0_DOF_BKG.u32); 

    G0_DOF_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_ALPHA.u32) + u32Data * GFX_OFFSET));
    G0_DOF_ALPHA.bits.dof_bk_alpha = stBkg.u32BkgA;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_DOF_ALPHA.u32) + u32Data * GFX_OFFSET), G0_DOF_ALPHA.u32); 

    return ;
}

HI_VOID  VDP_GFX_SetFlipEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetFlipEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.flip_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32); 
}

HI_VOID VDP_GFX_SetPreMultEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    U_G0_CBMPARA G0_CBMPARA;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_SetGfxPreMultEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.premult_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32); 
}

HI_VOID  VDP_GFX_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_G0_BK    G0_BK;
    U_G0_ALPHA G0_ALPHA;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerBkg() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_BK.u32) + u32Data * GFX_OFFSET));
    G0_BK.bits.vbk_y  = stBkg.u32BkgY*4;//0x0;  //
    G0_BK.bits.vbk_cb = stBkg.u32BkgU*4;//0x200;//
    G0_BK.bits.vbk_cr = stBkg.u32BkgV*4;//0x200;//
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_BK.u32) + u32Data * GFX_OFFSET), G0_BK.u32); 

    G0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_ALPHA.u32) + u32Data * GFX_OFFSET));
    G0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_ALPHA.u32) + u32Data * GFX_OFFSET), G0_ALPHA.u32); 

    return ;
}

HI_VOID  VDP_GFX_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    U_G0_CBMPARA G0_CBMPARA;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerGalpha() Select Wrong Video Layer ID\n");
        return ;
    }


    G0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32) + u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.galpha = u32Alpha0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32) + u32Data * GFX_OFFSET), G0_CBMPARA.u32); 

    return ;
}

HI_VOID VDP_GFX_SetPalpha(HI_U32 u32Data, HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1)
{


    U_G0_CBMPARA G0_CBMPARA;
    U_G0_CKEYMIN G0_CKEYMIN;
    U_G0_CKEYMAX G0_CKEYMAX;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetPalpha() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.palpha_en = bAlphaEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);


    G0_CKEYMIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET));                
    G0_CKEYMIN.bits.va1 = u32Alpha1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CKEYMIN.u32)+ u32Data * GFX_OFFSET), G0_CKEYMIN.u32);

    G0_CKEYMAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET));                
    G0_CKEYMAX.bits.va0 = u32Alpha0;                            
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CKEYMAX.u32)+ u32Data * GFX_OFFSET), G0_CKEYMAX.u32); 

    G0_CBMPARA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_CBMPARA.bits.palpha_range = bArange;            
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET), G0_CBMPARA.u32);

}

HI_VOID VDP_GFX_SetLayerNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    U_G0_NADDR G0_NADDR;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetLayerNAddr() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_NADDR.u32 = u32NAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_NADDR.u32)+ u32Data * GFX_OFFSET), G0_NADDR.u32);

}

HI_VOID  VDP_GFX_GetMuteEnable(HI_U32 u32Data, HI_U32 *pbEnable )
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_GetMuteEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    *pbEnable = G0_CTRL.bits.mute_en; 
}

HI_VOID  VDP_GFX_SetMuteEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetMuteEnable() Select Wrong Graph Layer ID\n");
        return ;
    }
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.mute_en = bEnable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 
}

HI_VOID  VDP_GFX_SetUpdMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    U_G0_CTRL G0_CTRL;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_Gfx_SetUpdMode() Select Wrong Graph Layer ID\n");
        return ;
    }

    /* G0 layer register update mode */
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET));
    G0_CTRL.bits.upd_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)+ u32Data * GFX_OFFSET), G0_CTRL.u32); 
}


HI_VOID  VDP_GFX_SetRegUp (HI_U32 u32Data)
{
    U_G0_UPD G0_UPD;

    /* GO layer register update */
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetRegup() Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_UPD.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_UPD.u32) + u32Data * GFX_OFFSET), G0_UPD.u32); 

    return ;
}
//---------------------------------------------------------------------------
//driver for GP layer
//---------------------------------------------------------------------------

#if GP0_EN
HI_VOID  VDP_GP_SetVideoPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_GP0_VFPOS GP0_VFPOS;
    U_GP0_VLPOS GP0_VLPOS;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetVideoPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    GP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET));
    GP0_VFPOS.bits.video_xfpos = stRect.u32X;
    GP0_VFPOS.bits.video_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_VFPOS.u32) + u32Data * GP_OFFSET), GP0_VFPOS.u32); 

    GP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET));
    GP0_VLPOS.bits.video_xlpos = stRect.u32X + stRect.u32Wth - 1;
    GP0_VLPOS.bits.video_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_VLPOS.u32) + u32Data * GP_OFFSET), GP0_VLPOS.u32); 
    return ;
}   

HI_VOID  VDP_GP_SetDispPos     (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_GP0_DFPOS GP0_DFPOS;
    U_GP0_DLPOS GP0_DLPOS;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetDispPos() Select Wrong Video Layer ID\n");
        return ;
    }

    /*video position */ 
    GP0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET));
    GP0_DFPOS.bits.disp_xfpos = stRect.u32X;
    GP0_DFPOS.bits.disp_yfpos = stRect.u32Y;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_DFPOS.u32) + u32Data * GP_OFFSET), GP0_DFPOS.u32); 

    GP0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET));
    GP0_DLPOS.bits.disp_xlpos = stRect.u32X + stRect.u32Wth - 1;
    GP0_DLPOS.bits.disp_ylpos = stRect.u32Y + stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_DLPOS.u32) + u32Data * GP_OFFSET), GP0_DLPOS.u32); 
    return ;
}   

HI_VOID  VDP_GP_SetInReso (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_GP0_IRESO GP0_IRESO;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetInReso() Select Wrong GP Layer ID\n");
        return ;
    }
    GP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET));
    GP0_IRESO.bits.iw = stRect.u32Wth - 1;
    GP0_IRESO.bits.ih = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_IRESO.u32) + u32Data * GP_OFFSET), GP0_IRESO.u32); 

    return ;
}  

HI_VOID  VDP_GP_SetOutReso (HI_U32 u32Data, VDP_RECT_S  stRect)
{
    U_GP0_ORESO GP0_ORESO;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetOutReso() Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_ORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET));
    GP0_ORESO.bits.ow = stRect.u32Wth - 1;
    GP0_ORESO.bits.oh = stRect.u32Hgt - 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ORESO.u32) + u32Data * GP_OFFSET), GP0_ORESO.u32); 

    return ;
}  

HI_VOID VDP_GP_SetIpOrder (HI_U32 u32Data, HI_U32 u32Chn, VDP_GP_ORDER_E enIpOrder)
{
    U_GP0_CTRL GP0_CTRL ;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetIpOrder() Select Wrong GP Layer ID\n");
        return ;
    }
    if((u32Data == VDP_LAYER_GP0) && (u32Chn == 1))
    {
        VDP_PRINT("Error,VDP_LAYER_GP0 have no channel 1\n");
        return;
    }

    GP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET));

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
                    GP0_CTRL.bits.mux1_sel = 3;
                    GP0_CTRL.bits.mux2_sel = 2;
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
                    GP0_CTRL.bits.mux1_sel = 0;
                    GP0_CTRL.bits.mux2_sel = 1;
                    GP0_CTRL.bits.aout_sel = 2;
                    GP0_CTRL.bits.bout_sel = 3;                    
                    break;
                }
            default://null
                {
                    GP0_CTRL.bits.mux1_sel = 3;
                    GP0_CTRL.bits.mux2_sel = 2;
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
                    GP0_CTRL.bits.mux1_sel = 3;
                    GP0_CTRL.bits.mux2_sel = 2;
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
//                     GP0_CTRL.bits.mux1_sel = 1;
//                     GP0_CTRL.bits.mux2_sel = 2;
//                     GP0_CTRL.bits.aout_sel = 0;
//                     GP0_CTRL.bits.bout_sel = 3;

                    GP0_CTRL.bits.mux1_sel = 0;
                    GP0_CTRL.bits.mux2_sel = 1;
                    GP0_CTRL.bits.aout_sel = 2;
                    GP0_CTRL.bits.bout_sel = 3;                    

                    break;
                }
            case VDP_GP_ORDER_ZME_CSC:
                {
//                    GP0_CTRL.bits.mux1_sel = 3;
//                    GP0_CTRL.bits.mux2_sel = 1;
//                    GP0_CTRL.bits.aout_sel = 0;
//                    GP0_CTRL.bits.bout_sel = 2;
                   
                    GP0_CTRL.bits.mux1_sel = 0;
                    GP0_CTRL.bits.mux2_sel = 1;
                    GP0_CTRL.bits.aout_sel = 2;
                    GP0_CTRL.bits.bout_sel = 3;      
                    break;
                }
            default://null
                {
                    GP0_CTRL.bits.mux1_sel = 3;
                    GP0_CTRL.bits.mux2_sel = 2;
                    GP0_CTRL.bits.aout_sel = 0;
                    GP0_CTRL.bits.bout_sel = 1;

                    break;
                }
        }
    }
    else
    {
        VDP_PRINT("Error,VDP_GP_SetIpOrder() Select Wrong GP Channel\n");
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET), GP0_CTRL.u32);
}

HI_VOID VDP_GP_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    U_GP0_CTRL GP0_CTRL;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetReadMode Select Wrong Graph Layer ID\n");
        return ;
    }

    GP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET));
    GP0_CTRL.bits.read_mode = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)+ u32Data * GP_OFFSET), GP0_CTRL.u32); 
}

HI_VOID  VDP_GP_SetParaUpd       (HI_U32 u32Data, VDP_GP_PARA_E enMode)
{
    U_GP0_PARAUP GP0_PARAUP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("error,VDP_GP_SetParaUpd() select wrong GP layer id\n");
        return ;
    }

    GP0_PARAUP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_PARAUP.u32) + u32Data * GP_OFFSET));

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
        VDP_PRINT("error,VDP_GP_SetParaUpd() select wrong mode!\n");
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_PARAUP.u32) + u32Data * GP_OFFSET), GP0_PARAUP.u32); 
    return ;
}


HI_VOID  VDP_GP_SetRegUp  (HI_U32 u32Data)
{
    U_GP0_UPD GP0_UPD;

    /* GP layer register update */
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetRegUp() Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_UPD.bits.regup = 0x1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_UPD.u32) + u32Data * GP_OFFSET), GP0_UPD.u32); 

    return ;
}

HI_VOID  VDP_GP_GetLayerGalpha (HI_U32 u32Data, HI_U32 *pu32Alpha)
{
    U_GP0_GALPHA GP0_GALPHA;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_GetLayerGalpha() Select Wrong gp Layer ID\n");
        return ;
    }


    GP0_GALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_GALPHA.u32) + u32Data * GP_OFFSET));
    *pu32Alpha = GP0_GALPHA.u32;
  
    return ;
}

HI_VOID  VDP_GP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    U_GP0_GALPHA GP0_GALPHA;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetLayerGalpha() Select Wrong gp Layer ID\n");
        return ;
    }


    GP0_GALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_GALPHA.u32) + u32Data * GP_OFFSET));
    GP0_GALPHA.bits.galpha = u32Alpha;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_GALPHA.u32) + u32Data * GP_OFFSET), GP0_GALPHA.u32); 

    return ;
}

HI_VOID  VDP_GP_SetLayerBkg(HI_U32 u32Data, VDP_BKG_S stBkg)
{
    U_GP0_BK GP0_BK;
    U_GP0_ALPHA     GP0_ALPHA;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetLayerBkg() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_BK.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_BK.u32) + u32Data * GP_OFFSET));
    GP0_BK.bits.vbk_y  = stBkg.u32BkgY;//0x0;  //
    GP0_BK.bits.vbk_cb = stBkg.u32BkgU;//0x200;//
    GP0_BK.bits.vbk_cr = stBkg.u32BkgV;//0x200;//
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_BK.u32) + u32Data * GP_OFFSET), GP0_BK.u32); 

    GP0_ALPHA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ALPHA.u32) + u32Data * GP_OFFSET));
    GP0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ALPHA.u32) + u32Data * GP_OFFSET), GP0_ALPHA.u32); 

    return ;
}

HI_VOID  VDP_GP_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    U_GP0_CSC_IDC  GP0_CSC_IDC;
    U_GP0_CSC_ODC  GP0_CSC_ODC;
    U_GP0_CSC_IODC GP0_CSC_IODC;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetCscDcCoef() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
    GP0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET), GP0_CSC_IDC.u32); 

    GP0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_ODC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
    GP0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_ODC.u32) + u32Data * GP_OFFSET), GP0_CSC_ODC.u32); 

    GP0_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_IODC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
    GP0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_IODC.u32) + u32Data * GP_OFFSET), GP0_CSC_IODC.u32); 

    return ;
}

HI_VOID   VDP_GP_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{   
    U_GP0_CSC_P0 GP0_CSC_P0;
    U_GP0_CSC_P1 GP0_CSC_P1;
    U_GP0_CSC_P2 GP0_CSC_P2;
    U_GP0_CSC_P3 GP0_CSC_P3;
    U_GP0_CSC_P4 GP0_CSC_P4;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetCscCoef Select Wrong GPeo ID\n");
        return ;
    }


    GP0_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_P0.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    GP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_P0.u32)+u32Data*GP_OFFSET), GP0_CSC_P0.u32);

    GP0_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_P1.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    GP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_P1.u32)+u32Data*GP_OFFSET), GP0_CSC_P1.u32);

    GP0_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_P2.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    GP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_P2.u32)+u32Data*GP_OFFSET), GP0_CSC_P2.u32);

    GP0_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_P3.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    GP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_P3.u32)+u32Data*GP_OFFSET), GP0_CSC_P3.u32);

    GP0_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_P4.u32)+u32Data*GP_OFFSET));
    GP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_P4.u32)+u32Data*GP_OFFSET), GP0_CSC_P4.u32);

}

HI_VOID  VDP_GP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    U_GP0_CSC_IDC GP0_CSC_IDC;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetCscEnable() Select Wrong GPeo Layer ID\n");
        return ;
    }

    GP0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET));
    GP0_CSC_IDC.bits.csc_en = 0;//u32bCscEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_IDC.u32) + u32Data * GP_OFFSET), GP0_CSC_IDC.u32); 

    return ;
}

HI_VOID VDP_GP_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)
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

        st_csc_dc_coef.csc_out_dc0 =  16 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc2 = 128 * u32DcPre;
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

        st_csc_dc_coef.csc_out_dc0 = 16  * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
        st_csc_dc_coef.csc_out_dc2 = 128 * u32DcPre;
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
    else if(enCscMode == VDP_CSC_YUV2YUV_MAX)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(31 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(31 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(31 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(31 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = 255 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = 255 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = 255 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = 255 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = 255 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = 255 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_MIN)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(-31 * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(-31 * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(-31 * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(-31 * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = -255 * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = -255 * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = -255 * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = -255 * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = -255 * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = -255 * u32DcPre;
    }
    else if(enCscMode == VDP_CSC_YUV2YUV_RAND)
    {
        st_csc_coef.csc_coef00     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef01     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef02     = (HI_S32)(sGetRand(-31,31) * u32Pre);

        st_csc_coef.csc_coef10     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef11     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef12     = (HI_S32)(sGetRand(-31,31) * u32Pre);

        st_csc_coef.csc_coef20     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef21     = (HI_S32)(sGetRand(-31,31) * u32Pre);
        st_csc_coef.csc_coef22     = (HI_S32)(sGetRand(-31,31) * u32Pre);

        st_csc_dc_coef.csc_in_dc2  = sGetRand(-255,255) * u32DcPre;
        st_csc_dc_coef.csc_in_dc1  = sGetRand(-255,255) * u32DcPre;
        st_csc_dc_coef.csc_in_dc0  = sGetRand(-255,255) * u32DcPre;

        st_csc_dc_coef.csc_out_dc2 = sGetRand(-255,255) * u32DcPre;
        st_csc_dc_coef.csc_out_dc1 = sGetRand(-255,255) * u32DcPre;
        st_csc_dc_coef.csc_out_dc0 = sGetRand(-255,255) * u32DcPre;
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

    VDP_GP_SetCscCoef  (u32Data,st_csc_coef);
    VDP_GP_SetCscDcCoef(u32Data,st_csc_dc_coef);

    return ;
}    

HI_VOID VDP_GP_SetZmeEnable  (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_GP0_ZME_HSP GP0_ZME_HSP;
    U_GP0_ZME_VSP GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /*GP zoom enable */
    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hsc_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vsc_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }

    return ;
}

HI_VOID VDP_GP_SetZmePhase   (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_S32 s32Phase)
{
    U_GP0_ZME_HLOFFSET  GP0_ZME_HLOFFSET;
    U_GP0_ZME_HCOFFSET  GP0_ZME_HCOFFSET;
    U_GP0_ZME_VOFFSET  GP0_ZME_VOFFSET;
    
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    /* GP zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HLOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HLOFFSET.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HLOFFSET.bits.hor_loffset = 0;//s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HLOFFSET.u32) + u32Data * GP_OFFSET), GP0_ZME_HLOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HCOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HCOFFSET.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HCOFFSET.bits.hor_coffset = 0;//s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HCOFFSET.u32) + u32Data * GP_OFFSET), GP0_ZME_HCOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VOFFSET.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vbtm_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VOFFSET.u32) + u32Data * GP_OFFSET), GP0_ZME_VOFFSET.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VOFFSET.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vtp_offset = s32Phase;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VOFFSET.u32) + u32Data * GP_OFFSET), GP0_ZME_VOFFSET.u32); 
    }

    return ;
}

HI_VOID VDP_GP_SetZmeFirEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_GP0_ZME_HSP        GP0_ZME_HSP;
    U_GP0_ZME_VSP        GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hafir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vafir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vfir_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }

    return ;
}

HI_VOID VDP_GP_SetZmeMidEnable   (HI_U32 u32Data, VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    U_GP0_ZME_HSP        GP0_ZME_HSP;
    U_GP0_ZME_VSP        GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hamid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hlmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_HSP.bits.hchmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vamid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vlmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
        GP0_ZME_VSP.bits.vchmid_en = u32bEnable;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 
    }

    return ;
}

HI_VOID VDP_GP_SetZmeHorRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_HSP.bits.hratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 

    return ;
}

HI_VOID VDP_GP_SetZmeVerRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    U_GP0_ZME_VSR        GP0_ZME_VSR;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSR.u32) + u32Data * GP_OFFSET));
    GP0_ZME_VSR.bits.vratio = u32Ratio;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSR.u32) + u32Data * GP_OFFSET), GP0_ZME_VSR.u32); 

    return ;
}
//modify add 
HI_VOID  VDP_GP_SetVfirOneTapEnable(HI_U32 u32Data, HI_U32 u32VfirOneTapEn)
{
    U_GP0_ZME_VSP GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetVfirOneTapEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_VSP.bits.vfir_1tap_en = u32VfirOneTapEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 

    return ;
}


HI_VOID  VDP_GP_SetVfirTwoTapEnable(HI_U32 u32Data, HI_U32 u32VfirTwoTapEn)
{
    U_GP0_ZME_VSP GP0_ZME_VSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetVfirTwoTapEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_VSP.bits.vfir_2tap_en = u32VfirTwoTapEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + u32Data * GP_OFFSET), GP0_ZME_VSP.u32); 

    return ;
}

HI_VOID VDP_GP_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeHfirOrder() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET));
    GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + u32Data * GP_OFFSET), GP0_ZME_HSP.u32); 

    return ;
}

HI_VOID VDP_GP_SetZmeCoefAddr  (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    U_GP0_HCOEFAD    GP0_HCOEFAD;
    U_GP0_VCOEFAD    GP0_VCOEFAD;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_GP_PARA_ZME_HOR)
    {
        GP0_HCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_HCOEFAD.u32) + u32Data * GP_OFFSET));
        GP0_HCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_HCOEFAD.u32) + u32Data * GP_OFFSET), GP0_HCOEFAD.u32); 
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VER)
    {
        GP0_VCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VCOEFAD.u32) + u32Data * GP_OFFSET));
        GP0_VCOEFAD.bits.coef_addr = u32Addr;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_VCOEFAD.u32) + u32Data * GP_OFFSET), GP0_VCOEFAD.u32); 
    }
    else
    {
        VDP_PRINT("Error,VDP_GP_SetZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}

HI_VOID VDP_GP_SetParaRd   (HI_U32 u32Data, VDP_GP_PARA_E enMode)
{
    U_GP0_PARARD GP0_PARARD;

    GP0_PARARD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_PARARD.u32) + u32Data * GP_OFFSET));

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetParaRd() Select Wrong Video Layer ID\n");
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
        VDP_PRINT("error,VDP_GP_SetParaRd() select wrong mode!\n");
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_PARARD.u32) + u32Data * GP_OFFSET), GP0_PARARD.u32); 
    return ;
}

HI_VOID  VDP_GP_SetTiEnable(HI_U32 u32Data, HI_U32 u32Md,HI_U32 u32Data1)
{
    U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiEnable() Select Wrong Graph Layer ID\n");
        return ;
    }

	if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
	{
		GP0_ZME_LTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
		GP0_ZME_LTICTRL.bits.lti_en = 0;//u32Data1;//no Gti in HiFoneB02
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32);

	}
	if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
	{
		GP0_ZME_CTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
		GP0_ZME_CTICTRL.bits.cti_en = 0;//u32Data1;//no Gti in HiFoneB02
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32);
	}

}

HI_VOID  VDP_GP_SetTiGainRatio(HI_U32 u32Data, HI_U32 u32Md, HI_S32 s32Data)
{
    U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_SetTiGainRatio() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lgain_ratio = s32Data;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32); 
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cgain_ratio = s32Data;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32); 
    }

}

HI_VOID  VDP_GP_SetTiMixRatio(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32mixing_ratio)

{
    U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiMixRatio() Select Wrong Graph Layer ID\n");
        return ;
    }


    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lmixing_ratio = u32mixing_ratio;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32); 
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cmixing_ratio = u32mixing_ratio;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32); 
    }

}

HI_VOID  VDP_GP_SetTiHfThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32TiHfThd)
{
    U_GP0_ZME_LHFREQTHD GP0_ZME_LHFREQTHD;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiHfThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LHFREQTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LHFREQTHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd0 = u32TiHfThd[0];
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd1 = u32TiHfThd[1];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LHFREQTHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LHFREQTHD.u32); 
    }

}

HI_VOID  VDP_GP_SetTiHpCoef(HI_U32 u32Data, HI_U32 u32Md, HI_S32 * s32Data)
{
    U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    U_GP0_ZME_LHPASSCOEF GP0_ZME_LHPASSCOEF;
    U_GP0_ZME_CHPASSCOEF GP0_ZME_CHPASSCOEF;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiHpCoef() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHPASSCOEF.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LHPASSCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef0 = s32Data[0];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef1 = s32Data[1];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef2 = s32Data[2];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef3 = s32Data[3];
        GP0_ZME_LTICTRL.bits.lhpass_coef4    = s32Data[4];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTICTRL.u32); 
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LHPASSCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_LHPASSCOEF.u32); 
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CHPASSCOEF.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CHPASSCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CHPASSCOEF.bits.chpass_coef0 = s32Data[0];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef1 = s32Data[1];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef2 = s32Data[2];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTICTRL.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTICTRL.u32); 
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CHPASSCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_CHPASSCOEF.u32); 
    }
}

HI_VOID  VDP_GP_SetTiCoringThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd)
{
    U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiCoringThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTITHD.bits.lcoring_thd = u32thd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTITHD.u32); 
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTITHD.bits.ccoring_thd = u32thd;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTITHD.u32); 
    }

}

HI_VOID  VDP_GP_SetTiSwingThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd, HI_U32 u32thd1)//have problem
{
    U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,Vou_SetTiSwingThd() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LTITHD.bits.lover_swing  = u32thd;
        GP0_ZME_LTITHD.bits.lunder_swing = u32thd1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_LTITHD.u32); 
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_CTITHD.bits.cover_swing  = u32thd;
        GP0_ZME_CTITHD.bits.cunder_swing = u32thd1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_CTITHD.u32)+ u32Data * GP_OFFSET), GP0_ZME_CTITHD.u32); 
    }

}

HI_VOID  VDP_GP_SetTiGainCoef(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32coef)
{
    U_GP0_ZME_LGAINCOEF GP0_ZME_LGAINCOEF;
    if(u32Data >= GP_MAX)
    {
        VDP_PRINT("Error,VDP_GP_SetTiGainCoef() Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LGAINCOEF.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_LGAINCOEF.u32)+ u32Data * GP_OFFSET));
        GP0_ZME_LGAINCOEF.bits.lgain_coef0 = u32coef[0];
        GP0_ZME_LGAINCOEF.bits.lgain_coef1 = u32coef[1];
        GP0_ZME_LGAINCOEF.bits.lgain_coef2 = u32coef[2];
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_ZME_LGAINCOEF.u32)+ u32Data * GP_OFFSET), GP0_ZME_LGAINCOEF.u32); 
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
        VDP_PRINT("Error,VDP_GP_SetTiDefThd() Select Wrong Graph Layer ID\n");
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

#if 1
//mod me
HI_VOID VDP_GFX_SetConnection(HI_U32 u32LayerId, HI_U32 u32Data )
{
    U_VOCTRL  VOCTRL;

	if(u32LayerId != VDP_LAYER_GFX3)
    {
        VDP_PRINT("Error, VDP_GFX_SetConnection() select wrong layer id\n");
        return ;

    }

    //if(u32Data == VDP_LAYER_CONN_DHD0) 
	//u32Data = VDP_LAYER_CONN_DHD1;

    VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32)));
    VOCTRL.bits.g3_sel = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);

}
#endif

#if GFX_CSC_EN
HI_VOID  VDP_GFX_SetCscDcCoef   (HI_U32 u32Data, VDP_CSC_DC_COEF_S pstCscCoef)
{
    U_G0_CSC_IDC  G0_CSC_IDC;
    U_G0_CSC_ODC  G0_CSC_ODC;
    U_G0_CSC_IODC G0_CSC_IODC;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetCscDcCoef() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET));
    G0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
    G0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET), G0_CSC_IDC.u32); 

    G0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_ODC.u32) + u32Data * GFX_OFFSET));
    G0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
    G0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_ODC.u32) + u32Data * GFX_OFFSET), G0_CSC_ODC.u32); 

    G0_CSC_IODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_IODC.u32) + u32Data * GFX_OFFSET));
    G0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
    G0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_IODC.u32) + u32Data * GFX_OFFSET), G0_CSC_IODC.u32); 

    return ;
}

HI_VOID   VDP_GFX_SetCscCoef(HI_U32 u32Data, VDP_CSC_COEF_S stCscCoef)
{   
    U_G0_CSC_P0 G0_CSC_P0;
    U_G0_CSC_P1 G0_CSC_P1;
    U_G0_CSC_P2 G0_CSC_P2;
    U_G0_CSC_P3 G0_CSC_P3;
    U_G0_CSC_P4 G0_CSC_P4;
    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetCscCoef Select Wrong Gfx ID\n");
        return ;
    }


    G0_CSC_P0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_P0.u32)+u32Data*GFX_OFFSET));
    G0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    G0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_P0.u32)+u32Data*GFX_OFFSET), G0_CSC_P0.u32);

    G0_CSC_P1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_P1.u32)+u32Data*GFX_OFFSET));
    G0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    G0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_P1.u32)+u32Data*GFX_OFFSET), G0_CSC_P1.u32);

    G0_CSC_P2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_P2.u32)+u32Data*GFX_OFFSET));
    G0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    G0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_P2.u32)+u32Data*GFX_OFFSET), G0_CSC_P2.u32);

    G0_CSC_P3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_P3.u32)+u32Data*GFX_OFFSET));
    G0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    G0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_P3.u32)+u32Data*GFX_OFFSET), G0_CSC_P3.u32);

    G0_CSC_P4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_P4.u32)+u32Data*GFX_OFFSET));
    G0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_P4.u32)+u32Data*GFX_OFFSET), G0_CSC_P4.u32);

}

HI_VOID  VDP_GFX_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
    U_G0_CSC_IDC G0_CSC_IDC;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetCscEnable() Select Wrong Gfx Layer ID\n");
        return ;
    }

    G0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET));
    G0_CSC_IDC.bits.csc_en = u32bCscEn;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET), G0_CSC_IDC.u32); 

    return ;
}

HI_VOID VDP_GFX_SetCscMode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode)
{
    VDP_CSC_COEF_S    st_csc_coef;
    VDP_CSC_DC_COEF_S st_csc_dc_coef;
    //U_G0_CSC_IDC G0_CSC_IDC;

    HI_S32 u32Pre   = 1 << 10;
    HI_S32 u32DcPre = 4;//1:8bit; 4:10bit

    //csc coef solid mode
    //if(u32Data == VDP_LAYER_GFX2)
    //{

    //    G0_CSC_IDC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET));
    //    G0_CSC_IDC.bits.csc_mode = enCscMode;
    //    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CSC_IDC.u32) + u32Data * GFX_OFFSET), G0_CSC_IDC.u32);
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

    VDP_GFX_SetCscCoef  (u32Data,st_csc_coef);
    VDP_GFX_SetCscDcCoef(u32Data,st_csc_dc_coef);

    return ;
}    
#endif

HI_VOID VDP_GP_SetCoefReadEnable(HI_U32 u32Id, HI_U32 u32Para)
{
    HI_U32 u32WrData;
    u32WrData = 1 << u32Para; 

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_PARARD.u32) + u32Id*4), u32WrData);
}

HI_VOID VDP_GP_SetCoefReadDisable(HI_U32 u32Id, HI_U32 u32Para)
{

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_PARARD.u32) + u32Id*4), 0);
}

//WCG_BEGING
HI_VOID VDP_GP_SetWcgEnable            (HI_U32 u32Data, HI_U32 u32Enable)
{
	U_GP0_WCG_CSC2CTRL     GP0_WCG_CSC2CTRL;
	U_GP0_CSC_ODC          GP0_CSC_ODC ;
	
	if(u32Data >= GP_MAX)
	{
		VDP_PRINT("Error,VDP_GP_SetWcgEnable() Select Wrong GP ID\n");
		return ;
	}
	
	GP0_WCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2CTRL.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2CTRL.bits.csc2_en = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2CTRL.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2CTRL.u32);
	  
	//set csc sign mode
	GP0_CSC_ODC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CSC_ODC.u32)+ u32Data * GP_OFFSET));
	GP0_CSC_ODC.bits.csc_sign_mode = u32Enable;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CSC_ODC.u32)+ u32Data * GP_OFFSET), GP0_CSC_ODC.u32);

}


HI_VOID VDP_GP_SetWcgCoef              (HI_U32 u32Data, VDP_WCG_COEF_S *pstWcgCoef)
{
	U_GP0_WCG_NON2LCTRL    GP0_WCG_NON2LCTRL;
    U_GP0_WCG_NON2LCOEF0   GP0_WCG_NON2LCOEF0;
    U_GP0_WCG_NON2LCOEF1   GP0_WCG_NON2LCOEF1;
    U_GP0_WCG_NON2LCOEF2   GP0_WCG_NON2LCOEF2;

    U_GP0_WCG_CSC2CTRL     GP0_WCG_CSC2CTRL;
    U_GP0_WCG_CSC2COEF0    GP0_WCG_CSC2COEF0;
    U_GP0_WCG_CSC2COEF1    GP0_WCG_CSC2COEF1;
    U_GP0_WCG_CSC2COEF2    GP0_WCG_CSC2COEF2;
    U_GP0_WCG_CSC2COEF3    GP0_WCG_CSC2COEF3;

    U_GP0_WCG_L2NONTHD     GP0_WCG_L2NONTHD;
    U_GP0_WCG_L2NONCOEF0   GP0_WCG_L2NONCOEF0;
    U_GP0_WCG_L2NONCOEF1   GP0_WCG_L2NONCOEF1;
    U_GP0_WCG_L2NONCOEF2   GP0_WCG_L2NONCOEF2; 

	if(u32Data >= GP_MAX)
	{
		VDP_PRINT("Error,VDP_GP_SetWcgCoef() Select Wrong GP ID\n");
		return ;
	}

	GP0_WCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2CTRL.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2CTRL.bits.csc2_scale_sel = pstWcgCoef->csc2_scale_sel;
	GP0_WCG_CSC2CTRL.bits.csc2_r_r_coef  = (pstWcgCoef->csc2_r_r_coef);
	GP0_WCG_CSC2CTRL.bits.csc2_r_g_coef  = (pstWcgCoef->csc2_r_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2CTRL.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2CTRL.u32);

	GP0_WCG_CSC2COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF0.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2COEF0.bits.csc2_r_b_coef  = (pstWcgCoef->csc2_r_b_coef);
	GP0_WCG_CSC2COEF0.bits.csc2_g_r_coef  = (pstWcgCoef->csc2_g_r_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF0.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2COEF0.u32);

	GP0_WCG_CSC2COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF1.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2COEF1.bits.csc2_g_g_coef  = (pstWcgCoef->csc2_g_g_coef);
	GP0_WCG_CSC2COEF1.bits.csc2_g_b_coef  = (pstWcgCoef->csc2_g_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF1.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2COEF1.u32);

	GP0_WCG_CSC2COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF2.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2COEF2.bits.csc2_b_r_coef  = (pstWcgCoef->csc2_b_r_coef);
	GP0_WCG_CSC2COEF2.bits.csc2_b_g_coef  = (pstWcgCoef->csc2_b_g_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF2.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2COEF2.u32);

	GP0_WCG_CSC2COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF3.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_CSC2COEF3.bits.csc2_b_b_coef  = (pstWcgCoef->csc2_b_b_coef);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_CSC2COEF3.u32)+ u32Data * GP_OFFSET), GP0_WCG_CSC2COEF3.u32);

	//-------------------------

	GP0_WCG_NON2LCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCTRL.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_NON2LCTRL.bits.gma_lin_neg_scl  = pstWcgCoef->gma_lin_neg_scl;
	GP0_WCG_NON2LCTRL.bits.gma_lin_init_slp = pstWcgCoef->gma_lin_init_slp;
	GP0_WCG_NON2LCTRL.bits.gma_lin_thre_0   = pstWcgCoef->gma_lin_thre_0;
	GP0_WCG_NON2LCTRL.bits.gma_lin_thre_1   = pstWcgCoef->gma_lin_thre_1;
	GP0_WCG_NON2LCTRL.bits.gma_lin_thre_2   = pstWcgCoef->gma_lin_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCTRL.u32)+ u32Data * GP_OFFSET), GP0_WCG_NON2LCTRL.u32);

	GP0_WCG_NON2LCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF0.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_NON2LCOEF0.bits.gma_lin_coef_a_0  = pstWcgCoef->gma_lin_coef_a_0;
	GP0_WCG_NON2LCOEF0.bits.gma_lin_coef_b_0  = pstWcgCoef->gma_lin_coef_b_0;
	GP0_WCG_NON2LCOEF0.bits.gma_lin_coef_c_0  = pstWcgCoef->gma_lin_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF0.u32)+ u32Data * GP_OFFSET), GP0_WCG_NON2LCOEF0.u32);

	GP0_WCG_NON2LCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF1.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_NON2LCOEF1.bits.gma_lin_coef_a_1  = pstWcgCoef->gma_lin_coef_a_1;
	GP0_WCG_NON2LCOEF1.bits.gma_lin_coef_b_1  = pstWcgCoef->gma_lin_coef_b_1;
	GP0_WCG_NON2LCOEF1.bits.gma_lin_coef_c_1  = pstWcgCoef->gma_lin_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF1.u32)+ u32Data * GP_OFFSET), GP0_WCG_NON2LCOEF1.u32);

	GP0_WCG_NON2LCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF2.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_NON2LCOEF2.bits.gma_lin_coef_a_2  = pstWcgCoef->gma_lin_coef_a_2;
	GP0_WCG_NON2LCOEF2.bits.gma_lin_coef_b_2  = pstWcgCoef->gma_lin_coef_b_2;
	GP0_WCG_NON2LCOEF2.bits.gma_lin_coef_c_2  = pstWcgCoef->gma_lin_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_NON2LCOEF2.u32)+ u32Data * GP_OFFSET), GP0_WCG_NON2LCOEF2.u32);

	GP0_WCG_L2NONTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONTHD.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_L2NONTHD.bits.lin_gma_thre_0  = pstWcgCoef->lin_gma_thre_0;
	GP0_WCG_L2NONTHD.bits.lin_gma_thre_1  = pstWcgCoef->lin_gma_thre_1;
	GP0_WCG_L2NONTHD.bits.lin_gma_thre_2  = pstWcgCoef->lin_gma_thre_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONTHD.u32)+ u32Data * GP_OFFSET), GP0_WCG_L2NONTHD.u32);

	GP0_WCG_L2NONCOEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF0.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_L2NONCOEF0.bits.lin_gma_coef_a_0  = pstWcgCoef->lin_gma_coef_a_0;
	GP0_WCG_L2NONCOEF0.bits.lin_gma_coef_b_0  = pstWcgCoef->lin_gma_coef_b_0;
	GP0_WCG_L2NONCOEF0.bits.lin_gma_coef_c_0  = pstWcgCoef->lin_gma_coef_c_0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF0.u32)+ u32Data * GP_OFFSET), GP0_WCG_L2NONCOEF0.u32);

	GP0_WCG_L2NONCOEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF1.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_L2NONCOEF1.bits.lin_gma_coef_a_1  = pstWcgCoef->lin_gma_coef_a_1;
	GP0_WCG_L2NONCOEF1.bits.lin_gma_coef_b_1  = pstWcgCoef->lin_gma_coef_b_1;
	GP0_WCG_L2NONCOEF1.bits.lin_gma_coef_c_1  = pstWcgCoef->lin_gma_coef_c_1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF1.u32)+ u32Data * GP_OFFSET), GP0_WCG_L2NONCOEF1.u32);

	GP0_WCG_L2NONCOEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF2.u32)+ u32Data * GP_OFFSET));
	GP0_WCG_L2NONCOEF2.bits.lin_gma_coef_a_2  = pstWcgCoef->lin_gma_coef_a_2;
	GP0_WCG_L2NONCOEF2.bits.lin_gma_coef_b_2  = pstWcgCoef->lin_gma_coef_b_2;
	GP0_WCG_L2NONCOEF2.bits.lin_gma_coef_c_2  = pstWcgCoef->lin_gma_coef_c_2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_WCG_L2NONCOEF2.u32)+ u32Data * GP_OFFSET), GP0_WCG_L2NONCOEF2.u32);

}

#endif //end of GP0_EN

HI_VOID VDP_GFX_SetMasterSel  (HI_U32 u32Data, HI_U32 u32MasterNum)
{
	U_VOMASTERSEL   VOMASTERSEL;
	U_VOMASTERSEL1  VOMASTERSEL1;

    U_G0_CTRL       G0_CTRL;
    
	VOMASTERSEL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)));
	VOMASTERSEL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)));
	
    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));

    if(u32MasterNum == VDP_MASTER0)
    {
        G0_CTRL.bits.ofl_master = 0;
        if(u32Data == VDP_LAYER_GFX0)
        {
            VOMASTERSEL.bits.g0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX1)
        {
            VOMASTERSEL.bits.g1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX2)
        {
            VOMASTERSEL.bits.g2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX3)
        {
            VOMASTERSEL.bits.g3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX4)
        {
            VOMASTERSEL.bits.g4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX5)
        {
            VOMASTERSEL.bits.g5_master_sel = 0;
        }
        else
        {
            VDP_PRINT("Error! VDP_GFX_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER1)
    {
        G0_CTRL.bits.ofl_master = 0;
        if(u32Data == VDP_LAYER_GFX0)
        {
            VOMASTERSEL.bits.g0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX1)
        {
            VOMASTERSEL.bits.g1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX2)
        {
            VOMASTERSEL.bits.g2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX3)
        {
            VOMASTERSEL.bits.g3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX4)
        {
            VOMASTERSEL.bits.g4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX5)
        {
            VOMASTERSEL.bits.g5_master_sel = 1;
        }
        else
        {
            VDP_PRINT("Error! VDP_GFX_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER2)
    {
        G0_CTRL.bits.ofl_master = 1;
        if(u32Data == VDP_LAYER_GFX0)
        {
            VOMASTERSEL1.bits.g0_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX1)
        {
            VOMASTERSEL1.bits.g1_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX2)
        {
            VOMASTERSEL1.bits.g2_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX3)
        {
            VOMASTERSEL1.bits.g3_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX4)
        {
            VOMASTERSEL1.bits.g4_master_sel = 0;
        }
        else if(u32Data == VDP_LAYER_GFX5)
        {
            VOMASTERSEL1.bits.g5_master_sel = 0;
        }
        else
        {
            VDP_PRINT("Error! VDP_GFX_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }
    else if(u32MasterNum == VDP_MASTER3)
    {
        G0_CTRL.bits.ofl_master = 1;
        if(u32Data == VDP_LAYER_GFX0)
        {
            VOMASTERSEL1.bits.g0_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX1)
        {
            VOMASTERSEL1.bits.g1_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX2)
        {
            VOMASTERSEL1.bits.g2_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX3)
        {
            VOMASTERSEL1.bits.g3_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX4)
        {
            VOMASTERSEL1.bits.g4_master_sel = 1;
        }
        else if(u32Data == VDP_LAYER_GFX5)
        {
            VOMASTERSEL1.bits.g5_master_sel = 1;
        }
        else
        {
            VDP_PRINT("Error! VDP_GFX_SetM0AndM1Sel() Select Wrong layer ID\n");
        }
    }


	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)), VOMASTERSEL.u32); 
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VOMASTERSEL1.u32)), VOMASTERSEL1.u32); 

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);
    
    return ;

}

HI_VOID VDP_GFX_SetDataMask    (HI_U32 u32Data, HI_U32 u32Odd, HI_U32 u32Even)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= VID_MAX)
    {
        VDP_PRINT("Error, VDP_VID_SetDataMask() Select Wrong Video Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.odd_data_mute  = u32Odd ;
    G0_CTRL.bits.even_data_mute = u32Even ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32);

    return ;
}

HI_VOID VDP_GP_SetTwoChnEnable    (HI_U32 u32Enable)
{
    U_LVDS_PHYCTRL1 LVDS_PHYCTRL1;

    LVDS_PHYCTRL1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL1.u32)));
    LVDS_PHYCTRL1.bits.clkds1 = u32Enable;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->LVDS_PHYCTRL1.u32)), LVDS_PHYCTRL1.u32);

    return ;
}

HI_VOID VDP_GP_SetOutputMode    (HI_U32 u32Mode)
{
    //u32Mode 
    //0:normal, the same as process
    //1:dual channel process, single channel output
    U_GP0_CTRL GP0_CTRL;

    GP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)));
    GP0_CTRL.bits.mute_en = u32Mode;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)), GP0_CTRL.u32);

    return ;
}

//add by dcmp

HI_VOID VDP_GFX_SetDcmpLossLess(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDcmpLossLess Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.lossless = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 
}

HI_VOID VDP_GFX_SetDcmpAlphaLossLess(HI_U32 u32Data, HI_U32 u32bEnable)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDcmpLossLess Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.lossless_a = u32bEnable ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 
}

HI_VOID VDP_GFX_SetDcmpCmpMode (HI_U32 u32Data, HI_U32 u32Mode)
{
    U_G0_CTRL G0_CTRL;

    if(u32Data >= GFX_MAX)
    {
        VDP_PRINT("Error,VDP_GFX_SetDcmpCmpMode Select Wrong Graph Layer ID\n");
        return ;
    }

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_CTRL.bits.cmp_mode = u32Mode ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET), G0_CTRL.u32); 
}

HI_VOID VDP_GFX_MixvBypassEn (HI_U32 u32En)
{
    U_MIXG0_MIX_BYPASS MIXG0_MIX_BYPASS;



    MIXG0_MIX_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX_BYPASS.u32)));
    MIXG0_MIX_BYPASS.bits.layer0_bypass_en = u32En ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX_BYPASS.u32)), MIXG0_MIX_BYPASS.u32); 
}

HI_VOID VDP_GFX_MixvPremultEn (HI_U32 u32En)
{
    U_MIXG0_MIX_BYPASS MIXG0_MIX_BYPASS;



    MIXG0_MIX_BYPASS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXG0_MIX_BYPASS.u32)));
    MIXG0_MIX_BYPASS.bits.layer0_premulti = u32En ;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXG0_MIX_BYPASS.u32)), MIXG0_MIX_BYPASS.u32); 
}

HI_VOID VDP_DRV_CalculateTotalGfxPixel(HI_U32 *pu32TotalGfxPixel)
{
    volatile U_GP0_IRESO GP0_IRESO;
    GP0_IRESO.u32    = VDP_RegRead(((HI_ULONG)(&(pVdpReg->GP0_IRESO.u32)) + VDP_LAYER_GFX0 * GP_OFFSET));
    *pu32TotalGfxPixel = (GP0_IRESO.bits.iw + 1) * (GP0_IRESO.bits.ih + 1);
    return;
}

HI_VOID VDP_DRV_GetZeroAlphaSum(HI_U32 *pu32ZeroAlphaSum)
{
    volatile U_GP0_GALPHA_SUM GP0_GALPHA_SUM;
    GP0_GALPHA_SUM.u32 = VDP_RegRead((HI_ULONG)(&(pVdpReg->GP0_GALPHA_SUM.u32)));
    *pu32ZeroAlphaSum = GP0_GALPHA_SUM.u32;
    return;
}

HI_VOID VDP_DRV_GetGfxState(HI_BOOL *pbNeedProcessGfx)
{
    volatile U_G0_CTRL G0_CTRL;
    HI_U32 u32TotalPixels = 0;
    HI_U32 u32SumZeroAlpha = 0;

    G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + VDP_LAYER_GFX0 * GFX_OFFSET));
    
    if (HI_FALSE == G0_CTRL.bits.surface_en)
    {
        *pbNeedProcessGfx = HI_FALSE;
        return;
    }

    if (VDP_GFX_IFMT_ARGB_8888 != G0_CTRL.bits.ifmt)
    {
        *pbNeedProcessGfx = HI_FALSE;
        return;
    }

    VDP_DRV_CalculateTotalGfxPixel(&u32TotalPixels);
    VDP_DRV_GetZeroAlphaSum(&u32SumZeroAlpha);
    
    if (u32TotalPixels == u32SumZeroAlpha)
    {
        *pbNeedProcessGfx = HI_FALSE;
        return;
    }
        
    *pbNeedProcessGfx = HI_TRUE;
    return;
}

