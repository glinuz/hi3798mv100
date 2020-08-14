/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : sre_taskmem.h
 * Description : task memory allocator.
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


#ifndef _SRE_TASKMEM_H
#define _SRE_TASKMEM_H
#include "sre_securemmu.h"


/* For ARM LPAE the block size is defined at 2MB so the task shift
 * must be 2MB otherwise on non LPAE system the task block size is 1MB*/
#ifdef ARM_PAE
#define TASK_MEM_SHIFT 21 //2M size
/* 10MB virtual space for task, 5 L1 ptes */
#define TASK_L1_TABLES  72
#else
#define TASK_MEM_SHIFT 20 //1M size
/* 10MB virtual space for task, 10 L1 ptes */
#define TASK_L1_TABLES  144

#endif

/* Number of bits used for TASK ASLR */
#define TASK_ASLR_WIDTH 8
#define TASK_ASLR_MASK ((1 << TASK_ASLR_WIDTH) - 1)
#define TASK_MEM_BLOCK_SIZE (1<<TASK_MEM_SHIFT)


#define TA_BASE_ADDRESS         0x10000000
#define TA_LOAD_ADDRESS         (0x10000000 + TASK_MEM_BLOCK_SIZE)
#define TRANSLATE_TA_ADDR(addr) ( TA_BASE_ADDRESS + (UINT32)(addr))
#define TRANSLATE_P_TA_ADDR(addr) ((UINT32)(addr) - TA_BASE_ADDRESS)


extern void *SRE_TaskMem_Malloc(UINT32 size, UINT32 hint);
extern void SRE_TaskMem_Free(UINT32 *addr);
extern void SRE_TaskMem_Init(void);
extern UINT32 SRE_TaskMem_GetOrder(UINT32 size);
extern UINT32 SRE_TaskMem_GetBase(UINT32 *tasknum);
extern VOID SRE_TaskMem_GetInternalTaskInfo(UINT32 *addr, UINT32 start_index, UINT32 num);
extern int phy_addr_check(paddr_t  phy_addr, UINT32 size);
extern int nomap_phy_addr_check(u64 phy_addr, UINT32 size);

#endif//_SRE_TASKMEM_H
