
#ifndef __HI_DRV_SPI_H__
#define __HI_DRV_SPI_H__

#include "hi_type.h"
#include "hi_debug.h"

#define MAX_SPI_CHANNEL        2

#define HI_FATAL_SPI(fmt...)    HI_FATAL_PRINT(HI_ID_SPI, fmt)
#define HI_ERR_SPI(fmt...)      HI_ERR_PRINT(HI_ID_SPI, fmt)
#define HI_WARN_SPI(fmt...)     HI_WARN_PRINT(HI_ID_SPI, fmt)
#define HI_INFO_SPI(fmt...)     HI_INFO_PRINT(HI_ID_SPI, fmt)

HI_S32 HI_DRV_SPI_Init(HI_VOID);
HI_S32 HI_DRV_SPI_DeInit(HI_VOID);
HI_S32 HI_DRV_SPI_Open(HI_U8 devId);
HI_S32 HI_DRV_SPI_Close(HI_U8 devId);
HI_VOID HI_DRV_SPI_SetCs_Gpio(HI_U8 devId);
HI_U8 HI_DRV_SPI_Get_CsCfg(HI_VOID);
HI_VOID HI_DRV_SPI_SetCs_Level(HI_U8 devId,HI_U32 Level);
HI_VOID HI_DRV_SPI_Set_CsCfg(HI_U8 gpioCs);
HI_S32 HI_DRV_SPI_SetFrom(HI_U8 devId,HI_U8 framemode,HI_U8 spo,HI_U8 sph,HI_U8 datawidth);
HI_S32 HI_DRV_SPI_GetFrom(HI_U8 devId,HI_U8 *framemode,HI_U8 *spo,HI_U8 *sph,HI_U8 *datawidth);
HI_VOID HI_DRV_SPI_SetBlend(HI_U8 devId,HI_U8 bBigEnd);
HI_U8 HI_DRV_SPI_GetBlend(HI_U8 devId);
HI_S32 HI_DRV_SPI_WriteIsr(HI_U8 devId,HI_U8 *Send, HI_U32 SendCnt);
HI_S32 HI_DRV_SPI_WriteQuery(HI_U8 devId,HI_U8 *Send, HI_U32 SendCnt);
HI_S32 HI_DRV_SPI_ReadQuery(HI_U8 devId, HI_U8 *Read, HI_U32 ReadCnt);
HI_S32 HI_DRV_SPI_ReadIsr(HI_U8 devId, HI_U8 *Read, HI_U32 ReadCnt);
HI_S32 HI_DRV_SPI_ReadEx(HI_U8 devId, HI_U8 *Send, HI_U32 SendCnt, HI_U8 *Read, HI_U32 ReadCnt);
HI_S32 HI_DRV_SPI_ReadEx_Isr(HI_U8 devId, HI_U8 *Send, HI_U32 SendCnt, HI_U8 *Read, HI_U32 ReadCnt);
HI_VOID HI_DRV_SPI_Enable(HI_U8 devId);
HI_VOID HI_DRV_SPI_DisEnable(HI_U8 devId);

HI_S32 HI_DRV_SPI_SetClk(HI_U8 devId, HI_U8 scr, HI_U8 cpsdvsr);


#endif

