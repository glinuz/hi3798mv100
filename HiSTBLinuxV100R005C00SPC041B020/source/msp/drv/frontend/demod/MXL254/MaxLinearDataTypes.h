/*****************************************************************************************
 *
 * FILE NAME          : MaxLinearDataTypes.h
 * 
 * AUTHOR             : Mariusz Murawski                        
 * DATE CREATED       : Nov/30, 2011
 *
 * DESCRIPTION        : This file contains MaxLinear-defined data types.
 *                      Instead of using ANSI C data type directly in source code
 *                      All module should include this header file.
 *                      And conditional compilation switch is also declared
 *                      here.
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/
  
#ifndef __MAXLINEAR_DATA_TYPES_H__
#define __MAXLINEAR_DATA_TYPES_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#ifndef _MSC_VER 
#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/div64.h> 
#include<linux/math64.h>
#include<linux/kernel.h>

#else
#include <stdint.h>
#endif
#endif

/*****************************************************************************************
    Macros
*****************************************************************************************/

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

/* Basic datatypes are defined below. 
 * If you do not wish to use them, or you already have
 * them defined, please add MXL_DO_NOT_DEFINE_BASIC_DATATYPES
 * to compilation defines
 */
#ifndef MXL_DO_NOT_DEFINE_BASIC_DATATYPES
#ifdef _MSC_VER
#ifndef __midl
typedef unsigned __int8     UINT8;
typedef unsigned __int16    UINT16;
typedef unsigned __int32    UINT32;
typedef unsigned __int64    UINT64;

typedef __int8              SINT8;
typedef __int16             SINT16;
#else
typedef small               SINT8;
typedef short               SINT16;
#endif

typedef __int32             SINT32;
typedef __int64             SINT64;
#else
typedef uint8_t             UINT8;
typedef uint16_t            UINT16;
typedef uint32_t            UINT32;
typedef uint64_t            UINT64;

typedef int8_t              SINT8;
typedef int16_t             SINT16;
typedef int32_t             SINT32;
typedef int64_t             SINT64;
#endif

typedef float               REAL32;
typedef double              REAL64;
#endif

#define MXL_MIN(x, y)                   (((x) < (y)) ? (x) : (y))
#define MXL_MAX(x, y)                   (((x) >= (y)) ? (x) : (y))
#define MXL_ABS(x)                      (((x) >= 0) ? (x) : -(x))
#if 1
 #define u64mod(x, y)             (  \
                                         {  \
                                           UINT64 out = 0; \
                                           UINT64 x_1 = (UINT64) x; \
                                           UINT32 y_1 = (UINT32) y; \
                                           if (y) \
                                           { \
                                             out = do_div(x_1,y_1); \
                                           } \
                                           x_1; \
                                        } \
                                         )

 #define s64mod(x, y)             (                                                                      \
                                            {                                                                                  \
                                               UINT64 out = 0;                                                    \
                                               UINT64 x_1;                                                        \
                                               UINT32 y_1;                                                        \
                                               SINT64 q_1;                                                        \
                                               if ((x) < 0) x_1 = (UINT64)(-(SINT64)(x)); else x_1 = (UINT64) x;  \
                                               if ((y) < 0) y_1 = (UINT32)(-(SINT32)(y)); else y_1 = (UINT32) y;  \
                                               if (y_1)                                                           \
                                               {                                                                  \
                                                out = do_div(x_1,y_1);                                           \
                                              }                                                                  \
                                               q_1 = (SINT64)x_1;                                                 \
                                               if ((x) < 0) q_1 = -q_1;                                           \
                                               if ((y) < 0) q_1 = -q_1;                                           \
                                              (SINT32) q_1;                                                               \
                                            }                                                                    \
                                       )

#define MXL_DIV_ROUND(x, y)  (((y)!=0)?(u64mod(((x) + u64mod((y),2)), (y))):0)

#define MXL_DIV_ROUND_S(x, y)  (((y)!=0)?(s64mod(((x) + ((((x)>=0)?1:-1)*s64mod((y),2))) , (y))):0)

#endif
// MXL_DIV_ROUND_S round div operation - use only for signed types
//#define MXL_DIV_ROUND_S(x, y)           (((y)!=0)?(((x) + ((((x)>=0)?1:-1)*(y)/2)) / (y)):0)

#if (((!defined __MXL_WIN__) || (defined _WIN_CLI_)) && (!defined _KEEP_STDCALL_))
#undef  __stdcall
#define __stdcall
#endif // (((!defined __MXL_WIN__) || (defined _WIN_CLI_)) && (!defined _KEEP_STDCALL_))

typedef UINT32 (__stdcall *MXL_CALLBACK_FN_T)(UINT8 devId, UINT32 callbackType, void * callbackPayload);

typedef enum 
{
  MXL_SUCCESS = 0,
  MXL_FAILURE = 1,
  MXL_INVALID_PARAMETER,
  MXL_NOT_INITIALIZED,
  MXL_ALREADY_INITIALIZED,
  MXL_NOT_SUPPORTED,
  MXL_NOT_READY,
  MXL_DATA_ERROR
} MXL_STATUS_E;

typedef enum 
{
  MXL_DISABLE = 0,
  MXL_ENABLE  = 1,  
        
  MXL_FALSE = 0,
  MXL_TRUE  = 1,  

  MXL_INVALID = 0,
  MXL_VALID   = 1,

  MXL_NO      = 0,
  MXL_YES     = 1,  
  
  MXL_OFF     = 0,
  MXL_ON      = 1  
} MXL_BOOL_E;

// sample oemData type. Create your own
typedef struct
{
  UINT32         i2cAddress;
  UINT32         i2cIndex;
} oem_data_t;

/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

#endif /* __MAXLINEAR_DATA_TYPES_H__ */

