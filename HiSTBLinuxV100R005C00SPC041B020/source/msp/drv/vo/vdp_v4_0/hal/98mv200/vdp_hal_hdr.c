//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_driver.cpp
// Author      : s173141
// Email       : 
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
#include "vdp_hal_hdr.h"
#include "vdp_hal_comm.h"
#include "vdp_define.h"


extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_GP_SetHdrSel    (HI_U32 u32Sel)
{
    //u32Mode 
    //0:normal, the same as process
    //1:dual channel process, single channel output
    U_GP0_CTRL GP0_CTRL;

    GP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)));
    GP0_CTRL.bits.gp_hdr_sel = u32Sel;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->GP0_CTRL.u32)), GP0_CTRL.u32);

    return ;
}

HI_VOID VDP_VP_SetHdrSel  (HI_U32 u32Sel)
{
    U_VP0_CTRL  VP0_CTRL;
	
	VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)));
	VP0_CTRL.bits.vp_hdr_sel = u32Sel;//DCI conncet to VP in HiFoneB02
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)), VP0_CTRL.u32);
	  
}

HI_VOID VDP_VP_SetIpt2YuvSel  (HI_U32 u32Sel)
{
    U_VP0_CTRL  VP0_CTRL;
	
	VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)));
	VP0_CTRL.bits.ipt2yuv_mux = u32Sel;//DCI conncet to VP in HiFoneB02
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32)), VP0_CTRL.u32);
	  
}


HI_VOID VDP_HDR_SetGCvmEn(HI_U32 gcvm_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.gcvm_en = gcvm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmIptInSel (HI_U32 u32Sel)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.ipt_in_sel = u32Sel;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVCvmS2uEn (HI_U32 s2u_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.vcvm_s2u_en = s2u_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmS2uEn (HI_U32 s2u_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.gcvm_s2u_en = s2u_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetCvm1U2sEn (HI_U32 u2s_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm1_u2s_en = u2s_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetIptOutSel(HI_U32 u32Sel)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.ipt_out_sel = u32Sel;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGcvmCoefParaup()
{
	U_DB_CVM1_PARA_UP DB_CVM1_PARA_UP;
	
	DB_CVM1_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)));
	DB_CVM1_PARA_UP.bits.gcvm_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)),DB_CVM1_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmSslutRdEn(HI_U32 gcvm_sslut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.gcvm_sslut_rd_en = gcvm_sslut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmTslutRdEn(HI_U32 gcvm_tslut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.gcvm_tslut_rd_en = gcvm_tslut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmSilutRdEn(HI_U32 gcvm_silut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.gcvm_silut_rd_en = gcvm_silut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmTilutRdEn(HI_U32 gcvm_tilut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.gcvm_tilut_rd_en = gcvm_tilut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmCoefAddr(HI_U32 gcvm_coef_addr)
{
	U_DB_CVM2_GCVM_COEFAD DB_CVM2_GCVM_COEFAD;
	
	DB_CVM2_GCVM_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_GCVM_COEFAD.u32)));
	DB_CVM2_GCVM_COEFAD.bits.gcvm_coef_addr = gcvm_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_GCVM_COEFAD.u32)),DB_CVM2_GCVM_COEFAD.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmLms2iptEnable(HI_U32 g_lms2ipt_en)
{
	U_DB_CVM2_LMS2IPT_CTRL DB_CVM2_LMS2IPT_CTRL;
	
	
	DB_CVM2_LMS2IPT_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_CTRL.u32) ));
	DB_CVM2_LMS2IPT_CTRL.bits.cvm1_lms2ipt_en = g_lms2ipt_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_CTRL.u32) ),DB_CVM2_LMS2IPT_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmImapLms2ipt(HI_S16 g_imap_lms2ipt[3][3])
{
	U_DB_CVM2_LMS2IPT_COEF0 DB_CVM2_LMS2IPT_COEF0;
	U_DB_CVM2_LMS2IPT_COEF1 DB_CVM2_LMS2IPT_COEF1;
	U_DB_CVM2_LMS2IPT_COEF2 DB_CVM2_LMS2IPT_COEF2;
	U_DB_CVM2_LMS2IPT_COEF3 DB_CVM2_LMS2IPT_COEF3;
	U_DB_CVM2_LMS2IPT_COEF4 DB_CVM2_LMS2IPT_COEF4;
	U_DB_CVM2_LMS2IPT_COEF5 DB_CVM2_LMS2IPT_COEF5;
	U_DB_CVM2_LMS2IPT_COEF6 DB_CVM2_LMS2IPT_COEF6;
	U_DB_CVM2_LMS2IPT_COEF7 DB_CVM2_LMS2IPT_COEF7;
	U_DB_CVM2_LMS2IPT_COEF8 DB_CVM2_LMS2IPT_COEF8;
	
	
	DB_CVM2_LMS2IPT_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF0.u32) ));
	DB_CVM2_LMS2IPT_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF1.u32) ));
	DB_CVM2_LMS2IPT_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF2.u32) ));
	DB_CVM2_LMS2IPT_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF3.u32) ));
	DB_CVM2_LMS2IPT_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF4.u32) ));
	DB_CVM2_LMS2IPT_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF5.u32) ));
	DB_CVM2_LMS2IPT_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF6.u32) ));
	DB_CVM2_LMS2IPT_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF7.u32) ));
	DB_CVM2_LMS2IPT_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF8.u32) ));
	DB_CVM2_LMS2IPT_COEF0.bits.v_imap_00_lms2ipt = g_imap_lms2ipt[0][0];
	DB_CVM2_LMS2IPT_COEF1.bits.v_imap_01_lms2ipt = g_imap_lms2ipt[0][1];
	DB_CVM2_LMS2IPT_COEF2.bits.v_imap_02_lms2ipt = g_imap_lms2ipt[0][2];
	DB_CVM2_LMS2IPT_COEF3.bits.v_imap_10_lms2ipt = g_imap_lms2ipt[1][0];
	DB_CVM2_LMS2IPT_COEF4.bits.v_imap_11_lms2ipt = g_imap_lms2ipt[1][1];
	DB_CVM2_LMS2IPT_COEF5.bits.v_imap_12_lms2ipt = g_imap_lms2ipt[1][2];
	DB_CVM2_LMS2IPT_COEF6.bits.v_imap_20_lms2ipt = g_imap_lms2ipt[2][0];
	DB_CVM2_LMS2IPT_COEF7.bits.v_imap_21_lms2ipt = g_imap_lms2ipt[2][1];
	DB_CVM2_LMS2IPT_COEF8.bits.v_imap_22_lms2ipt = g_imap_lms2ipt[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF0.u32) ),DB_CVM2_LMS2IPT_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF1.u32) ),DB_CVM2_LMS2IPT_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF2.u32) ),DB_CVM2_LMS2IPT_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF3.u32) ),DB_CVM2_LMS2IPT_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF4.u32) ),DB_CVM2_LMS2IPT_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF5.u32) ),DB_CVM2_LMS2IPT_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF6.u32) ),DB_CVM2_LMS2IPT_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF7.u32) ),DB_CVM2_LMS2IPT_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_COEF8.u32) ),DB_CVM2_LMS2IPT_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmImapLms2iptscale2p(HI_U32 g_imap_lms2iptscale2p)
{
	U_DB_CVM2_LMS2IPT_SCALE2P DB_CVM2_LMS2IPT_SCALE2P;
	
	
	DB_CVM2_LMS2IPT_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_SCALE2P.u32) ));
	DB_CVM2_LMS2IPT_SCALE2P.bits.v_imap_lms2iptscale2p = g_imap_lms2iptscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_SCALE2P.u32) ),DB_CVM2_LMS2IPT_SCALE2P.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmImapLms2iptmin(HI_U32 g_imap_lms2iptmin)
{
	U_DB_CVM2_LMS2IPT_MIN DB_CVM2_LMS2IPT_MIN;
	
	
	DB_CVM2_LMS2IPT_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_MIN.u32) ));
	DB_CVM2_LMS2IPT_MIN.bits.v_imap_lms2iptmin = g_imap_lms2iptmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_MIN.u32) ),DB_CVM2_LMS2IPT_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmImapLms2iptmax(HI_U32 g_imap_lms2iptmax)
{
	U_DB_CVM2_LMS2IPT_MAX DB_CVM2_LMS2IPT_MAX;
	
	
	DB_CVM2_LMS2IPT_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_MAX.u32) ));
	DB_CVM2_LMS2IPT_MAX.bits.v_imap_lms2iptmax = g_imap_lms2iptmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_MAX.u32) ),DB_CVM2_LMS2IPT_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmImapLms2iptOutDc(HI_S32  * g_imap_lms2ipt_out_dc)
{

    U_DB_CVM2_LMS2IPT_OUT_DC0   DB_CVM2_LMS2IPT_OUT_DC0; /* 0x3a34 */
    U_DB_CVM2_LMS2IPT_OUT_DC1   DB_CVM2_LMS2IPT_OUT_DC1; /* 0x3a38 */
    U_DB_CVM2_LMS2IPT_OUT_DC2   DB_CVM2_LMS2IPT_OUT_DC2; /* 0x3a3c */

	
	DB_CVM2_LMS2IPT_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC0.u32) ));
	DB_CVM2_LMS2IPT_OUT_DC0.bits.v_imap_lms2ipt_0_out_dc = g_imap_lms2ipt_out_dc[0];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC0.u32) ),DB_CVM2_LMS2IPT_OUT_DC0.u32);
	
	DB_CVM2_LMS2IPT_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC1.u32) ));
	DB_CVM2_LMS2IPT_OUT_DC1.bits.v_imap_lms2ipt_1_out_dc = g_imap_lms2ipt_out_dc[1];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC1.u32) ),DB_CVM2_LMS2IPT_OUT_DC1.u32);
	
	DB_CVM2_LMS2IPT_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC2.u32) ));
	DB_CVM2_LMS2IPT_OUT_DC2.bits.v_imap_lms2ipt_2_out_dc = g_imap_lms2ipt_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_LMS2IPT_OUT_DC2.u32) ),DB_CVM2_LMS2IPT_OUT_DC2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerVdrBitDepth(HI_U32 vdr_bit_depth)
{
	U_DB_COMPOSER_CTRL DB_COMPOSER_CTRL;
	
	
	DB_COMPOSER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_COMPOSER_CTRL.u32) ));
	DB_COMPOSER_CTRL.bits.vdr_bit_depth = vdr_bit_depth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_COMPOSER_CTRL.u32) ),DB_COMPOSER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerElUpsampleEn(HI_U32 el_upsample_en)
{
	U_DB_COMPOSER_CTRL DB_COMPOSER_CTRL;

	
	DB_COMPOSER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_COMPOSER_CTRL.u32) ));
	DB_COMPOSER_CTRL.bits.up_sample_en = el_upsample_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_COMPOSER_CTRL.u32) ),DB_COMPOSER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposeElUpsampleMax(HI_U32 el_upsample_max)
{
	U_DB_COMPOSER_US_CLIP DB_COMPOSER_US_CLIP ;	

	
	DB_COMPOSER_US_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_COMPOSER_US_CLIP.u32) ));
	DB_COMPOSER_US_CLIP.bits.hdr_us_max = el_upsample_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_COMPOSER_US_CLIP.u32) ),DB_COMPOSER_US_CLIP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposeElUpsampleMin(HI_U32 el_upsample_min)
{
	U_DB_COMPOSER_US_CLIP DB_COMPOSER_US_CLIP ;	

	
	DB_COMPOSER_US_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_COMPOSER_US_CLIP.u32) ));
	DB_COMPOSER_US_CLIP.bits.hdr_us_min = el_upsample_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_COMPOSER_US_CLIP.u32) ),DB_COMPOSER_US_CLIP.u32);
	
	return ;
}




HI_VOID VDP_VID_SetComposerBlNumPivotsY(HI_U32 bl_num_pivots)
{
	U_DB_POLY_CTRL DB_POLY_CTRL;
	
	
	DB_POLY_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ));
	DB_POLY_CTRL.bits.bl_num_pivots = bl_num_pivots;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ),DB_POLY_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerBlBitDepth(HI_U32 bl_bit_depth)
{
	U_DB_POLY_CTRL DB_POLY_CTRL;
	U_DB_MMR_CTRL DB_MMR_CTRL;
	
	
	DB_POLY_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ));
	DB_POLY_CTRL.bits.bl_bit_depth = bl_bit_depth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ),DB_POLY_CTRL.u32);
	
	DB_MMR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ));
	DB_MMR_CTRL.bits.bl_bit_depth = bl_bit_depth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ),DB_MMR_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlEn(HI_U32 bl_en)
{
	U_DB_POLY_CTRL DB_POLY_CTRL;
	
	
	DB_POLY_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ));
	DB_POLY_CTRL.bits.bl_en = bl_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_POLY_CTRL.u32) ),DB_POLY_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerBlPolyPivotValueY(HI_U32 bl_pivot_value[9])
{
	U_DB_BL_PIVOT_VALUE_0 DB_BL_PIVOT_VALUE_0;
    HI_U32 i;

    for(i=0;i<9;i++)
    {
	    DB_BL_PIVOT_VALUE_0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_0.u32) + i*4 ));
	    DB_BL_PIVOT_VALUE_0.bits.bl_0_pivot_value = bl_pivot_value[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_0.u32) + i*4 ),DB_BL_PIVOT_VALUE_0.u32);
    }
	
	return ;
}




HI_VOID VDP_VID_SetComposerBlPolyOrderY(HI_U32 bl_order[8])
{
	U_DB_BL_POLY_ORDER DB_BL_POLY_ORDER;
//	HI_U32 i;
	
	DB_BL_POLY_ORDER.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER.u32) ));
	DB_BL_POLY_ORDER.bits.bl_0_order = bl_order[0];
	DB_BL_POLY_ORDER.bits.bl_1_order = bl_order[1];
	DB_BL_POLY_ORDER.bits.bl_2_order = bl_order[2];
	DB_BL_POLY_ORDER.bits.bl_3_order = bl_order[3];
	DB_BL_POLY_ORDER.bits.bl_4_order = bl_order[4];
	DB_BL_POLY_ORDER.bits.bl_5_order = bl_order[5];
	DB_BL_POLY_ORDER.bits.bl_6_order = bl_order[6];
	DB_BL_POLY_ORDER.bits.bl_7_order = bl_order[7];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER.u32) ),DB_BL_POLY_ORDER.u32);
	
	return ;
}



HI_VOID VDP_VID_SetComposerBlPolyCoefY(HI_S32 bl_coef[8][3])
{
	U_DB_BL_COEF_00 DB_BL_COEF_00;
    HI_U32 i,j;
    
    for(i=0;i<8;i++)
    {
        for(j=0;j<3;j++)
        {
            DB_BL_COEF_00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_COEF_00.u32) + (i*3 + j)*4 ));
            DB_BL_COEF_00.bits.bl_00_coef = bl_coef[i][j];
	        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_COEF_00.u32) + (i*3 + j)*4 ),DB_BL_COEF_00.u32);
        }
    }
	
	return ;
}




HI_VOID VDP_VID_SetComposerBlNumPivotsV(HI_U32 bl_num_pivots_v)
{
	U_DB_MMR_CTRL DB_MMR_CTRL;
	
	
	DB_MMR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ));
	DB_MMR_CTRL.bits.bl_num_pivots_v = bl_num_pivots_v;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ),DB_MMR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerBlNumPivotsU(HI_U32 bl_num_pivots_u)
{
	U_DB_MMR_CTRL DB_MMR_CTRL;
	
	
	DB_MMR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ));
	DB_MMR_CTRL.bits.bl_num_pivots_u = bl_num_pivots_u;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ),DB_MMR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerMappingIdcV(HI_U32 mapping_idc_v)
{
	U_DB_MMR_CTRL DB_MMR_CTRL;
	
	
	DB_MMR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ));
	DB_MMR_CTRL.bits.mapping_idc_v = mapping_idc_v;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ),DB_MMR_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerMappingIdcU(HI_U32 mapping_idc_u)
{
	U_DB_MMR_CTRL DB_MMR_CTRL;
	
	
	DB_MMR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ));
	DB_MMR_CTRL.bits.mapping_idc_u = mapping_idc_u;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_MMR_CTRL.u32) ),DB_MMR_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyPivotValueU(HI_U32 bl_pivot_value[5])
{
	U_DB_BL_PIVOT_VALUE_U0 DB_BL_PIVOT_VALUE_U0;
    HI_U32 i;

    for(i=0;i<5;i++)
    {
	    DB_BL_PIVOT_VALUE_U0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_U0.u32) + i*4));
	    DB_BL_PIVOT_VALUE_U0.bits.bl_0_pivot_value_u = bl_pivot_value[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_U0.u32) + i*4 ),DB_BL_PIVOT_VALUE_U0.u32);
    }

	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyOrderU(HI_U32 bl_order[4])
{
	U_DB_BL_POLY_ORDER_U DB_BL_POLY_ORDER_U;
//    HI_U32 i;	
	
	DB_BL_POLY_ORDER_U.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER_U.u32) ));
	DB_BL_POLY_ORDER_U.bits.bl_0_order_u = bl_order[0];
	DB_BL_POLY_ORDER_U.bits.bl_1_order_u = bl_order[1];
	DB_BL_POLY_ORDER_U.bits.bl_2_order_u = bl_order[2];
	DB_BL_POLY_ORDER_U.bits.bl_3_order_u = bl_order[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER_U.u32) ),DB_BL_POLY_ORDER_U.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyCoefU(HI_S32 bl_coef[4][3])
{
	U_DB_BL_COEF_U00 DB_BL_COEF_U00;
	HI_U32 i,j;

	for(i=0;i<4;i++)
    {
	    for(j=0;j<3;j++)
        {
            DB_BL_COEF_U00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_COEF_U00.u32) + (i*3 +j)*4));
	        DB_BL_COEF_U00.bits.bl_00_coef_u = bl_coef[i][j];
	        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_COEF_U00.u32) + (i*3 + j)*4),DB_BL_COEF_U00.u32);
        }
    }

	return ;
}

HI_VOID VDP_VID_SetComposerBlMmrOrderU(HI_U32 bl_mmr_order_u)
{
	U_DB_BL_U_MMR_ORDER DB_BL_U_MMR_ORDER;
	
	
	DB_BL_U_MMR_ORDER.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_U_MMR_ORDER.u32) ));
	DB_BL_U_MMR_ORDER.bits.bl_mmr_order_u = bl_mmr_order_u;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_U_MMR_ORDER.u32) ),DB_BL_U_MMR_ORDER.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerBlMmrCoefU(HI_S32 bl_mmr_coef_u[22])
{
	U_DB_BL_MMR_COEF_U0 DB_BL_MMR_COEF_U0;
    HI_U32 i;

    for(i=0;i<22;i++)
    {
	    DB_BL_MMR_COEF_U0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_MMR_COEF_U0.u32) + i*4));
	    DB_BL_MMR_COEF_U0.bits.bl_0_mmr_coef_u   = bl_mmr_coef_u[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_MMR_COEF_U0.u32) + i*4 ),DB_BL_MMR_COEF_U0.u32);
    }
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyPivotValueV(HI_U32 bl_pivot_value[5])
{
	U_DB_BL_PIVOT_VALUE_V0 DB_BL_PIVOT_VALUE_V0;
    HI_U32 i;

    for(i=0;i<5;i++)
    {
	    DB_BL_PIVOT_VALUE_V0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_V0.u32) + i*4));
	    DB_BL_PIVOT_VALUE_V0.bits.bl_0_pivot_value_v = bl_pivot_value[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_PIVOT_VALUE_V0.u32) + i*4 ),DB_BL_PIVOT_VALUE_V0.u32);
    }
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyOrderV(HI_U32 bl_order[4])
{
	U_DB_BL_POLY_ORDER_V DB_BL_POLY_ORDER_V;
//    HI_U32 i;	
	
	DB_BL_POLY_ORDER_V.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER_V.u32) ));

	DB_BL_POLY_ORDER_V.bits.bl_0_order_v = bl_order[0];
	DB_BL_POLY_ORDER_V.bits.bl_1_order_v = bl_order[1];
	DB_BL_POLY_ORDER_V.bits.bl_2_order_v = bl_order[2];
	DB_BL_POLY_ORDER_V.bits.bl_3_order_v = bl_order[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_POLY_ORDER_V.u32) ),DB_BL_POLY_ORDER_V.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlPolyCoefV(HI_S32 bl_coef[4][3])
{
	U_DB_BL_COEF_V00 DB_BL_COEF_V00;
    HI_U32 i,j;	

    for(i=0;i<4;i++)
    {
        for(j=0;j<3;j++)
        {
	        DB_BL_COEF_V00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_COEF_V00.u32)+ (i*3 + j)*4 ));
	        DB_BL_COEF_V00.bits.bl_00_coef_v = bl_coef[i][j];
	        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_COEF_V00.u32) + (i*3 + j)*4 ),DB_BL_COEF_V00.u32);
        }
    }
	
	return ;
}


HI_VOID VDP_VID_SetComposerBlMmrOrderV(HI_U32 bl_mmr_order_v)
{
	U_DB_BL_V_MMR_ORDER DB_BL_V_MMR_ORDER;
	
	
	DB_BL_V_MMR_ORDER.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_V_MMR_ORDER.u32) ));
	DB_BL_V_MMR_ORDER.bits.bl_mmr_order_v = bl_mmr_order_v;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_V_MMR_ORDER.u32) ),DB_BL_V_MMR_ORDER.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerBlMmrCoefV(HI_S32 bl_mmr_coef_v[22])
{
	U_DB_BL_MMR_COEF_V0 DB_BL_MMR_COEF_V0;
    HI_U32 i;

    for(i=0;i<22;i++)
    {
	    DB_BL_MMR_COEF_V0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_BL_MMR_COEF_V0.u32) + i*4));
	    DB_BL_MMR_COEF_V0.bits.bl_0_mmr_coef_v   = bl_mmr_coef_v[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_BL_MMR_COEF_V0.u32) + i*4 ),DB_BL_MMR_COEF_V0.u32);
    }
	
	return ;
}


HI_VOID VDP_VID_SetComposerElEn(HI_U32 el_en)
{
	U_DB_NLDQ_CTRL DB_NLDQ_CTRL;
	
	
	DB_NLDQ_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_NLDQ_CTRL.u32) ));
	DB_NLDQ_CTRL.bits.el_en = el_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_NLDQ_CTRL.u32) ),DB_NLDQ_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerElBitDepth(HI_U32 el_bit_depth)
{
	U_DB_NLDQ_CTRL DB_NLDQ_CTRL;
	
	
	DB_NLDQ_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_NLDQ_CTRL.u32) ));
	DB_NLDQ_CTRL.bits.el_bit_depth = el_bit_depth;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_NLDQ_CTRL.u32) ),DB_NLDQ_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerElNldqOffsetY(HI_U32 el_nldq_offset_y)
{
	U_DB_EL_NLDQ_Y_OFFSET DB_EL_NLDQ_OFFSET_Y;
	
	
	DB_EL_NLDQ_OFFSET_Y.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_Y_OFFSET.u32) ));
	DB_EL_NLDQ_OFFSET_Y.bits.el_nldq_offset_y = el_nldq_offset_y;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_Y_OFFSET.u32) ),DB_EL_NLDQ_OFFSET_Y.u32);
	
	return ;
}


HI_VOID VDP_VID_SetComposerElNldqCoefY(HI_S32 el_nldq_coef_y[3])
{
	U_DB_EL_NLDQ_Y0_COEF DB_EL_NLDQ_COEF_Y0;
    HI_U32 i;	
	
    for(i=0;i<3;i++)
    {
	    DB_EL_NLDQ_COEF_Y0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_Y0_COEF.u32) + i*4));
	    DB_EL_NLDQ_COEF_Y0.bits.el_nldq_coef_0_y = el_nldq_coef_y[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_Y0_COEF.u32) + i*4),DB_EL_NLDQ_COEF_Y0.u32);
    }
	
	return ;
}


HI_VOID VDP_VID_SetComposerElNldqOffsetU(HI_U32 el_nldq_offset_u)
{
	U_DB_EL_NLDQ_U_OFFSET DB_EL_NLDQ_OFFSET_U;
	
	
	DB_EL_NLDQ_OFFSET_U.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_U_OFFSET.u32) ));
	DB_EL_NLDQ_OFFSET_U.bits.el_nldq_offset_u = el_nldq_offset_u;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_U_OFFSET.u32) ),DB_EL_NLDQ_OFFSET_U.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerElNldqCoefU(HI_S32 el_nldq_coef_u[3])
{
	U_DB_EL_NLDQ_U0_COEF DB_EL_NLDQ_COEF_U0;
    HI_U32 i;	
	
    for(i=0;i<3;i++)
    {
	    DB_EL_NLDQ_COEF_U0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_U0_COEF.u32) + i*4 ));
	    DB_EL_NLDQ_COEF_U0.bits.el_nldq_coef_0_u = el_nldq_coef_u[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_U0_COEF.u32) + i*4),DB_EL_NLDQ_COEF_U0.u32);
    }
	
	return ;
}


HI_VOID VDP_VID_SetComposerElNldqOffsetV(HI_U32 el_nldq_offset_v)
{
	U_DB_EL_NLDQ_V_OFFSET DB_EL_NLDQ_OFFSET_V;
	
	
	DB_EL_NLDQ_OFFSET_V.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_V_OFFSET.u32) ));
	DB_EL_NLDQ_OFFSET_V.bits.el_nldq_offset_v = el_nldq_offset_v;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_V_OFFSET.u32) ),DB_EL_NLDQ_OFFSET_V.u32);
	
	return ;
}

HI_VOID VDP_VID_SetComposerElNldqCoefV(HI_S32 el_nldq_coef_v[3])
{
	U_DB_EL_NLDQ_V0_COEF DB_EL_NLDQ_COEF_V0;
    HI_U32 i;	
	
    for(i=0;i<3;i++)
    {
	    DB_EL_NLDQ_COEF_V0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_V0_COEF.u32) + i*4));
	    DB_EL_NLDQ_COEF_V0.bits.el_nldq_coef_0_v = el_nldq_coef_v[i];
	    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_EL_NLDQ_V0_COEF.u32) + i*4 ),DB_EL_NLDQ_COEF_V0.u32);
    }
	
	return ;
}


//-------------------------------------------------------------------
//DM
//-------------------------------------------------------------------


HI_VOID VDP_HDR_SetVCvmOutOetf(HI_U32 cvm_out_oetf)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm_out_oetf = cvm_out_oetf;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVCvmDenormEn(HI_U32 denorm_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.v_denorm_en = denorm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmInEotf(HI_U32 v_in_eotf)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_in_eotf = v_in_eotf;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetCvmOutBits(HI_U32 cvm_out_bits)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm_out_bits = cvm_out_bits;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmInBits(HI_U32 v_in_bits)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_in_bits = v_in_bits;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetCvmOutColor(HI_U32 cvm_out_color)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm_out_color = cvm_out_color;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmInColor(HI_U32 v_in_color)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_in_color = v_in_color;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmGammaEn(HI_U32 v_gamma_en)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_gamma_en = v_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmRgb2lmsEn(HI_U32 v_rgb2lms_en)
{
	U_DB_VDM_RGB2LMS_CTRL DB_VDM_RGB2LMS_CTRL;
	
	
	DB_VDM_RGB2LMS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_CTRL.u32) ));
	DB_VDM_RGB2LMS_CTRL.bits.vdm_rgb2lms_en = v_rgb2lms_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_CTRL.u32) ),DB_VDM_RGB2LMS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmDegammaEn(HI_U32 v_degamma_en)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_degamma_en = v_degamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmYuv2rgbEn(HI_U32 v_yuv2rgb_en)
{
	U_DB_VDM_YUV2RGB_CTRL DB_VDM_YUV2RGB_CTRL;
	
	
	DB_VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ));
	DB_VDM_YUV2RGB_CTRL.bits.vdm_yuv2rgb_en = v_yuv2rgb_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ),DB_VDM_YUV2RGB_CTRL.u32);
	
	return ;
}
HI_VOID VDP_HDR_SetVDmYuv2rgbV0En(HI_U32 vdm_v0_csc_en)
{
	U_DB_VDM_YUV2RGB_CTRL DB_VDM_YUV2RGB_CTRL;
	
	
	DB_VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ));
	DB_VDM_YUV2RGB_CTRL.bits.vdm_v0_csc_en = vdm_v0_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ),DB_VDM_YUV2RGB_CTRL.u32);
	
	return ;
}
HI_VOID VDP_HDR_SetVDmYuv2rgbV1En(HI_U32 vdm_v1_csc_en)
{
	U_DB_VDM_YUV2RGB_CTRL DB_VDM_YUV2RGB_CTRL;
	
	
	DB_VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ));
	DB_VDM_YUV2RGB_CTRL.bits.vdm_v1_csc_en = vdm_v1_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ),DB_VDM_YUV2RGB_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmGammaV0En(HI_U32 vdm_v0_gamma_en)
{
	U_DB_VDM_YUV2RGB_CTRL DB_VDM_YUV2RGB_CTRL;
	
	
	DB_VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ));
	DB_VDM_YUV2RGB_CTRL.bits.vdm_v0_gamma_en = vdm_v0_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ),DB_VDM_YUV2RGB_CTRL.u32);
	
	return ;
}
HI_VOID VDP_HDR_SetVDmGammaV1En(HI_U32 vdm_v1_gamma_en)
{
	U_DB_VDM_YUV2RGB_CTRL DB_VDM_YUV2RGB_CTRL;
	
	
	DB_VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ));
	DB_VDM_YUV2RGB_CTRL.bits.vdm_v1_gamma_en = vdm_v1_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_CTRL.u32) ),DB_VDM_YUV2RGB_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmEn(HI_U32 v_vdm_en)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_vdm_en = v_vdm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmNormEn(HI_U32 v_norm_en)
{
	U_DB_VDM_CTRL DB_VDM_CTRL;
	
	
	DB_VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ));
	DB_VDM_CTRL.bits.v_norm_en = v_norm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_CTRL.u32) ),DB_VDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImapRangeMin(HI_U32 v_imap_range_min)
{
	U_DB_VDM_IN_RANGEMIN DB_VDM_IN_RANGEMIN;
	
	
	DB_VDM_IN_RANGEMIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGEMIN.u32) ));
	DB_VDM_IN_RANGEMIN.bits.v_imap_range_min = v_imap_range_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGEMIN.u32) ),DB_VDM_IN_RANGEMIN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRange(HI_U32 v_imap_range)
{
	U_DB_VDM_IN_RANGE DB_VDM_IN_RANGE;
	
	
	DB_VDM_IN_RANGE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGE.u32) ));
	DB_VDM_IN_RANGE.bits.v_imap_range = v_imap_range;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGE.u32) ),DB_VDM_IN_RANGE.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRangeInv(HI_U32 v_imap_range_inv)
{
	U_DB_VDM_IN_RANGEINV DB_VDM_IN_RANGEINV;
	
	
	DB_VDM_IN_RANGEINV.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGEINV.u32) ));
	DB_VDM_IN_RANGEINV.bits.v_imap_range_inv = v_imap_range_inv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IN_RANGEINV.u32) ),DB_VDM_IN_RANGEINV.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImap0Yuv2rgb(HI_S16 v_imap_yuv2rgb[3][3])
{
	U_DB_VDM_YUV2RGB_COEF00 DB_VDM_YUV2RGB_COEF00;
	U_DB_VDM_YUV2RGB_COEF01 DB_VDM_YUV2RGB_COEF01;
	U_DB_VDM_YUV2RGB_COEF02 DB_VDM_YUV2RGB_COEF02;
	U_DB_VDM_YUV2RGB_COEF03 DB_VDM_YUV2RGB_COEF03;
	U_DB_VDM_YUV2RGB_COEF04 DB_VDM_YUV2RGB_COEF04;
	U_DB_VDM_YUV2RGB_COEF05 DB_VDM_YUV2RGB_COEF05;
	U_DB_VDM_YUV2RGB_COEF06 DB_VDM_YUV2RGB_COEF06;
	U_DB_VDM_YUV2RGB_COEF07 DB_VDM_YUV2RGB_COEF07;
	U_DB_VDM_YUV2RGB_COEF08 DB_VDM_YUV2RGB_COEF08;
	
	
	DB_VDM_YUV2RGB_COEF00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF00.u32) ));
	DB_VDM_YUV2RGB_COEF01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF01.u32) ));
	DB_VDM_YUV2RGB_COEF02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF02.u32) ));
	DB_VDM_YUV2RGB_COEF03.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF03.u32) ));
	DB_VDM_YUV2RGB_COEF04.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF04.u32) ));
	DB_VDM_YUV2RGB_COEF05.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF05.u32) ));
	DB_VDM_YUV2RGB_COEF06.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF06.u32) ));
	DB_VDM_YUV2RGB_COEF07.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF07.u32) ));
	DB_VDM_YUV2RGB_COEF08.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF08.u32) ));
	DB_VDM_YUV2RGB_COEF00.bits.v_imap_00_yuv2rgb = v_imap_yuv2rgb[0][0];
	DB_VDM_YUV2RGB_COEF01.bits.v_imap_01_yuv2rgb = v_imap_yuv2rgb[0][1];
	DB_VDM_YUV2RGB_COEF02.bits.v_imap_02_yuv2rgb = v_imap_yuv2rgb[0][2];
	DB_VDM_YUV2RGB_COEF03.bits.v_imap_03_yuv2rgb = v_imap_yuv2rgb[1][0];
	DB_VDM_YUV2RGB_COEF04.bits.v_imap_04_yuv2rgb = v_imap_yuv2rgb[1][1];
	DB_VDM_YUV2RGB_COEF05.bits.v_imap_05_yuv2rgb = v_imap_yuv2rgb[1][2];
	DB_VDM_YUV2RGB_COEF06.bits.v_imap_06_yuv2rgb = v_imap_yuv2rgb[2][0];
	DB_VDM_YUV2RGB_COEF07.bits.v_imap_07_yuv2rgb = v_imap_yuv2rgb[2][1];
	DB_VDM_YUV2RGB_COEF08.bits.v_imap_08_yuv2rgb = v_imap_yuv2rgb[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF00.u32) ),DB_VDM_YUV2RGB_COEF00.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF01.u32) ),DB_VDM_YUV2RGB_COEF01.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF02.u32) ),DB_VDM_YUV2RGB_COEF02.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF03.u32) ),DB_VDM_YUV2RGB_COEF03.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF04.u32) ),DB_VDM_YUV2RGB_COEF04.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF05.u32) ),DB_VDM_YUV2RGB_COEF05.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF06.u32) ),DB_VDM_YUV2RGB_COEF06.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF07.u32) ),DB_VDM_YUV2RGB_COEF07.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF08.u32) ),DB_VDM_YUV2RGB_COEF08.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImap1Yuv2rgb(HI_S16 v_imap_yuv2rgb[3][3])
{
	U_DB_VDM_YUV2RGB_COEF10 DB_VDM_YUV2RGB_COEF10;
	U_DB_VDM_YUV2RGB_COEF11 DB_VDM_YUV2RGB_COEF11;
	U_DB_VDM_YUV2RGB_COEF12 DB_VDM_YUV2RGB_COEF12;
	U_DB_VDM_YUV2RGB_COEF13 DB_VDM_YUV2RGB_COEF13;
	U_DB_VDM_YUV2RGB_COEF14 DB_VDM_YUV2RGB_COEF14;
	U_DB_VDM_YUV2RGB_COEF15 DB_VDM_YUV2RGB_COEF15;
	U_DB_VDM_YUV2RGB_COEF16 DB_VDM_YUV2RGB_COEF16;
	U_DB_VDM_YUV2RGB_COEF17 DB_VDM_YUV2RGB_COEF17;
	U_DB_VDM_YUV2RGB_COEF18 DB_VDM_YUV2RGB_COEF18;
	
	
	DB_VDM_YUV2RGB_COEF10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF10.u32) ));
	DB_VDM_YUV2RGB_COEF11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF11.u32) ));
	DB_VDM_YUV2RGB_COEF12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF12.u32) ));
	DB_VDM_YUV2RGB_COEF13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF13.u32) ));
	DB_VDM_YUV2RGB_COEF14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF14.u32) ));
	DB_VDM_YUV2RGB_COEF15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF15.u32) ));
	DB_VDM_YUV2RGB_COEF16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF16.u32) ));
	DB_VDM_YUV2RGB_COEF17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF17.u32) ));
	DB_VDM_YUV2RGB_COEF18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF18.u32) ));
	DB_VDM_YUV2RGB_COEF10.bits.v_imap_10_yuv2rgb = v_imap_yuv2rgb[0][0];
	DB_VDM_YUV2RGB_COEF11.bits.v_imap_11_yuv2rgb = v_imap_yuv2rgb[0][1];
	DB_VDM_YUV2RGB_COEF12.bits.v_imap_12_yuv2rgb = v_imap_yuv2rgb[0][2];
	DB_VDM_YUV2RGB_COEF13.bits.v_imap_13_yuv2rgb = v_imap_yuv2rgb[1][0];
	DB_VDM_YUV2RGB_COEF14.bits.v_imap_14_yuv2rgb = v_imap_yuv2rgb[1][1];
	DB_VDM_YUV2RGB_COEF15.bits.v_imap_15_yuv2rgb = v_imap_yuv2rgb[1][2];
	DB_VDM_YUV2RGB_COEF16.bits.v_imap_16_yuv2rgb = v_imap_yuv2rgb[2][0];
	DB_VDM_YUV2RGB_COEF17.bits.v_imap_17_yuv2rgb = v_imap_yuv2rgb[2][1];
	DB_VDM_YUV2RGB_COEF18.bits.v_imap_18_yuv2rgb = v_imap_yuv2rgb[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF10.u32) ),DB_VDM_YUV2RGB_COEF10.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF11.u32) ),DB_VDM_YUV2RGB_COEF11.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF12.u32) ),DB_VDM_YUV2RGB_COEF12.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF13.u32) ),DB_VDM_YUV2RGB_COEF13.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF14.u32) ),DB_VDM_YUV2RGB_COEF14.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF15.u32) ),DB_VDM_YUV2RGB_COEF15.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF16.u32) ),DB_VDM_YUV2RGB_COEF16.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF17.u32) ),DB_VDM_YUV2RGB_COEF17.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_COEF18.u32) ),DB_VDM_YUV2RGB_COEF18.u32);
	
	return ;
}



HI_VOID VDP_HDR_SetVDmImapYuv2rgbscale2p(HI_U32 v_imap_yuv2rgbscale2p)
{
	U_DB_VDM_YUV2RGB_SCALE2P DB_VDM_YUV2RGB_SCALE2P;
	
	
	DB_VDM_YUV2RGB_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_SCALE2P.u32) ));
	DB_VDM_YUV2RGB_SCALE2P.bits.v_imap_yuv2rgbscale2p = v_imap_yuv2rgbscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_SCALE2P.u32) ),DB_VDM_YUV2RGB_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapYuv2rgbInDc0(HI_S32 v_imap_yuv2rgb_in_dc[3])
{
	U_DB_VDM_YUV2RGB_IN_DC00 DB_VDM_YUV2RGB_IN_DC00;
	U_DB_VDM_YUV2RGB_IN_DC01 DB_VDM_YUV2RGB_IN_DC01;
	U_DB_VDM_YUV2RGB_IN_DC02 DB_VDM_YUV2RGB_IN_DC02;
	
	
	DB_VDM_YUV2RGB_IN_DC00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC00.u32) ));
    DB_VDM_YUV2RGB_IN_DC01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC01.u32) ));
    DB_VDM_YUV2RGB_IN_DC02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC02.u32) ));
	DB_VDM_YUV2RGB_IN_DC00.bits.v_imap_yuv2rgb_00_in_dc = v_imap_yuv2rgb_in_dc[0];
	DB_VDM_YUV2RGB_IN_DC01.bits.v_imap_yuv2rgb_01_in_dc = v_imap_yuv2rgb_in_dc[1];
	DB_VDM_YUV2RGB_IN_DC02.bits.v_imap_yuv2rgb_02_in_dc = v_imap_yuv2rgb_in_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC00.u32) ),DB_VDM_YUV2RGB_IN_DC00.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC01.u32) ),DB_VDM_YUV2RGB_IN_DC01.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC02.u32) ),DB_VDM_YUV2RGB_IN_DC02.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImapYuv2rgbInDc1(HI_S32 v_imap_yuv2rgb_in_dc[3])
{
	U_DB_VDM_YUV2RGB_IN_DC10 DB_VDM_YUV2RGB_IN_DC10;
	U_DB_VDM_YUV2RGB_IN_DC11 DB_VDM_YUV2RGB_IN_DC11;
	U_DB_VDM_YUV2RGB_IN_DC12 DB_VDM_YUV2RGB_IN_DC12;
	
	
	DB_VDM_YUV2RGB_IN_DC10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC10.u32) ));
    DB_VDM_YUV2RGB_IN_DC11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC11.u32) ));
    DB_VDM_YUV2RGB_IN_DC12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC12.u32) ));
	DB_VDM_YUV2RGB_IN_DC10.bits.v_imap_yuv2rgb_10_in_dc = v_imap_yuv2rgb_in_dc[0];
	DB_VDM_YUV2RGB_IN_DC11.bits.v_imap_yuv2rgb_11_in_dc = v_imap_yuv2rgb_in_dc[1];
	DB_VDM_YUV2RGB_IN_DC12.bits.v_imap_yuv2rgb_12_in_dc = v_imap_yuv2rgb_in_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC10.u32) ),DB_VDM_YUV2RGB_IN_DC10.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC11.u32) ),DB_VDM_YUV2RGB_IN_DC11.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_IN_DC12.u32) ),DB_VDM_YUV2RGB_IN_DC12.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImapYuv2rgbOutDc0(HI_S32 v_imap_yuv2rgb_out_dc[3])
{
	U_DB_VDM_YUV2RGB_OUT_DC00 DB_VDM_YUV2RGB_OUT_DC00;
	U_DB_VDM_YUV2RGB_OUT_DC01 DB_VDM_YUV2RGB_OUT_DC01;
	U_DB_VDM_YUV2RGB_OUT_DC02 DB_VDM_YUV2RGB_OUT_DC02;
	
	
	DB_VDM_YUV2RGB_OUT_DC00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC00.u32) ));
    DB_VDM_YUV2RGB_OUT_DC01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC01.u32) ));
    DB_VDM_YUV2RGB_OUT_DC02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC02.u32) ));
	DB_VDM_YUV2RGB_OUT_DC00.bits.v_imap_yuv2rgb_00_out_dc = v_imap_yuv2rgb_out_dc[0];
	DB_VDM_YUV2RGB_OUT_DC01.bits.v_imap_yuv2rgb_01_out_dc = v_imap_yuv2rgb_out_dc[1];
	DB_VDM_YUV2RGB_OUT_DC02.bits.v_imap_yuv2rgb_02_out_dc = v_imap_yuv2rgb_out_dc[2];

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC00.u32) ),DB_VDM_YUV2RGB_OUT_DC00.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC01.u32) ),DB_VDM_YUV2RGB_OUT_DC01.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC02.u32) ),DB_VDM_YUV2RGB_OUT_DC02.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapYuv2rgbOutDc1(HI_S32 v_imap_yuv2rgb_out_dc[3])
{
	U_DB_VDM_YUV2RGB_OUT_DC10 DB_VDM_YUV2RGB_OUT_DC10;
	U_DB_VDM_YUV2RGB_OUT_DC11 DB_VDM_YUV2RGB_OUT_DC11;
	U_DB_VDM_YUV2RGB_OUT_DC12 DB_VDM_YUV2RGB_OUT_DC12;
	
	
	DB_VDM_YUV2RGB_OUT_DC10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC10.u32) ));
    DB_VDM_YUV2RGB_OUT_DC11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC11.u32) ));
    DB_VDM_YUV2RGB_OUT_DC12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC12.u32) ));
	DB_VDM_YUV2RGB_OUT_DC10.bits.v_imap_yuv2rgb_10_out_dc = v_imap_yuv2rgb_out_dc[0];
	DB_VDM_YUV2RGB_OUT_DC11.bits.v_imap_yuv2rgb_11_out_dc = v_imap_yuv2rgb_out_dc[1];
	DB_VDM_YUV2RGB_OUT_DC12.bits.v_imap_yuv2rgb_12_out_dc = v_imap_yuv2rgb_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC10.u32) ),DB_VDM_YUV2RGB_OUT_DC10.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC11.u32) ),DB_VDM_YUV2RGB_OUT_DC11.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_YUV2RGB_OUT_DC12.u32) ),DB_VDM_YUV2RGB_OUT_DC12.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRgb2lms(HI_S16 v_imap_rgb2lms[3][3])
{
	U_DB_VDM_RGB2LMS_COEF0 DB_VDM_RGB2LMS_COEF0;
	U_DB_VDM_RGB2LMS_COEF1 DB_VDM_RGB2LMS_COEF1;
	U_DB_VDM_RGB2LMS_COEF2 DB_VDM_RGB2LMS_COEF2;
	U_DB_VDM_RGB2LMS_COEF3 DB_VDM_RGB2LMS_COEF3;
	U_DB_VDM_RGB2LMS_COEF4 DB_VDM_RGB2LMS_COEF4;
	U_DB_VDM_RGB2LMS_COEF5 DB_VDM_RGB2LMS_COEF5;
	U_DB_VDM_RGB2LMS_COEF6 DB_VDM_RGB2LMS_COEF6;
	U_DB_VDM_RGB2LMS_COEF7 DB_VDM_RGB2LMS_COEF7;
	U_DB_VDM_RGB2LMS_COEF8 DB_VDM_RGB2LMS_COEF8;
	
	
	DB_VDM_RGB2LMS_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF0.u32) ));
	DB_VDM_RGB2LMS_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF1.u32) ));
	DB_VDM_RGB2LMS_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF2.u32) ));
	DB_VDM_RGB2LMS_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF3.u32) ));
	DB_VDM_RGB2LMS_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF4.u32) ));
	DB_VDM_RGB2LMS_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF5.u32) ));
	DB_VDM_RGB2LMS_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF6.u32) ));
	DB_VDM_RGB2LMS_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF7.u32) ));
	DB_VDM_RGB2LMS_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF8.u32) ));
	DB_VDM_RGB2LMS_COEF0.bits.v_imap_00_rgb2lms = v_imap_rgb2lms[0][0];
	DB_VDM_RGB2LMS_COEF1.bits.v_imap_01_rgb2lms = v_imap_rgb2lms[0][1];
	DB_VDM_RGB2LMS_COEF2.bits.v_imap_02_rgb2lms = v_imap_rgb2lms[0][2];
	DB_VDM_RGB2LMS_COEF3.bits.v_imap_10_rgb2lms = v_imap_rgb2lms[1][0];
	DB_VDM_RGB2LMS_COEF4.bits.v_imap_11_rgb2lms = v_imap_rgb2lms[1][1];
	DB_VDM_RGB2LMS_COEF5.bits.v_imap_12_rgb2lms = v_imap_rgb2lms[1][2];
	DB_VDM_RGB2LMS_COEF6.bits.v_imap_20_rgb2lms = v_imap_rgb2lms[2][0];
	DB_VDM_RGB2LMS_COEF7.bits.v_imap_21_rgb2lms = v_imap_rgb2lms[2][1];
	DB_VDM_RGB2LMS_COEF8.bits.v_imap_22_rgb2lms = v_imap_rgb2lms[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF0.u32) ),DB_VDM_RGB2LMS_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF1.u32) ),DB_VDM_RGB2LMS_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF2.u32) ),DB_VDM_RGB2LMS_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF3.u32) ),DB_VDM_RGB2LMS_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF4.u32) ),DB_VDM_RGB2LMS_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF5.u32) ),DB_VDM_RGB2LMS_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF6.u32) ),DB_VDM_RGB2LMS_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF7.u32) ),DB_VDM_RGB2LMS_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_COEF8.u32) ),DB_VDM_RGB2LMS_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRgb2lmsscale2p(HI_U32 v_imap_rgb2lmsscale2p)
{
	U_DB_VDM_RGB2LMS_SCALE2P DB_VDM_RGB2LMS_SCALE2P;
	
	
	DB_VDM_RGB2LMS_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_SCALE2P.u32) ));
	DB_VDM_RGB2LMS_SCALE2P.bits.v_imap_rgb2lmsscale2p = v_imap_rgb2lmsscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_SCALE2P.u32) ),DB_VDM_RGB2LMS_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRgb2lmsmin( HI_U32 v_imap_rgb2lmsmin)
{
	U_DB_VDM_RGB2LMS_MIN DB_VDM_RGB2LMS_MIN;
	
	DB_VDM_RGB2LMS_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_MIN.u32) ));
	DB_VDM_RGB2LMS_MIN.bits.v_imap_rgb2lmsmin = v_imap_rgb2lmsmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_MIN.u32) ),DB_VDM_RGB2LMS_MIN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmImapRgb2lmsmax( HI_U32 v_imap_rgb2lmsmax)
{
	U_DB_VDM_RGB2LMS_MAX DB_VDM_RGB2LMS_MAX;
	
	DB_VDM_RGB2LMS_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_MAX.u32) ));
	DB_VDM_RGB2LMS_MAX.bits.v_imap_rgb2lmsmax = v_imap_rgb2lmsmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_RGB2LMS_MAX.u32) ),DB_VDM_RGB2LMS_MAX.u32);
	
	return ;
}



HI_VOID VDP_HDR_SetVDmDegammaRdEn(HI_U32 degamma_rd_en)
{
	U_DB_VDM_PARA_REN DB_VDM_PARA_REN;
	
	
	DB_VDM_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32) ));
	DB_VDM_PARA_REN.bits.degamma_rd_en = degamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32) ),DB_VDM_PARA_REN.u32);
	
	return ;
}



HI_VOID VDP_HDR_SetVCvmRgb2yuvEn(HI_U32 v_rgb2yuv_en)
{
	U_DB_CVM1_RGB2YUV_CTRL DB_CVM1_RGB2YUV_CTRL;
	
	
	DB_CVM1_RGB2YUV_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_CTRL.u32) ));
	DB_CVM1_RGB2YUV_CTRL.bits.cvm1_rgb2yuv_en = v_rgb2yuv_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_CTRL.u32) ),DB_CVM1_RGB2YUV_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmMGammaEn(HI_U32 v_gamma_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.v_gamma_en = v_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmLms2rgbEn(HI_U32 v_lms2rgb_en)
{
	U_DB_CVM1_LMS2RGB_CTRL DB_CVM1_LMS2RGB_CTRL;
	
	
	DB_CVM1_LMS2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_CTRL.u32) ));
	DB_CVM1_LMS2RGB_CTRL.bits.cvm1_lms2rgb_en = v_lms2rgb_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_CTRL.u32) ),DB_CVM1_LMS2RGB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmDegammaEn(HI_U32 v_degamma_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.v_degamma_en = v_degamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmIpt2lmsEn(HI_U32 v_ipt2lms_en)
{
	U_DB_CVM1_IPT2LMS_CTRL DB_CVM1_IPT2LMS_CTRL;
	
	
	DB_CVM1_IPT2LMS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_CTRL.u32) ));
	DB_CVM1_IPT2LMS_CTRL.bits.cvm1_ipt2lms_en = v_ipt2lms_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_CTRL.u32) ),DB_CVM1_IPT2LMS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmEn(HI_U32 vcvm_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.vcvm_en = vcvm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmLms2iptEn(HI_U32 v_lms2ipt_en)
{
	U_DB_CVM1_LMS2IPT_CTRL DB_CVM1_LMS2IPT_CTRL;
	
	
	DB_CVM1_LMS2IPT_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_CTRL.u32) ));
	DB_CVM1_LMS2IPT_CTRL.bits.cvm1_lms2ipt_en = v_lms2ipt_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_CTRL.u32) ),DB_CVM1_LMS2IPT_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetCvmSel(HI_U32 cvm_sel)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm_sel = cvm_sel;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetCvmEn(HI_U32 cvm_en)
{
	U_DB_CVM_CTRL DB_CVM_CTRL;
	
	
	DB_CVM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ));
	DB_CVM_CTRL.bits.cvm_en = cvm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM_CTRL.u32) ),DB_CVM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmImapLms2ipt(HI_S16 v_imap_lms2ipt[3][3])
{
	U_DB_CVM1_LMS2IPT_COEF0 DB_CVM1_LMS2IPT_COEF0;
	U_DB_CVM1_LMS2IPT_COEF1 DB_CVM1_LMS2IPT_COEF1;
	U_DB_CVM1_LMS2IPT_COEF2 DB_CVM1_LMS2IPT_COEF2;
	U_DB_CVM1_LMS2IPT_COEF3 DB_CVM1_LMS2IPT_COEF3;
	U_DB_CVM1_LMS2IPT_COEF4 DB_CVM1_LMS2IPT_COEF4;
	U_DB_CVM1_LMS2IPT_COEF5 DB_CVM1_LMS2IPT_COEF5;
	U_DB_CVM1_LMS2IPT_COEF6 DB_CVM1_LMS2IPT_COEF6;
	U_DB_CVM1_LMS2IPT_COEF7 DB_CVM1_LMS2IPT_COEF7;
	U_DB_CVM1_LMS2IPT_COEF8 DB_CVM1_LMS2IPT_COEF8;
	
	
	DB_CVM1_LMS2IPT_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF0.u32) ));
	DB_CVM1_LMS2IPT_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF1.u32) ));
	DB_CVM1_LMS2IPT_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF2.u32) ));
	DB_CVM1_LMS2IPT_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF3.u32) ));
	DB_CVM1_LMS2IPT_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF4.u32) ));
	DB_CVM1_LMS2IPT_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF5.u32) ));
	DB_CVM1_LMS2IPT_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF6.u32) ));
	DB_CVM1_LMS2IPT_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF7.u32) ));
	DB_CVM1_LMS2IPT_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF8.u32) ));
	DB_CVM1_LMS2IPT_COEF0.bits.v_imap_00_lms2ipt = v_imap_lms2ipt[0][0];
	DB_CVM1_LMS2IPT_COEF1.bits.v_imap_01_lms2ipt = v_imap_lms2ipt[0][1];
	DB_CVM1_LMS2IPT_COEF2.bits.v_imap_02_lms2ipt = v_imap_lms2ipt[0][2];
	DB_CVM1_LMS2IPT_COEF3.bits.v_imap_10_lms2ipt = v_imap_lms2ipt[1][0];
	DB_CVM1_LMS2IPT_COEF4.bits.v_imap_11_lms2ipt = v_imap_lms2ipt[1][1];
	DB_CVM1_LMS2IPT_COEF5.bits.v_imap_12_lms2ipt = v_imap_lms2ipt[1][2];
	DB_CVM1_LMS2IPT_COEF6.bits.v_imap_20_lms2ipt = v_imap_lms2ipt[2][0];
	DB_CVM1_LMS2IPT_COEF7.bits.v_imap_21_lms2ipt = v_imap_lms2ipt[2][1];
	DB_CVM1_LMS2IPT_COEF8.bits.v_imap_22_lms2ipt = v_imap_lms2ipt[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF0.u32) ),DB_CVM1_LMS2IPT_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF1.u32) ),DB_CVM1_LMS2IPT_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF2.u32) ),DB_CVM1_LMS2IPT_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF3.u32) ),DB_CVM1_LMS2IPT_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF4.u32) ),DB_CVM1_LMS2IPT_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF5.u32) ),DB_CVM1_LMS2IPT_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF6.u32) ),DB_CVM1_LMS2IPT_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF7.u32) ),DB_CVM1_LMS2IPT_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_COEF8.u32) ),DB_CVM1_LMS2IPT_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmImapLms2iptscale2p(HI_U32 v_imap_lms2iptscale2p)
{
	U_DB_CVM1_LMS2IPT_SCALE2P DB_CVM1_LMS2IPT_SCALE2P;
	
	
	DB_CVM1_LMS2IPT_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_SCALE2P.u32) ));
	DB_CVM1_LMS2IPT_SCALE2P.bits.v_imap_lms2iptscale2p = v_imap_lms2iptscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_SCALE2P.u32) ),DB_CVM1_LMS2IPT_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapIpt2lms(HI_S16 v_omap_ipt2lms[3][3])
{
	U_DB_CVM1_IPT2LMS_COEF0 DB_CVM1_IPT2LMS_COEF0;
	U_DB_CVM1_IPT2LMS_COEF1 DB_CVM1_IPT2LMS_COEF1;
	U_DB_CVM1_IPT2LMS_COEF2 DB_CVM1_IPT2LMS_COEF2;
	U_DB_CVM1_IPT2LMS_COEF3 DB_CVM1_IPT2LMS_COEF3;
	U_DB_CVM1_IPT2LMS_COEF4 DB_CVM1_IPT2LMS_COEF4;
	U_DB_CVM1_IPT2LMS_COEF5 DB_CVM1_IPT2LMS_COEF5;
	U_DB_CVM1_IPT2LMS_COEF6 DB_CVM1_IPT2LMS_COEF6;
	U_DB_CVM1_IPT2LMS_COEF7 DB_CVM1_IPT2LMS_COEF7;
	U_DB_CVM1_IPT2LMS_COEF8 DB_CVM1_IPT2LMS_COEF8;
	
	
	DB_CVM1_IPT2LMS_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF0.u32) ));
	DB_CVM1_IPT2LMS_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF1.u32) ));
	DB_CVM1_IPT2LMS_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF2.u32) ));
	DB_CVM1_IPT2LMS_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF3.u32) ));
	DB_CVM1_IPT2LMS_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF4.u32) ));
	DB_CVM1_IPT2LMS_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF5.u32) ));
	DB_CVM1_IPT2LMS_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF6.u32) ));
	DB_CVM1_IPT2LMS_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF7.u32) ));
	DB_CVM1_IPT2LMS_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF8.u32) ));
	DB_CVM1_IPT2LMS_COEF0.bits.v_omap_00_ipt2lms = v_omap_ipt2lms[0][0];
	DB_CVM1_IPT2LMS_COEF1.bits.v_omap_01_ipt2lms = v_omap_ipt2lms[0][1];
	DB_CVM1_IPT2LMS_COEF2.bits.v_omap_02_ipt2lms = v_omap_ipt2lms[0][2];
	DB_CVM1_IPT2LMS_COEF3.bits.v_omap_10_ipt2lms = v_omap_ipt2lms[1][0];
	DB_CVM1_IPT2LMS_COEF4.bits.v_omap_11_ipt2lms = v_omap_ipt2lms[1][1];
	DB_CVM1_IPT2LMS_COEF5.bits.v_omap_12_ipt2lms = v_omap_ipt2lms[1][2];
	DB_CVM1_IPT2LMS_COEF6.bits.v_omap_20_ipt2lms = v_omap_ipt2lms[2][0];
	DB_CVM1_IPT2LMS_COEF7.bits.v_omap_21_ipt2lms = v_omap_ipt2lms[2][1];
	DB_CVM1_IPT2LMS_COEF8.bits.v_omap_22_ipt2lms = v_omap_ipt2lms[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF0.u32) ),DB_CVM1_IPT2LMS_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF1.u32) ),DB_CVM1_IPT2LMS_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF2.u32) ),DB_CVM1_IPT2LMS_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF3.u32) ),DB_CVM1_IPT2LMS_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF4.u32) ),DB_CVM1_IPT2LMS_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF5.u32) ),DB_CVM1_IPT2LMS_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF6.u32) ),DB_CVM1_IPT2LMS_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF7.u32) ),DB_CVM1_IPT2LMS_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_COEF8.u32) ),DB_CVM1_IPT2LMS_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapIpt2lmsscale2p(HI_U32 v_omap_ipt2lmsscale2p)
{
	U_DB_CVM1_IPT2LMS_SCALE2P DB_CVM1_IPT2LMS_SCALE2P;
	
	
	DB_CVM1_IPT2LMS_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_SCALE2P.u32) ));
	DB_CVM1_IPT2LMS_SCALE2P.bits.v_omap_ipt2lmsscale2p = v_omap_ipt2lmsscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_SCALE2P.u32) ),DB_CVM1_IPT2LMS_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapLms2rgb(HI_S16 v_omap_lms2rgb[3][3])
{
	U_DB_CVM1_LMS2RGB_COEF0 DB_CVM1_LMS2RGB_COEF0;
	U_DB_CVM1_LMS2RGB_COEF1 DB_CVM1_LMS2RGB_COEF1;
	U_DB_CVM1_LMS2RGB_COEF2 DB_CVM1_LMS2RGB_COEF2;
	U_DB_CVM1_LMS2RGB_COEF3 DB_CVM1_LMS2RGB_COEF3;
	U_DB_CVM1_LMS2RGB_COEF4 DB_CVM1_LMS2RGB_COEF4;
	U_DB_CVM1_LMS2RGB_COEF5 DB_CVM1_LMS2RGB_COEF5;
	U_DB_CVM1_LMS2RGB_COEF6 DB_CVM1_LMS2RGB_COEF6;
	U_DB_CVM1_LMS2RGB_COEF7 DB_CVM1_LMS2RGB_COEF7;
	U_DB_CVM1_LMS2RGB_COEF8 DB_CVM1_LMS2RGB_COEF8;
	
	
	DB_CVM1_LMS2RGB_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF0.u32) ));
	DB_CVM1_LMS2RGB_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF1.u32) ));
	DB_CVM1_LMS2RGB_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF2.u32) ));
	DB_CVM1_LMS2RGB_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF3.u32) ));
	DB_CVM1_LMS2RGB_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF4.u32) ));
	DB_CVM1_LMS2RGB_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF5.u32) ));
	DB_CVM1_LMS2RGB_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF6.u32) ));
	DB_CVM1_LMS2RGB_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF7.u32) ));
	DB_CVM1_LMS2RGB_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF8.u32) ));
	DB_CVM1_LMS2RGB_COEF0.bits.v_omap_00_lms2rgb = v_omap_lms2rgb[0][0];
	DB_CVM1_LMS2RGB_COEF1.bits.v_omap_01_lms2rgb = v_omap_lms2rgb[0][1];
	DB_CVM1_LMS2RGB_COEF2.bits.v_omap_02_lms2rgb = v_omap_lms2rgb[0][2];
	DB_CVM1_LMS2RGB_COEF3.bits.v_omap_10_lms2rgb = v_omap_lms2rgb[1][0];
	DB_CVM1_LMS2RGB_COEF4.bits.v_omap_11_lms2rgb = v_omap_lms2rgb[1][1];
	DB_CVM1_LMS2RGB_COEF5.bits.v_omap_12_lms2rgb = v_omap_lms2rgb[1][2];
	DB_CVM1_LMS2RGB_COEF6.bits.v_omap_20_lms2rgb = v_omap_lms2rgb[2][0];
	DB_CVM1_LMS2RGB_COEF7.bits.v_omap_21_lms2rgb = v_omap_lms2rgb[2][1];
	DB_CVM1_LMS2RGB_COEF8.bits.v_omap_22_lms2rgb = v_omap_lms2rgb[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF0.u32) ),DB_CVM1_LMS2RGB_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF1.u32) ),DB_CVM1_LMS2RGB_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF2.u32) ),DB_CVM1_LMS2RGB_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF3.u32) ),DB_CVM1_LMS2RGB_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF4.u32) ),DB_CVM1_LMS2RGB_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF5.u32) ),DB_CVM1_LMS2RGB_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF6.u32) ),DB_CVM1_LMS2RGB_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF7.u32) ),DB_CVM1_LMS2RGB_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_COEF8.u32) ),DB_CVM1_LMS2RGB_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapLms2rgbscale2p(HI_U32 v_omap_lms2rgbscale2p)
{
	U_DB_CVM1_LMS2RGB_SCALE2P DB_CVM1_LMS2RGB_SCALE2P;
	
	
	DB_CVM1_LMS2RGB_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_SCALE2P.u32) ));
	DB_CVM1_LMS2RGB_SCALE2P.bits.v_omap_lms2rgbscale2p = v_omap_lms2rgbscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_SCALE2P.u32) ),DB_CVM1_LMS2RGB_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapRgb2yuv(HI_S16 v_omap_rgb2yuv[3][3])
{
	U_DB_CVM1_RGB2YUV_COEF0 DB_CVM1_RGB2YUV_COEF0;
	U_DB_CVM1_RGB2YUV_COEF1 DB_CVM1_RGB2YUV_COEF1;
	U_DB_CVM1_RGB2YUV_COEF2 DB_CVM1_RGB2YUV_COEF2;
	U_DB_CVM1_RGB2YUV_COEF3 DB_CVM1_RGB2YUV_COEF3;
	U_DB_CVM1_RGB2YUV_COEF4 DB_CVM1_RGB2YUV_COEF4;
	U_DB_CVM1_RGB2YUV_COEF5 DB_CVM1_RGB2YUV_COEF5;
	U_DB_CVM1_RGB2YUV_COEF6 DB_CVM1_RGB2YUV_COEF6;
	U_DB_CVM1_RGB2YUV_COEF7 DB_CVM1_RGB2YUV_COEF7;
	U_DB_CVM1_RGB2YUV_COEF8 DB_CVM1_RGB2YUV_COEF8;
	
	
	DB_CVM1_RGB2YUV_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF0.u32) ));
	DB_CVM1_RGB2YUV_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF1.u32) ));
	DB_CVM1_RGB2YUV_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF2.u32) ));
	DB_CVM1_RGB2YUV_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF3.u32) ));
	DB_CVM1_RGB2YUV_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF4.u32) ));
	DB_CVM1_RGB2YUV_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF5.u32) ));
	DB_CVM1_RGB2YUV_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF6.u32) ));
	DB_CVM1_RGB2YUV_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF7.u32) ));
	DB_CVM1_RGB2YUV_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF8.u32) ));
	DB_CVM1_RGB2YUV_COEF0.bits.v_omap_00_rgb2yuv = v_omap_rgb2yuv[0][0];
	DB_CVM1_RGB2YUV_COEF1.bits.v_omap_01_rgb2yuv = v_omap_rgb2yuv[0][1];
	DB_CVM1_RGB2YUV_COEF2.bits.v_omap_02_rgb2yuv = v_omap_rgb2yuv[0][2];
	DB_CVM1_RGB2YUV_COEF3.bits.v_omap_10_rgb2yuv = v_omap_rgb2yuv[1][0];
	DB_CVM1_RGB2YUV_COEF4.bits.v_omap_11_rgb2yuv = v_omap_rgb2yuv[1][1];
	DB_CVM1_RGB2YUV_COEF5.bits.v_omap_12_rgb2yuv = v_omap_rgb2yuv[1][2];
	DB_CVM1_RGB2YUV_COEF6.bits.v_omap_20_rgb2yuv = v_omap_rgb2yuv[2][0];
	DB_CVM1_RGB2YUV_COEF7.bits.v_omap_21_rgb2yuv = v_omap_rgb2yuv[2][1];
	DB_CVM1_RGB2YUV_COEF8.bits.v_omap_22_rgb2yuv = v_omap_rgb2yuv[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF0.u32) ),DB_CVM1_RGB2YUV_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF1.u32) ),DB_CVM1_RGB2YUV_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF2.u32) ),DB_CVM1_RGB2YUV_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF3.u32) ),DB_CVM1_RGB2YUV_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF4.u32) ),DB_CVM1_RGB2YUV_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF5.u32) ),DB_CVM1_RGB2YUV_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF6.u32) ),DB_CVM1_RGB2YUV_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF7.u32) ),DB_CVM1_RGB2YUV_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_COEF8.u32) ),DB_CVM1_RGB2YUV_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapRgb2yuvscale2p(HI_U32 v_omap_rgb2yuvscale2p)
{
	U_DB_CVM1_RGB2YUV_SCALE2P DB_CVM1_RGB2YUV_SCALE2P;
	
	
	DB_CVM1_RGB2YUV_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_SCALE2P.u32) ));
	DB_CVM1_RGB2YUV_SCALE2P.bits.v_omap_rgb2yuvscale2p = v_omap_rgb2yuvscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_SCALE2P.u32) ),DB_CVM1_RGB2YUV_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapRgb2yuvOutDc(HI_S32  * v_omap_rgb2yuv_out_dc)
{
	U_DB_CVM1_RGB2YUV_OUT_DC0 DB_CVM1_RGB2YUV_OUT_DC0;
	U_DB_CVM1_RGB2YUV_OUT_DC1 DB_CVM1_RGB2YUV_OUT_DC1;
	U_DB_CVM1_RGB2YUV_OUT_DC2 DB_CVM1_RGB2YUV_OUT_DC2;
	
	
	DB_CVM1_RGB2YUV_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC0.u32) ));
	DB_CVM1_RGB2YUV_OUT_DC0.bits.v_omap_rgb2yuv_0_out_dc = v_omap_rgb2yuv_out_dc[0];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC0.u32) ),DB_CVM1_RGB2YUV_OUT_DC0.u32);
	
	DB_CVM1_RGB2YUV_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC1.u32) ));
	DB_CVM1_RGB2YUV_OUT_DC1.bits.v_omap_rgb2yuv_1_out_dc = v_omap_rgb2yuv_out_dc[1];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC1.u32) ),DB_CVM1_RGB2YUV_OUT_DC1.u32);
	
	DB_CVM1_RGB2YUV_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC2.u32) ));
	DB_CVM1_RGB2YUV_OUT_DC2.bits.v_omap_rgb2yuv_2_out_dc = v_omap_rgb2yuv_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_RGB2YUV_OUT_DC2.u32) ),DB_CVM1_RGB2YUV_OUT_DC2.u32);
	
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapRangeOver(HI_U32 v_omap_range_over)
{
	U_DB_CVM1_OUT_RANGEOVERONE DB_CVM1_OUT_RANGEOVERONE;
	
	
	DB_CVM1_OUT_RANGEOVERONE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OUT_RANGEOVERONE.u32) ));
	DB_CVM1_OUT_RANGEOVERONE.bits.v_omap_range_over = v_omap_range_over;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OUT_RANGEOVERONE.u32) ),DB_CVM1_OUT_RANGEOVERONE.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmOmapRangeMin(HI_U32 v_omap_range_min)
{
	U_DB_CVM1_OUT_RANGEMIN DB_CVM1_OUT_RANGEMIN;
	
	
	DB_CVM1_OUT_RANGEMIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OUT_RANGEMIN.u32) ));
	DB_CVM1_OUT_RANGEMIN.bits.v_omap_range_min = v_omap_range_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OUT_RANGEMIN.u32) ),DB_CVM1_OUT_RANGEMIN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmSslutRdEn(HI_U32 vcvm_sslut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.vcvm_sslut_rd_en = vcvm_sslut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmTslutRdEn(HI_U32 vcvm_tslut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.vcvm_tslut_rd_en = vcvm_tslut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmSilutRdEn(HI_U32 vcvm_silut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.vcvm_silut_rd_en = vcvm_silut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmTilutRdEn(HI_U32 vcvm_tilut_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.vcvm_tilut_rd_en = vcvm_tilut_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}




HI_VOID VDP_HDR_SetVCvmL2pqLutbRdEn(HI_U32 l2pq_lutb_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.l2pq_lutb_rd_en = l2pq_lutb_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmL2pqLutaRdEn(HI_U32 l2pq_luta_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.l2pq_luta_rd_en = l2pq_luta_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmL2pqLutxRdEn(HI_U32 l2pq_lutx_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ));
	DB_CVM1_PARA_REN.bits.l2pq_lutx_rd_en = l2pq_lutx_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32) ),DB_CVM1_PARA_REN.u32);
	
	return ;
}
/*
HI_VOID VDP_HDR_SetGCvmLms2IptEn(HI_U32 g_lms2ipt_en)
{
	U_DB_CVM2_RGB2YUV_CTRL DB_CVM2_RGB2YUV_CTRL;
	
	
	DB_CVM2_RGB2YUV_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_CTRL.u32) ));
	DB_CVM2_RGB2YUV_CTRL.bits.cvm2_rgb2yuv_en = g_lms2ipt_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_CTRL.u32) ),DB_CVM2_RGB2YUV_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmLms2Ipt(HI_S16 g_lms2ipt[3][3])
{
	U_DB_CVM2_RGB2YUV_COEF0 DB_CVM2_RGB2YUV_COEF0;
	U_DB_CVM2_RGB2YUV_COEF1 DB_CVM2_RGB2YUV_COEF1;
	U_DB_CVM2_RGB2YUV_COEF2 DB_CVM2_RGB2YUV_COEF2;
	U_DB_CVM2_RGB2YUV_COEF3 DB_CVM2_RGB2YUV_COEF3;
	U_DB_CVM2_RGB2YUV_COEF4 DB_CVM2_RGB2YUV_COEF4;
	U_DB_CVM2_RGB2YUV_COEF5 DB_CVM2_RGB2YUV_COEF5;
	U_DB_CVM2_RGB2YUV_COEF6 DB_CVM2_RGB2YUV_COEF6;
	U_DB_CVM2_RGB2YUV_COEF7 DB_CVM2_RGB2YUV_COEF7;
	U_DB_CVM2_RGB2YUV_COEF8 DB_CVM2_RGB2YUV_COEF8;
	
	
	DB_CVM2_RGB2YUV_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF0.u32) ));
	DB_CVM2_RGB2YUV_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF1.u32) ));
	DB_CVM2_RGB2YUV_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF2.u32) ));
	DB_CVM2_RGB2YUV_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF3.u32) ));
	DB_CVM2_RGB2YUV_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF4.u32) ));
	DB_CVM2_RGB2YUV_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF5.u32) ));
	DB_CVM2_RGB2YUV_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF6.u32) ));
	DB_CVM2_RGB2YUV_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF7.u32) ));
	DB_CVM2_RGB2YUV_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF8.u32) ));
	DB_CVM2_RGB2YUV_COEF0.bits.g_omap_00_rgb2yuv = g_lms2ipt[0][0];
	DB_CVM2_RGB2YUV_COEF1.bits.g_omap_01_rgb2yuv = g_lms2ipt[0][1];
	DB_CVM2_RGB2YUV_COEF2.bits.g_omap_02_rgb2yuv = g_lms2ipt[0][2];
	DB_CVM2_RGB2YUV_COEF3.bits.g_omap_10_rgb2yuv = g_lms2ipt[1][0];
	DB_CVM2_RGB2YUV_COEF4.bits.g_omap_11_rgb2yuv = g_lms2ipt[1][1];
	DB_CVM2_RGB2YUV_COEF5.bits.g_omap_12_rgb2yuv = g_lms2ipt[1][2];
	DB_CVM2_RGB2YUV_COEF6.bits.g_omap_20_rgb2yuv = g_lms2ipt[2][0];
	DB_CVM2_RGB2YUV_COEF7.bits.g_omap_21_rgb2yuv = g_lms2ipt[2][1];
	DB_CVM2_RGB2YUV_COEF8.bits.g_omap_22_rgb2yuv = g_lms2ipt[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF0.u32) ),DB_CVM2_RGB2YUV_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF1.u32) ),DB_CVM2_RGB2YUV_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF2.u32) ),DB_CVM2_RGB2YUV_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF3.u32) ),DB_CVM2_RGB2YUV_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF4.u32) ),DB_CVM2_RGB2YUV_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF5.u32) ),DB_CVM2_RGB2YUV_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF6.u32) ),DB_CVM2_RGB2YUV_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF7.u32) ),DB_CVM2_RGB2YUV_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_COEF8.u32) ),DB_CVM2_RGB2YUV_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGCvmLms2IptScale2p(HI_U32 g_lms2ipt_scale2p)
{
	U_DB_CVM2_RGB2YUV_SCALE2P DB_CVM2_RGB2YUV_SCALE2P;
	
	
	DB_CVM2_RGB2YUV_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_SCALE2P.u32) ));
	DB_CVM2_RGB2YUV_SCALE2P.bits.g_omap_rgb2yuvscale2p = g_lms2ipt_scale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_SCALE2P.u32) ),DB_CVM2_RGB2YUV_SCALE2P.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmLms2IptMin(HI_U32 g_imap_lms2iptmin)
{
	U_DB_CVM2_RGB2YUV_MIN DB_CVM2_RGB2YUV_MIN;
	
	DB_CVM2_RGB2YUV_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MIN.u32) ));
	DB_CVM2_RGB2YUV_MIN.bits.g_omap_rgb2yuvmin = g_imap_lms2iptmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MIN.u32) ),DB_CVM2_RGB2YUV_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmLms2IptMax(HI_U32 g_imap_lms2iptmax)
{
	U_DB_CVM2_RGB2YUV_MAX DB_CVM2_RGB2YUV_MAX;
	
	DB_CVM2_RGB2YUV_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MAX.u32) ));
	DB_CVM2_RGB2YUV_MAX.bits.g_omap_rgb2yuvmax = g_imap_lms2iptmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MAX.u32) ),DB_CVM2_RGB2YUV_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvmin(HI_U32 g_imap_lms2iptmin)
{
	U_DB_CVM2_RGB2YUV_MIN DB_CVM2_RGB2YUV_MIN;
	
	DB_CVM2_RGB2YUV_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MIN.u32) ));
	DB_CVM2_RGB2YUV_MIN.bits.g_omap_rgb2yuvmin = g_imap_lms2iptmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MIN.u32) ),DB_CVM2_RGB2YUV_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvmax(HI_U32 g_imap_lms2iptmax)
{
	U_DB_CVM2_RGB2YUV_MAX DB_CVM2_RGB2YUV_MAX;
	
	DB_CVM2_RGB2YUV_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MAX.u32) ));
	DB_CVM2_RGB2YUV_MAX.bits.g_omap_rgb2yuvmax = g_imap_lms2iptmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_MAX.u32) ),DB_CVM2_RGB2YUV_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGCvmLms2IptOutDc(HI_S32 g_lms2ipt_out_dc[3])
{
	U_DB_CVM2_RGB2YUV_OUT_DC0 DB_CVM2_RGB2YUV_OUT_DC0;
	U_DB_CVM2_RGB2YUV_OUT_DC1 DB_CVM2_RGB2YUV_OUT_DC1;
	U_DB_CVM2_RGB2YUV_OUT_DC2 DB_CVM2_RGB2YUV_OUT_DC2;
	
	
	DB_CVM2_RGB2YUV_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC0.u32) ));
	DB_CVM2_RGB2YUV_OUT_DC0.bits.g_omap_rgb2yuv_0_out_dc = g_lms2ipt_out_dc[0];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC0.u32) ),DB_CVM2_RGB2YUV_OUT_DC0.u32);
	
	DB_CVM2_RGB2YUV_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC1.u32) ));
	DB_CVM2_RGB2YUV_OUT_DC1.bits.g_omap_rgb2yuv_1_out_dc = g_lms2ipt_out_dc[1];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC1.u32) ),DB_CVM2_RGB2YUV_OUT_DC1.u32);
	
	DB_CVM2_RGB2YUV_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC2.u32) ));
	DB_CVM2_RGB2YUV_OUT_DC2.bits.g_omap_rgb2yuv_2_out_dc = g_lms2ipt_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM2_RGB2YUV_OUT_DC2.u32) ),DB_CVM2_RGB2YUV_OUT_DC2.u32);
	
	return ;
}

*/

HI_VOID VDP_HDR_SetVDmImapLms2iptmin(HI_U32 v_imap_lms2iptmin)
{
	U_DB_CVM1_LMS2IPT_MIN DB_CVM1_LMS2IPT_MIN;
	
	
	DB_CVM1_LMS2IPT_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_MIN.u32) ));
	DB_CVM1_LMS2IPT_MIN.bits.v_imap_lms2iptmin = v_imap_lms2iptmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_MIN.u32) ),DB_CVM1_LMS2IPT_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImapLms2iptmax(HI_U32 v_imap_lms2iptmax)
{
	U_DB_CVM1_LMS2IPT_MAX DB_CVM1_LMS2IPT_MAX;
	
	
	DB_CVM1_LMS2IPT_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_MAX.u32) ));
	DB_CVM1_LMS2IPT_MAX.bits.v_imap_lms2iptmax = v_imap_lms2iptmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_MAX.u32) ),DB_CVM1_LMS2IPT_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmOmapIpt2lmsmin(HI_U32 v_omap_ipt2lmsmin)
{
	U_DB_CVM1_IPT2LMS_MIN DB_CVM1_IPT2LMS_MIN;
	
	
	DB_CVM1_IPT2LMS_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_MIN.u32) ));
	DB_CVM1_IPT2LMS_MIN.bits.v_omap_ipt2lmsmin = v_omap_ipt2lmsmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_MIN.u32) ),DB_CVM1_IPT2LMS_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmOmapIpt2lmsmax(HI_U32 v_omap_ipt2lmsmax)
{
	U_DB_CVM1_IPT2LMS_MAX DB_CVM1_IPT2LMS_MAX;
	
	
	DB_CVM1_IPT2LMS_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_MAX.u32) ));
	DB_CVM1_IPT2LMS_MAX.bits.v_omap_ipt2lmsmax = v_omap_ipt2lmsmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_IPT2LMS_MAX.u32) ),DB_CVM1_IPT2LMS_MAX.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmOmapLms2rgbmin(HI_U32 v_omap_lms2rgbmin)
{
	U_DB_CVM1_LMS2RGB_MIN DB_CVM1_LMS2RGB_MIN;
	
	
	DB_CVM1_LMS2RGB_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_MIN.u32) ));
	DB_CVM1_LMS2RGB_MIN.bits.v_omap_lms2rgbmin = v_omap_lms2rgbmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_MIN.u32) ),DB_CVM1_LMS2RGB_MIN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmOmapLms2rgbmax(HI_U32 v_omap_lms2rgbmax)
{
	U_DB_CVM1_LMS2RGB_MAX DB_CVM1_LMS2RGB_MAX;
	
	
	DB_CVM1_LMS2RGB_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_MAX.u32) ));
	DB_CVM1_LMS2RGB_MAX.bits.v_omap_lms2rgbmax = v_omap_lms2rgbmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2RGB_MAX.u32) ),DB_CVM1_LMS2RGB_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmImapIptoff(HI_S32 imap_iptoff[3])
{
	U_DB_VDM_IMAP_IPTOFF0     DB_VDM_IMAP_IPTOFF0 ;
    U_DB_VDM_IMAP_IPTOFF1     DB_VDM_IMAP_IPTOFF1 ; 
    U_DB_VDM_IMAP_IPTOFF2     DB_VDM_IMAP_IPTOFF2 ;
	
	DB_VDM_IMAP_IPTOFF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF0.u32) ));
	DB_VDM_IMAP_IPTOFF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF1.u32) ));
	DB_VDM_IMAP_IPTOFF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF2.u32) ));
	DB_VDM_IMAP_IPTOFF0.bits.v_imap_ipt_0_off = imap_iptoff[0];
	DB_VDM_IMAP_IPTOFF1.bits.v_imap_ipt_1_off = imap_iptoff[1];
	DB_VDM_IMAP_IPTOFF2.bits.v_imap_ipt_2_off = imap_iptoff[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF0.u32) ),DB_VDM_IMAP_IPTOFF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF1.u32) ),DB_VDM_IMAP_IPTOFF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTOFF2.u32) ),DB_VDM_IMAP_IPTOFF2.u32);
}

HI_VOID VDP_HDR_SetVDmImapIptScale(HI_U32 scale)
{
	U_DB_VDM_IMAP_IPTSCALE     DB_VDM_IMAP_IPTSCALE ;
	
	DB_VDM_IMAP_IPTSCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTSCALE.u32) ));
	DB_VDM_IMAP_IPTSCALE.bits.v_imap_ipt_scale = scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_IMAP_IPTSCALE.u32) ),DB_VDM_IMAP_IPTSCALE.u32);
}


HI_VOID VDP_HDR_SetVCvmOmapIptoff(HI_S32 omap_iptoff[3])
{
	U_DB_CVM1_OMAP_IPTOFF0     DB_CVM1_OMAP_IPTOFF0 ;
    U_DB_CVM1_OMAP_IPTOFF1     DB_CVM1_OMAP_IPTOFF1 ; 
    U_DB_CVM1_OMAP_IPTOFF2     DB_CVM1_OMAP_IPTOFF2 ;
	
	DB_CVM1_OMAP_IPTOFF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF0.u32) ));
	DB_CVM1_OMAP_IPTOFF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF1.u32) ));
	DB_CVM1_OMAP_IPTOFF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF2.u32) ));
	DB_CVM1_OMAP_IPTOFF0.bits.cvm_omap_ipt_0_off = omap_iptoff[0];
	DB_CVM1_OMAP_IPTOFF1.bits.cvm_omap_ipt_1_off = omap_iptoff[1];
	DB_CVM1_OMAP_IPTOFF2.bits.cvm_omap_ipt_2_off = omap_iptoff[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF0.u32) ),DB_CVM1_OMAP_IPTOFF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF1.u32) ),DB_CVM1_OMAP_IPTOFF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTOFF2.u32) ),DB_CVM1_OMAP_IPTOFF2.u32);
}


HI_VOID VDP_HDR_SetVCvmOmapIptScale(HI_U32 scale)
{
	U_DB_CVM1_OMAP_IPTSCALE     DB_CVM1_OMAP_IPTSCALE ;
	
	DB_CVM1_OMAP_IPTSCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTSCALE.u32) ));
	DB_CVM1_OMAP_IPTSCALE.bits.cvm_omap_ipt_scale = scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_OMAP_IPTSCALE.u32) ),DB_CVM1_OMAP_IPTSCALE.u32);
}


HI_VOID VDP_HDR_SetVDmGammaLutbRdEn(HI_U32 gamma_lutb_rd_en)
{
	U_DB_VDM_PARA_REN DB_VDM_PARA_REN;
	
	DB_VDM_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)));
	DB_VDM_PARA_REN.bits.gamma_lutb_rd_en = gamma_lutb_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)),DB_VDM_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmGammaLutaRdEn(HI_U32 gamma_luta_rd_en)
{
	U_DB_VDM_PARA_REN DB_VDM_PARA_REN;
	
	DB_VDM_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)));
	DB_VDM_PARA_REN.bits.gamma_luta_rd_en = gamma_luta_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)),DB_VDM_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmGammaLutxRdEn(HI_U32 gamma_lutx_rd_en)
{
	U_DB_VDM_PARA_REN DB_VDM_PARA_REN;
	
	DB_VDM_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)));
	DB_VDM_PARA_REN.bits.gamma_lutx_rd_en = gamma_lutx_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_REN.u32)),DB_VDM_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmPq2lRdEn(HI_U32 pq2l_rd_en)
{
	U_DB_CVM1_PARA_REN DB_CVM1_PARA_REN;
	
	DB_CVM1_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32)));
	DB_CVM1_PARA_REN.bits.pq2l_rd_en = pq2l_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_REN.u32)),DB_CVM1_PARA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVCvmDegmmCoefAddr(HI_U32 degamma_coef_addr)
{
	U_DB_CVM1_DEGAMMA_COEFAD DB_CVM1_DEGAMMA_COEFAD;
	
	DB_CVM1_DEGAMMA_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_DEGAMMA_COEFAD.u32)));
	DB_CVM1_DEGAMMA_COEFAD.bits.degamma_coef_addr = degamma_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_DEGAMMA_COEFAD.u32)),DB_CVM1_DEGAMMA_COEFAD.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVCvmGammaCoefAddr(HI_U32 gamma_coef_addr)
{
	U_DB_CVM1_GAMMA_COEFAD DB_CVM1_GAMMA_COEFAD;
	
	DB_CVM1_GAMMA_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_GAMMA_COEFAD.u32)));
	DB_CVM1_GAMMA_COEFAD.bits.gamma_coef_addr = gamma_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_GAMMA_COEFAD.u32)),DB_CVM1_GAMMA_COEFAD.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVCvmVcvmCoefAddr(HI_U32 vcvm_coef_addr)
{
	U_DB_CVM1_VCVM_COEFAD DB_CVM1_VCVM_COEFAD;
	
	DB_CVM1_VCVM_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_VCVM_COEFAD.u32)));
	DB_CVM1_VCVM_COEFAD.bits.vcvm_coef_addr = vcvm_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_VCVM_COEFAD.u32)),DB_CVM1_VCVM_COEFAD.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVDmDegmmCoefAddr(HI_U32 vdm_coef_addr)
{
	U_DB_VDM_COEFAD DB_VDM_COEFAD;
	
	DB_VDM_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_COEFAD.u32)));
	DB_VDM_COEFAD.bits.vdm_coef_addr = vdm_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_COEFAD.u32)),DB_VDM_COEFAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetVDmGammaCoefAddr(HI_U32 vdm_coef_addr1)
{
	U_DB_VDM_COEFAD1 DB_VDM_COEFAD1;
	
	DB_VDM_COEFAD1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_COEFAD1.u32)));
	DB_VDM_COEFAD1.bits.vdm_coef_addr1 = vdm_coef_addr1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_COEFAD1.u32)),DB_VDM_COEFAD1.u32);
	
	return ;
}



HI_VOID VDP_HDR_SetDmHdrDsEn(HI_U32 hdr_ds_en)
{
	U_DB_HDR_DS_CTRL DB_HDR_DS_CTRL;
	
	DB_HDR_DS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CTRL.u32)));
	DB_HDR_DS_CTRL.bits.hdr_ds_en = hdr_ds_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CTRL.u32)),DB_HDR_DS_CTRL.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrDsMax(HI_U32 hdr_ds_max)
{
	U_DB_HDR_DS_CLIP DB_HDR_DS_CLIP;
	
	DB_HDR_DS_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CLIP.u32)));
	DB_HDR_DS_CLIP.bits.hdr_ds_max = hdr_ds_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CLIP.u32)),DB_HDR_DS_CLIP.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrDsMin(HI_U32 hdr_ds_min)
{
	U_DB_HDR_DS_CLIP DB_HDR_DS_CLIP;
	
	DB_HDR_DS_CLIP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CLIP.u32)));
	DB_HDR_DS_CLIP.bits.hdr_ds_min = hdr_ds_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_DS_CLIP.u32)),DB_HDR_DS_CLIP.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdLen(HI_U32 hdr_md_len)
{
	U_DB_HDR_METADATA_CTRL DB_HDR_METADATA_CTRL;
	
	DB_HDR_METADATA_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_CTRL.u32)));
	DB_HDR_METADATA_CTRL.bits.hdr_md_len = hdr_md_len;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_CTRL.u32)),DB_HDR_METADATA_CTRL.u32);
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdEn(HI_U32 hdr_md_en)
{
	U_DB_HDR_METADATA_CTRL DB_HDR_METADATA_CTRL;
	
	DB_HDR_METADATA_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_CTRL.u32)));
	DB_HDR_METADATA_CTRL.bits.hdr_md_en = hdr_md_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_CTRL.u32)),DB_HDR_METADATA_CTRL.u32);
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdEos(HI_U32 hdr_md_eos)
{
	U_DB_HDR_METADATA_HEAD DB_HDR_METADATA_HEAD;
	
	DB_HDR_METADATA_HEAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)));
	DB_HDR_METADATA_HEAD.bits.hdr_md_eos = hdr_md_eos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)),DB_HDR_METADATA_HEAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdId(HI_U32 hdr_md_id)
{
	U_DB_HDR_METADATA_HEAD DB_HDR_METADATA_HEAD;
	
	DB_HDR_METADATA_HEAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)));
	DB_HDR_METADATA_HEAD.bits.hdr_md_id = hdr_md_id;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)),DB_HDR_METADATA_HEAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdType(HI_U32 hdr_md_type)
{
	U_DB_HDR_METADATA_HEAD DB_HDR_METADATA_HEAD;
	
	DB_HDR_METADATA_HEAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)));
	DB_HDR_METADATA_HEAD.bits.hdr_md_type = hdr_md_type;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)),DB_HDR_METADATA_HEAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrMdVer(HI_U32 hdr_md_ver)
{
	U_DB_HDR_METADATA_HEAD DB_HDR_METADATA_HEAD;
	
	DB_HDR_METADATA_HEAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)));
	DB_HDR_METADATA_HEAD.bits.hdr_md_ver = hdr_md_ver;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)),DB_HDR_METADATA_HEAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmHdrNoMd(HI_U32 hdr_no_md)
{
	U_DB_HDR_METADATA_HEAD DB_HDR_METADATA_HEAD;
	
	DB_HDR_METADATA_HEAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)));
	DB_HDR_METADATA_HEAD.bits.hdr_no_md = hdr_no_md;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_METADATA_HEAD.u32)),DB_HDR_METADATA_HEAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmMetadataRdEn(HI_U32 metadata_rd_en)
{
	U_DB_METADATA_PARA_REN DB_METADATA_PARA_REN;
	
	DB_METADATA_PARA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_METADATA_PARA_REN.u32)));
	DB_METADATA_PARA_REN.bits.metadata_rd_en = metadata_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_METADATA_PARA_REN.u32)),DB_METADATA_PARA_REN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetDmMdParaup(HI_VOID)
{
	U_DB_METADATA_PARA_UP DB_METADATA_PARA_UP;
	
	DB_METADATA_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_METADATA_PARA_UP.u32)));
	DB_METADATA_PARA_UP.bits.metadata_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_METADATA_PARA_UP.u32)),DB_METADATA_PARA_UP.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmInputDegmmParaup(HI_VOID)
{
	U_DB_CVM1_PARA_UP DB_CVM1_PARA_UP;
	
	DB_CVM1_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)));
	DB_CVM1_PARA_UP.bits.degamma_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)),DB_CVM1_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetDmInputGammaParaup(HI_VOID)
{
	U_DB_CVM1_PARA_UP DB_CVM1_PARA_UP;
	
	DB_CVM1_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)));
	DB_CVM1_PARA_UP.bits.gamma_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)),DB_CVM1_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetDmMetadataCoefAddr(HI_U32 metadata_coef_addr)
{
	U_DB_METADATA_COEFAD DB_METADATA_COEFAD;
	
	DB_METADATA_COEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_METADATA_COEFAD.u32)));
	DB_METADATA_COEFAD.bits.metadata_coef_addr = metadata_coef_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_METADATA_COEFAD.u32)),DB_METADATA_COEFAD.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetDmCvmParaup(HI_VOID)
{
	U_DB_CVM1_PARA_UP DB_CVM1_PARA_UP;
	
	DB_CVM1_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)));
	DB_CVM1_PARA_UP.bits.vcvm_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_PARA_UP.u32)),DB_CVM1_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetDmOutputDegmmParaup(HI_VOID)
{
	U_DB_VDM_PARA_UP DB_VDM_PARA_UP;
	
	DB_VDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_UP.u32)));
	DB_VDM_PARA_UP.bits.vdm_coef_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_UP.u32)),DB_VDM_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetDmOutputGammaParaup(HI_VOID)
{
	U_DB_VDM_PARA_UP DB_VDM_PARA_UP;
	
	DB_VDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_UP.u32)));
	DB_VDM_PARA_UP.bits.vdm_coef1_upd = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_VDM_PARA_UP.u32)),DB_VDM_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetHdrEnable(HI_U32 hdr_en)
{
	U_DB_HDR_CTRL DB_HDR_CTRL;
	
	DB_HDR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_CTRL.u32)));
	DB_HDR_CTRL.bits.hdr_en = hdr_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_CTRL.u32)),DB_HDR_CTRL.u32);
	
	return ;
}

//HI_VOID VDP_HDR_SetDmHdrDsScale2p(HI_U32 hdr_ds_scale2p)
//{
//	U_HDR_DS_CTRL HDR_DS_CTRL;
//	
//	
//	HDR_DS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->HDR_DS_CTRL.u32) ));
//	HDR_DS_CTRL.bits.hdr_ds_scale2p = hdr_ds_scale2p;
//	VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDR_DS_CTRL.u32) ),HDR_DS_CTRL.u32);
//	
//	return ;
//}


HI_VOID VDP_HDR_SetDmEdrEn(HI_U32 edr_en)
{
	U_DB_HDR_CTRL DB_HDR_CTRL;
	
	DB_HDR_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_CTRL.u32)));
	DB_HDR_CTRL.bits.edr_en = edr_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_CTRL.u32)),DB_HDR_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetRegUp()
{
	U_DB_HDR_UPD DB_HDR_UPD;
	
	DB_HDR_UPD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_HDR_UPD.u32)));
	DB_HDR_UPD.bits.regup = 1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_HDR_UPD.u32)),DB_HDR_UPD.u32);
	
	return ;
}

// -------------gdm bt2020 ----------------
//
HI_VOID VDP_HDR_SetGDmEn(HI_U32 g_gdm_en)
{
	U_DB_GDM_CTRL DB_GDM_CTRL;
	
	
	DB_GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ));
	DB_GDM_CTRL.bits.g_gdm_en = g_gdm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ),DB_GDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmDegammaEn(HI_U32 g_degamma_en)
{
	U_DB_GDM_CTRL DB_GDM_CTRL;
	
	
	DB_GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ));
	DB_GDM_CTRL.bits.g_degamma_en = g_degamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ),DB_GDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmGammaEn(HI_U32 g_gamma_en)
{
	U_DB_GDM_CTRL DB_GDM_CTRL;
	
	
	DB_GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ));
	DB_GDM_CTRL.bits.g_gamma_en = g_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ),DB_GDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetAuthEn(HI_U32 gauth_en)
{
	U_DB_GDM_CTRL DB_GDM_CTRL;
	
	
	DB_GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ));
	DB_GDM_CTRL.bits.g_gdm_auth_en = gauth_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_CTRL.u32) ),DB_GDM_CTRL.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGdmParaUpd(HI_U32 u32Data, VDP_GP_PARA_E enMode)
{
	U_DB_GDM_COEFF_UPD  DB_GDM_COEFF_UPD;
	
    if (enMode == VDP_GP_PARA_HDR_DEGMM)
    {
        DB_GDM_COEFF_UPD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_COEFF_UPD.u32)));
        DB_GDM_COEFF_UPD.bits.degamm_coeff_upd = 0x1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_COEFF_UPD.u32)),DB_GDM_COEFF_UPD.u32);
    }
	
    else if (enMode == VDP_GP_PARA_HDR_GMM)
    {
        DB_GDM_COEFF_UPD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_COEFF_UPD.u32)));
        DB_GDM_COEFF_UPD.bits.gamm_coeff_upd = 0x1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_COEFF_UPD.u32)),DB_GDM_COEFF_UPD.u32);
    }
	return ;
}


HI_VOID VDP_GP_SetGdmDegammAddr(HI_U32 u32Data, HI_U32 u32GdmDegammAddr)
{
    U_DB_GDM_DEGAMM_ADDR DB_GDM_DEGAMM_ADDR;

    DB_GDM_DEGAMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMM_ADDR.u32)));
    DB_GDM_DEGAMM_ADDR.bits.degamm_coeff_addr = u32GdmDegammAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMM_ADDR.u32)),DB_GDM_DEGAMM_ADDR.u32);

}

HI_VOID VDP_GP_SetGdmGammAddr(HI_U32 u32Data, HI_U32 u32GdmGammAddr)
{
    U_DB_GDM_GAMM_ADDR DB_GDM_GAMM_ADDR;

    DB_GDM_GAMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMM_ADDR.u32)));
    DB_GDM_GAMM_ADDR.bits.gamm_coeff_addr = u32GdmGammAddr;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMM_ADDR.u32)),DB_GDM_GAMM_ADDR.u32);

}

HI_VOID VDP_HDR_SetGDmDegmmStep(HI_U32 g_degmm_step[4])
{
	U_DB_GDM_DEGAMMA_STEP DB_GDM_DEGAMMA_STEP;
	
	DB_GDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_STEP.u32)));
	DB_GDM_DEGAMMA_STEP.bits.g_degmm_x1_step = g_degmm_step[0];
	DB_GDM_DEGAMMA_STEP.bits.g_degmm_x2_step = g_degmm_step[1];
	DB_GDM_DEGAMMA_STEP.bits.g_degmm_x3_step = g_degmm_step[2];
	DB_GDM_DEGAMMA_STEP.bits.g_degmm_x4_step = g_degmm_step[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_STEP.u32)),DB_GDM_DEGAMMA_STEP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmGammaStep(HI_U32 g_gmm_step[4])
{
	U_DB_GDM_GAMMA_STEP DB_GDM_GAMMA_STEP;
	
	DB_GDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_STEP.u32)));
	DB_GDM_GAMMA_STEP.bits.g_gmm_x1_step = g_gmm_step[0];
	DB_GDM_GAMMA_STEP.bits.g_gmm_x2_step = g_gmm_step[1];
	DB_GDM_GAMMA_STEP.bits.g_gmm_x3_step = g_gmm_step[2];
	DB_GDM_GAMMA_STEP.bits.g_gmm_x4_step = g_gmm_step[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_STEP.u32)),DB_GDM_GAMMA_STEP.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmDegmmNum(HI_U32 g_degmm_num[4])
{
	U_DB_GDM_DEGAMMA_NUM DB_GDM_DEGAMMA_NUM;
	
	DB_GDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_NUM.u32)));
	DB_GDM_DEGAMMA_NUM.bits.g_degmm_x1_num  = g_degmm_num[0];
	DB_GDM_DEGAMMA_NUM.bits.g_degmm_x2_num  = g_degmm_num[1];
	DB_GDM_DEGAMMA_NUM.bits.g_degmm_x3_num  = g_degmm_num[2];
	DB_GDM_DEGAMMA_NUM.bits.g_degmm_x4_num  = g_degmm_num[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_NUM.u32)),DB_GDM_DEGAMMA_NUM.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmGmmNum(HI_U32 g_gmm_num[4])
{
	U_DB_GDM_GAMMA_NUM DB_GDM_GAMMA_NUM;
	
	DB_GDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_NUM.u32)));
	DB_GDM_GAMMA_NUM.bits.g_gmm_x1_num  = g_gmm_num[0];
	DB_GDM_GAMMA_NUM.bits.g_gmm_x2_num  = g_gmm_num[1];
	DB_GDM_GAMMA_NUM.bits.g_gmm_x3_num  = g_gmm_num[2];
	DB_GDM_GAMMA_NUM.bits.g_gmm_x4_num  = g_gmm_num[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_NUM.u32)),DB_GDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmDegmmPos(HI_U32 g_degmm_pos[4])
{
	U_DB_GDM_DEGAMMA_POS DB_GDM_DEGAMMA_POS;
	
	DB_GDM_DEGAMMA_POS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_POS.u32)));
	DB_GDM_DEGAMMA_POS.bits.g_degmm_x1_pos = g_degmm_pos[0];
	DB_GDM_DEGAMMA_POS.bits.g_degmm_x2_pos = g_degmm_pos[1];
	DB_GDM_DEGAMMA_POS.bits.g_degmm_x3_pos = g_degmm_pos[2];
	DB_GDM_DEGAMMA_POS.bits.g_degmm_x4_pos = g_degmm_pos[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_POS.u32)),DB_GDM_DEGAMMA_POS.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmGammaPos(HI_U32 g_gmm_pos[4])
{
	U_DB_GDM_GAMMA_POS0 DB_GDM_GAMMA_POS0;
	U_DB_GDM_GAMMA_POS1 DB_GDM_GAMMA_POS1;
	U_DB_GDM_GAMMA_POS2 DB_GDM_GAMMA_POS2;
	U_DB_GDM_GAMMA_POS3 DB_GDM_GAMMA_POS3;
	
	DB_GDM_GAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS0.u32)));
	DB_GDM_GAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS1.u32)));
	DB_GDM_GAMMA_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS2.u32)));
	DB_GDM_GAMMA_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS3.u32)));
	DB_GDM_GAMMA_POS0.bits.g_gmm_x1_pos = g_gmm_pos[0];
	DB_GDM_GAMMA_POS2.bits.g_gmm_x2_pos = g_gmm_pos[1];
	DB_GDM_GAMMA_POS1.bits.g_gmm_x3_pos = g_gmm_pos[2];
	DB_GDM_GAMMA_POS3.bits.g_gmm_x4_pos = g_gmm_pos[3];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS0.u32)),DB_GDM_GAMMA_POS0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS1.u32)),DB_GDM_GAMMA_POS1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS2.u32)),DB_GDM_GAMMA_POS2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_POS3.u32)),DB_GDM_GAMMA_POS3.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmGammaRdEn(HI_U32 gamma_rd_en)
{
	U_DB_GDM_GAMMA_REN DB_GDM_GAMMA_REN;
	
	
	DB_GDM_GAMMA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_REN.u32) ));
	DB_GDM_GAMMA_REN.bits.gamma_rd_en = gamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_GAMMA_REN.u32) ),DB_GDM_GAMMA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmDegammaRdEn(HI_U32 degamma_rd_en)
{
	U_DB_GDM_DEGAMMA_REN DB_GDM_DEGAMMA_REN;
	
	
	DB_GDM_DEGAMMA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_REN.u32) ));
	DB_GDM_DEGAMMA_REN.bits.degamma_rd_en = degamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_DEGAMMA_REN.u32) ),DB_GDM_DEGAMMA_REN.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmRgb2lmsEn(HI_U32 g_rgb2lms_en)
{
	U_DB_GDM_RGB2LMS_CTRL DB_GDM_RGB2LMS_CTRL;
	
	
	DB_GDM_RGB2LMS_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_CTRL.u32) ));
	DB_GDM_RGB2LMS_CTRL.bits.gdm_rgb2lms_en = g_rgb2lms_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_CTRL.u32) ),DB_GDM_RGB2LMS_CTRL.u32);
	
	return ;
}



HI_VOID VDP_HDR_SetGDmImapRgb2lms(HI_S16 g_imap_rgb2lms[3][3])
{
	U_DB_GDM_RGB2LMS_COEF0 DB_GDM_RGB2LMS_COEF0;
	U_DB_GDM_RGB2LMS_COEF1 DB_GDM_RGB2LMS_COEF1;
	U_DB_GDM_RGB2LMS_COEF2 DB_GDM_RGB2LMS_COEF2;
	U_DB_GDM_RGB2LMS_COEF3 DB_GDM_RGB2LMS_COEF3;
	U_DB_GDM_RGB2LMS_COEF4 DB_GDM_RGB2LMS_COEF4;
	U_DB_GDM_RGB2LMS_COEF5 DB_GDM_RGB2LMS_COEF5;
	U_DB_GDM_RGB2LMS_COEF6 DB_GDM_RGB2LMS_COEF6;
	U_DB_GDM_RGB2LMS_COEF7 DB_GDM_RGB2LMS_COEF7;
	U_DB_GDM_RGB2LMS_COEF8 DB_GDM_RGB2LMS_COEF8;
	
	
	DB_GDM_RGB2LMS_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF0.u32) ));
	DB_GDM_RGB2LMS_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF1.u32) ));
	DB_GDM_RGB2LMS_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF2.u32) ));
	DB_GDM_RGB2LMS_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF3.u32) ));
	DB_GDM_RGB2LMS_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF4.u32) ));
	DB_GDM_RGB2LMS_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF5.u32) ));
	DB_GDM_RGB2LMS_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF6.u32) ));
	DB_GDM_RGB2LMS_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF7.u32) ));
	DB_GDM_RGB2LMS_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF8.u32) ));
	DB_GDM_RGB2LMS_COEF0.bits.g_imap_00_rgb2lms = g_imap_rgb2lms[0][0];
	DB_GDM_RGB2LMS_COEF1.bits.g_imap_01_rgb2lms = g_imap_rgb2lms[0][1];
	DB_GDM_RGB2LMS_COEF2.bits.g_imap_02_rgb2lms = g_imap_rgb2lms[0][2];
	DB_GDM_RGB2LMS_COEF3.bits.g_imap_10_rgb2lms = g_imap_rgb2lms[1][0];
	DB_GDM_RGB2LMS_COEF4.bits.g_imap_11_rgb2lms = g_imap_rgb2lms[1][1];
	DB_GDM_RGB2LMS_COEF5.bits.g_imap_12_rgb2lms = g_imap_rgb2lms[1][2];
	DB_GDM_RGB2LMS_COEF6.bits.g_imap_20_rgb2lms = g_imap_rgb2lms[2][0];
	DB_GDM_RGB2LMS_COEF7.bits.g_imap_21_rgb2lms = g_imap_rgb2lms[2][1];
	DB_GDM_RGB2LMS_COEF8.bits.g_imap_22_rgb2lms = g_imap_rgb2lms[2][2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF0.u32) ),DB_GDM_RGB2LMS_COEF0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF1.u32) ),DB_GDM_RGB2LMS_COEF1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF2.u32) ),DB_GDM_RGB2LMS_COEF2.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF3.u32) ),DB_GDM_RGB2LMS_COEF3.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF4.u32) ),DB_GDM_RGB2LMS_COEF4.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF5.u32) ),DB_GDM_RGB2LMS_COEF5.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF6.u32) ),DB_GDM_RGB2LMS_COEF6.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF7.u32) ),DB_GDM_RGB2LMS_COEF7.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_COEF8.u32) ),DB_GDM_RGB2LMS_COEF8.u32);
	
	return ;
}


HI_VOID VDP_HDR_SetGDmImapRgb2lmsscale2p(HI_U32 g_imap_rgb2lmsscale2p)
{
	U_DB_GDM_RGB2LMS_SCALE2P DB_GDM_RGB2LMS_SCALE2P;
	
	
	DB_GDM_RGB2LMS_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_SCALE2P.u32) ));
	DB_GDM_RGB2LMS_SCALE2P.bits.g_imap_rgb2lmsscale2p = g_imap_rgb2lmsscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_SCALE2P.u32) ),DB_GDM_RGB2LMS_SCALE2P.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmImapRgb2lmsmin(HI_U32 g_imap_rgb2lmsmin)
{
	U_DB_GDM_RGB2LMS_MIN DB_GDM_RGB2LMS_MIN;
	
	
	DB_GDM_RGB2LMS_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_MIN.u32) ));
	DB_GDM_RGB2LMS_MIN.bits.g_imap_rgb2lmsmin = g_imap_rgb2lmsmin;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_MIN.u32) ),DB_GDM_RGB2LMS_MIN.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmImapRgb2lmsmax(HI_U32 g_imap_rgb2lmsmax)
{
	U_DB_GDM_RGB2LMS_MAX DB_GDM_RGB2LMS_MAX;
	
	
	DB_GDM_RGB2LMS_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_MAX.u32) ));
	DB_GDM_RGB2LMS_MAX.bits.g_imap_rgb2lmsmax = g_imap_rgb2lmsmax;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_MAX.u32) ),DB_GDM_RGB2LMS_MAX.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetGDmImapRgb2lmsOutDc(HI_S32 g_imap_rgb2lms_out_dc[3])
{
	U_DB_GDM_RGB2LMS_OUT_DC0 DB_GDM_RGB2LMS_OUT_DC0;
	U_DB_GDM_RGB2LMS_OUT_DC1 DB_GDM_RGB2LMS_OUT_DC1;
	U_DB_GDM_RGB2LMS_OUT_DC2 DB_GDM_RGB2LMS_OUT_DC2;
	
	
	DB_GDM_RGB2LMS_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC0.u32) ));
    DB_GDM_RGB2LMS_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC1.u32) ));
	DB_GDM_RGB2LMS_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC2.u32) ));
	DB_GDM_RGB2LMS_OUT_DC0.bits.g_imap_rgb2lms_0_out_dc = g_imap_rgb2lms_out_dc[0];
	DB_GDM_RGB2LMS_OUT_DC1.bits.g_imap_rgb2lms_1_out_dc = g_imap_rgb2lms_out_dc[1];
	DB_GDM_RGB2LMS_OUT_DC2.bits.g_imap_rgb2lms_2_out_dc = g_imap_rgb2lms_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC0.u32) ),DB_GDM_RGB2LMS_OUT_DC0.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC1.u32) ),DB_GDM_RGB2LMS_OUT_DC1.u32);
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_GDM_RGB2LMS_OUT_DC2.u32) ),DB_GDM_RGB2LMS_OUT_DC2.u32);
	
	return ;
}

HI_VOID VDP_HDR_SetVCvmImapLms2iptOutDc(HI_S32  * v_imap_lms2ipt_out_dc)
{

    U_DB_CVM1_LMS2IPT_OUT_DC0   DB_CVM1_LMS2IPT_OUT_DC0; /* 0x3a34 */
    U_DB_CVM1_LMS2IPT_OUT_DC1   DB_CVM1_LMS2IPT_OUT_DC1; /* 0x3a38 */
    U_DB_CVM1_LMS2IPT_OUT_DC2   DB_CVM1_LMS2IPT_OUT_DC2; /* 0x3a3c */

	
	DB_CVM1_LMS2IPT_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC0.u32) ));
	DB_CVM1_LMS2IPT_OUT_DC0.bits.v_imap_lms2ipt_0_out_dc = v_imap_lms2ipt_out_dc[0];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC0.u32) ),DB_CVM1_LMS2IPT_OUT_DC0.u32);
	
	DB_CVM1_LMS2IPT_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC1.u32) ));
	DB_CVM1_LMS2IPT_OUT_DC1.bits.v_imap_lms2ipt_1_out_dc = v_imap_lms2ipt_out_dc[1];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC1.u32) ),DB_CVM1_LMS2IPT_OUT_DC1.u32);
	
	DB_CVM1_LMS2IPT_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC2.u32) ));
	DB_CVM1_LMS2IPT_OUT_DC2.bits.v_imap_lms2ipt_2_out_dc = v_imap_lms2ipt_out_dc[2];
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->DB_CVM1_LMS2IPT_OUT_DC2.u32) ),DB_CVM1_LMS2IPT_OUT_DC2.u32);
	
	return ;
}


