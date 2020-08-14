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
#include <linux/sched.h>
#include <linux/i2c.h>
#include <linux/compat.h>

#include "hi_kernel_adapt.h"
#include "hi_drv_reg.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"
#include "drv_i2c.h"
#include "drv_i2c_ioctl.h"
#include "drv_i2c_ext.h"
#include "hi_common.h"
#include "hi_reg_common.h"
#include "hi_drv_i2c.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"

#define SONY_I2C_START_EN          (0x01)  /**< Output START (S) */
#define SONY_I2C_STOP_EN           (0x02)  /**< Output STOP  (P) */
#define I2C_WAIT_TIME_OUT          0x1000
#define I2C_ADDR_SIZE              0x1000
#define HI_MAX_I2C_NUM             8

/* I2C extend flags */
#define HII2C_M_NOSTOP             0x0100
#define HII2C_M_NOSTART            0x0200
#define HII2C_M_NOSTART_NOADDR     0x0400

enum hi_i2c_state {
    HII2C_STAT_RW_ERR = -1,
    HII2C_STAT_INIT,
    HII2C_STAT_RW,
    HII2C_STAT_SND_STOP,
    HII2C_STAT_RW_SUCCESS,
};

struct hi_i2c_dev {
    struct i2c_client *client;
    struct semaphore *sem;
    u32 num;
};

struct hi_i2c_host {
    struct device *dev;
    void __iomem *regs;
    struct clk *clk;
    int irq;
    
    struct i2c_adapter adapter;
    struct completion completion;

    struct i2c_msg *msg;
    unsigned int pos;
    unsigned int msg_len;
    
    int stop;
    int err;
    unsigned int freq;
    enum hi_i2c_state state;
};

static struct hi_i2c_dev hi_i2c_dummy = {0};
HI_U32 hi_i2c_valid_num = 0;
HI_U32 g_aI2cRate[HI_MAX_I2C_NUM] = {0};
static atomic_t i2c_init_counter = ATOMIC_INIT(0);

HI_S32 i2c_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return 0;
}

HI_S32 i2c_pm_resume(PM_BASEDEV_S *pdev)
{
    return 0;
}


static I2C_EXT_FUNC_S g_stI2cExtFuncs =
{
    .pfnI2cWriteConfig  = HI_DRV_I2C_WriteConfig,
    .pfnI2cWrite        = HI_DRV_I2C_Write,
    .pfnI2cRead         = HI_DRV_I2C_Read,
    .pfnI2cWriteNostop  = HI_DRV_I2C_Write_NoSTOP,
    .pfnI2cReadDirectly = HI_DRV_I2C_ReadDirectly,
    .pfnI2cSetRate      = HI_DRV_I2C_SetRate,
    .pfnI2cSuspend      = i2c_pm_suspend,
    .pfnI2cResume       = i2c_pm_resume,
};


HI_S32 I2C_DRV_SetRate(HI_U32 channel, HI_U32 I2cRate)
{
    struct hi_i2c_host *i2c_dev;

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    g_aI2cRate[channel] = I2cRate;

    i2c_dev = i2c_get_adapdata(hi_i2c_dummy.client[channel].adapter);
    i2c_dev->freq = I2cRate;

    return HI_SUCCESS;
}


/*
add by Jiang Lei 2010-08-24
I2C write finished acknowledgement function
it use to e2prom device ,make sure it finished write operation.
i2c master start next write operation must waiting when it acknowledge
e2prom write cycle finished.
 */
HI_S32 I2C_DRV_WriteConfig(HI_U32 channel, HI_U8 devAddr)
{
    return 0;
}

HI_S32 I2C_DRV_Write(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize, HI_BOOL bWithStop)
{
    HI_S32 ret;
    char *msgbuf;
    struct i2c_msg msg;
    char reg[4] = {0};

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pData) {
        HI_ERR_I2C("pData null\n");
        return HI_ERR_I2C_NULL_PTR;
    }

    if (regAddrSize > sizeof(HI_U32)) {
        HI_ERR_I2C("regAddrsize exceed sizeof HI_U32\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    memset(&msg, 0, sizeof(struct i2c_msg));
    msgbuf = kzalloc(regAddrSize + dataSize, GFP_KERNEL);
    if (!msgbuf) {
        HI_ERR_I2C("regAddrsize exceed sizeof HI_U32\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    msg.addr = devAddr >> 1;
    if (!bWithStop)
        msg.flags |= HII2C_M_NOSTOP;

    msg.len = regAddrSize + dataSize;

    reg[0] = (regAddr >> 24) & 0xFF;
    reg[1] = (regAddr >> 16) & 0xFF;
    reg[2] = (regAddr >> 8)  & 0xFF;
    reg[3] = (regAddr >> 0)  & 0xFF;
    memcpy(msgbuf, &(reg[sizeof(reg)-regAddrSize]), regAddrSize);
    memcpy(msgbuf+regAddrSize, (void *)pData, dataSize);
    msg.buf = msgbuf;

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, &msg, 1);
    if (ret<0) {
        HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
        ret = HI_ERR_I2C_WRITE_TIMEOUT;
    } else
        ret = HI_SUCCESS;

    kfree(msgbuf);
    return ret;
}

int I2C_DRV_Read(HI_U32 channel, HI_U8 devAddr, HI_BOOL bSendSlave, 
                 HI_U32 regAddr, HI_U32 regAddrSize,
                 HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;
    struct i2c_msg msg[2];
    char *msgbuf;
    char reg[4] = {0};

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pData) {
        HI_ERR_I2C("pData null\n");
        return HI_ERR_I2C_NULL_PTR;
    }

    if (regAddrSize > sizeof(HI_U32)) {
        HI_ERR_I2C("regAddrsize exceed sizeof HI_U32\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    msgbuf = kzalloc(regAddrSize, GFP_KERNEL);
    if (!msgbuf) {
        HI_ERR_I2C("I2C_DRV_Read kzalloc failed\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    memset(msg, 0, sizeof(msg));

    reg[0] = (regAddr >> 24) & 0xFF;
    reg[1] = (regAddr >> 16) & 0xFF;
    reg[2] = (regAddr >> 8)  & 0xFF;
    reg[3] = (regAddr >> 0)  & 0xFF;
    memcpy(msgbuf, &(reg[sizeof(reg)-regAddrSize]), regAddrSize);

    msg[0].addr = devAddr >> 1;
    msg[0].len = regAddrSize;
    msg[0].buf = msgbuf;
    msg[0].flags = 0;
    if (!bSendSlave)
        msg[0].flags |= HII2C_M_NOSTART_NOADDR;

    msg[1].addr = devAddr >> 1;
    msg[1].flags |= I2C_M_RD;
    msg[1].len = (u16)dataSize;
    msg[1].buf = pData;

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, msg, 2);
    if (ret<0) {
        HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
        ret = HI_ERR_I2C_WRITE_TIMEOUT;
    } else
        ret = HI_SUCCESS;

    kfree(msgbuf);
    return ret;  

}

int I2C_DRV_Read_SiLabs(HI_U32 channel, HI_U8 devAddr, HI_BOOL bSendSlave,
        HI_U32 regAddr, HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;
    struct i2c_msg msg[2];

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pData) {
        HI_ERR_I2C("pData null\n");
        return HI_ERR_I2C_NULL_PTR;
    }

    memset(msg, 0, sizeof(msg));

    msg[0].addr = devAddr >> 1;
    msg[0].len = 1;
    msg[0].buf = pData;
    msg[0].flags |= I2C_M_RD;

    msg[1].addr = devAddr >> 1;
    msg[1].flags |= I2C_M_RD;
    msg[1].len = dataSize;
    msg[1].buf = pData;

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, &msg[0], 1);
    if (ret<0) {
        HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
        return HI_ERR_I2C_WRITE_TIMEOUT;
    } 

    udelay(5);

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, &msg[1], 1);
    if (ret<0) {
        HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
        ret = HI_ERR_I2C_WRITE_TIMEOUT;
    } else 
        ret = HI_SUCCESS;

    return ret;  
}

HI_S32 I2C_DRV_Write_sony(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize, HI_U8 u8Mode)
{
    HI_S32 ret;
    struct i2c_msg msg = {0};
    char *msgbuf;
    char reg[4] = {0};

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pData) {
        HI_ERR_I2C("pData null\n");
        return HI_ERR_I2C_NULL_PTR;
    }

    if (regAddrSize > sizeof(HI_U32)) {
        HI_ERR_I2C("regAddrsize exceed sizeof HI_U32\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    msgbuf = kzalloc(regAddrSize + dataSize, GFP_KERNEL);
    if (!msgbuf) {
        HI_ERR_I2C("I2C_DRV_Read kzalloc failed\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }
    memset(&msg, 0, sizeof(struct i2c_msg));
    msg.addr = devAddr >> 1;
    if (!u8Mode & SONY_I2C_START_EN)
        msg.flags |= HII2C_M_NOSTART;

    if (!(u8Mode & SONY_I2C_STOP_EN))
        msg.flags |= HII2C_M_NOSTOP;

    msg.len = regAddrSize + dataSize;

    reg[0] = (regAddr >> 24) & 0xFF;
    reg[1] = (regAddr >> 16) & 0xFF;
    reg[2] = (regAddr >> 8)  & 0xFF;
    reg[3] = (regAddr >> 0)  & 0xFF;
    memcpy(msgbuf, &(reg[sizeof(reg)-regAddrSize]), regAddrSize);
    
    memcpy(msgbuf+regAddrSize, pData, dataSize);
    msg.buf = msgbuf;

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, &msg, 1);
    if (ret<0) {
                HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
                ret = HI_ERR_I2C_WRITE_TIMEOUT;
    } else
        ret = HI_SUCCESS;

    kfree(msgbuf);
    return ret;
}

int I2C_DRV_Read_sony(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize, HI_U8 u8Mode)
{
    HI_S32 ret;
    struct i2c_msg msg = {0};
    char *msgbuf;
    char reg[4] = {0};

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("i2c channel %d exceed max channel num %d\n",
                channel, hi_i2c_dummy.num);
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pData) {
        HI_ERR_I2C("pData null\n");
        return HI_ERR_I2C_NULL_PTR;
    }

    if (regAddrSize > sizeof(HI_U32)) {
        HI_ERR_I2C("regAddrsize exceed sizeof HI_U32\n");
        return HI_ERR_I2C_INVALID_PARA;
    }

    memset(&msg, 0, sizeof(struct i2c_msg));
    msgbuf = kzalloc(regAddrSize + dataSize, GFP_KERNEL);
    if (!msgbuf) {
        HI_ERR_I2C("I2C_DRV_Read kzalloc failed\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    msg.addr = devAddr >> 1;
    msg.flags |= I2C_M_RD;
    if (!u8Mode & SONY_I2C_START_EN)
        msg.flags |= HII2C_M_NOSTART_NOADDR;

    if (!(u8Mode & SONY_I2C_STOP_EN))
        msg.flags |= HII2C_M_NOSTOP;

    msg.len = regAddrSize + dataSize;
    reg[0] = (regAddr >> 24) & 0xFF;
    reg[1] = (regAddr >> 16) & 0xFF;
    reg[2] = (regAddr >> 8)  & 0xFF;
    reg[3] = (regAddr >> 0)  & 0xFF;
    memcpy(msgbuf, &(reg[sizeof(reg)-regAddrSize]), regAddrSize);
    memcpy(msgbuf+regAddrSize, pData, dataSize);
    msg.buf = msgbuf;

    ret = i2c_transfer(hi_i2c_dummy.client[channel].adapter, &msg, 1);
    if (ret<0) {
        HI_ERR_I2C("i2c_transfer failed:%d\n", ret);
        ret = HI_ERR_I2C_WRITE_TIMEOUT;
    } else
        ret = HI_SUCCESS;

    kfree(msgbuf);
    return ret;
}

/*****************************************************************************/

HI_S32 I2C_GetDataFromUser(unsigned long arg, I2C_DATA_S *i2cData,
        HI_U8 **pData, HI_BOOL copyData)
{
    void __user *argp = (void __user*)arg;

    if (!i2cData || !pData) {
        return HI_ERR_I2C_NULL_PTR;
    }

    if (copy_from_user(i2cData, argp, sizeof(I2C_DATA_S))) {
        HI_INFO_I2C("copy data from user fail\n");
        return HI_ERR_I2C_COPY_DATA_ERR;
    }

    if ((i2cData->DataLen > HI_I2C_MAX_LENGTH) ||
        (i2cData->I2cNum >= hi_i2c_dummy.num)) {
        return HI_ERR_I2C_INVALID_PARA;
    }

    *pData = HI_KMALLOC(HI_ID_I2C, i2cData->DataLen, GFP_KERNEL);
    if (!(*pData)) {
        HI_ERR_I2C("i2c kmalloc fail\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    if (copyData) {
        if (copy_from_user(*pData, i2cData->pData, i2cData->DataLen)) {
            HI_INFO_I2C("copy data from user fail\n");
            HI_KFREE(HI_ID_I2C, pData);
            pData = NULL;
            return HI_ERR_I2C_COPY_DATA_ERR;
        }
    }

    return HI_SUCCESS;
}

HI_S32 I2C_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    HI_S32  ret;
    HI_U8  *pData = NULL;
    I2C_DATA_S I2cData;
    I2C_RATE_S I2cRate;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    switch (cmd) {
    case CMD_I2C_WRITE:
        ret = I2C_GetDataFromUser(arg, &I2cData, &pData, HI_TRUE);
        if (ret != HI_SUCCESS) {
            if (pData) {
                HI_KFREE(HI_ID_I2C, pData);
                pData = NULL;
            }
            break;
        }

        if (I2cData.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cData.I2cNum, hi_i2c_dummy.num);
            HI_KFREE(HI_ID_I2C, pData);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cData.I2cNum]);
        if (ret) {
            HI_KFREE(HI_ID_I2C, pData);
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cData.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_Write(I2cData.I2cNum, I2cData.I2cDevAddr,
                I2cData.I2cRegAddr, I2cData.I2cRegCount, pData,
                I2cData.DataLen, HI_TRUE);

        HI_KFREE(HI_ID_I2C, pData);

        up(&hi_i2c_dummy.sem[I2cData.I2cNum]);

        break;

    case CMD_I2C_READ:
        ret = I2C_GetDataFromUser(arg, &I2cData, &pData, HI_FALSE);
        if (ret != HI_SUCCESS) {
            if (pData) {
                HI_KFREE(HI_ID_I2C, pData);
                pData = NULL;
            }
            HI_ERR_I2C("CMD_I2C_READ get data from user failed: 0x%x\n", ret);
            break;
        }

        if (I2cData.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cData.I2cNum, hi_i2c_dummy.num);
            HI_KFREE(HI_ID_I2C, pData);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cData.I2cNum]);
        if (ret) {
            HI_KFREE(HI_ID_I2C, pData);
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cData.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_Read(I2cData.I2cNum, I2cData.I2cDevAddr, HI_TRUE,
                I2cData.I2cRegAddr, I2cData.I2cRegCount, pData, I2cData.DataLen);

        if (HI_SUCCESS == ret) {
            if (copy_to_user(I2cData.pData, pData, I2cData.DataLen)) {
                HI_ERR_I2C("copy data to user fail\n");
                ret = HI_ERR_I2C_COPY_DATA_ERR;
            }
        }

        HI_KFREE(HI_ID_I2C, pData);

        up(&hi_i2c_dummy.sem[I2cData.I2cNum]);

        break;

    case CMD_I2C_SET_RATE:
        if (copy_from_user(&I2cRate, (void __user *)arg, sizeof(I2C_RATE_S))) {
            HI_ERR_I2C("copy data from user fail\n");
            ret = HI_FAILURE;
            break;
        }

        if (I2cRate.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cRate.I2cNum, hi_i2c_dummy.num);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cRate.I2cNum]);
        if (ret) {
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cRate.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_SetRate(I2cRate.I2cNum, I2cRate.I2cRate);

        up(&hi_i2c_dummy.sem[I2cRate.I2cNum]);
        break;

    default:
        return -ENOIOCTLCMD;

    }

    return ret;
}

#ifdef CONFIG_COMPAT
HI_S32 I2C_GetDataFromUser_Compat(unsigned long arg, I2C_DATA_COMPAT_S *i2cData,
        HI_U8 **pData, HI_BOOL copyData)
{
    void __user *argp = (void __user*)arg;

    if (!i2cData || !pData) {
        return HI_ERR_I2C_NULL_PTR;
    }

    if (copy_from_user(i2cData, argp, sizeof(I2C_DATA_S))) {
        HI_ERR_I2C("copy data from user fail\n");
        return HI_ERR_I2C_COPY_DATA_ERR;
    }

    if ((i2cData->DataLen > HI_I2C_MAX_LENGTH) ||
        (i2cData->I2cNum >= hi_i2c_dummy.num)) {
        return HI_ERR_I2C_INVALID_PARA;
    }

    *pData = HI_KMALLOC(HI_ID_I2C, i2cData->DataLen, GFP_KERNEL);
    if (!(*pData)) {
        HI_ERR_I2C("i2c kmalloc fail\n");
        return HI_ERR_I2C_MALLOC_ERR;
    }

    if (copyData) {
        if (copy_from_user(*pData, (HI_U8 *)compat_ptr(i2cData->pData), i2cData->DataLen)) {
            HI_ERR_I2C("copy data from user fail\n");
            HI_KFREE(HI_ID_I2C, pData);
            pData = NULL;
            return HI_ERR_I2C_COPY_DATA_ERR;
        }
    }

    return HI_SUCCESS;
}


HI_S32 I2C_Compat_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    HI_S32  ret;
    HI_U8  *pData = NULL;
    I2C_DATA_COMPAT_S I2cData;
    I2C_RATE_S I2cRate;
    void __user *argp = (void __user*)arg;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }
        
    switch (cmd) {
    case CMD_I2C_WRITE:
        ret = I2C_GetDataFromUser_Compat(arg, &I2cData, &pData, HI_TRUE);
        if (ret != HI_SUCCESS) {
            if (pData) {
                HI_KFREE(HI_ID_I2C, pData);
                pData = NULL;
            }
            break;
        }

        if (I2cData.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cData.I2cNum, hi_i2c_dummy.num);
            HI_KFREE(HI_ID_I2C, pData);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cData.I2cNum]);
        if (ret) {
            HI_KFREE(HI_ID_I2C, pData);
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cData.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_Write(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr,
                I2cData.I2cRegCount, pData, I2cData.DataLen, HI_TRUE);

        HI_KFREE(HI_ID_I2C, pData);

        up(&hi_i2c_dummy.sem[I2cData.I2cNum]);

        break;

    case CMD_I2C_READ:
        ret = I2C_GetDataFromUser_Compat(arg, &I2cData, &pData, HI_FALSE);
        if (ret != HI_SUCCESS) {
            if (pData) {
                HI_KFREE(HI_ID_I2C, pData);
                pData = NULL;
            }
            break;
        }

        if (I2cData.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cData.I2cNum, hi_i2c_dummy.num);
            HI_KFREE(HI_ID_I2C, pData);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cData.I2cNum]);
        if (ret) {
            HI_KFREE(HI_ID_I2C, pData);
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cData.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_Read(I2cData.I2cNum, I2cData.I2cDevAddr, HI_TRUE,
                I2cData.I2cRegAddr, I2cData.I2cRegCount, pData, I2cData.DataLen);

        if (HI_SUCCESS == ret) {
            if (copy_to_user((HI_U8 *)compat_ptr(I2cData.pData),
                pData, I2cData.DataLen)) {
                HI_ERR_I2C("copy data to user fail!\n");
                ret = HI_ERR_I2C_COPY_DATA_ERR;
            }
        }

        HI_KFREE(HI_ID_I2C, pData);

        up(&hi_i2c_dummy.sem[I2cData.I2cNum]);

        break;

    case  CMD_I2C_SET_RATE:
        if (copy_from_user(&I2cRate, argp, sizeof(I2C_RATE_S))) {
            HI_ERR_I2C("copy data from user fail\n");
            ret = HI_FAILURE;
            break;
        }

        if (I2cRate.I2cNum >= hi_i2c_dummy.num) {
            HI_ERR_I2C("channel %d exceed max i2c num %d\n", I2cRate.I2cNum, hi_i2c_dummy.num);
            return HI_ERR_I2C_INVALID_PARA;
        }

        ret = down_interruptible(&hi_i2c_dummy.sem[I2cRate.I2cNum]);
        if (ret) {
            HI_ERR_I2C("i2c channel %d in use, lock failed\n", I2cRate.I2cNum);
            return HI_FAILURE;
        }

        ret = I2C_DRV_SetRate(I2cRate.I2cNum, I2cRate.I2cRate);

        up(&hi_i2c_dummy.sem[I2cRate.I2cNum]);
        break;

    default:
        return -ENOIOCTLCMD;
    }

    return ret;
}
#endif


static HI_S32 I2C_DRV_GetAdaptersNum(struct device *dev, void *data)
{
        int id = 0;
        if (dev->type != &i2c_adapter_type)
                return 0;
        if (dev->of_node) {
                id = of_alias_get_id(dev->of_node, "i2c");
                if (id > *((long *)data))
                        *((long *)data) = id;
        } else {
            HI_ERR_I2C("i2c not configure in dts\n");
            return -1;
        }

        return 0;
}

static HI_S32 I2C_DRV_DummyAttachAdapter(struct device *dev, void *dummy)
{
    struct i2c_adapter *adap;

        if (dev->type != &i2c_adapter_type)
                return 0;

        adap = to_i2c_adapter(dev);
        if (!adap) {
                HI_ERR_I2C("i2c_get_adapter failed\n");
                return -ENODEV;
        }

        if (adap->nr >= hi_i2c_dummy.num) {
                HI_ERR_I2C("i2c_for_each_dev error, i2c_num:%d\n", hi_i2c_dummy.num);
                return 0;
        }

        sema_init(&hi_i2c_dummy.sem[adap->nr], 1);
        hi_i2c_dummy.client[adap->nr].adapter = adap;
    return 0;
}

static HI_VOID I2C_DRV_DummyDetachAdapter(HI_VOID)
{
    HI_S32 pos = 0;
    for (pos=0; pos<hi_i2c_dummy.num; pos++) {
        if (hi_i2c_dummy.client[pos].adapter)
            i2c_put_adapter(hi_i2c_dummy.client[pos].adapter);
    }
}

HI_S32 HI_DRV_I2C_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    long i2c_dev_num = -1;

    if (atomic_inc_return(&i2c_init_counter) != 1) {
        HI_WARN_I2C(" HI_DRV_I2C already registered:%d\n",
            atomic_read(&i2c_init_counter));
        return HI_SUCCESS;
    }

    s32Ret = HI_DRV_MODULE_Register(HI_ID_I2C, "HI_I2C", 
            (HI_VOID *)&g_stI2cExtFuncs);
    if (HI_SUCCESS != s32Ret) {
        HI_ERR_I2C("I2C register failed\n");
        return HI_FAILURE;
    }

    i2c_for_each_dev(&i2c_dev_num, I2C_DRV_GetAdaptersNum);
    if (i2c_dev_num == -1) {
        HI_ERR_I2C("i2c device not found, init failed: %d\n", i2c_dev_num);
        return HI_FAILURE;
    }

    hi_i2c_dummy.num = i2c_dev_num + 1;
    hi_i2c_valid_num = hi_i2c_dummy.num;

    hi_i2c_dummy.client = kzalloc(sizeof(struct i2c_client)*hi_i2c_dummy.num, GFP_KERNEL);
    if (!hi_i2c_dummy.client) {
        HI_ERR_I2C("hi_i2c_dummy kzalloc i2c_client failed:%d\n", hi_i2c_dummy.num);
        return HI_ERR_I2C_MALLOC_ERR;
    }

    hi_i2c_dummy.sem = kzalloc(sizeof(struct semaphore)*hi_i2c_dummy.num, GFP_KERNEL);
    if (!hi_i2c_dummy.sem) {
            HI_ERR_I2C("hi_i2c_dummy kzalloc sema failed\n");
                s32Ret = HI_ERR_I2C_MALLOC_ERR;
                goto sema_err;
        }

    s32Ret = i2c_for_each_dev(NULL, I2C_DRV_DummyAttachAdapter);
    if (s32Ret) {
        HI_ERR_I2C("hi_i2c_dummy attach adapter failed\n");
        s32Ret = HI_ERR_I2C_OPEN_ERR;
        goto attach_err;
    }

    return HI_SUCCESS;
    
attach_err:
    if (hi_i2c_dummy.sem) {
        kfree(hi_i2c_dummy.sem);
        hi_i2c_dummy.sem = NULL;
    }

sema_err:
    if (hi_i2c_dummy.client) {
        kfree(hi_i2c_dummy.client);
        hi_i2c_dummy.client = NULL;
    }
    return s32Ret;
}

HI_VOID HI_DRV_I2C_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (!atomic_dec_and_test(&i2c_init_counter)) {
        HI_WARN_I2C("HI_DRV_I2C_DeInit counter:%d\n",
            atomic_read(&i2c_init_counter));
        return;
    }

    I2C_DRV_DummyDetachAdapter();

    kfree(hi_i2c_dummy.client);
    kfree(hi_i2c_dummy.sem);
    hi_i2c_dummy.client = NULL;
    hi_i2c_dummy.sem = NULL;
    hi_i2c_dummy.num = 0;
    hi_i2c_valid_num = 0;

    s32Ret = HI_DRV_MODULE_UnRegister(HI_ID_I2C);
    if (HI_SUCCESS != s32Ret) {
        HI_ERR_I2C("I2C unregister failed\n");
    }

    pr_notice("HI_DRV_I2C_DeInit success\n");
    return;
}

HI_S32 HI_DRV_I2C_WriteConfig(HI_U32 channel, HI_U8 devAddr)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }
    ret = I2C_DRV_WriteConfig(channel, devAddr);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Write(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Write(channel, devAddr, regAddr, regAddrSize,
            pData, dataSize, HI_TRUE);

    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
                "Num=%d, Len=%d, data0=0x%x\n", ret, channel, devAddr,
                regAddr, regAddrSize, dataSize, pData[0]);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Write_NoSTOP(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Write(channel, devAddr, regAddr, regAddrSize,
            pData, dataSize, HI_FALSE);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
            "Num=%d, Len=%d, data0=0x%x\n", ret, channel, devAddr,
            regAddr, regAddrSize, dataSize, pData[0]);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Read(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }


    ret = I2C_DRV_Read(channel, devAddr, HI_TRUE, regAddr, regAddrSize,
            pData, dataSize);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
            "Num=%d, Len=%d\n", ret, channel, devAddr, regAddr,
            regAddrSize, dataSize);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Read_SiLabs(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
        HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Read_SiLabs(channel, devAddr, HI_TRUE, regAddr,
            regAddrSize, pData, dataSize);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
            "Num=%d, Len=%d\n", ret, channel, devAddr, regAddr,
            regAddrSize, dataSize);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Write_sony(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
            HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize, HI_U8 u8Mode)
{
    HI_S32  ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Write_sony(channel, devAddr, regAddr, regAddrSize,
            pData, dataSize, u8Mode);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, Num=%d, "
            "Len=%d, data0=0x%x\n", ret, channel, devAddr, regAddr,
            regAddrSize, dataSize, pData[0]);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_Read_sony(HI_U32 channel, HI_U8 devAddr, HI_U32 regAddr,
            HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize, HI_U8 u8Mode)
{
    HI_S32  ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Read_sony(channel, devAddr, regAddr, regAddrSize,
            pData, dataSize, u8Mode);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
            "Num=%d, Len=%d\n", ret, channel, devAddr, regAddr,
            regAddrSize, dataSize);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

/* Added begin: l00185424 20120131, for avl6211 demod */
/* Some I2C needn't send slave address before read */
HI_S32 HI_DRV_I2C_ReadDirectly(HI_U32 channel, HI_U8 devAddr, 
        HI_U32 regAddr, HI_U32 regAddrSize, HI_U8 *pData, HI_U32 dataSize)
{
    HI_S32 ret;

    if (atomic_read(&i2c_init_counter) <= 0) {
        HI_ERR_I2C("i2c not initialized\n");
        return HI_FAILURE;
    }

    if (channel >= hi_i2c_dummy.num) {
        HI_ERR_I2C("channel %d exceed max i2c num %d\n", channel, hi_i2c_dummy.num);
        return HI_FAILURE;
    }

    ret = down_interruptible(&hi_i2c_dummy.sem[channel]);
    if (ret) {
        HI_INFO_I2C("i2c channel %d in use, lock failed\n", channel);
        return HI_FAILURE;
    }

    ret = I2C_DRV_Read(channel, devAddr, HI_FALSE, regAddr,
            regAddrSize, pData, dataSize);
    HI_INFO_I2C("ret=0x%x, channel=%d, DevAddr=0x%x, RegAddr=0x%x, "
            "Num=%d, Len=%d\n", ret, channel, devAddr,regAddr,
            regAddrSize, dataSize);

    up(&hi_i2c_dummy.sem[channel]);

    return ret;
}

HI_S32 HI_DRV_I2C_SetRate(HI_U32 channel, HI_U32 I2cRate)
{
    return I2C_DRV_SetRate(channel, I2cRate);
}

/* Added end: l00185424 20120131, for avl6211 demod */

#ifndef MODULE
EXPORT_SYMBOL(I2C_Ioctl);
#endif
EXPORT_SYMBOL(HI_DRV_I2C_Init);
EXPORT_SYMBOL(HI_DRV_I2C_DeInit);
EXPORT_SYMBOL(HI_DRV_I2C_WriteConfig);
EXPORT_SYMBOL(HI_DRV_I2C_Write);
EXPORT_SYMBOL(HI_DRV_I2C_Read);
EXPORT_SYMBOL(HI_DRV_I2C_Read_SiLabs);
EXPORT_SYMBOL(HI_DRV_I2C_Read_sony);
EXPORT_SYMBOL(HI_DRV_I2C_Write_sony);
EXPORT_SYMBOL(HI_DRV_I2C_ReadDirectly);
EXPORT_SYMBOL(HI_DRV_I2C_Write_NoSTOP);
EXPORT_SYMBOL(HI_DRV_I2C_SetRate);
