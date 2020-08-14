/**
 * This product contains one or more programs protected under international
 * and U.S. copyright laws as unpublished works.  They are confidential and
 * proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
 * whole or in part, or the production of derivative works therefrom without
 * the express permission of Dolby Laboratories is prohibited.
 * Copyright 2011 - 2015 by Dolby Laboratories.
 * All rights reserved.
 *
 * @brief RPU decoder API.
 * @file rpu_api_decoder.h
 *
 * $Id$
 */

#ifndef _RPU_API_DECODER_H_
#define _RPU_API_DECODER_H_

#include <stdlib.h>
#include <stdio.h>
#include <rpu_api_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Defines
 ****************************************************************************/


/****************************************************************************
 * Enumerations
 ****************************************************************************/




/****************************************************************************
 * Type definitions
 ****************************************************************************/

/*!
    @brief
    The RPU decoder context.
    This context is used for all RPU decoder functions.
*/
typedef struct rpu_decoder_ctx_s rpu_decoder_ctx_t;

/****************************************************************************
 * Function definitions
 ****************************************************************************/

/*!
    @brief Initialize the RPU decoder.
    This function needs to be called before using the rpu decoder.
    @param [in]  rdc    pointer to the RPU decoder context
    @param [in]  flags  flags for the decoder initialization
                 see definition of \ref rpu_init_flags_t for details
    @return
        @li >=0     number of bytes allocated by rpu_decoder_init.
        @li  <0     error code
*/
LIBRPU_API int rpu_decoder_init(rpu_decoder_ctx_t** rdc, rpu_init_flags_t* flags);

/*!
    @brief
    Cleanup the RPU decoder.
    This function needs to be called to free all resources used by the rpu decoder.
    @param [in]  rdc  pointer to the RPU decoder context
*/
LIBRPU_API int rpu_decoder_cleanup(rpu_decoder_ctx_t* rdc);

/*!
    @brief Push a bitstream chunk into the rpu decoder.
    @details The bitstream chunk can be any size up to RPU_PRE_BUFFER_SIZE.
    If rpu_decoder_push_data is called repeatedly without calling rpu_decoder_get_output the input buffer might overflow. In this case rpu_decoder_push_data returns RPU_ERR_INPUT_BUFFER_FULL. No data is written to the input buffer.
    @param [in]  rdc          pointer to the RPU decoder context
    @param [in]  buffer       ponter to the input buffer
    @param [in]  buffer_size  size of the input buffer
    @param [in]  last         indicates the last buffer of the stream
    @return
        @li =0     no error
        @li <0     error code

*/
LIBRPU_API int rpu_decoder_push_data(rpu_decoder_ctx_t* rdc, unsigned char *buffer, int buffer_size, int last);


/*!
    @brief
    Read a rpu packet from the rpu decoder.
    @param [in]  rdc
        Pointer to the RPU decoder context.
    @param [out]  rpu_header
        Pointer to the rpu data structure.
    @param [out]  rpu_payload
        Pointer to the rpu payload data structure.
    @param [out]  rpu_payload_size
        Size of the rpu payload.
    @param [out]  dm_data_payload
        Pointer to the dm_data payload data structure.
    @param [out]  dm_data_payload_size
        Size of the dm_data payload.
    @return
        @li >=0    number of bits read from the bitstream
        @li <0     error code
*/
LIBRPU_API int rpu_decoder_get_output(rpu_decoder_ctx_t* rdc, rpu_data_header_t** rpu_header,
                                vdr_rpu_data_payload_t** rpu_payload, int* rpu_payload_size,
                                vdr_dm_data_payload_t** dm_data_payload, int* dm_data_payload_size);

/*!
    @brief
    Flushes the rpu decoder. After a flush all remaining RPU units in the decoder buffer can be read out.
    No data can be pushed in until the decoder receives a reset.
    @param [in]  rdc          pointer to the RPU decoder context
    @return
        @li =0     no error
        @li <0     error code
*/
LIBRPU_API int rpu_decoder_flush(rpu_decoder_ctx_t* rdc);

/*!
    @brief
    Resets the rpu decoder.
    The state of the decoder after a reset is the same as after initializing the decoder.
    No additional resources are allocated.
    @param [in]  rdc          pointer to the RPU decoder context
    @return
        @li =0     no error
        @li <0     error code
*/
LIBRPU_API int rpu_decoder_reset(rpu_decoder_ctx_t* rdc);


/*!
    @brief
    Indicate if the last RPU packet has been output
    @param [in]  rdc          pointer to the RPU decoder context
    @return
        @li 1     the last RPU packet has been output
        @li 0     either last RPU packet has not been output or we are not at end of stream yet
*/
LIBRPU_API int rpu_decoder_last_rpu_output(rpu_decoder_ctx_t* rdc);

/*!
    @brief
    This function dumps all rpu coeficients after the
    prediction from previous partitions.
    @param [in]  rpu_data
    Pointer to an rpu data structure.
*/
LIBRPU_API void rpu_coeff_dump(rpu_data_t* rpu_data);

#ifdef __cplusplus
}
#endif

#endif /* _RPU_API_DECODER_H_ */
