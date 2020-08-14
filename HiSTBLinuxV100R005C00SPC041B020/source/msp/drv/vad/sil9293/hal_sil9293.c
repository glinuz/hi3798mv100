/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hal_sil9293.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/03/08
  Description   :
  History       :
  1.Date        : 2013/03/08
    Author      : t00202585,j00169368
    Modification: Created file

******************************************************************************/

#include <linux/delay.h>
#include <asm/io.h>

#include "hal_sil9293.h"
#include "drv_sil9293.h"
#include "hi_drv_module.h"
#include "drv_gpio_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_i2c_ext.h"
#include "hi_drv_i2c.h"


#define ABS(x) ((x) >= 0 ? (x) : (-(x)))

#define HDMIRX_I2C_RATE_50K    50000   /*50k*/

static I2C_EXT_FUNC_S* gsHdmiRxI2cFunc   = HI_NULL;
static GPIO_I2C_EXT_FUNC_S* pstGpioI2cExt = HI_NULL;

static HI_U8 HDMIRX_FST_DEV = 0x64;
HI_BOOL gbHdmiRxSwI2c = HI_FALSE;
HI_U32 g_u32HdmiRxI2cNum = 3;  //I2C_3

HI_U32 gpioScl = 20; //gpio2_4
HI_U32 gpioSda = 19; //gpio2_3
HI_U32 gpioRst = 81; //gpio2_0
HI_U32 gpioInt = 82; //gpio2_1
bool bLowCi2ca = 1; //CI2CA Signal, affect System Control and Status REGISTER

//static inline
HI_VOID SIL9293_Read(HI_U32 u32Dev, HI_U32 u32Addr, HI_U8* pu8Value)
{
    HI_S32 s32Ret;
    if (HI_TRUE == gbHdmiRxSwI2c)
    {
        s32Ret = pstGpioI2cExt->pfnGpioI2cReadExt(g_u32HdmiRxI2cNum, u32Dev, u32Addr, 1, pu8Value, 1);
    }
    else
    {
        s32Ret = gsHdmiRxI2cFunc->pfnI2cRead(g_u32HdmiRxI2cNum, (HI_U8)u32Dev, u32Addr, 1, pu8Value, 1);
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("I2cNum:%d, u32Dev:%#x, u32Addr%#x, u8Value:%#x SIL9293_Read failed!\n",
                      g_u32HdmiRxI2cNum, u32Dev, u32Addr, *pu8Value);
    }
}

//static inline
HI_VOID SIL9293_Write(HI_U32 u32Dev, HI_U32 u32Addr, HI_U8 u8Value)
{
    HI_S32 s32Ret;
    if (HI_TRUE == gbHdmiRxSwI2c)
    {
        s32Ret = pstGpioI2cExt->pfnGpioI2cWriteExt(g_u32HdmiRxI2cNum, u32Dev, u32Addr, 1, &u8Value, 1);
    }
    else
    {
        s32Ret = gsHdmiRxI2cFunc->pfnI2cWrite(g_u32HdmiRxI2cNum, u32Dev, u32Addr, 1, &u8Value, 1);
    }
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("I2cNum:%d, u32Dev:%#x, u32Addr%#x, u8Value:%#x SIL9293_Write failed!\n",
                      g_u32HdmiRxI2cNum, u32Dev, u32Addr, u8Value);
    }
}

static inline
HI_VOID SIL9293_Read2B(HI_U32 u32Dev, HI_U32 u32Addr, HI_U32* pu32Value)
{
    HI_U32 i, u32Cnt = 0;
    HI_S32 s32Ret;
    HI_U16 u16Tmp;
    HI_U16 u16TmpLast = 0;

    for (i = 0; i < 100; i++)
    {
        if (HI_TRUE == gbHdmiRxSwI2c)
        {
            s32Ret = pstGpioI2cExt->pfnGpioI2cReadExt(g_u32HdmiRxI2cNum, u32Dev, u32Addr, 1, (HI_U8*)(&u16Tmp), 2);
        }
        else
        {
            s32Ret = gsHdmiRxI2cFunc->pfnI2cRead(g_u32HdmiRxI2cNum, u32Dev, u32Addr, 1, (HI_U8*)(&u16Tmp), 2);
        }
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_HDMIRX("I2cNum:%d, u32Dev:%#x, u32Addr%#x, u8Value:%#x SIL9293_Read failed!\n",
                          g_u32HdmiRxI2cNum, u32Dev, u32Addr, u16Tmp);
            i = 0;
            continue;
        }

        if (ABS(u16TmpLast - u16Tmp) < 5)
        {
            u32Cnt++;

            if (u32Cnt > 3)
            {
                break;
            }
        }
        else
        {
            u32Cnt = 0;
            u16TmpLast = u16Tmp;
        }
    }

    if (i == 100)
    {
        HI_ERR_HDMIRX("SIL9293_Read2B read err\n");
    }

    *pu32Value = (HI_U32)(u16Tmp);
}

HI_S32 SIL9293_HAL_Init(HI_VOID)
{
    HI_S32 s32Ret;
    GPIO_EXT_FUNC_S* pstGpioExt = HI_NULL;

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&pstGpioExt);
    if ((HI_NULL == pstGpioExt) || (HI_SUCCESS != s32Ret))
    {
        HI_ERR_HDMIRX("Get Function from GPIO failed.\n");
        return HI_FAILURE;
    }

    if (HI_STD_I2C_NUM < g_u32HdmiRxI2cNum)
    {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&pstGpioI2cExt);
        if ((HI_NULL == pstGpioI2cExt) || (HI_SUCCESS != s32Ret))
        {
            HI_ERR_HDMIRX("Get Function from GPIO_I2C failed.\n");
            return HI_FAILURE;
        }

        s32Ret = pstGpioI2cExt->pfnGpioI2cCreateChannel(&g_u32HdmiRxI2cNum, gpioScl, gpioSda);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_HDMIRX("pfnGpioI2cCreateChannel failed\n");
            return s32Ret;
        }
        gbHdmiRxSwI2c = HI_TRUE;
        
        HI_INFO_HDMIRX("SW I2c Num:%d\n",g_u32HdmiRxI2cNum);   
    }
    else
    {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_I2C, (HI_VOID**)&gsHdmiRxI2cFunc);
        if ((HI_SUCCESS != s32Ret) || !gsHdmiRxI2cFunc \
            || !gsHdmiRxI2cFunc->pfnI2cWrite \
            || !gsHdmiRxI2cFunc->pfnI2cRead \
            || !gsHdmiRxI2cFunc->pfnI2cReadDirectly \
            || !gsHdmiRxI2cFunc->pfnI2cWriteNostop \
            || !gsHdmiRxI2cFunc->pfnI2cSetRate)
        {
            HI_ERR_HDMIRX("I2C not found\n");
            return HI_FAILURE;
        }
        s32Ret = gsHdmiRxI2cFunc->pfnI2cSetRate(g_u32HdmiRxI2cNum, HDMIRX_I2C_RATE_50K);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_HDMIRX("I2C RATE 50K failed:%d\n", s32Ret);
            return HI_FAILURE;
        }
        gbHdmiRxSwI2c = HI_FALSE;
        
        HI_INFO_HDMIRX("HW I2c Num:%d\n",g_u32HdmiRxI2cNum);   
    }
    if (bLowCi2ca == 1)
    {
        HDMIRX_FST_DEV = 0x64;
    }
    else
    {
        HDMIRX_FST_DEV = 0x66;
    }

    s32Ret = pstGpioExt->pfnGpioDirSetBit(gpioRst, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("pfnGpioDirSetBit failed\n");
        return HI_FAILURE;
    }

    s32Ret = pstGpioExt->pfnGpioWriteBit(gpioRst, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("pfnGpioWriteBit failed\n");
        return HI_FAILURE;
    }

    msleep(20);
    s32Ret = pstGpioExt->pfnGpioWriteBit(gpioRst, 1);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("pfnGpioWriteBit failed\n");
        return HI_FAILURE;
    }

    msleep(20);

    s32Ret = pstGpioExt->pfnGpioDirSetBit(gpioInt, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("pfnGpioDirSetBit failed\n");
        return HI_FAILURE;
    }


    return HI_SUCCESS;
}

HI_VOID SIL9293_HAL_DeInit(HI_VOID)
{
    if (HI_TRUE == gbHdmiRxSwI2c)
    {
        HI_S32 s32Ret;
        GPIO_I2C_EXT_FUNC_S* pstGpioI2cExt = HI_NULL;

        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&pstGpioI2cExt);
        if ((HI_NULL == pstGpioI2cExt) || (HI_SUCCESS != s32Ret))
        {
            HI_ERR_HDMIRX("Get Function from GPIO_I2C failed.\n");
            return;
        }

        (void)pstGpioI2cExt->pfnGpioI2cDestroyChannel(g_u32HdmiRxI2cNum);
    }
    return;
}

HI_VOID SIL9293_HAL_ClearInt(HI_VOID)
{
    SIL9293_Write(HDMIRX_FST_DEV, 0x74, 0xFF);
}

HI_VOID SIL9293_HAL_Reset(HI_VOID)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x05, &u8Value);

    /* 0x05, AACRST:[5] HDCPRST:[3] ACRRST:[2] FIFORST:[1] SWRST:[0] */
    u8Value = u8Value | (BIT0 | BIT1 | BIT2 | BIT3 | BIT5);
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);

    msleep(20);

    u8Value = u8Value & (~(BIT0 | BIT1 | BIT2 | BIT3 | BIT5));
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);

    SIL9293_HAL_ResetDcFIFO();

    msleep(60);

    /* 0x05, SWRST_AUTO:[4] */
    u8Value = u8Value | BIT4;
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);
    /*set Bt1120 */
    SIL9293_HAL_SetBt656Enable(HI_FALSE);
}

HI_VOID SIL9293_HAL_ResetDcFIFO(HI_VOID)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x07, &u8Value);

    /* 0x07, DCFIFORST:[7] */
    u8Value = u8Value | BIT7;
    SIL9293_Write(HDMIRX_FST_DEV, 0x07, u8Value);

    msleep(20);

    u8Value = u8Value & (~BIT7);
    SIL9293_Write(HDMIRX_FST_DEV, 0x07, u8Value);

    msleep(60);
}

HI_VOID SIL9293_HAL_ResetHdcp(HI_VOID)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x05, &u8Value);

    /* 0x05, HDCPRST:[3] */
    u8Value = u8Value | BIT3;
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);

    msleep(20);

    u8Value = u8Value & (~BIT3);
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);
}

HI_BOOL SIL9293_HAL_HasAvi(HI_VOID)
{
    HI_U8 u8Value;

    /* 0x74 NO_AVI:[4] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x74, &u8Value);
    u8Value = u8Value & BIT4;

    if (u8Value)
    {
        SIL9293_Write(HDMIRX_FST_DEV, 0x74, 0x10);

        /* no AVI */
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_VOID SIL9293_HAL_UpOffset(HI_BOOL bRGB)
{
    if (bRGB)
    {
        SIL9293_Write(HDMIRX_FST_DEV, 0x4B, 0x0);
        SIL9293_Write(HDMIRX_FST_DEV, 0x4C, 0x0);
        SIL9293_Write(HDMIRX_FST_DEV, 0x4D, 0x0);
    }
    else
    {
        /* CbYCr: 10bit output, with BGR sequence */
        SIL9293_Write(HDMIRX_FST_DEV, 0x4B, 0x80);
        SIL9293_Write(HDMIRX_FST_DEV, 0x4C, 0xF);
        SIL9293_Write(HDMIRX_FST_DEV, 0x4D, 0x80);
    }

    SIL9293_Write(HDMIRX_FST_DEV, 0x4B, 0x0);
    SIL9293_Write(HDMIRX_FST_DEV, 0x4C, 0x0);
    SIL9293_Write(HDMIRX_FST_DEV, 0x4D, 0x0);
}

HI_VOID SIL9293_HAL_UpTmdsDcCtl(HI_VOID)
{
    HI_U8 u8Value;
    HI_PIXEL_BITWIDTH_E enBitWidth;

    SIL9293_HAL_GetColorDepth(&enBitWidth);

    SIL9293_Read(HDMIRX_FST_DEV, 0x81, &u8Value);
    u8Value = u8Value & BIT5;

    switch (enBitWidth)
    {
        case HI_PIXEL_BITWIDTH_8BIT:
            u8Value = u8Value | 0x0;
            break;
        case 1:
            u8Value = u8Value | 0x4;
            break;
        case 2:
            u8Value = u8Value | 0x5;
            break;
        default:
            u8Value = u8Value | 0x0;
            break;
    }

    SIL9293_Write(HDMIRX_FST_DEV, 0x81, u8Value);
}

HI_VOID SIL9293_HAL_SetPowerOn(HI_BOOL bPowerOn)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x08, &u8Value);

    /* 0x08 PD#:[0] */
    if (bPowerOn)
    {
        u8Value = u8Value | BIT0;
    }
    else
    {
        u8Value = u8Value & (~BIT0);
    }

    SIL9293_Write(HDMIRX_FST_DEV, 0x08, u8Value);
}

HI_VOID SIL9293_HAL_SetVideoAuto(HI_VOID)
{
    HI_U8 u8Value;

    /* 0x5F VID_AOF[7:0] Digital 4:2:2 output = 0xF0  0xC0 */
    SIL9293_Write(HDMIRX_FST_DEV, 0x5F, 0xF0);

    /* 0x4A INSSYNC[7] embended sync 0x80*/
    SIL9293_Write(HDMIRX_FST_DEV, 0x4A, 0x80);

    /* 0x08 BSEL[2] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x08, &u8Value);
    u8Value = u8Value | BIT2;
    SIL9293_Write(HDMIRX_FST_DEV, 0x08, u8Value);

    /* 0x49 DITHER_MODE[7:6] 0b01 = Dither to 8 bits*/
    SIL9293_Read(HDMIRX_FST_DEV, 0x49, &u8Value);
    u8Value = (u8Value & (~(BIT6 | BIT7))) ;
    SIL9293_Write(HDMIRX_FST_DEV, 0x49, u8Value);

    /* 0x4A DITH[5] 1=DITH ENABLE*/
    SIL9293_Read(HDMIRX_FST_DEV, 0x4A, &u8Value);
    u8Value = u8Value | BIT5;
    u8Value = u8Value | BIT1;
    SIL9293_Write(HDMIRX_FST_DEV, 0x4A, u8Value);

    /* 0x81 OFFSET_COMP_EN[5] 0 = Disable */
    SIL9293_Read(HDMIRX_FST_DEV, 0x81, &u8Value);
    u8Value = u8Value & (~BIT5);

    //u8Value = u8Value|BIT5;
    SIL9293_Write(HDMIRX_FST_DEV, 0x81, u8Value);

    SIL9293_Write(HDMIRX_RXC_DEV, 0x6c, 0x3F);
    SIL9293_Write(HDMIRX_RXC_DEV, 0x70, 0xC8);

    /* 0xB5 AVC_EN[2] 1 = AutoVideoControl Enable */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value | BIT2;

    //u8Value = 0xe0;//u8Value&(~BIT2);
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);
}

HI_VOID SIL9293_HAL_SetDDCEn(HI_BOOL bDDCEn)
{
    HI_U8 u8Value;

    /* 0x09 DDC0_EN[4] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x09, &u8Value);
    u8Value = u8Value | BIT4;
    SIL9293_Write(HDMIRX_FST_DEV, 0x09, u8Value);
}

HI_VOID SIL9293_HAL_SetHPG(HI_BOOL bHigh)
{
    if (bHigh)
    {
        SIL9293_Write(HDMIRX_FST_DEV, 0x0B, 1);
    }
    else
    {
        SIL9293_Write(HDMIRX_FST_DEV, 0x0B, 0);
    }
}

HI_VOID SIL9293_HAL_SetTMDSEn(HI_BOOL bTMDSEn)
{
    HI_U8 u8Value;

    /* 0x09 RX0_EN[0] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x09, &u8Value);

    if (bTMDSEn)
    {
        u8Value = u8Value | BIT0;
    }
    else
    {
        u8Value = u8Value & (~BIT0);
    }

    SIL9293_Write(HDMIRX_FST_DEV, 0x09, u8Value);
}

HI_VOID SIL9293_HAL_GetPixClk10KHZ(HI_U32* pu32PixClk)
{
    HI_U32 u32Xcnt = 0;
    HI_U32 u32PixClk;

    /* XCLK_IN_PCLK 0x6E[7:0], 0x6F[3:0]--[11:8]*/
    SIL9293_Read2B(HDMIRX_FST_DEV, 0x6E, &u32Xcnt);
    u32Xcnt = u32Xcnt & 0xFFF;

    if (0 == u32Xcnt)
    {
        *pu32PixClk = 0;
        return;
    }

    u32PixClk  = 4194304UL; //4196000UL; /* 2048*2048 27M XTAL*/
    u32PixClk /= u32Xcnt;

    if (u32PixClk > 65535)
    {
        u32PixClk = 65535;
    }

    *pu32PixClk = u32PixClk;
}

HI_VOID SIL9293_HAL_GetStable(HI_BOOL* pbStable)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x7B, &u8Value);

    /* 0x7B VRCHG:[4],HRCHG:[3] */
    if (u8Value & (BIT3 | BIT4))
    {

        *pbStable = HI_FALSE;
        SIL9293_Write(HDMIRX_FST_DEV, 0x7B, (BIT3 | BIT4));
    }
    else
    {
        *pbStable = HI_TRUE;
    }
}

HI_VOID SIL9293_HAL_GetClock(HI_BOOL* pbHasClock)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x06, &u8Value);

    /* 0x06 CKDT:[1],SCDT:[0] */
    if ((u8Value & BIT1) && (u8Value & BIT0))
    {
        *pbHasClock = HI_TRUE;
    }
    else
    {
        *pbHasClock = HI_FALSE;
    }
}

HI_VOID SIL9293_HAL_GetWH(HI_U32* pu32W, HI_U32* pu32H)
{
    HI_U32 u32Tmp = 0;

    /* DE_PIX 0x4E[7:0], 0x4F[3:0]--[11:8]*/
    SIL9293_Read2B(HDMIRX_FST_DEV, 0x4E, &u32Tmp);
    *pu32W = u32Tmp & 0xFFF;

    /* DE_LIN 0x50[7:0], 0x51[2:0]--[11:8]*/
    SIL9293_Read2B(HDMIRX_FST_DEV, 0x50, &u32Tmp);
    *pu32H = u32Tmp & 0xFFF;
}

HI_VOID SIL9293_HAL_GetHVTotal(HI_U32* pu32HTotal,
                               HI_U32* pu32VTotal)
{
    HI_OVERSAMPLE_MODE_E u8ICLK;
    HI_U32 u32HTotal = 0;
    HI_U32 u32VTotal = 0;

    SIL9293_HAL_GetICLK(&u8ICLK);

    /* H_RES 0x3A[7:0], 0x3B[4:0]--[12:8]*/
    SIL9293_Read2B(HDMIRX_FST_DEV, 0x3A, &u32HTotal);
    u32HTotal = u32HTotal & 0x1FFF;
    u32HTotal = u32HTotal << u8ICLK;

    /* V_RES 0x3C[7:0], 0x3D[2:0]--[12:8]*/
    SIL9293_Read2B(HDMIRX_FST_DEV, 0x3C, &u32VTotal);
    u32VTotal = u32VTotal & 0x1FFF;

    *pu32HTotal = u32HTotal;
    *pu32VTotal = u32VTotal;
}

HI_VOID SIL9293_HAL_GetFrameRate(HI_U32* pu32FrameRate)
{
    HI_U32 u32PixClk;
    HI_U32 u32HTotal;
    HI_U32 u32VTotal;
    HI_U32 u32FrmRateTmp;

    /*
     * FrmaeRate = PixClock/(H_TOTAL*V_TOTAL)
     * PixClock = XTALIN(XCLK)*2048/clock count
     */

    SIL9293_HAL_GetPixClk10KHZ(&u32PixClk);
    SIL9293_HAL_GetHVTotal(&u32HTotal, &u32VTotal);

    if ((0 == u32PixClk) || (0 == (u32HTotal * u32VTotal)))
    {
        *pu32FrameRate = 0;
        return;
    }

    u32FrmRateTmp = u32PixClk * 10 * 1000 / (u32HTotal * u32VTotal);

    if ((u32FrmRateTmp <= 24) && (u32FrmRateTmp >= 23))
    {
        *pu32FrameRate = 24;
    }
    else if ((u32FrmRateTmp <= 26) && (u32FrmRateTmp >= 25))
    {
        *pu32FrameRate = 25;
    }
    else if ((u32FrmRateTmp <= 31) && (u32FrmRateTmp >= 29))
    {
        *pu32FrameRate = 30;
    }
    else if ((u32FrmRateTmp <= 51) && (u32FrmRateTmp >= 49))
    {
        *pu32FrameRate = 50;
    }
    else if ((u32FrmRateTmp <= 57) && (u32FrmRateTmp >= 55))
    {
        /* not support 56, force to 60 */
        *pu32FrameRate = 60;
    }
    else if ((u32FrmRateTmp <= 61) && (u32FrmRateTmp >= 59))
    {
        *pu32FrameRate = 60;
    }
    else if ((u32FrmRateTmp <= 68) && (u32FrmRateTmp >= 65))
    {
        *pu32FrameRate = 67;
    }
    else if ((u32FrmRateTmp <= 71) && (u32FrmRateTmp >= 69))
    {
        *pu32FrameRate = 70;
    }
    else if ((u32FrmRateTmp <= 73) && (u32FrmRateTmp >= 71))
    {
        *pu32FrameRate = 72;
    }
    else if ((u32FrmRateTmp <= 76) && (u32FrmRateTmp >= 74))
    {
        *pu32FrameRate = 75;
    }
    else if ((u32FrmRateTmp <= 86) && (u32FrmRateTmp >= 84))
    {
        *pu32FrameRate = 85;
    }
    else
    {
        *pu32FrameRate = 60;
        HI_ERR_HDMIRX("No Fix FrameRate:u32FrmRateTmp=%d\n", u32FrmRateTmp);
    }
}

HI_VOID SIL9293_HAL_GetInterlace(HI_BOOL* pbInterlace)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x55, &u8Value);

    /* 0x55 INTRL:[2] */
    if (u8Value & BIT2)
    {
        *pbInterlace = HI_TRUE;
    }
    else
    {
        *pbInterlace = HI_FALSE;
    }
}

HI_VOID SIL9293_HAL_GetOverSample(HI_OVERSAMPLE_MODE_E* penOverSampleMode)
{
    HI_U8 u8Value;

    if (!SIL9293_HAL_HasAvi())
    {
        *penOverSampleMode = HI_OVERSAMPLE_1X;
        return;
    }

    /* 0x48 PR:[3:0] */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x48, &u8Value);
    u8Value = u8Value & (BIT0 | BIT1 | BIT2 | BIT3);

    switch (u8Value)
    {
        case 0:
            *penOverSampleMode = HI_OVERSAMPLE_1X;
            break;
        case 1:
            *penOverSampleMode = HI_OVERSAMPLE_2X;
            break;
        case 3:
            *penOverSampleMode = HI_OVERSAMPLE_4X;
            break;
        default:
            *penOverSampleMode = HI_OVERSAMPLE_1X;
            break;
    }
}

HI_VOID SIL9293_HAL_GetDviMode(HI_BOOL* pbDviMode)
{
    HI_U8 u8Value;

    *pbDviMode = HI_TRUE;

    /* 0x68 0x34 MODE_EN:[1] */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x34, &u8Value);
    u8Value = u8Value & BIT1;

    if (u8Value)
    {
        *pbDviMode = HI_FALSE;
    }
}

HI_VOID SIL9293_HAL_GetPixFmt(HI_PIXEL_FORMAT_E* penPixelFmt)
{
    HI_U8 u8Value;

    /* NO AVI:RGB */
    if (!SIL9293_HAL_HasAvi())
    {
        *penPixelFmt = HI_PIXEL_FORMAT_RGB_444;
        SIL9293_HAL_UpOffset(HI_TRUE);
        return;
    }

    /* 0x44 Y:[6:5] */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x44, &u8Value);
    u8Value = (u8Value & (BIT6 | BIT5)) >> 5;

    switch (u8Value)
    {
        case 0:
            *penPixelFmt = HI_PIXEL_FORMAT_RGB_444;
            SIL9293_HAL_UpOffset(HI_TRUE);
            break;
        case 1:
            *penPixelFmt = HI_PIXEL_FORMAT_YUV_422;
            SIL9293_HAL_UpOffset(HI_FALSE);
            break;
        case 2:
            *penPixelFmt = HI_PIXEL_FORMAT_YUV_444;
            SIL9293_HAL_UpOffset(HI_FALSE);
            break;
        default:
            *penPixelFmt = HI_PIXEL_FORMAT_RGB_444;
            SIL9293_HAL_UpOffset(HI_TRUE);
            break;
    }
}

HI_VOID SIL9293_HAL_GetColorSpace(HI_COLOR_SPACE_E* penColorSpace)
{
    HI_U8 u8Value;

    /* NO AVI:RGB */
    if (!SIL9293_HAL_HasAvi())
    {
        *penColorSpace = HI_COLOR_SPACE_RGB;
        return;
    }

    /* 0x45 Y:[7:6] */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x45, &u8Value);
    u8Value = (u8Value & (BIT7 | BIT6)) >> 6;

    switch (u8Value)
    {
        case 0:
            *penColorSpace = HI_COLOR_SPACE_RGB;
            break;
        case 1:
            *penColorSpace = HI_COLOR_SPACE_YCBCR_601;
            break;
        case 2:
            *penColorSpace = HI_COLOR_SPACE_YCBCR_709;
            break;
        default:
            *penColorSpace = HI_COLOR_SPACE_RGB;
            break;
    }
}

HI_VOID SIL9293_HAL_GetColorDepth(HI_PIXEL_BITWIDTH_E* penBitWidth)
{
    HI_U8 u8Value;

    /* 0x61 PixDepth:[1:0] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x61, &u8Value);
    u8Value = u8Value & (BIT1 | BIT0);

    switch (u8Value)
    {
        case 0:
            *penBitWidth = HI_PIXEL_BITWIDTH_8BIT;
            break;
        case 1:
            *penBitWidth = HI_PIXEL_BITWIDTH_10BIT;
            break;
        case 2:
            *penBitWidth = HI_PIXEL_BITWIDTH_12BIT;
            break;
        default:
            *penBitWidth = HI_PIXEL_BITWIDTH_BUTT;
            break;
    }
}

HI_VOID SIL9293_HAL_GetICLK(HI_OVERSAMPLE_MODE_E* penICLK)
{
    HI_U8 u8Value;

    /* 0x08 ICLK:[5:4] */
    SIL9293_Read(HDMIRX_FST_DEV, 0x08, &u8Value);
    u8Value = (u8Value & (BIT5 | BIT4)) >> 4;

    switch (u8Value)
    {
        case 0:
            *penICLK = HI_OVERSAMPLE_1X;
            break;
        case 1:
            *penICLK = HI_OVERSAMPLE_2X;
            break;
        case 2:
            *penICLK = HI_OVERSAMPLE_1X;
            break;
        case 3:
            *penICLK = HI_OVERSAMPLE_4X;
            break;
        default:
            *penICLK = HI_OVERSAMPLE_1X;
            break;
    }
}

HI_VOID SIL9293_HAL_SetBt656Enable(HI_BOOL bBt656Enable)
{
    HI_U8 u8Value;

    if (HI_TRUE == bBt656Enable) /*bt656*/
    {
        /*
        Enable Auto video down-sampler configuration.
        */
        SIL9293_Write(HDMIRX_FST_DEV, 0xB9, 0x80);

        /*
        Enable Auto extended bit configuration.
        Enable Auto dither configuration.
        Enable Auto YCbCr multiplexed configuration.
        Enable Auto embed digital sync code configuration.
        Enable Auto blanking data configuration.
        Enable Auto pixel replication configuration.
        */
        SIL9293_Write(HDMIRX_FST_DEV, 0xBA, 0x3F);

        /* 0x08 BSEL[5:4] Input Pixel Clock Replication */
        SIL9293_Read(HDMIRX_FST_DEV, 0x08, &u8Value);
        /*2x clock (pixel data sent twice) */
        u8Value |= BIT4 & ~BIT5;
        SIL9293_Write(HDMIRX_FST_DEV, 0x08, u8Value);

        SIL9293_Read(HDMIRX_FST_DEV, 0x4A, &u8Value);
        /*Insert SYNC (SAV/EAV) code into ITU.656 data stream*/
        u8Value |= BIT7;
        /*Multiplexed YC 4:2:2 into a single channel*/
        u8Value |= BIT6;
        /*10 bits to 8 bits Data Dither.*/
        u8Value |= BIT5;
        /*Downsample 4:4:4 to 4:2:2 downsampler*/
        u8Value |= BIT1;
        SIL9293_Write(HDMIRX_FST_DEV, 0x4A, u8Value);
    }
    else /*bt1120*/
    {
        SIL9293_Read(HDMIRX_FST_DEV, 0x4A, &u8Value);
        /*Insert SYNC (SAV/EAV) code into ITU.656 data stream*/
        /*Multiplexed YC 4:2:2 into a single channel*/
        u8Value &= ~(BIT7 | BIT6 | BIT2);
        SIL9293_Write(HDMIRX_FST_DEV, 0x4A, u8Value);

        /* 0x08 BSEL[5:4] Input Pixel Clock Replication */
        SIL9293_Read(HDMIRX_FST_DEV, 0x08, &u8Value);
        u8Value &= ~(BIT4 | BIT5);
        SIL9293_Write(HDMIRX_FST_DEV, 0x08, u8Value);

        SIL9293_Write(HDMIRX_FST_DEV, 0xB9, 0x00);
        SIL9293_Write(HDMIRX_FST_DEV, 0xBA, 0x00);
    }
}


/*
 * AUDIO FUNCTIONS
 */

HI_VOID SIL9293_HAL_SetAudioAuto(HI_VOID)
{
    HI_U8 u8Value;

    /* 0x26 SCK[6] I2S edge; SIZE[5]  16bit, input v = 1, 32bit, output v = 0 */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x26, &u8Value);

    //u8Value = u8Value|BIT6|BIT5;
    u8Value = (u8Value | BIT6) & (~BIT5);
    SIL9293_Write(HDMIRX_SEC_DEV, 0x26, u8Value);

    /* 0x88 PLL 0x88£¬0x89 PLL 0x16 */
    SIL9293_Write(HDMIRX_FST_DEV, 0x88, 0x88);
    SIL9293_Write(HDMIRX_FST_DEV, 0x89, 0x16);

    //:TODO:AEC

    SIL9293_HAL_SetFSSel(HI_TRUE);

    /* 0x68 0x27 [7:4] four switches for I2S channel */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x27, &u8Value);
    u8Value = u8Value | BIT7 | BIT6 | BIT5 | BIT4;
    SIL9293_Write(HDMIRX_SEC_DEV, 0x27, u8Value);

    /* 0x68 0x29 [2] I2S output switch */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x29, &u8Value);
    u8Value = u8Value | BIT2;
    SIL9293_Write(HDMIRX_SEC_DEV, 0x29, u8Value);

    /* 0xB5
     * AAC_OE[5] 0 = AutoAudioControl Output Disable
     * AAC_EN[0] 0 = AutoAudioControl Disable
     */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value & (~(BIT5 | BIT0));

    //u8Value = u8Value|(BIT5|BIT0);
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);
}

HI_VOID SIL9293_HAL_GetCTS(HI_U32* pu32CTS)
{
    HI_U8 u8ValueL;
    HI_U8 u8ValueM;
    HI_U8 u8ValueH;
    HI_U32 u32Tmp;

    SIL9293_Read(HDMIRX_SEC_DEV, 0x0C, &u8ValueL);
    SIL9293_Read(HDMIRX_SEC_DEV, 0x0D, &u8ValueM);
    SIL9293_Read(HDMIRX_SEC_DEV, 0x0E, &u8ValueH);

    u32Tmp = u8ValueH & 0xF;
    u32Tmp = (u32Tmp << 8) + (HI_U32)u8ValueM;
    u32Tmp = (u32Tmp << 8) + (HI_U32)u8ValueL;

    *pu32CTS = u32Tmp;
}

HI_VOID SIL9293_HAL_GetN(HI_U32* pu32N)
{
    HI_U8 u8ValueL;
    HI_U8 u8ValueM;
    HI_U8 u8ValueH;
    HI_U32 u32Tmp;

    SIL9293_Read(HDMIRX_SEC_DEV, 0x06, &u8ValueL);
    SIL9293_Read(HDMIRX_SEC_DEV, 0x07, &u8ValueM);
    SIL9293_Read(HDMIRX_SEC_DEV, 0x08, &u8ValueH);

    u32Tmp = u8ValueH & 0xF;
    u32Tmp = (u32Tmp << 8) + (HI_U32)u8ValueM;
    u32Tmp = (u32Tmp << 8) + (HI_U32)u8ValueL;

    *pu32N = u32Tmp;
}

HI_VOID SIL9293_HAL_SetFSSel(HI_BOOL bSW)
{
    HI_U8 u8Value;

    /* 0x68 0X00  FSSEL[1] */
    SIL9293_Read(HDMIRX_SEC_DEV, 0x00, &u8Value);
    if (u8Value & (~BIT1))
    {
        HI_INFO_HDMIRX("HDMIRX_SEC_DEV:0x%02x\n", u8Value);
        return;
    }
    if (bSW)
    {
        u8Value = u8Value | BIT1;
    }
    else
    {
        u8Value = u8Value & (~BIT1);
    }

    SIL9293_Write(HDMIRX_SEC_DEV, 0x00, u8Value);
}

HI_VOID SIL9293_HAL_CalcSWFS(HI_U32* pu32SWFS)
{
    HI_U32 u32CTS;
    HI_U32 u32N;
    HI_U32 u32PixClk;

    SIL9293_HAL_GetCTS(&u32CTS);
    SIL9293_HAL_GetN(&u32N);
    SIL9293_HAL_GetPixClk10KHZ(&u32PixClk);

    if ((0 == u32CTS) || (0 == u32N) || (0 == u32PixClk))
    {
        *pu32SWFS = 0;
        return;
    }

    *pu32SWFS = u32PixClk * u32N / u32CTS;
    *pu32SWFS = (*pu32SWFS) * 100 / 128;
}

HI_VOID SIL9293_HAL_UpdateFS(HI_U32 u32SWFS)
{
    HI_U8 u8Value;

    if ((u32SWFS > 200) && (u32SWFS <= 230))
    {
        /* not support */
        u8Value = AUDIO_CHST4__FS_22;
        u8Value = AUDIO_CHST4__FS_UNKNOWN;
    }
    else if ((u32SWFS > 230) && (u32SWFS <= 280))
    {
        /* not support */
        u8Value = AUDIO_CHST4__FS_24;
        u8Value = AUDIO_CHST4__FS_UNKNOWN;
    }
    else if ((u32SWFS > 280) && (u32SWFS <= 380))
    {
        u8Value = AUDIO_CHST4__FS_32;
    }
    else if ((u32SWFS > 380) && (u32SWFS <= 460))
    {
        u8Value = AUDIO_CHST4__FS_44;
    }
    else if ((u32SWFS > 460) && (u32SWFS <= 540))
    {
        u8Value = AUDIO_CHST4__FS_48;
    }
    else if ((u32SWFS > 540) && (u32SWFS <= 820))
    {
        /* not support */
        u8Value = AUDIO_CHST4__FS_UNKNOWN;
    }
    else if ((u32SWFS > 820) && (u32SWFS <= 921))
    {
        u8Value = AUDIO_CHST4__FS_88;
    }
    else if ((u32SWFS > 921) && (u32SWFS <= 1100))
    {
        u8Value = AUDIO_CHST4__FS_96;
    }
    else if ((u32SWFS > 1100) && (u32SWFS <= 1600))
    {
        u8Value = AUDIO_CHST4__FS_UNKNOWN;
    }
    else if ((u32SWFS > 1600) && (u32SWFS <= 1792))
    {
        u8Value = AUDIO_CHST4__FS_176;
    }
    else if ((u32SWFS > 1792) && (u32SWFS <= 2500))
    {
        u8Value = AUDIO_CHST4__FS_192;
    }
    else
    {
        u8Value = AUDIO_CHST4__FS_UNKNOWN;
    }

    if (AUDIO_CHST4__FS_UNKNOWN == u8Value)
    {
        SIL9293_HAL_SetFSSel(HI_FALSE);
    }
    else
    {
        SIL9293_HAL_SetFSSel(HI_TRUE);
    }

    u8Value = 0x50 | u8Value;
    SIL9293_Write(HDMIRX_SEC_DEV, 0x02, u8Value);
}

HI_VOID SIL9293_HAL_ResetAudioFIFO(HI_VOID)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x05, &u8Value);

    /* 0x05, FIFORST:[1] */
    u8Value = u8Value | BIT1;
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);

    msleep(20);

    u8Value = u8Value & (~BIT1);
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);
}

HI_VOID SIL9293_HAL_ResetACR(HI_VOID)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x05, &u8Value);

    /* 0x05, ACRRST:[5] */
    u8Value = u8Value | BIT5;
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);

    msleep(20);

    u8Value = u8Value & (~BIT5);
    SIL9293_Write(HDMIRX_FST_DEV, 0x05, u8Value);
}

HI_VOID SIL9293_HAL_ResetAAC(HI_VOID)
{
    HI_U8 u8Value;

    /*
        1.Set AAC_OE = 0 to disable AAC_OE (if used).
        2. Set AAC_EN = 0 to disable AAC.
        3. Set AACRST = 1 to perform AAC reset.
        4. Set AACRST = 0.
        5. Set AAC_EN = 1 to enable AAC.
        6. Set AAC_OE = 1 to enable AAC_OE (if used).
     */

    /* 1.Disable AAC_OE 0xB5-[5] */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value & (~BIT5);
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);

    msleep(5);

    /* 2.Disable AAC_EN 0xB5-[0] */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value & (~BIT0);
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);

    msleep(5);

    /* 3.AACRST=1,4.AACRST=0 */
    SIL9293_HAL_ResetACR();

    /* 5.Enable AAC_EN 0xB5-[0] */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value | BIT0;
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);

    msleep(5);

    /* 5.Enable AAC_OE 0xB5-[5] */
    SIL9293_Read(HDMIRX_FST_DEV, 0xB5, &u8Value);
    u8Value = u8Value | BIT5;
    SIL9293_Write(HDMIRX_FST_DEV, 0xB5, u8Value);

    msleep(5);
}

HI_VOID SIL9293_HAL_GetHdcpStatus(HI_HDCP_STATE_E* penHdcpState)
{
    HI_U8 u8Value;

    SIL9293_Read(HDMIRX_FST_DEV, 0x32, &u8Value);
    u8Value = (u8Value & (BIT5 | BIT4)) >> 4;

    switch (u8Value)
    {
        case 0:
            *penHdcpState = HI_HDCP_STATE_FAILED;
            break;
        case 1:
            *penHdcpState = HI_HDCP_STATE_TRYING;
            break;
        case 2:
            *penHdcpState = HI_HDCP_STATE_TRYING;
            break;
        case 3:
            *penHdcpState = HI_HDCP_STATE_SUCCESS;
            break;
        default:
            *penHdcpState = HI_HDCP_STATE_FAILED;
            break;
    }
}
