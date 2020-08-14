#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"

#include "hi_drv_mce.h"
#include "drv_mce_ext.h"
#include "drv_media_mem.h"
#include "drv_pdm_ext.h"
#include "drv_disp_ext.h"
#include "drv_hifb_ext.h"
#include "hi_osal.h"

#define MCE_NAME                "HI_MCE"
#define MMZ_INFOZONE_SIZE       (8 * 1024)

DEFINE_SEMAPHORE(g_MceMutex);

#define DRV_MCE_Lock()      \
    do{         \
        if(down_interruptible(&g_MceMutex))   \
        {       \
            HI_ERR_MCE("ERR: mce intf lock error!\n");  \
        }       \
      }while(0)

#define DRV_MCE_UnLock()      \
    do{         \
        up(&g_MceMutex);    \
      }while(0)


typedef struct tagMCE_REGISTER_PARAM_S{
    DRV_PROC_READ_FN        rdproc;
    DRV_PROC_WRITE_FN       wtproc;
}MCE_REGISTER_PARAM_S;

MCE_S   g_Mce =
{
    .hAvplay = HI_INVALID_HANDLE,
    .hWindow = HI_INVALID_HANDLE,
    .bPlayStop = HI_TRUE,
    .bMceExit = HI_TRUE,
    .BeginTime = 0,
    .EndTime = 0,
#ifdef ANDROID
    .stStopParam.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_COUNT,
    .stStopParam.u32PlayCount = 1,
#else
    .stStopParam.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BUTT,
#endif
    .playEnd = HI_FALSE
};

#if (1 == HI_PROC_SUPPORT)
static HI_S32 MCE_ProcRead(struct seq_file *p, HI_VOID *v)
{
#ifdef HI_MCE_SUPPORT
    MCE_S       *pMce = &g_Mce;
    HI_UNF_MCE_PLAY_PARAM_S *pPlayParam = HI_NULL;

    PROC_PRINT(p,"---------------------------PlayParam------------------------------\n");
    PROC_PRINT(p,"enPlayType:      %10d,   bPlayEnable:        %10d\n",
                       pMce->stMceParam.stPlayParam.enPlayType, pMce->stMceParam.stPlayParam.bPlayEnable
                   );

    pPlayParam = &pMce->stMceParam.stPlayParam;
    if (HI_UNF_MCE_TYPE_PLAY_DVB == pPlayParam->enPlayType)
    {
        PROC_PRINT(p,"u32VideoPid:     %10d,   u32AudioPid:        %10d\n"
                          "enVideoType:     %10d,   enAudioType:        %10d\n"
                          "u32Volume:       %10d,   enTrackMode:        %10d\n"
                          "enSigType:       %10d                            \n",
                          pPlayParam->unParam.stDvbParam.u32VideoPid, pPlayParam->unParam.stDvbParam.u32AudioPid,
                          pPlayParam->unParam.stDvbParam.enVideoType, pPlayParam->unParam.stDvbParam.enAudioType,
                          pPlayParam->unParam.stDvbParam.u32Volume, pPlayParam->unParam.stDvbParam.enTrackMode,
                          pPlayParam->unParam.stDvbParam.stConnectPara.enSigType
                        );

        if (HI_UNF_TUNER_SIG_TYPE_SAT == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
        {
            PROC_PRINT(p,"u32Freq:         %10d,   u32SymbolRate:      %10d\n"
                              "enPolar:         %10d                            \n",
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32Freq,pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32SymbolRate,
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.enPolar
                           );
        }
        else if (   (HI_UNF_TUNER_SIG_TYPE_CAB == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
                 || (HI_UNF_TUNER_SIG_TYPE_J83B == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType) )
        {
            PROC_PRINT(p,"u32Freq:         %10d,   u32SymbolRate:      %10d\n"
                              "enModType:       %10d                            \n",
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32Freq,pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32SymbolRate,
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.enModType
                           );
        }
        else if (   (HI_UNF_TUNER_SIG_TYPE_DVB_T == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
                 || (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
                 || (HI_UNF_TUNER_SIG_TYPE_ISDB_T == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
                 || (HI_UNF_TUNER_SIG_TYPE_DTMB == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType) )
        {
            PROC_PRINT(p,     "u32Freq:         %10d,   u32BandWidth:      %10d\n"
                              "enModType:       %10d                            \n",
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stTer.u32Freq, pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stTer.u32BandWidth,
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stTer.enModType
                           );
        }
    }
    else if (HI_UNF_MCE_TYPE_PLAY_TSFILE == pPlayParam->enPlayType)
    {
        PROC_PRINT(p,"u32VideoPid:     %10d,   u32AudioPid:        %10d\n"
                          "enVideoType:     %10d,   enAudioType:        %10d\n"
                          "u32Volume:       %10d,   enTrackMode:        %10d\n"
                          "u32ContentLen:   %10d                            \n",
                          pPlayParam->unParam.stTsParam.u32VideoPid, pPlayParam->unParam.stTsParam.u32AudioPid,
                          pPlayParam->unParam.stTsParam.enVideoType, pPlayParam->unParam.stTsParam.enAudioType,
                          pPlayParam->unParam.stTsParam.u32Volume, pPlayParam->unParam.stTsParam.enTrackMode,
                          pPlayParam->unParam.stTsParam.u32ContentLen
                        );
    }

    PROC_PRINT(p,"---------------------------PlayStatus-----------------------------\n");
    PROC_PRINT(p,"hAvplay:         %10d,   hWindow:            %10d\n"
                      "BeginTime:       %10d,   EndTime:            %10d\n"
                      "bPlayStop:       %10d\n"
                      "bMceExit:        %10d\n",
                      pMce->hAvplay,    pMce->hWindow,
                      pMce->BeginTime,  pMce->EndTime,
                      pMce->bPlayStop,
                      pMce->bMceExit
                   );
#endif

    return HI_SUCCESS;
}

HI_VOID MCE_ProcPrintHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper("Stop MCE use following command:\n");
    HI_DRV_PROC_EchoHelper("echo stop count|time PlayTime|PlayCount   >/proc/msp/mce\n");
    HI_DRV_PROC_EchoHelper("Such As:\n");
    HI_DRV_PROC_EchoHelper("echo stop count   2    >/proc/msp/mce\n");
    HI_DRV_PROC_EchoHelper("echo stop time  5000   >/proc/msp/mce\n");
    HI_DRV_PROC_EchoHelper("echo stoponly count   2    >/proc/msp/mce\n");
    HI_DRV_PROC_EchoHelper("echo stoponly time  5000   >/proc/msp/mce\n");
    HI_DRV_PROC_EchoHelper("echo exit   >/proc/msp/mce\n");
}

#define MCE_PROC_STR_STOP    "stop"
#define MCE_PROC_STR_COUNT   "count"
#define MCE_PROC_STR_TIME    "time"
#define MCE_PROC_STR_STOP_ONLY    "stoponly"

HI_S32 parse_proc_str(HI_CHAR *str, HI_UNF_MCE_STOPPARM_S *stStopParam, HI_UNF_MCE_EXITPARAM_S *stExitParam, HI_BOOL *bStopOnly)
{
    HI_U32 i;
    HI_BOOL bPara1 = HI_FALSE;//we sequently process the parameter, when firstly got the para successfully, we set this flag bPara1 as TRUE.
    HI_BOOL bPara2 = HI_FALSE;//we sequently process the parameter, when secondly got the para successfully, we  set this flag bPara2 as TRUE.
    HI_BOOL bPara3 = HI_FALSE;//we sequently process the parameter, when thirdly got the para successfully, we  set this flag bPara3 as TRUE.

    HI_U32 u32Value = 0;

    *bStopOnly = HI_FALSE;

    i=0;
    while(str[i]!='\0')
    {
        if(str[i] == ' ')//skip blank
        {
            i++;
            continue;
        }

        if(bPara1
            || (strncmp(str + i, MCE_PROC_STR_STOP_ONLY,strlen(MCE_PROC_STR_STOP_ONLY)) == 0)
            || (strncmp(str + i, MCE_PROC_STR_STOP,strlen(MCE_PROC_STR_STOP)) == 0))
        {
            if(bPara1 == HI_FALSE)
            {
                bPara1 = HI_TRUE;

                if (strncmp(str + i, MCE_PROC_STR_STOP_ONLY,strlen(MCE_PROC_STR_STOP_ONLY)) == 0)
                {
                    i += strlen(MCE_PROC_STR_STOP_ONLY);
                    *bStopOnly = HI_TRUE;
                }
                else
                {
                    i += strlen(MCE_PROC_STR_STOP);
                }

                continue;
            }
        }
        else
        {
            return HI_FAILURE;
        }

        if(bPara2 || strncmp(str + i, MCE_PROC_STR_COUNT,strlen(MCE_PROC_STR_COUNT)) == 0)
        {
            if(bPara2 == HI_FALSE)
            {
                bPara2 = HI_TRUE;
                i += strlen(MCE_PROC_STR_COUNT);
                stStopParam->enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_COUNT;
                continue;
            }
        }
        else if (bPara2 || strncmp(str + i, MCE_PROC_STR_TIME,strlen(MCE_PROC_STR_TIME)) == 0)
        {
            if(bPara2 == HI_FALSE)
            {
                bPara2 = HI_TRUE;
                i += strlen(MCE_PROC_STR_TIME);
                stStopParam->enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_TIME;
                continue;
            }
        }
        else
        {
            return HI_FAILURE;
        }


        if(bPara1 && bPara2)
        {
            if(strlen(str + i) > 9) //we consider it over 9bit number as illegal, 9 is an experienced value, without foundation.
            {
                HI_ERR_MCE("The Value is too big!\n");
                return HI_FAILURE;
            }

            if(str[i] < '0' ||  str[i] > '9')
            {
                break;
            }

            u32Value = u32Value * 10 + str[i] - '0';
            i++;
            bPara3 = HI_TRUE;
        }
    }

    if(bPara1 && bPara2 && bPara3 )
    {
        if(HI_UNF_MCE_PLAYCTRL_BY_TIME == stStopParam->enCtrlMode)
        {
            stStopParam->u32PlayTimeMs = u32Value;
        }
        else if (HI_UNF_MCE_PLAYCTRL_BY_COUNT== stStopParam->enCtrlMode)
        {
            stStopParam->u32PlayCount = u32Value;
        }
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 MCE_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
#ifdef HI_MCE_SUPPORT
    HI_CHAR           ProcPara[64]={0};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_MCE_STOPPARM_S stStopParam;
    HI_UNF_MCE_EXITPARAM_S stExitParam;
    HI_BOOL bStopOnly = HI_FALSE;

    if (count >= sizeof(ProcPara))
    {
        HI_ERR_MCE("ERROR: parameter string is too long!\n");
        return 0;
    }

    if (copy_from_user(ProcPara, buf, count))
    {
        return -EFAULT;
    }

    ProcPara[63] = '\0';

    memset(&stStopParam, 0 , sizeof(stStopParam));
    memset(&stExitParam, 0, sizeof(stExitParam));

    stStopParam.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;// currently we only support BLACK mode

    if (0 == strncmp(ProcPara, "exit", strlen("exit")))
    {
        MCE_S       *pMce = &g_Mce;

        if (!pMce->bPlayStop)
        {
            HI_ERR_MCE("please stop mce first!\n");
            return count;
        }

        stExitParam.hNewWin = HI_INVALID_HANDLE;
        s32Ret = HI_DRV_MCE_Exit(&stExitParam);

    }
    else
    {
        s32Ret = parse_proc_str(ProcPara, &stStopParam, &stExitParam, &bStopOnly);
        if(HI_FAILURE == s32Ret)
        {
            if (0 == strncmp(ProcPara, "stop", strlen("stop")))
            {
                HI_UNF_MCE_STOPPARM_S stStopParam;
                HI_UNF_MCE_EXITPARAM_S stExitParam;

            #ifdef ANDROID
                stStopParam.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_COUNT;
                stStopParam.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
                stStopParam.u32PlayCount = 1;
            #else
                stStopParam.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_TIME;
                stStopParam.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
                stStopParam.u32PlayTimeMs = 0;
            #endif

                s32Ret = HI_DRV_MCE_Stop(&stStopParam);
                stExitParam.hNewWin = HI_INVALID_HANDLE;
                s32Ret |= HI_DRV_MCE_Exit(&stExitParam);
            }
            else
            {
                MCE_ProcPrintHelp();
            }
        }
        else
        {
            if (bStopOnly)
            {
                s32Ret = HI_DRV_MCE_Stop(&stStopParam);
            }
            else
            {
                s32Ret = HI_DRV_MCE_Stop(&stStopParam);
                stExitParam.hNewWin = HI_INVALID_HANDLE;
                s32Ret |= HI_DRV_MCE_Exit(&stExitParam);
            }
        }
    }

#endif

    return count;
}
#endif


HI_S32 MCE_DRV_Open(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 MCE_DRV_Close(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 MCE_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32              Ret = HI_SUCCESS;
    HIFB_EXPORT_FUNC_S  *pstFbFuncs = HI_NULL;

    DRV_MCE_Lock();

    switch (cmd)
    {
        case HI_MCE_STOP_FASTPLAY_CMD:
        {
#ifdef HI_MCE_SUPPORT
            Ret = HI_DRV_MCE_Stop((HI_UNF_MCE_STOPPARM_S *)arg);
#endif
            break;
        }
        case HI_MCE_EXIT_FASTPLAY_CMD:
        {
#ifdef HI_MCE_SUPPORT
            Ret = HI_DRV_MCE_Exit((HI_UNF_MCE_EXITPARAM_S *)arg);
#endif
            break;
        }
        case HI_MCE_CLEAR_LOGO_CMD:
        {
            Ret = HI_DRV_MODULE_GetFunction(HI_ID_FB, (HI_VOID **)&pstFbFuncs);

            if ((HI_SUCCESS == Ret) && (HI_NULL != pstFbFuncs))
            {
                (HI_VOID)pstFbFuncs->pfnHifbSetLogoLayerEnable(HI_FALSE);
            }

            break;
        }
        default:
            DRV_MCE_UnLock();
            return -ENOIOCTLCMD;
    }

    DRV_MCE_UnLock();

    return Ret;
}

static long MCE_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    HI_S32 Ret;

    Ret = HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, MCE_Ioctl);

    return Ret;
}

HI_S32 MCE_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return HI_SUCCESS;
}

HI_S32 MCE_Resume(PM_BASEDEV_S *pdev)
{
    return HI_SUCCESS;
}

#if (1 == HI_PROC_SUPPORT)
static MCE_REGISTER_PARAM_S g_MceProcPara = {
    .rdproc = MCE_ProcRead,
    .wtproc = MCE_ProcWrite,
};
#endif

static UMAP_DEVICE_S g_MceRegisterData;


static struct file_operations g_MceFops =
{
    .owner          =   THIS_MODULE,
    .open           =   MCE_DRV_Open,
    .unlocked_ioctl =   MCE_DRV_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl   =   MCE_DRV_Ioctl,
#endif
    .release        =   MCE_DRV_Close,
};

static PM_BASEOPS_S g_MceDrvOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = MCE_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = MCE_Resume,
};

HI_S32 MCE_DRV_ModInit(HI_VOID)
{
    HI_S32              Ret;
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR             ProcName[16];
    DRV_PROC_ITEM_S     *pProcItem = HI_NULL;
#endif
    Ret = HI_DRV_MODULE_Register(HI_ID_FASTPLAY, MCE_NAME, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_MCE("register fastplay failed.\n");
        return HI_FAILURE;
    }

#if (1 == HI_PROC_SUPPORT)
    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_MCE);

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if(HI_NULL != pProcItem)
    {
        pProcItem->read = g_MceProcPara.rdproc;
        pProcItem->write = g_MceProcPara.wtproc;
    }
#endif

    HI_OSAL_Snprintf(g_MceRegisterData.devfs_name, sizeof(g_MceRegisterData.devfs_name), UMAP_DEVNAME_MCE);
    g_MceRegisterData.fops = &g_MceFops;
    g_MceRegisterData.minor = UMAP_MIN_MINOR_MCE;
    g_MceRegisterData.owner  = THIS_MODULE;
    g_MceRegisterData.drvops = &g_MceDrvOps;
    if (HI_DRV_DEV_Register(&g_MceRegisterData) < 0)
    {
        HI_FATAL_MCE("register MCE failed.\n");
        return HI_FAILURE;
    }

    return  0;
}

HI_VOID MCE_DRV_ModExit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR             ProcName[16];
#endif

    HI_DRV_DEV_UnRegister(&g_MceRegisterData);

#if (1 == HI_PROC_SUPPORT)
    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_MCE);
    HI_DRV_PROC_RemoveModule(ProcName);
#endif

    HI_DRV_MODULE_UnRegister(HI_ID_FASTPLAY);
}

#ifdef MODULE
module_init(MCE_DRV_ModInit);
module_exit(MCE_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

