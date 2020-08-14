/***********************************************************************************
*          Copyright 2004 - 2011, Hisilicon Tech. Co., Ltd.
*               ALL RIGHTS RESERVED
* FileName: drv_gpio.c
* Description:use gpio opreate
*
* History:
* Version   Date        Author       DefectNum    Description
* 0.01    2011-3-17  chenqiang  NULL           Create this file.
***********************************************************************************/
#include "hi_common.h"
#include "hi_drv_gpio.h"

#include "hi_reg.h"
#include "hi_reg_common.h"
#include "hi_error_mpi.h"

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)

#define HI_GPIO_GROUP_NUM   18

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000,
    0xF8B2A000,
    0xF8B2B000,
    0xF8B2C000,
    0xF8B2D000,
    0xF8B2E000,
    0xF8B2F000,
    0xF8B30000,
    0xF8B31000
};

#elif  defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)

#define HI_GPIO_GROUP_NUM   17

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000,
    0xF8B2A000,
    0xF8B2B000,
    0xF8B2C000,
    0xF8B2D000,
    0xF8B2E000,
    0xF8B2F000,
    0xF8B30000,
};

#elif  defined(CHIP_TYPE_hi3798cv200)

#define HI_GPIO_GROUP_NUM   13

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000,
    0xF8B2A000,
    0xF8B2B000,
    0xF8B2C000,
};


#elif  defined(CHIP_TYPE_hi3719mv100)||defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)

#define HI_GPIO_GROUP_NUM   10

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000
};


#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

#define HI_GPIO_GROUP_NUM   7

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
};

#elif  defined(CHIP_TYPE_hi3718mv100)

#define HI_GPIO_GROUP_NUM   9

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
};


#elif  defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

#define HI_GPIO_GROUP_NUM   11

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8004000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000,
    0xF8B2A000,
};


#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)

#define HI_GPIO_GROUP_NUM   20

HI_U32 g_GpioRegAddr[HI_GPIO_GROUP_NUM] =
{
    0xF8B20000,
    0xF8B21000,
    0xF8B22000,
    0xF8B23000,
    0xF8B24000,
    0xF8B25000,
    0xF8B26000,
    0xF8B27000,
    0xF8B28000,
    0xF8B29000,
    0xF8B2A000,
    0xF8B2B000,
    0xF8B2C000,
    0xF8B2D000,
    0xF8B2E000,
    0xF8B2F000,
    0xF8B30000,
    0xF8B31000,
    0xF8004000,  /* GPIO_STB0 */
    0xF8009000,  /* GPIO_STB1 */
};

#else

#error YOU MUST DEFINE  CHIP_TYPE!

#endif

#define HI_GPIO_BIT_NUM     8

#define HI_GPIO_MAX_BIT_NUM (HI_GPIO_GROUP_NUM * HI_GPIO_BIT_NUM)

#define HI_GPIO_DIR_REG     0x400

#define HI_GPIO_OUTPUT      1
#define HI_GPIO_INPUT       0

#define REG_USR_ADDR(RegAddr) * ((volatile HI_U32 *)(RegAddr))

static HI_BOOL DRV_GPIO_Convert(HI_U32 u32GpioNo, HI_U32 *pu32GroupNo, HI_U32 *pu32BitNo)
{
#if defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
    if ((u32GpioNo >= HI_GPIO_MAX_BIT_NUM)||(u32GpioNo==0)||((u32GpioNo>=15)&&(u32GpioNo<=17)))
    {
        return HI_FALSE;
    }
#else
	if (u32GpioNo >= HI_GPIO_MAX_BIT_NUM)
	{
		return HI_FALSE;
	}
#endif	

    *pu32GroupNo = u32GpioNo / HI_GPIO_BIT_NUM;
    *pu32BitNo = u32GpioNo % HI_GPIO_BIT_NUM;

    return HI_TRUE;
}

#ifdef HI_ADVCA_FUNCTION_RELEASE
 #define GPIO_DEGUB(format, arg...)
#else
 #define GPIO_DEGUB(format...)     HI_ERR_PRINT(HI_ID_GPIO, format)
#endif

HI_S32 HI_DRV_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType)
{	
#if    defined(CHIP_TYPE_hi3716cv200)   \
		|| defined(CHIP_TYPE_hi3716mv400)	\
		|| defined(CHIP_TYPE_hi3718cv100)	\
		|| defined(CHIP_TYPE_hi3719cv100)	\
		|| defined(CHIP_TYPE_hi3718mv100)	\
		|| defined(CHIP_TYPE_hi3719mv100)	

	HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo > 5 * 8 + 7)
		|| (u32GpioNo == 5 * 8 + 5)
		|| (u32GpioNo == 5 * 8 + 6))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;
	
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

	HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;
	
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo > 5 * 8 + 7)
		|| (u32GpioNo == 5 * 8 + 3)
		|| (u32GpioNo == 5 * 8 + 4))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;
	
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)

	HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;
		
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo >= 5 * 8 + 7))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;
		
		//FIXME hi3798cv100 no such register SC_GPIO_OD_CTRL
#elif defined(CHIP_TYPE_hi3796cv100)   \
	|| defined(CHIP_TYPE_hi3798cv100)   \
	|| defined(CHIP_TYPE_hi3798cv200)   \
	|| defined(CHIP_TYPE_hi3798mv200)	\
	|| defined(CHIP_TYPE_hi3798mv200_a)	

	return HI_SUCCESS;
		
#else
	return HI_ERR_GPIO_NOT_SUPPORT;
#endif

}

HI_S32 HI_DRV_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType)
{
		
#if    defined(CHIP_TYPE_hi3716cv200)   \
		|| defined(CHIP_TYPE_hi3716mv400)	\
		|| defined(CHIP_TYPE_hi3718cv100)	\
		|| defined(CHIP_TYPE_hi3719cv100)	\
		|| defined(CHIP_TYPE_hi3718mv100)	\
		|| defined(CHIP_TYPE_hi3719mv100)

	HI_U32 u32Value = 0;
	HI_U32 u32Bit = 0;
		
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo > 5 * 8 + 7)
		|| (u32GpioNo == 5 * 8 + 5)
		|| (u32GpioNo == 5 * 8 + 6))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;
		
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

	HI_U32 u32Value = 0;
	HI_U32 u32Bit = 0;
		
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo > 5 * 8 + 7)
		|| (u32GpioNo == 5 * 8 + 3)
		|| (u32GpioNo == 5 * 8 + 4))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;
		
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)

	HI_U32 u32Value = 0;
	HI_U32 u32Bit = 0;
			
	if ((u32GpioNo < 5 * 8 + 0)
		|| (u32GpioNo >= 5 * 8 + 7))
	{
		return HI_ERR_GPIO_NOT_SUPPORT;
	}
	u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;			
			//FIXME hi3798cv100 no such register SC_GPIO_OD_CTRL
#elif defined(CHIP_TYPE_hi3796cv100)   \
	|| defined(CHIP_TYPE_hi3798cv100)   \
	|| defined(CHIP_TYPE_hi3798cv200)   \
	|| defined(CHIP_TYPE_hi3798mv200)	\
	|| defined(CHIP_TYPE_hi3798mv200_a)	
	return HI_SUCCESS;
			
#else
	return HI_ERR_GPIO_NOT_SUPPORT;
#endif

}

HI_S32 HI_DRV_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_U32 u32DirBit)
{
    HI_S32 GpioDirtValue;
    HI_U32 GpioUsrAddr;
    HI_U32 u32GroupNo;
    HI_U32 u32BitX;

    if (HI_FALSE == DRV_GPIO_Convert(u32GpioNo, &u32GroupNo, &u32BitX))
    {
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((u32DirBit != HI_TRUE) && (u32DirBit != HI_FALSE))
    {
        GPIO_DEGUB("para bInput is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    GpioUsrAddr = g_GpioRegAddr[u32GroupNo];

    GpioDirtValue = REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG);

    if (!u32DirBit)   /*0 output */
    {
        GpioDirtValue |= (HI_GPIO_OUTPUT << u32BitX);
        REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG) = GpioDirtValue;
    }
    else /*1 input */
    {
        GpioDirtValue &= (~(1 << u32BitX));
        REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG) = GpioDirtValue;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_U32* pu32DirBit)
{
    HI_S32 GpioDirtValue;
    HI_U32 GpioUsrAddr;
    HI_U32 u32GroupNo;
    HI_U32 u32BitX;

    if (HI_FALSE == DRV_GPIO_Convert(u32GpioNo, &u32GroupNo, &u32BitX))
    {
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (!pu32DirBit)
    {
        GPIO_DEGUB("para pu32DirBit is null.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    GpioUsrAddr = g_GpioRegAddr[u32GroupNo];

    GpioDirtValue = REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG);

    GpioDirtValue &= (1 << u32BitX);

    if (GpioDirtValue == 0)
    {
        *pu32DirBit = HI_TRUE;  /*input*/
    }
    else
    {
        *pu32DirBit = HI_FALSE; /*output*/
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_WriteBit(HI_U32 u32GpioNo, HI_U32 u32BitValue)
{
    HI_S32 GpioDirtValue;
    HI_U32 GpioUsrAddr;
    HI_U32 u32GroupNo;
    HI_U32 u32BitX;

    if (HI_FALSE == DRV_GPIO_Convert(u32GpioNo, &u32GroupNo, &u32BitX))
    {
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((u32BitValue != HI_TRUE) && (u32BitValue != HI_FALSE))
    {
        GPIO_DEGUB("para u32BitValue is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    GpioUsrAddr = g_GpioRegAddr[u32GroupNo];

    GpioDirtValue = REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG);

    GpioDirtValue &= (1 << u32BitX);

    GpioDirtValue = GpioDirtValue >> u32BitX;

    if (GpioDirtValue != HI_GPIO_OUTPUT)
    {
        GPIO_DEGUB("GPIO Direction is input, write can not operate.\n");
        return HI_ERR_GPIO_INVALID_OPT;
    }

    if (u32BitValue)
    {
        REG_USR_ADDR(GpioUsrAddr + (4 << u32BitX)) = 1 << u32BitX;
    }
    else
    {
        REG_USR_ADDR(GpioUsrAddr + (4 << u32BitX)) = 0;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_ReadBit(HI_U32 u32GpioNo, HI_U32* pu32BitValue)
{
    HI_S32 GpioDirtValue;
    HI_U32 GpioUsrAddr;
    HI_U32 u32GroupNo;
    HI_U32 u32BitX;

    if (HI_FALSE == DRV_GPIO_Convert(u32GpioNo, &u32GroupNo, &u32BitX))
    {
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (!pu32BitValue)
    {
        GPIO_DEGUB("para pu32BitValue is null.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    GpioUsrAddr = g_GpioRegAddr[u32GroupNo];

    GpioDirtValue = REG_USR_ADDR(GpioUsrAddr + HI_GPIO_DIR_REG);

    GpioDirtValue &= (1 << u32BitX);

    if (GpioDirtValue != HI_GPIO_INPUT)
    {
        GPIO_DEGUB("GPIO Direction is output,read can not operate\n");
        return HI_ERR_GPIO_INVALID_OPT;
    }

    *pu32BitValue = REG_USR_ADDR(GpioUsrAddr + (4 << u32BitX)) >> u32BitX;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_Init(void)
{
    return HI_SUCCESS;
}
