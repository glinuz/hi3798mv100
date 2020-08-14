/****************************************************************************/
/*                   MONTAGE PROPRIETARY AND CONFIDENTIAL                   */
/*                   Montage Technology (Shanghai) Inc.                     */
/*                          All Rights Reserved                             */
/****************************************************************************/
/*
 * Filename:      M88TS2022.h
 *
 * Description:   Montage M88TS2022  Digital Satellite Tuner IC driver.
 *                
 * Author:        Kenn Wang
 *
 * Version:       1.12.01
 * Date:          2011-09-15 
****************************************************************************/

/* DEFINES: */
#ifndef __M88TS2022_H__
#define __M88TS2022_H__

#include "hi_type.h"

//HI_S32	SetTuner(HI_U32 u32TunerPort, HI_U32 fPLL, HI_U32 fSym, HI_U16 lpfOffset, HI_U8 gainHold);

extern HI_S32 M88TS2022_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 M88TS2022_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/);  //freq :unit MHZ, symbol:unit  Khz

#endif
