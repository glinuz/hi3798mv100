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
	HI_U8 *pWriteBuf = NULL;

	HI_U32 img_fd = -1;
	HI_U32 fd = -1;
	HI_S32 ret = 0;

	HI_U32 img_len2 = 0;
	HI_U64 start_addr = 0;
	HI_S32 ret_len = 0;
	char *partion_name = NULL;

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
	if (INVALID_FD == fd)
		return -1;

	img_fd = open(image_file, O_RDONLY);
	if (img_fd < 0) {
		printf("img_fd=%d: open yaffs fail!\n", img_fd);
		goto img_error2;
	}

	ret = HI_Flash_GetInfo(fd, &stFlashInfo);
	if (0 != ret) {
		printf("ret=%d: get flash info fail!\n", ret);
		goto img_error1;
	}

	img_len2 = (stFlashInfo.PageSize + stFlashInfo.OobSize) *
		   (stFlashInfo.BlockSize / stFlashInfo.PageSize);

	pWriteBuf = (HI_U8 *) malloc(img_len2);
	if (pWriteBuf == NULL) {
		printf("pWriteBuf malloc fail!\n");
		goto img_error1;
	}

	lseek(img_fd, 0, SEEK_SET);

	while (1) {
		if ((ret_len = read(img_fd, pWriteBuf, img_len2)) <= 0) {
			printf("ret=%d, img_len2=0x%x: read yaffs %s!\n",
				ret_len, img_len2, ret_len ? "fail" : "end");
			goto img_error0;
		}

		ret = HI_Flash_Write(fd, start_addr, pWriteBuf,
				     ret_len, HI_FLASH_RW_FLAG_WITH_OOB);
		if (0 >= ret) {
			printf("ret=%d: write length=0x%x, start_addr=0x%llx: "
			       "write yaffs fail!\n", ret, ret_len, start_addr);
			goto img_error0;
		}

		if (ret_len % (stFlashInfo.PageSize + stFlashInfo.OobSize)) {
			printf("wirte size(0x%x) should be aligned with "
			       "pagesize+oobsize(0x%x))!\n",
			       ret_len,
			       stFlashInfo.PageSize + stFlashInfo.OobSize);
			break;
		}

		start_addr += stFlashInfo.BlockSize;
	}

img_error0:
	free(pWriteBuf);
	printf("> %s: %d\n", __FUNCTION__, __LINE__);	//warning: free

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
