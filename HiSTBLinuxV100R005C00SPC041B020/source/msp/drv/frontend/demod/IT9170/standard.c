/********************************************************************************************/
/**                                                                                         */
/**       These functions in this file are used by API.                                     */
/**       Please not modify the content.                                                    */
/**                                                                                         */
/********************************************************************************************/
#define __SAMBADEMOD_H__
#include "standard.h"
#include "cmd.h"
#include "user.h"
#include "tuner.h"

#include "firmware.h"

#define NULL 0

#define Standard_MAX_BIT                8
#define Standard_MAX_CLOCK              12
#define Standard_MAX_BAND               3


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


static unsigned char SDRAM_Clock_Table[21][7] = {
    {0x65, 0x32, 0x80, 0x04, 0x4a, 0x48, 0x40},	//set  0 : 80MHz
	{0x65, 0x32, 0x78, 0x04, 0x4a, 0x48, 0x40},	//set  1 : 79MHz
	{0x65, 0x32, 0x70, 0x04, 0x4a, 0x48, 0x40},	//set  2 : 78MHz
	{0x65, 0x32, 0x68, 0x04, 0x4a, 0x48, 0x40},	//set  3 : 77MHz
	{0x65, 0x32, 0x60, 0x04, 0x4a, 0x48, 0x40},	//set  4 : 76MHz
	{0x65, 0x32, 0x58, 0x04, 0x4a, 0x48, 0x40},	//set  5 : 75MHz
	{0x65, 0x32, 0x50, 0x04, 0x4a, 0x48, 0x40},	//set  6 : 74MHz
	{0x65, 0x32, 0x48, 0x04, 0x4a, 0x48, 0x40},	//set  7 : 73MHz
	{0x65, 0x32, 0x40, 0x04, 0x4a, 0x48, 0x40},	//set  8 : 72MHz
	{0x65, 0x32, 0x38, 0x04, 0x4a, 0x48, 0x40},	//set  9 : 71MHz
    {0x65, 0x32, 0x30, 0x04, 0x4a, 0x48, 0x40},	//set 10 : 70MHz
    {0x65, 0x32, 0x28, 0x04, 0x4a, 0x48, 0x40},	//set 11 : 69MHz
    {0x65, 0x32, 0x20, 0x04, 0x4a, 0x48, 0x40},	//set 12 : 68MHz
    {0x65, 0x32, 0x18, 0x04, 0x4a, 0x48, 0x40},	//set 13 : 67MHz
	{0x65, 0x32, 0x10, 0x04, 0x4a, 0x48, 0x40},	//set 14 : 66MHz
	{0x65, 0x32, 0x08, 0x04, 0x4a, 0x48, 0x40},	//set 15 : 65MHz
    {0x65, 0x32, 0x00, 0x04, 0x4a, 0x48, 0x40},	//set 16 : 64MHz
    {0x65, 0x31, 0xf8, 0x04, 0x4a, 0x48, 0x40},	//set 17 : 63MHz
    {0x65, 0x31, 0xf0, 0x04, 0x4a, 0x48, 0x40},	//set 18 : 62MHz
    {0x65, 0x31, 0xe8, 0x04, 0x4a, 0x48, 0x40},	//set 19 : 61MHz
    {0x65, 0x31, 0xe0, 0x04, 0x4a, 0x48, 0x40},	//set 20 : 60MHz
};

static unsigned char Channel_RegisterSet_Map[70] = {
    0, 0, 0, 0, 0, 0, 0, 6, 3, 0,	        //ch0 ~ ch9
    5, 6, 1, 0, 16, 14, 14, 13, 13, 1,	    //ch10 ~ ch19
    0, 19, 18, 9, 18, 15, 14, 16, 13, 20,   //ch20 ~ ch29
	12, 1, 0, 18, 17, 16, 15, 14, 14, 14,   //ch30 ~ ch39
	13, 20, 6, 19, 1, 16, 15, 15, 14, 20,   //ch40 ~ ch49
	7, 20, 12, 18, 11, 16, 0, 1, 1, 20,     //ch50 ~ ch59
	20, 20, 17, 19, 17, 19, 16, 18, 15, 15, //ch60 ~ ch69
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

	//if (Cmd_sendCommand != NULL) {
		error = Cmd_sendCommand (demodulator, command, processor, writeBufferLength, writeBuffer, readBufferLength, readBuffer);
	//}

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
	
	/** Program SFOE, only for ISDB-T*/
	Byte* ADC_buf_ptr;
	// ASIC
	Byte ADC_20250000[24] = {	0x5E, 0x45, 0x5B, 0x7F, 0x56, 0x6A, 0x4D, 0xC5,
								0x2C, 0xF9, 0x2B, 0xA7, 0x29, 0x3A, 0x25, 0x1B,
								0x17, 0x01, 0x16, 0x53, 0x15, 0x16, 0x12, 0xFA	};
	// FPGA
	Byte ADC_20480000[24] = {	0x5F, 0x57, 0x5C, 0x89, 0x57, 0x65, 0x4E, 0xA8,
								0x2D, 0x7C, 0x2C, 0x26, 0x29, 0xB2, 0x25, 0x86,
								0x17, 0x44, 0x16, 0x94, 0x15, 0x53, 0x13, 0x31	};


	bw = 0;
	

	error = Standard_writeRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, bw);
	if (error) goto exit;

	if (adcFrequency == 20250000) 
		ADC_buf_ptr = ADC_20250000; 
	else  
		ADC_buf_ptr = ADC_20480000;

	error = Standard_writeRegisters (demodulator, Processor_OFDM, sfoe_NS_2k_GI32_15_8, 24, ADC_buf_ptr);
	if (error) goto exit;


	/** Program CFOE */
	if (adcFrequency == 20250000) {
		 coeff1_2048Nu = 0x0335EDD7;
		 coeff1_4096Nu = 0x019AF6EB;
		 coeff1_8191Nu = 0x00CD81E2;
		 coeff1_8192Nu = 0x00CD7B76;
		 coeff1_8193Nu = 0x00CD750A;
		 coeff2_2k = 0x019AF6EB;
		 coeff2_4k = 0x00CD7B76;
		 coeff2_8k = 0x0066BDBB;
		 bfsfcw_fftindex_ratio = 0x027E;
		 fftindex_bfsfcw_ratio = 0x019B;
	} else if (adcFrequency == 20480000) {
		 coeff1_2048Nu = 0x032CB2CB;
		 coeff1_4096Nu = 0x01965966;
		 coeff1_8191Nu = 0x00CB330C;
		 coeff1_8192Nu = 0x00CB2CB3;
		 coeff1_8193Nu = 0x00CB265A;
		 coeff2_2k = 0x01965966;
		 coeff2_4k = 0x00CB2CB3;
		 coeff2_8k = 0x00659659;
		 bfsfcw_fftindex_ratio = 0x0285;
		 fftindex_bfsfcw_ratio = 0x0196;
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
		int channel_no = 0;
	unsigned char register_set = 0;
	unsigned char isSetSDRAMClock = 0;
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
	//----------- set SDRAM Clock

	error = Standard_readRegister(demodulator, Processor_OFDM, var_API_PLL1_CTRL_EN, &isSetSDRAMClock);
	if (error) goto exit;

	if(isSetSDRAMClock){
		if(frequency < 400000){
			// v-band
			if(frequency > 177143)
				channel_no = (Byte)((frequency - 177143)/6000) + 7;
			else
				channel_no = 7;
			if(channel_no > 13)
				channel_no = 13;
		}else if(frequency < 803147){
			if(frequency < 473143)
				channel_no = 14;
			else
				channel_no = (Byte)((frequency - 473143)/6000) + 14;
		}else{
			channel_no = 69;
		}
		// find nearest Channel
		if((channel_no < 69) && (frequency > 177143)){
			if(frequency < 400000){
				if((frequency < 213143)){
					if((frequency - ((channel_no-7)*6000 + 177143)) > (((channel_no-7+1)*6000 + 177143) - frequency))
						channel_no++;

					if(channel_no > 13)
						channel_no = 13;
				}else{
					channel_no = 13;
				}
			}else{
				if(frequency > 473143){
					if((frequency - ((channel_no-14)*6000 + 473143)) > (((channel_no-14+1)*6000 + 473143) - frequency))
						channel_no++;
					
				}else{
					channel_no = 14;
				}
			}
		}
		register_set = Channel_RegisterSet_Map[channel_no];

		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfb25, SDRAM_Clock_Table[register_set][0]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbb5, SDRAM_Clock_Table[register_set][1]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbb6, SDRAM_Clock_Table[register_set][2]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbb7, SDRAM_Clock_Table[register_set][3]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbb9, SDRAM_Clock_Table[register_set][4]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbb9, SDRAM_Clock_Table[register_set][5]);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, 0xfbba, SDRAM_Clock_Table[register_set][6]);
		if (error) goto exit;
		
	}
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
	//DefaultDemodulator* demod = (DefaultDemodulator*)demodulator; 

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
			//if (Cmd_loadFirmware != NULL) {
				error = Cmd_loadFirmware (demodulator, firmwareLength, firmwareCodesPointer);
			//}
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
	//if (Cmd_writeRegisters != NULL) {
		error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, bufferLength, buffer);
	//}

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
		//if (Cmd_writeRegisters != NULL) {
			error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &value);
		//}
	} else {
		//if (Cmd_readRegisters != NULL) {
			error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
			if (error) goto exit;
		//}

		temp = REG_CREATE (value, temp, position, length);

		//if (Cmd_writeRegisters != NULL) {
			error = Cmd_writeRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
			if (error) goto exit;
		//}
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
	//if (Cmd_readRegisters != NULL) {
		error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, bufferLength, buffer);
	//}

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
	//if (Cmd_readRegisters != NULL) {
		error = Cmd_readRegisters (demodulator, processor, registerAddress, registerAddressLength, 1, &temp);
	//}
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

Dword Standard_getPostVitBer_LayerB (
    IN  Demodulator*    demodulator,
    OUT Dword*          postErrorCount, /** 24 bits */
    OUT Dword*          postBitCount,   /** 16 bits */
    OUT Word*           abortCount
) {
	Dword error = Error_NO_ERROR;
	Dword errorCount;
	Dword bitCount;
	Byte buffer[7];
	Word abort;

	*postErrorCount = 0;
	*postBitCount  = 0;

	error = Standard_readRegisters (demodulator, Processor_OFDM, rsd_abort_packet_cnt_B_7_0, r_rsd_packet_unit_B_15_8 - rsd_abort_packet_cnt_B_7_0 + 1, buffer);
	if (error) goto exit;

	abort = ((Word) buffer[rsd_abort_packet_cnt_B_15_8 - rsd_abort_packet_cnt_B_7_0] << 8) + buffer[rsd_abort_packet_cnt_B_7_0 - rsd_abort_packet_cnt_B_7_0];
	errorCount = ((Dword) buffer[rsd_bit_err_cnt_B_23_16 - rsd_abort_packet_cnt_B_7_0] << 16) + ((Dword) buffer[rsd_bit_err_cnt_B_15_8 - rsd_abort_packet_cnt_B_7_0] << 8) + buffer[rsd_bit_err_cnt_B_7_0 - rsd_abort_packet_cnt_B_7_0];
	bitCount = ((Dword) buffer[r_rsd_packet_unit_B_15_8 - rsd_abort_packet_cnt_B_7_0] << 8) + buffer[r_rsd_packet_unit_B_7_0 - rsd_abort_packet_cnt_B_7_0];
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

Dword Standard_getPostVitBer_LayerC (
    IN  Demodulator*    demodulator,
    OUT Dword*          postErrorCount, /** 24 bits */
    OUT Dword*          postBitCount,   /** 16 bits */
    OUT Word*           abortCount
) {
	Dword error = Error_NO_ERROR;
	Dword errorCount;
	Dword bitCount;
	Byte buffer[7];
	Word abort;

	*postErrorCount = 0;
	*postBitCount  = 0;

	error = Standard_readRegisters (demodulator, Processor_OFDM, rsd_abort_packet_cnt_C_7_0, r_rsd_packet_unit_C_15_8 - rsd_abort_packet_cnt_C_7_0 + 1, buffer);
	if (error) goto exit;

	abort = ((Word) buffer[rsd_abort_packet_cnt_C_15_8 - rsd_abort_packet_cnt_C_7_0] << 8) + buffer[rsd_abort_packet_cnt_C_7_0 - rsd_abort_packet_cnt_C_7_0];
	errorCount = ((Dword) buffer[rsd_bit_err_cnt_C_23_16 - rsd_abort_packet_cnt_C_7_0] << 16) + ((Dword) buffer[rsd_bit_err_cnt_C_15_8 - rsd_abort_packet_cnt_C_7_0] << 8) + buffer[rsd_bit_err_cnt_C_7_0 - rsd_abort_packet_cnt_C_7_0];
	bitCount = ((Dword) buffer[r_rsd_packet_unit_C_15_8 - rsd_abort_packet_cnt_C_7_0] << 8) + buffer[r_rsd_packet_unit_C_7_0 - rsd_abort_packet_cnt_C_7_0];
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

Dword Standard_setViterbiRange_LayerB (
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
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_B_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_B_15_8, 1, &temp1);
	if (error) goto exit;

exit:


	return (error);
}

Dword Standard_setViterbiRange_LayerC (
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
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_C_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_writeRegisters (demodulator, Processor_OFDM, rsd_packet_unit_C_15_8, 1, &temp1);
	if (error) goto exit;

exit:
	return (error);
}

Dword Standard_getViterbiRange_LayerB (
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
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_B_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_B_15_8, 1, &temp1);
	if (error) goto exit;
	*packetUnit = (Word) (temp1 << 8) + (Word) temp0;

exit:
	return (error);
}

Dword Standard_getViterbiRange_LayerC (
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
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_C_7_0, 1, &temp0);
	if (error) goto exit;
	error = Standard_readRegisters (demodulator, Processor_OFDM, r_rsd_packet_unit_C_15_8, 1, &temp1);
	if (error) goto exit;
	*packetUnit = (Word) (temp1 << 8) + (Word) temp0;

exit:
	return (error);
}

Dword Standard_getChannelModulation_LayerB (
	IN  Demodulator*            demodulator,
	OUT ChannelModulation*      channelModulation
) {
	Dword error = Error_NO_ERROR;

	Byte temp;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Get constellation type */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_carmod_b, reg_carmod_b_pos, reg_carmod_b_len, &temp);
	if (error) goto exit;
	channelModulation->constellation = (Constellation) temp;

	/** Get high code rate */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_cr_b, reg_cr_b_pos, reg_cr_b_len, &temp);
	if (error) goto exit;
	channelModulation->highCodeRate = (CodeRate) temp;
	// ISDB-T only has one code rate
	channelModulation->lowCodeRate  = (CodeRate) temp;

	/** Get guard interval */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_gi, reg_gi_pos, reg_gi_len, &temp);
	if (error) goto exit;
	channelModulation->interval = (Interval) temp;

	/** Get FFT mode */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_txmod, reg_txmod_pos, reg_txmod_len, &temp);
	if (error) goto exit;
	channelModulation->transmissionMode = (TransmissionModes) temp;

	/** Get bandwidth */
	// The g_reg_bw is a fake register
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, &temp);
	if (error) goto exit;
	channelModulation->bandwidth = (Bandwidth) Bandwidth_6M;

	/** Get frequency */
	channelModulation->frequency = demod->frequency;

exit :
	return (error);
}

Dword Standard_getChannelModulation_LayerC (
	IN  Demodulator*            demodulator,
	OUT ChannelModulation*      channelModulation
) {
	Dword error = Error_NO_ERROR;

	Byte temp;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Get constellation type */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_carmod_c, reg_carmod_c_pos, reg_carmod_c_len, &temp);
	if (error) goto exit;
	channelModulation->constellation = (Constellation) temp;

	/** Get high code rate */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_cr_c, reg_cr_c_pos, reg_cr_c_len, &temp);
	if (error) goto exit;
	channelModulation->highCodeRate = (CodeRate) temp;
	// ISDB-T only has one code rate
	channelModulation->lowCodeRate  = (CodeRate) temp;

	/** Get guard interval */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_gi, reg_gi_pos, reg_gi_len, &temp);
	if (error) goto exit;
	channelModulation->interval = (Interval) temp;

	/** Get FFT mode */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_txmod, reg_txmod_pos, reg_txmod_len, &temp);
	if (error) goto exit;
	channelModulation->transmissionMode = (TransmissionModes) temp;

	/** Get bandwidth */
	// The g_reg_bw is a fake register
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, &temp);
	if (error) goto exit;
	channelModulation->bandwidth = (Bandwidth) Bandwidth_6M;

	/** Get frequency */
	channelModulation->frequency = demod->frequency;

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
	OUT Long*           strengthDbm           /** DBm                                 */
)
{
	Dword error = Error_NO_ERROR;
	Byte temp;

	error = Standard_readRegister (demodulator, Processor_OFDM, var_p_inband, &temp);
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
	Dword chip_Type;
	Byte var[2];
	Byte chip_version = 0;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	demod->fcw = 0x00000000;
	demod->frequency = 642000;
	demod->initialized = False;



	Standard_clockTable[0].crystalFrequency = 12000;
	Standard_clockTable[0].adcFrequency = 20250000;
	Standard_clockTable[1].crystalFrequency = 20480;
	Standard_clockTable[1].adcFrequency = 20480000;
	
	error = Standard_readRegister(demodulator, Processor_LINK, chip_version_7_0, &chip_version);
	if (error) goto exit;
	error = Standard_readRegisters(demodulator, Processor_LINK, chip_version_7_0+1, 2, var);
	if (error) goto exit;
	chip_Type = var[1]<<8 | var[0];	
	
	
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
	
	error = Standard_writeRegister (demodulator, Processor_OFDM, 0xEC4C, 0xa8);
    if (error) if (error) goto exit;
    
    error = Standard_writeRegister (demodulator, Processor_LINK, 0xE00C, 0); //link E00C = 0  //disable SDRAM powerdown
	if (error) goto exit;
	error = Standard_writeRegister (demodulator, Processor_LINK, 0xD864, 0); //link D864 = 0  //SDRAM pad driving to minimum
	if (error) goto exit;   
	
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

    
    error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0x01);
    if (error) goto exit;                 
        
    /** Set 0xD827 to 0 as open drain for tuner i2c */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padodpu, 0);
	if (error) goto exit;

	/** Set 0xD829 to 0 as push pull for tuner AGC */
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_agc_od, 0);
	if (error) goto exit;
            
                     
    /** Open tuner */                 
	if (tunerDescription.openTunerFunc != NULL) {
		error = tunerDescription.openTunerFunc (demodulator);
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

   error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdrsr, 1);
   if (error) goto exit;

	/** Set registers for driving power 0xD830 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr2, 0);
	if (error) goto exit;
	
		
    /** enhance the performance while using DIP crystals **/
	error = Standard_writeRegister (demodulator, Processor_OFDM, 0xEC57, 0);
	if (error) goto exit;
	error = Standard_writeRegister (demodulator, Processor_OFDM, 0xEC58, 0);
	if (error) goto exit;
	
	/** Set ADC frequency multiplier **/
	error = Standard_setMultiplier(demodulator,Multiplier_1X); 
	if (error) goto exit;

	/** Set registers for driving power 0xD831 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr4, 0);
	if (error) goto exit;

	/** Set registers for driving power 0xD832 **/
	error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_top_padmiscdr8, 0);
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

	error = Standard_readRegisterBits (demodulator, Processor_OFDM, p_reg_r_tmcc_lock, reg_r_tmcc_lock_pos, reg_r_tmcc_lock_len, &temp);
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
	//Bool retry = True;
	demod = (DefaultDemodulator*) demodulator;
	
//check_lock:  
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
			goto exit;
		}
		User_delay (demodulator, 25);
		emptyLoop++;
	}

	if (emptyChannel == 1) goto exit;

	while (tpsLoop < 100) { //dyh
		/** Empty channel */
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

	while (mpeg2Loop < 100) { //timeout = loop* 25ms,dyh
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
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_carmod_a, reg_carmod_a_pos, reg_carmod_a_len, &temp);
	if (error) goto exit;
	channelModulation->constellation = (Constellation) temp;

	/** Get high code rate */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_cr_a, reg_cr_a_pos, reg_cr_a_len, &temp);
	if (error) goto exit;
	channelModulation->highCodeRate = (CodeRate) temp;
	// ISDB-T only has one code rate
	channelModulation->lowCodeRate  = (CodeRate) temp;

	/** Get guard interval */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_gi, reg_gi_pos, reg_gi_len, &temp);
	if (error) goto exit;
	channelModulation->interval = (Interval) temp;

	/** Get FFT mode */
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_txmod, reg_txmod_pos, reg_txmod_len, &temp);
	if (error) goto exit;
	channelModulation->transmissionMode = (TransmissionModes) temp;

	/** Get bandwidth */
	// The g_reg_bw is a fake register
	error = Standard_readRegisterBits (demodulator, Processor_OFDM, g_reg_bw, reg_bw_pos, reg_bw_len, &temp);
	if (error) goto exit;
	channelModulation->bandwidth = (Bandwidth) Bandwidth_6M;

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

	//Dword warning = Error_NO_ERROR;
	DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

	/** Enable DVB-T mode */
	error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_dvbt_en, reg_dvbt_en_pos, reg_dvbt_en_len, 1);
	if (error) goto exit;

	/** Enter sub mode */
	switch (streamType) {
        case StreamType_NONE:
        case StreamType_DVBT_DATAGRAM:
             error = Error_INVALID_DEV_TYPE;
             break;                
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
		if (error) goto exit;
		
		error = User_mpegConfig (demodulator);
        if (error) goto exit;
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
	//Byte buffer[2];

	demod = (DefaultDemodulator*) demodulator;

	/** Get statistic by stream type */
	error = Standard_isTpsLocked (demodulator, &demod->statistic.signalPresented);
	if (error) goto exit;
	
    error = Standard_isMpeg2Locked (demodulator, &demod->statistic.signalLocked);
	if (error) goto exit;
	
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
	Byte value[15] = {0};
	demod = (DefaultDemodulator*) demodulator;


	if (control) {
		/** Power up case */
        error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_p_if_en,  1);
	    if (error) goto exit;
	    error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_afe_mem0, 3, 1, 0);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_dyn0_clk, 0);
		if (error) goto exit;
		
				
		// resume the SDRAM

         error = Standard_writeRegisterBits (demodulator, Processor_OFDM, 0xFBB9, 5, 1, 0);
         if (error) goto exit;

         error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_sdc_endpd, 0);
         if (error) goto exit;

         error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_deintlv_en, 0);
         if (error) goto exit;

         error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_deintlv_en, 1);
         if (error) goto exit;		

	
		/** Set the desired stream type */
         error = Standard_setStreamType (demodulator, demod->streamType);
		 if (error) goto exit;

			/** Set the desired architecture type */
         error = Standard_setArchitecture (demodulator);
         if (error) goto exit;
		

	} else {
		/** Power down case */
		// suspend the SDRAM
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, 0xFBB9, 5, 1, 0);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_LINK, p_reg_sdc_endpd, 1);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_deintlv_en, 0);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_deintlv_en, 1);
		if (error) goto exit;		
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, 0xFBB9, 5, 1, 1);
		if (error) goto exit;
		
		error = Standard_writeRegister (demodulator, Processor_LINK, 0xD91B, 0x1);
		if (error) goto exit;
        error = Standard_writeRegister (demodulator, Processor_LINK, 0xD91C, 0x0);
		if (error) goto exit;
 
		//--suspend ofdm       
		
    	error = Standard_writeRegister (demodulator, Processor_OFDM, suspend_flag, 1);
		if (error) goto exit;
		error = Standard_writeRegister (demodulator, Processor_OFDM, trigger_ofsm, 0);
		if (error) goto exit;

		for (j = 0; j < 150; j++) {
			error = Standard_readRegister (demodulator, Processor_OFDM, suspend_flag, &temp);
			if (error) goto exit;
			if (!temp) break;
			User_delay (demodulator, 10);
		}
		error = Standard_writeRegisterBits (demodulator, Processor_OFDM, p_reg_afe_mem0, 3, 1, 1);
		if (error) goto exit;
		
		error = Standard_writeRegister (demodulator, Processor_OFDM, p_reg_dyn0_clk, 0);
		if (error) goto exit;
		//0xec40
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


Dword Standard_setMultiplier (
    IN  Demodulator*    demodulator,
    IN  Multiplier      multiplier
) {
    Dword error = Error_NO_ERROR;

    Byte newAdcMultiplier = 0;
    Byte buffer[3];
    //Long controlWord;
    //Byte i;
	Dword fcw;
    DefaultDemodulator* demod;

	demod = (DefaultDemodulator*) demodulator;

    if (multiplier == Multiplier_1X)
        newAdcMultiplier = 0;
    else
        newAdcMultiplier = 1;

   

    /** Write ADC multiplier factor to firmware. */
    error = Standard_writeRegister (demodulator, Processor_OFDM, adcx2, newAdcMultiplier);
    if (error) goto exit;


    /** Compute FCW and load them to device */
    error = Standard_computeFcw (demodulator, (Long) demod->adcFrequency, (Long) tunerDescription.ifFrequency, tunerDescription.inversion, &fcw);
	if (error) goto exit;	
	demod->fcw = fcw;


    buffer[0] = (Byte) (demod->fcw & 0x000000FF);
    buffer[1] = (Byte) ((demod->fcw & 0x0000FF00) >> 8);
    buffer[2] = (Byte) ((demod->fcw & 0x007F0000) >> 16);
   	error = Standard_writeRegisters (demodulator, Processor_OFDM, bfs_fcw_7_0, bfs_fcw_22_16 - bfs_fcw_7_0 + 1, buffer);


exit :
    return (error);
}


Dword Standard_reboot (
	IN  Demodulator*    demodulator
)  {
	Dword error = Error_NO_ERROR;
	Dword version;
	Byte i;
	DefaultDemodulator* demod;
	
	//Byte chip_version = 0;
	
    demod = (DefaultDemodulator*) demodulator;    
	
	error = Standard_getFirmwareVersion (demodulator, Processor_LINK, &version);
	if (error) goto exit;
	if (version == 0xFFFFFFFF) goto exit;       /** I2M and I2U */
	if (version != 0) {              

		error = Cmd_reboot (demodulator);		
		User_delay (demodulator, 10);
		version = 1;
		for (i = 0; i < 30; i++) {
			error = Standard_getFirmwareVersion (demodulator, Processor_LINK, &version);
			if (error == Error_NO_ERROR) break;
			User_delay (demodulator, 10);
		}
		if (error) 
			goto exit;
		
		if (version != 0)
			error = Error_REBOOT_FAIL;
	}

	error = Standard_writeRegisterBits (demodulator, Processor_LINK, p_reg_p_dmb_sw_reset, reg_p_dmb_sw_reset_pos, reg_p_dmb_sw_reset_len, 1);
	if (error) goto exit;

	demod->booted = False;

exit :
	return (error);
}

Dword Standard_getSNR (
    IN  Demodulator*    demodulator,
    OUT Byte*           snr
) {
    
    Dword error = Error_NO_ERROR;
    
    Byte   snr_value;

	error = Standard_readRegister(demodulator, Processor_OFDM, var_SNR, &snr_value); //gets superFrame num
    if(error) goto exit;
	
	*snr = snr_value; 
exit:    
    return error;
    
}
