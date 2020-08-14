/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#include <stddef.h>
#include <asm/io.h>
#include <platform.h>
#include <cpuinfo.h>

#define TO_UINT32(_p)   (*(volatile unsigned int *)(_p))
/*****************************************************************************/
/* Use this feature, Dcache should be disable */
unsigned int get_ddr_size(void)
{
	static unsigned int ddr_size = 0;
	volatile unsigned char *memskip;
	volatile unsigned char *membase = (unsigned char *)MEM_BASE_DDR;
	unsigned int orgin = TO_UINT32(membase);
	unsigned int rd_origin = 0, rd_verify = 0;
#ifdef REG_MEM_COMB
	unsigned int tmp;
	unsigned int mem_mode;
	unsigned int mem_comb;
#endif

	if (ddr_size)
		return ddr_size;

#ifdef REG_MEM_COMB
	tmp = readl(REG_MEM_COMB);
	mem_mode = (tmp & (MEM_MODE_MASK << MEM_MODE_SHIFT));
	if (mem_mode) {
		mem_comb = (tmp >> MEM_COMB_SHIFT) & MEM_COMB_MASK;
		ddr_size = (_256M << mem_comb) + (_128M << mem_comb);
		return ddr_size;
	}
#endif

	for (memskip = membase + _16M; memskip <= membase + get_max_ddr_size();
	     memskip += _16M) {
		TO_UINT32(membase) = 0xA9A9A9A9;
		rd_origin = TO_UINT32(memskip);

		TO_UINT32(membase) = 0x53535352;
		rd_verify = TO_UINT32(memskip);

		if (rd_origin != rd_verify) {
			ddr_size = (unsigned int)(memskip - membase);
			break;
		}
		
		if (_HI3798CV100A == get_chipid()
			|| _HI3798CV100 == get_chipid()
			|| _HI3796CV100 == get_chipid()) {
			if ((unsigned int)(memskip - membase) >= (_2G - _16M)) {
				ddr_size = _2G;
				break;
			}
		}

		if ((unsigned int)(memskip - membase) >= (_3G + _512M)) {
			ddr_size = _3G + _750M;
			break;
		}
	}

	/* restore membase value. */
	TO_UINT32(membase) = orgin;
	return ddr_size;
}
