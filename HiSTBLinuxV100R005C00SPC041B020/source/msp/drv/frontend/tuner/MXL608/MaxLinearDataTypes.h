/*******************************************************************************
 *
 * FILE NAME          : MaxLinearDataTypes.h
 * 
 * AUTHOR             : Brenndon Lee
 *                      Dong Liu 
 *
 * DATE CREATED       : Jul/31, 2006
 *                      Jan/23, 2010
 *
 * DESCRIPTION        : This file contains MaxLinear-defined data types.
 *                      Instead of using ANSI C data type directly in source code
 *                      All module should include this header file.
 *                      And conditional compilation switch is also declared
 *                      here.
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MAXLINEAR_DATA_TYPES_H__
#define __MAXLINEAR_DATA_TYPES_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

/******************************************************************************
    Macros
******************************************************************************/

#ifndef _POSIX_SOURCE 
#define _POSIX_SOURCE 
#endif

#ifdef _ANSI_C_SOURCE 
#define false                1
#define true                 0

#define TRUE                 1
#define FALSE                0
#endif 
/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
typedef unsigned char        UINT8;
typedef unsigned short       UINT16;
typedef unsigned int         UINT32;
typedef unsigned long long   UINT64;
typedef char                 SINT8;
typedef short                SINT16;
typedef int                  SINT32;
typedef float                REAL32;
typedef double               REAL64;
typedef unsigned long        ULONG_32;

#ifdef _ANSI_C_SOURCE
typedef unsigned char        bool;
#endif 

typedef enum 
{
  MXL_TRUE = 0,
  MXL_FALSE = 1,
  MXL_SUCCESS = 0,
  MXL_FAILED,
  MXL_BUSY,
  MXL_NULL_PTR,
  MXL_INVALID_PARAMETER,
  MXL_NOT_INITIALIZED,
  MXL_ALREADY_INITIALIZED,
  MXL_BUFFER_TOO_SMALL,
  MXL_NOT_SUPPORTED,
  MXL_TIMEOUT
} MXL_STATUS;

typedef enum{
	MxL_OK					        =  0x0,
	MxL_ERR_INIT			      =  0x1,
	MxL_ERR_RFTUNE			    =  0x2,
	MxL_ERR_SET_REG			    =  0x3,
	MxL_ERR_GET_REG			    =  0x4,
	MxL_ERR_MODE			      =  0x10,
	MxL_ERR_IF_FREQ			    =  0x11,
	MxL_ERR_XTAL_FREQ		    =  0x12,
	MxL_ERR_BANDWIDTH		    =  0x13,
	MxL_GET_ID_FAIL			    =  0x14,
	MxL_ERR_DEMOD_LOCK		  =  0x20,
	MxL_NOREADY_DEMOD_LOCK	=  0x21,
	MxL_ERR_OTHERS			    =  0x0A
}MxL_ERR_MSG;

typedef enum
{
  MXL_DISABLE = 0,
  MXL_ENABLE,

  MXL_UNLOCKED = 0,
  MXL_LOCKED,

  MXL_INVALID = 0, 
  MXL_VALID,      

  MXL_PORT_LOW = 0,
  MXL_PORT_HIGH,

  MXL_START = 0,
  MXL_FINISH,

  MXL_ABORT_TUNE = 0,
  MXL_START_TUNE,

  MXL_FINE_TUNE_STEP_DOWN = 0,
  MXL_FINE_TUNE_STEP_UP

} MXL_BOOL;

typedef enum 
{
  IFX_SUCCESS = 0,
  IFX_FAILED,
  IFX_BUSY,
  IFX_NULL_PTR,
  IFX_INVALID_PARAMETER,
  IFX_BUFFER_TOO_SMALL,
  IFX_TIMEOUT  
} IFX_STATUS;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

/******************************************************************************
    Prototypes
******************************************************************************/

#endif /* __MAXLINEAR_DATA_TYPES_H__ */

