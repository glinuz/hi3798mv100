/** \file vp_api.h
 * vp_api.h
 *
 * Header file for the API-II c files.
 *
 * This file contains the all of the VoicePath API-II function prototypes. This
 * file should be used to bring in the VP-API-II library modules that are
 * necessary for a given application.
 *
 *****************************************************************************
 * NOTE: Inclusion of only this file is sufficient to bring in the all the   *
 * necessary aspects of VP_API.                                              *
 *****************************************************************************
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7547 $
 * $LastChangedDate: 2010-07-23 16:23:41 -0500 (Fri, 23 Jul 2010) $
 */


#ifndef VP_API_H
#define VP_API_H

/******************************************************************************
 * VP-API Version                                                             *
 *****************************************************************************/
/*
 * The following version number tag is updated at every release of the VP-API.
 * Since the VP-API is a common interface that supports more than one device,
 * version number change might occur when any aspect of the VP-API gets
 * released.
 */
#define VP_API_VERSION_TAG (0x021101)

#define VP_API_VERSION_MAJOR_NUM ((VP_API_VERSION_TAG) >> 16)
#define VP_API_VERSION_MINOR_NUM (((VP_API_VERSION_TAG) & 0x00FF00) >> 8)
#define VP_API_VERSION_MINI_NUM (((VP_API_VERSION_TAG) & 0x0FF))

/* First include various basic data types used in the API */
#include "vp_api_types.h"

/******************************************************************************
 * Defines the configuration of VP-API library that needs to be built.        *
 * Please modify the following include file as per your VP-API library        *
 * requirement(s).                                                            *
 ******************************************************************************/
#include "vp_api_cfg.h"

/******************************************************************************
 *                  PLEASE DO NOT MODIFY BELOW THIS LINE                      *
 ******************************************************************************/
/* Include the main VP-API-Common file */
#include "vp_api_common.h"

/* Include the VP-API-Header file required for the CSLAC sequencer */
#if defined (VP_CSLAC_SEQ_EN)
#include "vp_api_cslac_seq.h"
#endif

/* Include the necessary files depending on the requirement of the project */
#if defined (VP_CC_790_SERIES)
#include "vp790_api.h"    /* Vp790 device specific API functions and typedefs */
#endif

/* Include the necessary files depending on the requirement of the project */
#if defined (VP_CC_792_SERIES)
#include "vp792_api.h"    /* Vp790 device specific API functions and typedefs */
#endif

#if defined (VP_CC_880_SERIES)
#include "vp880_api.h"    /* Vp880 device specific API functions and typedefs */

/*
 * For 0823 LM, the FXO CID line is controlled by I/O3. For 0803 LM, it's
 * controlled by I/O2. Control with I/O3 is better since I/O2 has more features
 *
 *  #define VP880_FXO_CID_LINE      VP880_IODATA_IO2    Use with 0803 ref
 *  #define VP880_FXO_CID_LINE      VP880_IODATA_IO3    Use with 0823 ref
 */
#define VP880_FXO_CID_LINE      VP880_IODATA_IO3
#endif

#if defined (VP_CC_890_SERIES)
#include "vp890_api.h"    /* Vp890 device specific API functions and typedefs */
#endif

#if defined (VP_CC_580_SERIES)
#include "vp580_api.h"    /* Vp580 device specific API functions and typedefs */
#endif

#if defined (VP_CC_VCP_SERIES)
#include "dvp_api.h"     /* VCP device specific API functions and typedefs */
#endif

#if defined (VP_CC_VCP2_SERIES)
#include "vcp2_api.h"     /* VCP2 device specific API functions and typedefs */
#endif

#if defined (VP_CC_KWRAP)
#include "vp_kernel.h"    /* KWRAP device specific API functions and typedefs */
#endif

/* Macros for calling a device-specific API function using the pointer in the
   Device Context: */
typedef void (*VpTempFuncPtrType) (void);

#define VP_CALL_DEV_FUNC(func, args) \
    (((pDevCtx->funPtrsToApiFuncs.func) == VP_NULL) ? VP_STATUS_FUNC_NOT_SUPPORTED : (pDevCtx->funPtrsToApiFuncs.func) args )

/*
 * Based on the existing structure of the include files the following def
 * have been moved here.
 */

#if defined(VP_CC_880_SERIES) || defined(VP_CC_790_SERIES) || defined(VP_CC_890_SERIES)

  typedef union {
      uint8 dummy; /* preventing an empty union */
  #if defined(VP_CC_880_SERIES) && defined(VP880_INCLUDE_TESTLINE_CODE)
    Vp880TestHeapType vpTestHeap880;
    #define VP_TEST_HEAP_IS_REQUIRED
  #endif

  #if defined(VP_CC_890_SERIES) && defined(VP890_INCLUDE_TESTLINE_CODE)
    Vp890TestHeapType vpTestHeap890;
    #define VP_TEST_HEAP_IS_REQUIRED
  #endif

  #if defined(VP_CC_790_SERIES) && defined(VP790_INCLUDE_TESTLINE_CODE)
    /* Vp790TestHeapType vpTestHeap790; */
    #define VP_TEST_HEAP_IS_REQUIRED
  #endif
} VpTestHeapType;
#else
typedef char VpTestHeapType;
#endif

#endif /* VP_API_H */




