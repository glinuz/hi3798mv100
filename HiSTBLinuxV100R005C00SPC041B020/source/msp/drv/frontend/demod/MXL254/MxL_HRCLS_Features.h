/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_Features.h
 * 
 * AUTHOR             : Jun Huang
 *
 * DATE CREATED       : 08/29/2011
 *
 * DESCRIPTION        : Header file for Hercules SW compilation switches
 *
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL_HRCLS_FEATURES_H__
#define __MXL_HRCLS_FEATURES_H__

/*****************************************************************************************
    Include Header Files
*****************************************************************************************/
#ifndef MXL_HRCLS_DEVICE_TYPE_EXTERNAL
#include "MxL_HRCLS_ProductId.h"
#endif // MXL_HRCLS_DEVICE_TYPE_EXTERNAL

/*****************************************************************************************
    Macros
*****************************************************************************************/

// --------------------------------------------------------------------------------------
// Utility macro to extract product specific compiler switch setting based on MXL_HRCLS_PROD_ID
// --------------------------------------------------------------------------------------

#if defined(MXL_HRCLS_265_ENABLE) || defined(MXL_HRCLS_267_ENABLE) || defined(MXL_HRCLS_269_ENABLE)
#define _MXL_HRCLS_SERDES_ENABLED_
#endif

#if defined(MXL_HRCLS_258_ENABLE) || defined(MXL_HRCLS_269_ENABLE) || defined(MXL_HRCLS_254_ENABLE) || defined(MXL_HRCLS_256_ENABLE) || defined(MXL_HRCLS_255_ENABLE) || defined(MXL_HRCLS_252_ENABLE)
#define _MXL_HRCLS_OOB_ENABLED_
#endif

#if defined(MXL_HRCLS_258_ENABLE) || \
    defined(MXL_HRCLS_269_ENABLE) || \
    defined(MXL_HRCLS_254_ENABLE) || \
    defined(MXL_HRCLS_255_ENABLE) || \
    defined(MXL_HRCLS_256_ENABLE) || \
    defined(MXL_HRCLS_252_ENABLE) || \
    defined(MXL_HRCLS_212_ENABLE) || \
    defined(MXL_HRCLS_213_ENABLE) || \
    defined(MXL_HRCLS_214_ENABLE) || \
    defined(_MXL_HRCLS_WAKE_ON_WAN_ENABLED_)
#ifndef _MXL_HRCLS_WAKE_ON_WAN_ENABLED_
  #define _MXL_HRCLS_WAKE_ON_WAN_ENABLED_
#endif
#define _MXL_HRCLS_DEMOD_ENABLED_
#endif

#if defined(MXL_HRCLS_258_ENABLE) || \
    defined(MXL_HRCLS_254_ENABLE) || \
    defined(MXL_HRCLS_255_ENABLE) || \
    defined(MXL_HRCLS_256_ENABLE) || \
    defined(MXL_HRCLS_252_ENABLE) || \
    defined(MXL_HRCLS_212_ENABLE) || \
    defined(MXL_HRCLS_213_ENABLE) || \
    defined(MXL_HRCLS_214_ENABLE) 
#define _MXL_HRCLS_XPT_ENABLED_
#endif

#if defined(MXL_HRCLS_265_ENABLE) || \
    defined(MXL_HRCLS_267_ENABLE) || \
    defined(MXL_HRCLS_269_ENABLE) || \
    defined(MXL_HRCLS_212_ENABLE) || \
    defined(MXL_HRCLS_213_ENABLE) || \
    defined(MXL_HRCLS_214_ENABLE) || \
    defined(MXL_HRCLS_254_ENABLE) 
#define _MXL_HRCLS_IFOUT_ENABLED_
#endif

/*****************************************************************************************
    Extra options not SKU-dependent
 *****************************************************************************************/   

#ifndef MXL_HRCLS_OPTIONS_EXTERNAL
  #define _MXL_HRCLS_DEBUG_ENABLED_
  #define not_MXL_HRCLS_FLOAT_CALC_ENABLED_
  #define not_MXL_HRCLS_XPT_KNOWN_PIDS_ENABLED_
#endif

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

#endif // __MXL_HRCLS_FEATURES_H__
