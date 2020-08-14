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
* @file si_lib_time_api.h
*
* @brief Time Library 
*
*****************************************************************************/

#ifndef __SI_LIB_TIME_API_H__
#define __SI_LIB_TIME_API_H__

/***** #include statements ***************************************************/

#include "si_datatypes.h"

/***** public macro definitions **********************************************/

/***** public type definitions ***********************************************/

typedef uint32_t SiiLibTimeMicro_t;
typedef uint32_t SiiLibTimeMilli_t;

/***** public functions ******************************************************/

//-------------------------------------------------------------------------------------------------
//! @brief      Returns number of passed milli seconds since TimeInit().
//!
//! @return     Number of milli seconds.
//-------------------------------------------------------------------------------------------------
SiiLibTimeMilli_t SiiLibTimeMilliGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Calculates time difference between t1 and t2 and prevent result from roll-over 
//!             corruption (t1<t2).
//!
//! @param[in]  t1 - Number of milli seconds.
//! @param[in]  t2 - Number of milli seconds.
//!
//! @return     Number of milli seconds.
//-------------------------------------------------------------------------------------------------
SiiLibTimeMilli_t SiiLibTimeMilliDiffGet( SiiLibTimeMilli_t t1, SiiLibTimeMilli_t t2 );

//-------------------------------------------------------------------------------------------------
//! @brief      Configures a milli time-out object. This object can be used with 'SiiLibTimeOutMilliIs'
//!             to find out if 'MilliTO' object has been expired.
//!             example:
//!             {
//!                 SiiLibTimeMilli_t MilliTO;
//!
//!                 SiiLibTimeOutMilliSet(&MilliTO, 100);
//!                 while( !SiiLibTimeOutMilliIs(MilliTO) )
//!                 { .... }
//!             }
//!
//! @param[in]  pMilliTO - pointer to 'MilliTO' object.
//! @param[in]  timeOut  - Number of milli seconds.
//-------------------------------------------------------------------------------------------------
void             SiiLibTimeOutMilliSet( SiiLibTimeMilli_t* pMilliTO, SiiLibTimeMilli_t timeOut );

//-------------------------------------------------------------------------------------------------
//! @brief      Finds out if 'MillTO' object has been expired.
//!
//! @param[in]  pMilliTO - pointer to 'MilliTO' object.
//!
//! @return     true if 'MilliTO' object has been expired.
//-------------------------------------------------------------------------------------------------
bool_t           SiiLibTimeOutMilliIs( const SiiLibTimeMilli_t* pMilliTO );

//-------------------------------------------------------------------------------------------------
//! @brief      Blocks execution for x number of milli seconds.
//!
//! @param[in]  milliDelay - Number of milli seconds.
//-------------------------------------------------------------------------------------------------
void             SiiLibTimeMilliDelay( SiiLibTimeMilli_t milliDelay );

#endif /* __SI_LIB_TIME_API_H__ */

/***** end of file ***********************************************************/
