/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpDqgHNYPPYlCl/pP8CHYYltgiRqk3H0yIbFuqar
+eidwWHrscvIyTQFGLJ7MSYiaAtWRLuOWhTnJc0oz1ZcngrG3v11YTLpNG5IdZOhK0TNEm/J
8BTo/Drh0Wqvvh/Ku/UvA7rYh+aM75kskRmfh+ZEKHGHIPv0qNWwqLtEqIwVgg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_normal.c
 *
 * Purpose: vdh hardware abstract layer normal file
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */


#include "public.h"
#include "vdh_normal.h"
#include "vdh_module.h"
#include "vfmw_osal.h"
#include "vfmw_proc.h"



/*============== LOCAL DEFINITION =============*/
#define VDH_PROC_NAME        "vfmw_hal"
#define VDH_CMD_LEN          (256)

#define VDH_CMD_HAL          "hal"


/*================= LOCAL TYPE ================*/
typedef enum
{
	DISTRIBUTOR_SLEEPING = 0,
	DISTRIBUTOR_RUNNING,
	DISTRIBUTOR_EXITED,
}DISTRIBUTOR_STATE_E;

typedef enum
{
	CMD_SLEEP = 0,
	CMD_RUN,
	CMD_EXIT,
}DISTRIBUTOR_CMD_E;

typedef struct
{
    UINT8                   InUsed;
    OSAL_LIST_HEAD          Node;
    TASK_PARAM_S            Task;
}TASK_NODE_S;

typedef struct
{
    UINT32                  TaskNum;
    TASK_NODE_S             TaskNode;
    OSAL_LIST_HEAD          TaskList;
    OSAL_RW_LOCK            TaskLock;
}TASK_LIST_S;

typedef struct
{
    TASK_LIST_S             NormalScdTask[SCD_MODULE_NUM];
    TASK_LIST_S             NormalMfdeTask[MFDE_MODULE_NUM];

    DISTRIBUTOR_CMD_E       DistributorCommand;
    DISTRIBUTOR_STATE_E     DistributorState;
    OSAL_TASK               DistributorThread;
    OSAL_EVENT_L            ThreadEvent;

    VDH_COMMON_CTX_S       *pCommonContext;

}VDH_NORMAL_CTX_S;


/*================ LOCAL MACRO =================*/
#define IS_DISTRIBUTOR_ALIVE(state)   (DISTRIBUTOR_SLEEPING==state || DISTRIBUTOR_RUNNING==state)
#define IS_WAKE_UP_BY(type, id)       (type & (1<<id))


/*================ LOCAL VALUE =================*/
SINT32            g_VDHInitCount = 0;
VDH_NORMAL_CTX_S  NormalCtx;
MEM_RECORD_S      g_HalMemRecord;


/*=============== EXTERN VALUE =================*/


/*============== INTERNAL FUNCTION =============*/

/******************************************
 Show thread state
******************************************/
inline const char* VDH_Show_Thread_State(DISTRIBUTOR_STATE_E state)
{
	switch (state)
    {
       case DISTRIBUTOR_SLEEPING:
            return "Zzz";

       case DISTRIBUTOR_RUNNING:
            return "Running";

       case DISTRIBUTOR_EXITED:
            return "Exited";

       default:
            return "Unkown";
	}
}


inline const char* VDH_Show_Thread_Command(DISTRIBUTOR_CMD_E cmd)
{
	switch (cmd)
    {
       case CMD_SLEEP:
            return "Sleep";

       case CMD_RUN:
            return "Run";

       case CMD_EXIT:
            return "Exit";

       default:
            return "Unkown";
	}
}

/******************************************
 Show module state
******************************************/
inline const char* VDH_Show_Module_State(MODULE_STATE_E state)
{
	switch (state)
    {
       case MODULE_IDLE:
            return "Idle";

       case MODULE_NORMAL_WORKING:
            return "Normal";

       case MODULE_SECURE_WORKING:
            return "Secure";

       default:
            return "Unkown";
	}
}

/******************************************
 Alloc VDH Distributor Common buffer
******************************************/
SINT32 VDH_Alloc_ComBuffer(VOID)
{
    UINT32 ComSize = sizeof(VDH_COMMON_CTX_S);

    memset(&g_HalMemRecord, 0, sizeof(MEM_RECORD_S));
#ifdef HI_SMMU_SUPPORT
    g_HalMemRecord.eMode = MEM_MMU_MMU;
#else
    g_HalMemRecord.eMode = MEM_CMA_CMA;
#endif

    if( MEM_MAN_OK != MEM_AllocMemBlock("VDH_COM", 0, ComSize, &g_HalMemRecord, 0) )
    {
        dprint(PRN_ERROR,"%s %d, alloc vdh hal ctx mem failed, size = %d\n", __func__, __LINE__, ComSize);
        return VDH_ERR;
    }
    NormalCtx.pCommonContext = (VDH_COMMON_CTX_S *)g_HalMemRecord.VirAddr;

    memset(NormalCtx.pCommonContext, 0, sizeof(VDH_COMMON_CTX_S));

    return VDH_OK;
}

/******************************************
 Free VDH Distributor Common buffer
******************************************/
VOID VDH_Free_ComBuffer(VOID)
{
    MEM_ReleaseMemBlock(g_HalMemRecord.PhyAddr, g_HalMemRecord.VirAddr);

    NormalCtx.pCommonContext = NULL;

    return;
}

/******************************************
 Select a task to process
******************************************/
SINT32 VDH_GetTask_To_Process(MODULE_TYPE_E Type, SINT32 ModuleID, TASK_PARAM_S *pTaskParam)
{
    SINT32 ret = VDH_ERR;
    UINT32 NormalWaitTime = 0, SecureWaitTime = 0;
    TASK_NODE_S    *pTaskNode  = NULL;
    TASK_NODE_S    *pTempNode  = NULL;
    TASK_LIST_S    *pTaskList  = NULL;
    TASK_MEM_S     *pTaskMem   = NULL;
    TASK_ARRAY_S   *pTaskArray = NULL;

    switch(Type)
    {
        case MODULE_SCD:
            pTaskList  = &NormalCtx.NormalScdTask[ModuleID];
            pTaskArray = &NormalCtx.pCommonContext->SecureScdArray[ModuleID];
            break;

        case MODULE_MFDE:
            pTaskList  = &NormalCtx.NormalMfdeTask[ModuleID];
            pTaskArray = &NormalCtx.pCommonContext->SecureMfdeArray[ModuleID];
            break;

        default:
            dprint(PRN_ERROR, "%s unkown module type %d!\n", __func__, Type);
            return VDH_ERR;
    }

    if (pTaskArray->Tail != pTaskArray->Head)
    {
        pTaskMem = &pTaskArray->Task[pTaskArray->Tail];
        if (1 == pTaskMem->InUsed)
        {
            if (0 == pTaskMem->Task.StartTime)
            {
                pTaskMem->Task.StartTime = OSAL_GetTimeInMs();
                SecureWaitTime = 1;
            }
            else
            {
                SecureWaitTime = OSAL_GetTimeInMs() - pTaskMem->Task.StartTime;
            }
        }
    }

    OSAL_WriteLockIRQ(&pTaskList->TaskLock);
    if (!OSAL_LIST_EMPTY(&pTaskList->TaskList))
    {
        OSAL_LIST_FOR_EACH_ENTRY_SAFE(pTaskNode, pTempNode, &pTaskList->TaskList, Node)
        {
            if (1 == pTaskNode->InUsed)
            {
                if (0 == pTaskNode->Task.StartTime)
                {
                    pTaskNode->Task.StartTime = OSAL_GetTimeInMs();
                    NormalWaitTime = 1;
                }
                else
                {
                    NormalWaitTime = OSAL_GetTimeInMs() - pTaskNode->Task.StartTime;
                }
                if (NormalWaitTime > SecureWaitTime)
                {
                    OSAL_LIST_DEL(&pTaskNode->Node);
                    pTaskNode->InUsed  = 0;
                    pTaskList->TaskNum = (pTaskList->TaskNum-1 > 0)? (pTaskList->TaskNum-1): 0;
                    memcpy(pTaskParam, &pTaskNode->Task, sizeof(TASK_PARAM_S));
                    ret = VDH_OK;
                }
                break;
            }
        }
    }
    OSAL_WriteUnLockIRQ(&pTaskList->TaskLock);

    if (ret != VDH_OK)
    {
        if (pTaskArray->Tail != pTaskArray->Head)
        {
            pTaskMem = &pTaskArray->Task[pTaskArray->Tail];
            if (1 == pTaskMem->InUsed)
            {
                memcpy(pTaskParam, &pTaskMem->Task, sizeof(TASK_PARAM_S));
                pTaskMem->InUsed    = 0;
                pTaskArray->TaskNum = (pTaskArray->TaskNum-1 > 0)? (pTaskArray->TaskNum-1): 0;
                pTaskArray->Tail    = (pTaskArray->Tail+1) % MAX_TASK_NUM;
                ret = VDH_OK;
            }
        }
    }

    if (VDH_OK == ret)
    {
        dprint(PRN_HAL, "Start %s Task: NormalWait %d, SecureWait %d\n", (NormalWaitTime>SecureWaitTime)? "Normal": "Secure", NormalWaitTime, SecureWaitTime);
    }

    return ret;
}


/******************************************
 Process the scd task
******************************************/
VOID VDH_Process_ScdTask(SINT32 ModuleID)
{
    SINT32 ret;
    VDH_COMMON_CTX_S *pCtx    = NULL;
    MODULE_DEC_S     *pModule = NULL;
    TASK_PARAM_S TaskParam;

    VDH_GET_COMMON_CTX(pCtx);

    pModule = &(pCtx->Scd_Module[ModuleID]);

    if (pModule->ModuleState != MODULE_IDLE)
    {
        return;
    }

    ret = VDH_Acquire_Usage(USAGE_SCD, ModuleID, SPONSOR_DISTR, 50, NO_FORCE);
    if (ret != VDH_OK)
    {
        return;
    }

    memset(&TaskParam, 0, sizeof(TaskParam));
    ret = VDH_GetTask_To_Process(MODULE_SCD, ModuleID, &TaskParam);
    if (ret != VDH_OK)
    {
        goto EXIT;
    }


    // set working state first, in case of interrupt happened before state config.
    if (NORMAL_SCD_TASK == TaskParam.Type)
    {
        pModule->NormalStartCount++;
        pModule->ModuleState = MODULE_NORMAL_WORKING;
        //dprint(PRN_ERROR, "Process Normal Scd Task, Chan id %d\n", TaskParam.ChanId);
    }
    else if (SECURE_SCD_TASK == TaskParam.Type)
    {
        pModule->SecureStartCount++;
        pModule->ModuleState = MODULE_SECURE_WORKING;
        //dprint(PRN_ERROR, "Process Secure Scd Task, Chan id %d\n", TaskParam.chan_id);
    }

    VDH_Start_Scd_Module(&TaskParam);

EXIT:
    VDH_Loose_Usage(USAGE_SCD, ModuleID, SPONSOR_DISTR);

    return;
}


/******************************************
 Process the mfde task
******************************************/
VOID VDH_Process_MfdeTask(SINT32 ModuleID)
{
    SINT32 ret;
    VDH_COMMON_CTX_S *pCtx  = NULL;
    MODULE_DEC_S     *pModule = NULL;
    TASK_PARAM_S TaskParam;

    VDH_GET_COMMON_CTX(pCtx);

    pModule = &(pCtx->Mfde_Module[ModuleID]);

    if (pModule->ModuleState != MODULE_IDLE)
    {
        return;
    }

    ret = VDH_Acquire_Usage(USAGE_MFDE, ModuleID, SPONSOR_DISTR, 50, NO_FORCE);
    if (ret != VDH_OK)
    {
        return;
    }

    memset(&TaskParam, 0, sizeof(TaskParam));
    ret = VDH_GetTask_To_Process(MODULE_MFDE, ModuleID, &TaskParam);
    if (ret != VDH_OK)
    {
        goto EXIT;
    }

    // set working state first, in case of interrupt happened before state config.
    if (NORMAL_MFDE_TASK == TaskParam.Type)
    {
        pModule->NormalStartCount++;
        pModule->ModuleState = MODULE_NORMAL_WORKING;
        //dprint(PRN_ERROR, "Process Normal Mfde Task, Chan id %d\n", TaskParam.ChanId);
    }
    else if (SECURE_MFDE_TASK == TaskParam.Type)
    {
        pModule->SecureStartCount++;
        pModule->ModuleState = MODULE_SECURE_WORKING;
        //dprint(PRN_ERROR, "Process Secure Mfde Task, Chan id %d\n", TaskParam.ChanId);
    }

    VDH_Start_Mfde_Module(&TaskParam);

EXIT:
    VDH_Loose_Usage(USAGE_MFDE, ModuleID, SPONSOR_DISTR);

    return;
}

/******************************************
 Enable all modules
******************************************/
inline VOID VDH_Enable_All_Module(VOID)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    for (i=0; i<SCD_MODULE_NUM; i++)
    {
        if (pCtx->Scd_Module[i].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Scd_Module(i);
            pCtx->Scd_Module[i].PowerState = MODULE_POWER_ON;
        }
    }
    for (i=0; i<MFDE_MODULE_NUM; i++)
    {
        if (pCtx->Mfde_Module[i].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Mfde_Module(i);
            pCtx->Mfde_Module[i].PowerState = MODULE_POWER_ON;
        }
    }
    for (i=0; i<BPD_MODULE_NUM; i++)
    {
        if (pCtx->Bpd_Module[i].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Bpd_Module(i);
            pCtx->Bpd_Module[i].PowerState = MODULE_POWER_ON;
        }
    }
    for (i=0; i<DSP_MODULE_NUM; i++)
    {
        if (pCtx->Dsp_Module[i].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Dsp_Module(i);
            pCtx->Dsp_Module[i].PowerState = MODULE_POWER_ON;
        }
    }

    return;
}


/******************************************
 Disable all modules
******************************************/
inline VOID VDH_Disable_All_Module(VOID)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    for (i=0; i<SCD_MODULE_NUM; i++)
    {
        if (pCtx->Scd_Module[i].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Scd_Module(i);
            pCtx->Scd_Module[i].PowerState = MODULE_POWER_OFF;
        }
    }
    for (i=0; i<MFDE_MODULE_NUM; i++)
    {
        if (pCtx->Mfde_Module[i].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Mfde_Module(i);
            pCtx->Mfde_Module[i].PowerState = MODULE_POWER_OFF;
        }
    }
    for (i=0; i<BPD_MODULE_NUM; i++)
    {
        if (pCtx->Bpd_Module[i].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Bpd_Module(i);
            pCtx->Bpd_Module[i].PowerState = MODULE_POWER_OFF;
        }
    }
    for (i=0; i<DSP_MODULE_NUM; i++)
    {
        if (pCtx->Dsp_Module[i].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Dsp_Module(i);
            pCtx->Dsp_Module[i].PowerState = MODULE_POWER_OFF;
        }
    }

    return;
}


/******************************************
 Enable module by close its clock
******************************************/
inline VOID VDH_Enable_Module(MODULE_TYPE_E ModuleType, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    if (MODULE_SCD == ModuleType)
    {
        if (pCtx->Scd_Module[ModuleID].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Scd_Module(ModuleID);
            pCtx->Scd_Module[ModuleID].PowerState = MODULE_POWER_ON;
        }
    }
    if (MODULE_MFDE == ModuleType)
    {
        if (pCtx->Mfde_Module[ModuleID].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Mfde_Module(ModuleID);
            pCtx->Mfde_Module[ModuleID].PowerState = MODULE_POWER_ON;
        }
    }
    if (MODULE_BPD == ModuleType)
    {
        if (pCtx->Bpd_Module[ModuleID].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Bpd_Module(ModuleID);
            pCtx->Bpd_Module[ModuleID].PowerState = MODULE_POWER_ON;
        }
    }
    if (MODULE_DSP == ModuleType)
    {
        if (pCtx->Dsp_Module[ModuleID].PowerState != MODULE_POWER_ON)
        {
            VDH_Enable_Dsp_Module(ModuleID);
            pCtx->Dsp_Module[ModuleID].PowerState = MODULE_POWER_ON;
        }
    }

    return;
}


/******************************************
 Disable module by close its clock
******************************************/
inline VOID VDH_Disable_Module(MODULE_TYPE_E ModuleType, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    if (MODULE_SCD == ModuleType)
    {
        if (pCtx->Scd_Module[ModuleID].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Scd_Module(ModuleID);
            pCtx->Scd_Module[ModuleID].PowerState = MODULE_POWER_OFF;
        }
    }
    if (MODULE_MFDE == ModuleType)
    {
        if (pCtx->Mfde_Module[ModuleID].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Mfde_Module(ModuleID);
            pCtx->Mfde_Module[ModuleID].PowerState = MODULE_POWER_OFF;
        }
    }
    if (MODULE_BPD == ModuleType)
    {
        if (pCtx->Bpd_Module[ModuleID].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Bpd_Module(ModuleID);
            pCtx->Bpd_Module[ModuleID].PowerState = MODULE_POWER_OFF;
        }
    }
    if (MODULE_DSP == ModuleType)
    {
        if (pCtx->Dsp_Module[ModuleID].PowerState != MODULE_POWER_OFF)
        {
            VDH_Disable_Dsp_Module(ModuleID);
            pCtx->Dsp_Module[ModuleID].PowerState = MODULE_POWER_OFF;
        }
    }

    return;
}


/******************************************
 VDH dsp code manage function,
 decide to load avs/lowdly code
******************************************/
VOID VDH_DSPCode_Management(SINT32 ModuleID)
{
    UINT8 i;
    DSP_REQUIRE_E    *pRequire = NULL;
    VDH_COMMON_CTX_S *pCtx  = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    for (i=0; i<2; i++)
    {
        if (0 == i)
        {
            /* normal path always has priority */
            pRequire = &pCtx->Dsp_Code[ModuleID].NormalRequire;
        }
        else
        {
            pRequire = &pCtx->Dsp_Code[ModuleID].SecureRequire;
        }

        /* check if is on call */
        if (DSP_REQUIRE_NONE == *pRequire)
        {
            continue;
        }

        /* check current dsp code state */
        if ((DSP_CODE_AVS    == pCtx->Dsp_Code[ModuleID].Code && DSP_REQUIRE_AVS    == *pRequire)
         || (DSP_CODE_LOWDLY == pCtx->Dsp_Code[ModuleID].Code && DSP_REQUIRE_LOWDLY == *pRequire))
        {
            /* dsp already loaded required code */
            return;
        }
        else if ((DSP_CODE_AVS    == pCtx->Dsp_Code[ModuleID].Code && DSP_REQUIRE_LOWDLY == *pRequire)
              || (DSP_CODE_LOWDLY == pCtx->Dsp_Code[ModuleID].Code && DSP_REQUIRE_AVS    == *pRequire))
        {
            /* dsp already in used */
            dprint(PRN_ERROR, "%s %s failed: dsp already in used, require %d, code type %d!\n", __func__, (0==i)?"normal":"secure", *pRequire, pCtx->Dsp_Code[ModuleID].Code);
            return;
        }
        else
        {
            /*
               Load code here:
               Before load dsp code, we should make sure that all needed modules power on.
               There is no need to call disable func for they will be managed latter.
            */
            VDH_Enable_Module(MODULE_MFDE, ModuleID);
            VDH_Enable_Module(MODULE_SCD, ModuleID);
            VDH_Enable_Module(MODULE_DSP, ModuleID);

            VDH_Load_Dsp_Code(ModuleID, *pRequire);
            pCtx->Dsp_Code[ModuleID].Code = (DSP_REQUIRE_AVS == *pRequire)? DSP_CODE_AVS: DSP_CODE_LOWDLY;
        }
    }

    return;
}


/******************************************
 VDH module manage function,
 power dissipation & dsp code load require
******************************************/
VOID VDH_Module_Management(VOID)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    // Dsp Power Manage
    for (i=0; i<DSP_MODULE_NUM; i++)
    {
        VDH_DSPCode_Management(i);

        if (1 == pCtx->Dsp_Module[i].NormalNeed || 1 == pCtx->Dsp_Module[i].SecureNeed)
        {
            VDH_Enable_Module(MODULE_DSP, i);
        }
        else
        {
            VDH_Disable_Module(MODULE_DSP, i);
        }
    }

    // Scd Power Manage
    for (i=0; i<SCD_MODULE_NUM; i++)
    {
        if (1 == pCtx->Scd_Module[i].NormalNeed || 1 == pCtx->Scd_Module[i].SecureNeed)
        {
            VDH_Enable_Module(MODULE_SCD, i);
        }
        else
        {
            VDH_Disable_Module(MODULE_SCD, i);
        }
    }

    // Mfde Power Manage
    for (i=0; i<MFDE_MODULE_NUM; i++)
    {
        if (1 == pCtx->Mfde_Module[i].NormalNeed || 1 == pCtx->Mfde_Module[i].SecureNeed)
        {
            VDH_Enable_Module(MODULE_MFDE, i);
        }
        else
        {
            VDH_Disable_Module(MODULE_MFDE, i);
        }
    }

    // Bpd Power Manage
    for (i=0; i<BPD_MODULE_NUM; i++)
    {
        if (1 == pCtx->Bpd_Module[i].NormalNeed || 1 == pCtx->Bpd_Module[i].SecureNeed)
        {
            VDH_Enable_Module(MODULE_BPD, i);
        }
        else
        {
            VDH_Disable_Module(MODULE_BPD, i);
        }
    }

    // Init
    if (1 == pCtx->HwInitNeed)
    {
        VDH_Init_Hardware();
        pCtx->HwInitNeed = 0;
    }

    return;
}


/******************************************
 Clear specific task list
******************************************/
SINT32 VDH_Clear_TaskList(MODULE_TYPE_E Type, SINT32 ModuleID)
{
    TASK_NODE_S    *pTaskNode  = NULL;
    TASK_NODE_S    *pTempNode  = NULL;
    TASK_LIST_S    *pTaskList  = NULL;

    switch(Type)
    {
        case MODULE_SCD:
            pTaskList  = &NormalCtx.NormalScdTask[ModuleID];
            break;

        case MODULE_MFDE:
            pTaskList  = &NormalCtx.NormalMfdeTask[ModuleID];
            break;

        default:
            dprint(PRN_ERROR, "%s unkown module type %d!\n", __func__, Type);
            return VDH_ERR;
    }

    OSAL_WriteLockIRQ(&pTaskList->TaskLock);
    if (!OSAL_LIST_EMPTY(&pTaskList->TaskList))
    {
        OSAL_LIST_FOR_EACH_ENTRY_SAFE(pTaskNode, pTempNode, &pTaskList->TaskList, Node)
        {
            OSAL_LIST_DEL(&pTaskNode->Node);
            pTaskNode->InUsed = 0;
        }
    }
    pTaskList->TaskNum = 0;
    OSAL_WriteUnLockIRQ(&pTaskList->TaskLock);

    return VDH_OK;
}


/******************************************
 Check distributor wake up reason
******************************************/
VOID VDH_Check_WakeUpReason(SINT32 type)
{
    if (-1 == type)
    {
        dprint(PRN_ERROR, "Distributor wake up by time out, Type = %d\n", type);
    }
    else
    {
        if (IS_WAKE_UP_BY(type, WAKE_UP_NORMAL_SCD_TASK))
        {
            dprint(PRN_ERROR, "Distributor wake up by normal scd task, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_NORMAL_MFDE_TASK))
        {
            dprint(PRN_ERROR, "Distributor wake up by normal mfde task, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_SECURE_SCD_TASK))
        {
            dprint(PRN_ERROR, "Distributor wake up by secure scd task, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_SECURE_MFDE_TASK))
        {
            dprint(PRN_ERROR, "Distributor wake up by secure mfde task, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_SCD_IDLE))
        {
            dprint(PRN_ERROR, "Distributor wake up by scd idle, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_MFDE_IDLE))
        {
            dprint(PRN_ERROR, "Distributor wake up by mfde idle, Type = %d\n", type);
        }
        if (IS_WAKE_UP_BY(type, WAKE_UP_EXIT))
        {
            dprint(PRN_ERROR, "Distributor wake up by exit cmmand, Type = %d\n", type);
        }
    }

    return;
}


/******************************************
 Distributor main entry
******************************************/
SINT32 VDH_Distributor( VOID *Argu )
{
    UINT32 i;
    SINT32 type = -1;
    VDH_COMMON_CTX_S *pCtx  = NULL;

    dprint(PRN_ERROR, "Enter %s\n", __func__);

    VDH_GET_COMMON_CTX_RET(pCtx);

    while(IS_DISTRIBUTOR_ALIVE(NormalCtx.DistributorState))
    {
        if (CMD_SLEEP == NormalCtx.DistributorCommand)
        {
            goto THREAD_SLEEP;
        }
        else if (CMD_EXIT == NormalCtx.DistributorCommand)
        {
            break;
        }

        NormalCtx.DistributorState = DISTRIBUTOR_RUNNING;

        VDH_Module_Management();

        for (i=0; i<SCD_MODULE_NUM; i++)
        {
            VDH_Process_ScdTask(i);
        }

        for (i=0; i<MFDE_MODULE_NUM; i++)
        {
            VDH_Process_MfdeTask(i);
        }

    THREAD_SLEEP:
        NormalCtx.DistributorState = DISTRIBUTOR_SLEEPING;
        type = OSAL_WaitEvent_l(&NormalCtx.ThreadEvent, 10);

        //VDH_Check_WakeUpReason(type);
    }

    NormalCtx.DistributorState = DISTRIBUTOR_EXITED;

    dprint(PRN_ERROR, "Exit %s\n", __func__);

    return VDH_OK;
}


/******************************************
 Read VDH Proc Info
******************************************/
SINT32 VDH_Read_Proc(struct seq_file *p, VOID *v)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx = NULL;

    if (1 == g_HalDisable)
    {
        seq_printf(p, "HAL Disable\n\n");
    }
    else
    {
        if (NULL == NormalCtx.pCommonContext)
        {
            dprint(PRN_ALWS,"No channel running, vdh distributor not created!\n");
            return VDH_OK;
        }

        VDH_GET_COMMON_CTX_RET(pCtx);

        seq_printf(p, "\n");
        seq_printf(p, "====================== VDH Distributor ======================\n");
        seq_printf(p, "\n");

        seq_printf(p, "Thread\n");
        seq_printf(p, "State            :%-7s  | Command             :%s\n",  VDH_Show_Thread_State(NormalCtx.DistributorState), VDH_Show_Thread_Command(NormalCtx.DistributorCommand));
        seq_printf(p, "\n");

        for (i=0; i<SCD_MODULE_NUM; i++)
        {
        seq_printf(p, "SCD  %d\n", i);
        seq_printf(p, "Power            :%-7d  | State               :%s\n", pCtx->Scd_Module[i].PowerState,        VDH_Show_Module_State(pCtx->Scd_Module[i].ModuleState));
        seq_printf(p, "Normal Need      :%-7d  | Secure Need         :%d\n", pCtx->Scd_Module[i].NormalNeed,        pCtx->Scd_Module[i].SecureNeed);
        seq_printf(p, "N_Start_Count    :%-7d  | S_Start_Count       :%d\n", pCtx->Scd_Module[i].NormalStartCount,  pCtx->Scd_Module[i].SecureStartCount);
        seq_printf(p, "N_Task_Num       :%-7d  | S_Task_Num          :%d\n", NormalCtx.NormalScdTask[i].TaskNum,    pCtx->SecureScdArray[i].TaskNum);
        seq_printf(p, "\n");
        }

        for (i=0; i<MFDE_MODULE_NUM; i++)
        {
        seq_printf(p, "MFDE %d\n", i);
        seq_printf(p, "Power            :%-7d  | State               :%s\n", pCtx->Mfde_Module[i].PowerState,       VDH_Show_Module_State(pCtx->Mfde_Module[i].ModuleState));
        seq_printf(p, "Normal Need      :%-7d  | Secure Need         :%d\n", pCtx->Mfde_Module[i].NormalNeed,       pCtx->Mfde_Module[i].SecureNeed);
        seq_printf(p, "N_Start_Count    :%-7d  | S_Start_Count       :%d\n", pCtx->Mfde_Module[i].NormalStartCount, pCtx->Mfde_Module[i].SecureStartCount);
        seq_printf(p, "N_Task_Num       :%-7d  | S_Task_Num          :%d\n", NormalCtx.NormalMfdeTask[i].TaskNum,   pCtx->SecureMfdeArray[i].TaskNum);
        seq_printf(p, "\n");
        }

        for (i=0; i<BPD_MODULE_NUM; i++)
        {
        seq_printf(p, "BPD  %d\n", i);
        seq_printf(p, "Power            :%-7d  | State               :%s\n", pCtx->Bpd_Module[i].PowerState,        VDH_Show_Module_State(pCtx->Bpd_Module[i].ModuleState));
        seq_printf(p, "Normal Need      :%-7d  | Secure Need         :%d\n", pCtx->Bpd_Module[i].NormalNeed,        pCtx->Bpd_Module[i].SecureNeed);
        seq_printf(p, "N_Start_Count    :%-7d  | S_Start_Count       :%d\n", pCtx->Bpd_Module[i].NormalStartCount,  pCtx->Bpd_Module[i].SecureStartCount);
        seq_printf(p, "\n");
        }

        for (i=0; i<DSP_MODULE_NUM; i++)
        {
        seq_printf(p, "DSP  %d\n", i);
        seq_printf(p, "Power            :%-7d  | State               :%s\n", pCtx->Dsp_Module[i].PowerState,  VDH_Show_Module_State(pCtx->Dsp_Module[i].ModuleState));
        seq_printf(p, "Normal Need      :%-7d  | Secure Need         :%d\n", pCtx->Dsp_Module[i].NormalNeed,  pCtx->Dsp_Module[i].SecureNeed);
        seq_printf(p, "\n");
        }

        seq_printf(p, "=============================================================\n");
        seq_printf(p, "\n");
    }

    return VDH_OK;
}


/******************************************
 Write VDH Proc Info
******************************************/
SINT32 VDH_Write_Proc(struct file *file, const char __user *buffer, size_t count, loff_t *data)
{
    UINT32 i,j;
    UINT8  buf[VDH_CMD_LEN];
    UINT8  str1[VDH_CMD_LEN];
    UINT8  str2[VDH_CMD_LEN];

    if(count >= sizeof(buf))
    {
        dprint(PRN_ALWS, "Parameter string is too long!\n");
        return VDH_ERR;
    }

    memset(buf, 0, sizeof(buf));
    if (copy_from_user(buf, buffer, count))
    {
        dprint(PRN_ALWS, "Copy from user failed!\n");
        return VDH_ERR;
    }
    buf[count] = 0;

    if (count < 1)
    {
        dprint(PRN_ALWS, "No valid commad!\n");
    }
    else
    {
        /* dat1 */
        i = 0;
        j = 0;
        for(; i < count; i++)
        {
            if(j==0 && buf[i]==' ')
            {
                continue;
            }
            if(buf[i] > ' ')
            {
                str1[j++] = buf[i];
            }
            if(j>0 && buf[i]==' ')
            {
                break;
            }
        }
        str1[j] = 0;

        /* dat2 */
        j = 0;
        for(; i < count; i++)
        {
            if(j==0 && buf[i]==' ')
            {
                continue;
            }
            if(buf[i] > ' ')
            {
                str2[j++] = buf[i];
            }
            if(j>0 && buf[i]==' ')
            {
                break;
            }
        }
        str2[j] = 0;

        if (!strncmp(str1, VDH_CMD_HAL, sizeof(VDH_CMD_HAL)))
        {
            if(0==strncmp(str2,"on",2))
            {
                g_HalDisable = 0;
                dprint(PRN_ALWS, "Enable vdh distributor.\n");
            }
            else if(0==strncmp(str2,"off",3))
            {
                g_HalDisable = 1;
                dprint(PRN_ALWS, "Disable vdh distributor.\n");
            }
            else
            {
                dprint(PRN_ALWS, "Invalid string %s, should be on/off.\n", str2);
            }
        }
        else
        {
            dprint(PRN_ALWS, "Unkown commad!\n");
        }
    }

    return count;
}


/*============== EXTERNAL FUNCTION =============*/

/******************************************
 Get Common Context
******************************************/
VDH_COMMON_CTX_S* VDH_Get_CommonCtx(VOID)
{
    if (NULL == NormalCtx.pCommonContext)
    {
        dprint(PRN_ERROR,"Get CommonCtx Failed: pCommonContext = NULL!\n");
        return NULL;
    }

    //dprint(PRN_ALWS, "sizeof(VDH_COMMON_CTX_S) = %d, sizeof(long) = %d\n", sizeof(VDH_COMMON_CTX_S), sizeof(long));

    return NormalCtx.pCommonContext;
}


/******************************************
 Initialize VDH hardware abstract layer
******************************************/
SINT32 VDH_Normal_Init(VOID)
{
    UINT32 i;
    SINT32 ret;

    dprint(PRN_ERROR, "Enter %s\n", __func__);

    if (g_VDHInitCount > 0)
    {
        dprint(PRN_ERROR, "Exit %s already init.\n", __func__);
        return VDH_OK;
    }
    g_VDHInitCount++;

    memset(&NormalCtx, 0, sizeof(VDH_NORMAL_CTX_S));

    ret = VDH_Alloc_ComBuffer();
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "%s VDH_Alloc_ComBuffer failed!\n", __func__);
        goto ERROR_EXIT0;
    }

    for (i=0; i<SCD_MODULE_NUM; i++)
    {
    	OSAL_INIT_LIST_HEAD(&NormalCtx.NormalScdTask[i].TaskList);
        OSAL_RWLockIRQInit(&NormalCtx.NormalScdTask[i].TaskLock);
    }
    for (i=0; i<MFDE_MODULE_NUM; i++)
    {
    	OSAL_INIT_LIST_HEAD(&NormalCtx.NormalMfdeTask[i].TaskList);
        OSAL_RWLockIRQInit(&NormalCtx.NormalMfdeTask[i].TaskLock);
    }

    OSAL_InitEvent_l(&NormalCtx.ThreadEvent, 0);

    ret = OSAL_CreateTask((VOID *)&NormalCtx.DistributorThread, "VDH_Distributor", VDH_Distributor);
    if (ret != OSAL_OK)
    {
        dprint(PRN_ERROR,"%s create task distributor failed\n", __func__);
        goto ERROR_EXIT1;
    }
    NormalCtx.DistributorCommand = CMD_RUN;

    VDH_Init_Module();
    //VDH_Enable_All_Module();

    dprint(PRN_ERROR, "Exit %s success.\n", __func__);

    return VDH_OK;

ERROR_EXIT1:
    VDH_Free_ComBuffer();

ERROR_EXIT0:
    g_VDHInitCount = (g_VDHInitCount-1<0)? 0: g_VDHInitCount-1;

    return VDH_ERR;
}


/******************************************
 Deinitialize VDH hardware abstract layer
******************************************/
SINT32 VDH_Exit(VOID)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx  = NULL;

    dprint(PRN_ERROR, "Enter %s\n", __func__);

    g_VDHInitCount = (g_VDHInitCount-1<0)? 0: g_VDHInitCount-1;

    if (g_VDHInitCount > 0)
    {
        dprint(PRN_ERROR, "Exit %s already exit.\n", __func__);
        return VDH_OK;
    }

    VDH_GET_COMMON_CTX_RET(pCtx);

    if (IS_DISTRIBUTOR_ALIVE(NormalCtx.DistributorState))
    {
        NormalCtx.DistributorCommand = CMD_EXIT;
        VDH_WakeUp_Distributor(WAKE_UP_EXIT);
        for (i=0; i<100; i++)
        {
            if (DISTRIBUTOR_EXITED == NormalCtx.DistributorState)
            {
                break;
            }
            OSAL_MSLEEP(5);
        }
        if (i >= 100)
        {
            dprint(PRN_ERROR, "Kill distributor thread failed!\n");
        }
    }

    // Shut down all module's power
    VDH_Disable_All_Module();

    // Free distributor share buffer
    VDH_Free_ComBuffer();

    memset(&NormalCtx, 0, sizeof(VDH_NORMAL_CTX_S));

    dprint(PRN_ERROR, "Exit %s success.\n", __func__);

    return VDH_OK;
}


/******************************************
 Create vdh distributor proc
******************************************/
VOID VDH_Init_Proc (VOID)
{
    SINT32 ret;

    ret = vfmw_proc_create(VDH_PROC_NAME, VDH_Read_Proc, VDH_Write_Proc);
    if (0 != ret)
    {
        dprint(PRN_ERROR, "%s create proc %s failed!\n", __func__, VDH_PROC_NAME);
        return;
    }

    return;
}


/******************************************
 Destroy vdh distributor proc
******************************************/
VOID VDH_Exit_Proc(VOID)
{
    vfmw_proc_destroy(VDH_PROC_NAME);

    return;
}


/******************************************
 Suspend vdh distributor
******************************************/
VOID VDH_Suspend(VOID)
{
    UINT32 i;
    VDH_COMMON_CTX_S *pCtx  = NULL;

    dprint(PRN_ERROR, "Enter %s\n", __func__);

    if (0 == g_VDHInitCount)
    {
        return;
    }

    VDH_GET_COMMON_CTX(pCtx);

    // Stop distributor thread
    if (IS_DISTRIBUTOR_ALIVE(NormalCtx.DistributorState))
    {
        NormalCtx.DistributorCommand = CMD_SLEEP;
        for (i=0; i<100; i++)
        {
            if (DISTRIBUTOR_SLEEPING == NormalCtx.DistributorState)
            {
                break;
            }
            OSAL_MSLEEP(5);
        }
        if (i >= 100)
        {
            dprint(PRN_ERROR, "Command distributor thread to sleep failed!\n");
        }
    }

    // Shut down all module's power
    VDH_Disable_All_Module();

    dprint(PRN_ERROR, "Exit %s\n", __func__);

    return;
}


/******************************************
 Resume vdh distributor
******************************************/
VOID VDH_Resume(VOID)
{
    dprint(PRN_ERROR, "Enter %s\n", __func__);

    if (0 == g_VDHInitCount)
    {
        return;
    }

    // Start distributor thread
    if (NormalCtx.pCommonContext != NULL)
    {
        NormalCtx.pCommonContext->HwInitNeed = 1;
    }
    NormalCtx.DistributorCommand = CMD_RUN;
    VDH_WakeUp_Distributor(WAKE_UP_EXIT);

    // Needed modules will power on in distributor thread

    dprint(PRN_ERROR, "Exit %s\n", __func__);

    return;
}

/******************************************
 Get VDH Hal Context data
******************************************/
SINT32 VDH_Get_ShareData(VDH_SHARE_DATA_S *pShareData)
{
    VDH_ASSERT_RET(NULL != NormalCtx.pCommonContext, "pCommonContext = NULL");

    pShareData->common_context = g_HalMemRecord.PhyAddr;

    dprint(PRN_ERROR, "VDH_Get_ShareData: common_context = 0x%x\n", pShareData->common_context);

    return VDH_OK;
}

/******************************************
 Post a normal task
******************************************/
SINT32 VDH_PostTask(TASK_PARAM_S *pTask)
{
    TASK_NODE_S        *pTaskNode = NULL;
    TASK_LIST_S        *pTaskList = NULL;
    WAKE_UP_TYPE_E      eWakeType = WAKE_UP_NOTHING;

    VDH_ASSERT_RET(NULL != NormalCtx.pCommonContext, "pCommonContext = NULL");
    VDH_ASSERT_RET(NULL != pTask,        "pTask = NULL");

    switch(pTask->Type)
    {
        case NORMAL_SCD_TASK:
            VDH_ASSERT_RET(pTask->ModuleId < SCD_MODULE_NUM, "invalid Scd ModuleID");
            pTaskList = &NormalCtx.NormalScdTask[pTask->ModuleId];
            eWakeType = WAKE_UP_NORMAL_SCD_TASK;
            //dprint(PRN_ERROR, "Post Normal Scd Task, Chan id %d\n", pTask->ChanId);
            break;

        case NORMAL_MFDE_TASK:
            VDH_ASSERT_RET(pTask->ModuleId < MFDE_MODULE_NUM, "invalid Mfde ModuleID");
            pTaskList = &NormalCtx.NormalMfdeTask[pTask->ModuleId];
            eWakeType = WAKE_UP_NORMAL_MFDE_TASK;
            //dprint(PRN_ERROR, "Post Normal Mfde Task, Chan id %d\n", pTask->ChanId);
            break;

        default:
            dprint(PRN_ERROR, "%s unkown task type %d!\n", __func__, pTask->Type);
            return VDH_ERR;
    }

    OSAL_WriteLockIRQ(&pTaskList->TaskLock);

    pTaskNode = &pTaskList->TaskNode;
    if (1 == pTaskNode->InUsed)
    {
        dprint(PRN_ERROR, "%s TaskNode InUsed!\n", __func__);
        OSAL_WriteUnLockIRQ(&pTaskList->TaskLock);
        return VDH_ERR;
    }

    memcpy(&pTaskNode->Task, pTask, sizeof(TASK_PARAM_S));

    OSAL_LIST_ADD_TAIL(&pTaskNode->Node, &pTaskList->TaskList);

    pTaskNode->InUsed = 1;

    pTaskList->TaskNum += 1;

    OSAL_WriteUnLockIRQ(&pTaskList->TaskLock);

    VDH_WakeUp_Distributor(eWakeType);

    return VDH_OK;
}


/******************************************
 Send event to wake up distributor thread
******************************************/
inline VOID VDH_WakeUp_Distributor(WAKE_UP_TYPE_E Type)
{
    OSAL_GiveEvent_l(&NormalCtx.ThreadEvent, (UINT32)Type);

    return;
}

/******************************************
 Check if secure need to wake up distributor thread
******************************************/
VOID VDH_Secure_WakeUp_Distributor(VOID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX(pCtx);

    if (pCtx->SecureWakeUp != WAKE_UP_NOTHING)
    {
        OSAL_GiveEvent_l(&NormalCtx.ThreadEvent, (UINT32)pCtx->SecureWakeUp);
        pCtx->SecureWakeUp = WAKE_UP_NOTHING;
    }

    return;
}

/******************************************
 Need module notification
******************************************/
SINT32 VDH_Need_Module(MODULE_TYPE_E Type, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX_RET(pCtx);

    switch(Type)
    {
        case MODULE_SCD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pCtx->Scd_Module[ModuleID].NormalNeed = 1;
            break;

        case MODULE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pCtx->Mfde_Module[ModuleID].NormalNeed = 1;
            break;

        case MODULE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pCtx->Bpd_Module[ModuleID].NormalNeed = 1;
            break;

        case MODULE_DSP:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < DSP_MODULE_NUM), "invalid Dsp ModuleID");
            pCtx->Dsp_Module[ModuleID].NormalNeed = 1;
            break;

        default:
            dprint(PRN_ERROR, "%s unkown module type %d!\n", __func__, Type);
            return VDH_ERR;
    }

    return VDH_OK;
}


/******************************************
 UnNeed module notification
******************************************/
SINT32 VDH_UnNeed_Module(MODULE_TYPE_E Type, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX_RET(pCtx);

    switch(Type)
    {
        case MODULE_SCD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pCtx->Scd_Module[ModuleID].NormalNeed = 0;
            break;

        case MODULE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pCtx->Mfde_Module[ModuleID].NormalNeed = 0;
            break;

        case MODULE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pCtx->Bpd_Module[ModuleID].NormalNeed = 0;
            break;

        case MODULE_DSP:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < DSP_MODULE_NUM), "invalid Dsp ModuleID");
            pCtx->Dsp_Module[ModuleID].NormalNeed = 0;
            break;

        default:
            dprint(PRN_ERROR, "%s unkown module type %d!\n", __func__, Type);
            return VDH_ERR;
    }

    return VDH_OK;
}


/******************************************
 Reset Module
******************************************/
SINT32 VDH_Reset_Module(MODULE_TYPE_E Type, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_GET_COMMON_CTX_RET(pCtx);

    switch(Type)
    {
        case MODULE_SCD:
            dprint(PRN_ERROR, "Normal call Reset Scd\n");
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");

            VDH_Acquire_Usage(USAGE_SCD, ModuleID, SPONSOR_NORMAL, 200, WITH_FORCE);
            VDH_Clear_TaskList(MODULE_SCD, ModuleID);
            if (MODULE_NORMAL_WORKING == pCtx->Scd_Module[ModuleID].ModuleState)
            {
                VDH_Reset_Scd_Module(ModuleID);
                VDH_Notify_ModuleIdle(MODULE_SCD, ModuleID);
            }
            VDH_Loose_Usage(USAGE_SCD, ModuleID, SPONSOR_NORMAL);
            break;

        case MODULE_MFDE:
            dprint(PRN_ERROR, "Normal call Reset Mfde\n");
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");

            VDH_Acquire_Usage(USAGE_MFDE, ModuleID, SPONSOR_NORMAL, 200, WITH_FORCE);
            VDH_Clear_TaskList(MODULE_MFDE, ModuleID);
            if (MODULE_NORMAL_WORKING == pCtx->Mfde_Module[ModuleID].ModuleState)
            {
                VDH_Reset_Mfde_Module(ModuleID);
                VDH_Notify_ModuleIdle(MODULE_MFDE, ModuleID);
            }
            VDH_Loose_Usage(USAGE_MFDE, ModuleID, SPONSOR_NORMAL);
            break;

        // TODO:: add other module reset function if needed

        default:
            dprint(PRN_ERROR, "%s unkown module type %d!\n", __func__, Type);
            return VDH_ERR;
    }

    return VDH_OK;
}

/******************************************
 Require to load dsp code
******************************************/
SINT32 VDH_Require_DspCode(SINT32 DspID, DSP_REQUIRE_E Type)
{
    VDH_COMMON_CTX_S *pCtx = NULL;

    VDH_ASSERT_RET((DspID >= 0 && DspID < DSP_MODULE_NUM), "invalid DspID");
    VDH_ASSERT_RET((DSP_REQUIRE_AVS == Type || DSP_REQUIRE_LOWDLY == Type), "invalid dsp code type");

    VDH_GET_COMMON_CTX_RET(pCtx);

    pCtx->Dsp_Code[DspID].NormalRequire = Type;
    dprint(PRN_ERROR, "Normal require dsp code type %d\n", Type);

    return VDH_OK;
}

