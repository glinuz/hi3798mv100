/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-03-30 01:30:20 #$
  File Revision : $Revision:: 4952 $
------------------------------------------------------------------------------*/
/**
 @file    sony_dvbc.h

          This file provides DVB-C related type definitions.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVBC_H
#define SONY_DVBC_H

/*------------------------------------------------------------------------------
 Enumerations
------------------------------------------------------------------------------*/
/**
 @brief DVB-C constellation.
*/
typedef enum {
    SONY_DVBC_CONSTELLATION_16QAM,          /**< 16-QAM */
    SONY_DVBC_CONSTELLATION_32QAM,          /**< 32-QAM */
    SONY_DVBC_CONSTELLATION_64QAM,          /**< 64-QAM */
    SONY_DVBC_CONSTELLATION_128QAM,         /**< 128-QAM */
    SONY_DVBC_CONSTELLATION_256QAM          /**< 256-QAM */
} sony_dvbc_constellation_t;

#endif /* SONY_DVBC_H */
