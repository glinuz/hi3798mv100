/*-----------------------------------------------------------------
 * timer   Configuration
------------------------------------------------------------------ */ 
#define TIMER_FEQ		HI3798MX_OSC_FREQ
#define TIMER_DIV		(1)			
#define TIMER_NOR_DIVIDER_US	((TIMER_FEQ/TIMER_DIV)/1000/1000)
#define DELAY_10S		(TIMER_NOR_DIVIDER_US*1000*1000*10)

#define TIMER_EN		(1<<7)
#define TIMER_MODE		(1<<6)
#define TIMER_PRE		(0<<2) 
#define TIMER_SIZE		(1<<1)

#define timer_reg_get(offset)	\
				*((volatile unsigned *)(REG_BASE_TIMER01+(offset)))
#define timer_reg_set(offset,val)	\
				(*(volatile unsigned *)(REG_BASE_TIMER01+(offset)))=(val);

static void timer_start(void)
{
	timer_reg_set(REG_TIMER_RELOAD, 0);
	timer_reg_set(REG_TIMER_CONTROL, TIMER_EN  | TIMER_MODE |
				TIMER_PRE | TIMER_SIZE);
	timer_reg_set(REG_TIMER_RELOAD, 0xffffffff);
}

static void timer_stop(void)
{
	timer_reg_set(REG_TIMER_CONTROL, 0);
}

static unsigned long timer_get_val(void)
{
	return 0xffffffff - timer_reg_get(REG_TIMER_VALUE);
}

/* max timeout value is 0xffffffff/0x18= 178 Second*/
static void early_udelay(unsigned int cnt)
{
	timer_start();
	while( timer_get_val() < cnt*TIMER_NOR_DIVIDER_US)
		;
	timer_stop();
}
