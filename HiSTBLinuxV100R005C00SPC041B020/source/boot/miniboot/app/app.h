/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Yan JianTao
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

#ifndef __APPH__
#define __APPH__

#include "cpuids.h"

#ifndef SYS_TYPESH
#define SYS_TYPESH

typedef signed char                   int8;
typedef signed short                  int16;
typedef signed int                    int32;
typedef signed long long              int64;

typedef unsigned char                 uint8;
typedef unsigned short                uint16;
typedef unsigned int                  uint32;
typedef unsigned long long            uint64;

typedef unsigned char                 u_char;
typedef unsigned int                  u_int;
typedef unsigned short                u_short;
typedef unsigned long                 u_long;

typedef uint32                        size_t;
typedef uint32                        ssize_t;
typedef uint32                        uintmax_t;
typedef int32                         intmax_t;
typedef int32                         ptrdiff_t;
typedef uint32                        uintptr_t;
typedef int64                         quad_t;
typedef uint64                        u_quad_t;

#endif /* SYS_TYPESH */

extern int sprintf(char *buf, const char *cfmt, ...);
extern int snprintf(char *str, size_t size, const char *format, ...);

#ifdef CONFIG_PRINT
extern int printf(const char *cfmt, ...);
extern int getchar(void);
extern int getc(void);
extern void putchar(int c);
extern void putc(int c);
extern int tstchar(void);
extern int isbreak(void);
extern int puts(const char *s);

#else

#  define printf(_cfmt, ...)
#  define getchar()    0
#  define getc()
#  define putchar(_c)
#  define putc(_c)
#  define tstchar()    0
#  define puts(_s)
#  define isbreak()     0
#endif

#ifndef STDARGH
#define STDARGH

typedef __builtin_va_list       va_list;

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

#endif /* STDARGH */

/* run image */
extern int image_load(const char *addr, const char *param);

/* libc */
extern void *memcpy(void *dest, const void *src, unsigned int n);
extern void *memset(void *s, int c, size_t n);

extern int memcmp(const void *b1, const void *b2, size_t len);
extern int strncmp(const char *s1, const char *s2, size_t len);
extern char *strncpy(char * dst, const char * src, size_t n);

/* malloc  */
extern void *__malloc(uint32 bytes, char *file, int line);
#define malloc(bytes) __malloc(bytes, __FILE__, __LINE__)
extern void __free(void *mem, char *file, int line);
#define free(mem) __free(mem, __FILE__, __LINE__)

/* env */
extern char *getenv(const char *key);
extern char *setenv(const char *key, const char *value);
extern char *getenv_buf(char **envbuf, int *envsize);
extern int strcmp(const char *s1, const char *s2);

/* string  */
extern size_t strlen(const char *s);
extern char *strstr(const char *s1, const char *s2);
extern void strcpy(char *dst, const char *src);
extern void strcat(char *dst, const char *src);

/* flash */
/* 1 - find, 0 - no find */
extern int flash_find_part(const char *devpartname, const char *devname,
			   const char *partname, uint64 *start, uint64 *size);
extern void *flash_open_part(uint32 flash, const char *partname);
extern void *flash_open_range(u_int flash, uint64 from,
			      uint64 length);
extern void flash_close(void *intf);
extern int64 flash_erase(void *intf, uint64 offset, uint64 length);
extern int64 flash_write(void *intf, uint64 offset, uint64 length, 
			 char *buf, int withoob);
extern int64 flash_read(void *intf, uint64 offset, uint64 length, 
			    char *buf, int withoob);
extern const char *flash_get_name(u_int type);
extern const void *flash_get_info(u_int flash);

/* cpu info */
#define BOOT_MEDIA_UNKNOWN                     (0)
#define BOOT_MEDIA_DDR                         (1)
#define BOOT_MEDIA_NAND                        (2)
#define BOOT_MEDIA_SPIFLASH                    (3)
#define BOOT_MEDIA_SPI_NAND                    (4)
#define BOOT_MEDIA_EMMC                        (5)
#define BOOT_MEDIA_SD                          (6)

extern int get_boot_media(void);

/* for ca nagra only */
extern uint32 get_ca_verify_data(uint32 *addr, uint32 *size);

extern int task_connect(uint32 msec, void (*handle)(void *args), void *args);

void *__resmalloc(uint32 bytes, char *file, int line);
#define resmalloc(_bytes) __resmalloc(_bytes, __FILE__, __LINE__)
void *__resmemalign(uint32 alignment, uint32 bytes, char *file, int line);
#define resmemalign(_alignment, _bytes) \
	__resmemalign(_alignment, _bytes, __FILE__, __LINE__)
/*
 * void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize)
 */
#define reserve_mem_alloc(_size, _allocsize) ({ \
	char *addr = NULL; uint32 __size = (_size + 4096 - 1) & ~(4096 - 1); \
		addr = __resmemalign(4096, __size, __FILE__, __LINE__); \
		*(_allocsize) = (__size); addr; })

void resfree(void *mem);

extern unsigned int get_ddr_size(void);
extern unsigned int get_timer_value(void);
extern void srand(unsigned int seed);
extern int rand(void);
extern uint64 get_chipid(void);
extern void udelay(unsigned int usec);
extern unsigned int get_ticks(void);
extern int get_board_type(void);
extern int run_cmd(char *cmdline, int length);
extern unsigned int do_gettime(unsigned long *sec, unsigned short *msec, unsigned short *usec);
#endif /* __APPH__ */
