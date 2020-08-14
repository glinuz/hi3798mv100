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
	HI_U8 *pReadBuf;
	HI_U32 i;
	HI_U32 flash_type = HI_FLASH_TYPE_NAND_0;
	HI_S32 ret = 0;
	HI_HANDLE fd = INVALID_FD;
	HI_U32 withoob = 0;

	if ((4 != argc) && (5 != argc) && (6 != argc)) {
		printf("Usage:\n"
		       "\t%s startaddr len withoob [partion_name [flash type]]\n"
		       "Example:\n"
		       "\t%s startaddr len 1 /dev/mtd3 1\n", argv[0], argv[0]);
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
	withoob = strtol(argv[3], NULL, 0);

	if (argv[4] && argv[5])
		flash_type = strtol(argv[5], NULL, 0);

	fd = HI_Flash_Open(flash_type, argv[4], u64Addr, u64Len);
	if (INVALID_FD == fd)
		return -1;

	ret = HI_Flash_GetInfo(fd, &stFlashInfo);
	if (0 != ret) {
		printf("ret=%d: get flash info fail!\n", ret);
		return -1;
	}

	pReadBuf = (HI_U8 *) malloc(u64Len);
	if (pReadBuf == NULL) {
		printf("pReadBuf malloc failure!\n");
		goto error0;
	}
	memset(pReadBuf, '\0', u64Len);

	if (withoob) {
		withoob = HI_FLASH_RW_FLAG_WITH_OOB;
	} else {
		withoob = HI_FLASH_RW_FLAG_RAW;
	}

	if (NULL == argv[4]) {
		printf("startaddr=0x%llx, len=0x%llx, withoob=%d\n",
			u64Addr, u64Len, withoob);
		ret = HI_Flash_Read(fd, 0, pReadBuf, (HI_U32)u64Len, withoob);
	} else {
		printf("partition=%s, startaddr=0x%llx, len=0x%llx, withoob=%d\n",
			argv[4], u64Addr, u64Len, withoob);
		ret = HI_Flash_Read(fd, u64Addr, pReadBuf,
				   (HI_U32)u64Len, withoob);
	}

	if (0 >= ret) {
		if (HI_FLASH_END_DUETO_BADBLOCK != ret) {
			printf("> %d Line: Flash Read failure(ret=%#x)!\n",
			       __LINE__, ret);
		}
	} else {
		for (i = 0; i < u64Len; i++) {
			if ((i % 16) == 0)
				printf("%08llx: ",
					argv[4]
					? (stFlashInfo.pPartInfo->StartAddr + u64Addr + i)
					: u64Addr + i);
			printf("%02x ", pReadBuf[i]);
			if (((i + 1) % 16) == 0)
				printf("\n");
		}
		printf("\n");
	}

	free(pReadBuf);

error0:
	ret = HI_Flash_Close(fd);
	if (0 != ret) {
		printf("> %d Line: Flash Close failure!\n", __LINE__);
		return -1;
	}

	return 0;
}
