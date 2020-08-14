/**
 *  @defgroup GLB Global definitions
 *  @brief
 *    Global definition used by all the other components.
*/
/**
**  @file ca_defs.h
**
**  @brief
**    This file defines the general types and constants used by the
**    NagraVision CA software.
**
**  @ingroup GLB
**
**  $Revision: 1.28 $
**
**  COPYRIGHT:
**    1998-2003 Nagravision S.A.
**
*/


#ifndef CA_DEFS_H
#define CA_DEFS_H

/* The following definitions are specific to WIN32 DLL stuff. */
/*  NAGRA_CA_EXPORTS  NAGRA_CA_WIN_NO_DLL    Result */
/*      undefined          undefined       win app, importing the ca dll      */
/*       defined           undefined       win ca dll, exporting the ca API   */
/*      undefined           defined        win app, with embedded ca (no dll) */
/*       defined            defined        not consistent, default to ca dll  */
#ifndef NAGRA_CA_API
#ifdef WIN32
#ifdef NAGRA_CA_EXPORTS
/* Inside the DLL. */
#define NAGRA_CA_API __declspec(dllexport)
#else
/* Outside of the DLL */
#ifdef NAGRA_CA_WIN_NO_DLL
/* Non DLL context. */
#define NAGRA_CA_API
#else
/* Application using the ca dll. */
#define NAGRA_CA_API __declspec(dllimport)
#endif
#endif
#else
/* Non WIN32 context. */
#define NAGRA_CA_API
#endif
#endif /* def NAGRA_CA_API */

/******************************************************************************/
/*                                                                            */
/*                           GENERAL INCLUDE FILES                            */
/*                                                                            */
/******************************************************************************/

//#include <string.h>
//#include <stdlib.h>
//#include <stddef.h>
//#include <stdio.h>
#ifndef HI_MINIBOOT_SUPPORT
#include "linux/types.h"
#else
#include "app.h"
#endif

#ifdef CA_INCLUDE_OBFUSCATE_SYMBOLS
#include "symbols.h"
#endif /* CA_INCLUDE_OBFUSCATE_SYMBOLS */

#ifdef NV_HAS_CONFIG_HEADER
#include "ain_cfg.h"
#endif /* NV_HAS_CONFIG_HEADER */

/******************************************************************************/
/*                                                                            */
/*                            CAT TYPES DEFINITIONS                           */
/*                                                                            */
/******************************************************************************/

#ifndef CA_SCALAR_TYPES
#define CA_SCALAR_TYPES
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

/** @cond APA_NO_EXPORT */
/**
 *  @brief
 *    Unsigned 64-bit type.
*/
#ifdef UINT64_TYPE

#ifndef LONG_LONG_INT_DEFINED
#define LONG_LONG_INT_DEFINED
#endif

  typedef UINT64_TYPE TUnsignedInt64;

#else /* UINT64_TYPE */

#undef LONG_LONG_INT_DEFINED

  /**
   * @brief
   *   Type used to simulate a 64-bit integer with two 32-bit.
  */
  typedef struct SUnsignedInt64
  {
    TUnsignedInt32  mostSignificantBits;
      /**<  This field contains the 32 most significant bits of the value.
      */
    TUnsignedInt32  leastSignificantBits;
      /**<  This field contains the 32 least significant bits of the value.
      */
  } TUnsignedInt64;
  
  /** Global variable used as a null value. */
  extern const TUnsignedInt64 gNullInt64;

#undef NULL_INT64
#define NULL_INT64  gNullInt64

#endif /* UINT64_TYPE */
/** @endcond */


/**
 *  @brief
 *    Boolean Type.
*/
typedef TUnsignedInt8         TBoolean;

/**
 * @brief
 *   The evaluation of a condition is false
 * @see TBoolean
*/
#ifndef FALSE
#define FALSE      0
#endif

/**
 * @brief
 *   The evaluation of a condition is true
 * @see TBoolean
*/
#ifndef TRUE
#define TRUE       (!FALSE)
#endif


/**
 *  @brief
 *    Character type
*/
typedef char                  TChar;


/**
 *  @brief
 *    Size type
*/
typedef size_t                TSize;


/** @cond APA_NO_EXPORT */
/**
 *  @brief
 *    NULL pointer
*/
#ifndef NULL
#define NULL    ((void *)0)
#endif
/** @endcond */

#endif /* CA_SCALAR_TYPES */

/** @cond APA_CAK_API */
/**
 *  @brief
 *    This enumeration defines the error status returned by the CA functions.
*/
typedef enum
{
  CA_NO_ERROR,
  /**<  The function terminated successfully.
  */
  CA_ERROR,
  /**<  Unspecified error happen during function execution.
  */
  CA_ERROR_CA_NOT_RUNNING
  /**<  The Nagra CA is currently not running and the requested
        operation can't be executed.
  */
} TCaStatus;
/** @endcond */


/******************************************************************************/
/*                                                                            */
/*                              BITS DEFINITION                               */
/*                                                                            */
/******************************************************************************/

/** @cond APA_NO_EXPORT */
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
/** @endcond */

/******************************************************************************/
/*                                                                            */
/*                             MACROS DEFINITION                              */
/*                                                                            */
/******************************************************************************/

/** @cond APA_NO_EXPORT */

/**
 *  @brief
 *    Max macro definition.
*/
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

/**
 *  @brief
 *    Min macro definition.
*/
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/** @endcond */

#endif /* CA_DEFS_H */

/* END OF FILE */
