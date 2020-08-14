#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#define CONFIG_PHYS_OFFSET	UL(0x0)

/* ddr max size */
#define PLAT_PHYS_END		UL(0xC0000000)

/*
 * sdk use mmap + /dev/mem widely, by default mmap("/dev/mem")
 * will map non-system memory(!pfn_valid) as strong order,
 * but we want to chang it to normal non-cached, because of
 * 1) normal non-cached has a better performance than strong order
 * 2) unaligned access(i.e. in software decoder) will fault in
 *     data abort exception for ARM architecture reason.
 * see phys_mem_access_prot() in /driver/char/mem.c
 */
#define is_ddr(phys)	((phys) >= CONFIG_PHYS_OFFSET && (phys) <= PLAT_PHYS_END)

#endif /* __ASM_ARCH_MEMORY_H */
