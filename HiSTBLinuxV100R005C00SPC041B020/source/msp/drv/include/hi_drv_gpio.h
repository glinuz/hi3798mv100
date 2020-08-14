/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_drv_gpio.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/07/07
  Description   :
  History       :
  1.Date        : 2010/07/07
    Author      : j40671
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_GPIO_H__
#define __HI_DRV_GPIO_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_unf_gpio.h"

#define HI_FATAL_GPIO(fmt...)   HI_FATAL_PRINT(HI_ID_GPIO, fmt)
#define HI_ERR_GPIO(fmt...)     HI_ERR_PRINT(HI_ID_GPIO, fmt)
#define HI_WARN_GPIO(fmt...)    HI_WARN_PRINT(HI_ID_GPIO, fmt)
#define HI_INFO_GPIO(fmt...)    HI_INFO_PRINT(HI_ID_GPIO, fmt)

#if    defined(CHIP_TYPE_hi3716cv200)   \
	|| defined(CHIP_TYPE_hi3716mv400)   \
	|| defined(CHIP_TYPE_hi3718cv100)   \
	|| defined(CHIP_TYPE_hi3719cv100)

#define HI_GPIO_GROUP_NUM   18

#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

#define HI_GPIO_GROUP_NUM   7

#elif defined(CHIP_TYPE_hi3719mv100)

#define HI_GPIO_GROUP_NUM   10

#elif defined(CHIP_TYPE_hi3718mv100)

#define HI_GPIO_GROUP_NUM   9

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

#define HI_GPIO_GROUP_NUM   11

#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)

#define HI_GPIO_GROUP_NUM   13

#elif defined (CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)

#define HI_GPIO_GROUP_NUM 10

#else

#error YOU MUST DEFINE CHIP_TYPE!

#endif

#define HI_GPIO_BIT_NUM     8

#define HI_GPIO_MAX_BIT_NUM (HI_GPIO_GROUP_NUM * HI_GPIO_BIT_NUM)

#define HI_GPIO_SPACE_SIZE  0x1000

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)

#define 	HI_GPIO_MAX_GROUP_NUM	18
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000
#define     HI_GPIO_9_ADDR 0xF8B29000
#define     HI_GPIO_10_ADDR 0xF8B2A000
#define     HI_GPIO_11_ADDR 0xF8B2B000
#define     HI_GPIO_12_ADDR 0xF8B2C000
#define     HI_GPIO_13_ADDR 0xF8B2D000
#define     HI_GPIO_14_ADDR 0xF8B2E000
#define     HI_GPIO_15_ADDR 0xF8B2F000
#define     HI_GPIO_16_ADDR 0xF8B30000
#define     HI_GPIO_17_ADDR 0xF8B31000

#elif defined(CHIP_TYPE_hi3719mv100)

#define 	HI_GPIO_MAX_GROUP_NUM	10
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000
#define     HI_GPIO_9_ADDR 0xF8B29000

#elif defined(CHIP_TYPE_hi3718mv100)

#define 	HI_GPIO_MAX_GROUP_NUM	9
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000

#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

#define 	HI_GPIO_MAX_GROUP_NUM	7
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

#define 	HI_GPIO_MAX_GROUP_NUM	11
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000
#define     HI_GPIO_9_ADDR 0xF8B29000
#define     HI_GPIO_10_ADDR 0xF8B2A000

#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)

#define 	HI_GPIO_MAX_GROUP_NUM	13
#define     HI_GPIO_IRQ_NO (108 + 32)

#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000
#define     HI_GPIO_9_ADDR 0xF8B29000
#define     HI_GPIO_10_ADDR 0xF8B2A000
#define     HI_GPIO_11_ADDR 0xF8B2B000
#define     HI_GPIO_12_ADDR 0xF8B2C000

#elif defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)||defined(CHIP_TYPE_hi3796mv200)
#define 	HI_GPIO_MAX_GROUP_NUM	10
#define     HI_GPIO_IRQ_NO (108 + 32)
#define     HI_GPIO_0_ADDR 0xF8B20000
#define     HI_GPIO_1_ADDR 0xF8B21000
#define     HI_GPIO_2_ADDR 0xF8B22000
#define     HI_GPIO_3_ADDR 0xF8B23000
#define     HI_GPIO_4_ADDR 0xF8B24000
#define     HI_GPIO_5_ADDR 0xF8004000
#define     HI_GPIO_6_ADDR 0xF8B26000
#define     HI_GPIO_7_ADDR 0xF8B27000
#define     HI_GPIO_8_ADDR 0xF8B28000
#define     HI_GPIO_9_ADDR 0xF8B29000

#endif

#define     HI_GPIO_DIR_REG     0x400
#define     HI_GPIO_IS_REG      0x404
#define     HI_GPIO_IBE_REG     0x408
#define     HI_GPIO_IEV_REG     0x40c
#define     HI_GPIO_IE_REG      0x410
#define     HI_GPIO_RIS_REG     0x414
#define     HI_GPIO_MIS_REG     0x418
#define     HI_GPIO_IC_REG      0x41c

#define     HI_GPIO_OUTPUT  1
#define     HI_GPIO_INPUT   0
#define     REG_USR_ADDR(RegAddr) * ((volatile HI_U32 *)(RegAddr))
#define     HI_GPIO_ADDR_SIZE   0x1000

typedef struct hiGPIO_DATA_S
{
    HI_U32                u32GpioNo;
    HI_U32                bEnable;
    HI_UNF_GPIO_INTTYPE_E enIntType;      /*gpio interrupt trigger mode*/
} GPIO_DATA_S;

typedef struct hiGPIO_INT_S
{
    HI_U32 u32GpioNo;
    HI_U32 u32TimeoutMs;
} GPIO_INT_S;

typedef struct hiGPIO_GET_GPIONUM_S
{
    HI_U8 u8GpioGrpNum;     /* gpio group number*/
    HI_U8 u8GpioMaxNum;     /* gpio  max number*/
} GPIO_GET_GPIONUM_S;

HI_S32    HI_DRV_GPIO_Init(HI_VOID);
HI_VOID   HI_DRV_GPIO_DeInit(HI_VOID);

HI_S32 HI_DRV_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_U32 u32DirBit);
HI_S32 HI_DRV_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_U32* pu32DirBit);
HI_S32 HI_DRV_GPIO_WriteBit(HI_U32 u32GpioNo, HI_U32 u32BitValue);
HI_S32 HI_DRV_GPIO_ReadBit(HI_U32 u32GpioNo, HI_U32* pu32BitValue);
HI_S32 HI_DRV_GPIO_GetGpioNum(GPIO_GET_GPIONUM_S* GpioNum);

HI_S32 HI_DRV_GPIO_SetIntType(HI_U32 u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntTriMode);
HI_S32 HI_DRV_GPIO_SetBitIntEnable(HI_U32 u32GpioNo, HI_BOOL bEnable);
HI_S32 HI_DRV_GPIO_RegisterServerFunc(HI_U32 u32GpioNo, HI_VOID (*func)(HI_U32));
HI_S32 HI_DRV_GPIO_UnregisterServerFunc(HI_U32 u32GpioNo);
HI_S32 HI_DRV_GPIO_CearGroupInt(HI_U32 u32GroupNo);
HI_S32 HI_DRV_GPIO_ClearBitInt(HI_U32 u32GpioNo);

#endif /* End of #ifndef __HI_DRV_GPIO_H__*/

