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

#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#else
#include "hi_common.h"
#endif
#include "hi_type.h"
#include "ir.h"
#include "ir_protocol.h"
#include "ir_utils.h"
#include "ir_register.h"
#if defined (CFG_ANDROID_TOOLCHAIN)
#include "hiir_ir2.h"
#endif
/*************************** public parameters ********************/
/* chip work mode, key or symbol. */
static u32 ir_work_mode = IR_MODE_SYMBOL; /* default mode symbol. */

static u32 ir_frequence = DFT_FREQ;

static u32 ir_keyhold_timeout = DFT_KEY_HOLD_TIME; /*ms*/

/************************** symbol mode parameters ****************/
/* reserve the parameter names. */
static u32 ir_symbol_width = DFT_SYMBOL_WIDTH;

static u32 ir_symbol_format = DFT_SYMBOL_FMT;

static u32 ir_int_level = DFT_INT_LEVEL;

/* for raw symbol read, specify this parameter. */
static u32 ir_key_fetch_raw = 0;

static struct  ir_buffer s_symbol_buf;
static struct  ir_buffer s_key_buf ;

#ifdef HI_MINIBOOT_SUPPORT
#define readl(_a)        (*(volatile u32 *)(_a))
#define writel(_v, _a)   (*(volatile u32 *)(_a) = (_v))
#endif

/**************************************************************************/
struct ir_priv ir_local = {0};

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
static inline void ir_stop(struct ir_priv *ir)
{
	writel(0, ir->base + IR_START);
}
static void ir_clear_fifo(struct ir_priv *ir)
{
	u32 cnt, i;
	cnt = readl(ir->base + IR_DATAH);
	for (i = 0; i < cnt; i++)
		readl(ir->base + IR_DATAL);
}

static int ir_config(struct ir_priv *ir)
{
	u32 val = 0;

	ir_enable(ir);

	while (((readl(ir->base + IR_BUSY)) & 0x1) && val++ < 1000)
	{
	    udelay(10*1000);
	}

	if (val >= 1000) 
	{
		hiir_error("ir module is busy!\n");
		return HI_FAILURE;
	}

	if (ir->chip_param.work_mode == IR_MODE_KEY) 
	{
		return HI_FAILURE;
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

	return 0;
}
static int ir_symbol_proc(struct ir_priv *ir)
{
	struct ir_protocol *ip;
	struct key_attr *key;
	enum IR_MATCH_RESULT r;
	int matchs;
	/* 1. find out which is a infrared code match the head symbol.
	 * 2. if no, parse from this head symbol.
	 * 3. if yes, we should check the whole frame to get a
	 * accurate match infrared code for this key.
	 */
	key = ir_first_read_key(ir->symbol_buf);
	while (key && key->upper && key->lower) {
		hiir_debug("header key at(%d): key->lower:%lld,"
				" key->upper:%lld",
				ir->symbol_buf->reader,
				key->lower, key->upper);
		ir_protocols_lock();
		ip = ir_prot_first();
		matchs = 0;
		while (ir_prot_valid(ip)) {
			if (!ip->match || ip->disabled ) {
				ip = ir_prot_next(ip);
				continue;
			}
			/* try find header first! */
			r = ip->match(IR_MTT_HEADER, ir->symbol_buf, ip);
			if (IR_MATCH_MATCH != r) {
				if (IR_MATCH_NEED_MORE_DATA == r) {
					matchs ++;
					ip = ir_prot_next(ip);
					continue;
				}
				if (ip->flag & INFR_HAS_ERR_HANDLE) {
					r = ip->match_error(IR_MTT_HEADER,
						ir->symbol_buf, ip);
					if (r != IR_MATCH_MATCH) {
						if (IR_MATCH_NEED_MORE_DATA == r)
							matchs ++;
						ip = ir_prot_next(ip);
						continue;
					}
				} else {
					ip = ir_prot_next(ip);
					continue;
				}
			} 
			/* find a match. */
			r = ip->match(IR_MTT_FRAME, ir->symbol_buf, ip);

			if (IR_MATCH_MATCH != r
				&& (ip->flag & INFR_HAS_ERR_HANDLE)) {

				r = ip->match_error(IR_MTT_FRAME,
					ir->symbol_buf, ip);
			}
			if (IR_MATCH_MATCH == r) {

			//	del_timer_sync(&ir->symbol_timer);

				/* if match, parse this frame */
				r = ip->handle(ir, ip, ir->symbol_buf,
						ir->key_buf);
				if (!r)
					wake_up_interruptible(&ir->read_wait);

				if (r && (ip->flag & INFR_HAS_ERR_HANDLE))
					ip->parse_error(ir, ip,
						ir->symbol_buf, ir->key_buf);
				ir_protocols_unlock();
				goto next_frame;
			} else if (IR_MATCH_NEED_MORE_DATA == r)
				matchs ++;

			ip = ir_prot_next(ip);
		}
		ir_protocols_unlock();
		/* if no match, discard this symbol. */
		if (!matchs) {
			key = ir_next_reader_clr_inc(ir->symbol_buf);
			continue;
		}

		/* if some infrared code match, do not discard,
		 * wait timer expires and try again.
		 */
		if (!ir_prot_valid(ip))
			return -1;

next_frame:
		key = ir_first_read_key(ir->symbol_buf);
	}
	return 0;
}

#if 0
static void ir_symbol_timer_proc(unsigned long data) 
{
	struct ir_priv *ir = &ir_local;
	struct key_attr *key;
	struct ir_protocol *ip;
	enum IR_MATCH_RESULT r;
	static unsigned long deadline;

	if (!ir_symbol_proc(ir))
		goto out;

	hiir_debug("Symbols cannot be parsed."\
			"Abandon them till frame header appear"\
			" or buffer empty!\n");
	/* Case symbol cannot be parsed, ignore symbols until
	 * new frame start symbol appears or buffer empty.
	 */
	key = ir_first_read_key(ir->symbol_buf);
	while (key && key->upper && key->lower) {

		ir_protocols_lock();
		ip = ir_prot_first();
		while (ir_prot_valid(ip)) {
			if (!ip->match || ip->disabled) {
				ip = ir_prot_next(ip);
				continue;
			}

			r = ip->match(IR_MTT_HEADER, ir->symbol_buf, ip);
			if (IR_MATCH_MATCH == r) {
				r = ip->match(IR_MTT_FRAME,
					ir->symbol_buf, ip);
				if (IR_MATCH_MATCH != r
					&& (ip->flag & INFR_HAS_ERR_HANDLE)) {

					r = ip->match_error(IR_MTT_FRAME,
							ir->symbol_buf, ip);
				}

				if (IR_MATCH_MATCH == r) {
					ir_protocols_unlock();
					ir_symbol_proc(ir);
					deadline = 0;
					return;
				}
				if (deadline && time_after_eq(jiffies,
					deadline)) {
					ip = ir_prot_next(ip);
					continue;
				} else
					deadline = jiffies +
						msecs_to_jiffies(
						SYMBOL_PARSE_TIMEOUT);

				/* A new frame start. Abort */
				del_timer_sync(&ir->symbol_timer);
				ir->symbol_timer.expires = jiffies
					+ msecs_to_jiffies(
						SYMBOL_PARSE_TIMEOUT);
				add_timer(&ir->symbol_timer);
				ir_protocols_unlock();
				goto out;
			} else if (IR_MATCH_NEED_MORE_DATA == r) {
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
					deadline)) {
					ip = ir_prot_next(ip);
					continue;
				} else {
					deadline = jiffies +
						msecs_to_jiffies(
							SYMBOL_PARSE_TIMEOUT);
					del_timer_sync(&ir->symbol_timer);
					ir->symbol_timer.expires = jiffies
						+ msecs_to_jiffies(
							SYMBOL_PARSE_TIMEOUT);
					add_timer(&ir->symbol_timer);
					ir_protocols_unlock();

					goto out;
				}
			}

			ip = ir_prot_next(ip);
		}
		ir_protocols_unlock();
		/* Clear the header symbol first!
		 * Think about there are symbols sequence like this:
		 * header,data,but no burst.
		 * We should destroy header first to avoid dead loop!
		 */
		memset(key, 0, sizeof(*key));

		ir->symbol_buf->reader ++;
		if (ir->symbol_buf->reader == MAX_SYMBOL_NUM)
			ir->symbol_buf->reader = 0;

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
#endif

static void ir_bh_symbol(void)
{
	struct ir_priv *ir = &ir_local;
	/* If frame cannot be parsed this time,
	 * we will try again after symbol_timer expires.
	 */
	if (ir_symbol_proc(ir)) {
		hiir_debug("parse symbols fail, try again when timer expires!\n");
#if 0
		del_timer_sync(&ir->symbol_timer);
		ir->symbol_timer.expires = jiffies
			+ msecs_to_jiffies(SYMBOL_PARSE_TIMEOUT);
		add_timer(&ir->symbol_timer)
#endif
	} else {
		hiir_debug("parse symbols success!\n");
	}
	return;
}
static int ir_isr_symbol(void)
{
	u32 stat, i, cnt, v;
	struct ir_priv *ir = &ir_local;
	struct key_attr key = {0};

	stat = readl(ir->base + IR_INT_STATUS);

	//hiir_debug("irq stat:0x%x\n",stat);

	stat &= IR_SYMBOL_STAT_MASK;
	if (stat & (IR_INTMS_OVERRUN)) {
		ir_disable(ir);
		ir_enable(ir);
		ir_clear_fifo(ir);

		writel((u32)~0, ir->base + IR_INT_CLR);
		stat = 0; 

		memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
		ir_stop(ir);
		ir_config(ir);
		ir_start(ir);
		if (ir->chip_param.ir_max_bits >= 1
				&& ir->chip_param.ir_max_bits < 0x1f) {

			ir_stop(ir);
			ir->chip_param.ir_max_bits --;
			ir_config(ir);

			v = readl(ir->base + IR_CFG);
			v &= 0xFFFFC0FF;
			v |= (ir->chip_param.ir_max_bits << 8);
			writel(v, ir->base + IR_CFG);
			ir_start(ir);
		}
		hiir_debug("int overrun!\n");
	}
	if (stat & (IR_INTMS_SYMB_RCV | IR_INTMS_TIME_OUT)) {
		cnt = readl(ir->base + IR_DATAH); 
		for (i = 0; i < cnt; i++) {
			v = readl(ir->base + IR_DATAL);
			key.upper = (unsigned long long)
				((v & 0xffff0000) >> 16) * 10;
			key.lower = (unsigned long long)(v & 0xffff) * 10;

			if (ir->key_fetch == KEY_FETCH_RAW)
				ir_insert_key_tail(ir->key_buf, &key);
			else
				ir_insert_key_tail(ir->symbol_buf, &key);
		}

		if (stat & IR_INTMS_SYMB_RCV)
			writel(IR_INTC_SYMB_RCV, ir->base + IR_INT_CLR);
		else
			writel(IR_INTC_TIME_OUT, ir->base + IR_INT_CLR);

		stat &= ~(IR_INTMS_SYMB_RCV | IR_INTMS_TIME_OUT); 

		if (ir->key_fetch == KEY_FETCH_PARSED)
			ir_bh_symbol();
	}
	if (stat) {
		hiir_error("Unkown interrupt 0x%x\n", stat);
		return -1;
	}
	return 0;
}

static int ir_open(void)
{
	int ret;
	struct ir_priv *ir = &ir_local;

	ret = ir_config(ir);
	if (ret)
		return ret;

	ir_clear_fifo(ir);
	
	writel((u32)~0, ir->base + IR_INT_CLR);

	if (ir->chip_param.work_mode == IR_MODE_KEY) 
	{
		hiir_error("work mode is key mode!\n");
		return HI_FAILURE;
	} 
	else
	{
		writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);
    }
    
	init_waitqueue_head(&ir->read_wait);

	ir_start(ir);
	
	ir->chip_param.module_enable = 1;

	return 0;
}

static int ir_close(void)
{
	struct ir_priv *ir = &ir_local;

	ir_clear_fifo(ir);
	
	writel((u32)~0, ir->base + IR_INT_CLR);
	writel((u32)~0, ir->base + IR_INT_MASK);
	
	ir_disable(ir);

	memset(ir->key_buf, 0, sizeof(struct ir_buffer));
	if (ir->chip_param.work_mode == IR_MODE_SYMBOL)
	{
		memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
    }
    
	ir->chip_param.module_enable = 0;

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

static int ir_ioctl(unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct ir_priv *ir = &ir_local;

	switch (cmd) 
	{
		case CMD_IR_ENABLE_KEYUP:
		{
			ir->key_up_event = arg;
			break;
        }
		case CMD_IR_ENABLE_REPKEY:
        {
			ir->key_repeat_event = arg;
			if (!ir->key_repeat_event)
				hiir_info("You have disabled key hold event,"\
					" which will cause some crazy key press"\
					" action not recevied.!\n"\
					" For some protocol like nec full,"\
					" press twice the same key very quickly will be"\
					" recoginzed as a hold event.\n");
			break;
        }
		case CMD_IR_SET_REPKEY_TIMEOUT:
        {
			ir->key_repeat_interval = arg;
			break;
        }    
		case CMD_IR_SET_ENABLE:
        {
			/* config ir_cfg register and set enable,
			 * enable interrupt. 
			 */
			if (!arg) 
			{
				ir_reset(ir);
				ir_stop(ir);
				ir->chip_param.module_enable = 0;
				break;
			}
			
			ret = ir_config(ir);
			if (ret)
				break;
		
			ir_clear_fifo(ir);
			
			if (ir->chip_param.work_mode == IR_MODE_KEY) 
			{
				hiir_error("work mode is key mode!\n");
				return HI_FAILURE;
			} 
			else
			{
				writel(IR_INT_MASK_SYMBOL, ir->base + IR_INT_MASK);
            }
            
			ir_start(ir);
			ir->chip_param.module_enable = 1;
			
			break;
        }
		case CMD_IR_RESET:
		{
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
        }			
		case CMD_IR_SET_BLOCKTIME:
		{
			/* set read time out */
			ir->key_read_timeout = arg;
			break;
	    }
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
		{
			if (arg > 1 && arg < 0)
				return HI_FAILURE;

			ir->key_fetch = (enum KEY_FETCH_MODE)arg;
			
			if (ir->key_fetch != KEY_FETCH_PARSED)
				return HI_FAILURE;
				
			break;
	    }		
		case CMD_IR_SET_PROT_ENABLE: 
		{
			struct ir_protocol *ip;
			char name[PROTOCOL_NAME_SZ] = {0};

			memcpy((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1);

			name[PROTOCOL_NAME_SZ - 1] = '\0';

			ip = ir_prot_first();
			
			while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT) 
			{
				if (!strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name) > strlen(name) ?
						strlen(ip->ir_code_name): strlen(name)))
				{
					ip->disabled = 0;
					return 0;
				}
				
				ip = ir_prot_next(ip);
			}
			
			return HI_FAILURE;
		}
		case CMD_IR_SET_PROT_DISABLE: 
		{
			struct ir_protocol *ip;
			char name[PROTOCOL_NAME_SZ] = {0};

			memcpy((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1);

			name[PROTOCOL_NAME_SZ - 1] = '\0';

			ip = ir_prot_first();
			while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT) 
			{
				if (!strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name) > strlen(name) ?
						strlen(ip->ir_code_name): strlen(name))) 
				{
					ip->disabled = 1;
					return 0;
				}
				
				ip = ir_prot_next(ip);
			}

			return HI_FAILURE;
		}
		case CMD_IR_GET_PROT_ENABLED: 
		{
			struct ir_protocol *ip;
			char name[PROTOCOL_NAME_SZ] = {0};

			memcpy((void*)name, (void*)arg, PROTOCOL_NAME_SZ - 1);

			name[PROTOCOL_NAME_SZ - 1] = '\0';

			ip = ir_prot_first();
			
			while (ip && ip->ir_code_name && ip->idx != IR_PROT_BUTT) 
			{
				if (!strncmp(ip->ir_code_name, name, strlen(ip->ir_code_name) > strlen(name) ?
						strlen(ip->ir_code_name): strlen(name))) 
				{
					memcpy((void*)arg, &ip->disabled, sizeof(int)); 
					return 0;
				}
				
				ip = ir_prot_next(ip);
			}
			
			return HI_FAILURE;
		}

		default:
			ret = HI_FAILURE;
			break;
	}
	
	return ret;
}
static ssize_t ir_read(char *buf, size_t count)
{
	struct ir_priv *ir = &ir_local;
	int ret = 0;
	struct key_attr *key;


	/* NOTE: writer may run over reader,
	 * in these case should increase buffer more large.
	 * Because ir module may not generate so many data
	 * at the same time in normal case, so here simply check cursor..
	 */
	if (ir->key_buf->writer == ir->key_buf->reader) {
		return 0;
	}
	ret = 0;
	while (ir->key_buf->writer != ir->key_buf->reader && ret < count) {
		key = ir_reader_inc(ir->key_buf);

		memcpy(buf + ret, key, sizeof(*key)); 

		memset(key, 0, sizeof(*key));
		ret += sizeof(*key);
	}

	return ret;
}

static int ir_request_resource(struct ir_priv *ir)
{
	if (ir->chip_param.work_mode == IR_MODE_SYMBOL) 
	{
		ir->symbol_buf = &s_symbol_buf;
		memset(ir->symbol_buf, 0, sizeof(struct ir_buffer));
	}

	ir->key_buf = &s_key_buf; 

	memset(ir->key_buf, 0, sizeof(struct ir_buffer));

	return HI_SUCCESS;
}

static void ir_free_resource(struct ir_priv *ir)
{
    return;
}

static inline void parameter_helper_symbol(void)
{
	hiir_error("Available parameters under symbol mode are: \n"\
		"\t- freq: frequence control.\n"\
		"\t- symbol_width: max pluse + space width\n"\
		"\t- int_level: how many symbols to generate a interrupt.\n"\
		"\t- key_fetch: key fetch mode, set 1 to fetch raw symbol\n");
}

#if 0
static void ir_adjust_freq(void)
{
    ir_frequence = DFT_FREQ;

	HI_CHIP_TYPE_E chip_type;
	HI_CHIP_VERSION_E chip_version;

	get_chiptype(&chip_type, &chip_version);
	if (HI_CHIP_TYPE_HI3716M == chip_type && HI_CHIP_VERSION_V300 == chip_version)
		ir_frequence = DFT_MV300_FREQ;
	else 
		ir_frequence = DFT_FREQ;
}
#endif

static int ir_check_module_param(void)
{
	if (ir_keyhold_timeout >= 1000 || ir_frequence > 0x7f) {
		hiir_error("key hold timeout >= 0x1000"
				" or frequence > 0x7f!\n");
		return -1;
	}
	if (ir_keyhold_timeout < 150)
		hiir_error("key hold time too small,"\
			"may due to subsequence of 'DOWN,HOLD,,,UP,DOWN,UP'"\
			"while a key being press for a while\n");
	if (ir_work_mode == IR_MODE_SYMBOL) {
		if (ir_symbol_width > 0xffff
				|| ir_symbol_format > 1
				|| ir_int_level  > 0x1f
				|| ir_frequence > 0x7f
				|| ir_key_fetch_raw > 1) {
			parameter_helper_symbol();
			return -1;
		}
		return 0;
	} 

	return -1;
}

HI_S32 HI_UNF_IR_GetValueWithProtocol(enum KEY_STATUS *penPressStatus, HI_U64 *pu64KeyId, 
		        HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs)
{
	struct key_attr key;
	int ret;
	unsigned long ticks_start, ticks_now;
	u32 diff_ms;
    unsigned long  tmp_sec;
	unsigned short tmp_msec,tmp_usec;
	struct ir_priv *ir = &ir_local;

	if (!penPressStatus) {
		hiir_error("para penPressStatus is null.\n");
		return -1;
	}

	if (!pu64KeyId)	{
		hiir_error("para pu64KeyId is null.\n");
		return -1;
	}

	if (pszProtocolName && s32NameSize < PROTOCOL_NAME_SZ) {
		hiir_error("Invalid protocol buffer!\n");
		return -1;
	}
	
	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	ret = 0;

	ticks_start = do_gettime(&tmp_sec,&tmp_msec,&tmp_usec);

	do {
		proc_timer_list();

		ir_isr_symbol();

		proc_timer_list();

		ret = ir_read((char *)&key, sizeof(struct key_attr));
		if (ret == sizeof(struct key_attr))
			break;

		ticks_now = do_gettime(&tmp_sec,&tmp_msec,&tmp_usec);

		diff_ms = (u32)((ticks_now - ticks_start) / 1000);

	} while (diff_ms < u32TimeoutMs); 

	if (ret != sizeof(struct key_attr))
		return -1;

	if (pszProtocolName) {
		memcpy(pszProtocolName, key.protocol_name, (HI_U32)(s32NameSize - 1));
		pszProtocolName[s32NameSize - 1] = '\0';
	}

	*penPressStatus = key.key_stat;

	*pu64KeyId = key.lower;
	if (key.upper) {
		HI_ERR_IR("This infrared code constains more than"\
				" 64bits data!\n");
	}
	
	return 0;
}

HI_S32 HI_UNF_IR_Init(HI_VOID)
{
	int             ret;
	struct ir_priv  *pIrPriv = &ir_local;

    /*already opened*/
    if (pIrPriv->open_cnt > 0)
    {
        return HI_SUCCESS;
    }

	if (ir_work_mode != IR_MODE_SYMBOL)
	{
	    HI_ERR_IR("only support symbol mode in boot!\n");
	    return HI_FAILURE;
	}

	if (ir_check_module_param()) 
	{
		hiir_error("Invalid module parameters!\n");
		return HI_FAILURE;
	}

	memset(pIrPriv, 0, sizeof(struct ir_priv));

	pIrPriv->chip_param.work_mode = IR_MODE_SYMBOL;
	pIrPriv->chip_param.ir_max_symbol = ir_symbol_width;
	pIrPriv->chip_param.ir_max_bits = ir_int_level;
	pIrPriv->key_fetch = KEY_FETCH_PARSED;
	pIrPriv->chip_param.ir_format = ir_symbol_format;
	pIrPriv->chip_param.ir_freq = ir_frequence;

	pIrPriv->irq = IR_IRQ_NO;
	pIrPriv->base = (void *)(IR_IO_BASE);

	pIrPriv->key_hold_timeout_time = ir_keyhold_timeout;

	pIrPriv->key_read_timeout = (u32)~0; 

	pIrPriv->key_up_event = 1;
	pIrPriv->key_repeat_event = 1;
	pIrPriv->key_repeat_interval = 200;

	ret = ir_protocol_init();
	if (ret)
		goto prot;

	ret = ir_request_resource(pIrPriv);
	if (ret)
		goto prot;

	ret = ir_open();
	if (ret)
		goto reg_dev;

    pIrPriv->open_cnt++;
	
	return HI_SUCCESS;
	
reg_dev:
	ir_free_resource(pIrPriv);
prot:
	hiir_error("open ir err, ret=%#x!\n", ret);
	return ret;
}

HI_S32 HI_UNF_IR_DeInit(HI_VOID)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0 )
	{
		return 0;
	}
	
	ir_close();

	ir_protocol_exit();

	ir_free_resource(ir);
	
    ir->open_cnt--;
    
	return 0;
}


HI_S32 HI_UNF_IR_Enable(HI_BOOL bEnable)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	return ir_ioctl(CMD_IR_SET_ENABLE, bEnable) == 0 ? 0:-1;
}


HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	return ir_ioctl(CMD_IR_ENABLE_KEYUP, bEnable) == 0 ? 0 : -1;
}

HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	return ir_ioctl(CMD_IR_ENABLE_REPKEY, bEnable) == 0 ? 0 : -1;
}

HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	if (u32TimeoutMs == 0)
	{
		HI_ERR_IR("u32TimeoutMs == 0, set to 108\n");
		u32TimeoutMs = 108;
	}

	if (u32TimeoutMs > 65536)
	{
		HI_ERR_IR("prompt:u32TimeoutMs > 65536, set to 65536\n");
		u32TimeoutMs = 65536;
	}

	return ir_ioctl(CMD_IR_SET_REPKEY_TIMEOUT, u32TimeoutMs) == 0 ? 0 : -1;
}



HI_S32 HI_UNF_IR_Reset(HI_VOID)
{
	struct ir_priv *ir = &ir_local;

    if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	return ir_ioctl(CMD_IR_RESET, 0) == 0 ? 0 : -1;
}
HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	if (!pszProtocolName || !pszProtocolName[0])
	{
		return -1;
	}

	return ir_ioctl(CMD_IR_SET_PROT_ENABLE, (unsigned long)pszProtocolName) == 0 ? 0 : -1;
}

HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName)
{
	struct ir_priv *ir = &ir_local;

	if (ir->open_cnt <= 0) 
	{
		hiir_error("ir module not open!\n");
		return -1;
	}

	if (!pszProtocolName ||!pszProtocolName[0])
	{
		return -1;
	}

	return ir_ioctl (CMD_IR_SET_PROT_DISABLE, (unsigned long)pszProtocolName) == 0 ? 0 : -1;
}

HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled)
{
	HI_S32 ret;
	struct ir_priv *ir = &ir_local;
	
	union NameEnable_U
	{
		char szname[PROTOCOL_NAME_SZ];
		int enable;
	} uNameEnable;

    if (ir->open_cnt <= 0) 	
    {
		hiir_error("ir module not open!\n");
		return -1;
	}

	if (!pszProtocolName ||!pszProtocolName[0] || !pbEnabled 
			|| strlen(pszProtocolName) >= PROTOCOL_NAME_SZ)
	{
		return -1;
	}

	memcpy(uNameEnable.szname, pszProtocolName, PROTOCOL_NAME_SZ - 1);
	uNameEnable.szname[PROTOCOL_NAME_SZ - 1] = '\0';

	ret = ir_ioctl(CMD_IR_GET_PROT_ENABLED, (unsigned long)uNameEnable.szname);
	if (ret) {
		hiir_error("ioctl fail!\n");
		return -1;
	}

	if (0 == uNameEnable.enable)
	{
		*pbEnabled = HI_TRUE;
	} 
	else if (1 == uNameEnable.enable)
	{
		*pbEnabled = HI_FALSE;
	}
	else 
	{
		return -1;
	}

	return 0;
}

#ifdef HIIR_DEBUG
HI_VOID HI_UNF_IR_SetDbgOn(HI_S32 on)
{
	ir_set_dbg(on);
}
HI_VOID HI_UNF_IR_SetInfoOn(HI_S32 on)
{
	ir_set_info(on);
}
#endif
