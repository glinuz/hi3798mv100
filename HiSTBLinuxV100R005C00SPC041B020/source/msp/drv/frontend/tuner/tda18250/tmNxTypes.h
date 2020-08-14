/*==========================================================================*/
/*     (Copyright (C) 2003 Koninklijke Philips Electronics N.V.             */
/*     All rights reserved.                                                 */
/*     This source code and any compilation or derivative thereof is the    */
/*     proprietary information of Koninklijke Philips Electronics N.V.      */
/*     and is confidential in nature.                                       */
/*     Under no circumstances is this software to be exposed to or placed   */
/*     under an Open Source License of any type without the expressed       */
/*     written permission of Koninklijke Philips Electronics N.V.           */
/*==========================================================================*/
/*
* Copyright (C) 2000,2001
*               Koninklijke Philips Electronics N.V.
*               All Rights Reserved.
*
* Copyright (C) 2000,2001 TriMedia Technologies, Inc.
*               All Rights Reserved.
*
*############################################################
*
* Module name  : tmNxTypes.h  %version: CFR_FEAP#22 %
*
* Last Update  : %date_modified: Tue Jul  8 18:08:00 2003 %
*
* Description: TriMedia/MIPS global type definitions.
*
* Document Ref: DVP Software Coding Guidelines Specification
* DVP/MoReUse Naming Conventions specification
* DVP Software Versioning Specification
* DVP Device Library Architecture Specification
* DVP Board Support Library Architecture Specification
* DVP Hardware API Architecture Specification
*
*
*############################################################
*/

#ifndef TM_NX_TYPES_H
#define TM_NX_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Environment macros:                                                        */
/*============================================================================*/

#ifdef TMFL_NATIVE_C_FORCED
 #undef TMFL_DOT_NET_2_0_TYPES
 #undef NXPFE
#endif

/*============================================================================*/
/* Standard Types                                                             */
/*============================================================================*/

    #define False           0
    #define True            1

    typedef void            CVoid;      /* Void (typeless) */
    typedef int             CInt;       /* machine-natural integer */
    typedef unsigned int    CUInt;      /* machine-natural unsigned integer */
    typedef signed   char   CInt8;      /*  8 bit   signed integer */
    typedef signed   short  CInt16;     /* 16 bit   signed integer */
    typedef signed   long   CInt32;     /* 32 bit   signed integer */
    typedef unsigned char   CUInt8;     /*  8 bit unsigned integer */
    typedef unsigned short  CUInt16;    /* 16 bit unsigned integer */
    typedef unsigned long   CUInt32;    /* 32 bit unsigned integer */
    typedef float           CFloat;     /* 32 bit floating point */
    typedef double          CDouble;    /* 32/64 bit floating point */
    typedef unsigned int    CBool;      /* Boolean (True/False) */
    typedef char            CChar;      /* character, character array ptr */
    typedef CUInt32         tmErrorCode_t;
    typedef CUInt32         tmProgressCode_t;

    /* Endianness */
    typedef CInt            Endian;
    #define BigEndian       0
    #define LittleEndian    1


#ifdef TMFL_DOT_NET_2_0_TYPES
    using namespace System;
    typedef int             Int;
    typedef SByte           Int8;
    typedef Byte            UInt8;
    typedef float           Float32;
    typedef unsigned int    Bool;
    typedef Void           *pVoid;
    #define Null            nullptr

#else
    typedef CVoid           Void;       /* Void (typeless) */
    typedef CInt            Int;        /* machine-natural integer */
    typedef CUInt           UInt;       /* machine-natural unsigned integer */
    typedef CInt8           Int8;       /*  8 bit   signed integer */
    typedef CInt16          Int16;      /* 16 bit   signed integer */
    typedef CInt32          Int32;      /* 32 bit   signed integer */
    typedef CUInt8          UInt8;      /*  8 bit unsigned integer */
    typedef CUInt16         UInt16;     /* 16 bit unsigned integer */
    typedef CUInt32         UInt32;     /* 32 bit unsigned integer */
    typedef CFloat          Float;      /* 32 bit floating point */
    typedef CDouble         Double;     /* 32/64 bit floating point */
    typedef CBool           Bool;       /* Boolean (True/False) */
    typedef CChar           Char;       /* character, character array ptr */
    typedef char           *String;     /* Null terminated 8 bit char str */

    typedef CVoid          *pVoid;      /* Void (typeless) */
    typedef Int            *pInt;       /* machine-natural integer */
    typedef UInt           *pUInt;      /* machine-natural unsigned integer */
    typedef Int8           *pInt8;      /*  8 bit   signed integer */
    typedef Int16          *pInt16;     /* 16 bit   signed integer */
    typedef Int32          *pInt32;     /* 32 bit   signed integer */
    typedef UInt8          *pUInt8;     /*  8 bit unsigned integer */
    typedef UInt16         *pUInt16;    /* 16 bit unsigned integer */
    typedef UInt32         *pUInt32;    /* 32 bit unsigned integer */
    typedef Float          *pFloat;     /* 32 bit floating point */
    typedef Double         *pDouble;    /* 32/64 bit floating point */
    typedef Bool           *pBool;      /* Boolean (True/False) */
    typedef Char           *pChar;      /* character, character array ptr */
    typedef String         *pString;    /* Null terminated 8 bit char str */

#ifdef __cplusplus
    #define Null            0
#else
    #define Null            ((Void *) 0)
#endif

    /* Legacy Types/Structures */
    typedef char           *Address;        /* Ready for address-arithmetic */
    typedef char const     *ConstAddress;
    typedef unsigned char   Byte;           /* Raw byte */
    typedef float           Float32;        /* Single-precision float */
    typedef double          Float64;        /* Double-precision float */
    typedef void           *Pointer;        /* Pointer to anonymous object */
    typedef void const     *ConstPointer;
    typedef char const     *ConstString;

#endif

    /*Assume that 64-bit integers are supported natively by C99 compilers and Visual
    C version 6.00 and higher. More discrimination in this area may be added
    here as necessary.*/
#ifndef TMFL_DOT_NET_2_0_TYPES
#if defined __STDC_VERSION__ && __STDC_VERSION__ > 199409L
    /*This can be enabled only when all explicit references to the hi and lo
    structure members are eliminated from client code.*/
    #define TMFL_NATIVE_INT64 1
    typedef signed   long long int Int64,  *pInt64;  /* 64-bit integer */
    typedef unsigned long long int UInt64, *pUInt64; /* 64-bit bitmask */
    /* #elif defined _MSC_VER && _MSC_VER >= 1200 */
    /* This can be enabled only when all explicit references to the hi and lo
       structure members are eliminated from client code. */
    /* #define TMFL_NATIVE_INT64 1 */
    /* 64-bit integer */
    /* typedef signed   __int64 Int64,  *pInt64; */
    /* 64-bit bitmask */
    /* typedef unsigned __int64 UInt64, *pUInt64; */
#else /*!(defined __STDC_VERSION__ && __STDC_VERSION__ > 199409L)*/
#define TMFL_NATIVE_INT64 0
    typedef
    struct
    {
        /*Get the correct endianness (this has no impact on any other part of
        the system, but may make memory dumps easier to understand).*/
#ifdef  _TARGET_PLATFORM_MSB_FIRST
        Int32 hi; UInt32 lo;
#else
        UInt32 lo; Int32 hi;
#endif
    }
    Int64, *pInt64; /* 64-bit integer */

    typedef
    struct
    {
#ifdef _TARGET_PLATFORM_MSB_FIRST
        UInt32 hi; UInt32 lo;
#else
        UInt32 lo; UInt32 hi;
#endif
    }
    UInt64, *pUInt64; /* 64-bit bitmask */
#endif /*defined __STDC_VERSION__ && __STDC_VERSION__ > 199409L*/
#endif /*TMFL_DOT_NET_2_0_TYPES*/


    /* Maximum length of device name in all BSP and capability structures */
#define HAL_DEVICE_NAME_LENGTH 16



    /* timestamp definition */
#ifndef TMFL_DOT_NET_2_0_TYPES
    typedef UInt64 tmTimeStamp_t, *ptmTimeStamp_t;
#endif
    /* for backwards compatibility with the older tmTimeStamp_t definition */
#define ticks   lo
#define hiTicks hi


/*============================================================================*/
/* Hardware device power states                                               */
/*============================================================================*/
    typedef enum tmPowerState
    {
        tmPowerOn = 0,                      /* Device powered on      (D0 state) */
        tmPowerStandby,                     /* Device power standby   (D1 state) */
        tmPowerSuspend,                     /* Device power suspended (D2 state) */
        tmPowerOff,                         /* Device powered off     (D3 state) */
        tmPowerMax                          /* Device power max */
    }   tmPowerState_t, *ptmPowerState_t;

/*============================================================================*/
/* Software Version Structure                                                 */
/*============================================================================*/

    typedef struct tmSWVersion
    {
        UInt32      compatibilityNr;        /* Interface compatibility number */
        UInt32      majorVersionNr;         /* Interface major version number */
        UInt32      minorVersionNr;         /* Interface minor version number */
        UInt32      buildVersionNr;         /* Interface build version number */
    }   tmSWVersion_t, *ptmSWVersion_t;


/*============================================================================*/
/* HW Unit Selection                                                          */
/*============================================================================*/
    typedef CInt tmUnitSelect_t, *ptmUnitSelect_t;

#define tmUnitNone  ((tmUnitSelect_t)(-1))
#define tmUnit0     0
#define tmUnit1     1
#define tmUnit2     2
#define tmUnit3     3
#define tmUnit4     4
#define tmUnit5     5

#define TMFL_OS_IS_PSOS               0
#define TMFL_OS_IS_NULLOS             0
#define TMFL_OS_IS_ECOS               0
#define TMFL_OS_IS_HPUNIX             0


/*============================================================================*/
/* Instance handle                                                            */
/*============================================================================*/
    typedef Int tmInstance_t, *ptmInstance_t;

#ifndef TMFL_DOT_NET_2_0_TYPES
    /* Callback function declaration */
    typedef Void (*ptmCallback_t) (UInt32 events, Void *pData, UInt32 userData);
    #define tmCallback_t ptmCallback_t /*compatibility*/
#endif


/*============================================================================*/
/* INLINE keyword for inline functions in all environments                    */
/*============================================================================*/

#if defined(_MSC_VER) 
    #define inline  __inline
#elif defined(__GNUC__)
    #undef inline
    #define inline __inline__
#elif TMFL_OS_IS_PSOS && TMFL_CPU_IS_MIPS

    /* NOTE regarding the keyword INLINE:
      
       Inline is not an ANSI-C keyword, hence every compiler can implement inlining
       the way it wants to. When using the dcc compiler this might possibly lead to
       redeclaration warnings when linking. For example:
      
            dld: warning: Redeclaration of tmmlGetMemHandle
            dld:    Defined in root.o
            dld:    and        tmmlApi.o(../../lib/pSOS-MIPS/libtmml.a)
      
       For the dcc compiler inlining is not on by default. When building a retail
       version ( _TMTGTREL=ret), inlining is turned on explicitly in the dvp1 pSOS
       makefiles by specifying -XO, which enables all standard optimizations plus
       some others, including inlining (see the Language User's Manual, D-CC and
       D-C++ Compiler Suites p46). When building a debug version ( _TMTGTREL=dbg),
       the optimizations are not turned on (and even if they were they would have
       been overruled by -g anyway).
      
       When a .h file with inline function declarations gets included in multiple
       source files, redeclaration warnings are issued.
      
       When building a retail version those functions are inlined, but in addition
       the function is also declared within the .o file, resulting in redeclaration
       warnings as the same function is also defined by including that same header
       file in other source files. Defining the functions as static inline rather
       than inline solves the problem, as now the additional function declaration
       is omitted (as now the compiler realizes that there is no point in keeping
       that declaration as it can only be called from within this specific file,
       but it isn't, because all calls are already inline).
      
       When building a debug version no inlining is done, but the functions are
       still defined within the .o file, again resulting in redeclaration warnings.
       Again, defining the functions to be static inline rather than inline solves
       the problem.

       Now if we would change the definition of the inline keyword for pSOS from
       __inline__ to static __inline__, all inline function definitions throughout
       the code would not issue redeclaration warnings anymore, but all static
       inline definitions would.
       If we don't change the definition of the inline keyword, all inline func-
       tion definitions would return redeclaration warnings.
      
       As this is a pSOS linker bug, it was decided not to change the code but
       rather to ignore the issued warnings.
    */

    #define inline  __inline__

#elif      TMFL_OS_IS_PSOS && TMFL_CPU_IS_TM
    /* TriMedia keyword is already inline */

#elif      TMFL_OS_IS_ECOS && TMFL_CPU_IS_MIPS

    #define inline __inline__

#elif      (TMFL_OS_IS_HPUNIX || TMFL_OS_IS_NULLOS)
    /*
       TMFL_OS_IS_HPUNIX is the HP Unix workstation target OS environment for the
       DVP SDE2 using the GNU gcc toolset.  It is the same as TMFL_OS_IS_NULLOS
       (which is inaccurately named because it is the HP Unix CPU/OS target
       environment).
    */
    /* LM; 02/07/2202; to be able to use Insure, I modify the definition of inline */
    /* #define inline  __inline__ */
    #define inline 

#elif TMFL_OS_IS_ECOS && TMFL_CPU_IS_MIPS

    #define inline

#else /* TMFL_OS_IS_??? */

    #define inline 

#endif /* TMFL_OS_IS_XXX */


/*============================================================================*/
/* Macros for deprecating                                                     */
/*============================================================================*/

#ifdef __GNUC__ 
    #undef DEPRECATED
    #define DEPRECATED(_LEGACYFUNC, _FUNC) _LEGACYFUNC __attribute__ ((deprecated("was declared deprecated. Use " ## #_FUNC ## " instead.")))
    #define DEPRECATED_MSG(_LEGACY, _NEW)
#elif defined(_MSC_VER) 
    #define DEPRECATED(_LEGACYFUNC, _FUNC) __declspec(deprecated("was declared deprecated. Use " ## #_FUNC ## " instead.")) _LEGACYFUNC 
    #define DEPRECATED_MSG(_LEGACY, _NEW) __pragma(message(__FILE__ "(0) : warning C4996: " _LEGACY " was declared deprecated. Use " _NEW " instead." ))
#else 
    #pragma message("WARNING: You need to implement DEPRECATED for this compiler") 
    #define DEPRECATED(_LEGACYFUNC, _FUNC) _LEGACYFUNC 
#endif 


#ifdef __cplusplus
}
#endif

#endif /* ndef TM_NX_TYPES_H */

