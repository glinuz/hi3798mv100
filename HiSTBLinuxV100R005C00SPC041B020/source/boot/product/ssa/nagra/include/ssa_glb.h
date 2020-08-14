/*
** NAME:
**   ssa_glb.h
** DESCRIPTION:
**   General definitions
** COPYRIGHT:
**   2002-2003 Kudelski SA NagraVision
** CONDITIONAL COMPILATION
**   None
***************************************************************
** $Header: /cvsroot/NagraSoft/STBProducts/components/ssa/src/com/ssa_glb.h,v 1.5 2003/06/20 11:18:18 hernande Exp $
***************************************************************
** HISTORY:
**   $Log: ssa_glb.h,v $
**   Revision 1.5  2003/06/20 11:18:18  hernande
**   PRMS ID: NONE
**   Protected agains conflicts with ca_defs.h
**
**   Revision 1.4  2003/02/27 13:17:01  hernande
**   PRMS ID: NONE
**   Cleaned up. Formatted following coding conventions.
**
**   Revision 1.3  2003/01/13 09:44:12  hernande
**   PRMS ID: NONE
**   Added directive to allow compilation for C++
**
**   Revision 1.2  2002/11/29 07:58:56  hernande
**   PRMS ID: NONE
**   Redistributed some SSA data types
**
**   Revision 1.1  2002/11/28 17:18:49  hernande
**   PRMS ID: NONE
**   First Version
**
**   Revision 1.1  2002/06/25 12:38:02  pican
**   New archive for the SSA software.
**
*/


#ifndef __SSA_GLB_H__
#define __SSA_GLB_H__

#ifndef CA_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                                                                            */
/*                                  DATA TYPES                                */
/*                                                                            */
/******************************************************************************/

/**
 *  @brief
 *    Signed 8-bit type.
*/
typedef signed char           TSignedInt8;

/**
 *  @brief
 *    Signed 16-bit type.
*/
typedef signed short int      TSignedInt16;

/**
 *  @brief
 *    Signed 32-bit type.
*/
typedef signed long int       TSignedInt32;


/**
 *  @brief
 *    Unsigned 8-bit type.
*/
typedef unsigned char         TUnsignedInt8;

/**
 *  @brief
 *    Unsigned 16-bit type.
*/
typedef unsigned short int    TUnsignedInt16;

/**
 *  @brief
 *    Unsigned 32-bit type.
*/
typedef unsigned long int     TUnsignedInt32;


/**
 *  @brief
 *    Boolean Type.
*/
typedef TUnsignedInt8         TBoolean;

/**
 *  @brief
 *    Character type.
*/
typedef char                  TChar;


/**
 *  @brief
 *    Size type.
*/
typedef unsigned int          TSize;


/******************************************************************************/
/*                                                                            */
/*                                  CONSTANTS                                 */
/*                                                                            */
/******************************************************************************/

#ifndef FALSE
#define FALSE      0
#endif

#ifndef TRUE
#define TRUE       (!FALSE)
#endif

/**
 *  @brief
 *    NULL pointer.
*/
#ifndef NULL
#define NULL    ((void *)0)
#endif

#define BIT0                      0x1U
#define BIT1                      0x2U
#define BIT2                      0x4U
#define BIT3                      0x8U
#define BIT4                     0x10U
#define BIT5                     0x20U
#define BIT6                     0x40U
#define BIT7                     0x80U
#define BIT8                    0x100U
#define BIT9                    0x200U
#define BIT10                   0x400U
#define BIT11                   0x800U
#define BIT12                  0x1000U
#define BIT13                  0x2000U
#define BIT14                  0x4000U
#define BIT15                  0x8000U
#define BIT16                 0x10000U
#define BIT17                 0x20000U
#define BIT18                 0x40000U
#define BIT19                 0x80000U
#define BIT20                0x100000U
#define BIT21                0x200000U
#define BIT22                0x400000U
#define BIT23                0x800000U
#define BIT24               0x1000000U
#define BIT25               0x2000000U
#define BIT26               0x4000000U
#define BIT27               0x8000000U
#define BIT28              0x10000000U
#define BIT29              0x20000000U
#define BIT30              0x40000000U
#define BIT31              0x80000000U


#ifdef __cplusplus
}
#endif

#endif /* CA_DEFS_H not defined */

#endif /* __SSA_GLB_H__ */

/* END OF FILE */
