#ifndef __S40V200_REG_H__
#define __S40V200_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mali4xx_cfg.h"

#define HISI_CRG_BASE 0xF8A22000
#define HISI_PMC_BASE 0xF8A23000

typedef enum
{
    CRG_REG_ADDR_FREQ_CFG  = 0x0048,          /* frequency configure */
    CRG_REG_ADDR_SOFT_RST  = 0x00d4,
    CRG_REG_ADDR_LOW_POWER = 0x0124,
    CRG_REG_ADDR_SYS_RB = 0x0154,
    CRG_REG_ADDR_SIZE = 0x0200,
} crg_reg_addr_gpu_addr;

typedef enum
{
    PMC_REG_ADDR_DIV_CLKCFG = 0x000c, /* PERI_PMC3 */
    PMC_REG_ADDR_PWM1_DUTY = 0x001c, /* PERI_PMC7 */
    PMC_REG_ADDR_PWM2_DUTY = 0x0020, /* PERI_PMC8 */
    PMC_REG_ADDR_PWM_MUL = 0x0024, /* PERI_PMC9 */

    PMC_REG_ADDR_POWER_TIME = 0x0048, /* PERI_PMC18 */

    PMC_REG_ADDR_GPU_FREQ = 0x0054, /* PERI_PMC21 */
    PMC_REG_ADDR_HPM1_CTL = 0x0068, /* PERI_PMC26 */
    PMC_REG_ADDR_HPM1_RECORD1 = 0x006c, /* PERI_PMC27 */
    PMC_REG_ADDR_HPM1_RECORD2 = 0x0070, /* PERI_PMC28 */
    PMC_REG_ADDR_HPM1_MONITOR = 0x0074, /* PERI_PMC29 */
    PMC_REG_ADDR_GPU_CTRL = 0x00B0, /* PERI_PMC44 */
    PMC_REG_ADDR_SIZE = 0x0200
} pmc_reg_addr_gpu_addr;

#define GPU_CLOCK_ENABLE_MASK (0x1)
#define GPU_CLOCK_PP0_ENABLE_MASK (0x1 << 8)
#define GPU_CLOCK_PP1_ENABLE_MASK (0x1 << 9)
#define GPU_CLOCK_PP2_ENABLE_MASK (0x1 << 10)
#define GPU_CLOCK_PP3_ENABLE_MASK (0x1 << 11)

#define GPU_CLOCK_ON (0x1)
#define GPU_CLOCK_OFF (0x0)
#define GPU_ALL_RESET_MASK (0x1 << 4)
#define GPU_PP0_RESET_MASK (0x1 << 5)
#define GPU_PP1_RESET_MASK (0x1 << 6)

#define GPU_PMU_CNT_BYPASS_MASK (0x1)
#define GPU_PMU_BYPASS_MASK (0x1 << 1)
#define GPU_PMU_PD_SRST_REQ_MASK (0x1 << 3)

#define GPU_USE_POWER_GOOD_CPU (0x1 << 16)
#define GPU_FREQ_SW_TREND_PMC_MASK (0x1 << 18)
#define GPU_FREQ_SW_PLL_PMC (0x1 << 19)
#define GPU_FREQ_FSM_ENABLE_MASK (0x1 << 20)
#define GPU_FREQ_SW_REQ_PMC_MASK (0x1 << 21)
#define GPU_FREQ_TIME_WAIT_POWER_GOOD_MASK (0x0000ffff)

#ifdef S40V200_FPGA
 #define GPU_FREQ_TIME_WAIT_POWER_GOOD_VALUE 0xFFFF      /* 0x2fff for asic   */
#else
 #define GPU_FREQ_TIME_WAIT_POWER_GOOD_VALUE 0x2fff      /* 0x2fff for asic   */
#endif

#define GPU_BEGIN_CFG_BYPASS_MASK (0x1 << 9)
#define GPU_DIV_CFG_BYPASS_MASK (0x1 << 8)
#define GPU_FREQ_DIV_CFG_CRG_MASK (0x3 << 4)
#define GPU_FREQ_SEL_CFG_CRG_MASK (0x7)

#define GPU_CLK_SW_OK_PMC (0x1 << 16)
#define GPU_PMC_POWER_TIME_VALUE (0x00ffff88)

#ifdef __cplusplus
}
#endif
#endif
