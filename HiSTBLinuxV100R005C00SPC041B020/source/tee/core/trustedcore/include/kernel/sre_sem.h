/**
 * @file sre_sem.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：信号量模块对外头文件。 \n
 */

#ifndef _SRE_SEM_H
#define _SRE_SEM_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**@defgroup SRE_sem 信号量
  *@ingroup SRE_comm
  */

/**
 * @ingroup SRE_sem
 * 信号量等待时间设定：表示永久等待。
 */
#define OS_WAIT_FOREVER     0xFFFFFFFF

/**
 * @ingroup SRE_sem
 * 信号量错误码：初始化信号量内存不足
 *
 * 值: 0x02000f00
 *
 * 解决方案: 可以将私有的静态内存空间配大
 */
#define OS_ERRNO_SEM_NO_MEMORY         SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x00)
/**
 * @ingroup SRE_sem
 * 信号量错误码：信号量句柄非法（错误或已删除）
 *
 * 值: 0x02000f01
 *
 * 解决方案: 查看输入的信号量句柄值是否有效
 */
#define OS_ERRNO_SEM_INVALID           SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x01)
/**
 * @ingroup SRE_sem
 * 信号量错误码：输入指针为空
 *
 * 值: 0x02000f02
 *
 * 解决方案: 查看指针是否输入为空
 */
#define OS_ERRNO_SEM_PTR_NULL       SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x02)
/**
 * @ingroup SRE_sem
 * 信号量错误码：没有空闲信号量
 *
 * 值: 0x02000f03
 *
 * 解决方案: 查看信号量模块是否打开，或配置更多信号量
 */
#define OS_ERRNO_SEM_ALL_BUSY          SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x03)
/**
 * @ingroup SRE_sem
 * 信号量错误码：信号量获取失败
 *
 * 值: 0x02000f04
 *
 * 解决方案: 查看是否将等待时间配置成0，此值无效。
 */
#define OS_ERRNO_SEM_UNAVAILABLE       SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x04)
/**
 * @ingroup SRE_sem
 * 信号量错误码：禁止中断处理函数阻塞于信号量
 *
 * 值: 0x02000f05
 *
 * 解决方案: 查看是否在中断中Pend信号量
 */
#define OS_ERRNO_SEM_PEND_INTERR       SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x05)
/**
 * @ingroup SRE_sem
 * 信号量错误码：任务切换锁定时，禁止任务阻塞于信号量
 *
 * 值: 0x02000f06
 *
 * 解决方案: 无
 */
#define OS_ERRNO_SEM_PEND_IN_LOCK      SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x06)
/**
 * @ingroup SRE_sem
 * 信号量错误码：信号量等待超时
 *
 * 值: 0x02000f07
 *
 * 解决方案: 无
 */
#define OS_ERRNO_SEM_TIMEOUT           SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x07)
/**
 * @ingroup SRE_sem
 * 信号量错误码：信号量发生溢出
 *
 * 值: 0x02000f08
 *
 * 解决方案: 查看输入的信号量计数值是否有效
 */
#define OS_ERRNO_SEM_OVERFLOW          SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x08)
/**
 * @ingroup SRE_sem
 * 信号量错误码：有阻塞于信号量的任务
 *
 * 值: 0x02000f09
 *
 * 解决方案: 无
 */
#define OS_ERRNO_SEM_PENDED            SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x09)

/**
 * @ingroup SRE_sem
 * 信号量错误码：信号量计数获取失败
 *
 * 值: 0x02000f0A
 *
 * 解决方案: 查看信号量句柄是否合法，或信号量是否已创建。
 */
#define OS_ERRNO_SEM_GET_COUNT_ERR      SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x0A)


/**
 * @ingroup SRE_sem
 * 信号量错误码：注册核内信号量失败
 *
 * 值: 0x02000f10
 *
 * 解决方案: 查看信号量模块信息配置是否正确
 */
#define OS_ERRNO_SEM_REG_ERROR         SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x10)

/**
 * @ingroup SRE_sem
 * 信号量错误码：调用#SRE_SemPendListGet时，指定的内存空间不足，无法存入全部的阻塞任务PID。
 *
 * 值: 0x02000f11
 *
 * 解决方案: 建议将数组长度配置为(#OS_TSK_MAX_SUPPORT_NUM + 1) * 4
 */
#define OS_ERRNO_SEM_INPUT_BUF_NOT_ENOUGH    SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x11)

/**
 * @ingroup SRE_sem
 * 信号量错误码：调用#SRE_SemPendListGet时，输入指针为空或者uwBufLen小于4
 *
 * 值: 0x02000f12
 *
 * 解决方案: 无
 */
#define OS_ERRNO_SEM_INPUT_ERROR       SRE_ERRNO_OS_ERROR(OS_MID_SEM, 0x12)

/**
 * @ingroup SRE_sem
 * 信号量句柄类型定义。
 */
typedef UINT32 SEM_HANDLE_T;

/**
 * @ingroup SRE_sem
 * 信号量模块配置信息的结构体定义。
 */
typedef struct tagSemModInfo {
	UINT16 usMaxNum;                /**< 最大支持的信号量数  */
} SEM_MOD_INFO_S;


/**
 *@ingroup SRE_sem
 *@brief 创建一个信号量。
 *
 *@par 描述:
 *根据用户指定的计数值，创建一个信号量，设定初始计数器数值。
 *@attention
 *@li 创建是否成功会受到"核内信号量裁剪开关"和"最大支持信号量"配置项的限制。
 *
 *@param uwCount [IN] 类型为#UINT32，计数器初始值，取值范围为[0, 0xFFFFFFFE]。
 *@param puwSemHandle [OUT] 类型为#SEM_HANDLE_T *，输出信号量句柄。
 *
 *@retval #SRE_OK            0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_OVERFLOW 0x02000f08，计数器初始值超出范围。
 *@retval #OS_ERRNO_SEM_ALL_BUSY 0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 *@retval #OS_ERRNO_SEM_PTR_NULL 0x02000f02，入参指针为空。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemDelete
*/
extern  UINT32 SRE_SemCreate(UINT32 uwCount,  SEM_HANDLE_T *puwSemHandle);

/**
 *@ingroup SRE_sem
 *@brief 删除一个信号量。
 *
 *@par 描述:
 *删除用户传入的信号量句柄指定的信号量，如果有任务阻塞于该信号量，则删除失败。
 *@attention 无
 *@li 不能删除有任务阻塞的信号量。
 *
 *@param uwSemHandle [IN] 类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *
 *@retval #SRE_OK           0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_INVALID 0x02000f01，信号量句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_SEM_PENDED  0x02000f09，有任务阻塞于该信号量，禁止删除。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemCreate
*/
extern UINT32 SRE_SemDelete(SEM_HANDLE_T uwSemHandle);

/**
 *@ingroup SRE_sem
 *@brief 重设信号量计数器数值函数。
 *
 *@par 描述:
 *根据用户输入信号量句柄和计数值，重设信号量计数器数值。
 *@attention 无
 *@li 如果有任务阻塞于该信号量，则重设失败。
 *
 *@param uwSemHandle [IN] 类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *@param uwCount [IN] 类型为#UINT32，计数器设定值，取值范围为[0, 0xFFFFFFFE]。
 *
 *@retval #SRE_OK            0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_INVALID  0x02000f01，信号量句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_SEM_OVERFLOW 0x02000f08，计数器设定值超出范围。
 *@retval #OS_ERRNO_SEM_PENDED   0x02000f09，有任务阻塞于该信号量，禁止重设。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemCreate | SRE_SemCountGet
*/
extern UINT32 SRE_SemReset(SEM_HANDLE_T uwSemHandle, UINT32 uwCount);

/**
 *@ingroup SRE_sem
 *@brief 获取信号量计数器数值。
 *
 *@par 描述:
 *根据用户输入信号量句柄和计数值，获取信号量计数器数值。
 *@attention 无
 *
 *@param uwSemHandle [IN]  类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *@param puwSemCnt   [OUT] 类型为#UINT32 *，信号量计数值，范围[0~0xfffffffe]。
 *
 *@retval #OS_ERRNO_SEM_GET_COUNT_ERR   0x02000f0A 获取失败，信号量句柄错误或已被删除。
 *@retval #SRE_OK                       0x00000000 获取信号量计数器值成功。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemCreate | SRE_SemReset
*/
extern UINT32 SRE_SemCountGet(SEM_HANDLE_T uwSemHandle, UINT32 *puwSemCnt);


/**
 *@ingroup SRE_sem
 *@brief 等待一个信号量。
 *
 *@par 描述:
 *等待用户传入信号量句柄指定的信号量，若其计数器值大于0，则直接减1返回成功。否则任务阻塞，
 *等待其他线程发布该信号量，等待的容忍时间可设定。
 *@attention
 *@li 在osStart之前不能调用该接口。
 *@li 等待时间可以选择零等待、等待特定时间、永久等待。
 *@li 当没有信号量资源时，非零等待时该函数只能被任务调用。
 *@li 在锁任务情况下，用户要PEND信号量，要保证当前有可用信号量资源。
 *@li 当在软中断或硬中断中调用此函数时，若有可用资源则pend成功，否则pend失败，uwTimeout参数无效。
 *
 *@param uwSemHandle [IN] 类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *@param uwTimeout [IN] 类型为#UINT32，等待时间限制，单位为tick，取值范围为[0, 0xffffffff]。
 *#OS_NO_WAIT或0表示不等待，#OS_WAIT_FOREVER或0xffffffff表示永久等待。
 *
 *@retval #SRE_OK                0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_INVALID      0x02000f01，信号量句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_SEM_UNAVAILABLE  0x02000f04，信号量零等待时获取失败。
 *@retval #OS_ERRNO_SEM_PEND_INTERR  0x02000f05，中断处理函数禁止发生阻塞。
 *@retval #OS_ERRNO_SEM_PEND_IN_LOCK 0x02000f06，系统可用资源为0且任务切换锁定时，禁止任务发生阻塞。
 *@retval #OS_ERRNO_SEM_TIMEOUT      0x02000f07，信号量等待超时。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemPend
*/
extern UINT32 SRE_SemPend(SEM_HANDLE_T uwSemHandle, UINT32 uwTimeout);

/**
 *@ingroup SRE_sem
 *@brief 发布指定的信号量。
 *
 *@par 描述:
 *发布指定的信号量，若没有任务等待该信号量，则直接将计数器加1返回。
 *否则唤醒为此信号量阻塞的任务列表中的第一个任务（最早阻塞的）。
 *@attention
 *@li 在osStart之前不能调用该接口
 *@li 在未锁任务的情况下，如果唤醒的任务优先级高于当前任务，则会立刻发生任务切换。
 *
 *@param uwSemHandle [IN] 类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *
 *@retval #SRE_OK            0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_INVALID  0x02000f01，信号量句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_SEM_OVERFLOW 0x02000f08，信号量加操作后溢出。
 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_SemPend
*/
extern UINT32 SRE_SemPost(SEM_HANDLE_T uwSemHandle);

/**
 *@ingroup SRE_sem
 *@brief 获取阻塞在指定核内信号量上的任务PID清单。
 *
 *@par 描述:
 *根据用户指定的核间信号量句柄，获取阻塞在指定核内信号量上的任务PID清单。若有任务阻塞于该核内信号量，则返回阻塞于该核内信号量的任务数目，以及相应任务的PID。
 *若没有任务阻塞于该核内信号量，则返回阻塞于该核内信号量的任务数目为0。
 *@attention
 *@li 用户应保证存储任务PID清单的内存空间足够大，建议将uwBufLen配置为(#OS_TSK_MAX_SUPPORT_NUM + 1) * 4bytes。
 *
 *@param uwSemHandle   [IN]  类型为#SEM_HANDLE_T，信号量句柄，来源于信号量创建成功的输出值。
 *@param puwTskCnt [OUT] 类型为#UINT32 *，阻塞于该核内信号量的任务数。
 *@param puwPidBuf [OUT] 类型为#UINT32 *，由用户指定的内存区域首地址，用于存储阻塞于指定核内信号量的任务PID。
 *@param uwBufLen  [IN]  类型为#UINT32，用户指定的内存区域的长度（单位：字节）。
 *
 *@retval #SRE_OK                        0x00000000，操作成功。
 *@retval #OS_ERRNO_SEM_INVALID              0x02000f01，信号量句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_SEM_INPUT_ERROR          0x02000f12，指针为空或者uwBufLen小于4。
 *@retval #OS_ERRNO_SEM_INPUT_BUF_NOT_ENOUGH 0x02000f11，指定的内存空间不足，但内存中的任务PID有效。

 *@par 依赖:
 *@li sre_sem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_TaskPendSemGet | SRE_McSemPendListGet
*/
extern UINT32 SRE_SemPendListGet(SEM_HANDLE_T uwSemHandle,
				 UINT32 *puwTskCnt,
				 UINT32 *puwPidBuf,
				 UINT32  uwBufLen);
extern UINT32 SRE_SemRelease(UINT32 uwTaskID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* _SRE_SEM_H */

/*
 * History: \n
 * 2009-3-4, h00133330, Create this file. \n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */
