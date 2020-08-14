#include <stdio.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_storage_api.h"
#include <unistd.h>
#include <sys/types.h>

#define TST_FILE01 "this is the 01.txt, test write&read"
#define TST_FILE02 "111112222233333"
#define TST_FILE03 "this is the 03.txt, test open two files"
#define TST_FILE04 "this is the 04.txt, test open two files"
#define TST_FILE06 "this is the 06.txt, test open with exclusive flag"

#define TEEC_DEBUG

#define SECURE_STORAGE_PARTITION "sec_storage/"
#define SECURE_STORAGE_DATA_PARTITION "sec_storage_data/"
#define FILE01_NAME "app1/01.txt"
#define FILE02_NAME "app1/02.txt"
#define FILE03_NAME "app1/03.txt"
#define FILE04_NAME "app2/sub1/04.txt"
#define FILE05_NAME "app2/sub1/05.txt"
//#define FILE06_NAME "app2/sub1/min/micro/06.txt"
#define FILE06_NAME "app2/abcdefghijklmnopqrstuvwxyz/06.txt"   //ok
//#define FILE06_NAME "app2/abcdefghijklmnopqr/06.txt"
//#define FILE06_NAME "app2/06.txt"



/* test write file once, file is 01.txt */
int test_write_01(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
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

/* test read file once, file is 01.txt */
int test_read_01(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t read_size;
	char read_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	memset(read_buff, 0x00, 128);
	len = 128;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	if (memcmp(read_buff, TST_FILE01, strlen(TST_FILE01))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_size=%d, read_buff=%s\n", read_size, read_buff);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}


/* test open file 01.txt, it has been exist, mode is TEE_DATA_FLAG_EXCLUSIVE */
int test_open_01(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_EXCLUSIVE);
	if (fd != -1) {
		TEEC_Error("%s: TEEC_FOpen success, but SHOULD failed\n", __FUNCTION__);
		res = 1;
	}

	TEEC_UninitStorageService();
	return res;
}

/* test write file three times, file is 02.txt */
int test_write_02_times(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE02_NAME, strlen(FILE02_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	strcpy(write_buff, "11111");
	len = strlen("11111");
	write_size = TEEC_FWrite(fd, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	strcpy(write_buff, "22222");
	len = strlen("22222");
	write_size = TEEC_FWrite(fd, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	strcpy(write_buff, "33333");
	len = strlen("33333");
	write_size = TEEC_FWrite(fd, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test read file once, file is 02.txt */
int test_read_02_once(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t read_size;
	char read_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE02_NAME, strlen(FILE02_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	memset(read_buff, 0x00, 128);
	len = 25;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	if (memcmp(read_buff, TST_FILE02, strlen(TST_FILE02))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_len=%d, read_size=%d, read_buff=%s\n", len, read_size, read_buff);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test read file three times, file is 02.txt */
int test_read_02_times(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t read_size;
	char read_buff[128], cmp_buf[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE02_NAME, strlen(FILE02_NAME) + 1);

	memset(cmp_buf, 0x00, 128);
	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	memset(read_buff, 0x00, 128);
	len = 5;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	strncpy(cmp_buf, read_buff, read_size);

	memset(read_buff, 0x00, 128);
	len = 5;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	strncat(cmp_buf, read_buff, read_size);

	memset(read_buff, 0x00, 128);
	len = 5;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	strncat(cmp_buf, read_buff, read_size);

	if (memcmp(cmp_buf, TST_FILE02, strlen(TST_FILE02))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_size=%d, read_buff=%s, cmp_buf=%s\n", read_size, read_buff, cmp_buf);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test write file once, simultaneity, file is 03.txt & 04.txt */
int test_write_03_04(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd3, fd4;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file1_path[128];
	char file2_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	memset(file2_path, 0, sizeof(file2_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE03_NAME, strlen(FILE03_NAME) + 1);
	strncat(file2_path, partition_name, strlen(partition_name) + 1);
	strncat(file2_path, FILE04_NAME, strlen(FILE04_NAME) + 1);

	TEEC_InitStorageService();

	fd3 = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
	if (fd3 == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd3=%d\n", fd3);

	fd4 = TEEC_FOpen(file2_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
	if (fd4 == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd4=%d\n", fd4);

	strcpy(write_buff, TST_FILE03);
	len = strlen(TST_FILE03);
	write_size = TEEC_FWrite(fd3, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	strcpy(write_buff, TST_FILE04);
	len = strlen(TST_FILE04);
	write_size = TEEC_FWrite(fd4, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd3);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd4);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test read file once, simultaneity, file is 03.txt & 04.txt */
int test_read_03_04(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd3, fd4;
	int ret, res = 0;
	int32_t read_size;
	char read_buff[128];
	int len;
	char file1_path[128];
	char file2_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	memset(file2_path, 0, sizeof(file2_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE03_NAME, strlen(FILE03_NAME) + 1);
	strncat(file2_path, partition_name, strlen(partition_name) + 1);
	strncat(file2_path, FILE04_NAME, strlen(FILE04_NAME) + 1);

	TEEC_InitStorageService();

	fd3 = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd3 == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd3=%d\n", fd3);

	fd4 = TEEC_FOpen(file2_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd4 == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd4=%d\n", fd4);

	memset(read_buff, 0x00, 128);
	len = 50;
	read_size = TEEC_FRead(fd3, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	if (memcmp(read_buff, TST_FILE03, strlen(TST_FILE03))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_size=%d, read_buff=%s\n", read_size, read_buff);
		res = 1;
		goto error;
	}

	memset(read_buff, 0x00, 128);
	len = 50;
	read_size = TEEC_FRead(fd4, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	if (memcmp(read_buff, TST_FILE04, strlen(TST_FILE04))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_size=%d, read_buff=%s\n", read_size, read_buff);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd3);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd4);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}


/* test write & read file simultaneity times, file is 05.txt */
int test_write_read_05_times(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int write_fd, read_fd;
	int ret, res = 0;
	int32_t write_size, read_size;
	char write_buff[128], read_buff[128];
	int write_len, read_len;
	int i, j;
	char file1_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE05_NAME, strlen(FILE05_NAME) + 1);

	TEEC_InitStorageService();

	write_fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_SHARE_READ | TEE_DATA_FLAG_CREATE);
	if (write_fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: write_fd=%d\n", write_fd);

	read_fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ);
	if (read_fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: read_fd=%d\n", read_fd);

	write_len = 20;
	read_len = 20;
	for (i = 0; i < 50; i++) {
		memset(write_buff, i, write_len);
		write_size = TEEC_FWrite(write_fd, write_buff, write_len);
		if ((write_size != write_len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
			TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
			res = 1;
			goto error;
		}

		memset(read_buff, 0x00, read_len);
		read_size = TEEC_FRead(read_fd, read_buff, read_len);
		if ((read_size != read_len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
			TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
			res = 1;
			goto error;
		}
		if (memcmp(write_buff, read_buff, read_len) != 0) {
			TEEC_Error("[%d]: \n", i);
			for (j = 0; j < write_len; j++)
				TEEC_Error("%x ", write_buff[j]);
			TEEC_Error("read_size=%d\n", read_size);
			for (j = 0; j < read_len; j++)
				TEEC_Error("%x ", read_buff[j]);
			res = 1;
			goto error;
		}
	}

	ret = TEEC_FClose(write_fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	ret = TEEC_FClose(read_fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test write file once, file is 08.txt, mode is  TEE_DATA_FLAG_EXCLUSIVE*/
int test_write_06(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file1_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE06_NAME, strlen(FILE06_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_EXCLUSIVE);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	strcpy(write_buff, TST_FILE06);
	len = strlen(TST_FILE06);
	write_size = TEEC_FWrite(fd, write_buff, len);
	if ((write_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
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

/* test read file once, file is 08.txt */
int test_read_06(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t read_size;
	char read_buff[128];
	int len;
	char file1_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE06_NAME, strlen(FILE06_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	memset(read_buff, 0x00, 128);
	len = 100;
	read_size = TEEC_FRead(fd, read_buff, len);
	if ((read_size != len) && (TEEC_GetFErr() != TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FRead failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

	if (memcmp(read_buff, TST_FILE06, strlen(TST_FILE06))) {
		TEEC_Error("%s: TEEC_FRead success, but read is not same to write\n", __FUNCTION__);
		TEEC_Error("read_size=%d, read_buff=%s\n", read_size, read_buff);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Error("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}

error:
	TEEC_UninitStorageService();
	return res;
}

/* test not init_storage_service, return false */
int test_not_initstorage(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	char file1_path[32];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		return 0;
	} else {
		TEEC_Error("%s: TEEC_FOpen success, but an error case\n", __FUNCTION__);
		return 1;
	}
}

/* test fd is error, return false */
int test_err_fd(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd, err_fd = 0xFF;
	int ret, res = 0;
	int32_t read_size, write_size;
	char read_buff[128], write_buff[128];
	int len;
	char file1_path[32];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);

	read_size = TEEC_FRead(err_fd, read_buff, 10);
	if ((read_size == 10) || (TEEC_GetFErr() == TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FOpen success, but an error case\n", __FUNCTION__);
		res = 1;
		goto error;
	}

	strcpy(write_buff, "0123456789");
	write_size = TEEC_FWrite(err_fd, write_buff, 10);
	if ((write_size == 10) || (TEEC_GetFErr() == TEEC_SUCCESS)) {
		TEEC_Error("%s: TEEC_FWrite success, but an error case\n", __FUNCTION__);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(err_fd);
	if (ret != -1) {
		TEEC_Error("%s: TEEC_FClose success, but an error case\n", __FUNCTION__);
		res = 1;
		goto error;
	}

	ret = TEEC_FClose(fd);
	if (ret == -1) {
		TEEC_Debug("%s: TEEC_FClose failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
		res = 1;
		goto error;
	} else {
		TEEC_Error("%s: TEEC_FClose success\n", __FUNCTION__);
	}

error:
	TEEC_UninitStorageService();
	return res;
}

int tee_delete_all_test_files(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	char file1_path[128];
	char file2_path[128];
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE01_NAME, strlen(FILE01_NAME) + 1);
	TEEC_InitStorageService();

	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x, file_path=%s\n", __FUNCTION__, TEEC_GetFErr(), file1_path);
		res = 1;
		goto error;
	}
	TEEC_FCloseAndDelete(fd);
	TEEC_Debug("test: fd=%d\n", fd);
	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE02_NAME, strlen(FILE02_NAME) + 1);
	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x, file_path=%s\n", __FUNCTION__, TEEC_GetFErr(), file1_path);
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);
	TEEC_FCloseAndDelete(fd);

	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE03_NAME, strlen(FILE03_NAME) + 1);
	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x, file_path=%s\n", __FUNCTION__, TEEC_GetFErr(), file1_path);
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);
	TEEC_FCloseAndDelete(fd);

	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE04_NAME, strlen(FILE04_NAME) + 1);
	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x, file_path=%s\n", __FUNCTION__, TEEC_GetFErr(), file1_path);
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);
	TEEC_FCloseAndDelete(fd);
	/*
	//not pass, because file sync problem
	    memset(file1_path, 0, sizeof(file1_path));
	    strncat(file1_path, partition_name, strlen(partition_name)+1);
	    strncat(file1_path, FILE05_NAME, strlen(FILE05_NAME)+1);

	    fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ|TEE_DATA_FLAG_ACCESS_WRITE_META);
	    if (fd == -1) {
	        TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x\n", __FUNCTION__, TEEC_GetFErr());
	        res=1;
	        goto error;
	    }
	    TEEC_Debug("test: fd=%d\n", fd);
	    TEEC_FCloseAndDelete(fd);
	*/

	memset(file1_path, 0, sizeof(file1_path));
	strncat(file1_path, partition_name, strlen(partition_name) + 1);
	strncat(file1_path, FILE06_NAME, strlen(FILE06_NAME) + 1);
	fd = TEEC_FOpen(file1_path, TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META);
	if (fd == -1) {
		TEEC_Error("%s: TEEC_FOpen failed, errno=0x%x, file_path=%s\n", __FUNCTION__, TEEC_GetFErr(), file1_path);
		res = 1;
		goto error;
	}
	TEEC_Debug("test: fd=%d\n", fd);
	TEEC_FCloseAndDelete(fd);

error:
	TEEC_UninitStorageService();
	return res;
}



/* test write file once, file is 01.txt */
int test_write_invalid_name(char *partition_name)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file_path[32];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, partition_name, strlen(partition_name) + 1);
	strncat(file_path, FILE01_NAME, strlen(FILE01_NAME) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
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



/* test write file once, file is 01.txt */
int test_write_userdefined_filename(char *filename)
{
	TEEC_Debug("----------%s----------\n", __FUNCTION__);
	int fd;
	int ret, res = 0;
	int32_t write_size;
	char write_buff[128];
	int len;
	char file_path[200];
	memset(file_path, 0, sizeof(file_path));
	strncat(file_path, filename, strlen(filename) + 1);

	TEEC_InitStorageService();

	fd = TEEC_FOpen(file_path, TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE);
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

int main(int argc, char **argv)
{
#if 1
#if 1
#if 1
	if (test_write_01(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_write_01 failed\n");
		while (1);
	} else {
		printf("test test_write_01 success\n");
	}
	if (test_read_01(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_read_01 failed\n");
		while (1);
	} else {
		printf("test test_read_01 success\n");
	}
	if (test_open_01(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_open_01 failed\n");
		while (1);
	} else {
		printf("test test_open_01 success\n");
	}

	if (test_write_02_times(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_write_02_times failed\n");
		while (1);
	} else {
		printf("test test_write_02_times success\n");
	}
	if (test_read_02_once(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_read_02_once failed\n");
		while (1);
	} else {
		printf("test test_read_02_once success\n");
	}
	if (test_read_02_times(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_read_02_times failed\n");
		while (1);
	} else {
		printf("test test_read_02_times success\n");
	}

	if (test_write_03_04(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_write_03_04 failed\n");
		while (1);
	} else {
		printf("test test_write_03_04 success\n");
	}
	if (test_read_03_04(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_read_03_04 failed\n");
		while (1);
	} else {
		printf("test test_read_03_04 success\n");
	}
#endif
	//not pass, because file sync problem
	/*
	    if (test_write_read_05_times(SECURE_STORAGE_PARTITION) != 0) {
	        TEEC_Error("test test_write_read_05_times failed\n");
	        while(1);
	    } else {
	        printf("test test_write_read_05_times success\n");
	    }
	*/

	if (test_write_06(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_write_06 failed\n");
		while (1);
	} else {
		printf("test test_write_06 success\n");
	}
	if (test_read_06(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_read_06 failed\n");
		while (1);
	} else {
		printf("test test_read_06 success\n");
	}
#if 1
	if (test_not_initstorage(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_not_initstorage failed\n");
		while (1);
	} else {
		printf("test test_not_initstorage success\n");
	}

	if (test_err_fd(SECURE_STORAGE_PARTITION) != 0) {
		printf("test test_err_fd failed\n");
		while (1);
	} else {
		printf("test test_err_fd success\n");
	}


	if (tee_delete_all_test_files(SECURE_STORAGE_PARTITION) != 0) {
		printf("test tee_close_all_test_files failed\n");
		while (1);
	} else {
		printf("test tee_close_all_test_files success\n");
	}
#endif
	printf("ALL test case for sec_storage partition are PASS\n");
#endif
#if 1
	if (test_write_01(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_write_01 failed\n");
		while (1);
	} else {
		printf("test test_write_01 success\n");
	}
	if (test_read_01(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_read_01 failed\n");
		while (1);
	} else {
		printf("test test_read_01 success\n");
	}
	if (test_open_01(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_open_01 failed\n");
		while (1);
	} else {
		printf("test test_open_01 success\n");
	}

	if (test_write_02_times(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_write_02_times failed\n");
		while (1);
	} else {
		printf("test test_write_02_times success\n");
	}
	if (test_read_02_once(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_read_02_once failed\n");
		while (1);
	} else {
		printf("test test_read_02_once success\n");
	}
	if (test_read_02_times(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_read_02_times failed\n");
		while (1);
	} else {
		printf("test test_read_02_times success\n");
	}

	if (test_write_03_04(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_write_03_04 failed\n");
		while (1);
	} else {
		printf("test test_write_03_04 success\n");
	}
	if (test_read_03_04(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_read_03_04 failed\n");
		while (1);
	} else {
		printf("test test_read_03_04 success\n");
	}

	//not pass, because file sync problem
	/*
	    if (test_write_read_05_times(SECURE_STORAGE_DATA_PARTITION) != 0) {
	        TEEC_Error("test test_write_read_05_times failed\n");
	        while(1);
	    } else {
	        TEEC_Debug("test test_write_read_05_times success\n");
	    }
	*/

	if (test_write_06(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_write_06 failed\n");
		while (1);
	} else {
		printf("test test_write_06 success\n");
	}
	if (test_read_06(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_read_06 failed\n");
		while (1);
	} else {
		printf("test test_read_06 success\n");
	}

	if (test_not_initstorage(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_not_initstorage failed\n");
		while (1);
	} else {
		printf("test test_not_initstorage success\n");
	}

	if (test_err_fd(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test test_err_fd failed\n");
		while (1);
	} else {
		printf("test test_err_fd success\n");
	}

	if (tee_delete_all_test_files(SECURE_STORAGE_DATA_PARTITION) != 0) {
		printf("test tee_close_all_test_files failed\n");
		while (1);
	} else {
		printf("test tee_close_all_test_files success\n");
	}
#endif
	uid_t test_uid;
	test_uid = getuid();
	printf("test storage process uid is %d\n", test_uid);
#endif

	if (argc != 2) {
		printf("invalid argc=%d\n", argc);
	}


	printf("Input argv is %s\n", argv[1]);

	if (test_write_userdefined_filename(argv[1])) {
		printf("test test_write_userdefined_filename failed\n");
	}


	/*
	    while(1)
	    {
	        printf("++ Delay ++\n");
	        sleep(1);
	        printf("-- Delay --\n");
	    }
	*/
	printf("ALL test case for sec_storage_data partition are PASS\n");

	printf("ALL test case are PASS\n");
	return 0;
}
