/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
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

#include <arm_gic.h>
#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <bl31.h>
#include <bl_common.h>
#include <console.h>
#include <cortex_a57.h>
#include <cortex_a53.h>
#include <debug.h>
#include <errno.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <hisi_def.h>
#include <stddef.h>
#include <hisi_private.h>
#include <string.h>

unsigned long __RO_START__;
unsigned long __RO_END__;

unsigned long __COHERENT_RAM_START__;
unsigned long __COHERENT_RAM_END__;

/*
 * The next 2 constants identify the extents of the code & RO data region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __RO_START__ and __RO_END__ linker symbols refer to page-aligned addresses.
 */
#define BL31_RO_BASE (unsigned long)(&__RO_START__)
#define BL31_RO_LIMIT (unsigned long)(&__RO_END__)

/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols
 * refer to page-aligned addresses.
 */
#define BL31_COHERENT_RAM_BASE (unsigned long)(&__COHERENT_RAM_START__)
#define BL31_COHERENT_RAM_LIMIT (unsigned long)(&__COHERENT_RAM_END__)


static entry_point_info_t bl33_image_ep_info, bl32_image_ep_info;

/*******************************************************************************
 * This variable holds the non-secure image entry address
 ******************************************************************************/
extern uint64_t ns_image_entrypoint;

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for
 * security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type.
 ******************************************************************************/
#if DISABLE_TEE == 1
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	return &bl33_image_ep_info;
}
#else
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	if (type == NON_SECURE)
		return &bl33_image_ep_info;

	if (type == SECURE)
		return &bl32_image_ep_info;

	return NULL;
}
#endif

/*******************************************************************************
 * Perform any BL31 specific platform actions. Populate the BL33 and BL32 image
 * info.
 ******************************************************************************/
void bl31_early_platform_setup(bl31_params_t *from_bl2,
				void *plat_params_from_bl2)
{

	/*
	 * Configure the UART port to be used as the console
	 */
	console_init(HISI_UART0_BASE, HISI_UART_CLOCK,
			HISI_BAUDRATE);

	/* Initialise crash console */
	plat_crash_console_init();

#if 1
	/*
	 * Copy BL3-3, BL3-2 entry point information.
	 * They are stored in Secure RAM, in BL2's address space.
	 */
	bl33_image_ep_info = *from_bl2->bl33_ep_info;
	bl32_image_ep_info = *from_bl2->bl32_ep_info;

#if DISABLE_TEE == 1
	SET_SECURITY_STATE(bl33_image_ep_info.h.attr,
			SECURE);
#else
	SET_SECURITY_STATE(bl33_image_ep_info.h.attr,
			NON_SECURE);

#endif

#else

	/* fix me later , pass the addr from uboot*/
	bl32_image_ep_info.pc = (uintptr_t)0x7e008000;
	SET_SECURITY_STATE(bl32_image_ep_info.h.attr,
			SECURE);

	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL32 image.
	 */
	bl32_image_ep_info.spsr = 0;

	bl33_image_ep_info.pc = (uintptr_t)0x1080000 ;
	/* pass dtb address */
	bl33_image_ep_info.args.arg0 = (uintptr_t)0x2000000 ;
	bl33_image_ep_info.spsr = SPSR_64(MODE_EL1,
			MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
	SET_SECURITY_STATE(bl33_image_ep_info.h.attr,
			NON_SECURE);

#endif
}

/*******************************************************************************
 * Initialize the gic, configure the SCR.
 ******************************************************************************/
#if DISABLE_TEE == 1
void bl31_platform_setup(void){
	plat_delay_timer_init();

#if 1
        __asm volatile("mrs     x0, cnthctl_el2\n"
                        "orr     x0, x0, #0x3\n"
                        "msr     cnthctl_el2, x0\n"
                        "msr    cntvoff_el2, xzr\n"
                        "msr     cntvoff_el2, x0\n"
                        "orr     x0, x0, #0x3\n"
                        "msr     cntkctl_el1, x0\n");

#endif
}
#else
void bl31_platform_setup(void)
{
         plat_delay_timer_init();

         /* Initialize the gic cpu and distributor interfaces */
         plat_hisi_gic_init();
         arm_gic_setup();
}
#endif
#if 0
extern void tf_printf(const char *fmt, ...);
static void dump_mem(char *phyaddr, uint32_t size)
{
        char *p = 0;

        if (phyaddr == NULL) {
                ERROR("%s:Invalid addr!\n", __func__);
                return;
        }

        p = phyaddr;
        while (p < (phyaddr + size)) {
                if (!((p - phyaddr) % 16) && ((p - phyaddr) != 0)) {
                        tf_printf("\n");
                        INFO("0x%x: %x ", p, *((unsigned int *)p));
                } else if ((p - phyaddr) == 0) {
                        INFO("0x%x: %x ", p, *((unsigned int *)p));
                } else
                        tf_printf("%x ", *((unsigned int *)p));

                p+=4;
        }

	tf_printf("\n");
	tf_printf("\n");
}
#endif
/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this only intializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl31_plat_arch_setup(void)
{
//#define BL33_LOAD_ADDR 0x80000
//#define BL33_SIZE 0x2000000

//	uintptr_t bl33_dst = BL33_LOAD_ADDR;
	uintptr_t fdt = bl33_image_ep_info.args.arg0;
	uintptr_t bl33_dst = bl33_image_ep_info.args.arg1;
	uintptr_t bl33_src = bl33_image_ep_info.pc;
	uint64_t bl33_size = bl33_image_ep_info.args.arg2;
	uintptr_t atags_end = bl33_image_ep_info.args.arg4;
	uint64_t fdt_max_size = bl33_image_ep_info.args.arg5;

	configure_mmu_el3(BL31_RO_BASE,
			(BL31_COHERENT_RAM_LIMIT - BL31_RO_BASE),
			BL31_RO_BASE,
			BL31_RO_LIMIT,
			BL31_COHERENT_RAM_BASE,
			BL31_COHERENT_RAM_LIMIT);

	INFO("Move bl33 from 0x%lx to 0x%lx, %lu Bytes\n", bl33_src, bl33_dst, bl33_size);
	memmove((void *)bl33_dst, (void *)bl33_src, bl33_size);
	flush_dcache_range(bl33_dst, bl33_size);
	bl33_image_ep_info.pc = bl33_dst;
	/*
	  The arm64 kernel require that the device tree blob (dtb) must be placed
	  on an 8-byte boundary within the first 512 megabytes from the start of
	  the kernel image and must not cross a 2-megabyte boundary. This is to
	  allow the kernel to map the blob using a single section mapping in the
	  initial page tables. To simplify the implemention, we fix the dtb location
	  to the tail of TAG params area.
	*/
	INFO("Move dtb from 0x%lx to 0x%lx, %lu Bytes\n", fdt, atags_end, fdt_max_size);
	memmove((void *)atags_end, (void *)fdt, fdt_max_size);
	flush_dcache_range(atags_end, fdt_max_size);
	bl33_image_ep_info.args.arg0 = atags_end;
}

