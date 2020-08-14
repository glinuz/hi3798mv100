#ifndef __HIIR_H__
#define __HIIR_H__

#ifndef HI_MINIBOOT_SUPPORT
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#include <asm/atomic.h>
#include <common.h>
#include <command.h>
#include <exports.h>

#include <asm/errno.h>
#include <asm/atomic.h>
#include <asm/io.h>
#endif

#include "timer.h"
#include "hi_unf_ir.h"


#define HI_FATAL_IR(fmt...) \
	            printf(fmt)

#define HI_ERR_IR(fmt...) \
	            printf(fmt)

#define HI_WARN_IR(fmt...) \
	            printf(fmt)

#define HI_INFO_IR(fmt...) \
	            printf(fmt)

#ifdef HI_MINIBOOT_SUPPORT
typedef unsigned int u32;
typedef unsigned int uint;
typedef unsigned int size_t;
typedef unsigned int ssize_t;
#endif

/* How many NEC ir code we will support at the same time.
 * NOTES: NEC ir code means protocol is nec,
 * but infrared code is not the same, including header and burst
 * are not simalar or wanna_bits is  not the same.
 */
#define MAX_NEC_INFR_NR     (20)
#define MAX_RC6_INFR_NR     (3)
#define MAX_RC5_INFR_NR     (3)
#define MAX_SONY_INFR_NR    (3)
#define MAX_TC9012_INFR_NR  (3)
#define MAX_CREDIT_INFR_NR  (3)

/* some buit-in protocol names defination */
#define NEC_FULL_2HEADERS                       "nec full 2headers"
#define NEC_SIMPLE_2HEADERS_DISTINGUISH_REPEAT  "nec simple 2headers changshu"
#define NEC_SIMPLE_UPD6121G                     "nec simple uPD6121G like"
#define NEC_SIMPLE_LC7461MCL13                  "nec simple LC7461MCL13 like"
#define NEC_FULL_DEZHOU                         "nec full dezhou like"
#define NEC_FULL_HUIZHOU                        "nec full huizhou like"
#define NEC_SIMPLE_2HEADERS_SAME_REPEAT         "nec simple 2headers gd"
#define CREDIT_CARD                             "credit"
#define SONY_D7C8                               "sony d7c8"
#define SONY_D7C5                               "sony d7c5"
#define TC9012                                  "tc9012"
#define RC6_32BIT                               "rc6 32bit data"
#define RC6_20BIT                               "rc6 20bit data"
#define RC6_16BIT                               "rc6 16bit data"
#define RC5_14BIT                               "rc5 14bit data"
#define EXTENDED_RC5_14BIT						"extended rc5 14bit"

/* rc6's burst may at offset of x of the expect position.
 * for example: the expect burst is at 32 from the frame start.
 * but, the actural position may be appear at one of
 * [.. 29, 30, 31, 32, 33, 34, 35 ..].
 * Why would this happe?? If you know it, please tell me why :(.
 */
#define RC6_BURST_OFFSET (3)

/* the same as RC6_BURST_OFFSET */
#define RC5_BURST_OFFSET (4)

#define MAX_DATA_BITS (sizeof(unsigned long long) * 8 * 2)

#define PROTOCOL_NAME_SZ    (32)

#define IR_IO_BASE		    (0xf8001000)
/* some default parameter defines */
#define DFT_SYMBOL_WIDTH	(0x3e80)
#define DFT_SYMBOL_FMT		(0)
#define DFT_INT_LEVEL		(1)
#define DFT_FREQ		    (23)
#define DFT_MV300_FREQ		(28)

#define DFT_KEY_HOLD_TIME	(300)

/*irq number*/
#define IR_IRQ_NO           (55 + 32)

#define MAX_SYMBOL_NUM	    (100)

#define SYMBOL_PARSE_TIMEOUT	(200)

enum IR_PROTOCOL_IDX
{
	IR_PROT_COMSIMPLE,
	IR_PROT_TC9012,
	IR_PROT_COMFULL,
	IR_PROT_SONY,

	IR_PROT_RC5,
	IR_PROT_RC5X,
	IR_PROT_RC6,
	
	IR_PROT_CREDIT,

	/* add your new protocol here. */
	IR_PROT_USER_DEFINED1 = 100,
	IR_PROT_USER_DEFINED2,
	IR_PROT_USER_DEFINED3,

	IR_PROT_BUTT
};

enum IR_MODE
{
	/* chip only report keys analyzed by itself. */
	IR_MODE_KEY,
	/* chip only report symbols */
	IR_MODE_SYMBOL,
	IR_MODE_BUTT
};

/* refer to register IR_CFG. */
struct ir_chip_param
{
	unsigned int ir_max_symbol;
	/* -store infrared code type while key mode.
	 * -store symbol 'byte-order'
	 */
	unsigned int ir_format;
	unsigned int ir_max_bits;
	enum IR_MODE work_mode;
	unsigned int ir_freq;

	int module_enable;
};

struct key_attr {
	/* upper 16bit data under key mode */
	unsigned long long upper;
	/* lower 32bit data under key mode
	 * or symbol value under symbol mode
	 */
	unsigned long long lower;

	char protocol_name[PROTOCOL_NAME_SZ];

	/* indentify key status. */
	enum KEY_STATUS key_stat;
};

struct ir_buffer
{
	struct key_attr buf[MAX_SYMBOL_NUM];
	int reader;
	int writer;
};

enum KEY_FETCH_MODE 
{
	KEY_FETCH_PARSED,
	KEY_FETCH_RAW
};

typedef int wait_queue_head_t;

struct ir_priv 
{
	void                    *base;  /* register base */
	int                     irq;    /* module irq number. */
	int                     major;
	struct ir_chip_param    chip_param;
	struct ir_buffer        *symbol_buf;
	struct ir_buffer        *key_buf;  /* key buffer to store keys driver analyzed. */

	u32                     key_hold_timeout_time; /* key down-state hold timeout time, ms */

	struct timer_list       symbol_timer;

	enum KEY_FETCH_MODE     key_fetch;

	wait_queue_head_t       read_wait;  /* wait for read call while no symbol or key received. */

	u32                     key_read_timeout;   /* how many ms read call can wait. */

	int                     key_up_event;   /* enable keyup event? */
	
	int                     key_repeat_event;   /* enable repeat key event? */
	
	int                     key_repeat_interval;    /* repeat key receive interval. */

	int                     open_cnt;   /*count of open*/
};


/********************************* Ioctl definitions ************/

/* 1:check keyup */
#define CMD_IR_ENABLE_KEYUP                     0x1
/* 1:check repkey, 0:hardware behave */
#define CMD_IR_ENABLE_REPKEY                    0x2
#define CMD_IR_SET_REPKEY_TIMEOUT               0x3
/* 1:enable ir, 0:disable ir */
#define CMD_IR_SET_ENABLE                       0x4
#define CMD_IR_RESET                            0x5
#define CMD_IR_SET_BLOCKTIME                    0x6
#define CMD_IR_SET_FORMAT                       0x7
#define CMD_IR_SET_BUF                          0x8
/* raw symbol fetch(1) or key fetch(0) */
#define CMD_IR_SET_FETCH_METHOD                 0x9
/* enable or disalbe a protocol */
#define CMD_IR_SET_PROT_ENABLE                  0xa
#define CMD_IR_SET_PROT_DISABLE                 0xb
#define CMD_IR_GET_PROT_ENABLED                 0xc

#endif
