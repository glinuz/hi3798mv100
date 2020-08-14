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
#ifndef __M88TC3800_H__
#define __M88TC3800_H__

#include "hi_type.h"

extern HI_S32 m88tc3800_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 m88tc3800_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/);  //freq :unit MHZ, symbol:unit  Khz
extern HI_S32 m88tc3800_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_VOID m88tc3800_tuner_resume(HI_U32 u32TunerPort);

#endif
