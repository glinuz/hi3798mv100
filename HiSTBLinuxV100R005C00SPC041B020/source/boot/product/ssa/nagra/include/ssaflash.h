/*
** NAME:
**   ssaflash.h
** DESCRIPTION:
**   SBP API
** COPYRIGHT:
**   2002-2003 Kudelski SA NagraVision
** CONDITIONAL COMPILATION
**   None
***************************************************************
** $Header: /cvsroot/NagraSoft/STBProducts/components/ssa/src/com/ssaflash.h,v 1.9 2003/05/21 14:04:12 hernande Exp $
***************************************************************
** HISTORY:
**   $Log: ssaflash.h,v $
**   Revision 1.9  2003/05/21 14:04:12  hernande
**   PRMS ID: NONE
**   Updated SBP API
**
**   Revision 1.8  2003/05/09 14:34:25  hernande
**   PRMS ID: NONE
**   Updated APIs: Removed hash type flag.
**
**   Revision 1.7  2003/03/04 16:40:41  hernande
**   PRMS ID: NONE
**   Updated type definitions for selective flash area set check feature.
**
**   Revision 1.6  2003/03/03 08:19:37  hernande
**   PRMS ID: NONE
**   Added selective flash area check feature. Updated delivery stuff in makefiles.
**
**   Revision 1.5  2003/02/19 15:48:06  hernande
**   PRMS ID: NONE
**   Updated following coding conventions
**
**   Revision 1.4  2003/01/13 09:44:13  hernande
**   PRMS ID: NONE
**   Added directive to allow compilation for C++
**
**   Revision 1.3  2002/12/03 15:44:39  hernande
**   PRMS ID: NONE
**   Updated debugging features. Changed context management in ssabdisk
**
**   Revision 1.2  2002/12/02 16:19:38  hernande
**   PRMS ID: NONE
**   Added debug data structures
**
**   Revision 1.1  2002/11/28 17:18:50  hernande
**   PRMS ID: NONE
**   First Version
**
*/

#ifndef __SSAFLASH_H__
#define __SSAFLASH_H__

#include "ssa.h"

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
**    etc.
** OUT:
**   None.
** IN OUT:
**   None.
** RETURN:
**   TIntegrityCheckStatus (see ssa.h)
** REMARK:
**   None.
*/

TIntegrityCheckStatus ssaIntegrityCheck(
	TUMCSSelector     xUMCS
);

#ifdef __cplusplus
}
#endif

#endif

