/*
 * FileName:
 * Author:         l00202565  Version: 0.0.1  Date: 2015-8-21
 * Description:
 * Version:
 * Function List:
 *                 1.
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> /*for errno*/
#include <fcntl.h>
#include <sys/types.h> /*for open close*/
#include <sys/stat.h>
#include <sys/ioctl.h> /*for ioctl*/
#include <sys/mman.h> /*for mmap*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <signal.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <pthread.h>

//debug switch
#define LOG_NDEBUG 0
#define LOG_TAG "teecd"

#define BUF_MAX_SIZE 4096

#include <cutils/log.h>
#include <cutils/sockets.h>
#include <cutils/properties.h>

#define TEE_MULTIUSER_SOCKET_NAME "tee-multi-user"

typedef struct {
	int magic;
	int status;
	int userid;
	int reserved;
} recv_msg_t;

void main(int argc, char *argv[])
{
	int len, ret, fd;
	recv_msg_t recvbuf;
	int mFrameworkSock;
	int mAcceptedConnection;
	struct sockaddr_un addr;

	if (argc < 2)
		return;

	ALOGI("start send framework multi-user notify on socket %s", TEE_MULTIUSER_SOCKET_NAME);

	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		ALOGE("Failed to open socket '%s': %s\n", TEE_MULTIUSER_SOCKET_NAME, strerror(errno));
		return;
	}

	memset(&addr, 0 , sizeof(addr));
	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), ANDROID_SOCKET_DIR"/%s",
		 TEE_MULTIUSER_SOCKET_NAME);

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		ALOGE("connect erro\n");
		return;
	}

	recvbuf.magic = 0xff00ff00;
	recvbuf.status = 0x01;
	recvbuf.userid = atoi(argv[1]);
	recvbuf.reserved = 0x12002001;
	ALOGI("connect ok, now send framework multi-user notify on socket %s", TEE_MULTIUSER_SOCKET_NAME);
	ret = send(fd, &recvbuf, sizeof(recv_msg_t), 0);
	if (ret < 0) {
		ALOGE("send erro\n");
		return;
	}

	close(fd);
	return;
}
