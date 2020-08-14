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

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

#include "hi_drv_struct.h"
#include "hi_drv_ir.h"
#include "drv_ir_ioctl.h"
#include "drv_ir_protocol.h"
#include "drv_ir_utils.h"
#include "drv_ir_register.h"

#include "hi_common.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#include "hi_osal.h"

#define IR_DEVICE_NAME UMAP_DEVNAME_IR
/*************************** public parameters ********************/
/* chip work mode, key or symbol. */
static u32 ir_work_mode = IR_MODE_SYMBOL; /* default mode symbol. */

static u32 ir_frequence = DFT_FREQ;
module_param_named(freq, ir_frequence, uint, S_IRUGO);

static u32 ir_keyhold_timeout = 300; /*ms*/
module_param_named(key_hold_timeout, ir_keyhold_timeout, uint, S_IRUGO);

/************************** symbol mode parameters ****************/
/* reserve the parameter names. */
static u32 ir_symbol_width = DFT_SYMBOL_WIDTH;
module_param_named(symbol_width, ir_symbol_width, uint, S_IRUGO);

static u32 ir_symbol_format = DFT_SYMBOL_FMT;
module_param_named(symbol_format, ir_symbol_format, uint, S_IRUGO);

static u32 ir_int_level = DFT_INT_LEVEL;
module_param_named(int_level, ir_int_level, uint, S_IRUGO);

/* for raw symbol read, specify this parameter. */
static u32 ir_key_fetch_raw = 0;
module_param_named(key_fetch, ir_key_fetch_raw, uint, S_IRUGO);

/**************************************************************************/
struct ir_priv ir_local;

static bool bMatched = 0;

u32 g_bTimeOutFlag = HI_FALSE;

extern spinlock_t irlock;

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);


static inline void ir_enable(struct ir_priv *ir)
{
    writel(0x1, ir->base + IR_EN);
}

static inline void ir_disable(struct ir_priv *ir)
{
    writel(0, ir->base + IR_EN);
}

static inline void ir_start(struct ir_priv *ir)
{
    writel(1, ir->base + IR_START);
}

static void ir_set_clk(bool bFlag)
{
    HI_U32 * pIRRest = HI_NULL_PTR;
    HI_U32 u32RegVal = 0;
    
    pIRRest = ioremap_nocache(0xf8000048, 0x4);
    if (HI_NULL_PTR == pIRRest)
    {
        hiir_error("ioremap_nocache IR Reset err! \n");
        return;
    }

    u32RegVal = readl(pIRRest);
    
    if (HI_TRUE == bFlag)
    {
        u32RegVal |= 0x10; //open clk
        u32RegVal &= 0xffffffdf;
    }
    else
    {
        u32RegVal |= 0x20; //close clk
        u32RegVal &= 0xffffffef;
    }

    writel(u32RegVal, pIRRest);
    
    iounmap(pIRRest);

    return;
}

#if 0
static inline void ir_stop(struct ir_priv *ir)
{
    writel(0, ir->base + IR_START);
}
#endif

static void ir_clear_fifo(struct ir_priv *ir)
{
    u32 cnt, v, i;

    cnt = readl(ir->base + IR_DATAH);
    for (i = 0; i < cnt; i++)
    {
        v = readl(ir->base + IR_DATAL);
    }
}

static int ir_config(struct ir_priv *ir)
{
    u32 val = 0;

    ir_enable(ir);

    while (((readl(ir->base + IR_BUSY)) & 0x1) && val++ < 1000)
    {
        (void)schedule_timeout(10);
    }

    if (val >= 1000)
    {
        hiir_info("ir module is busy!\n");
        return -EBUSY;
    }

    if (ir->chip_param.work_mode == IR_MODE_KEY)
    {
        return -EINVAL;
    }
    else
    {
        val = ((ir->chip_param.ir_max_symbol << 16)
               | (ir->chip_param.ir_format << 14)
               | (ir->chip_param.ir_max_bits << 8)
               | (ir->chip_param.work_mode << 7)
               | (ir->chip_param.ir_freq));
    }

    hiir_debug("IR work at %s mode, config value: 0x%08x\n",
               ir->chip_param.work_mode == IR_MODE_KEY ?
               "key" : "symbol",
               val);

    writel(val, ir->base + IR_CFG);
    bMatched = 0;
    return 0;
}

static int ir_symbol_proc(struct ir_priv *ir)
{
    struct ir_protocol *ip;
    struct key_attr *key;
    enum IR_MATCH_RESULT r;
    int matchs;
    unsigned long flag;

    /* 1. find out which is a infrared code match the head symbol.
     * 2. if no, parse from this head symbol.
     * 3. if yes, we should check the whole frame to get a
     * accurate match infrared code for this key.
     */
    key = ir_first_read_key(ir->symbol_buf);
    while (key && key->upper && key->lower)
    {
        hiir_debug("header key at(%d): key->lower:%lld,"
                   " key->upper:%lld",
                   ir->symbol_buf->reader,
                   key->lower, key->upper);

        spin_lock_irqsave(&irlock, flag);
        ip = ir_prot_first();
        matchs = 0;
        while (ir_prot_valid(ip))
        {
            if (!ip->match || ip->disabled)
            {
                ip = ir_prot_next(ip);
                continue;
            }

            /* try find header first! */
            r = ip->match(IR_MTT_HEADER, ir->symbol_buf, ip);
            if (IR_MATCH_MATCH != r)
            {
                if (IR_MATCH_NEED_MORE_DATA == r)
                {
                    matchs++;
                    ip = ir_prot_next(ip);
                    continue;
                }

                if (ip->flag & INFR_HAS_ERR_HANDLE)
                {
                    r = ip->match_error(IR_MTT_HEADER,
                                        ir->symbol_buf, ip);
                    if (r != IR_MATCH_MATCH)
                    {
                        if (IR_MATCH_NEED_MORE_DATA == r)
                        {
                            matchs++;
                        }

                        ip = ir_prot_next(ip);
                        continue;
                    }
                }
                else
                {
                    ip = ir_prot_next(ip);
                    continue;
                }
            }

            /* find a match. */
            r = ip->match(IR_MTT_FRAME, ir->symbol_buf, ip);

            if ((IR_MATCH_MATCH != r)
                && (ip->flag & INFR_HAS_ERR_HANDLE))
            {
                r = ip->match_error(IR_MTT_FRAME,
                                    ir->symbol_buf, ip);
            }

            if (IR_MATCH_MATCH == r)
            {
                bMatched = 1;
                mod_timer(&ir->symbol_timer,jiffies
                                   + msecs_to_jiffies(1));
                /* if match, parse this frame */
                r = ip->handle(ir, ip, ir->symbol_buf,
                               ir->key_buf);
                if (!r)
                {
                    wake_up_interruptible(&ir->read_wait);
                }

                if (r && (ip->flag & INFR_HAS_ERR_HANDLE))
                {
                    ip->parse_error(ir, ip,
                                    ir->symbol_buf, ir->key_buf);
                }
                spin_unlock_irqrestore(&irlock, flag);
                goto next_frame;
            }
            else if (IR_MATCH_NEED_MORE_DATA == r)
            {
                matchs++;
            }

            ip = ir_prot_next(ip);
        }
        spin_unlock_irqrestore(&irlock, flag);

        /* if no match, discard this symbol. */
        if (!matchs)
        {
            key = ir_next_reader_clr_inc(ir->symbol_buf);
            continue;
        }

        /* if some infrared code match, do not discard,
         * wait timer expires and try again.
         */
        if (!ir_prot_valid(ip))
        {
            return -1;
        }

next_frame:
        key = ir_first_read_key(ir->symbol_buf);
    }

    return 0;
}

static void ir_symbol_timer_proc(unsigned long data)
{
    struct ir_priv *ir = &ir_local;
    struct key_attr *key;
    struct ir_protocol *ip;
    enum IR_MATCH_RESULT r;
    static unsigned long deadline;
    unsigned long flag;

    if(bMatched)
    {
        bMatched = 0;
        return;
    }
    
    if (!ir_symbol_proc(ir))
    {
        goto out;
    }

    hiir_debug("Symbols cannot be parsed." \
               "Abandon them till frame header appear" \
               " or buffer empty!\n");

    /* Case symbol cannot be parsed, ignore symbols until
     * new frame start symbol appears or buffer empty.
     */
    key = ir_first_read_key(ir->symbol_buf);
    while (key && key->upper && key->lower)
    {
        spin_lock_irqsave(&irlock, flag);
        ip = ir_prot_first();
        while (ir_prot_valid(ip))
        {
            if (!ip->match || ip->disabled)
            {
                ip = ir_prot_next(ip);
                continue;
            }

            r = ip->match(IR_MTT_HEADER, ir->symbol_buf, ip);
            if (IR_MATCH_MATCH == r)
            {
                r = ip->match(IR_MTT_FRAME,
                              ir->symbol_buf, ip);
                if ((IR_MATCH_MATCH != r)
                    && (ip->flag & INFR_HAS_ERR_HANDLE))
                {
                    r = ip->match_error(IR_MTT_FRAME,
                                        ir->symbol_buf, ip);
                }

                if (IR_MATCH_MATCH == r)
                {
                    spin_unlock_irqrestore(&irlock, flag);
                    (void)ir_symbol_proc(ir);
                    deadline = 0;
                    return;
                }

                if (deadline && time_after_eq(jiffies,
                                              deadline))
                {

                    ip = ir_prot_next(ip);
                    continue;
                }
                else
                {
                    deadline = jiffies +
                               msecs_to_jiffies(
                        SYMBOL_PARSE_TIMEOUT);
                }
                /* A new frame start. Abort */
                ir->symbol_timer.expires = jiffies
                                           + msecs_to_jiffies(
                    SYMBOL_PARSE_TIMEOUT);
                mod_timer(&ir->symbol_timer,ir->symbol_timer.expires);
                 spin_unlock_irqrestore(&irlock, flag);
                goto out;
            }
            else if (IR_MATCH_NEED_MORE_DATA == r)
            {
                /* - For infrared code rc5, which header phase
                 * may constain two symbols,
                 * and the 2th is not appear yet,
                 * we will check after 200ms to see
                 * whether the 2th header key received
                 * or not. If still not received, discard
                 * the 1st key.
                 * - While a frame received not integrated,
                 * we need to conditional wait for a while,
                 * after timeout, and the frame still cannot
                 * be parsed, delete the frame.
                 *
                 * Another case is there are more infra code
                 * in protocol array, and some of them have
                 * more than one header bits.
                 * When the first loop of this timer returned
                 * needs more data, but take into account of
                 * 200ms after the first check,
                 * the follow symbol will be received,
                 * if not, we do discard the symbols
                 * for we have escaped 200ms!
                 */
                if (deadline && time_after_eq(jiffies,
                                              deadline))
                {
                    ip = ir_prot_next(ip);
                    continue;
                }
                else
                {
                    deadline = jiffies +
                               msecs_to_jiffies(
                        SYMBOL_PARSE_TIMEOUT);
                    ir->symbol_timer.expires = jiffies
                                               + msecs_to_jiffies(
                        SYMBOL_PARSE_TIMEOUT);
                    mod_timer(&ir->symbol_timer,ir->symbol_timer.expires);
                    spin_unlock_irqrestore(&irlock, flag);

                    goto out;
                }
            }

            ip = ir_prot_next(ip);
        }
         spin_unlock_irqrestore(&irlock, flag);

        /* Clear the header symbol first!
         * Think about there are symbols sequence like this:
         * header,data,but no burst.
         * We should destroy header first to avoid dead loop!
         */
        memset(key, 0, sizeof(*key));

        ir->symbol_buf->reader++;
        if (ir->symbol_buf->reader == MAX_SYMBOL_NUM)
        {
            ir->symbol_buf->reader = 0;
        }

        /* for we increased 'reader' cursor, so get next '0' */
        key = ir_get_read_key_n(ir->symbol_buf, 0);
    }

    /* All symbols are discarded, clear deadline,
     * no need to restart this timer.
     */
    deadline = 0;
out:
    return;
}

static void ir_bh_symbol(unsigned long data)
{
    struct ir_priv *ir = &ir_local;

    /* If frame cannot be parsed this time,
     * we will try again after symbol_timer expires.
     */
    if (ir_symbol_proc(&ir_local))
    {
        ir->symbol_timer.expires = jiffies
                                   + msecs_to_jiffies(SYMBOL_PARSE_TIMEOUT);
        mod_timer(&ir->symbol_timer,ir->symbol_timer.expires);
    }

    return;
}

static irqreturn_t ir_isr_symbol(int irq, void *dev_id)
{
    u32 stat, stat1, i, cnt, v;
    struct ir_priv *ir  = &ir_local;
    struct key_attr key = {
        0
    };
    stat = readl(ir->base + IR_INT_STATUS);

    hiir_debug("irq stat:0x%x\n", stat);
    stat1 = (stat & IR_KEY_STAT_MASK);
    if (stat1)
    {
        hiir_error("Unkown interrupt 0x%x\n", stat1);

        if (stat1 & IR_INTMS_RCV)
        {
            v = readl(ir->base + IR_DATAL);
            writel(IR_INTC_RCV, ir->base + IR_INT_CLR);
        }

        if (stat1 & IR_INTMS_FRAMERR)
        {
            writel(IR_INTC_FRAMERR, ir->base + IR_INT_CLR); 
        }

        if (stat1 & IR_INTMS_FRAMERR)
        {
            writel(IR_INTC_OVERFLOW, ir->base + IR_INT_CLR);  
        }

        if (stat1 & IR_INTMS_RELEASE)
        {
            writel(IR_INTC_RELEASE, ir->base + IR_INT_CLR);  
        }
        
    }
    
    g_bTimeOutFlag = HI_FALSE;

    stat &= IR_SYMBOL_STAT_MASK;
    if (stat & (IR_INTMS_OVERRUN))
    {
        ir_clear_fifo(ir);
        stat &= ~(IR_INTMS_OVERRUN);
        writel(IR_INTC_OVERRUN, ir->base + IR_INT_CLR);

        if ((ir->chip_param.ir_max_bits >= 1)
            && (ir->chip_param.ir_max_bits < 0x1f))
        {
            ir->chip_param.ir_max_bits--;

            v  = readl(ir->base + IR_CFG);
            v &= 0xFFFFC0FF;
            v |= (ir->chip_param.ir_max_bits << 8);
            writel(v, ir->base + IR_CFG);
        }

        hiir_debug("int overrun!\n");
    }

    if (stat & (IR_INTMS_SYMB_RCV | IR_INTMS_TIME_OUT))
    {
        cnt = readl(ir->base + IR_DATAH);
        for (i = 0; i < cnt; i++)
        {
            v = readl(ir->base + IR_DATAL);
            key.upper = (unsigned long long)
                        ((v & 0xffff0000) >> 16) * 10;
            key.lower = (unsigned long long)(v & 0xffff) * 10;
            if (ir->key_fetch == KEY_FETCH_RAW)
            {
                ir_insert_key_tail(ir->key_buf, &key);
            }
            else
            {
                ir_insert_key_tail(ir->symbol_buf, &key);
            }
        }

        if (stat & IR_INTMS_SYMB_RCV)
        {

            writel(IR_INTC_SYMB_RCV, ir->base + IR_INT_CLR);
        }
        else
        {
            writel(IR_INTC_TIME_OUT, ir->base + IR_INT_CLR);
            g_bTimeOutFlag = HI_TRUE;
        }

        stat &= ~(IR_INTMS_SYMB_RCV | IR_INTMS_TIME_OUT);

        if (ir->key_fetch == KEY_FETCH_PARSED)
        {

            tasklet_schedule(&ir->ir_bh);
        }
        else
        {
            wake_up_interruptible(&ir->read_wait);
        }
    }

    if (stat)
    {
        hiir_error("Unkown interrupt 0x%x\n", stat);
        //BUG();
    }

    return IRQ_HANDLED;
}

static void ir_init_prot(void)
{
#ifdef NEC_SUPPORT
    nec_init();
#endif
	
#ifdef RC6_SUPPORT
    rc6_init();
#endif
	
#ifdef RC5_SUPPORT
    rc5_init();
#endif
	
#ifdef SONY_SUPPORT
    sony_init();
#endif
	
#ifdef TC9012_SUPPORT
    tc9012_init();
#endif
	
#ifdef CREDIT_SUPPORT
    credit_init();
#endif

}

static void ir_deinit_prot(void)
{
#ifdef NEC_SUPPORT
    nec_exit();
#endif
	
#ifdef RC6_SUPPORT
    rc6_exit();
#endif
	
#ifdef RC5_SUPPORT
    rc5_exit();
#endif
	
#ifdef SONY_SUPPORT
    sony_exit();
#endif
	
#ifdef TC9012_SUPPORT
    tc9012_exit();
#endif
	
#ifdef CREDIT_SUPPORT
    credit_exit();
#endif

}

static int ir_open(struct inode *inode, struct file *filp)
{
    int ret;
    struct ir_priv *ir = &ir_local;    

    if (1 == atomic_inc_return(&ir->refcnt))
    {
		ir_set_clk(HI_TRUE);
		ir_init_prot();
        ret = ir_config(ir);
        if (ret)
        {
            return ret;
        }

        ir_clear_fifo(ir);
        writel((u32) ~0, ir->base + IR_INT_CLR);

        writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);

        init_waitqueue_head(&ir->read_wait);

        ir_start(ir);
    }

    return 0;
}

static int ir_close(struct inode *inode, struct file *filp)
{
    struct ir_priv *ir = &ir_local;

    if (down_interruptible(&ir->sem))
    {
        hiir_info("Fail to hold read semaphore!\n");
    }

    if (atomic_dec_and_test(&ir->refcnt))
    {
        ir_clear_fifo(ir);
        writel((u32) ~0, ir->base + IR_INT_CLR);
        writel((u32) ~0, ir->base + IR_INT_MASK);
        ir_disable(ir);

        memset(ir->key_buf, 0, sizeof(struct ir_buffer));
        if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
        {
            memset(ir->symbol_buf, 0,
                   sizeof(struct ir_buffer));
        }
		ir_deinit_prot();
		ir_set_clk(HI_FALSE);
    }

    up(&ir->sem);

    
    return 0;
}

static void ir_reset(struct ir_priv *ir)
{
    ir_enable(ir);

    writel(0, ir->base + IR_INT_MASK);
    memset(ir->key_buf, 0, sizeof(struct ir_buffer));
    if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
    {
        memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
    }

    ir_clear_fifo(ir);
    ir_disable(ir);
}

static long ir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long ret = 0;

    struct ir_priv *ir = &ir_local;

    switch (cmd)
    {
    case CMD_IR_ENABLE_KEYUP:
        ir->key_up_event = arg;
        break;
    case CMD_IR_ENABLE_REPKEY:
        ir->key_repeat_event = arg;
        if (!ir->key_repeat_event)
        {
            hiir_info("You have disabled key hold event," \
                      " which will cause some crazy key press" \
                      " action not recevied.!\n" \
                      " For some protocol like nec full," \
                      " press twice the same key very quickly will be" \
                      " recoginzed as a hold event.\n");
        }

        break;
    case CMD_IR_SET_REPKEY_TIMEOUT:
        ir->key_repeat_interval = arg;
        break;
    case CMD_IR_SET_ENABLE:

        /* config ir_cfg register and set enable,
         * enable interrupt.
         */
        if (!arg)
        {           
            ir_enable(ir);

            writel(0, ir->base + IR_INT_MASK);
            memset(ir->key_buf, 0, sizeof(struct ir_buffer));
            if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
            {
                memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
            }

            ir_clear_fifo(ir);
            ir_disable(ir);
            ir_deinit_prot();
            ir_set_clk(HI_FALSE);
            break;
        }
		
        ir_set_clk(HI_TRUE);
        ir_init_prot();
        ret = ir_config(ir);
        if (ret)
        {
            break;
        }

        ir_clear_fifo(ir);
        if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
        {
            writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);
        }

        ir_start(ir);
		
        break;
    case CMD_IR_RESET:

        /* disable interrupt,
         * clear key & symbol buf and more what?
         */
        writel(0, ir->base + IR_INT_MASK);
        memset(ir->key_buf, 0, sizeof(struct ir_buffer));
        if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
        {
            memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
        }

        ir_clear_fifo(ir);

        writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);
        break;
    case CMD_IR_SET_BLOCKTIME:

        /* set read time out */
        ir->key_read_timeout = arg;
        break;
    case CMD_IR_SET_FORMAT:

        /* I think this command is not necessary under both mode.
         * So, abandon it.
         */
        break;
    case CMD_IR_SET_BUF:

        /* Current buffer is large enough,
         * and in this driver, buffer size is fix.
         * Abandon it.
         */
        break;
    case CMD_IR_SET_FETCH_METHOD:
        ir->key_fetch = (enum KEY_FETCH_MODE)arg;
        break;
    case CMD_IR_SET_PROT_ENABLE:
    {
        struct ir_protocol *ip;
        char name[PROTOCOL_NAME_SZ] =
        {
            0
        };

        if (copy_from_user((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1))
        {
            return -EFAULT;
        }

        name[PROTOCOL_NAME_SZ - 1] = '\0';

        ip = ir_prot_first();
        while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT)
        {
            if (!HI_OSAL_Strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name)))
            {
                ip->disabled = 0;
                return 0;
            }

            hiir_info("\n ip->ir_code_name = %s, name = %s  \n", ip->ir_code_name, name);
            ip = ir_prot_next(ip);
        }

        return -EINVAL;
    }
    case CMD_IR_SET_PROT_DISABLE:
    {
        struct ir_protocol *ip;
        char name[PROTOCOL_NAME_SZ] = {
            0
        };

        if (copy_from_user((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1))
        {
            return -EFAULT;
        }

        name[PROTOCOL_NAME_SZ - 1] = '\0';

        ip = ir_prot_first();
        while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT)
        {
            if (!HI_OSAL_Strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name)))
            {
                ip->disabled = 1;
                return 0;
            }

            ip = ir_prot_next(ip);
        }

        return -EINVAL;
    }
    case CMD_IR_GET_PROT_ENABLED:
    {
        struct ir_protocol *ip;
        char name[PROTOCOL_NAME_SZ] = {
            0
        };

        if (copy_from_user((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1))
        {
            return -EFAULT;
        }

        name[PROTOCOL_NAME_SZ - 1] = '\0';
        ip = ir_prot_first();
        while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT)
        {
            if (!HI_OSAL_Strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name)))
            {
                if (copy_to_user((void*)arg, &ip->disabled, sizeof(int)))
                {
                    return -EFAULT;
                }

                return 0;
            }

            ip = ir_prot_next(ip);
        }

        return -EINVAL;
    }

    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

static unsigned int ir_poll(struct file *filp, struct poll_table_struct *wait)
{
    struct ir_priv *ir = &ir_local;

    if (down_interruptible(&ir->sem))
    {
        return -EAGAIN;
    }

    if (ir->key_buf->writer != ir->key_buf->reader)
    {
        goto out;
    }

    poll_wait(filp, &ir->read_wait, wait);
out:
    up(&ir->sem);
    return 0;
}

static ssize_t ir_read(struct file *filp, char __user *buf,
                       size_t count, loff_t *f_pos)
{
    struct ir_priv *ir = &ir_local;
    int ret = 0;
    struct key_attr *key;
	
	if( atomic_read(&ir->refcnt) == 0)
	{
		HI_FATAL_IR("not open!\n");
		return -1;
	}

    if (down_interruptible(&ir->sem))
    {
        return -EAGAIN;
    }

    ir->key_debug_trycount++;

    /* NOTE: writer may run over reader,
     * in these case should increase buffer more large.
     * Because ir module may not generate so many data
     * at the same time in normal case, so here simply check cursor..
     */
    while (ir->key_buf->writer == ir->key_buf->reader)
    {
        if (filp->f_flags & O_NONBLOCK)
        {
            ret = -EAGAIN;
            goto out;
        }
		
		if ((u32) ~0 == ir->key_read_timeout)
		{
			ir->key_read_timeout = 200;
		}
		
        ret = wait_event_interruptible_timeout(ir->read_wait,
                                               ir->key_buf->writer != ir->key_buf->reader,
                                               msecs_to_jiffies(ir->key_read_timeout));

        if (ret < 0)
        {
            ret = -EAGAIN;
            goto out;
        }
        else if (ret == 0)
        {
            ret = -ETIME;
            goto out;
        }
        else
        {
            break;
        }

    }

    ret = 0;
    while (ir->key_buf->writer != ir->key_buf->reader && ret < count)
    {
        key = ir_reader_inc(ir->key_buf);
        if (key->key_stat == KEY_STAT_DOWN)
        {
            HI_DRV_STAT_Event(STAT_EVENT_KEYOUT, (u32)key->lower);
        }

        if (copy_to_user(buf + ret, key, sizeof(*key)))
        {
            ret = -EFAULT;
            goto out;
        }

        ir->key_debug_succeedcount++;

        memset(key, 0, sizeof(*key));
        ret += sizeof(*key);
    }

out:
    up(&ir->sem);
    return ret;
}

static ssize_t ir_write(struct file *f, const char __user *b, size_t c, loff_t *o)
{
    return 0;
}

static struct file_operations ir_fops =
{
    .owner = THIS_MODULE,
    .open  = ir_open,
    .write = ir_write,
    .unlocked_ioctl = ir_ioctl,
#ifdef CONFIG_COMPAT
     .compat_ioctl = ir_ioctl,
#endif
    .poll  = ir_poll,
    .read  = ir_read,
    .release = ir_close
};
static void ir_init_callbacks(struct ir_priv *ir)
{
    ir->callbacks.isr_handle  = ir_isr_symbol;
    ir->callbacks.data_handle = ir_bh_symbol;
}

static int ir_request_resource(struct ir_priv *ir)
{
    int ret = -ENOMEM;
    if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
    {
        ir->symbol_buf =
            HI_KMALLOC(HI_ID_IR, sizeof(struct ir_buffer), GFP_KERNEL | __GFP_ZERO);
        if (!ir->symbol_buf)
        {
            goto symbol_buf;
        }

        tasklet_init(&ir->ir_bh, ir->callbacks.data_handle, (unsigned long)ir);
    }

    ir->key_buf = HI_KMALLOC(HI_ID_IR, sizeof(struct ir_buffer), GFP_KERNEL | __GFP_ZERO);
    if (!ir->key_buf)
    {
        goto key_buf;
    }

    init_timer(&ir->symbol_timer);
    ir->symbol_timer.function = ir_symbol_timer_proc;
    ir->symbol_timer.data = (unsigned long)ir;

    sema_init(&ir->sem, 1);

    ret = -EFAULT;
    if (request_irq(ir->irq, ir->callbacks.isr_handle, IRQF_DISABLED, "hi_ir_s2_irq", (void *)ir))
    {
        goto irq;
    }
    else
    {
        if(HI_SUCCESS != HI_DRV_SYS_SetIrqAffinity(HI_ID_IR, ir->irq))
        {
            hiir_info("HI_DRV_SYS_SetIrqAffinity failed.\n");
        }   
    }

    return 0;

irq:
    HI_KFREE(HI_ID_IR, ir->key_buf);
key_buf:
    if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
    {
        HI_KFREE(HI_ID_IR, ir->symbol_buf);
    }

symbol_buf:
    return ret;
}

static void ir_adjust_freq(void)
{
    HI_CHIP_TYPE_E chip_type = HI_CHIP_TYPE_BUTT;
    HI_CHIP_VERSION_E chip_version = HI_CHIP_VERSION_BUTT;

    /* get chip version and adjust frequence. */
    HI_DRV_SYS_GetChipVersion(&chip_type, &chip_version);

    if ((HI_CHIP_TYPE_HI3716M == chip_type) && (HI_CHIP_VERSION_V300 == chip_version))
    {
        ir_frequence = DFT_MV300_FREQ;
    }
    else
    {
        ir_frequence = DFT_FREQ;
    }

    return;
}

static void ir_free_resource(struct ir_priv *ir)
{
    del_timer_sync(&ir->symbol_timer);
    free_irq(ir->irq, (void *)ir);
    HI_KFREE(HI_ID_IR, ir->symbol_buf);
    HI_KFREE(HI_ID_IR, ir->key_buf);
}

static inline void parameter_helper_symbol(void)
{
    hiir_info("Available parameters under symbol mode are: \n" \
              "\t- freq: frequence control.\n" \
              "\t- symbol_width: max pluse + space width\n" \
              "\t- int_level: how many symbols to generate a interrupt.\n" \
              "\t- key_fetch: key fetch mode, set 1 to fetch raw symbol\n");
}

static int ir_check_module_param(void)
{
    if ((ir_keyhold_timeout >= 1000) || (ir_frequence > 0x7f))
    {
        hiir_info("key hold timeout >= 0x1000"
                  " or frequence > 0x7f!\n");
        return -EINVAL;
    }

    if (ir_keyhold_timeout < 150)
    {
        hiir_info("key hold time too small," \
                  "may due to subsequence of 'DOWN,HOLD,,,UP,DOWN,UP'" \
                  "while a key being press for a while\n");
    }

    /* Now we don't support key mode */
    if (ir_work_mode == IR_MODE_SYMBOL)
    {
        if ((ir_symbol_width > 0xffff)
            || (ir_symbol_format > 1)
            || (ir_int_level > 0x1f)
            || (ir_frequence > 0x7f)
            || (ir_key_fetch_raw > 1))
        {
            parameter_helper_symbol();
            return -EINVAL;
        }

        return 0;
    }

    return -EINVAL;
}

static struct miscdevice ir_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = IR_DEVICE_NAME,
    .fops  = &ir_fops,
};

static int ir_plat_driver_probe(struct platform_device *pdev)
{
    int ret;
    struct ir_priv *ir = &ir_local;

    ir_set_clk(HI_FALSE);
    ir_adjust_freq();

    if (ir_check_module_param())
    {
        hiir_info("Invalid module parameters!\n");
        return -EINVAL;
    }

    memset(ir, 0, sizeof(struct ir_priv));

    if (ir_work_mode == IR_MODE_SYMBOL)
    {
        ir->chip_param.ir_max_symbol = ir_symbol_width;
        ir->chip_param.ir_max_bits = ir_int_level;
        ir->key_fetch = ir_key_fetch_raw;
        ir->chip_param.ir_format = ir_symbol_format;
        ir->chip_param.ir_freq = ir_frequence;
    }

    ir->chip_param.work_mode = IR_MODE_SYMBOL;

    ret = ir_protocol_init();
    if (ret)
    {
        goto prot;
    }

    ir_init_callbacks(ir);

    HI_OSAL_Strncpy(ir->dev_name, IR_DEVICE_NAME,
            strlen(IR_DEVICE_NAME) >= (IR_DEVICE_NAME_SZ - 1) ?
            IR_DEVICE_NAME_SZ : (strlen(IR_DEVICE_NAME) + 1));

    ir->irq  = IR_IRQ_NO;
    ir->base = ioremap_nocache(IR_IO_BASE, IR_ADDR_SIZE);
    if (ir->base == HI_NULL)
    {
        hiir_info("ir ioremap_nocache err! \n");
        return -EINVAL;
    }

    ir->key_hold_timeout_time = ir_keyhold_timeout;

    ir->key_read_timeout = (u32) ~0;

    ir->key_up_event = 1;
    ir->key_repeat_event = 1;
    ir->key_repeat_interval = 200;
    ret = ir_request_resource(ir);
    if (ret)
    {
        goto res;
    }

    ret = misc_register(&ir_misc);
    if (ret)
    {
        goto reg_dev;
    }

    atomic_set(&ir->refcnt, 0);

#ifdef CONFIG_DEVFS_FS
    devfs_mk_cdev(MKDEV(ir->major, 0),
                  S_IFCHR | S_IRUGO | S_IWUSR, ir->dev_name);
#endif

#ifdef MODULE
    HI_PRINT("Load hi_ir.ko success.  \t(%s)\n", VERSION_STRING);
#endif
    return 0;
reg_dev:
    ir_free_resource(ir);
res:
prot:
    hiir_error("Load hi_ir.ko fail(0x%x)!\n", ret);
    return ret;
}

static int ir_plat_driver_remove(struct platform_device *pdev)
{
    struct ir_priv *ir = &ir_local;

#ifdef CONFIG_DEVFS_FS
    devfs_remove(ir->dev_name);
#endif

    misc_deregister(&ir_misc);

    ir_free_resource(ir);

    iounmap(ir->base);

    ir_protocol_exit();

    return 0;
}

#if (1 == HI_PROC_SUPPORT)
static HI_S32 IrProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 u32RegVal;
    struct ir_priv *ir = &ir_local;
    struct ir_protocol *ip;
    HI_U8 i = 0;
    unsigned long flag;

    u32RegVal = readl(ir->base + IR_EN);
    PROC_PRINT(p, "---------Hisilicon IR Info---------\n");
    PROC_PRINT(p,
                    "IR   Enable             \t :%s\n"
                    "IR   Code               \t :%s\n"
                    "IR   WorkMode           \t :%s\n"
                    "IR   FetchMode          \t :%s\n"
                    "IR   KeyUpEnable        \t :%s\n"
                    "IR   UpEventDelay       \t :%d(ms)\n"
                    "IR   RepeatkeyEnable    \t :%s\n"
                    "IR   RepkeyDelayTime    \t :%d(ms)\n"
                    "IR   ReportKeyBlockTime \t :%d(ms)\n"
                    "IR   ModuleFrequence    \t :%d(MHz)\n",

                    (u32RegVal == 1) ? "Enable" : "Disable",
                    "IR_S2",
                    (ir->chip_param.work_mode == 0) ? "Chip Report Keys" : "Chip Report Symbols",
                    (ir->key_fetch) ? "Drive Report Raw Symbols" : "Drive Report Parsed Symbols",
                    (ir->key_up_event) ? "Enable" : "Disable",
                    ir->key_hold_timeout_time,
                    (ir->key_repeat_event) ? "Enable" : "Disable",
                    ir->key_repeat_interval,
                    ir->key_read_timeout,
                    (ir->chip_param.ir_freq + 1));

    PROC_PRINT(p, "\nRegistered Protocols info:\n");

    spin_lock_irqsave(&irlock, flag);

    ip = ir_prot_first();

    while (ir_prot_valid(ip))
    {
        PROC_PRINT(p, "No.%d:\tStatus:%12s, \tName: \t%s \n",
                        i++, ip->disabled ? "Disabled" : "Enabled", ip->ir_code_name);
        ip = ir_prot_next(ip);
    }

    spin_unlock_irqrestore(&irlock, flag);

    PROC_PRINT(p, "\nKey getting info:\n");
    PROC_PRINT(p,
                    "Get(Try/OK) \t :%d/%d\n",
                    ir->key_debug_trycount, ir->key_debug_succeedcount);

    if (ir->key_buf)
    {
        PROC_PRINT(p, "\nKey buffer info:\n");
        PROC_PRINT(p, "\tBuffer size:  \t%d keys \n", MAX_SYMBOL_NUM);
        PROC_PRINT(p, "\tReader at:  \t%d\n", ir->key_buf->reader);
        PROC_PRINT(p, "\tWriter at:  \t%d\n", ir->key_buf->writer);
    }

    if ((ir->chip_param.work_mode == IR_MODE_SYMBOL) && ir->symbol_buf)
    {
        PROC_PRINT(p, "\nSymbol buffer info:\n");
        PROC_PRINT(p, "\tBuffer size: \t%d symbols\n", MAX_SYMBOL_NUM);
        PROC_PRINT(p, "\tReader at: \t%d\n", ir->symbol_buf->reader);
        PROC_PRINT(p, "\tWriter at: \t%d\n", ir->symbol_buf->writer);
    }

    return HI_SUCCESS;
}
#endif

#ifdef CONFIG_PM
static int ir_plat_driver_suspend(struct platform_device *pdev, \
                                  pm_message_t            state)
{
    struct ir_priv *ir = &ir_local;

    ir_disable(ir);
    return 0;
}

static int ir_plat_driver_resume(struct platform_device *pdev)
{
    struct ir_priv *ir = &ir_local;
    int ret;

    ir_reset(ir);
    if (atomic_read(&ir->refcnt) > 0)
    {
        ret = ir_config(ir);
        if (ret)
        {
            hiir_error(" Fail to set ir configuration. ret = 0x%x \n", ret);
        }


        writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);

            ir_start(ir);
    }

    return 0;
}

#else
#define ir_plat_driver_suspend (NULL)
#define ir_plat_driver_resume (NULL)
#endif
static struct platform_driver ir_platform_driver = {
    .probe   = ir_plat_driver_probe,
    .remove  = ir_plat_driver_remove,
    .suspend = ir_plat_driver_suspend,
    .resume  = ir_plat_driver_resume,
    .driver  = {
        .owner = THIS_MODULE,
        .name  = IR_DEVICE_NAME,
        .bus   = &platform_bus_type,
    },
};
#define IR_IO_SZ (SZ_1K * 4)
static struct resource ir_resources[] = {
    [0] = {
        .start = IR_IO_BASE,
        .end   = IR_IO_BASE + IR_IO_SZ - 1,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = IR_IRQ_NO,
        .end   = IR_IRQ_NO,
        .flags = IORESOURCE_IRQ,
    }
};
static void ir_platform_device_release(struct device* dev){}

static struct platform_device ir_platform_device = {
    .name                  = IR_DEVICE_NAME,
    .id  =                          0,
    .dev = {
        .platform_data     = NULL,
        .dma_mask          = (u64*)~0,
        .coherent_dma_mask = (u64) ~0,
        .release           = ir_platform_device_release,
    },
    .num_resources         = ARRAY_SIZE(ir_resources),
    .resource              = ir_resources,
};

int IR_DRV_ModInit(void)
{
    int ret = 0;
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S *item;


    DRV_PROC_EX_S stFnOpt =
    {
         .fnRead = IrProcRead,
    };
#endif
    (HI_VOID)HI_DRV_MODULE_Register(HI_ID_IR, "HI_IR", HI_NULL);
#if (1 == HI_PROC_SUPPORT)
    item = HI_DRV_PROC_AddModule(HI_MOD_IR, &stFnOpt, NULL);
    if (!item)
    {
        hiir_error("add proc ir failed\n");

        return HI_FAILURE;
    }
#endif
    ret = platform_device_register(&ir_platform_device);
    if (ret)
    {
        hiir_info("register platform device failed!");
        goto _error_register_device;
    }

    ret = platform_driver_register(&ir_platform_driver);
    if (ret)
    {
        hiir_info("register platform driver failed!");
        goto _error_register_driver;
    }

    return ret;

_error_register_driver:
    platform_device_unregister(&ir_platform_device);

_error_register_device:

    return -1;
}

void IR_DRV_ModExit(void)
{
#if (1 == HI_PROC_SUPPORT)
    HI_DRV_PROC_RemoveModule(HI_MOD_IR);
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_IR);

    platform_driver_unregister(&ir_platform_driver);

    platform_device_unregister(&ir_platform_device);
}

#ifdef MODULE
module_init(IR_DRV_ModInit);
module_exit(IR_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HISILION");

