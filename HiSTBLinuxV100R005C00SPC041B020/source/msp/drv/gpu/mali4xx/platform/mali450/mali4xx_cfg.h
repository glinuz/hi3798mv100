#ifndef __S40V200_CFG_H__
#define __S40V200_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define GPU_DVFS_ENABLE
#ifndef CHIP_TYPE_hi3798mv200
#define GPU_AVS_SUPPORT
#define GPU_NO_AVS_PROFILE 3
#endif
/*#define GPU_DVFS_SLOW*/

#define GPU_PP_SCALING_ENABLE
#define GPU_MAXFREQ_CONFIG_SUPPORT

/* #define GPU_PMU_HISI */
/* #define S40V200_VMIN_TEST */

#define GPU_DEDICATE_MEM_START 0x0
#define GPU_DEDICATE_MEM_SIZE 0x0

#ifdef __cplusplus
}
#endif
#endif
