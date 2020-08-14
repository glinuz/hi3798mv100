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

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/amba/pl022.h>
#include <linux/gpio.h>
#include <linux/atomic.h>
#include <linux/compat.h>

#include "hi_drv_spi.h"
#include "drv_spi_ioctl.h"
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "hi_drv_mem.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_reg_common.h"

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#define SPI0_BASE   0xF8B1A000
#define SPI1_BASE   0xF8B1B000
#else
#define SPI0_BASE   0xF8B1A000
#define SPI1_BASE   0xF8007000
#endif

/* SPI register definition .*/
#define SPI0_SR               (SPI0_BASE + 0x0C)
#define SPI0_TXFIFOCR         (SPI0_BASE + 0x28)
#define SPI0_RXFIFOCR         (SPI0_BASE + 0x2C)
#define SPI1_SR               (SPI1_BASE + 0x0C)
#define SPI1_TXFIFOCR         (SPI1_BASE + 0x28)
#define SPI1_RXFIFOCR         (SPI1_BASE + 0x2C)

#if defined(CHIP_TYPE_hi3798mv100)     || \
    defined(CHIP_TYPE_hi3796mv100)
    #define SPI0_CSGPIO_NO     (6*8+0)
    #define SPI1_CSGPIO_NO     (-1)
#elif   defined(CHIP_TYPE_hi3716mv410) || \
    defined(CHIP_TYPE_hi3716mv420)     || \
    defined(CHIP_TYPE_hi3798cv200)     || \
    defined(CHIP_TYPE_hi3798hv100)     || \
    defined(CHIP_TYPE_hi3798cv200_a)   || \
    defined(CHIP_TYPE_hi3798cv200_b)
    #define SPI0_CSGPIO_NO     (1*8+4)
    #define SPI1_CSGPIO_NO     (-1)
#else
    #define SPI0_CSGPIO_NO     (2*8+3)
    #define SPI1_CSGPIO_NO     (2*8+4)
#endif

#define MAX_SPI_TRANSFER_SIZE  (4*1024*1024)

struct hi_spi_dev
{
    struct semaphore   lock;
    struct spi_device *spi_dev;
    struct spi_board_info chip;
    struct pl022_config_chip pl022_config_info;
    u8 bits_per_word;
};

static struct hi_spi_dev spi_dummy[MAX_SPI_CHANNEL];
static HI_U32 spi_gpiocs;
static atomic_t spi_init_counter = ATOMIC_INIT(0);

static int spi_double_cs(void) {
#if    defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)
    return 0;
#else
    return 1;
#endif
}

HI_VOID SPI_WRITE_REG(HI_U32 RegAddr, HI_S32 value)
{
    void * __iomem ioaddr = ioremap_nocache(RegAddr, 0x4);
    if(ioaddr == HI_NULL) {
        HI_ERR_SPI("spi w ioremap failed.\n");
        return;
    }

    writel(value, ioaddr);
    iounmap(ioaddr);
}

HI_VOID SPI_READ_REG(HI_U32 RegAddr, HI_S32 *value)
{
    void * __iomem ioaddr;

    if (!value) {
        HI_ERR_SPI("null pointer\n");
        return;
    }
    
    ioaddr = ioremap_nocache(RegAddr, 0x4);
    if(ioaddr == HI_NULL) {
        HI_ERR_SPI("spi r ioremap failed.\n");
        return;
    }

    *value = readl(ioaddr);
    iounmap(ioaddr);
}

HI_U8 HI_DRV_SPI_Get_CsCfg(HI_VOID)
{
    return spi_gpiocs?1:0;
}

HI_VOID HI_DRV_SPI_Set_CsCfg(HI_U8 gpioCs)
{
    spi_gpiocs = gpioCs?1:0;
}

HI_VOID HI_DRV_SPI_SetCs_Gpio(HI_U8 id)
{
    int error;
    int gpio_no;
    struct spi_device *spi_dev;

    if (!spi_gpiocs) {
        HI_ERR_SPI("HI_DRV_SPI_Set_CsCfg(1) firstly\n");
        return;
    }

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return;
    }

    spi_dev = spi_dummy[id].spi_dev;
    if (!spi_dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return;
    }

    gpio_no = (id==0)?SPI0_CSGPIO_NO:SPI1_CSGPIO_NO;
        
    if (gpio_no < 0) {
        HI_ERR_SPI("don't support double chipselect, id:%d, gpio:%d\n", id, gpio_no);
        return;
    }
        
    error = devm_gpio_request(&(spi_dev->dev), gpio_no, NULL);
    if (error < 0)
        HI_ERR_SPI("gpio%d request failed:%d\n", gpio_no, error);

    /* set gpio direction output and high level to disable chipselect */
    error = gpio_direction_output(gpio_no, 1);
    if (error)
        HI_ERR_SPI("Set gpio%d direction output failed:%d\n",
            gpio_no, error);
}

HI_VOID HI_DRV_SPI_SetCs_Level(HI_U8 id,HI_U32 level)
{
    int gpio_no;

    if (!spi_gpiocs) {
        return;
    }

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return;
    }

    gpio_no = (id == 0)?SPI0_CSGPIO_NO:SPI1_CSGPIO_NO;

    if (gpio_no < 0) {
        HI_ERR_SPI("don't support double chipselect, id:%d\n", id);
        return;
    }

    gpio_set_value(gpio_no, level?1:0);

}

/* Loop Model  for test
set = 0 :normal
set = 1:loopMod
*/
HI_VOID HI_DRV_SPI_SetLoop(HI_U8 id,HI_U8 set)
{
    int ret;
    struct spi_device *dev;
    struct spi_board_info *chip;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return;
    }

    chip = &spi_dummy[id].chip;
    if (!chip) {
        HI_ERR_SPI("spi chip not initialized\n", id);
        return;
    }

    if (set) {
        dev->mode |= SPI_LOOP;
        chip->mode |= SPI_LOOP;
    } else {
        dev->mode &= ~SPI_LOOP;
        chip->mode &= ~SPI_LOOP;
    }

    ret = spi_setup(dev);
    if (ret < 0) {
        HI_ERR_SPI("spi_setup %d error:%d\n", id, ret);
   }
}

HI_VOID HI_DRV_SPI_SetFIFO(HI_U8 id,HI_U8 intsize)
{
    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return;
    }

    if(id == 0){
        SPI_WRITE_REG(SPI0_RXFIFOCR,intsize<<3);
    } else if(id == 1) {
        if (spi_double_cs())
            SPI_WRITE_REG(SPI1_RXFIFOCR,intsize<<3);
    }
}

/*
 * set SPI frame form routine.
 *
 * @param framemode: see enum ssp_interface
 *
 * @param sphvalue: SPICLKOUT phase (0/1)
 * @param sp0: SSPCLKOUT voltage level (0/1)
 * @param datavalue: data bit
 * 0000: reserved    0001: reserved    0010: reserved    0011: 4bit data
 * 0100: 5bit data   0101: 6bit data   0110:7bit data    0111: 8bit data
 * 1000: 9bit data   1001: 10bit data  1010:11bit data   1011: 12bit data
 * 1100: 13bit data  1101: 14bit data  1110:15bit data   1111: 16bit data
 *
 * @return value: 0--success; -1--error.
 *
 */

HI_S32 HI_DRV_SPI_SetFrom(HI_U8 id,HI_U8 framemode,
    HI_U8 spo,HI_U8 sph,HI_U8 bits)
{
    int ret;
    struct pl022_config_chip *pl022;
    struct spi_device *dev;
    struct spi_board_info *chip;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    chip = &spi_dummy[id].chip;
    pl022 = &spi_dummy[id].pl022_config_info;

    if (!chip || !pl022) {
        HI_ERR_SPI("spi chip or pl022_config_info not initialized\n", id);
        return -1;
    }

    pl022->iface = framemode;

    if (spo) {
        dev->mode |= SPI_CPOL;
        chip->mode |= SPI_CPOL;
    } else {
        dev->mode &= ~SPI_CPOL;
        chip->mode &= ~SPI_CPOL;
    }

    if (sph) {
        dev->mode |= SPI_CPHA;
        chip->mode |= SPI_CPHA;
    } else {
        dev->mode &= ~SPI_CPHA;
        chip->mode &= ~SPI_CPHA;
    }

    dev->bits_per_word = bits;
    spi_dummy[id].bits_per_word = bits;

    ret = spi_setup(dev);
    if (ret < 0) {
        HI_ERR_SPI("spi_setup error:%d, id:%d, sph:%d, spo:%d, bits:%d, framemode:%d\n",
            ret, id, sph, spo, bits, framemode);
       return ret;
   }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SPI_GetFrom(HI_U8 id,HI_U8 *framemode,HI_U8 *spo,
    HI_U8 *sph,HI_U8 *datawidth)
{
    struct spi_device *dev;
    struct pl022_config_chip *pl022;
    struct spi_board_info *chip;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    if (!framemode || !spo || !sph || !datawidth) {
        HI_ERR_SPI("null pointers\n");
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    chip = &spi_dummy[id].chip;
    pl022 = &spi_dummy[id].pl022_config_info;

    if (!chip || !pl022) {
        HI_ERR_SPI("spi chip or pl022_config_info not initialized\n", id);
        return -1;
    }

    *framemode = pl022->iface;
    *spo = (chip->mode & SPI_CPOL)?1:0;
    *sph = (chip->mode & SPI_CPHA)?1:0;
    *datawidth = dev->bits_per_word;

    HI_INFO_SPI("framemode=%d spo=%d sph=%d datawidth=%d\n",
        *framemode,*spo,*sph,*datawidth);

    return HI_SUCCESS;
}

/*
 * set SPI serial clock rate routine.
 *
 * @param scr: scr value.(0-255,usually it is 0)
 * @param cpsdvsr: Clock prescale divisor.(2-254 even)
 *
 * @return value: 0--success; -1--error.
 *
 */
HI_S32 HI_DRV_SPI_SetClk(HI_U8 id,HI_U8 scr,HI_U8 cpsdvsr)
{
    int ret;
    struct pl022_config_chip *pl022;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    pl022 = &spi_dummy[id].pl022_config_info;
    if (!pl022) {
        HI_ERR_SPI("spi pl022_config_info not initialized\n", id);
        return -1;
    }

    if (spi_dummy[id].spi_dev->controller_data != pl022) {
        HI_ERR_SPI("controller_data error\n");
        return -1;
    }

    pl022->clk_freq.cpsdvsr = cpsdvsr;
    pl022->clk_freq.scr = scr;

    ret = spi_setup(spi_dummy[id].spi_dev);
    if (ret < 0) {
        HI_ERR_SPI("channel %d spi_setup error:%d\n", id, ret);
        return ret;
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_SPI_SetBlend(HI_U8 id,HI_U8 bBigEnd)
{
    int ret;
    struct spi_device *dev;
    struct spi_board_info *chip;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return;
    }

    chip = &spi_dummy[id].chip;
    if (!chip) {
        HI_ERR_SPI("spi chip not initialized\n", id);
        return;
    }

    if (!bBigEnd) {
        dev->mode |= SPI_LSB_FIRST;
        chip->mode |= SPI_LSB_FIRST;
    } else {
        dev->mode &= ~SPI_LSB_FIRST;
        chip->mode &= ~SPI_LSB_FIRST;
    }

    ret = spi_setup(spi_dummy[id].spi_dev);
    if (ret < 0)
        HI_ERR_SPI("spi_setup %d error:%d\n", id, ret);
}

HI_U8 HI_DRV_SPI_GetBlend(HI_U8 id)
{
    struct spi_device *dev;
    struct spi_board_info *chip;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    chip = &spi_dummy[id].chip;
    return (chip && (chip->mode & SPI_LSB_FIRST))?0:1;
}

HI_U32 HI_DRV_SPI_FifoEmpty(HI_U8 id,HI_S32 bSend)
{
    HI_S32 ret = 0;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    if(id == 0) {
        SPI_READ_REG(SPI0_SR,&ret);
    } else if(id==1) {
        if (spi_double_cs())
            SPI_READ_REG(SPI1_SR,&ret);
        else
            return 2;
    } else {
        HI_ERR_SPI("para1 error!\n");
    }

    if (bSend) {
        return (ret & 0x1)?1:0; /* send fifo */
    } else {
        return (ret & 0x4)?0:1; /* receive fifo */
    }
}

/*
 *  write SPI_DR register rountine.
 *  @param  sdata: data of SSP_DR register
 */
HI_S32 HI_DRV_SPI_WriteQuery(HI_U8 id, HI_U8 *send, HI_U32 send_sz)
{
    int error = 0;
    struct spi_message   msg;
    struct spi_transfer  xfer;
    struct spi_device *dev;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    spi_message_init(&msg);
    memset(&xfer, 0, sizeof(struct spi_transfer));

    xfer.tx_buf    = send;
    xfer.rx_buf    = NULL;
    xfer.len       = send_sz;
    //xfer.cs_change = 0;
    spi_message_add_tail(&xfer, &msg);
    HI_DRV_SPI_SetCs_Level(id, 0);

    error = spi_sync(dev, &msg);
    if (error) {
        HI_ERR_SPI("spi_sync failed:%d\n", error);
    }
    HI_DRV_SPI_SetCs_Level(id, 1);

    return error;
}

HI_S32 HI_DRV_SPI_WriteIsr(HI_U8 devId,HI_U8 *Send, HI_U32 SendCnt)
{
    return HI_DRV_SPI_WriteQuery(devId, Send, SendCnt);
}

HI_S32 HI_DRV_SPI_ReadQuery(HI_U8 id, HI_U8 *read, HI_U32 read_sz)
{
    int error = 0;
    struct spi_message   msg;
    struct spi_transfer  xfer;
    struct spi_device *dev;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    spi_message_init(&msg);
    memset(&xfer, 0, sizeof(struct spi_transfer));

    /* Read zero byte and event code */
    xfer.tx_buf    = NULL;
    xfer.rx_buf    = read;
    xfer.len       = read_sz;
    //xfer.cs_change = 0;
    spi_message_add_tail(&xfer, &msg);

    HI_DRV_SPI_SetCs_Level(id, 0);
    error = spi_sync(dev, &msg);
    if (error) {
        HI_ERR_SPI("spi_sync failed:%d\n", error);
    }
    HI_DRV_SPI_SetCs_Level(id, 1);
    return error;
}

HI_S32 HI_DRV_SPI_ReadIsr(HI_U8 devId, HI_U8 *Read, HI_U32 ReadCnt)
{
    return HI_DRV_SPI_ReadQuery(devId, Read, ReadCnt);
}

/*
 *  read SPI_DR register rountine.
 *
 *  @return value: data from SSP_DR register readed
 *
 */
HI_S32 HI_DRV_SPI_ReadEx(HI_U8 id, HI_U8 *send, HI_U32 send_sz,
    HI_U8 *read, HI_U32 read_sz)
{
    int error = 0;
    struct spi_message   msg;
    struct spi_transfer  xfer[2];
    struct spi_device *dev;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    spi_message_init(&msg);
    memset(xfer, 0, sizeof(xfer));

    xfer[0].tx_buf    = send;
    xfer[0].rx_buf    = NULL;
    xfer[0].len       = send_sz;
    //xfer[0].cs_change = 0;
    spi_message_add_tail(&xfer[0], &msg);

    /* Read zero byte and event code */
    xfer[1].tx_buf    = NULL;
    xfer[1].rx_buf    = read;
    xfer[1].len       = read_sz;
    //xfer[1].cs_change = 0;
    spi_message_add_tail(&xfer[1], &msg);

    HI_DRV_SPI_SetCs_Level(id, 0);
    error = spi_sync(dev, &msg);
    if (error) {
        HI_ERR_SPI("spi_sync failed:%d\n", error);
    }
    HI_DRV_SPI_SetCs_Level(id, 1);
    return error;
}

HI_S32 HI_DRV_SPI_ReadEx_Isr(HI_U8 devId, HI_U8 *Send, HI_U32 SendCnt,
    HI_U8 *Read, HI_U32 ReadCnt)
{
    return HI_DRV_SPI_ReadEx(devId, Send, SendCnt, Read, ReadCnt);
}

HI_S32 HI_DRV_SPI_RW_LOOP(HI_U8 id, HI_U8 *send, HI_U32 send_sz,
    HI_U8 *read, HI_U32 read_sz)
{
    int error = 0;
    struct spi_message   msg;
    struct spi_transfer  xfer;
    struct spi_device *dev;

    if (id >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", id);
        return -1;
    }

    dev = spi_dummy[id].spi_dev;
    if (!dev) {
        HI_ERR_SPI("open spi channel %d firstly\n", id);
        return -1;
    }

    if (send_sz > read_sz) {
        HI_ERR_SPI("invalid args, send_sz=%d, read_sz=%d\n", send_sz, read_sz);
        return -1;
    }

    spi_message_init(&msg);
    memset(&xfer, 0, sizeof(xfer));

    xfer.tx_buf    = send;
    xfer.rx_buf    = read;
    xfer.len       = send_sz;
    spi_message_add_tail(&xfer, &msg);

    HI_DRV_SPI_SetCs_Level(id, 0);
    error = spi_sync(dev, &msg);
    if (error) {
        HI_ERR_SPI("spi_sync failed:%d\n", error);
    }
    HI_DRV_SPI_SetCs_Level(id, 1);
    return error;
}

HI_S32 HI_DRV_SPI_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg)
{
    HI_S32 error = 0;
    HI_U8  *pData = NULL;
    HI_U8  *rData = NULL;
    SPI_DATA_S SSPData;
    SPI_DATAEX_S EXData;
    SPI_FFORM_S SSPFForm;
    SPI_BLEND_S SSPBLEND;
    SPI_LOOP_S  SSPLOOP;
    unsigned long *pDevId;

    switch (cmd) {
    case CMD_SPI_OPEN:
        
        error = HI_DRV_SPI_Open(arg);
        if (error)
            HI_ERR_SPI("spi open failed:%d\n", error);
        else {
            pDevId = file->private_data;
            if (pDevId && arg < MAX_SPI_CHANNEL)
                pDevId[arg] = 1;
        }
        break;

    case CMD_SPI_CLOSE:
        error = HI_DRV_SPI_Close(arg);
        if (error)
            HI_ERR_SPI("spi close failed:%d\n", error);
        else {
            pDevId = file->private_data;
            if (pDevId && arg < MAX_SPI_CHANNEL)
                pDevId[arg] = 0;
        }
        break;

    case CMD_SPI_WRITE:
        if (copy_from_user(&SSPData, (void __user *)arg, sizeof(SPI_DATA_S))) {
            HI_ERR_SPI("ssp write copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if (SSPData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI, SSPData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp write kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        memset(pData,0,SSPData.sDataCnt);

        if (copy_from_user(pData, SSPData.sData, SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp write copy data from user fail\n");
            HI_KFREE(HI_ID_SPI, pData);
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if(HI_DRV_SPI_WriteQuery(SSPData.devId,pData,SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp write timeout\n");
            error = HI_ERR_SPI_WRITE_TIMEOUT;
        }

        HI_KFREE(HI_ID_SPI,pData);
        break;

    case CMD_SPI_READ:
        if (copy_from_user(&SSPData, (void __user *)arg, sizeof(SPI_DATA_S))) {
            HI_INFO_SPI("ssp read copy data from user fail!\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if (SSPData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,SSPData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp read ssp kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }
        memset(pData,0,SSPData.sDataCnt);

        if(HI_DRV_SPI_ReadQuery(SSPData.devId,pData,SSPData.sDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            break;
        }

        if (copy_to_user(SSPData.sData, pData, SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp read copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        break;

    case CMD_SPI_READEX:
        if (copy_from_user(&EXData, (void __user *)arg, sizeof(SPI_DATAEX_S))) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if ((EXData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) ||
            (EXData.rDataCnt >= MAX_SPI_TRANSFER_SIZE)) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,EXData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp pData kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        rData = HI_KMALLOC(HI_ID_SPI,EXData.rDataCnt, GFP_KERNEL);
        if (!rData) {
            HI_ERR_SPI("ssp rData kmalloc fail\n");
            HI_KFREE(HI_ID_SPI, pData);
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        memset(pData,0,EXData.sDataCnt);
        memset(rData,0,EXData.rDataCnt);

        if (copy_from_user(pData, EXData.sData, EXData.sDataCnt)) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (HI_DRV_SPI_ReadEx(EXData.devId,pData,EXData.sDataCnt,
            rData,EXData.rDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (copy_to_user(EXData.rData, rData, EXData.rDataCnt)) {
            HI_ERR_SPI("ssp readex copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        HI_KFREE(HI_ID_SPI, rData);
        break;

    case  CMD_SPI_SET_ATTR:
        if (copy_from_user(&SSPFForm, (void __user *)arg, sizeof(SPI_FFORM_S))) {
            HI_ERR_SPI("ssp setattr copy data from user fail\n");
            error = HI_FAILURE;
            break;
        }

        if (SSPFForm.cscfg)
            HI_DRV_SPI_Set_CsCfg(1);
        else
            HI_DRV_SPI_Set_CsCfg(0);

        HI_DRV_SPI_SetCs_Gpio(SSPFForm.devId);

        error = HI_DRV_SPI_SetFrom(SSPFForm.devId, SSPFForm.mode,SSPFForm.spo,
            SSPFForm.sph,SSPFForm.dss);
        break;

    case  CMD_SPI_GET_ATTR:
        if (copy_from_user(&SSPFForm, (void __user *)arg, sizeof(SPI_FFORM_S))) {
            HI_ERR_SPI("ssp getattr copy data from user fail\n");
            error = HI_FAILURE;
            break;
        }

        SSPFForm.cscfg = HI_DRV_SPI_Get_CsCfg();

        error = HI_DRV_SPI_GetFrom(SSPFForm.devId,&(SSPFForm.mode),&(SSPFForm.spo),
           &(SSPFForm.sph),&(SSPFForm.dss));
        if (HI_SUCCESS == error) {
            if (copy_to_user((void __user *)arg, &SSPFForm,sizeof(SPI_FFORM_S))) {
                HI_ERR_SPI("ssp getattr copy data to user fail\n");
                error = HI_ERR_SPI_COPY_DATA_ERR;
            }
        } else {
            HI_ERR_SPI("ssp HI_DRV_SPI_GetFrom fail\n");
        }

        break;

    case CMD_SPI_SET_BLEND:
        if (copy_from_user(&SSPBLEND, (void __user *)arg, sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp set blend copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }
        HI_DRV_SPI_SetBlend(SSPBLEND.devId,SSPBLEND.setbend);
        break;

    case CMD_SPI_GET_BLEND:
        if (copy_from_user(&SSPBLEND, (void __user *)arg, sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp get blend copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        SSPBLEND.setbend = HI_DRV_SPI_GetBlend(SSPBLEND.devId);
        if (copy_to_user((void __user *)arg, &SSPBLEND,sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp get blend copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }
        break;

    case CMD_SPI_SET_LOOP:
        if (copy_from_user(&SSPLOOP, (void __user *)arg, sizeof(SPI_LOOP_S))) {
            HI_ERR_SPI("ssp get loop from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        HI_DRV_SPI_SetLoop(SSPLOOP.devId, SSPLOOP.setLoop);
        break;

    case CMD_SPI_RW_LOOP:
        if (copy_from_user(&EXData, (void __user *)arg, sizeof(SPI_DATAEX_S))) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if ((EXData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) ||
            (EXData.rDataCnt >= MAX_SPI_TRANSFER_SIZE)) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,EXData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp pData kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }
        
        rData = HI_KMALLOC(HI_ID_SPI,EXData.rDataCnt, GFP_KERNEL);
        if (!rData) {
            HI_ERR_SPI("ssp rData kmalloc fail!\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            break;
        }

        memset(pData,0,EXData.sDataCnt);
        memset(rData,0,EXData.rDataCnt);

        if (copy_from_user(pData,EXData.sData, EXData.sDataCnt)) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if(HI_DRV_SPI_RW_LOOP(EXData.devId,pData,EXData.sDataCnt,
            rData,EXData.rDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (copy_to_user(EXData.rData, rData, EXData.rDataCnt)) {
            HI_ERR_SPI("ssp readex copy data to user fail!\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        HI_KFREE(HI_ID_SPI, rData);
        break;
    default:
        return -ENOIOCTLCMD;
    }

    return error;
}

#ifdef CONFIG_COMPAT
HI_S32 HI_DRV_SPI_Compat_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg)
{
    HI_S32 error = 0;
    HI_U8  *pData = NULL;
    HI_U8  *rData = NULL;
    SPI_DATA_COMPAT_S SSPData;
    SPI_DATAEX_COMPAT_S EXData;
    SPI_FFORM_S SSPFForm;
    SPI_BLEND_S SSPBLEND;
    SPI_LOOP_S  SSPLOOP;
    unsigned long *pDevId;

    switch (cmd) {
    case CMD_SPI_OPEN:
        
        error = HI_DRV_SPI_Open(arg);
        if (error)
            HI_ERR_SPI("spi open failed:%d\n", error);
        else {
            pDevId = file->private_data;
            if (pDevId && arg < MAX_SPI_CHANNEL)
                pDevId[arg] = 1;
        }
        break;

    case CMD_SPI_CLOSE:
        error = HI_DRV_SPI_Close(arg);
        if (error)
            HI_ERR_SPI("spi close failed:%d\n", error);
        else {
            pDevId = file->private_data;
            if (pDevId && arg < MAX_SPI_CHANNEL)
                pDevId[arg] = 0;
        }
        break;

    case CMD_SPI_WRITE:
    {
        SPI_DATA_COMPAT_S __user *pSSPData = compat_ptr(arg);
        if (copy_from_user(&SSPData, pSSPData, sizeof(SPI_DATA_COMPAT_S))) {
            HI_ERR_SPI("ssp write copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if (SSPData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI, SSPData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp write kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        memset(pData,0,SSPData.sDataCnt);

        if (copy_from_user(pData, (HI_U8 *)compat_ptr(SSPData.sData), SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp write copy data from user fail\n");
            HI_KFREE(HI_ID_SPI, pData);
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if(HI_DRV_SPI_WriteQuery(SSPData.devId,pData,SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp write timeout\n");
            error = HI_ERR_SPI_WRITE_TIMEOUT;
        }

        HI_KFREE(HI_ID_SPI,pData);
        break;
    }
    case CMD_SPI_READ:
    {
        SPI_DATA_COMPAT_S __user *pSSPData = compat_ptr(arg);
        if (copy_from_user(&SSPData, pSSPData, sizeof(SPI_DATA_COMPAT_S))) {
            HI_INFO_SPI("ssp read copy data from user fail!\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if (SSPData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,SSPData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp read ssp kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }
        memset(pData,0,SSPData.sDataCnt);

        if(HI_DRV_SPI_ReadQuery(SSPData.devId,pData,SSPData.sDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            break;
        }

        if (copy_to_user((HI_U8 *)compat_ptr(SSPData.sData), pData, SSPData.sDataCnt)) {
            HI_ERR_SPI("ssp read copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        break;
    }
    case CMD_SPI_READEX:
    {
        SPI_DATAEX_COMPAT_S __user *pSSPDataEx = compat_ptr(arg);
        if (copy_from_user(&EXData, pSSPDataEx, sizeof(SPI_DATAEX_COMPAT_S))) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if ((EXData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) ||
            (EXData.rDataCnt >= MAX_SPI_TRANSFER_SIZE)) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,EXData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp pData kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        rData = HI_KMALLOC(HI_ID_SPI,EXData.rDataCnt, GFP_KERNEL);
        if (!rData) {
            HI_ERR_SPI("ssp rData kmalloc fail\n");
            HI_KFREE(HI_ID_SPI, pData);
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }

        memset(pData,0,EXData.sDataCnt);
        memset(rData,0,EXData.rDataCnt);

        if (copy_from_user(pData, (HI_U8 *)compat_ptr(EXData.sData), EXData.sDataCnt)) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (HI_DRV_SPI_ReadEx(EXData.devId,pData,EXData.sDataCnt,
            rData,EXData.rDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (copy_to_user((HI_U8 *)compat_ptr(EXData.rData), rData, EXData.rDataCnt)) {
            HI_ERR_SPI("ssp readex copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        HI_KFREE(HI_ID_SPI, rData);
        break;
    }
    case  CMD_SPI_SET_ATTR:
    {
        SPI_FFORM_S __user *pSSPFForm =  compat_ptr(arg);
        if (copy_from_user(&SSPFForm, pSSPFForm, sizeof(SPI_FFORM_S))) {
            HI_ERR_SPI("ssp setattr copy data from user fail\n");
            error = HI_FAILURE;
            break;
        }

        if (SSPFForm.cscfg)
            HI_DRV_SPI_Set_CsCfg(1);
        else
            HI_DRV_SPI_Set_CsCfg(0);

        HI_DRV_SPI_SetCs_Gpio(SSPFForm.devId);

        error = HI_DRV_SPI_SetFrom(SSPFForm.devId, SSPFForm.mode,SSPFForm.spo,
            SSPFForm.sph,SSPFForm.dss);
        break;
    }
    case  CMD_SPI_GET_ATTR:
    {
        SPI_FFORM_S __user *pSSPFForm =  compat_ptr(arg);
        if (copy_from_user(&SSPFForm, pSSPFForm, sizeof(SPI_FFORM_S))) {
            HI_ERR_SPI("ssp getattr copy data from user fail\n");
            error = HI_FAILURE;
            break;
        }

        SSPFForm.cscfg = HI_DRV_SPI_Get_CsCfg();

        error = HI_DRV_SPI_GetFrom(SSPFForm.devId,&(SSPFForm.mode),&(SSPFForm.spo),
           &(SSPFForm.sph),&(SSPFForm.dss));
        if (HI_SUCCESS == error) {
            if (copy_to_user(pSSPFForm, &SSPFForm,sizeof(SPI_FFORM_S))) {
                HI_ERR_SPI("ssp getattr copy data to user fail\n");
                error = HI_ERR_SPI_COPY_DATA_ERR;
            }
        } else {
            HI_ERR_SPI("ssp HI_DRV_SPI_GetFrom fail\n");
        }

        break;
    }
    case CMD_SPI_SET_BLEND:
    {
        SPI_BLEND_S __user *pSSPBLEND =  compat_ptr(arg);
        if (copy_from_user(&SSPBLEND, pSSPBLEND, sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp set blend copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }
        HI_DRV_SPI_SetBlend(SSPBLEND.devId,SSPBLEND.setbend);
        break;
    }
    case CMD_SPI_GET_BLEND:
    {
        SPI_BLEND_S __user *pSSPBLEND =  compat_ptr(arg);
        if (copy_from_user(&SSPBLEND, pSSPBLEND, sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp get blend copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        SSPBLEND.setbend = HI_DRV_SPI_GetBlend(SSPBLEND.devId);
        if (copy_to_user(pSSPBLEND, &SSPBLEND,sizeof(SPI_BLEND_S))) {
            HI_ERR_SPI("ssp get blend copy data to user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }
        break;
    }
    case CMD_SPI_SET_LOOP:
    {
        SPI_LOOP_S __user *pSSPLOOP =  compat_ptr(arg);
        if (copy_from_user(&SSPLOOP, pSSPLOOP, sizeof(SPI_LOOP_S))) {
            HI_ERR_SPI("ssp get loop from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        HI_DRV_SPI_SetLoop(SSPLOOP.devId, SSPLOOP.setLoop);
        break;
    }
    case CMD_SPI_RW_LOOP:
    {
        SPI_DATAEX_COMPAT_S __user *pEXData =  compat_ptr(arg);
        if (copy_from_user(&EXData, pEXData, sizeof(SPI_DATAEX_COMPAT_S))) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        if ((EXData.sDataCnt >= MAX_SPI_TRANSFER_SIZE) ||
            (EXData.rDataCnt >= MAX_SPI_TRANSFER_SIZE)) {
            HI_ERR_SPI("ssp transfer msg too large\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            break;
        }

        pData = HI_KMALLOC(HI_ID_SPI,EXData.sDataCnt, GFP_KERNEL);
        if (!pData) {
            HI_ERR_SPI("ssp pData kmalloc fail\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            break;
        }
        
        rData = HI_KMALLOC(HI_ID_SPI,EXData.rDataCnt, GFP_KERNEL);
        if (!rData) {
            HI_ERR_SPI("ssp rData kmalloc fail!\n");
            error = HI_ERR_SPI_MALLOC_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            break;
        }

        memset(pData,0,EXData.sDataCnt);
        memset(rData,0,EXData.rDataCnt);

        if (copy_from_user(pData,(HI_U8 *)compat_ptr(EXData.sData), EXData.sDataCnt)) {
            HI_ERR_SPI("ssp readex copy data from user fail\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if(HI_DRV_SPI_RW_LOOP(EXData.devId,pData,EXData.sDataCnt,
            rData,EXData.rDataCnt)) {
            error = HI_ERR_SPI_READ_TIMEOUT;
            HI_KFREE(HI_ID_SPI, pData);
            HI_KFREE(HI_ID_SPI, rData);
            break;
        }

        if (copy_to_user((HI_U8 *)compat_ptr(EXData.rData), rData, EXData.rDataCnt)) {
            HI_ERR_SPI("ssp readex copy data to user fail!\n");
            error = HI_ERR_SPI_COPY_DATA_ERR;
        }

        HI_KFREE(HI_ID_SPI, pData);
        HI_KFREE(HI_ID_SPI, rData);
        break;
    }
    default:
        return -ENOIOCTLCMD;
    }

    return error;
}
#endif

static unsigned int get_spi_dev_id(char *devname)
{
    unsigned int id;
    if (!devname) {
        HI_ERR_SPI("get_spi_dev_id error:%s\n", devname);
        return 0;
    }

    if (sscanf(devname, "hi_spi_%u", &id) != 1) {
        HI_ERR_SPI("get_spi_dev_id sscanf error:%s\n", devname);
        return 0;
    }

    return id;
}

static int hi_spi_probe(struct spi_device *spi)
{
    unsigned int id = get_spi_dev_id(spi->modalias);

    if (id >= MAX_SPI_CHANNEL || !spi) {
        HI_ERR_SPI("hi_spi_probe failed, id:%d\n", id);
        return -1;
    }

    if (down_interruptible(&spi_dummy[id].lock)){
        HI_ERR_SPI("hi_spi_probe lock failed\n");
        return -ERESTARTSYS;
    }

    spi_dummy[id].spi_dev= spi;
    up(&spi_dummy[id].lock);

    return 0;
}

static int hi_spi_remove(struct spi_device *spi)
{
    unsigned int id = get_spi_dev_id(spi->modalias);

    if (id >= MAX_SPI_CHANNEL || !spi) {
        HI_ERR_SPI("hi_spi_remove failed, id:%d\n", id);
        return -1;
    }
    
    if (down_interruptible(&spi_dummy[id].lock)) {
        HI_ERR_SPI("hi_spi_remove lock failed\n");
        return -ERESTARTSYS;
    }

    spi_dummy[id].spi_dev= NULL;
    up(&spi_dummy[id].lock);

    return 0;
}

static struct spi_device_id hi_spi_drv_ids[] = {
    { "hi_spi_0",   0 },
    { "hi_spi_1",   0 },
    {},
};

static struct spi_driver hi_spi_driver = {
    .driver        = {
        .name    = "hi_spi",
        .owner    = THIS_MODULE,
    },
    .id_table     = hi_spi_drv_ids,
    .probe        = hi_spi_probe,
    .remove       = hi_spi_remove,
};

HI_S32 HI_DRV_SPI_Open(HI_U8 devid)
{
    struct spi_master *spi_master;
    struct spi_device *spi_device;
    struct spi_board_info *chip;
    int error;

    if (devid >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", devid);
        return -1;
    }

    spi_master = spi_busnum_to_master(devid);
    if (!spi_master){
        HI_ERR_SPI("spi_busnum_to_master(%d) returned NULL\n", devid);
        return -ENODEV;
    }

    spi_master->rt = true;
    spi_device = spi_alloc_device(spi_master);
    if (!spi_device) {
        HI_ERR_SPI("spi_alloc_device returned NULL\n");
        error = -ENOMEM;
        goto spi_master_fail;
    }

    chip = &spi_dummy[devid].chip;
    spi_device->controller_data  = &spi_dummy[devid].pl022_config_info;
    spi_device->max_speed_hz     = chip->max_speed_hz;
    spi_device->chip_select      = chip->chip_select;
    spi_device->mode             = chip->mode;
    spi_device->irq              = chip->irq;
    spi_device->bits_per_word    = spi_dummy[devid].bits_per_word;
    spi_device->controller_state = NULL;

    snprintf(spi_device->modalias, SPI_NAME_SIZE, "hi_spi_%d", devid);

    error = spi_add_device(spi_device);
    if (error < 0) {
        HI_ERR_SPI("spi_add_device returned %d\n", error);
        goto spi_dev_fail;
    }

    spi_master_put(spi_master);

    return 0;

spi_dev_fail:
    spi_dev_put(spi_device);

spi_master_fail:
    spi_master_put(spi_master);

    return error;
}

HI_S32 HI_DRV_SPI_Close(HI_U8 devid)
{
    if (devid >= MAX_SPI_CHANNEL) {
        HI_ERR_SPI("spi channel %d not support\n", devid);
        return -1;
    }
    
    if (spi_dummy[devid].spi_dev)
        spi_unregister_device(spi_dummy[devid].spi_dev);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_SPI_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    int i, error;
    struct pl022_config_chip *pl022;
    struct spi_board_info *chip;

    if (atomic_inc_return(&spi_init_counter) != 1) {
        HI_WARN_SPI(" HI_DRV_SPI already registered:%d\n",
            atomic_read(&spi_init_counter));
        return HI_SUCCESS;
    }

    s32Ret = HI_DRV_MODULE_Register(HI_ID_SPI, "HI_SPI", HI_NULL);
    if (HI_SUCCESS != s32Ret) {
        HI_ERR_SPI("register failed 0x%x.\n", s32Ret);
        return HI_FAILURE;
    }

    error = spi_register_driver(&hi_spi_driver);
    if (error < 0) {
        HI_ERR_SPI("spi_register_driver returned %d\n", error);
        return error;
    }

    for (i=0; i < MAX_SPI_CHANNEL; i++) {
        sema_init(&spi_dummy[i].lock, 1);

        pl022 = &spi_dummy[i].pl022_config_info;
        pl022->com_mode = POLLING_TRANSFER;
        pl022->iface = SSP_INTERFACE_MOTOROLA_SPI;
        pl022->hierarchy = SSP_MASTER;
        pl022->slave_tx_disable = 0,
        pl022->rx_lev_trig = SSP_RX_1_OR_MORE_ELEM,
        pl022->tx_lev_trig = SSP_TX_1_OR_MORE_EMPTY_LOC,
        pl022->ctrl_len = SSP_BITS_8,
        pl022->wait_state = SSP_MWIRE_WAIT_ZERO,
        pl022->duplex = SSP_MICROWIRE_CHANNEL_FULL_DUPLEX,
        pl022->clk_freq.cpsdvsr = 0x4;
        pl022->clk_freq.scr = 0x1;

        chip = &spi_dummy[i].chip;
        chip->max_speed_hz = 16000000;
        chip->chip_select = i;
        chip->mode = SPI_MODE_0;
        chip->irq = -1;
        spi_dummy[i].bits_per_word = 8;
    }
    pr_info("HI_DRV_SPI_Init success\n");

    return s32Ret;
}

HI_S32 HI_DRV_SPI_DeInit(HI_VOID)
{
    int i;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!atomic_dec_and_test(&spi_init_counter)) {
        HI_WARN_SPI("HI_DRV_SPI_DeInit counter:%d\n",
            atomic_read(&spi_init_counter));
        return -1;
    }

    for (i=0; i < MAX_SPI_CHANNEL; i++) {
        if (spi_dummy[i].spi_dev)
            spi_unregister_device(spi_dummy[i].spi_dev);
    }

    spi_unregister_driver(&hi_spi_driver);

    s32Ret = HI_DRV_MODULE_UnRegister(HI_ID_SPI);
    if (HI_SUCCESS != s32Ret) {
        HI_ERR_SPI("SPI Module unregister failed 0x%x.\n", s32Ret);
        return HI_FAILURE;
    }

    pr_info("HI_DRV_SPI_DeInit success\n");
    return s32Ret;
}

EXPORT_SYMBOL(HI_DRV_SPI_Init);
EXPORT_SYMBOL(HI_DRV_SPI_DeInit);
EXPORT_SYMBOL(HI_DRV_SPI_Open);
EXPORT_SYMBOL(HI_DRV_SPI_Close);
EXPORT_SYMBOL(HI_DRV_SPI_SetBlend);
EXPORT_SYMBOL(HI_DRV_SPI_Get_CsCfg);
EXPORT_SYMBOL(HI_DRV_SPI_Set_CsCfg);
EXPORT_SYMBOL(HI_DRV_SPI_SetCs_Gpio);
EXPORT_SYMBOL(HI_DRV_SPI_SetCs_Level);
EXPORT_SYMBOL(HI_DRV_SPI_ReadEx);
EXPORT_SYMBOL(HI_DRV_SPI_ReadEx_Isr);
EXPORT_SYMBOL(HI_DRV_SPI_WriteIsr);
EXPORT_SYMBOL(HI_DRV_SPI_WriteQuery);
EXPORT_SYMBOL(HI_DRV_SPI_ReadQuery);
EXPORT_SYMBOL(HI_DRV_SPI_ReadIsr);
EXPORT_SYMBOL(HI_DRV_SPI_SetFrom);
EXPORT_SYMBOL(HI_DRV_SPI_SetClk);
