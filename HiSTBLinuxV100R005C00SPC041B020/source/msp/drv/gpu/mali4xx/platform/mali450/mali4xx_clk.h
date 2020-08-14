#ifndef __S40V200_CLK_H__
#define __S40V200_CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mali_clk{
	char *name ;
	unsigned int rate ;
	unsigned int max_rate;
}mali_clk;

/*initial function*/
void __init  gpu_init_clocks(void);
void gpu_deinit_clocks(void);

void hisi_crg_reset(void);
void hisi_crg_clockon(void);
void hisi_crg_clockoff(void);

void hisi_sec_reset_enable(void);
void hisi_sec_reset_disable(void);

void hisi_pmc_setparameter(void);
void clk_gpu_setdefault(void);

/*AVS function*/
void gpu_vmin_init(void);
void gpu_vmin_deinit(void);

void gpu_set_freq_reg(unsigned int freq);
unsigned int gpu_dvfs_get_freq(void);
void gpu_set_utilization_reg(unsigned int utilization);

int gpu_avs_start(int profile);
void gpu_avs_enable(unsigned int enable);

/*DVFS functions is to  the clock freqency*/
int mali_gpu_set_voltage(unsigned int freq);
int clk_gpu_set_voltage(unsigned int volt);
int clk_gpu_set_rate(struct mali_clk *clk, unsigned rate) ;
unsigned int clk_gpu_get_rate(struct mali_clk *clk) ;
long clk_gpu_round_rate(struct mali_clk *clk, long rate) ;

int clk_gpu_get_index(unsigned rate);

/*standard function to modify the freqency and voltage*/
int gpu_dvfs_set_voltage(unsigned int voltage);
int gpu_dvfs_set_freq(unsigned int freq);
unsigned int gpu_dvfs_get_voltage(void);
unsigned int gpu_dvfs_get_freq(void);

/*others*/
int gpu_svb_identify(void);

#ifdef __cplusplus
}
#endif
#endif
