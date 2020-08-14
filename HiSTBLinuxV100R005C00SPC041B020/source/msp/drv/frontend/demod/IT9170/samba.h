#ifndef _SAMBA_H_
#define _SAMBA_H_

#define SAMBA_IQIK_M_CAL_MAX	64
#define SAMBA_IQIK_M_CAL_MID	32
#define SAMBA_TUNER_INIT_POLL_INTERVAL	2
#define SAMBA_TUNER_INIT_POLL_COUNTS	50

#define __SAMBADEMOD_H__
#include "standard.h"

/*----- for patch to wait for tuner init done -----*/
#include "error.h"
#include "user.h"

typedef enum
{
	SAMBA_GAIN,		/*return OMEGA_LNA_GAIN, OMEGA_PGC_GAIN, OMEGA_PGA1_GAIN, and OMEGA_PGA2_GAIN*/
	SAMBA_LNA_GAIN,
	SAMBA_PGC_GAIN,
	SAMBA_PGA1_GAIN,
	SAMBA_PGA2_GAIN,
	SAMBA_RF_BACKOFF,
	SAMBA_BB_BACKOFF,
	SAMBA_DCC,		/*return OMEGA_DCC_I and OMEGA_DCC_Q*/
	SAMBA_DCC_I,
	SAMBA_DCC_Q,
	SAMBA_RF_GMAX,
	SAMBA_TOTAL_GMAX,
	SAMBA_P_INBAND_SHIFT,
	SAMBA_P_ALL,	/*return OMEGA_P_RSSI, OMEGA_P_TOTAL, and OMEGA_P_INBAND*/
} SAMBA_Param;

extern Demodulator* Afatech_SAMBA_demodulator;
extern Byte Afatech_SAMBA_chip;

unsigned long samba_init(void);
unsigned long samba_setfreq(unsigned int bw_kHz, unsigned int rf_freq_kHz);
unsigned long samba_rmr(unsigned long mem_addr, unsigned char len, unsigned char *buf);
unsigned long samba_wmr(unsigned long mem_addr, unsigned char len, unsigned char *val);

unsigned int samba_get_cal_freq_iqik(unsigned int rf_freq_kHz, unsigned char iqik_m_cal);
unsigned int samba_get_freq_code(unsigned int rf_freq_kHz, unsigned int *nc, unsigned int *nv, unsigned int *mv);
unsigned int samba_get_nc(unsigned int rf_freq_kHz);
unsigned int samba_get_nv(unsigned int nc);
unsigned char samba_get_lna_cap_sel(unsigned int rf_freq_kHz);
unsigned int samba_get_lo_freq(unsigned int rf_freq_kHz);
unsigned char samba_get_lpf_bw(unsigned int bw_kHz);

unsigned long samba_get_param(SAMBA_Param param, int *buf);
unsigned long samba_set_param(SAMBA_Param param, int *val);

#endif
