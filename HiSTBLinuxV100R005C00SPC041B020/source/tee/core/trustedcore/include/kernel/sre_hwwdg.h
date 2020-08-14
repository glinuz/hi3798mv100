/**
 * @file sre_hwwdg.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：硬件看门狗模块的对外头文件。 \n
 */
/** @defgroup SRE_hwwdg 看门狗
   *@ingroup SRE_kernel
*/

#ifndef _SRE_HWWDG_H
#define _SRE_HWWDG_H

#include "sre_base.h"
#include "sre_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗超时的处理模式。
 */
typedef enum tagHwWdgProcMode
{
	OS_HWWDG_GENERATE_RESET = 0,        /**< 硬件看门狗超时时，复位单板       */
	OS_HWWDG_GENERATE_NMI   = 1,        /**< 硬件看门狗超时时，产生NMI中断    */
	OS_HWWDG_GENERATE_INT   = 2,        /**< 硬件看门狗超时时，产生可屏蔽中断 */
	OS_HWWDG_GENERATE_BUTT
}
HWWDG_PROC_MODE_E;


/*
 * 硬件看门狗模块的错误码定义。
 */
/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗错误码：硬件看门狗的超时时间间隔为零。
 *
 * 值: 0x02001501
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWWDG_TIMEOUT_ZERO                         SRE_ERRNO_OS_ERROR(OS_MID_HWWDG, 0x01)

/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗错误码：硬件看门狗的处理模式非法。
 *
 * 值: 0x02001502
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWWDG_INVALID_MODE                         SRE_ERRNO_OS_ERROR(OS_MID_HWWDG, 0x02)

/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗错误码：没有可用的硬件定时器资源。
 *
 * 值: 0x02001503
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWWDG_HWTMR_UNAVAILABLE                    SRE_ERRNO_OS_ERROR(OS_MID_HWWDG, 0x03)

/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗错误码：超时间隔过大
 *
 * 值: 0x02001504
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWWDG_HWTMR_UNREACHABLE                    SRE_ERRNO_OS_ERROR(OS_MID_HWWDG, 0x04)

/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗错误码：配置为产生中断，处理函数不能为空
 *
 * 值: 0x02001505
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWWDG_INTHOOK_NULL                         SRE_ERRNO_OS_ERROR(OS_MID_HWWDG, 0x05)

/**
* @ingroup  SRE_hwwdg
* @brief 硬件看门狗功能函数的类型定义。
*
* @par 描述:
* 通过该硬件看门狗功能函数类型定义函数，用于看门狗触发中断的处理函数。
*
* @attention 无
*
* @param  无
*
* @retval 无
* @par 依赖:
* <ul><li>sre_hwwdg.h:该接口声明所在的头文件 </li></ul>
* @since UniDSP V100R001C01
* @see
*/
typedef VOID (* HWWDG_PROC_FUNC)(VOID);

/**
 * @ingroup SRE_hwwdg
 * 硬件看门狗模块配置信息的结构体定义。
 *
 * 保存硬件看门狗模块的配置项信息。
 */
typedef struct tagHwWdgModInfo {
	UINT32 uwInterval;                                      /**< 硬件看门狗超时时间，单位ms */
	UINT16 usProcMode;                                      /**< 硬件看门狗处理模式         */
	UINT16 usResved;                                        /**< 保留                       */
	HWWDG_PROC_FUNC pfnHwwdbHandle;                         /**< 硬件看门狗超时后，如果产生的是可屏蔽中断，则调用此钩子处理*/
	HWI_HANDLE_T uwHwiNum;                                  /**< 硬件看门狗绑定的中断号     */
} HWWDG_MOD_INFO;


/**
 * @ingroup  SRE_hwwdg
 * @brief 清除看门狗。
 *
 * @par 描述:
 * 硬件看门狗的喂狗函数，硬件看门狗计数重新从零开始计数。
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_hwwdg.h:该接口声明所在的头文件 </li></ul>
 * @since UniDSP V100R001C01
 * @see OS_HWWDG_ENABLE | OS_HWWDG_DISABLE
 */
#define OS_HWWDG_SERVICE() \
    do \
    { \
        if (g_pfnHwWdgServiceHook != NULL) \
        { \
            g_pfnHwWdgServiceHook(); \
        } \
    } while (0)


/**
 * @ingroup  SRE_hwwdg
 * @brief 使能看门狗。
 *
 * @par 描述:
 * 使能看门狗后，看门狗重新开始计数。
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_hwwdg.h:该接口声明所在的头文件</li></ul>
 * @since UniDSP V100R001C01
 * @see OS_HWWDG_SERVICE | OS_HWWDG_DISABLE
 */
#define OS_HWWDG_ENABLE() \
    do \
    { \
        if (g_pfnHwWdgEnableHook != NULL) \
        { \
            g_pfnHwWdgEnableHook(); \
        } \
    } while (0)


/**
 * @ingroup  SRE_hwwdg
 * @brief 禁止看门狗。
 *
 * @par 描述:
 * 禁止看门狗后，看门狗定时器停止计数。
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_hwwdg.h:该接口声明所在的头文件</li></ul>
 * @since UniDSP V100R001C01
 * @see OS_HWWDG_SERVICE | OS_HWWDG_ENABLE
 */
#define OS_HWWDG_DISABLE() \
    do \
    { \
        if (g_pfnHwWdgDisableHook != NULL) \
        { \
            g_pfnHwWdgDisableHook(); \
        } \
    } while (0)


/* 若打开看门狗开关，则挂接RTOSck的的处理函数；若未打开看门狗开关，则挂接用的的处理函数*/
EXTERN_FAR HWWDG_PROC_FUNC g_pfnHwWdgServiceHook;       /* 清除看门狗钩子函数 */
EXTERN_FAR HWWDG_PROC_FUNC g_pfnHwWdgEnableHook;        /* 使能看门狗钩子函数 */
EXTERN_FAR HWWDG_PROC_FUNC g_pfnHwWdgDisableHook;       /* 禁止看门狗钩子函数 */



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_HWWDG_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

