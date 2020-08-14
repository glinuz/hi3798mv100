#include <linux/platform_device.h>
#include <mach/irqs.h>
#include <linux/hikapi.h>

static struct resource pmu_resource_s40[] = {
	[0] = {
		.start = INTNR_A9_PMU_INT0,
		.end   = INTNR_A9_PMU_INT0,
		.flags = IORESOURCE_IRQ,
	},

	[1] = {
		.start = INTNR_A9_PMU_INT1,
		.end   = INTNR_A9_PMU_INT1,
		.flags = IORESOURCE_IRQ,
	}
};

static struct platform_device A9_pmu_device_s40 = {
	.name = "arm-pmu",
	.id   = -1,
	.resource = pmu_resource_s40,
	.num_resources = ARRAY_SIZE(pmu_resource_s40),
};

static struct resource pmu_resource_s5[] = {
	[0] = {
		.start = INTNR_A9_PMU_INT0,
		.end   = INTNR_A9_PMU_INT0,
		.flags = IORESOURCE_IRQ,
	},

	[1] = {
		.start = INTNR_A9_PMU_INT1,
		.end   = INTNR_A9_PMU_INT1,
		.flags = IORESOURCE_IRQ,
	},

	[2] = {
		.start = INTNR_A9_PMU_INT2,
		.end   = INTNR_A9_PMU_INT2,
		.flags = IORESOURCE_IRQ,
	},
	
	[3] = {
		.start = INTNR_A9_PMU_INT3,
		.end   = INTNR_A9_PMU_INT3,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device A9_pmu_device_s5 = {
	.name = "arm-pmu",
	.id   = -1,
	.resource = pmu_resource_s5,
	.num_resources = ARRAY_SIZE(pmu_resource_s5),
};

static int __init pmu_init(void)
{
	u64 chipid = get_chipid(0ULL);

	if (_HI3798CV100A == chipid || _HI3798CV100 == chipid
		|| chipid == _HI3796CV100) {
		platform_device_register(&A9_pmu_device_s5);
	} else {
		platform_device_register(&A9_pmu_device_s40);
	}
	return 0;
};
arch_initcall(pmu_init);
