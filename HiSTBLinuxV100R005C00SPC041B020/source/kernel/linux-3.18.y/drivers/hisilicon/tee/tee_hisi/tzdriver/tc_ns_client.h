/*
 * FileName:
 * Author:         h00211444  Version: 0.0.1  Date: 2012-12-25
 * Description:
 * Version:
 * Function List:
 *                 1.
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#ifndef _TC_NS_CLIENT_H_
#define _TC_NS_CLIENT_H_

typedef struct {
	__u32 method;
	__u32 mdata;
} TC_NS_ClientLogin;

typedef union {
	struct {
		__u64 buffer;
		__u64 offset;
		__u64 size_addr;
	} memref;
	struct {
#ifdef CONFIG_DEVCHIP_PLATFORM
		/*TODO: fix the value b incorrect bug, by laiyingjun*/
		__u64 a_addr;
		__u64 b_addr;
#else
		__u64 *a_addr;
		__u64 *b_addr;
#endif
	} value;
} TC_NS_ClientParam;

typedef struct {
	__u32 code;
	__u32 origin;
} TC_NS_ClientReturn;

typedef struct {
	unsigned char uuid[16];
	__u32 session_id;
	__u32 cmd_id;
	TC_NS_ClientReturn returns;
	TC_NS_ClientLogin login;
	TC_NS_ClientParam params[4];
	__u32 paramTypes;
	__u8 started;
} TC_NS_ClientContext;

typedef struct {
	uint32_t seconds;
	uint32_t millis;
} TC_NS_Time;

#define	vmalloc_addr_valid(kaddr) \
	(((void *)(kaddr) >= (void *)VMALLOC_START) && \
	((void *)(kaddr) < (void *)VMALLOC_END))
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef CONFIG_IMAX_CIPHER
typedef struct {
	unsigned int cmd_id;
	unsigned int reserved;
	unsigned int operationh;
	unsigned int operationl;
} TC_SRV_ClientContext;
#endif
#endif

#define IMG_LOAD_FIND_NO_DEV_ID  0xFFFF00A5
#define IMG_LOAD_FIND_NO_SHARE_MEM 0xFFFF00A6
#define IMG_LOAD_SECURE_RET_ERROR 0xFFFF00A7

#define TST_CMD_01 (1)
#define TST_CMD_02 (2)
#define TST_CMD_03 (3)
#define TST_CMD_04 (4)
#define TST_CMD_05 (5)

#define MAX_SHA_256_SZ 32

#define TC_NS_CLIENT_IOCTL_SES_OPEN_REQ \
	 _IOW(TC_NS_CLIENT_IOC_MAGIC, 1, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 2, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_SEND_CMD_REQ \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 3, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 4, unsigned int)
#define TC_NS_CLIENT_IOCTL_WAIT_EVENT \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 5, unsigned int)
#define TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 6, unsigned int)
#define TC_NS_CLIENT_IOCTL_REGISTER_AGENT \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 7, unsigned int)
#define TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 8, unsigned int)
#define TC_NS_CLIENT_IOCTL_LOAD_APP_REQ \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 9, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_NEED_LOAD_APP \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 10, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_ALLOC_EXCEPTING_MEM \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 12, unsigned int)
#define TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 13, TC_NS_ClientContext)
#define TC_NS_CLIENT_IOCTL_LOGIN \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 14, int)
#define TC_NS_CLIENT_IOCTL_TST_CMD_REQ \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 15, int)
#define TC_NS_CLIENT_IOCTL_TUI_EVENT \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 16, int)
#define TC_NS_CLIENT_IOCTL_SYC_SYS_TIME \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 17, TC_NS_Time)
#define TC_NS_CLIENT_IOCTL_SET_NATIVE_IDENTITY \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 18, int)
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef CONFIG_IMAX_CIPHER
#define TC_NS_CLIENT_IOCTL_GENERAL_SRV_CMD_SEND \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 151, TC_SRV_ClientContext)
#define TC_NS_CLIENT_IOCTL_GENERAL_SRV \
	_IOWR(TC_NS_CLIENT_IOC_MAGIC, 161, TC_SRV_ClientContext)
#endif
#endif
#endif
