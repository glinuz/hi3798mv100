/**
 * @file sre_msg.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：消息模块的对外头文件。 \n
 */

/** @defgroup SRE_msg 消息
   *@ingroup SRE_comm
*/

#ifndef _SRE_MSG_H
#define _SRE_MSG_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/**
 * @ingroup SRE_msg
 * 无消息体发送消息时，第一个参数传入该值。
 */
#define OS_MSG_NO_PAYLOAD     0

/**
 * @ingroup SRE_msg
 * 使用于接收消息接口，消息等待时间设定，表示永久等待。
 */
#define OS_WAIT_FOREVER     0xFFFFFFFF

/**
 * @ingroup SRE_msg
 * 使用于接收消息接口，表示消息的发送者为硬中断。
 */
#define OS_MSG_HWI_SENDER   0x000000FF

/**
 * @ingroup SRE_msg
 * 消息错误码：申请消息体失败，输入ucAlignPow入参非法，输入的目的PID非法，分区内存已用尽或者本核消息节点资源不足
 *
 * 值: 0x00000000
 *
 * 解决方案:检查申请消息体接口入参是否合法，指定的分区内存是否充足等。
 */
#define OS_MSG_CREATE_ERR   0

/**
 * @ingroup SRE_msg
 * 消息错误码：消息接收函数中接收消息失败，可能原因是在硬中断中接收消息、在无消息的软中断中接收消息、任务中接收消息超时、或者正处于锁任务时进行超时等待。
 *
 * 值: 0xffffffff
 *
 * 解决方案:检查接收消息的线程类型，硬中断中不允许接收消息；确保在任务中超时等待消息时，系统不处于锁任务状态。
 */
#define OS_MSG_RECEIVE_ERR      0xFFFFFFFF

/**
 * @ingroup SRE_msg
 * 消息错误码：查询指定任务的指定消息队列失败。
 *
 * 值: 0xffffffff
 *
 * 解决方案: 只能查询本核已创建的任务；系统的idle任务不支持查询；只能查看任务的指定队列，且不能大于最大队列号。
 */
#define OS_MSGQ_NUM_GET_ERR     0xFFFFFFFF

/**
 * @ingroup SRE_msg
 * 消息错误码：消息长度获取函数(SRE_MsgLength)的错误返回值,消息句柄非法。
 *
 * 值: 0x00000000
 *
 * 解决方案: 检查传入的消息体指针是否正确。
 */
#define OS_MSG_LENGTH_ERR       0x00000000

/**
 * @ingroup SRE_msg
 * 消息错误码：传入的消息体句柄非法（错误或已删除或句柄指针为NULL），或者消息体正在被使用
 *
 * 值: 0x02000d00
 *
 * 解决方案: 检查该消息体是否只发送一次或者是否正在使用中；检查传入的消息体指针是否正确。
 */
#define OS_ERRNO_MSG_HANDLE_INVALID         SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x00)

/**
 * @ingroup SRE_msg
 * 消息错误码：发送消息时线程PID非法（线程非法、未创建或已删除，目的线程为系统的idle软中断、idle任务，目的任务消息队列非法）。
 * 注意:若错误信息从黑匣子中导出，则为核间通信时，目的核捕获到的错误信息。
 *
 * 值: 0x02000d01
 *
 * 解决方案: 检查接口传入的参数是否正确。
 */
#define OS_ERRNO_MSG_PID_INVALID            SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x01)

/**
 * @ingroup SRE_msg
 * 消息错误码：核内消息模块初始化过程中，申请私有静态内存失败
 *
 * 值: 0x02000d02
 *
 * 解决方案: 为私有静态内存配置更大内存空间
 *
 */
#define OS_ERRNO_MSG_NO_MEMORY              SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x02)

/**
 * @ingroup SRE_msg
 * 消息错误码：注册核内消息失败，核内消息节点的个数被配置为了0
 *
 * 值: 0x02000d03
 *
 * 解决方案: 若使用核内消息通信功能，请配置大于0的核内消息节点数
 *
 */
#define OS_ERRNO_MSG_REG_ERROR              SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x03)

/**
 * @ingroup SRE_msg
 * 消息错误码：发送消息时无空闲消息节点，可增加系统支持的最大消息节点数，
 * 注意:SRE_MsgQSend和SRE_MsgQRecv配对使用，以确保使用后的消息节点被释放
 *
 * 值: 0x02000d04
 *
 * 解决方案: 排查本核是否有短消息(仅82平台)，不带消息体消息未接收，若已接收，则可增加系统支持的最大消息节点数
 *
 */
#define OS_ERRNO_MSG_NO_MSGNODE             SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x04)

/**
 * @ingroup SRE_msg
 * 消息错误码：发送核间消息失败
 *
 * 值: 0x02000d05
 *
 * 解决方案: 查看目的核ID是否正确
 */
#define OS_ERRNO_MSG_MC_BUSY                SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x05)

/**
 * @ingroup SRE_msg
 * 消息错误码：任务消息队列号非法，大于该任务线程的最大消息队列号。
 *
 * 值: 0x02000d06
 *
 * 解决方案:确认传入的消息队列号是否合法。
 *
 */
#define OS_ERRNO_MSG_QID_INVALID            SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x06)

/**
 * @ingroup SRE_msg
 * 消息错误码：接收线程非法，在硬中断中接收
 *
 * 值: 0x02000d07
 *
 * 解决方案: 请确保不要在硬中断线程中接收消息。
 *
 */
#define OS_ERRNO_MSG_THREAD_INVALID         SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x07)

/**
 * @ingroup SRE_msg
 * 消息错误码：消息接收超时，在用户设置的超时时间内没有接收到消息。
 *
 * 值: 0x02000d08
 *
 * 解决方案: 确保超时时间内指定队列上有消息发送过来，如延时时间不够，可增大延时。
 *
 */
#define OS_ERRNO_MSG_RECEIVE_TIMEOUT        SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x08)

/**
 * @ingroup SRE_msg
 * 消息错误码：发送核间消息时，目的PID非法。
 *
 * 值: 0x02000d09
 *
 * 解决方案: 确保消息发送接口的目的PID是否正确。
 *
 */
#define OS_ERRNO_MCMSG_PID_INVALID          SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x09)

/**
 * @ingroup SRE_msg
 * 消息错误码：接收队列中无消息。
 *
 * 值: 0x02000d0a
 *
 * 解决方案: 请检查是否向该消息队列中发送了消息。
 *
 */
#define OS_ERRNO_MSG_QUEUE_EMPTY            SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0a)

/**
 * @ingroup SRE_msg
 * 消息错误码：发送核间消息时，核间消息模块未打开。
 *
 * 值: 0x02000d0b
 *
 * 解决方案: 请检查传入的消息类型值是否有效。
 *
 */
#define OS_ERRNO_MCMSG_HOOK_NULL             SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0b)

/**
 * @ingroup SRE_msg
 * 消息错误码：释放消息体内存时，传入的句柄地址未32字节对齐。
 *
 * 值: 0x02000d0c
 *
 * 解决方案: 请检查传入的消息体句柄值是否有效。
 *
 */
#define OS_ERRNO_MSG_HANDLE_ADDR_UNALIGN          SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0c)

/**
 * @ingroup SRE_msg
 * 消息错误码：短消息未接收处理，被强制释放
 *
 * 值: 0x02000d0d
 *
 * 解决方案: 请确保该消息被接收处理后再释放，以免造成内存泄漏
 *
 */
#define OS_ERRNO_MSG_SMSG_CAN_NOT_DELETE          SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0d)

/**
 * @ingroup SRE_msg
 * 消息错误码：申请消息体，输入的消息体内存大小参数为零
 *
 * 值: 0x02000d0e
 *
 * 解决方案: 请确保传入的消息体内存大小是否有效。
 *
 */
#define OS_ERRNO_MSG_MEM_SIZE_ZERO          SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0e)

/**
 * @ingroup SRE_msg
 * 消息错误码：申请消息体，从指定分区中申请不到足够大小的消息体内存
 *
 * 值: 0x02000d0f
 *
 * 解决方案: 检查传入的分区号是否有效，若有效则减小申请的消息体内存或者扩大指定的分区内存大小。
 *
 */
#define OS_ERRNO_MSG_MEM_ALLOC_FAIL         SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x0f)

/**
 * @ingroup SRE_msg
 * 消息错误码：长消息未接收处理，被强制释放消息体内存
 *
 * 值: 0x02000d10
 *
 * 解决方案: 请确保该消息被接收处理后再释放，以免造成内存泄漏
 *
 */
#define OS_ERRNO_MSG_LMSG_CAN_NOT_DELETE          SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x10)

/**
 * @ingroup SRE_msg
 * 消息错误码：获取本核短消息体长度值失败
 *
 * 值: 0x02000d11
 *
 * 解决方案: 在释放该短消息前，输入合理的消息句柄值以获取正确的短消息体长度
 *
 */
#define OS_ERRNO_MSG_GET_SMSG_LENGTH_FAIL        SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x11)

/**
 * @ingroup SRE_msg
 * 消息错误码：获取本核长消息体长度值失败
 *
 * 值: 0x02000d12
 *
 * 解决方案: 在释放该长消息前，输入合理的消息句柄值以获取正确的长消息体长度
 *
 */
#define OS_ERRNO_MSG_GET_LMSG_LENGTH_FAIL       SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x12)

/**
 * @ingroup SRE_msg
 * 消息错误码：82平台消息接收时，接收到消息类型为长消息，但长消息魔术字校验错误。
 *
 * 值: 0x02000d13
 *
 * 解决方案: 检查82平台使用SRE_QportSend发送OS消息时，消息是否由SRE_QportMsgCreate函数创建。
 *
 */
#define OS_ERRNO_MSG_LONGMSG_INVALID            SRE_ERRNO_OS_ERROR(OS_MID_MSG, 0x13)


/**
 * @ingroup SRE_msg
 * 消息体句柄类型定义。
 */
typedef UINT32 MSG_HANDLE_T;

/**
 * @ingroup SRE_msg
 * 消息发送者或接收者的线程PID类型定义。
 */
typedef UINT32 MSG_PID_T;

/**
 * @ingroup SRE_msg
 * 消息模块配置信息的结构体定义。
 */
typedef struct tagMsgModInfo {
	UINT32 uwMaxMsgNum;                  /*配置的最大消息结点个数  */
} MSG_MOD_INFO_S;

/**
 *@ingroup SRE_msg
 *@brief 消息体的申请函数。
 *
 *@par 描述:
 *申请指定大小的消息体内存。
 *@attention
 *@li 该接口与消息体释放接口#SRE_MsgFree配对使用，消息发送后需要释放。
 *@li 如果消息只需要传递消息ID号，则可省略申请、释放消息体步骤。
 *@li 消息通信需用户指定内存分区，uwPtNo作为申请消息体的内存分区号使用。
 *@li 核内消息通信可以是私有内存分区，也可以是共享内存分区，但建议使用私有内存以提高效率；核间的必须是共享内存分区
 *@li 说明：消息至少对齐为32字节对齐，如果用户输入的对齐方式小于32字节对齐则采用32字节，内部内存申请采用32字节对齐申请。
 *共享和私有pool算法实际消耗：用户对齐字节数（至少32对齐）+ 内存分配消耗32或64字节（mcpool算法管理可cache分区时消耗64字节，其他为32字节） + 实际用户size。
 *共享和私有fsc算法最坏消耗：用户对齐字节数（至少32对齐）+ 内存分配消耗44字节 + 实际用户size。
 *
 *@param uwPtNo     [IN] 类型#UINT32，分区号。
 *@param usSize     [IN] 类型#UINT16，需要申请的消息体大小（单位Byte）。
 *@param uwAlignPow [IN] 类型#UINT32，消息体内存的对齐方式。取值范围为[0, 8]。
 *采用2的幂次方形式表示，比如需要64对齐，则该参数应为6（2的6次方为64）。
 *当传入的数据ucAlignPow小于5时均按32字节对齐。
 *
 *@retval #OS_MSG_CREATE_ERR 0x00000000，申请失败，失败原因很可能是对齐方式大于8，申请所在分区未创建。
 *@retval 非0值，表示申请到的消息体句柄。
 *
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgFree
*/
extern MSG_HANDLE_T SRE_MsgAlloc(UINT32 uwPtNo, UINT16 usSize, UINT32 uwAlignPow);

/**
 *@ingroup SRE_msg
 *@brief 消息释放函数
 *
 *@par 描述:
 *消息体释放函数，用在消息体成功接收并使用完成之后。
 *@attention
 *@li 此函数需要与接收消息体的#SRE_MsgQRecv或者#SRE_MsgReceive成对使用，先接收然后释放。
 *@li 消息的接收和发送需准确配对，这样才能保证消息体的正常释放。
 *@li 无消息体的消息，不需要调用此函数释放消息体。
 *@li 可以通过函数返回值是否为#SRE_OK来判断消息是否释放成功。
 *@li 该函数入参正确性由用户保证，OS只对是否为0地址进行判断。
 *
 *@param uwMsgHandle [IN] 类型为#MSG_HANDLE_T，消息体句柄。
 *
 *@retval #SRE_OK                                       0x00000000，操作成功。
 *@retval #OS_ERRNO_MSG_HANDLE_INVALID                  0x02000d00，消息体句柄为非法值或者消息体正在被使用。
 *@retval #OS_ERRNO_MSG_SMSG_CAN_NOT_DELETE             0x02000d0d，短消息不能被删除，可能未被接收处理或者未被使用。
 *@retval #OS_ERRNO_MSG_HANDLE_ADDR_UNALIGN             0x02000d0c，输入的消息体句柄地址值未32字节对齐。
 *@retval #OS_ERRNO_MSG_LMSG_CAN_NOT_DELETE             0x02000d10，长消息不能被删除，可能未被接收处理或者未被使用。
 *@retval #OS_ERRNO_MEM_FREE_PTNO_INVALID               0x02000104，释放地址不在内存分区中。
 *@retval #OS_ERRNO_MEM_FREE_SH_DAMAGED                 0x02000311，内存释放时要释放的内存块的头被破坏，或已释放。
 *@retval #OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE      0x02000314，私有POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *@retval #OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE     0x02000419，共享动态POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *@retval #OS_ERRNO_MCSTA_FSCMEM_FREE_ISFORBIDEN        0x0200041e，共享静态FSC的内存不能释放。
 *@retval #OS_ERRNO_MEM_FREE_ADDR_INVALID               0x02000103，释放地址为空。
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgAlloc
*/
extern UINT32 SRE_MsgDelete(MSG_HANDLE_T uwMsgHandle);

/**
 *@ingroup SRE_msg
 *@brief 释放创建成功的消息体。
 *
 *@par 描述:

 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgDelete
*/
#define SRE_MsgFree(pHandle)       (UINT32)SRE_MsgDelete(pHandle)


/**
 *@ingroup SRE_msg
 *@brief 释放接收成功的消息体
 *
 *@par 描述:

 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgDelete
*/
#define SRE_MsgRelease(pHandle)     (UINT32)SRE_MsgDelete(pHandle)

/**
 *@ingroup SRE_msg
 *@brief 向指定任务或软中断的指定队列发消息。
 *
 *@par 描述:
 *发送消息到指定的任务的指定队列，支持核内和核间消息的发送。
 *发送消息到软中断的0队列，支持核内和核间消息的发送。
 *当任务因为接收该队列的消息而阻塞，则会唤醒该任务。
 *@attention
 *@li 目的线程只能是任务和软中断，不能是硬中断。目的线程请勿是系统占用的线程，比如IDLE任务。
 *@li 如果目的任务正在为接收该队列的消息而阻塞，则会唤醒该任务。若目的任务优先级高于其所在核中正在运行的任务，则会发生任务切换。
 *@li 如果目的线程是软中断，则直接激活该软中断，使其处于就绪态。
 *@li 消息可以不包含消息体，若发送不带消息体消息，则MSG_HANDLE_T入参为OS_MSG_NO_PAYLOAD，
 *@li 不包括消息体发送，SD6181/SD6108/SD6182平台只能传递4字节数据。
 *@li 若需要附带消息体，则消息体应来源于#SRE_MsgAlloc创建的消息体或#SRE_MsgRecv接收的其他消息体。
 *@li 其中带消息体发送，SD6182又有长短消息之分，用户传入数据不大于8字节，OS内部走短消息流程以提高通信效率。反之，OS内部走长消息流程，与SD6181/SD6108相似。
 *@li 核间发送时，用户需要自行保证接收端的核真实存在，以免造成内存泄漏。

 *@param uwMsgHandle    [IN] 类型#MSG_HANDLE_T，消息体句柄，来源于消息体创建成功或接收成功的输出值。
 *@param uwMsgID        [IN] 类型#UINT32，消息ID，用以标识该消息的唯一性。
 *@param uwDstPID       [IN] 类型#MSG_PID_T，消息目的线程PID。可以是核内或核间的任务或软中断的PID。
 *@param ucDstQID       [IN] 类型#UINT8，消息队列号(发送给软中断，此参数无效，但参数值应小于8，与任务保持一致)，任务的最大消息队列数是8，所以最大的队列号是7。
 *
 *@retval #SRE_OK                       0x00000000，操作成功。
 *@retval #OS_ERRNO_MSG_HANDLE_INVALID  0x02000d00，消息体句柄为非法值，或已被删除。
 *@retval #OS_ERRNO_MSG_PID_INVALID     0x02000d01，线程PID为非法值，或线程已被删除。
 *@retval #OS_ERRNO_MSG_NO_MSGNODE      0x02000d04，无空闲消息节点。
 *@retval #OS_ERRNO_MSG_MC_BUSY         0x02000d05，核间操作队列已满，发送核间消息失败。
 *@retval #OS_ERRNO_MSG_QID_INVALID     0x02000d06，向任务发消息时指定的接收消息的任务的Qid非法。
 *@retval #OS_ERRNO_IPC_SHQ_FULL        0x02001486，SD6181和SD6108平台下，发送核间中断，共享队列已满。
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgQRecv
*/
extern UINT32 SRE_MsgQSend(MSG_HANDLE_T uwMsgHandle, UINT32 uwMsgID, MSG_PID_T uwDstPID, UINT8 ucDstQID);

/**
 *@ingroup SRE_msg
 *@brief 从任务或软中断的消息队列中获取消息。
 *
 *@par 描述:
 *从任务自身的指定的消息队列中获取第一条消息，消息的接收采取先到先收的顺序进行。
 *从软中断的0号消息队列中获取第一条消息，消息的接收采取先到先收的顺序进行。
 *仅支持SD6108/SD6181/SD6182平台。
 *@attention
 *@li 软中断不支持多消息队列，故在软中断中调用此接口时，ucRecvQID参数无效，默认从0号消息队列接收消息。
 *@li 软中断不支持超时等待，故在软中断中调用此接口时，uwTimeout参数无效，若接收失败则直接返回不做等待。
 *@li 有消息体消息，接收成功后可使用*puwMsgHandle句柄获取消息体首地址。
 *@li 无消息体的消息的发送与接收依赖于消息节点作为中介，发送过程会获取一个消息节点，接收过程释放这个消息节点。系统可以使用的消息节点最大数量受配置项中核内、核间消息节点数限制。
 *
 *@param puwMsgHandle   [OUT] 类型#MSG_HANDLE_T *，对于有消息体消息，puwMsgHandle为返回的消息体首地址，若不需要此项输出，填写NULL即可。
 *@param puwMsgID       [OUT] 类型#UINT32 *，接收到的消息ID。若不需要此项输出，填写NULL即可。
 *@param puwSenderPID   [OUT] 类型#MSG_PID_T *，消息发送者的PID，为#OS_MSG_HWI_SENDER时表示发送者为硬中断。若不需要此项输出，填写NULL即可。
 *@param ucRecvQID      [IN]  类型#UINT8，任务的指定接收消息队列号；软中断线程接收时，此参数无效。
 *@param uwTimeout      [IN]  类型#UINT32，任务接收消息的等待时间限制，单位为tick，取值范围为[0, 0xffffffff]；软中断接收消息时，此参数无效。
 *
 *@retval #SRE_OK                       0x00000000，表示接收消息成功。
 *@retval #OS_ERRNO_MSG_QID_INVALID     0x02000d09，任务的消息队列ID非法。
 *@retval #OS_ERRNO_MSG_THREAD_INVALID  0x02000d0a，接收线程非法，不支持在硬中断中接收消息。
 *@retval #OS_ERRNO_MSG_RECEIVE_TIMEOUT 0x02000d0b，消息接收超时，在用户设置的超时时间内没有接收到消息。
 *@retval #OS_ERRNO_MSG_QUEUE_EMPTY     0x02000d0e，该消息队列中无消息。
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgQSend
*/
extern UINT32 SRE_MsgQRecv(MSG_HANDLE_T *puwMsgHandle, UINT32 *puwMsgID, MSG_PID_T *puwSenderPID, UINT8 ucRecvQID, UINT32 uwTimeout);

/**
 *@ingroup SRE_msg
 *@brief 获取本核任务的指定消息队列的消息数。
 *
 *@par 描述:
 *查询本核的任务的指定队列，返回该队列的消息数。
 *仅支持SD6108/SD6181/SD6182平台。
 *@attention
 *@li 只能是查询任务的指定队列消息数，不能是软中断或硬中断，且不能是IDLE任务。
 *@li 只能是查询本核的任务的指定消息队列消息数，不能查询其他核的任务消息队列的消息数。
 *
 *@param uwPID [IN] 类型#UINT32，只能是本核内的任务的PID。
 *@param ucQID [IN] 类型#UINT8，指定任务的消息队列号。

 *@retval  #OS_MSGQ_NUM_GET_ERR  0xFFFFFFFF，任务的PID非法，IDLE任务的PID或者指定任务的消息队列ID非法。
 *@retval  其他               指定任务的指定消息队列的消息数目。
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgQRecv|SRE_MsgQSend
*/
extern UINT32 SRE_MsgQNumGet(UINT32 uwPID, UINT8 ucQID);

/**
 *@ingroup SRE_msg
 *@brief 消息发送函数。
 *
 *@par 描述:
 *默认发送到目的端线程的0号消息队列，其它功能描述见SRE_MsgQSend
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgReceive
*/
#define SRE_MsgSend(uwMsgHandle, uwMsgID, uwDstPID) \
        SRE_MsgQSend(uwMsgHandle, uwMsgID, uwDstPID, 0)


/**
 *@ingroup SRE_msg
 *@brief 消息获取函数。
 *
 *@par 描述:
 *默认线程的0号消息队列接收消息，若带消息体消息接收成功，则返回消息体句柄uwMsgHandle。其它功能描述见SRE_MsgQRecv。
 *@attention
 *@li 当接收的是无消息体消息，则返回的句柄值uwMsgHandle为0，若接收的是带消息体消息，返回的句柄值uwMsgHandle是消息体首地址。
 *
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgSend
*/
extern MSG_HANDLE_T SRE_MsgReceive(UINT32 uwTimeout,
				   UINT32 *puwMsgID,
				   MSG_PID_T *puwSenderPID);

extern UINT32 SRE_MsgSnd(UINT32 uwMsgID, MSG_PID_T uwDstPID, void *msgp, UINT32 size);
extern UINT32 SRE_MsgRcv(UINT32 uwTimeout, UINT32 *puwMsgID, void *msgp, UINT32 size, MSG_PID_T *puwSenderPID);


/**
 *@ingroup SRE_msg
 *@brief 获取消息体长度。
 *
 *@par 描述:
 *获取消息内容长度，单位:字节。
 *@attention 用户需保证传入消息句柄的有效性
 *
 *@param uwMsgHandle [IN] 类型#MSG_HANDLE_T，消息体句柄，来源于消息体创建成功或接收成功的输出值。
 *
 *@retval  #OS_MSG_LENGTH_ERR  0x00000000，消息体句柄非法、已被删除或消息无消息体。
 *@retval  非0                 消息体长度。
 *@par 依赖:
 *@li SRE_msg.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C01
 *@see SRE_MsgAlloc
*/
extern UINT16 SRE_MsgLength(MSG_HANDLE_T uwMsgHandle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_MSG_H */

/**
 * History:
 * 2009-10-30 z00134877: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */


