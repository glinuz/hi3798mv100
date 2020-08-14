/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "drv_i2c.h"
#include "drv_i2c_ext.h"
#include "hi_drv_i2c.h"
#include "drv_i2c_ioctl.h"
#include "hi_common.h"
#include "drv_gpioi2c_ext.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"

#define I2C_WRITE_REG(Addr, Value) ((*(volatile HI_U32 *)(Addr)) = (Value))
#define I2C_READ_REG(Addr) (*(volatile HI_U32 *)(Addr))

extern HI_S32     i2c_pm_resume(PM_BASEDEV_S *pdev);
extern HI_S32     i2c_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);

extern HI_S32  I2C_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);
extern HI_S32  I2C_Compat_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static UMAP_DEVICE_S g_I2cRegisterData;

static GPIO_I2C_EXT_FUNC_S* g_pstGpioI2cExtFuncs = HI_NULL;
extern HI_U32 hi_i2c_valid_num;

extern HI_U32 g_aI2cRate[HI_I2C_MAX_NUM];
#ifdef HI_GPIOI2C_SUPPORT
extern I2C_GPIO_S g_stI2cGpio[HI_I2C_MAX_NUM];
#endif

#if (1 == HI_PROC_SUPPORT)
HI_S32 I2C_ProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U8 ii = 0;
    PROC_PRINT(p, "---------Hisilicon Standard I2C Info---------\n");
    PROC_PRINT(p, "No.            Rate\n");

    for (ii = 0; ii < hi_i2c_valid_num; ii++)
    {
        PROC_PRINT(p, "%d             %d\n", ii, g_aI2cRate[ii]);
    }

#ifdef HI_GPIOI2C_SUPPORT
    PROC_PRINT(p, "---------Hisilicon GPIO simulate I2C Info---------\n");
    PROC_PRINT(p, "No.            SCL_IO       SDA_IO\n");
    for (ii = 0; ii < HI_I2C_MAX_NUM; ii++)
    {
        if (g_stI2cGpio[ii].bUsed)
        {
            PROC_PRINT(p, "%d                %d           %d\n", g_stI2cGpio[ii].I2cNum,
                            g_stI2cGpio[ii].u32SCLGpioNo, g_stI2cGpio[ii].u32SDAGpioNo);
        }
    }
#endif

    return HI_SUCCESS;
}

static void usage(void) {
    HI_PRINT("\nUsage:\n");
    HI_PRINT("Read data: echo 'bus' 'device address' 'Register address' > /proc/msp/i2c\n");
    HI_PRINT("Write data: echo 'bus' 'device address' 'Register address' 'data' > /proc/msp/i2c\n");
    HI_PRINT("or:echo 'bus' 'device address' 'Register address' 'data number n(n<=32)' 'data1' ...'datan > /proc/msp/i2c\n");
    HI_PRINT("such as: echo 4 a0 5d 2 b c > /proc/msp/i2c \n");
    HI_PRINT("Look over i2c info: cat /proc/msp/i2c \n");
}

static HI_S32 I2C_proc_read(HI_U32 u32I2cNo, HI_U32 u32DevAddr, HI_U32 u32RegAddr, HI_U32 u32Val)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PRINT("ProcRead: i2c=%d, addr=0x%x, reg=0x%x\n", u32I2cNo, u32DevAddr, u32RegAddr);
    if (u32I2cNo < hi_i2c_valid_num)
    {
        s32Ret = HI_DRV_I2C_Read(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, 1, (HI_U8 *)&u32Val, 1);
        if (HI_SUCCESS != s32Ret)
            HI_ERR_I2C("Read failed(Ret:0x%x)\n\n", s32Ret);
        else
            HI_PRINT("0x%x\n", u32Val);
        return s32Ret;
    }

    if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIsUsed)
    {
        HI_BOOL bUsed = HI_FALSE;

        s32Ret = g_pstGpioI2cExtFuncs->pfnGpioI2cIsUsed(u32I2cNo, &bUsed);

        if ((s32Ret != HI_SUCCESS) || (bUsed == HI_TRUE)) {
                HI_INFO_I2C("Read failed(I2cNum=%d not valid)! \n", u32I2cNo);
                return HI_FAILURE;
        }

        if (g_pstGpioI2cExtFuncs->pfnGpioI2cReadExt) {
            s32Ret = g_pstGpioI2cExtFuncs->pfnGpioI2cReadExt(u32I2cNo, u32DevAddr, u32RegAddr, 1,
                                                 (unsigned char *)&u32Val, 1);
            if (HI_SUCCESS != s32Ret)
                HI_ERR_I2C("Read failed(Ret:0x%x)\n\n", s32Ret);
            else
                HI_PRINT("0x%x\n", u32Val);
        }
        return s32Ret;

    }

    HI_INFO_I2C("Read failed(I2cNum=%d not valid)! \n", u32I2cNo);
    return HI_FAILURE;
}

static HI_S32 I2C_proc_write(HI_U32 u32I2cNo, HI_U32 u32DevAddr,
        HI_U32 u32RegAddr, HI_U32 u8SendCount, HI_U8 *SendData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_PRINT("Write: u32I2cNo=%d, u32DevAddr=0x%x, u32RegAddr=0x%x, write number=0x%x\n", u32I2cNo,
        u32DevAddr, u32RegAddr, u8SendCount);

    if (u32I2cNo < hi_i2c_valid_num) {
        s32Ret = HI_DRV_I2C_Write(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, 1, SendData, u8SendCount);
        if (HI_SUCCESS != s32Ret)
            HI_ERR_I2C("Write failed(Ret:0x%x)", s32Ret);

        return s32Ret;
    }

    if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIsUsed) {
        HI_BOOL bUsed = HI_FALSE;

        s32Ret = g_pstGpioI2cExtFuncs->pfnGpioI2cIsUsed(u32I2cNo, &bUsed);

        if ((s32Ret != HI_SUCCESS) || (bUsed != HI_TRUE)) {
                HI_INFO_I2C("Write failed(I2cNum=%d not valid)! \n", u32I2cNo);
                return HI_FAILURE;
        }

        if (g_pstGpioI2cExtFuncs->pfnGpioI2cWriteExt) {
            s32Ret = g_pstGpioI2cExtFuncs->pfnGpioI2cWriteExt(u32I2cNo, u32DevAddr, u32RegAddr, 1,
                                                              SendData, u8SendCount);
            if (HI_SUCCESS != s32Ret)
                HI_ERR_I2C("Write failed(Ret:0x%x)\n\n", s32Ret);
        }
        return s32Ret;
    }

    HI_INFO_I2C("Write failed(I2cNum=%d not valid)! \n", u32I2cNo);
    return HI_FAILURE;
}

HI_S32 I2C_ProcWrite(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
{
    HI_U32 u32I2cNo = 0, u32DevAddr = 0, u32RegAddr = 0, u32Val = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    char acInput[256];
    char *p  = HI_NULL;
    HI_U32 i = 0;
    HI_U8 SendData[32] = {0};
    HI_U8 u8SendCount = 0;
    HI_U8 u8ArgCount = 0;
    HI_BOOL bCommondErr = HI_FALSE;
    HI_BOOL bSetRate = HI_FALSE;
    HI_U32 u32Rate = 0;

    if (count >= sizeof(acInput)) {
        HI_PRINT("commond line is too long, please try it in 256 Bytes\n");
        return HI_FAILURE;
    }

    memset(acInput, 0, sizeof(acInput));
    if (copy_from_user(acInput, buf, count))
        return HI_FAILURE;

    acInput[255] = '\0';

    /*begin deal with set rate cmd*/
    p = skip_spaces(acInput);
    if (!strncmp(p, "SetRate", strlen("SetRate"))) {
        s32Ret = sscanf(p, "SetRate %d %d", &u32I2cNo, &u32Rate);
        if (s32Ret != 2) {
            HI_PRINT("%s, i2c:%d, rate:%dHZ\n", p, u32I2cNo, u32Rate);
            bSetRate = HI_FALSE;
        } else
            bSetRate = HI_TRUE;
    }

    if (bSetRate) {
        if (u32I2cNo >= hi_i2c_valid_num)
            HI_PRINT("I2c NO.%d not support rate setting!\n", u32I2cNo);
        else
            (HI_VOID)I2C_DRV_SetRate(u32I2cNo, u32Rate);

        return count;
    }
    /*end deal with set rate cmd*/

    p = acInput;
    u8ArgCount = 0;

    for (i = 0; i < count && i < strlen(acInput); i++) {
        if ((acInput[i] == ' ') || (acInput[i] == '\n')) {
            u8ArgCount++;
        }
    }

    if (u8ArgCount < 3) {
        bCommondErr = HI_TRUE;
        goto exit;
    }

    u32I2cNo   = (HI_U32)simple_strtoul(p, &p, 16);
    u32DevAddr = (HI_U32)simple_strtoul(p + 1, &p, 16);
    u32RegAddr = (HI_U32)simple_strtoul(p + 1, &p, 16);


    u32Val = (HI_U32)simple_strtoul(p + 1, &p, 16);

    if (u8ArgCount > 4) {
        if (u32Val != (u8ArgCount - 4)) {
            bCommondErr = HI_TRUE;
            goto exit;
        }

        if ((u8ArgCount - 4) > sizeof(SendData)) {
            bCommondErr = HI_TRUE;
            goto exit;
        }

        for (i = 0; i < (u8ArgCount - 4); i++) {
            SendData[i] = (HI_U8)simple_strtoul(p + 1, &p, 16);
        }

        u8SendCount = u8ArgCount - 4;
    } else {
        SendData[0] = u32Val;
        u8SendCount = 1;
    }


    if ((u32I2cNo >= hi_i2c_valid_num) && (g_pstGpioI2cExtFuncs == HI_NULL)) {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID * *)&g_pstGpioI2cExtFuncs);
        if ((HI_SUCCESS != s32Ret) || (HI_NULL == g_pstGpioI2cExtFuncs)) {
            HI_PRINT("GPIO_I2C Function ERR: ret:0x%08x, func:0x%p\n", s32Ret,
                   g_pstGpioI2cExtFuncs);
            goto exit;
        }
    }

    if (u8ArgCount == 3)
            I2C_proc_read(u32I2cNo, u32DevAddr, u32RegAddr, u32Val);
        else
            I2C_proc_write(u32I2cNo, u32DevAddr, u32RegAddr, u8SendCount, SendData);

exit:
    if (bCommondErr) {
        usage();
    }

    return count;
}
#endif

static PM_BASEOPS_S i2c_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = i2c_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = i2c_pm_resume,
};

/*****************************************************************************/

static long I2C_INTF_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret = HI_FAILURE;
    I2C_DATA_S I2cData;
    void __user *argp = (void __user*)arg;

    switch (cmd)
    {
    /* if I2C channel num < 4 ,when execute ioctl select I2C_Ioctl,otherwise selet HI_DRV_GPIOI2C_Ioctl,  the same macro will compatible i2c and gpio-i2c*/
    case CMD_I2C_WRITE:
    case CMD_I2C_READ:
        if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_S))) {
            HI_INFO_I2C("copy data from user fail!\n");
            ret = HI_ERR_I2C_COPY_DATA_ERR;
            return ret;
        }

        if (I2cData.I2cNum >= HI_I2C_MAX_NUM) {
            HI_INFO_I2C("i2c chanenl (NO:%d) is invalid !\n", I2cData.I2cNum);
            return HI_ERR_I2C_INVALID_PARA;
        }

        if (I2cData.I2cNum < hi_i2c_valid_num) {
            return I2C_Ioctl(file, cmd, arg);

        } else if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl) {
            ret = g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl(file, cmd, arg);
        }  

        break;

    case CMD_I2C_SET_RATE:
            ret = I2C_Ioctl(file, cmd, arg);
            break;

    case CMD_I2C_CONFIG:
    case CMD_I2C_DESTROY:
            if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl)
                ret = g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl(file, cmd, arg);

            break;

    default:
            ret = -ENOIOCTLCMD;
            break;
    }

    return ret;
}

#ifdef CONFIG_COMPAT
static long I2C_INTF_Compat_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret = HI_FAILURE;
    I2C_DATA_COMPAT_S I2cData;
    void __user *argp = (void __user*)arg;

    switch (cmd)
    {
    /* if I2C channel num < 4 ,when execute ioctl select I2C_Ioctl,otherwise selet HI_DRV_GPIOI2C_Ioctl,  the same macro will compatible i2c and gpio-i2c*/
    case CMD_I2C_WRITE:
    case CMD_I2C_READ:
            if (copy_from_user(&I2cData, argp, sizeof(I2C_DATA_COMPAT_S))) {
                HI_INFO_I2C("copy data from user fail\n");
                ret = HI_ERR_I2C_COPY_DATA_ERR;
                return ret;
            }

            if (I2cData.I2cNum >= HI_I2C_MAX_NUM) {
                HI_INFO_I2C("i2c chanenl (NO:%d) is invalid !\n", I2cData.I2cNum);
                return HI_ERR_I2C_INVALID_PARA;
            }

            if (I2cData.I2cNum < hi_i2c_valid_num ) {
                return I2C_Compat_Ioctl(file, cmd, arg);

            } else if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl) {
                ret = g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl(file, cmd, arg);
            }

            break;

    case CMD_I2C_SET_RATE:
            ret = I2C_Compat_Ioctl(file, cmd, arg);
            break;

    case CMD_I2C_CONFIG:
    case CMD_I2C_DESTROY:
            if (g_pstGpioI2cExtFuncs && g_pstGpioI2cExtFuncs->pfnGpioI2cIOCtl)
                ret = g_pstGpioI2cExtFuncs->pfnGpioI2cCompatIOCtl(file, cmd, arg);

            break;

    default:
            ret = -ENOIOCTLCMD;
            break;
    }

    return ret;
}
#endif

HI_S32 I2C_Open(struct inode *inode, struct file *filp)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID * *)&g_pstGpioI2cExtFuncs);
    if ((HI_SUCCESS != s32Ret) || (HI_NULL == g_pstGpioI2cExtFuncs))
    {
        HI_INFO_I2C("Get GPIO_I2C Function ERR: ret:0x%08x\n", s32Ret);
    }

    return HI_SUCCESS;
}

HI_S32 I2C_Close(struct inode *inode, struct file *filp)
{
    return HI_SUCCESS;
}

static struct file_operations I2C_FOPS =
{
    .owner = THIS_MODULE,
    .open = I2C_Open,
    .unlocked_ioctl = I2C_INTF_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = I2C_INTF_Compat_Ioctl,
#endif
    .release = I2C_Close,
};

HI_S32 I2C_DRV_ModInit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S  *pProcItem;
#endif
    HI_DRV_I2C_Init(); /*HI_MCE_SUPPORT=yes or HI_MCE_SUPPORT=no, all will call HI_DRV_I2C_Init, make sure HI_I2C_Open call it */

    /* I2C driver register */
    snprintf(g_I2cRegisterData.devfs_name, sizeof(g_I2cRegisterData.devfs_name), UMAP_DEVNAME_I2C);
    g_I2cRegisterData.minor = UMAP_MIN_MINOR_I2C;
    g_I2cRegisterData.owner = THIS_MODULE;
    g_I2cRegisterData.fops   = &I2C_FOPS;
    g_I2cRegisterData.drvops = &i2c_drvops;
    if (HI_DRV_DEV_Register(&g_I2cRegisterData) < 0)
    {
        HI_FATAL_I2C("register I2C failed.\n");
        return HI_FAILURE;
    }
#if (1 == HI_PROC_SUPPORT)
    /* register i2c PROC funtion*/
    pProcItem = HI_DRV_PROC_AddModule(HI_MOD_I2C, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_INFO_I2C("add I2C proc failed.\n");
        HI_DRV_DEV_UnRegister(&g_I2cRegisterData);
        return HI_FAILURE;
    }

    pProcItem->read  = I2C_ProcRead;
    pProcItem->write = I2C_ProcWrite;
#endif
#ifdef MODULE
    HI_PRINT("Load hi_i2c.ko success.  \t(%s)\n", VERSION_STRING);
#endif
    return 0;
}

HI_VOID I2C_DRV_ModExit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_DRV_PROC_RemoveModule(HI_MOD_I2C);
#endif
    HI_DRV_DEV_UnRegister(&g_I2cRegisterData);

    HI_DRV_I2C_DeInit();

    return;
}

#ifdef MODULE
module_init(I2C_DRV_ModInit);
module_exit(I2C_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");
