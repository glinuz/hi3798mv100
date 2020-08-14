#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_unf_mce.h"
#include "hi_flash.h"
#include "hi_drv_struct.h"
#include "hi_drv_mce.h"

static HI_S32               g_MceDevFd = -1;
static const HI_CHAR        g_MceDevName[] = "/dev/"UMAP_DEVNAME_MCE;
static pthread_mutex_t      g_MceMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_MCE_LOCK()        (void)pthread_mutex_lock(&g_MceMutex);
#define HI_MCE_UNLOCK()      (void)pthread_mutex_unlock(&g_MceMutex);

HI_S32 HI_UNF_MCE_Init(HI_UNF_MCE_INIT_PARAM_S *pstInitParam)
{
    HI_MCE_LOCK();

    if(g_MceDevFd > 0)
    {
        HI_MCE_UNLOCK();
        return HI_SUCCESS;
    }

    g_MceDevFd = open(g_MceDevName, O_RDWR|O_NONBLOCK, 0);
    if(g_MceDevFd < 0)
    {
        HI_ERR_MCE("ERR: mce open err.\n");
        HI_MCE_UNLOCK();
        return HI_ERR_MCE_DEV_OPEN_ERR;
    }

    HI_MCE_UNLOCK();

    return HI_SUCCESS;
}

HI_VOID HI_UNF_MCE_DeInit(HI_VOID)
{
    HI_S32              Ret;

    HI_MCE_LOCK();

    if(g_MceDevFd < 0)
    {
        HI_MCE_UNLOCK();
        return;
    }

    Ret = close(g_MceDevFd);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: close mce err.\n");
        HI_MCE_UNLOCK();
        return;
    }

    g_MceDevFd = -1;

    HI_MCE_UNLOCK();

    return;
}

HI_S32  HI_UNF_MCE_ClearLogo(HI_VOID)
{
    HI_S32      Ret;

    if(g_MceDevFd < 0)
    {
        HI_ERR_MCE("ERR: mce not init.\n");
        return HI_ERR_MCE_DEV_NOT_INIT;
    }

    Ret = ioctl(g_MceDevFd, HI_MCE_CLEAR_LOGO_CMD);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: ioctl clear logo.\n");
        return HI_ERR_MCE_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_MCE_Stop(HI_UNF_MCE_STOPPARM_S *pstStopParam)
{
    HI_S32          Ret;

    if(g_MceDevFd < 0)
    {
        HI_ERR_MCE("ERR: mce not init.\n");
        return HI_ERR_MCE_DEV_NOT_INIT;
    }

    if (HI_NULL == pstStopParam)
    {
        HI_ERR_MCE("ERR: pstStopParam is NULL\n");
        return HI_ERR_MCE_PTR_NULL;
    }

    if (pstStopParam->enCtrlMode > HI_UNF_MCE_PLAYCTRL_BUTT)
    {
        HI_ERR_MCE("ERR: enCtrlMode is invalid\n");
        return HI_ERR_MCE_PARAM_INVALID;
    }

    if (pstStopParam->enStopMode > HI_UNF_AVPLAY_STOP_MODE_BUTT)
    {
        HI_ERR_MCE("ERR: enStopMode is invalid\n");
        return HI_ERR_MCE_PARAM_INVALID;
    }

    Ret = ioctl(g_MceDevFd, HI_MCE_STOP_FASTPLAY_CMD, pstStopParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: ioctl stop mce.\n");
        return HI_ERR_MCE_INVALID_OPT;
    }

    return Ret;
}

HI_S32 HI_UNF_MCE_Exit(HI_UNF_MCE_EXITPARAM_S *pstExitParam)
{
    HI_S32                  Ret;
    HI_UNF_MCE_EXITPARAM_S  stParam;

    if(g_MceDevFd < 0)
    {
        HI_ERR_MCE("ERR: mce not init.\n");
        return HI_ERR_MCE_DEV_NOT_INIT;
    }

    if (HI_NULL == pstExitParam)
    {
        stParam.hNewWin = HI_INVALID_HANDLE;
    }
    else
    {
        stParam = *pstExitParam;
    }

    Ret = ioctl(g_MceDevFd, HI_MCE_EXIT_FASTPLAY_CMD, &stParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: ioctl stop mce.\n");
        return HI_ERR_MCE_INVALID_OPT;
    }

    return Ret;
}

