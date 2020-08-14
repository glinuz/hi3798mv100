#include "Omega.h"
#include <linux/delay.h>

static unsigned int c_fband_min[13] = {
	/*the reason to use 392000(but not 400000) because cal_rssi will use 400000-8000= 392000;*/
	392000, 440000, 484000, 533000, 587000, 645000, 710000, 782000, 860000, 
	1450000, 1492000, 1660000, 1685000
};

static unsigned int c_fN_min[9] = {
	49000, 74000, 111000, 148000, 222000, 296000, 445000, 573000, 890000
};

static unsigned int c_fUHF_min = 392000;

static unsigned int c_lo_bias[4] = {6, 5, 4, 3};
static unsigned int c_lo_m1200[3] = {3885, 4395, 4905};
static unsigned int c_lo_m2048[3] = {4047, 4578, 5109};

static unsigned char g_clock_mode;
static unsigned int g_fxtal_kHz;
static unsigned int g_fdiv;

unsigned long omega_init(void)
{
	unsigned long error = 0;
	unsigned char buf[4], val;
	unsigned int m_bdry, nc, nv;
	unsigned long tmp_numer, tmp_denom;
	unsigned int cnt;

	/*----- read -----*/
	error = omega_rmr(0xEC86, 1, buf);		/*p_reg_p_tsm_init_clock_mode*/
	if (error) goto exit;
	error = omega_rmr(0xED03, 1, &buf[1]);	/*p_reg_p_fbc_n_code*/
	if (error) goto exit;
	error = omega_rmr(0xED23, 2, &buf[2]);	/*r_reg_r_fbc_m_bdry_7_0*/
	if (error) goto exit;

	/*----- set_clock_mode -----*/
	g_clock_mode = buf[0];

	/*----- set_fref_kHz & m_cal -----*/
	switch(g_clock_mode) {
		case 0:	g_fxtal_kHz = 2000;	g_fdiv = 3;	val = 16;	break;	/*12.00MHz, 12000/18 = 2000/3*/
		case 1:	g_fxtal_kHz = 640;	g_fdiv = 1;	val = 6;	break;	/*20.48MHz, 20480/32 = 640/1*/
		default:g_fxtal_kHz = 640;	g_fdiv = 1;	val = 6;	break;	/*20.48MHz, 20480/32 = 640/1*/
	}

	/*----- set_fbdry -----*/
	nc = buf[1];
	nv = omega_get_nv(nc);
	m_bdry = (buf[3]<<8) + buf[2];	/*m_bdry = m_bdry_15_8 << 8 + m_bdry_7_0*/

	/*----- patch to wait for fbc done -----*/
	cnt = 1;
	while (m_bdry == 0 && cnt < OMEGA_TUNER_INIT_POLL_COUNTS) {
		//Sleep(OMEGA_TUNER_INIT_POLL_INTERVAL);
		error = omega_rmr(0xED23, 2, &buf[2]);	/*r_reg_r_fbc_m_bdry_7_0*/
		if (error) goto exit;
		m_bdry = (buf[3]<<8) + buf[2];	/*m_bdry = m_bdry_15_8 << 8 + m_bdry_7_0*/
		cnt++;
	}
	if (m_bdry == 0) {
		error = Error_OMEGA_TUNER_INIT_FAIL;
		goto exit;
	}

	tmp_numer = (unsigned long)g_fxtal_kHz * (unsigned long)m_bdry;
	tmp_denom = (unsigned long)g_fdiv * (unsigned long)nv;
	c_fN_min[7] = (unsigned int) (tmp_numer / tmp_denom);

	/*----- patch to wait for all cal done, borrow p_reg_p_tsm_init_mode -----*/
	cnt = 1;
	error = omega_rmr(0xEC82, 1, buf);	/*p_reg_p_tsm_init_mode*/
	if (error) goto exit;
	while (buf[0] == 0 && cnt < OMEGA_TUNER_INIT_POLL_COUNTS) {
		//Sleep(OMEGA_TUNER_INIT_POLL_INTERVAL);
		error = omega_rmr(0xEC82, 1, buf);	/*p_reg_p_tsm_init_mode*/
		if (error) goto exit;
		cnt++;
	}
	if (buf[0] == 0) {
		error = Error_OMEGA_TUNER_INIT_FAIL;
		goto exit;
	}

	/*----- write -----*/
	error = omega_wmr(0xED81, 1, &val);		/*p_reg_p_iqik_m_cal*/

exit:
	return (error);
}
unsigned long omega_setfreq(unsigned int bw_kHz, unsigned int rf_freq_kHz)
{
	unsigned long error = 0;
	unsigned char val[7];
	unsigned char buf[2];
	unsigned int tmp;

	/*----- read -----*/
	error = omega_rmr(0xEC4C, 1, buf);		/*p_reg_t_ctrl*/
	if (error) goto exit;
	error = omega_rmr(0xED81, 1, &buf[1]);	/*p_reg_p_iqik_m_cal*/
	if (error) goto exit;

	val[0] = omega_get_lna_cap_sel(rf_freq_kHz);
	val[1] = omega_get_lpf_bw(bw_kHz);

	/*----- set_rf_mode -----*/
	//unsigned char ctrl;
	if (rf_freq_kHz < c_fUHF_min) {
		val[2] = buf[0]&0xE7;			/*ctrl<4:3>=0b00 for VHF*/
	} 
	else {
		val[2] = (buf[0]&0xE7) | 0x08;	/*ctrl<4:3>=0b01 for UHF*/
	}

	/*----- set_cal_freq -----*/
	tmp = omega_get_cal_freq_iqik(rf_freq_kHz, buf[1]);
	val[3] = (unsigned char) (tmp & 0xFF);
	val[4] = (unsigned char) ((tmp >> 8) & 0xFF);

	/*----- set_lo_freq -----*/
	tmp = omega_get_lo_freq(rf_freq_kHz);
	val[5] = (unsigned char) (tmp & 0xFF);
	val[6] = (unsigned char) ((tmp >> 8) & 0xFF);

	/*----- write -----*/
	//error = omega_wmr(0xEE06, 1, val);		/*p_reg_p_lnac_lna_cap_sel*/
	error = Standard_writeRegisters (Afatech_OMEGA_demodulator, Processor_OFDM, var_pre_lna_cap_sel, 1, val);
	if (error) goto exit;
	error = omega_wmr(0xEC56, 1, &val[1]);	/*p_reg_t_lpf_bw*/
	if (error) goto exit;
	error = omega_wmr(0xEC4C, 1, &val[2]);	/*p_reg_t_ctrl*/
	if (error) goto exit;
	/*----- the writing sequence is important for cal_freq and lo_freq, thus separate ----- */
	error = omega_wmr(0xEC4D, 1, &val[3]);	/*p_reg_t_cal_freq_7_0*/
	if (error) goto exit;
	error = omega_wmr(0xEC4E, 1, &val[4]);	/*p_reg_t_cal_freq_15_8*/
	if (error) goto exit;
	//error = omega_wmr(0xEC4F, 1, &val[5]);	/*p_reg_t_lo_freq_7_0*/
	error = Standard_writeRegisters (Afatech_OMEGA_demodulator, Processor_OFDM, var_pre_lo_freq_7_0, 1, &val[5]);
	if (error) goto exit;
	//error = omega_wmr(0xEC50, 1, &val[6]);	/*p_reg_t_lo_freq_15_8*/
	error = Standard_writeRegisters (Afatech_OMEGA_demodulator, Processor_OFDM, var_pre_lo_freq_15_8, 1, &val[6]);

exit:
	return (error);
}

/*omega_rmr (_read _multiple _registers)*/
unsigned long omega_rmr(unsigned long mem_addr, unsigned char len, unsigned char *buf)
{	
	return (Standard_readRegisters (
		Afatech_OMEGA_demodulator, Processor_OFDM, mem_addr, len, buf));
}
/*omega_wmr (_write _multiple _registers)*/
unsigned long omega_wmr(unsigned long mem_addr, unsigned char len, unsigned char *val)
{
	return (Standard_writeRegisters (
		Afatech_OMEGA_demodulator, Processor_OFDM, mem_addr, len, val));
}

unsigned int omega_get_cal_freq_iqik(unsigned int rf_freq_kHz, unsigned char iqik_m_cal)
{
	unsigned int nc, nv, mv, cal_freq;

	cal_freq = omega_get_freq_code(rf_freq_kHz, &nc, &nv, &mv);
	if (g_clock_mode==0 && (iqik_m_cal < OMEGA_IQIK_M_CAL_MID)) {
		mv = mv + ((((unsigned int)iqik_m_cal)*nv*9)>>5);
	}
	else if (g_clock_mode==0 && (iqik_m_cal >= OMEGA_IQIK_M_CAL_MID)) {
		iqik_m_cal = OMEGA_IQIK_M_CAL_MAX - iqik_m_cal;
		mv = mv - ((((unsigned int)iqik_m_cal)*nv*9)>>5);
	}
	else if (g_clock_mode==1 && (iqik_m_cal < OMEGA_IQIK_M_CAL_MID)) {
		mv = mv + ((((unsigned int)iqik_m_cal)*nv)>>1);
	}
	else {	/*(g_clock_mode==1 && (iqik_m_cal >= OMEGA_IQIK_M_CAL_MID))*/
		iqik_m_cal = OMEGA_IQIK_M_CAL_MAX - iqik_m_cal;
		mv = mv - ((((unsigned int)iqik_m_cal)*nv)>>1);
	}
	cal_freq = ((nc&0x07) << 13) + mv;

	return cal_freq;
}
unsigned int omega_get_freq_code(unsigned int rf_freq_kHz, unsigned int *nc, unsigned int *nv, unsigned int *mv)
{
	unsigned int freq_code;
	unsigned long tmp_tg, tmp_cal, tmp_m;
	*nc = omega_get_nc(rf_freq_kHz);
	*nv = omega_get_nv(*nc);
	tmp_tg = (unsigned long)rf_freq_kHz * (unsigned long)(*nv) * (unsigned long)g_fdiv;
	tmp_m = (tmp_tg / (unsigned long)g_fxtal_kHz);
	tmp_cal = tmp_m * (unsigned long)g_fxtal_kHz;
	if ((tmp_tg-tmp_cal) >= (g_fxtal_kHz>>1)) {tmp_m = tmp_m+1;}
	*mv = (unsigned int) (tmp_m);

	freq_code = (((*nc)&0x07) << 13) + (*mv);
	return freq_code;
}
unsigned int omega_get_nc(unsigned int rf_freq_kHz)
{
	unsigned int nc;
	if ((rf_freq_kHz <= c_fN_min[1]))										{nc = 0;}	/*74*/
	else if ((rf_freq_kHz > c_fN_min[1]) && (rf_freq_kHz <= c_fN_min[2]))	{nc = 1;}	/*74 111*/	
	else if ((rf_freq_kHz > c_fN_min[2]) && (rf_freq_kHz <= c_fN_min[3]))	{nc = 2;}	/*111 148*/
	else if ((rf_freq_kHz > c_fN_min[3]) && (rf_freq_kHz <= c_fN_min[4]))	{nc = 3;}	/*148 222*/	
	else if ((rf_freq_kHz > c_fN_min[4]) && (rf_freq_kHz <= c_fN_min[5]))	{nc = 4;}	/*222 296*/	
	else if ((rf_freq_kHz > c_fN_min[5]) && (rf_freq_kHz <= c_fN_min[6]))	{nc = 5;}	/*296 445*/
	else if ((rf_freq_kHz > c_fN_min[6]) && (rf_freq_kHz <= c_fN_min[7]))	{nc = 6;}	/*445 573*/
	else if ((rf_freq_kHz > c_fN_min[7]) && (rf_freq_kHz <= c_fN_min[8]))	{nc = 7;}	/*573 890*/
	else 																	{nc = 8;}	/*L-band*/

	return nc;
}
unsigned int omega_get_nv(unsigned int nc)
{
	unsigned int nv;
	switch(nc) {
		case 0:	nv = 48;	break;
		case 1:	nv = 32;	break;
		case 2:	nv = 24;	break;
		case 3:	nv = 16;	break;
		case 4:	nv = 12;	break;
		case 5:	nv = 8;	break;
		case 6:	nv = 6;	break;
		case 7:	nv = 4;	break;
		case 8: nv = 2; break;	/*L-band*/
		default:	nv = 2;	break;
	}
	return nv;
}


unsigned char omega_get_lna_cap_sel(unsigned int rf_freq_kHz)
{
	unsigned char lna_cap_sel;
	if		(rf_freq_kHz <= c_fband_min[1])									{lna_cap_sel=0;}	/*<=440*/
	else if	(rf_freq_kHz > c_fband_min[1] && rf_freq_kHz <= c_fband_min[2])	{lna_cap_sel=1;}	/*440 484*/
	else if	(rf_freq_kHz > c_fband_min[2] && rf_freq_kHz <= c_fband_min[3])	{lna_cap_sel=2;}	/*484 533*/
	else if	(rf_freq_kHz > c_fband_min[3] && rf_freq_kHz <= c_fband_min[4])	{lna_cap_sel=3;}	/*533 587*/
	else if	(rf_freq_kHz > c_fband_min[4] && rf_freq_kHz <= c_fband_min[5])	{lna_cap_sel=4;}	/*587 645*/ 
	else if	(rf_freq_kHz > c_fband_min[5] && rf_freq_kHz <= c_fband_min[6])	{lna_cap_sel=5;}	/*645 710*/
	else if	(rf_freq_kHz > c_fband_min[6] && rf_freq_kHz <= c_fband_min[7])	{lna_cap_sel=6;}	/*710 782*/
	else																	{lna_cap_sel=7;}	/*>782*/
	return lna_cap_sel;
}

unsigned int omega_get_lo_freq(unsigned int rf_freq_kHz)
{
	unsigned int nc, nv, mv, lo_freq;
	lo_freq = omega_get_freq_code(rf_freq_kHz, &nc, &nv, &mv);
	return lo_freq;
}

unsigned char omega_get_lpf_bw(unsigned int bw_kHz)
{
	unsigned char lpf_bw;
	switch(bw_kHz) {
		case 5000:	lpf_bw = 0;	break;	/*5.0MHz*/
		case 5500:	lpf_bw = 1;	break;	/*5.5MHz*/
		case 6000:	lpf_bw = 2;	break;	/*6.0MHz*/
		case 6500:	lpf_bw = 3;	break;	/*6.5MHz*/
		case 7000:	lpf_bw = 4;	break;	/*7.0MHz*/
		case 7500:	lpf_bw = 5;	break;	/*7.5MHz*/
		case 8000:	lpf_bw = 6;	break;	/*8.0MHz*/
		case 8500:	lpf_bw = 7;	break;	/*8.5MHz*/
		default:	lpf_bw = 6;	break;	/*default: 8MHz*/
	}
	return lpf_bw;
}

unsigned long omega_get_param(OMEGA_Param param, int *int_buf)
{
	unsigned long error = 0;
	//unsigned char buf[16];
	//double rf_gain, total_gain;

//exit:
	return (error);
}

unsigned long omega_set_param(OMEGA_Param param, int *int_val)
{
	unsigned long error = 0;
	//unsigned char val[4];
	//double rf_gain, total_gain;

//exit:
	return (error);
}
