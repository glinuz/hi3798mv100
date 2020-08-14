#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>

#include "spi_ids.h"
#include "hisfc300.h"

/* PERI_CRG48 SFC register*/
#define HISFC300_CRG48                     (0x00C0)
#define HISFC300_CRG48_RST                 (1 << 0)
#define HISFC300_CRG48_CLKEN               (1 << 1)
#define HISFC300_CRG48_CLK_24M             ((0x00)<<2) /* 24M */
#define HISFC300_CRG48_CLK_100M            ((0x01)<<2) /* 100M */

/*****************************************************************************/
void hisfc200_set_system_clock(struct hisfc_host *host,
		struct spi_operation *op, int clk_en)
{
	unsigned int regval = HISFC300_CRG48_CLK_24M;
	if (op && op->clock)
		regval = (op->clock & 0xC);

	if (clk_en)
		regval |= HISFC300_CRG48_CLKEN;

	if (readl(host->cfgreg + HISFC300_CRG48) != regval)
		writel(regval, (host->cfgreg + HISFC300_CRG48));
}

/*****************************************************************************/
void hisfc200_get_best_clock(unsigned int *_clock)
{
	int ix, clk;
  #define CLK_2X(_clk)   (((_clk) + 1) >> 1)
	unsigned int sysclk[] = {
		CLK_2X(24),  HISFC300_CRG48_CLK_24M,
		CLK_2X(100), HISFC300_CRG48_CLK_100M,
		0, 0,
	};
  #undef CLK_2X

	clk = HISFC300_CRG48_CLK_24M;
	for (ix = 0; sysclk[ix]; ix += 2) {
		if (*_clock < sysclk[ix])
			break;
		clk = sysclk[ix+1];
	}

	*_clock = clk;
}
/*****************************************************************************/
