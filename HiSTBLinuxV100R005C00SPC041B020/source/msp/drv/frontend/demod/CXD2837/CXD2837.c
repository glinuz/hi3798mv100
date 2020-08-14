#include <linux/module.h>
#include <linux/semaphore.h>

#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_drv_tuner.h"
#include "drv_tuner_ioctl.h"
#include "drv_demod.h"

#include "sony_common.h"
#include "sony_i2c.h"
#include "sony_integ.h"
#include "sony_demod.h"
#include "sony_tuner_terr_cable.h"
#include "sony_ascot2e.h"
#include "sony_tuner_ascot2e.h"
#include "sony_demod_dvbc.h"
#include "sony_demod_dvbt.h"
#include "sony_demod_dvbt2.h"
#include "sony_integ_dvbc.h"
#include "sony_integ_dvbt_t2.h"
#include "sony_demod_dvbt2_monitor.h"
#include "sony_demod_dvbt_monitor.h"
#include "sony_demod_dvbc_monitor.h"

#include "hi_kernel_adapt.h"

#define MAX_TUNER (3)

static sony_integ_t integ;
static sony_demod_t demod;
static sony_tuner_terr_cable_t tunerTerrCable;
//sony_ascot2d_t ascot2d;
static sony_ascot2e_t ascot2e;
sony_i2c_t i2c;

extern sony_result_t hi_i2c_read_sony(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData, uint32_t size, uint8_t mode);
extern sony_result_t hi_i2c_write_sony(struct sony_i2c_t* pI2c, uint8_t deviceAddress, const uint8_t *pData, uint32_t size, uint8_t mode);

HI_S32 cxd2837_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel,HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    sony_result_t result = SONY_RESULT_OK;
	uint8_t xtalFreqMHz = 16;
	uint8_t i2cAddress;
	uint32_t configFlags = 0;
	sony_demod_xtal_t xtalFreq;

	/* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    i2c.gwAddress = 0x00;                                   /* N/A */
    i2c.gwSub = 0x00;                                       /* N/A */
    i2c.Read = hi_i2c_read_sony;                           /* Base level HW interfacing I2C read function */
    i2c.Write = hi_i2c_write_sony;                         /* Base level HW interfacing I2C write function */
    i2c.ReadRegister = sony_i2c_CommonReadRegister;         /* Common wrapper function for multi byte Read operation */
    i2c.WriteRegister = sony_i2c_CommonWriteRegister;       /* Common wrapper function for multi byte Write operation */
    i2c.WriteOneRegister = sony_i2c_CommonWriteOneRegister; /* Common wrapper function for single byte Write operation */
    i2c.user = (void*)&u32TunerPort;                             /* I2C driver instance */


    //uint8_t xtalFreqMHz = 41;
    xtalFreqMHz = 16;
    //uint8_t i2cAddress = SONY_ASCOT2D_ADDRESS;
    i2cAddress = g_stTunerOps[u32TunerPort].u32TunerAddress; //SONY_ASCOT2E_ADDRESS;
    configFlags = 0;

    //result = sony_tuner_ascot2d_Create (&tunerTerrCable, xtalFreqMHz, i2cAddress, &i2c, configFlags, &ascot2d);
    result = sony_tuner_ascot2e_Create (&tunerTerrCable, xtalFreqMHz, i2cAddress, &i2c, configFlags, &ascot2e);
    if (result == SONY_RESULT_OK)
    {
        HI_INFO_TUNER (" Tuner Created with the following parameters:\n");
        HI_INFO_TUNER ("  - Tuner Type     : CXD2861 (ASCOT2D) \n");
        HI_INFO_TUNER ("  - XTal Frequency : %uMHz\n", xtalFreqMHz);
        HI_INFO_TUNER ("  - I2C Address    : %u\n", i2cAddress);
        HI_INFO_TUNER ("  - Config Flags   : %u\n\n", configFlags);
    }
    else
    {
        //HI_INFO_TUNER (" Error: Unable to create Sony ASCOT2D tuner driver. (result = %s)\n", Common_Result[result]);
        return -1;
    }


    /* Modify the following to suit your implementation */
    //sony_demod_xtal_t xtalFreq = SONY_DEMOD_XTAL_41000KHz;
    xtalFreq = SONY_DEMOD_XTAL_20500KHz;
    i2cAddress = g_stTunerOps[u32TunerPort].u32DemodAddress;//0xCA;

    /* Create parameters for integration structure:
     *  sony_integ_t * pInteg                       Integration object
     *  sony_demod_xtal_t xtalFreq                  Demodulator xTal frequency
     *  uint8_t i2cAddress                          Demodulator I2C address
     *  sony_i2c_t i2c                              Demodulator I2C driver
     *  sony_demod_t *pDemod                        Demodulator object
     *
     *  Note: Set the following to NULL to disable control
     *  sony_tuner_terr_cable_t * pTunerTerrCable   Terrestrial / Cable tuner object
     *  sony_tuner_sat_t * pTunerSat                Satellite tuner object
     *  sony_lnbc_t * pLnbc                         LNB Controller object
     */
    result = sony_integ_Create (&integ, xtalFreq, i2cAddress, &i2c, &demod
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
        /* Terrestrial and Cable supported so include the tuner object into the Create API */
        ,&tunerTerrCable
#endif
        );

    if (result == SONY_RESULT_OK)
    {
        HI_INFO_TUNER (" Demod Created with the following parameters:\n");
        //HI_INFO_TUNER ("  - XTal Frequency : %s\n", Common_DemodXtal[xtalFreq]);
        HI_INFO_TUNER ("  - I2C Address    : %u\n\n", i2cAddress);
    }
    else
    {
        //HI_INFO_TUNER (" Error: Unable to create demodulator driver. (result = %s)\n", Common_Result[result]);
        return -1;
    }

    result = sony_integ_InitializeT_C (&integ);
    if (result == SONY_RESULT_OK)
    {
        HI_INFO_TUNER (" Driver initialized, current state = SONY_DEMOD_STATE_SLEEP_T_C\n\n");
    }
    else
    {
        //HI_INFO_TUNER (" Error: Unable to initialise the integration driver to terrestiral / cable mode. (result = %s)\n", Common_Result[result]);
        return -1;
    }

    /* ---------------------------------------------------------------------------------
     * Configure the Demodulator
     * ------------------------------------------------------------------------------ */
#if 0
    /* DVB-T demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBT_5 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT_7 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.20);
    demod.iffreqConfig.configDVBT_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);

    /* DVB-T2 demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBT2_5 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT2_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT2_7 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.20);
    demod.iffreqConfig.configDVBT2_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);

    /* DVB-C demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBC = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.90);

    /* DVB-C2 demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBC2_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBC2_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);
#endif

    /* DVB-T demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBT_5 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT_7 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.20);
    demod.iffreqConfig.configDVBT_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);

    /* DVB-T2 demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBT2_5 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT2_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBT2_7 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.20);
    demod.iffreqConfig.configDVBT2_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);

    /* DVB-C demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBC = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.90);

    /* DVB-C2 demodulator IF configuration for ASCOT2D Low IF tuner */
    demod.iffreqConfig.configDVBC2_6 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (3.60);
    demod.iffreqConfig.configDVBC2_8 = SONY_DEMOD_MAKE_IFFREQ_CONFIG (4.80);

    demod.tunerOptimize = SONY_DEMOD_TUNER_OPTIMIZE_ASCOT2E;

	return HI_SUCCESS;
}

HI_S32 cxd2837_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
	return HI_SUCCESS;
}

HI_S32 cxd2837_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    sony_dvbc_tune_param_t tuneParam_c;
    sony_dvbt_tune_param_t tuneParam_t;
    sony_dvbt2_tune_param_t tuneParam_t2;
    sony_result_t tuneResult = SONY_RESULT_OK;

	HI_TUNER_CHECKPOINTER(pstChannel);
	/* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
    //cable
    case HI_UNF_TUNER_SIG_TYPE_CAB:
        /* Configure the DVBC tune parameters based on the channel requirements */
        tuneParam_c.centerFreqKHz = pstChannel->u32Frequency;    /* Channel center frequency in KHz */

        HI_INFO_TUNER (" Tune to DVB-C signal with the following parameters:\n");
        HI_INFO_TUNER ("  - Center Frequency    : %uKHz\n", tuneParam_c.centerFreqKHz);


        /* Perform DVBC Tune */
        tuneResult = sony_integ_dvbc_Tune (&integ, &tuneParam_c);
        HI_INFO_TUNER ("  - Result              : %d\n\n", tuneResult);
        break;

    //DVB-T
    case HI_UNF_TUNER_SIG_TYPE_DVB_T:
        switch (pstChannel->unSRBW.u32BandWidth)
        {
	        case 5000:
	            tuneParam_t.bandwidth = SONY_DEMOD_BW_5_MHZ;
	            break;

	        case 6000:
	            tuneParam_t.bandwidth = SONY_DEMOD_BW_6_MHZ;
	            break;

	        case 7000:
	            tuneParam_t.bandwidth = SONY_DEMOD_BW_7_MHZ;
	            break;

	        case 8000:
	            tuneParam_t.bandwidth = SONY_DEMOD_BW_8_MHZ;
	            break;

	        default:
	            return HI_FAILURE;
        }

        tuneParam_t.centerFreqKHz = pstChannel->u32Frequency;                   /* Channel centre frequency in KHz */
        tuneParam_t.profile = SONY_DVBT_PROFILE_HP;           /* Channel profile for hierachical modes.  For non-hierachical use HP */

        HI_INFO_TUNER (" Tune to DVB-T signal with the following parameters:\n");
        HI_INFO_TUNER ("  - Center Freq    : %uKHz\n", tuneParam_t.centerFreqKHz);
        HI_INFO_TUNER ("  - Bandwidth      : %d\n", tuneParam_t.bandwidth);
        HI_INFO_TUNER ("  - Profile        : %d\n", tuneParam_t.profile);

        /* Perform DVBT Tune */
        tuneResult = sony_integ_dvbt_Tune (&integ, &tuneParam_t);
        HI_INFO_TUNER ("  - Result         : %d\n\n", tuneResult);
        	break;
		case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
			switch (pstChannel->unSRBW.u32BandWidth)
	        {
	        case 1700:
	            tuneParam_t2.bandwidth = SONY_DEMOD_BW_1_7_MHZ;
	            break;

	        case 5000:
	            tuneParam_t2.bandwidth = SONY_DEMOD_BW_5_MHZ;
	            break;

	        case 6000:
	            tuneParam_t2.bandwidth = SONY_DEMOD_BW_6_MHZ;
	            break;

	        case 7000:
	            tuneParam_t2.bandwidth = SONY_DEMOD_BW_7_MHZ;
	            break;

	        case 8000:
	            tuneParam_t2.bandwidth = SONY_DEMOD_BW_8_MHZ;
	            break;

	        default:
	            return HI_FAILURE;
	        }

	        tuneParam_t2.centerFreqKHz = pstChannel->u32Frequency;                   /* Channel center frequency in KHz */
	        tuneParam_t2.dataPlpId = 0;                            /* PLP ID where multiple PLP's are available */
	        /* Additional tune information fed back from the driver.  This parameter should be checked
	        if the result from the tune call is SONY_RESULT_OK_CONFIRM. */
	        tuneParam_t2.tuneInfo = SONY_DEMOD_DVBT2_TUNE_INFO_OK;

	        HI_INFO_TUNER (" Tune to DVB-T2 signal with the following parameters:\n");
	        HI_INFO_TUNER ("  - Center Freq    : %uKHz\n", tuneParam_t2.centerFreqKHz);
	        HI_INFO_TUNER ("  - Bandwidth      : %d\n", tuneParam_t2.bandwidth);
	        HI_INFO_TUNER ("  - PLP ID         : %u\n", tuneParam_t2.dataPlpId);

	        /* Perform DVBT2 Tune */
	        tuneResult = sony_integ_dvbt2_Tune (&integ, &tuneParam_t2);
	        HI_INFO_TUNER ("  - Result         : %d\n", tuneResult);
			break;
		default:
			HI_ERR_TUNER("signal type cxd2837 not support.\n");
        	return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 cxd2837_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_demod_lock_result_t Lock_Result = SONY_DEMOD_LOCK_RESULT_NOTDETECT;

    HI_TUNER_CHECKPOINTER(penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
            result = sony_integ_dvbc_WaitTSLock(&integ);
            if(result == SONY_RESULT_OK)
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
            }
            else
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            sony_demod_dvbt_CheckTSLock(&demod,&Lock_Result);
            if(Lock_Result == SONY_DEMOD_LOCK_RESULT_LOCKED)
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
            }
            else
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            result = sony_integ_dvbt_t2_WaitTSLock(&integ);
            if(result == SONY_RESULT_OK)
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
            }
            else
            {
            	*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            }
            break;
        default:
            return HI_FAILURE;

    }

    return HI_SUCCESS;
}

HI_S32 cxd2837_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    uint32_t u32Ber_Value=0;
    sony_result_t result;

    HI_TUNER_CHECKPOINTER(pu32ber);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
            result = sony_demod_dvbc_monitor_PreRSBER(&demod,&u32Ber_Value);
            if(result == SONY_RESULT_OK)
            {
            	*pu32ber = u32Ber_Value;
            }
            else
            {
            	return result;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            result = sony_demod_dvbt_monitor_PreRSBER(&demod,&u32Ber_Value);
            if(result == SONY_RESULT_OK)
            {
            	*pu32ber = u32Ber_Value;
            }
            else
            {
            	return result;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            result = sony_demod_dvbt2_monitor_PreBCHBER(&demod,&u32Ber_Value);
            if(result == SONY_RESULT_OK)
            {
            	*pu32ber = u32Ber_Value;
            }
            else
            {
            	return result;
            }
            break;
        default:
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 cxd2837_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    int32_t s32Snr=0;

    HI_TUNER_CHECKPOINTER(pu32SNR);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
        	sony_demod_dvbc_monitor_SNR(&demod,&s32Snr);
        	break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
        	sony_demod_dvbt_monitor_SNR(&demod,&s32Snr);
        	break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
        	sony_demod_dvbt2_monitor_SNR(&demod,&s32Snr);
        	break;
        default:
        	return HI_FAILURE;
    }

    *pu32SNR = (HI_U32)s32Snr;

    return HI_SUCCESS;
}

HI_S32 cxd2837_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    uint8_t u8Strength=0;

    HI_TUNER_CHECKPOINTER(pu32SignalStrength);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
        	break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
        	sony_integ_dvbt_monitor_SSI(&integ,&u8Strength);
        	break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
        	sony_integ_dvbt2_monitor_SSI(&integ,&u8Strength);
        	break;
        default:
        	return HI_FAILURE;
    }

    pu32SignalStrength[1] =(HI_U32)(u8Strength);
    return HI_SUCCESS;
}

HI_S32 cxd2837_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /*ts type control*/
    switch (enTsType)
    {
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
        case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
            {
                sony_demod_SetConfig(&demod,SONY_DEMOD_CONFIG_PARALLEL_SEL,1);
                break;
            }
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
            sony_demod_SetConfig(&demod,SONY_DEMOD_CONFIG_PARALLEL_SEL,0);
            sony_demod_SetConfig(&demod,SONY_DEMOD_CONFIG_TSVALID_MASK,0);
            sony_demod_SetConfig(&demod,SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ,1);
            break;
		default:
			HI_ERR_TUNER("ts type not support.\n");
        	return HI_FAILURE;
    }

    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}

HI_S32 cxd2837_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    sony_dvbt_tpsinfo_t Tps_Info;
    sony_result_t result;
    sony_dvbc_constellation_t Qam = SONY_DVBC_CONSTELLATION_64QAM;
    sony_dvbt2_plp_btype_t Plp_Type = SONY_DVBT2_PLP_DATA;
    sony_dvbt2_plp_t Plp;
    sony_dvbt2_ofdm_t Ofdm;

    HI_TUNER_CHECKPOINTER(pstInfo);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
            pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
            pstInfo->unSignalInfo.stTer.u32Freq = tunerTerrCable.frequencyKHz;

            sony_demod_dvbc_monitor_QAM(&demod, &Qam);
            if (Qam == SONY_DVBC_CONSTELLATION_16QAM)
            {
                pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            }
            else if (Qam == SONY_DVBC_CONSTELLATION_32QAM)
            {
                pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_32;
            }
            else if (Qam == SONY_DVBC_CONSTELLATION_64QAM)
            {
                pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            }
            else if (Qam == SONY_DVBC_CONSTELLATION_128QAM)
            {
                pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_128;
            }
            else if (Qam == SONY_DVBC_CONSTELLATION_256QAM)
            {
                pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_256;
            }
            break;
    	case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T;
            pstInfo->unSignalInfo.stTer.u32Freq = tunerTerrCable.frequencyKHz;
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;

            if (integ.pDemod->bandwidth == SONY_DEMOD_BW_1_7_MHZ)
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 1700;
            }
            else if(integ.pDemod->bandwidth == SONY_DEMOD_BW_5_MHZ)
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 5000;
            }
            else if(integ.pDemod->bandwidth == SONY_DEMOD_BW_6_MHZ)
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 6000;
            }
            else if(integ.pDemod->bandwidth == SONY_DEMOD_BW_7_MHZ)
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 7000;
            }
            else if(integ.pDemod->bandwidth == SONY_DEMOD_BW_8_MHZ)
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 8000;
            }
            else
            {
                pstInfo->unSignalInfo.stTer.u32BandWidth = 0;
            }

            result = sony_demod_dvbt_monitor_TPSInfo(&demod,&Tps_Info);
            if(result == SONY_RESULT_OK)
            {
                /*check tuner modulation type*/
                if(Tps_Info.constellation == SONY_DVBT_CONSTELLATION_QPSK)
                {
                    pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
                }
                else if(Tps_Info.constellation == SONY_DVBT_CONSTELLATION_16QAM)
                {
                    pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
                }
                else if(Tps_Info.constellation == SONY_DVBT_CONSTELLATION_64QAM)
                {
                    pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
                }
                else
                {
                    pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_DEFAULT;
                }

                /*check the guard*/
                if(Tps_Info.guard == SONY_DVBT_GUARD_1_32)
                {
                    pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
                }
                else if(Tps_Info.guard == SONY_DVBT_GUARD_1_16)
                {
                    pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
                }
                else if(Tps_Info.guard == SONY_DVBT_GUARD_1_8)
                {
                    pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
                }
                else if(Tps_Info.guard == SONY_DVBT_GUARD_1_4)
                {
                    pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
                }

                /*check FFT mode*/
                if(Tps_Info.mode == SONY_DVBT_MODE_2K)
                {
                    pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
                }
                else if(Tps_Info.mode == SONY_DVBT_MODE_8K)
                {
                    pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
                }
                else
                {
                    pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_DEFAULT;
                }

                /*check hierarchy*/
                if(Tps_Info.hierarchy == SONY_DVBT_HIERARCHY_1)
                {
                    pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA1;
                }
                else if(Tps_Info.hierarchy == SONY_DVBT_HIERARCHY_2)
                {
                    pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA2;
                }
                else if(Tps_Info.hierarchy == SONY_DVBT_HIERARCHY_4)
                {
                    pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA4;
                }
                else
                {
                    pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_DEFAULT;
                }

                /*check fec rate*/
                if(Tps_Info.rateHP == SONY_DVBT_CODERATE_1_2)
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                }
                else if(Tps_Info.rateHP == SONY_DVBT_CODERATE_2_3)
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                }
                else if(Tps_Info.rateHP == SONY_DVBT_CODERATE_3_4)
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                }
                else if(Tps_Info.rateHP == SONY_DVBT_CODERATE_5_6)
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                }
                else if(Tps_Info.rateHP == SONY_DVBT_CODERATE_7_8)
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                }
                else
                {
                    pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_AUTO;
                }
            }
            else
            {

            }
            break;
    	case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T2;
            pstInfo->unSignalInfo.stTer.u32Freq = tunerTerrCable.frequencyKHz;
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;

            result = sony_demod_dvbt2_monitor_ActivePLP(&demod,Plp_Type,&Plp);
            if(result == SONY_RESULT_OK)
            {
                switch(Plp.constell)
                {
                    case SONY_DVBT2_QPSK:
                        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
                        break;
                    case SONY_DVBT2_QAM16:
                        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
                        break;
                    case SONY_DVBT2_QAM64:
                        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
                        break;
                    case SONY_DVBT2_QAM256:
                        pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_256;
                        break;
                    default:
                        break;
                    	//return HI_FAILURE;
                }

                switch(Plp.fec)
                {
                    case SONY_DVBT2_FEC_LDPC_16K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_16K;
                        break;
                    case SONY_DVBT2_FEC_LDPC_64K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_64K;
                        break;
                    default:
                    	break;
                }

                switch(Plp.plpCr)
                {
                    case SONY_DVBT2_R1_2:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                        break;
                    case SONY_DVBT2_R3_5:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_5;
                        break;
                    case SONY_DVBT2_R2_3:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    case SONY_DVBT2_R3_4:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    case SONY_DVBT2_R4_5:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
                        break;
                    case SONY_DVBT2_R5_6:
                        pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    default:
                        break;
                }
            }
            else
            {
                return HI_FAILURE;
            }

            result = sony_demod_dvbt2_monitor_Ofdm(&demod,&Ofdm);
            if(result == SONY_RESULT_OK)
            {
                switch(Ofdm.gi)
                {
                    case SONY_DVBT2_G1_32:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
                        break;
                    case SONY_DVBT2_G1_16:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
                        break;
                    case SONY_DVBT2_G1_8:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
                        break;
                    case SONY_DVBT2_G1_4:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
                        break;
                    case SONY_DVBT2_G1_128:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_128;
                        break;
                    case SONY_DVBT2_G19_128:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_128;
                        break;
                    case SONY_DVBT2_G19_256:
                        pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_256;
                        break;
                    default:
                        break;
                }

                switch(Ofdm.mode)
                {
                    case SONY_DVBT2_M2K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
                        break;
                    case SONY_DVBT2_M8K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
                        break;
                    case SONY_DVBT2_M4K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
                        break;
                    case SONY_DVBT2_M1K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_1K;
                        break;
                    case SONY_DVBT2_M16K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_16K;
                        break;
                    case SONY_DVBT2_M32K:
                        pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_32K;
                        break;
                    default:
                        break;

                }
            }

            break;
    	default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 cxd2837_setplpid(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode)
{
    sony_dvbt2_tune_param_t tuneParam_t2;
    sony_result_t result = SONY_RESULT_OK;

    /* Check tuner port. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        switch (g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth)
        {
            case 1700:
                tuneParam_t2.bandwidth = SONY_DEMOD_BW_1_7_MHZ;
                break;

            case 5000:
                tuneParam_t2.bandwidth = SONY_DEMOD_BW_5_MHZ;
                break;

            case 6000:
                tuneParam_t2.bandwidth = SONY_DEMOD_BW_6_MHZ;
                break;

            case 7000:
                tuneParam_t2.bandwidth = SONY_DEMOD_BW_7_MHZ;
                break;

            case 8000:
                tuneParam_t2.bandwidth = SONY_DEMOD_BW_8_MHZ;
                break;

            default:
                return HI_FAILURE;
    }

    tuneParam_t2.centerFreqKHz = g_stTunerOps[u32TunerPort].stCurrPara.u32Frequency;                   /* Channel center frequency in KHz */
    tuneParam_t2.dataPlpId = u8PLpId;                            /* PLP ID where multiple PLP's are available */
    /* Additional tune information fed back from the driver.  This parameter should be checked
    if the result from the tune call is SONY_RESULT_OK_CONFIRM. */
    tuneParam_t2.tuneInfo = SONY_DEMOD_DVBT2_TUNE_INFO_OK;

    HI_INFO_TUNER (" Tune to DVB-T2 signal with the following parameters:\n");
    HI_INFO_TUNER ("  - Center Freq    : %uKHz\n", tuneParam_t2.centerFreqKHz);
    HI_INFO_TUNER ("  - Bandwidth      : %d\n", tuneParam_t2.bandwidth);
    HI_INFO_TUNER ("  - PLP ID         : %u\n", tuneParam_t2.dataPlpId);

    /* Perform DVBT2 Tune */
    result = sony_integ_dvbt2_Tune (&integ, &tuneParam_t2);
    return result;
    }

    return result;
}

/*only used in DVB-T2*/
HI_S32 cxd2837_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum)
{
    uint8_t u8Plp_Id[256];
    uint8_t u8Plp_num=0;
    sony_result_t result;

    //HI_ASSERT(HI_NULL != pu8PLPNum);
    HI_TUNER_CHECKPOINTER(pu8PLPNum);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        memset(u8Plp_Id,0,sizeof(u8Plp_Id));
        /*get the number of PLPs*/
        result = sony_demod_dvbt2_monitor_DataPLPs(&demod,u8Plp_Id,&u8Plp_num);
        if(result == SONY_RESULT_OK)
        {
            *pu8PLPNum = (HI_U8)u8Plp_num;
        }
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

/*only used in DVB-T2*/
HI_S32 cxd2837_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType)
{
    sony_dvbt2_plp_btype_t Plp_Type = SONY_DVBT2_PLP_DATA;
    sony_dvbt2_plp_t Plp_Info;
    sony_result_t result;

    //HI_ASSERT(HI_NULL != penPLPType);
    HI_TUNER_CHECKPOINTER(penPLPType);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        result = sony_demod_dvbt2_monitor_ActivePLP(&demod,Plp_Type,&Plp_Info);
        if(result == SONY_RESULT_OK)
        {
            switch(Plp_Info.type)
            {
                case SONY_DVBT2_PLP_TYPE_COMMON:
                    *penPLPType = HI_UNF_TUNER_T2_PLP_TYPE_COM;
                    break;
                case SONY_DVBT2_PLP_TYPE_DATA1:
                    *penPLPType = HI_UNF_TUNER_T2_PLP_TYPE_DAT1;
                    break;
                case SONY_DVBT2_PLP_TYPE_DATA2:
                    *penPLPType = HI_UNF_TUNER_T2_PLP_TYPE_DAT2;
                    break;
                default:
                    return HI_FAILURE;
            }
        }
        else
        {
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 cxd2837_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    sony_result_t result;
    int32_t s32Freq_Offset;

    HI_TUNER_CHECKPOINTER(pu32Freq);
    HI_TUNER_CHECKPOINTER(pu32Symb);
    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:

            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            result = sony_demod_dvbt_monitor_CarrierOffset(&demod,&s32Freq_Offset);
            if(result == SONY_RESULT_OK)
            {
                if(s32Freq_Offset < 0 )
                {
                s32Freq_Offset = - s32Freq_Offset;
                }
                *pu32Freq = s32Freq_Offset;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            result = sony_demod_dvbt2_monitor_CarrierOffset(&demod,&s32Freq_Offset);
            if(result == SONY_RESULT_OK)
            {
                if(s32Freq_Offset < 0 )
                {
                s32Freq_Offset = - s32Freq_Offset;
                }
                *pu32Freq = s32Freq_Offset;
            }
            else
            {

            }
            break;
        default:
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID cxd2837_connect_timeout(HI_U32 u32ConnectTimeout)
{
    return;
}

