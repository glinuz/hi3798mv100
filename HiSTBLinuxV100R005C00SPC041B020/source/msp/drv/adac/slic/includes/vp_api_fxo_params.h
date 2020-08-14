/** \file vp_api_fxo_params.h
 * vp_api_fxo_params.h
 *
 * This file contains the Zarlink Semiconductor approved system parameters for proper
 * FXO operation on the reference designs
 *
 * $Revision: 5442 $
 * $LastChangedDate: 2009-08-17 12:13:24 -0500 (Mon, 17 Aug 2009) $
 */

#ifndef VP_API_FXO_PARAMS_H
#define VP_API_FXO_PARAMS_H

#define VP_FXO_DTMF_ON_DEFAULT          0x0027  /* DTMF On = 195mS */
#define VP_FXO_DTMF_OFF_DEFAULT         0x0028  /* DTMF Off = 200mS */
#define VP_FXO_FLASH_HOOK_DEFAULT       0x0078  /* Flash Hook = 600mS */
#define VP_FXO_PULSE_BREAK_DEFAULT      0x0C    /* Pulse Break = 60mS */
#define VP_FXO_PULSE_MAKE_DEFAULT       0x08    /* Pulse Make = 40mS */
#define VP_FXO_INTERDIG_DEFAULT         0x0028  /* Interdigit Period = 200mS */
#define VP_FXO_RING_DET_MAX_DEFAULT     0xEB    /* Ring Period Max = 58.75mS */
#define VP_FXO_RING_DET_MIN_DEFAULT     0x79    /* Ring Period Min = 30.25mS */
#define VP_FXO_RING_DET_VOLT_DEFAULT    0x02    /* Ring Voltage Min = 38V */
#define VP_FXO_DISC_DET_MIN_DEFAULT     0x03    /* Disconnect Det Min = 4.2V */
#define VP_FXO_LIU_DET_THRESH_DEFAULT   0x26    /* Line-In-Use Det Threshold = 38V */

/* In ms, time that FXO detector activity is ignored after a state change */
#define VP_FXO_STATE_CHANGE_DEBOUNCE    (50)

/* In ms, time that FXO detector activity is ignored after a ring trip */
#define VP_FXO_RING_TRIP_DEBOUNCE       (200)

/* In ms, time that FXO disconnect detector is debounced */
#define VP_FXO_DISCONNECT_DEBOUNCE      (300)

/* In ms, time that FXO will change to loop open if detecting disconnect */
#define VP_FXO_DISC_TO_LOOP_OPEN        (5000)

typedef enum
{
    VP_FXO_DIALING_PROFILE_DTMF_ON_MSB = 6,
    VP_FXO_DIALING_PROFILE_DTMF_ON_LSB = 7,
    VP_FXO_DIALING_PROFILE_DTMF_OFF_MSB = 8,
    VP_FXO_DIALING_PROFILE_DTMF_OFF_LSB = 9,
    VP_FXO_DIALING_PROFILE_FLASH_HOOK_MSB = 10,
    VP_FXO_DIALING_PROFILE_FLASH_HOOK_LSB = 11,
    VP_FXO_DIALING_PROFILE_PULSE_BREAK = 12,
    VP_FXO_DIALING_PROFILE_PULSE_MAKE = 13,
    VP_FXO_DIALING_PROFILE_INTERDIGIT_MSB = 14,
    VP_FXO_DIALING_PROFILE_INTERDIGIT_LSB = 15,
    VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX = 16,
    VP_FXO_DIALING_PROFILE_RING_PERIOD_MIN = 17,
    VP_FXO_DIALING_PROFILE_RING_VOLTAGE_MIN = 18,
    VP_FXO_DIALING_PROFILE_DISC_VOLTAGE_MIN = 19,
    VP_FXO_DIALING_PROFILE_LIU_THRESHOLD_MIN = 20,
    VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX_ACT = 21,
    VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_MSB = 22,
    VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_LSB = 23,
    VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_MSB = 24,
    VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_LSB = 25,
    VP_FXO_DIALING_PROFILE_POH_INT_TIME = 26,
    VP_FXO_DIALING_PROFILE_POH_THRESHOLD = 27,
    VP_FXO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp_fxo_dialingProfileParams;
#endif



