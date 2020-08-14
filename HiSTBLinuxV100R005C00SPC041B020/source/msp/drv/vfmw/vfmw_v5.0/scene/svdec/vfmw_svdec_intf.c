/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAVvVw4ERKDSiimgdao4nwOhxbHUZ3TWiJ5iG78
Gfd3kl5GX1vs1415DqtUpFUylTtZnfaqxZvJsNXp917qoh0ReeV9iHooecjEYjz3Ua5udHcv
K/Vbab9oDxMtXdWsBH2fYwco9wQr8Cd4ICMJ0Kh8Dw58pDpQ8q1+kzfVwDjs8A==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/






























#include "hi_type.h"
#include "hi_common.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_debug.h"
#include "syn_cmn.h"
#include "mem_manage.h"
#include "public.h"
//#include "syntax.h"

#include "vfmw.h"
#include "vfmw_svdec_ext.h"
#include "vfmw_debug.h"
#include "vfmw_ctrl.h"

#include "vfmw_osal_ext.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

#define SVDEC_NAME       "HI_SVDEC"

VFMW_SVDEC_EXPORT_FUNC_S s_VfmwSvdecExportFuncs;


#if 0
/* svdec use interface */
static VFMW_SVDEC_EXPORT_FUNC_S s_VfmwSvdecExportFuncs =
{
    .pfnVfmwKlibFlushCache          = klib_flush_cache,
    .pfnVfmwKernelFlushCache        = KernelFlushCache,
    .pfnVfmwMemMapRegisterAddr      = MEM_MapRegisterAddr,
    .pfnVfmwInsertImgToVoQueue      = InsertImgToVoQueue,
#ifndef HI_ADVCA_FUNCTION_RELEASE
    .pfnVfmwDprint                  = dprint_vfmw,
#else
    .pfnVfmwDprint                  = vfmw_dprint_nothing,
#endif
    .pfnVfmwVctrlGetChanIDByCtx     = VCTRL_GetChanIDByCtx,
    .pfnVfmwMemVir2Phy              = MEM_Vir2Phy,
    .pfnVfmwMemPhy2Vir              = MEM_Phy2Vir,
    .pfnVfmwResetVoQueue            = ResetVoQueue,
    .pfnVfmwRegisterSoftDecoder     = VCTRL_RegisterSoftDecoder,
    .pfnVfmwUnRegisterSoftDecoder   = VCTRL_UnRegisterSoftDecoder,
    .pfnVfmwLastFrameNotify         = VCTRL_SvdecLastFrame,
};
#endif


HI_S32 VFMW_SVDE_DRV_Init(HI_VOID)
{
    HI_S32  ret;

    s_VfmwSvdecExportFuncs.pfnVfmwKlibFlushCache        = vfmw_Osal_Func_Ptr_S.pfun_Osal_FlushCache;
    s_VfmwSvdecExportFuncs.pfnVfmwKernelFlushCache      = vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelFlushCache;

    s_VfmwSvdecExportFuncs.pfnVfmwMemMapRegisterAddr    = MEM_MapRegisterAddr;
    s_VfmwSvdecExportFuncs.pfnVfmwInsertImgToVoQueue    = InsertImgToVoQueue;

#ifndef HI_ADVCA_FUNCTION_RELEASE
    s_VfmwSvdecExportFuncs.pfnVfmwDprint                = dprint_vfmw;
#else
    s_VfmwSvdecExportFuncs.pfnVfmwDprint                = vfmw_dprint_nothing;
#endif

    s_VfmwSvdecExportFuncs.pfnVfmwVctrlGetChanIDByCtx   = VCTRL_GetChanIDByCtx;
    s_VfmwSvdecExportFuncs.pfnVfmwMemVir2Phy            = MEM_Vir2Phy;
    s_VfmwSvdecExportFuncs.pfnVfmwMemPhy2Vir            = MEM_Phy2Vir;
    s_VfmwSvdecExportFuncs.pfnVfmwResetVoQueue          = ResetVoQueue;
    s_VfmwSvdecExportFuncs.pfnVfmwRegisterSoftDecoder   = VCTRL_RegisterSoftDecoder;
    s_VfmwSvdecExportFuncs.pfnVfmwUnRegisterSoftDecoder = VCTRL_UnRegisterSoftDecoder;
    s_VfmwSvdecExportFuncs.pfnVfmwLastFrameNotify       = VCTRL_SvdecLastFrame;

    ret = HI_DRV_MODULE_Register(HI_ID_SVDEC, SVDEC_NAME, (HI_VOID *)&s_VfmwSvdecExportFuncs);

    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VFMW("HI_DRV_MODULE_SVDEC_Register failed\n");
        return ret;
    }

    HI_INFO_VFMW("inner vfmw mod init OK\n");
    return HI_SUCCESS;
}

HI_VOID VFMW_SVDEC_DRV_Exit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_SVDEC);
}

