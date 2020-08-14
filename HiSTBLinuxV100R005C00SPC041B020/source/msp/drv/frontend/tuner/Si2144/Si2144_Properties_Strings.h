/*************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties strings definitions
   FILE: Si2144_Properties_Strings.h
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_0_build_2
   Revision: REVISION_NAME
   Date: November 06 2014
   Tag: TAGNAME
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2144_PROPERTIES_STRINGS_H_
#define   _Si2144_PROPERTIES_STRINGS_H_

#ifdef Si2144_COMMAND_LINE_APPLICATION

extern Si2144_PropObj Si2144_prop;

typedef enum   {
  Si2144_UNSIGNED_CHAR,
  Si2144_SIGNED_CHAR,
  Si2144_UNSIGNED_INT,
  Si2144_SIGNED_INT
} Si2144_datatypeEnum;

typedef struct {
  const char *name;
  int         value;
} Si2144_optionStruct;

typedef enum {
  Si2144_COMMON,
  Si2144_DTV,
  Si2144_TUNER
} Si2144_featureEnum;

typedef struct {
  const char          *name;
  Si2144_datatypeEnum  datatype;
  void                *pField;
  int                  nbOptions;
  Si2144_optionStruct *option;
} Si2144_fieldDicoStruct;

typedef struct {
  Si2144_featureEnum      feature;
  const char             *name;
  int                     nbFields;
  Si2144_fieldDicoStruct *field;
  unsigned int            propertyCode;
} Si2144_propertyInfoStruct;


extern Si2144_optionStruct    Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE[];
extern Si2144_fieldDicoStruct Si2144_ACTIVE_LOOP_THROUGH_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_CRYSTAL_TRIM_PROP_XO_CAP[];
extern Si2144_fieldDicoStruct Si2144_CRYSTAL_TRIM_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD[];
extern Si2144_optionStruct    Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT[];
extern Si2144_fieldDicoStruct Si2144_DTV_AGC_AUTO_FREEZE_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED[];
extern Si2144_optionStruct    Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM[];
extern Si2144_fieldDicoStruct Si2144_DTV_AGC_SPEED_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE[];
extern Si2144_fieldDicoStruct Si2144_DTV_CONFIG_IF_PORT_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_EXT_AGC_PROP_MIN_10MV[];
extern Si2144_optionStruct    Si2144_DTV_EXT_AGC_PROP_MAX_10MV[];
extern Si2144_fieldDicoStruct Si2144_DTV_EXT_AGC_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK[];
extern Si2144_optionStruct    Si2144_DTV_IF_AGC_SPEED_PROP_DECAY[];
extern Si2144_fieldDicoStruct Si2144_DTV_IF_AGC_SPEED_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED[];
extern Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM[];
extern Si2144_fieldDicoStruct Si2144_DTV_INITIAL_AGC_SPEED_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD[];
extern Si2144_fieldDicoStruct Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ATSC[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DVBT[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DVBC[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC[];
extern Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DTMB[];
extern Si2144_fieldDicoStruct Si2144_DTV_INTERNAL_ZIF_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_LIF_FREQ_PROP_OFFSET[];
extern Si2144_fieldDicoStruct Si2144_DTV_LIF_FREQ_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_LIF_OUT_PROP_OFFSET[];
extern Si2144_optionStruct    Si2144_DTV_LIF_OUT_PROP_AMP[];
extern Si2144_fieldDicoStruct Si2144_DTV_LIF_OUT_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_MODE_PROP_BW[];
extern Si2144_optionStruct    Si2144_DTV_MODE_PROP_MODULATION[];
extern Si2144_optionStruct    Si2144_DTV_MODE_PROP_INVERT_SPECTRUM[];
extern Si2144_fieldDicoStruct Si2144_DTV_MODE_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_PGA_LIMITS_PROP_MIN[];
extern Si2144_optionStruct    Si2144_DTV_PGA_LIMITS_PROP_MAX[];
extern Si2144_fieldDicoStruct Si2144_DTV_PGA_LIMITS_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET[];
extern Si2144_optionStruct    Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE[];
extern Si2144_fieldDicoStruct Si2144_DTV_PGA_TARGET_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK[];
extern Si2144_optionStruct    Si2144_DTV_RF_AGC_SPEED_PROP_DECAY[];
extern Si2144_fieldDicoStruct Si2144_DTV_RF_AGC_SPEED_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP[];
extern Si2144_fieldDicoStruct Si2144_DTV_RF_TOP_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO[];
extern Si2144_fieldDicoStruct Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK[];
extern Si2144_optionStruct    Si2144_DTV_WB_AGC_SPEED_PROP_DECAY[];
extern Si2144_fieldDicoStruct Si2144_DTV_WB_AGC_SPEED_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE[];
extern Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE[];
extern Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO2_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE[];
extern Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO3_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_1[];
extern Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_2[];
extern Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_3[];
extern Si2144_fieldDicoStruct Si2144_TUNER_LO_INJECTION_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS[];
extern Si2144_fieldDicoStruct Si2144_TUNER_RETURN_LOSS_PROP_FIELDS[];

extern Si2144_optionStruct    Si2144_XOUT_PROP_AMP[];
extern Si2144_fieldDicoStruct Si2144_XOUT_PROP_FIELDS[];

extern Si2144_propertyInfoStruct Si2144_propertyDictionary[];

int   Si2144_PropertyNames       (L1_Si2144_Context *api, char *msg);
int   Si2144_PropertyIndex       (L1_Si2144_Context *api, const char *property);
int   Si2144_PropertyFields      (L1_Si2144_Context *api, int propIndex, char *msg);
int   Si2144_PropertyFieldIndex  (L1_Si2144_Context *api, int propIndex, char *field);
int   Si2144_PropertyFieldEnums  (L1_Si2144_Context *api, int propIndex, int fieldIndex, char *msg);
int   Si2144_GetValueFromEntry   (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, char *entry, int *value);
char *Si2144_GetEnumFromValue    (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, int iValue);
char *Si2144_GetEnumFromField    (L1_Si2144_Context *api, Si2144_fieldDicoStruct field);
int   Si2144_SetFieldFromEntry   (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, char *entry);
void  Si2144_showProperty        (L1_Si2144_Context *api, int propertyIndex);
void  Si2144_showPropertyNamed   (L1_Si2144_Context *api, char *property);
int   Si2144_showProperties      (L1_Si2144_Context *api, Si2144_featureEnum feature);
void  Si2144_setupProperty       (L1_Si2144_Context *api, int propIndex);
int   Si2144_setupProperties     (L1_Si2144_Context *api, Si2144_featureEnum feature);

#endif /* Si2144_COMMAND_LINE_APPLICATION */


int  Si2144_setupCOMMONProperties(L1_Si2144_Context *api);
int  Si2144_setupDTVProperties   (L1_Si2144_Context *api);
int  Si2144_setupTUNERProperties (L1_Si2144_Context *api);
int  Si2144_setupAllProperties   (L1_Si2144_Context *api);

#endif /* _Si2144_PROPERTIES_STRINGS_H_ */







