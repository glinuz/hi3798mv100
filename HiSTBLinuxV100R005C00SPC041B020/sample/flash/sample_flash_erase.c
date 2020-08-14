#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_flash.h"

HI_S32 main(int argc, char *argv[])
{
	HI_Flash_InterInfo_S stFlashInfo;
	HI_U64 u64Addr = 0;
	HI_U64 u64Len = 0;
	HI_S32 ret = 0;
	HI_U32 flash_type = HI_FLASH_TYPE_NAND_0;
	HI_HANDLE fd = INVALID_FD;

	if ((3 != argc) && (4 != argc) && (5 != argc)) {
		printf("Usage:\n"
		       "\t%s startaddr len [partion_name [flash type]]\n"
		       "Example:\n"
		       "\t%s startaddr len /dev/mtd3 1\n", argv[0], argv[0]);
		return 0;
	}

	ret = HI_SYS_Init();
	if (ret != HI_SUCCESS)
	{
		printf("call HI_SYS_Init failed.\n");
		return ret;
	}

	u64Addr = strtol(argv[1], NULL, 16);
	u64Len = strtol(argv[2], NULL, 16);

	if (argv[3] && argv[4])
		flash_type = strtol(argv[4], NULL, 0);

	fd = HI_Flash_Open(flash_type, argv[3], u64Addr, u64Len);
	if (INVALID_FD == fd)
		return -1;

	ret = HI_Flash_GetInfo(fd, &stFlashInfo);
	if (0 != ret) {
		printf("ret=%d: get flash info fail!\n", ret);
		return -1;
	}

	if (NULL == argv[3]) {
		printf("please input any key to start erase"
		       "startaddr=0x%llx, len=0x%llx\n", u64Addr, u64Len);
		getchar();
		ret = HI_Flash_Erase(fd, 0, u64Len);
	} else {
		printf("partition=%s, startaddr=0x%llx, len=0x%llx\n", argv[3],
			u64Addr, u64Len);
		ret = HI_Flash_Erase(fd, u64Addr, u64Len);
	}

	if (0 >= ret) {
		if (HI_FLASH_END_DUETO_BADBLOCK != ret) {
			printf("> %d Line: Flash Erase failure(ret=%#x)!\n",
			       __LINE__, ret);
		}
	}

	ret = HI_Flash_Close(fd);
	if (0 != ret) {
		printf("> %d Line: Flash Close failure!\n", __LINE__);
		return -1;
	}

	return 0;
}
