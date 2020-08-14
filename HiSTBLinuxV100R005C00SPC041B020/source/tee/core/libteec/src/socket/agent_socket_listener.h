#ifndef __AGENT_SOCKET_LISTENER_H_
#define __AGENT_SOCKET_LISTENER_H_

#ifndef TEE_SOCKET_DEBUG_ON_SERVER
	typedef TEEC_UUID uuid;
#endif
#include "tee_socket_agent_type.h"

/* socket id for agent */
#define AGENT_SOCKET_ID 0x69e85664

/* for smc device */
extern int g_socket_agent_fd;

/* for socket listener init */
int socket_agent_init();
/* for socket listener exit */
int socket_agent_exit();
/* for socket listener work */
void *thread_socket_work(void *ctrl);

#endif /* __AGENT_SOCKET_LISTENER_H_ */
