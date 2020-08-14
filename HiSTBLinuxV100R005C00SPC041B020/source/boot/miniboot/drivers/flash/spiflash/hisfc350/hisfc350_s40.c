/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>
#include <stdio.h>
#include <stddef.h>

#include <asm/io.h>

#include "sf_drv.h"
#include "spi_ids.h"
#include "hisfc350.h"

/*****************************************************************************/

/*periph hisfc CGR23 register*/
#define HISFC350_CRG23                      (0x005C)
#define HISFC350_CRG23_RST                  (1 << 4)
#define HISFC350_CRG23_CLKEN                (1 << 0)

#define HISFC350_CFG23_CLK_SRC_OFFSET       8
#define HISFC350_CFG23_CLK_SRC_MASK         0x7 << HISFC350_CFG23_CLK_SRC_OFFSET
#define HISFC350_CRG23_CLK_24M              0x0 << HISFC350_CFG23_CLK_SRC_OFFSET
#define HISFC350_CRG23_CLK_200M             0x5 << HISFC350_CFG23_CLK_SRC_OFFSET
#define HISFC350_CRG23_CLK_150M             0x4 << HISFC350_CFG23_CLK_SRC_OFFSET
#define HISFC350_CRG23_CLK_100M             0x6 << HISFC350_CFG23_CLK_SRC_OFFSET
#define HISFC350_CRG23_CLK_75M              0x7 << HISFC350_CFG23_CLK_SRC_OFFSET

/*****************************************************************************/

void hisfc350_set_system_clock(struct hisfc_host *host,
			       struct spi_operation *op,
			       int clk_en)
{
	unsigned int regval = HISFC350_CRG23_CLK_24M;

	if (op && op->clock) {
		regval = op->clock & HISFC350_CFG23_CLK_SRC_MASK;
		if (regval == HISFC350_CRG23_CLK_200M)
			regval = HISFC350_CRG23_CLK_150M;
	}

	if (clk_en)
		regval |= HISFC350_CRG23_CLKEN;

	if (readl(host->cfgreg + HISFC350_CRG23) != regval)
		writel(regval, (host->cfgreg + HISFC350_CRG23));
}

/*****************************************************************************/
void hisfc350_get_best_clock(unsigned int * clock)
{
	int ix,clk;
	unsigned int sysclk[] = {
		38,  HISFC350_CRG23_CLK_75M,
		50, HISFC350_CRG23_CLK_100M,
		75, HISFC350_CRG23_CLK_150M,
		100, HISFC350_CRG23_CLK_200M,
		0,0,
	};

	clk = HISFC350_CRG23_CLK_24M;
	for (ix = 0; sysclk[ix]; ix += 2) {
		if ((*clock) < sysclk[ix])
			break;
		clk = sysclk[ix+1];
	}
	(*clock) = clk;
}
/*****************************************************************************/
#ifdef CONFIG_HISFC350_SHOW_CYCLE_TIMING
static char * hisfc350_get_clock_str(unsigned int clk_reg)
{
	static char buffer[40];
	int ix;

	unsigned int clk_str[] = {
		0, 12,         /* 0x000 : 24M/2 */
		1, 12,         /* 0x000 : 24M/2 */
		2, 12,         /* 0x000 : 24M/2 */
		3, 12,         /* 0x000 : 24M/2 */
		4, 100,        /* 0x100 : 150M/2 */
		5, 75,         /* 0x101 : 200M/2 */
		6, 50,         /* 0x110 : 100M/2 */
		7, 38,         /* 0x111 : 75M/2 */
	};

	clk_reg = ((clk_reg & HISFC350_CFG23_CLK_SRC_MASK) >>
		HISFC350_CFG23_CLK_SRC_OFFSET);

	for (ix = 0; clk_str[ix] < 8; ix += 2) {
		if (clk_reg == clk_str[ix]) {
			sprintf(buffer, "%dM", clk_str[ix+1]);
			break;
		}
	}
	return buffer;
}
#endif /* CONFIG_HISFC350_SHOW_CYCLE_TIMING */
