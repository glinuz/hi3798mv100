
/*************************************************************
** file name:	hd2501_api.c
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/

/* include -------------------------------------------------*/

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

#include "hd2501_type.h"
#include "hd2501_drv.h"

#include "linux/kernel.h"

/* macro & structure ---------------------------------------*/
/* global & static variables -------------------------------*/

HI_U32 				genI2cChannelHD2501;
static BOOL         ABSSdemodInitialized = FALSE;		/* initialization flag */

/* functions define ----------------------------------------*/

HI_S32 hd2501_demod_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    U8 ret;
	if (u32TunerPort >= TUNER_NUM)
	{
		return HI_FAILURE;
	}
    HDIC_Demod_Print("%s[%d]: enter\n", __FUNCTION__, __LINE__);

	genI2cChannelHD2501 = enI2cChannel;

	//HI_DRV_GPIOI2C_Init();
	//HI_DRV_GPIOI2C_CreateGpioI2c(&genI2cChannelHD2501, 12, 11);
	
    /* Check initialize */
    if( ABSSdemodInitialized == TRUE )
    {
        return	HI_SUCCESS;
    }

	ret = HDIC2501_Demod_AutoCheck() ;
    if( HDIC_NO_ERROR == ret )
    {
    	ret = HDIC2501_TunerAutoCheck(u32TunerPort);
        if( HDIC_NO_ERROR == ret )
        {
            ABSSdemodInitialized = TRUE;
    HDIC_Demod_Print("%s[%d]: exit\n", __FUNCTION__, __LINE__);

            return HI_SUCCESS;
        }
    }

	HDIC_Demod_Print("Demod auto check fail. ret = %d\n", ret);
	return HI_FAILURE;
	
}

HI_S32 hd2501_demod_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel)
{
    U8 CheckLockTemp = 0 ;
	U8 locked = 0 ;
	U8 Status = 0;
    U32 Frequency	= 0 ; //= 11830;//1M Uint
    U32	SymbolRate 	= 0 ;

    HDIC_Demod_Print("tuner start to work\n");
    Frequency = pstChannel->u32Frequency/1000;
	SymbolRate = pstChannel->unSRBW.u32SymbolRate;
	
    HDIC2501_Init();
	HDIC_Demod_Print("%s:%d\n",__FUNCTION__,__LINE__);
	
    //------------------------------------------
	// Set polar
	HDIC2501_SetPolar( pstChannel->enPolar ) ;
	
    //------------------------------------------
    //Please set the tuner parameters and lock the tuner
    //请设置Tuner参数，并锁定Tuner

    HDIC2501_SetTuner(Frequency, SymbolRate);
	HDIC_Demod_Print("%s:%d; Freq=%d, Symb=%d\n",__FUNCTION__,__LINE__, Frequency, SymbolRate);


    //------------------------------------------
    //Please set the demod parameters, please check the HDIC2501.h to get the Setting Value
    //请设置Demod参数，设置的参数请查询HDIC2501.h
    //Example:
    //自动,SymbolRate未使用,芯片可以自动侦测符号率
    //HDIC2501_SetParameters(SYMBOLRATEAUTODETECT_AUTO,0);
    //手动,SymbolRate传入,如知道符号率,用手动模式会加快锁频过程.
    //HDIC2501_SetParameters(SYMBOLRATEAUTODETECT_MANUAL,28800000);
    HDIC2501_SetParameters(SYMBOLRATEAUTODETECT_MANUAL,SymbolRate);
	HDIC_Demod_Print("%s:%d\n",__FUNCTION__,__LINE__);

    //------------------------------------------
    HDIC2501_SoftReset();
	HDIC_Demod_Print("%s:%d\n",__FUNCTION__,__LINE__);

    for (CheckLockTemp=0; CheckLockTemp<20; CheckLockTemp++)
    {
        HDIC2501_IsDemodLocked(&locked,&Status);
		HDIC_Demod_Print("%s:%d; lock=%x, Status=%x\n",__FUNCTION__,__LINE__, locked, Status);

        HDIC2501_Wait(100); /* wait 100 ms */

        if (locked==1)
        {

            HDIC_Demod_Print("\n\r [HDIC2501]: HDIC2501 demod lock ok!!! \n\r");
            //If necessery, invoke the HDIC2501_GetMode function to get the information
            //此时你可以读出信号具体格式,符号率等
            //HDIC2501_GetMode(&Constellation,&CodingRate, &ScramblerMode, &TSorGS,&RollOff,&OperationMode,&TS_ID);
            //HDIC2501_GetSymbolRate(&SymbolRate);
            //根据读出的符号率和滚降系数,可以去计算出新Tuner的SAW的配置.
            //SAW = HDIC2501_GetNewSAW(SymbolRate,Rolloff);
            //FEPrintf_("  [2501] 2501 is locked \n\r");
            return HI_SUCCESS;
        }
    }

	return HI_FAILURE;
}

HI_S32 hd2501_demod_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
	//U8 ret;
    U8 locked = 0 ;
	U8 Status = 0 ;

    HDIC2501_IsDemodLocked(&locked,&Status);

    HDIC_Demod_Print("\n\r%s:%d, Locked = %d, Status=0x%x \n\r", __FUNCTION__, __LINE__, locked, Status);

    if(0x01 == locked)
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    else
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;

	return HI_SUCCESS;
}

HI_S32 hd2501_demod_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
	*pu32ber = 0 ;

	HDIC2501_GetSignalBER( pu32ber ) ;
	
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
	U8 ret;
	U32 signal_quality;

    HDIC_Demod_Print("%s[%d]: enter\n", __FUNCTION__, __LINE__);

	ret = HDIC2501_GetSignalSNR(&signal_quality);

	if(ret == HDIC_NO_ERROR)
		*pu32SNR = signal_quality;
	else
		*pu32SNR = 0;

	return HI_SUCCESS;
}

HI_S32 hd2501_demod_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
	U8 ret = 0 ;
	U16 signalStrength = 0 ;

	ret = HDIC2501_GetFieldStrength(&signalStrength);

	if(ret == HDIC_NO_ERROR)
		pu32SignalStrength[1] = signalStrength;
	else
		pu32SignalStrength[0] = 0;

	return HI_SUCCESS;
}

HI_S32 hd2501_demod_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
	pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_ABSS;
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
    HI_S32 ret = HI_SUCCESS ;
    
	if(u32Standby == 1)
	{
        ret = HDIC2501_Standby() ;
    }
    else
    {
        ret = HDIC2501_WakeUp() ;
    }
    
	if(ret == HDIC_NO_ERROR)
		return HI_SUCCESS;
	else
		return HI_FAILURE;
}

HI_S32 hd2501_demod_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode)
{
	return HI_SUCCESS;
}

HI_S32 hd2501_demod_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
	return HI_SUCCESS;
}




