#ifdef GPU_PROC_SUPPORT
/*#include <linux/clk.h>*/
/*#include <mach/platform.h>*/
#include <linux/clkdev.h>
#include <asm/clkdev.h>
/*#include <mach/clock.h>*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#ifdef CONFIG_PROC_FS
 #include <linux/proc_fs.h>
#endif
#include <linux/seq_file.h>

#include "mali_kernel_common.h"

#include "hi_drv_proc.h"
#include "hi_osal.h"
#include "mali4xx_pmu.h"
#include "mali4xx_core_scaling.h"
#include "mali4xx_cfg.h"
#include "mali4xx_clk.h"

#define GPU_CMD_MAX_NUM (2)
#define GPU_CMD_MAX_CMD_LEN (32)
#define GPU_CMD_MAX_VLAUE_LEN (32)
#define GPU_CMD_AVS "avs"
#define GPU_CMD_ON "on"
#define GPU_CMD_OFF "off"
#define GPU_CMD_VOLT "volt"
#define GPU_CMD_FREQ "freq"
#define GPU_CMD_HELP "help"
#define GPU_CMD_WAKEUPRESET "reset"
#define GPU_CMD_DEBUG "debug"
#define GPU_CMD_ENTERSMARTSTANDBY "entersmart"
#define GPU_CMD_QUITSMARTSTANDBY "quitsmart"

typedef struct tagGPU_CMD_DATA_S
{
    HI_U8 aszCmd[GPU_CMD_MAX_CMD_LEN];
    HI_U8 aszValue[GPU_CMD_MAX_VLAUE_LEN];
} GPU_CMD_DATA_S;

static GPU_CMD_DATA_S g_astGPUProcCmd[GPU_CMD_MAX_NUM];

extern int mali_dvfs_enable;
extern int mali_avs_enable;

HI_U32 GPUParseValue(HI_U8* pu8Value)
{
    if (strlen(pu8Value) < 2)
    {
        return (HI_U32)simple_strtoul(pu8Value, HI_NULL, 10);
    }
    else
    {
        if ((pu8Value[0] == '0') && ((pu8Value[1] == 'x') || (pu8Value[1] == 'X')))
        {
            return (HI_U32)simple_strtoul(pu8Value, HI_NULL, 16);
        }
        else
        {
            return (HI_U32)simple_strtoul(pu8Value, HI_NULL, 10);
        }
    }
}

HI_S32 GPUParseCmd(HI_U8* pu8Cmd, HI_U32 u32Len, GPU_CMD_DATA_S** pstCmd, HI_U32* pu32Num)
{
    HI_U32 u32Pos = 0;
    HI_U32 u32SavePos = 0;
    HI_U32 u32CmdNum = 0;
    HI_BOOL bIsCmd   = HI_TRUE;
    HI_U8 u8LastChar = ' ';

    if ((HI_NULL == pu8Cmd) || (0 == u32Len) || (HI_NULL == pstCmd) || (HI_NULL == pu32Num))
    {
        MALI_PRINT_ERROR(("Invalid.\n"));
        return HI_FAILURE;
    }

    u32CmdNum = 0;
    memset(g_astGPUProcCmd, 0, sizeof(g_astGPUProcCmd));

    while ((u32Pos < u32Len) && (pu8Cmd[u32Pos]))
    {
        switch (pu8Cmd[u32Pos])
        {
        case '\0':
        case '\n':
        case '\t':
            break;

        case '=':
            if (bIsCmd)
            {
                bIsCmd = HI_FALSE;
                u32SavePos = 0;
            }

            break;
        case ' ':
            if (!((' ' == u8LastChar) || ('=' == u8LastChar)))
            {
                bIsCmd = !bIsCmd;
                u32SavePos = 0;
                if (bIsCmd)
                {
                    if (u32CmdNum < GPU_CMD_MAX_NUM - 1)
                    {
                        u32CmdNum++;
                    }
                    else
                    {
                        goto out;
                    }
                }
            }

            break;
        default:
            if (bIsCmd)
            {
                if (u32SavePos < GPU_CMD_MAX_CMD_LEN)
                {
                    g_astGPUProcCmd[u32CmdNum].aszCmd[u32SavePos++] = pu8Cmd[u32Pos];
                }
            }
            else
            {
                if (u32SavePos < GPU_CMD_MAX_VLAUE_LEN)
                {
                    g_astGPUProcCmd[u32CmdNum].aszValue[u32SavePos++] = pu8Cmd[u32Pos];
                }
            }

            break;
        }

        u8LastChar = pu8Cmd[u32Pos];
        u32Pos++;
    }

out:
    if (strlen(g_astGPUProcCmd[u32CmdNum].aszCmd) > 0)
    {
        u32CmdNum += 1;
    }

    *pstCmd  = g_astGPUProcCmd;
    *pu32Num = u32CmdNum;
    return HI_SUCCESS;
}

HI_S32 GPUDebugCtrl(HI_U32 u32Para1, HI_U32 u32Para2)
{
    HI_U32 u32CpuCurFreq = 0;

    MALI_DEBUG_PRINT(2, ("\n gpu freq = 0x%x, voltage = 0x%x \n", u32Para1, u32Para2));

    if ((0 == u32Para1) && (0 == u32Para2))
    {
        MALI_PRINT_ERROR(("plese set the valid value \n"));
        return HI_SUCCESS;
    }

    if (0 == u32Para2)
    {
        gpu_dvfs_set_freq(u32Para1);

        return HI_SUCCESS;
    }

    if (0 == u32Para1)
    {
        gpu_dvfs_set_voltage(u32Para2);

        return HI_SUCCESS;
    }

    u32CpuCurFreq = gpu_dvfs_get_freq();
    if (u32Para1 > u32CpuCurFreq)
    {
        gpu_dvfs_set_voltage(u32Para2);
        gpu_dvfs_set_freq(u32Para1);
    }
    else
    {
        gpu_dvfs_set_freq(u32Para1);
        gpu_dvfs_set_voltage(u32Para2);
    }

    return HI_SUCCESS;
}

static HI_S32 GPUProcRead(struct seq_file *p, HI_VOID *v)
{
    PROC_PRINT(p, "---------Hisilicon GPU Info---------\n");
    PROC_PRINT(p, "Frequency			:%d(kHz)\n",gpu_dvfs_get_freq());
    PROC_PRINT(p, "Voltage				:%d(mv)\n",gpu_dvfs_get_voltage());
    PROC_PRINT(p, "GP_utilization			:%d(percent)\n",gpu_get_gp_utilization()*100/255);
    PROC_PRINT(p, "PP_utilization			:%d(percent)\n",gpu_get_pp_utilization()*100/255);

    if (1 == gpu_is_powerdown())
    {
        PROC_PRINT(p, "Power_status			:power down\n");
    }
    else
    {
        PROC_PRINT(p, "Power_status			:power up\n");
    }

    PROC_PRINT(p, "Shared_mem_size			:%d(MB)\n",CONFIG_GPU_MAX_SHARE_MEM_SIZE/1024/1024);
    PROC_PRINT(p, "Dedicated_mem_start		:0x%x \n",GPU_DEDICATE_MEM_START);
    PROC_PRINT(p, "Dedicated_mem_size		:%d(MB)\n",GPU_DEDICATE_MEM_SIZE/1024/1024 );

    if (1 == mali_dvfs_enable)
    {
        PROC_PRINT(p, "DVFS_enable			:enable\n");
    }
    else
    {
        PROC_PRINT(p, "DVFS_enable			:disable\n");
    }

#if defined (GPU_AVS_SUPPORT)
    if (1 == mali_avs_enable)
    {
        PROC_PRINT(p, "AVS_enable			:enable\n");
    }
    else
    {
        PROC_PRINT(p, "AVS_enable			:disable\n");
    }

#else
    PROC_PRINT(p, "AVS_enable			:disable\n");
#endif

    return 0;
}

static HI_VOID GPUProcHelper(HI_VOID)
{
    HI_DRV_PROC_EchoHelper(
        "echo volt=1080 > /proc/hisi/msp/pm_gpu, set gpu volt in mv.\n"
        "echo freq=432000 > /proc/hisi/msp/pm_gpu, set gpu freq in kHz.\n"
        "echo avs on/off > /proc/hisi/msp/pm_gpu, open/close gpu avs.\n"
        "echo freq=432000 volt=1080 > /proc/hisi/msp/pm_gpu, set gpu volt and freq simultaneity.\n"
    );

    return;
}

static HI_S32 GPUProcWrite(struct file * file,
                           const char __user * buf, size_t count, loff_t *ppos)
{
    HI_CHAR ProcPara[64] = {0};
    HI_S32 s32Ret;
    HI_U32 u32CmdNum = 0;
    GPU_CMD_DATA_S* pstCmd = HI_NULL;

    if(count > sizeof(ProcPara))
    {
        return -EFAULT;
    }

    if (copy_from_user(ProcPara, buf, count))
    {
        return -EFAULT;
    }

    s32Ret = GPUParseCmd(ProcPara, count, &pstCmd, &u32CmdNum);
    if (HI_SUCCESS != s32Ret)
    {
        goto err;
    }

    if (1 == u32CmdNum)
    {
        /* Only set GPU volt */
        if (0 == HI_OSAL_Strncasecmp(GPU_CMD_VOLT, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
        {
            GPUDebugCtrl(0, GPUParseValue(pstCmd[0].aszValue));
        }
        /* Only set GPU freq */
        else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
        {
            GPUDebugCtrl(GPUParseValue(pstCmd[0].aszValue), 0);
        }
        else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_AVS, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
        {
            /* GPU AVS ON */
            if (0 == HI_OSAL_Strncasecmp(GPU_CMD_ON, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
            {
                gpu_avs_enable(HI_TRUE);
            }
            /* GPU AVS OFF */
            else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_OFF, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
            {
                gpu_avs_enable(HI_FALSE);
            }
        }
        /* Help */
        else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_HELP, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
        {
            GPUProcHelper();
        }
        /* Support 0xXXX 0xXXX command */
        else /*if (('0' == pstCmd[0].aszCmd[0]) && ('0' == pstCmd[0].aszValue[0]))*/
        {
            GPUDebugCtrl(GPUParseValue(pstCmd[0].aszCmd), GPUParseValue(pstCmd[0].aszValue));
        }
    }
    else if (2 == u32CmdNum)
    {
        if ((0 == HI_OSAL_Strncasecmp(GPU_CMD_VOLT, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
            && (0 == HI_OSAL_Strncasecmp(GPU_CMD_FREQ, pstCmd[1].aszCmd, strlen(pstCmd[1].aszCmd))))
        {
            GPUDebugCtrl(GPUParseValue(pstCmd[1].aszValue), GPUParseValue(pstCmd[0].aszValue));
        }
        else if ((0 == HI_OSAL_Strncasecmp(GPU_CMD_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
                 && (0 == HI_OSAL_Strncasecmp(GPU_CMD_VOLT, pstCmd[1].aszCmd, strlen(pstCmd[1].aszCmd))))
        {
            GPUDebugCtrl(GPUParseValue(pstCmd[0].aszValue), GPUParseValue(pstCmd[1].aszValue));
        }
        else
        {
            goto err;
        }
    }
    else
    {
        goto err;
    }

    return count;

err:
    HI_DRV_PROC_EchoHelper("Invaid parameter.\n");
    GPUProcHelper();
    return count;
}

int gpu_proc_create(void)
{
    DRV_PROC_ITEM_S *item;

    item = HI_DRV_PROC_AddModule("pm_gpu", NULL, NULL);
    if (!item)
    {
        MALI_PRINT_ERROR(("add GPU proc module failed\n"));
        return -1;
    }

    item->read  = GPUProcRead;
    item->write = GPUProcWrite;
    return 0;
}

int gpu_proc_destroy(void)
{
    HI_DRV_PROC_RemoveModule("pm_gpu");
    return 0;
}

#endif
