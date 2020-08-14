
#include "vdp_hal_ip_vdm.h"
#include "vdp_hal_comm.h"
#include "vdp_define.h"


extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_VID_SetVdmVdmTmapInBdp(HI_U32 u32Data, HI_U32 vdm_tmap_in_bdp)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapInBdp Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_tmap_in_bdp = vdm_tmap_in_bdp;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCscBind(HI_U32 u32Data, HI_U32 vdm_csc_bind)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCscBind Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_csc_bind = vdm_csc_bind;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmGmmEn(HI_U32 u32Data, HI_U32 vdm_gmm_en)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmGmmEn Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_gmm_en = vdm_gmm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapEn(HI_U32 u32Data, HI_U32 vdm_tmap_en)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapEn Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_tmap_en = vdm_tmap_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDegmmEn(HI_U32 u32Data, HI_U32 vdm_degmm_en)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDegmmEn Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_degmm_en = vdm_degmm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmEn(HI_U32 u32Data, HI_U32 vdm_en)
{
	U_VDM_CTRL VDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmEn Select Wrong Layer ID\n");
	}
	
	VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CTRL.bits.vdm_en = vdm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32) + u32Data * VID_OFFSET),VDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmLumaCoef0Tmap(HI_U32 u32Data, HI_U32 vdm_luma_coef0_tmap)
{
	U_VDM_TMAP_LUMA_COEF0 VDM_TMAP_LUMA_COEF0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmLumaCoef0Tmap Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_LUMA_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF0.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_LUMA_COEF0.bits.vdm_luma_coef0_tmap = vdm_luma_coef0_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF0.u32) + u32Data * VID_OFFSET),VDM_TMAP_LUMA_COEF0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmLumaCoef1Tmap(HI_U32 u32Data, HI_U32 vdm_luma_coef1_tmap)
{
	U_VDM_TMAP_LUMA_COEF1 VDM_TMAP_LUMA_COEF1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmLumaCoef1Tmap Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_LUMA_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF1.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_LUMA_COEF1.bits.vdm_luma_coef1_tmap = vdm_luma_coef1_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF1.u32) + u32Data * VID_OFFSET),VDM_TMAP_LUMA_COEF1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmLumaCoef2Tmap(HI_U32 u32Data, HI_U32 vdm_luma_coef2_tmap)
{
	U_VDM_TMAP_LUMA_COEF2 VDM_TMAP_LUMA_COEF2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmLumaCoef2Tmap Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_LUMA_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF2.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_LUMA_COEF2.bits.vdm_luma_coef2_tmap = vdm_luma_coef2_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_COEF2.u32) + u32Data * VID_OFFSET),VDM_TMAP_LUMA_COEF2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapLumaScale(HI_U32 u32Data, HI_U32 vdm_tmap_luma_scale)
{
	U_VDM_TMAP_LUMA_SCALE VDM_TMAP_LUMA_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapLumaScale Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_LUMA_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_SCALE.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_LUMA_SCALE.bits.vdm_tmap_luma_scale = vdm_tmap_luma_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_LUMA_SCALE.u32) + u32Data * VID_OFFSET),VDM_TMAP_LUMA_SCALE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapCoefScale(HI_U32 u32Data, HI_U32 vdm_tmap_coef_scale)
{
	U_VDM_TMAP_COEF_SCALE VDM_TMAP_COEF_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapCoefScale Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_COEF_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_COEF_SCALE.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_COEF_SCALE.bits.vdm_tmap_coef_scale = vdm_tmap_coef_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_COEF_SCALE.u32) + u32Data * VID_OFFSET),VDM_TMAP_COEF_SCALE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapOutClipMin(HI_U32 u32Data, HI_U32 vdm_tmap_out_clip_min)
{
	U_VDM_TMAP_OUT_CLIP_MIN VDM_TMAP_OUT_CLIP_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapOutClipMin Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_OUT_CLIP_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_CLIP_MIN.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_OUT_CLIP_MIN.bits.vdm_tmap_out_clip_min = vdm_tmap_out_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_CLIP_MIN.u32) + u32Data * VID_OFFSET),VDM_TMAP_OUT_CLIP_MIN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapOutClipMax(HI_U32 u32Data, HI_U32 vdm_tmap_out_clip_max)
{
	U_VDM_TMAP_OUT_CLIP_MAX VDM_TMAP_OUT_CLIP_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapOutClipMax Select Wrong Layer ID\n");
	}
	
	VDM_TMAP_OUT_CLIP_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_CLIP_MAX.u32) + u32Data * VID_OFFSET));
	VDM_TMAP_OUT_CLIP_MAX.bits.vdm_tmap_out_clip_max = vdm_tmap_out_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_CLIP_MAX.u32) + u32Data * VID_OFFSET),VDM_TMAP_OUT_CLIP_MAX.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaDegmmAddr(HI_U32 u32Data, HI_U32 vdm_para_degmm_addr)
{
	U_VDM_PARA_DEGMM_ADDR VDM_PARA_DEGMM_ADDR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaDegmmAddr Select Wrong Layer ID\n");
	}
	
	VDM_PARA_DEGMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_DEGMM_ADDR.u32) + u32Data * VID_OFFSET));
	VDM_PARA_DEGMM_ADDR.bits.vdm_para_degmm_addr = vdm_para_degmm_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_DEGMM_ADDR.u32) + u32Data * VID_OFFSET),VDM_PARA_DEGMM_ADDR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaGmmAddr(HI_U32 u32Data, HI_U32 vdm_para_gmm_addr)
{
	U_VDM_PARA_GMM_ADDR VDM_PARA_GMM_ADDR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaGmmAddr Select Wrong Layer ID\n");
	}
	
	VDM_PARA_GMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_GMM_ADDR.u32) + u32Data * VID_OFFSET));
	VDM_PARA_GMM_ADDR.bits.vdm_para_gmm_addr = vdm_para_gmm_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_GMM_ADDR.u32) + u32Data * VID_OFFSET),VDM_PARA_GMM_ADDR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaTmapAddr(HI_U32 u32Data, HI_U32 vdm_para_tmap_addr)
{
	U_VDM_PARA_TMAP_ADDR VDM_PARA_TMAP_ADDR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaTmapAddr Select Wrong Layer ID\n");
	}
	
	VDM_PARA_TMAP_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_TMAP_ADDR.u32) + u32Data * VID_OFFSET));
	VDM_PARA_TMAP_ADDR.bits.vdm_para_tmap_addr = vdm_para_tmap_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_TMAP_ADDR.u32) + u32Data * VID_OFFSET),VDM_PARA_TMAP_ADDR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaTmapUpd(HI_U32 u32Data, HI_U32 vdm_para_tmap_upd)
{
	U_VDM_PARA_UP VDM_PARA_UP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaTmapUpd Select Wrong Layer ID\n");
	}
	
	VDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET));
	VDM_PARA_UP.bits.vdm_para_tmap_upd = vdm_para_tmap_upd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET),VDM_PARA_UP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaGmmUpd(HI_U32 u32Data, HI_U32 vdm_para_gmm_upd)
{
	U_VDM_PARA_UP VDM_PARA_UP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaGmmUpd Select Wrong Layer ID\n");
	}
	
	VDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET));
	VDM_PARA_UP.bits.vdm_para_gmm_upd = vdm_para_gmm_upd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET),VDM_PARA_UP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmParaDegmmUpd(HI_U32 u32Data, HI_U32 vdm_para_degmm_upd)
{
	U_VDM_PARA_UP VDM_PARA_UP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmParaDegmmUpd Select Wrong Layer ID\n");
	}
	
	VDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET));
	VDM_PARA_UP.bits.vdm_para_degmm_upd = vdm_para_degmm_upd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_PARA_UP.u32) + u32Data * VID_OFFSET),VDM_PARA_UP.u32);
	
	return ;
}

HI_VOID VDP_VID_SetVdmTmapOutDc0(HI_U32 vdm_tmap_out_dc0)
{
	U_VDM_TMAP_OUT_DC0 VDM_TMAP_OUT_DC0;
	
	VDM_TMAP_OUT_DC0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC0.u32));
	VDM_TMAP_OUT_DC0.bits.vdm_tmap_out_dc0 = vdm_tmap_out_dc0;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC0.u32),VDM_TMAP_OUT_DC0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmTmapOutDc1(HI_U32 vdm_tmap_out_dc1)
{
	U_VDM_TMAP_OUT_DC1 VDM_TMAP_OUT_DC1;
	
	VDM_TMAP_OUT_DC1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC1.u32));
	VDM_TMAP_OUT_DC1.bits.vdm_tmap_out_dc1 = vdm_tmap_out_dc1;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC1.u32),VDM_TMAP_OUT_DC1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmTmapOutDc2(HI_U32 vdm_tmap_out_dc2)
{
	U_VDM_TMAP_OUT_DC2 VDM_TMAP_OUT_DC2;
	
	VDM_TMAP_OUT_DC2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC2.u32));
	VDM_TMAP_OUT_DC2.bits.vdm_tmap_out_dc2 = vdm_tmap_out_dc2;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TMAP_OUT_DC2.u32),VDM_TMAP_OUT_DC2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDegammaRdEn(HI_U32 u32Data, HI_U32 vdm_degamma_rd_en)
{
	U_VDM_DEGMM_REN VDM_DEGMM_REN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDegammaRdEn Select Wrong Layer ID\n");
	}
	
	VDM_DEGMM_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGMM_REN.u32) + u32Data * VID_OFFSET));
	VDM_DEGMM_REN.bits.vdm_degamma_rd_en = vdm_degamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGMM_REN.u32) + u32Data * VID_OFFSET),VDM_DEGMM_REN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDegammaParaData(HI_U32 u32Data, HI_U32 vdm_degamma_para_data)
{
	U_VDM_DEGMM_DATA VDM_DEGMM_DATA;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDegammaParaData Select Wrong Layer ID\n");
	}
	
	VDM_DEGMM_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGMM_DATA.u32) + u32Data * VID_OFFSET));
	VDM_DEGMM_DATA.bits.vdm_degamma_para_data = vdm_degamma_para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGMM_DATA.u32) + u32Data * VID_OFFSET),VDM_DEGMM_DATA.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX4Step(HI_U32 u32Data, HI_U32 v_degmm_x4_step)
{
	U_VDM_DEGAMMA_STEP VDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX4Step Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_STEP.bits.v_degmm_x4_step = v_degmm_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX3Step(HI_U32 u32Data, HI_U32 v_degmm_x3_step)
{
	U_VDM_DEGAMMA_STEP VDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX3Step Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_STEP.bits.v_degmm_x3_step = v_degmm_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX2Step(HI_U32 u32Data, HI_U32 v_degmm_x2_step)
{
	U_VDM_DEGAMMA_STEP VDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX2Step Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_STEP.bits.v_degmm_x2_step = v_degmm_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX1Step(HI_U32 u32Data, HI_U32 v_degmm_x1_step)
{
	U_VDM_DEGAMMA_STEP VDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX1Step Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_STEP.bits.v_degmm_x1_step = v_degmm_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX2Pos(HI_U32 u32Data, HI_U32 v_degmm_x2_pos)
{
	U_VDM_DEGAMMA_POS0 VDM_DEGAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX2Pos Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS0.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_POS0.bits.v_degmm_x2_pos = v_degmm_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS0.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX1Pos(HI_U32 u32Data, HI_U32 v_degmm_x1_pos)
{
	U_VDM_DEGAMMA_POS0 VDM_DEGAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX1Pos Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS0.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_POS0.bits.v_degmm_x1_pos = v_degmm_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS0.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX4Pos(HI_U32 u32Data, HI_U32 v_degmm_x4_pos)
{
	U_VDM_DEGAMMA_POS1 VDM_DEGAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX4Pos Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS1.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_POS1.bits.v_degmm_x4_pos = v_degmm_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS1.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX3Pos(HI_U32 u32Data, HI_U32 v_degmm_x3_pos)
{
	U_VDM_DEGAMMA_POS1 VDM_DEGAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX3Pos Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS1.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_POS1.bits.v_degmm_x3_pos = v_degmm_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_POS1.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX4Num(HI_U32 u32Data, HI_U32 v_degmm_x4_num)
{
	U_VDM_DEGAMMA_NUM VDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX4Num Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_NUM.bits.v_degmm_x4_num = v_degmm_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX3Num(HI_U32 u32Data, HI_U32 v_degmm_x3_num)
{
	U_VDM_DEGAMMA_NUM VDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX3Num Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_NUM.bits.v_degmm_x3_num = v_degmm_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX2Num(HI_U32 u32Data, HI_U32 v_degmm_x2_num)
{
	U_VDM_DEGAMMA_NUM VDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX2Num Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_NUM.bits.v_degmm_x2_num = v_degmm_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVDegmmX1Num(HI_U32 u32Data, HI_U32 v_degmm_x1_num)
{
	U_VDM_DEGAMMA_NUM VDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVDegmmX1Num Select Wrong Layer ID\n");
	}
	
	VDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_DEGAMMA_NUM.bits.v_degmm_x1_num = v_degmm_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DEGAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapRdEn(HI_U32 u32Data, HI_U32 vdm_tmap_rd_en)
{
	U_VDM_TONEMAP_REN VDM_TONEMAP_REN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapRdEn Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_REN.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_REN.bits.vdm_tmap_rd_en = vdm_tmap_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_REN.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_REN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmTmapParaData(HI_U32 u32Data, HI_U32 vdm_tmap_para_data)
{
	U_VDM_TONEMAP_DATA VDM_TONEMAP_DATA;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmTmapParaData Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_DATA.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_DATA.bits.vdm_tmap_para_data = vdm_tmap_para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_DATA.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_DATA.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX4Step(HI_U32 u32Data, HI_U32 v_tmap_x4_step)
{
	U_VDM_TONEMAP_STEP VDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX4Step Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_STEP.bits.v_tmap_x4_step = v_tmap_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX3Step(HI_U32 u32Data, HI_U32 v_tmap_x3_step)
{
	U_VDM_TONEMAP_STEP VDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX3Step Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_STEP.bits.v_tmap_x3_step = v_tmap_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX2Step(HI_U32 u32Data, HI_U32 v_tmap_x2_step)
{
	U_VDM_TONEMAP_STEP VDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX2Step Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_STEP.bits.v_tmap_x2_step = v_tmap_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX1Step(HI_U32 u32Data, HI_U32 v_tmap_x1_step)
{
	U_VDM_TONEMAP_STEP VDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX1Step Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_STEP.bits.v_tmap_x1_step = v_tmap_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX1Pos(HI_U32 u32Data, HI_U32 v_tmap_x1_pos)
{
	U_VDM_TONEMAP_POS0 VDM_TONEMAP_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX1Pos Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS0.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_POS0.bits.v_tmap_x1_pos = v_tmap_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS0.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_POS0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX2Pos(HI_U32 u32Data, HI_U32 v_tmap_x2_pos)
{
	U_VDM_TONEMAP_POS1 VDM_TONEMAP_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX2Pos Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS1.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_POS1.bits.v_tmap_x2_pos = v_tmap_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS1.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_POS1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX4Num(HI_U32 u32Data, HI_U32 v_tmap_x4_num)
{
	U_VDM_TONEMAP_NUM VDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX4Num Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_NUM.bits.v_tmap_x4_num = v_tmap_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX3Num(HI_U32 u32Data, HI_U32 v_tmap_x3_num)
{
	U_VDM_TONEMAP_NUM VDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX3Num Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_NUM.bits.v_tmap_x3_num = v_tmap_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX2Num(HI_U32 u32Data, HI_U32 v_tmap_x2_num)
{
	U_VDM_TONEMAP_NUM VDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX2Num Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_NUM.bits.v_tmap_x2_num = v_tmap_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX1Num(HI_U32 u32Data, HI_U32 v_tmap_x1_num)
{
	U_VDM_TONEMAP_NUM VDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX1Num Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_NUM.bits.v_tmap_x1_num = v_tmap_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX3Pos(HI_U32 u32Data, HI_U32 v_tmap_x3_pos)
{
	U_VDM_TONEMAP_POS2 VDM_TONEMAP_POS2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX3Pos Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS2.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_POS2.bits.v_tmap_x3_pos = v_tmap_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS2.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_POS2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVTmapX4Pos(HI_U32 u32Data, HI_U32 v_tmap_x4_pos)
{
	U_VDM_TONEMAP_POS3 VDM_TONEMAP_POS3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVTmapX4Pos Select Wrong Layer ID\n");
	}
	
	VDM_TONEMAP_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS3.u32) + u32Data * VID_OFFSET));
	VDM_TONEMAP_POS3.bits.v_tmap_x4_pos = v_tmap_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS3.u32) + u32Data * VID_OFFSET),VDM_TONEMAP_POS3.u32);
	
	return ;
}

HI_VOID VDP_VID_SetVTmapX8Step(HI_U32 v_tmap_x8_step)
{
	U_VDM_TONEMAP_STEP2 VDM_TONEMAP_STEP2;
	
	VDM_TONEMAP_STEP2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32));
	VDM_TONEMAP_STEP2.bits.v_tmap_x8_step = v_tmap_x8_step;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32),VDM_TONEMAP_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX7Step(HI_U32 v_tmap_x7_step)
{
	U_VDM_TONEMAP_STEP2 VDM_TONEMAP_STEP2;
	
	VDM_TONEMAP_STEP2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32));
	VDM_TONEMAP_STEP2.bits.v_tmap_x7_step = v_tmap_x7_step;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32),VDM_TONEMAP_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX6Step(HI_U32 v_tmap_x6_step)
{
	U_VDM_TONEMAP_STEP2 VDM_TONEMAP_STEP2;
	
	VDM_TONEMAP_STEP2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32));
	VDM_TONEMAP_STEP2.bits.v_tmap_x6_step = v_tmap_x6_step;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32),VDM_TONEMAP_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX5Step(HI_U32 v_tmap_x5_step)
{
	U_VDM_TONEMAP_STEP2 VDM_TONEMAP_STEP2;
	
	VDM_TONEMAP_STEP2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32));
	VDM_TONEMAP_STEP2.bits.v_tmap_x5_step = v_tmap_x5_step;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_STEP2.u32),VDM_TONEMAP_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX5Pos(HI_U32 v_tmap_x5_pos)
{
	U_VDM_TONEMAP_POS4 VDM_TONEMAP_POS4;
	
	VDM_TONEMAP_POS4.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS4.u32));
	VDM_TONEMAP_POS4.bits.v_tmap_x5_pos = v_tmap_x5_pos;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS4.u32),VDM_TONEMAP_POS4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX6Pos(HI_U32 v_tmap_x6_pos)
{
	U_VDM_TONEMAP_POS5 VDM_TONEMAP_POS5;
	
	VDM_TONEMAP_POS5.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS5.u32));
	VDM_TONEMAP_POS5.bits.v_tmap_x6_pos = v_tmap_x6_pos;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS5.u32),VDM_TONEMAP_POS5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX8Num(HI_U32 v_tmap_x8_num)
{
	U_VDM_TONEMAP_NUM2 VDM_TONEMAP_NUM2;
	
	VDM_TONEMAP_NUM2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32));
	VDM_TONEMAP_NUM2.bits.v_tmap_x8_num = v_tmap_x8_num;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32),VDM_TONEMAP_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX7Num(HI_U32 v_tmap_x7_num)
{
	U_VDM_TONEMAP_NUM2 VDM_TONEMAP_NUM2;
	
	VDM_TONEMAP_NUM2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32));
	VDM_TONEMAP_NUM2.bits.v_tmap_x7_num = v_tmap_x7_num;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32),VDM_TONEMAP_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX6Num(HI_U32 v_tmap_x6_num)
{
	U_VDM_TONEMAP_NUM2 VDM_TONEMAP_NUM2;
	
	VDM_TONEMAP_NUM2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32));
	VDM_TONEMAP_NUM2.bits.v_tmap_x6_num = v_tmap_x6_num;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32),VDM_TONEMAP_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX5Num(HI_U32 v_tmap_x5_num)
{
	U_VDM_TONEMAP_NUM2 VDM_TONEMAP_NUM2;
	
	VDM_TONEMAP_NUM2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32));
	VDM_TONEMAP_NUM2.bits.v_tmap_x5_num = v_tmap_x5_num;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_NUM2.u32),VDM_TONEMAP_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX7Pos(HI_U32 v_tmap_x7_pos)
{
	U_VDM_TONEMAP_POS6 VDM_TONEMAP_POS6;
	
	VDM_TONEMAP_POS6.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS6.u32));
	VDM_TONEMAP_POS6.bits.v_tmap_x7_pos = v_tmap_x7_pos;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS6.u32),VDM_TONEMAP_POS6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVTmapX8Pos(HI_U32 v_tmap_x8_pos)
{
	U_VDM_TONEMAP_POS7 VDM_TONEMAP_POS7;
	
	VDM_TONEMAP_POS7.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS7.u32));
	VDM_TONEMAP_POS7.bits.v_tmap_x8_pos = v_tmap_x8_pos;
	VDP_RegWrite((HI_ULONG)&(pVdpReg->VDM_TONEMAP_POS7.u32),VDM_TONEMAP_POS7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmGammaRdEn(HI_U32 u32Data, HI_U32 vdm_gamma_rd_en)
{
	U_VDM_GAMMA_REN VDM_GAMMA_REN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmGammaRdEn Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_REN.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_REN.bits.vdm_gamma_rd_en = vdm_gamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_REN.u32) + u32Data * VID_OFFSET),VDM_GAMMA_REN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmGammaParaData(HI_U32 u32Data, HI_U32 vdm_gamma_para_data)
{
	U_VDM_GAMMMA_DATA VDM_GAMMMA_DATA;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmGammaParaData Select Wrong Layer ID\n");
	}
	
	VDM_GAMMMA_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMMA_DATA.u32) + u32Data * VID_OFFSET));
	VDM_GAMMMA_DATA.bits.vdm_gamma_para_data = vdm_gamma_para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMMA_DATA.u32) + u32Data * VID_OFFSET),VDM_GAMMMA_DATA.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX4Step(HI_U32 u32Data, HI_U32 v_gmm_x4_step)
{
	U_VDM_GAMMA_STEP VDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX4Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP.bits.v_gmm_x4_step = v_gmm_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX3Step(HI_U32 u32Data, HI_U32 v_gmm_x3_step)
{
	U_VDM_GAMMA_STEP VDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX3Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP.bits.v_gmm_x3_step = v_gmm_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX2Step(HI_U32 u32Data, HI_U32 v_gmm_x2_step)
{
	U_VDM_GAMMA_STEP VDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX2Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP.bits.v_gmm_x2_step = v_gmm_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX1Step(HI_U32 u32Data, HI_U32 v_gmm_x1_step)
{
	U_VDM_GAMMA_STEP VDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX1Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP.bits.v_gmm_x1_step = v_gmm_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX2Pos(HI_U32 u32Data, HI_U32 v_gmm_x2_pos)
{
	U_VDM_GAMMA_POS1 VDM_GAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX2Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS1.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS1.bits.v_gmm_x2_pos = v_gmm_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS1.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX1Pos(HI_U32 u32Data, HI_U32 v_gmm_x1_pos)
{
	U_VDM_GAMMA_POS0 VDM_GAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX1Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS0.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS0.bits.v_gmm_x1_pos = v_gmm_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS0.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX4Pos(HI_U32 u32Data, HI_U32 v_gmm_x4_pos)
{
	U_VDM_GAMMA_POS3 VDM_GAMMA_POS3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX4Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS3.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS3.bits.v_gmm_x4_pos = v_gmm_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS3.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX3Pos(HI_U32 u32Data, HI_U32 v_gmm_x3_pos)
{
	U_VDM_GAMMA_POS2 VDM_GAMMA_POS2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX3Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS2.bits.v_gmm_x3_pos = v_gmm_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX4Num(HI_U32 u32Data, HI_U32 v_gmm_x4_num)
{
	U_VDM_GAMMA_NUM VDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX4Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM.bits.v_gmm_x4_num = v_gmm_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX3Num(HI_U32 u32Data, HI_U32 v_gmm_x3_num)
{
	U_VDM_GAMMA_NUM VDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX3Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM.bits.v_gmm_x3_num = v_gmm_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX2Num(HI_U32 u32Data, HI_U32 v_gmm_x2_num)
{
	U_VDM_GAMMA_NUM VDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX2Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM.bits.v_gmm_x2_num = v_gmm_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX1Num(HI_U32 u32Data, HI_U32 v_gmm_x1_num)
{
	U_VDM_GAMMA_NUM VDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX1Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM.bits.v_gmm_x1_num = v_gmm_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX6Pos(HI_U32 u32Data, HI_U32 v_gmm_x6_pos)
{
	U_VDM_GAMMA_POS5 VDM_GAMMA_POS5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX6Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS5.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS5.bits.v_gmm_x6_pos = v_gmm_x6_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS5.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX5Pos(HI_U32 u32Data, HI_U32 v_gmm_x5_pos)
{
	U_VDM_GAMMA_POS4 VDM_GAMMA_POS4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX5Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS4.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS4.bits.v_gmm_x5_pos = v_gmm_x5_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS4.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX8Pos(HI_U32 u32Data, HI_U32 v_gmm_x8_pos)
{
	U_VDM_GAMMA_POS7 VDM_GAMMA_POS7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX8Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS7.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS7.bits.v_gmm_x8_pos = v_gmm_x8_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS7.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX7Pos(HI_U32 u32Data, HI_U32 v_gmm_x7_pos)
{
	U_VDM_GAMMA_POS6 VDM_GAMMA_POS6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX7Pos Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_POS6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS6.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_POS6.bits.v_gmm_x7_pos = v_gmm_x7_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_POS6.u32) + u32Data * VID_OFFSET),VDM_GAMMA_POS6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX8Step(HI_U32 u32Data, HI_U32 v_gmm_x8_step)
{
	U_VDM_GAMMA_STEP2 VDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX8Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP2.bits.v_gmm_x8_step = v_gmm_x8_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX7Step(HI_U32 u32Data, HI_U32 v_gmm_x7_step)
{
	U_VDM_GAMMA_STEP2 VDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX7Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP2.bits.v_gmm_x7_step = v_gmm_x7_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX6Step(HI_U32 u32Data, HI_U32 v_gmm_x6_step)
{
	U_VDM_GAMMA_STEP2 VDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX6Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP2.bits.v_gmm_x6_step = v_gmm_x6_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX5Step(HI_U32 u32Data, HI_U32 v_gmm_x5_step)
{
	U_VDM_GAMMA_STEP2 VDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX5Step Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_STEP2.bits.v_gmm_x5_step = v_gmm_x5_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_STEP2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX8Num(HI_U32 u32Data, HI_U32 v_gmm_x8_num)
{
	U_VDM_GAMMA_NUM2 VDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX8Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM2.bits.v_gmm_x8_num = v_gmm_x8_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX7Num(HI_U32 u32Data, HI_U32 v_gmm_x7_num)
{
	U_VDM_GAMMA_NUM2 VDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX7Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM2.bits.v_gmm_x7_num = v_gmm_x7_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX6Num(HI_U32 u32Data, HI_U32 v_gmm_x6_num)
{
	U_VDM_GAMMA_NUM2 VDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX6Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM2.bits.v_gmm_x6_num = v_gmm_x6_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVGmmX5Num(HI_U32 u32Data, HI_U32 v_gmm_x5_num)
{
	U_VDM_GAMMA_NUM2 VDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVGmmX5Num Select Wrong Layer ID\n");
	}
	
	VDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET));
	VDM_GAMMA_NUM2.bits.v_gmm_x5_num = v_gmm_x5_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_GAMMA_NUM2.u32) + u32Data * VID_OFFSET),VDM_GAMMA_NUM2.u32);
	
	return ;
}



HI_VOID VDP_VID_SetVdmVdmCoef00Csc(HI_U32 u32Data, HI_U32 vdm_coef00_csc)
{
	U_VDM_CSC_COEF00 VDM_CSC_COEF00;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef00Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF00.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF00.bits.vdm_coef00_csc = vdm_coef00_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF00.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF00.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef01Csc(HI_U32 u32Data, HI_U32 vdm_coef01_csc)
{
	U_VDM_CSC_COEF01 VDM_CSC_COEF01;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef01Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF01.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF01.bits.vdm_coef01_csc = vdm_coef01_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF01.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF01.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef02Csc(HI_U32 u32Data, HI_U32 vdm_coef02_csc)
{
	U_VDM_CSC_COEF02 VDM_CSC_COEF02;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef02Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF02.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF02.bits.vdm_coef02_csc = vdm_coef02_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF02.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF02.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef10Csc(HI_U32 u32Data, HI_U32 vdm_coef10_csc)
{
	U_VDM_CSC_COEF10 VDM_CSC_COEF10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef10Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF10.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF10.bits.vdm_coef10_csc = vdm_coef10_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF10.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef11Csc(HI_U32 u32Data, HI_U32 vdm_coef11_csc)
{
	U_VDM_CSC_COEF11 VDM_CSC_COEF11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef11Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF11.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF11.bits.vdm_coef11_csc = vdm_coef11_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF11.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef12Csc(HI_U32 u32Data, HI_U32 vdm_coef12_csc)
{
	U_VDM_CSC_COEF12 VDM_CSC_COEF12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef12Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF12.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF12.bits.vdm_coef12_csc = vdm_coef12_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF12.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef20Csc(HI_U32 u32Data, HI_U32 vdm_coef20_csc)
{
	U_VDM_CSC_COEF20 VDM_CSC_COEF20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef20Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF20.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF20.bits.vdm_coef20_csc = vdm_coef20_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF20.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF20.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef21Csc(HI_U32 u32Data, HI_U32 vdm_coef21_csc)
{
	U_VDM_CSC_COEF21 VDM_CSC_COEF21;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef21Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF21.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF21.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF21.bits.vdm_coef21_csc = vdm_coef21_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF21.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF21.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCoef22Csc(HI_U32 u32Data, HI_U32 vdm_coef22_csc)
{
	U_VDM_CSC_COEF22 VDM_CSC_COEF22;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCoef22Csc Select Wrong Layer ID\n");
	}
	
	VDM_CSC_COEF22.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF22.u32) + u32Data * VID_OFFSET));
	VDM_CSC_COEF22.bits.vdm_coef22_csc = vdm_coef22_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_COEF22.u32) + u32Data * VID_OFFSET),VDM_CSC_COEF22.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCscScale(HI_U32 u32Data, HI_U32 vdm_csc_scale)
{
	U_VDM_CSC_SCALE VDM_CSC_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCscScale Select Wrong Layer ID\n");
	}
	
	VDM_CSC_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_SCALE.u32) + u32Data * VID_OFFSET));
	VDM_CSC_SCALE.bits.vdm_csc_scale = vdm_csc_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_SCALE.u32) + u32Data * VID_OFFSET),VDM_CSC_SCALE.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCscClipMin(HI_U32 u32Data, HI_U32 vdm_csc_clip_min)
{
	U_VDM_CSC_CLIP_MIN VDM_CSC_CLIP_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCscClipMin Select Wrong Layer ID\n");
	}
	
	VDM_CSC_CLIP_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_CLIP_MIN.u32) + u32Data * VID_OFFSET));
	VDM_CSC_CLIP_MIN.bits.vdm_csc_clip_min = vdm_csc_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_CLIP_MIN.u32) + u32Data * VID_OFFSET),VDM_CSC_CLIP_MIN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmCscClipMax(HI_U32 u32Data, HI_U32 vdm_csc_clip_max)
{
	U_VDM_CSC_CLIP_MAX VDM_CSC_CLIP_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCscClipMax Select Wrong Layer ID\n");
	}
	
	VDM_CSC_CLIP_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_CLIP_MAX.u32) + u32Data * VID_OFFSET));
	VDM_CSC_CLIP_MAX.bits.vdm_csc_clip_max = vdm_csc_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_CLIP_MAX.u32) + u32Data * VID_OFFSET),VDM_CSC_CLIP_MAX.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmV1GammaEn(HI_U32 u32Data, HI_U32 vdm_v1_gamma_en)
{
	U_VDM_YUV2RGB_CTRL VDM_YUV2RGB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmV1GammaEn Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_CTRL.bits.vdm_v1_gamma_en = vdm_v1_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmV0GammaEn(HI_U32 u32Data, HI_U32 vdm_v0_gamma_en)
{
	U_VDM_YUV2RGB_CTRL VDM_YUV2RGB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmV0GammaEn Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_CTRL.bits.vdm_v0_gamma_en = vdm_v0_gamma_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_CTRL.u32);
	
	return ;
}





HI_VOID VDP_VID_SetVdmVdm00Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_00_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF00 VDM_YUV2RGB_COEF00;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm00Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF00.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF00.bits.vdm_00_yuv2rgb = vdm_00_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF00.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF00.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm01Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_01_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF01 VDM_YUV2RGB_COEF01;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm01Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF01.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF01.bits.vdm_01_yuv2rgb = vdm_01_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF01.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF01.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm02Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_02_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF02 VDM_YUV2RGB_COEF02;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm02Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF02.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF02.bits.vdm_02_yuv2rgb = vdm_02_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF02.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF02.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm03Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_03_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF03 VDM_YUV2RGB_COEF03;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm03Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF03.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF03.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF03.bits.vdm_03_yuv2rgb = vdm_03_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF03.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF03.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm04Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_04_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF04 VDM_YUV2RGB_COEF04;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm04Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF04.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF04.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF04.bits.vdm_04_yuv2rgb = vdm_04_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF04.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF04.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm05Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_05_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF05 VDM_YUV2RGB_COEF05;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm05Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF05.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF05.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF05.bits.vdm_05_yuv2rgb = vdm_05_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF05.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF05.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm06Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_06_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF06 VDM_YUV2RGB_COEF06;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm06Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF06.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF06.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF06.bits.vdm_06_yuv2rgb = vdm_06_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF06.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF06.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm07Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_07_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF07 VDM_YUV2RGB_COEF07;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm07Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF07.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF07.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF07.bits.vdm_07_yuv2rgb = vdm_07_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF07.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF07.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm08Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_08_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF08 VDM_YUV2RGB_COEF08;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm08Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF08.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF08.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF08.bits.vdm_08_yuv2rgb = vdm_08_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF08.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF08.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm10Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_10_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF10 VDM_YUV2RGB_COEF10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm10Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF10.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF10.bits.vdm_10_yuv2rgb = vdm_10_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF10.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm11Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_11_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF11 VDM_YUV2RGB_COEF11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm11Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF11.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF11.bits.vdm_11_yuv2rgb = vdm_11_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF11.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm12Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_12_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF12 VDM_YUV2RGB_COEF12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm12Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF12.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF12.bits.vdm_12_yuv2rgb = vdm_12_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF12.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm13Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_13_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF13 VDM_YUV2RGB_COEF13;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm13Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF13.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF13.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF13.bits.vdm_13_yuv2rgb = vdm_13_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF13.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF13.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm14Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_14_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF14 VDM_YUV2RGB_COEF14;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm14Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF14.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF14.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF14.bits.vdm_14_yuv2rgb = vdm_14_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF14.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF14.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm15Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_15_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF15 VDM_YUV2RGB_COEF15;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm15Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF15.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF15.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF15.bits.vdm_15_yuv2rgb = vdm_15_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF15.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF15.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm16Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_16_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF16 VDM_YUV2RGB_COEF16;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm16Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF16.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF16.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF16.bits.vdm_16_yuv2rgb = vdm_16_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF16.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF16.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm17Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_17_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF17 VDM_YUV2RGB_COEF17;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm17Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF17.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF17.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF17.bits.vdm_17_yuv2rgb = vdm_17_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF17.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF17.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm18Yuv2rgb(HI_U32 u32Data, HI_U32 vdm_18_yuv2rgb)
{
	U_VDM_YUV2RGB_COEF18 VDM_YUV2RGB_COEF18;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm18Yuv2rgb Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_COEF18.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF18.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_COEF18.bits.vdm_18_yuv2rgb = vdm_18_yuv2rgb;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_COEF18.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_COEF18.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgbscale2p(HI_U32 u32Data, HI_U32 vdm_yuv2rgbscale2p)
{
	U_VDM_YUV2RGB_SCALE2P VDM_YUV2RGB_SCALE2P;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgbscale2p Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_SCALE2P.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_SCALE2P.bits.vdm_yuv2rgbscale2p = vdm_yuv2rgbscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_SCALE2P.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb00InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_00_in_dc)
{
	U_VDM_YUV2RGB_IN_DC00 VDM_YUV2RGB_IN_DC00;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb00InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC00.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC00.bits.vdm_yuv2rgb_00_in_dc = vdm_yuv2rgb_00_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC00.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC00.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb01InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_01_in_dc)
{
	U_VDM_YUV2RGB_IN_DC01 VDM_YUV2RGB_IN_DC01;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb01InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC01.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC01.bits.vdm_yuv2rgb_01_in_dc = vdm_yuv2rgb_01_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC01.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC01.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb02InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_02_in_dc)
{
	U_VDM_YUV2RGB_IN_DC02 VDM_YUV2RGB_IN_DC02;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb02InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC02.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC02.bits.vdm_yuv2rgb_02_in_dc = vdm_yuv2rgb_02_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC02.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC02.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb00OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_00_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC00 VDM_YUV2RGB_OUT_DC00;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb00OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC00.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC00.bits.vdm_yuv2rgb_00_out_dc = vdm_yuv2rgb_00_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC00.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC00.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb01OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_01_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC01 VDM_YUV2RGB_OUT_DC01;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb01OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC01.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC01.bits.vdm_yuv2rgb_01_out_dc = vdm_yuv2rgb_01_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC01.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC01.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb02OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_02_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC02 VDM_YUV2RGB_OUT_DC02;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb02OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC02.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC02.bits.vdm_yuv2rgb_02_out_dc = vdm_yuv2rgb_02_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC02.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC02.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb10InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_10_in_dc)
{
	U_VDM_YUV2RGB_IN_DC10 VDM_YUV2RGB_IN_DC10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb10InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC10.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC10.bits.vdm_yuv2rgb_10_in_dc = vdm_yuv2rgb_10_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC10.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb11InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_11_in_dc)
{
	U_VDM_YUV2RGB_IN_DC11 VDM_YUV2RGB_IN_DC11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb11InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC11.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC11.bits.vdm_yuv2rgb_11_in_dc = vdm_yuv2rgb_11_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC11.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb12InDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_12_in_dc)
{
	U_VDM_YUV2RGB_IN_DC12 VDM_YUV2RGB_IN_DC12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb12InDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_IN_DC12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC12.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_IN_DC12.bits.vdm_yuv2rgb_12_in_dc = vdm_yuv2rgb_12_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_IN_DC12.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_IN_DC12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb10OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_10_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC10 VDM_YUV2RGB_OUT_DC10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb10OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC10.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC10.bits.vdm_yuv2rgb_10_out_dc = vdm_yuv2rgb_10_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC10.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC10.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb11OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_11_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC11 VDM_YUV2RGB_OUT_DC11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb11OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC11.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC11.bits.vdm_yuv2rgb_11_out_dc = vdm_yuv2rgb_11_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC11.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC11.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgb12OutDc(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_12_out_dc)
{
	U_VDM_YUV2RGB_OUT_DC12 VDM_YUV2RGB_OUT_DC12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgb12OutDc Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_OUT_DC12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC12.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_OUT_DC12.bits.vdm_yuv2rgb_12_out_dc = vdm_yuv2rgb_12_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_OUT_DC12.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_OUT_DC12.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgbClipMin(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_clip_min)
{
	U_VDM_YUV2RGB_MIN VDM_YUV2RGB_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgbClipMin Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_MIN.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_MIN.bits.vdm_yuv2rgb_clip_min = vdm_yuv2rgb_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_MIN.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_MIN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgbClipMax(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_clip_max)
{
	U_VDM_YUV2RGB_MAX VDM_YUV2RGB_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgbClipMax Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_MAX.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_MAX.bits.vdm_yuv2rgb_clip_max = vdm_yuv2rgb_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_MAX.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_MAX.u32);
	
	return ;
}




HI_VOID VDP_VID_SetVdmVdm00Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_00_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF0 VDM_RGB2YUV_COEF0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm00Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF0.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF0.bits.vdm_00_rgb2yuv = vdm_00_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF0.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm01Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_01_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF1 VDM_RGB2YUV_COEF1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm01Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF1.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF1.bits.vdm_01_rgb2yuv = vdm_01_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF1.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm02Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_02_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF2 VDM_RGB2YUV_COEF2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm02Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF2.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF2.bits.vdm_02_rgb2yuv = vdm_02_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF2.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm10Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_10_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF3 VDM_RGB2YUV_COEF3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm10Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF3.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF3.bits.vdm_10_rgb2yuv = vdm_10_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF3.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm11Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_11_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF4 VDM_RGB2YUV_COEF4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm11Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF4.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF4.bits.vdm_11_rgb2yuv = vdm_11_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF4.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm12Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_12_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF5 VDM_RGB2YUV_COEF5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm12Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF5.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF5.bits.vdm_12_rgb2yuv = vdm_12_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF5.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm20Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_20_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF6 VDM_RGB2YUV_COEF6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm20Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF6.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF6.bits.vdm_20_rgb2yuv = vdm_20_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF6.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm21Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_21_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF7 VDM_RGB2YUV_COEF7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm21Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF7.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF7.bits.vdm_21_rgb2yuv = vdm_21_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF7.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdm22Rgb2yuv(HI_U32 u32Data, HI_U32 vdm_22_rgb2yuv)
{
	U_VDM_RGB2YUV_COEF8 VDM_RGB2YUV_COEF8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdm22Rgb2yuv Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF8.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_COEF8.bits.vdm_22_rgb2yuv = vdm_22_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_COEF8.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_COEF8.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuvscale2p(HI_U32 u32Data, HI_U32 vdm_rgb2yuvscale2p)
{
	U_VDM_RGB2YUV_SCALE2P VDM_RGB2YUV_SCALE2P;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuvscale2p Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_SCALE2P.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_SCALE2P.bits.vdm_rgb2yuvscale2p = vdm_rgb2yuvscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_SCALE2P.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv0InDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_0_in_dc)
{
	U_VDM_RGB2YUV_IN_DC0 VDM_RGB2YUV_IN_DC0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv0InDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_IN_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC0.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_IN_DC0.bits.vdm_rgb2yuv_0_in_dc = vdm_rgb2yuv_0_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC0.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_IN_DC0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv1InDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_1_in_dc)
{
	U_VDM_RGB2YUV_IN_DC1 VDM_RGB2YUV_IN_DC1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv1InDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_IN_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC1.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_IN_DC1.bits.vdm_rgb2yuv_1_in_dc = vdm_rgb2yuv_1_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC1.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_IN_DC1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv2InDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_2_in_dc)
{
	U_VDM_RGB2YUV_IN_DC2 VDM_RGB2YUV_IN_DC2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv2InDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_IN_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC2.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_IN_DC2.bits.vdm_rgb2yuv_2_in_dc = vdm_rgb2yuv_2_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_IN_DC2.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_IN_DC2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv0OutDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_0_out_dc)
{
	U_VDM_RGB2YUV_OUT_DC0 VDM_RGB2YUV_OUT_DC0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv0OutDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC0.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_OUT_DC0.bits.vdm_rgb2yuv_0_out_dc = vdm_rgb2yuv_0_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC0.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_OUT_DC0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv1OutDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_1_out_dc)
{
	U_VDM_RGB2YUV_OUT_DC1 VDM_RGB2YUV_OUT_DC1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv1OutDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC1.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_OUT_DC1.bits.vdm_rgb2yuv_1_out_dc = vdm_rgb2yuv_1_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC1.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_OUT_DC1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuv2OutDc(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_2_out_dc)
{
	U_VDM_RGB2YUV_OUT_DC2 VDM_RGB2YUV_OUT_DC2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuv2OutDc Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC2.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_OUT_DC2.bits.vdm_rgb2yuv_2_out_dc = vdm_rgb2yuv_2_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_OUT_DC2.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_OUT_DC2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuvClipMin(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_clip_min)
{
	U_VDM_RGB2YUV_MIN VDM_RGB2YUV_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuvClipMin Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_MIN.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_MIN.bits.vdm_rgb2yuv_clip_min = vdm_rgb2yuv_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_MIN.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_MIN.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuvClipMax(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_clip_max)
{
	U_VDM_RGB2YUV_MAX VDM_RGB2YUV_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuvClipMax Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_MAX.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_MAX.bits.vdm_rgb2yuv_clip_max = vdm_rgb2yuv_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_MAX.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_MAX.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherRoundUnlim(HI_U32 u32Data, HI_U32 vdm_dither_round_unlim)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherRoundUnlim Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_round_unlim = vdm_dither_round_unlim;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherEn(HI_U32 u32Data, HI_U32 vdm_dither_en)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherEn Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_en = vdm_dither_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherMode(HI_U32 u32Data, HI_U32 vdm_dither_mode)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherMode Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_mode = vdm_dither_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherRound(HI_U32 u32Data, HI_U32 vdm_dither_round)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherRound Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_round = vdm_dither_round;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherDomainMode(HI_U32 u32Data, HI_U32 vdm_dither_domain_mode)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherDomainMode Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_domain_mode = vdm_dither_domain_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherTapMode(HI_U32 u32Data, HI_U32 vdm_dither_tap_mode)
{
	U_VDM_DITHER_CTRL VDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherTapMode Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_CTRL.bits.vdm_dither_tap_mode = vdm_dither_tap_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_CTRL.u32) + u32Data * VID_OFFSET),VDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedY0(HI_U32 u32Data, HI_U32 vdm_dither_sed_y0)
{
	U_VDM_DITHER_SED_Y0 VDM_DITHER_SED_Y0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedY0 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_Y0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y0.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_Y0.bits.vdm_dither_sed_y0 = vdm_dither_sed_y0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y0.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_Y0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedU0(HI_U32 u32Data, HI_U32 vdm_dither_sed_u0)
{
	U_VDM_DITHER_SED_U0 VDM_DITHER_SED_U0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedU0 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_U0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U0.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_U0.bits.vdm_dither_sed_u0 = vdm_dither_sed_u0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U0.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_U0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedV0(HI_U32 u32Data, HI_U32 vdm_dither_sed_v0)
{
	U_VDM_DITHER_SED_V0 VDM_DITHER_SED_V0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedV0 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_V0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V0.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_V0.bits.vdm_dither_sed_v0 = vdm_dither_sed_v0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V0.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_V0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedW0(HI_U32 u32Data, HI_U32 vdm_dither_sed_w0)
{
	U_VDM_DITHER_SED_W0 VDM_DITHER_SED_W0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedW0 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_W0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W0.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_W0.bits.vdm_dither_sed_w0 = vdm_dither_sed_w0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W0.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_W0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedY1(HI_U32 u32Data, HI_U32 vdm_dither_sed_y1)
{
	U_VDM_DITHER_SED_Y1 VDM_DITHER_SED_Y1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedY1 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_Y1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y1.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_Y1.bits.vdm_dither_sed_y1 = vdm_dither_sed_y1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y1.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_Y1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedU1(HI_U32 u32Data, HI_U32 vdm_dither_sed_u1)
{
	U_VDM_DITHER_SED_U1 VDM_DITHER_SED_U1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedU1 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_U1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U1.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_U1.bits.vdm_dither_sed_u1 = vdm_dither_sed_u1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U1.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_U1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedV1(HI_U32 u32Data, HI_U32 vdm_dither_sed_v1)
{
	U_VDM_DITHER_SED_V1 VDM_DITHER_SED_V1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedV1 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_V1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V1.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_V1.bits.vdm_dither_sed_v1 = vdm_dither_sed_v1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V1.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_V1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedW1(HI_U32 u32Data, HI_U32 vdm_dither_sed_w1)
{
	U_VDM_DITHER_SED_W1 VDM_DITHER_SED_W1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedW1 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_W1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W1.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_W1.bits.vdm_dither_sed_w1 = vdm_dither_sed_w1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W1.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_W1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedY2(HI_U32 u32Data, HI_U32 vdm_dither_sed_y2)
{
	U_VDM_DITHER_SED_Y2 VDM_DITHER_SED_Y2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedY2 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_Y2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y2.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_Y2.bits.vdm_dither_sed_y2 = vdm_dither_sed_y2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y2.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_Y2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedU2(HI_U32 u32Data, HI_U32 vdm_dither_sed_u2)
{
	U_VDM_DITHER_SED_U2 VDM_DITHER_SED_U2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedU2 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_U2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U2.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_U2.bits.vdm_dither_sed_u2 = vdm_dither_sed_u2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U2.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_U2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedV2(HI_U32 u32Data, HI_U32 vdm_dither_sed_v2)
{
	U_VDM_DITHER_SED_V2 VDM_DITHER_SED_V2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedV2 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_V2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V2.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_V2.bits.vdm_dither_sed_v2 = vdm_dither_sed_v2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V2.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_V2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedW2(HI_U32 u32Data, HI_U32 vdm_dither_sed_w2)
{
	U_VDM_DITHER_SED_W2 VDM_DITHER_SED_W2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedW2 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_W2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W2.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_W2.bits.vdm_dither_sed_w2 = vdm_dither_sed_w2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W2.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_W2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedY3(HI_U32 u32Data, HI_U32 vdm_dither_sed_y3)
{
	U_VDM_DITHER_SED_Y3 VDM_DITHER_SED_Y3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedY3 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_Y3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y3.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_Y3.bits.vdm_dither_sed_y3 = vdm_dither_sed_y3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_Y3.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_Y3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedU3(HI_U32 u32Data, HI_U32 vdm_dither_sed_u3)
{
	U_VDM_DITHER_SED_U3 VDM_DITHER_SED_U3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedU3 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_U3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U3.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_U3.bits.vdm_dither_sed_u3 = vdm_dither_sed_u3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_U3.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_U3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedV3(HI_U32 u32Data, HI_U32 vdm_dither_sed_v3)
{
	U_VDM_DITHER_SED_V3 VDM_DITHER_SED_V3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedV3 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_V3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V3.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_V3.bits.vdm_dither_sed_v3 = vdm_dither_sed_v3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_V3.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_V3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherSedW3(HI_U32 u32Data, HI_U32 vdm_dither_sed_w3)
{
	U_VDM_DITHER_SED_W3 VDM_DITHER_SED_W3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherSedW3 Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_SED_W3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W3.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_SED_W3.bits.vdm_dither_sed_w3 = vdm_dither_sed_w3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_SED_W3.u32) + u32Data * VID_OFFSET),VDM_DITHER_SED_W3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherThrMax(HI_U32 u32Data, HI_U32 vdm_dither_thr_max)
{
	U_VDM_DITHER_THR VDM_DITHER_THR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherThrMax Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_THR.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_THR.bits.vdm_dither_thr_max = vdm_dither_thr_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_THR.u32) + u32Data * VID_OFFSET),VDM_DITHER_THR.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmDitherThrMin(HI_U32 u32Data, HI_U32 vdm_dither_thr_min)
{
	U_VDM_DITHER_THR VDM_DITHER_THR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmDitherThrMin Select Wrong Layer ID\n");
	}
	
	VDM_DITHER_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_DITHER_THR.u32) + u32Data * VID_OFFSET));
	VDM_DITHER_THR.bits.vdm_dither_thr_min = vdm_dither_thr_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_DITHER_THR.u32) + u32Data * VID_OFFSET),VDM_DITHER_THR.u32);
	
	return ;
}



HI_VOID VDP_VID_SetVdmVdmCscEn(HI_U32 u32Data, HI_U32 vdm_csc_en)
{
	U_VDM_CSC_CTRL VDM_CSC_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmCscEn Select Wrong Layer ID\n");
	}
	
	VDM_CSC_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CSC_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_CSC_CTRL.bits.vdm_csc_en = vdm_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_CSC_CTRL.u32) + u32Data * VID_OFFSET),VDM_CSC_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmV1CscEn(HI_U32 u32Data, HI_U32 vdm_v1_csc_en)
{
	U_VDM_YUV2RGB_CTRL VDM_YUV2RGB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmV1CscEn Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_CTRL.bits.vdm_v1_csc_en = vdm_v1_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmV0CscEn(HI_U32 u32Data, HI_U32 vdm_v0_csc_en)
{
	U_VDM_YUV2RGB_CTRL VDM_YUV2RGB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmV0CscEn Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_CTRL.bits.vdm_v0_csc_en = vdm_v0_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmYuv2rgbEn(HI_U32 u32Data, HI_U32 vdm_yuv2rgb_en)
{
	U_VDM_YUV2RGB_CTRL VDM_YUV2RGB_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmYuv2rgbEn Select Wrong Layer ID\n");
	}
	
	VDM_YUV2RGB_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_YUV2RGB_CTRL.bits.vdm_yuv2rgb_en = vdm_yuv2rgb_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_YUV2RGB_CTRL.u32) + u32Data * VID_OFFSET),VDM_YUV2RGB_CTRL.u32);
	
	return ;
}


HI_VOID VDP_VID_SetVdmVdmRgb2yuvEn(HI_U32 u32Data, HI_U32 vdm_rgb2yuv_en)
{
	U_VDM_RGB2YUV_CTRL VDM_RGB2YUV_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetVdmVdmRgb2yuvEn Select Wrong Layer ID\n");
	}
	
	VDM_RGB2YUV_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_CTRL.u32) + u32Data * VID_OFFSET));
	VDM_RGB2YUV_CTRL.bits.vdm_rgb2yuv_en = vdm_rgb2yuv_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VDM_RGB2YUV_CTRL.u32) + u32Data * VID_OFFSET),VDM_RGB2YUV_CTRL.u32);
	
	return ;
}

HI_VOID VDP_VP0_SetCsc0Csc1Mode(HI_U32 u32Data, VDP_CSC_MODE_E enCscMode) 
{ 
	VDP_CSC_COEF_S    st_csc_coef;
	VDP_CSC_DC_COEF_S st_csc_dc_coef;
	HI_U32 u32Pre   = 1 << 13;
	HI_U32 u32DcPre = 4;//1:8bit; 4:10bit

	if(enCscMode == VDP_CSC_RGB2YUV_601)//limit range 
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)(0.257  * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)(0.504  * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)(0.098  * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(-0.148 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)(-0.291 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)( 0.439  * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 0.439  * u32Pre); 
		st_csc_coef.csc_coef21     = (HI_S32)(-0.368 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)(-0.071 * u32Pre);

		st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =  16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 = 128 * u32DcPre; 
	}
	else if(enCscMode == VDP_CSC_YUV2RGB_601)//limit range 
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)(-0.002 * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 1.596 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)( 1.164 * u32Pre); 
		st_csc_coef.csc_coef11     = (HI_S32)(-0.391 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)(-0.813 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)(-2.018  * u32Pre); 
		st_csc_coef.csc_coef22     = (HI_S32)(-0.001  * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =  0 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  0 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  0 * u32DcPre; 
	}
	else if(enCscMode == VDP_CSC_RGB2YUV_709)//limit range 
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)( 0.183 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0.614 * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 0.062 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(-0.101 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)(-0.338 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)( 0.439 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 0.439 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)(-0.399 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)(-0.040 * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = 0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = 0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = 0 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 = 16  * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 = 128 * u32DcPre;
	}
	else if(enCscMode == VDP_CSC_YUV2RGB_709)//limit range inverse to rgb2yuv 
	{
		st_csc_coef.csc_coef00     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0     * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 1.793 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)(-0.213 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)(-0.534 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)( 2.115 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 0     * u32Pre);
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
		st_csc_coef.csc_coef01     = (HI_S32)( 0.100 * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 0.191 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)( 0.990 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)(-0.110 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)(-0.072 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 0.984 * u32Pre);
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
		st_csc_coef.csc_coef11     = (HI_S32)( 1.018 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)( 0.114 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)( 0.074 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 1.025 * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre; 
	}
	else if(enCscMode == VDP_CSC_YUV2YUV_MIN)
	{ 
		st_csc_coef.csc_coef00     = 0;
		st_csc_coef.csc_coef01     = 0;
		st_csc_coef.csc_coef02     = 0;
		st_csc_coef.csc_coef10     = 0;
		st_csc_coef.csc_coef11     = 0;
		st_csc_coef.csc_coef12     = 0;
		st_csc_coef.csc_coef20     = 0;
		st_csc_coef.csc_coef21     = 0;
		st_csc_coef.csc_coef22     = 0;
		st_csc_dc_coef.csc_in_dc0  = 0;
		st_csc_dc_coef.csc_in_dc1  = 0;
		st_csc_dc_coef.csc_in_dc2  = 0;
		st_csc_dc_coef.csc_out_dc0 = 0;
		st_csc_dc_coef.csc_out_dc1 = 0;
		st_csc_dc_coef.csc_out_dc2 =  0;
	}
#if 0
	else if(enCscMode == VDP_CSC_YUV2RGB_BT2020_BT2020)//limit range
	{
		st_csc_coef.csc_coef00     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0     * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 1.679 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)(-0.187 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)(-0.650 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 1.164 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)( 2.142 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 0     * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =  0   * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  0   * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  0   * u32DcPre; 
	}
	else if(enCscMode == VDP_CSC_RGB2YUV_BT2020_BT2020)
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)( 0.2256 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0.5823 * u32Pre); st_csc_coef.csc_coef02     = (HI_S32)( 0.0509 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(-0.1227 * u32Pre); st_csc_coef.csc_coef11     = (HI_S32)(-0.3166 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)( 0.4392 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)( 0.4392 * u32Pre); st_csc_coef.csc_coef21     = (HI_S32)(-0.4039 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)(-0.0353 * u32Pre);
		st_csc_dc_coef.csc_in_dc0  =  0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  =  0 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  =  0 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre;
	}
	else if(enCscMode == VDP_CSC_YUV2YUV_BT2020_709)//wbc_dhd中的CSC
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0.018 * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)(-0.092 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)( 1.004 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)( 0.050 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)(-0.011 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 0.994 * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre;
	}
	else if(enCscMode == VDP_CSC_YUV2YUV_BT2020_601)//wbc_VP中的CSC
	{ 
		st_csc_coef.csc_coef00     = (HI_S32)(     1 * u32Pre);
		st_csc_coef.csc_coef01     = (HI_S32)( 0.115 * u32Pre);
		st_csc_coef.csc_coef02     = (HI_S32)( 0.104 * u32Pre);
		st_csc_coef.csc_coef10     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef11     = (HI_S32)( 0.995 * u32Pre);
		st_csc_coef.csc_coef12     = (HI_S32)(-0.059 * u32Pre);
		st_csc_coef.csc_coef20     = (HI_S32)(     0 * u32Pre);
		st_csc_coef.csc_coef21     = (HI_S32)(-0.083 * u32Pre);
		st_csc_coef.csc_coef22     = (HI_S32)( 0.976 * u32Pre);
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =   16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 =  128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 =  128 * u32DcPre; 
	}
#endif
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
		st_csc_dc_coef.csc_in_dc0  = -16  * u32DcPre;
		st_csc_dc_coef.csc_in_dc1  = -128 * u32DcPre;
		st_csc_dc_coef.csc_in_dc2  = -128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc0 =  16 * u32DcPre;
		st_csc_dc_coef.csc_out_dc1 = 128 * u32DcPre;
		st_csc_dc_coef.csc_out_dc2 = 128 * u32DcPre; 
	}

	VDP_VID_SetVdmVdmYuv2rgbscale2p(0, 13); 
	VDP_VID_SetVdmVdmYuv2rgbClipMin(0, 0);                                                 
	VDP_VID_SetVdmVdmYuv2rgbClipMax(0, 1023);  

	if(u32Data==0)
	{
		VDP_VID_SetVdmVdm00Yuv2rgb(u32Data, st_csc_coef.csc_coef00); 
		VDP_VID_SetVdmVdm01Yuv2rgb(u32Data, st_csc_coef.csc_coef01); 
		VDP_VID_SetVdmVdm02Yuv2rgb(u32Data, st_csc_coef.csc_coef02); 
		VDP_VID_SetVdmVdm03Yuv2rgb(u32Data, st_csc_coef.csc_coef10);
		VDP_VID_SetVdmVdm04Yuv2rgb(u32Data, st_csc_coef.csc_coef11); 
		VDP_VID_SetVdmVdm05Yuv2rgb(u32Data, st_csc_coef.csc_coef12);
		VDP_VID_SetVdmVdm06Yuv2rgb(u32Data, st_csc_coef.csc_coef20); 
		VDP_VID_SetVdmVdm07Yuv2rgb(u32Data, st_csc_coef.csc_coef21); 
		VDP_VID_SetVdmVdm08Yuv2rgb(u32Data, st_csc_coef.csc_coef22);           

		VDP_VID_SetVdmVdmYuv2rgb00InDc(u32Data, st_csc_dc_coef.csc_in_dc0); 
		VDP_VID_SetVdmVdmYuv2rgb01InDc(u32Data, st_csc_dc_coef.csc_in_dc1);
		VDP_VID_SetVdmVdmYuv2rgb02InDc(u32Data, st_csc_dc_coef.csc_in_dc2);
		VDP_VID_SetVdmVdmYuv2rgb00OutDc(u32Data, st_csc_dc_coef.csc_out_dc0); 
		VDP_VID_SetVdmVdmYuv2rgb01OutDc(u32Data, st_csc_dc_coef.csc_out_dc1); 
		VDP_VID_SetVdmVdmYuv2rgb02OutDc(u32Data, st_csc_dc_coef.csc_out_dc2); 
	}
	else if(u32Data==1)
	{
		u32Data=0;
		VDP_VID_SetVdmVdm10Yuv2rgb(u32Data, st_csc_coef.csc_coef00);
		VDP_VID_SetVdmVdm11Yuv2rgb(u32Data, st_csc_coef.csc_coef01); 
		VDP_VID_SetVdmVdm12Yuv2rgb(u32Data, st_csc_coef.csc_coef02); 
		VDP_VID_SetVdmVdm13Yuv2rgb(u32Data, st_csc_coef.csc_coef10); 
		VDP_VID_SetVdmVdm14Yuv2rgb(u32Data, st_csc_coef.csc_coef11); 
		VDP_VID_SetVdmVdm15Yuv2rgb(u32Data, st_csc_coef.csc_coef12); 
		VDP_VID_SetVdmVdm16Yuv2rgb(u32Data, st_csc_coef.csc_coef20); 
		VDP_VID_SetVdmVdm17Yuv2rgb(u32Data, st_csc_coef.csc_coef21);
		VDP_VID_SetVdmVdm18Yuv2rgb(u32Data, st_csc_coef.csc_coef22); 

		VDP_VID_SetVdmVdmYuv2rgb10InDc(u32Data, st_csc_dc_coef.csc_in_dc0); 
		VDP_VID_SetVdmVdmYuv2rgb11InDc(u32Data, st_csc_dc_coef.csc_in_dc1); 
		VDP_VID_SetVdmVdmYuv2rgb12InDc(u32Data, st_csc_dc_coef.csc_in_dc2); 
		VDP_VID_SetVdmVdmYuv2rgb10OutDc(u32Data, st_csc_dc_coef.csc_out_dc0); 
		VDP_VID_SetVdmVdmYuv2rgb11OutDc(u32Data, st_csc_dc_coef.csc_out_dc1); 
		VDP_VID_SetVdmVdmYuv2rgb12OutDc(u32Data, st_csc_dc_coef.csc_out_dc2); 
	} 
	return ;
}



HI_VOID VDP_VID_SetVdmCsc0Mode(HI_U32 enLayer, VDP_CSC_MODE_E enCscMode)
{
    VDP_VDM_CSC_COEF_S pstYuv2RgbCfg;
    memset((HI_VOID *)&pstYuv2RgbCfg,0,sizeof(VDP_VDM_CSC_COEF_S));
    if(enCscMode == VDP_CSC_YUV2RGB_601)
     {
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][0]           = 9535    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][1]           = 0       ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][2]           = 13074   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][0]           = 9535    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][1]           = -3203   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][2]           = -6660   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][0]           = 9535    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][1]           = 16531   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][2]           = 0       ; 
        
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[0]         = -64     ; 
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[1]         = -512    ; 
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[2]         = -512    ; 
        
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[0]        = 0       ; 
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[1]        = 0       ; 
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[2]        = 0       ;         
     }
    else if(enCscMode == VDP_CSC_YUV2RGB_709)
    {
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][0]           = 9567    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][1]           = 0       ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[0][2]           = 13792   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][0]           = 9567    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][1]           = -1539   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[1][2]           = -5344   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][0]           = 9567    ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][1]           = 17597   ; 
        pstYuv2RgbCfg.s16M33YUV2RGB0[2][2]           = 0       ; 
    
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[0]         = -64     ; 
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[1]         = -512    ; 
        pstYuv2RgbCfg.s32InOffsetYUV2RGB0[2]         = -512    ; 
        
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[0]        = 0       ; 
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[1]        = 0       ; 
        pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[2]        = 0       ; 
    }     
    else ;     
    
        pstYuv2RgbCfg.u16ScaleYUV2RGB                = 13      ; 
        pstYuv2RgbCfg.u16ClipMinYUV2RGB              = 0       ; 
        pstYuv2RgbCfg.u16ClipMaxYUV2RGB              = 1023    ; 

        VDP_VID_SetVdmVdm00Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[0][0]);
        VDP_VID_SetVdmVdm01Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[0][1]);
        VDP_VID_SetVdmVdm02Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[0][2]);
        VDP_VID_SetVdmVdm03Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[1][0]);
        VDP_VID_SetVdmVdm04Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[1][1]);
        VDP_VID_SetVdmVdm05Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[1][2]);
        VDP_VID_SetVdmVdm06Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[2][0]);
        VDP_VID_SetVdmVdm07Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[2][1]);
        VDP_VID_SetVdmVdm08Yuv2rgb(enLayer,  pstYuv2RgbCfg.s16M33YUV2RGB0[2][2]);

        VDP_VID_SetVdmVdmYuv2rgb00InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB0[0] );
        VDP_VID_SetVdmVdmYuv2rgb01InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB0[1]);
        VDP_VID_SetVdmVdmYuv2rgb02InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB0[2]);
        VDP_VID_SetVdmVdmYuv2rgb00OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[0]);
        VDP_VID_SetVdmVdmYuv2rgb01OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[1]);
        VDP_VID_SetVdmVdmYuv2rgb02OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB0[2]);

}
HI_VOID VDP_VID_SetVdmCsc1Mode(HI_U32 enLayer, VDP_CSC_MODE_E enCscMode)
{
        VDP_VDM_CSC_COEF_S pstYuv2RgbCfg;
		memset((HI_VOID *)&pstYuv2RgbCfg,0,sizeof(VDP_VDM_CSC_COEF_S));

        if(enCscMode == VDP_CSC_YUV2RGB_601)
         {
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][0]           = 9535    ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][1]           = 0       ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][2]           = 13074   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][0]           = 9535    ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][1]           = -3203   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][2]           = -6660   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][0]           = 9535    ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][1]           = 16531   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][2]           = 0       ; 

            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[0]         = -64     ; 
            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[1]         = -512    ; 
            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[2]         = -512    ; 
            
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[0]        = 0       ; 
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[1]        = 0       ; 
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[2]        = 0       ;    
         }
        else if(enCscMode == VDP_CSC_YUV2RGB_709)
        {
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][0]             = 9567    ; //BT2020
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][1]             = 0       ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[0][2]             = 13792   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][0]             = 9567    ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][1]             = -1539   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[1][2]             = -5344   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][0]             = 9567    ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][1]             = 17597   ; 
            pstYuv2RgbCfg.s16M33YUV2RGB1[2][2]             = 0       ; 

            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[0]          = -64     ; 
            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[1]          = -512    ; 
            pstYuv2RgbCfg.s32InOffsetYUV2RGB1[2]          = -512    ; 
            
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[0]          = 0       ; 
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[1]          = 0       ; 
            pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[2]          = 0       ; 
        }     
        else ;
        pstYuv2RgbCfg.u16ScaleYUV2RGB                = 13      ; 
        pstYuv2RgbCfg.u16ClipMinYUV2RGB              = 0       ; 
        pstYuv2RgbCfg.u16ClipMaxYUV2RGB              = 1023    ; 

        VDP_VID_SetVdmVdm10Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[0][0] );
        VDP_VID_SetVdmVdm11Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[0][1] );
        VDP_VID_SetVdmVdm12Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[0][2] );
        VDP_VID_SetVdmVdm13Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[1][0] );
        VDP_VID_SetVdmVdm14Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[1][1] );
        VDP_VID_SetVdmVdm15Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[1][2] );
        VDP_VID_SetVdmVdm16Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[2][0] );
        VDP_VID_SetVdmVdm17Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[2][1] );
        VDP_VID_SetVdmVdm18Yuv2rgb(enLayer, pstYuv2RgbCfg.s16M33YUV2RGB1[2][2] );

        VDP_VID_SetVdmVdmYuv2rgbscale2p(enLayer, pstYuv2RgbCfg.u16ScaleYUV2RGB);
        
        VDP_VID_SetVdmVdmYuv2rgb10InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB1[0]);
        VDP_VID_SetVdmVdmYuv2rgb11InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB1[1]);
        VDP_VID_SetVdmVdmYuv2rgb12InDc(enLayer, pstYuv2RgbCfg.s32InOffsetYUV2RGB1[2]);
        
        VDP_VID_SetVdmVdmYuv2rgb10OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[0] );
        VDP_VID_SetVdmVdmYuv2rgb11OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[1] );
        VDP_VID_SetVdmVdmYuv2rgb12OutDc(enLayer, pstYuv2RgbCfg.s32OutOffsetYUV2RGB1[2] );
        
        VDP_VID_SetVdmVdmYuv2rgbClipMin(enLayer, pstYuv2RgbCfg.u16ClipMinYUV2RGB);
        VDP_VID_SetVdmVdmYuv2rgbClipMax(enLayer, pstYuv2RgbCfg.u16ClipMaxYUV2RGB);
        return ;
}

HI_VOID VDP_VID_SetRgb2YuvMode(HI_U32 enLayer, VDP_CSC_MODE_E enCscMode)
{
    HI_S16 vdm_00_rgb2yuv    =  0;  
    HI_S16 vdm_01_rgb2yuv    =  0;  
    HI_S16 vdm_02_rgb2yuv    =  0;  
    HI_S16 vdm_10_rgb2yuv    =  0;  
    HI_S16 vdm_11_rgb2yuv    =  0;  
    HI_S16 vdm_12_rgb2yuv    =  0;  
    HI_S16 vdm_20_rgb2yuv    =  0;  
    HI_S16 vdm_21_rgb2yuv    =  0;  
    HI_S16 vdm_22_rgb2yuv    =  0;  
    HI_U16 vdm_rgb2yuvscale2p    =  0;  
    HI_S16 vdm_rgb2yuv_0_in_dc    =  0; 
    HI_S16 vdm_rgb2yuv_1_in_dc    =  0; 
    HI_S16 vdm_rgb2yuv_2_in_dc    =  0; 
    HI_S16 vdm_rgb2yuv_0_out_dc    =  0;    
    HI_S16 vdm_rgb2yuv_1_out_dc    =  0;    
    HI_S16 vdm_rgb2yuv_2_out_dc    =  0;    
    HI_U16 vdm_rgb2yuv_clip_min    =  0;    
    HI_U16 vdm_rgb2yuv_clip_max    =  0;    

    if(enCscMode == VDP_CSC_RGB2YUV_601)
    {
        vdm_00_rgb2yuv              = 4211    ; //BT709
        vdm_01_rgb2yuv              = 8258   ; 
        vdm_02_rgb2yuv              = 1606    ; 
        vdm_10_rgb2yuv              = -2425   ; 
        vdm_11_rgb2yuv              = -4768   ; 
        vdm_12_rgb2yuv              = 7193    ; 
        vdm_20_rgb2yuv              = 7193    ; 
        vdm_21_rgb2yuv              = -6029   ; 
        vdm_22_rgb2yuv              = -1163    ; 
                
        vdm_rgb2yuv_0_in_dc          = 0       ; 
        vdm_rgb2yuv_1_in_dc          = 0       ; 
        vdm_rgb2yuv_2_in_dc          = 0       ; 
        
        vdm_rgb2yuv_0_out_dc        = 64      ; 
        vdm_rgb2yuv_1_out_dc        = 512     ; 
        vdm_rgb2yuv_2_out_dc        = 512     ;  
    }
    else if(enCscMode == VDP_CSC_RGB2YUV_709)
    {
        vdm_00_rgb2yuv              = 2984    ; //BT709
        vdm_01_rgb2yuv              = 10034   ; 
        vdm_02_rgb2yuv              = 1013    ; 
        vdm_10_rgb2yuv              = -1643   ; 
        vdm_11_rgb2yuv              = -5531   ; 
        vdm_12_rgb2yuv              = 7175    ; 
        vdm_20_rgb2yuv              = 7175    ; 
        vdm_21_rgb2yuv              = -6518   ; 
        vdm_22_rgb2yuv              = -659    ; 
        
        vdm_rgb2yuv_0_in_dc          = 0       ; 
        vdm_rgb2yuv_1_in_dc          = 0       ; 
        vdm_rgb2yuv_2_in_dc          = 0       ; 
        
        vdm_rgb2yuv_0_out_dc        = 64      ; 
        vdm_rgb2yuv_1_out_dc        = 512     ; 
        vdm_rgb2yuv_2_out_dc        = 512     ;  
    }    
    else 
    {
        VDP_PRINT("Error! Rgb2YuvMode : No this Mode!!! \n");
    }

    vdm_rgb2yuvscale2p                = 14      ; 
    vdm_rgb2yuv_clip_min           = 0       ; 
    vdm_rgb2yuv_clip_max            = 1023    ; 

    VDP_VID_SetVdmVdm00Rgb2yuv(enLayer, vdm_00_rgb2yuv);
    VDP_VID_SetVdmVdm01Rgb2yuv(enLayer, vdm_01_rgb2yuv);
    VDP_VID_SetVdmVdm02Rgb2yuv(enLayer, vdm_02_rgb2yuv);
    VDP_VID_SetVdmVdm10Rgb2yuv(enLayer, vdm_10_rgb2yuv);
    VDP_VID_SetVdmVdm11Rgb2yuv(enLayer, vdm_11_rgb2yuv);
    VDP_VID_SetVdmVdm12Rgb2yuv(enLayer, vdm_12_rgb2yuv);
    VDP_VID_SetVdmVdm20Rgb2yuv(enLayer, vdm_20_rgb2yuv);
    VDP_VID_SetVdmVdm21Rgb2yuv(enLayer, vdm_21_rgb2yuv);
    VDP_VID_SetVdmVdm22Rgb2yuv(enLayer, vdm_22_rgb2yuv);
    
    VDP_VID_SetVdmVdmRgb2yuvscale2p(enLayer, vdm_rgb2yuvscale2p);
    
    VDP_VID_SetVdmVdmRgb2yuv0InDc(enLayer, vdm_rgb2yuv_0_in_dc);
    VDP_VID_SetVdmVdmRgb2yuv1InDc(enLayer, vdm_rgb2yuv_1_in_dc);
    VDP_VID_SetVdmVdmRgb2yuv2InDc(enLayer, vdm_rgb2yuv_2_in_dc);
    
    VDP_VID_SetVdmVdmRgb2yuv0OutDc(enLayer, vdm_rgb2yuv_0_out_dc);
    VDP_VID_SetVdmVdmRgb2yuv1OutDc(enLayer, vdm_rgb2yuv_1_out_dc);
    VDP_VID_SetVdmVdmRgb2yuv2OutDc(enLayer, vdm_rgb2yuv_2_out_dc);
    
    VDP_VID_SetVdmVdmRgb2yuvClipMin(enLayer, vdm_rgb2yuv_clip_min);
    VDP_VID_SetVdmVdmRgb2yuvClipMax(enLayer, vdm_rgb2yuv_clip_max);

}


