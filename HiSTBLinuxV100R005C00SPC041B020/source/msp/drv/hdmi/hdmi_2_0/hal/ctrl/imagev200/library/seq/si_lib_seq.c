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
* @file si_lib_seq.c
*
* @brief Sequencer (provides timers)
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

//#include <string.h>

#ifdef SII_ENV_BUILD_UFD
#define SII_LIB_UFD_PROFILER_ENABLE  1
#else
#define SII_LIB_UFD_PROFILER_ENABLE  0
#endif

#include "si_lib_obj_api.h"
#include "si_lib_seq_api.h"
#include "si_lib_log_api.h"
#if(SII_LIB_UFD_PROFILER_ENABLE)
#include "si_lib_time_api.h"
#endif /* SII_LIB_UFD_PROFILER_ENABLE */
#include "platform_api.h"
#include "sii_time.h"

/***** Register Module name **************************************************/

HDMI_MUTEX g_avmuteTimerMute;

SII_LIB_OBJ_MODULE_DEF(lib_seq);

/***** local macro definitions ***********************************************/

#define STOP_VALUE                     ((SiiLibSeqTimeMs_t)-1)

#define DIAG_CONVERT_TO_FLOAT(pf, pi)  { *pf += (float)*pi; *pi  = 0; }

#define MILLI_SEC_DIFF(t1, t2)         ( ((t2) < (t1)) ? ((((SiiLibSeqTimeMs_t)0)-1) - (t1) + (t2) + 1) : ((t2) - (t1)) )

/***** local type definitions ************************************************/

typedef struct
{
	SiiLibSeqCbFunc_t        cbFunc;      //!< Function pointer to timer callback
	SiiInst_t                parentInst;  //!< Pointer to parent object
	SiiLibSeqTimeMs_t        timeSet;     //!< Periodic time in msec. set by user
	SiiLibSeqPriority_t      priority;    //!< Priority (0-255) determines the position in timer table.
	bool_t                   bRealTime;   //!< Preserve realtime constratin
    SiiTimerId_t             enTimerId;
	SiiLibSeqTimeMs_t        timePrv;     //!< Previous time sample
	SiiLibSeqTimeMs_t        timeRun;     //!< Remaining time left in msec.

#if(SII_LIB_UFD_PROFILER_ENABLE)
	/* For diagnostics */
	uint32_t                 diagHits;    //!< Total number of timer hits so far
	SiiHalTimeMicro_t        diagMicro;   //!< Total accumulated time of timer processing time in micro seconds
	float                    fDiagMicro;  //!< Total accumulated time in micro seconds
#endif /* SII_LIB_UFD_PROFILER_ENABLE */
} Obj_t;

/***** local prototypes ******************************************************/

static void sPriorityUpdate( Obj_t* pObj );
//SiiLibSeqTimeMs_t sSequencerHandler( void );

#if(SII_LIB_UFD_PROFILER_ENABLE)
static void sDiagUpdateTotalTime( void );
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

/***** ufd definition ********************************************************/

#ifdef SII_ENV_BUILD_UFD
#include "si_lib_ufd_api.h"

static void sUfdStopAll( void );
static void sUfdDumpTimers( void );

static SiiLibUfdCmdRec_t sUfdCmdTbl[] =
{
	UFD_HDR( "Seq",            "Scheduler controls"  ),

	UFD_RC1( "SeqDmp",         VOI,         sUfdDumpTimers,                 NULL,                           "Lists all allocated timers"   ),
	UFD_RC1( "SeqSlp",         BOO,         SiiLibSeqSetSuspendMode,        SiiLibSeqIsSuspendMode,         "Suspend mode (sleep mode)"    ),
	UFD_RC1( "TimSlp",         BOO,         SiiLibSeqSetSuspendMode,        SiiLibSeqIsSuspendMode,         "Suspend mode (sleep mode)"    ), /* Legacy command */
	UFD_RC1( "SeqStopAll",     VOI,         sUfdStopAll,                    NULL,                           "Stop Timer Instance"          ),
	UFD_RC3( "SeqStart",       I32,I32,OBJ, SiiLibSeqTimerStart,            NULL,                           "Start Timer Instance"         ),
	UFD_RC2( "SeqStop",        VOI,OBJ,     SiiLibSeqTimerStop,             NULL,                           "Stop Timer Instance"          ),
	UFD_RC2( "SeqPrior",       I08,OBJ,     SiiLibSeqTimerPrioritySet,      SiiLibSeqTimerPriorityGet,      "Set priority (0..255)"        ),
	UFD_RC2( "SeqIsRun",       BOO,OBJ,     NULL,                           SiiLibSeqTimerRunningIs,        "Is timer running"             ),

	UFD_END()
};
#endif /* SII_ENV_BUILD_UFD */

/***** local data objects ****************************************************/

static bool_t             sbWake       = true;
static Obj_t*             spCurObj     = NULL;  /* reference to current processed timer instance */
static SiiLibSeqTimeMs_t  sOverRunTime = 0;
//static bool_t             sbExitNot    = true;

#if(SII_LIB_UFD_PROFILER_ENABLE)
/* For diagnostics */
static SiiHalTimeMicro_t  sDiagMicroPrv  = 0;
static SiiHalTimeMicro_t  sDiagMicroTot  = 0;
static float              sfDiagMicroTot = 0.0;
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

/***** public functions ******************************************************/


void SiiLibSeqSetSuspendMode( bool_t isTrue )
{
	sbWake = !isTrue;
}

#if SII_LIB_UFD_PROFILER_ENABLE
bool_t SiiLibSeqIsSuspendMode( void )
{
	return !sbWake;
}
#endif

SiiInst_t SiiLibSeqTimerCreate( const char* pNameStr, SiiLibSeqCbFunc_t cbFunc, SiiInst_t parentInst, SiiLibSeqPriority_t priority, SiiTimerId_t enTimerId)
{
	Obj_t* pObj = NULL;

	SII_PLATFORM_DEBUG_ASSERT(cbFunc);

	/* Do for first created instantiation only */
	if( SII_LIB_OBJ_ZEROINST() )
	{
#ifdef SII_ENV_BUILD_UFD
		SiiLibUfdRegisterCmdTable(sUfdCmdTbl);
#endif /* SII_ENV_BUILD_UFD */
	}

	/* Allocate memory for object */
	pObj = (Obj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(Obj_t));
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	/* Initialize instance */
	pObj->cbFunc      = cbFunc;
	pObj->parentInst  = parentInst;
	pObj->timeSet     = 0;
    pObj->enTimerId   = enTimerId;
	pObj->priority    = priority;
	pObj->bRealTime   = false;
	pObj->timePrv     = 0;
	pObj->timeRun     = STOP_VALUE;
#if(SII_LIB_UFD_PROFILER_ENABLE)
	pObj->diagHits    = 0;
	pObj->diagMicro   = 0;
	pObj->fDiagMicro  = 0;
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

	/* Repostion object in linked list based on priority */
	sPriorityUpdate(pObj);

	return SII_LIB_OBJ_INST(pObj);
}

void SiiLibSeqTimerDelete( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	/* De-allocate instantiation memory */
	SII_LIB_OBJ_DELETE(pObj);

	/* Do for last deleted instantiation only */
	if( SII_LIB_OBJ_ZEROINST() )
	{
#ifdef SII_ENV_BUILD_UFD
		SiiLibUfdUnRegisterCmdTable(sUfdCmdTbl);
#endif /* SII_ENV_BUILD_UFD */
	}
}

void SiiLibSeqTimerStart( SiiInst_t inst, SiiLibSeqTimeMs_t timeMs, SiiLibSeqTimeMs_t periodMs )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->timePrv = (uint16_t)SiI_get_global_time();
	pObj->timeRun = timeMs;
	pObj->timeSet = periodMs;
}

void SiiLibSeqTimerStartIsr( SiiInst_t inst, SiiLibSeqTimeMs_t timeMs, SiiLibSeqTimeMs_t periodMs )
{
	Obj_t* pObj = (Obj_t*)inst;

	pObj->timePrv = (uint16_t)SiI_get_global_time();
	pObj->timeRun = timeMs;
	pObj->timeSet = periodMs;
}

void SiiLibSeqTimerStop( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->timeRun = STOP_VALUE;
}

#if 0 // not used
bool_t SiiLibSeqTimerRunningIs( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	return (STOP_VALUE == pObj->timeRun) ? (true) : (false);
}

void SiiLibSeqTimerPeriodSet( SiiInst_t inst, SiiLibSeqTimeMs_t periodMs )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->timeSet = periodMs;
}

SiiLibSeqTimeMs_t SiiLibSeqTimerPeriodGet( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	return pObj->timeSet;
}

void SiiLibSeqTimerPrioritySet( SiiInst_t inst, SiiLibSeqPriority_t priority )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->priority = priority;
	sPriorityUpdate(pObj);
}

SiiLibSeqPriority_t SiiLibSeqTimerPriorityGet( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	return pObj->priority;
}

void SiiLibSeqTimerRealTimeSet( SiiInst_t inst, bool_t bIsTrue )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->bRealTime = bIsTrue;
}

bool_t SiiLibSeqTimerRealTimeIs( SiiInst_t inst )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_PNTR(inst);

	return pObj->bRealTime;
}

SiiLibSeqTimeMs_t SiiLibSeqTimerOverRunTimeGet( void )
{
	return sOverRunTime;
}

void SiiLibSeqAbort( void )
{
	sbExitNot = false;
}
#endif

void SiiLibSeqTask( void )
{
	while( 1 )
	{
		SiiSequencerHandler();
	}
}

/***** local functions *******************************************************/
extern void switchbank (unsigned char bank_number);

static void sPriorityUpdate( Obj_t* pObj )
{
	Obj_t* pTmpObj = (Obj_t*)SII_LIB_OBJ_FIRST();

	if( pTmpObj->priority > pObj->priority )
	{
		/* Move object to first place in list */
		SII_LIB_OBJ_MOVE(NULL, pObj);
	}
	else
	{
		Obj_t* pPrvObj;

		do {
			pPrvObj = pTmpObj;

			/* Go to next timer instance */
			pTmpObj = SII_LIB_OBJ_NEXT(pTmpObj);
		} while( (pTmpObj) && (pTmpObj->priority <= pObj->priority) );

		/* Move object to directly after pPrvObj */
		SII_LIB_OBJ_MOVE(pPrvObj, pObj);
	}
}

SiiLibSeqTimeMs_t SiiSequencerHandler( void )
{
	Obj_t*             pObj       = (Obj_t*)SII_LIB_OBJ_FIRST();
	SiiLibSeqTimeMs_t  timeNew    = (uint16_t)SiI_get_global_time();
	SiiLibSeqTimeMs_t  timeWakeUp = STOP_VALUE;

	/* Search timer list for timeouts */
	while( pObj )
	{
		spCurObj = pObj;

		/* All timers with priority greater than zero will be suspended if sbWake=false */
		if( (0 < pObj->priority) && !sbWake )
			break;

        if(pObj->enTimerId == SII_TIMER_AVMUTE)
        {
            HDMI_MUTEX_LOCK(g_avmuteTimerMute);
        }

		if( STOP_VALUE > pObj->timeRun )
		{
			SiiLibSeqTimeMs_t timeDiff = MILLI_SEC_DIFF(pObj->timePrv, timeNew);

			/* Update previous time */
			pObj->timePrv = timeNew;

			if( pObj->timeRun > timeDiff )
			{
				/* Update Running Time */
				pObj->timeRun -= timeDiff;

				if( pObj->timeRun < timeWakeUp )
					timeWakeUp = pObj->timeRun;
			}
			else /* timeout */
			{
				SiiLibSeqTimeMs_t timeTmp;

#if(SII_LIB_UFD_PROFILER_ENABLE)
				SiiHalTimeMicro_t T1, T2;
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

				/* Load running timer with over run time (latency of current time-out), which maybe queried only from Call Back function */
				sOverRunTime = timeDiff - pObj->timeRun;

				/* Reload or Stop timer */
				timeTmp = pObj->timeRun = (pObj->timeSet) ? (pObj->timeSet) : (STOP_VALUE);

				/* Call call-back function */
#if(SII_LIB_UFD_PROFILER_ENABLE)
				T1 = SiiHalTimeMicroGet();
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

				if(pObj->cbFunc)
					pObj->cbFunc(pObj->parentInst);

#if(SII_LIB_UFD_PROFILER_ENABLE)
				T2 = SiiHalTimeMicroGet();

				/* Update timing diagnostics */
				pObj->diagHits++;
				pObj->diagMicro += SiiHalTimeMicroDiffGet(T1, T2);

				/* Long term micro-time accumulation requires to convert to floating point domain */
				if( 0 > ((int)pObj->diagMicro) )
					DIAG_CONVERT_TO_FLOAT(&pObj->fDiagMicro, &pObj->diagMicro);
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

				/* Apply Real-Time mode for periodic timer: re-adjust new timer based on previous timer over run time */
				if( pObj->bRealTime )
				{
					if( (STOP_VALUE != pObj->timeRun) && (timeTmp == pObj->timeRun) )
					{
						if( pObj->timeRun < sOverRunTime )
						{
							SII_LIB_LOG_DEBUG1(pObj, "Real-Time violation by: %d msec!", sOverRunTime - pObj->timeRun);
							/* Full over run compensation is not possible */
							pObj->timeRun = 0;
						}
						else
						{
							/* Compensate for timer over run time */
							pObj->timeRun -= sOverRunTime;
						}
					}
				}
				sOverRunTime = 0;
				//break; /* process only 1 timer at the time */
			}
		}

        if(pObj->enTimerId == SII_TIMER_AVMUTE)
        {
            HDMI_MUTEX_UNLOCK(g_avmuteTimerMute);
        }

		/* Go to next timer instance */
		pObj = SII_LIB_OBJ_NEXT(pObj);
	}

	/* Make sure that spCurInst is only avaialble from scheduled timer call-backs */
	spCurObj = NULL;

#if(SII_LIB_UFD_PROFILER_ENABLE)
	/* Update total passed time in micro seconds for diagnostics */
	sDiagUpdateTotalTime();

	/* Long term micro-time accumulation requires to convert to floating point domain */
	if( 0 > ((int)sDiagMicroTot) )
		DIAG_CONVERT_TO_FLOAT(&sfDiagMicroTot, &sDiagMicroTot);
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

	return (pObj) ? ((SiiLibSeqTimeMs_t)0) : (timeWakeUp);
}

#if(SII_LIB_UFD_PROFILER_ENABLE)
static void sDiagUpdateTotalTime( void )
{
	SiiHalTimeMicro_t T2 = SiiHalTimeMicroGet();

	sDiagMicroTot += SiiHalTimeMicroDiffGet(sDiagMicroPrv, T2);
	sDiagMicroPrv  = T2;
}
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

#ifdef SII_ENV_BUILD_UFD
static void sUfdStopAll( void )
{
	Obj_t* pObj = (Obj_t*)SII_LIB_OBJ_FIRST();

	while( pObj )
	{
		pObj->timeRun = STOP_VALUE;

		/* Go to next timer instance */
		pObj = SII_LIB_OBJ_NEXT(pObj);
	}
}

static void sUfdDumpTimers( void )
{
	Obj_t*            pObj           = (Obj_t*)SII_LIB_OBJ_FIRST();
	uint8_t           Indx           = 1;
#if(SII_LIB_UFD_PROFILER_ENABLE)
	float             fMicroProcTot  = 0;
	SiiHalTimeMicro_t T1             = 0;
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

#if(SII_LIB_UFD_PROFILER_ENABLE)
	if( spCurObj )
	{
		/* Capture time before start printing diagnostics (only if called from Ufd handler) */
		T1 = SiiHalTimeMicroGet();
	}

	/* Update total passed time in micro seconds for diagnostics */
	sDiagUpdateTotalTime();
	DIAG_CONVERT_TO_FLOAT(&sfDiagMicroTot, &sDiagMicroTot);
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

	SiiLibUfdPrintLine(85);
	SiiLibUfdPrintf("                                    Inst        Periodic   Current     Total   Proc. \n");
	SiiLibUfdPrintf("No. Name                            ref  Pri.      Time      Time      Hits     (%%) \n");
	SiiLibUfdPrintLine(85);

	while( pObj )
	{
#if(SII_LIB_UFD_PROFILER_ENABLE)
		DIAG_CONVERT_TO_FLOAT(&pObj->fDiagMicro, &pObj->diagMicro);
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

		SiiLibUfdPrintf("%2d  ", Indx++);
		SiiLibUfdPrintf("%-30s  ", SiiLibObjNameInstanceGet(pObj));
		SiiLibUfdPrintObjReference(pObj);
		SiiLibUfdPrintf("  ");
		SiiLibUfdPrintf("%3d  ", pObj->priority);
		SiiLibUfdPrintf("%8ld  ", pObj->timeSet);
		if( STOP_VALUE > pObj->timeRun )
			SiiLibUfdPrintf("%8ld  ", pObj->timeRun);
		else
			SiiLibUfdPrintf(" STOPPED  ");
#if(SII_LIB_UFD_PROFILER_ENABLE)
		SiiLibUfdPrintf("%8ld  ", pObj->diagHits);
		SiiLibUfdPrintf("%6.2f", 100.0*(pObj->fDiagMicro/sfDiagMicroTot));
#endif /* SII_LIB_UFD_PROFILER_ENABLE */
		SiiLibUfdPrintf("\n");

#if(SII_LIB_UFD_PROFILER_ENABLE)
		/* Find total processing time */
		fMicroProcTot += pObj->fDiagMicro;

		/* Reset Diagnostics */
		pObj->fDiagMicro = 0.0;
		pObj->diagHits   = 0;
#endif /* SII_LIB_UFD_PROFILER_ENABLE */

		/* Go to next timer instance */
		pObj = SII_LIB_OBJ_NEXT(pObj);
	}
	SiiLibUfdPrintLine(85);
#if(SII_LIB_UFD_PROFILER_ENABLE)
	SiiLibUfdPrintf("    System Idle                                                              %6.2f\n", 100.0*(1.0 - fMicroProcTot/sfDiagMicroTot));
	SiiLibUfdPrintLine(85);

	/* Reset total capture time */
	sfDiagMicroTot = 0.0;

	/* Apply processing time compensation for printing diagnostics (only if called from Ufd handler) */
	if( spCurObj )
	{
		spCurObj->fDiagMicro = -1.0*SiiHalTimeMicroDiffGet(T1, SiiHalTimeMicroGet());
	}
#endif /* SII_LIB_UFD_PROFILER_ENABLE */
}
#endif /* SII_ENV_BUILD_UFD */

/***** end of file ***********************************************************/
