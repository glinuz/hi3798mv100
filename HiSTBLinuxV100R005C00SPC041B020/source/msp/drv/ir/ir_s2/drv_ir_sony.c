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

#include "hi_drv_ir.h"
#include "drv_ir_protocol.h"
#include "drv_ir_utils.h"
/* store the max & min value of pluse and space to minx and maxx */
#define mm_ps(phase, minp, maxp, mins, maxs)    \
    do {                    \
        minp = (phase)->minp;       \
        maxp = (phase)->maxp;       \
        mins = (phase)->mins;       \
        maxs = (phase)->maxs;       \
    } while (0)

extern u32 g_bTimeOutFlag;
static unsigned long sony_repeat_next_time[MAX_SONY_INFR_NR];
static struct key_attr sony_last_key[MAX_SONY_INFR_NR];
static struct timer_list sony_timer[MAX_SONY_INFR_NR];
static void sony_keyup_proc(unsigned long i)
{
    struct key_attr *last_key;
    extern struct ir_priv ir_local;
    if (i >= MAX_SONY_INFR_NR) {
        hiir_error("sony keyup timer, i > MAX_SONY_INFR_NR!\n");
        return;
    }

    last_key = &sony_last_key[i];
    if ((last_key->lower || last_key->upper)
        && last_key->key_stat != KEY_STAT_UP) {
        last_key->key_stat = KEY_STAT_UP;
        if (ir_local.key_up_event) {
            ir_insert_key_tail(ir_local.key_buf, last_key);
            wake_up_interruptible(&ir_local.read_wait);
        }
        last_key->lower = last_key->upper = 0;
    }
}
void sony_init(void)
{
    int i;
    for (i = 0; i < MAX_SONY_INFR_NR; i++) {
        init_timer(&sony_timer[i]);
        sony_timer[i].expires = 0;
        sony_timer[i].data = (unsigned long)~0;
        sony_timer[i].function = sony_keyup_proc;
    }
    memset(sony_last_key, 0,
        MAX_SONY_INFR_NR * sizeof(struct key_attr));
}
void sony_exit(void)
{
    int i;
    for (i = 0; i < MAX_SONY_INFR_NR; i++)
        del_timer_sync(&sony_timer[i]);
}
#define print_mm_sp(phase, name)        \
    do {                            \
        hiir_debug("%s: minp:%d, maxp:%d, mins:%d, maxp:%d\n",  \
            name,(phase)->minp, (phase)->maxp, (phase)->mins,\
            (phase)->maxs);\
    } while(0)
/* to see a frame is a full frame or a repeat frame */
enum IR_MATCH_RESULT sony_match(enum IR_MATCH_TYPE type,
        struct ir_buffer *buf, struct ir_protocol *ip)
{
    struct key_attr *key;
    int n, idx;
    idx = ip->priv;
    if (idx >= MAX_SONY_INFR_NR) {
        hiir_error("sony , private data error!\n");
        return IR_MATCH_NOT_MATCH;
    }
    key = &buf->buf[buf->reader];
    switch (type) {
    case IR_MTT_HEADER: {
        if (!key->upper && !key->lower)
            return IR_MATCH_NOT_MATCH;
        /* Is a frame header? */
        if (!key_match_phase(key, &ip->attr.header))
            return IR_MATCH_MATCH;
        hiir_debug("%s->%d, header not match! ip at:%p"\
                " header[p, s, f]: [%d, %d, %d],"\
                " key[l, u]:[%d, %d]\n",
                __func__, __LINE__, ip,
                ip->attr.header.pluse,
                ip->attr.header.space,
                ip->attr.header.factor,
                (u32)key->lower, (u32)key->upper);
        print_mm_sp(&ip->attr.header, "header");
        return IR_MATCH_NOT_MATCH;
    }
    case IR_MTT_FRAME: {
        unsigned long long minp, maxp;
        int i, j;
        hiir_debug("idx:%d, checking symbol(at %d)[%d, %d]!\n",
                idx, buf->reader,
                (u32)buf->buf[buf->reader].lower,
                (u32)buf->buf[buf->reader].upper
                );

        if (!key->upper && !key->lower) {
            hiir_debug("%s->%d, idx:%d, key empty!\n", __func__,__LINE__,idx);
            return IR_MATCH_NOT_MATCH;
        }
        /* header match? */
        hiir_debug("%s->%d, idx:%d, checking header!\n", __func__,__LINE__,idx);
        if (key_match_phase(key, &ip->attr.header)) {
            hiir_debug("%s->%d, header not match! ip at:%p"\
                " header[p, s, f]: [%d, %d, %d],"\
                " key[l, u]:[%d, %d]\n",
                __func__, __LINE__, ip,
                ip->attr.header.pluse,
                ip->attr.header.space,
                ip->attr.header.factor,
                (u32)key->lower, (u32)key->upper);
                return IR_MATCH_NOT_MATCH;
        } else
            n = ip->attr.burst_offset;

        /* try find burst. */
        hiir_debug("%s->%d,idx:%d header match!\n",__func__,__LINE__,idx);
        n = buf->reader + n;
        if (n >= MAX_SYMBOL_NUM)
            n -= MAX_SYMBOL_NUM;
#if 1
        /* check frame symbols */
        for (i = 0, j = i + buf->reader + 1; i < ip->attr.wanna_bits && j != n;
                i ++, j ++) {

            if (j >= MAX_SYMBOL_NUM)
                j -= MAX_SYMBOL_NUM;

            if (j == n)
                break;

            key = &buf->buf[j];

            if (!key->lower && !key->upper)
                return IR_MATCH_NEED_MORE_DATA;

            /* check data phase is exceed or not */
            if (key_match_phase(key, &ip->attr.b0)
                && key_match_phase(key, &ip->attr.b1))
                return IR_MATCH_NOT_MATCH;
        }
#endif
        hiir_debug("%s->%d, checking burst at(%d)!\n",
                __func__,__LINE__,n);
        key = &buf->buf[n];
        if (!key->upper && !key->lower) {
            hiir_debug("%s->%d, idx:%d, needs more data\n",
                    __func__,__LINE__,idx);
            return IR_MATCH_NEED_MORE_DATA;
        }

        /* burst match? */
        /* SONY frame burst may constains b0's pluse or b1's pluse
         * and space will > 20000.
         */
        minp = ip->attr.b0.minp;
        maxp = ip->attr.b1.maxp;
        if (((u32)key->lower >= minp && (u32)key->lower <= maxp)
            && (u32)key->upper > ip->attr.burst.space) {

            hiir_debug("%s->%d, idx:%d, frame burst match!\n",
                    __func__,__LINE__,idx);
            return IR_MATCH_MATCH;
        } else {
            hiir_debug("%s->%d, idx:%d. burst not match!"
                    " key[l, u][%d, %d],"
                    " burst[p, s, f]: [%d, %d, %d]\n",
                    __func__,__LINE__,idx,
                    (u32)key->lower, (u32)key->upper,
                    ip->attr.burst.pluse,
                    ip->attr.burst.space,
                    ip->attr.burst.factor);

            return IR_MATCH_NOT_MATCH;
        }
    }
    case IR_MTT_BURST:
        /* fall though */
    default:
        return IR_MATCH_NOT_MATCH;
    }
}
int sony_frame_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
        struct ir_buffer *rd, struct ir_buffer *wr)
{
    struct key_attr *symbol;
    struct key_attr key;
    u32 i;
    unsigned long long minp, maxp, mins, maxs;
    struct key_attr *last_key;
    int cnt = 0, fail = 0;
    uint idx = ip->priv;
    if (idx >= MAX_SONY_INFR_NR) {
        hiir_error("sony , private data error!\n");
        goto out;
    }
    del_timer_sync(&sony_timer[idx]);
    last_key = &sony_last_key[idx];
    /* header phase */
    memset(&key, 0, sizeof(struct key_attr));
    hiir_debug("try parse header(at %d)!\n", rd->reader);
    symbol = &rd->buf[rd->reader];
    mm_ps(&ip->attr.header, minp, maxp, mins, maxs);
    /* frame start? */
    if (!data_fallin((u32)symbol->lower, minp, maxp)
         || !data_fallin((u32)symbol->upper, mins, maxs)) {
        hiir_error("SONY : cannot parse!!!"\
            " header phase not match. symbol[l,u]: [%d, %d],"\
            " header phase[p, s, f]: [%d, %d, %d]."\
            " repeat phase[p, s, f]: [%d, %d, %d].\n",
            (u32)symbol->lower, (u32)symbol->upper,
            ip->attr.header.pluse, ip->attr.header.space,
            ip->attr.header.factor,
            ip->attr.repeat.pluse, ip->attr.repeat.space,
            ip->attr.repeat.factor);
        goto out;
    }
    /* data phase */
    memcpy(key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);
    symbol = ir_next_reader_clr_inc(rd);
    hiir_debug("try parse data(at %d)!\n", rd->reader);
    i = 0;
    while (symbol->upper && symbol->lower && i < ip->attr.wanna_bits) {
        mm_ps(&ip->attr.b0, minp, maxp, mins, maxs);
        if (data_fallin((u32)symbol->upper, mins, maxs)
            && data_fallin((u32)symbol->lower, minp, maxp)) {
            symbol = ir_next_reader_clr_inc( rd);
            i ++;
            continue;
        }

        mm_ps(&ip->attr.b1, minp, maxp, mins, maxs);
        if (data_fallin((u32)symbol->upper, mins, maxs)
            && data_fallin((u32)symbol->lower, minp, maxp)) {
            if (i < 64)
                key.lower |= (unsigned long long)
                    (((unsigned long long)1) << i);
            else
                key.upper |= (unsigned long long)
                    (((unsigned long long)1) << (i - 64));
            symbol = ir_next_reader_clr_inc(rd);
            i ++;
            continue;
        }
        /* meats burst. */
        if (i + 1 == ip->attr.wanna_bits ) {
            symbol = &rd->buf[rd->reader];
            hiir_debug("%s->%d, meets burst, i:%d, ip->attr.wanna_bits:%d, rd->reader:%d!\n",
                    __func__,__LINE__, i, ip->attr.wanna_bits, rd->reader);
            mm_ps(&ip->attr.b1, minp, maxp, mins, maxs);
            if (data_fallin((u32)symbol->lower, minp, maxp)) {
                hiir_debug("%s->%d, burst constains bit1!\n",__func__,__LINE__);
                if (i < 64)
                    key.lower |= (unsigned long long)
                        (((unsigned long long)1) << i);
                else
                    key.upper |= (unsigned long long)
                        (((unsigned long long)1) << (i - 64));

                i ++;
                break;
            }
            mm_ps(&ip->attr.b0, minp, maxp, mins, maxs);
            if (data_fallin((u32)symbol->lower, minp, maxp)) {
                hiir_debug("%s->%d, burst constains bit0!\n",__func__,__LINE__);
                i ++;
                break;
            }
        }
        hiir_info("SONY : unkown symbol[l, u]: [%d, %d],"\
            " b0[p, s, f]: [%d, %d, %d],"\
            " b1[p, s, f]: [%d, %d, %d]. Assume to 0!\n",
            (u32)symbol->lower, (u32)symbol->upper,
            ip->attr.b0.pluse, ip->attr.b0.space,
            ip->attr.b0.factor,
            ip->attr.b1.pluse, ip->attr.b1.space,
            ip->attr.b1.factor);
        i ++;
        symbol = ir_next_reader_clr_inc(rd);
        fail ++;
    }
    /* burst */
    hiir_debug("try parse burst(at %d)!\n", rd->reader);
    mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
    if (!fail && ((!data_fallin((u32)symbol->lower, ip->attr.b0.minp, ip->attr.b0.maxp)
            && !data_fallin((u32)symbol->lower, ip->attr.b1.minp, ip->attr.b1.maxp))
        || (symbol->upper < maxs
            && !data_fallin((u32)symbol->upper, mins, maxs)))) {
        hiir_info("SONY : unkown symbol[l, u]: [%d, %d],"\
            " burst[p, s, f]: [%d, %d, %d].\n",
            (u32)symbol->lower, (u32)symbol->upper,
            ip->attr.burst.pluse, ip->attr.burst.space,
            ip->attr.burst.factor);
        fail ++;
        goto out;
    } else {
        if (fail) {
            (HI_VOID)ir_next_reader_clr_inc(rd);
            goto out;
        }
        /* if a repeat key. */
        if ((last_key->upper || last_key->lower)
            && last_key->upper == key.upper
            && last_key->lower == key.lower
            && last_key->key_stat != KEY_STAT_UP){
            /* should i send a repeat key? */
            if (ir->key_repeat_event && time_after(jiffies,
                    sony_repeat_next_time[idx])) {

                last_key->key_stat = KEY_STAT_HOLD;
                ir_insert_key_tail(wr, last_key);
                (HI_VOID)ir_next_reader_clr_inc(rd);
                sony_repeat_next_time[idx] = jiffies +
                    msecs_to_jiffies(
                        ir->key_repeat_interval);

                cnt ++;
            }
            goto start_timer_out;
        }
        /* a new key received.
         * send a up event of last key if exist.
         */
        if ((last_key->upper || last_key->lower)
            && last_key->key_stat != KEY_STAT_UP
            && (last_key->upper != key.upper
                || last_key->lower != key.lower)) {
            last_key->key_stat = KEY_STAT_UP;
			if (ir->key_up_event) {
                ir_insert_key_tail(wr, last_key);
            }
			cnt ++;
        }

        hiir_debug("key parsed:[l:%llu, u:%llu, s:%d, p:%s]!\n",
            key.lower, key.upper, key.key_stat, key.protocol_name);

        key.key_stat = KEY_STAT_DOWN;
        ir_insert_key_tail(wr, &key);
        (HI_VOID)ir_next_reader_clr_inc(rd);
        sony_repeat_next_time[idx] = jiffies +
            msecs_to_jiffies(ir->key_repeat_interval);

        memcpy(last_key, &key, sizeof(struct key_attr));
        cnt ++;
    }
start_timer_out:
    del_timer_sync(&sony_timer[idx]);

    if (HI_TRUE == g_bTimeOutFlag)
    {
        g_bTimeOutFlag = HI_FALSE;
		last_key->key_stat = KEY_STAT_UP;
        if (ir->key_up_event)
        {
            ir_insert_key_tail(wr, last_key);
        }
		cnt++;
        //HI_ERR_IR(" send up key cnt = %d \n", cnt);
    }
    else
    {
        if (0 == ip->key_hold_timeout_time)
        {
            sony_timer[idx].expires = jiffies +
                                 msecs_to_jiffies(ir->key_hold_timeout_time);
        }
        else
        {
            sony_timer[idx].expires = jiffies +
                                 msecs_to_jiffies(ip->key_hold_timeout_time);
        }
        
        sony_timer[idx].data = idx;
        add_timer(&sony_timer[idx]);
    }

out:
    return fail ? -1 : (cnt ? 0 : -1);
}
EXPORT_SYMBOL(sony_match);
EXPORT_SYMBOL(sony_frame_full_parse);

