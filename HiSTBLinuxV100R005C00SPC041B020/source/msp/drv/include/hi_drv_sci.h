/*
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     2013-01-07 create this file
 *
 */

#ifndef __HI_DRV_SCI_H__
#define __HI_DRV_SCI_H__

#include "hi_debug.h"
#include "hi_unf_sci.h"

#define HI_FATAL_SCI(fmt...)    HI_FATAL_PRINT(HI_ID_SCI, fmt)
#define HI_ERR_SCI(fmt...)      HI_ERR_PRINT(HI_ID_SCI, fmt)
#define HI_WARN_SCI(fmt...)     HI_WARN_PRINT(HI_ID_SCI, fmt)
#define HI_INFO_SCI(fmt...)     HI_INFO_PRINT(HI_ID_SCI, fmt)

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)	\
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)

#define HI_SCI_PORT_NUM     (2)

#elif  defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)	\
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798hv100)

#define HI_SCI_PORT_NUM     (1)

#else

#error YOU MUST DEFINE  CHIP_TYPE!

#endif

#define SCI_ATR_BUF_LEN         256

typedef struct hiDRV_ATR_BUF_S
{
    HI_U8    DataBuf[SCI_ATR_BUF_LEN];
    HI_U32   DataLen;
}HI_DRV_ATR_BUF_S;


HI_S32 HI_DRV_SCI_Reset(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmReset);

HI_S32 HI_DRV_SCI_SendData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);

HI_S32 HI_DRV_SCI_ReceiveData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);

HI_S32 HI_DRV_SCI_ConfVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel);

HI_S32 HI_DRV_SCI_ConfDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel);

HI_S32 HI_DRV_SCI_ConfClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode);

HI_S32 HI_DRV_SCI_GetSciPara(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pSciPara);

HI_S32 HI_DRV_SCI_GetSciAtrBuf(HI_UNF_SCI_PORT_E enSciPort, HI_DRV_ATR_BUF_S  *pSciAtrBuf);

HI_S32 HI_DRV_SCI_Deactive(HI_UNF_SCI_PORT_E enSciPort);

HI_S32 HI_DRV_SCI_GetStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penState);

HI_S32 HI_DRV_SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency);

HI_S32 HI_DRV_SCI_Close(HI_UNF_SCI_PORT_E enSciPort);

HI_VOID HI_DRV_SCI_Init(HI_VOID);

#endif
