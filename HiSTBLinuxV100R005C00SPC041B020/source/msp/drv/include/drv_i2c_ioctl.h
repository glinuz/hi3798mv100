/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : priv_i2c.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/01/23
  Description   :
  History       :
  1.Date        : 2007/01/23
    Author      : j37987
    Modification: Created file

******************************************************************************/

#ifndef __DRV_I2C_IOCTL_H__
#define __DRV_I2C_IOCTL_H__

#include "hi_unf_i2c.h"
#include "hi_drv_i2c.h"

typedef struct hiI2C_DATA_S
{
    HI_U32    I2cNum;
    HI_U8     I2cDevAddr;
    HI_U32    I2cRegAddr;
    HI_U32    I2cRegCount;
    HI_U8    *pData;
    HI_U32    DataLen;
} I2C_DATA_S;

typedef struct hiI2C_DATA_COMPAT_S
{
    HI_U32    I2cNum;
    HI_U8     I2cDevAddr;
    HI_U32    I2cRegAddr;
    HI_U32    I2cRegCount;
    HI_U32    pData;
    HI_U32    DataLen;
} I2C_DATA_COMPAT_S;

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

/* Ioctl definitions */
#define CMD_I2C_READ _IOW(HI_ID_I2C, 0x1, HI_U32) // 1:check keyup
#define CMD_I2C_WRITE _IOW(HI_ID_I2C, 0x2, HI_U32) // 1:check repkey, 0:hardware behave
#define CMD_I2C_SET_RATE _IOW(HI_ID_I2C, 0x3, HI_U32)
#define CMD_I2C_CONFIG _IOWR(HI_ID_I2C, 0x4, I2C_GPIO_S) //config  gpioi2c
#define CMD_I2C_DESTROY _IOW(HI_ID_I2C, 0x5, HI_U32) //destroy gpioi2c

#endif /* End of #ifndef __DRV_I2C_IOCTL_H__*/
