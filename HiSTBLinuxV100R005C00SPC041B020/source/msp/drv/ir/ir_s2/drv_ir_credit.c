/* Credit frame constains start bit, N bit data, stop bit, one fill bit.
 * Here we use start bit as a header, fill bit as burst.
 * We will ignore stop bit will parse the frame. 
 * And the descripter array's wanna_bits should be set to N.
 */
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
#define mm_ps(phase, minp, maxp, mins, maxs)	\
	do {					\
		minp = (phase)->minp;		\
		maxp = (phase)->maxp;		\
		mins = (phase)->mins;		\
		maxs = (phase)->maxs;		\
	} while (0)

void credit_init(void)
{
}
void credit_exit(void)
{
}
#define print_mm_sp(phase, name)        \
	do {                            \
		hiir_debug("%s: minp:%d, maxp:%d, mins:%d, maxp:%d\n",  \
			name,(phase)->minp, (phase)->maxp, (phase)->mins,\
			(phase)->maxs);\
	} while(0)

/* to see a frame is a full frame or a repeat frame */
enum IR_MATCH_RESULT credit_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip)
{
	struct key_attr *key;
	int n, idx;
	idx = ip->priv;
	if (idx >= MAX_CREDIT_INFR_NR) {
		hiir_error("credit, private data error!\n");
		return IR_MATCH_NOT_MATCH;
	}
	key = &buf->buf[buf->reader];
	switch (type) {
	case IR_MTT_HEADER: {
		if (!key->upper && !key->lower)
			return IR_MATCH_NOT_MATCH;
		hiir_debug("%s->%d, idx:%d, checking header at %d[%d, %d]!\n", __func__,__LINE__,idx,
				buf->reader, (u32)key->lower, (u32)key->upper);
		/* Is a frame header? */
		if (!key_match_phase(key, &ip->attr.header))
			return IR_MATCH_MATCH;
		hiir_debug("%s->%d, idx:%d, header at %d [%d, %d] not match [%d, %d, %d]!\n", __func__,__LINE__,idx,
				buf->reader, (u32)key->lower, (u32)key->upper,ip->attr.header.pluse,
				ip->attr.header.space, ip->attr.header.factor);
		return IR_MATCH_NOT_MATCH;
	}
	case IR_MTT_FRAME: {
		unsigned long long minp, maxp, mins, maxs;
		int i, j;
		hiir_debug("idx:%d, checking symbol(at %d)[%d, %d]!\n",
				idx, buf->reader,
				(u32)buf->buf[buf->reader].lower,
				(u32)buf->buf[buf->reader].upper);

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
			n = ip->attr.wanna_bits + 1;
		/* checking stop bit. */
		hiir_debug("%s->%d,idx:%d checking stop bit!\n",__func__,__LINE__,idx);

		n = buf->reader + n;
		if (n >= MAX_SYMBOL_NUM)
			n -= MAX_SYMBOL_NUM;

		key = &buf->buf[n];
		if (!key->upper && !key->lower) {
			hiir_debug("%s->%d, idx:%d, needs more data\n",
					__func__,__LINE__,idx);
			return IR_MATCH_NEED_MORE_DATA;
		}

		if (key_match_phase(key, &ip->attr.b1)) {
			hiir_debug("%s->%d, stop bit not match!\n",__func__,__LINE__);
			return IR_MATCH_NOT_MATCH;
		}
		/* try find burst. */
		hiir_debug("%s->%d,idx:%d header match!\n",__func__,__LINE__,idx);
		n = ip->attr.wanna_bits + 2;
		n = buf->reader + n;
		if (n >= MAX_SYMBOL_NUM)
			n -= MAX_SYMBOL_NUM;
		
		key = &buf->buf[n];
		hiir_debug("%s->%d, checking burst at(%d) [%d, %d]!\n",
				__func__,__LINE__,n, (u32)key->lower, (u32)key->upper);
		if (!key->upper && !key->lower) {
			hiir_debug("%s->%d, idx:%d, needs more data\n",
					__func__,__LINE__,idx);
			return IR_MATCH_NEED_MORE_DATA;
		}
		
		/* burst match? */
		/* CREDIT frame burst may fall in [space ,8%],
		 * but it still can greater than space !
		 */
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		if (!key_match_phase(key, &ip->attr.burst)
			|| (data_fallin((u32)key->lower, minp, maxp)
				&& key->upper >= 20000)) {

			hiir_debug("%s->%d, idx:%d, frame burst match!\n",
					__func__,__LINE__,idx);
			/* check frame symbols */
			for (i = 0, j = i + buf->reader + 1; i < ip->attr.wanna_bits && j != n; 
					i ++, j ++) {

				if (j >= MAX_SYMBOL_NUM)
					j -= MAX_SYMBOL_NUM;

				if (j == n)
					break;

				key = &buf->buf[j];

				if (!key->lower && !key->upper) {
					hiir_error("%s->%d, need more symbols !\n",
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
int credit_frame_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct key_attr *symbol;
	struct key_attr key;
	u32 i;
	unsigned long long minp, maxp, mins, maxs;
	int cnt = 0, fail = 0;
	uint idx = ip->priv;
	if (idx >= MAX_CREDIT_INFR_NR) {
		hiir_error("credit, private data error!\n");
		goto out;
	}
	/* header phase */
	memset(&key, 0, sizeof(struct key_attr));
	hiir_debug("try parse header(at %d)!\n", rd->reader);
	symbol = &rd->buf[rd->reader];
	mm_ps(&ip->attr.header, minp, maxp, mins, maxs);
	/* frame start? */
	if (!data_fallin((u32)symbol->lower, minp, maxp)
		 || !data_fallin((u32)symbol->upper, mins, maxs)) {
		hiir_debug("CREDIT : cannot parse!!!"\
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
	/* + 1 to skip stop bits */
	while (symbol->upper && symbol->lower && i < ip->attr.wanna_bits + 1) {
		/* skip stop bit */
		if (i == ip->attr.wanna_bits) {
			symbol = ir_next_reader_clr_inc( rd);
			break;
		}

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
		hiir_info("%s->%d, CREDIT : unkown symbol[l, u]: [%d, %d],"\
			" b0[p, s, f]: [%d, %d, %d],"\
			" b1[p, s, f]: [%d, %d, %d]. Assume to 0!\n",
			__func__,__LINE__,
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

		hiir_info("%s->%d, CREDIT : unkown symbol[l, u]: [%d, %d],"\
			" burst[p, s, f]: [%d, %d, %d].\n",
			__func__,__LINE__,
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.burst.pluse, ip->attr.burst.space,
			ip->attr.burst.factor);
		fail ++;
		goto out;
	} else {
		if (fail) {
			symbol = ir_next_reader_clr_inc(rd);
			goto out;
		}


		key.key_stat = KEY_STAT_DOWN;
		memcpy(key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);

		hiir_debug("key parsed:[l:%llu, u:%llu, s:%d, p:%s]!\n",
			key.lower, key.upper, key.key_stat, key.protocol_name);

		ir_insert_key_tail(wr, &key);

		symbol = ir_next_reader_clr_inc(rd);

		cnt ++;
	}
out:
	return fail ? -1 : (cnt ? 0 : -1);
}
EXPORT_SYMBOL(credit_match);
EXPORT_SYMBOL(credit_frame_parse);

