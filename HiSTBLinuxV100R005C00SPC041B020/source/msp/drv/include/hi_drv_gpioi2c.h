/*     extdrv/include/gpio_i2c.h for Linux .
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     03-Apr-2006 create this file
 *
 */

#ifndef __HI_DRV_GPIO_I2C_H__
#define __HI_DRV_GPIO_I2C_H__

#include <linux/kernel.h>
#include "hi_type.h"

HI_S32  HI_DRV_GPIOI2C_Init(HI_VOID);
HI_VOID HI_DRV_GPIOI2C_DeInit(HI_VOID);

long HI_DRV_GPIOI2C_Ioctl(void *file, unsigned int cmd, unsigned long arg);

long HI_DRV_GPIOI2C_Compat_Ioctl(void *file, unsigned int cmd, unsigned long arg);

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

#endif
