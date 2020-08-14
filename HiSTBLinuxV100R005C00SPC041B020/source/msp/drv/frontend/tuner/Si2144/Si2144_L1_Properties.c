/*************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties definitions
   FILE: Si2144_L1_Properties.c
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_0_build_2
   Revision: REVISION_NAME
   Tag:  TAGNAME
   Date: November 06 2014
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2144_COMMAND_PROTOTYPES

#include "Si2144_L1_API.h"

/***********************************************************************************************************************
  Si2144_L1_SetProperty function
  Use:        property set function
              Used to call L1_SET_PROPERTY with the property Id and data provided.
  Parameter: *api     the Si2144 context
  Parameter: prop     the property Id
  Parameter: data     the property bytes
  Behavior:  This function will only download the property if required.
               Conditions to download the property are:
                - The property changes
                - The propertyWriteMode is set to Si2144_DOWNLOAD_ALWAYS
                - The property is unknown to Si2144_PackProperty (this may be useful for debug purpose)
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2144_L1_SetProperty         (L1_Si2144_Context *api, unsigned int prop_code, int  data) {
    unsigned char  reserved=0;
    return Si2144_L1_SET_PROPERTY (api, reserved, prop_code, data);
}
/***********************************************************************************************************************
  Si2144_L1_GetProperty function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2144 context
  Parameter: prop     the property Id
  Parameter: *data    a buffer to store the property bytes into
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2144_L1_GetProperty         (L1_Si2144_Context *api, unsigned int prop_code, int *data) {
    unsigned char  reserved          = 0;
    unsigned char res;
    res = Si2144_L1_GET_PROPERTY (api, reserved, prop_code);
    *data = api->rsp->get_property.data;
    return res;
}
 /***********************************************************************************************************************
  Si2144_L1_SetProperty2 function
  Use:        Sets the property given the property code.
  Parameter: *api     the Si2144 context
  Parameter: prop     the property Id

  Returns:    NO_Si2144_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2144_L1_SetProperty2        (L1_Si2144_Context *api, unsigned int prop_code) {
    int data, res;
    char msg[1000];
    res = Si2144_PackProperty(api->prop, prop_code, &data);
    if (res != NO_Si2144_ERROR) {
      snprintf(msg,1000, "\nSi2144_L1_SetProperty2: %s 0x%04x!\n\n", Si2144_L1_API_ERROR_TEXT(res), prop_code);
      SiTRACE(msg);
      SiERROR(msg);
      return res;
    }
    return Si2144_L1_SetProperty (api, prop_code & 0xffff, data);
  }
 /***********************************************************************************************************************
  Si2144_L1_GetProperty2 function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2144 context
  Parameter: prop     the property Id

  Returns:    NO_Si2144_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2144_L1_GetProperty2        (L1_Si2144_Context *api, unsigned int prop_code) {
  int data, res;
  res = Si2144_L1_GetProperty(api, prop_code & 0xffff, &data);
  if (res != NO_Si2144_ERROR) {
    SiTRACE("Si2144_L1_GetProperty2: %s 0x%04x\n", Si2144_L1_API_ERROR_TEXT(res), prop_code);
    SiERROR(Si2144_L1_API_ERROR_TEXT(res));
    return res;
  }
  return Si2144_UnpackProperty(api->prop, prop_code, data);
}
/*****************************************************************************************
 NAME: Si2144_downloadCOMMONProperties
  DESCRIPTION: Setup Si2144 COMMON properties configuration
  This function will download all the COMMON configuration properties.
  The function Si2144_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2144 Context
  Returns:    I2C transaction error code, NO_Si2144_ERROR if successful
  Programming Guide Reference:    COMMON setup flowchart
******************************************************************************************/
int  Si2144_downloadCOMMONProperties      (L1_Si2144_Context *api) {
  SiTRACE("Si2144_downloadCOMMONProperties\n");
#ifdef    Si2144_CRYSTAL_TRIM_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_CRYSTAL_TRIM_PROP_CODE                ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_CRYSTAL_TRIM_PROP */
#ifdef    Si2144_XOUT_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_XOUT_PROP_CODE                        ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_XOUT_PROP */
return NO_Si2144_ERROR;
}
/*****************************************************************************************
 NAME: Si2144_downloadDTVProperties
  DESCRIPTION: Setup Si2144 DTV properties configuration
  This function will download all the DTV configuration properties.
  The function Si2144_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2144 Context
  Returns:    I2C transaction error code, NO_Si2144_ERROR if successful
  Programming Guide Reference:    DTV setup flowchart
******************************************************************************************/
int  Si2144_downloadDTVProperties         (L1_Si2144_Context *api) {
  SiTRACE("Si2144_downloadDTVProperties\n");
#ifdef    Si2144_DTV_AGC_AUTO_FREEZE_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_AGC_AUTO_FREEZE_PROP_CODE         ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_AGC_AUTO_FREEZE_PROP */
#ifdef    Si2144_DTV_AGC_SPEED_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_AGC_SPEED_PROP_CODE               ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_AGC_SPEED_PROP */
#ifdef    Si2144_DTV_CONFIG_IF_PORT_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_CONFIG_IF_PORT_PROP_CODE          ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_CONFIG_IF_PORT_PROP */
#ifdef    Si2144_DTV_EXT_AGC_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_EXT_AGC_PROP_CODE                 ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_EXT_AGC_PROP */
#ifdef    Si2144_DTV_IF_AGC_SPEED_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_IF_AGC_SPEED_PROP_CODE            ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_IF_AGC_SPEED_PROP */
#ifdef    Si2144_DTV_INITIAL_AGC_SPEED_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_INITIAL_AGC_SPEED_PROP_CODE       ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_INITIAL_AGC_SPEED_PROP */
#ifdef    Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
#ifdef    Si2144_DTV_INTERNAL_ZIF_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_INTERNAL_ZIF_PROP_CODE            ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_INTERNAL_ZIF_PROP */
#ifdef    Si2144_DTV_LIF_FREQ_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_LIF_FREQ_PROP_CODE                ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_LIF_FREQ_PROP */
#ifdef    Si2144_DTV_LIF_OUT_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_LIF_OUT_PROP_CODE                 ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_LIF_OUT_PROP */
#ifdef    Si2144_DTV_MODE_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_MODE_PROP_CODE                    ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_MODE_PROP */
#ifdef    Si2144_DTV_PGA_LIMITS_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_PGA_LIMITS_PROP_CODE              ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_PGA_LIMITS_PROP */
#ifdef    Si2144_DTV_PGA_TARGET_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_PGA_TARGET_PROP_CODE              ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_PGA_TARGET_PROP */
#ifdef    Si2144_DTV_RF_AGC_SPEED_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_RF_AGC_SPEED_PROP_CODE            ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_RF_AGC_SPEED_PROP */
#ifdef    Si2144_DTV_RF_TOP_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_RF_TOP_PROP_CODE                  ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_RF_TOP_PROP */
#ifdef    Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE      ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP */
#ifdef    Si2144_DTV_WB_AGC_SPEED_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_DTV_WB_AGC_SPEED_PROP_CODE            ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_DTV_WB_AGC_SPEED_PROP */
return NO_Si2144_ERROR;
}
/*****************************************************************************************
 NAME: Si2144_downloadTUNERProperties
  DESCRIPTION: Setup Si2144 TUNER properties configuration
  This function will download all the TUNER configuration properties.
  The function Si2144_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2144 Context
  Returns:    I2C transaction error code, NO_Si2144_ERROR if successful
  Programming Guide Reference:    TUNER setup flowchart
******************************************************************************************/
int  Si2144_downloadTUNERProperties       (L1_Si2144_Context *api) {
  SiTRACE("Si2144_downloadTUNERProperties\n");
#ifdef    Si2144_ACTIVE_LOOP_THROUGH_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_ACTIVE_LOOP_THROUGH_PROP_CODE         ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_ACTIVE_LOOP_THROUGH_PROP */
#ifdef    Si2144_TUNER_BLOCKED_VCO_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_TUNER_BLOCKED_VCO_PROP_CODE           ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_TUNER_BLOCKED_VCO_PROP */
#ifdef    Si2144_TUNER_BLOCKED_VCO2_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_TUNER_BLOCKED_VCO2_PROP_CODE          ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_TUNER_BLOCKED_VCO2_PROP */
#ifdef    Si2144_TUNER_BLOCKED_VCO3_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_TUNER_BLOCKED_VCO3_PROP_CODE          ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_TUNER_BLOCKED_VCO3_PROP */
#ifdef    Si2144_TUNER_LO_INJECTION_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_TUNER_LO_INJECTION_PROP_CODE          ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_TUNER_LO_INJECTION_PROP */
#ifdef    Si2144_TUNER_RETURN_LOSS_PROP
  if (Si2144_L1_SetProperty2(api, Si2144_TUNER_RETURN_LOSS_PROP_CODE           ) != NO_Si2144_ERROR) {return ERROR_Si2144_SENDING_COMMAND;}
#endif /* Si2144_TUNER_RETURN_LOSS_PROP */
return NO_Si2144_ERROR;
}
int  Si2144_downloadAllProperties         (L1_Si2144_Context *api) {
  Si2144_downloadCOMMONProperties      (api);
  Si2144_downloadDTVProperties         (api);
  Si2144_downloadTUNERProperties       (api);
  return 0;
}

/***********************************************************************************************************************
  Si2144_PackProperty function
  Use:        This function will pack all the members of a property into an integer for the SetProperty function.

  Parameter: *prop          the Si2144 property context
  Parameter:  prop_code     the property Id
  Parameter:  *data         an int to store the property data

  Returns:    NO_Si2144_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2144_PackProperty            (Si2144_PropObj   *prop, unsigned int prop_code, int *data) {
    switch (prop_code) {
    #ifdef        Si2144_ACTIVE_LOOP_THROUGH_PROP
     case         Si2144_ACTIVE_LOOP_THROUGH_PROP_CODE:
      *data = (prop->active_loop_through.enable & Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE_MASK) << Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE_LSB ;
     break;
    #endif /*     Si2144_ACTIVE_LOOP_THROUGH_PROP */
    #ifdef        Si2144_CRYSTAL_TRIM_PROP
     case         Si2144_CRYSTAL_TRIM_PROP_CODE:
      *data = (prop->crystal_trim.xo_cap & Si2144_CRYSTAL_TRIM_PROP_XO_CAP_MASK) << Si2144_CRYSTAL_TRIM_PROP_XO_CAP_LSB ;
     break;
    #endif /*     Si2144_CRYSTAL_TRIM_PROP */
    #ifdef        Si2144_DTV_AGC_AUTO_FREEZE_PROP
     case         Si2144_DTV_AGC_AUTO_FREEZE_PROP_CODE:
      *data = (prop->dtv_agc_auto_freeze.thld    & Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD_MASK   ) << Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD_LSB  |
              (prop->dtv_agc_auto_freeze.timeout & Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT_MASK) << Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT_LSB ;
     break;
    #endif /*     Si2144_DTV_AGC_AUTO_FREEZE_PROP */
    #ifdef        Si2144_DTV_AGC_SPEED_PROP
     case         Si2144_DTV_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_agc_speed.if_agc_speed & Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK) << Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB  |
              (prop->dtv_agc_speed.agc_decim    & Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM_MASK   ) << Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM_LSB ;
     break;
    #endif /*     Si2144_DTV_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_CONFIG_IF_PORT_PROP
     case         Si2144_DTV_CONFIG_IF_PORT_PROP_CODE:
      *data = (prop->dtv_config_if_port.dtv_out_type & Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_MASK) << Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LSB ;
     break;
    #endif /*     Si2144_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2144_DTV_EXT_AGC_PROP
     case         Si2144_DTV_EXT_AGC_PROP_CODE:
      *data = (prop->dtv_ext_agc.min_10mv & Si2144_DTV_EXT_AGC_PROP_MIN_10MV_MASK) << Si2144_DTV_EXT_AGC_PROP_MIN_10MV_LSB  |
              (prop->dtv_ext_agc.max_10mv & Si2144_DTV_EXT_AGC_PROP_MAX_10MV_MASK) << Si2144_DTV_EXT_AGC_PROP_MAX_10MV_LSB ;
     break;
    #endif /*     Si2144_DTV_EXT_AGC_PROP */
    #ifdef        Si2144_DTV_IF_AGC_SPEED_PROP
     case         Si2144_DTV_IF_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_if_agc_speed.attack & Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK_MASK) << Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK_LSB  |
              (prop->dtv_if_agc_speed.decay  & Si2144_DTV_IF_AGC_SPEED_PROP_DECAY_MASK ) << Si2144_DTV_IF_AGC_SPEED_PROP_DECAY_LSB ;
     break;
    #endif /*     Si2144_DTV_IF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_initial_agc_speed.if_agc_speed & Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_MASK) << Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_LSB  |
              (prop->dtv_initial_agc_speed.agc_decim    & Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_MASK   ) << Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_LSB ;
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
      *data = (prop->dtv_initial_agc_speed_period.period & Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_MASK) << Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_LSB ;
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2144_DTV_INTERNAL_ZIF_PROP
     case         Si2144_DTV_INTERNAL_ZIF_PROP_CODE:
      *data = (prop->dtv_internal_zif.atsc   & Si2144_DTV_INTERNAL_ZIF_PROP_ATSC_MASK  ) << Si2144_DTV_INTERNAL_ZIF_PROP_ATSC_LSB  |
              (prop->dtv_internal_zif.qam_us & Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US_MASK) << Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US_LSB  |
              (prop->dtv_internal_zif.dvbt   & Si2144_DTV_INTERNAL_ZIF_PROP_DVBT_MASK  ) << Si2144_DTV_INTERNAL_ZIF_PROP_DVBT_LSB  |
              (prop->dtv_internal_zif.dvbc   & Si2144_DTV_INTERNAL_ZIF_PROP_DVBC_MASK  ) << Si2144_DTV_INTERNAL_ZIF_PROP_DVBC_LSB  |
              (prop->dtv_internal_zif.isdbt  & Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT_MASK ) << Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT_LSB  |
              (prop->dtv_internal_zif.isdbc  & Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC_MASK ) << Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC_LSB  |
              (prop->dtv_internal_zif.dtmb   & Si2144_DTV_INTERNAL_ZIF_PROP_DTMB_MASK  ) << Si2144_DTV_INTERNAL_ZIF_PROP_DTMB_LSB ;
     break;
    #endif /*     Si2144_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2144_DTV_LIF_FREQ_PROP
     case         Si2144_DTV_LIF_FREQ_PROP_CODE:
      *data = (prop->dtv_lif_freq.offset & Si2144_DTV_LIF_FREQ_PROP_OFFSET_MASK) << Si2144_DTV_LIF_FREQ_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2144_DTV_LIF_FREQ_PROP */
    #ifdef        Si2144_DTV_LIF_OUT_PROP
     case         Si2144_DTV_LIF_OUT_PROP_CODE:
      *data = (prop->dtv_lif_out.offset & Si2144_DTV_LIF_OUT_PROP_OFFSET_MASK) << Si2144_DTV_LIF_OUT_PROP_OFFSET_LSB  |
              (prop->dtv_lif_out.amp    & Si2144_DTV_LIF_OUT_PROP_AMP_MASK   ) << Si2144_DTV_LIF_OUT_PROP_AMP_LSB ;
     break;
    #endif /*     Si2144_DTV_LIF_OUT_PROP */
    #ifdef        Si2144_DTV_MODE_PROP
     case         Si2144_DTV_MODE_PROP_CODE:
      *data = (prop->dtv_mode.bw              & Si2144_DTV_MODE_PROP_BW_MASK             ) << Si2144_DTV_MODE_PROP_BW_LSB  |
              (prop->dtv_mode.modulation      & Si2144_DTV_MODE_PROP_MODULATION_MASK     ) << Si2144_DTV_MODE_PROP_MODULATION_LSB  |
              (prop->dtv_mode.invert_spectrum & Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_MASK) << Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_LSB ;
     break;
    #endif /*     Si2144_DTV_MODE_PROP */
    #ifdef        Si2144_DTV_PGA_LIMITS_PROP
     case         Si2144_DTV_PGA_LIMITS_PROP_CODE:
      *data = (prop->dtv_pga_limits.min & Si2144_DTV_PGA_LIMITS_PROP_MIN_MASK) << Si2144_DTV_PGA_LIMITS_PROP_MIN_LSB  |
              (prop->dtv_pga_limits.max & Si2144_DTV_PGA_LIMITS_PROP_MAX_MASK) << Si2144_DTV_PGA_LIMITS_PROP_MAX_LSB ;
     break;
    #endif /*     Si2144_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2144_DTV_PGA_TARGET_PROP
     case         Si2144_DTV_PGA_TARGET_PROP_CODE:
      *data = (prop->dtv_pga_target.pga_target      & Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET_MASK     ) << Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET_LSB  |
              (prop->dtv_pga_target.override_enable & Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK) << Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB ;
     break;
    #endif /*     Si2144_DTV_PGA_TARGET_PROP */
    #ifdef        Si2144_DTV_RF_AGC_SPEED_PROP
     case         Si2144_DTV_RF_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_rf_agc_speed.attack & Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK_MASK) << Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK_LSB  |
              (prop->dtv_rf_agc_speed.decay  & Si2144_DTV_RF_AGC_SPEED_PROP_DECAY_MASK ) << Si2144_DTV_RF_AGC_SPEED_PROP_DECAY_LSB ;
     break;
    #endif /*     Si2144_DTV_RF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_RF_TOP_PROP
     case         Si2144_DTV_RF_TOP_PROP_CODE:
      *data = (prop->dtv_rf_top.dtv_rf_top & Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP_MASK) << Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP_LSB ;
     break;
    #endif /*     Si2144_DTV_RF_TOP_PROP */
    #ifdef        Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      *data = (prop->dtv_rsq_rssi_threshold.lo & Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK) << Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB ;
     break;
    #endif /*     Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2144_DTV_WB_AGC_SPEED_PROP
     case         Si2144_DTV_WB_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_wb_agc_speed.attack & Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK_MASK) << Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK_LSB  |
              (prop->dtv_wb_agc_speed.decay  & Si2144_DTV_WB_AGC_SPEED_PROP_DECAY_MASK ) << Si2144_DTV_WB_AGC_SPEED_PROP_DECAY_LSB ;
     break;
    #endif /*     Si2144_DTV_WB_AGC_SPEED_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO_PROP
     case         Si2144_TUNER_BLOCKED_VCO_PROP_CODE:
      *data = (prop->tuner_blocked_vco.vco_code & Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE_MASK) << Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE_LSB ;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO2_PROP
     case         Si2144_TUNER_BLOCKED_VCO2_PROP_CODE:
      *data = (prop->tuner_blocked_vco2.vco_code & Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE_MASK) << Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE_LSB ;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO2_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO3_PROP
     case         Si2144_TUNER_BLOCKED_VCO3_PROP_CODE:
      *data = (prop->tuner_blocked_vco3.vco_code & Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE_MASK) << Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE_LSB ;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO3_PROP */
    #ifdef        Si2144_TUNER_LO_INJECTION_PROP
     case         Si2144_TUNER_LO_INJECTION_PROP_CODE:
      *data = (prop->tuner_lo_injection.band_1 & Si2144_TUNER_LO_INJECTION_PROP_BAND_1_MASK) << Si2144_TUNER_LO_INJECTION_PROP_BAND_1_LSB  |
              (prop->tuner_lo_injection.band_2 & Si2144_TUNER_LO_INJECTION_PROP_BAND_2_MASK) << Si2144_TUNER_LO_INJECTION_PROP_BAND_2_LSB  |
              (prop->tuner_lo_injection.band_3 & Si2144_TUNER_LO_INJECTION_PROP_BAND_3_MASK) << Si2144_TUNER_LO_INJECTION_PROP_BAND_3_LSB ;
     break;
    #endif /*     Si2144_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2144_TUNER_RETURN_LOSS_PROP
     case         Si2144_TUNER_RETURN_LOSS_PROP_CODE:
      *data = (Si2144_TUNER_RETURN_LOSS_PROP_RESERVED_RESERVED   & Si2144_TUNER_RETURN_LOSS_PROP_RESERVED_MASK  ) << Si2144_TUNER_RETURN_LOSS_PROP_RESERVED_LSB  |
              (prop->tuner_return_loss.returnloss & Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS_MASK) << Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS_LSB ;
     break;
    #endif /*     Si2144_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2144_XOUT_PROP
     case         Si2144_XOUT_PROP_CODE:
      *data = (prop->xout.amp & Si2144_XOUT_PROP_AMP_MASK) << Si2144_XOUT_PROP_AMP_LSB ;
     break;
    #endif /*     Si2144_XOUT_PROP */
     default : return ERROR_Si2144_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2144_ERROR;
}


/***********************************************************************************************************************
  Si2144_UnpackProperty function
  Use:        This function will unpack all the members of a property from an integer from the GetProperty function.

  Parameter: *prop          the Si2144 property context
  Parameter:  prop_code     the property Id
  Parameter:  data          the property data

  Returns:    NO_Si2144_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2144_UnpackProperty          (Si2144_PropObj   *prop, unsigned int prop_code, int  data) {
    switch (prop_code) {
    #ifdef        Si2144_ACTIVE_LOOP_THROUGH_PROP
     case         Si2144_ACTIVE_LOOP_THROUGH_PROP_CODE:
               prop->active_loop_through.enable = (data >> Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE_LSB) & Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE_MASK;
     break;
    #endif /*     Si2144_ACTIVE_LOOP_THROUGH_PROP */
    #ifdef        Si2144_CRYSTAL_TRIM_PROP
     case         Si2144_CRYSTAL_TRIM_PROP_CODE:
               prop->crystal_trim.xo_cap = (data >> Si2144_CRYSTAL_TRIM_PROP_XO_CAP_LSB) & Si2144_CRYSTAL_TRIM_PROP_XO_CAP_MASK;
     break;
    #endif /*     Si2144_CRYSTAL_TRIM_PROP */
    #ifdef        Si2144_DTV_AGC_AUTO_FREEZE_PROP
     case         Si2144_DTV_AGC_AUTO_FREEZE_PROP_CODE:
               prop->dtv_agc_auto_freeze.thld    = (data >> Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD_LSB   ) & Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD_MASK;
               prop->dtv_agc_auto_freeze.timeout = (data >> Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT_LSB) & Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT_MASK;
     break;
    #endif /*     Si2144_DTV_AGC_AUTO_FREEZE_PROP */
    #ifdef        Si2144_DTV_AGC_SPEED_PROP
     case         Si2144_DTV_AGC_SPEED_PROP_CODE:
               prop->dtv_agc_speed.if_agc_speed = (data >> Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB) & Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK;
               prop->dtv_agc_speed.agc_decim    = (data >> Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM_LSB   ) & Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM_MASK;
     break;
    #endif /*     Si2144_DTV_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_CONFIG_IF_PORT_PROP
     case         Si2144_DTV_CONFIG_IF_PORT_PROP_CODE:
               prop->dtv_config_if_port.dtv_out_type = (data >> Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LSB) & Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_MASK;
     break;
    #endif /*     Si2144_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2144_DTV_EXT_AGC_PROP
     case         Si2144_DTV_EXT_AGC_PROP_CODE:
               prop->dtv_ext_agc.min_10mv = (data >> Si2144_DTV_EXT_AGC_PROP_MIN_10MV_LSB) & Si2144_DTV_EXT_AGC_PROP_MIN_10MV_MASK;
               prop->dtv_ext_agc.max_10mv = (data >> Si2144_DTV_EXT_AGC_PROP_MAX_10MV_LSB) & Si2144_DTV_EXT_AGC_PROP_MAX_10MV_MASK;
     break;
    #endif /*     Si2144_DTV_EXT_AGC_PROP */
    #ifdef        Si2144_DTV_IF_AGC_SPEED_PROP
     case         Si2144_DTV_IF_AGC_SPEED_PROP_CODE:
               prop->dtv_if_agc_speed.attack = (data >> Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK_LSB) & Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK_MASK;
               prop->dtv_if_agc_speed.decay  = (data >> Si2144_DTV_IF_AGC_SPEED_PROP_DECAY_LSB ) & Si2144_DTV_IF_AGC_SPEED_PROP_DECAY_MASK;
     break;
    #endif /*     Si2144_DTV_IF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PROP_CODE:
               prop->dtv_initial_agc_speed.if_agc_speed = (data >> Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_LSB) & Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_MASK;
               prop->dtv_initial_agc_speed.agc_decim    = (data >> Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_LSB   ) & Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_MASK;
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
               prop->dtv_initial_agc_speed_period.period = (data >> Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_LSB) & Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_MASK;
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2144_DTV_INTERNAL_ZIF_PROP
     case         Si2144_DTV_INTERNAL_ZIF_PROP_CODE:
               prop->dtv_internal_zif.atsc   = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_ATSC_LSB  ) & Si2144_DTV_INTERNAL_ZIF_PROP_ATSC_MASK;
               prop->dtv_internal_zif.qam_us = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US_LSB) & Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US_MASK;
               prop->dtv_internal_zif.dvbt   = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_DVBT_LSB  ) & Si2144_DTV_INTERNAL_ZIF_PROP_DVBT_MASK;
               prop->dtv_internal_zif.dvbc   = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_DVBC_LSB  ) & Si2144_DTV_INTERNAL_ZIF_PROP_DVBC_MASK;
               prop->dtv_internal_zif.isdbt  = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT_LSB ) & Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT_MASK;
               prop->dtv_internal_zif.isdbc  = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC_LSB ) & Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC_MASK;
               prop->dtv_internal_zif.dtmb   = (data >> Si2144_DTV_INTERNAL_ZIF_PROP_DTMB_LSB  ) & Si2144_DTV_INTERNAL_ZIF_PROP_DTMB_MASK;
     break;
    #endif /*     Si2144_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2144_DTV_LIF_FREQ_PROP
     case         Si2144_DTV_LIF_FREQ_PROP_CODE:
               prop->dtv_lif_freq.offset = (data >> Si2144_DTV_LIF_FREQ_PROP_OFFSET_LSB) & Si2144_DTV_LIF_FREQ_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2144_DTV_LIF_FREQ_PROP */
    #ifdef        Si2144_DTV_LIF_OUT_PROP
     case         Si2144_DTV_LIF_OUT_PROP_CODE:
               prop->dtv_lif_out.offset = (data >> Si2144_DTV_LIF_OUT_PROP_OFFSET_LSB) & Si2144_DTV_LIF_OUT_PROP_OFFSET_MASK;
               prop->dtv_lif_out.amp    = (data >> Si2144_DTV_LIF_OUT_PROP_AMP_LSB   ) & Si2144_DTV_LIF_OUT_PROP_AMP_MASK;
     break;
    #endif /*     Si2144_DTV_LIF_OUT_PROP */
    #ifdef        Si2144_DTV_MODE_PROP
     case         Si2144_DTV_MODE_PROP_CODE:
               prop->dtv_mode.bw              = (data >> Si2144_DTV_MODE_PROP_BW_LSB             ) & Si2144_DTV_MODE_PROP_BW_MASK;
               prop->dtv_mode.modulation      = (data >> Si2144_DTV_MODE_PROP_MODULATION_LSB     ) & Si2144_DTV_MODE_PROP_MODULATION_MASK;
               prop->dtv_mode.invert_spectrum = (data >> Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_LSB) & Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_MASK;
     break;
    #endif /*     Si2144_DTV_MODE_PROP */
    #ifdef        Si2144_DTV_PGA_LIMITS_PROP
     case         Si2144_DTV_PGA_LIMITS_PROP_CODE:
               prop->dtv_pga_limits.min = (data >> Si2144_DTV_PGA_LIMITS_PROP_MIN_LSB) & Si2144_DTV_PGA_LIMITS_PROP_MIN_MASK;
               prop->dtv_pga_limits.max = (data >> Si2144_DTV_PGA_LIMITS_PROP_MAX_LSB) & Si2144_DTV_PGA_LIMITS_PROP_MAX_MASK;
     break;
    #endif /*     Si2144_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2144_DTV_PGA_TARGET_PROP
     case         Si2144_DTV_PGA_TARGET_PROP_CODE:
               prop->dtv_pga_target.pga_target      = (data >> Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET_LSB     ) & Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET_MASK;
               prop->dtv_pga_target.override_enable = (data >> Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB) & Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK;
     break;
    #endif /*     Si2144_DTV_PGA_TARGET_PROP */
    #ifdef        Si2144_DTV_RF_AGC_SPEED_PROP
     case         Si2144_DTV_RF_AGC_SPEED_PROP_CODE:
               prop->dtv_rf_agc_speed.attack = (data >> Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK_LSB) & Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK_MASK;
               prop->dtv_rf_agc_speed.decay  = (data >> Si2144_DTV_RF_AGC_SPEED_PROP_DECAY_LSB ) & Si2144_DTV_RF_AGC_SPEED_PROP_DECAY_MASK;
     break;
    #endif /*     Si2144_DTV_RF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_RF_TOP_PROP
     case         Si2144_DTV_RF_TOP_PROP_CODE:
               prop->dtv_rf_top.dtv_rf_top = (data >> Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP_LSB) & Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP_MASK;
     break;
    #endif /*     Si2144_DTV_RF_TOP_PROP */
    #ifdef        Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
               prop->dtv_rsq_rssi_threshold.lo = (data >> Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB) & Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK;
     break;
    #endif /*     Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2144_DTV_WB_AGC_SPEED_PROP
     case         Si2144_DTV_WB_AGC_SPEED_PROP_CODE:
               prop->dtv_wb_agc_speed.attack = (data >> Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK_LSB) & Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK_MASK;
               prop->dtv_wb_agc_speed.decay  = (data >> Si2144_DTV_WB_AGC_SPEED_PROP_DECAY_LSB ) & Si2144_DTV_WB_AGC_SPEED_PROP_DECAY_MASK;
     break;
    #endif /*     Si2144_DTV_WB_AGC_SPEED_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO_PROP
     case         Si2144_TUNER_BLOCKED_VCO_PROP_CODE:
               prop->tuner_blocked_vco.vco_code = (data >> Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE_LSB) & Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE_MASK;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO2_PROP
     case         Si2144_TUNER_BLOCKED_VCO2_PROP_CODE:
               prop->tuner_blocked_vco2.vco_code = (data >> Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE_LSB) & Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE_MASK;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO2_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO3_PROP
     case         Si2144_TUNER_BLOCKED_VCO3_PROP_CODE:
               prop->tuner_blocked_vco3.vco_code = (data >> Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE_LSB) & Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE_MASK;
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO3_PROP */
    #ifdef        Si2144_TUNER_LO_INJECTION_PROP
     case         Si2144_TUNER_LO_INJECTION_PROP_CODE:
               prop->tuner_lo_injection.band_1 = (data >> Si2144_TUNER_LO_INJECTION_PROP_BAND_1_LSB) & Si2144_TUNER_LO_INJECTION_PROP_BAND_1_MASK;
               prop->tuner_lo_injection.band_2 = (data >> Si2144_TUNER_LO_INJECTION_PROP_BAND_2_LSB) & Si2144_TUNER_LO_INJECTION_PROP_BAND_2_MASK;
               prop->tuner_lo_injection.band_3 = (data >> Si2144_TUNER_LO_INJECTION_PROP_BAND_3_LSB) & Si2144_TUNER_LO_INJECTION_PROP_BAND_3_MASK;
     break;
    #endif /*     Si2144_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2144_TUNER_RETURN_LOSS_PROP
     case         Si2144_TUNER_RETURN_LOSS_PROP_CODE:
               prop->tuner_return_loss.returnloss = (data >> Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS_LSB) & Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS_MASK;
     break;
    #endif /*     Si2144_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2144_XOUT_PROP
     case         Si2144_XOUT_PROP_CODE:
               prop->xout.amp = (data >> Si2144_XOUT_PROP_AMP_LSB) & Si2144_XOUT_PROP_AMP_MASK;
     break;
    #endif /*     Si2144_XOUT_PROP */
     default : return ERROR_Si2144_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2144_ERROR;
}
/***********************************************************************************************************************
  Si2144_storePropertiesDefaults function
  Use:        property defaults function
              Used to fill the propShadow structure with startup values.
  Parameter: *prop     the Si2144_PropObject structure

 |---------------------------------------------------------------------------------------------------------------------|
 | Do NOT change this code unless you really know what you're doing!                                                   |
 | It should reflect the part internal property settings after firmware download                                       |
 |---------------------------------------------------------------------------------------------------------------------|

 Returns:    void
 ***********************************************************************************************************************/
void          Si2144_storePropertiesDefaults (Si2144_PropObj   *prop) {
#ifdef    Si2144_CRYSTAL_TRIM_PROP
  prop->crystal_trim.xo_cap                  = Si2144_CRYSTAL_TRIM_PROP_XO_CAP_6P7PF ; /* (default '6p7pF') */
#endif /* Si2144_CRYSTAL_TRIM_PROP */

#ifdef    Si2144_XOUT_PROP
  prop->xout.amp                             = Si2144_XOUT_PROP_AMP_HIGH ; /* (default 'HIGH') */
#endif /* Si2144_XOUT_PROP */

#ifdef    Si2144_DTV_AGC_AUTO_FREEZE_PROP
  prop->dtv_agc_auto_freeze.thld             =   255; /* (default   255) */
  prop->dtv_agc_auto_freeze.timeout          =     1; /* (default     1) */
#endif /* Si2144_DTV_AGC_AUTO_FREEZE_PROP */

#ifdef    Si2144_DTV_AGC_SPEED_PROP
  prop->dtv_agc_speed.if_agc_speed           = Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_agc_speed.agc_decim              = Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2144_DTV_AGC_SPEED_PROP */

#ifdef    Si2144_DTV_CONFIG_IF_PORT_PROP
  prop->dtv_config_if_port.dtv_out_type      = Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF ; /* (default 'LIF_IF') */
#endif /* Si2144_DTV_CONFIG_IF_PORT_PROP */

#ifdef    Si2144_DTV_EXT_AGC_PROP
  prop->dtv_ext_agc.min_10mv                 =    50; /* (default    50) */
  prop->dtv_ext_agc.max_10mv                 =   200; /* (default   200) */
#endif /* Si2144_DTV_EXT_AGC_PROP */

#ifdef    Si2144_DTV_IF_AGC_SPEED_PROP
  prop->dtv_if_agc_speed.attack              = Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK_AUTO ; /* (default 'AUTO') */
  prop->dtv_if_agc_speed.decay               = Si2144_DTV_IF_AGC_SPEED_PROP_DECAY_AUTO  ; /* (default 'AUTO') */
#endif /* Si2144_DTV_IF_AGC_SPEED_PROP */

#ifdef    Si2144_DTV_INITIAL_AGC_SPEED_PROP
  prop->dtv_initial_agc_speed.if_agc_speed   = Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_initial_agc_speed.agc_decim      = Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2144_DTV_INITIAL_AGC_SPEED_PROP */

#ifdef    Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  prop->dtv_initial_agc_speed_period.period  =     0; /* (default     0) */
#endif /* Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */

#ifdef    Si2144_DTV_INTERNAL_ZIF_PROP
  prop->dtv_internal_zif.atsc                = Si2144_DTV_INTERNAL_ZIF_PROP_ATSC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.qam_us              = Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US_ZIF ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbt                = Si2144_DTV_INTERNAL_ZIF_PROP_DVBT_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbc                = Si2144_DTV_INTERNAL_ZIF_PROP_DVBC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbt               = Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbc               = Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dtmb                = Si2144_DTV_INTERNAL_ZIF_PROP_DTMB_LIF   ; /* (default 'LIF') */
#endif /* Si2144_DTV_INTERNAL_ZIF_PROP */

#ifdef    Si2144_DTV_LIF_FREQ_PROP
  prop->dtv_lif_freq.offset                  =  5000; /* (default  5000) */
#endif /* Si2144_DTV_LIF_FREQ_PROP */

#ifdef    Si2144_DTV_LIF_OUT_PROP
  prop->dtv_lif_out.offset                   =   148; /* (default   148) */
  prop->dtv_lif_out.amp                      =    27; /* (default    27) */
#endif /* Si2144_DTV_LIF_OUT_PROP */

#ifdef    Si2144_DTV_MODE_PROP
  prop->dtv_mode.bw                          = Si2144_DTV_MODE_PROP_BW_BW_8MHZ              ; /* (default 'BW_8MHZ') */
  prop->dtv_mode.modulation                  = Si2144_DTV_MODE_PROP_MODULATION_DVBT         ; /* (default 'DVBT') */
  prop->dtv_mode.invert_spectrum             = Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL  ; /* (default 'NORMAL') */
#endif /* Si2144_DTV_MODE_PROP */

#ifdef    Si2144_DTV_PGA_LIMITS_PROP
  prop->dtv_pga_limits.min                   =    -1; /* (default    -1) */
  prop->dtv_pga_limits.max                   =    -1; /* (default    -1) */
#endif /* Si2144_DTV_PGA_LIMITS_PROP */

#ifdef    Si2144_DTV_PGA_TARGET_PROP
  prop->dtv_pga_target.pga_target            =     0; /* (default     0) */
  prop->dtv_pga_target.override_enable       = Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2144_DTV_PGA_TARGET_PROP */

#ifdef    Si2144_DTV_RF_AGC_SPEED_PROP
  prop->dtv_rf_agc_speed.attack              = Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK_AUTO ; /* (default 'AUTO') */
  prop->dtv_rf_agc_speed.decay               = Si2144_DTV_RF_AGC_SPEED_PROP_DECAY_AUTO  ; /* (default 'AUTO') */
#endif /* Si2144_DTV_RF_AGC_SPEED_PROP */

#ifdef    Si2144_DTV_RF_TOP_PROP
  prop->dtv_rf_top.dtv_rf_top                = Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO ; /* (default 'AUTO') */
#endif /* Si2144_DTV_RF_TOP_PROP */

#ifdef    Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP
  prop->dtv_rsq_rssi_threshold.lo            =   -80; /* (default   -80) */
#endif /* Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP */

#ifdef    Si2144_DTV_WB_AGC_SPEED_PROP
  prop->dtv_wb_agc_speed.attack              = Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK_AUTO ; /* (default 'AUTO') */
  prop->dtv_wb_agc_speed.decay               = Si2144_DTV_WB_AGC_SPEED_PROP_DECAY_AUTO  ; /* (default 'AUTO') */
#endif /* Si2144_DTV_WB_AGC_SPEED_PROP */

#ifdef    Si2144_ACTIVE_LOOP_THROUGH_PROP
  prop->active_loop_through.enable           = Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE_ENABLE ; /* (default 'ENABLE') */
#endif /* Si2144_ACTIVE_LOOP_THROUGH_PROP */

#ifdef    Si2144_TUNER_BLOCKED_VCO_PROP
  prop->tuner_blocked_vco.vco_code           = 0x8000; /* (default 0x8000) */
#endif /* Si2144_TUNER_BLOCKED_VCO_PROP */

#ifdef    Si2144_TUNER_BLOCKED_VCO2_PROP
  prop->tuner_blocked_vco2.vco_code          = 0x8000; /* (default 0x8000) */
#endif /* Si2144_TUNER_BLOCKED_VCO2_PROP */

#ifdef    Si2144_TUNER_BLOCKED_VCO3_PROP
  prop->tuner_blocked_vco3.vco_code          = 0x8000; /* (default 0x8000) */
#endif /* Si2144_TUNER_BLOCKED_VCO3_PROP */

#ifdef    Si2144_TUNER_LO_INJECTION_PROP
  prop->tuner_lo_injection.band_1            = Si2144_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE ; /* (default 'HIGH_SIDE') */
  prop->tuner_lo_injection.band_2            = Si2144_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ; /* (default 'LOW_SIDE') */
  prop->tuner_lo_injection.band_3            = Si2144_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ; /* (default 'LOW_SIDE') */
#endif /* Si2144_TUNER_LO_INJECTION_PROP */

#ifdef    Si2144_TUNER_RETURN_LOSS_PROP
  prop->tuner_return_loss.returnloss         = Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS_RETURNLOSS_8DB ; /* (default 'RETURNLOSS_8DB') */
#endif /* Si2144_TUNER_RETURN_LOSS_PROP */

}
#ifdef    Si2144_GET_PROPERTY_STRING
/***********************************************************************************************************************
  Si2144_L1_PropertyText function
  Use:        property text function
              Used to turn the property data into clear text.
  Parameter: *prop     the Si2144 property structure (containing all properties)
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2144_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2144_L1_PropertyText          (Si2144_PropObj   *prop, unsigned int prop_code, const char *separator, char *msg) {
    switch (prop_code) {
    #ifdef        Si2144_ACTIVE_LOOP_THROUGH_PROP
     case         Si2144_ACTIVE_LOOP_THROUGH_PROP_CODE:
      snprintf(msg,1000,"ACTIVE_LOOP_THROUGH");
       strncat(msg,separator,1000); strncat(msg,"-ENABLE ",1000);
           if  (prop->active_loop_through.enable ==     0) strncat(msg,"DISABLE ",1000);
      else if  (prop->active_loop_through.enable ==     1) strncat(msg,"ENABLE  ",1000);
      else                                                STRING_APPEND_SAFE(msg,1000,"%d", prop->active_loop_through.enable);
     break;
    #endif /*     Si2144_ACTIVE_LOOP_THROUGH_PROP */
    #ifdef        Si2144_CRYSTAL_TRIM_PROP
     case         Si2144_CRYSTAL_TRIM_PROP_CODE:
      snprintf(msg,1000,"CRYSTAL_TRIM");
       strncat(msg,separator,1000); strncat(msg,"-XO_CAP ",1000);
           if  (prop->crystal_trim.xo_cap ==     0) strncat(msg,"4p7pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==     1) strncat(msg,"4p95pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==     2) strncat(msg,"5p2pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==     3) strncat(msg,"5p45pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==     4) strncat(msg,"5p7pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==     5) strncat(msg,"5p95pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==     6) strncat(msg,"6p2pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==     7) strncat(msg,"6p45pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==     8) strncat(msg,"6p7pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==     9) strncat(msg,"6p95pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==    10) strncat(msg,"7p2pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==    11) strncat(msg,"7p45pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==    12) strncat(msg,"7p7pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==    13) strncat(msg,"7p95pF ",1000);
      else if  (prop->crystal_trim.xo_cap ==    14) strncat(msg,"8p2pF  ",1000);
      else if  (prop->crystal_trim.xo_cap ==    15) strncat(msg,"8p45pF ",1000);
      else                                         STRING_APPEND_SAFE(msg,1000,"%d", prop->crystal_trim.xo_cap);
     break;
    #endif /*     Si2144_CRYSTAL_TRIM_PROP */
    #ifdef        Si2144_DTV_AGC_AUTO_FREEZE_PROP
     case         Si2144_DTV_AGC_AUTO_FREEZE_PROP_CODE:
      snprintf(msg,1000,"DTV_AGC_AUTO_FREEZE");
       strncat(msg,separator,1000); strncat(msg,"-THLD ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_agc_auto_freeze.thld);
       strncat(msg,separator,1000); strncat(msg,"-TIMEOUT ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_agc_auto_freeze.timeout);
     break;
    #endif /*     Si2144_DTV_AGC_AUTO_FREEZE_PROP */
    #ifdef        Si2144_DTV_AGC_SPEED_PROP
     case         Si2144_DTV_AGC_SPEED_PROP_CODE:
      snprintf(msg,1000,"DTV_AGC_SPEED");
       strncat(msg,separator,1000); strncat(msg,"-IF_AGC_SPEED ",1000);
           if  (prop->dtv_agc_speed.if_agc_speed ==     0) strncat(msg,"AUTO ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==    39) strncat(msg,"39   ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==    54) strncat(msg,"54   ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==    63) strncat(msg,"63   ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==    89) strncat(msg,"89   ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   105) strncat(msg,"105  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   121) strncat(msg,"121  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   137) strncat(msg,"137  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   158) strncat(msg,"158  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   172) strncat(msg,"172  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   185) strncat(msg,"185  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   196) strncat(msg,"196  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   206) strncat(msg,"206  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   216) strncat(msg,"216  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   219) strncat(msg,"219  ",1000);
      else if  (prop->dtv_agc_speed.if_agc_speed ==   222) strncat(msg,"222  ",1000);
      else                                                STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_agc_speed.if_agc_speed);
       strncat(msg,separator,1000); strncat(msg,"-AGC_DECIM ",1000);
           if  (prop->dtv_agc_speed.agc_decim    ==     0) strncat(msg,"OFF ",1000);
      else if  (prop->dtv_agc_speed.agc_decim    ==     1) strncat(msg,"2   ",1000);
      else if  (prop->dtv_agc_speed.agc_decim    ==     2) strncat(msg,"4   ",1000);
      else if  (prop->dtv_agc_speed.agc_decim    ==     3) strncat(msg,"8   ",1000);
      else                                                STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_agc_speed.agc_decim);
     break;
    #endif /*     Si2144_DTV_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_CONFIG_IF_PORT_PROP
     case         Si2144_DTV_CONFIG_IF_PORT_PROP_CODE:
      snprintf(msg,1000,"DTV_CONFIG_IF_PORT");
       strncat(msg,separator,1000); strncat(msg,"-DTV_OUT_TYPE ",1000);
           if  (prop->dtv_config_if_port.dtv_out_type ==     0) strncat(msg,"LIF_IF     ",1000);
      else if  (prop->dtv_config_if_port.dtv_out_type ==     4) strncat(msg,"LIF_SE_IFA ",1000);
      else                                                     STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_config_if_port.dtv_out_type);
     break;
    #endif /*     Si2144_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2144_DTV_EXT_AGC_PROP
     case         Si2144_DTV_EXT_AGC_PROP_CODE:
      snprintf(msg,1000,"DTV_EXT_AGC");
       strncat(msg,separator,1000); strncat(msg,"-MIN_10MV ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_ext_agc.min_10mv);
       strncat(msg,separator,1000); strncat(msg,"-MAX_10MV ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_ext_agc.max_10mv);
     break;
    #endif /*     Si2144_DTV_EXT_AGC_PROP */
    #ifdef        Si2144_DTV_IF_AGC_SPEED_PROP
     case         Si2144_DTV_IF_AGC_SPEED_PROP_CODE:
      snprintf(msg,1000,"DTV_IF_AGC_SPEED");
       strncat(msg,separator,1000); strncat(msg,"-ATTACK ",1000);
           if  (prop->dtv_if_agc_speed.attack ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_if_agc_speed.attack);
       strncat(msg,separator,1000); strncat(msg,"-DECAY ",1000);
           if  (prop->dtv_if_agc_speed.decay  ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_if_agc_speed.decay);
     break;
    #endif /*     Si2144_DTV_IF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PROP_CODE:
      snprintf(msg,1000,"DTV_INITIAL_AGC_SPEED");
       strncat(msg,separator,1000); strncat(msg,"-IF_AGC_SPEED ",1000);
           if  (prop->dtv_initial_agc_speed.if_agc_speed ==     0) strncat(msg,"AUTO ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    39) strncat(msg,"39   ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    54) strncat(msg,"54   ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    63) strncat(msg,"63   ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    89) strncat(msg,"89   ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   105) strncat(msg,"105  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   121) strncat(msg,"121  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   137) strncat(msg,"137  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   158) strncat(msg,"158  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   172) strncat(msg,"172  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   185) strncat(msg,"185  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   196) strncat(msg,"196  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   206) strncat(msg,"206  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   216) strncat(msg,"216  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   219) strncat(msg,"219  ",1000);
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   222) strncat(msg,"222  ",1000);
      else                                                        STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_initial_agc_speed.if_agc_speed);
       strncat(msg,separator,1000); strncat(msg,"-AGC_DECIM ",1000);
           if  (prop->dtv_initial_agc_speed.agc_decim    ==     0) strncat(msg,"OFF ",1000);
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     1) strncat(msg,"2   ",1000);
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     2) strncat(msg,"4   ",1000);
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     3) strncat(msg,"8   ",1000);
      else                                                        STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_initial_agc_speed.agc_decim);
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
      snprintf(msg,1000,"DTV_INITIAL_AGC_SPEED_PERIOD");
       strncat(msg,separator,1000); strncat(msg,"-PERIOD ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_initial_agc_speed_period.period);
     break;
    #endif /*     Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2144_DTV_INTERNAL_ZIF_PROP
     case         Si2144_DTV_INTERNAL_ZIF_PROP_CODE:
      snprintf(msg,1000,"DTV_INTERNAL_ZIF");
       strncat(msg,separator,1000); strncat(msg,"-ATSC ",1000);
           if  (prop->dtv_internal_zif.atsc   ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.atsc   ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.atsc);
       strncat(msg,separator,1000); strncat(msg,"-QAM_US ",1000);
           if  (prop->dtv_internal_zif.qam_us ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.qam_us ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.qam_us);
       strncat(msg,separator,1000); strncat(msg,"-DVBT ",1000);
           if  (prop->dtv_internal_zif.dvbt   ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.dvbt   ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.dvbt);
       strncat(msg,separator,1000); strncat(msg,"-DVBC ",1000);
           if  (prop->dtv_internal_zif.dvbc   ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.dvbc   ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.dvbc);
       strncat(msg,separator,1000); strncat(msg,"-ISDBT ",1000);
           if  (prop->dtv_internal_zif.isdbt  ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.isdbt  ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.isdbt);
       strncat(msg,separator,1000); strncat(msg,"-ISDBC ",1000);
           if  (prop->dtv_internal_zif.isdbc  ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.isdbc  ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.isdbc);
       strncat(msg,separator,1000); strncat(msg,"-DTMB ",1000);
           if  (prop->dtv_internal_zif.dtmb   ==     0) strncat(msg,"LIF ",1000);
      else if  (prop->dtv_internal_zif.dtmb   ==     1) strncat(msg,"ZIF ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_internal_zif.dtmb);
     break;
    #endif /*     Si2144_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2144_DTV_LIF_FREQ_PROP
     case         Si2144_DTV_LIF_FREQ_PROP_CODE:
      snprintf(msg,1000,"DTV_LIF_FREQ");
       strncat(msg,separator,1000); strncat(msg,"-OFFSET ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_lif_freq.offset);
     break;
    #endif /*     Si2144_DTV_LIF_FREQ_PROP */
    #ifdef        Si2144_DTV_LIF_OUT_PROP
     case         Si2144_DTV_LIF_OUT_PROP_CODE:
      snprintf(msg,1000,"DTV_LIF_OUT");
       strncat(msg,separator,1000); strncat(msg,"-OFFSET ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_lif_out.offset);
       strncat(msg,separator,1000); strncat(msg,"-AMP ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_lif_out.amp);
     break;
    #endif /*     Si2144_DTV_LIF_OUT_PROP */
    #ifdef        Si2144_DTV_MODE_PROP
     case         Si2144_DTV_MODE_PROP_CODE:
      snprintf(msg,1000,"DTV_MODE");
       strncat(msg,separator,1000); strncat(msg,"-BW ",1000);
           if  (prop->dtv_mode.bw              ==     6) strncat(msg,"BW_6MHZ   ",1000);
      else if  (prop->dtv_mode.bw              ==     7) strncat(msg,"BW_7MHZ   ",1000);
      else if  (prop->dtv_mode.bw              ==     8) strncat(msg,"BW_8MHZ   ",1000);
      else if  (prop->dtv_mode.bw              ==     9) strncat(msg,"BW_1P7MHZ ",1000);
      else if  (prop->dtv_mode.bw              ==    10) strncat(msg,"BW_6P1MHZ ",1000);
      else                                              STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_mode.bw);
       strncat(msg,separator,1000); strncat(msg,"-MODULATION ",1000);
           if  (prop->dtv_mode.modulation      ==     0) strncat(msg,"ATSC      ",1000);
      else if  (prop->dtv_mode.modulation      ==     1) strncat(msg,"QAM_US    ",1000);
      else if  (prop->dtv_mode.modulation      ==     2) strncat(msg,"DVBT      ",1000);
      else if  (prop->dtv_mode.modulation      ==     3) strncat(msg,"DVBC      ",1000);
      else if  (prop->dtv_mode.modulation      ==     4) strncat(msg,"ISDBT     ",1000);
      else if  (prop->dtv_mode.modulation      ==     5) strncat(msg,"ISDBC     ",1000);
      else if  (prop->dtv_mode.modulation      ==     6) strncat(msg,"DTMB      ",1000);
      else if  (prop->dtv_mode.modulation      ==    14) strncat(msg,"CW_LEGACY ",1000);
      else if  (prop->dtv_mode.modulation      ==    15) strncat(msg,"CW        ",1000);
      else                                              STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_mode.modulation);
       strncat(msg,separator,1000); strncat(msg,"-INVERT_SPECTRUM ",1000);
           if  (prop->dtv_mode.invert_spectrum ==     0) strncat(msg,"NORMAL   ",1000);
      else if  (prop->dtv_mode.invert_spectrum ==     1) strncat(msg,"INVERTED ",1000);
      else                                              STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_mode.invert_spectrum);
     break;
    #endif /*     Si2144_DTV_MODE_PROP */
    #ifdef        Si2144_DTV_PGA_LIMITS_PROP
     case         Si2144_DTV_PGA_LIMITS_PROP_CODE:
      snprintf(msg,1000,"DTV_PGA_LIMITS");
       strncat(msg,separator,1000); strncat(msg,"-MIN ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_pga_limits.min);
       strncat(msg,separator,1000); strncat(msg,"-MAX ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_pga_limits.max);
     break;
    #endif /*     Si2144_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2144_DTV_PGA_TARGET_PROP
     case         Si2144_DTV_PGA_TARGET_PROP_CODE:
      snprintf(msg,1000,"DTV_PGA_TARGET");
       strncat(msg,separator,1000); strncat(msg,"-PGA_TARGET ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_pga_target.pga_target);
       strncat(msg,separator,1000); strncat(msg,"-OVERRIDE_ENABLE ",1000);
           if  (prop->dtv_pga_target.override_enable ==     0) strncat(msg,"DISABLE ",1000);
      else if  (prop->dtv_pga_target.override_enable ==     1) strncat(msg,"ENABLE  ",1000);
      else                                                    STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_pga_target.override_enable);
     break;
    #endif /*     Si2144_DTV_PGA_TARGET_PROP */
    #ifdef        Si2144_DTV_RF_AGC_SPEED_PROP
     case         Si2144_DTV_RF_AGC_SPEED_PROP_CODE:
      snprintf(msg,1000,"DTV_RF_AGC_SPEED");
       strncat(msg,separator,1000); strncat(msg,"-ATTACK ",1000);
           if  (prop->dtv_rf_agc_speed.attack ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_rf_agc_speed.attack);
       strncat(msg,separator,1000); strncat(msg,"-DECAY ",1000);
           if  (prop->dtv_rf_agc_speed.decay  ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_rf_agc_speed.decay);
     break;
    #endif /*     Si2144_DTV_RF_AGC_SPEED_PROP */
    #ifdef        Si2144_DTV_RF_TOP_PROP
     case         Si2144_DTV_RF_TOP_PROP_CODE:
      snprintf(msg,1000,"DTV_RF_TOP");
       strncat(msg,separator,1000); strncat(msg,"-DTV_RF_TOP ",1000);
           if  (prop->dtv_rf_top.dtv_rf_top ==     0) strncat(msg,"AUTO    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     1) strncat(msg,"P10DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     2) strncat(msg,"P9P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     3) strncat(msg,"P9DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     4) strncat(msg,"P8P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     5) strncat(msg,"P8DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     6) strncat(msg,"P7P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     7) strncat(msg,"P7DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     8) strncat(msg,"P6P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==     9) strncat(msg,"P6DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    10) strncat(msg,"P5P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    11) strncat(msg,"P5DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    12) strncat(msg,"P4P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    13) strncat(msg,"P4DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    14) strncat(msg,"P3P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    15) strncat(msg,"P3DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    16) strncat(msg,"P2P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    17) strncat(msg,"P2DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    18) strncat(msg,"P1P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    19) strncat(msg,"P1DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    20) strncat(msg,"P0P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    21) strncat(msg,"0DB     ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    22) strncat(msg,"M0P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    23) strncat(msg,"M1DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    24) strncat(msg,"M1P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    25) strncat(msg,"M2DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    26) strncat(msg,"M2P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    27) strncat(msg,"M3DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    28) strncat(msg,"M3P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    29) strncat(msg,"M4DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    30) strncat(msg,"M4P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    31) strncat(msg,"M5DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    32) strncat(msg,"M5P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    33) strncat(msg,"M6DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    34) strncat(msg,"M6P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    35) strncat(msg,"M7DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    36) strncat(msg,"M7P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    37) strncat(msg,"M8DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    38) strncat(msg,"M8P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    39) strncat(msg,"M9DB    ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    40) strncat(msg,"M9P5DB  ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    41) strncat(msg,"M10DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    42) strncat(msg,"M10P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    43) strncat(msg,"M11DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    44) strncat(msg,"M11P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    45) strncat(msg,"M12DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    46) strncat(msg,"M12P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    47) strncat(msg,"M13DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    48) strncat(msg,"M13P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    49) strncat(msg,"M14DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    50) strncat(msg,"M14P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    51) strncat(msg,"M15DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    52) strncat(msg,"M15P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    53) strncat(msg,"M16DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    54) strncat(msg,"M16P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    55) strncat(msg,"M17DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    56) strncat(msg,"M17P5DB ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    57) strncat(msg,"M18DB   ",1000);
      else if  (prop->dtv_rf_top.dtv_rf_top ==    58) strncat(msg,"M18P5DB ",1000);
      else                                           STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_rf_top.dtv_rf_top);
     break;
    #endif /*     Si2144_DTV_RF_TOP_PROP */
    #ifdef        Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      snprintf(msg,1000,"DTV_RSQ_RSSI_THRESHOLD");
       strncat(msg,separator,1000); strncat(msg,"-LO ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_rsq_rssi_threshold.lo);
     break;
    #endif /*     Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2144_DTV_WB_AGC_SPEED_PROP
     case         Si2144_DTV_WB_AGC_SPEED_PROP_CODE:
      snprintf(msg,1000,"DTV_WB_AGC_SPEED");
       strncat(msg,separator,1000); strncat(msg,"-ATTACK ",1000);
           if  (prop->dtv_wb_agc_speed.attack ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_wb_agc_speed.attack);
       strncat(msg,separator,1000); strncat(msg,"-DECAY ",1000);
           if  (prop->dtv_wb_agc_speed.decay  ==     0) strncat(msg,"AUTO ",1000);
      else                                             STRING_APPEND_SAFE(msg,1000,"%d", prop->dtv_wb_agc_speed.decay);
     break;
    #endif /*     Si2144_DTV_WB_AGC_SPEED_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO_PROP
     case         Si2144_TUNER_BLOCKED_VCO_PROP_CODE:
      snprintf(msg,1000,"TUNER_BLOCKED_VCO");
       strncat(msg,separator,1000); strncat(msg,"-VCO_CODE ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_blocked_vco.vco_code);
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO2_PROP
     case         Si2144_TUNER_BLOCKED_VCO2_PROP_CODE:
      snprintf(msg,1000,"TUNER_BLOCKED_VCO2");
       strncat(msg,separator,1000); strncat(msg,"-VCO_CODE ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_blocked_vco2.vco_code);
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO2_PROP */
    #ifdef        Si2144_TUNER_BLOCKED_VCO3_PROP
     case         Si2144_TUNER_BLOCKED_VCO3_PROP_CODE:
      snprintf(msg,1000,"TUNER_BLOCKED_VCO3");
       strncat(msg,separator,1000); strncat(msg,"-VCO_CODE ",1000); STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_blocked_vco3.vco_code);
     break;
    #endif /*     Si2144_TUNER_BLOCKED_VCO3_PROP */
    #ifdef        Si2144_TUNER_LO_INJECTION_PROP
     case         Si2144_TUNER_LO_INJECTION_PROP_CODE:
      snprintf(msg,1000,"TUNER_LO_INJECTION");
       strncat(msg,separator,1000); strncat(msg,"-BAND_1 ",1000);
           if  (prop->tuner_lo_injection.band_1 ==     0) strncat(msg,"LOW_SIDE  ",1000);
      else if  (prop->tuner_lo_injection.band_1 ==     1) strncat(msg,"HIGH_SIDE ",1000);
      else                                               STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_lo_injection.band_1);
       strncat(msg,separator,1000); strncat(msg,"-BAND_2 ",1000);
           if  (prop->tuner_lo_injection.band_2 ==     0) strncat(msg,"LOW_SIDE  ",1000);
      else if  (prop->tuner_lo_injection.band_2 ==     1) strncat(msg,"HIGH_SIDE ",1000);
      else                                               STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_lo_injection.band_2);
       strncat(msg,separator,1000); strncat(msg,"-BAND_3 ",1000);
           if  (prop->tuner_lo_injection.band_3 ==     0) strncat(msg,"LOW_SIDE  ",1000);
      else if  (prop->tuner_lo_injection.band_3 ==     1) strncat(msg,"HIGH_SIDE ",1000);
      else                                               STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_lo_injection.band_3);
     break;
    #endif /*     Si2144_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2144_TUNER_RETURN_LOSS_PROP
     case         Si2144_TUNER_RETURN_LOSS_PROP_CODE:
      snprintf(msg,1000,"TUNER_RETURN_LOSS");
       strncat(msg,separator,1000); strncat(msg,"-RETURNLOSS ",1000);
           if  (prop->tuner_return_loss.returnloss ==   112) strncat(msg,"RETURNLOSS_5DB   ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    96) strncat(msg,"RETURNLOSS_5P5DB ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    64) strncat(msg,"RETURNLOSS_6DB   ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    48) strncat(msg,"RETURNLOSS_6P5DB ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    85) strncat(msg,"RETURNLOSS_7DB   ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    69) strncat(msg,"RETURNLOSS_7P5DB ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    90) strncat(msg,"RETURNLOSS_8DB   ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    74) strncat(msg,"RETURNLOSS_8P5DB ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    58) strncat(msg,"RETURNLOSS_9DB   ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    21) strncat(msg,"RETURNLOSS_9P5DB ",1000);
      else if  (prop->tuner_return_loss.returnloss ==    42) strncat(msg,"RETURNLOSS_10DB  ",1000);
      else                                                  STRING_APPEND_SAFE(msg,1000,"%d", prop->tuner_return_loss.returnloss);
     break;
    #endif /*     Si2144_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2144_XOUT_PROP
     case         Si2144_XOUT_PROP_CODE:
      snprintf(msg,1000,"XOUT");
       strncat(msg,separator,1000); strncat(msg,"-AMP ",1000);
           if  (prop->xout.amp ==     0) strncat(msg,"HIGH ",1000);
      else if  (prop->xout.amp ==     1) strncat(msg,"LOW  ",1000);
      else                              STRING_APPEND_SAFE(msg,1000,"%d", prop->xout.amp);
     break;
    #endif /*     Si2144_XOUT_PROP */
     default : STRING_APPEND_SAFE(msg,1000,"Unknown property code '0x%06x'\n", prop_code); return ERROR_Si2144_UNKNOWN_PROPERTY; break;
  }
  return NO_Si2144_ERROR;
}
/***********************************************************************************************************************
  Si2144_L1_FillPropertyStringText function
  Use:        property text retrieval function
              Used to retrieve the property text for a selected property.
  Parameter: *api      the Si2144 context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2144_ERROR if successful.
 ***********************************************************************************************************************/
void          Si2144_L1_FillPropertyStringText(L1_Si2144_Context *api, unsigned int prop_code, const char *separator, char *msg) {
  Si2144_L1_PropertyText (api->prop, prop_code, separator, msg);
}
/***********************************************************************************************************************
  Si2144_L1_GetPropertyString function
  Use:        current property text retrieval function
              Used to retrieve the property value from the hardware then retrieve the corresponding property text.
  Parameter: *api      the Si2144 context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2144_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2144_L1_GetPropertyString     (L1_Si2144_Context *api, unsigned int prop_code, const char *separator, char *msg) {
    int res;
    res = Si2144_L1_GetProperty2(api,prop_code);
    if (res!=NO_Si2144_ERROR) { STRING_APPEND_SAFE(msg,1000, "%s",Si2144_L1_API_ERROR_TEXT(res)); return res; }
    Si2144_L1_PropertyText(api->prop, prop_code, separator, msg);
    return NO_Si2144_ERROR;
}
#endif /* Si2144_GET_PROPERTY_STRING */








