//#include <linux/module.h>

#include "hi_type.h"
//#include "hi_unf_ecs.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#include <common.h>

#include "MxL603_TunerApi.h"
#include "MxL603_OEM_Drv.h"

#define EXAMPLE_DEV_MAX 2
#define MXL603_I2C_ADDR 0xc6
#define PLL_STEP 62500

/* Example of OEM Data, customers should have
below data structure declared at their appropriate 
places as per their software design 

typedef struct
{
  UINT8   i2c_address;
  UINT8   i2c_bus;
  sem_type_t sem;
  UINT16  i2c_cnt;
} user_data_t;  

user_data_t device_context[EXAMPLE_DEV_MAX];

*/
HI_BOOL mxl603_initflag = HI_FALSE;
user_data_t device_context[EXAMPLE_DEV_MAX];
HI_U8 s_u8devId_dvb_c = 0;

HI_S32 mxl603_set_tuner_dvb_c(HI_U32 u32TunerPort, HI_U8 i2c_channel, HI_U32 puRF)
{
    MXL_STATUS status; 
    //HI_U8 I2C_Addr;
    MXL_BOOL singleSupply_3_3V;
    MXL603_XTAL_SET_CFG_T xtalCfg;
    MXL603_IF_OUT_CFG_T ifOutCfg;
    MXL603_AGC_CFG_T agcCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;
    MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;		
    HI_U32 u32FreqInHz = puRF * 1000;
    //SINT16 rxPwrPtr = 0;

    s_u8devId_dvb_c = u32TunerPort;

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
    if(mxl603_initflag == HI_FALSE)
    {
        for (s_u8devId_dvb_c = 0; s_u8devId_dvb_c < EXAMPLE_DEV_MAX; s_u8devId_dvb_c++)
        {
            device_context[s_u8devId_dvb_c] .i2c_address = MXL603_I2C_ADDR;
            status = MxLWare603_API_CfgDrvInit(s_u8devId_dvb_c, (void *) &device_context[s_u8devId_dvb_c]); 
        }

        /* config tuner port */
        MxL603_I2C_SetPort(u32TunerPort);
        /* config i2c channel */
        MxL603_I2C_SetChn(i2c_channel);

        //devId = 0;
        s_u8devId_dvb_c = u32TunerPort;

        //Step 1 : Soft Reset MxL603
        status = MxLWare603_API_CfgDevSoftReset(s_u8devId_dvb_c);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgDevSoftReset\n");    
        }

        MxLWare603_OEM_Sleep(30);

        //Step 2 : Overwrite Default
        singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
        status = MxLWare603_API_CfgDevOverwriteDefaults(s_u8devId_dvb_c, singleSupply_3_3V);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgDevOverwriteDefaults\n");    
        }

        //Step 3 : XTAL Setting
        xtalCfg.xtalFreqSel = MXL603_XTAL_16MHz;
        xtalCfg.xtalCap = 24; //12;
        xtalCfg.clkOutEnable = MXL_DISABLE;//MXL_ENABLE;
        xtalCfg.clkOutDiv = MXL_DISABLE;
        xtalCfg.clkOutExt = MXL_DISABLE;
        xtalCfg.singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;单电源模式
        xtalCfg.XtalSharingMode = MXL_DISABLE;
        status = MxLWare603_API_CfgDevXtal(s_u8devId_dvb_c, xtalCfg);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgDevXtal\n");    
        }

        //Step 4 : IF Out setting
        ifOutCfg.ifOutFreq = MXL603_IF_7_2MHz;
        ifOutCfg.ifInversion = MXL_DISABLE;
        ifOutCfg.gainLevel = 11;
        ifOutCfg.manualFreqSet = MXL_DISABLE;
        ifOutCfg.manualIFOutFreqInKHz = 0;
        status = MxLWare603_API_CfgTunerIFOutParam(s_u8devId_dvb_c, ifOutCfg);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgTunerIFOutParam\n");    
        }

        //Step 5 : AGC Setting
        agcCfg.agcType = MXL603_AGC_EXTERNAL;//MXL603_AGC_SELF
        agcCfg.setPoint = 66;
        agcCfg.agcPolarityInverstion = MXL_DISABLE;
        status = MxLWare603_API_CfgTunerAGC(s_u8devId_dvb_c, agcCfg);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgTunerAGC\n");    
        }

        //Step 6 : Application Mode setting
        tunerModeCfg.signalMode = MXL603_DIG_DVB_C;//MXL603_DIG_DVB_T;
        tunerModeCfg.ifOutFreqinKHz = 7200;
        tunerModeCfg.xtalFreqSel = MXL603_XTAL_16MHz;
        tunerModeCfg.ifOutGainLevel = 11;
        status = MxLWare603_API_CfgTunerMode(s_u8devId_dvb_c, tunerModeCfg);
        if (status != MXL_SUCCESS)
        {
            TUNER_DEBUG("Error! MxLWare603_API_CfgTunerMode\n");    
        }
    }

    //Step 7 : Channel frequency & bandwidth setting
    chanTuneCfg.bandWidth = MXL603_CABLE_BW_8MHz;//MXL603_TERR_BW_8MHz;//
    chanTuneCfg.freqInHz = u32FreqInHz;//666000000;
    chanTuneCfg.signalMode = MXL603_DIG_DVB_C;//MXL603_DIG_DVB_T;
    chanTuneCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
    status = MxLWare603_API_CfgTunerChanTune(s_u8devId_dvb_c, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_DEBUG("Error! MxLWare603_API_CfgTunerChanTune\n");   
    }

    // Wait 15 ms 
    MxLWare603_OEM_Sleep(15);

    // Read back Tuner lock status
    status = MxLWare603_API_ReqTunerLockStatus(s_u8devId_dvb_c, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if (MXL_LOCKED == rfLockPtr && MXL_LOCKED == refLockPtr)
        {
            TUNER_DEBUG("Tuner locked\n");
        }
        else
        {
            TUNER_DEBUG("Tuner unlocked\n");
            return HI_FAILURE;
        }  
    }

    // To Change Channel, GOTO Step #7
    // To change Application mode settings, GOTO Step #6
    if(mxl603_initflag == HI_FALSE)
    mxl603_initflag = HI_TRUE;

    //status = MxLWare603_API_ReqTunerRxPower(devId, &rxPwrPtr);

    return HI_SUCCESS;
}

HI_S32 mxl603_set_tuner (HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    enMxl603TunerSigType = g_stTunerOps[u32TunerPort].enSigType;

    if(enMxl603TunerSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
    {  
        HI_S32 s32Ret = HI_SUCCESS;
        s32Ret = mxl603_set_tuner_dvb_c(u32TunerPort, enI2cChannel, u32PuRF);
        return s32Ret;
        
    }
    else
    {
        MXL_STATUS status; 
        UINT8 devId;
        MXL_BOOL singleSupply_3_3V;
        MXL603_XTAL_SET_CFG_T xtalCfg;
        MXL603_IF_OUT_CFG_T ifOutCfg;
        MXL603_AGC_CFG_T agcCfg;
        MXL603_TUNER_MODE_CFG_T tunerModeCfg = {0};
        MXL603_CHAN_TUNE_CFG_T chanTuneCfg = {0};
        MXL_BOOL refLockPtr = MXL_UNLOCKED;
        MXL_BOOL rfLockPtr = MXL_UNLOCKED;		

        devId = u32TunerPort;
        /* If OEM data is implemented, customer needs to use OEM data structure  
        related operation. Following code should be used as a reference. 
        For more information refer to sections 2.5 & 2.6 of 
        MxL603_mxLWare_API_UserGuide document.

        for (devId = 0; devId < EXAMPLE_DEV_MAX; devId++)
        {
        // assigning i2c address for  the device
        device_context[devId].i2c_address = GET_FROM_FILE_I2C_ADDRESS(devId);     

        // assigning i2c bus for  the device
        device_context[devId].i2c_bus = GET_FROM_FILE_I2C_BUS(devId);                      

        // create semaphore if necessary
        device_context[devId].sem = CREATE_SEM();                                                           

        // sample stat counter
        device_context[devId].i2c_cnt = 0;                                                                               

        status = MxLWare603_API_CfgDrvInit(devId, (void *) &device_context[devId]);  

        // if you don抰 want to pass any oem data, just use NULL as a parameter:
        // status = MxLWare603_API_CfgDrvInit(devId, NULL);  
        }

        */

        /* If OEM data is not required, customer should treat devId as 
         I2C slave Address */
        /*devId = MXL603_I2C_ADDR;*/

        //Step 1 : Soft Reset MxL603
        status = MxLWare603_API_CfgDevSoftReset(devId);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("<0>Error! MxLWare603_API_CfgDevSoftReset\n");    
        }

        //Step 2 : Overwrite Default
        singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
        status = MxLWare603_API_CfgDevOverwriteDefaults(devId, singleSupply_3_3V);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("<0>Error! MxLWare603_API_CfgDevOverwriteDefaults\n");    
        }

        //Step 3 : XTAL Setting
        xtalCfg.xtalFreqSel = MXL603_XTAL_24MHz;
        xtalCfg.xtalCap = 6;
        xtalCfg.clkOutEnable = MXL_ENABLE;
        xtalCfg.clkOutDiv = MXL_DISABLE;
        xtalCfg.clkOutExt = MXL_DISABLE;
        xtalCfg.singleSupply_3_3V = MXL_ENABLE;
        xtalCfg.XtalSharingMode = MXL_DISABLE;
        status = MxLWare603_API_CfgDevXtal(devId, xtalCfg);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("Error! MxLWare603_API_CfgDevXtal\n");    
        }

        //Step 4 : IF Out setting
        ifOutCfg.ifOutFreq = MXL603_IF_5MHz;
        ifOutCfg.ifInversion = MXL_DISABLE;
        ifOutCfg.gainLevel = 11;
        ifOutCfg.manualFreqSet = MXL_DISABLE;
        ifOutCfg.manualIFOutFreqInKHz = 0;
        status = MxLWare603_API_CfgTunerIFOutParam(devId, ifOutCfg);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("Error! MxLWare603_API_CfgTunerIFOutParam\n");    
        }

        //Step 5 : AGC Setting
        agcCfg.agcType = MXL603_AGC_EXTERNAL;
        agcCfg.setPoint = 66;
        agcCfg.agcPolarityInverstion = MXL_DISABLE;
        status = MxLWare603_API_CfgTunerAGC(devId, agcCfg);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("Error! MxLWare603_API_CfgTunerAGC\n");    
        }

        //Step 6 : Application Mode setting
        switch(g_stTunerOps[u32TunerPort].enSigType)
        {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
          tunerModeCfg.signalMode = MXL603_DIG_DVB_C;
          chanTuneCfg.signalMode = MXL603_DIG_DVB_C;
          break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
          tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
          chanTuneCfg.signalMode = MXL603_DIG_DVB_T;
          break;
        default:
        	TUNER_DEBUG("<0>Signal type not supported!\n");
        }
        //tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
        tunerModeCfg.ifOutFreqinKHz = 5000;
        tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
        tunerModeCfg.ifOutGainLevel = 11;
        status = MxLWare603_API_CfgTunerMode(devId, tunerModeCfg);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("Error! MxLWare603_API_CfgTunerMode\n");    
        }

        //Step 7 : Channel frequency & bandwidth setting
        if (MXL603_DIG_DVB_T == chanTuneCfg.signalMode)
        {
          switch(g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth)
          {
              case 6000:
                  chanTuneCfg.bandWidth = MXL603_TERR_BW_6MHz;
                  break;
              case 7000:
                  chanTuneCfg.bandWidth = MXL603_TERR_BW_7MHz;
                  break;
              case 8000:
                  chanTuneCfg.bandWidth = MXL603_TERR_BW_8MHz;
                  break;
              case 5000:
              case 1700:
              default:
                  TUNER_DEBUG("<0>Not supported!\n");
                  break;
          }
        }
        else if (MXL603_DIG_DVB_C == chanTuneCfg.signalMode)
        {
          chanTuneCfg.bandWidth = MXL603_CABLE_BW_8MHz;
        }

        //chanTuneCfg.bandWidth = MXL603_TERR_BW_8MHz;
        chanTuneCfg.freqInHz = u32PuRF*1000;
        //chanTuneCfg.signalMode = MXL603_DIG_DVB_T;
        chanTuneCfg.xtalFreqSel = MXL603_XTAL_24MHz;
        chanTuneCfg.startTune = MXL_START_TUNE;
        status = MxLWare603_API_CfgTunerChanTune(devId, chanTuneCfg);
        if (status != MXL_SUCCESS)
        {
        TUNER_DEBUG("Error! MxLWare603_API_CfgTunerChanTune\n");    
        }

        // Wait 15 ms 
        MxLWare603_OEM_Sleep(15);

        // Read back Tuner lock status
        status = MxLWare603_API_ReqTunerLockStatus(devId, &rfLockPtr, &refLockPtr);
        if (status == MXL_TRUE)
        {
        if (MXL_LOCKED == rfLockPtr && MXL_LOCKED == refLockPtr)
        {
          TUNER_DEBUG("Tuner locked\n");
        }
        else
          TUNER_DEBUG("Tuner unlocked\n");
        }

        // To Change Channel, GOTO Step #7

        // To change Application mode settings, GOTO Step #6

        return HI_SUCCESS;
    }
}

HI_S32 mxl603_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    SINT16 s16SignalStrength = 0;
    MXL_STATUS status;
    
    status = MxLWare603_API_ReqTunerRxPower(s_u8devId_dvb_c, &s16SignalStrength);
    if(MXL_SUCCESS != status)
    {
        return status;
    }
    if(s16SignalStrength < 0)
    {
        pu32SignalStrength[2] = 1;
        pu32SignalStrength[1] = (HI_U32)((HI_U32)0-s16SignalStrength);
    }
    else
    {
        pu32SignalStrength[2] = 0;
        pu32SignalStrength[1] = (HI_U32)s16SignalStrength;
    }
    
    return HI_SUCCESS;
}

HI_VOID mxl603_tuner_resume(HI_VOID)
{
    mxl603_initflag = HI_FALSE;
 
    return;
}
