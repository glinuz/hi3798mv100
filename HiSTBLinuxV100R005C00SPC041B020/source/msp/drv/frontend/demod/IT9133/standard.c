/********************************************************************************************/
/**                                                                                         */
/**       These functions in this file are used by API.                                     */
/**       Please not modify the content.                                                    */
/**                                                                                         */
/********************************************************************************************/
#define __OMAGADEMOD_H__
#include "standard.h"
#include "cmd.h"
#include "user.h"
#include "tuner.h"

#include <linux/module.h>

#include "firmware.h"

#define NULL 0

#define Standard_MAX_BIT                8
#define Standard_MAX_CLOCK              12
#define Standard_MAX_BAND               3

#if ((OFDM_VERSION3 < 14)||(LL_VERSION3 < 14))
#error Firmware version too old.  Please update Firmware version.
#endif

const Byte Standard_bitMask[Standard_MAX_BIT] = {
	0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF
};

static ClockTable Standard_clockTable[Standard_MAX_CLOCK] =
{
};

const BandTable Standard_bandTable[Standard_MAX_BAND] =
{
	{    30000,     300000     },      /** VHF    30MHz ~ 300MHz*/
	{    300000,    1000000    },      /** UHF    300MHz ~ 1000MHz*/
	{   1670000,    1680000    }       /** L-BAND */
};


/** local functions */


Dword Standard_divider (
	IN  Demodulator*    demodulator,
	IN  Dword           a,
	IN  Dword           b,
	IN  Dword           x
) {
	Dword answer = 0;
	Dword c = 0;
	Dword i = 0;

	if (a > b) {
		c = a / b;
		a = a - c * b;
	}

	for (i = 0; i < x; i++) {
		if (a >= b) {
			answer += 1;
			a-=b;
		}
		a <<= 1;
		answer <<= 1;
	}

	answer = (c << (Long) x) + answer;

	return (answer);
}


Dword Standard_computeCrystal (
	IN  Demodulator*    demodulator,
	IN  Long            crystalFrequency,   /** Crystal frequency (Hz) */
	OUT Dword*          crystal
) {
	Dword   error = Error_NO_ERROR;

	*crystal = (Long) Standard_divider (demodulator, (Dword) crystalFrequency, 1000000ul, 19ul);

	return (error);
}


Dword Standard_computeAdc (
	IN  Demodulator*    demodulator,
	IN  Long            adcFrequency,       /** ADC frequency (Hz) */
	OUT Dword*          adc
)
{
	Dword   error = Error_NO_ERROR;

	*adc = (Long) Standard_divider (demodulator, (Dword) adcFrequency, 1000000ul, 19ul);

	return (error);
}


Dword Standard_computeFcw (
	IN  Demodulator*    demodulator,
	IN  Long            adcFrequency,       /** ADC frequency (Hz)    */
	IN  Long            ifFrequency,        /** IF frequency (Hz)     */
	IN  Bool            inversion,          /** RF spectrum inversion */
	OUT Dword*          fcw
) {
	Dword error = Error_NO_ERROR;
	Long ifFreq;
	Long adcFreq;
	Long adcFreqHalf;
	Long adcFreqSample;
	Long invBfs;
	Long controlWord;
	Byte adcMultiplier;

	adcFreq = adcFrequency;
	ifFreq = ifFrequency;
	adcFreqHalf = adcFreq / 2;

	if (inversion == True)
		ifFreq = -1 * ifFreq;

	adcFreqSample = ifFreq;

	if (adcFreqSample >= 0)
		invBfs = 1;
	else {
		invBfs = -1;
		adcFreqSample = adcFreqSample * -1;
	}

	while (adcFreqSample > adcFreqHalf)
		adcFreqSample = adcFreqSample - adcFreq;

	/** Sample, spectrum at positive frequency */
	if(adcFreqSample >= 0)
		invBfs = invBfs * -1;
	else {
		invBfs = invBfs * 1;
		adcFreqSample = adcFreqSample * (-1);       /** Absolute value */
	}

	controlWord = (Long) Standard_divider (demodulator, (Dword) adcFreqSample, (Dword) adcFreq, 23ul);

	if (invBfs == -1) {
		controlWord *= -1;
	}

	/** Get ADC multiplier */
	error = Standard_readRegister (demodulator, Processor_OFDM, adcx2, &adcMultiplier);
	if (error) goto exit;

	if (adcMultiplier == 1) {
		controlWord /= 2;
	}

	*fcw = controlWord & 0x7FFFFF;

exit :
	return (error);
}


Dword Standard_programFcw (
	IN  Demodulator*    demodulator,
	IN  Long            shift,          /** Hz */
	IN  Dword           adcFrequency    /** Hz */
)
{
	Dword error = Error_NO_ERROR;
	Dword fcw;
	Long fcwShift;
	Byte temp0;
	Byte temp1;
	Byte temp2;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Get shift freq */
	fcwShift = (shift * 8 * 1024 + (Long) adcFrequency / (2 * 1024)) / (Long) adcFrequency * 1024;

	fcw  = (Dword) ((Long) demod->fcw + fcwShift);

	temp0 = (Byte) (fcw  & 0x000000FF);
	temp1 = (Byte) ((fcw & 0x0000FF00) >> 8);
	temp2 = (Byte) ((fcw & 0x007F0000) >> 16);

	error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_bfs_fcw_7_0, temp0);
	if (error) goto exit;
	error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_bfs_fcw_15_8, temp1);
	if (error) goto exit;
	error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_bfs_fcw_22_16, temp2);
	if (error) goto exit;

exit :
	return (error);
}


Dword Standard_sendCommand (
	IN  Demodulator*    demodulator,
	OUT Word            command,
	IN  Processor       processor,
	IN  Dword           writeBufferLength,
	IN  Byte*           writeBuffer,
	IN  Dword           readBufferLength,
	OUT Byte*           readBuffer
) {
	Dword error = Error_NO_ERROR;

	if (Cmd_sendCommand != NULL) {
		error = Cmd_sendCommand (demodulator, command, processor, writeBufferLength, writeBuffer, readBufferLength, readBuffer);
	}
	return (error);
}


Dword Standard_selectBandwidth (
	IN  Demodulator*    demodulator,
	IN  Word            bandwidth,          /** KHz              */
	IN  Dword           adcFrequency        /** Hz, ex: 20480000 */
) {
	Dword error = Error_NO_ERROR;
	Dword coeff1_2048Nu;
	Dword coeff1_4096Nu;
	Dword coeff1_8191Nu;
	Dword coeff1_8192Nu;
	Dword coeff1_8193Nu;
	Dword coeff2_2k;
	Dword coeff2_4k;
	Dword coeff2_8k;
	Word bfsfcw_fftindex_ratio;
	Word fftindex_bfsfcw_ratio;

	Byte temp0;
	Byte temp1;
	Byte temp2;
	Byte temp3;
	Byte buffer[36];
	Byte bw;
	Byte adcMultiplier;

	if (bandwidth == 5000)
		bw = 3;
	else if (bandwidth == 6000)
		bw = 0;
	else if (bandwidth == 7000)
		bw = 1;
	else if (bandwidth == 8000)
		bw = 2;
	else {
		error = Error_INVALID_BW;
		goto exit;
	}

	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, bw);
       if (error) goto exit;

	/** Program CFOE */
	if (adcFrequency == 20156250) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x02449b5c;
			coeff1_4096Nu = 0x01224dae;
			coeff1_8191Nu = 0x00912b60;
			coeff1_8192Nu = 0x009126d7;
			coeff1_8193Nu = 0x0091224e;
			coeff2_2k = 0x01224dae;
			coeff2_4k = 0x009126d7;
			coeff2_8k = 0x0048936b;
			bfsfcw_fftindex_ratio = 0x0387;
			fftindex_bfsfcw_ratio = 0x0122;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02b8ba6e;
			coeff1_4096Nu = 0x015c5d37;
			coeff1_8191Nu = 0x00ae340d;
			coeff1_8192Nu = 0x00ae2e9b;
			coeff1_8193Nu = 0x00ae292a;
			coeff2_2k = 0x015c5d37;
			coeff2_4k = 0x00ae2e9b;
			coeff2_8k = 0x0057174e;
			bfsfcw_fftindex_ratio = 0x02f1;
			fftindex_bfsfcw_ratio = 0x015c;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x032cd980;
			coeff1_4096Nu = 0x01966cc0;
			coeff1_8191Nu = 0x00cb3cba;
			coeff1_8192Nu = 0x00cb3660;
			coeff1_8193Nu = 0x00cb3007;
			coeff2_2k = 0x01966cc0;
			coeff2_4k = 0x00cb3660;
			coeff2_8k = 0x00659b30;
			bfsfcw_fftindex_ratio = 0x0285;
			fftindex_bfsfcw_ratio = 0x0196;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x03a0f893;
			coeff1_4096Nu = 0x01d07c49;
			coeff1_8191Nu = 0x00e84567;
			coeff1_8192Nu = 0x00e83e25;
			coeff1_8193Nu = 0x00e836e3;
			coeff2_2k = 0x01d07c49;
			coeff2_4k = 0x00e83e25;
			coeff2_8k = 0x00741f12;
			bfsfcw_fftindex_ratio = 0x0234;
			fftindex_bfsfcw_ratio = 0x01d0;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20187500) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x0243b546;
			coeff1_4096Nu = 0x0121daa3;
			coeff1_8191Nu = 0x0090f1d9;
			coeff1_8192Nu = 0x0090ed51;
			coeff1_8193Nu = 0x0090e8ca;
			coeff2_2k = 0x0121daa3;
			coeff2_4k = 0x0090ed51;
			coeff2_8k = 0x004876a9;
			bfsfcw_fftindex_ratio = 0x0388;
			fftindex_bfsfcw_ratio = 0x0122;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02b7a654;
			coeff1_4096Nu = 0x015bd32a;
			coeff1_8191Nu = 0x00adef04;
			coeff1_8192Nu = 0x00ade995;
			coeff1_8193Nu = 0x00ade426;
			coeff2_2k = 0x015bd32a;
			coeff2_4k = 0x00ade995;
			coeff2_8k = 0x0056f4ca;
			bfsfcw_fftindex_ratio = 0x02f2;
			fftindex_bfsfcw_ratio = 0x015c;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x032b9761;
			coeff1_4096Nu = 0x0195cbb1;
			coeff1_8191Nu = 0x00caec30;
			coeff1_8192Nu = 0x00cae5d8;
			coeff1_8193Nu = 0x00cadf81;
			coeff2_2k = 0x0195cbb1;
			coeff2_4k = 0x00cae5d8;
			coeff2_8k = 0x006572ec;
			bfsfcw_fftindex_ratio = 0x0286;
			fftindex_bfsfcw_ratio = 0x0196;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x039f886f;
			coeff1_4096Nu = 0x01cfc438;
			coeff1_8191Nu = 0x00e7e95b;
			coeff1_8192Nu = 0x00e7e21c;
			coeff1_8193Nu = 0x00e7dadd;
			coeff2_2k = 0x01cfc438;
			coeff2_4k = 0x00e7e21c;
			coeff2_8k = 0x0073f10e;
			bfsfcw_fftindex_ratio = 0x0235;
			fftindex_bfsfcw_ratio = 0x01d0;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20250000) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x0241eb3b;
			coeff1_4096Nu = 0x0120f59e;
			coeff1_8191Nu = 0x00907f53;
			coeff1_8192Nu = 0x00907acf;
			coeff1_8193Nu = 0x0090764b;
			coeff2_2k = 0x0120f59e;
			coeff2_4k = 0x00907acf;
			coeff2_8k = 0x00483d67;
			bfsfcw_fftindex_ratio = 0x038b;
			fftindex_bfsfcw_ratio = 0x0121;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02b580ad;
			coeff1_4096Nu = 0x015ac057;
			coeff1_8191Nu = 0x00ad6597;
			coeff1_8192Nu = 0x00ad602b;
			coeff1_8193Nu = 0x00ad5ac1;
			coeff2_2k = 0x015ac057;
			coeff2_4k = 0x00ad602b;
			coeff2_8k = 0x0056b016;
			bfsfcw_fftindex_ratio = 0x02f4;
			fftindex_bfsfcw_ratio = 0x015b;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x03291620;
			coeff1_4096Nu = 0x01948b10;
			coeff1_8191Nu = 0x00ca4bda;
			coeff1_8192Nu = 0x00ca4588;
			coeff1_8193Nu = 0x00ca3f36;
			coeff2_2k = 0x01948b10;
			coeff2_4k = 0x00ca4588;
			coeff2_8k = 0x006522c4;
			bfsfcw_fftindex_ratio = 0x0288;
			fftindex_bfsfcw_ratio = 0x0195;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x039cab92;
			coeff1_4096Nu = 0x01ce55c9;
			coeff1_8191Nu = 0x00e7321e;
			coeff1_8192Nu = 0x00e72ae4;
			coeff1_8193Nu = 0x00e723ab;
			coeff2_2k = 0x01ce55c9;
			coeff2_4k = 0x00e72ae4;
			coeff2_8k = 0x00739572;
			bfsfcw_fftindex_ratio = 0x0237;
			fftindex_bfsfcw_ratio = 0x01ce;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20583333) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x02388f54;
			coeff1_4096Nu = 0x011c47aa;
			coeff1_8191Nu = 0x008e2846;
			coeff1_8192Nu = 0x008e23d5;
			coeff1_8193Nu = 0x008e1f64;
			coeff2_2k = 0x011c47aa;
			coeff2_4k = 0x008e23d5;
			coeff2_8k = 0x004711ea;
			bfsfcw_fftindex_ratio = 0x039a;
			fftindex_bfsfcw_ratio = 0x011c;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02aa4598;
			coeff1_4096Nu = 0x015522cc;
			coeff1_8191Nu = 0x00aa96bb;
			coeff1_8192Nu = 0x00aa9166;
			coeff1_8193Nu = 0x00aa8c12;
			coeff2_2k = 0x015522cc;
			coeff2_4k = 0x00aa9166;
			coeff2_8k = 0x005548b3;
			bfsfcw_fftindex_ratio = 0x0300;
			fftindex_bfsfcw_ratio = 0x0155;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x031bfbdc;
			coeff1_4096Nu = 0x018dfdee;
			coeff1_8191Nu = 0x00c7052f;
			coeff1_8192Nu = 0x00c6fef7;
			coeff1_8193Nu = 0x00c6f8bf;
			coeff2_2k = 0x018dfdee;
			coeff2_4k = 0x00c6fef7;
			coeff2_8k = 0x00637f7b;
			bfsfcw_fftindex_ratio = 0x0293;
			fftindex_bfsfcw_ratio = 0x018e;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x038db21f;
			coeff1_4096Nu = 0x01c6d910;
			coeff1_8191Nu = 0x00e373a3;
			coeff1_8192Nu = 0x00e36c88;
			coeff1_8193Nu = 0x00e3656d;
			coeff2_2k = 0x01c6d910;
			coeff2_4k = 0x00e36c88;
			coeff2_8k = 0x0071b644;
			bfsfcw_fftindex_ratio = 0x0240;
			fftindex_bfsfcw_ratio = 0x01c7;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20416667) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x023d337f;
			coeff1_4096Nu = 0x011e99c0;
			coeff1_8191Nu = 0x008f515a;
			coeff1_8192Nu = 0x008f4ce0;
			coeff1_8193Nu = 0x008f4865;
			coeff2_2k = 0x011e99c0;
			coeff2_4k = 0x008f4ce0;
			coeff2_8k = 0x0047a670;
			bfsfcw_fftindex_ratio = 0x0393;
			fftindex_bfsfcw_ratio = 0x011f;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02afd765;
			coeff1_4096Nu = 0x0157ebb3;
			coeff1_8191Nu = 0x00abfb39;
			coeff1_8192Nu = 0x00abf5d9;
			coeff1_8193Nu = 0x00abf07a;
			coeff2_2k = 0x0157ebb3;
			coeff2_4k = 0x00abf5d9;
			coeff2_8k = 0x0055faed;
			bfsfcw_fftindex_ratio = 0x02fa;
			fftindex_bfsfcw_ratio = 0x0158;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x03227b4b;
			coeff1_4096Nu = 0x01913da6;
			coeff1_8191Nu = 0x00c8a518;
			coeff1_8192Nu = 0x00c89ed3;
			coeff1_8193Nu = 0x00c8988e;
			coeff2_2k = 0x01913da6;
			coeff2_4k = 0x00c89ed3;
			coeff2_8k = 0x00644f69;
			bfsfcw_fftindex_ratio = 0x028d;
			fftindex_bfsfcw_ratio = 0x0191;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x03951f32;
			coeff1_4096Nu = 0x01ca8f99;
			coeff1_8191Nu = 0x00e54ef7;
			coeff1_8192Nu = 0x00e547cc;
			coeff1_8193Nu = 0x00e540a2;
			coeff2_2k = 0x01ca8f99;
			coeff2_4k = 0x00e547cc;
			coeff2_8k = 0x0072a3e6;
			bfsfcw_fftindex_ratio = 0x023c;
			fftindex_bfsfcw_ratio = 0x01cb;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20480000) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x023b6db7;
			coeff1_4096Nu = 0x011db6db;
			coeff1_8191Nu = 0x008edfe5;
			coeff1_8192Nu = 0x008edb6e;
			coeff1_8193Nu = 0x008ed6f7;
			coeff2_2k = 0x011db6db;
			coeff2_4k = 0x008edb6e;
			coeff2_8k = 0x00476db7;
			bfsfcw_fftindex_ratio = 0x0396;
			fftindex_bfsfcw_ratio = 0x011e;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02adb6db;
			coeff1_4096Nu = 0x0156db6e;
			coeff1_8191Nu = 0x00ab7312;
			coeff1_8192Nu = 0x00ab6db7;
			coeff1_8193Nu = 0x00ab685c;
			coeff2_2k = 0x0156db6e;
			coeff2_4k = 0x00ab6db7;
			coeff2_8k = 0x0055b6db;
			bfsfcw_fftindex_ratio = 0x02fd;
			fftindex_bfsfcw_ratio = 0x0157;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x03200000;
			coeff1_4096Nu = 0x01900000;
			coeff1_8191Nu = 0x00c80640;
			coeff1_8192Nu = 0x00c80000;
			coeff1_8193Nu = 0x00c7f9c0;
			coeff2_2k = 0x01900000;
			coeff2_4k = 0x00c80000;
			coeff2_8k = 0x00640000;
			bfsfcw_fftindex_ratio = 0x028f;
			fftindex_bfsfcw_ratio = 0x0190;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x03924925;
			coeff1_4096Nu = 0x01c92492;
			coeff1_8191Nu = 0x00e4996e;
			coeff1_8192Nu = 0x00e49249;
			coeff1_8193Nu = 0x00e48b25;
			coeff2_2k = 0x01c92492;
			coeff2_4k = 0x00e49249;
			coeff2_8k = 0x00724925;
			bfsfcw_fftindex_ratio = 0x023d;
			fftindex_bfsfcw_ratio = 0x01c9;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20500000) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x023adeff;
			coeff1_4096Nu = 0x011d6f80;
			coeff1_8191Nu = 0x008ebc36;
			coeff1_8192Nu = 0x008eb7c0;
			coeff1_8193Nu = 0x008eb34a;
			coeff2_2k = 0x011d6f80;
			coeff2_4k = 0x008eb7c0;
			coeff2_8k = 0x00475be0;
			bfsfcw_fftindex_ratio = 0x0396;
			fftindex_bfsfcw_ratio = 0x011d;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02ad0b99;
			coeff1_4096Nu = 0x015685cc;
			coeff1_8191Nu = 0x00ab4840;
			coeff1_8192Nu = 0x00ab42e6;
			coeff1_8193Nu = 0x00ab3d8c;
			coeff2_2k = 0x015685cc;
			coeff2_4k = 0x00ab42e6;
			coeff2_8k = 0x0055a173;
			bfsfcw_fftindex_ratio = 0x02fd;
			fftindex_bfsfcw_ratio = 0x0157;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x031f3832;
			coeff1_4096Nu = 0x018f9c19;
			coeff1_8191Nu = 0x00c7d44b;
			coeff1_8192Nu = 0x00c7ce0c;
			coeff1_8193Nu = 0x00c7c7ce;
			coeff2_2k = 0x018f9c19;
			coeff2_4k = 0x00c7ce0c;
			coeff2_8k = 0x0063e706;
			bfsfcw_fftindex_ratio = 0x0290;
			fftindex_bfsfcw_ratio = 0x0190;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x039164cb;
			coeff1_4096Nu = 0x01c8b266;
			coeff1_8191Nu = 0x00e46056;
			coeff1_8192Nu = 0x00e45933;
			coeff1_8193Nu = 0x00e45210;
			coeff2_2k = 0x01c8b266;
			coeff2_4k = 0x00e45933;
			coeff2_8k = 0x00722c99;
			bfsfcw_fftindex_ratio = 0x023e;
			fftindex_bfsfcw_ratio = 0x01c9;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else if (adcFrequency == 20625000) {
		if (bandwidth == 5000) {
			coeff1_2048Nu = 0x02376948;
			coeff1_4096Nu = 0x011bb4a4;
			coeff1_8191Nu = 0x008ddec1;
			coeff1_8192Nu = 0x008dda52;
			coeff1_8193Nu = 0x008dd5e3;
			coeff2_2k = 0x011bb4a4;
			coeff2_4k = 0x008dda52;
			coeff2_8k = 0x0046ed29;
			bfsfcw_fftindex_ratio = 0x039c;
			fftindex_bfsfcw_ratio = 0x011c;
		} else if (bandwidth == 6000) {
			coeff1_2048Nu = 0x02a8e4bd;
			coeff1_4096Nu = 0x0154725e;
			coeff1_8191Nu = 0x00aa3e81;
			coeff1_8192Nu = 0x00aa392f;
			coeff1_8193Nu = 0x00aa33de;
			coeff2_2k = 0x0154725e;
			coeff2_4k = 0x00aa392f;
			coeff2_8k = 0x00551c98;
			bfsfcw_fftindex_ratio = 0x0302;
			fftindex_bfsfcw_ratio = 0x0154;
		} else if (bandwidth == 7000) {
			coeff1_2048Nu = 0x031a6032;
			coeff1_4096Nu = 0x018d3019;
			coeff1_8191Nu = 0x00c69e41;
			coeff1_8192Nu = 0x00c6980c;
			coeff1_8193Nu = 0x00c691d8;
			coeff2_2k = 0x018d3019;
			coeff2_4k = 0x00c6980c;
			coeff2_8k = 0x00634c06;
			bfsfcw_fftindex_ratio = 0x0294;
			fftindex_bfsfcw_ratio = 0x018d;
		} else if (bandwidth == 8000) {
			coeff1_2048Nu = 0x038bdba6;
			coeff1_4096Nu = 0x01c5edd3;
			coeff1_8191Nu = 0x00e2fe02;
			coeff1_8192Nu = 0x00e2f6ea;
			coeff1_8193Nu = 0x00e2efd2;
			coeff2_2k = 0x01c5edd3;
			coeff2_4k = 0x00e2f6ea;
			coeff2_8k = 0x00717b75;
			bfsfcw_fftindex_ratio = 0x0242;
			fftindex_bfsfcw_ratio = 0x01c6;
		} else {
			error = Error_INVALID_BW;
			goto exit;
		}
	} else {
		error = Error_INVALID_XTAL_FREQ;
		goto exit;
	}


	/** Get ADC multiplier */
	error = Standard_readRegister (demodulator, Processor_OFDM, adcx2, &adcMultiplier);
    if (error) goto exit;

	if (adcMultiplier == 1) {
		coeff1_2048Nu /= 2;
		coeff1_4096Nu /= 2;
		coeff1_8191Nu /= 2;
		coeff1_8192Nu /= 2;
		coeff1_8193Nu /= 2 ;
		coeff2_2k /= 2;
		coeff2_4k /= 2;
		coeff2_8k /= 2;
	}

	/** Write coeff1_2048Nu */
	/** Get Byte0 */
	temp0 = (Byte) (coeff1_2048Nu & 0x000000FF);
	/** Get Byte1 */
	temp1 = (Byte) ((coeff1_2048Nu & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff1_2048Nu & 0x00FF0000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff1_2048Nu & 0x03000000) >> 24);

	/** Gig endian to make 8051 happy */
	buffer[cfoe_NS_2048_coeff1_25_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_2048_coeff1_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_2048_coeff1_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_2048_coeff1_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff2_2k */
	/** Get Byte0 */
	temp0 = (Byte) ((coeff2_2k & 0x000000FF));
	/** Get Byte1 */
	temp1 = (Byte) ((coeff2_2k & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff2_2k & 0x00FF0000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff2_2k & 0x01000000) >> 24);

	/** Gig endian to make 8051 happy */
	buffer[cfoe_NS_2k_coeff2_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_2k_coeff2_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_2k_coeff2_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_2k_coeff2_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff1_8191Nu */
	/** Get Byte0 */
	temp0 = (Byte) ((coeff1_8191Nu & 0x000000FF));
	/** Get Byte1 */
	temp1 = (Byte) ((coeff1_8191Nu & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff1_8191Nu & 0x00FFC000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff1_8191Nu & 0x03000000) >> 24);

	/** Big endian to make 8051 happy */
	buffer[cfoe_NS_8191_coeff1_25_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_8191_coeff1_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_8191_coeff1_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_8191_coeff1_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff1_8192Nu */
	/** Get Byte0 */
	temp0  = (Byte) (coeff1_8192Nu & 0x000000FF);
	/** Get Byte1 */
	temp1 = (Byte) ((coeff1_8192Nu & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff1_8192Nu & 0x00FFC000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff1_8192Nu & 0x03000000) >> 24);

	/** Gig endian to make 8051 happy */
	buffer[cfoe_NS_8192_coeff1_25_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_8192_coeff1_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_8192_coeff1_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_8192_coeff1_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff1_8193Nu */
	/** Get Byte0 */
	temp0 = (Byte) ((coeff1_8193Nu & 0x000000FF));
	/** Get Byte1 */
	temp1 = (Byte) ((coeff1_8193Nu & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff1_8193Nu & 0x00FFC000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff1_8193Nu & 0x03000000) >> 24);

	/** Big endian to make 8051 happy */
	buffer[cfoe_NS_8193_coeff1_25_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_8193_coeff1_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_8193_coeff1_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_8193_coeff1_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff2_8k */
	/** Get Byte0 */
	temp0 = (Byte) ((coeff2_8k & 0x000000FF));
	/** Get Byte1 */
	temp1 = (Byte) ((coeff2_8k & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff2_8k & 0x00FF0000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff2_8k & 0x01000000) >> 24);

	/** Big endian to make 8051 happy */
	buffer[cfoe_NS_8k_coeff2_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_8k_coeff2_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_8k_coeff2_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_8k_coeff2_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff1_4096Nu */
	/** Get Byte0 */
	temp0 = (Byte) (coeff1_4096Nu & 0x000000FF);
	/** Get Byte1 */
	temp1 = (Byte) ((coeff1_4096Nu & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff1_4096Nu & 0x00FF0000) >> 16);
	/** Get Byte3[1:0] */
	/** Bit[7:2] will be written soon and so don't have to care them */
	temp3 = (Byte) ((coeff1_4096Nu & 0x03000000) >> 24);

	/** Big endian to make 8051 happy */
	buffer[cfoe_NS_4096_coeff1_25_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_4096_coeff1_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_4096_coeff1_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_4096_coeff1_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Write coeff2_4k */
	/** Get Byte0 */
	temp0 = (Byte) ((coeff2_4k & 0x000000FF));
	/** Get Byte1 */
	temp1 = (Byte) ((coeff2_4k & 0x0000FF00) >> 8);
	/** Get Byte2 */
	temp2 = (Byte) ((coeff2_4k & 0x00FF0000) >> 16);
	/** Get Byte3 */
	temp3 = (Byte) ((coeff2_4k & 0x01000000) >> 24);

	/** Big endian to make 8051 happy */
	buffer[cfoe_NS_4k_coeff2_24 - cfoe_NS_2048_coeff1_25_24] = temp3;
	buffer[cfoe_NS_4k_coeff2_23_16 - cfoe_NS_2048_coeff1_25_24] = temp2;
	buffer[cfoe_NS_4k_coeff2_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[cfoe_NS_4k_coeff2_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Get Byte0 */
	temp0 = (Byte) (bfsfcw_fftindex_ratio & 0x00FF);
	/** Get Byte1 */
	temp1 = (Byte) ((bfsfcw_fftindex_ratio & 0xFF00) >> 8);

	/** Big endian to make 8051 happy */
	buffer[bfsfcw_fftindex_ratio_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[bfsfcw_fftindex_ratio_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	/** Get Byte0 */
	temp0 = (Byte) (fftindex_bfsfcw_ratio & 0x00FF);
	/** Get Byte1 */
	temp1 = (Byte) ((fftindex_bfsfcw_ratio & 0xFF00) >> 8);

	/** Big endian to make 8051 happy */
	buffer[fftindex_bfsfcw_ratio_15_8 - cfoe_NS_2048_coeff1_25_24] = temp1;
	buffer[fftindex_bfsfcw_ratio_7_0 - cfoe_NS_2048_coeff1_25_24] = temp0;

	error = Standard_writeRegisters (demodulator, Processor_OFDM, cfoe_NS_2048_coeff1_25_24, 36, buffer);
      if (error) goto exit;

exit :
	return (error);
}


Dword Standard_setFrequency (
	IN  Demodulator*    demodulator,
	IN  Dword           frequency
) {
	Dword error = Error_NO_ERROR;
	Byte band;
	Byte i;
	DefaultDemodulator* demod = (DefaultDemodulator*)demodulator; 

	/** Clear easy mode flag first */
	error = Standard_writeRegister (demodulator, Processor_OFDM, Training_Mode, 0x00);
      if (error) goto exit;

	/** Clear empty_channel_status lock flag */
	error = Standard_writeRegister (demodulator, Processor_OFDM, empty_channel_status, 0x00);
      if (error) goto exit;

	/** Clear MPEG2 lock flag */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, r_mp2if_sync_byte_locked, mp2if_sync_byte_locked_pos, mp2if_sync_byte_locked_len, 0x00);
      if (error) goto exit;

	/** Determine frequency band */
	band = 0xFF;
	for (i = 0; i < Standard_MAX_BAND; i++) {
		if ((frequency >= Standard_bandTable[i].minimum) && (frequency <= Standard_bandTable[i].maximum)) {
			band = i;
			break;
		}
	}
	error = Standard_writeRegister (demodulator, Processor_OFDM, FreBand, band);
      if (error) goto exit;

	if (tunerDescription.setTunerFunc != NULL) {
		error = tunerDescription.setTunerFunc (demodulator, demod->bandwidth, frequency);
             if (error) goto exit;
	}

	/** Trigger ofsm */
	error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0);
      if (error) goto exit;

	demod->frequency = frequency;

exit :
	return (error);
}


Dword Standard_loadFirmware (
	IN  Demodulator*    demodulator,
	IN  Byte*           firmwareCodes,
	IN  Segment*        firmwareSegments,
	IN  Byte*           firmwarePartitions
) {
	Dword error = Error_NO_ERROR;
	Dword beginPartition;
	Dword endPartition;
	Dword version;
	Dword firmwareLength;
	Byte* firmwareCodesPointer;
	Word command;
	Dword i;
	DefaultDemodulator* demod = (DefaultDemodulator*)demodulator; 

	/** Set I2C master clock speed. */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_one_cycle_counter_tuner, User_I2C_SPEED);
	if (error) goto exit;

	firmwareCodesPointer = firmwareCodes;

	beginPartition = 0;
	endPartition = firmwarePartitions[0];

	for (i = beginPartition; i < endPartition; i++) {
		firmwareLength = firmwareSegments[i].segmentLength;
		if (firmwareSegments[i].segmentType == 0) {
			/** Dwonload firmware */
			error = Standard_sendCommand (demodulator, Command_FW_DOWNLOAD_BEGIN, Processor_LINK, 0, NULL, 0, NULL);
			if (error) goto exit;
			if (Cmd_loadFirmware != NULL) {
				error = Cmd_loadFirmware (demodulator, firmwareLength, firmwareCodesPointer);
			}
			if (error) goto exit;
			error = Standard_sendCommand (demodulator, Command_FW_DOWNLOAD_END, Processor_LINK, 0, NULL, 0, NULL);
			if (error) goto exit;
		} else if (firmwareSegments[i].segmentType == 1) {
			/** Copy firmware */
			error = Standard_sendCommand (demodulator, Command_SCATTER_WRITE, Processor_LINK, firmwareLength, firmwareCodesPointer, 0, NULL);
			if (error) goto exit;
		} else {
			/** Direct write firmware */
			command = (Word) (firmwareCodesPointer[0] << 8) + (Word) firmwareCodesPointer[1];
			error = Standard_sendCommand (demodulator, command, Processor_LINK, firmwareLength - 2, firmwareCodesPointer + 2, 0, NULL);
			if (error) goto exit;
		}
		firmwareCodesPointer += firmwareLength;
	}

	/** Boot */
	error = Standard_sendCommand (demodulator, Command_BOOT, Processor_LINK, 0, NULL, 0, NULL);
	if (error) goto exit;

	User_delay (demodulator, 10);

	/** Check if firmware is running */
	version = 0;
	error = Standard_getFirmwareVersion (demodulator, Processor_LINK, &version);
	if (error) goto exit;
	if (version == 0)
		error = Error_BOOT_FAIL;

exit :
	return (error);
}


Dword Standard_loadScript (
	IN  Demodulator*    demodulator,
	IN  Word*           scriptSets,
	IN  ValueSet*       scripts,
	IN  Word*           tunerScriptSets,
	IN  ValueSet*       tunerScripts
) {
	Dword error = Error_NO_ERROR;
	Word beginScript;
	Word endScript;
	Byte value1 = 0, value2 = 0, supportRelay = 0, bufferLens = 1;
	Word j;
	Byte buffer[20] = {0,};
	Dword tunerAddr, tunerAddrTemp;

	/** Querry SupportRelayCommandWrite **/
	error = Standard_readRegister (demodulator, Processor_OFDM, 0x004D, &supportRelay);
	if (error) goto exit;

	/** Enable RelayCommandWrite **/
	if (supportRelay) {
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0x004E, 1);
		if (error) goto exit;
	}

	if ((scriptSets[0] != 0) && (scripts != NULL)) {
		beginScript = 0;
		endScript = scriptSets[0];

		/** Load OFSM init script */
		for (j = beginScript; j < endScript; j++) {
			tunerAddr = tunerAddrTemp = scripts[j].address;
			buffer[0] = scripts[j].value;

			while (j < endScript && bufferLens < 20) {
				tunerAddrTemp += 1;
				if (tunerAddrTemp != scripts[j+1].address)
					break;

				buffer[bufferLens] = scripts[j+1].value;
				bufferLens ++;
				j ++;
			}

				error = Standard_writeRegisters (demodulator, Processor_OFDM, tunerAddr, bufferLens, buffer);
			if (error) goto exit;
			bufferLens = 1;
		}
	}


	/** Distinguish chip type */
	error = Standard_readRegister (demodulator, Processor_LINK, chip_version_7_0, &value1);
	if (error) goto exit;

	error = Standard_readRegister (demodulator, Processor_LINK, prechip_version_7_0, &value2);
	if (error) goto exit;

	if ((tunerScriptSets[0] != 0) && (tunerScripts != NULL)) {
		if (tunerScriptSets[1] == tunerScriptSets[0] && !(value1 == 0xF8 && value2 == 0xEA)) {
			/** New version */
			beginScript = tunerScriptSets[0];
			endScript = tunerScriptSets[0] + tunerScriptSets[1];
		} else {
			/** Old version */
			beginScript = 0;
			endScript = tunerScriptSets[0];
		}

		/** Load tuner init script */
		for (j = beginScript; j < endScript; j++) {
			tunerAddr = tunerAddrTemp = tunerScripts[j].address;
			buffer[0] = tunerScripts[j].value;

			while (j < endScript && bufferLens < 20) {
				tunerAddrTemp += 1;
				if (tunerAddrTemp != tunerScripts[j+1].address)
					break;

				buffer[bufferLens] = tunerScripts[j+1].value;
				bufferLens ++;
				j ++;
			}

				error = Standard_writeRegisters (demodulator, Processor_OFDM, tunerAddr, bufferLens, buffer);
			if (error) goto exit;
			bufferLens = 1;
		}
	}

	/** Disable RelayCommandWrite **/
	if (supportRelay) {
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0x004E, 0);
		if (error) goto exit;
	}

exit :
	return (error);
}

/** end of local functions */

Dword Standard_writeRegister (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	IN  Byte            value
) {
	return (Standard_writeRegisters (demodulator, processor, registerAddress, 1, &value));
}


Dword Standard_writeRegisters (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	IN  Byte            bufferLength,
	IN  Byte*           buffer
) {
	Dword error = Error_NO_ERROR;

	Byte registerAddressLength;

	if (processor == Processor_LINK) {
		if (registerAddress > 0x000000FF) {
			registerAddressLength = 2;
		} else {
			registerAddressLength = 1;
		}
	} else {
			registerAddressLength = 2;
	}
	if (Cmd_writeRegisters != NULL) {
		error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, bufferLength, buffer);
	}

	return (error);
}


Dword Standard_writeRegisterBits (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	IN  Byte            position,
	IN  Byte            length,
	IN  Byte            value
) {
	Dword error = Error_NO_ERROR;

	Byte registerAddressLength;
	Byte temp;

	if (processor == Processor_LINK) {
		if (registerAddress > 0x000000FF) {
			registerAddressLength = 2;
		} else {
			registerAddressLength = 1;
		}
	} else {
		registerAddressLength = 2;
	}
	if (length == 8) {
		if (Cmd_writeRegisters != NULL) {
			error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &value);
		}
	} else {
		if (Cmd_readRegisters != NULL) {
			error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
			if (error) goto exit;
		}

		temp = REG_CREATE (value, temp, position, length);

		if (Cmd_writeRegisters != NULL) {
			error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
			if (error) goto exit;
		}
	}
exit:
	return (error);
}


Dword Standard_readRegister (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	OUT Byte*           value
) {
	return (Standard_readRegisters (demodulator, processor, registerAddress, 1, value));
}


Dword Standard_readRegisters (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	IN  Byte            bufferLength,
	OUT Byte*           buffer
) {
	Dword error = Error_NO_ERROR;

	Byte registerAddressLength;

	if (processor == Processor_LINK) {
		if (registerAddress > 0x000000FF) {
			registerAddressLength = 2;
		} else {
			registerAddressLength = 1;
		}
	} else {
		registerAddressLength = 2;
	}
	if (Cmd_readRegisters != NULL) {
		error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, bufferLength, buffer);
	}

	return (error);
}


Dword Standard_readRegisterBits (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	IN  Dword           registerAddress,
	IN  Byte            position,
	IN  Byte            length,
	OUT Byte*           value
) {
	Dword error = Error_NO_ERROR;

	Byte temp = 0;
	Byte registerAddressLength;

	if (processor == Processor_LINK) {
		if (registerAddress > 0x000000FF) {
			registerAddressLength = 2;
		} else {
			registerAddressLength = 1;
		}
	} else {
		registerAddressLength = 2;
	}
	if (Cmd_readRegisters != NULL) {
		error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
	}
	if (error) goto exit;

	if (length == 8) {
		*value = temp;
	} else {
		temp = REG_GET (temp, position, length);
		*value = temp;
	}


exit :
	return (error);
}


Dword Standard_getFirmwareVersion (
	IN  Demodulator*    demodulator,
	IN  Processor       processor,
	OUT Dword*          version
) {
	Dword error = Error_NO_ERROR;

	Byte writeBuffer[1] = {0,};
	Byte readBuffer[4] = {0,};
	Byte value = 0;

	/** Check chip version */
	error = Standard_readRegister (demodulator, Processor_LINK, chip_version_7_0, &value);
	if (error) goto exit;

	if (value == 0xF8 || User_MAX_PKT_SIZE > 9) {
		/** Old version */
		writeBuffer[0] = 1;
		error = Standard_sendCommand (demodulator, Command_QUERYINFO, processor, 1, writeBuffer, 4, readBuffer);
		if (error) goto exit;
	} else {
		/** New version */
		error = Standard_sendCommand (demodulator, Command_FW_DOWNLOAD_END, Processor_LINK, 0, NULL, 0, NULL);
		if (error == 0x01000009) { /* Boot code*/
			readBuffer[0] = readBuffer[1] = readBuffer[2] = readBuffer[3] = 0;
			error = 0;
		} else if (error == 0x010000FA) { /* Firmware code*/
			if (processor == Processor_LINK)
			{
				error = Standard_readRegisters (demodulator, Processor_LINK, link_version_31_24, 1, readBuffer);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_LINK, link_version_23_16, 1, readBuffer + 1);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_LINK, link_version_15_8, 1, readBuffer + 2);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_LINK, link_version_7_0, 1, readBuffer + 3);
				if (error) goto exit;
			}
			else
			{
				error = Standard_readRegisters (demodulator, Processor_OFDM, ofdm_version_31_24, 1, readBuffer);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_OFDM, ofdm_version_23_16, 1, readBuffer + 1);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_OFDM, ofdm_version_15_8, 1, readBuffer + 2);
				if (error) goto exit;

				error = Standard_readRegisters (demodulator, Processor_OFDM, ofdm_version_7_0, 1, readBuffer + 3);
				if (error) goto exit;
			}
		} else /* error */
			goto exit;
	}

	*version = (Dword) (((Dword) readBuffer[0] << 24) + ((Dword) readBuffer[1] << 16) + ((Dword) readBuffer[2] << 8) + (Dword) readBuffer[3]);

exit :
	return (error);
}


Dword Standard_getPostVitBer (
	IN  Demodulator*    demodulator,
	OUT Dword*          postErrorCount,  /** 24 bits */
	OUT Dword*          postBitCount,    /** 16 bits */
	OUT Word*           abortCount
) {
	Dword error = Error_NO_ERROR;
	Dword errorCount;
	Dword bitCount;
	Byte buffer[7];
	Word abort;

	*postErrorCount = 0;
	*postBitCount  = 0;

	error = Standard_readRegisters (demodulator, Processor_OFDM, rsd_abort_packet_cnt_7_0, r_rsd_packet_unit_15_8 - rsd_abort_packet_cnt_7_0 + 1, buffer);
	if (error) goto exit;

	abort = ((Word) buffer[rsd_abort_packet_cnt_15_8 - rsd_abort_packet_cnt_7_0] << 8) + buffer[rsd_abort_packet_cnt_7_0 - rsd_abort_packet_cnt_7_0];
	errorCount = ((Dword) buffer[rsd_bit_err_cnt_23_16 - rsd_abort_packet_cnt_7_0] << 16) + ((Dword) buffer[rsd_bit_err_cnt_15_8 - rsd_abort_packet_cnt_7_0] << 8) + buffer[rsd_bit_err_cnt_7_0 - rsd_abort_packet_cnt_7_0];
	bitCount = ((Dword) buffer[r_rsd_packet_unit_15_8 - rsd_abort_packet_cnt_7_0] << 8) + buffer[r_rsd_packet_unit_7_0 - rsd_abort_packet_cnt_7_0];
	if (bitCount == 0) {
		/*error = Error_RSD_PKT_CNT_0;*/
		*postErrorCount = 1;
		*postBitCount = 2;
		*abortCount = 1000;
		goto exit;
	}

	*abortCount = abort;
	bitCount = bitCount;
	if (bitCount == 0) {
		*postErrorCount = 1;
		*postBitCount  = 2;
	} else {
		*postErrorCount = errorCount ;
		*postBitCount  = bitCount * 204 * 8;
	}

exit :
	return (error);
}


Dword Standard_getRfAgcGain (
	IN  Demodulator*    demodulator,
	OUT Byte*           rfAgc
) {
	Dword   error = Error_NO_ERROR;

	/** get rf_agc_control */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, r_reg_aagc_rf_gain, reg_aagc_rf_gain_pos, reg_aagc_rf_gain_len, rfAgc);

	return (error);
}


Dword Standard_getIfAgcGain (
	IN  Demodulator*    demodulator,
	OUT Byte*           ifAgc
) {
	Dword error = Error_NO_ERROR;

	/** get if_agc_control */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, r_reg_aagc_if_gain, reg_aagc_if_gain_pos, reg_aagc_if_gain_len, ifAgc);

	return (error);
}


Dword Standard_getSignalQuality (
	IN  Demodulator*    demodulator,
	OUT Byte*           quality
) {
	Dword error = Error_NO_ERROR;

	error = Standard_readRegister (demodulator, Processor_OFDM, signal_quality, quality);
	return (error);
}

Dword Standard_getSignalQualityIndication (
	IN  Demodulator*    demodulator,
	OUT Byte*           quality
) {
	Dword error = Error_NO_ERROR;    		
	Byte locked = 0;	
	Byte buffer[7]= {0};
	Word abort = 0;
    Byte bit_err_1;
    Byte bit_err_2;
    Byte bit_err_3;
    Byte SQ1 = 0;
    
 	error = Standard_readRegisters (demodulator, Processor_OFDM, tpsd_lock, 1, &locked);
 	if (error) goto exit;
 	
	error = Standard_readRegisters (demodulator, Processor_OFDM, rsd_abort_packet_cnt_7_0, r_rsd_packet_unit_15_8 - rsd_abort_packet_cnt_7_0 + 1, buffer);
	if (error) goto exit;

    bit_err_3 = buffer[rsd_bit_err_cnt_23_16 - rsd_abort_packet_cnt_7_0];
    bit_err_2 = buffer[rsd_bit_err_cnt_15_8 - rsd_abort_packet_cnt_7_0];
    bit_err_1 = buffer[rsd_bit_err_cnt_7_0 - rsd_abort_packet_cnt_7_0];


    if ((locked==0) || (bit_err_3 > 2))
        SQ1 = 0;
    else if (bit_err_3 > 0)    // SQ б╫ 42бу8
        SQ1 = 55 - (bit_err_3<<4) - (bit_err_2>>4);
    else if (bit_err_2 > 12)  // SQ б╫ 70бу42 
        SQ1 = 71 - (bit_err_2>>3);
    else if (bit_err_2 > 2)   // SQ б╫ 89бу70
        SQ1 = 95 - (bit_err_2<<1) - (bit_err_1>>7);  
    else 
        SQ1 = 100 - (bit_err_2<<2) - (bit_err_1>>6); //----- 100 бу 89 
                                           
                                   
exit:
    *quality = SQ1; 	
	return (error);
}

Dword Standard_getSignalStrengthIndication (
	IN  Demodulator*    demodulator,
	OUT Byte*           strength
) {
	Dword error = Error_NO_ERROR;
	Byte temp;
	Byte strengthDbm = 0;
	
	error = Standard_readRegister (demodulator, Processor_OFDM, 0x4521, &temp);
	if (error) goto exit;
	
	strengthDbm = (100 - temp);

	if(strengthDbm > 78)
	    *strength = 0;
    else if (strengthDbm < 50)
        *strength = 100;
    else
		*strength = 100 - (((strengthDbm - 50)*7)>>1);
	
exit :
	return (error);
}


Dword Standard_getSignalStrength (
	IN  Demodulator*    demodulator,
	OUT Byte*           strength
) {
	Dword error = Error_NO_ERROR;

	error = Standard_readRegister (demodulator, Processor_OFDM, var_signal_strength, strength);
	return (error);
}


Dword Standard_getSignalStrengthDbm (
	IN  Demodulator*    demodulator,
	IN  Long            rfpullUpVolt_X10,     /** RF pull up voltage multiplied by 10 */
	IN  Long            ifpullUpVolt_X10,     /** IF pull up voltage multiplied by 10 */
	OUT Long*           strengthDbm           /** DBm                                 */
)
{
	Dword error = Error_NO_ERROR;
	Byte temp;

	if ((rfpullUpVolt_X10 == 0) || (ifpullUpVolt_X10 == 0)) {
		error = Error_INV_PULLUP_VOLT;
		goto exit;
	}

	error = Standard_readRegister (demodulator, Processor_OFDM, 0x4521, &temp);
	if (error) goto exit;

	*strengthDbm = (Long) (temp - 100);

exit :
	return (error);
}


Dword Standard_initialize (
	IN  Demodulator*    demodulator,
	IN  StreamType      streamType
) {
	Dword error = Error_NO_ERROR;

	Dword crystal = 0;
	Dword adc = 0;
	Dword fcw = 0;
	Byte buffer[4];
	Dword version = 0;
	Word* tunerScriptSets = NULL;
	ValueSet* tunerScripts = NULL;
	Byte i = 0;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	demod->fcw = 0x00000000;
	demod->frequency = 642000;
	demod->initialized = False;



	Standard_clockTable[0].crystalFrequency = 12000;
	Standard_clockTable[0].adcFrequency = 20250000;
	Standard_clockTable[1].crystalFrequency = 20480;
	Standard_clockTable[1].adcFrequency = 20480000;
	error = Standard_getFirmwareVersion (demodulator, Processor_LINK, &version);
	if (error) goto exit;
	if (version != 0) {
		demod->booted = True;
	} else {
		demod->booted = False;
	}


	demod->firmwareCodes = Firmware_codes;
	demod->firmwareSegments = Firmware_segments;
	demod->firmwarePartitions = Firmware_partitions;
	demod->scriptSets = Firmware_scriptSets;
	demod->scripts = Firmware_scripts;

	/** Set up by default tunerDescription */ /** releaseExternalRemove */
	tunerScriptSets = tunerDescription.tunerScriptSetsTable;
	tunerScripts = tunerDescription.tunerScriptTable;


	error = Standard_readRegisterBits (demodulator, Processor_LINK, r_io_mux_pwron_clk_strap, io_mux_pwron_clk_strap_pos, io_mux_pwron_clk_strap_len, &i);
	if (error) goto exit;

	demod->crystalFrequency = Standard_clockTable[i].crystalFrequency;
	demod->adcFrequency = Standard_clockTable[i].adcFrequency;

	/** Write secondary I2C address to device */
	/** Enable or disable clock out for 2nd chip power saving */
	error = Standard_writeRegister (demodulator, Processor_LINK, second_i2c_address, 0x00);
	if (error) goto exit;

    if(demod->Clkout_en) {
	    error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_clkoen, 1);
    } else {
	    error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_clkoen, 0);
    }
	if (error) goto exit;

	/** Detect the HostA or HostB */
	error = Standard_readRegisterBits (demodulator, Processor_LINK, r_io_mux_pwron_hosta, io_mux_pwron_hosta_pos, io_mux_pwron_hosta_len, &demod->hostInterface);
	if (error) goto exit;

	/** Load firmware */
	if (demod->firmwareCodes != NULL) {
		if (demod->booted == False) {
			error = Standard_loadFirmware (demodulator, demod->firmwareCodes, demod->firmwareSegments, demod->firmwarePartitions);
			if (error) goto exit;
			demod->booted = True;
		}
	}

	/** Set I2C master clock 100k in order to support tuner I2C. */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_one_cycle_counter_tuner, 0x1a);
	if (error) goto exit;

	/** Open tuner */
	/** Set 0xD827 to 0 as open drain for tuner i2c */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padodpu, 0);
	if (error) goto exit;

	/** Set 0xD829 to 0 as push pull for tuner AGC */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_agc_od, 0);
	if (error) goto exit;

	if (tunerDescription.openTunerFunc != NULL) {
		error = tunerDescription.openTunerFunc (demodulator);
		if (error) goto exit;
	}
	/** Tell firmware the type of tuner. */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_link_ofsm_dummy_15_8, (Byte) tunerDescription.tunerId);
	if (error) goto exit;

	/** Initialize OFDM */
	if (demod->booted == True) {
		/** Set read-update bit to 1 for constellation */
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_feq_read_update, reg_feq_read_update_pos, reg_feq_read_update_len, 1);
		if (error) goto exit;

		/** Enable FEC Monitor */
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_fec_vtb_rsd_mon_en, fec_vtb_rsd_mon_en_pos, fec_vtb_rsd_mon_en_len, 1);
		if (error) goto exit;

		/** Compute ADC and load them to device */
		error = Standard_computeCrystal (demodulator, (Long) demod->crystalFrequency * 1000, &crystal);
		if (error) goto exit;

		buffer[0] = (Byte) (crystal & 0x000000FF);
		buffer[1] = (Byte) ((crystal & 0x0000FF00) >> 8);
		buffer[2] = (Byte) ((crystal & 0x00FF0000) >> 16);
		buffer[3] = (Byte) ((crystal & 0xFF000000) >> 24);
		error = Standard_writeRegisters (demodulator, Processor_OFDM, crystal_clk_7_0, 4, buffer);
		if (error) goto exit;

		/** Compute ADC and load them to device */
		error = Standard_computeAdc (demodulator, (Long) demod->adcFrequency, &adc);
		if (error) goto exit;

		buffer[0] = (Byte) (adc & 0x000000FF);
		buffer[1] = (Byte) ((adc & 0x0000FF00) >> 8);
		buffer[2] = (Byte) ((adc & 0x00FF0000) >> 16);
		error = Standard_writeRegisters (demodulator, Processor_OFDM, p_reg_f_adc_7_0, 3, buffer);
		if (error) goto exit;

		/** Compute FCW and load them to device */
		error = Standard_computeFcw (demodulator, (Long) demod->adcFrequency, (Long) tunerDescription.ifFrequency, tunerDescription.inversion, &fcw);
		if (error) goto exit;
		demod->fcw = fcw;

		buffer[0] = (Byte) (fcw & 0x000000FF);
		buffer[1] = (Byte) ((fcw & 0x0000FF00) >> 8);
		buffer[2] = (Byte) ((fcw & 0x007F0000) >> 16);
		error = Standard_writeRegisters (demodulator, Processor_OFDM, bfs_fcw_7_0, bfs_fcw_22_16 - bfs_fcw_7_0 + 1, buffer);    
		if (error) goto exit;
	}

	/** Load script */
	if (demod->scripts != NULL) {
		error = Standard_loadScript (demodulator, demod->scriptSets, demod->scripts, tunerScriptSets, tunerScripts);
		if (error) goto exit;
	}

	/** Set the desired stream type */
	error = Standard_setStreamType (demodulator, streamType);
	if (error) goto exit;

	/** Set the desired architecture type */
	error = Standard_setArchitecture (demodulator);
	if (error) goto exit;

	/** Set H/W MPEG2 locked detection **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_lock3_out, 1);
	if (error) goto exit;

	/** Set registers for driving power 0xD830 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr2, 0);
	if (error) goto exit;

	/** Set registers for driving power 0xD831 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr4, 0);
	if (error) goto exit;

	/** Set registers for driving power 0xD832 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr8, 0);
	if (error) goto exit;
	
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdrsr, 1);
	if (error) goto exit;
	
    /** enhance the performance while using DIP crystals **/
	error = Standard_writeRegister (demodulator, Processor_OFDM, 0xEC57, 0);
	if (error) goto exit;
	error = Standard_writeRegister (demodulator, Processor_OFDM, 0xEC58, 0);
	if (error) goto exit;
	
	/** Set ADC frequency multiplier **/
	error = Standard_setMultiplier(demodulator,Multiplier_2X); 
	if (error) goto exit;
	
	demod->initialized = True;

exit:

	return (error);
}


Dword Standard_finalize (
	IN  Demodulator*    demodulator
) {
	Dword error = Error_NO_ERROR;

	if (tunerDescription.closeTunerFunc != NULL) {
		error = tunerDescription.closeTunerFunc (demodulator);
	}

	return (error);
}


Dword Standard_isTpsLocked (
	IN  Demodulator*    demodulator,
	OUT Bool*           locked
) {
	Dword error = Error_NO_ERROR;

	Byte temp;

	*locked = False;

	error = Standard_readRegisterBits (demodulator, Processor_OFDM, p_fd_tpsd_lock, fd_tpsd_lock_pos, fd_tpsd_lock_len, &temp);
	if (error) goto exit;
	if (temp) *locked = True;

exit :

	return (error);
}


Dword Standard_isMpeg2Locked (
	IN  Demodulator*    demodulator,
	OUT Bool*           locked
) {
	Dword error = Error_NO_ERROR;

	Byte temp;

	*locked = False;

	error = Standard_readRegisterBits (demodulator, Processor_OFDM, r_mp2if_sync_byte_locked, mp2if_sync_byte_locked_pos, mp2if_sync_byte_locked_len, &temp);
	if (error) goto exit;
	if (temp) *locked = True;

exit :

	return (error);
}

Dword Standard_isLocked (
	IN  Demodulator*    demodulator,
	OUT Bool*           locked
) {
	Dword error = Error_NO_ERROR;

	Word emptyLoop = 0;
	Word tpsLoop = 0;
	Word mpeg2Loop = 0;
	Byte channels;
	Byte emptyChannel = 1;
	Byte tpsLocked = 0;
    
	DefaultDemodulator* demod;
	Bool retry = True;
	

	demod = (DefaultDemodulator*) demodulator;
  
check_lock:
              
	*locked = False;

	demod->statistic.signalPresented = False;
	demod->statistic.signalLocked = False;
	demod->statistic.signalQuality = 0;
	demod->statistic.signalStrength = 0;

	channels = 2;

	while (emptyLoop < 40) {
		error = Standard_readRegister (demodulator, Processor_OFDM, empty_channel_status, &channels);
		if (error) goto exit;
		if (channels == 1) {
			emptyChannel = 0;
			break;
		}
		if (channels == 2) {
			emptyChannel = 1;			
			//goto exit; 	
		}
		User_delay (demodulator, /*25*/50);
		emptyLoop++;
	}
    
	if (emptyChannel == 1)  goto exit;

	while (tpsLoop < 100) {
		
		error = Standard_isTpsLocked (demodulator, &demod->statistic.signalPresented);
		if (error) goto exit;
		if (demod->statistic.signalPresented == True) {
			tpsLocked = 1;
			break;
		}

		if (tpsLocked == 1) break;

		User_delay (demodulator, 25);
		tpsLoop++;
	}

	if (tpsLocked == 0) goto exit;
        
	while (mpeg2Loop < 100) { //timeout = loop* 25ms
		error = Standard_isMpeg2Locked (demodulator, &demod->statistic.signalLocked);
		if (error) goto exit;
		if (demod->statistic.signalLocked == True) {
			demod->statistic.signalQuality = 80;
			demod->statistic.signalStrength = 80;
			*locked = True;
			break;
		}
		User_delay (demodulator, 25);
		mpeg2Loop++;
	}

      
	demod->statistic.signalQuality = 0;
	demod->statistic.signalStrength = 20;

exit:
#if 0
    if(*locked == False && retry == True){
            
        retry=False;
        mpeg2Loop = 0;
	    tpsLoop = 0;  
        emptyLoop = 0;
		emptyChannel = 1;
    	tpsLocked = 0;
        // Clear empty_channel_status lock flag 
    	error = Standard_writeRegister (demodulator, Processor_OFDM, empty_channel_status, 0x00);
    	if (error) goto exit;  
         
        //Trigger ofsm /
    	error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0);
    	if (error) goto exit;
        goto check_lock;     
    }
#endif    
   
	return (error);
}

Dword Standard_getChannelModulation (
	IN  Demodulator*            demodulator,
	OUT ChannelModulation*      channelModulation
) {
	Dword error = Error_NO_ERROR;

	Byte temp;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Get constellation type */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_const, reg_tpsd_const_pos, reg_tpsd_const_len, &temp);
	if (error) goto exit;
	channelModulation->constellation = (Constellation) temp;

	/** Get TPS hierachy and alpha value */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_hier, reg_tpsd_hier_pos, reg_tpsd_hier_len, &temp);
	if (error) goto exit;
	channelModulation->hierarchy = (Hierarchy)temp;

	/** Get high/low priority */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_dec_pri, reg_dec_pri_pos, reg_dec_pri_len, &temp);
	if (error) goto exit;
	if (temp)
		channelModulation->priority = Priority_HIGH;
	else
		channelModulation->priority = Priority_LOW;

	/** Get high code rate */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_hpcr, reg_tpsd_hpcr_pos, reg_tpsd_hpcr_len, &temp);
	if (error) goto exit;
	channelModulation->highCodeRate = (CodeRate) temp;

	/** Get low code rate */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_lpcr, reg_tpsd_lpcr_pos, reg_tpsd_lpcr_len, &temp);
	if (error) goto exit;
	channelModulation->lowCodeRate  = (CodeRate) temp;

	/** Get guard interval */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_gi, reg_tpsd_gi_pos, reg_tpsd_gi_len, &temp);
	if (error) goto exit;
	channelModulation->interval = (Interval) temp;

	/** Get FFT mode */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_tpsd_txmod, reg_tpsd_txmod_pos, reg_tpsd_txmod_len, &temp);
	if (error) goto exit;
	channelModulation->transmissionMode = (TransmissionModes) temp;

	/** Get bandwidth */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, &temp);
	if (error) goto exit;
	channelModulation->bandwidth = (Bandwidth) temp;

	/** Get frequency */
	channelModulation->frequency = demod->frequency;

exit :

	return (error);
}



Dword Standard_acquireChannel (
	IN  Demodulator*    demodulator,
	IN  Word            bandwidth,
	IN  Dword           frequency
) {
	Dword error = Error_NO_ERROR;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	error = Standard_selectBandwidth (demodulator, bandwidth, demod->adcFrequency);
       if (error) goto exit;
	demod->bandwidth = bandwidth;

	/** Set frequency */
	error = Standard_setFrequency (demodulator, frequency);
      if (error) goto exit;

exit :
                   
	return (error);
}


Dword Standard_setStreamType (
	IN  Demodulator*    demodulator,
	IN  StreamType      streamType
) {
	Dword error = Error_NO_ERROR;

	Dword warning = Error_NO_ERROR;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Enable DVB-T mode */
	error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_dvbt_en, reg_dvbt_en_pos, reg_dvbt_en_len, 1);
	if (error) goto exit;

	/** Enter sub mode */
	switch (streamType) {
		case StreamType_DVBT_PARALLEL :
			error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_mpeg_ser_mode, mp2if_mpeg_ser_mode_pos, mp2if_mpeg_ser_mode_len, 0);
			if (error) goto exit;
			error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_mpeg_par_mode, mp2if_mpeg_par_mode_pos, mp2if_mpeg_par_mode_len, 1);
			if (error) goto exit;

			/** HostB interface is enabled */
			error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_top_hostb_mpeg_ser_mode, reg_top_hostb_mpeg_ser_mode_pos, reg_top_hostb_mpeg_ser_mode_len, 0);
			if (error) goto exit;
			error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_top_hostb_mpeg_par_mode, reg_top_hostb_mpeg_par_mode_pos, reg_top_hostb_mpeg_par_mode_len, 1);
			if (error) goto exit;
			break;
		case StreamType_DVBT_SERIAL :
			error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_mpeg_ser_mode, mp2if_mpeg_ser_mode_pos, mp2if_mpeg_ser_mode_len, 1);
			if (error) goto exit;

			/** HostB interface is enabled */
			error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_top_hostb_mpeg_ser_mode, reg_top_hostb_mpeg_ser_mode_pos, reg_top_hostb_mpeg_ser_mode_len, 1);
			if (error) goto exit;
			break;
		}
		error = User_mpegConfig (demodulator);

		demod->streamType = streamType;

exit :
	return (error);
}


Dword Standard_setArchitecture (
	IN  Demodulator*    demodulator
) {
	Dword error = Error_NO_ERROR;

	Byte standAlone;
	Byte upperChip;
	Byte upperHost;
	Byte lowerChip;
	Byte lowerHost;
	Byte dcaEnable;
	Byte phaseLatch;
	Byte fpgaLatch;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	upperChip = 0;
	upperHost = 0;
	lowerChip = 0;
	lowerHost = 0;
	phaseLatch = 0;
	fpgaLatch = 0;
	standAlone = 1;
	dcaEnable = 0;

	/** Pictor & Orion & Omega */
	/** Set dca_upper_chip */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_upper_chip, reg_dca_upper_chip_pos, reg_dca_upper_chip_len, upperChip);
	if (error) goto exit;
		error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_top_hostb_dca_upper, reg_top_hostb_dca_upper_pos, reg_top_hostb_dca_upper_len, upperHost);
	if (error) goto exit;

	/** Set dca_lower_chip */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_lower_chip, reg_dca_lower_chip_pos, reg_dca_lower_chip_len, lowerChip);
	if (error) goto exit;
		error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_top_hostb_dca_lower, reg_top_hostb_dca_lower_pos, reg_top_hostb_dca_lower_len, lowerHost);
	if (error) goto exit;

	/** Set phase latch */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_platch, reg_dca_platch_pos, reg_dca_platch_len, phaseLatch);
	if (error) goto exit;

	/** Set fpga latch */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_fpga_latch, reg_dca_fpga_latch_pos, reg_dca_fpga_latch_len, fpgaLatch);
	if (error) goto exit;

	/** Set stand alone */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_stand_alone, reg_dca_stand_alone_pos, reg_dca_stand_alone_len, standAlone);
	if (error) goto exit;

	/** Set DCA enable */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_dca_en, reg_dca_en_pos, reg_dca_en_len, dcaEnable);
	if (error) goto exit;

	if (demod->initialized == True) {
		error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0);
		if (error) goto exit;
	}

exit:
	return (error);
}


Dword Standard_setViterbiRange (
	IN  Demodulator*    demodulator,
	IN  Byte            superFrameCount,
	IN  Word            packetUnit
) {
	Dword error = Error_NO_ERROR;

	Byte temp0;
	Byte temp1;

	/** Set super frame count */
	error = Standard_writeRegister (demodulator, Processor_OFDM, qnt_vbc_sframe_num, superFrameCount);
	if (error) goto exit;

	/** Set packet unit. */
	temp0 = (Byte) packetUnit;
	temp1 = (Byte) (packetUnit >> 8);
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_15_8, 1, &temp1);
	if (error) goto exit;

exit:

	return (error);
}


Dword Standard_getViterbiRange (
	IN  Demodulator*    demodulator,
	IN  Byte*           frameCount,
	IN  Word*           packetUnit
) {
	Dword error = Error_NO_ERROR;

	Byte temp0;
	Byte temp1;

	/** Get super frame count */
	error = Standard_readRegister (demodulator, Processor_OFDM, qnt_vbc_sframe_num, frameCount);
	if (error) goto exit;

	/** Get packet unit. */
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_15_8, 1, &temp1);
	if (error) goto exit;
	*packetUnit = (Word) (temp1 << 8) + (Word) temp0;

exit:
	return (error);
}


Dword Standard_getStatistic (
	IN  Demodulator*    demodulator,
	OUT Statistic*      statistic
) {
	Dword error = Error_NO_ERROR;

	DefaultDemodulator* demod;
	Byte quality;
	Byte strength;
	Byte buffer[2];

	demod = (DefaultDemodulator*) demodulator;

	/** Get statistic by stream type */
	error = Standard_readRegisters (demodulator, Processor_OFDM, tpsd_lock, mpeg_lock - tpsd_lock + 1, buffer);
	if (error) goto exit;

	if (buffer[tpsd_lock - tpsd_lock])
		demod->statistic.signalPresented = True;
	else
		demod->statistic.signalPresented = False;

	if (buffer[mpeg_lock - tpsd_lock])
		demod->statistic.signalLocked = True;
	else
		demod->statistic.signalLocked = False;

	error = Standard_getSignalQuality (demodulator, &quality);
	if (error) goto exit;

	demod->statistic.signalQuality = quality;

	error = Standard_getSignalStrength (demodulator, &strength);
	if (error) goto exit;

	demod->statistic.signalStrength = strength;

	*statistic = demod->statistic;

exit :

	return (error);
}


Dword Standard_controlPowerSaving (
	IN  Demodulator*    demodulator,
	IN  Byte            control
) {
	Dword error = Error_NO_ERROR;
	DefaultDemodulator* demod;
    Byte temp;
       Byte j = 0;
	demod = (DefaultDemodulator*) demodulator;

	if (control) {
		/** Power up case */
        error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_p_if_en,  1);
	    if (error) goto exit;
	    error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_afe_mem0, 3, 1, 0);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_dyn0_clk, 0);
		if (error) goto exit;
	} else {
		/** Power down case */
		//--suspend ofdm
        
    	error = Standard_writeRegister (demodulator, Processor_OFDM, suspend_flag, 1);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0);
		if (error) goto exit;

		for (j = 0; j < 150; j++) {
			error = Standard_readRegister (demodulator, Processor_OFDM, suspend_flag, &temp);
			if (error) goto exit;
			if (!temp) break;
			User_delay (demodulator, /*10*/20);
		}
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_afe_mem0, 3, 1, 1);
		if (error) goto exit;
		//0xec40
		Byte value[15] = {0};
        error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_p_if_en,  0);
	    if (error) goto exit;
	
        //0xec02 ~ 0xec10
        value[1] = 0x0C;
        error = Standard_writeRegisters (demodulator, Processor_OFDM, p_reg_pd_a, 15, value);
	    if (error) goto exit;
	    value[1] = 0;

        //0xec12~0xec15
        error = Standard_writeRegisters (demodulator, Processor_OFDM, p_reg_lna_g, 4, value);
	    if (error) goto exit;

        //oxec17~0xec1f
        error = Standard_writeRegisters (demodulator, Processor_OFDM, p_reg_pgc, 9, value);
	    if (error) goto exit;

        //0xec22~0xec2b
        error = Standard_writeRegisters (demodulator, Processor_OFDM, p_reg_clk_del_sel, 10, value);
	    if (error) goto exit;

        //0xec20
        error = Standard_writeRegister (demodulator, Processor_OFDM, 0xec20,  0);
	    if (error) goto exit;

        //0xec3f
        error = Standard_writeRegister (demodulator, Processor_OFDM, 0xec3F,  1);
	    if (error) goto exit;
	}

exit :

	return (error);
}


Dword Standard_enableControlPowerSaving (
	IN  Demodulator*    demodulator,
	IN  Byte            enable
) {
	Dword error = Error_NO_ERROR;

    if(enable){
        error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_pwrdw_hwen,  1);
	    if (error) goto exit;

        error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_pwrdw,  1);
	    if (error) goto exit;
    }
    else{
        error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_pwrdw_hwen,  0);
	    if (error) goto exit;

        error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_pwrdw,  0);
	    if (error) goto exit;
    }

exit :
	return (error);
}


Dword Standard_controlPidFilter (
	IN  Demodulator*    demodulator,
	IN  Byte            control
) {
	Dword error = Error_NO_ERROR;

	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_pid_en, mp2if_pid_en_pos, mp2if_pid_en_len, control);

	return (error);
}


Dword Standard_resetPidFilter (
	IN  Demodulator*    demodulator
) {
	Dword error = Error_NO_ERROR;
	DefaultDemodulator* demod = (DefaultDemodulator*)demodulator; 

	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_pid_rst, mp2if_pid_rst_pos, mp2if_pid_rst_len, 1);
	if (error) goto exit;

	/** Reset Pid Counter of HW */
	demod->pidCounter = 0;

exit :
	return (error);
}


Dword Standard_addPidToFilter (
	IN  Demodulator*    demodulator,
	IN  Byte            index,
	IN  Word            value
) {
	Dword error = Error_NO_ERROR;

	Byte writeBuffer[2];

	/** Enable pid filter */
	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_pid_en, mp2if_pid_en_pos, mp2if_pid_en_len, 1);
	if (error) goto exit;

	writeBuffer[0] = (Byte) value;
	writeBuffer[1] = (Byte) (value >> 8);

	error = Standard_writeRegisters (demodulator, Processor_OFDM, p_mp2if_pid_dat_l, 2, writeBuffer);
	if (error) goto exit;

	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_mp2if_pid_index_en, mp2if_pid_index_en_pos, mp2if_pid_index_en_len, 1);
	if (error) goto exit;

	error = Standard_writeRegister (demodulator, Processor_OFDM, p_mp2if_pid_index, index);
	if (error) goto exit;

exit :

	return (error);
}

Dword Standard_getSNR (
    IN  Demodulator*    demodulator,
    OUT Byte*           snr
) {
    
    Dword error = Error_NO_ERROR;
    ChannelModulation    channelModulation;
    Dword   snr_value;

    error= Standard_getChannelModulation(demodulator, &channelModulation);
    if (error) goto exit;
    
    error = Standard_getSNRValue(demodulator, &snr_value);
    if (error) goto exit;
    
    if (channelModulation.transmissionMode == TransmissionMode_2K)
        snr_value = snr_value*4;
    else if (channelModulation.transmissionMode == TransmissionMode_4K)
        snr_value = snr_value*2;
    else 
        snr_value = snr_value*1;

    if(channelModulation.constellation == 0) //Constellation_QPSK 
    {
    
        if(snr_value < 0xB4771)         *snr = 0;
        else if(snr_value < 0xC1AED)    *snr = 1;
        else if(snr_value < 0xD0D27)    *snr = 2;
        else if(snr_value < 0xE4D19)    *snr = 3;
        else if(snr_value < 0xE5DA8)    *snr = 4;
        else if(snr_value < 0x107097)   *snr = 5;
        else if(snr_value < 0x116975)   *snr = 6;
        else if(snr_value < 0x1252D9)   *snr = 7;
        else if(snr_value < 0x131FA4)   *snr = 8;
        else if(snr_value < 0x13D5E1)   *snr = 9;
        else if(snr_value < 0x148E53)   *snr = 10;
        else if(snr_value < 0x15358B)   *snr = 11;
        else if(snr_value < 0x15DD29)   *snr = 12;
        else if(snr_value < 0x168112)   *snr = 13;
        else if(snr_value < 0x170B61)   *snr = 14;
        else if(snr_value < 0x17A532)   *snr = 15;
        else if(snr_value < 0x180F94)   *snr = 16;
        else if(snr_value < 0x186ED2)   *snr = 17;
        else if(snr_value < 0x18B271)   *snr = 18;
        else if(snr_value < 0x18E118)   *snr = 19;
        else if(snr_value < 0x18FF4B)   *snr = 20;
        else if(snr_value < 0x190AF1)   *snr = 21;
        else if(snr_value < 0x191451)   *snr = 22;
        else   *snr = 23;
    }
    else if ( channelModulation.constellation == 1) //Constellation_16QAM
    {
        if(snr_value < 0x4F0D5)    *snr = 0;
        else if(snr_value < 0x5387A)   *snr = 1;
        else if(snr_value < 0x573A4)   *snr = 2;
        else if(snr_value < 0x5A99E)   *snr = 3;
        else if(snr_value < 0x5CC80)   *snr = 4;
        else if(snr_value < 0x5EB62)   *snr = 5;
        else if(snr_value < 0x5FECF)   *snr = 6;
        else if(snr_value < 0x60B80)   *snr = 7;
        else if(snr_value < 0x62501)   *snr = 8;
        else if(snr_value < 0x64865)   *snr = 9;
        else if(snr_value < 0x69604)   *snr = 10;
        else if(snr_value < 0x6F356)   *snr = 11;
        else if(snr_value < 0x7706A)   *snr = 12;
        else if(snr_value < 0x804D3)   *snr = 13;
        else if(snr_value < 0x89D1A)   *snr = 14;
        else if(snr_value < 0x93E3D)   *snr = 15;
        else if(snr_value < 0x9E35D)   *snr = 16;
        else if(snr_value < 0xA7C3C)   *snr = 17;
        else if(snr_value < 0xAFAF8)   *snr = 18;
        else if(snr_value < 0xB719D)   *snr = 19;
        else if(snr_value < 0xBDA6A)   *snr = 20;
        else if(snr_value < 0xC0C75)   *snr = 21;
        else if(snr_value < 0xC3F7D)   *snr = 22;
        else if(snr_value < 0xC5E62)   *snr = 23;
        else if(snr_value < 0xC6C31)   *snr = 24;
        else if(snr_value < 0xC7925)   *snr = 25;
        else    *snr = 26;
    }
    else if ( channelModulation.constellation == 2) //Constellation_64QAM
    {
        if(snr_value < 0x256D0)    *snr = 0;
        else if(snr_value < 0x27A65)   *snr = 1;
        else if(snr_value < 0x29873)   *snr = 2;
        else if(snr_value < 0x2B7FE)   *snr = 3;
        else if(snr_value < 0x2CF1E)   *snr = 4;
        else if(snr_value < 0x2E234)   *snr = 5;
        else if(snr_value < 0x2F409)   *snr = 6;
        else if(snr_value < 0x30046)   *snr = 7;
        else if(snr_value < 0x30844)   *snr = 8;
        else if(snr_value < 0x30A02)   *snr = 9;
        else if(snr_value < 0x30CDE)   *snr = 10;
        else if(snr_value < 0x31031)   *snr = 11;
        else if(snr_value < 0x3144C)   *snr = 12;
        else if(snr_value < 0x315DD)   *snr = 13;
        else if(snr_value < 0x31920)   *snr = 14;
        else if(snr_value < 0x322D0)   *snr = 15;
        else if(snr_value < 0x339FC)   *snr = 16;
        else if(snr_value < 0x364A1)   *snr = 17;
        else if(snr_value < 0x38BCC)   *snr = 18;
        else if(snr_value < 0x3C7D3)   *snr = 19;
        else if(snr_value < 0x408CC)   *snr = 20;
        else if(snr_value < 0x43BED)   *snr = 21;
        else if(snr_value < 0x48061)   *snr = 22;
        else if(snr_value < 0x4BE95)   *snr = 23;
        else if(snr_value < 0x4FA7D)   *snr = 24;
        else if(snr_value < 0x52405)   *snr = 25;
        else if(snr_value < 0x5570D)   *snr = 26;
        else if(snr_value < 0x59FEB)   *snr = 27;
        else if(snr_value < 0x5BF38)   *snr = 28;
        else    *snr = 29;
    } else 
        goto exit;    

exit:    
    return error;
    
}

Dword Standard_getSNRValue(
      IN  Demodulator*    demodulator,
      OUT Dword*    snr_value
) {

    Dword error = Error_NO_ERROR;
   	Byte superFrame_num=0;
	Byte snr_reg[3];

	error = Standard_readRegisters(demodulator, Processor_OFDM, 0x2c,3,snr_reg);
    if(error) goto exit;
  
    *snr_value = (snr_reg[2]<<16)+(snr_reg[1]<<8)+snr_reg[0];
  
	error = Standard_readRegister(demodulator,Processor_OFDM, 0xF78b, (unsigned char *)&superFrame_num); //gets superFrame num
    if(error) goto exit;
	if(superFrame_num)
          *snr_value/=superFrame_num;  

exit:   
    return(error);

}

Dword Standard_setMultiplier (
    IN  Demodulator*    demodulator,
    IN  Multiplier      multiplier
) {
    Dword error = Error_NO_ERROR;
    Byte oldAdcMultiplier = 0;
    Byte newAdcMultiplier = 0;
    Byte buffer[3];
    Long controlWord;
    Byte i;
    DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

    /** Read ADC multiplier factor to firmware. */
    error = Standard_readRegister (demodulator,  Processor_OFDM, adcx2, &oldAdcMultiplier);
    if (error) goto exit;

    if (multiplier == Multiplier_1X)
        newAdcMultiplier = 0;
    else
        newAdcMultiplier = 1;

    if (oldAdcMultiplier == newAdcMultiplier) goto exit;

   
        /** Write ADC multiplier factor to firmware. */
        error = Standard_writeRegister (demodulator,  Processor_OFDM, adcx2, newAdcMultiplier);
        if (error) goto exit;
    

    /** Compute FCW and load them to device */
    if (demod->fcw >= 0x00400000) {
        controlWord = demod->fcw - 0x00800000;
    } else {
        controlWord = demod->fcw;
    }
    if (newAdcMultiplier == 1) {
        controlWord /= 2;
    } else {
        controlWord *= 2;
    }
    demod->fcw = 0x7FFFFF & controlWord;

    buffer[0] = (Byte) (demod->fcw & 0x000000FF);
    buffer[1] = (Byte) ((demod->fcw & 0x0000FF00) >> 8);
    buffer[2] = (Byte) ((demod->fcw & 0x007F0000) >> 16);
    
        error = Standard_writeRegisters (demodulator,  Processor_OFDM, bfs_fcw_7_0, bfs_fcw_22_16 - bfs_fcw_7_0 + 1, buffer);
    

exit :
    return (error);
}
