/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : backtraces.h
 * Description : backtraces dump
 * Author      : w00206976
 * Date        : 2015.3.24
 * Notes       :
 *
 *------------------------------------------------------------------------------
 * Modifications:
 *   Date        Author          Modifications
 *******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
 *******************************************************************************/
#ifndef BACKTRACES_H_
#define BACKTRACES_H_

#include "sre_task.h"

typedef struct task_symtab_entry {
	char  task_name[OS_TSK_NAME_LEN];
	u32   task_addr;
	u32   lib_symtab_addr;
	u32   lib_symtab_size;
	u32   lib_strtab_addr;
	u32   lib_strtab_size;
	u32   symtab_addr;
	u32   strtab_addr;
	u32   symtab_size;
	u32   strtab_size;
	struct task_symtab_entry *next;
} task_symtab_entry;


void add_to_tselist(task_symtab_entry *entry);
void del_from_tselist(char *task_name);
void dump_backtraces(UINT32 reg_fp, UINT32 reg_lr, UINT32 isException);
extern void uart_printf_func(const char *fmt, ...);

#endif
