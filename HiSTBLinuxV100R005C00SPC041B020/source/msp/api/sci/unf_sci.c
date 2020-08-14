/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_unf_sci.c
* Description:
*
* History:
* Version   Date                Author     DefectNum    Description
* main\1    2008-10-10   w00142069      NULL         Create this file.
***********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "drv_sci_ioctl.h"
#include "hi_drv_struct.h"

static HI_S32 g_SciDevFd = -1;

static pthread_mutex_t g_SciMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_SCI_LOCK()   (void)pthread_mutex_lock(&g_SciMutex);
#define HI_SCI_UNLOCK() (void)pthread_mutex_unlock(&g_SciMutex);

#define CHECK_SCI_OPEN() \
    do {\
        HI_SCI_LOCK(); \
        if (g_SciDevFd < 0)\
        {\
            HI_ERR_SCI("SCI is not open.\n"); \
            HI_SCI_UNLOCK(); \
            return HI_ERR_SCI_NOT_INIT; \
        } \
        HI_SCI_UNLOCK(); \
    } while (0)

HI_S32 HI_UNF_SCI_Init(HI_VOID)
{
    HI_SCI_LOCK();

    /* reopen will direct return success*/
    if (g_SciDevFd > 0)
    {
        HI_SCI_UNLOCK();
        return HI_SUCCESS;
    }

    g_SciDevFd = open("/dev/" UMAP_DEVNAME_SCI, O_RDWR, 0);
    if (g_SciDevFd < 0)
    {
        HI_FATAL_SCI("open SCI err.\n");
        HI_SCI_UNLOCK();
        return HI_ERR_SCI_OPEN_ERR;
    }

    HI_SCI_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_SCI_DeInit(HI_VOID)
{
    HI_S32 Ret;

    HI_SCI_LOCK();

    if (g_SciDevFd < 0)
    {
        HI_SCI_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_SciDevFd);

    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_SCI("Close SCI err.\n");
        HI_SCI_UNLOCK();
        return HI_ERR_SCI_CLOSE_ERR;
    }

    g_SciDevFd = -1;

    HI_SCI_UNLOCK();

    return HI_SUCCESS;
}

/*******************************************
Function:              HI_UNF_SCI_Open
Description:  open SCI device
Calls:       HI_SCI_Open
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency)
{
    HI_S32 Ret;
    SCI_OPEN_S SciOpen;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enSciProtocol >= HI_UNF_SCI_PROTOCOL_BUTT)
    {
        HI_ERR_SCI("para enSciProtocol is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (HI_UNF_SCI_PROTOCOL_T14 == enSciProtocol)
    {
        if ((u32Frequency < 1000) || (u32Frequency > 6000))
        {
            HI_ERR_SCI("para u32Frequency is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }
    else
    {
        if ((u32Frequency < 1000) || (u32Frequency > 5000))
        {
            HI_ERR_SCI("para u32Frequency is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }

    CHECK_SCI_OPEN();

    SciOpen.enSciPort = enSciPort;
    SciOpen.enSciProtocol = enSciProtocol;
    SciOpen.Frequency = u32Frequency;

    Ret = ioctl(g_SciDevFd, CMD_SCI_OPEN, &SciOpen);

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_Close
Description:  close SCI device
Calls:        HI_SCI_Close
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_Close(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_S32 Ret;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    Ret = ioctl(g_SciDevFd, CMD_SCI_CLOSE, &enSciPort);

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_ResetCard
Description:  reset card
Calls:        HI_UNF_SCI_ResetCard
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_ResetCard(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmResetValid)
{
    HI_S32 Ret;
    SCI_RESET_S SciReset;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if ((bWarmResetValid != HI_TRUE)
        && (bWarmResetValid != HI_FALSE)
    )
    {
        HI_ERR_SCI("para bWarmResetValid is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciReset.enSciPort  = enSciPort;
    SciReset.bWarmReset = bWarmResetValid;

    Ret = ioctl(g_SciDevFd, CMD_SCI_RESET, &SciReset);

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_DeactiveCard
Description:  deactive card
Calls:        HI_SCI_DeactiveCard
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_DeactiveCard(HI_UNF_SCI_PORT_E enSciPort)
{
    HI_S32 Ret;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    Ret = ioctl(g_SciDevFd, CMD_SCI_DEACTIVE, &enSciPort);

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_GetATR
Description:  get ATR data
Calls:        HI_SCI_GetATR
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_GetATR(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pu8AtrBuf, HI_U32 u32AtrBufSize, HI_U8 *pu8AtrRcvCount)
{
    HI_S32 Ret;
    SCI_ATR_S SciAtr;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pu8AtrBuf)
    {
        HI_ERR_SCI("para pu8AtrBuf is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (!u32AtrBufSize)
    {
        HI_ERR_SCI("para u32AtrBufSize is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pu8AtrRcvCount)
    {
        HI_ERR_SCI("para pu8AtrRcvCount is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    CHECK_SCI_OPEN();

    SciAtr.enSciPort = enSciPort;
    SciAtr.pAtrBuf = pu8AtrBuf;
    SciAtr.BufSize = u32AtrBufSize;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_ATR, &SciAtr);

    *pu8AtrRcvCount = SciAtr.DataLen;

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_GetCardStatus
Description:  get the status of card
Calls:        HI_UNF_SCI_GetCardStatus
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_GetCardStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penSciStatus)
{
    HI_S32 Ret;
    SCI_STATUS_S SciStatus;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!penSciStatus)
    {
        HI_ERR_SCI("para penSciStatus is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    CHECK_SCI_OPEN();

    SciStatus.enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_STATUS, &SciStatus);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *penSciStatus = SciStatus.enSciStatus;

    return HI_SUCCESS;
}

/*******************************************
Function:              HI_UNF_SCI_Send
Description:  send data to card
Calls:        HI_SCI_Send
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_Send(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf, HI_U32 u32SendLen, HI_U32 *pu32ActLen,
                       HI_U32 u32TimeoutMs)
{
    HI_S32 Ret;
    SCI_STATUS_S SciStatus;
    SCI_DATA_S SciData;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pSciSendBuf)
    {
        HI_ERR_SCI("para pSciSendBuf is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (!pu32ActLen)
    {
        HI_ERR_SCI("para pu32ActLen is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (!u32SendLen)
    {
        HI_ERR_SCI("para u32SendLen is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (u32SendLen > 512)
    {
        HI_ERR_SCI("para u32SendLen is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciStatus.enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_STATUS, &SciStatus);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (SciStatus.enSciStatus < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("current state cann't execute send opertaion.\n");
        return HI_ERR_SCI_INVALID_OPT;
    }

    SciData.enSciPort = enSciPort;
    SciData.pDataBuf = pSciSendBuf;
    SciData.BufSize   = u32SendLen;
    SciData.TimeoutMs = u32TimeoutMs;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SEND_DATA, &SciData);

    *pu32ActLen = SciData.DataLen;

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_Receive
Description:  receive data from card
Calls:        HI_SCI_Receive
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_Receive (HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciReceiveBuf, HI_U32 u32ReceiveLen, HI_U32 *pu32ActLen,
                           HI_U32 u32TimeoutMs)
{
    HI_S32 Ret;
    SCI_STATUS_S SciStatus;
    SCI_DATA_S SciData;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pSciReceiveBuf)
    {
        HI_ERR_SCI("para pSciReceiveBuf is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (!pu32ActLen)
    {
        HI_ERR_SCI("para pu32ActLen is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if ((u32ReceiveLen > 256) || (0 == u32ReceiveLen))
    {
        HI_ERR_SCI("para u32ReceiveLen is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciStatus.enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_STATUS, &SciStatus);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (SciStatus.enSciStatus < HI_UNF_SCI_STATUS_READATR)
    {
        HI_ERR_SCI("current state cann't execute recevie opertaion.\n");
        return HI_ERR_SCI_INVALID_OPT;
    }

    SciData.enSciPort = enSciPort;
    SciData.pDataBuf = pSciReceiveBuf;
    SciData.BufSize   = u32ReceiveLen;
    SciData.TimeoutMs = u32TimeoutMs;

    Ret = ioctl(g_SciDevFd, CMD_SCI_RECEIVE_DATA, &SciData);

    *pu32ActLen = SciData.DataLen;

    if (SciData.DataLen < u32ReceiveLen)
    {
        Ret = HI_ERR_SCI_RECEIVE_ERR;
    }

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_ConfigVccEn
Description:  set the valid level of Vcc
Calls:        HI_SCI_ConfigVccEn
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_ConfigVccEn(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciVcc)
{
    HI_S32 Ret;
    SCI_LEVEL_S SciLevel;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enSciVcc >= HI_UNF_SCI_LEVEL_BUTT)
    {
        HI_ERR_SCI("para enSciVcc is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciLevel.enSciPort  = enSciPort;
    SciLevel.enSciLevel = enSciVcc;

    Ret = ioctl(g_SciDevFd, CMD_SCI_CONF_VCC, &SciLevel);

    return Ret;
}

/*******************************************
Function:              HI_UNF_SCI_ConfigDetect
Description:  set the valid level of detect
Calls:        HI_SCI_ConfigDetect
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:         NA
Return:      ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_SCI_ConfigDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciDetect)
{
    HI_S32 Ret;
    SCI_LEVEL_S SciLevel;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enSciDetect >= HI_UNF_SCI_LEVEL_BUTT)
    {
        HI_ERR_SCI("para enSciDetect is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciLevel.enSciPort  = enSciPort;
    SciLevel.enSciLevel = enSciDetect;

    Ret = ioctl(g_SciDevFd, CMD_SCI_CONF_DETECT, &SciLevel);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_ConfigClkMode
Description		:config clk work mode(od or cmos)
Calls			:HI_SCI_ConfigClkMode
Data Accessed	:NA
Data Updated	:NA
Input			:NA
Output			:NA
Return			:ErrorCode(reference to document)
Others			:NA
*******************************************/
HI_S32 HI_UNF_SCI_ConfigClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode)
{
    HI_S32 Ret;
    SCI_IO_OUTPUTTYPE_S SciClk;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enClkMode >= HI_UNF_SCI_MODE_BUTT)
    {
        HI_ERR_SCI("para enClkMode is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciClk.enSciPort = enSciPort;
	SciClk.enIO = SCI_IO_CLK;
    SciClk.enOutputType = enClkMode;

    Ret = ioctl(g_SciDevFd, CMD_SCI_CONF_MODE, &SciClk);

    return Ret;
}

HI_S32 HI_UNF_SCI_ConfigResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode)
{
    HI_S32 Ret;
    SCI_IO_OUTPUTTYPE_S SciReset;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enResetMode >= HI_UNF_SCI_MODE_BUTT)
    {
        HI_ERR_SCI("para enResetMode is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciReset.enSciPort = enSciPort;
	SciReset.enIO = SCI_IO_RESET;
    SciReset.enOutputType = enResetMode;

    Ret = ioctl(g_SciDevFd, CMD_SCI_CONF_MODE, &SciReset);

    return Ret;
}	


HI_S32 HI_UNF_SCI_ConfigVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode)
{
    HI_S32 Ret;
    SCI_IO_OUTPUTTYPE_S SciVcc;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enVccEnMode >= HI_UNF_SCI_MODE_BUTT)
    {
        HI_ERR_SCI("para enVccEnMode is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciVcc.enSciPort = enSciPort;
	SciVcc.enIO = SCI_IO_VCC_EN;
    SciVcc.enOutputType = enVccEnMode;

    Ret = ioctl(g_SciDevFd, CMD_SCI_CONF_MODE, &SciVcc);

    return Ret;
}

HI_S32 HI_UNF_SCI_SwitchCard(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency)
{
    HI_S32 Ret;
    SCI_OPEN_S SciSwitch;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (enSciProtocol >= HI_UNF_SCI_PROTOCOL_BUTT)
    {
        HI_ERR_SCI("para enSciProtocol is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (HI_UNF_SCI_PROTOCOL_T14 == enSciProtocol)
    {
        if ((u32Frequency < 1000) || (u32Frequency > 6000))
        {
            HI_ERR_SCI("para u32Frequency is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }
    else
    {
        if ((u32Frequency < 1000) || (u32Frequency > 5000))
        {
            HI_ERR_SCI("para u32Frequency is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }

    SciSwitch.enSciPort = enSciPort;
    SciSwitch.enSciProtocol = enSciProtocol;
    SciSwitch.Frequency = u32Frequency;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SWITCH, &SciSwitch);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_SetEtuFactor
Description	:set work baudrate
Calls			:HI_UNF_SCI_SetEtuFactor
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_SetEtuFactor(HI_UNF_SCI_PORT_E enSciPort, HI_U32 u32ClkFactor, HI_U32 u32BaudFactor)

{
    HI_S32 Ret;
    SCI_EXT_BAUD_S SciExtBaud;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if ((u32ClkFactor < 372) || (u32ClkFactor > 2048))
    {
        HI_ERR_SCI("para u32ClkRate is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if ((u32BaudFactor < 1) || (u32BaudFactor > 32) || ((1 != u32BaudFactor) && (u32BaudFactor % 2) != 0))
    {
        HI_ERR_SCI("para u32BitRate is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    SciExtBaud.enSciPort = enSciPort;
    SciExtBaud.ClkRate = u32ClkFactor;
    SciExtBaud.BitRate = u32BaudFactor;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SET_BAUD, &SciExtBaud);
    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_SetGuardTime
Description	:set guard delay time
Calls			:HI_UNF_SCI_SetGuardTime
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_SetGuardTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 u32GuardTime)
{
    HI_S32 Ret;
    SCI_ADD_GUARD_S GuardTime;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (u32GuardTime > 254)
    {
        HI_ERR_SCI("para u32GuardTime is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    GuardTime.enSciPort = enSciPort;
    GuardTime.AddCharGuard = u32GuardTime;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SET_CHGUARD, &GuardTime);
    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_NegotiatePPS
Description	:request PPS negotiation
Calls			:HI_UNF_SCI_NegotiatePPS
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_NegotiatePPS(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf, HI_U32 Sendlen, HI_U32 RecTimeouts)
{
    HI_S32 Ret;
    SCI_STATUS_S SciStatus;
    SCI_PPS_S SciPPS;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pSciSendBuf)
    {
        HI_ERR_SCI("para pSciSendBuf pointer is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (5 < Sendlen)
    {
        HI_ERR_SCI("para Sendlen is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if ((RecTimeouts < 1) || (RecTimeouts > 10000))
    {
        HI_ERR_SCI("para RecTimeouts error.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    CHECK_SCI_OPEN();

    SciStatus.enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_STATUS, &SciStatus);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (SciStatus.enSciStatus < HI_UNF_SCI_STATUS_READY)
    {
        HI_ERR_SCI("current state cann't execute send opertaion.\n");
        return HI_ERR_SCI_INVALID_OPT;
    }

    memcpy((HI_U8 *)SciPPS.Send, pSciSendBuf, Sendlen);
    SciPPS.enSciPort = enSciPort;
    SciPPS.SendLen = Sendlen;
    SciPPS.RecTimeouts = RecTimeouts;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SEND_PPS_DATA, &SciPPS);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_GetPPSResponData
Description	:get PPS negotiation respond data
Calls			:HI_UNF_SCI_GetPPSResponData
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_GetPPSResponData(HI_UNF_SCI_PORT_E enSciPort, HI_U8* pRecBuf, HI_U32* pDataLen)
{
    HI_S32 Ret;
    SCI_PPS_S SciPPS;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pRecBuf)
    {
        HI_ERR_SCI("para RecBuf  is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    if (!pDataLen)
    {
        HI_ERR_SCI("para pDataLen  is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    SciPPS.enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_PPS_DATA, &SciPPS);

    memcpy(pRecBuf, (HI_U8 *)SciPPS.Receive, SciPPS.ReceiveLen);
    *pDataLen = SciPPS.ReceiveLen;

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_GetParams
Description	:get sci parameter
Calls			:HI_UNF_SCI_GetParams
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_GetParams(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pParams)
{
    HI_S32 Ret;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (!pParams)
    {
        HI_ERR_SCI("para pParams  is null.\n");
        return HI_ERR_SCI_NULL_PTR;
    }

    pParams->enSciPort = enSciPort;

    Ret = ioctl(g_SciDevFd, CMD_SCI_GET_PARAM, pParams);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_SetCharTimeout
Description	:set T0 or T1 char timeout
Calls			:HI_UNF_SCI_SetCharTimeout
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 MaxCharTime)
{
    HI_S32 Ret;
    SCI_CHARTIMEOUT_S Chtimeout;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (HI_UNF_SCI_PROTOCOL_T1 == enSciProtocol)
    {
        if ((MaxCharTime < 12) || (MaxCharTime > 32779))
        {
            HI_ERR_SCI("para MaxCharTime of T1 is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }
    else
    {
        if ((MaxCharTime < 960) || (MaxCharTime > 244800))
        {
            HI_ERR_SCI("para MaxCharTime is invalid.\n");
            return HI_ERR_SCI_INVALID_PARA;
        }
    }

    Chtimeout.enSciPort = enSciPort;
    Chtimeout.enSciProtocol = enSciProtocol;
    Chtimeout.CharTimeouts = MaxCharTime;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SET_CHARTIMEOUT, &Chtimeout);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_SetBlockTimeout
Description	:set T1 block timeout
Calls			:HI_UNF_SCI_SetBlockTimeout
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 MaxBlockTime)
{
    HI_S32 Ret;
    SCI_BLOCKTIMEOUT_S BlkTimeout;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if ((MaxBlockTime < 971) || (MaxBlockTime > 491531))
    {
        HI_ERR_SCI("para MaxBlockTime is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    BlkTimeout.enSciPort = enSciPort;
    BlkTimeout.BlockTimeouts = MaxBlockTime;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SET_BLOCKTIMEOUT, &BlkTimeout);

    return Ret;
}

/*******************************************
Function		:HI_UNF_SCI_SetTxRetries
Description	:set Tx retry times
Calls			:HI_UNF_SCI_SetTxRetries
Data Accessed	:NA
Data Updated	:NA
Input		:NA
Output		:NA
Return		:ErrorCode(reference to document)
Others		:NA
*******************************************/
HI_S32 HI_UNF_SCI_SetTxRetries(HI_UNF_SCI_PORT_E enSciPort, HI_U32 TxRetryTimes)
{
    HI_S32 Ret;
    SCI_TXRETRY_S TxRetry;

    if (enSciPort >= HI_SCI_PORT_NUM)
    {
        HI_ERR_SCI("para enSciPort is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    if (TxRetryTimes > 7)
    {
        HI_ERR_SCI("para TxRetryTimes is invalid.\n");
        return HI_ERR_SCI_INVALID_PARA;
    }

    TxRetry.enSciPort = enSciPort;
    TxRetry.TxRetryTimes = TxRetryTimes;

    Ret = ioctl(g_SciDevFd, CMD_SCI_SET_TXRETRY, &TxRetry);

    return Ret;
}
