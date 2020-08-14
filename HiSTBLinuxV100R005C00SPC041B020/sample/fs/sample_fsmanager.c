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
#include <unistd.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/vfs.h>

#include "hi_adp.h"
#include "sample_fsdebug.h"
#include "sample_fsdisk.h"
#include "sample_fsmanager.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

/**************** micro definition ****************/
#define MAX_USB_DATA_NUM 200 /*mp3,jpg...*/

/******************* struct definition ******************/

/******************* global variable   ******************/
static HI_BOOL sg_bDevValid = HI_FALSE;
static HI_BOOL sg_mUSB_InitFlag = HI_FALSE;

/******************* function declaration   ******************/
void *     FS_TaskUSB(void);

/*-----------------------------------------------------------------------------*
Function:        mUSB_FsFolderContentInit
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
HI_S32 FS_Init(void)
{
    pthread_t pid = 0;
    HI_S32 ret = -1;

    if (HI_TRUE == sg_mUSB_InitFlag)
    {
        return HI_SUCCESS;
    }

    ret = pthread_create(&pid, NULL, (void *)FS_TaskUSB, NULL);
    if (0 != ret)
    {
        printf("hiMonitor_TaskUSB create error!\n");
        return HI_FAILURE;
    }

    sg_mUSB_InitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/*-----------------------------------------------------------------------------*
Function:        FSM_GetFilesNumByPath
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
HI_S32 FS_GetFilesNumByPath(HI_CHAR* pPath, HI_U32* puFolderNum, HI_U32* puMp3Num, HI_U32* puJpegNum)
{
    USB_FILE_NAME FolderName[MAX_USB_DATA_NUM];
    USB_FILE_NAME MP3Name[MAX_USB_DATA_NUM];
    USB_FILE_NAME JpgName[MAX_USB_DATA_NUM];
    USB_FILE_NAME AviName[MAX_USB_DATA_NUM];
    HI_S32 FolderNum = 0;
    HI_S32 MP3Num = 0;
    HI_S32 JpgNum = 0;
    HI_S32 AviNum = 0;

    HI_CHAR *pSuffix = NULL;
    HI_CHAR subFilePath[MFS_MAX_LONG_NAME_LEN] = {0};
    HI_CHAR filePath[MFS_MAX_LONG_NAME_LEN] = {0};
    DIR *dir = NULL;

    struct   stat fileStruct;
    struct dirent* pEntry = NULL;

    MAPI_ASSERT(NULL != puFolderNum);
    MAPI_ASSERT(NULL != puMp3Num);
    MAPI_ASSERT(NULL != puJpegNum);

#if 0
    if (0 != access("/dev/scsi/host0/bus0/target0/lun0/part1", F_OK))
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "err:NO USB DEV!\n");
        return ML_FAIL;
    }
#endif

    strcpy(filePath, USB_MOUNT_POINT);
    strcat(filePath, pPath);

    if (0 != access(filePath, F_OK))
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "err:NO file :%s!\n", filePath);
        return HI_FAILURE;
    }

    dir = opendir(filePath);
    if (NULL == dir)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "opendir error!\n");
        return HI_FAILURE;
    }

    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);

    pEntry = readdir(dir);
    while (pEntry)
    {
        if ((0 != strcmp(pEntry->d_name, ".")) && (0 != strcmp(pEntry->d_name, "..")))
        {
            memset(&subFilePath, 0x00, strlen(subFilePath));

            strcpy(subFilePath, filePath);
            strcat(subFilePath, "/");
            strcat(subFilePath, pEntry->d_name);
            if (stat(subFilePath, &fileStruct) < 0)
            {
                hi_mapi_trace(MAPI_TRACE_ERROR, "stat   error!\n");
                return HI_FAILURE;
            }

            if (S_ISDIR(fileStruct.st_mode))
            {
                memcpy(FolderName[FolderNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                FolderNum++;
            }

            if ((pSuffix = strrchr ((char*)pEntry->d_name, '.')) != NULL)
            {
                if (!(strncmp(pSuffix, ".jpg", 4)) || (!strncmp(pSuffix, ".JPG", 4))
                    || !(strncmp(pSuffix, ".jpeg", 5)) || (!strncmp(pSuffix, ".JPEG", 5)))
                {
                    memcpy(JpgName[JpgNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    JpgNum++;
                }
                else if ((!(strncmp(pSuffix, ".avi", 4))) || (!(strncmp(pSuffix, ".AVI", 4)))
                         || (!(strncmp(pSuffix, ".mov", 4))) || (!(strncmp(pSuffix, ".MOV", 4))))
                {
                    memcpy(AviName[AviNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    AviNum++;
                }
                else if (!(strncmp(pSuffix, ".mp3", 4)) || (!strncmp(pSuffix, ".MP3", 4)))
                {
                    memcpy(MP3Name[MP3Num].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    MP3Num++;
                }
            }
        }

        pEntry = readdir(dir);

        /*file num should be less MAX_USB_DATA_NUM*/
        if ((MP3Num > MAX_USB_DATA_NUM) || (JpgNum > MAX_USB_DATA_NUM) || (FolderNum > MAX_USB_DATA_NUM)
            || (AviNum > MAX_USB_DATA_NUM))
        {
            hi_mapi_trace(MAPI_TRACE_WARNING, "TOO LARGE FILE NUMBER!\n");
            break;
        }
    }

    closedir(dir);

    *puFolderNum = FolderNum;
    *puJpegNum = JpgNum;
    *puMp3Num = MP3Num;

    return HI_SUCCESS;
}

/*-----------------------------------------------------------------------------*
Function:        mUSB_GetFilesContentByPath
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
HI_S32 FS_GetFilesContentByPath(HI_CHAR* pPath, USB_FILE_NAME** pFolder, USB_FILE_NAME** pMp3, USB_FILE_NAME** pJpeg)
{
    USB_FILE_NAME FolderName[MAX_USB_DATA_NUM];
    USB_FILE_NAME MP3Name[MAX_USB_DATA_NUM];
    USB_FILE_NAME JpgName[MAX_USB_DATA_NUM];
    USB_FILE_NAME AviName[MAX_USB_DATA_NUM];
    HI_S32 FolderNum = 0;
    HI_S32 MP3Num = 0;
    HI_S32 JpgNum = 0;
    HI_S32 AviNum = 0;

    HI_CHAR *pSuffix = NULL;
    HI_CHAR subFilePath[MFS_MAX_LONG_NAME_LEN] = {0};
    HI_CHAR filePath[MFS_MAX_LONG_NAME_LEN] = {0};
    DIR *dir = NULL;

    struct   stat fileStruct;
    struct dirent* pEntry = NULL;

    MAPI_ASSERT(NULL != pFolder);
    MAPI_ASSERT(NULL != pMp3);
    MAPI_ASSERT(NULL != pJpeg);

    strcpy(filePath, USB_MOUNT_POINT);
    strcat(filePath, pPath);
    dir = opendir(filePath);
    if (NULL == dir)
    {
        hi_mapi_trace(MAPI_TRACE_ERROR, "opendir error!\n");
        return HI_FAILURE;
    }

    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);
    memset(FolderName, 0, sizeof(USB_FILE_NAME) * MAX_USB_DATA_NUM);

    pEntry = readdir(dir);
    while (pEntry)
    {
        if ((0 != strcmp(pEntry->d_name, ".")) && (0 != strcmp(pEntry->d_name, "..")))
        {
            memset(&subFilePath, 0x00, strlen(subFilePath));

            strcpy(subFilePath, filePath);
            strcat(subFilePath, "/");
            strcat(subFilePath, pEntry->d_name);
            if (stat(subFilePath, &fileStruct) < 0)
            {
                hi_mapi_trace(MAPI_TRACE_ERROR, "lstat   error!\n");
                return HI_FAILURE;
            }

            if (S_ISDIR(fileStruct.st_mode))
            {
                memcpy(FolderName[FolderNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                FolderNum++;
            }

            if ((pSuffix = strrchr ((char*)pEntry->d_name, '.')) != NULL)
            {
                if (!(strncmp(pSuffix, ".jpg", 4)) || (!strncmp(pSuffix, ".JPG", 4))
                    || !(strncmp(pSuffix, ".jpeg", 5)) || (!strncmp(pSuffix, ".JPEG", 5)))
                {
                    memcpy(JpgName[JpgNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    JpgNum++;
                }
                else if ((!(strncmp(pSuffix, ".avi", 4))) || (!(strncmp(pSuffix, ".AVI", 4)))
                         || (!(strncmp(pSuffix, ".mov", 4))) || (!(strncmp(pSuffix, ".MOV", 4))))
                {
                    memcpy(AviName[AviNum].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    AviNum++;
                }
                else if (!(strncmp(pSuffix, ".mp3", 4)) || (!strncmp(pSuffix, ".MP3", 4)))
                {
                    memcpy(MP3Name[MP3Num].uFileName, pEntry->d_name, sizeof(pEntry->d_name));
                    MP3Num++;
                }
            }
        }

        pEntry = readdir(dir);

        if ((MP3Num > MAX_USB_DATA_NUM) || (JpgNum > MAX_USB_DATA_NUM) || (FolderNum > MAX_USB_DATA_NUM)
            || (AviNum > MAX_USB_DATA_NUM))
        {
            hi_mapi_trace(MAPI_TRACE_ERROR, "TOO LARGE USB FILE NUMBER !\n");
            break;
        }
    }

    closedir(dir);

    memcpy(pFolder, FolderName, sizeof(USB_FILE_NAME) * FolderNum);
    memcpy(pJpeg, JpgName, sizeof(USB_FILE_NAME) * JpgNum);
    memcpy(pMp3, MP3Name, sizeof(USB_FILE_NAME) * MP3Num);

    return HI_SUCCESS;
}

/*
 * USB scan task, if USB dev is inserted or removed, sent message to usb_test
 */
void * FS_TaskUSB(void)
{
    HI_BOOL bDevValidTmp = HI_FALSE;
    USB_DISK_INFO_S stDiskInfo = {0};
    HI_CHAR str[512] = {0};
    HI_CHAR diskname[40];
    HI_S32 ret;
    HI_S32 i;
    char disklist[][40]={
		"/dev/sda1",
		"/dev/sda",
		"/dev/sdb1",
		"/dev/sdb",
		"null",
    	};

    while (1)
    {
        usleep(200000);
	  
        i=0;
        while(strcmp(disklist[i],"null")	!=0)
        {
   	    if (0 == access(disklist[i], F_OK))
   	    {
   	  //  	 printf("Find disk %s!\n",disklist[i]);
   		 strcpy(diskname,disklist[i]);
   	        break;
   	    }
   	    i++;
        }
    	
        if(strcmp(disklist[i],"null")==0)
        {
    //        printf("No disk found!\n");
            bDevValidTmp = HI_FALSE;
        }
	  else
	  {
            bDevValidTmp = HI_TRUE;
	  }

        if (HI_FALSE == bDevValidTmp)
        {
            if (HI_TRUE == sg_bDevValid)
            {
                hi_mapi_trace(MAPI_TRACE_ERROR, "USB DEV PLUG-OUT!\n");
                memset(str, 0, 512);
                sprintf(str, "umount %s", USB_MOUNT_POINT);
                system(str);
                sg_bDevValid = HI_FALSE;
            }

            continue;
        }
        else
        {
            if (HI_FALSE == sg_bDevValid)
            {
                hi_mapi_trace(MAPI_TRACE_ERROR, "USB DEV PLUG-IN!\n");
            }
            else
            {
                continue;
            }
        }
		
        memset(str,0,sizeof(str));
        sprintf(str,"umount %s",USB_MOUNT_POINT);
        system(str);

        memset(str, 0, 512);
	  sprintf(str,"mount %s %s",diskname,USB_MOUNT_POINT);
        ret = system(str);

        if (ret < 0)
        {
            hi_mapi_trace(MAPI_TRACE_ERROR, "err:system() fail!\n");
        }
        else
        {
            sg_bDevValid = HI_TRUE;
            ret = FS_GetDiskInfo(USB_MOUNT_POINT, &stDiskInfo);
            if (HI_SUCCESS == ret)
            {
                stDiskInfo.pu8MountPoint = USB_MOUNT_POINT;
                printf("total:%dKB  free:%dKB\n", stDiskInfo.u32TotalSizeKB, stDiskInfo.u32FreeSizeKB);
            }

            continue;
        }

    }
}

HI_BOOL FS_IsDiskOk(void)
{
    return sg_bDevValid;
}

HI_S32 FS_GetDiskInfo(HI_CHAR *pu8DiskName, USB_DISK_INFO_S *pstDiskInfo)
{
    struct statfs fsstat;

    if (0 != statfs(pu8DiskName, &fsstat))
    {
        printf("get file state error!\n");
        return HI_FAILURE;
    }

    pstDiskInfo->u32TotalSizeKB = fsstat.f_blocks * fsstat.f_bsize / 1024; //KB
    pstDiskInfo->u32FreeSizeKB = fsstat.f_bfree * fsstat.f_bsize / 1024; //KB

    return HI_SUCCESS;
}

HI_S32 test_diskManage_list(void)
{
    HI_CHAR * pPath;
    HI_U32 puFolderNum = 0;
    HI_U32 puMp3Num  = 0;
    HI_U32 puJpegNum = 0;
    HI_S32 ret = HI_FAILURE;

    USB_FILE_NAME Foldername[20];
    USB_FILE_NAME MP3name[20];
    USB_FILE_NAME Jpegname[20];
    HI_U32 i;

    pPath = malloc(MAX_PATH_LENGTH);
    strcpy(pPath, "/");

    ret = FS_GetFilesNumByPath(pPath, &puFolderNum, &puMp3Num, &puJpegNum);
    if (HI_SUCCESS != ret)
    {
        printf("get num fail\n");
        return HI_FAILURE;
    }

    ret = FS_GetFilesContentByPath(pPath, (USB_FILE_NAME**)&Foldername, (USB_FILE_NAME**)&MP3name,
                                     (USB_FILE_NAME**)&Jpegname);
    if (HI_SUCCESS != ret)
    {
        printf("get num fail\n");
        return HI_FAILURE;
    }

    free(pPath);

    printf("==============   list all mp3,jpg, folder      ====================\n");
    printf(" mp3 file: ");
    for (i = 0; i < puMp3Num; i++)
    {
        printf("%s    ", MP3name[i].uFileName);
    }

    printf("\n jpeg file: ");
    for (i = 0; i < puJpegNum; i++)
    {
        printf("%s    ", Jpegname[i].uFileName);
    }

    printf("\n folder: ");
    for (i = 0; i < puFolderNum; i++)
    {
        printf(" %s    ", Foldername[i].uFileName);
    }

    printf("\n");
    printf("=====================================================\n");
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */
