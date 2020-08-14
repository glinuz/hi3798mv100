#include <linux/io.h>
#include <linux/delay.h>
#include "vpss_cbb_reg.h"
#include "vpss_common.h"
#include "vpss_reg_struct.h"

#if 1

#define VPSS_RegRead(addr) VPSS_REG_RegRead(addr)
#define VPSS_RegWrite(addr, value) VPSS_REG_RegWrite((addr), (value))
#define VPSS_WRITE_REG_SAFE(reg, val, errlog) \
{ \
	HI_S32 s32Timeout = 0; \
	reg = val; \
	while (val != reg) \
	{ \
		if (s32Timeout++ > 10) \
		{ \
			VPSS_FATAL(errlog); \
			return HI_FAILURE; \
		} \
		else \
		{ \
			udelay(1); \
		} \
	} \
}

HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b)
{
	volatile HI_U32 *addr = a;

	//printk("W [%08x] = %08x(%u,%u).\n", (unsigned int)addr, b, ((b&0xFFFF0000)>>16), (b&0xFFFF));
    *addr = b;
    return HI_SUCCESS;
}

HI_U32 VPSS_REG_RegRead(volatile HI_U32 *a)
{
	volatile HI_U32 *addr = a;

   //printk("R [%08x] = %08x(%u,%u).\n", (unsigned int)addr, (*(addr)), (*(addr)));
   return (*(addr));
}

HI_S32 VPSS_REG_ResetAppReg(S_VPSS_REGS_TYPE *pstVpssRegs, VPSS_REG_S *pstPqReg)
{
    memset((HI_U8 *)pstVpssRegs, 0, sizeof(S_VPSS_REGS_TYPE));

    if (pstPqReg)
    {
        VPSS_SAFE_MEMCPY((HI_U8 *)pstVpssRegs, (HI_U8 *)pstPqReg, sizeof(S_VPSS_REGS_TYPE));
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_DRV_SetClockEn(HI_BOOL bClkEn)
{
	if (bClkEn)
	{
		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_srst_req, 1, "VPSS reset timeout!\n");
		udelay(1);

		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_cken, 1, "VPSS clock enable timeout!\n");
		udelay(1);

		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_srst_req, 0, "VPSS unreset timeout!\n");
		udelay(1);
	}
	else
	{
		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_srst_req, 1, "VPSS reset timeout!\n");
		udelay(1);

		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_cken, 0, "VPSS disable clock timeout!\n");
	}

    return HI_SUCCESS;
}


HI_S32 VPSS_DRV_SetTimeOut(S_VPSS_REGS_TYPE *pstVpssRegs,HI_U32 u32Data)
{
	HI_U32 VPSS_TIMEOUT;
	HI_S32 s32RetryCnt;

	VPSS_TIMEOUT = 0;
	s32RetryCnt = 0;

	while (VPSS_TIMEOUT != u32Data)
	{
		VPSS_TIMEOUT = u32Data;
		VPSS_REG_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TIMEOUT.u32), VPSS_TIMEOUT);
		VPSS_TIMEOUT = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TIMEOUT.u32));
		s32RetryCnt++;

		if (VPSS_TIMEOUT != u32Data)
		{
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, u32Data, VPSS_TIMEOUT);
		}
	}

	return HI_SUCCESS;
}


HI_S32 VPSS_DRV_GetRawIntStatus( S_VPSS_REGS_TYPE *pstVpssRegs )
{
	U_VPSS_RAWINT VPSS_RAWINT;

	VPSS_RAWINT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RAWINT.u32));

	return VPSS_RAWINT.u32;
}

HI_S32 VPSS_DRV_GetIntStatus( S_VPSS_REGS_TYPE *pstVpssRegs )
{
	U_VPSS_RAWINT VPSS_INTSTATE;

	VPSS_INTSTATE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_INTSTATE.u32));

	return VPSS_INTSTATE.u32;
}


HI_VOID VPSS_DRV_ClrInt( S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 u32ClrInt )
{
	U_VPSS_INTCLR VPSS_INTCLR;

	VPSS_INTCLR.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_INTCLR.u32));
	VPSS_INTCLR.u32 = u32ClrInt;
	VPSS_REG_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_INTCLR.u32), VPSS_INTCLR.u32);
}

HI_VOID VPSS_DRV_SetIntMask( S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 u32Mask )
{
	U_VPSS_INTMASK VPSS_INTMASK;
	HI_S32 s32RetryCnt;

	VPSS_INTMASK.u32 = 0;
	s32RetryCnt = 0;

	while (VPSS_INTMASK.u32 != u32Mask)
	{
		VPSS_INTMASK.u32 = u32Mask;

		VPSS_REG_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_INTMASK.u32), VPSS_INTMASK.u32);
		VPSS_INTMASK.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_INTMASK.u32));

		s32RetryCnt++;

		if (VPSS_INTMASK.u32 != u32Mask)
		{
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, u32Mask, VPSS_INTMASK.u32);
		}
	}
}

//addr=0x0
HI_VOID VPSS_DRV_Set_VPSS_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield ,HI_U32 bfield_mode ,HI_U32 bfield_first ,HI_U32 img_pro_mode ,HI_U32 igbm_en ,HI_U32 ifmd_en ,HI_U32 prot ,HI_U32 rotate_en ,HI_U32 four_pix_en ,HI_U32 rfr_en ,HI_U32 tnr_en ,HI_U32 snr_en ,HI_U32 ref_vc1_en ,HI_U32 cur_vc1_en ,HI_U32 blk_det_en ,HI_U32 nx1_vc1_en ,HI_U32 rgme_en ,HI_U32 nx2_vc1_en ,HI_U32 mcdi_en ,HI_U32 dei_en ,HI_U32 chk_sum_en ,HI_U32 vhd0_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.bfield       = bfield       ;
     VPSS_CTRL.bits.bfield_mode  = bfield_mode  ;
     VPSS_CTRL.bits.bfield_first = bfield_first ;
     VPSS_CTRL.bits.img_pro_mode = img_pro_mode ;
     VPSS_CTRL.bits.igbm_en      = igbm_en      ;
     VPSS_CTRL.bits.ifmd_en      = ifmd_en      ;
     VPSS_CTRL.bits.prot         = prot         ;
     VPSS_CTRL.bits.rotate_en    = rotate_en    ;
     VPSS_CTRL.bits.four_pix_en  = four_pix_en  ;
     VPSS_CTRL.bits.rfr_en       = rfr_en       ;
     VPSS_CTRL.bits.tnr_en       = tnr_en       ;
     VPSS_CTRL.bits.snr_en       = snr_en       ;
     VPSS_CTRL.bits.ref_vc1_en   = ref_vc1_en   ;
     VPSS_CTRL.bits.cur_vc1_en   = cur_vc1_en   ;
     VPSS_CTRL.bits.blk_det_en   = blk_det_en   ;
     VPSS_CTRL.bits.nx1_vc1_en   = nx1_vc1_en   ;
     VPSS_CTRL.bits.rgme_en      = rgme_en      ;
     VPSS_CTRL.bits.nx2_vc1_en   = nx2_vc1_en   ;
     VPSS_CTRL.bits.mcdi_en      = mcdi_en      ;
     VPSS_CTRL.bits.dei_en       = dei_en       ;
     VPSS_CTRL.bits.chk_sum_en   = chk_sum_en   ;
     VPSS_CTRL.bits.vhd0_en      = vhd0_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
//addr=0x4
HI_VOID VPSS_DRV_Set_VPSS_CTRL2(S_VPSS_REGS_TYPE *pstVpssRegs )
{
     U_VPSS_CTRL2 VPSS_CTRL2;

     VPSS_CTRL2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL2.u32));

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL2.u32), VPSS_CTRL2.u32);

     return;
}
//addr=0x8
HI_VOID VPSS_DRV_Set_VPSS_CTRL3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_en ,HI_U32 vhd0_crop_en ,HI_U32 vhd0_pre_vfir_en ,HI_U32 zme_vhd0_en )
{
     U_VPSS_CTRL3 VPSS_CTRL3;

     VPSS_CTRL3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32));
     VPSS_CTRL3.bits.vhd0_lba_en      = vhd0_lba_en      ;
     VPSS_CTRL3.bits.vhd0_crop_en     = vhd0_crop_en     ;
     VPSS_CTRL3.bits.vhd0_pre_vfir_en = vhd0_pre_vfir_en ;
     VPSS_CTRL3.bits.zme_vhd0_en      = zme_vhd0_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32), VPSS_CTRL3.u32);

     return;
}

#if 1
//addr=0x0
HI_VOID VPSS_DRV_Set_bfield(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.bfield       = bfield       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_bfield_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield_mode )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.bfield_mode  = bfield_mode  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_bfield_first(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield_first )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.bfield_first = !bfield_first ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_img_pro_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 img_pro_mode )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.img_pro_mode = img_pro_mode ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_igbm_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 igbm_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.igbm_en      = igbm_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_ifmd_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ifmd_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.ifmd_en      = ifmd_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_prot(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prot )
{
	U_VPSS_CTRL VPSS_CTRL;

	VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));

	//0: security; 2: not security
	if (prot)
	{
		VPSS_CTRL.bits.prot = 0;
	}
	else
	{
		VPSS_CTRL.bits.prot = 2;
	}

	VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

	return;
}
HI_VOID VPSS_DRV_Set_rotate_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rotate_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.rotate_en    = rotate_en    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_four_pix_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 four_pix_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.four_pix_en  = four_pix_en  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_rfr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfr_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.rfr_en       = rfr_en       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_tnr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 tnr_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.tnr_en       = tnr_en       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_snr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_en )
{
     U_VPSS_CTRL VPSS_CTRL;
     U_VPSS_IN_CTRL VPSS_IN_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.snr_en       = snr_en       ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     if (snr_en)
     {
         VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));
         VPSS_IN_CTRL.bits.snr_mad_upfir_en = 0x1 ;
         VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);
     }

     return;
}
HI_VOID VPSS_DRV_Set_ref_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_vc1_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.ref_vc1_en   = ref_vc1_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_cur_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_vc1_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.cur_vc1_en   = cur_vc1_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_blk_det_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blk_det_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.blk_det_en   = blk_det_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_nx1_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx1_vc1_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.nx1_vc1_en   = nx1_vc1_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_rgme_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgme_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.rgme_en      = rgme_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_nx2_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_vc1_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.nx2_vc1_en   = nx2_vc1_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_mcdi_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcdi_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.mcdi_en      = mcdi_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_dei_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dei_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.dei_en       = dei_en       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_chk_sum_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chk_sum_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.chk_sum_en   = chk_sum_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
HI_VOID VPSS_DRV_Set_vhd0_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_en )
{
     U_VPSS_CTRL VPSS_CTRL;

     VPSS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32));
     VPSS_CTRL.bits.vhd0_en      = vhd0_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL.u32), VPSS_CTRL.u32);

     return;
}
//addr=0x4
//addr=0x8
HI_VOID VPSS_DRV_Set_vhd0_lba_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_en )
{
     U_VPSS_CTRL3 VPSS_CTRL3;

     VPSS_CTRL3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32));
     VPSS_CTRL3.bits.vhd0_lba_en      = vhd0_lba_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32), VPSS_CTRL3.u32);

     return;
}
HI_VOID VPSS_DRV_Set_vhd0_crop_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_en )
{
     U_VPSS_CTRL3 VPSS_CTRL3;

     VPSS_CTRL3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32));
     VPSS_CTRL3.bits.vhd0_crop_en     = vhd0_crop_en     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32), VPSS_CTRL3.u32);

     return;
}
HI_VOID VPSS_DRV_Set_vhd0_pre_vfir_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pre_vfir_en )
{
     U_VPSS_CTRL3 VPSS_CTRL3;

     VPSS_CTRL3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32));
     VPSS_CTRL3.bits.vhd0_pre_vfir_en = vhd0_pre_vfir_en ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32), VPSS_CTRL3.u32);

     return;
}
HI_VOID VPSS_DRV_Set_zme_vhd0_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 zme_vhd0_en )
{
     U_VPSS_CTRL3 VPSS_CTRL3;

     VPSS_CTRL3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32));
     VPSS_CTRL3.bits.zme_vhd0_en      = zme_vhd0_en      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CTRL3.u32), VPSS_CTRL3.u32);

     return;
}
#endif


//addr=0xc
HI_VOID VPSS_DRV_Set_VPSS_CMD_CNT_CHN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmd_cnt_rd_chn ,HI_U32 cmdlen_cnt_rd_chn ,HI_U32 cmd_cnt_wr_chn ,HI_U32 cmdlen_cnt_wr_chn )
{
     U_VPSS_CMD_CNT_CHN VPSS_CMD_CNT_CHN;

     VPSS_CMD_CNT_CHN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CMD_CNT_CHN.u32));
     VPSS_CMD_CNT_CHN.bits.cmd_cnt_rd_chn    = cmd_cnt_rd_chn    ;
     VPSS_CMD_CNT_CHN.bits.cmdlen_cnt_rd_chn = cmdlen_cnt_rd_chn ;
     VPSS_CMD_CNT_CHN.bits.cmd_cnt_wr_chn    = cmd_cnt_wr_chn    ;
     VPSS_CMD_CNT_CHN.bits.cmdlen_cnt_wr_chn = cmdlen_cnt_wr_chn ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CMD_CNT_CHN.u32), VPSS_CMD_CNT_CHN.u32);

     return;
}
//addr=0x10
HI_VOID VPSS_DRV_Set_VPSS_RCH_BYPASS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rch_transr_bypass ,HI_U32 rch_nx2c_tile_2bit_bypass ,HI_U32 rch_nx2y_tile_2bit_bypass ,HI_U32 rch_nx2c_head_bypass ,HI_U32 rch_nx2y_head_bypass ,HI_U32 rch_tunl_bypass ,HI_U32 rch_reec_bypass ,HI_U32 rch_reey_bypass ,HI_U32 rch_rgmv_cur_bypass ,HI_U32 rch_rgmv_nx1_bypass ,HI_U32 rch_prjv_cur_bypass ,HI_U32 rch_blkmv_ref_bypass ,HI_U32 rch_blkmv_cur_bypass ,HI_U32 rch_blkmv_nx1_bypass ,HI_U32 rch_prjh_cur_bypass ,HI_U32 rch_rst_bypass ,HI_U32 rch_srmd_bypass ,HI_U32 rch_trmd_bypass ,HI_U32 rch_nx2c_bypass ,HI_U32 rch_nx2y_bypass ,HI_U32 rch_nx1c_bypass ,HI_U32 rch_nx1y_bypass ,HI_U32 rch_refc_bypass ,HI_U32 rch_refy_bypass ,HI_U32 rch_curc_bypass ,HI_U32 rch_cury_bypass )
{
     U_VPSS_RCH_BYPASS VPSS_RCH_BYPASS;

     VPSS_RCH_BYPASS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RCH_BYPASS.u32));
     VPSS_RCH_BYPASS.bits.rch_transr_bypass         = rch_transr_bypass         ;
     VPSS_RCH_BYPASS.bits.rch_nx2c_tile_2bit_bypass = rch_nx2c_tile_2bit_bypass ;
     VPSS_RCH_BYPASS.bits.rch_nx2y_tile_2bit_bypass = rch_nx2y_tile_2bit_bypass ;
     VPSS_RCH_BYPASS.bits.rch_nx2c_head_bypass      = rch_nx2c_head_bypass      ;
     VPSS_RCH_BYPASS.bits.rch_nx2y_head_bypass      = rch_nx2y_head_bypass      ;
     VPSS_RCH_BYPASS.bits.rch_tunl_bypass           = rch_tunl_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_reec_bypass           = rch_reec_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_reey_bypass           = rch_reey_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_rgmv_cur_bypass       = rch_rgmv_cur_bypass       ;
     VPSS_RCH_BYPASS.bits.rch_rgmv_nx1_bypass       = rch_rgmv_nx1_bypass       ;
     VPSS_RCH_BYPASS.bits.rch_prjv_cur_bypass       = rch_prjv_cur_bypass       ;
     VPSS_RCH_BYPASS.bits.rch_blkmv_ref_bypass      = rch_blkmv_ref_bypass      ;
     VPSS_RCH_BYPASS.bits.rch_blkmv_cur_bypass      = rch_blkmv_cur_bypass      ;
     VPSS_RCH_BYPASS.bits.rch_blkmv_nx1_bypass      = rch_blkmv_nx1_bypass      ;
     VPSS_RCH_BYPASS.bits.rch_prjh_cur_bypass       = rch_prjh_cur_bypass       ;
     VPSS_RCH_BYPASS.bits.rch_rst_bypass            = rch_rst_bypass            ;
     VPSS_RCH_BYPASS.bits.rch_srmd_bypass           = rch_srmd_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_trmd_bypass           = rch_trmd_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_nx2c_bypass           = rch_nx2c_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_nx2y_bypass           = rch_nx2y_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_nx1c_bypass           = rch_nx1c_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_nx1y_bypass           = rch_nx1y_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_refc_bypass           = rch_refc_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_refy_bypass           = rch_refy_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_curc_bypass           = rch_curc_bypass           ;
     VPSS_RCH_BYPASS.bits.rch_cury_bypass           = rch_cury_bypass           ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RCH_BYPASS.u32), VPSS_RCH_BYPASS.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_RCH_BYPASS_FLAG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32Flag)
{
     U_VPSS_RCH_BYPASS VPSS_RCH_BYPASS;

     VPSS_RCH_BYPASS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RCH_BYPASS.u32));
     //VPSS_RCH_BYPASS.bits.u32         = u32Flag         ;
     VPSS_RCH_BYPASS.u32 = u32Flag         ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RCH_BYPASS.u32), VPSS_RCH_BYPASS.u32);

     return;
}

//addr=0x14
HI_VOID VPSS_DRV_Set_VPSS_WCH_BYPASS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 wch_tunl_bypass ,HI_U32 wch_transw_bypass ,HI_U32 wch_cuec_bypass ,HI_U32 wch_cuey_bypass ,HI_U32 wch_rgmvnx2_bypass ,HI_U32 wch_prjvnx2_bypass ,HI_U32 wch_blkmvnx2_bypass ,HI_U32 wch_prjhnx2_bypass ,HI_U32 wch_stt_bypass ,HI_U32 wch_twmd_bypass ,HI_U32 wch_wst_bypass ,HI_U32 wch_rfrc_bypass ,HI_U32 wch_rfry_bypass ,HI_U32 wch_vhd0c_bypass ,HI_U32 wch_vhd0y_bypass )
{
     U_VPSS_WCH_BYPASS VPSS_WCH_BYPASS;

     VPSS_WCH_BYPASS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_WCH_BYPASS.u32));
     VPSS_WCH_BYPASS.bits.wch_tunl_bypass     = wch_tunl_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_transw_bypass   = wch_transw_bypass   ;
     VPSS_WCH_BYPASS.bits.wch_cuec_bypass     = wch_cuec_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_cuey_bypass     = wch_cuey_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_rgmvnx2_bypass  = wch_rgmvnx2_bypass  ;
     VPSS_WCH_BYPASS.bits.wch_prjvnx2_bypass  = wch_prjvnx2_bypass  ;
     VPSS_WCH_BYPASS.bits.wch_blkmvnx2_bypass = wch_blkmvnx2_bypass ;
     VPSS_WCH_BYPASS.bits.wch_prjhnx2_bypass  = wch_prjhnx2_bypass  ;
     VPSS_WCH_BYPASS.bits.wch_stt_bypass      = wch_stt_bypass      ;
     VPSS_WCH_BYPASS.bits.wch_twmd_bypass     = wch_twmd_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_wst_bypass      = wch_wst_bypass      ;
     VPSS_WCH_BYPASS.bits.wch_rfrc_bypass     = wch_rfrc_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_rfry_bypass     = wch_rfry_bypass     ;
     VPSS_WCH_BYPASS.bits.wch_vhd0c_bypass    = wch_vhd0c_bypass    ;
     VPSS_WCH_BYPASS.bits.wch_vhd0y_bypass    = wch_vhd0y_bypass    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_WCH_BYPASS.u32), VPSS_WCH_BYPASS.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_WCH_BYPASS_FLAG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32Flag)
{
     U_VPSS_RCH_BYPASS VPSS_RCH_BYPASS;

     VPSS_RCH_BYPASS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_WCH_BYPASS.u32));
     //VPSS_RCH_BYPASS.bits.u32         = u32Flag         ;
     VPSS_RCH_BYPASS.u32 = u32Flag         ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_WCH_BYPASS.u32), VPSS_RCH_BYPASS.u32);

     return;
}

//addr=0x18
HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZMEORESO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_zme_oh ,HI_U32 vhd0_zme_ow )
{
     U_VPSS_VHD0_ZMEORESO VPSS_VHD0_ZMEORESO;

     VPSS_VHD0_ZMEORESO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZMEORESO.u32));
     VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh = vhd0_zme_oh -1;
     VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow = vhd0_zme_ow -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZMEORESO.u32), VPSS_VHD0_ZMEORESO.u32);

     return;
}
//addr=0x1c
HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_LHADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_lh )
{
     U_VPSS_VHD0_ZME_LHADDR VPSS_VHD0_ZME_LHADDR;

     VPSS_VHD0_ZME_LHADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_LHADDR.u32));
     VPSS_VHD0_ZME_LHADDR.bits.vhd0_scl_lh = vhd0_scl_lh ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_LHADDR.u32), VPSS_VHD0_ZME_LHADDR.u32);

     return;
}
//addr=0x20
HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_LVADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_lv )
{
     U_VPSS_VHD0_ZME_LVADDR VPSS_VHD0_ZME_LVADDR;

     VPSS_VHD0_ZME_LVADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_LVADDR.u32));
     VPSS_VHD0_ZME_LVADDR.bits.vhd0_scl_lv = vhd0_scl_lv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_LVADDR.u32), VPSS_VHD0_ZME_LVADDR.u32);

     return;
}
//addr=0x24
HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_CHADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_ch )
{
     U_VPSS_VHD0_ZME_CHADDR VPSS_VHD0_ZME_CHADDR;

     VPSS_VHD0_ZME_CHADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_CHADDR.u32));
     VPSS_VHD0_ZME_CHADDR.bits.vhd0_scl_ch = vhd0_scl_ch ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_CHADDR.u32), VPSS_VHD0_ZME_CHADDR.u32);

     return;
}
//addr=0x28
HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_CVADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_cv )
{
     U_VPSS_VHD0_ZME_CVADDR VPSS_VHD0_ZME_CVADDR;

     VPSS_VHD0_ZME_CVADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_CVADDR.u32));
     VPSS_VHD0_ZME_CVADDR.bits.vhd0_scl_cv = vhd0_scl_cv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_ZME_CVADDR.u32), VPSS_VHD0_ZME_CVADDR.u32);

     return;
}
//addr=0x2c
HI_VOID VPSS_DRV_Set_VPSS_TNR_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 tnr_cfg_addr )
{
     U_VPSS_TNR_ADDR VPSS_TNR_ADDR;

     VPSS_TNR_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_ADDR.u32));
     VPSS_TNR_ADDR.bits.tnr_cfg_addr = tnr_cfg_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_ADDR.u32), VPSS_TNR_ADDR.u32);

     return;
}
//addr=0x34
HI_VOID VPSS_DRV_Set_VPSS_SNR_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_cfg_addr )
{
     U_VPSS_SNR_ADDR VPSS_SNR_ADDR;

     VPSS_SNR_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_ADDR.u32));
     VPSS_SNR_ADDR.bits.snr_cfg_addr = snr_cfg_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_ADDR.u32), VPSS_SNR_ADDR.u32);

     return;
}
//addr=0x3c
HI_VOID VPSS_DRV_Set_VPSS_ZME_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 zme_cfg_addr )
{
     U_VPSS_ZME_ADDR VPSS_ZME_ADDR;

     VPSS_ZME_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_ZME_ADDR.u32));
     VPSS_ZME_ADDR.bits.zme_cfg_addr = zme_cfg_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_ZME_ADDR.u32), VPSS_ZME_ADDR.u32);

     return;
}
//addr=0x40
HI_VOID VPSS_DRV_Set_VPSS_DEI_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dei_cfg_addr )
{
     U_VPSS_DEI_ADDR VPSS_DEI_ADDR;

     VPSS_DEI_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DEI_ADDR.u32));
     VPSS_DEI_ADDR.bits.dei_cfg_addr = dei_cfg_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DEI_ADDR.u32), VPSS_DEI_ADDR.u32);

     return;
}
//addr=0x2f8
HI_VOID VPSS_DRV_Set_VPSS_NODEID(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nodeid )
{
     U_VPSS_NODEID VPSS_NODEID;

     VPSS_NODEID.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NODEID.u32));
     VPSS_NODEID.bits.nodeid = nodeid ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NODEID.u32), VPSS_NODEID.u32);

     return;
}

//addr=0x2fc
HI_VOID VPSS_DRV_Set_VPSS_PNEXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 *pu32PhyAddr)
{
	U_VPSS_PNEXT VPSS_PNEXT;
	HI_S32 s32RetryCnt;

	s32RetryCnt = 0;
	VPSS_PNEXT.u32 = 0;

	while ((VPSS_PNEXT.u32 != (HI_U32)(long)pu32PhyAddr) && (s32RetryCnt < 10))
	{
		VPSS_PNEXT.u32 = VPSS_RegRead((volatile HI_U32*)((unsigned long)&(pstVpssRegs->VPSS_PNEXT.u32)));
		VPSS_PNEXT.bits.p_next = (HI_U32)((unsigned long)pu32PhyAddr);
		VPSS_REG_RegWrite((volatile HI_U32*)((unsigned long)&(pstVpssRegs->VPSS_PNEXT.u32)), VPSS_PNEXT.u32);
		VPSS_PNEXT.u32 = VPSS_RegRead((volatile HI_U32*)((unsigned long)&(pstVpssRegs->VPSS_PNEXT.u32)));
		s32RetryCnt++;

		if (VPSS_PNEXT.u32 != (HI_U32)(long)pu32PhyAddr)
		{
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, pu32PhyAddr, VPSS_PNEXT.u32);
		}
	}

    if (10 <= s32RetryCnt)
    {
        VPSS_FATAL("VPSS_DRV_Set_VPSS_PNEXT fail\n");
    }

	return;
}

//addr=0x300
HI_VOID VPSS_DRV_Set_VPSS_START(S_VPSS_REGS_TYPE *pstVpssRegs)
{
    U_VPSS_START VPSS_START;

    VPSS_START.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_START.u32));
    /* make sure all config data have been written to DDR before VPSS logic starts.
	   Otherwise VPSS may get a accidental TIMEOUT crash.
	*/

	mb();

	VPSS_START.bits.start = 0x1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_START.u32), VPSS_START.u32);

    return;
}


//addr=0x314
HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 burst_len_cfg ,HI_U32 ck_gt_en_calc ,HI_U32 ck_gt_en ,HI_U32 init_timer ,HI_U32 woutstanding ,HI_U32 routstanding )
{
     U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;

     VPSS_MISCELLANEOUS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32));
     VPSS_MISCELLANEOUS.bits.burst_len_cfg = burst_len_cfg ;
     VPSS_MISCELLANEOUS.bits.ck_gt_en_calc = ck_gt_en_calc ;
     VPSS_MISCELLANEOUS.bits.ck_gt_en      = ck_gt_en      ;
     VPSS_MISCELLANEOUS.bits.init_timer    = init_timer    ;
     VPSS_MISCELLANEOUS.bits.woutstanding  = woutstanding  ;
     VPSS_MISCELLANEOUS.bits.routstanding  = routstanding  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS_OUTSTANDING(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 woutstanding ,HI_U32 routstanding )
{
     U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;

     VPSS_MISCELLANEOUS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32));

     VPSS_MISCELLANEOUS.bits.woutstanding  = woutstanding  ;
     VPSS_MISCELLANEOUS.bits.routstanding  = routstanding  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS_BURSTLENGTH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32BurstLength)
{
     U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;

     VPSS_MISCELLANEOUS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32));
     VPSS_MISCELLANEOUS.bits.burst_len_cfg  = u32BurstLength  ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32);

     return;
}

//addr=0x318
HI_VOID VPSS_DRV_Set_VPSS_MACCFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mac_ch_prio )
{
     U_VPSS_MACCFG VPSS_MACCFG;

     VPSS_MACCFG.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MACCFG.u32));
     VPSS_MACCFG.bits.mac_ch_prio = mac_ch_prio ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MACCFG.u32), VPSS_MACCFG.u32);

     return;
}
//addr=0x320
HI_VOID VPSS_DRV_Set_VPSS_FTCONFIG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 node_rst_en )
{
     U_VPSS_FTCONFIG VPSS_FTCONFIG;

     VPSS_FTCONFIG.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_FTCONFIG.u32));
     VPSS_FTCONFIG.bits.node_rst_en = node_rst_en ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_FTCONFIG.u32), VPSS_FTCONFIG.u32);

     return;
}
//addr=0x324
HI_VOID VPSS_DRV_Set_VPSS_INTMASK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bus_r_err_mask ,HI_U32 dcmp_err_mask ,HI_U32 vhd0_tunl_mask ,HI_U32 eof_end_mask ,HI_U32 bus_w_err_mask ,HI_U32 timeout_mask ,HI_U32 eof_mask )
{
     U_VPSS_INTMASK VPSS_INTMASK;

     VPSS_INTMASK.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_INTMASK.u32));
     VPSS_INTMASK.bits.bus_r_err_mask = bus_r_err_mask ;
     VPSS_INTMASK.bits.dcmp_err_mask  = dcmp_err_mask  ;
     VPSS_INTMASK.bits.vhd0_tunl_mask = vhd0_tunl_mask ;
     VPSS_INTMASK.bits.eof_end_mask   = eof_end_mask   ;
     VPSS_INTMASK.bits.bus_w_err_mask = bus_w_err_mask ;
     VPSS_INTMASK.bits.timeout_mask   = timeout_mask   ;
     VPSS_INTMASK.bits.eof_mask       = eof_mask       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_INTMASK.u32), VPSS_INTMASK.u32);

     return;
}
//addr=0x344
HI_VOID VPSS_DRV_Set_VPSS_BUS_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b128_split_en ,HI_U32 b256_split_en )
{
     U_VPSS_BUS_CTRL VPSS_BUS_CTRL;

     VPSS_BUS_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BUS_CTRL.u32));
     VPSS_BUS_CTRL.bits.b128_split_en = b128_split_en ;
     VPSS_BUS_CTRL.bits.b256_split_en = b256_split_en ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BUS_CTRL.u32), VPSS_BUS_CTRL.u32);

     return;
}
//addr=0x400
HI_VOID VPSS_DRV_Set_VPSS_IMGSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 imgheight ,HI_U32 imgwidth )
{
     U_VPSS_IMGSIZE VPSS_IMGSIZE;

     VPSS_IMGSIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IMGSIZE.u32));
     VPSS_IMGSIZE.bits.imgheight  = imgheight  -1;
     VPSS_IMGSIZE.bits.imgwidth   = imgwidth   -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IMGSIZE.u32), VPSS_IMGSIZE.u32);

     return;
}
//addr=0x404
HI_VOID VPSS_DRV_Set_VPSS_REFSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refheight ,HI_U32 refwidth )
{
     U_VPSS_REFSIZE VPSS_REFSIZE;

     VPSS_REFSIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REFSIZE.u32));
     VPSS_REFSIZE.bits.refheight  = refheight  -1;
     VPSS_REFSIZE.bits.refwidth   = refwidth   -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REFSIZE.u32), VPSS_REFSIZE.u32);

     return;
}
//addr=0x408
HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cts_bit_sel ,HI_U32 cts_en ,HI_U32 snr_mad_upfir_en ,HI_U32 uv_invert ,HI_U32 rfr_pix_bitw ,HI_U32 ref_nxt_pix_bitw ,HI_U32 in_pix_bitw ,HI_U32 in_format )
{
     U_VPSS_IN_CTRL VPSS_IN_CTRL;

     VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));
     VPSS_IN_CTRL.bits.cts_bit_sel      = cts_bit_sel      ;
     VPSS_IN_CTRL.bits.cts_en           = cts_en           ;
     VPSS_IN_CTRL.bits.snr_mad_upfir_en = snr_mad_upfir_en ;
     VPSS_IN_CTRL.bits.uv_invert        = uv_invert        ;
     VPSS_IN_CTRL.bits.rfr_pix_bitw     = rfr_pix_bitw     ;
     VPSS_IN_CTRL.bits.ref_nxt_pix_bitw = ref_nxt_pix_bitw ;
     VPSS_IN_CTRL.bits.in_pix_bitw      = in_pix_bitw      ;
     VPSS_IN_CTRL.bits.in_format        = in_format        ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_CTS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cts_bit_sel ,HI_U32 cts_en )
{
     U_VPSS_IN_CTRL VPSS_IN_CTRL;

     VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));
     VPSS_IN_CTRL.bits.cts_bit_sel      = cts_bit_sel      ;
     VPSS_IN_CTRL.bits.cts_en           = cts_en           ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_nx2(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 in_pix_bitw ,HI_U32 in_format )
{
     U_VPSS_IN_CTRL VPSS_IN_CTRL;

     VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));

     VPSS_IN_CTRL.bits.in_pix_bitw      = in_pix_bitw      ;
     VPSS_IN_CTRL.bits.in_format        = in_format        ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_ref(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 ref_nxt_pix_bitw )
{
	 U_VPSS_IN_CTRL VPSS_IN_CTRL;

	 VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));

	 VPSS_IN_CTRL.bits.ref_nxt_pix_bitw = ref_nxt_pix_bitw ;

	 VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);

	 return;
}

HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_rfr_bitw(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 rfr_pix_bitw )
{
	 U_VPSS_IN_CTRL VPSS_IN_CTRL;

	 VPSS_IN_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32));

	 VPSS_IN_CTRL.bits.rfr_pix_bitw = rfr_pix_bitw ;

	 VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_IN_CTRL.u32), VPSS_IN_CTRL.u32);

	 return;
}


//addr=0x40c
HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_dcmp_en ,HI_U32 nxt2_ver_offset ,HI_U32 nxt2_hor_offset ,HI_U32 nxt2_tile_format )
{
     U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;

     VPSS_NXT2_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32));
     VPSS_NXT2_CTRL.bits.nxt2_dcmp_en     = nxt2_dcmp_en     ;
     VPSS_NXT2_CTRL.bits.nxt2_ver_offset  = nxt2_ver_offset  ;
     VPSS_NXT2_CTRL.bits.nxt2_hor_offset  = nxt2_hor_offset  ;
     VPSS_NXT2_CTRL.bits.nxt2_tile_format = nxt2_tile_format ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32), VPSS_NXT2_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_decmp(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_dcmp_en )
{
     U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;

     VPSS_NXT2_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32));
     VPSS_NXT2_CTRL.bits.nxt2_dcmp_en     = nxt2_dcmp_en     ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32), VPSS_NXT2_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_offset(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_ver_offset ,HI_U32 nxt2_hor_offset )
{
     U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;

     VPSS_NXT2_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32));
     VPSS_NXT2_CTRL.bits.nxt2_ver_offset  = nxt2_ver_offset  ;
     VPSS_NXT2_CTRL.bits.nxt2_hor_offset  = nxt2_hor_offset  ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32), VPSS_NXT2_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_tile(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 nxt2_tile_format )
{
     U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;

     VPSS_NXT2_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32));
     VPSS_NXT2_CTRL.bits.nxt2_tile_format  = nxt2_tile_format  ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2_CTRL.u32), VPSS_NXT2_CTRL.u32);

     return;
}


//addr=0x410
HI_VOID VPSS_DRV_Set_VPSS_NXT2YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2y_addr )
{
     U_VPSS_NXT2YADDR VPSS_NXT2YADDR;

     VPSS_NXT2YADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2YADDR.u32));
     VPSS_NXT2YADDR.bits.nxt2y_addr = nxt2y_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2YADDR.u32), VPSS_NXT2YADDR.u32);

     return;
}
//addr=0x414
HI_VOID VPSS_DRV_Set_VPSS_NXT2CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2c_addr )
{
     U_VPSS_NXT2CADDR VPSS_NXT2CADDR;

     VPSS_NXT2CADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2CADDR.u32));
     VPSS_NXT2CADDR.bits.nxt2c_addr = nxt2c_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2CADDR.u32), VPSS_NXT2CADDR.u32);

     return;
}
//addr=0x418
HI_VOID VPSS_DRV_Set_VPSS_NXT2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2c_stride ,HI_U32 nxt2y_stride )
{
     U_VPSS_NXT2STRIDE VPSS_NXT2STRIDE;

     VPSS_NXT2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2STRIDE.u32));
     VPSS_NXT2STRIDE.bits.nxt2c_stride = nxt2c_stride ;
     VPSS_NXT2STRIDE.bits.nxt2y_stride = nxt2y_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT2STRIDE.u32), VPSS_NXT2STRIDE.u32);

     return;
}
//addr=0x41c
HI_VOID VPSS_DRV_Set_VPSS_NX2Y_HEAD_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2y_top_head_addr )
{
     U_VPSS_NX2Y_HEAD_ADDR VPSS_NX2Y_HEAD_ADDR;

     VPSS_NX2Y_HEAD_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2Y_HEAD_ADDR.u32));
     VPSS_NX2Y_HEAD_ADDR.bits.nx2y_top_head_addr = nx2y_top_head_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2Y_HEAD_ADDR.u32), VPSS_NX2Y_HEAD_ADDR.u32);

     return;
}
//addr=0x420
HI_VOID VPSS_DRV_Set_VPSS_NX2C_HEAD_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2c_top_head_addr )
{
     U_VPSS_NX2C_HEAD_ADDR VPSS_NX2C_HEAD_ADDR;

     VPSS_NX2C_HEAD_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2C_HEAD_ADDR.u32));
     VPSS_NX2C_HEAD_ADDR.bits.nx2c_top_head_addr = nx2c_top_head_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2C_HEAD_ADDR.u32), VPSS_NX2C_HEAD_ADDR.u32);

     return;
}
//addr=0x424
HI_VOID VPSS_DRV_Set_VPSS_NX2_HEAD_STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_head_stride )
{
     U_VPSS_NX2_HEAD_STRIDE VPSS_NX2_HEAD_STRIDE;

     VPSS_NX2_HEAD_STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_HEAD_STRIDE.u32));
     VPSS_NX2_HEAD_STRIDE.bits.nx2_head_stride = nx2_head_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_HEAD_STRIDE.u32), VPSS_NX2_HEAD_STRIDE.u32);

     return;
}
//addr=0x428
HI_VOID VPSS_DRV_Set_VPSS_NX2_TUNL_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_tunl_rd_interval ,HI_U32 nx2_tunl_en )
{
     U_VPSS_NX2_TUNL_CTRL VPSS_NX2_TUNL_CTRL;

     VPSS_NX2_TUNL_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_TUNL_CTRL.u32));
     VPSS_NX2_TUNL_CTRL.bits.nx2_tunl_rd_interval = nx2_tunl_rd_interval ;
     VPSS_NX2_TUNL_CTRL.bits.nx2_tunl_en          = nx2_tunl_en          ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_TUNL_CTRL.u32), VPSS_NX2_TUNL_CTRL.u32);

     return;
}
//addr=0x42c
HI_VOID VPSS_DRV_Set_VPSS_NX2_TUNL_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_tunl_addr )
{
     U_VPSS_NX2_TUNL_ADDR VPSS_NX2_TUNL_ADDR;

     VPSS_NX2_TUNL_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_TUNL_ADDR.u32));
     VPSS_NX2_TUNL_ADDR.bits.nx2_tunl_addr = nx2_tunl_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_TUNL_ADDR.u32), VPSS_NX2_TUNL_ADDR.u32);

     return;
}
//addr=0x430
HI_VOID VPSS_DRV_Set_VPSS_TESTPATTERN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pat_bkgrnd ,HI_U32 pat_dis_width ,HI_U32 pat_angle ,HI_U32 test_pat_en )
{
     U_VPSS_TESTPATTERN VPSS_TESTPATTERN;

     VPSS_TESTPATTERN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TESTPATTERN.u32));
     VPSS_TESTPATTERN.bits.pat_bkgrnd    = pat_bkgrnd    ;
     VPSS_TESTPATTERN.bits.pat_dis_width = pat_dis_width -1;
     VPSS_TESTPATTERN.bits.pat_angle     = pat_angle     ;
     VPSS_TESTPATTERN.bits.test_pat_en   = test_pat_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TESTPATTERN.u32), VPSS_TESTPATTERN.u32);

     return;
}
//addr=0x434
HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_ver_offset ,HI_U32 nxt1_hor_offset ,HI_U32 nxt1_tile_format )
{
     U_VPSS_NXT1_CTRL VPSS_NXT1_CTRL;

     VPSS_NXT1_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32));
     VPSS_NXT1_CTRL.bits.nxt1_ver_offset  = nxt1_ver_offset  ;
     VPSS_NXT1_CTRL.bits.nxt1_hor_offset  = nxt1_hor_offset  ;
     VPSS_NXT1_CTRL.bits.nxt1_tile_format = nxt1_tile_format ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32), VPSS_NXT1_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL_nxt1_tile_format(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_tile_format )
{
     U_VPSS_NXT1_CTRL VPSS_NXT1_CTRL;

     VPSS_NXT1_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32));
     VPSS_NXT1_CTRL.bits.nxt1_tile_format = nxt1_tile_format ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32), VPSS_NXT1_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL_offset(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_ver_offset ,HI_U32 nxt1_hor_offset )
{
     U_VPSS_NXT1_CTRL VPSS_NXT1_CTRL;

     VPSS_NXT1_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32));
     VPSS_NXT1_CTRL.bits.nxt1_ver_offset  = nxt1_ver_offset  ;
     VPSS_NXT1_CTRL.bits.nxt1_hor_offset  = nxt1_hor_offset  ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1_CTRL.u32), VPSS_NXT1_CTRL.u32);

     return;
}




//addr=0x438
HI_VOID VPSS_DRV_Set_VPSS_NXT1YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1y_addr )
{
     U_VPSS_NXT1YADDR VPSS_NXT1YADDR;

     VPSS_NXT1YADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1YADDR.u32));
     VPSS_NXT1YADDR.bits.nxt1y_addr = nxt1y_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1YADDR.u32), VPSS_NXT1YADDR.u32);

     return;
}
//addr=0x43c
HI_VOID VPSS_DRV_Set_VPSS_NXT1CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1c_addr )
{
     U_VPSS_NXT1CADDR VPSS_NXT1CADDR;

     VPSS_NXT1CADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1CADDR.u32));
     VPSS_NXT1CADDR.bits.nxt1c_addr = nxt1c_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1CADDR.u32), VPSS_NXT1CADDR.u32);

     return;
}
//addr=0x440
HI_VOID VPSS_DRV_Set_VPSS_NXT1STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1c_stride ,HI_U32 nxt1y_stride )
{
     U_VPSS_NXT1STRIDE VPSS_NXT1STRIDE;

     VPSS_NXT1STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1STRIDE.u32));
     VPSS_NXT1STRIDE.bits.nxt1c_stride = nxt1c_stride ;
     VPSS_NXT1STRIDE.bits.nxt1y_stride = nxt1y_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NXT1STRIDE.u32), VPSS_NXT1STRIDE.u32);

     return;
}
//addr=0x444
HI_VOID VPSS_DRV_Set_VPSS_CUR_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_ver_offset ,HI_U32 cur_hor_offset ,HI_U32 cur_tile_format )
{
     U_VPSS_CUR_CTRL VPSS_CUR_CTRL;

     VPSS_CUR_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUR_CTRL.u32));
     VPSS_CUR_CTRL.bits.cur_ver_offset  = cur_ver_offset  ;
     VPSS_CUR_CTRL.bits.cur_hor_offset  = cur_hor_offset  ;
     VPSS_CUR_CTRL.bits.cur_tile_format = cur_tile_format ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUR_CTRL.u32), VPSS_CUR_CTRL.u32);

     return;
}
//addr=0x448
HI_VOID VPSS_DRV_Set_VPSS_CURYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cury_addr )
{
     U_VPSS_CURYADDR VPSS_CURYADDR;

     VPSS_CURYADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CURYADDR.u32));
     VPSS_CURYADDR.bits.cury_addr = cury_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CURYADDR.u32), VPSS_CURYADDR.u32);

     return;
}
//addr=0x44c
HI_VOID VPSS_DRV_Set_VPSS_CURCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 curc_addr )
{
     U_VPSS_CURCADDR VPSS_CURCADDR;

     VPSS_CURCADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CURCADDR.u32));
     VPSS_CURCADDR.bits.curc_addr = curc_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CURCADDR.u32), VPSS_CURCADDR.u32);

     return;
}
//addr=0x450
HI_VOID VPSS_DRV_Set_VPSS_CURSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 curc_stride ,HI_U32 cury_stride )
{
     U_VPSS_CURSTRIDE VPSS_CURSTRIDE;

     VPSS_CURSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CURSTRIDE.u32));
     VPSS_CURSTRIDE.bits.curc_stride = curc_stride ;
     VPSS_CURSTRIDE.bits.cury_stride = cury_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CURSTRIDE.u32), VPSS_CURSTRIDE.u32);

     return;
}
//addr=0x454
HI_VOID VPSS_DRV_Set_VPSS_REF_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_ver_offset ,HI_U32 ref_hor_offset ,HI_U32 ref_tile_format )
{
     U_VPSS_REF_CTRL VPSS_REF_CTRL;

     VPSS_REF_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REF_CTRL.u32));
     VPSS_REF_CTRL.bits.ref_ver_offset  = ref_ver_offset  ;
     VPSS_REF_CTRL.bits.ref_hor_offset  = ref_hor_offset  ;
     VPSS_REF_CTRL.bits.ref_tile_format = ref_tile_format ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REF_CTRL.u32), VPSS_REF_CTRL.u32);

     return;
}
//addr=0x458
HI_VOID VPSS_DRV_Set_VPSS_REFYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refy_addr )
{
     U_VPSS_REFYADDR VPSS_REFYADDR;

     VPSS_REFYADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REFYADDR.u32));
     VPSS_REFYADDR.bits.refy_addr = refy_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REFYADDR.u32), VPSS_REFYADDR.u32);

     return;
}
//addr=0x45c
HI_VOID VPSS_DRV_Set_VPSS_REFCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refc_addr )
{
     U_VPSS_REFCADDR VPSS_REFCADDR;

     VPSS_REFCADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REFCADDR.u32));
     VPSS_REFCADDR.bits.refc_addr = refc_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REFCADDR.u32), VPSS_REFCADDR.u32);

     return;
}
//addr=0x460
HI_VOID VPSS_DRV_Set_VPSS_REFSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refc_stride ,HI_U32 refy_stride )
{
     U_VPSS_REFSTRIDE VPSS_REFSTRIDE;

     VPSS_REFSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REFSTRIDE.u32));
     VPSS_REFSTRIDE.bits.refc_stride = refc_stride ;
     VPSS_REFSTRIDE.bits.refy_stride = refy_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REFSTRIDE.u32), VPSS_REFSTRIDE.u32);

     return;
}
//addr=0x464
HI_VOID VPSS_DRV_Set_VPSS_REE_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs )
{
     U_VPSS_REE_CTRL VPSS_REE_CTRL;

     VPSS_REE_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REE_CTRL.u32));

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REE_CTRL.u32), VPSS_REE_CTRL.u32);

     return;
}
//addr=0x468
HI_VOID VPSS_DRV_Set_VPSS_REEYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reey_addr )
{
     U_VPSS_REEYADDR VPSS_REEYADDR;

     VPSS_REEYADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REEYADDR.u32));
     VPSS_REEYADDR.bits.reey_addr = reey_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REEYADDR.u32), VPSS_REEYADDR.u32);

     return;
}
//addr=0x46c
HI_VOID VPSS_DRV_Set_VPSS_REECADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reec_addr )
{
     U_VPSS_REECADDR VPSS_REECADDR;

     VPSS_REECADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REECADDR.u32));
     VPSS_REECADDR.bits.reec_addr = reec_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REECADDR.u32), VPSS_REECADDR.u32);

     return;
}
//addr=0x470
HI_VOID VPSS_DRV_Set_VPSS_REESTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reec_stride ,HI_U32 reey_stride )
{
     U_VPSS_REESTRIDE VPSS_REESTRIDE;

     VPSS_REESTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REESTRIDE.u32));
     VPSS_REESTRIDE.bits.reec_stride = reec_stride ;
     VPSS_REESTRIDE.bits.reey_stride = reey_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REESTRIDE.u32), VPSS_REESTRIDE.u32);

     return;
}
//addr=0x474
HI_VOID VPSS_DRV_Set_VPSS_PRJVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjv_cur_addr )
{
     U_VPSS_PRJVCURADDR VPSS_PRJVCURADDR;

     VPSS_PRJVCURADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJVCURADDR.u32));
     VPSS_PRJVCURADDR.bits.prjv_cur_addr = prjv_cur_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJVCURADDR.u32), VPSS_PRJVCURADDR.u32);

     return;
}
//addr=0x478
HI_VOID VPSS_DRV_Set_VPSS_PRJHCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_addr )
{
     U_VPSS_PRJHCURADDR VPSS_PRJHCURADDR;

     VPSS_PRJHCURADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJHCURADDR.u32));
     VPSS_PRJHCURADDR.bits.prjh_cur_addr = prjh_cur_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJHCURADDR.u32), VPSS_PRJHCURADDR.u32);

     return;
}
//addr=0x47c
HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_stride ,HI_U32 prjv_cur_stride )
{
     U_VPSS_PRJCURSTRIDE VPSS_PRJCURSTRIDE;

     VPSS_PRJCURSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32));
     VPSS_PRJCURSTRIDE.bits.prjh_cur_stride = prjh_cur_stride ;
     VPSS_PRJCURSTRIDE.bits.prjv_cur_stride = prjv_cur_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32), VPSS_PRJCURSTRIDE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE_H(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_stride)
{
     U_VPSS_PRJCURSTRIDE VPSS_PRJCURSTRIDE;

     VPSS_PRJCURSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32));
     VPSS_PRJCURSTRIDE.bits.prjh_cur_stride = prjh_cur_stride ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32), VPSS_PRJCURSTRIDE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE_V(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 prjv_cur_stride )
{
     U_VPSS_PRJCURSTRIDE VPSS_PRJCURSTRIDE;

     VPSS_PRJCURSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32));
     VPSS_PRJCURSTRIDE.bits.prjv_cur_stride = prjv_cur_stride ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJCURSTRIDE.u32), VPSS_PRJCURSTRIDE.u32);

     return;
}


//addr=0x480
HI_VOID VPSS_DRV_Set_VPSS_RGMVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_cur_addr )
{
     U_VPSS_RGMVCURADDR VPSS_RGMVCURADDR;

     VPSS_RGMVCURADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVCURADDR.u32));
     VPSS_RGMVCURADDR.bits.rgmv_cur_addr = rgmv_cur_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVCURADDR.u32), VPSS_RGMVCURADDR.u32);

     return;
}
//addr=0x484
HI_VOID VPSS_DRV_Set_VPSS_RGMVNX1ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx1_addr )
{
     U_VPSS_RGMVNX1ADDR VPSS_RGMVNX1ADDR;

     VPSS_RGMVNX1ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX1ADDR.u32));
     VPSS_RGMVNX1ADDR.bits.rgmv_nx1_addr = rgmv_nx1_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX1ADDR.u32), VPSS_RGMVNX1ADDR.u32);

     return;
}
//addr=0x488
HI_VOID VPSS_DRV_Set_VPSS_RGMVSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx1_stride ,HI_U32 rgmv_cur_stride )
{
     U_VPSS_RGMVSTRIDE VPSS_RGMVSTRIDE;

     VPSS_RGMVSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVSTRIDE.u32));
     VPSS_RGMVSTRIDE.bits.rgmv_nx1_stride = rgmv_nx1_stride ;
     VPSS_RGMVSTRIDE.bits.rgmv_cur_stride = rgmv_cur_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVSTRIDE.u32), VPSS_RGMVSTRIDE.u32);

     return;
}
//addr=0x48c
HI_VOID VPSS_DRV_Set_VPSS_BLKMVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_cur_addr )
{
     U_VPSS_BLKMVCURADDR VPSS_BLKMVCURADDR;

     VPSS_BLKMVCURADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVCURADDR.u32));
     VPSS_BLKMVCURADDR.bits.blkmv_cur_addr = blkmv_cur_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVCURADDR.u32), VPSS_BLKMVCURADDR.u32);

     return;
}
//addr=0x490
HI_VOID VPSS_DRV_Set_VPSS_BLKMVREFADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_ref_addr )
{
     U_VPSS_BLKMVREFADDR VPSS_BLKMVREFADDR;

     VPSS_BLKMVREFADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVREFADDR.u32));
     VPSS_BLKMVREFADDR.bits.blkmv_ref_addr = blkmv_ref_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVREFADDR.u32), VPSS_BLKMVREFADDR.u32);

     return;
}
//addr=0x494
HI_VOID VPSS_DRV_Set_VPSS_BLKMVSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_cur_stride ,HI_U32 blkmv_ref_stride )
{
     U_VPSS_BLKMVSTRIDE VPSS_BLKMVSTRIDE;

     VPSS_BLKMVSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVSTRIDE.u32));
     VPSS_BLKMVSTRIDE.bits.blkmv_cur_stride = blkmv_cur_stride ;
     VPSS_BLKMVSTRIDE.bits.blkmv_ref_stride = blkmv_ref_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVSTRIDE.u32), VPSS_BLKMVSTRIDE.u32);

     return;
}
//addr=0x498
HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX1ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx1_addr )
{
     U_VPSS_BLKMVNX1ADDR VPSS_BLKMVNX1ADDR;

     VPSS_BLKMVNX1ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX1ADDR.u32));
     VPSS_BLKMVNX1ADDR.bits.blkmv_nx1_addr = blkmv_nx1_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX1ADDR.u32), VPSS_BLKMVNX1ADDR.u32);

     return;
}
//addr=0x49c
HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX1STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx1_stride )
{
     U_VPSS_BLKMVNX1STRIDE VPSS_BLKMVNX1STRIDE;

     VPSS_BLKMVNX1STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX1STRIDE.u32));
     VPSS_BLKMVNX1STRIDE.bits.blkmv_nx1_stride = blkmv_nx1_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX1STRIDE.u32), VPSS_BLKMVNX1STRIDE.u32);

     return;
}
//addr=0x4a0
HI_VOID VPSS_DRV_Set_VPSS_MAD_RADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_raddr )
{
     U_VPSS_MAD_RADDR VPSS_MAD_RADDR;

     VPSS_MAD_RADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MAD_RADDR.u32));
     VPSS_MAD_RADDR.bits.mad_raddr = mad_raddr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MAD_RADDR.u32), VPSS_MAD_RADDR.u32);

     return;
}
//addr=0x4a4
HI_VOID VPSS_DRV_Set_VPSS_SNR_MAD_RADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_mad_raddr )
{
     U_VPSS_SNR_MAD_RADDR VPSS_SNR_MAD_RADDR;

     VPSS_SNR_MAD_RADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_MAD_RADDR.u32));
     VPSS_SNR_MAD_RADDR.bits.snr_mad_raddr = snr_mad_raddr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_MAD_RADDR.u32), VPSS_SNR_MAD_RADDR.u32);

     return;
}
//addr=0x4a8
HI_VOID VPSS_DRV_Set_VPSS_MADSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_stride )
{
     U_VPSS_MADSTRIDE VPSS_MADSTRIDE;

     VPSS_MADSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MADSTRIDE.u32));
     VPSS_MADSTRIDE.bits.mad_stride = mad_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MADSTRIDE.u32), VPSS_MADSTRIDE.u32);

     return;
}
//addr=0x4ac
HI_VOID VPSS_DRV_Set_VPSS_STRADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_raddr )
{
     U_VPSS_STRADDR VPSS_STRADDR;

     VPSS_STRADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_STRADDR.u32));
     VPSS_STRADDR.bits.st_raddr = st_raddr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_STRADDR.u32), VPSS_STRADDR.u32);

     return;
}
//addr=0x4b0
HI_VOID VPSS_DRV_Set_VPSS_STSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_stride )
{
     U_VPSS_STSTRIDE VPSS_STSTRIDE;

     VPSS_STSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_STSTRIDE.u32));
     VPSS_STSTRIDE.bits.st_stride  = st_stride  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_STSTRIDE.u32), VPSS_STSTRIDE.u32);

     return;
}
//addr=0x4b4
HI_VOID VPSS_DRV_Set_VPSS_TRANS_SRC_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_src_addr )
{
     U_VPSS_TRANS_SRC_ADDR VPSS_TRANS_SRC_ADDR;

     VPSS_TRANS_SRC_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_SRC_ADDR.u32));
     VPSS_TRANS_SRC_ADDR.bits.trans_src_addr = trans_src_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_SRC_ADDR.u32), VPSS_TRANS_SRC_ADDR.u32);

     return;
}
//addr=0x4b8
HI_VOID VPSS_DRV_Set_VPSS_TRANS_DES_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_des_addr )
{
     U_VPSS_TRANS_DES_ADDR VPSS_TRANS_DES_ADDR;

     VPSS_TRANS_DES_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_DES_ADDR.u32));
     VPSS_TRANS_DES_ADDR.bits.trans_des_addr = trans_des_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_DES_ADDR.u32), VPSS_TRANS_DES_ADDR.u32);

     return;
}
//addr=0x4bc
HI_VOID VPSS_DRV_Set_VPSS_TRANS_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_en ,HI_U32 trans_size )
{
     U_VPSS_TRANS_INFO VPSS_TRANS_INFO;

     VPSS_TRANS_INFO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_INFO.u32));
     VPSS_TRANS_INFO.bits.trans_en   = trans_en   ;
     VPSS_TRANS_INFO.bits.trans_size = trans_size ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TRANS_INFO.u32), VPSS_TRANS_INFO.u32);

     return;
}
//addr=0x4c0
HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_dither_mode ,HI_U32 vhd0_lcmp_en ,HI_U32 vhd0_uv_invert ,HI_U32 vhd0_dither_en ,HI_U32 vhd0_pix_bitw ,HI_U32 vhd0_flip ,HI_U32 vhd0_mirror ,HI_U32 vhd0_format )
{
     U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

     VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));
     VPSS_VHD0CTRL.bits.vhd0_dither_mode = vhd0_dither_mode ;
     VPSS_VHD0CTRL.bits.vhd0_lcmp_en     = vhd0_lcmp_en     ;
     VPSS_VHD0CTRL.bits.vhd0_uv_invert   = vhd0_uv_invert   ;
     VPSS_VHD0CTRL.bits.vhd0_dither_en   = vhd0_dither_en   ;
     VPSS_VHD0CTRL.bits.vhd0_pix_bitw    = vhd0_pix_bitw    ;
     VPSS_VHD0CTRL.bits.vhd0_flip        = vhd0_flip        ;
     VPSS_VHD0CTRL.bits.vhd0_mirror      = vhd0_mirror      ;
     VPSS_VHD0CTRL.bits.vhd0_format      = vhd0_format      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_bitw(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pix_bitw )
{
     U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

     VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));

     VPSS_VHD0CTRL.bits.vhd0_pix_bitw    = vhd0_pix_bitw    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_format(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_format )
{
     U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

     VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));

     VPSS_VHD0CTRL.bits.vhd0_format      = vhd0_format      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_dither(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_dither_mode, HI_U32 vhd0_dither_en )
{
     U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

     VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));
     VPSS_VHD0CTRL.bits.vhd0_dither_mode = vhd0_dither_mode ;
     VPSS_VHD0CTRL.bits.vhd0_dither_en   = vhd0_dither_en   ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

     return;
}

#if 1
HI_VOID VPSS_DRV_Set_vhd0_flip(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_flip)
{
	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

	VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));

	VPSS_VHD0CTRL.bits.vhd0_flip		= vhd0_flip 	   ;

	VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

	return;
}

HI_VOID VPSS_DRV_Set_vhd0_mirror(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_mirror)
{
	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

	VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));

	VPSS_VHD0CTRL.bits.vhd0_mirror		= vhd0_mirror 	   ;

	VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

	return;
}

HI_VOID VPSS_DRV_Set_vhd0_uv_invert(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_uv_invert)
{
	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

	VPSS_VHD0CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32));

	VPSS_VHD0CTRL.bits.vhd0_uv_invert		= vhd0_uv_invert 	   ;

	VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32);

	return;
}



#endif

//addr=0x4c4
HI_VOID VPSS_DRV_Set_VPSS_VHD0SIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_height ,HI_U32 vhd0_width )
{
     U_VPSS_VHD0SIZE VPSS_VHD0SIZE;

     VPSS_VHD0SIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0SIZE.u32));
     VPSS_VHD0SIZE.bits.vhd0_height = vhd0_height -1;
     VPSS_VHD0SIZE.bits.vhd0_width  = vhd0_width  -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0SIZE.u32), VPSS_VHD0SIZE.u32);

     return;
}
//addr=0x4c8
HI_VOID VPSS_DRV_Set_VPSS_VHD0YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0y_addr )
{
     U_VPSS_VHD0YADDR VPSS_VHD0YADDR;

     VPSS_VHD0YADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0YADDR.u32));
     VPSS_VHD0YADDR.bits.vhd0y_addr = vhd0y_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0YADDR.u32), VPSS_VHD0YADDR.u32);

     return;
}
//addr=0x4cc
HI_VOID VPSS_DRV_Set_VPSS_VHD0CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0c_addr )
{
     U_VPSS_VHD0CADDR VPSS_VHD0CADDR;

     VPSS_VHD0CADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CADDR.u32));
     VPSS_VHD0CADDR.bits.vhd0c_addr = vhd0c_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CADDR.u32), VPSS_VHD0CADDR.u32);

     return;
}
//addr=0x4d0
HI_VOID VPSS_DRV_Set_VPSS_VHD0STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0c_stride ,HI_U32 vhd0y_stride )
{
     U_VPSS_VHD0STRIDE VPSS_VHD0STRIDE;

     VPSS_VHD0STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0STRIDE.u32));
     VPSS_VHD0STRIDE.bits.vhd0c_stride = vhd0c_stride ;
     VPSS_VHD0STRIDE.bits.vhd0y_stride = vhd0y_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0STRIDE.u32), VPSS_VHD0STRIDE.u32);

     return;
}
//addr=0x4d4
HI_VOID VPSS_DRV_Set_VPSS_RFRCTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfr_dither_mode ,HI_U32 rfr_dither_en )
{
     U_VPSS_RFRCTRL VPSS_RFRCTRL;

     VPSS_RFRCTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRCTRL.u32));
     VPSS_RFRCTRL.bits.rfr_dither_mode = rfr_dither_mode ;
     VPSS_RFRCTRL.bits.rfr_dither_en   = rfr_dither_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRCTRL.u32), VPSS_RFRCTRL.u32);

     return;
}
//addr=0x4d8
HI_VOID VPSS_DRV_Set_VPSS_RFRYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfry_addr )
{
     U_VPSS_RFRYADDR VPSS_RFRYADDR;

     VPSS_RFRYADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRYADDR.u32));
     VPSS_RFRYADDR.bits.rfry_addr = rfry_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRYADDR.u32), VPSS_RFRYADDR.u32);

     return;
}
//addr=0x4dc
HI_VOID VPSS_DRV_Set_VPSS_RFRCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfrc_addr )
{
     U_VPSS_RFRCADDR VPSS_RFRCADDR;

     VPSS_RFRCADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRCADDR.u32));
     VPSS_RFRCADDR.bits.rfrc_addr = rfrc_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRCADDR.u32), VPSS_RFRCADDR.u32);

     return;
}
//addr=0x4e0
HI_VOID VPSS_DRV_Set_VPSS_RFRSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfrc_stride ,HI_U32 rfry_stride )
{
     U_VPSS_RFRSTRIDE VPSS_RFRSTRIDE;

     VPSS_RFRSTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRSTRIDE.u32));
     VPSS_RFRSTRIDE.bits.rfrc_stride = rfrc_stride ;
     VPSS_RFRSTRIDE.bits.rfry_stride = rfry_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RFRSTRIDE.u32), VPSS_RFRSTRIDE.u32);

     return;
}
//addr=0x4e4
HI_VOID VPSS_DRV_Set_VPSS_CUE_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs )
{
     U_VPSS_CUE_CTRL VPSS_CUE_CTRL;

     VPSS_CUE_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUE_CTRL.u32));

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUE_CTRL.u32), VPSS_CUE_CTRL.u32);

     return;
}
//addr=0x4e8
HI_VOID VPSS_DRV_Set_VPSS_CUEYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuey_addr )
{
     U_VPSS_CUEYADDR VPSS_CUEYADDR;

     VPSS_CUEYADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUEYADDR.u32));
     VPSS_CUEYADDR.bits.cuey_addr = cuey_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUEYADDR.u32), VPSS_CUEYADDR.u32);

     return;
}
//addr=0x4ec
HI_VOID VPSS_DRV_Set_VPSS_CUECADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuec_addr )
{
     U_VPSS_CUECADDR VPSS_CUECADDR;

     VPSS_CUECADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUECADDR.u32));
     VPSS_CUECADDR.bits.cuec_addr = cuec_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUECADDR.u32), VPSS_CUECADDR.u32);

     return;
}
//addr=0x4f0
HI_VOID VPSS_DRV_Set_VPSS_CUESTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuec_stride ,HI_U32 cuey_stride )
{
     U_VPSS_CUESTRIDE VPSS_CUESTRIDE;

     VPSS_CUESTRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUESTRIDE.u32));
     VPSS_CUESTRIDE.bits.cuec_stride = cuec_stride ;
     VPSS_CUESTRIDE.bits.cuey_stride = cuey_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUESTRIDE.u32), VPSS_CUESTRIDE.u32);

     return;
}
//addr=0x4f4
HI_VOID VPSS_DRV_Set_VPSS_PRJVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjv_nx2_addr )
{
     U_VPSS_PRJVNX2ADDR VPSS_PRJVNX2ADDR;

     VPSS_PRJVNX2ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJVNX2ADDR.u32));
     VPSS_PRJVNX2ADDR.bits.prjv_nx2_addr = prjv_nx2_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJVNX2ADDR.u32), VPSS_PRJVNX2ADDR.u32);

     return;
}
//addr=0x4f8
HI_VOID VPSS_DRV_Set_VPSS_PRJHNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_addr )
{
     U_VPSS_PRJHNX2ADDR VPSS_PRJHNX2ADDR;

     VPSS_PRJHNX2ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJHNX2ADDR.u32));
     VPSS_PRJHNX2ADDR.bits.prjh_nx2_addr = prjh_nx2_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJHNX2ADDR.u32), VPSS_PRJHNX2ADDR.u32);

     return;
}
//addr=0x4fc
HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_stride ,HI_U32 prjv_nx2_stride )
{
     U_VPSS_PRJNX2STRIDE VPSS_PRJNX2STRIDE;

     VPSS_PRJNX2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32));
     VPSS_PRJNX2STRIDE.bits.prjh_nx2_stride = prjh_nx2_stride ;
     VPSS_PRJNX2STRIDE.bits.prjv_nx2_stride = prjv_nx2_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32), VPSS_PRJNX2STRIDE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE_V(S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 prjv_nx2_stride )
{
     U_VPSS_PRJNX2STRIDE VPSS_PRJNX2STRIDE;

     VPSS_PRJNX2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32));

     VPSS_PRJNX2STRIDE.bits.prjv_nx2_stride = prjv_nx2_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32), VPSS_PRJNX2STRIDE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE_H(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_stride )
{
     U_VPSS_PRJNX2STRIDE VPSS_PRJNX2STRIDE;

     VPSS_PRJNX2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32));
     VPSS_PRJNX2STRIDE.bits.prjh_nx2_stride = prjh_nx2_stride ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PRJNX2STRIDE.u32), VPSS_PRJNX2STRIDE.u32);

     return;
}

//addr=0x500
HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx2_addr )
{
     U_VPSS_BLKMVNX2ADDR VPSS_BLKMVNX2ADDR;

     VPSS_BLKMVNX2ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX2ADDR.u32));
     VPSS_BLKMVNX2ADDR.bits.blkmv_nx2_addr = blkmv_nx2_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX2ADDR.u32), VPSS_BLKMVNX2ADDR.u32);

     return;
}
//addr=0x504
HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx2_stride )
{
     U_VPSS_BLKMVNX2STRIDE VPSS_BLKMVNX2STRIDE;

     VPSS_BLKMVNX2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX2STRIDE.u32));
     VPSS_BLKMVNX2STRIDE.bits.blkmv_nx2_stride = blkmv_nx2_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_BLKMVNX2STRIDE.u32), VPSS_BLKMVNX2STRIDE.u32);

     return;
}
//addr=0x508
HI_VOID VPSS_DRV_Set_VPSS_RGMVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx2_addr )
{
     U_VPSS_RGMVNX2ADDR VPSS_RGMVNX2ADDR;

     VPSS_RGMVNX2ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX2ADDR.u32));
     VPSS_RGMVNX2ADDR.bits.rgmv_nx2_addr = rgmv_nx2_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX2ADDR.u32), VPSS_RGMVNX2ADDR.u32);

     return;
}
//addr=0x50c
HI_VOID VPSS_DRV_Set_VPSS_RGMVNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx2_stride )
{
     U_VPSS_RGMVNX2STRIDE VPSS_RGMVNX2STRIDE;

     VPSS_RGMVNX2STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX2STRIDE.u32));
     VPSS_RGMVNX2STRIDE.bits.rgmv_nx2_stride = rgmv_nx2_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RGMVNX2STRIDE.u32), VPSS_RGMVNX2STRIDE.u32);

     return;
}
//addr=0x510
HI_VOID VPSS_DRV_Set_VPSS_CHK_SUM_W_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chk_sum_w_addr )
{
     U_VPSS_CHK_SUM_W_ADDR VPSS_CHK_SUM_W_ADDR;

     VPSS_CHK_SUM_W_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CHK_SUM_W_ADDR.u32));
     VPSS_CHK_SUM_W_ADDR.bits.chk_sum_w_addr = chk_sum_w_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CHK_SUM_W_ADDR.u32), VPSS_CHK_SUM_W_ADDR.u32);

     return;
}
//addr=0x514
HI_VOID VPSS_DRV_Set_VPSS_CHK_SUM_SEL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 winfo_sum_sel ,HI_U32 rinfo_sum_sel ,HI_U32 rch3_sum_sel ,HI_U32 rch2_sum_sel ,HI_U32 rch1_sum_sel ,HI_U32 rch0_sum_sel )
{
     U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

     VPSS_CHK_SUM_SEL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CHK_SUM_SEL.u32));
     VPSS_CHK_SUM_SEL.bits.winfo_sum_sel = winfo_sum_sel ;
     VPSS_CHK_SUM_SEL.bits.rinfo_sum_sel = rinfo_sum_sel ;
     VPSS_CHK_SUM_SEL.bits.rch3_sum_sel  = rch3_sum_sel  ;
     VPSS_CHK_SUM_SEL.bits.rch2_sum_sel  = rch2_sum_sel  ;
     VPSS_CHK_SUM_SEL.bits.rch1_sum_sel  = rch1_sum_sel  ;
     VPSS_CHK_SUM_SEL.bits.rch0_sum_sel  = rch0_sum_sel  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32);

     return;
}
//addr=0x518
HI_VOID VPSS_DRV_Set_VPSS_STWADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_waddr )
{
     U_VPSS_STWADDR VPSS_STWADDR;

     VPSS_STWADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_STWADDR.u32));
     VPSS_STWADDR.bits.st_waddr = st_waddr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_STWADDR.u32), VPSS_STWADDR.u32);

     return;
}
//addr=0x51c
HI_VOID VPSS_DRV_Set_VPSS_MAD_WADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_waddr )
{
     U_VPSS_MAD_WADDR VPSS_MAD_WADDR;

     VPSS_MAD_WADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MAD_WADDR.u32));
     VPSS_MAD_WADDR.bits.mad_waddr = mad_waddr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MAD_WADDR.u32), VPSS_MAD_WADDR.u32);

     return;
}
//addr=0x520
HI_VOID VPSS_DRV_Set_VPSS_STT_W_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 stt_w_addr )
{
     U_VPSS_STT_W_ADDR VPSS_STT_W_ADDR;

     VPSS_STT_W_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_STT_W_ADDR.u32));
     VPSS_STT_W_ADDR.bits.stt_w_addr = stt_w_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_STT_W_ADDR.u32), VPSS_STT_W_ADDR.u32);

     return;
}
//addr=0x524
HI_VOID VPSS_DRV_Set_VPSS_NX2Y_LB_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2ylb_addr )
{
     U_VPSS_NX2Y_LB_ADDR VPSS_NX2Y_LB_ADDR;

     VPSS_NX2Y_LB_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2Y_LB_ADDR.u32));
     VPSS_NX2Y_LB_ADDR.bits.nx2ylb_addr = nx2ylb_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2Y_LB_ADDR.u32), VPSS_NX2Y_LB_ADDR.u32);

     return;
}
//addr=0x528
HI_VOID VPSS_DRV_Set_VPSS_NX2C_LB_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2clb_addr )
{
     U_VPSS_NX2C_LB_ADDR VPSS_NX2C_LB_ADDR;

     VPSS_NX2C_LB_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2C_LB_ADDR.u32));
     VPSS_NX2C_LB_ADDR.bits.nx2clb_addr = nx2clb_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2C_LB_ADDR.u32), VPSS_NX2C_LB_ADDR.u32);

     return;
}
//addr=0x52c
HI_VOID VPSS_DRV_Set_VPSS_NX2_LB_STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2lb_stride )
{
     U_VPSS_NX2_LB_STRIDE VPSS_NX2_LB_STRIDE;

     VPSS_NX2_LB_STRIDE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_LB_STRIDE.u32));
     VPSS_NX2_LB_STRIDE.bits.nx2lb_stride = nx2lb_stride ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_LB_STRIDE.u32), VPSS_NX2_LB_STRIDE.u32);

     return;
}
//addr=0x530
HI_VOID VPSS_DRV_Set_VPSS_VHD0_TUNL_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_tunl_en ,HI_U32 vhd0_tunl_mode ,HI_U32 vhd0_tunl_finish_line )
{
     U_VPSS_VHD0_TUNL_CTRL VPSS_VHD0_TUNL_CTRL;

     VPSS_VHD0_TUNL_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_TUNL_CTRL.u32));
     VPSS_VHD0_TUNL_CTRL.bits.vhd0_tunl_en          = vhd0_tunl_en          ;
     VPSS_VHD0_TUNL_CTRL.bits.vhd0_tunl_mode        = vhd0_tunl_mode        ;
     VPSS_VHD0_TUNL_CTRL.bits.vhd0_tunl_finish_line = vhd0_tunl_finish_line ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_TUNL_CTRL.u32), VPSS_VHD0_TUNL_CTRL.u32);

     return;
}
//addr=0x534
HI_VOID VPSS_DRV_Set_VPSS_VHD0_TUNL_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_tunl_addr )
{
     U_VPSS_VHD0_TUNL_ADDR VPSS_VHD0_TUNL_ADDR;

     VPSS_VHD0_TUNL_ADDR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_TUNL_ADDR.u32));
     VPSS_VHD0_TUNL_ADDR.bits.vhd0_tunl_addr = vhd0_tunl_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_TUNL_ADDR.u32), VPSS_VHD0_TUNL_ADDR.u32);

     return;
}
//addr=0x580
HI_VOID VPSS_DRV_Set_VPSS_VPZME_CFG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pre_vfir_mode )
{
     U_VPSS_VPZME_CFG0 VPSS_VPZME_CFG0;
	 HI_U32 u32VfrMode;
     VPSS_VPZME_CFG0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VPZME_CFG0.u32));

	 switch ( vhd0_pre_vfir_mode )
	 {
	     case PREZME_2X:
			 u32VfrMode = 0x0;
	         break;
	     case PREZME_4X:
			 u32VfrMode = 0x1;
	         break;
	     case PREZME_8X:
			 u32VfrMode = 0x2;
	         break;
	     case PREZME_VFIELD:
			 u32VfrMode = 0x3;
	         break;
	     default:
 			 u32VfrMode = 0x3;
	 }

     VPSS_VPZME_CFG0.bits.vhd0_pre_vfir_mode = u32VfrMode & 0x3 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VPZME_CFG0.u32), VPSS_VPZME_CFG0.u32);

     return;
}
//addr=0x584
HI_VOID VPSS_DRV_Set_VPSS_RTT_CFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rotate_angle )
{
     U_VPSS_RTT_CFG VPSS_RTT_CFG;

     VPSS_RTT_CFG.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_RTT_CFG.u32));
     VPSS_RTT_CFG.bits.rotate_angle = rotate_angle ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_RTT_CFG.u32), VPSS_RTT_CFG.u32);

     return;
}
//addr=0x588
HI_VOID VPSS_DRV_Set_VPSS_VHD0CROP_SIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_height ,HI_U32 vhd0_crop_width )
{
     U_VPSS_VHD0CROP_SIZE VPSS_VHD0CROP_SIZE;

     VPSS_VHD0CROP_SIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CROP_SIZE.u32));
     VPSS_VHD0CROP_SIZE.bits.vhd0_crop_height = vhd0_crop_height -1;
     VPSS_VHD0CROP_SIZE.bits.vhd0_crop_width  = vhd0_crop_width  -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CROP_SIZE.u32), VPSS_VHD0CROP_SIZE.u32);

     return;
}
//addr=0x58c
HI_VOID VPSS_DRV_Set_VPSS_VHD0CROP_POS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_y ,HI_U32 vhd0_crop_x )
{
     U_VPSS_VHD0CROP_POS VPSS_VHD0CROP_POS;

     VPSS_VHD0CROP_POS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CROP_POS.u32));
     VPSS_VHD0CROP_POS.bits.vhd0_crop_y = vhd0_crop_y ;
     VPSS_VHD0CROP_POS.bits.vhd0_crop_x = vhd0_crop_x ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0CROP_POS.u32), VPSS_VHD0CROP_POS.u32);

     return;
}
//addr=0x590
HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_DSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_height ,HI_U32 vhd0_lba_width )
{
     U_VPSS_VHD0LBA_DSIZE VPSS_VHD0LBA_DSIZE;

     VPSS_VHD0LBA_DSIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_DSIZE.u32));
     VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_height = vhd0_lba_height -1;
     VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_width  = vhd0_lba_width  -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_DSIZE.u32), VPSS_VHD0LBA_DSIZE.u32);

     return;
}
//addr=0x594
HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_VFPOS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_yfpos ,HI_U32 vhd0_lba_xfpos )
{
     U_VPSS_VHD0LBA_VFPOS VPSS_VHD0LBA_VFPOS;

     VPSS_VHD0LBA_VFPOS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_VFPOS.u32));
     VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_yfpos = vhd0_lba_yfpos ;
     VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_xfpos = vhd0_lba_xfpos ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_VFPOS.u32), VPSS_VHD0LBA_VFPOS.u32);

     return;
}
//addr=0x598
HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_BK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_vbk_cr ,HI_U32 vhd0_vbk_cb ,HI_U32 vhd0_vbk_y )
{
     U_VPSS_VHD0LBA_BK VPSS_VHD0LBA_BK;

     VPSS_VHD0LBA_BK.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_BK.u32));
     VPSS_VHD0LBA_BK.bits.vhd0_vbk_cr = vhd0_vbk_cr ;
     VPSS_VHD0LBA_BK.bits.vhd0_vbk_cb = vhd0_vbk_cb ;
     VPSS_VHD0LBA_BK.bits.vhd0_vbk_y  = vhd0_vbk_y  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0LBA_BK.u32), VPSS_VHD0LBA_BK.u32);

     return;
}
//addr=0x59c
HI_VOID VPSS_DRV_Set_VPSS_REF_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_vc1_rangedfrm ,HI_U32 ref_vc1_profile ,HI_U32 ref_vc1_mapyflg ,HI_U32 ref_vc1_mapcflg ,HI_U32 ref_vc1_mapy ,HI_U32 ref_vc1_mapc )
{
     U_VPSS_REF_VC1_CTRL0 VPSS_REF_VC1_CTRL0;

     VPSS_REF_VC1_CTRL0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_REF_VC1_CTRL0.u32));
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_rangedfrm = ref_vc1_rangedfrm ;
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_profile   = ref_vc1_profile   ;
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_mapyflg   = ref_vc1_mapyflg   ;
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_mapcflg   = ref_vc1_mapcflg   ;
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_mapy      = ref_vc1_mapy      ;
     VPSS_REF_VC1_CTRL0.bits.ref_vc1_mapc      = ref_vc1_mapc      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_REF_VC1_CTRL0.u32), VPSS_REF_VC1_CTRL0.u32);

     return;
}
//addr=0x5a0
HI_VOID VPSS_DRV_Set_VPSS_CUR_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_vc1_rangedfrm ,HI_U32 cur_vc1_profile ,HI_U32 cur_vc1_mapyflg ,HI_U32 cur_vc1_mapcflg ,HI_U32 cur_vc1_mapy ,HI_U32 cur_vc1_mapc )
{
     U_VPSS_CUR_VC1_CTRL0 VPSS_CUR_VC1_CTRL0;

     VPSS_CUR_VC1_CTRL0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_CUR_VC1_CTRL0.u32));
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_rangedfrm = cur_vc1_rangedfrm ;
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_profile   = cur_vc1_profile   ;
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_mapyflg   = cur_vc1_mapyflg   ;
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_mapcflg   = cur_vc1_mapcflg   ;
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_mapy      = cur_vc1_mapy      ;
     VPSS_CUR_VC1_CTRL0.bits.cur_vc1_mapc      = cur_vc1_mapc      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_CUR_VC1_CTRL0.u32), VPSS_CUR_VC1_CTRL0.u32);

     return;
}
//addr=0x5a4
HI_VOID VPSS_DRV_Set_VPSS_NX1_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx1_vc1_rangedfrm ,HI_U32 nx1_vc1_profile ,HI_U32 nx1_vc1_mapyflg ,HI_U32 nx1_vc1_mapcflg ,HI_U32 nx1_vc1_mapy ,HI_U32 nx1_vc1_mapc )
{
     U_VPSS_NX1_VC1_CTRL0 VPSS_NX1_VC1_CTRL0;

     VPSS_NX1_VC1_CTRL0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX1_VC1_CTRL0.u32));
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_rangedfrm = nx1_vc1_rangedfrm ;
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_profile   = nx1_vc1_profile   ;
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_mapyflg   = nx1_vc1_mapyflg   ;
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_mapcflg   = nx1_vc1_mapcflg   ;
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_mapy      = nx1_vc1_mapy      ;
     VPSS_NX1_VC1_CTRL0.bits.nx1_vc1_mapc      = nx1_vc1_mapc      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX1_VC1_CTRL0.u32), VPSS_NX1_VC1_CTRL0.u32);

     return;
}
//addr=0x5a8
HI_VOID VPSS_DRV_Set_VPSS_NX2_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_vc1_rangedfrm ,HI_U32 nx2_vc1_profile ,HI_U32 nx2_vc1_mapyflg ,HI_U32 nx2_vc1_mapcflg ,HI_U32 nx2_vc1_mapy ,HI_U32 nx2_vc1_mapc )
{
     U_VPSS_NX2_VC1_CTRL0 VPSS_NX2_VC1_CTRL0;

     VPSS_NX2_VC1_CTRL0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_VC1_CTRL0.u32));
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_rangedfrm = nx2_vc1_rangedfrm ;
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_profile   = nx2_vc1_profile   ;
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_mapyflg   = nx2_vc1_mapyflg   ;
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_mapcflg   = nx2_vc1_mapcflg   ;
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_mapy      = nx2_vc1_mapy      ;
     VPSS_NX2_VC1_CTRL0.bits.nx2_vc1_mapc      = nx2_vc1_mapc      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_NX2_VC1_CTRL0.u32), VPSS_NX2_VC1_CTRL0.u32);

     return;
}
//addr=0x1000
HI_VOID VPSS_DRV_Set_VPSS_DIECTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_out_sel_l ,HI_U32 die_out_sel_c ,HI_U32 die_l_mode ,HI_U32 die_c_mode ,HI_U32 mc_only ,HI_U32 ma_only ,HI_U32 edge_smooth_en ,HI_U32 mchdir_l ,HI_U32 mchdir_c ,HI_U32 die_rst ,HI_U32 stinfo_stop ,HI_U32 edge_smooth_ratio )
{
     U_VPSS_DIECTRL VPSS_DIECTRL;

     VPSS_DIECTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32));
     VPSS_DIECTRL.bits.die_out_sel_l     = die_out_sel_l     ;
     VPSS_DIECTRL.bits.die_out_sel_c     = die_out_sel_c     ;
     VPSS_DIECTRL.bits.die_l_mode        = die_l_mode        ;
     VPSS_DIECTRL.bits.die_c_mode        = die_c_mode        ;
     VPSS_DIECTRL.bits.mc_only           = mc_only           ;
     VPSS_DIECTRL.bits.ma_only           = ma_only           ;
     VPSS_DIECTRL.bits.edge_smooth_en    = edge_smooth_en    ;
     VPSS_DIECTRL.bits.mchdir_l          = mchdir_l          ;
     VPSS_DIECTRL.bits.mchdir_c          = mchdir_c          ;
     VPSS_DIECTRL.bits.die_rst           = die_rst           ;
     VPSS_DIECTRL.bits.stinfo_stop       = stinfo_stop       ;
     VPSS_DIECTRL.bits.edge_smooth_ratio = edge_smooth_ratio ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DIECTRL_lc_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_l_mode ,HI_U32 die_c_mode )
{
     U_VPSS_DIECTRL VPSS_DIECTRL;

     VPSS_DIECTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32));

     VPSS_DIECTRL.bits.die_l_mode        = die_l_mode        ;
     VPSS_DIECTRL.bits.die_c_mode        = die_c_mode        ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

     return;
}


HI_VOID VPSS_DRV_Set_VPSS_DIECTRL_outsel(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_out_sel_l ,HI_U32 die_out_sel_c )
{
     U_VPSS_DIECTRL VPSS_DIECTRL;

     VPSS_DIECTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32));
     VPSS_DIECTRL.bits.die_out_sel_l     = die_out_sel_l     ;
     VPSS_DIECTRL.bits.die_out_sel_c     = die_out_sel_c     ;
     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

     return;
}

//addr=0x1004
HI_VOID VPSS_DRV_Set_VPSS_DIELMA2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chroma_mf_offset ,HI_U32 dei_st_rst_value ,HI_U32 rec_mode_en ,HI_U32 chroma_mf_max ,HI_U32 luma_mf_max ,HI_U32 motion_iir_en ,HI_U32 luma_scesdf_max ,HI_U32 frame_motion_smooth_en ,HI_U32 recmode_frmfld_blend_mode )
{
     U_VPSS_DIELMA2 VPSS_DIELMA2;

     VPSS_DIELMA2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIELMA2.u32));
     VPSS_DIELMA2.bits.chroma_mf_offset          = chroma_mf_offset          ;
     VPSS_DIELMA2.bits.dei_st_rst_value          = dei_st_rst_value          ;
     VPSS_DIELMA2.bits.rec_mode_en               = rec_mode_en               ;
     VPSS_DIELMA2.bits.chroma_mf_max             = chroma_mf_max             ;
     VPSS_DIELMA2.bits.luma_mf_max               = luma_mf_max               ;
     VPSS_DIELMA2.bits.motion_iir_en             = motion_iir_en             ;
     VPSS_DIELMA2.bits.luma_scesdf_max           = luma_scesdf_max           ;
     VPSS_DIELMA2.bits.frame_motion_smooth_en    = frame_motion_smooth_en    ;
     VPSS_DIELMA2.bits.recmode_frmfld_blend_mode = recmode_frmfld_blend_mode ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DIELMA2_mf(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chroma_mf_max ,HI_U32 luma_mf_max )
{
     U_VPSS_DIELMA2 VPSS_DIELMA2;

     VPSS_DIELMA2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIELMA2.u32));

     VPSS_DIELMA2.bits.chroma_mf_max             = chroma_mf_max;
     VPSS_DIELMA2.bits.luma_mf_max               = luma_mf_max;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

     return;
}


//addr=0x1008
HI_VOID VPSS_DRV_Set_VPSS_DIEINTEN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 min_north_strength ,HI_U32 dir_ratio_north )
{
     U_VPSS_DIEINTEN VPSS_DIEINTEN;

     VPSS_DIEINTEN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTEN.u32));
     VPSS_DIEINTEN.bits.min_north_strength = min_north_strength ;
     VPSS_DIEINTEN.bits.dir_ratio_north    = dir_ratio_north    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTEN.u32), VPSS_DIEINTEN.u32);

     return;
}
//addr=0x100c
HI_VOID VPSS_DRV_Set_VPSS_DIESCALE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 range_scale )
{
     U_VPSS_DIESCALE VPSS_DIESCALE;

     VPSS_DIESCALE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIESCALE.u32));
     VPSS_DIESCALE.bits.range_scale = range_scale ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIESCALE.u32), VPSS_DIESCALE.u32);

     return;
}
//addr=0x1010
HI_VOID VPSS_DRV_Set_VPSS_DIECHECK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bc1_gain ,HI_U32 bc1_autodz_gain ,HI_U32 bc1_max_dz )
{
     U_VPSS_DIECHECK1 VPSS_DIECHECK1;

     VPSS_DIECHECK1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECHECK1.u32));
     VPSS_DIECHECK1.bits.bc1_gain        = bc1_gain        ;
     VPSS_DIECHECK1.bits.bc1_autodz_gain = bc1_autodz_gain ;
     VPSS_DIECHECK1.bits.bc1_max_dz      = bc1_max_dz      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECHECK1.u32), VPSS_DIECHECK1.u32);

     return;
}
//addr=0x1014
HI_VOID VPSS_DRV_Set_VPSS_DIECHECK2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bc2_gain ,HI_U32 bc2_autodz_gain ,HI_U32 bc2_max_dz )
{
     U_VPSS_DIECHECK2 VPSS_DIECHECK2;

     VPSS_DIECHECK2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECHECK2.u32));
     VPSS_DIECHECK2.bits.bc2_gain        = bc2_gain        ;
     VPSS_DIECHECK2.bits.bc2_autodz_gain = bc2_autodz_gain ;
     VPSS_DIECHECK2.bits.bc2_max_dz      = bc2_max_dz      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECHECK2.u32), VPSS_DIECHECK2.u32);

     return;
}
//addr=0x1018
HI_VOID VPSS_DRV_Set_VPSS_DIEDIR0_3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir3_scale ,HI_U32 dir2_scale ,HI_U32 dir1_scale ,HI_U32 dir0_scale )
{
     U_VPSS_DIEDIR0_3 VPSS_DIEDIR0_3;

     VPSS_DIEDIR0_3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR0_3.u32));
     VPSS_DIEDIR0_3.bits.dir3_scale = dir3_scale ;
     VPSS_DIEDIR0_3.bits.dir2_scale = dir2_scale ;
     VPSS_DIEDIR0_3.bits.dir1_scale = dir1_scale ;
     VPSS_DIEDIR0_3.bits.dir0_scale = dir0_scale ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR0_3.u32), VPSS_DIEDIR0_3.u32);

     return;
}
//addr=0x101c
HI_VOID VPSS_DRV_Set_VPSS_DIEDIR4_7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir7_scale ,HI_U32 dir6_scale ,HI_U32 dir5_scale ,HI_U32 dir4_scale )
{
     U_VPSS_DIEDIR4_7 VPSS_DIEDIR4_7;

     VPSS_DIEDIR4_7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR4_7.u32));
     VPSS_DIEDIR4_7.bits.dir7_scale = dir7_scale ;
     VPSS_DIEDIR4_7.bits.dir6_scale = dir6_scale ;
     VPSS_DIEDIR4_7.bits.dir5_scale = dir5_scale ;
     VPSS_DIEDIR4_7.bits.dir4_scale = dir4_scale ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR4_7.u32), VPSS_DIEDIR4_7.u32);

     return;
}
//addr=0x1020
HI_VOID VPSS_DRV_Set_VPSS_DIEDIR8_11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir11_scale ,HI_U32 dir10_scale ,HI_U32 dir9_scale ,HI_U32 dir8_scale )
{
     U_VPSS_DIEDIR8_11 VPSS_DIEDIR8_11;

     VPSS_DIEDIR8_11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR8_11.u32));
     VPSS_DIEDIR8_11.bits.dir11_scale = dir11_scale ;
     VPSS_DIEDIR8_11.bits.dir10_scale = dir10_scale ;
     VPSS_DIEDIR8_11.bits.dir9_scale  = dir9_scale  ;
     VPSS_DIEDIR8_11.bits.dir8_scale  = dir8_scale  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR8_11.u32), VPSS_DIEDIR8_11.u32);

     return;
}
//addr=0x1024
HI_VOID VPSS_DRV_Set_VPSS_DIEDIR12_14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir14_scale ,HI_U32 dir13_scale ,HI_U32 dir12_scale )
{
     U_VPSS_DIEDIR12_14 VPSS_DIEDIR12_14;

     VPSS_DIEDIR12_14.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR12_14.u32));
     VPSS_DIEDIR12_14.bits.dir14_scale = dir14_scale ;
     VPSS_DIEDIR12_14.bits.dir13_scale = dir13_scale ;
     VPSS_DIEDIR12_14.bits.dir12_scale = dir12_scale ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIR12_14.u32), VPSS_DIEDIR12_14.u32);

     return;
}
//addr=0x1028
HI_VOID VPSS_DRV_Set_VPSS_DIESTA(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 c_height_cnt ,HI_U32 l_height_cnt ,HI_U32 cur_cstate ,HI_U32 cur_state )
{
     U_VPSS_DIESTA VPSS_DIESTA;

     VPSS_DIESTA.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIESTA.u32));
     VPSS_DIESTA.bits.c_height_cnt = c_height_cnt -1;
     VPSS_DIESTA.bits.l_height_cnt = l_height_cnt -1;
     VPSS_DIESTA.bits.cur_cstate   = cur_cstate   ;
     VPSS_DIESTA.bits.cur_state    = cur_state    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIESTA.u32), VPSS_DIESTA.u32);

     return;
}
//addr=0x102c
HI_VOID VPSS_DRV_Set_VPSS_DIEINTPSCL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intp_scale_ratio_8 ,HI_U32 intp_scale_ratio_7 ,HI_U32 intp_scale_ratio_6 ,HI_U32 intp_scale_ratio_5 ,HI_U32 intp_scale_ratio_4 ,HI_U32 intp_scale_ratio_3 ,HI_U32 intp_scale_ratio_2 ,HI_U32 intp_scale_ratio_1 )
{
     U_VPSS_DIEINTPSCL0 VPSS_DIEINTPSCL0;

     VPSS_DIEINTPSCL0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTPSCL0.u32));
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_8 = intp_scale_ratio_8 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_7 = intp_scale_ratio_7 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_6 = intp_scale_ratio_6 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_5 = intp_scale_ratio_5 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_4 = intp_scale_ratio_4 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_3 = intp_scale_ratio_3 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_2 = intp_scale_ratio_2 ;
     VPSS_DIEINTPSCL0.bits.intp_scale_ratio_1 = intp_scale_ratio_1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTPSCL0.u32), VPSS_DIEINTPSCL0.u32);

     return;
}
//addr=0x1030
HI_VOID VPSS_DRV_Set_VPSS_DIEINTPSCL1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intp_scale_ratio_15 ,HI_U32 intp_scale_ratio_14 ,HI_U32 intp_scale_ratio_13 ,HI_U32 intp_scale_ratio_12 ,HI_U32 intp_scale_ratio_11 ,HI_U32 intp_scale_ratio_10 ,HI_U32 intp_scale_ratio_9 )
{
     U_VPSS_DIEINTPSCL1 VPSS_DIEINTPSCL1;

     VPSS_DIEINTPSCL1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTPSCL1.u32));
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_15 = intp_scale_ratio_15 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_14 = intp_scale_ratio_14 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_13 = intp_scale_ratio_13 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_12 = intp_scale_ratio_12 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_11 = intp_scale_ratio_11 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_10 = intp_scale_ratio_10 ;
     VPSS_DIEINTPSCL1.bits.intp_scale_ratio_9  = intp_scale_ratio_9  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEINTPSCL1.u32), VPSS_DIEINTPSCL1.u32);

     return;
}
//addr=0x1034
HI_VOID VPSS_DRV_Set_VPSS_DIEDIRTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 strength_thd ,HI_U32 hor_edge_en ,HI_U32 edge_mode ,HI_U32 dir_thd ,HI_U32 bc_gain )
{
     U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;

     VPSS_DIEDIRTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIRTHD.u32));
     VPSS_DIEDIRTHD.bits.strength_thd = strength_thd ;
     VPSS_DIEDIRTHD.bits.hor_edge_en  = hor_edge_en  ;
     VPSS_DIEDIRTHD.bits.edge_mode    = edge_mode    ;
     VPSS_DIEDIRTHD.bits.dir_thd      = dir_thd      ;
     VPSS_DIEDIRTHD.bits.bc_gain      = bc_gain      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

     return;
}
//addr=0x1038
HI_VOID VPSS_DRV_Set_VPSS_DIEJITMTN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fld_motion_coring ,HI_U32 jitter_coring ,HI_U32 jitter_gain )
{
     U_VPSS_DIEJITMTN VPSS_DIEJITMTN;

     VPSS_DIEJITMTN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEJITMTN.u32));
     VPSS_DIEJITMTN.bits.fld_motion_coring = fld_motion_coring ;
     VPSS_DIEJITMTN.bits.jitter_coring     = jitter_coring     ;
     VPSS_DIEJITMTN.bits.jitter_gain       = jitter_gain       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

     return;
}
//addr=0x103c
HI_VOID VPSS_DRV_Set_VPSS_DIEFLDMTN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 long_motion_shf ,HI_U32 fld_motion_wnd_mode ,HI_U32 fld_motion_gain ,HI_U32 fld_motion_curve_slope ,HI_U32 fld_motion_thd_high ,HI_U32 fld_motion_thd_low )
{
     U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;

     VPSS_DIEFLDMTN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFLDMTN.u32));
     VPSS_DIEFLDMTN.bits.long_motion_shf        = long_motion_shf        ;
     VPSS_DIEFLDMTN.bits.fld_motion_wnd_mode    = fld_motion_wnd_mode    ;
     VPSS_DIEFLDMTN.bits.fld_motion_gain        = fld_motion_gain        ;
     VPSS_DIEFLDMTN.bits.fld_motion_curve_slope = fld_motion_curve_slope ;
     VPSS_DIEFLDMTN.bits.fld_motion_thd_high    = fld_motion_thd_high    ;
     VPSS_DIEFLDMTN.bits.fld_motion_thd_low     = fld_motion_thd_low     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

     return;
}
//addr=0x1040
HI_VOID VPSS_DRV_Set_VPSS_DIEMTNDIFFTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_diff_thd_3 ,HI_U32 motion_diff_thd_2 ,HI_U32 motion_diff_thd_1 ,HI_U32 motion_diff_thd_0 )
{
     U_VPSS_DIEMTNDIFFTHD0 VPSS_DIEMTNDIFFTHD0;

     VPSS_DIEMTNDIFFTHD0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNDIFFTHD0.u32));
     VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_3 = motion_diff_thd_3 ;
     VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_2 = motion_diff_thd_2 ;
     VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_1 = motion_diff_thd_1 ;
     VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_0 = motion_diff_thd_0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNDIFFTHD0.u32), VPSS_DIEMTNDIFFTHD0.u32);

     return;
}
//addr=0x1044
HI_VOID VPSS_DRV_Set_VPSS_DIEMTNDIFFTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 max_motion_iir_ratio ,HI_U32 min_motion_iir_ratio ,HI_U32 motion_diff_thd_5 ,HI_U32 motion_diff_thd_4 )
{
     U_VPSS_DIEMTNDIFFTHD1 VPSS_DIEMTNDIFFTHD1;

     VPSS_DIEMTNDIFFTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNDIFFTHD1.u32));
     VPSS_DIEMTNDIFFTHD1.bits.max_motion_iir_ratio = max_motion_iir_ratio ;
     VPSS_DIEMTNDIFFTHD1.bits.min_motion_iir_ratio = min_motion_iir_ratio ;
     VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_5    = motion_diff_thd_5    ;
     VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_4    = motion_diff_thd_4    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNDIFFTHD1.u32), VPSS_DIEMTNDIFFTHD1.u32);

     return;
}
//addr=0x1048
HI_VOID VPSS_DRV_Set_VPSS_DIEMTNIIRSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_iir_curve_ratio_0 ,HI_U32 motion_iir_curve_slope_3 ,HI_U32 motion_iir_curve_slope_2 ,HI_U32 motion_iir_curve_slope_1 ,HI_U32 motion_iir_curve_slope_0 )
{
     U_VPSS_DIEMTNIIRSLP VPSS_DIEMTNIIRSLP;

     VPSS_DIEMTNIIRSLP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNIIRSLP.u32));
     VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_ratio_0 = motion_iir_curve_ratio_0 ;
     VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_3 = motion_iir_curve_slope_3 ;
     VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_2 = motion_iir_curve_slope_2 ;
     VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_1 = motion_iir_curve_slope_1 ;
     VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_0 = motion_iir_curve_slope_0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNIIRSLP.u32), VPSS_DIEMTNIIRSLP.u32);

     return;
}
//addr=0x104c
HI_VOID VPSS_DRV_Set_VPSS_DIEMTNIIRRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_iir_curve_ratio_4 ,HI_U32 motion_iir_curve_ratio_3 ,HI_U32 motion_iir_curve_ratio_2 ,HI_U32 motion_iir_curve_ratio_1 )
{
     U_VPSS_DIEMTNIIRRAT VPSS_DIEMTNIIRRAT;

     VPSS_DIEMTNIIRRAT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNIIRRAT.u32));
     VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_4 = motion_iir_curve_ratio_4 ;
     VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_3 = motion_iir_curve_ratio_3 ;
     VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_2 = motion_iir_curve_ratio_2 ;
     VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_1 = motion_iir_curve_ratio_1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNIIRRAT.u32), VPSS_DIEMTNIIRRAT.u32);

     return;
}
//addr=0x1050
HI_VOID VPSS_DRV_Set_VPSS_DIEHISMODE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 his_motion_info_write_mode ,HI_U32 his_motion_write_mode ,HI_U32 his_motion_using_mode ,HI_U32 his_motion_en ,HI_U32 pre_info_en ,HI_U32 ppre_info_en ,HI_U32 rec_mode_frm_motion_step_1 ,HI_U32 rec_mode_frm_motion_step_0 ,HI_U32 rec_mode_fld_motion_step_1 ,HI_U32 rec_mode_fld_motion_step_0 )
{
     U_VPSS_DIEHISMODE VPSS_DIEHISMODE;

     VPSS_DIEHISMODE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEHISMODE.u32));
     VPSS_DIEHISMODE.bits.his_motion_info_write_mode = his_motion_info_write_mode ;
     VPSS_DIEHISMODE.bits.his_motion_write_mode      = his_motion_write_mode      ;
     VPSS_DIEHISMODE.bits.his_motion_using_mode      = his_motion_using_mode      ;
     VPSS_DIEHISMODE.bits.his_motion_en              = his_motion_en              ;
     VPSS_DIEHISMODE.bits.pre_info_en                = pre_info_en                ;
     VPSS_DIEHISMODE.bits.ppre_info_en               = ppre_info_en               ;
     VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_1 = rec_mode_frm_motion_step_1 ;
     VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_0 = rec_mode_frm_motion_step_0 ;
     VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_1 = rec_mode_fld_motion_step_1 ;
     VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_0 = rec_mode_fld_motion_step_0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DIEHISMODE_pre_info_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pre_info_en )
{
     U_VPSS_DIEHISMODE VPSS_DIEHISMODE;

     VPSS_DIEHISMODE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEHISMODE.u32));

     VPSS_DIEHISMODE.bits.pre_info_en                = pre_info_en                ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

     return;
}



//addr=0x1054
HI_VOID VPSS_DRV_Set_VPSS_DIEMORFLT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 med_blend_en ,HI_U32 mor_flt_en ,HI_U32 mor_flt_size ,HI_U32 mor_flt_thd )
{
     U_VPSS_DIEMORFLT VPSS_DIEMORFLT;

     VPSS_DIEMORFLT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMORFLT.u32));
     VPSS_DIEMORFLT.bits.med_blend_en = med_blend_en ;
     VPSS_DIEMORFLT.bits.mor_flt_en   = mor_flt_en   ;
     VPSS_DIEMORFLT.bits.mor_flt_size = mor_flt_size ;
     VPSS_DIEMORFLT.bits.mor_flt_thd  = mor_flt_thd  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMORFLT.u32), VPSS_DIEMORFLT.u32);

     return;
}
//addr=0x1058
HI_VOID VPSS_DRV_Set_VPSS_DIECOMBCHK0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 comb_chk_upper_limit ,HI_U32 comb_chk_lower_limit ,HI_U32 comb_chk_min_vthd ,HI_U32 comb_chk_min_hthd )
{
     U_VPSS_DIECOMBCHK0 VPSS_DIECOMBCHK0;

     VPSS_DIECOMBCHK0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECOMBCHK0.u32));
     VPSS_DIECOMBCHK0.bits.comb_chk_upper_limit = comb_chk_upper_limit ;
     VPSS_DIECOMBCHK0.bits.comb_chk_lower_limit = comb_chk_lower_limit ;
     VPSS_DIECOMBCHK0.bits.comb_chk_min_vthd    = comb_chk_min_vthd    ;
     VPSS_DIECOMBCHK0.bits.comb_chk_min_hthd    = comb_chk_min_hthd    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECOMBCHK0.u32), VPSS_DIECOMBCHK0.u32);

     return;
}
//addr=0x105c
HI_VOID VPSS_DRV_Set_VPSS_DIECOMBCHK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 comb_chk_en ,HI_U32 comb_chk_md_thd ,HI_U32 comb_chk_edge_thd )
{
     U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;

     VPSS_DIECOMBCHK1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECOMBCHK1.u32));
     VPSS_DIECOMBCHK1.bits.comb_chk_en       = comb_chk_en       ;
     VPSS_DIECOMBCHK1.bits.comb_chk_md_thd   = comb_chk_md_thd   ;
     VPSS_DIECOMBCHK1.bits.comb_chk_edge_thd = comb_chk_edge_thd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

     return;
}
//addr=0x1060
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_thd3 ,HI_U32 frame_motion_smooth_thd2 ,HI_U32 frame_motion_smooth_thd1 ,HI_U32 frame_motion_smooth_thd0 )
{
     U_VPSS_DIEFRMMTNSMTHTHD0 VPSS_DIEFRMMTNSMTHTHD0;

     VPSS_DIEFRMMTNSMTHTHD0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHTHD0.u32));
     VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd3 = frame_motion_smooth_thd3 ;
     VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd2 = frame_motion_smooth_thd2 ;
     VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd1 = frame_motion_smooth_thd1 ;
     VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd0 = frame_motion_smooth_thd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHTHD0.u32), VPSS_DIEFRMMTNSMTHTHD0.u32);

     return;
}
//addr=0x1064
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio_max ,HI_U32 frame_motion_smooth_ratio_min ,HI_U32 frame_motion_smooth_thd5 ,HI_U32 frame_motion_smooth_thd4 )
{
     U_VPSS_DIEFRMMTNSMTHTHD1 VPSS_DIEFRMMTNSMTHTHD1;

     VPSS_DIEFRMMTNSMTHTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHTHD1.u32));
     VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_max = frame_motion_smooth_ratio_max ;
     VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_min = frame_motion_smooth_ratio_min ;
     VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd5      = frame_motion_smooth_thd5      ;
     VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd4      = frame_motion_smooth_thd4      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHTHD1.u32), VPSS_DIEFRMMTNSMTHTHD1.u32);

     return;
}
//addr=0x1068
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio0 ,HI_U32 frame_motion_smooth_slope3 ,HI_U32 frame_motion_smooth_slope2 ,HI_U32 frame_motion_smooth_slope1 ,HI_U32 frame_motion_smooth_slope0 )
{
     U_VPSS_DIEFRMMTNSMTHSLP VPSS_DIEFRMMTNSMTHSLP;

     VPSS_DIEFRMMTNSMTHSLP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHSLP.u32));
     VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_ratio0 = frame_motion_smooth_ratio0 ;
     VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope3 = frame_motion_smooth_slope3 ;
     VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope2 = frame_motion_smooth_slope2 ;
     VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope1 = frame_motion_smooth_slope1 ;
     VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope0 = frame_motion_smooth_slope0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHSLP.u32), VPSS_DIEFRMMTNSMTHSLP.u32);

     return;
}
//addr=0x106c
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio4 ,HI_U32 frame_motion_smooth_ratio3 ,HI_U32 frame_motion_smooth_ratio2 ,HI_U32 frame_motion_smooth_ratio1 )
{
     U_VPSS_DIEFRMMTNSMTHRAT VPSS_DIEFRMMTNSMTHRAT;

     VPSS_DIEFRMMTNSMTHRAT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHRAT.u32));
     VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio4 = frame_motion_smooth_ratio4 ;
     VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio3 = frame_motion_smooth_ratio3 ;
     VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio2 = frame_motion_smooth_ratio2 ;
     VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio1 = frame_motion_smooth_ratio1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMMTNSMTHRAT.u32), VPSS_DIEFRMMTNSMTHRAT.u32);

     return;
}
//addr=0x1070
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_thd3 ,HI_U32 frame_field_blend_thd2 ,HI_U32 frame_field_blend_thd1 ,HI_U32 frame_field_blend_thd0 )
{
     U_VPSS_DIEFRMFLDBLENDTHD0 VPSS_DIEFRMFLDBLENDTHD0;

     VPSS_DIEFRMFLDBLENDTHD0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDTHD0.u32));
     VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd3 = frame_field_blend_thd3 ;
     VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd2 = frame_field_blend_thd2 ;
     VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd1 = frame_field_blend_thd1 ;
     VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd0 = frame_field_blend_thd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDTHD0.u32), VPSS_DIEFRMFLDBLENDTHD0.u32);

     return;
}
//addr=0x1074
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio_max ,HI_U32 frame_field_blend_ratio_min ,HI_U32 frame_field_blend_thd5 ,HI_U32 frame_field_blend_thd4 )
{
     U_VPSS_DIEFRMFLDBLENDTHD1 VPSS_DIEFRMFLDBLENDTHD1;

     VPSS_DIEFRMFLDBLENDTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDTHD1.u32));
     VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_max = frame_field_blend_ratio_max ;
     VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_min = frame_field_blend_ratio_min ;
     VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd5      = frame_field_blend_thd5      ;
     VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd4      = frame_field_blend_thd4      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDTHD1.u32), VPSS_DIEFRMFLDBLENDTHD1.u32);

     return;
}
//addr=0x1078
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio0 ,HI_U32 frame_field_blend_slope3 ,HI_U32 frame_field_blend_slope2 ,HI_U32 frame_field_blend_slope1 ,HI_U32 frame_field_blend_slope0 )
{
     U_VPSS_DIEFRMFLDBLENDSLP VPSS_DIEFRMFLDBLENDSLP;

     VPSS_DIEFRMFLDBLENDSLP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDSLP.u32));
     VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_ratio0 = frame_field_blend_ratio0 ;
     VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope3 = frame_field_blend_slope3 ;
     VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope2 = frame_field_blend_slope2 ;
     VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope1 = frame_field_blend_slope1 ;
     VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope0 = frame_field_blend_slope0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDSLP.u32), VPSS_DIEFRMFLDBLENDSLP.u32);

     return;
}
//addr=0x107c
HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio4 ,HI_U32 frame_field_blend_ratio3 ,HI_U32 frame_field_blend_ratio2 ,HI_U32 frame_field_blend_ratio1 )
{
     U_VPSS_DIEFRMFLDBLENDRAT VPSS_DIEFRMFLDBLENDRAT;

     VPSS_DIEFRMFLDBLENDRAT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDRAT.u32));
     VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio4 = frame_field_blend_ratio4 ;
     VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio3 = frame_field_blend_ratio3 ;
     VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio2 = frame_field_blend_ratio2 ;
     VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio1 = frame_field_blend_ratio1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFRMFLDBLENDRAT.u32), VPSS_DIEFRMFLDBLENDRAT.u32);

     return;
}
//addr=0x1080
HI_VOID VPSS_DRV_Set_VPSS_DIEMTNADJ(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_adjust_gain_chr ,HI_U32 motion_adjust_coring ,HI_U32 motion_adjust_gain )
{
     U_VPSS_DIEMTNADJ VPSS_DIEMTNADJ;

     VPSS_DIEMTNADJ.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNADJ.u32));
     VPSS_DIEMTNADJ.bits.motion_adjust_gain_chr = motion_adjust_gain_chr ;
     VPSS_DIEMTNADJ.bits.motion_adjust_coring   = motion_adjust_coring   ;
     VPSS_DIEMTNADJ.bits.motion_adjust_gain     = motion_adjust_gain     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMTNADJ.u32), VPSS_DIEMTNADJ.u32);

     return;
}
//addr=0x1084
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge_norm_1 ,HI_U32 edge_norm_0 )
{
     U_VPSS_DIEEDGENORM0 VPSS_DIEEDGENORM0;

     VPSS_DIEEDGENORM0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM0.u32));
     VPSS_DIEEDGENORM0.bits.edge_norm_1 = edge_norm_1 ;
     VPSS_DIEEDGENORM0.bits.edge_norm_0 = edge_norm_0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM0.u32), VPSS_DIEEDGENORM0.u32);

     return;
}
//addr=0x1088
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge_norm_3 ,HI_U32 edge_norm_2 )
{
     U_VPSS_DIEEDGENORM1 VPSS_DIEEDGENORM1;

     VPSS_DIEEDGENORM1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM1.u32));
     VPSS_DIEEDGENORM1.bits.edge_norm_3 = edge_norm_3 ;
     VPSS_DIEEDGENORM1.bits.edge_norm_2 = edge_norm_2 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM1.u32), VPSS_DIEEDGENORM1.u32);

     return;
}
//addr=0x108c
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_k3 ,HI_U32 edge_norm_5 ,HI_U32 edge_norm_4 )
{
     U_VPSS_DIEEDGENORM2 VPSS_DIEEDGENORM2;

     VPSS_DIEEDGENORM2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM2.u32));
     VPSS_DIEEDGENORM2.bits.mc_strength_k3 = mc_strength_k3 ;
     VPSS_DIEEDGENORM2.bits.edge_norm_5    = edge_norm_5    ;
     VPSS_DIEEDGENORM2.bits.edge_norm_4    = edge_norm_4    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM2.u32), VPSS_DIEEDGENORM2.u32);

     return;
}
//addr=0x1090
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_g3 ,HI_U32 edge_norm_7 ,HI_U32 edge_norm_6 )
{
     U_VPSS_DIEEDGENORM3 VPSS_DIEEDGENORM3;

     VPSS_DIEEDGENORM3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM3.u32));
     VPSS_DIEEDGENORM3.bits.mc_strength_g3 = mc_strength_g3 ;
     VPSS_DIEEDGENORM3.bits.edge_norm_7    = edge_norm_7    ;
     VPSS_DIEEDGENORM3.bits.edge_norm_6    = edge_norm_6    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM3.u32), VPSS_DIEEDGENORM3.u32);

     return;
}
//addr=0x1094
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd0 ,HI_U32 edge_norm_9 ,HI_U32 edge_norm_8 )
{
     U_VPSS_DIEEDGENORM4 VPSS_DIEEDGENORM4;

     VPSS_DIEEDGENORM4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM4.u32));
     VPSS_DIEEDGENORM4.bits.inter_diff_thd0 = inter_diff_thd0 ;
     VPSS_DIEEDGENORM4.bits.edge_norm_9     = edge_norm_9     ;
     VPSS_DIEEDGENORM4.bits.edge_norm_8     = edge_norm_8     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM4.u32), VPSS_DIEEDGENORM4.u32);

     return;
}
//addr=0x1098
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd1 ,HI_U32 edge_norm_11 ,HI_U32 edge_norm_10 )
{
     U_VPSS_DIEEDGENORM5 VPSS_DIEEDGENORM5;

     VPSS_DIEEDGENORM5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM5.u32));
     VPSS_DIEEDGENORM5.bits.inter_diff_thd1 = inter_diff_thd1 ;
     VPSS_DIEEDGENORM5.bits.edge_norm_11    = edge_norm_11    ;
     VPSS_DIEEDGENORM5.bits.edge_norm_10    = edge_norm_10    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGENORM5.u32), VPSS_DIEEDGENORM5.u32);

     return;
}
//addr=0x109c
HI_VOID VPSS_DRV_Set_VPSS_DIEEDGEFORMC(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd2 ,HI_U32 edge_scale ,HI_U32 edge_coring )
{
     U_VPSS_DIEEDGEFORMC VPSS_DIEEDGEFORMC;

     VPSS_DIEEDGEFORMC.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGEFORMC.u32));
     VPSS_DIEEDGEFORMC.bits.inter_diff_thd2 = inter_diff_thd2 ;
     VPSS_DIEEDGEFORMC.bits.edge_scale      = edge_scale      ;
     VPSS_DIEEDGEFORMC.bits.edge_coring     = edge_coring     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEEDGEFORMC.u32), VPSS_DIEEDGEFORMC.u32);

     return;
}
//addr=0x10a0
HI_VOID VPSS_DRV_Set_VPSS_DIEMCSTRENGTH0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_g0 ,HI_U32 mc_strength_k2 ,HI_U32 mc_strength_k1 ,HI_U32 mc_strength_k0 )
{
     U_VPSS_DIEMCSTRENGTH0 VPSS_DIEMCSTRENGTH0;

     VPSS_DIEMCSTRENGTH0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCSTRENGTH0.u32));
     VPSS_DIEMCSTRENGTH0.bits.mc_strength_g0 = mc_strength_g0 ;
     VPSS_DIEMCSTRENGTH0.bits.mc_strength_k2 = mc_strength_k2 ;
     VPSS_DIEMCSTRENGTH0.bits.mc_strength_k1 = mc_strength_k1 ;
     VPSS_DIEMCSTRENGTH0.bits.mc_strength_k0 = mc_strength_k0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCSTRENGTH0.u32), VPSS_DIEMCSTRENGTH0.u32);

     return;
}
//addr=0x10a4
HI_VOID VPSS_DRV_Set_VPSS_DIEMCSTRENGTH1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_maxg ,HI_U32 mc_strength_ming ,HI_U32 mc_strength_g2 ,HI_U32 mc_strength_g1 )
{
     U_VPSS_DIEMCSTRENGTH1 VPSS_DIEMCSTRENGTH1;

     VPSS_DIEMCSTRENGTH1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCSTRENGTH1.u32));
     VPSS_DIEMCSTRENGTH1.bits.mc_strength_maxg = mc_strength_maxg ;
     VPSS_DIEMCSTRENGTH1.bits.mc_strength_ming = mc_strength_ming ;
     VPSS_DIEMCSTRENGTH1.bits.mc_strength_g2   = mc_strength_g2   ;
     VPSS_DIEMCSTRENGTH1.bits.mc_strength_g1   = mc_strength_g1   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCSTRENGTH1.u32), VPSS_DIEMCSTRENGTH1.u32);

     return;
}
//addr=0x10a8
HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_c_mcbld ,HI_U32 k_c_mcw ,HI_U32 k_y_mcbld ,HI_U32 k_y_mcw )
{
     U_VPSS_DIEFUSION0 VPSS_DIEFUSION0;

     VPSS_DIEFUSION0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION0.u32));
     VPSS_DIEFUSION0.bits.k_c_mcbld  = k_c_mcbld  ;
     VPSS_DIEFUSION0.bits.k_c_mcw    = k_c_mcw    ;
     VPSS_DIEFUSION0.bits.k_y_mcbld  = k_y_mcbld  ;
     VPSS_DIEFUSION0.bits.k_y_mcw    = k_y_mcw    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION0.u32), VPSS_DIEFUSION0.u32);

     return;
}
//addr=0x10ac
HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_mcw_adj ,HI_U32 k0_mcw_adj ,HI_U32 x0_mcw_adj )
{
     U_VPSS_DIEFUSION1 VPSS_DIEFUSION1;

     VPSS_DIEFUSION1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION1.u32));
     VPSS_DIEFUSION1.bits.g0_mcw_adj = g0_mcw_adj ;
     VPSS_DIEFUSION1.bits.k0_mcw_adj = k0_mcw_adj ;
     VPSS_DIEFUSION1.bits.x0_mcw_adj = x0_mcw_adj ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION1.u32), VPSS_DIEFUSION1.u32);

     return;
}
//addr=0x10b0
HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mcbld ,HI_U32 k0_mcbld ,HI_U32 x0_mcbld ,HI_U32 k1_mcw_adj )
{
     U_VPSS_DIEFUSION2 VPSS_DIEFUSION2;

     VPSS_DIEFUSION2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION2.u32));
     VPSS_DIEFUSION2.bits.k1_mcbld   = k1_mcbld   ;
     VPSS_DIEFUSION2.bits.k0_mcbld   = k0_mcbld   ;
     VPSS_DIEFUSION2.bits.x0_mcbld   = x0_mcbld   ;
     VPSS_DIEFUSION2.bits.k1_mcw_adj = k1_mcw_adj ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION2.u32), VPSS_DIEFUSION2.u32);

     return;
}
//addr=0x10b4
HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_lai_bldmode ,HI_U32 k_curw_mcbld ,HI_U32 g0_mcbld )
{
     U_VPSS_DIEFUSION3 VPSS_DIEFUSION3;

     VPSS_DIEFUSION3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION3.u32));
     VPSS_DIEFUSION3.bits.mc_lai_bldmode = mc_lai_bldmode ;
     VPSS_DIEFUSION3.bits.k_curw_mcbld   = k_curw_mcbld   ;
     VPSS_DIEFUSION3.bits.g0_mcbld       = g0_mcbld       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEFUSION3.u32), VPSS_DIEFUSION3.u32);

     return;
}
//addr=0x10b8
HI_VOID VPSS_DRV_Set_VPSS_DIEMAGBMTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ma_gbm_thd0 ,HI_U32 ma_gbm_thd1 )
{
     U_VPSS_DIEMAGBMTHD0 VPSS_DIEMAGBMTHD0;

     VPSS_DIEMAGBMTHD0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMAGBMTHD0.u32));
     VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd0 = ma_gbm_thd0 ;
     VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd1 = ma_gbm_thd1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMAGBMTHD0.u32), VPSS_DIEMAGBMTHD0.u32);

     return;
}
//addr=0x10bc
HI_VOID VPSS_DRV_Set_VPSS_DIEMAGBMTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ma_gbm_thd2 ,HI_U32 ma_gbm_thd3 )
{
     U_VPSS_DIEMAGBMTHD1 VPSS_DIEMAGBMTHD1;

     VPSS_DIEMAGBMTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMAGBMTHD1.u32));
     VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd2 = ma_gbm_thd2 ;
     VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd3 = ma_gbm_thd3 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMAGBMTHD1.u32), VPSS_DIEMAGBMTHD1.u32);

     return;
}
//addr=0x10c0
HI_VOID VPSS_DRV_Set_VPSS_DIEMCGBMCOEF0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mtfilten_gmd ,HI_U32 mtth3_gmd ,HI_U32 mtth2_gmd ,HI_U32 mtth1_gmd ,HI_U32 mtth0_gmd )
{
     U_VPSS_DIEMCGBMCOEF0 VPSS_DIEMCGBMCOEF0;

     VPSS_DIEMCGBMCOEF0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCGBMCOEF0.u32));
     VPSS_DIEMCGBMCOEF0.bits.mtfilten_gmd = mtfilten_gmd ;
     VPSS_DIEMCGBMCOEF0.bits.mtth3_gmd    = mtth3_gmd    ;
     VPSS_DIEMCGBMCOEF0.bits.mtth2_gmd    = mtth2_gmd    ;
     VPSS_DIEMCGBMCOEF0.bits.mtth1_gmd    = mtth1_gmd    ;
     VPSS_DIEMCGBMCOEF0.bits.mtth0_gmd    = mtth0_gmd    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCGBMCOEF0.u32), VPSS_DIEMCGBMCOEF0.u32);

     return;
}
//addr=0x10c4
HI_VOID VPSS_DRV_Set_VPSS_DIEMCGBMCOEF1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_mag_gmd ,HI_U32 k_difh_gmd ,HI_U32 k_maxmag_gmd )
{
     U_VPSS_DIEMCGBMCOEF1 VPSS_DIEMCGBMCOEF1;

     VPSS_DIEMCGBMCOEF1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCGBMCOEF1.u32));
     VPSS_DIEMCGBMCOEF1.bits.k_mag_gmd    = k_mag_gmd    ;
     VPSS_DIEMCGBMCOEF1.bits.k_difh_gmd   = k_difh_gmd   ;
     VPSS_DIEMCGBMCOEF1.bits.k_maxmag_gmd = k_maxmag_gmd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEMCGBMCOEF1.u32), VPSS_DIEMCGBMCOEF1.u32);

     return;
}
//addr=0x10c8
HI_VOID VPSS_DRV_Set_VPSS_DIEKMAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 khoredge ,HI_U32 kmagv_2 ,HI_U32 kmagv_1 ,HI_U32 kmagh_2 ,HI_U32 kmagh_1 )
{
     U_VPSS_DIEKMAG1 VPSS_DIEKMAG1;

     VPSS_DIEKMAG1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEKMAG1.u32));
     VPSS_DIEKMAG1.bits.khoredge   = khoredge   ;
     VPSS_DIEKMAG1.bits.kmagv_2    = kmagv_2    ;
     VPSS_DIEKMAG1.bits.kmagv_1    = kmagv_1    ;
     VPSS_DIEKMAG1.bits.kmagh_2    = kmagh_2    ;
     VPSS_DIEKMAG1.bits.kmagh_1    = kmagh_1    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEKMAG1.u32), VPSS_DIEKMAG1.u32);

     return;
}
//addr=0x10cc
HI_VOID VPSS_DRV_Set_VPSS_DIEKMAG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 gain_framemotion ,HI_U32 motion_limt_2 ,HI_U32 motion_limt_1 ,HI_U32 frame_mag_en ,HI_U32 gain_horedge )
{
     U_VPSS_DIEKMAG2 VPSS_DIEKMAG2;

     VPSS_DIEKMAG2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEKMAG2.u32));
     VPSS_DIEKMAG2.bits.gain_framemotion = gain_framemotion ;
     VPSS_DIEKMAG2.bits.motion_limt_2    = motion_limt_2    ;
     VPSS_DIEKMAG2.bits.motion_limt_1    = motion_limt_1    ;
     VPSS_DIEKMAG2.bits.frame_mag_en     = frame_mag_en     ;
     VPSS_DIEKMAG2.bits.gain_horedge     = gain_horedge     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DIEKMAG2.u32), VPSS_DIEKMAG2.u32);

     return;
}
//addr=0x10d0
HI_VOID VPSS_DRV_Set_VPSS_PDPHISTTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hist_thd3 ,HI_U32 hist_thd2 ,HI_U32 hist_thd1 ,HI_U32 hist_thd0 )
{
     U_VPSS_PDPHISTTHD1 VPSS_PDPHISTTHD1;

     VPSS_PDPHISTTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPHISTTHD1.u32));
     VPSS_PDPHISTTHD1.bits.hist_thd3 = hist_thd3 ;
     VPSS_PDPHISTTHD1.bits.hist_thd2 = hist_thd2 ;
     VPSS_PDPHISTTHD1.bits.hist_thd1 = hist_thd1 ;
     VPSS_PDPHISTTHD1.bits.hist_thd0 = hist_thd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPHISTTHD1.u32), VPSS_PDPHISTTHD1.u32);

     return;
}
//addr=0x10d4
HI_VOID VPSS_DRV_Set_VPSS_PDPCCMOV(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mov_coring_norm ,HI_U32 mov_coring_tkr ,HI_U32 mov_coring_blk )
{
     U_VPSS_PDPCCMOV VPSS_PDPCCMOV;

     VPSS_PDPCCMOV.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCMOV.u32));
     VPSS_PDPCCMOV.bits.mov_coring_norm = mov_coring_norm ;
     VPSS_PDPCCMOV.bits.mov_coring_tkr  = mov_coring_tkr  ;
     VPSS_PDPCCMOV.bits.mov_coring_blk  = mov_coring_blk  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCMOV.u32), VPSS_PDPCCMOV.u32);

     return;
}
//addr=0x10d8
HI_VOID VPSS_DRV_Set_VPSS_PDCTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_mode ,HI_U32 bitsmov2r )
{
     U_VPSS_PDCTRL VPSS_PDCTRL;

     VPSS_PDCTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDCTRL.u32));
     VPSS_PDCTRL.bits.lasi_mode  = lasi_mode  ;
     VPSS_PDCTRL.bits.bitsmov2r  = bitsmov2r  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDCTRL.u32), VPSS_PDCTRL.u32);

     return;
}
//addr=0x10dc
HI_VOID VPSS_DRV_Set_VPSS_PDBLKTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 diff_movblk_thd )
{
     U_VPSS_PDBLKTHD VPSS_PDBLKTHD;

     VPSS_PDBLKTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDBLKTHD.u32));
     VPSS_PDBLKTHD.bits.diff_movblk_thd = diff_movblk_thd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDBLKTHD.u32), VPSS_PDBLKTHD.u32);

     return;
}
//addr=0x10e0
HI_VOID VPSS_DRV_Set_VPSS_PDUMTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 um_thd2 ,HI_U32 um_thd1 ,HI_U32 um_thd0 )
{
     U_VPSS_PDUMTHD VPSS_PDUMTHD;

     VPSS_PDUMTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDUMTHD.u32));
     VPSS_PDUMTHD.bits.um_thd2    = um_thd2    ;
     VPSS_PDUMTHD.bits.um_thd1    = um_thd1    ;
     VPSS_PDUMTHD.bits.um_thd0    = um_thd0    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDUMTHD.u32), VPSS_PDUMTHD.u32);

     return;
}
//addr=0x10e4
HI_VOID VPSS_DRV_Set_VPSS_PDPCCCORING(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 coring_blk ,HI_U32 coring_norm ,HI_U32 coring_tkr )
{
     U_VPSS_PDPCCCORING VPSS_PDPCCCORING;

     VPSS_PDPCCCORING.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCCORING.u32));
     VPSS_PDPCCCORING.bits.coring_blk  = coring_blk  ;
     VPSS_PDPCCCORING.bits.coring_norm = coring_norm ;
     VPSS_PDPCCCORING.bits.coring_tkr  = coring_tkr  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCCORING.u32), VPSS_PDPCCCORING.u32);

     return;
}
//addr=0x10e8
HI_VOID VPSS_DRV_Set_VPSS_PDPCCHTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pcc_hthd )
{
     U_VPSS_PDPCCHTHD VPSS_PDPCCHTHD;

     VPSS_PDPCCHTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCHTHD.u32));
     VPSS_PDPCCHTHD.bits.pcc_hthd   = pcc_hthd   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCHTHD.u32), VPSS_PDPCCHTHD.u32);

     return;
}
//addr=0x10ec
HI_VOID VPSS_DRV_Set_VPSS_PDPCCVTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pcc_vthd3 ,HI_U32 pcc_vthd2 ,HI_U32 pcc_vthd1 ,HI_U32 pcc_vthd0 )
{
     U_VPSS_PDPCCVTHD VPSS_PDPCCVTHD;

     VPSS_PDPCCVTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCVTHD.u32));
     VPSS_PDPCCVTHD.bits.pcc_vthd3 = pcc_vthd3 ;
     VPSS_PDPCCVTHD.bits.pcc_vthd2 = pcc_vthd2 ;
     VPSS_PDPCCVTHD.bits.pcc_vthd1 = pcc_vthd1 ;
     VPSS_PDPCCVTHD.bits.pcc_vthd0 = pcc_vthd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDPCCVTHD.u32), VPSS_PDPCCVTHD.u32);

     return;
}
//addr=0x10f0
HI_VOID VPSS_DRV_Set_VPSS_PDITDIFFVTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 itdiff_vthd3 ,HI_U32 itdiff_vthd2 ,HI_U32 itdiff_vthd1 ,HI_U32 itdiff_vthd0 )
{
     U_VPSS_PDITDIFFVTHD VPSS_PDITDIFFVTHD;

     VPSS_PDITDIFFVTHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDITDIFFVTHD.u32));
     VPSS_PDITDIFFVTHD.bits.itdiff_vthd3 = itdiff_vthd3 ;
     VPSS_PDITDIFFVTHD.bits.itdiff_vthd2 = itdiff_vthd2 ;
     VPSS_PDITDIFFVTHD.bits.itdiff_vthd1 = itdiff_vthd1 ;
     VPSS_PDITDIFFVTHD.bits.itdiff_vthd0 = itdiff_vthd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDITDIFFVTHD.u32), VPSS_PDITDIFFVTHD.u32);

     return;
}
//addr=0x10f4
HI_VOID VPSS_DRV_Set_VPSS_PDLASITHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_mov_thd ,HI_U32 lasi_edge_thd ,HI_U32 lasi_coring_thd )
{
     U_VPSS_PDLASITHD VPSS_PDLASITHD;

     VPSS_PDLASITHD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASITHD.u32));
     VPSS_PDLASITHD.bits.lasi_mov_thd    = lasi_mov_thd    ;
     VPSS_PDLASITHD.bits.lasi_edge_thd   = lasi_edge_thd   ;
     VPSS_PDLASITHD.bits.lasi_coring_thd = lasi_coring_thd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASITHD.u32), VPSS_PDLASITHD.u32);

     return;
}
//addr=0x10f8
HI_VOID VPSS_DRV_Set_VPSS_PDLASIMODE0COEF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_txt_coring ,HI_U32 lasi_txt_alpha )
{
     U_VPSS_PDLASIMODE0COEF VPSS_PDLASIMODE0COEF;

     VPSS_PDLASIMODE0COEF.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASIMODE0COEF.u32));
     VPSS_PDLASIMODE0COEF.bits.lasi_txt_coring = lasi_txt_coring ;
     VPSS_PDLASIMODE0COEF.bits.lasi_txt_alpha  = lasi_txt_alpha  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASIMODE0COEF.u32), VPSS_PDLASIMODE0COEF.u32);

     return;
}
//addr=0x10fc
HI_VOID VPSS_DRV_Set_VPSS_PDLASIMODE1COEF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_txt_thd3 ,HI_U32 lasi_txt_thd2 ,HI_U32 lasi_txt_thd1 ,HI_U32 lasi_txt_thd0 )
{
     U_VPSS_PDLASIMODE1COEF VPSS_PDLASIMODE1COEF;

     VPSS_PDLASIMODE1COEF.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASIMODE1COEF.u32));
     VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd3 = lasi_txt_thd3 ;
     VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd2 = lasi_txt_thd2 ;
     VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd1 = lasi_txt_thd1 ;
     VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd0 = lasi_txt_thd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDLASIMODE1COEF.u32), VPSS_PDLASIMODE1COEF.u32);

     return;
}
//addr=0x1100
HI_VOID VPSS_DRV_Set_VPSS_PDREGION(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 region1_y_stt ,HI_U32 region1_y_end )
{
     U_VPSS_PDREGION VPSS_PDREGION;

     VPSS_PDREGION.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_PDREGION.u32));
     VPSS_PDREGION.bits.region1_y_stt = region1_y_stt ;
     VPSS_PDREGION.bits.region1_y_end = region1_y_end ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_PDREGION.u32), VPSS_PDREGION.u32);

     return;
}
//addr=0x1104
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGDIFY(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rgdifycore ,HI_U32 g_rgdifycore ,HI_U32 core_rgdify ,HI_U32 lmt_rgdify )
{
     U_VPSS_MCDI_RGDIFY VPSS_MCDI_RGDIFY;

     VPSS_MCDI_RGDIFY.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGDIFY.u32));
     VPSS_MCDI_RGDIFY.bits.k_rgdifycore = k_rgdifycore ;
     VPSS_MCDI_RGDIFY.bits.g_rgdifycore = g_rgdifycore ;
     VPSS_MCDI_RGDIFY.bits.core_rgdify  = core_rgdify  ;
     VPSS_MCDI_RGDIFY.bits.lmt_rgdify   = lmt_rgdify   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGDIFY.u32), VPSS_MCDI_RGDIFY.u32);

     return;
}
//addr=0x1108
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGSAD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 coef_sadlpf ,HI_U32 kmv_rgsad ,HI_U32 k_tpdif_rgsad ,HI_U32 g_tpdif_rgsad )
{
     U_VPSS_MCDI_RGSAD VPSS_MCDI_RGSAD;

     VPSS_MCDI_RGSAD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGSAD.u32));
     VPSS_MCDI_RGSAD.bits.coef_sadlpf   = coef_sadlpf   ;
     VPSS_MCDI_RGSAD.bits.kmv_rgsad     = kmv_rgsad     ;
     VPSS_MCDI_RGSAD.bits.k_tpdif_rgsad = k_tpdif_rgsad ;
     VPSS_MCDI_RGSAD.bits.g_tpdif_rgsad = g_tpdif_rgsad ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGSAD.u32), VPSS_MCDI_RGSAD.u32);

     return;
}
//addr=0x110c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMV(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 thmag_rgmv ,HI_U32 th_saddif_rgmv ,HI_U32 th_0mvsad_rgmv )
{
     U_VPSS_MCDI_RGMV VPSS_MCDI_RGMV;

     VPSS_MCDI_RGMV.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMV.u32));
     VPSS_MCDI_RGMV.bits.thmag_rgmv     = thmag_rgmv     ;
     VPSS_MCDI_RGMV.bits.th_saddif_rgmv = th_saddif_rgmv ;
     VPSS_MCDI_RGMV.bits.th_0mvsad_rgmv = th_0mvsad_rgmv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMV.u32), VPSS_MCDI_RGMV.u32);

     return;
}
//addr=0x1110
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMAG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_mag_rg ,HI_U32 lmt_mag_rg )
{
     U_VPSS_MCDI_RGMAG0 VPSS_MCDI_RGMAG0;

     VPSS_MCDI_RGMAG0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMAG0.u32));
     VPSS_MCDI_RGMAG0.bits.core_mag_rg = core_mag_rg ;
     VPSS_MCDI_RGMAG0.bits.lmt_mag_rg  = lmt_mag_rg  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMAG0.u32), VPSS_MCDI_RGMAG0.u32);

     return;
}
//addr=0x1114
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_mv_rgmvls ,HI_U32 k_mv_rgmvls ,HI_U32 core_mag_rgmvls ,HI_U32 k_mag_rgmvls ,HI_U32 th_mvadj_rgmvls ,HI_U32 en_mvadj_rgmvls )
{
     U_VPSS_MCDI_RGMAG1 VPSS_MCDI_RGMAG1;

     VPSS_MCDI_RGMAG1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMAG1.u32));
     VPSS_MCDI_RGMAG1.bits.core_mv_rgmvls  = core_mv_rgmvls  ;
     VPSS_MCDI_RGMAG1.bits.k_mv_rgmvls     = k_mv_rgmvls     ;
     VPSS_MCDI_RGMAG1.bits.core_mag_rgmvls = core_mag_rgmvls ;
     VPSS_MCDI_RGMAG1.bits.k_mag_rgmvls    = k_mag_rgmvls    ;
     VPSS_MCDI_RGMAG1.bits.th_mvadj_rgmvls = th_mvadj_rgmvls ;
     VPSS_MCDI_RGMAG1.bits.en_mvadj_rgmvls = en_mvadj_rgmvls ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMAG1.u32), VPSS_MCDI_RGMAG1.u32);

     return;
}
//addr=0x1118
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGLS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_sad_rgls ,HI_U32 th_mag_rgls ,HI_U32 th_sad_rgls ,HI_U32 k_sadcore_rgmv )
{
     U_VPSS_MCDI_RGLS VPSS_MCDI_RGLS;

     VPSS_MCDI_RGLS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGLS.u32));
     VPSS_MCDI_RGLS.bits.k_sad_rgls     = k_sad_rgls     ;
     VPSS_MCDI_RGLS.bits.th_mag_rgls    = th_mag_rgls    ;
     VPSS_MCDI_RGLS.bits.th_sad_rgls    = th_sad_rgls    ;
     VPSS_MCDI_RGLS.bits.k_sadcore_rgmv = k_sadcore_rgmv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGLS.u32), VPSS_MCDI_RGLS.u32);

     return;
}
//addr=0x111c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_SEL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 force_mven ,HI_U32 force_mvx )
{
     U_VPSS_MCDI_SEL VPSS_MCDI_SEL;

     VPSS_MCDI_SEL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_SEL.u32));
     VPSS_MCDI_SEL.bits.force_mven = force_mven ;
     VPSS_MCDI_SEL.bits.force_mvx  = force_mvx  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_SEL.u32), VPSS_MCDI_SEL.u32);

     return;
}
//addr=0x1120
HI_VOID VPSS_DRV_Set_VPSS_MCDI_DLT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 th_blkmvx_mvdlt ,HI_U32 th_rgmvx_mvdlt ,HI_U32 th_ls_mvdlt ,HI_U32 th_vblkdist_mvdlt ,HI_U32 th_hblkdist_mvdlt )
{
     U_VPSS_MCDI_DLT0 VPSS_MCDI_DLT0;

     VPSS_MCDI_DLT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DLT0.u32));
     VPSS_MCDI_DLT0.bits.th_blkmvx_mvdlt   = th_blkmvx_mvdlt   ;
     VPSS_MCDI_DLT0.bits.th_rgmvx_mvdlt    = th_rgmvx_mvdlt    ;
     VPSS_MCDI_DLT0.bits.th_ls_mvdlt       = th_ls_mvdlt       ;
     VPSS_MCDI_DLT0.bits.th_vblkdist_mvdlt = th_vblkdist_mvdlt ;
     VPSS_MCDI_DLT0.bits.th_hblkdist_mvdlt = th_hblkdist_mvdlt ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DLT0.u32), VPSS_MCDI_DLT0.u32);

     return;
}
//addr=0x1124
HI_VOID VPSS_DRV_Set_VPSS_MCDI_DLT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_sadcore_mvdlt ,HI_U32 th_mag_mvdlt ,HI_U32 g_mag_mvdlt ,HI_U32 thl_sad_mvdlt ,HI_U32 thh_sad_mvdlt )
{
     U_VPSS_MCDI_DLT1 VPSS_MCDI_DLT1;

     VPSS_MCDI_DLT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DLT1.u32));
     VPSS_MCDI_DLT1.bits.k_sadcore_mvdlt = k_sadcore_mvdlt ;
     VPSS_MCDI_DLT1.bits.th_mag_mvdlt    = th_mag_mvdlt    ;
     VPSS_MCDI_DLT1.bits.g_mag_mvdlt     = g_mag_mvdlt     ;
     VPSS_MCDI_DLT1.bits.thl_sad_mvdlt   = thl_sad_mvdlt   ;
     VPSS_MCDI_DLT1.bits.thh_sad_mvdlt   = thh_sad_mvdlt   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DLT1.u32), VPSS_MCDI_DLT1.u32);

     return;
}
//addr=0x1128
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rglsw ,HI_U32 k_simimvw ,HI_U32 gh_core_simimv ,HI_U32 gl_core_simimv ,HI_U32 k_core_simimv )
{
     U_VPSS_MCDI_RGMCW0 VPSS_MCDI_RGMCW0;

     VPSS_MCDI_RGMCW0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW0.u32));
     VPSS_MCDI_RGMCW0.bits.k_rglsw        = k_rglsw        ;
     VPSS_MCDI_RGMCW0.bits.k_simimvw      = k_simimvw      ;
     VPSS_MCDI_RGMCW0.bits.gh_core_simimv = gh_core_simimv ;
     VPSS_MCDI_RGMCW0.bits.gl_core_simimv = gl_core_simimv ;
     VPSS_MCDI_RGMCW0.bits.k_core_simimv  = k_core_simimv  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW0.u32), VPSS_MCDI_RGMCW0.u32);

     return;
}
//addr=0x112c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_core_vsaddif ,HI_U32 k_rgsadadj_mcw ,HI_U32 core_rgsadadj_mcw ,HI_U32 k_mvy_mcw ,HI_U32 core_mvy_mcw ,HI_U32 rgtb_en_mcw )
{
     U_VPSS_MCDI_RGMCW1 VPSS_MCDI_RGMCW1;

     VPSS_MCDI_RGMCW1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW1.u32));
     VPSS_MCDI_RGMCW1.bits.k_core_vsaddif    = k_core_vsaddif    ;
     VPSS_MCDI_RGMCW1.bits.k_rgsadadj_mcw    = k_rgsadadj_mcw    ;
     VPSS_MCDI_RGMCW1.bits.core_rgsadadj_mcw = core_rgsadadj_mcw ;
     VPSS_MCDI_RGMCW1.bits.k_mvy_mcw         = k_mvy_mcw         ;
     VPSS_MCDI_RGMCW1.bits.core_mvy_mcw      = core_mvy_mcw      ;
     VPSS_MCDI_RGMCW1.bits.rgtb_en_mcw       = rgtb_en_mcw       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW1.u32), VPSS_MCDI_RGMCW1.u32);

     return;
}
//addr=0x1130
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_rgmag_mcw ,HI_U32 mode_rgysad_mcw ,HI_U32 k_vsaddifw ,HI_U32 gh_core_vsad_dif ,HI_U32 gl_core_vsaddif )
{
     U_VPSS_MCDI_RGMCW2 VPSS_MCDI_RGMCW2;

     VPSS_MCDI_RGMCW2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW2.u32));
     VPSS_MCDI_RGMCW2.bits.core_rgmag_mcw   = core_rgmag_mcw   ;
     VPSS_MCDI_RGMCW2.bits.mode_rgysad_mcw  = mode_rgysad_mcw  ;
     VPSS_MCDI_RGMCW2.bits.k_vsaddifw       = k_vsaddifw       ;
     VPSS_MCDI_RGMCW2.bits.gh_core_vsad_dif = gh_core_vsad_dif ;
     VPSS_MCDI_RGMCW2.bits.gl_core_vsaddif  = gl_core_vsaddif  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW2.u32), VPSS_MCDI_RGMCW2.u32);

     return;
}
//addr=0x1134
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_rgmag_mcw ,HI_U32 k0_rgmag_mcw ,HI_U32 x0_rgmag_mcw )
{
     U_VPSS_MCDI_RGMCW3 VPSS_MCDI_RGMCW3;

     VPSS_MCDI_RGMCW3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW3.u32));
     VPSS_MCDI_RGMCW3.bits.g0_rgmag_mcw = g0_rgmag_mcw ;
     VPSS_MCDI_RGMCW3.bits.k0_rgmag_mcw = k0_rgmag_mcw ;
     VPSS_MCDI_RGMCW3.bits.x0_rgmag_mcw = x0_rgmag_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW3.u32), VPSS_MCDI_RGMCW3.u32);

     return;
}
//addr=0x1138
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 x0_rgsad_mcw ,HI_U32 core_rgsad_mcw ,HI_U32 k1_rgmag_mcw )
{
     U_VPSS_MCDI_RGMCW4 VPSS_MCDI_RGMCW4;

     VPSS_MCDI_RGMCW4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW4.u32));
     VPSS_MCDI_RGMCW4.bits.x0_rgsad_mcw   = x0_rgsad_mcw   ;
     VPSS_MCDI_RGMCW4.bits.core_rgsad_mcw = core_rgsad_mcw ;
     VPSS_MCDI_RGMCW4.bits.k1_rgmag_mcw   = k1_rgmag_mcw   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW4.u32), VPSS_MCDI_RGMCW4.u32);

     return;
}
//addr=0x113c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_rgsad_mcw ,HI_U32 g0_rgsad_mcw ,HI_U32 k0_rgsad_mcw )
{
     U_VPSS_MCDI_RGMCW5 VPSS_MCDI_RGMCW5;

     VPSS_MCDI_RGMCW5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW5.u32));
     VPSS_MCDI_RGMCW5.bits.k1_rgsad_mcw = k1_rgsad_mcw ;
     VPSS_MCDI_RGMCW5.bits.g0_rgsad_mcw = g0_rgsad_mcw ;
     VPSS_MCDI_RGMCW5.bits.k0_rgsad_mcw = k0_rgsad_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW5.u32), VPSS_MCDI_RGMCW5.u32);

     return;
}
//addr=0x1140
HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rgsad_mcw ,HI_U32 x_rgsad_mcw ,HI_U32 k0_smrg_mcw ,HI_U32 x0_smrg_mcw )
{
     U_VPSS_MCDI_RGMCW6 VPSS_MCDI_RGMCW6;

     VPSS_MCDI_RGMCW6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW6.u32));
     VPSS_MCDI_RGMCW6.bits.k_rgsad_mcw = k_rgsad_mcw ;
     VPSS_MCDI_RGMCW6.bits.x_rgsad_mcw = x_rgsad_mcw ;
     VPSS_MCDI_RGMCW6.bits.k0_smrg_mcw = k0_smrg_mcw ;
     VPSS_MCDI_RGMCW6.bits.x0_smrg_mcw = x0_smrg_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_RGMCW6.u32), VPSS_MCDI_RGMCW6.u32);

     return;
}
//addr=0x1144
HI_VOID VPSS_DRV_Set_VPSS_MCDI_TPMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_tpmvdist_mcw ,HI_U32 g0_tpmvdist_mcw ,HI_U32 k0_tpmvdist_mcw ,HI_U32 x0_tpmvdist_mcw )
{
     U_VPSS_MCDI_TPMCW0 VPSS_MCDI_TPMCW0;

     VPSS_MCDI_TPMCW0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_TPMCW0.u32));
     VPSS_MCDI_TPMCW0.bits.k1_tpmvdist_mcw = k1_tpmvdist_mcw ;
     VPSS_MCDI_TPMCW0.bits.g0_tpmvdist_mcw = g0_tpmvdist_mcw ;
     VPSS_MCDI_TPMCW0.bits.k0_tpmvdist_mcw = k0_tpmvdist_mcw ;
     VPSS_MCDI_TPMCW0.bits.x0_tpmvdist_mcw = x0_tpmvdist_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_TPMCW0.u32), VPSS_MCDI_TPMCW0.u32);

     return;
}
//addr=0x1148
HI_VOID VPSS_DRV_Set_VPSS_MCDI_TPMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_core_tpmvdist_mcw ,HI_U32 b_core_tpmvdist_mcw ,HI_U32 k_avgmvdist_mcw ,HI_U32 k_minmvdist_mcw )
{
     U_VPSS_MCDI_TPMCW1 VPSS_MCDI_TPMCW1;

     VPSS_MCDI_TPMCW1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_TPMCW1.u32));
     VPSS_MCDI_TPMCW1.bits.k_core_tpmvdist_mcw = k_core_tpmvdist_mcw ;
     VPSS_MCDI_TPMCW1.bits.b_core_tpmvdist_mcw = b_core_tpmvdist_mcw ;
     VPSS_MCDI_TPMCW1.bits.k_avgmvdist_mcw     = k_avgmvdist_mcw     ;
     VPSS_MCDI_TPMCW1.bits.k_minmvdist_mcw     = k_minmvdist_mcw     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_TPMCW1.u32), VPSS_MCDI_TPMCW1.u32);

     return;
}
//addr=0x114c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_tbdif_mcw ,HI_U32 k0_max_mag_mcw ,HI_U32 k1_max_mag_mcw ,HI_U32 k_max_dif_mcw ,HI_U32 k_max_core_mcw ,HI_U32 k_difvcore_mcw ,HI_U32 k_difhcore_mcw )
{
     U_VPSS_MCDI_WNDMCW0 VPSS_MCDI_WNDMCW0;

     VPSS_MCDI_WNDMCW0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW0.u32));
     VPSS_MCDI_WNDMCW0.bits.k_tbdif_mcw    = k_tbdif_mcw    ;
     VPSS_MCDI_WNDMCW0.bits.k0_max_mag_mcw = k0_max_mag_mcw ;
     VPSS_MCDI_WNDMCW0.bits.k1_max_mag_mcw = k1_max_mag_mcw ;
     VPSS_MCDI_WNDMCW0.bits.k_max_dif_mcw  = k_max_dif_mcw  ;
     VPSS_MCDI_WNDMCW0.bits.k_max_core_mcw = k_max_core_mcw ;
     VPSS_MCDI_WNDMCW0.bits.k_difvcore_mcw = k_difvcore_mcw ;
     VPSS_MCDI_WNDMCW0.bits.k_difhcore_mcw = k_difhcore_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW0.u32), VPSS_MCDI_WNDMCW0.u32);

     return;
}
//addr=0x1150
HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mag_wnd_mcw ,HI_U32 g0_mag_wnd_mcw ,HI_U32 k0_mag_wnd_mcw ,HI_U32 x0_mag_wnd_mcw ,HI_U32 k_tbmag_mcw )
{
     U_VPSS_MCDI_WNDMCW1 VPSS_MCDI_WNDMCW1;

     VPSS_MCDI_WNDMCW1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW1.u32));
     VPSS_MCDI_WNDMCW1.bits.k1_mag_wnd_mcw = k1_mag_wnd_mcw ;
     VPSS_MCDI_WNDMCW1.bits.g0_mag_wnd_mcw = g0_mag_wnd_mcw ;
     VPSS_MCDI_WNDMCW1.bits.k0_mag_wnd_mcw = k0_mag_wnd_mcw ;
     VPSS_MCDI_WNDMCW1.bits.x0_mag_wnd_mcw = x0_mag_wnd_mcw ;
     VPSS_MCDI_WNDMCW1.bits.k_tbmag_mcw    = k_tbmag_mcw    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW1.u32), VPSS_MCDI_WNDMCW1.u32);

     return;
}
//addr=0x1154
HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_sad_wnd_mcw ,HI_U32 k0_sad_wnd_mcw ,HI_U32 x0_sad_wnd_mcw ,HI_U32 g1_mag_wnd_mcw )
{
     U_VPSS_MCDI_WNDMCW2 VPSS_MCDI_WNDMCW2;

     VPSS_MCDI_WNDMCW2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW2.u32));
     VPSS_MCDI_WNDMCW2.bits.g0_sad_wnd_mcw = g0_sad_wnd_mcw ;
     VPSS_MCDI_WNDMCW2.bits.k0_sad_wnd_mcw = k0_sad_wnd_mcw ;
     VPSS_MCDI_WNDMCW2.bits.x0_sad_wnd_mcw = x0_sad_wnd_mcw ;
     VPSS_MCDI_WNDMCW2.bits.g1_mag_wnd_mcw = g1_mag_wnd_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW2.u32), VPSS_MCDI_WNDMCW2.u32);

     return;
}
//addr=0x1158
HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g1_sad_wnd_mcw ,HI_U32 k1_sad_wnd_mcw )
{
     U_VPSS_MCDI_WNDMCW3 VPSS_MCDI_WNDMCW3;

     VPSS_MCDI_WNDMCW3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW3.u32));
     VPSS_MCDI_WNDMCW3.bits.g1_sad_wnd_mcw = g1_sad_wnd_mcw ;
     VPSS_MCDI_WNDMCW3.bits.k1_sad_wnd_mcw = k1_sad_wnd_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_WNDMCW3.u32), VPSS_MCDI_WNDMCW3.u32);

     return;
}
//addr=0x115c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_hvdif_dw ,HI_U32 b_bhvdif_dw ,HI_U32 k_bhvdif_dw ,HI_U32 core_bhvdif_dw ,HI_U32 gain_lpf_dw ,HI_U32 k_max_hvdif_dw )
{
     U_VPSS_MCDI_VERTWEIGHT0 VPSS_MCDI_VERTWEIGHT0;

     VPSS_MCDI_VERTWEIGHT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT0.u32));
     VPSS_MCDI_VERTWEIGHT0.bits.b_hvdif_dw     = b_hvdif_dw     ;
     VPSS_MCDI_VERTWEIGHT0.bits.b_bhvdif_dw    = b_bhvdif_dw    ;
     VPSS_MCDI_VERTWEIGHT0.bits.k_bhvdif_dw    = k_bhvdif_dw    ;
     VPSS_MCDI_VERTWEIGHT0.bits.core_bhvdif_dw = core_bhvdif_dw ;
     VPSS_MCDI_VERTWEIGHT0.bits.gain_lpf_dw    = gain_lpf_dw    ;
     VPSS_MCDI_VERTWEIGHT0.bits.k_max_hvdif_dw = k_max_hvdif_dw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT0.u32), VPSS_MCDI_VERTWEIGHT0.u32);

     return;
}
//addr=0x1160
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_mv_dw ,HI_U32 core_mv_dw ,HI_U32 k_difv_dw ,HI_U32 core_hvdif_dw )
{
     U_VPSS_MCDI_VERTWEIGHT1 VPSS_MCDI_VERTWEIGHT1;

     VPSS_MCDI_VERTWEIGHT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT1.u32));
     VPSS_MCDI_VERTWEIGHT1.bits.b_mv_dw       = b_mv_dw       ;
     VPSS_MCDI_VERTWEIGHT1.bits.core_mv_dw    = core_mv_dw    ;
     VPSS_MCDI_VERTWEIGHT1.bits.k_difv_dw     = k_difv_dw     ;
     VPSS_MCDI_VERTWEIGHT1.bits.core_hvdif_dw = core_hvdif_dw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT1.u32), VPSS_MCDI_VERTWEIGHT1.u32);

     return;
}
//addr=0x1164
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_hvdif_dw ,HI_U32 g0_hvdif_dw ,HI_U32 k0_hvdif_dw ,HI_U32 x0_hvdif_dw )
{
     U_VPSS_MCDI_VERTWEIGHT2 VPSS_MCDI_VERTWEIGHT2;

     VPSS_MCDI_VERTWEIGHT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT2.u32));
     VPSS_MCDI_VERTWEIGHT2.bits.k1_hvdif_dw = k1_hvdif_dw ;
     VPSS_MCDI_VERTWEIGHT2.bits.g0_hvdif_dw = g0_hvdif_dw ;
     VPSS_MCDI_VERTWEIGHT2.bits.k0_hvdif_dw = k0_hvdif_dw ;
     VPSS_MCDI_VERTWEIGHT2.bits.x0_hvdif_dw = x0_hvdif_dw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT2.u32), VPSS_MCDI_VERTWEIGHT2.u32);

     return;
}
//addr=0x1168
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mv_dw ,HI_U32 g0_mv_dw ,HI_U32 k0_mv_dw ,HI_U32 x0_mv_dw )
{
     U_VPSS_MCDI_VERTWEIGHT3 VPSS_MCDI_VERTWEIGHT3;

     VPSS_MCDI_VERTWEIGHT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT3.u32));
     VPSS_MCDI_VERTWEIGHT3.bits.k1_mv_dw   = k1_mv_dw   ;
     VPSS_MCDI_VERTWEIGHT3.bits.g0_mv_dw   = g0_mv_dw   ;
     VPSS_MCDI_VERTWEIGHT3.bits.k0_mv_dw   = k0_mv_dw   ;
     VPSS_MCDI_VERTWEIGHT3.bits.x0_mv_dw   = x0_mv_dw   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT3.u32), VPSS_MCDI_VERTWEIGHT3.u32);

     return;
}
//addr=0x116c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mt_dw ,HI_U32 g0_mt_dw ,HI_U32 k0_mt_dw ,HI_U32 x0_mt_dw )
{
     U_VPSS_MCDI_VERTWEIGHT4 VPSS_MCDI_VERTWEIGHT4;

     VPSS_MCDI_VERTWEIGHT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT4.u32));
     VPSS_MCDI_VERTWEIGHT4.bits.k1_mt_dw   = k1_mt_dw   ;
     VPSS_MCDI_VERTWEIGHT4.bits.g0_mt_dw   = g0_mt_dw   ;
     VPSS_MCDI_VERTWEIGHT4.bits.k0_mt_dw   = k0_mt_dw   ;
     VPSS_MCDI_VERTWEIGHT4.bits.x0_mt_dw   = x0_mt_dw   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT4.u32), VPSS_MCDI_VERTWEIGHT4.u32);

     return;
}
//addr=0x1170
HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_mt_dw ,HI_U32 k1_mv_mt ,HI_U32 x0_mv_mt ,HI_U32 g0_mv_mt )
{
     U_VPSS_MCDI_VERTWEIGHT5 VPSS_MCDI_VERTWEIGHT5;

     VPSS_MCDI_VERTWEIGHT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT5.u32));
     VPSS_MCDI_VERTWEIGHT5.bits.b_mt_dw    = b_mt_dw    ;
     VPSS_MCDI_VERTWEIGHT5.bits.k1_mv_mt   = k1_mv_mt   ;
     VPSS_MCDI_VERTWEIGHT5.bits.x0_mv_mt   = x0_mv_mt   ;
     VPSS_MCDI_VERTWEIGHT5.bits.g0_mv_mt   = g0_mv_mt   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_VERTWEIGHT5.u32), VPSS_MCDI_VERTWEIGHT5.u32);

     return;
}
//addr=0x1174
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mclpf_mode ,HI_U32 k_pxlmag_mcw ,HI_U32 x_pxlmag_mcw ,HI_U32 rs_pxlmag_mcw ,HI_U32 gain_mclpfh ,HI_U32 gain_dn_mclpfv ,HI_U32 gain_up_mclpfv )
{
     U_VPSS_MCDI_MC0 VPSS_MCDI_MC0;

     VPSS_MCDI_MC0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC0.u32));
     VPSS_MCDI_MC0.bits.mclpf_mode     = mclpf_mode     ;
     VPSS_MCDI_MC0.bits.k_pxlmag_mcw   = k_pxlmag_mcw   ;
     VPSS_MCDI_MC0.bits.x_pxlmag_mcw   = x_pxlmag_mcw   ;
     VPSS_MCDI_MC0.bits.rs_pxlmag_mcw  = rs_pxlmag_mcw  ;
     VPSS_MCDI_MC0.bits.gain_mclpfh    = gain_mclpfh    ;
     VPSS_MCDI_MC0.bits.gain_dn_mclpfv = gain_dn_mclpfv ;
     VPSS_MCDI_MC0.bits.gain_up_mclpfv = gain_up_mclpfv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC0.u32), VPSS_MCDI_MC0.u32);

     return;
}
//addr=0x1178
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g_pxlmag_mcw )
{
     U_VPSS_MCDI_MC1 VPSS_MCDI_MC1;

     VPSS_MCDI_MC1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC1.u32));
     VPSS_MCDI_MC1.bits.g_pxlmag_mcw = g_pxlmag_mcw ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC1.u32), VPSS_MCDI_MC1.u32);

     return;
}
//addr=0x117c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_c_vertw ,HI_U32 k_y_vertw )
{
     U_VPSS_MCDI_MC2 VPSS_MCDI_MC2;

     VPSS_MCDI_MC2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC2.u32));
     VPSS_MCDI_MC2.bits.k_c_vertw  = k_c_vertw  ;
     VPSS_MCDI_MC2.bits.k_y_vertw  = k_y_vertw  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC2.u32), VPSS_MCDI_MC2.u32);

     return;
}
//addr=0x1180
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_fstmt_mc ,HI_U32 x_fstmt_mc ,HI_U32 k1_mv_mc ,HI_U32 x0_mv_mc ,HI_U32 bdv_mcpos ,HI_U32 bdh_mcpos ,HI_U32 k_delta )
{
     U_VPSS_MCDI_MC3 VPSS_MCDI_MC3;

     VPSS_MCDI_MC3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC3.u32));
     VPSS_MCDI_MC3.bits.k_fstmt_mc = k_fstmt_mc ;
     VPSS_MCDI_MC3.bits.x_fstmt_mc = x_fstmt_mc ;
     VPSS_MCDI_MC3.bits.k1_mv_mc   = k1_mv_mc   ;
     VPSS_MCDI_MC3.bits.x0_mv_mc   = x0_mv_mc   ;
     VPSS_MCDI_MC3.bits.bdv_mcpos  = bdv_mcpos  ;
     VPSS_MCDI_MC3.bits.bdh_mcpos  = bdh_mcpos  ;
     VPSS_MCDI_MC3.bits.k_delta    = k_delta    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC3.u32), VPSS_MCDI_MC3.u32);

     return;
}
//addr=0x1184
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_hfcore_mc ,HI_U32 x_hfcore_mc ,HI_U32 g_slmt_mc ,HI_U32 k_slmt_mc ,HI_U32 x_slmt_mc ,HI_U32 g_fstmt_mc )
{
     U_VPSS_MCDI_MC4 VPSS_MCDI_MC4;

     VPSS_MCDI_MC4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC4.u32));
     VPSS_MCDI_MC4.bits.k_hfcore_mc = k_hfcore_mc ;
     VPSS_MCDI_MC4.bits.x_hfcore_mc = x_hfcore_mc ;
     VPSS_MCDI_MC4.bits.g_slmt_mc   = g_slmt_mc   ;
     VPSS_MCDI_MC4.bits.k_slmt_mc   = k_slmt_mc   ;
     VPSS_MCDI_MC4.bits.x_slmt_mc   = x_slmt_mc   ;
     VPSS_MCDI_MC4.bits.g_fstmt_mc  = g_fstmt_mc  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC4.u32), VPSS_MCDI_MC4.u32);

     return;
}
//addr=0x1188
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 r0_mc ,HI_U32 c0_mc ,HI_U32 g_hfcore_mc )
{
     U_VPSS_MCDI_MC5 VPSS_MCDI_MC5;

     VPSS_MCDI_MC5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC5.u32));
     VPSS_MCDI_MC5.bits.r0_mc       = r0_mc       ;
     VPSS_MCDI_MC5.bits.c0_mc       = c0_mc       ;
     VPSS_MCDI_MC5.bits.g_hfcore_mc = g_hfcore_mc ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC5.u32), VPSS_MCDI_MC5.u32);

     return;
}
//addr=0x118c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcmvrange ,HI_U32 r1_mc ,HI_U32 c1_mc )
{
     U_VPSS_MCDI_MC6 VPSS_MCDI_MC6;

     VPSS_MCDI_MC6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC6.u32));
     VPSS_MCDI_MC6.bits.mcmvrange  = mcmvrange  ;
     VPSS_MCDI_MC6.bits.r1_mc      = r1_mc      ;
     VPSS_MCDI_MC6.bits.c1_mc      = c1_mc      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC6.u32), VPSS_MCDI_MC6.u32);

     return;
}
//addr=0x1190
HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_frcount_mc ,HI_U32 x_frcount_mc ,HI_U32 scenechange_mc )
{
     U_VPSS_MCDI_MC7 VPSS_MCDI_MC7;

     VPSS_MCDI_MC7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC7.u32));
     VPSS_MCDI_MC7.bits.k_frcount_mc   = k_frcount_mc   ;
     VPSS_MCDI_MC7.bits.x_frcount_mc   = x_frcount_mc   ;
     VPSS_MCDI_MC7.bits.scenechange_mc = scenechange_mc ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_MC7.u32), VPSS_MCDI_MC7.u32);

     return;
}
//addr=0x1194
HI_VOID VPSS_DRV_Set_VPSS_MCDI_LCDINEW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcendc ,HI_U32 mcendr ,HI_U32 mcstartc ,HI_U32 mcstartr )
{
     U_VPSS_MCDI_LCDINEW0 VPSS_MCDI_LCDINEW0;

     VPSS_MCDI_LCDINEW0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_LCDINEW0.u32));
     VPSS_MCDI_LCDINEW0.bits.mcendc   = mcendc   ;
     VPSS_MCDI_LCDINEW0.bits.mcendr   = mcendr   ;
     VPSS_MCDI_LCDINEW0.bits.mcstartc = mcstartc ;
     VPSS_MCDI_LCDINEW0.bits.mcstartr = mcstartr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_LCDINEW0.u32), VPSS_MCDI_LCDINEW0.u32);

     return;
}
//addr=0x1198
HI_VOID VPSS_DRV_Set_VPSS_MCDI_LCDINEW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 movegain ,HI_U32 movecorig ,HI_U32 movethdl ,HI_U32 movethdh )
{
     U_VPSS_MCDI_LCDINEW1 VPSS_MCDI_LCDINEW1;

     VPSS_MCDI_LCDINEW1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_LCDINEW1.u32));
     VPSS_MCDI_LCDINEW1.bits.movegain   = movegain   ;
     VPSS_MCDI_LCDINEW1.bits.movecorig  = movecorig  ;
     VPSS_MCDI_LCDINEW1.bits.movethdl   = movethdl   ;
     VPSS_MCDI_LCDINEW1.bits.movethdh   = movethdh   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_LCDINEW1.u32), VPSS_MCDI_LCDINEW1.u32);

     return;
}
//addr=0x119c
HI_VOID VPSS_DRV_Set_VPSS_MCDI_NUMT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_numt_blden ,HI_U32 numt_gain ,HI_U32 numt_coring ,HI_U32 numt_lpf_en )
{
     U_VPSS_MCDI_NUMT VPSS_MCDI_NUMT;

     VPSS_MCDI_NUMT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_NUMT.u32));
     VPSS_MCDI_NUMT.bits.mc_numt_blden = mc_numt_blden ;
     VPSS_MCDI_NUMT.bits.numt_gain     = numt_gain     ;
     VPSS_MCDI_NUMT.bits.numt_coring   = numt_coring   ;
     VPSS_MCDI_NUMT.bits.numt_lpf_en   = numt_lpf_en   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_NUMT.u32), VPSS_MCDI_NUMT.u32);

     return;
}
//addr=0x11a0
HI_VOID VPSS_DRV_Set_VPSS_MCDI_DEMO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 demo_border ,HI_U32 demo_mode_r ,HI_U32 demo_mode_l ,HI_U32 demo_en )
{
     U_VPSS_MCDI_DEMO VPSS_MCDI_DEMO;

     VPSS_MCDI_DEMO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DEMO.u32));
     VPSS_MCDI_DEMO.bits.demo_border = demo_border ;
     VPSS_MCDI_DEMO.bits.demo_mode_r = demo_mode_r ;
     VPSS_MCDI_DEMO.bits.demo_mode_l = demo_mode_l ;
     VPSS_MCDI_DEMO.bits.demo_en     = demo_en     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_MCDI_DEMO.u32), VPSS_MCDI_DEMO.u32);

     return;
}
//addr=0x2000
HI_VOID VPSS_DRV_Set_VPSS_VHD0_HSP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hlmsc_en ,HI_U32 hchmsc_en ,HI_U32 hlmid_en ,HI_U32 hchmid_en ,HI_U32 hlfir_en ,HI_U32 hchfir_en ,HI_U32 hfir_order ,HI_U32 hratio )
{
     U_VPSS_VHD0_HSP VPSS_VHD0_HSP;

     VPSS_VHD0_HSP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HSP.u32));
     VPSS_VHD0_HSP.bits.hlmsc_en   = hlmsc_en   ;
     VPSS_VHD0_HSP.bits.hchmsc_en  = hchmsc_en  ;
     VPSS_VHD0_HSP.bits.hlmid_en   = hlmid_en   ;
     VPSS_VHD0_HSP.bits.hchmid_en  = hchmid_en  ;
     VPSS_VHD0_HSP.bits.hlfir_en   = hlfir_en   ;
     VPSS_VHD0_HSP.bits.hchfir_en  = hchfir_en  ;
     VPSS_VHD0_HSP.bits.hfir_order = hfir_order ;
     VPSS_VHD0_HSP.bits.hratio     = hratio     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32);

     return;
}
//addr=0x2004
HI_VOID VPSS_DRV_Set_VPSS_VHD0_HLOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hor_loffset )
{
     U_VPSS_VHD0_HLOFFSET VPSS_VHD0_HLOFFSET;

     VPSS_VHD0_HLOFFSET.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HLOFFSET.u32));
     VPSS_VHD0_HLOFFSET.bits.hor_loffset = hor_loffset ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HLOFFSET.u32), VPSS_VHD0_HLOFFSET.u32);

     return;
}
//addr=0x2008
HI_VOID VPSS_DRV_Set_VPSS_VHD0_HCOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hor_coffset )
{
     U_VPSS_VHD0_HCOFFSET VPSS_VHD0_HCOFFSET;

     VPSS_VHD0_HCOFFSET.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HCOFFSET.u32));
     VPSS_VHD0_HCOFFSET.bits.hor_coffset = hor_coffset ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_HCOFFSET.u32), VPSS_VHD0_HCOFFSET.u32);

     return;
}
//addr=0x200c
HI_VOID VPSS_DRV_Set_VPSS_VHD0_VSP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vlmsc_en ,HI_U32 vchmsc_en ,HI_U32 vlmid_en ,HI_U32 vchmid_en ,HI_U32 vlfir_en ,HI_U32 vchfir_en ,HI_U32 zme_out_fmt )
{
     U_VPSS_VHD0_VSP VPSS_VHD0_VSP;

     VPSS_VHD0_VSP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VSP.u32));
     VPSS_VHD0_VSP.bits.vlmsc_en    = vlmsc_en    ;
     VPSS_VHD0_VSP.bits.vchmsc_en   = vchmsc_en   ;
     VPSS_VHD0_VSP.bits.vlmid_en    = vlmid_en    ;
     VPSS_VHD0_VSP.bits.vchmid_en   = vchmid_en   ;
     VPSS_VHD0_VSP.bits.vlfir_en    = vlfir_en    ;
     VPSS_VHD0_VSP.bits.vchfir_en   = vchfir_en   ;
     VPSS_VHD0_VSP.bits.zme_out_fmt = zme_out_fmt ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32);

     return;
}
//addr=0x2010
HI_VOID VPSS_DRV_Set_VPSS_VHD0_VSR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vratio )
{
     U_VPSS_VHD0_VSR VPSS_VHD0_VSR;

     VPSS_VHD0_VSR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VSR.u32));
     VPSS_VHD0_VSR.bits.vratio     = vratio     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VSR.u32), VPSS_VHD0_VSR.u32);

     return;
}
//addr=0x2014
HI_VOID VPSS_DRV_Set_VPSS_VHD0_VOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vluma_offset ,HI_U32 vchroma_offset )
{
     U_VPSS_VHD0_VOFFSET VPSS_VHD0_VOFFSET;

     VPSS_VHD0_VOFFSET.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VOFFSET.u32));
     VPSS_VHD0_VOFFSET.bits.vluma_offset   = vluma_offset   ;
     VPSS_VHD0_VOFFSET.bits.vchroma_offset = vchroma_offset ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_VHD0_VOFFSET.u32), VPSS_VHD0_VOFFSET.u32);

     return;
}
//addr=0x2100
HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbm_out_mode ,HI_U32 det_hy_en ,HI_U32 dbm_demo_mode ,HI_U32 dbm_demo_en ,HI_U32 db_lum_hor_en ,HI_U32 nr_en ,HI_U32 dm_en ,HI_U32 db_en )
{
     U_VPSS_DB_CTRL VPSS_DB_CTRL;

     VPSS_DB_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32));
     VPSS_DB_CTRL.bits.dbm_out_mode  = dbm_out_mode  ;
     VPSS_DB_CTRL.bits.det_hy_en     = det_hy_en     ;
     VPSS_DB_CTRL.bits.dbm_demo_mode = dbm_demo_mode ;
     VPSS_DB_CTRL.bits.dbm_demo_en   = dbm_demo_en   ;
     VPSS_DB_CTRL.bits.db_lum_hor_en = db_lum_hor_en ;
     VPSS_DB_CTRL.bits.nr_en         = nr_en         ;
     VPSS_DB_CTRL.bits.dm_en         = dm_en         ;
     VPSS_DB_CTRL.bits.db_en         = db_en         ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_db(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 db_en )
{
     U_VPSS_DB_CTRL VPSS_DB_CTRL;

     VPSS_DB_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32));

     VPSS_DB_CTRL.bits.db_en         = db_en         ;

     if (!db_en)
     {
         VPSS_DB_CTRL.bits.det_hy_en = 0;
     }

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_dm(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_en )
{
     U_VPSS_DB_CTRL VPSS_DB_CTRL;

     VPSS_DB_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32));

     VPSS_DB_CTRL.bits.dm_en         = dm_en         ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_nr(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nr_en )
{
     U_VPSS_DB_CTRL VPSS_DB_CTRL;

     VPSS_DB_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32));

     VPSS_DB_CTRL.bits.nr_en         = nr_en         ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32);

     return;
}


//addr=0x2104
HI_VOID VPSS_DRV_Set_VPSS_DBM_DEMO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbm_demo_pos_x )
{
     U_VPSS_DBM_DEMO VPSS_DBM_DEMO;

     VPSS_DBM_DEMO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBM_DEMO.u32));
     VPSS_DBM_DEMO.bits.dbm_demo_pos_x = dbm_demo_pos_x ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBM_DEMO.u32), VPSS_DBM_DEMO.u32);

     return;
}
//addr=0x2108
HI_VOID VPSS_DRV_Set_VPSS_DB_DIR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_ctrst_thresh ,HI_U32 db_grad_sub_ratio )
{
     U_VPSS_DB_DIR VPSS_DB_DIR;

     VPSS_DB_DIR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_DIR.u32));
     VPSS_DB_DIR.bits.db_ctrst_thresh   = db_ctrst_thresh   ;
     VPSS_DB_DIR.bits.db_grad_sub_ratio = db_grad_sub_ratio ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_DIR.u32), VPSS_DB_DIR.u32);

     return;
}
//addr=0x210c
HI_VOID VPSS_DRV_Set_VPSS_DB_BLK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_blk_size )
{
     U_VPSS_DB_BLK VPSS_DB_BLK;

     VPSS_DB_BLK.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BLK.u32));
     VPSS_DB_BLK.bits.db_lum_h_blk_size = db_lum_h_blk_size ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BLK.u32), VPSS_DB_BLK.u32);

     return;
}
//addr=0x2110
HI_VOID VPSS_DRV_Set_VPSS_DB_RATIO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_txt_win_size ,HI_U32 db_global_db_strenth_lum ,HI_U32 db_lum_hor_filter_sel ,HI_U32 db_lum_hor_scale_ratio )
{
     U_VPSS_DB_RATIO VPSS_DB_RATIO;

     VPSS_DB_RATIO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_RATIO.u32));
     VPSS_DB_RATIO.bits.db_lum_hor_txt_win_size  = db_lum_hor_txt_win_size  ;
     VPSS_DB_RATIO.bits.db_global_db_strenth_lum = db_global_db_strenth_lum ;
     VPSS_DB_RATIO.bits.db_lum_hor_filter_sel    = db_lum_hor_filter_sel    ;
     VPSS_DB_RATIO.bits.db_lum_hor_scale_ratio   = db_lum_hor_scale_ratio   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_RATIO.u32), VPSS_DB_RATIO.u32);

     return;
}
//addr=0x2114
HI_VOID VPSS_DRV_Set_VPSS_DB_LHHF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_hf_var_gain2 ,HI_U32 db_lum_hor_hf_var_gain1 ,HI_U32 db_lum_hor_hf_var_core ,HI_U32 db_lum_hor_hf_diff_gain2 ,HI_U32 db_lum_hor_hf_diff_gain1 ,HI_U32 db_lum_hor_hf_diff_core )
{
     U_VPSS_DB_LHHF VPSS_DB_LHHF;

     VPSS_DB_LHHF.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LHHF.u32));
     VPSS_DB_LHHF.bits.db_lum_hor_hf_var_gain2  = db_lum_hor_hf_var_gain2  ;
     VPSS_DB_LHHF.bits.db_lum_hor_hf_var_gain1  = db_lum_hor_hf_var_gain1  ;
     VPSS_DB_LHHF.bits.db_lum_hor_hf_var_core   = db_lum_hor_hf_var_core   ;
     VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_gain2 = db_lum_hor_hf_diff_gain2 ;
     VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_gain1 = db_lum_hor_hf_diff_gain1 ;
     VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_core  = db_lum_hor_hf_diff_core  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LHHF.u32), VPSS_DB_LHHF.u32);

     return;
}
//addr=0x2118
HI_VOID VPSS_DRV_Set_VPSS_DB_LVHF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_bord_adj_gain ,HI_U32 db_lum_hor_adj_gain )
{
     U_VPSS_DB_LVHF VPSS_DB_LVHF;

     VPSS_DB_LVHF.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LVHF.u32));
     VPSS_DB_LVHF.bits.db_lum_hor_bord_adj_gain = db_lum_hor_bord_adj_gain ;
     VPSS_DB_LVHF.bits.db_lum_hor_adj_gain      = db_lum_hor_adj_gain      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LVHF.u32), VPSS_DB_LVHF.u32);

     return;
}
//addr=0x211c
HI_VOID VPSS_DRV_Set_VPSS_DB_CTRS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_max_lum_hor_db_dist ,HI_U32 db_ctrst_adj_gain2 ,HI_U32 db_ctrst_adj_gain1 ,HI_U32 db_dir_smooth_mode ,HI_U32 db_ctrst_adj_core )
{
     U_VPSS_DB_CTRS VPSS_DB_CTRS;

     VPSS_DB_CTRS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRS.u32));
     VPSS_DB_CTRS.bits.db_max_lum_hor_db_dist = db_max_lum_hor_db_dist ;
     VPSS_DB_CTRS.bits.db_ctrst_adj_gain2     = db_ctrst_adj_gain2     ;
     VPSS_DB_CTRS.bits.db_ctrst_adj_gain1     = db_ctrst_adj_gain1     ;
     VPSS_DB_CTRS.bits.db_dir_smooth_mode     = db_dir_smooth_mode     ;
     VPSS_DB_CTRS.bits.db_ctrst_adj_core      = db_ctrst_adj_core      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_CTRS.u32), VPSS_DB_CTRS.u32);

     return;
}
//addr=0x2120
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_delta_lut_p7 ,HI_U32 db_lum_hor_delta_lut_p6 ,HI_U32 db_lum_hor_delta_lut_p5 ,HI_U32 db_lum_hor_delta_lut_p4 ,HI_U32 db_lum_hor_delta_lut_p3 ,HI_U32 db_lum_hor_delta_lut_p2 ,HI_U32 db_lum_hor_delta_lut_p1 ,HI_U32 db_lum_hor_delta_lut_p0 )
{
     U_VPSS_DB_LUT0 VPSS_DB_LUT0;

     VPSS_DB_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT0.u32));
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p7 = db_lum_hor_delta_lut_p7 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p6 = db_lum_hor_delta_lut_p6 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p5 = db_lum_hor_delta_lut_p5 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p4 = db_lum_hor_delta_lut_p4 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p3 = db_lum_hor_delta_lut_p3 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p2 = db_lum_hor_delta_lut_p2 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p1 = db_lum_hor_delta_lut_p1 ;
     VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p0 = db_lum_hor_delta_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT0.u32), VPSS_DB_LUT0.u32);

     return;
}
//addr=0x2124
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_delta_lut_p15 ,HI_U32 db_lum_hor_delta_lut_p14 ,HI_U32 db_lum_hor_delta_lut_p13 ,HI_U32 db_lum_hor_delta_lut_p12 ,HI_U32 db_lum_hor_delta_lut_p11 ,HI_U32 db_lum_hor_delta_lut_p10 ,HI_U32 db_lum_hor_delta_lut_p9 ,HI_U32 db_lum_hor_delta_lut_p8 )
{
     U_VPSS_DB_LUT1 VPSS_DB_LUT1;

     VPSS_DB_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT1.u32));
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p15 = db_lum_hor_delta_lut_p15 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p14 = db_lum_hor_delta_lut_p14 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p13 = db_lum_hor_delta_lut_p13 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p12 = db_lum_hor_delta_lut_p12 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p11 = db_lum_hor_delta_lut_p11 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p10 = db_lum_hor_delta_lut_p10 ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p9  = db_lum_hor_delta_lut_p9  ;
     VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p8  = db_lum_hor_delta_lut_p8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT1.u32), VPSS_DB_LUT1.u32);

     return;
}
//addr=0x2128
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_str_fade_lut_p9 ,HI_U32 db_lum_h_str_fade_lut_p8 ,HI_U32 db_lum_h_str_fade_lut_p7 ,HI_U32 db_lum_h_str_fade_lut_p6 ,HI_U32 db_lum_h_str_fade_lut_p5 ,HI_U32 db_lum_h_str_fade_lut_p4 ,HI_U32 db_lum_h_str_fade_lut_p3 ,HI_U32 db_lum_h_str_fade_lut_p2 ,HI_U32 db_lum_h_str_fade_lut_p1 ,HI_U32 db_lum_h_str_fade_lut_p0 )
{
     U_VPSS_DB_LUT2 VPSS_DB_LUT2;

     VPSS_DB_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT2.u32));
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p9 = db_lum_h_str_fade_lut_p9 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p8 = db_lum_h_str_fade_lut_p8 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p7 = db_lum_h_str_fade_lut_p7 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p6 = db_lum_h_str_fade_lut_p6 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p5 = db_lum_h_str_fade_lut_p5 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p4 = db_lum_h_str_fade_lut_p4 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p3 = db_lum_h_str_fade_lut_p3 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p2 = db_lum_h_str_fade_lut_p2 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p1 = db_lum_h_str_fade_lut_p1 ;
     VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p0 = db_lum_h_str_fade_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT2.u32), VPSS_DB_LUT2.u32);

     return;
}
//addr=0x212c
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_str_fade_lut_p11 ,HI_U32 db_lum_h_str_fade_lut_p10 )
{
     U_VPSS_DB_LUT3 VPSS_DB_LUT3;

     VPSS_DB_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT3.u32));
     VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p11 = db_lum_h_str_fade_lut_p11 ;
     VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p10 = db_lum_h_str_fade_lut_p10 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT3.u32), VPSS_DB_LUT3.u32);

     return;
}
//addr=0x2130
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_gain_lut_p7 ,HI_U32 db_dir_str_gain_lut_p6 ,HI_U32 db_dir_str_gain_lut_p5 ,HI_U32 db_dir_str_gain_lut_p4 ,HI_U32 db_dir_str_gain_lut_p3 ,HI_U32 db_dir_str_gain_lut_p2 ,HI_U32 db_dir_str_gain_lut_p1 ,HI_U32 db_dir_str_gain_lut_p0 )
{
     U_VPSS_DB_LUT4 VPSS_DB_LUT4;

     VPSS_DB_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT4.u32));
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p7 = db_dir_str_gain_lut_p7 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p6 = db_dir_str_gain_lut_p6 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p5 = db_dir_str_gain_lut_p5 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p4 = db_dir_str_gain_lut_p4 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p3 = db_dir_str_gain_lut_p3 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p2 = db_dir_str_gain_lut_p2 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p1 = db_dir_str_gain_lut_p1 ;
     VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p0 = db_dir_str_gain_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT4.u32), VPSS_DB_LUT4.u32);

     return;
}
//addr=0x2134
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_lut_p7 ,HI_U32 db_dir_str_lut_p6 ,HI_U32 db_dir_str_lut_p5 ,HI_U32 db_dir_str_lut_p4 ,HI_U32 db_dir_str_lut_p3 ,HI_U32 db_dir_str_lut_p2 ,HI_U32 db_dir_str_lut_p1 ,HI_U32 db_dir_str_lut_p0 )
{
     U_VPSS_DB_LUT5 VPSS_DB_LUT5;

     VPSS_DB_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT5.u32));
     VPSS_DB_LUT5.bits.db_dir_str_lut_p7 = db_dir_str_lut_p7 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p6 = db_dir_str_lut_p6 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p5 = db_dir_str_lut_p5 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p4 = db_dir_str_lut_p4 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p3 = db_dir_str_lut_p3 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p2 = db_dir_str_lut_p2 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p1 = db_dir_str_lut_p1 ;
     VPSS_DB_LUT5.bits.db_dir_str_lut_p0 = db_dir_str_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT5.u32), VPSS_DB_LUT5.u32);

     return;
}
//addr=0x2138
HI_VOID VPSS_DRV_Set_VPSS_DB_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_lut_p15 ,HI_U32 db_dir_str_lut_p14 ,HI_U32 db_dir_str_lut_p13 ,HI_U32 db_dir_str_lut_p12 ,HI_U32 db_dir_str_lut_p11 ,HI_U32 db_dir_str_lut_p10 ,HI_U32 db_dir_str_lut_p9 ,HI_U32 db_dir_str_lut_p8 )
{
     U_VPSS_DB_LUT6 VPSS_DB_LUT6;

     VPSS_DB_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT6.u32));
     VPSS_DB_LUT6.bits.db_dir_str_lut_p15 = db_dir_str_lut_p15 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p14 = db_dir_str_lut_p14 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p13 = db_dir_str_lut_p13 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p12 = db_dir_str_lut_p12 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p11 = db_dir_str_lut_p11 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p10 = db_dir_str_lut_p10 ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p9  = db_dir_str_lut_p9  ;
     VPSS_DB_LUT6.bits.db_dir_str_lut_p8  = db_dir_str_lut_p8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_LUT6.u32), VPSS_DB_LUT6.u32);

     return;
}
//addr=0x2150
HI_VOID VPSS_DRV_Set_VPSS_DBD_THDEDGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_thd_edge )
{
     U_VPSS_DBD_THDEDGE VPSS_DBD_THDEDGE;

     VPSS_DBD_THDEDGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_THDEDGE.u32));
     VPSS_DBD_THDEDGE.bits.dbd_hy_thd_edge = dbd_hy_thd_edge ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_THDEDGE.u32), VPSS_DBD_THDEDGE.u32);

     return;
}
//addr=0x2154
HI_VOID VPSS_DRV_Set_VPSS_DBD_THDTXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_thd_txt )
{
     U_VPSS_DBD_THDTXT VPSS_DBD_THDTXT;

     VPSS_DBD_THDTXT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_THDTXT.u32));
     VPSS_DBD_THDTXT.bits.dbd_hy_thd_txt = dbd_hy_thd_txt ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_THDTXT.u32), VPSS_DBD_THDTXT.u32);

     return;
}
//addr=0x2158
HI_VOID VPSS_DRV_Set_VPSS_DBD_LUTWGT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_det_lut_wgt7 ,HI_U32 dbd_det_lut_wgt6 ,HI_U32 dbd_det_lut_wgt5 ,HI_U32 dbd_det_lut_wgt4 ,HI_U32 dbd_det_lut_wgt3 ,HI_U32 dbd_det_lut_wgt2 ,HI_U32 dbd_det_lut_wgt1 ,HI_U32 dbd_det_lut_wgt0 )
{
     U_VPSS_DBD_LUTWGT0 VPSS_DBD_LUTWGT0;

     VPSS_DBD_LUTWGT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_LUTWGT0.u32));
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt7 = dbd_det_lut_wgt7 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt6 = dbd_det_lut_wgt6 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt5 = dbd_det_lut_wgt5 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt4 = dbd_det_lut_wgt4 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt3 = dbd_det_lut_wgt3 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt2 = dbd_det_lut_wgt2 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt1 = dbd_det_lut_wgt1 ;
     VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt0 = dbd_det_lut_wgt0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_LUTWGT0.u32), VPSS_DBD_LUTWGT0.u32);

     return;
}
//addr=0x215c
HI_VOID VPSS_DRV_Set_VPSS_DBD_LUTWGT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_det_lut_wgt15 ,HI_U32 dbd_det_lut_wgt14 ,HI_U32 dbd_det_lut_wgt13 ,HI_U32 dbd_det_lut_wgt12 ,HI_U32 dbd_det_lut_wgt11 ,HI_U32 dbd_det_lut_wgt10 ,HI_U32 dbd_det_lut_wgt9 ,HI_U32 dbd_det_lut_wgt8 )
{
     U_VPSS_DBD_LUTWGT1 VPSS_DBD_LUTWGT1;

     VPSS_DBD_LUTWGT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_LUTWGT1.u32));
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt15 = dbd_det_lut_wgt15 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt14 = dbd_det_lut_wgt14 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt13 = dbd_det_lut_wgt13 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt12 = dbd_det_lut_wgt12 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt11 = dbd_det_lut_wgt11 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt10 = dbd_det_lut_wgt10 ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt9  = dbd_det_lut_wgt9  ;
     VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt8  = dbd_det_lut_wgt8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_LUTWGT1.u32), VPSS_DBD_LUTWGT1.u32);

     return;
}
//addr=0x2160
HI_VOID VPSS_DRV_Set_VPSS_DBD_RTNBDTXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rtn_bd_txt )
{
     U_VPSS_DBD_RTNBDTXT VPSS_DBD_RTNBDTXT;

     VPSS_DBD_RTNBDTXT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_RTNBDTXT.u32));
     VPSS_DBD_RTNBDTXT.bits.dbd_hy_rtn_bd_txt = dbd_hy_rtn_bd_txt ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_RTNBDTXT.u32), VPSS_DBD_RTNBDTXT.u32);

     return;
}
//addr=0x2164
HI_VOID VPSS_DRV_Set_VPSS_DBD_TSTBLKNUM(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_tst_blk_num )
{
     U_VPSS_DBD_TSTBLKNUM VPSS_DBD_TSTBLKNUM;

     VPSS_DBD_TSTBLKNUM.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_TSTBLKNUM.u32));
     VPSS_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num = dbd_hy_tst_blk_num ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_TSTBLKNUM.u32), VPSS_DBD_TSTBLKNUM.u32);

     return;
}
//addr=0x2168
HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut1 ,HI_U32 dbd_hy_rdx8bin_lut0 )
{
     U_VPSS_DBD_HYRDX8BINLUT0 VPSS_DBD_HYRDX8BINLUT0;

     VPSS_DBD_HYRDX8BINLUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT0.u32));
     VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut1 = dbd_hy_rdx8bin_lut1 ;
     VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut0 = dbd_hy_rdx8bin_lut0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT0.u32), VPSS_DBD_HYRDX8BINLUT0.u32);

     return;
}
//addr=0x216c
HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut3 ,HI_U32 dbd_hy_rdx8bin_lut2 )
{
     U_VPSS_DBD_HYRDX8BINLUT1 VPSS_DBD_HYRDX8BINLUT1;

     VPSS_DBD_HYRDX8BINLUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT1.u32));
     VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut3 = dbd_hy_rdx8bin_lut3 ;
     VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut2 = dbd_hy_rdx8bin_lut2 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT1.u32), VPSS_DBD_HYRDX8BINLUT1.u32);

     return;
}
//addr=0x2170
HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut5 ,HI_U32 dbd_hy_rdx8bin_lut4 )
{
     U_VPSS_DBD_HYRDX8BINLUT2 VPSS_DBD_HYRDX8BINLUT2;

     VPSS_DBD_HYRDX8BINLUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT2.u32));
     VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut5 = dbd_hy_rdx8bin_lut5 ;
     VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut4 = dbd_hy_rdx8bin_lut4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT2.u32), VPSS_DBD_HYRDX8BINLUT2.u32);

     return;
}
//addr=0x2174
HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut6 )
{
     U_VPSS_DBD_HYRDX8BINLUT3 VPSS_DBD_HYRDX8BINLUT3;

     VPSS_DBD_HYRDX8BINLUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT3.u32));
     VPSS_DBD_HYRDX8BINLUT3.bits.dbd_hy_rdx8bin_lut6 = dbd_hy_rdx8bin_lut6 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_HYRDX8BINLUT3.u32), VPSS_DBD_HYRDX8BINLUT3.u32);

     return;
}
//addr=0x2178
HI_VOID VPSS_DRV_Set_VPSS_DBD_BLKSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_max_blk_size ,HI_U32 dbd_min_blk_size )
{
     U_VPSS_DBD_BLKSIZE VPSS_DBD_BLKSIZE;

     VPSS_DBD_BLKSIZE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_BLKSIZE.u32));
     VPSS_DBD_BLKSIZE.bits.dbd_hy_max_blk_size = dbd_hy_max_blk_size ;
     VPSS_DBD_BLKSIZE.bits.dbd_min_blk_size    = dbd_min_blk_size    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_BLKSIZE.u32), VPSS_DBD_BLKSIZE.u32);

     return;
}
//addr=0x217c
HI_VOID VPSS_DRV_Set_VPSS_DBD_FLAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_thr_flat )
{
     U_VPSS_DBD_FLAT VPSS_DBD_FLAT;

     VPSS_DBD_FLAT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_FLAT.u32));
     VPSS_DBD_FLAT.bits.dbd_thr_flat = dbd_thr_flat ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DBD_FLAT.u32), VPSS_DBD_FLAT.u32);

     return;
}
//addr=0x2190
HI_VOID VPSS_DRV_Set_VPSS_DM_DIR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_opp_ang_ctrst_div ,HI_U32 dm_opp_ang_ctrst_t ,HI_U32 dm_ctrst_thresh ,HI_U32 dm_grad_sub_ratio )
{
     U_VPSS_DM_DIR VPSS_DM_DIR;

     VPSS_DM_DIR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIR.u32));
     VPSS_DM_DIR.bits.dm_opp_ang_ctrst_div = dm_opp_ang_ctrst_div ;
     VPSS_DM_DIR.bits.dm_opp_ang_ctrst_t   = dm_opp_ang_ctrst_t   ;
     VPSS_DM_DIR.bits.dm_ctrst_thresh      = dm_ctrst_thresh      ;
     VPSS_DM_DIR.bits.dm_grad_sub_ratio    = dm_grad_sub_ratio    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIR.u32), VPSS_DM_DIR.u32);

     return;
}
//addr=0x2194
HI_VOID VPSS_DRV_Set_VPSS_DM_EDGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_global_str ,HI_U32 dm_init_val_step ,HI_U32 dm_mmf_set )
{
     U_VPSS_DM_EDGE VPSS_DM_EDGE;

     VPSS_DM_EDGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_EDGE.u32));
     VPSS_DM_EDGE.bits.dm_global_str    = dm_global_str    ;
     VPSS_DM_EDGE.bits.dm_init_val_step = dm_init_val_step ;
     VPSS_DM_EDGE.bits.dm_mmf_set       = dm_mmf_set       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_EDGE.u32), VPSS_DM_EDGE.u32);

     return;
}
//addr=0x2198
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_sw_wht_lut_p3 ,HI_U32 dm_sw_wht_lut_p2 ,HI_U32 dm_sw_wht_lut_p1 ,HI_U32 dm_sw_wht_lut_p0 )
{
     U_VPSS_DM_LUT0 VPSS_DM_LUT0;

     VPSS_DM_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT0.u32));
     VPSS_DM_LUT0.bits.dm_sw_wht_lut_p3 = dm_sw_wht_lut_p3 ;
     VPSS_DM_LUT0.bits.dm_sw_wht_lut_p2 = dm_sw_wht_lut_p2 ;
     VPSS_DM_LUT0.bits.dm_sw_wht_lut_p1 = dm_sw_wht_lut_p1 ;
     VPSS_DM_LUT0.bits.dm_sw_wht_lut_p0 = dm_sw_wht_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT0.u32), VPSS_DM_LUT0.u32);

     return;
}
//addr=0x219c
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_limit_t_10 ,HI_U32 dm_limit_t ,HI_U32 dm_sw_wht_lut_p4 )
{
     U_VPSS_DM_LUT1 VPSS_DM_LUT1;

     VPSS_DM_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT1.u32));
     VPSS_DM_LUT1.bits.dm_limit_t_10    = dm_limit_t_10    ;
     VPSS_DM_LUT1.bits.dm_limit_t       = dm_limit_t       ;
     VPSS_DM_LUT1.bits.dm_sw_wht_lut_p4 = dm_sw_wht_lut_p4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT1.u32), VPSS_DM_LUT1.u32);

     return;
}
//addr=0x21a0
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_gain_lut_p7 ,HI_U32 dm_dir_str_gain_lut_p6 ,HI_U32 dm_dir_str_gain_lut_p5 ,HI_U32 dm_dir_str_gain_lut_p4 ,HI_U32 dm_dir_str_gain_lut_p3 ,HI_U32 dm_dir_str_gain_lut_p2 ,HI_U32 dm_dir_str_gain_lut_p1 ,HI_U32 dm_dir_str_gain_lut_p0 )
{
     U_VPSS_DM_LUT2 VPSS_DM_LUT2;

     VPSS_DM_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT2.u32));
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p7 = dm_dir_str_gain_lut_p7 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p6 = dm_dir_str_gain_lut_p6 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p5 = dm_dir_str_gain_lut_p5 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p4 = dm_dir_str_gain_lut_p4 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p3 = dm_dir_str_gain_lut_p3 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p2 = dm_dir_str_gain_lut_p2 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p1 = dm_dir_str_gain_lut_p1 ;
     VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p0 = dm_dir_str_gain_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT2.u32), VPSS_DM_LUT2.u32);

     return;
}
//addr=0x21a4
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_lut_p7 ,HI_U32 dm_dir_str_lut_p6 ,HI_U32 dm_dir_str_lut_p5 ,HI_U32 dm_dir_str_lut_p4 ,HI_U32 dm_dir_str_lut_p3 ,HI_U32 dm_dir_str_lut_p2 ,HI_U32 dm_dir_str_lut_p1 ,HI_U32 dm_dir_str_lut_p0 )
{
     U_VPSS_DM_LUT3 VPSS_DM_LUT3;

     VPSS_DM_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT3.u32));
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p7 = dm_dir_str_lut_p7 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p6 = dm_dir_str_lut_p6 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p5 = dm_dir_str_lut_p5 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p4 = dm_dir_str_lut_p4 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p3 = dm_dir_str_lut_p3 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p2 = dm_dir_str_lut_p2 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p1 = dm_dir_str_lut_p1 ;
     VPSS_DM_LUT3.bits.dm_dir_str_lut_p0 = dm_dir_str_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT3.u32), VPSS_DM_LUT3.u32);

     return;
}
//addr=0x21a8
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_lut_p15 ,HI_U32 dm_dir_str_lut_p14 ,HI_U32 dm_dir_str_lut_p13 ,HI_U32 dm_dir_str_lut_p12 ,HI_U32 dm_dir_str_lut_p11 ,HI_U32 dm_dir_str_lut_p10 ,HI_U32 dm_dir_str_lut_p9 ,HI_U32 dm_dir_str_lut_p8 )
{
     U_VPSS_DM_LUT4 VPSS_DM_LUT4;

     VPSS_DM_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT4.u32));
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p15 = dm_dir_str_lut_p15 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p14 = dm_dir_str_lut_p14 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p13 = dm_dir_str_lut_p13 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p12 = dm_dir_str_lut_p12 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p11 = dm_dir_str_lut_p11 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p10 = dm_dir_str_lut_p10 ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p9  = dm_dir_str_lut_p9  ;
     VPSS_DM_LUT4.bits.dm_dir_str_lut_p8  = dm_dir_str_lut_p8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT4.u32), VPSS_DM_LUT4.u32);

     return;
}
//addr=0x21ac
HI_VOID VPSS_DRV_Set_VPSS_DM_DIRC(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_mmf_limit_en ,HI_U32 dm_mndir_opp_ctrst_t )
{
     U_VPSS_DM_DIRC VPSS_DM_DIRC;

     VPSS_DM_DIRC.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIRC.u32));
     VPSS_DM_DIRC.bits.dm_mmf_limit_en      = dm_mmf_limit_en      ;
     VPSS_DM_DIRC.bits.dm_mndir_opp_ctrst_t = dm_mndir_opp_ctrst_t ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIRC.u32), VPSS_DM_DIRC.u32);

     return;
}
//addr=0x21b0
HI_VOID VPSS_DRV_Set_VPSS_DM_DIRO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_st_10 )
{
     U_VPSS_DM_DIRO VPSS_DM_DIRO;

     VPSS_DM_DIRO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIRO.u32));
     VPSS_DM_DIRO.bits.dm_csw_trsnt_st_10 = dm_csw_trsnt_st_10 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_DIRO.u32), VPSS_DM_DIRO.u32);

     return;
}
//addr=0x21b4
HI_VOID VPSS_DRV_Set_VPSS_DM_LSW(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_lsw_ratio ,HI_U32 dm_csw_trsnt_st ,HI_U32 dm_lw_ctrst_t_10 ,HI_U32 dm_lw_ctrst_t )
{
     U_VPSS_DM_LSW VPSS_DM_LSW;

     VPSS_DM_LSW.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LSW.u32));
     VPSS_DM_LSW.bits.dm_lsw_ratio     = dm_lsw_ratio     ;
     VPSS_DM_LSW.bits.dm_csw_trsnt_st  = dm_csw_trsnt_st  ;
     VPSS_DM_LSW.bits.dm_lw_ctrst_t_10 = dm_lw_ctrst_t_10 ;
     VPSS_DM_LSW.bits.dm_lw_ctrst_t    = dm_lw_ctrst_t    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LSW.u32), VPSS_DM_LSW.u32);

     return;
}
//addr=0x21b8
HI_VOID VPSS_DRV_Set_VPSS_DM_MMFLR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_lt ,HI_U32 dm_mmf_lr_10 ,HI_U32 dm_mmf_lr )
{
     U_VPSS_DM_MMFLR VPSS_DM_MMFLR;

     VPSS_DM_MMFLR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_MMFLR.u32));
     VPSS_DM_MMFLR.bits.dm_csw_trsnt_lt = dm_csw_trsnt_lt ;
     VPSS_DM_MMFLR.bits.dm_mmf_lr_10    = dm_mmf_lr_10    ;
     VPSS_DM_MMFLR.bits.dm_mmf_lr       = dm_mmf_lr       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_MMFLR.u32), VPSS_DM_MMFLR.u32);

     return;
}
//addr=0x21bc
HI_VOID VPSS_DRV_Set_VPSS_DM_MMFSR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_lt_10 ,HI_U32 dm_mmf_sr_10 ,HI_U32 dm_mmf_sr )
{
     U_VPSS_DM_MMFSR VPSS_DM_MMFSR;

     VPSS_DM_MMFSR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_MMFSR.u32));
     VPSS_DM_MMFSR.bits.dm_csw_trsnt_lt_10 = dm_csw_trsnt_lt_10 ;
     VPSS_DM_MMFSR.bits.dm_mmf_sr_10       = dm_mmf_sr_10       ;
     VPSS_DM_MMFSR.bits.dm_mmf_sr          = dm_mmf_sr          ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_MMFSR.u32), VPSS_DM_MMFSR.u32);

     return;
}
//addr=0x21c0
HI_VOID VPSS_DRV_Set_VPSS_DM_THR0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_blend_str ,HI_U32 dm_lim_res_blend_str2 ,HI_U32 dm_lim_res_blend_str1 ,HI_U32 dm_limit_lsw_ratio )
{
     U_VPSS_DM_THR0 VPSS_DM_THR0;

     VPSS_DM_THR0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_THR0.u32));
     VPSS_DM_THR0.bits.dm_dir_blend_str      = dm_dir_blend_str      ;
     VPSS_DM_THR0.bits.dm_lim_res_blend_str2 = dm_lim_res_blend_str2 ;
     VPSS_DM_THR0.bits.dm_lim_res_blend_str1 = dm_lim_res_blend_str1 ;
     VPSS_DM_THR0.bits.dm_limit_lsw_ratio    = dm_limit_lsw_ratio    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_THR0.u32), VPSS_DM_THR0.u32);

     return;
}
//addr=0x21c4
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p4 ,HI_U32 dm_trans_band_lut_p3 ,HI_U32 dm_trans_band_lut_p2 ,HI_U32 dm_trans_band_lut_p1 ,HI_U32 dm_trans_band_lut_p0 )
{
     U_VPSS_DM_LUT5 VPSS_DM_LUT5;

     VPSS_DM_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT5.u32));
     VPSS_DM_LUT5.bits.dm_trans_band_lut_p4 = dm_trans_band_lut_p4 ;
     VPSS_DM_LUT5.bits.dm_trans_band_lut_p3 = dm_trans_band_lut_p3 ;
     VPSS_DM_LUT5.bits.dm_trans_band_lut_p2 = dm_trans_band_lut_p2 ;
     VPSS_DM_LUT5.bits.dm_trans_band_lut_p1 = dm_trans_band_lut_p1 ;
     VPSS_DM_LUT5.bits.dm_trans_band_lut_p0 = dm_trans_band_lut_p0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT5.u32), VPSS_DM_LUT5.u32);

     return;
}
//addr=0x21c8
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p9 ,HI_U32 dm_trans_band_lut_p8 ,HI_U32 dm_trans_band_lut_p7 ,HI_U32 dm_trans_band_lut_p6 ,HI_U32 dm_trans_band_lut_p5 )
{
     U_VPSS_DM_LUT6 VPSS_DM_LUT6;

     VPSS_DM_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT6.u32));
     VPSS_DM_LUT6.bits.dm_trans_band_lut_p9 = dm_trans_band_lut_p9 ;
     VPSS_DM_LUT6.bits.dm_trans_band_lut_p8 = dm_trans_band_lut_p8 ;
     VPSS_DM_LUT6.bits.dm_trans_band_lut_p7 = dm_trans_band_lut_p7 ;
     VPSS_DM_LUT6.bits.dm_trans_band_lut_p6 = dm_trans_band_lut_p6 ;
     VPSS_DM_LUT6.bits.dm_trans_band_lut_p5 = dm_trans_band_lut_p5 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT6.u32), VPSS_DM_LUT6.u32);

     return;
}
//addr=0x21cc
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p14 ,HI_U32 dm_trans_band_lut_p13 ,HI_U32 dm_trans_band_lut_p12 ,HI_U32 dm_trans_band_lut_p11 ,HI_U32 dm_trans_band_lut_p10 )
{
     U_VPSS_DM_LUT7 VPSS_DM_LUT7;

     VPSS_DM_LUT7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT7.u32));
     VPSS_DM_LUT7.bits.dm_trans_band_lut_p14 = dm_trans_band_lut_p14 ;
     VPSS_DM_LUT7.bits.dm_trans_band_lut_p13 = dm_trans_band_lut_p13 ;
     VPSS_DM_LUT7.bits.dm_trans_band_lut_p12 = dm_trans_band_lut_p12 ;
     VPSS_DM_LUT7.bits.dm_trans_band_lut_p11 = dm_trans_band_lut_p11 ;
     VPSS_DM_LUT7.bits.dm_trans_band_lut_p10 = dm_trans_band_lut_p10 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT7.u32), VPSS_DM_LUT7.u32);

     return;
}
//addr=0x21d0
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p19 ,HI_U32 dm_trans_band_lut_p18 ,HI_U32 dm_trans_band_lut_p17 ,HI_U32 dm_trans_band_lut_p16 ,HI_U32 dm_trans_band_lut_p15 )
{
     U_VPSS_DM_LUT8 VPSS_DM_LUT8;

     VPSS_DM_LUT8.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT8.u32));
     VPSS_DM_LUT8.bits.dm_trans_band_lut_p19 = dm_trans_band_lut_p19 ;
     VPSS_DM_LUT8.bits.dm_trans_band_lut_p18 = dm_trans_band_lut_p18 ;
     VPSS_DM_LUT8.bits.dm_trans_band_lut_p17 = dm_trans_band_lut_p17 ;
     VPSS_DM_LUT8.bits.dm_trans_band_lut_p16 = dm_trans_band_lut_p16 ;
     VPSS_DM_LUT8.bits.dm_trans_band_lut_p15 = dm_trans_band_lut_p15 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT8.u32), VPSS_DM_LUT8.u32);

     return;
}
//addr=0x21d4
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p24 ,HI_U32 dm_trans_band_lut_p23 ,HI_U32 dm_trans_band_lut_p22 ,HI_U32 dm_trans_band_lut_p21 ,HI_U32 dm_trans_band_lut_p20 )
{
     U_VPSS_DM_LUT9 VPSS_DM_LUT9;

     VPSS_DM_LUT9.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT9.u32));
     VPSS_DM_LUT9.bits.dm_trans_band_lut_p24 = dm_trans_band_lut_p24 ;
     VPSS_DM_LUT9.bits.dm_trans_band_lut_p23 = dm_trans_band_lut_p23 ;
     VPSS_DM_LUT9.bits.dm_trans_band_lut_p22 = dm_trans_band_lut_p22 ;
     VPSS_DM_LUT9.bits.dm_trans_band_lut_p21 = dm_trans_band_lut_p21 ;
     VPSS_DM_LUT9.bits.dm_trans_band_lut_p20 = dm_trans_band_lut_p20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT9.u32), VPSS_DM_LUT9.u32);

     return;
}
//addr=0x21d8
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p29 ,HI_U32 dm_trans_band_lut_p28 ,HI_U32 dm_trans_band_lut_p27 ,HI_U32 dm_trans_band_lut_p26 ,HI_U32 dm_trans_band_lut_p25 )
{
     U_VPSS_DM_LUT10 VPSS_DM_LUT10;

     VPSS_DM_LUT10.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT10.u32));
     VPSS_DM_LUT10.bits.dm_trans_band_lut_p29 = dm_trans_band_lut_p29 ;
     VPSS_DM_LUT10.bits.dm_trans_band_lut_p28 = dm_trans_band_lut_p28 ;
     VPSS_DM_LUT10.bits.dm_trans_band_lut_p27 = dm_trans_band_lut_p27 ;
     VPSS_DM_LUT10.bits.dm_trans_band_lut_p26 = dm_trans_band_lut_p26 ;
     VPSS_DM_LUT10.bits.dm_trans_band_lut_p25 = dm_trans_band_lut_p25 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT10.u32), VPSS_DM_LUT10.u32);

     return;
}
//addr=0x21dc
HI_VOID VPSS_DRV_Set_VPSS_DM_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p30 )
{
     U_VPSS_DM_LUT11 VPSS_DM_LUT11;

     VPSS_DM_LUT11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT11.u32));
     VPSS_DM_LUT11.bits.dm_trans_band_lut_p30 = dm_trans_band_lut_p30 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DM_LUT11.u32), VPSS_DM_LUT11.u32);

     return;
}
//addr=0x2200
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_0 )
{
     U_VPSS_DB_BORD_FLAG0 VPSS_DB_BORD_FLAG0;

     VPSS_DB_BORD_FLAG0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG0.u32));
     VPSS_DB_BORD_FLAG0.bits.vpss_db_bord_flag_0 = vpss_db_bord_flag_0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG0.u32), VPSS_DB_BORD_FLAG0.u32);

     return;
}
//addr=0x2204
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_1 )
{
     U_VPSS_DB_BORD_FLAG1 VPSS_DB_BORD_FLAG1;

     VPSS_DB_BORD_FLAG1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG1.u32));
     VPSS_DB_BORD_FLAG1.bits.vpss_db_bord_flag_1 = vpss_db_bord_flag_1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG1.u32), VPSS_DB_BORD_FLAG1.u32);

     return;
}
//addr=0x2208
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_2 )
{
     U_VPSS_DB_BORD_FLAG2 VPSS_DB_BORD_FLAG2;

     VPSS_DB_BORD_FLAG2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG2.u32));
     VPSS_DB_BORD_FLAG2.bits.vpss_db_bord_flag_2 = vpss_db_bord_flag_2 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG2.u32), VPSS_DB_BORD_FLAG2.u32);

     return;
}
//addr=0x220c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_3 )
{
     U_VPSS_DB_BORD_FLAG3 VPSS_DB_BORD_FLAG3;

     VPSS_DB_BORD_FLAG3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG3.u32));
     VPSS_DB_BORD_FLAG3.bits.vpss_db_bord_flag_3 = vpss_db_bord_flag_3 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG3.u32), VPSS_DB_BORD_FLAG3.u32);

     return;
}
//addr=0x2210
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_4 )
{
     U_VPSS_DB_BORD_FLAG4 VPSS_DB_BORD_FLAG4;

     VPSS_DB_BORD_FLAG4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG4.u32));
     VPSS_DB_BORD_FLAG4.bits.vpss_db_bord_flag_4 = vpss_db_bord_flag_4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG4.u32), VPSS_DB_BORD_FLAG4.u32);

     return;
}
//addr=0x2214
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_5 )
{
     U_VPSS_DB_BORD_FLAG5 VPSS_DB_BORD_FLAG5;

     VPSS_DB_BORD_FLAG5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG5.u32));
     VPSS_DB_BORD_FLAG5.bits.vpss_db_bord_flag_5 = vpss_db_bord_flag_5 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG5.u32), VPSS_DB_BORD_FLAG5.u32);

     return;
}
//addr=0x2218
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_6 )
{
     U_VPSS_DB_BORD_FLAG6 VPSS_DB_BORD_FLAG6;

     VPSS_DB_BORD_FLAG6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG6.u32));
     VPSS_DB_BORD_FLAG6.bits.vpss_db_bord_flag_6 = vpss_db_bord_flag_6 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG6.u32), VPSS_DB_BORD_FLAG6.u32);

     return;
}
//addr=0x221c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_7 )
{
     U_VPSS_DB_BORD_FLAG7 VPSS_DB_BORD_FLAG7;

     VPSS_DB_BORD_FLAG7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG7.u32));
     VPSS_DB_BORD_FLAG7.bits.vpss_db_bord_flag_7 = vpss_db_bord_flag_7 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG7.u32), VPSS_DB_BORD_FLAG7.u32);

     return;
}
//addr=0x2220
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_8 )
{
     U_VPSS_DB_BORD_FLAG8 VPSS_DB_BORD_FLAG8;

     VPSS_DB_BORD_FLAG8.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG8.u32));
     VPSS_DB_BORD_FLAG8.bits.vpss_db_bord_flag_8 = vpss_db_bord_flag_8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG8.u32), VPSS_DB_BORD_FLAG8.u32);

     return;
}
//addr=0x2224
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_9 )
{
     U_VPSS_DB_BORD_FLAG9 VPSS_DB_BORD_FLAG9;

     VPSS_DB_BORD_FLAG9.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG9.u32));
     VPSS_DB_BORD_FLAG9.bits.vpss_db_bord_flag_9 = vpss_db_bord_flag_9 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG9.u32), VPSS_DB_BORD_FLAG9.u32);

     return;
}
//addr=0x2228
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_10 )
{
     U_VPSS_DB_BORD_FLAG10 VPSS_DB_BORD_FLAG10;

     VPSS_DB_BORD_FLAG10.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG10.u32));
     VPSS_DB_BORD_FLAG10.bits.vpss_db_bord_flag_10 = vpss_db_bord_flag_10 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG10.u32), VPSS_DB_BORD_FLAG10.u32);

     return;
}
//addr=0x222c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_11 )
{
     U_VPSS_DB_BORD_FLAG11 VPSS_DB_BORD_FLAG11;

     VPSS_DB_BORD_FLAG11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG11.u32));
     VPSS_DB_BORD_FLAG11.bits.vpss_db_bord_flag_11 = vpss_db_bord_flag_11 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG11.u32), VPSS_DB_BORD_FLAG11.u32);

     return;
}
//addr=0x2230
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_12 )
{
     U_VPSS_DB_BORD_FLAG12 VPSS_DB_BORD_FLAG12;

     VPSS_DB_BORD_FLAG12.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG12.u32));
     VPSS_DB_BORD_FLAG12.bits.vpss_db_bord_flag_12 = vpss_db_bord_flag_12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG12.u32), VPSS_DB_BORD_FLAG12.u32);

     return;
}
//addr=0x2234
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_13 )
{
     U_VPSS_DB_BORD_FLAG13 VPSS_DB_BORD_FLAG13;

     VPSS_DB_BORD_FLAG13.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG13.u32));
     VPSS_DB_BORD_FLAG13.bits.vpss_db_bord_flag_13 = vpss_db_bord_flag_13 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG13.u32), VPSS_DB_BORD_FLAG13.u32);

     return;
}
//addr=0x2238
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_14 )
{
     U_VPSS_DB_BORD_FLAG14 VPSS_DB_BORD_FLAG14;

     VPSS_DB_BORD_FLAG14.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG14.u32));
     VPSS_DB_BORD_FLAG14.bits.vpss_db_bord_flag_14 = vpss_db_bord_flag_14 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG14.u32), VPSS_DB_BORD_FLAG14.u32);

     return;
}
//addr=0x223c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_15 )
{
     U_VPSS_DB_BORD_FLAG15 VPSS_DB_BORD_FLAG15;

     VPSS_DB_BORD_FLAG15.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG15.u32));
     VPSS_DB_BORD_FLAG15.bits.vpss_db_bord_flag_15 = vpss_db_bord_flag_15 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG15.u32), VPSS_DB_BORD_FLAG15.u32);

     return;
}
//addr=0x2240
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_16 )
{
     U_VPSS_DB_BORD_FLAG16 VPSS_DB_BORD_FLAG16;

     VPSS_DB_BORD_FLAG16.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG16.u32));
     VPSS_DB_BORD_FLAG16.bits.vpss_db_bord_flag_16 = vpss_db_bord_flag_16 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG16.u32), VPSS_DB_BORD_FLAG16.u32);

     return;
}
//addr=0x2244
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_17 )
{
     U_VPSS_DB_BORD_FLAG17 VPSS_DB_BORD_FLAG17;

     VPSS_DB_BORD_FLAG17.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG17.u32));
     VPSS_DB_BORD_FLAG17.bits.vpss_db_bord_flag_17 = vpss_db_bord_flag_17 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG17.u32), VPSS_DB_BORD_FLAG17.u32);

     return;
}
//addr=0x2248
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_18 )
{
     U_VPSS_DB_BORD_FLAG18 VPSS_DB_BORD_FLAG18;

     VPSS_DB_BORD_FLAG18.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG18.u32));
     VPSS_DB_BORD_FLAG18.bits.vpss_db_bord_flag_18 = vpss_db_bord_flag_18 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG18.u32), VPSS_DB_BORD_FLAG18.u32);

     return;
}
//addr=0x224c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_19 )
{
     U_VPSS_DB_BORD_FLAG19 VPSS_DB_BORD_FLAG19;

     VPSS_DB_BORD_FLAG19.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG19.u32));
     VPSS_DB_BORD_FLAG19.bits.vpss_db_bord_flag_19 = vpss_db_bord_flag_19 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG19.u32), VPSS_DB_BORD_FLAG19.u32);

     return;
}
//addr=0x2250
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_20 )
{
     U_VPSS_DB_BORD_FLAG20 VPSS_DB_BORD_FLAG20;

     VPSS_DB_BORD_FLAG20.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG20.u32));
     VPSS_DB_BORD_FLAG20.bits.vpss_db_bord_flag_20 = vpss_db_bord_flag_20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG20.u32), VPSS_DB_BORD_FLAG20.u32);

     return;
}
//addr=0x2254
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_21 )
{
     U_VPSS_DB_BORD_FLAG21 VPSS_DB_BORD_FLAG21;

     VPSS_DB_BORD_FLAG21.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG21.u32));
     VPSS_DB_BORD_FLAG21.bits.vpss_db_bord_flag_21 = vpss_db_bord_flag_21 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG21.u32), VPSS_DB_BORD_FLAG21.u32);

     return;
}
//addr=0x2258
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_22 )
{
     U_VPSS_DB_BORD_FLAG22 VPSS_DB_BORD_FLAG22;

     VPSS_DB_BORD_FLAG22.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG22.u32));
     VPSS_DB_BORD_FLAG22.bits.vpss_db_bord_flag_22 = vpss_db_bord_flag_22 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG22.u32), VPSS_DB_BORD_FLAG22.u32);

     return;
}
//addr=0x225c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_23 )
{
     U_VPSS_DB_BORD_FLAG23 VPSS_DB_BORD_FLAG23;

     VPSS_DB_BORD_FLAG23.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG23.u32));
     VPSS_DB_BORD_FLAG23.bits.vpss_db_bord_flag_23 = vpss_db_bord_flag_23 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG23.u32), VPSS_DB_BORD_FLAG23.u32);

     return;
}
//addr=0x2260
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_24 )
{
     U_VPSS_DB_BORD_FLAG24 VPSS_DB_BORD_FLAG24;

     VPSS_DB_BORD_FLAG24.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG24.u32));
     VPSS_DB_BORD_FLAG24.bits.vpss_db_bord_flag_24 = vpss_db_bord_flag_24 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG24.u32), VPSS_DB_BORD_FLAG24.u32);

     return;
}
//addr=0x2264
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_25 )
{
     U_VPSS_DB_BORD_FLAG25 VPSS_DB_BORD_FLAG25;

     VPSS_DB_BORD_FLAG25.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG25.u32));
     VPSS_DB_BORD_FLAG25.bits.vpss_db_bord_flag_25 = vpss_db_bord_flag_25 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG25.u32), VPSS_DB_BORD_FLAG25.u32);

     return;
}
//addr=0x2268
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_26 )
{
     U_VPSS_DB_BORD_FLAG26 VPSS_DB_BORD_FLAG26;

     VPSS_DB_BORD_FLAG26.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG26.u32));
     VPSS_DB_BORD_FLAG26.bits.vpss_db_bord_flag_26 = vpss_db_bord_flag_26 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG26.u32), VPSS_DB_BORD_FLAG26.u32);

     return;
}
//addr=0x226c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_27 )
{
     U_VPSS_DB_BORD_FLAG27 VPSS_DB_BORD_FLAG27;

     VPSS_DB_BORD_FLAG27.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG27.u32));
     VPSS_DB_BORD_FLAG27.bits.vpss_db_bord_flag_27 = vpss_db_bord_flag_27 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG27.u32), VPSS_DB_BORD_FLAG27.u32);

     return;
}
//addr=0x2270
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_28 )
{
     U_VPSS_DB_BORD_FLAG28 VPSS_DB_BORD_FLAG28;

     VPSS_DB_BORD_FLAG28.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG28.u32));
     VPSS_DB_BORD_FLAG28.bits.vpss_db_bord_flag_28 = vpss_db_bord_flag_28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG28.u32), VPSS_DB_BORD_FLAG28.u32);

     return;
}
//addr=0x2274
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_29 )
{
     U_VPSS_DB_BORD_FLAG29 VPSS_DB_BORD_FLAG29;

     VPSS_DB_BORD_FLAG29.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG29.u32));
     VPSS_DB_BORD_FLAG29.bits.vpss_db_bord_flag_29 = vpss_db_bord_flag_29 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG29.u32), VPSS_DB_BORD_FLAG29.u32);

     return;
}
//addr=0x2278
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_30 )
{
     U_VPSS_DB_BORD_FLAG30 VPSS_DB_BORD_FLAG30;

     VPSS_DB_BORD_FLAG30.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG30.u32));
     VPSS_DB_BORD_FLAG30.bits.vpss_db_bord_flag_30 = vpss_db_bord_flag_30 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG30.u32), VPSS_DB_BORD_FLAG30.u32);

     return;
}
//addr=0x227c
HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_31 )
{
     U_VPSS_DB_BORD_FLAG31 VPSS_DB_BORD_FLAG31;

     VPSS_DB_BORD_FLAG31.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG31.u32));
     VPSS_DB_BORD_FLAG31.bits.vpss_db_bord_flag_31 = vpss_db_bord_flag_31 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_DB_BORD_FLAG31.u32), VPSS_DB_BORD_FLAG31.u32);

     return;
}
//addr=0x2300
HI_VOID VPSS_DRV_Set_VPSS_SNR_ENABLE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 test_en ,HI_U32 edgeprefilteren ,HI_U32 colorweighten ,HI_U32 motionedgeweighten ,HI_U32 cnr2den ,HI_U32 ynr2den )
{
     U_VPSS_SNR_ENABLE VPSS_SNR_ENABLE;

     VPSS_SNR_ENABLE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_ENABLE.u32));
     VPSS_SNR_ENABLE.bits.test_en            = test_en            ;
     VPSS_SNR_ENABLE.bits.edgeprefilteren    = edgeprefilteren    ;
     VPSS_SNR_ENABLE.bits.colorweighten      = colorweighten      ;
     VPSS_SNR_ENABLE.bits.motionedgeweighten = motionedgeweighten ;
     VPSS_SNR_ENABLE.bits.cnr2den            = cnr2den            ;
     VPSS_SNR_ENABLE.bits.ynr2den            = ynr2den            ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_ENABLE.u32), VPSS_SNR_ENABLE.u32);

     return;
}
//addr=0x2304
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIORANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgeoriratio ,HI_U32 edgeminratio ,HI_U32 edgemaxratio )
{
     U_VPSS_SNR_EDGE_RATIORANGE VPSS_SNR_EDGE_RATIORANGE;

     VPSS_SNR_EDGE_RATIORANGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIORANGE.u32));
     VPSS_SNR_EDGE_RATIORANGE.bits.edgeoriratio = edgeoriratio ;
     VPSS_SNR_EDGE_RATIORANGE.bits.edgeminratio = edgeminratio ;
     VPSS_SNR_EDGE_RATIORANGE.bits.edgemaxratio = edgemaxratio ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIORANGE.u32), VPSS_SNR_EDGE_RATIORANGE.u32);

     return;
}
//addr=0x2308
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRRANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgeoristrength ,HI_U32 edgeminstrength ,HI_U32 edgemaxstrength )
{
     U_VPSS_SNR_EDGE_STRRANGE VPSS_SNR_EDGE_STRRANGE;

     VPSS_SNR_EDGE_STRRANGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRRANGE.u32));
     VPSS_SNR_EDGE_STRRANGE.bits.edgeoristrength = edgeoristrength ;
     VPSS_SNR_EDGE_STRRANGE.bits.edgeminstrength = edgeminstrength ;
     VPSS_SNR_EDGE_STRRANGE.bits.edgemaxstrength = edgemaxstrength ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRRANGE.u32), VPSS_SNR_EDGE_STRRANGE.u32);

     return;
}
//addr=0x230c
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRTH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgestrth3 ,HI_U32 edgestrth2 ,HI_U32 edgestrth1 )
{
     U_VPSS_SNR_EDGE_STRTH VPSS_SNR_EDGE_STRTH;

     VPSS_SNR_EDGE_STRTH.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRTH.u32));
     VPSS_SNR_EDGE_STRTH.bits.edgestrth3 = edgestrth3 ;
     VPSS_SNR_EDGE_STRTH.bits.edgestrth2 = edgestrth2 ;
     VPSS_SNR_EDGE_STRTH.bits.edgestrth1 = edgestrth1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRTH.u32), VPSS_SNR_EDGE_STRTH.u32);

     return;
}
//addr=0x2310
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgestrk3 ,HI_U32 edgestrk2 ,HI_U32 edgestrk1 )
{
     U_VPSS_SNR_EDGE_STRK VPSS_SNR_EDGE_STRK;

     VPSS_SNR_EDGE_STRK.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRK.u32));
     VPSS_SNR_EDGE_STRK.bits.edgestrk3  = edgestrk3  ;
     VPSS_SNR_EDGE_STRK.bits.edgestrk2  = edgestrk2  ;
     VPSS_SNR_EDGE_STRK.bits.edgestrk1  = edgestrk1  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STRK.u32), VPSS_SNR_EDGE_STRK.u32);

     return;
}
//addr=0x2314
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth3 ,HI_U32 edgemeanth2 ,HI_U32 edgemeanth1 )
{
     U_VPSS_SNR_EDGE_MEANTH1 VPSS_SNR_EDGE_MEANTH1;

     VPSS_SNR_EDGE_MEANTH1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH1.u32));
     VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth3 = edgemeanth3 ;
     VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth2 = edgemeanth2 ;
     VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth1 = edgemeanth1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH1.u32), VPSS_SNR_EDGE_MEANTH1.u32);

     return;
}
//addr=0x2318
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth6 ,HI_U32 edgemeanth5 ,HI_U32 edgemeanth4 )
{
     U_VPSS_SNR_EDGE_MEANTH2 VPSS_SNR_EDGE_MEANTH2;

     VPSS_SNR_EDGE_MEANTH2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH2.u32));
     VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth6 = edgemeanth6 ;
     VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth5 = edgemeanth5 ;
     VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth4 = edgemeanth4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH2.u32), VPSS_SNR_EDGE_MEANTH2.u32);

     return;
}
//addr=0x231c
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth8 ,HI_U32 edgemeanth7 )
{
     U_VPSS_SNR_EDGE_MEANTH3 VPSS_SNR_EDGE_MEANTH3;

     VPSS_SNR_EDGE_MEANTH3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH3.u32));
     VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth8 = edgemeanth8 ;
     VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth7 = edgemeanth7 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANTH3.u32), VPSS_SNR_EDGE_MEANTH3.u32);

     return;
}
//addr=0x2320
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeank4 ,HI_U32 edgemeank3 ,HI_U32 edgemeank2 ,HI_U32 edgemeank1 )
{
     U_VPSS_SNR_EDGE_MEANK1 VPSS_SNR_EDGE_MEANK1;

     VPSS_SNR_EDGE_MEANK1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANK1.u32));
     VPSS_SNR_EDGE_MEANK1.bits.edgemeank4 = edgemeank4 ;
     VPSS_SNR_EDGE_MEANK1.bits.edgemeank3 = edgemeank3 ;
     VPSS_SNR_EDGE_MEANK1.bits.edgemeank2 = edgemeank2 ;
     VPSS_SNR_EDGE_MEANK1.bits.edgemeank1 = edgemeank1 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANK1.u32), VPSS_SNR_EDGE_MEANK1.u32);

     return;
}
//addr=0x2324
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANK2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeank8 ,HI_U32 edgemeank7 ,HI_U32 edgemeank6 ,HI_U32 edgemeank5 )
{
     U_VPSS_SNR_EDGE_MEANK2 VPSS_SNR_EDGE_MEANK2;

     VPSS_SNR_EDGE_MEANK2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANK2.u32));
     VPSS_SNR_EDGE_MEANK2.bits.edgemeank8 = edgemeank8 ;
     VPSS_SNR_EDGE_MEANK2.bits.edgemeank7 = edgemeank7 ;
     VPSS_SNR_EDGE_MEANK2.bits.edgemeank6 = edgemeank6 ;
     VPSS_SNR_EDGE_MEANK2.bits.edgemeank5 = edgemeank5 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_MEANK2.u32), VPSS_SNR_EDGE_MEANK2.u32);

     return;
}
//addr=0x2328
HI_VOID VPSS_DRV_Set_VPSS_SNR_WIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 c2dwinheight ,HI_U32 c2dwinwidth ,HI_U32 y2dwinheight ,HI_U32 y2dwinwidth )
{
     U_VPSS_SNR_WIN VPSS_SNR_WIN;

     VPSS_SNR_WIN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_WIN.u32));
     VPSS_SNR_WIN.bits.c2dwinheight = c2dwinheight -1;
     VPSS_SNR_WIN.bits.c2dwinwidth  = c2dwinwidth  -1;
     VPSS_SNR_WIN.bits.y2dwinheight = y2dwinheight -1;
     VPSS_SNR_WIN.bits.y2dwinwidth  = y2dwinwidth  -1;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_WIN.u32), VPSS_SNR_WIN.u32);

     return;
}
//addr=0x232c
HI_VOID VPSS_DRV_Set_VPSS_SNR_STR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 csnrstr ,HI_U32 ysnrstr )
{
     U_VPSS_SNR_STR VPSS_SNR_STR;

     VPSS_SNR_STR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR.u32));
     VPSS_SNR_STR.bits.csnrstr    = csnrstr    ;
     VPSS_SNR_STR.bits.ysnrstr    = ysnrstr    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR.u32), VPSS_SNR_STR.u32);

     return;
}
//addr=0x2330
HI_VOID VPSS_DRV_Set_VPSS_SNR_STR_OFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 stroffset3 ,HI_U32 stroffset2 ,HI_U32 stroffset1 ,HI_U32 stroffset0 )
{
     U_VPSS_SNR_STR_OFFSET VPSS_SNR_STR_OFFSET;

     VPSS_SNR_STR_OFFSET.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR_OFFSET.u32));
     VPSS_SNR_STR_OFFSET.bits.stroffset3 = stroffset3 ;
     VPSS_SNR_STR_OFFSET.bits.stroffset2 = stroffset2 ;
     VPSS_SNR_STR_OFFSET.bits.stroffset1 = stroffset1 ;
     VPSS_SNR_STR_OFFSET.bits.stroffset0 = stroffset0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR_OFFSET.u32), VPSS_SNR_STR_OFFSET.u32);

     return;
}
//addr=0x2334
HI_VOID VPSS_DRV_Set_VPSS_SNR_STR_ADJUST(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cstradjust ,HI_U32 ystradjust )
{
     U_VPSS_SNR_STR_ADJUST VPSS_SNR_STR_ADJUST;

     VPSS_SNR_STR_ADJUST.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR_ADJUST.u32));
     VPSS_SNR_STR_ADJUST.bits.cstradjust = cstradjust ;
     VPSS_SNR_STR_ADJUST.bits.ystradjust = ystradjust ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_STR_ADJUST.u32), VPSS_SNR_STR_ADJUST.u32);

     return;
}
//addr=0x2338
HI_VOID VPSS_DRV_Set_VPSS_SNR_SCHANGE_TH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechangeth )
{
     U_VPSS_SNR_SCHANGE_TH VPSS_SNR_SCHANGE_TH;

     VPSS_SNR_SCHANGE_TH.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_SCHANGE_TH.u32));
     VPSS_SNR_SCHANGE_TH.bits.scenechangeth = scenechangeth ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_SCHANGE_TH.u32), VPSS_SNR_SCHANGE_TH.u32);

     return;
}
//addr=0x233c
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio3 ,HI_U32 ratio2 ,HI_U32 ratio1 )
{
     U_VPSS_SNR_EDGE_RATIO_1 VPSS_SNR_EDGE_RATIO_1;

     VPSS_SNR_EDGE_RATIO_1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_1.u32));
     VPSS_SNR_EDGE_RATIO_1.bits.ratio3     = ratio3     ;
     VPSS_SNR_EDGE_RATIO_1.bits.ratio2     = ratio2     ;
     VPSS_SNR_EDGE_RATIO_1.bits.ratio1     = ratio1     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_1.u32), VPSS_SNR_EDGE_RATIO_1.u32);

     return;
}
//addr=0x2340
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio6 ,HI_U32 ratio5 ,HI_U32 ratio4 )
{
     U_VPSS_SNR_EDGE_RATIO_2 VPSS_SNR_EDGE_RATIO_2;

     VPSS_SNR_EDGE_RATIO_2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_2.u32));
     VPSS_SNR_EDGE_RATIO_2.bits.ratio6     = ratio6     ;
     VPSS_SNR_EDGE_RATIO_2.bits.ratio5     = ratio5     ;
     VPSS_SNR_EDGE_RATIO_2.bits.ratio4     = ratio4     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_2.u32), VPSS_SNR_EDGE_RATIO_2.u32);

     return;
}
//addr=0x2344
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio8 ,HI_U32 ratio7 )
{
     U_VPSS_SNR_EDGE_RATIO_3 VPSS_SNR_EDGE_RATIO_3;

     VPSS_SNR_EDGE_RATIO_3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_3.u32));
     VPSS_SNR_EDGE_RATIO_3.bits.ratio8     = ratio8     ;
     VPSS_SNR_EDGE_RATIO_3.bits.ratio7     = ratio7     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_RATIO_3.u32), VPSS_SNR_EDGE_RATIO_3.u32);

     return;
}
//addr=0x2348
HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge3 ,HI_U32 edge2 ,HI_U32 edge1 )
{
     U_VPSS_SNR_EDGE_STR VPSS_SNR_EDGE_STR;

     VPSS_SNR_EDGE_STR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STR.u32));
     VPSS_SNR_EDGE_STR.bits.edge3      = edge3      ;
     VPSS_SNR_EDGE_STR.bits.edge2      = edge2      ;
     VPSS_SNR_EDGE_STR.bits.edge1      = edge1      ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_EDGE_STR.u32), VPSS_SNR_EDGE_STR.u32);

     return;
}
//addr=0x234c
HI_VOID VPSS_DRV_Set_VPSS_SNR_TEST_COLOR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 test_color_cr ,HI_U32 test_color_cb )
{
     U_VPSS_SNR_TEST_COLOR VPSS_SNR_TEST_COLOR;

     VPSS_SNR_TEST_COLOR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_TEST_COLOR.u32));
     VPSS_SNR_TEST_COLOR.bits.test_color_cr = test_color_cr ;
     VPSS_SNR_TEST_COLOR.bits.test_color_cb = test_color_cb ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_TEST_COLOR.u32), VPSS_SNR_TEST_COLOR.u32);

     return;
}
//addr=0x2350
HI_VOID VPSS_DRV_Set_VPSS_SNR_SCHANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechange_mode2_en ,HI_U32 scenechange_bldcore ,HI_U32 scenechange_bldk ,HI_U32 scenechange_mode1_en ,HI_U32 scenechange_en ,HI_U32 scenechange_info )
{
     U_VPSS_SNR_SCHANGE VPSS_SNR_SCHANGE;

     VPSS_SNR_SCHANGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_SCHANGE.u32));
     VPSS_SNR_SCHANGE.bits.scenechange_mode2_en = scenechange_mode2_en ;
     VPSS_SNR_SCHANGE.bits.scenechange_bldcore  = scenechange_bldcore  ;
     VPSS_SNR_SCHANGE.bits.scenechange_bldk     = scenechange_bldk     ;
     VPSS_SNR_SCHANGE.bits.scenechange_mode1_en = scenechange_mode1_en ;
     VPSS_SNR_SCHANGE.bits.scenechange_en       = scenechange_en       ;
     VPSS_SNR_SCHANGE.bits.scenechange_info     = scenechange_info     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_SCHANGE.u32), VPSS_SNR_SCHANGE.u32);

     return;
}
//addr=0x2354
HI_VOID VPSS_DRV_Set_VPSS_SNR_MT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motionalpha )
{
     U_VPSS_SNR_MT VPSS_SNR_MT;

     VPSS_SNR_MT.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_MT.u32));
     VPSS_SNR_MT.bits.motionalpha = motionalpha ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_MT.u32), VPSS_SNR_MT.u32);

     return;
}
//addr=0x2358
HI_VOID VPSS_DRV_Set_VPSS_SNR_CBCR_W1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcr_weight1 ,HI_U32 cr_end1 ,HI_U32 cb_end1 ,HI_U32 cr_begin1 ,HI_U32 cb_begin1 )
{
     U_VPSS_SNR_CBCR_W1 VPSS_SNR_CBCR_W1;

     VPSS_SNR_CBCR_W1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_CBCR_W1.u32));
     VPSS_SNR_CBCR_W1.bits.cbcr_weight1 = cbcr_weight1 ;
     VPSS_SNR_CBCR_W1.bits.cr_end1      = cr_end1      ;
     VPSS_SNR_CBCR_W1.bits.cb_end1      = cb_end1      ;
     VPSS_SNR_CBCR_W1.bits.cr_begin1    = cr_begin1    ;
     VPSS_SNR_CBCR_W1.bits.cb_begin1    = cb_begin1    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_CBCR_W1.u32), VPSS_SNR_CBCR_W1.u32);

     return;
}
//addr=0x235c
HI_VOID VPSS_DRV_Set_VPSS_SNR_CBCR_W2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcr_weight2 ,HI_U32 cr_end2 ,HI_U32 cb_end2 ,HI_U32 cr_begin2 ,HI_U32 cb_begin2 )
{
     U_VPSS_SNR_CBCR_W2 VPSS_SNR_CBCR_W2;

     VPSS_SNR_CBCR_W2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_CBCR_W2.u32));
     VPSS_SNR_CBCR_W2.bits.cbcr_weight2 = cbcr_weight2 ;
     VPSS_SNR_CBCR_W2.bits.cr_end2      = cr_end2      ;
     VPSS_SNR_CBCR_W2.bits.cb_end2      = cb_end2      ;
     VPSS_SNR_CBCR_W2.bits.cr_begin2    = cr_begin2    ;
     VPSS_SNR_CBCR_W2.bits.cb_begin2    = cb_begin2    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_SNR_CBCR_W2.u32), VPSS_SNR_CBCR_W2.u32);

     return;
}
//addr=0x2360
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_01(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut07 ,HI_U32 motion_edge_lut06 ,HI_U32 motion_edge_lut05 ,HI_U32 motion_edge_lut04 ,HI_U32 motion_edge_lut03 ,HI_U32 motion_edge_lut02 ,HI_U32 motion_edge_lut01 ,HI_U32 motion_edge_lut00 )
{
     U_MOTION_EDGE_LUT_01 MOTION_EDGE_LUT_01;

     MOTION_EDGE_LUT_01.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_01.u32));
     MOTION_EDGE_LUT_01.bits.motion_edge_lut07 = motion_edge_lut07 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut06 = motion_edge_lut06 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut05 = motion_edge_lut05 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut04 = motion_edge_lut04 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut03 = motion_edge_lut03 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut02 = motion_edge_lut02 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut01 = motion_edge_lut01 ;
     MOTION_EDGE_LUT_01.bits.motion_edge_lut00 = motion_edge_lut00 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_01.u32), MOTION_EDGE_LUT_01.u32);

     return;
}
//addr=0x2364
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_02(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut0f ,HI_U32 motion_edge_lut0e ,HI_U32 motion_edge_lut0d ,HI_U32 motion_edge_lut0c ,HI_U32 motion_edge_lut0b ,HI_U32 motion_edge_lut0a ,HI_U32 motion_edge_lut09 ,HI_U32 motion_edge_lut08 )
{
     U_MOTION_EDGE_LUT_02 MOTION_EDGE_LUT_02;

     MOTION_EDGE_LUT_02.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_02.u32));
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0f = motion_edge_lut0f ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0e = motion_edge_lut0e ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0d = motion_edge_lut0d ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0c = motion_edge_lut0c ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0b = motion_edge_lut0b ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut0a = motion_edge_lut0a ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut09 = motion_edge_lut09 ;
     MOTION_EDGE_LUT_02.bits.motion_edge_lut08 = motion_edge_lut08 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_02.u32), MOTION_EDGE_LUT_02.u32);

     return;
}
//addr=0x2368
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_03(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut17 ,HI_U32 motion_edge_lut16 ,HI_U32 motion_edge_lut15 ,HI_U32 motion_edge_lut14 ,HI_U32 motion_edge_lut13 ,HI_U32 motion_edge_lut12 ,HI_U32 motion_edge_lut11 ,HI_U32 motion_edge_lut10 )
{
     U_MOTION_EDGE_LUT_03 MOTION_EDGE_LUT_03;

     MOTION_EDGE_LUT_03.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_03.u32));
     MOTION_EDGE_LUT_03.bits.motion_edge_lut17 = motion_edge_lut17 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut16 = motion_edge_lut16 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut15 = motion_edge_lut15 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut14 = motion_edge_lut14 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut13 = motion_edge_lut13 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut12 = motion_edge_lut12 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut11 = motion_edge_lut11 ;
     MOTION_EDGE_LUT_03.bits.motion_edge_lut10 = motion_edge_lut10 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_03.u32), MOTION_EDGE_LUT_03.u32);

     return;
}
//addr=0x236c
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_04(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut1f ,HI_U32 motion_edge_lut1e ,HI_U32 motion_edge_lut1d ,HI_U32 motion_edge_lut1c ,HI_U32 motion_edge_lut1b ,HI_U32 motion_edge_lut1a ,HI_U32 motion_edge_lut19 ,HI_U32 motion_edge_lut18 )
{
     U_MOTION_EDGE_LUT_04 MOTION_EDGE_LUT_04;

     MOTION_EDGE_LUT_04.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_04.u32));
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1f = motion_edge_lut1f ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1e = motion_edge_lut1e ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1d = motion_edge_lut1d ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1c = motion_edge_lut1c ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1b = motion_edge_lut1b ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut1a = motion_edge_lut1a ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut19 = motion_edge_lut19 ;
     MOTION_EDGE_LUT_04.bits.motion_edge_lut18 = motion_edge_lut18 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_04.u32), MOTION_EDGE_LUT_04.u32);

     return;
}
//addr=0x2370
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_05(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut27 ,HI_U32 motion_edge_lut26 ,HI_U32 motion_edge_lut25 ,HI_U32 motion_edge_lut24 ,HI_U32 motion_edge_lut23 ,HI_U32 motion_edge_lut22 ,HI_U32 motion_edge_lut21 ,HI_U32 motion_edge_lut20 )
{
     U_MOTION_EDGE_LUT_05 MOTION_EDGE_LUT_05;

     MOTION_EDGE_LUT_05.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_05.u32));
     MOTION_EDGE_LUT_05.bits.motion_edge_lut27 = motion_edge_lut27 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut26 = motion_edge_lut26 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut25 = motion_edge_lut25 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut24 = motion_edge_lut24 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut23 = motion_edge_lut23 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut22 = motion_edge_lut22 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut21 = motion_edge_lut21 ;
     MOTION_EDGE_LUT_05.bits.motion_edge_lut20 = motion_edge_lut20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_05.u32), MOTION_EDGE_LUT_05.u32);

     return;
}
//addr=0x2374
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_06(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut2f ,HI_U32 motion_edge_lut2e ,HI_U32 motion_edge_lut2d ,HI_U32 motion_edge_lut2c ,HI_U32 motion_edge_lut2b ,HI_U32 motion_edge_lut2a ,HI_U32 motion_edge_lut29 ,HI_U32 motion_edge_lut28 )
{
     U_MOTION_EDGE_LUT_06 MOTION_EDGE_LUT_06;

     MOTION_EDGE_LUT_06.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_06.u32));
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2f = motion_edge_lut2f ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2e = motion_edge_lut2e ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2d = motion_edge_lut2d ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2c = motion_edge_lut2c ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2b = motion_edge_lut2b ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut2a = motion_edge_lut2a ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut29 = motion_edge_lut29 ;
     MOTION_EDGE_LUT_06.bits.motion_edge_lut28 = motion_edge_lut28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_06.u32), MOTION_EDGE_LUT_06.u32);

     return;
}
//addr=0x2378
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_07(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut37 ,HI_U32 motion_edge_lut36 ,HI_U32 motion_edge_lut35 ,HI_U32 motion_edge_lut34 ,HI_U32 motion_edge_lut33 ,HI_U32 motion_edge_lut32 ,HI_U32 motion_edge_lut31 ,HI_U32 motion_edge_lut30 )
{
     U_MOTION_EDGE_LUT_07 MOTION_EDGE_LUT_07;

     MOTION_EDGE_LUT_07.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_07.u32));
     MOTION_EDGE_LUT_07.bits.motion_edge_lut37 = motion_edge_lut37 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut36 = motion_edge_lut36 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut35 = motion_edge_lut35 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut34 = motion_edge_lut34 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut33 = motion_edge_lut33 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut32 = motion_edge_lut32 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut31 = motion_edge_lut31 ;
     MOTION_EDGE_LUT_07.bits.motion_edge_lut30 = motion_edge_lut30 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_07.u32), MOTION_EDGE_LUT_07.u32);

     return;
}
//addr=0x237c
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_08(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut3f ,HI_U32 motion_edge_lut3e ,HI_U32 motion_edge_lut3d ,HI_U32 motion_edge_lut3c ,HI_U32 motion_edge_lut3b ,HI_U32 motion_edge_lut3a ,HI_U32 motion_edge_lut39 ,HI_U32 motion_edge_lut38 )
{
     U_MOTION_EDGE_LUT_08 MOTION_EDGE_LUT_08;

     MOTION_EDGE_LUT_08.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_08.u32));
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3f = motion_edge_lut3f ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3e = motion_edge_lut3e ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3d = motion_edge_lut3d ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3c = motion_edge_lut3c ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3b = motion_edge_lut3b ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut3a = motion_edge_lut3a ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut39 = motion_edge_lut39 ;
     MOTION_EDGE_LUT_08.bits.motion_edge_lut38 = motion_edge_lut38 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_08.u32), MOTION_EDGE_LUT_08.u32);

     return;
}
//addr=0x2380
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_09(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut47 ,HI_U32 motion_edge_lut46 ,HI_U32 motion_edge_lut45 ,HI_U32 motion_edge_lut44 ,HI_U32 motion_edge_lut43 ,HI_U32 motion_edge_lut42 ,HI_U32 motion_edge_lut41 ,HI_U32 motion_edge_lut40 )
{
     U_MOTION_EDGE_LUT_09 MOTION_EDGE_LUT_09;

     MOTION_EDGE_LUT_09.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_09.u32));
     MOTION_EDGE_LUT_09.bits.motion_edge_lut47 = motion_edge_lut47 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut46 = motion_edge_lut46 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut45 = motion_edge_lut45 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut44 = motion_edge_lut44 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut43 = motion_edge_lut43 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut42 = motion_edge_lut42 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut41 = motion_edge_lut41 ;
     MOTION_EDGE_LUT_09.bits.motion_edge_lut40 = motion_edge_lut40 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_09.u32), MOTION_EDGE_LUT_09.u32);

     return;
}
//addr=0x2384
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut4f ,HI_U32 motion_edge_lut4e ,HI_U32 motion_edge_lut4d ,HI_U32 motion_edge_lut4c ,HI_U32 motion_edge_lut4b ,HI_U32 motion_edge_lut4a ,HI_U32 motion_edge_lut49 ,HI_U32 motion_edge_lut48 )
{
     U_MOTION_EDGE_LUT_10 MOTION_EDGE_LUT_10;

     MOTION_EDGE_LUT_10.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_10.u32));
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4f = motion_edge_lut4f ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4e = motion_edge_lut4e ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4d = motion_edge_lut4d ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4c = motion_edge_lut4c ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4b = motion_edge_lut4b ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut4a = motion_edge_lut4a ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut49 = motion_edge_lut49 ;
     MOTION_EDGE_LUT_10.bits.motion_edge_lut48 = motion_edge_lut48 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_10.u32), MOTION_EDGE_LUT_10.u32);

     return;
}
//addr=0x2388
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut57 ,HI_U32 motion_edge_lut56 ,HI_U32 motion_edge_lut55 ,HI_U32 motion_edge_lut54 ,HI_U32 motion_edge_lut53 ,HI_U32 motion_edge_lut52 ,HI_U32 motion_edge_lut51 ,HI_U32 motion_edge_lut50 )
{
     U_MOTION_EDGE_LUT_11 MOTION_EDGE_LUT_11;

     MOTION_EDGE_LUT_11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_11.u32));
     MOTION_EDGE_LUT_11.bits.motion_edge_lut57 = motion_edge_lut57 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut56 = motion_edge_lut56 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut55 = motion_edge_lut55 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut54 = motion_edge_lut54 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut53 = motion_edge_lut53 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut52 = motion_edge_lut52 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut51 = motion_edge_lut51 ;
     MOTION_EDGE_LUT_11.bits.motion_edge_lut50 = motion_edge_lut50 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_11.u32), MOTION_EDGE_LUT_11.u32);

     return;
}
//addr=0x238c
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut5f ,HI_U32 motion_edge_lut5e ,HI_U32 motion_edge_lut5d ,HI_U32 motion_edge_lut5c ,HI_U32 motion_edge_lut5b ,HI_U32 motion_edge_lut5a ,HI_U32 motion_edge_lut59 ,HI_U32 motion_edge_lut58 )
{
     U_MOTION_EDGE_LUT_12 MOTION_EDGE_LUT_12;

     MOTION_EDGE_LUT_12.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_12.u32));
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5f = motion_edge_lut5f ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5e = motion_edge_lut5e ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5d = motion_edge_lut5d ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5c = motion_edge_lut5c ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5b = motion_edge_lut5b ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut5a = motion_edge_lut5a ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut59 = motion_edge_lut59 ;
     MOTION_EDGE_LUT_12.bits.motion_edge_lut58 = motion_edge_lut58 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_12.u32), MOTION_EDGE_LUT_12.u32);

     return;
}
//addr=0x2390
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut67 ,HI_U32 motion_edge_lut66 ,HI_U32 motion_edge_lut65 ,HI_U32 motion_edge_lut64 ,HI_U32 motion_edge_lut63 ,HI_U32 motion_edge_lut62 ,HI_U32 motion_edge_lut61 ,HI_U32 motion_edge_lut60 )
{
     U_MOTION_EDGE_LUT_13 MOTION_EDGE_LUT_13;

     MOTION_EDGE_LUT_13.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_13.u32));
     MOTION_EDGE_LUT_13.bits.motion_edge_lut67 = motion_edge_lut67 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut66 = motion_edge_lut66 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut65 = motion_edge_lut65 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut64 = motion_edge_lut64 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut63 = motion_edge_lut63 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut62 = motion_edge_lut62 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut61 = motion_edge_lut61 ;
     MOTION_EDGE_LUT_13.bits.motion_edge_lut60 = motion_edge_lut60 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_13.u32), MOTION_EDGE_LUT_13.u32);

     return;
}
//addr=0x2394
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut6f ,HI_U32 motion_edge_lut6e ,HI_U32 motion_edge_lut6d ,HI_U32 motion_edge_lut6c ,HI_U32 motion_edge_lut6b ,HI_U32 motion_edge_lut6a ,HI_U32 motion_edge_lut69 ,HI_U32 motion_edge_lut68 )
{
     U_MOTION_EDGE_LUT_14 MOTION_EDGE_LUT_14;

     MOTION_EDGE_LUT_14.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_14.u32));
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6f = motion_edge_lut6f ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6e = motion_edge_lut6e ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6d = motion_edge_lut6d ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6c = motion_edge_lut6c ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6b = motion_edge_lut6b ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut6a = motion_edge_lut6a ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut69 = motion_edge_lut69 ;
     MOTION_EDGE_LUT_14.bits.motion_edge_lut68 = motion_edge_lut68 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_14.u32), MOTION_EDGE_LUT_14.u32);

     return;
}
//addr=0x2398
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut77 ,HI_U32 motion_edge_lut76 ,HI_U32 motion_edge_lut75 ,HI_U32 motion_edge_lut74 ,HI_U32 motion_edge_lut73 ,HI_U32 motion_edge_lut72 ,HI_U32 motion_edge_lut71 ,HI_U32 motion_edge_lut70 )
{
     U_MOTION_EDGE_LUT_15 MOTION_EDGE_LUT_15;

     MOTION_EDGE_LUT_15.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_15.u32));
     MOTION_EDGE_LUT_15.bits.motion_edge_lut77 = motion_edge_lut77 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut76 = motion_edge_lut76 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut75 = motion_edge_lut75 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut74 = motion_edge_lut74 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut73 = motion_edge_lut73 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut72 = motion_edge_lut72 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut71 = motion_edge_lut71 ;
     MOTION_EDGE_LUT_15.bits.motion_edge_lut70 = motion_edge_lut70 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_15.u32), MOTION_EDGE_LUT_15.u32);

     return;
}
//addr=0x239c
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut7f ,HI_U32 motion_edge_lut7e ,HI_U32 motion_edge_lut7d ,HI_U32 motion_edge_lut7c ,HI_U32 motion_edge_lut7b ,HI_U32 motion_edge_lut7a ,HI_U32 motion_edge_lut79 ,HI_U32 motion_edge_lut78 )
{
     U_MOTION_EDGE_LUT_16 MOTION_EDGE_LUT_16;

     MOTION_EDGE_LUT_16.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_16.u32));
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7f = motion_edge_lut7f ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7e = motion_edge_lut7e ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7d = motion_edge_lut7d ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7c = motion_edge_lut7c ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7b = motion_edge_lut7b ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut7a = motion_edge_lut7a ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut79 = motion_edge_lut79 ;
     MOTION_EDGE_LUT_16.bits.motion_edge_lut78 = motion_edge_lut78 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_16.u32), MOTION_EDGE_LUT_16.u32);

     return;
}
//addr=0x23a0
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut87 ,HI_U32 motion_edge_lut86 ,HI_U32 motion_edge_lut85 ,HI_U32 motion_edge_lut84 ,HI_U32 motion_edge_lut83 ,HI_U32 motion_edge_lut82 ,HI_U32 motion_edge_lut81 ,HI_U32 motion_edge_lut80 )
{
     U_MOTION_EDGE_LUT_17 MOTION_EDGE_LUT_17;

     MOTION_EDGE_LUT_17.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_17.u32));
     MOTION_EDGE_LUT_17.bits.motion_edge_lut87 = motion_edge_lut87 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut86 = motion_edge_lut86 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut85 = motion_edge_lut85 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut84 = motion_edge_lut84 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut83 = motion_edge_lut83 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut82 = motion_edge_lut82 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut81 = motion_edge_lut81 ;
     MOTION_EDGE_LUT_17.bits.motion_edge_lut80 = motion_edge_lut80 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_17.u32), MOTION_EDGE_LUT_17.u32);

     return;
}
//addr=0x23a4
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut8f ,HI_U32 motion_edge_lut8e ,HI_U32 motion_edge_lut8d ,HI_U32 motion_edge_lut8c ,HI_U32 motion_edge_lut8b ,HI_U32 motion_edge_lut8a ,HI_U32 motion_edge_lut89 ,HI_U32 motion_edge_lut88 )
{
     U_MOTION_EDGE_LUT_18 MOTION_EDGE_LUT_18;

     MOTION_EDGE_LUT_18.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_18.u32));
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8f = motion_edge_lut8f ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8e = motion_edge_lut8e ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8d = motion_edge_lut8d ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8c = motion_edge_lut8c ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8b = motion_edge_lut8b ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut8a = motion_edge_lut8a ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut89 = motion_edge_lut89 ;
     MOTION_EDGE_LUT_18.bits.motion_edge_lut88 = motion_edge_lut88 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_18.u32), MOTION_EDGE_LUT_18.u32);

     return;
}
//addr=0x23a8
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut97 ,HI_U32 motion_edge_lut96 ,HI_U32 motion_edge_lut95 ,HI_U32 motion_edge_lut94 ,HI_U32 motion_edge_lut93 ,HI_U32 motion_edge_lut92 ,HI_U32 motion_edge_lut91 ,HI_U32 motion_edge_lut90 )
{
     U_MOTION_EDGE_LUT_19 MOTION_EDGE_LUT_19;

     MOTION_EDGE_LUT_19.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_19.u32));
     MOTION_EDGE_LUT_19.bits.motion_edge_lut97 = motion_edge_lut97 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut96 = motion_edge_lut96 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut95 = motion_edge_lut95 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut94 = motion_edge_lut94 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut93 = motion_edge_lut93 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut92 = motion_edge_lut92 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut91 = motion_edge_lut91 ;
     MOTION_EDGE_LUT_19.bits.motion_edge_lut90 = motion_edge_lut90 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_19.u32), MOTION_EDGE_LUT_19.u32);

     return;
}
//addr=0x23ac
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut9f ,HI_U32 motion_edge_lut9e ,HI_U32 motion_edge_lut9d ,HI_U32 motion_edge_lut9c ,HI_U32 motion_edge_lut9b ,HI_U32 motion_edge_lut9a ,HI_U32 motion_edge_lut99 ,HI_U32 motion_edge_lut98 )
{
     U_MOTION_EDGE_LUT_20 MOTION_EDGE_LUT_20;

     MOTION_EDGE_LUT_20.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_20.u32));
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9f = motion_edge_lut9f ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9e = motion_edge_lut9e ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9d = motion_edge_lut9d ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9c = motion_edge_lut9c ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9b = motion_edge_lut9b ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut9a = motion_edge_lut9a ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut99 = motion_edge_lut99 ;
     MOTION_EDGE_LUT_20.bits.motion_edge_lut98 = motion_edge_lut98 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_20.u32), MOTION_EDGE_LUT_20.u32);

     return;
}
//addr=0x23b0
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_luta7 ,HI_U32 motion_edge_luta6 ,HI_U32 motion_edge_luta5 ,HI_U32 motion_edge_luta4 ,HI_U32 motion_edge_luta3 ,HI_U32 motion_edge_luta2 ,HI_U32 motion_edge_luta1 ,HI_U32 motion_edge_luta0 )
{
     U_MOTION_EDGE_LUT_21 MOTION_EDGE_LUT_21;

     MOTION_EDGE_LUT_21.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_21.u32));
     MOTION_EDGE_LUT_21.bits.motion_edge_luta7 = motion_edge_luta7 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta6 = motion_edge_luta6 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta5 = motion_edge_luta5 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta4 = motion_edge_luta4 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta3 = motion_edge_luta3 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta2 = motion_edge_luta2 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta1 = motion_edge_luta1 ;
     MOTION_EDGE_LUT_21.bits.motion_edge_luta0 = motion_edge_luta0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_21.u32), MOTION_EDGE_LUT_21.u32);

     return;
}
//addr=0x23b4
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutaf ,HI_U32 motion_edge_lutae ,HI_U32 motion_edge_lutad ,HI_U32 motion_edge_lutac ,HI_U32 motion_edge_lutab ,HI_U32 motion_edge_lutaa ,HI_U32 motion_edge_luta9 ,HI_U32 motion_edge_luta8 )
{
     U_MOTION_EDGE_LUT_22 MOTION_EDGE_LUT_22;

     MOTION_EDGE_LUT_22.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_22.u32));
     MOTION_EDGE_LUT_22.bits.motion_edge_lutaf = motion_edge_lutaf ;
     MOTION_EDGE_LUT_22.bits.motion_edge_lutae = motion_edge_lutae ;
     MOTION_EDGE_LUT_22.bits.motion_edge_lutad = motion_edge_lutad ;
     MOTION_EDGE_LUT_22.bits.motion_edge_lutac = motion_edge_lutac ;
     MOTION_EDGE_LUT_22.bits.motion_edge_lutab = motion_edge_lutab ;
     MOTION_EDGE_LUT_22.bits.motion_edge_lutaa = motion_edge_lutaa ;
     MOTION_EDGE_LUT_22.bits.motion_edge_luta9 = motion_edge_luta9 ;
     MOTION_EDGE_LUT_22.bits.motion_edge_luta8 = motion_edge_luta8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_22.u32), MOTION_EDGE_LUT_22.u32);

     return;
}
//addr=0x23b8
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutb7 ,HI_U32 motion_edge_lutb6 ,HI_U32 motion_edge_lutb5 ,HI_U32 motion_edge_lutb4 ,HI_U32 motion_edge_lutb3 ,HI_U32 motion_edge_lutb2 ,HI_U32 motion_edge_lutb1 ,HI_U32 motion_edge_lutb0 )
{
     U_MOTION_EDGE_LUT_23 MOTION_EDGE_LUT_23;

     MOTION_EDGE_LUT_23.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_23.u32));
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb7 = motion_edge_lutb7 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb6 = motion_edge_lutb6 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb5 = motion_edge_lutb5 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb4 = motion_edge_lutb4 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb3 = motion_edge_lutb3 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb2 = motion_edge_lutb2 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb1 = motion_edge_lutb1 ;
     MOTION_EDGE_LUT_23.bits.motion_edge_lutb0 = motion_edge_lutb0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_23.u32), MOTION_EDGE_LUT_23.u32);

     return;
}
//addr=0x23bc
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutbf ,HI_U32 motion_edge_lutbe ,HI_U32 motion_edge_lutbd ,HI_U32 motion_edge_lutbc ,HI_U32 motion_edge_lutbb ,HI_U32 motion_edge_lutba ,HI_U32 motion_edge_lutb9 ,HI_U32 motion_edge_lutb8 )
{
     U_MOTION_EDGE_LUT_24 MOTION_EDGE_LUT_24;

     MOTION_EDGE_LUT_24.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_24.u32));
     MOTION_EDGE_LUT_24.bits.motion_edge_lutbf = motion_edge_lutbf ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutbe = motion_edge_lutbe ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutbd = motion_edge_lutbd ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutbc = motion_edge_lutbc ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutbb = motion_edge_lutbb ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutba = motion_edge_lutba ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutb9 = motion_edge_lutb9 ;
     MOTION_EDGE_LUT_24.bits.motion_edge_lutb8 = motion_edge_lutb8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_24.u32), MOTION_EDGE_LUT_24.u32);

     return;
}
//addr=0x23c0
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutc7 ,HI_U32 motion_edge_lutc6 ,HI_U32 motion_edge_lutc5 ,HI_U32 motion_edge_lutc4 ,HI_U32 motion_edge_lutc3 ,HI_U32 motion_edge_lutc2 ,HI_U32 motion_edge_lutc1 ,HI_U32 motion_edge_lutc0 )
{
     U_MOTION_EDGE_LUT_25 MOTION_EDGE_LUT_25;

     MOTION_EDGE_LUT_25.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_25.u32));
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc7 = motion_edge_lutc7 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc6 = motion_edge_lutc6 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc5 = motion_edge_lutc5 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc4 = motion_edge_lutc4 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc3 = motion_edge_lutc3 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc2 = motion_edge_lutc2 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc1 = motion_edge_lutc1 ;
     MOTION_EDGE_LUT_25.bits.motion_edge_lutc0 = motion_edge_lutc0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_25.u32), MOTION_EDGE_LUT_25.u32);

     return;
}
//addr=0x23c4
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutcf ,HI_U32 motion_edge_lutce ,HI_U32 motion_edge_lutcd ,HI_U32 motion_edge_lutcc ,HI_U32 motion_edge_lutcb ,HI_U32 motion_edge_lutca ,HI_U32 motion_edge_lutc9 ,HI_U32 motion_edge_lutc8 )
{
     U_MOTION_EDGE_LUT_26 MOTION_EDGE_LUT_26;

     MOTION_EDGE_LUT_26.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_26.u32));
     MOTION_EDGE_LUT_26.bits.motion_edge_lutcf = motion_edge_lutcf ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutce = motion_edge_lutce ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutcd = motion_edge_lutcd ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutcc = motion_edge_lutcc ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutcb = motion_edge_lutcb ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutca = motion_edge_lutca ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutc9 = motion_edge_lutc9 ;
     MOTION_EDGE_LUT_26.bits.motion_edge_lutc8 = motion_edge_lutc8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_26.u32), MOTION_EDGE_LUT_26.u32);

     return;
}
//addr=0x23c8
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutd7 ,HI_U32 motion_edge_lutd6 ,HI_U32 motion_edge_lutd5 ,HI_U32 motion_edge_lutd4 ,HI_U32 motion_edge_lutd3 ,HI_U32 motion_edge_lutd2 ,HI_U32 motion_edge_lutd1 ,HI_U32 motion_edge_lutd0 )
{
     U_MOTION_EDGE_LUT_27 MOTION_EDGE_LUT_27;

     MOTION_EDGE_LUT_27.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_27.u32));
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd7 = motion_edge_lutd7 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd6 = motion_edge_lutd6 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd5 = motion_edge_lutd5 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd4 = motion_edge_lutd4 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd3 = motion_edge_lutd3 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd2 = motion_edge_lutd2 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd1 = motion_edge_lutd1 ;
     MOTION_EDGE_LUT_27.bits.motion_edge_lutd0 = motion_edge_lutd0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_27.u32), MOTION_EDGE_LUT_27.u32);

     return;
}
//addr=0x23cc
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutdf ,HI_U32 motion_edge_lutde ,HI_U32 motion_edge_lutdd ,HI_U32 motion_edge_lutdc ,HI_U32 motion_edge_lutdb ,HI_U32 motion_edge_lutda ,HI_U32 motion_edge_lutd9 ,HI_U32 motion_edge_lutd8 )
{
     U_MOTION_EDGE_LUT_28 MOTION_EDGE_LUT_28;

     MOTION_EDGE_LUT_28.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_28.u32));
     MOTION_EDGE_LUT_28.bits.motion_edge_lutdf = motion_edge_lutdf ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutde = motion_edge_lutde ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutdd = motion_edge_lutdd ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutdc = motion_edge_lutdc ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutdb = motion_edge_lutdb ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutda = motion_edge_lutda ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutd9 = motion_edge_lutd9 ;
     MOTION_EDGE_LUT_28.bits.motion_edge_lutd8 = motion_edge_lutd8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_28.u32), MOTION_EDGE_LUT_28.u32);

     return;
}
//addr=0x23d0
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lute7 ,HI_U32 motion_edge_lute6 ,HI_U32 motion_edge_lute5 ,HI_U32 motion_edge_lute4 ,HI_U32 motion_edge_lute3 ,HI_U32 motion_edge_lute2 ,HI_U32 motion_edge_lute1 ,HI_U32 motion_edge_lute0 )
{
     U_MOTION_EDGE_LUT_29 MOTION_EDGE_LUT_29;

     MOTION_EDGE_LUT_29.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_29.u32));
     MOTION_EDGE_LUT_29.bits.motion_edge_lute7 = motion_edge_lute7 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute6 = motion_edge_lute6 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute5 = motion_edge_lute5 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute4 = motion_edge_lute4 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute3 = motion_edge_lute3 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute2 = motion_edge_lute2 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute1 = motion_edge_lute1 ;
     MOTION_EDGE_LUT_29.bits.motion_edge_lute0 = motion_edge_lute0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_29.u32), MOTION_EDGE_LUT_29.u32);

     return;
}
//addr=0x23d4
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutef ,HI_U32 motion_edge_lutee ,HI_U32 motion_edge_luted ,HI_U32 motion_edge_lutec ,HI_U32 motion_edge_luteb ,HI_U32 motion_edge_lutea ,HI_U32 motion_edge_lute9 ,HI_U32 motion_edge_lute8 )
{
     U_MOTION_EDGE_LUT_30 MOTION_EDGE_LUT_30;

     MOTION_EDGE_LUT_30.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_30.u32));
     MOTION_EDGE_LUT_30.bits.motion_edge_lutef = motion_edge_lutef ;
     MOTION_EDGE_LUT_30.bits.motion_edge_lutee = motion_edge_lutee ;
     MOTION_EDGE_LUT_30.bits.motion_edge_luted = motion_edge_luted ;
     MOTION_EDGE_LUT_30.bits.motion_edge_lutec = motion_edge_lutec ;
     MOTION_EDGE_LUT_30.bits.motion_edge_luteb = motion_edge_luteb ;
     MOTION_EDGE_LUT_30.bits.motion_edge_lutea = motion_edge_lutea ;
     MOTION_EDGE_LUT_30.bits.motion_edge_lute9 = motion_edge_lute9 ;
     MOTION_EDGE_LUT_30.bits.motion_edge_lute8 = motion_edge_lute8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_30.u32), MOTION_EDGE_LUT_30.u32);

     return;
}
//addr=0x23d8
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutf7 ,HI_U32 motion_edge_lutf6 ,HI_U32 motion_edge_lutf5 ,HI_U32 motion_edge_lutf4 ,HI_U32 motion_edge_lutf3 ,HI_U32 motion_edge_lutf2 ,HI_U32 motion_edge_lutf1 ,HI_U32 motion_edge_lutf0 )
{
     U_MOTION_EDGE_LUT_31 MOTION_EDGE_LUT_31;

     MOTION_EDGE_LUT_31.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_31.u32));
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf7 = motion_edge_lutf7 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf6 = motion_edge_lutf6 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf5 = motion_edge_lutf5 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf4 = motion_edge_lutf4 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf3 = motion_edge_lutf3 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf2 = motion_edge_lutf2 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf1 = motion_edge_lutf1 ;
     MOTION_EDGE_LUT_31.bits.motion_edge_lutf0 = motion_edge_lutf0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_31.u32), MOTION_EDGE_LUT_31.u32);

     return;
}
//addr=0x23dc
HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_32(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutff ,HI_U32 motion_edge_lutfe ,HI_U32 motion_edge_lutfd ,HI_U32 motion_edge_lutfc ,HI_U32 motion_edge_lutfb ,HI_U32 motion_edge_lutfa ,HI_U32 motion_edge_lutf9 ,HI_U32 motion_edge_lutf8 )
{
     U_MOTION_EDGE_LUT_32 MOTION_EDGE_LUT_32;

     MOTION_EDGE_LUT_32.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_32.u32));
     MOTION_EDGE_LUT_32.bits.motion_edge_lutff = motion_edge_lutff ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutfe = motion_edge_lutfe ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutfd = motion_edge_lutfd ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutfc = motion_edge_lutfc ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutfb = motion_edge_lutfb ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutfa = motion_edge_lutfa ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutf9 = motion_edge_lutf9 ;
     MOTION_EDGE_LUT_32.bits.motion_edge_lutf8 = motion_edge_lutf8 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->MOTION_EDGE_LUT_32.u32), MOTION_EDGE_LUT_32.u32);

     return;
}
//addr=0x3800
HI_VOID VPSS_DRV_Set_VPSS_TNR_CONTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 marketcoor ,HI_U32 marketmode ,HI_U32 marketmodeen ,HI_U32 sharemotionen ,HI_U32 mcnr_bypass_en ,HI_U32 cnren ,HI_U32 ynren ,HI_U32 nren )
{
     U_VPSS_TNR_CONTRL VPSS_TNR_CONTRL;

     VPSS_TNR_CONTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CONTRL.u32));
     VPSS_TNR_CONTRL.bits.marketcoor     = marketcoor     ;
     VPSS_TNR_CONTRL.bits.marketmode     = marketmode     ;
     VPSS_TNR_CONTRL.bits.marketmodeen   = marketmodeen   ;
     VPSS_TNR_CONTRL.bits.sharemotionen  = sharemotionen  ;
     VPSS_TNR_CONTRL.bits.mcnr_bypass_en = mcnr_bypass_en ;
     VPSS_TNR_CONTRL.bits.cnren          = cnren          ;
     VPSS_TNR_CONTRL.bits.ynren          = ynren          ;
     VPSS_TNR_CONTRL.bits.nren           = nren           ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CONTRL.u32), VPSS_TNR_CONTRL.u32);

     return;
}
//addr=0x3804
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMD_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 alpha2 ,HI_U32 alpha1 ,HI_U32 cmotionlpfmode ,HI_U32 ymotionlpfmode ,HI_U32 cmotioncalcmode ,HI_U32 ymotioncalcmode ,HI_U32 meancadjshift ,HI_U32 meanyadjshift ,HI_U32 mean_cmotion_adj_en ,HI_U32 mean_ymotion_adj_en ,HI_U32 mdprelpfen )
{
     U_VPSS_TNR_MAMD_CTRL VPSS_TNR_MAMD_CTRL;

     VPSS_TNR_MAMD_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMD_CTRL.u32));
     VPSS_TNR_MAMD_CTRL.bits.alpha2              = alpha2              ;
     VPSS_TNR_MAMD_CTRL.bits.alpha1              = alpha1              ;
     VPSS_TNR_MAMD_CTRL.bits.cmotionlpfmode      = cmotionlpfmode      ;
     VPSS_TNR_MAMD_CTRL.bits.ymotionlpfmode      = ymotionlpfmode      ;
     VPSS_TNR_MAMD_CTRL.bits.cmotioncalcmode     = cmotioncalcmode     ;
     VPSS_TNR_MAMD_CTRL.bits.ymotioncalcmode     = ymotioncalcmode     ;
     VPSS_TNR_MAMD_CTRL.bits.meancadjshift       = meancadjshift       ;
     VPSS_TNR_MAMD_CTRL.bits.meanyadjshift       = meanyadjshift       ;
     VPSS_TNR_MAMD_CTRL.bits.mean_cmotion_adj_en = mean_cmotion_adj_en ;
     VPSS_TNR_MAMD_CTRL.bits.mean_ymotion_adj_en = mean_ymotion_adj_en ;
     VPSS_TNR_MAMD_CTRL.bits.mdprelpfen          = mdprelpfen          ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMD_CTRL.u32), VPSS_TNR_MAMD_CTRL.u32);

     return;
}
//addr=0x3808
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMD_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmdcore ,HI_U32 cmdgain ,HI_U32 ymdcore ,HI_U32 ymdgain )
{
     U_VPSS_TNR_MAMD_GAIN VPSS_TNR_MAMD_GAIN;

     VPSS_TNR_MAMD_GAIN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMD_GAIN.u32));
     VPSS_TNR_MAMD_GAIN.bits.cmdcore    = cmdcore    ;
     VPSS_TNR_MAMD_GAIN.bits.cmdgain    = cmdgain    ;
     VPSS_TNR_MAMD_GAIN.bits.ymdcore    = ymdcore    ;
     VPSS_TNR_MAMD_GAIN.bits.ymdgain    = ymdgain    ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMD_GAIN.u32), VPSS_TNR_MAMD_GAIN.u32);

     return;
}
//addr=0x380c
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAND_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkdiffthd ,HI_U32 noisedetecten )
{
     U_VPSS_TNR_MAND_CTRL VPSS_TNR_MAND_CTRL;

     VPSS_TNR_MAND_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAND_CTRL.u32));
     VPSS_TNR_MAND_CTRL.bits.blkdiffthd    = blkdiffthd    ;
     VPSS_TNR_MAND_CTRL.bits.noisedetecten = noisedetecten ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAND_CTRL.u32), VPSS_TNR_MAND_CTRL.u32);

     return;
}
//addr=0x3810
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAND_THD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 flatthdmax ,HI_U32 flatthdmed ,HI_U32 flatthdmin )
{
     U_VPSS_TNR_MAND_THD VPSS_TNR_MAND_THD;

     VPSS_TNR_MAND_THD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAND_THD.u32));
     VPSS_TNR_MAND_THD.bits.flatthdmax = flatthdmax ;
     VPSS_TNR_MAND_THD.bits.flatthdmed = flatthdmed ;
     VPSS_TNR_MAND_THD.bits.flatthdmin = flatthdmin ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAND_THD.u32), VPSS_TNR_MAND_THD.u32);

     return;
}
//addr=0x3814
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 gm_adj ,HI_U32 mvlpfmode ,HI_U32 mvrefmode ,HI_U32 motionestimateen )
{
     U_VPSS_TNR_MAME_CTRL VPSS_TNR_MAME_CTRL;

     VPSS_TNR_MAME_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_CTRL.u32));
     VPSS_TNR_MAME_CTRL.bits.gm_adj           = gm_adj           ;
     VPSS_TNR_MAME_CTRL.bits.mvlpfmode        = mvlpfmode        ;
     VPSS_TNR_MAME_CTRL.bits.mvrefmode        = mvrefmode        ;
     VPSS_TNR_MAME_CTRL.bits.motionestimateen = motionestimateen ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_CTRL.u32), VPSS_TNR_MAME_CTRL.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_CTRL_mcTnr(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 bEnable )
{
     U_VPSS_TNR_MAME_CTRL VPSS_TNR_MAME_CTRL;

     VPSS_TNR_MAME_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_CTRL.u32));

     VPSS_TNR_MAME_CTRL.bits.motionestimateen = bEnable ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_CTRL.u32), VPSS_TNR_MAME_CTRL.u32);

     return;
}

//addr=0x3818
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mag_pnl_core_xmv ,HI_U32 mag_pnl_gain_xmv ,HI_U32 mag_pnl_core_0mv ,HI_U32 mag_pnl_gain_0mv )
{
     U_VPSS_TNR_MAME_GAIN VPSS_TNR_MAME_GAIN;

     VPSS_TNR_MAME_GAIN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_GAIN.u32));
     VPSS_TNR_MAME_GAIN.bits.mag_pnl_core_xmv = mag_pnl_core_xmv ;
     VPSS_TNR_MAME_GAIN.bits.mag_pnl_gain_xmv = mag_pnl_gain_xmv ;
     VPSS_TNR_MAME_GAIN.bits.mag_pnl_core_0mv = mag_pnl_core_0mv ;
     VPSS_TNR_MAME_GAIN.bits.mag_pnl_gain_0mv = mag_pnl_gain_0mv ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_GAIN.u32), VPSS_TNR_MAME_GAIN.u32);

     return;
}
//addr=0x381c
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_STD_CORE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 std_pnl_core_xmv ,HI_U32 std_pnl_gain_xmv ,HI_U32 std_pnl_core_0mv ,HI_U32 std_pnl_gain_0mv ,HI_U32 std_core_xmv ,HI_U32 std_core_0mv )
{
     U_VPSS_TNR_MAME_STD_CORE VPSS_TNR_MAME_STD_CORE;

     VPSS_TNR_MAME_STD_CORE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_STD_CORE.u32));
     VPSS_TNR_MAME_STD_CORE.bits.std_pnl_core_xmv = std_pnl_core_xmv ;
     VPSS_TNR_MAME_STD_CORE.bits.std_pnl_gain_xmv = std_pnl_gain_xmv ;
     VPSS_TNR_MAME_STD_CORE.bits.std_pnl_core_0mv = std_pnl_core_0mv ;
     VPSS_TNR_MAME_STD_CORE.bits.std_pnl_gain_0mv = std_pnl_gain_0mv ;
     VPSS_TNR_MAME_STD_CORE.bits.std_core_xmv     = std_core_xmv     ;
     VPSS_TNR_MAME_STD_CORE.bits.std_core_0mv     = std_core_0mv     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_STD_CORE.u32), VPSS_TNR_MAME_STD_CORE.u32);

     return;
}
//addr=0x3820
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_STD_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 std_pnl_core ,HI_U32 std_pnl_gain )
{
     U_VPSS_TNR_MAME_STD_GAIN VPSS_TNR_MAME_STD_GAIN;

     VPSS_TNR_MAME_STD_GAIN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_STD_GAIN.u32));
     VPSS_TNR_MAME_STD_GAIN.bits.std_pnl_core = std_pnl_core ;
     VPSS_TNR_MAME_STD_GAIN.bits.std_pnl_gain = std_pnl_gain ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_STD_GAIN.u32), VPSS_TNR_MAME_STD_GAIN.u32);

     return;
}
//addr=0x3824
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_MV_THD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 adj_xmv_max ,HI_U32 adj_0mv_max ,HI_U32 adj_xmv_min ,HI_U32 adj_0mv_min ,HI_U32 adj_mv_min ,HI_U32 adj_mv_max )
{
     U_VPSS_TNR_MAME_MV_THD VPSS_TNR_MAME_MV_THD;

     VPSS_TNR_MAME_MV_THD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_MV_THD.u32));
     VPSS_TNR_MAME_MV_THD.bits.adj_xmv_max = adj_xmv_max ;
     VPSS_TNR_MAME_MV_THD.bits.adj_0mv_max = adj_0mv_max ;
     VPSS_TNR_MAME_MV_THD.bits.adj_xmv_min = adj_xmv_min ;
     VPSS_TNR_MAME_MV_THD.bits.adj_0mv_min = adj_0mv_min ;
     VPSS_TNR_MAME_MV_THD.bits.adj_mv_min  = adj_mv_min  ;
     VPSS_TNR_MAME_MV_THD.bits.adj_mv_max  = adj_mv_max  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAME_MV_THD.u32), VPSS_TNR_MAME_MV_THD.u32);

     return;
}
//addr=0x3828
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPEN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcrweight2 ,HI_U32 cbcrweight1 ,HI_U32 cbcrupdateen )
{
     U_VPSS_TNR_MAMC_CBCR_UPEN VPSS_TNR_MAMC_CBCR_UPEN;

     VPSS_TNR_MAMC_CBCR_UPEN.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPEN.u32));
     VPSS_TNR_MAMC_CBCR_UPEN.bits.cbcrweight2  = cbcrweight2  ;
     VPSS_TNR_MAMC_CBCR_UPEN.bits.cbcrweight1  = cbcrweight1  ;
     VPSS_TNR_MAMC_CBCR_UPEN.bits.cbcrupdateen = cbcrupdateen ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPEN.u32), VPSS_TNR_MAMC_CBCR_UPEN.u32);

     return;
}
//addr=0x382c
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 crend1 ,HI_U32 cbend1 ,HI_U32 crbegin1 ,HI_U32 cbbegin1 )
{
     U_VPSS_TNR_MAMC_CBCR_UPTHD1 VPSS_TNR_MAMC_CBCR_UPTHD1;

     VPSS_TNR_MAMC_CBCR_UPTHD1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPTHD1.u32));
     VPSS_TNR_MAMC_CBCR_UPTHD1.bits.crend1     = crend1     ;
     VPSS_TNR_MAMC_CBCR_UPTHD1.bits.cbend1     = cbend1     ;
     VPSS_TNR_MAMC_CBCR_UPTHD1.bits.crbegin1   = crbegin1   ;
     VPSS_TNR_MAMC_CBCR_UPTHD1.bits.cbbegin1   = cbbegin1   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPTHD1.u32), VPSS_TNR_MAMC_CBCR_UPTHD1.u32);

     return;
}
//addr=0x3830
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPTHD2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 crend2 ,HI_U32 cbend2 ,HI_U32 crbegin2 ,HI_U32 cbbegin2 )
{
     U_VPSS_TNR_MAMC_CBCR_UPTHD2 VPSS_TNR_MAMC_CBCR_UPTHD2;

     VPSS_TNR_MAMC_CBCR_UPTHD2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPTHD2.u32));
     VPSS_TNR_MAMC_CBCR_UPTHD2.bits.crend2     = crend2     ;
     VPSS_TNR_MAMC_CBCR_UPTHD2.bits.cbend2     = cbend2     ;
     VPSS_TNR_MAMC_CBCR_UPTHD2.bits.crbegin2   = crbegin2   ;
     VPSS_TNR_MAMC_CBCR_UPTHD2.bits.cbbegin2   = cbbegin2   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_CBCR_UPTHD2.u32), VPSS_TNR_MAMC_CBCR_UPTHD2.u32);

     return;
}
//addr=0x3834
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_PRE_MAD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motionmergeratio ,HI_U32 premotionalpha ,HI_U32 premotionmergemode )
{
     U_VPSS_TNR_MAMC_PRE_MAD VPSS_TNR_MAMC_PRE_MAD;

     VPSS_TNR_MAMC_PRE_MAD.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_PRE_MAD.u32));
     VPSS_TNR_MAMC_PRE_MAD.bits.motionmergeratio   = motionmergeratio   ;
     VPSS_TNR_MAMC_PRE_MAD.bits.premotionalpha     = premotionalpha     ;
     VPSS_TNR_MAMC_PRE_MAD.bits.premotionmergemode = premotionmergemode ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_PRE_MAD.u32), VPSS_TNR_MAMC_PRE_MAD.u32);

     return;
}
//addr=0x3838
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_MAP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotioncore ,HI_U32 cmotiongain ,HI_U32 ymotioncore ,HI_U32 ymotiongain ,HI_U32 motionmappingen )
{
     U_VPSS_TNR_MAMC_MAP VPSS_TNR_MAMC_MAP;

     VPSS_TNR_MAMC_MAP.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_MAP.u32));
     VPSS_TNR_MAMC_MAP.bits.cmotioncore     = cmotioncore     ;
     VPSS_TNR_MAMC_MAP.bits.cmotiongain     = cmotiongain     ;
     VPSS_TNR_MAMC_MAP.bits.ymotioncore     = ymotioncore     ;
     VPSS_TNR_MAMC_MAP.bits.ymotiongain     = ymotiongain     ;
     VPSS_TNR_MAMC_MAP.bits.motionmappingen = motionmappingen ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_MAP.u32), VPSS_TNR_MAMC_MAP.u32);

     return;
}
//addr=0x383c
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_SALTUS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 saltuslevel ,HI_U32 saltusdecten )
{
     U_VPSS_TNR_MAMC_SALTUS VPSS_TNR_MAMC_SALTUS;

     VPSS_TNR_MAMC_SALTUS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_SALTUS.u32));
     VPSS_TNR_MAMC_SALTUS.bits.saltuslevel  = saltuslevel  ;
     VPSS_TNR_MAMC_SALTUS.bits.saltusdecten = saltusdecten ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_SALTUS.u32), VPSS_TNR_MAMC_SALTUS.u32);

     return;
}
//addr=0x3840
HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_MCADJ(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmclpfmode ,HI_U32 ymclpfmode ,HI_U32 cmcadjen ,HI_U32 ymcadjen )
{
     U_VPSS_TNR_MAMC_MCADJ VPSS_TNR_MAMC_MCADJ;

     VPSS_TNR_MAMC_MCADJ.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_MCADJ.u32));
     VPSS_TNR_MAMC_MCADJ.bits.cmclpfmode = cmclpfmode ;
     VPSS_TNR_MAMC_MCADJ.bits.ymclpfmode = ymclpfmode ;
     VPSS_TNR_MAMC_MCADJ.bits.cmcadjen   = cmcadjen   ;
     VPSS_TNR_MAMC_MCADJ.bits.ymcadjen   = ymcadjen   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MAMC_MCADJ.u32), VPSS_TNR_MAMC_MCADJ.u32);

     return;
}
//addr=0x3844
HI_VOID VPSS_DRV_Set_VPSS_TNR_FILR_MODE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cnonrrange ,HI_U32 ynonrrange ,HI_U32 cmotionmode ,HI_U32 ymotionmode )
{
     U_VPSS_TNR_FILR_MODE VPSS_TNR_FILR_MODE;

     VPSS_TNR_FILR_MODE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FILR_MODE.u32));
     VPSS_TNR_FILR_MODE.bits.cnonrrange  = cnonrrange  ;
     VPSS_TNR_FILR_MODE.bits.ynonrrange  = ynonrrange  ;
     VPSS_TNR_FILR_MODE.bits.cmotionmode = cmotionmode ;
     VPSS_TNR_FILR_MODE.bits.ymotionmode = ymotionmode ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FILR_MODE.u32), VPSS_TNR_FILR_MODE.u32);

     return;
}

HI_VOID VPSS_DRV_Set_VPSS_TNR_FILR_MODE_ymmode(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 ymotionmode )
{
     U_VPSS_TNR_FILR_MODE VPSS_TNR_FILR_MODE;

     VPSS_TNR_FILR_MODE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FILR_MODE.u32));

     VPSS_TNR_FILR_MODE.bits.ymotionmode = ymotionmode ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FILR_MODE.u32), VPSS_TNR_FILR_MODE.u32);

     return;
}

//addr=0x3848
HI_VOID VPSS_DRV_Set_VPSS_TNR_SCENE_CHANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechangeinfo ,HI_U32 scenechangeen )
{
     U_VPSS_TNR_SCENE_CHANGE VPSS_TNR_SCENE_CHANGE;

     VPSS_TNR_SCENE_CHANGE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_SCENE_CHANGE.u32));
     VPSS_TNR_SCENE_CHANGE.bits.scenechangeinfo = scenechangeinfo ;
     VPSS_TNR_SCENE_CHANGE.bits.scenechangeen   = scenechangeen   ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_SCENE_CHANGE.u32), VPSS_TNR_SCENE_CHANGE.u32);

     return;
}
//addr=0x384c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cfg_cmdcore ,HI_U32 cfg_cmdgain ,HI_U32 cfg_ymdcore ,HI_U32 cfg_ymdgain )
{
     U_VPSS_TNR_CFG VPSS_TNR_CFG;

     VPSS_TNR_CFG.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CFG.u32));
     VPSS_TNR_CFG.bits.cfg_cmdcore = cfg_cmdcore ;
     VPSS_TNR_CFG.bits.cfg_cmdgain = cfg_cmdgain ;
     VPSS_TNR_CFG.bits.cfg_ymdcore = cfg_ymdcore ;
     VPSS_TNR_CFG.bits.cfg_ymdgain = cfg_ymdgain ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CFG.u32), VPSS_TNR_CFG.u32);

     return;
}
//addr=0x3850
HI_VOID VPSS_DRV_Set_VPSS_TNR_FLAT_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 flatinfoymax ,HI_U32 flatinfoxmax ,HI_U32 flatinfoymin ,HI_U32 flatinfoxmin )
{
     U_VPSS_TNR_FLAT_INFO VPSS_TNR_FLAT_INFO;

     VPSS_TNR_FLAT_INFO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FLAT_INFO.u32));
     VPSS_TNR_FLAT_INFO.bits.flatinfoymax = flatinfoymax ;
     VPSS_TNR_FLAT_INFO.bits.flatinfoxmax = flatinfoxmax ;
     VPSS_TNR_FLAT_INFO.bits.flatinfoymin = flatinfoymin ;
     VPSS_TNR_FLAT_INFO.bits.flatinfoxmin = flatinfoxmin ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_FLAT_INFO.u32), VPSS_TNR_FLAT_INFO.u32);

     return;
}
//addr=0x3854
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingymax ,HI_U32 yblendingxmax ,HI_U32 yblendingymin ,HI_U32 yblendingxmin )
{
     U_VPSS_TNR_YBLEND VPSS_TNR_YBLEND;

     VPSS_TNR_YBLEND.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLEND.u32));
     VPSS_TNR_YBLEND.bits.yblendingymax = yblendingymax ;
     VPSS_TNR_YBLEND.bits.yblendingxmax = yblendingxmax ;
     VPSS_TNR_YBLEND.bits.yblendingymin = yblendingymin ;
     VPSS_TNR_YBLEND.bits.yblendingxmin = yblendingxmin ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLEND.u32), VPSS_TNR_YBLEND.u32);

     return;
}
//addr=0x3858
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingymax ,HI_U32 cblendingxmax ,HI_U32 cblendingymin ,HI_U32 cblendingxmin )
{
     U_VPSS_TNR_CBLEND VPSS_TNR_CBLEND;

     VPSS_TNR_CBLEND.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLEND.u32));
     VPSS_TNR_CBLEND.bits.cblendingymax = cblendingymax ;
     VPSS_TNR_CBLEND.bits.cblendingxmax = cblendingxmax ;
     VPSS_TNR_CBLEND.bits.cblendingymin = cblendingymin ;
     VPSS_TNR_CBLEND.bits.cblendingxmin = cblendingxmin ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLEND.u32), VPSS_TNR_CBLEND.u32);

     return;
}
//addr=0x385c
HI_VOID VPSS_DRV_Set_VPSS_TNR_DTBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dtblendingymax ,HI_U32 dtblendingxmax ,HI_U32 dtblendingymin ,HI_U32 dtblendingxmin )
{
     U_VPSS_TNR_DTBLEND VPSS_TNR_DTBLEND;

     VPSS_TNR_DTBLEND.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_DTBLEND.u32));
     VPSS_TNR_DTBLEND.bits.dtblendingymax = dtblendingymax ;
     VPSS_TNR_DTBLEND.bits.dtblendingxmax = dtblendingxmax ;
     VPSS_TNR_DTBLEND.bits.dtblendingymin = dtblendingymin ;
     VPSS_TNR_DTBLEND.bits.dtblendingxmin = dtblendingxmin ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_DTBLEND.u32), VPSS_TNR_DTBLEND.u32);

     return;
}
//addr=0x3900
HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio3 ,HI_U32 mean_motion_ratio2 ,HI_U32 mean_motion_ratio1 ,HI_U32 mean_motion_ratio0 )
{
     U_VPSS_TNR_MEAN_MOTION_LUT0 VPSS_TNR_MEAN_MOTION_LUT0;

     VPSS_TNR_MEAN_MOTION_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT0.u32));
     VPSS_TNR_MEAN_MOTION_LUT0.bits.mean_motion_ratio3 = mean_motion_ratio3 ;
     VPSS_TNR_MEAN_MOTION_LUT0.bits.mean_motion_ratio2 = mean_motion_ratio2 ;
     VPSS_TNR_MEAN_MOTION_LUT0.bits.mean_motion_ratio1 = mean_motion_ratio1 ;
     VPSS_TNR_MEAN_MOTION_LUT0.bits.mean_motion_ratio0 = mean_motion_ratio0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT0.u32), VPSS_TNR_MEAN_MOTION_LUT0.u32);

     return;
}
//addr=0x3904
HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio7 ,HI_U32 mean_motion_ratio6 ,HI_U32 mean_motion_ratio5 ,HI_U32 mean_motion_ratio4 )
{
     U_VPSS_TNR_MEAN_MOTION_LUT1 VPSS_TNR_MEAN_MOTION_LUT1;

     VPSS_TNR_MEAN_MOTION_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT1.u32));
     VPSS_TNR_MEAN_MOTION_LUT1.bits.mean_motion_ratio7 = mean_motion_ratio7 ;
     VPSS_TNR_MEAN_MOTION_LUT1.bits.mean_motion_ratio6 = mean_motion_ratio6 ;
     VPSS_TNR_MEAN_MOTION_LUT1.bits.mean_motion_ratio5 = mean_motion_ratio5 ;
     VPSS_TNR_MEAN_MOTION_LUT1.bits.mean_motion_ratio4 = mean_motion_ratio4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT1.u32), VPSS_TNR_MEAN_MOTION_LUT1.u32);

     return;
}
//addr=0x3908
HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio11 ,HI_U32 mean_motion_ratio10 ,HI_U32 mean_motion_ratio9 ,HI_U32 mean_motion_ratio8 )
{
     U_VPSS_TNR_MEAN_MOTION_LUT2 VPSS_TNR_MEAN_MOTION_LUT2;

     VPSS_TNR_MEAN_MOTION_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT2.u32));
     VPSS_TNR_MEAN_MOTION_LUT2.bits.mean_motion_ratio11 = mean_motion_ratio11 ;
     VPSS_TNR_MEAN_MOTION_LUT2.bits.mean_motion_ratio10 = mean_motion_ratio10 ;
     VPSS_TNR_MEAN_MOTION_LUT2.bits.mean_motion_ratio9  = mean_motion_ratio9  ;
     VPSS_TNR_MEAN_MOTION_LUT2.bits.mean_motion_ratio8  = mean_motion_ratio8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT2.u32), VPSS_TNR_MEAN_MOTION_LUT2.u32);

     return;
}
//addr=0x390c
HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio15 ,HI_U32 mean_motion_ratio14 ,HI_U32 mean_motion_ratio13 ,HI_U32 mean_motion_ratio12 )
{
     U_VPSS_TNR_MEAN_MOTION_LUT3 VPSS_TNR_MEAN_MOTION_LUT3;

     VPSS_TNR_MEAN_MOTION_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT3.u32));
     VPSS_TNR_MEAN_MOTION_LUT3.bits.mean_motion_ratio15 = mean_motion_ratio15 ;
     VPSS_TNR_MEAN_MOTION_LUT3.bits.mean_motion_ratio14 = mean_motion_ratio14 ;
     VPSS_TNR_MEAN_MOTION_LUT3.bits.mean_motion_ratio13 = mean_motion_ratio13 ;
     VPSS_TNR_MEAN_MOTION_LUT3.bits.mean_motion_ratio12 = mean_motion_ratio12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_MEAN_MOTION_LUT3.u32), VPSS_TNR_MEAN_MOTION_LUT3.u32);

     return;
}
//addr=0x3940
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut3 ,HI_U32 yblendingalphalut2 ,HI_U32 yblendingalphalut1 ,HI_U32 yblendingalphalut0 )
{
     U_VPSS_TNR_YBLENDING_LUT0 VPSS_TNR_YBLENDING_LUT0;

     VPSS_TNR_YBLENDING_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT0.u32));
     VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut3 = yblendingalphalut3 ;
     VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut2 = yblendingalphalut2 ;
     VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut1 = yblendingalphalut1 ;
     VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut0 = yblendingalphalut0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT0.u32), VPSS_TNR_YBLENDING_LUT0.u32);

     return;
}
//addr=0x3944
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut7 ,HI_U32 yblendingalphalut6 ,HI_U32 yblendingalphalut5 ,HI_U32 yblendingalphalut4 )
{
     U_VPSS_TNR_YBLENDING_LUT1 VPSS_TNR_YBLENDING_LUT1;

     VPSS_TNR_YBLENDING_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT1.u32));
     VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut7 = yblendingalphalut7 ;
     VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut6 = yblendingalphalut6 ;
     VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut5 = yblendingalphalut5 ;
     VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut4 = yblendingalphalut4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT1.u32), VPSS_TNR_YBLENDING_LUT1.u32);

     return;
}
//addr=0x3948
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut11 ,HI_U32 yblendingalphalut10 ,HI_U32 yblendingalphalut9 ,HI_U32 yblendingalphalut8 )
{
     U_VPSS_TNR_YBLENDING_LUT2 VPSS_TNR_YBLENDING_LUT2;

     VPSS_TNR_YBLENDING_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT2.u32));
     VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut11 = yblendingalphalut11 ;
     VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut10 = yblendingalphalut10 ;
     VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut9  = yblendingalphalut9  ;
     VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut8  = yblendingalphalut8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT2.u32), VPSS_TNR_YBLENDING_LUT2.u32);

     return;
}
//addr=0x394c
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut15 ,HI_U32 yblendingalphalut14 ,HI_U32 yblendingalphalut13 ,HI_U32 yblendingalphalut12 )
{
     U_VPSS_TNR_YBLENDING_LUT3 VPSS_TNR_YBLENDING_LUT3;

     VPSS_TNR_YBLENDING_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT3.u32));
     VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut15 = yblendingalphalut15 ;
     VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut14 = yblendingalphalut14 ;
     VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut13 = yblendingalphalut13 ;
     VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut12 = yblendingalphalut12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT3.u32), VPSS_TNR_YBLENDING_LUT3.u32);

     return;
}
//addr=0x3950
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut19 ,HI_U32 yblendingalphalut18 ,HI_U32 yblendingalphalut17 ,HI_U32 yblendingalphalut16 )
{
     U_VPSS_TNR_YBLENDING_LUT4 VPSS_TNR_YBLENDING_LUT4;

     VPSS_TNR_YBLENDING_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT4.u32));
     VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut19 = yblendingalphalut19 ;
     VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut18 = yblendingalphalut18 ;
     VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut17 = yblendingalphalut17 ;
     VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut16 = yblendingalphalut16 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT4.u32), VPSS_TNR_YBLENDING_LUT4.u32);

     return;
}
//addr=0x3954
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut23 ,HI_U32 yblendingalphalut22 ,HI_U32 yblendingalphalut21 ,HI_U32 yblendingalphalut20 )
{
     U_VPSS_TNR_YBLENDING_LUT5 VPSS_TNR_YBLENDING_LUT5;

     VPSS_TNR_YBLENDING_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT5.u32));
     VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut23 = yblendingalphalut23 ;
     VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut22 = yblendingalphalut22 ;
     VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut21 = yblendingalphalut21 ;
     VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut20 = yblendingalphalut20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT5.u32), VPSS_TNR_YBLENDING_LUT5.u32);

     return;
}
//addr=0x3958
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut27 ,HI_U32 yblendingalphalut26 ,HI_U32 yblendingalphalut25 ,HI_U32 yblendingalphalut24 )
{
     U_VPSS_TNR_YBLENDING_LUT6 VPSS_TNR_YBLENDING_LUT6;

     VPSS_TNR_YBLENDING_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT6.u32));
     VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut27 = yblendingalphalut27 ;
     VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut26 = yblendingalphalut26 ;
     VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut25 = yblendingalphalut25 ;
     VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut24 = yblendingalphalut24 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT6.u32), VPSS_TNR_YBLENDING_LUT6.u32);

     return;
}
//addr=0x395c
HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut31 ,HI_U32 yblendingalphalut30 ,HI_U32 yblendingalphalut29 ,HI_U32 yblendingalphalut28 )
{
     U_VPSS_TNR_YBLENDING_LUT7 VPSS_TNR_YBLENDING_LUT7;

     VPSS_TNR_YBLENDING_LUT7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT7.u32));
     VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut31 = yblendingalphalut31 ;
     VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut30 = yblendingalphalut30 ;
     VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut29 = yblendingalphalut29 ;
     VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut28 = yblendingalphalut28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YBLENDING_LUT7.u32), VPSS_TNR_YBLENDING_LUT7.u32);

     return;
}
//addr=0x3960
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut3 ,HI_U32 cblendingalphalut2 ,HI_U32 cblendingalphalut1 ,HI_U32 cblendingalphalut0 )
{
     U_VPSS_TNR_CBLENDING_LUT0 VPSS_TNR_CBLENDING_LUT0;

     VPSS_TNR_CBLENDING_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT0.u32));
     VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut3 = cblendingalphalut3 ;
     VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut2 = cblendingalphalut2 ;
     VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut1 = cblendingalphalut1 ;
     VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut0 = cblendingalphalut0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT0.u32), VPSS_TNR_CBLENDING_LUT0.u32);

     return;
}
//addr=0x3964
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut7 ,HI_U32 cblendingalphalut6 ,HI_U32 cblendingalphalut5 ,HI_U32 cblendingalphalut4 )
{
     U_VPSS_TNR_CBLENDING_LUT1 VPSS_TNR_CBLENDING_LUT1;

     VPSS_TNR_CBLENDING_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT1.u32));
     VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut7 = cblendingalphalut7 ;
     VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut6 = cblendingalphalut6 ;
     VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut5 = cblendingalphalut5 ;
     VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut4 = cblendingalphalut4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT1.u32), VPSS_TNR_CBLENDING_LUT1.u32);

     return;
}
//addr=0x3968
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut11 ,HI_U32 cblendingalphalut10 ,HI_U32 cblendingalphalut9 ,HI_U32 cblendingalphalut8 )
{
     U_VPSS_TNR_CBLENDING_LUT2 VPSS_TNR_CBLENDING_LUT2;

     VPSS_TNR_CBLENDING_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT2.u32));
     VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut11 = cblendingalphalut11 ;
     VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut10 = cblendingalphalut10 ;
     VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut9  = cblendingalphalut9  ;
     VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut8  = cblendingalphalut8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT2.u32), VPSS_TNR_CBLENDING_LUT2.u32);

     return;
}
//addr=0x396c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut15 ,HI_U32 cblendingalphalut14 ,HI_U32 cblendingalphalut13 ,HI_U32 cblendingalphalut12 )
{
     U_VPSS_TNR_CBLENDING_LUT3 VPSS_TNR_CBLENDING_LUT3;

     VPSS_TNR_CBLENDING_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT3.u32));
     VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut15 = cblendingalphalut15 ;
     VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut14 = cblendingalphalut14 ;
     VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut13 = cblendingalphalut13 ;
     VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut12 = cblendingalphalut12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT3.u32), VPSS_TNR_CBLENDING_LUT3.u32);

     return;
}
//addr=0x3970
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut19 ,HI_U32 cblendingalphalut18 ,HI_U32 cblendingalphalut17 ,HI_U32 cblendingalphalut16 )
{
     U_VPSS_TNR_CBLENDING_LUT4 VPSS_TNR_CBLENDING_LUT4;

     VPSS_TNR_CBLENDING_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT4.u32));
     VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut19 = cblendingalphalut19 ;
     VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut18 = cblendingalphalut18 ;
     VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut17 = cblendingalphalut17 ;
     VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut16 = cblendingalphalut16 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT4.u32), VPSS_TNR_CBLENDING_LUT4.u32);

     return;
}
//addr=0x3974
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut23 ,HI_U32 cblendingalphalut22 ,HI_U32 cblendingalphalut21 ,HI_U32 cblendingalphalut20 )
{
     U_VPSS_TNR_CBLENDING_LUT5 VPSS_TNR_CBLENDING_LUT5;

     VPSS_TNR_CBLENDING_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT5.u32));
     VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut23 = cblendingalphalut23 ;
     VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut22 = cblendingalphalut22 ;
     VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut21 = cblendingalphalut21 ;
     VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut20 = cblendingalphalut20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT5.u32), VPSS_TNR_CBLENDING_LUT5.u32);

     return;
}
//addr=0x3978
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut27 ,HI_U32 cblendingalphalut26 ,HI_U32 cblendingalphalut25 ,HI_U32 cblendingalphalut24 )
{
     U_VPSS_TNR_CBLENDING_LUT6 VPSS_TNR_CBLENDING_LUT6;

     VPSS_TNR_CBLENDING_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT6.u32));
     VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut27 = cblendingalphalut27 ;
     VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut26 = cblendingalphalut26 ;
     VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut25 = cblendingalphalut25 ;
     VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut24 = cblendingalphalut24 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT6.u32), VPSS_TNR_CBLENDING_LUT6.u32);

     return;
}
//addr=0x397c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut31 ,HI_U32 cblendingalphalut30 ,HI_U32 cblendingalphalut29 ,HI_U32 cblendingalphalut28 )
{
     U_VPSS_TNR_CBLENDING_LUT7 VPSS_TNR_CBLENDING_LUT7;

     VPSS_TNR_CBLENDING_LUT7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT7.u32));
     VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut31 = cblendingalphalut31 ;
     VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut30 = cblendingalphalut30 ;
     VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut29 = cblendingalphalut29 ;
     VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut28 = cblendingalphalut28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CBLENDING_LUT7.u32), VPSS_TNR_CBLENDING_LUT7.u32);

     return;
}
//addr=0x3980
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut3 ,HI_U32 ymotionstrlut2 ,HI_U32 ymotionstrlut1 ,HI_U32 ymotionstrlut0 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT0 VPSS_TNR_YMOTIONSTR_LUT0;

     VPSS_TNR_YMOTIONSTR_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT0.u32));
     VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut3 = ymotionstrlut3 ;
     VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut2 = ymotionstrlut2 ;
     VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut1 = ymotionstrlut1 ;
     VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut0 = ymotionstrlut0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT0.u32), VPSS_TNR_YMOTIONSTR_LUT0.u32);

     return;
}
//addr=0x3984
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut7 ,HI_U32 ymotionstrlut6 ,HI_U32 ymotionstrlut5 ,HI_U32 ymotionstrlut4 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT1 VPSS_TNR_YMOTIONSTR_LUT1;

     VPSS_TNR_YMOTIONSTR_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT1.u32));
     VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut7 = ymotionstrlut7 ;
     VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut6 = ymotionstrlut6 ;
     VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut5 = ymotionstrlut5 ;
     VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut4 = ymotionstrlut4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT1.u32), VPSS_TNR_YMOTIONSTR_LUT1.u32);

     return;
}
//addr=0x3988
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut11 ,HI_U32 ymotionstrlut10 ,HI_U32 ymotionstrlut9 ,HI_U32 ymotionstrlut8 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT2 VPSS_TNR_YMOTIONSTR_LUT2;

     VPSS_TNR_YMOTIONSTR_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT2.u32));
     VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut11 = ymotionstrlut11 ;
     VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut10 = ymotionstrlut10 ;
     VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut9  = ymotionstrlut9  ;
     VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut8  = ymotionstrlut8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT2.u32), VPSS_TNR_YMOTIONSTR_LUT2.u32);

     return;
}
//addr=0x398c
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut15 ,HI_U32 ymotionstrlut14 ,HI_U32 ymotionstrlut13 ,HI_U32 ymotionstrlut12 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT3 VPSS_TNR_YMOTIONSTR_LUT3;

     VPSS_TNR_YMOTIONSTR_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT3.u32));
     VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut15 = ymotionstrlut15 ;
     VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut14 = ymotionstrlut14 ;
     VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut13 = ymotionstrlut13 ;
     VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut12 = ymotionstrlut12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT3.u32), VPSS_TNR_YMOTIONSTR_LUT3.u32);

     return;
}
//addr=0x3990
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut19 ,HI_U32 ymotionstrlut18 ,HI_U32 ymotionstrlut17 ,HI_U32 ymotionstrlut16 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT4 VPSS_TNR_YMOTIONSTR_LUT4;

     VPSS_TNR_YMOTIONSTR_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT4.u32));
     VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut19 = ymotionstrlut19 ;
     VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut18 = ymotionstrlut18 ;
     VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut17 = ymotionstrlut17 ;
     VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut16 = ymotionstrlut16 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT4.u32), VPSS_TNR_YMOTIONSTR_LUT4.u32);

     return;
}
//addr=0x3994
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut23 ,HI_U32 ymotionstrlut22 ,HI_U32 ymotionstrlut21 ,HI_U32 ymotionstrlut20 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT5 VPSS_TNR_YMOTIONSTR_LUT5;

     VPSS_TNR_YMOTIONSTR_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT5.u32));
     VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut23 = ymotionstrlut23 ;
     VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut22 = ymotionstrlut22 ;
     VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut21 = ymotionstrlut21 ;
     VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut20 = ymotionstrlut20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT5.u32), VPSS_TNR_YMOTIONSTR_LUT5.u32);

     return;
}
//addr=0x3998
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut27 ,HI_U32 ymotionstrlut26 ,HI_U32 ymotionstrlut25 ,HI_U32 ymotionstrlut24 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT6 VPSS_TNR_YMOTIONSTR_LUT6;

     VPSS_TNR_YMOTIONSTR_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT6.u32));
     VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut27 = ymotionstrlut27 ;
     VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut26 = ymotionstrlut26 ;
     VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut25 = ymotionstrlut25 ;
     VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut24 = ymotionstrlut24 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT6.u32), VPSS_TNR_YMOTIONSTR_LUT6.u32);

     return;
}
//addr=0x399c
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut31 ,HI_U32 ymotionstrlut30 ,HI_U32 ymotionstrlut29 ,HI_U32 ymotionstrlut28 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT7 VPSS_TNR_YMOTIONSTR_LUT7;

     VPSS_TNR_YMOTIONSTR_LUT7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT7.u32));
     VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut31 = ymotionstrlut31 ;
     VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut30 = ymotionstrlut30 ;
     VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut29 = ymotionstrlut29 ;
     VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut28 = ymotionstrlut28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT7.u32), VPSS_TNR_YMOTIONSTR_LUT7.u32);

     return;
}
//addr=0x39a0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut35 ,HI_U32 ymotionstrlut34 ,HI_U32 ymotionstrlut33 ,HI_U32 ymotionstrlut32 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT8 VPSS_TNR_YMOTIONSTR_LUT8;

     VPSS_TNR_YMOTIONSTR_LUT8.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT8.u32));
     VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut35 = ymotionstrlut35 ;
     VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut34 = ymotionstrlut34 ;
     VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut33 = ymotionstrlut33 ;
     VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut32 = ymotionstrlut32 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT8.u32), VPSS_TNR_YMOTIONSTR_LUT8.u32);

     return;
}
//addr=0x39a4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut39 ,HI_U32 ymotionstrlut38 ,HI_U32 ymotionstrlut37 ,HI_U32 ymotionstrlut36 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT9 VPSS_TNR_YMOTIONSTR_LUT9;

     VPSS_TNR_YMOTIONSTR_LUT9.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT9.u32));
     VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut39 = ymotionstrlut39 ;
     VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut38 = ymotionstrlut38 ;
     VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut37 = ymotionstrlut37 ;
     VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut36 = ymotionstrlut36 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT9.u32), VPSS_TNR_YMOTIONSTR_LUT9.u32);

     return;
}
//addr=0x39a8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut43 ,HI_U32 ymotionstrlut42 ,HI_U32 ymotionstrlut41 ,HI_U32 ymotionstrlut40 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT10 VPSS_TNR_YMOTIONSTR_LUT10;

     VPSS_TNR_YMOTIONSTR_LUT10.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT10.u32));
     VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut43 = ymotionstrlut43 ;
     VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut42 = ymotionstrlut42 ;
     VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut41 = ymotionstrlut41 ;
     VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut40 = ymotionstrlut40 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT10.u32), VPSS_TNR_YMOTIONSTR_LUT10.u32);

     return;
}
//addr=0x39ac
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut47 ,HI_U32 ymotionstrlut46 ,HI_U32 ymotionstrlut45 ,HI_U32 ymotionstrlut44 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT11 VPSS_TNR_YMOTIONSTR_LUT11;

     VPSS_TNR_YMOTIONSTR_LUT11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT11.u32));
     VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut47 = ymotionstrlut47 ;
     VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut46 = ymotionstrlut46 ;
     VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut45 = ymotionstrlut45 ;
     VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut44 = ymotionstrlut44 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT11.u32), VPSS_TNR_YMOTIONSTR_LUT11.u32);

     return;
}
//addr=0x39b0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut51 ,HI_U32 ymotionstrlut50 ,HI_U32 ymotionstrlut49 ,HI_U32 ymotionstrlut48 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT12 VPSS_TNR_YMOTIONSTR_LUT12;

     VPSS_TNR_YMOTIONSTR_LUT12.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT12.u32));
     VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut51 = ymotionstrlut51 ;
     VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut50 = ymotionstrlut50 ;
     VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut49 = ymotionstrlut49 ;
     VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut48 = ymotionstrlut48 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT12.u32), VPSS_TNR_YMOTIONSTR_LUT12.u32);

     return;
}
//addr=0x39b4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut55 ,HI_U32 ymotionstrlut54 ,HI_U32 ymotionstrlut53 ,HI_U32 ymotionstrlut52 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT13 VPSS_TNR_YMOTIONSTR_LUT13;

     VPSS_TNR_YMOTIONSTR_LUT13.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT13.u32));
     VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut55 = ymotionstrlut55 ;
     VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut54 = ymotionstrlut54 ;
     VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut53 = ymotionstrlut53 ;
     VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut52 = ymotionstrlut52 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT13.u32), VPSS_TNR_YMOTIONSTR_LUT13.u32);

     return;
}
//addr=0x39b8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut59 ,HI_U32 ymotionstrlut58 ,HI_U32 ymotionstrlut57 ,HI_U32 ymotionstrlut56 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT14 VPSS_TNR_YMOTIONSTR_LUT14;

     VPSS_TNR_YMOTIONSTR_LUT14.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT14.u32));
     VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut59 = ymotionstrlut59 ;
     VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut58 = ymotionstrlut58 ;
     VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut57 = ymotionstrlut57 ;
     VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut56 = ymotionstrlut56 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT14.u32), VPSS_TNR_YMOTIONSTR_LUT14.u32);

     return;
}
//addr=0x39bc
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut63 ,HI_U32 ymotionstrlut62 ,HI_U32 ymotionstrlut61 ,HI_U32 ymotionstrlut60 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT15 VPSS_TNR_YMOTIONSTR_LUT15;

     VPSS_TNR_YMOTIONSTR_LUT15.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT15.u32));
     VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut63 = ymotionstrlut63 ;
     VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut62 = ymotionstrlut62 ;
     VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut61 = ymotionstrlut61 ;
     VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut60 = ymotionstrlut60 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT15.u32), VPSS_TNR_YMOTIONSTR_LUT15.u32);

     return;
}
//addr=0x39c0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut67 ,HI_U32 ymotionstrlut66 ,HI_U32 ymotionstrlut65 ,HI_U32 ymotionstrlut64 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT16 VPSS_TNR_YMOTIONSTR_LUT16;

     VPSS_TNR_YMOTIONSTR_LUT16.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT16.u32));
     VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut67 = ymotionstrlut67 ;
     VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut66 = ymotionstrlut66 ;
     VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut65 = ymotionstrlut65 ;
     VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut64 = ymotionstrlut64 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT16.u32), VPSS_TNR_YMOTIONSTR_LUT16.u32);

     return;
}
//addr=0x39c4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut71 ,HI_U32 ymotionstrlut70 ,HI_U32 ymotionstrlut69 ,HI_U32 ymotionstrlut68 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT17 VPSS_TNR_YMOTIONSTR_LUT17;

     VPSS_TNR_YMOTIONSTR_LUT17.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT17.u32));
     VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut71 = ymotionstrlut71 ;
     VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut70 = ymotionstrlut70 ;
     VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut69 = ymotionstrlut69 ;
     VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut68 = ymotionstrlut68 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT17.u32), VPSS_TNR_YMOTIONSTR_LUT17.u32);

     return;
}
//addr=0x39c8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut75 ,HI_U32 ymotionstrlut74 ,HI_U32 ymotionstrlut73 ,HI_U32 ymotionstrlut72 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT18 VPSS_TNR_YMOTIONSTR_LUT18;

     VPSS_TNR_YMOTIONSTR_LUT18.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT18.u32));
     VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut75 = ymotionstrlut75 ;
     VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut74 = ymotionstrlut74 ;
     VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut73 = ymotionstrlut73 ;
     VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut72 = ymotionstrlut72 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT18.u32), VPSS_TNR_YMOTIONSTR_LUT18.u32);

     return;
}
//addr=0x39cc
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut79 ,HI_U32 ymotionstrlut78 ,HI_U32 ymotionstrlut77 ,HI_U32 ymotionstrlut76 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT19 VPSS_TNR_YMOTIONSTR_LUT19;

     VPSS_TNR_YMOTIONSTR_LUT19.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT19.u32));
     VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut79 = ymotionstrlut79 ;
     VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut78 = ymotionstrlut78 ;
     VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut77 = ymotionstrlut77 ;
     VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut76 = ymotionstrlut76 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT19.u32), VPSS_TNR_YMOTIONSTR_LUT19.u32);

     return;
}
//addr=0x39d0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut83 ,HI_U32 ymotionstrlut82 ,HI_U32 ymotionstrlut81 ,HI_U32 ymotionstrlut80 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT20 VPSS_TNR_YMOTIONSTR_LUT20;

     VPSS_TNR_YMOTIONSTR_LUT20.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT20.u32));
     VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut83 = ymotionstrlut83 ;
     VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut82 = ymotionstrlut82 ;
     VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut81 = ymotionstrlut81 ;
     VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut80 = ymotionstrlut80 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT20.u32), VPSS_TNR_YMOTIONSTR_LUT20.u32);

     return;
}
//addr=0x39d4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut87 ,HI_U32 ymotionstrlut86 ,HI_U32 ymotionstrlut85 ,HI_U32 ymotionstrlut84 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT21 VPSS_TNR_YMOTIONSTR_LUT21;

     VPSS_TNR_YMOTIONSTR_LUT21.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT21.u32));
     VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut87 = ymotionstrlut87 ;
     VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut86 = ymotionstrlut86 ;
     VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut85 = ymotionstrlut85 ;
     VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut84 = ymotionstrlut84 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT21.u32), VPSS_TNR_YMOTIONSTR_LUT21.u32);

     return;
}
//addr=0x39d8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut91 ,HI_U32 ymotionstrlut90 ,HI_U32 ymotionstrlut89 ,HI_U32 ymotionstrlut88 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT22 VPSS_TNR_YMOTIONSTR_LUT22;

     VPSS_TNR_YMOTIONSTR_LUT22.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT22.u32));
     VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut91 = ymotionstrlut91 ;
     VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut90 = ymotionstrlut90 ;
     VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut89 = ymotionstrlut89 ;
     VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut88 = ymotionstrlut88 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT22.u32), VPSS_TNR_YMOTIONSTR_LUT22.u32);

     return;
}
//addr=0x39dc
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut95 ,HI_U32 ymotionstrlut94 ,HI_U32 ymotionstrlut93 ,HI_U32 ymotionstrlut92 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT23 VPSS_TNR_YMOTIONSTR_LUT23;

     VPSS_TNR_YMOTIONSTR_LUT23.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT23.u32));
     VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut95 = ymotionstrlut95 ;
     VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut94 = ymotionstrlut94 ;
     VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut93 = ymotionstrlut93 ;
     VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut92 = ymotionstrlut92 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT23.u32), VPSS_TNR_YMOTIONSTR_LUT23.u32);

     return;
}
//addr=0x39e0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut99 ,HI_U32 ymotionstrlut98 ,HI_U32 ymotionstrlut97 ,HI_U32 ymotionstrlut96 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT24 VPSS_TNR_YMOTIONSTR_LUT24;

     VPSS_TNR_YMOTIONSTR_LUT24.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT24.u32));
     VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut99 = ymotionstrlut99 ;
     VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut98 = ymotionstrlut98 ;
     VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut97 = ymotionstrlut97 ;
     VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut96 = ymotionstrlut96 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT24.u32), VPSS_TNR_YMOTIONSTR_LUT24.u32);

     return;
}
//addr=0x39e4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut103 ,HI_U32 ymotionstrlut102 ,HI_U32 ymotionstrlut101 ,HI_U32 ymotionstrlut100 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT25 VPSS_TNR_YMOTIONSTR_LUT25;

     VPSS_TNR_YMOTIONSTR_LUT25.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT25.u32));
     VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut103 = ymotionstrlut103 ;
     VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut102 = ymotionstrlut102 ;
     VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut101 = ymotionstrlut101 ;
     VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut100 = ymotionstrlut100 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT25.u32), VPSS_TNR_YMOTIONSTR_LUT25.u32);

     return;
}
//addr=0x39e8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut107 ,HI_U32 ymotionstrlut106 ,HI_U32 ymotionstrlut105 ,HI_U32 ymotionstrlut104 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT26 VPSS_TNR_YMOTIONSTR_LUT26;

     VPSS_TNR_YMOTIONSTR_LUT26.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT26.u32));
     VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut107 = ymotionstrlut107 ;
     VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut106 = ymotionstrlut106 ;
     VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut105 = ymotionstrlut105 ;
     VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut104 = ymotionstrlut104 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT26.u32), VPSS_TNR_YMOTIONSTR_LUT26.u32);

     return;
}
//addr=0x39ec
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut111 ,HI_U32 ymotionstrlut110 ,HI_U32 ymotionstrlut109 ,HI_U32 ymotionstrlut108 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT27 VPSS_TNR_YMOTIONSTR_LUT27;

     VPSS_TNR_YMOTIONSTR_LUT27.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT27.u32));
     VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut111 = ymotionstrlut111 ;
     VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut110 = ymotionstrlut110 ;
     VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut109 = ymotionstrlut109 ;
     VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut108 = ymotionstrlut108 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT27.u32), VPSS_TNR_YMOTIONSTR_LUT27.u32);

     return;
}
//addr=0x39f0
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut115 ,HI_U32 ymotionstrlut114 ,HI_U32 ymotionstrlut113 ,HI_U32 ymotionstrlut112 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT28 VPSS_TNR_YMOTIONSTR_LUT28;

     VPSS_TNR_YMOTIONSTR_LUT28.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT28.u32));
     VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut115 = ymotionstrlut115 ;
     VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut114 = ymotionstrlut114 ;
     VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut113 = ymotionstrlut113 ;
     VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut112 = ymotionstrlut112 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT28.u32), VPSS_TNR_YMOTIONSTR_LUT28.u32);

     return;
}
//addr=0x39f4
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut119 ,HI_U32 ymotionstrlut118 ,HI_U32 ymotionstrlut117 ,HI_U32 ymotionstrlut116 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT29 VPSS_TNR_YMOTIONSTR_LUT29;

     VPSS_TNR_YMOTIONSTR_LUT29.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT29.u32));
     VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut119 = ymotionstrlut119 ;
     VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut118 = ymotionstrlut118 ;
     VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut117 = ymotionstrlut117 ;
     VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut116 = ymotionstrlut116 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT29.u32), VPSS_TNR_YMOTIONSTR_LUT29.u32);

     return;
}
//addr=0x39f8
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut123 ,HI_U32 ymotionstrlut122 ,HI_U32 ymotionstrlut121 ,HI_U32 ymotionstrlut120 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT30 VPSS_TNR_YMOTIONSTR_LUT30;

     VPSS_TNR_YMOTIONSTR_LUT30.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT30.u32));
     VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut123 = ymotionstrlut123 ;
     VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut122 = ymotionstrlut122 ;
     VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut121 = ymotionstrlut121 ;
     VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut120 = ymotionstrlut120 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT30.u32), VPSS_TNR_YMOTIONSTR_LUT30.u32);

     return;
}
//addr=0x39fc
HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut127 ,HI_U32 ymotionstrlut126 ,HI_U32 ymotionstrlut125 ,HI_U32 ymotionstrlut124 )
{
     U_VPSS_TNR_YMOTIONSTR_LUT31 VPSS_TNR_YMOTIONSTR_LUT31;

     VPSS_TNR_YMOTIONSTR_LUT31.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT31.u32));
     VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut127 = ymotionstrlut127 ;
     VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut126 = ymotionstrlut126 ;
     VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut125 = ymotionstrlut125 ;
     VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut124 = ymotionstrlut124 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_YMOTIONSTR_LUT31.u32), VPSS_TNR_YMOTIONSTR_LUT31.u32);

     return;
}
//addr=0x3a00
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut3 ,HI_U32 cmotionstrlut2 ,HI_U32 cmotionstrlut1 ,HI_U32 cmotionstrlut0 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT0 VPSS_TNR_CMOTIONSTR_LUT0;

     VPSS_TNR_CMOTIONSTR_LUT0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT0.u32));
     VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut3 = cmotionstrlut3 ;
     VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut2 = cmotionstrlut2 ;
     VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut1 = cmotionstrlut1 ;
     VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut0 = cmotionstrlut0 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT0.u32), VPSS_TNR_CMOTIONSTR_LUT0.u32);

     return;
}
//addr=0x3a04
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut7 ,HI_U32 cmotionstrlut6 ,HI_U32 cmotionstrlut5 ,HI_U32 cmotionstrlut4 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT1 VPSS_TNR_CMOTIONSTR_LUT1;

     VPSS_TNR_CMOTIONSTR_LUT1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT1.u32));
     VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut7 = cmotionstrlut7 ;
     VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut6 = cmotionstrlut6 ;
     VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut5 = cmotionstrlut5 ;
     VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut4 = cmotionstrlut4 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT1.u32), VPSS_TNR_CMOTIONSTR_LUT1.u32);

     return;
}
//addr=0x3a08
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut11 ,HI_U32 cmotionstrlut10 ,HI_U32 cmotionstrlut9 ,HI_U32 cmotionstrlut8 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT2 VPSS_TNR_CMOTIONSTR_LUT2;

     VPSS_TNR_CMOTIONSTR_LUT2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT2.u32));
     VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut11 = cmotionstrlut11 ;
     VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut10 = cmotionstrlut10 ;
     VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut9  = cmotionstrlut9  ;
     VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut8  = cmotionstrlut8  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT2.u32), VPSS_TNR_CMOTIONSTR_LUT2.u32);

     return;
}
//addr=0x3a0c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut15 ,HI_U32 cmotionstrlut14 ,HI_U32 cmotionstrlut13 ,HI_U32 cmotionstrlut12 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT3 VPSS_TNR_CMOTIONSTR_LUT3;

     VPSS_TNR_CMOTIONSTR_LUT3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT3.u32));
     VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut15 = cmotionstrlut15 ;
     VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut14 = cmotionstrlut14 ;
     VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut13 = cmotionstrlut13 ;
     VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut12 = cmotionstrlut12 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT3.u32), VPSS_TNR_CMOTIONSTR_LUT3.u32);

     return;
}
//addr=0x3a10
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut19 ,HI_U32 cmotionstrlut18 ,HI_U32 cmotionstrlut17 ,HI_U32 cmotionstrlut16 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT4 VPSS_TNR_CMOTIONSTR_LUT4;

     VPSS_TNR_CMOTIONSTR_LUT4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT4.u32));
     VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut19 = cmotionstrlut19 ;
     VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut18 = cmotionstrlut18 ;
     VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut17 = cmotionstrlut17 ;
     VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut16 = cmotionstrlut16 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT4.u32), VPSS_TNR_CMOTIONSTR_LUT4.u32);

     return;
}
//addr=0x3a14
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut23 ,HI_U32 cmotionstrlut22 ,HI_U32 cmotionstrlut21 ,HI_U32 cmotionstrlut20 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT5 VPSS_TNR_CMOTIONSTR_LUT5;

     VPSS_TNR_CMOTIONSTR_LUT5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT5.u32));
     VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut23 = cmotionstrlut23 ;
     VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut22 = cmotionstrlut22 ;
     VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut21 = cmotionstrlut21 ;
     VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut20 = cmotionstrlut20 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT5.u32), VPSS_TNR_CMOTIONSTR_LUT5.u32);

     return;
}
//addr=0x3a18
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut27 ,HI_U32 cmotionstrlut26 ,HI_U32 cmotionstrlut25 ,HI_U32 cmotionstrlut24 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT6 VPSS_TNR_CMOTIONSTR_LUT6;

     VPSS_TNR_CMOTIONSTR_LUT6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT6.u32));
     VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut27 = cmotionstrlut27 ;
     VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut26 = cmotionstrlut26 ;
     VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut25 = cmotionstrlut25 ;
     VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut24 = cmotionstrlut24 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT6.u32), VPSS_TNR_CMOTIONSTR_LUT6.u32);

     return;
}
//addr=0x3a1c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut31 ,HI_U32 cmotionstrlut30 ,HI_U32 cmotionstrlut29 ,HI_U32 cmotionstrlut28 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT7 VPSS_TNR_CMOTIONSTR_LUT7;

     VPSS_TNR_CMOTIONSTR_LUT7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT7.u32));
     VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut31 = cmotionstrlut31 ;
     VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut30 = cmotionstrlut30 ;
     VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut29 = cmotionstrlut29 ;
     VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut28 = cmotionstrlut28 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT7.u32), VPSS_TNR_CMOTIONSTR_LUT7.u32);

     return;
}
//addr=0x3a20
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut35 ,HI_U32 cmotionstrlut34 ,HI_U32 cmotionstrlut33 ,HI_U32 cmotionstrlut32 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT8 VPSS_TNR_CMOTIONSTR_LUT8;

     VPSS_TNR_CMOTIONSTR_LUT8.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT8.u32));
     VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut35 = cmotionstrlut35 ;
     VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut34 = cmotionstrlut34 ;
     VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut33 = cmotionstrlut33 ;
     VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut32 = cmotionstrlut32 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT8.u32), VPSS_TNR_CMOTIONSTR_LUT8.u32);

     return;
}
//addr=0x3a24
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut39 ,HI_U32 cmotionstrlut38 ,HI_U32 cmotionstrlut37 ,HI_U32 cmotionstrlut36 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT9 VPSS_TNR_CMOTIONSTR_LUT9;

     VPSS_TNR_CMOTIONSTR_LUT9.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT9.u32));
     VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut39 = cmotionstrlut39 ;
     VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut38 = cmotionstrlut38 ;
     VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut37 = cmotionstrlut37 ;
     VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut36 = cmotionstrlut36 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT9.u32), VPSS_TNR_CMOTIONSTR_LUT9.u32);

     return;
}
//addr=0x3a28
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut43 ,HI_U32 cmotionstrlut42 ,HI_U32 cmotionstrlut41 ,HI_U32 cmotionstrlut40 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT10 VPSS_TNR_CMOTIONSTR_LUT10;

     VPSS_TNR_CMOTIONSTR_LUT10.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT10.u32));
     VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut43 = cmotionstrlut43 ;
     VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut42 = cmotionstrlut42 ;
     VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut41 = cmotionstrlut41 ;
     VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut40 = cmotionstrlut40 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT10.u32), VPSS_TNR_CMOTIONSTR_LUT10.u32);

     return;
}
//addr=0x3a2c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut47 ,HI_U32 cmotionstrlut46 ,HI_U32 cmotionstrlut45 ,HI_U32 cmotionstrlut44 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT11 VPSS_TNR_CMOTIONSTR_LUT11;

     VPSS_TNR_CMOTIONSTR_LUT11.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT11.u32));
     VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut47 = cmotionstrlut47 ;
     VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut46 = cmotionstrlut46 ;
     VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut45 = cmotionstrlut45 ;
     VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut44 = cmotionstrlut44 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT11.u32), VPSS_TNR_CMOTIONSTR_LUT11.u32);

     return;
}
//addr=0x3a30
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut51 ,HI_U32 cmotionstrlut50 ,HI_U32 cmotionstrlut49 ,HI_U32 cmotionstrlut48 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT12 VPSS_TNR_CMOTIONSTR_LUT12;

     VPSS_TNR_CMOTIONSTR_LUT12.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT12.u32));
     VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut51 = cmotionstrlut51 ;
     VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut50 = cmotionstrlut50 ;
     VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut49 = cmotionstrlut49 ;
     VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut48 = cmotionstrlut48 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT12.u32), VPSS_TNR_CMOTIONSTR_LUT12.u32);

     return;
}
//addr=0x3a34
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut55 ,HI_U32 cmotionstrlut54 ,HI_U32 cmotionstrlut53 ,HI_U32 cmotionstrlut52 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT13 VPSS_TNR_CMOTIONSTR_LUT13;

     VPSS_TNR_CMOTIONSTR_LUT13.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT13.u32));
     VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut55 = cmotionstrlut55 ;
     VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut54 = cmotionstrlut54 ;
     VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut53 = cmotionstrlut53 ;
     VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut52 = cmotionstrlut52 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT13.u32), VPSS_TNR_CMOTIONSTR_LUT13.u32);

     return;
}
//addr=0x3a38
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut59 ,HI_U32 cmotionstrlut58 ,HI_U32 cmotionstrlut57 ,HI_U32 cmotionstrlut56 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT14 VPSS_TNR_CMOTIONSTR_LUT14;

     VPSS_TNR_CMOTIONSTR_LUT14.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT14.u32));
     VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut59 = cmotionstrlut59 ;
     VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut58 = cmotionstrlut58 ;
     VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut57 = cmotionstrlut57 ;
     VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut56 = cmotionstrlut56 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT14.u32), VPSS_TNR_CMOTIONSTR_LUT14.u32);

     return;
}
//addr=0x3a3c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut63 ,HI_U32 cmotionstrlut62 ,HI_U32 cmotionstrlut61 ,HI_U32 cmotionstrlut60 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT15 VPSS_TNR_CMOTIONSTR_LUT15;

     VPSS_TNR_CMOTIONSTR_LUT15.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT15.u32));
     VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut63 = cmotionstrlut63 ;
     VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut62 = cmotionstrlut62 ;
     VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut61 = cmotionstrlut61 ;
     VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut60 = cmotionstrlut60 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT15.u32), VPSS_TNR_CMOTIONSTR_LUT15.u32);

     return;
}
//addr=0x3a40
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut67 ,HI_U32 cmotionstrlut66 ,HI_U32 cmotionstrlut65 ,HI_U32 cmotionstrlut64 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT16 VPSS_TNR_CMOTIONSTR_LUT16;

     VPSS_TNR_CMOTIONSTR_LUT16.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT16.u32));
     VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut67 = cmotionstrlut67 ;
     VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut66 = cmotionstrlut66 ;
     VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut65 = cmotionstrlut65 ;
     VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut64 = cmotionstrlut64 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT16.u32), VPSS_TNR_CMOTIONSTR_LUT16.u32);

     return;
}
//addr=0x3a44
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut71 ,HI_U32 cmotionstrlut70 ,HI_U32 cmotionstrlut69 ,HI_U32 cmotionstrlut68 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT17 VPSS_TNR_CMOTIONSTR_LUT17;

     VPSS_TNR_CMOTIONSTR_LUT17.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT17.u32));
     VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut71 = cmotionstrlut71 ;
     VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut70 = cmotionstrlut70 ;
     VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut69 = cmotionstrlut69 ;
     VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut68 = cmotionstrlut68 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT17.u32), VPSS_TNR_CMOTIONSTR_LUT17.u32);

     return;
}
//addr=0x3a48
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut75 ,HI_U32 cmotionstrlut74 ,HI_U32 cmotionstrlut73 ,HI_U32 cmotionstrlut72 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT18 VPSS_TNR_CMOTIONSTR_LUT18;

     VPSS_TNR_CMOTIONSTR_LUT18.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT18.u32));
     VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut75 = cmotionstrlut75 ;
     VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut74 = cmotionstrlut74 ;
     VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut73 = cmotionstrlut73 ;
     VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut72 = cmotionstrlut72 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT18.u32), VPSS_TNR_CMOTIONSTR_LUT18.u32);

     return;
}
//addr=0x3a4c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut79 ,HI_U32 cmotionstrlut78 ,HI_U32 cmotionstrlut77 ,HI_U32 cmotionstrlut76 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT19 VPSS_TNR_CMOTIONSTR_LUT19;

     VPSS_TNR_CMOTIONSTR_LUT19.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT19.u32));
     VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut79 = cmotionstrlut79 ;
     VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut78 = cmotionstrlut78 ;
     VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut77 = cmotionstrlut77 ;
     VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut76 = cmotionstrlut76 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT19.u32), VPSS_TNR_CMOTIONSTR_LUT19.u32);

     return;
}
//addr=0x3a50
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut83 ,HI_U32 cmotionstrlut82 ,HI_U32 cmotionstrlut81 ,HI_U32 cmotionstrlut80 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT20 VPSS_TNR_CMOTIONSTR_LUT20;

     VPSS_TNR_CMOTIONSTR_LUT20.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT20.u32));
     VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut83 = cmotionstrlut83 ;
     VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut82 = cmotionstrlut82 ;
     VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut81 = cmotionstrlut81 ;
     VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut80 = cmotionstrlut80 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT20.u32), VPSS_TNR_CMOTIONSTR_LUT20.u32);

     return;
}
//addr=0x3a54
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut87 ,HI_U32 cmotionstrlut86 ,HI_U32 cmotionstrlut85 ,HI_U32 cmotionstrlut84 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT21 VPSS_TNR_CMOTIONSTR_LUT21;

     VPSS_TNR_CMOTIONSTR_LUT21.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT21.u32));
     VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut87 = cmotionstrlut87 ;
     VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut86 = cmotionstrlut86 ;
     VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut85 = cmotionstrlut85 ;
     VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut84 = cmotionstrlut84 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT21.u32), VPSS_TNR_CMOTIONSTR_LUT21.u32);

     return;
}
//addr=0x3a58
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut91 ,HI_U32 cmotionstrlut90 ,HI_U32 cmotionstrlut89 ,HI_U32 cmotionstrlut88 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT22 VPSS_TNR_CMOTIONSTR_LUT22;

     VPSS_TNR_CMOTIONSTR_LUT22.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT22.u32));
     VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut91 = cmotionstrlut91 ;
     VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut90 = cmotionstrlut90 ;
     VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut89 = cmotionstrlut89 ;
     VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut88 = cmotionstrlut88 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT22.u32), VPSS_TNR_CMOTIONSTR_LUT22.u32);

     return;
}
//addr=0x3a5c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut95 ,HI_U32 cmotionstrlut94 ,HI_U32 cmotionstrlut93 ,HI_U32 cmotionstrlut92 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT23 VPSS_TNR_CMOTIONSTR_LUT23;

     VPSS_TNR_CMOTIONSTR_LUT23.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT23.u32));
     VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut95 = cmotionstrlut95 ;
     VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut94 = cmotionstrlut94 ;
     VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut93 = cmotionstrlut93 ;
     VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut92 = cmotionstrlut92 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT23.u32), VPSS_TNR_CMOTIONSTR_LUT23.u32);

     return;
}
//addr=0x3a60
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut99 ,HI_U32 cmotionstrlut98 ,HI_U32 cmotionstrlut97 ,HI_U32 cmotionstrlut96 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT24 VPSS_TNR_CMOTIONSTR_LUT24;

     VPSS_TNR_CMOTIONSTR_LUT24.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT24.u32));
     VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut99 = cmotionstrlut99 ;
     VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut98 = cmotionstrlut98 ;
     VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut97 = cmotionstrlut97 ;
     VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut96 = cmotionstrlut96 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT24.u32), VPSS_TNR_CMOTIONSTR_LUT24.u32);

     return;
}
//addr=0x3a64
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut103 ,HI_U32 cmotionstrlut102 ,HI_U32 cmotionstrlut101 ,HI_U32 cmotionstrlut100 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT25 VPSS_TNR_CMOTIONSTR_LUT25;

     VPSS_TNR_CMOTIONSTR_LUT25.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT25.u32));
     VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut103 = cmotionstrlut103 ;
     VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut102 = cmotionstrlut102 ;
     VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut101 = cmotionstrlut101 ;
     VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut100 = cmotionstrlut100 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT25.u32), VPSS_TNR_CMOTIONSTR_LUT25.u32);

     return;
}
//addr=0x3a68
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut107 ,HI_U32 cmotionstrlut106 ,HI_U32 cmotionstrlut105 ,HI_U32 cmotionstrlut104 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT26 VPSS_TNR_CMOTIONSTR_LUT26;

     VPSS_TNR_CMOTIONSTR_LUT26.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT26.u32));
     VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut107 = cmotionstrlut107 ;
     VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut106 = cmotionstrlut106 ;
     VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut105 = cmotionstrlut105 ;
     VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut104 = cmotionstrlut104 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT26.u32), VPSS_TNR_CMOTIONSTR_LUT26.u32);

     return;
}
//addr=0x3a6c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut111 ,HI_U32 cmotionstrlut110 ,HI_U32 cmotionstrlut109 ,HI_U32 cmotionstrlut108 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT27 VPSS_TNR_CMOTIONSTR_LUT27;

     VPSS_TNR_CMOTIONSTR_LUT27.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT27.u32));
     VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut111 = cmotionstrlut111 ;
     VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut110 = cmotionstrlut110 ;
     VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut109 = cmotionstrlut109 ;
     VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut108 = cmotionstrlut108 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT27.u32), VPSS_TNR_CMOTIONSTR_LUT27.u32);

     return;
}
//addr=0x3a70
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut115 ,HI_U32 cmotionstrlut114 ,HI_U32 cmotionstrlut113 ,HI_U32 cmotionstrlut112 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT28 VPSS_TNR_CMOTIONSTR_LUT28;

     VPSS_TNR_CMOTIONSTR_LUT28.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT28.u32));
     VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut115 = cmotionstrlut115 ;
     VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut114 = cmotionstrlut114 ;
     VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut113 = cmotionstrlut113 ;
     VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut112 = cmotionstrlut112 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT28.u32), VPSS_TNR_CMOTIONSTR_LUT28.u32);

     return;
}
//addr=0x3a74
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut119 ,HI_U32 cmotionstrlut118 ,HI_U32 cmotionstrlut117 ,HI_U32 cmotionstrlut116 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT29 VPSS_TNR_CMOTIONSTR_LUT29;

     VPSS_TNR_CMOTIONSTR_LUT29.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT29.u32));
     VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut119 = cmotionstrlut119 ;
     VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut118 = cmotionstrlut118 ;
     VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut117 = cmotionstrlut117 ;
     VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut116 = cmotionstrlut116 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT29.u32), VPSS_TNR_CMOTIONSTR_LUT29.u32);

     return;
}
//addr=0x3a78
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut123 ,HI_U32 cmotionstrlut122 ,HI_U32 cmotionstrlut121 ,HI_U32 cmotionstrlut120 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT30 VPSS_TNR_CMOTIONSTR_LUT30;

     VPSS_TNR_CMOTIONSTR_LUT30.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT30.u32));
     VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut123 = cmotionstrlut123 ;
     VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut122 = cmotionstrlut122 ;
     VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut121 = cmotionstrlut121 ;
     VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut120 = cmotionstrlut120 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT30.u32), VPSS_TNR_CMOTIONSTR_LUT30.u32);

     return;
}
//addr=0x3a7c
HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut127 ,HI_U32 cmotionstrlut126 ,HI_U32 cmotionstrlut125 ,HI_U32 cmotionstrlut124 )
{
     U_VPSS_TNR_CMOTIONSTR_LUT31 VPSS_TNR_CMOTIONSTR_LUT31;

     VPSS_TNR_CMOTIONSTR_LUT31.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT31.u32));
     VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut127 = cmotionstrlut127 ;
     VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut126 = cmotionstrlut126 ;
     VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut125 = cmotionstrlut125 ;
     VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut124 = cmotionstrlut124 ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->VPSS_TNR_CMOTIONSTR_LUT31.u32), VPSS_TNR_CMOTIONSTR_LUT31.u32);

     return;
}
#endif

#if 1
//addr=0xf000
HI_VOID VPSS_DRV_Set_SMMU_SCR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ptw_pf ,HI_U32 int_en ,HI_U32 glb_bypass )
{
	U_VPSS_SMMU_SCR SMMU_SCR;
	HI_S32 s32RetryCnt;
	HI_U32 u32BKData;

	s32RetryCnt = 0;
	SMMU_SCR.bits.ptw_pf	 = ptw_pf	  ;
	SMMU_SCR.bits.int_en	 = int_en	  ;
	SMMU_SCR.bits.glb_bypass = glb_bypass ;

	u32BKData = SMMU_SCR.u32;
	SMMU_SCR.u32 = 0;

	while ((SMMU_SCR.bits.ptw_pf != ptw_pf)
			|| (SMMU_SCR.bits.int_en != int_en)
			|| (SMMU_SCR.bits.glb_bypass != glb_bypass))
	{
		SMMU_SCR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_SCR.u32));
		SMMU_SCR.bits.ptw_pf	 = ptw_pf	  ;
		SMMU_SCR.bits.int_en	 = int_en	  ;
		SMMU_SCR.bits.glb_bypass = glb_bypass ;

		VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_SCR.u32), SMMU_SCR.u32);
		SMMU_SCR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_SCR.u32));

		s32RetryCnt++;

		if ((SMMU_SCR.u32&0xFFFFF) != u32BKData)
		{
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, u32BKData, (SMMU_SCR.u32&0xFFFFF));
		}
	}

	return;
}
//addr=0xf008
HI_VOID VPSS_DRV_Set_SMMU_LP_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 auto_clk_gt_en )
{
     U_VPSS_SMMU_LP_CTRL SMMU_LP_CTRL;

     SMMU_LP_CTRL.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_LP_CTRL.u32));
     SMMU_LP_CTRL.bits.auto_clk_gt_en = auto_clk_gt_en ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_LP_CTRL.u32), SMMU_LP_CTRL.u32);

     return;
}
//addr=0xf010
HI_VOID VPSS_DRV_Set_SMMU_INTMASK_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_msk ,HI_U32
ints_tlbinvalid_rd_msk ,HI_U32 ints_ptw_trans_msk ,HI_U32 ints_tlbmiss_msk )
{
     U_VPSS_SMMU_INTMASK_S SMMU_INTMASK_S;

     SMMU_INTMASK_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTMASK_S.u32));
     SMMU_INTMASK_S.bits.ints_tlbinvalid_wr_msk = ints_tlbinvalid_wr_msk ;
     SMMU_INTMASK_S.bits.ints_tlbinvalid_rd_msk = ints_tlbinvalid_rd_msk ;
     SMMU_INTMASK_S.bits.ints_ptw_trans_msk     = ints_ptw_trans_msk     ;
     SMMU_INTMASK_S.bits.ints_tlbmiss_msk       = ints_tlbmiss_msk       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTMASK_S.u32), SMMU_INTMASK_S.u32);

     return;
}
//addr=0xf014
HI_VOID VPSS_DRV_Set_SMMU_INTRAW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_raw ,HI_U32
ints_tlbinvalid_rd_raw ,HI_U32 ints_ptw_trans_raw ,HI_U32 ints_tlbmiss_raw )
{
     U_VPSS_SMMU_INTRAW_S SMMU_INTRAW_S;

     SMMU_INTRAW_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTRAW_S.u32));
     SMMU_INTRAW_S.bits.ints_tlbinvalid_wr_raw = ints_tlbinvalid_wr_raw ;
     SMMU_INTRAW_S.bits.ints_tlbinvalid_rd_raw = ints_tlbinvalid_rd_raw ;
     SMMU_INTRAW_S.bits.ints_ptw_trans_raw     = ints_ptw_trans_raw     ;
     SMMU_INTRAW_S.bits.ints_tlbmiss_raw       = ints_tlbmiss_raw       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTRAW_S.u32), SMMU_INTRAW_S.u32);

     return;
}
//addr=0xf018
HI_VOID VPSS_DRV_Set_SMMU_INTSTAT_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_stat ,HI_U32
ints_tlbinvalid_rd_stat ,HI_U32 ints_ptw_trans_stat ,HI_U32 ints_tlbmiss_stat )
{
     U_VPSS_SMMU_INTSTAT_S SMMU_INTSTAT_S;

     SMMU_INTSTAT_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_S.u32));
     SMMU_INTSTAT_S.bits.ints_tlbinvalid_wr_stat = ints_tlbinvalid_wr_stat ;
     SMMU_INTSTAT_S.bits.ints_tlbinvalid_rd_stat = ints_tlbinvalid_rd_stat ;
     SMMU_INTSTAT_S.bits.ints_ptw_trans_stat     = ints_ptw_trans_stat     ;
     SMMU_INTSTAT_S.bits.ints_tlbmiss_stat       = ints_tlbmiss_stat       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_S.u32), SMMU_INTSTAT_S.u32);

     return;
}

HI_S32 VPSS_DRV_Get_SMMU_INTSTAT_S(S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 *pu32SecureState,HI_U32 *pu32NoSecureState)
{
    U_SMMU_INTSTAT_S SMMU_INTSTAT_S;
    U_SMMU_INTSTAT_S SMMU_INTSTAT_NS;

    SMMU_INTSTAT_S.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_S.u32));
    *pu32SecureState = SMMU_INTSTAT_S.u32;
    SMMU_INTSTAT_NS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_NS.u32));
    *pu32NoSecureState = SMMU_INTSTAT_NS.u32;

	if (0 != SMMU_INTSTAT_NS.u32)
	{
		HI_U32 SMMU_FAULT_ADDR_PTW_NS, SMMU_FAULT_ID_PTW_NS, SMMU_FAULT_PTW_NUM;
		HI_U32 SMMU_FAULT_ADDR_RD_NS, SMMU_FAULT_TLB_RD_NS, SMMU_FAULT_ID_RD_NS;
		HI_U32 SMMU_FAULT_ADDR_WR_NS, SMMU_FAULT_TLB_WR_NS,	SMMU_FAULT_ID_WR_NS;

		SMMU_FAULT_ADDR_PTW_NS  = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_NS.u32));
		SMMU_FAULT_ID_PTW_NS	= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_NS.u32));
		SMMU_FAULT_PTW_NUM		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_PTW_NUM.u32));
		SMMU_FAULT_ADDR_RD_NS	= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_NS.u32));
		SMMU_FAULT_TLB_RD_NS	= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_NS.u32));
		SMMU_FAULT_ID_RD_NS		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_NS.u32));

		SMMU_FAULT_ADDR_WR_NS   = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_NS.u32));
		SMMU_FAULT_TLB_WR_NS    = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_NS.u32));
		SMMU_FAULT_ID_WR_NS     = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_NS.u32));

        if (0x0 != SMMU_INTSTAT_NS.bits.ints_tlbinvalid_rd_stat)
        {
            HI_PRINT("SMMU_READ_ERR MODULE : %s  NSSTATE : %#x ReadAddr:%#x\n",
                               HI_MOD_VPSS,
                                SMMU_INTSTAT_NS.u32,
                                SMMU_FAULT_ADDR_RD_NS);
        }

        if (0x0 != SMMU_INTSTAT_NS.bits.ints_tlbinvalid_wr_stat)
        {
            HI_PRINT("SMMU_WRITE_ERR MODULE : %s  NSSTATE : %#x WriteAddr:%#x\n",
                               HI_MOD_VPSS,
                                SMMU_INTSTAT_NS.u32,
                                SMMU_FAULT_ADDR_WR_NS);
        }

	    HI_PRINT("SMMU_FAULT_ADDR_PTW_NS:%x\nSMMU_FAULT_ID_PTW_NS:%u\nSMMU_FAULT_PTW_NUM:%u\nSMMU_FAULT_TLB_RD_NS:%#x\nSMMU_FAULT_ID_RD_NS:%u\nSMMU_FAULT_TLB_WR_NS:%#x\nSMMU_FAULT_ID_WR_NS:%u\n"
                        ,
        	    		SMMU_FAULT_ADDR_PTW_NS,
        	    		SMMU_FAULT_ID_PTW_NS,
        	    		SMMU_FAULT_PTW_NUM,
        	    		SMMU_FAULT_TLB_RD_NS,
        	    		SMMU_FAULT_ID_RD_NS,
        	    		SMMU_FAULT_TLB_WR_NS,
        			    SMMU_FAULT_ID_WR_NS
    			);

	}

	if (0 != SMMU_INTSTAT_S.u32)
	{
		HI_U32 SMMU_FAULT_ADDR_PTW_S, SMMU_FAULT_ID_PTW_S, SMMU_FAULT_PTW_NUM;
		HI_U32 SMMU_FAULT_ADDR_RD_S, SMMU_FAULT_TLB_RD_S, SMMU_FAULT_ID_RD_S;
		HI_U32 SMMU_FAULT_ADDR_WR_S, SMMU_FAULT_TLB_WR_S,	SMMU_FAULT_ID_WR_S;
		HI_U32 SMMU_SCB_TTBR;

		SMMU_FAULT_ADDR_PTW_S  	= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_S.u32));
		SMMU_FAULT_ID_PTW_S		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_S.u32));
		SMMU_FAULT_PTW_NUM		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_PTW_NUM.u32));
		SMMU_FAULT_ADDR_RD_S	= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_S.u32));
		SMMU_FAULT_TLB_RD_S		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_S.u32));
		SMMU_FAULT_ID_RD_S		= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_S.u32));
		SMMU_SCB_TTBR			= VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_SCB_TTBR.u32));
		SMMU_FAULT_ADDR_WR_S    = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_S.u32));
		SMMU_FAULT_TLB_WR_S     = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_S.u32));
		SMMU_FAULT_ID_WR_S      = VPSS_REG_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_S.u32));


        if (0x0 != SMMU_INTSTAT_S.bits.ints_tlbinvalid_rd_stat)
        {
            HI_PRINT("SMMU_READ_ERR MODULE : %s  SSTATE : %#x ReadAddr:%#x\n",
                               HI_MOD_VPSS,
                                SMMU_INTSTAT_S.u32,
                                SMMU_FAULT_ADDR_RD_S);
        }

        if (0x0 != SMMU_INTSTAT_S.bits.ints_tlbinvalid_wr_stat)
        {
            HI_PRINT("SMMU_WRITE_ERR MODULE : %s  SSTATE : %#x WriteAddr:%#x\n",
                               HI_MOD_VPSS,
                                SMMU_INTSTAT_S.u32,
                                SMMU_FAULT_ADDR_WR_S);
        }

	    HI_PRINT(
	    "SMMU_FAULT_ADDR_PTW_S:%x\nSMMU_FAULT_ID_PTW_S:%u\nSMMU_FAULT_PTW_NUM:%u\nSMMU_FAULT_TLB_RD_S:%#x\nSMMU_FAULT_ID_RD_S:%u\nSMMU_FAULT_TLB_WR_S:%#x\nSMMU_FAULT_ID_WR_S:%u\nSMMU_SCB_TTBR:%x\n"
                        ,
        	    		SMMU_FAULT_ADDR_PTW_S,
        	    		SMMU_FAULT_ID_PTW_S,
        	    		SMMU_FAULT_PTW_NUM,
        	    		SMMU_FAULT_TLB_RD_S,
        	    		SMMU_FAULT_ID_RD_S,
        	    		SMMU_FAULT_TLB_WR_S,
        			    SMMU_FAULT_ID_WR_S,
        			    SMMU_SCB_TTBR
    			);
	}

    return HI_SUCCESS;
}

//addr=0xf01c
HI_VOID VPSS_DRV_Set_SMMU_INTCLR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_clr ,HI_U32
ints_tlbinvalid_rd_clr ,HI_U32 ints_ptw_trans_clr ,HI_U32 ints_tlbmiss_clr )
{
     U_VPSS_SMMU_INTCLR_S SMMU_INTCLR_S;

     SMMU_INTCLR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTCLR_S.u32));
     SMMU_INTCLR_S.bits.ints_tlbinvalid_wr_clr = ints_tlbinvalid_wr_clr ;
     SMMU_INTCLR_S.bits.ints_tlbinvalid_rd_clr = ints_tlbinvalid_rd_clr ;
     SMMU_INTCLR_S.bits.ints_ptw_trans_clr     = ints_ptw_trans_clr     ;
     SMMU_INTCLR_S.bits.ints_tlbmiss_clr       = ints_tlbmiss_clr       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTCLR_S.u32), SMMU_INTCLR_S.u32);

     return;
}
//addr=0xf020
HI_VOID VPSS_DRV_Set_SMMU_INTMASK_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_msk ,HI_U32
intns_tlbinvalid_rd_msk ,HI_U32 intns_ptw_trans_msk ,HI_U32 intns_tlbmiss_msk )
{
     U_VPSS_SMMU_INTMASK_NS SMMU_INTMASK_NS;

     SMMU_INTMASK_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTMASK_NS.u32));
     SMMU_INTMASK_NS.bits.intns_tlbinvalid_wr_msk = intns_tlbinvalid_wr_msk ;
     SMMU_INTMASK_NS.bits.intns_tlbinvalid_rd_msk = intns_tlbinvalid_rd_msk ;
     SMMU_INTMASK_NS.bits.intns_ptw_trans_msk     = intns_ptw_trans_msk     ;
     SMMU_INTMASK_NS.bits.intns_tlbmiss_msk       = intns_tlbmiss_msk       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTMASK_NS.u32), SMMU_INTMASK_NS.u32);

     return;
}
//addr=0xf024
HI_VOID VPSS_DRV_Set_SMMU_INTRAW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_raw ,HI_U32
intns_tlbinvalid_rd_raw ,HI_U32 intns_ptw_trans_raw ,HI_U32 intns_tlbmiss_raw )
{
     U_VPSS_SMMU_INTRAW_NS SMMU_INTRAW_NS;

     SMMU_INTRAW_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTRAW_NS.u32));
     SMMU_INTRAW_NS.bits.intns_tlbinvalid_wr_raw = intns_tlbinvalid_wr_raw ;
     SMMU_INTRAW_NS.bits.intns_tlbinvalid_rd_raw = intns_tlbinvalid_rd_raw ;
     SMMU_INTRAW_NS.bits.intns_ptw_trans_raw     = intns_ptw_trans_raw     ;
     SMMU_INTRAW_NS.bits.intns_tlbmiss_raw       = intns_tlbmiss_raw       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTRAW_NS.u32), SMMU_INTRAW_NS.u32);

     return;
}
//addr=0xf028
HI_VOID VPSS_DRV_Set_SMMU_INTSTAT_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_stat ,HI_U32
intns_tlbinvalid_rd_stat ,HI_U32 intns_ptw_trans_stat ,HI_U32 intns_tlbmiss_stat )
{
     U_VPSS_SMMU_INTSTAT_NS SMMU_INTSTAT_NS;

     SMMU_INTSTAT_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_NS.u32));
     SMMU_INTSTAT_NS.bits.intns_tlbinvalid_wr_stat = intns_tlbinvalid_wr_stat ;
     SMMU_INTSTAT_NS.bits.intns_tlbinvalid_rd_stat = intns_tlbinvalid_rd_stat ;
     SMMU_INTSTAT_NS.bits.intns_ptw_trans_stat     = intns_ptw_trans_stat     ;
     SMMU_INTSTAT_NS.bits.intns_tlbmiss_stat       = intns_tlbmiss_stat       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTSTAT_NS.u32), SMMU_INTSTAT_NS.u32);

     return;
}
//addr=0xf02c
HI_VOID VPSS_DRV_Set_SMMU_INTCLR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_clr ,HI_U32
intns_tlbinvalid_rd_clr ,HI_U32 intns_ptw_trans_clr ,HI_U32 intns_tlbmiss_clr )
{
     U_VPSS_SMMU_INTCLR_NS SMMU_INTCLR_NS;

     SMMU_INTCLR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_INTCLR_NS.u32));
     SMMU_INTCLR_NS.bits.intns_tlbinvalid_wr_clr = intns_tlbinvalid_wr_clr ;
     SMMU_INTCLR_NS.bits.intns_tlbinvalid_rd_clr = intns_tlbinvalid_rd_clr ;
     SMMU_INTCLR_NS.bits.intns_ptw_trans_clr     = intns_ptw_trans_clr     ;
     SMMU_INTCLR_NS.bits.intns_tlbmiss_clr       = intns_tlbmiss_clr       ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_INTCLR_NS.u32), SMMU_INTCLR_NS.u32);

     return;
}
//addr=0xf208
HI_VOID VPSS_DRV_Set_SMMU_SCB_TTBR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scb_ttbr )
{
     U_VPSS_SMMU_SCB_TTBR SMMU_SCB_TTBR;

     SMMU_SCB_TTBR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_SCB_TTBR.u32));
     SMMU_SCB_TTBR.bits.scb_ttbr = scb_ttbr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_SCB_TTBR.u32), SMMU_SCB_TTBR.u32);

     return;
}
//addr=0xf20c
HI_VOID VPSS_DRV_Set_SMMU_CB_TTBR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cb_ttbr )
{
     U_VPSS_SMMU_CB_TTBR SMMU_CB_TTBR;
	 HI_S32 s32RetryCnt;
	 SMMU_CB_TTBR.u32 = 0;

	 s32RetryCnt = 0;

	 while (cb_ttbr != SMMU_CB_TTBR.bits.cb_ttbr)
	 {
		SMMU_CB_TTBR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_CB_TTBR.u32));
		SMMU_CB_TTBR.bits.cb_ttbr = cb_ttbr ;
		VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_CB_TTBR.u32), SMMU_CB_TTBR.u32);
		SMMU_CB_TTBR.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_CB_TTBR.u32));

        s32RetryCnt++;

		if (SMMU_CB_TTBR.bits.cb_ttbr != (HI_U32)cb_ttbr)
		{
		   VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, cb_ttbr, SMMU_CB_TTBR.bits.cb_ttbr);
		}
	}

     return;
}
//addr=0xf2f0
HI_VOID VPSS_DRV_Set_SMMU_ERR_RDADDR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_s_rd_addr )
{
     U_VPSS_SMMU_ERR_RDADDR_S SMMU_ERR_RDADDR_S;

     SMMU_ERR_RDADDR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_RDADDR_S.u32));
     SMMU_ERR_RDADDR_S.bits.err_s_rd_addr = err_s_rd_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_RDADDR_S.u32), SMMU_ERR_RDADDR_S.u32);

     return;
}
//addr=0xf2f4
HI_VOID VPSS_DRV_Set_SMMU_ERR_WRADDR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_s_wr_addr )
{
     U_VPSS_SMMU_ERR_WRADDR_S SMMU_ERR_WRADDR_S;

     SMMU_ERR_WRADDR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_WRADDR_S.u32));
     SMMU_ERR_WRADDR_S.bits.err_s_wr_addr = err_s_wr_addr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_WRADDR_S.u32), SMMU_ERR_WRADDR_S.u32);

     return;
}
//addr=0xf304
HI_VOID VPSS_DRV_Set_SMMU_ERR_RDADDR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_ns_rd_addr )
{
     U_VPSS_SMMU_ERR_RDADDR_NS SMMU_ERR_RDADDR_NS;
	 HI_S32 s32RetryCnt;
	 SMMU_ERR_RDADDR_NS.u32 = 0;
	  s32RetryCnt = 0;

	  while (err_ns_rd_addr != SMMU_ERR_RDADDR_NS.bits.err_ns_rd_addr)
	  {
		SMMU_ERR_RDADDR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_RDADDR_NS.u32));
		SMMU_ERR_RDADDR_NS.bits.err_ns_rd_addr = err_ns_rd_addr ;

		VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_RDADDR_NS.u32), SMMU_ERR_RDADDR_NS.u32);
		SMMU_ERR_RDADDR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_RDADDR_NS.u32));

		 s32RetryCnt++;

		 if (SMMU_ERR_RDADDR_NS.bits.err_ns_rd_addr != (HI_U32)err_ns_rd_addr)
		 {
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, err_ns_rd_addr,
				SMMU_ERR_RDADDR_NS.bits.err_ns_rd_addr);
		 }
	 }



     return;
}
//addr=0xf308
HI_VOID VPSS_DRV_Set_SMMU_ERR_WRADDR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_ns_wr_addr )
{
	U_VPSS_SMMU_ERR_WRADDR_NS SMMU_ERR_WRADDR_NS;
	HI_S32 s32RetryCnt;

	SMMU_ERR_WRADDR_NS.u32 = 0;
	s32RetryCnt = 0;

	while (err_ns_wr_addr != SMMU_ERR_WRADDR_NS.bits.err_ns_wr_addr)
	{
		SMMU_ERR_WRADDR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_WRADDR_NS.u32));
		SMMU_ERR_WRADDR_NS.bits.err_ns_wr_addr = err_ns_wr_addr ;

		VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_WRADDR_NS.u32), SMMU_ERR_WRADDR_NS.u32);
		SMMU_ERR_WRADDR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_ERR_WRADDR_NS.u32));

		s32RetryCnt++;

		if (SMMU_ERR_WRADDR_NS.bits.err_ns_wr_addr != (HI_U32)err_ns_wr_addr)
		{
			VPSS_FATAL("retry:%d (exp %x  real %x)\n", s32RetryCnt, err_ns_wr_addr,
			SMMU_ERR_WRADDR_NS.bits.err_ns_wr_addr);
		}
	}

	return;
}
//addr=0xf310
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_PTW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_ptw_s )
{
     U_VPSS_SMMU_FAULT_ADDR_PTW_S SMMU_FAULT_ADDR_PTW_S;

     SMMU_FAULT_ADDR_PTW_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_S.u32));
     SMMU_FAULT_ADDR_PTW_S.bits.fault_addr_ptw_s = fault_addr_ptw_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_S.u32), SMMU_FAULT_ADDR_PTW_S.u32);

     return;
}
//addr=0xf314
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_PTW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_strm_id_s ,HI_U32
fault_index_id_s )
{
     U_VPSS_SMMU_FAULT_ID_PTW_S SMMU_FAULT_ID_PTW_S;

     SMMU_FAULT_ID_PTW_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_S.u32));
     SMMU_FAULT_ID_PTW_S.bits.fault_strm_id_s  = fault_strm_id_s  ;
     SMMU_FAULT_ID_PTW_S.bits.fault_index_id_s = fault_index_id_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_S.u32), SMMU_FAULT_ID_PTW_S.u32);

     return;
}
//addr=0xf320
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_PTW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_ptw_ns )
{
     U_VPSS_SMMU_FAULT_ADDR_PTW_NS SMMU_FAULT_ADDR_PTW_NS;

     SMMU_FAULT_ADDR_PTW_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_NS.u32));
     SMMU_FAULT_ADDR_PTW_NS.bits.fault_addr_ptw_ns = fault_addr_ptw_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_PTW_NS.u32), SMMU_FAULT_ADDR_PTW_NS.u32);

     return;
}
//addr=0xf324
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_PTW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_strm_id_ns ,HI_U32
fault_index_id_ns )
{
     U_VPSS_SMMU_FAULT_ID_PTW_NS SMMU_FAULT_ID_PTW_NS;

     SMMU_FAULT_ID_PTW_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_NS.u32));
     SMMU_FAULT_ID_PTW_NS.bits.fault_strm_id_ns  = fault_strm_id_ns  ;
     SMMU_FAULT_ID_PTW_NS.bits.fault_index_id_ns = fault_index_id_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_PTW_NS.u32), SMMU_FAULT_ID_PTW_NS.u32);

     return;
}
//addr=0xf328
HI_VOID VPSS_DRV_Set_SMMU_FAULT_PTW_NUM(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_ns_ptw_num ,HI_U32
fault_s_ptw_num )
{
     U_VPSS_SMMU_FAULT_PTW_NUM SMMU_FAULT_PTW_NUM;

     SMMU_FAULT_PTW_NUM.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_PTW_NUM.u32));
     SMMU_FAULT_PTW_NUM.bits.fault_ns_ptw_num = fault_ns_ptw_num ;
     SMMU_FAULT_PTW_NUM.bits.fault_s_ptw_num  = fault_s_ptw_num  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_PTW_NUM.u32), SMMU_FAULT_PTW_NUM.u32);

     return;
}
//addr=0xf330
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_wr_s )
{
     U_VPSS_SMMU_FAULT_ADDR_WR_S SMMU_FAULT_ADDR_WR_S;

     SMMU_FAULT_ADDR_WR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_S.u32));
     SMMU_FAULT_ADDR_WR_S.bits.fault_addr_wr_s = fault_addr_wr_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_S.u32), SMMU_FAULT_ADDR_WR_S.u32);

     return;
}
//addr=0xf334
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_wr_s )
{
     U_VPSS_SMMU_FAULT_TLB_WR_S SMMU_FAULT_TLB_WR_S;

     SMMU_FAULT_TLB_WR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_S.u32));
     SMMU_FAULT_TLB_WR_S.bits.fault_tlb_wr_s = fault_tlb_wr_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_S.u32), SMMU_FAULT_TLB_WR_S.u32);

     return;
}
//addr=0xf338
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_wr_s ,HI_U32
fault_index_id_wr_s )
{
     U_VPSS_SMMU_FAULT_ID_WR_S SMMU_FAULT_ID_WR_S;

     SMMU_FAULT_ID_WR_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_S.u32));
     SMMU_FAULT_ID_WR_S.bits.fault_str_id_wr_s   = fault_str_id_wr_s   ;
     SMMU_FAULT_ID_WR_S.bits.fault_index_id_wr_s = fault_index_id_wr_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_S.u32), SMMU_FAULT_ID_WR_S.u32);

     return;
}
//addr=0xf340
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_wr_ns )
{
     U_VPSS_SMMU_FAULT_ADDR_WR_NS SMMU_FAULT_ADDR_WR_NS;

     SMMU_FAULT_ADDR_WR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_NS.u32));
     SMMU_FAULT_ADDR_WR_NS.bits.fault_addr_wr_ns = fault_addr_wr_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_WR_NS.u32), SMMU_FAULT_ADDR_WR_NS.u32);

     return;
}
//addr=0xf344
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_wr_ns )
{
     U_VPSS_SMMU_FAULT_TLB_WR_NS SMMU_FAULT_TLB_WR_NS;

     SMMU_FAULT_TLB_WR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_NS.u32));
     SMMU_FAULT_TLB_WR_NS.bits.fault_tlb_wr_ns = fault_tlb_wr_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_WR_NS.u32), SMMU_FAULT_TLB_WR_NS.u32);

     return;
}
//addr=0xf348
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_wr_ns ,HI_U32
fault_index_id_wr_ns )
{
     U_VPSS_SMMU_FAULT_ID_WR_NS SMMU_FAULT_ID_WR_NS;

     SMMU_FAULT_ID_WR_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_NS.u32));
     SMMU_FAULT_ID_WR_NS.bits.fault_str_id_wr_ns   = fault_str_id_wr_ns   ;
     SMMU_FAULT_ID_WR_NS.bits.fault_index_id_wr_ns = fault_index_id_wr_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_WR_NS.u32), SMMU_FAULT_ID_WR_NS.u32);

     return;
}
//addr=0xf350
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_rd_s )
{
     U_VPSS_SMMU_FAULT_ADDR_RD_S SMMU_FAULT_ADDR_RD_S;

     SMMU_FAULT_ADDR_RD_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_S.u32));
     SMMU_FAULT_ADDR_RD_S.bits.fault_addr_rd_s = fault_addr_rd_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_S.u32), SMMU_FAULT_ADDR_RD_S.u32);

     return;
}
//addr=0xf354
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_rd_s )
{
     U_VPSS_SMMU_FAULT_TLB_RD_S SMMU_FAULT_TLB_RD_S;

     SMMU_FAULT_TLB_RD_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_S.u32));
     SMMU_FAULT_TLB_RD_S.bits.fault_tlb_rd_s = fault_tlb_rd_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_S.u32), SMMU_FAULT_TLB_RD_S.u32);

     return;
}
//addr=0xf358
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_rd_s ,HI_U32
fault_index_id_rd_s )
{
     U_VPSS_SMMU_FAULT_ID_RD_S SMMU_FAULT_ID_RD_S;

     SMMU_FAULT_ID_RD_S.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_S.u32));
     SMMU_FAULT_ID_RD_S.bits.fault_str_id_rd_s   = fault_str_id_rd_s   ;
     SMMU_FAULT_ID_RD_S.bits.fault_index_id_rd_s = fault_index_id_rd_s ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_S.u32), SMMU_FAULT_ID_RD_S.u32);

     return;
}
//addr=0xf360
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_rd_ns )
{
     U_VPSS_SMMU_FAULT_ADDR_RD_NS SMMU_FAULT_ADDR_RD_NS;

     SMMU_FAULT_ADDR_RD_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_NS.u32));
     SMMU_FAULT_ADDR_RD_NS.bits.fault_addr_rd_ns = fault_addr_rd_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ADDR_RD_NS.u32), SMMU_FAULT_ADDR_RD_NS.u32);

     return;
}
//addr=0xf364
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_rd_ns )
{
     U_VPSS_SMMU_FAULT_TLB_RD_NS SMMU_FAULT_TLB_RD_NS;

     SMMU_FAULT_TLB_RD_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_NS.u32));
     SMMU_FAULT_TLB_RD_NS.bits.fault_tlb_rd_ns = fault_tlb_rd_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TLB_RD_NS.u32), SMMU_FAULT_TLB_RD_NS.u32);

     return;
}
//addr=0xf368
HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_rd_ns ,HI_U32
fault_index_id_rd_ns )
{
     U_VPSS_SMMU_FAULT_ID_RD_NS SMMU_FAULT_ID_RD_NS;

     SMMU_FAULT_ID_RD_NS.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_NS.u32));
     SMMU_FAULT_ID_RD_NS.bits.fault_str_id_rd_ns   = fault_str_id_rd_ns   ;
     SMMU_FAULT_ID_RD_NS.bits.fault_index_id_rd_ns = fault_index_id_rd_ns ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_ID_RD_NS.u32), SMMU_FAULT_ID_RD_NS.u32);

     return;
}
//addr=0xf36c
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TBU_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tbu_num ,HI_U32
fault_tlbinvalid_err_ns ,HI_U32 fault_tlbmiss_err_ns ,HI_U32 fault_tlbinvalid_err_s ,HI_U32 fault_tlbmiss_err_s )
{
     U_VPSS_SMMU_FAULT_TBU_INFO SMMU_FAULT_TBU_INFO;

     SMMU_FAULT_TBU_INFO.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TBU_INFO.u32));
     SMMU_FAULT_TBU_INFO.bits.fault_tbu_num           = fault_tbu_num           ;
     SMMU_FAULT_TBU_INFO.bits.fault_tlbinvalid_err_ns = fault_tlbinvalid_err_ns ;
     SMMU_FAULT_TBU_INFO.bits.fault_tlbmiss_err_ns    = fault_tlbmiss_err_ns    ;
     SMMU_FAULT_TBU_INFO.bits.fault_tlbinvalid_err_s  = fault_tlbinvalid_err_s  ;
     SMMU_FAULT_TBU_INFO.bits.fault_tlbmiss_err_s     = fault_tlbmiss_err_s     ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TBU_INFO.u32), SMMU_FAULT_TBU_INFO.u32);

     return;
}
//addr=0xf370
HI_VOID VPSS_DRV_Set_SMMU_FAULT_TBU_DBG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 read_command_counter ,HI_U32
arch_stall_n ,HI_U32 tbu_arreadym ,HI_U32 ar_readys ,HI_U32 ar_valids ,HI_U32 write_command_counter ,HI_U32
awch_stall_n ,HI_U32 tbu_awreadym ,HI_U32 aw_readys ,HI_U32 aw_valids )
{
     U_VPSS_SMMU_FAULT_TBU_DBG SMMU_FAULT_TBU_DBG;

     SMMU_FAULT_TBU_DBG.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TBU_DBG.u32));
     SMMU_FAULT_TBU_DBG.bits.read_command_counter  = read_command_counter  ;
     SMMU_FAULT_TBU_DBG.bits.arch_stall_n          = arch_stall_n          ;
     SMMU_FAULT_TBU_DBG.bits.tbu_arreadym          = tbu_arreadym          ;
     SMMU_FAULT_TBU_DBG.bits.ar_readys             = ar_readys             ;
     SMMU_FAULT_TBU_DBG.bits.ar_valids             = ar_valids             ;
     SMMU_FAULT_TBU_DBG.bits.write_command_counter = write_command_counter ;
     SMMU_FAULT_TBU_DBG.bits.awch_stall_n          = awch_stall_n          ;
     SMMU_FAULT_TBU_DBG.bits.tbu_awreadym          = tbu_awreadym          ;
     SMMU_FAULT_TBU_DBG.bits.aw_readys             = aw_readys             ;
     SMMU_FAULT_TBU_DBG.bits.aw_valids             = aw_valids             ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_FAULT_TBU_DBG.u32), SMMU_FAULT_TBU_DBG.u32);

     return;
}
//addr=0xf374
HI_VOID VPSS_DRV_Set_SMMU_PREF_BUFFER_EMPTY(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pref_buffer_empty )
{
     U_VPSS_SMMU_PREF_BUFFER_EMPTY SMMU_PREF_BUFFER_EMPTY;

     SMMU_PREF_BUFFER_EMPTY.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_PREF_BUFFER_EMPTY.u32));
     SMMU_PREF_BUFFER_EMPTY.bits.pref_buffer_empty = pref_buffer_empty ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_PREF_BUFFER_EMPTY.u32), SMMU_PREF_BUFFER_EMPTY.u32);

     return;
}
//addr=0xf378
HI_VOID VPSS_DRV_Set_SMMU_PTWQ_IDLE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ptwq15_idle_state ,HI_U32 ptwq14_idle_state
,HI_U32 ptwq13_idle_state ,HI_U32 ptwq12_idle_state ,HI_U32 ptwq11_idle_state ,HI_U32 ptwq10_idle_state ,HI_U32
ptwq9_idle_state ,HI_U32 ptwq8_idle_state ,HI_U32 ptwq7_idle_state ,HI_U32 ptwq6_idle_state ,HI_U32 ptwq5_idle_state ,
HI_U32 ptwq4_idle_state ,HI_U32 ptwq3_idle_state ,HI_U32 ptwq2_idle_state ,HI_U32 ptwq1_idle_state ,HI_U32
ptwq0_idle_state )
{
     U_VPSS_SMMU_PTWQ_IDLE SMMU_PTWQ_IDLE;

     SMMU_PTWQ_IDLE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_PTWQ_IDLE.u32));
     SMMU_PTWQ_IDLE.bits.ptwq15_idle_state = ptwq15_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq14_idle_state = ptwq14_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq13_idle_state = ptwq13_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq12_idle_state = ptwq12_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq11_idle_state = ptwq11_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq10_idle_state = ptwq10_idle_state ;
     SMMU_PTWQ_IDLE.bits.ptwq9_idle_state  = ptwq9_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq8_idle_state  = ptwq8_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq7_idle_state  = ptwq7_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq6_idle_state  = ptwq6_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq5_idle_state  = ptwq5_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq4_idle_state  = ptwq4_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq3_idle_state  = ptwq3_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq2_idle_state  = ptwq2_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq1_idle_state  = ptwq1_idle_state  ;
     SMMU_PTWQ_IDLE.bits.ptwq0_idle_state  = ptwq0_idle_state  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_PTWQ_IDLE.u32), SMMU_PTWQ_IDLE.u32);

     return;
}
//addr=0xf37c
HI_VOID VPSS_DRV_Set_SMMU_RESET_STATE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 smmu_rst_state )
{
     U_VPSS_SMMU_RESET_STATE SMMU_RESET_STATE;

     SMMU_RESET_STATE.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_RESET_STATE.u32));
     SMMU_RESET_STATE.bits.smmu_rst_state = smmu_rst_state ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_RESET_STATE.u32), SMMU_RESET_STATE.u32);

     return;
}
//addr=0xf380
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 in_out_cmd_cnt_rd ,HI_U32 rdy_debug_rd ,
HI_U32 vld_debug_rd ,HI_U32 cur_miss_cnt_rd ,HI_U32 last_miss_cnt_rd )
{
     U_VPSS_SMMU_MASTER_DBG0 SMMU_MASTER_DBG0;

     SMMU_MASTER_DBG0.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG0.u32));
     SMMU_MASTER_DBG0.bits.in_out_cmd_cnt_rd = in_out_cmd_cnt_rd ;
     SMMU_MASTER_DBG0.bits.rdy_debug_rd      = rdy_debug_rd      ;
     SMMU_MASTER_DBG0.bits.vld_debug_rd      = vld_debug_rd      ;
     SMMU_MASTER_DBG0.bits.cur_miss_cnt_rd   = cur_miss_cnt_rd   ;
     SMMU_MASTER_DBG0.bits.last_miss_cnt_rd  = last_miss_cnt_rd  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG0.u32), SMMU_MASTER_DBG0.u32);

     return;
}
//addr=0xf384
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 in_out_cmd_cnt_wr ,HI_U32 rdy_debug_wr ,
HI_U32 vld_debug_wr ,HI_U32 cur_miss_cnt_wr ,HI_U32 last_miss_cnt_wr )
{
     U_VPSS_SMMU_MASTER_DBG1 SMMU_MASTER_DBG1;

     SMMU_MASTER_DBG1.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG1.u32));
     SMMU_MASTER_DBG1.bits.in_out_cmd_cnt_wr = in_out_cmd_cnt_wr ;
     SMMU_MASTER_DBG1.bits.rdy_debug_wr      = rdy_debug_wr      ;
     SMMU_MASTER_DBG1.bits.vld_debug_wr      = vld_debug_wr      ;
     SMMU_MASTER_DBG1.bits.cur_miss_cnt_wr   = cur_miss_cnt_wr   ;
     SMMU_MASTER_DBG1.bits.last_miss_cnt_wr  = last_miss_cnt_wr  ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG1.u32), SMMU_MASTER_DBG1.u32);

     return;
}
//addr=0xf388
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_double_upd_cnt_rd ,HI_U32
last_double_upd_cnt_rd ,HI_U32 cur_double_miss_cnt_rd ,HI_U32 last_double_miss_cnt_rd )
{
     U_VPSS_SMMU_MASTER_DBG2 SMMU_MASTER_DBG2;

     SMMU_MASTER_DBG2.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG2.u32));
     SMMU_MASTER_DBG2.bits.cur_double_upd_cnt_rd   = cur_double_upd_cnt_rd   ;
     SMMU_MASTER_DBG2.bits.last_double_upd_cnt_rd  = last_double_upd_cnt_rd  ;
     SMMU_MASTER_DBG2.bits.cur_double_miss_cnt_rd  = cur_double_miss_cnt_rd  ;
     SMMU_MASTER_DBG2.bits.last_double_miss_cnt_rd = last_double_miss_cnt_rd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG2.u32), SMMU_MASTER_DBG2.u32);

     return;
}
//addr=0xf38c
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mst_fsm_cur ,HI_U32 cur_double_upd_cnt_wr
,HI_U32 last_double_upd_cnt_wr ,HI_U32 cur_double_miss_cnt_wr ,HI_U32 last_double_miss_cnt_wr )
{
     U_VPSS_SMMU_MASTER_DBG3 SMMU_MASTER_DBG3;

     SMMU_MASTER_DBG3.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG3.u32));
     SMMU_MASTER_DBG3.bits.mst_fsm_cur             = mst_fsm_cur             ;
     SMMU_MASTER_DBG3.bits.cur_double_upd_cnt_wr   = cur_double_upd_cnt_wr   ;
     SMMU_MASTER_DBG3.bits.last_double_upd_cnt_wr  = last_double_upd_cnt_wr  ;
     SMMU_MASTER_DBG3.bits.cur_double_miss_cnt_wr  = cur_double_miss_cnt_wr  ;
     SMMU_MASTER_DBG3.bits.last_double_miss_cnt_wr = last_double_miss_cnt_wr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG3.u32), SMMU_MASTER_DBG3.u32);

     return;
}
//addr=0xf520
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel1_chn_miss_cnt_rd ,HI_U32
last_sel1_chn_miss_cnt_rd )
{
     U_VPSS_SMMU_MASTER_DBG4 SMMU_MASTER_DBG4;

     SMMU_MASTER_DBG4.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG4.u32));
     SMMU_MASTER_DBG4.bits.cur_sel1_chn_miss_cnt_rd  = cur_sel1_chn_miss_cnt_rd  ;
     SMMU_MASTER_DBG4.bits.last_sel1_chn_miss_cnt_rd = last_sel1_chn_miss_cnt_rd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG4.u32), SMMU_MASTER_DBG4.u32);

     return;
}
//addr=0xf524
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel2_chn_miss_cnt_rd ,HI_U32
last_sel2_chn_miss_cnt_rd )
{
     U_VPSS_SMMU_MASTER_DBG5 SMMU_MASTER_DBG5;

     SMMU_MASTER_DBG5.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG5.u32));
     SMMU_MASTER_DBG5.bits.cur_sel2_chn_miss_cnt_rd  = cur_sel2_chn_miss_cnt_rd  ;
     SMMU_MASTER_DBG5.bits.last_sel2_chn_miss_cnt_rd = last_sel2_chn_miss_cnt_rd ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG5.u32), SMMU_MASTER_DBG5.u32);

     return;
}
//addr=0xf528
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel1_chn_miss_cnt_wr ,HI_U32
last_sel1_chn_miss_cnt_wr )
{
     U_VPSS_SMMU_MASTER_DBG6 SMMU_MASTER_DBG6;

     SMMU_MASTER_DBG6.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG6.u32));
     SMMU_MASTER_DBG6.bits.cur_sel1_chn_miss_cnt_wr  = cur_sel1_chn_miss_cnt_wr  ;
     SMMU_MASTER_DBG6.bits.last_sel1_chn_miss_cnt_wr = last_sel1_chn_miss_cnt_wr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG6.u32), SMMU_MASTER_DBG6.u32);

     return;
}
//addr=0xf52c
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel2_chn_miss_cnt_wr ,HI_U32
last_sel2_chn_miss_cnt_wr )
{
     U_VPSS_SMMU_MASTER_DBG7 SMMU_MASTER_DBG7;

     SMMU_MASTER_DBG7.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG7.u32));
     SMMU_MASTER_DBG7.bits.cur_sel2_chn_miss_cnt_wr  = cur_sel2_chn_miss_cnt_wr  ;
     SMMU_MASTER_DBG7.bits.last_sel2_chn_miss_cnt_wr = last_sel2_chn_miss_cnt_wr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG7.u32), SMMU_MASTER_DBG7.u32);

     return;
}
//addr=0xf530
HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 sel1_chn_rd ,HI_U32 sel2_chn_rd ,HI_U32
sel1_chn_wr ,HI_U32 sel2_chn_wr )
{
     U_VPSS_SMMU_MASTER_DBG8 SMMU_MASTER_DBG8;

     SMMU_MASTER_DBG8.u32 = VPSS_RegRead((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG8.u32));
     SMMU_MASTER_DBG8.bits.sel1_chn_rd = sel1_chn_rd ;
     SMMU_MASTER_DBG8.bits.sel2_chn_rd = sel2_chn_rd ;
     SMMU_MASTER_DBG8.bits.sel1_chn_wr = sel1_chn_wr ;
     SMMU_MASTER_DBG8.bits.sel2_chn_wr = sel2_chn_wr ;

     VPSS_RegWrite((volatile HI_U32*)&(pstVpssRegs->SMMU_MASTER_DBG8.u32), SMMU_MASTER_DBG8.u32);

     return;
}
#endif

