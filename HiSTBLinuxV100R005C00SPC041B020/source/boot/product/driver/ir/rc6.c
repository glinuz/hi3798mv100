#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif
#include "ir.h"
#include "ir_protocol.h"
#include "ir_utils.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "errno.h"
#endif

#define RC6_UNIT		    445   /* us */     
#define RC6_HEADER_NBITS	4	  /* not including toggle bit */
#define RC6_PREFIX_PULSE	(6 * RC6_UNIT)
#define RC6_PREFIX_SPACE	(2 * RC6_UNIT)
#define RC6_BIT_START		(1 * RC6_UNIT)
#define RC6_BIT_END			(1 * RC6_UNIT)
#define RC6_TOGGLE_START	(2 * RC6_UNIT)
#define RC6_TOGGLE_END		(2 * RC6_UNIT)
#define RC6_TOGGLE_MASK		0x8000	  

enum RC6_STATE{
	STATE_INACTIVE,		//0
	STATE_PREFIX_SPACE,	//1
	STATE_HEADER_BIT_START,	//2
	STATE_HEADER_BIT_END,	//3
	STATE_TOGGLE_START,	//4
	STATE_TOGGLE_END,	//5
	STATE_BIT_START,	//6
	STATE_BIT_END,		//7
	STATE_FINISHED,		//8
};

struct rc6_ir {
	enum RC6_STATE state;
	unsigned int header;
	unsigned long long bits;
	int count;
	unsigned long long scancode;
	struct key_attr this_key;
	struct key_attr last_key;
	struct ir_signal prev_signal;
};

static struct rc6_ir rc6_data[MAX_RC6_INFR_NR];
static unsigned long rc6_repeat_next_time[MAX_RC6_INFR_NR];
static struct timer_list rc6_timer[MAX_RC6_INFR_NR];
static void rc6_keyup_proc(unsigned long i)
{
	struct key_attr *last_key;
	extern struct ir_priv ir_local;
	if (i >= MAX_RC6_INFR_NR) {
		hiir_error("rc6 keyup timer, i > MAX_RC6_INFR_NR!\n");
		return;
	}

	last_key = &rc6_data[i].last_key;
	if ((last_key->lower || last_key->upper)
			&& last_key->key_stat != KEY_STAT_UP) {
		last_key->key_stat = KEY_STAT_UP;
		if (ir_local.key_up_event) {
			ir_insert_key_tail(ir_local.key_buf, last_key);
			wake_up_interruptible(&ir_local.read_wait);
		}
	}
}
void rc6_init(void)
{
	int i;
	for (i = 0; i < MAX_RC6_INFR_NR; i++) {
		init_timer(&rc6_timer[i]);
		rc6_timer[i].expires = 0;
		rc6_timer[i].data = (unsigned long)~0;
		rc6_timer[i].function = rc6_keyup_proc;
	}
	memset(rc6_data, 0, sizeof(struct rc6_ir) * MAX_RC6_INFR_NR);
}
void rc6_exit(void)
{
	int i;
	for (i = 0; i < MAX_RC6_INFR_NR; i++)
		del_timer(&rc6_timer[i]);
}

static int ir_rc6_decode(struct rc6_ir *data, struct ir_signal signal,
		struct ir_protocol *ip)
{
	int i;
	i = ip->priv;
	if (i >= MAX_RC6_INFR_NR)
		goto out;

	if (!geq_margin(signal.duration, RC6_UNIT, RC6_UNIT / 2))
		goto out;
		
again:
	hiir_debug("RC6 decode started at state %i (%uus %s) "
		"count %d header 0x%x bits 0x%llx, count:%d\n",
		data->state, signal.duration, TO_STR(signal.pulse),
		data->count, data->header, data->bits, data->count);

	if (!geq_margin(signal.duration, RC6_UNIT, RC6_UNIT / 2))
		return 0;

	switch (data->state) {
	case STATE_INACTIVE:
		if (!signal.pulse)
			break;

		if (!eq_margin(signal.duration, RC6_PREFIX_PULSE, RC6_UNIT))
			break;

		data->state = STATE_PREFIX_SPACE;
		data->bits = 0;
		data->count = 0;
		data->header = 0;
		memset(&data->this_key, 0, sizeof(struct key_attr));
		return 0;

	case STATE_PREFIX_SPACE:
		if (signal.pulse)
			break;

		if (!eq_margin(signal.duration, RC6_PREFIX_SPACE,
			RC6_UNIT / 2))
			break;

		data->state = STATE_HEADER_BIT_START;
		return 0;

	case STATE_HEADER_BIT_START:
		if (!eq_margin(signal.duration, RC6_BIT_START,
			RC6_UNIT / 2))
			break;

		data->header <<= 1;
		if (signal.pulse)
			data->header |= 1;

		data->count++;
		data->state = STATE_HEADER_BIT_END;
		return 0;

	case STATE_HEADER_BIT_END:
		if (!is_transition(&signal, &data->prev_signal))
			break;

		if (data->count == RC6_HEADER_NBITS)
			data->state = STATE_TOGGLE_START;
		else
			data->state = STATE_HEADER_BIT_START;

		decrease_duration(&signal, RC6_BIT_END);
		goto again;

	case STATE_TOGGLE_START:
		if (!eq_margin(signal.duration, RC6_TOGGLE_START, RC6_UNIT / 2))
			break;

		data->state = STATE_TOGGLE_END;
		return 0;

	case STATE_TOGGLE_END:
		if (!is_transition(&signal, &data->prev_signal) 
			|| !geq_margin(signal.duration, RC6_TOGGLE_END,
				RC6_UNIT / 2))
			break;

		data->state = STATE_BIT_START;
		decrease_duration(&signal, RC6_TOGGLE_END);
		data->count = 0;

		goto again;

	case STATE_BIT_START:
		if (!eq_margin(signal.duration, RC6_BIT_START, RC6_UNIT / 2))
			break;
		
		data->bits <<= 1;
		if (signal.pulse)
			data->bits |= 1;
		data->count++;
		data->state = STATE_BIT_END;
		return 0;

	case STATE_BIT_END:
		if (!is_transition(&signal, &data->prev_signal))
			break;

		if (data->count == ip->attr.wanna_bits) {
			data->scancode = data->bits;
			data->this_key.lower = (data->scancode & (~RC6_TOGGLE_MASK));
			data->this_key.upper = 0;
			data->state = STATE_FINISHED;
			return 0;
		} else
			data->state = STATE_BIT_START;

		decrease_duration(&signal, RC6_BIT_END);
		goto again;

	case STATE_FINISHED:
		if (signal.pulse)
			break;

		data->state = STATE_INACTIVE;
		return 0;
	}

out:
	hiir_info("RC6 decode failed at state %i (%uus %s), bits received:%d\n",
		   data->state, signal.duration, TO_STR(signal.pulse), data->count);
	data->state = STATE_INACTIVE;
	return -EINVAL;
}
enum IR_MATCH_RESULT rc6_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip)
{
	struct key_attr *symbol;
	int n;
	symbol = &head->buf[head->reader];
	if (!symbol->upper && !symbol->lower)
		return IR_MATCH_NOT_MATCH;
	switch (type) {
	case IR_MTT_HEADER:
		hiir_debug("rc6, header matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (!key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc6, header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		hiir_debug("rc6, header matching: header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc6, frame matching. symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc6, frame matching: header mismatch!\n");
			return IR_MATCH_NOT_MATCH;
		}
		hiir_debug("rc6, frame matching, header match at:%d, [l, u]:[%d, %d]\n",
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
			hiir_debug("rc6, frame matching: burst match at %d."\
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
 * if rc6_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT rc6_match_error_handle(enum IR_MATCH_TYPE type,
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
		hiir_debug("rc6 match err handle, header matching."\
			" symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (!key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc6 match err handle,"\
				" header matching: match!\n");
			return IR_MATCH_MATCH;
		}
		hiir_debug("rc6 match err handle, header matching:"\
				" header mismatch!\n");
		break;
	case IR_MTT_FRAME:
		hiir_debug("rc6 match err handle, frame matching."\
			" symbol[l,u]:[%d, %d],"\
			"header:[p, s, f]:[%d, %d, %d]\n",
			(u32)symbol->lower, (u32)symbol->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor);

		if (key_match_phase(symbol, &ip->attr.header)) {
			hiir_debug("rc6, frame matching: header mismatch!\n");
			return IR_MATCH_NOT_MATCH;
		}
		hiir_debug("rc6, frame err matching, header match at:%d, [l, u]:[%d, %d]\n",
			head->reader, (u32)symbol->lower, (u32)symbol->upper);

		for (i = head->reader + 1, n = 0; n < MAX_DATA_BITS; n++, i++) {
			if (i >= MAX_SYMBOL_NUM)
				i -= MAX_SYMBOL_NUM;
			symbol = &head->buf[i];
			if (!symbol->upper && !symbol->lower) {
				hiir_debug("%s->%d, needs more data\n",
						__func__,__LINE__);
				return IR_MATCH_NEED_MORE_DATA;
			}
			hiir_debug("rc6, frame err matching. symbol[l,u]:[%d, %d] at %d,"\
					"burst:[p, s, f]:[%d, %d, %d]\n",
					(u32)symbol->lower, (u32)symbol->upper, i,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
			/* ignore lower symbol. */
			if (symbol->upper > ip->attr.burst.maxs
					|| data_fallin(symbol->upper,
						ip->attr.burst.mins,
						ip->attr.burst.maxs)) {
				hiir_debug("rc6, frame matching: burst match at %d. symbol[l,u]:[%d, %d],"\
					"burst:[p, s, f]:[%d, %d, %d]\n", i,
					(u32)symbol->lower, (u32)symbol->upper,
					ip->attr.burst.pluse, ip->attr.burst.space,
					ip->attr.burst.factor);
				if (n >= ip->attr.burst_offset)
					n = n - ip->attr.burst_offset;
				else 
					n = ip->attr.burst_offset - n;
				if (n <= RC6_BURST_OFFSET) {
					hiir_debug("%s->%d, rc6 frame err matching :"\
						" burst at %d, (n=%d), frame match!\n",
						__func__,__LINE__,i, n);
					return IR_MATCH_MATCH;
				} else {
					hiir_debug("%s->%d, rc6 frame err matching :"\
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
 * if parse_rc6 returns !0, this routine can be called.
 * discard the frame we cannot parsed.. 
 * if we do not dicard this frame, it maybe recognized as rc5 or other 
 * infrared code....
 */
void rc6_parse_error_handle(struct ir_priv *ir,
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


int parse_rc6(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct ir_signal ir_pulse, ir_space;
	struct rc6_ir *rc6; 
	struct key_attr *symbol;
	int ret, i, fail, cnt = 0;
	int syms;
	if (ip->priv >= MAX_RC6_INFR_NR) {
		hiir_error("ip->priv > MAX_RC6_INFR_NR!\n");
		return -1;
	}

	rc6 = &rc6_data[ip->priv];
	symbol = &rd->buf[rd->reader];
	fail = i = 0;
	rc6->state = STATE_INACTIVE;
	syms = 0;
	while (symbol && symbol->lower && symbol->upper && !fail) {
		syms ++;
		hiir_debug("%s->%d, parse symbol [l, u]:[%d, %d] at %d, symbols:%d\n",
			__func__,__LINE__, 
			(u32)symbol->lower, (u32)symbol->upper, rd->reader,syms);
		ir_pulse.duration = symbol->lower;
		ir_pulse.pulse = 1;
		ret = ir_rc6_decode(rc6, ir_pulse, ip);
		if (ret)
			fail = 1;
		rc6->prev_signal = ir_pulse;

		ir_space.duration = symbol->upper;
		ir_space.pulse = 0;
		ret = ir_rc6_decode(rc6, ir_space, ip);
		if (ret)
			fail = 1;
		rc6->prev_signal = ir_space;
		symbol = ir_next_reader_clr_inc(rd);
		if ((rc6->state == STATE_FINISHED
			|| rc6->state == STATE_INACTIVE)) {
			hiir_debug("%s->%d, rc6->state :%d\n",__func__,__LINE__,rc6->state);
			break;
		}
	}
	if (fail) {
		hiir_info("Errors occured while decode rc6 frame, discard this frame!\n");
		return -1;
	}

	memcpy(rc6->this_key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);

	del_timer(&rc6_timer[ip->priv]);
	/* if a repeat key. */
	if ((rc6->this_key.lower == rc6->last_key.lower 
		&& rc6->this_key.upper == rc6->last_key.upper)
		&& rc6->last_key.key_stat != KEY_STAT_UP) {

		if (ir->key_repeat_event
				&& time_after(jiffies,
					rc6_repeat_next_time[ip->priv])) {

			rc6->last_key.key_stat = KEY_STAT_HOLD;
			ir_insert_key_tail(wr, &rc6->last_key);

			rc6_repeat_next_time[ip->priv] = jiffies + 
				msecs_to_jiffies(ir->key_repeat_interval);
			cnt ++;
		}
		goto out;
	}
	/* if a new key recevied, send a key up event of last key. */
	if ((rc6->last_key.lower || rc6->last_key.upper)
		&& rc6->last_key.key_stat != KEY_STAT_UP
		&& (rc6->last_key.lower != rc6->this_key.lower
			|| rc6->last_key.upper != rc6->this_key.upper)) {

		if (ir->key_up_event) {
			rc6->last_key.key_stat = KEY_STAT_UP; 
			ir_insert_key_tail(wr, &rc6->last_key);
			cnt ++;
		}
	}
	/* the new key */
	rc6->this_key.key_stat = KEY_STAT_DOWN;
	ir_insert_key_tail(wr, &rc6->this_key);
	cnt ++;
	rc6_repeat_next_time[ip->priv] = jiffies + 
		msecs_to_jiffies(ir->key_repeat_interval);
out:

	rc6_timer[ip->priv].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	rc6_timer[ip->priv].data = ip->priv;
	add_timer(&rc6_timer[ip->priv]);
	memcpy(&rc6->last_key, &rc6->this_key, sizeof(struct key_attr));
	return cnt ? 0 : -1;
}
EXPORT_SYMBOL(rc6_match);
EXPORT_SYMBOL(parse_rc6);
EXPORT_SYMBOL(rc6_match_error_handle);
EXPORT_SYMBOL(rc6_parse_error_handle);

