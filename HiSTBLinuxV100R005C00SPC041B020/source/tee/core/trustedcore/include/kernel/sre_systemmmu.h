#include <sre_base.h>
#include <sre_arm_mmu.h>
#include "sre_task.h"
#include <sre_securemmu.h>

#ifndef __SRE_SYSTEMMMU_H__
#define __SRE_SYSTEMMMU_H__

typedef struct tagTskCB TSK_CB_S;


void mmu_init(void);
void mmu_insert_pt0(paddr_t addr);
void mmu_insert_pt1(paddr_t addr);
void mmu_enable_virt_addr(void);
void mmu_disable_virt_addr(void);
void change_pte_access(pgd_t pgd, UINT32 va, UINT32 ap);

/* Setup task memory before load */
UINT32 SRE_SMemPrepareTaskLoad(TSK_CB_S *pstTask, pgd_t service_table, UINT32 addr, UINT32 size);
UINT32 SRE_SMemPrepareTaskDelete(TSK_CB_S *pstTask, UINT32 uwTaskPID);

/* Task shared memory allocation */
VOID *SRE_MemAllocShared(UINT32 uwSize);
UINT32 SRE_MemFreeShared(VOID *pAddr);

#endif //__SRE_SYSTEMMMU_H__
