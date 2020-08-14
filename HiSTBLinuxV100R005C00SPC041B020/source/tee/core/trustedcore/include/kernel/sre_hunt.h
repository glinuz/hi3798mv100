/**
  * @file sre_hunt.h
  *
  * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
  *
  * 描述：PID获取的对外头文件。\n
  */

#ifndef _SRE_HUNT_H
#define _SRE_HUNT_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/**@defgroup sre_hunt Hunt处理
  *@ingroup SRE_comm
  */

/**
 * @ingroup sre_hunt
 * HUNT错误码：HUNT模块初始化失败.
 *
 * 值: 0x02001f00
 *
 * 解决方案：确认申请共享静态fsc内存或者私有fsc内存分区是否失败
 */
#define OS_ERRNO_HUNT_INIT_ERROR        SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x00)

/**
 * @ingroup sre_hunt
 * HUNT错误码：创建任务或软中断时，线程名重名.
 *
 * 值: 0x02001f01
 *
 * 解决方案：创建任务或软中断时，确保线程名本核内唯一.
 */
#define OS_ERRNO_HUNT_THREAD_NAME_REPEAT    SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x01)

/**
 * @ingroup sre_hunt
 * HUNT错误码：通过线程ID或线程名获取线程PID时，该线程未创建或已删除.
 *
 * 值: 0x02001f02
 *
 * 解决方案：确保线程已创建且未被删除.
 */
#define OS_ERRNO_HUNT_THREAD_NOT_CREAT      SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x02)

/**
 * @ingroup sre_hunt
 * HUNT错误码：通过线程ID或线程名获取线程PID时，入参非法.
 *
 * 值: 0x02001f03
 *
 * 解决方案：确保指针非空，以及核号合法.
 */
#define OS_ERRNO_HUNT_ILLEGAL_INPUT         SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x03)

/**
*@ingroup sre_hunt
*@brief 通过指定核的线程(任务、软中断)名获取其PID。
*
*@par 描述:
*通过本接口获取线程PID。
*@attention
*SRE_HuntByName函数的使用有两种方式，用户可以二选一:
*1）每个核都在OS初始化完成之后调用SRE_HuntMature，然后调用SRE_HuntByName；
*2）每个核都在OS初始化完成之后调用SRE_WaitForAllCores，然后调用SRE_HuntByName;
*@param ucCoreID  [IN] 类型为#UINT8，线程所在核号，小于核数。
*@param pThreadName [IN] 类型为#CHAR*，线程名,不超过15个字符。
*@param puwPID [OUT] 类型为#UINT32*，线程PID。
*
*@retval #SRE_OK ,获取PID成功。
*@retval #OS_ERRNO_HUNT_THREAD_NOT_CREAT 0x02001f02，该线程未创建或已删除。
*@retval #OS_ERRNO_HUNT_ILLEGAL_INPUT    0x02001f03，入参非法。
*@par 依赖:
*sre_hunt.h：该接口声明所在的头文件。
*@since RTOSck V100R001C01
*@see SRE_HuntMature
*/
extern UINT32 SRE_HuntByName(UINT8 ucCoreID, CHAR  *pThreadName, UINT32 *puwPID);


/**
*@ingroup sre_hunt
*@brief 标记本核所有的任务和软中断已创建完成（OS的IDLE除外），其它核可以成功查询本核的PID。
*
*@par 描述:
*置上本核mature标志。同时置标志：使得本核在调用#SRE_HuntByName时，如果目的核没有启动，则一直等待，直到目的核置上了mature标志才返回查询结果。
*@attention
*必须在本核所有的任务和软中断（OS的IDLE除外）创建完成之后调用。SRE_HuntByName必须在SRE_HuntMature执行之后调用.
*@param 无。
*
*@retval 无。
*@par 依赖:
*sre_hunt.h：该接口声明所在的头文件。
*@since RTOSck V100R001C01
*@see SRE_HuntByName
*/
extern VOID SRE_HuntMature(VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _SRE_HUNT_H */

/**
 * History:
 * 2010-03-10 z00134877: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */
