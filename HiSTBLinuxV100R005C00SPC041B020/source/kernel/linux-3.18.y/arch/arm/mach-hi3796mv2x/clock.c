/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.5.28
 *
******************************************************************************/

#include <linux/clkdev.h>
#include <linux/clk-private.h>

#ifdef CONFIG_HI3796MV2X_FPGA
#  define UARTx_CLK 54000000
#else
#  define UARTx_CLK 75000000
#endif

void __init hi3796mv2x_clock_init(void)
{
	struct clk *clk;

	clk = clk_register_fixed_rate(NULL, "timer2", NULL, CLK_IS_ROOT, 24000000);
	clk_register_clkdev(clk, "timer2", NULL);

	clk = clk_register_fixed_rate(NULL, "timer3", NULL, CLK_IS_ROOT, 24000000);
	clk_register_clkdev(clk, "timer3", NULL);

	clk = clk_register_fixed_rate(NULL, "uart:0", NULL, CLK_IS_ROOT, UARTx_CLK);
	clk_register_clkdev(clk, NULL, "uart:0");

	clk = clk_register_fixed_rate(NULL, "uart:2", NULL, CLK_IS_ROOT, UARTx_CLK);
	clk_register_clkdev(clk, NULL, "uart:2");
}
