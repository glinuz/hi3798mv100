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
	HI_U32 u64Len = 0;
	HI_U32 u32Data = 0;
	HI_U8 *pWriteBuf;
	HI_U32 i;
	HI_U32 flash_type = HI_FLASH_TYPE_NAND_0;
	HI_S32 ret = 0;
	HI_HANDLE fd = INVALID_FD;

	if ((4 != argc) && (5 != argc) && (6 != argc)) {
		printf("Usage:\n"
		       "\t%s startaddr len fill [partion_name [flash type]]\n"
		       "Example:\n"
		       "\t%s startaddr len 0x5a /dev/mtd3 1\n", argv[0], argv[0]);
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
	u32Data = strtol(argv[3], NULL, 16);

	if (argv[4] && argv[5])
		flash_type = strtol(argv[5], NULL, 0);

	fd = HI_Flash_Open(flash_type, argv[4], u64Addr, u64Len);
	if (INVALID_FD == fd) {
		return -1;
	}

	ret = HI_Flash_GetInfo(fd, &stFlashInfo);
	if (0 != ret) {
		printf("ret=%d: get flash info fail!\n", ret);
		return -1;
	}

	pWriteBuf = (HI_U8 *) malloc(u64Len);
	if (pWriteBuf == NULL) {
		printf("pWriteBuf malloc failure!\n");
		goto error1;
	}
	memset(pWriteBuf, '\0', u64Len);
	for (i = 0; i < u64Len; i++) {
		pWriteBuf[i] = u32Data;
	}

	if (NULL == argv[4]) {
		printf("startaddr=0x%llx, len=0x%x, withoob=%d\n",
			u64Addr, u64Len, HI_FLASH_RW_FLAG_RAW);
		ret = HI_Flash_Write(fd, 0, pWriteBuf,
			(HI_U32)u64Len, HI_FLASH_RW_FLAG_RAW);
	}
	else {
		printf("partition=%s, startaddr=0x%llx, len=0x%x, withoob=%d\n",
			argv[4], u64Addr, u64Len, HI_FLASH_RW_FLAG_RAW);
		ret = HI_Flash_Write(fd, u64Addr, pWriteBuf,
			(HI_U32)u64Len, HI_FLASH_RW_FLAG_RAW);
	}
	if (0 >= ret) {
		if (HI_FLASH_END_DUETO_BADBLOCK != ret) {
			printf("> %d Line: Flash Write failure(ret=%#x)!\n",
			       __LINE__, ret);
		}
	}
	free(pWriteBuf);

error1:
	ret = HI_Flash_Close(fd);
	if (0 != ret) {
		printf("> %d Line: Flash Close failure!\n", __LINE__);
		return -1;
	}

	return 0;
}
