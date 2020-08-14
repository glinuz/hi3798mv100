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

#include <uboot.h>
#include "hi_drv_gpioi2c.h"
#include "hi_common.h"
#include "hi_reg.h"
#include "hi_reg_common.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_i2c.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "delay.h"
#endif


#define GPIO_0_BASE     (0XF8B20000)
#define GPIO_1_BASE     (0XF8B21000)
#define GPIO_2_BASE     (0XF8B22000)
#define GPIO_3_BASE     (0XF8B23000)
#define GPIO_4_BASE     (0XF8B24000)

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)	\
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv200_a) \
	|| defined(CHIP_TYPE_hi3798cv200)
#define GPIO_5_BASE     (0xF8004000)
#else
#define GPIO_5_BASE     (0XF8B25000)
#endif

#define GPIO_6_BASE     (0XF8B26000)
#define GPIO_7_BASE     (0XF8B27000)

#if  defined(CHIP_TYPE_hi3718mv100)
#define GPIO_8_BASE     (0XF8B28000)
#define     HI_GPIO_MAX_GROUP_NUM   9

#elif  defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3798mv100)
#define     HI_GPIO_MAX_GROUP_NUM   7

#elif  defined(CHIP_TYPE_hi3719mv100)|| defined(CHIP_TYPE_hi3798mv200)|| defined(CHIP_TYPE_hi3798mv200_a) 
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define     HI_GPIO_MAX_GROUP_NUM   10

#elif  defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define GPIO_10_BASE    (0XF8B2A000)
#define     HI_GPIO_MAX_GROUP_NUM   11

#elif  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define GPIO_10_BASE    (0XF8B2A000)
#define GPIO_11_BASE    (0XF8B2B000)
#define GPIO_12_BASE    (0XF8B2C000)
#define GPIO_13_BASE    (0XF8B2D000)
#define GPIO_14_BASE    (0XF8B2E000)
#define GPIO_15_BASE    (0XF8B2F000)
#define GPIO_16_BASE    (0XF8B30000)
#define GPIO_17_BASE    (0XF8B31000)
#define     HI_GPIO_MAX_GROUP_NUM   18

#elif defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define GPIO_10_BASE    (0XF8B2A000)
#define GPIO_11_BASE    (0XF8B2B000)
#define GPIO_12_BASE    (0XF8B2C000)
#define GPIO_13_BASE    (0XF8B2D000)
#define GPIO_14_BASE    (0XF8B2E000)
#define GPIO_15_BASE    (0XF8B2F000)
#define GPIO_16_BASE    (0XF8B30000)
#define     HI_GPIO_MAX_GROUP_NUM   17

#elif defined(CHIP_TYPE_hi3798cv200)
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define GPIO_10_BASE    (0XF8B2A000)
#define GPIO_11_BASE    (0XF8B2B000)
#define GPIO_12_BASE    (0XF8B2C000)
#define     HI_GPIO_MAX_GROUP_NUM   13


#elif defined(CHIP_TYPE_hi3796cv100)    \
   || defined(CHIP_TYPE_hi3798cv100)
#define GPIO_8_BASE     (0XF8B28000)
#define GPIO_9_BASE     (0XF8B29000)
#define GPIO_10_BASE    (0XF8B2A000)
#define GPIO_11_BASE    (0XF8B2B000)
#define GPIO_12_BASE    (0XF8B2C000)
#define GPIO_13_BASE    (0XF8B2D000)
#define GPIO_14_BASE    (0XF8B2E000)
#define GPIO_15_BASE    (0XF8B2F000)
#define GPIO_16_BASE    (0XF8B30000)
#define GPIO_17_BASE    (0XF8B31000)
#define GPIO_18_BASE    (0XF8B04000)
#define GPIO_19_BASE    (0XF8B09000)
#define     HI_GPIO_MAX_GROUP_NUM   20
#else
#define     HI_GPIO_MAX_GROUP_NUM   8
#endif

#define     HI_GPIO_DIR_REG     0x400
#define HI_GPIO_BIT_NUM     8


#define HI_I2C_MAX_NUM_USER (15)

#define HI_I2C_MAX_NUM_RESERVE 1
#define HI_I2C_MAX_NUM (HI_I2C_MAX_NUM_USER + HI_I2C_MAX_NUM_RESERVE)

static int g_u32GpioRegAddrs[HI_GPIO_MAX_GROUP_NUM] = {0};

I2C_GPIO_S g_stI2cGpio[HI_I2C_MAX_NUM];

typedef struct hiI2C_DATA_GPIO_S
{
    HI_U32              I2cChannel;
    HI_U32              u32GpioI2CSCL; /*which GPIO will be defined I2CSCL*/
    HI_U32              u32GpioI2CSDA; /* which GPIO will be defined I2CSDA*/
    HI_U32              u32GpioDirClk; /*I2CSCL direction register*/
    HI_U32              u32GpioDirData; /*I2CSDA direction register*/
    HI_U32              u32GpioI2CSdaReg; /*SDA GPIO  relation data buffer register,indicate high or low level about input or output(u32GpioDirData confirm the direction)*/
    HI_U32              u32GpioI2CSclReg; /*SCL GPIO indicate high or low level about input or output(u32GpioDirData confirm the direction)*/
} I2C_DATA_GPIO_S;

static I2C_DATA_GPIO_S g_stI2cDataGpio[HI_I2C_MAX_NUM];

int i2cmode;

/*1: read, S:DevAddr:A:RegAddr:A:S:DevAddr:RecData:Stop
       write, S:DevAddr:A:RegAddr:A:WriteData:Stop
   2: read, S:DevAddr:A:RegAddr:A:Dealy:Stop:S:DevAddr:A:RecData:NoA:S
       write, S:DevAddr:A:RegAddr:A:WriteData:S*/

#define HW_REG(reg) * ((volatile unsigned int *)(reg))
#define DELAY(I2cNum, us) time_delay_us(I2cNum, us)

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
        I2C_GPIO_DEGUB("i2c_clr Error input.\n");
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
        I2C_GPIO_DEGUB("i2c_set Error input.\n");
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
        I2C_GPIO_DEGUB("receive Err ack = 1 \n");
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

    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_NULL == pu8Data) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
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

    I2C_GPIO_DEGUB("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, data0=0x%x\n",
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

    if ((I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[I2cNum].bUsed))
    {
        I2C_GPIO_DEGUB("the I2cNum haven't requested ,can't read operation.\n");
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
            I2C_GPIO_DEGUB("i2c not receive ack!\n");
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

    I2C_GPIO_DEGUB("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, Num=%d, Len=%d, data0=0x%x\n", I2cNum, devaddress,
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

    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
        return HI_FAILURE;
    }

    I2C_GPIO_DEGUB("\t I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, data0=0x%x\n",
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

    return HI_SUCCESS;
}

static HI_S32 DRV_GPIOI2C_WriteData(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData,
                                    HI_U32 DataLen, HI_BOOL bSendStopCondtion)
{
    HI_U32 i;
    HI_U8 RegAddr;

    if ((I2cNum >= HI_I2C_MAX_NUM) || (HI_FALSE == g_stI2cGpio[I2cNum].bUsed))
    {
        I2C_GPIO_DEGUB("the I2cNum haven't requested ,can't write operation.\n");
        return HI_FAILURE;
    }

    I2C_GPIO_DEGUB("I2cNum=%d, DevAddr=0x%x, RegAddr=0x%x, Num=%d, Len=%d, data0=0x%x\n", I2cNum, devaddress, address,
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


/*
 *  read data from the I2C bus by GPIO simulated of a digital camera device rountine.
 *
 *  @param  devaddress:  address of the device
 *  @param  address: address of register within device
 *
 */

HI_S32 HI_DRV_GPIOI2C_SCCBRead(HI_U32 u32I2cNum, HI_U8 u8Devaddr, HI_U8 u8RegAddress, HI_U8 *pu8Data)
{
    if ((u32I2cNum >= HI_I2C_MAX_NUM) || (HI_NULL == pu8Data) || (HI_FALSE == g_stI2cGpio[u32I2cNum].bUsed))
    {
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

    return HI_SUCCESS;
}


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
    unsigned int u32GpioBaseClock, u32GpioBaseData;

    /*inner function, parameters is checked before this function is called*/

    u32GpioBaseClock = g_u32GpioRegAddrs[whichGpioClock];
    u32GpioBaseData = g_u32GpioRegAddrs[whichGpioData];

    g_stI2cDataGpio[I2cNum].I2cChannel = I2cNum;
    g_stI2cDataGpio[I2cNum].u32GpioI2CSCL = (1 << clockBit);
    g_stI2cDataGpio[I2cNum].u32GpioI2CSDA = (1 << dataBit);

    g_stI2cDataGpio[I2cNum].u32GpioDirData = u32GpioBaseData + HI_GPIO_DIR_REG;
    g_stI2cDataGpio[I2cNum].u32GpioDirClk = u32GpioBaseClock + HI_GPIO_DIR_REG;
    g_stI2cDataGpio[I2cNum].u32GpioI2CSdaReg = (u32GpioBaseData + ((g_stI2cDataGpio[I2cNum].u32GpioI2CSDA) << 2));
    g_stI2cDataGpio[I2cNum].u32GpioI2CSclReg = (u32GpioBaseClock + ((g_stI2cDataGpio[I2cNum].u32GpioI2CSCL) << 2));

    I2C_GPIO_DEGUB("DRV_GPIOI2C_Config OK(I2cNum=%d, SCL=gpio%d_%d, SDA=gpio%d_%d, i2cmode=%d).\n", I2cNum,
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
	if ( (whichGpioClock >= HI_GPIO_MAX_GROUP_NUM)||(whichGpioClock==0)||((whichGpioClock>=15)&&(whichGpioClock<=17))\
			||(whichGpioData >= HI_GPIO_MAX_GROUP_NUM)||(whichGpioData==0)||((whichGpioData>=15)&&(whichGpioData<=17))\
			|| (u32SCLGpioNo == u32SDAGpioNo) 
	    )   
	{
		I2C_GPIO_DEGUB("error: GPIO NO. %d and NO. %d is invalid!\n", u32SCLGpioNo, u32SDAGpioNo);
		return HI_ERR_GPIO_INVALID_PARA;
	}
#else
    if (   (whichGpioClock >= HI_GPIO_MAX_GROUP_NUM)
        || (whichGpioData >= HI_GPIO_MAX_GROUP_NUM)
        || (u32SCLGpioNo == u32SDAGpioNo) )
    {
        I2C_GPIO_DEGUB("error: GPIO NO. %d and NO. %d is invalid!\n", u32SCLGpioNo, u32SDAGpioNo);
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
                I2C_GPIO_DEGUB("GPIO NO. %d is used to GpioClock!\n", TmpGpioClkNo);
                return HI_ERR_GPIO_INVALID_PARA;
            }

            if ((u32SCLGpioNo == TmpGpioSDANo) || (u32SDAGpioNo == TmpGpioSDANo))
            {
                I2C_GPIO_DEGUB("GPIO NO. %d is used to GpioData!\n", TmpGpioSDANo);
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
        I2C_GPIO_DEGUB("i2c channel all  have used ,request i2c channel fail  !\n");
        return HI_FAILURE;
    }

    /* the gpio can be use*/
    Ret = DRV_GPIOI2C_Config(ValidI2cNum, whichGpioClock, whichGpioData, clockBit, dataBit);
    if (HI_SUCCESS != Ret)
    {
        I2C_GPIO_DEGUB("config gpio i2c failure !\n");
        return HI_FAILURE;
    }

    *pu32I2cNum = ValidI2cNum;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIOI2C_DestroyGpioI2c(HI_U32 I2cNum)
{
    if (HI_FALSE == g_stI2cGpio[I2cNum].bUsed)
    {
        I2C_GPIO_DEGUB("u32I2cNum = %d is not used!\n", I2cNum);
        return HI_FAILURE;
    }

    if ((--g_stI2cGpio[I2cNum].u32Count) == 0)
    {
        g_stI2cGpio[I2cNum].bUsed = HI_FALSE;
    }

    I2C_GPIO_DEGUB("gpio_i2c_destroy OK(I2cNum=%d, SCL=gpio No.%d, SDA=gpio No.%d).\n", I2cNum,
                    g_stI2cGpio[I2cNum].u32SCLGpioNo, g_stI2cGpio[I2cNum].u32SDAGpioNo);

    return HI_SUCCESS;
}

/*************************************************************************/

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

    if (gpioinitialized == 0)
    {
        gpioinitialized = 1;

        g_u32GpioRegAddrs[0] = GPIO_0_BASE;
        g_u32GpioRegAddrs[1] = GPIO_1_BASE;
        g_u32GpioRegAddrs[2] = GPIO_2_BASE;
        g_u32GpioRegAddrs[3] = GPIO_3_BASE;
        g_u32GpioRegAddrs[4] = GPIO_4_BASE;
        g_u32GpioRegAddrs[5] = GPIO_5_BASE;

    #if defined(CHIP_TYPE_hi3718mv100)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
    #elif defined(CHIP_TYPE_hi3719mv100)||defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
	#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
		g_u32GpioRegAddrs[10] = GPIO_10_BASE;
	
	#elif  defined(CHIP_TYPE_hi3796mv100)||defined(CHIP_TYPE_hi3798mv100)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
    #elif  defined(CHIP_TYPE_hi3716cv200)   \
        || defined(CHIP_TYPE_hi3716mv400)   \
        || defined(CHIP_TYPE_hi3718cv100)   \
        || defined(CHIP_TYPE_hi3719cv100)
        g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
        g_u32GpioRegAddrs[10] = GPIO_10_BASE;
        g_u32GpioRegAddrs[11] = GPIO_11_BASE;
        g_u32GpioRegAddrs[12] = GPIO_12_BASE;
        g_u32GpioRegAddrs[13] = GPIO_13_BASE;
        g_u32GpioRegAddrs[14] = GPIO_14_BASE;
        g_u32GpioRegAddrs[15] = GPIO_15_BASE;
        g_u32GpioRegAddrs[16] = GPIO_16_BASE;
        g_u32GpioRegAddrs[17] = GPIO_17_BASE;
	#elif  defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
        g_u32GpioRegAddrs[10] = GPIO_10_BASE;
        g_u32GpioRegAddrs[11] = GPIO_11_BASE;
        g_u32GpioRegAddrs[12] = GPIO_12_BASE;
        g_u32GpioRegAddrs[13] = GPIO_13_BASE;
        g_u32GpioRegAddrs[14] = GPIO_14_BASE;
        g_u32GpioRegAddrs[15] = GPIO_15_BASE;
        g_u32GpioRegAddrs[16] = GPIO_16_BASE;
	#elif  defined(CHIP_TYPE_hi3798cv200)
		g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
        g_u32GpioRegAddrs[10] = GPIO_10_BASE;
        g_u32GpioRegAddrs[11] = GPIO_11_BASE;
        g_u32GpioRegAddrs[12] = GPIO_12_BASE;
    #elif  defined(CHIP_TYPE_hi3796cv100)   \
        || defined(CHIP_TYPE_hi3798cv100)
        g_u32GpioRegAddrs[6] = GPIO_6_BASE;
        g_u32GpioRegAddrs[7] = GPIO_7_BASE;
        g_u32GpioRegAddrs[8] = GPIO_8_BASE;
        g_u32GpioRegAddrs[9] = GPIO_9_BASE;
        g_u32GpioRegAddrs[10] = GPIO_10_BASE;
        g_u32GpioRegAddrs[11] = GPIO_11_BASE;
        g_u32GpioRegAddrs[12] = GPIO_12_BASE;
        g_u32GpioRegAddrs[13] = GPIO_13_BASE;
        g_u32GpioRegAddrs[14] = GPIO_14_BASE;
        g_u32GpioRegAddrs[15] = GPIO_15_BASE;
        g_u32GpioRegAddrs[16] = GPIO_16_BASE;
        g_u32GpioRegAddrs[17] = GPIO_17_BASE;
        g_u32GpioRegAddrs[18] = GPIO_18_BASE;
        g_u32GpioRegAddrs[19] = GPIO_19_BASE;
    #endif

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

    gpioinitialized = 0;

    for (i = HI_STD_I2C_NUM; i < HI_I2C_MAX_NUM; i++)
    {
        g_stI2cGpio[i].bUsed = HI_FALSE;
        g_stI2cGpio[i].u32Count = 0;
    }
}

