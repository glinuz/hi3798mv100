/******************************************************************************
*    Copyright (c) 2014 by Hisilicon.
*    All rights reserved.
******************************************************************************/

#include "hisfc400.h"

/* periph hisfc CGR23 register*/
#define HISFC400_CRG23                                          (0x005C)
#define HISFC400_CRG23_RST                                      (1 << 7)
#define HISFC400_CRG23_CLK2X_RST                                (1 << 3)
#define HISFC400_CRG23_CLKEN                                    (1 << 6)
#define HISFC400_CRG23_CLK2X_CLKEN                              (1 << 5)


#define HISFC400_CRG23_CLK_37M                                  0x000700
#define HISFC400_CRG23_CLK_50M                                  0x000600
#define HISFC400_CRG23_CLK_75M                                  0x000400
#define HISFC400_CRG23_CLK_100M                                 0x000500
#define HISFC400_CFG23_CLK_SRC_OFFSET                           8


/*****************************************************************************/
void hisfc400_set_system_clock(struct hisfc_host *host,
			       struct spi_operation *op, int clk_en)
{
#ifndef HI3716MV310_FPGA
	unsigned int regval = HISFC400_CRG23_CLK_37M;
	if (op && op->clock)
		regval = (op->clock & 0x700);

	if (clk_en)
		regval |= (HISFC400_CRG23_CLKEN | HISFC400_CRG23_CLK2X_CLKEN);
	
	if (readl(host->sysreg + HISFC400_CRG23 ) != regval)
		writel(regval, (host->sysreg + HISFC400_CRG23));
#endif
}

/*****************************************************************************/
void hisfc400_get_best_clock(unsigned int *clock)
{
#ifndef HI3716MV310_FPGA
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

