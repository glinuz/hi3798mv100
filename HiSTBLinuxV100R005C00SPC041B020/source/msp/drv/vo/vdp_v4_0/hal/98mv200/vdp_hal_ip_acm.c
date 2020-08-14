
#include "vdp_hal_ip_acm.h"
#include "vdp_hal_comm.h"


extern S_VDP_REGS_TYPE *pVdpReg;

HI_VOID VDP_VID_SetAcmAcm3En(HI_U32 u32Data, HI_U32 acm3_en)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcm3En Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm3_en = acm3_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcm2En(HI_U32 u32Data, HI_U32 acm2_en)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcm2En Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm2_en = acm2_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcm1En(HI_U32 u32Data, HI_U32 acm1_en)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcm1En Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm1_en = acm1_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcm0En(HI_U32 u32Data, HI_U32 acm0_en)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcm0En Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm0_en = acm0_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmSecBlk0(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmSecBlk0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_sec_blk = acm_sec_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmFirBlk0(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmFirBlk0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_fir_blk = acm_fir_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDUOff0(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDUOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_d_u_off = acm_d_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCUOff0(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCUOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_c_u_off = acm_c_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBUOff0(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBUOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_b_u_off = acm_b_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAUOff0(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
	U_VHDACM0 VHDACM0;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAUOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET));
	VHDACM0.bits.acm_a_u_off = acm_a_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM0.u32) + u32Data * VID_OFFSET),VHDACM0.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmTestEn(HI_U32 u32Data, HI_U32 acm_test_en)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmTestEn Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_test_en = acm_test_en;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetAcmAcmDbgMode(HI_U32 u32Data, HI_U32 acm_dbg_mode)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmTestEn Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_dbg_mode = acm_dbg_mode;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetAcmAcmProtMode(HI_U32 u32Data, HI_U32 bw_pro_mode)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmTestEn Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.bw_pro_mode = bw_pro_mode ;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}

HI_VOID VDP_VID_SetAcmAcmDVOff0(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDVOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_d_v_off = acm_d_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCVOff0(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCVOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_c_v_off = acm_c_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBVOff0(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBVOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_b_v_off = acm_b_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAVOff0(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
	U_VHDACM1 VHDACM1;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAVOff0 Select Wrong Layer ID\n");
	}
	
	VHDACM1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET));
	VHDACM1.bits.acm_a_v_off = acm_a_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM1.u32) + u32Data * VID_OFFSET),VHDACM1.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmSecBlk1(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmSecBlk1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_sec_blk = acm_sec_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmFirBlk1(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmFirBlk1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_fir_blk = acm_fir_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDUOff1(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDUOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_d_u_off = acm_d_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCUOff1(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCUOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_c_u_off = acm_c_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBUOff1(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBUOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_b_u_off = acm_b_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAUOff1(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
	U_VHDACM2 VHDACM2;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAUOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET));
	VHDACM2.bits.acm_a_u_off = acm_a_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM2.u32) + u32Data * VID_OFFSET),VHDACM2.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDVOff1(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
	U_VHDACM3 VHDACM3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDVOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET));
	VHDACM3.bits.acm_d_v_off = acm_d_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET),VHDACM3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCVOff1(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
	U_VHDACM3 VHDACM3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCVOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET));
	VHDACM3.bits.acm_c_v_off = acm_c_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET),VHDACM3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBVOff1(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
	U_VHDACM3 VHDACM3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBVOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET));
	VHDACM3.bits.acm_b_v_off = acm_b_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET),VHDACM3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAVOff1(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
	U_VHDACM3 VHDACM3;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAVOff1 Select Wrong Layer ID\n");
	}
	
	VHDACM3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET));
	VHDACM3.bits.acm_a_v_off = acm_a_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM3.u32) + u32Data * VID_OFFSET),VHDACM3.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmSecBlk2(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmSecBlk2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_sec_blk = acm_sec_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmFirBlk2(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmFirBlk2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_fir_blk = acm_fir_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDUOff2(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDUOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_d_u_off = acm_d_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCUOff2(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCUOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_c_u_off = acm_c_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBUOff2(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBUOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_b_u_off = acm_b_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAUOff2(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
	U_VHDACM4 VHDACM4;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAUOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM4.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET));
	VHDACM4.bits.acm_a_u_off = acm_a_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM4.u32) + u32Data * VID_OFFSET),VHDACM4.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDVOff2(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
	U_VHDACM5 VHDACM5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDVOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET));
	VHDACM5.bits.acm_d_v_off = acm_d_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET),VHDACM5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCVOff2(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
	U_VHDACM5 VHDACM5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCVOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET));
	VHDACM5.bits.acm_c_v_off = acm_c_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET),VHDACM5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBVOff2(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
	U_VHDACM5 VHDACM5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBVOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET));
	VHDACM5.bits.acm_b_v_off = acm_b_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET),VHDACM5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAVOff2(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
	U_VHDACM5 VHDACM5;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAVOff2 Select Wrong Layer ID\n");
	}
	
	VHDACM5.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET));
	VHDACM5.bits.acm_a_v_off = acm_a_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM5.u32) + u32Data * VID_OFFSET),VHDACM5.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmSecBlk3(HI_U32 u32Data, HI_U32 acm_sec_blk)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmSecBlk3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_sec_blk = acm_sec_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmFirBlk3(HI_U32 u32Data, HI_U32 acm_fir_blk)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmFirBlk3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_fir_blk = acm_fir_blk;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDUOff3(HI_U32 u32Data, HI_U32 acm_d_u_off)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDUOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_d_u_off = acm_d_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCUOff3(HI_U32 u32Data, HI_U32 acm_c_u_off)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCUOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_c_u_off = acm_c_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBUOff3(HI_U32 u32Data, HI_U32 acm_b_u_off)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBUOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_b_u_off = acm_b_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAUOff3(HI_U32 u32Data, HI_U32 acm_a_u_off)
{
	U_VHDACM6 VHDACM6;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAUOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM6.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET));
	VHDACM6.bits.acm_a_u_off = acm_a_u_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM6.u32) + u32Data * VID_OFFSET),VHDACM6.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmDVOff3(HI_U32 u32Data, HI_U32 acm_d_v_off)
{
	U_VHDACM7 VHDACM7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmDVOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET));
	VHDACM7.bits.acm_d_v_off = acm_d_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET),VHDACM7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmCVOff3(HI_U32 u32Data, HI_U32 acm_c_v_off)
{
	U_VHDACM7 VHDACM7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmCVOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET));
	VHDACM7.bits.acm_c_v_off = acm_c_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET),VHDACM7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmBVOff3(HI_U32 u32Data, HI_U32 acm_b_v_off)
{
	U_VHDACM7 VHDACM7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmBVOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET));
	VHDACM7.bits.acm_b_v_off = acm_b_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET),VHDACM7.u32);
	
	return ;
}


HI_VOID VDP_VID_SetAcmAcmAVOff3(HI_U32 u32Data, HI_U32 acm_a_v_off)
{
	U_VHDACM7 VHDACM7;
	
	if(u32Data >= VID_MAX)
	{
		VDP_PRINT("Error, VDP_VID_SetAcmAcmAVOff3 Select Wrong Layer ID\n");
	}
	
	VHDACM7.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET));
	VHDACM7.bits.acm_a_v_off = acm_a_v_off;
	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VHDACM7.u32) + u32Data * VID_OFFSET),VHDACM7.u32);
	
	return ;
}




