/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PLAT_DEF_H__
#define __PLAT_DEF_H__

#define HISI_PRIMARY_CPU	0x0

/* Special value used to verify platform parameters from BL2 to BL3-1 */
#define MT_BL31_PLAT_PARAM_VAL	0x0f1e2d3c4b5a6978ULL

/*******************************************************************************
 * Platform memory map related constants
 ******************************************************************************/
#define DEVICE_BASE             0xf0000000
#define DEVICE_SIZE             0x0f000000

#define DRAM_NS_BASE            0x00000000
#define DRAM_NS_SIZE            0xF0000000

#define TEE_SEC_MEM_BASE	0x70000000
#define TEE_SEC_MEM_SIZE	0x10000000

/*******************************************************************************
 * UART related constants
 ******************************************************************************/
#define HISI_UART0_BASE		(0xf8b00000)
#define HISI_BAUDRATE		(115200)

#define HISI_UART_CLOCK		(75000000)
/*******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS	24000000
#define SYS_COUNTER_FREQ_IN_MHZ		24

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/

/* Base MTK_platform compatible GIC memory map */
#define BASE_GICD_BASE		(0xf1001000)
#define BASE_GICC_BASE		(0xf1002000)
#define BASE_GICR_BASE		(0xf1000000)	/* no GICR in GIC-400 */

#define GIC_PRIVATE_SIGNALS	(32)

/* FIQ platform related define */
#define HISI_IRQ_SEC_SGI_0	8
#define HISI_IRQ_SEC_SGI_1	9
#define HISI_IRQ_SEC_SGI_2	10
#define HISI_IRQ_SEC_SGI_3	11
#define HISI_IRQ_SEC_SGI_4	12
#define HISI_IRQ_SEC_SGI_5	13
#define HISI_IRQ_SEC_SGI_6	14
#define HISI_IRQ_SEC_SGI_7	15

#define HISI_IRQ_SEC_TIMER0	60
#define HISI_IRQ_SEC_TIMER1	50
#define HISI_IRQ_SEC_TIMER2	52
#define HISI_IRQ_SEC_TIMER3	88

/*******************************************************************************
 * WDG related constants
 ******************************************************************************/
#define HISI_WDG0_BASE 0xF8A2C000


#endif /* __PLAT_DEF_H__ */
