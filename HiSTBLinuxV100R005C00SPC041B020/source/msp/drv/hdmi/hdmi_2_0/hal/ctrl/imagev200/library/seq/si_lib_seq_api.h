/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_lib_seq.h
*
* @brief Sequencer (provides timers)
*
*****************************************************************************/

#ifndef __SI_LIB_SEQ_API_H__
#define __SI_LIB_SEQ_API_H__

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "si_lib_time_api.h"
#include "si_lib_obj_api.h"
#include "si_lib_log_api.h"

/***** public macro definitions **********************************************/

#define SII_LIB_SEQ_TIMER_CREATE(pstr, pfunc, inst, prior, timerid)             SiiLibSeqTimerCreate(pstr, pfunc, inst, prior, timerid)

/***** public type definitions ***********************************************/

typedef uint16_t SiiLibSeqTimeMs_t;
typedef uint8_t  SiiLibSeqPriority_t;
typedef void (*SiiLibSeqCbFunc_t)( SiiInst_t parentInst );

typedef enum
{
    SII_TIMER_CPI,
    SII_TIMER_TPG,
    SII_TIMER_TX_INT,
    SII_TIMER_SCALER,
    SII_TIMER_HDCP,
    SII_TIMER_HWUPDATE,
    SII_TIMER_AVMUTE,
    SII_TIMER_SCR_WAIT_E,
    SII_TIMER_SCR_RP,
    SII_TIMER_SCDC_IP,
    SII_TIMER_BLUESCRN,
    SII_TIMER_ZERO_DRM_INFO,
    SII_TIMER_HDR_MODE_CHANGE,
    SII_TIMER_CEC_AUTO_PING,
}SiiTimerId_t;

/***** public functions ******************************************************/

//-------------------------------------------------------------------------------------------------
//! @brief      Scheduler suspend/resume control.
//!
//! @param[in]  isTrue
//!             - true  Enable suspend mode
//!             - false Resume scheduler
//-------------------------------------------------------------------------------------------------
void SiiLibSeqSetSuspendMode( bool_t isTrue );

//-------------------------------------------------------------------------------------------------
//! @brief      Suspend suspend/resume status.
//!
//! @retval     true  Scheduler is suspended
//! @retval     false Scheduler is active
//-------------------------------------------------------------------------------------------------
bool_t SiiLibSeqIsSuspendMode( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Create timer handler.
//!
//! @param[in]  pNameStr    Name of timer
//! @param[in]  c51bank     c51 memory bank pointer of where call-back function resides
//! @param[in]  cbFunc      Function pointer of local handler function
//! @param[in]  parentInst  Pointer reference to parent object
//! @param[in]  priority    Priority (0..255) 0=highest, 255=lowest
//! @param[in]  enTimerId   timer id
//! @return     Instance
//-------------------------------------------------------------------------------------------------
SiiInst_t SiiLibSeqTimerCreate( const char* pNameStr, SiiLibSeqCbFunc_t cbFunc, SiiInst_t parentInst, SiiLibSeqPriority_t priority, SiiTimerId_t enTimerId);

//-------------------------------------------------------------------------------------------------
//! @brief      Destroy timer handler.
//!
//! @param[in]  inst : Reference to instance
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerDelete( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Start timer.
//!
//! @param[in]  inst     Reference to instance
//! @param[in]  timeMs   Timer interval between this Start function and next call-back function.
//! @param[in]  periodMs Time interval of periodic timer (use '0' if one-shot timer)
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerStart( SiiInst_t inst, SiiLibSeqTimeMs_t timeMs, SiiLibSeqTimeMs_t periodMs );
void SiiLibSeqTimerStartIsr( SiiInst_t inst, SiiLibSeqTimeMs_t timeMs, SiiLibSeqTimeMs_t periodMs );

//-------------------------------------------------------------------------------------------------
//! @brief      Stop timer.
//!
//! @param[in]  inst Reference to instance
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerStop( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Stop all timers.
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerStopAll( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Timer running status.
//!
//! @param[in]  inst   Reference to instance
//!
//! @retval     true  Timer is running
//! @retval     false Timer is stopped
//-------------------------------------------------------------------------------------------------
bool_t SiiLibSeqTimerRunningIs( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Periodic time control.
//!
//! @param[in]  inst     Reference to instance
//! @param[in]  periodMs Periodic time in milliseconds
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerPeriodSet( SiiInst_t inst, SiiLibSeqTimeMs_t periodMs );

//-------------------------------------------------------------------------------------------------
//! @brief      Periodic time status.
//!
//! @param[in]  inst  Reference to instance
//!
//! @return     Periodic time in milliseconds
//-------------------------------------------------------------------------------------------------
SiiLibSeqTimeMs_t SiiLibSeqTimerPeriodGet( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Timer priority control.
//!
//! @param[in]  inst       Reference to instance
//! @param[in]  priority   0..255 (0=highest,255=lowest)
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerPrioritySet( SiiInst_t inst, SiiLibSeqPriority_t priority );

//-------------------------------------------------------------------------------------------------
//! @brief      Timer priority status.
//!
//! @param[in]  inst Reference to instance
//!
//! @return     Priority (0=highest....255=lowest).
//-------------------------------------------------------------------------------------------------
SiiLibSeqPriority_t SiiLibSeqTimerPriorityGet( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Timer real-time control.
//!
//!             If enabled and timer is used in repetitive mode the scheduler will preserve the
//!             the timer expiration frequency despite if timer handling is delayed by other timer
//!             handlers.
//!
//! @param[in]  inst     Reference to instance
//! @param[in]  bIsTrue  \c true, \c false
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTimerRealTimeSet( SiiInst_t inst, bool_t bIsTrue );

//-------------------------------------------------------------------------------------------------
//! @brief      Timer real-time status.
//!
//!             If enabled and timer is used in repetitive mode the scheduler will preserve the
//!             the timer expiration frequency despite if timer handling is delayed by other timer
//!             handlers.
//!
//! @param[in]  inst    Reference to instance
//!
//! @retval     true   Real-time mode is enabled
//! @retval     false  Real-time mode is disabled
//-------------------------------------------------------------------------------------------------
bool_t SiiLibSeqTimerRealTimeIs( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Provides over run time. Can only be called from timer call back function.
//!
//! @return     Overrun time in msec.
//-------------------------------------------------------------------------------------------------
SiiLibSeqTimeMs_t SiiLibSeqTimerOverRunTimeGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Abort sequencer task.
//!
//!             This function exits the sequencer task.
//-------------------------------------------------------------------------------------------------
void SiiLibSeqAbort( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Sequencer Handler.
//!
//!             Starts framework operations.
//!             This function won't exit.
//-------------------------------------------------------------------------------------------------
SiiLibSeqTimeMs_t SiiSequencerHandler( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Sequencer task.
//!
//!             Starts framework operations.
//!             This function won't exit.
//-------------------------------------------------------------------------------------------------
void SiiLibSeqTask( void );

extern struct semaphore g_avmuteTimerMute;

#endif // __SI_LIB_SEQ_API_H__

/***** end of file ***********************************************************/
