/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties strings definitions
   FILE: Si2147_Properties_Strings.h
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Date: November 06 2013
   Tag: ROM50_3_4_build_4_V0.1
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2147_PROPERTIES_STRINGS_H_
#define   _Si2147_PROPERTIES_STRINGS_H_

#ifdef Si2147_COMMAND_LINE_APPLICATION

extern Si2147_PropObj Si2147_prop;

typedef enum   {
  Si2147_UNSIGNED_CHAR,
  Si2147_SIGNED_CHAR,
  Si2147_UNSIGNED_INT,
  Si2147_SIGNED_INT
} Si2147_datatypeEnum;

typedef struct {
  const char *name;
  int         value;
} Si2147_optionStruct;

typedef enum {
  Si2147_ATV,
  Si2147_COMMON,
  Si2147_DTV,
  Si2147_TUNER
} Si2147_featureEnum;

typedef struct {
  const char          *name;
  Si2147_datatypeEnum  datatype;
  void                *pField;
  int                  nbOptions;
  Si2147_optionStruct *option;
} Si2147_fieldDicoStruct;

typedef struct {
  Si2147_featureEnum      feature;
  const char             *name;
  int                     nbFields;
  Si2147_fieldDicoStruct *field;
  unsigned int            propertyCode;
} Si2147_propertyInfoStruct;


extern Si2147_optionStruct    Si2147_ATV_AFC_RANGE_PROP_RANGE_KHZ[];
extern Si2147_fieldDicoStruct Si2147_ATV_AFC_RANGE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_AGC_SPEED_PROP_IF_AGC_SPEED[];
extern Si2147_fieldDicoStruct Si2147_ATV_AGC_SPEED_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED[];
extern Si2147_optionStruct    Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD[];
extern Si2147_fieldDicoStruct Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_GAIN[];
extern Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_PERIOD[];
extern Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_OFFSET[];
extern Si2147_fieldDicoStruct Si2147_ATV_ARTIFICIAL_SNOW_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE[];
extern Si2147_optionStruct    Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE[];
extern Si2147_fieldDicoStruct Si2147_ATV_CONFIG_IF_PORT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_EXT_AGC_PROP_MIN_10MV[];
extern Si2147_optionStruct    Si2147_ATV_EXT_AGC_PROP_MAX_10MV[];
extern Si2147_fieldDicoStruct Si2147_ATV_EXT_AGC_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_GPIO_SEL[];
extern Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_WIDTH[];
extern Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_OFFSET[];
extern Si2147_fieldDicoStruct Si2147_ATV_HSYNC_OUT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_IEN_PROP_CHLIEN[];
extern Si2147_optionStruct    Si2147_ATV_IEN_PROP_PCLIEN[];
extern Si2147_fieldDicoStruct Si2147_ATV_IEN_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_CHLNEGEN[];
extern Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_PCLNEGEN[];
extern Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_CHLPOSEN[];
extern Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_PCLPOSEN[];
extern Si2147_fieldDicoStruct Si2147_ATV_INT_SENSE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_LIF_FREQ_PROP_OFFSET[];
extern Si2147_fieldDicoStruct Si2147_ATV_LIF_FREQ_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_LIF_OUT_PROP_OFFSET[];
extern Si2147_optionStruct    Si2147_ATV_LIF_OUT_PROP_AMP[];
extern Si2147_fieldDicoStruct Si2147_ATV_LIF_OUT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_PGA_TARGET_PROP_PGA_TARGET[];
extern Si2147_optionStruct    Si2147_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE[];
extern Si2147_fieldDicoStruct Si2147_ATV_PGA_TARGET_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_RF_TOP_PROP_ATV_RF_TOP[];
extern Si2147_fieldDicoStruct Si2147_ATV_RF_TOP_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_LO[];
extern Si2147_optionStruct    Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_HI[];
extern Si2147_fieldDicoStruct Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_VIDEO_SYS[];
extern Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_COLOR[];
extern Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM[];
extern Si2147_fieldDicoStruct Si2147_ATV_VIDEO_MODE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP[];
extern Si2147_fieldDicoStruct Si2147_ATV_VSNR_CAP_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_CRYSTAL_TRIM_PROP_XO_CAP[];
extern Si2147_fieldDicoStruct Si2147_CRYSTAL_TRIM_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_AGC_FREEZE_INPUT_PROP_LEVEL[];
extern Si2147_optionStruct    Si2147_DTV_AGC_FREEZE_INPUT_PROP_PIN[];
extern Si2147_fieldDicoStruct Si2147_DTV_AGC_FREEZE_INPUT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_AGC_SPEED_PROP_IF_AGC_SPEED[];
extern Si2147_optionStruct    Si2147_DTV_AGC_SPEED_PROP_AGC_DECIM[];
extern Si2147_fieldDicoStruct Si2147_DTV_AGC_SPEED_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE[];
extern Si2147_optionStruct    Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE[];
extern Si2147_fieldDicoStruct Si2147_DTV_CONFIG_IF_PORT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_EXT_AGC_PROP_MIN_10MV[];
extern Si2147_optionStruct    Si2147_DTV_EXT_AGC_PROP_MAX_10MV[];
extern Si2147_fieldDicoStruct Si2147_DTV_EXT_AGC_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_IEN_PROP_CHLIEN[];
extern Si2147_fieldDicoStruct Si2147_DTV_IEN_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED[];
extern Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM[];
extern Si2147_fieldDicoStruct Si2147_DTV_INITIAL_AGC_SPEED_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD[];
extern Si2147_fieldDicoStruct Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ATSC[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_QAM_US[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DVBT[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DVBC[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ISDBT[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ISDBC[];
extern Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DTMB[];
extern Si2147_fieldDicoStruct Si2147_DTV_INTERNAL_ZIF_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_INT_SENSE_PROP_CHLNEGEN[];
extern Si2147_optionStruct    Si2147_DTV_INT_SENSE_PROP_CHLPOSEN[];
extern Si2147_fieldDicoStruct Si2147_DTV_INT_SENSE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_LIF_FREQ_PROP_OFFSET[];
extern Si2147_fieldDicoStruct Si2147_DTV_LIF_FREQ_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_LIF_OUT_PROP_OFFSET[];
extern Si2147_optionStruct    Si2147_DTV_LIF_OUT_PROP_AMP[];
extern Si2147_fieldDicoStruct Si2147_DTV_LIF_OUT_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_MODE_PROP_BW[];
extern Si2147_optionStruct    Si2147_DTV_MODE_PROP_MODULATION[];
extern Si2147_optionStruct    Si2147_DTV_MODE_PROP_INVERT_SPECTRUM[];
extern Si2147_fieldDicoStruct Si2147_DTV_MODE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_PGA_LIMITS_PROP_MIN[];
extern Si2147_optionStruct    Si2147_DTV_PGA_LIMITS_PROP_MAX[];
extern Si2147_fieldDicoStruct Si2147_DTV_PGA_LIMITS_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_PGA_TARGET_PROP_PGA_TARGET[];
extern Si2147_optionStruct    Si2147_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE[];
extern Si2147_fieldDicoStruct Si2147_DTV_PGA_TARGET_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_RF_TOP_PROP_DTV_RF_TOP[];
extern Si2147_fieldDicoStruct Si2147_DTV_RF_TOP_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_LO[];
extern Si2147_optionStruct    Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_HI[];
extern Si2147_fieldDicoStruct Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH[];
extern Si2147_fieldDicoStruct Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_MASTER_IEN_PROP_TUNIEN[];
extern Si2147_optionStruct    Si2147_MASTER_IEN_PROP_ATVIEN[];
extern Si2147_optionStruct    Si2147_MASTER_IEN_PROP_DTVIEN[];
extern Si2147_optionStruct    Si2147_MASTER_IEN_PROP_ERRIEN[];
extern Si2147_optionStruct    Si2147_MASTER_IEN_PROP_CTSIEN[];
extern Si2147_fieldDicoStruct Si2147_MASTER_IEN_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_TUNER_BLOCKED_VCO_PROP_VCO_CODE[];
extern Si2147_fieldDicoStruct Si2147_TUNER_BLOCKED_VCO_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_TUNER_IEN_PROP_TCIEN[];
extern Si2147_optionStruct    Si2147_TUNER_IEN_PROP_RSSILIEN[];
extern Si2147_optionStruct    Si2147_TUNER_IEN_PROP_RSSIHIEN[];
extern Si2147_fieldDicoStruct Si2147_TUNER_IEN_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_TCNEGEN[];
extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSILNEGEN[];
extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSIHNEGEN[];
extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_TCPOSEN[];
extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSILPOSEN[];
extern Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSIHPOSEN[];
extern Si2147_fieldDicoStruct Si2147_TUNER_INT_SENSE_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_1[];
extern Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_2[];
extern Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_3[];
extern Si2147_fieldDicoStruct Si2147_TUNER_LO_INJECTION_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_TUNER_RETURN_LOSS_PROP_CONFIG[];
extern Si2147_optionStruct    Si2147_TUNER_RETURN_LOSS_PROP_MODE[];
extern Si2147_fieldDicoStruct Si2147_TUNER_RETURN_LOSS_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS[];
extern Si2147_fieldDicoStruct Si2147_WIDE_BAND_ATT_THRS_PROP_FIELDS[];

extern Si2147_optionStruct    Si2147_XOUT_PROP_AMP[];
extern Si2147_fieldDicoStruct Si2147_XOUT_PROP_FIELDS[];

extern Si2147_propertyInfoStruct Si2147_propertyDictionary[];

int   Si2147_PropertyNames       (L1_Si2147_Context *api, char *msg);
int   Si2147_PropertyIndex       (L1_Si2147_Context *api, const char *property);
int   Si2147_PropertyFields      (L1_Si2147_Context *api, int propIndex, char *msg);
int   Si2147_PropertyFieldIndex  (L1_Si2147_Context *api, int propIndex, char *field);
int   Si2147_PropertyFieldEnums  (L1_Si2147_Context *api, int propIndex, int fieldIndex, char *msg);
int   Si2147_GetValueFromEntry   (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, char *entry, int *value);
char *Si2147_GetEnumFromValue    (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, int iValue);
char *Si2147_GetEnumFromField    (L1_Si2147_Context *api, Si2147_fieldDicoStruct field);
int   Si2147_SetFieldFromEntry   (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, char *entry);
void  Si2147_showProperty        (L1_Si2147_Context *api, int propertyIndex);
void  Si2147_showPropertyNamed   (L1_Si2147_Context *api, char *property);
int   Si2147_showProperties      (L1_Si2147_Context *api, Si2147_featureEnum feature);
void  Si2147_setupProperty       (L1_Si2147_Context *api, int propIndex);
int   Si2147_setupProperties     (L1_Si2147_Context *api, Si2147_featureEnum feature);

#endif /* Si2147_COMMAND_LINE_APPLICATION */

int  Si2147_setupATVProperties   (L1_Si2147_Context *api);
int  Si2147_setupCOMMONProperties(L1_Si2147_Context *api);
int  Si2147_setupDTVProperties   (L1_Si2147_Context *api);
int  Si2147_setupTUNERProperties (L1_Si2147_Context *api);
int  Si2147_setupAllProperties   (L1_Si2147_Context *api);


#endif /* _Si2147_PROPERTIES_STRINGS_H_ */





