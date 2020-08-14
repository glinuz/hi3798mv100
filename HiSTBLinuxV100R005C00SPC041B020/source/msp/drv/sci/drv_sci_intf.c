#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/compat.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "drv_sci.h"
#include "hi_kernel_adapt.h"

#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_osal.h"
#include "drv_sci_ext.h"

#define PROC_NAME_LEN (8)

static UMAP_DEVICE_S g_SciRegisterData;
static atomic_t g_SciCount = ATOMIC_INIT(0);
HI_DECLARE_MUTEX(g_SciMutex);

static HI_U32 g_SciOpenNum[HI_UNF_SCI_PORT_BUTT];

extern HI_U32 SCI_ClkRate[];  /* {372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048, 0, 0}*/
extern HI_U32 SCI_BitRate[];  /* {0, 1, 2, 4, 8, 16, 32, 0, 12, 20, 0, 0, 0, 0, 0, 0} */


static HI_S32 sci_pm_suspend (PM_BASEDEV_S *pdev, pm_message_t state);
static HI_S32 sci_pm_resume (PM_BASEDEV_S *pdev);

static SCI_EXT_FUNC_S g_stSciExtFuncs =
{
    .pfnSciSuspend      = sci_pm_suspend,
    .pfnSciResume       = sci_pm_resume,
};

#if (1 == HI_PROC_SUPPORT)

static HI_CHAR g_CardStateString[][20] = {"UNINIT",
                                          "FIRSTINIT",
                                          "NOCARD",
                                          "INACTIVECARD",
                                          "WAITATR",
                                          "READATR",
                                          "READY",
                                          "RX",
                                          "TX"};
static HI_CHAR g_ProtocolString[][5] = {"T=0", "T=1", "T=14"};
static HI_CHAR g_ClockModeString[][5] = {"CMOS", "OD"};

HI_VOID SCI_DspProc(struct seq_file *p, SCI_PARA_S *pSciPara, ATR_BUF_S *pSciAtrBuf, HI_U32 GetAtrRet)
{
    HI_U8 AtrMask = 0x0, AtrFi = 0x1, AtrDi = 0x1;
    HI_U32 AtrNum = 0, ActualSciClk;

    PROC_PRINT(p, "%-20s:", "ATR");
    for (AtrNum = 0; AtrNum <= pSciAtrBuf->DataLen - 1; AtrNum++)
    {
        PROC_PRINT(p, "0x%x ", pSciAtrBuf->DataBuf[AtrNum]);
    }

    AtrNum = 0;
    PROC_PRINT(p, "\n%-20s:0x%x\n", "TS", pSciAtrBuf->DataBuf[AtrNum++]);
    PROC_PRINT(p, "%-20s:0x%x\n", "T0", pSciAtrBuf->DataBuf[AtrNum]);

    /*interface byte*/
    AtrMask = (pSciAtrBuf->DataBuf[AtrNum] & 0xf0) >> 3;
    if ((AtrMask >> 1) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x", "TA1", pSciAtrBuf->DataBuf[++AtrNum]);
        AtrDi = pSciAtrBuf->DataBuf[AtrNum] & 0x0f;
        AtrFi = (pSciAtrBuf->DataBuf[AtrNum] & 0xf0) >> 4;
        PROC_PRINT(p, " (FI = %u,DI = %u)\n", SCI_ClkRate[AtrFi], SCI_BitRate[AtrDi]);
    }

    if ((AtrMask >> 2) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x\n", "TB1", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 3) & 0x01)
    {
        ++AtrNum;
        PROC_PRINT(p, "%-20s:0x%x (N = %d)\n", "TC1", pSciAtrBuf->DataBuf[AtrNum], pSciAtrBuf->DataBuf[AtrNum]);
    }

    if ((AtrMask >> 4) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x\n", "TD1", pSciAtrBuf->DataBuf[++AtrNum]);
        AtrMask = (pSciAtrBuf->DataBuf[AtrNum] & 0xf0) >> 3;
    }
    else
    {
        AtrMask = 0x0;
    }

    if ((AtrMask >> 1) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x (present indicate is specific mode)\n", "TA2", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 2) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x\n", "TB2", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 3) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x (noly for T0 show IC card max char timeout)\n", "TC2",
                        pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 4) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x\n", "TD2", pSciAtrBuf->DataBuf[++AtrNum]);
        AtrMask = (pSciAtrBuf->DataBuf[AtrNum] & 0xf0) >> 3;
    }
    else
    {
        AtrMask = 0x0;
    }

    if ((AtrMask >> 1) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x (INF length)\n", "TA3", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 2) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x (b5¡«b8:BWI  b1¡«b4:CWI)\n", "TB3", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    if ((AtrMask >> 3) & 0x01)
    {
        PROC_PRINT(p, "%-20s:0x%x\n", "TC3", pSciAtrBuf->DataBuf[++AtrNum]);
    }

    /*History Byte*/
    ++AtrNum;
    if (HI_UNF_SCI_PROTOCOL_T0 == pSciPara->SciAttr.enSciProtocol)
    {
        PROC_PRINT(p, "%-20s:", "HistoryByte");
        while (AtrNum <= pSciAtrBuf->DataLen - 1)
        {
            PROC_PRINT(p, "0x%x ", pSciAtrBuf->DataBuf[AtrNum++]);
        }

        PROC_PRINT(p, "\n");
    }
    else
    {
        PROC_PRINT(p, "%-20s:", "HistoryByte");
        while (AtrNum <= pSciAtrBuf->DataLen - 2)
        {
            PROC_PRINT(p, " 0x%x", pSciAtrBuf->DataBuf[AtrNum++]);
        }

        PROC_PRINT(p, "\n%-20s:0x%x\n", "TCK", pSciAtrBuf->DataBuf[AtrNum]);
    }

    ActualSciClk = pSciPara->u32SysClk / (pSciPara->SciState.SciSetupPara.ClockIcc + 1) / 2;

    PROC_PRINT(p, "%-20s:%u\n", "ActualSciClk", ActualSciClk);
    PROC_PRINT(p, "%-20s:%u\n", "ExpectBaudRate", pSciPara->SciState.SciSetupPara.BitRate
                    * pSciPara->SciAttr.Frequency * 1000
                    / pSciPara->SciState.SciSetupPara.ClkRate);
    PROC_PRINT(p, "%-20s:%u \n", "CalcBaudFlag", pSciPara->SciState.bForceFlag);
    PROC_PRINT(p, "%-20s:%u \n", "bSetExtBaudFlag", pSciPara->SciState.bSetExtBaudFlag);
    PROC_PRINT(p, "%-20s:%u\n", "ClkRate(F)", pSciPara->SciState.SciSetupPara.ClkRate);
    PROC_PRINT(p, "%-20s:%u\n", "BitRate(D)", pSciPara->SciState.SciSetupPara.BitRate);
    PROC_PRINT(p, "%-20s:%u\n", "BaudRate", pSciPara->SciState.SciSetupPara.BitRate * ActualSciClk * 1000 \
                    / pSciPara->SciState.SciSetupPara.ClkRate);
    PROC_PRINT(p, "%-20s:%u etu\n", "AddCharGuard", pSciPara->SciState.SciArtPara.ChGuard);
    PROC_PRINT(p, "%-20s:%u etu\n", "BlockGuard", pSciPara->SciState.SciSetupPara.BlockGuard);
    PROC_PRINT(p, "%-20s:%u \n", "Value", pSciPara->SciState.SciSetupPara.EtuValue);
    PROC_PRINT(p, "%-20s:%u \n", "Baud", pSciPara->SciState.SciSetupPara.BaudValue);
    if (HI_UNF_SCI_PROTOCOL_T1 == pSciPara->SciAttr.enSciProtocol)
    {
        PROC_PRINT(p, "%-20s:%u \n", "CharTimeout", pSciPara->SciState.SciSetupPara.CharTimeout);
        PROC_PRINT(p, "%-20s:%u \n", "BlockTimeout", pSciPara->SciState.SciSetupPara.BlockTimeout);
    }
    else
    {
        PROC_PRINT(p, "%-20s:%u \n", "CharTimeout", pSciPara->SciState.SciSetupPara.CharTimeout);
    }

    return;
}

HI_S32 SCI_ProcRead(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S    *pProcItem;
    HI_UNF_SCI_PORT_E enSciPort = HI_UNF_SCI_PORT0;
    HI_S32 Ret;
    SCI_PARA_S SciPara;
    ATR_BUF_S SciAtrBuf;
    HI_U8 ii = 0;
    HI_CHAR s8Buff[PROC_NAME_LEN];

    pProcItem = p->private;

    for (ii = HI_UNF_SCI_PORT0; ii < HI_SCI_PORT_NUM; ii++)
    {
        memset(s8Buff, 0, sizeof(s8Buff));
        HI_OSAL_Snprintf(s8Buff, PROC_NAME_LEN, "sci%d", ii);

        if (0 == strncmp(pProcItem->entry_name, s8Buff, PROC_NAME_LEN))
        {
            enSciPort = ii;
            break;
        }
    }

    PROC_PRINT(p, "---------Hisilicon SCI%d Info---------\n", enSciPort);

    if (0 != g_SciOpenNum[enSciPort])
    {
        Ret = SCI_GetSciPara(enSciPort, &SciPara);
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_SCI("SCI_ProcRead SCI%u SCI_GetSciPara failed.\n", enSciPort);
            return Ret;
        }

        PROC_PRINT(p, "%-20s:%s\n", "Sci State", g_CardStateString[SciPara.SciState.enSciCrtState]);
        PROC_PRINT(p, "%-20s:%d\n", "SetFrequency", SciPara.SciAttr.Frequency);
        PROC_PRINT(p, "%-20s:%s\n", "Protocol", g_ProtocolString[SciPara.SciAttr.enSciProtocol]);
        PROC_PRINT(p, "%-20s:%d\n", "VccEnLevel", SciPara.SciAttr.enSciVcc);
        PROC_PRINT(p, "%-20s:%d\n", "DetectLevel", SciPara.SciAttr.enSciDetect);
        PROC_PRINT(p, "%-20s:%s\n", "ClockMode", g_ClockModeString[SciPara.SciAttr.enClkMode]);
    PROC_PRINT(p, "%-20s:%s\n", "ResetMode", g_ClockModeString[SciPara.SciAttr.enResetMode]);
        PROC_PRINT(p, "%-20s:%s\n", "VccMode", g_ClockModeString[SciPara.SciAttr.enVccEnMode]);

        if (SciPara.SciState.enSciCrtState >= HI_UNF_SCI_STATUS_READATR)
        {
            Ret = SCI_GetSciAtrBuf(enSciPort, &SciAtrBuf);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_SCI("SCI_ProcRead SCI%u SCI_GetSciAtrBuf failed.\n", enSciPort);
                return Ret;
            }

            SCI_DspProc(p, &SciPara, &SciAtrBuf, Ret);
        }

        return HI_SUCCESS;
    }
    else
    {
        PROC_PRINT(p, "SCI%d is not open\n", enSciPort);
        return HI_SUCCESS;
    }
}

#endif


HI_S32 SCI_ExtOpen(SCI_OPEN_S *pSciOpen, SCI_DEV_STATE_S *pSciDevState)
{
    HI_S32 Ret = HI_SUCCESS;

    if (pSciOpen->enSciPort < HI_SCI_PORT_NUM)
    {
        if (!pSciDevState->bSci[pSciOpen->enSciPort])
        {
            /*first open sci0 will execute SCI_Open initialize sci0 config*/
            if (!g_SciOpenNum[pSciOpen->enSciPort])
            {
                Ret = SCI_Open(pSciOpen->enSciPort, pSciOpen->enSciProtocol, pSciOpen->Frequency);
                if (Ret != HI_SUCCESS)
                {
                    HI_FATAL_SCI("call SCI_Open failed.\n");
                    return Ret;
                }
            }

            pSciDevState->bSci[pSciOpen->enSciPort] = HI_TRUE;
            g_SciOpenNum[pSciOpen->enSciPort]++;
        }
    }
    else
    {
        Ret = HI_FAILURE;
    }

    return Ret;
}

HI_S32 SCI_ExtClose(HI_UNF_SCI_PORT_E enSciPort, SCI_DEV_STATE_S *pSciDevState)
{
    HI_S32 Ret = HI_SUCCESS;

    /* close SCI0 */
    if (enSciPort < HI_SCI_PORT_NUM)
    {
        pSciDevState->bSci[enSciPort] = HI_FALSE;
        g_SciOpenNum[enSciPort]--;

        if (!g_SciOpenNum[enSciPort])
        {
            Ret = SCI_Close(enSciPort);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_SCI("call SCI_Close failed.\n");
            }
        }
    }

    return Ret;
}

HI_S32 SCI_CheckState(HI_UNF_SCI_PORT_E enSciPort, SCI_DEV_STATE_S *pSciDevState)
{
    if ((enSciPort < HI_SCI_PORT_NUM) && pSciDevState->bSci[enSciPort])
    {
        return HI_SUCCESS;
    }

    return HI_ERR_SCI_INVALID_PARA;
}

HI_S32 SCI_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    SCI_DEV_STATE_S    *pSciDevState;
    HI_S32 Ret;

    pSciDevState = file->private_data;

    Ret = down_interruptible(&g_SciMutex);

    switch (cmd)
    {
    case CMD_SCI_OPEN:
    {
        SCI_OPEN_S     *pSciOpen;

        pSciOpen = (SCI_OPEN_S *)arg;

        Ret = SCI_ExtOpen(pSciOpen, pSciDevState);

        break;
    }

    case CMD_SCI_CLOSE:
    {
        Ret = SCI_CheckState(*((HI_UNF_SCI_PORT_E *)arg), pSciDevState);
        if (HI_SUCCESS == Ret)
        {
            Ret = SCI_ExtClose(*((HI_UNF_SCI_PORT_E *)arg), pSciDevState);
        }
        else
        {
            Ret = HI_SUCCESS;
        }

        break;
    }

    default:
        break;
    }

    up(&g_SciMutex);

    switch (cmd)
    {
    case CMD_SCI_RESET:
    {
        SCI_RESET_S  *pSciReset;

        pSciReset = (SCI_RESET_S *)arg;

        Ret = SCI_Reset(pSciReset->enSciPort, pSciReset->bWarmReset);

        break;
    }

    case CMD_SCI_DEACTIVE:
    {
        Ret = SCI_Deactive(*((HI_UNF_SCI_PORT_E *)arg));

        break;
    }

    case CMD_SCI_GET_ATR:
    {
        SCI_ATR_S  *PSciAtr;

        PSciAtr = (SCI_ATR_S *)arg;

        Ret = SCI_GetAtr(PSciAtr->enSciPort, PSciAtr->pAtrBuf, PSciAtr->BufSize, &PSciAtr->DataLen);

        break;
    }

    case CMD_SCI_GET_STATUS:
    {
        SCI_STATUS_S   *pSciStatus;

        pSciStatus = (SCI_STATUS_S *)arg;

        Ret = SCI_GetStatus(pSciStatus->enSciPort, &pSciStatus->enSciStatus);

        break;
    }

    case CMD_SCI_CONF_VCC:
    {
        SCI_LEVEL_S   *pSciLevel;

        pSciLevel = (SCI_LEVEL_S *)arg;

        Ret = SCI_ConfVcc(pSciLevel->enSciPort, pSciLevel->enSciLevel);

        break;
    }

    case CMD_SCI_CONF_DETECT:
    {
        SCI_LEVEL_S   *pSciLevel;

        pSciLevel = (SCI_LEVEL_S *)arg;

        Ret = SCI_ConfDetect(pSciLevel->enSciPort, pSciLevel->enSciLevel);

        break;
    }

    case CMD_SCI_CONF_MODE:
    {
        SCI_IO_OUTPUTTYPE_S   *pSciIO;

        pSciIO = (SCI_IO_OUTPUTTYPE_S *)arg;

        if (SCI_IO_CLK == pSciIO->enIO)
        {
            Ret = SCI_ConfClkMode(pSciIO->enSciPort, pSciIO->enOutputType);
        }
        else if (SCI_IO_RESET == pSciIO->enIO)
        {
            Ret = SCI_ConfResetMode(pSciIO->enSciPort, pSciIO->enOutputType);
        }
        else if (SCI_IO_VCC_EN == pSciIO->enIO)
        {
            Ret = SCI_ConfVccEnMode(pSciIO->enSciPort, pSciIO->enOutputType);
        }
        else
        {
            Ret = HI_ERR_SCI_NOTSUPPORT;
        }

        break;
    }

    case CMD_SCI_SEND_DATA:
    {
        SCI_DATA_S    *pSciData;

        pSciData = (SCI_DATA_S *)arg;

        Ret = SCI_SendData(pSciData->enSciPort, pSciData->pDataBuf, pSciData->BufSize, &pSciData->DataLen,
                           pSciData->TimeoutMs);

        break;
    }

    case CMD_SCI_RECEIVE_DATA:
    {
        SCI_DATA_S    *pSciData;

        pSciData = (SCI_DATA_S *)arg;

        Ret = SCI_ReceiveData(pSciData->enSciPort, pSciData->pDataBuf, pSciData->BufSize, &pSciData->DataLen,
                              pSciData->TimeoutMs);

        if (HI_ERR_SCI_RECEIVE_ERR == Ret)
        {
            Ret = HI_SUCCESS;
        }

        break;
    }

    case CMD_SCI_SWITCH:
    {
        SCI_OPEN_S     *pSciOpen;

        pSciOpen = (SCI_OPEN_S *)arg;

        Ret = SCI_Switch(pSciOpen->enSciPort, pSciOpen->enSciProtocol, pSciOpen->Frequency);

        break;
    }

    case CMD_SCI_SET_BAUD:
    {
        SCI_EXT_BAUD_S     *pSciExtBaud;

        pSciExtBaud = (SCI_EXT_BAUD_S *)arg;

        Ret = SCI_ExtSetBaud(pSciExtBaud->enSciPort, pSciExtBaud->ClkRate, pSciExtBaud->BitRate);

        break;
    }

    case CMD_SCI_SET_CHGUARD:
    {
        SCI_ADD_GUARD_S  *pSciAddGuard;
        pSciAddGuard = (SCI_ADD_GUARD_S *)arg;

        Ret = SCI_SetCharGuard(pSciAddGuard->enSciPort, pSciAddGuard->AddCharGuard);
        break;
    }

    case CMD_SCI_SEND_PPS_DATA:
    {
        SCI_PPS_S    *pSciSendPPS;

        pSciSendPPS = (SCI_PPS_S *)arg;

        Ret = SCI_PPS_Negotiation(pSciSendPPS->enSciPort, pSciSendPPS->Send, pSciSendPPS->SendLen,
                                  pSciSendPPS->RecTimeouts);

        break;
    }

    case CMD_SCI_GET_PPS_DATA:
    {
        SCI_PPS_S    *pSciRecPPS;

        pSciRecPPS = (SCI_PPS_S *)arg;

        Ret = SCI_PPS_GetRecBuf(pSciRecPPS->enSciPort, pSciRecPPS->Receive, &pSciRecPPS->ReceiveLen);

        break;
    }

    case CMD_SCI_GET_PARAM:
    {
        HI_UNF_SCI_PARAMS_S_PTR pSciOutParam;
        pSciOutParam = (HI_UNF_SCI_PARAMS_S_PTR)arg;

        Ret = SCI_OutPutParam(pSciOutParam->enSciPort, pSciOutParam);

        break;
    }

    case CMD_SCI_SET_CHARTIMEOUT:
    {
        SCI_CHARTIMEOUT_S *ChTimeout;
        ChTimeout = (SCI_CHARTIMEOUT_S *)arg;

        Ret = SCI_SetCharTimeout(ChTimeout->enSciPort, ChTimeout->enSciProtocol, ChTimeout->CharTimeouts);
        break;
    }

    case CMD_SCI_SET_BLOCKTIMEOUT:
    {
        SCI_BLOCKTIMEOUT_S *BlkTimeout;
        BlkTimeout = (SCI_BLOCKTIMEOUT_S *)arg;

        Ret = SCI_SetBlockTimeout(BlkTimeout->enSciPort, BlkTimeout->BlockTimeouts);

        break;
    }

    case CMD_SCI_SET_TXRETRY:
    {
        SCI_TXRETRY_S *TxRetry;
        TxRetry = (SCI_TXRETRY_S *)arg;

        Ret = SCI_SetTxretry(TxRetry->enSciPort, TxRetry->TxRetryTimes);

        break;
    }

    default:
        Ret = HI_SUCCESS;
        break;
    }

    return Ret;
}

#ifdef CONFIG_COMPAT
HI_S32 SCI_Compat_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    SCI_DEV_STATE_S    *pSciDevState;
    HI_S32 Ret;

    pSciDevState = file->private_data;

    Ret = down_interruptible(&g_SciMutex);

    switch (cmd)
    {
        case CMD_SCI_OPEN:
        {
            SCI_OPEN_S     *pSciOpen;

            pSciOpen = (SCI_OPEN_S *)arg;

            Ret = SCI_ExtOpen(pSciOpen, pSciDevState);

            break;
        }

        case CMD_SCI_CLOSE:
        {
            Ret = SCI_CheckState(*((HI_UNF_SCI_PORT_E *)arg), pSciDevState);
            if (HI_SUCCESS == Ret)
            {
                Ret = SCI_ExtClose(*((HI_UNF_SCI_PORT_E *)arg), pSciDevState);
            }
            else
            {
                Ret = HI_SUCCESS;
            }

            break;
        }

        default:
            break;
    }

    up(&g_SciMutex);

    switch (cmd)
    {
        case CMD_SCI_RESET:
        {
            SCI_RESET_S  *pSciReset;

            pSciReset = (SCI_RESET_S *)arg;

            Ret = SCI_Reset(pSciReset->enSciPort, pSciReset->bWarmReset);

            break;
        }

        case CMD_SCI_DEACTIVE:
        {
            Ret = SCI_Deactive(*((HI_UNF_SCI_PORT_E *)arg));

            break;
        }

        case CMD_SCI_COMPAT_GET_ATR:
        {
            SCI_ATR_COMPAT_S  *PSciAtr;

            PSciAtr = (SCI_ATR_COMPAT_S *)arg;
#ifdef CONFIG_COMPAT
            Ret = SCI_GetAtr(PSciAtr->enSciPort, (HI_U8 *)compat_ptr(PSciAtr->pAtrBuf), PSciAtr->BufSize, &PSciAtr->DataLen);
#endif
            break;
        }

        case CMD_SCI_GET_STATUS:
        {
            SCI_STATUS_S   *pSciStatus;

            pSciStatus = (SCI_STATUS_S *)arg;

            Ret = SCI_GetStatus(pSciStatus->enSciPort, &pSciStatus->enSciStatus);

            break;
        }

        case CMD_SCI_CONF_VCC:
        {
            SCI_LEVEL_S   *pSciLevel;

            pSciLevel = (SCI_LEVEL_S *)arg;

            Ret = SCI_ConfVcc(pSciLevel->enSciPort, pSciLevel->enSciLevel);

            break;
        }

        case CMD_SCI_CONF_DETECT:
        {
            SCI_LEVEL_S   *pSciLevel;

            pSciLevel = (SCI_LEVEL_S *)arg;

            Ret = SCI_ConfDetect(pSciLevel->enSciPort, pSciLevel->enSciLevel);

            break;
        }

        case CMD_SCI_CONF_MODE:
        {
            SCI_IO_OUTPUTTYPE_S   *pSciIO;

            pSciIO = (SCI_IO_OUTPUTTYPE_S *)arg;

            if (SCI_IO_CLK == pSciIO->enIO)
            {
                Ret = SCI_ConfClkMode(pSciIO->enSciPort, pSciIO->enOutputType);
            }
            else if (SCI_IO_RESET == pSciIO->enIO)
            {
                Ret = SCI_ConfResetMode(pSciIO->enSciPort, pSciIO->enOutputType);
            }
            else if (SCI_IO_VCC_EN == pSciIO->enIO)
            {
                Ret = SCI_ConfVccEnMode(pSciIO->enSciPort, pSciIO->enOutputType);
            }
            else
            {
                Ret = HI_ERR_SCI_NOTSUPPORT;
            }

            break;
        }

        case CMD_SCI_COMPAT_SEND_DATA:
        {
            SCI_DATA_COMPAT_S    *pSciData;

            pSciData = (SCI_DATA_COMPAT_S *)arg;
#ifdef CONFIG_COMPAT
            Ret = SCI_SendData(pSciData->enSciPort,(HI_U8 *)compat_ptr(pSciData->pDataBuf), pSciData->BufSize, &pSciData->DataLen,
                               pSciData->TimeoutMs);
#endif
            break;
        }

        case CMD_SCI_COMPAT_RECEIVE_DATA:
        {
            SCI_DATA_COMPAT_S    *pSciData;

            pSciData = (SCI_DATA_COMPAT_S *)arg;
#ifdef CONFIG_COMPAT
            Ret = SCI_ReceiveData(pSciData->enSciPort, (HI_U8 *)compat_ptr(pSciData->pDataBuf), pSciData->BufSize, &pSciData->DataLen,
                                  pSciData->TimeoutMs);
#endif
            if (HI_ERR_SCI_RECEIVE_ERR == Ret)
            {
                Ret = HI_SUCCESS;
            }

            break;
        }

        case CMD_SCI_SWITCH:
        {
            SCI_OPEN_S     *pSciOpen;

            pSciOpen = (SCI_OPEN_S *)arg;

            Ret = SCI_Switch(pSciOpen->enSciPort, pSciOpen->enSciProtocol, pSciOpen->Frequency);

            break;
        }

        case CMD_SCI_SET_BAUD:
        {
            SCI_EXT_BAUD_S     *pSciExtBaud;

            pSciExtBaud = (SCI_EXT_BAUD_S *)arg;

            Ret = SCI_ExtSetBaud(pSciExtBaud->enSciPort, pSciExtBaud->ClkRate, pSciExtBaud->BitRate);

            break;
        }

        case CMD_SCI_SET_CHGUARD:
        {
            SCI_ADD_GUARD_S  *pSciAddGuard;
            pSciAddGuard = (SCI_ADD_GUARD_S *)arg;

            Ret = SCI_SetCharGuard(pSciAddGuard->enSciPort, pSciAddGuard->AddCharGuard);
            break;
        }

        case CMD_SCI_SEND_PPS_DATA:
        {
            SCI_PPS_S    *pSciSendPPS;

            pSciSendPPS = (SCI_PPS_S *)arg;

            Ret = SCI_PPS_Negotiation(pSciSendPPS->enSciPort, pSciSendPPS->Send, pSciSendPPS->SendLen,
                                      pSciSendPPS->RecTimeouts);

            break;
        }

        case CMD_SCI_GET_PPS_DATA:
        {
            SCI_PPS_S    *pSciRecPPS;

            pSciRecPPS = (SCI_PPS_S *)arg;

            Ret = SCI_PPS_GetRecBuf(pSciRecPPS->enSciPort, pSciRecPPS->Receive, &pSciRecPPS->ReceiveLen);

            break;
        }

        case CMD_SCI_GET_PARAM:
        {
            HI_UNF_SCI_PARAMS_S_PTR pSciOutParam;
            pSciOutParam = (HI_UNF_SCI_PARAMS_S_PTR)arg;

            Ret = SCI_OutPutParam(pSciOutParam->enSciPort, pSciOutParam);

            break;
        }

        case CMD_SCI_SET_CHARTIMEOUT:
        {
            SCI_CHARTIMEOUT_S *ChTimeout;
            ChTimeout = (SCI_CHARTIMEOUT_S *)arg;

            Ret = SCI_SetCharTimeout(ChTimeout->enSciPort, ChTimeout->enSciProtocol, ChTimeout->CharTimeouts);
            break;
        }

        case CMD_SCI_SET_BLOCKTIMEOUT:
        {
            SCI_BLOCKTIMEOUT_S *BlkTimeout;
            BlkTimeout = (SCI_BLOCKTIMEOUT_S *)arg;

            Ret = SCI_SetBlockTimeout(BlkTimeout->enSciPort, BlkTimeout->BlockTimeouts);

            break;
        }

        case CMD_SCI_SET_TXRETRY:
        {
            SCI_TXRETRY_S *TxRetry;
            TxRetry = (SCI_TXRETRY_S *)arg;

            Ret = SCI_SetTxretry(TxRetry->enSciPort, TxRetry->TxRetryTimes);

            break;
        }

        default:
            Ret = HI_SUCCESS;
            break;
    }

    return Ret;
}
#endif

static HI_S32 SCI_DRV_Open(struct inode *inode, struct file *filp)
{
    HI_S32 Ret;
    SCI_DEV_STATE_S    *pSciDevState = HI_NULL;
    HI_U8 ii = 0;

    Ret = down_interruptible(&g_SciMutex);

    pSciDevState = HI_KMALLOC(HI_ID_SCI, sizeof(SCI_DEV_STATE_S), GFP_KERNEL);
    if (!pSciDevState)
    {
        HI_FATAL_SCI("malloc pSciDevState failed.\n");
        up(&g_SciMutex);
        return HI_FAILURE;
    }

    if (1 == atomic_inc_return(&g_SciCount))
    {
        SCI_Init();
    }

    for (ii = 0; ii < HI_SCI_PORT_NUM; ii++)
    {
        pSciDevState->bSci[ii] = HI_FALSE;
    }

    filp->private_data = pSciDevState;

    up(&g_SciMutex);
    return HI_SUCCESS;
}

static HI_S32 SCI_DRV_Close(struct inode *inode, struct file *filp)
{
    HI_S32 Ret;
    SCI_DEV_STATE_S    *pSciDevState;
    HI_U8 ii = 0;

    Ret = down_interruptible(&g_SciMutex);

    pSciDevState = filp->private_data;

    for (ii = HI_UNF_SCI_PORT0; ii < HI_SCI_PORT_NUM; ii++)
    {
        if (pSciDevState->bSci[ii])
        {
            Ret = SCI_ExtClose(ii, pSciDevState);
            if (Ret != HI_SUCCESS)
            {
                up(&g_SciMutex);
                return HI_FAILURE;
            }
        }
    }

    if (atomic_dec_and_test(&g_SciCount))
    {}

    HI_KFREE(HI_ID_SCI, filp->private_data);

    up(&g_SciMutex);

    return HI_SUCCESS;
}

static long SCI_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, SCI_Ioctl);

    return Ret;
}

#ifdef CONFIG_COMPAT
static long SCI_DRV_Compat_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, SCI_Compat_Ioctl);

    return Ret;
}
#endif

static struct file_operations SCI_FOPS =
{
    owner: THIS_MODULE,
    open: SCI_DRV_Open,
    unlocked_ioctl: SCI_DRV_Ioctl,
#ifdef CONFIG_COMPAT
    compat_ioctl: SCI_DRV_Compat_Ioctl,
#endif
    release: SCI_DRV_Close,
};

static HI_S32 sci_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    int ret;

    ret = down_trylock(&g_SciMutex);
    if (ret)
    {
        HI_FATAL_SCI("sci_pm_suspend  lock err!\n");
        return HI_FAILURE;
    }

#if 0
    // 1
    if (0 == atomic_read(&g_SciCount))
    {
        up(&g_SciMutex);
        HI_FATAL_SCI("sci_pm_suspend  have close device!\n");
        return -1;
    }
#endif


    // 2
    ret = SCI_Suspend();
    if (ret)
    {
        up(&g_SciMutex);
        return HI_FAILURE;
    }

    up(&g_SciMutex);
    HI_PRINT("SCI suspend OK\n");
    return HI_SUCCESS;
}

static HI_S32 sci_pm_resume(PM_BASEDEV_S *pdev)
{
    int ret;

    ret = down_trylock(&g_SciMutex);
    if (ret)
    {
        HI_FATAL_SCI("sci_pm_resume  lock err!\n");
        return HI_FAILURE;
    }

    /*resume sci module will reopen and active smart card*/
    ret = SCI_Resume();
    if (ret)
    {
        up(&g_SciMutex);
        return HI_FAILURE;
    }

    up(&g_SciMutex);
    HI_PRINT("SCI resume OK\n");
    return HI_SUCCESS;
}

static PM_BASEOPS_S sci_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = sci_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = sci_pm_resume,
};

HI_S32 SCI_DRV_ModInit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S  *pProcItem;
#endif
    HI_U8 ii = 0;
    HI_CHAR s8Buff[HI_UNF_SCI_PORT_BUTT][PROC_NAME_LEN];

    (HI_VOID)HI_DRV_MODULE_Register(HI_ID_SCI, "HI_SCI", (HI_VOID *)&g_stSciExtFuncs);

    HI_OSAL_Snprintf(g_SciRegisterData.devfs_name, sizeof(g_SciRegisterData.devfs_name), UMAP_DEVNAME_SCI);
    g_SciRegisterData.minor  = UMAP_MIN_MINOR_SCI;
    g_SciRegisterData.owner  = THIS_MODULE;
    g_SciRegisterData.drvops = &sci_drvops;
    g_SciRegisterData.fops = &SCI_FOPS;

    /* SCI device regeister */
    if (HI_DRV_DEV_Register(&g_SciRegisterData) < 0)
    {
        HI_FATAL_SCI("register SCI failed.\n");
        return HI_FAILURE;
    }

    /* register SCI proc function*/
    for (ii = 0; ii < HI_SCI_PORT_NUM; ii++)
    {
        memset(s8Buff[ii], 0, sizeof(s8Buff[ii]));
        HI_OSAL_Snprintf(s8Buff[ii], PROC_NAME_LEN, "sci%d", ii);
#if (1 == HI_PROC_SUPPORT)
        pProcItem = HI_DRV_PROC_AddModule(s8Buff[ii], HI_NULL, HI_NULL);
        if (!pProcItem)
        {
            HI_FATAL_SCI("add sci%d proc failed.\n", ii);
            HI_DRV_DEV_UnRegister(&g_SciRegisterData);
            return HI_FAILURE;
        }

        pProcItem->read  = SCI_ProcRead;
#endif
        g_SciOpenNum[ii] = 0;
    }
#ifdef MODULE
    HI_PRINT("Load hi_sci.ko success.   \t(%s)\n", VERSION_STRING);
#endif
    return HI_SUCCESS;
}

HI_VOID SCI_DRV_ModExit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_SCI);
#if (1 == HI_PROC_SUPPORT)
    HI_DRV_PROC_RemoveModule("sci0");
#endif
	if(HI_SCI_PORT_NUM > 1)
	{
    	HI_DRV_PROC_RemoveModule("sci1");
	}
    HI_DRV_DEV_UnRegister(&g_SciRegisterData);
    return;
}

#ifdef MODULE
module_init(SCI_DRV_ModInit);
module_exit(SCI_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");
