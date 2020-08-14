/******************************************************************************
*    NAND Flash Controller V301 Device Driver
*    Copyright (c) 2009-2010 by Hisilicon.
*    All rights reserved.
* ***
*    
******************************************************************************/

#include <nand.h>
#include <asm/io.h>
#include "hinfc301.h"

/******************************************************************************/

#define HINFC301_PERIPHERY_REGBASE                  (0x20030000)

#define PERI_CRG30                                  (0x0060)
#define PERI_CRG30_CLK_EN                           (1U << 1)
#define PERI_CRG30_CLK_SEL_99M                      (1U << 2)

/******************************************************************************/

void hinfc301_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned int reg_val = readl(HINFC301_PERIPHERY_REGBASE + PERI_CRG30);
	if (enable)
	{
		reg_val |= (PERI_CRG30_CLK_EN | PERI_CRG30_CLK_SEL_99M);
	}
	else
	{
		reg_val &= ~PERI_CRG30_CLK_EN;
	}
	writel(reg_val, (HINFC301_PERIPHERY_REGBASE + PERI_CRG30));
}

