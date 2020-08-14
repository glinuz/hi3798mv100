#ifndef _LIBC_PRIVATE_H_
#define _LIBC_PRIVATE_H_

void __xprintf(const char *fmt, va_list ap,
	       void (*xputc)(unsigned n, void *cookie),
	       void *cookie);

#endif
