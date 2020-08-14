#ifndef __USER_H__
#define __USER_H__


//#include <stdio.h>
#include "type.h"
#include "error.h"

#define User_MAX_PKT_SIZE               255
#define User_USE_SHORT_CMD              0

#define User_RETRY_MAX_LIMIT            100


/** Define I2C master speed, the default value 0x07 means 366KHz (1000000000 / (24.4 * 16 * User_I2C_SPEED)). */
#define User_I2C_SPEED              0x07

/** Define I2C address of secondary chip when Diversity mode or PIP mode is active. */
#define User_I2C_ADDRESS            0x38




/**
 * Delay Function
 */
Dword User_delay (
    IN  Demodulator*    demodulator,
    IN  Dword           dwMs
);

void Task_delay(IN  Dword dwTickNum);


/**
 * Enter critical section
 */
Dword User_enterCriticalSection (
    IN  Demodulator*    demodulator
);


/**
 * Leave critical section
 */
Dword User_leaveCriticalSection (
    IN  Demodulator*    demodulator
);


/**
 * Config MPEG2 interface
 */
Dword User_mpegConfig (
    IN  Demodulator*    demodulator
);


/**
 * Write data via "Control Bus"
 * I2C mode : uc2WireAddr mean demodulator chip address, the default value is 0x38
 * USB mode : uc2WireAddr is useless, don't have to send this data
 */
Dword User_busTx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    IN  Byte*           buffer
);


/**
 * Read data via "Control Bus"
 * I2C mode : uc2WireAddr mean demodulator chip address, the default value is 0x38
 * USB mode : uc2WireAddr is useless, don't have to send this data
 */
Dword User_busRx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    OUT Byte*           buffer
);


#endif
