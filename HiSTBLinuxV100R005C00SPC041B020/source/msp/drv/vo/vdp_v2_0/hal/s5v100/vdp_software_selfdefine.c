#ifdef HI_DISP_BUILD_FULL
#include <linux/kernel.h>
#endif

#include "vdp_reg.h"
#include "vdp_driver.h"
#include "vdp_define.h"
#include "drv_disp_rwzb.h"
#include "hi_reg_common.h"
#include "drv_disp_com.h"

extern S_VDP_REGS_TYPE *pVdpReg;


#define USE_LVDS 0

HI_VOID LVDS_Init(HI_VOID);


HI_VOID  VDP_DRIVER_SetVirtualAddr(HI_U32 virAddr)
{
    pVdpReg = (S_VDP_REGS_TYPE *)virAddr;
}

HI_VOID VDP_SetG3Position(HI_U32 u32G3Position)
{
    U_VOCTRL               VOCTRL;

    VOCTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VOCTRL.u32)));

    if (u32G3Position)
    {
        VOCTRL.bits.g3_sel = 1;
    }
    else
    {
        VOCTRL.bits.g3_sel = 0;
    }

    VDP_RegWrite((HI_U32)(&(pVdpReg->VOCTRL.u32)), VOCTRL.u32);

    return ;
}

HI_VOID  VDP_DRIVER_Initial()
{
    VDP_BKG_S stBkg;
    memset((void*)&stBkg, 0, sizeof(VDP_BKG_S));

    /*1st:  all the master's read and write outtanding.*/
    VDP_SetRdOutStd(VDP_MASTER0, 0, 7);
    VDP_SetRdOutStd(VDP_MASTER0, 1, 7);
    VDP_SetWrOutStd(VDP_MASTER0, 0, 7);

    VDP_SetRdOutStd(VDP_MASTER1, 0, 7);
    VDP_SetRdOutStd(VDP_MASTER1, 1, 7);
    VDP_SetWrOutStd(VDP_MASTER1, 0, 7);

    VDP_SetRdOutStd(VDP_MASTER2, 0, 7);
    VDP_SetRdOutStd(VDP_MASTER2, 1, 7);
    VDP_SetWrOutStd(VDP_MASTER2, 0, 7);

    VDP_SetRdOutStd(VDP_MASTER3, 0, 7);
    VDP_SetRdOutStd(VDP_MASTER3, 1, 7);
    VDP_SetWrOutStd(VDP_MASTER3, 0, 7);

    /*2nd:  allocate the master's for  all the vidlayer and gfx.*/
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_V0,1);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_V1,1);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_V3,1);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_V4,1);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_WBC0,0);

    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_G0,0);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_G1,0);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_G2,0);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_G3,0);
    VDP_SetM0AndM1Sel(VDP_LAYER_MASTER_G4,0);

    /*3rd: about cbm*/
    VDP_CBM_SetMixerBkg(VDP_CBM_MIXV0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIXG0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX1,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX2,stBkg);

    VDP_MIXV_SetPrio(VDP_LAYER_VID0);
    VDP_MIXV_SetPrio1(VDP_LAYER_VID1);


    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_VP0, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_GP0, 1);

    VDP_CBM_SetMixerPrio(VDP_CBM_MIX2, VDP_CBM_CBM, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX2, VDP_CBM_GFX3, 1);

    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID3, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID4, 1);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_GP1, 2);

    /*4¡¢wb location: cbm2*/
    VDP_WBC_SetConnection(VDP_LAYER_WBC_HD0, 1);

    /*5. g3 bind to MIXG0 */
    VDP_SetG3Position(1);

    /*DCI bind to VP*/
    VDP_VP_SetVpSel(0, 1);

    VDP_DHD_DEFAULT();
    VDP_DSD_DEFAULT();

 #if USE_LVDS
    LVDS_Init();
 #endif

    return;
}


/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
/*****************it's the beginning of  videolayer**************************************/
HI_VOID  VDP_VID_SetInReso2(HI_U32 u32Data, HI_RECT_S *pstRect, HI_RECT_S *pstRectOrigin)
{
    U_V0_IRESO V0_IRESO;
  //HI_PRINT("%s-u32Data:%d!\n", __FILE__,u32Data);
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

HI_VOID  VDP_VID_SetZmeFirEnable2(HI_U32 u32Data,HI_U32 u32bEnableHl,HI_U32 u32bEnableHc,HI_U32 u32bEnableVl,HI_U32 u32bEnableVc)
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

HI_VOID  VDP_VID_SetZmeVchTap(HI_U32 u32Data, HI_U32 u32VscTap)
{
    /*in s5v100 ,you can't set vsc_chroma_tap, so this func should
      fall into chip dir.*/
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

HI_VOID  VDP_VID_Layer4LinkToMix2( HI_BOOL bEnable )
{
    /*in s5v100, video and gfx layer could not switch to cbmixer1 or 2, or 3,
    * so this func here is null.
    */
    return;
}

HI_VOID  VDP_VID_MultiSetRegionReso(HI_U32 u32Data, HI_U32 u32RegionNo,HI_RECT_S stRect)
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

HI_VOID  VDP_VID_GetCscReg(HI_U32 u32Data, HI_U32 *pdata)
{
    pdata[0] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET));
    pdata[1] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET));
    pdata[2] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET));
    pdata[3] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P0.u32) + u32Data * VID_OFFSET));
    pdata[4] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P1.u32) + u32Data * VID_OFFSET));
    pdata[5] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P2.u32) + u32Data * VID_OFFSET));
    pdata[6] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P3.u32) + u32Data * VID_OFFSET));
    pdata[7] = VDP_RegRead((HI_U32)(&(pVdpReg->V0_CSC_P4.u32) + u32Data * VID_OFFSET));
}


HI_VOID  VDP_VID_SetCscReg(HI_U32 u32Data, HI_U32 *pdata)
{
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IDC.u32) + u32Data * VID_OFFSET), pdata[0]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_ODC.u32) + u32Data * VID_OFFSET), pdata[1]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_IODC.u32) + u32Data * VID_OFFSET), pdata[2]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P0.u32) + u32Data * VID_OFFSET), pdata[3]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P1.u32) + u32Data * VID_OFFSET), pdata[4]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P2.u32) + u32Data * VID_OFFSET), pdata[5]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P3.u32) + u32Data * VID_OFFSET), pdata[6]);
    VDP_RegWrite((HI_U32)(&(pVdpReg->V0_CSC_P4.u32) + u32Data * VID_OFFSET), pdata[7]);
}


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

/*****************it's the beginning of  disp**************************************/
/*****************it's the beginning of  disp**************************************/
/*****************it's the beginning of  disp**************************************/
/*****************it's the beginning of  disp**************************************/
/*****************it's the beginning of  disp**************************************/
/*****************it's the beginning of  disp**************************************/
HI_VOID VDP_DHD_DEFAULT( )
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
    volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;

    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)), DHD0_SYNC_INV.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);

    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32)), DHD0_DATA_SEL.u32);
}

HI_VOID VDP_DSD_DEFAULT( )
{
   volatile U_DHD0_CTRL DHD0_CTRL;
   volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
   volatile U_VO_MUX VO_MUX;
   volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;


    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;
    VO_MUX.u32= 0x1;


    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_SYNC_INV.u32)+ 1*CHN_OFFSET), DHD0_SYNC_INV.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+ 1*CHN_OFFSET), DHD0_CTRL.u32);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX.u32)), VO_MUX.u32);

    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_DATA_SEL.u32) + 1*CHN_OFFSET), DHD0_DATA_SEL.u32);
}

HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;

    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.intf_en = 0;
    DHD0_CTRL.bits.cbar_en = 0;
    DHD0_CTRL.bits.fpga_lmt_en = 0;
    DHD0_CTRL.bits.hdmi_mode = DHD_YUV_TO_HDMI;
    DHD0_CTRL.bits.gmm_en = 0;
    DHD0_CTRL.bits.disp_mode = 0;
    DHD0_CTRL.bits.regup = 1;

    /**s5v100 added, should confirm is this right.**/
    DHD0_CTRL.bits.twochn_debug = 0;
    DHD0_CTRL.bits.mirror_en = 0;

    VDP_RegWrite((HI_U32)(&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}

HI_VOID VDP_DISP_SetTiming(HI_U32 u32hd_id,VDP_DISP_SYNCINFO_S *pstSyncInfo)
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

        {
            DHD0_HSYNC1.bits.hact = pstSyncInfo->u32Hact -1;
            DHD0_HSYNC1.bits.hbb  = pstSyncInfo->u32Hbb -1;
            DHD0_HSYNC2.bits.hfb  = pstSyncInfo->u32Hfb -1;
            DHD0_HSYNC2.bits.hmid = pstSyncInfo->u32Hmid -1;
        }

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


HI_VOID VDP_DISP_SetFramePackingEn(HI_U32 u32hd_id, HI_U32 u32Enable)
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

HI_VOID VDP_DISP_SetHdmiClk(HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv)
{
    /*crg 54.*/
}

HI_VOID VDP_DISP_GetIntMask(HI_U32 *pu32masktype)
{
    /* Dispaly interrupt mask enable */
    *pu32masktype = VDP_RegRead((HI_U32)&(pVdpReg->VOINTMSK.u32));

    return;
}

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
        default:
        {
            HI_PRINT("Error! DP_DISP_SetSyncInv Wrong Select\n");
            return ;
        }
    }
    return ;
}


HI_VOID  VDP_DISP_SetCscEnable(HI_U32 u32hd_id, HI_U32 enCSC)
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

HI_VOID VDP_DISP_SetDataSel(HI_U32 u32hd_id, HI_U32 u32DataSel)
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

HI_VOID VDP_DISP_SetCscDcCoef(HI_U32 u32hd_id,VDP_CSC_DC_COEF_S stCscCoef)
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

HI_VOID VDP_DISP_SetCscCoef(HI_U32 u32hd_id,VDP_CSC_COEF_S stCscCoef)
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

HI_U32 VDP_DHD_GetDispMode(HI_U32 u32Data)
{
    U_DHD0_CTRL DHD0_CTRL;
    DHD0_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DHD0_CTRL.u32));

    return DHD0_CTRL.bits.iop;
}

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

/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/
/*************************it's the beginning of clk.*********************/

HI_VOID DISP_ResetCRG(HI_VOID)
{
    U_PERI_CRG54 unTmpValue;
    U_PERI_CRG127 unTmpValue127;

    unTmpValue127.bits.vo_ppc_cken  = 1;
    unTmpValue127.bits.sd_ppc_cken  = 1;
    unTmpValue127.bits.hd_ppc_cken  = 1;
    unTmpValue127.bits.vdp_cfg_cken = 1;
    unTmpValue127.bits.vdp_clk_sel  = 0;

    g_pstRegCrg->PERI_CRG127.u32 = unTmpValue127.u32;
    DISP_MSLEEP(5);

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
BIT[17-16] = vo_hd_clk_sel     <= 00:sd_ini;01:hd0_ini;10:hd1_ini;11:reserved
BIT[15-14] = vo_sd_clk_div     <= 00, 1/2; 01, 1/4; 1X, 1
BIT[13-12] = vo_sd_clk_sel     <= 00:sd_ini;01:hd0_ini;10:hd1_ini;11:reserved
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
    //unTmpValue.bits.vo_cken          = 1;
    unTmpValue.bits.vo_sd_cken       = 1;
    unTmpValue.bits.vo_sdate_cken    = 1;
    unTmpValue.bits.vo_hd_cken       = 1;
    unTmpValue.bits.vo_hdate_cken    = 1;
    unTmpValue.bits.vdac_ch0_cken    = 1;
    unTmpValue.bits.vdac_ch1_cken    = 1;
    unTmpValue.bits.vdac_ch2_cken    = 1;
    unTmpValue.bits.vdac_ch3_cken    = 1;
    unTmpValue.bits.vo_clkout_cken   = 1;
    unTmpValue.bits.vo_sd_clk_sel    = 0;
    unTmpValue.bits.vo_sd_clk_div    = 2;
    unTmpValue.bits.vo_hd_clk_sel    = 1;
    unTmpValue.bits.vo_hd_clk_div    = 0;
    unTmpValue.bits.vdac_ch0_clk_sel = 1;
    unTmpValue.bits.vdac_ch1_clk_sel = 1;
    unTmpValue.bits.vdac_ch2_clk_sel = 1;
    unTmpValue.bits.vdac_ch3_clk_sel = 1;
    unTmpValue.bits.vo_clkout_sel    = 1;
    unTmpValue.bits.vo_clkout_pctrl  = 0;

    unTmpValue.bits.hdmi_clk_sel     = 1;
    unTmpValue.bits.vo_sd_hdmi_clk_sel = 0;
    unTmpValue.bits.vo_hd_hdmi_clk_sel = 0;
    unTmpValue.bits.vou_srst_req       = 1;

    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    DISP_MSLEEP(5);
    unTmpValue.bits.vou_srst_req = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    DISP_MSLEEP(1);

    return;
}

HI_VOID VDP_CloseClkResetModule(HI_VOID)
{
    U_PERI_CRG54 unTmpValue1;
    U_PERI_CRG126 unTmpValue2;
    U_PERI_CRG127 unTmpValue127;

    /*to be reviewed, datasheet: vdac bg frequency sel:S5V100 does not need.*/
    unTmpValue2.u32 = g_pstRegCrg->PERI_CRG126.u32;
    unTmpValue2.bits.vdac_b_srst_req = 1;
    unTmpValue2.bits.vdac_g_srst_req = 1;
    unTmpValue2.bits.vdac_r_srst_req = 1;
    unTmpValue2.bits.vdac_c_srst_req = 1;
    g_pstRegCrg->PERI_CRG126.u32 = unTmpValue2.u32;

    udelay(10);

    unTmpValue2.bits.vdac_rct_cken = 0;
    unTmpValue2.bits.vdac_bg_cken = 0;
    g_pstRegCrg->PERI_CRG126.u32 = unTmpValue2.u32;

    /*firt reset and wait for 10us.*/
    unTmpValue1.u32 = g_pstRegCrg->PERI_CRG54.u32;
    unTmpValue1.bits.vou_srst_req = 1;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;

    udelay(10);

    unTmpValue1.bits.vo_clkout_cken = 0;
    unTmpValue1.bits.vdac_ch3_cken = 0;
    unTmpValue1.bits.vdac_ch2_cken = 0;
    unTmpValue1.bits.vdac_ch1_cken = 0;
    unTmpValue1.bits.vdac_ch0_cken = 0;
    unTmpValue1.bits.vo_hdate_cken = 0;
    unTmpValue1.bits.vo_hd_cken = 0;
    unTmpValue1.bits.vo_sdate_cken = 0;
    unTmpValue1.bits.vo_sd_cken = 0;
    unTmpValue1.bits.vo_bus_cken = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;

    udelay(10);

    /*close all the ppc and cfg clk.*/
    unTmpValue127.bits.vo_ppc_cken  = 0;
    unTmpValue127.bits.sd_ppc_cken  = 0;
    unTmpValue127.bits.hd_ppc_cken  = 0;
    unTmpValue127.bits.vdp_cfg_cken = 0;
    unTmpValue127.bits.vdp_clk_sel  = 0;
    g_pstRegCrg->PERI_CRG127.u32 = unTmpValue127.u32;

    udelay(10);
    return ;
}


HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode)
{
    U_PERI_CRG127 PERI_CRG127Tmp;

    PERI_CRG127Tmp.u32 = g_pstRegCrg->PERI_CRG127.u32;
    PERI_CRG127Tmp.bits.vdp_clk_sel = u32VDPClkMode;/*300M*/
    g_pstRegCrg->PERI_CRG127.u32 = PERI_CRG127Tmp.u32;
}

#if  USE_LVDS
HI_VOID VDP_LVDS_Init(HI_VOID)
{
    VDP_RegWrite((HI_U32)(&(pVdpReg->VBO_CTRL.u32)),0);
    VDP_INTF_SetLvdsEnable(HI_TRUE);
    VDP_INTF_SetLvdsBitMode(1);
    //*(HI_U32*)(reg_vdp_base_va + 0xd404) = 0x4bc68800;
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SWAP0.u32)),0x4bc68800);
    //LVDS phy
    //*(HI_U32*)(reg_vdp_base_va + 0xd504) = 0;   //VBO PHY
    //*(HI_U32*)(reg_vdp_base_va + 0xd410) = 0xf4000000;   //LVDS_PHYCTRL0

    VDP_RegWrite((HI_U32)(&(pVdpReg->VBO_PHYCTRL.u32)),0);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)),0xf4000000);
    VDP_DISP_SetDitherMode  (VDP_CHN_DHD0, VDP_DISP_INTF_LCD, 1);

    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_CTRL.u32)),0x80d80000);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SWAP0.u32)),0x00405300);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SWAP1.u32)),0x98765432);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SWAP2.u32)),0x10000000);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL0.u32)),0xf0000000);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL1.u32)),0x00000000);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_PHYCTRL2.u32)),0x00000000);
    VDP_RegWrite((HI_U32)(&(pVdpReg->LVDS_SPHYSTAT.u32)),0x00000001);

    return;

}

HI_VOID CRG_LVDS_Init(HI_VOID)

{
     #if 1
    U_PERI_CRG116 PERI_CRG116Tmp;

    U_PERI_CRG122 PERI_CRG122Tmp;
    //PERI_CRG116Tmp.u32 = g_pstRegCrg->PERI_CRG116.u32;
    PERI_CRG116Tmp.u32 = 0x101;
    PERI_CRG116Tmp.bits.lvds_cksel = 3;/*div 4*/
    //PERI_CRG116Tmp.bits.lvds_cken = 1;/*en clk*/
    g_pstRegCrg->PERI_CRG116.u32 = PERI_CRG116Tmp.u32;


    //PERI_CRG122Tmp.u32 = g_pstRegCrg->PERI_CRG122.u32;
    PERI_CRG122Tmp.u32 = 0x0;
    //PERI_CRG122Tmp.bits.vo_hd_stb_tv_cksel = 1;/*div 2*/
    g_pstRegCrg->PERI_CRG122.u32 = PERI_CRG122Tmp.u32;

    #else

    #endif
}

HI_VOID LVDS_Init(HI_VOID)
{
    CRG_LVDS_Init();
    VDP_LVDS_Init();
}
#endif


/*************************it's the beginning of VP.*********************/
/*************************it's the beginning of VP.*********************/

HI_VOID  VDP_VP_GetLayerReso(HI_U32 u32Data, HI_RECT_S *stRect)
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



HI_VOID  VDP_VP_SetLayer_InReso(HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
#if VP0_EN
    U_VP0_IRESO VP0_IRESO;

    if(u32Data >= VID_MAX)
    {
        HI_PRINT("Error,VDP_VP_SetLayerReso() Select Wrong Video Layer ID\n");
        return ;
    }

    VP0_IRESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    VP0_IRESO.bits.iw = stRect.u32IWth - 1;
    VP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    VDP_RegWrite((HI_U32)(&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET), VP0_IRESO.u32);

    return ;
#endif
}

HI_VOID  VDP_VP_SetLayer_OutReso(HI_U32 u32Data, VDP_DISP_RECT_S  stRect)
{
#if VP0_EN
    U_VP0_VFPOS VP0_VFPOS;
    U_VP0_VLPOS VP0_VLPOS;
    U_VP0_DFPOS VP0_DFPOS;
    U_VP0_DLPOS VP0_DLPOS;

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

    return ;
#endif
}

/*************************it's the beginning of MIX.*********************/
/*************************it's the beginning of MIX.*********************/
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

/**************************************************************************************/
/**************************************************************************************/


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
        /* rwzb need,  we do date adjust instead of csc.
         * this operation is common for every chip but cv200*/
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF56.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF57.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF58.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_U32)&(pVdpReg->DATE_COEFF59.u32), *pu32Src13tmp++);
#endif
    }
}

HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.c_gain = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}


#define DISP_DATE_SRC_COEF_INDEX_DEFAULT 0
#define DISP_DATE_SRC_COEF_INDEX_480P  1
#define DISP_DATE_SRC_COEF_INDEX_576P  1
#define DISP_DATE_SRC_COEF_INDEX_720P  2
#define DISP_DATE_SRC_COEF_INDEX_1080I 2
#define DISP_DATE_SRC_COEF_INDEX_1080P 3



HI_VOID VDP_DATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt)
{
    if (DISP_VENC_HDATE0 == eDate)
    {
        U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
        U_HDATE_OUT_CTRL     HDATE_OUT_CTRL;

        HDATE_OUT_CTRL.u32 = 0;
        HDATE_VIDEO_FORMAT.u32 = 0;

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

HI_VOID VDP_DATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync)
{
    //TODO SELECT RGB or YPbPr   and sync mode
    if (HI_DRV_DISP_INTF_YPBPR0 == enIntfId)
    {
        if (DISP_VENC_HDATE0 ==  eDate )
        {
            U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));

            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 0;/*YCbCr->YPbPr*/
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
            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 1;/*YCbCr->RGB ITU-R BT709*/
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


HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.dis_ire = u32Data;
    VDP_RegWrite((HI_U32)(&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


HI_VOID VDP_VDAC_ResetCRG(HI_VOID)
{
    U_PERI_CRG126 PERI_CRG126TMP;

    PERI_CRG126TMP.u32 = g_pstRegCrg->PERI_CRG126.u32;

    /*open the clock.*/
    PERI_CRG126TMP.bits.vdac_rct_cken    = 1;
    PERI_CRG126TMP.bits.vdac_bg_cken     = 1;

    /*software reset the 4 dacs.*/
    PERI_CRG126TMP.bits.vdac_c_srst_req  = 1;
    PERI_CRG126TMP.bits.vdac_r_srst_req  = 1;
    PERI_CRG126TMP.bits.vdac_g_srst_req  = 1;
    PERI_CRG126TMP.bits.vdac_b_srst_req  = 1;

    /*vdac  frequency divider
        00:1;
        01:2;
        10:3;
        11:4.*/
    PERI_CRG126TMP.bits.vdac_bg_clk_div  = 1;

    /*all the 4 dacs are positive-phase.*/
    PERI_CRG126TMP.bits.vdac_c_clk_pctrl = 0;
    PERI_CRG126TMP.bits.vdac_r_clk_pctrl = 0;
    PERI_CRG126TMP.bits.vdac_g_clk_pctrl = 0;
    PERI_CRG126TMP.bits.vdac_b_clk_pctrl = 0;

    g_pstRegCrg->PERI_CRG126.u32 = PERI_CRG126TMP.u32;
}

HI_VOID VDP_VDAC_Reset(HI_VOID)
{

    VDP_VDAC_ResetCRG();
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_DAC_CTRL.u32)), 0x80000000);
    DISP_MSLEEP(1);
    VDP_RegWrite((HI_U32)(&(pVdpReg->VO_MUX_DAC.u32)), 0);
    return;
}

HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset)
{
    U_PERI_CRG126 PERI_CRG126Tmp;

    PERI_CRG126Tmp.u32 = g_pstRegCrg->PERI_CRG126.u32;

    switch(uVdac)
    {
        case 0:
            PERI_CRG126Tmp.bits.vdac_c_srst_req = (bReset == HI_TRUE) ? 1 : 0;
            break;
        case 1:
            PERI_CRG126Tmp.bits.vdac_r_srst_req = (bReset == HI_TRUE) ? 1 : 0;
            break;

        case 2:
            PERI_CRG126Tmp.bits.vdac_g_srst_req = (bReset == HI_TRUE) ? 1 : 0;
            break;
        case 3:
            PERI_CRG126Tmp.bits.vdac_b_srst_req = (bReset == HI_TRUE) ? 1 : 0;
            break;
        default:
            return;
    }

    g_pstRegCrg->PERI_CRG126.u32 = PERI_CRG126Tmp.u32;

    return;
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

/*to be confirmed*/
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
        case 1:
            PERI_CRG54Tmp.bits.vdac_ch1_cken = (enable == HI_TRUE)? 1 : 0;
            break;
        case 2:
            PERI_CRG54Tmp.bits.vdac_ch2_cken = (enable == HI_TRUE)? 1 : 0;
            break;
        case 3:
            PERI_CRG54Tmp.bits.vdac_ch3_cken = (enable == HI_TRUE)? 1 : 0;
            break;
        default:
            break;
    }

    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;

    return;
}
/*************************it's the beginning of WBC0.*********************/
/*************************it's the beginning of WBC0.*********************/
/*************************it's the beginning of WBC0.*********************/
/*************************it's the beginning of WBC0.*********************/
/*************************it's the beginning of WBC0.*********************/

HI_VOID VDP_WBC_SetOutFmtUVOrder(VDP_LAYER_WBC_E enLayer, HI_U32 uvOrder)
{
}

#if 0
HI_VOID VDP_WBC_SetZmePhaseH(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
}

HI_VOID VDP_WBC_SetZmePhaseV(VDP_LAYER_WBC_E enLayer, HI_S32 s32PhaseL, HI_S32 s32PhaseC)
{
}
#endif

HI_U32 VDP_WBC_GetAlwaysProFlag(/*VDP_LAYER_WBC_E enLayer,*/ HI_BOOL *bAlwaysPro)
{
    return HI_SUCCESS;
}

HI_VOID VDP_RegSave(HI_U32 u32RegBackAddr)
{
    S_VDP_REGS_TYPE  *pVdpBackReg;
    pVdpBackReg = (S_VDP_REGS_TYPE *)u32RegBackAddr;

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
    memcpy((HI_VOID *)(&(pVdpBackReg->MIXV0_BKG.u32)), (HI_VOID *)(&(pVdpReg->MIXV0_BKG.u32)), 0x700);

    /*DHDx*/
    memcpy((HI_VOID *)(&(pVdpBackReg->DHD0_CTRL)), (HI_VOID *)(&(pVdpReg->DHD0_CTRL)), 0x400*2);
    /*DATE*/
    memcpy((HI_VOID *)(&(pVdpBackReg->HDATE_VERSION)), (HI_VOID *)(&(pVdpReg->HDATE_VERSION)), 0x300);

}

HI_VOID VDP_RegReStore(HI_U32 u32RegBackAddr)
{

    S_VDP_REGS_TYPE  *pVdpBackReg;

    pVdpBackReg = (S_VDP_REGS_TYPE *)u32RegBackAddr;

    /* save Reg */
    memcpy((HI_VOID *)(&(pVdpReg->VOCTRL)), (HI_VOID *)(&(pVdpBackReg->VOCTRL)), 0x100*2);
    memcpy((HI_VOID *)(&(pVdpReg->WBC_DHD_LOCATE)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD_LOCATE)), 0x100);
    memcpy((HI_VOID *)(&(pVdpReg->COEF_DATA)), (HI_VOID *)(&(pVdpBackReg->COEF_DATA)), 0x100);
    /*video layer */
    memcpy((HI_VOID *)(&(pVdpReg->V0_CTRL)), (HI_VOID *)(&(pVdpBackReg->V0_CTRL)), 0x800*5);

    /*VP0 VP1*/
    memcpy((HI_VOID *)(&(pVdpReg->VP0_CTRL)), (HI_VOID *)(&(pVdpBackReg->VP0_CTRL)), 0x800*2);

    /*DWBC0*/
    memcpy((HI_VOID *)(&(pVdpReg->WBC_DHD0_CTRL)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD0_CTRL)), 0x400);

    /*MIXER*/
    memcpy((HI_VOID *)(&(pVdpReg->MIXV0_BKG)), (HI_VOID *)(&(pVdpBackReg->MIXV0_BKG)), 0x700);

    /*DHDx*/
    memcpy((HI_VOID *)(&(pVdpReg->DHD0_CTRL)), (HI_VOID *)(&(pVdpBackReg->DHD0_CTRL)), 0x400*2);

    /*DATE*/
    memcpy((HI_VOID *)(&(pVdpReg->HDATE_VERSION)), (HI_VOID *)(&(pVdpBackReg->HDATE_VERSION)), 0x300);

}

/*s5v100, now is null.*/
HI_VOID VDP_DATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta)
{
    return;
}

HI_VOID VDP_DATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable)
{
    return;
}


HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH)
{

    *pu32VersionL =  0x76756F76UL;
    *pu32VersionH =  0x30313134UL;
   return HI_SUCCESS;
}

HI_VOID VDP_VP_SetAcmDbgEn(HI_U32 u32Data , HI_U32 u32bAcmDbgEn)
{
}

HI_VOID VDP_VP_SetAcmStretch(HI_U32 u32Data, HI_U32 u32AcmStrch)
{
}

HI_VOID VDP_VP_SetAcmClipRange(HI_U32 u32Data, HI_U32 u32AcmClipRange)
{
}

HI_VOID VDP_VP_SetAcmCliporwrap(HI_U32 u32Data, HI_U32 u32AcmCliporwrap)
{
}

HI_VOID VDP_VP_SetAcmChmThd(HI_U32 u32Data, HI_U32 u32AcmCbcrthr)
{
}

HI_VOID VDP_VP_SetAcmGainLum(HI_U32 u32Data, HI_U32 u32AcmGain0)
{
}

HI_VOID VDP_VP_SetAcmGainSat(HI_U32 u32Data, HI_U32 u32AcmGain2)
{
}

HI_VOID VDP_VP_SetAcmGainHue(HI_U32 u32Data, HI_U32 u32AcmGain1)
{
}

HI_VOID VDP_VP_SetAcmCoefAddr(HI_U32 u32Data, HI_U32 u32CoefAddr)
{
}

HI_VOID  VDP_VP0_SetAccCad(HI_U32 u32Data, HI_U32 u32addr)
{
}


HI_VOID VDP_VP_SetAccThd( HI_U32 u32Data,ACCTHD_S stAccThd)
{
}

HI_VOID VDP_VP_SetAccTab(HI_U32 u32Data,HI_U16 *upTable)
{
}

HI_VOID VDP_VP_SetAccCtrl(HI_U32 u32Data,HI_U32 uAccEn, HI_U32 uAccMode)
{
}

HI_VOID  VDP_VP0_SetParaUpd(HI_U32 u32Data, VDP_DISP_COEFMODE_E  enMode)
{

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
