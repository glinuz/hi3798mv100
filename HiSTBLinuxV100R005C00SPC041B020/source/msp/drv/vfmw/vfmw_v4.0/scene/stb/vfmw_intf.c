/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCrrbhPAEEtymxOObUotWP7PcjQvU/i5OJUvBcN
m5NZF/YOG/0AfFclWxFC4gXmmDSuowyX133daSRjjlPeIzaVKDVhGV/qlP4undgG/JI9dNrx
QMNfDu5X+CyCaGmlKZldil1JsWZIYUMKe4dzcFA9q8Q5Bw42EC2cJRJML4myiw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include "hi_type.h"
#include "hi_common.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_debug.h"
#include "syn_cmn.h"
#include "mem_manage.h"
#include "syntax.h"

#include "vfmw.h"
#include "vfmw_ext.h"
#include "vfmw_debug.h"

#include "vfmw_osal_ext.h"

#include "vfmw_osal.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

#define VFMW_NAME       "HI_VFMW"

#ifdef HI_TEE_SUPPORT
extern HI_VOID TVP_VDEC_OpenModule(HI_VOID);
extern HI_VOID TVP_VDEC_ExitModule(HI_VOID);
extern HI_S32  TVP_VDEC_Init(HI_S32 (*VdecCallback)(HI_S32, HI_S32, HI_VOID *));
extern HI_S32  TVP_VDEC_InitWithOperation(VDEC_OPERATION_S *pArgs);
extern HI_S32  TVP_VDEC_Exit(HI_VOID);
extern HI_S32  TVP_VDEC_Control(HI_S32 ChanID, VDEC_CID_E eCmdID, HI_VOID *pArgs);
extern HI_S32  TVP_VDEC_Suspend(HI_VOID);
extern HI_S32  TVP_VDEC_Resume(HI_VOID);
extern HI_S32  TVP_VDEC_SetDbgOption (HI_U32 opt, HI_U8 *p_args);
extern HI_S32  TVP_VDEC_TrustDecoderInit(VDEC_OPERATION_S *pArgs);
extern HI_S32  TVP_VDEC_TrustDecoderExit(HI_VOID);
extern HI_S32  TVP_VDEC_init_proc (HI_VOID);
#endif

/* svdec used vfmw function */
extern HI_S32  VFMW_SVDE_DRV_Init (HI_VOID);
extern HI_VOID VFMW_SVDEC_DRV_Exit (HI_VOID);

extern HI_VOID SVDEC_ModeInit(HI_VOID);
extern HI_VOID SVDEC_ModeExit(HI_VOID);

#ifdef HI_TEE_SUPPORT
static VFMW_EXPORT_FUNC_S s_VfmwExportFuncs =
{
    .pfnVfmwOpenModule              = TVP_VDEC_OpenModule,
    .pfnVfmwExitModule              = TVP_VDEC_ExitModule,
    .pfnVfmwInit                    = TVP_VDEC_Init,
    .pfnVfmwInitWithOperation       = TVP_VDEC_InitWithOperation,
    .pfnVfmwControl                 = TVP_VDEC_Control,
    .pfnVfmwExit                    = TVP_VDEC_Exit,
    .pfnVfmwSuspend                 = TVP_VDEC_Suspend,
    .pfnVfmwResume                  = TVP_VDEC_Resume,
    .pfnVfmwSetDbgOption            = TVP_VDEC_SetDbgOption,
    .pfnVfmwTrustDecoderInit        = TVP_VDEC_TrustDecoderInit,
    .pfnVfmwTrustDecoderExit        = TVP_VDEC_TrustDecoderExit,
};
#else
static VFMW_EXPORT_FUNC_S s_VfmwExportFuncs =
{
    .pfnVfmwOpenModule              = VDEC_OpenModule,
    .pfnVfmwExitModule              = VDEC_ExitModule,
    .pfnVfmwInit                    = VDEC_Init,
    .pfnVfmwInitWithOperation       = VDEC_InitWithOperation,
    .pfnVfmwControl                 = VDEC_Control,
    .pfnVfmwExit                    = VDEC_Exit,
    .pfnVfmwSuspend                 = VDEC_Suspend,
    .pfnVfmwResume                  = VDEC_Resume,
    .pfnVfmwSetDbgOption            = VCTRL_SetDbgOption,
    .pfnVfmwTrustDecoderInit        = HI_NULL,
    .pfnVfmwTrustDecoderExit        = HI_NULL,
};
#endif

HI_S32 VFMW_DRV_Init(HI_VOID)
{
    HI_S32  ret;

    InitVfmwInterface();
    ret = HI_DRV_MODULE_Register(HI_ID_VFMW, VFMW_NAME, (HI_VOID *)&s_VfmwExportFuncs);

    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VFMW("HI_DRV_MODULE_VDEC_Register failed\n");
        return ret;
    }

    VDEC_OpenModule();   // open proc
    HI_INFO_VFMW("inner vfmw mod init OK\n");
#ifdef HI_TEE_SUPPORT
    TVP_VDEC_OpenModule();
#endif
    return HI_SUCCESS;
}

HI_VOID VFMW_DRV_Exit(HI_VOID)
{
    VDEC_ExitModule();   // clcose proc

    HI_DRV_MODULE_UnRegister(HI_ID_VFMW);
}

HI_S32 VFMW_DRV_ModInit(void)
{
    HI_S32 ret;
    int i __attribute__((unused));
    //InitVfmwInterface();

#ifndef VFMW_SUPPORT
#ifndef HI_MCE_SUPPORT
    ret = VFMW_DRV_Init();

    if (HI_SUCCESS != ret)
    {
        return -1;
    }

#endif

    /* svdec */
    ret = VFMW_SVDE_DRV_Init();

    if (HI_SUCCESS != ret)
    {
        return -1;
    }

#endif

#ifndef HI_MCE_SUPPORT
    for (i = 0; i < MAX_VDH_NUM; i++)
    {
        VDM_CloseHardware(i);
        DSP_CloseHardware(i);
    }

    BPD_CloseHardware();
#endif

#ifdef HI_TEE_SUPPORT

#ifndef  HI_ADVCA_FUNCTION_RELEASE

    ret = TVP_VDEC_init_proc();
#endif
#endif

#ifdef MODULE
    HI_PRINT("Load hi_vfmw.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return 0;
}

HI_VOID VFMW_DRV_ModExit(void)
{
#ifndef VFMW_SUPPORT
#ifndef HI_MCE_SUPPORT
    VFMW_DRV_Exit();
#endif
    /* svdec */
    VFMW_SVDEC_DRV_Exit();
#endif

#ifdef HI_TEE_SUPPORT
    TVP_VDEC_ExitModule();
#endif

#ifdef MODULE
    HI_PRINT("Unload hi_vfmw.ko success.\t(%s)\n", VERSION_STRING);
#endif
    return ;
}

#ifdef MODULE
module_init(VFMW_DRV_ModInit);
module_exit(VFMW_DRV_ModExit);
#endif

//EXPORT_SYMBOL(vfmw_Osal_Func_Ptr_S);

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

