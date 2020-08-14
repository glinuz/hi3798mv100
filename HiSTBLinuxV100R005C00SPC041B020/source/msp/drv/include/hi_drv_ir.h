#ifndef __HI_DRV_IR_H__
#define __HI_DRV_IR_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_unf_ir.h"

#define HI_FATAL_IR(fmt ...)    HI_FATAL_PRINT(HI_ID_IR, fmt)
#define HI_ERR_IR(fmt ...)      HI_ERR_PRINT(HI_ID_IR, fmt)
#define HI_WARN_IR(fmt ...)     HI_WARN_PRINT(HI_ID_IR, fmt)
#define HI_INFO_IR(fmt ...)     HI_INFO_PRINT(HI_ID_IR, fmt)
#define HI_DEBUG_IR(fmt ...)    HI_DBG_PRINT(HI_ID_IR, fmt)

/* How many NEC ir code we will support at the same time.
 * NOTES: NEC ir code means protocol is nec,
 * but infrared code is not the same, including header and burst
 * are not simalar or wanna_bits is  not the same.
 */
#define MAX_NEC_INFR_NR (20)
#define MAX_RC6_INFR_NR (3)
#define MAX_RC5_INFR_NR (3)
#define MAX_SONY_INFR_NR (3)
#define MAX_TC9012_INFR_NR (3)
#define MAX_CREDIT_INFR_NR (3)

/* some buit-in protocol names defination */
#define NEC_FULL_2HEADERS "nec full 2headers"
#define NEC_SIMPLE_2HEADERS_DISTINGUISH_REPEAT "nec simple 2headers changshu"
#define NEC_SIMPLE_UPD6121G "nec simple uPD6121G like"
#define NEC_SIMPLE_LC7461MCL13 "nec simple LC7461MCL13 like"
#define NEC_FULL_DEZHOU "nec full dezhou like"
#define NEC_FULL_HUIZHOU "nec full huizhou like"
#define NEC_SIMPLE_2HEADERS_SAME_REPEAT "nec simple 2headers gd"
#define CREDIT_CARD "credit"
#define SONY_D7C8 "sony d7c8"
#define SONY_D7C5 "sony d7c5"
#define TC9012 "tc9012"
#define RC6_32BIT "rc6 32bit data"
#define RC6_20BIT "rc6 20bit data"
#define RC6_16BIT "rc6 16bit data"
#define RC5_14BIT "rc5 14bit data"
#define EXTENDED_RC5_14BIT "extended rc5 14bit"

/* rc6's burst may at offset of x of the expect position.
 * for example: the expect burst is at 32 from the frame start.
 * but, the actural position may be appear at one of
 * [.. 29, 30, 31, 32, 33, 34, 35 ..].
 * Why would this happe?? If you know it, please tell me why :(.
 */
#define RC6_BURST_OFFSET (4)

/* the same as RC6_BURST_OFFSET */
#define RC5_BURST_OFFSET (4)

/* irq define */
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)	\
    || defined(CHIP_TYPE_hi3798cv200)   \
	|| defined(CHIP_TYPE_hi3798mv200)   \
	|| defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a)	\
    || defined(CHIP_TYPE_hi3798hv100)
#define IR_IRQ_NO   (47 + 32)
#define IR_IO_BASE  (0xf8001000)
#else
#error YOU MUST DEFINE  CHIP_TYPE!
#endif


/* some default parameter defines */
#define DFT_SYMBOL_WIDTH (0x3e80)
#define DFT_SYMBOL_FMT (0)
#define DFT_INT_LEVEL (1)
#define DFT_FREQ (23)
#define DFT_MV300_FREQ (28)

#define MAX_DATA_BITS (sizeof(unsigned long long) * 8 * 2)
#define PROTOCOL_NAME_SZ (32)

#define IR_DEVICE_NAME_SZ (64)

/* the max bits of an infrared code we have meet is 48,
 * set this macro to 100 may hold 2 key press event.
 */
#define MAX_SYMBOL_NUM (500)

#define SYMBOL_PARSE_TIMEOUT (200)

#define IR_ADDR_SIZE    0x1000

enum KEY_FETCH_MODE
{
    KEY_FETCH_PARSED,
    KEY_FETCH_RAW
};

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

enum KEY_STATUS
{
    KEY_STAT_DOWN,
    KEY_STAT_HOLD,
    KEY_STAT_UP,
    KEY_STAT_BUTT
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
};

struct key_attr
{
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

/* for ir_std */
typedef struct
{
    HI_U32               IrKeyDataH;
    HI_U32               IrKeyDataL;
    HI_UNF_IR_PROTOCOL_E IrProtocol;
    HI_UNF_KEY_STATUS_E  IrKeyState;
}IR_KEY_S;

#ifdef __KERNEL__

struct ir_buffer
{
    struct key_attr buf[MAX_SYMBOL_NUM];
    int             reader;
    int             writer;
};

typedef irqreturn_t (*isr_proc)(int irq, void *dev_id);
typedef void (*data_proc)(unsigned long data);
struct ir_callbacks
{
    isr_proc  isr_handle;
    data_proc data_handle;
};

struct ir_priv
{
    /* register base */
    void * __iomem base;

    /* module irq number. */
    int      irq;
    int      major;
    char     dev_name[IR_DEVICE_NAME_SZ];
    atomic_t refcnt;

    struct ir_chip_param chip_param;

    struct ir_buffer *symbol_buf;

    /* key buffer to store keys driver analyzed. */
    struct ir_buffer *key_buf;

    /* key down-state hold timeout time, ms */
    u32 key_hold_timeout_time;

    struct timer_list symbol_timer;

    enum KEY_FETCH_MODE key_fetch;

    /* wait for read call while no symbol or key received. */
    wait_queue_head_t read_wait;

    /* how many ms read call can wait. */
    u32 key_read_timeout;

    /* only allow one user to fetch keys or symbols. */
    struct semaphore sem;

    struct ir_callbacks   callbacks;
    struct tasklet_struct ir_bh;

    /* enable keyup event? */
    int key_up_event;

    /* enable repeat key event? */
    int key_repeat_event;

    /* repeat key receive interval. */
    int key_repeat_interval;

    /* count of tring to get key */
    u32 key_debug_trycount;

    /* count of succeed to get key */
    u32 key_debug_succeedcount;

};
#endif

HI_S32 HI_DRV_IR_Init(HI_VOID);
HI_S32 HI_DRV_IR_DeInit(HI_VOID);
HI_S32 HI_DRV_IR_Enable(HI_BOOL bEnable);
HI_S32 HI_DRV_IR_GetValueWithProtocol(enum KEY_STATUS *penPressStatus, HI_U64 *pu64KeyId,
                                      HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs);
HI_S32 HI_DRV_IR_EnableKeyUp(HI_BOOL bEnable);
HI_S32 HI_DRV_IR_EnableRepKey(HI_BOOL bEnable);
HI_S32 HI_DRV_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs);
HI_S32 HI_DRV_IR_SetFetchMode(HI_S32 s32Mode);

#endif /* __HI_DRV_IR_H__ */
