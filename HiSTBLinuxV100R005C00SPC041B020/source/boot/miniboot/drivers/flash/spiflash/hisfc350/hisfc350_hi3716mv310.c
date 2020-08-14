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

#include "spi_ids.h"
#include "hisfc350.h"

/* periph hisfc CGR23 register for Hi3716MV310*/
#define HI3716MV310_HISFC350_CRG23                              (0x005C)
#define HI3716MV310_HISFC350_CRG23_RST                          (1 << 4)
#define HI3716MV310_HISFC350_CRG23_CLK2X_RST                    (1 << 3)
#define HI3716MV310_HISFC350_CRG23_CLKEN                        (1 << 0)
#define HI3716MV310_HISFC350_CRG23_CLK2X_CLKEN                  (1 << 5)

#define HI3716MV310_HISFC350_CRG23_CLK_12M                      0x000000
#define HI3716MV310_HISFC350_CRG23_CLK_37M                      0x000700
#define HI3716MV310_HISFC350_CRG23_CLK_50M                      0x000600
#define HI3716MV310_HISFC350_CRG23_CLK_75M                      0x000400
#define HI3716MV310_HISFC350_CRG23_CLK_100M                     0x000500
#define HI3716MV310_HISFC350_CFG23_CLK_SRC_OFFSET               8

/* periph hisfc CGR31 register for Hi3716mv300 */
#define HI3716MV310_HISFC350_CRG23                 (0x005C)
#define HI3716MV310_HISFC350_CRG23_RST             (1 << 4)
#define HI3716MV310_HISFC350_CRG23_CLK2X_RST       (1 << 3)
#define HI3716MV310_HISFC350_CRG23_CLKEN           (1 << 0)
#define HI3716MV310_HISFC350_CRG23_CLK2X_CLKEN     (1 << 5)
#define HI3716MV310_HISFC350_CFG23_CLK_SRC_OFFSET   8

#define HI3716MV310_HISFC350_CRG23_CLK_12M          0x000000
#define HI3716MV310_HISFC350_CRG23_CLK_37M          0x000700
#define HI3716MV310_HISFC350_CRG23_CLK_50M          0x000600
#define HI3716MV310_HISFC350_CRG23_CLK_75M          0x000400
#define HI3716MV310_HISFC350_CRG23_CLK_100M         0x000500

/*****************************************************************************/

void hisfc350_set_system_clock(struct hisfc_host *host,
			       struct spi_operation *op,
			       int clk_en)
{

	unsigned int regval = HI3716MV310_HISFC350_CRG23_CLK_12M;
	if (op && op->clock)
		regval = (op->clock & 0x700);

	if (clk_en)
		regval |= (HI3716MV310_HISFC350_CRG23_CLKEN 
			   | HI3716MV310_HISFC350_CRG23_CLK2X_CLKEN);

	if (readl(host->cfgreg + HI3716MV310_HISFC350_CRG23 ) != regval)
		writel(regval, (host->cfgreg + HI3716MV310_HISFC350_CRG23));
}

/*****************************************************************************/
void hisfc350_get_best_clock(unsigned int * clock)
{
	int ix,clk;
	unsigned int sysclk[] = {
		12,  HI3716MV310_HISFC350_CRG23_CLK_12M,
		38,  HI3716MV310_HISFC350_CRG23_CLK_37M,
		50,  HI3716MV310_HISFC350_CRG23_CLK_50M,
		75,  HI3716MV310_HISFC350_CRG23_CLK_75M,
		100, HI3716MV310_HISFC350_CRG23_CLK_100M,
		0,0,
	};

	clk = HI3716MV310_HISFC350_CRG23_CLK_12M;
	for (ix = 0; sysclk[ix] != 0; ix += 2) {
		if (*clock < sysclk[ix])
			break;
		clk = sysclk[ix+1];
	}

	*clock = clk;
}
/*****************************************************************************/
#ifdef CONFIG_HISFC350_SHOW_CYCLE_TIMING
static char * hisfc350_get_clock_str(unsigned int clk_reg)
{
	static char buffer[40];
	int ix;

	static unsigned int clk_str[] = {
		0, 12,
		1, 12,
		2, 12,
		3, 12,
		4, 75,         /* 0x100 : 75M */
		5, 100,        /* 0x101: 100M */
		6, 50,         /* 0x110 : 50M */
		7, 37,         /* 0x111 : 37M */
	};

	clk_reg = (clk_reg & 0x700) >> HI3716MV310_HISFC350_CFG23_CLK_SRC_OFFSET;

	for (ix = 0; clk_str[ix] < 8; ix += 2) {
		if (clk_reg == clk_str[ix]) {
			snprintf(buffer, sizeof(buffer), "%dM",
				 clk_str[ix+1]);
			break;
		}
	}
	return buffer;
}
#endif /* CONFIG_HISFC350_SHOW_CYCLE_TIMING */
