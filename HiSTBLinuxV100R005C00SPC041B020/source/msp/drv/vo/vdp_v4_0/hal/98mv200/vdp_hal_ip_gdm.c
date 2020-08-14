
#include "vdp_hal_ip_gdm.h"
#include "vdp_hal_comm.h"
#include "vdp_define.h"


extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_GP_SetGdmGdmTmapInBdp(HI_U32 u32Data, HI_U32 gdm_tmap_in_bdp)
{
	U_GDM_CTRL GDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapInBdp Select Wrong Layer ID\n");
	}
	
	GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CTRL.bits.gdm_tmap_in_bdp = gdm_tmap_in_bdp;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET),GDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmGmmEn(HI_U32 u32Data, HI_U32 gdm_gmm_en)
{
	U_GDM_CTRL GDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmGmmEn Select Wrong Layer ID\n");
	}
	
	GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CTRL.bits.gdm_gmm_en = gdm_gmm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET),GDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapEn(HI_U32 u32Data, HI_U32 gdm_tmap_en)
{
	U_GDM_CTRL GDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapEn Select Wrong Layer ID\n");
	}
	
	GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CTRL.bits.gdm_tmap_en = gdm_tmap_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET),GDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDegmmEn(HI_U32 u32Data, HI_U32 gdm_degmm_en)
{
	U_GDM_CTRL GDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDegmmEn Select Wrong Layer ID\n");
	}
	
	GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CTRL.bits.gdm_degmm_en = gdm_degmm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET),GDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmEn(HI_U32 u32Data, HI_U32 gdm_en)
{
	U_GDM_CTRL GDM_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmEn Select Wrong Layer ID\n");
	}
	
	GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CTRL.bits.gdm_en = gdm_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32) + u32Data * GP_OFFSET),GDM_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmLumaCoef0Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef0_tmap)
{
	U_GDM_TMAP_LUMA_COEF0 GDM_TMAP_LUMA_COEF0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmLumaCoef0Tmap Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_LUMA_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF0.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_LUMA_COEF0.bits.gdm_luma_coef0_tmap = gdm_luma_coef0_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF0.u32) + u32Data * GP_OFFSET),GDM_TMAP_LUMA_COEF0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmLumaCoef1Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef1_tmap)
{
	U_GDM_TMAP_LUMA_COEF1 GDM_TMAP_LUMA_COEF1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmLumaCoef1Tmap Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_LUMA_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF1.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_LUMA_COEF1.bits.gdm_luma_coef1_tmap = gdm_luma_coef1_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF1.u32) + u32Data * GP_OFFSET),GDM_TMAP_LUMA_COEF1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmLumaCoef2Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef2_tmap)
{
	U_GDM_TMAP_LUMA_COEF2 GDM_TMAP_LUMA_COEF2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmLumaCoef2Tmap Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_LUMA_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF2.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_LUMA_COEF2.bits.gdm_luma_coef2_tmap = gdm_luma_coef2_tmap;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_COEF2.u32) + u32Data * GP_OFFSET),GDM_TMAP_LUMA_COEF2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapLumaScale(HI_U32 u32Data, HI_U32 gdm_tmap_luma_scale)
{
	U_GDM_TMAP_LUMA_SCALE GDM_TMAP_LUMA_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapLumaScale Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_LUMA_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_SCALE.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_LUMA_SCALE.bits.gdm_tmap_luma_scale = gdm_tmap_luma_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_LUMA_SCALE.u32) + u32Data * GP_OFFSET),GDM_TMAP_LUMA_SCALE.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapCoefScale(HI_U32 u32Data, HI_U32 gdm_tmap_coef_scale)
{
	U_GDM_TMAP_COEF_SCALE GDM_TMAP_COEF_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapCoefScale Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_COEF_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_COEF_SCALE.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_COEF_SCALE.bits.gdm_tmap_coef_scale = gdm_tmap_coef_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_COEF_SCALE.u32) + u32Data * GP_OFFSET),GDM_TMAP_COEF_SCALE.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapOutClipMin(HI_U32 u32Data, HI_U32 gdm_tmap_out_clip_min)
{
	U_GDM_TMAP_OUT_CLIP_MIN GDM_TMAP_OUT_CLIP_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapOutClipMin Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_OUT_CLIP_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_OUT_CLIP_MIN.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_OUT_CLIP_MIN.bits.gdm_tmap_out_clip_min = gdm_tmap_out_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_OUT_CLIP_MIN.u32) + u32Data * GP_OFFSET),GDM_TMAP_OUT_CLIP_MIN.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapOutClipMax(HI_U32 u32Data, HI_U32 gdm_tmap_out_clip_max)
{
	U_GDM_TMAP_OUT_CLIP_MAX GDM_TMAP_OUT_CLIP_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapOutClipMax Select Wrong Layer ID\n");
	}
	
	GDM_TMAP_OUT_CLIP_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TMAP_OUT_CLIP_MAX.u32) + u32Data * GP_OFFSET));
	GDM_TMAP_OUT_CLIP_MAX.bits.gdm_tmap_out_clip_max = gdm_tmap_out_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TMAP_OUT_CLIP_MAX.u32) + u32Data * GP_OFFSET),GDM_TMAP_OUT_CLIP_MAX.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmParaGmmAddr(HI_U32 u32Data, HI_U32 gdm_para_gmm_addr)
{
	U_GDM_PARA_GMM_ADDR GDM_PARA_GMM_ADDR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmParaGmmAddr Select Wrong Layer ID\n");
	}
	
	GDM_PARA_GMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_GMM_ADDR.u32) + u32Data * GP_OFFSET));
	GDM_PARA_GMM_ADDR.bits.gdm_para_gmm_addr = gdm_para_gmm_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_PARA_GMM_ADDR.u32) + u32Data * GP_OFFSET),GDM_PARA_GMM_ADDR.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmParaTmapAddr(HI_U32 u32Data, HI_U32 gdm_para_tmap_addr)
{
	U_GDM_PARA_TMAP_ADDR GDM_PARA_TMAP_ADDR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmParaTmapAddr Select Wrong Layer ID\n");
	}
	
	GDM_PARA_TMAP_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_TMAP_ADDR.u32) + u32Data * GP_OFFSET));
	GDM_PARA_TMAP_ADDR.bits.gdm_para_tmap_addr = gdm_para_tmap_addr;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_PARA_TMAP_ADDR.u32) + u32Data * GP_OFFSET),GDM_PARA_TMAP_ADDR.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmParaTmapUpd(HI_U32 u32Data, HI_U32 gdm_para_tmap_upd)
{
	U_GDM_PARA_UP GDM_PARA_UP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmParaTmapUpd Select Wrong Layer ID\n");
	}
	
	GDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_UP.u32) + u32Data * GP_OFFSET));
	GDM_PARA_UP.bits.gdm_para_tmap_upd = gdm_para_tmap_upd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_PARA_UP.u32) + u32Data * GP_OFFSET),GDM_PARA_UP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmParaGmmUpd(HI_U32 u32Data, HI_U32 gdm_para_gmm_upd)
{
	U_GDM_PARA_UP GDM_PARA_UP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmParaGmmUpd Select Wrong Layer ID\n");
	}
	
	GDM_PARA_UP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_UP.u32) + u32Data * GP_OFFSET));
	GDM_PARA_UP.bits.gdm_para_gmm_upd = gdm_para_gmm_upd;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_PARA_UP.u32) + u32Data * GP_OFFSET),GDM_PARA_UP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX4Step(HI_U32 u32Data, HI_U32 g_degmm_x4_step)
{
	U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX4Step Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_STEP.bits.g_degmm_x4_step = g_degmm_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX3Step(HI_U32 u32Data, HI_U32 g_degmm_x3_step)
{
	U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX3Step Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_STEP.bits.g_degmm_x3_step = g_degmm_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX2Step(HI_U32 u32Data, HI_U32 g_degmm_x2_step)
{
	U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX2Step Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_STEP.bits.g_degmm_x2_step = g_degmm_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX1Step(HI_U32 u32Data, HI_U32 g_degmm_x1_step)
{
	U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX1Step Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_STEP.bits.g_degmm_x1_step = g_degmm_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX2Pos(HI_U32 u32Data, HI_U32 g_degmm_x2_pos)
{
	U_GDM_DEGAMMA_POS0 GDM_DEGAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX2Pos Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS0.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_POS0.bits.g_degmm_x2_pos = g_degmm_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS0.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX1Pos(HI_U32 u32Data, HI_U32 g_degmm_x1_pos)
{
	U_GDM_DEGAMMA_POS0 GDM_DEGAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX1Pos Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS0.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_POS0.bits.g_degmm_x1_pos = g_degmm_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS0.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX4Pos(HI_U32 u32Data, HI_U32 g_degmm_x4_pos)
{
	U_GDM_DEGAMMA_POS1 GDM_DEGAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX4Pos Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS1.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_POS1.bits.g_degmm_x4_pos = g_degmm_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS1.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX3Pos(HI_U32 u32Data, HI_U32 g_degmm_x3_pos)
{
	U_GDM_DEGAMMA_POS1 GDM_DEGAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX3Pos Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS1.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_POS1.bits.g_degmm_x3_pos = g_degmm_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_POS1.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX4Num(HI_U32 u32Data, HI_U32 g_degmm_x4_num)
{
	U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX4Num Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_NUM.bits.g_degmm_x4_num = g_degmm_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX3Num(HI_U32 u32Data, HI_U32 g_degmm_x3_num)
{
	U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX3Num Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_NUM.bits.g_degmm_x3_num = g_degmm_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX2Num(HI_U32 u32Data, HI_U32 g_degmm_x2_num)
{
	U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX2Num Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_NUM.bits.g_degmm_x2_num = g_degmm_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGDegmmX1Num(HI_U32 u32Data, HI_U32 g_degmm_x1_num)
{
	U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGDegmmX1Num Select Wrong Layer ID\n");
	}
	
	GDM_DEGAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_DEGAMMA_NUM.bits.g_degmm_x1_num = g_degmm_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DEGAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_DEGAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapRdEn(HI_U32 u32Data, HI_U32 gdm_tmap_rd_en)
{
	U_GDM_TONEMAP_REN GDM_TONEMAP_REN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapRdEn Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_REN.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_REN.bits.gdm_tmap_rd_en = gdm_tmap_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_REN.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_REN.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmTmapParaData(HI_U32 u32Data, HI_U32 gdm_tmap_para_data)
{
	U_GDM_TONEMAP_DATA GDM_TONEMAP_DATA;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmTmapParaData Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_DATA.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_DATA.bits.gdm_tmap_para_data = gdm_tmap_para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_DATA.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_DATA.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX4Step(HI_U32 u32Data, HI_U32 g_tmap_x4_step)
{
	U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX4Step Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_STEP.bits.g_tmap_x4_step = g_tmap_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX3Step(HI_U32 u32Data, HI_U32 g_tmap_x3_step)
{
	U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX3Step Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_STEP.bits.g_tmap_x3_step = g_tmap_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX2Step(HI_U32 u32Data, HI_U32 g_tmap_x2_step)
{
	U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX2Step Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_STEP.bits.g_tmap_x2_step = g_tmap_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX1Step(HI_U32 u32Data, HI_U32 g_tmap_x1_step)
{
	U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX1Step Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_STEP.bits.g_tmap_x1_step = g_tmap_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_STEP.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX1Pos(HI_U32 u32Data, HI_U32 g_tmap_x1_pos)
{
	U_GDM_TONEMAP_POS0 GDM_TONEMAP_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX1Pos Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS0.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_POS0.bits.g_tmap_x1_pos = g_tmap_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS0.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_POS0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX2Pos(HI_U32 u32Data, HI_U32 g_tmap_x2_pos)
{
	U_GDM_TONEMAP_POS1 GDM_TONEMAP_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX2Pos Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS1.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_POS1.bits.g_tmap_x2_pos = g_tmap_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS1.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_POS1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX4Num(HI_U32 u32Data, HI_U32 g_tmap_x4_num)
{
	U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX4Num Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_NUM.bits.g_tmap_x4_num = g_tmap_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX3Num(HI_U32 u32Data, HI_U32 g_tmap_x3_num)
{
	U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX3Num Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_NUM.bits.g_tmap_x3_num = g_tmap_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX2Num(HI_U32 u32Data, HI_U32 g_tmap_x2_num)
{
	U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX2Num Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_NUM.bits.g_tmap_x2_num = g_tmap_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX1Num(HI_U32 u32Data, HI_U32 g_tmap_x1_num)
{
	U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX1Num Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_NUM.bits.g_tmap_x1_num = g_tmap_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_NUM.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX3Pos(HI_U32 u32Data, HI_U32 g_tmap_x3_pos)
{
	U_GDM_TONEMAP_POS2 GDM_TONEMAP_POS2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX3Pos Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS2.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_POS2.bits.g_tmap_x3_pos = g_tmap_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS2.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_POS2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGTmapX4Pos(HI_U32 u32Data, HI_U32 g_tmap_x4_pos)
{
	U_GDM_TONEMAP_POS3 GDM_TONEMAP_POS3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGTmapX4Pos Select Wrong Layer ID\n");
	}
	
	GDM_TONEMAP_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS3.u32) + u32Data * GP_OFFSET));
	GDM_TONEMAP_POS3.bits.g_tmap_x4_pos = g_tmap_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_TONEMAP_POS3.u32) + u32Data * GP_OFFSET),GDM_TONEMAP_POS3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmGammaRdEn(HI_U32 u32Data, HI_U32 gdm_gamma_rd_en)
{
	U_GDM_GAMMA_REN GDM_GAMMA_REN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmGammaRdEn Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_REN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_REN.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_REN.bits.gdm_gamma_rd_en = gdm_gamma_rd_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_REN.u32) + u32Data * GP_OFFSET),GDM_GAMMA_REN.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmGammaParaData(HI_U32 u32Data, HI_U32 gdm_gamma_para_data)
{
	U_GDM_GAMMMA_DATA GDM_GAMMMA_DATA;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmGammaParaData Select Wrong Layer ID\n");
	}
	
	GDM_GAMMMA_DATA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMMA_DATA.u32) + u32Data * GP_OFFSET));
	GDM_GAMMMA_DATA.bits.gdm_gamma_para_data = gdm_gamma_para_data;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMMA_DATA.u32) + u32Data * GP_OFFSET),GDM_GAMMMA_DATA.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX4Step(HI_U32 u32Data, HI_U32 g_gmm_x4_step)
{
	U_GDM_GAMMA_STEP GDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX4Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP.bits.g_gmm_x4_step = g_gmm_x4_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX3Step(HI_U32 u32Data, HI_U32 g_gmm_x3_step)
{
	U_GDM_GAMMA_STEP GDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX3Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP.bits.g_gmm_x3_step = g_gmm_x3_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX2Step(HI_U32 u32Data, HI_U32 g_gmm_x2_step)
{
	U_GDM_GAMMA_STEP GDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX2Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP.bits.g_gmm_x2_step = g_gmm_x2_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX1Step(HI_U32 u32Data, HI_U32 g_gmm_x1_step)
{
	U_GDM_GAMMA_STEP GDM_GAMMA_STEP;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX1Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP.bits.g_gmm_x1_step = g_gmm_x1_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX2Pos(HI_U32 u32Data, HI_U32 g_gmm_x2_pos)
{
	U_GDM_GAMMA_POS0 GDM_GAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX2Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS0.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS0.bits.g_gmm_x2_pos = g_gmm_x2_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS0.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX1Pos(HI_U32 u32Data, HI_U32 g_gmm_x1_pos)
{
	U_GDM_GAMMA_POS0 GDM_GAMMA_POS0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX1Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS0.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS0.bits.g_gmm_x1_pos = g_gmm_x1_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS0.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX4Pos(HI_U32 u32Data, HI_U32 g_gmm_x4_pos)
{
	U_GDM_GAMMA_POS1 GDM_GAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX4Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS1.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS1.bits.g_gmm_x4_pos = g_gmm_x4_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS1.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX3Pos(HI_U32 u32Data, HI_U32 g_gmm_x3_pos)
{
	U_GDM_GAMMA_POS1 GDM_GAMMA_POS1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX3Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS1.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS1.bits.g_gmm_x3_pos = g_gmm_x3_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS1.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX4Num(HI_U32 u32Data, HI_U32 g_gmm_x4_num)
{
	U_GDM_GAMMA_NUM GDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX4Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM.bits.g_gmm_x4_num = g_gmm_x4_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX3Num(HI_U32 u32Data, HI_U32 g_gmm_x3_num)
{
	U_GDM_GAMMA_NUM GDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX3Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM.bits.g_gmm_x3_num = g_gmm_x3_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX2Num(HI_U32 u32Data, HI_U32 g_gmm_x2_num)
{
	U_GDM_GAMMA_NUM GDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX2Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM.bits.g_gmm_x2_num = g_gmm_x2_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX1Num(HI_U32 u32Data, HI_U32 g_gmm_x1_num)
{
	U_GDM_GAMMA_NUM GDM_GAMMA_NUM;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX1Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM.bits.g_gmm_x1_num = g_gmm_x1_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM.u32);
	
	return ;
}

HI_VOID VDP_GP_SetGdmGGmmX8Step(HI_U32 u32Data, HI_U32 g_gmm_x8_step)
{
	U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX8Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP2.bits.g_gmm_x8_step = g_gmm_x8_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX7Step(HI_U32 u32Data, HI_U32 g_gmm_x7_step)
{
	U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX7Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP2.bits.g_gmm_x7_step = g_gmm_x7_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX6Step(HI_U32 u32Data, HI_U32 g_gmm_x6_step)
{
	U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX6Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP2.bits.g_gmm_x6_step = g_gmm_x6_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX5Step(HI_U32 u32Data, HI_U32 g_gmm_x5_step)
{
	U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX5Step Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_STEP2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_STEP2.bits.g_gmm_x5_step = g_gmm_x5_step;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_STEP2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_STEP2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX6Pos(HI_U32 u32Data, HI_U32 g_gmm_x6_pos)
{
	U_GDM_GAMMA_POS2 GDM_GAMMA_POS2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX6Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS2.bits.g_gmm_x6_pos = g_gmm_x6_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX5Pos(HI_U32 u32Data, HI_U32 g_gmm_x5_pos)
{
	U_GDM_GAMMA_POS2 GDM_GAMMA_POS2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX5Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS2.bits.g_gmm_x5_pos = g_gmm_x5_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX8Pos(HI_U32 u32Data, HI_U32 g_gmm_x8_pos)
{
	U_GDM_GAMMA_POS3 GDM_GAMMA_POS3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX8Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS3.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS3.bits.g_gmm_x8_pos = g_gmm_x8_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS3.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX7Pos(HI_U32 u32Data, HI_U32 g_gmm_x7_pos)
{
	U_GDM_GAMMA_POS3 GDM_GAMMA_POS3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX7Pos Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_POS3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS3.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_POS3.bits.g_gmm_x7_pos = g_gmm_x7_pos;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_POS3.u32) + u32Data * GP_OFFSET),GDM_GAMMA_POS3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX8Num(HI_U32 u32Data, HI_U32 g_gmm_x8_num)
{
	U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX8Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM2.bits.g_gmm_x8_num = g_gmm_x8_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX7Num(HI_U32 u32Data, HI_U32 g_gmm_x7_num)
{
	U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX7Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM2.bits.g_gmm_x7_num = g_gmm_x7_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX6Num(HI_U32 u32Data, HI_U32 g_gmm_x6_num)
{
	U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX6Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM2.bits.g_gmm_x6_num = g_gmm_x6_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGGmmX5Num(HI_U32 u32Data, HI_U32 g_gmm_x5_num)
{
	U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGGmmX5Num Select Wrong Layer ID\n");
	}
	
	GDM_GAMMA_NUM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET));
	GDM_GAMMA_NUM2.bits.g_gmm_x5_num = g_gmm_x5_num;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_GAMMA_NUM2.u32) + u32Data * GP_OFFSET),GDM_GAMMA_NUM2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCscEn(HI_U32 u32Data, HI_U32 gdm_csc_en)
{
	U_GDM_CSC_CTRL GDM_CSC_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCscEn Select Wrong Layer ID\n");
	}
	
	GDM_CSC_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_CSC_CTRL.bits.gdm_csc_en = gdm_csc_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_CTRL.u32) + u32Data * GP_OFFSET),GDM_CSC_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef00Csc(HI_U32 u32Data, HI_U32 gdm_coef00_csc)
{
	U_GDM_CSC_COEF00 GDM_CSC_COEF00;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef00Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF00.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF00.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF00.bits.gdm_coef00_csc = gdm_coef00_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF00.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF00.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef01Csc(HI_U32 u32Data, HI_U32 gdm_coef01_csc)
{
	U_GDM_CSC_COEF01 GDM_CSC_COEF01;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef01Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF01.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF01.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF01.bits.gdm_coef01_csc = gdm_coef01_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF01.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF01.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef02Csc(HI_U32 u32Data, HI_U32 gdm_coef02_csc)
{
	U_GDM_CSC_COEF02 GDM_CSC_COEF02;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef02Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF02.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF02.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF02.bits.gdm_coef02_csc = gdm_coef02_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF02.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF02.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef10Csc(HI_U32 u32Data, HI_U32 gdm_coef10_csc)
{
	U_GDM_CSC_COEF10 GDM_CSC_COEF10;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef10Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF10.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF10.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF10.bits.gdm_coef10_csc = gdm_coef10_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF10.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF10.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef11Csc(HI_U32 u32Data, HI_U32 gdm_coef11_csc)
{
	U_GDM_CSC_COEF11 GDM_CSC_COEF11;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef11Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF11.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF11.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF11.bits.gdm_coef11_csc = gdm_coef11_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF11.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF11.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef12Csc(HI_U32 u32Data, HI_U32 gdm_coef12_csc)
{
	U_GDM_CSC_COEF12 GDM_CSC_COEF12;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef12Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF12.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF12.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF12.bits.gdm_coef12_csc = gdm_coef12_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF12.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF12.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef20Csc(HI_U32 u32Data, HI_U32 gdm_coef20_csc)
{
	U_GDM_CSC_COEF20 GDM_CSC_COEF20;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef20Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF20.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF20.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF20.bits.gdm_coef20_csc = gdm_coef20_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF20.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF20.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef21Csc(HI_U32 u32Data, HI_U32 gdm_coef21_csc)
{
	U_GDM_CSC_COEF21 GDM_CSC_COEF21;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef21Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF21.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF21.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF21.bits.gdm_coef21_csc = gdm_coef21_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF21.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF21.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCoef22Csc(HI_U32 u32Data, HI_U32 gdm_coef22_csc)
{
	U_GDM_CSC_COEF22 GDM_CSC_COEF22;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCoef22Csc Select Wrong Layer ID\n");
	}
	
	GDM_CSC_COEF22.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF22.u32) + u32Data * GP_OFFSET));
	GDM_CSC_COEF22.bits.gdm_coef22_csc = gdm_coef22_csc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_COEF22.u32) + u32Data * GP_OFFSET),GDM_CSC_COEF22.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCscScale(HI_U32 u32Data, HI_U32 gdm_csc_scale)
{
	U_GDM_CSC_SCALE GDM_CSC_SCALE;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCscScale Select Wrong Layer ID\n");
	}
	
	GDM_CSC_SCALE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_SCALE.u32) + u32Data * GP_OFFSET));
	GDM_CSC_SCALE.bits.gdm_csc_scale = gdm_csc_scale;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_SCALE.u32) + u32Data * GP_OFFSET),GDM_CSC_SCALE.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCscClipMin(HI_U32 u32Data, HI_U32 gdm_csc_clip_min)
{
	U_GDM_CSC_CLIP_MIN GDM_CSC_CLIP_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCscClipMin Select Wrong Layer ID\n");
	}
	
	GDM_CSC_CLIP_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_CLIP_MIN.u32) + u32Data * GP_OFFSET));
	GDM_CSC_CLIP_MIN.bits.gdm_csc_clip_min = gdm_csc_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_CLIP_MIN.u32) + u32Data * GP_OFFSET),GDM_CSC_CLIP_MIN.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmCscClipMax(HI_U32 u32Data, HI_U32 gdm_csc_clip_max)
{
	U_GDM_CSC_CLIP_MAX GDM_CSC_CLIP_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmCscClipMax Select Wrong Layer ID\n");
	}
	
	GDM_CSC_CLIP_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CSC_CLIP_MAX.u32) + u32Data * GP_OFFSET));
	GDM_CSC_CLIP_MAX.bits.gdm_csc_clip_max = gdm_csc_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_CSC_CLIP_MAX.u32) + u32Data * GP_OFFSET),GDM_CSC_CLIP_MAX.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuvEn(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_en)
{
	U_GDM_RGB2YUV_CTRL GDM_RGB2YUV_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuvEn Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_CTRL.bits.gdm_rgb2yuv_en = gdm_rgb2yuv_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_CTRL.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm00Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_00_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF0 GDM_RGB2YUV_COEF0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm00Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF0.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF0.bits.gdm_00_rgb2yuv = gdm_00_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF0.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm01Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_01_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF1 GDM_RGB2YUV_COEF1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm01Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF1.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF1.bits.gdm_01_rgb2yuv = gdm_01_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF1.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm02Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_02_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF2 GDM_RGB2YUV_COEF2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm02Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF2.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF2.bits.gdm_02_rgb2yuv = gdm_02_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF2.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm10Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_10_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF3 GDM_RGB2YUV_COEF3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm10Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF3.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF3.bits.gdm_10_rgb2yuv = gdm_10_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF3.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm11Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_11_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF4 GDM_RGB2YUV_COEF4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm11Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF4.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF4.bits.gdm_11_rgb2yuv = gdm_11_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF4.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF4.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm12Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_12_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF5 GDM_RGB2YUV_COEF5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm12Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF5.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF5.bits.gdm_12_rgb2yuv = gdm_12_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF5.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF5.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm20Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_20_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF6 GDM_RGB2YUV_COEF6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm20Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF6.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF6.bits.gdm_20_rgb2yuv = gdm_20_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF6.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF6.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm21Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_21_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF7 GDM_RGB2YUV_COEF7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm21Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF7.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF7.bits.gdm_21_rgb2yuv = gdm_21_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF7.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF7.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdm22Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_22_rgb2yuv)
{
	U_GDM_RGB2YUV_COEF8 GDM_RGB2YUV_COEF8;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdm22Rgb2yuv Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_COEF8.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF8.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_COEF8.bits.gdm_22_rgb2yuv = gdm_22_rgb2yuv;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_COEF8.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_COEF8.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuvscale2p(HI_U32 u32Data, HI_U32 gdm_rgb2yuvscale2p)
{
	U_GDM_RGB2YUV_SCALE2P GDM_RGB2YUV_SCALE2P;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuvscale2p Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_SCALE2P.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_SCALE2P.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_SCALE2P.bits.gdm_rgb2yuvscale2p = gdm_rgb2yuvscale2p;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_SCALE2P.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_SCALE2P.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv0InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_0_in_dc)
{
	U_GDM_RGB2YUV_IN_DC0 GDM_RGB2YUV_IN_DC0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv0InDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_IN_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC0.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_IN_DC0.bits.gdm_rgb2yuv_0_in_dc = gdm_rgb2yuv_0_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC0.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_IN_DC0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv1InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_1_in_dc)
{
	U_GDM_RGB2YUV_IN_DC1 GDM_RGB2YUV_IN_DC1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv1InDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_IN_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC1.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_IN_DC1.bits.gdm_rgb2yuv_1_in_dc = gdm_rgb2yuv_1_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC1.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_IN_DC1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv2InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_2_in_dc)
{
	U_GDM_RGB2YUV_IN_DC2 GDM_RGB2YUV_IN_DC2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv2InDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_IN_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC2.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_IN_DC2.bits.gdm_rgb2yuv_2_in_dc = gdm_rgb2yuv_2_in_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_IN_DC2.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_IN_DC2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv0OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_0_out_dc)
{
	U_GDM_RGB2YUV_OUT_DC0 GDM_RGB2YUV_OUT_DC0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv0OutDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_OUT_DC0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC0.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_OUT_DC0.bits.gdm_rgb2yuv_0_out_dc = gdm_rgb2yuv_0_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC0.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_OUT_DC0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv1OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_1_out_dc)
{
	U_GDM_RGB2YUV_OUT_DC1 GDM_RGB2YUV_OUT_DC1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv1OutDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_OUT_DC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC1.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_OUT_DC1.bits.gdm_rgb2yuv_1_out_dc = gdm_rgb2yuv_1_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC1.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_OUT_DC1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuv2OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_2_out_dc)
{
	U_GDM_RGB2YUV_OUT_DC2 GDM_RGB2YUV_OUT_DC2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuv2OutDc Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_OUT_DC2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC2.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_OUT_DC2.bits.gdm_rgb2yuv_2_out_dc = gdm_rgb2yuv_2_out_dc;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_OUT_DC2.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_OUT_DC2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuvClipMin(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_clip_min)
{
	U_GDM_RGB2YUV_MIN GDM_RGB2YUV_MIN;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuvClipMin Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_MIN.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_MIN.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_MIN.bits.gdm_rgb2yuv_clip_min = gdm_rgb2yuv_clip_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_MIN.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_MIN.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmRgb2yuvClipMax(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_clip_max)
{
	U_GDM_RGB2YUV_MAX GDM_RGB2YUV_MAX;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmRgb2yuvClipMax Select Wrong Layer ID\n");
	}
	
	GDM_RGB2YUV_MAX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_MAX.u32) + u32Data * GP_OFFSET));
	GDM_RGB2YUV_MAX.bits.gdm_rgb2yuv_clip_max = gdm_rgb2yuv_clip_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_RGB2YUV_MAX.u32) + u32Data * GP_OFFSET),GDM_RGB2YUV_MAX.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherRoundUnlim(HI_U32 u32Data, HI_U32 gdm_dither_round_unlim)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherRoundUnlim Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_round_unlim = gdm_dither_round_unlim;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherEn(HI_U32 u32Data, HI_U32 gdm_dither_en)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherEn Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_en = gdm_dither_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherMode(HI_U32 u32Data, HI_U32 gdm_dither_mode)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherMode Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_mode = gdm_dither_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherRound(HI_U32 u32Data, HI_U32 gdm_dither_round)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherRound Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_round = gdm_dither_round;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherDomainMode(HI_U32 u32Data, HI_U32 gdm_dither_domain_mode)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherDomainMode Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_domain_mode = gdm_dither_domain_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherTapMode(HI_U32 u32Data, HI_U32 gdm_dither_tap_mode)
{
	U_GDM_DITHER_CTRL GDM_DITHER_CTRL;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherTapMode Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_CTRL.bits.gdm_dither_tap_mode = gdm_dither_tap_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_CTRL.u32) + u32Data * GP_OFFSET),GDM_DITHER_CTRL.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedY0(HI_U32 u32Data, HI_U32 gdm_dither_sed_y0)
{
	U_GDM_DITHER_SED_Y0 GDM_DITHER_SED_Y0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedY0 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_Y0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y0.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_Y0.bits.gdm_dither_sed_y0 = gdm_dither_sed_y0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y0.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_Y0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedU0(HI_U32 u32Data, HI_U32 gdm_dither_sed_u0)
{
	U_GDM_DITHER_SED_U0 GDM_DITHER_SED_U0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedU0 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_U0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U0.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_U0.bits.gdm_dither_sed_u0 = gdm_dither_sed_u0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U0.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_U0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedV0(HI_U32 u32Data, HI_U32 gdm_dither_sed_v0)
{
	U_GDM_DITHER_SED_V0 GDM_DITHER_SED_V0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedV0 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_V0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V0.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_V0.bits.gdm_dither_sed_v0 = gdm_dither_sed_v0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V0.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_V0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedW0(HI_U32 u32Data, HI_U32 gdm_dither_sed_w0)
{
	U_GDM_DITHER_SED_W0 GDM_DITHER_SED_W0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedW0 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_W0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W0.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_W0.bits.gdm_dither_sed_w0 = gdm_dither_sed_w0;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W0.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_W0.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedY1(HI_U32 u32Data, HI_U32 gdm_dither_sed_y1)
{
	U_GDM_DITHER_SED_Y1 GDM_DITHER_SED_Y1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedY1 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_Y1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y1.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_Y1.bits.gdm_dither_sed_y1 = gdm_dither_sed_y1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y1.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_Y1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedU1(HI_U32 u32Data, HI_U32 gdm_dither_sed_u1)
{
	U_GDM_DITHER_SED_U1 GDM_DITHER_SED_U1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedU1 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_U1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U1.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_U1.bits.gdm_dither_sed_u1 = gdm_dither_sed_u1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U1.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_U1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedV1(HI_U32 u32Data, HI_U32 gdm_dither_sed_v1)
{
	U_GDM_DITHER_SED_V1 GDM_DITHER_SED_V1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedV1 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_V1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V1.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_V1.bits.gdm_dither_sed_v1 = gdm_dither_sed_v1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V1.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_V1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedW1(HI_U32 u32Data, HI_U32 gdm_dither_sed_w1)
{
	U_GDM_DITHER_SED_W1 GDM_DITHER_SED_W1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedW1 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_W1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W1.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_W1.bits.gdm_dither_sed_w1 = gdm_dither_sed_w1;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W1.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_W1.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedY2(HI_U32 u32Data, HI_U32 gdm_dither_sed_y2)
{
	U_GDM_DITHER_SED_Y2 GDM_DITHER_SED_Y2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedY2 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_Y2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y2.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_Y2.bits.gdm_dither_sed_y2 = gdm_dither_sed_y2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y2.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_Y2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedU2(HI_U32 u32Data, HI_U32 gdm_dither_sed_u2)
{
	U_GDM_DITHER_SED_U2 GDM_DITHER_SED_U2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedU2 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_U2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U2.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_U2.bits.gdm_dither_sed_u2 = gdm_dither_sed_u2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U2.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_U2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedV2(HI_U32 u32Data, HI_U32 gdm_dither_sed_v2)
{
	U_GDM_DITHER_SED_V2 GDM_DITHER_SED_V2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedV2 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_V2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V2.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_V2.bits.gdm_dither_sed_v2 = gdm_dither_sed_v2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V2.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_V2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedW2(HI_U32 u32Data, HI_U32 gdm_dither_sed_w2)
{
	U_GDM_DITHER_SED_W2 GDM_DITHER_SED_W2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedW2 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_W2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W2.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_W2.bits.gdm_dither_sed_w2 = gdm_dither_sed_w2;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W2.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_W2.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedY3(HI_U32 u32Data, HI_U32 gdm_dither_sed_y3)
{
	U_GDM_DITHER_SED_Y3 GDM_DITHER_SED_Y3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedY3 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_Y3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y3.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_Y3.bits.gdm_dither_sed_y3 = gdm_dither_sed_y3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_Y3.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_Y3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedU3(HI_U32 u32Data, HI_U32 gdm_dither_sed_u3)
{
	U_GDM_DITHER_SED_U3 GDM_DITHER_SED_U3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedU3 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_U3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U3.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_U3.bits.gdm_dither_sed_u3 = gdm_dither_sed_u3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_U3.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_U3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedV3(HI_U32 u32Data, HI_U32 gdm_dither_sed_v3)
{
	U_GDM_DITHER_SED_V3 GDM_DITHER_SED_V3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedV3 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_V3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V3.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_V3.bits.gdm_dither_sed_v3 = gdm_dither_sed_v3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_V3.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_V3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherSedW3(HI_U32 u32Data, HI_U32 gdm_dither_sed_w3)
{
	U_GDM_DITHER_SED_W3 GDM_DITHER_SED_W3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherSedW3 Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_SED_W3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W3.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_SED_W3.bits.gdm_dither_sed_w3 = gdm_dither_sed_w3;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_SED_W3.u32) + u32Data * GP_OFFSET),GDM_DITHER_SED_W3.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherThrMax(HI_U32 u32Data, HI_U32 gdm_dither_thr_max)
{
	U_GDM_DITHER_THR GDM_DITHER_THR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherThrMax Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_THR.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_THR.bits.gdm_dither_thr_max = gdm_dither_thr_max;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_THR.u32) + u32Data * GP_OFFSET),GDM_DITHER_THR.u32);
	
	return ;
}


HI_VOID VDP_GP_SetGdmGdmDitherThrMin(HI_U32 u32Data, HI_U32 gdm_dither_thr_min)
{
	U_GDM_DITHER_THR GDM_DITHER_THR;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_GP_SetGdmGdmDitherThrMin Select Wrong Layer ID\n");
	}
	
	GDM_DITHER_THR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_DITHER_THR.u32) + u32Data * GP_OFFSET));
	GDM_DITHER_THR.bits.gdm_dither_thr_min = gdm_dither_thr_min;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->GDM_DITHER_THR.u32) + u32Data * GP_OFFSET),GDM_DITHER_THR.u32);
	
	return ;
}




