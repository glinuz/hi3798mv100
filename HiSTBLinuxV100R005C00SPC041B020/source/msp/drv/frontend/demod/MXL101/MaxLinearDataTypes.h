/*****************************************************************************************
 *
 * FILE NAME          : MaxLinearDataTypes.h
 * 
 * AUTHOR             : Brenndon Lee
 * DATE CREATED       : Jul/31, 2006
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

/*****************************************************************************************
    Macros
*****************************************************************************************/
#define __WINDOWS_PLATFORM__

#define BIG_TO_LITTLE_16(ptr, offset)    ((ptr[offset++] << 8)| ptr[offset++])

#define WRITE_OP  0
#define READ_OP   1
/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef char           SINT8;
typedef short          SINT16;
typedef int            SINT32;
typedef float          REAL32;
typedef double         REAL64;

#ifdef __WINDOWS_PLATFORM__

typedef enum 
{
  MXL_TRUE = 0,
  MXL_FALSE = 1,  

} MXL_STATUS;

typedef enum 
{
  MXL_DISABLE = 0,
  MXL_ENABLE,
        
  MXL_NO_FREEZE = 0,
  MXL_FREEZE,

  MXL_UNLOCKED = 0,
  MXL_LOCKED,
  
  MXL_OFF = 0,
  MXL_ON

} MXL_BOOL;

#else // __WINDOWS_PLATFORM__

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#ifdef BOOL
#undef BOOL
#endif

typedef enum 
{
  TRUE = 1,
  FALSE = 0,
        
  PASSED = 1,
  FAILED = 0,

  GOOD = 1,
  BAD = 0,
  
  YES = 1,
  NO = 0,

  ENABLED = 1,
  DISABLED = 0,

  ON = 1,
  OFF = 0,

  SUCCESS = 1,
  FAIL = 0,

  VALID = 1,
  INVALID = 0,
  
  BUSY = 1,
  IDLE = 0,

} BOOL;  
#endif  //  __WINDOWS_PLATFORM__

// The macro for memory-mapped register access

#define HWD_REG32(addr)             (*(volatile UINT32*)addr)

#define HWD_REG_WRITE32(addr, data) (*(volatile UINT32*)addr = (UINT32)data)
#define HWD_REG_WRITE16(addr, data) (*(volatile UINT16*)addr = (UINT16)data)
#define HWD_REG_WRITE08(addr, data) (*(volatile UINT8*)addr = (UINT8)data)

#define HWD_REG_READ32(addr)        (*(volatile UINT32*)addr)
#define HWD_REG_READ16(addr)        (*(volatile UINT16*)addr)
#define HWD_REG_READ08(addr)        (*(volatile UINT8*)addr)

#define PHY_REG32(addr)             (*(volatile UINT32*)addr)

#define PHY_REG_WRITE32(addr, data) (*(volatile UINT32*)addr = (UINT32)data)
#define PHY_REG_WRITE16(addr, data) (*(volatile UINT16*)addr = (UINT16)data)
#define PHY_REG_WRITE08(addr, data) (*(volatile UINT8*)addr = (UINT8)data)

#define PHY_REG_READ32(addr)        (*(volatile UINT32*)addr)
#define PHY_REG_READ16(addr)        (*(volatile UINT16*)addr)
#define PHY_REG_READ08(addr)        (*(volatile UINT8*)addr)


/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

#endif /* __MAXLINEAR_DATA_TYPES_H__ */

