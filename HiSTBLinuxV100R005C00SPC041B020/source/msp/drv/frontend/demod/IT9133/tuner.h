/*
 * @(#)Afatech_OMEGA.h
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


#ifndef __Afatech_OMEGA_H__
#define __Afatech_OMEGA_H__


#define OMEGA_VER_MAJOR    2
#define OMEGA_VER_MINOR    0


extern TunerDescription tunerDescription;


/**
 *
 */
Dword OMEGA_open (
	IN  Demodulator*	demodulator
);


/**
 *
 */
Dword OMEGA_close (
	IN  Demodulator*	demodulator
);


/**
 *
 */
Dword OMEGA_set (
	IN  Demodulator*	demodulator,
    IN  Word			bandwidth,
    IN  Dword			frequency
);


/**
 * support LNA Type.
 *
 * @param demodulator the handle of demodulator.
 * @param supporttype the type of LNA .
 *        0: OMEGA ; 1: OMEGA_LNA_Config_1 ; 1: OMEGA_LNA_Config_2
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     error = OMEGA_supportLNA(demod, 0); // 0: OMEGA ; 1: OMEGA_LNA_Config_1 ; 1: OMEGA_LNA_Config_2
 *     if (error){
 *		  printf("Initialize LNA type failed.0x%08x\n", error);
 *		  return;
 *	   }
 *     error = Demodulator_initialize (demod, streamType);
 * </pre>
 */
Dword OMEGA_supportLNA (
    IN  Demodulator*    demodulator,
    IN  Byte            supporttype
 );
#endif
