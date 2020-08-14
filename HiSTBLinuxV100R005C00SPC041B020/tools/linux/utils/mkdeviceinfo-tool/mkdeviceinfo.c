/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

/*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "deviceinfo.h"


#ifdef  __cplusplus
extern "C" {
#endif

#define DEVICEINFO_DEFSIZE		(512*1024)	/*64kb or 128kb*/
#define DEVICEINFO_DEFARGS 		"00000003-00000001-1d000021"

int main(int argc,char* argv[])
{
    HI_U8 *deviceinfo_buf = NULL ;
    HI_U8 ac8TmpBuf[128];
    HI_U8 *pCurPos = NULL;
    HI_U8 *pPosLength = NULL;
    HI_LOADER_STB_INFO_S stLoaderStbInfo = {0};
    FILE *fp;
	int oc = 0;
	HI_U32 deviceinfo_size = DEVICEINFO_DEFSIZE;
	HI_S8 deviceinfo_defstr[] = DEVICEINFO_DEFARGS;
    
	if (argc < 2) {
		printf("Usage: mkdeviceinfo:\n"
		       "       [-z size(KB) -s stbidstring]\n");
    	printf("Example:./mkdeviceinfo -z 512 -s 00000003-00000001-1d000021\n");
		return 1;
	}

	while ((oc = getopt(argc, argv, "s:z:")) != -1) {
		switch (oc) {
		case 's':
			memcpy(deviceinfo_defstr, optarg, strlen(optarg)+1);
			break;
		case 'z':
			deviceinfo_size = strtoul(optarg,NULL, 10)*1024;
			printf("stbid_size is %#x\n", deviceinfo_size);
		default:
			break;
		}
	}
	
    printf("make defaut deviceinfo.bin ...\n");
	
	sscanf(deviceinfo_defstr, "%x-%x-%x", &stLoaderStbInfo.u32OUI, &stLoaderStbInfo.u32HWVersion, &stLoaderStbInfo.u32HWSerialNum);	
	
    deviceinfo_buf =malloc(deviceinfo_size);
    if(NULL == deviceinfo_buf)
    {
        printf("malloc(0x%x) faile!\n",deviceinfo_size);
        return -1;
    }
    memset(deviceinfo_buf, 0xFF, deviceinfo_size);
	
	pCurPos = deviceinfo_buf;
	/* reserve MAC , but unused for loader */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "%s", "FFFFFFFFFFFF");
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* SN */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "%08x", stLoaderStbInfo.u32HWSerialNum);
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */
    
    /* MAGICNUM */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "MAGICNUM=%s", STBINFO);
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* reserve LENGTH(LENGTH=xxxxxxxx) space */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "LENGTH=12345678");
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pPosLength = pCurPos ;
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */
    
    /* OUI */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "OUI=%08x", stLoaderStbInfo.u32OUI);
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */
    
    /* HW */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "HW=%08x", stLoaderStbInfo.u32HWVersion
);
    strncpy(pCurPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pCurPos += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* update LENGTH */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "LENGTH=%08x", (HI_U32)(pCurPos - deviceinfo_buf));
    strncpy(pPosLength, ac8TmpBuf, strlen(ac8TmpBuf) + 1);

	
    fp = fopen("deviceinfo.bin", "w+b");
    if(NULL == fp)
    {
        printf("open deviceinfo.bin error!");
		free(deviceinfo_buf);
        return -1;
    }
    fwrite(deviceinfo_buf, 1, deviceinfo_size, fp);    
    fclose(fp);
	
	
    printf("#----------defaut device info----------#\n");
    printf("set defaut deviceinfo size = %#x\n", deviceinfo_size);
   	printf("==================stb info================\n");
	printf("Factory ID             \t0x%08x\n", stLoaderStbInfo.u32OUI);
	printf("HW version             \t0x%08x\n", stLoaderStbInfo.u32HWVersion);
	printf("HW serial num          \t0x%08x\n", stLoaderStbInfo.u32HWSerialNum);
	printf("==========================================\n");
    printf("#----------defaut device info----------#\n");
    free(deviceinfo_buf);
    printf("make defaut deviceinfo.bin success.\n");
	return 0;
}

#ifdef  __cplusplus
}
#endif

