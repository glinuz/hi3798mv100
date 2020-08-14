/*
** NAME:
**   ssaflad.h
** DESCRIPTION:
**   SBP API
**   This a debug version, that provides detailed information
**   about code execution and status.
** COPYRIGHT:
**   2003 Kudelski SA NagraVision
** CONDITIONAL COMPILATION
**   None
***************************************************************
** $Header: /cvsroot/NagraSoft/STBProducts/components/ssa/src/com/ssaflad.h,v 1.5 2003/05/21 14:04:12 hernande Exp $
***************************************************************
** HISTORY:
**   $Log: ssaflad.h,v $
**   Revision 1.5  2003/05/21 14:04:12  hernande
**   PRMS ID: NONE
**   Updated SBP API
**
**   Revision 1.4  2003/05/09 14:34:24  hernande
**   PRMS ID: NONE
**   Updated APIs: Removed hash type flag.
**
**   Revision 1.3  2003/03/04 16:40:41  hernande
**   PRMS ID: NONE
**   Updated type definitions for selective flash area set check feature.
**
**   Revision 1.2  2003/03/03 08:19:37  hernande
**   PRMS ID: NONE
**   Added selective flash area check feature. Updated delivery stuff in makefiles.
**
**   Revision 1.1  2003/02/19 15:52:16  hernande
**   PRMS ID: NONE
**   Added separate header files for Nagra-debug version of SSA API.
**
**
*/

#ifndef __SSAFLAD_H__
#define __SSAFLAD_H__

/******************************************************************************/
/*                                                                            */
/*                               INCLUDE FILES                                */
/*                                                                            */
/******************************************************************************/

#include "ssa.h"
#include "ssadbg.h"

/******************************************************************************/
/*                             PUBLIC FUNCTIONS                               */
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*
** NAME:
**   ssaIntegrityCheck
** DESCRIPTION:
**   This function is responsible of validating the whole flash memory.
** PRECONDITION:
**   The SBP memory map is put in place.
** POSTCONDITION:
**   None.
** SIDE EFFECT:
**   None.
** IN:
**	 xUMCS
**	   32-bit value telling which UMCS to check.
**    Bit_0 = 1 => check UMCS 1.
**    Bit_1 = 1 => check UMCS 2.
** OUT:
**	 pxSsaDebugData
**	   pointer to debug data structure where the function will put debug information.
** IN OUT:
**   None.
** RETURN:
**   TIntegrityCheckStatus (see ssa.h)
** REMARK:
**   None.
*/
TIntegrityCheckStatus ssaIntegrityCheck(
	TUMCSSelector           xUMCS,
	TSsaDebugData*				pxSsaDebugData
);

#ifdef __cplusplus
}
#endif

#endif

