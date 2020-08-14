/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_userproc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/15
  Description   : Support user proc function.
  History       :
  1.Date        : 2013/07/15
    Author      : l00185424
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

/* Unf headers */

/* Drv headers */
#include "hi_drv_memdev.h"

/* Local headers */
#include "mpi_userproc.h"
#include "hi_mpi_mem.h"
#include "hi_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#if !(0 == HI_PROC_SUPPORT)

/***************************** Macro Definition ******************************/

#define UPROC_LOCK(Mutex)                                 \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_lock(&Mutex); 	      \
    if ( s32Ret != 0 )                                    \
    {													  \
    	HI_ERR_UPROC("UPROC_LOCK error\n"); \
    }                                                     \
}while(0)

#define UPROC_UNLOCK(Mutex)                               \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_unlock(&Mutex);         \
    if ( s32Ret != 0 )                                    \
    {    												  \
    	HI_ERR_UPROC("UPROC_UNLOCK error\n");             \
    }                                                     \
}while(0)


#define HI_PROC_MAX_PARAM_NUM (8)
#define HI_PROC_MAX_PARAM_LEN (128)

#define UPROC_CHECK_INIT \
    UPROC_LOCK(g_stUprocParam.stMutex); \
    if (-1 == g_stUprocParam.s32Fd) \
    { \
        UPROC_UNLOCK(g_stUprocParam.stMutex); \
        HI_ERR_UPROC("USERPROC not init!\n"); \
        return HI_FAILURE; \
    } \
    UPROC_UNLOCK(g_stUprocParam.stMutex);


/*************************** Structure Definition ****************************/

typedef struct tagUSERPROC_PARAM_S
{
    HI_S32      s32Fd;
    HI_BOOL     bThreadStop;
    pthread_t   stThread;
    pthread_mutex_t stMutex;
    HI_U8       u8CmdBuf[HI_PROC_MAX_PARAM_NUM][HI_PROC_MAX_PARAM_LEN];
    HI_U8*      apu8Cmd[HI_PROC_MAX_PARAM_NUM];
}USERPROC_PARAM_S;


/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/

static USERPROC_PARAM_S g_stUprocParam =
{
    .s32Fd = -1,
    .stMutex = PTHREAD_MUTEX_INITIALIZER,
    .apu8Cmd = {
        g_stUprocParam.u8CmdBuf[0], g_stUprocParam.u8CmdBuf[1],
        g_stUprocParam.u8CmdBuf[2], g_stUprocParam.u8CmdBuf[3],
        g_stUprocParam.u8CmdBuf[4], g_stUprocParam.u8CmdBuf[5],
        g_stUprocParam.u8CmdBuf[6], g_stUprocParam.u8CmdBuf[7]
    }
};

static HI_VOID* MPI_UPROC_Thread(HI_VOID*);

/*********************************** Code ************************************/
HI_S32 MPI_UPROC_Init(HI_VOID)
{
    HI_S32 s32Ret;

    UPROC_LOCK(g_stUprocParam.stMutex);

    if (-1 == g_stUprocParam.s32Fd)
    {
        g_stUprocParam.s32Fd = open("/dev/"HI_USERPROC_DEVNAME, O_RDWR);
        if (0 >= g_stUprocParam.s32Fd)
        {
            UPROC_UNLOCK(g_stUprocParam.stMutex);
            HI_FATAL_UPROC("Open %s err!\n", HI_USERPROC_DEVNAME);
            return HI_FAILURE;
        }

        g_stUprocParam.bThreadStop = HI_FALSE;
        s32Ret = pthread_create(&g_stUprocParam.stThread, HI_NULL, MPI_UPROC_Thread, HI_NULL);
        if (HI_SUCCESS != s32Ret)
        {
            (HI_VOID)close(g_stUprocParam.s32Fd);
            g_stUprocParam.s32Fd = -1;
            UPROC_UNLOCK(g_stUprocParam.stMutex);
            HI_FATAL_UPROC("Create userproc thread err!\n");
            return HI_FAILURE;
        }
    }

    UPROC_UNLOCK(g_stUprocParam.stMutex);

    return HI_SUCCESS;
}

HI_S32 MPI_UPROC_DeInit(HI_VOID)
{
    UPROC_LOCK(g_stUprocParam.stMutex);

    if (-1 != g_stUprocParam.s32Fd)
    {
        g_stUprocParam.bThreadStop = HI_TRUE;
        (HI_VOID)pthread_join(g_stUprocParam.stThread, HI_NULL);

        (HI_VOID)close(g_stUprocParam.s32Fd);
        g_stUprocParam.s32Fd = -1;
    }

    UPROC_UNLOCK(g_stUprocParam.stMutex);

    return HI_SUCCESS;
}

HI_S32 MPI_UPROC_AddDir(const HI_CHAR *pszName)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_PROC_DIR_NAME_S stName = {0};

    if ((HI_NULL == pszName) || (strlen(pszName) == 0) || (strlen(pszName) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid dir name!\n");
        return HI_FAILURE;
    }

    UPROC_CHECK_INIT;

    memcpy(stName, pszName, sizeof(stName)-1);

    s32Ret = ioctl(g_stUprocParam.s32Fd, UMPIOC_ADD_DIR, stName);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_UPROC("proc created dir %s fail.\n", pszName);
    }
    else
    {
        HI_INFO_UPROC("proc created dir %s success.\n", pszName);
    }

    return s32Ret;
}

HI_S32 MPI_UPROC_RemoveDir(const HI_CHAR *pszName)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_PROC_DIR_NAME_S stName = {0};

    if ((HI_NULL == pszName) || (strlen(pszName) == 0) || (strlen(pszName) > MAX_PROC_NAME_LEN))
    {
        HI_ERR_UPROC("Invalid dir name!\n");
        return HI_FAILURE;
    }

    UPROC_CHECK_INIT;

    memcpy(stName, pszName, sizeof(stName)-1);

    s32Ret = ioctl(g_stUprocParam.s32Fd, UMPIOC_REMOVE_DIR, stName);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_UPROC("proc remove dir %s fail.\n", pszName);
    }
    else
    {
        HI_INFO_UPROC("proc remove dir %s success.\n", pszName);
    }

    return s32Ret;
}

HI_S32 MPI_UPROC_AddEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_DRV_USRMODEPROC_ENTRY_S stEntry;

    if ((HI_INVALID_MODULE_ID == u32ModuleID) || (HI_NULL == pstEntry))
    {
        HI_ERR_UPROC("Invalid param!\n");
        return HI_FAILURE;
    }

    if ((HI_NULL == pstEntry->pszEntryName) ||
        (0 == strlen(pstEntry->pszEntryName)) || (MAX_PROC_NAME_LEN < strlen(pstEntry->pszEntryName)))
    {
        HI_ERR_UPROC("Invalid entry name!\n");
        return HI_FAILURE;
    }

    if ((HI_NULL != pstEntry->pszDirectory) &&
        ((0 == strlen(pstEntry->pszDirectory)) || (MAX_PROC_NAME_LEN < strlen(pstEntry->pszDirectory))))
    {
        HI_ERR_UPROC("Invalid dir name!\n");
        return HI_FAILURE;
    }

    if ((HI_NULL == pstEntry->pfnShowProc) && (HI_NULL == pstEntry->pfnCmdProc))
    {
        HI_ERR_UPROC("Need one valid func at least!\n");
        return HI_FAILURE;
    }

    UPROC_CHECK_INIT;

    memset(&stEntry, 0, sizeof(stEntry));
    HI_OSAL_Strncpy(stEntry.aszName, pstEntry->pszEntryName, sizeof(stEntry.aszName)-1);
    if (HI_NULL != pstEntry->pszDirectory)
    {
        HI_OSAL_Strncpy(stEntry.aszParent, pstEntry->pszDirectory, sizeof(stEntry.aszParent)-1);
    }
    stEntry.pfnShowFunc = pstEntry->pfnShowProc;
    stEntry.pfnCmdFunc = pstEntry->pfnCmdProc;
    stEntry.pPrivData   = pstEntry->pPrivData;

    s32Ret = ioctl(g_stUprocParam.s32Fd, UMPIOC_ADD_ENTRY, &stEntry);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_UPROC("proc created entry %s fail:%d.\n", pstEntry->pszEntryName, s32Ret);
    }
    else
    {
        HI_INFO_UPROC("proc created entry %s success.\n", pstEntry->pszEntryName);
    }

    return s32Ret;
}

HI_S32 MPI_UPROC_RemoveEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_DRV_USRMODEPROC_ENTRY_S stEntry;

    if ((HI_INVALID_MODULE_ID == u32ModuleID) || (HI_NULL == pstEntry))
    {
        HI_ERR_UPROC("Invalid param!\n");
        return HI_FAILURE;
    }

    if ((HI_NULL == pstEntry->pszEntryName) ||
        (0 == strlen(pstEntry->pszEntryName)) || (MAX_PROC_NAME_LEN < strlen(pstEntry->pszEntryName)))
    {
        HI_ERR_UPROC("Invalid entry name!\n");
        return HI_FAILURE;
    }

    if ((HI_NULL != pstEntry->pszDirectory) &&
        ((0 == strlen(pstEntry->pszDirectory)) || (MAX_PROC_NAME_LEN < strlen(pstEntry->pszDirectory))))
    {
        HI_ERR_UPROC("Invalid dir name!\n");
        return HI_FAILURE;
    }

    UPROC_CHECK_INIT;

    memset(&stEntry, 0, sizeof(stEntry));
    HI_OSAL_Strncpy(stEntry.aszName, pstEntry->pszEntryName, sizeof(stEntry.aszName)-1);
    if (HI_NULL != pstEntry->pszDirectory)
    {
        HI_OSAL_Strncpy(stEntry.aszParent, pstEntry->pszDirectory, sizeof(stEntry.aszParent)-1);
    }

    s32Ret = ioctl(g_stUprocParam.s32Fd, UMPIOC_REMOVE_ENTRY, &stEntry);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_UPROC("proc remove entry %s fail:%d.\n", pstEntry->pszEntryName, s32Ret);
    }
    else
    {
        HI_INFO_UPROC("proc remove entry %s success.\n", pstEntry->pszEntryName);
    }

    return s32Ret;
}

static HI_U32 MPI_UPROC_ParseCmd(const HI_CHAR* pu8Cmd)
{
    const HI_CHAR* p = pu8Cmd;
    HI_U32 i = 0, j = 0;

    memset(g_stUprocParam.u8CmdBuf, 0, sizeof(g_stUprocParam.u8CmdBuf));
    while (*p)
    {
        if (*p == ' ')
        {
            if (j !=0 )
            {
                i++;
                j = 0;
            }
            if (i >= HI_PROC_MAX_PARAM_NUM)
            {
                i--;
                break;
            }
        }
        else
        {
            if (j<HI_PROC_MAX_PARAM_LEN-1)
            {
                g_stUprocParam.u8CmdBuf[i][j++] = (HI_U8)*p;
            }
        }
        p++;
    }

    return i+1;
}

static HI_VOID* MPI_UPROC_Thread(HI_VOID* pParam)
{
    HI_S32 s32Ret;
    HI_DRV_USRMODEPROC_CMDINFO_S stCMD;
    HI_PROC_SHOW_BUFFER_S stBuffer;

    while (!g_stUprocParam.bThreadStop)
    {
        memset(&stCMD, 0, sizeof(HI_DRV_USRMODEPROC_CMD_S));
        s32Ret = ioctl(g_stUprocParam.s32Fd, UMPIOC_GETCMD, &stCMD);

        if (HI_SUCCESS != s32Ret)
        {
            usleep(10*1000);
            HI_WARN_UPROC("proc GetCMD fail.\n");
            continue;
        }

        if (strlen(stCMD.stCmd.aszCmd) > 0)
        {
            HI_INFO_UPROC("proc GetCMD success: %s.\n", stCMD.stCmd.aszCmd);

            stBuffer.pu8Buf = (HI_U8*)HI_MALLOC(HI_ID_PROC, HI_PROC_BUFFER_SIZE);
            if (HI_NULL != stBuffer.pu8Buf)
            {
                stBuffer.u32Offset   = 0;
                stBuffer.u32Size     = HI_PROC_BUFFER_SIZE;

                memset(stBuffer.pu8Buf, 0, stBuffer.u32Size);

                /* read */
                if (0 == HI_OSAL_Strncmp(HI_UPROC_READ_CMD, stCMD.stCmd.aszCmd, strlen(HI_UPROC_READ_CMD)+1))
                {
                    stCMD.stEntry.pfnShowFunc(&stBuffer, stCMD.stEntry.pPrivData);

                    if (HI_SUCCESS != ioctl(g_stUprocParam.s32Fd, UMPIOC_WAKE_READ_TASK, &stBuffer))
                    {
                        HI_ERR_UPROC("proc print fail.\n");
                    }
                }
                else /* write */
                {
                    stCMD.stEntry.pfnCmdFunc(&stBuffer,
                            MPI_UPROC_ParseCmd(stCMD.stCmd.aszCmd),
                            g_stUprocParam.apu8Cmd,
                            stCMD.stEntry.pPrivData);

                    if (HI_SUCCESS != ioctl(g_stUprocParam.s32Fd, UMPIOC_WAKE_WRITE_TASK, &stBuffer))
                    {
                        HI_ERR_UPROC("proc print fail.\n");
                    }
                }
                HI_FREE(HI_ID_PROC, stBuffer.pu8Buf);
            }
            else
            {
                HI_ERR_UPROC("map usrproc log buffer failed.\n");
            }
        }

        usleep(10*1000);
    }

    return HI_NULL;
}

#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

