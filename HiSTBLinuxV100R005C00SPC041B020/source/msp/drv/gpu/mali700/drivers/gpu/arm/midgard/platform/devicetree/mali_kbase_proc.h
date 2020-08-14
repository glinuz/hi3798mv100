#ifndef __MALI_GPU_PROC_H__
#define __MALI_GPU_PROC_H__

#ifdef __cplusplus
extern "C" {
#endif

int kbase_proc_create(void);
int kbase_proc_destroy(void);


int kbase_regulator_get(void);
int kbase_regulator_set(int voltage);
int kbase_clk_get(void);
int kbase_clk_set(int freq);
int kbase_dvfs_enable(int enable);
int kbase_dvfs_status(void);
int kbase_opp_update(int freq, int voltage);
int kbase_get_utilisation(void);
int kbase_power_status(void);

#ifdef __cplusplus
}
#endif
#endif
