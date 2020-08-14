/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpDqgHNYPPYlCl/pP8CHYYltgiRqk3H0yIbFuqar
+eidwXLkKY5WgKKVQl9KSvf12mA5sZ4fabA3zpQ8kiE4wVh1nS6EXlBS6ZGSWKZIbaNAbDZg
vKLkVaxYu8hPJjoIXEoXt72MggEvjp9kBPBktzAZHLnNPuh0h5BbIQjHb5UxmA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_secure.c
 *
 * Purpose: vdh hardware abstract layer secure file
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */


#include "public.h"
#include "vdh_secure.h"
#include "vdh_module.h"


/*============== LOCAL DEFINITION ==============*/
typedef struct 
{   
    SINT32            *pDistributorEvent;
    VDH_COMMON_CTX_S  *pCommonContext;
} VDH_SECURE_CTX_S;



/*================ LOCAL MACRO =================*/


/*================ LOCAL VALUE =================*/
SINT32            g_SecureInit = 0;
VDH_SECURE_CTX_S  g_SecureCtx;


/*============== INTERNAL FUNCTION =============*/


/*============== EXTERNAL FUNCTION =============*/

/******************************************
 Get Common Context 
******************************************/
VDH_COMMON_CTX_S* VDH_Get_CommonCtx(VOID)
{    
    if (g_SecureInit <= 0)
    {
        dprint(PRN_ERROR,"Error: g_SecureCtx.IsInit != 1\n");
        return NULL;
    }

    return (g_SecureCtx.pCommonContext); 
}

/******************************************
 Initialize VDH hardware abstract layer 
******************************************/
SINT32 VDH_Secure_Init(VDH_SHARE_DATA_S *pShareData)
{
    SINT32 ret;
    
#ifdef HI_SMMU_SUPPORT
    VDMHAL_SetSmmuPageTableAddr(0);
    VDMHAL_EnableSmmu();
#endif

    ret = VDH_Map_ShareData(pShareData);
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "VDH_Map_ShareData failed, return %d\n", ret);
        return VDH_ERR;
    }

    return VDH_OK;
}

/******************************************
 Map Share Data 
 for communication between TEE & REE
******************************************/
SINT32 VDH_Map_ShareData(VDH_SHARE_DATA_S *pShareData)
{
    if (g_SecureInit > 0)
    {
        dprint(PRN_ERROR, "Exit %s already map.\n", __func__);
        return VDH_OK;
    }
    g_SecureInit++;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&g_SecureCtx, 0, sizeof(VDH_SECURE_CTX_S));

    g_SecureCtx.pCommonContext = vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMmap(pShareData->common_context);
    if (NULL == g_SecureCtx.pCommonContext)
    {
        dprint(PRN_ERROR, "Map pCommonContext Failed!\n");
        return VDH_ERR;
    }

    return VDH_OK;
}


/******************************************
 Unmap Share Data 
 for communication between TEE & REE
******************************************/
SINT32 VDH_UnMap_ShareData(VOID)
{    
    g_SecureInit = (g_SecureInit-1<0)? 0: g_SecureInit-1;

    if (g_SecureInit > 0)
    {
        dprint(PRN_ERROR, "Exit %s already unmap.\n", __func__);
        return VDH_OK;
    }
    
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMunMap(g_SecureCtx.pCommonContext);

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&g_SecureCtx, 0, sizeof(VDH_SECURE_CTX_S));

    return VDH_OK;
}


/******************************************
 Post a secure task 
******************************************/
SINT32 VDH_PostTask(TASK_PARAM_S *pTask)
{  
    SINT32 ret;
    WAKE_UP_TYPE_E type = WAKE_UP_NOTHING;
    TASK_ARRAY_S     *pTaskArray = NULL;
    VDH_COMMON_CTX_S *pCtx       = NULL;
    TASK_MEM_S       *pTaskMem   = NULL;
    
    VDH_GET_COMMON_CTX_RET(pCtx);

    switch (pTask->Type)
    {
        case SECURE_SCD_TASK:
            VDH_ASSERT_RET((pTask->ModuleId >= 0 && pTask->ModuleId < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pTaskArray = &pCtx->SecureScdArray[pTask->ModuleId];
            type = WAKE_UP_SECURE_SCD_TASK;
            //dprint(PRN_ERROR, "Post Secure Scd Task, Chan id %d\n", pTask->chan_id);
            break;
            
        case SECURE_MFDE_TASK:
            VDH_ASSERT_RET((pTask->ModuleId >= 0 && pTask->ModuleId < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pTaskArray = &pCtx->SecureMfdeArray[pTask->ModuleId];
            type = WAKE_UP_SECURE_MFDE_TASK;
            //dprint(PRN_ERROR, "Post Secure Mfde Task, Chan id %d\n", pTask->chan_id);
            break;
            
        default:
            dprint(PRN_ERROR, "%s ERROR: unkown type %d\n", __func__, pTask->Type);
            return VDH_ERR;
    }

    if (((pTaskArray->Head+1) % MAX_TASK_NUM) == pTaskArray->Tail)
    {
        dprint(PRN_FATAL, "Type(%d): Task ARRAY OVERFLOW!\n", pTask->Type);
        return VDH_ERR;
    }
    if (pTaskArray->Head >= MAX_TASK_NUM)
    {
        dprint(PRN_FATAL, "%s pTaskArray->Head invalid!\n", __func__, pTaskArray->Head);
        pTaskArray->Head = 0;
        return VDH_ERR;
    }
    
    pTaskMem = &pTaskArray->Task[pTaskArray->Head];
    if (1 == pTaskMem->InUsed)
    {
        dprint(PRN_ERROR, "%s ERROR: TaskMem %d IsValid = 0\n", __func__, pTaskArray->Head);
        return VDH_ERR;
    }
    
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCpy(&pTaskMem->Task, pTask, sizeof(TASK_PARAM_S));   
    pTaskMem->InUsed     = 1;
    pTaskArray->TaskNum += 1;
    pTaskArray->Head     = (pTaskArray->Head+1) % MAX_TASK_NUM; 
    
    VDH_WakeUp_Distributor(type);
    
    return VDH_OK;
}


/******************************************
 Set secure flag to info distributor
******************************************/
inline VOID VDH_WakeUp_Distributor(WAKE_UP_TYPE_E Type)
{
    VDH_COMMON_CTX_S *pCtx = NULL;
    
    VDH_GET_COMMON_CTX(pCtx);
    
    // TODO: add method to wake up distributor thread
    pCtx->SecureWakeUp = Type;
    
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
            pCtx->Scd_Module[ModuleID].SecureNeed = 1;
            break;
            
        case MODULE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pCtx->Mfde_Module[ModuleID].SecureNeed = 1;
            break;
            
        case MODULE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pCtx->Bpd_Module[ModuleID].SecureNeed = 1;
            break;
            
        case MODULE_DSP:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < DSP_MODULE_NUM), "invalid Dsp ModuleID");
            pCtx->Dsp_Module[ModuleID].SecureNeed = 1;
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
            pCtx->Scd_Module[ModuleID].SecureNeed = 0;
            break;
            
        case MODULE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pCtx->Mfde_Module[ModuleID].SecureNeed = 0;
            break;
            
        case MODULE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pCtx->Bpd_Module[ModuleID].SecureNeed = 0;
            break;
            
        case MODULE_DSP:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < DSP_MODULE_NUM), "invalid Dsp ModuleID");
            pCtx->Dsp_Module[ModuleID].SecureNeed = 0;
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
            dprint(PRN_ERROR, "Secure call VDH reset scd.\n");
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            
			VDH_Acquire_Usage(USAGE_SCD, ModuleID, SPONSOR_SECURE, 200, WITH_FORCE);
            vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&pCtx->SecureScdArray[ModuleID], 0, sizeof(TASK_ARRAY_S));
            pCtx->SecureScdArray[ModuleID].TaskNum = 0;
            if (MODULE_SECURE_WORKING == pCtx->Scd_Module[ModuleID].ModuleState)
            {
                VDH_Reset_Scd_Module(ModuleID);
                VDH_Notify_ModuleIdle(MODULE_SCD, ModuleID);
            }
            VDH_Loose_Usage(USAGE_SCD, ModuleID, SPONSOR_SECURE);
            break;
            
        case MODULE_MFDE:
            dprint(PRN_ERROR, "Secure call VDH reset mfde.\n");
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            
            VDH_Acquire_Usage(USAGE_MFDE, ModuleID, SPONSOR_SECURE, 200, WITH_FORCE);
            vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&pCtx->SecureMfdeArray[ModuleID], 0, sizeof(TASK_ARRAY_S));
            pCtx->SecureMfdeArray[ModuleID].TaskNum = 0;
            if (MODULE_SECURE_WORKING == pCtx->Mfde_Module[ModuleID].ModuleState)
            {
                VDH_Reset_Mfde_Module(ModuleID);
                VDH_Notify_ModuleIdle(MODULE_MFDE, ModuleID);
            }
            VDH_Loose_Usage(USAGE_MFDE, ModuleID, SPONSOR_SECURE);
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
    
    pCtx->Dsp_Code[DspID].SecureRequire = Type;
    dprint(PRN_ERROR, "Secure require dsp code type %d\n", Type);
    
    return VDH_OK;
}


