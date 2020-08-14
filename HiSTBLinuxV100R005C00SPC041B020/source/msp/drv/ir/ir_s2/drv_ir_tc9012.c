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
static unsigned long tc9012_repeat_next_time[MAX_TC9012_INFR_NR];
static struct key_attr tc9012_last_key[MAX_TC9012_INFR_NR];
static struct timer_list tc9012_timer[MAX_TC9012_INFR_NR];
static void tc9012_keyup_proc(unsigned long i)
{
    struct key_attr *last_key;
    extern struct ir_priv ir_local;
    if (i >= MAX_TC9012_INFR_NR) {
        hiir_error("tc9012 keyup timer, i > MAX_TC9012_INFR_NR!\n");
        return;
    }

    last_key = &tc9012_last_key[i];
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
void tc9012_init(void)
{
    int i;
    for (i = 0; i < MAX_TC9012_INFR_NR; i++) {
        init_timer(&tc9012_timer[i]);
        tc9012_timer[i].expires = 0;
        tc9012_timer[i].data = (unsigned long)~0;
        tc9012_timer[i].function = tc9012_keyup_proc;
    }
    memset(tc9012_last_key, 0,
        MAX_TC9012_INFR_NR * sizeof(struct key_attr));
}
void tc9012_exit(void)
{
    int i;
    for (i = 0; i < MAX_TC9012_INFR_NR; i++)
        del_timer_sync(&tc9012_timer[i]);
}
#define print_mm_sp(phase, name)        \
    do {                            \
        hiir_debug("%s: minp:%d, maxp:%d, mins:%d, maxp:%d\n",  \
            name,(phase)->minp, (phase)->maxp, (phase)->mins,\
            (phase)->maxs);\
    } while(0)
/* to see a frame is a full frame or a repeat frame */
enum IR_MATCH_RESULT tc9012_match(enum IR_MATCH_TYPE type,
        struct ir_buffer *buf, struct ir_protocol *ip)
{
    struct key_attr *key;
    int n, idx;
    idx = ip->priv;
    if (idx >= MAX_TC9012_INFR_NR) {
        hiir_error("tc9012 , private data error!\n");
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
        /* repeate frame match? */
        if (!ip->attr.repeat.pluse && !ip->attr.repeat.space)
            return IR_MATCH_NOT_MATCH;
        if (!key_match_phase(key, &ip->attr.repeat))
            return IR_MATCH_MATCH;
        hiir_debug("%s->%d, repeat not match! ip at:%p"\
                " repeat[p, s, f]: [%d, %d, %d],"\
                " key[l, u]:[%d, %d]\n",
                __func__, __LINE__, ip,
                ip->attr.repeat.pluse,
                ip->attr.repeat.space,
                ip->attr.repeat.factor,
                (u32)key->lower, (u32)key->upper);

        print_mm_sp(&ip->attr.repeat, "repeat");
        return IR_MATCH_NOT_MATCH;
    }
    case IR_MTT_FRAME: {
        unsigned long long minp, maxp, mins, maxs;
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
        }
        /* repeate frame match? */
        if (!ip->attr.repeat.pluse && !ip->attr.repeat.space)
            return IR_MATCH_NOT_MATCH;
        hiir_debug("%s->%d, idx:%d, checking repeat!\n", __func__,__LINE__,idx);
        if (key_match_phase(key, &ip->attr.repeat)) {
            hiir_debug("%s->%d, idx:%d, repeat not match!\n",
                    __func__, __LINE__,idx);
            return IR_MATCH_NOT_MATCH;
        } else  {
            /* if this is repeate key, then check burst here. */
            n = 2;

            n = buf->reader + n;
            if (n >= MAX_SYMBOL_NUM)
                n -= MAX_SYMBOL_NUM;


            key = &buf->buf[n];
            hiir_debug("%s->%d, idx:%d, checking repeat burst(at %d)[%d, %d]!\n",
                    __func__,__LINE__,idx, n, (u32)key->lower, (u32)key->upper);
            if (!key->upper && !key->lower) {
                hiir_debug("%s->%d, checking repeat burst, need more symbols!\n",
                        __func__,__LINE__);
                return IR_MATCH_NEED_MORE_DATA;
            }
            mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
            if (data_fallin((u32)key->lower, minp, maxp)
                    && (((u32)key->upper > ip->attr.burst.maxs)
                        || data_fallin((u32)key->upper, mins, maxs))) {
                /* case a frame only constains repeate key, no first frame,
                 * we cannot recognize the key value, so dicard this repeat key.
                 */
                if ((tc9012_last_key[idx].lower || tc9012_last_key[idx].upper)
                        && tc9012_last_key[idx].key_stat != KEY_STAT_UP) {

                    hiir_debug("%s->%d, idx:%d, repeat burst match!\n",
                            __func__,__LINE__,idx);
                    return IR_MATCH_MATCH;
                } else  {
                    hiir_debug("%s->%d, idx:%d, repeat burst not match!\n",
                            __func__,__LINE__,idx);
                    return IR_MATCH_NOT_MATCH;
                }
            }
        }
        /* the first frame */
        n = ip->attr.wanna_bits + 1;
        /* try find burst. */
        hiir_debug("%s->%d,idx:%d header match!\n",__func__,__LINE__,idx);
        n = buf->reader + n;
        if (n >= MAX_SYMBOL_NUM)
            n -= MAX_SYMBOL_NUM;

        hiir_debug("%s->%d, checking burst at(%d)!\n",
                __func__,__LINE__,n);
        key = &buf->buf[n];
        if (!key->upper && !key->lower) {
            hiir_debug("%s->%d, idx:%d, needs more data\n",
                    __func__,__LINE__,idx);
            return IR_MATCH_NEED_MORE_DATA;
        }

        /* burst match? */
        /* TC9012 frame burst may fall in [space ,8%],
         * but it still can greater than space !
         */
        mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
        if (data_fallin((u32)key->lower, minp, maxp)
            && (((u32)key->upper > ip->attr.burst.maxs)
                || data_fallin((u32)key->upper, mins, maxs))) {
            hiir_debug("%s->%d, checking frame!\n",
                    __func__,__LINE__);
            /* check frame symbols */
            for (i = 0, j = i + buf->reader + 1; i < ip->attr.wanna_bits && j != n;
                    i ++, j ++) {

                if (j >= MAX_SYMBOL_NUM)
                    j -= MAX_SYMBOL_NUM;

                if (j == n)
                    break;

                key = &buf->buf[j];

                if (!key->lower && !key->upper) {
                    hiir_error("%s->%d, need more symbols BUT BURST appears!\n",
                            __func__,__LINE__);
                    return IR_MATCH_NEED_MORE_DATA;
                }

                /* check data phase is exceed or not */
                if (key_match_phase(key, &ip->attr.b0)
                    && key_match_phase(key, &ip->attr.b1)) {
                    hiir_debug("%s->%d, symbol[%d, %d] not exceed"\
                            " b0 or b1's max space or pluse!\n",
                            __func__,__LINE__,
                            (u32)key->upper,(u32)key->lower);
                    return IR_MATCH_NOT_MATCH;
                }
            }
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
int tc9012_frame_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
        struct ir_buffer *rd, struct ir_buffer *wr)
{
    struct key_attr *symbol;
    struct key_attr *symbol2;
    struct key_attr key;
    unsigned long long minp, maxp, mins, maxs;
    unsigned i, n;
    struct key_attr *last_key;
    int cnt = 0, fail = 0;
    uint idx = ip->priv;
    if (idx >= MAX_TC9012_INFR_NR) {
        hiir_error("tc9012 , private data error!\n");
        goto out;
    }
    del_timer_sync(&tc9012_timer[idx]);
    last_key = &tc9012_last_key[idx];
    /* header phase */
    memset(&key, 0, sizeof(struct key_attr));
    hiir_debug("try parse header(at %d)!\n", rd->reader);
    symbol = &rd->buf[rd->reader];

    mm_ps(&ip->attr.header, minp, maxp, mins, maxs);
    /* frame start? */
    if (!data_fallin((u32)symbol->lower, minp, maxp)
         || !data_fallin((u32)symbol->upper, mins, maxs)) {
        /* repeate key? */
        mm_ps(&ip->attr.repeat, minp, maxp, mins, maxs);
        if (data_fallin((u32)symbol->lower, minp, maxp)
            && data_fallin((u32)symbol->upper, mins, maxs))
            goto repeat_key;

        hiir_error("TC9012 : cannot parse!!!"\
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
    /* checking burst. if a repeat frame, burst will appear at current pos + 2. */
    if (rd->reader + 2 >= MAX_SYMBOL_NUM)
        n = rd->reader + 2 - MAX_SYMBOL_NUM;
    else
        n = rd->reader + 2;

    symbol2 = &rd->buf[n];
    hiir_debug("%s->%d, checking burst at %d [%d, %d]\n",
            __func__,__LINE__, n, (u32)symbol2->lower, (u32)symbol2->upper);

    mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
    if (data_fallin((u32)symbol2->lower, minp, maxp)
        && (symbol2->upper > maxs
            || data_fallin((u32)symbol2->upper, mins, maxs))) {
        goto repeat_key;
    }

    /* data phase */
    memcpy(key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);
    symbol = ir_next_reader_clr_inc(rd);
    hiir_debug("try parse data(at %d)!\n", rd->reader);
    i = 0;
    while (symbol->upper && symbol->lower && i < ip->attr.wanna_bits) {
        /* bit 0? */
        mm_ps(&ip->attr.b0, minp, maxp, mins, maxs);
        if (data_fallin((u32)symbol->lower, minp, maxp)
            && data_fallin((u32)symbol->upper, mins, maxs)) {
            symbol = ir_next_reader_clr_inc( rd);
            i ++;
            continue;
        }
        /* bit 1? */
        mm_ps(&ip->attr.b1, minp, maxp, mins, maxs);
        if (data_fallin((u32)symbol->lower, minp, maxp)
            && data_fallin((u32)symbol->upper, mins,maxs)) {
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
        hiir_info("%s->%d, i:%d,TC9012 : unkown symbol[l, u]: [%d, %d],"\
            " b0[p, s, f]: [%d, %d, %d],"\
            " b1[p, s, f]: [%d, %d, %d]. Discard this frame!\n",
            __func__,__LINE__, i,
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
    if (!data_fallin((u32)symbol->lower, minp, maxp)
        || (symbol->upper < maxs
            && !data_fallin((u32)symbol->upper, mins, maxs))) {
        hiir_info("%s->%d, TC9012 : unkown symbol[l, u]: [%d, %d],"\
            " burst[p, s, f]: [%d, %d, %d].\n",
            __func__,__LINE__,
            (u32)symbol->lower, (u32)symbol->upper,
            ip->attr.burst.pluse, ip->attr.burst.space,
            ip->attr.burst.factor);
        fail ++;
        goto out;
    } else {
        if (!fail && (last_key->upper || last_key->lower)
                && last_key->key_stat != KEY_STAT_UP) {
            last_key->key_stat = KEY_STAT_UP;
            if (ir->key_up_event) {
                ir_insert_key_tail(wr, last_key);
            }
			cnt ++;
        }
        key.key_stat = KEY_STAT_DOWN;

        hiir_debug("key parsed:[l:0x%llx, u:0x%llx, s:%d, p:%s]!\n",
            key.lower, key.upper, key.key_stat, key.protocol_name);
        if (!fail) {
            ir_insert_key_tail(wr, &key);
            cnt ++;
            tc9012_repeat_next_time[idx] = jiffies +
                msecs_to_jiffies(ir->key_repeat_interval);

            memcpy(last_key, &key, sizeof(struct key_attr));
            hiir_debug("idx:%d, key parsed ,last_key[l,u,s] [0x%x,0x%x,%d],idx:%d\n",
                idx, (u32)last_key->lower,(u32)last_key->upper,
                    last_key->key_stat,
                    idx);
        }
        symbol = ir_next_reader_clr_inc(rd);
    }
repeat_key:
    /* repeat? */
    do {
        enum IR_MATCH_RESULT r;
        hiir_debug("idx:%d,try parse repeat header(at %d)[%d, %d]!\n",
                idx, rd->reader,
                (u32)ir->symbol_buf->buf[rd->reader].lower,
                (u32)ir->symbol_buf->buf[rd->reader].upper
                );
        /* checking a full repeat frame received or not. */
        r = ip->match(IR_MTT_FRAME, ir->symbol_buf, ip);
        if (IR_MATCH_MATCH != r){
            hiir_debug("%s->%d, idx:%d repeat frame not match\n",
                    __func__, __LINE__, idx);
            //if (IR_MATCH_NEED_MORE_DATA == r)
        //      goto out;
        //  else
                goto start_timer_out;
        }
        mm_ps(&ip->attr.repeat, minp, maxp, mins, maxs);
        if (!data_fallin((u32)symbol->lower, minp, maxp)
            || !data_fallin((u32)symbol->upper, mins, maxs)) {
            hiir_debug("%s->%d, repeat burst not match!\n",
                    __func__,__LINE__);
            goto start_timer_out;
        }

        /* clear repeat header */
        (HI_VOID)ir_next_reader_clr_inc(rd);
        /* skip C0 bit in the repeat frame. */
        symbol = ir_get_read_key_n(rd, 1);
        /* burst */
        hiir_debug("%s->%d,idx:%d, try parse repeat burst(at %d)[%d, %d]!\n",
                __func__,__LINE__,
                idx,rd->reader, (u32)symbol->lower, (u32)symbol->upper);
        /* TC9012 burst's space may fallin [space, 8%], but it also
         * may greate than maxp.
         */
        mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
        if (!data_fallin((u32)symbol->lower, minp, maxp)
            || (symbol->upper < maxs
                && !data_fallin((u32)symbol->upper, mins, maxs))){
            hiir_debug("TC9012 : repeat burst not match"\
                "[l, u]: [%d, %d],"\
                " burst[p, s, f]: [%d, %d, %d].\n",
                (u32)symbol->lower, (u32)symbol->upper,
                ip->attr.burst.pluse, ip->attr.burst.space,
                ip->attr.burst.factor);
            /* clear C0 bit in the repeat frame. */
            (HI_VOID)ir_next_reader_clr_inc(rd);
        } else {
            /* clear C0 bit in the repeat frame. */
            (HI_VOID)ir_next_reader_clr_inc(rd);
            hiir_debug("repeat parsed ,last_key[l,u,s] [0x%x,0x%x,%d],"
                    "time_after:%d,idx:%d!\n",
                    (u32)last_key->lower,(u32)last_key->upper,
                    last_key->key_stat,
                    time_after(jiffies,tc9012_repeat_next_time[idx]),
                    idx);
            if (!fail && (last_key->upper || last_key->lower)
                && ir->key_repeat_event
                && time_after(jiffies,
                    tc9012_repeat_next_time[idx])) {
                hiir_debug("insert repeat parsed !\n");

                last_key->key_stat =
                    last_key->key_stat == KEY_STAT_UP ?
                        KEY_STAT_DOWN : KEY_STAT_HOLD;

                ir_insert_key_tail(wr, last_key);
                tc9012_repeat_next_time[idx] = jiffies +
                    msecs_to_jiffies(
                        ir->key_repeat_interval);
                cnt ++;
            }
        }
        symbol = ir_next_reader_clr_inc(rd);
    } while(symbol->upper && symbol->lower);
start_timer_out:
    del_timer_sync(&tc9012_timer[idx]);

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
            tc9012_timer[idx].expires = jiffies +
                                 msecs_to_jiffies(ir->key_hold_timeout_time);
        }
        else
        {
            tc9012_timer[idx].expires = jiffies +
                                 msecs_to_jiffies(ip->key_hold_timeout_time);
        }

        tc9012_timer[idx].data = idx;
        add_timer(&tc9012_timer[idx]);
    }
out:
    return fail ? -1 : (cnt ? 0 : -1);
}
EXPORT_SYMBOL(tc9012_frame_simple_parse);
EXPORT_SYMBOL(tc9012_match);

