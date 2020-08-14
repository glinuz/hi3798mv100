/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
 File Name     : hi_unf_gpio.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date   : 2008-06-05
Last Modified by:
Description   : Application programming interfaces (APIs) of the external chip software (ECS)
Function List :
Change History:
******************************************************************************/
#ifndef __HI_UNF_GPIO_H__
#define __HI_UNF_GPIO_H__

#include "hi_common.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      GPIO */
/** @{*/  /** <!-- [GPIO] */

#define HI_UNF_GPIO_Open  HI_UNF_GPIO_Init
#define HI_UNF_GPIO_Close HI_UNF_GPIO_DeInit

/**type of GPIO interrupt*/
/** CNcomment:GPIO 中断类型*/
typedef enum hiUNF_GPIO_INTTYPE_E
{
    HI_UNF_GPIO_INTTYPE_UP, /**<spring by the up edge*/                 /**< CNcomment:上升沿触发*/
    HI_UNF_GPIO_INTTYPE_DOWN, /**<spring by the down edge*/               /**< CNcomment:下降沿触发*/
    HI_UNF_GPIO_INTTYPE_UPDOWN, /**<spring by both the up and down edge*/   /**< CNcomment:双沿触发*/
    HI_UNF_GPIO_INTTYPE_HIGH, /**<spring by the high level*/              /**< CNcomment:高电平触发*/
    HI_UNF_GPIO_INTTYPE_LOW, /**<spring by the low level*/               /**< CNcomment:低电平触发*/
    HI_UNF_GPIO_INTTYPE_BUTT, /**<Invalid value*/                        /**< CNcomment:非法边界值*/
} HI_UNF_GPIO_INTTYPE_E;

/**GPIO output type*/
/** CNcomment:GPIO 输出类型*/
typedef enum hiUNF_GPIO_OUTPUTTYPE_E
{
	HI_UNF_GPIO_OUTPUTTYPE_CMOS,
	HI_UNF_GPIO_OUTPUTTYPE_OD,
	HI_UNF_GPIO_OUTPUTTYPE_BUTT,		
}HI_UNF_GPIO_OUTPUTTYPE_E;

/** @}*/  /** <!-- ==== Structure Definition End ====*/




/******************************* API Declaration *****************************/
/** \addtogroup      GPIO*/
/** @{*/  /** <!-- [GPIO] */

/**
 \brief Starts the general-purpose input/output (GPIO) device.
CNcomment:\brief 打开GPIO（General Purpose Input/Output）设备。CNend

 \attention \n
This API can be called repeatedly.
CNcomment:重复打开会成功。CNend

 \param  N/A       CNcomment:无。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_FAILURE			Initialize GPIO failed.                 	CNcomment:打开GPIO失败。CNend
 \retval ::HI_ERR_GPIO_OPEN_ERR  Initialize GPIO failed.                 	CNcomment:打开GPIO失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_Init(HI_VOID);

/**
 \brief Stops the GPIO device.
CNcomment:\brief 关闭GPIO设备。CNend

 \attention \n
This API can be called repeatedly.
CNcomment:重复关闭会成功。CNend
 \param  N/A  CNcomment:无。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_CLOSE_ERR  Deinitialize GPIO failed.                 	CNcomment:打开GPIO失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_DeInit(HI_VOID);

/**
 \brief Reads data from a GPIO pin.
CNcomment:\brief 从GPIO单个引脚读取数据。CNend

 \attention The pin number is defined as follows: Pin number = GPIO group ID x 8 + GPIO pin ID in the group\n
For example, GPIO1_2 indicates pin 2 in group 1, and the pin number is 10 (1 x 8 + 2).\n
Both pin group ID and pin number are numbered from 0. Each HD chip provides GPIO pins number reference to HD chip hardware manual.\n
CNcomment:\attention 管脚号计算规则: 管脚号 = GPIO组号*8 + GPIO管脚在组内的遍号。\n
比如GPIO1_2,代表第1组第2脚，那么管脚号=1*8+2=10。\n
GPIO的组号和管脚号的编号都从0开始。高清芯片提供的gpio管脚数请参考相关的芯片硬件手册\n CNend

 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[out] pbHighVolt   Pointer to the input level of a pin.  CNcomment:指针类型，返回管脚输入电平。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NULL_PTR  Pointer Parameters is NULL.                 	CNcomment:指针参数为空指针。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_INVALID_OPT  Invalid Operation.              			CNcomment:非法操作。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_ReadBit(HI_U32 u32GpioNo, HI_BOOL  *pbHighVolt);

/**
 \brief Writes data to a GPIO pin.
CNcomment:\brief 向GPIO单个引脚输出数据。CNend
 \attention \n
N/A
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] bHighVolt  Output level of a pin              CNcomment:管脚输出电平。CNend
                      0: low level                       CNcomment:0: 低电平 CNend
                      1: high level                      CNcomment:1：高电平 CNend
                     Others: high level                  CNcomment:其他：高电平。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_INVALID_OPT  Invalid Operation.              			CNcomment:非法操作。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_WriteBit(HI_U32 u32GpioNo, HI_BOOL bHighVolt );

/**
 \brief Sets the direction (input or output) of a GPIO pin.
CNcomment:\brief 设置GPIO单个引脚输入输出方向。CNend

 \attention \n
When setting the operating mode of a GPIO pin, ensure that it works in GPIO mode only.\n
This is because the GPIO pin may be multiplexed.\n
CNcomment:设置对应管脚的工作方式时，必须保证该管脚仅工作在GPIO模式下，GPIO管脚有可能被复用。CNend

 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] bInput  Boolean variable that indicates the direction of a pin  CNcomment:布尔变量，标识管脚方向。CNend
                   HI_TRUE: input pin  HI_TRUE.                           CNcomment:该管脚用于输入。CNend
                   HI_FALSE: output pin HI_FALSE.                         CNcomment:该管脚用于输出。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_BOOL bInput);

/**
 \brief Obtains the direction (input or output) of a GPIO pin.
CNcomment:\brief 获取GPIO单个引脚输入输出方向。CNend

 \attention \n
N/A
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type.     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[out] pbInput   Pointer to the boolean variable that indicates the direction of a pin.  CNcomment:指针类型，指向布尔型变量，用来返回管脚方向。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NULL_PTR  Pointer Parameters is NULL.                 	CNcomment:指针参数为空指针。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_BOOL    *pbInput);

/**
 \brief Starts the cipher device.
CNcomment:\brief 设置GPIO单个引脚的中断类型。CNend

 \attention \n
N/A
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] enIntType    interrupt type                                CNcomment:中断类型。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_INTTYPE_NOT_SUPPORT  interupt type is not support.       CNcomment:不支持的中断类型。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_FAILED_SETINT  set interupt type failed.              	CNcomment:设置中断类型失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_SetIntType(HI_U32 u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntType);

/**
 \brief set GPIO single pin interrupt enable
CNcomment:\brief 设置GPIO单个引脚的中断使能。CNend
 \attention \n
interrupte type HI_UNF_GPIO_INTTYPE_LOW and HI_UNF_GPIO_INTTYPE_HIGH is not support
CNcomment:不支持HI_UNF_GPIO_INTTYPE_LOW 和 HI_UNF_GPIO_INTTYPE_HIGH中断类型。CNend
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] bEnable  HI_TRUE: interrupt enable, HI_FALSE: interrupt disable CNcomment:HI_TRUE: 中断使能 ,HI_FALSE: 中断禁止。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_FAILED_SETENABLE  enable interupt failed.              	CNcomment:使能中断失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_SetIntEnable(HI_U32 u32GpioNo, HI_BOOL bEnable);

/**
 \brief query GPIO interrupt, report it if there is interrupter happen.\n
CNcomment:\brief 查询GPIO中断，只要有GPIO中断就会上报上来。CNend
 \attention \n
N/A
 \param[out] p32GpioNo    get interrupt pin number.        CNcomment:获取中断的管脚号。CNend
 \param[in] u32TimeoutMs  get interrupt timeout.           CNcomment:获取中断超时时间。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_NULL_PTR  Pointer Parameters is NULL.                 	CNcomment:指针参数为空指针。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_FAILED_GETINT  Query interupt failed.              	CNcomment:查询中断失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_QueryInt(HI_U32 *p32GpioNo, HI_U32 u32TimeoutMs);

/**
 \brief Obtains the output type (od or cmos) of a GPIO pin.
CNcomment:\brief 设置GPIO单个引脚的输出类型。CNend

 \attention \n
N/A
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type.     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] enOutputType   Pointer to the enumerate variable that indicates the direction of a pin.  CNcomment:指针类型，指向枚举型变量，用来返回管脚输出类型。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_FAILED_SETOUTPUTTYPE Get output type failed.				CNcomment:设置输出类型失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType);

/**
 \brief Obtains the output type (od or cmos) of a GPIO pin.
CNcomment:\brief 获取GPIO单个引脚的输出类型。CNend

 \attention \n
N/A
 \param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type.     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[out] penOutputType   Pointer to the enumerate variable that indicates the output type of a pin.  CNcomment:指针类型，指向枚举型变量，用来返回管脚输出类型。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA	 Parameters Invalid.                 		CNcomment:非法参数。CNend
 \retval ::HI_ERR_GPIO_NOT_INIT  GPIO module is not initialiazed.              	CNcomment:GPIO模块没有初始化。CNend
 \retval ::HI_ERR_GPIO_FAILED_GETOUTPUTTYPE Get output type failed.				CNcomment:获取输出类型失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType);


/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_UNF_GPIO_H__ */
