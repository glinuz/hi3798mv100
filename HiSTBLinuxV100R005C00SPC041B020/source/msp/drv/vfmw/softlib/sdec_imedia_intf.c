/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4XHUJ17ZfYNIPE4fzM4xa2W1E2WCg7mw+NbNzPGsbFVbkXKqVlkTr0ZC7YKTBpgPAEIi
xc8/S8t/um66r++oIzUBJhYkt+sFCpJf13CktdEqQp1CxX6sGh9Lam+OD8PtsQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/***********************************************************************
*
* Copyright (c) 2007 HUAWEI - All Rights Reserved
*
* File     : $sdec_imedia_ko.c$
* Date     : $2011/03/14$
* Revision : $v1.1$
* Purpose  : give a ko interface to the soft-dec-lib
*
* Change History:
*
* Date                       Author                          Change
* ====                       ======                          ======
* 2011/03/14                 z56361                          Original.
*
* Dependencies:
* Linux OS
*
************************************************************************/
#include "hi_common.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"

#include "sdec_imedia.h"
#include "vfmw_ctrl.h"
#include "vfmw_svdec_ext.h"

VFMW_SVDEC_EXPORT_FUNC_S *pVfmwToSvdecFun;

static UMAP_DEVICE_S g_SvdecRegisterData;

static struct file_operations SVDEC_FOPS =
{
owner   :
    THIS_MODULE,
open    :
    NULL,
unlocked_ioctl :
    NULL,
release :
    NULL,
};

static struct tagPM_BASEOPS_S SVDEC_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = NULL,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = NULL,
};

HI_S32 SVDEC_ModeInit(HI_VOID)
{
    HI_S32 s32Ret;
    iMediaSDEC_FUNC_S  stSdecFunc;

    /* ÏòVFMW×¢²áÈí½âÂë¿â */
    stSdecFunc.pfun_iMediaSoftDEC_Create = iMediaSoftDEC_Create;
    stSdecFunc.pfun_iMediaSoftDEC_Destroy = iMediaSoftDEC_Destroy;
    stSdecFunc.pfun_iMediaSoftDEC_Init = iMediaSoftDEC_Init;
    stSdecFunc.pfun_iMediaSoftDEC_Decode = iMediaSoftDEC_Decode;
    stSdecFunc.pfun_iMediaSoftDEC_RecycleImage = iMediaSoftDEC_RecycleImage;
    stSdecFunc.pfun_iMediaSoftDEC_GetRemainImg = iMediaSoftDEC_GetRemainImg;
    stSdecFunc.pfun_iMediaSoftDEC_GetImageBuffer = iMediaSoftDEC_GetImageBuffer;

    /* get vfmw export functions for svdec */
    pVfmwToSvdecFun = HI_NULL;

    /* Get vfmw functions */
    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_SVDEC, (HI_VOID **) & (pVfmwToSvdecFun));

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    //    if (VDEC_OK != VCTRL_RegisterSoftDecoder( &stSdecFunc ))
    if (VDEC_OK != (pVfmwToSvdecFun->pfnVfmwRegisterSoftDecoder)(&stSdecFunc))
    {
        return HI_FAILURE;
    }

    /* ×¢²ávsdecÉè±¸ */
    snprintf(g_SvdecRegisterData.devfs_name, sizeof(g_SvdecRegisterData.devfs_name), UMAP_DEVNAME_SVDEC);
    g_SvdecRegisterData.minor  = UMAP_MIN_MINOR_SVDEC;
    g_SvdecRegisterData.owner  = THIS_MODULE;
    g_SvdecRegisterData.fops   = &SVDEC_FOPS;
    g_SvdecRegisterData.drvops = &SVDEC_drvops;

    if (HI_DRV_DEV_Register(&g_SvdecRegisterData) < 0)
    {
        return HI_FAILURE;
    }

#ifdef MODULE
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_PRINT("Load hi_svdec.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

    return 0;
}

HI_VOID SVDEC_ModeExit(HI_VOID)
{
    HI_S32 s32Ret;

    HI_DRV_DEV_UnRegister(&g_SvdecRegisterData);

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_SVDEC, (HI_VOID **) & (pVfmwToSvdecFun));
    if (HI_SUCCESS != s32Ret)
    {
        return;
    }

    (pVfmwToSvdecFun->pfnVfmwUnRegisterSoftDecoder)();
    //    VCTRL_UnRegisterSoftDecoder();

    return;
}

#ifdef MODULE
module_init(SVDEC_ModeInit);
module_exit(SVDEC_ModeExit);
#else
EXPORT_SYMBOL(SVDEC_ModeInit);
EXPORT_SYMBOL(SVDEC_ModeExit);
#endif
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");







