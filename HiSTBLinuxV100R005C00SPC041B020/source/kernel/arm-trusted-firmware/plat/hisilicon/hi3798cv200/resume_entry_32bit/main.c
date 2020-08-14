
/* ARM64 REG */
#define REG_PERI_CPU_RVBARADDR		0xf8a80034
#define REG_PERI_CPU_AARCH_MODE		0xf8a80030
#define REG_BASE_SCTL                   0xF8000000
#define REG_SC_GEN30                    0x00F8
/* ARM System counter reg */
#define REG_SYS_CNT_BASE		0xF8BB0000
#define REG_SYS_CNTCR			REG_SYS_CNT_BASE
#define REG_SYS_CNTFID0			(REG_SYS_CNT_BASE + 0x20)

#define dsb() //__asm__ __volatile__ ("dsb" : : : "memory")

#define readl(_a)        ({ unsigned int __v = (*(volatile unsigned int *)(_a)); dsb(); __v; })
#define writel(_v, _a)   ({  dsb(); (*(volatile unsigned int *)(_a) = (_v)); })

int main(void)
{
#if 0
	unsigned int val;

	writel(readl(REG_BASE_SCTL + REG_SC_GEN30), REG_PERI_CPU_RVBARADDR);
	writel(readl(REG_PERI_CPU_AARCH_MODE) | 0xF, REG_PERI_CPU_AARCH_MODE);

	/* Enable System counter */
	writel(0x1, REG_SYS_CNTFID0);
	writel(0x103, REG_SYS_CNTCR);


	/* warm reseting */
	__asm__ __volatile__("isb\n\r"
		"dsb\n\r"
		"mrc p15, 0, %0, c12, c0, 2\n\r"
		"orr %0, %0, #0x3\n\r"
		"mcr p15, 0, %0, c12, c0, 2\n\r"
		"isb\n\r"
		"wfi":"=r"(val)::"cc");
	while(1);
#endif
	return 0;
}

