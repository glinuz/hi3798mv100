#ifndef __SCI_HAL_H__
#define __SCI_HAL_H__

#include "drv_sci_ioctl.h"
#include "drv_sci_reg.h"

#ifdef __cplusplus
#if __cplusplus
       extern "C"{
#endif
#endif

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798hv100)
#define IRQ_SCI0    (36 + 32)
#define IRQ_SCI1    (37 + 32)

#define SCI0_CRG_OFFSET   (0x74)
#define SCI1_CRG_OFFSET   (0x74)
#define SCI0_PHY_ADDR     (0xF8B18000)
#define SCI1_PHY_ADDR     (0xF8B19000)

#else
#define IRQ_SCI0    (31 + 32)
#define IRQ_SCI1    (32 + 32)

#define SCI0_CRG_OFFSET   (0xE8)
#define SCI1_CRG_OFFSET   (0xEC)
#define SCI0_PHY_ADDR     (0x101F3000)
#define SCI1_PHY_ADDR     (0x101F4000)

#endif

#define SCI_DFT_REF_CLOCK  60000UL

#define SCI_FIFO_SIZE      32

typedef enum hiSCI_INT_E
{
    SCI_INT_CARDIN    = 0x1,
    SCI_INT_CARDOUT   = 0x2,
    SCI_INT_CARDUP    = 0x4,
    SCI_INT_CARDDOWN  = 0x8,
    SCI_INT_TXERR     = 0x10,
    SCI_INT_ATRSTOUT  = 0x20,
    SCI_INT_ATRDTOUT  = 0x40,
    SCI_INT_BLKTOUT   = 0x80,
    SCI_INT_CHTOUT    = 0x100,
    SCI_INT_RTOUT     = 0x200,
    SCI_INT_RORI      = 0x400,
    SCI_INT_CLKSTPI   = 0x800,
    SCI_INT_CLKACTI   = 0x1000,
    SCI_INT_RXTIDE    = 0x2000,
    SCI_INT_TXTIDE    = 0x4000
}SCI_INT_E;

#define SCI_DFT_INT     ((SCI_INT_CARDIN)|(SCI_INT_CARDOUT)\
                        |(SCI_INT_CARDUP)|(SCI_INT_CARDDOWN)\
                        |(SCI_INT_RXTIDE)|(SCI_INT_ATRSTOUT)\
                        |(SCI_INT_ATRDTOUT)|(SCI_INT_RORI))


typedef enum hiSCI_PARITY_E
{
    SCI_PARITY_EVEN,
    SCI_PARITY_ODD
} SCI_PARITY_E;

typedef enum hiSCI_DIRECTION_E
{
    SCI_DIRECTION_RX,
    SCI_DIRECTION_TX
} SCI_DIRECTION_E;


HI_VOID SCI_HAL_Init(HI_UNF_SCI_PORT_E enSciPort, SCI_REG_S *pSciReg);
HI_VOID SCI_HAL_SetSCIClock(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);
HI_VOID SCI_HAL_SetSCIReset(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_ClearSCIReset(HI_UNF_SCI_PORT_E enSciPort);

HI_U32 SCI_HAL_ReadData(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_WriteData(HI_UNF_SCI_PORT_E enSciPort, HI_U8 SciData);

HI_BOOL SCI_HAL_GetCardPresent(HI_UNF_SCI_PORT_E enSciPort);

HI_U32 SCI_HAL_GetCr0(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_SetVcc(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciVcc);
HI_VOID SCI_HAL_SetDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciDetect);
HI_VOID SCI_HAL_SetDataReversal(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bDataReversal);
HI_VOID SCI_HAL_SetTxParityMode(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, SCI_PARITY_E enParity);
HI_VOID SCI_HAL_SetRxParityMode(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, SCI_PARITY_E enParity);
HI_VOID SCI_HAL_SetParityEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);
HI_BOOL SCI_HAL_GetParityEnable(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_SetClock(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable, HI_U32 Value);
HI_VOID SCI_HAL_SetResetMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enResetMode);
HI_VOID SCI_HAL_SetVccEnMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enVccEnMode);

HI_VOID SCI_HAL_SetAtrTimeoutEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);
HI_VOID SCI_HAL_SetBlockTimeoutEnable(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);
HI_VOID SCI_HAL_SetClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_MODE_E enClkMode);
HI_VOID SCI_HAL_SetDirection(HI_UNF_SCI_PORT_E enSciPort, SCI_DIRECTION_E enDirection);
HI_VOID SCI_HAL_SetBlockProtect(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);
HI_VOID SCI_HAL_SelectCounter(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value);
HI_VOID SCI_HAL_SetSync(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bEnable);

HI_VOID SCI_HAL_Startup(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_Finish(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_Reset(HI_UNF_SCI_PORT_E enSciPort);

HI_VOID SCI_HAL_SetClkIcc(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value);
HI_VOID SCI_HAL_SetEtu(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value);
HI_VOID SCI_HAL_SetBaud(HI_UNF_SCI_PORT_E enSciPort, HI_U32 Value);
HI_VOID SCI_HAL_SetTide(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxTide, HI_U32 TxTide);
HI_VOID SCI_HAL_SetStableTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StableTime);
HI_VOID SCI_HAL_SetActiveTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 ActiveTime);
HI_VOID SCI_HAL_SetDeactTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 DeactTime);
HI_VOID SCI_HAL_SetAtrStartTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 AtrStartTime);
HI_VOID SCI_HAL_SetAtrDurationTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 AtrDurationTime);
HI_VOID SCI_HAL_SetStopTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StopTime);
HI_VOID SCI_HAL_SetStartTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 StartTime);
HI_VOID SCI_HAL_SetRetry(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxRetry, HI_U32 TxReTry);
HI_VOID SCI_HAL_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 CharTimeout);
HI_VOID SCI_HAL_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockTimeout);
HI_VOID SCI_HAL_SetCharGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 CharGuard);
HI_VOID SCI_HAL_SetBlockGuard(HI_UNF_SCI_PORT_E enSciPort, HI_U32 BlockGuard);
HI_VOID SCI_HAL_SetRxTime(HI_UNF_SCI_PORT_E enSciPort, HI_U32 RxTime);
HI_U32  SCI_HAL_GetTxFifo(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_ClearTxFifo(HI_UNF_SCI_PORT_E enSciPort);
HI_U32  SCI_HAL_GetRxFifo(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_ClearRxFifo(HI_UNF_SCI_PORT_E enSciPort);

HI_VOID SCI_HAL_DisableAllInt(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_EnableInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType);
HI_VOID SCI_HAL_DisableInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType);
HI_U32  SCI_HAL_GetIntState(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_ClearAllInt(HI_UNF_SCI_PORT_E enSciPort);
HI_VOID SCI_HAL_ClearInt(HI_UNF_SCI_PORT_E enSciPort, SCI_INT_E SciIntType);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
