/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hal_aiao.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/

#include <asm/setup.h>
#include <asm/io.h>
#include <linux/delay.h>

#include "hi_type.h"
#include "hi_reg_common.h"
#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "hal_aiao.h"
#include "hal_aiao_priv.h"
#include "hal_aiao_func.h"
#include "audio_util.h"
#include "hi_reg_common.h"
#ifdef ALSA_DEBUG_TIME
#include <linux/time.h>
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

DEFINE_SEMAPHORE(g_HalAiaoMutex);

atomic_t  atmAiaoInitCnt;

static HI_S32 g_AiaoSuspendFlag = 0;
static HI_S32 g_AiaoResumeFlag = 0;

typedef struct
{
    HI_VOID*      pPort[AIAO_INT_BUTT];
    AIAO_IsrFunc* fRegIsr[AIAO_INT_BUTT];

#ifdef HI_AIAO_TIMER_SUPPORT
    AIAO_TimerIsrFunc* fTimerIsr[AIAO_INT_BUTT];
    HI_VOID*           timersub[AIAO_INT_BUTT];
#endif

    HI_VOID* sub[AIAO_INT_BUTT];    //only for alsa isr HI_ALSA_AI_SUPPORT
} AIAO_GLOBAL_SOURCE_S;

/* private state */
static AIAO_GLOBAL_SOURCE_S g_AIAORm;

/*
 *  AIAO interrupt handle function
 */
#ifndef AIAO_ALSA_DRV_V2
volatile HI_U32 aiao_isr_num = 0;
EXPORT_SYMBOL(aiao_isr_num);
#endif

#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
static AIAO_I2S_BOARD_CONFIG g_stI2SBoardSettings[AIAO_MAX_EXT_I2S_NUMBER];
static HI_VOID   HAL_AIAO_BoardI2SReset(HI_VOID)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;
    HI_S32 n;

    for (n = 0; n < AIAO_MAX_EXT_I2S_NUMBER; n++)
    {
        pstI2SBorad = &g_stI2SBoardSettings[n];
        pstI2SBorad->u32InitFlag = 0;
        pstI2SBorad->enTxPortID = AIAO_PORT_BUTT;
        pstI2SBorad->enRxPortID = AIAO_PORT_BUTT;
    }

#if defined (HI_I2S0_SUPPORT)
    pstI2SBorad = &g_stI2SBoardSettings[0];

    pstI2SBorad->enTxPortID = AIAO_PORT_TX0;
    pstI2SBorad->enTxSource = AIAO_TX0;
    pstI2SBorad->enTxCrgSource = AIAO_TX_CRG0;
    pstI2SBorad->enTxCrgMode = AIAO_CRG_MODE_MASTER;

    pstI2SBorad->enRxPortID = AIAO_PORT_RX0;
    pstI2SBorad->enRxSource = AIAO_RX0;
    /* hi3716cv200 aiao external i2s rx allways work at   AIAO_CRG_MODE_DUPLICATE */
    pstI2SBorad->enRxCrgSource = AIAO_TX_CRG0;
    pstI2SBorad->enRxCrgMode = AIAO_CRG_MODE_DUPLICATE;
#endif

#if defined (HI_I2S1_SUPPORT)
    pstI2SBorad = &g_stI2SBoardSettings[1];

    pstI2SBorad->enTxPortID = AIAO_PORT_TX1;
    pstI2SBorad->enTxSource = AIAO_TX1;
    pstI2SBorad->enTxCrgSource = AIAO_TX_CRG1;
    pstI2SBorad->enTxCrgMode = AIAO_CRG_MODE_MASTER;

#ifdef HI_FPGA
    pstI2SBorad->enRxPortID = AIAO_PORT_RX3;
    pstI2SBorad->enRxSource = AIAO_RX2;
    pstI2SBorad->enRxCrgSource = AIAO_TX_CRG2;
#else
    pstI2SBorad->enRxPortID = AIAO_PORT_RX1;
    pstI2SBorad->enRxSource = AIAO_RX1;
    /* hi3716cv200 aiao external i2s rx allways work at   AIAO_CRG_MODE_DUPLICATE */
    pstI2SBorad->enRxCrgSource = AIAO_TX_CRG1;
#endif
    pstI2SBorad->enRxCrgMode = AIAO_CRG_MODE_DUPLICATE;
#endif
}

static HI_BOOL AIAOValidateBoardI2S(AIAO_PORT_ID_E enPortID)
{
#if    defined(CHIP_TYPE_hi3751v100)  \
    || defined(CHIP_TYPE_hi3751v100b) \
    || defined(CHIP_TYPE_hi3751v600)  \
    || defined(CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)  \
    || defined(CHIP_TYPE_hi3751v620)
    if ((AIAO_PORT_TX0 == enPortID) || (AIAO_PORT_TX6 == enPortID) ||
        (AIAO_PORT_RX0 == enPortID) || (AIAO_PORT_RX3 == enPortID))
#else
    if ((AIAO_PORT_TX0 == enPortID) || (AIAO_PORT_TX1 == enPortID) ||
        (AIAO_PORT_RX0 == enPortID) || (AIAO_PORT_RX1 == enPortID))
#endif
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID AIAOSetI2SClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttr)
{
    HI_S32 n;

    for (n = 0; n < AIAO_MAX_EXT_I2S_NUMBER; n++)
    {
        if (g_stI2SBoardSettings[n].enRxPortID == enPortID && !(g_stI2SBoardSettings[n].u32InitFlag & 0x2))
        {
            // Set Rx port in use of Tx clock
            if (AIAO_CRG_MODE_DUPLICATE == pstIfAttr->enCrgMode)
            {
                (HI_VOID)iHAL_AIAO_P_SetI2SMasterClk(g_stI2SBoardSettings[n].enTxPortID, pstIfAttr);
            }
            else if (AIAO_CRG_MODE_SLAVE == pstIfAttr->enCrgMode)
            {
                (HI_VOID)iHAL_AIAO_P_SetI2SlaveClk(g_stI2SBoardSettings[n].enTxPortID, pstIfAttr);
            }
        }
    }
}
#endif

static HI_S32 AIAOCheckBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S* pstIfAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;

#if defined(HI_I2S0_SUPPORT) || defined(HI_I2S1_SUPPORT)
    HI_BOOL bValidBoardI2S = AIAOValidateBoardI2S(enPortID);

    if (HI_TRUE == bValidBoardI2S)
    {
        s32Ret = HAL_AIAO_P_CheckBoardI2SOpenAttr(enPortID, pstIfAttr);
    }
#endif

    return s32Ret;
}

static HI_VOID AIAOSetBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S* pstIfAttr)
{
#if defined(HI_I2S0_SUPPORT) || defined(HI_I2S1_SUPPORT)
    HI_BOOL bValidBoardI2S = AIAOValidateBoardI2S(enPortID);

    if (HI_TRUE == bValidBoardI2S)
    {
        HAL_AIAO_P_SetBoardI2SOpenAttr(enPortID, pstIfAttr);
        AIAOSetI2SClk(enPortID, (AIAO_IfAttr_S*)pstIfAttr);
    }
#endif
}

#ifdef HI_AIAO_TIMER_SUPPORT
static void  AIAOTimerIsr(HI_U32 Id)
{
    AIAO_TIMER_ID_E enTimerID;

    enTimerID = ID2TIMER(Id);

    if (g_AIAORm.fTimerIsr[Id])
    {
        g_AIAORm.fTimerIsr[Id](enTimerID, g_AIAORm.timersub[Id]);
    }

    iHAL_TIMER_ClearTimer(enTimerID);
}
#endif

static void  AIAOPortIsr(HI_U32 Id)
{
    HI_U32 PortIntRawStatus;
    AIAO_PORT_S* pstPort;
    AIAO_PORT_ID_E enPortId;
    enPortId = ID2PORT(Id);
    PortIntRawStatus = iHAL_AIAO_P_GetIntStatusRaw(enPortId);
    pstPort = (AIAO_PORT_S*)g_AIAORm.pPort[Id];

    if (pstPort)
    {
        if (g_AIAORm.fRegIsr[Id])
        {
            g_AIAORm.fRegIsr[Id](enPortId, PortIntRawStatus, g_AIAORm.sub[Id]);
        }
    }

    iHAL_AIAO_P_ClrInt(enPortId, PortIntRawStatus);
}

static irqreturn_t AIAOIsr(int irq, void* dev_id)
{
    HI_U32 Id;
    HI_U32 TopIntStatus;
    TopIntStatus = iHAL_AIAO_GetTopIntStatus();

    for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
    {
        if (IntStatus(Id, TopIntStatus))
        {
#ifdef HI_AIAO_TIMER_SUPPORT
            if ((Id >= AIAO_TIMER_INT0) && (Id <= AIAO_TIMER_INT1))
            {
                AIAOTimerIsr(Id);
            }
            else
#endif
            {
                AIAOPortIsr(Id);
            }
        }
    }

    return IRQ_HANDLED;
}

HI_VOID    HAL_AIAO_PowerOff(HI_VOID)
{
    HI_S32 Ret;
    Ret = down_interruptible(&g_HalAiaoMutex);

    if (HI_SUCCESS != Ret)
    {
        return;
    }

    AIAO_HW_PowerOff();

    up(&g_HalAiaoMutex);

    return;
}

HI_S32    HAL_AIAO_Suspend(HI_VOID)
{
    if (!g_AiaoSuspendFlag)
    {
        HAL_AIAO_FreeIsr();
        g_AiaoSuspendFlag = 1;
    }

    HAL_AIAO_PowerOff();
    g_AiaoResumeFlag = 0;

    return HI_SUCCESS;
}

HI_S32    HAL_AIAO_Resume(HI_VOID)
{
    HI_S32 Ret;

    if (!g_AiaoResumeFlag)
    {

        Ret = HAL_AIAO_PowerOn();

        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_AIAO("AIAO hw reset fail\n");
            return HI_FAILURE;
        }

        Ret = HAL_AIAO_RequestIsr();
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_AIAO("AIAO request isr fail\n");
            return HI_FAILURE;
        }

#if defined (HI_AIAO_TIMER_SUPPORT)
        //support hard Time, so can't enbale timer0(bit[27]) and timer1(bit[28]) interrupt
        iHAL_AIAO_SetTopInt(0x07ffffff);  /* enable all top interrupt */
#else
        //don't support hard Time
        iHAL_AIAO_SetTopInt(0xffffffff);  /* enable all top interrupt */
#endif
        g_AiaoResumeFlag = 1;
    }

    g_AiaoSuspendFlag = 0;
    return HI_SUCCESS;
}

HI_S32    HAL_AIAO_PowerOn(HI_VOID)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);

#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
    HAL_AIAO_BoardI2SReset();
#endif

    Ret = AIAO_HW_PowerOn();

    up(&g_HalAiaoMutex);
    return Ret;
}

HI_S32    HAL_AIAO_RequestIsr(HI_VOID)
{
    if (request_irq(AIAO_IRQ_NUM, AIAOIsr, IRQF_DISABLED, "hi_aiao_irq", NULL) != 0)
    {
        HI_FATAL_AIAO("request_irq failed irq num =%d!\n", AIAO_IRQ_NUM);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID    HAL_AIAO_FreeIsr(HI_VOID)
{
    /* free irq */
    free_irq(AIAO_IRQ_NUM, NULL);
}

/* global function */
HI_S32                  HAL_AIAO_Init(HI_VOID)
{
    HI_S32 Ret;
    HI_U32 Id;

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (atomic_inc_return(&atmAiaoInitCnt) == 1)
    {
        if (HI_SUCCESS != iHAL_AIAO_Init())
        {
            HI_FATAL_AIAO("iHAL_AIAO_Init failed \n");
            up(&g_HalAiaoMutex);
            return HI_FAILURE;
        }

        /* init aiao state */
        for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
        {
            g_AIAORm.pPort[Id] = HI_NULL;
        }

        if (HI_SUCCESS != HAL_AIAO_RequestIsr())
        {
            HI_FATAL_AIAO("request_irq failed\n");
            up(&g_HalAiaoMutex);
            return HI_FAILURE;
        }

#ifdef HI_AIAO_TIMER_SUPPORT
        iHAL_AIAO_SetTopInt(0x07ffffff);  /* enable all top interrupt */
#else
        iHAL_AIAO_SetTopInt(0xffffffff);  /* enable all top interrupt */
#endif

#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
        HAL_AIAO_BoardI2SReset();
#endif

    }

    up(&g_HalAiaoMutex);
    return HI_SUCCESS;
}

HI_VOID                 HAL_AIAO_DeInit(HI_VOID)
{
    HI_S32 Ret;
    HI_U32 Id;

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (atomic_dec_return(&atmAiaoInitCnt) == 0)
    {
        iHAL_AIAO_SetTopInt(0);  /* disable all top interrupt */
        HAL_AIAO_FreeIsr();

        /* close port */
        for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
        {
            if (g_AIAORm.pPort[Id])
            {
                iHAL_AIAO_P_Close((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
            }

            g_AIAORm.pPort[Id]   = HI_NULL;
            g_AIAORm.fRegIsr[Id] = HI_NULL;
        }

        iHAL_AIAO_DeInit();
    }

    up(&g_HalAiaoMutex);
}

HI_VOID                 HAL_AIAO_GetHwCapability(HI_U32* pu32Capability)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    iHAL_AIAO_GetHwCapability(pu32Capability);
    up(&g_HalAiaoMutex);
}

HI_VOID                 HAL_AIAO_GetHwVersion(HI_U32* pu32Version)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    iHAL_AIAO_GetHwVersion(pu32Version);
    up(&g_HalAiaoMutex);
}

HI_VOID                 HAL_AIAO_DBG_RWReg(AIAO_Dbg_Reg_S* pstReg)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    iHAL_AIAO_DBG_RWReg(pstReg);
    up(&g_HalAiaoMutex);
}

HI_VOID                 HAL_AIAO_SetTopInt(HI_U32 u32Multibit)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    iHAL_AIAO_SetTopInt(u32Multibit);
    up(&g_HalAiaoMutex);
}

HI_U32                  HAL_AIAO_GetTopIntRawStatus(HI_VOID)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    Ret = iHAL_AIAO_GetTopIntRawStatus();
    up(&g_HalAiaoMutex);
    return Ret;
}

HI_U32                  HAL_AIAO_GetTopIntStatus(HI_VOID)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);
    Ret = iHAL_AIAO_GetTopIntStatus();
    up(&g_HalAiaoMutex);
    return Ret;
}

/* global port function */
HI_S32                  HAL_AIAO_P_Open(AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S* pstConfig)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 Id = PORT2ID(enPortID);
    AIAO_PORT_S*  pstPort;
    AIAO_IsrFunc* pIsrFunc;

    s32Ret = down_interruptible(&g_HalAiaoMutex);

    if (HI_NULL != g_AIAORm.pPort[Id])
    {
        HI_ERR_AO("AIAO Port(%d) had been open!\n", Id);
        up(&g_HalAiaoMutex);
        return HI_FAILURE;
    }

    s32Ret = AIAOCheckBoardI2SOpenAttr(enPortID, &pstConfig->stIfAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AIAOCheckBoardI2SOpenAttr failed(0x%x)!\n", s32Ret);
        up(&g_HalAiaoMutex);
        return s32Ret;
    }

    s32Ret = iHAL_AIAO_P_Open(enPortID, pstConfig, &pstPort, &pIsrFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("iHAL_AIAO_P_Open failed(0x%x)!\n", s32Ret);
        up(&g_HalAiaoMutex);
        return s32Ret;
    }

    g_AIAORm.pPort[Id]   = (HI_VOID*)pstPort;
    g_AIAORm.fRegIsr[Id] = pIsrFunc;
    g_AIAORm.sub[Id]     = pstConfig->pSubstream;

    AIAOSetBoardI2SOpenAttr(enPortID, &pstConfig->stIfAttr);

#if defined (CHIP_TYPE_hi3751v100) || defined (CHIP_TYPE_hi3751v100b)
    if ((AIAO_PORT_RX2 == enPortID) || (AIAO_PORT_TX3 == enPortID) ||
        (AIAO_PORT_TX4 == enPortID) || (AIAO_PORT_TX5 == enPortID))
    {
        (HI_VOID)iHAL_AIAO_P_SetI2SMasterClk(AIAO_PORT_TX2, &pstConfig->stIfAttr);
    }
#endif

    up(&g_HalAiaoMutex);

    return s32Ret;
}

HI_VOID                 HAL_AIAO_P_Close(AIAO_PORT_ID_E enPortID)
{
    HI_S32 Ret;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_P_Close((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
        g_AIAORm.pPort[Id] = HI_NULL;
#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
        HAL_AIAO_P_DestroyBoardI2SOpenAttr(enPortID);
#endif

    }
    up(&g_HalAiaoMutex);
}

#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
HI_S32 HAL_AIAO_P_SetOpType(AIAO_PORT_ID_E enPortID, AIAO_OP_TYPE_E enOpType)
{
    HI_U32 Id  = PORT2ID(enPortID);
    HI_S32 Ret = HI_FAILURE;
    Ret = down_interruptible(&g_HalAiaoMutex);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AIAO("down_interruptible Failed\n");
        return HI_FAILURE;
    }
    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetOpType(enPortID, enOpType);
    }
    else
    {
        HI_ERR_AIAO("g_AIAORm.hPort[Id] nulll \n");
    }
    up(&g_HalAiaoMutex);
    return Ret;
}
#endif

HI_S32 HAL_AIAO_P_SetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S* pstAttr)
{
    HI_U32 Id = PORT2ID(enPortID);
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = down_interruptible(&g_HalAiaoMutex);

    s32Ret = AIAOCheckBoardI2SOpenAttr(enPortID, &pstAttr->stIfAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AIAOCheckBoardI2SOpenAttr failed(0x%x)!\n", s32Ret);
        up(&g_HalAiaoMutex);
        return s32Ret;
    }

    if (g_AIAORm.pPort[Id])
    {
        s32Ret = iHAL_AIAO_P_SetAttr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("iHAL_AIAO_P_SetAttr failed(0x%x)!\n", s32Ret);
            up(&g_HalAiaoMutex);
            return s32Ret;
        }
    }

    AIAOSetBoardI2SOpenAttr(enPortID, &pstAttr->stIfAttr);

    up(&g_HalAiaoMutex);

    return s32Ret;
}

static HI_S32 HAL_AIAO_P_Check(const AIAO_PORT_ID_E enPortID)
{
    HI_U32 u32Capability;
    HI_U32 Id;

    HAL_AIAO_GetHwCapability(&u32Capability);

    for (Id = 0; Id < AIAO_INT_BUTT; Id++)
    {
        if (0x01 & (u32Capability >> Id))
        {
            if (ID2PORT(Id) == enPortID)
            {
                return HI_SUCCESS;
            }
        }
    }
    //HI_ERR_AIAO("HAL_AIAO_P_Check enPortID 0x%x not support\n",enPortID);
    return HI_FAILURE;
}

HI_S32  HAL_AIAO_P_Open_Veri(AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S* pstConfig)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 Id = PORT2ID(enPortID);
    AIAO_IsrFunc* pIsrFunc;
    AIAO_PORT_S*  pstPort;

    s32Ret = HAL_AIAO_P_Check(enPortID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HAL_AIAO_P_Check failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = down_interruptible(&g_HalAiaoMutex);

    if (HI_NULL != g_AIAORm.pPort[Id])
    {
        HI_ERR_AO("AIAO Port(%d) had been open!\n", Id);
        up(&g_HalAiaoMutex);
        return HI_FAILURE;
    }

    s32Ret = iHAL_AIAO_P_Open(enPortID, pstConfig, &pstPort, &pIsrFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("iHAL_AIAO_P_Open failed(0x%x)!\n", s32Ret);
        up(&g_HalAiaoMutex);
        return s32Ret;
    }

    g_AIAORm.pPort[Id]   = (HI_VOID*)pstPort;
    g_AIAORm.fRegIsr[Id] = pIsrFunc;

    up(&g_HalAiaoMutex);

    return s32Ret;
}

HI_VOID HAL_AIAO_P_Close_Veri(AIAO_PORT_ID_E enPortID)
{
    HI_S32 s32Ret;
    HI_U32 Id = PORT2ID(enPortID);

    s32Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_P_Close((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
        g_AIAORm.pPort[Id] = HI_NULL;
    }

    up(&g_HalAiaoMutex);
}

HI_S32 HAL_AIAO_P_GetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    HI_U32 Id = PORT2ID(enPortID);

    s32Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        s32Ret = iHAL_AIAO_P_GetAttr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstAttr);
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    up(&g_HalAiaoMutex);

    return s32Ret;
}

#ifdef HI_AIAO_TIMER_SUPPORT
HI_S32 HAL_AIAO_T_Create(AIAO_TIMER_ID_E enTimerID, const AIAO_Timer_Create_S* pstParam)
{
    HI_S32 Ret = HI_SUCCESS;
    AIAO_TimerIsrFunc* pIsr;
    HI_U32 Id = TIMER2ID(enTimerID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (HI_NULL == g_AIAORm.pPort[Id])
    {
        AIAO_TIMER_S pstPort;
        pIsr = pstParam->pTimerIsrFunc;
        Ret = iHAL_AIAO_T_Create(ID2TIMER(Id), pstParam, &pstPort, &pIsr);

        if (HI_SUCCESS == Ret)
        {
            g_AIAORm.pPort[Id]   = (HI_VOID*)pstPort;
            g_AIAORm.fTimerIsr[Id] = pIsr;
            g_AIAORm.timersub[Id] = pstParam->substream;;
        }
    }

    up(&g_HalAiaoMutex);
    HI_INFO_AIAO("Create Timer ID Success\n", enTimerID);

    return Ret;
}

HI_VOID  HAL_AIAO_T_Destroy(AIAO_TIMER_ID_E enTimerID)
{
    HI_S32 Ret;
    HI_U32 Id = TIMER2ID(enTimerID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_T_Destroy((AIAO_TIMER_S)g_AIAORm.pPort[Id]);
        g_AIAORm.pPort[Id] = HI_NULL;
    }

    up(&g_HalAiaoMutex);
    HI_INFO_AIAO("Destroy Timer ID Success\n", enTimerID);
}

HI_S32 HAL_AIAO_T_SetTimerAttr(AIAO_TIMER_ID_E enTimerID, const AIAO_TIMER_Attr_S* pstAttrParam)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = TIMER2ID(enTimerID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_T_SetTimerAttr((AIAO_TIMER_S)g_AIAORm.pPort[Id], pstAttrParam);
    }
    else
    {
        HI_INFO_AIAO("Set Timer %d Atrr Fail,Timer Not Create \n", enTimerID);
        up(&g_HalAiaoMutex);
        return HI_FAILURE;
    }

    up(&g_HalAiaoMutex);
    HI_INFO_AIAO("Set Timer %d config %d,Rate %d,fclk_div %d,bclk_div %d success\n", enTimerID, pstAttrParam->u32Config, pstAttrParam->stIfAttr.enRate, pstAttrParam->stIfAttr.u32FCLK_DIV, pstAttrParam->stIfAttr.u32BCLK_DIV);

    return Ret;
}

HI_S32  HAL_AIAO_T_SetTimerEnable(AIAO_TIMER_ID_E enTimerID, HI_BOOL bEnalbe)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = TIMER2ID(enTimerID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_T_SetTimerEnalbe((AIAO_TIMER_S)g_AIAORm.pPort[Id], bEnalbe);
    }
    else
    {
        HI_INFO_AIAO("Set Timer %d enable Faild, Timer Not Create \n", enTimerID);
        up(&g_HalAiaoMutex);
        return HI_FAILURE;
    }

    up(&g_HalAiaoMutex);
    HI_INFO_AIAO("Destroy Timer ID Success\n", enTimerID);

    return Ret;
}

HI_S32  HAL_AIAO_T_GetStatus(AIAO_TIMER_ID_E enTimerID, AIAO_TIMER_Status_S* Param)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = TIMER2ID(enTimerID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_T_GetStatus((AIAO_TIMER_S)g_AIAORm.pPort[Id], Param);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}
HI_VOID HAL_AIAO_T_TIMERProcess(AIAO_TIMER_ID_E enTimerID, HI_VOID* pst)
{
    HI_U32 Id = TIMER2ID(enTimerID);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_T_TIMERProcess((AIAO_TIMER_S)g_AIAORm.pPort[Id]);
    }
}
#endif

HI_S32                  HAL_AIAO_P_Start(AIAO_PORT_ID_E enPortID)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_Start((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32                  HAL_AIAO_P_Stop(AIAO_PORT_ID_E enPortID, AIAO_PORT_STOPMODE_E enStopMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_Stop((AIAO_PORT_S*)g_AIAORm.pPort[Id], enStopMode);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32                  HAL_AIAO_P_Mute(AIAO_PORT_ID_E enPortID, HI_BOOL bMute)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_Mute((AIAO_PORT_S*)g_AIAORm.pPort[Id], bMute);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSampleRate(AIAO_PORT_ID_E enPortID, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    //to do
    return HI_SUCCESS;
}


HI_S32                  HAL_AIAO_P_SetVolume(AIAO_PORT_ID_E enPortID, HI_U32 u32VolumedB)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetVolume((AIAO_PORT_S*)g_AIAORm.pPort[Id], u32VolumedB);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSpdifCategoryCode(AIAO_PORT_ID_E enPortID, AIAO_SPDIF_CATEGORYCODE_E eCategoryCode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);
    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetSpdifCategoryCode((AIAO_PORT_S*)g_AIAORm.pPort[Id], eCategoryCode);
    }

    up(&g_HalAiaoMutex);
    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSpdifSCMSMode(AIAO_PORT_ID_E enPortID, AIAO_SPDIF_SCMS_MODE_E eSCMSMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetSpdifSCMSMode((AIAO_PORT_S*)g_AIAORm.pPort[Id], eSCMSMode);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetTrackMode(AIAO_PORT_ID_E enPortID, AIAO_TRACK_MODE_E enTrackMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetTrackMode((AIAO_PORT_S*)g_AIAORm.pPort[Id], enTrackMode);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32 HAL_AIAO_P_SetBypass(AIAO_PORT_ID_E enPortID, HI_BOOL bByBass)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetBypass((AIAO_PORT_S*)g_AIAORm.pPort[Id], bByBass);
    }

    return Ret;
}

HI_S32 HAL_AIAO_P_SetMode(AIAO_PORT_ID_E enPortID, HI_BOOL bMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetMode((AIAO_PORT_S*)g_AIAORm.pPort[Id], bMode);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_GetUserCongfig(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pstUserConfig)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_GetUserCongfig((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstUserConfig);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32                  HAL_AIAO_P_GetStatus(AIAO_PORT_ID_E enPortID, AIAO_PORT_STAUTS_S* pstProcInfo)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_GetStatus((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstProcInfo);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_S32                  HAL_AIAO_P_SelectSpdifSource(AIAO_PORT_ID_E enPortID, AIAO_SPDIFPORT_SOURCE_E eSrcChnId)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SelectSpdifSource((AIAO_PORT_S*)g_AIAORm.pPort[Id], eSrcChnId);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSpdifOutPort(AIAO_PORT_ID_E enPortID, HI_S32 bEn)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetSpdifOutPort((AIAO_PORT_S*)g_AIAORm.pPort[Id], bEn);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetI2SSdSelect(AIAO_PORT_ID_E enPortID, AIAO_I2SDataSel_S*  pstSdSel)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_SetI2SSdSelect((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstSdSel);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_U32                  HAL_AIAO_P_ReadData_NotUpRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize, HI_U32* pu32Rptr, HI_U32* pu32Wptr)
{
    HI_U32 ReadBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        ReadBytes = iHAL_AIAO_P_ReadData_NotUpRptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Dest, u32DestSize, pu32Rptr, pu32Wptr);
    }

    return ReadBytes;
}

/* port buffer function */
HI_U32                  HAL_AIAO_P_ReadData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 ReadBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        ReadBytes = iHAL_AIAO_P_ReadData((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Dest, u32DestSize);
    }

    return ReadBytes;
}

HI_U32                  HAL_AIAO_P_WriteData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 WriteBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        WriteBytes = iHAL_AIAO_P_WriteData((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Src, u3SrcLen);
    }

    return WriteBytes;
}

HI_U32                  HAL_AIAO_P_PrepareData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 WriteBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        WriteBytes = iHAL_AIAO_P_PrepareData((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Src, u3SrcLen);
    }

    return WriteBytes;
}

HI_U32                  HAL_AIAO_P_QueryBufData_ProvideRptr(AIAO_PORT_ID_E enPortID, HI_U32* pu32Rptr)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);
    //HI_S32 Ret;

    //Ret = down_interruptible(&g_HalAiaoMutex);   //save pcm thread call this function

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_QueryBufData_ProvideRptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Rptr);
    }

    //up(&g_HalAiaoMutex);

    return Bytes;
}

HI_U32                  HAL_AIAO_P_QueryBufData(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_QueryBufData((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    up(&g_HalAiaoMutex);

    return Bytes;
}

HI_U32                  HAL_AIAO_P_QueryBufFree(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);
    HI_S32 Ret;

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_QueryBufFree((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    up(&g_HalAiaoMutex);

    return Bytes;
}

HI_U32                  HAL_AIAO_P_UpdateRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_UpdateRptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Dest, u32DestSize);
    }

    return Bytes;
}

HI_U32                  HAL_AIAO_P_UpdateWptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_UpdateWptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Src, u3SrcLen);
    }

    return Bytes;
}

HI_VOID                  HAL_AIAO_P_GetDelayMs(AIAO_PORT_ID_E enPortID, HI_U32* pu32Delayms)
{
    HI_U32 Id = PORT2ID(enPortID);

    *pu32Delayms = 0;

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_P_GetDelayMs((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Delayms);
        return ;
    }

    return ;
}


HI_S32                  HAL_AIAO_P_GetRbfAttr(AIAO_PORT_ID_E enPortID, AIAO_RBUF_ATTR_S* pstRbfAttr)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    Ret = down_interruptible(&g_HalAiaoMutex);

    if (g_AIAORm.pPort[Id])
    {
        Ret = iHAL_AIAO_P_GetRbfAttr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pstRbfAttr);
    }

    up(&g_HalAiaoMutex);

    return Ret;
}

HI_VOID HAL_AIAO_P_ProcStatistics(AIAO_PORT_ID_E enPortID, HI_U32 u32IntStatus, void* pst)//HI_ALSA_AI_SUPPORT
{
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_P_ProcStatistics((AIAO_PORT_S*)g_AIAORm.pPort[Id], u32IntStatus);
    }
}

HI_VOID HAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_ID_E enPortID, HI_S32 bEn)
{
    iHAL_AIAO_P_SetSPDIFPortEn(enPortID, bEn);
}

#ifdef HI_ALSA_AI_SUPPORT
HI_U32  HAL_AIAO_P_ALSA_UpdateRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_ALSA_UpdateRptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Dest, u32DestSize);
    }

    return Bytes;
}

HI_U32  HAL_AIAO_P_ALSA_UpdateWptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_ALSA_UpdateWptr((AIAO_PORT_S*)g_AIAORm.pPort[Id], pu32Dest, u32DestSize);
    }

    return Bytes;
}

HI_U32  HAL_AIAO_P_ALSA_FLASH(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        iHAL_AIAO_P_ALSA_FLASH((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    return  HI_SUCCESS;
}

HI_U32 HAL_AIAO_P_ALSA_QueryWritePos(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_ALSA_QueryWritePos((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    return Bytes;
}

HI_U32 HAL_AIAO_P_ALSA_QueryReadPos(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.pPort[Id])
    {
        Bytes = iHAL_AIAO_P_ALSA_QueryReadPos((AIAO_PORT_S*)g_AIAORm.pPort[Id]);
    }

    return Bytes;
}
#endif

static AIAO_PORT_USER_CFG_S g_stAiaoTxI2SDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_2,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxHdmiHbrSDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_8,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_192K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize * 16,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_TRUE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxHdmiI2SSDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_2,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
    .enOpType              = SND_OP_TYPE_I2S,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxSpdDefaultOpenAttr =
{
    .stIfAttr             =
    {
        .enCrgMode        = AIAO_CRG_MODE_MASTER,
        .enChNum          = AIAO_I2S_CHNUM_2,
        .enBitDepth       = AIAO_BIT_DEPTH_16,
        .enRate           = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV      =                    128,
        .u32BCLK_DIV      =                      2,
    },
    .stBufConfig          =
    {
        .u32PeriodBufSize = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber  = PERIOND_NUM,
    },
    .enTrackMode          = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate         = AIAO_DF_FadeInRate,
    .enFadeOutRate        = AIAO_DF_FadeOutRate,
    .bMute                = HI_FALSE,
    .bMuteFade            = HI_TRUE,
    .u32VolumedB          = 0x79,
    .bByBass              = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
    .enOpType             = SND_OP_TYPE_I2S,
};
static AIAO_I2S_SOURCE_E g_enTxI2SSourceTab[8] =
{
    AIAO_TX0,
    AIAO_TX1,
    AIAO_TX2,
    AIAO_TX3,
    AIAO_TX4,
    AIAO_TX5,
    AIAO_TX6,
    AIAO_TX7,
};
static AIAO_CRG_SOURCE_E g_enTxCrgSourceTab[8] =
{
    AIAO_TX_CRG0,
    AIAO_TX_CRG1,
    AIAO_TX_CRG2,
    AIAO_TX_CRG3,
    AIAO_TX_CRG4,
    AIAO_TX_CRG5,
    AIAO_TX_CRG6,
    AIAO_TX_CRG7,
};

static AIAO_I2S_SOURCE_E g_enRxI2SSourceTab[8] =
{
    AIAO_RX0,
    AIAO_RX1,
    AIAO_RX2,
    AIAO_RX3,
    AIAO_RX4,
    AIAO_RX5,
    AIAO_RX6,
    AIAO_RX7,
};
static AIAO_CRG_SOURCE_E g_enRxCrgSourceTab[8] =
{
    AIAO_RX_CRG0,
    AIAO_RX_CRG1,
    AIAO_RX_CRG2,
    AIAO_RX_CRG3,
    AIAO_RX_CRG4,
    AIAO_RX_CRG5,
    AIAO_RX_CRG6,
    AIAO_RX_CRG7,
};


HI_VOID HAL_AIAO_P_SetTxI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_IsrFunc*      pIsrFunc)//i2s only card set proc func HI_ALSA_I2S_ONLY_SUPPORT
{
    g_stAiaoTxI2SDefaultOpenAttr.pIsrFunc = pIsrFunc;
}

HI_VOID HAL_AIAO_P_GetTxI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetHdmiHbrDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxHdmiHbrSDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetHdmiI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxHdmiI2SSDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetTxSpdDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxSpdDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
}


HI_VOID HAL_AIAO_P_GetRxAdcDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751v500) || defined(CHIP_TYPE_hi3751v620)
    pAttr->stIfAttr.enSource   = g_enRxI2SSourceTab[PORT2CHID(AIAO_PORT_RX2)];
#else
    pAttr->stIfAttr.enSource   = g_enRxI2SSourceTab[PORT2CHID(enPortID)];
#endif
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(AIAO_PORT_TX2)];
    pAttr->stIfAttr.enCrgMode = AIAO_CRG_MODE_DUPLICATE;
}

HI_VOID HAL_AIAO_P_GetRxSifDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));

#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751v500) || defined(CHIP_TYPE_hi3751v620)
    pAttr->stIfAttr.enSource   = g_enRxI2SSourceTab[PORT2CHID(AIAO_PORT_RX1)];
    pAttr->stIfAttr.eCrgSource = g_enRxCrgSourceTab[PORT2CHID(AIAO_PORT_RX1)];
#else
    pAttr->stIfAttr.enSource   = g_enRxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enRxCrgSourceTab[PORT2CHID(enPortID)];
#endif
    pAttr->stIfAttr.enCrgMode = AIAO_CRG_MODE_SLAVE;
}

HI_VOID HAL_AIAO_P_GetRxHdmiDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr)
{
    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enRxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enRxCrgSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.enCrgMode = AIAO_CRG_MODE_SLAVE;
}

#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
HI_VOID HAL_AIAO_P_GetBoardTxI2SDfAttr(HI_U32 u32BoardI2sNum, HI_UNF_I2S_ATTR_S*  pstI2sAttr, AIAO_PORT_ID_E* penPortID,
                                       AIAO_PORT_USER_CFG_S* pAttr)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;

    pstI2SBorad = &g_stI2SBoardSettings[u32BoardI2sNum];

    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));

    pAttr->stIfAttr.enCrgMode = (HI_TRUE == pstI2sAttr->bMaster) ? AIAO_CRG_MODE_MASTER : AIAO_CRG_MODE_SLAVE;
    pAttr->stIfAttr.enChNum = AUTIL_CHNUM_UNF2AIAO(pstI2sAttr->enChannel);
    pAttr->stIfAttr.enBitDepth  = AUTIL_BITDEPTH_UNF2AIAO(pstI2sAttr->enBitDepth);
    pAttr->stIfAttr.u32FCLK_DIV = AUTIL_BclkFclkDiv(pstI2sAttr->enMclkSel, pstI2sAttr->enBclkSel);
    pAttr->stIfAttr.u32BCLK_DIV = (HI_U32)pstI2sAttr->enBclkSel;
    pAttr->stIfAttr.eCrgSource = pstI2SBorad->enTxCrgSource;
    pAttr->stIfAttr.enSource = pstI2SBorad->enTxSource;
    pAttr->stIfAttr.u32PcmDelayCycles = 0;
    pAttr->stIfAttr.enI2SMode = AUTIL_I2S_MODE_UNF2AIAO(pstI2sAttr->enI2sMode);

    if (HI_UNF_I2S_PCM_MODE == pstI2sAttr->enI2sMode)
    {
        pAttr->stIfAttr.enRiseEdge = (HI_TRUE
                                      == pstI2sAttr->bPcmSampleRiseEdge) ? AIAO_MODE_EDGE_RISE : AIAO_MODE_EDGE_FALL;
        pAttr->stIfAttr.u32PcmDelayCycles = (HI_U32)pstI2sAttr->enPcmDelayCycle;
    }

    if (AIAO_BIT_DEPTH_24 == pAttr->stIfAttr.enBitDepth)
    {
        pAttr->stBufConfig.u32PeriodBufSize *= 2;
    }

    *penPortID = pstI2SBorad->enTxPortID;
}

HI_VOID HAL_AIAO_P_GetBoardRxI2SDfAttr(HI_U32 u32BoardI2sNum, AIAO_PORT_ID_E* penPortID,
                                       AIAO_PORT_USER_CFG_S* pAttr)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;

    pstI2SBorad = &g_stI2SBoardSettings[u32BoardI2sNum];

    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));

    pAttr->stIfAttr.enCrgMode = pstI2SBorad->enRxCrgMode;
    pAttr->stIfAttr.eCrgSource = pstI2SBorad->enRxCrgSource;
    pAttr->stIfAttr.enSource = pstI2SBorad->enRxSource;
    pAttr->stIfAttr.u32PcmDelayCycles = 0;

    *penPortID = pstI2SBorad->enRxPortID;
}

HI_S32 HAL_AIAO_P_CheckBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S*     pstNewIfAttr)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;
    AIAO_IfAttr_S*     pstOldIfAttr = NULL;
    HI_S32 n;

    for (n = 0; n < AIAO_MAX_EXT_I2S_NUMBER; n++)
    {
        pstI2SBorad = &g_stI2SBoardSettings[n];

        if ((enPortID == pstI2SBorad->enTxPortID) || (enPortID == pstI2SBorad->enRxPortID))
        {
            if (!pstI2SBorad->u32InitFlag)
            {
                return HI_SUCCESS;
            }

            if (((enPortID == pstI2SBorad->enRxPortID) && (!(pstI2SBorad->u32InitFlag & 0x2 )))
                || ((enPortID == pstI2SBorad->enTxPortID) && (!(pstI2SBorad->u32InitFlag & 0x1 ))))
            {
                return HI_SUCCESS;
            }

            pstOldIfAttr = &pstI2SBorad->stIfCommonAttr;
            break;
        }
    }

    if (AIAO_MAX_EXT_I2S_NUMBER == n)
    {
        return HI_SUCCESS;
    }

    if (pstOldIfAttr->enBitDepth != pstNewIfAttr->enBitDepth)
    {
        return HI_FAILURE;
    }

    if (pstOldIfAttr->enChNum != pstNewIfAttr->enChNum)
    {
        return HI_FAILURE;
    }

    if (pstOldIfAttr->enRate != pstNewIfAttr->enRate)
    {
        return HI_FAILURE;
    }

    if (pstOldIfAttr->enI2SMode != pstNewIfAttr->enI2SMode)
    {
        return HI_FAILURE;
    }

    if (pstOldIfAttr->u32PcmDelayCycles != pstNewIfAttr->u32PcmDelayCycles)
    {
        return HI_FAILURE;
    }

    if ((AIAO_CRG_MODE_SLAVE == pstOldIfAttr->enCrgMode) || (AIAO_CRG_MODE_SLAVE == pstNewIfAttr->enCrgMode))
    {
        if (pstOldIfAttr->enCrgMode != pstNewIfAttr->enCrgMode)
        {
            return HI_FAILURE;
        }
    }

    if ((pstOldIfAttr->u32FCLK_DIV != pstNewIfAttr->u32FCLK_DIV)
        || (pstOldIfAttr->u32BCLK_DIV != pstNewIfAttr->u32BCLK_DIV))
    {
        return HI_FAILURE;
    }

    if (pstI2SBorad->u32InitFlag)
    {
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

HI_VOID HAL_AIAO_P_SetBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S*     pstNewIfAttr)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;
    AIAO_IfAttr_S*     pstOldIfAttr;
    HI_S32 n;

    for (n = 0; n < AIAO_MAX_EXT_I2S_NUMBER; n++)
    {
        pstI2SBorad = &g_stI2SBoardSettings[n];

        if ((enPortID == pstI2SBorad->enTxPortID) || (enPortID == pstI2SBorad->enRxPortID))
        {
            pstOldIfAttr = &pstI2SBorad->stIfCommonAttr;
            break;
        }
    }

    if (AIAO_MAX_EXT_I2S_NUMBER == n)
    {
        return;
    }

    //With do check before, safe here
    memcpy(pstOldIfAttr, pstNewIfAttr, sizeof(AIAO_IfAttr_S));

    if (enPortID == pstI2SBorad->enTxPortID)
    {
        pstI2SBorad->u32InitFlag |= 0x2;  // set tx
    }
    else if (enPortID == pstI2SBorad->enRxPortID)
    {
        pstI2SBorad->u32InitFlag |= 0x1;  // set rx
    }

    return;
}

HI_VOID HAL_AIAO_P_DestroyBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID)
{
    AIAO_I2S_BOARD_CONFIG* pstI2SBorad;
    HI_S32 n;

    for (n = 0; n < AIAO_MAX_EXT_I2S_NUMBER; n++)
    {
        pstI2SBorad = &g_stI2SBoardSettings[n];

        if ((enPortID == pstI2SBorad->enTxPortID) || (enPortID == pstI2SBorad->enRxPortID))
        {
            break;
        }
    }

    if (AIAO_MAX_EXT_I2S_NUMBER == n)
    {
        return;
    }

    if (enPortID == pstI2SBorad->enTxPortID)
    {
        pstI2SBorad->u32InitFlag &= 0x1;  // clr tx
    }
    else if (enPortID == pstI2SBorad->enRxPortID)
    {
        pstI2SBorad->u32InitFlag &= 0x2;  // clr rx
    }

    return;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
