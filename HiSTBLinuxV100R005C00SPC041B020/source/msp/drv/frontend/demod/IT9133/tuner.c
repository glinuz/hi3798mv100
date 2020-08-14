/**
 * @(#)Afatech_OMEGA.cpp
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
 * Copyright 2009 Afatech, Inc. All rights reserved.
 *
 */


#include "type.h"
#include "error.h"
#include "user.h"
#include "register.h"
#define __OMAGADEMOD_H__
#include "standard.h"
#include "tuner.h"
#include "Omega.h"
#include "Firmware_Afa_Omega_Script.h"
#include "Firmware_Afa_Omega_LNA_Config_1_Script.h"
#include "Firmware_Afa_Omega_LNA_Config_2_Script.h"

#define NULL 0
Demodulator* Afatech_OMEGA_demodulator;

Dword OMEGA_open (
	IN  Demodulator*	demodulator
) {
	Dword error = Error_NO_ERROR;
	Afatech_OMEGA_demodulator = demodulator;
	error = omega_init();

	return (error);
}

Dword OMEGA_close (
	IN  Demodulator*	demodulator
) {
	return (Error_NO_ERROR);
}

Dword OMEGA_set (
	IN  Demodulator*	demodulator,
	IN  Word			bandwidth,
	IN  Dword			frequency
) {
	Dword error = Error_NO_ERROR;
	Afatech_OMEGA_demodulator = demodulator;
	error = omega_setfreq((unsigned int)bandwidth, (unsigned int)frequency);
	
	return (error);
}


TunerDescription tunerDescription= {
    OMEGA_open,
    OMEGA_close,
    OMEGA_set,
    NULL,
    NULL,
    OMEGA_ADDRESS,			/** tuner i2c address */
    2,						/** length of tuner register address */
    0,						/** tuner if */
    False,					/** spectrum inverse */
    0x38,					/** tuner id */
};

Dword OMEGA_supportLNA (
    IN  Demodulator*    demodulator,
    IN  Byte            supporttype
 ) {
     Dword error = Error_INVALID_DEV_TYPE;

	 switch (supporttype){
		case OMEGA_NORMAL:
			tunerDescription.tunerScriptTable = OMEGA_scripts;
            tunerDescription.tunerScriptSetsTable = OMEGA_scriptSets;
            tunerDescription.tunerId = 0x38;
            error = Error_NO_ERROR;
			break;
		case OMEGA_LNA_Config_1:
			tunerDescription.tunerScriptTable = OMEGA_LNA_Config_1_scripts;
            tunerDescription.tunerScriptSetsTable = OMEGA_LNA_Config_1_scriptSets;
            tunerDescription.tunerId = 0x51;
            error = Error_NO_ERROR;
			break;
		case OMEGA_LNA_Config_2:
			tunerDescription.tunerScriptTable = OMEGA_LNA_Config_2_scripts;
            tunerDescription.tunerScriptSetsTable = OMEGA_LNA_Config_2_scriptSets;
            tunerDescription.tunerId = 0x52;
            error = Error_NO_ERROR;
			break;
		default:

			break;

	 }
	 
	 return error;
  }
