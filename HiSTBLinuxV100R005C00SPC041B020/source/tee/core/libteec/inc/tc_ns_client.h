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
#include "tee_client_type.h"
#define TC_DEBUG

#define TC_NS_CLIENT_IOC_MAGIC	't'
#define TC_NS_CLIENT_DEV			"tc_ns_client"
#define TC_NS_CLIENT_DEV_NAME	"/dev/tc_ns_client"
#define TC_NS_MYDEVICE_NAME "/dev/mydevice_sample"
typedef struct {
	unsigned int method;
	unsigned int mdata;
} TC_NS_ClientLogin;

typedef union {
	struct {
		unsigned int buffer;
		unsigned int buffer_h_addr;
		unsigned int offset;
		unsigned int h_offset;
		unsigned int size_addr;
		unsigned int size_h_addr;
	} memref;
	struct {
		unsigned int a_addr;
		unsigned int a_h_addr;
		unsigned int b_addr;
		unsigned int b_h_addr;
	} value;
} TC_NS_ClientParam;

typedef struct {
	unsigned int code;
	unsigned int origin;
} TC_NS_ClientReturn;

typedef struct {
	unsigned char uuid[16];
	unsigned int session_id;
	unsigned int cmd_id;
	TC_NS_ClientReturn returns;
	TC_NS_ClientLogin login;
	TC_NS_ClientParam params[4];
	unsigned int paramTypes;
	bool started;
} TC_NS_ClientContext;

typedef struct {
	uint32_t seconds;
	uint32_t millis;
} TC_NS_Time;



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
#define TC_NS_CLIENT_IOCTL_LOAD_APP_EXCEPT \
    _IOWR(TC_NS_CLIENT_IOC_MAGIC, 11, unsigned int)
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

TEEC_Result TEEC_CheckOperation(TEEC_Operation * operation);
void TEEC_Encode(TC_NS_ClientContext *cli_context, TEEC_UUID service_id,
			uint32_t session_id, uint32_t cmd_id, TC_NS_ClientLogin cli_login,
			TEEC_Operation *operation);


#endif
