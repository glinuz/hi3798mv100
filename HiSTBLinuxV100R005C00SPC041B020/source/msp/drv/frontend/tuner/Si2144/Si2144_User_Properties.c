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
  Si2144_storeUserProperties function
  Use:        property preparation function
              Used to fill the prop structure with user values.
  Parameter: *prop    a property structure to be filled

  Returns:    void
 ***********************************************************************************************************************/
void          Si2144_storeUserProperties    (Si2144_PropObj   *prop) {
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
  prop->dtv_lif_out.amp                      =    Si2144_DTV_LIF_OUT_PROP_AMP_DEFAULT; /* (default    27) */
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
