#include <linux/module.h>
#include "hi_debug.h"
#include "hi_type.h"
#include "hi_unf_i2c.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "MN_DMD_driver.h"
#include "MN_DMD_device.h"
#include "MN_DMD_common.h"
#include "MN8847x_UserFunction.h"  
#include "MN8847x.h"
#include "MN_I2C.h"
//#include <mach/platform.h>
#include "mxl608.h"

/*Global Variable*/
DMD_PARAMETER_t g_stMN8847xDemod[TUNER_NUM];
DMD_u32_t g_I2cNum;  

HI_S32 MN8847x_Init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    DMD_PARAMETER_t * pstParaMeter;
    DMD_u8_t rd;
    HI_U32 i;

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    pstParaMeter->userdata = g_stTunerOps + u32TunerPort;
    pstParaMeter->devid = u32TunerPort;

    g_I2cNum = enI2cChannel;

    if( MN8847X_DMD_open(pstParaMeter) == DMD_E_OK )
    {
        HI_INFO_TUNER("OK.\n");
    }
    else
    {
        HI_INFO_TUNER("NG.\n");
    }

    MN8847X_DMD_device_read_chipid(&rd);
	if(DEMOD_TYPE != rd)
	{
		HI_ERR_TUNER("ERROR !! Wrong match of CHIP[%d] and Defined DMD type[%d]! \n",rd,DEMOD_TYPE);
		return HI_FAILURE;
	}
    
    /* Initialize Parameter Variable */
    for(i=0;i<DMD_INFORMATION_MAX;i++)
    {
        pstParaMeter->info[i] = 0;		//NOT_DEFINED
    }
  
    pstParaMeter->funit = DMD_E_KHZ;


    if( DMD_E_OK != MN8847X_DMD_init(pstParaMeter))
    {
        HI_ERR_TUNER("DMD_init failed.");
        return HI_FAILURE;
    }
    HI_INFO_TUNER("MN8847x_Init OK\n");

    return HI_SUCCESS;
    
}

HI_S32 MN8847x_Connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    DMD_PARAMETER_t * pstParaMeter;
    HI_U8 enI2cChan;
    DMD_ERROR_t enRet;
    DMD_u32_t     i2caddr;

    HI_ASSERT(HI_NULL != pstChannel);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }


    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    enI2cChan = g_stTunerOps[u32TunerPort].enI2cChannel;
    i2caddr = g_stTunerOps[u32TunerPort].u32DemodAddress;

    pstParaMeter->freq = pstChannel->u32Frequency;

    HI_INFO_TUNER("sigType:%d,freq:%d kHz\n",g_stTunerOps[u32TunerPort].enSigType,pstParaMeter->freq );
    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            pstParaMeter->system = DMD_E_DVBT;
            switch(pstChannel->unTer.enDVBT)
            {
                case HI_UNF_TUNER_TS_PRIORITY_HP:
                    pstParaMeter->dvbt_hp_lp_sel = DMD_E_DVBT_HIER_SEL_HP;   
                    HI_INFO_TUNER("---->t: %d [0-none,1-HP,2-LP]\n",pstChannel->unTer.enDVBT);
                    break;
                case HI_UNF_TUNER_TS_PRIORITY_LP:
                default:
                    pstParaMeter->dvbt_hp_lp_sel = DMD_E_DVBT_HIER_SEL_LP; 
                    HI_INFO_TUNER("---->t: %d [0-none,1-HP,2-LP]\n",pstChannel->unTer.enDVBT);
                    break;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            pstParaMeter->system = DMD_E_DVBT2;
            pstParaMeter->dvbt2_plp_no = pstChannel->unTer.enDVBT2.u8PlpId;
            switch(pstChannel->unTer.enDVBT2.enChannelAttr)
            {
                case HI_UNF_TUNER_TER_MODE_BASE:
                    pstParaMeter->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_BASE; 
                    HI_INFO_TUNER("---->t2: %d [0-base,1-lite]\n",pstParaMeter->t2_work_profile_mode);
                    break;
                case HI_UNF_TUNER_TER_MODE_LITE:
                    pstParaMeter->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_LITE;         
                    HI_INFO_TUNER("---->t2: %d [0-base,1-lite]\n",pstParaMeter->t2_work_profile_mode);
                    break;
                default:
                    pstParaMeter->t2_work_profile_mode = DMD_E_DVBT2_PROFILE_TYPE_UNDEFINED; 
                    HI_INFO_TUNER("---->t2: %d [0-base,1-lite,2-undef]\n",pstParaMeter->t2_work_profile_mode);
                    break;
            }
            break;
        default:
        	HI_ERR_TUNER("Signal type[0x%x] not supported!\n",g_stTunerOps[u32TunerPort].enSigType);
           return HI_FAILURE;
    }
    HI_INFO_TUNER("---->pstParaMeter->system :%d\n",pstParaMeter->system );

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T == g_stTunerOps[u32TunerPort].enSigType ||
        HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        switch(pstChannel->unSRBW.u32BandWidth)
        {
            case 5000:
                pstParaMeter->bw = DMD_E_BW_5MHZ;
                break;
            case 6000:
                pstParaMeter->bw = DMD_E_BW_6MHZ;
                break;
            case 7000:
                pstParaMeter->bw = DMD_E_BW_7MHZ;
                break;
            case 8000:
                pstParaMeter->bw = DMD_E_BW_8MHZ;
                break;
            case 1700:
                pstParaMeter->bw = DMD_E_BW_1_7MHZ;
                break;
            default:
                HI_ERR_TUNER("-->unsupport,bw:%d kHz\n",pstChannel->unSRBW.u32BandWidth);
                return HI_FAILURE;
               
        }
    }    
    //DMD_wait(500);
    enRet =  g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, enI2cChan, pstParaMeter->freq);
    if (HI_SUCCESS != enRet)
    {
        HI_ERR_TUNER("MN8847x set tuner not ok\n");
        return enRet;
    }
    HI_INFO_TUNER("MN8847x set tuner ok, f:%d Hz\n",pstParaMeter->freq);

    /*set ts interface to parallel or serial*/

    switch(g_stTunerOps[u32TunerPort].enTsType)
    {
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
        case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
            pstParaMeter->ts_out = DMD_E_TSOUT_PARALLEL_FIXED_CLOCK;
            HI_INFO_TUNER("---->tsout:%d\n",pstParaMeter->ts_out);
            break;
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
            pstParaMeter->ts_out = DMD_E_TSOUT_SERIAL_FIXED_CLOCK;
            HI_INFO_TUNER("---->tsout:%d\n",pstParaMeter->ts_out);
            break;
        default:
            HI_ERR_TUNER("enTsType unsupport:%d\n",g_stTunerOps[u32TunerPort].enTsType);
            return HI_FAILURE;
    }
    
    MN8847X_DMD_set_ts_output(pstParaMeter->ts_out); 
    
	/* Call pre tune process */
	//DMD_device_pre_tune(pstParaMeter); //default empty 

    //HI_ERR_TUNER("-->mode:%d,freq:%d,bw:%d,plpno:%d,hp_lp:%d,t2_profile:%d\n",pstParaMeter->system,pstParaMeter->freq,pstParaMeter->bw,
    //   pstParaMeter->dvbt2_plp_no,pstParaMeter->dvbt_hp_lp_sel,pstParaMeter->t2_work_profile_mode);
    enRet = MN8847X_DMD_set_system(pstParaMeter);
    if (DMD_E_OK != enRet)
    {
        return HI_FAILURE;
    }
    
    MN8847X_DMD_device_post_tune(pstParaMeter);
    //DMD_read_registers();
    return HI_SUCCESS;
}

HI_S32 MN8847x_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
	return HI_SUCCESS;
}

HI_VOID MN8847x_connect_timeout(HI_U32 u32ConnectTimeout)
{
    return;
}
HI_S32 MN8847x_GetStatus (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    DMD_PARAMETER_t * pstParaMeter;
    //DMD_ERROR_t enRet;

    HI_ASSERT(HI_NULL != penTunerStatus);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;

    *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    MN8847X_DMD_get_info( pstParaMeter , DMD_E_INFO_LOCK);
    //HI_INFO_TUNER("---->lock:0x%x [0-locked, 1-nosignal, 2-nosync, 3-error]\n",pstParaMeter->info[DMD_E_INFO_LOCK]);
    if( pstParaMeter->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED )
    {
        HI_INFO_TUNER("--------> Signal Locked!! \n");
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    else
    {
        HI_INFO_TUNER("--------> Signal unLocked: 0x%x [0-locked, 1-nosignal, 2-nosync, 3-error]\n",pstParaMeter->info[DMD_E_INFO_LOCK]);
    }
    return HI_SUCCESS;
}

HI_S32 MN8847x_SetTsType(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    DMD_PARAMETER_t * pstParaMeter;
    
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
    
    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    g_stTunerOps[u32TunerPort].enTsType = enTsType;
    HI_INFO_TUNER("The stream type of UNF Layer is %d.\n",enTsType);

    switch(enTsType)
    {
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
        case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
            pstParaMeter->ts_out = DMD_E_TSOUT_PARALLEL_FIXED_CLOCK;
            break;
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
            pstParaMeter->ts_out = DMD_E_TSOUT_SERIAL_FIXED_CLOCK;
            break;
        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*获取误码率，pu32ber处存放错误数，pu32ber+1处存放收到数据总数*/
HI_S32 MN8847x_GetBer(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    DMD_PARAMETER_t * pstParaMeter;

    HI_ASSERT(HI_NULL != pu32ber);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    if (DMD_E_OK != MN8847X_DMD_BER(pstParaMeter->system, pu32ber, pu32ber + 1))
    {
        return HI_FAILURE;
    }

    if (pu32ber[1])
    {
        pu32ber[0] = pu32ber[0] * 1000000000 / pu32ber[1];
        pu32ber[1] = 0;
        pu32ber[2] = 9;
    }
    else
    {
        pu32ber[0] = 0;
        pu32ber[1] = 0;
        pu32ber[2] = 0;
    }

    HI_INFO_TUNER("-->*pu32ber: %d*10-9\n",pu32ber[0]);
    return HI_SUCCESS;
}

/*获取信噪比，结果已经转换为dB*/
HI_S32 MN8847x_GetSnr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    DMD_PARAMETER_t * pstParaMeter;
    HI_U32 ulCntInt = 0;
    HI_U32 ulCntDec = 0;

    HI_ASSERT(HI_NULL != pu32SNR);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
    
    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    
    if (DMD_E_OK != MN8847X_DMD_CNR(pstParaMeter->system, &ulCntInt, &ulCntDec))
    {
       return HI_FAILURE;
    }
    /*只取整数部分*/
    *pu32SNR = ulCntInt;
    HI_INFO_TUNER("-->*pu32SNR:%d\n",*pu32SNR);
    return HI_SUCCESS;
}

HI_S32 MN8847x_GetSignalStrength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    DMD_PARAMETER_t * pstParaMeter;

    HI_ASSERT(HI_NULL != pu32SignalStrength);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    pu32SignalStrength[1] = MN8847X_DMD_AGC();
    HI_INFO_TUNER("-->*pu32SignalStrength:%d\n",pu32SignalStrength[1]);
    return HI_SUCCESS;
}

HI_S32 MN8847x_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    DMD_PARAMETER_t* pstParaMeter;
    //HI_U32 i = 0;

    HI_ASSERT(HI_NULL != pstInfo);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
    
    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    pstInfo->enSigType = g_stTunerOps[u32TunerPort].enSigType;
        
    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == pstInfo->enSigType)
    {
        /*get the constellation*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT2_DAT_PLP_MOD);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD])
        {
        case 0:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case 1:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 2:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        case 3:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
        default:
            return HI_FAILURE;	
        }
        
        /*get fft size*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT2_MODE);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT2_MODE])
        {
        case 0:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_1K;
            break;
        case 1:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
            break;
        case 2:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
            break;
        case 3:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
            break;
        case 4:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_16K;
            break;
        case 5:
	     pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_32K;
            break;
        default:
            return HI_FAILURE;	
        }

        /*get guard interval*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT2_GI);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT2_GI])
        {
        case 0:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 1:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
            break;
        case 2:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
            break;
        case 3:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
            break;
        case 4:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 5:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_128;
            break;
        case 6:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_128;
            break;
        case 7:
	     pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_256;
            break;
        default:
            return HI_FAILURE;	
        }

        /*get fec*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT2_L1_COD);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT2_L1_COD])
        {
        case 0:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
            break;
        case 1:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_5;
            break;
        case 2:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
            break;
        case 3:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
            break;
        case 4:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
            break;
        case 5:
	     pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
            break;
        default:
            return HI_FAILURE;	
        }        
    }
    else if (HI_UNF_TUNER_SIG_TYPE_DVB_T == pstInfo->enSigType)
    {
        /*get the constellation*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_CONSTELLATION);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT_CONSTELLATION])
        {
        case 0:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case 1:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 2:
	     pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        default:
            return HI_FAILURE;	
        }
        
        /*get fft size*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_MODE);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT_MODE])
        {
         case 0:
 	    pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
             break;
         case 1:
 	    pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
             break;
         default:
             return HI_FAILURE;	
        }

        /*get guard interval*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_GI);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT_GI])
        {
        case 0:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
            break;
        default:
            return HI_FAILURE;	
        }

        /*get priority*/    
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_HIERARCHY_SELECT);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT])
        {
            case 0:
                pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_LP;
                break;
            case 1:
                pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;
                break;
            default:
                return HI_FAILURE;	
        }
       
        /*get alpha*/
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_HIERARCHY);
        switch (pstParaMeter->info[DMD_E_INFO_DVBT_HIERARCHY])
        {
        case 0:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_NONE;
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_NO;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA1;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA2;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA4;
            break;
        default:
            return HI_FAILURE;	
        }
       
        /*get fec*/
        if (HI_UNF_TUNER_TS_PRIORITY_HP == pstInfo->unSignalInfo.stTer.enTsPriority)
        {
            MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_HP_CODERATE);

            switch (pstParaMeter->info[DMD_E_INFO_DVBT_HP_CODERATE])
            {
            case 0:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                break;
            case 1:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                break;
            case 2:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                break;
            case 3:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                break;
            case 4:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                break;
            default:
                return HI_FAILURE;	
            }
        }
        else
        {
            MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT_LP_CODERATE);

            switch (pstParaMeter->info[DMD_E_INFO_DVBT_LP_CODERATE])
            {
            case 0:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                break;
            case 1:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                break;
            case 2:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                break;
            case 3:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                break;
            case 4:
                pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                break;
            default:
                return HI_FAILURE;	
            }
        }
    }
    else
    {
        return HI_FAILURE;	
    }
    
    return HI_SUCCESS;
}

/*only used in DVB-T2*/
HI_S32 MN8847x_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum)
{
    DMD_PARAMETER_t* pstParaMeter;

    HI_ASSERT(HI_NULL != pu8PLPNum);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        /*get the number of PLPs*/
        MN8847X_DMD_get_info( pstParaMeter, DMD_E_INFO_DVBT2_NUM_PLP );
        *pu8PLPNum = pstParaMeter->info[DMD_E_INFO_DVBT2_NUM_PLP];
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

/*only used in DVB-T2*/
HI_S32 MN8847x_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType)
{
    DMD_PARAMETER_t* pstParaMeter;

    HI_ASSERT(HI_NULL != penPLPType);
    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == g_stTunerOps[u32TunerPort].enSigType)
    {
        MN8847X_DMD_wait(300);
        MN8847X_DMD_get_info(pstParaMeter, DMD_E_INFO_DVBT2_DAT_PLP_TYPE);
        *penPLPType = pstParaMeter->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE];
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

/*only used in DVB-T2*/
HI_S32 MN8847x_set_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode)
{
    DMD_PARAMETER_t* pstParaMeter;

    /* Check tuner port. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    
    MN8847X_DMD_set_info( pstParaMeter , DMD_E_INFO_DVBT2_SELECTED_PLP , u8PLpId & 0xFF );
    return HI_SUCCESS;
}

HI_S32 MN8847x_get_plp_id(HI_U32 u32TunerPort, HI_U8 *u8PlpId)
{
    DMD_PARAMETER_t* pstParaMeter;
    
    /* Check tuner port. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    pstParaMeter = g_stMN8847xDemod + u32TunerPort;
    
    MN8847X_DMD_get_info( pstParaMeter , DMD_E_INFO_DVBT2_SELECTED_PLP);
    *u8PlpId = pstParaMeter->info[DMD_E_INFO_DVBT2_SELECTED_PLP];
    return HI_SUCCESS;

}

HI_S32 MN8847x_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    /*guarantee the pointer is not NULL*/
    HI_ASSERT(HI_NULL != pu32Freq);
    HI_ASSERT(HI_NULL != pu32Symb);

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }
    
    *pu32Freq = 0;
    *pu32Symb = 0;
    
    return HI_SUCCESS;
}
/*
HI_VOID MN8847x_TunerResume(HI_VOID)
{

    DMD_PARAMETER_t * pstParaMeter;

    //本来应该要根据u32TunerPort来选择哪个tuner的，但是目前这个接口无法知道u32TunerPort,
    //目前的应用暂时只有一个tuner，就先用0,后续此接口要考虑多tuner的情况
    pstParaMeter = g_stMN8847xDemod;
    *((volatile unsigned int *)(IO_ADDRESS(0x10203160))) = 0;
    MN8847X_DMD_init(pstParaMeter);
    MN8847X_DMD_device_set_sig_type(pstParaMeter);
    return;
}
*/

