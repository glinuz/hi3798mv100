#ifndef __TEE_SOCKET_AGENT_TYPE_H_
#define __TEE_SOCKET_AGENT_TYPE_H_

#ifdef TEE_SOCKET_DEBUG_ON_SERVER
typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEE_UUID;
typedef TEE_UUID uuid;
#endif

typedef enum {
	sec_socket_gethostip,
	sec_socket_open,
	sec_socket_close,
	sec_socket_send,
	sec_socket_write,
	sec_socket_recv,
	sec_socket_read,
	sec_socket_ioctl,
} socket_cmd_t;

/*
 * If protocol is TCP:
 *  Returns the amount of queued unread data in the receive buffer.
 * If protocal is UDP:
 *  Returns the size of the next pending datagram in the integer in bytes.
 */
#define SOCKET_GET_AMOUNT_OF_UNREAD_DATA 0x11

#define SERVER_ADDR_MAX_LEN 50
struct socket_gethostip {
	uint8_t host_ip[SERVER_ADDR_MAX_LEN];
	uint32_t domain;
	uint32_t hostname_len;
	uint32_t hostname[1];
};
struct socket_open {
	uint32_t domain; /* the communication domain */
	uint32_t protocol; /* the particular protocol to be used with the socket */
	uint8_t server_addr[SERVER_ADDR_MAX_LEN]; /* the server ip address */
	uint32_t server_port; /* the server port number */
};
struct socket_close {
	int32_t sockfd;
};
struct socket_msg {
	uint32_t last_frame; /* for recv */
	uint32_t length_whole; /* for send */
	uint32_t length_part; /* for send.recv */
	uint32_t buffer[1]; /* for used.recv */
};
struct socket_msg2 {
	int32_t sockfd; /* for send.recv */
	uint32_t timeout; /* for send.recv */
	uint32_t protocol; /* TCP or UDP */
	struct socket_open server; /* for UDP send.recv */
	uint32_t length; /* for recv */
	uint32_t cmd; /* for ioctl */
};
struct socket_ioctl {
	int32_t sockfd;
	uint32_t protocol; /* TCP or UDP */
	uint32_t cmd;
};

/*
 * ret: the result of cmd
 * ret2: if ret is success, ret2 is real result. for example, open success, ret2 is sockfd
 * err: if ret is failed, err is errno
 */
struct socket_ctrl_t {
	socket_cmd_t cmd;
	uint32_t ret;
	uint32_t ret2;
	uint32_t err;
	uint32_t magic;
	uuid service_id;
	uint32_t session_id;
	union _args {
		struct socket_gethostip gethostip;
		struct socket_open open;
		struct socket_close close;
		struct socket_msg send;
		struct socket_msg2 write;
		struct socket_msg recv;
		struct socket_msg2 read;
		struct socket_ioctl ioctl;
	} args;
};

enum Socket_Agent_Errno {
	AGENT_CMD_SUCCESS = 0xFF0000,
	AGENT_GENERIC_MSG_CONTINUE,
	AGENT_GENERIC_OUT_OF_MEMORY,
	AGENT_CMD_NOT_SUPPORT,
	AGENT_CMD_GETHOSTIP_ERR,
	AGENT_CMD_OPEN_DOMAIN_ERR,
	AGENT_CMD_OPEN_PROTOCOL_ERR,
	AGENT_CMD_OPEN_SOCKET_FAIL,
	AGENT_CMD_OPEN_INET_FAIL,
	AGENT_CMD_OPEN_CONNECT_FAIL,
	AGENT_CMD_CLOSE_FAIL,
	AGENT_CMD_SEND_FAIL,
	AGENT_CMD_RECV_FAIL,
	AGENT_CMD_IOCTL_FAIL,
};

#endif /* __TEE_SOCKET_AGENT_TYPE_H_ */
