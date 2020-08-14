#ifndef __DRV_GPIOI2C_H_
#define __DRV_GPIOI2C_H_

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef struct hiI2C_DATA_S
{
    HI_U32    I2cNum;
    HI_U8     I2cDevAddr;
    HI_U32    I2cRegAddr;
    HI_U32    I2cRegCount;
    HI_U8    *pData;
    HI_U32    DataLen;
} I2C_DATA_S;

typedef struct hiI2C_RATE_S
{
    HI_U32 I2cNum;
    HI_U32 I2cRate;
} I2C_RATE_S;

typedef struct hiI2C_GPIO_S
{
    HI_U32  I2cNum;
    HI_U32  u32SCLGpioNo;
    HI_U32  u32SDAGpioNo;
    HI_BOOL bUsed;
    HI_U32  u32Count;
} I2C_GPIO_S;

HI_S32 DRV_GPIOI2C_Config(HI_U32 I2cNum, HI_U32 whichGpioClock, HI_U32 whichGpioData, HI_U32 clockBit, HI_U32 dataBit);
HI_S32 DRV_GPIOI2C_IsUsed(HI_U32 I2cNum, HI_BOOL *bUsed);
HI_S32  HI_DRV_GPIOI2C_Init(HI_VOID);
HI_VOID HI_DRV_GPIOI2C_DeInit(HI_VOID);

HI_S32 HI_DRV_GPIOI2C_SCCBRead(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data);

HI_S32 HI_DRV_GPIOI2C_Read(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data);
HI_S32 HI_DRV_GPIOI2C_Write(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 u8Data);
HI_S32 HI_DRV_GPIOI2C_CreateGpioI2c(HI_U32 *pu32I2cNum, HI_U32 u32SCLGpioNo, HI_U32 u32SDAGpioNo);
HI_S32 HI_DRV_GPIOI2C_DestroyGpioI2c(HI_U32 I2cNum);

HI_S32 HI_DRV_GPIOI2C_ReadExt(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                              HI_U32 DataLen);
HI_S32 HI_DRV_GPIOI2C_ReadExtDirectly(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen,
                                      HI_U8 *pData, HI_U32 DataLen);
HI_S32 HI_DRV_GPIOI2C_WriteExt(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                               HI_U32 DataLen);
HI_S32 HI_DRV_GPIOI2C_WriteExtNoSTOP(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen,
                                     HI_U8 *pData, HI_U32 DataLen);

#define I2C_GPIO_DEGUB(format, arg...)//    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif


