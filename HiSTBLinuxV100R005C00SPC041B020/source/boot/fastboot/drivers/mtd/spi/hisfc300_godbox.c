#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>

#include "spi_ids.h"
#include "hisfc300.h"

/*periph hisfc CGR31 register*/
#define HISFC300_CRG31                                          (0x00BC)
#define HISFC300_CRG31_RST                                      (1 << 0)
#define HISFC300_CRG31_CLKEN                                    (1 << 8)
#define HISFC300_CRG31_CLK_24M                                  (0)       /* 24M */ 
#define HISFC300_CRG31_CLK_99M                                  (0x40000) /* 99M */ 
#define HISFC300_CRG31_CLK_50M                                  (0x50000) /* 49.5M */ 
#define HISFC300_CFG31_CLK_149M                                 (0x70000) /* 148.5M */

/*****************************************************************************/

void hisfc200_set_system_clock(struct hisfc_host *host,struct spi_operation *op, int clk_en)
{
	unsigned int regval = HISFC300_CRG31_CLK_24M;
	if (op && op->clock)
		regval = (op->clock & 0x70000);

	if (clk_en)
		regval |= HISFC300_CRG31_CLKEN;

	if (readl(host->cfgreg + HISFC300_CRG31 ) != regval)
	{
		writel(regval, (host->cfgreg + HISFC300_CRG31));
	}
}
/*****************************************************************************/
void hisfc200_get_best_clock(unsigned int * _clock)
{
	int ix,clk;
  #define CLK_2X(_clk)   (((_clk) + 1) >> 1)
	unsigned int sysclk[] = {
		CLK_2X(24),  HISFC300_CRG31_CLK_24M, 
		CLK_2X(50),  HISFC300_CRG31_CLK_50M,  /* 49.5M */
		CLK_2X(99),  HISFC300_CRG31_CLK_99M,
		CLK_2X(149), HISFC300_CFG31_CLK_149M, /* 148.5M */
		0,0,
	};
  #undef CLK_2X	
		
	clk = HISFC300_CRG31_CLK_24M; 
	for (ix = 0; sysclk[ix]; ix += 2) 
	{
	   if (*_clock < sysclk[ix]) 
		  break; 
	   clk = sysclk[ix+1]; 
	}
	
	*_clock = clk;
}
/*****************************************************************************/

