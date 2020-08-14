/**********************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:
 * Description:
 *
 * History:
 * Version          Date             Author    DefectNum    Description
 * V1.00     May24th,2010        f48792     NA              1 st release
 ***********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hi_adp.h"
#include "sample_fs.h"
#include "sample_fsmanager.h"
#include "sample_fstst.h"
#include "sample_fsdisk.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

HI_CHAR usr_cmd[6] = {0};

static CaseItem_S sg_main_list[5] =
{
    {"Performance test of disks",                  test_diskPeformance_list},
    {"Test of formatting disks",                   test_fdisk_list         },
    {"Test of reading and writing files in disks", test_diskFs_list        },
    {"Test of searching files in disks",           test_diskManage_list    },
    {"Quit",                                       NULL                    },
};

void GetUsrInput(void)
{
    memset(usr_cmd, 0, sizeof(usr_cmd));
    read(0, usr_cmd, sizeof(usr_cmd));
    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 run_case_num = 0;
    HI_S32 total_case = 0;

    total_case = (int)(sizeof(sg_main_list) / sizeof(sg_main_list[0]));

    SHOW_WELCOME();

    if (HI_SUCCESS != FS_Init())
    {
        printf("Init fail\n");
        return (-1);
    }

    printf("Disk Check... \n");
    sleep(2);

    if (HI_TRUE != FS_IsDiskOk())
    {
        printf("No disk device, please PLUG disk ... \n");
    }

    /*now enter into sub test*/
    while (1)
    {
#if 0
        if (ML_TRUE != mUSB_IsDiskOk)
        {
            sleep(1);
            continue;
        }
#endif


        SHOW_LIST(sg_main_list);
        GetUsrInput();

        run_case_num = atoi(usr_cmd);
        if ((run_case_num < 1) || (run_case_num > total_case))
        {
            printf("invalid cmd, should be [1 - %02d]\n", total_case);
            continue;
        }

        if (run_case_num == total_case)  //quit
        {
            exit(0);
        }

        sg_main_list[run_case_num - 1].func();
    }
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
