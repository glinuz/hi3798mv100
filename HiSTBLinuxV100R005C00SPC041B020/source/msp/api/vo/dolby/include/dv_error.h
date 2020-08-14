/**
* This product contains one or more programs protected under international
* and U.S. copyright laws as unpublished works.  They are confidential and
* proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
* whole or in part, or the production of derivative works therefrom without
* the express permission of Dolby Laboratories is prohibited.
* Copyright 2013 - 2015 by Dolby Laboratories.
* All rights reserved.
*
* @brief Dolby Vision modules error reporting definition
* @file  dv_error.h
*
* $Id$
*/

#ifndef __DV_ERROR_H__
#define __DV_ERROR_H__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/*! brief Dolby Vision software module ID definition
*/
typedef enum _dv_mod_id_e_
{
    VES_DMX_MODULE_ID = 0,
    METADATA_PARSER_MODULE_ID,
    COMPOSER_MODULE_ID,
    DISPLAY_MGNT_MODULE_ID,
    EDR_OVER_HDMI_MODULE_ID,
    DITHER_MODULE_ID,
    EDBEC_MODULE_ID,
	MAX_MODULE_ID
} dv_mod_id_t;


/*! @brief Error reporting macro for the Dolby Vision software modules
*/
#define DV_ERROR(m,x)                  (((m) << 24) | (x))
#define VES_DMX_ERROR(x)               (DV_ERROR(VES_DMX_MODULE_ID, (x)))
#define METADATA_PARSER_ERROR(x)       (DV_ERROR(METADATA_PARSER_MODULE_ID, (x)))
#define COMPOSER_ERROR(x)              (DV_ERROR(COMPOSER_MODULE_ID, (x)))
#define DISPLAY_MGNT_ERROR(x)          (DV_ERROR(DISPLAY_MGNT_MODULE_ID, (x)))
#define EDR_OVER_HDMI_ERROR(x)         (DV_ERROR(EDR_OVER_HDMI_MODULE_ID, (x)))
#define DITHER_ERROR(x)                (DV_ERROR(DITHER_MODULE_ID, (x)))
#define EDBEC_ERROR(x)                 (DV_ERROR(EDBEC_MODULE_ID, (x)))


/*! @brief Get the error string for the given error code

    @param[in]  err_code the error code

    @return
    @li NULL     if the error code is not recognized
    @li non-zero if the error code is valid, a pointer to error string
*/
char *dv_get_errstr(uint32_t  err_code);


#ifdef __cplusplus
}
#endif


#endif /* __DV_ERROR_H__ */

