
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_x.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_isr.h"
#include "drv_disp_osal.h"
#include "hi_drv_sys.h"
#include "drv_display.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 s_DispISRMngrInitFlag = -1;
static DISP_ISR_M_S s_DispISRMngr;
DEFINE_SPINLOCK(g_IsrListOptLock);


#define DispCheckNullPointer(ptr) \
    {                                \
        if (!ptr)                    \
        {                            \
            DISP_ERROR("DISP ERROR! Input null pointer in %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_NULL_PTR;  \
        }                             \
    }
HI_S32 DISP_ISR_SwitchIntterrup(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType, HI_BOOL bEnable)
{
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    //printk("Open enDisp=%d int %d\n", enDisp, eType);
    DispCheckNullPointer( pfOpt);
    DispCheckNullPointer( pfOpt->PF_SetIntEnable);
    switch(eType)
    {
        case HI_DRV_DISP_C_INTPOS_0_PERCENT:
            if (HI_DRV_DISPLAY_0 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D0_0_PERCENT, bEnable);
            }
            else if (HI_DRV_DISPLAY_1 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D1_0_PERCENT, bEnable);
            }
            break;
        case HI_DRV_DISP_C_INTPOS_90_PERCENT:
            if (HI_DRV_DISPLAY_0 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D0_90_PERCENT, bEnable);
            }
            else if (HI_DRV_DISPLAY_1 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D1_90_PERCENT, bEnable);
            }
            break;
        case HI_DRV_DISP_C_INTPOS_100_PERCENT:
            if (HI_DRV_DISPLAY_0 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D0_100_PERCENT, bEnable);
            }
            else if (HI_DRV_DISPLAY_1 ==  enDisp)
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D1_100_PERCENT, bEnable);
            }
            break;
        case HI_DRV_DISP_C_DHD0_WBC:
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_WBCDHD_PARTFNI, bEnable);
            }
            break;
         case HI_DRV_DISP_C_VID_WBC:
            {
                pfOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_WBC_VP, bEnable);
            }
            break;
         case HI_DRV_DISP_C_SMMU:
            {
                pfOpt->PF_SetSMMUIntEnable( bEnable);
            }
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}


HI_S32 DISP_ISR_ResetChn(HI_DRV_DISPLAY_E enDisp)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag >= 0)
    {
        return HI_FAILURE;
    }

    pstChn = &s_DispISRMngr.stDispChn[enDisp];

    DISP_MEMSET(pstChn, 0, sizeof(DISP_ISR_CHN_S));

    pstChn->enDisp = enDisp;
    pstChn->stCBInfo.eEventType = HI_DRV_DISP_C_EVET_NONE;

    pstChn->bEnable = HI_FALSE;

    return HI_SUCCESS;
}



HI_S32 DISP_ISR_Init(HI_VOID)
{
    HI_DRV_DISPLAY_E enDisp;

    if (s_DispISRMngrInitFlag >= 0)
    {
        return HI_SUCCESS;
    }

    DISP_MEMSET(&s_DispISRMngr, 0, sizeof(DISP_ISR_M_S));

    for (enDisp=HI_DRV_DISPLAY_0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
    {
        DISP_ISR_ResetChn(enDisp);
    }

    s_DispISRMngrInitFlag++;

    return HI_SUCCESS;
}


HI_S32 DISP_ISR_DeInit(HI_VOID)
{
    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_SUCCESS;
    }

    DISP_MEMSET(&s_DispISRMngr, 0, sizeof(DISP_ISR_M_S));

    s_DispISRMngrInitFlag--;

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Suspend(HI_VOID)
{
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer( pfOpt->PF_GetIntSetting);

    pfOpt->PF_GetIntSetting(&s_DispISRMngr.u32IntMaskSave4Suspend);

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Resume(HI_VOID)
{
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_SetIntEnable);

    pfOpt->PF_SetIntEnable(s_DispISRMngr.u32IntMaskSave4Suspend, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_OpenChn(HI_DRV_DISPLAY_E enDisp)
{
    if ( (s_DispISRMngrInitFlag < 0) || (enDisp >= HI_DRV_DISPLAY_BUTT))
    {
        return HI_FAILURE;
    }

    if (s_DispISRMngr.stDispChn[enDisp].bEnable == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    DISP_ISR_ResetChn(enDisp);

    s_DispISRMngr.stDispChn[enDisp].bEnable = HI_TRUE;

    s_DispISRMngr.u32ChnNumber++;

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_CloseChn(HI_DRV_DISPLAY_E enDisp)
{
    if ( (s_DispISRMngrInitFlag < 0) || (enDisp >= HI_DRV_DISPLAY_BUTT))
    {
        return HI_FAILURE;
    }

    if (s_DispISRMngr.stDispChn[enDisp].bEnable == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    s_DispISRMngr.stDispChn[enDisp].bEnable = HI_FALSE;

    s_DispISRMngr.u32ChnNumber--;

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SearchNode(DISP_ISR_CHN_S *pstChn, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                           HI_DRV_DISP_CALLBACK_S *pstCB)
{
    HI_U32 u, v;

    v = pstChn->stList[eType].u32NodeFlag;

    for (u = 0; u < DEF_DISP_ISR_LIST_LENGTH && v; u++)
    {
        if (    ( v & (1<<u) )
            && (pstChn->stList[eType].stNode[u].pfDISP_Callback == pstCB->pfDISP_Callback)
            && (pstChn->stList[eType].stNode[u].hDst== pstCB->hDst)
            )
        {
            return (HI_S32)u;
        }
    }

    return -1;
}

HI_S32 DISP_ISR_SearchNullNode(DISP_ISR_CHN_S *pstChn, HI_DRV_DISP_CALLBACK_TYPE_E eType)
{
    HI_S32 i, v;

    for (i=0; i<DEF_DISP_ISR_LIST_LENGTH; i++)
    {
        v = 1 << i;
        if (!(pstChn->stList[eType].u32NodeFlag & v) )
        {
            return i;
        }
    }

    return -1;
}


static HI_U32 s_DispIntTable[HI_DRV_DISPLAY_BUTT][HI_DRV_DISP_C_TYPE_BUTT] =
{
//NONE, SHOW_MODE, INTPOS_0_PERCENT, INTPOS_90_PERCENT,       GFX_WBC, REG_UP
{0, DISP_INTERRUPT_D0_0_PERCENT, DISP_INTERRUPT_D0_90_PERCENT,DISP_INTERRUPT_D0_100_PERCENT, 0, 0,0},
{0, DISP_INTERRUPT_D1_0_PERCENT, DISP_INTERRUPT_D1_90_PERCENT, DISP_INTERRUPT_D1_100_PERCENT,DISP_INTERRUPT_WBCDHD_PARTFNI, DISP_INTERRUPT_WBC_VP,0},
{0, 0, 0, 0, 0,0,0},
};

HI_S32 DISP_ISR_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCB)
{
    DISP_ISR_CHN_S *pstChn;
    HI_U32 u32NodeFlagNew;
    HI_S32 index;
    HI_SIZE_T irqflag = 0;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    DispCheckID(enDisp);

    spin_lock_irqsave(&g_IsrListOptLock, irqflag);
    pstChn = &s_DispISRMngr.stDispChn[enDisp];
    if (pstChn->bEnable != HI_TRUE)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
        return HI_FAILURE;
    }


    //printk("DISP_ISR_RegCallback  disp=%d, type=%d\n", enDisp, eType);
    index = DISP_ISR_SearchNullNode(pstChn, eType);
    if (index < 0)
    {
        DISP_ERROR("DISP %d  callback reach max number!\n", enDisp);
        spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
        return HI_FAILURE;
    }



    // record callback info
    pstChn->stList[eType].stNode[index].hDst = pstCB->hDst;
    pstChn->stList[eType].stNode[index].eCallBackPrior  = pstCB->eCallBackPrior;
    pstChn->stList[eType].stNode[index].pfDISP_Callback = pstCB->pfDISP_Callback;

    //update display channel node flag
    u32NodeFlagNew = pstChn->stList[eType].u32NodeFlag;
    u32NodeFlagNew = u32NodeFlagNew | (1 << index);
    pstChn->stList[eType].u32NodeFlag = u32NodeFlagNew;

    // enable interrupt
    if (pstChn->stList[eType].u32NodeFlag)
    {
        DISP_ISR_SwitchIntterrup(enDisp, eType, HI_TRUE);
    }

    spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCB)
{
    DISP_ISR_CHN_S *pstChn;
    HI_U32 u32NodeFlagNew;
    HI_S32 index;
    HI_SIZE_T irqflag = 0;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    DispCheckID(enDisp);

    spin_lock_irqsave(&g_IsrListOptLock, irqflag);
    pstChn = &s_DispISRMngr.stDispChn[enDisp];

    index = DISP_ISR_SearchNode(pstChn, eType, pstCB);
    if (index < 0)
    {
        DISP_ERROR("Callback is not exist!\n");
        spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
        return HI_FAILURE;
    }

    // update node flag
    u32NodeFlagNew = pstChn->stList[eType].u32NodeFlag;
    u32NodeFlagNew = u32NodeFlagNew & (~(1 << index));
    pstChn->stList[eType].u32NodeFlag = u32NodeFlagNew;

    /* clear node record */
    pstChn->stList[eType].stNode[index].pfDISP_Callback = HI_NULL;
    pstChn->stList[eType].stNode[index].hDst = HI_NULL;

    if (!pstChn->stList[eType].u32NodeFlag)
    {
        DISP_ISR_SwitchIntterrup(enDisp, eType, HI_FALSE);
    }

    spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SetEvent(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_EVENT_E eEvent)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = &s_DispISRMngr.stDispChn[enDisp];
    if (pstChn->bEnable != HI_TRUE)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }

    pstChn->stCBInfo.eEventType = eEvent;

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SetDispInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = &s_DispISRMngr.stDispChn[enDisp];
    if (pstChn->bEnable != HI_TRUE)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }

    pstChn->stCBInfo.stDispInfo = *pstDispInfo;

    return HI_SUCCESS;
}

//#define DEF_DISP_ISR_Main_RETURN_VALUE HI_SUCCESS
#define DEF_DISP_ISR_Main_RETURN_VALUE IRQ_HANDLED

static HI_U32 s_DebugMyIntCount = 0;
#define DEF_DEBUG_DISP_INT_MAX_NUMBER 1000
HI_VOID DISP_ISR_Execute_ByPriorty(DISP_ISR_CHN_S *pstDisp,
                                   HI_DRV_DISP_CALLBACK_TYPE_E eIntType)
{
    HI_U32 u32IsrBitMap = 0, n = 0;
    HI_DRV_DISP_CALLBACK_PRIORTY_E eIsrExecutePriorty  = HI_DRV_DISP_CALLBACK_PRIORTY_BUTT;

    u32IsrBitMap = pstDisp->stList[eIntType].u32NodeFlag;

#if 1
    for (eIsrExecutePriorty = HI_DRV_DISP_CALLBACK_PRIORTY_HIGH;
         eIsrExecutePriorty < HI_DRV_DISP_CALLBACK_PRIORTY_BUTT;
         eIsrExecutePriorty ++)
#endif
    {
        for (n = 0; (n < DEF_DISP_ISR_LIST_LENGTH) && u32IsrBitMap; n++)
        {
            if (u32IsrBitMap & (1 << n))
            {
                if ((pstDisp->stList[eIntType].stNode[n].pfDISP_Callback)
                     && (pstDisp->stList[eIntType].stNode[n].eCallBackPrior == eIsrExecutePriorty))
                {
                    pstDisp->stList[eIntType].stNode[n].pfDISP_Callback(pstDisp->stList[eIntType].stNode[n].hDst, &pstDisp->stCBInfo);
                    u32IsrBitMap = u32IsrBitMap - (1 << n);
                }

                if( !irqs_disabled() )
                {
                    DISP_PRINT("#######$$$$$$$$$$$............eIntType=%u, n=%d\n",eIntType, n);
                }
            }
        }

        if (!u32IsrBitMap)
        {
            break;
        }
    }


    return;
}

HI_S32 DISP_ISR_Main(HI_S32 irq, HI_VOID *dev_id)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_DRV_DISP_CALLBACK_TYPE_E eIntType;
    DISP_ISR_CHN_S *pstDisp;
    HI_U32 u32IntState = 0;
    HI_U32 u32IntState1 = 0;
    HI_S32 i;
    HI_SIZE_T irqflag = 0;
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    // if get int ops failed, return
    DispCheckNullPointer( pfOpt);
    DispCheckNullPointer( pfOpt->PF_GetMaskedIntState);
    DispCheckNullPointer( pfOpt->PF_CleanIntState);
    DispCheckNullPointer( pfOpt->FP_GetChnBottomFlag);

    // s1 get interrupt state
    pfOpt->PF_GetMaskedIntState(&u32IntState);
    pfOpt->PF_GetMaskedIntState1(&u32IntState1);

    // s2 clear interrupt state
    pfOpt->PF_CleanIntState(u32IntState);
    pfOpt->PF_CleanIntState1(u32IntState1);

    s_DebugMyIntCount++;

    // s5.0 if display is not open, return
    if (!s_DispISRMngr.u32ChnNumber)
    {
        return DEF_DISP_ISR_Main_RETURN_VALUE;
    }

    spin_lock_irqsave(&g_IsrListOptLock, irqflag);

    for(i = (HI_S32) HI_DRV_DISPLAY_1; i >= 0; i--)
    {
        enDisp = (HI_DRV_DISPLAY_E)i;
        pstDisp = &s_DispISRMngr.stDispChn[enDisp];

        if (pstDisp->bEnable != HI_TRUE)
        {
            continue;
        }

        for (eIntType=HI_DRV_DISP_C_INTPOS_0_PERCENT; eIntType<HI_DRV_DISP_C_TYPE_BUTT; eIntType++)
        {
            HI_BOOL bBtm;
            HI_U32 vcnt;

            if(!(s_DispIntTable[enDisp][eIntType] & u32IntState) )
            {
                continue;
            }

            if( s_DispIntTable[enDisp][HI_DRV_DISP_C_INTPOS_0_PERCENT] & u32IntState )
            {
                HI_U32 Ct;

                HI_DRV_SYS_GetTimeStampMs((HI_U32 *)&Ct);

                // get top and bottom flag
                pfOpt->FP_GetChnBottomFlag(enDisp, &bBtm, &vcnt);

                pstDisp->stCBInfo.stDispInfo.bIsBottomField = bBtm;
                pstDisp->stCBInfo.stDispInfo.u32Vline = vcnt;
            }

            DISP_ISR_Execute_ByPriorty(pstDisp, eIntType);
            u32IntState = u32IntState & (~(HI_U32)s_DispIntTable[enDisp][eIntType]);
        }
    }

    if (u32IntState)
    {
        DISP_FATAL("Unespexted interrup 0x%x happened!\n", u32IntState);
    }

    spin_unlock_irqrestore(&g_IsrListOptLock, irqflag);
    return DEF_DISP_ISR_Main_RETURN_VALUE;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */












