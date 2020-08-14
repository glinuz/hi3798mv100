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
#include <stddef.h>

#include <asm/io.h>

#include "nand_drv.h"
#include "hinfc504.h"

#define S40_HINFC600_PERI_CRG_REG_BASE      (REG_BASE_CRG)
#define S40_PERI_CRG24                      (S40_HINFC600_PERI_CRG_REG_BASE + 0x60)

#define S40_PERI_CRG24_CLK_EN               (0x1U << 0)

#define S40_PERI_CRG24_CLK_SEL_MASK         (0x7U << 8)
#define S40_PERI_CRG24_CLK_SEL_24M          (0x0U << 8)
#define S40_PERI_CRG24_CLK_SEL_200M         (0x4U << 8)
#define S40_PERI_CRG24_CLK_SEL_150M         (0x5U << 8)
#define S40_PERI_CRG24_CLK_SEL_100M         (0x6U << 8)
#define S40_PERI_CRG24_CLK_SEL_75M          (0x7U << 8)

#define S40_PERI_CRG24_NF_SRST_REQ          (0x1U << 4)

/******************************************************************************/

void hinfc504_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned long reg_val;

	reg_val = readl(S40_PERI_CRG24);

	reg_val &= ~S40_PERI_CRG24_CLK_SEL_MASK;

	if (enable) {
		reg_val |= (S40_PERI_CRG24_CLK_EN
			| S40_PERI_CRG24_CLK_SEL_200M);
	} else {
		reg_val &= ~S40_PERI_CRG24_CLK_EN;
	}

	reg_val &= ~S40_PERI_CRG24_NF_SRST_REQ;

	writel(reg_val, S40_PERI_CRG24);
}

