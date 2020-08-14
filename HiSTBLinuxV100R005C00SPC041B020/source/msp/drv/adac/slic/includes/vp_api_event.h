/** \file vp_api_event.h
 * vp_api_event.h
 *
 * This file contains declaration associated with VP-API events.
 *
 * $Revision: 7191 $
 * $LastChangedDate: 2010-05-19 10:51:29 -0500 (Wed, 19 May 2010) $
 */

#ifndef VP_API_EVENT_H
#define VP_API_EVENT_H

#include "vp_api_types.h"

#define VP_EVENT_MASK_ALL       0xFFFF

/* Event category type */
typedef enum {
    VP_EVCAT_FAULT     = 0,
    VP_EVCAT_SIGNALING = 1,
    VP_EVCAT_RESPONSE  = 2,
    VP_EVCAT_TEST      = 3,
    VP_EVCAT_PROCESS   = 4,
    VP_EVCAT_FXO       = 5,
    VP_EVCAT_PACKET    = 6,
    VP_NUM_EVCATS,
    VP_EVCAT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpEventCategoryType;

typedef struct {
    uint16 faults;      /**< Fault event mask */
    uint16 signaling;   /**< Signaling event mask */
    uint16 response;    /**< Mailbox response event mask */
    uint16 test;        /**< Test events */
    uint16 process;     /**< Call process related events */
    uint16 fxo;         /**< FXO events mask */
    uint16 packet;      /**< Packet events mask - no longer used */
} VpOptionEventMaskType;

typedef enum {
    VP_DEV_EVID_BAT_FLT         = 0x0001, /**< Device detected battery fault */
    VP_DEV_EVID_CLK_FLT         = 0x0002, /**< Device detected clock fault */
    VP_LINE_EVID_THERM_FLT      = 0x0004, /**< Thermal Overload condition */
    VP_LINE_EVID_DC_FLT         = 0x0008, /**< DC Fault detected on line */
    VP_LINE_EVID_AC_FLT         = 0x0010, /**< AC Fault detected on line */
    VP_LINE_EVID_SYNC_FLT       = 0x0020, /**< SLAC Synchronization fault */
    VP_LINE_EVID_RES_LEAK_FLT   = 0x0040, /**< Low loop resistance while on-hook */
    VP_DEV_EVID_WDT_FLT         = 0x2000, /**< Watchdog timer fault */
    VP_DEV_EVID_EVQ_OFL_FLT     = 0x4000, /**< event queue overflow fault */
    VP_DEV_EVID_SYSTEM_FLT      = 0x8000, /**< VCP2 system fault */
    /* The following fault events are device-specific, not line-specific */
    VP_EVCAT_FAULT_DEV_EVENTS   = (int)(VP_DEV_EVID_BAT_FLT     |
                                        VP_DEV_EVID_CLK_FLT     |
                                        VP_DEV_EVID_EVQ_OFL_FLT |
                                        VP_DEV_EVID_WDT_FLT     |
                                        VP_DEV_EVID_SYSTEM_FLT),
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_FAULT_UNDEFINED    = (int)(~(VP_DEV_EVID_BAT_FLT     |
                                          VP_DEV_EVID_CLK_FLT     |
                                          VP_LINE_EVID_THERM_FLT  |
                                          VP_LINE_EVID_DC_FLT     |
                                          VP_LINE_EVID_AC_FLT     |
                                          VP_LINE_EVID_SYNC_FLT   |
                                          VP_LINE_EVID_RES_LEAK_FLT |
                                          VP_DEV_EVID_WDT_FLT     |
                                          VP_DEV_EVID_EVQ_OFL_FLT |
                                          VP_DEV_EVID_SYSTEM_FLT)), /* NOTES:
    * The typecasting (int) is necessary in some compilers that have
    * sizeof(enum)  == sizeof(int) == 2bytes and also because of 'C' rules for
    * hexadecimal constants. Hexadecimal constants are treated as unsigned and
    * enum variables are of type signed int. Thus compilers generate warning
    * when code attempts to assign number that is bigger than what a signed enum
    * can hold */

    VP_EVCAT_FAULT_UNMASK_ALL   = (int)(VP_EVENT_MASK_ALL &
                                         VP_EVCAT_FAULT_UNDEFINED),
    VP_EVCAT_FAULT_MASK_ALL     = (int)(~VP_EVCAT_FAULT_UNDEFINED),
    VP_EVCAT_FAULT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpFaultEventType;

typedef enum {
    VP_LINE_EVID_HOOK_OFF       = 0x0001, /**< Off-Hook detected */
    VP_LINE_EVID_HOOK_ON        = 0x0002, /**< On-Hook detected */
    VP_LINE_EVID_GKEY_DET       = 0x0004, /**< Ground Key detected */
    VP_LINE_EVID_GKEY_REL       = 0x0008, /**< Ground Key release detected */
    VP_LINE_EVID_FLASH          = 0x0010, /**< Hook switch flash detected */
    VP_LINE_EVID_STARTPULSE     = 0x0020, /**< Start of a dial pulse detected */
    VP_LINE_EVID_DTMF_DIG       = 0x0040, /**< New DTMF digit detected */
    VP_LINE_EVID_PULSE_DIG      = 0x0080, /**< New pulse digit detected */
    VP_LINE_EVID_MTONE          = 0x0100, /**< Modem tone detected */
    VP_DEV_EVID_TS_ROLLOVER     = 0x0200, /**< Timestamp rollover */

    VP_LINE_EVID_US_TONE_DETECT = 0x0400, /**< Upstream tone detection event */
    VP_LINE_EVID_DS_TONE_DETECT = 0x0800, /**< Downstream tone detection event*/
    VP_DEV_EVID_SEQUENCER       = 0x1000, /**< Sequencer Event */
    VP_LINE_EVID_BREAK_MAX      = 0x2000, /**< Break Max time has been exceeded */
    VP_LINE_EVID_EXTD_FLASH     = 0x4000, /**< Flash Hook between flash max and on-hook min */
    VP_LINE_EVID_HOOK_PREQUAL   = 0x8000, /**< debounced off/on hook detected, but < offHookMin */

    /* The following signaling events are device-specific, not line-specific */
    VP_EVCAT_SIGNALING_DEV_EVENTS  = (int)(VP_DEV_EVID_TS_ROLLOVER |
                                            VP_DEV_EVID_SEQUENCER),
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_SIGNALING_UNDEFINED   = (int)(~(VP_LINE_EVID_HOOK_OFF          |
                                             VP_LINE_EVID_HOOK_ON           |
                                             VP_LINE_EVID_GKEY_DET          |
                                             VP_LINE_EVID_GKEY_REL          |
                                             VP_LINE_EVID_FLASH             |
                                             VP_LINE_EVID_STARTPULSE        |
                                             VP_LINE_EVID_DTMF_DIG          |
                                             VP_LINE_EVID_PULSE_DIG         |
                                             VP_LINE_EVID_MTONE             |
                                             VP_DEV_EVID_TS_ROLLOVER        |
                                             VP_LINE_EVID_US_TONE_DETECT    |
                                             VP_LINE_EVID_DS_TONE_DETECT    |
                                             VP_DEV_EVID_SEQUENCER          |
                                             VP_LINE_EVID_BREAK_MAX         |
                                             VP_LINE_EVID_EXTD_FLASH        |
                                             VP_LINE_EVID_HOOK_PREQUAL)),

    VP_EVCAT_SIGNALING_UNMASK_ALL   = (int)(VP_EVENT_MASK_ALL &
                                             VP_EVCAT_SIGNALING_UNDEFINED),
    VP_EVCAT_SIGNALING_MASK_ALL     = (int)(~VP_EVCAT_SIGNALING_UNDEFINED),
    VP_EVCAT_SIG_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpSignalingEventType;

typedef enum {
    VP_DEV_EVID_BOOT_CMP        = 0x0001, /**< VCP Startup sequence has
                                           * completed; Non-maskable
                                           */
    VP_LINE_EVID_LLCMD_TX_CMP   = 0x0002, /**< Low level write transaction has
                                           * completed
                                           */
    VP_LINE_EVID_LLCMD_RX_CMP   = 0x0004, /**< Low Level read transaction has
                                           * completed
                                           */
    VP_DEV_EVID_DNSTR_MBOX      = 0x0008, /**< Dowstream Mailbox event ready */
    VP_LINE_EVID_RD_OPTION      = 0x0010, /**< Read Option request complete */
    VP_LINE_EVID_RD_LOOP        = 0x0020, /**< Read Loop Conditions is complete
                                           */
    VP_EVID_CAL_CMP             = 0x0040, /**< Device or Line Calibration is
                                           * complete
                                           */
    VP_EVID_CAL_BUSY            = 0x0080, /**< Calibration not performed, line
                                           * busy
                                           */
    VP_LINE_EVID_GAIN_CMP       = 0x0100, /**< Gain Adjustment complete */
    VP_LINE_EVID_QUERY_CMP      = 0x0200, /**< Query complete */
    VP_DEV_EVID_DEV_INIT_CMP    = 0x0400, /**< Device Init complete */
    VP_LINE_EVID_LINE_INIT_CMP  = 0x0800, /**< Line Init complete */
    VP_DEV_EVID_IO_ACCESS_CMP   = 0x1000, /**< Device Access complete */
    VP_LINE_EVID_LINE_IO_RD_CMP = 0x2000, /**< Line GPIO read complete */
    VP_LINE_EVID_LINE_IO_WR_CMP = 0x4000, /**< Line GPIO write complete */
    VP_LINE_EVID_SLAC_INIT_CMP  = 0x8000, /**< SLAC Init complete */
    /* The following response events are device-specific, not line-specific */
    VP_EVCAT_RESPONSE_DEV_EVENTS = (int)(VP_DEV_EVID_BOOT_CMP       |
                                        VP_DEV_EVID_DNSTR_MBOX      |
                                        VP_DEV_EVID_DEV_INIT_CMP    |
                                        VP_DEV_EVID_IO_ACCESS_CMP),
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_RESPONSE_UNDEFINED = (int)(~(VP_DEV_EVID_BOOT_CMP          |
                                          VP_LINE_EVID_LLCMD_TX_CMP     |
                                          VP_LINE_EVID_LLCMD_RX_CMP     |
                                          VP_DEV_EVID_DNSTR_MBOX        |
                                          VP_LINE_EVID_RD_OPTION        |
                                          VP_LINE_EVID_RD_LOOP          |
                                          VP_EVID_CAL_CMP               |
                                          VP_EVID_CAL_BUSY              |
                                          VP_LINE_EVID_GAIN_CMP         |
                                          VP_LINE_EVID_QUERY_CMP        |
                                          VP_DEV_EVID_DEV_INIT_CMP      |
                                          VP_LINE_EVID_LINE_INIT_CMP    |
                                          VP_DEV_EVID_IO_ACCESS_CMP     |
                                          VP_LINE_EVID_LINE_IO_RD_CMP   |
                                          VP_LINE_EVID_LINE_IO_WR_CMP   |
                                          VP_LINE_EVID_SLAC_INIT_CMP)),

    VP_EVCAT_RESPONSE_UNMASK_ALL   = (int)(VP_EVENT_MASK_ALL &
                                            VP_EVCAT_RESPONSE_UNDEFINED),
    VP_EVCAT_RESPONSE_MASK_ALL     = (int)(~VP_EVCAT_RESPONSE_UNDEFINED),
    VP_EVCAT_RES_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpResponseEventType;

typedef enum
{
    VP_LINE_EVID_TEST_CMP   = 0x0001, /**< A requested test has been completed*/
    VP_LINE_EVID_TEST_RSVD1 = 0x0002, /**< Reserved event */
    VP_LINE_EVID_DTONE_DET  = 0x0004, /**< Dial-Tone Test - Dialtone Detected */
    VP_LINE_EVID_DTONE_LOSS = 0x0008, /**< Dial-Tone Test - Dialtone Lost */
    VP_DEV_EVID_STEST_CMP   = 0x0010, /**< Self Test Function complete */
    VP_DEV_EVID_CHKSUM      = 0x0020, /**< Code-space checksum is complete */
    VP_LINE_EVID_ABORT      = 0x0040, /**< Event requesting abort test */
    /* The following test events are device-specific, not line-specific */
    VP_EVCAT_TEST_DEV_EVENTS = (int)(VP_DEV_EVID_CHKSUM),
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_TEST_UNDEFINED =    (int)(~(VP_LINE_EVID_TEST_CMP       |
                                         VP_LINE_EVID_DTONE_DET      |
                                         VP_LINE_EVID_DTONE_LOSS     |
                                         VP_DEV_EVID_STEST_CMP       |
                                         VP_DEV_EVID_CHKSUM)),

    VP_EVCAT_TEST_UNMASK_ALL    =  (int)(VP_EVENT_MASK_ALL &
                                          VP_EVCAT_TEST_UNDEFINED),
    VP_EVCAT_TEST_MASK_ALL      =  (int)(~VP_EVCAT_TEST_UNDEFINED),
    VP_EVCAT_TEST_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpTestEventType;

typedef enum
{
    VP_LINE_EVID_MTR_CMP    = 0x0001,   /**< Metering Signal complete */
    VP_LINE_EVID_MTR_ABORT  = 0x0002,   /**< Metering Signal aborted */
    VP_LINE_EVID_CID_DATA   = 0x0004,   /**< Caller ID Transmission buffer is
                                         * empty
                                         */
    VP_LINE_EVID_RING_CAD   = 0x0008,   /**< Ring Cadence State change
                                         * information
                                         */
    VP_LINE_EVID_SIGNAL_CMP = 0x0010,   /**< Signal Generation complete */
    VP_LINE_EVID_MTR_CAD    = 0x0020,   /**< Metering cadence */

    VP_LINE_EVID_TONE_CAD   = 0x0040,   /**< Tone Cadence complete */

    VP_LINE_EVID_MTR_ROLLOVER = 0x0080, /**< Metering count rollover */
    VP_LINE_EVID_GEN_TIMER  = 0x0100,   /**< Timer complete / canceled / error */
    VP_LINE_EVID_USER       = 0x0200,   /**< User-defined event */

    /* There are no device-specific process events */
    VP_EVCAT_PROCESS_DEV_EVENTS = 0,

    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_PROCESS_UNDEFINED =  (int)(~(VP_LINE_EVID_MTR_CMP        |
                                          VP_LINE_EVID_MTR_ABORT      |
                                          VP_LINE_EVID_CID_DATA       |
                                          VP_LINE_EVID_RING_CAD       |
                                          VP_LINE_EVID_SIGNAL_CMP     |
                                          VP_LINE_EVID_MTR_CAD        |
                                          VP_LINE_EVID_TONE_CAD       |
                                          VP_LINE_EVID_MTR_ROLLOVER   |
                                          VP_LINE_EVID_GEN_TIMER      |
                                          VP_LINE_EVID_USER)),

    VP_EVCAT_PROCESS_UNMASK_ALL =  (int)(VP_EVENT_MASK_ALL &
                                          VP_EVCAT_PROCESS_UNDEFINED),
    VP_EVCAT_PROCESS_MASK_ALL   =  (int)(~VP_EVCAT_PROCESS_UNDEFINED),
    VP_EVCAT_PRO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProcessEventType;

typedef enum {
    /* FXO API Event Types */
    VP_LINE_EVID_RING_ON    = 0x0001, /**< Ringing detected on line */
    VP_LINE_EVID_RING_OFF   = 0x0002, /**< Ringing previously detected has
                                       * stopped
                                       */
    VP_LINE_EVID_LIU        = 0x0004, /**< Line In Use by adjacent line off
                                       * hook
                                       */
    VP_LINE_EVID_LNIU       = 0x0008, /**< Line In Use by adjacent line on
                                       * hook */
    VP_LINE_EVID_FEED_DIS   = 0x0010, /**< Line feed disabled (or too low) */
    VP_LINE_EVID_FEED_EN    = 0x0020, /**< Line feed enabled */
    VP_LINE_EVID_DISCONNECT = 0x0040, /**< Line feed removed while loop close */
    VP_LINE_EVID_RECONNECT  = 0x0080, /**< Line feed connected while loop
                                       * close
                                       */
    VP_LINE_EVID_POLREV     = 0x0100, /**< Line polarity reversal has occurred*/
    VP_LINE_EVID_POH        = 0x0200, /**< Parallel line off-hook */
    VP_LINE_EVID_PNOH       = 0x0400, /**< Parallel line on-hook */

    /* There are no device-specific FXO events */
    VP_EVCAT_FXO_DEV_EVENTS = 0,
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_FXO_UNDEFINED =     (int)(~(VP_LINE_EVID_RING_ON        |
                                         VP_LINE_EVID_RING_OFF       |
                                         VP_LINE_EVID_LIU            |
                                         VP_LINE_EVID_LNIU           |
                                         VP_LINE_EVID_FEED_DIS       |
                                         VP_LINE_EVID_FEED_EN        |
                                         VP_LINE_EVID_DISCONNECT     |
                                         VP_LINE_EVID_RECONNECT      |
                                         VP_LINE_EVID_POLREV         |
                                         VP_LINE_EVID_POH            |
                                         VP_LINE_EVID_PNOH)),

    VP_EVCAT_FXO_UNMASK_ALL =      (int)(VP_EVENT_MASK_ALL &
                                     VP_EVCAT_FXO_UNDEFINED),
    VP_EVCAT_FXO_MASK_ALL   =      (int)(~VP_EVCAT_FXO_UNDEFINED),
    VP_EVCAT_FXO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpFxoEventType;

typedef enum {
    VP_LINE_EVID_US_PKT_RDY     = 0x0001, /**< Upstream Packet ready event */
    VP_LINE_EVID_NEED_DS_PKT    = 0x0002, /**< Need Downstream Packet event */
    VP_LINE_EVID_PKT_ERROR      = 0x0004, /**< Packet type error event */
    VP_LINE_EVID_PKT_LOST       = 0x0008, /**< Packet loss event */
    VP_LINE_EVID_RD_PKT_STATS   = 0x0010, /**< Read packet statistics complete*/
    /* There are no device-specific packet events */
    VP_EVCAT_PACKET_DEV_EVENTS  = 0,
    /* Remaining undefined events should be masked to protect against
     * future changes */
    VP_EVCAT_PACKET_UNDEFINED =   (int)(~(VP_LINE_EVID_US_PKT_RDY       |
                                          VP_LINE_EVID_NEED_DS_PKT      |
                                          VP_LINE_EVID_PKT_ERROR        |
                                          VP_LINE_EVID_PKT_LOST         |
                                          VP_LINE_EVID_RD_PKT_STATS)),

    VP_EVCAT_PACKET_UNMASK_ALL  =   (int)(VP_EVENT_MASK_ALL &
                                          VP_EVCAT_PACKET_UNDEFINED),
    VP_EVCAT_PACKET_MASK_ALL    =  (int)(~VP_EVCAT_PACKET_UNDEFINED),
    VP_EVCAT_PKT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpPacketEventType;

/* Enumerations for interpreting event data in the VpGetEvent() field */
typedef enum {
    VP_BAT_FLT_NONE = 0x0,
    VP_BAT_FLT_BAT2 = 0x1,
    VP_BAT_FLT_BAT1 = 0x2,
    VP_BAT_FLT_BAT3 = 0x4,
    VP_BAT_FLT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBatFltEventDataType;

typedef enum {
    VP_RING_CAD_BREAK = 0,
    VP_RING_CAD_MAKE  = 1,
    VP_RING_CAD_DONE  = 2,
    VP_RING_CAD_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpRingCadEventDataType;

typedef enum {
    VP_CID_DATA_NEED_MORE_DATA = 0,
    VP_CID_DATA_TX_DONE        = 1,
    VP_CID_DATA_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCidDataEventDataType;

typedef enum {
    VP_DIG_1         = 0x01,
    VP_DIG_2         = 0x02,
    VP_DIG_3         = 0x03,
    VP_DIG_4         = 0x04,
    VP_DIG_5         = 0x05,
    VP_DIG_6         = 0x06,
    VP_DIG_7         = 0x07,
    VP_DIG_8         = 0x08,
    VP_DIG_9         = 0x09,
    VP_DIG_10        = 0x0A,    /* pulse only */
    VP_DIG_11        = 0x0B,    /* pulse only */
    VP_DIG_12        = 0x0C,    /* pulse only */
    VP_DIG_13        = 0x0D,    /* pulse only */
    VP_DIG_14        = 0x0E,    /* pulse only */
    VP_DIG_15        = 0x0F,    /* pulse only */
    VP_DIG_ZERO      = 0x0A,    /* DTMF only */
    VP_DIG_ASTER     = 0x0B,    /* DTMF only */
    VP_DIG_POUND     = 0x0C,    /* DTMF only */
    VP_DIG_A         = 0x0D,    /* DTMF only */
    VP_DIG_B         = 0x0E,    /* DTMF only */
    VP_DIG_C         = 0x0F,    /* DTMF only */
    VP_DIG_D         = 0x00,    /* DTMF only */
    VP_DIG_NONE      = 0xFF,
    VP_DIG_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_DIG_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDigitType;

/*
 * Indicates which set of Dial Pulse Specifications were met that is generating
 * the dial pulse event.
 */
typedef enum {
    VP_DP_PARAM1 = 0,
    VP_DP_PARAM2 = 1,
    VP_DP_PARAM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDialPulseParamType;

typedef enum {
    VP_DIG_SENSE_BREAK = 0x0000,
    VP_DIG_SENSE_MAKE  = 0x0010,
    VP_DIG_SENSE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDigitSenseType;

typedef enum {
    VP_POLREV_NORMAL  = 0, /* Line detected Reverse-to-Normal Transition */
    VP_POLREV_REVERSE = 1, /* Line detected Normal-to-Reverse Transition */
    VP_POLREV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpPolRevEventDataType;

typedef enum {
    VP_SYNC_MB_ERR        = 0x00,
    VP_SYNC_SEQ_BAD_INST  = 0x81, /* Unknown instruction in sequence */
    VP_SYNC_SEQ_BAD_BR    = 0x82, /* Sequencer encountered bad branch (either
                                   * a forward branch or too many nesting levels */
    VP_SYNC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpSyncFltEventDataType;

typedef enum {
    VP_GEN_TIMER_STATUS_CMP      = 0,
    VP_GEN_TIMER_STATUS_CANCELED = 1,
    VP_GEN_TIMER_STATUS_RESRC_NA = 2,
    VP_GEN_TIMER_STATUS_UNKNOWN  = 3,
    VP_GEN_TIMER_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpGenTimerStatusType;

typedef uint16 VpHookPrequalEventDataType;
#define VP_HOOK_PREQUAL_START   (0) /* Debounced off-hook detected, but offHookMin > 0 */
#define VP_HOOK_PREQUAL_ABORT   (1) /* Debounced on-hook detected, but duration < offHookMin */


/**
 * Non-maskable events are (with reasons):
 *
 *   Boot Complete -- Cannot mask.  Masking events requires that the boot has
 *                   been completed.
 *   Read Option -- To modify mask interrupts requires that the current mask
 *                  options can be read.  The only way to know the read option
 *                  is ready is via an unmasked Read Option Complete event.
 *   Device Init -- Fundamental event to know when device is initialized and
 *                  remaining options/line configurations can be set.  There
 *                  is no other standard "timeout" mechanism to know when the
 *                  device has been initialized properly.
 *   Line Init -- Fundamental event to know when a line is initialized and ready
 *                to be setup with remaining user defined options.  There is no
 *                other standard "timeout" mechanism to know when the line has
 *                been initialized properly.
 *   Events that have responses -- These events are non-maskable. If an event
 *                that has a response is masked, and if the response is not
 *                read, in case of CSLAC devices, it blocks response buffer.
 *                In case of VCP class of devices, response mailbox is
 *                locked up. If this is followed by another command that
 *                requires the response mailbox, that command can't complete
 *                execution since the response mailbox is busy. Now no more
 *                commands can be issued because the previous command can't
 *                complete.
 */

#define VP_API_NONMASK_FAULT_EVENTS         0x0000
#define VP_API_NONMASK_SIGNALING_EVENTS     0x0000

#define VP_API_NONMASK_RESPONSE_EVENTS      (VP_DEV_EVID_BOOT_CMP \
                                           | VP_LINE_EVID_LLCMD_RX_CMP \
                                           | VP_LINE_EVID_RD_OPTION \
                                           | VP_LINE_EVID_RD_LOOP \
                                           | VP_LINE_EVID_GAIN_CMP \
                                           | VP_DEV_EVID_DEV_INIT_CMP \
                                           | VP_LINE_EVID_LINE_INIT_CMP \
                                           | VP_DEV_EVID_IO_ACCESS_CMP \
                                           | VP_LINE_EVID_LINE_IO_RD_CMP \
                                           | VP_LINE_EVID_SLAC_INIT_CMP)

#define VP_API_NONMASK_TEST_EVENTS          (VP_LINE_EVID_TEST_CMP \
                                           | VP_DEV_EVID_CHKSUM)

#define VP_API_NONMASK_PROCESS_EVENTS       0x0000
#define VP_API_NONMASK_FXO_EVENTS           0x0000
#define VP_API_NONMASK_PACKET_EVENTS        0x0000

#endif /* VP_API_EVENT_H */
