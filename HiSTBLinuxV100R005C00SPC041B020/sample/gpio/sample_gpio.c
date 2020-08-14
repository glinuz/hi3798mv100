#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_gpio.h"
#include "hi_adp_boardcfg.h"

static HI_U32 g_u32Multi_useRegAddr = 0;
static HI_U32 g_u32Multi_useValue;

HI_S32 main(int argc, char *argv[])
{
    HI_S32 Ret;
    HI_U32 u32IntWaitTime = 5000;
    HI_U32 u32TestGpioNum ;
    HI_U32 u32IntValue = 0xff;
    HI_BOOL ReadBitVal;
    HI_U32 u32Value = 0;
    HI_U32 u32Group;
    HI_U32 u32Bit;
    HI_CHAR str[32];

    printf("Attention:\n"
           "This sample will change the multiplex function of GPIO pin.\n"
           "Please DO NOT press Ctrl+C to quit!\n");

	(HI_VOID)HI_SYS_Init();

    printf("Please input the GPIO port you want to test:\n");
    scanf("%s", str);    
    u32Value = (HI_U32)strtoul(str, 0, 0);
    u32Group = u32Value / 8;
    u32Bit = u32Value % 8;
    u32TestGpioNum = u32Value;

    printf("Please input multi-use register address of the GPIO port:\n");
    scanf("%s", str);
    u32Value = (HI_U32)strtoul(str, 0, 0);
    g_u32Multi_useRegAddr = u32Value;

    printf("Please input multi-use value used for the GPIO port:\n");
    scanf("%s", str);
    u32Value = (HI_U32)strtoul(str, 0, 0);

    HI_SYS_ReadRegister(g_u32Multi_useRegAddr, &g_u32Multi_useValue);
    HI_SYS_WriteRegister(g_u32Multi_useRegAddr, u32Value);

    Ret = HI_UNF_GPIO_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR0;
    }
	
	getchar();

    printf("Press any key to set GPIO%d_%d to output mode and output high level\n", u32Group, u32Bit);
    getchar();
    Ret = HI_UNF_GPIO_SetDirBit(u32TestGpioNum, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }
    Ret = HI_UNF_GPIO_WriteBit(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Press any key to set GPIO%d_%d to input mode and get the input level\n", u32Group, u32Bit);
    getchar();
    Ret = HI_UNF_GPIO_SetDirBit(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    Ret = HI_UNF_GPIO_ReadBit(u32TestGpioNum, &ReadBitVal);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("GPIO%d_%d Input: %d\n", u32Group, u32Bit, ReadBitVal);

    printf("Press any key to set GPIO%d_%d to up-interrupt mode.\n", u32Group, u32Bit);
    getchar();
    Ret = HI_UNF_GPIO_SetIntType(u32TestGpioNum, HI_UNF_GPIO_INTTYPE_UP);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    Ret = HI_UNF_GPIO_SetIntEnable(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Please wait for %d seconds to query interrupt on GPIO%d_%d\n", u32IntWaitTime / 1000, u32Group, u32Bit);
    Ret = HI_UNF_GPIO_QueryInt(&u32IntValue, u32IntWaitTime);
    if ((HI_SUCCESS == Ret) && (u32IntValue == u32TestGpioNum))
    {
        printf("GPIO%d_%d get an up edge interrupt\n", u32Group, u32Bit);
    }
    else if (HI_ERR_GPIO_GETINT_TIMEOUT == Ret)
    {
        printf("GPIO%d_%d has not got an up edge interrupt in %d seconds!\n", u32Group, u32Bit, u32IntWaitTime / 1000);
    }

    Ret = HI_UNF_GPIO_SetIntEnable(u32TestGpioNum, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Press any key to quit normally.");
    getchar();
ERR1:

    Ret = HI_UNF_GPIO_DeInit();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
    }

ERR0:
    HI_SYS_WriteRegister(g_u32Multi_useRegAddr, g_u32Multi_useValue);

	(HI_VOID)HI_SYS_DeInit();
	
    return Ret;
}
