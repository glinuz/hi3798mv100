//------------------------------------------------------------------------------
// Copyright 2007, Silicon Image, Inc.  All rights reserved.
//
// No part of this work may be reproduced, modified, distributed, transmitted,
// transcribed, or translated into any language or computer format, in any form
// or by any means without written permission of: Silicon Image, Inc.,
// 1060 East Arques Avenue, Sunnyvale, California 94085
//------------------------------------------------------------------------------
#ifndef __HAL_H__
#define __HAL_H__


#include "si_typedefs.h"
 

//-------------------------------------------------------------------------------
// Microprocessor HAL Function Prototypes
//-------------------------------------------------------------------------------
void UART_Init(void);

void TIMER_Init(void);
void TIMER_Set(HI_U8 timer, HI_U16 value);
HI_U8 TIMER_Expired(HI_U8 timer);
void TIMER_Wait(HI_U8 index, HI_U16 value);
HI_U16 TIMER_GetTickCounter( void );

HI_U8 GPIO_GetPins(HI_U8 pinMask);
void GPIO_SetPins(HI_U8 pinMask);
void GPIO_ClearPins(HI_U8 pinMask);


#endif  // _HAL_H__
