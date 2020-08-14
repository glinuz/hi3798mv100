#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/kernel.h>
/*#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif*/
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/clkdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>

#include "hi_drv_proc.h"
#include "hi_osal.h"
#include "mali_gpu_proc.h"
#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include <mali_kbase_pm.h>
#include <mali_kbase_config_platform.h>

#define GPU_CMD_MAX_NUM (2)
#define GPU_CMD_MAX_CMD_LEN (32)
#define GPU_CMD_MAX_VLAUE_LEN (32)
#define GPU_CMD_AVS "avs"
#define GPU_CMD_DVFS "dvfs"
#define GPU_CMD_ON "on"
#define GPU_CMD_OFF "off"
#define GPU_CMD_VOLT "volt"
#define GPU_CMD_FREQ "freq"
#define GPU_CMD_MAX_FREQ "max_freq"
#define GPU_CMD_MIN_FREQ "min_freq"
#define GPU_CMD_FREQ_RISE "rise"
#define GPU_CMD_FREQ_DROP "drop"
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
        printk(KERN_ERR "Invalid.\n");
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
	HI_U32 u32GpuCurFreq = 0;

	if ((0 == u32Para1) && (0 == u32Para2))
	{
		printk(KERN_ERR "plese set the valid value \n");
		return HI_SUCCESS;
	}

	if(0 != u32Para1)
	{
		if(u32Para1 > kbase_get_max_frequency())
		{
			u32Para1 = kbase_get_max_frequency();
		}
		else if(u32Para1 < kbase_get_min_frequency())
		{
			u32Para1 = kbase_get_min_frequency();
		}
	}

	if (0 == u32Para2)
	{
		kbase_set_frequency_slowly(u32Para1);
		udelay(100);
		printk("\nFrequency set to %d!\n", kbase_get_frequency_slowly());

		return HI_SUCCESS;
	}

	if (0 == u32Para1)
	{
		kbase_set_voltage(u32Para2);
		printk("\nVoltage set to %d!\n", kbase_get_voltage());

		return HI_SUCCESS;
	}

	u32GpuCurFreq = kbase_get_frequency_slowly();
	if (u32Para1 > u32GpuCurFreq)
	{
		kbase_set_voltage(u32Para2);
		kbase_set_frequency_slowly(u32Para1);
		udelay(100);		
		printk("\nFrequency set to %d, Voltage set to %d!\n", kbase_get_frequency_slowly(), kbase_get_voltage());
	}
	else
	{
		kbase_set_frequency_slowly(u32Para1);
		kbase_set_voltage(u32Para2);
		udelay(100);		
		printk("\nFrequency set to %d, Voltage set to %d!\n", kbase_get_frequency_slowly(), kbase_get_voltage());
	}

	return HI_SUCCESS;
}

static HI_S32 GPUProcRead(struct seq_file *p, HI_VOID *v)
{
    PROC_PRINT(p, "---------Hisilicon GPU Info---------\n");

	PROC_PRINT(p, "Frequency			:%d(kHz)\n",kbase_get_frequency_slowly());
	PROC_PRINT(p, "Voltage				:%d(mv)\n", kbase_get_voltage());
	PROC_PRINT(p, "Utilization			:%d(%%)\n", kbase_get_utilisation());
	if (1 == kbase_is_powerup())
	{
		PROC_PRINT(p, "Power_status			:power up\n");
	}
	else
	{
		PROC_PRINT(p, "Power_status			:power down\n");
	}

	if (1 == kbase_is_dvfs_enable())
	{
		PROC_PRINT(p, "DVFS_status			:on\n");
	}
	else
	{
		PROC_PRINT(p, "DVFS_status			:off\n");
	}

	if (1 == kbase_is_avs_enable())
	{
		PROC_PRINT(p, "AVS_status			:on\n");
	}
	else
	{
		PROC_PRINT(p, "AVS_status			:off\n");
	}
	PROC_PRINT(p, "Max_frequency			:%d(kHz)\n", kbase_get_max_frequency());
	PROC_PRINT(p, "Min_frequency			:%d(kHz)\n", kbase_get_min_frequency());

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
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_FREQ_RISE, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			kbase_rise_frequency_slowly();
		}
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_FREQ_DROP, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			kbase_drop_frequency_slowly();
		}
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_MAX_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			kbase_set_max_frequency(GPUParseValue(pstCmd[0].aszValue));
			printk("Max frequency set to %d!\n", kbase_get_max_frequency());
		}
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_MIN_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			kbase_set_min_frequency(GPUParseValue(pstCmd[0].aszValue));
			printk("Min frequency set to %d!\n", kbase_get_min_frequency());
		}
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_DVFS, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
		/* GPU DVFS ON */
			if (0 == HI_OSAL_Strncasecmp(GPU_CMD_ON, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_dvfs_enable(1);
				printk("DVFS enabled!\n");
			}
		/* GPU DVFS OFF */
			else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_OFF, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_dvfs_enable(0);
				printk("DVFS disabled!\n");
			}
		}
		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_AVS, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
        	{
            /* GPU AVS ON */
			if (0 == HI_OSAL_Strncasecmp(GPU_CMD_ON, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
            		{
						kbase_avs_enable(1);
						printk("AVS enabled!\n");
            		}
            /* GPU AVS OFF */
            		else if (0 == HI_OSAL_Strncasecmp(GPU_CMD_OFF, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
            		{
            			kbase_avs_enable(0);
						printk("AVS disabled!\n");
            		}
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
        printk(KERN_ERR "add GPU proc module failed\n");
        return -1;
    }

    item->read = GPUProcRead;
    item->write = GPUProcWrite;
    return 0;
}

int gpu_proc_destroy(void)
{
    HI_DRV_PROC_RemoveModule("pm_gpu");
    return 0;
}
