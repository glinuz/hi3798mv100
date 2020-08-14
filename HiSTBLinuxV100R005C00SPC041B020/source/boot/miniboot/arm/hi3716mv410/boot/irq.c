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

#include <stdio.h>
#include <boot.h>

/*****************************************************************************/

void do_undefined_instruction (void)
{
	early_puts("undefined instruction\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_software_interrupt (void)
{
	early_puts("software interrupt\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_prefetch_abort (void)
{
	early_puts("prefetch abort\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_data_abort (void)
{
	early_puts("data abort\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_not_used (void)
{
	early_puts("not used\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_fiq (void)
{
	early_puts("fast interrupt request\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}

void do_irq (void)
{
	early_puts("interrupt request\r\n");
	early_puts("Resetting CPU ...\r\n");
	reset_cpu (0);
}
