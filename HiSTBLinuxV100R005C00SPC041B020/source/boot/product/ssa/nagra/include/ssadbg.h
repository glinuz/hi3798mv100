/*
** NAME:
**   ssadbg.h
** DESCRIPTION:
**   Definitions for Nagravision debug version of SSA libraries.
** COPYRIGHT:
**   2003 Kudelski SA NagraVision
** CONDITIONAL COMPILATION
**   None
***************************************************************
** $Header: /cvsroot/NagraSoft/STBProducts/components/ssa/src/com/ssadbg.h,v 1.12 2003/02/27 18:06:51 hernande Exp $
***************************************************************
** HISTORY:
**   $Log: ssadbg.h,v $
**   Revision 1.12  2003/02/27 18:06:51  hernande
**   PRMS ID: NONE
**   Increased the number of debug structures for modules to 4
**
**   Revision 1.11  2003/02/19 15:48:05  hernande
**   PRMS ID: NONE
**   Updated following coding conventions
**
**   Revision 1.10  2003/01/13 09:44:12  hernande
**   PRMS ID: NONE
**   Added directive to allow compilation for C++
**
**   Revision 1.9  2002/12/11 15:54:19  hernande
**   PRMS ID: NONE
**   Removed status values definitions from ssadbg.h
**   Changed names of RSA and SHA functions to avoid conflicts with
**   old SSD library.
**
**   Revision 1.8  2002/12/06 12:57:17  hernande
**   PRMS ID: NONE
**   Updated setting debug data
**
**   Revision 1.7  2002/12/04 17:04:45  hernande
**   PRMS ID: NONE
**   Removed ifdef NAGRA_DEBUG
**
**   Revision 1.6  2002/12/03 15:44:39  hernande
**   PRMS ID: NONE
**   Updated debugging features. Changed context management in ssabdisk
**
**   Revision 1.5  2002/12/03 08:02:25  hernande
**   PRMS ID: NONE
**   Changed include
**
**   Revision 1.4  2002/12/02 17:00:41  hernande
**   PRMS ID: NONE
**   Added addresses of validation keys to debug data structures.
**
**   Revision 1.3  2002/12/02 16:30:10  hernande
**   PRMS ID: NONE
**   Changed SsaSignatureCheckStatus definition.
**
**   Revision 1.2  2002/12/02 16:24:52  hernande
**   PRMS ID: NONE
**   Fixed small bug
**
**   Revision 1.1  2002/12/02 16:20:39  hernande
**   PRMS ID: NONE
**   First version. Defines debug data structures for SSA.
**
*/

#ifndef __SSADBG_H__
#define __SSADBG_H__

#include "ssa.h"

#define DEBUG_NUMBER_OF_APP_MODULES		4

#ifdef __cplusplus
extern "C" {
#endif

typedef TUnsignedInt32		TSsaSignatureCheckStatus;

typedef struct SSsaBlockCheckStatus {
	TUnsignedInt32					startAddress;
	TUnsignedInt32					length;
	TUnsignedInt32					signatureAddress;
	TUnsignedInt32					signatureLength;
	TBoolean							doubleSigned;
	TUnsignedInt32					firstCheckKeyAddress;
	TUnsignedInt32					secondCheckKeyAddress;
	TSsaSignatureCheckStatus	status;
} TSsaBlockCheckStatus;

typedef struct SSsaDebugData {
	TIntegrityCheckStatus		globalResult;
	TIntegrityCheckStatus		ssaDataArrayResult;
	TIntegrityCheckStatus		bootCodeResult;
	TIntegrityCheckStatus		pairingDataResult;
	TIntegrityCheckStatus		jumpCodeResult;
	TIntegrityCheckStatus		moduleResult;
	TSsaBlockCheckStatus			ssaDataArrayStatus;
	TSsaBlockCheckStatus			bootCodeStatus;
	TSsaBlockCheckStatus			bepCodeStatus;
	TSsaBlockCheckStatus			pairingDataStatus;
	TSsaBlockCheckStatus			secSsaDataArrayStatus;
	TSsaBlockCheckStatus			moduleStatus[DEBUG_NUMBER_OF_APP_MODULES];
} TSsaDebugData;

#ifdef __cplusplus
}
#endif

#endif /*__SSADBG_H__*/
