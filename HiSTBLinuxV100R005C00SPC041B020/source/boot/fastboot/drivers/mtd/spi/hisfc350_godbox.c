/******************************************************************************
*    Copyright (c) 2009-2011 by HiC.
*    All rights reserved.
* ***
*    Create by Czyong. 2011-08-27
*
******************************************************************************/

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>

#include "spi_ids.h"
#include "hisfc350.h"

/* periph hisfc CGR31 register*/
#define HISFC350_CRG31                                          (0x00BC)
#define HISFC350_CRG31_RST                                      (1 << 0)
#define HISFC350_CRG31_CLKEN                                    (1 << 8)

#define HISFC350_CRG31_CLK_25M                                  0x00050000
#define HISFC350_CRG31_CLK_50M                                  0x00040000
#define HISFC350_CRG31_CLK_75M                                  0x00060000
#define HISFC350_CFG31_CLK_SRC_OFFSET                           16


/* periph hisfc CGR31 register*/
#define HI3712_HISFC350_CRG31                                   (0x00BC)
#define HI3712_HISFC350_CRG31_RST                               (1 << 0)
#define HI3712_HISFC350_CRG31_CLKEN                             (1 << 8)

#define HI3712_HISFC350_CRG31_CLK_24M                           0x00000000
#define HI3712_HISFC350_CRG31_CLK_50M                           0x00090000
#define HI3712_HISFC350_CRG31_CLK_99M                           0x00080000
#define HI3712_HISFC350_CRG31_CLK_198M                          0x000a0000
#define HI3712_HISFC350_CRG31_CLK_149M                          0x000c0000
#define HI3712_HISFC350_CFG31_CLK_SRC_OFFSET                    16

/*****************************************************************************/

static void hisfc350_set_system_clock(struct hisfc_host *host,
	struct spi_operation *op, int clk_en)
{
	if (_HI3712_V100 == get_chipid()) {
		unsigned int regval = HI3712_HISFC350_CRG31_CLK_24M;
		if (op && op->clock) {
			regval = op->clock & 0xf0000;
			if (regval == HI3712_HISFC350_CRG31_CLK_198M)
				regval = HI3712_HISFC350_CRG31_CLK_149M;
		}

		if (clk_en)
			regval |= HI3712_HISFC350_CRG31_CLKEN;

		if (readl(host->cfgreg + HI3712_HISFC350_CRG31 ) != regval)
			writel(regval, (host->cfgreg + HI3712_HISFC350_CRG31));
	} else {
		unsigned int regval = HISFC350_CRG31_CLK_25M;
		if (op && op->clock)
			regval = (op->clock & 0x70000);

		if (clk_en)
			regval |= HISFC350_CRG31_CLKEN;

		if (readl(host->cfgreg + HISFC350_CRG31 ) != regval)
			writel(regval, (host->cfgreg + HISFC350_CRG31));
	}
}
/*****************************************************************************/
static void hisfc350_get_best_clock(unsigned int * clock)
{
	int ix,clk;

	if (_HI3712_V100 == get_chipid()) {
		unsigned int sysclk[] = {
			25,  HI3712_HISFC350_CRG31_CLK_50M,
			50,  HI3712_HISFC350_CRG31_CLK_99M,
			75,  HI3712_HISFC350_CRG31_CLK_149M,
			100, HI3712_HISFC350_CRG31_CLK_198M,
			0,0,
		};

		clk = HI3712_HISFC350_CRG31_CLK_24M;
		for (ix = 0; sysclk[ix] != 0; ix += 2) {
			if (*clock < sysclk[ix])
				break;
			clk = sysclk[ix+1];
		}

		*clock = clk;
	} else {
		unsigned int sysclk[] = {
			25, HISFC350_CRG31_CLK_25M,
			50, HISFC350_CRG31_CLK_50M,
			75, HISFC350_CRG31_CLK_75M,
			0,0,
		};

		clk = HISFC350_CRG31_CLK_25M; 
		for (ix = 0; sysclk[ix] != 0; ix += 2) {
			if (*clock < sysclk[ix])
				break;
			clk = sysclk[ix+1];
		}

		*clock = clk;
	}
}
/*****************************************************************************/
#ifdef CONFIG_HISFC350_SHOW_CYCLE_TIMING
static char * hisfc350_get_clock_str(unsigned int clk_reg)
{
	static char buffer[40];
	int ix;

	if (_HI3712_V100 == get_chipid()) {
		static unsigned int clk_str[] = {
			0, 12,
			1, 12,
			2, 12,
			3, 12,
			4, 12,
			5, 12,
			6, 12,
			7, 12,
			8, 45,
			9, 25,
			10, 100,
			11, 100,
			12, 75,
			13, 75,
			14, 75,
			15, 75,
		};

		clk_reg = (clk_reg & 0xf0000) >> HI3712_HISFC350_CFG31_CLK_SRC_OFFSET;

		for (ix = 0; clk_str[ix] < 16; ix += 2) {
			if (clk_reg == clk_str[ix]) {
				sprintf(buffer, "%dM", clk_str[ix+1]);
				break;
			}
		}
		return buffer;
	} else {
		static unsigned int clk_str[] = {
			4, 50,         /* 0x100 : 50M */
			5, 25,         /* 0x101 : 25M */
			6, 75,         /* 0x110 : 75M */
			7, 75,         /* 0x111 : 75M */
			0, 0,
		};

		clk_reg = (clk_reg & 0x70000) >> HISFC350_CFG31_CLK_SRC_OFFSET;

		for (ix = 0; clk_str[ix] != 0; ix += 2) {
			if (clk_reg == clk_str[ix]) {
				sprintf(buffer, "%dM", clk_str[ix+1]);
				break;
			}
		}
		return buffer;
	}
}
#endif /* CONFIG_HISFC350_SHOW_CYCLE_TIMING */
