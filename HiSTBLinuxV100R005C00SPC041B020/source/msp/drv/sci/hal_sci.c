#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>

#include "hal_sci.h"
#include "drv_sci.h"
#include "hi_reg_common.h"

volatile SCI_REG_S       *g_pSciReg[HI_UNF_SCI_PORT_BUTT];

HI_VOID SCI_HAL_Init(HI_UNF_SCI_PORT_E enSciPort, SCI_REG_S *pSciReg)
{
    g_pSciReg[enSciPort] = pSciReg;

    return;
}

HI_S32 SCI_EnableClock(HI_UNF_SCI_PORT_E enSciPort,HI_BOOL bEnable)
{
	HI_U32 u32Timeout = 0;
	U_PERI_CRG29 uTmpValue;

	if(HI_UNF_SCI_PORT0 == enSciPort)
	{
	    uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
	    uTmpValue.bits.sci0_cken = bEnable;
		g_pstRegCrg->PERI_CRG29.u32 = uTmpValue.u32;
		while(u32Timeout < 100)
		{
			uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
			if(uTmpValue.bits.sci0_cken != bEnable )
			{
				u32Timeout++;
				udelay(1);
			}
			else
			{
				break;
			}
		}
		if(u32Timeout >= 100)
		{
			HI_ERR_SCI("SCI0 set clock timeout!bEnale:%d!\n",bEnable);
			return HI_FAILURE;
		}
	}
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798cv100)
	if(HI_UNF_SCI_PORT1 == enSciPort)
	{
		uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
	    uTmpValue.bits.sci1_cken = bEnable;
		g_pstRegCrg->PERI_CRG29.u32 = uTmpValue.u32;
		while(u32Timeout < 100)
		{
			uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
			if(uTmpValue.bits.sci1_cken != bEnable )
			{
				u32Timeout++;
				udelay(1);
			}
			else
			{
				break;
			}
		}
		if(u32Timeout >= 100)
		{
			HI_ERR_SCI("SCI1 set clock timeout!bEnale:%d!\n",bEnable);
			return HI_FAILURE;
		}

	}
#endif
	mb();
	return HI_SUCCESS;
}

HI_S32 SCI_ResetClock(HI_UNF_SCI_PORT_E enSciPort,HI_BOOL bReset)
{
	HI_U32 u32Timeout = 0;
	U_PERI_CRG29 uTmpValue;

	if(enSciPort > HI_UNF_SCI_PORT1)
	{
		HI_ERR_SCI("Err param1!!\n");
		return HI_FAILURE;
	}
	
	if(HI_UNF_SCI_PORT0 == enSciPort)
	{
	    uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
	    uTmpValue.bits.sci0_srst_req = bReset;
		g_pstRegCrg->PERI_CRG29.u32 = uTmpValue.u32;
		while(u32Timeout < 100)
		{
			uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
			if(uTmpValue.bits.sci0_srst_req != bReset )
			{
				u32Timeout++;
				udelay(1);
			}
			else
			{
				break;
			}
		}
		if(u32Timeout >= 100)
		{
			HI_ERR_SCI("SCI0 reset clock timeout!bReset:%d!\n",bReset);
			return HI_FAILURE;
		}
	}
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798cv100)
	if(HI_UNF_SCI_PORT1 == enSciPort)
	{
		uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
	    uTmpValue.bits.sci1_srst_req = bReset;
		g_pstRegCrg->PERI_CRG29.u32 = uTmpValue.u32;
		while(u32Timeout < 100)
		{
			uTmpValue.u32  = g_pstRegCrg->PERI_CRG29.u32;
			if(uTmpValue.bits.sci1_srst_req != bReset )
			{
				u32Timeout++;
				udelay(1);
			}
			else
			{
				break;
			}
		}
		if(u32Timeout >= 100)
		{
			HI_ERR_SCI("SCI1 reset clock timeout!bReset:%d!\n",bReset);
			return HI_FAILURE;
		}

	}
#endif
	mb();
	return HI_SUCCESS;
}


/*set sci clk module*/
HI_VOID SCI_HAL_SetSCIClock(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
	if(enSciPort > HI_UNF_SCI_PORT1)
	{
		HI_ERR_SCI("Err param1!!\n");
		return;
	}
	
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798cv100)

    if (HI_TRUE == bEnable)
    {
        if(HI_UNF_SCI_PORT0 == enSciPort)
        {
			if(SCI_EnableClock(HI_UNF_SCI_PORT0,HI_TRUE))
				return;
		}
        else
		{
			if(SCI_EnableClock(HI_UNF_SCI_PORT1,HI_TRUE))
				return;
		}
    }
    else
    {
        if(HI_UNF_SCI_PORT0 == enSciPort)
        {
			if(SCI_EnableClock(HI_UNF_SCI_PORT0,HI_FALSE))
				return;
		}
        else
		{
			if(SCI_EnableClock(HI_UNF_SCI_PORT1,HI_FALSE))
				return;
		}
    }

#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798hv100)

    if (HI_UNF_SCI_PORT0 == enSciPort)
    {
        if (HI_TRUE == bEnable)
        {
            if(SCI_EnableClock(HI_UNF_SCI_PORT0,HI_TRUE))
				return;
        }
        else
        {
            if(SCI_EnableClock(HI_UNF_SCI_PORT0,HI_FALSE))
				return;
        }
    }
#endif
	 
    return;
}

/*sci module reset*/
HI_VOID SCI_HAL_SetSCIReset(HI_UNF_SCI_PORT_E enSciPort)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798cv100)

    if(HI_UNF_SCI_PORT0 == enSciPort)
    {
		if(SCI_ResetClock(HI_UNF_SCI_PORT0,HI_TRUE))
			return;
	}
    else
	{
		if(SCI_ResetClock(HI_UNF_SCI_PORT1,HI_TRUE))
			return;
	}

#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798hv100)

    if(HI_UNF_SCI_PORT0 == enSciPort)
    {
		if(SCI_ResetClock(HI_UNF_SCI_PORT0,HI_TRUE))
			return;
	}
#endif
    return;
}

/*remove sci module reset*/
HI_VOID SCI_HAL_ClearSCIReset(HI_UNF_SCI_PORT_E enSciPort)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798cv100)

	if(HI_UNF_SCI_PORT0 == enSciPort)
    {
		if(SCI_ResetClock(HI_UNF_SCI_PORT0,HI_FALSE))
			return;
	}
    else
	{
		if(SCI_ResetClock(HI_UNF_SCI_PORT1,HI_FALSE))
			return;
	}
	
#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798hv100)

    if(HI_UNF_SCI_PORT0 == enSciPort)
    {
		if(SCI_ResetClock(HI_UNF_SCI_PORT0,HI_FALSE))
			return;
	}
#endif

    return;
}

HI_U32 SCI_HAL_ReadData(HI_UNF_SCI_PORT_E enSciPort)
{
    return g_pSciReg[enSciPort]->SciData.Value;
}

HI_VOID SCI_HAL_WriteData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 SciData)
{
    g_pSciReg[enSciPort]->SciData.Value = SciData;
    return;
}

/*      CR0         */
HI_BOOL SCI_HAL_GetCardPresent(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SYNCACT_REG SciSyncAct;

    SciSyncAct.Value = g_pSciReg[enSciPort]->SciSyncAct.Value;

    if (SciSyncAct.Bits.cardpresent)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_U32 SCI_HAL_GetCr0(HI_UNF_SCI_PORT_E enSciPort)
{
    return g_pSciReg[enSciPort]->SciCr0.Value;
}

HI_VOID SCI_HAL_SetVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciVcc)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.vccen_inv = !enSciVcc;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_VOID SCI_HAL_SetDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciDetect)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.detect_inv = !enSciDetect;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_VOID SCI_HAL_SetDataReversal(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bDataReversal)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.sense = bDataReversal;
    SciCr0.Bits.order = bDataReversal;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_VOID SCI_HAL_SetTxParityMode(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, SCI_PARITY_E enParity)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.txparity = enParity;
    SciCr0.Bits.txnak = bEnable;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_VOID SCI_HAL_SetRxParityMode(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, SCI_PARITY_E enParity)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.rxparity = enParity;
    SciCr0.Bits.rxnak = bEnable;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_VOID SCI_HAL_SetParityEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.paritybit = bEnable;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

HI_BOOL SCI_HAL_GetParityEnable(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    return SciCr0.Bits.paritybit;
}

HI_VOID SCI_HAL_SetClock(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, HI_U32 Value)
{
    SCI_SCR0_REG SciCr0;

    SciCr0.Value = g_pSciReg[enSciPort]->SciCr0.Value;

    SciCr0.Bits.clkdis = bEnable;
    SciCr0.Bits.clkval = Value;

    g_pSciReg[enSciPort]->SciCr0.Value = SciCr0.Value;

    return;
}

/*      CR1         */
HI_VOID SCI_HAL_SetAtrTimeoutEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.atrden = bEnable;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SetBlockTimeoutEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.blken = bEnable;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SetDirection(HI_UNF_SCI_PORT_E enSciPort, SCI_DIRECTION_E enDirection)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.mode = enDirection;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SetClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.clkzl = enClkMode;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SetResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)

    U_PERI_SIM_OD_CTRL uTmpValue;

    uTmpValue.u32 = g_pstRegPeri->PERI_SIM_OD_CTRL.u32;

    if (enSciPort == HI_UNF_SCI_PORT0)
    {
        uTmpValue.bits.sim0_rst_od_sel = enResetMode;
    }
    else
    {
        uTmpValue.bits.sim1_rst_od_sel = enResetMode;
    }

    g_pstRegPeri->PERI_SIM_OD_CTRL.u32 = uTmpValue.u32;
#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)

    U_PERI_SIM_OD_CTRL uTmpValue;

    if (HI_UNF_SCI_PORT0 == enSciPort)
    {
        uTmpValue.u32 = g_pstRegPeri->PERI_SIM_OD_CTRL.u32;

        uTmpValue.bits.sim0_rst_od_sel = enResetMode;

        g_pstRegPeri->PERI_SIM_OD_CTRL.u32 = uTmpValue.u32;
    }
#elif defined(CHIP_TYPE_hi3798cv200)||defined(CHIP_TYPE_hi3798mv200)|| defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)
	
	SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.rstoutctl = enResetMode;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;
#endif
	return;
}

HI_VOID SCI_HAL_SetVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)

    U_PERI_SIM_OD_CTRL uTmpValue;

    uTmpValue.u32 = g_pstRegPeri->PERI_SIM_OD_CTRL.u32;

    if (enSciPort == HI_UNF_SCI_PORT0)
    {
        uTmpValue.bits.sim0_pwren_od_sel = enVccEnMode;
    }
    else
    {
        uTmpValue.bits.sim1_pwren_od_sel = enVccEnMode;
    }

    g_pstRegPeri->PERI_SIM_OD_CTRL.u32 = uTmpValue.u32;
#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)

    U_PERI_SIM_OD_CTRL uTmpValue;

    if (HI_UNF_SCI_PORT0 == enSciPort)
    {
        uTmpValue.u32 = g_pstRegPeri->PERI_SIM_OD_CTRL.u32;

        uTmpValue.bits.sim0_pwren_od_sel = enVccEnMode;

        g_pstRegPeri->PERI_SIM_OD_CTRL.u32 = uTmpValue.u32;
    }
#elif defined(CHIP_TYPE_hi3798cv200)||defined(CHIP_TYPE_hi3798mv200)|| defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)
	
	SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.vccenoutctl = enVccEnMode;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

#endif
	return;
}

HI_VOID SCI_HAL_SetBlockProtect(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.bgten = bEnable;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SelectCounter(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.exdbnce = Value;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

HI_VOID SCI_HAL_SetSync(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable)
{
    SCI_SCR1_REG SciCr1;

    SciCr1.Value = g_pSciReg[enSciPort]->SciCr1.Value;

    SciCr1.Bits.synccard = bEnable;

    g_pSciReg[enSciPort]->SciCr1.Value = SciCr1.Value;

    return;
}

/*      CR2         */
HI_VOID SCI_HAL_Startup(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SCR2_REG SciCr2;

    SciCr2.Value = g_pSciReg[enSciPort]->SciCr2.Value;

    SciCr2.Bits.startup = 1;

    g_pSciReg[enSciPort]->SciCr2.Value = SciCr2.Value;

    return;
}

HI_VOID SCI_HAL_Finish(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SCR2_REG SciCr2;

    SciCr2.Value = g_pSciReg[enSciPort]->SciCr2.Value;

    SciCr2.Bits.finish = 1;

    g_pSciReg[enSciPort]->SciCr2.Value = SciCr2.Value;

    return;
}

HI_VOID SCI_HAL_Reset(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SCR2_REG SciCr2;

    SciCr2.Value = g_pSciReg[enSciPort]->SciCr2.Value;

    SciCr2.Bits.wreset = 1;

    g_pSciReg[enSciPort]->SciCr2.Value = SciCr2.Value;

    return;
}

HI_VOID SCI_HAL_SetClkIcc(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value)
{
    g_pSciReg[enSciPort]->SciClkIcc.Value = Value;

    return;
}

HI_VOID SCI_HAL_SetEtu(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value)
{
    g_pSciReg[enSciPort]->SciValue.Value = Value;

    return;
}

HI_VOID SCI_HAL_SetBaud(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value)
{
    g_pSciReg[enSciPort]->SciBaud.Value = Value;

    return;
}

HI_VOID SCI_HAL_SetTide(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxTide, HI_U32 TxTide)
{
    SCI_TIDE_REG SciTide;

    SciTide.Value = g_pSciReg[enSciPort]->SciTide.Value;

    SciTide.Bits.txtide = TxTide;
    SciTide.Bits.rxtide = RxTide;

    g_pSciReg[enSciPort]->SciTide.Value = SciTide.Value;

    return;
}

HI_VOID SCI_HAL_SetStableTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StableTime)
{
    g_pSciReg[enSciPort]->SciStable.Value = StableTime;

    return;
}

HI_VOID SCI_HAL_SetActiveTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 ActiveTime)
{
    g_pSciReg[enSciPort]->SciATime.Value = ActiveTime;

    return;
}

HI_VOID SCI_HAL_SetDeactTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 DeactTime)
{
    g_pSciReg[enSciPort]->SciDTime.Value = DeactTime;

    return;
}

HI_VOID SCI_HAL_SetAtrStartTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 AtrStartTime)
{
    g_pSciReg[enSciPort]->SciAtrsTime.Value = AtrStartTime;

    return;
}

HI_VOID SCI_HAL_SetAtrDurationTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 AtrDurationTime)
{
    g_pSciReg[enSciPort]->SciAtrdTime.Value = AtrDurationTime;

    return;
}

HI_VOID SCI_HAL_SetStopTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StopTime)
{
    g_pSciReg[enSciPort]->SciStopTime.Value = StopTime;

    return;
}

HI_VOID SCI_HAL_SetStartTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StartTime)
{
    g_pSciReg[enSciPort]->SciStartTime.Value = StartTime;

    return;
}

HI_VOID SCI_HAL_SetRetry(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxRetry, HI_U32 TxReTry)
{
    SCI_RETRY_REG SciRetry;

    SciRetry.Value = g_pSciReg[enSciPort]->SciRetry.Value;

    SciRetry.Bits.txretry = TxReTry;
    SciRetry.Bits.rxretry = RxRetry;

    g_pSciReg[enSciPort]->SciRetry.Value = SciRetry.Value;

    return;
}

/*must set high 16 bit of SciChTimems register at first,then set low 16bit of register*/
HI_VOID SCI_HAL_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 CharTimeout)
{
    g_pSciReg[enSciPort]->SciChTimems.Value = (CharTimeout & 0xffff0000) >> 16;
    g_pSciReg[enSciPort]->SciChTimels.Value = CharTimeout & 0xffff;

    return;
}

HI_VOID SCI_HAL_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockTimeout)
{
    g_pSciReg[enSciPort]->SciBlkTimems.Value = (BlockTimeout & 0xffff0000) >> 16;
    g_pSciReg[enSciPort]->SciBlkTimels.Value = BlockTimeout & 0xffff;

    return;
}

HI_VOID SCI_HAL_SetCharGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 CharGuard)
{
    g_pSciReg[enSciPort]->SciChGuard.Value = CharGuard;
    return;
}

HI_VOID SCI_HAL_SetBlockGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockGuard)
{
    g_pSciReg[enSciPort]->SciBlkGuard.Value = BlockGuard;

    return;
}

HI_VOID SCI_HAL_SetRxTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxTime)
{
    g_pSciReg[enSciPort]->SciRxTime.Value = RxTime;

    return;
}

HI_U32 SCI_HAL_GetTxFifo(HI_UNF_SCI_PORT_E enSciPort)
{
    return g_pSciReg[enSciPort]->SciTxCount.Value;
}

HI_VOID SCI_HAL_ClearTxFifo(HI_UNF_SCI_PORT_E enSciPort)
{
    g_pSciReg[enSciPort]->SciTxCount.Value = 0xffffffff;

    return;
}

HI_U32 SCI_HAL_GetRxFifo(HI_UNF_SCI_PORT_E enSciPort)
{
    return g_pSciReg[enSciPort]->SciRxCount.Value;
}

HI_VOID SCI_HAL_ClearRxFifo(HI_UNF_SCI_PORT_E enSciPort)
{
    g_pSciReg[enSciPort]->SciRxCount.Value = 0xffffffff;

    return;
}

HI_VOID SCI_HAL_DisableAllInt(HI_UNF_SCI_PORT_E enSciPort)
{
    g_pSciReg[enSciPort]->SciImsc.Value = 0;

    return;
}

HI_VOID SCI_HAL_EnableInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType)
{
    SCI_IMSC_REG SciImsc;

    SciImsc.Value = g_pSciReg[enSciPort]->SciImsc.Value;

    SciImsc.Value |= SciIntType;

    g_pSciReg[enSciPort]->SciImsc.Value = SciImsc.Value;

    return;
}

HI_VOID SCI_HAL_DisableInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType)
{
    SCI_IMSC_REG SciImsc;

    SciImsc.Value = g_pSciReg[enSciPort]->SciImsc.Value;

    SciImsc.Value = SciImsc.Value & (~SciIntType);

    g_pSciReg[enSciPort]->SciImsc.Value = SciImsc.Value;

    return;
}

HI_U32 SCI_HAL_GetIntState(HI_UNF_SCI_PORT_E enSciPort)
{
    return g_pSciReg[enSciPort]->SciMis.Value;
}

HI_VOID SCI_HAL_ClearAllInt(HI_UNF_SCI_PORT_E enSciPort)
{
    g_pSciReg[enSciPort]->SciIcr.Value = 0xffffffff;

    return;
}

HI_VOID SCI_HAL_ClearInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType)
{
    g_pSciReg[enSciPort]->SciIcr.Value = SciIntType;

    return;
}
