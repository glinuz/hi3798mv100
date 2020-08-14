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

#ifndef BOOTH
#define BOOTH
/******************************************************************************/

#include <config.h>
#include <stddef.h>

typedef int (*fn_init)(void);

extern fn_init cmdline_entry;

extern unsigned int _bss_start;
extern unsigned int _bss_end;
extern unsigned int _text_end;
extern unsigned int _blank_zone_start;
extern unsigned int _blank_zone_end;

extern char __text_start[];
extern char __text_end[];
extern char __bss_start[];
extern char __bss_end[];
extern char __irq_stack_start[];
extern char __irq_stack_end[];
extern char *__global_start;
extern char *__global_end;
extern char __page_table_start[];
extern char __page_table_end[];

extern char __end[];

/* arm system function */
extern void reboot(void);
extern void reset_cpu(unsigned long addr);
extern long long get_chipid_reg(void);
extern unsigned int get_ca_vendor_reg(void);

/* cpu.c */
extern unsigned int get_ca_vendor(void);

extern unsigned int get_timer_clock(void);

unsigned int get_ddr_size(void);

void start_cmdline(void);

int boot_start(void);
void app_init(void);
void uart_init(void *ptr);
void sysmem_init(void);
/******************************************************************************/
#endif /* BOOTH */
