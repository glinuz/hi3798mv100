/*****************************************************************************
**
**  Name: mt_userdef.h
**
**  Copyright 2006-2009 Microtune, Inc. All Rights Reserved
**
**  This source code file contains confidential information and/or trade
**  secrets of Microtune, Inc. or its affiliates and is subject to the
**  terms of your confidentiality agreement with Microtune, Inc. or one of
**  its affiliates, as applicable and the terms of the end-user software
**  license agreement (EULA).
**
**  Protected by US Patent 7,035,614 and additional patents pending or filed.
**
**  BY INSTALLING, COPYING, OR USING THE MICROTUNE SOFTWARE, YOU AGREE TO BE
**  BOUND BY ALL OF THE TERMS AND CONDITIONS OF THE MICROTUNE, INC. END-USER
**  SOFTWARE LICENSE AGREEMENT (EULA), INCLUDING BUT NOT LIMITED TO, THE
**  CONFIDENTIALITY OBLIGATIONS DESCRIBED THEREIN. IF YOU DO NOT AGREE, DO NOT
**  INSTALL, COPY, OR USE THE MICROTUNE SOFTWARE AND IMMEDIATELY RETURN OR
**  DESTROY ANY COPIES OF MICROTUNE SOFTWARE OR CONFIDENTIAL INFORMATION THAT
**  YOU HAVE IN YOUR POSSESSION.
*****************************************************************************/

/*****************************************************************************
**
**  Name: mt_userdef.h
**
**  Description:    User-defined data types needed by MicroTuner source code.
**
**                  Customers must provide the code for these functions
**                  in the file "mt_userdef.c".
**
**                  Customers must verify that the typedef's in the
**                  "Data Types" section are correct for their platform.
**
**  Functions
**  Requiring
**  Implementation: MT_WriteRead
**                  MT_Sleep
**
**  References:     None
**
**  Exports:        None
**
**  CVS ID:         $Id: mt_userdef.h,v 1.1 2010/09/06 09:12:57 software Exp $
**  CVS Source:     $Source: /home/cvs/software/tuners/MT2081/mt_userdef.h,v $
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**   082   12-06-2004    JWS    Multi-tuner support - requires MTxxxx_CNT
**                              declarations
**   N/A   04-13-2007    JWS    Added Signed 8-bit type - S8Data
**   N/A   11-03-2009    TP     New generation of functions and types
**
*****************************************************************************/
#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>

#include "mt2081.h"

#include "hi_debug.h"
#include "hi_type.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

#if !defined( __MT2081_USERDEF_H )
#define __MT2081_USERDEF_H

//#include "mt2081.h"

#include "mt2081_errordef.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  Data Types
*/
typedef unsigned char   mt_uint8_t;     /*  type corresponds to unsigned 8 bits    */
typedef char            mt_int8_t;      /*  type corresponds to signed 8 bits      */
typedef unsigned short  mt_uint16_t;    /*  type corresponds to unsigned 16 bits   */
typedef short           mt_int16_t;     /*  type corresponds to signed 16 bits     */
typedef unsigned int    mt_uint32_t;    /*  type corresponds to unsigned 32 bits   */
typedef int             mt_int32_t;     /*  type corresponds to signed 32 bits     */
typedef void *          mt_handle_t;    /*  memory pointer type                    */
typedef void *          mt_user_t;      /*  user defined argument type for user data */
                                        /*  in user defined MT_XX function calls   */

#define MAX_UDATA         (4294967295UL)  /*  max value storable in mt_uint32_t      */
#define MT_MAX_UINT32     (4294967295)
#define MT_MAX_SINT32     (2147483647)

#ifndef NULL                            /*  NULL Pointer/Value address             */
 #define NULL 0
#endif

/*
** Define an MTxxxx_CNT macro for each type of tuner that will be built
** into your application (e.g. MT2081).
*/
#define MT2081_CNT   (4)                 /* number or MT2081 tuners to be used in this project */
#define MT_TUNER_CNT (4)

#if defined MT2081_CNT
#define MT2081_WRITE_READ_MTU   (32)              /* Maximum number of registers(16bit) per I2C transaction */
#endif


/*
**  Optional user-defined Error/Info Codes  (examples below)
**
**  This is the area where you can define user-specific error/info return
**  codes to be returned by any of the functions you are responsible for
**  writing such as MT_WriteSub() and MT_ReadSub.  There are four bits
**  available in the status field for your use.  When set, these
**  bits will be returned in the status word returned by any tuner driver
**  call.  If you OR in the MT_ERROR bit as well, the tuner driver code
**  will treat the code as an error.
**
**  The following are a few examples of errors you can provide.
**
**  Example 1:
**  You might check to see the hUserData handle is correct and issue
**  MY_USERDATA_INVALID which would be defined like this:
**
**  #define MY_USERDATA_INVALID  (MT_USER_ERROR | MT_USER_DEFINED1)
**
**
**  Example 2:
**  You might be able to provide more descriptive two-wire bus errors:
**
**  #define NO_ACK   (MT_USER_ERROR | MT_USER_DEFINED1)
**  #define NO_NACK  (MT_USER_ERROR | MT_USER_DEFINED2)
**  #define BUS_BUSY (MT_USER_ERROR | MT_USER_DEFINED3)
**
**
**  Example 3:
**  You can also provide information (non-error) feedback:
**
**  #define MY_INFO_1   (MT_USER_DEFINED1)
**
**
**  Example 4:
**  You can combine the fields together to make a multi-bit field.
**  This one can provide the tuner number based off of the addr
**  passed to MT_WriteSub or MT_ReadSub.  It assumes that
**  MT_USER_DEFINED4 through MT_USER_DEFINED1 are contiguously. If
**  TUNER_NUM were OR'ed into the status word on an error, you could
**  use this to identify which tuner had the problem (and whether it
**  was during a read or write operation).
**
**  #define TUNER_NUM  ((addr & 0x07) >> 1) << MT_USER_SHIFT
**
*/


/*****************************************************************************
**
**  Name: MT_WriteRead
**
**  Description:    Write values to device using a two-wire serial bus.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to tuner's Open function.
**                  addr       - device serial bus address  (value passed
**                               as parameter to MTxxxx_Open)
**                  pData_w    - pointer to the Data to be written to the
**                               device
**                  cnt_w      - number of bytes to be written
**                  pData_r    - pointer to the Data to be read from the
**                               device
**                  cnt_r      - number of bytes to be read
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      user-defined
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code for this
**                  function to write data using the tuner's 2-wire serial
**                  bus.
**
**                  The hUserData parameter is a user-specific argument.
**                  If additional arguments are needed for the user's
**                  serial bus read/write functions, this argument can be
**                  used to supply the necessary information.
**                  The hUserData parameter is initialized in the tuner's Open
**                  function.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**   N/A   11-03-2009    TP     New generation of functions and types
**
*****************************************************************************/
mt_uint32_t MT_WriteRead(mt_user_t   hUserData,
                         mt_uint32_t addr,
                         mt_uint8_t *pData_w,
                         mt_uint32_t cnt_w,
                         mt_uint8_t *pData_r,
                         mt_uint32_t cnt_r);


/*****************************************************************************
**
**  Name: MT_Sleep
**
**  Description:    Delay execution for "nMinDelayTime" milliseconds
**
**  Parameters:     hUserData     - User-specific I/O parameter that was
**                                  passed to tuner's Open function.
**                  nMinDelayTime - Delay time in milliseconds
**
**  Returns:        None.
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code that
**                  blocks execution for the specified period of time.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   03-25-2004    DAD    Original
**
*****************************************************************************/
void MT2081_Sleep(mt_user_t   hUserData,
              mt_uint32_t nMinDelayTime);


/*
** Definitions for a systems-wide multituner spuravoidance
*/
#if (defined(MT2082_CNT) && (MT2082_CNT > 0)) || (defined(MT2084_CNT) && (MT2084_CNT > 0))
struct spuravoid_entry {
    mt_uint8_t active;
    mt_uint32_t frequency;
    mt_uint32_t bandwidth;
    mt_uint8_t priority;
    struct spuravoid_entry *next;
};

void spuravoid_register_entry(struct spuravoid_entry *entry);
struct spuravoid_entry *spuravoid_get_list_head();
#endif

#if defined( __cplusplus )
}
#endif

#endif
