/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_sync_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
*******************************************************************************/
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/string.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_mmz.h"

#include "drv_sync.h"
#include "drv_sync_intf.h"
#include "hi_drv_module.h"
#include "hi_module.h"
#include "drv_sync_ext.h"
#include "hi_kernel_adapt.h"
#include "hi_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define SYNC_NAME   "HI_SYNC"

#define DRV_SYNC_TRY_LOCK(pMutex)    \
    do{ \
        if(down_trylock(pMutex)) \
        {       \
            HI_FATAL_SYNC("ERR: Sync try lock error!\n");    \
            return -1; \
        }   \
    }while(0)

#define DRV_SYNC_LOCK(pMutex)    \
    do{ \
        if(down_interruptible(pMutex)) \
        {       \
            HI_FATAL_SYNC("ERR: Sync lock error!\n");    \
            return -1; \
        }   \
    }while(0)

#define DRV_SYNC_UNLOCK(pMutex)    \
    do{ \
        up(pMutex); \
    }while(0)

#define SYNC_CHECK_ID(id)   \
        do { \
            if ((id) >= SYNC_MAX_NUM) { \
                HI_ERR_SYNC("invalid id\n"); \
                return HI_ERR_SYNC_INVALID_PARA; \
            } \
        } while (0)

static atomic_t    g_SyncCount = ATOMIC_INIT(0);

SYNC_GLOBAL_STATE_S  g_SyncGlobalState;

HI_DECLARE_MUTEX(g_SyncMutex);

#ifndef HI_ADVCA_FUNCTION_RELEASE
SYNC_REGISTER_PARAM_S  *g_pSyncProcPara = NULL;

SYNC_S  *SYNC_getInfoPtr(HI_U32 SyncId)
{
    if(SyncId >= SYNC_MAX_NUM)
    {
        return NULL;
    }

    return g_SyncGlobalState.SyncInfo[SyncId].pSync;
}

static HI_S32 SYNC_InitUsedProc(HI_VOID)
{
    HI_U32            i, j;
    HI_CHAR           ProcName[16];
    DRV_PROC_ITEM_S  *pProcItem = NULL;

    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].pSync)
        {
            HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s%02d", HI_MOD_SYNC, i);
            pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
            if (!pProcItem)
            {
                HI_FATAL_SYNC("add %s proc failed.\n", ProcName);

                for(j=0; j<i; j++)
                {
                    if (g_SyncGlobalState.SyncInfo[j].pSync)
                    {
                        HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s%02d", HI_MOD_SYNC, j);
                        HI_DRV_PROC_RemoveModule(ProcName);
                    }
                }

                return HI_FAILURE;
            }
            pProcItem->read  = g_pSyncProcPara->rdproc;
            pProcItem->write = g_pSyncProcPara->wtproc;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID SYNC_DeInitUsedProc(HI_VOID)
{
    HI_U32  i;
    HI_CHAR ProcName[16];

    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].pSync)
        {
            HI_OSAL_Snprintf(ProcName, sizeof(ProcName),  "%s%02d", HI_MOD_SYNC, i);
            HI_DRV_PROC_RemoveModule(ProcName);
        }
    }

    return;
}


HI_S32 SYNC_IntfRegister(SYNC_REGISTER_PARAM_S *SyncProcPara)
{
    HI_S32  Ret;

    g_pSyncProcPara = SyncProcPara;

    Ret = SYNC_InitUsedProc();
    if (Ret != HI_SUCCESS)
    {
        g_pSyncProcPara = HI_NULL;
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_VOID SYNC_IntfUnRegister(HI_VOID)
{
    SYNC_DeInitUsedProc();
    g_pSyncProcPara = HI_NULL;

    return;
}
#endif

HI_S32 SYNC_Create(SYNC_CREATE_S *pSyncCreate, struct file *file)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR           ProcName[12];
#endif
    MMZ_BUFFER_S      MemBuf;
    HI_S32            Ret;
    HI_U32            i;
    HI_CHAR           BufName[32];

    if (!pSyncCreate)
    {
        return HI_FAILURE;
    }

    if (SYNC_MAX_NUM == g_SyncGlobalState.SyncCount)
    {
        HI_ERR_SYNC("the sync num is max.\n");
        return HI_ERR_SYNC_CREATE_ERR;
    }

    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        if (HI_NULL == g_SyncGlobalState.SyncInfo[i].pSync)
        {
            break;
        }
    }

    if (SYNC_MAX_NUM == i)
    {
        HI_ERR_SYNC("Not free sync handle.\n");
        return HI_ERR_SYNC_CREATE_ERR;
    }

    HI_OSAL_Snprintf(BufName, sizeof(BufName), "SYNC_Inst%02d", i);

    Ret = HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, 0x1000, 0, &MemBuf);
    if (Ret != HI_SUCCESS)
    {
        HI_FATAL_SYNC("malloc %s mmz failed.\n", BufName);
        return Ret;
    }

#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s%02d", HI_MOD_SYNC, i);

    if (g_pSyncProcPara)
    {
        pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
        if (!pProcItem)
        {
            HI_FATAL_SYNC("add %s proc failed.\n", ProcName);
            return HI_FAILURE;
        }
        pProcItem->read  = g_pSyncProcPara->rdproc;
        pProcItem->write = g_pSyncProcPara->wtproc;
    }
#endif

    g_SyncGlobalState.SyncInfo[i].pSync = (SYNC_S *)MemBuf.pu8StartVirAddr;
    g_SyncGlobalState.SyncInfo[i].SyncPhyAddr = MemBuf.u32StartPhyAddr;
    g_SyncGlobalState.SyncInfo[i].File = (HI_VOID *)file;

    pSyncCreate->SyncId = i;
    pSyncCreate->SyncPhyAddr = MemBuf.u32StartPhyAddr;
    g_SyncGlobalState.SyncCount++;
    if (g_SyncGlobalState.AddSyncIns)
    {
        g_SyncGlobalState.AddSyncIns(&i);
    }

    return HI_SUCCESS;
}


HI_S32 SYNC_Destroy(HI_U32 SyncId)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_CHAR   ProcName[15];
#endif
    MMZ_BUFFER_S      MemBuf;

    SYNC_CHECK_ID(SyncId);

    if (HI_NULL == g_SyncGlobalState.SyncInfo[SyncId].pSync)
    {
        HI_ERR_SYNC("this is invalid handle.\n");
        return HI_ERR_SYNC_DESTROY_ERR;
    }
#ifndef HI_ADVCA_FUNCTION_RELEASE
    if (g_pSyncProcPara)
    {
        memset(ProcName, 0, sizeof(ProcName));
        HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s%02d", HI_MOD_SYNC, SyncId);
        HI_DRV_PROC_RemoveModule(ProcName);
    }
#endif
    MemBuf.pu8StartVirAddr = (HI_U8 *)g_SyncGlobalState.SyncInfo[SyncId].pSync;
    MemBuf.u32StartPhyAddr = g_SyncGlobalState.SyncInfo[SyncId].SyncPhyAddr;
    MemBuf.u32Size = 0x1000;
    HI_DRV_MMZ_UnmapAndRelease(&MemBuf);

    g_SyncGlobalState.SyncInfo[SyncId].pSync = HI_NULL;
    g_SyncGlobalState.SyncInfo[SyncId].SyncPhyAddr = HI_NULL;
    g_SyncGlobalState.SyncInfo[SyncId].File = HI_NULL;
    g_SyncGlobalState.SyncInfo[SyncId].SyncUsrAddr = HI_NULL;

    g_SyncGlobalState.SyncCount--;
    if (g_SyncGlobalState.DelSyncIns)
    {
        g_SyncGlobalState.DelSyncIns(&SyncId);
    }

    return HI_SUCCESS;
}


HI_S32 SYNC_SetUsrAddr(SYNC_USR_ADDR_S *pSyncUsrAddr)
{
    if (!pSyncUsrAddr)
    {
        return HI_FAILURE;
    }

    if (!pSyncUsrAddr->SyncUsrAddr)
    {
        return HI_FAILURE;
    }

    SYNC_CHECK_ID(pSyncUsrAddr->SyncId);

    g_SyncGlobalState.SyncInfo[pSyncUsrAddr->SyncId].SyncUsrAddr = pSyncUsrAddr->SyncUsrAddr;

    return HI_SUCCESS;
}


HI_S32 SYNC_CheckId(SYNC_USR_ADDR_S *pSyncUsrAddr, struct file *file)
{
    if (!pSyncUsrAddr)
    {
        return HI_FAILURE;
    }

    SYNC_CHECK_ID(pSyncUsrAddr->SyncId);

    if (g_SyncGlobalState.SyncInfo[pSyncUsrAddr->SyncId].File != ((HI_VOID *)file))
    {
        HI_ERR_SYNC("this is invalid handle.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    if (HI_NULL == g_SyncGlobalState.SyncInfo[pSyncUsrAddr->SyncId].pSync)
    {
        HI_ERR_SYNC("this is invalid handle.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    pSyncUsrAddr->SyncUsrAddr = g_SyncGlobalState.SyncInfo[pSyncUsrAddr->SyncId].SyncUsrAddr;

    return HI_SUCCESS;
}

HI_S32 SYNC_CheckNum(HI_U32 *pSyncNum, struct file *file)
{
    HI_U32   i;

    if (!pSyncNum)
    {
        return HI_FAILURE;
    }

    *pSyncNum = 0;
    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        /*multi-thread process in MCE mode*/
        /*CNcomment:此时mce方式类多线程处理即可 */
        if (g_SyncGlobalState.SyncInfo[i].File == ((HI_VOID *)file))
        {
            (*pSyncNum)++;
        }
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_COMPAT
HI_S32 SYNC_COMPAT_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32                  Ret;

    DRV_SYNC_LOCK(&g_SyncMutex);

    switch (cmd)
    {
        case CMD_SYNC_CREATE:
        {
            SYNC_CREATE_S  *pSyncCreate;

            pSyncCreate = (SYNC_CREATE_S *)arg;

            Ret = SYNC_Create(pSyncCreate, file);

            break;
        }

        case CMD_SYNC_DESTROY:
        {
            Ret = SYNC_Destroy(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_COMPAT_SET_USRADDR:
        {
            SYNC_USR_ADDR_S          SyncUsrAddr;
            SYNC_COMPAT_USR_ADDR_S  *pSyncCompatUsrAddr = (SYNC_COMPAT_USR_ADDR_S *)arg;

            SyncUsrAddr.SyncId      = pSyncCompatUsrAddr->SyncId;
            SyncUsrAddr.SyncUsrAddr = compat_ptr(pSyncCompatUsrAddr->SyncUsrAddr);
            Ret = SYNC_SetUsrAddr(&SyncUsrAddr);

            break;
        }

        case CMD_SYNC_COMPAT_CHECK_ID:
        {
            SYNC_USR_ADDR_S          SyncUsrAddr;
            SYNC_COMPAT_USR_ADDR_S  *pSyncCompatUsrAddr = (SYNC_COMPAT_USR_ADDR_S *)arg;

            SyncUsrAddr.SyncId = pSyncCompatUsrAddr->SyncId;
            Ret = SYNC_CheckId(&SyncUsrAddr, file);
            if (HI_SUCCESS == Ret)
            {
                pSyncCompatUsrAddr->SyncUsrAddr = ptr_to_compat(SyncUsrAddr.SyncUsrAddr);
            }

            break;
        }

        case CMD_SYNC_CHECK_NUM:
        {
            Ret = SYNC_CheckNum((HI_U32 *)arg, file);

            break;
        }

        case CMD_SYNC_START_SYNC:
        {
            Ret = SYNC_StartSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_AUD_JUDGE:
        {
            SYNC_AUD_JUDGE_S  *pAudJudge;

            pAudJudge = (SYNC_AUD_JUDGE_S *)arg;

            SYNC_AudProc(pAudJudge->hSync, &pAudJudge->AudInfo, &pAudJudge->AudOpt);

            Ret = HI_SUCCESS;

            break;
        }

        case CMD_SYNC_VID_JUDGE:
        {
            SYNC_VID_JUDGE_S  *pVidJudge;

            pVidJudge = (SYNC_VID_JUDGE_S *)arg;

            SYNC_VidProc(pVidJudge->hSync, &pVidJudge->VidInfo, &pVidJudge->VidOpt);

            Ret = HI_SUCCESS;

            break;
        }

        case CMD_SYNC_PAUSE_SYNC:
        {
            Ret = SYNC_PauseSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_RESUME_SYNC:
        {
            Ret = SYNC_ResumeSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_GET_TIME:
        {
            SYNC_GET_TIME_S    *pSyncTime;

            pSyncTime = (SYNC_GET_TIME_S *)arg;

            Ret = SYNC_GetTime(pSyncTime->SyncId, &pSyncTime->LocalTime, &pSyncTime->PlayTime);

            break;
        }

        default:
            DRV_SYNC_UNLOCK(&g_SyncMutex);
            return -ENOIOCTLCMD;
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return Ret;
}
#endif

HI_S32 SYNC_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32           Ret;

    DRV_SYNC_LOCK(&g_SyncMutex);

    switch (cmd)
    {
        case CMD_SYNC_CREATE:
        {
            SYNC_CREATE_S  *pSyncCreate;

            pSyncCreate = (SYNC_CREATE_S *)arg;

            Ret = SYNC_Create(pSyncCreate, file);

            break;
        }

        case CMD_SYNC_DESTROY:
        {
            Ret = SYNC_Destroy(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_SET_USRADDR:
        {
            SYNC_USR_ADDR_S *pSyncUsrAddr;

            pSyncUsrAddr = (SYNC_USR_ADDR_S *)arg;

            Ret = SYNC_SetUsrAddr(pSyncUsrAddr);

            break;
        }

        case CMD_SYNC_CHECK_ID:
        {
            SYNC_USR_ADDR_S *pSyncUsrAddr;

            pSyncUsrAddr = (SYNC_USR_ADDR_S *)arg;

            Ret = SYNC_CheckId(pSyncUsrAddr, file);

            break;
        }

        case CMD_SYNC_CHECK_NUM:
        {
            Ret = SYNC_CheckNum((HI_U32 *)arg, file);

            break;
        }

        case CMD_SYNC_START_SYNC:
        {
            Ret = SYNC_StartSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_AUD_JUDGE:
        {
            SYNC_AUD_JUDGE_S  *pAudJudge;

            pAudJudge = (SYNC_AUD_JUDGE_S *)arg;

            SYNC_AudProc(pAudJudge->hSync, &pAudJudge->AudInfo, &pAudJudge->AudOpt);

            Ret = HI_SUCCESS;

            break;
        }

        case CMD_SYNC_VID_JUDGE:
        {
            SYNC_VID_JUDGE_S  *pVidJudge;

            pVidJudge = (SYNC_VID_JUDGE_S *)arg;

            SYNC_VidProc(pVidJudge->hSync, &pVidJudge->VidInfo, &pVidJudge->VidOpt);

            Ret = HI_SUCCESS;

            break;
        }

        case CMD_SYNC_PAUSE_SYNC:
        {
            Ret = SYNC_PauseSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_RESUME_SYNC:
        {
            Ret = SYNC_ResumeSync(*((HI_U32 *)arg));

            break;
        }

        case CMD_SYNC_GET_TIME:
        {
            SYNC_GET_TIME_S    *pSyncTime;

            pSyncTime = (SYNC_GET_TIME_S *)arg;

            Ret = SYNC_GetTime(pSyncTime->SyncId, &pSyncTime->LocalTime, &pSyncTime->PlayTime);

            break;
        }

        default:
            DRV_SYNC_UNLOCK(&g_SyncMutex);
            return -ENOIOCTLCMD;
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return Ret;
}

HI_S32 SYNC_DRV_Open(struct inode *finode, struct file  *ffile)
{

    DRV_SYNC_LOCK(&g_SyncMutex);

    if (1 == atomic_inc_return(&g_SyncCount))
    {
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}


HI_S32 SYNC_DRV_Close(struct inode *finode, struct file  *ffile)
{
    HI_S32           i;
    HI_S32           Ret;

    DRV_SYNC_LOCK(&g_SyncMutex);

    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].File == ((HI_VOID *)ffile))
        {
            Ret = SYNC_Destroy(i);
            if (Ret != HI_SUCCESS)
            {
                DRV_SYNC_UNLOCK(&g_SyncMutex);
                return -1;
            }
        }
    }

    if (atomic_dec_and_test(&g_SyncCount))
    {
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}


HI_S32 SYNC_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_U32  i;

    DRV_SYNC_TRY_LOCK(&g_SyncMutex);

    /* if SYNC device has never opened by any process,return directly */
    if (!atomic_read(&g_SyncCount))
    {
        DRV_SYNC_UNLOCK(&g_SyncMutex);
        return 0;
    }

    /* it needs to pause when standby,because system time keep running*/
    for (i = 0; i < SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].pSync)
        {
            SYNC_PauseSync(i);
        }
    }

    HI_PRINT("SYNC suspend OK\n");

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}


HI_S32 SYNC_Resume(PM_BASEDEV_S *pdev)
{
    HI_U32  i;

    DRV_SYNC_TRY_LOCK(&g_SyncMutex);

    if (!atomic_read(&g_SyncCount))
    {
        DRV_SYNC_UNLOCK(&g_SyncMutex);
        return 0;
    }

    for (i = 0; i < SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].pSync)
        {
            SYNC_ResumeSync(i);
        }
    }

    HI_PRINT("SYNC resume OK\n");

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}


#ifndef MODULE
#ifndef HI_ADVCA_FUNCTION_RELEASE
EXPORT_SYMBOL(SYNC_getInfoPtr);
EXPORT_SYMBOL(SYNC_IntfRegister);
EXPORT_SYMBOL(SYNC_IntfUnRegister);
#endif
EXPORT_SYMBOL(SYNC_Ioctl);
EXPORT_SYMBOL(SYNC_DRV_Open);
EXPORT_SYMBOL(SYNC_DRV_Close);
EXPORT_SYMBOL(SYNC_Suspend);
EXPORT_SYMBOL(SYNC_Resume);
/*Temporary useage*/
EXPORT_SYMBOL(SYNC_VidProc);
EXPORT_SYMBOL(SYNC_GetLocalTime);
#endif

static SYNC_EXPORT_FUNC_S g_SyncExportFuncs =
{
    .pfnSYNC_VidProc = SYNC_VidProc,
    .pfnSYNC_PcrProc = SYNC_PcrProc,
};

HI_S32 SYNC_DRV_Init(HI_VOID)
{
    HI_S32    ret;
    HI_U32    i;

    ret = HI_DRV_MODULE_Register(HI_ID_SYNC, SYNC_NAME, (HI_VOID*)&g_SyncExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_SYNC("ERR: HI_DRV_MODULE_Register!\n");
        return ret;
    }
#ifndef HI_ADVCA_FUNCTION_RELEASE
    g_pSyncProcPara = HI_NULL;
#endif
    g_SyncGlobalState.SyncCount = 0;
    g_SyncGlobalState.AddSyncIns = 0;
    g_SyncGlobalState.DelSyncIns = 0;
    g_SyncGlobalState.SyncCount = 0;
    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        g_SyncGlobalState.SyncInfo[i].pSync = HI_NULL;
        g_SyncGlobalState.SyncInfo[i].SyncPhyAddr = HI_NULL;
        g_SyncGlobalState.SyncInfo[i].File = HI_NULL;
        g_SyncGlobalState.SyncInfo[i].SyncUsrAddr = HI_NULL;
    }

    return 0;
}

HI_VOID SYNC_DRV_Exit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_SYNC);
    return;
}


HI_S32 SYNC_MOD_Init(HI_VOID)
{
    DRV_SYNC_LOCK(&g_SyncMutex);

    if (1 == atomic_inc_return(&g_SyncCount))
    {
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}

HI_S32 SYNC_MOD_DeInit(HI_VOID)
{
    HI_S32           i;
    HI_S32           Ret;

    DRV_SYNC_LOCK(&g_SyncMutex);

    for (i=0; i<SYNC_MAX_NUM; i++)
    {
        if (g_SyncGlobalState.SyncInfo[i].pSync && (HI_NULL == g_SyncGlobalState.SyncInfo[i].File))
        {
            Ret = SYNC_Destroy(i);
            if (Ret != HI_SUCCESS)
            {
                DRV_SYNC_UNLOCK(&g_SyncMutex);
                return -1;
            }
        }
    }

    if (atomic_dec_and_test(&g_SyncCount))
    {
    }

    DRV_SYNC_UNLOCK(&g_SyncMutex);

    return 0;
}

//#ifdef HI_MCE_SUPPORT
/*the following interface is only for mce now*/
/*CNCommont: 下述接口均为SYNC模块驱动下对外接口，目前仅提供给MCE模块使用*/
static HI_S32 DRV_SYNC_CheckHandle(HI_HANDLE hSync, SYNC_S **ppSync, HI_U32 *pSyncID)
{
    *pSyncID = hSync & 0xff;

    if(*pSyncID >= SYNC_MAX_NUM)
    {
        HI_ERR_SYNC("ERR: invalid handle!");
        return HI_FAILURE;
    }

    if(HI_NULL == g_SyncGlobalState.SyncInfo[*pSyncID].pSync)
    {
        HI_ERR_SYNC("ERR: pSync = null!");
        return HI_FAILURE;
    }

    *ppSync = g_SyncGlobalState.SyncInfo[*pSyncID].pSync;

    return HI_SUCCESS;
}

static HI_VOID DRV_SYNC_ResetStatInfo(SYNC_S *pSync, SYNC_CHAN_E enChn)
{
    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidFirstCome = HI_FALSE;
        pSync->VidFirstSysTime = HI_INVALID_TIME;
        pSync->VidFirstPts = HI_INVALID_PTS;
        pSync->VidLastPts = HI_INVALID_PTS;
        pSync->VidPreSyncTargetInit = HI_FALSE;
        pSync->VidPreSyncTargetTime = HI_INVALID_TIME;
        pSync->VidLocalTimeFlag = HI_FALSE;
        pSync->VidLastSysTime = HI_INVALID_TIME;
        pSync->VidLastLocalTime = HI_INVALID_TIME;
        pSync->VidPauseLocalTime = HI_INVALID_TIME;
        pSync->VidPtsSeriesCnt = 0;

        pSync->CrtBufStatus.VidBufState = SYNC_BUF_STATE_NORMAL;
        pSync->CrtBufStatus.VidBufPercent = 0;
        pSync->CrtBufStatus.bOverflowDiscFrm = HI_FALSE;
        pSync->VidSyndAdjust = HI_FALSE;
        pSync->VidDisPlayCnt = 0;
        pSync->VidDiscardCnt = 0;
        pSync->VidRepPlayCnt = 0;
        pSync->VidRepeatCnt = 0;
        pSync->VidFirstPlay = HI_FALSE;
        pSync->VidFirstPlayTime = HI_INVALID_TIME;
        pSync->VidInfo.SrcPts = HI_INVALID_PTS;
        pSync->VidInfo.Pts = HI_INVALID_PTS;
        pSync->VidInfo.FrameTime = 40;
        pSync->VidInfo.DelayTime = 20;

        pSync->VidFirstValidCome = HI_FALSE;
        pSync->VidFirstValidPts = HI_INVALID_PTS;
        pSync->VidLastSrcPts = HI_INVALID_PTS;
        pSync->VidPtsLoopBack = HI_FALSE;

        pSync->VidFirstDecPts = HI_INVALID_PTS;
        pSync->VidSecondDecPts = HI_INVALID_PTS;
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudFirstCome = HI_FALSE;
        pSync->AudFirstSysTime = HI_INVALID_TIME;
        pSync->AudFirstPts = HI_INVALID_PTS;
        pSync->AudLastPts = HI_INVALID_PTS;
        pSync->AudLastBufTime = 0;
        pSync->AudPreSyncTargetInit = HI_FALSE;
        pSync->AudPreSyncTargetTime = HI_INVALID_TIME;
        pSync->AudLocalTimeFlag = HI_FALSE;
        pSync->AudLastSysTime = HI_INVALID_TIME;
        pSync->AudLastLocalTime = HI_INVALID_TIME;
        pSync->AudPauseLocalTime = HI_INVALID_TIME;
        pSync->AudPtsSeriesCnt = 0;

        pSync->CrtBufStatus.AudBufState = SYNC_BUF_STATE_NORMAL;
        pSync->CrtBufStatus.AudBufPercent = 0;

        pSync->AudReSync = HI_TRUE;
        pSync->AudReBufFund = HI_TRUE;
        pSync->AudPlayCnt = 0;
        pSync->AudDiscardCnt = 0;
        pSync->AudRepeatCnt = 0;
        pSync->AudInfo.SrcPts = HI_INVALID_PTS;
        pSync->AudInfo.Pts = HI_INVALID_PTS;
        pSync->AudInfo.FrameTime = 24;
        pSync->AudInfo.BufTime = 0;
        pSync->AudInfo.FrameNum = 0;
        pSync->AudFirstPlay = HI_FALSE;
        pSync->AudFirstPlayTime = HI_INVALID_TIME;

        pSync->AudFirstValidCome = HI_FALSE;
        pSync->AudFirstValidPts = HI_INVALID_PTS;
        pSync->AudLastSrcPts = HI_INVALID_PTS;
        pSync->AudPtsLoopBack = HI_FALSE;
    }

    if ((!pSync->VidEnable)
      &&(!pSync->AudEnable)
       )
    {
        pSync->CrtStatus = SYNC_STATUS_STOP;
        pSync->PreSyncStartSysTime = HI_INVALID_TIME;
        pSync->PreSyncEndSysTime = HI_INVALID_TIME;
        pSync->PreSyncFinish = HI_FALSE;
        pSync->BufFundEndSysTime = HI_INVALID_TIME;
        pSync->BufFundFinish = HI_FALSE;
        pSync->PreSyncTarget = SYNC_CHAN_VID;
        pSync->PreSyncTargetTime = HI_INVALID_TIME;
        pSync->PreSyncTargetInit = HI_FALSE;

        pSync->PcrSyncInfo.PcrFirstCome = HI_FALSE;
        pSync->PcrSyncInfo.PcrAdjustDeltaOK = HI_TRUE;
        pSync->PcrSyncInfo.PcrFirstSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrFirst = HI_INVALID_PTS;
        pSync->PcrSyncInfo.PcrLast = HI_INVALID_PTS;
        pSync->PcrSyncInfo.PcrLastSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrLastLocalTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrPauseLocalTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrLocalTimeFlag = HI_FALSE;
        pSync->PcrSyncInfo.PcrSeriesCnt = 0;
        pSync->PcrSyncInfo.PcrSyncStartSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrDelta = 0;
        pSync->PcrSyncInfo.enPcrAdjust = SYNC_SCR_ADJUST_BUTT;
        pSync->PcrSyncInfo.PcrLoopBack = HI_FALSE;

        pSync->ScrInitFlag = HI_FALSE;
        pSync->ScrFirstLocalTime = HI_INVALID_TIME;
        pSync->ScrFirstSysTime = HI_INVALID_TIME;
        pSync->ScrLastLocalTime = HI_INVALID_TIME;
        pSync->ScrLastSysTime = HI_INVALID_TIME;

        pSync->AudReSync = HI_FALSE;
        pSync->AudReBufFund = HI_FALSE;

        pSync->SyncEvent.bAudPtsJump = HI_FALSE;
        pSync->SyncEvent.bVidPtsJump = HI_FALSE;
        pSync->SyncEvent.bStatChange = HI_FALSE;
        pSync->LoopBackTime = HI_INVALID_TIME;
        pSync->LoopBackFlag = HI_FALSE;
    }

    return;
}

HI_S32 HI_DRV_SYNC_Init(HI_VOID)
{
    HI_S32          Ret;

    /*KO加载前半部*/
    Ret = SYNC_DRV_Init();

    /*本模块基本初始化*/
    Ret |= SYNC_MOD_Init();

    return Ret;
}

HI_S32 HI_DRV_SYNC_DeInit(HI_VOID)
{
    HI_S32          Ret;

    Ret = SYNC_MOD_DeInit();

    SYNC_DRV_Exit();

    return Ret;
}

HI_S32 HI_DRV_SYNC_Create(HI_UNF_SYNC_ATTR_S *pstSyncAttr, HI_HANDLE *phSync)
{
    HI_S32          Ret;
    SYNC_S          *pSync = HI_NULL;
    SYNC_CREATE_S   SyncCreate;

    if(HI_NULL == pstSyncAttr || HI_NULL == phSync)
    {
        return HI_FAILURE;
    }


    Ret = SYNC_Create(&SyncCreate, HI_NULL);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: SYNC_MOD_Ioctl!");
        return Ret;
    }

    pSync = g_SyncGlobalState.SyncInfo[SyncCreate.SyncId].pSync;

    pSync->SyncAttr  = *pstSyncAttr;
    pSync->VidEnable = HI_FALSE;
    pSync->AudEnable = HI_FALSE;
    pSync->CrtStatus = SYNC_STATUS_STOP;
    pSync->bPrint = HI_TRUE;
    pSync->bUseStopRegion = HI_TRUE;

    DRV_SYNC_ResetStatInfo(pSync, SYNC_CHAN_VID);
    DRV_SYNC_ResetStatInfo(pSync, SYNC_CHAN_AUD);

    *phSync = (HI_ID_SYNC << 16) | SyncCreate.SyncId;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_Destroy(HI_HANDLE hSync)
{
    HI_S32              Ret;
    HI_U32              SyncID;

    SyncID = hSync & 0xff;

    Ret = SYNC_Destroy(SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: SYNC_Destroy.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS ;
}

HI_S32 HI_DRV_SYNC_Start(HI_HANDLE hSync, SYNC_CHAN_E enChn)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidEnable = HI_TRUE;
        Ret = SYNC_StartSync(SyncID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_SYNC("SYNC_StartSync err.\n");
            return HI_FAILURE;
        }
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudEnable = HI_TRUE;
        Ret = SYNC_StartSync(SyncID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_SYNC("SYNC_StartSync err.\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_Stop(HI_HANDLE hSync, SYNC_CHAN_E enChn)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidEnable = HI_FALSE;
        DRV_SYNC_ResetStatInfo(pSync, SYNC_CHAN_VID);
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudEnable = HI_FALSE;
        DRV_SYNC_ResetStatInfo(pSync, SYNC_CHAN_AUD);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_Play(HI_HANDLE hSync)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    pSync->CrtStatus = SYNC_STATUS_PLAY;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_AudJudge(HI_HANDLE hSync, SYNC_AUD_INFO_S *pAudInfo, SYNC_AUD_OPT_S *pAudOpt)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    SYNC_AudProc(hSync, pAudInfo, pAudOpt);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_VidJudge(HI_HANDLE hSync, SYNC_VID_INFO_S *pVidInfo, SYNC_VID_OPT_S *pVidOpt)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    SYNC_VidProc(hSync, pVidInfo, pVidOpt);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_GetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    memcpy(pstSyncAttr, &pSync->SyncAttr, sizeof(HI_UNF_SYNC_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYNC_SetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pSyncAttr)
{
    HI_S32      Ret;
    SYNC_S      *pSync = HI_NULL;
    HI_U32      SyncID;

    Ret = DRV_SYNC_CheckHandle(hSync, &pSync, &SyncID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_SYNC("ERR: DRV_SYNC_CheckHandle.\n");
        return HI_FAILURE;
    }

    memcpy(&pSync->SyncAttr, pSyncAttr, sizeof(HI_UNF_SYNC_ATTR_S));

    return HI_SUCCESS;
}
//#endif

HI_S32 HI_DRV_SYNC_RegCallback(SyncManage synAddCallBack,SyncManage synDelCallBack)
{
    g_SyncGlobalState.AddSyncIns = synAddCallBack;
    g_SyncGlobalState.DelSyncIns = synDelCallBack;
    return HI_SUCCESS;
}

EXPORT_SYMBOL(HI_DRV_SYNC_Init);
EXPORT_SYMBOL(HI_DRV_SYNC_DeInit);
EXPORT_SYMBOL(HI_DRV_SYNC_Stop);
EXPORT_SYMBOL(HI_DRV_SYNC_Start);
EXPORT_SYMBOL(HI_DRV_SYNC_GetAttr);
EXPORT_SYMBOL(HI_DRV_SYNC_SetAttr);
EXPORT_SYMBOL(HI_DRV_SYNC_Create);
EXPORT_SYMBOL(HI_DRV_SYNC_Destroy);
EXPORT_SYMBOL(HI_DRV_SYNC_VidJudge);
EXPORT_SYMBOL(HI_DRV_SYNC_Play);
EXPORT_SYMBOL(HI_DRV_SYNC_RegCallback);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
