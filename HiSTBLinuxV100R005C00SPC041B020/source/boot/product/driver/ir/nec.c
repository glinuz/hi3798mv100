#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif
#include "ir.h"
#include "ir_protocol.h"
#include "ir_utils.h"
/* store the max & min value of pluse and space to minx and maxx */
#define mm_ps(phase, minp, maxp, mins, maxs)	\
	do {					\
		minp = (phase)->minp;		\
		maxp = (phase)->maxp;		\
		mins = (phase)->mins;		\
		maxs = (phase)->maxs;		\
	} while (0)

static unsigned long nec_repeat_next_time[MAX_NEC_INFR_NR];
static struct key_attr nec_last_key[MAX_NEC_INFR_NR];
static struct timer_list nec_timer[MAX_NEC_INFR_NR];
static void nec_keyup_proc(unsigned long i)
{
	struct key_attr *last_key;
	extern struct ir_priv ir_local;
	if (i >= MAX_NEC_INFR_NR) {
		hiir_error("nec keyup timer, i > MAX_NEC_INFR_NR!\n");
		return;
	}

	last_key = &nec_last_key[i];
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
void nec_init(void)
{
	int i;
	for (i = 0; i < MAX_NEC_INFR_NR; i++) {
		init_timer(&nec_timer[i]);
		nec_timer[i].expires = 0;
		nec_timer[i].data = (unsigned long)~0;
		nec_timer[i].function = nec_keyup_proc;
	}
	memset(nec_last_key, 0, 
		MAX_NEC_INFR_NR * sizeof(struct key_attr));
}
void nec_exit(void)
{
	int i;
	for (i = 0; i < MAX_NEC_INFR_NR; i++)
		del_timer(&nec_timer[i]);
}
#define print_mm_sp(phase, name)        \
	do {                            \
		hiir_debug("%s: minp:%d, maxp:%d, mins:%d, maxp:%d\n",  \
			name,(phase)->minp, (phase)->maxp, (phase)->mins,\
			(phase)->maxs);\
	} while(0)

/* to see a frame is a full frame or a repeat frame */
enum IR_MATCH_RESULT nec_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip)
{
	struct key_attr *key;
	int n, idx;
	idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
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
			/* repeate frame match? */
			if (!ip->attr.repeat.pluse && !ip->attr.repeat.space)
				return IR_MATCH_NOT_MATCH;
			hiir_debug("%s->%d, idx:%d, checking repeat!\n", __func__,__LINE__,idx);
			if (key_match_phase(key, &ip->attr.repeat)) {
				hiir_debug("%s->%d, idx:%d, repeat not match!\n",
						__func__, __LINE__,idx);
				return IR_MATCH_NOT_MATCH;
			} else {
				/* if this is repeate key, then check burst here. */
				n = 1;

				n = buf->reader + n;
				if (n >= MAX_SYMBOL_NUM)
					n -= MAX_SYMBOL_NUM;
				hiir_debug("%s->%d, idx:%d, checking repeat burst!\n",
						__func__,__LINE__,idx);

				key = &buf->buf[n];
				if (!key->upper && !key->lower) {
					return IR_MATCH_NEED_MORE_DATA;
				}
				mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
				if (data_fallin((u32)key->lower, minp, maxp)
					&& (((u32)key->upper > ip->attr.burst.maxs)
						|| data_fallin((u32)key->upper, mins, maxs))) {
					/* case a frame only constains repeate key, no first frame,
					 * we cannot recognize the key value, so dicard this repeat key.
					 */
					if ((nec_last_key[idx].lower || nec_last_key[idx].upper)
							&& nec_last_key[idx].key_stat != KEY_STAT_UP)
						return IR_MATCH_MATCH;
					else 
						return IR_MATCH_NOT_MATCH;


				}
				return IR_MATCH_NOT_MATCH;
			}
		} else
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
		/* NEC frame burst may fall in [space ,8%],
		 * but it still can greater than space !
		 */
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		if (data_fallin((u32)key->lower, minp, maxp)
			&& (((u32)key->upper > ip->attr.burst.maxs)
				|| data_fallin((u32)key->upper, mins, maxs))) {
			hiir_debug("%s->%d, idx:%d, frame burst match!\n",
					__func__,__LINE__,idx);

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
					//hiir_error("%s->%d, need more symbols BUT BURST appears!\n",
					//		__func__,__LINE__);
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
int nec_frame_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct key_attr *symbol;
	struct key_attr key;
	unsigned long long minp, maxp, mins, maxs;
	unsigned i;
	struct key_attr *last_key;
	int cnt = 0, fail = 0;
	uint idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		goto out;
	}
	del_timer(&nec_timer[idx]);
	last_key = &nec_last_key[idx];
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

		hiir_error("NEC : cannot parse!!!"\
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
		hiir_info("%s->%d,NEC : unkown symbol[l, u]: [%d, %d],"\
			" b0[p, s, f]: [%d, %d, %d],"\
			" b1[p, s, f]: [%d, %d, %d]. Discard this frame!\n",
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
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
			if (ir->key_up_event) {
				last_key->key_stat = KEY_STAT_UP;
				ir_insert_key_tail(wr, last_key);
				cnt ++;
			}
		}
		key.key_stat = KEY_STAT_DOWN;

		hiir_debug("%s->%d,key parsed:[l:%d, u:%d, s:%d, p:%s]!\n",__func__,__LINE__,
			(u32)key.lower, (u32)key.upper, key.key_stat, key.protocol_name);
		if (!fail) {
			ir_insert_key_tail(wr, &key);
			cnt ++;
			nec_repeat_next_time[idx] = jiffies + 
				msecs_to_jiffies(ir->key_repeat_interval);

			memcpy(last_key, &key, sizeof(struct key_attr));
			hiir_debug("%s->%d,idx:%d, key parsed ,last_key[l,u,s] [0x%x,0x%x,%d],idx:%d\n",
					__func__,__LINE__,
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
			hiir_debug("%s->%d, repeat frame not match\n",
					__func__, __LINE__);
			if (IR_MATCH_NEED_MORE_DATA == r)
				goto out;
			else
				goto start_timer_out;
		}
		mm_ps(&ip->attr.repeat, minp, maxp, mins, maxs);
		if (!data_fallin((u32)symbol->lower, minp, maxp)
			|| !data_fallin((u32)symbol->upper, mins, maxs)) {
			hiir_debug("%s->%d, repeat burst not match!\n",
					__func__,__LINE__);
			goto start_timer_out;
		}
		/* burst */
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		symbol = ir_next_reader_clr_inc(rd);
		hiir_debug("idx:%d, try parse repeat burst(at %d)!\n", idx,rd->reader);
		/* NEC burst's space may fallin [space, 8%], but it also
		 * may greate than maxp.
		 */
		if (!data_fallin((u32)symbol->lower, minp, maxp)
			|| (symbol->upper < maxs
				&& !data_fallin((u32)symbol->upper,
					mins, maxs))){
			hiir_debug("NEC : repeat burst not match"\
				"[l, u]: [%d, %d],"\
				" burst[p, s, f]: [%d, %d, %d].\n",
				(u32)symbol->lower, (u32)symbol->upper,
				ip->attr.burst.pluse, ip->attr.burst.space,
				ip->attr.burst.factor);
		} else {
			hiir_debug("repeat parsed ,last_key[l,u,s] [0x%x,0x%x,%d],"
					"time_after:%d,idx:%d!\n",
					(u32)last_key->lower,(u32)last_key->upper,
					last_key->key_stat,
					time_after(jiffies,nec_repeat_next_time[idx]),
					idx);
			if (!fail && (last_key->upper || last_key->lower)
				&& ir->key_repeat_event 
				&& time_after(jiffies,
					nec_repeat_next_time[idx])) {
				hiir_debug("insert repeat parsed !\n");

				last_key->key_stat = 
					last_key->key_stat == KEY_STAT_UP ?
						KEY_STAT_DOWN : KEY_STAT_HOLD;

				ir_insert_key_tail(wr, last_key);
				nec_repeat_next_time[idx] = jiffies + 
					msecs_to_jiffies(
						ir->key_repeat_interval);
				cnt ++;
			}
		}
		symbol = ir_next_reader_clr_inc(rd);
	} while(symbol->upper && symbol->lower);
start_timer_out:
	del_timer(&nec_timer[idx]);
	nec_timer[idx].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	nec_timer[idx].data = idx;
	add_timer(&nec_timer[idx]);
out:
	return fail ? -1 : (cnt ? 0 : -1);
}
int nec_frame_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct key_attr *symbol;
	struct key_attr key;
	u32 i;
	unsigned long long minp, maxp, mins, maxs;
	struct key_attr *last_key;
	int cnt = 0, fail = 0;
	uint idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		goto out;
	}
	del_timer(&nec_timer[idx]);
	last_key = &nec_last_key[idx];
	/* header phase */
	memset(&key, 0, sizeof(struct key_attr));
	hiir_debug("try parse header(at %d)!\n", rd->reader);
	symbol = &rd->buf[rd->reader];
	mm_ps(&ip->attr.header, minp, maxp, mins, maxs);
	/* frame start? */
	if (!data_fallin((u32)symbol->lower, minp, maxp)
		 || !data_fallin((u32)symbol->upper, mins, maxs)) {
		hiir_error("NEC : cannot parse!!!"\
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
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
		/* if a repeat key. */
		if ((last_key->upper || last_key->lower)
			&& last_key->upper == key.upper
			&& last_key->lower == key.lower
			&& last_key->key_stat != KEY_STAT_UP){
			/* should i send a repeat key? */
			if (ir->key_repeat_event && time_after(jiffies,
					nec_repeat_next_time[idx])) {

				last_key->key_stat = KEY_STAT_HOLD;
				ir_insert_key_tail(wr, last_key);
				symbol = ir_next_reader_clr_inc(rd);
				nec_repeat_next_time[idx] = jiffies + 
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
			if (ir->key_up_event) {
				last_key->key_stat = KEY_STAT_UP;
				ir_insert_key_tail(wr, last_key);
				cnt ++;
			}
		}

		hiir_debug("key parsed:[l:%llu, u:%llu, s:%d, p:%s]!\n",
			key.lower, key.upper, key.key_stat, key.protocol_name);

		key.key_stat = KEY_STAT_DOWN;
		ir_insert_key_tail(wr, &key);
		symbol = ir_next_reader_clr_inc(rd);
		nec_repeat_next_time[idx] = jiffies + 
			msecs_to_jiffies(ir->key_repeat_interval);

		memcpy(last_key, &key, sizeof(struct key_attr));
		cnt ++;
	}
start_timer_out:
	del_timer(&nec_timer[idx]);
	nec_timer[idx].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	nec_timer[idx].data = idx;
	add_timer(&nec_timer[idx]);

out:
	return fail ? -1 : (cnt ? 0 : -1);
}

enum IR_MATCH_RESULT nec_flex_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip)
{
	struct key_attr *key;
	int i, n;
	key = &buf->buf[buf->reader];
	switch (type) {
	case IR_MTT_HEADER: {
		/* Is a frame header? */
		if (!key_match_phase(key, &ip->attr.header)) {
			return IR_MATCH_MATCH;
		}
		/* Ignore repeat header checking.
		 * Because this match checking only happen while previours
		 * symbols cannot be parsed, and timer will dicards them.
		 */
		return IR_MATCH_NOT_MATCH;
	}
	case IR_MTT_FRAME: {
		unsigned long minp, maxp, mins, maxs;
		/* header match? */
		if (key_match_phase(key, &ip->attr.header)) {
			hiir_debug("%s->%d, header not match! ip at:%p"\
				" header[p, s, f]: [%d, %d, %d],"\
				" key[l, u]:[%d, %d]\n",
				__func__, __LINE__, ip,
				ip->attr.header.pluse,
				ip->attr.header.space,
				ip->attr.header.factor,
				(u32)key->lower, (u32)key->upper);
			/* repeate frame match? */
			if (!ip->attr.repeat.pluse && !ip->attr.repeat.space)
				return IR_MATCH_NOT_MATCH;
			if (key_match_phase(key, &ip->attr.repeat)) {

				hiir_debug("%s->%d, repeat not match!\n",
						__func__, __LINE__);
				return IR_MATCH_NOT_MATCH;
			}
		}		
		/* try find burst. */
		for (i = buf->reader + 1, n = 0; n < MAX_DATA_BITS; n++, i++) {
			if (i >= MAX_SYMBOL_NUM)
				i -= MAX_SYMBOL_NUM;
			key = &buf->buf[i];
			if (!key->upper && !key->lower) {
				hiir_debug("%s->%d, needs more data\n",
						__func__,__LINE__);
				return IR_MATCH_NEED_MORE_DATA;
			}

			/* burst match? */
			/* NEC frame burst may fall in [space, 8%],
			 * but it still can greater than space!
			 */
			mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
			if (data_fallin((u32)key->lower, minp, maxp)
				&& ((key->upper > ip->attr.burst.space)
					|| data_fallin((u32)key->upper, 
						mins, maxs))) {

				hiir_debug("%s->%d, frame burst match at:%d!\n",
						__func__,__LINE__, i);
				return IR_MATCH_MATCH;
			}
		}
		hiir_debug("%s->%d, no burst found!\n", __func__, __LINE__);
		return IR_MATCH_NOT_MATCH;
	}
	case IR_MTT_BURST:
		/* fall though */
	default:
		return IR_MATCH_NOT_MATCH;
	}
}
int nec_frame_full_flex_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct key_attr *symbol;
	struct key_attr key;
	u32 i;
	unsigned long long minp, maxp, mins, maxs;
	struct key_attr *last_key;
	int cnt = 0, fail = 0;

	uint idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		goto out;
	}
	del_timer(&nec_timer[idx]);
	last_key = &nec_last_key[idx];
	/* header phase */
	memset(&key, 0, sizeof(struct key_attr));
	hiir_debug("try parse header(at %d)!\n", rd->reader);
	symbol = &rd->buf[rd->reader];
	mm_ps(&ip->attr.header, minp, maxp, mins, maxs);
	/* frame start? */
	if (!data_fallin((u32)symbol->lower, minp, maxp)
		 || !data_fallin((u32)symbol->upper, mins, maxs)) {
		hiir_error("NEC : cannot parse!!!"\
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
	/* parse max MAX_DATA_BITS bits */
	while (symbol->upper && symbol->lower && i < MAX_DATA_BITS) {

		mm_ps(&ip->attr.b0, minp, maxp, mins, maxs);
		if (data_fallin((u32)symbol->lower, minp, maxp)
			&& data_fallin((u32)symbol->upper, mins, maxs)) {
			symbol = ir_next_reader_clr_inc( rd);
			i ++;
			continue;
		}

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
		/* burst */
		hiir_debug("try parse burst(at %d)!\n", rd->reader);
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		if (data_fallin((u32)symbol->lower, minp, maxp)
			&& (symbol->upper > maxs
				|| data_fallin((u32)symbol->upper, mins, maxs))) {
			/* if a repeat key. */
			if ((last_key->upper || last_key->lower)
				&& last_key->upper == key.upper
				&& last_key->lower == key.lower
				&& last_key->key_stat != KEY_STAT_UP){
				/* should i send a repeat key? */
				if (ir->key_repeat_event 
					&& time_after_eq(jiffies,
						nec_repeat_next_time[idx])) {
					if (!fail) {
						last_key->key_stat = KEY_STAT_HOLD;
						ir_insert_key_tail(wr, last_key);
						nec_repeat_next_time[idx] = jiffies + 
							msecs_to_jiffies(
								ir->key_repeat_interval);
						cnt ++;
					}
					symbol = ir_next_reader_clr_inc(rd);
				}
				/* if shouldn't, out. */
				goto start_timer_out;
			}
			/* a new key received.
			 * send a up event of last key if exist.
			 */
			if ((last_key->upper || last_key->lower) 
					&& last_key->key_stat != KEY_STAT_UP
					&& (last_key->upper != key.upper
						|| last_key->lower != key.lower)) {
				if (!fail && ir->key_up_event) {
					last_key->key_stat = KEY_STAT_UP;
					ir_insert_key_tail(wr, last_key);
					cnt ++;
				}
			}
			if (!fail) {
				key.key_stat = KEY_STAT_DOWN;
				hiir_debug("key parsed:[l:%llu, u:%llu,"\
						" s:%d, p:%s]!\n",
						key.lower, key.upper,
						key.key_stat, key.protocol_name);
				ir_insert_key_tail(wr, &key);
				cnt ++;
				memcpy(last_key, &key, sizeof(struct key_attr));
				nec_repeat_next_time[idx] = jiffies + 
					msecs_to_jiffies(ir->key_repeat_interval);
			}
			symbol = ir_next_reader_clr_inc(rd);
			goto start_timer_out;
		}
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
			" b0[p, s, f]: [%d, %d, %d],"\
			" b1[p, s, f]: [%d, %d, %d]. Discard this frame!\n",
			__func__,__LINE__,
			(u32)symbol->lower, (u32)symbol->upper, 
			ip->attr.b0.pluse, ip->attr.b0.space,
			ip->attr.b0.factor,
			ip->attr.b1.pluse, ip->attr.b1.space,
			ip->attr.b1.factor);

		symbol = ir_next_reader_clr_inc(rd);
		fail ++;
	}

start_timer_out:
	del_timer(&nec_timer[idx]);
	nec_timer[idx].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	nec_timer[idx].data = idx;
	add_timer(&nec_timer[idx]);

out:
	return fail ? -1 : (cnt ? 0: -1);
}
static enum IR_MATCH_RESULT nec_2headers_match_header(struct ir_buffer* buf,
		struct ir_protocol *ip)
{
	struct key_attr *key;
	struct key_attr *key2;
	int n, idx;
	idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		return IR_MATCH_NOT_MATCH;
	}
	key = &buf->buf[buf->reader];
	if (!key->upper && !key->lower)
		return IR_MATCH_NOT_MATCH;


	hiir_debug("%s->%d,idx:%d, checking header key[l, u] at:%d: [%d, %d]\n",
			__func__,__LINE__,idx,
			buf->reader, (u32)key->lower, (u32)key->upper);
	/* if the protocol's header and second header and repeat is the same.
	 * we should check out which phase this key is.
	 */
	if (!key_match_phase(key, &ip->attr.header)
		&& !key_match_phase(key, &ip->attr.repeat)) {
		
		unsigned long long minp, maxp, mins, maxs;
		n = buf->reader + 1;
		if (n >= MAX_SYMBOL_NUM)
			n -= MAX_SYMBOL_NUM;
		/* check repeat.*/	
		key2 = &buf->buf[n];
		hiir_debug("%s->%d,idx:%d, checking repeat burst at %d [%d, %d].\n",
				__func__,__LINE__, idx, n, (u32)key->lower, (u32)key->upper);
		if (!key2->upper && !key2->lower) {
			hiir_debug("%s->%d, idx :%d, repeat burst not appear. need more symbols!\n",
					__func__,__LINE__,idx);
			return IR_MATCH_NEED_MORE_DATA;
		}
		/* this is a repeat frame. */
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		if ((data_fallin((u32)key2->lower, minp, maxp)
					&& (((u32)key2->upper > ip->attr.burst.space)
						|| data_fallin((u32)key2->upper, mins, maxs)))) {
			if ((nec_last_key[idx].lower || nec_last_key[idx].upper)
					&& nec_last_key[idx].key_stat != KEY_STAT_UP) {

				hiir_debug("%s->%d, idx :%d, repeat burst match. this is repeat frame!\n",
						__func__,__LINE__,idx);
				return IR_MATCH_MATCH;
			} else {
				hiir_debug("%s->%d, idx :%d, repeat burst match. but no last key, this frame is not mine.!\n",
						__func__,__LINE__,idx);
				return IR_MATCH_NOT_MATCH;
			}
		}
		/* if repeat burst not appear, then  fall though to check frame. */
		hiir_debug("%s->%d, idx:%d,it seems that this is not a repeat frame. checking whole!\n",
				__func__,__LINE__,idx);
	}

	/* checking second header */
	n = buf->reader + ip->attr.second_header_at;
	if (n >= MAX_SYMBOL_NUM)
		n -= MAX_SYMBOL_NUM;
	key2 = &buf->buf[n];

	if (!key_match_phase(key, &ip->attr.header)) {
		if (!key2->upper && !key2->lower) {

			if (!ip->attr.repeat.pluse && !ip->attr.repeat.space) 
				return IR_MATCH_NEED_MORE_DATA;
			/* if some protocol's repeat phase is the same as the header,
			 * we need to check if this is a repeat here.
			 */
			if (!key_match_phase(key2, &ip->attr.repeat)) {
				hiir_debug("%s->%d idx:%d, checking repeat.\n",__func__,__LINE__,idx);
				if ((nec_last_key[idx].lower || nec_last_key[idx].upper)
						&& nec_last_key[idx].key_stat != KEY_STAT_UP) {
					hiir_debug("%s->%d idx:%d, repeat match.\n",__func__,__LINE__,idx);
					return IR_MATCH_MATCH;
				} else {
					hiir_debug("%s->%d idx:%d, repeat not match.\n",__func__,__LINE__, idx);
					return IR_MATCH_NOT_MATCH;
				}
			}
			hiir_debug("%s->%d,idx:%d,need more data to check second header\n",
					__func__,__LINE__,idx);
			/* if this is not a repeat frame, we should report symbols not enough. */
			return IR_MATCH_NEED_MORE_DATA;
		}

		hiir_debug("%s->%d,idx:%d,checking second header key[l, u] at:%d: [%d, %d]\n",
				__func__,__LINE__,idx,
				n, (u32)key2->lower, (u32)key2->upper);
		if (!key_match_phase(key2, &ip->attr.second_header))
			return IR_MATCH_MATCH;
	}
	hiir_debug("%s->%d, header not match! ip(idx:%d) at:%p"\
			" header[p, s, f]: [%d, %d, %d],"\
			" second header[p, s, f]: [%d, %d, %d],"\
			" key[l, u] at %d:[%d, %d]\n",
			__func__, __LINE__, idx,ip, 
			ip->attr.header.pluse,
			ip->attr.header.space,
			ip->attr.header.factor,
			ip->attr.second_header.pluse,
			ip->attr.second_header.space,
			ip->attr.second_header.factor,
			buf->reader,	
			(u32)key->lower, (u32)key->upper);
	/* repeate frame match? */
	if (!ip->attr.repeat.pluse && !ip->attr.repeat.space) 
		return IR_MATCH_NOT_MATCH;
	if (!key_match_phase(key, &ip->attr.repeat)) {
		hiir_debug("%s->%d idx:%d, checking repeat.\n",__func__,__LINE__,idx);
		if ((nec_last_key[idx].lower || nec_last_key[idx].upper)
				&& nec_last_key[idx].key_stat != KEY_STAT_UP) {
			hiir_debug("%s->%d idx:%d, repeat match.\n",__func__,__LINE__, idx);
			return IR_MATCH_MATCH;
		} else { 
			hiir_debug("%s->%d idx:%d, repeat not match.\n",__func__,__LINE__,idx);
			return IR_MATCH_NOT_MATCH;
		}
	}
	hiir_debug("%s->%d, repeat not match! ip(idx :%d) at:%p"\
			" repeat[p, s, f]: [%d, %d, %d],"\
			" key[l, u]:[%d, %d]\n",
			__func__, __LINE__, idx, ip, 
			ip->attr.repeat.pluse,
			ip->attr.repeat.space,
			ip->attr.repeat.factor,
			(u32)key->lower, (u32)key->upper);

	return IR_MATCH_NOT_MATCH;
}
static enum IR_MATCH_RESULT nec_2headers_match_frame(struct ir_buffer* buf,
		struct ir_protocol *ip)
{
	unsigned long long minp, maxp, mins, maxs;
	int i, j, n;
	struct key_attr *key;
	struct key_attr *key2;
	key = &buf->buf[buf->reader];
	if (!key->upper && !key->lower)
		return IR_MATCH_NOT_MATCH;
	/* checking second header */
	n = buf->reader + ip->attr.second_header_at;
	if (n >= MAX_SYMBOL_NUM)
		n -= MAX_SYMBOL_NUM;
	key2 = &buf->buf[n];

	if (key_match_phase(key, &ip->attr.header)
			|| key_match_phase(key2, &ip->attr.second_header)) {
		hiir_debug("%s->%d, header not match! ip at:%p"\
				" header[p, s, f]: [%d, %d, %d],"\
				" second header[p, s, f]: [%d, %d, %d],"\
				" key[l, u] at:%d :[%d, %d],"
				" key2[l, u] at:%d :[%d, %d]\n",
				__func__, __LINE__, ip,
				ip->attr.header.pluse,
				ip->attr.header.space,
				ip->attr.header.factor,
				ip->attr.second_header.pluse,
				ip->attr.second_header.space,
				ip->attr.second_header.factor,
				buf->reader,
				(u32)key->lower, (u32)key->upper,
				n,
				(u32)key2->lower, (u32)key2->upper);
		/* repeate frame match? */
		if (!ip->attr.repeat.pluse && !ip->attr.repeat.space)
			return IR_MATCH_NOT_MATCH;
		if (key_match_phase(key, &ip->attr.repeat)) {

			hiir_debug("%s->%d, repeat not match!\n",
					__func__, __LINE__);
			return IR_MATCH_NOT_MATCH;
		} else  {
			n = 1;
		}
	} else
		n = ip->attr.wanna_bits + 2;

	/* try find burst. */
	n = buf->reader + n;
	if (n >= MAX_SYMBOL_NUM)
		n -= MAX_SYMBOL_NUM;


	hiir_debug("%s->%d, checking burst at(%d)!\n",
			__func__,__LINE__,n);

	key = &buf->buf[n];
	if (!key->upper && !key->lower) {
		hiir_debug("%s->%d, needs more data\n",
				__func__,__LINE__);
		return IR_MATCH_NEED_MORE_DATA;
	}

	/* burst match? */
	/* NEC frame burst may fall in [space ,8%],
	 * but it still can greater than space !
	 */
	mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
	if (data_fallin((u32)key->lower, minp, maxp)
			&& (((u32)key->upper > ip->attr.burst.space)
				|| data_fallin((u32)key->upper, mins, maxs))) {

		hiir_debug("%s->%d, checking frame!\n",
				__func__,__LINE__);
#if 1
		/* check frame symbols */
		for (i = 1, j = i + buf->reader ; i <= ip->attr.wanna_bits && j != n; 
				i ++, j ++) {

			if (j >= MAX_SYMBOL_NUM)
				j -= MAX_SYMBOL_NUM;

			if (j == n)
				break;

			key = &buf->buf[j];

			if (!key->lower && !key->upper) {
				//hiir_error("%s->%d, need more symbols BUT BURST appears!\n",
				//		__func__,__LINE__);
				return IR_MATCH_NEED_MORE_DATA;
			}

			hiir_debug("%s->%d, i:%d, ip->attr.second_header_at:%d, "\
					"key:[p, s], [%d,%d], "\
					"second_header:[p, s, f], [%d, %d, %d].\n",
					__func__,__LINE__,
					i, ip->attr.second_header_at,
					(u32)key->lower, (u32)key->upper,
					(u32)ip->attr.second_header.pluse,
					(u32)ip->attr.second_header.space,
					ip->attr.second_header.factor);

			if (i == ip->attr.second_header_at) {
				hiir_debug("%s->%d, match second_header! i:%d, ip->attr.second_header_at:%d, "\
						"symbol:[l, u] at:%d: [%d, %d]\n",
						__func__,__LINE__,
						i, ip->attr.second_header_at,
						j,
						(u32)key->lower, (u32)key->upper);
				if (key_match_phase(key, &ip->attr.second_header))
					return IR_MATCH_NOT_MATCH;
				continue;
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
		/* try check repeat
		 *
		 * This check is specially for protocols which have the same
		 * header, second header, and the same position of the second header,
		 * and the same bit numbers. But the repeat header is not the same.
		 *
		 * So ugly check...
		 *
		 * If some key press do not constains repeat frame, the first protocol
		 * meats will be used to parse the frame.
		 */
		/* skip burst*/
		j += 2;
		if (j >= MAX_SYMBOL_NUM)
			j -= MAX_SYMBOL_NUM;
		key = &buf->buf[j];
		hiir_debug("Checking repeat header at:%d,[%d, %d]\n",j, (u32)key->lower,(u32)key->upper);
		if (key->lower && key->upper
				&& ip->attr.repeat.pluse && ip->attr.repeat.space) {
			if (!key_match_phase(key, &ip->attr.repeat))
				return IR_MATCH_MATCH;
			if (data_fallin(key->lower, ip->attr.repeat.minp, ip->attr.repeat.maxp)
				|| data_fallin(key->upper, ip->attr.repeat.mins, ip->attr.repeat.maxs))
				return IR_MATCH_NOT_MATCH;
		}
#endif
		return IR_MATCH_MATCH;
	} else {
		hiir_debug("%s->%d, burst not match!"
				" key[l, u][%d, %d],"
				" burst[p, s, f]: [%d, %d, %d]\n",
				__func__,__LINE__,
				(u32)key->lower, (u32)key->upper,
				ip->attr.burst.pluse, 
				ip->attr.burst.space,
				ip->attr.burst.factor);
		return IR_MATCH_NOT_MATCH;
	}

}
/* to see a frame is a full frame or a repeat frame */
enum IR_MATCH_RESULT nec_2headers_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip)
{
	switch (type) {
	case IR_MTT_HEADER: 
		return nec_2headers_match_header(buf, ip);
	case IR_MTT_FRAME: 
		return nec_2headers_match_frame(buf, ip);
	case IR_MTT_BURST:
		/* fall though */
	default:
		return IR_MATCH_NOT_MATCH;
	}
}
int nec_frame_2headers_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
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
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		goto out;
	}
	del_timer(&nec_timer[idx]);
	last_key = &nec_last_key[idx];
	/* header phase */
	memset(&key, 0, sizeof(struct key_attr));
	hiir_debug("try parse header(at %d)!\n", rd->reader);
	symbol = &rd->buf[rd->reader];
	mm_ps(&ip->attr.header, minp, maxp, mins, maxs);

	/* try get second header */
	n = rd->reader + ip->attr.second_header_at;
	if (n >= MAX_SYMBOL_NUM)
		n -= MAX_SYMBOL_NUM;
	symbol2 = &rd->buf[n];
	/* frame start? */
	if (key_match_phase(symbol, &ip->attr.header)
		|| key_match_phase(symbol2, &ip->attr.second_header)) {
		/* repeate key? */
		if (!key_match_phase(symbol, &ip->attr.repeat))
			goto repeat_key;
		hiir_error("NEC 2h simple: cannot parse!!!"\
			" header phase not match. symbol[l,u]: [%d, %d],"\
			" symbol2[l,u]: [%d, %d], "\
			" header phase[p, s, f]: [%d, %d, %d]."\
			" second header phase[p, s, f]: [%d, %d, %d]."\
			" repeat phase[p, s, f]: [%d, %d, %d].\n",
			(u32)symbol->lower, (u32)symbol->upper,
			(u32)symbol2->lower, (u32)symbol2->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor,
			ip->attr.second_header.pluse, ip->attr.second_header.space,
			ip->attr.second_header.factor,
			ip->attr.repeat.pluse, ip->attr.repeat.space,
			ip->attr.repeat.factor);

		goto out;
	}
	/* data phase */
	memcpy(key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);
	symbol = ir_next_reader_clr_inc(rd);
	hiir_debug("try parse data(at %d)!\n", rd->reader);
	i = 1;
	while (symbol->upper && symbol->lower && i <= ip->attr.wanna_bits) {
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
			if (i - 1 < 64)
				key.lower |= (unsigned long long)
					(((unsigned long long)1) << (i - 1));
			else
				key.upper |= (unsigned long long)
					(((unsigned long long)1) << (i - 64 - 1));
			symbol = ir_next_reader_clr_inc(rd);
			i ++;
			continue;
		}
		if (i == ip->attr.second_header_at) {
			hiir_debug("simple parse skip second_header! i:%d, ip->attr.second_header_at:%d, "\
				"symbol:[l, u]: [%d, %d]\n",
				i, ip->attr.second_header_at,
				(u32)symbol->lower, (u32)symbol->upper);
			symbol = ir_next_reader_clr_inc(rd);
			continue;
		}

		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
			" b0[p, s, f]: [%d, %d, %d],"\
			" b1[p, s, f]: [%d, %d, %d]. Discard this frame!\n",
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
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
			if (ir->key_up_event) {
				last_key->key_stat = KEY_STAT_UP;
				ir_insert_key_tail(wr, last_key);
				cnt ++;
			}
		}
		key.key_stat = KEY_STAT_DOWN;

		hiir_debug("key parsed:[l:%lld, u:%lld, s:%d, p:%s]!\n",
			key.lower, key.upper, key.key_stat, key.protocol_name);
		if (!fail) {
			ir_insert_key_tail(wr, &key);
			cnt ++;
			nec_repeat_next_time[idx] = jiffies + 
				msecs_to_jiffies(ir->key_repeat_interval);

			memcpy(last_key, &key, sizeof(struct key_attr));
		}
		symbol = ir_next_reader_clr_inc(rd);
	}
repeat_key:
	/* repeat? */
	do {
		enum IR_MATCH_RESULT r;
		hiir_debug("try parse repeat header(at %d)!\n", rd->reader);
		/* checking a full repeat frame received or not. */
		r = ip->match(IR_MTT_FRAME, ir->symbol_buf, ip);
		if (IR_MATCH_MATCH != r){
			hiir_debug("%s->%d, repeat frame not match\n",
					__func__, __LINE__);
			if (IR_MATCH_NEED_MORE_DATA == r)
				goto out;
			else
				goto start_timer_out;
		}
		mm_ps(&ip->attr.repeat, minp, maxp, mins, maxs);
		if (!data_fallin((u32)symbol->lower, minp, maxp)
			|| !data_fallin((u32)symbol->upper, mins, maxs)) {
			hiir_debug("%s->%d, repeat burst not match!\n",
					__func__,__LINE__);
			goto start_timer_out;
		}
		/* burst */
		mm_ps(&ip->attr.burst, minp, maxp, mins, maxs);
		symbol = ir_next_reader_clr_inc(rd);
		hiir_debug("try parse repeat burst(at %d)!\n", rd->reader);
		/* NEC burst's space may fallin [space, 8%], but it also
		 * may greate than maxp.
		 */
		if (!data_fallin((u32)symbol->lower, minp, maxp)
			|| (symbol->upper < maxs
				&& !data_fallin((u32)symbol->upper,
					mins, maxs))){
			hiir_debug("NEC : repeat burst not match"\
				"[l, u]: [%d, %d],"\
				" burst[p, s, f]: [%d, %d, %d].\n",
				(u32)symbol->lower, (u32)symbol->upper,
				ip->attr.burst.pluse, ip->attr.burst.space,
				ip->attr.burst.factor);
		} else {
			if (!fail && (last_key->upper || last_key->lower)
				&& ir->key_repeat_event 
				&& time_after(jiffies,
					nec_repeat_next_time[idx])) {

				last_key->key_stat = 
					last_key->key_stat == KEY_STAT_UP ?
						KEY_STAT_DOWN : KEY_STAT_HOLD;

				ir_insert_key_tail(wr, last_key);
				nec_repeat_next_time[idx] = jiffies + 
					msecs_to_jiffies(
						ir->key_repeat_interval);
				cnt ++;
			}
		}
		symbol = ir_next_reader_clr_inc(rd);
	} while(symbol->upper && symbol->lower);
start_timer_out:
	del_timer(&nec_timer[idx]);
	nec_timer[idx].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	nec_timer[idx].data = idx;
	add_timer(&nec_timer[idx]);
out:
	return fail ? -1 : (cnt ? 0 : -1);
}
int nec_frame_2headers_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr)
{
	struct key_attr *symbol;
	struct key_attr *symbol2;
	struct key_attr key;
	u32 i, n;
	unsigned long long minp, maxp, mins, maxs;
	struct key_attr *last_key;
	int cnt = 0, fail = 0;
	uint idx = ip->priv;
	if (idx >= MAX_NEC_INFR_NR) {
		hiir_error("nec , private data error!\n");
		goto out;
	}
	del_timer(&nec_timer[idx]);
	last_key = &nec_last_key[idx];
	/* header phase */
	memset(&key, 0, sizeof(struct key_attr));
	hiir_debug("try parse header(at %d)!\n", rd->reader);
	symbol = &rd->buf[rd->reader];

	/* try get second header */
	n = rd->reader + ip->attr.second_header_at;
	if (n >= MAX_SYMBOL_NUM)
		n -= MAX_SYMBOL_NUM;
	symbol2 = &rd->buf[n];
	/* frame start? */
	if (key_match_phase(symbol, &ip->attr.header)
		|| key_match_phase(symbol2, &ip->attr.second_header)) {
		hiir_error("NEC full 2header: cannot parse!!!"\
			" header phase not match. symbol[l,u]: [%d, %d],"\
			" symbol2[l,u]: [%d, %d], "\
			" header phase[p, s, f]: [%d, %d, %d]."\
			" second header phase[p, s, f]: [%d, %d, %d]."\
			" repeat phase[p, s, f]: [%d, %d, %d].\n",
			(u32)symbol->lower, (u32)symbol->upper,
			(u32)symbol2->lower, (u32)symbol2->upper,
			ip->attr.header.pluse, ip->attr.header.space,
			ip->attr.header.factor,
			ip->attr.second_header.pluse, ip->attr.second_header.space,
			ip->attr.second_header.factor,
			ip->attr.repeat.pluse, ip->attr.repeat.space,
			ip->attr.repeat.factor);
		goto out;
	}
	/* data phase */
	memcpy(key.protocol_name,  ip->ir_code_name, PROTOCOL_NAME_SZ);
	symbol = ir_next_reader_clr_inc(rd);
	hiir_debug("try parse data(at %d)!\n", rd->reader);
	i = 1;
	while (symbol->upper && symbol->lower && i <= ip->attr.wanna_bits) {
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
			if (i - 1 < 64)
				key.lower |= (unsigned long long)
					(((unsigned long long)1) << (i - 1));
			else
				key.upper |= (unsigned long long)
					(((unsigned long long)1) << (i - 64 - 1));
			symbol = ir_next_reader_clr_inc(rd);
			i ++;
			continue;
		}
	
		if (i == ip->attr.second_header_at) {
			hiir_debug("%s->%d, full parse skip second_header! i:%d, ip->attr.second_header_at:%d, "\
				"symbol:[l, u]: [%d, %d]\n",
				__func__,__LINE__,
				i, ip->attr.second_header_at,
				(u32)symbol->lower, (u32)symbol->upper);
			symbol = ir_next_reader_clr_inc(rd);
			continue;
		}

		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
		hiir_info("%s->%d, NEC : unkown symbol[l, u]: [%d, %d],"\
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
		/* if a repeat key. */
		if ((last_key->upper || last_key->lower)
			&& last_key->upper == key.upper
			&& last_key->lower == key.lower
			&& last_key->key_stat != KEY_STAT_UP){
			/* should i send a repeat key? */
			if (ir->key_repeat_event && time_after(jiffies,
					nec_repeat_next_time[idx])) {

				last_key->key_stat = KEY_STAT_HOLD;
				ir_insert_key_tail(wr, last_key);
				symbol = ir_next_reader_clr_inc(rd);
				nec_repeat_next_time[idx] = jiffies + 
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
			if (ir->key_up_event) {
				last_key->key_stat = KEY_STAT_UP;
				ir_insert_key_tail(wr, last_key);
				cnt ++;
			}
		}

		hiir_debug("key parsed:[l:%llu, u:%llu, s:%d, p:%s]!\n",
			key.lower, key.upper, key.key_stat, key.protocol_name);

		key.key_stat = KEY_STAT_DOWN;
		ir_insert_key_tail(wr, &key);
		symbol = ir_next_reader_clr_inc(rd);
		nec_repeat_next_time[idx] = jiffies + 
			msecs_to_jiffies(ir->key_repeat_interval);

		memcpy(last_key, &key, sizeof(struct key_attr));
		cnt ++;
	}
start_timer_out:
	del_timer(&nec_timer[idx]);
	nec_timer[idx].expires = jiffies +
		msecs_to_jiffies(ir->key_hold_timeout_time);
	nec_timer[idx].data = idx;
	add_timer(&nec_timer[idx]);

out:
	return fail ? -1 : (cnt ? 0 : -1);
}

EXPORT_SYMBOL(nec_match);
EXPORT_SYMBOL(nec_frame_simple_parse);
EXPORT_SYMBOL(nec_frame_full_parse);
EXPORT_SYMBOL(nec_2headers_match);
EXPORT_SYMBOL(nec_frame_2headers_simple_parse);
EXPORT_SYMBOL(nec_frame_2headers_full_parse);

