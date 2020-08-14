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
* @file si_lib_log.c
*
* @brief 
*
*****************************************************************************/

/***** #include statements ***************************************************/
#include <stdarg.h>

#include "si_datatypes.h"
#include "si_lib_log_api.h"
#include "si_lib_time_api.h"
#include "platform_api.h"
#include "sii_time.h"
#include "si_lib_obj_api.h"


/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_log);

/***** local macro definitions ***********************************************/

#define LOG_LINE_WRAP              (80)

/***** local type definitions ************************************************/

/***** local prototypes ******************************************************/

static uint_t sLogLimited( uint_t size, const char* pStr );
static void sLogPutString( char* pStr );

extern int vsnrprintf(char *str, size_t size, const char *format, va_list ap);

/***** local data objects ****************************************************/

static uint16_t sLinePos = 0;

/***** public functions ******************************************************/
#if 1
void SiiLibLogTimeStamp(const char* pClassStr, void* pObj)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
//	SiiLibTimeMilli_t mSec = SiI_get_global_time();
#endif
	uint16_t tot  = 0;

	/* Print time stamp */
	SII_PRINTF("\n%d.%03d-", (SiiLibTimeMilli_t)SiI_get_global_time()/1000, (SiiLibTimeMilli_t)SiI_get_global_time()%1000);
	
	/* Print module name */
	tot += (uint16_t)sLogLimited(12, pClassStr);

	/* If instance print instance name */
	if( pObj )
	{
		/* Separation character */
		SII_PRINTF(".");
		tot++;

		tot += (uint16_t)sLogLimited(12, SII_LIB_OBJ_NAME_INSTANCE(pObj));
	}

	/* Print alignment space characters */
	tot = (24 < tot) ? (0) : (24-tot);
	while( tot-- )
		SII_PRINTF(" ");

	/* Print end of preamble */
	SII_PRINTF((": "));
	
	sLinePos = 29;
}
#else
void SiiLibLogTimeStamp(const char* funcName, uint32_t line)
{
	SiiLibTimeMilli_t mSec = SiI_get_global_time();
	uint16_t tot  = 0;

	/* Print time stamp */
	SII_PRINTF(("\n%d.%03d-", mSec/1000, mSec%1000));
	
	/* Print module name */
	tot += (uint16_t)sLogLimited(12, pClassStr);

	/* If instance print instance name */
	if( pObj )
	{
		/* Separation character */
		SII_PRINTF(("."));
		tot++;

		tot += (uint16_t)sLogLimited(12, SII_LIB_OBJ_NAME_INSTANCE(pObj));
	}

	/* Print alignment space characters */
	tot = (24 < tot) ? (0) : (24-tot);
	while( tot-- )
		SII_PRINTF((" "));

	/* Print end of preamble */
	SII_PRINTF((": "));
	
	sLinePos = 29;
}
#endif


void SiiLibLogPrintf( char* pFrm, ...)
{
	va_list  arg;
	uint16_t chars;
	char     str[300]; // CEC_LOGGER requires 160

	va_start(arg, pFrm);
	chars = SII_VNSPRINTF(str, 300, pFrm, arg);
	va_end(arg);
	if(chars > ((int)sizeof(str)))
	{
		return;
	}

	sLogPutString(str);
}

/***** local functions *******************************************************/

static uint_t sLogLimited( uint_t size, const char* pStr )
{
	uint_t i;

	for( i=0; i<size; i++ )
	{
		/* Check for end of string */
		if( !(*pStr) )
			break;

		SII_PRINTF("%c", *pStr);
		pStr++;
	}
	return i;
}


static void sLogPutString( char* pStr )
{
	while( *pStr )
	{
		if( *pStr == '\n' )
		{
			sLinePos = 0;
		}
		else
		{
			/* Apply indent */
			if( 0 == sLinePos )
			{
				SII_PRINTF(("\n    "));
			}
			else if( LOG_LINE_WRAP < sLinePos )
			{
				SII_PRINTF(("\n    "));
				sLinePos = 0;
			}
			SII_PRINTF("%c", *pStr);
			sLinePos++;
		}
		pStr++;
	}
}

/***** end of file ***********************************************************/
