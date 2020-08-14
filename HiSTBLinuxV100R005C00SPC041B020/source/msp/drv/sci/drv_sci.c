#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "hi_drv_sys.h"
#include "drv_sci.h"
#include "hi_kernel_adapt.h"

#define SCI_ATR_ITEM(_grp, _cur) ((unsigned short)((((_cur) + 9) << 4) + (_grp)))

#define CHECK_PORTID()\
do{\
    if (enSciPort >= HI_SCI_PORT_NUM)\
    {\
        HI_ERR_SCI("Port is invalid.\n");\
        return HI_ERR_SCI_INVALID_PARA;\
    }\
}while(0)


extern volatile SCI_REG_S       *g_pSciReg[HI_UNF_SCI_PORT_BUTT];

irqreturn_t SCI_Isr(HI_S32 irq, HI_VOID *dev_id);
HI_VOID     SCI_ReadTasklet(HI_UNF_SCI_PORT_E enSciPort);

HI_U32 SCI_ClkRate[] = {372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048, 0, 0};
HI_U32 SCI_BitRate[] = {0, 1, 2, 4, 8, 16, 32, 0, 12, 20, 0, 0, 0, 0, 0, 0};
HI_U32 SusClkRate[2], SusBitRate[2];
HI_BOOL SusForceFlag[2], SusSetExtBaudFlag[2];
HI_UNF_SCI_PROTOCOL_E SusSciProtocol[2];
HI_UNF_SCI_STATUS_E SusSciState[2];

SCI_PARA_S g_SciPara[HI_UNF_SCI_PORT_BUTT];
ATR_BUF_S g_SciAtrBuf[HI_UNF_SCI_PORT_BUTT];
SCI_BUF_S g_SciDataBuf[HI_UNF_SCI_PORT_BUTT];
SCI_BUF_S g_SciSendDataBuf[HI_UNF_SCI_PORT_BUTT];
SCI_PPS_S g_SciPPS[HI_UNF_SCI_PORT_BUTT];

static DEFINE_SPINLOCK(sci_lock);

static SCI_PORT_ATTR_S g_astSciPortAttr[HI_UNF_SCI_PORT_BUTT] = {{SCI0_CRG_OFFSET, IRQ_SCI0, SCI0_PHY_ADDR},
                                                                 {SCI1_CRG_OFFSET, IRQ_SCI1, SCI1_PHY_ADDR}};

HI_S32 SCI_GetPortAttr(HI_UNF_SCI_PORT_E enSciPort, SCI_PORT_ATTR_S *pstPortAttr)
{
    if ((enSciPort >= HI_SCI_PORT_NUM) || (HI_NULL == pstPortAttr))
    {
        return HI_FAILURE;
    }

    memcpy(pstPortAttr, &g_astSciPortAttr[enSciPort], sizeof(SCI_PORT_ATTR_S));

    return HI_SUCCESS;
}

HI_VOID SCI_CleanBuf(HI_UNF_SCI_PORT_E enSciPort)
{
    g_SciDataBuf[enSciPort].Write = 0;
    g_SciDataBuf[enSciPort].Read = 0;
    g_SciDataBuf[enSciPort].DataLen = 0;

    SCI_HAL_ClearTxFifo(enSciPort);
    SCI_HAL_ClearRxFifo(enSciPort);
}

HI_VOID SCI_SetState(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E enState)
{
    g_SciPara[enSciPort].SciState.enSciCrtState = enState;
    return;
}

HI_VOID SCI_InitSetupPara(HI_UNF_SCI_PORT_E enSciPort)
{
    g_SciPara[enSciPort].SciState.enSciProtocol = HI_UNF_SCI_PROTOCOL_T0;
    g_SciPara[enSciPort].SciState.bSciDataReversal = HI_FALSE;
    g_SciPara[enSciPort].SciState.SciSetupPara.bRxParity  = HI_FALSE;
    g_SciPara[enSciPort].SciState.SciSetupPara.bTxParity  = HI_FALSE;
    g_SciPara[enSciPort].SciState.SciSetupPara.enRxParity = SCI_PARITY_EVEN;
    g_SciPara[enSciPort].SciState.SciSetupPara.enTxParity = SCI_PARITY_EVEN;

    g_SciPara[enSciPort].SciState.SciSetupPara.ClockIcc = g_SciPara[enSciPort].u32SysClk
                                                          / g_SciPara[enSciPort].SciAttr.Frequency
                                                          / 2 - 1;
    g_SciPara[enSciPort].SciState.SciSetupPara.StableTime = SCI_DFT_STABLE_TIME * g_SciPara[enSciPort].u32SysClk
                                                            / 65535;

    g_SciPara[enSciPort].SciState.SciSetupPara.RxTide = 1;
    g_SciPara[enSciPort].SciState.SciSetupPara.TxTide = 1;
    g_SciPara[enSciPort].SciState.SciSetupPara.ActiveTime = SCI_DFT_ACT_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.DeactTime = SCI_DFT_DEACT_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.AtrStartTime = SCI_DFT_ATRS_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.AtrDuration = SCI_DFT_ATRD_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.StopTime     = SCI_DFT_STOP_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.StartTime    = SCI_DFT_START_TIME;
    g_SciPara[enSciPort].SciState.SciSetupPara.RxRetries    = 3;
    g_SciPara[enSciPort].SciState.SciSetupPara.TxRetries    = 3;
    g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout  = SCI_DFT_CHAR_TIME - 12;
    g_SciPara[enSciPort].SciState.SciSetupPara.BlockTimeout = SCI_DFT_BLOCK_TIME - 12;
    g_SciPara[enSciPort].SciState.SciSetupPara.CharGuard    = 0;
    g_SciPara[enSciPort].SciState.SciSetupPara.BlockGuard   = SCI_DFT_BLKGUARD_TIME - 12;
    g_SciPara[enSciPort].SciState.SciSetupPara.RxTimeout    = SCI_DFT_RX_TIME;

    g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate = 372;
    g_SciPara[enSciPort].SciState.SciSetupPara.BitRate = 1;

    return;
}

HI_VOID SCI_DefaultConfig(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SETUP_PARA_S      *pSciSetupPara;

    SCI_InitSetupPara(enSciPort);

    pSciSetupPara = &g_SciPara[enSciPort].SciState.SciSetupPara;

    /***************************
    CR0 register default config
    ***************************/

    /*set default data sequence*/
    SCI_HAL_SetDataReversal(enSciPort, g_SciPara[enSciPort].SciState.bSciDataReversal);

    /*set send data parity check*/
    SCI_HAL_SetTxParityMode(enSciPort, pSciSetupPara->bTxParity, pSciSetupPara->enTxParity);

    /*set receive data parity check*/
    SCI_HAL_SetRxParityMode(enSciPort, pSciSetupPara->bRxParity, pSciSetupPara->enRxParity);

    /*enable clk,the clk pin is low when disable clk*/
    SCI_HAL_SetClock(enSciPort, HI_FALSE, 0);

    /***************************
      CR1 register default config
    ***************************/

    /*set receive ATR timeout count*/
    SCI_HAL_SetAtrTimeoutEnable(enSciPort, HI_TRUE);

    /*set receive block data timeout count*/
    SCI_HAL_SetBlockTimeoutEnable(enSciPort, HI_FALSE);

    /*set default receice direction*/
    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);

    /*set block guard time count*/
    SCI_HAL_SetBlockProtect(enSciPort, HI_TRUE);

    /*set card present check  counter*/
    SCI_HAL_SelectCounter(enSciPort, 0);

    /*set  asynchronism order*/
    SCI_HAL_SetSync(enSciPort, 0);

    SCI_HAL_SetClkIcc(enSciPort, pSciSetupPara->ClockIcc);

    SCI_HAL_SetTide(enSciPort, pSciSetupPara->RxTide, pSciSetupPara->TxTide);

    SCI_HAL_SetStableTime(enSciPort, pSciSetupPara->StableTime);

    SCI_HAL_SetActiveTime(enSciPort, pSciSetupPara->ActiveTime);

    SCI_HAL_SetDeactTime(enSciPort, pSciSetupPara->DeactTime);

    SCI_HAL_SetAtrStartTime(enSciPort, pSciSetupPara->AtrStartTime);

    SCI_HAL_SetAtrDurationTime(enSciPort, pSciSetupPara->AtrDuration);

    SCI_HAL_SetCharTimeout(enSciPort, pSciSetupPara->CharTimeout);

    SCI_HAL_SetStopTime(enSciPort, pSciSetupPara->StopTime);

    SCI_HAL_SetStartTime(enSciPort, pSciSetupPara->StartTime);

    SCI_HAL_SetRetry(enSciPort, pSciSetupPara->RxRetries, pSciSetupPara->TxRetries);

    SCI_HAL_SetRxTime(enSciPort, pSciSetupPara->RxTimeout);

    return;
}

HI_VOID SCI_CalcEtuBaud(HI_UNF_SCI_PORT_E enSciPort, HI_U32 ClkRate, HI_U32 BitRate)
{
    HI_U32 BaudTotal, EtuValue, BaudValue;
    HI_U32 ActualSciClk;

    ActualSciClk = g_SciPara[enSciPort].u32SysClk / (g_SciPara[enSciPort].SciState.SciSetupPara.ClockIcc + 1) / 2;
    BaudTotal = (ClkRate / BitRate) * (g_SciPara[enSciPort].u32SysClk / ActualSciClk);

    EtuValue = 5;
    while (EtuValue < 255)
    {
        if ((BaudTotal % EtuValue == 0) && ((BaudTotal / EtuValue - 1) < 0xffff))
        {
            break;
        }

        EtuValue++;
    }

    BaudValue = BaudTotal / EtuValue - 1;

    g_SciPara[enSciPort].SciState.SciSetupPara.EtuValue  = EtuValue;
    g_SciPara[enSciPort].SciState.SciSetupPara.BaudValue = BaudValue;

    SCI_HAL_SetEtu(enSciPort, EtuValue);
    SCI_HAL_SetBaud(enSciPort, BaudValue);

    return;
}

HI_VOID SCI_CfgT1Para(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_SETUP_PARA_S      *pSciSetupPara;

    pSciSetupPara = &g_SciPara[enSciPort].SciState.SciSetupPara;

    SCI_HAL_SetCharTimeout(enSciPort, pSciSetupPara->CharTimeout);

    SCI_HAL_SetBlockTimeout(enSciPort, pSciSetupPara->BlockTimeout);

    SCI_HAL_SetBlockGuard(enSciPort, pSciSetupPara->BlockGuard);

    return;
}

HI_VOID SCI_ConfigT0(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_DefaultConfig(enSciPort);

    SCI_CalcEtuBaud(enSciPort, SCI_ClkRate[F_372], SCI_BitRate[D_1]);

    SCI_HAL_SetParityEnable(enSciPort, HI_TRUE);

    /* if T0 support BlockGuard, it can be set here*/
}

HI_VOID SCI_ConfigT1(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_DefaultConfig(enSciPort);

    SCI_CfgT1Para(enSciPort);

    SCI_CalcEtuBaud(enSciPort, SCI_ClkRate[F_372], SCI_BitRate[D_1]);

    SCI_HAL_SetParityEnable(enSciPort, HI_TRUE);
}

/* T14 don't need parity check,set it default F factor equel 620 and D factor equal 1*/
HI_VOID SCI_ConfigT14(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_DefaultConfig(enSciPort);

    g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate = 620;
    SCI_CalcEtuBaud(enSciPort, 620, SCI_BitRate[D_1]);

    SCI_HAL_SetParityEnable(enSciPort, HI_FALSE);

    /* if T14 support BlockGuard, it can be set here*/
}

HI_VOID SCI_ConfigProtocol(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_UNF_SCI_PROTOCOL_E enSciProtocol;

    enSciProtocol = g_SciPara[enSciPort].SciAttr.enSciProtocol;

    if (HI_UNF_SCI_PROTOCOL_T14 == enSciProtocol)
    {
        SCI_ConfigT14(enSciPort);
    }
    else if (HI_UNF_SCI_PROTOCOL_T0 == enSciProtocol)
    {
        SCI_ConfigT0(enSciPort);
    }
    else if (HI_UNF_SCI_PROTOCOL_T1 == enSciProtocol)
    {
        SCI_ConfigT1(enSciPort);
    }

    return;
}

HI_VOID SCI_CardIn(HI_UNF_SCI_PORT_E enSciPort)
{
    if (g_SciPara[enSciPort].SciState.enSciCrtState == HI_UNF_SCI_STATUS_NOCARD)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
    }

    return;
}

HI_VOID SCI_CardOut(HI_UNF_SCI_PORT_E enSciPort)
{
    g_SciPara[enSciPort].SciState.bForceFlag = HI_FALSE;
    g_SciPara[enSciPort].SciState.bSetExtBaudFlag = HI_FALSE;

    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_NOCARD);

    wake_up_interruptible(&g_SciPara[enSciPort].SciRecWaitQueue);
}

HI_VOID SCI_CardDown(HI_UNF_SCI_PORT_E enSciPort)
{
    if (g_SciPara[enSciPort].SciState.enSciCrtState > HI_UNF_SCI_STATUS_INACTIVECARD)
    {
        SCI_HAL_DisableAllInt(enSciPort);
        SCI_HAL_ClearAllInt(enSciPort);

        SCI_ConfigProtocol(enSciPort);

        g_SciPara[enSciPort].ErrType = 0;
        g_SciPara[enSciPort].SciState.bForceFlag = HI_FALSE;
        g_SciPara[enSciPort].SciState.bSetExtBaudFlag = HI_FALSE;

        SCI_CleanBuf(enSciPort);

        g_SciAtrBuf[enSciPort].DataLen = 0;
        g_SciPara[enSciPort].SciState.SciArtPara.Grouping = SCI_ATR_GROUP_START;
        g_SciPara[enSciPort].SciState.SciArtPara.Checksum = 0;
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask = 0;

        if (SCI_HAL_GetCardPresent(enSciPort))
        {
            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
        }
        else
        {
            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_NOCARD);
        }

        SCI_HAL_EnableInt(enSciPort, SCI_DFT_INT);
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState != HI_UNF_SCI_STATUS_NOCARD)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
    }
}

HI_VOID SCI_RxTide(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_U32 RegData;
    HI_U32 DataLen;

    if (((g_SciDataBuf[enSciPort].Write + 1) % SCI_READ_BUF_LEN) == g_SciDataBuf[enSciPort].Read)
    {
        SCI_CleanBuf(enSciPort);
        return;
    }

    RegData = SCI_HAL_ReadData(enSciPort);

    if (SCI_HAL_GetParityEnable(enSciPort) && (RegData & 0x100))
    {
        HI_ERR_SCI("SCI parity failed.\n");
    }

    g_SciDataBuf[enSciPort].DataBuf[g_SciDataBuf[enSciPort].Write] = (unsigned char)(RegData);

    g_SciDataBuf[enSciPort].Write = (g_SciDataBuf[enSciPort].Write + 1) % SCI_READ_BUF_LEN;

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        tasklet_schedule(&g_SciPara[enSciPort].SciReadTasklet);
    }
    else
    {
        if (!g_SciDataBuf[enSciPort].DataLen)
        {
            return;
        }

        if (g_SciDataBuf[enSciPort].Write >= g_SciDataBuf[enSciPort].Read)
        {
            DataLen = g_SciDataBuf[enSciPort].Write - g_SciDataBuf[enSciPort].Read;
        }
        else
        {
            DataLen = SCI_READ_BUF_LEN - g_SciDataBuf[enSciPort].Read + g_SciDataBuf[enSciPort].Write;
        }

        if (DataLen >= g_SciDataBuf[enSciPort].DataLen)
        {
            g_SciPara[enSciPort].bDataEnough = HI_TRUE;
            wake_up_interruptible(&g_SciPara[enSciPort].SciRecWaitQueue);
        }
    }

    return;
}

HI_VOID SCI_PushData(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_U32 FifoLen = 0;
    HI_U8 *pTmpBuf;
    HI_U32 u32Count = 0;

    if (g_SciSendDataBuf[enSciPort].Read < g_SciSendDataBuf[enSciPort].DataLen)
    {
        SCI_HAL_DisableInt(enSciPort, SCI_INT_TXTIDE);
        FifoLen = SCI_HAL_GetTxFifo(enSciPort);

        pTmpBuf = &(g_SciSendDataBuf[enSciPort].DataBuf[g_SciSendDataBuf[enSciPort].Read]);
        for(u32Count=FifoLen;
            ((u32Count<SCI_FIFO_SIZE) 
            && (g_SciSendDataBuf[enSciPort].Read < g_SciSendDataBuf[enSciPort].DataLen));
            u32Count++)
        {
            SCI_HAL_WriteData(enSciPort, *pTmpBuf);
            pTmpBuf++;
            g_SciSendDataBuf[enSciPort].Read++;
        }
        
        if (g_SciSendDataBuf[enSciPort].Read == g_SciSendDataBuf[enSciPort].DataLen)
        {
            SCI_HAL_ClearRxFifo(enSciPort);
            SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);
            wake_up_interruptible(&g_SciPara[enSciPort].SciRecWaitQueue);
        }
        else
        {
            SCI_HAL_EnableInt(enSciPort, SCI_INT_TXTIDE);
        }
    }
    else
    {
        SCI_HAL_ClearRxFifo(enSciPort);
        SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);
        SCI_HAL_DisableInt(enSciPort, SCI_INT_TXTIDE);
    }

    return;
}

HI_VOID SCI_CharReversal(HI_U8 *pAtrData)
{
    HI_S32 TmpValue = 0;
    HI_S32 i;

    for (i = 8; i > 0; i--)
    {
        if (*pAtrData & 0x1)
        {
            TmpValue |= 0x1;
        }

        TmpValue  = TmpValue << 1;
        *pAtrData = *pAtrData >> 1;
    }

    *pAtrData = 0xFF & ~(TmpValue >> 1);
}

HI_VOID SCI_PPS_ClearDataBuf(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_U32 i;

    for (i = 0; i < 6; i++)
    {
        g_SciPPS[enSciPort].Send[i] = 0;
        g_SciPPS[enSciPort].Receive[i] = 0;
    }

    g_SciPPS[enSciPort].SendLen = 0;
    g_SciPPS[enSciPort].ReceiveLen = 0;
}

HI_S32 SCI_PPS_SendPackage(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSendBuf, HI_U32 PPSLen)
{
    HI_S32 SendNo = 0, tmp;
    HI_U32 SendChecksum = 0, PPSFormat = 0;

    /*need clear PPS data buffer before send package*/
    SCI_PPS_ClearDataBuf(enSciPort);

    /*It will  negotiate PPS with ATR Protocol and TA1 value return from card  when PPSLen is 0 */
    if (0 == PPSLen)
    {
        g_SciPPS[enSciPort].Send[SendNo] = 0xff;    /*PPS*/
        SendNo++;
        if ((HI_UNF_SCI_PROTOCOL_T0 != g_SciPara[enSciPort].SciAttr.enSciProtocol) \
            && (HI_UNF_SCI_PROTOCOL_T1 != g_SciPara[enSciPort].SciAttr.enSciProtocol) \
            && (HI_UNF_SCI_PROTOCOL_T14 != g_SciPara[enSciPort].SciAttr.enSciProtocol))
        {
            HI_ERR_SCI("Not support PPS protocol type !\n");
            return HI_ERR_SCI_PPS_PTYPE_ERR;
        }

        if (HI_UNF_SCI_PROTOCOL_T14 == g_SciPara[enSciPort].SciAttr.enSciProtocol)
        {
            PPSFormat |= ((g_SciPara[enSciPort].SciAttr.enSciProtocol + 12) & 0x0F);
        }
        else
        {
            PPSFormat |= (g_SciPara[enSciPort].SciAttr.enSciProtocol & 0x0F);
        }

        g_SciPPS[enSciPort].Send[SendNo] = PPSFormat;       /*PPS0*/

        if ((g_SciAtrBuf[enSciPort].DataBuf[1] & 0x10))     /*Atr TA1 present,will send PPS1 */
        {
            PPSFormat |= (0x01 << 4);
            g_SciPPS[enSciPort].Send[SendNo] = PPSFormat;   /*PPS0*/

            SendNo++;
            g_SciPPS[enSciPort].Send[SendNo] = g_SciAtrBuf[enSciPort].DataBuf[2];       /*PPS1*/
        }
        else
        {
            HI_INFO_SCI("No PPS factor negotiation !\n");
        }

        SendNo++;
        tmp = SendNo;
        while (tmp > 0)
        {
            SendChecksum = SendChecksum ^ g_SciPPS[enSciPort].Send[--tmp];
            HI_INFO_SCI(" g_SciPPS.Send[%d]: %X  \n", tmp, g_SciPPS[enSciPort].Send[tmp]);
        }

        g_SciPPS[enSciPort].Send[SendNo] = SendChecksum;        /*PCK*/
        g_SciPPS[enSciPort].SendLen = SendNo + 1;
        HI_INFO_SCI(" g_SciPPS.Send[%d]: %X  \n", SendNo, g_SciPPS[enSciPort].Send[SendNo]);
    }
    /*It will negotiate PPS with pSendBuf value from user*/
    else
    {
        memcpy(g_SciPPS[enSciPort].Send, pSendBuf, PPSLen);
        if (0xff != g_SciPPS[enSciPort].Send[0])
        {
            HI_ERR_SCI("Not PPS Command !\n");
            return HI_ERR_SCI_PPS_NOTSUPPORT_ERR;
        }

        if (HI_UNF_SCI_PROTOCOL_T0 != (g_SciPPS[enSciPort].Send[1] & 0x0f)    \
            && (HI_UNF_SCI_PROTOCOL_T1 != (g_SciPPS[enSciPort].Send[1] & 0x0f))    \
            && (14 != (g_SciPPS[enSciPort].Send[1] & 0x0f)))
        {
            HI_ERR_SCI("Not support PPS protocol type !\n");
            return HI_ERR_SCI_PPS_PTYPE_ERR;
        }

        if (!(g_SciPPS[enSciPort].Send[1] & 0x10))
        {
            HI_INFO_SCI("No PPS factor negotiation !\n");
        }

        for (tmp = 0; tmp < PPSLen; tmp++)
        {
            HI_INFO_SCI(" g_SciPPS.Send[%d]: %X  \n", tmp, g_SciPPS[enSciPort].Send[tmp]);
            SendChecksum ^= g_SciPPS[enSciPort].Send[tmp];
        }

        g_SciPPS[enSciPort].Send[tmp] = SendChecksum;
        HI_INFO_SCI(" g_SciPPS.Send[%d]: %X  \n", tmp, g_SciPPS[enSciPort].Send[tmp]);

        g_SciPPS[enSciPort].SendLen = PPSLen + 1;
    }

    return HI_SUCCESS;
}

HI_S32 SCI_PPS_Receive(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RecTimeouts)
{
    HI_S32 Ret, ReceiveNo = 0, tmp = 0, i, PPSFlag = HI_SUCCESS;
    HI_U32 RelRecLen, Format, times;
    HI_BOOL ChProtocol = HI_FALSE, Flag = HI_FALSE;

	CHECK_PORTID();

    g_SciPPS[enSciPort].ReceiveLen = 0;
    Ret = SCI_ReceivePPSData( enSciPort, &g_SciPPS[enSciPort].Receive[ReceiveNo], 1, &RelRecLen, RecTimeouts);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SCI("receive PPSS  fail,expect receive %d,but noly receive %d \n", 1, RelRecLen);
        return Ret;
    }

    if (0xff != g_SciPPS[enSciPort].Receive[ReceiveNo]) /*if isn't card  PPS response will warm reset and return*/
    {
        HI_ERR_SCI("receive data isn't PPS  response\n");
        return HI_ERR_SCI_PPS_NOTSUPPORT_ERR;
    }

    ReceiveNo++;
    Ret = SCI_ReceivePPSData( enSciPort, &g_SciPPS[enSciPort].Receive[ReceiveNo], 1, &RelRecLen, RecTimeouts);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SCI("receive PPS0  fail,expect receive %d,but noly receive %d \n", 1, RelRecLen);
        return Ret;
    }

    Format = g_SciPPS[enSciPort].Receive[ReceiveNo];
    if ((Format & 0x0f) == (g_SciPPS[enSciPort].Send[1] & 0X0F))
    {
        /*if PPS negotiation protocol don't equal original set protocal will reconfig protocal */
        if ((Format & 0x0f) != g_SciPara[enSciPort].SciAttr.enSciProtocol)
        {
            ChProtocol = HI_TRUE;
        }

        g_SciPara[enSciPort].SciState.enSciProtocol = Format & 0x0f;
        g_SciPara[enSciPort].SciAttr.enSciProtocol = Format & 0x0f;
        Format = Format >> 3;
    }
    else
    {
        PPSFlag = HI_ERR_SCI_PPS_PTYPE_ERR;
    }

    for (i = 0; i < 4; i++)
    {
        tmp++;              /*receive counter*/
        Format = Format >> 1; /*Format indicate which PPSx (1~3) will receive*/

        if ((Format & 0x01) || (4 == tmp)) /*4==tmp is PCK*/
        {
            ReceiveNo++;

            Ret = SCI_ReceivePPSData( enSciPort, &g_SciPPS[enSciPort].Receive[ReceiveNo], 1, &RelRecLen, RecTimeouts);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_SCI("receive PPSx pakeage fail,expect receive %d,but noly receive %d \n", 1, RelRecLen);
                PPSFlag = HI_ERR_SCI_RECEIVE_ERR;
                break;
            }
        }

        switch (tmp)
        {
        case 1:         /*PPS1*/
            if (Format & 0x01)
            {
                /*PPS0 bit5 =1 and PPS1 response == PPS1 requeset, negotiation success,set negotiation Fn, Dn*/
                if (g_SciPPS[enSciPort].Receive[ReceiveNo] == g_SciPPS[enSciPort].Send[2])
                {
                    g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate =
                        SCI_ClkRate[(g_SciPPS[enSciPort].Receive[ReceiveNo] & 0xf0) >> 4];
                    g_SciPara[enSciPort].SciState.SciSetupPara.BitRate =
                        SCI_BitRate[(g_SciPPS[enSciPort].Receive[ReceiveNo] & 0x0f)];
                }
                else
                {
                    PPSFlag = HI_ERR_SCI_PPS_FACTOR_ERR;
                }
            }
            else //negotiation success
            {
                /*if PPS0 bit = 0, PPS1 is absent, set negotiation Fd/Dd*/
                g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate = 372;
                g_SciPara[enSciPort].SciState.SciSetupPara.BitRate = 1; 
            }

            break;

        case 2:         /*PPS2*/
            if (Format & 0x01)
            {
                HI_INFO_SCI(" PPS2.\n");
            }

            break;

        case 3:         /*PPS3*/

            break;

        case 4:              /*PCK*/

            break;

        default:
            PPSFlag = HI_ERR_SCI_PPS_NOTSUPPORT_ERR;
            break;
        }
    }

    g_SciPPS[enSciPort].ReceiveLen = ReceiveNo + 1;

    while (ReceiveNo >= 0)
    {
        HI_INFO_SCI("g_SciPPS[enSciPort].Receive[%d] = %x \n", ReceiveNo, g_SciPPS[enSciPort].Receive[ReceiveNo]);
        ReceiveNo--;
    }

    udelay(100);

    if (HI_SUCCESS == PPSFlag)
    {
        HI_S32 Ret;

        Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

        if (!g_SciPara[enSciPort].bSciEnable)
        {
            HI_ERR_SCI("PPS set:SCI %d is not opened.\n", enSciPort);
            up(&g_SciPara[enSciPort].SciSem);
            return HI_ERR_SCI_INVALID_OPT;
        }

        if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
        {
            HI_ERR_SCI("PPS set:SCI%d state is not ready\n", enSciPort);
            up(&g_SciPara[enSciPort].SciSem);
            return HI_ERR_SCI_RECEIVE_ERR;
        }

        /*have change protocol,will reconfig*/
        if (HI_TRUE == ChProtocol)
        {
            HI_FATAL_SCI("PPS negotiation change protocol \n");
            SCI_ConfigProtocol(enSciPort);
        }

        SCI_CalcEtuBaud(enSciPort, g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate,
                        g_SciPara[enSciPort].SciState.SciSetupPara.BitRate );
        up(&g_SciPara[enSciPort].SciSem);
        HI_INFO_SCI("PPS negotiation success \n");
    }
    else
    {
        HI_ERR_SCI("SCI%d PPS negotiatioan fail,will warm reset... \n", enSciPort);
        Ret = SCI_Reset( enSciPort, HI_TRUE);

        for (times = 0; times < 60; times++)
        {
            if (g_SciPara[enSciPort].SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READY)
            {
                Flag = HI_TRUE;
                break;
            }

            mdelay(50);

            HI_FATAL_SCI("SCI PPS reset count %d!\n", times);
        }

        if (Flag)
        {
            HI_FATAL_SCI("SCI  PPS warm Reset OK!\n");
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_SCI("SCI  PPS warm Reset Fail!\n ");
        }
    }

    return PPSFlag;
}

HI_S32 SCI_PPS_Negotiation(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSendBuf, HI_U32 PPSLen, HI_U32 RecTimeouts)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 SendLen, RelSendLen;
    
	CHECK_PORTID();
	if(PPSLen > 6)
		return HI_ERR_SCI_INVALID_PARA;

    /*TA2 absent is Negotiation mode*/
    if (HI_FALSE == g_SciPara[enSciPort].SciState.bForceFlag)
    {
        /*Package  PPS command*/
        Ret = SCI_PPS_SendPackage( enSciPort, pSendBuf, PPSLen);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_SCI("SCI%d  SCI_PPS_SendPackage fail \n", enSciPort);
            return Ret;
        }

        SendLen = g_SciPPS[enSciPort].SendLen;
    }
    /*TA2 present and bit5=0, will direct set TA1*/
    else
    {
        HI_ERR_SCI(" SCI%d direct set TA1, not need PPS  Negotiation .\n", enSciPort);
        return HI_ERR_SCI_PPS_NOTSUPPORT_ERR;
    }

    /*Send PPS command to card */
    Ret = SCI_SendPPSData( enSciPort, g_SciPPS[enSciPort].Send, SendLen, &RelSendLen, 5000);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SCI("send PPS pakeage fail,expect send %d,but noly send %d \n", SendLen, RelSendLen);
        return Ret;
    }

    /*Receive PPS response from card */
    Ret = SCI_PPS_Receive( enSciPort, RecTimeouts);

    return Ret;
}

HI_S32 SCI_SetAtrByte(HI_UNF_SCI_PORT_E enSciPort, HI_U32 AtrMask)
{

	CHECK_PORTID();

    //Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    /*TA1 present && TA2 present and bit5=0, will direct set TA1*/
    if ((g_SciAtrBuf[enSciPort].DataBuf[1] & 0x10) && g_SciPara[enSciPort].SciState.bForceFlag)
    {
        HI_FATAL_SCI(" SCI%d driect set TA1.\n", enSciPort);
        g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate =
            SCI_ClkRate[g_SciPara[enSciPort].SciState.SciArtPara.enClkRate];
        g_SciPara[enSciPort].SciState.SciSetupPara.BitRate =
            SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate];
        SCI_CalcEtuBaud(enSciPort, SCI_ClkRate[g_SciPara[enSciPort].SciState.SciArtPara.enClkRate],
                        SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate]);
    }

    /*TC1, set char guard time in opposite direction*/
    if (AtrMask & (0x1 << 4))
    {
        if (g_SciPara[enSciPort].SciState.SciArtPara.ChGuard < 255)
        {
            if (
                (HI_UNF_SCI_PROTOCOL_T0 == g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType)
                || (HI_UNF_SCI_PROTOCOL_T14 == g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType)
            )
            {
                SCI_HAL_SetBlockGuard(enSciPort, g_SciPara[enSciPort].SciState.SciArtPara.ChGuard);
            }
        }
    }

    /*set TC2*/
    if (AtrMask & (0x1 << 8))
    {
        SCI_HAL_SetCharTimeout(enSciPort, g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime);
    }

    /*set TB3*/
    if (AtrMask & (0x1 << 11))
    {
        SCI_HAL_SetCharTimeout(enSciPort, g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime);
        SCI_HAL_SetBlockTimeout(enSciPort, g_SciPara[enSciPort].SciState.SciArtPara.BlockTime);
    }

    //up(&g_SciPara[enSciPort].SciSem);

    return HI_SUCCESS;
}

HI_S32 SCI_AtrProcessTS(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    if (SCI_ATR_CONV_NORM == AtrData)
    {
        g_SciPara[enSciPort].SciState.bSciDataReversal = HI_FALSE;
    }
    else if (SCI_ATR_CONV_NORM_INV == AtrData)
    {
        g_SciPara[enSciPort].SciState.bSciDataReversal = HI_TRUE;
    }
    else if (SCI_ATR_CONV_INV == AtrData)
    {
        g_SciPara[enSciPort].SciState.bSciDataReversal = HI_TRUE;
        g_SciAtrBuf[enSciPort].DataBuf[0] = SCI_ATR_CONV_NORM_INV;
    }
    else
    {
        HI_ERR_SCI("SCI_AtrProcessTS error.\n");
        return HI_FAILURE;
    }

    g_SciPara[enSciPort].SciState.SciArtPara.Grouping = 0;         /*format character*/
    g_SciPara[enSciPort].SciState.SciArtPara.Current  = 0;         /*format character*/
    g_SciPara[enSciPort].SciState.SciArtPara.Checksum = 0;         /*initialise the checksum*/
    g_SciPara[enSciPort].SciState.SciArtPara.AtrMask   = 0;
    g_SciPara[enSciPort].SciState.SciArtPara.ChGuard   = 0;
    g_SciPara[enSciPort].SciState.SciArtPara.enClkRate = F_372;  /*clock rate conversion*/
    g_SciPara[enSciPort].SciState.SciArtPara.enBitRate = D_1;    /*bit rate adjustment*/

    return HI_SUCCESS;
}

HI_VOID SCI_AtrFormatTi(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    HI_U8 TiLs;

    TiLs = AtrData & 0xf;

    /*mask of expected characters A-D.
     *Bit 0 = A, Bit 1 = B, Bit 2 = C, Bit 3 = D */
    g_SciPara[enSciPort].SciState.SciArtPara.CharPresent = (AtrData & 0xf0) >> 4;

    if (0 == g_SciPara[enSciPort].SciState.SciArtPara.Grouping)
    {
        /*check if it's the T0 character*/
        g_SciPara[enSciPort].SciState.SciArtPara.NumHist = TiLs;         /*number of historical chars*/
        g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType  = 0;      /*first block always protocol 0*/
        g_SciPara[enSciPort].SciState.SciArtPara.bIntfByteQual = HI_FALSE;
        g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum = HI_FALSE;
    }
    else if (1 == g_SciPara[enSciPort].SciState.SciArtPara.Grouping)
    {
        g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType = TiLs;  /*first offered protocol*/
    }
    else if ((g_SciPara[enSciPort].SciState.SciArtPara.Grouping > 1) && (TiLs == 15))
    {
        g_SciPara[enSciPort].SciState.SciArtPara.bIntfByteQual = HI_TRUE; /* T = 15 indicates gobal interface byte */
        g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum = HI_TRUE;       /*we now expect a checksum*/
    }

    /*check if this block specifies a non-zero protocol*/
    if (1 == g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType)
    {
        g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum = HI_TRUE;              /*we now expect a checksum*/
        g_SciPara[enSciPort].SciState.enSciProtocol = HI_UNF_SCI_PROTOCOL_T1;
        SCI_HAL_SetBlockGuard(enSciPort, SCI_DFT_BLKGUARD_TIME - 12);
    }
    else if (14 == g_SciPara[enSciPort].SciState.SciArtPara.ProtocolType)
    {
        g_SciPara[enSciPort].SciState.enSciProtocol = HI_UNF_SCI_PROTOCOL_T14;
        g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum = HI_TRUE;  /*we now expect a checksum*/
    }

    if (!g_SciPara[enSciPort].SciState.SciArtPara.CharPresent)
    {
        /*stop if no chars in next block*/
        g_SciPara[enSciPort].SciState.SciArtPara.Grouping = SCI_ATR_GROUP_DONE;
        return;
    }

    g_SciPara[enSciPort].SciState.SciArtPara.Grouping++;          /*next group (i) of characters*/
    g_SciPara[enSciPort].SciState.SciArtPara.Current = 0;         /*Character TAi is next*/

    return;
}

HI_VOID SCI_AtrParamSet(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    /*Create pointers to the instance data block and the register base*/
    HI_U32 AtrGrouping, AtrCurrent;
    HI_S32 AtrItem;

    AtrGrouping = g_SciPara[enSciPort].SciState.SciArtPara.Grouping;
    AtrCurrent = g_SciPara[enSciPort].SciState.SciArtPara.Current;

    /*The ATR grouping and the current item within the grouping determine
     *how characters are interpreted.  The PL131_ATR_ITEM macro allows us to use a
     *single switch statement
     */
    AtrItem = SCI_ATR_ITEM(AtrGrouping, AtrCurrent);

    switch (AtrItem)
    {
        /*specify the clock rate conversion and bit rate adjustment factor*/
    case 0xA1:        /*character A1*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.enClkRate = (AtrData & 0xf0) >> 4;      /*clock rate conversion*/
        g_SciPara[enSciPort].SciState.SciArtPara.enBitRate = AtrData & 0xf;              /*bit rate adjustment*/
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 2);
        break;
    }

        /*programa valtage and current ,chip not support*/
    case 0xB1:    /*character B1*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 3);
        break;
    }

        /*specify the character-character guard time*/
    case 0xC1:        /*character C1*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.ChGuard = (AtrData & 0xff);

        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 4);
        break;
    }

        /*TA2 present is specify mode*/
    case 0xA2:
    {
        if (!(AtrData & 0x10))
        {
            g_SciPara[enSciPort].SciState.bForceFlag = HI_TRUE;
        }

        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 6);
        break;
    }

        /*programa valtage use replace TB1 ,chip not support*/
    case 0xB2:    /*character B2*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 7);
        break;
    }

        /*tow  sequence Char timeout, noly for T0*/
    case 0xC2:    /*character C2*/
    {
        HI_U32 Di;

        Di = SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate];

        if (Di > 0)
        {
            g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime = (AtrData & 0xff) * 960 * Di;
            g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout = (AtrData & 0xff) * 960 * Di;
        }
        else
        {
            g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime = (AtrData & 0xff) * 960;
            g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout = (AtrData & 0xff) * 960;
        }

        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 8);
        break;
    }

        /*T1 card information area size */
    case 0xA3:    /*character A3*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.DataInfSize = (AtrData & 0xff);
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 10);
        break;
    }

        /*T1  CWT and BWT*/
    case 0xB3:    /*character B3*/
    {
        HI_U32 Value;

        Value = (AtrData & 0xf0) >> 4;

        if (g_SciPara[enSciPort].SciState.SciArtPara.enClkRate)
        {
            HI_U32 Di;
            HI_U32 Fi;

            Fi = SCI_ClkRate[g_SciPara[enSciPort].SciState.SciArtPara.enClkRate];
            Di = SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate];

            if ((Fi > 0) && (Di > 0))
            {
                g_SciPara[enSciPort].SciState.SciArtPara.BlockTime = (1 << Value) * 960 * 372 * Di / Fi + 11;
            }
            else
            {
                g_SciPara[enSciPort].SciState.SciArtPara.BlockTime = (1 << Value) * 960 + 11;
            }

            g_SciPara[enSciPort].SciState.SciSetupPara.BlockTimeout =
                g_SciPara[enSciPort].SciState.SciArtPara.BlockTime;
        }

        Value = AtrData & 0xf;

        g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime = (1 << Value) + 11;
        g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout = (1 << Value) + 11;
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 11);

        break;
    }

        /*T1 checksum type*/
    case 0xC3:    /*character C3*/
    {
        g_SciPara[enSciPort].SciState.SciArtPara.ChecksumType = (AtrData & 0x01);
        g_SciPara[enSciPort].SciState.SciArtPara.AtrMask |= (0X01 << 12);
        break;
    }

        /*notify the user of an ATR item*/
    default:
    {
        break;
    }
    }

    return;
}

HI_S32 SCI_AtrProcessFormat(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    if ((0 == g_SciPara[enSciPort].SciState.SciArtPara.Current)
        || (4 == g_SciPara[enSciPort].SciState.SciArtPara.Current)
    )
    {
        SCI_AtrFormatTi(enSciPort, AtrData);
    }
    /*other character of ATR (TAi, TBi, TCi)*/
    else
    {
        SCI_AtrParamSet(enSciPort, AtrData);
    }

    /*next character of ATR (TAi, TBi, TCi, TDi)*/

    /*find which character (A-D) is expected next*/
    do
    {
        g_SciPara[enSciPort].SciState.SciArtPara.Current++;
    } while ((g_SciPara[enSciPort].SciState.SciArtPara.Current < 5)
             && (!(g_SciPara[enSciPort].SciState.SciArtPara.CharPresent
                   & (1UL << (g_SciPara[enSciPort].SciState.SciArtPara.Current - 1))
                 )));

    /*if we've reached Current == 5, this block has finished*/
    if (5 == g_SciPara[enSciPort].SciState.SciArtPara.Current)
    {
        g_SciPara[enSciPort].SciState.SciArtPara.Grouping = SCI_ATR_GROUP_DONE;
        SCI_HAL_SetTide(enSciPort, 1, 1);
    }

    return HI_SUCCESS;
}

HI_S32 SCI_AtrProcessEnd(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    HI_U32 CharGuard = 0;

    /*--------TODO need wait up getart cmd--------*/
    SCI_HAL_DisableInt(enSciPort, SCI_INT_ATRDTOUT);

    /*Guard time*/
    if (255 == g_SciPara[enSciPort].SciState.SciArtPara.ChGuard)
    {
        if (HI_UNF_SCI_PROTOCOL_T0 == g_SciPara[enSciPort].SciState.enSciProtocol)
        {
            CharGuard = 0;
        }
        else
        {
            CharGuard = 0;
        }
    }
    else if (g_SciPara[enSciPort].SciState.SciArtPara.ChGuard < 255)
    {
        if (HI_UNF_SCI_PROTOCOL_T1 == g_SciPara[enSciPort].SciState.enSciProtocol)
        {
            CharGuard = g_SciPara[enSciPort].SciState.SciArtPara.ChGuard + 1;
        }
        else
        {
            CharGuard = g_SciPara[enSciPort].SciState.SciArtPara.ChGuard;
        }
    }

    SCI_HAL_SetCharGuard(enSciPort, CharGuard);

    /*set handshake settings for T0 mode*/
    if (HI_UNF_SCI_PROTOCOL_T0 == g_SciPara[enSciPort].SciState.enSciProtocol)
    {
        SCI_HAL_SetTxParityMode(enSciPort, HI_TRUE, SCI_PARITY_EVEN);
        SCI_HAL_SetRxParityMode(enSciPort, HI_TRUE, SCI_PARITY_EVEN);
    }
    else
    {
        SCI_HAL_SetTxParityMode(enSciPort, HI_FALSE, SCI_PARITY_EVEN);
        SCI_HAL_SetRxParityMode(enSciPort, HI_FALSE, SCI_PARITY_EVEN);
    }

    /*set data convention for remaining data*/
    SCI_HAL_SetDataReversal(enSciPort, g_SciPara[enSciPort].SciState.bSciDataReversal);
    SCI_HAL_SetAtrTimeoutEnable(enSciPort, HI_FALSE);
#if 0
    if ((g_SciAtrBuf[enSciPort].DataBuf[1] & 0x10) && g_SciPara[enSciPort].SciState.bForceFlag)
    {
        g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate =
            SCI_ClkRate[g_SciPara[enSciPort].SciState.SciArtPara.enClkRate];
        g_SciPara[enSciPort].SciState.SciSetupPara.BitRate =
            SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate];
        SCI_CalcEtuBaud(enSciPort, SCI_ClkRate[g_SciPara[enSciPort].SciState.SciArtPara.enClkRate],
                        SCI_BitRate[g_SciPara[enSciPort].SciState.SciArtPara.enBitRate]);
    }
#endif


    SCI_SetAtrByte(enSciPort, g_SciPara[enSciPort].SciState.SciArtPara.AtrMask);

    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_READY);

    return HI_SUCCESS;
}

HI_S32 SCI_AtrProcessHis(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    /*character after historical is the checksum - should be zero*/
    if (g_SciPara[enSciPort].SciState.SciArtPara.NumHist == 0)
    {
        if (g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum)
        {
            HI_U32 tmp = g_SciAtrBuf[enSciPort].DataLen;
            while (tmp-- > 1)
            {
                g_SciPara[enSciPort].SciState.SciArtPara.Checksum =
                    (HI_U8)(g_SciPara[enSciPort].SciState.SciArtPara.Checksum ^ g_SciAtrBuf[enSciPort].DataBuf[tmp]);
            }

            if (g_SciPara[enSciPort].SciState.SciArtPara.Checksum)
            {
                /* NOTIFICATION - ATR error*/
                HI_ERR_SCI("ATR error.\n");
                return HI_FAILURE;
            }
        }
    }

    /*ATR completed - set up parameters*/
    /*finished if reached 0 (or 1 and no checksum), NumHist == 0 have received TCK ,NumHist == 1 not have TCK use for T0*/
    if (((g_SciPara[enSciPort].SciState.SciArtPara.NumHist == 0)
         && g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum)
        || ((g_SciPara[enSciPort].SciState.SciArtPara.NumHist == 1)
            && (!g_SciPara[enSciPort].SciState.SciArtPara.bHasChecksum))
    )
    {
        SCI_AtrProcessEnd(enSciPort, AtrData);
    }

    if (g_SciPara[enSciPort].SciState.SciArtPara.NumHist != 0)
    {
        g_SciPara[enSciPort].SciState.SciArtPara.NumHist--; /*next historical character*/
    }

    return HI_SUCCESS;
}

HI_S32 SCI_AtrProcess(HI_UNF_SCI_PORT_E enSciPort, HI_U8 AtrData)
{
    if (g_SciPara[enSciPort].SciState.bSciDataReversal)
    {
        SCI_CharReversal(&AtrData);
    }

    g_SciAtrBuf[enSciPort].DataBuf[g_SciAtrBuf[enSciPort].DataLen] = AtrData;
    g_SciAtrBuf[enSciPort].DataLen++;

    /*first character of ATR (TS: convention)*/
    if (SCI_ATR_GROUP_START == g_SciPara[enSciPort].SciState.SciArtPara.Grouping)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_READATR);
        return SCI_AtrProcessTS(enSciPort, AtrData);
    }
    /*format character for next block (T0 or TDi)*/
    else if (g_SciPara[enSciPort].SciState.SciArtPara.Grouping != SCI_ATR_GROUP_DONE)
    {
        return SCI_AtrProcessFormat(enSciPort, AtrData);
    }
    /*historical data*/
    else
    {
        return SCI_AtrProcessHis(enSciPort, AtrData);
    }
}

HI_VOID SCI_Init(HI_VOID)
{
    HI_U32 i;
    SCI_PORT_ATTR_S     stPortAttr;
	SCI_REG_S * sciKerAdrr;

    for (i = HI_UNF_SCI_PORT0; i < HI_SCI_PORT_NUM; i++)
    {
        g_SciPara[i].bSciEnable = HI_FALSE;
        g_SciPara[i].SciAttr.enSciProtocol = HI_UNF_SCI_PROTOCOL_T0;
        g_SciPara[i].SciAttr.Frequency = 1000;
        g_SciPara[i].SciAttr.RxTimeout = 1000;
        g_SciPara[i].SciAttr.TxTimeout = 1000;
#if    defined(CHIP_TYPE_hi3716mv410)   \
		|| defined(CHIP_TYPE_hi3716mv420)
        g_SciPara[i].SciAttr.enSciVcc = HI_UNF_SCI_LEVEL_LOW;
#else
		g_SciPara[i].SciAttr.enSciVcc = HI_UNF_SCI_LEVEL_HIGH;
#endif
        g_SciPara[i].SciAttr.enSciDetect = HI_UNF_SCI_LEVEL_HIGH;
        g_SciPara[i].SciAttr.enClkMode = HI_UNF_SCI_MODE_OD;
        g_SciPara[i].SciAttr.enResetMode = HI_UNF_SCI_MODE_OD;
        g_SciPara[i].SciAttr.enVccEnMode = HI_UNF_SCI_MODE_OD;

        g_SciPara[i].ErrType = 0;
       
		g_SciPara[i].u32SysClk = SCI_DFT_REF_CLOCK; 
		
        HI_INIT_MUTEX(&g_SciPara[i].SciSem);

        init_waitqueue_head(&g_SciPara[i].SciRecWaitQueue);

        tasklet_init(&g_SciPara[i].SciReadTasklet, (HI_VOID *)SCI_ReadTasklet, i);

        if (HI_SUCCESS != SCI_GetPortAttr(i, &stPortAttr))
        {
            return;
        }
		sciKerAdrr = (SCI_REG_S *)ioremap_nocache(stPortAttr.u32RegBaseAddr, sizeof(SCI_REG_S));
		if (sciKerAdrr == HI_NULL)
		{
			HI_ERR_SCI("sci ioremap_nocache err!\n");
			return ;
		}

        SCI_HAL_Init(i, sciKerAdrr);
    }

    return;
}

HI_S32 SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency)
{
    HI_S32 Ret;
    static HI_CHAR s8Buff[HI_UNF_SCI_PORT_BUTT][16];
    SCI_PORT_ATTR_S stPortAttr;

    g_SciPara[enSciPort].SciAttr.enSciProtocol = enSciProtocol;
    g_SciPara[enSciPort].SciAttr.Frequency = Frequency;

    g_SciPara[enSciPort].SciState.enSciProtocol = enSciProtocol;

    /*enable clk module */
    SCI_HAL_SetSCIClock(enSciPort, HI_TRUE);
    SCI_HAL_ClearSCIReset(enSciPort);

    SCI_HAL_SetClkMode(enSciPort, g_SciPara[enSciPort].SciAttr.enClkMode);
    SCI_HAL_SetResetMode(enSciPort, g_SciPara[enSciPort].SciAttr.enResetMode);
    SCI_HAL_SetVccEnMode(enSciPort, g_SciPara[enSciPort].SciAttr.enVccEnMode);

    SCI_HAL_SetVcc(enSciPort, g_SciPara[enSciPort].SciAttr.enSciVcc);
    SCI_HAL_SetDetect(enSciPort, g_SciPara[enSciPort].SciAttr.enSciDetect);
    SCI_HAL_DisableAllInt(enSciPort);
    SCI_HAL_ClearAllInt(enSciPort);

    if (SCI_HAL_GetCardPresent(enSciPort))
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
    }
    else
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_NOCARD);
    }

    if (HI_SUCCESS != SCI_GetPortAttr(enSciPort, &stPortAttr))
    {
        return HI_FAILURE;
    }

    memset(s8Buff[enSciPort], 0, sizeof(s8Buff[enSciPort]));
    snprintf(s8Buff[enSciPort], sizeof(s8Buff[0]), "hi_sci%d_irq", enSciPort);

    Ret = request_irq(stPortAttr.u32IrqNo, SCI_Isr, IRQF_DISABLED, s8Buff[enSciPort], NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_FATAL_SCI("register SCI%d Isr failed 0x%x.\n", enSciPort, Ret);
        return HI_FAILURE;
    }
	else
	{
		if(HI_SUCCESS != HI_DRV_SYS_SetIrqAffinity(HI_ID_SCI, stPortAttr.u32IrqNo))
		{
        	HI_FATAL_SCI("HI_DRV_SYS_SetIrqAffinity failed.\n");
    	}	
	}

    SCI_HAL_EnableInt(enSciPort, SCI_DFT_INT);

    g_SciPara[enSciPort].bSciEnable = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 SCI_Close(HI_UNF_SCI_PORT_E enSciPort)
{
    SCI_PORT_ATTR_S stPortAttr;

    g_SciPara[enSciPort].bSciEnable = HI_FALSE;

	
    SCI_HAL_DisableAllInt(enSciPort);
	
	iounmap(g_pSciReg[enSciPort]);

    if (HI_SUCCESS != SCI_GetPortAttr(enSciPort, &stPortAttr))
    {
        return HI_FAILURE;
    }

    free_irq(stPortAttr.u32IrqNo, HI_NULL);

    /*close clk*/
    SCI_HAL_SetSCIReset(enSciPort);
    SCI_HAL_SetSCIClock(enSciPort, HI_FALSE);

    return HI_SUCCESS;
}

HI_S32 SCI_Reset(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmReset)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_HAL_DisableAllInt(enSciPort);
    SCI_HAL_ClearAllInt(enSciPort);

    SCI_ConfigProtocol(enSciPort);

    g_SciPara[enSciPort].ErrType = 0;
    g_SciPara[enSciPort].SciState.bForceFlag = HI_FALSE;
    g_SciPara[enSciPort].SciState.bSetExtBaudFlag = HI_FALSE;

    SCI_CleanBuf(enSciPort);

    g_SciAtrBuf[enSciPort].DataLen = 0;
    g_SciPara[enSciPort].SciState.SciArtPara.Grouping = SCI_ATR_GROUP_START;
    g_SciPara[enSciPort].SciState.SciArtPara.Checksum = 0;
    g_SciPara[enSciPort].SciState.SciArtPara.AtrMask = 0;
    g_SciPara[enSciPort].SciState.SciArtPara.CharacterTime = SCI_DFT_WAIT_TIME;
    g_SciPara[enSciPort].SciState.SciArtPara.BlockTime = SCI_DFT_BLOCK_TIME - 12;

    if (SCI_HAL_GetCardPresent(enSciPort))
    {
        if (!bWarmReset)
        {
            SCI_HAL_Finish(enSciPort);
            mdelay(10);

            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
            SCI_HAL_Startup(enSciPort);
        }
        else
        {
            if (g_SciPara[enSciPort].SciState.enSciCrtState > HI_UNF_SCI_STATUS_INACTIVECARD)
            {
                SCI_HAL_Reset(enSciPort);
            }
            else
            {
                SCI_HAL_Startup(enSciPort);
            }

            //          SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_WAITATR);
        }

        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_WAITATR);
    }
    else
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_NOCARD);
    }

    SCI_HAL_EnableInt(enSciPort, SCI_DFT_INT);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_Deactive(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (SCI_HAL_GetCardPresent(enSciPort))
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
        SCI_HAL_Finish(enSciPort);
        mdelay(10);
    }

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_GetAtr(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pAtrBuf, HI_U32 AtrBufSize, HI_U8 *pAtrDataLen)
{
    HI_S32 Ret;
    HI_U8 AtrTmpData[SCI_ATR_BUF_LEN];
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pAtrDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI has not atr.\n");
        *pAtrDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NO_ATR;
    }

    if (g_SciAtrBuf[enSciPort].DataLen > AtrBufSize)
    {
        *pAtrDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    *pAtrDataLen = g_SciAtrBuf[enSciPort].DataLen;

    memcpy((HI_VOID *)AtrTmpData, (HI_VOID *)g_SciAtrBuf[enSciPort].DataBuf, g_SciAtrBuf[enSciPort].DataLen);

    if (copy_to_user(pAtrBuf, AtrTmpData, *pAtrDataLen))
    {
        HI_ERR_SCI("copy atr data failed.\n");
        *pAtrDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_GetStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penState)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    *penState = g_SciPara[enSciPort].SciState.enSciCrtState;

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_GetSciPara(HI_UNF_SCI_PORT_E enSciPort, SCI_PARA_S  *pSciPara)
{
    HI_U32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);
    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_FIRSTINIT)
    {
        HI_ERR_SCI("SCI parameter invalid.\n");
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    memcpy(pSciPara, &g_SciPara[enSciPort], sizeof(SCI_PARA_S));
    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_GetSciAtrBuf(HI_UNF_SCI_PORT_E enSciPort, ATR_BUF_S  *pSciAtrBuf)
{
    HI_U32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);
    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI ATR has not ready.\n");
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NO_ATR;
    }

    memcpy(pSciAtrBuf, &g_SciAtrBuf[enSciPort], sizeof(ATR_BUF_S));
    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_PPS_GetRecBuf(HI_UNF_SCI_PORT_E enSciPort, HI_U8* pRecBuf, HI_U32* pDataLen)
{
    HI_U32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d has not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI state has not ready.\n");
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_PPS_NOTSUPPORT_ERR;
    }

    memcpy(pRecBuf, g_SciPPS[enSciPort].Receive, g_SciPPS[enSciPort].ReceiveLen);
    *pDataLen = g_SciPPS[enSciPort].ReceiveLen;

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_OutPutParam(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pSciOutParam)
{
    HI_U32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d has not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_WAITATR)
    {
        HI_ERR_SCI("SCI has not finished init.\n");
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NOT_INIT;
    }

    pSciOutParam->Fi = g_SciPara[enSciPort].SciState.SciArtPara.enClkRate;
    pSciOutParam->Di = g_SciPara[enSciPort].SciState.SciArtPara.enBitRate;
    pSciOutParam->ActalClkRate = g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate;
    pSciOutParam->ActalBitRate = g_SciPara[enSciPort].SciState.SciSetupPara.BitRate;
    pSciOutParam->GuardDelay = g_SciPara[enSciPort].SciState.SciArtPara.ChGuard;
    pSciOutParam->enProtocolType = g_SciPara[enSciPort].SciState.enSciProtocol;
    pSciOutParam->TxRetries = g_SciPara[enSciPort].SciState.SciSetupPara.TxRetries;

    if (HI_UNF_SCI_PROTOCOL_T1 == g_SciPara[enSciPort].SciState.enSciProtocol)
    {
        pSciOutParam->CharTimeouts  = g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout;
        pSciOutParam->BlockTimeouts = g_SciPara[enSciPort].SciState.SciSetupPara.BlockTimeout;
    }
    else
    {
        pSciOutParam->CharTimeouts  = g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout;
        pSciOutParam->BlockTimeouts = 0;
    }

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_ConfVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    g_SciPara[enSciPort].SciAttr.enSciVcc = enSciLevel;

    SCI_HAL_SetVcc(enSciPort, g_SciPara[enSciPort].SciAttr.enSciVcc);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_ConfDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    g_SciPara[enSciPort].SciAttr.enSciDetect = enSciLevel;

    SCI_HAL_SetDetect(enSciPort, g_SciPara[enSciPort].SciAttr.enSciDetect);

    msleep(150);
    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_ConfClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    g_SciPara[enSciPort].SciAttr.enClkMode = enClkMode;

    SCI_HAL_SetClkMode(enSciPort, g_SciPara[enSciPort].SciAttr.enClkMode);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_ConfResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)	\
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3716mv410)	\
    || defined(CHIP_TYPE_hi3716mv420)
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (enResetMode >= HI_UNF_SCI_MODE_BUTT)
    {
        HI_ERR_SCI("Invalid param, port:%d, mode:%d\n", enSciPort, enResetMode);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    g_SciPara[enSciPort].SciAttr.enResetMode = enResetMode;

    SCI_HAL_SetResetMode(enSciPort, enResetMode);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
#else

    return HI_ERR_SCI_NOTSUPPORT;
#endif
}

HI_S32 SCI_ConfVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)	\
    || defined(CHIP_TYPE_hi3798cv200)	\
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)	\
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3716mv410)	\
    || defined(CHIP_TYPE_hi3716mv420)
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (enVccEnMode >= HI_UNF_SCI_MODE_BUTT)
    {
        HI_ERR_SCI("Invalid param, port:%d, mode:%d\n", enSciPort, enVccEnMode);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    g_SciPara[enSciPort].SciAttr.enVccEnMode = enVccEnMode;

    SCI_HAL_SetVccEnMode(enSciPort, enVccEnMode);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
#else

    return HI_ERR_SCI_NOTSUPPORT;
#endif
}

#if 0
HI_S32 SCI_SendData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    HI_S32 Ret;
    HI_U8       *pTmpBuf;
    HI_U32 TmpLen;
    HI_U32 FifoLen;
    HI_U32 SleepCnt = 0;

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_CleanBuf(enSciPort);

    g_SciPara[enSciPort].SciAttr.TxTimeout = TimeoutMs;

    if (copy_from_user((HI_VOID *)g_SciDataBuf[enSciPort].DataBuf, (HI_VOID *)pDataBuf, BufSize))
    {
        HI_ERR_SCI("copy_from_user err.\n");
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_TX);
    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_TX);

    pTmpBuf = g_SciDataBuf[enSciPort].DataBuf;
    TmpLen = BufSize;

    Ret = HI_SUCCESS;

    while (TmpLen)
    {
        /*
         * Check card exist or not every time.
         * If card cannot work now or card out, return immediately.
         */
        if (g_SciPara[enSciPort].SciState.enSciCrtState != HI_UNF_SCI_STATUS_TX)
        {
            *pDataLen = 0;
            up(&g_SciPara[enSciPort].SciSem);
            return HI_ERR_SCI_SEND_ERR;
        }

        FifoLen = SCI_HAL_GetTxFifo(enSciPort);

        while ((FifoLen < SCI_FIFO_SIZE) && TmpLen)
        {
            SCI_HAL_WriteData(enSciPort, *pTmpBuf);

            TmpLen--;
            pTmpBuf++;
            FifoLen = SCI_HAL_GetTxFifo(enSciPort);
        }

        /*
         * For some error(some comunication error or card out), try timeout..
         */
        if (SCI_FIFO_SIZE == FifoLen)
        {
            continue;
        }

        if (TmpLen)
        {
            if (SleepCnt < (g_SciPara[enSciPort].SciAttr.TxTimeout / 10))
            {
                msleep(10);
                SleepCnt++;
            }
            else
            {
                HI_ERR_SCI("send timeout\n");
                Ret = HI_ERR_SCI_SEND_ERR;
                break;
            }
        }
    }

    SCI_HAL_ClearRxFifo(enSciPort);
    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);

    /* Set ready state only if card exist and can work correctly. */
    if (g_SciPara[enSciPort].SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READY)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_READY);
    }

    *pDataLen = BufSize - TmpLen;

    up(&g_SciPara[enSciPort].SciSem);
    return Ret;
}
#else
HI_S32 SCI_SendData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    HI_S32 Ret;
    HI_U8       *pTmpBuf;
    HI_U32 u32Count = 0;
	CHECK_PORTID();
    if(BufSize >= 512)
		return HI_ERR_SCI_INVALID_PARA;
    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_CleanBuf(enSciPort);

    g_SciPara[enSciPort].SciAttr.TxTimeout = TimeoutMs;

    if (copy_from_user((HI_VOID *)g_SciSendDataBuf[enSciPort].DataBuf, (HI_VOID *)pDataBuf, BufSize))
    {
        HI_ERR_SCI("copy_from_user err.\n");
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }
    
    g_SciSendDataBuf[enSciPort].DataLen = BufSize;
    g_SciSendDataBuf[enSciPort].Read = 0;
    
    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_TX);
    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_TX);
    SCI_HAL_SetTide(enSciPort, 1, 0);

    pTmpBuf = g_SciSendDataBuf[enSciPort].DataBuf;
    for (u32Count=0; (u32Count<BufSize) && (u32Count<SCI_FIFO_SIZE); u32Count++)
    {
        SCI_HAL_WriteData(enSciPort, *pTmpBuf);
        pTmpBuf++;
        g_SciSendDataBuf[enSciPort].Read++;
    }
    SCI_HAL_ClearInt(enSciPort, SCI_INT_TXTIDE);
    SCI_HAL_EnableInt(enSciPort, SCI_INT_TXTIDE);

    if (g_SciSendDataBuf[enSciPort].Read < g_SciSendDataBuf[enSciPort].DataLen)
    {
        Ret = wait_event_interruptible_timeout(g_SciPara[enSciPort].SciRecWaitQueue,
                                           ((g_SciSendDataBuf[enSciPort].Read == g_SciSendDataBuf[enSciPort].DataLen)
                                            || (g_SciPara[enSciPort].SciState.enSciCrtState != HI_UNF_SCI_STATUS_TX)),
                                            g_SciPara[enSciPort].SciAttr.TxTimeout * HZ / 1000); 
    }

    if ((g_SciSendDataBuf[enSciPort].Read == g_SciSendDataBuf[enSciPort].DataLen)
        && (g_SciPara[enSciPort].SciState.enSciCrtState == HI_UNF_SCI_STATUS_TX))
    {
        Ret = HI_SUCCESS;
    }
    else
    {
        Ret = HI_ERR_SCI_SEND_ERR;
    }

    SCI_HAL_DisableInt(enSciPort, SCI_INT_TXTIDE);
    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);

    /* Set ready state only if card exist and can work correctly. */
    if (g_SciPara[enSciPort].SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READY)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_READY);
    }

    *pDataLen = g_SciSendDataBuf[enSciPort].Read;
    
    up(&g_SciPara[enSciPort].SciSem);
    return Ret;
}
#endif


HI_S32 SCI_SendPPSData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    HI_S32 Ret;
    HI_U8       *pTmpBuf;
    HI_U32 TmpLen;
    HI_U32 FifoLen;
    HI_U32 SleepCnt = 0;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_CleanBuf(enSciPort);

    g_SciPara[enSciPort].SciAttr.TxTimeout = TimeoutMs;

    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_TX);
    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_TX);

    pTmpBuf = pDataBuf;
    TmpLen = BufSize;

    Ret = HI_SUCCESS;

    while (TmpLen)
    {
        /*
         * Check card exist or not every time.
         * If card cannot work now or card out, return immediately.
         */
        if (g_SciPara[enSciPort].SciState.enSciCrtState != HI_UNF_SCI_STATUS_TX)
        {
            *pDataLen = 0;
            up(&g_SciPara[enSciPort].SciSem);
            return HI_ERR_SCI_SEND_ERR;
        }

        FifoLen = SCI_HAL_GetTxFifo(enSciPort);

        while ((FifoLen < SCI_FIFO_SIZE) && TmpLen)
        {
            if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
            {
                *pDataLen = 0;
                up(&g_SciPara[enSciPort].SciSem);
                return HI_ERR_SCI_SEND_ERR;
            }

            SCI_HAL_WriteData(enSciPort, *pTmpBuf);

            TmpLen--;
            pTmpBuf++;
            FifoLen = SCI_HAL_GetTxFifo(enSciPort);
        }

        /*
         * For some error(some comunication error or card out), try timeout..
         */
        if (SCI_FIFO_SIZE == FifoLen)
        {
            continue;
        }

        if (TmpLen)
        {
            if (SleepCnt < (g_SciPara[enSciPort].SciAttr.TxTimeout / 10))
            {
                msleep(10);
                SleepCnt++;
            }
            else
            {
                HI_ERR_SCI("send timeout\n");
                Ret = HI_ERR_SCI_SEND_ERR;
                break;
            }
        }
    }

    SCI_HAL_ClearRxFifo(enSciPort);
    SCI_HAL_SetDirection(enSciPort, SCI_DIRECTION_RX);

    /* Set ready state only if card exist and can work correctly. */
    if (g_SciPara[enSciPort].SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READY)
    {
        SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_READY);
    }

    *pDataLen = BufSize - TmpLen;

    up(&g_SciPara[enSciPort].SciSem);
    return Ret;
}

HI_S32 SCI_ReceiveData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    HI_S32 Ret;
    HI_SIZE_T IrqFlags;
    HI_U32 DataLen;
    HI_U8       *pTmpBuf;
    HI_U8 u8Erro = 0;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI %d stat is not ready.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    g_SciPara[enSciPort].SciAttr.RxTimeout = TimeoutMs;

    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_RX);

    g_SciDataBuf[enSciPort].DataLen = BufSize;

    spin_lock_irqsave(&sci_lock, IrqFlags);

    if (g_SciDataBuf[enSciPort].Write >= g_SciDataBuf[enSciPort].Read)
    {
        DataLen = g_SciDataBuf[enSciPort].Write - g_SciDataBuf[enSciPort].Read;
    }
    else
    {
        DataLen = SCI_READ_BUF_LEN - g_SciDataBuf[enSciPort].Read + g_SciDataBuf[enSciPort].Write;
    }

    spin_unlock_irqrestore(&sci_lock, IrqFlags);

    if (DataLen < BufSize)
    {
        if (!g_SciPara[enSciPort].SciAttr.RxTimeout)
        {
            u8Erro = 1;
            goto REC_OUT;
        }

        g_SciPara[enSciPort].bDataEnough = HI_FALSE;

        Ret = wait_event_interruptible_timeout(g_SciPara[enSciPort].SciRecWaitQueue,
                                               (g_SciPara[enSciPort].bDataEnough
                                                || (g_SciPara[enSciPort].SciState.enSciCrtState
                                                    == HI_UNF_SCI_STATUS_NOCARD)
                                                || (g_SciPara[enSciPort].ErrType != 0)),
                                               g_SciPara[enSciPort].SciAttr.RxTimeout * HZ / 1000);

        if (!Ret)
        {
            u8Erro = 2;
            HI_ERR_SCI("read timeout\n");
            goto REC_OUT;
        }

        if (g_SciPara[enSciPort].SciState.enSciCrtState == HI_UNF_SCI_STATUS_NOCARD)
        {
            u8Erro = 3;
            goto REC_OUT;
        }

        if (g_SciPara[enSciPort].ErrType != 0)
        {
            u8Erro = 4;
            HI_ERR_SCI("error irq.\n");
            g_SciPara[enSciPort].ErrType = 0;
            goto REC_OUT;
        }
    }

REC_OUT:

    spin_lock_irqsave(&sci_lock, IrqFlags);

    if (g_SciDataBuf[enSciPort].Write >= g_SciDataBuf[enSciPort].Read)
    {
        DataLen = g_SciDataBuf[enSciPort].Write - g_SciDataBuf[enSciPort].Read;
    }
    else
    {
        DataLen = SCI_READ_BUF_LEN - g_SciDataBuf[enSciPort].Read + g_SciDataBuf[enSciPort].Write;
    }

    spin_unlock_irqrestore(&sci_lock, IrqFlags);

    pTmpBuf = &(g_SciDataBuf[enSciPort].DataBuf[g_SciDataBuf[enSciPort].Read]);

    if (DataLen < BufSize)
    {
        *pDataLen = DataLen;

        if ((u8Erro == 2) || (u8Erro == 4))
        {
            Ret = HI_ERR_SCI_RECEIVE_ERR;
        }
        else
        {
            Ret = HI_ERR_SCI_INVALID_OPT;
        }

        HI_ERR_SCI("data %d is less than need %d.\n", DataLen, BufSize);
    }
    else
    {
        *pDataLen = BufSize;
        Ret = HI_SUCCESS;
    }

    if (DataLen)
    {
        if (copy_to_user(pDataBuf, pTmpBuf, *pDataLen))
        {
            HI_ERR_SCI("copy_to_user err.\n");
            *pDataLen = 0;
            up(&g_SciPara[enSciPort].SciSem);
            return HI_ERR_SCI_INVALID_OPT;
        }

        spin_lock_irqsave(&sci_lock, IrqFlags);

        g_SciDataBuf[enSciPort].Read = (g_SciDataBuf[enSciPort].Read + *pDataLen) % SCI_READ_BUF_LEN;

        g_SciDataBuf[enSciPort].DataLen = 0;

        spin_unlock_irqrestore(&sci_lock, IrqFlags);
    }

    up(&g_SciPara[enSciPort].SciSem);
    return Ret;
}

HI_S32 SCI_ReceivePPSData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen,
                          HI_U32 TimeoutMs)
{
    HI_S32 Ret;
    HI_SIZE_T IrqFlags;
    HI_U32 DataLen;
    HI_U8       *pTmpBuf;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI %d stat is not ready.\n", enSciPort);
        *pDataLen = 0;
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_RECEIVE_ERR;
    }

    g_SciPara[enSciPort].SciAttr.RxTimeout = TimeoutMs;

    SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_RX);

    g_SciDataBuf[enSciPort].DataLen = BufSize;

    spin_lock_irqsave(&sci_lock, IrqFlags);

    if (g_SciDataBuf[enSciPort].Write >= g_SciDataBuf[enSciPort].Read)
    {
        DataLen = g_SciDataBuf[enSciPort].Write - g_SciDataBuf[enSciPort].Read;
    }
    else
    {
        DataLen = SCI_READ_BUF_LEN - g_SciDataBuf[enSciPort].Read + g_SciDataBuf[enSciPort].Write;
    }

    spin_unlock_irqrestore(&sci_lock, IrqFlags);

    if (DataLen < BufSize)
    {
        if (!g_SciPara[enSciPort].SciAttr.RxTimeout)
        {
            goto REC_OUT;
        }

        g_SciPara[enSciPort].bDataEnough = HI_FALSE;

        Ret = wait_event_interruptible_timeout(g_SciPara[enSciPort].SciRecWaitQueue,
                                               (g_SciPara[enSciPort].bDataEnough
                                                || (g_SciPara[enSciPort].SciState.enSciCrtState
                                                    == HI_UNF_SCI_STATUS_NOCARD)
                                                || (g_SciPara[enSciPort].ErrType != 0)),
                                               g_SciPara[enSciPort].SciAttr.RxTimeout * HZ / 1000);

        if (!Ret)
        {
            HI_ERR_SCI("read timeout\n");
            goto REC_OUT;
        }

        if (g_SciPara[enSciPort].SciState.enSciCrtState == HI_UNF_SCI_STATUS_NOCARD)
        {
            goto REC_OUT;
        }

        if (g_SciPara[enSciPort].ErrType != 0)
        {
            HI_ERR_SCI("error irq.\n");
            g_SciPara[enSciPort].ErrType = 0;
            goto REC_OUT;
        }
    }

REC_OUT:

    spin_lock_irqsave(&sci_lock, IrqFlags);

    if (g_SciDataBuf[enSciPort].Write >= g_SciDataBuf[enSciPort].Read)
    {
        DataLen = g_SciDataBuf[enSciPort].Write - g_SciDataBuf[enSciPort].Read;
    }
    else
    {
        DataLen = SCI_READ_BUF_LEN - g_SciDataBuf[enSciPort].Read + g_SciDataBuf[enSciPort].Write;
    }

    spin_unlock_irqrestore(&sci_lock, IrqFlags);

    pTmpBuf = &(g_SciDataBuf[enSciPort].DataBuf[g_SciDataBuf[enSciPort].Read]);

    if (DataLen < BufSize)
    {
        *pDataLen = DataLen;
        Ret = HI_ERR_SCI_RECEIVE_ERR;
        HI_ERR_SCI("data %d is less than need %d.\n", DataLen, BufSize);
    }
    else
    {
        *pDataLen = BufSize;
        Ret = HI_SUCCESS;
    }

    if (DataLen)
    {
        memcpy(pDataBuf, pTmpBuf, *pDataLen);
        spin_lock_irqsave(&sci_lock, IrqFlags);

        g_SciDataBuf[enSciPort].Read = (g_SciDataBuf[enSciPort].Read + *pDataLen) % SCI_READ_BUF_LEN;

        g_SciDataBuf[enSciPort].DataLen = 0;

        spin_unlock_irqrestore(&sci_lock, IrqFlags);
    }

    up(&g_SciPara[enSciPort].SciSem);
    return Ret;
}

HI_S32 SCI_Switch(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    SCI_HAL_SetSCIReset(enSciPort);
    SCI_HAL_ClearSCIReset(enSciPort);

    g_SciPara[enSciPort].SciAttr.enSciProtocol = enSciProtocol;
    g_SciPara[enSciPort].SciAttr.Frequency = Frequency;

    g_SciPara[enSciPort].SciState.enSciProtocol = enSciProtocol;

    up(&g_SciPara[enSciPort].SciSem);

    Ret = SCI_Reset(enSciPort, HI_FALSE);

    return Ret;
}

HI_S32 SCI_ExtSetBaud( HI_UNF_SCI_PORT_E enSciPort, HI_U32 ClkRate, HI_U32 BitRate)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI %d is not opened.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI%d stat is not ready\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_RECEIVE_ERR;
    }

    g_SciPara[enSciPort].SciState.SciSetupPara.ClkRate = ClkRate;
    g_SciPara[enSciPort].SciState.SciSetupPara.BitRate = BitRate;
    SCI_CalcEtuBaud(enSciPort, ClkRate, BitRate);
    g_SciPara[enSciPort].SciState.bSetExtBaudFlag = HI_TRUE;

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_SetCharGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 ExtChGuard)
{
    HI_S32 Ret;
    HI_U32 CharGuard = 0;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI%d is not open.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI%d is not ready\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_RECEIVE_ERR;
    }

    if (255 == ExtChGuard)
    {
        if (HI_UNF_SCI_PROTOCOL_T0 == g_SciPara[enSciPort].SciState.enSciProtocol)
        {
            CharGuard = 0;
        }
        else
        {
            CharGuard = 0;
        }
    }
    else if (ExtChGuard < 255)
    {
        if (HI_UNF_SCI_PROTOCOL_T1 == g_SciPara[enSciPort].SciState.enSciProtocol)
        {
            CharGuard = ExtChGuard + 1;
        }
        else
        {
            CharGuard = ExtChGuard;
        }
    }

    g_SciPara[enSciPort].SciState.SciArtPara.ChGuard = CharGuard;
    SCI_HAL_SetCharGuard(enSciPort, CharGuard);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 CharTimeouts)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI%d has not open.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI%d has not ready\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NO_ATR;
    }

    if (enSciProtocol != g_SciPara[enSciPort].SciState.enSciProtocol)
    {
        HI_ERR_SCI("SCI%d :Expect according to protocol set chartimeout conflict with current card protocol\n",
                   enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    g_SciPara[enSciPort].SciState.SciSetupPara.CharTimeout = CharTimeouts;
    SCI_HAL_SetCharTimeout( enSciPort, CharTimeouts);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockTimeouts)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI%d has not open.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI%d has not ready\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NO_ATR;
    }

    if (HI_UNF_SCI_PROTOCOL_T1 != g_SciPara[enSciPort].SciState.enSciProtocol)
    {
        HI_ERR_SCI("SCI%d : set BlockTimeout noly for T1 protocol\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_PARA;
    }

    g_SciPara[enSciPort].SciState.SciSetupPara.BlockTimeout = BlockTimeouts;
    SCI_HAL_SetBlockTimeout( enSciPort, BlockTimeouts);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_S32 SCI_SetTxretry(HI_UNF_SCI_PORT_E enSciPort, HI_U32 TxRetryTimes)
{
    HI_S32 Ret;
	CHECK_PORTID();

    Ret = down_interruptible(&g_SciPara[enSciPort].SciSem);

    if (!g_SciPara[enSciPort].bSciEnable)
    {
        HI_ERR_SCI("SCI%d has not open.\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_INVALID_OPT;
    }

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("SCI%d has not ready\n", enSciPort);
        up(&g_SciPara[enSciPort].SciSem);
        return HI_ERR_SCI_NO_ATR;
    }

    g_SciPara[enSciPort].SciState.SciSetupPara.TxRetries = TxRetryTimes;
    SCI_HAL_SetRetry( enSciPort, g_SciPara[enSciPort].SciState.SciSetupPara.RxRetries, TxRetryTimes);

    up(&g_SciPara[enSciPort].SciSem);
    return HI_SUCCESS;
}

HI_VOID SCI_ReadTasklet(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_U8 SciData;
    unsigned long flags;

    if (g_SciPara[enSciPort].SciState.enSciCrtState < HI_UNF_SCI_STATUS_READY)
    {
        do
        {
            spin_lock_irqsave(&sci_lock, flags);
            if (g_SciDataBuf[enSciPort].Write == g_SciDataBuf[enSciPort].Read)
            {
                spin_unlock_irqrestore(&sci_lock, flags);
                return;
            }

            spin_unlock_irqrestore(&sci_lock, flags);

            SciData = g_SciDataBuf[enSciPort].DataBuf[g_SciDataBuf[enSciPort].Read];

            if (0x3b == SciData)
            {
                HI_INFO_SCI("recv atr data 0x3b\n");
            }

            g_SciDataBuf[enSciPort].Read = (g_SciDataBuf[enSciPort].Read + 1) % SCI_READ_BUF_LEN;
        } while (0 == SCI_AtrProcess(enSciPort, SciData));

        if (SCI_HAL_GetCardPresent(enSciPort))
        {
            SCI_HAL_Finish(enSciPort);
            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
        }
    }

    return;
}

irqreturn_t SCI_Isr(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 IntState;
    HI_UNF_SCI_PORT_E enSciPort = HI_UNF_SCI_PORT0;
    HI_U8 ii = 0;
    SCI_PORT_ATTR_S stPortAttr;

    for (ii = HI_UNF_SCI_PORT0; ii < HI_SCI_PORT_NUM; ii++)
    {
        if (HI_SUCCESS != SCI_GetPortAttr(ii, &stPortAttr))
        {
            return IRQ_HANDLED;
        }

        if (irq == stPortAttr.u32IrqNo)
        {
            enSciPort = ii;
            break;
        }
    }

    IntState = SCI_HAL_GetIntState(enSciPort);

    while (IntState)
    {
        if (IntState & SCI_INT_CARDIN)
        {
            HI_INFO_SCI("sci%d card in\n", enSciPort);
            SCI_HAL_ClearInt(enSciPort, SCI_INT_CARDIN);
            SCI_CardIn(enSciPort);
        }

        if (IntState & SCI_INT_CARDOUT)
        {
            HI_INFO_SCI("sci%d card out\n", enSciPort);
            SCI_HAL_ClearInt(enSciPort, SCI_INT_CARDOUT);
            SCI_CardOut(enSciPort);
        }

        if (IntState & SCI_INT_CARDDOWN)
        {
            HI_INFO_SCI("sci%d card down\n", enSciPort);
            SCI_HAL_ClearInt(enSciPort, SCI_INT_CARDDOWN);
            SCI_CardDown(enSciPort);
        }

        if (IntState & SCI_INT_CARDUP)
        {
            HI_INFO_SCI("sci%d card up\n", enSciPort);

            SCI_HAL_ClearInt(enSciPort, SCI_INT_CARDUP);

            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_WAITATR);
        }

        if (IntState & SCI_INT_RXTIDE)
        {
            SCI_HAL_ClearInt(enSciPort, SCI_INT_RXTIDE);
            SCI_RxTide(enSciPort);
        }

        if (IntState & SCI_INT_ATRSTOUT)
        {
#if defined(HI_ADVCA_TYPE_CONAX)
        	SCI_HAL_Finish(enSciPort);
#endif
            SCI_HAL_ClearInt(enSciPort, SCI_INT_ATRSTOUT);
            HI_ERR_SCI("sci%d ATR start timeout.\n", enSciPort);
            SCI_SetState(enSciPort, HI_UNF_SCI_STATUS_INACTIVECARD);
        }

        if (IntState & SCI_INT_ATRDTOUT)
        {
            SCI_HAL_ClearInt(enSciPort, SCI_INT_ATRDTOUT);
            HI_ERR_SCI("sci%d ATR duration timeout.\n", enSciPort);
        }

        if (IntState & SCI_INT_CHTOUT)
        {
            SCI_HAL_ClearInt(enSciPort, SCI_INT_CHTOUT);
            HI_ERR_SCI("sci%d character transfer timeout.\n", enSciPort);
        }

        if (IntState & SCI_INT_RORI)
        {
            SCI_HAL_ClearInt(enSciPort, SCI_INT_RORI);
            HI_ERR_SCI("sci%d receive fifo overflow\n", enSciPort);
            g_SciPara[enSciPort].ErrType = SCI_INT_RORI;
            wake_up_interruptible(&g_SciPara[enSciPort].SciRecWaitQueue);
        }

        if (IntState & SCI_INT_TXTIDE)
        {
            HI_INFO_SCI("sci%d fifo overflow\n", enSciPort);
            SCI_PushData(enSciPort);
            SCI_HAL_ClearInt(enSciPort, SCI_INT_TXTIDE);
        }

        IntState = SCI_HAL_GetIntState(enSciPort);
    }

    return IRQ_HANDLED;
}

HI_S32 SCI_Suspend(HI_VOID)
{
    HI_U32 index;

    for (index = HI_UNF_SCI_PORT0; index < HI_SCI_PORT_NUM; index++)
    {
        if (g_SciPara[index].bSciEnable)
        {
            SusForceFlag[index] = g_SciPara[index].SciState.bForceFlag;
            SusSetExtBaudFlag[index] = g_SciPara[index].SciState.bSetExtBaudFlag;
            SusSciProtocol[index] = g_SciPara[index].SciAttr.enSciProtocol;
            SusClkRate[index]  = g_SciPara[index].SciState.SciSetupPara.ClkRate;
            SusBitRate[index]  = g_SciPara[index].SciState.SciSetupPara.BitRate;
            SusSciState[index] = g_SciPara[index].SciState.enSciCrtState;

            SCI_HAL_DisableAllInt(index);
            SCI_HAL_Finish(index);
            SCI_SetState(index, HI_UNF_SCI_STATUS_INACTIVECARD);
        }
    }

    return HI_SUCCESS;
}

HI_S32 SCI_Resume(HI_VOID)
{
    HI_U32 index, times, Flag = HI_FALSE;
    HI_S32 Ret;

    for (index = HI_UNF_SCI_PORT0; index < HI_SCI_PORT_NUM; index++)
    {
        if (g_SciPara[index].bSciEnable)
        {
            SCI_HAL_SetSCIClock(index, HI_TRUE);
            SCI_HAL_ClearSCIReset(index);

            SCI_ConfClkMode( index, g_SciPara[index].SciAttr.enClkMode);
            SCI_ConfResetMode( index, g_SciPara[index].SciAttr.enResetMode);
            SCI_ConfVccEnMode( index, g_SciPara[index].SciAttr.enVccEnMode);

            SCI_ConfVcc( index, g_SciPara[index].SciAttr.enSciVcc);
            SCI_ConfDetect( index, g_SciPara[index].SciAttr.enSciDetect);

            g_SciPara[index].SciState.enSciCrtState = SusSciState[index];
            g_SciPara[index].SciAttr.enSciProtocol = SusSciProtocol[index];

            Ret = SCI_GetStatus( index, &g_SciPara[index].SciState.enSciCrtState);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_SCI("Resume SCI_GetStatus Fail !\n");
                return HI_FAILURE;
            }

            HI_FATAL_SCI("SCI%d  state before Resume reset is %d !\n", index, g_SciPara[index].SciState.enSciCrtState);

            if (g_SciPara[index].SciState.enSciCrtState <= HI_UNF_SCI_STATUS_NOCARD)
            {
                SCI_HAL_DisableAllInt(index);
    			SCI_HAL_ClearAllInt(index);
            	SCI_HAL_EnableInt(index, SCI_DFT_INT);
                HI_FATAL_SCI("Not inset Card, don't  Resume sci!\n");
                return HI_FAILURE;
            }

            Ret = SCI_Reset(index, HI_FALSE);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_SCI("Resume SCI_Reset Fail !\n");
                return HI_FAILURE;
            }

            for (times = 0; times < 60; times++)
            {
                if (g_SciPara[index].SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READY)
                {
                    Flag = HI_TRUE;
                    break;
                }

                mdelay(50);

                HI_FATAL_SCI("SCI Resume reset count %d!\n", times);
            }

            if (Flag)
            {
                HI_FATAL_SCI("SCI Resume cold Reset OK!\n");

                g_SciPara[index].SciState.bForceFlag = SusForceFlag[index];
                g_SciPara[index].SciState.bSetExtBaudFlag = SusSetExtBaudFlag[index];
                g_SciPara[index].SciState.SciSetupPara.ClkRate = SusClkRate[index];
                g_SciPara[index].SciState.SciSetupPara.BitRate = SusBitRate[index];
                SCI_CalcEtuBaud( index, g_SciPara[index].SciState.SciSetupPara.ClkRate,
                                 g_SciPara[index].SciState.SciSetupPara.BitRate);
            }
            else
            {
                HI_FATAL_SCI("SCI Resume cold Reset Fail!\n");
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SCI_Reset(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmReset)
{
    return SCI_Reset(enSciPort, bWarmReset);
}

HI_S32 HI_DRV_SCI_SendData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    if ((HI_NULL == pDataBuf) || (HI_NULL == pDataLen))
    {
        return HI_ERR_SCI_INVALID_PARA;
    }

    return SCI_SendData(enSciPort, pDataBuf, BufSize, pDataLen, TimeoutMs);
}

HI_S32 HI_DRV_SCI_ReceiveData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs)
{
    if ((HI_NULL == pDataBuf) || (HI_NULL == pDataLen))
    {
        return HI_ERR_SCI_INVALID_PARA;
    }

    return SCI_ReceiveData(enSciPort, pDataBuf, BufSize, pDataLen, TimeoutMs);
}

HI_S32 HI_DRV_SCI_ConfVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel)
{
    return SCI_ConfVcc(enSciPort, enSciLevel);
}

HI_S32 HI_DRV_SCI_ConfDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel)
{
    return SCI_ConfDetect(enSciPort, enSciLevel);
}

HI_S32 HI_DRV_SCI_ConfClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode)
{
    return SCI_ConfClkMode(enSciPort, enClkMode);
}

HI_S32 HI_DRV_SCI_GetSciPara(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pSciPara)
{
    if (HI_NULL == pSciPara)
    {
        return HI_ERR_SCI_INVALID_PARA;
    }

    return SCI_OutPutParam(enSciPort, pSciPara);
}

HI_S32 HI_DRV_SCI_GetSciAtrBuf(HI_UNF_SCI_PORT_E enSciPort, HI_DRV_ATR_BUF_S  *pSciAtrBuf)
{
    if (HI_NULL == pSciAtrBuf)
    {
        return HI_ERR_SCI_INVALID_PARA;
    }

    return SCI_GetSciAtrBuf(enSciPort, (ATR_BUF_S *)pSciAtrBuf);
}

HI_S32 HI_DRV_SCI_Deactive(HI_UNF_SCI_PORT_E enSciPort)
{
    return SCI_Deactive(enSciPort);
}

HI_S32 HI_DRV_SCI_GetStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penState)
{
    if (HI_NULL == penState)
    {
        return HI_ERR_SCI_INVALID_PARA;
    }

    return SCI_GetStatus(enSciPort, penState);
}

HI_S32 HI_DRV_SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency)
{
    return SCI_Open(enSciPort, enSciProtocol, Frequency);
}

HI_S32 HI_DRV_SCI_Close(HI_UNF_SCI_PORT_E enSciPort)
{
    return SCI_Close(enSciPort);
}

HI_VOID HI_DRV_SCI_Init(HI_VOID)
{
    return SCI_Init();
}

EXPORT_SYMBOL(HI_DRV_SCI_Reset);
EXPORT_SYMBOL(HI_DRV_SCI_SendData);
EXPORT_SYMBOL(HI_DRV_SCI_ReceiveData);
EXPORT_SYMBOL(HI_DRV_SCI_ConfVcc);
EXPORT_SYMBOL(HI_DRV_SCI_ConfDetect);
EXPORT_SYMBOL(HI_DRV_SCI_ConfClkMode);	
EXPORT_SYMBOL(HI_DRV_SCI_GetSciPara);
EXPORT_SYMBOL(HI_DRV_SCI_GetSciAtrBuf);
EXPORT_SYMBOL(HI_DRV_SCI_Deactive);
EXPORT_SYMBOL(HI_DRV_SCI_GetStatus);
EXPORT_SYMBOL(HI_DRV_SCI_Open);
EXPORT_SYMBOL(HI_DRV_SCI_Close);
EXPORT_SYMBOL(HI_DRV_SCI_Init);

