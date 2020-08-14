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

#ifndef STDIOH
#define STDIOH
/******************************************************************************/
#include <config.h>
#include <stddef.h>

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

#  include <early_uart.h>
#  define early_printf uart_early_printf
#  define early_puts   uart_early_puts
#else

#  define printf(_cfmt, ...)
#  define getchar()     0
#  define getc()
#  define putchar(_c)
#  define putc(_c)
#  define tstchar()     0
#  define puts(_s)
#  define isbreak()     0
#  define early_printf(_cfmt, ...)
#  define early_puts(_ss)
#endif

/* printf message for tool */
#define pr_tool printf

char *u64tohstr(const uint64 size, char *unit);
char *u32tohstr(const uint32 size, char *unit);
char *u32tohhstr(uint32 size);
char *u64tohhstr(uint64 size);
char *ultohstr(unsigned long long size);
/******************************************************************************/
#endif /* STDIOH */
