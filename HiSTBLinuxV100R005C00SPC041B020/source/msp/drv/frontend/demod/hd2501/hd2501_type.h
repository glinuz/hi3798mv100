/*************************************************************
** file name:		hd2501_type.h
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/
#ifndef	_HD2501_TYPE_H_
#define _HD2501_TYPE_H_

#define	_USE_HD2501_CHIP_


#pragma once
/* Common unsigned types */
#ifndef DEFINED_DOUBLE
#define DEFINED_DOUBLE
typedef double DOUBLE;
#endif

#ifndef DEFINED_FLOAT
#define DEFINED_FLOAT
typedef float FLOAT;
#endif

#ifndef DEFINED_IL32
#define DEFINED_IL32
typedef long IL32;
#endif

#ifndef DEFINED_U8
#define DEFINED_U8
typedef unsigned char  U8;
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
typedef unsigned short U16;
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
typedef unsigned int   U32;
#endif

#ifndef DEFINED_U64
#define DEFINED_U64
typedef unsigned long long   U64;
#endif


/* Common signed types */
#ifndef DEFINED_S8
#define DEFINED_S8
typedef signed char  S8;
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
typedef signed short S16;
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
typedef signed int   S32;
#endif

#ifndef DEFINED_S64
#define DEFINED_S64
typedef signed long long  S64;
#endif

typedef volatile U8   DU8;
typedef volatile U16  DU16;
typedef volatile U32  DU32;


/* Boolean type (values should be among TRUE and FALSE constants only) */
#ifndef DEFINED_BOOL
#define DEFINED_BOOL
#ifndef BOOL
typedef int BOOL;
#define BOOL BOOL
#endif
#endif


/* Function return error code */
typedef U32 HDIC_ErrorCode_t;


/* Exported Constants ------------------------------------------------------ */

#define HDIC_INVALID_HANDLE                 (0)

/* BOOL type constant values */
#ifndef TRUE
    #define TRUE (1 == 1)
#endif
#ifndef FALSE
    #define FALSE (!TRUE)
#endif
#ifndef NULL
	#define	NULL	(void*)0
#endif


enum
{
    HDIC_NO_ERROR = 0,
    HDIC_ERROR_BAD_PARAMETER,           /* Bad parameter passed       */
    HDIC_ERROR_NO_MEMORY,               /* Memory allocation failed   */
    HDIC_ERROR_ALREADY_INITIALIZED,     /* Device already initialized */
    HDIC_ERROR_NEVER_INITIALIZE,		/* Device have no initialize  */
    HDIC_ERROR_NO_FREE_HANDLES,         /* Cannot open device again   */
    HDIC_ERROR_OPEN_HANDLE,             /* At least one open handle   */
    HDIC_ERROR_INVALID_HANDLE,          /* Handle is not valid        */
    HDIC_ERROR_FEATURE_NOT_SUPPORTED,   /* Feature unavailable        */
    HDIC_ERROR_INTERRUPT_INSTALL,       /* Interrupt install failed   */
    HDIC_ERROR_INTERRUPT_UNINSTALL,     /* Interrupt uninstall failed */
    HDIC_ERROR_TIMEOUT,                 /* Timeout occured            */
    HDIC_ERROR_DEVICE_BUSY,             /* Device is currently busy   */
    HDIC_ERROR_UNKNOWN_DEVICE,          /* 3 Unknown device name      */
    HDIC_ERROR_CREATE_MUTEX_FAILED,	    /* Create mutex failed        */
    HDIC_ERROR_CREATE_SEMAPHORE_FAILED, /* Create semaphore failed    */
    HDIC_ERROR_IIC_ERROR,
    HDIC_ERROR_FAIL
};

#endif // #define _HD2501_TYPE_H_