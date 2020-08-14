#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> /*for errno*/
#include <sys/types.h> /*for open close*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h> /*for ioctl*/
#include <sys/mman.h> /*for mmap*/
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
//debug switch
#define LOG_NDEBUG 0
#define LOG_TAG "teecd"
#include <cutils/log.h>
#include "tc_ns_client.h"

int main(int argc, char *argv[])
{


	int fd;
	void *buff = NULL;
	int ret;

	printf("Start to open mydevice:call kernel tee api\n");
	fd = open(TC_NS_MYDEVICE_NAME, O_RDWR);
	if (fd == -1) {
		printf("Failed to open mydevice\n");
		return -1;
	}
	printf("Succeed to open mydevice.\n");

	(void)close(fd);
	printf("Succeed to close mydevice.\n");
	return 0;
}
