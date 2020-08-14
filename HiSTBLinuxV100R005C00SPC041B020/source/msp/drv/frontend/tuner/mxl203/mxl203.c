
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>

#include <hi_debug.h>
#include "hi_type.h"
#include "drv_i2c_ext.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

#include "MxL203RF_API.h"


//#define MXL203_I2C_ADDR  0xC0   /* 0xC2 0xC4 0xC6 */
#define PLL_STEP 62500

#if 1
ShiftLUT SHIFT_LUT8[]=
{
	{331000000, 0x18, 0x04},
	{338000000, 0x2C, 0x04},
	{339000000, 0xB4, 0x07},
	{346000000, 0xD4, 0x07},
	{363000000, 0xD0, 0x07},
	{394000000, 0x2C, 0x04},
	{403000000, 0x62, 0x04},
	{410000000, 0x2C, 0x04},
	{411000000, 0xD4, 0x07},
	{418000000, 0xD4, 0x07},
	{434000000, 0xD4, 0x07},
	{435000000, 0xD4, 0x07},
	{466000000, 0x2C, 0x04},
	{506000000, 0xD4, 0x07},
	{538000000, 0x2C, 0x04},
	{578000000, 0xD4, 0x07},
	{610000000, 0x2C, 0x04},
	{650000000, 0xD4, 0x07},
	{682000000, 0x3C, 0x04},
	{722000000, 0xCA, 0x07},
	{754000000, 0x34, 0x04},
	{794000000, 0xCC, 0x07},
	{826000000, 0x34, 0x04},
	{866000000, 0xCC, 0x07},
	{-1, 0, 0}
};

ShiftLUT SHIFT_LUT6[]=
{
	{339000000, 0xD4, 0x07},
	{345000000, 0x2C, 0x04},
	{357000000, 0x2C, 0x04},
	{363000000, 0xD2, 0x07},
	{375000000, 0xD4, 0x07},
	{381000000, 0x2C, 0x04},
	{393000000, 0x2C, 0x04},
	{399000000, 0xD4, 0x07},
	{411000000, 0xD4, 0x07},
	{417000000, 0x2C, 0x04},
	{429000000, 0x2C, 0x04},
	{435000000, 0xD4, 0x07},
	{447000000, 0xD4, 0x07},
	{453000000, 0x2C, 0x04},
	{465000000, 0x2C, 0x04},
	{471000000, 0xD4, 0x07},
	{501000000, 0x2C, 0x04},
	{507000000, 0xD4, 0x07},
	{537000000, 0x2C, 0x04},
	{543000000, 0xD4, 0x07},
	{573000000, 0x2C, 0x04},
	{579000000, 0xD4, 0x07},
	{609000000, 0x2C, 0x04},
	{615000000, 0xD4, 0x07},
	{645000000, 0x37, 0x04},
	{651000000, 0xCA, 0x07},
	{681000000, 0x34, 0x04},
	{687000000, 0xCC, 0x07},
	{717000000, 0x2C, 0x04},
	{723000000, 0xD4, 0x07},
	{753000000, 0x2C, 0x04},
	{759000000, 0xD4, 0x07},
	{789000000, 0x2C, 0x04},
	{795000000, 0xD4, 0x07},
	{825000000, 0x34, 0x04},
	{831000000, 0xCC, 0x07},
	{861000000, 0x34, 0x04},
	{867000000, 0xCC, 0x07},
	{-1, 0, 0}
};
#endif

HI_S32 mxl203_set_tuner(HI_U32 u32TunerPort, HI_U8 i2c_channel, HI_U32 puRF)
{
    MxL_ERR_MSG Ret;
    MxL203RF_TunerConfigS stMxlTuner;
    HI_U32 u32FreqInHz = puRF * 1000;

    /* config tuner port */
    MxL_I2C_SetPort(u32TunerPort);
    /* config i2c channel */
    MxL_I2C_SetChn(i2c_channel);

    /* initial tuner */
    stMxlTuner.I2C_Addr = g_stTunerOps[u32TunerPort].u32TunerAddress;
    stMxlTuner.Mode = MxL_MODE_CAB_STD;
    stMxlTuner.Xtal_Freq = MxL_XTAL_24_MHZ;
    stMxlTuner.IF_Freq = MxL_IF_7_2_MHZ;
    stMxlTuner.IF_Spectrum = MxL_NORMAL_IF;
    stMxlTuner.ClkOut_Setting = MxL_CLKOUT_DISABLE;
    stMxlTuner.ClkOut_Amp = MxL_CLKOUT_AMP_0;
    stMxlTuner.BW_MHz = MxL_BW_8MHz;
    stMxlTuner.Xtal_Cap = MxL_XTAL_CAP_12_PF;
   
    Ret = MxL_Tuner_Init(&stMxlTuner);
    if (MxL_OK != Ret)
    {
        HI_FATAL_TUNER("MxL_Tuner_Init failed:%x\n",Ret);    
        return HI_FAILURE;
    }
    
    MxL_Enable_LT(&stMxlTuner,1);

    if ( HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType )
    {
		if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
		{
        	u32FreqInHz += PLL_STEP*2;
        }
        else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
        {
        	u32FreqInHz += PLL_STEP*1/2;
        }
        else
        {
        	u32FreqInHz += PLL_STEP*1/2;
        }
    }

    /* set tuner */
    Ret = MxL_Tuner_RFTune(&stMxlTuner,u32FreqInHz,MxL_BW_8MHz);
    if (MxL_OK != Ret)
    {
        HI_FATAL_TUNER("MxL_Tuner_RFTune failed:%x\n",Ret);    
        return HI_FAILURE;
    }

	return HI_SUCCESS; 
}

