/*     extdrv/include/gpio_i2c.h for Linux .
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     03-Apr-2006 create this file
 *
 */

#ifndef _GPIO_I2C_EXT_H
#define _GPIO_I2C_EXT_H

#include <linux/kernel.h>
#include <linux/fs.h>
#include "hi_type.h"

HI_S32 GPIOI2C_DRV_ModInit(HI_VOID);
HI_VOID GPIOI2C_DRV_ModExit(HI_VOID);

typedef long (*FN_GPIO_I2C_IOCTL)(void *, unsigned int, unsigned long);

//typedef HI_S32 (*FN_GPIO_I2C_CONFIG)(HI_U32, HI_U32, HI_U32, HI_U32, HI_U32, HI_U32);

typedef HI_S32 (*FN_GPIO_I2C_WRITE)(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 u8Data);
typedef HI_S32 (*FN_GPIO_I2C_WRITE_EXT)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);

//typedef HI_S32 (*FN_GPIO_I2C_WRITE_EXT_NOSTOP)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);

typedef HI_S32 (*FN_GPIO_I2C_READ)(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data);
typedef HI_S32 (*FN_GPIO_I2C_READ_EXT)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);

//typedef HI_S32 (*FN_GPIO_I2C_READ_EXT_NOSTOP)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);

//typedef HI_S32 (*FN_GPIO_I2C_CLOSE)(struct inode *, struct file  *);
typedef HI_S32 (*FN_GPIO_I2C_SCCB_READ)(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data);

//typedef HI_S32 (*FN_GPIO_I2C_SENDDATA_TUNER)(unsigned int, unsigned char, i2c_data_tuner_s *);
//typedef HI_S32 (*FN_GPIO_I2C_RECEIVEDATA_TUNER)(unsigned int, unsigned char , i2c_data_tuner_s *);

typedef HI_S32 (*FN_GPIO_I2C_CREATE_CHANNEL)(HI_U32 *, HI_U32, HI_U32);
typedef HI_S32 (*FN_GPIO_I2C_DESTROY_CHANNEL)(HI_U32);
typedef HI_S32 (*FN_GPIO_I2C_IS_USED)(HI_U32, HI_BOOL *);

typedef struct
{
    FN_GPIO_I2C_IOCTL pfnGpioI2cIOCtl;
	FN_GPIO_I2C_IOCTL pfnGpioI2cCompatIOCtl;

    //FN_GPIO_I2C_CONFIG    pfnGpioI2cConfig;
    FN_GPIO_I2C_WRITE     pfnGpioI2cWrite;
    FN_GPIO_I2C_WRITE_EXT pfnGpioI2cWriteExt;
    FN_GPIO_I2C_WRITE_EXT pfnGpioI2cWriteExtNOSTOP;
    FN_GPIO_I2C_READ      pfnGpioI2cRead;
    FN_GPIO_I2C_READ_EXT  pfnGpioI2cReadExt;
    FN_GPIO_I2C_READ_EXT  pfnGpioI2cReadExtDiRectly;

    //FN_GPIO_I2C_CLOSE      pfnGpioI2cClose;
    FN_GPIO_I2C_SCCB_READ pfnGpioI2cSCCBRead;
    //FN_GPIO_I2C_SENDDATA_TUNER    pfnGpioI2cTunerSendData;
    //FN_GPIO_I2C_RECEIVEDATA_TUNER pfnGpioI2cTunerReceiveData;
    FN_GPIO_I2C_CREATE_CHANNEL  pfnGpioI2cCreateChannel;
    FN_GPIO_I2C_DESTROY_CHANNEL pfnGpioI2cDestroyChannel;
    FN_GPIO_I2C_IS_USED         pfnGpioI2cIsUsed;
} GPIO_I2C_EXT_FUNC_S;

#endif
