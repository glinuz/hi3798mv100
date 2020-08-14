/******************************************************************************
*    Copyright (c) 2014 by Hisilicon.
*    All rights reserved.
******************************************************************************/

#include "hisfc400.h"

/* periph hisfc CGR23 register*/
#define HISFC400_CRG23                                          (0x005C)
#define HISFC400_CRG23_RST                                      (1 << 1)
#define HISFC400_CRG23_CLKEN                                    (1 << 0)

#define HISFC400_CRG23_CLK_37M                                  0x001c
#define HISFC400_CRG23_CLK_50M                                  0x0018
#define HISFC400_CRG23_CLK_75M                                  0x0010
#define HISFC400_CRG23_CLK_100M                                 0x0014
#define HISFC400_CFG23_CLK_SRC_OFFSET                           2


/*****************************************************************************/
void hisfc400_set_system_clock(struct hisfc_host *host,
			       struct spi_operation *op, int clk_en)
{
#ifndef CONFIG_HIFONE_FPGA
	unsigned int regval = HISFC400_CRG23_CLK_37M;
	if (op && op->clock)
		regval = (op->clock & 0x1c);

	if (clk_en)
		regval |= HISFC400_CRG23_CLKEN;

	if (readl(host->sysreg + HISFC400_CRG23) != regval)
		writel(regval, (host->sysreg + HISFC400_CRG23));
#endif
}

/*****************************************************************************/
void hisfc400_get_best_clock(unsigned int *clock)
{
#ifndef CONFIG_HIFONE_FPGA
	int ix,clk;

		unsigned int sysclk[] = {
			38,  HISFC400_CRG23_CLK_37M,
			50,  HISFC400_CRG23_CLK_50M,
			75,  HISFC400_CRG23_CLK_75M,
			100, HISFC400_CRG23_CLK_100M,
			0,0,
		};

		clk = HISFC400_CRG23_CLK_37M; 
		for (ix = 0; sysclk[ix] != 0; ix += 2) {
			if (*clock < sysclk[ix])
				break;
			clk = sysclk[ix+1];
		}

		*clock = clk;
#endif
}

