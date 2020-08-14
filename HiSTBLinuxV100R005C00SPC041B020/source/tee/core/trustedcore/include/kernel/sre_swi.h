/**
 * @file sre_swi.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：软中断模块的对外头文件。 \n
 */
/**@defgroup SRE_interrupt 中断
   *@ingroup SRE_kernel
   */
/** @defgroup SRE_swi 软中断
  *@ingroup SRE_interrupt
 */

#ifndef _SRE_SWI_H
#define _SRE_SWI_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#if (OS_INCLUDE_SWI == YES)
/**
 * @ingroup SRE_swi
 * 可用的软中断优先级的宏定义。
 *
 * 软中断共支持16个优先级，分别为0-15。0表示最高优先级，15表示最低优先级。
 * Tick中断使用优先级0，用户应不创建此优先级的软中断。
 * 当任务模块关闭时，IDLE软中断自动创建，使用优先级15，用户应不创建此优先级的软中断。
 */
#define OS_SWI_PRIORITY_00                                  0

/**
 * @ingroup SRE_swi
 * 软中断优先级为1。
  */
#define OS_SWI_PRIORITY_01                                  1

/**
 * @ingroup SRE_swi
 * 软中断优先级为2。
  */
#define OS_SWI_PRIORITY_02                                  2

/**
 * @ingroup SRE_swi
 * 软中断优先级为3。
  */
#define OS_SWI_PRIORITY_03                                  3

/**
 * @ingroup SRE_swi
 * 软中断优先级为4。
  */
#define OS_SWI_PRIORITY_04                                  4

/**
 * @ingroup SRE_swi
 * 软中断优先级为5。
  */
#define OS_SWI_PRIORITY_05                                  5

/**
 * @ingroup SRE_swi
 * 软中断优先级为6。
  */
#define OS_SWI_PRIORITY_06                                  6

/**
 * @ingroup SRE_swi
 * 软中断优先级为7。
  */
#define OS_SWI_PRIORITY_07                                  7

/**
 * @ingroup SRE_swi
 * 软中断优先级为8。
  */
#define OS_SWI_PRIORITY_08                                  8

/**
 * @ingroup SRE_swi
 * 软中断优先级为9。
  */
#define OS_SWI_PRIORITY_09                                  9

/**
 * @ingroup SRE_swi
 * 软中断优先级为10。
  */
#define OS_SWI_PRIORITY_10                                  10

/**
 * @ingroup SRE_swi
 * 软中断优先级为11。
  */
#define OS_SWI_PRIORITY_11                                  11

/**
 * @ingroup SRE_swi
 * 软中断优先级为12。
  */
#define OS_SWI_PRIORITY_12                                  12

/**
 * @ingroup SRE_swi
 * 软中断优先级为13。
  */
#define OS_SWI_PRIORITY_13                                  13

/**
 * @ingroup SRE_swi
 * 软中断优先级为14。
  */
#define OS_SWI_PRIORITY_14                                  14

/**
 * @ingroup SRE_swi
 * 软中断优先级为15，也是最低优先级。
  */
#define OS_SWI_PRIORITY_15                                  15

/**
 * @ingroup SRE_swi
 * 若使能优先级级别为OS_SWI_PRIORITY_ALL，则所有优先级的软中断都可能得到调度。
  */
#define OS_SWI_PRIORITY_ALL                                 16

/**
 * @ingroup SRE_swi
 * 禁止软中断优先级。
 */
#define OS_SWI_PRIORITY_NONE                                254     /**< Disabled SWI priority */


/**
 * @ingroup SRE_swi
 * 软中断优先级，保留给任务调度。
 */
#define OS_SWI_PRIORITY_LOWEST                              15      /**< 软中断优先级，保留给任务调度 */

/**
 * @ingroup SRE_swi
 * 软中断名的最大长度。
 *
 * 软中断名的最大长度，包括结尾符'\0'。
 */
#define OS_SWI_NAME_LEN                                     16

/**
 * @ingroup SRE_swi
 * 软中断或软中断控制块状态标志。
 *
 * OS_SWI_UNUSED     --- 软中断控制块未被使用。
 */
#define OS_SWI_UNUSED                                       0x0000  /**< 软中断控制块未被使用。 */
/**
 * @ingroup SRE_swi
 * 软中断或软中断控制块状态标志。
 *
 * OS_SWI_CREATED    --- 软中断已创建。
 */
#define OS_SWI_CREATED                                      0x0001  /**< 软中断已创建。         */
/**
 * @ingroup SRE_swi
 * 软中断或软中断控制块状态标志。
 *
 * OS_SWI_READY      --- 软中断已加入就绪队列。
 */
#define OS_SWI_READY                                        0x0004  /**< 软中断已加入就绪队列。 */
/**
 * @ingroup SRE_swi
 * 软中断或软中断控制块状态标志。
 *
 * OS_SWI_ACTIVE     --- 软中断正被处理。
 */

#define OS_SWI_ACTIVE                                       0x0008  /**< 软中断正被处理。       */

#if  (OS_HAVE_COPROCESSOR1 == YES)
/**
 * @ingroup SRE_swi
 * 软中断或软中断控制块状态标志。
 *
 * OS_SWI_CP_CONTEXT_SAVED     --- 软中断使用coprocessor的矢量寄存器现场已保存。
 */

#define OS_SWI_CP_CONTEXT_SAVED                             0x0002  /**< 软中断使用coprocessor的矢量寄存器现场已保存 */
#endif

/*
 * 软中断模块的错误码定义。
 */
/**
 * @ingroup SRE_swi
 * 软中断错误码：申请内存失败。
 *
 * 值: 0x02000900
 *
 * 解决方案:请适当增加系统默认私有静态FSC分区大小
 */
#define OS_ERRNO_SWI_NO_MEMORY                              SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x00)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断PID非法。
 *
 * 值: 0x02000901
 *
 * 解决方案:确保输入的软中断PID是合法的
 */
#define OS_ERRNO_SWI_ID_INVALID                             SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x01)

/**
 * @ingroup SRE_swi
 * 软中断错误码：指针参数为空。
 *
 * 值: 0x02000902
 *
 * 解决方案:在创建软中断时，需要确保指针参数非NULL
 */
#define OS_ERRNO_SWI_PTR_NULL                               SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x02)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断未创建或已经删除。
 *
 * 值: 0x02000903
 *
 * 解决方案:在删除软中断时，需要确保该软中断已经创建且未删除
 */
#define OS_ERRNO_SWI_NOT_CREATED                            SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x03)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断未激活。
 *
 * 值: 0x02000904
 *
 * 解决方案:查询当前激活软中断的PID时，请保证有软中断处于激活态。
 */
#define OS_ERRNO_SWI_NOT_ACTIVE                             SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x04)

/**
 * @ingroup SRE_swi
 * 软中断错误码：入参错误，软中断处理函数为空指针。
 *
 * 值: 0x02000905
 *
 * 解决方案:在创建软中断时，请确保软中断处理函数非空。
 */
#define OS_ERRNO_SWI_PROC_FUNC_NULL                         SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x05)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断优先级非法，优先级为0-15。
 *
 * 值: 0x02000906
 *
 * 解决方案:在创建软中断时，请确保软中断的优先级为0-15.
 */
#define OS_ERRNO_SWI_PRIORITY_INVALID                       SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x06)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断配置的最大支持数不合适，最大支持数不能大于125。
 *
 * 值: 0x02000907
 *
 * 解决方案:在软中断配置项中减少最大支持数，使之小于等于125.
 */
#define OS_ERRNO_SWI_MAX_NUM_NOT_SUITED                     SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x07)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断名为空字符串或者空指针
 *
 * 值: 0x02000909
 *
 * 解决方案:创建软中断时，确保软中断名不为空。
 */
#define OS_ERRNO_SWI_NAME_EMPTY                             SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x09)

/**
 * @ingroup SRE_swi
 * 软中断错误码：核间中断触发失败。
 *
 * 值: 0x0200090b
 *
 * 解决方案:检查核间软中断配置项OS_INCLUDE_MCSWI是否为YES
 */
#define OS_ERRNO_SWI_MC_INT_FAIL                            SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x0b)

/**
 * @ingroup SRE_swi
 * 软中断错误码：IDLE软中断不能删除。在裁剪掉任务模块的情况下，系统需要依赖IDLE软中断完成类似IDLE任务的功能，IDLE软中断不可删除。
 *
 * 值: 0x0200090c
 *
 * 解决方案: 当任务模块裁剪开关关闭时，确保删除的软中断不是IDLE软中断。
 */
#define OS_ERRNO_SWI_DELETE_IDLE_SWI                        SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x0c)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断控制块已用完。
 *
 * 值: 0x0200090d
 *
 * 解决方案:将配置项“最大允许创建的软中断数”的值改大，但不能超过125。
 */

#define OS_ERRNO_SWI_SCB_USED_OUT                           SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x0d)

/**
 * @ingroup SRE_swi
 * 软中断错误码：ACTIVE软中断不可删除
 *
 * 值: 0x0200090e
 *
 * 解决方案: 删除软中断前，确保删除的软中断不处于激活态。
 */
#define OS_ERRNO_SWI_ACTIVE_DEL_INVALID                     SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x0e)

/**
 * @ingroup SRE_swi
 * 软中断错误码：IDLE软中断创建失败，致命错误
 *
 * 值: 0x0200090f
 *
 * 解决方案: 任务模块裁剪时，请确保IDLE软中断创建成功。
 */
#define OS_ERRNO_SWI_BGD_CREATE_FAIL                        SRE_ERRNO_OS_FATAL(OS_MID_SWI, 0x0f)

/**
 * @ingroup SRE_swi
 * 软中断错误码：核间软中断触发失败，软中断没有创建或者软中断PID非法，此错误检测在接收方进行
 *
 * 值: 0x02000910
 *
 * 解决方案: 触发核间软中断时，确保该软中断PID合法。
 */
#define OS_ERRNO_SWI_MC_ACTIVE_FAIL                         SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x10)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断的coprocessor上下文保存区域未按16字节对齐。
 *
 * 值: 0x02000911
 *
 * 解决方案: 请确保传入的软中断的coprocessor上下文保存区域地址按16字节对齐。
 */
#define OS_ERRNO_SWI_CP_SAVE_AREA_NOT_ALIGN                 SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x11)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断的coprocessor上下文保存区域的地址为空指针。
 *
 * 值: 0x02000912
 *
 * 解决方案: 请确保传入的软中断的coprocessor上下文保存区域地址不为空。
 */
#define OS_ERRNO_SWI_CP_SAVE_AREA_NULL                       SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x12)

/**
 * @ingroup SRE_swi
 * 软中断错误码：IDLE软中断未激活。
 *
 * 值: 0x02000913
 *
 * 解决方案:任务模块被裁剪后，请保证IDLE软中断。
 */
#define OS_ERRNO_SWI_BGD_NOT_ACTIVE                           SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x13)

/**
 * @ingroup SRE_swi
 * 软中断错误码：激活软中断时，输入的软中断PID非法。
 *
 * 值: 0x02000914
 *
 * 解决方案:激活软中断时，请保证输入的软中断PID合法。
 */
#define OS_ERRNO_SWI_ACT_ID_INVALID                            SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x14)

/**
 * @ingroup SRE_swi
 * 软中断错误码：增量计数模式触发的软中断PID非法。
 *
 * 值: 0x02000915
 *
 * 解决方案:确保输入的软中断PID是合法的。
 */
#define OS_ERRNO_SWI_ID_ACT_INC_INVALID                        SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x15)

/**
 * @ingroup SRE_swi
 * 软中断错误码：减量计数模式触发的软中断PID非法。
 *
 * 值: 0x02000916
 *
 * 解决方案:确保输入的软中断PID是合法的。
 */
#define OS_ERRNO_SWI_ID_ACT_DEC_INVALID                        SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x16)

/**
 * @ingroup SRE_swi
 * 软中断错误码：直接触发模式触发的软中断未创建或已经删除。
 *
 * 值: 0x02000917
 *
 * 解决方案:在触发软中断时，需要确保该软中断已经创建且未删除。
 */
#define OS_ERRNO_SWI_ACT_NOT_CREATED                           SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x17)

/**
 * @ingroup SRE_swi
 * 软中断错误码：增量计数模式触发的软中断未创建或已经删除。
 *
 * 值: 0x02000918
 *
 * 解决方案:在触发软中断时，需要确保该软中断已经创建且未删除。
 */
#define OS_ERRNO_SWI_ACT_INC_NOT_CREATED                       SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x18)

/**
 * @ingroup SRE_swi
 * 软中断错误码：增量计数模式触发的软中断未创建或已经删除。
 *
 * 值: 0x02000919
 *
 * 解决方案:在触发软中断时，需要确保该软中断已经创建且未删除。
 */
#define OS_ERRNO_SWI_ACT_DEC_NOT_CREATED                       SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x19)

/**
 * @ingroup SRE_swi
 * 软中断错误码：置位掩码模式触发软中断时，输入的软中断PID非法。
 *
 * 值: 0x0200091a
 *
 * 解决方案:在使用置位掩码模式触发软中断时，请确保PID合法。
 */
#define OS_ERRNO_SWI_ID_ACT_OR_INVALID                         SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1a)

/**
 * @ingroup SRE_swi
 * 软中断错误码：置位掩码模式触发的软中断未创建或者已经删除。
 *
 * 值: 0x0200091b
 *
 * 解决方案:在使用置位掩码模式触发软中断时，请确保该软中断已经创建。
 */
#define OS_ERRNO_SWI_ACT_OR_NOT_CREATED                        SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1b)

/**
 * @ingroup SRE_swi
 * 软中断错误码：清位掩码模式触发软中断时，输入的软中断PID非法。
 *
 * 值: 0x0200091c
 *
 * 解决方案:在使用清位掩码模式触发软中断时，请确保PID合法。
 */
#define OS_ERRNO_SWI_ID_ACT_AND_INVALID                        SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1c)

/**
 * @ingroup SRE_swi
 * 软中断错误码：清位掩码模式触发的软中断未创建或者已经删除。
 *
 * 值: 0x0200091d
 *
 * 解决方案:在使用清位掩码模式触发软中断时，请确保该软中断已经创建。
 */
#define OS_ERRNO_SWI_ACT_AND_NOT_CREATED                       SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1d)

/**
 * @ingroup SRE_swi
 * 软中断错误码：查询软中断信息时，输入的软中断PID非法。
 *
 * 值: 0x0200091e
 *
 * 解决方案:查询软中断信息时，确保输入的软中断PID是合法的
 */
#define OS_ERRNO_SWI_ID_INFO_INVALID                           SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1e)

/**
 * @ingroup SRE_swi
 * 软中断错误码：查询软中断信息时，指针参数为空。
 *
 * 值: 0x0200091f
 *
 * 解决方案:查询软中断信息时，需要确保指针参数非NULL
 */
#define OS_ERRNO_SWI_INFO_PTR_NULL                             SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x1f)

/**
 * @ingroup SRE_swi
 * 软中断错误码：查询当前激活软中断PID时，指针参数为空。
 *
 * 值: 0x02000920
 *
 * 解决方案:在查询当前激活软中断PID时，需要确保指针参数非NULL
 */
#define OS_ERRNO_SWI_SELF_PTR_NULL                             SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x20)

/**
 * @ingroup SRE_swi
 * 软中断错误码：软中断待处理的消息非零。
 *
 * 值: 0x02000921
 *
 * 解决方案:删除软中断前请确认已处理完所有消息
 */
#define OS_ERRNO_SWI_MSG_NONZERO                               SRE_ERRNO_OS_ERROR(OS_MID_SWI, 0x21)

/**
 * @ingroup SRE_swi
 * 软中断号的类型定义。
 */
typedef UINT32                                              SWI_HANDLE_T;


/**
 * @ingroup SRE_swi
 * 软中断优先级的类型定义。
 */
typedef UINT16                                              SWI_PRIOR_T;


/**
 * @ingroup SRE_swi
 * 软中断状态的类型定义。
 */
typedef UINT16                                              SWI_STATUS_T;

/**
 * @ingroup  SRE_swi
 * @brief 软中断处理函数的类型定义。
 *
 * @par 描述:
 * 通过软中断处理函数类型定义软中断处理函数，用户通过该钩子函数响应软中断。
 *
 * @attention 无。
 *
 * @param  puwSwiID    [OUT] 类型为#UINT32，软中断创建成功后返回的PID。
 *
 * @retval 无。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see 无。
 */
typedef VOID (* SWI_PROC_FUNC)(UINT32);

/**
 * @ingroup SRE_swi
 * 软中断模块配置信息的结构体定义。
 *
 * 保存软中断模块的配置项信息。
 */
typedef struct tagSwiModInfo {
	UINT32 uwMaxNum;                    /* 最大支持的软中断数 */
} SWI_MOD_INFO_S;

/**
 * @ingroup SRE_swi
 * 软中断信息的结构体定义。
 *
 * 用户可以查询的软中断信息。
 */
typedef struct tagSwiInfo {
	SWI_STATUS_T  usStatus;            /* 软中断状态          */
	SWI_PRIOR_T   usPriority;          /* 软中断优先级        */
	UINT32        uwNumOfMsg;          /* 软中待接收的消息数  */
} SWI_INFO_S;

/**
 * @ingroup SRE_swi
 * 软中断创建入参结构体定义。
 *
 * 用户创建软中断入参结构体，包括软中断处理函数、优先级、初始值和名称。
 */
typedef struct tagSWI_INIT_PARA {
	SWI_PROC_FUNC pfnHandler  ;         /* 软中断触发时的处理函数。         */
	SWI_PRIOR_T usPriority;             /* 类型为SWI_PRIOR_T，软中断优先级。*/
	UINT16 usResved;                    /* 类型为UINT16，保留值。           */
	UINT32 uwInitValue;                 /* 类型为UINT32，软中断初始值。     */
	CHAR *pcSwiName;                    /* 类型为CHAR*，软中断名称。        */
} SWI_INIT_PARA_S;

/**
 * @ingroup  SRE_swi
 * @brief 创建软中断。
 *
 * @par 描述:
 * 为本核创建一个软中断。软中断一般在硬中断中激活，以分担处理硬中断中的一些次要功能。
 *
 * @attention
 * <ul>
 * <li>优先级取值范围0-15，最高优先级0，最低优先级15 </li>
 * <li>一个优先级可以创建多个软中断，如果裁减了任务模块，则不能创建最低优先级的软中断，否则，创建后可能将得不到调度</li>
 * <li>软中断名的最大长度为16字节(含结束符)同一核内软中断名不允许重复，且不允许和任务名重名</li>
 * </ul>
 *
 * @param  puwSwiID    [OUT] 类型为#SWI_HANDLE_T*，软中断创建成功后返回的PID。
 * @param  pstInitPara [IN]  类型为#SWI_INIT_PARA_S，软中断创建输入参数结构体信息。它包括以下四个成员:
 * @param  pfnHandler  [IN]  类型为#SWI_PROC_FUNC，软中断触发时的处理函数。
 * @param  usPriority  [IN]  类型为#SWI_PRIOR_T，软中断优先级。
 * @param  uwInitValue [IN]  类型为#UINT32，软中断初始值。
 * @param  pcSwiName   [IN]  类型为#CHAR*，软中断名称。
 *
 * @retval #OS_ERRNO_SWI_PTR_NULL               0x02000902，指针参数为空。
 * @retval #OS_ERRNO_SWI_PROC_FUNC_NULL         0x02000905，软中断处理函数为空。
 * @retval #OS_ERRNO_SWI_PRIORITY_INVALID       0x02000906，软中断优先级非法。
 * @retval #OS_ERRNO_SWI_NAME_EMPTY             0x02000909，软中断名字不能为NULL和空串。
 * @retval #OS_ERRNO_SWI_SCB_USED_OUT           0x0200090d，没有可用的软中断控制块
 * @retval #OS_ERRNO_HUNT_THREAD_NAME_REPEAT    0x02001f01，创建软中断时，线程名重名.
 * @retval #SRE_OK                              0x00000000，创建软中断成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SwiDelete | SRE_SwiActivate
 */
extern UINT32 SRE_SwiCreate(SWI_HANDLE_T *puwSwiID, SWI_INIT_PARA_S *pstInitPara);

/**
 * @ingroup  SRE_swi
 * @brief 删除软中断。
 *
 * @par 描述:
 * 删除一个软中断，回收软中断控制块资源。
 *
 * @attention
 * <ul>
 * <li>只能删除本核的软中断。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #OS_ERRNO_SWI_ID_INVALID             0x02000901，软中断PID非法。
 * @retval #OS_ERRNO_SWI_NOT_CREATED            0x02000903，软中断未创建。
 * @retval #OS_ERRNO_SWI_MSG_NONZERO            0x02000921，软中断待处理的消息非零。
 * @retval #OS_ERRNO_SWI_DELETE_IDLE_SWI        0x0200090c，IDLE软中断不能删除。
 * @retval #OS_ERRNO_SWI_ACTIVE_DEL_INVALID     0x0200090e，激活软中断不可删除。
 * @retval #SRE_OK                              0x00000000，删除软中断成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SwiDelete | SRE_SwiActivate
 */
extern UINT32 SRE_SwiDelete(SWI_HANDLE_T uwSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 使能软中断。
 *
 * @par 描述:
 * 使能本核高于指定优先级的软中断。只有高于指定优先级的软中断处理才会被使能，
 * 输入OS_SWI_PRIORITY_ALL，使能全部优先级软中断。
 *
 * @attention
 * <ul>
 * <li>该设置仅对本核有效。</li>
 * <li>在背景线程未启动前，请勿调用此接口。</li>
 * </ul>
 *
 * @param  usPriority [IN] 类型为#SWI_HANDLE_T，软中断优先级。
 *
 * @retval 无
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SwiDisable
 */
extern VOID SRE_SwiEnable(SWI_PRIOR_T usPriority);


/**
 * @ingroup  SRE_swi
 * @brief 禁止软中断。
 *
 * @par 描述:
 * 禁止本核所有的软中断处理。
 *
 * @attention
 * <ul>
 * <li>禁止后将不会调用软中断处理函数，但激活的软中断仍会加入就绪队列。</li>
 * <li>在背景线程未启动前，请勿调用此接口。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 禁止前的软中断使能级别。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiEnable
 */
extern SWI_PRIOR_T SRE_SwiDisable(VOID);


/**
 * @ingroup  SRE_swi
 * @brief 直接触发软中断。
 *
 * @par 描述:
 * 激活一个软中断，不改变软中断的计数值。支持核间操作。
 *
 * @attention
 * <ul>
 * <li>在背景线程未启动前，请勿调用此接口。</li>
 * <li>软中断得以执行时，实时值存入该软中断的锁存值中，实时值恢复为创建时的初始值。</li>
 * <li>对于多核RTOSck系统，本接口的操作对象可以是本核或者其它核的软中断。</li>
 * <li>获取非本核软中断PID的方法：创建时将软中断PID保存在核间共享变量中，或使用Hunt模块接口通过软中断名称获取其PID。</li>
 * <li>如果操作对象为非本核软中断，则会触发核间中断，发送激活操作。</li>
 * <li>触发核间软中断时，只支持目的核号是否合法的判断，不支持软中断PID是否合法的判断。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID，来源于软中断创建成功的输出。
 *
 * @retval #OS_ERRNO_SWI_ACT_ID_INVALID         0x02000914，软中断PID非法。
 * @retval #OS_ERRNO_SWI_ACT_NOT_CREATED        0x02000917，软中断未创建。
 * @retval #OS_ERRNO_SWI_MC_INT_FAIL            0x0200090b，触发核间软中断失败。
 * @retval #OS_ERRNO_MCHWI_DST_INVALID          0x02001480，激活软中断时，软中断的ID无效。
 * @retval #OS_ERRNO_IPC_SHQ_FULL               0x02001486，6181和6108平台下，发送核间中断，共享队列已满。
 * @retval #SRE_OK                              0x00000000，激活软中断成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiEnable | SRE_SwiDisable
 */
extern UINT32 SRE_SwiActivate(SWI_HANDLE_T uwSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 增量计数模式触发软中断。
 *
 * @par 描述:
 * 实时值加1，并激活相应软中断。
 *
 * @attention
 * <ul>
 * <li>参数为创建成功时返回的PID号，也可以通过SRE_HuntByName获取软中断的PID。</li>
 * <li>软中断实时值在创建中断时通过中断初始值指定。</li>
 * <li>触发核间软中断时，只支持目的核号是否合法的判断，不支持软中断PID是否合法的判断。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #OS_ERRNO_SWI_ID_ACT_INC_INVALID     0x02000915，软中断PID非法。
 * @retval #OS_ERRNO_SWI_ACT_INC_NOT_CREATED    0x02000918，软中断未创建。
 * @retval #OS_ERRNO_SWI_MC_INT_FAIL            0x0200090b，核间软中断触发失败。
 * @retval #OS_ERRNO_MCHWI_DST_INVALID          0x02001480，激活软中断时，软中断的ID无效。
 * @retval #OS_ERRNO_IPC_SHQ_FULL               0x02001486，6181和6108平台下，发送核间中断，共享队列已满。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiDec | SRE_SwiOr | SRE_SwiAndn
 */
extern UINT32 SRE_SwiInc(SWI_HANDLE_T uwSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 减量计数模式触发软中断。
 *
 * @par 描述:
 * 实时值减1，减到0时才激活软中断，否则，只是做减1操作
 *
 * @attention
 * <ul>
 * <li>如果实时值已经为0了，调用该接口，不会再次触发软中断。</li>
 * <li>参数为创建成功时返回的PID号，也可以通过SRE_HuntByName获取软中断的PID。</li>
 * <li>软中断实时值在创建软中断时通过软中断初始值指定。</li>
 * <li>触发核间软中断时，只支持目的核号是否合法的判断，不支持软中断PID是否合法的判断。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #OS_ERRNO_SWI_ID_ACT_DEC_INVALID     0x02000916，软中断PID非法。
 * @retval #OS_ERRNO_SWI_ACT_DEC_NOT_CREATED    0x02000919，软中断未创建。
 * @retval #OS_ERRNO_SWI_MC_INT_FAIL            0x0200090b，核间软中断触发失败。
 * @retval #OS_ERRNO_MCHWI_DST_INVALID          0x02001480，激活软中断时，软中断的ID无效。
 * @retval #OS_ERRNO_IPC_SHQ_FULL               0x02001486，6181和6108平台下，发送核间中断，共享队列已满。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiInc | SRE_SwiOr | SRE_SwiAndn
 */
extern UINT32 SRE_SwiDec(SWI_HANDLE_T uwSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 置位掩码模式触发软中断。
 *
 * @par 描述:
 * 软中断实时值或上掩码值，并激活软中断。
 *
 * @attention
 * <ul>
 * <li>参数为创建成功时返回的PID号，也可以通过SRE_HuntByName获取软中断的PID。</li>
 * <li>软中断实时值在创建软中断时通过软中断初始值指定。</li>
 * <li>该接口不支持核间触发。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 * @param  uwMask  [IN] 类型为#UINT32，掩码值。
 *
 * @retval #OS_ERRNO_SWI_ID_ACT_OR_INVALID      0x0200091a，软中断PID非法。
 * @retval #OS_ERRNO_SWI_ACT_OR_NOT_CREATED     0x0200091b，软中断未创建。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiInc | SRE_SwiDec | SRE_SwiAndn
 */
extern UINT32 SRE_SwiOr(SWI_HANDLE_T uwSwiID, UINT32 uwMask);


/**
 * @ingroup  SRE_swi
 * @brief 清位掩码模式触发软中断。
 *
 * @par 描述:
 * 软中断实时值清除掩码值为1的位，当实时值清为0时会激活软中断。
 *
 * @attention
 * <ul>
 * <li>参数为创建成功时返回的PID号，也可以通过SRE_HuntByName获取软中断的PID。</li>
 * <li>软中断实时值在创建软中断时通过软中断初始值指定。</li>
 * <li>该接口不支持核间触发。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断号。
 * @param  uwMask  [IN] 类型为#UINT32，掩码值。
 *
 * @retval #OS_ERRNO_SWI_ID_ACT_AND_INVALID     0x0200091c，软中断PID非法。
 * @retval #OS_ERRNO_SWI_ACT_AND_NOT_CREATED    0x0200091d，软中断未创建。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiInc | SRE_SwiDec | SRE_SwiOr
 */
extern UINT32 SRE_SwiAndn(SWI_HANDLE_T uwSwiID, UINT32 uwMask);


/**
 * @ingroup  SRE_swi
 * @brief 获取当前软中断的锁存值。
 *
 * @par 描述:
 * 获取本核当前软中断的锁存值，只能在软中断处理时调用。
 *
 * @attention 无
 *
 * @param  无
 *
 * @retval #OS_INVALID         0xffffffff，没有处于激活态的软中断，无效返回
 * @retval #锁存值 软中断处理中返回的锁存值。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiInc | SRE_SwiDec | SRE_SwiOr | SRE_SwiAndn
 */
extern UINT32 SRE_SwiValueGet(VOID);


/**
 * @ingroup  SRE_swi
 * @brief 获取软中断的状态。
 *
 * @par 描述:
 * 获取本核指定软中断的状态，不支持获取其他核的软中断状态。
 *
 * @attention 无
 * <ul>
 * <li>软中断状态由以下四种状态组合而成：未使用(OS_SWI_UNUSED), 已创建(OS_SWI_CREATED), 就绪(OS_SWI_READY), 激活(OS_SWI_ACTIVE)
 * 组合后的软中断状态为：未使用、已创建、已创建且就绪、已创建且激活。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #OS_INVALID         0xffffffff，该软中断PID无效，失败返回。
 * @retval #其它为SWI的状态值。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiValueGet
 */
extern SWI_STATUS_T SRE_SwiStatusGet(SWI_HANDLE_T uwSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 获取软中断的信息
 *
 * @par 描述:
 * 获取本核指定软中断的信息，包括软中断状态、优先级和未接收的消息数。
 *
 * @attention 无
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 * @param  pstSwiInfo [OUT] 类型#SWI_INFO_S *，保存软中断信息。
 *
 * @retval #OS_ERRNO_SWI_INFO_PTR_NULL          0x0200091f，指针参数为空。
 * @retval #OS_ERRNO_SWI_ID_INFO_INVALID        0x0200091e，软中断PID非法。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V200R003C03
 * @see SRE_SwiValueGet | SRE_SwiStatusGet | SRE_SwiSelf
 */
extern UINT32 SRE_SwiInfoGet(SWI_HANDLE_T uwSwiID, SWI_INFO_S *pstSwiInfo);


/**
 * @ingroup  SRE_swi
 * @brief 获取当前软中断号。
 *
 * @par 描述:
 * 返回本核当前软中断号，只能在软中断处理时调用。
 *
 * @attention 无
 *
 * @param  pusSwiID [OUT] 类型为#SWI_HANDLE_T *，保存软中断号。
 *
 * @retval #OS_ERRNO_SWI_SELF_PTR_NULL          0x02000920，指针参数为空。
 * @retval #OS_ERRNO_SWI_NOT_ACTIVE             0x02000904，软中断未激活。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V100R001C01
 * @see SRE_SwiValueGet | SRE_SwiStatusGet
 */
extern UINT32 SRE_SwiSelf(SWI_HANDLE_T *pusSwiID);


/**
 * @ingroup  SRE_swi
 * @brief 获取软中断优先级
 *
 * @par 描述:
 * 获取本核指定软中断的优先级
 *
 * @attention 无
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #0xFFFF    获取失败
 * @retval #其它值    获取优先级成功
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SwiValueGet | SRE_SwiStatusGet | SRE_SwiSelf
 */
extern SWI_PRIOR_T SRE_SwiPriorityGet(SWI_HANDLE_T uwSwiID);

/**
 * @ingroup  SRE_swi
 * @brief 获取软中断未接收消息的个数
 *
 * @par 描述:
 * 获取软中断未接收消息个数
 *
 * @attention 无
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 *
 * @retval #-1       获取失败
 * @retval #其它值   获取待接收的消息个数成功
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since RTOSck V100R001C01
 * @see SRE_SwiValueGet | SRE_SwiStatusGet | SRE_SwiSelf
 */
extern INT32 SRE_SwiMsgNumGet(SWI_HANDLE_T uwSwiID);


#if  (OS_HAVE_COPROCESSOR1 == YES)
/**
 * @ingroup  SRE_swi
 * @brief 软中断中执行矢量操作的入口。
 *
 * @par 描述:
 * 软中断中执行矢量操作的入口, 矢量操作开始之前需调用此接口。
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
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V200R003C05
 * @see SRE_TaskCpExit
 */
extern VOID SRE_SwiCpEntry(VOID);

/**
 * @ingroup  SRE_swi
 * @brief 在软中断中执行矢量操作的出口。
 *
 * @par 描述:
 * 软中断中执行矢量操作的出口，矢量操作完成之后需调用此接口。
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
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V200R003C05
 * @see SRE_SwiCpEntry
 */
extern VOID SRE_SwiCpExit(VOID);


/**
 * @ingroup  SRE_swi
 * @brief 给软中断分配矢量操作上下文数据保存区
 *
 * @par 描述:
 * 给指定软中断分配矢量操作上下文数据保存区
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6108/SD6181平台的DSP核，SD6182平台的DSP核，CPU核，HACCC核。</li>
 * <li>保存矢量操作上下文用到核的双LoadStore指令，建议配置的SaveAreaA/SaveAreaB在不同的Memory Bank中。</li>
 * <li>传入的矢量上下文数据保存区起始地址必须按16字节对齐。</li>
 * <li>矢量上下文数据保存区大小需由用户根据芯片实际存在的矢量寄存器总大小确定。</li>
 * </ul>
 *
 * @param  uwSwiID [IN] 类型为#SWI_HANDLE_T，软中断PID号。
 * @param  pstParam [IN] 类型#CP_SAVE_CFG_PARAM_S *，参数结构指针。
 *
 * @retval #OS_ERRNO_SWI_CP_SAVE_AREA_NULL      0x02000912，软中断的coprocessor上下文保存区域的地址为空指针。
 * @retval #OS_ERRNO_SWI_ID_INVALID             0x02000901，软中断PID非法。
 * @retval #OS_ERRNO_SWI_CP_SAVE_AREA_NOT_ALIGN 0x02000911，软中断的coprocessor上下文保存区域未按16字节对齐。
 * @retval #SRE_OK                          0x00000000，成功。
 *
 * @par 依赖:
 * <ul><li>sre_swi.h：该接口声明所在的头文件。</li></ul>
 * @since UniDSP V200R003C05
 * @see SRE_SwiCpEntry | SRE_SwiCpExit
 */
extern UINT32 SRE_SwiCpSaveCfg(SWI_HANDLE_T uwSwiID, CP_SAVE_CFG_PARAM_S *pstParam);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_SWI_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

