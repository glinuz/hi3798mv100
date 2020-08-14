#include "arm64_main.h"

#define REG_BASE_CPU_LP                                 0xF8A22048
#define REG_CPU_LP_CPU_SW_BEGIN                         10
#define CPU_REG_BASE_RST                                0xF8A22050
#define CPU_REG_COREPO_SRST                             12
#define CPU_REG_CORE_SRST                               8

#define dsb() //__asm__ __volatile__ ("dsb" : : : "memory")

#define readl(_a)        ({ unsigned int __v = (*(volatile unsigned int *)(_a)); dsb(); __v; })
#define writel(_v, _a)   ({  dsb(); (*(volatile unsigned int *)(_a) = (_v)); })

int show_msg(const char * fmt, ...);
void slave_cores_power_up(int cpu)
{
	unsigned int regval, regval_bak;

	/* select 400MHz before start slave cores */
	regval_bak = readl(REG_BASE_CPU_LP);
	writel(0x206, REG_BASE_CPU_LP);
	writel(0x606, REG_BASE_CPU_LP);

	/* clear the slave cpu arm_por_srst_req reset */
	regval = readl(CPU_REG_BASE_RST);
	regval &= ~(1 << (cpu + CPU_REG_COREPO_SRST));
	writel(regval, CPU_REG_BASE_RST);

	/* clear the slave cpu reset */
	regval = readl(CPU_REG_BASE_RST);
	regval &= ~(1 << (cpu + CPU_REG_CORE_SRST));
	writel(regval, CPU_REG_BASE_RST);

	/* restore cpu freq */
	regval = regval_bak & (~(1 << REG_CPU_LP_CPU_SW_BEGIN));
	writel(regval, REG_BASE_CPU_LP);
	writel(regval_bak, REG_BASE_CPU_LP);

}

void arm64boot_main(void)
{
	//(*(volatile unsigned *)0xF8B00000) = '~';
	return;
}

