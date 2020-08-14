#ifndef __TEE_SOCKET_BOTH_H_
#define __TEE_SOCKET_BOTH_H_

typedef enum TEE_ipSocket_ipVersion_e {
	TEE_IP_VERSION_DC = 0, /* don't care */
	TEE_IP_VERSION_4  = 1,
	TEE_IP_VERSION_6  = 2,
	TEE_IP_VERSION_MAX = 0xFFFFFFFF
} TEE_ipSocket_ipVersion;

#endif //__TEE_SOCKET_BOTH_H_
