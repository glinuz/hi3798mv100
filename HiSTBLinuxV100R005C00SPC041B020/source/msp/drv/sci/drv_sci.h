#ifndef __SCI_H__
#define __SCI_H__

#include <linux/interrupt.h>

#include "hal_sci.h"

#ifdef __cplusplus
#if __cplusplus
       extern "C"{
#endif
#endif

#define SCI_DFT_STABLE_TIME     136
#define SCI_DFT_ACT_TIME        42500
#define SCI_DFT_DEACT_TIME      200
#if defined(HI_ADVCA_TYPE_CONAX)
#define SCI_DFT_ATRS_TIME       41000
#else
#define SCI_DFT_ATRS_TIME       40000
#endif
#define SCI_DFT_ATRD_TIME       19200
#define SCI_DFT_STOP_TIME       1860
#define SCI_DFT_START_TIME      700

#define SCI_DFT_CHAR_TIME       65535
#define SCI_DFT_BLOCK_TIME      9600
#define SCI_DFT_BLKGUARD_TIME   22
#define SCI_DFT_RX_TIME         25000

#define SCI_DFT_WAIT_TIME      9600

#define SCI_ATR_BUF_LEN	        256
#define SCI_READ_BUF_LEN        512

#define SCI_ATR_GROUP_START     127
#define SCI_ATR_GROUP_DONE      128

#define SCI_ATR_CONV_NORM       0x3b
#define SCI_ATR_CONV_NORM_INV   0x3f
#define SCI_ATR_CONV_INV        0x03


typedef struct hiSCI_ATTR_S
{
    HI_UNF_SCI_PROTOCOL_E    enSciProtocol;
    HI_U32                   Frequency;
    HI_U32                   RxTimeout;
    HI_U32                   TxTimeout;
    HI_UNF_SCI_LEVEL_E       enSciVcc;
    HI_UNF_SCI_LEVEL_E       enSciDetect;
    HI_UNF_SCI_MODE_E        enClkMode;
	HI_UNF_SCI_MODE_E        enResetMode;
	HI_UNF_SCI_MODE_E        enVccEnMode;
}SCI_ATTR_S;

typedef struct hiSCI_SETUP_PARA_S
{
    HI_BOOL         bRxParity;
    HI_BOOL         bTxParity;
    SCI_PARITY_E    enRxParity;
    SCI_PARITY_E    enTxParity;

    HI_U32          ClockIcc;
    HI_U32          EtuValue;
    HI_U32          BaudValue;
    HI_U32          RxTide;
    HI_U32          TxTide;
    HI_U32          StableTime;
    HI_U32          ActiveTime;
    HI_U32          DeactTime;
    HI_U32          AtrStartTime;
    HI_U32          AtrDuration;
    HI_U32          StopTime;
    HI_U32          StartTime;
    HI_U32          RxRetries;
    HI_U32          TxRetries;

    HI_U32          CharTimeout;
    HI_U32          BlockTimeout;
    HI_U32          CharGuard;
    HI_U32          BlockGuard;
    HI_U32          RxTimeout;

    HI_U32          ClkRate;
    HI_U32	        BitRate;
} SCI_SETUP_PARA_S;

typedef enum hiSCI_BIT_RATE_E
{
    D_RFU0     = 0x0,      /*Reserved*/
    D_1        = 0x1,      /*D=1*/
    D_2        = 0x2,      /*D=2*/
    D_4        = 0x3,      /*D=4*/
    D_8        = 0x4,      /*D=8*/
    D_16       = 0x5,      /*D=16*/
    D_32       = 0x6,      /*D=32*/
    D_RFU7     = 0x7,      /*Reserved*/
    D_12       = 0x8,      /*D=12*/
    D_20       = 0x9,      /*D=20*/
    D_RFUA     = 0xa,      /*Reserved*/
    D_RFUB     = 0xb,      /*Reserved*/
    D_RFUC     = 0xc,      /*Reserved*/
    D_RFUD     = 0xd,      /*Reserved*/
    D_RFUE     = 0xe,      /*Reserved*/
    D_RFUF     = 0xf       /*Reserved*/
}SCI_BIT_RATE_E;

typedef enum hiSCI_CLOCK_RATE_E
{
    F_RFU0     = 0x0,      /*Reserved*/
    F_372      = 0x1,      /*F=372*/
    F_558      = 0x2,      /*F=558*/
    F_744      = 0x3,      /*F=744*/
    F_1116     = 0x4,      /*F=1116*/
    F_1488     = 0x5,      /*F=1488*/
    F_1860     = 0x6,      /*F=1860*/
    F_RFU7     = 0x7,      /*Reserved*/
    F_RFU8     = 0x8,      /*Reserved*/
    F_512      = 0x9,      /*F=512*/
    F_768      = 0xa,      /*F=768*/
    F_1024     = 0xb,      /*F=1024*/
    F_1536     = 0xc,      /*F=1536*/
    F_2048     = 0xd,      /*F=2048*/
    F_RFUE     = 0xe,      /*Reserved*/
    F_RFUF     = 0xf       /*Reserved*/
}SCI_CLOCK_RATE_E;


typedef struct hiSCI_ATR_PARA_S
{
    SCI_BIT_RATE_E       enBitRate;         /*bit rate adjustment*/
    SCI_CLOCK_RATE_E     enClkRate;         /*clock rate conversion*/
    HI_U32               ProtocolType;      /*first offered protocol type*/
    HI_U32               AltProtocolType;             /*alternative offered protocol type*/
    HI_U32               ChGuard;           /*char-char guard time*/
    HI_U32               CharacterTime;     /*Character to character timeout (etus) (default 9600)*/
    HI_U32               BlockTime;         /*Timeout between blocks (etus) (default xxxx)*/
    HI_U32               BlockGuard;        /*Minimum time between characters in oposite directions (etus) (default 16)*/
    HI_U32		  DataInfSize;	/*show card capabilitily receive data informatioan field size of T1 card*/
    HI_BOOL		  ChecksumType; /*0:LRC  ;1: CRC  (default 0)   may be changed by TC3*/
	
      

    HI_BOOL              bIntfByteQual;     /*T = 15 has been received indicating global                                          * interface byte qualifier */
    HI_BOOL              bHasChecksum;      /*are we expecting a checksum?*/
    HI_U32               Checksum;          /*maintains a checksum*/
    HI_U32               CharPresent;       /*mask of characters for this group*/
    HI_U32               Grouping;          /*grouping being processed*/
    HI_U32               Current;           /*character being processed*/
    HI_U32               NumHist;           /*number of chars of historical data to be received */
    HI_U32		  AtrMask;		/*show which ATR byte have received*/
}SCI_ATR_PARA_S;


typedef struct hiSCI_STATE_S
{
    HI_UNF_SCI_STATUS_E       enSciCrtState;      /*current state of the smartcard*/
    HI_UNF_SCI_PROTOCOL_E     enSciProtocol;      /*block or character transfer*/
    HI_BOOL                   bSciDataReversal;     /*Data convention*/
    SCI_SETUP_PARA_S          SciSetupPara;    /*specified setup parameters*/
    SCI_ATR_PARA_S            SciArtPara;        /*data regarding ATR setup*/
    HI_BOOL                   bForceFlag;
    HI_BOOL 			  bSetExtBaudFlag;   /*set extern baud flag*/
}SCI_STATE_S;

typedef struct hiSCI_PARA_S
{
    HI_BOOL                  bSciEnable;
    SCI_ATTR_S               SciAttr;
 	SCI_STATE_S              SciState;
    HI_U32                   ErrType;
    HI_BOOL                  bDataEnough;
    HI_U32                   u32SysClk;
    struct semaphore         SciSem;
	struct tasklet_struct	 SciReadTasklet;
    wait_queue_head_t        SciRecWaitQueue;
}SCI_PARA_S;

typedef struct hiATR_BUF_S
{
    HI_U8    DataBuf[SCI_ATR_BUF_LEN];
    HI_U32   DataLen;
}ATR_BUF_S;

typedef struct hiSCI_BUF_S
{
    HI_U32   Write;
    HI_U32   Read;
    HI_U32   DataLen;
    HI_U8    DataBuf[SCI_READ_BUF_LEN + 1];
}SCI_BUF_S;

typedef struct hiSCI_PORT_ATTR_S
{
	HI_U32		u32CRGAddrOffset;
	HI_U32		u32IrqNo;
	HI_U32   	u32RegBaseAddr;

}SCI_PORT_ATTR_S;

HI_VOID SCI_Init(HI_VOID);
HI_S32 SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency);
HI_S32 SCI_Close(HI_UNF_SCI_PORT_E enSciPort);
HI_S32 SCI_Reset(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmReset);
HI_S32 SCI_Deactive(HI_UNF_SCI_PORT_E enSciPort);
HI_S32 SCI_GetAtr(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pAtrBuf, HI_U32 AtrBufSize, HI_U8 *pAtrDataLen);
HI_S32 SCI_GetStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penState);
HI_S32 SCI_GetSciPara(HI_UNF_SCI_PORT_E enSciPort, SCI_PARA_S *pSciPara);
HI_S32 SCI_GetSciAtrBuf(HI_UNF_SCI_PORT_E enSciPort, ATR_BUF_S  *pSciAtrBuf);
HI_S32 SCI_PPS_GetRecBuf(HI_UNF_SCI_PORT_E enSciPort, HI_U8* pRecBuf, HI_U32* pDataLen);
HI_S32 SCI_OutPutParam(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pSciOutParam);
HI_S32 SCI_ConfVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel);
HI_S32 SCI_ConfDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciLevel);
HI_S32 SCI_ConfClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode);
HI_S32 SCI_ConfResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode);
HI_S32 SCI_ConfVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode);
HI_S32 SCI_SendPPSData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);
HI_S32 SCI_SendData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);
HI_S32 SCI_ReceiveData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);
HI_S32 SCI_ReceivePPSData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pDataBuf, HI_U32 BufSize, HI_U32 *pDataLen, HI_U32 TimeoutMs);
HI_S32 SCI_PPS_Negotiation(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSendBuf, HI_U32 PPSLen,  HI_U32 RecTimeouts);
HI_S32 SCI_Switch(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 Frequency);
HI_S32 SCI_ExtSetBaud( HI_UNF_SCI_PORT_E enSciPort, HI_U32 ClkRate, HI_U32 BitRate);
HI_S32 SCI_SetCharGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 ExtChGuard);
HI_S32 SCI_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol,HI_U32 CharTimeouts);
HI_S32 SCI_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockTimeouts);
HI_S32 SCI_SetTxretry(HI_UNF_SCI_PORT_E enSciPort, HI_U32 TxRetryTimes);
HI_S32 SCI_Suspend(HI_VOID);
HI_S32 SCI_Resume(HI_VOID);
HI_S32 SCI_GetPortAttr(HI_UNF_SCI_PORT_E enSciPort, SCI_PORT_ATTR_S *pstPortAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
