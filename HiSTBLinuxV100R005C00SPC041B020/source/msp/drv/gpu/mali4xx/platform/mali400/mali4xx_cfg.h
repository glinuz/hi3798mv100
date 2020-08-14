#ifndef __S40V200_CFG_H__
#define __S40V200_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define S40V200_VMIN_TEST
#define GPU_DVFS_ENABLE
#define GPU_AVS_SUPPORT
#define GPU_PP_SCALING_ENABLE
#define GPU_MAXFREQ_CONFIG_SUPPORT

#if defined (CHIP_TYPE_hi3719mv100) || defined (CHIP_TYPE_hi3718mv100)
 #define GPU_NO_AVS_PROFILE 4   /* 300MHz */
#else
 #define GPU_NO_AVS_PROFILE 5   /* 250MHz */
#endif

#define GPU_DEDICATE_MEM_START 0x0
#define GPU_DEDICATE_MEM_SIZE 0x0

#ifdef __cplusplus
}
#endif
#endif
