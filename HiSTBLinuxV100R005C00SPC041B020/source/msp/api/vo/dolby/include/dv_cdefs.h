/**
* This product contains one or more programs protected under international
* and U.S. copyright laws as unpublished works.  They are confidential and
* proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
* whole or in part, or the production of derivative works therefrom without
* the express permission of Dolby Laboratories is prohibited.
* Copyright 2013 - 2015 by Dolby Laboratories.
* All rights reserved.
*
* @brief Dolby Vision common definitions
* @file  dv_cdefs.h
*
* $Id$
*/

#ifndef __DV_CDEFS_H__
#define __DV_CDEFS_H__

#include <stdint.h>


#define DV_MAX_NUM_PLANES              3 
       /**<@brief maximum number of planes in a Dolby Vision raw image buffer */

/*! @brief Access unit tag definition
*/
#define AU_START_FLAG                  (1 << 0)
#define AU_END_FLAG                    (1 << 1)
#define AU_PTS_VALID_FLAG              (1 << 3)
#define AU_DTS_VALID_FLAG              (1 << 4)
#define AU_EOS_FLAG                    (1 << 5)


#ifdef __cplusplus
extern "C" {
#endif


/*! @brief Dolby Vision elementary stream codec type
*/
typedef enum _dv_es_codec_e_
{
    DV_AVC_CODEC = 0,                  /**<@brief AVC/H.264 elementary stream */
    DV_HEVC_CODEC,                    /**<@brief HEVC/H.265 elementary stream */
    DV_MAX_CODEC
} dv_es_codec_t;

/*! @brief Dolby Vision elementary stream type
*/
typedef enum _dv_es_e_
{
    DV_BASE_LAYER_ES,                 /**<@brief base layer elementary stream */
    DV_ENHANCEMENT_LAYER_ES,   /**<@brief enhancement layer elementary stream */
    DV_METADATA_ES,                     /**<@brief metadata elementary stream */
	DV_MAX_ES
} dv_es_t;

/*! @brief Dolby Vision VES implementation type
*/
typedef enum _dv_ves_imp_e_
{
    DV_SINGLE_VES_IMP,                   /**<@brief single VES implementation */
    DV_DUAL_VES_IMP,                       /**<@brief dual VES implementation */
	DV_MAX_VES_IMP
} dv_ves_imp_t;

/*! @brief Dolby Vision VES type
*/
typedef enum _dv_ves_id_e_
{
    DV_COMBO_VES,                                        /**<@brief combo VES */
    DV_BASE_LAYER_VES,                         /**<@brief base layer only VES */
    DV_ENHANCEMENT_LAYER_VES,                /**<@brief enhancement layer VES */
} dv_ves_id_t;

/*! @brief Dolby Vision color channel 
*/
typedef enum _dv_color_channel_e_
{
    /* YCbCr */
    DV_CHAN_Y    = 0,
    DV_CHAN_Cb   = 1,
    DV_CHAN_Cr   = 2,
    DV_CHAN_CbCr = 1,

    /* GBR */
    DV_CHAN_G    = 0,
    DV_CHAN_B    = 1,
    DV_CHAN_R    = 2,
} dv_color_channel_t;

/*! @brief Dolby Vision pixel format
*/
typedef enum _dv_pix_fmt_e_
{
    DV_IMC2_PIX_FMT = 0,                    /**<@brief YUV4:2:0 planar format */
    DV_NV12_PIX_FMT = 1,  /**<@brief YUV4:2:0 Y planar, UV interleaved format */
    DV_I420_PIX_FMT = 2,                    /**<@brief YUV4:2:0 planar format */
    DV_I422_PIX_FMT = 15,              /**<@brief YUV4:2:2 interleaved format */
    DV_RGB_PIX_FMT  = 16,                   /**<@brief RGB interleaved format */
    DV_RGBA_PIX_FMT = 17,                  /**<@brief RGBA interleaved format */
    DV_MAX_PIX_FMT
} dv_pix_fmt_t;

/*! @brief Timestamp
*/
typedef struct _timestamp_s_
{
	uint64_t time_val;                            /**<@brief time stamp value */
    uint32_t time_scale;        /**<@brief time scale of the time stamp value */
} timestamp_t;

/*! @brief Message log function prototype
*/
typedef int (*msg_log_func_t)
	(
	const char *format, ...
	);


#ifdef __cplusplus
}
#endif


#endif /* __DV_CDEFS_H__ */

