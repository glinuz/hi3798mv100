/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAVvVw4ERKDSiimgdao4nwOhxbHUZ3TWiJ5iG78
Gfd3kl4mhQ5PKaW8bqfJj0ceB5m7Qi7MramDVZmGxInSaieACT+Yo766W8efXwsEYufznzy2
4UjxPMll5L+c0ltWzLAPha5ADtiUt2QHAzgmS6N8/uqUNn17Jv1NJ0ydg8h4Wg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/





























/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
    文 件 名   : vdec_firmware.c
    版 本 号   : 初稿
    作    者   :
    生成日期   :
    最近修改   :
    功能描述   : firmware的对外接口实现


 修改历史   :
    1.日    期 : 2009-01-08
    作    者   : z56361
    修改内容   :

******************************************************************************/

#include "vfmw.h"
#include "product.h"
#include "ctrl_case.h"
#include "mem_manage.h"
#include "public.h"
#include "vfmw_ctrl.h"
#include "syntax.h"
#include "vfmw_osal_ext.h"

#ifdef ENV_ARMLINUX_KERNEL
#include "vfmw_osal.h"
#elif defined(ENV_SOS_KERNEL)
#include "trusted_vfmw_osal.h"
#endif


/*======================================================================*/
/*   全局数据                                                           */
/*======================================================================*/
SINT32 g_VfmwInitCount  = 0;
VDMHAL_FUN_PTR_S g_vdm_hal_fun_ptr;
Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;


/*======================================================================*/
/*   外部数据                                                           */
/*======================================================================*/
//extern OSAL_IRQ_SPIN_LOCK g_SpinLock_fsp;
//extern OSAL_IRQ_SPIN_LOCK g_SpinLockIrq_Destroy;
//extern OSAL_IRQ_SPIN_LOCK g_ActivateLock;

//extern SINT32  g_SleepTime;
#ifdef ENV_SOS_KERNEL
extern MEMORY_NEEDED_SECVFMW_S *g_pVfmSharewMem;
extern RAW_ARRAY_NS *g_pStreamList;
extern CALLBACK_ARRAY_NS       *g_pCallBackList;
extern VDEC_CHAN_OPTION_S      *g_pChanOption;
extern VDEC_CHAN_STATE_S       *g_pChanState;
extern IMAGE_QUEUE_NS          *g_pImageList;
extern UINT8                   *g_pProcBuf;
#endif


/*======================================================================*/
/*   内部接口                                                           */
/*======================================================================*/

/************************************************************************
    VDEC 对VFMW所有的静态资源进行初始化
 ************************************************************************/
VOID VDEC_InitStaticResource(VOID)
{
    SINT32 ChanID = 0;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_TASKLOCK);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_RECORDLOCK);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_SPINLOCK);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_SPINLOCK_FSP);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_FRAMEBUF);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_SPINLOCKIRQ_DESTROY);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_ACTIVATELOCK);

#if defined(VFMW_DNR_SUPPORT) || defined(VFMW_BTL_SUPPORT)
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit(G_SPINLOCKISR_IMAGE2D);
#endif

    for (ChanID = 0; ChanID < MAX_CHAN_NUM; ChanID++)
    {
        vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaInit(G_RLSSTREAM_SEM, ChanID);
    }

    vfmw_Osal_Func_Ptr_S.pfun_Osal_InitEvent(G_INTWAITQUEUE, 1);

    return;
}


#ifdef ENV_SOS_KERNEL
SINT32 VDEC_InitSecureShareData(VDH_SHARE_DATA_S *ShareData)
{
    SINT32 ret = VDEC_ERR;

#ifdef VDH_DISTRIBUTOR_ENABLE
    VCTRL_WriteProc(1, ShareData->hal_disable);
#endif

    VCTRL_WriteProc(0, ShareData->sec_print_word);

    dprint(PRN_ERROR, "SOS:share_mem_addr=0x%x\n", ShareData->share_mem_addr);

    g_pVfmSharewMem = (MEMORY_NEEDED_SECVFMW_S *)vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMmap(ShareData->share_mem_addr);
    if (g_pVfmSharewMem == NULL)
    {
        dprint(PRN_ERROR, "Map g_pVfmSharewMem Failed!\n");

        return VDEC_ERR;
    }

    g_pCallBackList = &g_pVfmSharewMem->CallBackBuf;
    g_pChanOption   = &g_pVfmSharewMem->ChanOption;
    g_pStreamList   = g_pVfmSharewMem->StreamBuf;
    g_pChanState    = g_pVfmSharewMem->ChanState;
    g_pImageList    = g_pVfmSharewMem->ImageQue;
    g_pProcBuf      = g_pVfmSharewMem->ProcBuf;

    return VDEC_OK;
}

VOID VDEC_ExitSecureShareData(VOID)
{
    if (g_pVfmSharewMem != NULL)
    {
        vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMunMap(g_pVfmSharewMem);
		g_pVfmSharewMem = NULL;
    }

	g_pChanOption   = NULL;
	g_pStreamList   = NULL;
    g_pCallBackList = NULL;
    g_pChanState    = NULL;
    g_pImageList    = NULL;
	g_pProcBuf      = NULL;

    return ;
}
#endif

#ifdef VDH_DISTRIBUTOR_ENABLE
/************************************************************************
    硬件抽象层初始化接口: 区分安全和非安的初始化
 ************************************************************************/
SINT32 VDEC_InitDistributor(VDEC_OPERATION_S *pArgs)
{
    SINT32 ret = VDEC_ERR;

#if defined(ENV_ARMLINUX_KERNEL)
    ret = VDH_Normal_Init();
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "VDH_Init failed, return %d\n", ret);
        return VDEC_ERR;
    }
#elif defined(ENV_SOS_KERNEL)
    ret = VDH_Secure_Init(&pArgs->ShareData);
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "VDH_Secure_Init failed, return %d\n", ret);
        return VDEC_ERR;
    }
#endif

    return VDEC_OK;
}

/************************************************************************
    硬件抽象层去初始化接口: 区分安全和非安的去初始化
 ************************************************************************/
VOID VDEC_ExitDistributor(VOID)
{
    SINT32 ret = VDH_ERR;

#if defined(ENV_ARMLINUX_KERNEL)
    ret = VDH_Exit();
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "VDH_Exit failed, return %d\n", ret);
    }
#elif defined(ENV_SOS_KERNEL)
    ret = VDH_UnMap_ShareData();
    if (ret != VDH_OK)
    {
        dprint(PRN_ERROR, "VDH_UnMap_ShareData failed, return %d\n", ret);
    }
#endif

    return;
}
#endif

SINT32 VDEC_InitHardware(VDEC_OPERATION_S *pArgs)
{
    SINT32 ret = VDEC_ERR;

    MMUDRV_OpenHardware(0);

#ifdef VDH_DISTRIBUTOR_ENABLE
    if (g_HalDisable != 1)
    {
        ret = VDEC_InitDistributor(pArgs);
    }
    else
#endif
    {
        VDMDRV_OpenHardware(0);
        VDMHAL_GlbReset();

    #ifdef HI_SMMU_SUPPORT
        VDMHAL_SetSmmuPageTableAddr(0);
        VDMHAL_EnableSmmu();
    #endif
        VDMDRV_CloseHardware(0);

        ret = VDEC_OK;
    }

    return ret;
}

VOID VDEC_ExitHardware(VOID)
{
#ifdef VDH_DISTRIBUTOR_ENABLE
    if (g_HalDisable != 1)
    {
        VDEC_ExitDistributor();
    }
#endif

    MMUDRV_CloseHardware(0);

    return;
}

/*======================================================================*/
/*   对外接口                                                           */
/*======================================================================*/

/************************************************************************
    打开/退出VDEC组件: 在insmod/rmmod时调用如下函数，
    主要职责是创建/销毁proc文件系统
 ************************************************************************/
VOID VDEC_OpenModule(VOID)
{
#ifdef ENV_ARMLINUX_KERNEL
    InitVfmwInterface();
    vfmw_Osal_Func_Ptr_S.pfun_Osal_vfmw_proc_init();
#elif defined(ENV_SOS_KERNEL)
    InitTrustedVfmwInterface();
#endif

	vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaInit(G_INIT_DEINIT_SEM, 0xFFFFFFFF);

    PDT_Init(&g_vdm_hal_fun_ptr);
}

VOID VDEC_ExitModule(VOID)
{
#ifdef ENV_ARMLINUX_KERNEL
    vfmw_Osal_Func_Ptr_S.pfun_Osal_vfmw_proc_exit();
#endif
}

/************************************************************************
    VDEC全局初始化，带扩展配置信息
 ************************************************************************/
SINT32 VDEC_Init(VDEC_OPERATION_S *pArgs)
{
    SINT32 ret = VDEC_ERR;

    if (pArgs == NULL)
    {
        dprint(PRN_ERROR, "VDEC_InitWithOperation has NULL parameter, error!\n");
        return VDEC_ERR;
    }

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaDown(G_INIT_DEINIT_SEM, 0xFFFFFFFF);

    if (g_VfmwInitCount >= 1)
    {
        goto OK_EXIT;
    }

    AcceleratorCharacter = NULL;

	VDEC_InitStaticResource();

    MEM_ManagerWithOperation(pArgs);

#ifdef ENV_SOS_KERNEL
    ret = VDEC_InitSecureShareData(&pArgs->ShareData);
    if (ret != VDEC_OK)
    {
        dprint(PRN_ERROR, "VDEC_InitSecureVFMW Failed!\n");
        goto ERR_EXIT;
    }
#endif

    ret = VCTRL_OpenVfmw(pArgs);
    if (ret != VCTRL_OK)
    {
        dprint(PRN_ERROR, "VCTRL_OpenVfmw Failed!\n");
        goto ERR_EXIT;
    }

    ret = VDEC_InitHardware(pArgs);
    if (ret != VDEC_OK)
    {
        dprint(PRN_ERROR, "VDEC_InitHardware Failed!\n");
        VCTRL_CloseVfmw();
        goto ERR_EXIT;
    }

OK_EXIT:
    g_VfmwInitCount++;
    VCTRL_SetCallBack(pArgs->eAdapterType, pArgs->VdecCallback);

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaUp(G_INIT_DEINIT_SEM, 0xFFFFFFFF);

    return VDEC_OK;

ERR_EXIT:
    g_VfmwInitCount = 0;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaUp(G_INIT_DEINIT_SEM, 0xFFFFFFFF);

    return VDEC_ERR;
}


/************************************************************************
    VDEC全局去初始化
 ************************************************************************/
SINT32 VDEC_Exit(VOID)
{
    SINT8  IsErrorFlag = 0;
    SINT32 ret = VDEC_ERR;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaDown(G_INIT_DEINIT_SEM, 0xFFFFFFFF);

    g_VfmwInitCount = (g_VfmwInitCount > 0) ? g_VfmwInitCount-1 : 0;

    if (g_VfmwInitCount > 0)
    {
		vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaUp(G_INIT_DEINIT_SEM, 0xFFFFFFFF);
        return VDEC_OK;
    }

    ret = VCTRL_CloseVfmw();
	if (ret != VCTRL_OK)
    {
        dprint(PRN_FATAL, "%s call close vfmw failed!\n", __func__);
        IsErrorFlag = 1;
    }

#ifdef ENV_SOS_KERNEL
    VDEC_ExitSecureShareData();
#endif

    VDEC_ExitHardware();

    MEM_ManagerClearOperation();

    g_VfmwInitCount = 0;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaUp(G_INIT_DEINIT_SEM,0xFFFFFFFF);

    return  (IsErrorFlag == 0) ? VDEC_OK : VDEC_ERR;
}

/************************************************************************
    VDEC控制引擎
    ChanID:  需要操作的通道号(对于通道无关操作，此参数可为任意值)
    eCmdID:  命令编码，指定需要VDEC执行何种动作
    pArgs:   命令参数，其格式与eCmdID相关
 ************************************************************************/
SINT32 VDEC_Control(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs)
{
    if (g_VfmwInitCount <= 0)
    {
        dprint(PRN_FATAL, "%s: VFMW has been destroyed, so any operation is unuseful!\n", __func__);
        return VDEC_ERR;
    }

    return CASE_HandleEntry(ChanID, eCmdID, pArgs);
}

/************************************************************************
    解码器待机接口
 ************************************************************************/
SINT32 VDEC_Suspend(VOID)
{
    VCTRL_Suspend();
    dprint(PRN_FATAL, "VDEC_Suspend ok !\n");

    return VDEC_OK;
}

SINT32 VDEC_Resume(VOID)
{
    VCTRL_Resume();
    dprint(PRN_FATAL, "VDEC_Resume ok !\n");

    return VDEC_OK;
}

/************************************************************************
    解码器获取/释放图像接口
 ************************************************************************/
SINT32 VDEC_GetImage(SINT32 ChanID, IMAGE *pImage)
{
    return VCTRL_GetChanImage(ChanID, pImage);
}

SINT32 VDEC_ReleaseImage(SINT32 ChanID, IMAGE *pImage)
{
    return VCTRL_ReleaseChanImage(ChanID, pImage);
}

#ifdef VFMW_KTEST
EXPORT_SYMBOL(VDEC_GetImage);
EXPORT_SYMBOL(VDEC_ReleaseImage);
#endif

