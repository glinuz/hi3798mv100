#include "hi_drv_vpss.h"
#include "vpss_ctrl.h"
#include "drv_vpss_ext.h"
#include "vpss_common.h"
#include "vpss_instance.h"
#include "hi_drv_module.h"
#include "hi_drv_disp.h"

#include "vpss_src.h"
/*for HI_DECLARE_MUTEX*/
#include "hi_kernel_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define BUF_DBG_OUT 0
#define BUF_DBG_IN 0

HI_DECLARE_MUTEX(g_VpssMutex);

static const HI_CHAR    g_VpssDevName[] = "/dev/"UMAP_DEVNAME_VPSS;

static VPSS_EXPORT_FUNC_S s_VpssExportFuncs =
{
    .pfnVpssGlobalInit = HI_DRV_VPSS_GlobalInit,
    .pfnVpssGlobalDeInit = HI_DRV_VPSS_GlobalDeInit,
    
    .pfnVpssGetDefaultCfg = HI_DRV_VPSS_GetDefaultCfg,
    .pfnVpssCreateVpss = HI_DRV_VPSS_CreateVpss,
    .pfnVpssDestroyVpss = HI_DRV_VPSS_DestroyVpss,
    .pfnVpssSetVpssCfg = HI_DRV_VPSS_SetVpssCfg,
    .pfnVpssGetVpssCfg = HI_DRV_VPSS_GetVpssCfg,

    .pfnVpssGetDefaultPortCfg = HI_DRV_VPSS_GetDefaultPortCfg,
    .pfnVpssCreatePort = HI_DRV_VPSS_CreatePort,
    .pfnVpssDestroyPort = HI_DRV_VPSS_DestroyPort,
    .pfnVpssGetPortCfg = HI_DRV_VPSS_GetPortCfg,
    .pfnVpssSetPortCfg = HI_DRV_VPSS_SetPortCfg,
    .pfnVpssEnablePort = HI_DRV_VPSS_EnablePort,

    .pfnVpssSendCommand = HI_DRV_VPSS_SendCommand,

    .pfnVpssGetPortFrame = HI_DRV_VPSS_GetPortFrame,    
    .pfnVpssRelPortFrame = HI_DRV_VPSS_RelPortFrame,

    .pfnVpssGetPortBufListState = HI_DRV_VPSS_GetPortBufListState,
    .pfnVpssCheckPortBufListFul = HI_NULL,

    .pfnVpssSetSourceMode = HI_DRV_VPSS_SetSourceMode,
    .pfnVpssPutImage = HI_DRV_VPSS_PutImage,
    .pfnVpssGetImage = HI_DRV_VPSS_GetImage,

    .pfnVpssRegistHook = HI_DRV_VPSS_RegistHook,
    
};

static UMAP_DEVICE_S g_VpssRegisterData;

static struct file_operations s_VpssFileOps =
{
    .owner          = THIS_MODULE,
    .open           = NULL,
    .unlocked_ioctl = NULL,
    .release        = NULL,
};

HI_S32 HI_DRV_VPSS_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VPSS_Resume(PM_BASEDEV_S *pdev)
{
    return HI_SUCCESS;
}

static PM_BASEOPS_S  s_VpssBasicOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = HI_DRV_VPSS_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = HI_DRV_VPSS_Resume,
};

HI_S32 HI_DRV_VPSS_Init(HI_VOID)
{
    HI_S32 s32Ret;
    
    s32Ret = HI_DRV_MODULE_Register(HI_ID_VPSS,VPSS_NAME,(HI_VOID*)&s_VpssExportFuncs);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Regist HI_ID_VPSS failed\n");
        return HI_FAILURE;
    }

    s32Ret = VPSS_CTRL_Init();
    if (s32Ret != HI_SUCCESS)
    {
        goto VPSS_Init_UnRegist_Module;
    }
    
    VPSS_CTRL_SetMceFlag(HI_TRUE);
    
    return HI_SUCCESS;

VPSS_Init_UnRegist_Module:
    HI_DRV_MODULE_UnRegister(HI_ID_VPSS);
    
    return HI_FAILURE;
}

HI_VOID HI_DRV_VPSS_Exit(HI_VOID)
{   
    VPSS_FATAL("Can't be supported\n");    
}

HI_S32 VPSS_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_OSAL_Snprintf(g_VpssRegisterData.devfs_name, 64, UMAP_DEVNAME_VPSS);

    g_VpssRegisterData.fops   = &s_VpssFileOps;
    g_VpssRegisterData.minor  = UMAP_MIN_MINOR_VPSS;
    g_VpssRegisterData.owner  = THIS_MODULE;
    g_VpssRegisterData.drvops = &s_VpssBasicOps;
    
    if (HI_DRV_DEV_Register(&g_VpssRegisterData) < 0)
    {
        VPSS_FATAL("register VPSS failed.\n");
        return HI_FAILURE;
    }
    
    s32Ret = HI_DRV_MODULE_Register(HI_ID_VPSS,VPSS_NAME,(HI_VOID*)&s_VpssExportFuncs);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Regist HI_ID_VPSS failed\n");
        goto DRV_Dev_UnRegister;
    }
    
    s32Ret = VPSS_CTRL_Init();
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("VPSS_CTRL_Init Failed\n");
        goto DRV_Dev_MODULE_UnRegist;
    }

    VPSS_CTRL_SetMceFlag(HI_FALSE);
    
    return HI_SUCCESS;
    
DRV_Dev_MODULE_UnRegist:
    HI_DRV_MODULE_UnRegister(HI_ID_VPSS);

DRV_Dev_UnRegister:
    HI_DRV_DEV_UnRegister(&g_VpssRegisterData);

    return HI_FAILURE;
    
}

HI_VOID VPSS_DRV_Exit(HI_VOID)
{
    (HI_VOID)VPSS_CTRL_DelInit();
    (HI_VOID)HI_DRV_MODULE_UnRegister(HI_ID_VPSS);
    (HI_VOID)HI_DRV_DEV_UnRegister(&g_VpssRegisterData);
}

HI_S32 VPSS_DRV_ModInit(HI_VOID)
{
    VPSS_DRV_Init();

#ifdef MODULE
    HI_PRINT("Load hi_vpss.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID VPSS_DRV_ModExit(HI_VOID)
{    
    VPSS_DRV_Exit();
    
#ifdef MODULE
    HI_PRINT("Unload hi_vpss.ko success.\t(%s)\n", VERSION_STRING);
#endif
}

HI_S32 HI_DRV_VPSS_GlobalInit(HI_VOID)
{
    HI_S32 s32Ret;
    
    s32Ret = VPSS_CTRL_Init();
    if (s32Ret == HI_FAILURE)
    {
        VPSS_FATAL("GlobalInit Error.\n");
    }
    return s32Ret;
      
}

HI_S32 HI_DRV_VPSS_GlobalDeInit(HI_VOID)
{
    HI_S32 s32Ret;
	
    s32Ret = VPSS_CTRL_DelInit();
    if (s32Ret == HI_FAILURE)
    {
        VPSS_FATAL("GlobalDeInit Error.\n");
    }
    
    return s32Ret;
}

HI_S32  HI_DRV_VPSS_GetDefaultCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    VPSS_INST_GetDefInstCfg(pstVpssCfg);

    return HI_SUCCESS;
}

HI_S32  HI_DRV_VPSS_CreateVpss(HI_DRV_VPSS_CFG_S *pstVpssCfg,VPSS_HANDLE *hVPSS)
{
    VPSS_HANDLE hInst;
    
    hInst = VPSS_CTRL_CreateInstance(pstVpssCfg);
    if(hInst != VPSS_INVALID_HANDLE)
    {
        *hVPSS = hInst;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32  HI_DRV_VPSS_DestroyVpss(VPSS_HANDLE hVPSS)
{
    HI_S32 s32Ret;
    
    s32Ret = down_interruptible(&g_VpssMutex);
    s32Ret = VPSS_CTRL_DestoryInstance(hVPSS);  
	up(&g_VpssMutex);
    
    return s32Ret;
}

HI_S32  HI_DRV_VPSS_SetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if(pstInstance)
    {
        s32Ret = VPSS_INST_SetInstCfg(pstInstance, pstVpssCfg);
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
    
}

HI_S32  HI_DRV_VPSS_GetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    VPSS_INSTANCE_S * pstInstance;
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if(pstInstance)
    {
        VPSS_INST_GetInstCfg(pstInstance, pstVpssCfg);
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32  HI_DRV_VPSS_GetDefaultPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg)
{
    VPSS_INST_GetDefPortCfg(pstVpssPortCfg);

    return HI_SUCCESS;
}

HI_S32  HI_DRV_VPSS_CreatePort(VPSS_HANDLE hVPSS,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg,VPSS_HANDLE *phPort)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    
    if(pstInstance)
    {
        VPSS_CTRL_Pause(hVPSS);
        s32Ret = VPSS_INST_CreatePort(pstInstance, pstVpssPortCfg, phPort);
        VPSS_CTRL_Resume(hVPSS);
        return s32Ret;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32  HI_DRV_VPSS_DestroyPort(VPSS_HANDLE hPort)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if(!pstInstance)
    {
        return HI_FAILURE;
    }
    
	VPSS_CTRL_Pause(hVPSS);
    s32Ret = VPSS_INST_DestoryPort(pstInstance, hPort);
    VPSS_CTRL_Resume(hVPSS);
    
    return s32Ret;
}

HI_S32  HI_DRV_VPSS_GetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
    
    pstInstance= VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }

    s32Ret = VPSS_INST_GetPortCfg(pstInstance, hPort,pstVpssPortCfg);

    return s32Ret;
}   

HI_S32  HI_DRV_VPSS_SetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }
    
    s32Ret = VPSS_INST_CheckPortCfg(pstInstance, hPort,pstVpssPortCfg);
    
    if(s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("SetPortCfg Error.\n");
        return HI_FAILURE;
    }
    s32Ret = VPSS_INST_SetPortCfg(pstInstance, hPort,pstVpssPortCfg);

    return s32Ret;
}

HI_S32  HI_DRV_VPSS_EnablePort(VPSS_HANDLE hPort, HI_BOOL bEnable)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    
    if(!pstInstance)
    {
        return HI_FAILURE;
    }
    
    //VPSS_OSAL_DownLock(&(pstInstance->stInstLock));
    s32Ret = VPSS_INST_EnablePort(pstInstance, hPort,bEnable);
    //VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
    return s32Ret;
}

HI_S32  HI_DRV_VPSS_SendCommand(VPSS_HANDLE hVPSS, HI_DRV_VPSS_USER_COMMAND_E eCommand, HI_VOID *pArgs)
{   
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret = HI_SUCCESS;

    if (eCommand == HI_DRV_VPSS_USER_COMMAND_IMAGEREADY)
    {
        s32Ret = VPSS_CTRL_WakeUpThread();
        goto CMD_OUT;
    }
    
    s32Ret = down_interruptible(&g_VpssMutex); 
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    
    if(!pstInstance)
    {
        up(&g_VpssMutex);
        return HI_FAILURE;
    }
	
    s32Ret = VPSS_INST_ReplyUserCommand(pstInstance,eCommand,pArgs);

    
	up(&g_VpssMutex);
	
CMD_OUT:
    return s32Ret;
    
}

HI_S32  HI_DRV_VPSS_GetPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    
    if (!pstInstance)
    {
        return HI_FAILURE;
    }

    s32Ret = VPSS_INST_GetPortFrame(pstInstance,hPort,pstVpssFrame);
    
    if (s32Ret == HI_SUCCESS)
    { 
        #if BUF_DBG_OUT
            HI_PRINT("%s h %#x Get %d addr %#x\n",
                    __func__,
                    hPort,
                    pstVpssFrame->u32FrameIndex,
                    pstVpssFrame->stBufAddr[0].u32PhyAddr_Y);
        #endif
        VPSS_INFO("\n Port = %d GetPortFrame %d Success",hPort,pstVpssFrame->u32FrameIndex);
	}
    else
    {
        VPSS_INFO("\n Port = %d GetPortFrame Failed",hPort);
    }


    return s32Ret;
}

HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }
    #if BUF_DBG_IN
        HI_PRINT("%s h %#x Rel %d addr %#x\n",
                    __func__,
                    hPort,
                    pstVpssFrame->u32FrameIndex,
                    pstVpssFrame->stBufAddr[0].u32PhyAddr_Y);
    #endif
    s32Ret = VPSS_INST_RelPortFrame(pstInstance,hPort,pstVpssFrame);

    if(s32Ret != HI_SUCCESS)
    {
        VPSS_INFO("\t\n Port = %d RelPortFrame %d Failed",hPort,pstVpssFrame->u32FrameIndex);
    }
    else
    {   
        VPSS_INFO("\n Port = %d RelPortFrame %d Success",hPort,pstVpssFrame->u32FrameIndex);
    }
    
    (HI_VOID)VPSS_CTRL_WakeUpThread();

    return s32Ret;
}

HI_S32  HI_DRV_VPSS_RegistHook(VPSS_HANDLE hVPSS, HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }
    s32Ret = VPSS_INST_SetCallBack(pstInstance, hDst, pfVpssCallback);

    return s32Ret;
    
}

HI_S32 HI_DRV_VPSS_PutImage(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage)
{
    VPSS_INSTANCE_S * pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }

    return VPSS_SRCIN_SendImage(&pstInstance->stSrcIn, pstImage);
}

HI_S32 HI_DRV_VPSS_GetImage(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage)
{
    VPSS_INSTANCE_S * pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }

    return VPSS_SRCIN_CallImage(&pstInstance->stSrcIn, pstImage);
}

HI_S32 HI_DRV_VPSS_SetSourceMode(VPSS_HANDLE hVPSS,
                          HI_DRV_VPSS_SOURCE_MODE_E eSrcMode,
                          HI_DRV_VPSS_SOURCE_FUNC_S* pstRegistSrcFunc)
{
    VPSS_INSTANCE_S * pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if(!pstInstance)
    {
        return HI_FAILURE;
    }

    switch(eSrcMode)
    {
        case VPSS_SOURCE_MODE_USERACTIVE:
            return VPSS_INST_SetUserActiveMode(pstInstance);
            break;
        case VPSS_SOURCE_MODE_VPSSACTIVE:
            if(pstRegistSrcFunc == HI_NULL)
            {
                VPSS_FATAL("pstRegistSrcFunc is NULL.\n");
                return HI_FAILURE;
            }
            else
            {
                pstInstance->eSrcImgMode = VPSS_SOURCE_MODE_VPSSACTIVE;
                if (pstRegistSrcFunc->VPSS_GET_SRCIMAGE == HI_NULL
                    || pstRegistSrcFunc->VPSS_REL_SRCIMAGE== HI_NULL)
                {
                    VPSS_FATAL("VPSS_GET_SRCIMAGE || VPSS_REL_SRCIMAGE is NULL.\n");
                    return HI_FAILURE;
                }
                else
                {
                    VPSS_IN_SOURCE_S stSrcInfo;
                    pstInstance->stSrcFuncs.VPSS_GET_SRCIMAGE = 
                                    pstRegistSrcFunc->VPSS_GET_SRCIMAGE;
                    pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE = 
                                    pstRegistSrcFunc->VPSS_REL_SRCIMAGE; 
                    
                    stSrcInfo.pfnAcqCallback = (PFN_IN_RlsCallback)pstInstance->stSrcFuncs.VPSS_GET_SRCIMAGE;
                    stSrcInfo.pfnRlsCallback = (PFN_IN_RlsCallback)pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE;
                    stSrcInfo.enMode = VPSS_SOURCE_MODE_VPSSACTIVE;
                    stSrcInfo.hSource = pstInstance->ID;
                    VPSS_IN_SetSrcMode(&(pstInstance->stInEntity), stSrcInfo);
                }
            }
            return HI_SUCCESS;
            break;
        default:
            VPSS_FATAL("SourceMode is invalid.\n");
            return HI_FAILURE;
            break;
    }
}

HI_S32  HI_DRV_VPSS_GetPortBufListState(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstVpssBufListState)
{
    VPSS_INSTANCE_S * pstInstance;
    HI_S32 s32Ret;
    VPSS_HANDLE hVPSS;
    hVPSS = PORTHANDLE_TO_VPSSID(hPort);
	
    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if(pstInstance)
    {
        s32Ret = VPSS_INST_GetPortListState(pstInstance,hPort,pstVpssBufListState);
        return s32Ret;
    }
    else
    {
        return HI_FAILURE;
    }
    
}

HI_S32  HI_DRV_VPSS_InvokeDisplayInfo(HI_DISP_DISPLAY_INFO_S *pstDisplayInfo)
{
	HI_U32 u32fmt;
	if (pstDisplayInfo->bIsMaster)
	{
		VPSS_INSTANCE_S *pstInstance;

		pstInstance = VPSS_CTRL_GetOnlyInstance(VPSS_IP_0);

		if (pstInstance)
		{
			if (pstDisplayInfo->stFmtResolution.s32Width == 3840
					&& pstDisplayInfo->stFmtResolution.s32Height == 2160)
			{
				if (atomic_read(&pstInstance->atmUhdFmt) == 0)
				{
					atomic_inc(&(pstInstance->atmUhdFmt));
					u32fmt = atomic_read(&(pstInstance->atmUhdFmt));
				}

			}
			else
			{
				if (atomic_read(&pstInstance->atmUhdFmt) != 0)
				{
					atomic_dec(&(pstInstance->atmUhdFmt));
					u32fmt = atomic_read(&(pstInstance->atmUhdFmt));
				}
			}
		}
	}

	return HI_SUCCESS; 
}

#ifdef MODULE
module_init(VPSS_DRV_ModInit);
module_exit(VPSS_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HISILICON");


EXPORT_SYMBOL(HI_DRV_VPSS_GlobalInit);
EXPORT_SYMBOL(HI_DRV_VPSS_GlobalDeInit);
EXPORT_SYMBOL(HI_DRV_VPSS_GetDefaultCfg);
EXPORT_SYMBOL(HI_DRV_VPSS_CreateVpss);
EXPORT_SYMBOL(HI_DRV_VPSS_DestroyVpss);

EXPORT_SYMBOL(HI_DRV_VPSS_GetDefaultPortCfg);
EXPORT_SYMBOL(HI_DRV_VPSS_CreatePort);
EXPORT_SYMBOL(HI_DRV_VPSS_EnablePort);
EXPORT_SYMBOL(HI_DRV_VPSS_DestroyPort);
EXPORT_SYMBOL(HI_DRV_VPSS_GetPortCfg);
EXPORT_SYMBOL(HI_DRV_VPSS_SetPortCfg);
EXPORT_SYMBOL(HI_DRV_VPSS_RegistHook);
EXPORT_SYMBOL(HI_DRV_VPSS_GetPortFrame);
EXPORT_SYMBOL(HI_DRV_VPSS_RelPortFrame);
EXPORT_SYMBOL(HI_DRV_VPSS_SendCommand);
EXPORT_SYMBOL(HI_DRV_VPSS_SetSourceMode);
EXPORT_SYMBOL(HI_DRV_VPSS_PutImage);
EXPORT_SYMBOL(HI_DRV_VPSS_GetImage);
EXPORT_SYMBOL(HI_DRV_VPSS_InvokeDisplayInfo);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

