/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_osr_k.c
Version             : Initial Draft
Author              :
Created             : 2015/07/11
Description         :
Function List       :
History             :
Date                            Author                  Modification
2015/07/11                y00181162                              Created file
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT


#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/workqueue.h>
#include <asm/io.h>
#include "drv_tde_ext.h"
#include "hi_tde_type.h"
#include "hi_drv_tde.h"
#include "tde_osictl.h"
#include "tde_osilist.h"
#include "tde_hal.h"
#include "tde_handle.h"
#include "wmalloc.h"
#include "tde_adp.h"
#include "hi_gfx_comm_k.h"
#include "hi_reg_common.h"
#include "hi_gfx_sys_k.h"

typedef unsigned long       HI_UL;
#define TDE_NAME    "HI_TDE"

STATIC spinlock_t s_taskletlock;
extern struct miscdevice gfx_dev;

STATIC spinlock_t s_TdeRefLock;
#ifdef TDE_HWC_COOPERATE
STATIC HI_U32 g_TdeIrqHandle = 0;
#endif

typedef HI_S32(*TdeCtlPtrFunc)(unsigned long *argp);
typedef struct TdeCtlFuncDispatchItem
{
    HI_U32        u32TdeCmd;
    TdeCtlPtrFunc tde_ctl_ptr_func;
}TDE_CTL_FUNC_DISPATCH_ITEM;

STATIC HI_S32 TdeOsrBeginJob(unsigned long *argp);
STATIC HI_S32 TdeOsrBitBlit(unsigned long *argp);
STATIC HI_S32 TdeOsrSolidDraw(unsigned long *argp);
STATIC HI_S32 TdeOsrQuickCopy(unsigned long *argp);
STATIC HI_S32 TdeOsrQuickResize(unsigned long *argp);
STATIC HI_S32 TdeOsrQuickFill(unsigned long *argp);
STATIC HI_S32 TdeOsrQuickDeflicker(unsigned long *argp);
STATIC HI_S32 TdeOsrMbBitBlit(unsigned long *argp);
STATIC HI_S32 TdeOsrEndJob(unsigned long *argp);
STATIC HI_S32 TdeOsrWaitForDone(unsigned long *argp);
STATIC HI_S32 TdeOsrCancelJob(unsigned long *argp);
STATIC HI_S32 TdeOsrBitMapMaskRop(unsigned long *argp);
STATIC HI_S32 TdeOsrBitMapMaskBlend(unsigned long *argp);
STATIC HI_S32 TdeOsrWaitAllDone(unsigned long *argp);
STATIC HI_S32 TdeOsrReset(unsigned long *argp);

STATIC HI_S32 TdeOsrSetDeflickerLevel(unsigned long *argp);
STATIC HI_S32 TdeOsrGetDeflickerLevel(unsigned long *argp);
STATIC HI_S32 TdeOsrSetAlphaThreshholdValue(unsigned long *argp);
STATIC HI_S32 TdeOsrGetAlphaThreshholdValue(unsigned long *argp);
STATIC HI_S32 TdeOsrSetAlphaThreshholdState(unsigned long *argp);
STATIC HI_S32 TdeOsrGetAlphaThreshholdState(unsigned long *argp);
STATIC HI_S32 TdeOsrPatternFill(unsigned long *argp);
STATIC HI_S32 TdeOsrEnableRegionDeflicker(unsigned long *argp);




TDE_CTL_FUNC_DISPATCH_ITEM g_paTdeCtlFuncDispatchItem[]= { { 0 ,NULL},\
   {TDE_BEGIN_JOB ,TdeOsrBeginJob},                                 {TDE_BIT_BLIT, TdeOsrBitBlit},\
   {TDE_SOLID_DRAW, TdeOsrSolidDraw},                               {TDE_QUICK_COPY, TdeOsrQuickCopy},\
   {TDE_QUICK_RESIZE, TdeOsrQuickResize},                           {TDE_QUICK_FILL, TdeOsrQuickFill},\
   {TDE_QUICK_DEFLICKER, TdeOsrQuickDeflicker},                     {TDE_MB_BITBLT, TdeOsrMbBitBlit},\
   {TDE_END_JOB, TdeOsrEndJob},                                     {TDE_WAITFORDONE, TdeOsrWaitForDone},\
   {TDE_CANCEL_JOB, TdeOsrCancelJob},                               {TDE_BITMAP_MASKROP, TdeOsrBitMapMaskRop},\
   {TDE_BITMAP_MASKBLEND, TdeOsrBitMapMaskBlend},                   {TDE_WAITALLDONE, TdeOsrWaitAllDone},\
   {TDE_RESET, TdeOsrReset},                                        {0, NULL},\
   {TDE_SET_DEFLICKERLEVEL, TdeOsrSetDeflickerLevel},               {TDE_GET_DEFLICKERLEVEL, TdeOsrGetDeflickerLevel},\
   {TDE_SET_ALPHATHRESHOLD_VALUE, TdeOsrSetAlphaThreshholdValue},   {TDE_GET_ALPHATHRESHOLD_VALUE, TdeOsrGetAlphaThreshholdValue},\
   {TDE_SET_ALPHATHRESHOLD_STATE, TdeOsrSetAlphaThreshholdState},   {TDE_GET_ALPHATHRESHOLD_STATE, TdeOsrGetAlphaThreshholdState},\
   {TDE_PATTERN_FILL, TdeOsrPatternFill},                           {TDE_ENABLE_REGIONDEFLICKER ,TdeOsrEnableRegionDeflicker},\
   {0, NULL },                                                      {0, NULL },\
   {0, NULL } };

#define TDE_IOCTL_FUNC_NUM     (sizeof(g_paTdeCtlFuncDispatchItem)/sizeof(HI_U32*))


#define TDE_CHECK_IOCTL_CMD(cmd)\
    do{\
        if(cmd < 1 || cmd >= TDE_IOCTL_FUNC_NUM)\
        {\
            return -EFAULT;\
        }\
    }while(0)

#define TDE_IOCTL_CHECK_PARA(argp)\
    do{\
        if(NULL == argp)\
        {\
            return -EFAULT;\
        }\
    }while(0)

STATIC int tde_osr_isr(int irq, void *dev_id);
STATIC void tde_tasklet_func(unsigned long int_status);

/* TDE equipment quoted count */
STATIC atomic_t g_TDECount = ATOMIC_INIT(0);

#ifdef CONFIG_TDE_PM_ENABLE
int tde_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);
int tde_pm_resume(PM_BASEDEV_S *pdev);
#endif
#ifdef TDE_TIME_COUNT
TDE_timeval_s g_stTimeStart;
TDE_timeval_s g_stTimeEnd;
HI_U64 g_u64TimeDiff;
#endif

DECLARE_TASKLET(tde_tasklet, tde_tasklet_func, 0);
#ifdef CONFIG_TDE_TDE_EXPORT_FUNC
STATIC TDE_EXPORT_FUNC_S s_TdeExportFuncs =
{
    .pfnTdeOpen             = TdeOsiOpen,
    .pfnTdeClose            = TdeOsiClose,
    .pfnTdeBeginJob         = TdeOsiBeginJob,
    .pfnTdeEndJob           = TdeOsiEndJob,
    .pfnTdeCancelJob        = TdeOsiCancelJob,
    .pfnTdeWaitForDone      = TdeOsiWaitForDone,
    .pfnTdeWaitAllDone      = TdeOsiWaitAllDone,
    .pfnTdeQuickCopy        = TdeOsiQuickCopy,
    .pfnTdeQuickFill        = TdeOsiQuickFill,
    .pfnTdeQuickResize      = TdeOsiQuickResize,
    .pfnTdeQuickFlicker     = TdeOsiQuickFlicker,
    .pfnTdeBlit             = TdeOsiBlit,
    .pfnTdeMbBlit           = TdeOsiMbBlit,
    .pfnTdeSolidDraw        = TdeOsiSolidDraw,
    .pfnTdeSetDeflickerLevel        = TdeOsiSetDeflickerLevel,
    .pfnTdeEnableRegionDeflicker    = TdeOsiEnableRegionDeflicker,
    .pfnTdeSetAlphaThresholdValue   = TdeOsiSetAlphaThresholdValue,
	.pfnTdeSetAlphaThresholdState   = TdeOsiSetAlphaThresholdState,
	.pfnTdeGetAlphaThresholdState   = TdeOsiGetAlphaThresholdState,
    .pfnTdeCalScaleRect     = TdeCalScaleRect,
    #ifdef CONFIG_TDE_PM_ENABLE
    .pfnTdeSuspend			= tde_pm_suspend,
    .pfnTdeResume			= tde_pm_resume,
    #endif
	.pfnTdeQuickCopyEx		= TdeOsiQuickCopyEx,
    #ifdef CONFIG_TDE_BLIT_EX
	.pfnTdeSingleBlitEx		= TdeOsiSingleBlitEx,
    #endif
    .pfnTdeLockWorkingFlag = TdeOsiListLockWorkingFlag,
    .pfnTdeUnlockWorkingFlag = TdeOsiListUnlockWorkingFlag,
    .pfnTdeGetWorkingFlag = TdeOsiListGetWorkingFlag
};
#endif

HI_S32 HI_TDE_MODULE_GetFunction( TDE_EXPORT_FUNC_S** ppFunc)
{
	*ppFunc=&s_TdeExportFuncs;
	return 0;
}


HI_VOID tde_cleanup_module_k(HI_VOID);

HI_S32 tde_init_module_k(HI_VOID)
{
    int ret;

    if (TdeHalInit(TDE_REG_BASEADDR) < 0)
    {
        TDE_TRACE(TDE_KERN_ERR, "TdeHalInit failure!\n");
        return -1;
    }
#ifndef TDE_HWC_COOPERATE
    if (0 != request_irq(TDE_INTNUM, (irq_handler_t)tde_osr_isr,
                         IRQF_PROBE_SHARED, "hi_tde_irq", NULL))
    {
        TDE_TRACE(TDE_KERN_ERR, "request_irq for TDE failure!\n");
        TdeHalRelease();
        return -1;
    }
#else
    if (0 != request_irq(TDE_INTNUM, (irq_handler_t)tde_osr_isr,
                         IRQF_SHARED, "hi_tde_irq", &g_TdeIrqHandle))
    {
        TDE_TRACE(TDE_KERN_ERR, "request_irq for TDE failure!\n");
        TdeHalRelease();
        return -1;
    }
#endif
	ret = HI_GFX_SetIrq(HIGFX_TDE_ID,TDE_INTNUM);
    if(HI_SUCCESS != ret){
        tde_cleanup_module_k();
        return ret;
	}

    TdeOsiListInit();

    ret = HI_GFX_MODULE_Register(HIGFX_TDE_ID, TDE_NAME,&s_TdeExportFuncs);
    if (HI_SUCCESS != ret)
    {
        TDE_TRACE(TDE_KERN_ERR, "register module failed!\n");
        tde_cleanup_module_k();
        return ret;
    }

    spin_lock_init(&s_taskletlock);
    spin_lock_init(&s_TdeRefLock);

    return 0;
}

HI_VOID tde_cleanup_module_k(HI_VOID)
{
    HI_GFX_MODULE_UnRegister(HI_ID_TDE);

    TdeOsiListTerm();
#ifndef TDE_HWC_COOPERATE
    free_irq(TDE_INTNUM, NULL);
#else
	free_irq(TDE_INTNUM, &g_TdeIrqHandle);
#endif
    TdeHalRelease();
    return;
}

int tde_open(struct inode *finode, struct file  *ffile)
{
    UNUSED(finode);
    UNUSED(ffile);
    if (1 == atomic_inc_return(&g_TDECount))
    {
        (HI_VOID)TdeHalOpen();
    }

    return 0;
}

HI_VOID ConvertSurface(TDE2_SURFACE_S  *stSrc,TDE2_IOC_SURFACE_S *pstDst)
{
	HI_SIZE_T clutPhyAddr;
    clutPhyAddr = (HI_SIZE_T)pstDst->u32ClutPhyAddr;

    stSrc->bAlphaExt1555 = pstDst->bAlphaExt1555;
    stSrc->bAlphaMax255 = pstDst->bAlphaMax255;
    stSrc->bYCbCrClut= pstDst->bYCbCrClut;
    stSrc->enColorFmt= pstDst->enColorFmt;
    stSrc->pu8ClutPhyAddr = (HI_U8 *)clutPhyAddr;
    stSrc->u32CbCrPhyAddr= pstDst->u32CbCrPhyAddr;
    stSrc->u32CbCrStride= pstDst->u32CbCrStride;
    stSrc->u32Height= pstDst->u32Height;
    stSrc->u32PhyAddr= pstDst->u32PhyAddr;
    stSrc->u32Stride= pstDst->u32Stride;
    stSrc->u32Width= pstDst->u32Width;
    stSrc->u8Alpha0= pstDst->u8Alpha0;
    stSrc->u8Alpha1= pstDst->u8Alpha1;
}

int tde_release(struct inode *finode, struct file  *ffile)
{
    UNUSED(finode);
    UNUSED(ffile);

    if (atomic_dec_and_test(&g_TDECount))
    {
        //todo:
        //tasklet_kill(&tde_tasklet);
    }
    TdeFreePendingJob();
    if ( atomic_read(&g_TDECount) < 0 )
    {
        atomic_set(&g_TDECount, 0);
    }

    return 0;
}

typedef union
{
    TDE_BITBLIT_CMD_S stBlitCmd;
    TDE_SOLIDDRAW_CMD_S stDrawCmd;
    TDE_QUICKDEFLICKER_CMD_S stDeflickerCmd;
    TDE_QUICKCOPY_CMD_S stCopyCmd;
    TDE_QUICKRESIZE_CMD_S stResizeCmd;
    TDE_QUICKFILL_CMD_S stFillCmd;
    TDE_ENDJOB_CMD_S stEndCmd;
    TDE_MBBITBLT_CMD_S stMbBlitCmd;
    TDE_BITMAP_MASKROP_CMD_S stMaskRopCmd;
    TDE_BITMAP_MASKBLEND_CMD_S stMaskBlendCmd;
    TDE_PATTERN_FILL_CMD_S stPatternCmd;
}TDE_IOCTL_CMD_U;

long tde_ioctl(struct file*  ffile, unsigned int  cmd, unsigned long arg)
{
    HI_U8 u8Cmd = 0;
    void __user* argp = (void __user*)arg;

    UNUSED(ffile);

    u8Cmd = _IOC_NR(cmd);//(HI_U8)(cmd & 0xff);

    TDE_CHECK_IOCTL_CMD(u8Cmd);

    if (cmd != g_paTdeCtlFuncDispatchItem[u8Cmd].u32TdeCmd)
    {
        return -EFAULT;
    }

    if (HI_NULL == g_paTdeCtlFuncDispatchItem[u8Cmd].tde_ctl_ptr_func)
    {
        return -EFAULT;
    }

    return g_paTdeCtlFuncDispatchItem[u8Cmd].tde_ctl_ptr_func(argp);
}

STATIC HI_S32 TdeOsrBeginJob(unsigned long *argp)
{
    TDE_HANDLE s32Handle;
    HI_S32 ret;

    TDE_IOCTL_CHECK_PARA(argp);

    if ((ret = TdeOsiBeginJob(&s32Handle)) < 0)
    {
        return ret;
    }

    if (copy_to_user(argp, &s32Handle, sizeof(TDE_HANDLE)))
    {
        return -EFAULT;
    }

    return 0;
}
STATIC HI_S32 TdeOsrBitBlit(unsigned long *argp)
{
    TDE2_SURFACE_S stBackGround = {0};
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_SURFACE_S stDst = {0};
    TDE_IOCTL_CMD_U unCmd;



    TDE2_SURFACE_S* pstBackGround = NULL;
    TDE2_RECT_S* pstBackGroundRect= NULL;
    TDE2_SURFACE_S* pstForeGround = NULL;
    TDE2_RECT_S* pstForeGroundRect= NULL;
    TDE2_OPT_S* pstOpt= NULL;
    HI_GFX_Memset(&unCmd, 0 , sizeof(TDE_IOCTL_CMD_U));

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stBlitCmd, argp, sizeof(TDE_BITBLIT_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stBackGround, &unCmd.stBlitCmd.stBackGround);
    ConvertSurface(&stForeGround, &unCmd.stBlitCmd.stForeGround);
    ConvertSurface(&stDst, &unCmd.stBlitCmd.stDst);
    pstBackGround       = ((unCmd.stBlitCmd.u32NullIndicator >> 1) & 1) ? NULL : &stBackGround;
    pstBackGroundRect   = ((unCmd.stBlitCmd.u32NullIndicator >> 2) & 1) ? NULL : &unCmd.stBlitCmd.stBackGroundRect;
    pstForeGround       = ((unCmd.stBlitCmd.u32NullIndicator >> 3) & 1) ? NULL : &stForeGround;
    pstForeGroundRect   = ((unCmd.stBlitCmd.u32NullIndicator >> 4) & 1) ? NULL : &unCmd.stBlitCmd.stForeGroundRect;
    pstOpt              = ((unCmd.stBlitCmd.u32NullIndicator >> 7) & 1) ? NULL : &unCmd.stBlitCmd.stOpt;

    return TdeOsiBlit(unCmd.stBlitCmd.s32Handle, pstBackGround, pstBackGroundRect,
                      pstForeGround, pstForeGroundRect, &stDst, &unCmd.stBlitCmd.stDstRect,
                      pstOpt);
}
STATIC HI_S32 TdeOsrSolidDraw(unsigned long *argp)
{
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_SURFACE_S stDst = {0};
    TDE_IOCTL_CMD_U unCmd;


    TDE2_SURFACE_S* pstForeGround = NULL;
    TDE2_RECT_S* pstForeGroundRect= NULL;
    TDE2_FILLCOLOR_S* pstFillColor= NULL;
    TDE2_OPT_S* pstOpt= NULL;
    HI_GFX_Memset(&unCmd, 0 , sizeof(TDE_IOCTL_CMD_U));

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stDrawCmd, argp, sizeof(TDE_SOLIDDRAW_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stForeGround, &unCmd.stDrawCmd.stForeGround);
    ConvertSurface(&stDst, &unCmd.stDrawCmd.stDst);
    pstForeGround       = ((unCmd.stDrawCmd.u32NullIndicator >> 1) & 1) ? NULL : &stForeGround;
    pstForeGroundRect   = ((unCmd.stDrawCmd.u32NullIndicator >> 2) & 1) ? NULL : &unCmd.stDrawCmd.stForeGroundRect;
    pstFillColor        = ((unCmd.stDrawCmd.u32NullIndicator >> 5) & 1) ? NULL : &unCmd.stDrawCmd.stFillColor;
    pstOpt              = ((unCmd.stDrawCmd.u32NullIndicator >> 6) & 1) ? NULL : &unCmd.stDrawCmd.stOpt;

    return TdeOsiSolidDraw(unCmd.stDrawCmd.s32Handle, pstForeGround, pstForeGroundRect,
                           &stDst, &unCmd.stDrawCmd.stDstRect, pstFillColor, pstOpt);
}
STATIC HI_S32 TdeOsrQuickCopy(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;
    TDE2_SURFACE_S stDst = {0};
    TDE2_SURFACE_S stSrc = {0};

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stCopyCmd, argp, sizeof(TDE_QUICKCOPY_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stSrc, &unCmd.stCopyCmd.stSrc);
    ConvertSurface(&stDst, &unCmd.stCopyCmd.stDst);
    return TdeOsiQuickCopy(unCmd.stCopyCmd.s32Handle, &stSrc, &unCmd.stCopyCmd.stSrcRect, &stDst,
                           &unCmd.stCopyCmd.stDstRect);
}
STATIC HI_S32 TdeOsrQuickResize(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;
    TDE2_SURFACE_S stSrc = {0};
    TDE2_SURFACE_S stDst = {0};

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stResizeCmd, argp, sizeof(TDE_QUICKRESIZE_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stSrc, &unCmd.stResizeCmd.stSrc);
    ConvertSurface(&stDst, &unCmd.stResizeCmd.stDst);
    return TdeOsiQuickResize(unCmd.stResizeCmd.s32Handle, &stSrc, &unCmd.stResizeCmd.stSrcRect,
                             &stDst, &unCmd.stResizeCmd.stDstRect);
}
STATIC HI_S32 TdeOsrQuickFill(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;
    TDE2_SURFACE_S stDst = {0};

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stFillCmd, argp, sizeof(TDE_QUICKFILL_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stDst, &unCmd.stFillCmd.stDst);
    return TdeOsiQuickFill(unCmd.stFillCmd.s32Handle, &stDst, &unCmd.stFillCmd.stDstRect,
                           unCmd.stFillCmd.u32FillData);

}
STATIC HI_S32 TdeOsrQuickDeflicker(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;
    TDE2_SURFACE_S stDst = {0};
    TDE2_SURFACE_S stSrc = {0};

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stDeflickerCmd, argp, sizeof(TDE_QUICKDEFLICKER_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stSrc, &unCmd.stDeflickerCmd.stSrc);
    ConvertSurface(&stDst, &unCmd.stDeflickerCmd.stDst);
    return TdeOsiQuickFlicker(unCmd.stDeflickerCmd.s32Handle, &stSrc, &unCmd.stDeflickerCmd.stSrcRect, &stDst,
                              &unCmd.stDeflickerCmd.stDstRect);
}
STATIC HI_S32 TdeOsrMbBitBlit(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;
    TDE2_SURFACE_S stDst = {0};

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stMbBlitCmd, argp, sizeof(TDE_MBBITBLT_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stDst, &unCmd.stMbBlitCmd.stDst);
    return TdeOsiMbBlit(unCmd.stMbBlitCmd.s32Handle, &unCmd.stMbBlitCmd.stMB, &unCmd.stMbBlitCmd.stMbRect, &stDst, &unCmd.stMbBlitCmd.stDstRect, &unCmd.stMbBlitCmd.stMbOpt);
}
STATIC HI_S32 TdeOsrEndJob(unsigned long *argp)
{
    TDE_IOCTL_CMD_U unCmd;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stEndCmd, argp, sizeof(TDE_ENDJOB_CMD_S)))
    {
        return -EFAULT;
    }

    return TdeOsiEndJob(unCmd.stEndCmd.s32Handle, unCmd.stEndCmd.bBlock, unCmd.stEndCmd.u32TimeOut, unCmd.stEndCmd.bSync, NULL, NULL);


}
STATIC HI_S32 TdeOsrWaitForDone(unsigned long *argp)
{
    TDE_HANDLE s32Handle;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&s32Handle, argp, sizeof(TDE_HANDLE)))
    {
        return -EFAULT;
    }

    return TdeOsiWaitForDone(s32Handle, TDE_MAX_WAIT_TIMEOUT);
}
STATIC HI_S32 TdeOsrCancelJob(unsigned long *argp)
{
    TDE_HANDLE s32Handle;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&s32Handle, argp, sizeof(TDE_HANDLE)))
    {
        return -EFAULT;
    }

    return TdeOsiCancelJob(s32Handle);
}
STATIC HI_S32 TdeOsrBitMapMaskRop(unsigned long *argp)
{
    TDE2_SURFACE_S stBackGround = {0};
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_SURFACE_S stDst = {0};
    TDE2_SURFACE_S stMask = {0};
    TDE_IOCTL_CMD_U unCmd;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stMaskRopCmd, argp, sizeof(TDE_BITMAP_MASKROP_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stBackGround, &unCmd.stMaskRopCmd.stBackGround);
    ConvertSurface(&stForeGround, &unCmd.stMaskRopCmd.stForeGround);
    ConvertSurface(&stMask, &unCmd.stMaskRopCmd.stMask);
    ConvertSurface(&stDst, &unCmd.stMaskRopCmd.stDst);
    return TdeOsiBitmapMaskRop(unCmd.stMaskRopCmd.s32Handle,
                               &stBackGround, &unCmd.stMaskRopCmd.stBackGroundRect,
                               &stForeGround, &unCmd.stMaskRopCmd.stForeGroundRect,
                               &stMask, &unCmd.stMaskRopCmd.stMaskRect,
                               &stDst, &unCmd.stMaskRopCmd.stDstRect,
                               unCmd.stMaskRopCmd.enRopCode_Color, unCmd.stMaskRopCmd.enRopCode_Alpha);
}
STATIC HI_S32 TdeOsrBitMapMaskBlend(unsigned long *argp)
{
    TDE2_SURFACE_S stBackGround = {0};
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_SURFACE_S stDst = {0};
    TDE2_SURFACE_S stMask = {0};
    TDE_IOCTL_CMD_U unCmd;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stMaskBlendCmd, argp, sizeof(TDE_BITMAP_MASKBLEND_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stBackGround, &unCmd.stMaskBlendCmd.stBackGround);
    ConvertSurface(&stForeGround, &unCmd.stMaskBlendCmd.stForeGround);
    ConvertSurface(&stMask, &unCmd.stMaskBlendCmd.stMask);
    ConvertSurface(&stDst, &unCmd.stMaskBlendCmd.stDst);
    return TdeOsiBitmapMaskBlend(unCmd.stMaskBlendCmd.s32Handle, &stBackGround, &unCmd.stMaskBlendCmd.stBackGroundRect,
                                 &stForeGround, &unCmd.stMaskBlendCmd.stForeGroundRect, &stMask, &unCmd.stMaskBlendCmd.stMaskRect,
                                 &stDst, &unCmd.stMaskBlendCmd.stDstRect, unCmd.stMaskBlendCmd.u8Alpha, unCmd.stMaskBlendCmd.enBlendMode);
}
STATIC HI_S32 TdeOsrWaitAllDone(unsigned long *argp)
{
    return TdeOsiWaitAllDone(HI_FALSE);
}
STATIC HI_S32 TdeOsrReset(unsigned long *argp)
{
    TdeOsiReset();
    return HI_SUCCESS;
}

STATIC HI_S32 TdeOsrSetDeflickerLevel(unsigned long *argp)
{
    TDE_DEFLICKER_LEVEL_E eDeflickerLevel;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&eDeflickerLevel, argp, sizeof(TDE_DEFLICKER_LEVEL_E)))
    {
        return -EFAULT;
    }

    return TdeOsiSetDeflickerLevel(eDeflickerLevel);

}
STATIC HI_S32 TdeOsrGetDeflickerLevel(unsigned long *argp)
{
    TDE_DEFLICKER_LEVEL_E eDeflickerLevel;

    TDE_IOCTL_CHECK_PARA(argp);

    if (TdeOsiGetDeflickerLevel(&eDeflickerLevel) != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    if (copy_to_user(argp, &eDeflickerLevel, sizeof(TDE_DEFLICKER_LEVEL_E)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;

}
STATIC HI_S32 TdeOsrSetAlphaThreshholdValue(unsigned long *argp)
{
    HI_U8 u8ThresholdValue;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&u8ThresholdValue, argp, sizeof(HI_U8)))
    {
        return -EFAULT;
    }

    return TdeOsiSetAlphaThresholdValue(u8ThresholdValue);

}
STATIC HI_S32 TdeOsrGetAlphaThreshholdValue(unsigned long *argp)
{
    HI_U8 u8ThresholdValue;

    TDE_IOCTL_CHECK_PARA(argp);

    if (TdeOsiGetAlphaThresholdValue(&u8ThresholdValue))
    {
        return HI_FAILURE;
    }

    if (copy_to_user(argp, &u8ThresholdValue, sizeof(HI_U8)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;

}
STATIC HI_S32 TdeOsrSetAlphaThreshholdState(unsigned long *argp)
{
    HI_BOOL bEnAlphaThreshold;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&bEnAlphaThreshold, argp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }

    return TdeOsiSetAlphaThresholdState(bEnAlphaThreshold);

}
STATIC HI_S32 TdeOsrGetAlphaThreshholdState(unsigned long *argp)
{
    HI_BOOL bEnAlphaThreshold;

    TDE_IOCTL_CHECK_PARA(argp);

    TdeOsiGetAlphaThresholdState(&bEnAlphaThreshold);

    if (copy_to_user(argp, &bEnAlphaThreshold, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;

}
STATIC HI_S32 TdeOsrPatternFill(unsigned long *argp)
{
    TDE2_SURFACE_S stBackGround = {0};
    TDE2_SURFACE_S stForeGround = {0};
    TDE2_SURFACE_S stDst = {0};
    TDE_IOCTL_CMD_U unCmd;

    TDE2_RECT_S* pstBackGroundRect;
    TDE2_RECT_S* pstForeGroundRect;
    TDE2_RECT_S* pstDstRect;
    TDE2_PATTERN_FILL_OPT_S* pstOpt;
    TDE2_SURFACE_S* pstBackGround = NULL;
    TDE2_SURFACE_S* pstForeGround = NULL;
    TDE2_SURFACE_S* pstDst = NULL;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&unCmd.stPatternCmd, argp, sizeof(TDE_PATTERN_FILL_CMD_S)))
    {
        return -EFAULT;
    }

    ConvertSurface(&stBackGround, &unCmd.stPatternCmd.stBackGround);
    ConvertSurface(&stForeGround, &unCmd.stPatternCmd.stForeGround);
    ConvertSurface(&stDst, &unCmd.stPatternCmd.stDst);
    pstBackGround       = ((unCmd.stPatternCmd.u32NullIndicator >> 1) & 1) ? NULL : &stBackGround;
    pstBackGroundRect   = ((unCmd.stPatternCmd.u32NullIndicator >> 2) & 1) ? NULL : &unCmd.stPatternCmd.stBackGroundRect;
    pstForeGround       = ((unCmd.stPatternCmd.u32NullIndicator >> 3) & 1) ? NULL : &stForeGround;
    pstForeGroundRect   = ((unCmd.stPatternCmd.u32NullIndicator >> 4) & 1) ? NULL : &unCmd.stPatternCmd.stForeGroundRect;
    pstDst              = ((unCmd.stPatternCmd.u32NullIndicator >> 5) & 1) ? NULL : &stDst;
    pstDstRect          = ((unCmd.stPatternCmd.u32NullIndicator >> 6) & 1) ? NULL : &unCmd.stPatternCmd.stDstRect;
    pstOpt              = ((unCmd.stPatternCmd.u32NullIndicator >> 7) & 1) ? NULL : &unCmd.stPatternCmd.stOpt;
    return TdeOsiPatternFill(unCmd.stPatternCmd.s32Handle, pstBackGround,
                             pstBackGroundRect, pstForeGround, pstForeGroundRect,
                             pstDst, pstDstRect, pstOpt);

}
STATIC HI_S32 TdeOsrEnableRegionDeflicker(unsigned long *argp)
{
    HI_BOOL bRegionDeflicker;

    TDE_IOCTL_CHECK_PARA(argp);

    if (copy_from_user(&bRegionDeflicker, argp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }

    return TdeOsiEnableRegionDeflicker(bRegionDeflicker);

}


#ifdef TDE_COREDUMP_DEBUG
 extern volatile HI_U32* s_pu32BaseVirAddr;

#define TDE_READ_REG(base, offset) \
    (*(volatile unsigned int   *)((unsigned int)(base) + (offset)))
#endif

STATIC int tde_osr_isr(int irq, void *dev_id)
{
    HI_U32 int_status;
 #ifdef TDE_TIME_COUNT
    (HI_VOID)TDE_gettimeofday(&g_stTimeStart);
#endif
    int_status = TdeHalCtlIntStats();

    /* AI7D02547 Interrupt handling while suspend to die */
    if(int_status & TDE_DRV_INT_ERROR)
    {
        #ifdef TDE_COREDUMP_DEBUG
        HI_U32 u32ReadStats = 0;
        HI_U32 i;
        for(i=0;i<74;i++)
        {
            u32ReadStats = TDE_READ_REG(s_pu32BaseVirAddr, (0x800 + i*4));
            TDE_TRACE(TDE_KERN_INFO,"\n--------- ADDR:0x%x Value:0x%x---------\n",(0x800 + i*4),u32ReadStats);
        }
        #endif

        #ifdef CONFIG_TDE_USE_SDK_CRG_ENABLE
          U_PERI_CRG37 unTempValue;
          HI_U32 i;
          unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;
          unTempValue.bits.tde_srst_req = 0x1;
          g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
          for(i=0;i<100;i++)
          {
              ;
          }
          unTempValue.bits.tde_srst_req = 0x0;
          g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;
        #endif
        TDE_TRACE(TDE_KERN_ERR, "tde interrupts coredump!\n");
        TdeHalResumeInit();

        return IRQ_HANDLED;
    }
 #ifdef TDE_HWC_COOPERATE
    if(int_status & TDE_DRV_INT_TIMEOUT)
    {
        TDE_TRACE(TDE_KERN_ERR, "tde hardware timeout!\n");
        TdeHalResumeInit();
        return IRQ_HANDLED;
    }
#endif
    TDE_TRACE(TDE_KERN_DEBUG, "tde register int status: 0x%x!\n", (HI_U32)int_status);

    tde_tasklet.data = tde_tasklet.data |((HI_UL)int_status);

    tasklet_schedule(&tde_tasklet);

    return IRQ_HANDLED;
}

STATIC void tde_tasklet_func(unsigned long int_status)
{
    HI_SIZE_T lockflags;
    TDE_LOCK(&s_taskletlock,lockflags);
    tde_tasklet.data &= (~int_status);
    TDE_UNLOCK(&s_taskletlock,lockflags);

#ifdef TDE_TIME_COUNT
    (HI_VOID)TDE_gettimeofday(&g_stTimeEnd);

    g_u64TimeDiff = (g_stTimeEnd.tv_sec - g_stTimeStart.tv_sec)*1000000
         + (g_stTimeEnd.tv_usec - g_stTimeStart.tv_usec);
    TDE_TRACE(TDE_KERN_DEBUG, "tde int status: 0x%x, g_u64TimeDiff:%d!\n", (HI_U32)int_status, (HI_U32)g_u64TimeDiff);
#endif

#ifdef TDE_HWC_COOPERATE
    if(int_status & TDE_DRV_INT_NODE)
    {
        TdeOsiListNodeComp();
    }
    TdeOsiListComp();
#else
    if(int_status&TDE_DRV_INT_NODE)
    {
        TdeOsiListNodeComp();
    }
#endif

}

#ifdef CONFIG_TDE_PM_ENABLE
/* tde wait for start  */
int tde_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    UNUSED(pdev);
    UNUSED(state);

    TdeOsiWaitAllDone(HI_FALSE);

	TdeHalSuspend();

    HI_PRINT("TDE suspend OK\n");

    return 0;
}

/* wait for resume */
int tde_pm_resume(PM_BASEDEV_S *pdev)
{
    UNUSED(pdev);

    if ( atomic_read(&g_TDECount) > 0 )
    {
        TdeHalResumeInit();
    }
    HI_PRINT("TDE resume OK\n");

    return 0;
}
#endif

/*****************************************************************************
 Prototype       : TdeOsiOpen
 Description     : open TDE equipment
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/26
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 TdeOsiOpen(HI_VOID)
{
    return tde_open(NULL, NULL);
}

/*****************************************************************************
 Prototype       : TdeOsiClose
 Description     : close TDE equipment
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/26
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 TdeOsiClose(HI_VOID)
{
    return tde_release(NULL, NULL);
}




#ifndef MODULE
EXPORT_SYMBOL(tde_pm_suspend);
EXPORT_SYMBOL(tde_pm_resume);
#endif
EXPORT_SYMBOL(TdeOsiOpen);
EXPORT_SYMBOL(TdeOsiClose);
EXPORT_SYMBOL(tde_ioctl);
EXPORT_SYMBOL(tde_open);
EXPORT_SYMBOL(tde_release);
EXPORT_SYMBOL(tde_init_module_k);
EXPORT_SYMBOL(tde_cleanup_module_k);


#endif
