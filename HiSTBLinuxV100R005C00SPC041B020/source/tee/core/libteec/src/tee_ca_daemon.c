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
#ifndef WITH_DEVCHIP_PLATFORM
#include <private/android_filesystem_config.h>
#endif
#include <signal.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <pthread.h>
#ifndef WITH_DEVCHIP_PLATFORM
#include <openssl/sha.h>
#endif

#ifdef _ANDROID_PLATFORM_HISI
#include <cutils/log.h>
#include <cutils/properties.h>
#endif

#include "tc_ns_client.h"
#include "tee_ca_daemon.h"
#include "tee_client_type.h"
#include "tee_get_app_cert.h"
#include "securec.h"
#include "tcu_authentication.h"
#include "tee_log.h"

/*debug switch*/
#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif
/*#define LOG_NDEBUG 0 */

#ifdef  LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "teecd"


#ifdef WITH_DEVCHIP_PLATFORM
#define HASH_FILE_MAX_SIZE 8192
#define SIGNED_LENGTH      32
#define UUID_LENGTH      16
#endif

#define BACKLOG_LEN 10
/*lint -e578 */
static int send_file_descriptor(int socket, int fd_to_send)
{
	struct msghdr message;
	struct iovec iov[1];
	struct cmsghdr *control_message;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	errno_t ret;

	ret = memset_s(&message, sizeof(struct msghdr), 0, sizeof(struct msghdr));
	if (ret) {
		tloge("memset failed!\n");
		return ret;
	}

	ret = memset_s(ctrl_buf, CMSG_SPACE(sizeof(int)), 0, CMSG_SPACE(sizeof(int)));
	if (ret) {
		tloge("memset failed!\n");
		return ret;
	}

	/* We are passing at least one byte of data so that recvmsg() will not return 0 */
	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_iov = iov;
	message.msg_iovlen = 1;
	message.msg_controllen =  CMSG_SPACE(sizeof(int));
	message.msg_control = ctrl_buf;

	control_message = CMSG_FIRSTHDR(&message);
	if (control_message) {
		control_message->cmsg_level = SOL_SOCKET;
		control_message->cmsg_type = SCM_RIGHTS;
		control_message->cmsg_len = CMSG_LEN(sizeof(int));
		/*lint -e826 */
		*((int *) CMSG_DATA(control_message)) = fd_to_send;
	}

	return sendmsg(socket, &message, 0);
}

#define __le8  unsigned char
#define MAX_PATH_LENGTH	256

void tee_get_ca_name(unsigned long pid, char *ca_name, unsigned int len) {
	FILE *fd;
	int ret;

	(void)snprintf_s(ca_name, len, len, "/proc/%ld/cmdline", pid);
	fd = fopen(ca_name, "r");
	if(NULL == fd) {
	    tloge("fopen[%s] is error\n", ca_name);
	    return;
	}

	memset_s(ca_name, len, 0x00, len);
	if (ret) {
		tloge("memset failed!\n");
		return;
	}
	(void)fread(ca_name, 1, len, fd);
	fclose(fd);
	fd = NULL;
}

int tee_check_ca_cert(int type, unsigned char *block, unsigned int bufl,
			  unsigned long uid, unsigned long pid)
{
	(void)block;
	(void)bufl;
#ifdef WITH_DEVCHIP_PLATFORM
    #define TA_COUNTER_NATIVE 55
    const static __le8 *array_cert_native[TA_COUNTER_NATIVE] = {
        "widevine_ce_cdm_unittest 0",
        "tee_test_agent 0",
        "tee_test_CA 0",
        "tee_test_cancellation 0",
        "tee_test_cipher 0",
        "tee_test_elfload 0",
        "tee_test_hdcp_storekey 0",
        "tee_test_hdcp_write 0",
        "tee_test_login 0",
        "tee_test_random 0",
        "tee_test_random 0",
        "tee_test_hwi_ipc 0",
        "tee_test_invoke 0",
        "tee_test_mem 0",
        "tee_test_secboot 0",
        "tee_test_secure_timer 0",
        "tee_test_sess 0",
        "tee_test_stability 0",
        "/bin/tee_test_storage 0",
        "tee_test_ut 0",
        "tee_test_time 0",
		"tee_test_main 0",
		"teec_permctrl 0",
        "teec_hello 0",
        "teec_permctrl 0",
        "hisi_keymaster_test 0",
        "hisi_gatekeeper_test 0",
        "hisi_demo_client 0",
        "./tee_test_cipher 0",
        "tee_test_beidou 0",
	    "./st_cipher 0",
        "./st_mmz 0",
	    "./st_demux 0",
	    "./sample_tee_tsplay 0",
	    "./sample_tee_tsplay_pid 0",
        "sample_omxvdec 0",
        "sample_esplay 0",
        "sample_rawplay 0",
		"hisi_drv_hdmi 0",
        "/system/bin/mediaserver 0",
        "/system/bin/mediaserver 1013",
        "/system/bin/drmserver 0",
        "/system/bin/drmserver 1019",
        "sample_keybox 0",
        "sample_checkkeybox 0",
        "test_modularwv_hal 0",
        "oemcrypto_test 0",
        "sample_playreadypk_play 0",
        "sample_playreadypk_provision 0",
        "/system/bin/mediadrmserver 0",
        "/system/bin/mediadrmserver 1013",
        "./sample_pvr_tee_rec 0",
        "./sample_pvr_tee_play 0",
        "./sample_tee_esplay 0",
        "./st_avplay 0",
    };
#else
#ifdef DEF_ENG
	#define TA_COUNTER_NATIVE (42)
#else
	#define TA_COUNTER_NATIVE (22)
#endif
	static const __le8 array_cert_native[TA_COUNTER_NATIVE][] = {
	};
#endif
	int rc = 0;
	int ret;
	int index = 0;
#ifndef WITH_DEVCHIP_PLATFORM
	__le8 buf[SHA256_DIGEST_LENGTH];

	ret = memset_s(buf, SHA256_DIGEST_LENGTH, 0, SHA256_DIGEST_LENGTH);
	if (ret) {
		tloge("memset failed!\n");
		return ret;
	}
#endif
	char path[MAX_PATH_LENGTH] = {0};
	__le8 ca_cert[BUF_MAX_SIZE] = {0};
	FILE *fd;
#ifndef WITH_DEVCHIP_PLATFORM
	SHA256_CTX c;
	(void)SHA256_Init(&c);
#endif
	if (CA_CERT_NATIVE == type) {
		ret = snprintf_s(path, MAX_PATH_LENGTH - 1, MAX_PATH_LENGTH - 1, "/proc/%ld/cmdline", pid);
		if (-1 == ret) {
			tloge("snprintf_s failed!\n");
			return ret;
		}
		fd = fopen(path, "r");
		if (NULL == fd) {
			tloge("fopen is error\n");
			return -1;
		}

		ret = memset_s(path, MAX_PATH_LENGTH, 0x00, MAX_PATH_LENGTH);
		if (ret) {
			tloge("memset failed!\n");
			fclose(fd);
			return ret;
		}

		(void)fread(path, 1, MAX_PATH_LENGTH - 1, fd);
		fclose(fd);

		ret = memset_s(ca_cert, BUF_MAX_SIZE, 0x00, BUF_MAX_SIZE);
		if (ret) {
			tloge("memset failed!\n");
			return ret;
		}
#ifdef WITH_DEVCHIP_PLATFORM
		ret = snprintf_s((char *)ca_cert, BUF_MAX_SIZE - 1,  BUF_MAX_SIZE - 1, "%s %ld", path, uid);
		if (-1 == ret) {
			tloge("snprintf_s failed!\n");
			return ret;
		}
		for(index=0; index < TA_COUNTER_NATIVE; index++) {
			ret = memcmp(ca_cert, array_cert_native[index], strlen(array_cert_native[index]));
			if(0 == ret) {
				break;
			}
		}
#else
		ret = snprintf_s((char *)ca_cert, BUF_MAX_SIZE - 1,  BUF_MAX_SIZE - 1, "%s%ld", path, uid);
		if (-1 == ret) {
                        tloge("snprintf_s failed!\n");
                        return ret;
                }
		/*here we not check the ret value according to boringssl*/
		(void)SHA256_Update(&c, ca_cert, BUF_MAX_SIZE);

		rc = SHA256_Final(buf, &c);
		if (rc == 0) {
			tloge("failed to SHA256_Final, rc 0x%x\n", rc);
			return -1;
		}

		for (index = 0; index < TA_COUNTER_NATIVE; index++) {
			ret = memcmp(buf,array_cert_native[index],
						 sizeof(__le8) * SHA256_DIGEST_LENGTH);
			if (0 == ret) {
				break;
			}
		}
#endif
	} else if (CA_CERT_APK == type) {
		ret = 0;
	}
	return ret;
}

void *thread_ca_server_work(void *dummy)
{
	struct ucred cr;
	int ret, t;
	int s, s2;
	uint8_t *buf = NULL;
	unsigned int bufl;
	uint32_t type = 0;
	char ca_name[MAX_PATH_LENGTH] = {0};

	(void)dummy;
	/* Open a socket (a UNIX domain stream socket)*/
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0) {
		tloge("can't open stream socket, errno=%d", errno);
		pthread_exit((void *)-1);
	}
	/* Fill in address structure and bind to socket*/
	struct sockaddr_un local, remote;
	ret = memset_s(&local, sizeof(local), 0, sizeof(local));
	if (ret) {
		tloge("memset_s sockaddr_un local failed!\n");
		close(s);
		return NULL;
	}
	local.sun_family = AF_UNIX;

	ret = strncpy_s(local.sun_path, sizeof(local.sun_path),
			  TC_NS_SOCKET_NAME, sizeof(TC_NS_SOCKET_NAME));
	if (ret) {
		tloge("strncpy_s %s failed!\n",TC_NS_SOCKET_NAME);
		close(s);
		return NULL;
	}

	socklen_t len = (int)(strlen(local.sun_path) + sizeof(local.sun_family));
	/* Make the socket in the Abstract Domain(no path but everyone can connect)*/
	local.sun_path[0] = 0;
	if (bind(s, (struct sockaddr *) &local, len) < 0) {
		tloge("bind() to server socket failed, errno=%d", errno);
		pthread_exit((void *)-1);
	}

	/* Start listening on the socket*/
	if (listen(s, BACKLOG_LEN) < 0) {
		tloge("listen() failed, errno=%d", errno);
		pthread_exit((void *)-1);
	}

	tlogi("\n********* successfully initialized *********\n");

	while (1) { /*lint !e716 */
		/*int done, n;*/
		tlogi("Waiting for a connection...\n");
		t = sizeof(remote);
		s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&t);
		if (-1 == s2) {
			tloge("accept() to server socket failed, errno=%d", errno);
			break;
		}


		len = sizeof(struct ucred);
		if (getsockopt(s2, SOL_SOCKET, SO_PEERCRED, &cr, &len) < 0) {
			tloge("peercred failed %s\n", strerror(errno));
			break;
		}
#ifndef WITH_DEVCHIP_PLATFORM
		/* Trans the xml file to tzdriver: */
		tcu_authentication();
#endif
		int fd = open(TC_NS_CLIENT_DEV_NAME, O_RDWR);
		if (fd == -1) {
			tloge("Failed to open %s: %s\n", TC_NS_CLIENT_DEV_NAME, strerror(errno));
			goto close_socket;
		}

		buf = malloc(BUF_MAX_SIZE);
		if (NULL == buf) {
			tloge("malloc fail\n");
			goto close_socket;
		}

		bufl = BUF_MAX_SIZE;
		ret = memset_s(buf, BUF_MAX_SIZE, 0, BUF_MAX_SIZE);
		if (ret) {
			tloge("memset failed, ret=0x%x\n",ret);
			goto close_socket;
		}

		if (!tee_get_app_cert((int)cr.pid, (int)cr.uid, &bufl, buf, &type)) {
			ret = tee_check_ca_cert((int)type, buf, bufl, (unsigned long)cr.uid, (unsigned long)cr.pid);
			if (0 == ret) {
				/*lint -e569 */
				ret = ioctl(fd, TC_NS_CLIENT_IOCTL_LOGIN, buf); /*lint !e835 */
				if (ret) {
					tloge("Failed set login info for client err=%u!\n", ret);
					/* Since login information was not set
					 *we can't allow the client
					 * to get the socket */
					goto close_socket;
				}
			} else {
				tloge("CERT check failed<%d>.\n", ret);
				goto close_socket;
			}
		} else {
			tloge("Failed to get app cert!\n");
			/* Inform the driver the cert could not be set */
			/*lint -e569 */
			ret = ioctl(fd, TC_NS_CLIENT_IOCTL_LOGIN, NULL); /*lint !e835 */
			if (ret) {
				tloge("Failed to set login information for client err=%u!\n", ret);
				goto close_socket;
			}
		}

		ret = send_file_descriptor(s2, fd);
		if (ret < 0) {
			tloge("Failed to send file desc client err=%u!\n", ret);
		}

close_socket:
		if (fd != -1)
			close(fd);
		if (buf != NULL)
		        free(buf);/* [false alarm]:won't free again as while loop will malloc new one */
		close(s2);
	}

	pthread_exit(0);
	return NULL;
}
