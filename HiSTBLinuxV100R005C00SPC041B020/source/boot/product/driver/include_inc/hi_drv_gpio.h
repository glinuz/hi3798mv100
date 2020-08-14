/***********************************************************************************
*              Copyright 2004 - 2011, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_gpio.h
* Description:use gpio opreate
*
* History:
* Version   Date                Author       DefectNum    Description
* 0.01    2011-3-17      chenqiang      NULL           Create this file.
***********************************************************************************/
#ifndef __DRV_GPIO_EXT_H__
#define __DRV_GPIO_EXT_H__

#include "hi_type.h"

#define HI_ERR_GPIO_NOT_INIT        (HI_S32)(0x80470003)
#define HI_ERR_GPIO_INVALID_PARA    (HI_S32)(0x80470004)
#define HI_ERR_GPIO_NULL_PTR        (HI_S32)(0x80470005)
#define HI_ERR_GPIO_INVALID_OPT     (HI_S32)(0x80470006)

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)	\
    || defined(CHIP_TYPE_hi3716mv410)	\
    || defined(CHIP_TYPE_hi3716mv420)	\
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
	|| defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)
#define REG_BASE_GPIO5  0xF8004000
#elif defined(CHIP_TYPE_hi3796cv100)    \
   || defined(CHIP_TYPE_hi3798cv100)
#define REG_BASE_GPIO5  0xF8B25000
#else
#error YOU MUST DEFINE  CHIP_TYPE!
#endif

/**GPIO output type*/
/** CNcomment:GPIO  ‰≥ˆ¿‡–Õ*/
typedef enum hiUNF_GPIO_OUTPUTTYPE_E
{
    HI_UNF_GPIO_OUTPUTTYPE_CMOS,
    HI_UNF_GPIO_OUTPUTTYPE_OD,
    HI_UNF_GPIO_OUTPUTTYPE_BUTT,
}HI_UNF_GPIO_OUTPUTTYPE_E;


HI_S32 HI_DRV_GPIO_Init(void);

/*param[in] u32GpioNo :gpio No.,for example the No. of GPIO5_1 is 41(5*8+1);
param[in] u32DirBit: 0 - output, 1 - input
 */
HI_S32 HI_DRV_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_U32 u32DirBit);
HI_S32 HI_DRV_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_U32* pu32DirBit );
HI_S32 HI_DRV_GPIO_ReadBit(HI_U32 u32GpioNo, HI_U32* pu32BitValue);
HI_S32 HI_DRV_GPIO_WriteBit(HI_U32 u32GpioNo, HI_U32 u32BitValue);
HI_S32 HI_DRV_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType);
HI_S32 HI_DRV_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType);

#endif
