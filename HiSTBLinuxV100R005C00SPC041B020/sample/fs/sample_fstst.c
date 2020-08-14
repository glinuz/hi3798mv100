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
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "hi_adp.h"
#include "sample_fsdebug.h"
#include "sample_fstst.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

HI_S32 DataWriteToDisk(void)
{
    HI_S32 ret;

    ret = system("dd if=/dev/zero of=hi.img count=10240 bs=4096;sync;");
    if (ret < 0)
    {
        printf("%d@%s, dd fail\n", __LINE__, __func__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DataReadFromDisk(void)
{
    HI_S32 ret;

    ret = system("dd if=hi.img of=/dev/zero count=10240 bs=4096;sync;");
    if (ret < 0)
    {
        printf("%d@%s, dd fail\n", __LINE__, __func__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 test_diskPeformance_list(void)
{
    HI_S32 ret = HI_FAILURE;
    struct timeval tm_now, tm_last;
    HI_S32 u32mSecond;
    Disk_PERFORMANCE_INFO_S stPerformanceInfo = {0};

    /* read rate test*/
    gettimeofday(&tm_last, NULL);
    ret = DataWriteToDisk();
    if (HI_SUCCESS != ret)
    {
        printf("DataWriteToDisk fail!\n");
        return HI_FAILURE;
    }

    gettimeofday(&tm_now, NULL);
    u32mSecond = ((tm_now.tv_sec - tm_last.tv_sec) * 1000 + tm_now.tv_usec / 1000 - tm_last.tv_usec / 1000);
    stPerformanceInfo.u32WriteDiskRate = 10240 * 4096 / u32mSecond;

    /* write rate test*/
    gettimeofday(&tm_last, NULL);
    ret = DataReadFromDisk();
    if (HI_SUCCESS != ret)
    {
        printf("DataWriteToDisk fail!\n");
        return HI_FAILURE;
    }

    gettimeofday(&tm_now, NULL);
    u32mSecond = ((tm_now.tv_sec - tm_last.tv_sec) * 1000 + tm_now.tv_usec / 1000 - tm_last.tv_usec / 1000);
    stPerformanceInfo.u32ReadDiskRate = 10240 * 4096 / u32mSecond;

    printf("============================================\n");
    printf("*Result: Write Disk Rate = %d KBps\n", stPerformanceInfo.u32WriteDiskRate);
    printf("*Result: Read Disk Rate = %d KBps\n", stPerformanceInfo.u32ReadDiskRate);
    printf("============================================\n");
    return HI_SUCCESS;
}

HI_S32 test_fdisk_list(void)
{
    printf("%s come\n", __func__);
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */
