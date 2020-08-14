#include <config.h>

.section .image,#alloc

#ifdef CONFIG_DDR_TRAINING_HI3716MV300
.globl hi3716mv300_ddr_training_data_start
hi3716mv300_ddr_training_data_start:
.incbin "../arch/arm/cpu/godbox/ddr_training/hi3716mv300/ddr_training.bin"
.globl hi3716mv300_ddr_training_data_end
hi3716mv300_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HI3716MV300 */

#ifdef CONFIG_DDR_TRAINING_HI3716CV100
.globl hi3716cv100_ddr_training_data_start
hi3716cv100_ddr_training_data_start:
.incbin "../arch/arm/cpu/godbox/ddr_training/hi3716cv100/ddr_training.bin"
.globl hi3716cv100_ddr_training_data_end
hi3716cv100_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HI3716CV100 */

#ifdef CONFIG_DDR_TRAINING_HI3712V100
.globl hi3712v100_ddr_training_data_start
hi3712v100_ddr_training_data_start:
.incbin "../arch/arm/cpu/godbox/ddr_training/hi3712v100/ddr_training.bin"
.globl hi3712v100_ddr_training_data_end
hi3712v100_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HI3712V100 */

#ifdef CONFIG_DDR_TRAINING_S40
.globl s40_ddr_training_data_start
s40_ddr_training_data_start:
.incbin "../arch/arm/cpu/s40/ddr_training/s40/ddr_training.bin"
.globl s40_ddr_training_data_end
s40_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_S40 */

#ifdef CONFIG_DDR_TRAINING_HI3798MX
.globl hi3798mx_ddr_training_data_start
hi3798mx_ddr_training_data_start:
.incbin "../arch/arm/cpu/hi3798mx/ddr_training/hi3798mx/ddr_training.bin"
.globl hi3798mx_ddr_training_data_end
hi3798mx_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HI3798MX */

#ifdef CONFIG_DDR_TRAINING_HIFONE
.globl hifone_ddr_training_data_start
hifone_ddr_training_data_start:
.incbin "../arch/arm/cpu/hifone/ddr_training/hifone/ddr_training.bin"
.globl hifone_ddr_training_data_end
hifone_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HIFONE */

#ifdef CONFIG_DDR_TRAINING_S5
.globl s5_ddr_training_data_start
s5_ddr_training_data_start:
.incbin "../arch/arm/cpu/s5/ddr_training/s5/ddr_training.bin"
.globl s5_ddr_training_data_end
s5_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_S5 */

#ifdef CONFIG_DDR_TRAINING_HI3716MV410
.globl hi3716mv410_ddr_training_data_start
hi3716mv410_ddr_training_data_start:
.incbin "../arch/arm/cpu/hi3716mv410/ddr_training/hi3716mv410/ddr_training.bin"
.globl hi3716mv410_ddr_training_data_end
hi3716mv410_ddr_training_data_end:
#endif /* CONFIG_DDR_TRAINING_HI3716MV410 */

