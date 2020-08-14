/**
 * @file sre_base.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：RTOSck的基础头文件。 \n
 */

/** @defgroup SRE_memory  内存接口 */
/** @defgroup SRE_kernel  内核接口 */
/** @defgroup SRE_comm    通信接口 */
/** @defgroup SRE_inspect 调试接口 */
/** @defgroup v_Patch     补丁接口 */

#ifndef _SRE_BASE_H
#define _SRE_BASE_H

#include "sre_buildef.h"
#include "sre_typedef.h"
#include "sre_module.h"
#include "sre_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define osTskScheduleFast osTskSchedule
#define osTaskTrap  osSwiTrap
#define OS_ASSERT
#define OS_ASSERT_COND(expression)

#define ALIGN(uwAddr, uwBoundary)       (((uwAddr) + (uwBoundary) - 1) & ~((uwBoundary) - 1))
#define TRUNCATE(uwAddr, uwSize)        ((uwAddr) & ~((uwSize) - 1))

#define READ_UINT8(ucValue, ucArg)                          ucValue = (UINT8)(ucArg)
#define READ_UINT16(usValue, usArg)                         usValue = (UINT16)(usArg)
#define READ_UINT32(uwValue, uwArg)                         uwValue = (UINT32)(uwArg)
#define READ_UINT64(ullValue, ullArg)                       ullValue = (UINT64)(ullArg)
#define GET_UINT8(ucArg)                                    (UINT8)(ucArg)
#define GET_UINT16(usArg)                                   (UINT16)(usArg)
#define GET_UINT32(uwArg)                                   (UINT32)(uwArg)
#define GET_UINT64(ullArg)                                  (UINT64)(ullArg)
#define WRITE_UINT8(ucArg, ucValue)                         ucArg = (UINT8)(ucValue)
#define WRITE_UINT16(usArg, usValue)                        usArg = (UINT16)(usValue)
#define WRITE_UINT32(uwArg, uwValue)                        uwArg = (UINT32)(uwValue)
#define WRITE_UINT64(ullArg, ullValue)                      ullArg = (UINT64)(ullValue)

#define W_DWB {asm volatile ("dsb");}

#define write32(addr, val) {W_DWB; \
        (*(volatile unsigned int *)  (addr) = (val));\
        W_DWB;}
#define  read32(addr)      (*(volatile unsigned int *)  (addr))
#define write16(addr, val)  {W_DWB; \
        (*(volatile unsigned short *)(addr) = (val));\
         W_DWB;}
#define  read16(addr)      (*(volatile unsigned short *)(addr))
/**
 * @ingroup  SRE_sys
 * 从线程PID获取核内线程handle号
 */
#define GET_HANDLE(pid)               ((pid)&0x000000FF)
/**
 * @ingroup  SRE_sys
 * 从线程PID获取核号
 */
#define GET_COREID(pid)               ((UINT8)(((pid)&0x0000FF00) >> 8))
/**
 * @ingroup  SRE_sys
 * 将coreid与handle组成PID
 */
#define COMPOSE_PID(coreid, handle)   ((((UINT32)(coreid))<<8) + ((UINT8)(handle))) /* 将coreid与handle组成PID,UIPC不使用该接口 */
/**
 * @ingroup  SRE_sys
 * 核间功能模块的未初始化标志位
 */
#define OS_MC_PHASE_UNINIT                                  0
/**
 * @ingroup  SRE_sys
 * 核间功能模块的正在初始化标志位
 */
#define OS_MC_PHASE_HANDLE                                  0xae1234ea
/**
 * @ingroup  SRE_sys
 * 核间功能模块的初始化完成标志位
 */
#define OS_MC_PHASE_INITED                                  0xad5678da

/**
 * @ingroup  SRE_sys
 *核间信号量、信号量、消息等待时间设定：表示不等待。
 */
#define OS_NO_WAIT            0

/**
 * @ingroup  SRE_sys
 * CpuTick结构体类型。
 *
 * 用于记录64位的cycle计数值。
 */
typedef struct tagCpuTick {
	UINT32 uwCntHi;         /**<  cycle计数高32位 */
	UINT32 uwCntLo;         /**<  cycle计数低32位 */
} CPU_TICK;


/**
 * @ingroup  SRE_sys
 * @brief 开中断。
 *
 * @par 描述:
 * 开启全局可屏蔽中断。
 *
 * @attention 中断服务函数里慎用该接口，会引起中断优先级反转
 *
 * @param 无
 *
 * @retval 开启全局中断前的中断状态值。
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_IntLock | SRE_IntRestore
 */
OS_SEC_ALW_INLINE INLINE UINTPTR SRE_IntUnLock(VOID)
{
	UINTPTR uwCpsSave;
	__asm__ __volatile__(
		"MRS   %0, cpsr\n"
		"BIC   %0, %0, #0xc0\n"
		"MSR   cpsr_c, %0"
		:"=r"(uwCpsSave) : "0"(uwCpsSave));
	return uwCpsSave;
}

/**
 * @ingroup  SRE_sys
 * @brief 关中断。
 *
 * @par 描述:
 * 关闭全局可屏蔽中断。
 *
 * @attention 在关全局中断后，禁止调用引起内核调度的相关接口，如SRE_TaskDelay接口
 *
 * @param 无
 *
 * @retval 关闭全局中断前的中断状态值。
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_IntUnLock | SRE_IntRestore
 */
OS_SEC_ALW_INLINE INLINE UINTPTR SRE_IntLock(VOID)
{
	/*DTS2013040106948--start*/
	UINTPTR uvIntSave;

	__asm__ __volatile__(
		"mrs %0, cpsr"
		: "=r"(uvIntSave));

	__asm__ __volatile__("cpsid if");


	return uvIntSave;

	/*DTS2013040106948--end*/
}

/**
 * @ingroup  SRE_sys
 * @brief 恢复中断状态接口。
 *
 * @par 描述:
 * 恢复原中断状态寄存器。
 *
 * @attention
 *<ul>
 *<li>该接口必须和关闭全局中断或者是开启全局中断接口成对使用，以关全局中断或者开全局中断操作的返回值为入参</li>
 *<li>以保证中断可以恢复到关全局中断或者开全局中断操作前的状态</li>
 *</ul>
 * @param  uvIntSave [IN]类型#UINTPTR，关全局中断SRE_IntLock和开全局中断SRE_IntUnLock的返回值。
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_IntUnLock | SRE_IntLock
 */

OS_SEC_ALW_INLINE INLINE VOID SRE_IntRestore(UINTPTR uvIntSave)
{
	/*DTS2013040106948--start*/
	__asm__ __volatile__(
		"MSR     cpsr_c, %0"
		::"r"(uvIntSave));
	/*DTS2013040106948--end*/
}

/**
 * @ingroup  SRE_sys
 * @brief 获取当前的64位cycle计数,cycle计数的高32位保存到参数puwCntHi，低32为保存到参数puwCntLo。
 *
 * @par 描述:
 * 获取当前的64位cycle计数,cycle计数的高32位保存到参数puwCntHi，低32为保存到参数puwCntLo。
 *
 * @attention 无
 *
 * @param  puwCntHi [OUT]类型#UINT32 *，保存cycle计数的高32位。
 * @param  puwCntLo [OUT]类型#UINT32 *，保存cycle计数的低32位。
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CycleCountGet64()
 */
extern VOID SRE_CycleCountGet(UINT32 *puwCntHi, UINT32 *puwCntLo);

/**
 * @ingroup  SRE_sys
 * @brief 获取当前的64位time stamp计数(即系统运行的cycles)。
 *
 * @par 描述:
 * 获取当前的64位time stamp计数(即系统运行的cycles)。
 *
 * @attention 获取的是64bit cycles 数据
 *
 * @param  无
 *
 * @retval 0~0xFFFFFFFFFFFFFFFF 系统当前的cycle数
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CycleCountGet32()
 */
//extern UINT64    SRE_CycleCountGet64(void);

/**
 * @ingroup  SRE_sys
 * @brief 获取当前的32位time stamp计数(即系统运行的cycles)。
 *
 * @par 描述:
 * 获取当前的32位time stamp计数(即系统运行的cycles)。
 *
 * @attention 获取的是32bit cycles 数据
 * 若两次获取时间过长，会发生数据翻转现象，此情况下建议使用获取64bit数据接口。
 *
 * @param  无
 *
 * @retval 0~0xFFFFFFFF  系统当前的cycle数
 * @par 依赖:
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_CycleCountGet64()
 */
#if (OS_HARDWARE_PLATFORM != OS_CORTEX_A9)
OS_SEC_ALW_INLINE INLINE UINT32 SRE_CycleCountGet32()
{
	UINT32 uwCCOUNT;
	__asm__ __volatile__("rsr %0, ccount" : "=a"(uwCCOUNT));

	return uwCCOUNT;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _SRE_BASE_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

