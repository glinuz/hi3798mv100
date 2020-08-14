#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include "hi_unf_otp.h"
#include "hi_type.h"


void showusage()
{
    printf("Usage:\n"
           "To burn to nomal  chipset and lock id word: ./sample_otp_lockidword 0\n"
           "To burn to secure chipset and lock id word: ./sample_otp_lockidword 1\n"
           "To get idword lock flag                   : ./sample_otp_lockidword 2\n"
           );
}

HI_S32 main(int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bLockFlag = HI_FALSE;

    if (argc < 2)
    {
        showusage();
        return HI_SUCCESS;
    }


    ret = HI_UNF_OTP_Init();
	if(HI_SUCCESS != ret)
	{
		printf("OTP init failed!\n");
		return HI_FAILURE;
	}

    if (0 == atoi(argv[1]))
    {
        ret = HI_UNF_OTP_BurnToNormalChipset();
        if (HI_SUCCESS != ret)
        {
            printf("Burn to normal chipset failed\n");
            (HI_VOID)HI_UNF_OTP_DeInit();
            return HI_FAILURE;
        }

        printf("Burn to normal chipset success\n");
    }
    else if (1 == atoi(argv[1]))
    {
        ret = HI_UNF_OTP_BurnToSecureChipset();
        if (HI_SUCCESS != ret)
        {
            printf("Burn to secure chipset failed\n");
            (HI_VOID)HI_UNF_OTP_DeInit();
            return HI_FAILURE;
        }
            printf("Burn to secure chipset success\n");
    }
    else if (2 == atoi(argv[1]))
    {
        ret = HI_UNF_OTP_GetIDWordLockFlag(&bLockFlag);
        if (HI_SUCCESS != ret)
        {
            printf("Get ID word flag failed\n");
            (HI_VOID)HI_UNF_OTP_DeInit();
            return HI_FAILURE;
        }

    	if (HI_TRUE == bLockFlag)
    	{
    		printf("ID word has already been locked\n");
    	}
        else
        {
     		printf("ID word has not been locked\n");
        }
    }
    else
    {
        showusage();
    }


    (HI_VOID)HI_UNF_OTP_DeInit();

    return HI_SUCCESS;
}




