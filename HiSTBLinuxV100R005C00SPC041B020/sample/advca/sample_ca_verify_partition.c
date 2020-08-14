#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "authdefine.h"

#include "hi_common.h"
#include "hi_unf_wdg.h"

HI_S32 main(int argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32WdgNum = 0;

    ret = HI_SYS_Init();
    if (HI_FAILURE == ret)
    {
        SAMPLE_PRINTF("HI_SYS_Init() failed [%s:%d]\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }

    if (4 == argc)
    {
        HI_U32 offset = 0;
        offset = (HI_U32)strtoul((const char*)argv[3], 0, 16);
        ret = CA_common_authPartition(argv[1], argv[2], offset);
    }
    else if (3 == argc)
    {
        ret = CA_common_authPartition(argv[1], argv[2], 0);
    }
    else if (2 == argc)  /* kernel, recovery etc. */
    {
        ret = CA_common_authPartition(argv[1], argv[1], 0);
    }
    else
    {
        SAMPLE_PRINTF("Invalid parameter!, Useage: \n");
        SAMPLE_PRINTF("     verify_parttion system systemsign 0x0\n");
        SAMPLE_PRINTF("     verify_parttion system systemsign\n");
        SAMPLE_PRINTF("  or other partiton kernel recovery etc.\n");
        SAMPLE_PRINTF("     verify_parttion kernel\n");

        return 0;
    }

    if (HI_SUCCESS == ret)
    {
        SAMPLE_PRINTF("Authenitation success!\n");
    }
    else
    {
        SAMPLE_PRINTF("\nError: Authenitation Failure!\n");
        SAMPLE_PRINTF("Please Reboot!!!!!!!!!\n");
        sleep(1);
        system("reboot");
    }

    return 0;
}

