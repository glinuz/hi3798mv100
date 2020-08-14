/*************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
    PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES. 

   API functions prototypes used by commands and properties
   FILE: Si2144_Commands_Prototypes.h
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_0_build_2
   Revision: REVISION_NAME
   Date: November 06 2014
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    Si2144_COMMANDS_PROTOTYPES_H
#define    Si2144_COMMANDS_PROTOTYPES_H

unsigned char Si2144_CurrentResponseStatus (L1_Si2144_Context *api, unsigned char ptDataBuffer);
unsigned char Si2144_pollForResetState     (L1_Si2144_Context *api);
unsigned char Si2144_pollForCTS            (L1_Si2144_Context *api);
unsigned char Si2144_pollForResponse       (L1_Si2144_Context *api, unsigned int nbBytes, unsigned char *pByteBuffer);
unsigned char Si2144_GetStatus (L1_Si2144_Context *api);
unsigned char Si2144_L1_SendCommand2       (L1_Si2144_Context *api, unsigned int cmd_code);
unsigned char Si2144_L1_SetProperty        (L1_Si2144_Context *api, unsigned int prop_code, int  data);
unsigned char Si2144_L1_GetProperty        (L1_Si2144_Context *api, unsigned int prop_code, int *data);
unsigned char Si2144_L1_SetProperty2       (L1_Si2144_Context *api, unsigned int prop_code);
unsigned char Si2144_L1_GetProperty2       (L1_Si2144_Context *api, unsigned int prop_code);

#ifdef    Si2144_GET_PROPERTY_STRING
unsigned char Si2144_L1_GetPropertyString  (L1_Si2144_Context *api, unsigned int prop_code, const char *separator, char *msg);
unsigned char Si2144_L1_PropertyText       (Si2144_PropObj   *prop, unsigned int prop_code, const char *separator, char *msg);
#endif /* Si2144_GET_PROPERTY_STRING */

#ifdef    Si2144_GET_COMMAND_STRINGS
  unsigned char   Si2144_L1_GetCommandResponseString(L1_Si2144_Context *api, unsigned int cmd_code, const char *separator, char *msg);
#endif /* Si2144_GET_COMMAND_STRINGS */

#ifdef    Si2144_AGC_OVERRIDE_CMD
unsigned char Si2144_L1_AGC_OVERRIDE    (L1_Si2144_Context *api,
                                         unsigned char   force_max_gain,
                                         unsigned char   force_top_gain);
#endif /* Si2144_AGC_OVERRIDE_CMD */
#ifdef    Si2144_CONFIG_CLOCKS_CMD
unsigned char Si2144_L1_CONFIG_CLOCKS   (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   clock_mode,
                                         unsigned char   en_xout);
#endif /* Si2144_CONFIG_CLOCKS_CMD */
#ifdef    Si2144_CONFIG_I2C_CMD
unsigned char Si2144_L1_CONFIG_I2C      (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   i2c_broadcast);
#endif /* Si2144_CONFIG_I2C_CMD */
#ifdef    Si2144_CONFIG_PINS_CMD
unsigned char Si2144_L1_CONFIG_PINS     (L1_Si2144_Context *api,
                                         unsigned char   gpio1_mode,
                                         unsigned char   gpio1_read,
                                         unsigned char   gpio2_mode,
                                         unsigned char   gpio2_read,
                                         unsigned char   agc1_mode,
                                         unsigned char   agc1_read,
                                         unsigned char   agc2_mode,
                                         unsigned char   agc2_read,
                                         unsigned char   xout_mode);
#endif /* Si2144_CONFIG_PINS_CMD */
#ifdef    Si2144_EXIT_BOOTLOADER_CMD
unsigned char Si2144_L1_EXIT_BOOTLOADER (L1_Si2144_Context *api,
                                         unsigned char   func,
                                         unsigned char   ctsien);
#endif /* Si2144_EXIT_BOOTLOADER_CMD */
#ifdef    Si2144_FINE_TUNE_CMD
unsigned char Si2144_L1_FINE_TUNE       (L1_Si2144_Context *api,
                                         unsigned char   persistence,
                                                   int   offset_500hz);
#endif /* Si2144_FINE_TUNE_CMD */
#ifdef    Si2144_GET_PROPERTY_CMD
unsigned char Si2144_L1_GET_PROPERTY    (L1_Si2144_Context *api,
                                         unsigned char   reserved,
                                         unsigned int    prop);
#endif /* Si2144_GET_PROPERTY_CMD */
#ifdef    Si2144_GET_REV_CMD
unsigned char Si2144_L1_GET_REV         (L1_Si2144_Context *api);
#endif /* Si2144_GET_REV_CMD */
#ifdef    Si2144_PART_INFO_CMD
unsigned char Si2144_L1_PART_INFO       (L1_Si2144_Context *api);
#endif /* Si2144_PART_INFO_CMD */
#ifdef    Si2144_PART_INFO2_CMD
unsigned char Si2144_L1_PART_INFO2      (L1_Si2144_Context *api);
#endif /* Si2144_PART_INFO2_CMD */
#ifdef    Si2144_POLL_CTS_CMD
unsigned char Si2144_L1_POLL_CTS        (L1_Si2144_Context *api);
#endif /* Si2144_POLL_CTS_CMD */
#ifdef    Si2144_POWER_DOWN_HW_CMD
unsigned char Si2144_L1_POWER_DOWN_HW   (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   pdb_xo_osc,
                                         unsigned char   reserved1,
                                         unsigned char   en_xout,
                                         unsigned char   reserved2,
                                         unsigned char   reserved3,
                                         unsigned char   reserved4,
                                         unsigned char   reserved5,
                                         unsigned char   reserved6,
                                         unsigned char   reserved7,
                                         unsigned char   reserved8,
                                         unsigned char   reserved9);
#endif /* Si2144_POWER_DOWN_HW_CMD */
#ifdef    Si2144_POWER_UP_CMD
unsigned char Si2144_L1_POWER_UP        (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   clock_mode,
                                         unsigned char   en_xout,
                                         unsigned char   config_1p8v,
                                         unsigned char   reserved3,
                                         unsigned char   reserved4,
                                         unsigned char   reserved5,
                                         unsigned char   reserved6,
                                         unsigned char   reserved7,
                                         unsigned char   reserved8);
#endif /* Si2144_POWER_UP_CMD */
#ifdef    Si2144_REREAD_REPLY_CMD
unsigned char Si2144_L1_REREAD_REPLY    (L1_Si2144_Context *api,
                                         unsigned char   offset);
#endif /* Si2144_REREAD_REPLY_CMD */
#ifdef    Si2144_RESET_HW_CMD
unsigned char Si2144_L1_RESET_HW        (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   reserved1,
                                         unsigned char   reserved2);
#endif /* Si2144_RESET_HW_CMD */
#ifdef    Si2144_SET_PROPERTY_CMD
unsigned char Si2144_L1_SET_PROPERTY    (L1_Si2144_Context *api,
                                         unsigned char   reserved,
                                         unsigned int    prop,
                                         unsigned int    data);
#endif /* Si2144_SET_PROPERTY_CMD */
#ifdef    Si2144_STANDBY_CMD
unsigned char Si2144_L1_STANDBY         (L1_Si2144_Context *api,
                                         unsigned char   type);
#endif /* Si2144_STANDBY_CMD */
#ifdef    Si2144_TUNER_STATUS_CMD
unsigned char Si2144_L1_TUNER_STATUS    (L1_Si2144_Context *api);
#endif /* Si2144_TUNER_STATUS_CMD */
#ifdef    Si2144_TUNER_TUNE_FREQ_CMD
unsigned char Si2144_L1_TUNER_TUNE_FREQ (L1_Si2144_Context *api,
                                         unsigned char   mode,
                                         unsigned long   freq);
#endif /* Si2144_TUNER_TUNE_FREQ_CMD */
#ifdef    Si2144_WAKE_UP_CMD
unsigned char Si2144_L1_WAKE_UP         (L1_Si2144_Context *api,
                                         unsigned char   subcode,
                                         unsigned char   reset,
                                         unsigned char   clock_freq,
                                         unsigned char   reserved9,
                                         unsigned char   func,
                                         unsigned char   wake_up);
#endif /* Si2144_WAKE_UP_CMD */

#endif /* Si2144_COMMANDS_PROTOTYPES_H */













