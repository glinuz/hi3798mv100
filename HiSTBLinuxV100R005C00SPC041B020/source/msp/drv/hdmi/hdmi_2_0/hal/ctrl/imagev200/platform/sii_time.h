/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file	 sii_time.h
*
* @brief Time related APIs
*
*****************************************************************************/
#ifndef _SII_TIME_H_
#define _SII_TIME_H_
#include "si_datatypes.h"

typedef struct {
    int day;
    int hour;
    int min;
    int sec;
    int msec;
} SII_LOCAL_TIME;


//
// This function will return time with ms
//

void Sii_Init_ProcessTime(void);
unsigned long Sii_Get_Process_Time(void);
extern unsigned long SiI_get_global_time( void );
unsigned long SiI_difftime( unsigned long t );
extern void SiI_DelayMS( unsigned long delay );


extern bool_t SiI_TimerDelay(unsigned long baseTime, unsigned long delay);
void sii_mcutime( SII_LOCAL_TIME *tm );
unsigned long sii_mktime( SII_LOCAL_TIME *tm ); // Not supported in Win


#endif // _SII_TIME_H_
