/******************************************************************************
 *    COPYRIGHT (C) 2012 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2012-12-22
 *
******************************************************************************/

#ifndef DDR_TRAININGH
#define DDR_TRAININGH

#define DDR_TRAINING_STAT_REG		(0xf800009c) /* SC_GEN7 */
#define DDR_TRAINING_STAT_BITS_RD	(0)
#define DDR_TRAINING_STAT_BITS_WAIT	(1)
#define DDR_TRAINING_STAT_BITS_WS	(2)
#define DDR_TRAINING_STAT_BITS_RS	(3)
#define DDR_TRAINING_STAT_BITS_DES	(4)
struct regval_t {
	unsigned int reg;
	unsigned int val;
};

struct ddrtr_result_t {
	unsigned int count;
#define DDR_TRAINING_MAX_VALUE 2*4*6
	struct regval_t reg[DDR_TRAINING_MAX_VALUE];
	char data[2*1024];
};

struct ddrtr_param_t {
#define DDRTR_PARAM_TRAINING         1
#define DDRTR_PARAM_PRESSURE         2
#define DDRTR_PARAM_ADDRTRAIN        3
	unsigned int cmd;
	union {
		struct {
			unsigned int start;
			unsigned int length;
		} train;
		struct {
			unsigned int mode;
			unsigned int codetype;
			unsigned int burstnum;
			unsigned int count;
			unsigned int changebit;
		} pressure;
	};
};

typedef struct ddrtr_result_t *(*ddrtr_t)(struct ddrtr_param_t *param);

#endif /* DDR_TRAININGH */