/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_gpio.c
* Description:
*
* History:
* Version   Date                Author       DefectNum    Description
* main\1    2007-10-16      w54542      NULL           Create this file.
***********************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/printk.h>
#include <linux/gpio/driver.h>
#include <linux/list.h>
#include <linux/semaphore.h>

#include "hi_common.h"
#include "hi_error_mpi.h"
#include "drv_gpio_ioctl.h"
#include "hi_drv_sys.h"
#include "hi_module.h"
#include "hi_drv_reg.h"
#include "hi_drv_module.h"
#include "hi_reg_common.h"
#include "drv_gpio_ext.h"
#include "drv_gpio.h"

#define  GPIO_MAX_BUF 256
#define  GPIO_BUF_HEAD g_stGpioAttr.GpioIrqList[g_stGpioAttr.Head]
#define  GPIO_BUF_TAIL g_stGpioAttr.GpioIrqList[g_stGpioAttr.Tail]
#define  INC_BUF(x, len) (((x) + 1) % (len))

typedef struct
{
    HI_BOOL               GpioIntEnable;
    HI_UNF_GPIO_INTTYPE_E GpioIntType;
    HI_VOID (*GpioServer)(HI_U32 u32GpioNO);
} GPIO_INT_ATTR_S;

typedef struct
{
    /* gpio interrupt list */
    HI_U32            Head;
    HI_U32            Tail;
    HI_U32            GpioIrqList[GPIO_MAX_BUF];
    HI_U32            GpioIrqListSize;

    /* gpio wait queue */
    HI_U32            GpioWaitTimeout;
    wait_queue_head_t GpioIntWaitQueue;
    
    unsigned long            GpioRequest[GPIO_MAX_BUF];
    GPIO_INT_ATTR_S   GpioIrqAttr[GPIO_MAX_BUF];
    struct semaphore  mutex;
} GPIO_ATTR_S;

struct gpio_desc {
    struct gpio_chip    *chip;
    unsigned long       flags;
/* flag symbols are bit numbers */
#define FLAG_REQUESTED  0
#define FLAG_IS_OUT 1
#define FLAG_EXPORT 2   /* protected by sysfs_lock */
#define FLAG_SYSFS  3   /* exported via /sys/class/gpio/control */
#define FLAG_TRIG_FALL  4   /* trigger on falling edge */
#define FLAG_TRIG_RISE  5   /* trigger on rising edge */
#define FLAG_ACTIVE_LOW 6   /* value has active low */
#define FLAG_OPEN_DRAIN 7   /* Gpio is open drain type */
#define FLAG_OPEN_SOURCE 8  /* Gpio is open source type */
#define FLAG_USED_AS_IRQ 9  /* GPIO is connected to an IRQ */

#define ID_SHIFT    16  /* add new flags before this one */

#define GPIO_FLAGS_MASK     ((1 << ID_SHIFT) - 1)
#define GPIO_TRIGGER_MASK   (BIT(FLAG_TRIG_FALL) | BIT(FLAG_TRIG_RISE))

    const char      *label;
};

static GPIO_GET_GPIONUM_S g_GpioNum;
static atomic_t gpio_init_counter = ATOMIC_INIT(0);
static GPIO_ATTR_S g_stGpioAttr;
extern struct spinlock gpio_lock;
extern struct list_head gpio_chips;

HI_VOID DRV_GPIO_Dump(HI_VOID)
{
    HI_S32 i;
    pr_notice("request gpio:\n");
    for (i=0; i<GPIO_MAX_BUF; i++) {
        if (g_stGpioAttr.GpioRequest[i] != -1)
            printk(KERN_NOTICE " %4d", i);
    }
    
}

HI_BOOL DRV_GPIO_REQUEST(HI_U32 u32GpioNo, HI_SIZE_T priv)
{
    HI_S32 Ret=0;
 
    if (u32GpioNo >= GPIO_MAX_BUF) {
        HI_ERR_GPIO("requested gpio exceed GPIO_MAX_BUF(%d):%d\n",
            GPIO_MAX_BUF, u32GpioNo);
        return HI_FALSE;
    }

    if (atomic_read(&gpio_init_counter) <= 0) {
        HI_ERR_GPIO("gpio not initialized\n");
        return HI_FALSE;
    }

    Ret = down_interruptible(&g_stGpioAttr.mutex);
    if (Ret) {
        HI_ERR_GPIO("down_interruptible:%d\n", Ret);
        return HI_FALSE;
    }
 
    if (g_stGpioAttr.GpioRequest[u32GpioNo] != -1) {
        Ret = HI_TRUE;
        goto exit;
    }

    Ret = gpio_request(u32GpioNo, NULL);
    if (Ret < 0) {
        HI_ERR_GPIO("gpio%d request failed:%d\n", u32GpioNo, Ret);
        Ret = HI_FALSE;
        goto exit;
    }

    if (priv != -1)
        g_stGpioAttr.GpioRequest[u32GpioNo] = priv;
    else
        g_stGpioAttr.GpioRequest[u32GpioNo] = u32GpioNo;

    Ret = HI_TRUE;
    //DRV_GPIO_Dump();

exit:
    up(&g_stGpioAttr.mutex);
    return Ret;
}

HI_VOID DRV_GPIO_FREE(HI_U32 u32GpioNo)
{
    HI_S32 Ret;

    if (atomic_read(&gpio_init_counter) <= 0) {
        HI_ERR_GPIO("gpio not initialized\n");
        return;
    }

    Ret = down_interruptible(&g_stGpioAttr.mutex);
    if (Ret) {
        HI_ERR_GPIO("down_interruptible:%d\n", Ret);
        return;
    }

    if (g_stGpioAttr.GpioRequest[u32GpioNo] != -1) {

        if (g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable == HI_TRUE) {
            free_irq(gpio_to_irq(u32GpioNo), (HI_VOID *)((unsigned long)u32GpioNo));
            g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable = HI_FALSE;
        }

        gpio_free(u32GpioNo);
        g_stGpioAttr.GpioRequest[u32GpioNo] = -1;

    }
    up(&g_stGpioAttr.mutex);
}

HI_S32 HI_DRV_GPIO_PM_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_PRINT("GPIO suspend OK\n");
    return 0;
}

HI_S32 HI_DRV_GPIO_PM_Resume(PM_BASEDEV_S *pdev)
{
    HI_PRINT("GPIO resume OK\n");
    return 0;
}

static GPIO_EXT_FUNC_S g_stGpioExportFuncs =
{
    .pfnGpioDirGetBit            = HI_DRV_GPIO_GetDirBit,
    .pfnGpioDirSetBit            = HI_DRV_GPIO_SetDirBit,
    .pfnGpioReadBit              = HI_DRV_GPIO_ReadBit,
    .pfnGpioWriteBit             = HI_DRV_GPIO_WriteBit,
    .pfnGpioGetNum               = HI_DRV_GPIO_GetGpioNum,
    .pfnGpioRegisterServerFunc   = HI_DRV_GPIO_RegisterServerFunc,
    .pfnGpioUnRegisterServerFunc = HI_DRV_GPIO_UnregisterServerFunc,
    .pfnGpioSetIntType           = HI_DRV_GPIO_SetIntType,
    .pfnGpioSetIntEnable         = HI_DRV_GPIO_SetBitIntEnable,
    .pfnGpioClearGroupInt        = HI_DRV_GPIO_CearGroupInt,
    .pfnGpioClearBitInt          = HI_DRV_GPIO_ClearBitInt,
    .pfnGpioSuspend              = HI_DRV_GPIO_PM_Suspend,
    .pfnGpioResume               = HI_DRV_GPIO_PM_Resume
};

HI_S32 HI_DRV_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_U32 u32DirBit)
{
    HI_BOOL Ret=0; 

    if ((u32DirBit != HI_TRUE)
     && (u32DirBit != HI_FALSE)) {
        HI_ERR_GPIO("Invalid direction:%d\n", u32DirBit);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    Ret = DRV_GPIO_REQUEST(u32GpioNo, (HI_SIZE_T)u32GpioNo);
    if (Ret == HI_FALSE)
        return HI_ERR_GPIO_INVALID_OPT;

    if (!u32DirBit) /*0 output */
        Ret = gpio_direction_output(u32GpioNo, 0);
    else /*1 input */
        Ret = gpio_direction_input(u32GpioNo);
        
    if (Ret) {
        HI_ERR_GPIO("Set gpio%d direction %s failed:%d\n",
            u32GpioNo, u32DirBit?"input":"output", Ret);
        DRV_GPIO_FREE(u32GpioNo);
        return HI_ERR_GPIO_FAILED_SETDIRECT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_U32* pu32DirBit)
{
    HI_BOOL Ret = 0;
    HI_S32 status = -1;
    struct gpio_desc    *desc;

    if (!pu32DirBit) {
        HI_ERR_GPIO("pu32DirBit null\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    Ret = DRV_GPIO_REQUEST(u32GpioNo, (HI_SIZE_T)u32GpioNo);
    if (Ret == HI_FALSE)
        return HI_ERR_GPIO_INVALID_OPT;

    desc = gpio_to_desc(u32GpioNo);
    status = test_bit(FLAG_IS_OUT, &desc->flags)
                ? GPIOF_DIR_OUT : GPIOF_DIR_IN;

    if (status == GPIOF_DIR_IN)
        *pu32DirBit = HI_TRUE;
    else if (status == GPIOF_DIR_OUT)
        *pu32DirBit = HI_FALSE;
    else {
        HI_ERR_GPIO("Get gpio%d direction failed:%d\n",
            u32GpioNo, status);
        DRV_GPIO_FREE(u32GpioNo);
        return HI_ERR_GPIO_FAILED_GETDIRECT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_WriteBit(HI_U32 u32GpioNo, HI_U32 u32BitValue)
{
    HI_BOOL Ret = 0;
    HI_S32 status = -1;
    struct gpio_desc *desc;

    if ((u32BitValue != HI_TRUE)
     && (u32BitValue != HI_FALSE)) {
        HI_ERR_GPIO("Invalid WriteBit:%d\n", u32BitValue);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    Ret = DRV_GPIO_REQUEST(u32GpioNo, (HI_SIZE_T)u32GpioNo);
    if (Ret == HI_FALSE)
        return HI_ERR_GPIO_INVALID_OPT;

    desc = gpio_to_desc(u32GpioNo);
    status = test_bit(FLAG_IS_OUT, &desc->flags)
                ? GPIOF_DIR_OUT : GPIOF_DIR_IN;

    if (status != GPIOF_DIR_OUT) {
        HI_ERR_GPIO("Input direction, write denied\n");
        return HI_ERR_GPIO_INVALID_OPT;
    }

    gpio_set_value(u32GpioNo, u32BitValue);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_ReadBit(HI_U32 u32GpioNo, HI_U32* pu32BitValue)
{
    HI_BOOL Ret = 0;
    HI_S32 status = -1;
    struct gpio_desc *desc;

    if (!pu32BitValue) {
        HI_ERR_GPIO("pu32BitValue null\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    Ret = DRV_GPIO_REQUEST(u32GpioNo, (HI_SIZE_T)u32GpioNo);
    if (Ret == HI_FALSE)
        return HI_ERR_GPIO_INVALID_OPT;

    desc = gpio_to_desc(u32GpioNo);
    status = test_bit(FLAG_IS_OUT, &desc->flags)
                ? GPIOF_DIR_OUT : GPIOF_DIR_IN;

    if (status != GPIOF_DIR_IN) {
        HI_ERR_GPIO("Output direction, read denied\n");
        return HI_ERR_GPIO_INVALID_OPT;
    }

    *pu32BitValue = gpio_get_value(u32GpioNo);

    return HI_SUCCESS;
}

/*
u32GpioGroup: gpio group number
u32BitX:      gpio bit offset
enIntTriMode: interruput type ,please see HI_UNF_GPIO_INTTYPE_E
 */
HI_S32 HI_DRV_GPIO_SetIntType(HI_U32 u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntTriMode)
{
    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum) {
        HI_INFO_GPIO("Invalid parameter, u32GpioNum:%d\n", u32GpioNo);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (enIntTriMode >= HI_UNF_GPIO_INTTYPE_BUTT) {
        HI_INFO_GPIO("Invalid parameter, enIntTriMode:%d\n", enIntTriMode);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntType = enIntTriMode;

    return HI_SUCCESS;
}

static irqreturn_t GPIO_Isr(HI_S32 irq, HI_VOID * gpioNum)
{
    HI_SIZE_T u32GpioNum;
    u32GpioNum = (HI_SIZE_T)gpioNum;

    if (HI_TRUE == g_stGpioAttr.GpioIrqAttr[u32GpioNum].GpioIntEnable) {

        if (HI_NULL != g_stGpioAttr.GpioIrqAttr[u32GpioNum].GpioServer) {
            g_stGpioAttr.GpioIrqAttr[u32GpioNum].GpioServer(u32GpioNum);
            HI_INFO_GPIO("gpio[%d].GpioServer executed\n", u32GpioNum);
        }

        HI_INFO_GPIO("Recive gpio interrupt: GPIO%d. \n", u32GpioNum);

        GPIO_BUF_HEAD = u32GpioNum;

        g_stGpioAttr.Head = INC_BUF(g_stGpioAttr.Head, g_stGpioAttr.GpioIrqListSize);

        wake_up_interruptible(&(g_stGpioAttr.GpioIntWaitQueue));
    }

    return IRQ_HANDLED;
}

HI_S32 HI_DRV_GPIO_SetBitIntEnable(HI_U32 u32GpioNo, HI_BOOL bEnable)
{
    HI_U32 irq;
    HI_S32 flags = 0, irqType;
    HI_BOOL Ret;

    Ret = DRV_GPIO_REQUEST(u32GpioNo, (HI_SIZE_T)u32GpioNo);
        if (Ret == HI_FALSE) 
            return HI_ERR_GPIO_INVALID_OPT;

    if (bEnable) {   /* not marsk  */

        gpio_direction_input(u32GpioNo);
        irq = gpio_to_irq(u32GpioNo);

        irqType = g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntType;

        switch (irqType)
        {
        case HI_UNF_GPIO_INTTYPE_UP:
            flags |= IRQF_TRIGGER_RISING;
            break;

        case HI_UNF_GPIO_INTTYPE_DOWN:
            flags |= IRQF_TRIGGER_FALLING;
            break;

        case HI_UNF_GPIO_INTTYPE_UPDOWN:
            flags |= IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING;
            break;

        /* IRQF_TRIGGER_HIGH or IRQF_TRIGGER_LOW will trigger
           continuously interrupts, must avoid it */
        case HI_UNF_GPIO_INTTYPE_HIGH:
            flags |= IRQF_TRIGGER_HIGH;
            break;

        case HI_UNF_GPIO_INTTYPE_LOW:
            flags |= IRQF_TRIGGER_LOW;
            break;

        default:
            flags = IRQF_TRIGGER_RISING;
            break;
        }

        Ret = request_irq(irq, GPIO_Isr, flags, "gpio", (HI_VOID *)((unsigned long)u32GpioNo));
        if (Ret) {
            HI_ERR_GPIO("Request_irq irq%d on gpio%d failed: %d\n",
                irq, u32GpioNo, Ret);
            DRV_GPIO_FREE(u32GpioNo);
            g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable = HI_FALSE;
            return HI_ERR_GPIO_FAILED_SETINT;
        }

        g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable = HI_TRUE;

    } else {           /*  marsk  */

        if (g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable == HI_TRUE) {
            free_irq(gpio_to_irq(u32GpioNo), (HI_VOID *)((unsigned long)u32GpioNo));
            g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioIntEnable = HI_FALSE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_RegisterServerFunc(HI_U32 u32GpioNo, HI_VOID (*func)(HI_U32))
{
    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum) {
        HI_INFO_GPIO("Invalid parameter, u32GpioNum:%d\n", u32GpioNo);
        return HI_FAILURE;
    }

    if (HI_NULL == func) {
        HI_INFO_GPIO("Register func para is null, u32GpioNum%d \n", u32GpioNo);
        return HI_FAILURE;
    }

    if (HI_NULL != g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioServer) {
        HI_INFO_GPIO("GPIO %d had registered gpio server pragram \n", u32GpioNo);
        return HI_FAILURE;
    }

    g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioServer = func;
    HI_INFO_GPIO("Gpio %d finished register gpio server function \n", u32GpioNo);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_UnregisterServerFunc(HI_U32 u32GpioNo)
{
    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum) {
        HI_INFO_GPIO("Invalid parameter, u32GpioNum:%d\n", u32GpioNo);
        return HI_FAILURE;
    }

    g_stGpioAttr.GpioIrqAttr[u32GpioNo].GpioServer = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_ClearBitInt(HI_U32 u32GpioNo)
{
    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum) {
        HI_INFO_GPIO("Invalid parameter, u32GpioNum:%d\n", u32GpioNo);
        return HI_ERR_GPIO_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_CearGroupInt(HI_U32 u32GpioGroup)
{
    if (u32GpioGroup >= g_GpioNum.u8GpioGrpNum) {
        HI_ERR_GPIO("Invalid parameter, u32GpioGroup:%u\n", u32GpioGroup);
        return HI_ERR_GPIO_INVALID_PARA;
    }

#if  defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
    if ((u32GpioGroup==0) || ((u32GpioGroup>=15) && (u32GpioGroup<=17))) {
        HI_ERR_GPIO("Invalid parameter, u32GpioGroup:%u\n", u32GpioGroup);
        return HI_ERR_GPIO_INVALID_PARA;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_DRV_GPIO_GetGpioNum(GPIO_GET_GPIONUM_S* GpioNum)
{
    unsigned long	flags;
    struct gpio_chip *chip;

    if (HI_NULL == GpioNum) {
        HI_ERR_GPIO("GpioNum null\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    GpioNum->u8GpioGrpNum = 0;
    GpioNum->u8GpioMaxNum = 0;

    spin_lock_irqsave(&gpio_lock, flags);
    list_for_each_entry(chip, &gpio_chips, list) {
        GpioNum->u8GpioGrpNum ++;
        GpioNum->u8GpioMaxNum += chip->ngpio;
    }
	spin_unlock_irqrestore(&gpio_lock, flags);

    return HI_SUCCESS;
}

HI_S32 DRV_GPIO_Open(struct inode *inode, struct file *filp)
{
    return HI_SUCCESS;
}

HI_S32 DRV_GPIO_Close(struct inode *inode, struct file *filp)
{
    HI_S32 Ret, i;

    if (atomic_read(&gpio_init_counter) <= 0) {
        HI_ERR_GPIO("gpio not initialized\n");
        return HI_FAILURE;
    }

    Ret = down_interruptible(&g_stGpioAttr.mutex);
    if (Ret) {
        HI_ERR_GPIO("down_interruptible:%d\n", Ret);
        return HI_FAILURE;
    }

    for (i=0; i < GPIO_MAX_BUF; i++) {
        if (g_stGpioAttr.GpioRequest[i] == (HI_SIZE_T)filp) {
            if (g_stGpioAttr.GpioIrqAttr[i].GpioIntEnable == HI_TRUE) {
                free_irq(gpio_to_irq(i), (HI_VOID *)((unsigned long)i));
                g_stGpioAttr.GpioIrqAttr[i].GpioIntEnable = HI_FALSE;
            }

            gpio_free(i);
            g_stGpioAttr.GpioRequest[i] = -1;
        }
    }

    //DRV_GPIO_Dump();
    up(&g_stGpioAttr.mutex);

    return HI_SUCCESS;
}


HI_S32 DRV_GPIO_QueryInt (GPIO_INT_S * pGpioIntValue)
{
    HI_S32 Ret = 0;
    HI_U32 GpioIntNum;

    g_stGpioAttr.GpioWaitTimeout = pGpioIntValue->u32TimeoutMs;

    while (g_stGpioAttr.Head == g_stGpioAttr.Tail) {

        if (0xffffffff == g_stGpioAttr.GpioWaitTimeout) {
            Ret = wait_event_interruptible(g_stGpioAttr.GpioIntWaitQueue, 
                (g_stGpioAttr.Head != g_stGpioAttr.Tail));

            if (Ret < 0) {
                HI_INFO_GPIO("Wait_event_interruptible failed:%d\n", Ret);
                return -ERESTARTSYS;
            }

        } else {
            Ret = wait_event_interruptible_timeout(g_stGpioAttr.GpioIntWaitQueue,
                (g_stGpioAttr.Head != g_stGpioAttr.Tail),
                (long)(g_stGpioAttr.GpioWaitTimeout * HZ / 1000));

            if (Ret < 0) {
                HI_INFO_GPIO("Wait_event_interruptible_timeout failed:%d\n",
                     Ret);
                return -ERESTARTSYS;

            } else if (0 == Ret) {
                HI_INFO_GPIO("Wait_event_interruptible_timeout timrout\n");
                return HI_ERR_GPIO_GETINT_TIMEOUT;
            }
        }
    }

    if (g_stGpioAttr.Head != g_stGpioAttr.Tail) {
        GpioIntNum = GPIO_BUF_TAIL;
        g_stGpioAttr.Tail = INC_BUF(g_stGpioAttr.Tail, g_stGpioAttr.GpioIrqListSize);

        pGpioIntValue->u32GpioNo = GpioIntNum;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
        || defined(CHIP_TYPE_hi3716mv400)   \
        || defined(CHIP_TYPE_hi3718cv100)   \
        || defined(CHIP_TYPE_hi3719cv100)   \
        || defined(CHIP_TYPE_hi3718mv100)   \
        || defined(CHIP_TYPE_hi3719mv100)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo > 5 * 8 + 7)
        || (u32GpioNo == 5 * 8 + 5)
        || (u32GpioNo == 5 * 8 + 6))
    {
        return HI_ERR_GPIO_NOT_SUPPORT;
    }
    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;

#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo > 5 * 8 + 7)
        || (u32GpioNo == 5 * 8 + 3)
        || (u32GpioNo == 5 * 8 + 4))
    {
        return HI_ERR_GPIO_NOT_SUPPORT;
    }

    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || \
defined(CHIP_TYPE_hi3798cv200_a)|| defined(CHIP_TYPE_hi3798cv200_b)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo >= 5 * 8 + 7))
    {
            return HI_ERR_GPIO_NOT_SUPPORT;
    }

    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;
    u32Value &= ~(1 << u32Bit);
    u32Value |= enOutputType << u32Bit;

    g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32 = u32Value;

    return HI_SUCCESS;

    //FIXME hi3798cv100 no such register SC_GPIO_OD_CTRL
#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100) || \
	defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
    return HI_SUCCESS;
#else
    return HI_ERR_GPIO_NOT_SUPPORT;
#endif
}

HI_S32 DRV_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType)
{

#if    defined(CHIP_TYPE_hi3716cv200)   \
            || defined(CHIP_TYPE_hi3716mv400)   \
            || defined(CHIP_TYPE_hi3718cv100)   \
            || defined(CHIP_TYPE_hi3719cv100)   \
            || defined(CHIP_TYPE_hi3718mv100)   \
            || defined(CHIP_TYPE_hi3719mv100)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo > 5 * 8 + 7)
        || (u32GpioNo == 5 * 8 + 5)
        || (u32GpioNo == 5 * 8 + 6))
    {
        return HI_ERR_GPIO_NOT_SUPPORT;
    }

    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;

#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo > 5 * 8 + 7)
        || (u32GpioNo == 5 * 8 + 3)
        || (u32GpioNo == 5 * 8 + 4))
    {
        return HI_ERR_GPIO_NOT_SUPPORT;
    }

    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || \
defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)

    HI_U32 u32Value = 0;
    HI_U32 u32Bit = 0;

    if ((u32GpioNo < 5 * 8 + 0)
        || (u32GpioNo >= 5 * 8 + 7))
    {
            return HI_ERR_GPIO_NOT_SUPPORT;
    }

    u32Value = g_pstRegSysCtrl->SC_GPIO_OD_CTRL.u32;

    u32Bit = u32GpioNo % 8;

    u32Value = (u32Value >> u32Bit) & 0x1;
    *penOutputType = (HI_UNF_GPIO_OUTPUTTYPE_E)u32Value;

    return HI_SUCCESS;

        //FIXME hi3798cv100 no such register SC_GPIO_OD_CTRL
#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100) || \
	defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)
        return HI_SUCCESS;
#else
    return HI_ERR_GPIO_NOT_SUPPORT;
#endif

}

HI_S32 HI_DRV_GPIO_Init(void)
{
    HI_S32 ret;
    HI_U32 i;

    if (atomic_inc_return(&gpio_init_counter) != 1) {
        HI_WARN_GPIO(" HI_DRV_GPIO already registered:%d\n",
            atomic_read(&gpio_init_counter));
        return HI_SUCCESS;
    }

    ret = HI_DRV_MODULE_Register(HI_ID_GPIO, "HI_GPIO",
        (HI_VOID *)&g_stGpioExportFuncs);
    if (HI_SUCCESS != ret) {
        HI_INFO_GPIO(" GPIO Module register failed: 0x%x\n", ret);
        return HI_FAILURE;
    }

    g_stGpioAttr.Head = 0;
    g_stGpioAttr.Tail = 0;
    g_stGpioAttr.GpioIrqListSize = GPIO_MAX_BUF;
    g_stGpioAttr.GpioWaitTimeout = 0xffffffff;

    sema_init(&g_stGpioAttr.mutex, 1);     

    ret = HI_DRV_GPIO_GetGpioNum(&g_GpioNum);

    for (i=0; i < GPIO_MAX_BUF; i++) {
        g_stGpioAttr.GpioRequest[i] = -1;
        g_stGpioAttr.GpioIrqAttr[i].GpioIntEnable = HI_FALSE;
        g_stGpioAttr.GpioIrqAttr[i].GpioIntType = HI_UNF_GPIO_INTTYPE_DOWN;
        g_stGpioAttr.GpioIrqAttr[i].GpioServer = HI_NULL;
    }

    init_waitqueue_head(&g_stGpioAttr.GpioIntWaitQueue);

    return HI_SUCCESS;
}

void  HI_DRV_GPIO_DeInit(void)
{
    HI_U32 i;
    HI_S32 Ret;

    if (!atomic_dec_and_test(&gpio_init_counter)) {
        HI_WARN_GPIO("HI_DRV_GPIO_DeInit counter:%d\n",
            atomic_read(&gpio_init_counter));
        return;
    }

    for (i=0; i < GPIO_MAX_BUF; i++) {
        if (g_stGpioAttr.GpioRequest[i] != -1) {
            if (g_stGpioAttr.GpioIrqAttr[i].GpioIntEnable == HI_TRUE) {
                free_irq(gpio_to_irq(i), (HI_VOID *)((unsigned long)i));
                g_stGpioAttr.GpioIrqAttr[i].GpioIntEnable = HI_FALSE;
            }
            g_stGpioAttr.GpioIrqAttr[i].GpioIntType = HI_UNF_GPIO_INTTYPE_DOWN;
            g_stGpioAttr.GpioIrqAttr[i].GpioServer = HI_NULL;

            gpio_free(i);
            g_stGpioAttr.GpioRequest[i] = -1;
        }
    }

    Ret = HI_DRV_MODULE_UnRegister(HI_ID_GPIO);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_GPIO("GPIO Module unregister failed 0x%x.\n", Ret);
    }

    return;
}

#ifdef MODULE
EXPORT_SYMBOL(HI_DRV_GPIO_Init);
EXPORT_SYMBOL(HI_DRV_GPIO_DeInit);
#endif

EXPORT_SYMBOL(HI_DRV_GPIO_GetDirBit);
EXPORT_SYMBOL(HI_DRV_GPIO_SetDirBit);
EXPORT_SYMBOL(HI_DRV_GPIO_WriteBit);
EXPORT_SYMBOL(HI_DRV_GPIO_ReadBit);
EXPORT_SYMBOL(HI_DRV_GPIO_GetGpioNum);
EXPORT_SYMBOL(HI_DRV_GPIO_RegisterServerFunc);
EXPORT_SYMBOL(HI_DRV_GPIO_UnregisterServerFunc);
EXPORT_SYMBOL(HI_DRV_GPIO_SetIntType);
EXPORT_SYMBOL(HI_DRV_GPIO_SetBitIntEnable);
EXPORT_SYMBOL(HI_DRV_GPIO_CearGroupInt);
EXPORT_SYMBOL(HI_DRV_GPIO_ClearBitInt);
