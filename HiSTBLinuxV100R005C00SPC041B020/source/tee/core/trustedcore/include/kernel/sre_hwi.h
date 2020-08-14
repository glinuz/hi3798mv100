/**
 * @file sre_hwi.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：硬中断模块的对外头文件。 \n
 */

/**@defgroup SRE_interrupt 中断
  *@ingroup SRE_kernel
*/
/** @defgroup SRE_hwi 硬中断
 *@ingroup SRE_interrupt
*/

#ifndef _SRE_HWI_H
#define _SRE_HWI_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#define SRE_CPU0    0
#define SRE_CPU1    1
#define SRE_CPU2    2
#define SRE_CPU3    3


/**
 * @ingroup SRE_hwi
 * 硬中断最高优先级。
 */
#define SRE_HWI_HIGHEST_PRIORITY      SRE_HWI_PRIORITY0

/**
 * @ingroup SRE_hwi
 * 硬中断支持的优先级数目。
 */
#define SRE_HWI_PRIORITIES_NUMBER     32

/**
 * 硬中断模式的宏定义。
 */

/**
 * @ingroup SRE_hwi
 * 组合型硬中断。
 */
#define OS_HWI_MODE_COMBINE                                 0x8000

/**
 * @ingroup SRE_hwi
 * 独立型硬中断。
 */
#define OS_HWI_MODE_ENGROSS                                 0x4000

/**
 * @ingroup SRE_hwi
 * 缺省硬中断模式。
 */
#define OS_HWI_MODE_DEFAULT                                 OS_HWI_MODE_ENGROSS

/**
 * @ingroup SRE_hwi
 * 作为使能或屏蔽所有硬中断函数的入参，仅Tensilica平台有效。
 */
#define OS_HWI_ALL                                          0x7fff

/*
 * 硬中断模块的错误码定义。
 */

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：中断号（事件号）非法。
 *
 * 值: 0x02001400
 *
 * 解决方案：确保中断号合法，Tensilica:[0,31];
 */
#define OS_ERRNO_HWI_NUM_INVALID                            SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x00)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：硬中断已被创建或相应中断向量号已被其它中断占用。
 *
 * 值: 0x02001402
 *
 * 解决方案：更换中断号
 */
#define OS_ERRNO_HWI_ALREADY_CREATED                        SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x02)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：硬中断处理函数为空。
 *
 * 值: 0x02001403
 *
 * 解决方案：传入非空的有效处理函数
 */
#define OS_ERRNO_HWI_PROC_FUNC_NULL                         SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x03)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：删除TICK中断。
 *
 * 值: 0x02001405
 *
 * 解决方案：更换中断号
 */
#define OS_ERRNO_HWI_DELETE_TICK_INT                        SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x05)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：组合型中断创建失败，为组合型中断节点申请私有静态内存失败
 *
 * 值: 0x02001408
 *
 * 解决方案: 增大私有静态内存分区大小
 */
#define OS_ERRNO_HWI_MEMORY_ALLOC_FAILED                    SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x08)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：组合型中断函数注册失败，该组合型中断已创建了相同的中断处理函数
 *
 * 值: 0x02001409
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWI_COMBINEHOOK_ALREADY_CREATED            SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x09)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：创建的中断函数即不是安全型，也不是非安全型
 *
 * 值: 0x0200140a
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWI_MODE_ERROR                             SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0a)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：删除未创建的硬中断
 *
 * 值: 0x0200140b
 *
 * 解决方案: 无
 */
#define OS_ERRNO_HWI_DELETED                                SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0b)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：未进行硬中断模式设置
 *
 * 值: 0x0200140c
 *
 * 解决方案: 调用中断创建函数前，需要先调用中断模式设置函数，进行模式参数设置
 */
#define OS_ERRNO_HWI_MODE_UNSET                             SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0c)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：硬中断触发接口入参错误，输入无效的核号
 *
 * 值: 0x0200140d
 *
 * 解决方案: 输入本核核号
 */
#define OS_ERRNO_HWI_CORE_ID_INVALID                            SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0d)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：硬中断触发接口入参错误，输入无效的中断号
 *
 * 值: 0x0200140e
 *
 * 解决方案: SD6182的CPU,DSP,HAC CC核，及SD6181/SD6108的CPU核只有7号可以软件触发，SD6181/SD6108的DSP核7号与11号可以软件触发
 */
#define OS_ERRNO_HWI_NUM_NOT_SOFT_TRIGGERD                      SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0e)

/**
 * @ingroup SRE_hwi
 * 硬中断错误码：中断优先级设置有误。
 *
 * 值: 0x0200140f
 *
 * 解决方案：优先级应小于256
 */
#define OS_ERRNO_HWI_PRIORITY_VALUE                       SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x0f)

/**
 * @ingroup SRE_hwi
 * No free slot for HWI IPC notification
 *
 * 值: 0x02001410
 *
 */
#define OS_ERRNO_HWI_IPC_ALL_BUSY                        SRE_ERRNO_OS_ERROR(OS_MID_HWI, 0x10)

/**
 * @ingroup SRE_hwi
 * 硬中断号的类型定义。
 */
typedef UINT32                                              HWI_HANDLE_T;

/**
 * @ingroup SRE_hwi
 * 硬中断优先级的类型定义。
 */
typedef UINT16                                              HWI_PRIOR_T;

/**
 * @ingroup SRE_hwi
 * 硬中断模式配置信息的类型定义。
 */
typedef UINT16                                              HWI_MODE_T;

/**
 * @ingroup SRE_hwi
 * 硬中断处理函数的参数类型定义。
 */
typedef UINT32                                              HWI_ARG_T;

/**
* @ingroup  SRE_hwi
* @brief 硬中断处理函数的类型定义。
*
* @par 描述:
* 通过硬中断处理函数的类型定义硬中断处理函数，在硬中断触发时调用该中断处理函数。
*
* @attention 无。
*
* @param  uwParam1 [IN] 类型#HWI_ARG_T，硬中断处理函数的参数。
*
* @retval 无。
* @par 依赖:
* <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
* @since UniDSP V100R001C01
* @see 无。
*/
typedef VOID (* HWI_PROC_FUNC)(HWI_ARG_T);


/**
 * @ingroup  SRE_hwi
 * @brief 设置硬中断属性接口。
 *
 * @par 描述:
 * 在创建硬中断前，必须要配置好硬中断的模式，
 包括独立型（#OS_HWI_MODE_ENGROSS）和组合型（#OS_HWI_MODE_COMBINE）两种配置模式。
 *
 * @attention
 * <ul>
 * <li>Tensilica下的SD6181, SD6108, SD6182芯片，可用硬中断号（事件号）为0-31。</li>
 * <li>其中SD6181和SD6108的优先级为1-6，SD6182的优先级为1-5。</li>
 * <li>硬中断创建成功后，会使能相应的事件号，但并不使能相应向量的中断，需要显式调用#SRE_HwiEnable单独使能。</li>
 * </ul>
 *
 * @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号。
 * @param  usHwiPrio [IN] 类型#HWI_PRIOR_T，硬中断优先级，由于Tensilica平台下优先级由硬件固定，故此参数无效。
 * @param  usMode [IN] 类型#HWI_MODE_T，该硬中断的工作模式，独立型或者组合型。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                     0x02001400，中断号（事件号）非法。
 * @retval #OS_ERRNO_HWI_MODE_ERROR                      0x0200140a，创建的中断函数即不是独立型，也不是组合型。
 * @retval #OS_ERRNO_HWI_ALREADY_CREATED                 0x02001402，重复创建硬中断函数。
 * @retval #SRE_OK                                       0x00000000，硬中断创建成功。
 * @par 依赖:
 * <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_HwiCreate
 **/

/**
 * @ingroup  SRE_hwi
 * @brief 创建硬中断函数。
 *
 * @par 描述:
 * 注册硬中断的处理函数。
 *
 * @attention
 * <ul>
 * <li>在调用该函数之前，请先确保已经设置了中断属性。</li>
 * <li>Tensilica下的SD6181, SD6108, SD6182芯片，可用硬中断号（事件号）为0-31。</li>
 * <li>其中SD6181和SD6108的优先级为1-6，SD6182的优先级为1-5。</li>
 * <li>硬中断创建成功后，会使能相应的事件号，但并不使能相应向量的中断，需要显式调用#SRE_HwiEnable单独使能。</li>
 * </ul>
 *
 * @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号。
 * @param  pfnHandler [IN] 类型#HWI_PROC_FUNC，硬中断触发时的处理函数。
 * @param  uwArg [IN] 类型#HWI_ARG_T，调用硬中断处理函数时传递的参数。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                     0x02001400，中断号（事件号）非法。
 * @retval #OS_ERRNO_HWI_PROC_FUNC_NULL                  0x02001403，硬中断处理函数为空。
 * @retval #OS_ERRNO_HWI_MODE_UNSET                      0x0200140c，未进行硬中断模式设置。
 * @retval #OS_ERRNO_HWI_MEMORY_ALLOC_FAILED             0x02001408，组合型中断节点申请私有静态内存失败
 * @retval #OS_ERRNO_HWI_COMBINEHOOK_ALREADY_CREATED     0x02001409，组合型中断已创建了相同的中断处理函数。
 * @retval #OS_ERRNO_HWI_ALREADY_CREATED                 0x02001402，硬中断已被创建或相应中断向量号已被其它中断占用。
 * @retval #SRE_OK                                       0x00000000，硬中断创建成功。
 * @par 依赖:
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_HwiDelete
 **/
extern UINT32 SRE_HwiCreate(HWI_HANDLE_T  uwHwiNum,
			    HWI_PRIOR_T   usHwiPrio,
			    HWI_MODE_T    usMode,
			    HWI_PROC_FUNC pfnHandler,
			    HWI_ARG_T     uwArg);
extern UINT32 SRE_HwiSetTargetCPU(HWI_HANDLE_T uwHwiNum);

/**
 * @ingroup  SRE_hwi
 * @brief 删除硬中断函数。
 *
 * @par 描述:
 * 屏蔽相应硬中断或事件，取消硬中断处理函数的注册。
 *
 * @attention
 * <ul>
 * <li>Tensilica下的SD6181, SD6108, SD6182芯片，可用硬中断号（事件号）为0-31。</li>
 * </ul>
 *
 * @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID            0x02001400，中断号（事件号）非法。
 * @retval #OS_ERRNO_HWI_DELETE_TICK_INT        0x02001405，删除TICK中断。
 * @retval #OS_ERRNO_HWI_DELETED                0x0200140b，删除未创建的硬中断。
 * @retval #SRE_OK                              0x00000000，硬中断删除成功。
 * @par 依赖:
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_HwiCreate
 */
extern UINT32 SRE_HwiDelete(HWI_HANDLE_T uwHwiNum);

/**
* @ingroup  SRE_hwi
* @brief 还原指定中断号的中断使能寄存器的值。
*
* @par 描述:
* 将之前调用#uniHwiDisable、#uniHwiEnable的返回值，重新填入中断使能寄存器。
*
* @attention
* <ul>
* <li>针对Tensilica平台有效。</li>
* </ul>
*
* @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号。
* @param  uvHwiSave [IN] 类型#UINTPTR，中断使能寄存器的保存值。
*
* @retval 无
* @par 依赖:
* <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
* @since RTOSck V100R001C01
* @see SRE_HwiEnable | SRE_HwiDisable
*/
extern VOID SRE_HwiRestore(HWI_HANDLE_T  uwHwiNum, UINTPTR  uvHwiSave);

/**
* @ingroup  SRE_hwi
* @brief 激活指定核号内的硬中断。
*
* @par 描述:
*
*
* @attention
* <ul>
* <li>针对Tensilica平台有效。</li>
* </ul>
*
* @param  uwDstCore [IN] 类型#UINT32， 目标核号。 目前只支持指定为本核。
* @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号，只支持软件可触发的中断号（芯片差异性）。
*
* @retval #OS_ERRNO_HWI_NUM_INVALID                0x02001400，中断号（事件号）非法。
* @retval #OS_ERRNO_HWI_CORE_ID_INVALID            0x0200140d，输入无效的核号。
* @retval #OS_ERRNO_HWI_NUM_NOT_SOFT_TRIGGERD      0x0200140e，输入无效的中断号，软件不可触发。
* @retval #SRE_OK                                  0x00000000，硬中断请求位清除成功。
* @par 依赖:
* <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
* @since RTOSck V100R001C01
*/
extern UINT32 SRE_HwiTrigger(UINT32 uwMode, UINT32 uwDstCore , HWI_HANDLE_T uwHwiNum);

/**
 * @ingroup  SRE_hwi
 * @brief 清除硬中断的Pending位。
 *
 * @par 描述:
 * 显式清除硬中断或事件的请求位，因为有的硬件响应中断后不会自动清Pending位。
 *
 * @attention
 * <ul>
 * <li>对于Tensilica，此参数为中断向量号0-31。</li>
 * </ul>
 *
 * @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，硬中断号。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID            0x02001400，中断号（事件号）非法。
 * @retval #SRE_OK                              0x00000000，硬中断请求位清除成功。
 * @par 依赖:
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_HwiCreate
 */
extern UINT32 SRE_HwiClearPendingBit(HWI_HANDLE_T uwHwiNum);


/**
* @ingroup  SRE_hwi
* @brief 屏蔽指定的硬中断。
*
* @par 描述:
* 禁止DSP响应指定硬中断的请求。
*
* @attention
* <ul>
* <li>对于Tensilica，此参数为中断向量号0-31。</li>
* <li>Tensilica特有功能:当入参为OS_HWI_ALL时，可屏蔽所有硬中断。</li>
* </ul>
*
* @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，依据不同的芯片，硬中断号或中断向量号，见注意事项。
*
* @retval #屏蔽前的中断使能寄存器的值。

* @par 依赖:
* <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
* @since RTOSck V100R001C01
* @see SRE_HwiEnable
*/
extern UINTPTR SRE_HwiDisable(HWI_HANDLE_T uwHwiNum);

/**
* @ingroup  SRE_hwi
* @brief 使能指定的硬中断。
*
* @par 描述:
* 允许DSP响应指定硬中断的请求。
*
* @attention
* <ul>
* <li>对于Tensilica，此参数为中断向量号0-31。</li>
* <li>Tensilica特有功能:当入参为OS_HWI_ALL时，可使能所有硬中断。</li>
* <li>对于不同芯片，此返回值代表的意义有所差异，差异细节见下面返回值说明</li>
* </ul>
*
* @param  uwHwiNum [IN] 类型#HWI_HANDLE_T，依据不同的芯片，硬中断号或中断向量号，见注意事项。
*
* @retval #使能前的中断使能寄存器的值。

* @par 依赖:
* <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
* @since RTOSck V100R001C01
* @see SRE_HwiDisable
*/
extern UINTPTR SRE_HwiEnable(HWI_HANDLE_T uwHwiNum);
extern UINT32 SRE_OsIntLock(UINT32 *puwSpsr);
extern VOID SRE_OsIntRestore(UINT32 *puwSpsr, UINT32 uwR0);
extern UINT32 SRE_OsIntUnLock(UINT32 *puwSpsr);
extern UINT32 SRE_GetCurrCPSR(VOID);
extern UINT32 SRE_GetCurrSPSR(VOID);


extern UINT32 osHwiIpcInit(VOID);
extern UINT32 SRE_HwiMsgRegister(HWI_HANDLE_T uwHwiNum);
extern UINT32 SRE_HwiMsgDeregister(HWI_HANDLE_T uwHwiNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_HWI_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

