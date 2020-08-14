/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-03-28 01:40:32 #$
  File Revision : $Revision:: 4945 $
------------------------------------------------------------------------------*/

#ifndef SONY_I2C_LOG_H
#define SONY_I2C_LOG_H

//#include <stdio.h>
#include "sony_i2c.h"

/**
 @brief The i2c logging structure. This struct instance is stored in "user" member of sony_i2c_t struct. 
*/
typedef struct sony_i2c_log_t {
    sony_i2c_t* pI2cReal;   /**< "Real" i2c struct instance */
    FILE* fp;               /**< FILE pointer for saving log data */
} sony_i2c_log_t;

/**
  @brief I2c struct instance creation (for logging)

  @param pI2c         Instance of i2c control struct
  @param pI2cReal     Instance of "Real" i2c control struct
  @param pI2cLog      Instance of sony_i2c_log_t struct
  @return SONY_RESULT_OK if success
*/
sony_result_t sony_i2c_CreateI2cLog(sony_i2c_t *pI2c, sony_i2c_t *pI2cReal, sony_i2c_log_t *pI2cLog);

/**
  @brief Enable/Disable i2c logging

  @param pI2c         Instance of i2c control struct
  @param fp           File pointer for saving log (NULL->disabling logging)
  @return SONY_RESULT_OK if success
*/
sony_result_t sony_i2c_EnableI2cLog(sony_i2c_t *pI2c, FILE *fp);

#endif /* SONY_I2C_LOG_H */
