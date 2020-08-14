#ifndef __ASM_ARCH_EARLY_DEBUG_H
#define __ASM_ARCH_EARLY_DEBUG_H

#ifdef CONFIG_DEBUG_LL

extern void printascii(const char *);
extern void printhex8(long);
extern void printch(char);

#define edb_putc(x) printch(x)
#define edb_putstr(x) printascii(x)
#define edb_puthex(x) printhex8(x)
#define edb_putul(x) printhex8(x)
#define edb_trace() do {	\
	edb_putstr(__func__);	\
	edb_putstr("\t");	\
	edb_putstr("\t");	\
	edb_putstr("[");	\
	edb_putstr(__FILE__);	\
	edb_putstr("]\n");	\
} while (0)

#else

#define edb_putc(x)
#define edb_puthex(x)
#define edb_putul(x)
#define edb_putstr(x)
#define edb_trace(level)

#endif

#endif /* __ASM_ARCH_EARLY_DEBUG_H */

