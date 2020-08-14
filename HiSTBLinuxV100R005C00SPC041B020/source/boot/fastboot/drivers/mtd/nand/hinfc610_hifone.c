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

#define HIFONE_HINFC610_PERI_CRG_REG_BASE (REG_BASE_CRG)
#define HIFONE_PERI_CRG24                 (HIFONE_HINFC610_PERI_CRG_REG_BASE + 0x60)

#define HIFONE_PERI_CRG24_CLK_EN          (0x1U << 0)

#define HIFONE_PERI_CRG24_CLK_SEL_MASK    (0x3U << 8)
#define HIFONE_PERI_CRG24_CLK_SEL_24M     (0x0U << 8)
#define HIFONE_PERI_CRG24_CLK_SEL_200M    (0x1U << 8)
#define HIFONE_PERI_CRG24_CLK_SEL_150M    (0x2U << 8)
#define HIFONE_PERI_CRG24_CLK_SEL_100M    (0x3U << 8)

#define HIFONE_PERI_CRG24_NF_SRST_REQ     (0x1U << 4)

/******************************************************************************/

void hinfc610_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned long reg_val;

	reg_val = readl(HIFONE_PERI_CRG24);

	reg_val &= ~HIFONE_PERI_CRG24_CLK_SEL_MASK;

	if (enable) {
		reg_val |= (HIFONE_PERI_CRG24_CLK_EN
			| HIFONE_PERI_CRG24_CLK_SEL_200M);
	} else {
		reg_val &= ~HIFONE_PERI_CRG24_CLK_EN;
	}

	reg_val &= ~HIFONE_PERI_CRG24_NF_SRST_REQ;

	writel(reg_val, HIFONE_PERI_CRG24);
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
	regval = readl(HIFONE_PERI_CRG24);

	regval &= ~HIFONE_PERI_CRG24_CLK_SEL_MASK;
	regval |= enable ? HIFONE_PERI_CRG24_CLK_SEL_100M
		: HIFONE_PERI_CRG24_CLK_SEL_200M;

	writel(regval, HIFONE_PERI_CRG24);
}
