
/* NOTE: When you are adding a protocol to this array,
 * please make sure there are no header phase are simlar
 * and wanna_bits is the same.
 *
 * If the case descripted before happened,
 * you should consider the two infra code are the same,
 * and its no necessary to add the new code.
 *
 * - Becareful to modify the factor of header/burst/repeat header.
 *   The factor of these too large will due to one protocol may 
 *   parsed as another.
 */
static struct ir_protocol ir_protocols[] = 
{
#ifdef NEC_SUPPORT
	/* NEC FULL 2headers. wugang */
	{
		{0},				/* list */
		NEC_FULL_2HEADERS,		/* protocol name */
		IR_PROT_COMSIMPLE,		/* protocol */
		(INFR_UNIQUE | INFR_FULL_REPEAT),	/* flag */
		{				/* protocol attr */
			{6000, 2000, 10},	/* header phase */
			{3640, 3640, 10},	/* second header */
			25,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1120, 50},	/* b1 phase */
			{560, 40000, 50},	/* burst phase */
			{0, 0, 0},		/* repeate phase */
			560,			/* time slice */
			40,			/* wanna bits */
			41			/* burst at */
		},
		nec_2headers_match,		/* phase matcher */
		0,				/* private data */
		nec_frame_2headers_full_parse,	/* fram parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
		0,                  /* key_hold_timeout_time, 0 means using the default value */
		0                   /* 0: enable; 1: disable*/
	},
	/* NEC SIMPLE 2headers. changshu */
	{
		{0},				/* list */
		NEC_SIMPLE_2HEADERS_DISTINGUISH_REPEAT,	/* protocol name */
		IR_PROT_COMSIMPLE,		/* protocol */
		(INFR_UNIQUE |INFR_SIMPLE_REPEAT),	/* flag */
		{				/* protocol attr */
			{3640, 1800, 10},	/* header phase */
			{3640, 1800, 10},	/* second header */
			25,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1120, 50},	/* b1 phase */
			{560, 40000, 50},	/* burst phase */
			{3640, 3640, 10},	/* repeate phase */
			560,			/* time slice */
			40,			/* wanna bits */
			41			/* burst at */
		},
		nec_2headers_match,		/* phase matcher */
		1,				/* private data */
		nec_frame_2headers_simple_parse,		/* fram parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */		
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/
	},
	/* NEC simple repeate constains
	 * uPD6121G/D6121/BU5777/D1913
	 */
	{
		{0},				/* list */
		NEC_SIMPLE_UPD6121G,	/* protocol name */
		IR_PROT_COMSIMPLE,		/* protocol */
		(INFR_UNIQUE |INFR_SIMPLE_REPEAT),	/* flag */
		{				/* protocol attr */
			{9000, 4500, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1690, 50},	/* b1 phase */
			{560, 40000, 50},	/* burst phase */
			{9000, 2250, 20},	/* repeate phase */
			560,			/* time slice */
			32,			/* wanna bits */
			32			/* burst at */
		},
		nec_match,			/* phase matcher */
		2,				/* private data */
		nec_frame_simple_parse,		/* fram parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
		200,                        /* key_hold_timeout_time */
		0                           /* 0: enable; 1: disable*/
	},
	/* NEC simple repeat, LC7461M-C13 */
	{
		{0},				/* list */
		NEC_SIMPLE_LC7461MCL13,	/* protocol name */
		IR_PROT_COMSIMPLE,		/* protocol */
		(INFR_UNIQUE | INFR_SIMPLE_REPEAT),	/* flag */
		{				/* protocol attr */
			{9000, 4500, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1690, 50},	/* b1 phase */
			{560, 40000, 50},	/* burst phase */
			{9000, 2250, 10},	/* repeate phase */
			560,			/* time slice */
			42,			/* wanna bits */
			42			/* burst at */
		},
		nec_match,			/* phase matcher */
		3,				/* private data */
		nec_frame_simple_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */		
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/
	},
	/* dezhou */
	{
		{0},				/* list */
		NEC_FULL_DEZHOU,		/* protocol name */
		IR_PROT_COMFULL,		/* protocol */
		INFR_UNIQUE | INFR_FULL_REPEAT,	/* flag */
		{				/* protocol attr */
			{3500, 1700, 25},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{400, 400, 50},		/* b0 phase */
			{400, 1350, 50},	/* b1 phase */
			{400, 48000, 25},	/* burst phase */
			{0, 0},			/* repeate phase */
			400,			/* time slice */
			48,			/* wanna bits */
			48			/* burst at */
		},
		nec_match,			/* phase matcher */
		4,				/* private data */
		nec_frame_full_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/
	},
	/* huizhou */
	{
		{0},				/* list */
		NEC_FULL_HUIZHOU,	/* protocol name */
		IR_PROT_COMFULL,		/* protocol */
		INFR_UNIQUE | INFR_FULL_REPEAT,	/* flag */
		{				/* protocol attr */
			{5000, 5000, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{260, 510, 50},		/* b0 phase */
			{520, 1000, 50},	/* b1 phase */
			{520, 20000, 10},	/* burst phase */
			{0, 0},			/* repeate phase */
			260,			/* time slice */
			12,			/* wanna bits */
			12			/* burst at */
		},
		nec_match,			/* phase matcher */
		5,				/* private data */
		nec_frame_full_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/		
	},
	/* NEC SIMPLE 2headers. guangdong */
	{
		{0},				/* list */
		NEC_SIMPLE_2HEADERS_SAME_REPEAT,	/* protocol name */
		IR_PROT_COMSIMPLE,		/* protocol */
		(INFR_UNIQUE |INFR_SIMPLE_REPEAT),	/* flag */
		{				/* protocol attr */
			{3640, 1800, 10},	/* header phase */
			{3640, 1800, 10},	/* second header */
			25,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1120, 50},	/* b1 phase */
			{560, 40000, 50},	/* burst phase */
			{3640, 1800, 10},	/* repeate phase */
			560,			/* time slice */
			40,			/* wanna bits */
			41			/* burst at */
		},
		nec_2headers_match,		/* phase matcher */
		6,				/* private data */
		nec_frame_2headers_simple_parse,		/* fram parser */
		NULL,				/* match error handle */
		NULL,   			/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/		
	},
#endif

#ifdef CREDIT_SUPPORT
	/* credit */
	{
		{0},				/* list */
		CREDIT_CARD,			/* protocol name */
		IR_PROT_CREDIT,			/* protocol */
		INFR_UNIQUE ,			/* flag */
		{				/* protocol attr */
			{420, 420, 30},		/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{420, 420, 30},		/* b0 phase */
			{420, 1620, 30},	/* b1 phase */
			{420, 1000, 15},	/* burst phase */
			{0, 0},			/* repeate phase */
			420,			/* time slice */
			8,			/* wanna bits */
			9			/* burst at */
		},
		credit_match,			/* phase matcher */
		0,				/* private data */
		credit_frame_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */		
		1				    /* default disabled */
	},
#endif

#ifdef SONY_SUPPORT

	/* sony d7c5 */
	{
		{0},				/* list */
		SONY_D7C5,			/* protocol name */
		IR_PROT_SONY,			/* protocol */
		INFR_UNIQUE | INFR_FULL_REPEAT,	/* flag */
		{				/* protocol attr */
			{2400, 600, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{600, 600, 20},		/* b0 phase */
			{1200, 600, 20},	/* b1 phase */
			{600, 20000, 20},	/* burst phase */
			{0, 0},			/* repeate phase */
			600,			/* time slice */
			12,			/* wanna bits */
			12			/* burst at */
		},
		sony_match,			/* phase matcher */
		0,				/* private data */
		sony_frame_full_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/      		
	},
	/* sony d7c8 */
	{
		{0},				/* list */
		SONY_D7C8,			/* protocol name */
		IR_PROT_SONY,			/* protocol */
		INFR_UNIQUE | INFR_FULL_REPEAT,	/* flag */
		{				/* protocol attr */
			{2400, 600, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{600, 600, 20},		/* b0 phase */
			{1200, 600, 20},	/* b1 phase */
			{600, 20000, 20},	/* burst phase */
			{0, 0},			/* repeate phase */
			600,			/* time slice */
			15,			/* wanna bits */
			15			/* burst at */
		},
		sony_match,			/* phase matcher */
		1,				/* private data */
		sony_frame_full_parse,		/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/              		
	},
#endif

#ifdef TC9012_SUPPORT
	/* tc9012 */
	{
		{0},				/* list */
		TC9012,			/* protocol name */
		IR_PROT_TC9012,			/* protocol */
		INFR_UNIQUE | INFR_FULL_REPEAT,	/* flag */
		{				/* protocol attr */
			{4500, 4500, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{560, 560, 50},		/* b0 phase */
			{560, 1690, 50},	/* b1 phase */
			{560, 40000, 40},	/* burst phase */
			{4500, 4500, 10},	/* repeate phase */
			560,			/* time slice */
			32,			/* wanna bits */
			32			/* burst at */
		},
		tc9012_match,			/* phase matcher */
		0,				/* private data */
		tc9012_frame_simple_parse,	/* frame parser */
		NULL,				/* match error handle */
		NULL,				/* parse error handle */
        0,                  /* key_hold_timeout_time, 0 means using the default value */
        0                   /* 0: enable; 1: disable*/              		
	},
#endif

#ifdef RC6_SUPPORT
	/* rc6 */				/* BGCTV - gehuazaixian */
	{
		{0},				/* list */
		RC6_32BIT,		/* protocol name */
		IR_PROT_RC6,			/* protocol */
		INFR_UNIQUE | INFR_NO_REPEAT 
			| INFR_HAS_ERR_HANDLE,	/* flag */
		{				/* protocol attr */
			{2666, 889, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{0, 0, 0},		/* b0 phase */
			{0, 0, 0},		/* b1 phase */
			{0, 40000, 50},		/* burst phase */
			{0, 0},			/* repeate phase */
			445,			/* time slice */
			32,			/* wanna bits */
			32			/* burst at */
		},
		rc6_match,			/* phase matcher */
		0,				/* private data */
		parse_rc6,			/* frame parser */
		rc6_match_error_handle,		/* match error handle */
		rc6_parse_error_handle,		/* parse error handle */
        0,                          /* key_hold_timeout_time, 0 means using the default value */
        0                           /* 0: enable; 1: disable*/              

	},
	/* rc6 3 */				/* chuangwei TATA(Sky */
	{
		{0},				/* list */		
		RC6_20BIT,		/* protocol name */
		IR_PROT_RC6,			/* protocol */
		INFR_UNIQUE | INFR_NO_REPEAT 
			| INFR_HAS_ERR_HANDLE,	/* flag */
		{				/* protocol attr */
			{2666, 889, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{0, 0, 0},		/* b0 phase */
			{0, 0, 0},		/* b1 phase */
			{0, 40000, 50},		/* burst phase */
			{0, 0},			/* repeate phase */
			445,			/* time slice */
			20,			/* wanna bits */
			24			/* burst at */
		},
		rc6_match,			/* phase matcher */
		1,				/* private data */
		parse_rc6,			/* frame parser */
		rc6_match_error_handle,		/* match error handle */
		rc6_parse_error_handle,		/* parse error handle */
        0,                          /* key_hold_timeout_time, 0 means using the default value */
        0                           /* 0: enable; 1: disable*/              
	},
	/* rc6 2 */				/* philips dvd player */
	{
		{0},				/* list */
		RC6_16BIT,		/* protocol name */
		IR_PROT_RC6,			/* protocol */
		INFR_UNIQUE | INFR_NO_REPEAT 
			| INFR_HAS_ERR_HANDLE,	/* flag */
		{				/* protocol attr */
			{2666, 889, 10},	/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{0, 0, 0},		/* b0 phase */
			{0, 0, 0},		/* b1 phase */
			{0, 40000, 50},		/* burst phase */
			{0, 0},			/* repeate phase */
			445,			/* time slice */
			16,			/* wanna bits */
			16			/* burst at */
		},
		rc6_match,			/* phase matcher */
		2,				/* private data */
		parse_rc6,			/* frame parser */
		rc6_match_error_handle,		/* match error handle */
		rc6_parse_error_handle,		/* parse error handle */
        0,                          /* key_hold_timeout_time, 0 means using the default value */
        0                           /* 0: enable; 1: disable*/              
	},
#endif

#ifdef RC5_SUPPORT
	/* rc5 */
	{
		{0},				/* list */
		RC5_14BIT,		/* protocol name */
		IR_PROT_RC5,			/* protocol */
		INFR_UNIQUE | INFR_NO_REPEAT 
			| INFR_HAS_ERR_HANDLE,	/* flag */
		{				/* protocol attr */
			{889, 889, 20},		/* header phase */
			{0, 0, 0},		/* second header */
			0,			/* second header at */
			{889, 889, 50},		/* b0 phase */
			{889, 889, 50},		/* b1 phase */
			{889, 40000, 50},	/* burst phase */
			{0, 0},			/* repeate phase */
			889,			/* time slice */
			14,			/* wanna bits */
			11			/* burst at */
		},
		rc5_match,			/* phase matcher */
		0,				/* private data */
		parse_rc5,			/* frame parser */
		rc5_match_error_handle,		/* match error handle */
		rc5_parse_error_handle,		/* parse error handle */
        0,                          /* key_hold_timeout_time, 0 means using the default value */
        0                           /* 0: enable; 1: disable*/              
	},

    /* Extended rc5 */
    {
        {  0},              /* list */
        EXTENDED_RC5_14BIT,     /* protocol name */
        IR_PROT_RC5,            /* protocol */
        INFR_UNIQUE | INFR_NO_REPEAT
        | INFR_HAS_ERR_HANDLE,      /* flag */
        {               /* protocol attr */
            {1750, 0, 20},      /* header phase */
            {0, 0, 0},      /* second header */
            0,          /* second header at */
            {889, 889, 50},     /* b0 phase */
            {889, 889, 50},     /* b1 phase */
            {889, 90000, 50},   /* burst phase */
            {0, 0},         /* repeate phase */
            889,            /* time slice */
            14,         /* wanna bits */
            11          /* burst at */
        },
        extended_rc5_match,     /* phase matcher */
        1,              /* private data */
        parse_extended_rc5,     /* frame parser */
        extended_rc5_match_error_handle,    /* match error handle */
        extended_rc5_parse_error_handle,    /* parse error handle */
        0,                                  /* key_hold_timeout_time, 0 means using the default value */
        0                                   /* 0: enable; 1: disable*/              
    },
#endif
	/* DO NOT REMOVE THIS. */
	{{0}, NULL, IR_PROT_BUTT, INFR_IFFY, {{0}}, NULL, 0, NULL}
};

