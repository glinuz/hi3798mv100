#include "TP_TYPE.h"
#include "TP5001.h"
#include "linux/string.h"
#include "linux/ctype.h"
#include "hi_drv_tuner.h"

//#include "IIC.h"
#include "linux/kernel.h"
#include "asm/div64.h"

#ifdef _USE_TP5001_CHIP_

// ==========================================================================================================
// Marco & define
// 
//#define FPGA_PLATFORM
//#define TUNER_CONTROL_BY_HOST

#define MAX 200
extern HI_U32 genI2cChannel;
extern HI_U32 gTP5001_addr;
extern HI_U32 gTP5001_tuner_addr;

// ==========================================================================================================


// ==========================================================================================================
// Global Variable
// 
const TP_UINT8 g_code_version[] = {"TP500101.62"};
TP_UINT32 g_symbol_rate;
TP_UINT8 gTuner_initialized = 0;
TP_UINT8 gxtal_sel;
TP_UINT8 gui_tuner_type;
TP_UINT8 g_chip_type;
// ==========================================================================================================


// ==========================================================================================================
// Internal Function
//
// ----------------------------------------------------------------------------------------------------------
// Description: used by set_symbol_rate function
//

TP_UINT8 getN(TP_UINT32 symbol_rate, TP_UINT32 sample_clock)
{
	TP_INT32 A;

	A = (3*sample_clock) / (5 * symbol_rate);

	if(A>=1 && A <= 2)
		return 1;
	else if(A>2 && A<=4)
		return 2;
	else if(A>4 && A<=8)
		return 4;
	else if(A>8 && A<=16)
		return 8;
	else if(A>16 && A<=32)
		return 16;
	else if(A>32 && A<=64)
		return 32;
	else 
		return 0;
}
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// Host iic write/read one byte
//
TP_UINT8 IIC_WRITE_ONE_BYTE(TP_UINT16 REG_ADDR, TP_UINT8 REG_VALUE)
{
 	TP_UINT8 result;
	result = TP_iic_write(gTP5001_addr, REG_ADDR, &REG_VALUE, 1);
	return result;
}

TP_UINT8 IIC_READ_ONE_BYTE(TP_UINT16 REG_ADDR, TP_UINT8* REG_VALUE) 
{	
	TP_UINT8 result;
	result = TP_iic_read(gTP5001_addr, REG_ADDR, REG_VALUE, 1);
  	return result;
}
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// TP5001 IIC Master used for tuner, read and write
//
TP_UINT8 IIC_repeat_start_read(TP_UINT8 reg_addr, TP_UINT8 * read_buffer, TP_UINT32 read_length)
{
	TP_UINT16 tuner_addr;
	TP_UINT8 register_value;
	TP_UINT8 i,result;

	// ??\u017d?0x0c bit0 is 1  cmd_idle
	i=0;
	while(1)
	{
		result = IIC_READ_ONE_BYTE(0xFE0C, &register_value);
		if(result != TP_SUCCESS)
			return result;
		i++;
		if((register_value & 0x01) == 1)
			break;
		if(i==200)
			return TP_IIC_IDLE_ERR;
	}

	result = IIC_WRITE_ONE_BYTE(0xFE10, reg_addr);  
	if(result != TP_SUCCESS)
		return result;

	register_value = 0x80 | (read_length-1);
	result = IIC_WRITE_ONE_BYTE(0xFE08, register_value);
	if(result != TP_SUCCESS)
		return result;
	
	i=0;
	while(1)
	{
		result = IIC_READ_ONE_BYTE(0xFE0C, &register_value);
		if(result != TP_SUCCESS)
			return result;
		i++;
		if((register_value & 0x01) == 1)
		{
			if((register_value & 0x06) != 0)
			{
				return TP_IIC_IDLE_ERR;
			}
			break;
		}
		if(i==200)
			return TP_IIC_IDLE_ERR;
	}

	tuner_addr = 0xFE20;
	for(i=0; i<read_length; i++)
	{
		result = IIC_READ_ONE_BYTE(tuner_addr, &read_buffer[i]);
		if(result != TP_SUCCESS)
			return result;
		tuner_addr ++;
	}
	return TP_SUCCESS;
}



static TP_UINT8 tuner_register_read(TP_UINT8 device_addr,TP_UINT8 reg_addr, TP_UINT8 * read_buffer, TP_UINT32 read_length)
{
#ifdef TUNER_CONTROL_BY_HOST
	return TP_iic_read_tuner(device_addr, reg_addr, read_buffer, read_length);
#else
	TP_UINT8 result;
	if(read_length > 16)
		return TP_TUNER_IIC_WR_TOO_LONG;
	
	result = IIC_repeat_start_read(reg_addr,read_buffer,read_length);
	if(result != TP_SUCCESS)
		return result;
#endif

	return TP_SUCCESS;
}



static TP_UINT8 tuner_register_write(TP_UINT8 device_addr, TP_UINT8 * write_buffer, TP_UINT32 write_length)
{
#ifdef TUNER_CONTROL_BY_HOST
	return TP_iic_write_tuner(device_addr, write_buffer, write_length);
#else

	TP_UINT16 tuner_addr;
	TP_UINT8 register_value;
	TP_UINT8 i,result;

	if(write_length > 16)
		return TP_TUNER_IIC_WR_TOO_LONG;
	
	// ??\u017d?0x0c bit0 is 1  cmd_idle
	i=0;
	while(1)
	{
		result = IIC_READ_ONE_BYTE(0xFE0C, &register_value);
		if(result != TP_SUCCESS)
			return result;
		i++;
		if((register_value & 0x01) == 1)
			break;
		if(i==200)
			return TP_IIC_IDLE_ERR;
	}

	tuner_addr = 0xFE10;

	for(i=0; i<write_length; i++)
	{
		result = IIC_WRITE_ONE_BYTE(tuner_addr, write_buffer[i]);
		if(result != TP_SUCCESS)
			return result;
		tuner_addr++;
	}
	register_value = 0x40 | (write_length-1);
	result = IIC_WRITE_ONE_BYTE(0xFE08, register_value);
	if(result != TP_SUCCESS)
		return result;


	// ??\u017d?0x0c bit0 is 1  cmd_idle
	i=0;
	while(1)
	{
		result = IIC_READ_ONE_BYTE(0xFE0C, &register_value);
		if(result != TP_SUCCESS)
			return result;
		i++;
		if((register_value & 0x01) == 1)
		{
			if((register_value & 0x06) != 0)
			{
				return TP_IIC_IDLE_ERR;
			}
			break;
		}
		if(i==200)
			return TP_IIC_IDLE_ERR;
	}

	return TP_SUCCESS;
#endif
}
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// tuner iic write
// device_addr: input, never to use, device addr must set after system_init(), write 0xFE01 register
// write_buffer: input, buffer to store reg + data
// write_length: input, length of reg + data
// 
TP_UINT8 TP_iic_tuner_write(TP_UINT8 device_addr, TP_UINT8 * write_buffer, TP_UINT32 write_length)
{
	return tuner_register_write(device_addr, write_buffer,write_length);
}
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// tuner iic read
// device_addr: input, never to use, device addr must set after system_init(), write 0xFE01 register
// reg_addr: register address
// value: read value buffer
// length: read value length
//
TP_UINT8 TP_iic_tuner_read(TP_UINT8 device_addr, TP_UINT8 reg_addr, TP_UINT8 * value, TP_UINT32 length)
{
	return tuner_register_read(0, reg_addr,value,length);
}
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// system init: used for PLL ClockEnable IIC TS and some system level init
//
TP_UINT8 system_init(void)
{
	TP_UINT8 result;
	TP_UINT8 reg_value[5];

// ???\u0152??IIC?˿?
	result = IIC_WRITE_ONE_BYTE(0xFF80, 0x00);
	if(result != TP_SUCCESS)
		return result;

	// check TP5001 chip ID
	result = TP_iic_read(gTP5001_addr, 0x3F0F, reg_value, 2);
	if(result != TP_SUCCESS)
		return result;
	if(reg_value[0] != 0x50 || reg_value[1] != 0x01)
		return TP_CHIP_ID_ERROR;

#ifndef FPGA_PLATFORM
	// PLL CFG
	// PLL ?ο\u0152ʱ??????ʹ??
	// PEG_CLK_SEL ??\u0153???
	// PLL LOCKӲ\u0152??Զ?ѡ??
	// PLL ??Ƶ????Ӳ\u0152??Զ?ѡ??
	// PLL IPʹ??

	// get xtal pin 
	result = IIC_READ_ONE_BYTE(0x3F09, &gxtal_sel);
	if(result != TP_SUCCESS)
	{
		gxtal_sel = 0;
		return result;
	}
	//clk div cfg :spi  sys ldpc
	if(gxtal_sel == 3)
	{
		reg_value[0] = 0x38;
		reg_value[1] = 0x05;
		reg_value[2] = 0x02;
		reg_value[3] = 0x05; 
	}
	else
	{
		reg_value[0] = 0x25;
		reg_value[1] = 0x03;
		reg_value[2] = 0x01;
		reg_value[3] = 0x03; 
	}
	result = TP_iic_write(gTP5001_addr, 0x3F0B, reg_value, 4);
	if(result != TP_SUCCESS)
		return result;

// set PLL =============================================
	result = IIC_WRITE_ONE_BYTE(0x3f00, 0xD7);      // PDRST????\u017eߵ??\u0153
	if(result != TP_SUCCESS)
		return result;

	//XIN  <---->  clk cfg    PLL_OUT_CLK 400MHz
	//Pll_m_cfg Pll_od_cfg Pll_n_cfg    
	//dvbspi_pdiv  Sys_pdiv_reg  Ldpc_pdiv_reg 
	if(gxtal_sel == 0)    //4M 
	{
		reg_value[0] = 190;
		reg_value[1] = 0x01;
		reg_value[2] = 0x01;
		reg_value[3] = 0x00;	//low 6 bits
		reg_value[4] = 0x23;	//high 8 bits
		result = TP_iic_write(gTP5001_addr, 0x3F04, reg_value, 5);
		if(result != TP_SUCCESS)
			return result;
	}
	else if(gxtal_sel == 1)//10M
	{
		reg_value[0] = 76;
		reg_value[1] = 0x01;
		reg_value[2] = 0x01;
		reg_value[3] = 0x00;	//low 6 bits
		reg_value[4] = 0x54;	//high 8 bits
		result = TP_iic_write(gTP5001_addr, 0x3F04, reg_value, 5);
		if(result != TP_SUCCESS)
			return result;
	}
	else if(gxtal_sel == 2)//20M
	{
		reg_value[0] = 76;
		reg_value[1] = 0x01;
		reg_value[2] = 0x02;
		reg_value[3] = 0x00;	//low 6 bits
		reg_value[4] = 0xa8;	//high 8 bits
		result = TP_iic_write(gTP5001_addr, 0x3F04, reg_value, 5);
		if(result != TP_SUCCESS)
			return result;	
	}
	else if(gxtal_sel == 3)//27M
	{
		reg_value[0] = 0xBE;
		reg_value[1] = 0;
		reg_value[2] = 9;
		reg_value[3] = 0x00;	//low 6 bits
		reg_value[4] = 0xe8;	//high 8 bits
		result = TP_iic_write(gTP5001_addr, 0x3F04, reg_value, 5);
		if(result != TP_SUCCESS)
			return result;
	}

	result = IIC_WRITE_ONE_BYTE(0x3f00, 0xD6);   // ??????M N OD ֮??PDRST -?? 
	if(result != TP_SUCCESS)
		return result;
//==========end of set pll====================

//set adc cfg0 and adc_en  ========================
	result = IIC_WRITE_ONE_BYTE(0x3f01, 0xF3);  //72<->10bit  73 <-> 8bit
	if(result != TP_SUCCESS)
		return result;
	result = IIC_WRITE_ONE_BYTE(0x3f03, 0x03);  //07 <->high 8 bit 03 <->low 8bit
	if(result != TP_SUCCESS)
		return result;
// ========== end set adc ===========================
	//clk_en
	result = IIC_WRITE_ONE_BYTE(0x3f0A, 0x7F);   
	if(result != TP_SUCCESS)
		return result;
	

#endif

	// set GPIO is output, default is 0
	result = IIC_WRITE_ONE_BYTE(0x3D01, 0xFF);
	if(result != TP_SUCCESS)
		return result;
	
	// ????TS\u0153ӿ?
	if(gxtal_sel == 3)
		reg_value[0] = 0x38;  //
	else
		reg_value[0] = 0x25;  // clockΪ396(396.6)/40 = 9.9MHz 
	result = TP_set_ts_interface(negitive_edge, para_port, reg_value[0]);	// Ĭ?? ??\u0153??ز??? ???ж˿? 
	if(result != TP_SUCCESS)
		return result;

#ifdef SERIES_PORT
	if(gxtal_sel == 3)
		reg_value[0] = 7;		// 570/(7+1) = 71.25Mhz
	else
		reg_value[0] = 4; 	// 380/(4+1) = 76MHz
	result = TP_set_ts_interface(negitive_edge, series_port, reg_value[0]);	// Ĭ?? ??\u0153??ز??? ???ж˿? 
	if(result != TP_SUCCESS)
		return result;
#endif

	//  IIC_MSTʹ??
	result = IIC_WRITE_ONE_BYTE(0xFE00, 0x01);
	if(result != TP_SUCCESS)
		return result;

	// ????Tuner IIC?ٶ?Ϊ400KHz Ĭ??clockΪ95MHz / 238 = 399KHz
	result = IIC_WRITE_ONE_BYTE(0xFE02, 0xEA);
	if(result != TP_SUCCESS)
		return result;

	// set ldpc output mode, \u0152?????????\u0178ݶ???
	result = IIC_WRITE_ONE_BYTE(0x0A10, 0x04);
	if(result != TP_SUCCESS)
		return result;
	return TP_SUCCESS;
}

// ==========================================================================================================


// ==========================================================================================================
// External Function
//

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_version
// Description: TP5001?\u0178Ƭ???õ\u0153???\u0178??
// Output:
//		pVersion: ????SDK???\u0178?ŵ?Buffer????Ҫ16??\u0153ڵ?\u017d?\u017d???\u0152?
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_version(TP_INT8 * pVersion)
{
	TP_UINT8 i;
	
	for(i=0; i<sizeof(g_code_version); i++)
	{
		*(pVersion + i) = g_code_version[i];
	}
	
	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_symbol_rate
// Description: TP5001?\u0178Ƭ?????÷?????
// Input:
//		symbol_rate:  ?????ʣ???λHz
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//

TP_UINT8 TP_set_symbol_rate(TP_UINT32 symbol_rate)
{

	TP_UINT64 temp;
	TP_UINT32 res;
	TP_UINT8 result,N;
	TP_UINT8 reg_value[8];
	TP_UINT32 sample_rate;
	
	g_symbol_rate = symbol_rate;

	if(symbol_rate >= 20000000)
	{
		if(gui_tuner_type == SHARP_6306_TUNER)
		{
			reg_value[0] = 0x16;   //wnr change
			reg_value[1] = 0x06;   // wnr change
		}
		else
		{  
			reg_value[0] = 0x15;   //wnr change   15 
			reg_value[1] = 0x05;   // wnr change   05
		}
		
	}
	else if(symbol_rate >= 8000000)
	{
		reg_value[0] = 0x14;   //wnr change   15 
		reg_value[1] = 0x04 ;   // wnr change   05
	}
	else if(symbol_rate >= 3000000)
	{
		reg_value[0] = 0x12;   //wnr change   15 
		reg_value[1] = 0x02;   // wnr change   05
	}
	else
	{
		reg_value[0] = 0x11;   //wnr change   15 
		reg_value[1] = 0x01;   // wnr change   05
	}

	result = TP_iic_write(gTP5001_addr, 0x0538, reg_value, 2);
	if(result != TP_SUCCESS)
		return result;
	
	// demod soft reset
	result = IIC_WRITE_ONE_BYTE(0x1100,0xfe);
	if(result != TP_SUCCESS)
		return result;
	result = IIC_WRITE_ONE_BYTE(0x1100,0xfc);
	if(result != TP_SUCCESS)
		return result;

#ifndef FPGA_PLATFORM	
		sample_rate = 95000000;
#else
	sample_rate = 60000000;
#endif

	temp = sample_rate;

// \u0152???\u0152?????bitsync????

	//temp /= getN(symbol_rate, sample_rate);  
	//temp /= symbol_rate;
	//temp *= 1048576;   //1<<20
	//temp /= 2;  


	N = getN(symbol_rate, sample_rate);  
	do_div(temp,N);
	temp = temp * (1<<19);
	do_div(temp ,symbol_rate);
	
	
	res = temp;
	reg_value[0] = (res) & 0xFF;
	reg_value[1] = (res>>8) & 0xFF;
	reg_value[2] = (res>>16) & 0xFF;
	result = TP_iic_write(gTP5001_addr, 0x0309, reg_value, 3);
	if(result != TP_SUCCESS)
		return result;
	
	// 	NCO Decimation Ratio
	//temp = symbol_rate;
	//temp /= sample_rate; //ADC?????? 
	//temp *= 32768;

	temp = symbol_rate;
	temp *= 32768;
	do_div(temp,sample_rate);

	res = temp;
	reg_value[0] = res & 0xFF;
	reg_value[1] = (res>>8) & 0x7F;
	result = TP_iic_write(gTP5001_addr, 0x0114, reg_value, 2);
	if(result != TP_SUCCESS)
		return result;
	
	
	//FIR
	reg_value[0] = getN(symbol_rate, sample_rate);
	switch(reg_value[0] )
	{
		case 1:
			reg_value[0] = 0;
			break;
		case 2:
			reg_value[0] = 1;
			break;
		case 4:
			reg_value[0] = 2;
			break;
		case 8:
			reg_value[0] = 3;
			break;
		case 16:
			reg_value[0] = 4;
			break;
		case 32:
			reg_value[0] = 5;
			break;
		default :
			break;
	}
	result = IIC_WRITE_ONE_BYTE(0x011f,reg_value[0] );
	if(result != TP_SUCCESS)
		return result;

	result = IIC_WRITE_ONE_BYTE(0x0D04,0x3a);
	if(result != TP_SUCCESS)
		return result;
	//????
	result = IIC_WRITE_ONE_BYTE(0x1100,0xfd);
	if(result != TP_SUCCESS)
		return result;

	// enable adc output
	result = IIC_WRITE_ONE_BYTE(0x3F01,0x73);
	if(result != TP_SUCCESS)
		return result;


	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_signal_quality
// Description: TP?\u0178Ƭ???õ\u0153?ź???
// Output:
//		p_quality_percent:  ?? 0: 0%  --   100: 100%
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_signal_quality(TP_UINT8 * p_quality_percent)
{       
	TP_UINT8 result;
	TP_UINT8 reg_buffer[5];
	TP_UINT32 beta, total;
	TP_UINT64 chn_pwr, noise_pwr;
	TP_UINT32 temp;
	TP_UINT8 i;
	TP_UINT8 mean_count;


	result = IIC_READ_ONE_BYTE(0x050d, reg_buffer);
	if(result != TP_SUCCESS)
		return result;

	if( (reg_buffer[0] & 0x01) == 0x01)
	{
		
		beta = total = 0;
		result = IIC_READ_ONE_BYTE(0x110d, reg_buffer);
		if(result != TP_SUCCESS)
			return result;
		if(reg_buffer[0] == 0x80)
			mean_count=100;
		else
			mean_count=1;		
		

		for(i=0; i<mean_count; i++) 
		{

			result = TP_iic_read(gTP5001_addr, 0x054d, reg_buffer, 5);
			if(result != TP_SUCCESS)
					return result;
		
			temp = reg_buffer[0] | (reg_buffer[1]<<8) | ((reg_buffer[2] & 0x03)<<16);

			beta += temp;

			temp = reg_buffer[3] | ((reg_buffer[4] & 0x7f)<<8);

			total += temp;
		}

		chn_pwr = beta;
		noise_pwr = total;

		if(noise_pwr == 0)
			chn_pwr = 0;
		else
		{
			//chn_pwr = chn_pwr *10000 / noise_pwr;
			chn_pwr *= 10000;
			do_div(chn_pwr,noise_pwr);
			
		}

		if(chn_pwr < 10000) chn_pwr = 0;
		else if(chn_pwr < 10233) chn_pwr = 1 ;
		else if(chn_pwr < 10471) chn_pwr = 2 ;
		else if(chn_pwr < 10715) chn_pwr = 3 ;
		else if(chn_pwr < 10965) chn_pwr = 4 ;
		else if(chn_pwr < 11220) chn_pwr = 5 ;
		else if(chn_pwr < 11482) chn_pwr = 6 ;
		else if(chn_pwr < 11749) chn_pwr = 7 ;
		else if(chn_pwr < 12023) chn_pwr = 8 ;
		else if(chn_pwr < 12303) chn_pwr = 9 ;
		else if(chn_pwr < 12589) chn_pwr = 10;
		else if(chn_pwr < 12882) chn_pwr = 11;
		else if(chn_pwr < 13183) chn_pwr = 12;
		else if(chn_pwr < 13490) chn_pwr = 13;
		else if(chn_pwr < 13804) chn_pwr = 14;
		else if(chn_pwr < 14125) chn_pwr = 15;
		else if(chn_pwr < 14454) chn_pwr = 16;
		else if(chn_pwr < 14791) chn_pwr = 17;
		else if(chn_pwr < 15136) chn_pwr = 18;
		else if(chn_pwr < 15488) chn_pwr = 19;
		else if(chn_pwr < 15849) chn_pwr = 20;
		else if(chn_pwr < 16218) chn_pwr = 21;
		else if(chn_pwr < 16596) chn_pwr = 22;
		else if(chn_pwr < 16982) chn_pwr = 23;
		else if(chn_pwr < 17378) chn_pwr = 24;
		else if(chn_pwr < 17783) chn_pwr = 25;
		else if(chn_pwr < 18197) chn_pwr = 26;
		else if(chn_pwr < 18621) chn_pwr = 27;
		else if(chn_pwr < 19055) chn_pwr = 28;
		else if(chn_pwr < 19498) chn_pwr = 29;
		else if(chn_pwr < 19953) chn_pwr = 30;
		else if(chn_pwr < 20417) chn_pwr = 31;
		else if(chn_pwr < 20893) chn_pwr = 32;
		else if(chn_pwr < 21380) chn_pwr = 33;
		else if(chn_pwr < 21878) chn_pwr = 34;
		else if(chn_pwr < 22387) chn_pwr = 35;
		else if(chn_pwr < 22909) chn_pwr = 36;
		else if(chn_pwr < 23442) chn_pwr = 37;
		else if(chn_pwr < 23988) chn_pwr = 38;
		else if(chn_pwr < 24547) chn_pwr = 39;
		else if(chn_pwr < 25119) chn_pwr = 40;
		else if(chn_pwr < 25704) chn_pwr = 41;
		else if(chn_pwr < 26303) chn_pwr = 42;
		else if(chn_pwr < 26915) chn_pwr = 43;
		else if(chn_pwr < 27542) chn_pwr = 44;
		else if(chn_pwr < 28184) chn_pwr = 45;
		else if(chn_pwr < 28840) chn_pwr = 46;
		else if(chn_pwr < 29512) chn_pwr = 47;
		else if(chn_pwr < 30200) chn_pwr = 48;
		else if(chn_pwr < 30903) chn_pwr = 49;
		else if(chn_pwr < 31623) chn_pwr = 50;
		else if(chn_pwr < 32359) chn_pwr = 51;
		else if(chn_pwr < 33113) chn_pwr = 52;
		else if(chn_pwr < 33884) chn_pwr = 53;
		else if(chn_pwr < 34674) chn_pwr = 54;
		else if(chn_pwr < 35481) chn_pwr = 55;
		else if(chn_pwr < 36308) chn_pwr = 56;
		else if(chn_pwr < 37154) chn_pwr = 57;
		else if(chn_pwr < 38019) chn_pwr = 58;
		else if(chn_pwr < 38905) chn_pwr = 59;
		else if(chn_pwr < 39811) chn_pwr = 60;
		else if(chn_pwr < 40738) chn_pwr = 61;
		else if(chn_pwr < 41687) chn_pwr = 62;
		else if(chn_pwr < 42658) chn_pwr = 63;
		else if(chn_pwr < 43652) chn_pwr = 64;
		else if(chn_pwr < 44668) chn_pwr = 65;
		else if(chn_pwr < 45709) chn_pwr = 66;
		else if(chn_pwr < 46774) chn_pwr = 67;
		else if(chn_pwr < 47863) chn_pwr = 68;
		else if(chn_pwr < 48978) chn_pwr = 69;
		else if(chn_pwr < 50119) chn_pwr = 70;
		else if(chn_pwr < 52481) chn_pwr = 71;
		else if(chn_pwr < 54954) chn_pwr = 72;
		else if(chn_pwr < 57544) chn_pwr = 73;
		else if(chn_pwr < 60256) chn_pwr = 74;
		else if(chn_pwr < 63096) chn_pwr = 75;
		else if(chn_pwr < 66069) chn_pwr = 76;
		else if(chn_pwr < 69183) chn_pwr = 77;
		else if(chn_pwr < 72444) chn_pwr = 78;
		else if(chn_pwr < 75858) chn_pwr = 79;
		else if(chn_pwr < 79433) chn_pwr = 80;
		else if(chn_pwr < 83176) chn_pwr = 81;
		else if(chn_pwr < 87096) chn_pwr = 82;
		else if(chn_pwr < 91201) chn_pwr = 83;
		else if(chn_pwr < 95499) chn_pwr = 84;
		else if(chn_pwr < 100000) chn_pwr = 85;
		else if(chn_pwr < 104713) chn_pwr = 86;
		else if(chn_pwr < 109648) chn_pwr = 87;
		else if(chn_pwr < 114815) chn_pwr = 88;
		else if(chn_pwr < 120226) chn_pwr = 89;
		else if(chn_pwr < 125893) chn_pwr = 90;
		else if(chn_pwr < 131826) chn_pwr = 91;
		else if(chn_pwr < 138038) chn_pwr = 92;
		else if(chn_pwr < 144544) chn_pwr = 93;
		else if(chn_pwr < 151356) chn_pwr = 94;
		else if(chn_pwr < 158489) chn_pwr = 95;
		else if(chn_pwr < 165959) chn_pwr = 96;
		else if(chn_pwr < 173780) chn_pwr = 97;
		else if(chn_pwr < 181970) chn_pwr = 98;
		else if(chn_pwr < 190546) chn_pwr = 99;
		else chn_pwr = 100;
	}
	else
	{
		chn_pwr = 0;
	}
	*p_quality_percent = (TP_UINT8)chn_pwr;


	return TP_SUCCESS;
}

TP_UINT8 TP_get_signal_quality_DB(TP_UINT8 * p_quality_DB)
{
	TP_UINT8 result;
	TP_UINT8 reg_buffer[5];
	TP_UINT32 beta, total;
	TP_UINT64 chn_pwr, noise_pwr;
	TP_UINT32 temp;
	TP_UINT8 i;
	TP_UINT8 mean_count;


	result = IIC_READ_ONE_BYTE(0x050d, reg_buffer);
	if(result != TP_SUCCESS)
		return result;

	if( (reg_buffer[0] & 0x01) == 0x01)
	{
		
		beta = total = 0;
		result = IIC_READ_ONE_BYTE(0x110d, reg_buffer);
		if(result != TP_SUCCESS)
			return result;
		if(reg_buffer[0] == 0x80)
			mean_count=100;
		else
			mean_count=1;		
		

		for(i=0; i<mean_count; i++) 
		{

			result = TP_iic_read(gTP5001_addr, 0x054d, reg_buffer, 5);
			if(result != TP_SUCCESS)
					return result;
		
			temp = reg_buffer[0] | (reg_buffer[1]<<8) | ((reg_buffer[2] & 0x03)<<16);

			beta += temp;

			temp = reg_buffer[3] | ((reg_buffer[4] & 0x7f)<<8);

			total += temp;
		}

		chn_pwr = beta;
		noise_pwr = total;

		if(noise_pwr == 0)
			chn_pwr = 0;
		else
		{
			//chn_pwr = chn_pwr *10000 / noise_pwr;
			chn_pwr *= 10000;
			do_div(chn_pwr,noise_pwr);
		}
		
		if(chn_pwr < 10000) chn_pwr = 0;
		else if(chn_pwr < 10233) chn_pwr = 1 ;
		else if(chn_pwr < 10471) chn_pwr = 2 ;
		else if(chn_pwr < 10715) chn_pwr = 3 ;
		else if(chn_pwr < 10965) chn_pwr = 4 ;
		else if(chn_pwr < 11220) chn_pwr = 5 ;
		else if(chn_pwr < 11482) chn_pwr = 6 ;
		else if(chn_pwr < 11749) chn_pwr = 7 ;
		else if(chn_pwr < 12023) chn_pwr = 8 ;
		else if(chn_pwr < 12303) chn_pwr = 9 ;
		else if(chn_pwr < 12589) chn_pwr = 10;
		else if(chn_pwr < 12882) chn_pwr = 11;
		else if(chn_pwr < 13183) chn_pwr = 12;
		else if(chn_pwr < 13490) chn_pwr = 13;
		else if(chn_pwr < 13804) chn_pwr = 14;
		else if(chn_pwr < 14125) chn_pwr = 15;
		else if(chn_pwr < 14454) chn_pwr = 16;
		else if(chn_pwr < 14791) chn_pwr = 17;
		else if(chn_pwr < 15136) chn_pwr = 18;
		else if(chn_pwr < 15488) chn_pwr = 19;
		else if(chn_pwr < 15849) chn_pwr = 20;
		else if(chn_pwr < 16218) chn_pwr = 21;
		else if(chn_pwr < 16596) chn_pwr = 22;
		else if(chn_pwr < 16982) chn_pwr = 23;
		else if(chn_pwr < 17378) chn_pwr = 24;
		else if(chn_pwr < 17783) chn_pwr = 25;
		else if(chn_pwr < 18197) chn_pwr = 26;
		else if(chn_pwr < 18621) chn_pwr = 27;
		else if(chn_pwr < 19055) chn_pwr = 28;
		else if(chn_pwr < 19498) chn_pwr = 29;
		else if(chn_pwr < 19953) chn_pwr = 30;
		else if(chn_pwr < 20417) chn_pwr = 31;
		else if(chn_pwr < 20893) chn_pwr = 32;
		else if(chn_pwr < 21380) chn_pwr = 33;
		else if(chn_pwr < 21878) chn_pwr = 34;
		else if(chn_pwr < 22387) chn_pwr = 35;
		else if(chn_pwr < 22909) chn_pwr = 36;
		else if(chn_pwr < 23442) chn_pwr = 37;
		else if(chn_pwr < 23988) chn_pwr = 38;
		else if(chn_pwr < 24547) chn_pwr = 39;
		else if(chn_pwr < 25119) chn_pwr = 40;
		else if(chn_pwr < 25704) chn_pwr = 41;
		else if(chn_pwr < 26303) chn_pwr = 42;
		else if(chn_pwr < 26915) chn_pwr = 43;
		else if(chn_pwr < 27542) chn_pwr = 44;
		else if(chn_pwr < 28184) chn_pwr = 45;
		else if(chn_pwr < 28840) chn_pwr = 46;
		else if(chn_pwr < 29512) chn_pwr = 47;
		else if(chn_pwr < 30200) chn_pwr = 48;
		else if(chn_pwr < 30903) chn_pwr = 49;
		else if(chn_pwr < 31623) chn_pwr = 50;
		else if(chn_pwr < 32359) chn_pwr = 51;
		else if(chn_pwr < 33113) chn_pwr = 52;
		else if(chn_pwr < 33884) chn_pwr = 53;
		else if(chn_pwr < 34674) chn_pwr = 54;
		else if(chn_pwr < 35481) chn_pwr = 55;
		else if(chn_pwr < 36308) chn_pwr = 56;
		else if(chn_pwr < 37154) chn_pwr = 57;
		else if(chn_pwr < 38019) chn_pwr = 58;
		else if(chn_pwr < 38905) chn_pwr = 59;
		else if(chn_pwr < 39811) chn_pwr = 60;
		else if(chn_pwr < 40738) chn_pwr = 61;
		else if(chn_pwr < 41687) chn_pwr = 62;
		else if(chn_pwr < 42658) chn_pwr = 63;
		else if(chn_pwr < 43652) chn_pwr = 64;
		else if(chn_pwr < 44668) chn_pwr = 65;
		else if(chn_pwr < 45709) chn_pwr = 66;
		else if(chn_pwr < 46774) chn_pwr = 67;
		else if(chn_pwr < 47863) chn_pwr = 68;
		else if(chn_pwr < 48978) chn_pwr = 69;
		else if(chn_pwr < 50119) chn_pwr = 70;
		else if(chn_pwr < 52481) chn_pwr = 72;
		else if(chn_pwr < 54954) chn_pwr = 74;
		else if(chn_pwr < 57544) chn_pwr = 76;
		else if(chn_pwr < 60256) chn_pwr = 78;
		else if(chn_pwr < 63096) chn_pwr = 80;
		else if(chn_pwr < 66069) chn_pwr = 82;
		else if(chn_pwr < 69183) chn_pwr = 84;
		else if(chn_pwr < 72444) chn_pwr = 86;
		else if(chn_pwr < 75858) chn_pwr = 88;
		else if(chn_pwr < 79433) chn_pwr = 90;
		else if(chn_pwr < 83176) chn_pwr = 92;
		else if(chn_pwr < 87096) chn_pwr = 94;
		else if(chn_pwr < 91201) chn_pwr = 96;
		else if(chn_pwr < 95499) chn_pwr = 98;
		else if(chn_pwr < 100000) chn_pwr = 100;
		else if(chn_pwr < 104713) chn_pwr = 102;
		else if(chn_pwr < 109648) chn_pwr = 104;
		else if(chn_pwr < 114815) chn_pwr = 106;
		else if(chn_pwr < 120226) chn_pwr = 108;
		else if(chn_pwr < 125893) chn_pwr = 110;
		else if(chn_pwr < 131826) chn_pwr = 112;
		else if(chn_pwr < 138038) chn_pwr = 114;
		else if(chn_pwr < 144544) chn_pwr = 116;
		else if(chn_pwr < 151356) chn_pwr = 118;
		else if(chn_pwr < 158489) chn_pwr = 120;
		else if(chn_pwr < 165959) chn_pwr = 122;
		else if(chn_pwr < 173780) chn_pwr = 124;
		else if(chn_pwr < 181970) chn_pwr = 126;
		else if(chn_pwr < 190546) chn_pwr = 128;
		else chn_pwr = 130;
	}
	else
	{
		chn_pwr = 0;
	}
	*p_quality_DB = chn_pwr;
	
	return TP_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_signal_strength
// Description: TP?\u0178Ƭ???õ\u0153?ź?ǿ??
// Output:
//		p_signal_strength:  ?ź?ǿ??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_signal_strength(TP_UINT8 * p_signal_strength)
{
	TP_UINT8 register_value[2];
	TP_UINT8 result;
	TP_INT32 sum=0;

	if(gui_tuner_type == RDA_5812_TUNER)
	{
		TP_UINT8 A,B,C,D,E,P1,P2;
		TP_UINT16 P;
		TP_INT32  temp=0,Plog=0;
		TP_UINT8 reg_addr,device_addr = gTP5001_tuner_addr;
		TP_INT32  err=0;

		TP_INT32 Gain_lna[]={-73,-15,36,72,110,160,200};
		TP_INT32 Gain_i2v[]={0,67,136,210};
		TP_INT32 Gain_filter[]={-60,-2,56,112};

		reg_addr =0x91;
		result = TP_iic_tuner_read( device_addr,reg_addr, register_value, 1);
		if(result != TP_SUCCESS)
				return result;
		A = (register_value[0]>>6) & 0x03;

		reg_addr =0x05;		
		result = TP_iic_tuner_read( device_addr,reg_addr, register_value, 2);
		if(result != TP_SUCCESS)
				return result;
		B = (register_value[0]>>4) & 0x03;
		C = (register_value[0]>>2) & 0x03;
		D = register_value[0] & 0x03;
		E = register_value[1];

		result = TP_iic_read(gTP5001_addr, 0x0D0C, register_value, 2);
		if(result != TP_SUCCESS)
				return result;	
		P1 = register_value[0];
		P2 = register_value[1] & 0x01;
		P = P1 + P2*256;
		
		err = (P -128);
		if(err<0)
			Plog = err/16;
		else
			Plog = err/8;
		
		temp = -(14 + Gain_lna[A+B]/10 + Gain_i2v[C]/10 + Gain_filter[D]/10 + 2 * E/10) + Plog;	
		temp = (temp + 95) * 2; 
		sum = temp;

		if(sum<=0)
			sum =0;
		if(sum>100)
			sum = 100;

		*p_signal_strength = (TP_INT8)sum;
	}
	else if(gui_tuner_type == RDA_5815_TUNER)
	{
		TP_INT32 temp=0,temp1=0;
		TP_UINT8 P1,P2;
		TP_UINT16 P;
		TP_INT32  err=0,Plog=0;
		result = TP_iic_read(gTP5001_addr, 0x0D0E, register_value, 1);
		if(result != TP_SUCCESS)
			return result;

		temp = register_value[0];

		result = TP_iic_read(gTP5001_addr, 0x0D0C, register_value, 2);
		if(result != TP_SUCCESS)
				return result;	
		P1 = register_value[0];
		P2 = register_value[1] & 0x01;
		P = P1 + P2*256;
		
		err = (P -128);
		if(err<0)
			Plog = err/16;
		else
			Plog = err/8;

		temp1 = 3672;			
		temp1 = temp1*temp/10000-89;
		
		temp1 = temp1 + Plog;
		temp1 = (temp1 + 95) * 2; 
		sum = temp1;

		if(sum<=0)
			sum =0;
		if(sum>100)
			sum = 100;

		*p_signal_strength = (TP_INT8)sum;
	}
	else if(gui_tuner_type == AV2020_TUNER)
	{
		TP_INT32 temp=0,temp1=0;
		TP_UINT8 P1,P2;
		TP_UINT16 P;
		TP_INT32  err=0,Plog=0;

		result = TP_iic_read(gTP5001_addr, 0x0D0E, register_value, 1);
		if(result != TP_SUCCESS)
			return result;
		temp = register_value[0];

		result = TP_iic_read(gTP5001_addr, 0x0D0C, register_value, 2);
		if(result != TP_SUCCESS)
				return result;	
		P1 = register_value[0];
		P2 = register_value[1] & 0x01;
		P = P1 + P2*256;
		
		err = (P -128);
		if(err<0)
			Plog = err/16;
		else
			Plog = err/8;


		temp1 = 49;
		temp1 = temp1*temp/100-128;
		
		temp1 = temp1 + Plog;
		temp1 = (temp1 + 95) * 2; 
		sum = temp1;

		if(sum<=0)
			sum =0;
		if(sum>100)
			sum = 100;

		*p_signal_strength = (TP_INT8)sum;
	}
	else  
	{
		TP_INT32 temp=0,temp1=0;
		TP_UINT8 P1,P2;
		TP_UINT16 P;
		TP_INT32  err=0,Plog=0;

		result = TP_iic_read(gTP5001_addr, 0x0D0E, register_value, 1);
		if(result != TP_SUCCESS)
			return result;
		temp = register_value[0];

		result = TP_iic_read(gTP5001_addr, 0x0D0C, register_value, 2);
		if(result != TP_SUCCESS)
				return result;	
		P1 = register_value[0];
		P2 = register_value[1] & 0x01;
		P = P1 + P2*256;
		
		err = P -128;
		if(err<0)
			Plog = err/16;
			//Plog = err >> 4;
		else
			Plog = err/8;
			//Plog = err >> 3;

		if(temp<105)
		{
			//temp1 = (3.0/20.0);
			temp1 = 15*temp/100-90;
		}
		else
		{
			//temp1 = (3.0/7.0);
			temp1 = 43*temp/100-120;
		}
		temp1 = temp1 + Plog;
		temp1 = (temp1 + 95) * 2; 
		sum = (TP_INT32)temp1;

		if(sum<=0)
			sum =0;
		if(sum>100)
			sum = 100;

		*p_signal_strength = (TP_INT8)sum;
	}

	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_freq_offset
// Description: TP?\u0178Ƭ???õ\u0153Ƶƫֵ
// Output:
//		p_freq_offset: Ƶ??ƫ??ֵ????λ(KHz)
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_freq_offset(TP_INT32 * p_freq_offset)
{
	TP_UINT8 result;
	TP_UINT8 reg_value[3];
	TP_UINT64 temp;

	TP_INT32 Temp2;
	
	result = TP_iic_read(gTP5001_addr, 0x0128, reg_value, 3);
	if(result != TP_SUCCESS)
			return result;
	
	Temp2 = (reg_value[0]<<16) | (reg_value[1]<<8) |  reg_value[2];
	if(Temp2>(1<<23)-1)
		Temp2 = Temp2 - (1<<24);
	temp = Temp2;
	
	temp *= g_symbol_rate;

	//temp /= (1<<24);
	//temp = temp/1000;  //output KHz

	do_div(temp,(1<<24));
	do_div(temp,1000);

	temp = -temp;

	*p_freq_offset = temp;

	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_lock_status
// Description: TP?\u0178Ƭ???õ\u0153???\u0161?\u017d̬
// Return:
//		TP_SUCCESS: ???\u0161
//		TP_NOT_LOCK: û?????\u0161
//
TP_UINT8 TP_get_lock_status(void)
{
	TP_UINT8 register_value;
	TP_UINT8 result;

	result = IIC_READ_ONE_BYTE(0x110d, &register_value);
	if(result != TP_SUCCESS)
		return result;
	if((register_value &0x80 )== 0x80)
	{
	//	TP_get_freq_offset(&TP_offset);
	//	if((TP_offset >= -5000) && (TP_offset <= 5000))
			return TP_SUCCESS;
	//	else
	//		return TP_NOT_LOCK;
	}
	else
		return TP_NOT_LOCK;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_rf_pola
// Description: TP?\u0178Ƭ??????\u0152???
// Input:
//		Pola: \u0152??ԣ?Normal: ????(VH_SEL??\u0153?Ϊ??)??Invert: ?\u017d??(VH_SEL??\u0153?Ϊ\u017e?)
// Return:
//		TP_SUCCESS: ???\u0161
//
TP_UINT8 TP_set_rf_pola( TP_RFAGCPola Pola) 
{
	TP_UINT8 result;
	TP_UINT8 register_value;
	
	if(Pola == RA_Normal)
	{
		register_value = (1<<6);
		result = IIC_WRITE_ONE_BYTE(0x3E00, register_value);
		if(result != TP_SUCCESS)
			return result;
	}
	else if(Pola == RA_Invert)
	{			
		register_value = 0;	
		result = IIC_WRITE_ONE_BYTE(0x3E00, register_value);
		if(result != TP_SUCCESS)
			return result;	
	}
	else
		return TP_PARA_ERR;
	
	return TP_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_rf_tuner
// Description: TP?\u0178Ƭ??????Tuner
// Input:
//		frequency:  Ƶ??, xx(MHz)
//		Symbol_Rate_Value:	?????? xx(Hz)
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//		TP_SET_TUNER_ERR: ????????????ȷ
//
#if 0
TP_UINT8 TP_set_rf_tuner(TP_UINT32 frequency, TP_UINT32 Symbol_Rate_Value)
{
	TP_UINT8 result;

#if 0
	if (gui_tuner_type == RDA_5812_TUNER)
	{
		if(gTuner_initialized == 0)
		{
			gTuner_initialized = 1;

			result = rda_5812_init();
			if(result != TP_SUCCESS)
				return result;
		}
		
		return rda_5812_set_frequency(frequency, Symbol_Rate_Value);
	}

	if (gui_tuner_type == RDA_5815_TUNER)
	{
		if(gTuner_initialized == 0)
		{
			gTuner_initialized = 1;

			result = rda_5815_init();
			if(result != TP_SUCCESS)
				return result;
		}
		
		return rda_5815_set_frequency(frequency, Symbol_Rate_Value/1000);
	}
#endif
	if (gui_tuner_type == RDA_5815M_TUNER)
	{
		if(gTuner_initialized == 0)
		{
			gTuner_initialized = 1;

			result = rda_5815M_init();
			if(result != TP_SUCCESS)
				return result;
		}

		return rda_5815M_set_frequency(frequency, Symbol_Rate_Value/1000);
	}
#if 0
	if (gui_tuner_type == AV2020_TUNER)
	{
		if(gTuner_initialized == 0)
		{
			gTuner_initialized = 1;

			result = av2020_init();
			if(result != TP_SUCCESS)
				return result;
		}
		
		return Tuner_control(frequency, Symbol_Rate_Value);
	}
#endif

	/*if (gui_tuner_type == S305_TUNER)
	{	
	return S305_set_frequency(frequency, Symbol_Rate_Value);
	}*/

	return TP_TUNER_ID_ERR;
}
#endif

// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_iq_switch
// Description: TP?\u0178Ƭ??????IQ?\u017d??
// Input:
//		iq_pola: IQ??\u0152???   Normal:????   Invert: ?\u017d??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_iq_switch(TP_IQPola iq_pola)
{
	TP_UINT8 ret;

	if(iq_pola == Normal)
	{
#ifdef FPGA_PLATFORM
		ret = IIC_WRITE_ONE_BYTE(0x0000, 0x01);
#else
		ret = IIC_WRITE_ONE_BYTE(0x0000, 0x00);
#endif 
		if(ret != TP_SUCCESS)
			return ret;
	}
	else if(iq_pola == Invert)
	{
#ifdef FPGA_PLATFORM
		ret = IIC_WRITE_ONE_BYTE(0x0000, 0x03);
#else
		ret = IIC_WRITE_ONE_BYTE(0x0000, 0x02);
#endif 						
		if(ret != TP_SUCCESS)
			return ret;
	}
	else
	{
		return TP_PARA_ERR;
	}

	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_ts_interface
// Description: TP?\u0178Ƭ??????AGC????
// Input:
//		active_edge: positive_edge, ??????     negitive_edge, ??\u0153???
//		port_type: series_port, \u017d???      para_port, ????
//		clock_div: SPIʱ?ӷ?Ƶ????,    SPI_CLK = MAIN_CLK / (clock_div)     MAIN_CLK:  4M:380   10M:380   20M:380   27M:378
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_ts_interface(TP_SPI_edge active_edge, TP_SPI_port_type port_type, TP_UINT8 clock_div)
{
	TP_UINT8 reg_value;
	TP_UINT8 ret;

	reg_value = 0x0;
	
	// ??\u017e?MPEG_CLK_SEL?ź??????ػ?????\u0153???
	ret = IIC_READ_ONE_BYTE(0x3f00, &reg_value);   
	if(ret != TP_SUCCESS)
		return ret;
	reg_value = (reg_value&0xef) | (active_edge<<4);

	ret = IIC_WRITE_ONE_BYTE(0x3f00, reg_value);   
	if(ret != TP_SUCCESS)
		return ret;

	//SSI MODE 
	ret = IIC_READ_ONE_BYTE(0x0C01, &reg_value);   
	if(ret != TP_SUCCESS)
		return ret;

	if(port_type == out_disable)
	{
		reg_value |= 0x04;		// set register bit 2 = 0   output enable
		ret = IIC_WRITE_ONE_BYTE(0x0C01, reg_value);
		if(ret != TP_SUCCESS)
			return ret;
	}
	else
	{
		reg_value = (reg_value & 0xfa) | port_type;		// set register bit 2 = 0   output enable
		ret = IIC_WRITE_ONE_BYTE(0x0C01, reg_value);
		if(ret != TP_SUCCESS)
			return ret;
	}
	// set clk div
	if(clock_div != 0)
	{
		reg_value = (clock_div) & 0x7f ;
		ret = IIC_WRITE_ONE_BYTE(0x3F0B, reg_value);
		if(ret != TP_SUCCESS)
			return ret;
	}

	return TP_SUCCESS;
}

TP_UINT8 TP_Standby(void)
{
		TP_UINT8 reg_value;
		int ret;

		ret = IIC_READ_ONE_BYTE(0x0C01, &reg_value);
		if(ret != TP_SUCCESS)
			return ret;
		reg_value |= 0x04;		// set register bit 2 = 0   output enable
		ret = IIC_WRITE_ONE_BYTE(0x0C01, reg_value);
		if(ret != TP_SUCCESS)
			return ret;

		return TP_SUCCESS;
}

TP_UINT8 TP_Resume(void)
{
		TP_UINT8 reg_value;
		int ret;

		ret = IIC_READ_ONE_BYTE(0x0C01, &reg_value);
		if(ret != TP_SUCCESS)
			return ret;
		reg_value &= 0xFB;		// set register bit 2 = 0   output enable
		ret = IIC_WRITE_ONE_BYTE(0x0C01, reg_value);
		if(ret != TP_SUCCESS)
			return ret;

		return TP_SUCCESS;
}


// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_sleep
// Description: TP?\u0178Ƭ\u0153???˯??ģ?\u0153
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_sleep()
{
	//TP_Reset();
	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_soft_reset
// Description: TP?\u0178Ƭ??\u017e\u017dλ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_soft_reset()
{
	TP_UINT8 register_value;
	TP_UINT8 result;

	result = IIC_READ_ONE_BYTE(0x3F00, &register_value);
	if(result != TP_SUCCESS)
		return result;
	register_value &= ~0x80;
	result = IIC_WRITE_ONE_BYTE(0x3F00, register_value);
	if(result != TP_SUCCESS)
		return result;

	TP_Delay(1);

	return TP_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------
//
// Function Name: TP_SendDiseqcCommand
// Description: TP Diseqc interface
// Paramenter:
//			length: input, diseqc command length
//          pCommands: input, diseqc command buffer          pReturnLength: Input/Output command return length
//          pReturnBuffer: return buffer
// Return:
//		TP_SUCCESS: OK

TP_UINT8 TP_Diseqc_close(void)
{
	TP_UINT8 result, temp;
//close diseqc module clk
	result = IIC_READ_ONE_BYTE(0x3F0A, &temp);
	if(result != TP_SUCCESS)
		return result;

	result = IIC_WRITE_ONE_BYTE(0x3F0A, temp & ~0x10);
	return result;
}
TP_UINT8 TP_Diseqc_Init(int sys_clk)
{
	TP_UINT8 result, temp;
	TP_UINT8 clk_div_pause[3];
	int div;

//open diseqc module clk
	result = IIC_READ_ONE_BYTE(0x3F0A, &temp);
	if(result != TP_SUCCESS)
		return result;
	if ((temp & 0x10) == 0 )
	{
		result = IIC_WRITE_ONE_BYTE(0x3F0A, temp | 0x10);
		if(result != TP_SUCCESS)
			return result;
	}
//diseqc init, cfg 22k clk, pause time
	result = IIC_WRITE_ONE_BYTE(0x3E00, 0x80);	//reset diseqc module except regs
	if(result != TP_SUCCESS)
		return result;

	div = sys_clk / 22000;
	clk_div_pause[0] = (TP_UINT8)div;
	clk_div_pause[1] = (TP_UINT8)(div>>8);
	clk_div_pause[2] = 1;
	result = TP_iic_write(gTP5001_addr, 0x3E02, clk_div_pause, 3);
	return result;
}


TP_UINT8 TP_SendDiseqcCommand(TP_UINT8  length, 
                                 TP_UINT8* pCommands, 
                                 TP_UINT8* pReturnLength, 
                                 TP_UINT8* pReturnBuffer)
{
	TP_UINT8 result, i;
	TP_UINT8 tx_buf[4];
	TP_UINT8 rx_buf[4];

	// check parameter
	if((length > 8) || (length<1))
		return TP_PARA_ERR;
	if(*pReturnLength > 16)
		return TP_PARA_ERR;

	//open clk, reset diseqc, set 22k divider pause time
	result = TP_Diseqc_Init(100000000);
	if(result != TP_SUCCESS)
		return result;
	//because of use loop test mode, rx -- tx, so must firstly enbale rx
	//write rx ctrl
	result = IIC_WRITE_ONE_BYTE(0x3E10, 0x1);
	if(result != TP_SUCCESS)
		return result;

	//write tx data
	result = TP_iic_write(gTP5001_addr, 0x3E08, pCommands, length);
	if(result != TP_SUCCESS)
		return result;

	tx_buf[0] = (1<<6) | (1<<2);	//write tx ctrl
	tx_buf[1] = length-1;	//write tx cnt, 0=send 1 byte, 7=send 8 bytes data
	result = TP_iic_write(gTP5001_addr, 0x3E00, tx_buf, 2);
	if(result != TP_SUCCESS)
		return result;
	
	//read rx cnt, untill this reg value is equ to (tx_cnt+1)
	while(1)
	{
		result = IIC_READ_ONE_BYTE(0x3E11, rx_buf);
		if(result != TP_SUCCESS)
			return result;
		if (rx_buf[0] == *pReturnLength)
			break;
	}
	//read data parity result
	result = IIC_READ_ONE_BYTE(0x3E12, rx_buf+1);
	if(result != TP_SUCCESS)
		return result;
	result = IIC_READ_ONE_BYTE(0x3E13, rx_buf+2);
	if(result != TP_SUCCESS)
		return result;

	//read rx data
	for(i=0; i<*pReturnLength; i++, pReturnBuffer++)
	{
		result = IIC_READ_ONE_BYTE(0x3E14, pReturnBuffer);
		if(result != TP_SUCCESS)
			return result;
	}
	//close module clk
	result = TP_Diseqc_close();
	return result;
}

#if 0
TP_UINT8 TP_tuner_dynamic_detect(void)
{
	TP_UINT8 reg_data[2];
	TP_UINT8 result = 0;
	TP_UINT8 found = 0;

	gui_tuner_type = 0;
  
  //RDA5812 RDA5815 addr = 0x0C
	
	if (found == 0)
	{
		result = IIC_WRITE_ONE_BYTE(0xFE01, RDA5812_DEV_ADDR<<1);
		if(result == TP_SUCCESS)
		{
			result = TP_iic_tuner_read(RDA5812_DEV_ADDR, 0x01, reg_data, 1);
            printk("---->[%s,%d] result:%d,rda reg1 data:0x%x\n",__func__,__LINE__,result,reg_data[0]);
			if(result == TP_SUCCESS)
			{
				if((reg_data[0] & 0xF0) == 0xC0)
				{ 
					gui_tuner_type = RDA_5812_TUNER;
					found = 1;
				}
				else if((reg_data[0] & 0xFF) == 0xF8)
				{
					gui_tuner_type = RDA_5815M_TUNER;
					found = 1;
				}
				else if((reg_data[0] & 0xF0) == 0xF0)
				{
					gui_tuner_type = RDA_5815_TUNER;
					found = 1;
				}
			}					
		}
	}

	//AV2020  addr = 0x63
	if (found == 0)
	{
		AV2020_DEV_ADDR = 0x63;
		result = IIC_WRITE_ONE_BYTE(0xFE01, AV2020_DEV_ADDR<<1);
		if(result == TP_SUCCESS)
		{
			result = TP_iic_tuner_read(AV2020_DEV_ADDR, 0x00, reg_data, 1);
			if(result == TP_SUCCESS)
			{		
				gui_tuner_type = AV2020_TUNER ;					
				found = 1;	
			}
		}
		if(found == 0 )
		{
			AV2020_DEV_ADDR = 0x62;
			result = IIC_WRITE_ONE_BYTE(0xFE01, AV2020_DEV_ADDR<<1);
			if(result == TP_SUCCESS)
			{
				result = TP_iic_tuner_read(AV2020_DEV_ADDR, 0x00, reg_data, 1);
				if(result == TP_SUCCESS)
				{		
					gui_tuner_type = AV2020_TUNER ;					
					found = 1;	
				}
			}
		}
	}

	// S305 addr = 0x61
	// S305 addr = 0x61 mtv600
/*	if(found == 0)
	{
		result = IIC_WRITE_ONE_BYTE(0xFE01, S305_DEV_ADDR<<1);
		if(result == TP_SUCCESS)
		{
			reg_data[0] = 0x08;     //for MTV600_TUNER this reg is read only
			reg_data[1] = 0x4E;
			result = TP_iic_tuner_write(S305_DEV_ADDR, reg_data, 2);
			if(result == TP_SUCCESS)
			{
				result = TP_iic_tuner_read(S305_DEV_ADDR, 0x00, reg_data, 2);
				if(result == TP_SUCCESS)
				{
					if(reg_data[1] == 0x4E)
					{
						gui_tuner_type = S305_TUNER;
						found = 1;
					}
				}
			}
		}
	}	
*/
	if(found == 0)
		return TP_TUNER_ID_ERR;
	else
		return TP_SUCCESS;

}
#endif


// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_init
// Description: TP?\u0178Ƭ?????\u0152??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_init(TP_UINT8 tuner_type)
{
	TP_UINT8 result;
	TP_UINT8 reg_value[6];

	// ???\u0152??ȫ\u0178ֱ?
	g_symbol_rate = 0;	
	
	result = system_init();
	if(result != TP_SUCCESS)
		return result;

    gui_tuner_type = tuner_type;
	#if 0
	result = TP_tuner_dynamic_detect();
	if(result != TP_SUCCESS)
		return result;
       
	if(gui_tuner_type == 0)
		return TP_TUNER_ID_ERR;
    #endif
   
	result = IIC_WRITE_ONE_BYTE(0xFE01, gTP5001_tuner_addr);
	// ????IQ
	if(gui_tuner_type == SHARP_6306_TUNER)
		result = TP_set_iq_switch(Normal);
	else if(gui_tuner_type == SHARP_6903_TUNER)
		result = TP_set_iq_switch(Normal);
	else if(gui_tuner_type == RDA_5812_TUNER)
		result = TP_set_iq_switch(Invert);	
	else if(gui_tuner_type == RDA_5815_TUNER)
		result = TP_set_iq_switch(Invert);	
	else if(gui_tuner_type == S305_TUNER)
		result = TP_set_iq_switch(Invert);	
	else if(gui_tuner_type == AV2020_TUNER)
		result = TP_set_iq_switch(Normal);	
	else if(gui_tuner_type == RDA_5815M_TUNER)
		result = TP_set_iq_switch(Invert);	
	else
		result = TP_set_iq_switch(Normal);
	if(result != TP_SUCCESS)
		return result;
	
	result = IIC_WRITE_ONE_BYTE(0x0500,0x01);
	if(result != TP_SUCCESS)
		return result;
 
	reg_value[0] = 0x22;
	reg_value[1] = 0x04;
	reg_value[2] = 0x04;
	reg_value[3] = 0x03;
	result = TP_iic_write(gTP5001_addr, 0x1103, reg_value, 4);
	if(result != TP_SUCCESS)
		return result;

	reg_value[0] = 0x20;
	reg_value[1] = 0x00;
	reg_value[2] = 0x05;
	reg_value[3] = 0x00;
	result = TP_iic_write(gTP5001_addr, 0x0305, reg_value, 4);
	if(result != TP_SUCCESS)
		return result;

#ifdef FPGA_PLATFORM
	result = IIC_WRITE_ONE_BYTE(0x0d0b,0x7f);
	if(result != TP_SUCCESS)
		return result;
#endif
	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_deinit
// Description: TP?\u0178Ƭ???\u017d???\u0152??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_deinit()
{
	return TP_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_statistic_ber_bler
// Description: GK5109S?\u0178Ƭ???õ\u0153BER??BLERͳ\u0152?ֵ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
TP_UINT8 TP_get_statistic_ber_bler(TP_UINT64 *pBer_total, TP_UINT64 *pBer_error, TP_UINT64 *pBler_total, TP_UINT64 *pBler_error)
{
	TP_UINT8 result;
	TP_UINT64 tempValue;
	TP_UINT8 temp[16];
	TP_UINT32 i;

//	if(g_chip_type == CHIP_TYPE_GK5109S)
//	{
		result = TP_iic_read(gTP5001_addr, 0x0B00, temp, 5);
		if(result != TP_SUCCESS)
			return result;
		tempValue = 0;
		for(i=0; i<5; i++)
		{
			tempValue |= temp[4-i];
			tempValue <<= 8;
		}
		tempValue >>= 8;
		*pBer_total = tempValue;

		result = TP_iic_read(gTP5001_addr, 0x0B05, temp, 5);
		if(result != TP_SUCCESS)
			return result;
		tempValue = 0;
		for(i=0; i<5; i++)
		{
			tempValue |= temp[4-i];
			tempValue <<= 8;
		}
		tempValue >>= 8;
		*pBer_error = tempValue;

		result = TP_iic_read(gTP5001_addr, 0x0A11, temp, 4);
		if(result != TP_SUCCESS)
			return result;
		tempValue = 0;
		for(i=0; i<4; i++)
		{
			tempValue |= temp[3-i];
			tempValue <<= 8;
		}
		tempValue >>= 8;
		*pBler_total = tempValue;

		result = TP_iic_read(gTP5001_addr, 0x0A15, temp,4);
		if(result != TP_SUCCESS)
			return result;
		tempValue = 0;
		for(i=0; i<4; i++)
		{
			tempValue |= temp[3-i];
			tempValue <<= 8;
		}
		tempValue >>= 8;
		*pBler_error = tempValue;
//	}

	return TP_SUCCESS;
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
// Function Name: TP_clear_statistic_ber_bler
// Description: GK5109S?\u0178Ƭ??????BER??BLERͳ\u0152?ֵ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160

TP_UINT8 TP_clear_statistic_ber_bler(TP_UINT8 type)
{
	TP_UINT8 temp[2];
	TP_UINT8 result;
	
	temp[0] = 1;
	if(type & 0x01)
	{
		result = TP_iic_write(gTP5001_addr, 0x0B0A, temp, 1);
		if(result != TP_SUCCESS)
			return result;
	}
	temp[0] = 5;
	if(type & 0x02)	
	{
		result = TP_iic_write(gTP5001_addr, 0x0A10, temp, 1);
		if(result != TP_SUCCESS)
			return result;
	}

	return TP_SUCCESS;
}	
// ---------------------------------------------------------------------------------------------------------


#endif //_USE_TP5001_CHIP_

