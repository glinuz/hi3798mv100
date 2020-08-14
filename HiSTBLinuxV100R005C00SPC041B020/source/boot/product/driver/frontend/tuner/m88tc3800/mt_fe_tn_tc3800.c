/***************************************************************************/
/* MONTAGE PROPRIETARY AND CONFIDENTIAL                                    */
/* Montage Technology (Shanghai) Co., Ltd                                  */
/* Copyright (C) 2015                                                      */
/* Montage Technology Group Limited and/or its affiliated companies        */
/* All Rights Reserved                                                     */
/***************************************************************************/
/*
 * Filename:        mt_fe_tn_tc3800.c
 *
 * Description:     Montage M88TC3800 Digital Tuner IC driver.
 *
 *
 * Author:          Lei.Zhu
 *
 * Version:         0.01.11 beta
 * Date:            2015-03-04

 * History:
 * Description      Version     Date            Author
 *---------------------------------------------------------------------------
 * File Create      0.00.00     2013.08.20      Lei.Zhu
 * Modify           0.01.00     2013.11.06      YZ.Huang
 * Modify           0.01.01     2013.12.23      Roy.Xu
 * Modify           0.01.02     2014.01.09      Roy.Xu
 * Modify           0.01.03     2014.02.28      Lei.Zhu
 * Modify           0.01.04     2014.05.08      Lei.Zhu
 * Modify           0.01.05     2014.07.15      Lei.Zhu
 * Modify           0.01.06     2014.08.07      Lei.Zhu
 * Modify           0.01.07     2014.11.06      Lei.Zhu
 * Modify           0.01.08     2014.12.11      Lei.Zhu
 * Modify           0.01.09     2014.12.17      Lei.Zhu
 *---------------------------------------------------------------------------
 ***************************************************************************/
//#include <linux/delay.h>
//#include <linux/semaphore.h>

//#include "linux/kernel.h"

//#include <hi_debug.h>
#include "hi_type.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"

#include "mt_fe_tn_tc3800.h"

#ifndef HI_MINIBOOT_SUPPORT
#include <common.h>
#endif



HI_U32 g_m88tc3800_tuner_i2c_channel = 0;
HI_U32 g_m88tc3800_tuner_port = 0;
HI_U32 g_m88tc3800_initflag = HI_FALSE;

MT_FE_TN_DEVICE_SETTINGS_TC3800 handle;

S32 _mt_fe_tn_get_reg_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 *reg_data)
{
	HI_S32 i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
    HI_U8 au8RcvData[65] = { 0 };
	HI_S32 datalen = 1;

	qam_config_i2c_out (g_m88tc3800_tuner_port, 1);

	stI2cDataStr.pu8ReceiveBuf = au8RcvData;
	stI2cDataStr.pu8SendBuf = &reg_addr;
	stI2cDataStr.u32ReceiveLength = datalen;
	stI2cDataStr.u32SendLength = 1;

	s32Ret = tuner_i2c_receive_data (g_stTunerOps[g_m88tc3800_tuner_port].enI2cChannel, g_stTunerOps[g_m88tc3800_tuner_port].u32TunerAddress/*|0x01 */ ,
		&stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[m88tc3800 error]%s %d IIC read err! status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	for (i = 0; i < datalen; i++)
	{
		reg_data[i] = au8RcvData[i];
	}

	return HI_SUCCESS;
}


S32 _mt_fe_tn_set_reg_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 reg_data)
{
	//HI_U8  i;
	HI_S32 s32Ret;
	TUNER_I2C_DATA_S stI2cDataStr = { 0 };
    HI_U8 au8SendData[65] = { 0 };

	qam_config_i2c_out (g_m88tc3800_tuner_port, 1);

    au8SendData[0] = reg_addr;
	au8SendData[1] = reg_data;

	stI2cDataStr.pu8ReceiveBuf = 0;
	stI2cDataStr.u32ReceiveLength = 0;
	stI2cDataStr.pu8SendBuf = au8SendData;
	stI2cDataStr.u32SendLength = 2;	//subaddress + data
	s32Ret = tuner_i2c_send_data (g_stTunerOps[g_m88tc3800_tuner_port].enI2cChannel,g_stTunerOps[g_m88tc3800_tuner_port].u32TunerAddress, &stI2cDataStr);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_TUNER("[m88tc3800 error]%s %d IIC write err!status:0x%x\n", __FUNCTION__, __LINE__, s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;

}


S32 _mt_fe_tn_set_reg_bit_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 reg_addr, U8 data, U8 high_bit, U8 low_bit)
{
	U8 tmp = 0, value = 0;
	
	if(high_bit < low_bit)
	{
		return -1;
	}
	
	data <<= (7 + low_bit - high_bit);
	data &= 0xFF;
	data >>= (7 - high_bit);
	data &= 0xFF;
	
	tmp = 0xFF;
	tmp <<= (7 + low_bit - high_bit);
	tmp &= 0xFF;
	tmp >>= (7 - high_bit);
	tmp &= 0xFF;
	
	_mt_fe_tn_get_reg_tc3800(handle, reg_addr, &value);
	value &= ~tmp;
	value |= data;
	_mt_fe_tn_set_reg_tc3800(handle, reg_addr, value);

	return HI_SUCCESS;
}

void _mt_sleep(U32 ticks_ms)
{
	msleep_interruptible(ticks_ms);
}



/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_RF_front_tc3800								 */
/*****************************************************************************/
void _mt_fe_tn_set_RF_front_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U8	buf1 = 0, buf2 = 0, buf3 = 0, buf4 = 0;

	U32	fl = 112000;
	U32	fm = 331000;

	U32 freq_KHz = handle->tuner_freq;

	if(handle->tuner_int_im == 0)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7b, 0x27);
		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, 0x22);

		if(freq_KHz <= 800000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0xe7);
		}
		else
		{
			if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2))
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0xe7);
			}
			else
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0x67);
			}
		}
	}
	else if(handle->tuner_int_im == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7b, 0x1b);
		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, 0x22);

		if(freq_KHz <= 800000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0x9b);
		}
		else
		{
			if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2))
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0x9b);
			}
			else
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0x1b);
			}
		}
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7b, 0x2b);
		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, 0x55);
		_mt_fe_tn_set_reg_tc3800(handle, 0x7d, 0xab);
	}

	if(freq_KHz <= fl)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x71, 0x08);
		_mt_fe_tn_set_reg_tc3800(handle, 0x82, 0xac);
		_mt_fe_tn_set_reg_tc3800(handle, 0x0e, 0xfd);

		if(freq_KHz <= 112000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x76, 0x8b);
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x76, 0x6b);
		}
	}
	else if(freq_KHz <= fm)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x71, 0x04);
		_mt_fe_tn_set_reg_tc3800(handle, 0x82, 0x8c);
		_mt_fe_tn_set_reg_tc3800(handle, 0x0e, 0x7d);
		_mt_fe_tn_set_reg_tc3800(handle, 0x76, 0x8b);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x71, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0x82, 0x8c);
		_mt_fe_tn_set_reg_tc3800(handle, 0x0e, 0x7d);
		_mt_fe_tn_set_reg_tc3800(handle, 0x76, 0x6b);
	}

	if(freq_KHz <= 283000)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7a, 0xc0);
	}
	else if(freq_KHz <= 498000)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7a, 0xc2);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x7a, 0xc4);
	}

	if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
	{
		buf1 = 0;
		_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe1);
		buf4 = 2;
	}
	else
	{
		if(freq_KHz <= 331000)
		{
			if((handle->tuner_moni_type == 0) || (handle->tuner_moni_type == 3))
			{
				buf1 = 2;
				_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe9);
			}
			else
			{
				buf1 = 1;
				_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe5);
			}
		}
		else
		{
			buf1 = 1;
			_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe5);
		}
	}

	if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt == 3))
	{
		if((handle->tuner_int_im == 0) || (handle->tuner_int_im == 1))
		{
			buf2 = 34;
			buf3 = 34;
		}
		else
		{
			buf2 = 85;
			buf3 = 85;
		}
	}
	else
	{
		buf2 = 0;
		buf3 = 119;
	}

	if((handle->tuner_moni_type == 0) || (handle->tuner_moni_type == 3))
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xb6, 0x02);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);

		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf2);
	}
	else if(handle->tuner_moni_type == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xb6, 0x02);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);

		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf3);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xb6, 0x02);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf4);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf4);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2);

		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf2);
	}

	if(handle->tuner_lb_sen_boost == 1)
	{
		if(freq_KHz <= fl)
		{
			if(freq_KHz <= 54000)
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x75, 0x00);
			}
			else
			{
				_mt_fe_tn_set_reg_tc3800(handle, 0x75, 0x07);
			}
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x75, 0x27);
		}
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_Mixer_tc3800									 */
/*****************************************************************************/
void _mt_fe_tn_set_Mixer_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U32 freq_KHz = handle->tuner_freq;

	if(((freq_KHz <= 331000) && (freq_KHz > 112000)) || ((freq_KHz <= 112000) && (handle->tuner_lb_sen_boost < 2)))
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x25, 0x02);
		_mt_fe_tn_set_reg_tc3800(handle, 0x27, 0x95);

		if(freq_KHz < 167000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x26, 0xdd);
		}
		else if(freq_KHz < 231000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x26, 0x64);
		}
		else if(freq_KHz < 335000)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x26, 0x94);
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x26, 0x02);
		}
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x25, 0x82);
		_mt_fe_tn_set_reg_tc3800(handle, 0x27, 0x85);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_PLL_freq_tc3800								 */
/*****************************************************************************/
void _mt_fe_tn_set_PLL_freq_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U8	buf = 0, buf1 = 0, buf2 = 0;

	U32	FR_A = 48000;
	U32	FR_B = 48000;
	U32	FD = 108000;
	U32	F = 0;
	U32	FR = 0;
	U32	FVCO_TG = 0;
	U32	FVCO_V = 0;
	S32	LDIV = 0;
	U32	div_out[5] ={0, 0, 0, 0, 0};
	U32	NS32 = 0;
	U32	NH = 0;
	U32	CDIV = 0;
	U8	ICP_CODE = 0;
	U32	rdiv_x2 = 0;
	U32	FD_Hz = 0;
	S32	n_cdiv = 0;
	U32	F_CDIV_1[30] =
		{
			 68500, 155000, 202000, 205500, 212500, 226000, 283000, 387000, 411000, 419000,
			427000, 474000, 530000, 548000, 562000, 566000, 618000, 620000, 714000, 746000,
			774000, 808000, 818000, 822000, 826000, 838000, 850000, 854000, 904000, 948000
		 };
	U32 F_CDIV_2[7]= { 198500, 219000, 226500, 658000, 794000, 876000, 906000};

	U32	F_CDIV27_1[7] = {419000, 474000, 498000, 506000, 650000, 730000, 786000};
	U32	F_CDIV27_2[3] = {634000, 674000, 778000};
	U32	F_CDIV27_3[5] = {538000, 610000, 626000, 754000, 834000};
	U32	F_CDIV27_4[4] = { 52500, 578000, 842000, 858000};
	   	
	U32 freq_KHz = handle->tuner_freq;

	handle->tuner_int_sdm	 = 1;
	handle->tuner_int_nhalf	 = 1;

	if (handle->tuner_crystal == 27000)
	{
		FR_A = 54000;
		FR_B = 54000;
	}
	_mt_fe_tn_set_reg_tc3800(handle, 0x56, 0x78);

	_mt_fe_tn_set_reg_tc3800(handle, 0x4b, 0x39);

	_mt_fe_tn_set_reg_tc3800(handle, 0x47, 0x0f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x42, 0x47);
	_mt_fe_tn_set_reg_tc3800(handle, 0x41, 0xd0);

	_mt_fe_tn_set_reg_tc3800(handle, 0x54, 0x08);

	if(((freq_KHz <= 331000) && (freq_KHz > 112000)) || ((freq_KHz <= 112000) && (handle->tuner_lb_sen_boost < 2)))
	{
		if(freq_KHz < 56000)
		{
			LDIV = 72;
			buf = 0x1f;
			FR = FR_B;
		}
		else if(freq_KHz < 83500)
		{
			LDIV = 48;
			buf = 0x1b;
			FR = FR_B;
		}
		else if(freq_KHz < 112000)
		{
			LDIV = 36;
			buf = 0x17;
			FR = FR_B;
		}
		else if(freq_KHz < 167000)
		{
			LDIV = 24;
			buf = 0x13;
			FR = FR_B;
		}
		else if(freq_KHz < 231000)
		{
			LDIV = 16;
			buf = 0x12;
			FR = FR_A;
		}
		else if(freq_KHz < 335000)
		{
			LDIV = 12;
			buf = 0x11;
			FR = FR_B;
		}
		else
		{
			LDIV = 8;
			buf = 0x10;
			FR = FR_A;
		}
	}
	else
	{
		if(freq_KHz < 62500)
		{
			LDIV = 64;
			buf = 0x0b;
			FR = FR_A;
		}
		else if(freq_KHz < 83500)
		{
			LDIV = 48;
			buf = 0x0e;
			FR = FR_B;
		}
		else if(freq_KHz < 125000)
		{
			LDIV = 32;
			buf = 0x0a;
			FR = FR_A;
		}
		else if(freq_KHz < 167000)
		{
			LDIV = 24;
			buf = 0x0d;
			FR = FR_B;
		}
		else if(freq_KHz < 250000)
		{
			LDIV = 16;
			buf = 0x09;
			FR = FR_A;
		}
		else if(freq_KHz < 335000)
		{
			LDIV = 12;
			buf = 0x0c;
			FR = FR_B;
		}
		else if(freq_KHz < 500000)
		{
			LDIV = 8;
			buf = 0x08;
			FR = FR_A;
		}
		else if(freq_KHz < 670000)
		{
			LDIV = 6;
			buf = 0x04;
			FR = FR_B;
		}
		else
		{
			LDIV = 4;
			buf = 0x00;
			FR = FR_A;
		}
	}

	handle->tuner_ldiv = LDIV;

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x11, buf, 4, 0);

	FVCO_TG = LDIV * handle->tuner_freq;

	rdiv_x2=(U32)((4 * handle->tuner_crystal / FR + 1) / 2);

	if ((((U8)rdiv_x2) & 0x01) == 0)
	{
		buf = (U8)(rdiv_x2 & 0x0f);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, buf, 7, 4);
	}
	else
	{
		buf = (U8)(rdiv_x2 & 0x07);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, buf, 7, 5);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, 0x01, 4, 4);
	}

	handle->tuner_int_frv_K = 2 * handle->tuner_crystal / rdiv_x2;

	buf1 = (U8)handle->tuner_int_nhalf;
	buf2 = (U8)handle->tuner_int_sdm;

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x40, buf1, 1, 1);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x41, buf2, 7, 7);

	_mt_fe_tn_div_ratio_cal_tc3800(handle, FVCO_TG, handle->tuner_int_frv_K, FD, handle->tuner_int_nhalf, handle->tuner_int_sdm, div_out);

	NS32 = div_out[0];
	NH = div_out[1];
	F = div_out[2];
	CDIV = div_out[3];

	if(F == 0)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x41, 0x00, 7, 7);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x46, 0xe8);
	}
    if(handle->tuner_crystal == 24000)
	{
		if(((freq_KHz >= 51000) && (freq_KHz <= 53000)) ||
		   (freq_KHz == 78000) || (freq_KHz == 79000) ||
		   ((freq_KHz >= 104000) && (freq_KHz <= 109000)))
		{
			CDIV = CDIV - 2;
		}
	
		if ((freq_KHz == 55000) || (freq_KHz == 56000))
		{
			CDIV = CDIV - 6;
		}
	
		if ((freq_KHz == 57000) || (freq_KHz == 83000) ||
			(freq_KHz == 84000) || (freq_KHz == 85000) ||
			(freq_KHz == 110000) || (freq_KHz == 111000))
		{
			CDIV = CDIV - 3;
		}
	
		for(n_cdiv = 0; n_cdiv < 30; n_cdiv ++)
		{
			if (freq_KHz == F_CDIV_1[n_cdiv])
			{
				CDIV = CDIV - 1;
			}
		}
	
		for(n_cdiv = 0; n_cdiv < 7; n_cdiv ++)
		{
			if(freq_KHz == F_CDIV_2[n_cdiv])
			{
				CDIV = CDIV - 2;
			}
		}
	}
	else if(handle->tuner_crystal == 27000)
	{
		for(n_cdiv = 0; n_cdiv < 7; n_cdiv ++)
		{
			if (freq_KHz == F_CDIV27_1[n_cdiv])
			{
				CDIV = CDIV - 1;
			}
		}
		
		for(n_cdiv = 0; n_cdiv < 3; n_cdiv ++)
		{
			if (freq_KHz == F_CDIV27_2[n_cdiv])
			{
				CDIV = CDIV - 2;
			}
		}
		
		for(n_cdiv = 0; n_cdiv < 5; n_cdiv ++)
		{
			if (freq_KHz == F_CDIV27_3[n_cdiv])
			{
				CDIV = CDIV - 3;
			}
		}
		
		for(n_cdiv = 0; n_cdiv < 4; n_cdiv ++)
		{
			if (freq_KHz == F_CDIV27_4[n_cdiv])
		{
			CDIV = CDIV - 4;
		}		
	}
	}


	buf = (U8)(NS32 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4f, buf);

	buf = (U8)(((F & 0xff00) >> 8) & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x51, buf);

	buf = (U8)(F & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x52, buf);

	buf1 = (U8)((((NS32 >> 8) & 0x03) << 4) & 0x30);
	buf = (U8)(((NH & 0x01) << 6) & 0x40);
	buf1 |= buf;
	buf = (U8)((F >> 16) & 0x0f);
	buf1 |= buf;

	_mt_fe_tn_get_reg_tc3800(handle, 0x4e, &buf);
	buf &= 0x80;
	buf |= buf1;
	_mt_fe_tn_set_reg_tc3800(handle, 0x4e, buf);

	buf = (U8)(CDIV & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x50, buf);


	FVCO_V = div_out[4];
	FD_Hz = FVCO_V * 1000 / CDIV;

	_mt_fe_tn_set_reg_tc3800(handle, 0x49, 0x76);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4a, 0x04);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x48, 0x03, 2, 0);

	ICP_CODE =(U8)(11 * (3680 *1000 / FVCO_V));

	if (handle->tuner_int_frv_K == 8000)	ICP_CODE =(U8)(1 * (17*3990*1000 / FVCO_V));
	if (handle->tuner_int_frv_K == 24000)	ICP_CODE =(U8)((1 * (8*3680*2000 / FVCO_V) + 1)/2);
	if (handle->tuner_int_frv_K == 28800)	ICP_CODE =(U8)(1 * (10*3680*1000 / FVCO_V));
	if (handle->tuner_int_frv_K == 57600)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);
	if (handle->tuner_int_frv_K == 54000)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);
	if (handle->tuner_int_frv_K == 48000)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);

	if (ICP_CODE >= 31)
	{
		ICP_CODE = 31;
	}

	if(F > 0)
	{
		if (handle->tuner_int_frv_K == 8000)	ICP_CODE =(U8)(1 * (15*3990*1000 / FVCO_V));
		if (handle->tuner_int_frv_K == 24000)	ICP_CODE =(U8)((1 * (4*3680*2000 / FVCO_V) + 1)/2);
		if (handle->tuner_int_frv_K == 28800)	ICP_CODE =(U8)((1 * (6*3680*2000 / FVCO_V) + 1)/2);
		if (handle->tuner_int_frv_K == 57600)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
		if (handle->tuner_int_frv_K == 54000)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
		if (handle->tuner_int_frv_K == 48000)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
	}

	buf = (U8)(ICP_CODE & 0x1f);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x48, buf, 7, 3);

	handle->tuner_int_fm = FD_Hz;

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, 0x03, 3, 2);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, 0x02, 3, 2);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, 0x03, 3, 2);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4b, 0x00, 3, 2);

	_mt_sleep(3);

	_mt_fe_tn_set_reg_tc3800(handle, 0x56, 0xf8);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_BB_tc3800    									 */
/*****************************************************************************/
void _mt_fe_tn_set_BB_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U8  bandwidth = 0;

	bandwidth = handle->tuner_bandwidth;

	if (bandwidth == 6)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x2d, 0x57);
	}
	else if (bandwidth == 7)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x2d, 0x56);
	}
	else if (bandwidth == 10)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x2d, 0x54);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x2d, 0x55);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_BB_Cal_tc3800    								 */
/*****************************************************************************/
void _mt_fe_tn_set_BB_Cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 bandwidth)
{
	U32	Fcal_tia = 0;
	U32	Fcal_bq = 0;
	U32	FR_V = handle->tuner_int_frv_K;
	U8 	N_HALF = handle->tuner_int_nhalf;
	U8 	SDM = handle->tuner_int_sdm;

	U32 NS32_tia = 0, NH_tia = 0, F_tia = 0, CDIV_tia = 0;
	U32 NS32_bq = 0, NH_bq = 0, F_bq = 0, CDIV_bq = 0;

	U32	div_out[5] = {0, 0, 0, 0, 0};
	U8  buf = 0, buf1 = 0, buf2 = 0;

	if (bandwidth == 6)
	{
		Fcal_tia	 = 36810;
		Fcal_bq		 = 23310;
	}
	else if (bandwidth == 7)
	{
		Fcal_tia	 = 42950;
		Fcal_bq		 = 27200;
	}
	else if (bandwidth == 10)
	{
		Fcal_tia	 = 61350;
		Fcal_bq		 = 38850;
	}
	else
	{
		if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
		{
			Fcal_tia	 = 49080;
			Fcal_bq		 = 31080;
		}
		else
		{
			Fcal_tia	 = 34080;
			Fcal_bq		 = 30080;
		}
	}

	_mt_fe_tn_div_ratio_cal_tc3800(handle, 3860000, FR_V, Fcal_tia, N_HALF, SDM, div_out);

	NS32_tia = div_out[0];
	NH_tia = div_out[1];
	F_tia = div_out[2];
	CDIV_tia = div_out[3];

	_mt_fe_tn_div_ratio_cal_tc3800(handle, 3108000, FR_V, Fcal_bq, N_HALF, SDM, div_out);

	NS32_bq = div_out[0];
	NH_bq = div_out[1];
	F_bq = div_out[2];
	CDIV_bq = div_out[3];

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x4c, 0x01, 7, 7);

	buf	 = (U8)((F_tia >> 16) & 0x0f);
	buf1 = (U8)((NS32_tia >> 4) & 0x30);
	buf2 = (U8)((NH_tia << 6) & 0x40);
	buf = (U8)((buf | buf1) | buf2);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)(NS32_tia & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)(CDIV_tia & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)((F_tia >> 8) & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)(F_tia & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);

	buf	 = (U8)((F_bq >> 16) & 0x0f);
	buf1 = (U8)((NS32_bq >> 4) & 0x30);
	buf2 = (U8)((NH_bq << 6) & 0x40);
	buf = (U8)((buf | buf1) | buf2);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);

	buf = (U8)(NS32_bq & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)(CDIV_bq & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)((F_bq >> 8) & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);
	buf = (U8)(F_bq & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4d, buf);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_DAC_tc3800    								 */
/*****************************************************************************/
void _mt_fe_tn_set_DAC_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U32		DACFreq_KHz = 0;
	U32		fc = 0, fadc = 0, fsd = 0, f2d = 0;
	U8		tempnumber = 0;
	S32		FreqTrue108_Hz = 0;
	S32		f1 = 0, f2 = 0, delta1 = 0, Totalnum1 = 0;
	S32		cntT = 0, cntin = 0, NCOI = 0, z0 = 0, z1 = 0, z2 = 0, tmp = 0;
	S32		f1f2number = 0;

	if(handle->tuner_application == 2)
	{
		DACFreq_KHz = 4100;
	}
	else
	{
		DACFreq_KHz = handle->tuner_dac;
	}

	if (handle->tuner_application == 4)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x1e, 0x85);
	}
	else if(DACFreq_KHz == 4500)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x1e, 0x55);
	}
	else if(DACFreq_KHz == 7200)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x1e, 0x95);
	}
	else if(DACFreq_KHz == 4100)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x1e, 0x55);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x1e, 0x95);
	}

	FreqTrue108_Hz =  (S32)(handle->tuner_int_fm);
	f1f2number = ((DACFreq_KHz * 1000 * 512)/ FreqTrue108_Hz) * 128
				+ (((DACFreq_KHz * 1000 * 512) % FreqTrue108_Hz) * 32 / FreqTrue108_Hz) * 4;

	_mt_fe_tn_set_reg_tc3800(handle, 0xf7, (U8)((f1f2number & 0xff00) >> 8));
	_mt_fe_tn_set_reg_tc3800(handle, 0xf8, (U8)((f1f2number & 0x00ff)));

	f1 = 4096;
	fc = FreqTrue108_Hz;
	fadc = fc / 4;
	fsd = 27000000;
	if (handle->tuner_application == 4)
	{
		f2d = handle->tuner_bandwidth * 1000 / 2 - 150;
	}
	else
	{
		f2d = handle->tuner_bandwidth * 1000 / 2 + 0;
	}

	f2 = (fsd / 250) * f2d / ((fc + 500) / 1000);
	delta1 = ((f1 - f2) << 15) / f2;
	Totalnum1 = ((f1 - f2) << 15) - delta1 * f2;
	cntT = f2;
	cntin = Totalnum1;
	NCOI = delta1;
	z0 = cntin;
	z1 = cntT;
	z2 = NCOI;

	tempnumber = (U8)((z0 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe0, (U8)(tempnumber & 0x0f));
	tempnumber = (U8)(z0 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe1, tempnumber);
	tempnumber = (U8)((z1 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe2, tempnumber);
	tempnumber = (U8)(z1 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe3, tempnumber);
	tempnumber = (U8)((z2 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe4, tempnumber);
	tempnumber = (U8)(z2 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xe5, tempnumber);

	tmp = f1;
	f1 = f2;
	f2 = tmp / 2;
	delta1 = ((f1 - f2) << 15) / f2;
	Totalnum1 = ((f1 - f2) << 15) - delta1 * f2;
	NCOI = (f1 << 15) / f2 - (1 << 15);
	cntT = 	f2;
	cntin = Totalnum1;
	z0 = cntin;
	z1 = cntT;
	z2 = NCOI;

	tempnumber = (U8)((z0 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xef, (U8)(tempnumber & 0x0f));
	tempnumber = (U8)(z0 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xf0, tempnumber);
	tempnumber = (U8)((z1 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xf1, tempnumber);
	tempnumber = (U8)(z1 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xf2, tempnumber);
	tempnumber = (U8)((z2 & 0xff00) >> 8);
	_mt_fe_tn_set_reg_tc3800(handle, 0xf3, tempnumber);
	tempnumber = (U8)(z2 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xf4, tempnumber);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_preset_tc3800    									 */
/*****************************************************************************/
void _mt_fe_tn_preset_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	_mt_fe_tn_set_reg_tc3800(handle, 0x0a, 0xef);
	_mt_fe_tn_set_reg_tc3800(handle, 0x0b, 0x3f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x8a, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x8d, 0x3f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x2a, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x2b, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x9b, 0xd9);
	_mt_fe_tn_set_reg_tc3800(handle, 0xac, 0x35);
	_mt_fe_tn_set_reg_tc3800(handle, 0xad, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0xa9, 0x9f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x31, 0x09);
	_mt_fe_tn_set_reg_tc3800(handle, 0x33, 0x0f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x72, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x74, 0x00);

	if(handle->tuner_int_fast_set == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xa0, 0x08);
		_mt_fe_tn_set_reg_tc3800(handle, 0x70, 0x12);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x97, 0x01, 2, 0);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xa0, 0x0a);
		_mt_fe_tn_set_reg_tc3800(handle, 0x70, 0x13);
	}


	if((handle->tuner_mode == 1) || (handle->tuner_mode == 2))
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x24);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x04);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_div_ratio_cal_tc3800  							 */
/*****************************************************************************/
void _mt_fe_tn_div_ratio_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 FVCO_TG, U32 FR_V, U32 FD, U8 N_HALF, U8 SDM, U32 *div_out)
{
	U32	Ndiv2_scale			= 0;
	U32	div_N_V_scale_right	= 0;
	U32	div_N_V_scale_wrong	= 0;
	U32	den_shift_bit		= 7;
	U32	scale				= 128;
	U32	scale2				= 8;

	if((FR_V == 8000) || (FR_V == 24000))
	{
		den_shift_bit = 6;
	}
	else if((FR_V == 16000) || (FR_V == 48000) || (FR_V == 28800))
	{
		den_shift_bit = 7;
	}
	else if((FR_V == 32000) || (FR_V == 59600))
	{
		den_shift_bit = 8;
	}
	else if(FR_V == 54000)
	{
		den_shift_bit = 4;
	}
	else if(FR_V == 27000)
	{
		den_shift_bit = 3;
	}
	else
	{
		den_shift_bit = 6;
	}

	Ndiv2_scale = FVCO_TG * 1024 / (FR_V >> den_shift_bit);
	scale  = (U32)0x01 << (10 + den_shift_bit);
	scale2 = (U32)0x01 << (20 - 10 - den_shift_bit);

	if ((N_HALF == 0) && (SDM == 0))
	{
		div_out[0] = (U32)((Ndiv2_scale + scale / 2) / scale);
		div_out[1] = 0;
		div_out[2] = 0;
	}
	else if ((N_HALF == 0) && (SDM == 1))
	{
		div_out[0] = (U32)(Ndiv2_scale / scale);
		div_out[1] = 0;
		div_out[2] = (U32)(scale2 * (Ndiv2_scale - div_out[0] * scale));
	}
	else if ((N_HALF == 1) && (SDM == 0))
	{
		div_out[0] = (U32)((Ndiv2_scale + scale / 4) / scale);

		if((Ndiv2_scale - div_out[0] * scale) < (scale / 4))
			div_out[1] = 0;
		else
			div_out[1] = 1;

		div_out[2] = 0;
	}
	else
	{
		div_out[0] = (U32)(Ndiv2_scale / scale);

		if((Ndiv2_scale - div_out[0]*scale) < (scale / 2))
			div_out[1] = 0;
		else
			div_out[1] = 1;

		div_out[2] = 2 * scale2 * (Ndiv2_scale - div_out[0] * scale - div_out[1] * scale / 2);
	}

	if(N_HALF == 0)
	{
		div_N_V_scale_right = div_out[0] * scale + div_out[1] * scale / 2 + div_out[2] / (scale2 * (1 + 0));
	}
	else
	{
		div_N_V_scale_right = div_out[0] * scale + div_out[1] * scale / 2 + div_out[2] / (scale2 * (1 + 1));
	}

	div_N_V_scale_wrong = div_out[0]*scale + div_out[1] * scale / 2 + div_out[2]/(scale2 * (1 + div_out[1]));

	div_out[3] = (U32) ((div_N_V_scale_wrong * (FR_V >> den_shift_bit) / FD + 1024 / 2) / 1024);

	div_out[4] = (U32) (div_N_V_scale_right * (FR_V >> den_shift_bit) / 1024);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_freq_fine_tune_tc3800  							 */
/*****************************************************************************/
void _mt_fe_tn_freq_fine_tune_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 Delta_sign, U8 Num_step)
{
	_mt_fe_tn_freq_fine_tune_core_tc3800(handle, Delta_sign, Num_step, handle->tuner_ldiv, handle->tuner_int_frv_K, handle->tuner_int_nhalf, handle->tuner_int_sdm);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_freq_fine_tune_core_tc3800  						 */
/*****************************************************************************/
void _mt_fe_tn_freq_fine_tune_core_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 Delta_sign, U8 Num_step, U32 LDIV, U32 FR_V, U8 N_HALF, U8 SDM)
{
	U32	Ndiv2_scale			 = 0;
	U32	div_N_V_scale_right	 = 0;
	U32	den_shift_bit		 = 7;
	U32	scale				 = 128;
	U32	scale2				 = 8;

	U32	NS32				 = 0;
	U32	NH					 = 0;
	U32	CDIV				 = 0;
	U32 F					 = 0;
	U32 FVCO_V				 = 0;

	U32 Delta_Ndiv2_scale	 = 0;
	U8 ICP_CODE				 = 0;
	U8	buf					 = 0;
	U8	buf1				 = 0;
	U8	buf2				 = 0;

	_mt_fe_tn_get_reg_tc3800(handle, 0x4e, &buf2);
	NS32 = (buf2 << 4 ) & 0x300;
	_mt_fe_tn_get_reg_tc3800(handle, 0x4f, &buf2);
	NS32 = NS32 + (buf2 & 0xff);
	_mt_fe_tn_get_reg_tc3800(handle, 0x4e, &buf2);
	NH = (buf2 & 0x40) >> 6;
	_mt_fe_tn_get_reg_tc3800(handle, 0x4e, &buf2);
	F = (buf2 & 0x0f) << 16;
	_mt_fe_tn_get_reg_tc3800(handle, 0x51, &buf2);
	F = F + ((buf2 & 0xff) << 8 );
	_mt_fe_tn_get_reg_tc3800(handle, 0x52, &buf2);
	F = F + (buf2 & 0xff);
	_mt_fe_tn_get_reg_tc3800(handle, 0x50, &buf2);
	CDIV = buf2 & 0xff;


	if((FR_V == 8000) || (FR_V == 24000))
	{
		den_shift_bit = 6;
	}
	else if((FR_V == 16000) || (FR_V == 48000) || (FR_V == 28800))
	{
		den_shift_bit = 7;
	}
	else if((FR_V == 32000) || (FR_V == 59600))
	{
		den_shift_bit = 8;
	}
	else if(FR_V == 54000)
	{
		den_shift_bit = 4;
	}
	else if(FR_V == 27000)
	{
		den_shift_bit = 3;
	}
	else
	{
		den_shift_bit = 6;
	}


	scale  = (U32)0x01 << (10 + den_shift_bit);
	scale2 = (U32)0x01 << (20 - 10 - den_shift_bit);

	Delta_Ndiv2_scale = (U32)((LDIV * Num_step * 1000 * 1024 / (FR_V >> den_shift_bit)) /64);


	if(N_HALF == 0)
	{
		div_N_V_scale_right = NS32*scale + NH*scale/2 + F/(scale2*(1+0));
	}
	else
	{
		div_N_V_scale_right = NS32*scale + NH*scale/2 + F/(scale2*(1+1));
	}

	if (Delta_sign ==1)
	{
		Ndiv2_scale = div_N_V_scale_right + Delta_Ndiv2_scale;
	}
	else
	{
		Ndiv2_scale = div_N_V_scale_right - Delta_Ndiv2_scale;
	}


	if ((N_HALF == 0) && (SDM == 0))
	{
		NS32 = (U32)((Ndiv2_scale + scale/2) / scale);
		NH = 0;
		F = 0;
	}
	else if ((N_HALF == 0) && (SDM == 1))
	{
		NS32 = (U32)(Ndiv2_scale/scale);
		NH = 0;
		F = (U32)(scale2 * (Ndiv2_scale - NS32*scale));
	}
	else if ((N_HALF == 1) && (SDM == 0))
	{
		NS32 = (U32)((Ndiv2_scale + scale/4)/scale);

		if((Ndiv2_scale - NS32*scale) < (scale/4))
			NH = 0;
		else
			NH = 1;

		F = 0;
	}
	else
	{
		NS32 = (U32)(Ndiv2_scale/scale);

		if((Ndiv2_scale - NS32*scale) < (scale/2))
			NH = 0;
		else
			NH = 1;

		F = 2*scale2*(Ndiv2_scale - NS32*scale - NH*scale/2);
	}


	buf = (U8)(NS32 & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x4f, buf);

	buf = (U8)(((F & 0xff00) >> 8) & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x51, buf);

	buf = (U8)(F & 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x52, buf);

	buf1 = (U8)((((NS32 >> 8) & 0x03) << 4) & 0x30);
	buf = (U8)(((NH & 0x01) << 6) & 0x40);
	buf1 |= buf;
	buf = (U8)((F >> 16) & 0x0f);
	buf1 |= buf;

	_mt_fe_tn_get_reg_tc3800(handle, 0x4e, &buf);
	buf &= 0x80;
	buf |= buf1;
	_mt_fe_tn_set_reg_tc3800(handle, 0x4e, buf);

	FVCO_V = (U32) (Ndiv2_scale * (FR_V >> den_shift_bit) / 1024);

	ICP_CODE =(U8)(11 * (3680 *1000 / FVCO_V));
	if((F < 100)||(F > 1048476))
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x41, 0x00, 7, 7);
		_mt_fe_tn_set_reg_tc3800(handle, 0x46, 0xe0);
		if (FR_V == 8000)	ICP_CODE =(U8)(1 * (17*3990*1000 / FVCO_V));
		if (FR_V == 24000)	ICP_CODE =(U8)((1 * (8*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 28800)	ICP_CODE =(U8)(1 * (10*3680*1000 / FVCO_V));
		if (FR_V == 57600)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 54000)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 48000)	ICP_CODE =(U8)((1 * (5*3680*2000 / FVCO_V) + 1)/2);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x41, 0x01, 7, 7);
		_mt_fe_tn_set_reg_tc3800(handle, 0x46, 0xe8);
		if (FR_V == 8000)	ICP_CODE =(U8)(1 * (15*3990*1000 / FVCO_V));
		if (FR_V == 24000)	ICP_CODE =(U8)((1 * (4*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 28800)	ICP_CODE =(U8)((1 * (6*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 57600)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 54000)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
		if (FR_V == 48000)	ICP_CODE =(U8)((1 * (3*3680*2000 / FVCO_V) + 1)/2);
	}

	if (ICP_CODE >= 31)
	{
		ICP_CODE = 31;
	}
	buf = (U8)(ICP_CODE & 0x1f);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x48, buf, 7, 3);

	handle->tuner_int_fm = FVCO_V * 1000 / CDIV;

	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x40);
	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0x00);
	_mt_fe_tn_set_DAC_tc3800(handle);

	_mt_fe_tn_set_reg_tc3800(handle, 0xc9, 0x15);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_tune_tc3800       							 */
/*****************************************************************************/
void _mt_fe_tn_set_tune_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 WaitTime)
{
	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x7f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0xf8);

	_mt_fe_tn_set_RF_front_tc3800(handle);

	_mt_fe_tn_set_Mixer_tc3800(handle);

	_mt_fe_tn_set_PLL_freq_tc3800(handle);

	_mt_fe_tn_set_BB_tc3800(handle);
	_mt_fe_tn_set_BB_Cal_tc3800(handle, handle->tuner_bandwidth);

	_mt_fe_tn_set_DAC_tc3800(handle);

	_mt_fe_tn_preset_tc3800(handle);

	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0x00);

	if (handle->tuner_clock_out == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x13, 0x67);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x13, 0x66);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x30, 0x10);

	_mt_fe_tn_set_reg_tc3800(handle, 0x88, 0xf7);
	_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0x86);
	_mt_fe_tn_set_reg_tc3800(handle, 0x99, 0xf6);
	_mt_fe_tn_set_reg_tc3800(handle, 0x9a, 0x75);
	_mt_fe_tn_set_reg_tc3800(handle, 0xbe, 0x01);

	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x01);
	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x00);

	_mt_sleep(WaitTime);

	if (handle->tuner_clock_out == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x13, 0x65);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x13, 0x64);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x30, 0x00);

	if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0x93);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0xb3);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x9a, 0x93);

	return;
}

/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_sysmon_tc3800       							 */
/*****************************************************************************/
void _mt_fe_tn_set_sysmon_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	if(handle->tuner_int_fast_set == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xb5, 0x14);
		_mt_fe_tn_set_reg_tc3800(handle, 0xbb, 0x00);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xb5, 0x75);

		if((handle->tuner_moni_type == 3) || (handle->tuner_application == 4))
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0xbb, 0xe8);
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0xbb, 0x28);
		}
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0xbc, 0x02);

	if(handle->tuner_moni_type == 2)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 3, 3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x26);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x10);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x20);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 3, 3);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x01);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x26);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x01);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x26);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb7, 0x0f);
	}

	if(handle->tuner_moni_type == 0)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 2, 2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x01);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x1d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x2a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);

		if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0c);
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0d);
		}

		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x17);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x2a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
	}
	else if(handle->tuner_moni_type == 1)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 2, 2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0b);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x14);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x03);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x12);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0e);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x17);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x2a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
	}
	else if(handle->tuner_moni_type == 2)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 2, 2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x01);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x1d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x2a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0c);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x19);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x31);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0xb6, 0x01, 2, 2);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x01);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x1d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x2a);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x00);

		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0d);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x13);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x19);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x23);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x39);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x43);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x3f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb8, 0x0f);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_poweron_set_tc3800     							 */
/*****************************************************************************/
void _mt_fe_tn_poweron_set_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x11, 0x00, 7, 7);

	if (handle->tuner_clock_out == 1)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x13, 0x01, 0, 0);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x13, 0x00, 0, 0);
	}

	if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x10, 0x01, 7, 6);
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x12, 0x0a, 7, 3);
	}

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x2e, 0x00, 5, 5);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x68, 0x03, 7, 6);
	_mt_fe_tn_set_reg_tc3800(handle, 0x03, 0x00);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x0f, 0x01, 4, 4);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x55, 0x02, 5, 4);

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x79, 0x02, 3, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x85, 0x00, 1, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x17, 0x07, 2, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x77, 0x02, 6, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x78, 0x02, 6, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x75, 0x02, 5, 4);

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x34, 0x00, 3, 2);

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0xad, 0x01, 5, 5);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x90);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xa0);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xa1);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xb1);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xb5);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xb7);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xf7);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x7f);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0xae, 0x04);

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x1c, 0x03, 6, 5);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x1d, 0x00, 2, 0);

	_mt_fe_tn_set_reg_tc3800(handle, 0x90, 0x10);

	if((handle->tuner_application == 3)|| (handle->tuner_application == 4))
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x87, 0x04, 7, 5);
		_mt_fe_tn_set_reg_tc3800(handle, 0x23, 0xff);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x87, 0x01, 7, 5);
		_mt_fe_tn_set_reg_tc3800(handle, 0x23, 0x95);
	}

	if(handle->tuner_application == 3)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x5d, 0x10);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x5d, 0x00);
	}

	if(handle->tuner_int_fast_set == 1)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x97, 0x01, 2, 0);
		_mt_fe_tn_set_reg_tc3800(handle, 0x61, 0x03);
	}
	else
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x97, 0x04, 2, 0);
		_mt_fe_tn_set_reg_tc3800(handle, 0x61, 0x4b);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x9c, 0xff);
	_mt_fe_tn_set_reg_tc3800(handle, 0x9d, 0x0c);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x1a, 0x07, 7, 5);
	_mt_fe_tn_set_reg_tc3800(handle, 0x21, 0x91);

	_mt_fe_tn_set_reg_tc3800(handle, 0xa1, 0x2a);
	_mt_fe_tn_set_reg_tc3800(handle, 0xa2, 0xb2);

	if (handle->tuner_application == 0)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xa3, 0x4e);
		_mt_fe_tn_set_reg_tc3800(handle, 0xa4, 0x6e);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xa3, 0x5e);
		_mt_fe_tn_set_reg_tc3800(handle, 0xa4, 0x84);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0xa5, 0x2a);
	_mt_fe_tn_set_reg_tc3800(handle, 0xa6, 0xb2);
	_mt_fe_tn_set_reg_tc3800(handle, 0xa8, 0x93);

	_mt_fe_tn_set_sysmon_tc3800(handle);

	if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2))
	{
		_mt_fe_tn_set_mch_cal_tc3800(handle);

		_mt_fe_tn_set_reg_tc3800(handle, 0x3a, 0x1d);
		_mt_fe_tn_set_reg_tc3800(handle, 0x35, 0x18);
		_mt_fe_tn_set_reg_tc3800(handle, 0x36, 0xe0);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb4, 0x00);
		_mt_fe_tn_set_reg_tc3800(handle, 0x59, 0x34);

		_mt_fe_tn_set_tune_tc3800(handle, 2000);

		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x60, 0x00, 0, 0);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x3a, 0x5d);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_lpf_cal_tc3800									 */
/*****************************************************************************/
void _mt_fe_tn_lpf_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U8 buf = 0, buf1 = 0, buf2 = 0, buf3 = 0;
	U8 tia_cap = 0, bq_cap = 0, temp_code = 0;
	U8 buf1_rf = 0, buf2_rf = 0, buf3_rf = 0, buf4_rf = 0;

	_mt_fe_tn_get_reg_tc3800(handle, 0x8a, &buf2);
	buf2 = (U8)(buf2 | 0x10);
	_mt_fe_tn_set_reg_tc3800(handle, 0x8a, buf2);

	_mt_fe_tn_get_reg_tc3800(handle, 0x7e, &buf3);
	buf3 = (U8)(buf3 | 0x88);
	_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf3);

	_mt_fe_tn_set_reg_tc3800(handle, 0x22, 0x41);

	_mt_fe_tn_set_reg_tc3800(handle, 0x35, 0x18);
	_mt_fe_tn_set_reg_tc3800(handle, 0x36, 0x07);
	_mt_fe_tn_set_reg_tc3800(handle, 0xb4, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x59, 0x34);

	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x01);
	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x00);

	_mt_sleep(10);

	_mt_fe_tn_get_reg_tc3800(handle, 0x31, &buf);
	tia_cap = (U8)(buf & 0x3f);
	_mt_fe_tn_get_reg_tc3800(handle, 0x33, &buf);
	bq_cap = (U8)(buf & 0x1f);
	_mt_fe_tn_get_reg_tc3800(handle, 0x3f, &buf);
	temp_code = (U8)((buf >> 4) & 0x0f);


	if((handle->tuner_moni_type == 2) || (handle->tuner_moni_type == 3))
	{
		buf1 = 0x02;
	}
	else
	{
		if(temp_code >= 12)
		{
			buf1 = 0x03;
		}
		else if(temp_code >= 10)
		{
			buf1 = 0x02;
		}
		else
		{
			buf1 = 0x01;
		}
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0xb6, 0x01);
	buf = buf1;
	buf = (U8)((buf << 5) | bq_cap);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, buf);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, tia_cap);
	buf = buf1;
	buf = (U8)((buf << 5) | bq_cap);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, buf);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, tia_cap);
	buf = buf1;
	buf = (U8)((buf << 5) | bq_cap);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, buf);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, tia_cap);
	buf = buf1;
	buf = (U8)((buf << 5) | bq_cap);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, buf);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, tia_cap);
	buf = buf1;
	buf = (U8)((buf << 5) | bq_cap);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, buf);
	_mt_fe_tn_set_reg_tc3800(handle, 0xba, tia_cap);

	if((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
	{
		buf1 = (U8)(buf1 | 0x20);
	}
	else
	{
		buf1 = buf1;
	}
	_mt_fe_tn_set_reg_tc3800(handle, 0x22, buf1);
	_mt_fe_tn_set_reg_tc3800(handle, 0x5f, temp_code);

	buf2 = (U8)(buf2 & 0xef);
	_mt_fe_tn_set_reg_tc3800(handle, 0x8a, buf2);
	_mt_fe_tn_set_reg_tc3800(handle, 0x8a, 0x00);

	buf3 = (U8)(buf3 & 0x77);
	_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf3);

	_mt_sleep(1);


	if(((handle->tuner_application == 3) || (handle->tuner_application == 4)) && (handle->tuner_freq >= 780000) && (handle->tuner_freq <= 790000))
	{
		if(temp_code >= 12)
		{
			buf1_rf = 4;
			_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xf1);
			buf4_rf = 4;
		}
		else if(temp_code >= 10)
		{
			buf1_rf = 2;
			_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe9);
			buf4_rf = 3;
		}
		else
		{
			buf1_rf = 1;
			_mt_fe_tn_set_reg_tc3800(handle, 0x1f, 0xe5);
			buf4_rf = 2;
		}

		buf2_rf = 0;
		buf3_rf = 119;

		_mt_fe_tn_set_reg_tc3800(handle, 0xb6, 0x02);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf4_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf4_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf3_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf1_rf);
		_mt_fe_tn_set_reg_tc3800(handle, 0xb9, buf2_rf);

		_mt_fe_tn_set_reg_tc3800(handle, 0x7e, buf2_rf);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_mch_cal_tc3800								 */
/*****************************************************************************/
void _mt_fe_tn_set_mch_cal_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x3b, 0x33, 6, 0);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x79, 0x0c, 7, 4);
	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x7b, 0x00, 7, 6);

	_mt_fe_tn_set_reg_bit_tc3800(handle, 0x3b, 0x01, 7, 7);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x4F);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x47);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x43);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x52);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x49);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x43);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x53);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x4F);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x49);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x45);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x41);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x38);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x52);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3C);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x4B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x46);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x41);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x51);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x49);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x41);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x39);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x42);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x50);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x44);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x3C);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x4E);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x23);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x20);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1E);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x25);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x21);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1E);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x25);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x23);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x21);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1F);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1A);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x19);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x25);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x22);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1F);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x24);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x21);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1A);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1D);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x24);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1E);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x23);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x80);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x44);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x44);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x44);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x08);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x08);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xC8);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xC8);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xC8);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xC8);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x28);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x88);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xD1);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xD1);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xB2);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x52);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x12);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xB2);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x93);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x93);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x93);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x93);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x17);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x1B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x9B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x9B);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xDF);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x44);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0x95);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xA5);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xE5);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xF5);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xF9);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xFA);
	_mt_fe_tn_set_reg_tc3800(handle, 0x3c, 0xFA);

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_mch_app_tc3800								 */
/*****************************************************************************/
void _mt_fe_tn_set_mch_app_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{

	U16	ccval = 0, ccval_h = 0, ccval_l = 0;
	U8	buf = 0;
	U8	idxh = 0, idxl = 0;
	U32 freq_ch = handle->tuner_freq;
	U32 freq_h = 0, freq_l = 0;

	_mt_fe_tn_get_reg_tc3800(handle, 0x60, &buf);
	buf = (U8)(buf & 0x01);

	if((((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2)) && (buf == 0x00))
	{
		if(freq_ch >= 954000)			{freq_h=1010000;	freq_l=954000;	idxh=0;		idxl=0;}
		else if(freq_ch >=858000)		{freq_h=954000;		freq_l=858000;	idxh=0;		idxl=1;}
		else if(freq_ch >=810000)		{freq_h=858000;		freq_l=810000;	idxh=1;		idxl=2;}
		else if(freq_ch >=738000)		{freq_h=810000;		freq_l=738000;	idxh=2;		idxl=3;}
		else if(freq_ch >=658000)		{freq_h=738000;		freq_l=658000;	idxh=3;		idxl=4;}
		else if(freq_ch >=586000)		{freq_h=658000;		freq_l=586000;	idxh=4;		idxl=5;}
		else if(freq_ch >=538000)		{freq_h=586000;		freq_l=538000;	idxh=5;		idxl=6;}
		else if(freq_ch >=498000)		{freq_h=538000;		freq_l=498000;	idxh=6;		idxl=7;}
		else if(freq_ch >=474000)		{freq_h=498000;		freq_l=474000;	idxh=7;		idxl=8;}
		else if(freq_ch >=443000)		{freq_h=474000;		freq_l=443000;	idxh=8;		idxl=9;}
		else if(freq_ch >=419000)		{freq_h=443000;		freq_l=419000;	idxh=9;		idxl=10;}
		else if(freq_ch >=395000)		{freq_h=419000;		freq_l=395000;	idxh=10;	idxl=11;}
		else if(freq_ch >=371000)		{freq_h=395000;		freq_l=371000;	idxh=11;	idxl=12;}
		else if(freq_ch >=355000)		{freq_h=371000;		freq_l=355000;	idxh=12;	idxl=13;}
		else if(freq_ch >=339000)		{freq_h=355000;		freq_l=339000;	idxh=13;	idxl=14;}
		else if(freq_ch > 331000)		{freq_h=339000;		freq_l=331000;	idxh=14;	idxl=14;}
		else if(freq_ch >=243000)		{freq_h=331000;		freq_l=243000;	idxh=15;	idxl=16;}
		else if(freq_ch >=227000)		{freq_h=243000;		freq_l=227000;	idxh=16;	idxl=17;}
		else if(freq_ch >=211000)		{freq_h=227000;		freq_l=211000;	idxh=17;	idxl=18;}
		else if(freq_ch >=195000)		{freq_h=211000;		freq_l=195000;	idxh=18;	idxl=19;}
		else if(freq_ch >=179000)		{freq_h=195000;		freq_l=179000;	idxh=19;	idxl=20;}
		else if(freq_ch >=163000)		{freq_h=179000;		freq_l=163000;	idxh=20;	idxl=21;}
		else if(freq_ch >=147000)		{freq_h=163000;		freq_l=147000;	idxh=21;	idxl=22;}
		else if(freq_ch >=131000)		{freq_h=147000;		freq_l=131000;	idxh=22;	idxl=23;}
		else if(freq_ch > 112000)		{freq_h=131000;		freq_l=115000;	idxh=23;	idxl=24;}
		else if(freq_ch >=88000)		{freq_h=112000;		freq_l=88000;	idxh=25;	idxl=25;}
		else if(freq_ch >=80000)		{freq_h=88000;		freq_l=80000;	idxh=25;	idxl=26;}
		else if(freq_ch >=68500)		{freq_h=80000;		freq_l=68500;	idxh=26;	idxl=27;}
		else if(freq_ch >=60500)		{freq_h=68500;		freq_l=60500;	idxh=27;	idxl=28;}
		else if(freq_ch >=52500)		{freq_h=60500;		freq_l=52500;	idxh=28;	idxl=29;}
		else							{freq_h=52500;		freq_l=42500;	idxh=29;	idxl=29;}


		_mt_fe_tn_set_reg_tc3800(handle, 0x3d, idxh);
		_mt_fe_tn_get_reg_tc3800(handle, 0x3d, &buf);
		ccval_h = (U16)((buf & 0x80) << 1);
		_mt_fe_tn_get_reg_tc3800(handle, 0x3e, &buf);
		ccval_h = (U16)(ccval_h + buf);

		_mt_fe_tn_set_reg_tc3800(handle, 0x3d, idxl);
		_mt_fe_tn_get_reg_tc3800(handle, 0x3d, &buf);
		ccval_l = (U16)((buf & 0x80) << 1);
		_mt_fe_tn_get_reg_tc3800(handle, 0x3e, &buf);
		ccval_l = (U16)(ccval_l + buf);

		if(freq_ch >= 1010000)
		{
			ccval_h = 0;
			ccval_l = 0;
		}
		else if(freq_ch >= 954000)
		{
			ccval_h = 0;
		}

		if (ccval_l > ccval_h)
		{
			ccval = (U16)(ccval_h + (ccval_l - ccval_h) * (freq_h - freq_ch) / (freq_h - freq_l));
		}
		else
		{
			ccval = (U16)(ccval_h + (ccval_h - ccval_l) * (freq_h - freq_ch) / (freq_h - freq_l));
		}

		buf = (U8)(((ccval >> 8) & 0x01) | 0x04);
		_mt_fe_tn_set_reg_tc3800(handle, 0x72, buf);
		buf = (U8)(ccval & 0xff);
		_mt_fe_tn_set_reg_tc3800(handle, 0x73, buf);
		buf = (U8)((ccval & 0x7f) | 0x80);
		_mt_fe_tn_set_reg_tc3800(handle, 0x74, buf);
		if(handle->tuner_int_fast_set == 0)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x70, 0x1b);			
		}
		else
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0x70, 0x1a);
		}

		_mt_fe_tn_get_reg_tc3800(handle, 0x71, &buf);
		buf = (U8)(buf | 0x10);
		_mt_fe_tn_set_reg_tc3800(handle, 0x71, buf);
	}

	return;
}


/*****************************************************************************/
/*Function: 	_mt_fe_tn_set_appendix_tc3800								 */
/*****************************************************************************/
void _mt_fe_tn_set_appendix_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	return;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_wakeup_tc3800												 */
/*****************************************************************************/
void mt_fe_tn_wakeup_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	_mt_fe_tn_set_reg_tc3800(handle, 0x06, 0x0d);

	return;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_sleep_tc3800												*/
/*****************************************************************************/
void mt_fe_tn_sleep_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	_mt_fe_tn_set_reg_tc3800(handle, 0x06, 0x05);

	_mt_fe_tn_set_reg_tc3800(handle, 0xbf, 0x01);

	return;
}

/*****************************************************************************/
/*API: 	mt_fe_tn_init_tc3800												 */
/*****************************************************************************/
void mt_fe_tn_init_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	if (handle->tuner_init_OK != 1)
	{
		handle->tuner_dev_addr			 = g_stTunerOps[g_m88tc3800_tuner_port].u32TunerAddress;

		handle->tuner_application		 = 0;
		handle->tuner_app_pre			 = 0x0f;
		handle->tuner_mode				 = 0;

		handle->tuner_bandwidth			 = 8;
		handle->tuner_crystal			 = 24000;
		handle->tuner_dac				 = 7200;

		handle->tuner_mtt				 = 0x00;

		handle->tuner_int_fast_set		 = 1;
		handle->tuner_int_lt			 = 2;
		handle->tuner_clock_out			 = 0;
		handle->tuner_int_im			 = 0;
		handle->tuner_custom_cfg		 = MT_TC3800_CUSTOM_CONFIG;
		handle->tuner_version			 = 111;  
		handle->tuner_time				 = 15030410;

		handle->tuner_init_OK			 = 1;
	}

	if (handle->tuner_mtt == 0x00)
	{
		U8 tmp = 0;

		_mt_fe_tn_get_reg_tc3800(handle, 0x01, &tmp);

		switch(tmp)
		{
			case 0xb4:
				handle->tuner_mtt = 0xB4;
				break;

			case 0xb5:
			default:
				handle->tuner_mtt = 0xB5;
				break;
		}
	}

	return;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_set_freq_tc3800											 */
/*****************************************************************************/
void mt_fe_tn_set_freq_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32 freq_KHz)
{

	U8 buf = 0, buf1 = 0, buf2 = 0, buf3 = 0, buf4 = 0;
	U8 tWait1 = 0, tWait2 = 0;

	mt_fe_tn_init_tc3800(handle);
	
	if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[g_m88tc3800_tuner_port].enDemodDevType)
    {
		handle->tuner_bandwidth			 = 6;
		handle->tuner_dac				 = 4100;
    }
    else
    {
		handle->tuner_bandwidth			 = 8;
		handle->tuner_dac				 = 7200;
	} 

	if (handle->tuner_application == 0)
	{
		handle->tuner_moni_type = 1;
		handle->tuner_lb_sen_boost = 0;
	}
	else if ((handle->tuner_application == 1) || (handle->tuner_application == 3) || (handle->tuner_application == 4))
	{
		handle->tuner_moni_type = 2;
		handle->tuner_lb_sen_boost = 2;
	}
	else if (handle->tuner_application == 2)
	{
		handle->tuner_moni_type = 3;
		handle->tuner_lb_sen_boost = 0;
	}
	else
	{
		handle->tuner_moni_type = 1;
		handle->tuner_lb_sen_boost = 0;
	}

	if (handle->tuner_app_pre != handle->tuner_application)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xbd, 0x00);
		handle->tuner_app_pre = handle->tuner_application;
	}

	if(handle->tuner_int_fast_set == 1)
	{
		tWait1 = 15;
		tWait2 = 15;
		buf2 = 0x82;
	}
	else
	{
		tWait1 = 20;
		tWait2 = 80;
		buf2 = 0xc2;
	}

	if((handle->tuner_moni_type == 2) || (handle->tuner_moni_type == 3))
	{
		buf2 = (U8)(buf2 & 0xfd);
	}

	if((handle->tuner_application == 3)|| (handle->tuner_application == 4))
	{
		buf3 = 0x80;
	}
	else
	{
		buf3 = 0x60;
	}

	handle->tuner_freq = freq_KHz;


	mt_fe_tn_wakeup_tc3800(handle);

	_mt_fe_tn_get_reg_tc3800(handle, 0xbd, &buf);

	if(buf == 0x00)
	{
		if(handle ->tuner_mtt == 0xB4)
		{
			_mt_sleep(100);
			_mt_fe_tn_set_reg_tc3800(handle, 0x08, 0x20);
			_mt_fe_tn_set_reg_tc3800(handle, 0x07, 0x20);
			_mt_fe_tn_set_reg_tc3800(handle, 0x08, 0x00);
			_mt_fe_tn_set_reg_tc3800(handle, 0x07, 0x00);
			_mt_sleep(10);
			
		}
		_mt_fe_tn_poweron_set_tc3800(handle);
		_mt_fe_tn_set_reg_tc3800(handle, 0xbd, 0xff);
		
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x3a, 0x5d);
		
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x7f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0xf8);

	_mt_fe_tn_set_RF_front_tc3800(handle);

	_mt_fe_tn_set_Mixer_tc3800(handle);

	_mt_fe_tn_set_PLL_freq_tc3800(handle);

	_mt_fe_tn_set_BB_tc3800(handle);

	_mt_fe_tn_preset_tc3800(handle);

	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x0f);
	_mt_fe_tn_set_reg_tc3800(handle, 0x04, 0x00);
	_mt_fe_tn_set_reg_tc3800(handle, 0x05, 0x00);

	if (handle->tuner_clock_out == 1)
	{
		buf4 = (U8)(buf3 | 0x07);
	}
	else
	{
		buf4 = (U8)(buf3 | 0x06);
	}
	_mt_fe_tn_set_reg_tc3800(handle, 0x13, buf4);

	_mt_fe_tn_set_reg_tc3800(handle, 0x30, 0x10);

	_mt_fe_tn_set_mch_app_tc3800(handle);

	_mt_fe_tn_set_BB_Cal_tc3800(handle, handle->tuner_bandwidth);
	_mt_fe_tn_lpf_cal_tc3800(handle);

	_mt_fe_tn_set_DAC_tc3800(handle);

	if(handle->tuner_application == 3)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xc9, 0x05);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0xc9, 0x15);
	}
	
	_mt_fe_tn_set_reg_tc3800(handle, 0x35, 0x19);
	_mt_fe_tn_set_reg_tc3800(handle, 0x36, 0xe0);
	_mt_fe_tn_set_reg_tc3800(handle, 0x59, 0x34);

	if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2))
	{
		_mt_fe_tn_get_reg_tc3800(handle, 0x8a, &buf1);
		buf1 = (U8)(buf1 | 0x10);
		_mt_fe_tn_set_reg_tc3800(handle, 0x8a, buf1);

		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0x95);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0x86);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x88, 0xf7);
	_mt_fe_tn_set_reg_tc3800(handle, 0x99, 0xf6);
	_mt_fe_tn_set_reg_tc3800(handle, 0x9a, 0x75);
	_mt_fe_tn_set_reg_tc3800(handle, 0xbe, 0x01);

	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x01);

	_mt_fe_tn_set_reg_tc3800(handle, 0x00, 0x00);

	_mt_fe_tn_set_reg_tc3800(handle, 0xb4, buf2);

	_mt_sleep(tWait1);

	if(((handle->tuner_mode == 1) || (handle->tuner_mode == 2)) && (handle->tuner_int_lt >= 2))
	{
		buf1 = (U8)(buf1 & 0xef);
		_mt_fe_tn_set_reg_tc3800(handle, 0x8a, buf1);
	}

	_mt_sleep(tWait2);

	if(handle->tuner_int_fast_set == 1)
	{
		_mt_fe_tn_set_reg_bit_tc3800(handle, 0x97, 0x03, 2, 0);
	}

	if (handle->tuner_clock_out == 1)
	{
		buf4 = (U8)(buf3 | 0x05);
	}
	else
	{
		buf4 = (U8)(buf3 | 0x04);
	}
	_mt_fe_tn_set_reg_tc3800(handle, 0x13, buf4);

	_mt_fe_tn_set_reg_tc3800(handle, 0x30, 0x00);

	if((handle->tuner_application == 1) || (handle->tuner_application == 3)|| (handle->tuner_application == 4))
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0x93);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x89, 0xb3);
	}

	_mt_fe_tn_set_reg_tc3800(handle, 0x9a, 0x93);


	if((handle->tuner_mode == 1) || (handle->tuner_mode == 2))
	{
		_mt_fe_tn_get_reg_tc3800(handle, 0xbd, &buf);

		if(buf == 0xff)
		{
			_mt_fe_tn_set_reg_tc3800(handle, 0xbd, 0x0f);
			_mt_fe_tn_get_reg_tc3800(handle, 0x8a, &buf);
			buf = (U8)(buf & 0x0f);
			if(buf > 9)
				_mt_fe_tn_set_reg_tc3800(handle, 0x14, 0x95);
			else if (buf > 6)
				_mt_fe_tn_set_reg_tc3800(handle, 0x14, 0x75);
			else if (buf > 3)
				_mt_fe_tn_set_reg_tc3800(handle, 0x14, 0x55);
			else
				_mt_fe_tn_set_reg_tc3800(handle, 0x14, 0x45);
		}
	}


	if(handle->tuner_mode == 0)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x04);
	}
	else if(handle->tuner_mode == 1)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x24);
	}
	else if(handle->tuner_mode == 2)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x3c);
		mt_fe_tn_sleep_tc3800(handle);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x0c, 0x04);
		mt_fe_tn_sleep_tc3800(handle);
	}

	if(handle->tuner_application == 3)
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x59, 0xb7);
	}
	else
	{
		_mt_fe_tn_set_reg_tc3800(handle, 0x59, 0x34);
	}
	
	return;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_set_BB_filter_band_tc3800									 */
/*****************************************************************************/
void mt_fe_tn_set_BB_filter_band_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U8 bandwidth)
{
	handle->tuner_bandwidth = bandwidth;

	_mt_fe_tn_set_BB_tc3800(handle);

	return;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_get_signal_strength_tc3800									 */
/*****************************************************************************/
static const S32 xm_list_dBm[16][16] =
{
	{   0,    0,  602,  954, 1204, 1398, 1556, 1690, 1806, 1908, 2000, 2083, 2158, 2228, 2292, 2352},
	{2408, 2461, 2511, 2558, 2602, 2644, 2685, 2723, 2760, 2796, 2830, 2863, 2894, 2925, 2954, 2983},
	{3010, 3037, 3063, 3088, 3113, 3136, 3160, 3182, 3204, 3226, 3246, 3267, 3287, 3306, 3326, 3344},
	{3362, 3380, 3398, 3415, 3432, 3449, 3465, 3481, 3496, 3512, 3527, 3542, 3556, 3571, 3585, 3599},
	{3612, 3626, 3639, 3652, 3665, 3678, 3690, 3703, 3715, 3727, 3738, 3750, 3762, 3773, 3784, 3795},
	{3806, 3817, 3828, 3838, 3849, 3859, 3869, 3879, 3889, 3899, 3908, 3918, 3928, 3937, 3946, 3955},
	{3965, 3974, 3982, 3991, 4000, 4009, 4017, 4026, 4034, 4042, 4051, 4059, 4067, 4075, 4083, 4091},
	{4098, 4106, 4114, 4121, 4129, 4136, 4144, 4151, 4158, 4166, 4173, 4180, 4187, 4194, 4201, 4208},
	{4214, 4221, 4228, 4235, 4241, 4248, 4254, 4261, 4267, 4273, 4280, 4286, 4292, 4298, 4305, 4311},
	{4317, 4323, 4329, 4335, 4341, 4346, 4352, 4358, 4364, 4369, 4375, 4381, 4386, 4392, 4397, 4403},
	{4408, 4414, 4419, 4424, 4430, 4435, 4440, 4445, 4451, 4456, 4461, 4466, 4471, 4476, 4481, 4486},
	{4491, 4496, 4501, 4506, 4511, 4515, 4520, 4525, 4530, 4534, 4539, 4544, 4548, 4553, 4558, 4562},
	{4567, 4571, 4576, 4580, 4585, 4589, 4593, 4598, 4602, 4606, 4611, 4615, 4619, 4624, 4628, 4632},
	{4636, 4640, 4644, 4649, 4653, 4657, 4661, 4665, 4669, 4673, 4677, 4681, 4685, 4689, 4693, 4697},
	{4700, 4704, 4708, 4712, 4716, 4720, 4723, 4727, 4731, 4735, 4738, 4742, 4746, 4749, 4753, 4757},
	{4760, 4764, 4768, 4771, 4775, 4778, 4782, 4785, 4789, 4792, 4796, 4799, 4803, 4806, 4810, 4813}
};


S32 mt_fe_tn_get_signal_strength_tc3800(MT_FE_Tuner_Handle_TC3800 handle)
{
	U8 buf = 0, bufH = 0, bufL = 0;

	S32 level = -107;
	S32 gain_x10 = 0;
	S32 gain_ref_x10 = 0;
	S32 pd_x10 = 0;
	S32 pdtref_x10 = -390;

	U32 freq_KHz = handle->tuner_freq;

	_mt_fe_tn_get_reg_tc3800(handle, 0x7e, &buf);
	if(buf == 0)
	{
		pdtref_x10 = -390;

		if(freq_KHz <= 100000)
			gain_ref_x10 = 520;
		else if(freq_KHz <= 200000)
			gain_ref_x10 = 520;
		else if(freq_KHz <= 300000)
			gain_ref_x10 = 530;
		else if(freq_KHz <= 335000)
			gain_ref_x10 = 530;
		else if(freq_KHz <= 400000)
			gain_ref_x10 = 520;
		else if(freq_KHz <= 450000)
			gain_ref_x10 = 530;
		else if(freq_KHz <= 500000)
			gain_ref_x10 = 540;
		else if(freq_KHz <= 600000)
			gain_ref_x10 = 545;
		else if(freq_KHz <= 700000)
			gain_ref_x10 = 555;
		else if(freq_KHz <= 750000)
			gain_ref_x10 = 560;
		else if(freq_KHz <= 800000)
			gain_ref_x10 = 560;
		else if(freq_KHz <= 900000)
			gain_ref_x10 = 570;
		else
			gain_ref_x10 = 565;
	}
	else
	{
		pdtref_x10 = -430;

		if (buf == 0x66)
		{
			pdtref_x10 += 5;
		}
		else if (buf == 0x55)
		{
			pdtref_x10 += 10;
		}
		else if (buf == 0x44)
		{
			pdtref_x10 += 15;
		}
		else if (buf == 0x33)
		{
			pdtref_x10 += 20;
		}
		else if (buf == 0x22)
		{
			pdtref_x10 += 25;
		}
		else
		{
			pdtref_x10 += 0;
		}

		if(freq_KHz <= 100000)
			gain_ref_x10 = 545;
		else if(freq_KHz <= 200000)
			gain_ref_x10 = 540;
		else if(freq_KHz <= 300000)
			gain_ref_x10 = 550;
		else if(freq_KHz <= 335000)
			gain_ref_x10 = 550;
		else if(freq_KHz <= 400000)
			gain_ref_x10 = 540;
		else if(freq_KHz <= 450000)
			gain_ref_x10 = 545;
		else if(freq_KHz <= 500000)
			gain_ref_x10 = 545;
		else if(freq_KHz <= 600000)
			gain_ref_x10 = 560;
		else if(freq_KHz <= 700000)
			gain_ref_x10 = 555;
		else if(freq_KHz <= 750000)
			gain_ref_x10 = 560;
		else if(freq_KHz <= 800000)
			gain_ref_x10 = 535;
		else if(freq_KHz <= 900000)
			gain_ref_x10 = 550;
		else
			gain_ref_x10 = 525;
	}

	_mt_fe_tn_get_reg_tc3800(handle, 0x5c, &buf);

	gain_x10 = gain_ref_x10 + 300 - 1350 + (buf*10);

	_mt_fe_tn_get_reg_tc3800(handle, 0xc8, &buf);
	bufH = (U8)(buf >> 4);
	bufL = (U8)(buf & 0x0f);

	pd_x10 = xm_list_dBm[bufH][bufL] / 10 + 5 + pdtref_x10;

	level = (pd_x10 - gain_x10)/10;

	if (handle->tuner_application == 4)
	{
		level -= 3;
	}

	return level;
}


/*****************************************************************************/
/*API: 	mt_fe_tn_get_diagnose_info_tc3800									 */
/*****************************************************************************/
void mt_fe_tn_get_diagnose_info_tc3800(MT_FE_Tuner_Handle_TC3800 handle, U32* data1, U32* data2)
{
	return;
}


HI_S32 m88tc3800_init_tuner(HI_U32 u32TunerPort)
{
   	HI_U32 u32StatTime = 0;
    HI_U32 u32EndTime = 0;
   
   g_m88tc3800_tuner_port = u32TunerPort;
   
	handle.tuner_init_OK = 0;

	mt_fe_tn_init_tc3800( &handle );
	
    return HI_SUCCESS;
}

HI_S32 m88tc3800_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF)
{
	U32 freq_KHz;
	
   	HI_U32 u32StatTime = 0;
    HI_U32 u32EndTime = 0;

	g_m88tc3800_tuner_port = u32TunerPort;
	g_m88tc3800_tuner_i2c_channel = enI2cChannel;
	
	freq_KHz = puRF;
	mt_fe_tn_set_freq_tc3800(&handle,freq_KHz);

    return HI_SUCCESS;
}

HI_S32 m88tc3800_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
	HI_U32 s32SignalStrength = 0;

	s32SignalStrength = mt_fe_tn_get_signal_strength_tc3800( &handle );

	s32SignalStrength = s32SignalStrength + 107;

	pu32SignalStrength[1] = (HI_U32)s32SignalStrength;
		 
    return HI_SUCCESS;
}

HI_VOID m88tc3800_tuner_resume (HI_U32 u32TunerPort)
{
    return;
}



