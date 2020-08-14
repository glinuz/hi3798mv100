#ifndef __ASM_MEM_H__
#define __ASM_MEM_H__

#define lspull          lsr
#define lspush          lsl

/*
 * Data preload for architectures that support it
 */
//#if __LINUX_ARM_ARCH__ >= 5
#define PLD(code...)	code
//#else
//#define PLD(code...)

/*
 * This can be used to enable code to cacheline align the destination
 * pointer when bulk writing to memory.  Experiments on StrongARM and
 * XScale didn't show this a worthwhile thing to do when the cache is not
 * set to write-allocate (this would need further testing on XScale when WA
 * is used).
 *
 * On Feroceon there is much to gain however, regardless of cache mode.
 */
//#ifdef CONFIG_CPU_FEROCEON
#define CALGN(code...) code
//#else
//#define CALGN(code...)
//#endif

#define __ALIGN		.align 4,0x90

#define ALIGN __ALIGN


#ifndef ENTRY
#define ENTRY(name) \
  .globl name; \
  ALIGN; \
  name:
#endif


#ifndef END
#define END(name) \
  .size name, .-name
#endif

#define ENDPROC(name) \
  .type name, %function; \
  END(name)

#endif
