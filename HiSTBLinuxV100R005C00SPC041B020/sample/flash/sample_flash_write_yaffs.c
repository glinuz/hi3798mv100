#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_flash.h"

#define FLASH_NAME  "/dev/mtd3"

HI_S32 main(int argc, char *argv[])
{
	HI_Flash_InterInfo_S stFlashInfo;
	char *image_file = NULL;
	char *partion_name = FLASH_NAME;
	HI_U8 *pWriteBuf = NULL;

	HI_U32 img_fd = -1;
	HI_S32 img_len = 0;
	HI_U32 fd = -1;
	HI_S32 ret = 0;

	if ((2 != argc) && (3 != argc)) {
		printf("Usage:\n"
		       "\t%s file_path [partion_name]\n"
		       "Example:\n"
		       "\t%s test.yaffs /dev/mtd3\n", argv[0], argv[0]);
		return 0;
	}

	ret = HI_SYS_Init();
	if (ret != HI_SUCCESS)
	{
		printf("call HI_SYS_Init failed.\n");
		return ret;
	}

	image_file = argv[1];
	if (argv[2])
		partion_name = argv[2];

	fd = HI_Flash_Open(HI_FLASH_TYPE_NAND_0, partion_name, 0x0, 0x0);
	if (INVALID_FD == fd) {
		printf("ret=%d: open %s failed!\n", fd, partion_name);
		return -1;
	}

	img_fd = open(image_file, O_RDONLY);
	if (img_fd < 0) {
		printf("ret=%d: open %s failed!\n", img_fd, image_file);
		goto img_error2;
	}

	img_len = lseek(img_fd, 0, SEEK_END);
	lseek(img_fd, 0, SEEK_SET);

	ret = HI_Flash_GetInfo(fd, &stFlashInfo);
	if (0 != ret) {
		printf("ret=%d: get flash info failed!\n", ret);
		goto img_error1;
	}

	if (img_len > stFlashInfo.PartSize) {
		printf("wirte size(0x%x) beyound flash partion size(0x%llx)!\n",
		      img_len, stFlashInfo.PartSize);
	}

	if (img_len % (stFlashInfo.PageSize + stFlashInfo.OobSize)) {
		printf("wirte size(0x%x) should be aligned with "
		       "pagesize+oobsize(0x%x))!\n",
		       img_len, stFlashInfo.PageSize + stFlashInfo.OobSize);
	}
	pWriteBuf = (HI_U8 *) malloc(stFlashInfo.PartSize);
	if (pWriteBuf == NULL) {
		printf("pWriteBuf malloc failed!\n");
		goto img_error1;
	}
	//memset(pWriteBuf, '\0', mtdinfo.size); // warning: not kill
	memset(pWriteBuf, '\0', stFlashInfo.PartSize);

	if ((ret = read(img_fd, pWriteBuf, img_len)) != img_len) {
		printf("read yaffs fail!\n");
		goto img_error0;
	}

	ret = HI_Flash_Write(fd, 0x0, pWriteBuf, img_len,
			     HI_FLASH_RW_FLAG_WITH_OOB
			     | HI_FLASH_RW_FLAG_ERASE_FIRST);
	if (0 >= ret)
		printf("ret=%d: write yaffs failed!\n", ret);
	else
		printf("flash write yaffs OK!\n");

img_error0:
	free(pWriteBuf);
	printf("> %s: %d\n", __FUNCTION__, __LINE__); //warning: free

img_error1:
	ret = close(img_fd);
	if (0 != ret)
		printf("ret=%d: close yaffs failed!\n", ret);

img_error2:
	ret = HI_Flash_Close(fd);
	if (0 != ret) {
		printf("ret=%d: Flash Close failed!\n", ret);
		return -1;
	}

	return 0;
}
