/*
** NAME:
**   ssa.h
** DESCRIPTION:
**   General definitions for SSA.
** COPYRIGHT:
**   2002-2003 Kudelski SA NagraVision
** CONDITIONAL COMPILATION
**   None
***************************************************************
** $Header: /cvsroot/NagraSoft/STBProducts/components/ssa/src/com/ssa.h,v 1.21 2003/05/21 14:04:12 hernande Exp $
***************************************************************
** HISTORY:
**   $Log: ssa.h,v $
**   Revision 1.21  2003/05/21 14:04:12  hernande
**   PRMS ID: NONE
**   Updated SBP API
**
**   Revision 1.20  2003/05/16 17:55:04  hernande
**   PRMS ID: NONE
**   Updated
**
**   Revision 1.19  2003/05/14 18:22:59  hernande
**   PRMS ID: NONE
**   Finished updates for new signature format. Split code SHA-1/MD5. Updated return codes for SBP and SSD APIs.
**
**   Revision 1.18  2003/05/09 14:34:22  hernande
**   PRMS ID: NONE
**   Updated APIs: Removed hash type flag.
**
**   Revision 1.17  2003/04/30 08:26:29  hernande
**   PRMS ID: NONE
**   Updated comments.
**
**   Revision 1.16  2003/04/29 15:25:19  bielmann
**   PRMS ID: NONE
**   Updated after security code review by Dominique Bongard.
**
**   Revision 1.15  2003/04/11 11:19:04  hernande
**   PRMS ID: NONE
**   Updated SSA integrity check status code values.
**
**   Revision 1.14  2003/04/10 15:54:16  hernande
**   PRMS ID: NONE
**   Removed "TOO_BIG" return codes of ssaIntegrityCheck()
**
**   Revision 1.13  2003/04/09 11:37:27  hernande
**   PRMS ID: NONE
**   Updated comments.
**
**   Revision 1.12  2003/03/31 16:54:23  hernande
**   PRMS ID: NONE
**   Updated hash checking mechanism. Refactorized code, so that check of hash(es) is done only in ssasc.c. Added new compilation option for test/production version of the SSA libraries. Now test library checks that keys are test keys, and prod library checks that keys are prod keys. Added check of signature padding. Added test cases for checks of hash and padding.
**
**   Revision 1.11  2003/03/10 09:13:56  hernande
**   PRMS ID: NONE
**   Removed flash area start address and length information. Updated ssa Integrity Check return code format.
**
**   Revision 1.10  2003/03/04 16:40:41  hernande
**   PRMS ID: NONE
**   Updated type definitions for selective flash area set check feature.
**
**   Revision 1.9  2003/03/03 15:58:29  hernande
**   PRMS ID: NONE
**   Added constants for flash area selector.
**
**   Revision 1.8  2003/03/03 08:19:37  hernande
**   PRMS ID: NONE
**   Added selective flash area check feature. Updated delivery stuff in makefiles.
**
**   Revision 1.7  2003/02/27 13:17:01  hernande
**   PRMS ID: NONE
**   Cleaned up. Formatted following coding conventions.
**
**   Revision 1.6  2003/02/19 15:48:05  hernande
**   PRMS ID: NONE
**   Updated following coding conventions
**
**   Revision 1.5  2003/01/27 17:57:44  hernande
**   PRMS ID: NONE
**   Changed signature check return values (OK != 0)
**
**   Revision 1.4  2003/01/13 09:44:11  hernande
**   PRMS ID: NONE
**   Added directive to allow compilation for C++
**
**   Revision 1.3  2002/11/29 08:07:28  hernande
**   PRMS ID: NONE
**   Redistributed some ssa data types
**
**   Revision 1.2  2002/11/29 07:58:56  hernande
**   PRMS ID: NONE
**   Redistributed some SSA data types
**
**   Revision 1.1  2002/11/28 17:18:49  hernande
**   PRMS ID: NONE
**   First Version
**
*/

#ifndef __SSA_H__
#define __SSA_H__

#include "ssa_glb.h"

/******************************************************************************/
/*                                                                            */
/*                                 DATA TYPES                                 */
/*                                                                            */
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************/
/* SBP Integrity Check Status Codes                           */
/*                                                            */
/* 31      24       16       8       0                        */
/* +--------+--------+--------+--------+                      */
/* |      Module Status       |  G.S.  |                      */
/* +--------+--------+--------+--------+                      */
/*                                                            */
/*  G.S.:          Global Status                              */
/*                                                            */
/*  Module Status: 24 bits, each bit is a flag indicating the */
/*                 status of 1 module. If flag=0, the module  */
/*                 is valid. If flag=1, then the module is    */
/*                 not valid.                                 */
/*                                                            */
/**************************************************************/

typedef TUnsignedInt32     TIntegrityCheckStatus;

/****************************/
/* Global Status codes
*/

/* Errors in Locked Flash */
#define  ICS_LOCKED_MEMORY_INVALID              0
/* Errors in Unlocked Flash */
#define  ICS_SSA_DA_2_INVALID                   1
#define  ICS_MODULES_INVALID                    2
/* Full Integrity check is successful */
#define  ICS_VALID_NO_ERROR                     3

/* If G.S. = ICS_SSA_DA_2_INVALID
   then the Secondary SSA Data Array is not valid.
   No code contained in application modules can be executed,
   because the application modules cannot be checked.
   The STB should implement a recovery mechanism that
   gets a valid version of the Secondary SSA Data Array.
   Otherwise, the STB must stop booting. */

/****************************/
/* Module Status flags
*/

/* These flags are meaningful only when G.S. = ICS_MODULES_INVALID */
/* If the UMCS selector argument is incorrect or there is a problem
   in the definition of the UMC and UMCS, then ssaIntegrityCheck()
   returns ICS_MODULES_INVALID | ICS_ALL_MODULES_INVALID. */

#define  ICS_MODULE_1_INVALID      BIT8
#define  ICS_MODULE_2_INVALID      BIT9
#define  ICS_MODULE_3_INVALID      BIT10
#define  ICS_MODULE_4_INVALID      BIT11
#define  ICS_MODULE_5_INVALID      BIT12
#define  ICS_MODULE_6_INVALID      BIT13
#define  ICS_MODULE_7_INVALID      BIT14
#define  ICS_MODULE_8_INVALID      BIT15
#define  ICS_MODULE_9_INVALID      BIT16
#define  ICS_MODULE_10_INVALID      BIT17
#define  ICS_MODULE_11_INVALID     BIT18
#define  ICS_MODULE_12_INVALID     BIT19
#define  ICS_MODULE_13_INVALID     BIT20
#define  ICS_MODULE_14_INVALID     BIT21
#define  ICS_MODULE_15_INVALID     BIT22
#define  ICS_MODULE_16_INVALID     BIT23
#define  ICS_MODULE_17_INVALID     BIT24
#define  ICS_MODULE_18_INVALID     BIT25
#define  ICS_MODULE_19_INVALID     BIT26
#define  ICS_MODULE_20_INVALID     BIT27
#define  ICS_MODULE_21_INVALID     BIT28
#define  ICS_MODULE_22_INVALID     BIT29
#define  ICS_MODULE_23_INVALID     BIT30
#define  ICS_MODULE_24_INVALID     BIT31
#define  ICS_ALL_MODULES_INVALID   0xFFFFFF00


/* Generic signature check status */

typedef TUnsignedInt32  TCheckStatus;

#define CS_INVALID            0    /* Signature invalid */
#define CS_VALID_NO_ERROR     1    /* Signature valid */

/* UMCS selector (can be combined using OR (|) ) */

typedef TUnsignedInt32 TUMCSSelector;

#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_1     BIT0
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_2     BIT1
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_3     BIT2
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_4     BIT3
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_5     BIT4
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_6     BIT5
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_7     BIT6
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_8     BIT7
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_9     BIT8
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_10    BIT9
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_11    BIT10
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_12    BIT11
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_13    BIT12
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_14    BIT13
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_15    BIT14
#define SSA_UNLOCKED_MEMORY_CONTAINER_SET_16    BIT15

#ifdef __cplusplus
}
#endif


#endif

