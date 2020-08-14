#ifndef __IR_PROTOCOL_H__
#define __IR_PROTOCOL_H__
/* microsecond on each. */
struct phase_attr
{
	int pluse;
	int space;
	/* this is used to specify delta value of this phase */
	int factor;
	/* atomic cuclated from pluse/spcace/factor */
	int maxp;
	int minp;
	int maxs;
	int mins;
};
struct ir_symbol_attr
{
	struct phase_attr header;
	struct phase_attr second_header;
	int second_header_at;
	struct phase_attr b0;
	struct phase_attr b1;
	struct phase_attr burst;
	struct phase_attr repeat;
	/* mininal time silce, micorsecond unit. */
	int min_slice;
	/* how many bits this protocol will report in a frame. */
	int wanna_bits;
	/* how many symbols passed will meet an burst.
	 * NEC like protocol, this is the same with wanna_bits.
	 * but rc6 may not.
	 */
	int burst_offset;
};
enum INFRARED_FLAG
{
	/* protocol is unique. */
	INFR_UNIQUE = 1,
	/* some protocol is similar to this. */
	INFR_SIMILAR = (1 << 1),
	/* for som reason this protocol cannot used. */
	INFR_UNSUPPORT = (1 << 2),

	INFR_SIMPLE_REPEAT = (1 << 3),

	INFR_FULL_REPEAT = (1 << 4),
	INFR_NO_REPEAT = (1 << 5),
	/* bits number of frame data is not fix. */
	INFR_FLEX_DATA	= (1 << 6),
	
	INFR_HAS_ERR_HANDLE = (1 << 7),
	/* - this is used to significate this protocol 
	 * has some problems and cannot be used. 
	 * - maybe not use.
	 */
	INFR_IFFY = (1 << 31)
};

/* frame parser.
 * -parameters:
 * ir: ir priv data, parser may use key_stat_timer or some other information.
 * prot: infrared code feature. 
 * symbol_head: symbols to parse.
 * key_head: where the keys parsed out to store.
 *
 * - call context:
 *  1.	caller should guarantee an entire frame is received
 * before call this handle.
 * For nec simple repeat like infrared code, caller guarantee at least
 * a full frame or a repeat frame is received. If a full frame is received,
 * this handle should judge a repeat frame is successfully received or not.
 *
 *  2.	irq disabled.
 *
 * - requirement: each handle should increase symb_head->reader
 * while a frame is parsed success.
 *
 * - functional requirements:
 *  1. this handle should parse a frame start from symb_head->reader,
 * if the frame have be parsed successfully, remember to increase
 * symb_head->reader.
 *  2. judge each frame's key state.
 *  3. according ir->key_up_event and ir->key_repeat_event/
 * ir->key_repeat_interval to decide if a key up event will be sent
 * or a repeat key event will be sent, and the frequence of repeat key.
 *  4. formate bit order, big endian or litter endian.
 * - return value:
 *  0 while at least a frame have been parsed successfully.
 *  others on error occured.
 */
struct ir_protocol;
typedef int (*frame_parser)(struct ir_priv *ir, struct ir_protocol *prot,
		struct ir_buffer *symb_head, struct ir_buffer *key_head);

/* phase match result */
enum IR_MATCH_RESULT
{
	/* frame match successful */
	IR_MATCH_MATCH,
	/* more data is needed. cannot judge match or not */
	IR_MATCH_NEED_MORE_DATA,
	/* frame not match. */
	IR_MATCH_NOT_MATCH
};
enum IR_MATCH_TYPE
{
	IR_MTT_HEADER,
	IR_MTT_BURST,
	IR_MTT_FRAME
};
/* phase_match:
 * to decide whether the frame start from buffer->reader
 * is a frame of the protocol ip or a repeat frame start
 * if the infrared code constains repeat frame.
 *
 * - call context: irq disabled.
 */
typedef enum IR_MATCH_RESULT (*phase_match)(enum IR_MATCH_TYPE type,
		struct ir_buffer *buffer, struct ir_protocol *ip);
/* match_error_handle:
 * - case call phase_match fail, driver will call this handle
 * if ip->flag & INFR_HAS_ERR_HANDLE is !0.
 * - return value:
 * While return 0, the driver will try to call frame_parser
 * with @type is set to IR_MTT_FRAME, if @type is IR_MTT_HEADER,
 * driver will call phase_match again but
 * @type is set to IR_MTT_FRAME.
 * While return !0, this frame will be parse to
 * the next infrared code parser.
 * - call context:
 * irq disabled.
 */
typedef enum IR_MATCH_RESULT (*match_error_handle)(enum IR_MATCH_TYPE type,
		struct ir_buffer *buffer,
		struct ir_protocol *ip);
/* parse_error_handle:
 * - case call frame_parser fail, driver will call this handle
 * if ip->flag & INFR_HAS_ERR_HANDLE is !0.
 * - call context:
 * irq disabled
 */
typedef void (*parse_error_handle)(struct ir_priv *ir,
		struct ir_protocol *prot, struct ir_buffer *symb_head,
		struct ir_buffer *key_head);
struct ir_protocol 
{
	struct list_head node;
	//atomic_t refcnt;

	char *ir_code_name;

	enum IR_PROTOCOL_IDX idx;
	enum INFRARED_FLAG flag;
	struct ir_symbol_attr attr;

	phase_match match;
	/* the private data may used by parser
	 * which bind with infra code or protocol.
	 */
	uint priv;
	frame_parser handle;
	match_error_handle match_error;
	parse_error_handle parse_error;

    unsigned int key_hold_timeout_time;
    
	/* Is this protocol disabled ?*/
	int disabled;
};

int ir_protocol_init(void);
int ir_protocol_exit(void);

struct ir_protocol *ir_prot_first(void);
struct ir_protocol *ir_prot_next(struct ir_protocol *curr);

int ir_prot_valid(struct ir_protocol *ip);

int key_match_phase(struct key_attr *key, struct phase_attr *phase);
/*******************************************************************/
/* NEC handles */
/* nec matcher for nec simple and full. */
enum IR_MATCH_RESULT nec_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);
/* nec simple parser. */
int nec_frame_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *readfrom, struct ir_buffer *writeto);
/* nec full parser. */
int nec_frame_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *readfrom, struct ir_buffer *writeto);

#if 0
/* nec matcher for nec full but databits not fix. */
enum IR_MATCH_RESULT nec_flex_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);
/* nec parser for nec full but databits not fix. */
int nec_frame_full_flex_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);
#endif

/* nec protocol with 2headers */
int nec_frame_2headers_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);

int nec_frame_2headers_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);

enum IR_MATCH_RESULT nec_2headers_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);

void nec_init(void);
void nec_exit(void);
/*****************************************************************/
/* RC6 handles */

int parse_rc6(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *symb_head, struct ir_buffer *key_head);

enum IR_MATCH_RESULT rc6_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip);
/* call condition:
 * if rc6_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT rc6_match_error_handle(enum IR_MATCH_TYPE type,
		struct ir_buffer *buffer,
		struct ir_protocol *ip);
/* call condition:
 * if parse_rc6 returns !0, this routine can be called.
 */
void rc6_parse_error_handle(struct ir_priv *ir,
		struct ir_protocol *prot, struct ir_buffer *symb_head,
		struct ir_buffer *key_head);

void rc6_init(void);
void rc6_exit(void);
/*****************************************************************/
/* RC5 handles */

int parse_rc5(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *symb_head, struct ir_buffer *key_head);

enum IR_MATCH_RESULT rc5_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip);
/* call condition:
 * if rc5_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT rc5_match_error_handle(enum IR_MATCH_TYPE type,
		struct ir_buffer *buffer,
		struct ir_protocol *ip);
/* call condition:
 * if parse_rc5 returns !0, this routine can be called.
 */
void rc5_parse_error_handle(struct ir_priv *ir,
		struct ir_protocol *prot, struct ir_buffer *symb_head,
		struct ir_buffer *key_head);

/* Extended rc5 handles */
int parse_extended_rc5(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *symb_head, struct ir_buffer *key_head);

enum IR_MATCH_RESULT extended_rc5_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *head, struct ir_protocol *ip);
/* call condition:
 * if extended_rc5_match return not match, this routine will be called.
 */
enum IR_MATCH_RESULT extended_rc5_match_error_handle(enum IR_MATCH_TYPE type,
		struct ir_buffer *buffer,
		struct ir_protocol *ip);
/* call condition:
 * if extended_rc5_match returns !0, this routine can be called.
 */
void extended_rc5_parse_error_handle(struct ir_priv *ir,
		struct ir_protocol *prot, struct ir_buffer *symb_head,
		struct ir_buffer *key_head);

void rc5_init(void);
void rc5_exit(void);

/*****************************************************************/
/* sony handles */
void sony_init(void);
void sony_exit(void);
enum IR_MATCH_RESULT sony_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);
int sony_frame_full_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);
/*****************************************************************/
/* sony handles */
void tc9012_init(void);
void tc9012_exit(void);
enum IR_MATCH_RESULT tc9012_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);
int tc9012_frame_simple_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);

/*****************************************************************/
/* credit handles */
void credit_init(void);
void credit_exit(void);
enum IR_MATCH_RESULT credit_match(enum IR_MATCH_TYPE type,
		struct ir_buffer *buf, struct ir_protocol *ip);
int credit_frame_parse(struct ir_priv *ir, struct ir_protocol *ip,
		struct ir_buffer *rd, struct ir_buffer *wr);
#endif
