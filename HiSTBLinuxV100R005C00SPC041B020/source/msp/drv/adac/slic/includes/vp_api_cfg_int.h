/** \file vp_api_cfg_int.h
 * vp_api_cfg_int.h
 *
 * This file contains internal configuration and compile time settings for
 * building appropriate VP-API library modules needed for any application.

 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 6419 $
 * $LastChangedDate: 2010-02-12 16:40:10 -0600 (Fri, 12 Feb 2010) $
 */

#ifndef VP_API_CFG_INT_H
#define VP_API_CFG_INT_H

/******************************************************************************
 * Device Context Size                                                        *
 *****************************************************************************/
/*
 * Define the maximum number of lines per device in your system. Note that a
 * system (i.e., all devices controlled by one instance of VP-API) could have
 * more than one type of device, each of those devices may support different
 * number of lines. For example in a system, device A might support 4 lines
 * and device B might support 32 lines, in such a scenario, the following macro
 * should be defined as 32. */
#if defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KERNEL_SERIES)
    #define VP_MAX_LINES_PER_DEVICE     (128)
#elif defined(VP_CC_VCP_SERIES)
    #define VP_MAX_LINES_PER_DEVICE     (32)
#elif defined(VP_CC_792_SERIES)
    #define VP_MAX_LINES_PER_DEVICE     (8)
#elif defined(VP_CC_790_SERIES) || defined(VP_CC_580_SERIES)
    #define VP_MAX_LINES_PER_DEVICE     (4)
#else /* VP_CC_880_SERIES + VP_CC_890_SERIES */
    #define VP_MAX_LINES_PER_DEVICE     (2)
#endif

/* Special Case for Kernel Wrapper */
#if defined(VP_CC_KWRAP)
  #undef VP_MAX_LINES_PER_DEVICE
  #define VP_MAX_LINES_PER_DEVICE (128)
#endif

/******************************************************************************
 * Library Specific COMPILE-TIME OPTIONS and defines                          *
 *****************************************************************************/
/* #if defined (VP_CC_880_SERIES) || defined (VP_CC_890_SERIES) */
    #define CSLAC_GAIN_RELATIVE
    #define CSLAC_GAIN_ABS
/* #endif */

#ifdef VP_CC_880_SERIES

/*
 * The following values exist for demo or (Zarlink) test purposes only. DO NOT
 * change to #define in production code. Unexpected or damaging behavior may
 * occur.
 */
#undef  VP880_AUTO_BAT_DETECT
#undef  VP880_ENABLE_PROD_TEST

/*
 * Modifications to previous settings to correct for potential inconsistencies.
 * DO NOT CHANGE anything below this line. All corrections should be made above
 */
#if !defined (VP880_TRACKER_SUPPORT) && defined (VP880_LP_SUPPORT)
    #error "Invalid 880 Compiler Combination: Low Power Mode Requires Tracker defined"
#endif

#if defined (VP880_FXS_SUPPORT) && (!defined (VP880_ABS_SUPPORT) && !defined (VP880_TRACKER_SUPPORT))
    #error "Invalid 880 Compiler Combination: FXS requires either ABS or Tracker defined"
#endif

#ifndef VP880_FXS_SUPPORT
    #undef VP880_ABS_SUPPORT
    #undef VP880_INCLUDE_TESTLINE_CODE
    #undef VP880_LP_SUPPORT
#endif

#if !defined (VP880_FXS_SUPPORT) && !defined (VP880_FXO_SUPPORT)
    #error "Invalid 880 Compiler Combination: Either FXS or FXO must be defined if using VE880 device"
#endif

/*
 * Current Limiting option in the VP880 library limits the peak switcher current
 * such that a 3REN load can be supported at 90V peak. Disable this option if
 * 5REN is required.
 */
#undef  VP880_CURRENT_LIMIT

#endif /* VP_CC_880_SERIES */

#if (defined (VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) || defined(VP_CC_KWRAP)
    /*
     * #defines used in unit conversion function
     * All units are based on the following calculations
     *      Internal voltage unit   = 480.0  / 2^31 Volts
     *      Internal current unit   = 240e-3 / 2^31 Amps
     *      Internal freq uint      = 12e3   / 2^31 Hz
     */
    #define VP880_UNIT_MV                          4474L    /* mV unit */
    #define VP880_UNIT_MVRMS                       6327L    /* mVrms unit */
    #define VP880_UNIT_ADC_VAB                     -32768L  /* vab step from ADC */
    #define VP880_UNIT_ADC_VAB_RMS                 46341L   /* vab step from ADC rms*/
    #define VP880_UNIT_ADC_AC_VAB_RMS                664L   /* vab step from ADC Metallic Vrms*/
    #define VP880_UNIT_DAC_RING                    21076L   /* step due to DAC in ringing*/
    #define VP880_UNIT_TRACK_SWZ                23091612L   /* step used for tracking switching regulator Z */
    #define VP880_UNIT_UA                          8948L    /* uA unit */
    #define VP880_UNIT_ADC_IMET_NOR                16384L   /* met I from ADC in normal VC */
    #define VP880_UNIT_ADC_IMET_NOR_JA             15756L   /* met I from ADC in normal JA*/
    #define VP880_UNIT_ADC_IMET_RING               32768L   /* met I from ADC in ringing*/
    #define VP880_UNIT_ADC_ILG                     11469L   /* long current from ADC*/
    #define VP880_UNIT_DECHZ                       17896L   /* Deci Hz*/
    #define VP880_UNIT_MHZ                         179L     /* mHz */
    #define VP880_UNIT_FREQ                        65536L   /* siggen freq step */
    #define VP880_UNIT_RAMP_TIME                   1365L    /* Ramp time (-1/3) in ms*/

    #define VP880_UNIT_CONVERT(DATA, I_UNIT, O_UNIT)    (int32)(((int32)DATA * I_UNIT) / O_UNIT)

    /* converts requested slope into SigGen Freq */
    #define VP880_SLOPE_TO_FREQ(AMP,SLOPE)  (int16)(((int32)SLOPE * (int32)VP880_UNIT_RAMP_TIME) / (int32)AMP)

    #define VP880_AC_RLOOP_MAX_BAT_LVL              75000L /* mV */
    #define VP880_AC_RLOOP_MAX_TEST_LVL         ((VP880_AC_RLOOP_MAX_BAT_LVL - 5000) / 2)

#endif /*VP880_INCLUDE_TESTLINE_CODE*/


#ifdef VP_CC_890_SERIES

#ifndef VP890_FXS_SUPPORT
    /*
     * Tests (using testline) is supported only for FXS termination types. If
     * line tests are required, make sure FXS support is enabled.
     */
    #undef VP890_INCLUDE_TESTLINE_CODE

    /* LPM is an FXS termination type. FXS must be supported to support LPM. */
    #undef  VP890_LP_SUPPORT
#endif

/* Define this to attempt to reduce the level of the noise signal generated for
 * adaptive balance B-Filter calibration.  Reducing the level will increase the
 * negative effect of outside noise on the line and can reduce performance */
#undef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL

#endif /* VP_CC_890_SERIES*/

#if (defined (VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE)) || defined(VP_CC_KWRAP)
    /*
     * #defines used in unit conversion function
     * All units are based on the following calculations
     *      Internal voltage unit   = 480.0  / 2^31 Volts
     *      Internal current unit   = 240e-3 / 2^31 Amps
     *      Internal freq uint      = 12e3   / 2^31 Hz
     */
    #define VP890_UNIT_MV                          4474L    /* mV unit */
    #define VP890_UNIT_MVRMS                       6327L    /* mVrms unit */
    #define VP890_UNIT_ADC_VAB                     -32768L  /* vab step from ADC */
    #define VP890_UNIT_ADC_VAB_RMS                 46341L   /* vab step from ADC rms*/
    #define VP890_UNIT_ADC_AC_VAB_RMS              664L     /* vab step from ADC Metallic Vrms*/
    #define VP890_UNIT_DAC_RING                    21076L   /* step due to DAC in ringing*/
    #define VP890_UNIT_TRACK_SWZ                23091612L   /* step used for tracking switching regulator Z */
    #define VP890_UNIT_UA                          8948L    /* uA unit */
    #define VP890_UNIT_ADC_IMET_NOR                15756L   /* met I from ADC in normal*/
    #define VP890_UNIT_ADC_IMET_RING               32768L   /* met I from ADC in ringing*/
    #define VP890_UNIT_ADC_ILG                     11469L   /* long current from ADC*/
    #define VP890_UNIT_DECHZ                       17896L   /* Deci Hz*/
    #define VP890_UNIT_MHZ                         179L     /* mHz */
    #define VP890_UNIT_FREQ                        65536L   /* siggen freq step */
    #define VP890_UNIT_RAMP_TIME                   1365L    /* Ramp time (-1/3) in ms*/

    #define VP890_UNIT_CONVERT(DATA, I_UNIT, O_UNIT) (int32)( ((int32)DATA * I_UNIT) / O_UNIT)

    /* converts requested slope into SigGen Freq */
    #define VP890_SLOPE_TO_FREQ(AMP,SLOPE) (int16) ( ((int32)SLOPE * (int32)VP890_UNIT_RAMP_TIME) / (int32)AMP)

    #define VP890_AC_RLOOP_MAX_BAT_LVL              75000L /* mV */
    #define VP890_AC_RLOOP_MAX_TEST_LVL             ((VP890_AC_RLOOP_MAX_BAT_LVL - 5000) / 2)

#endif /* VP890_INCLUDE_TESTLINE_CODE */

/******************************************************************************
 * COMPILE-TIME OPTIONS:: Top level VP-API Default Device and Line Options    *
 *****************************************************************************/

 /* Debounce time to mask hook activity when a pol rev is generated */
#define VP_POLREV_DEBOUNCE_TIME     (100)

/* The following defines specify the default settings of the device and line
 * options used in the API */
#include "vp_api_option.h"
#include "vp_api_event.h"

/* Default Dial Pulse Options specified in units of 125micro sec  */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN          (33 * 8)    /* == 33 ms */
#define VP_OPTION_DEFAULT_DP_BREAK_MAX          (100 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN           (17 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX           (75 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN      (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN          (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX          (1300 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN        VP_OPTION_DEFAULT_DP_FLASH_MAX
#define VP_OPTION_DEFAULT_DP_OFF_HOOK_MIN       0

/*
 * Specification 2 is usefull if you need to detect between two non-overlapping
 * dial pulse specifications.
 */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_BREAK_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN2     (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN2       (0 * 8)

/* Default setting for Critical fault option */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN     TRUE
/* CSLAC-880 and VCP-880 do not support the detection of AC, DC critical faults.
 * So, The VP-API uses the following default values for those devices.
 * Note that changing the following values will result in VpInitDevice()
 * failure. */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN_880      FALSE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN_880      FALSE

/* Default setting for Zero cross option */
#define VP_OPTION_DEFAULT_ZERO_CROSS            VP_OPTION_ZC_M4B

/* Default setting for Ramp to standby option */
#define VP_OPTION_DEFAULT_RAMP2STBY             0

/* Default setting for Pulse mode option */
#define VP_OPTION_DEFAULT_PULSE_MODE            VP_OPTION_PULSE_DECODE_OFF

/* Default CODEC Mode */
#define VP_OPTION_DEFAULT_CODEC_MODE            VP_OPTION_ALAW

/* Default setting for PCM Highway option */
#define VP_OPTION_DEFAULT_PCM_HWY               VP_OPTION_HWY_A

/* Default setting for Loop back option */
#define VP_OPTION_DEFAULT_LOOP_BACK             VP_OPTION_LB_OFF

/* Default setting for Line state option */
#define VP_OPTION_DEFAULT_LS_BAT_REV            FALSE
#define VP_OPTION_DEFAULT_LS_BAT                VP_OPTION_BAT_AUTO

/*
 * Default Event Masks
 * Some events are non-maskable and will be modified by the API.  The user can
 * arbitrarily set the mask bits here as needed.  A mask bit set to '0' means
 * the event is not masked.
 * As a default mask all the events that are not defined in the version of
 * the API.
 */
#define VP_OPTION_DEFAULT_FAULT_EVENT_MASK    \
        (((VP_EVCAT_FAULT_MASK_ALL) | /* Please change ONLY this */ \
           VP_EVCAT_FAULT_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_FAULT_EVENTS)) /*Unmask non maskable events */

#define VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK  \
        (((VP_EVCAT_SIGNALING_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_SIGNALING_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_SIGNALING_EVENTS))

#define VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK   \
        (((VP_EVCAT_RESPONSE_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_RESPONSE_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_RESPONSE_EVENTS))

#define VP_OPTION_DEFAULT_TEST_EVENT_MASK       \
        (((VP_EVCAT_TEST_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_TEST_UNDEFINED) & /* Mask those that are not defined */  \
         (~VP_API_NONMASK_TEST_EVENTS))

#define VP_OPTION_DEFAULT_PROCESS_EVENT_MASK    \
        (((VP_EVCAT_PROCESS_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_PROCESS_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_PROCESS_EVENTS))

#define VP_OPTION_DEFAULT_FXO_EVENT_MASK        \
        (((VP_EVCAT_FXO_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_FXO_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_FXO_EVENTS))

#define VP_OPTION_DEFAULT_PACKET_EVENT_MASK     \
        (((VP_EVCAT_PACKET_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_PACKET_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_PACKET_EVENTS))

/* Default setting for Ring Control option */
#define VP_OPTION_DEFAULT_RC_ZERO_CROSS         VP_OPTION_ZC_M4B
#define VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL (100 * 8) /* 100ms */
#define VP_OPTION_DEFAULT_RC_RING_EXIT_STATE    VP_LINE_TALK

/* Default setting for DTMF Mode option */
#define VP_OPTION_DEFAULT_DM_DTMF_CONTROL_MODE  VP_OPTION_DTMF_DECODE_OFF
#define VP_OPTION_DEFAULT_DM_DIRECTION          VP_DIRECTION_US

/* Default setting for PCM TX/RX path control */
#define VP_OPTION_DEFAULT_PCM_TXRX_CNTRL        VP_OPTION_PCM_BOTH

/* Default setting for DTMF Spec option */
#define VP_OPTION_DEFAULT_DTMF_SPEC             VP_OPTION_DTMF_SPEC_ATT

/* Default setting for Park Mode option */
#define VP_OPTION_DEFAULT_PARK_MODE_DISC        4   /* 2 seconds */
#define VP_OPTION_DEFAULT_PARK_MODE_STANDBY     3   /* 300 ms */

/* Default setting for DC Feed Slope option */
#define VP_OPTION_DEFAULT_DCFEED_SLOPE          109 /* 4 V/ms */

/* Default setting for hook detection mode option */
#define VP_OPTION_DEFAULT_HOOK_DETECT_MODE      VP_OPTION_HOOKDET_NORMAL

/* Default settings for PCM highway signaling and control */
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_ENABLE    FALSE
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_CTLTS     0
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_SIGTS     0

/* Default setting for PCM highway lien state control mode */
#define VP_OPTION_DEFAULT_LINESTATE_CTL_MODE    VP_OPTION_LINESTATE_CTL_NORMAL

/******************************************************************************
 *    VCP2 library specific other defines and conditional compile flags       *
 ******************************************************************************/

/* Specify the maximum number of SLACs which will be controlled by the VCP2 in
 * this application.  If controlling fewer than 16 SLACs, you can reduce this
 * value to save memory. */
#define VP_VCP2_MAX_NUM_SLACS 16

/* Specify the maximum number of channels controlled by any individual SLAC.
 * If you are controlling a VCP2-792 device, this should be 8.  If you are
 * controlling a VCP2-790 device, you can reduce this value to 4 to save
 * memory.  If you are controlling a VCP2-880 device, you can reduce this value
 * to 2 to save memory.  If you are controlling a combination of
 * SLACs (unlikely), use the number corresponding to the SLAC with the most
 * channels. */
#define VP_VCP2_MAX_NUM_CHANS  8

/* Set this option to support the VCP2 emulator. */
#define VP_VCP2_EMU_SUPPORT

/* Do not define this unless you know what you're doing. */
#undef VP_VCP2_SUPPORT_UNDOCUMENTED_TESTS

/******************************************************************************
 *     VCP library specific other defines and conditional compile flags       *
 ******************************************************************************/

/* Define the following to enable debug print statements from the VCP API */
#undef DEBUG_VCP_API

#ifdef DEBUG_VCP_API
#include <stdio.h>
#include "hi_common_id.h"
#include "hi_common_log.h"
/* Define the following as necessary to redirect the debug output */
#define term_printf printf

/* Further classification enabling/disabling debug output */
#define DEBUG_BOOTLOAD              /* Debug bootload function */
#define DEBUG_VCP_API_MB            /* MailBox related debug output */
#define DEBUG_VCP_API_EVENT         /* Event masking related debug output */
#define DEBUG_VCP_API_DEFLT_OPTN    /* Default option debugging */
#endif /* DEBUG_VCP_API */

/******************************************************************************
 *                      API Debug output redirection                          *
 ******************************************************************************/
#if defined(DEBUG_VCP_API)
#ifndef term_printf
#include <stdio.h>
#include "hi_common_id.h"
#include "hi_common_log.h"
/* Define the following as necessary to redirect the debug output */
#define term_printf printf
#endif /* term_printf */
#endif /* DEBUG_VCP_API  */

#if defined (VP_CC_VCP_SERIES) || defined (VP_CC_VCP2_SERIES)
/******************************************************************************
 * Library Specific default options                                           *
 *****************************************************************************/
/* VoicePath API provides a mechanism where individual library's default
 * options could be different compared to top level API default options.
 * If you need such a change please review the file included below.
 */
#include "vp_api_lib_cfg.h"
#endif
#endif /* VP_API_CFG_INT_H */
