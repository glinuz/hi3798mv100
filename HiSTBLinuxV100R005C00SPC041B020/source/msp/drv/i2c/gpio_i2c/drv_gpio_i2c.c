/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/spinlock.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/unistd.h>
#include <linux/compat.h>

#include "hi_kernel_adapt.h"
#include "hi_common.h"
#include "drv_i2c_ioctl.h"
#include "drv_gpio_ioctl.h"
#include "drv_gpioi2c_ext.h"
#include "drv_gpioi2c.h"
#include "hi_drv_gpioi2c.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "hi_drv_sys.h"
#include "hi_drv_gpio.h"
#if  defined(CHIP_TYPE_hi3718mv100)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR,HI_GPIO_7_ADDR,HI_GPIO_8_ADDR};

#elif defined(CHIP_TYPE_hi3719mv100)|| defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR,HI_GPIO_7_ADDR,HI_GPIO_8_ADDR,HI_GPIO_9_ADDR};

#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR};

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR,HI_GPIO_7_ADDR,HI_GPIO_8_ADDR,HI_GPIO_9_ADDR,\
                                                    HI_GPIO_10_ADDR};

#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR,HI_GPIO_7_ADDR,HI_GPIO_8_ADDR,HI_GPIO_9_ADDR,\
                                                    HI_GPIO_10_ADDR,HI_GPIO_11_ADDR,HI_GPIO_12_ADDR};

#elif  defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719cv100)

static HI_U32 g_GpioI2cPhyAddr[HI_GPIO_MAX_GROUP_NUM]={HI_GPIO_0_ADDR,HI_GPIO_1_ADDR,HI_GPIO_2_ADDR,HI_GPIO_3_ADDR,HI_GPIO_4_ADDR,\
                                                    HI_GPIO_5_ADDR,HI_GPIO_6_ADDR,HI_GPIO_7_ADDR,HI_GPIO_8_ADDR,HI_GPIO_9_ADDR,\
                                                    HI_GPIO_10_ADDR,HI_GPIO_11_ADDR,HI_GPIO_12_ADDR,HI_GPIO_13_ADDR,HI_GPIO_14_ADDR,\
                                                    HI_GPIO_15_ADDR,HI_GPIO_16_ADDR,HI_GPIO_17_ADDR};

#endif

static DEFINE_SPINLOCK(gpio_i2c_lock);

static HI_U8 *g_u32GpioRegAddrs[HI_GPIO_MAX_GROUP_NUM];

I2C_GPIO_S g_stI2cGpio[HI_I2C_MAX_NUM];

typedef struct hiI2C_DATA_GPIO_S
{
    HI_U32              I2cChannel;
    HI_U32              u32GpioI2CSCL; /*which GPIO will be defined I2CSCL*/
    HI_U32              u32GpioI2CSDA; /* which GPIO will be defined I2CSDA*/
    HI_U8              *u32GpioDirClk; /*I2CSCL direction register*/
    HI_U8              *u32GpioDirData; /*I2CSDA direction register*/
    HI_U8              *u32GpioI2CSdaReg; /*SDA GPIO  relation data buffer register,indicate high or low level about input or output(u32GpioDirData confirm the direction)*/
    HI_U8              *u32GpioI2CSclReg; /*SCL GPIO indicate high or low level about input or output(u32GpioDirData confirm the direction)*/
} I2C_DATA_GPIO_S;

static I2C_DATA_GPIO_S g_stI2cDataGpio[HI_I2C_MAX_NUM];

//static unsigned int   g_u32Random =0 ;
int i2cmode;

/*1: read, S:DevAddr:A:RegAddr:A:S:DevAddr:RecData:Stop
       write, S:DevAddr:A:RegAddr:A:WriteData:Stop
   2: read, S:DevAddr:A:RegAddr:A:Dealy:Stop:S:DevAddr:A:RecData:NoA:S
       write, S:DevAddr:A:RegAddr:A:WriteData:S*/

#define HI_FATAL_GPIOI2C(fmt...) \
    HI_FATAL_PRINT(HI_ID_GPIO_I2C, fmt)

#define HI_ERR_GPIOI2C(fmt...) \
    HI_ERR_PRINT(HI_ID_GPIO_I2C, fmt)

#define HI_WARN_GPIOI2C(fmt...) \
    HI_WARN_PRINT(HI_ID_GPIO_I2C, fmt)

#define HI_INFO_GPIOI2C(fmt...) \
    HI_INFO_PRINT(HI_ID_GPIO_I2C, fmt)

#define HW_REG(reg) * ((volatile unsigned int *)(reg))
#define DELAY(I2cNum, us) time_delay_us(I2cNum, us)

struct semaphore gpio_i2_sem;

static GPIO_I2C_EXT_FUNC_S g_stGpioI2cExtFuncs =
{
    .pfnGpioI2cIOCtl           = HI_DRV_GPIOI2C_Ioctl,
#ifdef CONFIG_COMPAT		
    .pfnGpioI2cCompatIOCtl     = HI_DRV_GPIOI2C_Compat_Ioctl,
#endif
    //.pfnGpioI2cConfig         = DRV_GPIOI2C_Config,
    .pfnGpioI2cWrite           = HI_DRV_GPIOI2C_Write,
    .pfnGpioI2cRead            = HI_DRV_GPIOI2C_Read,
    .pfnGpioI2cWriteExt        = HI_DRV_GPIOI2C_WriteExt,
    .pfnGpioI2cReadExt         = HI_DRV_GPIOI2C_ReadExt,
    .pfnGpioI2cReadExtDiRectly = HI_DRV_GPIOI2C_ReadExtDirectly,
    .pfnGpioI2cWriteExtNOSTOP  = HI_DRV_GPIOI2C_WriteExtNoSTOP,

    //.pfnGpioI2cClose          = DRV_GPIOI2C_Close,
    .pfnGpioI2cSCCBRead        = HI_DRV_GPIOI2C_SCCBRead,
    //.pfnGpioI2cTunerSendData  = gpio_i2c_senddata_tuner,
    //.pfnGpioI2cTunerReceiveData = gpio_i2c_receivedata_tuner,
    .pfnGpioI2cCreateChannel   = HI_DRV_GPIOI2C_CreateGpioI2c,
    .pfnGpioI2cDestroyChannel  = HI_DRV_GPIOI2C_DestroyGpioI2c,
    .pfnGpioI2cIsUsed          = DRV_GPIOI2C_IsUsed,
};

/*
 * I2C by GPIO simulated  clear 0 routine.
 *
 * @param whichline: GPIO control line
 *
 */
static void i2c_clr(int I2cNum, unsigned char whichline)
{
    unsigned char regvalue;

    if (whichline == g_stI2cDataGpio[I2cNum].u32GpioI2CSCL)
    {
        regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirClk);
        regvalue |= g_stI2cDataGpio[I2cNum].u32GpioI2CSCL;
        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirClk) = regvalue;

        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioI2CSclReg) = 0;
        return;
    }
    else if (whichline == g_stI2cDataGpio[I2cNum].u32GpioI2CSDA)
    {
        regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
        regvalue |= g_stI2cDataGpio[I2cNum].u32GpioI2CSDA;
        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioI2CSdaReg) = 0;
        return;
    }
    else
    {
        HI_ERR_GPIOI2C("i2c_clr Error input.\n");
        return;
    }
}

/*
 * I2C by GPIO simulated  set 1 routine.
 *
 * @param whichline: GPIO control line
 *
 */
static void i2c_set(int I2cNum, unsigned char whichline)
{
    unsigned char regvalue;

    if (whichline == g_stI2cDataGpio[I2cNum].u32GpioI2CSCL)
    {
        regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirClk);
        regvalue |= g_stI2cDataGpio[I2cNum].u32GpioI2CSCL;
        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirClk) = regvalue;

        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioI2CSclReg) = g_stI2cDataGpio[I2cNum].u32GpioI2CSCL;
        return;
    }
    else if (whichline == g_stI2cDataGpio[I2cNum].u32GpioI2CSDA)
    {
        regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
        regvalue |= g_stI2cDataGpio[I2cNum].u32GpioI2CSDA;
        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioI2CSdaReg) = g_stI2cDataGpio[I2cNum].u32GpioI2CSDA;
        return;
    }
    else
    {
        HI_ERR_GPIOI2C("i2c_set Error input.\n");
        return;
    }
}

/*
 *  delays for a specified number of micro seconds rountine.
 *
 *  @param usec: number of micro seconds to pause for
 *
 */
static void time_delay_us(int I2cNum, unsigned int usec)
{
    unsigned int i;

    for (i = 0; i < usec; i++)
    {
        udelay(1);
    }
}

/*
 * I2C by GPIO simulated  read data routine.
 *
 * @return value: a bit for read
 *
 */

static unsigned char i2c_data_read(int I2cNum)
{
    unsigned char regvalue;

    regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
    regvalue &= (~g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

    //  DELAY(I2cNum, 1);

    regvalue = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioI2CSdaReg);
    if ((regvalue & g_stI2cDataGpio[I2cNum].u32GpioI2CSDA) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * sends a start bit via I2C rountine.
 *
 */
static void i2c_start_bit(int I2cNum)
{
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    DELAY(I2cNum, 2);
}

/*
 * sends a stop bit via I2C rountine.
 *
 */
static void i2c_stop_bit(int I2cNum)
{
    /* clock the ack */

    //    DELAY(I2cNum, 1);
    //    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    //    DELAY(I2cNum, 1);
    //    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);

    /* actual stop bit */

    //    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    DELAY(I2cNum, 1);
}

/*
 * sends a character over I2C rountine.
 *
 * @param  c: character to send
 *
 */
static void i2c_send_byte(int I2cNum, unsigned char c)
{
    int i;
    HI_SIZE_T IrqFlags;

    spin_lock_irqsave(&gpio_i2c_lock, IrqFlags);
    for (i = 0; i < 8; i++)
    {
        i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
        DELAY(I2cNum, 1);

        if (c & (1 << (7 - i)))
        {
            i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
        }
        else
        {
            i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
        }

        DELAY(I2cNum, 1);
        i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
        DELAY(I2cNum, 1);
        DELAY(I2cNum, 1);
        i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    }

    DELAY(I2cNum, 1);
    spin_unlock_irqrestore(&gpio_i2c_lock, IrqFlags);
}

/*  receives a character from I2C rountine.
 *
 *  @return value: character received
 *
 */
static unsigned char i2c_receive_byte(int I2cNum)
{
    int j = 0;
    int i;
    unsigned char regvalue;
    HI_SIZE_T IrqFlags;

    spin_lock_irqsave(&gpio_i2c_lock, IrqFlags);
    for (i = 0; i < 8; i++)
    {
        DELAY(I2cNum, 1);
        i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
        DELAY(I2cNum, 2);
        i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);

        regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
        regvalue &= (~g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
        HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;
        DELAY(I2cNum, 1);

        if (i2c_data_read(I2cNum))
        {
            j += (1 << (7 - i));
        }

        DELAY(I2cNum, 1);
        i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    }

    spin_unlock_irqrestore(&gpio_i2c_lock, IrqFlags);

    DELAY(I2cNum, 1);

    return j;
}

/*  receives an acknowledge from I2C rountine.
 *
 *  @return value: 0--Ack received; 1--Nack received
 *
 */
static int i2c_receive_ack(int I2cNum)
{
    int nack;
    unsigned char regvalue;

    DELAY(I2cNum, 1);

    regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
    regvalue &= (~g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);

    nack = i2c_data_read(I2cNum);

    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 2);

    if (nack == 0)
    {
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_GPIOI2C("receive Err ack = 1 \n");
        return HI_FAILURE;
    }
}

/*
 * sends an acknowledge over I2C rountine.
 *
 */
static void i2c_send_ack(int I2cNum)
{
    unsigned char regvalue;

    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);

    //    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
    regvalue &= (~g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

    DELAY(I2cNum, 2);
}

static void i2c_send_noack(int I2cNum)
{
    unsigned char regvalue;

    //  HI_ERR_GPIOI2C("i2c_send_ack DELAY3 = %d \n", 3);

    DELAY(I2cNum, 1);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    DELAY(I2cNum, 1);
    i2c_set(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 2);
    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    DELAY(I2cNum, 1);

    //    i2c_clr(I2cNum, g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    regvalue  = HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData);
    regvalue &= (~g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    HW_REG(g_stI2cDataGpio[I2cNum].u32GpioDirData) = regvalue;

    DELAY(I2cNum, 1);
}

/*
 *  read data from the I2C bus by GPIO simulated of a device rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *
 *  @return value: data from the device readed
 *
 */

HI_S32 HI_DRV_GPIOI2C_Read(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data)
{
    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
        return HI_FAILURE;;
    }

    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_NULL == pu8Data) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
        up(&gpio_i2_sem);
        return HI_FAILURE;
    }

    i2c_start_bit(u32I2cNum);

    i2c_send_byte(u32I2cNum, (unsigned char)(u8Devaddr));
    i2c_receive_ack(u32I2cNum);

    i2c_send_byte(u32I2cNum, u8RegAddress);
    i2c_receive_ack(u32I2cNum);

    if (2 == i2cmode)
    {
        i2c_stop_bit(u32I2cNum);
        DELAY(u32I2cNum, 5);
    }

    i2c_start_bit(u32I2cNum);

    i2c_send_byte(u32I2cNum, (unsigned char)(u8Devaddr) | 1);
    i2c_receive_ack(u32I2cNum);

    *pu8Data = i2c_receive_byte(u32I2cNum);
    //i2c_send_ack();   /* tw2864 DO NOT need this */
    if (2 == i2cmode)
    {
        i2c_send_noack(u32I2cNum);
    }

    i2c_stop_bit(u32I2cNum);

    up(&gpio_i2_sem);
    HI_INFO_GPIOI2C("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, data0=0x%x\n",
                    u32I2cNum, u8Devaddr, u8RegAddress, *pu8Data);

    return HI_SUCCESS;
}

static HI_S32 DRV_GPIOI2C_ReadData(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen,
                                   HI_BOOL bSendRegAddress, HI_U8 *pData, HI_U32 DataLen)
{
    HI_U32 i = 0;
    HI_U32 ret; //TC MODIFY
    HI_U8 reval = 0;
    HI_U8 RegAddr;

    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C("Semaphore lock is   error. \n");
        return HI_FAILURE;
    }

    if ((I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[I2cNum].bUsed))
    {
        HI_ERR_GPIOI2C("the I2cNum haven't requested ,can't read operation.\n");
        up(&gpio_i2_sem);
        return HI_FAILURE;
    }

    if (bSendRegAddress)
    {
        //send start condition
        i2c_start_bit(I2cNum);

        //send slave device address
        i2c_send_byte(I2cNum, (unsigned char)(devaddress));

        //TC MODIFY START
        ret = i2c_receive_ack(I2cNum);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_GPIOI2C("i2c not receive ack!\n");
            up(&gpio_i2_sem);
            return HI_FAILURE;
        }

        //TC MODIFY END

        //send register address
        for (i = 0; i < (addresslen); i++)
        {
            RegAddr = address >> ((addresslen - i - 1) * 8);

            i2c_send_byte(I2cNum, RegAddr);
            i2c_receive_ack(I2cNum);
        }
    }

    //send start condition
    i2c_start_bit(I2cNum);

    //send slave device address
    i2c_send_byte(I2cNum, (unsigned char)(devaddress) | 1);
    i2c_receive_ack(I2cNum);

    //receive data
    for (i = 0; i < DataLen; i++)
    {
        reval = i2c_receive_byte(I2cNum);

        *(pData + i) = reval;

        if (i == DataLen - 1)
        {
            i2c_send_noack(I2cNum);
        }
        else
        {
            i2c_send_ack(I2cNum);
        }
    }

    //send stop condition
    i2c_stop_bit(I2cNum);
    up(&gpio_i2_sem);

    HI_INFO_GPIOI2C("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, Num=%d, Len=%d, data0=0x%x\n", I2cNum, devaddress,
                    address, addresslen, DataLen, *pData);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIOI2C_ReadExt(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                              HI_U32 DataLen)
{
    return DRV_GPIOI2C_ReadData(I2cNum, devaddress, address, addresslen, HI_TRUE, pData, DataLen);
}

HI_S32 HI_DRV_GPIOI2C_ReadExtDirectly(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                                      HI_U32 DataLen)
{
    return DRV_GPIOI2C_ReadData(I2cNum, devaddress, address, addresslen, HI_FALSE, pData, DataLen);
}

#if 0
HI_S32 gpio_i2c_read_ext(int I2cNum, unsigned char devaddress, unsigned char address, unsigned char *rdval)
{
    int rxdata;

    if (!g_stI2cGpio[I2cNum].bUsed)
    {
        HI_ERR_GPIOI2C("gpio_i2c_read failed(I2cNum=%d not valid)! \n", I2cNum);
        return HI_FAILURE;
    }

    /* semaphore P */
    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    i2c_start_bit(I2cNum);
    i2c_send_byte(I2cNum, (unsigned char)(devaddress));
    i2c_receive_ack(I2cNum);
    i2c_send_byte(I2cNum, address);
    i2c_receive_ack(I2cNum);

    if (2 == i2cmode)
    {
        i2c_stop_bit(I2cNum);
        DELAY(I2cNum, 5);
    }

    i2c_start_bit(I2cNum);
    i2c_send_byte(I2cNum, (unsigned char)(devaddress) | 1);
    i2c_receive_ack(I2cNum);
    rxdata = i2c_receive_byte(I2cNum);
    //i2c_send_ack();   /* tw2864 DO NOT need this */
    if (2 == i2cmode)
    {
        i2c_send_noack(I2cNum);
    }

    i2c_stop_bit(I2cNum);
    *rdval = rxdata;
    up(&gpio_i2_sem);

    return 0;
}

#endif

/*
 *  writes data to a device on the I2C bus rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *  @param  data:   data for write to device
 *
 */

HI_S32 HI_DRV_GPIOI2C_Write(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 u8Data)
{
    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
        return HI_FAILURE;;
    }

    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
        up(&gpio_i2_sem);
        return HI_FAILURE;
    }

    HI_INFO_GPIOI2C("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, data0=0x%x\n",
                    u32I2cNum, u8Devaddr, u8RegAddress, u8Data);

    i2c_start_bit(u32I2cNum);

    i2c_send_byte(u32I2cNum, (unsigned char)(u8Devaddr));
    i2c_receive_ack(u32I2cNum);

    i2c_send_byte(u32I2cNum, u8RegAddress);
    i2c_receive_ack(u32I2cNum);

    i2c_send_byte(u32I2cNum, u8Data);
    if (2 == i2cmode)
    {
        i2c_receive_ack(u32I2cNum);
        DELAY(u32I2cNum, 10);
    }

    i2c_stop_bit(u32I2cNum);
    up(&gpio_i2_sem);

    return HI_SUCCESS;
}

static HI_S32 DRV_GPIOI2C_WriteData(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                                    HI_U32 DataLen, HI_BOOL bSendStopCondtion)
{
    HI_U32 i;
    HI_U8 RegAddr;

    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if ((I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[I2cNum].bUsed))
    {
        HI_ERR_GPIOI2C("the I2cNum haven't requested ,can't write operation.\n");
        up(&gpio_i2_sem);
        return HI_FAILURE;
    }

    HI_INFO_GPIOI2C("I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, Num=%d, Len=%d, data0=0x%x\n", I2cNum, devaddress, address,
                    addresslen, DataLen, pData[0]);

    i2c_start_bit(I2cNum);

    i2c_send_byte(I2cNum, (unsigned char)(devaddress));
    i2c_receive_ack(I2cNum);

    for (i = 0; i < (addresslen); i++)
    {
        RegAddr = address >> ((addresslen - i - 1) * 8);

        i2c_send_byte(I2cNum, (unsigned char)RegAddr);
        i2c_receive_ack(I2cNum);
    }

    for (i = 0; i < DataLen; i++)
    {
        i2c_send_byte(I2cNum, pData[i]);
        i2c_receive_ack(I2cNum);
    }

    if (bSendStopCondtion)
    {
        i2c_stop_bit(I2cNum);
    }

    up(&gpio_i2_sem);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIOI2C_WriteExt(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                               HI_U32 DataLen)
{
    return DRV_GPIOI2C_WriteData(I2cNum, devaddress, address, addresslen, pData, DataLen, HI_TRUE);
}

HI_S32 HI_DRV_GPIOI2C_WriteExtNoSTOP(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                                     HI_U32 DataLen)
{
    return DRV_GPIOI2C_WriteData(I2cNum, devaddress, address, addresslen, pData, DataLen, HI_FALSE);
}

#if 0
HI_S32 gpio_i2c_write_ext(int I2cNum, unsigned char devaddress, unsigned char address, unsigned char data)
{
    if (!g_stI2cGpio[I2cNum].bUsed)
    {
        HI_ERR_GPIOI2C("gpio_i2c_write failed(I2cNum=%d not valid)! \n", I2cNum);
        return HI_FAILURE;
    }

    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    i2c_start_bit(I2cNum);
    i2c_send_byte(I2cNum, (unsigned char)(devaddress));
    i2c_receive_ack(I2cNum);
    i2c_send_byte(I2cNum, address);
    i2c_receive_ack(I2cNum);
    i2c_send_byte(I2cNum, data);
    if (2 == i2cmode)
    {
        i2c_receive_ack(I2cNum);
        DELAY(I2cNum, 10);
    }

    i2c_stop_bit(I2cNum);
    up(&gpio_i2_sem);

    return 0;
}

#endif

/*
 *  read data from the I2C bus by GPIO simulated of a digital camera device rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *
 */

HI_S32 HI_DRV_GPIOI2C_SCCBRead(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data)
{
    if (down_interruptible(&gpio_i2_sem))
    {
        HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
        return HI_FAILURE;;
    }

    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_NULL == pu8Data) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
        up(&gpio_i2_sem);
        return HI_FAILURE;
    }

    i2c_start_bit(u32I2cNum);
    i2c_send_byte(u32I2cNum, (unsigned char)(u8Devaddr));
    i2c_receive_ack(u32I2cNum);
    i2c_send_byte(u32I2cNum, u8RegAddress);
    i2c_receive_ack(u32I2cNum);
    i2c_stop_bit(u32I2cNum);
    i2c_start_bit(u32I2cNum);
    i2c_send_byte(u32I2cNum, (unsigned char)(u8Devaddr) | 1);
    i2c_receive_ack(u32I2cNum);
    *pu8Data = i2c_receive_byte(u32I2cNum);
    i2c_send_ack(u32I2cNum);
    i2c_stop_bit(u32I2cNum);
    up(&gpio_i2_sem);

    return HI_SUCCESS;
}

#if 0
int gpio_i2c_senddata_tuner(unsigned int portnum, unsigned char devaddress,
                            i2c_data_tuner_s *p_data)
{
    unsigned int i;

    if ((0 == portnum) || (1 == portnum))
    {
        if (down_interruptible(&gpio_i2_sem))
        {
            HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
            return HI_FAILURE;
        }

        i2c_start_bit(portnum);
        i2c_send_byte(portnum, (unsigned char)(devaddress));
        i2c_receive_ack(portnum);

        i2c_send_byte(portnum, *(p_data->pu8SendBuf));
        i2c_receive_ack(portnum);

        for (i = 1; i < (p_data->u32SendLength); i++)
        {
            i2c_send_byte(portnum, *(p_data->pu8SendBuf + i));
            i2c_receive_ack(portnum);
        }

        i2c_stop_bit(portnum);
        up(&gpio_i2_sem);
    }
    else
    {
        HI_ERR_GPIOI2C("fail");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int gpio_i2c_receivedata_tuner(unsigned int portnum,
                               unsigned char devaddress, i2c_data_tuner_s *p_data)
{
    int i = 0;
    int j = 0;
    int reval = 0;

    if ((0 == portnum) || (1 == portnum))
    {
        {
            if (down_interruptible(&gpio_i2_sem))
            {
                HI_ERR_GPIOI2C( "Semaphore lock is  error. \n");
                return HI_FAILURE;
            }

            i2c_start_bit(portnum);
            i2c_send_byte(portnum, (unsigned char)(devaddress));
            i2c_receive_ack(portnum);

            i2c_send_byte(portnum, *(p_data->pu8SendBuf));
            i2c_receive_ack(portnum);

            for (j = 1; j < p_data->u32SendLength; j++)
            {
                i2c_send_byte(portnum, *(p_data->pu8SendBuf + i));
                i2c_receive_ack(portnum);
            }

            i2c_stop_bit(portnum);
        }

        {
            i2c_start_bit(portnum);
            i2c_send_byte(portnum, (unsigned char)(devaddress) | 1);
            i2c_receive_ack(portnum);

            for (i = 0; i < p_data->u32ReceiveLength; i++)
            {
                reval = i2c_receive_byte(portnum);
                *(p_data->pu8ReceiveBuf + i) = reval;
                i2c_send_ack(portnum);
            }

            i2c_stop_bit(portnum);
            up(&gpio_i2_sem);
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#endif

HI_S32 DRV_GPIOI2C_IsUsed(HI_U32 I2cNum, HI_BOOL *bUsed)
{
    if ((I2cNum > HI_I2C_MAX_NUM_USER) || (HI_NULL == bUsed))
    {
        return HI_FAILURE;
    }

    *bUsed = g_stI2cGpio[I2cNum].bUsed;
    return HI_SUCCESS;
}

// TODO: porting g_stI2cGpio member to this function
HI_S32 DRV_GPIOI2C_Config(HI_U32 I2cNum, HI_U32 whichGpioClock, HI_U32 whichGpioData, HI_U32 clockBit, HI_U32 dataBit)
{
    HI_U8 *u32GpioBaseClock;
    HI_U8 *u32GpioBaseData;

    /*inner function, parameters is checked before this function is called*/
#if 0
    if (whichGpioClock >= HI_GPIO_GROUP_NUM)
    {
        HI_ERR_GPIOI2C("error: clock gpio id(%d) is invalid!\n", whichGpioClock);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (whichGpioData >= HI_GPIO_GROUP_NUM)
    {
        HI_ERR_GPIOI2C("error: data gpio id(%d) is invalid!\n", whichGpioData);
        return HI_ERR_GPIO_INVALID_PARA;
    }
#endif


    u32GpioBaseClock = g_u32GpioRegAddrs[whichGpioClock];
    u32GpioBaseData = g_u32GpioRegAddrs[whichGpioData];

    g_stI2cDataGpio[I2cNum].I2cChannel = I2cNum;
    g_stI2cDataGpio[I2cNum].u32GpioI2CSCL = (1 << clockBit);
    g_stI2cDataGpio[I2cNum].u32GpioI2CSDA = (1 << dataBit);

    g_stI2cDataGpio[I2cNum].u32GpioDirData = u32GpioBaseData + HI_GPIO_DIR_REG;
    g_stI2cDataGpio[I2cNum].u32GpioDirClk = u32GpioBaseClock + HI_GPIO_DIR_REG;
    g_stI2cDataGpio[I2cNum].u32GpioI2CSdaReg = (u32GpioBaseData + ((g_stI2cDataGpio[I2cNum].u32GpioI2CSDA) << 2));
    g_stI2cDataGpio[I2cNum].u32GpioI2CSclReg = (u32GpioBaseClock + ((g_stI2cDataGpio[I2cNum].u32GpioI2CSCL) << 2));

    HI_INFO_GPIOI2C("DRV_GPIOI2C_Config OK(I2cNum=%d, SCL=gpio%d_%d, SDA=gpio%d_%d, i2cmode=%d).\n", I2cNum,
                    whichGpioClock, clockBit, whichGpioData, dataBit, i2cmode);

    g_stI2cGpio[I2cNum].I2cNum = I2cNum;
    g_stI2cGpio[I2cNum].u32SCLGpioNo = whichGpioClock * HI_GPIO_BIT_NUM + clockBit;
    g_stI2cGpio[I2cNum].u32SDAGpioNo = whichGpioData * HI_GPIO_BIT_NUM + dataBit;
    g_stI2cGpio[I2cNum].bUsed = HI_TRUE;
    g_stI2cGpio[I2cNum].u32Count = 1;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIOI2C_CreateGpioI2c(HI_U32 *pu32I2cNum, HI_U32 u32SCLGpioNo, HI_U32 u32SDAGpioNo)
{
    HI_S32 Ret;
    HI_U32 i, ValidI2cNum;
    HI_U32 TmpGpioClkNo;
    HI_U32 TmpGpioSDANo;
    HI_U32 whichGpioClock;
    HI_U32 whichGpioData;
    HI_U32 clockBit;
    HI_U32 dataBit;

    whichGpioClock = u32SCLGpioNo / HI_GPIO_BIT_NUM;
    clockBit = u32SCLGpioNo % HI_GPIO_BIT_NUM;
    whichGpioData = u32SDAGpioNo / HI_GPIO_BIT_NUM;
    dataBit = u32SDAGpioNo % HI_GPIO_BIT_NUM;

#if  defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
    if ( (whichGpioClock >= HI_GPIO_GROUP_NUM)||(whichGpioClock==0)||((whichGpioClock>=15)&&(whichGpioClock<=17))\
        ||(whichGpioData >= HI_GPIO_GROUP_NUM)||(whichGpioData==0)||((whichGpioData>=15)&&(whichGpioData<=17))\
        || (u32SCLGpioNo == u32SDAGpioNo)
       )
    {
        HI_ERR_GPIOI2C("error: GPIO NO. %d and NO. %d is invalid!\n", u32SCLGpioNo, u32SDAGpioNo);
        return HI_ERR_GPIO_INVALID_PARA;
    }
#else
    if (   (whichGpioClock >= HI_GPIO_GROUP_NUM)
        || (whichGpioData >= HI_GPIO_GROUP_NUM)
        || (u32SCLGpioNo == u32SDAGpioNo) )
    {
        HI_ERR_GPIOI2C("error: GPIO NO. %d and NO. %d is invalid!\n", u32SCLGpioNo, u32SDAGpioNo);
        return HI_ERR_GPIO_INVALID_PARA;
    }
#endif
    /*whether the gpio have used in other I2cNum  */
    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++)
    {
        if (g_stI2cGpio[i].bUsed)
        {
            TmpGpioClkNo  = g_stI2cGpio[i].u32SCLGpioNo;
            TmpGpioSDANo = g_stI2cGpio[i].u32SDAGpioNo;

            if ((TmpGpioClkNo == u32SCLGpioNo) && (TmpGpioSDANo == u32SDAGpioNo))
            {
                g_stI2cGpio[i].u32Count++;
                *pu32I2cNum = g_stI2cGpio[i].I2cNum;

                return HI_SUCCESS;
            }

            if ((u32SCLGpioNo == TmpGpioClkNo) || (u32SDAGpioNo == TmpGpioClkNo))
            {
                HI_ERR_GPIOI2C("GPIO NO. %d is used to GpioClock!\n", TmpGpioClkNo);
                return HI_ERR_GPIO_INVALID_PARA;
            }

            if ((u32SCLGpioNo == TmpGpioSDANo) || (u32SDAGpioNo == TmpGpioSDANo))
            {
                HI_ERR_GPIOI2C("GPIO NO. %d is used to GpioData!\n", TmpGpioSDANo);
                return HI_ERR_GPIO_INVALID_PARA;
            }
        }
    }

    /*find a idle i2c channel*/
    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++)
    {
        if (HI_FALSE == g_stI2cGpio[i].bUsed)
        {
            ValidI2cNum = i;
            break;
        }
    }

    if (i >= HI_I2C_MAX_NUM)
    {
        HI_ERR_GPIOI2C("i2c channel all  have used ,request i2c channel fail  !\n");
        return HI_FAILURE;
    }

    /* the gpio can be use*/
    Ret = DRV_GPIOI2C_Config(ValidI2cNum, whichGpioClock, whichGpioData, clockBit, dataBit);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_GPIOI2C("config gpio i2c failure !\n");
        return HI_FAILURE;
    }

    *pu32I2cNum = ValidI2cNum;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIOI2C_DestroyGpioI2c(HI_U32 I2cNum)
{
    if (I2cNum >= HI_I2C_MAX_NUM)
    {
        return HI_ERR_I2C_MALLOC_ERR;
    }

    if (HI_FALSE == g_stI2cGpio[I2cNum].bUsed)
    {
        HI_ERR_GPIOI2C("u32I2cNum = %d is not used!\n", I2cNum);
        return HI_FAILURE;
    }

    if ((--g_stI2cGpio[I2cNum].u32Count) == 0)
    {
        g_stI2cGpio[I2cNum].bUsed = HI_FALSE;
    }

    HI_INFO_GPIOI2C("gpio_i2c_destroy OK(I2cNum=%d, SCL=gpio No.%d, SDA=gpio No.%d).\n", I2cNum,
                    g_stI2cGpio[I2cNum].u32SCLGpioNo, g_stI2cGpio[I2cNum].u32SDAGpioNo);

    return HI_SUCCESS;
}

/*************************************************************************/
HI_DECLARE_MUTEX(gpioI2c_Mutex);

long HI_DRV_GPIOI2C_Ioctl(void *file, unsigned int cmd, unsigned long arg)
{
    long    Ret;
    HI_U8  *pData = NULL;
    I2C_DATA_S I2cData;
    I2C_GPIO_S I2cGpio;
    void __user *argp = (void __user*)arg;

    Ret = down_interruptible(&gpioI2c_Mutex);
    if (Ret) {
        HI_ERR_GPIOI2C("lock g_I2cMutex error.\n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
        case CMD_I2C_WRITE:
            if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            if (I2cData.DataLen > HI_I2C_MAX_LENGTH)
            {
                Ret = HI_ERR_I2C_INVALID_PARA;
                break;
            }

            pData = HI_KMALLOC(HI_ID_GPIO_I2C, I2cData.DataLen, GFP_KERNEL);
            if (!pData)
            {
                HI_ERR_GPIOI2C("i2c kmalloc fail!\n");
                Ret = HI_ERR_I2C_MALLOC_ERR;
                break;
            }

            if (copy_from_user(pData, I2cData.pData, I2cData.DataLen))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                HI_KFREE(HI_ID_GPIO_I2C, pData);
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            Ret = HI_DRV_GPIOI2C_WriteExt(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr, I2cData.I2cRegCount,
                                          pData, I2cData.DataLen);
            HI_KFREE(HI_ID_GPIO_I2C, pData);
            break;

        case CMD_I2C_READ:
            if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            if (I2cData.DataLen > HI_I2C_MAX_LENGTH)
            {
                Ret = HI_ERR_I2C_INVALID_PARA;
                break;
            }

            pData = HI_KMALLOC(HI_ID_GPIO_I2C, I2cData.DataLen, GFP_KERNEL);
            if (!pData)
            {
                HI_ERR_GPIOI2C("i2c kmalloc fail!\n");
                Ret = HI_ERR_I2C_MALLOC_ERR;
                break;
            }

            Ret = HI_DRV_GPIOI2C_ReadExt(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr, I2cData.I2cRegCount, pData,
                                         I2cData.DataLen);
            if (HI_SUCCESS == Ret)
            {
                if (copy_to_user(I2cData.pData, pData, I2cData.DataLen))
                {
                    HI_ERR_GPIOI2C("copy data to user fail!\n");
                    Ret = HI_ERR_I2C_COPY_DATA_ERR;
                }
            }

            HI_KFREE(HI_ID_GPIO_I2C, pData);
            break;

        case CMD_I2C_CONFIG:
            if (copy_from_user(&I2cGpio, argp, sizeof(I2C_GPIO_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_FAILURE;
                break;
            }

            Ret = HI_DRV_GPIOI2C_CreateGpioI2c(&(I2cGpio.I2cNum), I2cGpio.u32SCLGpioNo, I2cGpio.u32SDAGpioNo);
            if (HI_SUCCESS == Ret)
            {
                if (copy_to_user(argp, (void *)&I2cGpio, sizeof(I2C_GPIO_S)))
                {
                    HI_ERR_GPIOI2C("copy data to user fail!\n");
                    Ret = HI_ERR_I2C_COPY_DATA_ERR;
                }
            }

            break;

        case CMD_I2C_DESTROY:
            if (copy_from_user(&I2cGpio, argp, sizeof(I2C_GPIO_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_FAILURE;
                break;
            }

            Ret = HI_DRV_GPIOI2C_DestroyGpioI2c(I2cGpio.I2cNum);

            break;

        default:
            up(&gpioI2c_Mutex);
            return -ENOIOCTLCMD;
    }

    up(&gpioI2c_Mutex);
    return Ret;
}

#ifdef CONFIG_COMPAT
long HI_DRV_GPIOI2C_Compat_Ioctl(void *file, unsigned int cmd, unsigned long arg)
{
    long    Ret;
    HI_U8  *pData = NULL;
    I2C_DATA_COMPAT_S I2cData;
    I2C_GPIO_S I2cGpio;
    void __user *argp = (void __user*)arg;

    Ret = down_interruptible(&gpioI2c_Mutex);
    if (Ret) {
        HI_ERR_GPIOI2C("lock g_I2cMutex error.\n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
        case CMD_I2C_WRITE:
            if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_COMPAT_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            if (I2cData.DataLen > HI_I2C_MAX_LENGTH)
            {
                Ret = HI_ERR_I2C_INVALID_PARA;
                break;
            }

            pData = HI_KMALLOC(HI_ID_GPIO_I2C, I2cData.DataLen, GFP_KERNEL);
            if (!pData)
            {
                HI_ERR_GPIOI2C("i2c kmalloc fail!\n");
                Ret = HI_ERR_I2C_MALLOC_ERR;
                break;
            }

            if (copy_from_user(pData, (HI_U8 *)compat_ptr(I2cData.pData), I2cData.DataLen))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                HI_KFREE(HI_ID_GPIO_I2C, pData);
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            Ret = HI_DRV_GPIOI2C_WriteExt(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr, I2cData.I2cRegCount,
                                          pData, I2cData.DataLen);
            HI_KFREE(HI_ID_GPIO_I2C, pData);
            break;

        case CMD_I2C_READ:
            if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_ERR_I2C_COPY_DATA_ERR;
                break;
            }

            if (I2cData.DataLen > HI_I2C_MAX_LENGTH)
            {
                Ret = HI_ERR_I2C_INVALID_PARA;
                break;
            }

            pData = HI_KMALLOC(HI_ID_GPIO_I2C, I2cData.DataLen, GFP_KERNEL);
            if (!pData)
            {
                HI_ERR_GPIOI2C("i2c kmalloc fail!\n");
                Ret = HI_ERR_I2C_MALLOC_ERR;
                break;
            }

            Ret = HI_DRV_GPIOI2C_ReadExt(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr, I2cData.I2cRegCount, pData,
                                         I2cData.DataLen);
            if (HI_SUCCESS == Ret)
            {
                if (copy_to_user((HI_U8 *)compat_ptr(I2cData.pData), pData, I2cData.DataLen))
                {
                    HI_ERR_GPIOI2C("copy data to user fail!\n");
                    Ret = HI_ERR_I2C_COPY_DATA_ERR;
                }
            }

            HI_KFREE(HI_ID_GPIO_I2C, pData);
            break;

        case CMD_I2C_CONFIG:
            if (copy_from_user(&I2cGpio, argp, sizeof(I2C_GPIO_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_FAILURE;
                break;
            }

            Ret = HI_DRV_GPIOI2C_CreateGpioI2c(&(I2cGpio.I2cNum), I2cGpio.u32SCLGpioNo, I2cGpio.u32SDAGpioNo);
            if (HI_SUCCESS == Ret)
            {
                if (copy_to_user(argp, (void *)&I2cGpio, sizeof(I2C_GPIO_S)))
                {
                    HI_ERR_GPIOI2C("copy data to user fail!\n");
                    Ret = HI_ERR_I2C_COPY_DATA_ERR;
                }
            }

            break;

        case CMD_I2C_DESTROY:
            if (copy_from_user(&I2cGpio, argp, sizeof(I2C_GPIO_S)))
            {
                HI_ERR_GPIOI2C("copy data from user fail!\n");
                Ret = HI_FAILURE;
                break;
            }

            Ret = HI_DRV_GPIOI2C_DestroyGpioI2c(I2cGpio.I2cNum);

            break;

        default:
            up(&gpioI2c_Mutex);
            return -ENOIOCTLCMD;
    }

    up(&gpioI2c_Mutex);
    return Ret;
}
#endif

#ifdef GPIO_I2C_PM
int gpio_i2c_suspend (PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_PRINT("GPIO_I2C suspend OK\n");
    return HI_SUCCESS;
}

int gpio_i2c_resume(int I2cNum, PM_BASEDEV_S *pdev)
{
    i2c_set(g_stI2cDataGpio[I2cNum].u32GpioI2CSCL);
    i2c_set(g_stI2cDataGpio[I2cNum].u32GpioI2CSDA);
    HI_PRINT("GPIO_I2C resume OK\n");
    return HI_SUCCESS;
}

#endif

static unsigned int gpioinitialized = 0;

/*
 * initializes I2C interface routine.
 *
 * @return value:0--success; 1--error.
 *
 */
HI_S32 HI_DRV_GPIOI2C_Init(HI_VOID)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_DRV_MODULE_Register(HI_ID_GPIO_I2C, "HI_GPIO_I2C", (HI_VOID *)&g_stGpioI2cExtFuncs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_GPIOI2C(" GPIO_I2C Module register failed 0x%x.\n", s32Ret);
    }

    if (gpioinitialized == 0)
    {
        HI_INIT_MUTEX(&gpio_i2_sem);
        gpioinitialized = 1;

        for(i = 0; i < HI_GPIO_MAX_GROUP_NUM; i++)
        {
            g_u32GpioRegAddrs[i] = (HI_U8 *)ioremap_nocache(g_GpioI2cPhyAddr[i], HI_GPIO_ADDR_SIZE);
            if (g_u32GpioRegAddrs[i] == HI_NULL)
            {
                HI_WARN_GPIO("ioremap_nocache err! \n");
                return HI_FAILURE;
            }
        }

        for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++)
        {
            g_stI2cGpio[i].bUsed = HI_FALSE;
            g_stI2cGpio[i].u32Count = 0;
        }
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_GPIOI2C_DeInit(HI_VOID)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_DRV_MODULE_UnRegister(HI_ID_GPIO_I2C);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_GPIOI2C(" GPIO_I2C Module unregister failed 0x%x.\n", s32Ret);
    }

    gpioinitialized = 0;

    for (i = 0; i < HI_GPIO_MAX_GROUP_NUM; i++)
    {
        iounmap(g_u32GpioRegAddrs[i]);
    }

    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++)
    {
        g_stI2cGpio[i].bUsed = HI_FALSE;
        g_stI2cGpio[i].u32Count = 0;
    }
}

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(HI_DRV_GPIOI2C_Init);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_DeInit);
#if 1

//EXPORT_SYMBOL(HI_DRV_GPIOI2C_Ioctl);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_Write);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_Read);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_WriteExt);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_ReadExt);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_ReadExtDirectly);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_WriteExtNoSTOP);

//EXPORT_SYMBOL(DRV_GPIOI2C_Close);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_SCCBRead);
//EXPORT_SYMBOL(gpio_i2c_senddata_tuner);
//EXPORT_SYMBOL(gpio_i2c_receivedata_tuner);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_CreateGpioI2c);
EXPORT_SYMBOL(HI_DRV_GPIOI2C_DestroyGpioI2c);

//EXPORT_SYMBOL(DRV_GPIOI2C_IsUsed);
#endif
EXPORT_SYMBOL(i2cmode);
EXPORT_SYMBOL(g_stI2cGpio);
