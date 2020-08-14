#ifndef __HI3798MV2X_H__
#define __HI3798MV2X_H__

#define CONFIG_ARCH_HI3798MV2X

#undef CONFIG_BOOT_SIMULATE
#undef CONFIG_FPGA
#define CONFIG_SHOW_AUXCODE_INFO

#ifdef CONFIG_BOOT_SIMULATE
/* reg init debug */
#undef CONFIG_REG_DEBUG
#undef CONFIG_FPGA
#undef CONFIG_SHOW_AUXCODE_INFO
#define step(x)  writel(x, REG_BASE_SCTL + REG_SC_GEN31)
#else
#define step(x)
#endif

#endif /* __HI3798MV2X_H__  */
