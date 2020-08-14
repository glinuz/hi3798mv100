/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_aiao_func.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/

#include <asm/setup.h>
#include <asm/io.h>

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_struct.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_mem.h"
#include "hi_drv_module.h"
#include "hi_drv_mmz.h"
#include "audio_util.h"

#include "hal_aiao_func.h"
#include "hal_aiao_priv.h"
#include "hal_aoe.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID AIAOMmzName(AIAO_PORT_ID_E enPortID, HI_CHAR *pszMmzName, HI_U32 u32NameSize)
{
    HI_U32 ChnId = PORT2CHID(enPortID);

    switch (PORT2MODE(enPortID))
    {
    case AIAO_MODE_RXI2S:
        snprintf(pszMmzName, u32NameSize, "%s%02d", "aiao_rx", ChnId);
        break;
    case AIAO_MODE_TXI2S:
        snprintf(pszMmzName, u32NameSize, "%s%02d", "aiao_tx", ChnId);
        break;
    case AIAO_MODE_TXSPDIF:
        snprintf(pszMmzName, u32NameSize, "%s%02d", "aiao_sp", ChnId);
        break;
    default:
        break;
    }
}

static HI_U32 AIAOCalcBufferSize(HI_U32 *pu32PeriodBufSize, HI_U32 *pu32PeriodNumber)
{
    HI_U32 PeriodBufSize = *pu32PeriodBufSize;
    HI_U32 PeriodNumber = *pu32PeriodNumber;

    if (PeriodNumber > AIAO_BUFFER_PERIODNUM_MAX)
    {
        PeriodNumber = AIAO_BUFFER_PERIODNUM_MAX;
    }

    if (PeriodNumber < AIAO_BUFFER_PERIODNUM_MIN)
    {
        PeriodNumber = AIAO_BUFFER_PERIODNUM_MIN;
    }

    if (PeriodBufSize % AIAO_BUFFER_SIZE_ALIGN)
    {
        PeriodBufSize -= (PeriodBufSize % AIAO_BUFFER_SIZE_ALIGN);
    }

    if (PeriodBufSize < AIAO_BUFFER_PERIODSIZE_MIN)
    {
        PeriodBufSize = AIAO_BUFFER_PERIODSIZE_MIN;
    }

    if (PeriodBufSize > AIAO_BUFFER_PERIODSIZE_MAX)
    {
        PeriodBufSize = AIAO_BUFFER_PERIODSIZE_MAX;
    }

    *pu32PeriodBufSize = PeriodBufSize;
    *pu32PeriodNumber = PeriodNumber;

    return PeriodNumber * PeriodBufSize;
}

static HI_VOID PortBufFlush(AIAO_PORT_S* pstPort)
{
#if 0
    HI_VOID                 AIAO_HW_GetBufu32Wptr(AIAO_PORT_ID_E enPortID, HI_U32 *pu32Wptr);
    HI_VOID                 AIAO_HW_GetBufu32Rptr(AIAO_PORT_ID_E enPortID, HI_U32 *pu32Rptr);
#else
    pstPort->stBuf.u32BUFF_RPTR = 0;
    pstPort->stBuf.u32BUFF_WPTR = 0;
#endif
    AIAO_HW_SetBuf(pstPort->enPortID, &pstPort->stBuf);

    //memset(pstPort->stCB.pu8Data, 0, pstPort->stCB.u32Lenght);
    return;
}

static HI_S32 PortBufInit(AIAO_PORT_S* pstPort)
{
    AIAO_PORT_USER_CFG_S *pstConfig = &pstPort->stUserCongfig;
    AIAO_BufAttr_S *pstBufConfig = &pstConfig->stBufConfig;
    HI_U32 *pu32WptrAddr = 0, *pu32RptrAddr = 0;
    HI_U32 uBufSize = AIAOCalcBufferSize(&pstBufConfig->u32PeriodBufSize, &pstBufConfig->u32PeriodNumber);
    HI_U32 u32StartPhyAddr;
    HI_U8 *pu8StartVirAddr;

    if(HI_TRUE == pstConfig->bExtDmaMem)
    {
        if(!pstConfig->stExtMem.tBufPhyAddr || !pstConfig->stExtMem.tBufVirAddr)
        {
            HI_FATAL_AIAO("PhyAddr(0x%x) VirAddr(0x%x) invalid \n", pstConfig->stExtMem.tBufPhyAddr,pstConfig->stExtMem.tBufVirAddr);
            return HI_FAILURE;
        }
        if(pstConfig->stExtMem.tBufPhyAddr%AIAO_BUFFER_ADDR_ALIGN)
        {
            HI_FATAL_AIAO("PhyAddr(0x%x) should align to (0x%x) invalid \n", pstConfig->stExtMem.tBufPhyAddr,AIAO_BUFFER_ADDR_ALIGN);
            return HI_FAILURE;
        }
        if(uBufSize > pstConfig->stExtMem.u32BufSize)
        {
            HI_FATAL_AIAO("ExtMem(0x%x) less than(0x%x) \n", pstConfig->stExtMem.u32BufSize,uBufSize);
            return HI_FAILURE;
        }
        u32StartPhyAddr = pstConfig->stExtMem.tBufPhyAddr;
        pu8StartVirAddr = (HI_U8*)pstConfig->stExtMem.tBufVirAddr;
    }
    else
    {
        // step 1.0, MMZ
        AIAOMmzName(pstPort->enPortID, pstPort->szProcMmzName, sizeof(pstPort->szProcMmzName));
        if (HI_SUCCESS
            != HI_DRV_MMZ_AllocAndMap(pstPort->szProcMmzName, MMZ_OTHERS, uBufSize, AIAO_BUFFER_ADDR_ALIGN, &pstPort->stMmz))
        {
            HI_FATAL_AIAO("Unable to mmz %s \n", pstPort->szProcMmzName);
            return HI_FAILURE;
        }
        u32StartPhyAddr = pstPort->stMmz.u32StartPhyAddr;
        pu8StartVirAddr = pstPort->stMmz.pu8StartVirAddr;
    }

    AIAO_HW_GetRptrAndWptrRegAddr(pstPort->enPortID, &pu32WptrAddr, &pu32RptrAddr);

    // step 2.0, CIRC BUf
    CIRC_BUF_Init(&pstPort->stCB,
                  pu32WptrAddr,
                  pu32RptrAddr,
                  pu8StartVirAddr,
                  uBufSize);

    // step 3.0, AIAO CIRC BUf Reg
    pstPort->stBuf.u32BUFF_SADDR = u32StartPhyAddr;
    pstPort->stBuf.u32BUFF_WPTR = 0;
    pstPort->stBuf.u32BUFF_RPTR = 0;
    pstPort->stBuf.u32BUFF_SIZE = uBufSize;

    if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
    {
        pstPort->stBuf.u32PeriodBufSize = pstBufConfig->u32PeriodBufSize;
        pstPort->stBuf.u32ThresholdSize = pstBufConfig->u32PeriodBufSize;
    }
    else
    {
        pstPort->stBuf.u32PeriodBufSize = pstBufConfig->u32PeriodBufSize;
        pstPort->stBuf.u32ThresholdSize = pstBufConfig->u32PeriodBufSize;
    }

    AIAO_HW_SetBuf(pstPort->enPortID, &pstPort->stBuf);
    return HI_SUCCESS;
}

static HI_VOID PortBufDeInit(AIAO_PORT_S* pstPort)
{
    CIRC_BUF_DeInit(&pstPort->stCB);

    if(HI_TRUE != pstPort->stUserCongfig.bExtDmaMem)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pstPort->stMmz);
    }
}

static HI_VOID PortSetI2SAttr(AIAO_PORT_S* pstPort)
{
    AIAO_PORT_USER_CFG_S *pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetIfAttr(pstPort->enPortID, &pstConfig->stIfAttr);
}

static HI_VOID PortSetUserDefaultCongfig(AIAO_PORT_S* pstPort)
{
    AIAO_PORT_ID_E enPortID = pstPort->enPortID;
    AIAO_PORT_USER_CFG_S *pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetTrackMode(enPortID, pstConfig->enTrackMode);
    AIAO_HW_SetFadeInRate(enPortID, pstConfig->enFadeInRate);
    AIAO_HW_SetFadeOutRate(enPortID, pstConfig->enFadeOutRate);
    AIAO_HW_SetMuteFade(enPortID, pstConfig->bMuteFade);
    AIAO_HW_SetMute(enPortID, pstConfig->bMute);
    AIAO_HW_SetVolumedB(enPortID, pstConfig->u32VolumedB);
    AIAO_HW_SetBypass(enPortID, pstConfig->bByBass);
    return;
}

static HI_VOID PortResetProcStatus(AIAO_PORT_S* pstPort)
{
    memset(&pstPort->stStatus, 0, sizeof(AIAO_PROC_STAUTS_S));
}

static HI_S32 PortStart(AIAO_PORT_S* pstPort)
{
    HI_S32 Ret;

    if (AIAO_PORT_STATUS_STOP == pstPort->enStatus)
    {
        Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_START);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AIAO("AIAO_START error\n");
            return Ret;
        }

        pstPort->enStatus = AIAO_PORT_STATUS_START;
    }
    else if (AIAO_PORT_STATUS_STOP_PENDDING == pstPort->enStatus)
    {
        Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_STOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AIAO("AIAO_STOP error\n");
            return Ret;
        }

        PortBufFlush(pstPort);

        //restore user mute setting
        AIAO_HW_SetMute(pstPort->enPortID, pstPort->stUserCongfig.bMute);
        Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_START);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AIAO("AIAO_START error\n");
            return Ret;
        }

        pstPort->enStatus = AIAO_PORT_STATUS_START;
    }

    return HI_SUCCESS;
}

static HI_S32 PortStop(AIAO_PORT_S* pstPort, AIAO_PORT_STOPMODE_E enStopMode)
{
    HI_S32 Ret;

    if (AIAO_STOP_FADEOUT == enStopMode)
    {
        enStopMode = AIAO_STOP_IMMEDIATE;
    }

    switch (pstPort->enStatus)
    {
    case AIAO_PORT_STATUS_START:
        {
            /* stop immediately */
            Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_STOP);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("AIAO_STOP error\n");
                return Ret;
            }

            PortBufFlush(pstPort);
            pstPort->enStatus = AIAO_PORT_STATUS_STOP;
        }

        break;
    case AIAO_PORT_STATUS_STOP_PENDDING:
        if (AIAO_STOP_FADEOUT != enStopMode)
        {
            /* stop immediately */
            Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_STOP);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("AIAO_STOP error\n");
                return Ret;
            }

            PortBufFlush(pstPort);

            //restore user mute setting
            AIAO_HW_SetMute(pstPort->enPortID, pstPort->stUserCongfig.bMute);
            pstPort->enStatus = AIAO_PORT_STATUS_STOP;
        }

        break;
    case AIAO_PORT_STATUS_STOP:
        break;
    }

    if (AIAO_PORT_STATUS_STOP == pstPort->enStatus)
    {
        PortResetProcStatus(pstPort);
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SelectSpdifSource(AIAO_PORT_S* pstPort, AIAO_SPDIFPORT_SOURCE_E eSrcChnId)
{
    AIAO_ASSERT_NULL(pstPort);

    AIAO_HW_SetSPDIFPortSelect(pstPort->enPortID, eSrcChnId);
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetSpdifOutPort(AIAO_PORT_S* pstPort, HI_S32 bEn)
{
    AIAO_ASSERT_NULL(pstPort);

    AIAO_HW_SetSPDIFPortEn(pstPort->enPortID, bEn);
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetI2SSdSelect(AIAO_PORT_S* pstPort, AIAO_I2SDataSel_S  *pstSdSel)
{
    AIAO_ASSERT_NULL(pstPort);

    AIAO_HW_SetI2SDataSelect(pstPort->enPortID, AIAO_I2S_SD0, pstSdSel->enSD0);
    AIAO_HW_SetI2SDataSelect(pstPort->enPortID, AIAO_I2S_SD1, pstSdSel->enSD1);
    AIAO_HW_SetI2SDataSelect(pstPort->enPortID, AIAO_I2S_SD2, pstSdSel->enSD2);
    AIAO_HW_SetI2SDataSelect(pstPort->enPortID, AIAO_I2S_SD3, pstSdSel->enSD3);
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_Start(AIAO_PORT_S* pstPort)
{
    AIAO_ASSERT_NULL(pstPort);
    return PortStart(pstPort);
}

HI_S32 iHAL_AIAO_P_Stop(AIAO_PORT_S* pstPort, AIAO_PORT_STOPMODE_E enStopMode)
{
    AIAO_ASSERT_NULL(pstPort);
    return PortStop(pstPort, enStopMode);
}

HI_S32 iHAL_AIAO_P_Mute(AIAO_PORT_S* pstPort, HI_BOOL bMute)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetMute(pstPort->enPortID, bMute);
    pstConfig->bMute = bMute;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetVolume(AIAO_PORT_S* pstPort, HI_U32 u32VolumedB)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetVolumedB(pstPort->enPortID, u32VolumedB);
    pstConfig->u32VolumedB = u32VolumedB;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetSpdifCategoryCode(AIAO_PORT_S* pstPort, AIAO_SPDIF_CATEGORYCODE_E eCategoryCode)
{
    AIAO_PORT_USER_CFG_S *pstConfig;
    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;
    AIAO_HW_SetSPDIF_Category_SCMS(pstPort->enPortID, pstConfig->enSPDIFSCMSMode, eCategoryCode);
    pstConfig->enSPDIFCategoryCode = eCategoryCode;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetSpdifSCMSMode(AIAO_PORT_S* pstPort, AIAO_SPDIF_SCMS_MODE_E eSCMSMode)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetSPDIF_Category_SCMS(pstPort->enPortID, eSCMSMode, pstConfig->enSPDIFCategoryCode);
    pstConfig->enSPDIFSCMSMode = eSCMSMode;

    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetTrackMode(AIAO_PORT_S* pstPort, AIAO_TRACK_MODE_E enTrackMode)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetTrackMode(pstPort->enPortID, enTrackMode);
    pstConfig->enTrackMode = enTrackMode;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetBypass(AIAO_PORT_S* pstPort, HI_BOOL bByBass)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetBypass(pstPort->enPortID, bByBass);
    pstConfig->bByBass = bByBass;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_SetMode(AIAO_PORT_S* pstPort, HI_BOOL bMode)
{
    AIAO_PORT_USER_CFG_S *pstConfig;

    AIAO_ASSERT_NULL(pstPort);
    pstConfig = &pstPort->stUserCongfig;

    AIAO_HW_SetMode(pstPort->enPortID, bMode);

    return HI_SUCCESS;
}

/*
typedef struct
{
    AIAO_IfAttr_S     stIfAttr;
    AIAO_BufAttr_S    stBufConfig;
} AIAO_PORT_ATTR_S;
*/

HI_S32 iHAL_AIAO_P_SetI2SlaveClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S *pstIfAttr)
{
    AIAO_HW_SetI2SSlaveClk(enPortID, pstIfAttr);
    return HI_SUCCESS;
}

#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
HI_S32 iHAL_AIAO_P_SetOpType(AIAO_PORT_ID_E enPortID, AIAO_OP_TYPE_E enOpType)
{
    AIAO_HW_SetOpType(enPortID, enOpType);

    return HI_SUCCESS;
}
#endif

HI_S32 iHAL_AIAO_P_SetI2SMasterClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S *pstIfAttr)
{
    AIAO_HW_SetI2SMasterClk(enPortID, pstIfAttr);
    return HI_SUCCESS;
}

HI_VOID iHAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_ID_E enPortID, HI_S32 bEn)
{
    AIAO_HW_SetSPDIFPortEn(enPortID, bEn);
}

HI_S32 iHAL_AIAO_P_SetAttr(AIAO_PORT_S* pstPort, AIAO_PORT_ATTR_S *pstAttr)
{
    AIAO_ASSERT_NULL(pstPort);
    AIAO_ASSERT_NULL(pstAttr);

    //todo check attr
    if(AIAO_PORT_STATUS_STOP != pstPort->enStatus)
    {
        HI_FATAL_AIAO("pstPort[0x%x] enStatus is not AIAO_PORT_STATUS_STOP\n", pstPort->enPortID);
        return HI_FAILURE;
    }

    // free buffer
    PortBufDeInit(pstPort);

    memcpy(&pstPort->stUserCongfig.stIfAttr, &pstAttr->stIfAttr, sizeof(AIAO_IfAttr_S));
    memcpy(&pstPort->stUserCongfig.stBufConfig, &pstAttr->stBufConfig, sizeof(AIAO_BufAttr_S));

    // AIAO Buf Init
    if (HI_FAILURE == PortBufInit(pstPort))
    {
        HI_FATAL_AIAO("PortBufInit failed\n");
        return HI_FAILURE;
    }

    // AIAO CRG/I2S
    PortSetI2SAttr(pstPort);

    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_GetAttr(AIAO_PORT_S* pstPort, AIAO_PORT_ATTR_S *pstAttr)
{
    AIAO_ASSERT_NULL(pstPort);
    AIAO_ASSERT_NULL(pstAttr);

    memcpy(&pstAttr->stIfAttr,&pstPort->stUserCongfig.stIfAttr, sizeof(AIAO_IfAttr_S));
    memcpy(&pstAttr->stBufConfig,&pstPort->stUserCongfig.stBufConfig, sizeof(AIAO_BufAttr_S));
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_Open(const AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S *pstConfig, AIAO_PORT_S**ppstPort,AIAO_IsrFunc** pIsr)
{
    AIAO_PORT_S* pstPort = HI_NULL;

    AIAO_ASSERT_NULL(pstConfig);
    AIAO_ASSERT_NULL(ppstPort);

    // step 1 malloc AIAO_PORT_S
    pstPort = AUTIL_AIAO_MALLOC(HI_ID_AIAO, sizeof(AIAO_PORT_S), GFP_KERNEL);
    if (pstPort == HI_NULL)
    {
        HI_FATAL_AIAO("malloc AIAO_PORT_S failed\n");
        return HI_FAILURE;
    }

    memset(pstPort, 0, sizeof(AIAO_PORT_S));
    memcpy(&pstPort->stUserCongfig, pstConfig, sizeof(AIAO_PORT_USER_CFG_S));
    pstPort->enPortID = enPortID;
    pstPort->enStatus = AIAO_PORT_STATUS_STOP;

    // step 2 AIAO Buf Init
    if (HI_FAILURE == PortBufInit(pstPort))
    {
        AUTIL_AIAO_FREE(HI_ID_AIAO, pstPort);
        HI_FATAL_AIAO("PortBufInit failed\n");
        return HI_FAILURE;
    }
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
#if !defined(HI_AIAO_VERIFICATION_SUPPORT)
    if(enPortID != AIAO_PORT_SPDIF_TX0)
#endif
#endif
    {
        // step 3 AIAO CRG/I2S
        PortSetI2SAttr(pstPort);

        // step 3.1 AIAO interrupt setting

        if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
        {
            AIAO_HW_SetInt(pstPort->enPortID, (HI_U32)(1L << AIAO_TXINT0_BUF_TRANSFINISH));
        }
        else
        {
            AIAO_HW_SetInt(pstPort->enPortID, (HI_U32)(1L << AIAO_RXINT0_BUF_TRANSFINISH));
        }
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
        AIAO_HW_SetOpType(pstPort->enPortID, pstConfig->enOpType);
#endif
        // step 4 AIAO user default config
        PortSetUserDefaultCongfig(pstPort);
    }

    // step 5 AIAO init internal state
    PortResetProcStatus(pstPort);

    *ppstPort = (AIAO_PORT_S*)pstPort;
    *pIsr = pstConfig->pIsrFunc;
    return HI_SUCCESS;
}

#ifdef HI_AIAO_TIMER_SUPPORT
HI_S32 iHAL_AIAO_T_Create(AIAO_TIMER_ID_E enTimerID, const AIAO_Timer_Create_S *pstParam, AIAO_TIMER_S *ppstPort,AIAO_TimerIsrFunc** pIsr)
{
    AIAO_TIMER_S pstPort = HI_NULL;
    AIAO_ASSERT_NULL(pstParam);

    pstPort = AUTIL_AIAO_MALLOC(HI_ID_AIAO, sizeof(AIAO_TIMER_CTX_S), GFP_KERNEL);
    if (pstPort == HI_NULL)
    {
        HI_FATAL_AIAO("malloc AIAO_TIMER_CTX_S failed\n");
        return HI_FAILURE;
    }

    memset(pstPort, 0, sizeof(AIAO_TIMER_CTX_S));
    memcpy(&pstPort->stTimerParam, pstParam, sizeof(AIAO_Timer_Create_S));
    pstPort->enTimerID = enTimerID;
    *ppstPort = pstPort;
    *pIsr = pstParam->pTimerIsrFunc;

    return HI_SUCCESS;
}

HI_VOID iHAL_AIAO_T_Destroy(AIAO_TIMER_S pstPort)
{
    AIAO_ASSERT_NULL(pstPort);

    AUTIL_AIAO_FREE(HI_ID_AIAO, pstPort);

    return;
}

HI_S32 iHAL_AIAO_T_SetTimerAttr(AIAO_TIMER_S pstPort,const AIAO_TIMER_Attr_S *pstAttrParam)
{
    AIAO_ASSERT_NULL(pstPort);

    AIAO_TIMER_SetAttr(pstPort->enTimerID,&(pstAttrParam->stIfAttr), pstAttrParam->u32Config);
    pstPort->stTimerAttr.u32Config = pstAttrParam->u32Config;
    memcpy(&(pstPort->stTimerAttr.stIfAttr),&(pstAttrParam->stIfAttr),sizeof(AIAO_IfTimerAttr_S));
    pstPort->stTimerStatus.enRate = pstAttrParam->stIfAttr.enRate;
    pstPort->stTimerStatus.u32Config = pstAttrParam->u32Config;

    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_T_SetTimerEnalbe(AIAO_TIMER_S pstPort,HI_BOOL bEnalbe)
{
    AIAO_ASSERT_NULL(pstPort);

    AIAO_TIMER_SetEnable(pstPort->enTimerID, bEnalbe);
    pstPort->stTimerStatus.bEnable = bEnalbe;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_T_GetStatus(AIAO_TIMER_S pstPort, AIAO_TIMER_Status_S *pstStatus)
{
    AIAO_ASSERT_NULL(pstPort);

    memcpy(pstStatus, &pstPort->stTimerStatus, sizeof(AIAO_TIMER_Status_S));
    return HI_SUCCESS;
}

HI_VOID iHAL_AIAO_T_TIMERProcess(AIAO_TIMER_S pstPort)
{
    AIAO_TIMER_Status_S *pstTimerStatus;

    pstTimerStatus = &pstPort->stTimerStatus;
    pstTimerStatus->u32IsrCnt++;
}

HI_VOID iHAL_TIMER_ClearTimer(AIAO_TIMER_ID_E enTimerID)
{
    AIAO_TIMER_ClearTimer(enTimerID);
}
#endif

HI_VOID iHAL_AIAO_P_Close(AIAO_PORT_S* pstPort)
{
    HI_S32 Ret;

    AIAO_ASSERT_NULL(pstPort);

    // step 1 stop channel
    Ret = PortStop(pstPort, AIAO_STOP_IMMEDIATE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AIAO("PortStop error\n");
        return;
    }

    // step 1.1 AIAO interrupt setting
    if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
    {
        AIAO_HW_ClrInt(pstPort->enPortID, (HI_U32)AIAO_TXINT_ALL);
    }
    else
    {
        AIAO_HW_ClrInt(pstPort->enPortID, (HI_U32)AIAO_RXINT_ALL);
    }

    // step 2 free buffer
    PortBufDeInit(pstPort);

    // step 3 free dev sourece
    AUTIL_AIAO_FREE(HI_ID_AIAO, pstPort);

    return;
}

HI_S32 iHAL_AIAO_P_GetUserCongfig(AIAO_PORT_S* pstPort, AIAO_PORT_USER_CFG_S *pstUserConfig)
{
    AIAO_ASSERT_NULL(pstPort);
    AIAO_ASSERT_NULL(pstUserConfig);
    memcpy(pstUserConfig, &pstPort->stUserCongfig, sizeof(AIAO_PORT_USER_CFG_S));
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_GetStatus(AIAO_PORT_S* pstPort, AIAO_PORT_STAUTS_S *pstStatus)
{
    AIAO_ASSERT_NULL(pstPort);
    memcpy(&pstStatus->stProcStatus, &pstPort->stStatus, sizeof(AIAO_PROC_STAUTS_S));
    memcpy(&pstStatus->stUserConfig, &pstPort->stUserCongfig, sizeof(AIAO_PORT_USER_CFG_S));
    memcpy(&pstStatus->stBuf, &pstPort->stBuf, sizeof(AIAO_BufInfo_S));
    memcpy(&pstStatus->stCircBuf, &pstPort->stCB, sizeof(CIRC_BUF_S));
    pstStatus->enStatus = pstPort->enStatus;
    return HI_SUCCESS;
}

HI_S32 iHAL_AIAO_P_GetRbfAttr(AIAO_PORT_S* pstPort, AIAO_RBUF_ATTR_S *pstRbfAttr)
{
    HI_U32 *pu32BufVirWptr, *pu32BufVirRptr;

    AIAO_ASSERT_NULL(pstPort);
    if(HI_TRUE==pstPort->stUserCongfig.bExtDmaMem)
    {
        pstRbfAttr->tBufPhyAddr = pstPort->stUserCongfig.stExtMem.tBufPhyAddr;
        pstRbfAttr->tBufVirAddr = pstPort->stUserCongfig.stExtMem.tBufVirAddr;
        pstRbfAttr->u32BufSize    = pstPort->stBuf.u32BUFF_SIZE;
    }
    else
    {
        pstRbfAttr->tBufPhyAddr = pstPort->stMmz.u32StartPhyAddr;
        pstRbfAttr->tBufVirAddr = (HI_VIRT_ADDR_T)pstPort->stMmz.pu8StartVirAddr;
        pstRbfAttr->u32BufSize    = pstPort->stBuf.u32BUFF_SIZE;
    }

    AIAO_HW_GetRptrAndWptrRegAddr(pstPort->enPortID, &pu32BufVirWptr, &pu32BufVirRptr);
    AIAO_HW_GetRptrAndWptrRegPhyAddr(pstPort->enPortID, &pstRbfAttr->tBufPhyWptr, &pstRbfAttr->tBufPhyRptr);
    pstRbfAttr->tBufVirWptr = (HI_VIRT_ADDR_T)pu32BufVirWptr;
    pstRbfAttr->tBufVirRptr = (HI_VIRT_ADDR_T)pu32BufVirRptr;

    return HI_SUCCESS;
}

static HI_VOID AIAO_HAL_P_TxIsrProc(AIAO_PROC_STAUTS_S *pstProc, HI_U32 u32IntStatus)  //proc
{
    if(u32IntStatus & AIAO_TXINT0_BUF_TRANSFINISH_MASK)
    {
        pstProc->uDMACnt++;
    }

#if 1
    if (u32IntStatus & AIAO_TXINT1_BUF_EMPTY_MASK)
    {
        /* Ignore the buf empty INT at start beginning */
        if (pstProc->uBusFiFoEmptyCnt)
        {
            pstProc->uBufEmptyCnt++;
        }
    }

    if (u32IntStatus & AIAO_TXINT2_BUF_AEMPTY_MASK)
    {
        /* Ignore the buf allmost empty INT at start beginning */
        if (pstProc->uBusFiFoEmptyCnt)
        {
            pstProc->uBufEmptyWarningCnt++;
        }
    }
    else
    {
        if (u32IntStatus & AIAO_TXINT3_BUF_FIFOEMPTY_MASK)
        {
            pstProc->uBusTimeOutCnt++;
        }
    }

    if (u32IntStatus & AIAO_TXINT3_BUF_FIFOEMPTY_MASK)
    {
        pstProc->uBusFiFoEmptyCnt++;
    }

    if (u32IntStatus & AIAO_TXINT4_IF_FIFOEMPTY_MASK)
    {
        pstProc->uInfFiFoEmptyCnt++;
    }

    if (u32IntStatus & AIAO_TXINT7_DATA_BROKEN_MASK)
    {
        pstProc->uInfEmptyCntReal++;
    }
#endif
}

static HI_VOID AIAO_HAL_P_RxIsrProc(AIAO_PROC_STAUTS_S *pstProc, HI_U32 u32IntStatus)
{
    if(u32IntStatus & AIAO_RXINT0_BUF_TRANSFINISH_MASK)
    {
        pstProc->uDMACnt++;
    }

#if 1
    if (u32IntStatus & AIAO_RXINT1_BUF_FULL_MASK)
    {
        pstProc->uBufFullCnt++;
    }

    if (u32IntStatus & AIAO_RXINT2_BUF_AFULL_MASK)
    {
        pstProc->uBufFullWarningCnt++;
    }
    else
    {
        if (u32IntStatus & AIAO_RXINT3_BUF_FIFOFULL_MASK)
        {
            pstProc->uBusTimeOutCnt++;
        }
    }

    if (u32IntStatus & AIAO_RXINT3_BUF_FIFOFULL_MASK)
    {
        pstProc->uBusFiFoFullCnt++;
    }

    if (u32IntStatus & AIAO_RXINT4_IF_FIFOFULL_MASK)
    {
        pstProc->uInfFiFoFullCnt++;
    }
#endif
}

HI_VOID iHAL_AIAO_P_ProcStatistics(AIAO_PORT_S* pstPort, HI_U32 u32IntStatus)
{
    AIAO_PROC_STAUTS_S *pstProc;

    AIAO_ASSERT_NULL(pstPort);
    pstProc = &pstPort->stStatus;
    if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
    {
        AIAO_HAL_P_TxIsrProc(pstProc, u32IntStatus);
    }
    else
    {
        AIAO_HAL_P_RxIsrProc(pstProc, u32IntStatus);
    }
}

HI_U32 iHAL_AIAO_P_ReadData_NotUpRptr(AIAO_PORT_S* pstPort, HI_U8 * pu32Dest, HI_U32 u32DestSize, HI_U32 *pu32Rptr, HI_U32 *pu32Wptr)
{
    CIRC_BUF_S *pstCB;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    HI_ASSERT_RET(HI_NULL != pu32Dest);

    pstCB = &pstPort->stCB;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        if (AIAO_DIR_RX == PORT2DIR(pstPort->enPortID))
        {
            Bytes = CIRC_BUF_Read_NotUpRptr(pstCB, pu32Dest, u32DestSize, pu32Rptr, pu32Wptr);
        }
        else
        {
            HI_WARN_AIAO("AIAO Tx Buf can't been read.\n");
        }
    }

    return Bytes;
}

HI_U32 iHAL_AIAO_P_ReadData(AIAO_PORT_S* pstPort, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    HI_ASSERT_RET(HI_NULL != pu32Dest);

    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        if (AIAO_DIR_RX == PORT2DIR(pstPort->enPortID))
        {
            pstProc->uTryReadCnt++;
            Bytes = CIRC_BUF_Read(pstCB, pu32Dest, u32DestSize);
            pstProc->uTotalByteRead += Bytes;
        }
        else
        {
            HI_WARN_AIAO("AIAO Tx Buf can't been read.\n");
        }
    }

    return Bytes;
}

HI_U32 iHAL_AIAO_P_WriteData(AIAO_PORT_S* pstPort, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;

    pstProc->uTryWriteCnt++;
    Bytes = CIRC_BUF_Write(pstCB, pu32Src, u3SrcLen);
    pstProc->uTotalByteWrite += Bytes;

    return Bytes;
}

HI_U32 iHAL_AIAO_P_PrepareData(AIAO_PORT_S* pstPort, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;
    HI_S32 Ret;

    AIAO_ASSERT_NULL(pstPort);
    HI_ASSERT(HI_NULL != pu32Src);

    pstCB = &pstPort->stCB;
    pstProc = &pstPort->stStatus;

    if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
    {
        if (AIAO_PORT_STATUS_STOP_PENDDING == pstPort->enStatus)
        {
            Ret = AIAO_HW_SetStart(pstPort->enPortID, AIAO_STOP);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AIAO("AIAO_STOP error\n");
                return 0;
            }

            PortBufFlush(pstPort);

            //restore user mute setting
            AIAO_HW_SetMute(pstPort->enPortID, pstPort->stUserCongfig.bMute);
            pstPort->enStatus = AIAO_PORT_STATUS_STOP;
        }

        pstProc->uTryWriteCnt++;
        Bytes = CIRC_BUF_Write(pstCB, pu32Src, u3SrcLen);
        pstProc->uTotalByteWrite += Bytes;
    }
    else
    {
        HI_WARN_AIAO("AIAO Rx Buf can't been write.\n");
    }

    return Bytes;
}

HI_U32 iHAL_AIAO_P_QueryBufData_ProvideRptr(AIAO_PORT_S* pstPort, HI_U32 *pu32Rptr)
{
    AIAO_ASSERT_NULL(pstPort);
    return CIRC_BUF_QueryBusy_ProvideRptr(&pstPort->stCB, pu32Rptr);
}

HI_U32 iHAL_AIAO_P_QueryBufData(AIAO_PORT_S* pstPort)
{
    AIAO_ASSERT_NULL(pstPort);
    return CIRC_BUF_QueryBusy(&pstPort->stCB);
}

HI_U32 iHAL_AIAO_P_QueryBufFree(AIAO_PORT_S* pstPort)
{
    AIAO_ASSERT_NULL(pstPort);

    return CIRC_BUF_QueryFree(&pstPort->stCB);
}

HI_VOID iHAL_AIAO_P_GetDelayMs(AIAO_PORT_S* pstPort, HI_U32 * pu32Delayms)
{
    HI_U32 Bytes,u32FrameSize;
    AIAO_ASSERT_NULL(pstPort);

    Bytes = CIRC_BUF_QueryBusy(&pstPort->stCB);
    u32FrameSize = AUTIL_CalcFrameSize((HI_U32)pstPort->stUserCongfig.stIfAttr.enChNum,(HI_U32)pstPort->stUserCongfig.stIfAttr.enBitDepth);
    *pu32Delayms =  AUTIL_ByteSize2LatencyMs(Bytes, u32FrameSize, (HI_U32)pstPort->stUserCongfig.stIfAttr.enRate);
}

HI_U32 iHAL_AIAO_P_UpdateRptr(AIAO_PORT_S* pstPort, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    HI_ASSERT(HI_NULL != pu32Dest);

    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        if (AIAO_DIR_RX == PORT2DIR(pstPort->enPortID))
        {
            pstProc->uTryReadCnt++;
            Bytes = CIRC_BUF_UpdateRptr(pstCB, pu32Dest, u32DestSize);
            pstProc->uTotalByteRead += Bytes;
        }
        else
        {
            HI_WARN_AIAO("AIAO Tx Buf can't been read.\n");
        }
    }

    return Bytes;
}

HI_U32 iHAL_AIAO_P_UpdateWptr(AIAO_PORT_S* pstPort, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    HI_ASSERT(HI_NULL != pu32Src);
    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;

    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        if (AIAO_DIR_TX == PORT2DIR(pstPort->enPortID))
        {
            pstProc->uTryWriteCnt++;
            Bytes = CIRC_BUF_UpdateWptr(pstCB, pu32Src, u3SrcLen);
            pstProc->uTotalByteWrite += Bytes;
        }
        else
        {
            HI_WARN_AIAO("AIAO Rx Buf can't been write.\n");
        }
    }
    else
    {
        HI_WARN_AIAO("AIAO Tx should been start before write.\n");
    }

    return Bytes;
}

HI_S32 iHAL_AIAO_Init(HI_VOID)
{
    return AIAO_HW_Init();
}

HI_VOID iHAL_AIAO_DeInit(HI_VOID)
{
    AIAO_HW_DeInit();
}

HI_VOID iHAL_AIAO_GetHwCapability(HI_U32 *pu32Capability)
{
    AIAO_HW_GetHwCapability(pu32Capability);
}

HI_VOID iHAL_AIAO_GetHwVersion(HI_U32 *pu32Version)
{
    AIAO_HW_GetHwVersion(pu32Version);
}

HI_VOID iHAL_AIAO_DBG_RWReg(AIAO_Dbg_Reg_S *pstReg)
{
    AIAO_HW_DBG_RWReg(pstReg);
}

HI_VOID iHAL_AIAO_SetTopInt(HI_U32 u32Multibit)
{
    AIAO_HW_SetTopInt(u32Multibit);
}

HI_U32 iHAL_AIAO_GetTopIntRawStatus(HI_VOID)
{
    return AIAO_HW_GetTopIntRawStatus();
}

HI_U32 iHAL_AIAO_GetTopIntStatus(HI_VOID)
{
    return AIAO_HW_GetTopIntStatus();
}

HI_VOID iHAL_AIAO_P_SetInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit)
{
    AIAO_HW_SetInt(enPortID,u32Multibit);
}

HI_VOID iHAL_AIAO_P_ClrInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit)
{
    AIAO_HW_ClrInt(enPortID,u32Multibit);
}

HI_U32 iHAL_AIAO_P_GetIntStatusRaw(AIAO_PORT_ID_E enPortID)
{
    return AIAO_HW_GetIntStatusRaw(enPortID);
}

HI_U32 iHAL_AIAO_P_GetIntStatus(AIAO_PORT_ID_E enPortID)
{
    return iHAL_AIAO_P_GetIntStatus(enPortID);
}

#ifdef HI_ALSA_AI_SUPPORT
HI_U32 iHAL_AIAO_P_ALSA_QueryWritePos (AIAO_PORT_S* pstPort)
{
    CIRC_BUF_S *pstCB;
    HI_U32 Bytes = 0;

    AIAO_ASSERT_NULL(pstPort);
    pstCB   = &pstPort->stCB;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        Bytes = CIRC_BUF_ALSA_QueryWritePos(pstCB);
    }
    return Bytes;
}

HI_U32 iHAL_AIAO_P_ALSA_QueryReadPos (AIAO_PORT_S* pstPort)
{
    CIRC_BUF_S *pstCB;
    HI_U32 Bytes = 0;
    AIAO_ASSERT_NULL(pstPort);
    pstCB   = &pstPort->stCB;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        Bytes = CIRC_BUF_ALSA_QueryReadPos(pstCB);
    }
    return Bytes;
}

HI_U32 iHAL_AIAO_P_ALSA_UpdateRptr(AIAO_PORT_S* pstPort, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;
    AIAO_ASSERT_NULL(pstPort);
    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        pstProc->uTryReadCnt++;
        Bytes = CIRC_BUF_ALSA_UpdateRptr(pstCB, u32DestSize);
        pstProc->uTotalByteRead += Bytes;
    }
    return Bytes;
}

HI_U32 iHAL_AIAO_P_ALSA_UpdateWptr(AIAO_PORT_S* pstPort, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    CIRC_BUF_S *pstCB;
    AIAO_PROC_STAUTS_S *pstProc;
    HI_U32 Bytes = 0;
    AIAO_ASSERT_NULL(pstPort);
    pstCB   = &pstPort->stCB;
    pstProc = &pstPort->stStatus;
    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        pstProc->uTryReadCnt++;
        Bytes = CIRC_BUF_ALSA_UpdateWptr(pstCB,u32DestSize);
        pstProc->uTotalByteRead += Bytes;
    }
    return Bytes;
}

HI_U32 iHAL_AIAO_P_ALSA_FLASH(AIAO_PORT_S* pstPort)
{
    CIRC_BUF_S *pstCB;
    AIAO_ASSERT_NULL(pstPort);

    pstCB = &pstPort->stCB;

    if (AIAO_PORT_STATUS_START == pstPort->enStatus)
    {
        CIRC_BUF_ALSA_Flush(pstCB);
    }

    return HI_SUCCESS;
}
#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
