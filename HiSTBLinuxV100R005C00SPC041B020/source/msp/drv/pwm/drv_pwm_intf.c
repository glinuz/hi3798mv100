#include <linux/kernel.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_kernel_adapt.h"
#include "hi_osal.h"
#include "hi_drv_proc.h"

#include "drv_pwm_ioctl.h"
#include "drv_pwm_ext.h"

#define PWM_CLK_FREQ        (54 * 1000 * 1000)      /*pwm work clock freq*/

/********************* pwm register start **************************/
#define PWM_REG_BASE        (0xF8B01000)
#define PWM0_REG_CFG        (PWM_REG_BASE + 0x0)
#define PWM0_REG_CTRL       (PWM_REG_BASE + 0x4)
#define PWM0_REG_STATE      (PWM_REG_BASE + 0x8)
#define PWM1_REG_CFG        (PWM_REG_BASE + 0x10)
#define PWM1_REG_CTRL       (PWM_REG_BASE + 0x14)
#define PWM1_REG_STATE      (PWM_REG_BASE + 0x18)
#define PWM2_REG_CFG        (PWM_REG_BASE + 0x20)
#define PWM2_REG_CTRL       (PWM_REG_BASE + 0x24)
#define PWM2_REG_STATE      (PWM_REG_BASE + 0x28)
/********************** pwm register end ***************************/

#define PWM_NAME            "HI_PWM"

#define DRV_PWM_LOCK(pMutex)    \
    do{ \
        if(down_interruptible(pMutex)) \
        {       \
            HI_ERR_PWM("ERR: pwm lock error!\n");    \
        }   \
    }while(0)

#define DRV_PWM_UNLOCK(pMutex)    \
    do{ \
        up(pMutex); \
    }while(0)


typedef struct tagPWM_REGISTER_PARAM_S
{
    DRV_PROC_READ_FN        rdproc;
    DRV_PROC_WRITE_FN       wtproc;
}PWM_REGISTER_PARAM_S;

/*
typedef struct tagPWM_GLOBAL_S
{
    struct semaphore        PwmMutex[HI_UNF_PWM_BUTT];
}PWM_GLOBAL_S;

static PWM_GLOBAL_S     g_PwmGlobal;
*/

static HI_S32 PWM_GetRegs(HI_UNF_PWM_E enPWM, PWM_REG_S *pstRegs)
{
    HI_S32  Ret = HI_SUCCESS;

    switch (enPWM)
    {
        case HI_UNF_PWM_0:
        {
            pstRegs->u32Cfg   = IO_ADDRESS(PWM0_REG_CFG);
            pstRegs->u32Ctrl  = IO_ADDRESS(PWM0_REG_CTRL);
            pstRegs->u32State = IO_ADDRESS(PWM0_REG_STATE);

            break;
        }
        case HI_UNF_PWM_1:
        {
            pstRegs->u32Cfg   = IO_ADDRESS(PWM1_REG_CFG);
            pstRegs->u32Ctrl  = IO_ADDRESS(PWM1_REG_CTRL);
            pstRegs->u32State = IO_ADDRESS(PWM1_REG_STATE);

            break;
        }
        case HI_UNF_PWM_2:
        {
            pstRegs->u32Cfg   = IO_ADDRESS(PWM2_REG_CFG);
            pstRegs->u32Ctrl  = IO_ADDRESS(PWM2_REG_CTRL);
            pstRegs->u32State = IO_ADDRESS(PWM2_REG_STATE);

            break;
        }
        default:
            Ret = HI_FAILURE;
            break;
    }

    return Ret;
}

HI_S32 PWM_GetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr)
{
    HI_S32                      Ret;
    PWM_REG_S                   stRegs = {0};
    volatile HI_U32             u32PwmCfg;

    Ret = PWM_GetRegs(enPWM, &stRegs);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    u32PwmCfg = readl((HI_VOID *)stRegs.u32Cfg);

    pstAttr->u32Freq = PWM_CLK_FREQ / ((u32PwmCfg & 0xffff) + 1);

    pstAttr->u32DutyRatio = ((u32PwmCfg & 0xffff) + 1) * 1000 / (u32PwmCfg >> 16);

    return HI_SUCCESS;
}

HI_S32 PWM_SetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr)
{
    HI_S32                      Ret;
    PWM_REG_S                   stRegs = {0};
    HI_U32                      u32PwmCfg;

    Ret = PWM_GetRegs(enPWM, &stRegs);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    u32PwmCfg = (PWM_CLK_FREQ / pstAttr->u32Freq - 1) +
                ((PWM_CLK_FREQ / pstAttr->u32Freq * pstAttr->u32DutyRatio / 1000) << 16);

    writel(u32PwmCfg,(HI_VOID *)stRegs.u32Cfg);

    return HI_SUCCESS;
}

HI_S32 PWM_SendSignal(HI_UNF_PWM_E enPWM, HI_U32 u32CarrierSigDurationUs, HI_U32 u32LowLevelSigDurationUs)
{
    HI_S32                      Ret;
    PWM_REG_S                   stRegs = {0};
    volatile HI_U32             u32PwmState;
    volatile HI_U32             u32PwmCfg;
    ktime_t                     WaitTime;

    Ret = PWM_GetRegs(enPWM, &stRegs);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    /* enable pwm */
    writel(0x5, (HI_VOID *)stRegs.u32Ctrl);

    /* confirm state is wright */
    u32PwmState = readl((HI_VOID *)stRegs.u32State);
    u32PwmCfg = readl((HI_VOID *)stRegs.u32Cfg);
    if (u32PwmState != u32PwmCfg)
    {
        return HI_FAILURE;
    }

    /* output carrier signal for needed times */
    WaitTime = ns_to_ktime((HI_U64)u32CarrierSigDurationUs * 1000);
    set_current_state(TASK_UNINTERRUPTIBLE);
    schedule_hrtimeout(&WaitTime, HRTIMER_MODE_REL);

    /* disable pwm */
    writel(0x0,(HI_VOID *)stRegs.u32Ctrl);

    /* output low-level signal for needed times */
    WaitTime = ns_to_ktime((HI_U64)u32LowLevelSigDurationUs * 1000);
    set_current_state(TASK_UNINTERRUPTIBLE);
    schedule_hrtimeout(&WaitTime, HRTIMER_MODE_REL);

    return HI_SUCCESS;
}

static HI_S32 PWM_ProcRead(struct seq_file *p, HI_VOID *v)
{
    return HI_SUCCESS;
}

static HI_S32 PWM_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    return HI_SUCCESS;
}

HI_S32 PWM_DRV_Open(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 PWM_DRV_Close(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 PWM_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32                  Ret = HI_SUCCESS;
    PWM_ATTR_CMD_PARA_S     stAttrCmd;
    PWM_SIGNAL_CMD_PARA_S   stSigCmd;

    switch (cmd)
    {
        case CMD_PWM_GETATTR:
        {
            memcpy(&stAttrCmd, (PWM_ATTR_CMD_PARA_S *)arg, sizeof(PWM_ATTR_CMD_PARA_S));

            Ret = PWM_GetAttr(stAttrCmd.enPwm, &stAttrCmd.stPwmAttr);

            break;
        }
        case CMD_PWM_SETATTR:
        {
            memcpy(&stAttrCmd, (PWM_ATTR_CMD_PARA_S *)arg, sizeof(PWM_ATTR_CMD_PARA_S));

            Ret = PWM_SetAttr(stAttrCmd.enPwm, &stAttrCmd.stPwmAttr);

            break;
        }
        case CMD_PWM_SENDSIGNAL:
        {
            memcpy(&stSigCmd, (PWM_SIGNAL_CMD_PARA_S *)arg, sizeof(PWM_SIGNAL_CMD_PARA_S));

            Ret = PWM_SendSignal(stSigCmd.enPwm, stSigCmd.u32CarrierSigDurationUs, stSigCmd.u32LowLevelSigDurationUs);

            break;
        }

        default:
            return -ENOIOCTLCMD;
    }

    return Ret;
}

static long PWM_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    HI_S32 Ret;

    Ret = HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, PWM_Ioctl);

    return Ret;
}

HI_S32 PWM_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return HI_SUCCESS;
}

HI_S32 PWM_Resume(PM_BASEDEV_S *pdev)
{
    return HI_SUCCESS;
}

static PWM_REGISTER_PARAM_S g_PwmProcPara = {
    .rdproc = PWM_ProcRead,
    .wtproc = PWM_ProcWrite,
};

static UMAP_DEVICE_S g_PwmRegisterData;


static struct file_operations g_PwmFops =
{
    .owner          =    THIS_MODULE,
    .open           =    PWM_DRV_Open,
    .unlocked_ioctl =    PWM_DRV_Ioctl,
    .release        =    PWM_DRV_Close,
};

static PM_BASEOPS_S g_PwmDrvOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = PWM_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = PWM_Resume,
};

HI_S32 PWM_DRV_ModInit(HI_VOID)
{
    HI_S32              Ret;
    HI_CHAR             ProcName[16];
    DRV_PROC_ITEM_S     *pProcItem = HI_NULL;

    Ret = HI_DRV_MODULE_Register(HI_ID_PWM, PWM_NAME, HI_NULL);

    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_PWM);

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if(HI_NULL != pProcItem)
    {
        pProcItem->read = g_PwmProcPara.rdproc;
        pProcItem->write = g_PwmProcPara.wtproc;
    }

    HI_OSAL_Snprintf(g_PwmRegisterData.devfs_name, sizeof(g_PwmRegisterData.devfs_name), UMAP_DEVNAME_PWM);
    g_PwmRegisterData.fops = &g_PwmFops;
    g_PwmRegisterData.minor = UMAP_MIN_MINOR_PWM;
    g_PwmRegisterData.owner  = THIS_MODULE;
    g_PwmRegisterData.drvops = &g_PwmDrvOps;
    if (HI_DRV_DEV_Register(&g_PwmRegisterData) < 0)
    {
        HI_FATAL_PWM("register PWM failed.\n");
        return HI_FAILURE;
    }

    return  HI_SUCCESS;
}

HI_VOID PWM_DRV_ModExit(HI_VOID)
{
    HI_CHAR             ProcName[16];

    HI_DRV_DEV_UnRegister(&g_PwmRegisterData);

    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_PWM);
    HI_DRV_PROC_RemoveModule(ProcName);

    HI_DRV_MODULE_UnRegister(HI_ID_PWM);
}

#ifdef MODULE
module_init(PWM_DRV_ModInit);
module_exit(PWM_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

