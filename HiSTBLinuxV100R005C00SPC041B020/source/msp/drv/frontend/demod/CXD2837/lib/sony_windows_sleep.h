/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-06-22 19:06:21 #$
  File Revision : $Revision:: 5632 $
------------------------------------------------------------------------------*/
/**
 @file    sony_windows_sleep.h

          This file provides the Windows specific sleep function.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_WINDOWS_SLEEP_H
#define SONY_WINDOWS_SLEEP_H

/*------------------------------------------------------------------------------
 Function Prototypes
------------------------------------------------------------------------------*/
/**
 @brief High precision sleep function for Windows PC.
        This function ensures that sleeping time is not shorter than specified time.

 @param sleepTimeMs Sleep time in ms.

 @return 1 if successful. 0 if failed.
*/
int sony_windows_Sleep (unsigned long sleepTimeMs);

#endif /* SONY_WINDOWS_SLEEP_H */

