/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name       : hi_unf_wdg.h
Version         : Initial draft
Author          : HiSilicon multimedia software group
Created Date    : 2008-06-05
Last Modified by:
Description     : Application programming interfaces (APIs) of the external chip software (ECS)
Function List   :
Change History  :
******************************************************************************/
#ifndef __HI_UNF_WDG_H__
#define __HI_UNF_WDG_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_UNF_WDG_Open     HI_UNF_WDG_Init
#define HI_UNF_WDG_Close    HI_UNF_WDG_DeInit

/******************************* API Declaration *****************************/
/** \addtogroup      WDG*/
/** @{*/  /** <!-- [WDG] */

/**
 \brief Starts the WDG device.
CNcomment:\brief 初始化WDG（Watch Dog）设备。CNend

 \attention \n
By default, the WDG device is disabled after it is started. In this case, you need to call HI_UNF_WDG_Enable to enable it.\n
CNcomment:打开之后，WDG默认是禁止的，需要显式调用HI_UNF_WDG_Enable使能WDG设备。CNend\N

 \param N/A          	CNcomment:无。CNend
 \retval 0 Success.   	CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_FAILED_INIT	open failed
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_Init(HI_VOID);

/**
 \brief Stops the WDG device.
CNcomment:\brief 去初始化WDG设备。CNend

 \attention \n
N/A
 \param N/A          	CNcomment:无。CNend
 \retval 0 Success.   	CNcomment:成功。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_DeInit(HI_VOID);

/**
 \brief Get the number of WDG device.
CNcomment:\brief 获取WDG设备数量。CNend

 \attention \n
You can call this API to get thenumber of WDG chipset supports after the WDG device is started.
CNcomment:打开WDG设备后，调用此接口获取芯片支持的WDG的数量。CNend

 \param[out] pu32WdgNum  The number of WDG chipset supports        	CNcomment:芯片支持的WDG的数量。CNend
 \retval 0 Success  CNcomment:成功 CNend
 \retval ::HI_FAILURE The Parameter pu32WdgNum is NULL.       CNcomment:参数指针pu32WdgNum为空。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_GetCapability(HI_U32 *pu32WdgNum);

/**
 \brief Enables the WDG device.
CNcomment:\brief 使能WDG设备。CNend

 \attention \n
You must call HI_UNF_WDG_Enable after the WDG device is started.
CNcomment:打开WDG设备后，必须显式调用使能接口。CNend

 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \retval 0 Success.  	CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT The WDG device is not initialized.       CNcomment:WDG设备未初始化。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA The Paramteter is invalid. 			CNcomment:参数无效。CNend
 \retval ::HI_ERR_WDG_FAILED_ENABLE enable watchdog failed.				CNcomment:使能看门狗失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_Enable(HI_U32 u32WdgNum);

/**
 \brief Disables the WDG device.
CNcomment:\brief 禁止WDG设备。CNend

 \attention \n
After calling this API, you cannot feed and reset the WDG.
CNcomment:调用此函数后，喂狗和复位操作不起作用。CNend

 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG设备未初始化。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA The Paramteter is invalid. 			CNcomment:参数无效。CNend
 \retval ::HI_ERR_WDG_FAILED_DISABLE  disable watchdog failed.			CNcomment:禁止看门狗失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_Disable(HI_U32 u32WdgNum);

/**
 \brief Obtains the interval of feeding the WDG.
CNcomment:\brief 获取喂狗时间间隔。CNend

 \attention \n
The interval precision is as high as 1000 ms.
CNcomment:时间间隔精确到1000ms。CNend

 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \param[in] pu32Value  Interval of feeding the WDG, in ms.             	CNcomment:喂狗时间间隔，单位为ms。CNend
 \retval 0 Success.                                                  	CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.     	CNcomment:WDG 设备未初始化。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA  The WDG input pointer is invalid.  	CNcomment:WDG输入指针无效。CNend
 \retval ::HI_ERR_WDG_FAILED_SETTIMEOUT get timeout failed.			  	CNcomment:WDG获取超时时间失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_GetTimeout(HI_U32 u32WdgNum, HI_U32 *pu32Value);

/**
 \brief Sets the interval of feeding the WDG.
CNcomment:\brief 设置喂狗时间间隔。CNend

 \attention \n
N/A
 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \param[out] u32Value  Interval of feeding the WDG, in ms.                		CNcomment:喂狗时间间隔，单位为ms。CNend
 \retval 0 Success.                                                      		CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT The WDG device is not initialized.       		CNcomment:WDG设备未初始化。CNend
 \retval ::HI_ERR_WDG_FAILED_SETTIMEOUT The WDG set timeout failed.   			CNcomment:WDG设置超时时间失败。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. 		CNcomment:WDG输入参数无效。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_SetTimeout(HI_U32 u32WdgNum, HI_U32 u32Value);

/**
 \brief Feeds the WDG.
CNcomment:\brief 执行喂狗操作。CNend

 \attention \n
N/A
 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \retval 0 Success.                                                   	CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG设备未初始化。CNend
 \retval ::HI_ERR_WDG_FAILED_CLEARWDG  The WDG clear watchdog failed.   CNcomment:WDG 喂狗失败。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA The Paramteter is invalid. 			CNcomment:参数无效。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_Clear(HI_U32 u32WdgNum);

/**
 \brief Resets the entire system.
CNcomment:\brief 用于复位整个系统。CNend

 \attention \n
N/A
 \param[in] u32WdgNum WDG No. to operate.        	CNcomment:执行操作的WDG号。CNend
 \retval 0 Success. CNcomment:成功。CNend
 \retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.  CNcomment:WDG设备未初始化。CNend
 \retval ::HI_ERR_WDG_FAILED_RESET The WDG reset failed.   			CNcomment:WDG复位失败。CNend
 \retval ::HI_ERR_WDG_INVALID_PARA The Paramteter is invalid. 		CNcomment:参数无效。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_WDG_Reset(HI_U32 u32WdgNum);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HI_UNF_WDG_H__ */
