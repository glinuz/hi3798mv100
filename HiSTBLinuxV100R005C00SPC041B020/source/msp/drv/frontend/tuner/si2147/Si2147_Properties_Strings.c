/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties strings functions definitions
   FILE: Si2147_L1_Properties.c
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2147_COMMAND_PROTOTYPES

#include "hi_debug.h"
#include "Si2147_L1_API.h"
#include "Si2147_Properties_Strings.h"

#ifdef Si2147_COMMAND_LINE_APPLICATION

/* define a local PropObj variable to use for the Property Dictionary initializer */
Si2147_PropObj Si2147_prop;
char optionText[20];

const char *Si2147_featureText[] =
{
  "ATV",
  "COMMON",
  "DTV",
  "TUNER",
};

Si2147_optionStruct    Si2147_ATV_AFC_RANGE_PROP_RANGE_KHZ[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE", 65535}
};
Si2147_fieldDicoStruct Si2147_ATV_AFC_RANGE_PROP_FIELDS[] =
{
  {"RANGE_KHZ", Si2147_UNSIGNED_INT, &Si2147_prop.atv_afc_range.range_khz, sizeof(Si2147_ATV_AFC_RANGE_PROP_RANGE_KHZ)/sizeof(Si2147_optionStruct), Si2147_ATV_AFC_RANGE_PROP_RANGE_KHZ},
};

Si2147_optionStruct    Si2147_ATV_AGC_SPEED_PROP_IF_AGC_SPEED[] =
{
  {"AUTO",       0},
  {"89",        89},
  {"105",      105},
  {"121",      121},
  {"137",      137},
  {"158",      158},
  {"172",      172},
  {"178",      178},
  {"185",      185},
  {"196",      196},
  {"206",      206},
  {"216",      216},
  {"219",      219},
  {"222",      222},
  {"223",      223},
  {"248",      248},
  {"250",      250},
  {"251",      251},
  {"CUSTOM",     1},
};
Si2147_fieldDicoStruct Si2147_ATV_AGC_SPEED_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_agc_speed.if_agc_speed, sizeof(Si2147_ATV_AGC_SPEED_PROP_IF_AGC_SPEED)/sizeof(Si2147_optionStruct), Si2147_ATV_AGC_SPEED_PROP_IF_AGC_SPEED},
};

Si2147_optionStruct    Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED[] =
{
  {"89",        89},
  {"105",      105},
  {"121",      121},
  {"137",      137},
  {"158",      158},
  {"172",      172},
  {"178",      178},
  {"185",      185},
  {"196",      196},
  {"206",      206},
  {"216",      216},
  {"219",      219},
  {"222",      222},
  {"223",      223},
  {"248",      248},
  {"250",      250},
  {"251",      251},
  {"CUSTOM",     1},
};
Si2147_optionStruct    Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_agc_speed_low_rssi.if_agc_speed, sizeof(Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED)/sizeof(Si2147_optionStruct), Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_IF_AGC_SPEED},
  {"THLD",           Si2147_SIGNED_CHAR, &Si2147_prop.atv_agc_speed_low_rssi.thld        , sizeof(Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD        )/sizeof(Si2147_optionStruct), Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_THLD        },
};

Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_GAIN[] =
{
  {"AUTO",     0},
  {"0DB",      1},
  {"6DB",      2},
  {"12DB",     3},
  {"18DB",     4},
  {"24DB",     5},
  {"30DB",     6},
  {"36DB",     7},
  {"42DB",     8},
  {"OFF",      9},
};
Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_PERIOD[] =
{
  {"LONG",      0},
  {"SHORT",     1},
};
Si2147_optionStruct    Si2147_ATV_ARTIFICIAL_SNOW_PROP_OFFSET[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_ATV_ARTIFICIAL_SNOW_PROP_FIELDS[] =
{
  {"GAIN",   Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_artificial_snow.gain  , sizeof(Si2147_ATV_ARTIFICIAL_SNOW_PROP_GAIN  )/sizeof(Si2147_optionStruct), Si2147_ATV_ARTIFICIAL_SNOW_PROP_GAIN  },
  {"PERIOD", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_artificial_snow.period, sizeof(Si2147_ATV_ARTIFICIAL_SNOW_PROP_PERIOD)/sizeof(Si2147_optionStruct), Si2147_ATV_ARTIFICIAL_SNOW_PROP_PERIOD},
  {"OFFSET",   Si2147_SIGNED_CHAR, &Si2147_prop.atv_artificial_snow.offset, sizeof(Si2147_ATV_ARTIFICIAL_SNOW_PROP_OFFSET)/sizeof(Si2147_optionStruct), Si2147_ATV_ARTIFICIAL_SNOW_PROP_OFFSET},
};

Si2147_optionStruct    Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE[] =
{
  {"LIF_DIFF_IF1",     8},
  {"LIF_DIFF_IF2",    10},
  {"LIF_SE_IF1A",     12},
  {"LIF_SE_IF2A",     14},
};
Si2147_optionStruct    Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE[] =
{
  {"INTERNAL",     0},
  {"AGC1_3DB",     1},
  {"AGC2_3DB",     2},
};
Si2147_fieldDicoStruct Si2147_ATV_CONFIG_IF_PORT_PROP_FIELDS[] =
{
  {"ATV_OUT_TYPE",   Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_config_if_port.atv_out_type  , sizeof(Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE  )/sizeof(Si2147_optionStruct), Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE  },
  {"ATV_AGC_SOURCE", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_config_if_port.atv_agc_source, sizeof(Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE)/sizeof(Si2147_optionStruct), Si2147_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE},
};

Si2147_optionStruct    Si2147_ATV_EXT_AGC_PROP_MIN_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_optionStruct    Si2147_ATV_EXT_AGC_PROP_MAX_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_fieldDicoStruct Si2147_ATV_EXT_AGC_PROP_FIELDS[] =
{
  {"MIN_10MV", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_ext_agc.min_10mv, sizeof(Si2147_ATV_EXT_AGC_PROP_MIN_10MV)/sizeof(Si2147_optionStruct), Si2147_ATV_EXT_AGC_PROP_MIN_10MV},
  {"MAX_10MV", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_ext_agc.max_10mv, sizeof(Si2147_ATV_EXT_AGC_PROP_MAX_10MV)/sizeof(Si2147_optionStruct), Si2147_ATV_EXT_AGC_PROP_MAX_10MV},
};

Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_GPIO_SEL[] =
{
  {"NONE",      0},
  {"GPIO1",     1},
  {"GPIO2",     2},
};
Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_WIDTH[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",    51}
};
Si2147_optionStruct    Si2147_ATV_HSYNC_OUT_PROP_OFFSET[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_ATV_HSYNC_OUT_PROP_FIELDS[] =
{
  {"GPIO_SEL", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_hsync_out.gpio_sel, sizeof(Si2147_ATV_HSYNC_OUT_PROP_GPIO_SEL)/sizeof(Si2147_optionStruct), Si2147_ATV_HSYNC_OUT_PROP_GPIO_SEL},
  {"WIDTH",    Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_hsync_out.width   , sizeof(Si2147_ATV_HSYNC_OUT_PROP_WIDTH   )/sizeof(Si2147_optionStruct), Si2147_ATV_HSYNC_OUT_PROP_WIDTH   },
  {"OFFSET",     Si2147_SIGNED_CHAR, &Si2147_prop.atv_hsync_out.offset  , sizeof(Si2147_ATV_HSYNC_OUT_PROP_OFFSET  )/sizeof(Si2147_optionStruct), Si2147_ATV_HSYNC_OUT_PROP_OFFSET  },
};

Si2147_optionStruct    Si2147_ATV_IEN_PROP_CHLIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_ATV_IEN_PROP_PCLIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_ATV_IEN_PROP_FIELDS[] =
{
  {"CHLIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_ien.chlien, sizeof(Si2147_ATV_IEN_PROP_CHLIEN)/sizeof(Si2147_optionStruct), Si2147_ATV_IEN_PROP_CHLIEN},
  {"PCLIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_ien.pclien, sizeof(Si2147_ATV_IEN_PROP_PCLIEN)/sizeof(Si2147_optionStruct), Si2147_ATV_IEN_PROP_PCLIEN},
};

Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_CHLNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_PCLNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_CHLPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_ATV_INT_SENSE_PROP_PCLPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_ATV_INT_SENSE_PROP_FIELDS[] =
{
  {"CHLNEGEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_int_sense.chlnegen, sizeof(Si2147_ATV_INT_SENSE_PROP_CHLNEGEN)/sizeof(Si2147_optionStruct), Si2147_ATV_INT_SENSE_PROP_CHLNEGEN},
  {"PCLNEGEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_int_sense.pclnegen, sizeof(Si2147_ATV_INT_SENSE_PROP_PCLNEGEN)/sizeof(Si2147_optionStruct), Si2147_ATV_INT_SENSE_PROP_PCLNEGEN},
  {"CHLPOSEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_int_sense.chlposen, sizeof(Si2147_ATV_INT_SENSE_PROP_CHLPOSEN)/sizeof(Si2147_optionStruct), Si2147_ATV_INT_SENSE_PROP_CHLPOSEN},
  {"PCLPOSEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_int_sense.pclposen, sizeof(Si2147_ATV_INT_SENSE_PROP_PCLPOSEN)/sizeof(Si2147_optionStruct), Si2147_ATV_INT_SENSE_PROP_PCLPOSEN},
};

Si2147_optionStruct    Si2147_ATV_LIF_FREQ_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",  7000}
};
Si2147_fieldDicoStruct Si2147_ATV_LIF_FREQ_PROP_FIELDS[] =
{
  {"OFFSET", Si2147_UNSIGNED_INT, &Si2147_prop.atv_lif_freq.offset, sizeof(Si2147_ATV_LIF_FREQ_PROP_OFFSET)/sizeof(Si2147_optionStruct), Si2147_ATV_LIF_FREQ_PROP_OFFSET},
};

Si2147_optionStruct    Si2147_ATV_LIF_OUT_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_optionStruct    Si2147_ATV_LIF_OUT_PROP_AMP[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_fieldDicoStruct Si2147_ATV_LIF_OUT_PROP_FIELDS[] =
{
  {"OFFSET", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_lif_out.offset, sizeof(Si2147_ATV_LIF_OUT_PROP_OFFSET)/sizeof(Si2147_optionStruct), Si2147_ATV_LIF_OUT_PROP_OFFSET},
  {"AMP",    Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_lif_out.amp   , sizeof(Si2147_ATV_LIF_OUT_PROP_AMP   )/sizeof(Si2147_optionStruct), Si2147_ATV_LIF_OUT_PROP_AMP   },
};

Si2147_optionStruct    Si2147_ATV_PGA_TARGET_PROP_PGA_TARGET[] =
{
  {"MIN_RANGE",   -13},
  {"MAX_RANGE",     7}
};
Si2147_optionStruct    Si2147_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_ATV_PGA_TARGET_PROP_FIELDS[] =
{
  {"PGA_TARGET",        Si2147_SIGNED_CHAR, &Si2147_prop.atv_pga_target.pga_target     , sizeof(Si2147_ATV_PGA_TARGET_PROP_PGA_TARGET     )/sizeof(Si2147_optionStruct), Si2147_ATV_PGA_TARGET_PROP_PGA_TARGET     },
  {"OVERRIDE_ENABLE", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_pga_target.override_enable, sizeof(Si2147_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE)/sizeof(Si2147_optionStruct), Si2147_ATV_PGA_TARGET_PROP_OVERRIDE_ENABLE},
};

Si2147_optionStruct    Si2147_ATV_RF_TOP_PROP_ATV_RF_TOP[] =
{
  {"AUTO",      0},
  {"P5DB",      1},
  {"P4DB",      2},
  {"P3DB",      3},
  {"P2DB",      4},
  {"P1DB",      5},
  {"0DB",       6},
  {"M1DB",      7},
  {"M2DB",      8},
  {"M3DB",      9},
  {"M4DB",     10},
  {"M5DB",     11},
  {"M6DB",     12},
  {"M7DB",     13},
  {"M8DB",     14},
  {"M9DB",     15},
  {"M10DB",    16},
  {"M11DB",    17},
  {"M12DB",    18},
  {"M13DB",    19},
  {"M14DB",    20},
  {"M15DB",    21},
  {"M16DB",    22},
  {"M17DB",    23},
  {"M18DB",    24},
};
Si2147_fieldDicoStruct Si2147_ATV_RF_TOP_PROP_FIELDS[] =
{
  {"ATV_RF_TOP", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_rf_top.atv_rf_top, sizeof(Si2147_ATV_RF_TOP_PROP_ATV_RF_TOP)/sizeof(Si2147_optionStruct), Si2147_ATV_RF_TOP_PROP_ATV_RF_TOP},
};

Si2147_optionStruct    Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_LO[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_optionStruct    Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_HI[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[] =
{
  {"LO",   Si2147_SIGNED_CHAR, &Si2147_prop.atv_rsq_rssi_threshold.lo, sizeof(Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_LO)/sizeof(Si2147_optionStruct), Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_LO},
  {"HI",   Si2147_SIGNED_CHAR, &Si2147_prop.atv_rsq_rssi_threshold.hi, sizeof(Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_HI)/sizeof(Si2147_optionStruct), Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_HI},
};

Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_VIDEO_SYS[] =
{
  {"B",      0},
  {"GH",     1},
  {"M",      2},
  {"N",      3},
  {"I",      4},
  {"DK",     5},
  {"L",      6},
  {"LP",     7},
};
Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_COLOR[] =
{
  {"PAL_NTSC",     0},
  {"SECAM",        1},
};
Si2147_optionStruct    Si2147_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM[] =
{
  {"NORMAL",       0},
  {"INVERTED",     1},
};
Si2147_fieldDicoStruct Si2147_ATV_VIDEO_MODE_PROP_FIELDS[] =
{
  {"VIDEO_SYS",       Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_video_mode.video_sys      , sizeof(Si2147_ATV_VIDEO_MODE_PROP_VIDEO_SYS      )/sizeof(Si2147_optionStruct), Si2147_ATV_VIDEO_MODE_PROP_VIDEO_SYS      },
  {"COLOR",           Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_video_mode.color          , sizeof(Si2147_ATV_VIDEO_MODE_PROP_COLOR          )/sizeof(Si2147_optionStruct), Si2147_ATV_VIDEO_MODE_PROP_COLOR          },
  {"INVERT_SPECTRUM", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_video_mode.invert_spectrum, sizeof(Si2147_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM)/sizeof(Si2147_optionStruct), Si2147_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM},
};

Si2147_optionStruct    Si2147_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_ATV_VSNR_CAP_PROP_FIELDS[] =
{
  {"ATV_VSNR_CAP", Si2147_UNSIGNED_CHAR, &Si2147_prop.atv_vsnr_cap.atv_vsnr_cap, sizeof(Si2147_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP)/sizeof(Si2147_optionStruct), Si2147_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP},
};

Si2147_optionStruct    Si2147_CRYSTAL_TRIM_PROP_XO_CAP[] =
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
Si2147_fieldDicoStruct Si2147_CRYSTAL_TRIM_PROP_FIELDS[] =
{
  {"XO_CAP", Si2147_UNSIGNED_CHAR, &Si2147_prop.crystal_trim.xo_cap, sizeof(Si2147_CRYSTAL_TRIM_PROP_XO_CAP)/sizeof(Si2147_optionStruct), Si2147_CRYSTAL_TRIM_PROP_XO_CAP},
};

Si2147_optionStruct    Si2147_DTV_AGC_FREEZE_INPUT_PROP_LEVEL[] =
{
  {"LOW",      0},
  {"HIGH",     1},
};
Si2147_optionStruct    Si2147_DTV_AGC_FREEZE_INPUT_PROP_PIN[] =
{
  {"NONE",         0},
  {"GPIO1",        1},
  {"GPIO2",        2},
  {"RESERVED",     3},
  {"AGC1",         4},
  {"AGC2",         5},
  {"LIF1A",        6},
  {"LIF1B",        7},
};
Si2147_fieldDicoStruct Si2147_DTV_AGC_FREEZE_INPUT_PROP_FIELDS[] =
{
  {"LEVEL", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_agc_freeze_input.level, sizeof(Si2147_DTV_AGC_FREEZE_INPUT_PROP_LEVEL)/sizeof(Si2147_optionStruct), Si2147_DTV_AGC_FREEZE_INPUT_PROP_LEVEL},
  {"PIN",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_agc_freeze_input.pin  , sizeof(Si2147_DTV_AGC_FREEZE_INPUT_PROP_PIN  )/sizeof(Si2147_optionStruct), Si2147_DTV_AGC_FREEZE_INPUT_PROP_PIN  },
};

Si2147_optionStruct    Si2147_DTV_AGC_SPEED_PROP_IF_AGC_SPEED[] =
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
Si2147_optionStruct    Si2147_DTV_AGC_SPEED_PROP_AGC_DECIM[] =
{
  {"OFF",     0},
  {"2",       1},
  {"4",       2},
  {"8",       3},
};
Si2147_fieldDicoStruct Si2147_DTV_AGC_SPEED_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_agc_speed.if_agc_speed, sizeof(Si2147_DTV_AGC_SPEED_PROP_IF_AGC_SPEED)/sizeof(Si2147_optionStruct), Si2147_DTV_AGC_SPEED_PROP_IF_AGC_SPEED},
  {"AGC_DECIM",    Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_agc_speed.agc_decim   , sizeof(Si2147_DTV_AGC_SPEED_PROP_AGC_DECIM   )/sizeof(Si2147_optionStruct), Si2147_DTV_AGC_SPEED_PROP_AGC_DECIM   },
};

Si2147_optionStruct    Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE[] =
{
  {"LIF_IF1",         0},
  {"LIF_IF2",         1},
  {"LIF_SE_IF1A",     4},
  {"LIF_SE_IF2A",     5},
};
Si2147_optionStruct    Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE[] =
{
  {"INTERNAL",      0},
  {"AGC1_3DB",      1},
  {"AGC2_3DB",      2},
  {"AGC1_FULL",     3},
  {"AGC2_FULL",     4},
};
Si2147_fieldDicoStruct Si2147_DTV_CONFIG_IF_PORT_PROP_FIELDS[] =
{
  {"DTV_OUT_TYPE",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_config_if_port.dtv_out_type  , sizeof(Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE  )/sizeof(Si2147_optionStruct), Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE  },
  {"DTV_AGC_SOURCE", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_config_if_port.dtv_agc_source, sizeof(Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE)/sizeof(Si2147_optionStruct), Si2147_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE},
};

Si2147_optionStruct    Si2147_DTV_EXT_AGC_PROP_MIN_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_optionStruct    Si2147_DTV_EXT_AGC_PROP_MAX_10MV[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_fieldDicoStruct Si2147_DTV_EXT_AGC_PROP_FIELDS[] =
{
  {"MIN_10MV", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_ext_agc.min_10mv, sizeof(Si2147_DTV_EXT_AGC_PROP_MIN_10MV)/sizeof(Si2147_optionStruct), Si2147_DTV_EXT_AGC_PROP_MIN_10MV},
  {"MAX_10MV", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_ext_agc.max_10mv, sizeof(Si2147_DTV_EXT_AGC_PROP_MAX_10MV)/sizeof(Si2147_optionStruct), Si2147_DTV_EXT_AGC_PROP_MAX_10MV},
};

Si2147_optionStruct    Si2147_DTV_IEN_PROP_CHLIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_DTV_IEN_PROP_FIELDS[] =
{
  {"CHLIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_ien.chlien, sizeof(Si2147_DTV_IEN_PROP_CHLIEN)/sizeof(Si2147_optionStruct), Si2147_DTV_IEN_PROP_CHLIEN},
};

Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED[] =
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
Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM[] =
{
  {"OFF",     0},
  {"2",       1},
  {"4",       2},
  {"8",       3},
};
Si2147_fieldDicoStruct Si2147_DTV_INITIAL_AGC_SPEED_PROP_FIELDS[] =
{
  {"IF_AGC_SPEED", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_initial_agc_speed.if_agc_speed, sizeof(Si2147_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED)/sizeof(Si2147_optionStruct), Si2147_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED},
  {"AGC_DECIM",    Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_initial_agc_speed.agc_decim   , sizeof(Si2147_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM   )/sizeof(Si2147_optionStruct), Si2147_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM   },
};

Si2147_optionStruct    Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE", 65535}
};
Si2147_fieldDicoStruct Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS[] =
{
  {"PERIOD", Si2147_UNSIGNED_INT, &Si2147_prop.dtv_initial_agc_speed_period.period, sizeof(Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD)/sizeof(Si2147_optionStruct), Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD},
};

Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ATSC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_QAM_US[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DVBT[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DVBC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ISDBT[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_ISDBC[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_optionStruct    Si2147_DTV_INTERNAL_ZIF_PROP_DTMB[] =
{
  {"LIF",     0},
  {"ZIF",     1},
};
Si2147_fieldDicoStruct Si2147_DTV_INTERNAL_ZIF_PROP_FIELDS[] =
{
  {"ATSC",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.atsc  , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_ATSC  )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_ATSC  },
  {"QAM_US", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.qam_us, sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_QAM_US)/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_QAM_US},
  {"DVBT",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.dvbt  , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_DVBT  )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_DVBT  },
  {"DVBC",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.dvbc  , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_DVBC  )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_DVBC  },
  {"ISDBT",  Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.isdbt , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_ISDBT )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_ISDBT },
  {"ISDBC",  Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.isdbc , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_ISDBC )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_ISDBC },
  {"DTMB",   Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_internal_zif.dtmb  , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_DTMB  )/sizeof(Si2147_optionStruct), Si2147_DTV_INTERNAL_ZIF_PROP_DTMB  },
};

Si2147_optionStruct    Si2147_DTV_INT_SENSE_PROP_CHLNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_DTV_INT_SENSE_PROP_CHLPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_DTV_INT_SENSE_PROP_FIELDS[] =
{
  {"CHLNEGEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_int_sense.chlnegen, sizeof(Si2147_DTV_INT_SENSE_PROP_CHLNEGEN)/sizeof(Si2147_optionStruct), Si2147_DTV_INT_SENSE_PROP_CHLNEGEN},
  {"CHLPOSEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_int_sense.chlposen, sizeof(Si2147_DTV_INT_SENSE_PROP_CHLPOSEN)/sizeof(Si2147_optionStruct), Si2147_DTV_INT_SENSE_PROP_CHLPOSEN},
};

Si2147_optionStruct    Si2147_DTV_LIF_FREQ_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",  7000}
};
Si2147_fieldDicoStruct Si2147_DTV_LIF_FREQ_PROP_FIELDS[] =
{
  {"OFFSET", Si2147_UNSIGNED_INT, &Si2147_prop.dtv_lif_freq.offset, sizeof(Si2147_DTV_LIF_FREQ_PROP_OFFSET)/sizeof(Si2147_optionStruct), Si2147_DTV_LIF_FREQ_PROP_OFFSET},
};

Si2147_optionStruct    Si2147_DTV_LIF_OUT_PROP_OFFSET[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_optionStruct    Si2147_DTV_LIF_OUT_PROP_AMP[] =
{
  {"MIN_RANGE",     0},
  {"MAX_RANGE",   255}
};
Si2147_fieldDicoStruct Si2147_DTV_LIF_OUT_PROP_FIELDS[] =
{
  {"OFFSET", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_lif_out.offset, sizeof(Si2147_DTV_LIF_OUT_PROP_OFFSET)/sizeof(Si2147_optionStruct), Si2147_DTV_LIF_OUT_PROP_OFFSET},
  {"AMP",    Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_lif_out.amp   , sizeof(Si2147_DTV_LIF_OUT_PROP_AMP   )/sizeof(Si2147_optionStruct), Si2147_DTV_LIF_OUT_PROP_AMP   },
};

Si2147_optionStruct    Si2147_DTV_MODE_PROP_BW[] =
{
  {"BW_6MHZ",       6},
  {"BW_7MHZ",       7},
  {"BW_8MHZ",       8},
  {"BW_1P7MHZ",     9},
  {"BW_6P1MHZ",    10},
};
Si2147_optionStruct    Si2147_DTV_MODE_PROP_MODULATION[] =
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
Si2147_optionStruct    Si2147_DTV_MODE_PROP_INVERT_SPECTRUM[] =
{
  {"NORMAL",       0},
  {"INVERTED",     1},
};
Si2147_fieldDicoStruct Si2147_DTV_MODE_PROP_FIELDS[] =
{
  {"BW",              Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_mode.bw             , sizeof(Si2147_DTV_MODE_PROP_BW             )/sizeof(Si2147_optionStruct), Si2147_DTV_MODE_PROP_BW             },
  {"MODULATION",      Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_mode.modulation     , sizeof(Si2147_DTV_MODE_PROP_MODULATION     )/sizeof(Si2147_optionStruct), Si2147_DTV_MODE_PROP_MODULATION     },
  {"INVERT_SPECTRUM", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_mode.invert_spectrum, sizeof(Si2147_DTV_MODE_PROP_INVERT_SPECTRUM)/sizeof(Si2147_optionStruct), Si2147_DTV_MODE_PROP_INVERT_SPECTRUM},
};

Si2147_optionStruct    Si2147_DTV_PGA_LIMITS_PROP_MIN[] =
{
  {"MIN_RANGE",    -1},
  {"MAX_RANGE",    56}
};
Si2147_optionStruct    Si2147_DTV_PGA_LIMITS_PROP_MAX[] =
{
  {"MIN_RANGE",    -1},
  {"MAX_RANGE",    56}
};
Si2147_fieldDicoStruct Si2147_DTV_PGA_LIMITS_PROP_FIELDS[] =
{
  {"MIN",   Si2147_SIGNED_CHAR, &Si2147_prop.dtv_pga_limits.min, sizeof(Si2147_DTV_PGA_LIMITS_PROP_MIN)/sizeof(Si2147_optionStruct), Si2147_DTV_PGA_LIMITS_PROP_MIN},
  {"MAX",   Si2147_SIGNED_CHAR, &Si2147_prop.dtv_pga_limits.max, sizeof(Si2147_DTV_PGA_LIMITS_PROP_MAX)/sizeof(Si2147_optionStruct), Si2147_DTV_PGA_LIMITS_PROP_MAX},
};

Si2147_optionStruct    Si2147_DTV_PGA_TARGET_PROP_PGA_TARGET[] =
{
  {"MIN_RANGE",   -13},
  {"MAX_RANGE",     7}
};
Si2147_optionStruct    Si2147_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_DTV_PGA_TARGET_PROP_FIELDS[] =
{
  {"PGA_TARGET",        Si2147_SIGNED_CHAR, &Si2147_prop.dtv_pga_target.pga_target     , sizeof(Si2147_DTV_PGA_TARGET_PROP_PGA_TARGET     )/sizeof(Si2147_optionStruct), Si2147_DTV_PGA_TARGET_PROP_PGA_TARGET     },
  {"OVERRIDE_ENABLE", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_pga_target.override_enable, sizeof(Si2147_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE)/sizeof(Si2147_optionStruct), Si2147_DTV_PGA_TARGET_PROP_OVERRIDE_ENABLE},
};

Si2147_optionStruct    Si2147_DTV_RF_TOP_PROP_DTV_RF_TOP[] =
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
Si2147_fieldDicoStruct Si2147_DTV_RF_TOP_PROP_FIELDS[] =
{
  {"DTV_RF_TOP", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_rf_top.dtv_rf_top, sizeof(Si2147_DTV_RF_TOP_PROP_DTV_RF_TOP)/sizeof(Si2147_optionStruct), Si2147_DTV_RF_TOP_PROP_DTV_RF_TOP},
};

Si2147_optionStruct    Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_LO[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_optionStruct    Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_HI[] =
{
  {"MIN_RANGE",  -128},
  {"MAX_RANGE",   127}
};
Si2147_fieldDicoStruct Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS[] =
{
  {"LO",   Si2147_SIGNED_CHAR, &Si2147_prop.dtv_rsq_rssi_threshold.lo, sizeof(Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_LO)/sizeof(Si2147_optionStruct), Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_LO},
  {"HI",   Si2147_SIGNED_CHAR, &Si2147_prop.dtv_rsq_rssi_threshold.hi, sizeof(Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_HI)/sizeof(Si2147_optionStruct), Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_HI},
};

Si2147_optionStruct    Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH[] =
{
  {"4_Hz",          0},
  {"8_Hz",          1},
  {"15_Hz",         2},
  {"30_Hz",         3},
  {"61_Hz",         4},
  {"121_Hz",        5},
  {"243_Hz",        6},
  {"486_Hz",        7},
  {"972_Hz",        8},
  {"1943_Hz",       9},
  {"3888_Hz",      10},
  {"7779_Hz",      11},
  {"15573_Hz",     12},
  {"31207_Hz",     13},
  {"62658_Hz",     14},
  {"126303_Hz",    15},
  {"DEFAULT",      16},
};
Si2147_fieldDicoStruct Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_FIELDS[] =
{
  {"BANDWIDTH", Si2147_UNSIGNED_CHAR, &Si2147_prop.dtv_zif_dc_canceller_bw.bandwidth, sizeof(Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH)/sizeof(Si2147_optionStruct), Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_BANDWIDTH},
};

Si2147_optionStruct    Si2147_MASTER_IEN_PROP_TUNIEN[] =
{
  {"OFF",     0},
  {"ON",      1},
};
Si2147_optionStruct    Si2147_MASTER_IEN_PROP_ATVIEN[] =
{
  {"OFF",     0},
  {"ON",      1},
};
Si2147_optionStruct    Si2147_MASTER_IEN_PROP_DTVIEN[] =
{
  {"OFF",     0},
  {"ON",      1},
};
Si2147_optionStruct    Si2147_MASTER_IEN_PROP_ERRIEN[] =
{
  {"OFF",     0},
  {"ON",      1},
};
Si2147_optionStruct    Si2147_MASTER_IEN_PROP_CTSIEN[] =
{
  {"OFF",     0},
  {"ON",      1},
};
Si2147_fieldDicoStruct Si2147_MASTER_IEN_PROP_FIELDS[] =
{
  {"TUNIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.master_ien.tunien, sizeof(Si2147_MASTER_IEN_PROP_TUNIEN)/sizeof(Si2147_optionStruct), Si2147_MASTER_IEN_PROP_TUNIEN},
  {"ATVIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.master_ien.atvien, sizeof(Si2147_MASTER_IEN_PROP_ATVIEN)/sizeof(Si2147_optionStruct), Si2147_MASTER_IEN_PROP_ATVIEN},
  {"DTVIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.master_ien.dtvien, sizeof(Si2147_MASTER_IEN_PROP_DTVIEN)/sizeof(Si2147_optionStruct), Si2147_MASTER_IEN_PROP_DTVIEN},
  {"ERRIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.master_ien.errien, sizeof(Si2147_MASTER_IEN_PROP_ERRIEN)/sizeof(Si2147_optionStruct), Si2147_MASTER_IEN_PROP_ERRIEN},
  {"CTSIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.master_ien.ctsien, sizeof(Si2147_MASTER_IEN_PROP_CTSIEN)/sizeof(Si2147_optionStruct), Si2147_MASTER_IEN_PROP_CTSIEN},
};

Si2147_optionStruct    Si2147_TUNER_BLOCKED_VCO_PROP_VCO_CODE[] =
{
  {"MIN_RANGE", -32768},
  {"MAX_RANGE", 32767}
};
Si2147_fieldDicoStruct Si2147_TUNER_BLOCKED_VCO_PROP_FIELDS[] =
{
  {"VCO_CODE",   Si2147_SIGNED_INT, &Si2147_prop.tuner_blocked_vco.vco_code, sizeof(Si2147_TUNER_BLOCKED_VCO_PROP_VCO_CODE)/sizeof(Si2147_optionStruct), Si2147_TUNER_BLOCKED_VCO_PROP_VCO_CODE},
};

Si2147_optionStruct    Si2147_TUNER_IEN_PROP_TCIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_IEN_PROP_RSSILIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_IEN_PROP_RSSIHIEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_TUNER_IEN_PROP_FIELDS[] =
{
  {"TCIEN",    Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_ien.tcien   , sizeof(Si2147_TUNER_IEN_PROP_TCIEN   )/sizeof(Si2147_optionStruct), Si2147_TUNER_IEN_PROP_TCIEN   },
  {"RSSILIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_ien.rssilien, sizeof(Si2147_TUNER_IEN_PROP_RSSILIEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_IEN_PROP_RSSILIEN},
  {"RSSIHIEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_ien.rssihien, sizeof(Si2147_TUNER_IEN_PROP_RSSIHIEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_IEN_PROP_RSSIHIEN},
};

Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_TCNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSILNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSIHNEGEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_TCPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSILPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_optionStruct    Si2147_TUNER_INT_SENSE_PROP_RSSIHPOSEN[] =
{
  {"DISABLE",     0},
  {"ENABLE",      1},
};
Si2147_fieldDicoStruct Si2147_TUNER_INT_SENSE_PROP_FIELDS[] =
{
  {"TCNEGEN",    Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.tcnegen   , sizeof(Si2147_TUNER_INT_SENSE_PROP_TCNEGEN   )/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_TCNEGEN   },
  {"RSSILNEGEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.rssilnegen, sizeof(Si2147_TUNER_INT_SENSE_PROP_RSSILNEGEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_RSSILNEGEN},
  {"RSSIHNEGEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.rssihnegen, sizeof(Si2147_TUNER_INT_SENSE_PROP_RSSIHNEGEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_RSSIHNEGEN},
  {"TCPOSEN",    Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.tcposen   , sizeof(Si2147_TUNER_INT_SENSE_PROP_TCPOSEN   )/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_TCPOSEN   },
  {"RSSILPOSEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.rssilposen, sizeof(Si2147_TUNER_INT_SENSE_PROP_RSSILPOSEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_RSSILPOSEN},
  {"RSSIHPOSEN", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_int_sense.rssihposen, sizeof(Si2147_TUNER_INT_SENSE_PROP_RSSIHPOSEN)/sizeof(Si2147_optionStruct), Si2147_TUNER_INT_SENSE_PROP_RSSIHPOSEN},
};

Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_1[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_2[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2147_optionStruct    Si2147_TUNER_LO_INJECTION_PROP_BAND_3[] =
{
  {"LOW_SIDE",      0},
  {"HIGH_SIDE",     1},
};
Si2147_fieldDicoStruct Si2147_TUNER_LO_INJECTION_PROP_FIELDS[] =
{
  {"BAND_1", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_lo_injection.band_1, sizeof(Si2147_TUNER_LO_INJECTION_PROP_BAND_1)/sizeof(Si2147_optionStruct), Si2147_TUNER_LO_INJECTION_PROP_BAND_1},
  {"BAND_2", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_lo_injection.band_2, sizeof(Si2147_TUNER_LO_INJECTION_PROP_BAND_2)/sizeof(Si2147_optionStruct), Si2147_TUNER_LO_INJECTION_PROP_BAND_2},
  {"BAND_3", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_lo_injection.band_3, sizeof(Si2147_TUNER_LO_INJECTION_PROP_BAND_3)/sizeof(Si2147_optionStruct), Si2147_TUNER_LO_INJECTION_PROP_BAND_3},
};

Si2147_optionStruct    Si2147_TUNER_RETURN_LOSS_PROP_CONFIG[] =
{
  {"3",       3},
  {"7",       7},
  {"11",     11},
  {"15",     15},
  {"19",     19},
  {"23",     23},
  {"27",     27},
  {"31",     31},
  {"35",     35},
  {"39",     39},
  {"43",     43},
  {"47",     47},
  {"51",     51},
  {"59",     59},
  {"67",     67},
  {"75",     75},
  {"83",     83},
  {"91",     91},
  {"103",   103},
  {"115",   115},
  {"127",   127},
};
Si2147_optionStruct    Si2147_TUNER_RETURN_LOSS_PROP_MODE[] =
{
  {"TERRESTRIAL",     0},
  {"CABLE",           1},
};
Si2147_fieldDicoStruct Si2147_TUNER_RETURN_LOSS_PROP_FIELDS[] =
{
  {"CONFIG", Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_return_loss.config, sizeof(Si2147_TUNER_RETURN_LOSS_PROP_CONFIG)/sizeof(Si2147_optionStruct), Si2147_TUNER_RETURN_LOSS_PROP_CONFIG},
  {"MODE",   Si2147_UNSIGNED_CHAR, &Si2147_prop.tuner_return_loss.mode  , sizeof(Si2147_TUNER_RETURN_LOSS_PROP_MODE  )/sizeof(Si2147_optionStruct), Si2147_TUNER_RETURN_LOSS_PROP_MODE  },
};

Si2147_optionStruct    Si2147_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS[] =
{
  {"AUTO",       0},
  {"P6DBM",      6},
  {"P5DBM",      7},
  {"P4DBM",      8},
  {"P3DBM",      9},
  {"P2DBM",     10},
  {"P1DBM",     11},
  {"0DBM",      12},
  {"M1DBM",     13},
  {"M2DBM",     14},
  {"M3DBM",     15},
  {"M4DBM",     16},
  {"M5DBM",     17},
  {"M6DBM",     18},
  {"M7DBM",     19},
  {"M8DBM",     20},
  {"M9DBM",     21},
  {"M10DBM",    22},
  {"M11DBM",    23},
  {"M12DBM",    24},
};
Si2147_fieldDicoStruct Si2147_WIDE_BAND_ATT_THRS_PROP_FIELDS[] =
{
  {"WB_ATT_THRS", Si2147_UNSIGNED_CHAR, &Si2147_prop.wide_band_att_thrs.wb_att_thrs, sizeof(Si2147_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS)/sizeof(Si2147_optionStruct), Si2147_WIDE_BAND_ATT_THRS_PROP_WB_ATT_THRS},
};

Si2147_optionStruct    Si2147_XOUT_PROP_AMP[] =
{
  {"HIGH",     0},
  {"LOW",      1},
};
Si2147_fieldDicoStruct Si2147_XOUT_PROP_FIELDS[] =
{
  {"AMP", Si2147_UNSIGNED_CHAR, &Si2147_prop.xout.amp, sizeof(Si2147_XOUT_PROP_AMP)/sizeof(Si2147_optionStruct), Si2147_XOUT_PROP_AMP},
};

Si2147_propertyInfoStruct Si2147_propertyDictionary[] =
{
  {Si2147_ATV   , "ATV_AFC_RANGE"               , sizeof(Si2147_ATV_AFC_RANGE_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_AFC_RANGE_PROP_FIELDS                , Si2147_ATV_AFC_RANGE_PROP                },
  {Si2147_ATV   , "ATV_AGC_SPEED"               , sizeof(Si2147_ATV_AGC_SPEED_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_AGC_SPEED_PROP_FIELDS                , Si2147_ATV_AGC_SPEED_PROP                },
  {Si2147_ATV   , "ATV_AGC_SPEED_LOW_RSSI"      , sizeof(Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_FIELDS      )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP_FIELDS       , Si2147_ATV_AGC_SPEED_LOW_RSSI_PROP       },
  {Si2147_ATV   , "ATV_ARTIFICIAL_SNOW"         , sizeof(Si2147_ATV_ARTIFICIAL_SNOW_PROP_FIELDS         )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_ARTIFICIAL_SNOW_PROP_FIELDS          , Si2147_ATV_ARTIFICIAL_SNOW_PROP          },
  {Si2147_ATV   , "ATV_CONFIG_IF_PORT"          , sizeof(Si2147_ATV_CONFIG_IF_PORT_PROP_FIELDS          )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_CONFIG_IF_PORT_PROP_FIELDS           , Si2147_ATV_CONFIG_IF_PORT_PROP           },
  {Si2147_ATV   , "ATV_EXT_AGC"                 , sizeof(Si2147_ATV_EXT_AGC_PROP_FIELDS                 )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_EXT_AGC_PROP_FIELDS                  , Si2147_ATV_EXT_AGC_PROP                  },
  {Si2147_ATV   , "ATV_HSYNC_OUT"               , sizeof(Si2147_ATV_HSYNC_OUT_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_HSYNC_OUT_PROP_FIELDS                , Si2147_ATV_HSYNC_OUT_PROP                },
  {Si2147_ATV   , "ATV_IEN"                     , sizeof(Si2147_ATV_IEN_PROP_FIELDS                     )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_IEN_PROP_FIELDS                      , Si2147_ATV_IEN_PROP                      },
  {Si2147_ATV   , "ATV_INT_SENSE"               , sizeof(Si2147_ATV_INT_SENSE_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_INT_SENSE_PROP_FIELDS                , Si2147_ATV_INT_SENSE_PROP                },
  {Si2147_ATV   , "ATV_LIF_FREQ"                , sizeof(Si2147_ATV_LIF_FREQ_PROP_FIELDS                )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_LIF_FREQ_PROP_FIELDS                 , Si2147_ATV_LIF_FREQ_PROP                 },
  {Si2147_ATV   , "ATV_LIF_OUT"                 , sizeof(Si2147_ATV_LIF_OUT_PROP_FIELDS                 )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_LIF_OUT_PROP_FIELDS                  , Si2147_ATV_LIF_OUT_PROP                  },
  {Si2147_ATV   , "ATV_PGA_TARGET"              , sizeof(Si2147_ATV_PGA_TARGET_PROP_FIELDS              )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_PGA_TARGET_PROP_FIELDS               , Si2147_ATV_PGA_TARGET_PROP               },
  {Si2147_ATV   , "ATV_RF_TOP"                  , sizeof(Si2147_ATV_RF_TOP_PROP_FIELDS                  )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_RF_TOP_PROP_FIELDS                   , Si2147_ATV_RF_TOP_PROP                   },
  {Si2147_ATV   , "ATV_RSQ_RSSI_THRESHOLD"      , sizeof(Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_FIELDS      )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP_FIELDS       , Si2147_ATV_RSQ_RSSI_THRESHOLD_PROP       },
  {Si2147_ATV   , "ATV_VIDEO_MODE"              , sizeof(Si2147_ATV_VIDEO_MODE_PROP_FIELDS              )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_VIDEO_MODE_PROP_FIELDS               , Si2147_ATV_VIDEO_MODE_PROP               },
  {Si2147_ATV   , "ATV_VSNR_CAP"                , sizeof(Si2147_ATV_VSNR_CAP_PROP_FIELDS                )/sizeof(Si2147_fieldDicoStruct), Si2147_ATV_VSNR_CAP_PROP_FIELDS                 , Si2147_ATV_VSNR_CAP_PROP                 },
  {Si2147_COMMON, "CRYSTAL_TRIM"                , sizeof(Si2147_CRYSTAL_TRIM_PROP_FIELDS                )/sizeof(Si2147_fieldDicoStruct), Si2147_CRYSTAL_TRIM_PROP_FIELDS                 , Si2147_CRYSTAL_TRIM_PROP                 },
  {Si2147_COMMON, "MASTER_IEN"                  , sizeof(Si2147_MASTER_IEN_PROP_FIELDS                  )/sizeof(Si2147_fieldDicoStruct), Si2147_MASTER_IEN_PROP_FIELDS                   , Si2147_MASTER_IEN_PROP                   },
  {Si2147_COMMON, "XOUT"                        , sizeof(Si2147_XOUT_PROP_FIELDS                        )/sizeof(Si2147_fieldDicoStruct), Si2147_XOUT_PROP_FIELDS                         , Si2147_XOUT_PROP                         },
  {Si2147_DTV   , "DTV_AGC_FREEZE_INPUT"        , sizeof(Si2147_DTV_AGC_FREEZE_INPUT_PROP_FIELDS        )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_AGC_FREEZE_INPUT_PROP_FIELDS         , Si2147_DTV_AGC_FREEZE_INPUT_PROP         },
  {Si2147_DTV   , "DTV_AGC_SPEED"               , sizeof(Si2147_DTV_AGC_SPEED_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_AGC_SPEED_PROP_FIELDS                , Si2147_DTV_AGC_SPEED_PROP                },
  {Si2147_DTV   , "DTV_CONFIG_IF_PORT"          , sizeof(Si2147_DTV_CONFIG_IF_PORT_PROP_FIELDS          )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_CONFIG_IF_PORT_PROP_FIELDS           , Si2147_DTV_CONFIG_IF_PORT_PROP           },
  {Si2147_DTV   , "DTV_EXT_AGC"                 , sizeof(Si2147_DTV_EXT_AGC_PROP_FIELDS                 )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_EXT_AGC_PROP_FIELDS                  , Si2147_DTV_EXT_AGC_PROP                  },
  {Si2147_DTV   , "DTV_IEN"                     , sizeof(Si2147_DTV_IEN_PROP_FIELDS                     )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_IEN_PROP_FIELDS                      , Si2147_DTV_IEN_PROP                      },
  {Si2147_DTV   , "DTV_INITIAL_AGC_SPEED"       , sizeof(Si2147_DTV_INITIAL_AGC_SPEED_PROP_FIELDS       )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_INITIAL_AGC_SPEED_PROP_FIELDS        , Si2147_DTV_INITIAL_AGC_SPEED_PROP        },
  {Si2147_DTV   , "DTV_INITIAL_AGC_SPEED_PERIOD", sizeof(Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS)/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_FIELDS , Si2147_DTV_INITIAL_AGC_SPEED_PERIOD_PROP },
  {Si2147_DTV   , "DTV_INTERNAL_ZIF"            , sizeof(Si2147_DTV_INTERNAL_ZIF_PROP_FIELDS            )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_INTERNAL_ZIF_PROP_FIELDS             , Si2147_DTV_INTERNAL_ZIF_PROP             },
  {Si2147_DTV   , "DTV_INT_SENSE"               , sizeof(Si2147_DTV_INT_SENSE_PROP_FIELDS               )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_INT_SENSE_PROP_FIELDS                , Si2147_DTV_INT_SENSE_PROP                },
  {Si2147_DTV   , "DTV_LIF_FREQ"                , sizeof(Si2147_DTV_LIF_FREQ_PROP_FIELDS                )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_LIF_FREQ_PROP_FIELDS                 , Si2147_DTV_LIF_FREQ_PROP                 },
  {Si2147_DTV   , "DTV_LIF_OUT"                 , sizeof(Si2147_DTV_LIF_OUT_PROP_FIELDS                 )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_LIF_OUT_PROP_FIELDS                  , Si2147_DTV_LIF_OUT_PROP                  },
  {Si2147_DTV   , "DTV_MODE"                    , sizeof(Si2147_DTV_MODE_PROP_FIELDS                    )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_MODE_PROP_FIELDS                     , Si2147_DTV_MODE_PROP                     },
  {Si2147_DTV   , "DTV_PGA_LIMITS"              , sizeof(Si2147_DTV_PGA_LIMITS_PROP_FIELDS              )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_PGA_LIMITS_PROP_FIELDS               , Si2147_DTV_PGA_LIMITS_PROP               },
  {Si2147_DTV   , "DTV_PGA_TARGET"              , sizeof(Si2147_DTV_PGA_TARGET_PROP_FIELDS              )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_PGA_TARGET_PROP_FIELDS               , Si2147_DTV_PGA_TARGET_PROP               },
  {Si2147_DTV   , "DTV_RF_TOP"                  , sizeof(Si2147_DTV_RF_TOP_PROP_FIELDS                  )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_RF_TOP_PROP_FIELDS                   , Si2147_DTV_RF_TOP_PROP                   },
  {Si2147_DTV   , "DTV_RSQ_RSSI_THRESHOLD"      , sizeof(Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS      )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP_FIELDS       , Si2147_DTV_RSQ_RSSI_THRESHOLD_PROP       },
  {Si2147_DTV   , "DTV_ZIF_DC_CANCELLER_BW"     , sizeof(Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_FIELDS     )/sizeof(Si2147_fieldDicoStruct), Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP_FIELDS      , Si2147_DTV_ZIF_DC_CANCELLER_BW_PROP      },
  {Si2147_TUNER , "TUNER_BLOCKED_VCO"           , sizeof(Si2147_TUNER_BLOCKED_VCO_PROP_FIELDS           )/sizeof(Si2147_fieldDicoStruct), Si2147_TUNER_BLOCKED_VCO_PROP_FIELDS            , Si2147_TUNER_BLOCKED_VCO_PROP            },
  {Si2147_TUNER , "TUNER_IEN"                   , sizeof(Si2147_TUNER_IEN_PROP_FIELDS                   )/sizeof(Si2147_fieldDicoStruct), Si2147_TUNER_IEN_PROP_FIELDS                    , Si2147_TUNER_IEN_PROP                    },
  {Si2147_TUNER , "TUNER_INT_SENSE"             , sizeof(Si2147_TUNER_INT_SENSE_PROP_FIELDS             )/sizeof(Si2147_fieldDicoStruct), Si2147_TUNER_INT_SENSE_PROP_FIELDS              , Si2147_TUNER_INT_SENSE_PROP              },
  {Si2147_TUNER , "TUNER_LO_INJECTION"          , sizeof(Si2147_TUNER_LO_INJECTION_PROP_FIELDS          )/sizeof(Si2147_fieldDicoStruct), Si2147_TUNER_LO_INJECTION_PROP_FIELDS           , Si2147_TUNER_LO_INJECTION_PROP           },
  {Si2147_TUNER , "TUNER_RETURN_LOSS"           , sizeof(Si2147_TUNER_RETURN_LOSS_PROP_FIELDS           )/sizeof(Si2147_fieldDicoStruct), Si2147_TUNER_RETURN_LOSS_PROP_FIELDS            , Si2147_TUNER_RETURN_LOSS_PROP            },
  {Si2147_TUNER , "WIDE_BAND_ATT_THRS"          , sizeof(Si2147_WIDE_BAND_ATT_THRS_PROP_FIELDS          )/sizeof(Si2147_fieldDicoStruct), Si2147_WIDE_BAND_ATT_THRS_PROP_FIELDS           , Si2147_WIDE_BAND_ATT_THRS_PROP           },
};

int   Si2147_PropertyNames        (L1_Si2147_Context *api, char *msg)
{
  int propIndex;
  int size;
  api = api; /* to avoid 'unused parameter' warning */
  size = sizeof(Si2147_propertyDictionary)/sizeof(Si2147_propertyInfoStruct);
  msg[0]='\0';
  for (propIndex=0; propIndex<size; propIndex++)
  {
    strcat(msg,Si2147_propertyDictionary[propIndex].name);
    if (propIndex< (size-1)) strcat(msg," ");
  }
  return propIndex;
};

int   Si2147_PropertyIndex        (L1_Si2147_Context *api, const char *property)
{
  int propIndex;
  int size;
  api = api; /* to avoid 'unused parameter' warning */
  size = sizeof(Si2147_propertyDictionary)/sizeof(Si2147_propertyInfoStruct);
  for (propIndex=0; propIndex <size; propIndex++)
  {
    if (strcmp_nocase(property, Si2147_propertyDictionary[propIndex].name)==0)
    {
      return propIndex;
    }
  }
  return -1;
};

int   Si2147_PropertyFields       (L1_Si2147_Context *api, int propIndex, char *msg)
{
  int fieldIndex;
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (fieldIndex=0; fieldIndex < Si2147_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    strcat(msg, Si2147_propertyDictionary[propIndex].field[fieldIndex].name);
    if (fieldIndex< (Si2147_propertyDictionary[propIndex].nbFields-1)) strcat(msg," ");
  }
  return -1;
};

int   Si2147_PropertyFieldIndex   (L1_Si2147_Context *api, int propIndex, char *field)
{
  int fieldIndex;
  api = api; /* to avoid 'unused parameter' warning */
  for (fieldIndex=0; fieldIndex < Si2147_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    if (strcmp_nocase(field, Si2147_propertyDictionary[propIndex].field[fieldIndex].name)==0)
    {
      return fieldIndex;
    }
  }
  return -1;
};

int   Si2147_PropertyFieldEnums   (L1_Si2147_Context *api, int propIndex, int fieldIndex, char *msg)
{
  int enumIndex;
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (enumIndex=0; enumIndex < Si2147_propertyDictionary[propIndex].field[fieldIndex].nbOptions; enumIndex++)
  {
    strcat(msg, Si2147_propertyDictionary[propIndex].field[fieldIndex].option[enumIndex].name);
    if (enumIndex < (Si2147_propertyDictionary[propIndex].field[fieldIndex].nbOptions-1)) strcat(msg," ");
  }
  return enumIndex;
};

int   Si2147_PropertyFieldRanges  (L1_Si2147_Context *api, int propIndex, int fieldIndex, char *msg)
{
  int index;
  char buf[10];
  api = api; /* to avoid 'unused parameter' warning */
  msg[0]='\0';
  for (index=0; index < 2; index++)
  {
    itoa(Si2147_propertyDictionary[propIndex].field[fieldIndex].option[index].value,buf,10);
    strcat(msg, buf);
    if (index < 1) strcat(msg," - ");
  }
  return index;
};

int   Si2147_GetValueFromEntry    (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, char *entry, int *value)
{
  int optionIndex;
  api = api; /* to avoid 'unused parameter' warning */

  if (!strcmp_nocase(field.option[0].name,"MIN_RANGE"))
  {
    if (field.datatype== Si2147_UNSIGNED_INT || field.datatype== Si2147_UNSIGNED_CHAR)
    {
      *value = (unsigned int)atoi(entry);
      return 1;
    }
    else if (field.datatype == Si2147_SIGNED_INT || field.datatype == Si2147_SIGNED_CHAR)
    {
      *value = (int)atoi(entry);
      return 1;
    }
  }
  for (optionIndex=0; optionIndex < field.nbOptions; optionIndex++)
  {
    if (strcmp_nocase(entry, field.option[optionIndex].name     )==0)
    {
      *value = field.option[optionIndex].value;
      return 1;
    }
  }
  return 0;
};

char *Si2147_GetEnumFromValue     (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, int iValue)
{
  int optionIndex;
  api = api; /* to avoid 'unused parameter' warning */
  if (field.nbOptions ==0 )
    {
    sprintf(optionText, "%d", iValue);
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

int   Si2147_GetValueFromField    (L1_Si2147_Context *api, Si2147_fieldDicoStruct field)
{
  short intValue;
  short *fieldAsInt;
  short iTmp=0;
  fieldAsInt = &intValue;

  if (field.datatype == Si2147_SIGNED_INT   )
  {
      fieldAsInt= (short *) ((field.pField-(void *) &Si2147_prop)+ (void *)api->prop);
  }
  else if (field.datatype == Si2147_UNSIGNED_CHAR)
  {
      iTmp = *((unsigned char *) ((field.pField-(void *) &Si2147_prop)+ (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else if (field.datatype == Si2147_SIGNED_CHAR)
  {
      iTmp= *((char *) ((field.pField-(void *) &Si2147_prop)+ (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else /* if (field.datatype == Si2147_UNSIGNED_INT) */
  {
      fieldAsInt = (unsigned short *) ((field.pField-(void *) &Si2147_prop)+ (void *)api->prop);
  }

  return  *fieldAsInt;
};

char *Si2147_GetEnumFromField     (L1_Si2147_Context *api, Si2147_fieldDicoStruct field)
{
  int intValue;
  int *fieldAsInt;
  int iTmp=0;
  fieldAsInt = &intValue;

  if (field.datatype == Si2147_SIGNED_INT   )
  {
      fieldAsInt= (int *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop);
  }
  else if (field.datatype == Si2147_UNSIGNED_CHAR)
  {
      iTmp = *((unsigned char *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else if (field.datatype == Si2147_SIGNED_CHAR)
  {
      iTmp= *((char *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop));
      fieldAsInt = &iTmp;
  }
  else /* if (field.datatype == Si2147_UNSIGNED_INT) */
  {
      fieldAsInt = (unsigned int *)((field.pField-(void *) &Si2147_prop) + (void *)api->prop);
  }

  return Si2147_GetEnumFromValue(api, field, *fieldAsInt);
};

int   Si2147_SetFieldFromEntry    (L1_Si2147_Context *api, Si2147_fieldDicoStruct field, char *entry)
{
  int *fieldAsInt;
  int iTmp=0;
  fieldAsInt = &iTmp;
  if (Si2147_GetValueFromEntry(api, field, entry, fieldAsInt) == 0) return 0;
  if (field.datatype == Si2147_SIGNED_INT )
  {
    *( (int *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop))=iTmp;
  }
  else if (field.datatype == Si2147_UNSIGNED_CHAR)
  {
    *( (unsigned char *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop))=iTmp;
  }
  else if (field.datatype == Si2147_SIGNED_CHAR)
  {
    *( (char *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop))=iTmp;
  }
  else /* if (field.datatype == Si2147_UNSIGNED_INT) */
  {
    *( (unsigned int *) ((field.pField-(void *) &Si2147_prop) + (void *)api->prop))=iTmp;
  }

  return 1;
};

void  Si2147_showProperty         (L1_Si2147_Context *api, int propIndex)
{
  int fieldIndex;
  for (fieldIndex=0; fieldIndex<Si2147_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    HI_PRINT("%s:%s:%s = ", Si2147_featureText[Si2147_propertyDictionary[propIndex].feature], Si2147_propertyDictionary[propIndex].name, Si2147_propertyDictionary[propIndex].field[fieldIndex].name);
    HI_PRINT("<%s>\n", Si2147_GetEnumFromField(api, Si2147_propertyDictionary[propIndex].field[fieldIndex]));
  }

}

void  Si2147_showPropertyNamed    (L1_Si2147_Context *api, char *property)
{
  int propIndex;

  if ((propIndex = Si2147_PropertyIndex(api, property)) >= 0)
  {
    Si2147_showProperty(api, propIndex);
  }
  else
  {
    HI_PRINT("Unknown property '%s'\n", property);
  }

}

void  Si2147_setupProperty        (L1_Si2147_Context *api, int propIndex)
{
  #define LMAX 20
  char entry[LMAX];
  char setup_msg[1000];
  int fieldIndex;
  char isRangeType = 1;

  for (fieldIndex=0; fieldIndex<Si2147_propertyDictionary[propIndex].nbFields; fieldIndex++)
  {
    isRangeType = (strcmp_nocase(Si2147_propertyDictionary[propIndex].field[fieldIndex].option->name,"MIN_RANGE") == 0);

    HI_PRINT("%s:%s:%s = ", Si2147_featureText[Si2147_propertyDictionary[propIndex].feature], Si2147_propertyDictionary[propIndex].name, Si2147_propertyDictionary[propIndex].field[fieldIndex].name);

    if ( isRangeType )
    {
      HI_PRINT("<%d>\n", Si2147_GetValueFromField(api, Si2147_propertyDictionary[propIndex].field[fieldIndex]));
      Si2147_PropertyFieldRanges(api, propIndex, fieldIndex, setup_msg);
      HI_PRINT("Range: <%s>  ",setup_msg);
    }
    else
    {
      Si2147_PropertyFieldEnums(api, propIndex, fieldIndex, setup_msg);
      HI_PRINT("<%s>\n Options: <%s>\n", Si2147_GetEnumFromField(api, Si2147_propertyDictionary[propIndex].field[fieldIndex]), setup_msg);
    }

    fgets(entry,LMAX,stdin);

    if (strlen(entry) > 1)
    {
      entry[strlen(entry)-1]='\0';
      if (!Si2147_SetFieldFromEntry(api, Si2147_propertyDictionary[propIndex].field[fieldIndex], entry))
      {
        HI_PRINT("Invalid entry!\n");
      }
    }

    if (isRangeType)
      HI_PRINT("<%d>\n\n", Si2147_GetValueFromField(api, Si2147_propertyDictionary[propIndex].field[fieldIndex]));
    else
      HI_PRINT("<%s>\n\n", Si2147_GetEnumFromField(api, Si2147_propertyDictionary[propIndex].field[fieldIndex]));
  }
}

int   Si2147_setupProperties      (L1_Si2147_Context *api, Si2147_featureEnum feature)
{
  int propertyIndex;
  int dictSize;

  dictSize=sizeof(Si2147_propertyDictionary)/sizeof(Si2147_propertyInfoStruct);

  HI_PRINT("\n%s properties:\n\n", Si2147_featureText[feature]);

  for (propertyIndex=0; propertyIndex<dictSize; propertyIndex++)
  {
    if (Si2147_propertyDictionary[propertyIndex].feature != feature) continue;
    Si2147_setupProperty(api, propertyIndex);
  }
  return 0;
}

#endif /* Si2147_COMMAND_LINE_APPLICATION */

/* _property_string_functions_insertion_start */

int Si2147_setupATVProperties       (L1_Si2147_Context *api)
{
  SiTRACE("Si2147_setupATVProperties       \n");
#ifdef    Si2147_COMMAND_LINE_APPLICATION
  Si2147_setupProperties(api, Si2147_ATV);
#endif /* Si2147_COMMAND_LINE_APPLICATION */
  return 0;
}

int Si2147_setupCOMMONProperties    (L1_Si2147_Context *api)
{
  SiTRACE("Si2147_setupCOMMONProperties    \n");
#ifdef    Si2147_COMMAND_LINE_APPLICATION
  Si2147_setupProperties(api, Si2147_COMMON);
#endif /* Si2147_COMMAND_LINE_APPLICATION */
  return 0;
}

int Si2147_setupDTVProperties       (L1_Si2147_Context *api)
{
  SiTRACE("Si2147_setupDTVProperties       \n");
#ifdef    Si2147_COMMAND_LINE_APPLICATION
  Si2147_setupProperties(api, Si2147_DTV);
#endif /* Si2147_COMMAND_LINE_APPLICATION */
  return 0;
}

int Si2147_setupTUNERProperties     (L1_Si2147_Context *api)
{
  SiTRACE("Si2147_setupTUNERProperties     \n");
#ifdef    Si2147_COMMAND_LINE_APPLICATION
  Si2147_setupProperties(api, Si2147_TUNER);
#endif /* Si2147_COMMAND_LINE_APPLICATION */
  return 0;
}

int Si2147_setupAllProperties  (L1_Si2147_Context *api)
{
  Si2147_setupATVProperties       (api);
  Si2147_setupCOMMONProperties    (api);
  Si2147_setupDTVProperties       (api);
  Si2147_setupTUNERProperties     (api);
  return 0;
}






