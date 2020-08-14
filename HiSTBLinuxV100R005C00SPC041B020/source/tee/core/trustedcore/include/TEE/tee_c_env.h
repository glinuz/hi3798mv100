/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : tee_c_env.h
 * Description : TEE application c language environment initialization
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

#ifndef _TEE_C_ENV_H
#define _TEE_C_ENV_H
#include "sre_typedef.h"
#include "sre_task.h"

#define TEE_HEAP_MOD_NO  0          //os no use
#define TEE_HEAP_PT_NO  0x2        //must be unique

typedef struct {
	UINT32 heap_size;
	UINT32 heap_boundary;
} heap_info_t;

extern UINT32 TEE_BSS_START;
extern UINT32 TEE_LIB_END;

extern void cinit00(void);

#endif//_TEE_C_ENV_H
