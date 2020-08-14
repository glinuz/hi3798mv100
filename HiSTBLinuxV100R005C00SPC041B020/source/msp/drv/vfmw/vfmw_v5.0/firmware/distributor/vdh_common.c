/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpDqgHNYPPYlCl/pP8CHYYltgiRqk3H0yIbFuqar
+eidwcF7WM8ANkOENMw/NfemVe0UG2kijl32IpvcyWpOucKEOR220i4WGk4/sf6cdl8c28/p
e8SqN70sF4RCooW1MMx6kgZvxClQz87V4cJsv6O+fE6/IKvqylaz1h7cV7lRyg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_common.c
 *
 * Purpose: vdh hardware abstract layer common file
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */


#include "public.h"
#include "vdh_common.h"
#include "vdh_module.h"


/*=============== LOCAL DEFINITION =============*/


/*================= LOCAL MACRO ================*/


/*================= LOCAL VALUE ================*/
// Control value, can shut down distributor by proc write
UINT8 g_HalDisable = 0;


/*============== INTERNAL FUNCTION =============*/


/*============== EXTERNAL FUNCTION =============*/

/******************************************
 Client notify distributor module idle
******************************************/
VOID VDH_Notify_ModuleIdle(MODULE_TYPE_E Module, SINT32 ModuleID)
{
    VDH_COMMON_CTX_S *pCtx = NULL;
    
    VDH_GET_COMMON_CTX(pCtx);

    switch (Module)
    {
        case MODULE_SCD:
            VDH_ASSERT((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pCtx->Scd_Module[ModuleID].ModuleState = MODULE_IDLE;
            VDH_WakeUp_Distributor(WAKE_UP_SCD_IDLE);
            break;
        case MODULE_MFDE:
            VDH_ASSERT((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pCtx->Mfde_Module[ModuleID].ModuleState = MODULE_IDLE;
            VDH_WakeUp_Distributor(WAKE_UP_MFDE_IDLE);
            break;
        default:
            dprint(PRN_FATAL, "%s unsupport module %d\n", __func__, Module);
            break;
    }
    
    return;    
}


/******************************************
 Acquire Usage  
******************************************/
SINT32 VDH_Acquire_Usage(USAGE_MODULE_E Usage, SINT32 ModuleID, SPONSOR_TYPE_E Sponsor, UINT32 MaxWaitInMs, UINT8 ForceFlag)
{
    UINT8  *pMaster = NULL;
    UINT8  *pGuest0 = NULL;
    UINT8  *pGuest1 = NULL;
    UINT32 CurWaitInMs   = 0;
    UINT32 StartTimeInMs = 0;	
    UINT32 CurTimeInMs   = 0;
    VDH_USAGE_S      *pUsage = NULL;
    VDH_COMMON_CTX_S *pCtx   = NULL;
    
    VDH_GET_COMMON_CTX_RET(pCtx);

    // get usage struct
    switch (Usage)
    {
        case USAGE_SCD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pUsage = &(pCtx->Scd_Module[ModuleID].Usage);
            break;
        case USAGE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pUsage = &(pCtx->Mfde_Module[ModuleID].Usage);
            break;
        case USAGE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pUsage = &(pCtx->Bpd_Module[ModuleID].Usage);
            break;
        default:
            dprint(PRN_ERROR, "%s unsupport Usage %d!\n", __func__, Usage);
            return VDH_ERR;
    }
    
    // get usage member
    switch (Sponsor)
    {
        case SPONSOR_DISTR:
            pMaster = &(pUsage->DistrUsage);
            pGuest0 = &(pUsage->NormalUsage);
            pGuest1 = &(pUsage->SecureUsage);
            break;
        case SPONSOR_NORMAL:
            pMaster = &(pUsage->NormalUsage);
            pGuest0 = &(pUsage->DistrUsage);
            pGuest1 = &(pUsage->SecureUsage);
            break;
        case SPONSOR_SECURE:
            pMaster = &(pUsage->SecureUsage);
            pGuest0 = &(pUsage->DistrUsage);
            pGuest1 = &(pUsage->NormalUsage);
            break;
        default:
            dprint(PRN_ERROR, "%s unsupport sponsor %d!\n", __func__, Sponsor);
            return VDH_ERR;
    }

    // try to get usage
	StartTimeInMs = vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInMs();
    
    do 
    {
        // first check
        if (0 == (*pGuest0) && 0 == (*pGuest1))
        {
            (*pMaster) = 1;
            
            // second check
            if (0 == (*pGuest0) && 0 == (*pGuest1))
            {
                break;
            }
            
            // second check failed, loose usage, try again
            dprint(PRN_FATAL, "Sponsor %d second wait usage %d failed, try again!\n", Sponsor, Usage);
        }
        (*pMaster) = 0;
        
        CurTimeInMs = vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInMs();
        if (CurTimeInMs < StartTimeInMs)
        {
            StartTimeInMs = 0;
        }
        CurWaitInMs = CurTimeInMs - StartTimeInMs;

        if (SPONSOR_DISTR == Sponsor)
        {
            vfmw_Osal_Func_Ptr_S.pfun_Osal_msleep(5);
        }
    }while(CurWaitInMs < MaxWaitInMs);

    if (CurWaitInMs < MaxWaitInMs)
    {
        return VDH_OK;
    }
    else
    {
        if (WITH_FORCE == ForceFlag)
        {
            dprint(PRN_ERROR, "Sponsor %d wait usage %d time out(%d ms), Force to get it!\n", Sponsor, Usage, MaxWaitInMs, *pGuest0, *pGuest1);
            (*pMaster) = 1;
            return VDH_OK;
        }
        else
        {
            dprint(PRN_FATAL, "Sponsor %d wait usage %d time out(%d ms), Guest0 = %d, Guest1 = %d!\n", Sponsor, Usage, MaxWaitInMs, *pGuest0, *pGuest1);
            return VDH_ERR;
        }
    }
}


/******************************************
 Loose Usage 
******************************************/
SINT32 VDH_Loose_Usage(USAGE_MODULE_E Usage, SINT32 ModuleID, SPONSOR_TYPE_E Sponsor)
{
    UINT8 *pMaster = NULL;
    VDH_USAGE_S      *pUsage = NULL;
    VDH_COMMON_CTX_S *pCtx = NULL;
    
    VDH_GET_COMMON_CTX_RET(pCtx);

    // get usage struct
    switch (Usage)
    {
        case USAGE_SCD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < SCD_MODULE_NUM), "invalid Scd ModuleID");
            pUsage = &(pCtx->Scd_Module[ModuleID].Usage);
            break;
        case USAGE_MFDE:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < MFDE_MODULE_NUM), "invalid Mfde ModuleID");
            pUsage = &(pCtx->Mfde_Module[ModuleID].Usage);
            break;
        case USAGE_BPD:
            VDH_ASSERT_RET((ModuleID >= 0 && ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
            pUsage = &(pCtx->Bpd_Module[ModuleID].Usage);
            break;
        default:
            dprint(PRN_ERROR, "%s unsupport Usage %d!\n", __func__, Usage);
            return VDH_ERR;
    }
    
    // get usage member
    switch (Sponsor)
    {
        case SPONSOR_DISTR:
            pMaster = &(pUsage->DistrUsage);
            break;
        case SPONSOR_NORMAL:
            pMaster = &(pUsage->NormalUsage);
            break;
        case SPONSOR_SECURE:
            pMaster = &(pUsage->SecureUsage);
            break;
        default:
            dprint(PRN_ERROR, "%s unsupport sponsor %d!\n", __func__, Sponsor);
            return VDH_ERR;
    }

    // loose usage
    (*pMaster) = 0;
    
    return VDH_OK;
}


/******************************************
 Get Parameter 
******************************************/
SINT32 VDH_Get_Parameter(PARAM_INDEX_E Index, VOID *pParam)
{
    VDH_COMMON_CTX_S *pCtx        = NULL;
    PARAM_STATE_S    *pStateParam = NULL;
    
    VDH_ASSERT_RET(NULL != pParam, "pParam = NULL");
    
    VDH_GET_COMMON_CTX_RET(pCtx);

    // TODO: extend other case if needed
    switch(Index)
    {
        case PARAM_INDEX_STATE:
            pStateParam = (PARAM_STATE_S *)pParam;
            if (MODULE_BPD == pStateParam->Type)
            {
                VDH_ASSERT_RET((pStateParam->ModuleID >= 0 && pStateParam->ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
                pStateParam->State = pCtx->Bpd_Module[pStateParam->ModuleID].ModuleState;
            }
            else
            {
                dprint(PRN_ERROR, "%s Index %d unsupport module type %d!\n", __func__, Index, pStateParam->Type);
            }
            break;
            
        default:
            dprint(PRN_ERROR, "%s unsupport index %d!\n", __func__, Index);
            return VDH_ERR;
    }
    
    return VDH_OK;
}


/******************************************
 Set Parameter 
******************************************/
SINT32 VDH_Set_Parameter(PARAM_INDEX_E Index, VOID *pParam)
{
    VDH_COMMON_CTX_S *pCtx        = NULL;
    PARAM_STATE_S    *pStateParam = NULL;
    
    VDH_ASSERT_RET(NULL != pParam, "pParam = NULL");
    
    VDH_GET_COMMON_CTX_RET(pCtx);
    
    // TODO: extend other case if needed
    switch(Index)
    {
        case PARAM_INDEX_STATE:
            pStateParam = (PARAM_STATE_S *)pParam;
            if (MODULE_BPD == pStateParam->Type)
            {
                VDH_ASSERT_RET((pStateParam->ModuleID >= 0 && pStateParam->ModuleID < BPD_MODULE_NUM), "invalid Bpd ModuleID");
                pCtx->Bpd_Module[pStateParam->ModuleID].ModuleState = pStateParam->State;
                if (MODULE_NORMAL_WORKING == pStateParam->State)
                {
                    pCtx->Bpd_Module[pStateParam->ModuleID].NormalStartCount++;
                }
                else if (MODULE_SECURE_WORKING == pStateParam->State)
                {
                    pCtx->Bpd_Module[pStateParam->ModuleID].SecureStartCount++;
                }
            }
            else
            {
                dprint(PRN_ERROR, "%s unsupport module type %d!\n", __func__, pStateParam->Type);
            }
            break;
            
        default:
            dprint(PRN_ERROR, "%s unsupport index %d!\n", __func__, Index);
            return VDH_ERR;
    }
    
    return VDH_OK;
}


/******************************************
 Record Reg Data into Task 
******************************************/
VOID VDH_Record_RegData(TASK_PARAM_S *pTask, REG_BASE_E Base, UINT32 Reg, UINT32 Data)
{
    VDH_ASSERT(NULL != pTask, "pTask = NULL");
    
    if (REG_CRG == Base)
    {
        VDH_Record_CrgRegData(&(pTask->CRG_Regs), Reg, Data);
    }
    else if (REG_SCD == Base)
    {
        VDH_Record_ScdRegData(&(pTask->SCD_Regs), Reg, Data);
    }
    else if (REG_MFDE == Base)
    {
        VDH_Record_MfdeRegData(&(pTask->MFDE_Regs), Reg, Data);
    }
    else
    {
        dprint(PRN_ERROR, "%s unsupport task type %d!\n", __func__, pTask->Type);
    }
    
    return;
}

