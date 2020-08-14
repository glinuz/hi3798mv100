/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	.text :
	{
		__text_start = .;
		INCLUDE platform.lds
		*(.text)
	}
	__text_end = .;
	. = . + 0x84;
	. = ALIGN(4);
	__init_start = .;
	.init : { *(.init) }
	__init_end = .;

	. = ALIGN(4);
	__cmd_start = .;
	._cmd : { *(._cmd) }
	__cmd_end = .;

	. = ALIGN(4);
	.symbol : { *(.symbol) }

	. = ALIGN(4);
	.image : { *(.image) }

	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	.got : { *(.got) }

	.tail :
	{
		arm/hi3798mv2x/boot/tail.o	(.tail)
	}

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;

	. = ALIGN(0x100000);
	__page_table_start = .;
	. = . + 0x4000; /* 16K */
	__page_table_end = .;

	. = ALIGN(4);
	__irq_stack_start = .;
	. = . + 0x20000; /* reserve 128K */
	__irq_stack_end = .;

	. = ALIGN(0x100000);
	__end = .;
}
