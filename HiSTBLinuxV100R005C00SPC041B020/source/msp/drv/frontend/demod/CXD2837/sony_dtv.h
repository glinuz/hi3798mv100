/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-04-12 16:44:28 #$
  File Revision : $Revision:: 5006 $
------------------------------------------------------------------------------*/
/**
 @file    sony_dtv.h

          This file provides DTV system specific definitions.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DTV_H
#define SONY_DTV_H

/*------------------------------------------------------------------------------
  Enumerations
------------------------------------------------------------------------------*/
/**
 @brief System (DVB-T/T2/C/C2/S/S2) 
*/
typedef enum {
    SONY_DTV_SYSTEM_UNKNOWN,        /**< Unknown. */
    SONY_DTV_SYSTEM_DVBT,           /**< DVB-T. */
    SONY_DTV_SYSTEM_DVBT2,          /**< DVB-T2. */
    SONY_DTV_SYSTEM_DVBC,           /**< DVB-C. */
    SONY_DTV_SYSTEM_DVBC2,          /**< DVB-C2. */
    SONY_DTV_SYSTEM_DVBS,           /**< DVB-S. */
    SONY_DTV_SYSTEM_DVBS2,          /**< DVB-S2. */
    SONY_DTV_SYSTEM_ANY             /**< Used for multiple system scanning / blind tuning */
} sony_dtv_system_t;

#endif /* SONY_DTV_H */
