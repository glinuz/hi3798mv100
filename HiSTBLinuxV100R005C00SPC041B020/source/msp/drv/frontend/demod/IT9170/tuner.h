/*
 * @(#)Afatech_SAMBA.h
 *
 * ==========================================================
 * Version: 2.0
 * Date:    2009.06.15
 * ==========================================================
 *
 * ==========================================================
 * History:
 *
 * Date         Author      Description
 * ----------------------------------------------------------
 *
 * 2009.06.15   M.-C. Ho    new tuner
 * ==========================================================
 *
 * Copyright 2007 Afatech, Inc. All rights reserved.
 *
 */


#ifndef __Afatech_SAMBA_H__
#define __Afatech_SAMBA_H__


#define SAMBA_VER_MAJOR    2
#define SAMBA_VER_MINOR    0


extern TunerDescription tunerDescription;


/**
 *
 */
Dword SAMBA_open (
	IN  Demodulator*	demodulator
);


/**
 *
 */
Dword SAMBA_close (
	IN  Demodulator*	demodulator
);


/**
 *
 */
Dword SAMBA_set (
	IN  Demodulator*	demodulator,
    IN  Word			bandwidth,
    IN  Dword			frequency
);


/**
 * set Config type.
 *
 * @param demodulator the handle of demodulator.
 * @param configtype the type of Config .
 *        0: SAMBA ; 1: Config_1 ; 
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     error = SAMBA_setConfig(demod, 0); 
 *     if (error){
 *		  printf("Initialize Config type failed.0x%08x\n", error);
 *		  return;
 *	   }
 *     error = Demodulator_initialize (demod, streamType);
 * </pre>
 */
Dword SAMBA_setConfig  (
    IN  Demodulator*    demodulator,
    IN  Byte            configtype
 );
#endif
