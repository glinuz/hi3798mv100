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

#define HI3716MV310_HINFC610_PERI_CRG_REG_BASE (REG_BASE_CRG)
#define HI3716MV310_PERI_CRG24                 (HI3716MV310_HINFC610_PERI_CRG_REG_BASE + 0x60)

#define HI3716MV310_PERI_CRG24_CLK_EN          (0x1U << 0)

#define HI3716MV310_PERI_CRG24_CLK_SEL_MASK    (0x7U << 8)
#define HI3716MV310_PERI_CRG24_CLK_SEL_24M     (0x0U << 8)
#define HI3716MV310_PERI_CRG24_CLK_SEL_200M    (0x4U << 8)

#define HI3716MV310_PERI_CRG24_NF_SRST_REQ     (0x1U << 4)

/******************************************************************************/

void hinfc610_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned long reg_val;

	reg_val = readl(HI3716MV310_PERI_CRG24);

	reg_val &= ~HI3716MV310_PERI_CRG24_CLK_SEL_MASK;

	if (enable) {
		reg_val |= (HI3716MV310_PERI_CRG24_CLK_EN
			| HI3716MV310_PERI_CRG24_CLK_SEL_200M);
	} else {
		reg_val &= ~HI3716MV310_PERI_CRG24_CLK_EN;
	}

	reg_val &= ~HI3716MV310_PERI_CRG24_NF_SRST_REQ;

	writel(reg_val, HI3716MV310_PERI_CRG24);
}
/*****************************************************************************/

static void hinfc610_sync_clock(struct hinfc_host *host, unsigned int enable)
{
	/* nandcv620 on godbox(Hi3716Mv310) not support sync nand. */
	return;
}
