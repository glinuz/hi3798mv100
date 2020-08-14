
#include "vdp_hal_ip_sharpen.h"
#include "vdp_hal_comm.h"

extern S_VDP_REGS_TYPE *pVdpReg;

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
#ifdef VDP_HI3798MV200
        SPCTRL.bits.demo_pos = u32DemoPos;
#else
    #ifdef VDP_HI3798CV200
        SPCTRL.bits.demo_pos = u32DemoPos/2*2;
    #else
        SPCTRL.bits.demo_pos = u32DemoPos;
    #endif
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

