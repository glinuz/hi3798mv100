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

#ifndef  _SAMPLE_FSTST_H_
#define  _SAMPLE_FSTST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    HI_S32 u32WriteDiskRate; //KBPS
    HI_S32 u32ReadDiskRate;  //KBPS
} Disk_PERFORMANCE_INFO_S;

HI_S32 DataWriteToDisk(void);
HI_S32 DataReadFromDisk(void);

HI_S32 test_diskPeformance_list(void);
HI_S32 test_fdisk_list(void);

#ifdef __cplusplus
}
#endif

#endif
