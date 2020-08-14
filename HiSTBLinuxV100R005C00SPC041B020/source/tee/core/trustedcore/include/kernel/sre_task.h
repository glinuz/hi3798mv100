/**
 * @file sre_task.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：任务模块的对外头文件。 \n
 */
/**@defgroup SRE_task 任务基本功能
  *@ingroup SRE_tsk
*/

/**@defgroup SRE_tsk 任务
 *@ingroup SRE_kernel
*/

#ifndef _SRE_TASK_H
#define _SRE_TASK_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/**
 * @ingroup SRE_task
 * 任务名的最大长度。
 *
 * 任务名的最大长度，包括结尾符'\0'。
 */
#define OS_TSK_NAME_LEN                                     16

/**
 * @ingroup SRE_task
 * 任务的消息队列最大的个数。
 *
 */
#define OS_TSK_MSGQ_MAX                                     8


/**
 * @ingroup SRE_task
 * 空任务ID。
 *
 * 调用uniTskYield时，使用OS_TSK_NULL_ID，由OS选择就绪队列中的第一个任务。
 */
#define OS_TSK_NULL_ID                                      0xFFFFFFFF


/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 * 任务共支持32个优先级，分别为0-31。0表示最高优先级，31表示最低优先级。
 * IDLE任务使用此优先级，用户应不创建此优先级的任务。
 */
#define OS_TSK_PRIORITY_HIGHEST                             0

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_01                                  1

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_02                                  2

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_03                                  3

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_04                                  4

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_05                                  5

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_06                                  6

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_07                                  7

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_08                                  8

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_09                                  9

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_10                                  10

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_11                                  11

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_12                                  12

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_13                                  13

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_14                                  14

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_15                                  15

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_16                                  16

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_17                                  17

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_18                                  18

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_19                                  19

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_20                                  20

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_21                                  21

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_22                                  22

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_23                                  23

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_24                                  24

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_25                                  25

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_26                                  26

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_27                                  27

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_28                                  28

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_29                                  29

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 */
#define OS_TSK_PRIORITY_30                                  30

/**
 * @ingroup SRE_task
 * 可用的任务优先级宏定义。
 *
 * 任务的最低优先级。
 */
#define OS_TSK_PRIORITY_LOWEST                              31

/**
 * @ingroup SRE_task
 * 任务的优先级个数。
 *
 * 任务支持32个优先级。
 */
#define OS_TSK_NUM_OF_PRIORITIES                            32


/**
 * @ingroup SRE_task
 * 最小任务栈大小。
 *
 * 若指定的任务栈大小低于此值（任务上下文大小向上16字节对齐 + 160字节），
 * 任务将创建失败。
 */
#define OS_TSK_MIN_STACK_SIZE                               (ALIGN(0x130, 16) + 0xA0)

#define OS_TSK_STACK_GAP_SIZE                               0x1000
#define OS_TSK_STACK_GAP_MAGICNUMBER                        0xDEADBEEF
#define OS_TSK_USER_HEAP_RESERVED_SIZE                      0x1000
#define OS_TSK_PT_RESERVED_SIZE                             0x1000
/*
______
CODE
__r1___ //OS_TSK_STACK_GAP_SIZE reseved region
HEAP
OS_TSK_USER_HEAP_RESERVED_SIZE
__r2___ //OS_TSK_STACK_GAP_SIZE reseved region
OS_TSK_PT_RESERVED_SIZE
STACK
__r3___ //OS_TSK_STACK_GAP_SIZE reseved region
*/
#define OS_TSK_USER_STACK_HEAP_EXTRA (OS_TSK_PT_RESERVED_SIZE + OS_TSK_USER_HEAP_RESERVED_SIZE + 3 * OS_TSK_STACK_GAP_SIZE)
#define OS_TSK_USER_STACK_HEAP_EXTRA_ALIGN 0x1000
/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务控制块未被使用。
 */
#define OS_TSK_UNUSED                                       0x0001

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被挂起。
 */
#define OS_TSK_SUSPEND                                      0x0004

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被阻塞（等待信号量）。
 */
#define OS_TSK_PEND                                         0x0008  /**< 任务被阻塞（等待信号量）。     */

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务等待超时（P信号量超时）。
 */
#define OS_TSK_TIMEOUT                                      0x0010  /**< 任务等待超时（P信号量超时）。  */

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被延时。
 */
#define OS_TSK_DELAY                                        0x0020  /**< 任务被延时。                   */

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务已就绪，已加入就绪队列。
 */
#define OS_TSK_READY                                        0x0040  /**< 任务已就绪，已加入就绪队列。   */

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务正运行，仍在就绪队列。
 */
#define OS_TSK_RUNNING                                      0x0080

/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被阻塞（等待快速信号量）。
 */
#define OS_TSK_FSEM_PEND                                    0x0100
/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被阻塞（等待消息）。
 */
#define OS_TSK_MSG_PEND                                     0x0200
/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务被阻塞（等待核间信号量）。
 */
#define OS_TSK_MCSEM_PEND                                   0x0400

/**
 * @ingroup SRE_task
 * 任务在内核态
 *
 * 任务阻塞于内核态。
 */
#define OS_TSK_KERNEL_PEND                                   0x0800


#if  (OS_HAVE_COPROCESSOR1 == YES)
/**
 * @ingroup SRE_task
 * 任务或任务控制块状态标志。
 *
 * 任务使用coprocessor的矢量寄存器现场已保存。
 */
#define OS_TSK_CP_CONTEXT_SAVED                             0x0002  /**< 任务使用coprocessor的矢量寄存器现场已保存。 */
#endif

/*
 * 任务模块的错误码定义。
 */
/**
 * @ingroup SRE_task
 * 任务错误码：申请内存失败。
 *
 * 值: 0x03000800
 *
 * 解决方案: 分配更大的私有FSC内存分区
 *
 */
#define OS_ERRNO_TSK_NO_MEMORY                              SRE_ERRNO_OS_FATAL(OS_MID_TSK, 0x00)

/**
 * @ingroup SRE_task
 * 任务错误码：指针参数为空。
 *
 * 值: 0x02000801
 *
 * 解决方案: 检查参数指针是否为NUL。
 */
#define OS_ERRNO_TSK_PTR_NULL                               SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x01)

/**
 * @ingroup SRE_task
 * 任务错误码：任务栈大小未按16字节大小对齐。
 *
 * 值: 0x02000802
 *
 * 解决方案: 检查入参任务栈大小是否按16字节对齐。
 */
#define OS_ERRNO_TSK_STKSZ_NOT_ALIGN                        SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x02)

/**
 * @ingroup SRE_task
 * 任务错误码：任务优先级非法。
 *
 * 值: 0x02000803
 *
 * 解决方案: 检查入参任务优先级是否大于31。
 */
#define OS_ERRNO_TSK_PRIOR_ERROR                            SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x03)

/**
 * @ingroup SRE_task
 * 任务错误码：任务入口函数为空。
 *
 * 值: 0x02000804
 *
 * 解决方案: 检查入参任务入口函数是否为NULL。
 */
#define OS_ERRNO_TSK_ENTRY_NULL                             SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x04)

/**
 * @ingroup SRE_task
 * 任务错误码：任务名的指针为空或任务名为空字符串。
 *
 * 值: 0x02000805
 *
 * 解决方案: 检查任务名指针和任务名。
 */
#define OS_ERRNO_TSK_NAME_EMPTY                             SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x05)

/**
 * @ingroup SRE_task
 * 任务错误码：指定的任务栈空间太小。
 *
 * 值: 0x02000806
 *
 * 解决方案: 检查任务栈是否小于0x1D0。
 */
#define OS_ERRNO_TSK_STKSZ_TOO_SMALL                        SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x06)

/**
 * @ingroup SRE_task
 * 任务错误码：任务ID非法。
 *
 * 值: 0x02000807
 *
 * 解决方案: 检查入参任务PID是否合法
 */
#define OS_ERRNO_TSK_ID_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x07)

/**
 * @ingroup SRE_task
 * 任务错误码：任务已被挂起。
 *
 * 值: 0x02000808
 *
 * 解决方案: 检查所挂起任务是否为已挂起任务。
 */
#define OS_ERRNO_TSK_ALREADY_SUSPENDED                      SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x08)

/**
 * @ingroup SRE_task
 * 任务错误码：任务未被挂起。
 *
 * 值: 0x02000809
 *
 * 解决方案: 检查所恢复任务是否未挂起。
 */
#define OS_ERRNO_TSK_NOT_SUSPENDED                          SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x09)

/**
 * @ingroup SRE_task
 * 任务错误码：任务未创建。
 *
 * 值: 0x0200080a
 *
 * 解决方案: 检查任务是否创建
 */
#define OS_ERRNO_TSK_NOT_CREATED                            SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x0a)

/**
 * @ingroup SRE_task
 * 任务错误码：在锁任务的状态下删除当前任务。
 *
 * 值: 0x0300080b
 *
 *解决方案: 用户确保删除任务时，将任务解锁。
 *
 */
#define OS_ERRNO_TSK_DELETE_LOCKED                          SRE_ERRNO_OS_FATAL(OS_MID_TSK, 0x0b)

/**
 * @ingroup SRE_task
 * 任务错误码：任务待处理的消息数非零。
 *
 * 值: 0x0200080c
 *
 * 解决方案: 检查所删除任务是否尚有未处理的消息。
 */
#define OS_ERRNO_TSK_MSG_NONZERO                            SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x0c)

/**
 * @ingroup SRE_task
 * 任务错误码：在硬中断或软中断的处理中进行延时操作。
 *
 * 值: 0x0300080d
 *
 *解决方案: 此操作禁止在中断中进行调度。
 *
 */
#define OS_ERRNO_TSK_DELAY_IN_INT                           SRE_ERRNO_OS_FATAL(OS_MID_TSK, 0x0d)

/**
 * @ingroup SRE_task
 * 任务错误码：在锁任务的状态下进行延时操作。
 *
 * 值: 0x0200080e
 *
 * 解决方案: 检查是否锁任务。
 */
#define OS_ERRNO_TSK_DELAY_IN_LOCK                          SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x0e)

/**
 * @ingroup SRE_task
 * 任务错误码：任务ID不在Yield操作指定的优先级队列中。
 *
 * 值: 0x0200080f
 *
 * 解决方案: 检查操作的任务的优先级。
 */
#define OS_ERRNO_TSK_YIELD_INVALID_TASK                     SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x0f)

/**
 * @ingroup SRE_task
 * 任务错误码：Yield操作指定的优先级队列中，就绪任务数小于2。
 *
 * 值: 0x02000810
 *
 * 解决方案: 检查指定优先级就绪任务，确保就绪任务数大于1。
 */
#define OS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK                  SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x10)

/**
 * @ingroup SRE_task
 * 任务错误码：没有可用的任务控制块资源或配置项中任务裁剪关闭。
 *
 * 值: 0x02000811
 *
 * 解决方案: 打开配置项中任务裁剪开关，并配置足够大的任务资源数。
 */
#define OS_ERRNO_TSK_TCB_UNAVAILABLE                        SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x11)

/**
 * @ingroup SRE_task
 * 任务错误码：任务钩子不匹配，即要删除的钩子未注册或已取消。
 *
 * 值: 0x02000812
 *
 * 解决方案: 检查钩子是否已注册。
 */
#define OS_ERRNO_TSK_HOOK_NOT_MATCH                         SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x12)

/**
 * @ingroup SRE_task
 * 任务错误码：任务钩子已达到最大允许的注册数。
 *
 * 值: 0x02000813
 *
 * 解决方案: 检查任务钩子个数。
 */
#define OS_ERRNO_TSK_HOOK_IS_FULL                           SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x13)

/**
 * @ingroup SRE_task
 * 任务错误码：操作IDLE任务。
 *
 * 值: 0x02000814
 *
 * 解决方案: 检查是否操作IDLE任务。
 */
#define OS_ERRNO_TSK_OPERATE_IDLE                           SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x14)

/**
 * @ingroup SRE_task
 * 任务错误码：在锁任务的状态下挂起当前任务。
 *
 * 值: 0x03000815
 *
 *解决方案: 确保任务挂起的时候，任务已经解锁。
 *
 */
#define OS_ERRNO_TSK_SUSPEND_LOCKED                         SRE_ERRNO_OS_FATAL(OS_MID_TSK, 0x15)

/**
 * @ingroup SRE_task
 * 任务错误码：释放任务栈失败。
 *
 * 值: 0x02000817
 *
 * 解决方案: 无
 */
#define OS_ERRNO_TSK_FREE_STACK_FAILED                      SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x17)

/**
 * @ingroup SRE_task
 * 任务错误码：任务栈区间配置太小。
 *
 * 值: 0x02000818
 *
 * 解决方案: 在Config.h中配置任务栈大小超过OS_TSK_MIN_STACK_SIZE 。
 */
#define OS_ERRNO_TSK_STKAREA_TOO_SMALL                      SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x18)

/**
 * @ingroup SRE_task
 * 任务错误码：系统初始化任务激活失败
 *
 * 值: 0x03000819
 *
 * 解决方案: 查看任务栈是否配置错误。
 *
 */
#define OS_ERRNO_TSK_ACTIVE_FAILED                          SRE_ERRNO_OS_FATAL(OS_MID_TSK, 0x19)

/**
 * @ingroup SRE_task
 * 任务错误码：配置的任务数量太多，最多126个.不包括Idle任务。
 *
 * 值: 0x0200081a
 *
 * 解决方案: 在任务配置项中将最大任务数改为小于等于126个。
 */
#define OS_ERRNO_TSK_CONFIG_TOO_MANY                        SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x1a)

/**
 * @ingroup SRE_task
 * 任务错误码：任务的coprocessor上下文保存区域未按16字节对齐。
 *
 * 值: 0x0200081b
 *
 * 解决方案: 检查保存区起始地址是否16字节对齐。
 */
#define OS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN                 SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x1b)

/**
 * @ingroup SRE_task
 * 任务错误码：任务的MSG队列个数超过8。
 *
 * 值: 0x0200081d
 *
 * 解决方案: 确认任务创建的消息队列数不超过8。
 */
#define OS_ERRNO_TSK_MSG_Q_TOO_MANY                         SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x1d)

/**
 * @ingroup SRE_task
 * 任务错误码：任务的coprocessor上下文保存区域的地址为空指针。
 *
 * 值: 0x0200081e
 *
 * 解决方案: 检查保存区起始地址是否为NULL
 */
#define OS_ERRNO_TSK_CP_SAVE_AREA_NULL                      SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x1e)

/**
 * @ingroup SRE_task
 * 任务错误码：任务自删除时释放未接收消息的内存失败
 *
 * 值: 0x0200081f
 *
 * 解决方案: 无
 */
#define OS_ERRNO_TSK_SELF_DELETE_ERR                        SRE_ERRNO_OS_ERROR(OS_MID_TSK, 0x1f)


#define OS_INIT_TASK_NAME "TEEGlobalTask"
/**
 * @ingroup SRE_task
 * 任务ID的类型定义。
 */
typedef UINT32                                              TSK_HANDLE_T;

/**
 * @ingroup SRE_task
 * 任务优先级的类型定义。
 */
typedef UINT16                                              TSK_PRIOR_T;

/**
 * @ingroup SRE_task
 * 任务状态的类型定义。
 */
typedef UINT16                                              TSK_STATUS_T;

/**
 * @ingroup SRE_task
 * 任务栈信息的结构体定义。
 *
 * 保存任务栈的信息。
 */
typedef struct tagStackInfo {
	UINT32 uwTop;                                           /**< 栈顶                       */
	UINT32 uwBottom;                                        /**< 栈底                       */
	UINT32 uwSP;                                            /**< 栈当前SP指针值             */
	UINT32 uwCurrUsed;                                      /**< 栈当前使用的大小           */
	UINT32 uwPeakUsed;                                      /**< 栈使用的历史峰值           */
	BOOL   bOvf;                                            /**< 栈是否溢出                 */
} STACK_INFO_S;

/**
* @ingroup  SRE_task
* @brief 任务入口函数类型定义。
*
* @par 描述:
* 用户通过任务入口函数类型定义任务入口函数，在任务创建触发之后调用该函数进行任务执行。
* @attention 无。
*
* @param  uwParam1 [IN] 类型#UINT32，传递给任务处理函数的第一个参数。
* @param  uwParam2 [IN] 类型#UINT32，传递给任务处理函数的第二个参数。
* @param  uwParam3 [IN] 类型#UINT32，传递给任务处理函数的第三个参数。
* @param  uwParam4 [IN] 类型#UINT32，传递给任务处理函数的第四个参数。
*
* @retval 无。
* @par 依赖:
* <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
* @since UniDSP V100R001C01
* @see 无。
*/
typedef VOID (* TSK_ENTRY_FUNC)(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4);
/**
 * @ingroup  SRE_task
 * @brief 任务创建钩子函数类型定义。
 *
 * @par 描述:
 * 用户通过任务创建钩子函数类型定义任务创建钩子，在系统创建任务时，调用该钩子。
 * @attention 无。
 *
 * @param  puwTaskPID [IN] 类型#UINT32，新创建任务的PID。
 *
 * @retval 无。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see 无。
 */
typedef UINT32(* TSK_CREATE_HOOK)(UINT32 uwTaskPID);

/**
* @ingroup  SRE_task
* @brief 任务删除钩子函数类型定义。
*
* @par 描述:
* 用户通过任务删除钩子函数类型定义任务删除钩子，在系统对任务进行删除时，调用该钩子。
* @attention 无。
*
* @param  puwTaskPID [IN] 类型#UINT32，删除任务的PID。
*
* @retval 无。
* @par 依赖:
* <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
* @since UniDSP V100R001C01
* @see 无。
*/
typedef UINT32(* TSK_DELETE_HOOK)(UINT32 uwTaskPID);

/**
 * @ingroup  SRE_task
 * @brief 任务切换钩子函数类型定义。
 *
 * @par 描述:
 * 用户通过任务切换钩子函数类型定义任务切换钩子，在系统对任务进行切换时，调用该钩子。
 * @attention 无。
 *
 * @param  uwLastTaskPID [IN] 类型#UINT32，上一个任务的PID。
 * @param  uwNextTaskPID [IN] 类型#UINT32，下一个任务的PID。
 *
 * @retval 无。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see 无。
 */
typedef UINT32(* TSK_SWITCH_HOOK)(UINT32 uwLastTaskPID, UINT32 uwNextTaskPID);

/**
 * @ingroup SRE_task
 * 任务模块配置信息的结构体定义。
 *
 * 保存任务模块的配置项信息。
 */
typedef struct tagTskModInfo {
	UINT32 uwMaxNum;                /**< 最大支持的任务数           */
	UINT32 uwDefaultSize;           /**< 缺省的任务栈大小           */
	UINT32 uwIdleStackSize;         /**< Idle任务的任务栈大小       */
	UINT32 uwStackAreaAddr;         /**< 任务栈区间的起始地址       */
	UINT32 uwStackAreaSize;         /**< 任务栈区间的大小           */
	UINT32 uwPriThreshold;          /**<任务抢占优先级阀值开关
                                                    --YES,  只有当X任务的优先级高于A任务的抢占阀值的，才可以抢占A任务;
                                                    --NO,  (Default),  当X任务的优先级高于A任务的优先级，就可以抢占A任务*/
} TSK_MOD_INFO_S;

typedef struct {
	UINT32 uwRegionStart;  /*start of TA load mem virt addr*/
	UINT32 uwRegionSize;   /*size of TA running mem*/
	UINT32 uwStackPtSize; /*task user space stack size*/
	UINT32 uwHeapSize;   /*task heap size*/
	UINT32 uwVirtAddr;  /*task_table addr*/
} TSK_MEM_STRUCT_S;

typedef struct {
	UINT32 uwRegionStart;
	UINT32 uwRegionSize;
	UINT32 uwStackPtSize;
	UINT32 uwHeapSize;
	UINT32 uwVirtAddr;
} TSK_MEM_INFO_S;

typedef void (*task_entry_t)(void);

struct sessison_task_info {
	task_entry_t pentry;
	UINT32 mem_region_start;
	UINT32 mem_region_size;
	UINT32 mem_table;
	UINT32 stack_pt_number;
	char *task_name;
	UINT32 mem_virt_addr;
};

typedef struct {
	UINT32 timeLow;
	UINT16 timeMid;
	UINT16 timeHiAndVersion;
	UINT8 clockSeqAndNode[8];
} TEE_UUID_TSK;


/**
 * @ingroup SRE_task
 * 任务创建参数的结构体定义。
 *
 * 传递任务创建时指定的参数信息。
 */
typedef struct tagTskInitParam {
	TSK_ENTRY_FUNC pfnTaskEntry;     /**< 任务入口函数               */
	TSK_PRIOR_T    usTaskPrio;       /**< 任务优先级                 */
	UINT16 usQNum;                   /**< 消息队列数                 */
	UINT32 auwArgs[4];               /**< 任务参数，最多4个          */
	UINT32 uwStackSize;              /**< 任务栈的大小               */
	CHAR  *pcName;                   /**< 任务名                     */
	UINT32 uwResved;                 /**< 保留                       */
	UINT32 uwPrivateData;            /**< 专属于本任务的私有数据     */
	UINT32 uwUsrSpacePartNo;
	UINT64 ullPermissions;           /**< Permission bit field    */
	UINT32 uwSPID;                   /**< Service Provider ID     */
	TSK_MEM_STRUCT_S usMemStruct;
	TEE_UUID_TSK uuid;
} TSK_INIT_PARAM_S;

/**
 * @ingroup SRE_task
 * 任务信息的结构体定义。
 *
 * 用户可以查询的任务信息。
 */
typedef struct tagTskInfo {
	UINT32 uwSP;                     /**< 任务切换时的SP             */
	UINT32 uwPC;                     /**< 任务切换时的PC             */
	TSK_STATUS_T usTaskStatus;       /**< 任务状态                   */
	TSK_PRIOR_T  usTaskPrio;         /**< 任务优先级                 */
	UINT32 uwStackSize;              /**< 任务栈的大小               */
	UINT32 uwTopOfStack;             /**< 任务栈的栈顶               */
	UINT32 uwNumOfMsg;               /**< 任务接收到的消息数         */
	CHAR   acName[OS_TSK_NAME_LEN];  /**< 任务名                     */
} TSK_INFO_S;

/**
 * @ingroup SRE_task
 * 任务上下文的结构体定义。
 *
 * 用户可以查询的任务上下文信息
 */
typedef struct tagTskContext {
	UINT32 auwA[16];                /**< 当前窗口的a0-a15               */
	UINT32 uwPC;                    /**< 程序计数器                     */
	UINT32 uwPS;                    /**< 程序状态寄存器                 */
	UINT32 uwLBEG;                  /**< Loop Begin寄存器               */
	UINT32 uwLEND;                  /**< Loop End寄存器                 */
	UINT32 uwLCOUNT;                /**< Loop Count寄存器               */
	UINT32 uwSAR;                   /**< 移位数寄存器                   */
} TSK_CONTEXT_S;

/**
 * @ingroup SRE_task
 * Structure contains interrupted task context
 *
 * Generic structure to hold taks GPRS and context
 */
typedef struct tagTskRegs {
	UINT32 r0;
	UINT32 r1;
	UINT32 r2;
	UINT32 r3;
	UINT32 r4;
	UINT32 r5;
	UINT32 r6;
	UINT32 r7;
	UINT32 r8;
	UINT32 r9;
	UINT32 r10;
	UINT32 r11;
	UINT32 r12;
	UINT32 usr_sp;
	UINT32 usr_lr;
	UINT32 svc_sp;
	UINT32 svc_lr;
	/* The following 2 will be used in bulk for RFE */
	UINT32 pc;
	UINT32 cpsr;
} TSK_REGS_S;
/**
 * @ingroup  SRE_task
 * @brief 创建任务。
 *
 * @par 描述:
 * 创建一个任务。在Root任务起来前创建的任务只是简单地加入就绪队列。
 * 系统初始化后创建的任务，如果优先级高于当前任务且未锁任务，则立即发生任务调度并被运行，
 * 否则加入就绪队列。
 *
 * @attention
 * <ul>
 * <li>任务创建时，会对之前自删除任务的任务控制块和任务栈进行回收。</li>
 * <li>任务名的最大长度为16字节，含结束符。</li>
 * <li>创建任务时需要配置消息队列数。</li>
 * <li>同一核内任务名不允许重复，且不允许和软中断重名。</li>
 * <li>若指定的任务栈大小为0，则使用配置项#OS_TSK_DEFAULT_STACK_SIZE指定的默认的任务栈大小。</li>
 * <li>任务栈的大小必须按16字节大小对齐。确定任务栈大小的原则是，够用就行：多了浪费，少了任务栈溢出。具体多少取决于需要消耗多少的栈内存，视情况而定：函数调用层次越深，栈内存开销越大，局部数组越大，局部变量越多，栈内存开销也越大。</li>
 * </ul>
 *
 * @param  puwTaskPID [OUT] 类型#UINT32 *，保存任务PID。
 * @param  pstInitParam [IN] 类型#TSK_INIT_PARAM_S *，任务创建参数。
 *
 * @retval #OS_ERRNO_TSK_NO_MEMORY              0x03000800，申请内存失败。
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_STKSZ_NOT_ALIGN        0x02000802，任务栈大小未按16字节大小对齐。
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ENTRY_NULL             0x02000804，任务入口函数为空。
 * @retval #OS_ERRNO_TSK_NAME_EMPTY             0x02000805，任务名的指针为空，但可以是空字符串。
 * @retval #OS_ERRNO_TSK_STKSZ_TOO_SMALL        0x02000806，指定的任务栈空间太小。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_SUSPENDED          0x02000809，任务未被挂起。
 * @retval #OS_ERRNO_TSK_FREE_STACK_FAILED      0x02000817，释放任务栈失败。
 * @retval #OS_ERRNO_TSK_TCB_UNAVAILABLE        0x02000811，没有可用的任务控制块资源。
 * @retval #OS_ERRNO_TSK_MSG_Q_TOO_MANY         0x0200081d，任务的MSG队列个数超过8。
 * @retval #OS_ERRNO_HUNT_THREAD_NAME_REPEAT    0x02001f01，创建任务时，线程名重名.
 * @retval #SRE_OK                          0x00000000，任务创建成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskDelete | SRE_TaskCreateHookAdd
 */
extern UINT32 SRE_TaskCreate(UINT32 *puwTaskPID, TSK_INIT_PARAM_S *pstInitParam);


/**
 * @ingroup  SRE_task
 * @brief 恢复任务。
 *
 * @par 描述:
 * 恢复挂起的任务。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若任务仍处于延时、阻塞态，则只是取消挂起态，并不加入就绪队列。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_NOT_SUSPENDED          0x02000809，任务未被挂起。
 * @retval #SRE_OK                          0x00000000，恢复任务成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskSuspend
 */
extern UINT32 SRE_TaskResume(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 挂起任务。
 *
 * @par 描述:
 * 挂起指定的任务，任务将从就绪队列中被删除。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若为当前任务且已锁任务，则不能被挂起。</li>
 * <li>IDLE任务不能被挂起。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_ALREADY_SUSPENDED      0x02000808，任务已被挂起。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #OS_ERRNO_TSK_SUSPEND_LOCKED         0x03000815，在锁任务的状态下挂起当前任务。
 * @retval #SRE_OK                          0x00000000，挂起任务成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskResume | SRE_TaskLock
 */
extern UINT32 SRE_TaskSuspend(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 删除任务。
 *
 * @par 描述:
 * 删除指定的任务，释放任务栈和任务控制块资源。
 *
 * @attention
 * <ul>
 * <li>若为自删除，则任务控制块和任务栈在下一次创建任务时才回收。</li>
 * <li>对于任务自删除，RTOSck不处理该任务相关的信号量和接收到的消息。</li>
 * <li>任务自删除时，删除钩子不允许进行pend信号量、挂起等操作。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_DELETE_LOCKED          0x0300080b，在锁任务的状态下删除当前任务。
 * @retval #OS_ERRNO_TSK_MSG_NONZERO            0x0200080c，任务待处理的消息数非零。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #SRE_OK                          0x00000000，删除任务成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCreate | SRE_TaskDeleteHookAdd
 */
extern UINT32 SRE_TaskDelete(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 任务延迟。
 *
 * @par 描述:
 * 延迟当前运行任务的执行。任务延时等待指定的Tick数后，重新参与调度。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>硬中断或软中断处理中，或已锁任务，则延时操作失败。</li>
 * <li>若传入参数0，且未锁任务调度，则顺取同优先级队列中的下一个任务执行。
 * 如没有同级的就绪任务，则不发生任务调度，继续执行原任务。</li>
 * </ul>
 *
 * @param  uwTick [IN] 类型#UINT32，延迟的Tick数。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_YIELD_INVALID_TASK     0x0200080f，任务ID不在Yield操作指定的优先级队列中。
 * @retval #OS_ERRNO_TSK_DELAY_IN_INT           0x0300080d，在硬中断或软中断的处理中进行延时操作。
 * @retval #OS_ERRNO_TSK_DELAY_IN_LOCK          0x0200080e，在锁任务的状态下进行延时操作。
 * @retval #OS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK  0x02000810，Yield操作指定的优先级队列中，就绪任务数小于2。
 * @retval #SRE_OK                          0x00000000，任务延时成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskYield
 */
extern UINT32 SRE_TaskDelay(UINT32 uwTick);


/**
 * @ingroup  SRE_task
 * @brief 锁任务调度。
 *
 * @par 描述:
 * 锁任务调度。若任务调度被锁，则不发生任务切换。
 *
 * @attention
 * <ul>
 * <li>只是锁任务调度，并不关中断，因此任务仍可被中断打断。</li>
 * <li>执行此函数则锁计数值加1，解锁则锁计数值减1。因此，必须与#SRE_TaskUnlock配对使用。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskUnlock
 */
extern VOID SRE_TaskLock(VOID);


/**
 * @ingroup  SRE_task
 * @brief 解锁任务调度。
 *
 * @par 描述:
 * 任务锁计数值减1。若嵌套加锁，则只有锁计数值减为0才真正的解锁了任务调度。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>执行此函数则锁计数值加1，解锁则锁计数值减1。因此，必须与#SRE_TaskLock配对使用。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskLock
 */
extern VOID SRE_TaskUnlock(VOID);


/**
 * @ingroup  SRE_task
 * @brief 获取当前任务PID。
 *
 * @par 描述:
 * 获取处于运行态的任务PID。
 *
 * @attention
 * <ul>
 * <li>硬中断或软中断处理中，也可获取当前任务PID，即被中断打断的任务。</li>
 * <li>在任务切换钩子处理中调用时，获取的是切入任务的ID。</li>
 * </ul>
 *
 * @param  puwTaskPID [IN] 类型#UINT32，保存任务PID。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务ID非法。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskStatusGet | SRE_TaskInfoGet
 */
extern UINT32 SRE_TaskSelf(UINT32 *puwTaskPID);

extern UINT32 SRE_TaskSelfUUID(TEE_UUID_TSK *puwTaskUUID);


/**
 * @ingroup  SRE_task
 * @brief 检查任务PID。
 *
 * @par 描述:
 * 检查任务PID是否合法。
 *
 * @attention
 * <ul>
 * <li>任务ID并不是从0开始编号的。</li>
 * <li>该接口只支持本核任务PID的检测。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #  合法返回TRUE，非法返回FALSE。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskSelf
 */
extern BOOL SRE_TaskIDCheck(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 获取任务状态。
 *
 * @par 描述:
 * 获取指定任务的状态。
 *
 * @attention 无
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #OS_INVALID    失败返回。
 * @retval # 成功返回任务状态。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskInfoGet | SRE_TaskContextGet | SRE_TaskPrivateDataGet
 */
extern TSK_STATUS_T SRE_TaskStatusGet(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 获取任务信息。
 *
 * @par 描述:
 * 获取指定任务的信息。
 *
 * @attention
 * <ul>
 * <li>若获取当前任务的信息，则只在硬中断、软中断、异常的处理中才有意义，否则获取的SP值和PC值是不准的。(在TXensa平台下不允许在自身任务中获取上下文信息)</li>
 * <li>由于任务切换时，上下文信息也保存在任务栈中，因此任务信息中的SP是保存上下文之后的实际的SP值。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 * @param  pstTaskInfo [IN] 类型#TSK_INFO_S *，保存任务信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskStatusGet | SRE_TaskContextGet | SRE_TaskPrivateDataGet
 */
extern UINT32 SRE_TaskInfoGet(UINT32 uwTaskPID, TSK_INFO_S *pstTaskInfo);
extern UINT32 SRE_TaskInfoGet_EX(UINT32 uwTaskPID, TSK_INFO_S *pstTaskInfo);


/**
 * @ingroup  SRE_task
 * @brief 获取任务上下文。
 *
 * @par 描述:
 * 获取指定任务的上下文。
 *
 * @attention
 * <ul>
 * <li>若获取当前任务的上下文信息，则只在硬中断、软中断、异常的处理中才有意义，否则获取的PC值是不准的。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 * @param  pstContext [IN] 类型#TSK_CONTEXT_S *，保存任务上下文信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskStatusGet | SRE_TaskInfoGet | SRE_TaskPrivateDataGet
 */
extern UINT32 SRE_TaskContextGet(UINT32 uwTaskPID, TSK_CONTEXT_S *pstContext);


/**
 * @ingroup  SRE_task
 * @brief 获取私有数据。
 *
 * @par 描述:
 * 获取当前任务的私有数据。
 *
 * @attention
 * <ul>
 * <li>只能获取当前任务的私有数据。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval # 返回该任务对应的私有数据。如未设置，则返回的值不确定。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskPrivateDataSet
 */
extern UINT32 SRE_TaskPrivateDataGet();


/**
 * @ingroup  SRE_task
 * @brief 设置私有数据值。
 *
 * @par 描述:
 * 设置当前任务的私有数据值。
 *
 * @attention
 * <ul>
 * <li>只能在任务处理中调用。若在中断中设置，则操作的是刚被打断的任务。</li>
 * </ul>
 *
 * @param  uwPrivateData [IN] 类型#UINT32，数据值。
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskPrivateDataGet
 */
extern VOID SRE_TaskPrivateDataSet(UINT32 uwPrivateData);


/**
 * @ingroup  SRE_task
 * @brief 获取优先级。
 *
 * @par 描述:
 * 获取指定任务的优先级。
 *
 * @attention 无
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 *
 * @retval #OS_INVALID 失败返回。
 * @retval # 成功返回任务优先级。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskPrioritySet
 */
extern TSK_PRIOR_T SRE_TaskPriorityGet(UINT32 uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief 设置优先级。
 *
 * @par 描述:
 * 设置指定任务的优先级。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若设置的优先级高于当前运行的任务，则可能引发任务调度。</li>
 * <li>若调整当前运行任务的优先级，同样可能引发任务调度。</li>
 * </ul>
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 * @param  usTaskPrio [IN] 类型#TSK_PRIOR_T，任务优先级。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskPriorityGet
 */
extern UINT32 SRE_TaskPrioritySet(UINT32 uwTaskPID, TSK_PRIOR_T usTaskPrio);


/**
 * @ingroup  SRE_task
 * @brief 调整指定优先级的任务调度顺序。
 *
 * @par 描述:
 * 若uwNextTask为#OS_TSK_NULL_ID，则优先级队列中的第一个就绪任务调整至队尾；
 * 否则，将uwNextTask指定的任务调整至优先级队列的队首。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>前提是指定优先级的就绪队列至少要有两个就绪任务，否则报错返回。</li>
 * </ul>
 *
 * @param  usTaskPrio [IN] 类型#TSK_PRIOR_T，任务usTaskPrio，指定任务优先级队列。
 * @param  uwNextTask [IN] 类型#UINT32，任务ID或OS_TSK_NULL_ID。
 * @param  puwYeildTo [IN] 类型#UINT32 *，保存被调整到队首的任务PID，可为NULL。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_YIELD_INVALID_TASK     0x0200080f，任务PID不在Yield操作指定的优先级队列中。
 * @retval #OS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK  0x02000810，Yield操作指定的优先级队列中，就绪任务数小于2。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskSuspend
 */
extern UINT32 SRE_TaskYield(TSK_PRIOR_T  usTaskPrio,
			    UINT32 uwNextTask,
			    UINT32 *puwYeildTo);

/**
 * @ingroup  SRE_task
 * @brief 注册任务创建钩子。
 *
 * @par 描述:
 * 注册任务创建钩子函数。钩子函数在任务创建成功后被调用。
 *
 * @attention
 * <ul>
 * <li>当前最大只允许注册一个任务创建钩子函数。</li>
 * <li>不应在任务创建钩子里创建任务。</li>
 * </ul>
 *
 * @param  pfnHook [IN] 类型#TSK_CREATE_HOOK，任务创建钩子函数。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_HOOK_IS_FULL           0x02000813，任务钩子已达到最大允许的注册数。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCreateHookDelete | SRE_TaskDeleteHookAdd | SRE_TaskSwitchHookAdd
 */
extern UINT32 SRE_TaskCreateHookAdd(TSK_CREATE_HOOK pfnHook);


/**
 * @ingroup  SRE_task
 * @brief 取消任务创建钩子。
 *
 * @par 描述:
 * 取消指定的任务创建钩子函数。钩子函数在任务创建成功后被调用。
 *
 * @attention
 * <ul>
 * <li>当前最大只允许注册一个任务创建钩子函数。</li>
 * </ul>
 *
 * @param  pfnHook [IN] 类型#TSK_CREATE_HOOK，任务创建钩子函数。
 *
 * @retval #OS_ERRNO_TSK_HOOK_NOT_MATCH         0x02000812，任务钩子不匹配，即要删除的钩子未注册或已取消。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCreateHookAdd
 */
extern UINT32 SRE_TaskCreateHookDelete(TSK_CREATE_HOOK pfnHook);


/**
 * @ingroup  SRE_task
 * @brief 注册任务删除钩子。
 *
 * @par 描述:
 * 注册任务删除钩子函数。钩子函数在资源回收前被调用。
 *
 * @attention
 * <ul>
 * <li>当前最大只允许注册一个任务删除钩子函数。</li>
 * <li>任务删除钩子中不允许进行pend信号量操作。</li>
 * </ul>
 *
 * @param  pfnHook [IN] 类型#TSK_DELETE_HOOK，任务删除钩子函数。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_HOOK_IS_FULL           0x02000813，任务钩子已达到最大允许的注册数。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskDeleteHookDelete | SRE_TaskCreateHookAdd | SRE_TaskSwitchHookAdd
 */
extern UINT32 SRE_TaskDeleteHookAdd(TSK_DELETE_HOOK pfnHook);


/**
 * @ingroup  SRE_task
 * @brief 取消任务删除钩子。
 *
 * @par 描述:
 * 取消指定的任务删除钩子。钩子函数在资源回收前被调用。
 *
 * @attention
 * <ul>
 * <li>当前最大只允许注册一个任务删除钩子函数。</li>
 * <li>不应在任务删除钩子里删除任务。</li>
 * </ul>
 *
 * @param  pfnHook [IN] 类型#TSK_DELETE_HOOK，任务删除钩子函数。
 *
 * @retval #OS_ERRNO_TSK_HOOK_NOT_MATCH         0x02000812，任务钩子不匹配，即要删除的钩子未注册或已取消。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskDeleteHookAdd
 */
extern UINT32 SRE_TaskDeleteHookDelete(TSK_DELETE_HOOK pfnHook);


/**
 * @ingroup  SRE_task
 * @brief 注册任务切换钩子。
 *
 * @par 描述:
 * 注册任务切换钩子函数。钩子函数在切入新任务前被调用。
 *
 * @attention
 * <ul>
 * <li>总共可以注册8个任务切换钩子函数，其中系统最多会注册4个任务切换钩子函数，因此，建议用户不要注册超过4个任务切换钩子函数。</li>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在任务切换钩子里进行可能引起任务调度的处理，如：任务延时、P信号量等。</li>
 * </ul>
 *
 * @param  pfnHook [IN] 类型#TSK_SWITCH_HOOK，任务切换钩子函数。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_HOOK_IS_FULL           0x02000813，任务钩子已达到最大允许的注册数。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskSwitchHookDelete | SRE_TaskDeleteHookAdd | SRE_TaskCreateHookAdd
 */
extern UINT32 SRE_TaskSwitchHookAdd(TSK_SWITCH_HOOK pfnHook);


/**
 * @ingroup  SRE_task
 * @brief 取消任务切换钩子。
 *
 * @par 描述:
 * 取消指定的任务切换钩子。钩子函数在切入新任务前被调用。
 *
 * @attention  无
 *
 * @param  pfnHook [IN] 类型#TSK_SWITCH_HOOK，任务切换钩子函数。
 *
 * @retval #OS_ERRNO_TSK_HOOK_NOT_MATCH         0x02000812，任务钩子不匹配，即要删除的钩子未注册或已取消。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskSwitchHookAdd
 */
extern UINT32 SRE_TaskSwitchHookDelete(TSK_SWITCH_HOOK pfnHook);

/**
 * @ingroup  SRE_task
 * @brief 查询本核指定任务正在PEND的信号量。
 *
 * @par 描述:
 * 根据任务状态和任务控制块，判断任务是否在PEND信号量，以及PEND的信号量ID。
 *
 * @attention
 * <ul>
 * <li>用户应先判断PEND状态，状态为0表明任务没有被信号量阻塞。</li>
 * <li>快速信号量没有信号量ID，若任务阻塞于快速信号量，则返回的信号量ID为#OS_INVALID。</li>
 * </ul>
 *
 * @param  uwTaskID     [IN]  类型#UINT32，任务PID。
 * @param  pusSemID     [OUT] 类型#UINT16 *，任务PEND的信号量ID或者#OS_INVALID。
 * @param  pusPendState [OUT] 类型#UINT16 *，任务的PEND状态：0，#OS_TSK_FSEM_PEND，#OS_TSK_PEND，#OS_TSK_MCSEM_PEND。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL       0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID     0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED    0x0200080a，任务未创建。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SemPendListGet | SRE_McSemPendListGet
 */
extern UINT32 SRE_TaskPendSemGet(TSK_HANDLE_T uwTaskID, UINT16 *pusSemID, UINT16 *pusPendState);

/**
 * @ingroup  SRE_task
 * @brief 查询当前运行任务指定队列的消息待接收个数。
 *
 * @par 描述:
 *查询当前运行消息队列uwQid上待接收消息个数。
 *
 * @attention
 * <ul>
 * <li>6181/6108/6182平台上表示任务上的指定的消息队列的消息数，其他平台不支持多消息队列，该接口表示消息队列上的消息数，入参uwQid无效。</li>
 * </ul>
 *
 * @param  uwQid [IN] 类型#UINT32，6181/6108/6182平台上表示任务上的消息队列，其他平台无效入参。
 *
 * @retval  当前运行任务指定队列的消息待接收个数。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see 无
 */
extern UINT32 SRE_TaskMsgNumGet(UINT32 uwQid);

/**
 * @ingroup  SRE_task
 * @brief 获取指定任务的堆栈信息。
 *
 * @par 描述:
 * 获取指定任务的堆栈信息。
 *
 * @attention 无
 *
 * @param  uwTaskPID [IN] 类型#UINT32，任务PID。
 * @param  pstStackInfo [OUT] 类型#STACK_INFO_S，任务栈信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL      0x02000801，入参指针为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID    0x02000807，入参任务ID不合法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED   0x0200080a，任务未创建。
 * @retval #SRE_OK                     0X00000000，成功。
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C00
 */
extern UINT32 SRE_TaskStackInfoGet(TSK_HANDLE_T uwTaskPID, STACK_INFO_S *pstStackInfo);

/**
 * @ingroup  SRE_task
 * @brief
 *
 * @par 描述:
 *
 *
 * @attention 无
 *
 * @param
 *
 * @retval #SRE_OK                     0X00000000，成功。
 * @par 依赖:
 * @since RTOSck V100R001C00
 */
extern UINT32 SRE_TaskCurrentMemInfo(TSK_MEM_INFO_S *usTaskMemInfo);

/**
 * @ingroup  SRE_task
 * @brief Get task vmem information
 *
 * @par 描述:
 *
 *
 * @attention 无
 *
 * @param  uwTaskPID [IN] Task PID
 *
 * @retval
 * @par 依赖:
 * @since RTOSck V100R001C00
 */
extern VOID *SRE_TaskVmemGet(TSK_HANDLE_T uwTaskPID);


/**
 * @ingroup  SRE_task
 * @brief Get task mem table pointer
 *
 * @par 描述:
 *
 *
 * @attention 无
 *
 * @param  uwTaskPID [IN] Task PID
 *
 * @retval
 * @par 依赖:
 * @since RTOSck V100R001C00
 */
extern VOID *SRE_TaskMemTableGet(TSK_HANDLE_T uwTaskPID);



#if  (OS_HAVE_COPROCESSOR1 == YES)

/**
 * @ingroup  SRE_task
 * @brief 给任务分配矢量操作上下文数据保存区
 *
 * @par 描述:
 * 给指定任务分配矢量操作上下文数据保存区
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6108/SD6181平台的DSP核，SD6182平台的DSP核，CPU核，HACCC核。</li>
 * <li>保存矢量操作上下文用到核的双LoadStore指令，建议配置的SaveAreaA/SaveAreaB在不同的Memory Bank中。</li>
 * <li>传入的矢量上下文数据保存区起始地址必须按16字节对齐。</li>
 * <li>矢量上下文数据保存区大小需由用户根据芯片实际存在的矢量寄存器总大小确定。</li>
 * </ul>
 * @param  uwTaskID [IN] 类型为#TSK_HANDLE_T，任务ID号
 * @param  pstParam [IN] 类型#CP_SAVE_CFG_PARAM_S *，参数结构指针
 *
 * @retval #OS_ERRNO_TSK_CP_SAVE_AREA_NULL      0x0200081e，任务的coprocessor上下文保存区域的地址为空指针。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，输入任务ID非法。
 * @retval #OS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN 0x0200801b，保存区域未按16字节对齐。
 * @retval #SRE_OK                          0x00000000，成功。
 *
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCpEntry | SRE_TaskCpExit
 */
extern UINT32 SRE_TaskCpSaveCfg(TSK_HANDLE_T uwTaskID, CP_SAVE_CFG_PARAM_S *pstParam);

/**
 * @ingroup  SRE_task
 * @brief 任务中执行矢量操作的入口
 *
 * @par 描述:
 * 任务中执行矢量操作的入口, 矢量操作开始之前需调用此接口。
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6108/SD6181平台的DSP核，SD6182平台的DSP核，CPU核，HACCC核。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 *
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCpExit
 */
extern VOID SRE_TaskCpEntry(VOID);

/**
 * @ingroup  SRE_task
 * @brief 任务中执行矢量操作的出口
 *
 * @par 描述:
 * 任务中执行矢量操作的出口，矢量操作完成之后需调用此接口。
 * @attention
 * <ul>
 * <li>该操作适用于SD6108/SD6181平台的DSP核，SD6182平台的DSP核，CPU核，HACCC核。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 *
 * @par 依赖:
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_TaskCpEntry
 */
extern VOID SRE_TaskCpExit(VOID);

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

