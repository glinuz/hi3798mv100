/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAVvVw4ERKDSiimgdao4nwOhxbHUZ3TWiJ5iG78
Gfd3kl9zpI3vm9NdL4bdKSLpwFJgHgzIPjYaskmihxHIDMF5luJ2KE2bpi4BsngUUqQaw8y9
CdVuUMfmzLVx8D78knDH2bfjFqYx7pPUvIU12MEL9/b3fLgL4KuXol/MwVxGOg==#*/
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

#include "vfmw_adapter.h"

atomic_t ModInitCount = ATOMIC_INIT(0);

#define VFMW_NAME       "HI_VFMW"

/* svdec used vfmw function */
#ifdef VFMW_H263_SUPPORT
extern HI_S32  VFMW_SVDE_DRV_Init (HI_VOID);
extern HI_VOID VFMW_SVDEC_DRV_Exit (HI_VOID);
#endif

extern HI_VOID SVDEC_ModeInit(HI_VOID);
extern HI_VOID SVDEC_ModeExit(HI_VOID);


static VFMW_EXPORT_FUNC_S s_VfmwExportFuncs =
{
    .pfnVfmwInit                    = VFMW_Init,
    .pfnVfmwControl                 = VFMW_Control,
    .pfnVfmwExit                    = VFMW_Exit,
    .pfnVfmwSuspend                 = VFMW_Suspend,
    .pfnVfmwResume                  = VFMW_Resume,
    .pfnVfmwGetImage                = VFMW_GetImage,
    .pfnVfmwReleaseImage            = VFMW_ReleaseImage,
};

HI_S32 VFMW_DRV_Init(HI_VOID)
{
    HI_S32  ret;

    if (atomic_inc_return(&ModInitCount) == 1)
    {
        ret = HI_DRV_MODULE_Register(HI_ID_VFMW, VFMW_NAME, (HI_VOID *)&s_VfmwExportFuncs);

        if (HI_SUCCESS != ret)
        {
            HI_FATAL_VFMW("HI_DRV_MODULE_VDEC_Register failed\n");
            return ret;
        }

        VFMW_OpenModule();
        HI_INFO_VFMW("inner vfmw mod init OK\n");
    }

    return HI_SUCCESS;
}

HI_VOID VFMW_DRV_Exit(HI_VOID)
{
    atomic_set(&ModInitCount, 0);
    VFMW_ExitModule();
    HI_DRV_MODULE_UnRegister(HI_ID_VFMW);
}

HI_S32 VFMW_DRV_ModInit(void)
{
#ifndef HI_MCE_SUPPORT
    HI_U32 i;
#endif

    if (HI_SUCCESS != VFMW_DRV_Init())
    {
        return -1;
    }

    /* svdec */
#ifdef VFMW_H263_SUPPORT
    if (HI_SUCCESS != VFMW_SVDE_DRV_Init())
    {
        return -1;
    }
#endif

#ifndef HI_MCE_SUPPORT

    for (i = 0; i < MAX_VDH_NUM; i++)
    {
        VDMDRV_CloseHardware(i);
        DSPDRV_CloseHardware(i);
    }

    BPDDRV_CloseHardware(0);
#endif

#ifdef VDH_DISTRIBUTOR_ENABLE
    VDH_Init_Proc();
#endif

#ifdef MODULE
    HI_PRINT("Load hi_vfmw.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return 0;
}

HI_VOID VFMW_DRV_ModExit(void)
{
    VFMW_DRV_Exit();
    /* svdec */
#ifdef VFMW_H263_SUPPORT
    VFMW_SVDEC_DRV_Exit();
#endif

#ifdef VDH_DISTRIBUTOR_ENABLE
    VDH_Exit_Proc();
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

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

