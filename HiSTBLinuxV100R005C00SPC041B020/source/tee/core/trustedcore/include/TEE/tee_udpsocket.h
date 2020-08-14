#ifndef __TEE_UDPSOCKET_H_
#define __TEE_UDPSOCKET_H_

#include "tee_isocket.h"
#include "tee_tcp_udp_socket.h"

/* Protocol identifier */
#define TEE_ISOCKET_PROTOCOLID_UDP 0x66

/* Instance specific errors */
#define TEE_ISOCKET_UDP_ERROR_HOSTNAME  0xF1020001

/* Instance specific ioctl functions */
#define TEE_UDP_CHANGEADDR              0x66000001
#define TEE_UDP_CHANGEPORT              0x66000002

/* declare the client setup struct variables */
typedef struct TEE_udpSocket_Setup_s {
	TEE_ipSocket_ipVersion  ipVersion;
	char                    *server_addr;
	int                     server_port;
} TEE_udpSocket_Setup;

/* declare the function pointer handle */
extern TEE_iSocket *const TEE_udpSocket;

#endif //__TEE_UDPSOCKET_H_
