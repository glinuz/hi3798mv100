#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif
#include "ir.h"
#include "ir_protocol.h"
#include "ir_utils.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "errno.h"
#endif

#define RC5_NBITS		14
#define CHECK_RC5X_NBITS	8
#define RC5_UNIT		889 /* us */
#define RC5_HEADER_PULSE	(2 * RC5_UNIT)

#define RC5_BIT_START		(1 * RC5_UNIT)
#define RC5_BIT_END		(1 * RC5_UNIT)
#define RC5X_SPACE		(4 * RC5_UNIT)

enum RC5_STATE {
	STATE_INACTIVE,		//0
	STATE_BIT_START,	//1
	STATE_BIT_END,		//2
	STATE_CHECK_RC5X,	//3
	STATE_FINISHED,		//4
};

struct rc5_ir {		
	enum RC5_STATE state;		
	unsigned long long bits;
	unsigned long long scancode;		
	int count;
	struct key_attr this_key;
	struct key_attr last_key;
	int has_last_key;
	struct ir_signal prev_signal;
};
static struct rc5_ir rc5_data[MAX_RC5_INFR_NR];
static unsigned long rc5_repeat_next_time[MAX_RC5_INFR_NR];
static struct timer_list rc5_timer[MAX_RC5_INFR_NR];
static void rc5_keyup_proc(unsigned long i)
{
	struct key_attr *last_key;
	extern struct ir_priv ir_local;
	if (i >= MAX_RC5_INFR_NR) {
		hiir_error("rc5 keyup timer, i > MAX_RC5_INFR_NR!\n");
		return;
	}

	last_key = &rc5_data[i].last_key;
	if ( rc5_data[i].has_last_key
			&& last_key->key_stat != KEY_STAT_UP) {
		last_key->key_stat = KEY_STAT_UP;
		if (ir_local.key_up_event) {
			ir_insert_key_tail(ir_local.key_buf, last_key);
			wake_up_interruptible(&ir_local.read_wait);
		}
	}
}
void rc5_init(void)
{
	int i;
	for (i = 0; i < MAX_RC5_INFR_NR; i++) {
		init_timer(&rc5_timer[i]);
		rc5_timer[i].expires = 0;
		rc5_timer[i].data = (unsigned long)~0;
		rc5_timer[i].function = rc5_keyup_proc;
	}
	memset(rc5_data, 0, sizeof(struct rc5_ir) * MAX_RC5_INFR_NR);
}
void rc5_exit(void)
{
	int i;
	for (i = 0; i < MAX_RC5_INFR_NR; i++)
		del_timer(&rc5_timer[i]);
}
static int ir_rc5_decode(struct rc5_ir *data, struct ir_signal signal, struct ir_protocol *ip)
{
	int i = ip->priv;
	if (i > MAX_RC5_INFR_NR)
		goto out;

	if (!geq_margin(signal.duration, RC5_UNIT, RC5_UNIT / 2))
		goto out;

again:
	hiir_debug("RC5 decode started at state %i (%uus %s)\n",
			data->state, signal.duration, TO_STR(signal.pulse));

	if (!geq_margin(signal.duration, RC5_UNIT, RC5_UNIT / 2))
		return 0;

	switch (data->state) {

	case STATE_INACTIVE:
		if (!signal.pulse)
			break;

		data->state = STATE_BIT_START;
		data->count = 1;
		data->bits = 0;
		memset(&data->this_key, 0, sizeof(struct key_attr));
		/* We just need enough bits to get to STATE_CHECK_RC5X */
		decrease_duration(&signal, RC5_BIT_START);
		goto again;

	case STATE_BIT_START:
		if (!eq_margin(signal.duration, RC5_BIT_START, RC5_UNIT / 2))
			break;

		data->bits <<= 1;
		if (!signal.pulse)
			data->bits |= 1;
		data->count++;

		data->state = STATE_BIT_END;
		return 0;

	case STATE_BIT_END:
		if (!is_transition(&signal, &data->prev_signal))
			break;

		if (data->count == ip->attr.wanna_bits) {
			data->scancode = data->bits;
			data->this_key.lower = data->scancode & 0x7ff;
			data->this_key.upper = 0;

			data->state = STATE_FINISHED;
		}
		else if (data->count == CHECK_RC5X_NBITS)
		{
			data->state = STATE_CHECK_RC5X;
		}
		else
		{
			data->state = STATE_BIT_START;
		}
		decrease_duration(&signal, RC5_BIT_END);
		goto again;

	case STATE_CHECK_RC5X:
		
		data->state = STATE_BIT_START;
		goto again;

	case STATE_FINISHED:
		if (signal.pulse){
			break;
		}

		data->state = STATE_INACTIVE;
		return 0;
	}

out:
	hiir_info("RC5 decode failed at state %i (%uus %s), data->count:%d\n",
			   data->state, signal.duration, TO_STR(signal.pulse), data->count);
	data->state = STATE_INACTIVE;
	return -EINVAL;
}

/* call condition:
 * if rc5_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT rc5_match_error_handle(enum IR_MATCH_TYPE type,
		struct ir_buffer *head,
		struct ir_protocol *ip)
{
	struct key_attr *symbol;
	int n, i;
	symbol = &head->buf[head->reader];
	if (!symbol->upper && !symbol->lower)
		return IR_MATCH_NOT_MATCH;
	switch (type) {
	case IR_MTT_HEADER:
		hiir_debug("rc5, header matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (!key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		hiir_debug("rc5, header matching: header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc5, frame matching: header mismatch!\n");
			return IR_MATCH_NOT_MATCH;
		}
		hiir_debug("rc5, frame matching, header match at:%d, [l, u]:[%d, %d]\n",
			head->reader, (u32)symbol->lower, (u32)symbol->upper);
                /* try find burst. */
		for (i = head->reader + 1, n = 0; n < MAX_DATA_BITS; n++, i++) {
			if (i >= MAX_SYMBOL_NUM)
				i -= MAX_SYMBOL_NUM;
			symbol = &head->buf[i];
			if (!symbol->upper && !symbol->lower) {
				hiir_debug("%s->%d, needs more data\n",
						__func__,__LINE__);
				return IR_MATCH_NEED_MORE_DATA;
			}
			hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d] at %d,"\
					"burst:[p, s, f]:[%d, %d, %d]\n",
					(u32)symbol->lower, (u32)symbol->upper, i,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
			/* ignore lower symbol. */
			if (symbol->upper > ip->attr.burst.maxs
					|| data_fallin(symbol->upper,
						ip->attr.burst.mins,
						ip->attr.burst.maxs)) {
				hiir_debug("rc5, frame matching: burst match at %d. symbol[l,u]:[%d, %d],"\
					"burst:[p, s, f]:[%d, %d, %d]\n", i,
					(u32)symbol->lower, (u32)symbol->upper,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
				if (n >= ip->attr.burst_offset)
					n = n - ip->attr.burst_offset;
				else
					n = ip->attr.burst_offset - n;
				if (n <= RC5_BURST_OFFSET) {
					hiir_debug(KERN_DEBUG"%s->%d, rc5 frame err matching :"\
							" burst at %d, (n=%d), frame match!\n",
							__func__,__LINE__,i, n);
					return IR_MATCH_MATCH;
				} else {
					hiir_debug(KERN_DEBUG "%s->%d, rc5 frame err matching :"\
							" burst at %d,(n=%d), frame not match!\n",
							__func__,__LINE__,i,n);
					return IR_MATCH_NOT_MATCH;
				}
			}
		}
		
		hiir_debug("%s->%d, burst not found!\n", __func__,__LINE__);
		break;
	case IR_MTT_BURST:
		break;
	}
	return IR_MATCH_NOT_MATCH;
}
/* call condition:
 * if parse_rc5 returns !0, this routine can be called.
 */
void rc5_parse_error_handle(struct ir_priv *ir,
		struct ir_protocol *ip, struct ir_buffer *symb_head,
		struct ir_buffer *key_head)
{
	struct key_attr *symbol;

	symbol = &symb_head->buf[symb_head->reader];
	while ((symbol->upper || symbol->lower)) {
		/* clear symbols till burst appears. */
		hiir_debug("%s->%d, process symbol:[l,u] at %d:[%d, %d]\n",
				__func__,__LINE__,symb_head->reader,
				(u32)symbol->lower,(u32)symbol->upper);
		if (symbol->upper > ip->attr.burst.maxs
				|| data_fallin(symbol->upper,
					ip->attr.burst.mins,
					ip->attr.burst.maxs)) {
			symbol = ir_next_reader_clr_inc(symb_head);
			break;
		}
		symbol = ir_next_reader_clr_inc(symb_head);
	}
}

enum IR_MATCH_RESULT rc5_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip)
{
	struct key_attr *symbol;
	int n;
	symbol = &head->buf[head->reader];
	if (!symbol->upper && !symbol->lower)
		return IR_MATCH_NOT_MATCH;
	switch (type) {
	case IR_MTT_HEADER:
		hiir_debug("rc5, header matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (!key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		hiir_debug("rc5, header matching: header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc5, frame matching: header mismatch!\n");
			return IR_MATCH_NOT_MATCH;
		}
		hiir_debug("rc5, frame matching, header match at:%d, [l, u]:[%d, %d]\n",
			head->reader, (u32)symbol->lower, (u32)symbol->upper);
		/* try find burst. */
		n = head->reader + ip->attr.burst_offset + 1;
		if (n >= MAX_SYMBOL_NUM)
			n -= MAX_SYMBOL_NUM;
		symbol = &head->buf[n];
		/* ignore lower symbol. */
		if (symbol->upper > ip->attr.burst.maxs
			|| data_fallin(symbol->upper,
				ip->attr.burst.mins,
				ip->attr.burst.maxs)) {
			hiir_debug("rc5, frame matching: burst match at %d."\
				" symbol[l,u]:[%d, %d],"\
				"burst:[p, s, f]:[%d, %d, %d]\n", head->reader,
				(u32)symbol->lower, (u32)symbol->upper,
				ip->attr.burst.pluse, ip->attr.burst.space,
				ip->attr.burst.factor);

			return IR_MATCH_MATCH;
		}
		hiir_debug("%s->%d, burst not found!\n", __func__,__LINE__);
		break;
	case IR_MTT_BURST:
		break;
	}
	return IR_MATCH_NOT_MATCH;
}
int parse_rc5(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct ir_signal ir_pulse, ir_space;
	struct rc5_ir *rc5; 
	struct key_attr *symbol;
	int ret, i, fail, cnt = 0;
#ifdef HIIR_DEBUG
	int syms = 0;
#endif
	if (ip->priv >= MAX_RC5_INFR_NR) {
		hiir_error("ip->pirv > MAX_RC5_INFR_NR!\n");
		return -1;
	}
	rc5 = &rc5_data[ip->priv];
	symbol = &rd->buf[rd->reader];
	fail = i = 0;
	rc5->state = STATE_INACTIVE;
	while (symbol && symbol->lower && symbol->upper && !fail) {
		hiir_debug("%s->%d, parse symbol [l, u]:[%d, %d] at %d,"\
			" symbols:%d\n", __func__, __LINE__,
				(u32)symbol->lower, (u32)symbol->upper,
				rd->reader,syms ++);

		ir_pulse.duration = symbol->lower;
		ir_pulse.pulse = 1;
		ret = ir_rc5_decode(rc5, ir_pulse, ip);
		if (ret)
			fail = 1;

		rc5->prev_signal = ir_pulse;
		ir_space.duration = symbol->upper;
		ir_space.pulse = 0;
		ret = ir_rc5_decode(rc5, ir_space, ip);
		if (ret)
			fail = 1;
		rc5->prev_signal = ir_space;
		symbol = ir_next_reader_clr_inc(rd);
		if (rc5->state == STATE_FINISHED
			|| rc5->state == STATE_INACTIVE) {
			break;
		}
	}
	if (fail) {
		hiir_info("Errors occured while decod rc5 frame,"\
				" discard this frame!\n");
		return -1;
	}

	memcpy(rc5->this_key.protocol_name, ip->ir_code_name, PROTOCOL_NAME_SZ);
	del_timer(&rc5_timer[ip->priv]);
	/* if a repeat key. */
	if (rc5->has_last_key
		&& (rc5->this_key.lower == rc5->last_key.lower 
			&& rc5->this_key.upper == rc5->last_key.upper)
		&& rc5->last_key.key_stat != KEY_STAT_UP) {

		if (ir->key_repeat_event
				&& time_after(jiffies,
					rc5_repeat_next_time[ip->priv])) {
			rc5->last_key.key_stat = KEY_STAT_HOLD;
			ir_insert_key_tail(wr, &rc5->last_key);
			rc5_repeat_next_time[ip->priv] = jiffies + 
				msecs_to_jiffies(ir->key_repeat_interval);
			cnt ++;
		}
		goto out;
	}
	/* if a new key recevied, send a key up event of last key. */
	if (rc5->has_last_key 
		&& rc5->last_key.key_stat != KEY_STAT_UP
		&& (rc5->last_key.lower != rc5->this_key.lower
			|| rc5->last_key.upper != rc5->this_key.upper)) {
		if (ir->key_up_event) {
			rc5->last_key.key_stat = KEY_STAT_UP; 
			ir_insert_key_tail(wr, &rc5->last_key);
			cnt ++;
		}
	}
	/* the new key */
	rc5->this_key.key_stat = KEY_STAT_DOWN;
	ir_insert_key_tail(wr, &rc5->this_key);
	cnt ++;
	rc5_repeat_next_time[ip->priv] = jiffies + 
		msecs_to_jiffies(ir->key_repeat_interval);
out:
	rc5->has_last_key = 1;
	memcpy(&rc5->last_key, &rc5->this_key, sizeof(struct key_attr));

	rc5_timer[ip->priv].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	rc5_timer[ip->priv].data = ip->priv;
	add_timer(&rc5_timer[ip->priv]);

	return cnt ? 0 : -1;
}
/* Extended rc5 handles */
int parse_extended_rc5(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct ir_signal ir_pulse, ir_space;
	struct rc5_ir *rc5; 
	struct key_attr *symbol;
	int ret, i, fail, cnt = 0;
#ifdef HIIR_DEBUG
	int syms = 0;
#endif
	if (ip->priv >= MAX_RC5_INFR_NR) {
		hiir_error("ip->pirv > MAX_RC5_INFR_NR!\n");
		return -1;
	}
	rc5 = &rc5_data[ip->priv];
	symbol = &rd->buf[rd->reader];
	fail = i = 0;
	rc5->state = STATE_INACTIVE;
	while (symbol && symbol->lower && symbol->upper && !fail) {
		hiir_debug("%s->%d, parse symbol [l, u]:[%d, %d] at %d,"\
			" symbols:%d\n", __func__, __LINE__,
				(u32)symbol->lower, (u32)symbol->upper,
				rd->reader,syms ++);

		ir_pulse.duration = symbol->lower;
		ir_pulse.pulse = 1;
		ret = ir_rc5_decode(rc5, ir_pulse, ip);
		if (ret)
			fail = 1;

		rc5->prev_signal = ir_pulse;
		ir_space.duration = symbol->upper;
		ir_space.pulse = 0;
		ret = ir_rc5_decode(rc5, ir_space, ip);
		if (ret)
			fail = 1;
		rc5->prev_signal = ir_space;
		symbol = ir_next_reader_clr_inc(rd);
		if (rc5->state == STATE_FINISHED
			|| rc5->state == STATE_INACTIVE) {
			break;
		}
	}
	if (fail) {
		hiir_info("Errors occured while decod rc5 frame,"\
				" discard this frame!\n");
		return -1;
	}

	memcpy(rc5->this_key.protocol_name, ip->ir_code_name, PROTOCOL_NAME_SZ);
	del_timer(&rc5_timer[ip->priv]);
	/* if a repeat key. */
	if (rc5->has_last_key
		&& (rc5->this_key.lower == rc5->last_key.lower 
			&& rc5->this_key.upper == rc5->last_key.upper)
		&& rc5->last_key.key_stat != KEY_STAT_UP) {

		if (ir->key_repeat_event
				&& time_after(jiffies,
					rc5_repeat_next_time[ip->priv])) {
			rc5->last_key.key_stat = KEY_STAT_HOLD;
			ir_insert_key_tail(wr, &rc5->last_key);
			rc5_repeat_next_time[ip->priv] = jiffies + 
				msecs_to_jiffies(ir->key_repeat_interval);
			cnt ++;
		}
		goto out;
	}
	/* if a new key recevied, send a key up event of last key. */
	if (rc5->has_last_key 
		&& rc5->last_key.key_stat != KEY_STAT_UP
		&& (rc5->last_key.lower != rc5->this_key.lower
			|| rc5->last_key.upper != rc5->this_key.upper)) {
		if (ir->key_up_event) {
			rc5->last_key.key_stat = KEY_STAT_UP; 
			ir_insert_key_tail(wr, &rc5->last_key);
			cnt ++;
		}
	}
	/* the new key */
	rc5->this_key.key_stat = KEY_STAT_DOWN;
	ir_insert_key_tail(wr, &rc5->this_key);
	cnt ++;
	rc5_repeat_next_time[ip->priv] = jiffies + 
		msecs_to_jiffies(ir->key_repeat_interval);
out:
	rc5->has_last_key = 1;
	memcpy(&rc5->last_key, &rc5->this_key, sizeof(struct key_attr));

	rc5_timer[ip->priv].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	rc5_timer[ip->priv].data = ip->priv;
	add_timer(&rc5_timer[ip->priv]);

	return cnt ? 0 : -1;
}

enum IR_MATCH_RESULT extended_rc5_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip)
{
	struct key_attr *symbol;
	int n;
	symbol = &head->buf[head->reader];
	if (!symbol->upper && !symbol->lower)
		return IR_MATCH_NOT_MATCH;
	switch (type) {
	case IR_MTT_HEADER:
		hiir_debug("rc5, header matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);
		/* Extended rc5's header may show up as [1750, 1750, 20%] or [1750, 889, 20%]*/
		/* check first part 1750(20%)*/
		if (data_fallin((u32)symbol->lower, (u32)ip->attr.header.minp, 
				(u32)ip->attr.header.maxp)
			/* check second part 1750(20%) or 889(20%)*/
			&& (data_fallin(symbol->upper, ip->attr.header.minp, 
					ip->attr.header.maxp)
				|| data_fallin(symbol->upper, RC5_UNIT * (100 - ip->attr.header.factor) / 100,
					RC5_UNIT * (100 + ip->attr.header.factor) / 100))){
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		hiir_debug("rc5, header matching: header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		/* Extended rc5's header may show up as [1750, 1750, 20%] or [1750, 889, 20%]*/
		/* check first part 1750(20%)*/
		if (!data_fallin(symbol->lower, ip->attr.header.minp, 
				ip->attr.header.maxp)
			/* check second part 1750(20%) or 889(20%)*/
			|| (!data_fallin(symbol->upper, ip->attr.header.minp, 
					ip->attr.header.maxp)
				&& !data_fallin(symbol->upper, RC5_UNIT * (100 - ip->attr.header.factor) / 100,
					RC5_UNIT * (100 + ip->attr.header.factor) / 100))){
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_NOT_MATCH;
		}
		
		hiir_debug("rc5, frame matching, header match at:%d, [l, u]:[%d, %d]\n",
			head->reader, (u32)symbol->lower, (u32)symbol->upper);
		/* try find burst. */
		n = head->reader + ip->attr.burst_offset + 1;
		if (n >= MAX_SYMBOL_NUM)
			n -= MAX_SYMBOL_NUM;
		symbol = &head->buf[n];
		/* ignore lower symbol. */
		if (symbol->upper > ip->attr.burst.maxs
			|| data_fallin(symbol->upper,
				ip->attr.burst.mins,
				ip->attr.burst.maxs)) {
			hiir_debug("rc5, frame matching: burst match at %d."\
				" symbol[l,u]:[%d, %d],"\
				"burst:[p, s, f]:[%d, %d, %d]\n", head->reader,
				(u32)symbol->lower, (u32)symbol->upper,
				ip->attr.burst.pluse, ip->attr.burst.space,
				ip->attr.burst.factor);

			return IR_MATCH_MATCH;
		}
		hiir_debug("%s->%d, burst not found!\n", __func__,__LINE__);
		break;
	case IR_MTT_BURST:
		break;
	}
	return IR_MATCH_NOT_MATCH;
}
/* call condition:
 * if extended_rc5_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT extended_rc5_match_error_handle(enum IR_MATCH_TYPE type,
		struct ir_buffer *head,
		struct ir_protocol *ip)
{
	struct key_attr *symbol;
	int n, i;
	symbol = &head->buf[head->reader];
	if (!symbol->upper && !symbol->lower)
		return IR_MATCH_NOT_MATCH;
	switch (type) {
	case IR_MTT_HEADER:
		hiir_debug("rc5, header matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);
		
		/* Extended rc5's header may show up as [1750, 1750, 20%] or [1750, 889, 20%]*/
		/* check first part 1750(20%)*/
		if (data_fallin((u32)symbol->lower, (u32)ip->attr.header.minp, 
				(u32)ip->attr.header.maxp)
			/* check second part 1750(20%) or 889(20%)*/
			&& (data_fallin((u32)symbol->upper, (u32)ip->attr.header.minp, 
					(u32)ip->attr.header.maxp)
				|| data_fallin((u32)symbol->upper, RC5_UNIT * (100 - ip->attr.header.factor) / 100,
					RC5_UNIT * (100 + ip->attr.header.factor) / 100))){
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		
		hiir_debug("rc5, header matching: header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		/* Extended rc5's header may show up as [1750, 1750, 20%] or [1750, 889, 20%]*/
		/* check first part 1750(20%)*/
		if (!data_fallin((u32)symbol->lower, (u32)ip->attr.header.minp, 
				(u32)ip->attr.header.maxp)
			/* check second part 1750(20%) or 889(20%)*/
			|| (data_fallin((u32)symbol->upper, (u32)ip->attr.header.minp, 
					(u32)ip->attr.header.maxp)
				&& data_fallin((u32)symbol->upper, RC5_UNIT * (100 - ip->attr.header.factor) / 100,
					RC5_UNIT * (100 + ip->attr.header.factor) / 100))){
			hiir_debug("rc5, header matching: match!\n");
			return IR_MATCH_NOT_MATCH;
		}

		hiir_debug("rc5, frame matching, header match at:%d, [l, u]:[%d, %d]\n",
			head->reader, (u32)symbol->lower, (u32)symbol->upper);
                /* try find burst. */
		for (i = head->reader + 1, n = 0; n < MAX_DATA_BITS; n++, i++) {
			if (i >= MAX_SYMBOL_NUM)
				i -= MAX_SYMBOL_NUM;
			symbol = &head->buf[i];
			if (!symbol->upper && !symbol->lower) {
				hiir_debug("%s->%d, needs more data\n",
						__func__,__LINE__);
				return IR_MATCH_NEED_MORE_DATA;
			}
			hiir_debug("rc5, frame matching. symbol[l,u]:[%d, %d] at %d,"\
					"burst:[p, s, f]:[%d, %d, %d]\n",
					(u32)symbol->lower, (u32)symbol->upper, i,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
			/* ignore lower symbol. */
			if (symbol->upper > ip->attr.burst.maxs
					|| data_fallin(symbol->upper,
						ip->attr.burst.mins,
						ip->attr.burst.maxs)) {
				hiir_debug("rc5, frame matching: burst match at %d. symbol[l,u]:[%d, %d],"\
					"burst:[p, s, f]:[%d, %d, %d]\n", i,
					(u32)symbol->lower, (u32)symbol->upper,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
				if (n >= ip->attr.burst_offset)
					n = n - ip->attr.burst_offset;
				else
					n = ip->attr.burst_offset - n;
				if (n <= RC5_BURST_OFFSET) {
					hiir_debug(KERN_DEBUG"%s->%d, rc5 frame err matching :"\
							" burst at %d, (n=%d), frame match!\n",
							__func__,__LINE__,i, n);
					return IR_MATCH_MATCH;
				} else {
					hiir_debug(KERN_DEBUG "%s->%d, rc5 frame err matching :"\
							" burst at %d,(n=%d), frame not match!\n",
							__func__,__LINE__,i,n);
					return IR_MATCH_NOT_MATCH;
				}
			}
		}
		
		hiir_debug("%s->%d, burst not found!\n", __func__,__LINE__);
		break;
	case IR_MTT_BURST:
		break;
	}
	return IR_MATCH_NOT_MATCH;
}
/* call condition:
 * if extended_rc5_match returns !0, this routine can be called.
 */
void extended_rc5_parse_error_handle(struct ir_priv *ir,
		struct ir_protocol *ip, struct ir_buffer *symb_head,
		struct ir_buffer *key_head)
{
	struct key_attr *symbol;

	symbol = &symb_head->buf[symb_head->reader];
	while ((symbol->upper || symbol->lower)) {
		/* clear symbols till burst appears. */
		hiir_debug("%s->%d, process symbol:[l,u] at %d:[%d, %d]\n",
				__func__,__LINE__,symb_head->reader,
				(u32)symbol->lower,(u32)symbol->upper);
		if (symbol->upper > ip->attr.burst.maxs
				|| data_fallin(symbol->upper,
					ip->attr.burst.mins,
					ip->attr.burst.maxs)) {
			symbol = ir_next_reader_clr_inc(symb_head);
			break;
		}
		symbol = ir_next_reader_clr_inc(symb_head);
	}
}

EXPORT_SYMBOL(rc5_match);
EXPORT_SYMBOL(parse_rc5);
EXPORT_SYMBOL(rc5_parse_error_handle);
EXPORT_SYMBOL(rc5_match_error_handle);

