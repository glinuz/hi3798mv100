/**
 * This product contains one or more programs protected under international
 * and U.S. copyright laws as unpublished works.  They are confidential and
 * proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
 * whole or in part, or the production of derivative works therefrom without
 * the express permission of Dolby Laboratories is prohibited.
 * Copyright 2011 - 2015 by Dolby Laboratories.
 * All rights reserved.
 *
 * @brief RPU encoder API.
 * @file rpu_api_encoder.h
 *
 * $Id$
 */

#ifndef _RPU_API_ENCODER_H_
#define _RPU_API_ENCODER_H_

#include <stdlib.h>
#include <stdio.h>
#include "rpu_api_common.h"

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
typedef struct rpu_encoder_ctx_s rpu_encoder_ctx_t;


/****************************************************************************
 * Function definitions
 ****************************************************************************/

LIBRPU_API int rpu_writer_init(rpu_encoder_ctx_t** rpu_encoder_ctx, rpu_data_t** rpu_data, rpu_init_flags_t* flags);

LIBRPU_API int rpu_writer_cleanup(rpu_encoder_ctx_t* rec, rpu_data_t* rpu_data);

LIBRPU_API int rpu_write(rpu_encoder_ctx_t* rec, rpu_data_t* rpu_data, unsigned char** nalu_buffer, int* nalu_len);



#ifdef __cplusplus
}
#endif

#endif /* _RPU_API_ENCODER_H_ */
