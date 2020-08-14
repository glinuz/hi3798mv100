/******************************************************************************
*    NAND Flash Controller V504 Device Driver
*    Copyright (c) 2009-2010 by Hisilicon.
*    All rights reserved.
* ***
*
******************************************************************************/

#include <nand.h>
#include <asm/io.h>
#include "hinfc610.h"

#define HI3716CV200_HINFC610_PERI_CRG_REG_BASE (REG_BASE_CRG)
#define HI3716CV200_PERI_CRG24                 (HI3716CV200_HINFC610_PERI_CRG_REG_BASE + 0x60)

#define HI3716CV200_PERI_CRG24_CLK_EN          (0x1U << 0)

#define HI3716CV200_PERI_CRG24_CLK_SEL_MASK    (0x7U << 8)
#define HI3716CV200_PERI_CRG24_CLK_SEL_24M     (0x0U << 8)
#define HI3716CV200_PERI_CRG24_CLK_SEL_200M    (0x4U << 8)
#define HI3716CV200_PERI_CRG24_CLK_SEL_150M    (0x5U << 8)
#define HI3716CV200_PERI_CRG24_CLK_SEL_125M    (0x6U << 8)
#define HI3716CV200_PERI_CRG24_CLK_SEL_100M    (0x7U << 8)

#define HI3716CV200_PERI_CRG24_NF_SRST_REQ     (0x1U << 4)

/******************************************************************************/

void hinfc610_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned long reg_val;

	reg_val = readl(HI3716CV200_PERI_CRG24);

	reg_val &= ~HI3716CV200_PERI_CRG24_CLK_SEL_MASK;

	if (enable) {
		reg_val |= (HI3716CV200_PERI_CRG24_CLK_EN
			| HI3716CV200_PERI_CRG24_CLK_SEL_200M);
	} else {
		reg_val &= ~HI3716CV200_PERI_CRG24_CLK_EN;
	}

	reg_val &= ~HI3716CV200_PERI_CRG24_NF_SRST_REQ;

	writel(reg_val, HI3716CV200_PERI_CRG24);
}
/*****************************************************************************/

static void hinfc610_sync_clock(struct hinfc_host *host, unsigned int enable)
{
	unsigned int regval;

	hinfc_write(host, HINFC610_SYNC_ONFI_T_CAD
			  | HINFC610_SYNC_ONFI_T_DQZ
			  | HINFC610_SYNC_TOGGLE_PRE_RDQS
			  | HINFC610_SYNC_TOGGLE_POST_RDQS
			  | HINFC610_SYNC_TOGGLE_PRE_WDQS
			  | HINFC610_SYNC_TOGGLE_POST_WDQS
			  | HINFC610_SYNC_TOGGLE_RW_PSTH,
		    HINFC610_SYNC_TIMING);

	/*
	 * toggle1.0 nandc clock must be lower than 150M, otherwise
	 * read/write will fail.
	 * here configure to 100M for safety.
	 */
	regval = readl(HI3716CV200_PERI_CRG24);

	regval &= ~HI3716CV200_PERI_CRG24_CLK_SEL_MASK;
	regval |= enable ? HI3716CV200_PERI_CRG24_CLK_SEL_100M
		: HI3716CV200_PERI_CRG24_CLK_SEL_200M;

	writel(regval, HI3716CV200_PERI_CRG24);
}
