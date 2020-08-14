#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
/* #include "tee_socket_debug.h" */
#include "agent_socket_buffer.h"
#include "agent_socket_listener.h"
#include <securec.h>
#define LOG_NDEBUG 0
#ifdef	LOG_TAG
#undef	LOG_TAG
#define LOG_TAG "teecd"
#endif
#include <cutils/log.h>
#include "tee_client_type.h"
#include "../tc_ns_client.h"
#define LOGE ALOGE

int g_socket_agent_fd = -1;
static struct socket_buffer_pool_t *m_socket_pool;

/* All these macros value from SecureOS */
#define IP_VERSION_4				1
#define IP_VERSION_6				2
#define TEE_ISOCKET_PROTOCOLID_TCP_IP		0x65
#define TEE_ISOCKET_PROTOCOLID_UDP		0x66
#define TEE_ISOCKET_ERROR_REMOTE_CLOSED		0xF1007002
#define TEE_ISOCKET_ERROR_TIMEOUT		0xF1007003
#define TEE_ISOCKET_ERROR_OUT_OF_RESOURCES	0xF1007004
#define TEE_ERROR_COMMUNICATION			0xFFFF000E
#define TEE_TIMEOUT_INFINITE			(0xFFFFFFFF)
/* macros value from SecureOS over */

static void socket_gethostip_work(struct socket_ctrl_t *trans_ctrl)
{
#if 0
	struct hostent *he;
	struct in_addr **addr_list;
	char *hostname;
	char *hostip = NULL;
	errno_t rc = EOK;

	hostname = (char *)trans_ctrl->args.gethostip.hostname;
	if ((he = gethostbyname(hostname)) == NULL) {
		LOGE("gethostbyname %s failed\n", hostname);
		trans_ctrl->ret = AGENT_CMD_GETHOSTIP_ERR;
		return;
	}

	/* h_addrtype: The type of address; always AF_INET or AF_INET6 at present. */
	if (he->h_addrtype == AF_INET) {
		trans_ctrl->args.gethostip.domain = IP_VERSION_4;
	} else if (he->h_addrtype == AF_INET6) {
		trans_ctrl->args.gethostip.domain = IP_VERSION_6;
	} else {
		trans_ctrl->ret = AGENT_CMD_GETHOSTIP_ERR;
		return;
	}

	addr_list = (struct in_addr **)he->h_addr_list;
	hostip = inet_ntoa(*addr_list[0]);
	if (hostip == NULL) {
		trans_ctrl->ret = AGENT_CMD_GETHOSTIP_ERR;
		return;
	}

	rc = memcpy_s(trans_ctrl->args.gethostip.host_ip,
		sizeof(trans_ctrl->args.gethostip.host_ip), hostip, SERVER_ADDR_MAX_LEN);
	if (EOK != rc) {
		trans_ctrl->ret = AGENT_CMD_GETHOSTIP_ERR;
		return;
	}

	LOGV("%s resolved to %s, domain=%d\n", hostname, trans_ctrl->args.gethostip.host_ip,
		trans_ctrl->args.gethostip.domain);

	trans_ctrl->ret = AGENT_CMD_SUCCESS;
#else
	trans_ctrl->ret = AGENT_CMD_NOT_SUPPORT;
#endif
}

static void socket_open_work(struct socket_ctrl_t *trans_ctrl)
{
	int sockfd = -1;
	int domain, type, protocol;
	struct sockaddr_in server_addr;
	struct socket_open *open = &trans_ctrl->args.open;
	errno_t rc = EOK;

	/* verify the communication domain */
	if (open->domain == IP_VERSION_4) {
		domain = AF_INET;
	} else if (open->domain == IP_VERSION_6) {
		domain = AF_INET6;
	} else {
		trans_ctrl->ret = AGENT_CMD_OPEN_DOMAIN_ERR;
		goto cleanup;
	}

	/* verify the communication protocol */
	if (open->protocol == TEE_ISOCKET_PROTOCOLID_TCP_IP) {
		type = SOCK_STREAM;
		protocol = IPPROTO_TCP;
	} else if (open->protocol == TEE_ISOCKET_PROTOCOLID_UDP) {
		type = SOCK_DGRAM;
		protocol = IPPROTO_UDP;
	} else {
		trans_ctrl->ret = AGENT_CMD_OPEN_PROTOCOL_ERR;
		goto cleanup;
	}

	/* 1. create an endpoint for communication */
	if ((sockfd = socket(domain, type, protocol)) == -1) {
		LOGE("socket(%d %d %d) failed: %d\n", domain, type, protocol, errno);
		trans_ctrl->ret = AGENT_CMD_OPEN_SOCKET_FAIL;
		trans_ctrl->err = TEE_ISOCKET_ERROR_OUT_OF_RESOURCES;
		goto cleanup;
	}

	/* UDP dont need to connect server */
	if (protocol == IPPROTO_UDP)
		goto out;

	/* convert address from text to binary form */
	rc = memset_s(&server_addr, sizeof(server_addr), 0, sizeof(server_addr));
	if (EOK != rc) {
		LOGE("memset_s failed %d\n", rc);
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		goto cleanup;
	}

	server_addr.sin_family = (uint16_t)domain; /*lint !e734 */
	server_addr.sin_port = htons((uint16_t)open->server_port); /*lint !e734 */

	if (inet_pton(domain, (char *)open->server_addr, &server_addr.sin_addr) != 1) {
		LOGE("inet_pton failed: af=%d, src=%s\n", domain, open->server_addr);
		trans_ctrl->ret = AGENT_CMD_OPEN_INET_FAIL;
		goto cleanup;
	}

	/* 2. initiate a connection on a socket */
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

		LOGE("connect to server(%s:%d) failed: %d\n",
		     open->server_addr, open->server_port, errno);

		trans_ctrl->ret = AGENT_CMD_OPEN_SOCKET_FAIL;

		if (errno == ENETUNREACH || errno == ECONNREFUSED || errno == EHOSTDOWN)
			trans_ctrl->err = TEE_ERROR_COMMUNICATION;
		else /*  (errno == ETIMEDOUT) */
			trans_ctrl->err = TEE_ISOCKET_ERROR_TIMEOUT;

		goto cleanup;
	}

out:
	trans_ctrl->ret = AGENT_CMD_SUCCESS;
	trans_ctrl->ret2 = (uint32_t)sockfd; /*lint !e732 */
	return;

cleanup:

	if (sockfd != -1)
		close(sockfd);
}

static void socket_close_work(struct socket_ctrl_t *trans_ctrl)
{
	if (close(trans_ctrl->args.close.sockfd) == 0) {
		trans_ctrl->ret = AGENT_CMD_SUCCESS;
	} else {
		trans_ctrl->ret = AGENT_CMD_CLOSE_FAIL;
		LOGE("close socket failed: %d\n", errno);
	}
}

static void socket_send_work(struct socket_ctrl_t *trans_ctrl)
{
	ssize_t ret;
	uint32_t length_whole, length_part;
	TEEC_UUID tmp_uuid;
	uint32_t ssid;
	struct socket_msg *send_msg = &trans_ctrl->args.send;
	errno_t rc = EOK;

	/* Get the parameters from share memory */
	rc = memcpy_s(&tmp_uuid, sizeof(tmp_uuid),
		&trans_ctrl->service_id, sizeof(TEEC_UUID)); /*lint !e838 */
	if (EOK != rc) {
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	ssid = trans_ctrl->session_id;
	length_whole = send_msg->length_whole;
	length_part = send_msg->length_part;

	/*  Store buffer to uuid-pool */
	ret = socket_store_buffer(m_socket_pool, tmp_uuid, ssid,
		(uint8_t *)send_msg->buffer, length_part, length_whole);
	if (ret == 0)
		trans_ctrl->ret = AGENT_CMD_SUCCESS;
	else
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
}

static void socket_write_work(struct socket_ctrl_t *trans_ctrl)
{
	ssize_t ret = -1;
	struct timeval timeout;
	int sockfd;
	uint32_t tee_timeout;
	int flags;
	TEEC_UUID tmp_uuid;
	uint32_t ssid;
	uint8_t *buf = NULL;
	uint32_t buf_len = 0;
	struct sockaddr_in server_addr;
	struct socket_msg2 *msgwrite = &trans_ctrl->args.write;
	errno_t rc = EOK;

	/* Get the parameters from share memory */
	rc = memcpy_s(&tmp_uuid, sizeof(tmp_uuid),
		&trans_ctrl->service_id, sizeof(TEEC_UUID)); /*lint !e838 */
	if (EOK != rc) {
		LOGE("memcpy_s failed: %d\n", rc);
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	ssid = trans_ctrl->session_id;
	sockfd = msgwrite->sockfd;
	tee_timeout = msgwrite->timeout;

	if (tee_timeout != TEE_TIMEOUT_INFINITE) {
		timeout.tv_sec = tee_timeout / 1000; /*lint !e713 */
		timeout.tv_usec = (tee_timeout -
			timeout.tv_sec * 1000) * 1000; /*lint !e737 !e713 */
		ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
		flags = 0;
	} else {
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		flags = MSG_WAITALL;
	}

	if (ret != 0) {
		LOGE("setsockopt failed: %d\n", errno);
		trans_ctrl->ret = AGENT_CMD_RECV_FAIL;
		return;
	}

	/*  Get the buffer from pool */
	buf = socket_get_whole_buffer(m_socket_pool, tmp_uuid, ssid, &buf_len); /*lint !e838 */
	if (NULL == buf) {
		LOGE("socket write work: get whole buffer failed\n");
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	/*  Transmit a message to another socket */
	if (msgwrite->protocol == TEE_ISOCKET_PROTOCOLID_TCP_IP) {
		ret = send(sockfd, buf, buf_len, flags);
	} else if (msgwrite->protocol == TEE_ISOCKET_PROTOCOLID_UDP) {
		rc = memset_s(&server_addr, sizeof(server_addr), 0, sizeof(server_addr));
		if (EOK != rc) {
			LOGE("memset_s failed: %d\n", rc);
			trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
			return;
		}

		if (msgwrite->server.domain == IP_VERSION_4)
			server_addr.sin_family = AF_INET;
		else if (msgwrite->server.domain == IP_VERSION_6)
			server_addr.sin_family = AF_INET6;

		server_addr.sin_port = htons(
			(uint16_t)msgwrite->server.server_port); /*lint !e734 */
		ret = inet_pton(server_addr.sin_family, (char *)msgwrite->server.server_addr,
				&server_addr.sin_addr);
		if (ret != 1) {
			trans_ctrl->ret = AGENT_CMD_SEND_FAIL;
			return;
		}

		ret = sendto(sockfd, buf, buf_len, flags, (struct sockaddr *)&server_addr,
			     sizeof(server_addr));
	}

	/*  Clear the buffer after send */
	socket_clear_buffer(m_socket_pool, tmp_uuid, ssid);

	/*  Update the result */
	if (ret != -1) {
		trans_ctrl->ret = AGENT_CMD_SUCCESS;
		trans_ctrl->ret2 = ret; /*lint !e732 */
	} else {
		LOGE("send data to server failed: %d\n", errno);
		trans_ctrl->ret = AGENT_CMD_SEND_FAIL;

		if (errno == EPIPE || errno == ECONNRESET)
			trans_ctrl->err = TEE_ISOCKET_ERROR_REMOTE_CLOSED;
		else if (errno == EAGAIN || errno == ETIMEDOUT)
			trans_ctrl->err = TEE_ISOCKET_ERROR_TIMEOUT;
		else /*  errno: ECONNREFUSED, ECONNRESET, EHOSTDOWN, ENETDOWN, ENETRESET */
			trans_ctrl->err = TEE_ERROR_COMMUNICATION;
	}
}

static void socket_read_work(struct socket_ctrl_t *trans_ctrl)
{
	ssize_t ret;
	struct timeval timeout;
	int sockfd;
	uint32_t tee_timeout;
	int flags;
	TEEC_UUID tmp_uuid;
	uint32_t ssid;
	uint32_t length_whole;
	uint8_t *buf = NULL;
	uint32_t buf_len = 0;
	struct sockaddr_in server_addr;
	socklen_t server_addr_length = sizeof(server_addr);
	char s_addr[SERVER_ADDR_MAX_LEN];
	uint16_t s_port;
	struct socket_msg2 *msgread = &trans_ctrl->args.read;
	errno_t rc = EOK;

	(void)memset_s(&server_addr, sizeof(server_addr), 0, sizeof(server_addr));

	/* Get the parameters from share memory */
	rc = memcpy_s(&tmp_uuid, sizeof(tmp_uuid),
		&trans_ctrl->service_id, sizeof(TEEC_UUID)); /*lint !e838 */
	if (EOK != rc) {
		LOGE("memcpy_s failed: %d\n", rc);
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	ssid = trans_ctrl->session_id;
	sockfd = msgread->sockfd;
	tee_timeout = msgread->timeout;
	length_whole = msgread->length;

	if (tee_timeout != TEE_TIMEOUT_INFINITE) {
		timeout.tv_sec = tee_timeout / 1000; /*lint !e713 */
		timeout.tv_usec = (tee_timeout -
			timeout.tv_sec * 1000) * 1000; /*lint !e737 !e713 */
		ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		flags = 0;
	} else {
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		flags = MSG_WAITALL;
	}

	if (ret != 0) {
		LOGE("setsockopt failed: %d\n", errno);
		trans_ctrl->ret = AGENT_CMD_RECV_FAIL;
		return;
	}

	/*  Store buffer to uuid-pool */
	ret = socket_store_buffer(m_socket_pool,
				  tmp_uuid, ssid,
				  NULL, 0, length_whole);

	/*  Get the buffer from pool */
	buf = socket_get_whole_buffer(m_socket_pool, tmp_uuid, ssid, &buf_len); /*lint !e838 */
	if (NULL == buf) {
		LOGE("setsockopt failed: socket_get_whole_buffer ret null\n");
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	/*  Receive messages from a socket */
	if (msgread->protocol == TEE_ISOCKET_PROTOCOLID_TCP_IP) {
		ret = recv(sockfd, buf, buf_len, flags);
	} else if (msgread->protocol == TEE_ISOCKET_PROTOCOLID_UDP) {
		ret = recvfrom(sockfd, buf, buf_len, flags, (struct sockaddr *)&server_addr,
			       &server_addr_length);
	}

	if (ret == -1 || ret == 0) {
		LOGE("recv data from server failed: %d\n", errno);
		trans_ctrl->ret = AGENT_CMD_RECV_FAIL;

		if (errno == EPIPE || errno == ECONNRESET)
			trans_ctrl->err = TEE_ISOCKET_ERROR_REMOTE_CLOSED;
		else if (errno == EAGAIN || errno == ETIMEDOUT)
			trans_ctrl->err = TEE_ISOCKET_ERROR_TIMEOUT;
		else /*  errno: ECONNREFUSED, ECONNRESET, EHOSTDOWN, ENETDOWN, ENETRESET */
			trans_ctrl->err = TEE_ERROR_COMMUNICATION;

		/*  Clear the buffer */
		socket_clear_buffer(m_socket_pool, tmp_uuid, ssid);
	} else {
		trans_ctrl->ret = AGENT_CMD_SUCCESS;
		trans_ctrl->ret2 = ret; /*lint !e732 */

		if (msgread->protocol == TEE_ISOCKET_PROTOCOLID_UDP) {
			inet_ntop(server_addr.sin_family, &server_addr.sin_addr, s_addr,
				sizeof(s_addr));
			s_port = ntohs((uint16_t)server_addr.sin_port); /*lint !e734 */
			rc = memcpy_s(msgread->server.server_addr,
					sizeof(msgread->server.server_addr),
					s_addr, sizeof(msgread->server.server_addr));
			if (EOK != rc) {
				LOGE("memcpy_s failed: %d\n", rc);
				trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
				return;
			}

			msgread->server.server_port = (uint32_t)s_port; /*lint !e732 */
		}

		socket_update_buffer_length(m_socket_pool, tmp_uuid, ssid, ret); /*lint !e732 */
	}
}

static void socket_recv_work(struct socket_ctrl_t *trans_ctrl)
{
	TEEC_UUID tmp_uuid;
	uint32_t ssid;
	uint8_t *buf = NULL;
	uint32_t buf_len = 0;
	uint32_t last_frm;
	struct socket_msg *tmp_recv = &trans_ctrl->args.recv;
	errno_t rc = EOK;

	/* Get the parameters from share memory */
	rc = memcpy_s(&tmp_uuid, sizeof(tmp_uuid),
		&trans_ctrl->service_id, sizeof(TEEC_UUID)); /*lint !e838 */
	if (EOK != rc) {
		LOGE("memcpy_s failed: %d\n", rc);
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	ssid = trans_ctrl->session_id;
	buf_len = tmp_recv->length_part; /*lint !e838 */
	last_frm = tmp_recv->last_frame;

	/*  Get the buffer from pool */
	buf = socket_get_part_buffer(m_socket_pool, tmp_uuid, ssid, &buf_len); /*lint !e838 */
	if (NULL == buf) {
		LOGE("socket_get_part_buffer failed:\n");
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}
	rc = memcpy_s(tmp_recv->buffer, tmp_recv->length_part, buf, buf_len);
	if (EOK != rc) {
		LOGE("memcpy_s failed: %d\n", rc);
		trans_ctrl->ret = AGENT_GENERIC_OUT_OF_MEMORY;
		return;
	}

	trans_ctrl->ret = AGENT_CMD_SUCCESS;

	/*  Clear the buffer receive all data */
	if (last_frm)
		socket_clear_buffer(m_socket_pool, tmp_uuid, ssid);
}

static void socket_ioctl_work(struct socket_ctrl_t *trans_ctrl)
{
	int32_t sockfd, value = 0;
	uint32_t cmd;
	int ret = -1;

	/* Get the parameters from share memory */
	sockfd = trans_ctrl->args.ioctl.sockfd;
	cmd = trans_ctrl->args.ioctl.cmd;

	if (cmd == SOCKET_GET_AMOUNT_OF_UNREAD_DATA) {
		ret = ioctl(sockfd, FIONREAD, &value);
		trans_ctrl->ret2 = (uint32_t)value; /*lint !e732 */
	}

	if (ret < 0)
		trans_ctrl->ret = AGENT_CMD_IOCTL_FAIL;
	else
		trans_ctrl->ret = AGENT_CMD_SUCCESS;
}

struct socket_work_table {
	socket_cmd_t cmd;
	void (*socket_work)(struct socket_ctrl_t *trans_ctrl);
};
static struct socket_work_table socket_work_t[] = {
	{sec_socket_gethostip,	socket_gethostip_work},
	{sec_socket_open,	socket_open_work},
	{sec_socket_close,	socket_close_work},
	{sec_socket_send,	socket_send_work},
	{sec_socket_write,	socket_write_work},
	{sec_socket_recv,	socket_recv_work},
	{sec_socket_read,	socket_read_work},
	{sec_socket_ioctl,	socket_ioctl_work},
};
#define number_of_cmd(x) (sizeof(x)/sizeof(x[0]))

void *thread_socket_work(void *ctrl)
{
	struct socket_ctrl_t *trans_ctrl = (struct socket_ctrl_t *)ctrl;
	int ret;
	int found = 0;
	uint32_t i;

	if (NULL == trans_ctrl) {
		return NULL;
	}

#ifdef AGENT_SOCKET_ENABLE

	while (1) { /*lint !e716 */

		ret = ioctl(g_socket_agent_fd,
			(int)TC_NS_CLIENT_IOCTL_WAIT_EVENT, AGENT_SOCKET_ID); /*lint !e835 */
		if (ret) {
			LOGE("socket agent wait event failed\n");
			break;
		}

#endif

		for (i = 0; i < number_of_cmd(socket_work_t); i++) {
			if (trans_ctrl->cmd == socket_work_t[i].cmd) {
				found = 1;
				socket_work_t[i].socket_work(ctrl);
			}
		}

		if (found == 0)
			trans_ctrl->ret = AGENT_CMD_NOT_SUPPORT;

#ifdef AGENT_SOCKET_ENABLE
		trans_ctrl->magic = AGENT_SOCKET_ID;
		ret = ioctl(g_socket_agent_fd,
			(int)TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE, /*lint !e835 */
			AGENT_SOCKET_ID);
		if (ret) {
			LOGE("socket agent send reponse failed\n");
			break;
		}

	}

#endif

	return NULL;
}

int socket_agent_init()
{
	m_socket_pool = socket_pool_init();

	if (m_socket_pool)
		return 0;
	else
		return -1;
}
int socket_agent_exit()
{
	if (m_socket_pool) {
		/* TODO: free the socket pool */
	}

	return 0;
}
