/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties definitions
   FILE: Si2147_User_Properties.c
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 11 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2147_COMMAND_PROTOTYPES

#include "Si2147_L1_API.h"

/***********************************************************************************************************************
  Si2147_storeUserProperties function
  Use:        property preparation function
              Used to fill the prop structure with user values.
  Parameter: *api     the Si2147 context

  Returns:    void
 ***********************************************************************************************************************/
void          Si2147_storeUserProperties    (Si2147_PropObj *prop) {
  //SiTRACE("Si2147_storeUserProperties\n");

#ifdef    Si2147_CRYSTAL_TRIM_PROP
  prop->crystal_trim.xo_cap                  = Si2147_CRYSTAL_TRIM_PROP_XO_CAP_6P7PF ; /* (default '6p7pF') */
#endif /* Si2147_CRYSTAL_TRIM_PROP */

#ifdef    Si2147_MASTER_IEN_PROP
  prop->master_ien.tunien                    = Si2147_MASTER_IEN_PROP_TUNIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.atvien                    = Si2147_MASTER_IEN_PROP_ATVIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.dtvien                    = Si2147_MASTER_IEN_PROP_DTVIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.errien                    = Si2147_MASTER_IEN_PROP_ERRIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.ctsien                    = Si2147_MASTER_IEN_PROP_CTSIEN_OFF ; /* (default 'OFF') */
#endif /* Si2147_MASTER_IEN_PROP */

#ifdef    Si2147_XOUT_PROP
  prop->xout.amp                             = Si2147_XOUT_PROP_AMP_HIGH ; /* (default 'HIGH') */
#endif /* Si2147_XOUT_PROP */

#ifdef    Si2147_DTV_AGC_FREEZE_INPUT_PROP
  prop->dtv_agc_freeze_input.level           = Si2147_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_LOW  ; /* (default 'LOW') */
  prop->dtv_agc_freeze_input.pin             = Si2147_DTV_AGC_FREEZE_INPUT_PROP_PIN_NONE   ; /* (default 'NONE') */
#endif /* Si2147_DTV_AGC_FREEZE_INPUT_PROP */

#ifdef    Si2147_DTV_AGC_SPEED_PROP
  prop->dtv_agc_speed.if_agc_speed           = Si2147_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_agc_speed.agc_decim              = Si2147_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2147_DTV_AGC_SPEED_PROP */

#ifdef    Si2147_DTV_CONFIG_IF_PORT_PROP
  prop->dtv_config_if_port.dtv_out_type      = Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF1    ; /* (default 'LIF_IF2') */
  prop->dtv_config_if_port.dtv_agc_source    = Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_AGC1_3DB; /* (default 'INTERNAL') */
#endif /* Si2147_DTV_CONFIG_IF_PORT_PROP */

#ifdef    Si2147_DTV_EXT_AGC_PROP
  prop->dtv_ext_agc.min_10mv                 =    50; /* (default    50) */
  prop->dtv_ext_agc.max_10mv                 =   100; /* (default   200) */
#endif /* Si2147_DTV_EXT_AGC_PROP */

#ifdef    Si2147_DTV_IEN_PROP
  prop->dtv_ien.chlien                       = Si2147_DTV_IEN_PROP_CHLIEN_ENABLE ; /* (default 'ENABLE') */
#endif /* Si2147_DTV_IEN_PROP */

#ifdef    Si2147_DTV_INITIAL_AGC_SPEED_PROP
  prop->dtv_initial_agc_speed.if_agc_speed   = Si2147_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
  prop->dtv_initial_agc_speed.agc_decim      = Si2147_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */
#endif /* Si2147_DTV_INITIAL_AGC_SPEED_PROP */

#ifdef    Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  prop->dtv_initial_agc_speed_period.period  =     0; /* (default     0) */
#endif /* Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */

#ifdef    Si2147_DTV_INTERNAL_ZIF_PROP
  prop->dtv_internal_zif.atsc                = Si2147_DTV_INTERNAL_ZIF_PROP_ATSC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.qam_us              = Si2147_DTV_INTERNAL_ZIF_PROP_QAM_US_ZIF ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbt                = Si2147_DTV_INTERNAL_ZIF_PROP_DVBT_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dvbc                = Si2147_DTV_INTERNAL_ZIF_PROP_DVBC_ZIF   ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbt               = Si2147_DTV_INTERNAL_ZIF_PROP_ISDBT_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.isdbc               = Si2147_DTV_INTERNAL_ZIF_PROP_ISDBC_ZIF  ; /* (default 'ZIF') */
  prop->dtv_internal_zif.dtmb                = Si2147_DTV_INTERNAL_ZIF_PROP_DTMB_ZIF   ; /* (default 'ZIF') */
#endif /* Si2147_DTV_INTERNAL_ZIF_PROP */

#ifdef    Si2147_DTV_INT_SENSE_PROP
  prop->dtv_int_sense.chlnegen               = Si2147_DTV_INT_SENSE_PROP_CHLNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->dtv_int_sense.chlposen               = Si2147_DTV_INT_SENSE_PROP_CHLPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2147_DTV_INT_SENSE_PROP */

#ifdef    Si2147_DTV_LIF_FREQ_PROP
  prop->dtv_lif_freq.offset                  =  5000; /* (default  5000) */
#endif /* Si2147_DTV_LIF_FREQ_PROP */

#ifdef    Si2147_DTV_LIF_OUT_PROP
  prop->dtv_lif_out.offset                   =   100; /* (default   148) */
  prop->dtv_lif_out.amp                      =    27; /* (default    27) */
#endif /* Si2147_DTV_LIF_OUT_PROP */

#ifdef    Si2147_DTV_MODE_PROP
  prop->dtv_mode.bw                          = Si2147_DTV_MODE_PROP_BW_BW_8MHZ              ; /* (default 'BW_8MHZ') */
  prop->dtv_mode.modulation                  = Si2147_DTV_MODE_PROP_MODULATION_DVBC         ; /* (default 'DVBT') */
  prop->dtv_mode.invert_spectrum             = Si2147_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL  ; /* (default 'NORMAL') */
#endif /* Si2147_DTV_MODE_PROP */

#ifdef    Si2147_DTV_PGA_LIMITS_PROP
  prop->dtv_pga_limits.min                   =    -1; /* (default    -1) */
  prop->dtv_pga_limits.max                   =    -1; /* (default    -1) */
#endif /* Si2147_DTV_PGA_LIMITS_PROP */

#ifdef    Si2147_DTV_PGA_TARGET_PROP
  prop->dtv_pga_target.pga_target            =     0; /* (default     0) */
  prop->dtv_pga_target.override_enable       = Si2147_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2147_DTV_PGA_TARGET_PROP */

#ifdef    Si2147_DTV_RF_TOP_PROP
  prop->dtv_rf_top.dtv_rf_top                = Si2147_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO ; /* (default 'AUTO') */
#endif /* Si2147_DTV_RF_TOP_PROP */

#ifdef    Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP
  prop->dtv_rsq_rssi_threshold.lo            =   -80; /* (default   -80) */
  prop->dtv_rsq_rssi_threshold.hi            =     0; /* (default     0) */
#endif /* Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP */

#ifdef    Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP
  prop->dtv_zif_dc_canceller_bw.bandwidth    = Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH_DEFAULT ; /* (default 'DEFAULT') */
#endif /* Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP */

#ifdef    Si2147_TUNER_BLOCKED_VCO_PROP
  prop->tuner_blocked_vco.vco_code           = 0x8000; /* (default 0x8000) */
#endif /* Si2147_TUNER_BLOCKED_VCO_PROP */

#ifdef    Si2147_TUNER_IEN_PROP
  prop->tuner_ien.tcien                      = Si2147_TUNER_IEN_PROP_TCIEN_ENABLE     ; /* (default 'ENABLE') */
  prop->tuner_ien.rssilien                   = Si2147_TUNER_IEN_PROP_RSSILIEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_ien.rssihien                   = Si2147_TUNER_IEN_PROP_RSSIHIEN_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2147_TUNER_IEN_PROP */

#ifdef    Si2147_TUNER_INT_SENSE_PROP
  prop->tuner_int_sense.tcnegen              = Si2147_TUNER_INT_SENSE_PROP_TCNEGEN_DISABLE    ; /* (default 'DISABLE') */
  prop->tuner_int_sense.rssilnegen           = Si2147_TUNER_INT_SENSE_PROP_RSSILNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_int_sense.rssihnegen           = Si2147_TUNER_INT_SENSE_PROP_RSSIHNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->tuner_int_sense.tcposen              = Si2147_TUNER_INT_SENSE_PROP_TCPOSEN_ENABLE     ; /* (default 'ENABLE') */
  prop->tuner_int_sense.rssilposen           = Si2147_TUNER_INT_SENSE_PROP_RSSILPOSEN_ENABLE  ; /* (default 'ENABLE') */
  prop->tuner_int_sense.rssihposen           = Si2147_TUNER_INT_SENSE_PROP_RSSIHPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2147_TUNER_INT_SENSE_PROP */

#ifdef    Si2147_TUNER_LO_INJECTION_PROP
  prop->tuner_lo_injection.band_1            = Si2147_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE ; /* (default 'HIGH_SIDE') */
  prop->tuner_lo_injection.band_2            = Si2147_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ; /* (default 'LOW_SIDE') */
  prop->tuner_lo_injection.band_3            = Si2147_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ; /* (default 'LOW_SIDE') */
#endif /* Si2147_TUNER_LO_INJECTION_PROP */

#ifdef    Si2147_TUNER_RETURN_LOSS_PROP
  prop->tuner_return_loss.config             = Si2147_TUNER_RETURN_LOSS_PROP_CONFIG_115         ; /* (default '127') */
  prop->tuner_return_loss.mode               = Si2147_TUNER_RETURN_LOSS_PROP_MODE_CABLE   ; /* (default 'TERRESTRIAL') */
#endif /* Si2147_TUNER_RETURN_LOSS_PROP */


}

