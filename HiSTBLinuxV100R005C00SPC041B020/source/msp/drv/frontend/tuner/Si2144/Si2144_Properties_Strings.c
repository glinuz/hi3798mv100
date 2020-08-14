/*************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties strings functions definitions
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
#include "Si2144_Properties_Strings.h"

#ifdef    Si2144_COMMAND_LINE_APPLICATION

/* define a local PropObj variable to use for the Property Dictionary initializer */
Si2144_PropObj Si2144_prop;
char optionText[20];

const char *Si2144_featureText[] =
{
  "COMMON",
  "DTV",
  "TUNER",
};

Si2144_optionStruct    Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2144_fieldDicoStruct Si2144_ACTIVE_LOOP_THROUGH_PROP_FIELDS[] =
{
  {"ENABLE", Si2144_UNSIGNED_CHAR, &Si2144_prop.active_loop_through.enable, sizeof(Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE)/sizeof(Si2144_optionStruct), Si2144_ACTIVE_LOOP_THROUGH_PROP_ENABLE},
};

Si2144_optionStruct    Si2144_CRYSTAL_TRIM_PROP_XO_CAP[] =
{
  {"4p7pF",      0},
  {"4p95pF",     1},
  {"5p2pF",      2},
  {"5p45pF",     3},
  {"5p7pF",      4},
  {"5p95pF",     5},
  {"6p2pF",      6},
  {"6p45pF",     7},
  {"6p7pF",      8},
  {"6p95pF",     9},
  {"7p2pF",     10},
  {"7p45pF",    11},
  {"7p7pF",     12},
  {"7p95pF",    13},
  {"8p2pF",     14},
  {"8p45pF",    15},
};
Si2144_fieldDicoStruct Si2144_CRYSTAL_TRIM_PROP_FIELDS[] =
{
  {"XO_CAP", Si2144_UNSIGNED_CHAR, &Si2144_prop.crystal_trim.xo_cap, sizeof(Si2144_CRYSTAL_TRIM_PROP_XO_CAP)/sizeof(Si2144_optionStruct), Si2144_CRYSTAL_TRIM_PROP_XO_CAP},
};

Si2144_optionStruct    Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2144_optionStruct    Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT[] =
{
  {"MIN_RANGE",     1},
  {"MAX_RANGE",   250}
};
Si2144_fieldDicoStruct Si2144_DTV_AGC_AUTO_FREEZE_PROP_FIELDS[] =
{
  {"THLD",    Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_agc_auto_freeze.thld   , sizeof(Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD   )/sizeof(Si2144_optionStruct), Si2144_DTV_AGC_AUTO_FREEZE_PROP_THLD   },
  {"TIMEOUT", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_agc_auto_freeze.timeout, sizeof(Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT)/sizeof(Si2144_optionStruct), Si2144_DTV_AGC_AUTO_FREEZE_PROP_TIMEOUT},
};

Si2144_optionStruct    Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED[] =
{
  {"AUTO",     0},
  {"39",      39},
  {"54",      54},
  {"63",      63},
  {"89",      89},
  {"105",    105},
  {"121",    121},
  {"137",    137},
  {"158",    158},
  {"172",    172},
  {"185",    185},
  {"196",    196},
  {"206",    206},
  {"216",    216},
  {"219",    219},
  {"222",    222},
};
Si2144_optionStruct    Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM[] =
{
  {"OFF",     0},
  {"2",       1},
  {"4",       2},
  {"8",       3},
};
Si2144_fieldDicoStruct Si2144_DTV_AGC_SPEED_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_agc_speed.if_agc_speed, sizeof(Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED)/sizeof(Si2144_optionStruct), Si2144_DTV_AGC_SPEED_PROP_IF_AGC_SPEED},
  {"AGC_DECIM",    Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_agc_speed.agc_decim   , sizeof(Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM   )/sizeof(Si2144_optionStruct), Si2144_DTV_AGC_SPEED_PROP_AGC_DECIM   },
};

Si2144_optionStruct    Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE[] =
{
  {"LIF_IF",         0},
  {"LIF_SE_IFA",     4},
};
Si2144_fieldDicoStruct Si2144_DTV_CONFIG_IF_PORT_PROP_FIELDS[] =
{
  {"DTV_OUT_TYPE", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_config_if_port.dtv_out_type, sizeof(Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE)/sizeof(Si2144_optionStruct), Si2144_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE},
};

Si2144_optionStruct    Si2144_DTV_EXT_AGC_PROP_MIN_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2144_optionStruct    Si2144_DTV_EXT_AGC_PROP_MAX_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2144_fieldDicoStruct Si2144_DTV_EXT_AGC_PROP_FIELDS[] =
{
  {"MIN_10MV", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_ext_agc.min_10mv, sizeof(Si2144_DTV_EXT_AGC_PROP_MIN_10MV)/sizeof(Si2144_optionStruct), Si2144_DTV_EXT_AGC_PROP_MIN_10MV},
  {"MAX_10MV", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_ext_agc.max_10mv, sizeof(Si2144_DTV_EXT_AGC_PROP_MAX_10MV)/sizeof(Si2144_optionStruct), Si2144_DTV_EXT_AGC_PROP_MAX_10MV},
};

Si2144_optionStruct    Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0}
};
Si2144_optionStruct    Si2144_DTV_IF_AGC_SPEED_PROP_DECAY[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0}
};
Si2144_fieldDicoStruct Si2144_DTV_IF_AGC_SPEED_PROP_FIELDS[] =
{
  {"ATTACK", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_if_agc_speed.attack, sizeof(Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK)/sizeof(Si2144_optionStruct), Si2144_DTV_IF_AGC_SPEED_PROP_ATTACK},
  {"DECAY",  Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_if_agc_speed.decay , sizeof(Si2144_DTV_IF_AGC_SPEED_PROP_DECAY )/sizeof(Si2144_optionStruct), Si2144_DTV_IF_AGC_SPEED_PROP_DECAY },
};

Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED[] =
{
  {"AUTO",     0},
  {"39",      39},
  {"54",      54},
  {"63",      63},
  {"89",      89},
  {"105",    105},
  {"121",    121},
  {"137",    137},
  {"158",    158},
  {"172",    172},
  {"185",    185},
  {"196",    196},
  {"206",    206},
  {"216",    216},
  {"219",    219},
  {"222",    222},
};
Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM[] =
{
  {"OFF",     0},
  {"2",       1},
  {"4",       2},
  {"8",       3},
};
Si2144_fieldDicoStruct Si2144_DTV_INITIAL_AGC_SPEED_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_initial_agc_speed.if_agc_speed, sizeof(Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED)/sizeof(Si2144_optionStruct), Si2144_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED},
  {"AGC_DECIM",    Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_initial_agc_speed.agc_decim   , sizeof(Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM   )/sizeof(Si2144_optionStruct), Si2144_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM   },
};

Si2144_optionStruct    Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE", 65535}
};
Si2144_fieldDicoStruct Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS[] =
{
  {"PERIOD", Si2144_UNSIGNED_INT, &Si2144_prop.dtv_initial_agc_speed_period.period, sizeof(Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD)/sizeof(Si2144_optionStruct), Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD},
};

Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ATSC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DVBT[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DVBC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_optionStruct    Si2144_DTV_INTERNAL_ZIF_PROP_DTMB[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2144_fieldDicoStruct Si2144_DTV_INTERNAL_ZIF_PROP_FIELDS[] =
{
  {"ATSC",   Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.atsc  , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_ATSC  )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_ATSC  },
  {"QAM_US", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.qam_us, sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US)/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_QAM_US},
  {"DVBT",   Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.dvbt  , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_DVBT  )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_DVBT  },
  {"DVBC",   Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.dvbc  , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_DVBC  )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_DVBC  },
  {"ISDBT",  Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.isdbt , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_ISDBT },
  {"ISDBC",  Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.isdbc , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_ISDBC },
  {"DTMB",   Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_internal_zif.dtmb  , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_DTMB  )/sizeof(Si2144_optionStruct), Si2144_DTV_INTERNAL_ZIF_PROP_DTMB  },
};

Si2144_optionStruct    Si2144_DTV_LIF_FREQ_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",  7000}
};
Si2144_fieldDicoStruct Si2144_DTV_LIF_FREQ_PROP_FIELDS[] =
{
  {"OFFSET", Si2144_UNSIGNED_INT, &Si2144_prop.dtv_lif_freq.offset, sizeof(Si2144_DTV_LIF_FREQ_PROP_OFFSET)/sizeof(Si2144_optionStruct), Si2144_DTV_LIF_FREQ_PROP_OFFSET},
};

Si2144_optionStruct    Si2144_DTV_LIF_OUT_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2144_optionStruct    Si2144_DTV_LIF_OUT_PROP_AMP[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2144_fieldDicoStruct Si2144_DTV_LIF_OUT_PROP_FIELDS[] =
{
  {"OFFSET", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_lif_out.offset, sizeof(Si2144_DTV_LIF_OUT_PROP_OFFSET)/sizeof(Si2144_optionStruct), Si2144_DTV_LIF_OUT_PROP_OFFSET},
  {"AMP",    Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_lif_out.amp   , sizeof(Si2144_DTV_LIF_OUT_PROP_AMP   )/sizeof(Si2144_optionStruct), Si2144_DTV_LIF_OUT_PROP_AMP   },
};

Si2144_optionStruct    Si2144_DTV_MODE_PROP_BW[] =
{
  {"BW_6MHZ",       6},
  {"BW_7MHZ",       7},
  {"BW_8MHZ",       8},
  {"BW_1P7MHZ",     9},
  {"BW_6P1MHZ",    10},
};
Si2144_optionStruct    Si2144_DTV_MODE_PROP_MODULATION[] =
{
  {"ATSC",          0},
  {"QAM_US",        1},
  {"DVBT",          2},
  {"DVBC",          3},
  {"ISDBT",         4},
  {"ISDBC",         5},
  {"DTMB",          6},
  {"CW_LEGACY",    14},
  {"CW",           15},
};
Si2144_optionStruct    Si2144_DTV_MODE_PROP_INVERT_SPECTRUM[] =
{
  {"NORMAL",       0},
  {"INVERTED",     1},
};
Si2144_fieldDicoStruct Si2144_DTV_MODE_PROP_FIELDS[] =
{
  {"BW",              Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_mode.bw             , sizeof(Si2144_DTV_MODE_PROP_BW             )/sizeof(Si2144_optionStruct), Si2144_DTV_MODE_PROP_BW             },
  {"MODULATION",      Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_mode.modulation     , sizeof(Si2144_DTV_MODE_PROP_MODULATION     )/sizeof(Si2144_optionStruct), Si2144_DTV_MODE_PROP_MODULATION     },
  {"INVERT_SPECTRUM", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_mode.invert_spectrum, sizeof(Si2144_DTV_MODE_PROP_INVERT_SPECTRUM)/sizeof(Si2144_optionStruct), Si2144_DTV_MODE_PROP_INVERT_SPECTRUM},
};

Si2144_optionStruct    Si2144_DTV_PGA_LIMITS_PROP_MIN[] =
{
  {"MIN_RANGE",    -1},
  {"MAX_RANGE",    56}
};
Si2144_optionStruct    Si2144_DTV_PGA_LIMITS_PROP_MAX[] =
{
  {"MIN_RANGE",    -1},
  {"MAX_RANGE",    56}
};
Si2144_fieldDicoStruct Si2144_DTV_PGA_LIMITS_PROP_FIELDS[] =
{
  {"MIN",   Si2144_SIGNED_CHAR, &Si2144_prop.dtv_pga_limits.min, sizeof(Si2144_DTV_PGA_LIMITS_PROP_MIN)/sizeof(Si2144_optionStruct), Si2144_DTV_PGA_LIMITS_PROP_MIN},
  {"MAX",   Si2144_SIGNED_CHAR, &Si2144_prop.dtv_pga_limits.max, sizeof(Si2144_DTV_PGA_LIMITS_PROP_MAX)/sizeof(Si2144_optionStruct), Si2144_DTV_PGA_LIMITS_PROP_MAX},
};

Si2144_optionStruct    Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET[] =
{
  {"MIN_RANGE",   -13},
  {"MAX_RANGE",     7}
};
Si2144_optionStruct    Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2144_fieldDicoStruct Si2144_DTV_PGA_TARGET_PROP_FIELDS[] =
{
  {"PGA_TARGET",        Si2144_SIGNED_CHAR, &Si2144_prop.dtv_pga_target.pga_target     , sizeof(Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET     )/sizeof(Si2144_optionStruct), Si2144_DTV_PGA_TARGET_PROP_PGA_TARGET     },
  {"OVERRIDE_ENABLE", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_pga_target.override_enable, sizeof(Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE)/sizeof(Si2144_optionStruct), Si2144_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE},
};

Si2144_optionStruct    Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK[] =
{

  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0}
};
Si2144_optionStruct    Si2144_DTV_RF_AGC_SPEED_PROP_DECAY[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0}

};
Si2144_fieldDicoStruct Si2144_DTV_RF_AGC_SPEED_PROP_FIELDS[] =
{
  {"ATTACK", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_rf_agc_speed.attack, sizeof(Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK)/sizeof(Si2144_optionStruct), Si2144_DTV_RF_AGC_SPEED_PROP_ATTACK},
  {"DECAY",  Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_rf_agc_speed.decay , sizeof(Si2144_DTV_RF_AGC_SPEED_PROP_DECAY )/sizeof(Si2144_optionStruct), Si2144_DTV_RF_AGC_SPEED_PROP_DECAY },
};

Si2144_optionStruct    Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP[] =
{
  {"AUTO",        0},
  {"P10DB",       1},
  {"P9P5DB",      2},
  {"P9DB",        3},
  {"P8P5DB",      4},
  {"P8DB",        5},
  {"P7P5DB",      6},
  {"P7DB",        7},
  {"P6P5DB",      8},
  {"P6DB",        9},
  {"P5P5DB",     10},
  {"P5DB",       11},
  {"P4P5DB",     12},
  {"P4DB",       13},
  {"P3P5DB",     14},
  {"P3DB",       15},
  {"P2P5DB",     16},
  {"P2DB",       17},
  {"P1P5DB",     18},
  {"P1DB",       19},
  {"P0P5DB",     20},
  {"0DB",        21},
  {"M0P5DB",     22},
  {"M1DB",       23},
  {"M1P5DB",     24},
  {"M2DB",       25},
  {"M2P5DB",     26},
  {"M3DB",       27},
  {"M3P5DB",     28},
  {"M4DB",       29},
  {"M4P5DB",     30},
  {"M5DB",       31},
  {"M5P5DB",     32},
  {"M6DB",       33},
  {"M6P5DB",     34},
  {"M7DB",       35},
  {"M7P5DB",     36},
  {"M8DB",       37},
  {"M8P5DB",     38},
  {"M9DB",       39},
  {"M9P5DB",     40},
  {"M10DB",      41},
  {"M10P5DB",    42},
  {"M11DB",      43},
  {"M11P5DB",    44},
  {"M12DB",      45},
  {"M12P5DB",    46},
  {"M13DB",      47},
  {"M13P5DB",    48},
  {"M14DB",      49},
  {"M14P5DB",    50},
  {"M15DB",      51},
  {"M15P5DB",    52},
  {"M16DB",      53},
  {"M16P5DB",    54},
  {"M17DB",      55},
  {"M17P5DB",    56},
  {"M18DB",      57},
  {"M18P5DB",    58},
};
Si2144_fieldDicoStruct Si2144_DTV_RF_TOP_PROP_FIELDS[] =
{
  {"DTV_RF_TOP", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_rf_top.dtv_rf_top, sizeof(Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP)/sizeof(Si2144_optionStruct), Si2144_DTV_RF_TOP_PROP_DTV_RF_TOP},
};

Si2144_optionStruct    Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2144_fieldDicoStruct Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[] =
{
  {"LO",   Si2144_SIGNED_CHAR, &Si2144_prop.dtv_rsq_rssi_threshold.lo, sizeof(Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO)/sizeof(Si2144_optionStruct), Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_LO},
};

Si2144_optionStruct    Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK[] =
{

  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0}
};
Si2144_optionStruct    Si2144_DTV_WB_AGC_SPEED_PROP_DECAY[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   231},
  {"AUTO",     0},
};
Si2144_fieldDicoStruct Si2144_DTV_WB_AGC_SPEED_PROP_FIELDS[] =
{
  {"ATTACK", Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_wb_agc_speed.attack, sizeof(Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK)/sizeof(Si2144_optionStruct), Si2144_DTV_WB_AGC_SPEED_PROP_ATTACK},
  {"DECAY",  Si2144_UNSIGNED_CHAR, &Si2144_prop.dtv_wb_agc_speed.decay , sizeof(Si2144_DTV_WB_AGC_SPEED_PROP_DECAY )/sizeof(Si2144_optionStruct), Si2144_DTV_WB_AGC_SPEED_PROP_DECAY },
};

Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE[] =
{
  {"MIN_RANGE", -32768},
  {"MAX_RANGE", 32767}
};
Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO_PROP_FIELDS[] =
{
  {"VCO_CODE",   Si2144_SIGNED_INT, &Si2144_prop.tuner_blocked_vco.vco_code, sizeof(Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE)/sizeof(Si2144_optionStruct), Si2144_TUNER_BLOCKED_VCO_PROP_VCO_CODE},
};

Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE[] =
{
  {"MIN_RANGE", -32768},
  {"MAX_RANGE", 32767}
};
Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO2_PROP_FIELDS[] =
{
  {"VCO_CODE",   Si2144_SIGNED_INT, &Si2144_prop.tuner_blocked_vco2.vco_code, sizeof(Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE)/sizeof(Si2144_optionStruct), Si2144_TUNER_BLOCKED_VCO2_PROP_VCO_CODE},
};

Si2144_optionStruct    Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE[] =
{
  {"MIN_RANGE", -32768},
  {"MAX_RANGE", 32767}
};
Si2144_fieldDicoStruct Si2144_TUNER_BLOCKED_VCO3_PROP_FIELDS[] =
{
  {"VCO_CODE",   Si2144_SIGNED_INT, &Si2144_prop.tuner_blocked_vco3.vco_code, sizeof(Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE)/sizeof(Si2144_optionStruct), Si2144_TUNER_BLOCKED_VCO3_PROP_VCO_CODE},
};

Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_1[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_2[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2144_optionStruct    Si2144_TUNER_LO_INJECTION_PROP_BAND_3[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2144_fieldDicoStruct Si2144_TUNER_LO_INJECTION_PROP_FIELDS[] =
{
  {"BAND_1", Si2144_UNSIGNED_CHAR, &Si2144_prop.tuner_lo_injection.band_1, sizeof(Si2144_TUNER_LO_INJECTION_PROP_BAND_1)/sizeof(Si2144_optionStruct), Si2144_TUNER_LO_INJECTION_PROP_BAND_1},
  {"BAND_2", Si2144_UNSIGNED_CHAR, &Si2144_prop.tuner_lo_injection.band_2, sizeof(Si2144_TUNER_LO_INJECTION_PROP_BAND_2)/sizeof(Si2144_optionStruct), Si2144_TUNER_LO_INJECTION_PROP_BAND_2},
  {"BAND_3", Si2144_UNSIGNED_CHAR, &Si2144_prop.tuner_lo_injection.band_3, sizeof(Si2144_TUNER_LO_INJECTION_PROP_BAND_3)/sizeof(Si2144_optionStruct), Si2144_TUNER_LO_INJECTION_PROP_BAND_3},
};

Si2144_optionStruct    Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS[] =
{
  {"RETURNLOSS_5DB",     112},
  {"RETURNLOSS_5P5DB",    96},
  {"RETURNLOSS_6DB",      64},
  {"RETURNLOSS_6P5DB",    48},
  {"RETURNLOSS_7DB",      85},
  {"RETURNLOSS_7P5DB",    69},
  {"RETURNLOSS_8DB",      90},
  {"RETURNLOSS_8P5DB",    74},
  {"RETURNLOSS_9DB",      58},
  {"RETURNLOSS_9P5DB",    21},
  {"RETURNLOSS_10DB",     42},
};
Si2144_fieldDicoStruct Si2144_TUNER_RETURN_LOSS_PROP_FIELDS[] =
{
  {"RETURNLOSS", Si2144_UNSIGNED_CHAR, &Si2144_prop.tuner_return_loss.returnloss, sizeof(Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS)/sizeof(Si2144_optionStruct), Si2144_TUNER_RETURN_LOSS_PROP_RETURNLOSS},
};

Si2144_optionStruct    Si2144_XOUT_PROP_AMP[] =
{
  {"HIGH",     0},
  {"LOW",      1},
};
Si2144_fieldDicoStruct Si2144_XOUT_PROP_FIELDS[] =
{
  {"AMP", Si2144_UNSIGNED_CHAR, &Si2144_prop.xout.amp, sizeof(Si2144_XOUT_PROP_AMP)/sizeof(Si2144_optionStruct), Si2144_XOUT_PROP_AMP},
};

Si2144_propertyInfoStruct Si2144_propertyDictionary[] =
{
  {Si2144_COMMON, "CRYSTAL_TRIM"                , sizeof(Si2144_CRYSTAL_TRIM_PROP_FIELDS                )/sizeof(Si2144_fieldDicoStruct), Si2144_CRYSTAL_TRIM_PROP_FIELDS                 , Si2144_CRYSTAL_TRIM_PROP                 },
  {Si2144_COMMON, "XOUT"                        , sizeof(Si2144_XOUT_PROP_FIELDS                        )/sizeof(Si2144_fieldDicoStruct), Si2144_XOUT_PROP_FIELDS                         , Si2144_XOUT_PROP                         },
  {Si2144_DTV   , "DTV_AGC_AUTO_FREEZE"         , sizeof(Si2144_DTV_AGC_AUTO_FREEZE_PROP_FIELDS         )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_AGC_AUTO_FREEZE_PROP_FIELDS          , Si2144_DTV_AGC_AUTO_FREEZE_PROP          },
  {Si2144_DTV   , "DTV_AGC_SPEED"               , sizeof(Si2144_DTV_AGC_SPEED_PROP_FIELDS               )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_AGC_SPEED_PROP_FIELDS                , Si2144_DTV_AGC_SPEED_PROP                },
  {Si2144_DTV   , "DTV_CONFIG_IF_PORT"          , sizeof(Si2144_DTV_CONFIG_IF_PORT_PROP_FIELDS          )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_CONFIG_IF_PORT_PROP_FIELDS           , Si2144_DTV_CONFIG_IF_PORT_PROP           },
  {Si2144_DTV   , "DTV_EXT_AGC"                 , sizeof(Si2144_DTV_EXT_AGC_PROP_FIELDS                 )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_EXT_AGC_PROP_FIELDS                  , Si2144_DTV_EXT_AGC_PROP                  },
  {Si2144_DTV   , "DTV_IF_AGC_SPEED"            , sizeof(Si2144_DTV_IF_AGC_SPEED_PROP_FIELDS            )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_IF_AGC_SPEED_PROP_FIELDS             , Si2144_DTV_IF_AGC_SPEED_PROP             },
  {Si2144_DTV   , "DTV_INITIAL_AGC_SPEED"       , sizeof(Si2144_DTV_INITIAL_AGC_SPEED_PROP_FIELDS       )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_INITIAL_AGC_SPEED_PROP_FIELDS        , Si2144_DTV_INITIAL_AGC_SPEED_PROP        },
  {Si2144_DTV   , "DTV_INITIAL_AGC_SPEED_PERIOD", sizeof(Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS)/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS , Si2144_DTV_INITIAL_AGC_SPEED_PERIOD_PROP },
  {Si2144_DTV   , "DTV_INTERNAL_ZIF"            , sizeof(Si2144_DTV_INTERNAL_ZIF_PROP_FIELDS            )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_INTERNAL_ZIF_PROP_FIELDS             , Si2144_DTV_INTERNAL_ZIF_PROP             },
  {Si2144_DTV   , "DTV_LIF_FREQ"                , sizeof(Si2144_DTV_LIF_FREQ_PROP_FIELDS                )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_LIF_FREQ_PROP_FIELDS                 , Si2144_DTV_LIF_FREQ_PROP                 },
  {Si2144_DTV   , "DTV_LIF_OUT"                 , sizeof(Si2144_DTV_LIF_OUT_PROP_FIELDS                 )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_LIF_OUT_PROP_FIELDS                  , Si2144_DTV_LIF_OUT_PROP                  },
  {Si2144_DTV   , "DTV_MODE"                    , sizeof(Si2144_DTV_MODE_PROP_FIELDS                    )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_MODE_PROP_FIELDS                     , Si2144_DTV_MODE_PROP                     },
  {Si2144_DTV   , "DTV_PGA_LIMITS"              , sizeof(Si2144_DTV_PGA_LIMITS_PROP_FIELDS              )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_PGA_LIMITS_PROP_FIELDS               , Si2144_DTV_PGA_LIMITS_PROP               },
  {Si2144_DTV   , "DTV_PGA_TARGET"              , sizeof(Si2144_DTV_PGA_TARGET_PROP_FIELDS              )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_PGA_TARGET_PROP_FIELDS               , Si2144_DTV_PGA_TARGET_PROP               },
  {Si2144_DTV   , "DTV_RF_AGC_SPEED"            , sizeof(Si2144_DTV_RF_AGC_SPEED_PROP_FIELDS            )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_RF_AGC_SPEED_PROP_FIELDS             , Si2144_DTV_RF_AGC_SPEED_PROP             },
  {Si2144_DTV   , "DTV_RF_TOP"                  , sizeof(Si2144_DTV_RF_TOP_PROP_FIELDS                  )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_RF_TOP_PROP_FIELDS                   , Si2144_DTV_RF_TOP_PROP                   },
  {Si2144_DTV   , "DTV_RSQ_RSSI_THRESHOLD"      , sizeof(Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS      )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS       , Si2144_DTV_RSQ_RSSI_THRESHOLD_PROP       },
  {Si2144_DTV   , "DTV_WB_AGC_SPEED"            , sizeof(Si2144_DTV_WB_AGC_SPEED_PROP_FIELDS            )/sizeof(Si2144_fieldDicoStruct), Si2144_DTV_WB_AGC_SPEED_PROP_FIELDS             , Si2144_DTV_WB_AGC_SPEED_PROP             },
  {Si2144_TUNER , "ACTIVE_LOOP_THROUGH"         , sizeof(Si2144_ACTIVE_LOOP_THROUGH_PROP_FIELDS         )/sizeof(Si2144_fieldDicoStruct), Si2144_ACTIVE_LOOP_THROUGH_PROP_FIELDS          , Si2144_ACTIVE_LOOP_THROUGH_PROP          },
  {Si2144_TUNER , "TUNER_BLOCKED_VCO"           , sizeof(Si2144_TUNER_BLOCKED_VCO_PROP_FIELDS           )/sizeof(Si2144_fieldDicoStruct), Si2144_TUNER_BLOCKED_VCO_PROP_FIELDS            , Si2144_TUNER_BLOCKED_VCO_PROP            },
  {Si2144_TUNER , "TUNER_BLOCKED_VCO2"          , sizeof(Si2144_TUNER_BLOCKED_VCO2_PROP_FIELDS          )/sizeof(Si2144_fieldDicoStruct), Si2144_TUNER_BLOCKED_VCO2_PROP_FIELDS           , Si2144_TUNER_BLOCKED_VCO2_PROP           },
  {Si2144_TUNER , "TUNER_BLOCKED_VCO3"          , sizeof(Si2144_TUNER_BLOCKED_VCO3_PROP_FIELDS          )/sizeof(Si2144_fieldDicoStruct), Si2144_TUNER_BLOCKED_VCO3_PROP_FIELDS           , Si2144_TUNER_BLOCKED_VCO3_PROP           },
  {Si2144_TUNER , "TUNER_LO_INJECTION"          , sizeof(Si2144_TUNER_LO_INJECTION_PROP_FIELDS          )/sizeof(Si2144_fieldDicoStruct), Si2144_TUNER_LO_INJECTION_PROP_FIELDS           , Si2144_TUNER_LO_INJECTION_PROP           },
  {Si2144_TUNER , "TUNER_RETURN_LOSS"           , sizeof(Si2144_TUNER_RETURN_LOSS_PROP_FIELDS           )/sizeof(Si2144_fieldDicoStruct), Si2144_TUNER_RETURN_LOSS_PROP_FIELDS            , Si2144_TUNER_RETURN_LOSS_PROP            },
};

int   Si2144_PropertyNames        (L1_Si2144_Context *api, char *msg)
{
  int propIndex;
  int size;
  api = api; /* to avoid 'unused parameter' warning */
  size = sizeof(Si2144_propertyDictionary)/sizeof(Si2144_propertyInfoStruct);
  msg[0]='\0';
  for (propIndex=0; propIndex<size; propIndex++)
  {
    strcat(msg,Si2144_propertyDictionary[propIndex].name);
    if (propIndex< (size-1)) strcat(msg," ");
  }
  return propIndex;
};

int   Si2144_PropertyIndex        (L1_Si2144_Context *api, const char *property)
{
  int propIndex;
  int size;
  api = api; /* to avoid 'unused parameter' warning */
  size = sizeof(Si2144_propertyDictionary)/sizeof(Si2144_propertyInfoStruct);
  for (propIndex=0; propIndex <size; propIndex++)
  {
    if (Si2144_strcmp_nocase(property, Si2144_propertyDictionary[propIndex].name)==0)
    {
      return propIndex;
    }
  }
  return -1;
};

int   Si2144_PropertyFields       (L1_Si2144_Context *api, int propIndex, char *msg)
{
  int fieldIndex;
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (fieldIndex=0; fieldIndex < Si2144_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    strcat(msg, Si2144_propertyDictionary[propIndex].field[fieldIndex].name);
    if (fieldIndex< (Si2144_propertyDictionary[propIndex].nbFields-1)) strcat(msg," ");
  }
  return -1;
};

int   Si2144_PropertyFieldIndex   (L1_Si2144_Context *api, int propIndex, char *field)
{
  int fieldIndex;
  api = api; /* to avoid 'unused parameter' warning */
  for (fieldIndex=0; fieldIndex < Si2144_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    if (Si2144_strcmp_nocase(field, Si2144_propertyDictionary[propIndex].field[fieldIndex].name)==0)
    {
      return fieldIndex;
    }
  }
  return -1;
};

int   Si2144_PropertyFieldEnums   (L1_Si2144_Context *api, int propIndex, int fieldIndex, char *msg)
{
  int enumIndex;
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (enumIndex=0; enumIndex < Si2144_propertyDictionary[propIndex].field[fieldIndex].nbOptions; enumIndex++)
  {
    strcat(msg, Si2144_propertyDictionary[propIndex].field[fieldIndex].option[enumIndex].name);
    if (enumIndex < (Si2144_propertyDictionary[propIndex].field[fieldIndex].nbOptions-1)) strcat(msg," ");
  }
  return enumIndex;
};

int   Si2144_PropertyFieldRanges  (L1_Si2144_Context *api, int propIndex, int fieldIndex, char *msg)
{
  int index;
  char buf[10];
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (index=0; index < 2; index++)
  {
    itoa(Si2144_propertyDictionary[propIndex].field[fieldIndex].option[index].value,buf,10);
    strcat(msg, buf);
    if (index < 1) strcat(msg," - ");
  }
  return index;
};

int   Si2144_GetValueFromEntry    (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, char *entry, int *value)
{
  int optionIndex;
  api = api; /* to avoid 'unused parameter' warning */

  if (!Si2144_strcmp_nocase(field.option[0].name,"MIN_RANGE"))
  {
    if (field.datatype== Si2144_UNSIGNED_INT || field.datatype== Si2144_UNSIGNED_CHAR)
    {
      *value = (unsigned int)atoi(entry);
      return 1;
    }
    else if (field.datatype == Si2144_SIGNED_INT || field.datatype == Si2144_SIGNED_CHAR)
    {
      *value = (int)atoi(entry);
      return 1;
    }
  }
  for (optionIndex=0; optionIndex < field.nbOptions; optionIndex++)
  {
    if (Si2144_strcmp_nocase(entry, field.option[optionIndex].name     )==0)
    {
      *value = field.option[optionIndex].value;
      return 1;
    }
  }
  return 0;
};

char *Si2144_GetEnumFromValue     (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, int iValue)
{
  int optionIndex;
  api = api; /* to avoid 'unused parameter' warning */
  if (field.nbOptions ==0 )
    {
    snprintf(optionText, "%d", iValue);
    return optionText;
  }
  for (optionIndex=0; optionIndex < field.nbOptions; optionIndex++)
  {
    if (iValue == field.option[optionIndex].value)
    {
      return (char*)field.option[optionIndex].name;
    }
  }
  return (char*)NULL;
};

int   Si2144_GetValueFromField    (L1_Si2144_Context *api, Si2144_fieldDicoStruct field)
{
  short intValue;
  short *fieldAsInt;
  short iTmp=0;
  fieldAsInt = &intValue;

  if (field.datatype == Si2144_SIGNED_INT   )
  {
      fieldAsInt= (short *) ((field.pField-(void *) &Si2144_prop)+ (void *)api->prop);
  }
  else if (field.datatype == Si2144_UNSIGNED_CHAR)
  {
      iTmp = *((unsigned char *) ((field.pField-(void *) &Si2144_prop)+ (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else if (field.datatype == Si2144_SIGNED_CHAR)
  {
      iTmp= *((char *) ((field.pField-(void *) &Si2144_prop)+ (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else /* if (field.datatype == Si2144_UNSIGNED_INT) */
  {
      fieldAsInt = (unsigned short *) ((field.pField-(void *) &Si2144_prop)+ (void *)api->prop);
  }

  return  *fieldAsInt;
};

char *Si2144_GetEnumFromField     (L1_Si2144_Context *api, Si2144_fieldDicoStruct field)
{
  int intValue;
  int *fieldAsInt;
  int iTmp=0;
  fieldAsInt = &intValue;

  if (field.datatype == Si2144_SIGNED_INT   )
  {
      fieldAsInt= (int *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop);
  }
  else if (field.datatype == Si2144_UNSIGNED_CHAR)
  {
      iTmp = *((unsigned char *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else if (field.datatype == Si2144_SIGNED_CHAR)
  {
      iTmp= *((char *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else /* if (field.datatype == Si2144_UNSIGNED_INT) */
  {
      fieldAsInt = (unsigned int *)((field.pField-(void *) &Si2144_prop) + (void *)api->prop);
  }

  return Si2144_GetEnumFromValue(api, field, *fieldAsInt);
};

int   Si2144_SetFieldFromEntry    (L1_Si2144_Context *api, Si2144_fieldDicoStruct field, char *entry)
{
  int *fieldAsInt;
  int iTmp=0;
  fieldAsInt = &iTmp;
  if (Si2144_GetValueFromEntry(api, field, entry, fieldAsInt) == 0) return 0;
  if (field.datatype == Si2144_SIGNED_INT )
  {
    *( (int *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop))=iTmp;
  }
  else if (field.datatype == Si2144_UNSIGNED_CHAR)
  {
    *( (unsigned char *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop))=iTmp;
  }
  else if (field.datatype == Si2144_SIGNED_CHAR)
  {
    *( (char *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop))=iTmp;
  }
  else /* if (field.datatype == Si2144_UNSIGNED_INT) */
  {
    *( (unsigned int *) ((field.pField-(void *) &Si2144_prop) + (void *)api->prop))=iTmp;
  }

  return 1;
};

void  Si2144_showProperty         (L1_Si2144_Context *api, int propIndex)
{
  int fieldIndex;
  for (fieldIndex=0; fieldIndex<Si2144_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    HI_INFO_TUNER("%s:%s:%s = ", Si2144_featureText[Si2144_propertyDictionary[propIndex].feature], Si2144_propertyDictionary[propIndex].name, Si2144_propertyDictionary[propIndex].field[fieldIndex].name);
    HI_INFO_TUNER("<%s>\n", Si2144_GetEnumFromField(api, Si2144_propertyDictionary[propIndex].field[fieldIndex]));
  }

}

void  Si2144_showPropertyNamed    (L1_Si2144_Context *api, char *property)
{
  int propIndex;

  if ((propIndex = Si2144_PropertyIndex(api, property)) >= 0)
  {
    Si2144_showProperty(api, propIndex);
  }
  else
  {
    HI_INFO_TUNER("Unknown property '%s'\n", property);
  }

}

void  Si2144_setupProperty        (L1_Si2144_Context *api, int propIndex)
{
  #define LMAX 20
  char entry[LMAX];
  char setup_msg[1000];
  int fieldIndex;
  char isRangeType = 1;

  for (fieldIndex=0; fieldIndex<Si2144_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    isRangeType = (Si2144_strcmp_nocase(Si2144_propertyDictionary[propIndex].field[fieldIndex].option->name,"MIN_RANGE") == 0);

    HI_INFO_TUNER("%s:%s:%s = ", Si2144_featureText[Si2144_propertyDictionary[propIndex].feature], Si2144_propertyDictionary[propIndex].name, Si2144_propertyDictionary[propIndex].field[fieldIndex].name);

    if ( isRangeType )
    {
      HI_INFO_TUNER("<%d>\n", Si2144_GetValueFromField(api, Si2144_propertyDictionary[propIndex].field[fieldIndex]));
      Si2144_PropertyFieldRanges(api, propIndex, fieldIndex, setup_msg);
      HI_INFO_TUNER("Range: <%s>  ",setup_msg);
    }
    else
    {
      Si2144_PropertyFieldEnums(api, propIndex, fieldIndex, setup_msg);
      HI_INFO_TUNER("<%s>\n Options: <%s>\n", Si2144_GetEnumFromField(api, Si2144_propertyDictionary[propIndex].field[fieldIndex]), setup_msg);
    }

    fgets(entry,LMAX,stdin);

    if (strlen(entry) > 1)
    {
      entry[strlen(entry)-1]='\0';
      if (!Si2144_SetFieldFromEntry(api, Si2144_propertyDictionary[propIndex].field[fieldIndex], entry))
      {
        HI_INFO_TUNER("Invalid entry!\n");
      }
    }

    if (isRangeType)
      HI_INFO_TUNER("<%d>\n\n", Si2144_GetValueFromField(api, Si2144_propertyDictionary[propIndex].field[fieldIndex]));
    else
      HI_INFO_TUNER("<%s>\n\n", Si2144_GetEnumFromField(api, Si2144_propertyDictionary[propIndex].field[fieldIndex]));
  }
}

int   Si2144_setupProperties      (L1_Si2144_Context *api, Si2144_featureEnum feature)
{
  int propertyIndex;
  int dictSize;

  dictSize=sizeof(Si2144_propertyDictionary)/sizeof(Si2144_propertyInfoStruct);

  HI_INFO_TUNER("\n%s properties:\n\n", Si2144_featureText[feature]);

  for (propertyIndex=0; propertyIndex<dictSize; propertyIndex++)
  {
    if (Si2144_propertyDictionary[propertyIndex].feature != feature) continue;
    Si2144_setupProperty(api, propertyIndex);
  }
  return 0;
}

int Si2144_setupCOMMONProperties    (L1_Si2144_Context *api)
{
  SiTRACE("Si2144_setupCOMMONProperties    \n");
  Si2144_setupProperties(api, Si2144_COMMON);
  return 0;
}

int Si2144_setupDTVProperties       (L1_Si2144_Context *api)
{
  SiTRACE("Si2144_setupDTVProperties       \n");
  Si2144_setupProperties(api, Si2144_DTV);
  return 0;
}

int Si2144_setupTUNERProperties     (L1_Si2144_Context *api)
{
  SiTRACE("Si2144_setupTUNERProperties     \n");
  Si2144_setupProperties(api, Si2144_TUNER);
  return 0;
}

int Si2144_setupAllProperties  (L1_Si2144_Context *api)
{
  Si2144_setupCOMMONProperties    (api);
  Si2144_setupDTVProperties       (api);
  Si2144_setupTUNERProperties     (api);
  return 0;
}


#endif /* Si2144_COMMAND_LINE_APPLICATION */







