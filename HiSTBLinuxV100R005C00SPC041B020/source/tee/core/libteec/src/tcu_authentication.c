/*
 * file: tcu_authentication.c
 * author: wangdedong
 * create date: 2016/6/21
 * description: The file excutes the ca's identification check.
 * It will send the xml id info to the tzdriver, and then transmit
 * to the secure os.
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
#include <openssl/sha.h>
/*debug switch*/
#define LOG_NDEBUG 0
#define LOG_TAG "teecd"

#define HASH_FILE_MAX_SIZE 8192

#include <cutils/log.h>
#include <cutils/properties.h>
#include "tc_ns_client.h"
#include "tee_ca_daemon.h"
#include "tee_client_type.h"
#include "securec.h"

static int get_file_size(const char *path)
{
	int filesize = -1;
	FILE *fp;
	int ret;

	fp = fopen(path, "r");
	if (fp == NULL)
		return filesize;
	ret = fseek(fp, 0L, SEEK_END);
	if (ret < 0) {
		fclose(fp);
		return filesize;
	}
	filesize = (int)ftell(fp);
	fclose(fp);
	return filesize;
}

static int read_a_file(int buflen, uint8_t *buffer, const char *path)
{
	FILE *fd;
	int num;

	if ((buflen < 0) || (buflen > HASH_FILE_MAX_SIZE) || (NULL == buffer) || (NULL == path)) {
		ALOGE("read a file, param is error\n");
		return -2;
	}
	fd = fopen(path, "rb");
	if (NULL == fd) {
		ALOGE("open file %s fail\n", path);
		return -1;
	}
	num = (int)fread(buffer, sizeof(char), (unsigned int)buflen, fd);
	if (num != buflen) {
		ALOGE("read file read number:%d\n", num);
		fclose(fd);
		return -1;
	}

	fclose(fd);
	return 0;
}

static int tee_set_native_ca_hash(const char *xml_flie)
{
	int ret, fd, buflen;
	uint8_t *buffer;

	if (NULL == xml_flie) {
		ALOGE("set native ca hash: error xml file inut\n");
		return -1;
	}
	buflen = get_file_size(xml_flie);
	if ((buflen <= 0) || (buflen > HASH_FILE_MAX_SIZE)) {
		ALOGW("auth xml file(%s) not exist or length illegal: %d\n", xml_flie, buflen);
		return -1;
	}

	buffer = malloc((unsigned int)buflen);
	if (NULL == buffer) {
		ALOGE("malloc fail\n");
		return -1;
	}

	ret = memset_s(buffer, (unsigned int)buflen, 0, (unsigned int)buflen);
	if (ret) {
		ALOGE("memset failed!\n");
		free(buffer);
		return ret;
	}

	fd = open(TC_NS_CLIENT_DEV_NAME, O_RDWR);
	if (fd < 0) {
		ALOGE("Failed to open %s: %s\n", TC_NS_CLIENT_DEV_NAME, strerror(errno));
		free(buffer);
		return -1;
	}

	ret = read_a_file(buflen, buffer, xml_flie);
	if (ret) {
		ALOGE("read file fail %d\n", ret);
		goto error;
	}
	/*lint -e835 -esym(835,*)*/
	ret = ioctl(fd, (int)(TC_NS_CLIENT_IOCTL_SET_NATIVE_IDENTITY), buffer); /*lint -e835 */
	/*lint -e835 +esym(835,*)*/
	if (ret) {
		ALOGE("ioctl fail %d\n", ret);
	}

error:
	free(buffer);
	close(fd);
	return ret;
}

#define HASH_XML_FILE "/system/etc/native_packages.xml"
#define SBIN_HASH_FILE "/res/native_packages.xml"
#define VENDOR_HASH_FILE "/vendor/etc/native_packages.xml"
void tcu_authentication(void)
{
	/*for ca xml status*/
	static int g_hashxml_setted;
	static int g_sbinhashxml_setted;
	static int g_vendor_hashxml_setted;

	/*lint -e727 -esym(727,*)*/
	if (0 == g_hashxml_setted) { /*lint -e727 */
	/*lint -e727 +esym(727,*)*/
		if (0 == tee_set_native_ca_hash(HASH_XML_FILE)) {
			g_hashxml_setted = 1;
		}
	}
	if (0 == g_sbinhashxml_setted) { /*lint -e727 */
		if (0 == tee_set_native_ca_hash(SBIN_HASH_FILE)) {
			g_sbinhashxml_setted = 1;
		}
	}
	if (0 == g_vendor_hashxml_setted) { /*lint -e727 */
		if (0 == tee_set_native_ca_hash(VENDOR_HASH_FILE)) {
			g_vendor_hashxml_setted = 1;
		}
	}
}

