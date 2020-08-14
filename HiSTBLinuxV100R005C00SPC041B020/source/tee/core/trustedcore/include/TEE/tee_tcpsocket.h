#ifndef __TEE_TCPSOCKET_H_
#define __TEE_TCPSOCKET_H_

#include "tee_isocket.h"
#include "tee_tcp_udp_socket.h"

/* Protocol identifier */
#define TEE_ISOCKET_PROTOCOLID_TCP_IP 0x65

/* Instance specific errors */
#define TEE_ISOCKET_TCP_ERROR_HOSTNAME  0xF1010001

/* declare the client setup struct variables */
typedef struct TEE_tcpSocket_Setup_s {
	TEE_ipSocket_ipVersion  ipVersion;
	char                    *server_addr;
	int                     server_port;
} TEE_tcpSocket_Setup;

/* declare the function pointer handle */
extern TEE_iSocket *const TEE_tcpSocket;

#endif //__TEE_TCPSOCKET_H_
