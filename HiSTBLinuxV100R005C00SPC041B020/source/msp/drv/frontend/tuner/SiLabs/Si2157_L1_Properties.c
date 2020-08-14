/*************************************************************************************
                  Silicon Laboratories Broadcast Si2157 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties definitions
   FILE: Si2157_L1_Properties.c
   Supported IC : Si2157
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2157_COMMAND_PROTOTYPES

#include "Si2157_L1_API.h"

/***********************************************************************************************************************
  Si2157_L1_SetProperty function
  Use:        property set function
              Used to call L1_SET_PROPERTY with the property Id and data provided.
  Parameter: *api     the Si2157 context
  Parameter: prop     the property Id
  Parameter: data     the property bytes
  Behavior:  This function will only download the property if required.
               Conditions to download the property are:
                - The property changes
                - The propertyWriteMode is set to Si2157_DOWNLOAD_ALWAYS
                - The property is unknown to Si2157_PackProperty (this may be useful for debug purpose)
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2157_L1_SetProperty         (L1_Si2157_Context *api, unsigned int prop_code, int  data) {
    unsigned char  reserved          = 0;
        return Si2157_L1_SET_PROPERTY (api, reserved, prop_code, data);
}
/***********************************************************************************************************************
  Si2157_L1_GetProperty function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2157 context
  Parameter: prop     the property Id
  Parameter: *data    a buffer to store the property bytes into
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2157_L1_GetProperty         (L1_Si2157_Context *api, unsigned int prop_code, int *data) {
    unsigned char  reserved          = 0;
    unsigned char res;
    res = Si2157_L1_GET_PROPERTY (api, reserved, prop_code);
    *data = api->rsp->get_property.data;
    return res;
}
/***********************************************************************************************************************
  Si2157_L1_SetProperty2 function
  Use:        Sets the property given the property code.
  Parameter: *api     the Si2157 context
  Parameter: prop     the property Id

  Returns:    NO_Si2157_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2157_L1_SetProperty2(L1_Si2157_Context *api, unsigned int prop_code) {
    int data, res;
#ifdef    Si2157_GET_PROPERTY_STRING
    char msg[1000];
#endif /* Si2157_GET_PROPERTY_STRING */
    res = Si2157_PackProperty(api->prop, prop_code, &data);
    if (res != NO_Si2157_ERROR) {
      SiTRACE("\nSi2157_L1_SetProperty2: %s 0x%04x!\n\n", Si2157_L1_API_ERROR_TEXT(res), prop_code);
      return res;
    }
#ifdef    Si2157_GET_PROPERTY_STRING
    Si2157_L1_PropertyText(api->prop, prop_code, (char*)" ", msg);
    SiTRACE("%s\n",msg);
#endif /* Si2157_GET_PROPERTY_STRING */
    return Si2157_L1_SetProperty (api, prop_code & 0xffff, data);
  }
/***********************************************************************************************************************
  Si2157_L1_GetProperty2 function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2157 context
  Parameter: prop     the property Id

  Returns:    NO_Si2157_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2157_L1_GetProperty2        (L1_Si2157_Context *api, unsigned int prop_code) {
  int data, res;
  res = Si2157_L1_GetProperty(api, prop_code & 0xffff, &data);
  if (res != NO_Si2157_ERROR) {
    SiTRACE("Si2157_L1_GetProperty2: %s 0x%04x\n", Si2157_L1_API_ERROR_TEXT(res), prop_code);
    SiERROR(Si2157_L1_API_ERROR_TEXT(res));
    return res;
  }
  return Si2157_UnpackProperty(api->prop, prop_code, data);
}
/*****************************************************************************************
 NAME: Si2157_downloadATVProperties
  DESCRIPTION: Setup Si2157 ATV properties configuration
  This function will download all the ATV configuration properties.
  The function Si2157_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2157 Context
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
  Programming Guide Reference:    ATV setup flowchart
******************************************************************************************/
int  Si2157_downloadATVProperties         (L1_Si2157_Context *api) {
  SiTRACE("Si2157_downloadATVProperties\n");
#ifdef    Si2157_ATV_AFC_RANGE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_AFC_RANGE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_AFC_RANGE_PROP */
#ifdef    Si2157_ATV_AGC_SPEED_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_AGC_SPEED_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_AGC_SPEED_PROP */
#ifdef    Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP */
#ifdef    Si2157_ATV_ARTIFICIAL_SNOW_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_ARTIFICIAL_SNOW_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_ARTIFICIAL_SNOW_PROP */
#ifdef    Si2157_ATV_CONFIG_IF_PORT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_CONFIG_IF_PORT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_CONFIG_IF_PORT_PROP */
#ifdef    Si2157_ATV_EXT_AGC_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_EXT_AGC_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_EXT_AGC_PROP */
#ifdef    Si2157_ATV_HSYNC_OUT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_HSYNC_OUT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_HSYNC_OUT_PROP */
#ifdef    Si2157_ATV_IEN_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_IEN_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_IEN_PROP */
#ifdef    Si2157_ATV_INT_SENSE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_INT_SENSE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_INT_SENSE_PROP */
#ifdef    Si2157_ATV_LIF_FREQ_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_LIF_FREQ_PROP */
#ifdef    Si2157_ATV_LIF_OUT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_LIF_OUT_PROP */
#ifdef    Si2157_ATV_PGA_TARGET_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_PGA_TARGET_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_PGA_TARGET_PROP */
#ifdef    Si2157_ATV_RF_TOP_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_RF_TOP_PROP */
#ifdef    Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP */
#ifdef    Si2157_ATV_VIDEO_MODE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_VIDEO_MODE_PROP */
#ifdef    Si2157_ATV_VSNR_CAP_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_ATV_VSNR_CAP_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_ATV_VSNR_CAP_PROP */
return NO_Si2157_ERROR;
}
/*****************************************************************************************
 NAME: Si2157_downloadCOMMONProperties
  DESCRIPTION: Setup Si2157 COMMON properties configuration
  This function will download all the COMMON configuration properties.
  The function Si2157_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2157 Context
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
  Programming Guide Reference:    COMMON setup flowchart
******************************************************************************************/
int  Si2157_downloadCOMMONProperties      (L1_Si2157_Context *api) {
  SiTRACE("Si2157_downloadCOMMONProperties\n");
#ifdef    Si2157_CRYSTAL_TRIM_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_CRYSTAL_TRIM_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_CRYSTAL_TRIM_PROP */
#ifdef    Si2157_MASTER_IEN_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_MASTER_IEN_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_MASTER_IEN_PROP */
#ifdef    Si2157_XOUT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_XOUT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_XOUT_PROP */
return NO_Si2157_ERROR;
}
/*****************************************************************************************
 NAME: Si2157_downloadDTVProperties
  DESCRIPTION: Setup Si2157 DTV properties configuration
  This function will download all the DTV configuration properties.
  The function Si2157_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2157 Context
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
  Programming Guide Reference:    DTV setup flowchart
******************************************************************************************/
int  Si2157_downloadDTVProperties         (L1_Si2157_Context *api) {
  SiTRACE("Si2157_downloadDTVProperties\n");
#ifdef    Si2157_DTV_AGC_FREEZE_INPUT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_AGC_FREEZE_INPUT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_AGC_FREEZE_INPUT_PROP */
#ifdef    Si2157_DTV_AGC_SPEED_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_AGC_SPEED_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_AGC_SPEED_PROP */
#ifdef    Si2157_DTV_CONFIG_IF_PORT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_CONFIG_IF_PORT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_CONFIG_IF_PORT_PROP */
#ifdef    Si2157_DTV_EXT_AGC_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_EXT_AGC_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_EXT_AGC_PROP */
#ifdef    Si2157_DTV_IEN_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_IEN_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_IEN_PROP */
#ifdef    Si2157_DTV_INITIAL_AGC_SPEED_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_INITIAL_AGC_SPEED_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_INITIAL_AGC_SPEED_PROP */
#ifdef    Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
#ifdef    Si2157_DTV_INTERNAL_ZIF_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_INTERNAL_ZIF_PROP */
#ifdef    Si2157_DTV_INT_SENSE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_INT_SENSE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_INT_SENSE_PROP */
#ifdef    Si2157_DTV_LIF_FREQ_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_LIF_FREQ_PROP */
#ifdef    Si2157_DTV_LIF_OUT_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_LIF_OUT_PROP */
#ifdef    Si2157_DTV_MODE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_MODE_PROP */
#ifdef    Si2157_DTV_PGA_LIMITS_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_PGA_LIMITS_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_PGA_LIMITS_PROP */
#ifdef    Si2157_DTV_PGA_TARGET_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_PGA_TARGET_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_PGA_TARGET_PROP */
#ifdef    Si2157_DTV_RESERVED0_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_RESERVED0_PROP_CODE               ) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_RESERVED0_PROP */
#ifdef    Si2157_DTV_RF_TOP_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_RF_TOP_PROP */
#ifdef    Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP */
#ifdef    Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP */
return NO_Si2157_ERROR;
}
/*****************************************************************************************
 NAME: Si2157_downloadTUNERProperties
  DESCRIPTION: Setup Si2157 TUNER properties configuration
  This function will download all the TUNER configuration properties.
  The function Si2157_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2157 Context
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
  Programming Guide Reference:    TUNER setup flowchart
******************************************************************************************/
int  Si2157_downloadTUNERProperties       (L1_Si2157_Context *api) {
  SiTRACE("Si2157_downloadTUNERProperties\n");
#ifdef    Si2157_TUNER_BLOCKED_VCO_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_TUNER_BLOCKED_VCO_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_TUNER_BLOCKED_VCO_PROP */
#ifdef    Si2157_TUNER_IEN_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_TUNER_IEN_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_TUNER_IEN_PROP */
#ifdef    Si2157_TUNER_INT_SENSE_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_TUNER_INT_SENSE_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_TUNER_INT_SENSE_PROP */
#ifdef    Si2157_TUNER_LO_INJECTION_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_TUNER_LO_INJECTION_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_TUNER_LO_INJECTION_PROP */
#ifdef    Si2157_TUNER_RETURN_LOSS_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_TUNER_RETURN_LOSS_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_TUNER_RETURN_LOSS_PROP */
#ifdef    Si2157_WIDE_BAND_ATT_THRS_PROP
  if (Si2157_L1_SetProperty2(api, Si2157_WIDE_BAND_ATT_THRS_PROP_CODE) != NO_Si2157_ERROR) {return ERROR_Si2157_SENDING_COMMAND;}
#endif /* Si2157_WIDE_BAND_ATT_THRS_PROP */
return NO_Si2157_ERROR;
}
int Si2157_downloadAllProperties         (L1_Si2157_Context *api) {
  Si2157_downloadATVProperties         (api);
  Si2157_downloadCOMMONProperties      (api);
  Si2157_downloadDTVProperties         (api);
  Si2157_downloadTUNERProperties       (api);
  return 0;
}

/***********************************************************************************************************************
  Si2157_PackProperty function
  Use:        This function will pack all the members of a property into an integer for the SetProperty function.

  Parameter: *prop          the Si2157 property context
  Parameter:  prop_code     the property Id
  Parameter:  *data         an int to store the property data

  Returns:    NO_Si2157_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2157_PackProperty            (Si2157_PropObj   *prop, unsigned int prop_code, int *data) {
    switch (prop_code) {
    #ifdef        Si2157_ATV_AFC_RANGE_PROP
     case         Si2157_ATV_AFC_RANGE_PROP_CODE:
      *data = (prop->atv_afc_range.range_khz & Si2157_ATV_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2157_ATV_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2157_ATV_AFC_RANGE_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_PROP
     case         Si2157_ATV_AGC_SPEED_PROP_CODE:
      *data = (prop->atv_agc_speed.if_agc_speed & Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK) << Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB ;
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP
     case         Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_CODE:
      *data = (prop->atv_agc_speed_low_rssi.if_agc_speed & Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED_MASK) << Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED_LSB  |
              (prop->atv_agc_speed_low_rssi.thld         & Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD_MASK        ) << Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD_LSB ;
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP */
    #ifdef        Si2157_ATV_ARTIFICIAL_SNOW_PROP
     case         Si2157_ATV_ARTIFICIAL_SNOW_PROP_CODE:
      *data = (prop->atv_artificial_snow.gain   & Si2157_ATV_ARTIFICIAL_SNOW_PROP_GAIN_MASK  ) << Si2157_ATV_ARTIFICIAL_SNOW_PROP_GAIN_LSB  |
              (prop->atv_artificial_snow.period & Si2157_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_MASK) << Si2157_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_LSB  |
              (prop->atv_artificial_snow.offset & Si2157_ATV_ARTIFICIAL_SNOW_PROP_OFFSET_MASK) << Si2157_ATV_ARTIFICIAL_SNOW_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2157_ATV_ARTIFICIAL_SNOW_PROP */
    #ifdef        Si2157_ATV_CONFIG_IF_PORT_PROP
     case         Si2157_ATV_CONFIG_IF_PORT_PROP_CODE:
      *data = (prop->atv_config_if_port.atv_out_type   & Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_MASK  ) << Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LSB  |
              (prop->atv_config_if_port.atv_agc_source & Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_MASK) << Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_LSB ;
     break;
    #endif /*     Si2157_ATV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_ATV_EXT_AGC_PROP
     case         Si2157_ATV_EXT_AGC_PROP_CODE:
      *data = (prop->atv_ext_agc.min_10mv & Si2157_ATV_EXT_AGC_PROP_MIN_10MV_MASK) << Si2157_ATV_EXT_AGC_PROP_MIN_10MV_LSB  |
              (prop->atv_ext_agc.max_10mv & Si2157_ATV_EXT_AGC_PROP_MAX_10MV_MASK) << Si2157_ATV_EXT_AGC_PROP_MAX_10MV_LSB ;
     break;
    #endif /*     Si2157_ATV_EXT_AGC_PROP */
    #ifdef        Si2157_ATV_HSYNC_OUT_PROP
     case         Si2157_ATV_HSYNC_OUT_PROP_CODE:
      *data = (prop->atv_hsync_out.gpio_sel & Si2157_ATV_HSYNC_OUT_PROP_GPIO_SEL_MASK) << Si2157_ATV_HSYNC_OUT_PROP_GPIO_SEL_LSB  |
              (prop->atv_hsync_out.width    & Si2157_ATV_HSYNC_OUT_PROP_WIDTH_MASK   ) << Si2157_ATV_HSYNC_OUT_PROP_WIDTH_LSB  |
              (prop->atv_hsync_out.offset   & Si2157_ATV_HSYNC_OUT_PROP_OFFSET_MASK  ) << Si2157_ATV_HSYNC_OUT_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2157_ATV_HSYNC_OUT_PROP */
    #ifdef        Si2157_ATV_IEN_PROP
     case         Si2157_ATV_IEN_PROP_CODE:
      *data = (prop->atv_ien.chlien & Si2157_ATV_IEN_PROP_CHLIEN_MASK) << Si2157_ATV_IEN_PROP_CHLIEN_LSB  |
              (prop->atv_ien.pclien & Si2157_ATV_IEN_PROP_PCLIEN_MASK) << Si2157_ATV_IEN_PROP_PCLIEN_LSB ;
     break;
    #endif /*     Si2157_ATV_IEN_PROP */
    #ifdef        Si2157_ATV_INT_SENSE_PROP
     case         Si2157_ATV_INT_SENSE_PROP_CODE:
      *data = (prop->atv_int_sense.chlnegen & Si2157_ATV_INT_SENSE_PROP_CHLNEGEN_MASK) << Si2157_ATV_INT_SENSE_PROP_CHLNEGEN_LSB  |
              (prop->atv_int_sense.pclnegen & Si2157_ATV_INT_SENSE_PROP_PCLNEGEN_MASK) << Si2157_ATV_INT_SENSE_PROP_PCLNEGEN_LSB  |
              (prop->atv_int_sense.chlposen & Si2157_ATV_INT_SENSE_PROP_CHLPOSEN_MASK) << Si2157_ATV_INT_SENSE_PROP_CHLPOSEN_LSB  |
              (prop->atv_int_sense.pclposen & Si2157_ATV_INT_SENSE_PROP_PCLPOSEN_MASK) << Si2157_ATV_INT_SENSE_PROP_PCLPOSEN_LSB ;
     break;
    #endif /*     Si2157_ATV_INT_SENSE_PROP */
    #ifdef        Si2157_ATV_LIF_FREQ_PROP
     case         Si2157_ATV_LIF_FREQ_PROP_CODE:
      *data = (prop->atv_lif_freq.offset & Si2157_ATV_LIF_FREQ_PROP_OFFSET_MASK) << Si2157_ATV_LIF_FREQ_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2157_ATV_LIF_FREQ_PROP */
    #ifdef        Si2157_ATV_LIF_OUT_PROP
     case         Si2157_ATV_LIF_OUT_PROP_CODE:
      *data = (prop->atv_lif_out.offset & Si2157_ATV_LIF_OUT_PROP_OFFSET_MASK) << Si2157_ATV_LIF_OUT_PROP_OFFSET_LSB  |
              (prop->atv_lif_out.amp    & Si2157_ATV_LIF_OUT_PROP_AMP_MASK   ) << Si2157_ATV_LIF_OUT_PROP_AMP_LSB ;
     break;
    #endif /*     Si2157_ATV_LIF_OUT_PROP */
    #ifdef        Si2157_ATV_PGA_TARGET_PROP
     case         Si2157_ATV_PGA_TARGET_PROP_CODE:
      *data = (prop->atv_pga_target.pga_target      & Si2157_ATV_PGA_TARGET_PROP_PGA_TARGET_MASK     ) << Si2157_ATV_PGA_TARGET_PROP_PGA_TARGET_LSB  |
              (prop->atv_pga_target.override_enable & Si2157_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK) << Si2157_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB ;
     break;
    #endif /*     Si2157_ATV_PGA_TARGET_PROP */
    #ifdef        Si2157_ATV_RF_TOP_PROP
     case         Si2157_ATV_RF_TOP_PROP_CODE:
      *data = (prop->atv_rf_top.atv_rf_top & Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_MASK) << Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_LSB ;
     break;
    #endif /*     Si2157_ATV_RF_TOP_PROP */
    #ifdef        Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      *data = (prop->atv_rsq_rssi_threshold.lo & Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK) << Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB  |
              (prop->atv_rsq_rssi_threshold.hi & Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK) << Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB ;
     break;
    #endif /*     Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_ATV_VIDEO_MODE_PROP
     case         Si2157_ATV_VIDEO_MODE_PROP_CODE:
      *data = (prop->atv_video_mode.video_sys       & Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_MASK      ) << Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LSB  |
              (prop->atv_video_mode.color           & Si2157_ATV_VIDEO_MODE_PROP_COLOR_MASK          ) << Si2157_ATV_VIDEO_MODE_PROP_COLOR_LSB  |
              (prop->atv_video_mode.invert_spectrum & Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_MASK) << Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_LSB ;
     break;
    #endif /*     Si2157_ATV_VIDEO_MODE_PROP */
    #ifdef        Si2157_ATV_VSNR_CAP_PROP
     case         Si2157_ATV_VSNR_CAP_PROP_CODE:
      *data = (prop->atv_vsnr_cap.atv_vsnr_cap & Si2157_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_MASK) << Si2157_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_LSB ;
     break;
    #endif /*     Si2157_ATV_VSNR_CAP_PROP */
    #ifdef        Si2157_CRYSTAL_TRIM_PROP
     case         Si2157_CRYSTAL_TRIM_PROP_CODE:
      *data = (prop->crystal_trim.xo_cap & Si2157_CRYSTAL_TRIM_PROP_XO_CAP_MASK) << Si2157_CRYSTAL_TRIM_PROP_XO_CAP_LSB ;
     break;
    #endif /*     Si2157_CRYSTAL_TRIM_PROP */
    #ifdef        Si2157_DTV_AGC_FREEZE_INPUT_PROP
     case         Si2157_DTV_AGC_FREEZE_INPUT_PROP_CODE:
      *data = (prop->dtv_agc_freeze_input.level & Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_MASK) << Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_LSB  |
              (prop->dtv_agc_freeze_input.pin   & Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_MASK  ) << Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_LSB ;
     break;
    #endif /*     Si2157_DTV_AGC_FREEZE_INPUT_PROP */
    #ifdef        Si2157_DTV_AGC_SPEED_PROP
     case         Si2157_DTV_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_agc_speed.if_agc_speed & Si2157_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK) << Si2157_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB  |
              (prop->dtv_agc_speed.agc_decim    & Si2157_DTV_AGC_SPEED_PROP_AGC_DECIM_MASK   ) << Si2157_DTV_AGC_SPEED_PROP_AGC_DECIM_LSB ;
     break;
    #endif /*     Si2157_DTV_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_CONFIG_IF_PORT_PROP
     case         Si2157_DTV_CONFIG_IF_PORT_PROP_CODE:
      *data = (prop->dtv_config_if_port.dtv_out_type   & Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_MASK  ) << Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LSB  |
              (prop->dtv_config_if_port.dtv_agc_source & Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_MASK) << Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_LSB ;
     break;
    #endif /*     Si2157_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_DTV_EXT_AGC_PROP
     case         Si2157_DTV_EXT_AGC_PROP_CODE:
      *data = (prop->dtv_ext_agc.min_10mv & Si2157_DTV_EXT_AGC_PROP_MIN_10MV_MASK) << Si2157_DTV_EXT_AGC_PROP_MIN_10MV_LSB  |
              (prop->dtv_ext_agc.max_10mv & Si2157_DTV_EXT_AGC_PROP_MAX_10MV_MASK) << Si2157_DTV_EXT_AGC_PROP_MAX_10MV_LSB ;
     break;
    #endif /*     Si2157_DTV_EXT_AGC_PROP */
    #ifdef        Si2157_DTV_IEN_PROP
     case         Si2157_DTV_IEN_PROP_CODE:
      *data = (prop->dtv_ien.chlien & Si2157_DTV_IEN_PROP_CHLIEN_MASK) << Si2157_DTV_IEN_PROP_CHLIEN_LSB ;
     break;
    #endif /*     Si2157_DTV_IEN_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PROP_CODE:
      *data = (prop->dtv_initial_agc_speed.if_agc_speed & Si2157_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_MASK) << Si2157_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_LSB  |
              (prop->dtv_initial_agc_speed.agc_decim    & Si2157_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_MASK   ) << Si2157_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_LSB ;
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
      *data = (prop->dtv_initial_agc_speed_period.period & Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_MASK) << Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_LSB ;
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2157_DTV_INTERNAL_ZIF_PROP
     case         Si2157_DTV_INTERNAL_ZIF_PROP_CODE:
      *data = (prop->dtv_internal_zif.atsc   & Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_MASK  ) << Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_LSB  |
              (prop->dtv_internal_zif.qam_us & Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_MASK) << Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_LSB  |
              (prop->dtv_internal_zif.dvbt   & Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_MASK  ) << Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_LSB  |
              (prop->dtv_internal_zif.dvbc   & Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_MASK  ) << Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_LSB  |
              (prop->dtv_internal_zif.isdbt  & Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_MASK ) << Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_LSB  |
              (prop->dtv_internal_zif.isdbc  & Si2157_DTV_INTERNAL_ZIF_PROP_ISDBC_MASK ) << Si2157_DTV_INTERNAL_ZIF_PROP_ISDBC_LSB  |
              (prop->dtv_internal_zif.dtmb   & Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_MASK  ) << Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_LSB ;
     break;
    #endif /*     Si2157_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2157_DTV_INT_SENSE_PROP
     case         Si2157_DTV_INT_SENSE_PROP_CODE:
      *data = (prop->dtv_int_sense.chlnegen & Si2157_DTV_INT_SENSE_PROP_CHLNEGEN_MASK) << Si2157_DTV_INT_SENSE_PROP_CHLNEGEN_LSB  |
              (prop->dtv_int_sense.chlposen & Si2157_DTV_INT_SENSE_PROP_CHLPOSEN_MASK) << Si2157_DTV_INT_SENSE_PROP_CHLPOSEN_LSB ;
     break;
    #endif /*     Si2157_DTV_INT_SENSE_PROP */
    #ifdef        Si2157_DTV_LIF_FREQ_PROP
     case         Si2157_DTV_LIF_FREQ_PROP_CODE:
      *data = (prop->dtv_lif_freq.offset & Si2157_DTV_LIF_FREQ_PROP_OFFSET_MASK) << Si2157_DTV_LIF_FREQ_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2157_DTV_LIF_FREQ_PROP */
    #ifdef        Si2157_DTV_LIF_OUT_PROP
     case         Si2157_DTV_LIF_OUT_PROP_CODE:
      *data = (prop->dtv_lif_out.offset & Si2157_DTV_LIF_OUT_PROP_OFFSET_MASK) << Si2157_DTV_LIF_OUT_PROP_OFFSET_LSB  |
              (prop->dtv_lif_out.amp    & Si2157_DTV_LIF_OUT_PROP_AMP_MASK   ) << Si2157_DTV_LIF_OUT_PROP_AMP_LSB ;
     break;
    #endif /*     Si2157_DTV_LIF_OUT_PROP */
    #ifdef        Si2157_DTV_MODE_PROP
     case         Si2157_DTV_MODE_PROP_CODE:
      *data = (prop->dtv_mode.bw              & Si2157_DTV_MODE_PROP_BW_MASK             ) << Si2157_DTV_MODE_PROP_BW_LSB  |
              (prop->dtv_mode.modulation      & Si2157_DTV_MODE_PROP_MODULATION_MASK     ) << Si2157_DTV_MODE_PROP_MODULATION_LSB  |
              (prop->dtv_mode.invert_spectrum & Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_MASK) << Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_LSB ;
     break;
    #endif /*     Si2157_DTV_MODE_PROP */
    #ifdef        Si2157_DTV_PGA_LIMITS_PROP
     case         Si2157_DTV_PGA_LIMITS_PROP_CODE:
      *data = (prop->dtv_pga_limits.min & Si2157_DTV_PGA_LIMITS_PROP_MIN_MASK) << Si2157_DTV_PGA_LIMITS_PROP_MIN_LSB  |
              (prop->dtv_pga_limits.max & Si2157_DTV_PGA_LIMITS_PROP_MAX_MASK) << Si2157_DTV_PGA_LIMITS_PROP_MAX_LSB ;
     break;
    #endif /*     Si2157_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2157_DTV_PGA_TARGET_PROP
     case         Si2157_DTV_PGA_TARGET_PROP_CODE:
      *data = (prop->dtv_pga_target.pga_target      & Si2157_DTV_PGA_TARGET_PROP_PGA_TARGET_MASK     ) << Si2157_DTV_PGA_TARGET_PROP_PGA_TARGET_LSB  |
              (prop->dtv_pga_target.override_enable & Si2157_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK) << Si2157_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB ;
     break;
    #endif /*     Si2157_DTV_PGA_TARGET_PROP */
    #ifdef        Si2157_DTV_RF_TOP_PROP
     case         Si2157_DTV_RF_TOP_PROP_CODE:
      *data = (prop->dtv_rf_top.dtv_rf_top & Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_MASK) << Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_LSB ;
     break;
    #endif /*     Si2157_DTV_RF_TOP_PROP */
    #ifdef        Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      *data = (prop->dtv_rsq_rssi_threshold.lo & Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK) << Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB  |
              (prop->dtv_rsq_rssi_threshold.hi & Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK) << Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB ;
     break;
    #endif /*     Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP
     case         Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_CODE:
      *data = (prop->dtv_zif_dc_canceller_bw.bandwidth & Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_MASK) << Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_LSB ;
     break;
    #endif /*     Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP */
    #ifdef        Si2157_MASTER_IEN_PROP
     case         Si2157_MASTER_IEN_PROP_CODE:
      *data = (prop->master_ien.tunien & Si2157_MASTER_IEN_PROP_TUNIEN_MASK) << Si2157_MASTER_IEN_PROP_TUNIEN_LSB  |
              (prop->master_ien.atvien & Si2157_MASTER_IEN_PROP_ATVIEN_MASK) << Si2157_MASTER_IEN_PROP_ATVIEN_LSB  |
              (prop->master_ien.dtvien & Si2157_MASTER_IEN_PROP_DTVIEN_MASK) << Si2157_MASTER_IEN_PROP_DTVIEN_LSB  |
              (prop->master_ien.errien & Si2157_MASTER_IEN_PROP_ERRIEN_MASK) << Si2157_MASTER_IEN_PROP_ERRIEN_LSB  |
              (prop->master_ien.ctsien & Si2157_MASTER_IEN_PROP_CTSIEN_MASK) << Si2157_MASTER_IEN_PROP_CTSIEN_LSB ;
     break;
    #endif /*     Si2157_MASTER_IEN_PROP */
    #ifdef        Si2157_TUNER_BLOCKED_VCO_PROP
     case         Si2157_TUNER_BLOCKED_VCO_PROP_CODE:
      *data = (prop->tuner_blocked_vco.vco_code & Si2157_TUNER_BLOCKED_VCO_PROP_VCO_CODE_MASK) << Si2157_TUNER_BLOCKED_VCO_PROP_VCO_CODE_LSB ;
     break;
    #endif /*     Si2157_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2157_TUNER_IEN_PROP
     case         Si2157_TUNER_IEN_PROP_CODE:
      *data = (prop->tuner_ien.tcien    & Si2157_TUNER_IEN_PROP_TCIEN_MASK   ) << Si2157_TUNER_IEN_PROP_TCIEN_LSB  |
              (prop->tuner_ien.rssilien & Si2157_TUNER_IEN_PROP_RSSILIEN_MASK) << Si2157_TUNER_IEN_PROP_RSSILIEN_LSB  |
              (prop->tuner_ien.rssihien & Si2157_TUNER_IEN_PROP_RSSIHIEN_MASK) << Si2157_TUNER_IEN_PROP_RSSIHIEN_LSB ;
     break;
    #endif /*     Si2157_TUNER_IEN_PROP */
    #ifdef        Si2157_TUNER_INT_SENSE_PROP
     case         Si2157_TUNER_INT_SENSE_PROP_CODE:
      *data = (prop->tuner_int_sense.tcnegen    & Si2157_TUNER_INT_SENSE_PROP_TCNEGEN_MASK   ) << Si2157_TUNER_INT_SENSE_PROP_TCNEGEN_LSB  |
              (prop->tuner_int_sense.rssilnegen & Si2157_TUNER_INT_SENSE_PROP_RSSILNEGEN_MASK) << Si2157_TUNER_INT_SENSE_PROP_RSSILNEGEN_LSB  |
              (prop->tuner_int_sense.rssihnegen & Si2157_TUNER_INT_SENSE_PROP_RSSIHNEGEN_MASK) << Si2157_TUNER_INT_SENSE_PROP_RSSIHNEGEN_LSB  |
              (prop->tuner_int_sense.tcposen    & Si2157_TUNER_INT_SENSE_PROP_TCPOSEN_MASK   ) << Si2157_TUNER_INT_SENSE_PROP_TCPOSEN_LSB  |
              (prop->tuner_int_sense.rssilposen & Si2157_TUNER_INT_SENSE_PROP_RSSILPOSEN_MASK) << Si2157_TUNER_INT_SENSE_PROP_RSSILPOSEN_LSB  |
              (prop->tuner_int_sense.rssihposen & Si2157_TUNER_INT_SENSE_PROP_RSSIHPOSEN_MASK) << Si2157_TUNER_INT_SENSE_PROP_RSSIHPOSEN_LSB ;
     break;
    #endif /*     Si2157_TUNER_INT_SENSE_PROP */
    #ifdef        Si2157_TUNER_LO_INJECTION_PROP
     case         Si2157_TUNER_LO_INJECTION_PROP_CODE:
      *data = (prop->tuner_lo_injection.band_1 & Si2157_TUNER_LO_INJECTION_PROP_BAND_1_MASK) << Si2157_TUNER_LO_INJECTION_PROP_BAND_1_LSB  |
              (prop->tuner_lo_injection.band_2 & Si2157_TUNER_LO_INJECTION_PROP_BAND_2_MASK) << Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LSB  |
              (prop->tuner_lo_injection.band_3 & Si2157_TUNER_LO_INJECTION_PROP_BAND_3_MASK) << Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LSB ;
     break;
    #endif /*     Si2157_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2157_TUNER_RETURN_LOSS_PROP
     case         Si2157_TUNER_RETURN_LOSS_PROP_CODE:
      *data = (prop->tuner_return_loss.config & Si2157_TUNER_RETURN_LOSS_PROP_CONFIG_MASK) << Si2157_TUNER_RETURN_LOSS_PROP_CONFIG_LSB  |
              (prop->tuner_return_loss.mode   & Si2157_TUNER_RETURN_LOSS_PROP_MODE_MASK  ) << Si2157_TUNER_RETURN_LOSS_PROP_MODE_LSB ;
     break;
    #endif /*     Si2157_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2157_WIDE_BAND_ATT_THRS_PROP
     case         Si2157_WIDE_BAND_ATT_THRS_PROP_CODE:
      *data = (prop->wide_band_att_thrs.wb_att_thrs & Si2157_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS_MASK) << Si2157_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS_LSB ;
     break;
    #endif /*     Si2157_WIDE_BAND_ATT_THRS_PROP */
    #ifdef        Si2157_XOUT_PROP
     case         Si2157_XOUT_PROP_CODE:
      *data = (prop->xout.amp & Si2157_XOUT_PROP_AMP_MASK) << Si2157_XOUT_PROP_AMP_LSB ;
     break;
    #endif /*     Si2157_XOUT_PROP */
     default : return ERROR_Si2157_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2157_ERROR;
  }


/***********************************************************************************************************************
  Si2157_UnpackProperty function
  Use:        This function will unpack all the members of a property from an integer from the GetProperty function.

  Parameter: *prop          the Si2157 property context
  Parameter:  prop_code     the property Id
  Parameter:  data          the property data

  Returns:    NO_Si2157_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2157_UnpackProperty          (Si2157_PropObj   *prop, unsigned int prop_code, int  data) {
    switch (prop_code) {
    #ifdef        Si2157_ATV_AFC_RANGE_PROP
     case         Si2157_ATV_AFC_RANGE_PROP_CODE:
               prop->atv_afc_range.range_khz = (data >> Si2157_ATV_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2157_ATV_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2157_ATV_AFC_RANGE_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_PROP
     case         Si2157_ATV_AGC_SPEED_PROP_CODE:
               prop->atv_agc_speed.if_agc_speed = (data >> Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB) & Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK;
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP
     case         Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_CODE:
               prop->atv_agc_speed_low_rssi.if_agc_speed = (data >> Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED_LSB) & Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED_MASK;
               prop->atv_agc_speed_low_rssi.thld         = (data >> Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD_LSB        ) & Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD_MASK;
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP */
    #ifdef        Si2157_ATV_ARTIFICIAL_SNOW_PROP
     case         Si2157_ATV_ARTIFICIAL_SNOW_PROP_CODE:
               prop->atv_artificial_snow.gain   = (data >> Si2157_ATV_ARTIFICIAL_SNOW_PROP_GAIN_LSB  ) & Si2157_ATV_ARTIFICIAL_SNOW_PROP_GAIN_MASK;
               prop->atv_artificial_snow.period = (data >> Si2157_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_LSB) & Si2157_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_MASK;
               prop->atv_artificial_snow.offset = (data >> Si2157_ATV_ARTIFICIAL_SNOW_PROP_OFFSET_LSB) & Si2157_ATV_ARTIFICIAL_SNOW_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2157_ATV_ARTIFICIAL_SNOW_PROP */
    #ifdef        Si2157_ATV_CONFIG_IF_PORT_PROP
     case         Si2157_ATV_CONFIG_IF_PORT_PROP_CODE:
               prop->atv_config_if_port.atv_out_type   = (data >> Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LSB  ) & Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_MASK;
               prop->atv_config_if_port.atv_agc_source = (data >> Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_LSB) & Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_MASK;
     break;
    #endif /*     Si2157_ATV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_ATV_EXT_AGC_PROP
     case         Si2157_ATV_EXT_AGC_PROP_CODE:
               prop->atv_ext_agc.min_10mv = (data >> Si2157_ATV_EXT_AGC_PROP_MIN_10MV_LSB) & Si2157_ATV_EXT_AGC_PROP_MIN_10MV_MASK;
               prop->atv_ext_agc.max_10mv = (data >> Si2157_ATV_EXT_AGC_PROP_MAX_10MV_LSB) & Si2157_ATV_EXT_AGC_PROP_MAX_10MV_MASK;
     break;
    #endif /*     Si2157_ATV_EXT_AGC_PROP */
    #ifdef        Si2157_ATV_HSYNC_OUT_PROP
     case         Si2157_ATV_HSYNC_OUT_PROP_CODE:
               prop->atv_hsync_out.gpio_sel = (data >> Si2157_ATV_HSYNC_OUT_PROP_GPIO_SEL_LSB) & Si2157_ATV_HSYNC_OUT_PROP_GPIO_SEL_MASK;
               prop->atv_hsync_out.width    = (data >> Si2157_ATV_HSYNC_OUT_PROP_WIDTH_LSB   ) & Si2157_ATV_HSYNC_OUT_PROP_WIDTH_MASK;
               prop->atv_hsync_out.offset   = (data >> Si2157_ATV_HSYNC_OUT_PROP_OFFSET_LSB  ) & Si2157_ATV_HSYNC_OUT_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2157_ATV_HSYNC_OUT_PROP */
    #ifdef        Si2157_ATV_IEN_PROP
     case         Si2157_ATV_IEN_PROP_CODE:
               prop->atv_ien.chlien = (data >> Si2157_ATV_IEN_PROP_CHLIEN_LSB) & Si2157_ATV_IEN_PROP_CHLIEN_MASK;
               prop->atv_ien.pclien = (data >> Si2157_ATV_IEN_PROP_PCLIEN_LSB) & Si2157_ATV_IEN_PROP_PCLIEN_MASK;
     break;
    #endif /*     Si2157_ATV_IEN_PROP */
    #ifdef        Si2157_ATV_INT_SENSE_PROP
     case         Si2157_ATV_INT_SENSE_PROP_CODE:
               prop->atv_int_sense.chlnegen = (data >> Si2157_ATV_INT_SENSE_PROP_CHLNEGEN_LSB) & Si2157_ATV_INT_SENSE_PROP_CHLNEGEN_MASK;
               prop->atv_int_sense.pclnegen = (data >> Si2157_ATV_INT_SENSE_PROP_PCLNEGEN_LSB) & Si2157_ATV_INT_SENSE_PROP_PCLNEGEN_MASK;
               prop->atv_int_sense.chlposen = (data >> Si2157_ATV_INT_SENSE_PROP_CHLPOSEN_LSB) & Si2157_ATV_INT_SENSE_PROP_CHLPOSEN_MASK;
               prop->atv_int_sense.pclposen = (data >> Si2157_ATV_INT_SENSE_PROP_PCLPOSEN_LSB) & Si2157_ATV_INT_SENSE_PROP_PCLPOSEN_MASK;
     break;
    #endif /*     Si2157_ATV_INT_SENSE_PROP */
    #ifdef        Si2157_ATV_LIF_FREQ_PROP
     case         Si2157_ATV_LIF_FREQ_PROP_CODE:
               prop->atv_lif_freq.offset = (data >> Si2157_ATV_LIF_FREQ_PROP_OFFSET_LSB) & Si2157_ATV_LIF_FREQ_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2157_ATV_LIF_FREQ_PROP */
    #ifdef        Si2157_ATV_LIF_OUT_PROP
     case         Si2157_ATV_LIF_OUT_PROP_CODE:
               prop->atv_lif_out.offset = (data >> Si2157_ATV_LIF_OUT_PROP_OFFSET_LSB) & Si2157_ATV_LIF_OUT_PROP_OFFSET_MASK;
               prop->atv_lif_out.amp    = (data >> Si2157_ATV_LIF_OUT_PROP_AMP_LSB   ) & Si2157_ATV_LIF_OUT_PROP_AMP_MASK;
     break;
    #endif /*     Si2157_ATV_LIF_OUT_PROP */
    #ifdef        Si2157_ATV_PGA_TARGET_PROP
     case         Si2157_ATV_PGA_TARGET_PROP_CODE:
               prop->atv_pga_target.pga_target      = (data >> Si2157_ATV_PGA_TARGET_PROP_PGA_TARGET_LSB     ) & Si2157_ATV_PGA_TARGET_PROP_PGA_TARGET_MASK;
               prop->atv_pga_target.override_enable = (data >> Si2157_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB) & Si2157_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK;
     break;
    #endif /*     Si2157_ATV_PGA_TARGET_PROP */
    #ifdef        Si2157_ATV_RF_TOP_PROP
     case         Si2157_ATV_RF_TOP_PROP_CODE:
               prop->atv_rf_top.atv_rf_top = (data >> Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_LSB) & Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_MASK;
     break;
    #endif /*     Si2157_ATV_RF_TOP_PROP */
    #ifdef        Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE:
               prop->atv_rsq_rssi_threshold.lo = (data >> Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB) & Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK;
               prop->atv_rsq_rssi_threshold.hi = (data >> Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB) & Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK;
     break;
    #endif /*     Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_ATV_VIDEO_MODE_PROP
     case         Si2157_ATV_VIDEO_MODE_PROP_CODE:
               prop->atv_video_mode.video_sys       = (data >> Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LSB      ) & Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_MASK;
               prop->atv_video_mode.color           = (data >> Si2157_ATV_VIDEO_MODE_PROP_COLOR_LSB          ) & Si2157_ATV_VIDEO_MODE_PROP_COLOR_MASK;
               prop->atv_video_mode.invert_spectrum = (data >> Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_LSB) & Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_MASK;
     break;
    #endif /*     Si2157_ATV_VIDEO_MODE_PROP */
    #ifdef        Si2157_ATV_VSNR_CAP_PROP
     case         Si2157_ATV_VSNR_CAP_PROP_CODE:
               prop->atv_vsnr_cap.atv_vsnr_cap = (data >> Si2157_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_LSB) & Si2157_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_MASK;
     break;
    #endif /*     Si2157_ATV_VSNR_CAP_PROP */
    #ifdef        Si2157_CRYSTAL_TRIM_PROP
     case         Si2157_CRYSTAL_TRIM_PROP_CODE:
               prop->crystal_trim.xo_cap = (data >> Si2157_CRYSTAL_TRIM_PROP_XO_CAP_LSB) & Si2157_CRYSTAL_TRIM_PROP_XO_CAP_MASK;
     break;
    #endif /*     Si2157_CRYSTAL_TRIM_PROP */
    #ifdef        Si2157_DTV_AGC_FREEZE_INPUT_PROP
     case         Si2157_DTV_AGC_FREEZE_INPUT_PROP_CODE:
               prop->dtv_agc_freeze_input.level = (data >> Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_LSB) & Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_MASK;
               prop->dtv_agc_freeze_input.pin   = (data >> Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_LSB  ) & Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_MASK;
     break;
    #endif /*     Si2157_DTV_AGC_FREEZE_INPUT_PROP */
    #ifdef        Si2157_DTV_AGC_SPEED_PROP
     case         Si2157_DTV_AGC_SPEED_PROP_CODE:
               prop->dtv_agc_speed.if_agc_speed = (data >> Si2157_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB) & Si2157_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK;
               prop->dtv_agc_speed.agc_decim    = (data >> Si2157_DTV_AGC_SPEED_PROP_AGC_DECIM_LSB   ) & Si2157_DTV_AGC_SPEED_PROP_AGC_DECIM_MASK;
     break;
    #endif /*     Si2157_DTV_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_CONFIG_IF_PORT_PROP
     case         Si2157_DTV_CONFIG_IF_PORT_PROP_CODE:
               prop->dtv_config_if_port.dtv_out_type   = (data >> Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LSB  ) & Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_MASK;
               prop->dtv_config_if_port.dtv_agc_source = (data >> Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_LSB) & Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_MASK;
     break;
    #endif /*     Si2157_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_DTV_EXT_AGC_PROP
     case         Si2157_DTV_EXT_AGC_PROP_CODE:
               prop->dtv_ext_agc.min_10mv = (data >> Si2157_DTV_EXT_AGC_PROP_MIN_10MV_LSB) & Si2157_DTV_EXT_AGC_PROP_MIN_10MV_MASK;
               prop->dtv_ext_agc.max_10mv = (data >> Si2157_DTV_EXT_AGC_PROP_MAX_10MV_LSB) & Si2157_DTV_EXT_AGC_PROP_MAX_10MV_MASK;
     break;
    #endif /*     Si2157_DTV_EXT_AGC_PROP */
    #ifdef        Si2157_DTV_IEN_PROP
     case         Si2157_DTV_IEN_PROP_CODE:
               prop->dtv_ien.chlien = (data >> Si2157_DTV_IEN_PROP_CHLIEN_LSB) & Si2157_DTV_IEN_PROP_CHLIEN_MASK;
     break;
    #endif /*     Si2157_DTV_IEN_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PROP_CODE:
               prop->dtv_initial_agc_speed.if_agc_speed = (data >> Si2157_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_LSB) & Si2157_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_MASK;
               prop->dtv_initial_agc_speed.agc_decim    = (data >> Si2157_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_LSB   ) & Si2157_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_MASK;
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
               prop->dtv_initial_agc_speed_period.period = (data >> Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_LSB) & Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_MASK;
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2157_DTV_INTERNAL_ZIF_PROP
     case         Si2157_DTV_INTERNAL_ZIF_PROP_CODE:
               prop->dtv_internal_zif.atsc   = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_LSB  ) & Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_MASK;
               prop->dtv_internal_zif.qam_us = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_LSB) & Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_MASK;
               prop->dtv_internal_zif.dvbt   = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_LSB  ) & Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_MASK;
               prop->dtv_internal_zif.dvbc   = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_LSB  ) & Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_MASK;
               prop->dtv_internal_zif.isdbt  = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_LSB ) & Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_MASK;
               prop->dtv_internal_zif.isdbc  = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_ISDBC_LSB ) & Si2157_DTV_INTERNAL_ZIF_PROP_ISDBC_MASK;
               prop->dtv_internal_zif.dtmb   = (data >> Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_LSB  ) & Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_MASK;
     break;
    #endif /*     Si2157_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2157_DTV_INT_SENSE_PROP
     case         Si2157_DTV_INT_SENSE_PROP_CODE:
               prop->dtv_int_sense.chlnegen = (data >> Si2157_DTV_INT_SENSE_PROP_CHLNEGEN_LSB) & Si2157_DTV_INT_SENSE_PROP_CHLNEGEN_MASK;
               prop->dtv_int_sense.chlposen = (data >> Si2157_DTV_INT_SENSE_PROP_CHLPOSEN_LSB) & Si2157_DTV_INT_SENSE_PROP_CHLPOSEN_MASK;
     break;
    #endif /*     Si2157_DTV_INT_SENSE_PROP */
    #ifdef        Si2157_DTV_LIF_FREQ_PROP
     case         Si2157_DTV_LIF_FREQ_PROP_CODE:
               prop->dtv_lif_freq.offset = (data >> Si2157_DTV_LIF_FREQ_PROP_OFFSET_LSB) & Si2157_DTV_LIF_FREQ_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2157_DTV_LIF_FREQ_PROP */
    #ifdef        Si2157_DTV_LIF_OUT_PROP
     case         Si2157_DTV_LIF_OUT_PROP_CODE:
               prop->dtv_lif_out.offset = (data >> Si2157_DTV_LIF_OUT_PROP_OFFSET_LSB) & Si2157_DTV_LIF_OUT_PROP_OFFSET_MASK;
               prop->dtv_lif_out.amp    = (data >> Si2157_DTV_LIF_OUT_PROP_AMP_LSB   ) & Si2157_DTV_LIF_OUT_PROP_AMP_MASK;
     break;
    #endif /*     Si2157_DTV_LIF_OUT_PROP */
    #ifdef        Si2157_DTV_MODE_PROP
     case         Si2157_DTV_MODE_PROP_CODE:
               prop->dtv_mode.bw              = (data >> Si2157_DTV_MODE_PROP_BW_LSB             ) & Si2157_DTV_MODE_PROP_BW_MASK;
               prop->dtv_mode.modulation      = (data >> Si2157_DTV_MODE_PROP_MODULATION_LSB     ) & Si2157_DTV_MODE_PROP_MODULATION_MASK;
               prop->dtv_mode.invert_spectrum = (data >> Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_LSB) & Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_MASK;
     break;
    #endif /*     Si2157_DTV_MODE_PROP */
    #ifdef        Si2157_DTV_PGA_LIMITS_PROP
     case         Si2157_DTV_PGA_LIMITS_PROP_CODE:
               prop->dtv_pga_limits.min = (data >> Si2157_DTV_PGA_LIMITS_PROP_MIN_LSB) & Si2157_DTV_PGA_LIMITS_PROP_MIN_MASK;
               prop->dtv_pga_limits.max = (data >> Si2157_DTV_PGA_LIMITS_PROP_MAX_LSB) & Si2157_DTV_PGA_LIMITS_PROP_MAX_MASK;
     break;
    #endif /*     Si2157_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2157_DTV_PGA_TARGET_PROP
     case         Si2157_DTV_PGA_TARGET_PROP_CODE:
               prop->dtv_pga_target.pga_target      = (data >> Si2157_DTV_PGA_TARGET_PROP_PGA_TARGET_LSB     ) & Si2157_DTV_PGA_TARGET_PROP_PGA_TARGET_MASK;
               prop->dtv_pga_target.override_enable = (data >> Si2157_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_LSB) & Si2157_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_MASK;
     break;
    #endif /*     Si2157_DTV_PGA_TARGET_PROP */
    #ifdef        Si2157_DTV_RF_TOP_PROP
     case         Si2157_DTV_RF_TOP_PROP_CODE:
               prop->dtv_rf_top.dtv_rf_top = (data >> Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_LSB) & Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_MASK;
     break;
    #endif /*     Si2157_DTV_RF_TOP_PROP */
    #ifdef        Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
               prop->dtv_rsq_rssi_threshold.lo = (data >> Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB) & Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK;
               prop->dtv_rsq_rssi_threshold.hi = (data >> Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB) & Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK;
     break;
    #endif /*     Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP
     case         Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_CODE:
               prop->dtv_zif_dc_canceller_bw.bandwidth = (data >> Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_LSB) & Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_MASK;
     break;
    #endif /*     Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP */
    #ifdef        Si2157_MASTER_IEN_PROP
     case         Si2157_MASTER_IEN_PROP_CODE:
               prop->master_ien.tunien = (data >> Si2157_MASTER_IEN_PROP_TUNIEN_LSB) & Si2157_MASTER_IEN_PROP_TUNIEN_MASK;
               prop->master_ien.atvien = (data >> Si2157_MASTER_IEN_PROP_ATVIEN_LSB) & Si2157_MASTER_IEN_PROP_ATVIEN_MASK;
               prop->master_ien.dtvien = (data >> Si2157_MASTER_IEN_PROP_DTVIEN_LSB) & Si2157_MASTER_IEN_PROP_DTVIEN_MASK;
               prop->master_ien.errien = (data >> Si2157_MASTER_IEN_PROP_ERRIEN_LSB) & Si2157_MASTER_IEN_PROP_ERRIEN_MASK;
               prop->master_ien.ctsien = (data >> Si2157_MASTER_IEN_PROP_CTSIEN_LSB) & Si2157_MASTER_IEN_PROP_CTSIEN_MASK;
     break;
    #endif /*     Si2157_MASTER_IEN_PROP */
    #ifdef        Si2157_TUNER_BLOCKED_VCO_PROP
     case         Si2157_TUNER_BLOCKED_VCO_PROP_CODE:
               prop->tuner_blocked_vco.vco_code = (data >> Si2157_TUNER_BLOCKED_VCO_PROP_VCO_CODE_LSB) & Si2157_TUNER_BLOCKED_VCO_PROP_VCO_CODE_MASK;
     break;
    #endif /*     Si2157_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2157_TUNER_IEN_PROP
     case         Si2157_TUNER_IEN_PROP_CODE:
               prop->tuner_ien.tcien    = (data >> Si2157_TUNER_IEN_PROP_TCIEN_LSB   ) & Si2157_TUNER_IEN_PROP_TCIEN_MASK;
               prop->tuner_ien.rssilien = (data >> Si2157_TUNER_IEN_PROP_RSSILIEN_LSB) & Si2157_TUNER_IEN_PROP_RSSILIEN_MASK;
               prop->tuner_ien.rssihien = (data >> Si2157_TUNER_IEN_PROP_RSSIHIEN_LSB) & Si2157_TUNER_IEN_PROP_RSSIHIEN_MASK;
     break;
    #endif /*     Si2157_TUNER_IEN_PROP */
    #ifdef        Si2157_TUNER_INT_SENSE_PROP
     case         Si2157_TUNER_INT_SENSE_PROP_CODE:
               prop->tuner_int_sense.tcnegen    = (data >> Si2157_TUNER_INT_SENSE_PROP_TCNEGEN_LSB   ) & Si2157_TUNER_INT_SENSE_PROP_TCNEGEN_MASK;
               prop->tuner_int_sense.rssilnegen = (data >> Si2157_TUNER_INT_SENSE_PROP_RSSILNEGEN_LSB) & Si2157_TUNER_INT_SENSE_PROP_RSSILNEGEN_MASK;
               prop->tuner_int_sense.rssihnegen = (data >> Si2157_TUNER_INT_SENSE_PROP_RSSIHNEGEN_LSB) & Si2157_TUNER_INT_SENSE_PROP_RSSIHNEGEN_MASK;
               prop->tuner_int_sense.tcposen    = (data >> Si2157_TUNER_INT_SENSE_PROP_TCPOSEN_LSB   ) & Si2157_TUNER_INT_SENSE_PROP_TCPOSEN_MASK;
               prop->tuner_int_sense.rssilposen = (data >> Si2157_TUNER_INT_SENSE_PROP_RSSILPOSEN_LSB) & Si2157_TUNER_INT_SENSE_PROP_RSSILPOSEN_MASK;
               prop->tuner_int_sense.rssihposen = (data >> Si2157_TUNER_INT_SENSE_PROP_RSSIHPOSEN_LSB) & Si2157_TUNER_INT_SENSE_PROP_RSSIHPOSEN_MASK;
     break;
    #endif /*     Si2157_TUNER_INT_SENSE_PROP */
    #ifdef        Si2157_TUNER_LO_INJECTION_PROP
     case         Si2157_TUNER_LO_INJECTION_PROP_CODE:
               prop->tuner_lo_injection.band_1 = (data >> Si2157_TUNER_LO_INJECTION_PROP_BAND_1_LSB) & Si2157_TUNER_LO_INJECTION_PROP_BAND_1_MASK;
               prop->tuner_lo_injection.band_2 = (data >> Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LSB) & Si2157_TUNER_LO_INJECTION_PROP_BAND_2_MASK;
               prop->tuner_lo_injection.band_3 = (data >> Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LSB) & Si2157_TUNER_LO_INJECTION_PROP_BAND_3_MASK;
     break;
    #endif /*     Si2157_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2157_TUNER_RETURN_LOSS_PROP
     case         Si2157_TUNER_RETURN_LOSS_PROP_CODE:
               prop->tuner_return_loss.config = (data >> Si2157_TUNER_RETURN_LOSS_PROP_CONFIG_LSB) & Si2157_TUNER_RETURN_LOSS_PROP_CONFIG_MASK;
               prop->tuner_return_loss.mode   = (data >> Si2157_TUNER_RETURN_LOSS_PROP_MODE_LSB  ) & Si2157_TUNER_RETURN_LOSS_PROP_MODE_MASK;
     break;
    #endif /*     Si2157_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2157_WIDE_BAND_ATT_THRS_PROP
     case         Si2157_WIDE_BAND_ATT_THRS_PROP_CODE:
               prop->wide_band_att_thrs.wb_att_thrs = (data >> Si2157_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS_LSB) & Si2157_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS_MASK;
     break;
    #endif /*     Si2157_WIDE_BAND_ATT_THRS_PROP */
    #ifdef        Si2157_XOUT_PROP
     case         Si2157_XOUT_PROP_CODE:
               prop->xout.amp = (data >> Si2157_XOUT_PROP_AMP_LSB) & Si2157_XOUT_PROP_AMP_MASK;
     break;
    #endif /*     Si2157_XOUT_PROP */
     default : return ERROR_Si2157_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2157_ERROR;
  }
/***********************************************************************************************************************
  Si2157_storePropertiesDefaults function
  Use:        property defaults function
              Used to fill the propShadow structure with startup values.
  Parameter: *prop     the Si2157_PropObject structure

 |---------------------------------------------------------------------------------------------------------------------|
 | Do NOT change this code unless you really know what you're doing!                                                   |
 | It should reflect the part internal property settings after firmware download                                       |
 |---------------------------------------------------------------------------------------------------------------------|

 Returns:    void
 ***********************************************************************************************************************/
void          Si2157_storePropertiesDefaults(Si2157_PropObj   *prop) {
  SiTRACE("Si2157_storePropertiesDefaults\n");
#ifdef    Si2157_ATV_AFC_RANGE_PROP
  prop->atv_afc_range.range_khz              =  1000; /* (default  1000) */
#endif /* Si2157_ATV_AFC_RANGE_PROP */

#ifdef    Si2157_ATV_AGC_SPEED_PROP
  prop->atv_agc_speed.if_agc_speed           = Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
#endif /* Si2157_ATV_AGC_SPEED_PROP */

#ifdef    Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP
  prop->atv_agc_speed_low_rssi.if_agc_speed  = Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED_158 ; /* (default '158') */
  prop->atv_agc_speed_low_rssi.thld          =  -128; /* (default  -128) */
#endif /* Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP */

#ifdef    Si2157_ATV_ARTIFICIAL_SNOW_PROP
  prop->atv_artificial_snow.gain             = Si2157_ATV_ARTIFICIAL_SNOW_PROP_GAIN_AUTO   ; /* (default 'AUTO') */
  prop->atv_artificial_snow.period           = Si2157_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_LONG ; /* (default 'LONG') */
  prop->atv_artificial_snow.offset           =     0; /* (default     0) */
#endif /* Si2157_ATV_ARTIFICIAL_SNOW_PROP */

#ifdef    Si2157_ATV_CONFIG_IF_PORT_PROP
  prop->atv_config_if_port.atv_out_type      = Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_DIFF_IF1   ; /* (default 'LIF_DIFF_IF1') */
  prop->atv_config_if_port.atv_agc_source    = Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_INTERNAL     ; /* (default 'INTERNAL') */
#endif /* Si2157_ATV_CONFIG_IF_PORT_PROP */

#ifdef    Si2157_ATV_EXT_AGC_PROP
  prop->atv_ext_agc.min_10mv                 =    50; /* (default    50) */
  prop->atv_ext_agc.max_10mv                 =   200; /* (default   200) */
#endif /* Si2157_ATV_EXT_AGC_PROP */

#ifdef    Si2157_ATV_HSYNC_OUT_PROP
  prop->atv_hsync_out.gpio_sel               = Si2157_ATV_HSYNC_OUT_PROP_GPIO_SEL_NONE ; /* (default 'NONE') */
  prop->atv_hsync_out.width                  =    42; /* (default    42) */
  prop->atv_hsync_out.offset                 =     0; /* (default     0) */
#endif /* Si2157_ATV_HSYNC_OUT_PROP */

#ifdef    Si2157_ATV_IEN_PROP
  prop->atv_ien.chlien                       = Si2157_ATV_IEN_PROP_CHLIEN_ENABLE  ; /* (default 'ENABLE') */
  prop->atv_ien.pclien                       = Si2157_ATV_IEN_PROP_PCLIEN_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2157_ATV_IEN_PROP */

#ifdef    Si2157_ATV_INT_SENSE_PROP
  prop->atv_int_sense.chlnegen               = Si2157_ATV_INT_SENSE_PROP_CHLNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->atv_int_sense.pclnegen               = Si2157_ATV_INT_SENSE_PROP_PCLNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->atv_int_sense.chlposen               = Si2157_ATV_INT_SENSE_PROP_CHLPOSEN_ENABLE  ; /* (default 'ENABLE') */
  prop->atv_int_sense.pclposen               = Si2157_ATV_INT_SENSE_PROP_PCLPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2157_ATV_INT_SENSE_PROP */

#ifdef    Si2157_ATV_LIF_FREQ_PROP
  prop->atv_lif_freq.offset                  =  5000; /* (default  5000) */
#endif /* Si2157_ATV_LIF_FREQ_PROP */

#ifdef    Si2157_ATV_LIF_OUT_PROP
  prop->atv_lif_out.offset                   =   148; /* (default   148) */
  prop->atv_lif_out.amp                      =   100; /* (default   100) */
#endif /* Si2157_ATV_LIF_OUT_PROP */

#ifdef    Si2157_ATV_PGA_TARGET_PROP
  prop->atv_pga_target.pga_target            =     0; /* (default     0) */
  prop->atv_pga_target.override_enable       = Si2157_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2157_ATV_PGA_TARGET_PROP */

#ifdef    Si2157_ATV_RF_TOP_PROP
  prop->atv_rf_top.atv_rf_top                = Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_AUTO ; /* (default 'AUTO') */
#endif /* Si2157_ATV_RF_TOP_PROP */

#ifdef    Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP
  prop->atv_rsq_rssi_threshold.lo            =   -70; /* (default   -70) */
  prop->atv_rsq_rssi_threshold.hi            =     0; /* (default     0) */
#endif /* Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP */

#ifdef    Si2157_ATV_VIDEO_MODE_PROP
  prop->atv_video_mode.video_sys             = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B              ; /* (default 'B') */
  prop->atv_video_mode.color                 = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC           ; /* (default 'PAL_NTSC') */
  prop->atv_video_mode.invert_spectrum       = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED ; /* (default 'INVERTED') */
#endif /* Si2157_ATV_VIDEO_MODE_PROP */

#ifdef    Si2157_ATV_VSNR_CAP_PROP
  prop->atv_vsnr_cap.atv_vsnr_cap            =     0; /* (default     0) */
#endif /* Si2157_ATV_VSNR_CAP_PROP */

#ifdef    Si2157_CRYSTAL_TRIM_PROP
  prop->crystal_trim.xo_cap                  = Si2157_CRYSTAL_TRIM_PROP_XO_CAP_6P7PF ; /* (default '6p7pF') */
#endif /* Si2157_CRYSTAL_TRIM_PROP */

#ifdef    Si2157_MASTER_IEN_PROP
  prop->master_ien.tunien                    = Si2157_MASTER_IEN_PROP_TUNIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.atvien                    = Si2157_MASTER_IEN_PROP_ATVIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.dtvien                    = Si2157_MASTER_IEN_PROP_DTVIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.errien                    = Si2157_MASTER_IEN_PROP_ERRIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.ctsien                    = Si2157_MASTER_IEN_PROP_CTSIEN_OFF ; /* (default 'OFF') */
#endif /* Si2157_MASTER_IEN_PROP */

#ifdef    Si2157_XOUT_PROP
  prop->xout.amp                             = Si2157_XOUT_PROP_AMP_HIGH ; /* (default 'HIGH') */
#endif /* Si2157_XOUT_PROP */

#ifdef    Si2157_DTV_AGC_FREEZE_INPUT_PROP
  prop->dtv_agc_freeze_input.level           = Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_LOW  ; /* (default 'LOW') */
  prop->dtv_agc_freeze_input.pin             = Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_NONE   ; /* (default 'NONE') */
#endif /* Si2157_DTV_AGC_FREEZE_INPUT_PROP */

#ifdef    Si2157_DTV_AGC_SPEED_PROP
  prop->dtv_agc_speed.if_agc_speed           = Si2157_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_agc_speed.agc_decim              = Si2157_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2157_DTV_AGC_SPEED_PROP */

#ifdef    Si2157_DTV_CONFIG_IF_PORT_PROP
  prop->dtv_config_if_port.dtv_out_type      = Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF2    ; /* (default 'LIF_IF2') */
  prop->dtv_config_if_port.dtv_agc_source    = Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_INTERNAL ; /* (default 'INTERNAL') */
#endif /* Si2157_DTV_CONFIG_IF_PORT_PROP */

#ifdef    Si2157_DTV_EXT_AGC_PROP
  prop->dtv_ext_agc.min_10mv                 =    50; /* (default    50) */
  prop->dtv_ext_agc.max_10mv                 =   200; /* (default   200) */
#endif /* Si2157_DTV_EXT_AGC_PROP */

#ifdef    Si2157_DTV_IEN_PROP
  prop->dtv_ien.chlien                       = Si2157_DTV_IEN_PROP_CHLIEN_ENABLE ; /* (default 'ENABLE') */
#endif /* Si2157_DTV_IEN_PROP */

#ifdef    Si2157_DTV_INITIAL_AGC_SPEED_PROP
  prop->dtv_initial_agc_speed.if_agc_speed   = Si2157_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_initial_agc_speed.agc_decim      = Si2157_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2157_DTV_INITIAL_AGC_SPEED_PROP */

#ifdef    Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  prop->dtv_initial_agc_speed_period.period  =     0; /* (default     0) */
#endif /* Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */

#ifdef    Si2157_DTV_INTERNAL_ZIF_PROP
  prop->dtv_internal_zif.atsc                = Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.qam_us              = Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_ZIF ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbt                = Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbc                = Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbt               = Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbc               = Si2157_DTV_INTERNAL_ZIF_PROP_ISDBC_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dtmb                = Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_ZIF   ; /* (default 'ZIF') */
#endif /* Si2157_DTV_INTERNAL_ZIF_PROP */

#ifdef    Si2157_DTV_INT_SENSE_PROP
  prop->dtv_int_sense.chlnegen               = Si2157_DTV_INT_SENSE_PROP_CHLNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->dtv_int_sense.chlposen               = Si2157_DTV_INT_SENSE_PROP_CHLPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2157_DTV_INT_SENSE_PROP */

#ifdef    Si2157_DTV_LIF_FREQ_PROP
  prop->dtv_lif_freq.offset                  =  5000; /* (default  5000) */
#endif /* Si2157_DTV_LIF_FREQ_PROP */

#ifdef    Si2157_DTV_LIF_OUT_PROP
  prop->dtv_lif_out.offset                   =   148; /* (default   148) */
  prop->dtv_lif_out.amp                      =    27; /* (default    27) */
#endif /* Si2157_DTV_LIF_OUT_PROP */

#ifdef    Si2157_DTV_MODE_PROP
  prop->dtv_mode.bw                          = Si2157_DTV_MODE_PROP_BW_BW_8MHZ              ; /* (default 'BW_8MHZ') */
  prop->dtv_mode.modulation                  = Si2157_DTV_MODE_PROP_MODULATION_DVBT         ; /* (default 'DVBT') */
  prop->dtv_mode.invert_spectrum             = Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL  ; /* (default 'NORMAL') */
#endif /* Si2157_DTV_MODE_PROP */

#ifdef    Si2157_DTV_PGA_LIMITS_PROP
  prop->dtv_pga_limits.min                   =    -1; /* (default    -1) */
  prop->dtv_pga_limits.max                   =    -1; /* (default    -1) */
#endif /* Si2157_DTV_PGA_LIMITS_PROP */

#ifdef    Si2157_DTV_PGA_TARGET_PROP
  prop->dtv_pga_target.pga_target            =     0; /* (default     0) */
  prop->dtv_pga_target.override_enable       = Si2157_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2157_DTV_PGA_TARGET_PROP */

#ifdef    Si2157_DTV_RF_TOP_PROP
  prop->dtv_rf_top.dtv_rf_top                = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO ; /* (default 'AUTO') */
#endif /* Si2157_DTV_RF_TOP_PROP */

#ifdef    Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP
  prop->dtv_rsq_rssi_threshold.lo            =   -80; /* (default   -80) */
  prop->dtv_rsq_rssi_threshold.hi            =     0; /* (default     0) */
#endif /* Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP */

#ifdef    Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP
  prop->dtv_zif_dc_canceller_bw.bandwidth    = Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_DEFAULT ; /* (default 'DEFAULT') */
#endif /* Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP */

#ifdef    Si2157_TUNER_BLOCKED_VCO_PROP
  prop->tuner_blocked_vco.vco_code           = 0x8000; /* (default 0x8000) */
#endif /* Si2157_TUNER_BLOCKED_VCO_PROP */

#ifdef    Si2157_TUNER_IEN_PROP
  prop->tuner_ien.tcien                      = Si2157_TUNER_IEN_PROP_TCIEN_ENABLE     ; /* (default 'ENABLE') */
  prop->tuner_ien.rssilien                   = Si2157_TUNER_IEN_PROP_RSSILIEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_ien.rssihien                   = Si2157_TUNER_IEN_PROP_RSSIHIEN_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2157_TUNER_IEN_PROP */

#ifdef    Si2157_TUNER_INT_SENSE_PROP
  prop->tuner_int_sense.tcnegen              = Si2157_TUNER_INT_SENSE_PROP_TCNEGEN_DISABLE    ; /* (default 'DISABLE') */
  prop->tuner_int_sense.rssilnegen           = Si2157_TUNER_INT_SENSE_PROP_RSSILNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_int_sense.rssihnegen           = Si2157_TUNER_INT_SENSE_PROP_RSSIHNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_int_sense.tcposen              = Si2157_TUNER_INT_SENSE_PROP_TCPOSEN_ENABLE     ; /* (default 'ENABLE') */
  prop->tuner_int_sense.rssilposen           = Si2157_TUNER_INT_SENSE_PROP_RSSILPOSEN_ENABLE  ; /* (default 'ENABLE') */
  prop->tuner_int_sense.rssihposen           = Si2157_TUNER_INT_SENSE_PROP_RSSIHPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2157_TUNER_INT_SENSE_PROP */

#ifdef    Si2157_TUNER_LO_INJECTION_PROP
  prop->tuner_lo_injection.band_1            = Si2157_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE ; /* (default 'HIGH_SIDE') */
  prop->tuner_lo_injection.band_2            = Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ; /* (default 'LOW_SIDE') */
  prop->tuner_lo_injection.band_3            = Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ; /* (default 'LOW_SIDE') */
#endif /* Si2157_TUNER_LO_INJECTION_PROP */

#ifdef    Si2157_TUNER_RETURN_LOSS_PROP
  prop->tuner_return_loss.config             = Si2157_TUNER_RETURN_LOSS_PROP_CONFIG_127         ; /* (default '127') */
  prop->tuner_return_loss.mode               = Si2157_TUNER_RETURN_LOSS_PROP_MODE_TERRESTRIAL   ; /* (default 'TERRESTRIAL') */
#endif /* Si2157_TUNER_RETURN_LOSS_PROP */

#ifdef    Si2157_WIDE_BAND_ATT_THRS_PROP
  prop->wide_band_att_thrs.wb_att_thrs       = Si2157_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS_AUTO ; /* (default 'AUTO') */
#endif /*     Si2157_WIDE_BAND_ATT_THRS_PROP */

}
#ifdef    Si2157_GET_PROPERTY_STRING
/***********************************************************************************************************************
  Si2157_L1_PropertyText function
  Use:        property text function
              Used to turn the property data into clear text.
  Parameter: *prop     the Si2157 property structure (containing all properties)
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string (usually declared with a size of 1000 bytes)
  Returns:    NO_Si2157_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2157_L1_PropertyText          (Si2157_PropObj   *prop, unsigned int prop_code, const char *separator, char *msg) {
    switch (prop_code) {
    #ifdef        Si2157_ATV_AFC_RANGE_PROP
     case         Si2157_ATV_AFC_RANGE_PROP_CODE:
      sprintf(msg,"ATV_AFC_RANGE");
       strcat(msg,separator); strcat(msg,"-RANGE_KHZ "); sprintf(msg,"%s%d", msg, prop->atv_afc_range.range_khz);
     break;
    #endif /*     Si2157_ATV_AFC_RANGE_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_PROP
     case         Si2157_ATV_AGC_SPEED_PROP_CODE:
      sprintf(msg,"ATV_AGC_SPEED");
       strcat(msg,separator); strcat(msg,"-IF_AGC_SPEED ");
           if  (prop->atv_agc_speed.if_agc_speed ==     0) strcat(msg,"AUTO   ");
      else if  (prop->atv_agc_speed.if_agc_speed ==    89) strcat(msg,"89     ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   105) strcat(msg,"105    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   121) strcat(msg,"121    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   137) strcat(msg,"137    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   158) strcat(msg,"158    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   172) strcat(msg,"172    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   178) strcat(msg,"178    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   185) strcat(msg,"185    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   196) strcat(msg,"196    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   206) strcat(msg,"206    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   216) strcat(msg,"216    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   219) strcat(msg,"219    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   222) strcat(msg,"222    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   223) strcat(msg,"223    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   248) strcat(msg,"248    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   250) strcat(msg,"250    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==   251) strcat(msg,"251    ");
      else if  (prop->atv_agc_speed.if_agc_speed ==     1) strcat(msg,"CUSTOM ");
      else                                                sprintf(msg,"%s%d", msg, prop->atv_agc_speed.if_agc_speed);
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_PROP */
    #ifdef        Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP
     case         Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_CODE:
      sprintf(msg,"ATV_AGC_SPEED_LOW_RSSI");
       strcat(msg,separator); strcat(msg,"-IF_AGC_SPEED ");
           if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==    89) strcat(msg,"89     ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   105) strcat(msg,"105    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   121) strcat(msg,"121    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   137) strcat(msg,"137    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   158) strcat(msg,"158    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   172) strcat(msg,"172    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   178) strcat(msg,"178    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   185) strcat(msg,"185    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   196) strcat(msg,"196    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   206) strcat(msg,"206    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   216) strcat(msg,"216    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   219) strcat(msg,"219    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   222) strcat(msg,"222    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   223) strcat(msg,"223    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   248) strcat(msg,"248    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   250) strcat(msg,"250    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==   251) strcat(msg,"251    ");
      else if  (prop->atv_agc_speed_low_rssi.if_agc_speed ==     1) strcat(msg,"CUSTOM ");
      else                                                         sprintf(msg,"%s%d", msg, prop->atv_agc_speed_low_rssi.if_agc_speed);
       strcat(msg,separator); strcat(msg,"-THLD "); sprintf(msg,"%s%d", msg, prop->atv_agc_speed_low_rssi.thld);
     break;
    #endif /*     Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP */
    #ifdef        Si2157_ATV_ARTIFICIAL_SNOW_PROP
     case         Si2157_ATV_ARTIFICIAL_SNOW_PROP_CODE:
      sprintf(msg,"ATV_ARTIFICIAL_SNOW");
       strcat(msg,separator); strcat(msg,"-GAIN ");
           if  (prop->atv_artificial_snow.gain   ==     0) strcat(msg,"AUTO ");
      else if  (prop->atv_artificial_snow.gain   ==     1) strcat(msg,"0DB  ");
      else if  (prop->atv_artificial_snow.gain   ==     2) strcat(msg,"6DB  ");
      else if  (prop->atv_artificial_snow.gain   ==     3) strcat(msg,"12DB ");
      else if  (prop->atv_artificial_snow.gain   ==     4) strcat(msg,"18DB ");
      else if  (prop->atv_artificial_snow.gain   ==     5) strcat(msg,"24DB ");
      else if  (prop->atv_artificial_snow.gain   ==     6) strcat(msg,"30DB ");
      else if  (prop->atv_artificial_snow.gain   ==     7) strcat(msg,"36DB ");
      else if  (prop->atv_artificial_snow.gain   ==     8) strcat(msg,"42DB ");
      else if  (prop->atv_artificial_snow.gain   ==     9) strcat(msg,"OFF  ");
      else                                                sprintf(msg,"%s%d", msg, prop->atv_artificial_snow.gain);
       strcat(msg,separator); strcat(msg,"-PERIOD ");
           if  (prop->atv_artificial_snow.period ==     0) strcat(msg,"LONG  ");
      else if  (prop->atv_artificial_snow.period ==     1) strcat(msg,"SHORT ");
      else                                                sprintf(msg,"%s%d", msg, prop->atv_artificial_snow.period);
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->atv_artificial_snow.offset);
     break;
    #endif /*     Si2157_ATV_ARTIFICIAL_SNOW_PROP */
    #ifdef        Si2157_ATV_CONFIG_IF_PORT_PROP
     case         Si2157_ATV_CONFIG_IF_PORT_PROP_CODE:
      sprintf(msg,"ATV_CONFIG_IF_PORT");
       strcat(msg,separator); strcat(msg,"-ATV_OUT_TYPE ");
           if  (prop->atv_config_if_port.atv_out_type   ==     8) strcat(msg,"LIF_DIFF_IF1 ");
      else if  (prop->atv_config_if_port.atv_out_type   ==    10) strcat(msg,"LIF_DIFF_IF2 ");
      else if  (prop->atv_config_if_port.atv_out_type   ==    12) strcat(msg,"LIF_SE_IF1A  ");
      else if  (prop->atv_config_if_port.atv_out_type   ==    14) strcat(msg,"LIF_SE_IF2A  ");
      else                                                       sprintf(msg,"%s%d", msg, prop->atv_config_if_port.atv_out_type);
       strcat(msg,separator); strcat(msg,"-ATV_AGC_SOURCE ");
           if  (prop->atv_config_if_port.atv_agc_source ==     0) strcat(msg,"INTERNAL ");
      else if  (prop->atv_config_if_port.atv_agc_source ==     1) strcat(msg,"AGC1_3DB ");
      else if  (prop->atv_config_if_port.atv_agc_source ==     2) strcat(msg,"AGC2_3DB ");
      else                                                       sprintf(msg,"%s%d", msg, prop->atv_config_if_port.atv_agc_source);
     break;
    #endif /*     Si2157_ATV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_ATV_EXT_AGC_PROP
     case         Si2157_ATV_EXT_AGC_PROP_CODE:
      sprintf(msg,"ATV_EXT_AGC");
       strcat(msg,separator); strcat(msg,"-MIN_10MV "); sprintf(msg,"%s%d", msg, prop->atv_ext_agc.min_10mv);
       strcat(msg,separator); strcat(msg,"-MAX_10MV "); sprintf(msg,"%s%d", msg, prop->atv_ext_agc.max_10mv);
     break;
    #endif /*     Si2157_ATV_EXT_AGC_PROP */
    #ifdef        Si2157_ATV_HSYNC_OUT_PROP
     case         Si2157_ATV_HSYNC_OUT_PROP_CODE:
      sprintf(msg,"ATV_HSYNC_OUT");
       strcat(msg,separator); strcat(msg,"-GPIO_SEL ");
           if  (prop->atv_hsync_out.gpio_sel ==     0) strcat(msg,"NONE  ");
      else if  (prop->atv_hsync_out.gpio_sel ==     1) strcat(msg,"GPIO1 ");
      else if  (prop->atv_hsync_out.gpio_sel ==     2) strcat(msg,"GPIO2 ");
      else                                            sprintf(msg,"%s%d", msg, prop->atv_hsync_out.gpio_sel);
       strcat(msg,separator); strcat(msg,"-WIDTH "); sprintf(msg,"%s%d", msg, prop->atv_hsync_out.width);
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->atv_hsync_out.offset);
     break;
    #endif /*     Si2157_ATV_HSYNC_OUT_PROP */
    #ifdef        Si2157_ATV_IEN_PROP
     case         Si2157_ATV_IEN_PROP_CODE:
      sprintf(msg,"ATV_IEN");
       strcat(msg,separator); strcat(msg,"-CHLIEN ");
           if  (prop->atv_ien.chlien ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_ien.chlien ==     1) strcat(msg,"ENABLE  ");
      else                                    sprintf(msg,"%s%d", msg, prop->atv_ien.chlien);
       strcat(msg,separator); strcat(msg,"-PCLIEN ");
           if  (prop->atv_ien.pclien ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_ien.pclien ==     1) strcat(msg,"ENABLE  ");
      else                                    sprintf(msg,"%s%d", msg, prop->atv_ien.pclien);
     break;
    #endif /*     Si2157_ATV_IEN_PROP */
    #ifdef        Si2157_ATV_INT_SENSE_PROP
     case         Si2157_ATV_INT_SENSE_PROP_CODE:
      sprintf(msg,"ATV_INT_SENSE");
       strcat(msg,separator); strcat(msg,"-CHLNEGEN ");
           if  (prop->atv_int_sense.chlnegen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_int_sense.chlnegen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->atv_int_sense.chlnegen);
       strcat(msg,separator); strcat(msg,"-PCLNEGEN ");
           if  (prop->atv_int_sense.pclnegen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_int_sense.pclnegen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->atv_int_sense.pclnegen);
       strcat(msg,separator); strcat(msg,"-CHLPOSEN ");
           if  (prop->atv_int_sense.chlposen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_int_sense.chlposen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->atv_int_sense.chlposen);
       strcat(msg,separator); strcat(msg,"-PCLPOSEN ");
           if  (prop->atv_int_sense.pclposen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_int_sense.pclposen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->atv_int_sense.pclposen);
     break;
    #endif /*     Si2157_ATV_INT_SENSE_PROP */
    #ifdef        Si2157_ATV_LIF_FREQ_PROP
     case         Si2157_ATV_LIF_FREQ_PROP_CODE:
      sprintf(msg,"ATV_LIF_FREQ");
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->atv_lif_freq.offset);
     break;
    #endif /*     Si2157_ATV_LIF_FREQ_PROP */
    #ifdef        Si2157_ATV_LIF_OUT_PROP
     case         Si2157_ATV_LIF_OUT_PROP_CODE:
      sprintf(msg,"ATV_LIF_OUT");
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->atv_lif_out.offset);
       strcat(msg,separator); strcat(msg,"-AMP "); sprintf(msg,"%s%d", msg, prop->atv_lif_out.amp);
     break;
    #endif /*     Si2157_ATV_LIF_OUT_PROP */
    #ifdef        Si2157_ATV_PGA_TARGET_PROP
     case         Si2157_ATV_PGA_TARGET_PROP_CODE:
      sprintf(msg,"ATV_PGA_TARGET");
       strcat(msg,separator); strcat(msg,"-PGA_TARGET "); sprintf(msg,"%s%d", msg, prop->atv_pga_target.pga_target);
       strcat(msg,separator); strcat(msg,"-OVERRIDE_ENABLE ");
           if  (prop->atv_pga_target.override_enable ==     0) strcat(msg,"DISABLE ");
      else if  (prop->atv_pga_target.override_enable ==     1) strcat(msg,"ENABLE  ");
      else                                                    sprintf(msg,"%s%d", msg, prop->atv_pga_target.override_enable);
     break;
    #endif /*     Si2157_ATV_PGA_TARGET_PROP */
    #ifdef        Si2157_ATV_RF_TOP_PROP
     case         Si2157_ATV_RF_TOP_PROP_CODE:
      sprintf(msg,"ATV_RF_TOP");
       strcat(msg,separator); strcat(msg,"-ATV_RF_TOP ");
           if  (prop->atv_rf_top.atv_rf_top ==     0) strcat(msg,"AUTO  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     1) strcat(msg,"P5DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     2) strcat(msg,"P4DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     3) strcat(msg,"P3DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     4) strcat(msg,"P2DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     5) strcat(msg,"P1DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     6) strcat(msg,"0DB   ");
      else if  (prop->atv_rf_top.atv_rf_top ==     7) strcat(msg,"M1DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     8) strcat(msg,"M2DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==     9) strcat(msg,"M3DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    10) strcat(msg,"M4DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    11) strcat(msg,"M5DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    12) strcat(msg,"M6DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    13) strcat(msg,"M7DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    14) strcat(msg,"M8DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    15) strcat(msg,"M9DB  ");
      else if  (prop->atv_rf_top.atv_rf_top ==    16) strcat(msg,"M10DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    17) strcat(msg,"M11DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    18) strcat(msg,"M12DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    19) strcat(msg,"M13DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    20) strcat(msg,"M14DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    21) strcat(msg,"M15DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    22) strcat(msg,"M16DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    23) strcat(msg,"M17DB ");
      else if  (prop->atv_rf_top.atv_rf_top ==    24) strcat(msg,"M18DB ");
      else                                           sprintf(msg,"%s%d", msg, prop->atv_rf_top.atv_rf_top);
     break;
    #endif /*     Si2157_ATV_RF_TOP_PROP */
    #ifdef        Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      sprintf(msg,"ATV_RSQ_RSSI_THRESHOLD");
       strcat(msg,separator); strcat(msg,"-LO "); sprintf(msg,"%s%d", msg, prop->atv_rsq_rssi_threshold.lo);
       strcat(msg,separator); strcat(msg,"-HI "); sprintf(msg,"%s%d", msg, prop->atv_rsq_rssi_threshold.hi);
     break;
    #endif /*     Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_ATV_VIDEO_MODE_PROP
     case         Si2157_ATV_VIDEO_MODE_PROP_CODE:
      sprintf(msg,"ATV_VIDEO_MODE");
       strcat(msg,separator); strcat(msg,"-VIDEO_SYS ");
           if  (prop->atv_video_mode.video_sys       ==     0) strcat(msg,"B  ");
      else if  (prop->atv_video_mode.video_sys       ==     1) strcat(msg,"GH ");
      else if  (prop->atv_video_mode.video_sys       ==     2) strcat(msg,"M  ");
      else if  (prop->atv_video_mode.video_sys       ==     3) strcat(msg,"N  ");
      else if  (prop->atv_video_mode.video_sys       ==     4) strcat(msg,"I  ");
      else if  (prop->atv_video_mode.video_sys       ==     5) strcat(msg,"DK ");
      else if  (prop->atv_video_mode.video_sys       ==     6) strcat(msg,"L  ");
      else if  (prop->atv_video_mode.video_sys       ==     7) strcat(msg,"LP ");
      else                                                    sprintf(msg,"%s%d", msg, prop->atv_video_mode.video_sys);
       strcat(msg,separator); strcat(msg,"-COLOR ");
           if  (prop->atv_video_mode.color           ==     0) strcat(msg,"PAL_NTSC ");
      else if  (prop->atv_video_mode.color           ==     1) strcat(msg,"SECAM    ");
      else                                                    sprintf(msg,"%s%d", msg, prop->atv_video_mode.color);
       strcat(msg,separator); strcat(msg,"-INVERT_SPECTRUM ");
           if  (prop->atv_video_mode.invert_spectrum ==     0) strcat(msg,"NORMAL   ");
      else if  (prop->atv_video_mode.invert_spectrum ==     1) strcat(msg,"INVERTED ");
      else                                                    sprintf(msg,"%s%d", msg, prop->atv_video_mode.invert_spectrum);
     break;
    #endif /*     Si2157_ATV_VIDEO_MODE_PROP */
    #ifdef        Si2157_ATV_VSNR_CAP_PROP
     case         Si2157_ATV_VSNR_CAP_PROP_CODE:
      sprintf(msg,"ATV_VSNR_CAP");
       strcat(msg,separator); strcat(msg,"-ATV_VSNR_CAP "); sprintf(msg,"%s%d", msg, prop->atv_vsnr_cap.atv_vsnr_cap);
     break;
    #endif /*     Si2157_ATV_VSNR_CAP_PROP */
    #ifdef        Si2157_CRYSTAL_TRIM_PROP
     case         Si2157_CRYSTAL_TRIM_PROP_CODE:
      sprintf(msg,"CRYSTAL_TRIM");
       strcat(msg,separator); strcat(msg,"-XO_CAP ");
           if  (prop->crystal_trim.xo_cap ==     0) strcat(msg,"4p7pF  ");
      else if  (prop->crystal_trim.xo_cap ==     1) strcat(msg,"4p95pF ");
      else if  (prop->crystal_trim.xo_cap ==     2) strcat(msg,"5p2pF  ");
      else if  (prop->crystal_trim.xo_cap ==     3) strcat(msg,"5p45pF ");
      else if  (prop->crystal_trim.xo_cap ==     4) strcat(msg,"5p7pF  ");
      else if  (prop->crystal_trim.xo_cap ==     5) strcat(msg,"5p95pF ");
      else if  (prop->crystal_trim.xo_cap ==     6) strcat(msg,"6p2pF  ");
      else if  (prop->crystal_trim.xo_cap ==     7) strcat(msg,"6p45pF ");
      else if  (prop->crystal_trim.xo_cap ==     8) strcat(msg,"6p7pF  ");
      else if  (prop->crystal_trim.xo_cap ==     9) strcat(msg,"6p95pF ");
      else if  (prop->crystal_trim.xo_cap ==    10) strcat(msg,"7p2pF  ");
      else if  (prop->crystal_trim.xo_cap ==    11) strcat(msg,"7p45pF ");
      else if  (prop->crystal_trim.xo_cap ==    12) strcat(msg,"7p7pF  ");
      else if  (prop->crystal_trim.xo_cap ==    13) strcat(msg,"7p95pF ");
      else if  (prop->crystal_trim.xo_cap ==    14) strcat(msg,"8p2pF  ");
      else if  (prop->crystal_trim.xo_cap ==    15) strcat(msg,"8p45pF ");
      else                                         sprintf(msg,"%s%d", msg, prop->crystal_trim.xo_cap);
     break;
    #endif /*     Si2157_CRYSTAL_TRIM_PROP */
    #ifdef        Si2157_DTV_AGC_FREEZE_INPUT_PROP
     case         Si2157_DTV_AGC_FREEZE_INPUT_PROP_CODE:
      sprintf(msg,"DTV_AGC_FREEZE_INPUT");
       strcat(msg,separator); strcat(msg,"-LEVEL ");
           if  (prop->dtv_agc_freeze_input.level ==     0) strcat(msg,"LOW  ");
      else if  (prop->dtv_agc_freeze_input.level ==     1) strcat(msg,"HIGH ");
      else                                                sprintf(msg,"%s%d", msg, prop->dtv_agc_freeze_input.level);
       strcat(msg,separator); strcat(msg,"-PIN ");
           if  (prop->dtv_agc_freeze_input.pin   ==     0) strcat(msg,"NONE     ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     1) strcat(msg,"GPIO1    ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     2) strcat(msg,"GPIO2    ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     3) strcat(msg,"RESERVED ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     4) strcat(msg,"AGC1     ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     5) strcat(msg,"AGC2     ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     6) strcat(msg,"LIF1A    ");
      else if  (prop->dtv_agc_freeze_input.pin   ==     7) strcat(msg,"LIF1B    ");
      else                                                sprintf(msg,"%s%d", msg, prop->dtv_agc_freeze_input.pin);
     break;
    #endif /*     Si2157_DTV_AGC_FREEZE_INPUT_PROP */
    #ifdef        Si2157_DTV_AGC_SPEED_PROP
     case         Si2157_DTV_AGC_SPEED_PROP_CODE:
      sprintf(msg,"DTV_AGC_SPEED");
       strcat(msg,separator); strcat(msg,"-IF_AGC_SPEED ");
           if  (prop->dtv_agc_speed.if_agc_speed ==     0) strcat(msg,"AUTO ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==    39) strcat(msg,"39   ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==    54) strcat(msg,"54   ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==    63) strcat(msg,"63   ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==    89) strcat(msg,"89   ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   105) strcat(msg,"105  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   121) strcat(msg,"121  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   137) strcat(msg,"137  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   158) strcat(msg,"158  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   172) strcat(msg,"172  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   185) strcat(msg,"185  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   196) strcat(msg,"196  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   206) strcat(msg,"206  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   216) strcat(msg,"216  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   219) strcat(msg,"219  ");
      else if  (prop->dtv_agc_speed.if_agc_speed ==   222) strcat(msg,"222  ");
      else                                                sprintf(msg,"%s%d", msg, prop->dtv_agc_speed.if_agc_speed);
       strcat(msg,separator); strcat(msg,"-AGC_DECIM ");
           if  (prop->dtv_agc_speed.agc_decim    ==     0) strcat(msg,"OFF ");
      else if  (prop->dtv_agc_speed.agc_decim    ==     1) strcat(msg,"2   ");
      else if  (prop->dtv_agc_speed.agc_decim    ==     2) strcat(msg,"4   ");
      else if  (prop->dtv_agc_speed.agc_decim    ==     3) strcat(msg,"8   ");
      else                                                sprintf(msg,"%s%d", msg, prop->dtv_agc_speed.agc_decim);
     break;
    #endif /*     Si2157_DTV_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_CONFIG_IF_PORT_PROP
     case         Si2157_DTV_CONFIG_IF_PORT_PROP_CODE:
      sprintf(msg,"DTV_CONFIG_IF_PORT");
       strcat(msg,separator); strcat(msg,"-DTV_OUT_TYPE ");
           if  (prop->dtv_config_if_port.dtv_out_type   ==     0) strcat(msg,"LIF_IF1     ");
      else if  (prop->dtv_config_if_port.dtv_out_type   ==     1) strcat(msg,"LIF_IF2     ");
      else if  (prop->dtv_config_if_port.dtv_out_type   ==     4) strcat(msg,"LIF_SE_IF1A ");
      else if  (prop->dtv_config_if_port.dtv_out_type   ==     5) strcat(msg,"LIF_SE_IF2A ");
      else                                                       sprintf(msg,"%s%d", msg, prop->dtv_config_if_port.dtv_out_type);
       strcat(msg,separator); strcat(msg,"-DTV_AGC_SOURCE ");
           if  (prop->dtv_config_if_port.dtv_agc_source ==     0) strcat(msg,"INTERNAL  ");
      else if  (prop->dtv_config_if_port.dtv_agc_source ==     1) strcat(msg,"AGC1_3DB  ");
      else if  (prop->dtv_config_if_port.dtv_agc_source ==     2) strcat(msg,"AGC2_3DB  ");
      else if  (prop->dtv_config_if_port.dtv_agc_source ==     3) strcat(msg,"AGC1_FULL ");
      else if  (prop->dtv_config_if_port.dtv_agc_source ==     4) strcat(msg,"AGC2_FULL ");
      else                                                       sprintf(msg,"%s%d", msg, prop->dtv_config_if_port.dtv_agc_source);
     break;
    #endif /*     Si2157_DTV_CONFIG_IF_PORT_PROP */
    #ifdef        Si2157_DTV_EXT_AGC_PROP
     case         Si2157_DTV_EXT_AGC_PROP_CODE:
      sprintf(msg,"DTV_EXT_AGC");
       strcat(msg,separator); strcat(msg,"-MIN_10MV "); sprintf(msg,"%s%d", msg, prop->dtv_ext_agc.min_10mv);
       strcat(msg,separator); strcat(msg,"-MAX_10MV "); sprintf(msg,"%s%d", msg, prop->dtv_ext_agc.max_10mv);
     break;
    #endif /*     Si2157_DTV_EXT_AGC_PROP */
    #ifdef        Si2157_DTV_IEN_PROP
     case         Si2157_DTV_IEN_PROP_CODE:
      sprintf(msg,"DTV_IEN");
       strcat(msg,separator); strcat(msg,"-CHLIEN ");
           if  (prop->dtv_ien.chlien ==     0) strcat(msg,"DISABLE ");
      else if  (prop->dtv_ien.chlien ==     1) strcat(msg,"ENABLE  ");
      else                                    sprintf(msg,"%s%d", msg, prop->dtv_ien.chlien);
     break;
    #endif /*     Si2157_DTV_IEN_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PROP_CODE:
      sprintf(msg,"DTV_INITIAL_AGC_SPEED");
       strcat(msg,separator); strcat(msg,"-IF_AGC_SPEED ");
           if  (prop->dtv_initial_agc_speed.if_agc_speed ==     0) strcat(msg,"AUTO ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    39) strcat(msg,"39   ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    54) strcat(msg,"54   ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    63) strcat(msg,"63   ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==    89) strcat(msg,"89   ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   105) strcat(msg,"105  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   121) strcat(msg,"121  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   137) strcat(msg,"137  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   158) strcat(msg,"158  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   172) strcat(msg,"172  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   185) strcat(msg,"185  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   196) strcat(msg,"196  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   206) strcat(msg,"206  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   216) strcat(msg,"216  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   219) strcat(msg,"219  ");
      else if  (prop->dtv_initial_agc_speed.if_agc_speed ==   222) strcat(msg,"222  ");
      else                                                        sprintf(msg,"%s%d", msg, prop->dtv_initial_agc_speed.if_agc_speed);
       strcat(msg,separator); strcat(msg,"-AGC_DECIM ");
           if  (prop->dtv_initial_agc_speed.agc_decim    ==     0) strcat(msg,"OFF ");
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     1) strcat(msg,"2   ");
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     2) strcat(msg,"4   ");
      else if  (prop->dtv_initial_agc_speed.agc_decim    ==     3) strcat(msg,"8   ");
      else                                                        sprintf(msg,"%s%d", msg, prop->dtv_initial_agc_speed.agc_decim);
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef        Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
     case         Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE:
      sprintf(msg,"DTV_INITIAL_AGC_SPEED_PERIOD");
       strcat(msg,separator); strcat(msg,"-PERIOD "); sprintf(msg,"%s%d", msg, prop->dtv_initial_agc_speed_period.period);
     break;
    #endif /*     Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef        Si2157_DTV_INTERNAL_ZIF_PROP
     case         Si2157_DTV_INTERNAL_ZIF_PROP_CODE:
      sprintf(msg,"DTV_INTERNAL_ZIF");
       strcat(msg,separator); strcat(msg,"-ATSC ");
           if  (prop->dtv_internal_zif.atsc   ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.atsc   ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.atsc);
       strcat(msg,separator); strcat(msg,"-QAM_US ");
           if  (prop->dtv_internal_zif.qam_us ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.qam_us ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.qam_us);
       strcat(msg,separator); strcat(msg,"-DVBT ");
           if  (prop->dtv_internal_zif.dvbt   ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.dvbt   ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.dvbt);
       strcat(msg,separator); strcat(msg,"-DVBC ");
           if  (prop->dtv_internal_zif.dvbc   ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.dvbc   ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.dvbc);
       strcat(msg,separator); strcat(msg,"-ISDBT ");
           if  (prop->dtv_internal_zif.isdbt  ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.isdbt  ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.isdbt);
       strcat(msg,separator); strcat(msg,"-ISDBC ");
           if  (prop->dtv_internal_zif.isdbc  ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.isdbc  ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.isdbc);
       strcat(msg,separator); strcat(msg,"-DTMB ");
           if  (prop->dtv_internal_zif.dtmb   ==     0) strcat(msg,"LIF ");
      else if  (prop->dtv_internal_zif.dtmb   ==     1) strcat(msg,"ZIF ");
      else                                             sprintf(msg,"%s%d", msg, prop->dtv_internal_zif.dtmb);
     break;
    #endif /*     Si2157_DTV_INTERNAL_ZIF_PROP */
    #ifdef        Si2157_DTV_INT_SENSE_PROP
     case         Si2157_DTV_INT_SENSE_PROP_CODE:
      sprintf(msg,"DTV_INT_SENSE");
       strcat(msg,separator); strcat(msg,"-CHLNEGEN ");
           if  (prop->dtv_int_sense.chlnegen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->dtv_int_sense.chlnegen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->dtv_int_sense.chlnegen);
       strcat(msg,separator); strcat(msg,"-CHLPOSEN ");
           if  (prop->dtv_int_sense.chlposen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->dtv_int_sense.chlposen ==     1) strcat(msg,"ENABLE  ");
      else                                            sprintf(msg,"%s%d", msg, prop->dtv_int_sense.chlposen);
     break;
    #endif /*     Si2157_DTV_INT_SENSE_PROP */
    #ifdef        Si2157_DTV_LIF_FREQ_PROP
     case         Si2157_DTV_LIF_FREQ_PROP_CODE:
      sprintf(msg,"DTV_LIF_FREQ");
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->dtv_lif_freq.offset);
     break;
    #endif /*     Si2157_DTV_LIF_FREQ_PROP */
    #ifdef        Si2157_DTV_LIF_OUT_PROP
     case         Si2157_DTV_LIF_OUT_PROP_CODE:
      sprintf(msg,"DTV_LIF_OUT");
       strcat(msg,separator); strcat(msg,"-OFFSET "); sprintf(msg,"%s%d", msg, prop->dtv_lif_out.offset);
       strcat(msg,separator); strcat(msg,"-AMP "); sprintf(msg,"%s%d", msg, prop->dtv_lif_out.amp);
     break;
    #endif /*     Si2157_DTV_LIF_OUT_PROP */
    #ifdef        Si2157_DTV_MODE_PROP
     case         Si2157_DTV_MODE_PROP_CODE:
      sprintf(msg,"DTV_MODE");
       strcat(msg,separator); strcat(msg,"-BW ");
           if  (prop->dtv_mode.bw              ==     6) strcat(msg,"BW_6MHZ   ");
      else if  (prop->dtv_mode.bw              ==     7) strcat(msg,"BW_7MHZ   ");
      else if  (prop->dtv_mode.bw              ==     8) strcat(msg,"BW_8MHZ   ");
      else if  (prop->dtv_mode.bw              ==     9) strcat(msg,"BW_1P7MHZ ");
      else if  (prop->dtv_mode.bw              ==    10) strcat(msg,"BW_6P1MHZ ");
      else                                              sprintf(msg,"%s%d", msg, prop->dtv_mode.bw);
       strcat(msg,separator); strcat(msg,"-MODULATION ");
           if  (prop->dtv_mode.modulation      ==     0) strcat(msg,"ATSC      ");
      else if  (prop->dtv_mode.modulation      ==     1) strcat(msg,"QAM_US    ");
      else if  (prop->dtv_mode.modulation      ==     2) strcat(msg,"DVBT      ");
      else if  (prop->dtv_mode.modulation      ==     3) strcat(msg,"DVBC      ");
      else if  (prop->dtv_mode.modulation      ==     4) strcat(msg,"ISDBT     ");
      else if  (prop->dtv_mode.modulation      ==     5) strcat(msg,"ISDBC     ");
      else if  (prop->dtv_mode.modulation      ==     6) strcat(msg,"DTMB      ");
      else if  (prop->dtv_mode.modulation      ==    14) strcat(msg,"CW_LEGACY ");
      else if  (prop->dtv_mode.modulation      ==    15) strcat(msg,"CW        ");
      else                                              sprintf(msg,"%s%d", msg, prop->dtv_mode.modulation);
       strcat(msg,separator); strcat(msg,"-INVERT_SPECTRUM ");
           if  (prop->dtv_mode.invert_spectrum ==     0) strcat(msg,"NORMAL   ");
      else if  (prop->dtv_mode.invert_spectrum ==     1) strcat(msg,"INVERTED ");
      else                                              sprintf(msg,"%s%d", msg, prop->dtv_mode.invert_spectrum);
     break;
    #endif /*     Si2157_DTV_MODE_PROP */
    #ifdef        Si2157_DTV_PGA_LIMITS_PROP
     case         Si2157_DTV_PGA_LIMITS_PROP_CODE:
      sprintf(msg,"DTV_PGA_LIMITS");
       strcat(msg,separator); strcat(msg,"-MIN "); sprintf(msg,"%s%d", msg, prop->dtv_pga_limits.min);
       strcat(msg,separator); strcat(msg,"-MAX "); sprintf(msg,"%s%d", msg, prop->dtv_pga_limits.max);
     break;
    #endif /*     Si2157_DTV_PGA_LIMITS_PROP */
    #ifdef        Si2157_DTV_PGA_TARGET_PROP
     case         Si2157_DTV_PGA_TARGET_PROP_CODE:
      sprintf(msg,"DTV_PGA_TARGET");
       strcat(msg,separator); strcat(msg,"-PGA_TARGET "); sprintf(msg,"%s%d", msg, prop->dtv_pga_target.pga_target);
       strcat(msg,separator); strcat(msg,"-OVERRIDE_ENABLE ");
           if  (prop->dtv_pga_target.override_enable ==     0) strcat(msg,"DISABLE ");
      else if  (prop->dtv_pga_target.override_enable ==     1) strcat(msg,"ENABLE  ");
      else                                                    sprintf(msg,"%s%d", msg, prop->dtv_pga_target.override_enable);
     break;
    #endif /*     Si2157_DTV_PGA_TARGET_PROP */
    #ifdef        Si2157_DTV_RF_TOP_PROP
     case         Si2157_DTV_RF_TOP_PROP_CODE:
      sprintf(msg,"DTV_RF_TOP");
       strcat(msg,separator); strcat(msg,"-DTV_RF_TOP ");
           if  (prop->dtv_rf_top.dtv_rf_top ==     0) strcat(msg,"AUTO    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     1) strcat(msg,"P10DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     2) strcat(msg,"P9P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     3) strcat(msg,"P9DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     4) strcat(msg,"P8P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     5) strcat(msg,"P8DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     6) strcat(msg,"P7P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     7) strcat(msg,"P7DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     8) strcat(msg,"P6P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==     9) strcat(msg,"P6DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    10) strcat(msg,"P5P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    11) strcat(msg,"P5DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    12) strcat(msg,"P4P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    13) strcat(msg,"P4DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    14) strcat(msg,"P3P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    15) strcat(msg,"P3DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    16) strcat(msg,"P2P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    17) strcat(msg,"P2DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    18) strcat(msg,"P1P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    19) strcat(msg,"P1DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    20) strcat(msg,"P0P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    21) strcat(msg,"0DB     ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    22) strcat(msg,"M0P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    23) strcat(msg,"M1DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    24) strcat(msg,"M1P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    25) strcat(msg,"M2DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    26) strcat(msg,"M2P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    27) strcat(msg,"M3DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    28) strcat(msg,"M3P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    29) strcat(msg,"M4DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    30) strcat(msg,"M4P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    31) strcat(msg,"M5DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    32) strcat(msg,"M5P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    33) strcat(msg,"M6DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    34) strcat(msg,"M6P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    35) strcat(msg,"M7DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    36) strcat(msg,"M7P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    37) strcat(msg,"M8DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    38) strcat(msg,"M8P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    39) strcat(msg,"M9DB    ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    40) strcat(msg,"M9P5DB  ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    41) strcat(msg,"M10DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    42) strcat(msg,"M10P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    43) strcat(msg,"M11DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    44) strcat(msg,"M11P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    45) strcat(msg,"M12DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    46) strcat(msg,"M12P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    47) strcat(msg,"M13DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    48) strcat(msg,"M13P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    49) strcat(msg,"M14DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    50) strcat(msg,"M14P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    51) strcat(msg,"M15DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    52) strcat(msg,"M15P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    53) strcat(msg,"M16DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    54) strcat(msg,"M16P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    55) strcat(msg,"M17DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    56) strcat(msg,"M17P5DB ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    57) strcat(msg,"M18DB   ");
      else if  (prop->dtv_rf_top.dtv_rf_top ==    58) strcat(msg,"M18P5DB ");
      else                                           sprintf(msg,"%s%d", msg, prop->dtv_rf_top.dtv_rf_top);
     break;
    #endif /*     Si2157_DTV_RF_TOP_PROP */
    #ifdef        Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP
     case         Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE:
      sprintf(msg,"DTV_RSQ_RSSI_THRESHOLD");
       strcat(msg,separator); strcat(msg,"-LO "); sprintf(msg,"%s%d", msg, prop->dtv_rsq_rssi_threshold.lo);
       strcat(msg,separator); strcat(msg,"-HI "); sprintf(msg,"%s%d", msg, prop->dtv_rsq_rssi_threshold.hi);
     break;
    #endif /*     Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef        Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP
     case         Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_CODE:
      sprintf(msg,"DTV_ZIF_DC_CANCELLER_BW");
       strcat(msg,separator); strcat(msg,"-BANDWIDTH ");
           if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     0) strcat(msg,"4_Hz      ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     1) strcat(msg,"8_Hz      ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     2) strcat(msg,"15_Hz     ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     3) strcat(msg,"30_Hz     ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     4) strcat(msg,"61_Hz     ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     5) strcat(msg,"121_Hz    ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     6) strcat(msg,"243_Hz    ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     7) strcat(msg,"486_Hz    ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     8) strcat(msg,"972_Hz    ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==     9) strcat(msg,"1943_Hz   ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    10) strcat(msg,"3888_Hz   ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    11) strcat(msg,"7779_Hz   ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    12) strcat(msg,"15573_Hz  ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    13) strcat(msg,"31207_Hz  ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    14) strcat(msg,"62658_Hz  ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    15) strcat(msg,"126303_Hz ");
      else if  (prop->dtv_zif_dc_canceller_bw.bandwidth ==    16) strcat(msg,"DEFAULT   ");
      else                                                       sprintf(msg,"%s%d", msg, prop->dtv_zif_dc_canceller_bw.bandwidth);
     break;
    #endif /*     Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP */
    #ifdef        Si2157_MASTER_IEN_PROP
     case         Si2157_MASTER_IEN_PROP_CODE:
      sprintf(msg,"MASTER_IEN");
       strcat(msg,separator); strcat(msg,"-TUNIEN ");
           if  (prop->master_ien.tunien ==     0) strcat(msg,"OFF ");
      else if  (prop->master_ien.tunien ==     1) strcat(msg,"ON  ");
      else                                       sprintf(msg,"%s%d", msg, prop->master_ien.tunien);
       strcat(msg,separator); strcat(msg,"-ATVIEN ");
           if  (prop->master_ien.atvien ==     0) strcat(msg,"OFF ");
      else if  (prop->master_ien.atvien ==     1) strcat(msg,"ON  ");
      else                                       sprintf(msg,"%s%d", msg, prop->master_ien.atvien);
       strcat(msg,separator); strcat(msg,"-DTVIEN ");
           if  (prop->master_ien.dtvien ==     0) strcat(msg,"OFF ");
      else if  (prop->master_ien.dtvien ==     1) strcat(msg,"ON  ");
      else                                       sprintf(msg,"%s%d", msg, prop->master_ien.dtvien);
       strcat(msg,separator); strcat(msg,"-ERRIEN ");
           if  (prop->master_ien.errien ==     0) strcat(msg,"OFF ");
      else if  (prop->master_ien.errien ==     1) strcat(msg,"ON  ");
      else                                       sprintf(msg,"%s%d", msg, prop->master_ien.errien);
       strcat(msg,separator); strcat(msg,"-CTSIEN ");
           if  (prop->master_ien.ctsien ==     0) strcat(msg,"OFF ");
      else if  (prop->master_ien.ctsien ==     1) strcat(msg,"ON  ");
      else                                       sprintf(msg,"%s%d", msg, prop->master_ien.ctsien);
     break;
    #endif /*     Si2157_MASTER_IEN_PROP */
    #ifdef        Si2157_TUNER_BLOCKED_VCO_PROP
     case         Si2157_TUNER_BLOCKED_VCO_PROP_CODE:
      sprintf(msg,"TUNER_BLOCKED_VCO");
       strcat(msg,separator); strcat(msg,"-VCO_CODE "); sprintf(msg,"%s%d", msg, prop->tuner_blocked_vco.vco_code);
     break;
    #endif /*     Si2157_TUNER_BLOCKED_VCO_PROP */
    #ifdef        Si2157_TUNER_IEN_PROP
     case         Si2157_TUNER_IEN_PROP_CODE:
      sprintf(msg,"TUNER_IEN");
       strcat(msg,separator); strcat(msg,"-TCIEN ");
           if  (prop->tuner_ien.tcien    ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_ien.tcien    ==     1) strcat(msg,"ENABLE  ");
      else                                        sprintf(msg,"%s%d", msg, prop->tuner_ien.tcien);
       strcat(msg,separator); strcat(msg,"-RSSILIEN ");
           if  (prop->tuner_ien.rssilien ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_ien.rssilien ==     1) strcat(msg,"ENABLE  ");
      else                                        sprintf(msg,"%s%d", msg, prop->tuner_ien.rssilien);
       strcat(msg,separator); strcat(msg,"-RSSIHIEN ");
           if  (prop->tuner_ien.rssihien ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_ien.rssihien ==     1) strcat(msg,"ENABLE  ");
      else                                        sprintf(msg,"%s%d", msg, prop->tuner_ien.rssihien);
     break;
    #endif /*     Si2157_TUNER_IEN_PROP */
    #ifdef        Si2157_TUNER_INT_SENSE_PROP
     case         Si2157_TUNER_INT_SENSE_PROP_CODE:
      sprintf(msg,"TUNER_INT_SENSE");
       strcat(msg,separator); strcat(msg,"-TCNEGEN ");
           if  (prop->tuner_int_sense.tcnegen    ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.tcnegen    ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.tcnegen);
       strcat(msg,separator); strcat(msg,"-RSSILNEGEN ");
           if  (prop->tuner_int_sense.rssilnegen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.rssilnegen ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.rssilnegen);
       strcat(msg,separator); strcat(msg,"-RSSIHNEGEN ");
           if  (prop->tuner_int_sense.rssihnegen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.rssihnegen ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.rssihnegen);
       strcat(msg,separator); strcat(msg,"-TCPOSEN ");
           if  (prop->tuner_int_sense.tcposen    ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.tcposen    ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.tcposen);
       strcat(msg,separator); strcat(msg,"-RSSILPOSEN ");
           if  (prop->tuner_int_sense.rssilposen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.rssilposen ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.rssilposen);
       strcat(msg,separator); strcat(msg,"-RSSIHPOSEN ");
           if  (prop->tuner_int_sense.rssihposen ==     0) strcat(msg,"DISABLE ");
      else if  (prop->tuner_int_sense.rssihposen ==     1) strcat(msg,"ENABLE  ");
      else                                                sprintf(msg,"%s%d", msg, prop->tuner_int_sense.rssihposen);
     break;
    #endif /*     Si2157_TUNER_INT_SENSE_PROP */
    #ifdef        Si2157_TUNER_LO_INJECTION_PROP
     case         Si2157_TUNER_LO_INJECTION_PROP_CODE:
      sprintf(msg,"TUNER_LO_INJECTION");
       strcat(msg,separator); strcat(msg,"-BAND_1 ");
           if  (prop->tuner_lo_injection.band_1 ==     0) strcat(msg,"LOW_SIDE  ");
      else if  (prop->tuner_lo_injection.band_1 ==     1) strcat(msg,"HIGH_SIDE ");
      else                                               sprintf(msg,"%s%d", msg, prop->tuner_lo_injection.band_1);
       strcat(msg,separator); strcat(msg,"-BAND_2 ");
           if  (prop->tuner_lo_injection.band_2 ==     0) strcat(msg,"LOW_SIDE  ");
      else if  (prop->tuner_lo_injection.band_2 ==     1) strcat(msg,"HIGH_SIDE ");
      else                                               sprintf(msg,"%s%d", msg, prop->tuner_lo_injection.band_2);
       strcat(msg,separator); strcat(msg,"-BAND_3 ");
           if  (prop->tuner_lo_injection.band_3 ==     0) strcat(msg,"LOW_SIDE  ");
      else if  (prop->tuner_lo_injection.band_3 ==     1) strcat(msg,"HIGH_SIDE ");
      else                                               sprintf(msg,"%s%d", msg, prop->tuner_lo_injection.band_3);
     break;
    #endif /*     Si2157_TUNER_LO_INJECTION_PROP */
    #ifdef        Si2157_TUNER_RETURN_LOSS_PROP
     case         Si2157_TUNER_RETURN_LOSS_PROP_CODE:
      sprintf(msg,"TUNER_RETURN_LOSS");
       strcat(msg,separator); strcat(msg,"-CONFIG ");
           if  (prop->tuner_return_loss.config ==     3) strcat(msg,"3   ");
      else if  (prop->tuner_return_loss.config ==     7) strcat(msg,"7   ");
      else if  (prop->tuner_return_loss.config ==    11) strcat(msg,"11  ");
      else if  (prop->tuner_return_loss.config ==    15) strcat(msg,"15  ");
      else if  (prop->tuner_return_loss.config ==    19) strcat(msg,"19  ");
      else if  (prop->tuner_return_loss.config ==    23) strcat(msg,"23  ");
      else if  (prop->tuner_return_loss.config ==    27) strcat(msg,"27  ");
      else if  (prop->tuner_return_loss.config ==    31) strcat(msg,"31  ");
      else if  (prop->tuner_return_loss.config ==    35) strcat(msg,"35  ");
      else if  (prop->tuner_return_loss.config ==    39) strcat(msg,"39  ");
      else if  (prop->tuner_return_loss.config ==    43) strcat(msg,"43  ");
      else if  (prop->tuner_return_loss.config ==    47) strcat(msg,"47  ");
      else if  (prop->tuner_return_loss.config ==    51) strcat(msg,"51  ");
      else if  (prop->tuner_return_loss.config ==    59) strcat(msg,"59  ");
      else if  (prop->tuner_return_loss.config ==    67) strcat(msg,"67  ");
      else if  (prop->tuner_return_loss.config ==    75) strcat(msg,"75  ");
      else if  (prop->tuner_return_loss.config ==    83) strcat(msg,"83  ");
      else if  (prop->tuner_return_loss.config ==    91) strcat(msg,"91  ");
      else if  (prop->tuner_return_loss.config ==   103) strcat(msg,"103 ");
      else if  (prop->tuner_return_loss.config ==   115) strcat(msg,"115 ");
      else if  (prop->tuner_return_loss.config ==   127) strcat(msg,"127 ");
      else                                              sprintf(msg,"%s%d", msg, prop->tuner_return_loss.config);
       strcat(msg,separator); strcat(msg,"-MODE ");
           if  (prop->tuner_return_loss.mode   ==     0) strcat(msg,"TERRESTRIAL ");
      else if  (prop->tuner_return_loss.mode   ==     1) strcat(msg,"CABLE       ");
      else                                              sprintf(msg,"%s%d", msg, prop->tuner_return_loss.mode);
     break;
    #endif /*     Si2157_TUNER_RETURN_LOSS_PROP */
    #ifdef        Si2157_WIDE_BAND_ATT_THRS_PROP
     case         Si2157_WIDE_BAND_ATT_THRS_PROP_CODE:
      sprintf(msg,"WIDE_BAND_ATT_THRS");
       strcat(msg,separator); strcat(msg,"-WB_ATT_THRS ");
           if  (api->prop->wide_band_att_thrs.wb_att_thrs ==     0) strcat(msg,"AUTO   ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==     6) strcat(msg,"P6DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==     7) strcat(msg,"P5DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==     8) strcat(msg,"P4DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==     9) strcat(msg,"P3DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    10) strcat(msg,"P2DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    11) strcat(msg,"P1DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    12) strcat(msg,"0DBM   ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    13) strcat(msg,"M1DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    14) strcat(msg,"M2DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    15) strcat(msg,"M3DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    16) strcat(msg,"M4DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    17) strcat(msg,"M5DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    18) strcat(msg,"M6DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    19) strcat(msg,"M7DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    20) strcat(msg,"M8DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    21) strcat(msg,"M9DBM  ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    22) strcat(msg,"M10DBM ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    23) strcat(msg,"M11DBM ");
      else if  (api->prop->wide_band_att_thrs.wb_att_thrs ==    24) strcat(msg,"M12DBM ");
      else                                                         sprintf(msg,"%s%d", msg, api->prop->wide_band_att_thrs.wb_att_thrs);
     break;
    #endif /*     Si2157_WIDE_BAND_ATT_THRS_PROP */
    #ifdef        Si2157_XOUT_PROP
     case         Si2157_XOUT_PROP_CODE:
      sprintf(msg,"XOUT");
       strcat(msg,separator); strcat(msg,"-AMP ");
           if  (prop->xout.amp ==     0) strcat(msg,"HIGH ");
      else if  (prop->xout.amp ==     1) strcat(msg,"LOW  ");
      else                              sprintf(msg,"%s%d", msg, prop->xout.amp);
     break;
    #endif /*     Si2157_XOUT_PROP */
     default : sprintf(msg,"Unknown property code '0x%06x'\n", prop_code); return ERROR_Si2157_UNKNOWN_PROPERTY; break;
  }
  return NO_Si2157_ERROR;
}
/***********************************************************************************************************************
  Si2157_L1_FillPropertyStringText function
  Use:        property text retrieval function
              Used to retrieve the property text for a selected property.
  Parameter: *api      the Si2157 context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string (usually declared with a size of 1000 bytes)
  Returns:    NO_Si2157_ERROR if successful.
 ***********************************************************************************************************************/
void          Si2157_L1_FillPropertyStringText(L1_Si2157_Context *api, unsigned int prop_code, const char *separator, char *msg) {
  Si2157_L1_PropertyText (api->prop, prop_code, separator, msg);
    }
/***********************************************************************************************************************
  Si2157_L1_GetPropertyString function
  Use:        current property text retrieval function
              Used to retrieve the property value from the hardware then retrieve the corresponding property text.
  Parameter: *api      the Si2157 context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string (usually declared with a size of 1000 bytes)
  Returns:    NO_Si2157_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2157_L1_GetPropertyString     (L1_Si2157_Context *api, unsigned int prop_code, const char *separator, char *msg) {
    int res;
    res = Si2157_L1_GetProperty2(api,prop_code);
    if (res!=NO_Si2157_ERROR) { sprintf(msg, "%s",Si2157_L1_API_ERROR_TEXT(res)); return res; }
    Si2157_L1_PropertyText(api->prop, prop_code, separator, msg);
    return NO_Si2157_ERROR;
  }
#endif /* Si2157_GET_PROPERTY_STRING */








