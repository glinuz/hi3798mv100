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
* @file si_lib_time.c
*
* @brief Time library
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "si_lib_obj_api.h"
#include "si_lib_time_api.h"
#include "platform_api.h"
#include "sii_time.h"

/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_time);

/***** local macro definitions ***********************************************/

#define MILLI_TO_MAX          (((SiiLibTimeMilli_t)~0)>>1)  /* Maximum milli out must be set to less than half the range of SiiSysTimeMilli_t */

/***** local type definitions ************************************************/

/***** local prototypes ******************************************************/

//static void sTimeOutMilliSet( SiiLibTimeMilli_t* pMilliTO, SiiLibTimeMilli_t timeOut );
//static bool_t sTimeOutMilliIs( const SiiLibTimeMilli_t* pMilliTO );

/***** local data objects ****************************************************/

/***** public functions ******************************************************/

#if 0 // currently not used.
SiiLibTimeMilli_t SiiLibTimeMilliDiffGet( SiiLibTimeMilli_t t1, SiiLibTimeMilli_t t2 )
{
	if( t2 < t1 )
		return((SiiLibTimeMilli_t)~0) - t1 + t2 + 1;
	else
		return t2 - t1;
}

void SiiLibTimeOutMilliSet( SiiLibTimeMilli_t* pMilliTO, SiiLibTimeMilli_t timeOut )
{
	sTimeOutMilliSet(pMilliTO, timeOut);
}

bool_t SiiLibTimeOutMilliIs( const SiiLibTimeMilli_t* pMilliTO )
{
	return sTimeOutMilliIs(pMilliTO);
}
#endif

void SiiLibTimeMilliDelay( SiiLibTimeMilli_t millDelay )
{
	SiI_DelayMS(millDelay);
}
#if 0
/***** local functions *******************************************************/

static void sTimeOutMilliSet( SiiLibTimeMilli_t* pMilliTO, SiiLibTimeMilli_t timeOut )
{
	SII_PLATFORM_DEBUG_ASSERT(MILLI_TO_MAX>timeOut);
	*pMilliTO = SiI_get_global_time() + timeOut;
}

static bool_t sTimeOutMilliIs( const SiiLibTimeMilli_t* pMilliTO )
{
	SiiLibTimeMilli_t milliNew = SiI_get_global_time();
	SiiLibTimeMilli_t milliDif = (*pMilliTO > milliNew) ? (*pMilliTO - milliNew) : (milliNew - *pMilliTO);

	if( MILLI_TO_MAX < milliDif )
		return (*pMilliTO >  milliNew) ? (true) : (false);
	else
		return (*pMilliTO <= milliNew) ? (true) : (false);
}
#endif

/***** end of file ***********************************************************/
