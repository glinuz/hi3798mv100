#include <stdio.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_storage_api.h"

#define TST_FILE01 "this is the 01.txt, test write&read"
#define TST_FILE02 "111112222233333"
#define TST_FILE03 "this is the 03.txt, test open two files"
#define TST_FILE04 "this is the 04.txt, test open two files"
#define TST_FILE06 "this is the 06.txt, test open with exclusive flag"

/* test write file once, file is 01.txt */
int login_write_01()
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;

	TEEC_InitStorageService();

	fd = TEEC_FOpen("/data/01.txt", TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	strcpy(write_buff, TST_FILE01);
	len = strlen(TST_FILE01);
	write_size = TEEC_FWrite(fd, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed,errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed\n");
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}


int main()
{
	if (login_write_01() != 0) {
		printf("test test_write_01 failed\n");
		while (1);
	} else {
		printf("test test_write_01 success\n");
	}
	printf("ALL test case are PASS\n");
	return 0;
}
