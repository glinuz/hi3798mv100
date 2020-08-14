/**
* This product contains one or more programs protected under international
* and U.S. copyright laws as unpublished works.  They are confidential and
* proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
* whole or in part, or the production of derivative works therefrom without
* the express permission of Dolby Laboratories is prohibited.
* Copyright 2013 - 2015 by Dolby Laboratories.
* All rights reserved.
*
* @brief Dolby Vision VES splitter interface header definition file
* @file  dv_ves_dmx.h
*
* $Id$
*/

#ifndef __DV_VES_DMX_H__
#define __DV_VES_DMX_H__

#include "dv_cdefs.h"


#ifdef __cplusplus
extern "C" {
#endif


/*! @brief VES splitter callback notification event
*/
typedef enum _dv_ves_dmx_evt_e_
{
    VES_DMX_START_CODE = 0,                       /**<@brief START CODE found */
    VES_DMX_EMU_START_CODE_AT_EOB,
                  /**<@brief emulated START CODE at end of buffer found found */
    VES_DMX_NAL_PAYLOAD                                /**<@brief NAL payload */
} dv_ves_dmx_evt_t;

/*! @brief Demuxed NAL callback function prototype
*/
typedef void(*nal_dmx_cb_func_t)
(
    void             *p_ctx,                    /**<@brief pointer to context */
    dv_es_t           es,        /**<@brief identify the Dolby Vision ES type */
    dv_ves_dmx_evt_t  nal_dmx_evt,
                          /**<@brief VES splitter callback notification event */
    uint32_t          au_flags,                 /**<@brief AU descriptor tags */
    timestamp_t      *p_dts,
            /**<@brief pointer to DTS if AU_DTS_VALID_FLAG is set in au_flags */
    timestamp_t      *p_pts,
            /**<@brief pointer to DPS if AU_PTS_VALID_FLAG is set in au_flags */
    uint8_t          *p_nal_buf,
                                /**<@brief pointer to the demuxed NAL payload */
    uint32_t          nal_len                   /**<@brief NAL payload length */
    );

/*! @brief Dolby Vision VES splitter configuration structure
*/
typedef struct _dv_ves_dmx_conf_s_
{
    dv_ves_imp_t       ves_imp;
    uint8_t            f_derive_bl_pts;
    uint8_t            f_derive_el_pts;
    dv_es_codec_t      bl_codec;
    dv_es_codec_t      el_codec;
    msg_log_func_t     pf_msg_log;
    nal_dmx_cb_func_t  pf_nal_dmx_cb;
    void              *p_nal_dmx_cb_ctx;
} dv_ves_dmx_conf_t;

/*! @brief Dolby Vision VES splitter return code definition
*/
typedef enum _dv_ves_dmx_rc_e_
{
    DV_VES_DMX_OK = 0,
    DV_VES_DMX_UNINITIALIZE,                      /* module is not initialized yet */
    DV_VES_DMX_INVALID_CONTEXT_HANDLE,                   /* invalid context handle */
    DV_VES_DMX_INVALID_ARG,                                    /* invalid argument */
    DV_VES_DMX_INVALID_VES_TYPE,                               /* invalid VES type */
    DV_VES_DMX_OUT_OF_MEM,                                        /* out of memory */
    DV_VES_DMX_UNKNOWN_ERR
} dv_ves_dmx_rc_t;

/*! @brief Dolby Vision VES splitter handle
*/
typedef void  *dv_ves_dmx_handle_t;


/*! @brief Get the Dolby Vision VES splitter module
           API version string.

    @return
        A pointer to the API version string.
*/
char *dv_ves_dmx_get_api_ver
    (
    void
    );


/*! @brief Get the Dolby Vision VES splitter module
           alogrithm version string.

    @return
        A pointer to the algorithm version string.
*/
char *dv_ves_dmx_get_algo_ver
    (
    void
    );


/*! @brief Create the Dolby Vision VES splitter.

    @return
        @li non-zero if successful, a handle for the DOlby Vision VES splitter.
        @li NULL     if there was an error.
*/
dv_ves_dmx_handle_t dv_ves_dmx_create
    (
    void
    );


/*! @brief Destroy the Dolby Vision VES splitter.

    @param[in] h_ves_dmx  handle for the Dolby Vision VES splitter

    @return
        @li DV_VES_DMX_OK if successful.
        @li non-zero      if there was an error.
*/
dv_ves_dmx_rc_t dv_ves_dmx_destroy
    (
    dv_ves_dmx_handle_t  h_ves_dmx
    );


/*! @brief Initialize and configure the Dolby Vision VES splitter.

    @param[in] h_ves_dmx  handle for the Dolby Vision VES splitter
    @param[in] p_conf     pointer to the configuration block

    @return
        @li DV_VES_DMX_OK if successful.
        @li non-zero      if there was an error.
*/
dv_ves_dmx_rc_t dv_ves_dmx_init
    (
    dv_ves_dmx_handle_t   h_ves_dmx,
    dv_ves_dmx_conf_t    *p_conf
    );


/*! @brief Reset the Dolby Vision VES splitter to the state after initialization.

    @param[in] h_ves_dmx  handle for the Dolby Vision VES splitter

    @return
        @li DV_VES_DMX_OK if successful.
        @li non-zero      if there was an error.
*/
dv_ves_dmx_rc_t dv_ves_dmx_reset
    (
    dv_ves_dmx_handle_t  h_ves_dmx
    );


/*! @brief Demux the Dolby Vision VES buffer.

    @param[in] h_ves_dmx    handle for the Dolby Vision VES splitter
    @param[in] ves_id       identify the VES type
    @param[in] au_flags     AU VES buffer descriptor tags
    @param[in] p_dts        pointer to DTS if AU_DTS_VALID_FLAG is set in au_flags
    @param[in] p_dts        pointer to PTS if AU_PTS_VALID_FLAG is set in au_flags
    @param[in] p_ves_buf    pointer to the VES buffer
    @param[in] ves_buf_len  VES buffer length

    @return
        @li DV_VES_DMX_OK if successful.
        @li non-zero      if there was an error.
*/
dv_ves_dmx_rc_t dv_ves_dmx_process
    (
    dv_ves_dmx_handle_t   h_ves_dmx,
    dv_ves_id_t           ves_id,
    uint32_t              au_flags,
    timestamp_t          *p_dts,
    timestamp_t          *p_pts,
    uint8_t              *p_ves_buf,
    uint32_t              ves_buf_len
    );


/*! @brief Get the error string for the given Dolby Vision VES splitter
           error code.

    @param[in]  err_code the error code

    @return
        @li NULL     if the error code is not recognized.
        @li non-zero if the error code is valid, a pointer to error string.
*/
char *dv_ves_dmx_get_errstr
    (
    uint32_t  err_code
    );


#ifdef __cplusplus
}
#endif


#endif /* __DV_VES_DMX_H__ */

