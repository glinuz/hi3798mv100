#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include "hi_unf_otp.h"
#include "hi_type.h"

HI_S32 main(int argc, char *argv[])
{
    HI_S32 ret =HI_SUCCESS;
    HI_BOOL bLockFlag = HI_FALSE;
    
    ret = HI_UNF_OTP_Init();
	if(HI_SUCCESS != ret)
	{
		printf("OTP init failed\n");
		return HI_FAILURE;
	}
	
    ret = HI_UNF_OTP_GetStbRootKeyLockFlag(&bLockFlag);
	if(HI_SUCCESS != ret)
	{
		printf("OTP init failed\n");
	    (HI_VOID)HI_UNF_OTP_DeInit();
		return HI_FAILURE;
	}

    printf("get stb root key lock flag : %d\n", bLockFlag);

    (HI_VOID)HI_UNF_OTP_DeInit();

    return HI_SUCCESS;
}


