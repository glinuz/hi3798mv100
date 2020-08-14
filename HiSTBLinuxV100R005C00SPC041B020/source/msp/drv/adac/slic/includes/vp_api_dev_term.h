/** \file vp_api_dev_term.h
 * vp_api_dev_term.h
 *
 * This file contains declaration for VP-API Device and termination types.
 *
 * $Revision: 7395 $
 * $LastChangedDate: 2010-06-23 11:54:35 -0500 (Wed, 23 Jun 2010) $
 */

#ifndef VP_API_DEV_TERM_H
#define VP_API_DEV_TERM_H

/******************************************************************************
 *                                ENUMERATIONS                                *
 ******************************************************************************/
/* Valid Device Types supported by VP-API-II. For the corresponding part numbers
 * supported by each device type please see VP-API-II documentation.
 */
typedef enum
{
    VP_DEV_RSRVD1 = 0,
    VP_DEV_580_SERIES,
    VP_DEV_790_SERIES,
    VP_DEV_VCP_SERIES,
    VP_DEV_880_SERIES = 4,  /* Fixed value to match calibration profile */
    VP_DEV_RSVD_SERIES,
    VP_DEV_VCP2_SERIES,
    VP_DEV_KWRAP,
    VP_DEV_890_SERIES = 8,  /* Fixed value to match calibration profile */
    VP_DEV_792_SERIES,
    VP_DEV_792_GROUP,
    VP_NUM_DEV_TYPES,
    VP_DEV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceType;

/**
 * Deprecated name (VCP) based on older name for VCP. All new applications
 * should use VCP instead
 */
#define VP_DEV_DVP_SERIES  VP_DEV_VCP_SERIES

/** Valid Termination Types (per line) supported by API-II.  See the VP-API-II
    Reference Guide for descriptions. */
typedef enum
{
    /* FXS */
    VP_TERM_FXS_GENERIC            = 0x00, VP_TERM_FXS_FIRST = VP_TERM_FXS_GENERIC,
    VP_TERM_FXS_ISOLATE            = 0x01,
    VP_TERM_FXS_TITO_TL_R          = 0x02,
    VP_TERM_FXS_CO_TL              = 0x03,
    VP_TERM_FXS_75181              = 0x04,
    VP_TERM_FXS_75282              = 0x05,
    VP_TERM_FXS_RDT                = 0x06,
    VP_TERM_FXS_RR                 = 0x07,
    VP_TERM_FXS_SPLITTER           = 0x08,
    VP_TERM_FXS_TO_TL              = 0x09,
    VP_TERM_FXS_LOW_PWR            = 0x0A,
    VP_TERM_FXS_TI                 = 0x0B,
    VP_TERM_FXS_SPLITTER_LP        = 0x0C,
    VP_TERM_FXS_ISOLATE_LP         = 0x0D,
    VP_TERM_FXS_LCAS               = 0x0E,
    VP_TERM_FXS_RR_TI              = 0x0F,
    VP_TERM_FXS_CUSTOM             = 0x10,
    VP_TERM_FXS_TL_MW              = 0x11, VP_TERM_FXS_LAST = VP_TERM_FXS_TL_MW,

    /* MeLT */
    VP_TERM_MELT_RELAY_CAL_CT      = 0x40, VP_TERM_MELT_FIRST = VP_TERM_MELT_RELAY_CAL_CT,
    VP_TERM_MELT_RELAY_CAL_SPLIT   = 0x41,
    VP_TERM_MELT_RELAY_CAL_BRIDGED = 0x42,
    VP_TERM_MELT_LTAS_CT           = 0x43,
    VP_TERM_MELT_RELAY_CT          = 0x44,
    VP_TERM_MELT_RELAY_SPLIT       = 0x45,
    VP_TERM_MELT_RELAY_BRIDGED     = 0x46, VP_TERM_MELT_LAST = VP_TERM_MELT_RELAY_BRIDGED,

    /* FXO */
    VP_TERM_FXO_GENERIC            = 0x80, VP_TERM_FXO_FIRST = VP_TERM_FXO_GENERIC,
    VP_TERM_FXO_DISC               = 0x81,
    VP_TERM_FXO_CUSTOM             = 0x82, VP_TERM_FXO_LAST = VP_TERM_FXO_CUSTOM,

    VP_TERM_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpTermType;

#endif /* VP_API_DEV_TERM_H */
