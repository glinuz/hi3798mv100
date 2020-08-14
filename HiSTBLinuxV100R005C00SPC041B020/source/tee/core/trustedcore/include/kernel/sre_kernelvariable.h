/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : sre_kernelvariable.h
 * Description : define varibales used both in kernel and monitor.
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

#ifndef _SRE_KERNELVARIABLE_H
#define _SRE_KERNELVARIABLE_H
#include "sre_typedef.h"
#include "sre_securemmu.h"

typedef struct {
	UINT32 tzbsp_fiq;
	UINT32 valid_params_flag;
	UINT32 secure_uart_enable;
	UINT32 params_stack[8];
	UINT32 params_out_stack[8];
} kernel_shared_varibles_t;
extern kernel_shared_varibles_t k_variables;

extern void SRE_PullKernelVariables(kernel_shared_varibles_t *pVar);
extern void SRE_UpdateKernelVariables(kernel_shared_varibles_t *pVar);
extern void SRE_PushKernelVariables(kernel_shared_varibles_t *pVar);
extern void SRE_PushRdrMemAddr(paddr_t addr, UINT32 size);

#endif//_SRE_KERNELVARIABLE_H
