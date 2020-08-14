
/*-----------------------------------------------------------------
 * timer   Configuration
------------------------------------------------------------------ */ 
#define TIMER_LOAD      (0)
#define TIMER_VALUE     (0x4)
#define TIMER_CONTROL   (0x8)

#define TIMER_EN        (1<<7)
#define TIMER_MODE      (1<<6)
#define TIMER_PRE       (0<<2)         /* no divider */
#define TIMER_SIZE      (1<<1)

#define CFG_CLK_XTAL   24000000

#ifdef CFG_FPGA    
#define TIMER_FEQ       (25000000)
#else
#define TIMER_FEQ       CFG_CLK_XTAL
#endif

#define TIMER_DIV       (1)            /* change from 256 to 1, not divide. */

#ifndef CFG_EDA_VERIFY
#define TIMER_NOR_DIVIDER_US   (TIMER_FEQ/TIMER_DIV/1000000)
#else
#define TIMER_NOR_DIVIDER_US   (1)
#endif

#define REG_TIMER23_BASE    (0xf8a29000)                 /* timer23 base: 0xf8a2_9000 */

#undef reg_get
#define reg_get(addr) (*(volatile unsigned int *)(addr))

#undef reg_set
#define reg_set(addr, val) (*(volatile unsigned int *)(addr) = (val))

typedef unsigned int u32;
typedef unsigned int size_t;

/*
notes: all writable global variables should be placed in bss section, 
should not have default value except zero
*/
extern u32 mod_normal;
static u32 timer_base = 0;

#define timer_reg_get(offset)     reg_get(timer_base + (offset))
#define timer_reg_set(offset,val) reg_set(timer_base + (offset), val)

#define TIMER_NOR_DIVIDER_MS   ((TIMER_FEQ/TIMER_DIV)/1000)
#define TIMER_SLOW_DIVIDER_MS  (TIMER_NOR_DIVIDER_MS)      /* timer clock always is 24MHz */

void timer_init(void)
{
	if (timer_base) {
		return;
	}

	timer_base = REG_TIMER23_BASE;

	timer_reg_set(TIMER_LOAD, 0);
	timer_reg_set(TIMER_CONTROL, TIMER_EN  | TIMER_MODE |
			TIMER_PRE | TIMER_SIZE);
}

void timer_deinit(void)
{
	timer_reg_set(TIMER_CONTROL, 0);
}

void timer_start(void)
{
	timer_reg_set(TIMER_LOAD, 0xffffffff);
}

unsigned long timer_get_val(void)
{
	return 0xffffffff - timer_reg_get(TIMER_VALUE);
}
unsigned long timer_get_divider(void)
{
	return TIMER_NOR_DIVIDER_MS;
}

void udelay(unsigned long us)
{ 
	unsigned long tmp = us*TIMER_NOR_DIVIDER_US;
	timer_start();
	while(timer_get_val() < tmp);
}


