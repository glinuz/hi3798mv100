/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_common.h
 *
 * Purpose: vdh hardware abstract layer common header
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */

#ifndef __VDH_COMMON_H__
#define __VDH_COMMON_H__

#include "vfmw.h"
#include "sysconfig.h"
#include "vdh_module.h"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------- GLOBAL CONTROL ------------------------------*/
extern UINT8 g_HalDisable;

/*----------------------------------- MACRO -----------------------------------*/

#define VDH_OK                 (0)
#define VDH_ERR                (-1)

#define WITH_FORCE             (1)
#define NO_FORCE               (0)

#define MAX_TASK_NUM           (2)

#define SCD_MODULE_NUM         (MAX_SCD_NUM)
#define MFDE_MODULE_NUM        (MAX_VDH_NUM)
#define DSP_MODULE_NUM         (MAX_DSP_NUM)
#define BPD_MODULE_NUM         (1)

#define VDH_ASSERT_RET( cond, else_print )                      \
do {                                                            \
    if(!(cond))                                                 \
    {                                                           \
        dprint(PRN_FATAL, "%s: %s\n", __func__, else_print);    \
        return VDH_ERR;                                         \
    }                                                           \
}while(0)

#define VDH_ASSERT( cond, else_print )                          \
do {                                                            \
    if(!(cond))                                                 \
    {                                                           \
        dprint(PRN_FATAL, "%s: %s\n", __func__, else_print);    \
        return;                                                 \
    }                                                           \
}while(0)

#define VDH_GET_COMMON_CTX(pCtx)                                \
do {                                                            \
    pCtx = VDH_Get_CommonCtx();                                 \
    VDH_ASSERT(NULL != pCtx, "get common ctx failed!");         \
}while(0)

#define VDH_GET_COMMON_CTX_RET(pCtx)                            \
do {                                                            \
    pCtx = VDH_Get_CommonCtx();                                 \
    VDH_ASSERT_RET(NULL != pCtx, "get common ctx failed!");     \
}while(0)


/*----------------------------------- ENUM ------------------------------------*/

typedef enum
{
    MODULE_SCD = 0,
    MODULE_MFDE,
    MODULE_BPD,
    MODULE_DSP,
}MODULE_TYPE_E;

typedef enum
{
    MODULE_IDLE,
    MODULE_NORMAL_WORKING,
    MODULE_SECURE_WORKING,
}MODULE_STATE_E;

typedef enum
{
    DSP_CODE_NONE,
    DSP_CODE_AVS,
    DSP_CODE_LOWDLY,
}DSP_CODE_E;

typedef enum
{
    MODULE_POWER_OFF = 0,
    MODULE_POWER_ON,
}MODULE_POWER_E;

typedef enum
{
    WAKE_UP_NOTHING = 0,
    WAKE_UP_NORMAL_SCD_TASK,
    WAKE_UP_NORMAL_MFDE_TASK,
    WAKE_UP_SECURE_SCD_TASK,
    WAKE_UP_SECURE_MFDE_TASK,
    WAKE_UP_SCD_IDLE,
    WAKE_UP_MFDE_IDLE,
    WAKE_UP_EXIT,
}WAKE_UP_TYPE_E;

typedef enum
{
    PARAM_INDEX_STATE = 0,
}PARAM_INDEX_E;

typedef enum
{
    REG_CRG = 0,
    REG_SCD,
    REG_MFDE,
    REG_BPD,
    REG_DSP,
}REG_BASE_E;

typedef enum
{
    USAGE_SCD = 0,       // scd  module permit
    USAGE_MFDE,          // mfde module permit
    USAGE_BPD,           // bpd  module permit
}USAGE_MODULE_E;

typedef enum
{
    SPONSOR_DISTR = 0,   // distributor thread
    SPONSOR_NORMAL,      // normal thread
    SPONSOR_SECURE,      // secure thread
}SPONSOR_TYPE_E;


/*--------------------------------- STRUCTURE ---------------------------------*/

typedef struct
{
    MODULE_TYPE_E    Type;
    SINT32           ModuleID;
    MODULE_STATE_E   State;
}PARAM_STATE_S;

typedef struct
{
    UINT8            DistrUsage;
    UINT8            NormalUsage;
    UINT8            SecureUsage;
}VDH_USAGE_S;

typedef struct
{
    VDH_USAGE_S      Usage;
    UINT8            NormalNeed;
    UINT8            SecureNeed;
    MODULE_POWER_E   PowerState;
    MODULE_STATE_E   ModuleState;
    UINT32           NormalStartCount;
    UINT32           SecureStartCount;
}MODULE_DEC_S;

typedef struct
{
    UINT8            InUsed;
    TASK_PARAM_S     Task;
}TASK_MEM_S;

typedef struct
{
    SINT32           Head;
    SINT32           Tail;
    UINT32           TaskNum;
    TASK_MEM_S       Task[MAX_TASK_NUM];
}TASK_ARRAY_S;

typedef struct
{
    DSP_CODE_E       Code;
    DSP_REQUIRE_E    NormalRequire;
    DSP_REQUIRE_E    SecureRequire;
}DSP_CODE_S;

typedef struct
{
    UINT8            HwInitNeed;
    WAKE_UP_TYPE_E   SecureWakeUp;
    MODULE_DEC_S     Scd_Module[SCD_MODULE_NUM];
    MODULE_DEC_S     Mfde_Module[MFDE_MODULE_NUM];
    MODULE_DEC_S     Bpd_Module[BPD_MODULE_NUM];
    MODULE_DEC_S     Dsp_Module[DSP_MODULE_NUM];
    DSP_CODE_S       Dsp_Code[DSP_MODULE_NUM];

    TASK_ARRAY_S     SecureScdArray[SCD_MODULE_NUM];
    TASK_ARRAY_S     SecureMfdeArray[MFDE_MODULE_NUM];
} VDH_COMMON_CTX_S;


/*---------------------------------- FUNCTION ---------------------------------*/

/******************************************
 Get Common Context
******************************************/
VDH_COMMON_CTX_S* VDH_Get_CommonCtx(VOID);

/******************************************
 Post a new task into VDH work list
******************************************/
SINT32 VDH_PostTask(TASK_PARAM_S *pTask);

/******************************************
 Send event to wake up distributor thread
******************************************/
VOID   VDH_WakeUp_Distributor(WAKE_UP_TYPE_E Type);

/******************************************
 Client notify distributor module idle
******************************************/
VOID   VDH_Notify_ModuleIdle(MODULE_TYPE_E Type, SINT32 ModuleID);

/******************************************
 Need module notification
******************************************/
SINT32 VDH_Need_Module(MODULE_TYPE_E Type, SINT32 ModuleID);

/******************************************
 UnNeed module notification
******************************************/
SINT32 VDH_UnNeed_Module(MODULE_TYPE_E Type, SINT32 ModuleID);

/******************************************
 Reset Module
******************************************/
SINT32 VDH_Reset_Module(MODULE_TYPE_E Type, SINT32 ModuleID);

/******************************************
 Acquire Usage
******************************************/
SINT32 VDH_Acquire_Usage(USAGE_MODULE_E Index, SINT32 ModuleID, SPONSOR_TYPE_E Sponsor, UINT32 MaxWaitInMs, UINT8 ForceFlag);

/******************************************
 Loose Usage
******************************************/
SINT32 VDH_Loose_Usage(USAGE_MODULE_E Index, SINT32 ModuleID, SPONSOR_TYPE_E Sponsor);

/******************************************
 Require to load dsp code
******************************************/
SINT32 VDH_Require_DspCode(SINT32 DspID, DSP_REQUIRE_E Type);

/******************************************
 Get Parameter
******************************************/
SINT32 VDH_Get_Parameter(PARAM_INDEX_E Index, VOID *pParam);

/******************************************
 Set Parameter
******************************************/
SINT32 VDH_Set_Parameter(PARAM_INDEX_E Index, VOID *pParam);

/******************************************
 Record Reg Data into Task
******************************************/
VOID   VDH_Record_RegData(TASK_PARAM_S *pTask, REG_BASE_E Base, UINT32 Reg, UINT32 Data);

#ifdef __cplusplus
}
#endif

#endif
