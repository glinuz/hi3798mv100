/** \file vp_api_timer.h
 * vp_api_timer.h
 *
 * Header file for all timer typedefs used in API-II (internal).
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 6874 $
 * $LastChangedDate: 2010-04-08 15:15:43 -0500 (Thu, 08 Apr 2010) $
 */

#ifndef VP_API_TIMER_H
#define VP_API_TIMER_H

/* IMT average for Voltage to Frequency (FXO) smoothing */
#define VP_IMT_AVG_LENGTH 2  /* FXO variable, but need this to compile */

#define VP_ACTIVATE_TIMER       0x8000
#define VP_TIMER_TIME_MASK      0x7FFF

typedef enum {
    VP_LINE_CID_DEBOUNCE,       /**< Debounce for known CID state changes */
    VP_LINE_TIMER_CID_DTMF,     /**< Timer for DTMF generation in CID */
    VP_LINE_TIMER_FAULT,        /**< The fault timer */
    VP_LINE_RING_EXIT_DEBOUNCE, /**< Line Debounce specifically for Ring Exit */
    VP_LINE_POLREV_DEBOUNCE,    /**< Hook Mask on Polarity Reversals */
    VP_LINE_DISCONNECT_EXIT,    /**< Line Debounce for Disconnect Recovery */
    VP_LINE_GND_START_TIMER,    /**< Timer for Recovery from Tip-Open */
    VP_LINE_CAL_LINE_TIMER,     /**< Timer for CalLine operations */
    VP_LINE_PING_TIMER,         /**< Timer to prevent phone "ping" */
    VP_LINE_OFFHOOK_DELAY,      /**< Offhook report delay when not pulse detecting */
    VP_LINE_TRACKER_DISABLE,    /**< Timer used to disable switcher with low
                                 * power termination type.
                                 */
    VP_LINE_CAL_TIMER,          /**< Timer for Cal operations */
    VP_LINE_GPIO_CLKOUT_TIMER,  /**< Timer to use a GPIO as a slow clock output */
    VP_LINE_INTERNAL_TESTTERM_TIMER,/**< Timer for the internal test termination */

    VP_LINE_SPEEDUP_RECOVERY_TIMER, /**< Timer to restore speedup recovery */

    VP_LINE_TIMER_LAST,         /**< Place holder to indicate number of line timers */
    VP_LINE_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineTimerType;

/**< Time after disconnect recovery for Hook mask */
#define VP_DISCONNECT_RECOVERY_TIME     100

typedef enum {
    VP_DEV_TIMER_TESTLINE,          /* Test line timer to ensure a more consistant
                                     * MPI PCM collect routine, leave this timer
                                     * as the first in the enumeration type */
    VP_DEV_TIMER_CLKFAIL,           /* Clock fail interrupt timer */
    VP_DEV_TIMER_ABSCAL,            /* ABS Calibration timer */
    VP_DEV_TIMER_LP_CHANGE,         /* Lower Power Mode Switcher Changes */
    VP_DEV_TIMER_ABV_CAL,           /* ABV Caibration  device timers */
    VP_DEV_TIMER_ENTER_RINGING,     /* In-rush limiting enter ringing device timers */
    VP_DEV_TIMER_EXIT_RINGING,      /* Used to enable Low Power Switching */
    VP_DEV_TIMER_WB_MODE_CHANGE,    /* Used to mask clock faults */
    VP_DEV_TIMER_LAST,              /* Place holder to indicate number of device timers */
    VP_DEV_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDevTimerType;

#define VP_DEV_TIMER_EXIT_RINGING_SAMPLE    (5000)
#define VP_WB_CHANGE_MASK_TIME  (20)

/** FXO specific timer variables to be used for FXO type lines only */
typedef enum {
    VP_CSLAC_FXS_TIMER,
    VP_CSLAC_FXO_TIMER,
    VP_CSLAC_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCslacTimerType;


#if (defined (VP_CC_880_SERIES) && defined (VP880_FXO_SUPPORT)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXO_SUPPORT)) || \
     defined (VP_CC_580_SERIES) || defined (VP_CC_KWRAP)
typedef struct {
    uint16  highToLowTime;      /**< Device timestamp of last high to low change */
    uint16  prevHighToLowTime;
    bool    noCount;            /**< TRUE when not counting - lack of activity */
    bool    lastState;          /**< TRUE if last known high, FALSE if low */
    uint16  timeLastPolRev;     /**< Time in 0.25ms since polrev detected */
    uint16  timePrevPolRev;     /**< Time in 0.25ms since prev polrev detected */
    uint8   maxPeriod;          /**< Time in 0.25ms that ringing is detected */
    uint16  lastStateChange;    /**< Time in 1mS since last state change */
    uint16  lastNotLiu;         /**< Time in 1mS since last Not LIU detected */
    uint16  disconnectDebounce; /**< Time in 1mS to debounce disconnect events */
    uint16  disconnectDuration; /**< Time in ticks that disconnect is detected */
    uint8   liuDebounce;        /**< Time in 1mS to debounce LIU detection that
                                     accompanies some kinds of ringing */
    uint8   ringOffDebounce;    /**< Time in 1mS to ignore disconnect after ring_off */
    uint8   ringTimer;          /**< When this timer expires, ringing is off */
    uint8   cidCorrectionTimer; /**< 1mS increments for CID correction timing */
    uint8   bCalTimer;          /**< 1mS increments for BFilter sampling time */
    uint16  fxoDiscIO2Change;   /**< Countdown in 1ms since IO2 changed for FXO_DISC termType */
    uint16  pllRecovery;        /**< Timer in ticks to recover PLL when FXO Disconnect to OHT */
    uint16  currentMonitorTimer;/**< Used to provide FXO line current buffer */
    uint16  measureBFilterTimer;/**< 1mS increments for BFilter sampling time */
    uint8   lowVoltageTimer;    /**< Timer in ticks for the low voltage
                                     disconnect/LIU distinction workaround */
} VpFXOTimerType;
#endif

/** Union of FXO and FXS timers since only one type can exist for each line */
typedef union {
#if (defined (VP_CC_880_SERIES) && defined (VP880_FXO_SUPPORT)) || \
    (defined (VP_CC_890_SERIES) && defined (VP890_FXO_SUPPORT)) || \
     defined (VP_CC_580_SERIES) || defined (VP_CC_KWRAP)
    VpFXOTimerType fxoTimer;
#endif
    uint16 timer[VP_LINE_TIMER_LAST]; /**< FXS Array of timers */
} VpCslacTimers;

typedef struct {
    VpCslacTimerType type;
    VpCslacTimers timers;
} VpCslacTimerStruct;
#endif
