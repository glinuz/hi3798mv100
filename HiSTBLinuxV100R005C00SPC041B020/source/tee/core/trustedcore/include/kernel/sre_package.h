/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : sre_package.h
 * Description : header file for unpack secure img.
 * Author      : h00206996
 * Date        : 2013.5.25
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

#ifndef _SRE_PACKAGE_H
#define _SRE_PACKAGE_H

#include "sre_typedef.h"
#include "sre_task.h"

typedef struct {
	UINT32 header_size;      //Header size
	UINT32 kernel_load_addr; //RTOSCK load address
	UINT32 kernel_size;      //RTOSCK size
	UINT32 task_num;         //TASK_NUM
	UINT32 task_total_size;
	UINT32 got_size;
} secure_img_header_t;

typedef struct {
	UINT32 time_Low;
	UINT16 time_Mid;
	UINT16 timeHiAndVersion;
	UINT8 clockSeqAndNode[8];
} TASK_UUID;

typedef struct {
	UINT32 img_addr;         //task img addr
	UINT32 img_size;
	UINT32 task_size;        //task runtime allocate size
	char task_name[20];
	TASK_UUID task_uuid;
} secure_img_task_header_t;

typedef struct {
	UINT32 offset;         //symtab offset
	UINT32 symtab_size;    //symtab size
	UINT32 strtab_size;    //strtab size
	UINT32 bss_size;       //bss size
} img_symbol_header_t;

typedef struct {
	UINT32 process_start_address;
	UINT32 tee_text_start;
	UINT32 tee_got_start;
	UINT32 tee_bss_start;
	UINT32 tee_relplt_start;
	UINT32 tee_relplt_end;
	UINT32 tee_reldyn_start;
	UINT32 tee_reldyn_end;
	UINT32 tee_reladyn_start;
	UINT32 tee_reladyn_end;
	UINT32 tee_relaplt_start;
	UINT32 tee_relaplt_end;
	UINT32 tee_dynamic_start;
	UINT32 tee_dynamic_end;
	UINT32 tee_dynsym_start;
	UINT32 tee_dynsym_end;
	UINT32 tee_lib_end;
	UINT32 tee_symtab_start;
	UINT32 tee_symtab_end;
} section_info_t;


extern TSK_ENTRY_FUNC global_task_load(TSK_INIT_PARAM_S *);
extern UINT32 SRE_Dynamic_Loadelf(char *file_buffer,
				  int file_size,
				  UINT32 stack_size,
				  UINT32 heap_size,
				  section_info_t *p_section_info,
				  struct sessison_task_info *p_sti
				 );
extern UINT32 SRE_Release_Dynamic_Region(char *task_name, UINT32 *p_addr, UINT32 *vm_addr, UINT32 size);
#endif//_SRE_PACKAGE_H
