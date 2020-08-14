/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_errno.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2011年10月26日
  最近修改   :
  功能描述   : ICS终端软件平台的错误码公共头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年10月26日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ICS_ERRNO_H__
#define __ICS_ERRNO_H__

#include "osal_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

/*定义平台断言*/
#define ICS_ASSERT(a) ((void *)0)

/**
 *@ingroup linux_sme_base Linux
 *描述:生成模块内的私有错误码。(模块ID + 模块内具体错误码 = 完整的错误码)
 *错误码说明（ERRNO）：
 *1、用于标识一类错误，占用两个字节
 *2、采用“公共错误码＋私有错误码”的定义方式
 *3、公共错误码全局唯一（系统中大部分的返回值都采用公共错误码）
 *4、私有错误码各个模块内部唯一即可（不同模块间的ERRNO可以相同）
 *范围说明:
 *0     ICS_OK，特殊错误码，代表成功
 *0X1000 - 0X1FFF 模块私有错误码，顺序定义
 *0X2000 - 0X2FFF 公共错误码，集中顺序定义
 *0X3000 - 0XFFFF 暂时保留，其中部分字段被信息标识使用
 *-1 ICS_ERROR，特殊错误码
 *错误ID（ERRID）：
 *1、用于标识某个模块的某类错误，占用四个字节，为了描述方便定义这个概念
 *2、等于 MID（高两个字节）＋ERRNO（低两个字节）
 *---------------------------------
 *|31-----------16|15-------------0|
 *---------------------------------
 *模块ID         模块内的错误码id
*/
#define ICS_ERRNO_BEGIN_USER              0X1000                     /* 模块内的私有错误码基础值 */
#define ICS_ERRNO_BEGIN_COMM              0X2000                     /* 模块内的公共错误码基础值 */
#define ICS_ERRNO_MK_USERNO(n)            (ICS_ERRNO_BEGIN_USER+(n)) /* 计算模块内的私有错误码 n+基础值 */
#define ICS_ERRNO_MK_COMMNO(n)            (ICS_ERRNO_BEGIN_COMM+(n)) /* 计算模块内的公共错误码 n+基础值 */
#define ICS_MID_MK_HI16(MID)              ((MID) << 16)              /* 模块ID */
#define ICS_ERRNO_MK_MODUSER(MID, n)      ICS_MID_MK_HI16(MID) + ICS_ERRNO_MK_USERNO(n) /* 模块内私有错误码 */
#define ICS_ERRNO_MK_MODCOMM(MID, n)      ICS_MID_MK_HI16(MID) + ICS_ERRNO_MK_COMMNO(n) /* 模块内共有错误码 */

/**
 *@ingroup linux_sme_base Linux
 *描述:用户直接使用的信息码值。(模块ID + 模块内具体信息码 = 完整的信息码)
 *信息ID和问题ID采用统一的编码规则，编码范围分段：
 *1、信息码占用两个字节
 *2、信息ID ＝ 模块ID（高两个字节） ＋ 信息码（低两个字节）
 *3、信息码分段如下:
 *范围说明:
 *0X0000 - 0X2FFF 错误码
 *0X3000 - 0X2FFF 用户直接使用的信息码
 *0X3FFF - 0X7FFF 日志扫描工具生成的信息码
 *0X8000 - 0XFFFF 保留
 *对于产品自行定义的情况下，只要求模块ID的定义保持唯一，信息码的
 *具体定义可以完全由产品自行决定
 */
#define ICS_INFNO_BEGIN                   0X3000
#define ICS_INFID_MAKE(MID, INFNO)        ((MID<<16) + ICS_INFNO_BEGIN + INFNO)

#ifndef ICS_FALSE
#ifdef __cplusplus
#define ICS_FALSE false
#else
#define ICS_FALSE ((V_BOOL)0)
#endif
#endif

#ifndef ICS_TRUE
#ifdef __cplusplus
#define ICS_TRUE true
#else
#define ICS_TRUE ((V_BOOL)1)
#endif
#endif

#ifndef ICS_SUCCESS
#define ICS_SUCCESS 0
#endif

#ifndef ICS_FAIL
#define ICS_FAIL 1
#endif

#ifndef ICS_ERROR
#define ICS_ERROR 0xffffffff
#endif

/**Common Error Code */
#define ICS_OK                     (0)  /* 正确 */

/**
*@ingroup ICS_base
*0x1 操作权限受限。
*/
#define ICS_ERRNO_PERM             (1)  /* Operation not permitted */

/**
*@ingroup sme_base
*0x2 文件或者文件夹不存在。
*/
#define ICS_ERRNO_NOENT            (2)  /* No such file or directory */

/**
*@ingroup sme_base
*0x3 进程或者线程不存在。
*/
#define ICS_ERRNO_SRCH             (3)  /* No such process */

/**
*@ingroup sme_base
*0x4 不能在中断下调用。
*/
#define ICS_ERRNO_INTR             (4)  /* Interrupted system call */

/**
*@ingroup sme_base
*0x5 IO操作失败。
*/
#define ICS_ERRNO_IO               (5)  /* I/O error */

/**
*@ingroup sme_base
*0x6 错误的设备或者地址。
*/
#define ICS_ERRNO_NXIO             (6)  /* No such device or address */

/**
*@ingroup sme_base
*0x7 参数列表过长。
*/
#define ICS_ERRNO_2BIG             (7)  /* Argument list too long */

/**
*@ingroup sme_base
*0x8 可执行文件格式错误。
*/
#define ICS_ERRNO_NOEXEC           (8)  /* ICS_ERRNO_xec format error */

/**
*@ingroup sme_base
*0x9 文件数量错误。
*/
#define ICS_ERRNO_BADF             (9)  /* Bad file number */

/**
*@ingroup sme_base
*0xA 子进程不存在。
*/
#define ICS_ERRNO_CHILD           (10)  /* No child processes */

/**
*@ingroup sme_base
*0xB 请稍后重试。
*/
#define ICS_ERRNO_AGAIN           (11)  /* Try again */

/**
*@ingroup sme_base
*0xC 内存不足。
*/
#define ICS_ERRNO_NOMEM           (12)  /* Out of memory */

/**
*@ingroup sme_base
*0xD 权限受限。
*/
#define ICS_ERRNO_ACCES           (13)  /* Permission denied */

/**
*@ingroup sme_base
*0xE 地址错误。
*/
#define ICS_ERRNO_FAULT           (14)  /* Bad address */

/**
*@ingroup sme_base
*0xF 块设备请求失败。
*/
#define ICS_ERRNO_NOTBLK          (15)  /* Block device required */

/**
*@ingroup sme_base
*0x10 设备或者资源繁忙。
*/
#define ICS_ERRNO_BUSY            (16)  /* Device or resource busy */

/**
*@ingroup sme_base
*0x11 文件已经存在。
*/
#define ICS_ERRNO_EXIST           (17)  /* File exists */

/**
*@ingroup sme_base
*0x12 跨设备连接失败。
*/
#define ICS_ERRNO_XDEV            (18)  /* Cross-device link */

/**
*@ingroup sme_base
*0x13 设备不存在。
*/
#define ICS_ERRNO_NODEV           (19)  /* No such device */

/**
*@ingroup sme_base
*0x14 非目录文件。
*/
#define ICS_ERRNO_NOTDIR          (20)  /* Not a directory */

/**
*@ingroup sme_base
*0x15 目录文件。
*/
#define ICS_ERRNO_ISDIR           (21)  /* Is a directory */

/**
*@ingroup sme_base
*0x16 参数错误。
*/
#define ICS_ERRNO_INVAL           (22)  /* Invalid argument */

/**
*@ingroup sme_base
*0x17 文件列表溢出。
*/
#define ICS_ERRNO_NFILE           (23)  /* File table overflow */

/**
*@ingroup sme_base
*0x18 打开文件过多。
*/
#define ICS_ERRNO_MFILE           (24)  /* Too many open files */

/**
*@ingroup sme_base
*0x19 非打印设备。
*/
#define ICS_ERRNO_NOTTY           (25)  /* Not a typewriter */

/**
*@ingroup sme_base
*0x1A 文本文件繁忙。
*/
#define ICS_ERRNO_TXTBSY          (26)  /* Text file busy */

/**
*@ingroup sme_base
*0x1B 文件过大。
*/
#define ICS_ERRNO_FBIG            (27)  /* File too large */

/**
*@ingroup sme_base
*0x1C 设备没有足够的空间。
*/
#define ICS_ERRNO_NOSPC           (28)  /* No space left on device */

/**
*@ingroup sme_base
*0x1D 非法请求。
*/
#define ICS_ERRNO_SPIPE           (29)  /* Illegal seek */

/**
*@ingroup sme_base
*0x1E 只读文件系统。
*/
#define ICS_ERRNO_ROFS            (30)  /* Read-only file system */

/**
*@ingroup sme_base
*0x1F 连接过多。
*/
#define ICS_ERRNO_MLINK           (31)  /* Too many links */

/**
*@ingroup sme_base
*0x20 管道断开。
*/
#define ICS_ERRNO_PIPE            (32)  /* Broken pipe */

/**
*@ingroup sme_base
*0x21 变量超出范围。
*/
#define ICS_ERRNO_DOM             (33)  /* Math argument out of domain of func */

/**
*@ingroup sme_base
*0x22 数学计算结果超出描述范围。
*/
#define ICS_ERRNO_RANGE           (34)  /* Math result not representable */

/* Posix defines these to be the same */
/**
*@ingroup sme_base
*0x23 将会发生资源死锁。
*/
#define ICS_ERRNO_DEADLK          (35)             /* Resource deadlock would occur */

/**
*@ingroup sme_base
*0x23 将会发生资源死锁。
*/
#define ICS_ERRNO_DEADLOCK        ICS_ERRNO_DEADLK /* Resource deadlock would occur */

/**
*@ingroup sme_base
*0x24 文件名过长。
*/
#define ICS_ERRNO_NAMETOOLONG     (36)  /* File name too long */

/**
*@ingroup sme_base
*0x25 无记录锁可用。
*/
#define ICS_ERRNO_NOLCK           (37)  /* No record locks available */

/**
*@ingroup sme_base
*0x26 函数没有实现。
*/
#define ICS_ERRNO_NOSYS           (38)  /* Function not implemented */

/**
*@ingroup sme_base
*0x27 目录不为空。
*/
#define ICS_ERRNO_NOTEMPTY        (39)  /* Directory not empty */

/**
*@ingroup sme_base
*0x28 链接或者调用发生循环。
*/
#define ICS_ERRNO_LOOP            (40)  /* Too many symbolic links encountered */

/* It is not suggested to use unknown error number */
/**
*@ingroup sme_base
*0x29 未知错误码，用户定义的起始错误码。
*/
#define ICS_ERRNO_UNKNOWN         (41)     /* Unkown reason */

/**
*@ingroup sme_base
*0x2A 操作阻止。
*/
#define ICS_ERRNO_WOULDBLOCK      (42)  /* Operation would block */

/**
*@ingroup sme_base
*0x2B 没有期望类型的消息。
*/
#define ICS_ERRNO_NOMSG           (43)  /* No message of desired type */

/**
*@ingroup sme_base
*0x2C 标识符移除。
*/
#define ICS_ERRNO_IDRM            (44)  /* Identifier removed */

/**
*@ingroup sme_base
*0x2D 通道数超过范围。
*/
#define ICS_ERRNO_CHRNG           (45)  /* Channel number out of range */

/**
*@ingroup sme_base
*0x2E 级别2未同步。
*/
#define ICS_ERRNO_L2NSYNC         (46)  /* Level 2 not synchronized */

/**
*@ingroup sme_base
*0x2F 级别3停止。
*/
#define ICS_ERRNO_L3HLT           (47)  /* Level 3 halted */

/**
*@ingroup sme_base
*0x30 级别3重启。
*/
#define ICS_ERRNO_L3RST           (48)  /* Level 3 reset */

/**
*@ingroup sme_base
*0x31 连接数量超过限制。
*/
#define ICS_ERRNO_LNRNG           (49)  /* Link number out of range */

/**
*@ingroup sme_base
*0x32 协议驱动未挂接。
*/
#define ICS_ERRNO_UNATCH          (50)  /* Protocol driver not attached */

/**
*@ingroup sme_base
*0x33没有可用的CSI设备。
*/
#define ICS_ERRNO_NOCSI           (51)  /* No CSI structure available */

/**
*@ingroup sme_base
*0x34 级别2停止。
*/
#define ICS_ERRNO_L2HLT           (52)  /* Level 2 halted */

/**
*@ingroup sme_base
*0x35 调换错误。
*/
#define ICS_ERRNO_BADE            (53)  /* Invalid exchange */

/**
*@ingroup sme_base
*0x36 无效的请求描述符。
*/
#define ICS_ERRNO_BADR            (54)  /* Invalid request descriptor */

/**
*@ingroup sme_base
*0x37 交换区满。
*/
#define ICS_ERRNO_XFULL           (55)  /* ICS_ERRNO_xchange full */

/**
*@ingroup sme_base
*0x38 没有结点。
*/
#define ICS_ERRNO_NOANO           (56)  /* No anode */

/**
*@ingroup sme_base
*0x39 无效的请求码。
*/
#define ICS_ERRNO_BADRQC          (57)  /* Invalid request code */

/**
*@ingroup sme_base
*0x3A 无效的槽。
*/
#define ICS_ERRNO_BADSLT          (58)  /* Invalid slot */

/**
*@ingroup sme_base
*0x3B 坏的字体文件格式。
*/
#define ICS_ERRNO_BFONT           (59)  /* Bad font file format */

/**
*@ingroup sme_base
*0x3C 不是流设备。
*/
#define ICS_ERRNO_NOSTR           (60)  /* Device not a stream */

/**
*@ingroup sme_base
*0x3D 无可用的数据。
*/
#define ICS_ERRNO_NODATA          (61)  /* No data available */

/**
*@ingroup sme_base
*0x3E 定时器停止。
*/
#define ICS_ERRNO_TIME            (62)  /* Timer expired */

/**
*@ingroup sme_base
*0x3F 流资源不足。
*/
#define ICS_ERRNO_NOSR            (63)  /* Out of streams resources */

/**
*@ingroup sme_base
*0x40 机器未连接网络。
*/
#define ICS_ERRNO_NONET           (64)  /* Machine is not on the network */

/**
*@ingroup sme_base
*0x41 包未安装。
*/
#define ICS_ERRNO_NOPKG           (65)  /* Package not installed */

/**
*@ingroup sme_base
*0x42 远程目标。
*/
#define ICS_ERRNO_REMOTE          (66)  /* Object is remote */

/**
*@ingroup sme_base
*0x43 连接中断。
*/
#define ICS_ERRNO_NOLINK          (67)  /* Link has been severed */

/**
*@ingroup sme_base
*0x44 广播失败。
*/
#define ICS_ERRNO_ADV             (68)  /* Advertise error */

/**
*@ingroup sme_base
*0x45 挂载失败。
*/
#define ICS_ERRNO_SRMNT           (69)  /* Srmount error */

/**
*@ingroup sme_base
*0x46 发送失败。
*/
#define ICS_ERRNO_COMM            (70)  /* Communication error on send */

/**
*@ingroup sme_base
*0x47 协议错误。
*/
#define ICS_ERRNO_PROTO           (71)  /* Protocol error */

/**
*@ingroup sme_base
*0x48 下一跳尝试。
*/
#define ICS_ERRNO_MULTIHOP        (72)  /* Multihop attempted */

/**
*@ingroup sme_base
*0x49 远程文件系统错误。
*/
#define ICS_ERRNO_DOTDOT          (73)  /* RFS specific error */

/**
*@ingroup sme_base
*0x4A 消息数据内容无效。
*/
#define ICS_ERRNO_BADMSG          (74)  /* Not a data message */

/**
*@ingroup sme_base
*0x4B 赋值超出变量类型定义取值范围。
*/
#define ICS_ERRNO_OVERFLOW        (75)  /* Value too large for defined data type */

/**
*@ingroup sme_base
*0x4C 网络重名。
*/
#define ICS_ERRNO_NOTUNIQ         (76)  /* Name not unique on network */

/**
*@ingroup sme_base
*0x4D 文件描述符发生错误。
*/
#define ICS_ERRNO_BADFD           (77)  /* File descriptor in bad state */

/**
*@ingroup sme_base
*0x4E 远程地址发生变更。
*/
#define ICS_ERRNO_REMCHG          (78)  /* Remote address changed */

/**
*@ingroup sme_base
*0x4F 无法访问必要的共享库。
*/
#define ICS_ERRNO_LIBACC          (79)  /* Can not access a needed shared library */

/**
*@ingroup sme_base
*0x50 访问损坏的共享库。
*/
#define ICS_ERRNO_LIBBAD          (80)  /* Accessing a corrupted shared library */


/**
*@ingroup sme_base
*0x51 a.out文件中.lib区域遭破坏。
*/
#define ICS_ERRNO_LIBSCN          (81)  /* .lib section in a.out corrupted */

/**
*@ingroup sme_base
*0x52 链接的共享库过多。
*/
#define ICS_ERRNO_LIBMAX          (82)  /* Attempting to link in too many shared libraries */

/**
*@ingroup sme_base
*0x53 不能直接执行共享库。
*/
#define ICS_ERRNO_LIBEXEC         (83)  /* Cannot exec a shared library directly */

/**
*@ingroup sme_base
*0x54 字节序不正确。
*/
#define ICS_ERRNO_ILSEQ           (84)  /* Illegal byte sequence */

/**
*@ingroup sme_base
*0x55 系统调用被打断，应该重新启动。
*/
#define ICS_ERRNO_RESTART         (85)  /* Interrupted system call should be restarted */

/**
*@ingroup sme_base
*0x56 流管道错误。
*/
#define ICS_ERRNO_STRPIPE         (86)  /* Streams pipe error */

/**
*@ingroup sme_base
*0x57 用户过多。
*/
#define ICS_ERRNO_USERS           (87)  /* Too many users */

/**
*@ingroup sme_base
*0x58 没有套接字可供操作。
*/
#define ICS_ERRNO_NOTSOCK         (88)  /* Socket operation on non-socket */

/**
*@ingroup sme_base
*0x59 目的地址请求失败。
*/
#define ICS_ERRNO_DESTADDRREQ     (89)  /* Destination address required */

/**
*@ingroup sme_base
*0x5A 消息过长。
*/
#define ICS_ERRNO_MSGSIZE         (90)  /* Message too long */

/**
*@ingroup sme_base
*0x5B 错误的协议类型。
*/
#define ICS_ERRNO_PROTOTYPE       (91)  /* Protocol wrong type for socket */

/**
*@ingroup sme_base
*0x5C 协议不可用。
*/
#define ICS_ERRNO_NOPROTOOPT      (92)  /* Protocol not available */

/**
*@ingroup sme_base
*0x5D 协议不支持。
*/
#define ICS_ERRNO_PROTONOSUPPORT  (93)  /* Protocol not supported */

/**
*@ingroup sme_base
*0x5E 套接字类型不支持。
*/
#define ICS_ERRNO_SOCKTNOSUPPORT  (94)  /* Socket type not supported */

/**
*@ingroup sme_base
*0x5F 传输端不支持此操作。
*/
#define ICS_ERRNO_OPNOTSUPP       (95)  /* Operation not supported on transport endpoint */

/**
*@ingroup sme_base
*0x60 协议族不支持。
*/
#define ICS_ERRNO_PFNOSUPPORT     (96)  /* Protocol family not supported */

/**
*@ingroup sme_base
*0x61 地址不能访问。
*/
#define ICS_ERRNO_AFNOSUPPORT     (97)  /* Address family not supported by protocol */

/**
*@ingroup sme_base
*0x62 地址已经使用。
*/
#define ICS_ERRNO_ADDRINUSE       (98)  /* Address already in use */

/**
*@ingroup sme_base
*0x63 申请地址请求失败。
*/
#define ICS_ERRNO_ADDRNOTAVAIL    (99)  /* Cannot assign requested address */

/**
*@ingroup sme_base
*0x64 网络关闭。
*/
#define ICS_ERRNO_NETDOWN         (100) /* Network is down */

/**
*@ingroup sme_base
*0x65 网络不可达。
*/
#define ICS_ERRNO_NETUNREACH      (101) /* Network is unreachable */

/**
*@ingroup sme_base
*0x66 复位导致网络连接断开。
*/
#define ICS_ERRNO_NETRESET        (102) /* Network dropped connection because of reset */

/**
*@ingroup sme_base
*0x67 软件原因导致连接终止。
*/
#define ICS_ERRNO_CONNABORTED     (103) /* Software caused connection abort */

/**
*@ingroup sme_base
*0x68 同级重置连接。
*/
#define ICS_ERRNO_CONNRESET       (104) /* Connection reset by peer */

/**
*@ingroup sme_base
*0x69 没有可用的缓存。
*/
#define ICS_ERRNO_NOBUFS          (105) /* No buffer space available */

/**
*@ingroup sme_base
*0x6A 传输端点已经建立连接。
*/
#define ICS_ERRNO_ISCONN          (106) /* Transport endpoint is already connected */

/**
*@ingroup sme_base
*0x6B 传输端点没有建立连接。
*/
#define ICS_ERRNO_NOTCONN         (107) /* Transport endpoint is not connected */

/**
*@ingroup sme_base
*0x6C 传输端关闭后无法发送。
*/
#define ICS_ERRNO_SHUTDOWN        (108) /* Cannot send after transport endpoint shutdown */

/**
*@ingroup sme_base
*0x6D 引用过多无法分片。
*/
#define ICS_ERRNO_TOOMANYREFS     (109) /* Too many references: cannot splice */

/**
*@ingroup sme_base
*0x6E 连接超时。
*/
#define ICS_ERRNO_TIMEDOUT        (110) /* Connection timed out */

/**
*@ingroup sme_base
*0x6F 拒绝建立连接。
*/
#define ICS_ERRNO_CONNREFUSED     (111) /* Connection refused */

/**
*@ingroup sme_base
*0x70 主机关机。
*/
#define ICS_ERRNO_HOSTDOWN        (112) /* Host is down */

/**
*@ingroup sme_base
*0x71 与主机没有路由。
*/
#define ICS_ERRNO_HOSTUNREACH     (113) /* No route to host */

/**
*@ingroup sme_base
*0x72 操作就绪。
*/
#define ICS_ERRNO_ALREADY         (114) /* Operation already in progress */

/**
*@ingroup sme_base
*0x73 操作进行中。
*/
#define ICS_ERRNO_INPROGRESS      (115) /* Operation now in progress */

/**
*@ingroup sme_base
*0x74 旧的NFS文件句柄。
*/
#define ICS_ERRNO_STALE           (116) /* Stale NFS file handle */

/**
*@ingroup sme_base
*0x75 结构需要清理。
*/
#define ICS_ERRNO_UCLEAN          (117) /* Structure needs cleaning */

/**
*@ingroup sme_base
*0x76 非XENIX命名类型文件。
*/
#define ICS_ERRNO_NOTNAM          (118) /* Not a XENIX named type file */

/**
*@ingroup sme_base
*0x77 XENIX信号量资源耗尽。
*/
#define ICS_ERRNO_NAVAIL          (119) /* No XENIX semaphores available */

/**
*@ingroup sme_base
*0x78 一个命名类型的文件。
*/
#define ICS_ERRNO_ISNAM           (120) /* Is a named type file */

/**
*@ingroup sme_base
*0x79 远程IO操作失败。
*/
#define ICS_ERRNO_REMOTEIO        (121) /* Remote I/O error */

/**
*@ingroup sme_base
*0x7A 超过配额限制。
*/
#define ICS_ERRNO_DQUOT           (122) /* Quota exceeded */

/**
*@ingroup sme_base
*0x7B 介质类型不存在。
*/
#define ICS_ERRNO_NOMEDIUM        (123) /* No medium found */

/**
*@ingroup sme_base
*0x7C 介质类型错误。
*/
#define ICS_ERRNO_MEDIUMTYPE      (124) /* Wrong medium type */

/**
*@ingroup sme_base
*0x7D 模块没有初始化。
*/
#define ICS_ERRNO_NOTINIT         (125) /* module not init */

/**
*@ingroup sme_base
*0x7E 套接字句柄异常或失效。
*/
#define ICS_ERRNO_INVASOCHANDLE         (126) /* socket handle invalid or exception */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __ICS_ERRNO_H__ */
