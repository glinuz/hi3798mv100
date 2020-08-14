#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>

#include "spi_ids.h"
#include "hisfc300.h"

/*periph hisfc CGR25 register*/
#define HISFC300_CRG25                                          (0x0064)
#define HISFC300_CRG25_RST                                      (1 << 0)
#define HISFC300_CRG25_CLKEN                                    (1 << 1)
#define HISFC300_CRG25_CLK_24M                                  ((0x00)<<2) /* 24M */ 
#define HISFC300_CRG25_CLK_100M                                 ((0x01)<<2) /* 100M */ 
#define HISFC300_CFG25_CLK_149M                                 ((0x11)<<2) /* 148.5M */

/*****************************************************************************/
void hisfc200_set_system_clock(struct hisfc_host *host,struct spi_operation *op, int clk_en)
{
	unsigned int regval = HISFC300_CRG25_CLK_24M;
	if (op && op->clock)
		regval = (op->clock & 0xC);

	if (clk_en)
		regval |= HISFC300_CRG25_CLKEN;

	if (readl(host->cfgreg + HISFC300_CRG25 ) != regval)
	{
		writel(regval, (host->cfgreg + HISFC300_CRG25));
	}
}

/*****************************************************************************/
void hisfc200_get_best_clock(unsigned int * _clock)
{
	int ix,clk;
  #define CLK_2X(_clk)   (((_clk) + 1) >> 1)
	unsigned int sysclk[] = {
		CLK_2X(24),  HISFC300_CRG25_CLK_24M, 
		CLK_2X(100),  HISFC300_CRG25_CLK_100M,
		CLK_2X(149), HISFC300_CFG25_CLK_149M, /* 148.5M */
		0,0,
	};
  #undef CLK_2X	
	
	clk = HISFC300_CRG25_CLK_24M; 
	for (ix = 0; sysclk[ix]; ix += 2) 
	{
	   if (*_clock < sysclk[ix]) 
		  break; 
	   clk = sysclk[ix+1]; 
	}
	
	*_clock = clk;
}
/*****************************************************************************/
