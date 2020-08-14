/**
* This product contains one or more programs protected under international
* and U.S. copyright laws as unpublished works.  They are confidential and
* proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
* whole or in part, or the production of derivative works therefrom without
* the express permission of Dolby Laboratories is prohibited.
* Copyright 2011 - 2015 by Dolby Laboratories.
* All rights reserved.
*
* @brief Dolby Vision metadata parser interface header definition file
* @file  dv_md_parser.h
*
* $Id$
*/

#ifndef __DV_MD_PARSER_H__
#define __DV_MD_PARSER_H__

#include "dv_cdefs.h"
#include "rpu_api_decoder.h"


#ifdef __cplusplus
extern "C" {
#endif


/*! @brief Dolby Vision metadata parser callback notification event
*/
typedef enum _dv_md_parser_evt_e_
{
    MD_PARSER_SAMPLE_AVAIL_EVT = 0,        /**<@brief metadata sample decoded */
    MD_PARSER_ERROR_EVT,
    MD_PARSER_EVT_MAX
} dv_md_parser_evt_t;

/*! @brief Dolby Vision metadata parser notification event callback function
           prototype
*/
typedef void (*md_parser_evt_cb_func_t)
    (
    void                   *p_ctx,         /**<@brief pointer to user context */
    dv_md_parser_evt_t      md_parser_evt,
                                /**<@brief metadata parser notification event */
	timestamp_t            *p_pts,
                                /**<@brief PTS for the parsed metadata sample */
    rpu_data_header_t      *hdr,                   /**<@brief metadata header */
    vdr_rpu_data_payload_t *rpu_payload,      /**<@brief RPU metadata payload */
    int                     rpu_payload_sz,
                                         /**<@brief RPU metadata payload size */
    vdr_dm_data_payload_t  *dm_payload,
                               /**<@brief display management metadata payload */
    int                     dm_payload_sz,
                          /**<@brief display management metadata payload size */
    int                     len
                            /**<@brief number of bits read from the bitstream */
    );

/*! @brief Dolby Vision metadata parser configuration structure
*/
typedef struct _dv_md_parser_conf_s_
{
	md_parser_evt_cb_func_t  pf_md_parser_cb;
                                /**<@brief notification event callback function */
	void                    *p_md_parser_cb_ctx;        /**<@brief user context */
    rpu_init_flags_t         flags; /**<@brief RPU decoder initialization flags */
	msg_log_func_t           pf_msg_log;    /**<@brief message logging function */
} dv_md_parser_conf_t;

/*! @brief Dolby Vision metadata parser return code definition
*/
typedef enum _dv_md_parser_rc_e_
{
    DV_MD_PARSER_OK = 0,
    DV_MD_PARSER_UNINITIALIZE,       /**<@brief module is not initialized yet */
    DV_MD_PARSER_INVALID_CONTEXT_HANDLE,    /**<@brief invalid context handle */
    DV_MD_PARSER_OUT_OF_MEMORY,      /**<@brief metadata parser out of memory */
    DV_MD_PARSER_INVALID_BISTREAM_DATA,      /**<@brief invalid bistream data */
    DV_MD_PARSER_NOT_SUPPORTED_ERR,          /**<@brief not supported feature */
    DV_MD_PARSER_INVALID_LEVEL,     /**<@brief violation of level limitations */
    DV_MD_PARSER_INVALID_PROFILE,
                                 /**<@brief violation of  profile limitations */
    DV_MD_PARSER_INVALID_SPATIAL_FILTER,
                           /**<@brief violation of spatial filter limitations */
    DV_MD_PARSER_INVALID_ARGUMENT,               /**<@brief invalid arguments */
    DV_MD_PARSER_INTERNAL_ERR,
                  /**<@brief brief internal unspecified metadata parser error */
    DV_MD_PARSER_VERSION_ERR,                         /* wrong version number */
    DV_MD_PARSER_UNKNOWN_ERR
} dv_md_parser_rc_t;

/*! @brief Dolby Vision metadata parser handle
*/
typedef void  *dv_md_parser_handle_t;


/*! @brief Get the Dolby Vision metadata parser module API version string.

    @return
        A pointer to the API version string.
*/
char *dv_md_parser_get_api_ver
    (
    void
    );


/*! @brief Get the Dolby Vision metadata parser module alogrithm version string.

    @return
        A pointer to the algorithm version string.
*/
char *dv_md_parser_get_algo_ver
    (
    void
    );


/*! @brief Create the Dolby Vision metadata parser instance.

    @return
        @li non-zero if successful, a handle for the Dolby Vision metadata
                     parser instance.
        @li NULL     if there was an error.
*/
dv_md_parser_handle_t dv_md_parser_create
	(
	void
	);


/*! @brief Destroy the Dolby Vision metadata parser instance. This function will 
           release all the resources allocated at initialization.

    @param[in] h_md_parser  handle for the Dolby Vision metadata parser instance

    @return
        @li DV_MD_PARSER_OK if successful.
        @li non-zero        if there was an error.
*/
dv_md_parser_rc_t dv_md_parser_destroy
	(
	dv_md_parser_handle_t  h_md_parser
	);


/*! @brief Initialize and configure the Dolby Vision metadata parser instance.
           This function will also allocated any resources required by metadata
           parser.

    @param[in] h_md_parser  handle for the Dolby Vision metadata parser instance
    @param[in] p_conf       pointer to the configuration block

    @return
        @li DV_MD_PARSER_OK if successful.
        @li non-zero        if there was an error.
*/
dv_md_parser_rc_t dv_md_parser_init
	(
	dv_md_parser_handle_t  h_md_parser,
	dv_md_parser_conf_t   *p_conf
	);


/*! @brief Reset the Dolby Vision metadata parser to the state after initialization.

    @param[in] h_md_parser  handle for the Dolby Vision metadata parser instance

    @return
        @li DV_MD_PARSER_OK if successful.
        @li non-zero        if there was an error.
*/
dv_md_parser_rc_t dv_md_parser_reset
	(
	dv_md_parser_handle_t  h_md_parser
	);


/*! @brief Parse the Dolby Vision metadata buffer.

    @param[in] h_md_parser  handle for the Dolby Vision metadata parser instance
    @param[in] au_flags     metadata AU buffer descriptor tags
    @param[in] p_pts        pointer to PTS if AU_PTS_VALID_FLAG is set in au_flags
    @param[in] p_md_buf     pointer to the metadata buffer
    @param[in] md_buf_len   metadata buffer length
    @param[in] last         indicates the last buffer of the stream

    @return
        @li DV_MD_PARSER_OK if successful.
        @li non-zero        if there was an error.
*/
dv_md_parser_rc_t dv_md_parser_process
	(
	dv_md_parser_handle_t  h_md_parser,
    uint32_t               au_flags,
	timestamp_t           *p_pts,
	uint8_t               *p_md_buf,
	uint32_t               md_buf_len,
    int                    last
	);

/*! @brief Return the last Dolby Vision metadata parser error.

    @param[in] h_md_parser  handle for the Dolby Vision metadata parser instance

    @return
        The last error
*/
dv_md_parser_rc_t dv_md_parser_get_last_err
	(
	dv_md_parser_handle_t  h_md_parser
	);

/*! @brief Get the error string for the given Dolby Vision metadata parser
           error code.

    @param[in]  err_code the error code

    @return
        @li NULL     if the error code is not recognized.
        @li non-zero if the error code is valid, a pointer to error string.
*/
char *dv_md_parser_get_errstr
	(
	uint32_t  err_code
	);


/*! @brief This function dumps all rpu coeficients after the
           prediction from previous partitions.

    @param [in] p_rpu_data  pointer to an rpu data structure.
*/
void dv_md_parser_coeff_dump(rpu_data_t*  p_rpu_data);



#ifdef __cplusplus
}
#endif


#endif /* __DV_MD_PARSER_H__ */

