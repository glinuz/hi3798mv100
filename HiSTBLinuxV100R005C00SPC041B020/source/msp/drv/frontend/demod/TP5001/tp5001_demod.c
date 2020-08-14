#include <linux/delay.h>


#include "hi_type.h"
//#include "hi_drv_i2c.h"
#include "hi_drv_gpioi2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "drv_demod.h"

#include "TP5001.h"
#if defined(TUNER_DEV_TYPE_RDA5815)
#include "RDA5815.h"
#endif

#include "linux/kernel.h"

// ==========================================================================================================
// Marco & Global Variable
// ==========================================================================================================
HI_U32 genI2cChannel;
HI_U32 gTP5001_addr;
HI_U32 gTP5001_tuner_addr;

// ==========================================================================================================
// Internal Function
// ==========================================================================================================


// ==========================================================================================================
// External Function
// ==========================================================================================================
TP_UINT8 TP_iic_write(TP_UINT8 device_address, TP_UINT16 register_address, TP_UINT8 * value_buffer, TP_UINT32 length)
{
	HI_S32 ret;
    
    if(HI_STD_I2C_NUM > genI2cChannel)
    {
        ret = HI_DRV_I2C_Write(genI2cChannel, device_address, register_address, 2, value_buffer, length);
    }
    else
    {
    	ret = HI_DRV_GPIOI2C_WriteExt(genI2cChannel, device_address, register_address, 2, value_buffer, length);
    }
    
	if(HI_SUCCESS == ret)
		return TP_SUCCESS;
	else
		return TP_IIC_WRERR;
}


TP_UINT8 TP_iic_read(TP_UINT8 device_address, TP_UINT16 register_address, TP_UINT8 * value_buffer, TP_UINT32 length)
{
	HI_S32 ret;
    if(HI_STD_I2C_NUM > genI2cChannel)
    {
        ret = HI_DRV_I2C_Read(genI2cChannel, device_address, register_address, 2, value_buffer, length);
    }
    else
    {
    	ret = HI_DRV_GPIOI2C_ReadExt(genI2cChannel, device_address, register_address, 2, value_buffer, length);
    }

	if(HI_SUCCESS == ret)
		return TP_SUCCESS;
	else
		return TP_IIC_WRERR;
}


void TP_Delay( TP_UINT32 uiMS )
{
	mdelay(2*uiMS); //dongfuhai
	return;
}

HI_S32 tp5001_demod_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
	TP_UINT8 ret;
    TP_UINT8 tuner_type;
	if (u32TunerPort >= TUNER_NUM)
	{
		return HI_FAILURE;
	}

	genI2cChannel = enI2cChannel;
    gTP5001_addr = g_stTunerOps[u32TunerPort].u32DemodAddress;
    gTP5001_tuner_addr = g_stTunerOps[u32TunerPort].u32TunerAddress;
	//HI_DRV_GPIOI2C_CreateGpioI2c(&genI2cChannel, 12, 11);
	
    switch(enTunerDevType)
    {
        case HI_UNF_TUNER_DEV_TYPE_RDA5815:
            tuner_type = RDA_5815M_TUNER;
            break;
        default:
            tuner_type = RDA_5815M_TUNER;
            break;
    }
	ret = TP_init(tuner_type);
	printk("\nafter TP_init ret is %d", ret);
	if(TP_SUCCESS != ret)
	{
		return HI_FAILURE;
	}
#if defined(TUNER_DEV_TYPE_RDA5815)
    ret = RDA5815_init_tuner(u32TunerPort);
    if(ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
	
}

HI_S32 tp5001_demod_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel)
{
	unsigned char ret;
#if defined(TUNER_DEV_TYPE_RDA5815)
    ret = RDA5815_set_tuner(u32TunerPort,genI2cChannel,pstChannel->u32Frequency);
    if(ret != HI_SUCCESS)
   {
        return HI_FAILURE;
    }
#else
	//ret = TP_set_rf_tuner(pstChannel->u32Frequency/1000, pstChannel->unSRBW.u32SymbolRate);
	//if(ret != TP_SUCCESS)
	//	return HI_FAILURE;

#endif


	ret = TP_set_symbol_rate(pstChannel->unSRBW.u32SymbolRate);
	if(ret != TP_SUCCESS)
		return HI_FAILURE;

	if(HI_UNF_TUNER_FE_POLARIZATION_R == pstChannel->enPolar)
	{
        HI_INFO_TUNER("---->HI_UNF_TUNER_FE_POLARIZATION_R\n");
		ret = TP_set_rf_pola(RA_Normal);
		if(ret != TP_SUCCESS)
			return HI_FAILURE;
	}
	else if(HI_UNF_TUNER_FE_POLARIZATION_L == pstChannel->enPolar)
	{
        HI_INFO_TUNER("---->HI_UNF_TUNER_FE_POLARIZATION_L\n");
		ret = TP_set_rf_pola(RA_Invert);
		if(ret != TP_SUCCESS)
			return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
	TP_UINT8 ret;

	ret = TP_get_lock_status();
	if(TP_SUCCESS == ret)
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
	else
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;

	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
	int ret;
	TP_UINT64 ber_total, ber_err, bler_total, bler_err;

	ret = TP_get_statistic_ber_bler(&ber_total, &ber_err, &bler_total, &bler_err);

	if(ret == TP_SUCCESS)
	{
		pu32ber[0] = bler_total;
		pu32ber[1] = bler_err;
		pu32ber[2] = 0;
	}
	else
	{
		pu32ber[0] = pu32ber[1] = pu32ber[2] = 0;
	}

	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
	TP_UINT8 ret;
	TP_UINT8 signal_quality_DB;

	ret = TP_get_signal_quality_DB(&signal_quality_DB);

	if(ret == TP_SUCCESS)
		*pu32SNR = signal_quality_DB;
	else
		*pu32SNR = 0;

	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_signal_quality(HI_U32 u32TunerPort, HI_U32* pu32SignalQuality)
{
    TP_UINT8 ret;
        TP_UINT8 signal_quality;
    
        ret = TP_get_signal_quality(&signal_quality);
    
        if(ret == TP_SUCCESS)
            *pu32SignalQuality = signal_quality;
        else
            *pu32SignalQuality = 0;

        return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
	TP_UINT8 ret;
	TP_UINT8 signalStrength;

	ret = TP_get_signal_strength(&signalStrength);

	if(ret == TP_SUCCESS)
		pu32SignalStrength[1] = signalStrength;
	else
		pu32SignalStrength[1] = 0;

	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
	pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_ABSS;
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_demod_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
	int ret;
	
	if(u32Standby == 1)
		ret = TP_Standby();
	else
		ret = TP_Resume();
	if(ret == TP_SUCCESS)
		return HI_SUCCESS;
	else
		return HI_FAILURE;
}

HI_S32 tp5001_demod_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode)
{
	return HI_SUCCESS;
}

HI_S32 tp5001_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
	return HI_SUCCESS;
}




