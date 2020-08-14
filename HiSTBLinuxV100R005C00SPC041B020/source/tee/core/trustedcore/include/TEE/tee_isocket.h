#ifndef __TEE_ISOCKET_H_
#define __TEE_ISOCKET_H_

#include "tee_internal_api.h"

#define TEE_HANDLE_NULL 0x00000000
#define TEE_SOCKET_VERSION 0x01000000
#define TEE_SOCKET_TCP_VERSION TEE_SOCKET_VERSION
#define TEE_SOCKET_UDP_VERSION TEE_SOCKET_VERSION

struct __TEE_iSocketHandle {
	void *dataptr;
};
typedef struct __TEE_iSocketHandle *TEE_iSocketHandle;

typedef const struct TEE_iSocket_s {
	uint32_t TEE_iSocketVersion;
	uint8_t protocolID;

	TEE_Result(*open)(
		TEE_iSocketHandle *ctx,
		void *setup,
		uint32_t *protocolError);

	TEE_Result(*close)(
		TEE_iSocketHandle ctx);

	TEE_Result(*send)(
		TEE_iSocketHandle ctx,
		const void *buf,
		uint32_t *length,
		uint32_t timeout);

	TEE_Result(*recv)(
		TEE_iSocketHandle ctx,
		void *buf,
		uint32_t *length,
		uint32_t timeout);

	uint32_t (*error)(
		TEE_iSocketHandle ctx);

	TEE_Result(*ioctl)(
		TEE_iSocketHandle ctx,
		uint32_t commandCode,
		void *buf,
		uint32_t *length);

} TEE_iSocket;

#endif //__TEE_ISOCKET_H_
