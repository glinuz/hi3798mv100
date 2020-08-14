/**
 * @file sre_sys.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：系统模块的对外头文件。 \n
 */


/** @defgroup SRE_sys 系统基本功能接口 */

#ifndef _SRE_SYS_H
#define _SRE_SYS_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/*
 * 系统模块的错误码定义。
 */

/**
 * @ingroup SRE_sys
 * 系统基本功能错误码：指针参数为空。
 *
 * 值: 0x02000011
 *
 * 解决方案: 无
 */
#define OS_ERRNO_SYS_PTR_NULL                               SRE_ERRNO_OS_ERROR(OS_MID_SYS, 0x11)

/**
 * @ingroup SRE_sys
 * 系统基本功能错误码：系统主频配置非法。
 *
 * 值: 0x02000012
 *
 * 解决方案: 在Config.h中配置合理的系统主频。
 */
#define OS_ERRNO_SYS_CLOCK_INVALID                          SRE_ERRNO_OS_ERROR(OS_MID_SYS, 0x12)

/**
 * @ingroup SRE_sys
 * 系统基本功能错误码：用户的配置选项OS_SYS_CORE_MAX_NUM有误，应该和芯片匹配。
 *
 * 值: 0x02000013
 *
 * 解决方案:修改配置选项OS_SYS_CORE_MAX_NUM，和芯片匹配。
 *
 */
#define OS_ERRNO_SYS_MAXNUMOFCORES_IS_INVALID               SRE_ERRNO_OS_ERROR(OS_MID_SYS, 0x13)

/**
 * @ingroup SRE_sys
 * 系统基本功能错误码：外设异常接管核配置错误。
 *
 * 值: 0x02000014
 *
 * 解决方案:外设异常接管核只能是CPU12-CPU16核中的一个,且所有镜像的config配置的核号必须一致。
 *
 */
#define OS_ERRNO_SYS_PERIERRCOREID_IS_INVALID               SRE_ERRNO_OS_ERROR(OS_MID_SYS, 0x14)

/**
 * @ingroup SRE_sys
 * 系统基本功能错误码：钩子已达到最大允许的注册数。
 *
 * 值: 0x02000017
 *
 * 解决方案:避免重复注册
 *
 */
#define OS_ERRNO_SYS_HOOK_IS_FULL                           SRE_ERRNO_OS_ERROR(OS_MID_SYS, 0x17)

extern UINT32               g_uwSystemClock;
extern UINT64               g_ullUniTicks;
/**
 * @ingroup  SRE_sys
 * @brief 获取本核的tick计数私有地址
 *
 * @par 描述:
 * 获取本核的tick计数私有地址
 *
 * @attention 无
 *
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无
 */
#define SRE_GetTickAddr()   ((UINT32)(UINT32 *)&g_ullUniTicks)

#define SRE_SYS_MS_PER_SECOND                               1000
#define SRE_SYS_US_PER_SECOND                               1000000

/**
 * @ingroup  SRE_sys
 * @brief 转换cycle为毫秒
 *
 * @par 描述:
 * 转换cycle为毫秒
 *
 * @attention 无
 *
 * @param  udwCycle    [IN] 类型为#UINT64，cycle数
 *
 * @retval 0~0xFFFFFFFFFFFFFFFF 转换后的毫秒数
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_Cycle2US
 */
OS_SEC_ALW_INLINE INLINE UINT64 SRE_Cycle2MS(UINT64 udwCycle)
{
	return (UINT64)((udwCycle / (g_uwSystemClock / SRE_SYS_MS_PER_SECOND)));
}

/**
 * @ingroup  SRE_sys
 * @brief 转换cycle为微秒
 *
 * @par 描述:
 * 转换cycle为微秒
 *
 * @attention 无
 *
 * @param  udwCycle    [IN] 类型为#UINT64，cycle数
 *
 * @retval 0~0xFFFFFFFFFFFFFFFF 转换后的微秒数
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_Cycle2MS
 */
OS_SEC_ALW_INLINE INLINE UINT64 SRE_Cycle2US(UINT64 udwCycle)
{
	return (UINT64)((udwCycle / (g_uwSystemClock / SRE_SYS_US_PER_SECOND)));
}


/**
 * @ingroup  SRE_sys
 * @brief 获取当前的tick计数
 *
 * @par 描述:
 * 获取当前的tick计数
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval  0~0xFFFFFFFFFFFFFFFF 当前的tick计数
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CycleCountGet
 */
extern UINT64 SRE_TickCountGet(VOID);

/**
 * @ingroup  SRE_sys
 * @brief 获取每个tick对应的硬件cycle数
 *
 * @par 描述:
 * 获取每个tick对应的硬件cycle数
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval 0~0xFFFFFFFF 每个tick对应的硬件cycle数
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无
 */
extern UINT32 SRE_CyclePerTickGet(VOID);

/**
 * @ingroup  SRE_sys
 * @brief 延迟时间(单位cycle)
 *
 * @par 描述:
 * 延迟时间(单位cycle)
 *
 * @attention 无
 *
 * @param  udwCycle    [IN] 类型为#UINT32，cycle数
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无
 */
extern VOID SRE_DelayCycle(UINT32 uwCycles);

/**
 * @ingroup  SRE_sys
 * @brief 延迟时间(单位微秒)
 *
 * @par 描述:
 * 延迟时间(单位微秒)
 *
 * @attention 无
 *
 * @param  uwDelay    [IN] 类型为#UINT32，微秒数
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_DelayMs
 */
extern VOID SRE_DelayUs(UINT32 uwDelay);

/**
 * @ingroup  SRE_sys
 * @brief 延迟时间(单位毫秒)
 *
 * @par 描述:
 * 延迟时间(单位毫秒)
 *
 * @attention 无
 *
 * @param  uwDelay    [IN] 类型为#UINT32，毫秒数
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_DelayUs
 */
extern VOID SRE_DelayMs(UINT32 uwDelay);

/**
 * @ingroup  SRE_sys
 * @brief 系统重启钩子函数的类型定义。
 *
 * @par 描述:
 * 用户通过系统重启钩子函数的类型定义系统重启钩子，在系统重启之前调用该钩子。
 *
 * @attention 无。
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无。
*/

typedef VOID (*SYS_REBOOT_FUNC)(VOID);

/**
 * @ingroup  SRE_sys
 * @brief IDLE循环钩子的类型定义。
 *
 * @par 描述:
 * 用户通过IDLE循环钩子的函数类型定义函数，系统在进入IDLE循环之前调用该钩子。
 *
 * @attention 无。
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无。
*/
typedef VOID (* OS_IDLE_HOOK)(VOID);


/**
 * @ingroup SRE_sys
 * 系统模块配置信息的结构体定义。
 *
 * 保存系统模块的配置项信息。
 */
typedef struct tagSysModInfo {
	UINT32 uwSystemClock;                                   /**< CPU主频，时钟周期          */
	UINT8  ucCoreID;                                        /**< 当前核ID                   */
	UINT8  ucCoreRunNum;                                    /**< 实际运行的核数             */
	UINT8  ucCoreMaxNum;                                    /**< 最大支持的核数             */
	UINT8  ucCoreMaster;                                    /**< 主核ID                     */
	SYS_REBOOT_FUNC pfnRebootHook;                          /**< 用户注册的单板复位函数     */
	OS_IDLE_HOOK pfnIdleHook;                               /**< 用户注册的IDLE钩子函数     */
	UINT8  ucCpuType;                                       /**< CPU type                   */
	UINT8  ucPeriErrCoreID;                                 /**< 外设错误接管核             */
	UINT16 usResved;
} SYS_MOD_INFO_S;


/**
 * @ingroup  SRE_sys
 * @brief 复位单板。
 *
 * @par 描述:
 * 复位单板，程序重新加载执行。
 *
 * @attention
 * <ul>
 * <li>并非直接复位单板，而是关中断，等待看门狗复位。</li>
 * <li>用户可以通过配置项OS_SYS_REBOOT_HOOK挂接复位函数。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see 无
 */
extern VOID SRE_Reboot(VOID);

/**
 * @ingroup  SRE_sys
 * @brief 当前核ID。
 *
 * @par 描述:
 * 当前核ID。
 *
 * @attention
 * <ul>
 * <li>核ID为硬件寄存器中的ID号。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 物理核ID。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see g_ucMasterCoreID
 */
extern UINT8 g_ucLocalCoreID;

/**
 * @ingroup  SRE_sys
 * @brief 获取当前核ID。
 *
 * @par 描述:
 * 获取当前核ID。
 *
 * @attention
 * <ul>
 * <li>获取的核ID为硬件寄存器中的ID号。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 物理核ID。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_GetNumberOfCores | SRE_GetMasterCore
 */
#define SRE_GetCoreID()          (UINT8)(g_ucLocalCoreID)

/**
 * @ingroup  SRE_sys
 * @brief 主核ID。
 *
 * @par 描述:
 * 主核ID。
 *
 * @attention
 * <ul>
 * <li>核ID为硬件寄存器中的ID号。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 物理核ID。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see g_ucLocalCoreID
 */
extern UINT8 g_ucMasterCoreID;

/**
 * @ingroup  SRE_sys
 * @brief 获取主核ID。
 *
 * @par 描述:
 * 获取主核ID。
 *
 * @attention
 * <ul>
 * <li>获取的核ID为硬件寄存器中的ID号。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 物理核ID。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_GetNumberOfCores | SRE_GetCoreID
 */
#define SRE_GetMasterCore()      ((UINT8)(g_ucMasterCoreID + (UINT8)0))

/**
 * @ingroup  SRE_sys
 * @brief 实际运行的核数。
 *
 * @par 描述:
 * 实际运行的核数。
 *
 * @attention
 * <ul>
 * <li>实际运行的核数，而不是RTOSck最大支持的核数。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 核数。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since  RTOSck V100R001C01
 * @see g_ucMaxNumOfCores
 */
extern UINT8 g_ucNumOfCores;

/**
 * @ingroup  SRE_sys
 * @brief 最大可支持运行的核数。
 *
 * @par 描述:
 * 最大可支持运行的核数。
 *
 * @attention
 * <ul>
 * <li>为RTOSck最大支持的核数，而不是RTOSck实际运行的核数。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 核数。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since  RTOSck V100R001C01
 * @see g_ucNumOfCores
 */
extern UINT8 g_ucMaxNumOfCores;
/**
 * @ingroup  SRE_sys
 * @brief 获取实际运行的核数。
 *
 * @par 描述:
 * 获取实际运行的核数。
 *
 * @attention
 * <ul>
 * <li>获取的为实际运行的核数，而不是OS最大支持的核数。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 核数。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since  RTOSck V100R001C01
 * @see SRE_GetCoreID | SRE_GetMasterCore
 */
#define SRE_GetNumberOfCores()       ((UINT8)(g_ucNumOfCores + (UINT8)0))

/**
 * @ingroup  SRE_sys
 * @brief 获取最大可支持运行的核数。
 *
 * @par 描述:
 * 获取最大可支持运行的核数。
 *
 * @attention
 * <ul>
 * <li>获取的为OS最大支持的核数，而不是OS实际运行的核数。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 核数。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since  RTOSck V100R001C01
 * @see SRE_GetCoreID | SRE_GetMaxNumberOfCores
 */
#define SRE_GetMaxNumberOfCores()     ((UINT8)(g_ucMaxNumOfCores + (UINT8)0))

/**
 * @ingroup  SRE_sys
 * @brief 获取系统的主频。
 *
 * @par 描述:
 * 获取系统的主频。
 *
 * @attention
 *
 * @param
 *
 * @retval 系统主频
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_GetCoreID
 */
extern UINT32 SRE_GetSysClk(VOID);

/**
 * @ingroup  SRE_sys
 * @brief 获取OS的版本号。
 *
 * @par 描述:
 * 获取OS的版本号。版本编号格式为RTOSck Vx00R00xCxx SVNID(XXX)
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval 指向OS版本号的字符串指针。 如：RTOSck V100R001C01 SVNID (XXX) 。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SetAppVersion | SRE_GetAppVersion
 */
extern CHAR *SRE_GetOsVersion(VOID);


/**
 * @ingroup  SRE_sys
 * @brief 注册应用程序的版本号。
 *
 * @par 描述:
 * 注册应用程序的版本号。
 *
 * @attention
 * <ul>
 * <li>系统APP版本号保存在全局数组中，可以设置的APP版本号最大长度为64字节</li>
 * </ul>
 *
 * @param  pcAppVersion [IN] 类型#CHAR *，指向应用程序版本号的字符串指针。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL               0x02000011，指针参数为空。
 * @retval #SRE_OK                          0x02000000，注册APP版本号成功。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_GetAppVersion | SRE_GetOsVersion
 */
extern UINT32 SRE_SetAppVersion(CHAR *pcAppVersion);


/**
 * @ingroup  SRE_sys
 * @brief 获取应用程序的版本号。
 *
 * @par 描述:
 * 获取应用程序的版本号。
 *
 * @attention 若之前未注册，则返回指向"Unknown"的指针。
 *
 * @param  无
 *
 * @retval 指向应用程序版本号的字符串指针。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SetAppVersion | SRE_GetOsVersion
 */
extern CHAR *SRE_GetAppVersion(VOID);


/**
 * @ingroup  SRE_sys
 * @brief 将cycle值转换成毫秒。
 *
 * @par 描述:
 * 将cycle值转换成毫秒。
 *
 * @attention 当系统主频不为1000000整数倍时，该接口获取到的us数有误差。
 *
 * @param  pstCpuTick [IN] 类型#CPU_TICK *，cycle计数值。
 * @param  puwMsHi [IN] 类型#UINT32 *，保存毫秒的高32位。
 * @param  puwMsLo [IN] 类型#UINT32 *，保存毫秒的低32位。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL               0x02000011，指针参数为空。
 * @retval #SRE_OK                          0x02000000，操作成功。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CpuTick2US
 */
extern UINT32 SRE_CpuTick2MS(CPU_TICK *pstCpuTick, UINT32 *puwMsHi, UINT32 *puwMsLo);


/**
 * @ingroup  SRE_sys
 * @brief 将cycle值转换成微秒。
 *
 * @par 描述:
 * 将cycle值转换成微秒。
 *
 * @attention 无
 *
 * @param  pstCpuTick [IN] 类型#CPU_TICK *，cycle计数值。
 * @param  puwUsHi [IN] 类型#UINT32 *，保存微秒的高32位。
 * @param  puwUsLo [IN] 类型#UINT32 *，保存微秒的低32位。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL               0x02000011，指针参数为空。
 * @retval #SRE_OK                          0x00000000，操作成功。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CpuTick2MS
 */
extern UINT32 SRE_CpuTick2US(CPU_TICK *pstCpuTick, UINT32 *puwUsHi, UINT32 *puwUsLo);


/**
 * @ingroup  SRE_sys
 * @brief 注册IDLE循环进入前调用的钩子。
 *
 * @par 描述:
 * 为本核注册IDLE循环进入前调用的钩子，该钩子只会被调用一次。
 *
 * @attention
 * <ul>
 * <li>注册的钩子只在进入IDLE循环前执行一次。</li>
 * <li>若任务未裁剪，则作用的是任务IDLE循环；若任务被裁剪，则作用的是软中断IDLE循环。</li>
 * </ul>
 *
 * @param  pfnHook    [IN] 类型为#OS_IDLE_HOOK，钩子函数。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL               0x02000011，指针参数为空。
 * @retval #OS_ERRNO_SYS_HOOK_IS_FULL           0x02000016，IDLE钩子已达到最大允许的注册数。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_IdleHookAdd
 */
extern UINT32 SRE_IdlePrefixHookAdd(OS_IDLE_HOOK pfnHook);


/**
 * @ingroup  SRE_sys
 * @brief 注册IDLE循环中调用的钩子
 *
 * @par 描述:
 * 注册在IDLE任务或IDLE软中断循环中调用的钩子函数，最多只能注册一个，NULL表示取消钩子注册。
 *
 * @attention
 *<ul>
 * <li>钩子中不能调用引起任务阻塞或切换的函数。</li>
 * <li>如果在sre_config.h中配置了OS_TSK_IDLE_HOOK再调用该函数会返回失败。</li>
 * </ul>
 *
 * @param  pfnHook [OUT] 类型#OS_IDLE_HOOK，IDLE钩子函数，该参数可以为空。
 *
 * @retval #SRE_OK                  0x00000000，注册成功。
 * @retval #OS_ERRNO_SYS_HOOK_IS_FULL   0x02000016，IDLE钩子已达到最大允许的注册数。
 * @par 依赖:
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_IdlePrefixHookAdd
 */
#if defined(TESTSUITE_RTOSck_UT) || defined(TESTSUITE_RTOSck_PT)||defined(TESTSUITE_RTOSck_IT)
extern UINT32 SRE_IdleHookAdd(OS_IDLE_HOOK pfnHook);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_SYS_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

