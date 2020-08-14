/*************************************************************
** file name:		AV2026.h
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/

#ifndef __AV2026_H__
#define __AV2026_H__

#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/* include -------------------------------------------------*/

#include "hd2501_type.h"

/* macro & structure ---------------------------------------*/

// AV2026
#define  AV2026_tuner_crystal 27 //unit is MHz

/* global & static variables -------------------------------*/
/* extern functions declaration ----------------------------*/

U8 AV2026_Tuner_I2C_write(U8 regAddr, U8* data, U8 length);
U8 AV2026_Tuner_control(U32 channel_freq, U32 bb_sym);

/* static functions declaration ----------------------------*/
/* functions define ----------------------------------------*/
/* static functions definition -----------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif  //#define __AV2026_H__

