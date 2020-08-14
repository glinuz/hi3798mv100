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

#include <pthread.h>
#include <unistd.h>

#include <memory.h>
#include <sys/stat.h>
#include <dirent.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/vfs.h>

#include <errno.h>

#include "hi_adp.h"
#include "sample_fsdebug.h"
#include "sample_fsdisk.h"
#include "sample_fs.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

/**************** micro definition ****************/

//#define TEST_DEBUG

/******************* struct definition ******************/

/******************* global variable   ******************/
static HI_U32 sg_DirSndEntry = 0;
extern HI_CHAR usr_cmd[6];

/******************* function declaration   ******************/
HI_S32  test_fs_01();
HI_S32  test_fs_02();
HI_S32  test_fs_03();

static CaseItem_S DISKFS_LIST[4] =
{
    {"[DISKFS]Interface test of the system time",      test_fs_01},
    {"[DISKFS]Interface test of the folders in disks", test_fs_02},
    {"[DISKFS]Interface test of the files in disks",   test_fs_03},
    {"[DISKFS]Back",                   NULL      },
};

/*-----------------------------------------------------------------------------*
Function:        mFS_SystemDateSet
Description:
Calls:
Data Accessed:
Data Updated:
Input:
param
Output:
Return:
Others:
 *------------------------------------------------------------------------------*/
HI_S32 FS_SystemDateSet(HI_U32 year, HI_U32 month, HI_U32 day)
{
    time_t stTime;
    struct tm *p = NULL;
    struct timeval stTv;

    memset(&stTime, 0, sizeof(time_t));
    time(&stTime);
    p = localtime(&stTime);
    if (p == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_SystemDateGet \n");
        return HI_FAILURE;
    }

    p->tm_year = year - 1900;
    p->tm_mon  = month - 1;
    p->tm_mday = day;

    stTime = mktime(p);

    memset(&stTv, 0, sizeof(struct timeval));
    stTv.tv_sec = stTime;

    settimeofday(&stTv, NULL);

    return HI_SUCCESS;
}

HI_S32 FS_SystemTimeSet(HI_U32 hour, HI_U32 minute, HI_U32 second)
{
    time_t stTime;
    struct tm *p = NULL;
    struct timeval stTv;

    memset(&stTime, 0, sizeof(time_t));
    time(&stTime);
    p = localtime(&stTime);
    if (p == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_SystemDateGet \n");
        return HI_FAILURE;
    }

    p->tm_hour = hour;
    p->tm_min = minute;
    p->tm_sec = second;

    stTime = mktime(p);

    memset(&stTv, 0, sizeof(struct timeval));
    stTv.tv_sec = stTime;

    settimeofday(&stTv, NULL);

    return HI_SUCCESS;
}

HI_S32 FS_SystemDateGet(HI_U32 *year, HI_U32 *month, HI_U32 *day)
{
    time_t stTime;
    struct tm *p = NULL;

    MAPI_ASSERT(year != HI_NULL);
    MAPI_ASSERT(month != HI_NULL);
    MAPI_ASSERT(day != HI_NULL);

    memset(&stTime, 0, sizeof(time_t));
    time(&stTime);
    p = localtime(&stTime);
    if (p == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_SystemDateGet \n");
        return HI_FAILURE;
    }

    *year  = 1900 + p->tm_year;
    *month = 1 + p->tm_mon;
    *day = p->tm_mday;

    return HI_SUCCESS;
}

HI_S32 FS_SystemTimeGet(HI_U32 *hour, HI_U32 *minute, HI_U32 *second)
{
    time_t stTime;
    struct tm *p = NULL;
    
    MAPI_ASSERT(hour != HI_NULL);
    MAPI_ASSERT(minute != HI_NULL);
    MAPI_ASSERT(second != HI_NULL);

    memset(&stTime, 0, sizeof(time_t));
    time(&stTime);
    p = localtime(&stTime);
    if (p == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_SystemDateGet \n");
        return HI_FAILURE;
    }

    *hour   = p->tm_hour;
    *minute = p->tm_min;
    *second = p->tm_sec;

    return HI_SUCCESS;
}

/*-----------------------------------------------------------------------------*
Function:        FS_DirAttributesRead
Description:    
Calls:
Data Accessed:
Data Updated:
Input:
param
Output:
Return:
Others:      
 *------------------------------------------------------------------------------*/
HI_S32 FS_DirAttributesRead(HI_CHAR *dirName, HI_U32 *attributes)
{
     HI_S32 ret = -1;
     
    MAPI_ASSERT(dirName != HI_NULL);
    MAPI_ASSERT(attributes != HI_NULL);

    ret = chmod(dirName, 0777);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirAttributesRead return:%d\n", ret);
        return HI_FAILURE;
    }

    *attributes = 777;
    return HI_SUCCESS;
}

HI_S32 FS_DirAttributesSet(HI_CHAR *dirName, HI_U32 attributes)
{
     int ret = -1;

    MAPI_ASSERT(dirName != HI_NULL);

    ret = chmod(dirName, 0777);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirAttributesRead return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_DirCreate(HI_CHAR *dirName)
{
    HI_S32 ret;

    MAPI_ASSERT(dirName != HI_NULL);

    if (0 == access(dirName, F_OK))
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "DIR( %s ) has been existed!\n", dirName);
        return HI_SUCCESS;
    }

    ret = mkdir(dirName, 777);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mkdir return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_DirDelete(HI_CHAR *dirName)
{
    HI_S32 ret;

    MAPI_ASSERT(NULL != dirName);

    ret = access(dirName, F_OK);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "dir not exit\n");
        return HI_SUCCESS;
    }

    ret = rmdir(dirName);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "rmdir return:%d\n", ret);
        perror("rmdir ");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_DirRename(HI_CHAR *oldDirName, HI_CHAR *newDirName)
{
    HI_CHAR str[512] = {0};
    HI_S32 ret = -1;

    MAPI_ASSERT(NULL != oldDirName);
    MAPI_ASSERT(NULL != newDirName);

    ret = access(oldDirName, F_OK);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirRename fail. dir not exit\n");
        return HI_FAILURE;
    }

    sprintf(str, "mv %s %s", oldDirName, newDirName);
    ret = system(str);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirRename fail!return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_DirFirstEntryFind(HI_CHAR *entryName)
{
    HI_CHAR buf[512] = {0};
    HI_CHAR *pPath = NULL;
    DIR *dir = NULL;
    HI_CHAR tmpBuf[512] = {0};

    struct stat fileStruct;
    struct dirent* pEntry = NULL;

    MAPI_ASSERT(NULL != entryName);

    memset(buf, 0, sizeof(buf));
    pPath = getcwd(buf, sizeof(buf)); /*get current path*/
    if (pPath == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirDefaultGet fail\n");
        return HI_FAILURE;
    }

    if (0 != access(buf, F_OK))
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "err:NO file :%s!\n", buf);
        return HI_FAILURE;
    }

    dir = opendir(buf);
    if (NULL == dir)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "opendir error!\n");
        return HI_FAILURE;
    }

    pEntry = readdir(dir);
    while (pEntry)
    {
        if ((0 != strcmp(pEntry->d_name, ".")) && (0 != strcmp(pEntry->d_name, "..")))
        {
            memset(tmpBuf, 0x00, sizeof(tmpBuf));
            strcpy(tmpBuf, buf);
            strcat(tmpBuf, "/");
            strcat(tmpBuf, pEntry->d_name);
            if (stat(tmpBuf, &fileStruct) < 0)
            {
                hi_mapi_trace(MAPI_TRACE_ERROR, "stat   error!\n");
                return HI_FAILURE;
            }

            if (S_ISDIR(fileStruct.st_mode))
            {
                memcpy(entryName, pEntry->d_name, sizeof(pEntry->d_name));
                sg_DirSndEntry = 1;
                break;
            }
        }

        pEntry = readdir(dir);
    }

    if (pEntry == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "ERROR: have been come the end of DIR!\n");
        entryName = NULL;    /*search end in dir*/
        sg_DirSndEntry = 0;
    }

    closedir(dir);

    return HI_SUCCESS;
}

HI_S32 FS_DirNextEntryFind(HI_CHAR *entryName)
{
    HI_CHAR buf[512] = {0};
    HI_CHAR *pPath = NULL;
    DIR *dir = NULL;
    HI_CHAR tmpBuf[512] = {0};
    HI_S32 i = 0;

    struct   stat fileStruct;
    struct dirent* pEntry = NULL;

    MAPI_ASSERT(NULL != entryName);

    memset(buf, 0, sizeof(buf));
    pPath = getcwd(buf, sizeof(buf));
    if (pPath == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirDefaultGet fail\n");
        return HI_FAILURE;
    }

    if (0 != access(buf, F_OK))
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "err:NO file :%s!\n", buf);
        return HI_FAILURE;
    }

    dir = opendir(buf);
    if (NULL == dir)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "opendir error!\n");
        return HI_FAILURE;
    }

    pEntry = readdir(dir);
    while (pEntry) /*from one dir to another*/
    {
        if ((0 != strcmp(pEntry->d_name, ".")) && (0 != strcmp(pEntry->d_name, "..")))
        {
            memset(tmpBuf, 0x00, sizeof(tmpBuf));
            strcpy(tmpBuf, buf);
            strcat(tmpBuf, "/");
            strcat(tmpBuf, pEntry->d_name);
            if (stat(tmpBuf, &fileStruct) < 0)
            {
                perror("stat fail: ");
                hi_mapi_trace(MAPI_TRACE_ERROR, "stat   error!\n");
                return HI_FAILURE;
            }

            if (S_ISDIR(fileStruct.st_mode))
            {
                if (i != sg_DirSndEntry)
                {
                    i++;
                    pEntry = readdir(dir);
                    continue;
                }

                memcpy(entryName, pEntry->d_name, sizeof(pEntry->d_name));
                sg_DirSndEntry++;
                break;
            }
        }

        pEntry = readdir(dir);
    }

    if (pEntry == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "NOTE: Serching arrive at  the end of the directory!\n");
        *entryName = '\0';
        entryName = NULL;
        sg_DirSndEntry = 0;
    }

    closedir(dir);

    return HI_SUCCESS;
}

HI_S32 FS_DirNameTest(HI_CHAR *dirName)
{
    /*confirm*/
    int ret = -1;

    ret = access(dirName, F_OK);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "DIR doesnt exit\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_DirInformationGet(HI_CHAR *dirName, MFS_DIRENTRY_INFO *dirEntryInfo)
{
    HI_S32 ret = -1;
    struct stat stDirStat;
    time_t stTime;
    struct tm *p  = NULL;
    HI_CHAR buf[512] = {0};
    HI_CHAR *pPath = NULL;

    MAPI_ASSERT(NULL != dirEntryInfo);
    MAPI_ASSERT(NULL != dirName);

    memset(&stDirStat, 0, sizeof(struct stat));
    ret = stat(dirName, &stDirStat);
    if (0 != ret)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirInformationGet fail:%d\n", ret);
        return HI_FAILURE;
    }

    dirEntryInfo->size = stDirStat.st_size;
    dirEntryInfo->attributes = (HI_U32)stDirStat.st_mode;

    memset(&stTime, 0, sizeof(time_t));
    stTime = stDirStat.st_mtime; /*the last modify time*/
    p = localtime(&stTime);
    if (p == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirInformationGet \n");
        return HI_FAILURE;
    }

    dirEntryInfo->year  = 1900 + p->tm_year;
    dirEntryInfo->month = 1 + p->tm_mon;
    dirEntryInfo->day    = p->tm_mday;
    dirEntryInfo->hour   = p->tm_hour;
    dirEntryInfo->minute = p->tm_min;
    dirEntryInfo->second = p->tm_sec;

    memset(&buf, 0, strlen(buf));

    pPath = getcwd(buf, sizeof(buf));
    if (pPath == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "getwd fail\n");
        return HI_FAILURE;
    }

    strcat(buf, "/");
    strcat(buf, dirName);
    if (stat(buf, &stDirStat) < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "stat   error!\n");
        return HI_FAILURE;
    }

    strcpy(dirEntryInfo->fxDirLongName, buf);
    strcpy(dirEntryInfo->fxDirLongFileDoubleName, buf); /*confirm ???*/

    return HI_SUCCESS;
}

HI_S32 FS_DirDefaultSet(HI_CHAR *newPathName)
{
    HI_S32 ret = -1;
    HI_CHAR *pathname;

    pathname = malloc(MFS_MAX_LONG_NAME_LEN);
    if (pathname == NULL)
    {
        return HI_FAILURE;
    }

    memset(pathname, 0, MFS_MAX_LONG_NAME_LEN);
    strcat(pathname, newPathName);

    ret = chdir(pathname);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "cd %s fail\n", pathname);
        free(pathname);
        return HI_FAILURE;
    }

    free(pathname);
    return HI_SUCCESS;
}

/*confirm **
 *char **   */
HI_S32 FS_DirDefaultGet(HI_CHAR **returnPathName)
{
    char buf[512] = {0};
    char *pPath = NULL;

    memset(buf, 0, sizeof(buf));
    pPath = getcwd(buf, sizeof(buf));
    if (pPath == NULL)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirDefaultGet fail\n");
        return HI_FAILURE;
    }
    strcpy((char *)returnPathName, buf);
    return HI_SUCCESS;
}

HI_S32 FS_FileCreate(HI_CHAR *fileName)
{
    HI_S32 ret = -1;
    HI_S32 fd;

    MAPI_ASSERT(NULL != fileName);

    ret = access(fileName, F_OK);
    if (ret == 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "file has been exit\n");
        return HI_SUCCESS;
    }

    fd = creat(fileName, 777);
    if (fd < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "create file fail\n");
        return HI_FAILURE;
    }

    close(fd);

    return HI_SUCCESS;
}

HI_S32 FS_FileDelete(HI_CHAR *fileName)
{
    char str[512] = {0};
    int ret = -1;

    MAPI_ASSERT(NULL != fileName);

    ret = access(fileName, F_OK);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "file not exit\n");
        return HI_SUCCESS;
    }

    sprintf(str, "rm -fr %s", fileName);
    ret = system(str);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_FileDelete fail! return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileRename(HI_CHAR *oldFileName, HI_CHAR *newFileName)
{
    HI_CHAR str[512] = {0};
    HI_S32 ret = -1;

    MAPI_ASSERT(NULL != oldFileName);

    ret = access(oldFileName, F_OK);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "file not exit\n");
        return HI_FAILURE;
    }

    sprintf(str, "mv %s %s", oldFileName, newFileName);
    ret = system(str);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_FileRename fail!return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileAttributesSet(HI_CHAR *fileName, HI_U32 attributes)
{
    HI_S32 ret = -1;

    ret = chmod(fileName, 0777);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirAttributesRead return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileAttributesRead(HI_CHAR *fileName, HI_U32 *attributes)
{
    HI_S32 ret = -1;

    ret = chmod(fileName, 0777);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_DirAttributesRead return:%d\n", ret);
        return HI_FAILURE;
    }

    *attributes = 777;
    return HI_SUCCESS;
}

HI_S32 FS_FileOpen(HI_S32  *fileDesc, HI_CHAR *fileName, HI_U32 openType)
{
    HI_S32 fd = 0;
    HI_S32 fg = 0;

    MAPI_ASSERT(NULL != fileName);

    if (openType == MFS_OPEN_FOR_READ)
    {
        fg |= O_RDWR;
    }                                              
    if (openType == MFS_OPEN_FOR_WRITE)
    {
        fg |= O_RDWR;
    }

    fd = open(fileName, fg, 0777);
    if (fd < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "file:%s open return:%d\n", fileName, fd);
        return HI_FAILURE;
    }

    *fileDesc = fd;
    return HI_SUCCESS;
}

HI_S32 FS_FileClose(HI_S32 fileDesc)
{
    HI_S32 ret;

    ret = close(fileDesc);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "close return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileRead(HI_S32 fileDesc, void  *buffer, HI_U32 requestSize, HI_U32 *actualSize)
{
    HI_S32 ret = -1;

    MAPI_ASSERT(buffer != NULL);

    ret = read(fileDesc, buffer, requestSize);
    if (ret < 0)
    {
        perror("read file error: ");
        hi_mapi_trace(MAPI_TRACE_ERROR, "read return:%d\n", ret);
        return HI_FAILURE;
    }

    *actualSize = (HI_U32)ret;

    return HI_SUCCESS;
}

HI_S32 FS_FileWrite(HI_S32 fileDesc, void *buffer, HI_U32 size)
{
    HI_S32 ret = -1;

    MAPI_ASSERT(buffer != NULL);

    ret = write(fileDesc, buffer, size);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "write return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileRelativeSeek(HI_S32 fileDesc, HI_U32 byteOffset, HI_U32 seekFrom)
{
    HI_S32 ret = -1;
    HI_U32 seekopt;
    off_t offset;

    switch (seekFrom)
    {
    case MFS_SEEK_BEGIN:
        seekopt = SEEK_SET;
        offset = (off_t)byteOffset;
        break;
    case MFS_SEEK_END:
        seekopt = SEEK_END;
        offset = (off_t)byteOffset;
        break;
    case MFS_SEEK_FORWARD:
        seekopt = SEEK_CUR;
        offset = (off_t)byteOffset;
        break;
    case MFS_SEEK_BACK:
        seekopt = SEEK_CUR;
        offset = (off_t)0x0 - (off_t)byteOffset;      /*negative*/
        break;
    default:
        seekopt = SEEK_SET;
        offset = (off_t)byteOffset;
        break;
    }

    ret = lseek(fileDesc, offset, seekopt);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "lseek return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileSeek(HI_S32 fileDesc, HI_U32 byteOffset)
{
    int ret = -1;

    ret = lseek(fileDesc, byteOffset, 0);
    if (ret < 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "lseek return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_FileTruncate(HI_S32 fileDesc, HI_U32 size)
{
    int ret = -1;

    ret = ftruncate(fileDesc, size);
    if (ret != 0)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "mFS_FileTruncate return:%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 FS_MediaFlush(void)
{
    if (0 == access(USB_MOUNT_POINT, F_OK))
    {
        sync();

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 FS_MediaSpaceAvailable  (HI_U32 *availableBytes)
{
    struct statfs fsstat;

    if (0 != statfs(USB_MOUNT_POINT, &fsstat))
    {
        printf("get file state error!\n");
        return HI_FAILURE;
    }

    *availableBytes = fsstat.f_bfree * fsstat.f_bsize / 1024;
    return HI_SUCCESS;
}

HI_S32 test_diskFs_list(void)
{
    HI_S32 run_case_num = 0;
    HI_S32 total_case = 0;

    total_case = (int)(sizeof(DISKFS_LIST) / sizeof(DISKFS_LIST[0]));

    while (1)
    {
        SHOW_LIST(DISKFS_LIST);
        GetUsrInput();

        run_case_num = atoi(usr_cmd);
        if ((run_case_num < 1) || (run_case_num > total_case))
        {
            printf("invalid cmd, should be [1 - %02d]\n", total_case);
            continue;
        }

        if (run_case_num == total_case)  //back
        {
            break;
        }

        DISKFS_LIST[run_case_num - 1].func();
    }

    return HI_SUCCESS;
}

/*time test*/
HI_S32 test_fs_01()
{
    HI_U32 year;
    HI_U32 month;
    HI_U32 day;
    HI_U32 hour;
    HI_U32 minute;
    HI_U32 second;

    HI_U32 u32Getyear;
    HI_U32 u32Getmonth;
    HI_U32 u32Getday;
    HI_U32 u32Gethour;
    HI_U32 u32Getminute;
    HI_U32 u32Getsecond;

    HI_U32 ret;

    year  = 2009;
    month = 6;
    day    = 26;
    hour   = 12;
    minute = 30;
    second = 30;

    ret  = FS_SystemDateSet(year, month, day);
    ret |= FS_SystemTimeSet(hour, minute, second);
    if (ret |= HI_SUCCESS)
    {
        printf("set date and time fail\n");
        return HI_FAILURE;
    }

    ret  = FS_SystemDateGet(&u32Getyear, &u32Getmonth, &u32Getday);
    ret |= FS_SystemTimeGet(&u32Gethour, &u32Getminute, &u32Getsecond);
    if (ret |= HI_SUCCESS)
    {
        printf("get date and time fail\n");
        return HI_FAILURE;
    }

    if ((year == u32Getyear) && (month == u32Getmonth) && (day == u32Getday))
    {
        printf("get date success\n");
    }
    else
    {
        printf("today is %d:%d:%d \n", u32Getyear, u32Getmonth, u32Getday);
        return HI_FAILURE;
    }

    if ((hour == u32Gethour) && (minute == u32Getminute) && (second == u32Getsecond))
    {
        printf("get time success\n");
    }
    else
    {
        printf("now is %d:%d:%d \n", u32Gethour, u32Getminute, u32Getsecond);
        return HI_FAILURE;
    }

    printf("test 01 ok \n");
    return HI_SUCCESS;
}

/*dir test*/
HI_S32 test_fs_02()
{
    HI_S32 ret;
    HI_CHAR PathName[10][200] = {{0}};
    HI_U32 attributes;
    MFS_DIRENTRY_INFO dirEntryInfo;
    HI_CHAR DirName[200] = {0};
    HI_CHAR *pDirName = NULL;

    pDirName = DirName;
    memset(DirName, 0, sizeof(DirName));

    ret = FS_DirDefaultGet((char **)PathName);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDefaultGet fail \n");
        return HI_FAILURE;
    }
    else
    {
        //printf("current DIR is %s \n",PathName[0]);
    }

    if (0 != access(USB_MOUNT_POINT, F_OK))
    {
        printf("err:NO file :%s!\n", USB_MOUNT_POINT);
        return HI_FAILURE;
    }

    ret = FS_DirDefaultSet(USB_MOUNT_POINT);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDefaultSet fail \n");
        return HI_FAILURE;
    }

    ret  = FS_DirCreate("DIR1");
    ret |= FS_DirCreate("DIR2");
    ret |= FS_DirCreate("DIR3");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirCreate fail \n");
        return HI_FAILURE;
    }

    ret = FS_DirDelete("DIR3");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDelete fail \n");
        return HI_FAILURE;
    }

    ret  = FS_DirRename("DIR1", "DIR4");
    ret |= FS_DirRename("DIR2", "DIR5");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDelete fail \n");
        return HI_FAILURE;
    }

    ret  = FS_DirAttributesSet("DIR4", 777);
    ret |= FS_DirAttributesRead("DIR4", &attributes);
    if ((ret != HI_SUCCESS) && (attributes != 777))
    {
        printf("mFS_DirAttributesSet fail \n");
        return HI_FAILURE;
    }

    ret = FS_DirNameTest("DIR4");
    ret |= FS_DirNameTest("DIR5");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirNameTest fail \n");
        return HI_FAILURE;
    }

    memset(&dirEntryInfo, 0, sizeof(MFS_DIRENTRY_INFO));
    dirEntryInfo.fxDirLongName = PathName[0];
    dirEntryInfo.fxDirLongFileDoubleName = PathName[1];
    ret = FS_DirInformationGet("DIR5", &dirEntryInfo);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirInformationGet fail \n");
        return HI_FAILURE;
    }

#ifdef TEST_DEBUG
    printf("DIR5 last ment time:%d:%d:%d, %d:%d:%d\n", dirEntryInfo.year, \
           dirEntryInfo.month, \
           dirEntryInfo.day, \
           dirEntryInfo.hour, \
           dirEntryInfo.minute, \
           dirEntryInfo.second);
    printf("DIR5 attributes = %d\n", dirEntryInfo.attributes);
    printf("DIR5 size = %lu\n", dirEntryInfo.size);
    printf("DIR5 fxDirLongName = %s\n", dirEntryInfo.fxDirLongName);
    printf("DIR5 fxDirLongFileDoubleName = %s\n", dirEntryInfo.fxDirLongFileDoubleName);
#endif


#if 0
    ret = mFS_DirDefaultSet("DIR4"); /*change to this directory */
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDefaultGet fail \n");
        return ML_FAIL;
    }

    ret  = mFS_DirCreate("DIR1");
    ret |= mFS_DirCreate("DIR2");
    ret |= mFS_DirCreate("DIR3");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirCreate fail \n");
        return ML_FAIL;
    }
#endif


    ret = FS_DirFirstEntryFind(pDirName);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirFirstEntryFind fail \n");
        return HI_FAILURE;
    }

#if TEST_DEBUG
    if (pDirName != NULL)
    {
        printf("DirName = %s\n", pDirName);
    }
#endif


    while (*pDirName)
    {
        ret = FS_DirNextEntryFind(pDirName);
        if (ret != HI_SUCCESS)
        {
            printf("mFS_DirFirstEntryFind fail \n");
            return HI_FAILURE;
        }

#if TEST_DEBUG
        if (*pDirName != (char)NULL)
        {
            printf("DirName = %s\n", pDirName);
        }
#endif

    }

    printf("test_fs_02 ok \n");
    return HI_SUCCESS;
}

/*file test*/
HI_S32 test_fs_03()
{
    HI_S32 ret;
    HI_U32 attributes;
    HI_S32 fd;
    HI_CHAR buf[100] = {0};
    HI_U32 actualSize;
    HI_CHAR GetBuf[100] = {0};
    HI_CHAR PathName[10][200] = {{0}};
    MFS_DIRENTRY_INFO dirEntryInfo;

    if (0 != access(USB_MOUNT_POINT, F_OK))
    {
        printf("err:NO file :%s!\n", USB_MOUNT_POINT);
        return HI_FAILURE;
    }

    ret = FS_DirDefaultSet(USB_MOUNT_POINT); 
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDefaultGet fail \n");
        return HI_FAILURE;
    }

    ret = FS_DirDefaultSet("DIR4");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirDefaultSet fail \n");
        return HI_FAILURE;
    }

    ret  = FS_FileCreate("file1.mp3");
    ret |= FS_FileCreate("file2.mp3");
    ret |= FS_FileCreate("file3.jpg");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileCreate fail \n");
        return HI_FAILURE;
    }

    ret = FS_FileDelete("file2.mp3");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileDelete fail \n");
        return HI_FAILURE;
    }

    ret = FS_FileRename("file1.mp3", "file4.jpg");
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileDelete fail \n");
        return HI_FAILURE;
    }

    ret  = FS_FileAttributesSet("file4.jpg", 777);
    ret |= FS_FileAttributesRead("file4.jpg", &attributes);
    if ((ret != HI_SUCCESS) && (attributes != 777))
    {
        printf("mFS_FileAttributesRead fail \n");
        return HI_FAILURE;
    }

    memset(buf, 0x1, sizeof(buf));
    ret  = FS_FileOpen(&fd, "file4.jpg", MFS_OPEN_FOR_WRITE);
    ret |= FS_FileWrite(fd, buf, sizeof(buf));
    ret |= FS_FileSeek(fd, 0);
    ret |= FS_FileRead(fd, GetBuf, sizeof(GetBuf), &actualSize);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileDelete fail \n");
        return HI_FAILURE;
    }

    ret = strncmp(buf, GetBuf, sizeof(buf));
    if (ret != 0)
    {
        printf("compare fail\n");
        return HI_FAILURE;
    }

    memset(buf, 0x0, sizeof(buf));
    ret  = FS_FileRelativeSeek(fd, sizeof(buf), MFS_SEEK_BEGIN);
    ret |= FS_FileWrite(fd, buf, sizeof(buf));
    ret |= FS_FileRelativeSeek(fd, sizeof(buf), MFS_SEEK_BACK);
    ret |= FS_FileRead(fd, GetBuf, sizeof(GetBuf), &actualSize);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileDelete fail \n");
        return HI_FAILURE;
    }

    ret = strncmp(buf, GetBuf, sizeof(buf));
    if (ret != 0)
    {
        printf("compare fail\n");
        return HI_FAILURE;
    }

    ret = FS_FileTruncate(fd, 150);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileTruncate fail \n");
        return HI_FAILURE;
    }

    ret = FS_FileClose(fd);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_FileClose fail \n");
        return HI_FAILURE;
    }

    memset(&dirEntryInfo, 0, sizeof(MFS_DIRENTRY_INFO));
    dirEntryInfo.fxDirLongName = PathName[0];
    dirEntryInfo.fxDirLongFileDoubleName = PathName[1];
    ret = FS_DirInformationGet("file4.jpg", &dirEntryInfo);
    if (ret != HI_SUCCESS)
    {
        printf("mFS_DirInformationGet fail \n");
        return HI_FAILURE;
    }

    if (dirEntryInfo.size != 150)
    {
        printf("mFS_FileTruncate fail\n");
        return HI_FAILURE;
    }

    printf("test_fs_03 ok\n");
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */
