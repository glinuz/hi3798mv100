#include "vpss_reg_3798cv200.h"
#include "vpss_hal_3798cv200.h"
#include "vpss_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


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
    *a = b;      
    //printk("[%08#x] = %08#x.\n",a,b);
    return HI_SUCCESS;
}

HI_U32 VPSS_REG_RegRead(volatile HI_U32* a)
{
   return (*(a));
}


HI_S32 VPSS_REG_ReSetCRG(HI_U32 u32Type) 
{   
#if 0
    U_PERI_CRG60 PERI_CRG60;
    
    PERI_CRG60.u32 = g_pstRegCrg->PERI_CRG60.u32;
    PERI_CRG60.bits.vpss_cken = 1;
    PERI_CRG60.bits.vpss_srst_req = 1;
    g_pstRegCrg->PERI_CRG60.u32 = PERI_CRG60.u32;
    /*wait for reg ready*/
    udelay(1);
    PERI_CRG60.u32 = g_pstRegCrg->PERI_CRG60.u32;
    PERI_CRG60.bits.vpss_cken = 1;
    PERI_CRG60.bits.vpss_srst_req = 0;
    g_pstRegCrg->PERI_CRG60.u32 = PERI_CRG60.u32;
    udelay(1);
#else
    

    
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetClockEn(HI_BOOL bClkEn)
{
	if (bClkEn)
	{
		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_srst_req, 1, "VPSS reset timeout!\n");
		udelay(1);

		//0:333Mhz, 1:400Mhz, 2:300Mhz
		VPSS_WRITE_REG_SAFE(g_pstRegCrg->PERI_CRG60.bits.vpss_clk_sel, 1, "VPSS clock select timeout!\n");
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

HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg)
{
    //*ppstPhyReg = (VPSS_REG_S * )IO_ADDRESS(VPSS_BASE_ADDR);

	*ppstPhyReg = (VPSS_REG_S *)ioremap_nocache(VPSS_BASE_ADDR, VPSS_REG_SIZE);

	if (NULL == *ppstPhyReg)
	{
		VPSS_FATAL("ioremap_nocache VPSS_REG(%#x) Failed\n", VPSS_BASE_ADDR);
		
		return HI_FAILURE;
	}

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U8 * pu8AppAddr)
{
    *ppstAppReg = (VPSS_REG_S * )pu8AppAddr;

    return HI_SUCCESS;
}

#if 0
HI_S32 VPSS_REG_SmmuInit(HI_U32 u32AppAddr)
{
    VPSS_REG_S *pstReg = (VPSS_REG_S * )u32AppAddr;

    pstReg->VPSS_SMMU_SCR.bits.glb_bypass = HI_FALSE;
    pstReg->VPSS_SMMU_SCR.bits.int_en = HI_TRUE;

    pstReg->VPSS_SMMU_LP_CTRL.bits.auto_clk_gt_en = HI_TRUE;

    return HI_SUCCESS;
}
#endif


/*配置结点重置*/
HI_S32 VPSS_REG_ResetAppReg(HI_U32 *pu32AppAddr,VPSS_REG_S *pstPqReg)
{
    VPSS_REG_S *pstReg;
    HI_U8 *pu8FlushAddr;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    pu8FlushAddr = (HI_U8 *)pstReg;

    memset(pu8FlushAddr, 0, sizeof(VPSS_REG_S)); 

    if (pstPqReg)
    {
        memcpy((HI_U8 *)pstReg, (HI_U8 *)pstPqReg, sizeof(VPSS_REG_S));
    }
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32), 0x3006466);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PNEXT), 0x0);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INTMASK.u32), 0xff);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetIntMask(HI_U32 *pu32AppAddr,HI_U32 u32Mask)
{
    U_VPSS_INTMASK VPSS_INTMASK;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_INTMASK.u32 = u32Mask;    
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INTMASK.u32), VPSS_INTMASK.u32); 

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetIntMask(HI_U32 *pu32AppAddr,HI_U32 *pu32Mask)
{
    VPSS_REG_S *pstReg;
    
    U_VPSS_INTMASK VPSS_INTMASK;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_INTMASK.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_INTMASK.u32));

    *pu32Mask = VPSS_INTMASK.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetIntState(HI_U32 *pu32AppAddr,HI_U32 *pu32Int)
{
    U_VPSS_INTSTATE VPSS_INTSTATE;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_INTSTATE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_INTSTATE.u32));

    *pu32Int = VPSS_INTSTATE.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetRawIntState(HI_U32 *pu32AppAddr,HI_U32 *pu32RawInt)
{
    U_VPSS_RAWINT VPSS_RAWINT;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_RAWINT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RAWINT.u32));

    *pu32RawInt = VPSS_RAWINT.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_ClearIntState(HI_U32 *pu32AppAddr,HI_U32 u32Data)
{
    U_VPSS_INTCLR VPSS_INTCLR;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_INTCLR.u32 = u32Data;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INTCLR.u32), VPSS_INTCLR.u32);

    return HI_SUCCESS;
}
/********************************/


HI_S32 VPSS_REG_SetBurstLen(HI_U32 *pu32AppAddr,REG_BURST_LEN_E eLen)
{ 
    U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MISCELLANEOUS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32));
    VPSS_MISCELLANEOUS.bits.burst_len_cfg = eLen;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32); 


    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetCkGtEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable, HI_BOOL bCalcEnable)
{ 
    U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MISCELLANEOUS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32));
    VPSS_MISCELLANEOUS.bits.ck_gt_en = bEnable;
    VPSS_MISCELLANEOUS.bits.ck_gt_en_calc = bCalcEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetTimeOut(HI_U32 *pu32AppAddr,HI_U32 u32Data)
{ 
    HI_U32 VPSS_TIMEOUT;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_TIMEOUT = u32Data;
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TIMEOUT), VPSS_TIMEOUT);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetSmmuIntEn(HI_U8 *pu32AppAddr,HI_BOOL bEnable)
{ 
    U_SMMU_SCR VPSS_SMMU_SCR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_SMMU_SCR.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32));
    VPSS_SMMU_SCR.bits.int_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32), VPSS_SMMU_SCR.u32); 

    
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_ClearSmmuInt(HI_U8 *pu32AppAddr)
{ 
    U_SMMU_INTCLR_S SMMU_INTCLR_S;
    U_SMMU_INTCLR_NS  SMMU_INTCLR_NS;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    SMMU_INTCLR_S.u32 = 0xf;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_INTCLR_S.u32), SMMU_INTCLR_S.u32); 

    SMMU_INTCLR_NS.u32 = 0xf;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_INTCLR_NS.u32), SMMU_INTCLR_NS.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetSmmuIntState(HI_U8 *pu32AppAddr,HI_U32 *pu32SecureState,HI_U32 *pu32NoSecureState)
{ 
    U_SMMU_INTSTAT_S VPSS_SMMU_INTSTAT_S;
    U_SMMU_INTSTAT_S VPSS_SMMU_INTSTAT_NS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_SMMU_INTSTAT_S.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_INTSTAT_S.u32));

    *pu32SecureState = VPSS_SMMU_INTSTAT_S.u32;
    
    VPSS_SMMU_INTSTAT_NS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_INTSTAT_NS.u32));

    *pu32NoSecureState = VPSS_SMMU_INTSTAT_NS.u32;
    
    return HI_SUCCESS;
}

#if 0
HI_S32 VPSS_REG_SetSmmuGlbBypass(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{ 
    U_VPSS_SMMU_SCR VPSS_SMMU_SCR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_SMMU_SCR.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32));
    VPSS_SMMU_SCR.bits.glb_bypass = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32), VPSS_SMMU_SCR.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetSmmuAutoClkGtEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{ 
    U_VPSS_SMMU_LP_CTRL VPSS_SMMU_LP_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_SMMU_LP_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_LP_CTRL.u32));
    VPSS_SMMU_LP_CTRL.bits.auto_clk_gt_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_LP_CTRL.u32), VPSS_SMMU_LP_CTRL.u32); 

    return HI_SUCCESS;
}
#endif


HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 *pu32PhyAddr)
{
    U_VPSS_START VPSS_START;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)pu32PhyAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PNEXT), u32AppAddr);

	/* Add mb() to make sure all data have been written to DDR before logic start. */
	mb();

    VPSS_START.u32 = 0x1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_START.u32), VPSS_START.u32);
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_CloseClock(HI_VOID)
{  
#if 0
	U_PERI_CRG60 PERI_CRG60;
    
    PERI_CRG60.u32 = g_pstRegCrg->PERI_CRG60.u32;
    PERI_CRG60.bits.vpss_cken = 0;
    g_pstRegCrg->PERI_CRG60.u32 = PERI_CRG60.u32;
    /*wait for reg ready*/
    udelay(1);
#endif
    return HI_SUCCESS;
    
}
HI_S32 VPSS_REG_OpenClock(HI_VOID)
{  
#if 0
	U_PERI_CRG60 PERI_CRG60;
    
    PERI_CRG60.u32 = g_pstRegCrg->PERI_CRG60.u32;
    PERI_CRG60.bits.vpss_cken = 1;
    g_pstRegCrg->PERI_CRG60.u32 = PERI_CRG60.u32;
    /*wait for reg ready*/
    udelay(1);
#endif
	return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetClockState(HI_BOOL *pbOpened)
{  
//    U_PERI_CRG60 PERI_CRG60;
    
//    PERI_CRG60.u32 = g_pstRegCrg->PERI_CRG60.u32;
//    if (PERI_CRG60.bits.vpss_cken == 1)
    {
        *pbOpened = HI_TRUE;
    }

#if 0

    else
    {
        *pbOpened = HI_FALSE;
    }
#endif
	return HI_SUCCESS;
    
}

HI_S32 VPSS_REG_SetTNrEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.tnr_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetTNrMadMode(HI_U32 *pu32AppAddr,HI_U32 u32Mode)
{
    U_VPSS_CTRL3 VPSS_CTRL3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.tnr_mad_mode = u32Mode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetSNrEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.snr_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetIfmdEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.ifmd_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetIglbEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.igbm_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_Set4PixEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.four_pix_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
} 



HI_S32 VPSS_REG_SetProtEn(HI_U32 *pu32AppAddr,HI_U32 u32Prot)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    if (u32Prot == HI_TRUE)
    {
        VPSS_CTRL.bits.prot = 0x0;
    }
    else
    {
        VPSS_CTRL.bits.prot = 0x2;
    }
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetRfrEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.rfr_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
}

HI_VOID VPSS_REG_SetDetEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.str_det_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_VOID VPSS_REG_SetBlkDetEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.blk_det_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_VOID VPSS_REG_SetPgbmEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.pgbm_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_VOID VPSS_REG_SetVC1En(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.vc1_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_VOID VPSS_REG_SetMedsEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.meds_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_VOID VPSS_REG_SetMcdiEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.mcdi_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
} 

HI_S32 VPSS_REG_EnDei(HI_U32 *pu32AppAddr,HI_BOOL bEnDei)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.dei_en = bEnDei;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetChkSumEn(HI_U32 *pu32AppAddr,HI_U32 u32Chksum)
{
    U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.chk_sum_en = u32Chksum;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDcmpEn(HI_U32 *pu32AppAddr,HI_U32 u32En)
{
    U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_NXT2_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL.u32));
    VPSS_NXT2_CTRL.bits.nxt2_dcmp_en = u32En;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL.u32), VPSS_NXT2_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDcmpHeadAddr(HI_U32 *pu32AppAddr,HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr,HI_U32 u32Headsize)
{
    HI_U32 VPSS_NEXT2YT_HEADADDR;
    HI_U32 VPSS_NEXT2CT_HEADADDR;
	HI_U32 VPSS_NEXT2YB_HEADADDR;
    HI_U32 VPSS_NEXT2CB_HEADADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    {   

        VPSS_NEXT2YT_HEADADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2Y_TOP_HEAD_ADDR));
        VPSS_NEXT2CT_HEADADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2C_TOP_HEAD_ADDR));
		VPSS_NEXT2YB_HEADADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2Y_BOT_HEAD_ADDR));
        VPSS_NEXT2CB_HEADADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2C_BOT_HEAD_ADDR));

		VPSS_NEXT2YT_HEADADDR = u32YHeadaddr;
		VPSS_NEXT2CT_HEADADDR = u32CHeadaddr;
		VPSS_NEXT2YB_HEADADDR = u32YHeadaddr+u32Headsize/2;
		VPSS_NEXT2CB_HEADADDR = u32CHeadaddr+u32Headsize/4;
        //printk("VPSS_NEXT2YT_HEADADDR=0x%08x,VPSS_NEXT2CT_HEADADDR=0x%08x,VPSS_NEXT2YB_HEADADDR=0x%08x,VPSS_NEXT2CB_HEADADDR=0x%08x,u32Headsize=0x%08x \n",VPSS_NEXT2YT_HEADADDR,VPSS_NEXT2CT_HEADADDR,VPSS_NEXT2YB_HEADADDR,VPSS_NEXT2CB_HEADADDR,u32Headsize);
        VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2Y_TOP_HEAD_ADDR),VPSS_NEXT2YT_HEADADDR);
        VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2C_TOP_HEAD_ADDR),VPSS_NEXT2CT_HEADADDR);
		VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2Y_BOT_HEAD_ADDR),VPSS_NEXT2YB_HEADADDR);
        VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2C_BOT_HEAD_ADDR),VPSS_NEXT2CB_HEADADDR);
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDcmpHeadStride(HI_U32 *pu32AppAddr,HI_U32  u32HeadStride)
{
    U_VPSS_NX2_HEAD_STRIDE VPSS_NX2_HEAD_STRIDE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_NX2_HEAD_STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2_HEAD_STRIDE.u32));
    VPSS_NX2_HEAD_STRIDE.bits.nx2_head_stride = u32HeadStride;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2_HEAD_STRIDE.u32), VPSS_NX2_HEAD_STRIDE.u32); 
    
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetRfrPixBitw(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw)
{
    U_VPSS_CTRL2 VPSS_CTRL2;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));
    VPSS_CTRL2.bits.rfr_pix_bitw = ePixw;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetRefNxtPixBitw(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw)
{
    U_VPSS_CTRL2 VPSS_CTRL2;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));
    VPSS_CTRL2.bits.ref_nxt_pix_bitw = ePixw;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetInPixBitw(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw)
{
    U_VPSS_CTRL2 VPSS_CTRL2;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));
    VPSS_CTRL2.bits.in_pix_bitw = ePixw;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32); 

    return HI_SUCCESS;
}

















/*********************************VC1****************************************************/
HI_S32 VPSS_REG_SetVc1RangeEn(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_BOOL bEnable)
{
    U_VPSS_VC1_CTRL0 VPSS_VC1_CTRL0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePos)
    {
        case NEXT2_FIELD:
            VPSS_VC1_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32));
            VPSS_VC1_CTRL0.bits.fr0_vc1_en = bEnable;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32), VPSS_VC1_CTRL0.u32); 
            break;

        default:
            VPSS_FATAL("REG ERR!\n");
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetVc1Profile(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Profile)
{
    U_VPSS_VC1_CTRL0 VPSS_VC1_CTRL0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePos)
    {
        case NEXT2_FIELD:
            VPSS_VC1_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32));
            VPSS_VC1_CTRL0.bits.fr0_vc1_profile = u32Profile;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32), VPSS_VC1_CTRL0.u32); 
            break;

        default:
            VPSS_FATAL("REG ERR!\n");
    }
    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetVc1Map(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32MapY,HI_U32 u32MapC)
{
    U_VPSS_VC1_CTRL0 VPSS_VC1_CTRL0;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;


    switch(ePos)
    {
        case NEXT2_FIELD:
            VPSS_VC1_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32));
            VPSS_VC1_CTRL0.bits.fr0_vc1_mapc = u32MapC;
            VPSS_VC1_CTRL0.bits.fr0_vc1_mapy = u32MapY;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32), VPSS_VC1_CTRL0.u32); 
            break;
        default:
            VPSS_FATAL("REG ERR!\n");
    }
    return HI_SUCCESS;
}           

HI_S32 VPSS_REG_SetVc1MapFlag(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YFlag,HI_U32 u32CFlag)
{
    U_VPSS_VC1_CTRL0 VPSS_VC1_CTRL0;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePos)
    {
        case NEXT2_FIELD:
            VPSS_VC1_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32));
            VPSS_VC1_CTRL0.bits.fr0_vc1_mapcflg = u32CFlag;
            VPSS_VC1_CTRL0.bits.fr0_vc1_mapyflg = u32YFlag;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32), VPSS_VC1_CTRL0.u32); 
            break;
        default:
            VPSS_FATAL("REG ERR!\n");
    }
    return HI_SUCCESS;
}  

HI_S32 VPSS_REG_SetVc1RangeDfrmEn(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32EnVc1)
{
    U_VPSS_VC1_CTRL0 VPSS_VC1_CTRL0;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;


    switch(ePos)
    {
        case NEXT2_FIELD:
            VPSS_VC1_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32));
            VPSS_VC1_CTRL0.bits.fr0_vc1_rangedfrm = u32EnVc1;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VC1_CTRL0.u32), VPSS_VC1_CTRL0.u32); 
            break;
        default:
            VPSS_FATAL("REG ERR!\n");
    }
    return HI_SUCCESS;
}
/*********************************VC1****************************************************/




/*********************************check sum****************************************************/
HI_S32 VPSS_REG_SetRch0SumSel(HI_U32 *pu32AppAddr,HI_U32 u32rch0sumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.rch0_sum_sel = u32rch0sumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetRch1SumSel(HI_U32 *pu32AppAddr,HI_U32 u32rch1sumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.rch1_sum_sel = u32rch1sumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetRch2SumSel(HI_U32 *pu32AppAddr,HI_U32 u32rch2sumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.rch2_sum_sel = u32rch2sumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetRch3SumSel(HI_U32 *pu32AppAddr,HI_U32 u32rch3sumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.rch3_sum_sel = u32rch3sumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetRinfoSumSel(HI_U32 *pu32AppAddr,HI_U32 u32rinfosumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.rinfo_sum_sel =  u32rinfosumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetWinfoSumSel(HI_U32 *pu32AppAddr,HI_U32 u32winfosumsel)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_CHK_SUM_SEL.bits.winfo_sum_sel =  u32winfosumsel;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
} //lzx



/*********************************check sum****************************************************/




HI_S32 VPSS_REG_SetMadRaddr(HI_U32 *pu32AppAddr,HI_U32 u32MadRaddr)
{
    HI_U32 VPSS_MAD_RADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MAD_RADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MAD_RADDR));
    VPSS_MAD_RADDR = u32MadRaddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MAD_RADDR), VPSS_MAD_RADDR); 

    return HI_SUCCESS;
} //lzxa

HI_S32 VPSS_REG_SetMadWaddr(HI_U32 *pu32AppAddr,HI_U32 u32MadWaddr)
{
    HI_U32 VPSS_MAD_WADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MAD_WADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MAD_WADDR));
    VPSS_MAD_WADDR = u32MadWaddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MAD_WADDR), VPSS_MAD_WADDR); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetMadStride(HI_U32 *pu32AppAddr,HI_U32 u32MadStride)
{
    U_VPSS_MADSTRIDE VPSS_MADSTRIDE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MADSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MADSTRIDE.u32));
    VPSS_MADSTRIDE.bits.mad_stride = u32MadStride;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MADSTRIDE.u32), VPSS_MADSTRIDE.u32); 

    return HI_SUCCESS;
} //lzx


HI_S32 VPSS_REG_SetRfrDither(HI_U32 *pu32AppAddr,HI_U32 u32DitherMode, HI_BOOL bDitherEn)
{
    U_VPSS_RFRCTRL VPSS_RFRCTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_RFRCTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRCTRL.u32));
    VPSS_RFRCTRL.bits.rfr_dither_mode = u32DitherMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRCTRL.u32), VPSS_RFRCTRL.u32); 

    VPSS_RFRCTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRCTRL.u32));
    VPSS_RFRCTRL.bits.rfr_dither_en = bDitherEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRCTRL.u32), VPSS_RFRCTRL.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetRfrYaddr(HI_U32 *pu32AppAddr,HI_U32 u32RfrYaddr)
{
    HI_U32 VPSS_RFRYADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_RFRYADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRYADDR));
    VPSS_RFRYADDR = u32RfrYaddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRYADDR), VPSS_RFRYADDR); 

    return HI_SUCCESS;
} //lzxa	

HI_S32 VPSS_REG_SetRfrCaddr(HI_U32 *pu32AppAddr,HI_U32 u32RfrCaddr)
{
    HI_U32  VPSS_RFRCADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_RFRCADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRCADDR));
    VPSS_RFRCADDR = u32RfrCaddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRCADDR), VPSS_RFRCADDR); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetRfrStride(HI_U32 *pu32AppAddr,HI_U32 u32RfrYStride,HI_U32 u32RfrCStride)
{
    U_VPSS_RFRSTRIDE VPSS_RFRSTRIDE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_RFRSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRSTRIDE.u32));
    VPSS_RFRSTRIDE.bits.rfry_stride = u32RfrYStride;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRSTRIDE.u32), VPSS_RFRSTRIDE.u32); 

    VPSS_RFRSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RFRSTRIDE.u32));
    VPSS_RFRSTRIDE.bits.rfrc_stride = u32RfrCStride;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RFRSTRIDE.u32), VPSS_RFRSTRIDE.u32); 

    return HI_SUCCESS;
} 


HI_S32 VPSS_REG_SetVhd0Dither(HI_U32 *pu32AppAddr,HI_U32 u32DitherMode,HI_BOOL bDitherEn)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_dither_en = bDitherEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_dither_mode = u32DitherMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetVhd0Mirror(HI_U32 *pu32AppAddr,HI_U32 u32Vhd0Mirror)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_mirror = u32Vhd0Mirror;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
} //lzx

HI_S32 VPSS_REG_SetVhd0Flip(HI_U32 *pu32AppAddr,HI_U32 u32Vhd0Flip)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_flip = u32Vhd0Flip;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetVhd0UVInvert(HI_U32 *pu32AppAddr,HI_U32 u32Vhd0uvivt)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_uv_invert = u32Vhd0uvivt;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetVhd0PixW(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixW)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_pix_bitw = ePixW;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetLcmpEn(HI_U32 *pu32AppAddr,HI_U32 LcmpEnable)
{
    U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
    VPSS_VHD0CTRL.bits.vhd0_lcmp_en = LcmpEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetRefWidth(HI_U32 *pu32AppAddr,HI_U32 RefWidth)
{
    U_VPSS_REFSIZE VPSS_REFSIZE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_REFSIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REFSIZE.u32));
    VPSS_REFSIZE.bits.refwidth = RefWidth-1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFSIZE.u32), VPSS_REFSIZE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetRefHight(HI_U32 *pu32AppAddr,HI_U32 RefHight)
{
    U_VPSS_REFSIZE VPSS_REFSIZE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_REFSIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REFSIZE.u32));
    VPSS_REFSIZE.bits.refheight = RefHight-1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFSIZE.u32), VPSS_REFSIZE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetNodeID(HI_U32 *pu32AppAddr,HI_U32 u32NodeID)
{
    HI_U32  VPSS_NODEID;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_NODEID = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NODEID));
    VPSS_NODEID = u32NodeID;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NODEID), VPSS_NODEID); 

    return HI_SUCCESS;
}

#if 0
HI_S32 VPSS_REG_SetAxiRID(HI_U32 *pu32AppAddr,REG_AXI_RID_CHN_E eChn, HI_U32 u32AxiRID)
{
    U_VPSS_RID_CFG0  VPSS_RID_CFG0;
    U_VPSS_RID_CFG1  VPSS_RID_CFG1;
    U_VPSS_RID_CFG2  VPSS_RID_CFG2;
    U_VPSS_RID_CFG3  VPSS_RID_CFG3;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    //TODO:
    switch(eChn)
    {  
        case REG_NX2C_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.nx2c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_NX2Y_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.nx2y_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_NX1C_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.nx1c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_NX1Y_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.nx1y_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_REFC_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.refc_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_REFY_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.refy_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_CURC_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.curc_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;
        case REG_CURY_RID:
            VPSS_RID_CFG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32));
            VPSS_RID_CFG0.bits.cury_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG0.u32), VPSS_RID_CFG0.u32); 
            break;

        case REG_PR0C_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.pr0c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_PR2C_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.pr2c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_PR2Y_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.pr2y_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_PR1C_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.pr1c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_PR1Y_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.pr1y_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_NX3C_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.nx3c_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_NX3Y_ID:
            VPSS_RID_CFG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32));
            VPSS_RID_CFG1.bits.nx3y_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG1.u32), VPSS_RID_CFG1.u32); 
            break;
        case REG_CCRC_ID:
            VPSS_RID_CFG2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32));
            VPSS_RID_CFG2.bits.ccrc_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32), VPSS_RID_CFG2.u32); 
            break;
        case REG_CCRY_ID:
            VPSS_RID_CFG2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32));
            VPSS_RID_CFG2.bits.ccry_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32), VPSS_RID_CFG2.u32); 
            break;
        case REG_SRMD_ID:
            VPSS_RID_CFG2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32));
            VPSS_RID_CFG2.bits.srmd_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32), VPSS_RID_CFG2.u32); 
            break;
        case REG_TRMD_ID:
            VPSS_RID_CFG2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32));
            VPSS_RID_CFG2.bits.trmd_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32), VPSS_RID_CFG2.u32); 
            break;
        case REG_RST_RID:
            VPSS_RID_CFG2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32));
            VPSS_RID_CFG2.bits.rst_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG2.u32), VPSS_RID_CFG2.u32); 
            break;
        case REG_DS_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.ds_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
        case REG_PRJ_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.prj_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
        case REG_RGMV_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.rgmv_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
        case REG_BLKMV_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.blkmv_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
        case REG_REEC_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.reec_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
        case REG_REEY_ID:
            VPSS_RID_CFG3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32));
            VPSS_RID_CFG3.bits.reey_id_sel = u32AxiRID;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RID_CFG3.u32), VPSS_RID_CFG3.u32); 
            break;
       default:
            VPSS_FATAL("Reg Error : %d.\n",eChn);
            break;
    }

    return HI_SUCCESS;
}  
#endif


HI_S32 VPSS_REG_SetSmmuBypass(HI_U32 *pu32AppAddr,REG_SMMU_CHN_E eChn, HI_BOOL bBypass)
{
    return HI_SUCCESS;
}  

HI_S32 VPSS_REG_SetRchSmmuBypass(HI_U32 *pu32AppAddr,HI_U32 u32Mask)
{
    U_VPSS_RCH_BYPASS  VPSS_RCH_BYPASS;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_RCH_BYPASS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RCH_BYPASS.u32));
    VPSS_RCH_BYPASS.u32 = u32Mask;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RCH_BYPASS.u32), VPSS_RCH_BYPASS.u32); 
    return HI_SUCCESS;
}  


HI_S32 VPSS_REG_SetWchSmmuBypass(HI_U32 *pu32AppAddr,HI_U32 u32Mask)
{
    U_VPSS_WCH_BYPASS  VPSS_WCH_BYPASS;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_WCH_BYPASS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_WCH_BYPASS.u32));
    VPSS_WCH_BYPASS.u32 = u32Mask;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_WCH_BYPASS.u32), VPSS_WCH_BYPASS.u32); 

    return HI_SUCCESS;
}  
HI_S32 VPSS_REG_SetGlobalBypass(HI_U8* pu32PhyAddr,HI_BOOL bBypass)
{
    U_SMMU_SCR  VPSS_SMMU_SCR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32PhyAddr;

    VPSS_SMMU_SCR.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32));
    VPSS_SMMU_SCR.bits.glb_bypass = bBypass;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCR.u32), VPSS_SMMU_SCR.u32); 

    return HI_SUCCESS;
}  

HI_S32 VPSS_REG_EnPort(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_CTRL.bits.vhd0_en = bEnable;
            break;
        default:
            VPSS_FATAL("\n ePort Error");
    }

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    


    return HI_SUCCESS;
}


/********************************/
HI_S32 VPSS_REG_SetImgSize(HI_U32 *pu32AppAddr,HI_U32 u32Width,HI_U32 u32Height,HI_BOOL bProgressive)
{
    U_VPSS_IMGSIZE VPSS_IMGSIZE;
    VPSS_REG_S *pstReg;
    (HI_VOID)bProgressive;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_IMGSIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_IMGSIZE.u32));

    VPSS_IMGSIZE.bits.imgheight = u32Height -1;
    VPSS_IMGSIZE.bits.imgwidth = u32Width - 1;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_IMGSIZE.u32), VPSS_IMGSIZE.u32);
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetImgAddr(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Caddr)
{
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFYADDR), u32Yaddr);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFCADDR), u32Caddr);
            break;
        case CUR_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURYADDR), u32Yaddr);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURCADDR), u32Caddr);
            break;
        case NEXT1_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1YADDR), u32Yaddr);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1CADDR), u32Caddr);
            break;
        case NEXT2_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2YADDR), u32Yaddr);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2CADDR), u32Caddr);
            break;
            
        default:
            VPSS_FATAL("FIELD ERROR\n");
    }
    
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetImgAddrLB(HI_U32 *pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Cbaddr)
{
    HI_U32    VPSS_NX2Y_LB_ADDR;
    HI_U32    VPSS_NX2C_LB_ADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_NX2Y_LB_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2Y_LB_ADDR));
    VPSS_NX2Y_LB_ADDR = u32Yaddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2Y_LB_ADDR), VPSS_NX2Y_LB_ADDR);

	VPSS_NX2C_LB_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2C_LB_ADDR));
    VPSS_NX2C_LB_ADDR= u32Cbaddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2C_LB_ADDR), VPSS_NX2C_LB_ADDR);

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetImgStride(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_CURSTRIDE    VPSS_CURSTRIDE;
    U_VPSS_REFSTRIDE    VPSS_REFSTRIDE;
    U_VPSS_NXT1STRIDE   VPSS_NXT1STRIDE;
    U_VPSS_NXT2STRIDE   VPSS_NXT2STRIDE;
    
    VPSS_REG_S *pstReg;
    
    VPSS_INFO("ePos: %d, stride : Y = %d, C = %d\n",ePos,u32YStride,u32CStride);

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_REFSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REFSTRIDE.u32));
            
            VPSS_REFSTRIDE.bits.refy_stride = u32YStride;
            VPSS_REFSTRIDE.bits.refc_stride = u32CStride;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFSTRIDE.u32), VPSS_REFSTRIDE.u32);
            break;
        case CUR_FIELD:
            VPSS_CURSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CURSTRIDE.u32));
            
            VPSS_CURSTRIDE.bits.cury_stride = u32YStride;
            VPSS_CURSTRIDE.bits.curc_stride = u32CStride;
            
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURSTRIDE.u32), VPSS_CURSTRIDE.u32);
            break;
        case NEXT1_FIELD:
            VPSS_NXT1STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT1STRIDE.u32));
            
            VPSS_NXT1STRIDE.bits.nxt1y_stride = u32YStride;
            VPSS_NXT1STRIDE.bits.nxt1c_stride = u32CStride;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1STRIDE.u32), VPSS_NXT1STRIDE.u32);
            break;
        case NEXT2_FIELD:
            VPSS_NXT2STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT2STRIDE.u32));
            
            VPSS_NXT2STRIDE.bits.nxt2y_stride = u32YStride;
            VPSS_NXT2STRIDE.bits.nxt2c_stride = u32CStride;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2STRIDE.u32), VPSS_NXT2STRIDE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetImgStrideLB(HI_U32 *pu32AppAddr,HI_U32 u32YStride)
{
    U_VPSS_NX2_LB_STRIDE   VPSS_NX2_LB_STRIDE;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_NX2_LB_STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NX2_LB_STRIDE.u32));
    VPSS_NX2_LB_STRIDE.bits.nx2lb_stride = u32YStride;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NX2_LB_STRIDE.u32), VPSS_NX2_LB_STRIDE.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetReadPixBitWidth(HI_U32 *pu32AppAddr,REG_CHANELPOS_E eChn,HI_DRV_PIXEL_BITWIDTH_E enBitWidth)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(eChn)
    {       
        case CUR_CHANEL:
            VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2));
            
            VPSS_CTRL2.bits.in_pix_bitw = enBitWidth;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2), VPSS_CTRL2.u32 );
            break;
        case REF_CHANEL:
            VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2));
            
            VPSS_CTRL2.bits.ref_nxt_pix_bitw = enBitWidth;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2), VPSS_CTRL2.u32 );
            break;
        default:
            VPSS_FATAL("FIELD ERROR\n");
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetWbcPixBitWidth(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E enBitWidth)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2));
    
    VPSS_CTRL2.bits.rfr_pix_bitw = enBitWidth;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2), VPSS_CTRL2.u32 );
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetImgTile(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnTile)
{
    U_VPSS_REF_CTRL VPSS_REF_CTRL;
    U_VPSS_CUR_CTRL VPSS_CUR_CTRL;
    U_VPSS_NXT1_CTRL VPSS_NXT1_CTRL;
    U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_REF_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REF_CTRL));
            
            VPSS_REF_CTRL.bits.ref_tile_format = EnTile;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REF_CTRL), VPSS_REF_CTRL.u32 );
            break;
            
        case CUR_FIELD:
            VPSS_CUR_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CUR_CTRL));
            
            VPSS_CUR_CTRL.bits.cur_tile_format = EnTile;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUR_CTRL), VPSS_CUR_CTRL.u32 );
            break;
        case NEXT1_FIELD:
            VPSS_NXT1_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT1_CTRL));
            
            VPSS_NXT1_CTRL.bits.nxt1_tile_format = EnTile;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1_CTRL), VPSS_NXT1_CTRL.u32 );
            break;
        case NEXT2_FIELD:
            VPSS_NXT2_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL));
            
            VPSS_NXT2_CTRL.bits.nxt2_tile_format = EnTile;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL), VPSS_NXT2_CTRL.u32 );
            break;
        default:
            VPSS_FATAL("FIELD ERROR\n");
    }
    
    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetImgOffset(HI_U32 *pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32HorOffset,HI_U32 u32VerOffset)
{
   
    U_VPSS_REF_CTRL VPSS_REF_CTRL;
    U_VPSS_CUR_CTRL VPSS_CUR_CTRL;
    U_VPSS_NXT1_CTRL VPSS_NXT1_CTRL;
    U_VPSS_NXT2_CTRL VPSS_NXT2_CTRL;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_REF_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REF_CTRL));
            
            VPSS_REF_CTRL.bits.ref_ver_offset = u32VerOffset;
            VPSS_REF_CTRL.bits.ref_hor_offset = u32HorOffset;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REF_CTRL), VPSS_REF_CTRL.u32 );
            break;
            
        case CUR_FIELD:
            VPSS_CUR_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CUR_CTRL));
            
            VPSS_CUR_CTRL.bits.cur_ver_offset = u32VerOffset;
            VPSS_CUR_CTRL.bits.cur_hor_offset = u32HorOffset;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUR_CTRL), VPSS_CUR_CTRL.u32 );
            break;
        case NEXT1_FIELD:
            VPSS_NXT1_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT1_CTRL));
            
            VPSS_NXT1_CTRL.bits.nxt1_ver_offset = u32VerOffset;
            VPSS_NXT1_CTRL.bits.nxt1_hor_offset = u32HorOffset;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1_CTRL), VPSS_NXT1_CTRL.u32 );
            break;
        case NEXT2_FIELD:
            VPSS_NXT2_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL));
            
            VPSS_NXT2_CTRL.bits.nxt2_ver_offset = u32VerOffset;
            VPSS_NXT2_CTRL.bits.nxt2_hor_offset = u32HorOffset;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2_CTRL), VPSS_NXT2_CTRL.u32 );
            break;
            
        default:
            VPSS_FATAL("FIELD ERROR\n");
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetImgFormat(HI_U32 *pu32AppAddr,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    VPSS_REG_S *pstReg;
    
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
            VPSS_CTRL2.bits.in_format = 0x0;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_NV61:
            VPSS_CTRL2.bits.in_format = 0x1;
            break;
        case HI_DRV_PIX_FMT_YUV400:
            VPSS_CTRL2.bits.in_format = 0x4;
            break; 
        case HI_DRV_PIX_FMT_YUV_444:
            VPSS_CTRL2.bits.in_format = 0x5;
            break;  
        case HI_DRV_PIX_FMT_YUV422_2X1:
            VPSS_CTRL2.bits.in_format = 0x6;
            break;
        case HI_DRV_PIX_FMT_YUV420p:
            VPSS_CTRL2.bits.in_format = 0x8;
            break;  
        case HI_DRV_PIX_FMT_YUV411:
            VPSS_CTRL2.bits.in_format = 0x9;
            break;  
        case HI_DRV_PIX_FMT_YUV410p:
            VPSS_CTRL2.bits.in_format = 0xa;
            break;  
        case HI_DRV_PIX_FMT_YUYV:
            VPSS_CTRL2.bits.in_format = 0xb;
            break;  
        case HI_DRV_PIX_FMT_YVYU:
            VPSS_CTRL2.bits.in_format = 0xc;
            break;  
        case HI_DRV_PIX_FMT_UYVY:
            VPSS_CTRL2.bits.in_format = 0xd;
            break; 
        default:
            VPSS_FATAL("REG ERROR %d\n",eFormat);
    }
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetImgReadMod(HI_U32 *pu32AppAddr,HI_BOOL bField)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield = bField;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return HI_SUCCESS;
}
/********************************/



/********************************/
HI_S32 VPSS_REG_SetFrmSize(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VHD0SIZE VPSS_VHD0SIZE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0SIZE.u32));
            VPSS_VHD0SIZE.bits.vhd0_height = u32Height-1;
            VPSS_VHD0SIZE.bits.vhd0_width = u32Width-1;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0SIZE.u32), VPSS_VHD0SIZE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetChkSumAddr(HI_U32 *pu32AppAddr,HI_U32 u32CKSUMaddr)
{
    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_W_ADDR), u32CKSUMaddr);           
    return HI_SUCCESS;
}//lzx

HI_S32 VPSS_REG_SetSttWrAddr(HI_U32 *pu32AppAddr,HI_U32 u32SttWrAddr)
{
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_STT_W_ADDR), u32SttWrAddr); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetFrmAddr(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr)
{
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0YADDR), u32Yaddr);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CADDR), u32Caddr);            
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetFrmStride(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_VHD0STRIDE VPSS_VHD0STRIDE;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0STRIDE.u32));
            VPSS_VHD0STRIDE.bits.vhd0y_stride = u32YStride;
            VPSS_VHD0STRIDE.bits.vhd0c_stride = u32CStride;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0STRIDE.u32), VPSS_VHD0STRIDE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetFrmFormat(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    HI_U32 u32Format = 0;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
            u32Format = 0x0;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_NV61:
            u32Format = 0x1;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            return HI_FAILURE;
    }
    
    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_CTRL2.bits.vhd0_format = u32Format;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
     
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32);
    return HI_SUCCESS;
}
/********************************/

/********************************/
HI_S32 VPSS_REG_SetZmeGlbEnable(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL3 VPSS_CTRL3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.zme_vhd0_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetZmeEnable(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD0_VSP VPSS_VHD0_VSP;
    U_VPSS_VHD0_HSP VPSS_VHD0_HSP;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode ==  REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hlmsc_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hchmsc_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vlmsc_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vchmsc_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VHD0_ZMEORESO VPSS_VHD0_ZMEORESO;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0_ZMEORESO.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZMEORESO.u32));
            VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh = u32Height - 1;
            VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow = u32Width - 1;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZMEORESO.u32), VPSS_VHD0_ZMEORESO.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeFirEnable(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD0_VSP VPSS_VHD0_VSP;
    U_VPSS_VHD0_HSP VPSS_VHD0_HSP;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode ==  REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hlfir_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hchfir_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vlfir_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vchfir_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeMidEnable(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD0_VSP VPSS_VHD0_VSP;
    U_VPSS_VHD0_HSP VPSS_VHD0_HSP;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode == REG_ZME_MODE_HORL)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hlmid_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode ==  REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hchmid_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vlmid_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
                VPSS_VHD0_VSP.bits.vchmid_en = bEnable;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmePhase(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_S32 s32Phase)
{
    U_VPSS_VHD0_HLOFFSET VPSS_VHD0_HLOFFSET;
    U_VPSS_VHD0_HCOFFSET VPSS_VHD0_HCOFFSET;
    U_VPSS_VHD0_VOFFSET VPSS_VHD0_VOFFSET;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VHD0_HCOFFSET.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HCOFFSET.u32));
                VPSS_VHD0_HCOFFSET.bits.hor_coffset = s32Phase;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HCOFFSET.u32), VPSS_VHD0_HCOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VHD0_HLOFFSET.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HLOFFSET.u32));
                VPSS_VHD0_HLOFFSET.bits.hor_loffset = s32Phase;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HLOFFSET.u32), VPSS_VHD0_HLOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VHD0_VOFFSET.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VOFFSET.u32));
                VPSS_VHD0_VOFFSET.bits.vchroma_offset = s32Phase;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VOFFSET.u32), VPSS_VHD0_VOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VHD0_VOFFSET.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VOFFSET.u32));
                VPSS_VHD0_VOFFSET.bits.vluma_offset = s32Phase;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VOFFSET.u32), VPSS_VHD0_VOFFSET.u32); 
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
HI_S32 VPSS_REG_SetZmeRatio(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Ratio)
{
    U_VPSS_VHD0_HSP VPSS_VHD0_HSP;
    U_VPSS_VHD0_VSR VPSS_VHD0_VSR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HOR)
            {
                VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
                VPSS_VHD0_HSP.bits.hratio = u32Ratio;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            }
            else
            {
                VPSS_VHD0_VSR.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSR.u32));
                VPSS_VHD0_VSR.bits.vratio = u32Ratio;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSR.u32), VPSS_VHD0_VSR.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
HI_S32 VPSS_REG_SetZmeHfirOrder(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirst)
{
    U_VPSS_VHD0_HSP VPSS_VHD0_HSP;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0_HSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32));
            VPSS_VHD0_HSP.bits.hfir_order = bVfirst;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_HSP.u32), VPSS_VHD0_HSP.u32); 
            break;
		default:
			break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeInFmt(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_VHD0_VSP VPSS_VHD0_VSP;

    HI_U32 u32Format;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16:
            u32Format = 0x0;
            break;
        default:
            VPSS_FATAL("REG ERROR format %d\n",eFormat);
            return HI_FAILURE;
    }
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
            VPSS_VHD0_VSP.bits.zme_in_fmt = u32Format;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeOutFmt(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_VHD0_VSP VPSS_VHD0_VSP;

    HI_U32 u32Format;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16:
            u32Format = 0x0;
            break;
        default:
            VPSS_FATAL("REG ERROR format %d\n",eFormat);
            return HI_FAILURE;
    }
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0_VSP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32));
            VPSS_VHD0_VSP.bits.zme_out_fmt = u32Format;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_VSP.u32), VPSS_VHD0_VSP.u32); 
            
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr)
{

    HI_U32 VPSS_VHD0_ZME_LHADDR;
    HI_U32 VPSS_VHD0_ZME_LVADDR;
    HI_U32 VPSS_VHD0_ZME_CHADDR;
    HI_U32 VPSS_VHD0_ZME_CVADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VHD0_ZME_CHADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_CHADDR));
                VPSS_VHD0_ZME_CHADDR = u32Addr;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_CHADDR), VPSS_VHD0_ZME_CHADDR); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VHD0_ZME_LHADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_LHADDR));
                VPSS_VHD0_ZME_LHADDR = u32Addr;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_LHADDR), VPSS_VHD0_ZME_LHADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VHD0_ZME_CVADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_CVADDR));
                VPSS_VHD0_ZME_CVADDR = u32Addr;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_CVADDR), VPSS_VHD0_ZME_CVADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VHD0_ZME_LVADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_LVADDR));
                VPSS_VHD0_ZME_LVADDR = u32Addr;
                VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_ZME_LVADDR), VPSS_VHD0_ZME_LVADDR);
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
                                    
/********************************/

/********************************/

HI_VOID VPSS_REG_SetDetMode(HI_U32 *pu32AppAddr,HI_U32 u32Mode)
{
    U_STR_DET_VIDCTRL STR_DET_VIDCTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    STR_DET_VIDCTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDCTRL.u32));
    STR_DET_VIDCTRL.bits.vid_mode = u32Mode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDCTRL.u32), STR_DET_VIDCTRL.u32);

    return;
}
HI_VOID VPSS_REG_SetDetBlk(HI_U32 *pu32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr)
{
    U_STR_DET_VIDBLKPOS0_1 STR_DET_VIDBLKPOS0_1;
    U_STR_DET_VIDBLKPOS2_3 STR_DET_VIDBLKPOS2_3;
    U_STR_DET_VIDBLKPOS4_5 STR_DET_VIDBLKPOS4_5;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    switch (blk_id)
    {
        case 0:
        {
            STR_DET_VIDBLKPOS0_1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS0_1.u32));
            
            STR_DET_VIDBLKPOS0_1.bits.blk0_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS0_1.bits.blk0_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS0_1.u32), STR_DET_VIDBLKPOS0_1.u32);
            break;
        }
        case 1:
        {
            STR_DET_VIDBLKPOS0_1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS0_1.u32));
            
            STR_DET_VIDBLKPOS0_1.bits.blk1_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS0_1.bits.blk1_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS0_1.u32), STR_DET_VIDBLKPOS0_1.u32);
            
            break;
        }
        case 2:
        {
            STR_DET_VIDBLKPOS2_3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS2_3.u32));
            
            STR_DET_VIDBLKPOS2_3.bits.blk2_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS2_3.bits.blk2_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS2_3.u32), STR_DET_VIDBLKPOS2_3.u32);
            break;
        }
        case 3:
        {
            STR_DET_VIDBLKPOS2_3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS2_3.u32));
            
            STR_DET_VIDBLKPOS2_3.bits.blk3_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS2_3.bits.blk3_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS2_3.u32), STR_DET_VIDBLKPOS2_3.u32);
            
            break;
        }
        case 4:
        {
            STR_DET_VIDBLKPOS4_5.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS4_5.u32));
            
            STR_DET_VIDBLKPOS4_5.bits.blk4_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS4_5.bits.blk4_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS4_5.u32), STR_DET_VIDBLKPOS4_5.u32);
            break;
        }
        case 5:
        {
            STR_DET_VIDBLKPOS4_5.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS4_5.u32));
            
            STR_DET_VIDBLKPOS4_5.bits.blk5_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS4_5.bits.blk5_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->STR_DET_VIDBLKPOS4_5.u32), STR_DET_VIDBLKPOS4_5.u32);
            
            break;
        }

        default:
        {
            VPSS_FATAL("Error! Wrong Vou_SetViDetBlk() ID Select\n");
            return ;
        }
    }

    return ;

}

/********************************/


/*************************************************************************************************/

/*DEI*/
HI_S32 VPSS_REG_RstDei(HI_U32 *pu32AppAddr,HI_BOOL bRst)//,HI_U32 u32RstValue)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.die_rst = bRst;
    //VPSS_DIECTRL.bits.st_rst_value = u32RstValue;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 *pu32AppAddr,HI_BOOL bTopFirst)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    //HI_SV_TRACE("bfield_first : %d.\n",bTopFirst);
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield_first = !bTopFirst;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
} 
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 *pu32AppAddr,HI_BOOL bBottom)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    //HI_SV_TRACE("bfield_mode : %d.\n",bBottom);
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield_mode = bBottom;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
	

    return HI_SUCCESS;
    
}


HI_S32 VPSS_REG_SetDeiAddr(HI_U32 *pu32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr)
{

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    switch(eField)
    {   
        case LAST_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFYADDR), u32YAddr); 
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFCADDR), u32CAddr); 
            break;

        case CUR_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURYADDR), u32YAddr); 
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURCADDR), u32CAddr); 
            break;

        case NEXT1_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1YADDR), u32YAddr); 
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1CADDR), u32CAddr); 
            break;

        case NEXT2_FIELD:
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2YADDR), u32YAddr); 
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2CADDR), u32CAddr); 
            break;    

        default:
            VPSS_FATAL("REG ERROR\n");

    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDeiStride(HI_U32 *pu32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_REFSTRIDE    VPSS_REFSTRIDE;
    U_VPSS_CURSTRIDE    VPSS_CURSTRIDE;
    U_VPSS_NXT1STRIDE   VPSS_NXT1STRIDE;
    U_VPSS_NXT2STRIDE   VPSS_NXT2STRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_INFO("stride : Y = %d, C = %d\n",u32YStride,u32CStride);

    switch(eField)
    {   
        case LAST_FIELD:
            VPSS_REFSTRIDE.bits.refy_stride= u32YStride;
            VPSS_REFSTRIDE.bits.refc_stride= u32CStride;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REFSTRIDE.u32), VPSS_REFSTRIDE.u32); 
            break;
            
        case CUR_FIELD:
            VPSS_CURSTRIDE.bits.cury_stride = u32YStride;
            VPSS_CURSTRIDE.bits.curc_stride= u32CStride;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CURSTRIDE.u32), VPSS_CURSTRIDE.u32); 
            break;
            
        case NEXT1_FIELD:
            VPSS_NXT1STRIDE.bits.nxt1y_stride= u32YStride;
            VPSS_NXT1STRIDE.bits.nxt1c_stride= u32CStride;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT1STRIDE.u32), VPSS_NXT1STRIDE.u32); 
            break;
            
        case NEXT2_FIELD:
            VPSS_NXT2STRIDE.bits.nxt2y_stride= u32YStride;
            VPSS_NXT2STRIDE.bits.nxt2c_stride= u32CStride;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2STRIDE.u32), VPSS_NXT2STRIDE.u32); 
            break;
            
        default:
            VPSS_FATAL("REG ERROR\n");

    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetModeEn(HI_U32 *pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode)
{
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetOutSel(HI_U32 *pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_out_sel_c = bEnMode;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_out_sel_l = bEnMode;
    }
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMode(HI_U32 *pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_U32  u32Mode)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_c_mode = u32Mode;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_l_mode = u32Mode;
    }
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStInfo(HI_U32 *pu32AppAddr,HI_BOOL bStop)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.stinfo_stop = bStop;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMfMax(HI_U32 *pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bMax)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIELMA2.bits.chroma_mf_max = bMax;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIELMA2.bits.luma_mf_max = bMax;
    }
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetLuSceSdfMax(HI_U32 *pu32AppAddr,HI_BOOL bMax)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.luma_scesdf_max = bMax;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecModeEn(HI_U32 *pu32AppAddr,HI_BOOL bEnMode)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.rec_mode_en = bEnMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrEn(HI_U32 *pu32AppAddr,HI_BOOL bEnIIr)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.motion_iir_en = bEnIIr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionSmoothEn(HI_U32 *pu32AppAddr,HI_BOOL bEnSmooth)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.frame_motion_smooth_en = bEnSmooth;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionBlendEn(HI_U32 *pu32AppAddr,HI_BOOL bEnBlend)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.recmode_frmfld_blend_mode = bEnBlend;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetChromeOffset(HI_U32 *pu32AppAddr,HI_U8 u8Offset)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.chroma_mf_offset = u8Offset;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMinIntern(HI_U32 *pu32AppAddr,HI_U32 u32MinIntern)
{
    U_VPSS_DIEINTEN VPSS_DIEINTEN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEINTEN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIEINTEN.bits.min_north_strength = u32MinIntern;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEINTEN.u32), VPSS_DIEINTEN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetInternVer(HI_U32 *pu32AppAddr,HI_U32 u32InternVer)
{
    U_VPSS_DIEINTEN VPSS_DIEINTEN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEINTEN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIEINTEN.bits.dir_ratio_north = u32InternVer;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEINTEN.u32), VPSS_DIEINTEN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRangeScale(HI_U32 *pu32AppAddr,HI_U32 u32Scale)
{
    U_VPSS_DIESCALE VPSS_DIESCALE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIESCALE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIESCALE.bits.range_scale = u32Scale;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIESCALE.u32), VPSS_DIESCALE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCK1(HI_U32 *pu32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max)
{
    U_VPSS_DIECHECK1 VPSS_DIECHECK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECHECK1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECHECK1.u32));
    VPSS_DIECHECK1.bits.bc1_gain = u32Gain;
    VPSS_DIECHECK1.bits.bc1_autodz_gain = u32Range;
    VPSS_DIECHECK1.bits.bc1_max_dz = u32Max;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECHECK1.u32), VPSS_DIECHECK1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCK2(HI_U32 *pu32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max)
{
    U_VPSS_DIECHECK2 VPSS_DIECHECK2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECHECK2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECHECK2.u32));
    VPSS_DIECHECK2.bits.bc2_gain = u32Gain;
    VPSS_DIECHECK2.bits.bc2_autodz_gain =  u32Range;
    VPSS_DIECHECK2.bits.bc2_max_dz = u32Max;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECHECK2.u32), VPSS_DIECHECK2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDIR(HI_U32 *pu32AppAddr,HI_S32 *ps32MultDir)
{
    U_VPSS_DIEDIR0_3 VPSS_DIEDIR0_3;
    U_VPSS_DIEDIR4_7 VPSS_DIEDIR4_7;
    U_VPSS_DIEDIR8_11 VPSS_DIEDIR8_11;
    U_VPSS_DIEDIR12_14 VPSS_DIEDIR12_14;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIR0_3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIR0_3.u32));
    VPSS_DIEDIR0_3.bits.dir0_scale = ps32MultDir[0];
    VPSS_DIEDIR0_3.bits.dir1_scale = ps32MultDir[1];
    VPSS_DIEDIR0_3.bits.dir2_scale = ps32MultDir[2];
    VPSS_DIEDIR0_3.bits.dir3_scale = ps32MultDir[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIR0_3.u32), VPSS_DIEDIR0_3.u32);
    
    VPSS_DIEDIR4_7.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIR4_7.u32));
    VPSS_DIEDIR4_7.bits.dir4_scale = ps32MultDir[4];
    VPSS_DIEDIR4_7.bits.dir5_scale = ps32MultDir[5];
    VPSS_DIEDIR4_7.bits.dir6_scale = ps32MultDir[6];
    VPSS_DIEDIR4_7.bits.dir7_scale = ps32MultDir[7];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIR4_7.u32), VPSS_DIEDIR4_7.u32);
    
    VPSS_DIEDIR8_11.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIR8_11.u32));
    VPSS_DIEDIR8_11.bits.dir8_scale = ps32MultDir[8];
    VPSS_DIEDIR8_11.bits.dir9_scale = ps32MultDir[9];
    VPSS_DIEDIR8_11.bits.dir10_scale = ps32MultDir[10];
    VPSS_DIEDIR8_11.bits.dir11_scale = ps32MultDir[11];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIR8_11.u32), VPSS_DIEDIR8_11.u32);
    
    VPSS_DIEDIR12_14.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIR12_14.u32));
    VPSS_DIEDIR12_14.bits.dir12_scale = ps32MultDir[12];
    VPSS_DIEDIR12_14.bits.dir13_scale = ps32MultDir[13];
    VPSS_DIEDIR12_14.bits.dir14_scale = ps32MultDir[14];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIR12_14.u32), VPSS_DIEDIR12_14.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 *pu32AppAddr,HI_S32 *ps32Rat)
{
    U_VPSS_DIEINTPSCL0 VPSS_DIEINTPSCL0;
    U_VPSS_DIEINTPSCL1 VPSS_DIEINTPSCL1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEINTPSCL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEINTPSCL0.u32));
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_1 = ps32Rat[0];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_2 = ps32Rat[1];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_3 = ps32Rat[2];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_4 = ps32Rat[3];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_5 = ps32Rat[4];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_6 = ps32Rat[5];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_7 = ps32Rat[6];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_8 = ps32Rat[7];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEINTPSCL0.u32), VPSS_DIEINTPSCL0.u32);

    VPSS_DIEINTPSCL1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEINTPSCL1.u32));
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_9  = ps32Rat[8];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_10 = ps32Rat[9];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_11 = ps32Rat[10];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_12 = ps32Rat[11];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_13 = ps32Rat[12];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_14 = ps32Rat[13];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_15 = ps32Rat[14];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEINTPSCL1.u32), VPSS_DIEINTPSCL1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStrenThd(HI_U32 *pu32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.strength_thd = s32Thd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDirThd(HI_U32 *pu32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.dir_thd = s32Thd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetBcGain(HI_U32 *pu32AppAddr,HI_S32 s32BcGain)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.bc_gain = s32BcGain;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_EnHorEdge(HI_U32 *pu32AppAddr,HI_BOOL bEdgeEn)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.hor_edge_en = bEdgeEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_EnEdgeMode(HI_U32 *pu32AppAddr,HI_BOOL bEdgeModeEn)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.edge_mode = bEdgeModeEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterCoring(HI_U32 *pu32AppAddr,HI_S32 s32Coring)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_coring = s32Coring;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterGain(HI_U32 *pu32AppAddr,HI_S32 s32Gain)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_gain = s32Gain;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionSlope(HI_U32 *pu32AppAddr,HI_S32 s32Slope)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_curve_slope = s32Slope;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}

HI_S32 VPSS_REG_SetMotionCoring(HI_U32 *pu32AppAddr,HI_S32 s32Coring)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.fld_motion_coring = s32Coring;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;        
}
HI_S32 VPSS_REG_SetMotionGain(HI_U32 *pu32AppAddr,HI_S32 s32Gain)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_gain = s32Gain;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionHThd(HI_U32 *pu32AppAddr,HI_S32 s32HThd)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_thd_high = s32HThd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionLThd(HI_U32 *pu32AppAddr,HI_S32 s32LThd)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_thd_low = s32LThd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}

HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 *pu32AppAddr,HI_S32 *ps32Thd)
{
    U_VPSS_DIEMTNDIFFTHD0 VPSS_DIEMTNDIFFTHD0;
    U_VPSS_DIEMTNDIFFTHD1 VPSS_DIEMTNDIFFTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEMTNDIFFTHD0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD0.u32));
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_0 = ps32Thd[0];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_1 = ps32Thd[1];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_2 = ps32Thd[2];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_3 = ps32Thd[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD0.u32), VPSS_DIEMTNDIFFTHD0.u32);

    VPSS_DIEMTNDIFFTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD1.u32));
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_4 = ps32Thd[4];
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_5 = ps32Thd[5];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD1.u32), VPSS_DIEMTNDIFFTHD1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 *pu32AppAddr,HI_S32 *ps32Slope)
{
    U_VPSS_DIEMTNIIRSLP VPSS_DIEMTNIIRSLP;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEMTNIIRSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRSLP.u32));
    VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_0 = ps32Slope[0];
    VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_1 = ps32Slope[1];
    VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_2 = ps32Slope[2];
    VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_3 = ps32Slope[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRSLP.u32), VPSS_DIEMTNIIRSLP.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 *pu32AppAddr,HI_S32 *ps32Ratio)
{
    U_VPSS_DIEMTNIIRSLP VPSS_DIEMTNIIRSLP;
    U_VPSS_DIEMTNIIRRAT VPSS_DIEMTNIIRRAT;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEMTNIIRSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRSLP.u32));
    VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_ratio_0 = ps32Ratio[0];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRSLP.u32), VPSS_DIEMTNIIRSLP.u32);

    VPSS_DIEMTNIIRRAT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRRAT.u32));
    VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_1 = ps32Ratio[1];
    VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_2 = ps32Ratio[2];
    VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_3 = ps32Ratio[3];
    VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_4 = ps32Ratio[4];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNIIRRAT.u32), VPSS_DIEMTNIIRRAT.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionIIrRatioMaxMin(HI_U32 *pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min)
{
    U_VPSS_DIEMTNDIFFTHD1 VPSS_DIEMTNDIFFTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEMTNDIFFTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD1.u32));
    VPSS_DIEMTNDIFFTHD1.bits.max_motion_iir_ratio = s32Max;
    VPSS_DIEMTNDIFFTHD1.bits.min_motion_iir_ratio = s32Min;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNDIFFTHD1.u32), VPSS_DIEMTNDIFFTHD1.u32);

    return HI_SUCCESS;
}
//////////////////////
HI_S32 VPSS_REG_SetSmoothDiffThd(HI_U32 *pu32AppAddr,HI_S32 *ps32Thd)
{
    U_VPSS_DIEFRMMTNSMTHTHD0 VPSS_DIEFRMMTNSMTHTHD0;
    U_VPSS_DIEFRMMTNSMTHTHD1 VPSS_DIEFRMMTNSMTHTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMMTNSMTHTHD0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD0.u32));
    VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd0 = ps32Thd[0];
    VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd1 = ps32Thd[1];
    VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd2 = ps32Thd[2];
    VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd3 = ps32Thd[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD0.u32), VPSS_DIEFRMMTNSMTHTHD0.u32);

    VPSS_DIEFRMMTNSMTHTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD1.u32));
    VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd4 = ps32Thd[4];
    VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd5 = ps32Thd[5];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD1.u32), VPSS_DIEFRMMTNSMTHTHD1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetSmoothSlope(HI_U32 *pu32AppAddr,HI_S32 *ps32Slope)
{
    U_VPSS_DIEFRMMTNSMTHSLP VPSS_DIEFRMMTNSMTHSLP;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMMTNSMTHSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHSLP.u32));
    VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope0 = ps32Slope[0];
    VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope1 = ps32Slope[1];
    VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope2 = ps32Slope[2];
    VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope3 = ps32Slope[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHSLP.u32), VPSS_DIEFRMMTNSMTHSLP.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetSmoothRatio(HI_U32 *pu32AppAddr,HI_S32 *ps32Ratio)
{
    U_VPSS_DIEFRMMTNSMTHSLP VPSS_DIEFRMMTNSMTHSLP;
    U_VPSS_DIEFRMMTNSMTHRAT VPSS_DIEFRMMTNSMTHRAT;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMMTNSMTHSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHSLP.u32));
    VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_ratio0 = ps32Ratio[0];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHSLP.u32), VPSS_DIEFRMMTNSMTHSLP.u32);

    VPSS_DIEFRMMTNSMTHRAT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHRAT.u32));
    VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio1 = ps32Ratio[1];
    VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio2 = ps32Ratio[2];
    VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio3 = ps32Ratio[3];
    VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio4 = ps32Ratio[4];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHRAT.u32), VPSS_DIEFRMMTNSMTHRAT.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetSmoothRatioMaxMin(HI_U32 *pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min)
{
    U_VPSS_DIEFRMMTNSMTHTHD1 VPSS_DIEFRMMTNSMTHTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMMTNSMTHTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD1.u32));
    VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_max = s32Max;
    VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_min = s32Min;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMMTNSMTHTHD1.u32), VPSS_DIEFRMMTNSMTHTHD1.u32);

    return HI_SUCCESS;
}
///////////////////////
HI_S32 VPSS_REG_SetBlendDiffThd(HI_U32 *pu32AppAddr,HI_S32 *ps32Thd)
{
    U_VPSS_DIEFRMFLDBLENDTHD0 VPSS_DIEFRMFLDBLENDTHD0;
    U_VPSS_DIEFRMFLDBLENDTHD1 VPSS_DIEFRMFLDBLENDTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMFLDBLENDTHD0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD0.u32));
    VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd0 = ps32Thd[0];
    VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd1 = ps32Thd[1];
    VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd2 = ps32Thd[2];
    VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd3 = ps32Thd[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD0.u32), VPSS_DIEFRMFLDBLENDTHD0.u32);

    VPSS_DIEFRMFLDBLENDTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD1.u32));
    VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd4 = ps32Thd[4];
    VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd5 = ps32Thd[5];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD1.u32), VPSS_DIEFRMFLDBLENDTHD1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetBlendSlope(HI_U32 *pu32AppAddr,HI_S32 *ps32Slope)
{
    U_VPSS_DIEFRMFLDBLENDSLP VPSS_DIEFRMFLDBLENDSLP;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMFLDBLENDSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDSLP.u32));
    VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope0 = ps32Slope[0];
    VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope1 = ps32Slope[1];
    VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope2 = ps32Slope[2];
    VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope3 = ps32Slope[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDSLP.u32), VPSS_DIEFRMFLDBLENDSLP.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetBlendRatio(HI_U32 *pu32AppAddr,HI_S32 *ps32Ratio)
{
    U_VPSS_DIEFRMFLDBLENDSLP VPSS_DIEFRMFLDBLENDSLP;
    U_VPSS_DIEFRMFLDBLENDRAT VPSS_DIEFRMFLDBLENDRAT;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMFLDBLENDSLP.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDSLP.u32));
    VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_ratio0 = ps32Ratio[0];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDSLP.u32), VPSS_DIEFRMFLDBLENDSLP.u32);

    VPSS_DIEFRMFLDBLENDRAT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDRAT.u32));
    VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio1 = ps32Ratio[1];
    VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio2 = ps32Ratio[2];
    VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio3 = ps32Ratio[3];
    VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio4 = ps32Ratio[4];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDRAT.u32), VPSS_DIEFRMFLDBLENDRAT.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetBlendRatioMaxMin(HI_U32 *pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min)
{
    U_VPSS_DIEFRMFLDBLENDTHD1 VPSS_DIEFRMFLDBLENDTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEFRMFLDBLENDTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD1.u32));
    VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_max = s32Max;
    VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_min = s32Min;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFRMFLDBLENDTHD1.u32), VPSS_DIEFRMFLDBLENDTHD1.u32);

    return HI_SUCCESS;
}
///////////////////////
HI_S32 VPSS_REG_SetRecFldStep(HI_U32 *pu32AppAddr,HI_S32 *ps32Step)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_0 = ps32Step[0];
    VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_1 = ps32Step[1];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 *pu32AppAddr,HI_S32 *ps32Step)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_0 = ps32Step[0];
    VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_1 = ps32Step[1];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisEn(HI_U32 *pu32AppAddr,HI_BOOL bEnHis)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_en = bEnHis;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisInfoWrMode(HI_U32 *pu32AppAddr,HI_BOOL bHisMtnInfoWrMd)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_info_write_mode = bHisMtnInfoWrMd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetHisWrMode(HI_U32 *pu32AppAddr,HI_BOOL bHisMtnWrMd)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_write_mode = bHisMtnWrMd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisUseMode(HI_U32 *pu32AppAddr,HI_BOOL bHisMtnUseMd)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_using_mode = bHisMtnUseMd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetMorFlt(HI_U32 *pu32AppAddr,HI_BOOL bEnflt,HI_S8 s8FltSize,HI_S8 s8FltThd)
{
  U_VPSS_DIEMORFLT VPSS_DIEMORFLT;
  VPSS_REG_S *pstReg;

  pstReg = (VPSS_REG_S*)pu32AppAddr;

  VPSS_DIEMORFLT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMORFLT.u32));

  VPSS_DIEMORFLT.bits.mor_flt_en   = bEnflt;
  VPSS_DIEMORFLT.bits.mor_flt_size = s8FltSize;
  VPSS_DIEMORFLT.bits.mor_flt_thd  = s8FltThd;
  
  VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMORFLT.u32), VPSS_DIEMORFLT.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionAdj(HI_U32 *pu32AppAddr,HI_S32 s32Gain,HI_S32 s32Chom,HI_S32 s32Coring)
{
  U_VPSS_DIEMTNADJ VPSS_DIEMTNADJ;
  VPSS_REG_S *pstReg;

  pstReg = (VPSS_REG_S*)pu32AppAddr;

  VPSS_DIEMTNADJ.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMTNADJ.u32));

  VPSS_DIEMTNADJ.bits.motion_adjust_gain   = s32Gain;
  VPSS_DIEMTNADJ.bits.motion_adjust_coring = s32Coring;
  VPSS_DIEMTNADJ.bits.motion_adjust_gain_chr  = s32Chom;
  
  VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMTNADJ.u32), VPSS_DIEMTNADJ.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionThd(HI_U32 *pu32AppAddr,HI_S32 s32Small,HI_S32 s32Large)
{
//TODO!!!
#if 0
  U_VPSS_DIEGLBMTNTHD VPSS_DIEGLBMTNTHD;
  VPSS_REG_S *pstReg;

  pstReg = (VPSS_REG_S*)pu32AppAddr;

  VPSS_DIEGLBMTNTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEGLBMTNTHD.u32));

  VPSS_DIEGLBMTNTHD.bits.small_motion_thd   = s32Small;
  VPSS_DIEGLBMTNTHD.bits.large_motion_thd   = s32Large;
  
  VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEGLBMTNTHD.u32), VPSS_DIEGLBMTNTHD.u32);
#endif
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetBlendEn(HI_U32 *pu32AppAddr,HI_BOOL bEnBlend)
{
  U_VPSS_DIEMORFLT VPSS_DIEMORFLT;
  VPSS_REG_S *pstReg;

  pstReg = (VPSS_REG_S*)pu32AppAddr;

  VPSS_DIEMORFLT.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMORFLT.u32));
  VPSS_DIEMORFLT.bits.med_blend_en = bEnBlend;
  VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMORFLT.u32), VPSS_DIEMORFLT.u32);

  return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetHisPreEn(HI_U32 *pu32AppAddr,HI_BOOL bEnPre)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.pre_info_en = bEnPre;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 *pu32AppAddr,HI_BOOL bEnPpre)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.ppre_info_en = bEnPpre;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetCombLimit(HI_U32 *pu32AppAddr,HI_S32  s32UpLimit,HI_S32  s32LowLimit)
{
    U_VPSS_DIECOMBCHK0 VPSS_DIECOMBCHK0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECOMBCHK0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK0.u32));
    VPSS_DIECOMBCHK0.bits.comb_chk_lower_limit = s32LowLimit;
    VPSS_DIECOMBCHK0.bits.comb_chk_upper_limit = s32UpLimit;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK0.u32), VPSS_DIECOMBCHK0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombThd(HI_U32 *pu32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd)
{
    U_VPSS_DIECOMBCHK0 VPSS_DIECOMBCHK0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECOMBCHK0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK0.u32));
    VPSS_DIECOMBCHK0.bits.comb_chk_min_hthd = s32Hthd;
    VPSS_DIECOMBCHK0.bits.comb_chk_min_vthd = s32Vthd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK0.u32), VPSS_DIECOMBCHK0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombEn(HI_U32 *pu32AppAddr,HI_BOOL bEnComb)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_en = bEnComb;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombMdThd(HI_U32 *pu32AppAddr,HI_S32 s32MdThd)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_md_thd = s32MdThd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 *pu32AppAddr,HI_S32 s32EdgeThd)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_edge_thd = s32EdgeThd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetStWrAddr(HI_U32 *pu32AppAddr,HI_U32 u32Addr)
{
    HI_U32 VPSS_STWADDR;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_STWADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_STWADDR));
    VPSS_STWADDR = u32Addr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_STWADDR), VPSS_STWADDR);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStRdAddr(HI_U32 *pu32AppAddr,HI_U32 u32Addr)
{
    HI_U32 VPSS_STRADDR;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_STRADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_STRADDR));
    VPSS_STRADDR = u32Addr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_STRADDR), VPSS_STRADDR);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetStStride(HI_U32 *pu32AppAddr,HI_U32 u32Stride)
{
    U_VPSS_STSTRIDE VPSS_STSTRIDE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_STSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_STSTRIDE.u32));
    VPSS_STSTRIDE.bits.st_stride = u32Stride;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_STSTRIDE.u32), VPSS_STSTRIDE.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 *pu32AppAddr,HI_U32 u32ParaPhyAddr)
{
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DEI_ADDR), u32ParaPhyAddr); 
   
    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetDBMAddr(HI_U32 *pu32AppAddr,HI_U32 u32PhyAddr)
{
   
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DBM_ADDR),u32PhyAddr); 
   
    return HI_SUCCESS;
}                 




HI_S32 VPSS_REG_SetOutstanding(HI_U32 *pu32AppAddr,HI_U32 ROuts, HI_U32 WOuts)
{
    U_VPSS_MISCELLANEOUS VPSS_MISCELLANEOUS;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    

    VPSS_MISCELLANEOUS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32));

    VPSS_MISCELLANEOUS.bits.routstanding = ROuts;    
    VPSS_MISCELLANEOUS.bits.woutstanding = WOuts;    
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32), VPSS_MISCELLANEOUS.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetAxiID(HI_U32 *pu32AppAddr,REG_AXI_CHN_E eChn, HI_U32 u32AxiID)
{
    return HI_SUCCESS;
}



/**FMD */
HI_S32 VPSS_REG_SetDeiEdgeSmoothRatio(HI_U32 *pu32AppAddr,HI_S8 u8Data)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.edge_smooth_ratio = u8Data;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32); 

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDeiStillBlkThd(HI_U32 *pu32AppAddr,HI_S8 u8Data)
{
    U_VPSS_PDBLKTHD VPSS_PDBLKTHD;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDBLKTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDBLKTHD.u32));
    VPSS_PDBLKTHD.bits.diff_movblk_thd = u8Data;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDBLKTHD.u32), VPSS_PDBLKTHD.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiHistThd(HI_U32 *pu32AppAddr,HI_S8 *pu8Data)
{
    U_VPSS_PDPHISTTHD1  VPSS_PDPHISTTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_PDPHISTTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPHISTTHD1.u32));
    VPSS_PDPHISTTHD1.bits.hist_thd0 = pu8Data[0];
    VPSS_PDPHISTTHD1.bits.hist_thd1 = pu8Data[1];
    VPSS_PDPHISTTHD1.bits.hist_thd2 = pu8Data[2];
    VPSS_PDPHISTTHD1.bits.hist_thd3 = pu8Data[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDPHISTTHD1.u32), VPSS_PDPHISTTHD1.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiUmThd(HI_U32 *pu32AppAddr,HI_S8 *pu8Data)
{
    U_VPSS_PDUMTHD VPSS_PDUMTHD;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDUMTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDUMTHD.u32));
    VPSS_PDUMTHD.bits.um_thd0 = pu8Data[0];
    VPSS_PDUMTHD.bits.um_thd1 = pu8Data[1];
    VPSS_PDUMTHD.bits.um_thd2 = pu8Data[2];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDUMTHD.u32), VPSS_PDUMTHD.u32); 
    
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDeiCoring(HI_U32 *pu32AppAddr,HI_S8 s8CorBlk,HI_S8 s8CorNorm,HI_S8 s8CorTkr)
{
    U_VPSS_PDPCCCORING VPSS_PDPCCCORING;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDPCCCORING.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCCORING.u32));
    VPSS_PDPCCCORING.bits.coring_blk = s8CorBlk;
    VPSS_PDPCCCORING.bits.coring_norm = s8CorNorm;
    VPSS_PDPCCCORING.bits.coring_tkr = s8CorTkr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDPCCCORING.u32), VPSS_PDPCCCORING.u32); 
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiMovCoring(HI_U32 *pu32AppAddr,HI_S8 s8Blk,HI_S8 s8Norm,HI_S8 s8Tkr)
{
    U_VPSS_PDPCCMOV VPSS_PDPCCMOV;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDPCCMOV.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCMOV.u32));
    VPSS_PDPCCMOV.bits.mov_coring_blk = s8Blk;
    VPSS_PDPCCMOV.bits.mov_coring_norm = s8Norm;
    VPSS_PDPCCMOV.bits.mov_coring_tkr = s8Tkr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDPCCMOV.u32), VPSS_PDPCCMOV.u32);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDeiPccHThd(HI_U32 *pu32AppAddr,HI_S8 s8Data)
{
    U_VPSS_PDPCCHTHD VPSS_PDPCCHTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDPCCHTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCHTHD.u32));
    VPSS_PDPCCHTHD.bits.pcc_hthd = s8Data;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDPCCHTHD.u32), VPSS_PDPCCHTHD.u32); 
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiPccVThd(HI_U32 *pu32AppAddr,HI_S8 *ps8Data)
{
    U_VPSS_PDPCCVTHD VPSS_PDPCCVTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDPCCVTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCVTHD.u32));
    VPSS_PDPCCVTHD.bits.pcc_vthd0 = ps8Data[0];
    VPSS_PDPCCVTHD.bits.pcc_vthd1 = ps8Data[1];
    VPSS_PDPCCVTHD.bits.pcc_vthd2 = ps8Data[2];
    VPSS_PDPCCVTHD.bits.pcc_vthd3 = ps8Data[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDPCCVTHD.u32), VPSS_PDPCCVTHD.u32); 
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiItDiff(HI_U32 *pu32AppAddr,HI_S8 *ps8Data)
{
    U_VPSS_PDITDIFFVTHD VPSS_PDITDIFFVTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDITDIFFVTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDITDIFFVTHD.u32));
    VPSS_PDITDIFFVTHD.bits.itdiff_vthd0 = ps8Data[0];
    VPSS_PDITDIFFVTHD.bits.itdiff_vthd1 = ps8Data[1];
    VPSS_PDITDIFFVTHD.bits.itdiff_vthd2 = ps8Data[2];
    VPSS_PDITDIFFVTHD.bits.itdiff_vthd3 = ps8Data[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDITDIFFVTHD.u32), VPSS_PDITDIFFVTHD.u32); 
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiLasiCtrl(HI_U32 *pu32AppAddr,HI_S8 s8Thr,HI_S8 s8EdgeThr,HI_S8 s8lasiThr)
{
//modify by zz
    U_VPSS_PDLASITHD VPSS_PDLASITHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDLASITHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASITHD.u32));
    VPSS_PDLASITHD.bits.lasi_mov_thd = s8lasiThr;
    VPSS_PDLASITHD.bits.lasi_edge_thd = s8EdgeThr;
    VPSS_PDLASITHD.bits.lasi_coring_thd = s8Thr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDLASITHD.u32),VPSS_PDLASITHD.u32);
    
    return HI_SUCCESS; 
}

HI_S32 VPSS_REG_SetDeiPdBitMove(HI_U32 *pu32AppAddr,HI_S32  s32Data)
{
    U_VPSS_PDCTRL VPSS_PDCTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_PDCTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDCTRL.u32));
    VPSS_PDCTRL.bits.bitsmov2r = s32Data + 6;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDCTRL.u32),VPSS_PDCTRL.u32); 
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiDirMch(HI_U32 *pu32AppAddr,HI_BOOL  bNext)
{
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetEdgeSmooth(HI_U32 *pu32AppAddr,HI_BOOL  bEdgeSmooth)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.edge_smooth_en = bEdgeSmooth;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32),VPSS_DIECTRL.u32);
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetPdIchd(HI_U32 *pu32AppAddr,HI_S32 *pstData)
{
    *pstData = 0;
    return HI_SUCCESS;
}


/*************************************************************************************************/


HI_S32 VPSS_REG_SetUVConvertEn(HI_U32 *pu32AppAddr,HI_U32 u32EnUV)
{
   
    U_VPSS_CTRL2 VPSS_CTRL2;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    VPSS_CTRL2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32));
    VPSS_CTRL2.bits.uv_invert = u32EnUV;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32); 
    
    return HI_SUCCESS; 
}

HI_S32 VPSS_REG_SetDbmEn(HI_U32 *pu32AppAddr,HI_BOOL  bEnDBM)
{
	U_VPSS_CTRL VPSS_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.dbm_en = bEnDBM;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDbmOutMode(HI_U32 *pu32AppAddr,HI_U32  u32Mode)
{
	U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
    VPSS_DB_CTRL.bits.dbm_out_mode = u32Mode;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDsEn(HI_U32 *pu32AppAddr,HI_BOOL  bEnDS)
{
	U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
    VPSS_DB_CTRL.bits.ds_en = bEnDS;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDrEn(HI_U32 *pu32AppAddr,HI_BOOL  bEnDR)
{
    U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
    VPSS_DB_CTRL.bits.dr_en = bEnDR;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 

    return HI_SUCCESS;
}
  

HI_S32 VPSS_REG_SetDmEn(HI_U32 *pu32AppAddr,HI_BOOL  bEnDm)
{
    U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
    VPSS_DB_CTRL.bits.dm_en = bEnDm;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 

    return HI_SUCCESS;
}
    

HI_S32 VPSS_REG_SetDbmDemoEn(HI_U32 *pu32AppAddr,HI_BOOL  bEnDBM)
{
    U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
    VPSS_DB_CTRL.bits.dbm_demo_en = bEnDBM;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDbDetectEn(HI_U32 *pu32AppAddr,REG_DBDET_MODE_E eDbDetMode,HI_BOOL  bEnDbDetect)
{

    U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(eDbDetMode)
    {
/*
        case REG_DBDET_MODE_SIZE:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.det_size_en = bEnDbDetect;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
            */
        case REG_DBDET_MODE_HY:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.det_hy_en = bEnDbDetect;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
        case REG_DBDET_MODE_HC:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.det_hc_en = bEnDbDetect;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
        case REG_DBDET_MODE_VY:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.det_vy_en = bEnDbDetect;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
        default:
            VPSS_FATAL("ERROR CFG --> eDbDetMode : %d, bEnDRDetect: %d!\n",eDbDetMode,bEnDbDetect);    
    }

    
    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetDbEn(HI_U32 *pu32AppAddr,REG_DB_MODE_E eDbMode,HI_BOOL  bEnDb)
{

    U_VPSS_DB_CTRL VPSS_DB_CTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(eDbMode)
    {
/*        case REG_DB_MODE_VC:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.db_chr_ver_en = bEnDb;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;*/
        case REG_DB_MODE_HY:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.db_lum_hor_en = bEnDb;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
/*        case REG_DB_MODE_HC:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.db_chr_hor_en = bEnDb;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;*/
        case REG_DB_MODE_VY:
            VPSS_DB_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32));
            VPSS_DB_CTRL.bits.db_lum_ver_en = bEnDb;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DB_CTRL.u32), VPSS_DB_CTRL.u32); 
            break;
        default:
            VPSS_FATAL("ERROR CFG --> eDbDetMode : %d, bEnDRDetect: %d!\n",eDbMode,bEnDb);    
    }

    
    return HI_SUCCESS;

}


HI_S32 VPSS_REG_SetLBAEn(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnLba)
{
    U_VPSS_CTRL3 VPSS_CTRL3;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_CTRL3.bits.vhd0_lba_en = bEnLba;
            break;
        default:
            VPSS_FATAL("\nReg Error\n");
            return HI_FAILURE;
    }
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32);
	

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetLBABg(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Color,HI_U32 u32Alpha)
{
    U_VPSS_VHD0LBA_BK VPSS_VHD0LBA_BK;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0LBA_BK.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_BK.u32));
            VPSS_VHD0LBA_BK.bits.vhd0_vbk_y = (u32Color & 0x3FF00000) >> 20;
            VPSS_VHD0LBA_BK.bits.vhd0_vbk_cb = (u32Color & 0xFFC00) >> 10;
            VPSS_VHD0LBA_BK.bits.vhd0_vbk_cr = u32Color & 0x3ff;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_BK.u32), VPSS_VHD0LBA_BK.u32);
            break;

        default:
            VPSS_FATAL("Reg Error\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetLbaDispSize(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    VPSS_REG_S *pstReg;

    U_VPSS_VHD0LBA_DSIZE VPSS_VHD0LBA_DSIZE;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0LBA_DSIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_DSIZE.u32));

            VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_width = u32Width -1;
            VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_height = u32Height -1;
                
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_DSIZE.u32), VPSS_VHD0LBA_DSIZE.u32);
            
            break; //lzx

        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
    
}
HI_S32 VPSS_REG_SetLbaVfPos(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos)
{
    U_VPSS_VHD0LBA_VFPOS VPSS_VHD0LBA_VFPOS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0LBA_VFPOS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_VFPOS.u32));
            
            VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_xfpos = u32XFPos;
            VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_yfpos = u32YFPos;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_VFPOS.u32), VPSS_VHD0LBA_VFPOS.u32);
            break;

        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
    
}

HI_S32 VPSS_REG_SetLBADispPos(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width)
{
    VPSS_REG_S *pstReg;
    
    U_VPSS_VHD0LBA_DSIZE VPSS_VHD0LBA_DSIZE;

    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0LBA_DSIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_DSIZE.u32));
            
            VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_width = u32Width - 1;
            VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_height = u32Height - 1;
           
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_DSIZE.u32), VPSS_VHD0LBA_DSIZE.u32);
            
            break;
        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
    
}
HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VHD0LBA_VFPOS VPSS_VHD0LBA_VFPOS;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0LBA_VFPOS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_VFPOS.u32));
            
            VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_xfpos = u32XFPos;
            VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_yfpos = u32YFPos;
            //VPSS_VHDLBA_VSIZE.bits.video_width =  u32Width -1;
            //VPSS_VHDLBA_VSIZE.bits.video_height = u32Height - 1;

            
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0LBA_VFPOS.u32), VPSS_VHD0LBA_VFPOS.u32);
            //VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHDLBA_VSIZE.u32), VPSS_VHDLBA_VSIZE.u32);
            break;
        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
    
}

HI_S32 VPSS_REG_SetOutCropVidPos(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,
                            HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VHD0CROP_POS VPSS_VHD0CROP_POS;
    U_VPSS_VHD0CROP_SIZE  VPSS_VHD0CROP_SIZE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0CROP_POS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_POS.u32));
            VPSS_VHD0CROP_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_SIZE.u32));
            
            VPSS_VHD0CROP_POS.bits.vhd0_crop_x = u32XFPos;
            VPSS_VHD0CROP_POS.bits.vhd0_crop_y = u32YFPos;
            VPSS_VHD0CROP_SIZE.bits.vhd0_crop_width =  u32Width -1;
            VPSS_VHD0CROP_SIZE.bits.vhd0_crop_height = u32Height - 1;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_POS.u32), VPSS_VHD0CROP_POS.u32);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_SIZE.u32), VPSS_VHD0CROP_SIZE.u32);
            break;

        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
    
}                            

HI_S32 VPSS_REG_SetTunlEn(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnTunl)
{
    U_VPSS_TUNL_CTRL0  VPSS_TUNL_CTRL0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_TUNL_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32));
            VPSS_TUNL_CTRL0.bits.vhd0_tunl_en = bEnTunl;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32), VPSS_TUNL_CTRL0.u32); 
            break;

       default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetTunlFinishLine(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32FinishLine)
{
    U_VPSS_TUNL_CTRL0  VPSS_TUNL_CTRL0;
    //U_VPSS_TUNL_CTRL1  VPSS_TUNL_CTRL1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_TUNL_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32));
            VPSS_TUNL_CTRL0.bits.vhd0_tunl_finish_line = s32FinishLine;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32), VPSS_TUNL_CTRL0.u32); 
            break;

       default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetTunlMode(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort, REG_TUNLPOS_E  s32TunlMode)
{
    U_VPSS_TUNL_CTRL1  VPSS_TUNL_CTRL1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {  
        case VPSS_REG_HD:
            VPSS_TUNL_CTRL1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL1.u32));
            VPSS_TUNL_CTRL1.bits.vhd0_tunl_mode = s32TunlMode;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL1.u32), VPSS_TUNL_CTRL1.u32); 
            break;

       default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetTunlAddr(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32TunlAddr)
{
    HI_U32  VPSS_VHD_TUNL_ADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;
    
    switch(ePort)
    {  
        case VPSS_REG_HD:
            VPSS_VHD_TUNL_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0_TUNL_ADDR));
            VPSS_VHD_TUNL_ADDR = u32TunlAddr;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0_TUNL_ADDR), VPSS_VHD_TUNL_ADDR); 
            break;
       default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetAddr(HI_U32 *pu32AppAddr,VPSS_REG_ADDR_E eType,HI_U32 u32Addr)
{
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

	switch(eType)
	{

		case REG_VPSS_ZME_ADDR:
			VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_ZME_ADDR), u32Addr);
			break;
			
		case REG_VPSS_DEI_ADDR:
			VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DEI_ADDR), u32Addr);
			break;
			
		case REG_VPSS_CHK_SUM_W_ADDR:
			VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_W_ADDR), u32Addr);
			break;
			
		case REG_VPSS_PNEXT_ADDR:
			VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PNEXT), u32Addr);
			break;   

		default:
			VPSS_FATAL("un-know VPSS_REG_ADDR_E!\n");
			break;
	}
	
    return HI_SUCCESS;
} 

HI_S32 VPSS_REG_SetCurTunlAddr(HI_U32 *pu32AppAddr,HI_U32 u32TunlAddr)
{
    HI_U32  VPSS_CUR_TUNL_ADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CUR_TUNL_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CUR_TUNL_ADDR));
    VPSS_CUR_TUNL_ADDR = u32TunlAddr;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUR_TUNL_ADDR), VPSS_CUR_TUNL_ADDR);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetCurTunlEn(HI_U32 *pu32AppAddr,HI_BOOL u32CurTunlEn)
{
    U_VPSS_TUNL_CTRL0  VPSS_TUNL_CTRL0;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_TUNL_CTRL0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32));
    VPSS_TUNL_CTRL0.bits.cur_tunl_en = u32CurTunlEn;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL0.u32), VPSS_TUNL_CTRL0.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetCurTunlInterval(HI_U32 *pu32AppAddr,REG_FRAMEPOS_E ePort,HI_S32  s32CurTunlInterval)
{
    U_VPSS_TUNL_CTRL1  VPSS_TUNL_CTRL1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    /*
        s32CurTunlInterval meas every s32CurTunlInterval  * cycle *1000 to read CUR_TUNL_ADDR
    */
    switch(ePort)
    {  
        case CUR_FRAME:
            VPSS_TUNL_CTRL1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL1.u32));
            VPSS_TUNL_CTRL1.bits.cur_tunl_rd_interval = s32CurTunlInterval;    
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TUNL_CTRL1.u32), VPSS_TUNL_CTRL1.u32); 
            break;
       default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetInCropPos(HI_U32 *pu32AppAddr,HI_U32 u32InCropY,HI_U32 u32InCropX)
{
    U_VPSS_INCROP_POS VPSS_INCROP_POS;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_INCROP_POS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_INCROP_POS.u32));
    VPSS_INCROP_POS.bits.in_crop_y = u32InCropY;
    VPSS_INCROP_POS.bits.in_crop_x = u32InCropX;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INCROP_POS.u32), VPSS_INCROP_POS.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetInCropEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL3 VPSS_CTRL3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.in_crop_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetInCropMode(HI_U32 *pu32AppAddr,HI_BOOL bInCropMode)
{
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetInCropSize(HI_U32 *pu32AppAddr,HI_U32 u32InCropHeight,HI_U32 u32InCropWidth)
{
    U_VPSS_INCROP_SIZE VPSS_INCROP_SIZE;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_INCROP_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_INCROP_SIZE.u32));
    VPSS_INCROP_SIZE.bits.in_crop_height = u32InCropHeight -1;
    VPSS_INCROP_SIZE.bits.in_crop_width  = u32InCropWidth -1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INCROP_SIZE.u32), VPSS_INCROP_SIZE.u32); 

    return HI_SUCCESS;
}


/****PORT CROP*****/

HI_S32 VPSS_REG_SetPortCropPos(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropY,HI_U32 u32CropX)
{

    U_VPSS_VHD0CROP_POS VPSS_VHD0CROP_POS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {

        case VPSS_REG_HD:
            VPSS_VHD0CROP_POS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_POS.u32));
            VPSS_VHD0CROP_POS.bits.vhd0_crop_y = u32CropY;
            VPSS_VHD0CROP_POS.bits.vhd0_crop_x = u32CropX;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_POS.u32), VPSS_VHD0CROP_POS.u32); 
            break;

        default:
            VPSS_FATAL("\nReg Error\n");
    } 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPortCropSize(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropHeight,HI_U32 u32CropWidth)
{
    U_VPSS_VHD0CROP_SIZE VPSS_VHD0CROP_SIZE;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0CROP_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_SIZE.u32));
            VPSS_VHD0CROP_SIZE.bits.vhd0_crop_height  = u32CropHeight - 1;
            VPSS_VHD0CROP_SIZE.bits.vhd0_crop_width = u32CropWidth - 1;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CROP_SIZE.u32), VPSS_VHD0CROP_SIZE.u32); 
            break;
        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetPortCropEn(HI_U32 *pu32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL3 VPSS_CTRL3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.vhd0_crop_en = bEnable;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetPortMirrorEn(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bMirrorEn)
{
	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;
	VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
            VPSS_VHD0CTRL.bits.vhd0_mirror = bMirrorEn;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 
            break;
        default:
            VPSS_FATAL("Reg Error\n");
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPortFlipEn(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bFlipEn)
{
	
	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

	VPSS_REG_S *pstReg;
	
	pstReg = (VPSS_REG_S*)pu32AppAddr;

	switch(ePort)
	{
		case VPSS_REG_HD:
			VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD0CTRL.u32));
			VPSS_VHD0CTRL.bits.vhd0_flip = bFlipEn;
			VPSS_REG_RegWrite(&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 
			break;
		default:
			VPSS_FATAL("\nReg Error\n");
	}
	return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPreVfirEn(HI_U32 *pu32AppAddr,HI_BOOL bVfirEn)
{
    
	U_VPSS_CTRL3 VPSS_CTRL3;
	VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.vhd0_pre_vfir_en = bVfirEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPreVfirMode(HI_U32 *pu32AppAddr,HI_U32 u32VfirMode)
{
	U_VPSS_CTRL3 VPSS_CTRL3;
	VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
    VPSS_CTRL3.bits.vhd0_pre_vfir_mode = u32VfirMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetRotation(HI_U32 *pu32AppAddr,HI_U32 u32Angle, HI_U32 u32ProMode)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.rotate_en = 0x1;
    VPSS_CTRL.bits.rotate_angle = u32Angle;
    VPSS_CTRL.bits.img_pro_mode = u32ProMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetPortUVInvert(HI_U32 *pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bUVInvert)
{

	U_VPSS_VHD0CTRL VPSS_VHD0CTRL;

	VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD0CTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32));
            VPSS_VHD0CTRL.bits.vhd0_uv_invert = bUVInvert;
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_VHD0CTRL.u32), VPSS_VHD0CTRL.u32); 
            break;
        default:
            VPSS_FATAL("\nReg Error\n");
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_GetPFCNT(HI_U32 *pu32AppAddr,HI_U32 *pu32Int)
{
    HI_U32  VPSS_PFCNT;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;
    
    VPSS_PFCNT = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RAWINT.u32));

    *pu32Int = VPSS_PFCNT;

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetFidelity(HI_U32 * pu32AppAddr,HI_BOOL bEnFidelity)
{
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetStMode(HI_U32 * pu32AppAddr,HI_BOOL bLumaMax,HI_BOOL bChromaMax)
{
    VPSS_REG_S *pstReg;
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    
    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.chroma_mf_max = bChromaMax;    
    VPSS_DIELMA2.bits.luma_mf_max = bLumaMax;    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetPreZme(HI_U32 * pu32AppAddr,
                            VPSS_REG_PREZME_E enHor,VPSS_REG_PREZME_E enVer)
{
    U_VPSS_CTRL3 VPSS_CTRL3;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;
    VPSS_CTRL3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32));
/*
    switch(enHor)
    {
        case PREZME_DISABLE:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x0;
            break;
        case PREZME_2X:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x1;
            VPSS_CTRL.bits.vhd0_pre_hfir_mode = 0x0;
            break;
        case PREZME_4X:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x1;
            VPSS_CTRL.bits.vhd0_pre_hfir_mode = 0x1;
            break;
        case PREZME_8X:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x1;
            VPSS_CTRL.bits.vhd0_pre_hfir_mode = 0x2;
            break;
        case PREZME_BUTT:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x0;
            break;
         default:
            VPSS_CTRL.bits.vhd0_pre_hfir_en = 0x0;
            break;
    }
*/
    switch(enVer)
    {
        case PREZME_DISABLE:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x0;
            break;
        case PREZME_2X:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x1;
            VPSS_CTRL3.bits.vhd0_pre_vfir_mode = 0x0;
            break;
        case PREZME_4X:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x1;
            VPSS_CTRL3.bits.vhd0_pre_vfir_mode = 0x1;
            break;
        case PREZME_8X:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x1;
            VPSS_CTRL3.bits.vhd0_pre_vfir_mode = 0x2;
            break;
        case PREZME_VFIELD:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x1;
            VPSS_CTRL3.bits.vhd0_pre_vfir_mode = 0x3;
            break;
        case PREZME_BUTT:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x0;
            break;
         default:
            VPSS_CTRL3.bits.vhd0_pre_vfir_en = 0x0;
            break;
    }
    
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CTRL3.u32), VPSS_CTRL3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetTnrAddr(HI_U32 *pu32AppAddr,HI_U32 u32TNRAddr,HI_U32 u32CLUTAddr)
{
    HI_U32  VPSS_TNR_ADDR;
    HI_U32  VPSS_TNR_CLUT_ADDR;

    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S*)pu32AppAddr;
     
    VPSS_TNR_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TNR_ADDR));
    VPSS_TNR_ADDR = u32TNRAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TNR_ADDR), VPSS_TNR_ADDR);

    VPSS_TNR_CLUT_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TNR_CLUT_ADDR));
    VPSS_TNR_CLUT_ADDR = u32CLUTAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TNR_CLUT_ADDR), VPSS_TNR_CLUT_ADDR);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetSnrAddr(HI_U32 *pu32AppAddr,HI_U32 u32SNRAddr)
{
    HI_U32  VPSS_SNR_ADDR;

    VPSS_REG_S *pstReg;
    pstReg = (VPSS_REG_S*)pu32AppAddr;
     
    VPSS_SNR_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SNR_ADDR));
    VPSS_SNR_ADDR = u32SNRAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SNR_ADDR), VPSS_SNR_ADDR);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMktCoordinate(HI_U32 *pu32AppAddr,HI_U32 u32MktCoordinate)
{
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDbmDemoXPos(HI_U32 *pu32AppAddr,HI_U32 u32Xpos)
{
    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetTnr(HI_U32 * pu32AppAddr,HI_BOOL bDemoEn)
{
    return HI_SUCCESS;
}


//======================READ STT VALUE=================================

HI_S32 VPSS_REG_AppSttRegInit(S_STT_REGS_TYPE **ppstAppReg,HI_U32* pu32VirAddr)
{
    *ppstAppReg = (S_STT_REGS_TYPE * )pu32VirAddr;

    return HI_SUCCESS;
}

HI_VOID VPSS_REG_GetDetPixel(HI_U32 *pu32AppAddr,HI_U32 BlkNum, HI_U8* pstData)
{

    HI_U32  pixdata;
    S_STT_REGS_TYPE *pstReg;
    
    pstReg = (S_STT_REGS_TYPE*)pu32AppAddr;
    
    if (BlkNum > 5)
    {
        VPSS_FATAL("Error! Vou_GetDetPixel() Selected Wrong BLKNUM!\n");
        return ;
    }

    pixdata = VPSS_REG_RegRead((&(pstReg->STR_DET_VIDBLK0TOL0.u32) + BlkNum * 2 ));

    pstData[0] = (pixdata & 0xff);
    pstData[1] = (pixdata & 0xff00) >> 8;
    pstData[2] = (pixdata & 0xff0000) >> 16;
    pstData[3] = (pixdata & 0xff000000) >> 24;
    pixdata = VPSS_REG_RegRead((&(pstReg->STR_DET_VIDBLK0TOL0.u32)) + (BlkNum * 2  + 1) );

    pstData[4] = (pixdata & 0xff);
    pstData[5] = (pixdata & 0xff00) >> 8;
    pstData[6] = (pixdata & 0xff0000) >> 16;
    pstData[7] = (pixdata & 0xff000000) >> 24;

    return ;


}
/********************************/


HI_S32 VPSS_REG_GetHisBin(HI_U32 *pu32AppAddr,HI_S32 *pstData)
{   
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    pstData[0] = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDHISTBIN[0]));
    pstData[1] = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDHISTBIN[1]));
    pstData[2] = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDHISTBIN[2]));
    pstData[3] = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDHISTBIN[3]));
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetItDiff(HI_U32 *pu32AppAddr,HI_S32 *pstData)
{
    HI_S32 VPSS_PDFRMITDIFF;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    VPSS_PDFRMITDIFF = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDFRMITDIFF));

    *pstData = VPSS_PDFRMITDIFF;

    return HI_SUCCESS;
}


//TODO!!!  这里只有区域0 的，没有区域1 的
HI_S32 VPSS_REG_GetPccData(HI_U32 *pu32AppAddr,HI_S32 *ps32FFWD,
                             HI_S32 *ps32FWD,HI_S32 *ps32BWD,HI_S32 *ps32CRSS,
                             /*HI_S32 *ps32PW,*/HI_S32 *ps32FWDTKR,HI_S32 *ps32WDTKR)
{
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE*)pu32AppAddr;

    *ps32FFWD = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCFFWD0));
    *ps32FWD = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCFWD0));
    *ps32BWD = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCBWD0));
    *ps32CRSS = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCCRSS0));
    *ps32FWDTKR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCFWDTKR0));
    *ps32WDTKR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDPCCBWDTKR0));

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_GetPdMatch(HI_U32 *pu32AppAddr,
                            HI_S32 *ps32Match0,HI_S32 *ps32UnMatch0,
                            HI_S32 *ps32Match1,HI_S32 *ps32UnMatch1)
{
    HI_S32 VPSS_PDUMMATCH0;
    HI_S32 VPSS_PDUMNOMATCH0;
    HI_S32 VPSS_PDUMMATCH1;
    HI_S32 VPSS_PDUMNOMATCH1;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;

    
    VPSS_PDUMMATCH0 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDUMMATCH0));
    VPSS_PDUMNOMATCH0 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDUMNOMATCH0));
    VPSS_PDUMMATCH1 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDUMMATCH1));
    VPSS_PDUMNOMATCH1 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDUMNOMATCH1));

    *ps32Match0 = VPSS_PDUMMATCH0;
    *ps32UnMatch0 = VPSS_PDUMNOMATCH0;
    *ps32Match1 = VPSS_PDUMMATCH1;
    *ps32UnMatch1 = VPSS_PDUMNOMATCH1;
    return HI_SUCCESS;
}                       


//TODO!!!  这里只有区域0 的，没有区域1 的
HI_S32 VPSS_REG_GetLasiCnt(HI_U32 *pu32AppAddr,
                            HI_S32 *ps32Cnt14,HI_S32 *ps32Cnt32,
                            HI_S32 *ps32Cnt34)
{
    HI_S32 VPSS_PDLASICNT14;
    HI_S32 VPSS_PDLASICNT32;
    HI_S32 VPSS_PDLASICNT34;
    
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;

    VPSS_PDLASICNT14 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASICNT140));
    VPSS_PDLASICNT32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASICNT320));
    VPSS_PDLASICNT34 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASICNT340));

    *ps32Cnt14 = VPSS_PDLASICNT14;
    *ps32Cnt32 = VPSS_PDLASICNT32;
    *ps32Cnt34 = VPSS_PDLASICNT34;

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_GetBlkSad(HI_U32 *pu32AppAddr,HI_S32 *pstData)
{
    HI_S32  VPSS_PDSTLBLKSAD[16];
    HI_U32 u32Count;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE*)pu32AppAddr;

    for(u32Count = 0; u32Count < 16; u32Count ++)
    {
        VPSS_PDSTLBLKSAD[u32Count] = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDSTLBLKSAD[u32Count]));
        pstData[u32Count] = VPSS_PDSTLBLKSAD[u32Count];
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbHyCnt(HI_U32 *pu32AppAddr,HI_U32 *pu32DbHyCnt)
{
    HI_U32 DB_HY_COUNTER;
    HI_U32 u32Count;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    for(u32Count = 0; u32Count < 1920; u32Count ++)
    {
        DB_HY_COUNTER = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_HY_COUNTER[u32Count]));
        pu32DbHyCnt[u32Count] = DB_HY_COUNTER;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbBorder(HI_U32 *pu32AppAddr,HI_U32 *pu32DbBorder)
{
    HI_U32 DB_BORDER;
    HI_U32 u32Count;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    for(u32Count = 0; u32Count < 128; u32Count ++)
    {
        DB_BORDER = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_BORDER[u32Count]));
        pu32DbBorder[u32Count] = DB_BORDER;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbHySizeHyCnt(HI_U32 *pu32AppAddr,HI_U32 *pu32DbHySizeHyCounter)
{
    U_DB_HY_SIZE DB_HY_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_HY_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_HY_SIZE.u32));
    
    *pu32DbHySizeHyCounter = DB_HY_SIZE.bits.hy_counter;

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbHySizeHySize(HI_U32 *pu32AppAddr,HI_U32 *pu32DbHySizeHySize)
{
    U_DB_HY_SIZE DB_HY_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_HY_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_HY_SIZE.u32));
    
    *pu32DbHySizeHySize = DB_HY_SIZE.bits.hy_size;

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_GetDbHcSizeHcCnt(HI_U32 *pu32AppAddr,HI_U32 *pu32DbHcSizeHcCounter)
{
    U_DB_HC_SIZE DB_HC_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_HC_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_HC_SIZE.u32));
    
    *pu32DbHcSizeHcCounter = DB_HC_SIZE.bits.hc_counter;

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbHcSizeHcSize(HI_U32 *pu32AppAddr,HI_U32 *pu32DbHcSizeHcSize)
{
    U_DB_HC_SIZE DB_HC_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_HC_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_HC_SIZE.u32));
    
    *pu32DbHcSizeHcSize = DB_HC_SIZE.bits.hc_size;

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbVySizeVyCnt(HI_U32 *pu32AppAddr,HI_U32 *pu32DbVySizeVyCounter)
{
    U_DB_VY_SIZE DB_VY_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_VY_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_VY_SIZE.u32));
    
    *pu32DbVySizeVyCounter = DB_VY_SIZE.bits.vy_counter;

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDbVySizeVySize(HI_U32 *pu32AppAddr,HI_U32 *pu32DbVySizeVySize)
{
    U_DB_VY_SIZE DB_VY_SIZE;
    S_STT_REGS_TYPE *pstReg;

    pstReg = (S_STT_REGS_TYPE *)pu32AppAddr;
    
    DB_VY_SIZE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->DB_VY_SIZE.u32));
    
    *pu32DbVySizeVySize = DB_VY_SIZE.bits.vy_size;

    return HI_SUCCESS;
}


/**************************** ADDR ********************************************/
HI_S32 VPSS_REG_SetReeAddr(HI_U32 *pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Caddr,HI_U32 u32YStr,HI_U32 u32CStr)
{
    U_VPSS_REESTRIDE VPSS_REESTRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REEYADDR), u32Yaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REECADDR), u32Caddr); 

    VPSS_REESTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_REESTRIDE.u32));
    VPSS_REESTRIDE.bits.reey_stride = u32YStr;
    VPSS_REESTRIDE.bits.reec_stride = u32YStr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_REESTRIDE.u32), VPSS_REESTRIDE.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetPrjCurAddr(HI_U32 *pu32AppAddr,HI_U32 u32Vaddr,HI_U32 u32Haddr,HI_U32 u32VStr,HI_U32 u32HStr)
{
    U_VPSS_PRJCURSTRIDE VPSS_PRJCURSTRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJVCURADDR), u32Vaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJHCURADDR), u32Haddr); 

    VPSS_PRJCURSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PRJCURSTRIDE.u32));
    VPSS_PRJCURSTRIDE.bits.prjh_cur_stride = u32HStr;
    VPSS_PRJCURSTRIDE.bits.prjv_cur_stride = u32VStr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJCURSTRIDE.u32), VPSS_PRJCURSTRIDE.u32); 

    return HI_SUCCESS;
}




HI_S32 VPSS_REG_SetRgmvAddr(HI_U32 *pu32AppAddr,HI_U32 u32Curaddr,HI_U32 u32Nx1addr,HI_U32 u32CurStr,HI_U32 u32Nx1Str)
{
    U_VPSS_RGMVSTRIDE VPSS_RGMVSTRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RGMVCURADDR), u32Curaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RGMVNX1ADDR), u32Nx1addr); 

    VPSS_RGMVSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RGMVSTRIDE.u32));
    VPSS_RGMVSTRIDE.bits.rgmv_cur_stride = u32CurStr;
    VPSS_RGMVSTRIDE.bits.rgmv_nx1_stride = u32Nx1Str;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RGMVSTRIDE.u32), VPSS_RGMVSTRIDE.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetBlkmvAddr(HI_U32 *pu32AppAddr,HI_U32 u32Curaddr,HI_U32 u32Refaddr,HI_U32 u32CurStr,HI_U32 u32RefStr)
{
    U_VPSS_BLKMVSTRIDE VPSS_BLKMVSTRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVCURADDR), u32Curaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVREFADDR), u32Refaddr); 

    VPSS_BLKMVSTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_BLKMVSTRIDE.u32));
    VPSS_BLKMVSTRIDE.bits.blkmv_cur_stride = u32CurStr;
    VPSS_BLKMVSTRIDE.bits.blkmv_ref_stride = u32RefStr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVSTRIDE.u32), VPSS_BLKMVSTRIDE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetCueAddr(HI_U32 *pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Caddr,HI_U32 u32YStr,HI_U32 u32CStr)
{
    U_VPSS_CUESTRIDE VPSS_CUESTRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUEYADDR), u32Yaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUECADDR), u32Caddr); 

    VPSS_CUESTRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CUESTRIDE.u32));
    VPSS_CUESTRIDE.bits.cuec_stride = u32CStr;
    VPSS_CUESTRIDE.bits.cuey_stride = u32YStr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CUESTRIDE.u32), VPSS_CUESTRIDE.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPrjNx2Addr(HI_U32 *pu32AppAddr,HI_U32 u32Vaddr,HI_U32 u32Haddr,HI_U32 u32VStr,HI_U32 u32HStr)
{
    U_VPSS_PRJNX2STRIDE VPSS_PRJNX2STRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJVNX2ADDR), u32Vaddr);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJHNX2ADDR), u32Haddr); 

    VPSS_PRJNX2STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PRJNX2STRIDE.u32));
    VPSS_PRJNX2STRIDE.bits.prjh_nx2_stride = u32HStr;
    VPSS_PRJNX2STRIDE.bits.prjv_nx2_stride = u32VStr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PRJNX2STRIDE.u32), VPSS_PRJNX2STRIDE.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetRgmvNx2Addr(HI_U32 *pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str)
{
//    HI_U32  VPSS_RGMVNX2ADDR;
    U_VPSS_RGMVNX2STRIDE VPSS_RGMVNX2STRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RGMVNX2ADDR), u32addr);

    VPSS_RGMVNX2STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_RGMVNX2STRIDE.u32));
    VPSS_RGMVNX2STRIDE.bits.rgmv_nx2_stride = u32Str;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_RGMVNX2STRIDE.u32), VPSS_RGMVNX2STRIDE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetBlkmvNx1Addr(HI_U32 *pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str)
{
//    HI_U32  VPSS_BLKMVNX1ADDR;
    U_VPSS_BLKMVNX1STRIDE VPSS_BLKMVNX1STRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX1ADDR), u32addr);

    VPSS_BLKMVNX1STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX1STRIDE.u32));
    VPSS_BLKMVNX1STRIDE.bits.blkmv_nx1_stride = u32Str;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX1STRIDE.u32), VPSS_BLKMVNX1STRIDE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetBlkmvNx2Addr(HI_U32 *pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str)
{
//    HI_U32  VPSS_BLKMVNX2ADDR;
    U_VPSS_BLKMVNX2STRIDE VPSS_BLKMVNX2STRIDE;

    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)pu32AppAddr;

    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX2ADDR), u32addr);

    VPSS_BLKMVNX2STRIDE.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX2STRIDE.u32));
    VPSS_BLKMVNX2STRIDE.bits.blkmv_nx2_stride = u32Str;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_BLKMVNX2STRIDE.u32), VPSS_BLKMVNX2STRIDE.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetSNRMadRaddr(HI_U32 *pu32AppAddr,HI_U32 u32MadRaddr)
{
    HI_U32  VPSS_SNR_MAD_RADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_SNR_MAD_RADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SNR_MAD_RADDR));
    VPSS_SNR_MAD_RADDR = u32MadRaddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SNR_MAD_RADDR), VPSS_SNR_MAD_RADDR); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetNx2Craddr(HI_U32 *pu32AppAddr,HI_U32 u32Craddr)
{
    HI_U32  VPSS_NXT2CRADDR;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_NXT2CRADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_NXT2CRADDR));
    VPSS_NXT2CRADDR = u32Craddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_NXT2CRADDR), VPSS_NXT2CRADDR); 

    return HI_SUCCESS;
}

/****************************TEST PATTERN********************************************/
HI_S32 VPSS_REG_SetTestPat(HI_U32 *pu32AppAddr,HI_U32 u32BkGrnd,HI_U32 u32PatDisWidth,HI_U32 u32PatAngle,HI_U32 u32TestPatEn)
{
    U_VPSS_TESTPATTERN VPSS_TESTPATTERN;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_TESTPATTERN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TESTPATTERN.u32));
    VPSS_TESTPATTERN.bits.pat_bkgrnd = u32BkGrnd;
    VPSS_TESTPATTERN.bits.pat_dis_width  = u32PatDisWidth;
    VPSS_TESTPATTERN.bits.pat_angle = u32BkGrnd;
    VPSS_TESTPATTERN.bits.test_pat_en = u32TestPatEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TESTPATTERN.u32), VPSS_TESTPATTERN.u32); 

    return HI_SUCCESS;
}


//TODO 
HI_S32 VPSS_REG_SetChkSumSel(HI_U32 *pu32AppAddr,HI_U32 u32BkGrnd,HI_U32 u32PatDisWidth,HI_U32 u32PatAngle,HI_U32 u32TestPatEn)
{
    U_VPSS_CHK_SUM_SEL VPSS_CHK_SUM_SEL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_CHK_SUM_SEL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32));
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_CHK_SUM_SEL.u32), VPSS_CHK_SUM_SEL.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetMacCfgUp(HI_U32 *pu32AppAddr,REG_CHN_ARBITRATE_E eChn)
{
    HI_U32  VPSS_MACCFG;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MACCFG = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MACCFG));
    VPSS_MACCFG |= (1 << eChn);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MACCFG), VPSS_MACCFG); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMacCfgDown(HI_U32 *pu32AppAddr,REG_CHN_ARBITRATE_E eChn)
{
    HI_U32 VPSS_MACCFG;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MACCFG = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MACCFG));
    VPSS_MACCFG &= ~(1 << eChn);
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MACCFG), VPSS_MACCFG); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieMcOnly(HI_U32 *pu32AppAddr,HI_BOOL bEn)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.mc_only = bEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieMaOnly(HI_U32 *pu32AppAddr,HI_BOOL bEn)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.ma_only = bEn;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieMcDir(HI_U32 *pu32AppAddr,HI_U32 u32LDir,HI_U32 u32CDir)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.mchdir_l = u32LDir;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32); 

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32));
    VPSS_DIECTRL.bits.mchdir_c = u32CDir;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_GetDieSTA(HI_U32 *pu32AppAddr,HI_U32 *pu32CHeightCnt,HI_U32 *pu32LHeightCnt,HI_U32 *pu32CState,HI_U32 *pu32State)
{
    U_VPSS_DIESTA VPSS_DIESTA;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIESTA.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIESTA.u32));
    *pu32CHeightCnt = VPSS_DIESTA.bits.c_height_cnt;
    *pu32LHeightCnt = VPSS_DIESTA.bits.l_height_cnt;
    *pu32CState = VPSS_DIESTA.bits.cur_cstate;
    *pu32State = VPSS_DIESTA.bits.cur_state;

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieLongMotionShf(HI_U32 *pu32AppAddr,HI_U32 u32Mode)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.long_motion_shf = u32Mode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieFldMotionShf(HI_U32 *pu32AppAddr,HI_U32 u32Mode)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_wnd_mode = u32Mode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieEdgeNorm(HI_U32 *pu32AppAddr,HI_U32 *u32Norm)
{
    U_VPSS_DIEEDGENORM0 VPSS_DIEEDGENORM0;
    U_VPSS_DIEEDGENORM1 VPSS_DIEEDGENORM1;
    U_VPSS_DIEEDGENORM2 VPSS_DIEEDGENORM2;
    U_VPSS_DIEEDGENORM3 VPSS_DIEEDGENORM3;
    U_VPSS_DIEEDGENORM4 VPSS_DIEEDGENORM4;
    U_VPSS_DIEEDGENORM5 VPSS_DIEEDGENORM5;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEEDGENORM0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM0.u32));
    VPSS_DIEEDGENORM0.bits.edge_norm_0 = u32Norm[0];
    VPSS_DIEEDGENORM0.bits.edge_norm_1 = u32Norm[1];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM0.u32), VPSS_DIEEDGENORM0.u32); 

    VPSS_DIEEDGENORM1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM1.u32));
    VPSS_DIEEDGENORM1.bits.edge_norm_2 = u32Norm[2];
    VPSS_DIEEDGENORM1.bits.edge_norm_3 = u32Norm[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM1.u32), VPSS_DIEEDGENORM1.u32); 

    VPSS_DIEEDGENORM2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM2.u32));
    VPSS_DIEEDGENORM2.bits.edge_norm_4 = u32Norm[4];
    VPSS_DIEEDGENORM2.bits.edge_norm_5 = u32Norm[5];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM2.u32), VPSS_DIEEDGENORM2.u32); 

    VPSS_DIEEDGENORM3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM3.u32));
    VPSS_DIEEDGENORM3.bits.edge_norm_6 = u32Norm[6];
    VPSS_DIEEDGENORM3.bits.edge_norm_7 = u32Norm[7];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM3.u32), VPSS_DIEEDGENORM3.u32); 

    VPSS_DIEEDGENORM4.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM4.u32));
    VPSS_DIEEDGENORM4.bits.edge_norm_8 = u32Norm[8];
    VPSS_DIEEDGENORM4.bits.edge_norm_9 = u32Norm[9];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM4.u32), VPSS_DIEEDGENORM4.u32); 

    VPSS_DIEEDGENORM5.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM5.u32));
    VPSS_DIEEDGENORM5.bits.edge_norm_10 = u32Norm[10];
    VPSS_DIEEDGENORM5.bits.edge_norm_11 = u32Norm[11];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM5.u32), VPSS_DIEEDGENORM5.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetMcStrength(HI_U32 *pu32AppAddr,HI_U32 u32k3,HI_U32 u32g3)
{
    U_VPSS_DIEEDGENORM2 VPSS_DIEEDGENORM2;
    U_VPSS_DIEEDGENORM3 VPSS_DIEEDGENORM3;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEEDGENORM2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM2.u32));
    VPSS_DIEEDGENORM2.bits.mc_strength_k3 = u32k3;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM2.u32), VPSS_DIEEDGENORM2.u32); 

    VPSS_DIEEDGENORM3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM3.u32));
    VPSS_DIEEDGENORM3.bits.mc_strength_g3 = u32g3;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM3.u32), VPSS_DIEEDGENORM3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetInterDiff(HI_U32 *pu32AppAddr,HI_U32 u32Thd0,HI_U32 u32Thd1,HI_U32 u32Thd2)
{
    U_VPSS_DIEEDGENORM4 VPSS_DIEEDGENORM4;
    U_VPSS_DIEEDGENORM5 VPSS_DIEEDGENORM5;
    U_VPSS_DIEEDGEFORMC VPSS_DIEEDGEFORMC;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEEDGENORM4.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM4.u32));
    VPSS_DIEEDGENORM4.bits.inter_diff_thd0 = u32Thd0;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM4.u32), VPSS_DIEEDGENORM4.u32); 

    VPSS_DIEEDGENORM5.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM5.u32));
    VPSS_DIEEDGENORM5.bits.inter_diff_thd1 = u32Thd1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGENORM5.u32), VPSS_DIEEDGENORM5.u32); 

    VPSS_DIEEDGEFORMC.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGEFORMC.u32));
    VPSS_DIEEDGEFORMC.bits.inter_diff_thd2 = u32Thd2;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGEFORMC.u32), VPSS_DIEEDGEFORMC.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetEdgeFormc(HI_U32 *pu32AppAddr,HI_U32 u32Scale,HI_U32 u32Coring)
{
    U_VPSS_DIEEDGEFORMC VPSS_DIEEDGEFORMC;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEEDGEFORMC.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEEDGEFORMC.u32));
    VPSS_DIEEDGEFORMC.bits.edge_scale = u32Scale;
    VPSS_DIEEDGEFORMC.bits.edge_coring = u32Coring;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEEDGEFORMC.u32), VPSS_DIEEDGEFORMC.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMcStrEngTh(HI_U32 *pu32AppAddr,HI_U32 *u32Gstr,HI_U32 *u32Kstr)
{
    U_VPSS_DIEMCSTRENGTH0 VPSS_DIEMCSTRENGTH0;
    U_VPSS_DIEMCSTRENGTH1 VPSS_DIEMCSTRENGTH1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEMCSTRENGTH0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH0.u32));
    VPSS_DIEMCSTRENGTH0.bits.mc_strength_g0 = u32Gstr[0];
    VPSS_DIEMCSTRENGTH0.bits.mc_strength_k0 = u32Kstr[0];
    VPSS_DIEMCSTRENGTH0.bits.mc_strength_k1 = u32Kstr[0];
    VPSS_DIEMCSTRENGTH0.bits.mc_strength_k2 = u32Kstr[0];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH0.u32), VPSS_DIEMCSTRENGTH0.u32); 

    VPSS_DIEMCSTRENGTH1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH1.u32));
    VPSS_DIEMCSTRENGTH1.bits.mc_strength_g1 = u32Gstr[1];
    VPSS_DIEMCSTRENGTH1.bits.mc_strength_g2 = u32Gstr[2];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH1.u32), VPSS_DIEMCSTRENGTH1.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetEdgeFormcMaxMin(HI_U32 *pu32AppAddr,HI_U32 u32Max,HI_U32 u32Min)
{
    U_VPSS_DIEMCSTRENGTH1 VPSS_DIEMCSTRENGTH1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEMCSTRENGTH1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH1.u32));
    VPSS_DIEMCSTRENGTH1.bits.mc_strength_maxg = u32Max;
    VPSS_DIEMCSTRENGTH1.bits.mc_strength_ming = u32Min;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMCSTRENGTH1.u32), VPSS_DIEMCSTRENGTH1.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieFusMcb(HI_U32 *pu32AppAddr,HI_U32 u32McbldY,HI_U32 u32McbldC)
{
    U_VPSS_DIEFUSION0 VPSS_DIEFUSION0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFUSION0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32));
    VPSS_DIEFUSION0.bits.k_y_mcbld = u32McbldY;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32), VPSS_DIEFUSION0.u32); 

    VPSS_DIEFUSION0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32));
    VPSS_DIEFUSION0.bits.k_c_mcbld = u32McbldC;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32), VPSS_DIEFUSION0.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieFusMcw(HI_U32 *pu32AppAddr,HI_U32 u32McwY,HI_U32 u32McwC)
{
    U_VPSS_DIEFUSION0 VPSS_DIEFUSION0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFUSION0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32));
    VPSS_DIEFUSION0.bits.k_y_mcw = u32McwY;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32), VPSS_DIEFUSION0.u32); 

    VPSS_DIEFUSION0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32));
    VPSS_DIEFUSION0.bits.k_c_mcw = u32McwC;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION0.u32), VPSS_DIEFUSION0.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieFusMcwAdj(HI_U32 *pu32AppAddr,HI_U32 u32Mcwg0,HI_U32 u32Mcwk0,HI_U32 u32Mcwk1,HI_U32 u32Mcwx0)
{
    U_VPSS_DIEFUSION1 VPSS_DIEFUSION1;
    U_VPSS_DIEFUSION2 VPSS_DIEFUSION2;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFUSION1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION1.u32));
    VPSS_DIEFUSION1.bits.g0_mcw_adj = u32Mcwg0;
    VPSS_DIEFUSION1.bits.k0_mcw_adj = u32Mcwk0;
    VPSS_DIEFUSION1.bits.x0_mcw_adj = u32Mcwx0;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION1.u32), VPSS_DIEFUSION1.u32); 

    VPSS_DIEFUSION2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION2.u32));
    VPSS_DIEFUSION2.bits.k1_mcw_adj = u32Mcwk1;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION2.u32), VPSS_DIEFUSION2.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetDieFusMcbld(HI_U32 *pu32AppAddr,HI_U32 u32Mcbldg0,HI_U32 u32Mcbldx0,HI_U32 u32Mcbldk1,HI_U32 u32Mcbldk0)
{
    U_VPSS_DIEFUSION2 VPSS_DIEFUSION2;
    U_VPSS_DIEFUSION3 VPSS_DIEFUSION3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFUSION2.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION2.u32));
    VPSS_DIEFUSION2.bits.k0_mcbld = u32Mcbldk0;
    VPSS_DIEFUSION2.bits.k1_mcbld = u32Mcbldk1;
    VPSS_DIEFUSION2.bits.x0_mcbld = u32Mcbldx0;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION2.u32), VPSS_DIEFUSION2.u32); 

    VPSS_DIEFUSION3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32));
    VPSS_DIEFUSION3.bits.g0_mcbld = u32Mcbldg0;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32), VPSS_DIEFUSION3.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieFusMcLaiBldMode(HI_U32 *pu32AppAddr,HI_U32 u32Mode)
{
    U_VPSS_DIEFUSION3 VPSS_DIEFUSION3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;


    VPSS_DIEFUSION3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32));
    VPSS_DIEFUSION3.bits.mc_lai_bldmode = u32Mode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32), VPSS_DIEFUSION3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieFusKCurwMcbld(HI_U32 *pu32AppAddr,HI_U32 u32Mcbld)
{
    U_VPSS_DIEFUSION3 VPSS_DIEFUSION3;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEFUSION3.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32));
    VPSS_DIEFUSION3.bits.k_curw_mcbld = u32Mcbld;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEFUSION3.u32), VPSS_DIEFUSION3.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDieMaGbmThd(HI_U32 *pu32AppAddr,HI_U32 *pu32MaGbmThd)
{
    U_VPSS_DIEMAGBMTHD0 VPSS_DIEMAGBMTHD0;
    U_VPSS_DIEMAGBMTHD1 VPSS_DIEMAGBMTHD1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEMAGBMTHD0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMAGBMTHD0.u32));
    VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd0 = pu32MaGbmThd[0];
    VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd1 = pu32MaGbmThd[1];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMAGBMTHD0.u32), VPSS_DIEMAGBMTHD0.u32); 

    VPSS_DIEMAGBMTHD1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMAGBMTHD1.u32));
    VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd2 = pu32MaGbmThd[2];
    VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd3 = pu32MaGbmThd[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMAGBMTHD1.u32), VPSS_DIEMAGBMTHD1.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetDieMcGbmCoef(HI_U32 *pu32AppAddr,HI_U32 *pu32Mtth,HI_U32 u32Mtfilten,HI_U32 u32Mag
                                                                ,HI_U32 u32Difh,HI_U32 u32MacMag)
{
    U_VPSS_DIEMCGBMCOEF0 VPSS_DIEMCGBMCOEF0;
    U_VPSS_DIEMCGBMCOEF1 VPSS_DIEMCGBMCOEF1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_DIEMCGBMCOEF0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMCGBMCOEF0.u32));
    VPSS_DIEMCGBMCOEF0.bits.mtfilten_gmd = u32Mtfilten;
    VPSS_DIEMCGBMCOEF0.bits.mtth0_gmd = pu32Mtth[0];
    VPSS_DIEMCGBMCOEF0.bits.mtth1_gmd = pu32Mtth[1];
    VPSS_DIEMCGBMCOEF0.bits.mtth2_gmd = pu32Mtth[2];
    VPSS_DIEMCGBMCOEF0.bits.mtth3_gmd = pu32Mtth[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMCGBMCOEF0.u32), VPSS_DIEMCGBMCOEF0.u32); 


    VPSS_DIEMCGBMCOEF1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_DIEMCGBMCOEF1.u32));
    VPSS_DIEMCGBMCOEF1.bits.k_mag_gmd = u32Mag;
    VPSS_DIEMCGBMCOEF1.bits.k_difh_gmd = u32Difh;
    VPSS_DIEMCGBMCOEF1.bits.k_maxmag_gmd = u32MacMag;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_DIEMCGBMCOEF1.u32), VPSS_DIEMCGBMCOEF1.u32); 

    return HI_SUCCESS;
}




HI_S32 VPSS_REG_SetPdLasiMode(HI_U32 *pu32AppAddr,HI_U32 u32LasiMode)
{
    U_VPSS_PDCTRL VPSS_PDCTRL;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_PDCTRL.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDCTRL.u32));
    VPSS_PDCTRL.bits.lasi_mode = u32LasiMode;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDCTRL.u32), VPSS_PDCTRL.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetPdBlkThd(HI_U32 *pu32AppAddr,HI_U32 u32DiffMovblkThd)
{
    U_VPSS_PDBLKTHD VPSS_PDBLKTHD;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_PDBLKTHD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDBLKTHD.u32));
    VPSS_PDBLKTHD.bits.diff_movblk_thd = u32DiffMovblkThd;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDBLKTHD.u32), VPSS_PDBLKTHD.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetPdModeCoef(HI_U32 *pu32AppAddr,HI_U32 u32Coring,HI_U32 u32alpha,HI_U32 *pu32Thd)
{
    U_VPSS_PDLASIMODE0COEF VPSS_PDLASIMODE0COEF;
    U_VPSS_PDLASIMODE1COEF VPSS_PDLASIMODE1COEF;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_PDLASIMODE0COEF.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASIMODE0COEF.u32));
    VPSS_PDLASIMODE0COEF.bits.lasi_txt_coring = u32Coring;
    VPSS_PDLASIMODE0COEF.bits.lasi_txt_alpha = u32alpha;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDLASIMODE0COEF.u32), VPSS_PDLASIMODE0COEF.u32); 

    VPSS_PDLASIMODE1COEF.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDLASIMODE1COEF.u32));
    VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd0 = pu32Thd[0];
    VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd1 = pu32Thd[1];
    VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd2 = pu32Thd[2];
    VPSS_PDLASIMODE1COEF.bits.lasi_txt_thd3 = pu32Thd[3];
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDLASIMODE1COEF.u32), VPSS_PDLASIMODE1COEF.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetPdRegion(HI_U32 *pu32AppAddr,HI_U32 u32Stt,HI_U32 u32End)
{
    U_VPSS_PDREGION VPSS_PDREGION;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_PDREGION.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_PDREGION.u32));
    VPSS_PDREGION.bits.region1_y_stt = u32Stt;
    VPSS_PDREGION.bits.region1_y_end = u32End;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PDREGION.u32), VPSS_PDREGION.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMcdiRgdify(HI_U32 *pu32AppAddr,HI_U32 u32k,HI_U32 u32g,HI_U32 u32core,HI_U32 u32lmt)
{
    U_VPSS_MCDI_RGDIFY VPSS_MCDI_RGDIFY;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGDIFY.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGDIFY.u32));
    VPSS_MCDI_RGDIFY.bits.k_rgdifycore = u32k;
    VPSS_MCDI_RGDIFY.bits.g_rgdifycore = u32g;
    VPSS_MCDI_RGDIFY.bits.core_rgdify = u32core;
    VPSS_MCDI_RGDIFY.bits.lmt_rgdify = u32lmt;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGDIFY.u32), VPSS_MCDI_RGDIFY.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetMcdiRgsad(HI_U32 *pu32AppAddr,HI_U32 u32k,HI_U32 u32g,HI_U32 u32Sadlpf,HI_U32 u32Kmv)
{
    U_VPSS_MCDI_RGSAD VPSS_MCDI_RGSAD;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGSAD.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGSAD.u32));
    VPSS_MCDI_RGSAD.bits.coef_sadlpf = u32Sadlpf;
    VPSS_MCDI_RGSAD.bits.kmv_rgsad = u32Kmv;
    VPSS_MCDI_RGSAD.bits.k_tpdif_rgsad = u32k;
    VPSS_MCDI_RGSAD.bits.g_tpdif_rgsad = u32g;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGSAD.u32), VPSS_MCDI_RGSAD.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetMcdiRgmv(HI_U32 *pu32AppAddr,HI_U32 u32thmag,HI_U32 u32saddif,HI_U32 u320mvsad)
{
    U_VPSS_MCDI_RGMV VPSS_MCDI_RGMV;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGMV.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMV.u32));
    VPSS_MCDI_RGMV.bits.thmag_rgmv = u32thmag;
    VPSS_MCDI_RGMV.bits.th_saddif_rgmv = u32saddif;
    VPSS_MCDI_RGMV.bits.th_0mvsad_rgmv = u320mvsad;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMV.u32), VPSS_MCDI_RGMV.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetMcdiRgmag0(HI_U32 *pu32AppAddr,HI_U32 u32core,HI_U32 u32lmt)
{
    U_VPSS_MCDI_RGMAG0 VPSS_MCDI_RGMAG0;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGMAG0.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMAG0.u32));
    VPSS_MCDI_RGMAG0.bits.core_mag_rg = u32core;
    VPSS_MCDI_RGMAG0.bits.lmt_mag_rg = u32lmt;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMAG0.u32), VPSS_MCDI_RGMAG0.u32); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetMcdiRgmag1(HI_U32 *pu32AppAddr,HI_U32 u32CoreMv,HI_U32 KMv,HI_U32 u32CoreMag,
                                                           HI_U32 u32KMag,HI_U32 u32Th,HI_U32 u32En)
{
    U_VPSS_MCDI_RGMAG1 VPSS_MCDI_RGMAG1;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGMAG1.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMAG1.u32));
    VPSS_MCDI_RGMAG1.bits.core_mv_rgmvls = u32CoreMv;
    VPSS_MCDI_RGMAG1.bits.k_mv_rgmvls = KMv;
    VPSS_MCDI_RGMAG1.bits.core_mag_rgmvls = u32CoreMag;
    VPSS_MCDI_RGMAG1.bits.k_mag_rgmvls = u32KMag;
    VPSS_MCDI_RGMAG1.bits.th_mvadj_rgmvls = u32Th;
    VPSS_MCDI_RGMAG1.bits.en_mvadj_rgmvls = u32En;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGMAG1.u32), VPSS_MCDI_RGMAG1.u32); 

    return HI_SUCCESS;
}



HI_S32 VPSS_REG_SetMcdiRgls(HI_U32 *pu32AppAddr,HI_U32 u32ksad,HI_U32 thmad,HI_U32 thsad,
                                                           HI_U32 u32ksadcore)
{
    U_VPSS_MCDI_RGLS VPSS_MCDI_RGLS;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_MCDI_RGLS.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGLS.u32));
    VPSS_MCDI_RGLS.bits.k_sad_rgls = u32ksad;
    VPSS_MCDI_RGLS.bits.th_mag_rgls = thmad;
    VPSS_MCDI_RGLS.bits.th_sad_rgls = thsad;
    VPSS_MCDI_RGLS.bits.k_sadcore_rgmv = u32ksadcore;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MCDI_RGLS.u32), VPSS_MCDI_RGLS.u32); 

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetSmmuAddr(HI_U8* pu32PhyAddr,HI_U32 u32PageAddr,HI_U32 u32ErrReadAddr,HI_U32 u32ErrWriteAddr)
{
    VPSS_REG_S *pstReg;
    
    HI_U32  SMMU_CB_TTBR;
   // HI_U32  SMMU_SCB_TTBR;
    HI_U32  SMMU_ERR_RDADDR_S;
    HI_U32  SMMU_ERR_WRADDR_S;
    HI_U32  SMMU_ERR_RDADDR_NS;
    HI_U32  SMMU_ERR_WRADDR_NS;
    
    pstReg = (VPSS_REG_S*)pu32PhyAddr;

    SMMU_CB_TTBR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_CB_TTBR));
    SMMU_CB_TTBR = u32PageAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_CB_TTBR), SMMU_CB_TTBR); 

   // SMMU_SCB_TTBR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCB_TTBR));
    //SMMU_SCB_TTBR = u32PageAddr;
    //VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_SCB_TTBR), SMMU_SCB_TTBR); 

    SMMU_ERR_RDADDR_S = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_RDADDR_S));
    SMMU_ERR_RDADDR_S = u32ErrReadAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_RDADDR_S), SMMU_ERR_RDADDR_S); 
    
    SMMU_ERR_WRADDR_S = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_WRADDR_S));
    SMMU_ERR_WRADDR_S = u32ErrWriteAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_WRADDR_S), SMMU_ERR_WRADDR_S); 

    SMMU_ERR_RDADDR_NS = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_RDADDR_NS));
    SMMU_ERR_RDADDR_NS = u32ErrReadAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_RDADDR_NS), SMMU_ERR_RDADDR_NS); 

    SMMU_ERR_WRADDR_NS = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_WRADDR_NS));
    SMMU_ERR_WRADDR_NS = u32ErrWriteAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_SMMU_ERR_WRADDR_NS), SMMU_ERR_WRADDR_NS); 

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetTransMeta(HI_U32 *pu32AppAddr,
                                HI_U32 u32SrcAddr,
                                HI_U32 u32DstAddr,
                                HI_U32 u32Size)
{
    VPSS_REG_S *pstReg;
    HI_U32 VPSS_TRANS_SRC_ADDR;
    HI_U32 VPSS_TRANS_DES_ADDR;
    U_VPSS_TRANS_INFO VPSS_TRANS_INFO;
    
    pstReg = (VPSS_REG_S*)pu32AppAddr;

    VPSS_TRANS_SRC_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TRANS_SRC_ADDR));
    VPSS_TRANS_SRC_ADDR = u32SrcAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TRANS_SRC_ADDR), VPSS_TRANS_SRC_ADDR); 

    VPSS_TRANS_DES_ADDR = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TRANS_DES_ADDR));
    VPSS_TRANS_DES_ADDR = u32DstAddr;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TRANS_DES_ADDR), VPSS_TRANS_DES_ADDR);

    VPSS_TRANS_INFO.u32 = VPSS_REG_RegRead((volatile HI_U32*)&(pstReg->VPSS_TRANS_INFO.u32));
    VPSS_TRANS_INFO.bits.trans_size = u32Size;
    VPSS_TRANS_INFO.bits.trans_en = HI_TRUE;
    VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_TRANS_INFO.u32), VPSS_TRANS_INFO.u32);
    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */

