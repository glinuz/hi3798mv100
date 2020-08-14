#ifndef __MALI_KBASE_AVS_H__
#define __MALI_KBASE_AVS_H__

#ifdef __cplusplus
extern "C" {
#endif

int kbase_avs_init(void);
int kbase_avs_deinit(void);
int kbase_avs_sethpm(int freq_khz);
int kbase_avs_start(int freq_khz);
int kbase_avs_enable(int enable);
int kbase_avs_status(void);
unsigned long kbase_svb_reset_voltage(unsigned long freq, unsigned long voltage);

#ifdef __cplusplus
}
#endif
#endif

