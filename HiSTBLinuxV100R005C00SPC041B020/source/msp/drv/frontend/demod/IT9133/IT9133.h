#ifndef __GANYMEDE_H__
#define __GANYMEDE_H__


#include "type.h"
#include "user.h"
#include "error.h"
#include "register.h"
#include "variable.h"
#include "cmd.h"
#define __OMAGADEMOD_H__
#include "standard.h"
#include "version.h"

/**
 * Write one byte (8 bits) to a specific register in demodulator.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the address of the register to be written.
 * @param value the value to be written.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     // Set the value of register 0xA000 in demodulator to 0.
 *     error = Demodulator_writeRegister ((Demodulator*) &demod, Processor_LINK, 0xA000, 0);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_writeRegister (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            value
);


/**
 * Write a sequence of bytes to the contiguous registers in demodulator.
 * The maximum burst size is restricted by the capacity of bus. If bus
 * could transfer N bytes in one cycle, then the maximum value of
 * bufferLength would be N - 5.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the start address of the registers to be written.
 * @param bufferLength the number of registers to be written.
 * @param buffer a byte array which is used to store values to be written.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Byte buffer[3] = { 0x00, 0x01, 0x02 };
 *     DefaultDemodulator demod;
 *
 *     // Set the value of register 0xA000 in demodulator to 0.
 *     // Set the value of register 0xA001 in demodulator to 1.
 *     // Set the value of register 0xA002 in demodulator to 2.
 *     error = Demodulator_writeRegisters ((Demodulator*) &demod, Processor_LINK, 0xA000, 3, buffer);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_writeRegisters (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            bufferLength,
    IN  Byte*           buffer
);


/**
 * Modify bits in the specific register.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the address of the register to be written.
 * @param position the start position of bits to be modified (0 means the
 *        LSB of the specifyed register).
 * @param length the length of bits.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     // Modify the LSB of register 0xA000 in demodulator to 0.
 *     error = Demodulator_writeRegisterBits ((Demodulator*) &demod, Processor_LINK, 0xA000, 0, 1, 0);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_writeRegisterBits (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            position,
    IN  Byte            length,
    IN  Byte            value
);


/**
 * Read one byte (8 bits) from a specific register in demodulator.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the address of the register to be read.
 * @param value the pointer used to store the value read from demodulator
 *        register.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Byte value;
 *     DefaultDemodulator demod;
 *
 *     // Get the value of register 0xA000 in demodulator.
 *     error = Demodulator_readRegister ((Demodulator*) &demod, Processor_LINK, 0xA000, &value);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *     printf ("The value of 0xA000 is %2x", value);
 * </pre>
 */
Dword Demodulator_readRegister (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    OUT Byte*           value
);


/**
 * Read a sequence of bytes from the contiguous registers in demodulator.
 * The maximum burst size is restricted by the capacity of bus. If bus
 * could transfer N bytes in one cycle, then the maximum value of
 * bufferLength would be N - 5.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the address of the register to be read.
 * @param bufferLength the number of registers to be read.
 * @param buffer a byte array which is used to store values to be read.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Byte buffer[3];
 *     DefaultDemodulator demod;
 *
 *     // Get the value of register 0xA000, 0xA001, 0xA002 in demodulator.
 *     error = Demodulator_readRegisters ((Demodulator*) &demod, Processor_LINK, 0xA000, 3, buffer);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *     printf ("The value of 0xA000 is %2x", buffer[0]);
 *     printf ("The value of 0xA001 is %2x", buffer[1]);
 *     printf ("The value of 0xA002 is %2x", buffer[2]);
 * </pre>
 */
Dword Demodulator_readRegisters (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            bufferLength,
    OUT Byte*           buffer
);


/**
 * Read bits of the specified register.
 *
 * @param demodulator the handle of demodulator.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param registerAddress the address of the register to be read.
 * @param position the start position of bits to be read (0 means the
 *        LSB of the specifyed register).
 * @param length the length of bits.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Byte value;
 *     DefaultDemodulator demod;
 *
 *     // Read the LSB of register 0xA000 in demodulator.
 *     error = Demodulator_readRegisterBits ((Demodulator*) &demod, Processor_LINK, 0xA000, 0, 1, &value);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *     printf ("The value of LSB of 0xA000 is %2x", value);
 * </pre>
 */
Dword Demodulator_readRegisterBits (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            position,
    IN  Byte            length,
    OUT Byte*           value
);


/**
 * Get the version of firmware.
 *
 * @param demodulator the handle of demodulator.
 * @param version the version of firmware.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Dword version;
 *     DefaultDemodulator demod;
 *
 *     // Get the version of Link layer firmware.
 *     error = Demodulator_getFirmwareVersion ((Demodulator*) &demod, Processor_LINK, &version);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("The version of firmware is : %X", version);
 * </pre>
 */
Dword Demodulator_getFirmwareVersion (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    OUT Dword*          version
);


/**
 * Get post VitBer
 *
 * @param demodulator the handle of demodulator.
 * @param postErrorCount error count after viterbi
 * @param postBitCount total count after viterbi
 * @param abortCount error count after reed-soloman
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_getPostVitBer_it9133 (
    IN  Demodulator*    demodulator,
    OUT Dword*          postErrorCount,  /** 24 bits */
    OUT Dword*          postBitCount,    /** 16 bits */
    OUT Word*           abortCount
);

/**
 * Get siganl quality Indication.
 *
 * @param demodulator the handle of demodulator.
 * @param quality The value of signal quality.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Demodulator_getSignalQualityIndication (
    IN  Demodulator*    demodulator,
    OUT Byte*           quality
);

/**
 * Get signal strength
 *
 * @param demodulator the handle of demodulator.
 * @param strength The value of signal strength that calculations of ¡§score mapping¡¨ from the signal strength (dBm) to the ¡§0-100¡¨ scoring.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_getSignalStrengthIndication (
    IN  Demodulator*    demodulator,
    OUT Byte*           strength
);


/**
 * Get signal strength in dbm
 *
 * @param demodulator the handle of demodulator.
 * @param rfpullUpVolt_X10 the pullup voltag of RF multiply 10.
 * @param ifpullUpVolt_X10 the pullup voltag of IF multiply 10.
 * @param strengthDbm The value of signal strength in DBm.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_getSignalStrengthDbm (
    IN  Demodulator*    demodulator,
    IN  Long            rfpullUpVolt_X10,     /** RF pull up voltage multiplied by 10   */
    IN  Long            ifpullUpVolt_X10,     /** IF pull up voltage multiplied by 10   */
    OUT Long*           strengthDbm           /** DBm                                   */
);



/**
 * First, download firmware from host to demodulator. Actually, firmware is
 * put in firmware.h as a part of source code. Therefore, in order to
 * update firmware the host have to re-compile the source code.
 * Second, setting all parameters which will be need at the beginning.
 *
 * @param demodulator the handle of demodulator.
 * @param streamType The format of output stream.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     // Initialize demodulators.
 *     // SAW Filter  : 8MHz
 *     // Stream Type : IP Datagram.
 *     error = Demodulator_initialize ((Demodulator*) &demod, StreamType_IP_DATAGRAM);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_initialize_it9133 (
    IN  Demodulator*    demodulator,
    IN  StreamType      streamType
);


/**
 * Power off the demodulators.
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     // Finalize demodulators.
 *     error = Demodulator_finalize ((demod*) &ganymede);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_finalize (
    IN  Demodulator*    demodulator
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @param locked the result of frequency tuning. True if there is
 *        demodulator can lock signal, False otherwise.
 * @see Demodulator_acquireChannel_it9133
 */
Dword Demodulator_isLocked_it9133 (
    IN  Demodulator*    demodulator,
    OUT Bool*           locked
);


/**
 * Get channel modulation related information.
 *
 * @param demodulator the handle of demodulator.
 * @param channelModulation The modulation of channel.
 * @return Error_NO_ERROR: successful, other non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_getChannelModulation (
    IN  Demodulator*            demodulator,
    OUT ChannelModulation*      channelModulation
);


/**
 * Specify the bandwidth of channel and tune the channel to the specific
 * frequency. 
 * In DVB-T mode, after calling this function the output parameter dvbH
 * should return False and host could use output parameter "locked" to check
 * if the channel has correct TS output.
 *
 * @param demodulator the handle of demodulator.
 * @param bandwidth The channel bandwidth.
 *        DVB-T: 5000, 6000, 7000, and 8000 (KHz).
 * @param frequency the channel frequency in KHz.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Bool locked;
 *     DefaultDemodulator demod;
 *
 *     error = Demodulator_acquireChannel ((Demodulator*) &demod, 8000, 666000);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *
 *     error = Demodulator_isLocked ((Demodulator*) &demod, &locked);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *
 *     if (locked == True) {
 *         // In DVB-T mode.
 *         // Start to process TS
 *     }
 * </pre>
 */
Dword Demodulator_acquireChannel_it9133 (
    IN  Demodulator*    demodulator,
    IN  Word            bandwidth,
    IN  Dword           frequency
);


/**
 * Get the statistic values of demodulator, it includes Pre-Viterbi BER,
 * Post-Viterbi BER, Abort Count, Signal Presented Flag, Signal Locked Flag,
 * Signal Quality, Signal Strength.
 *
 * @param demodulator the handle of demodulator.
 * @param statistic the structure that store all statistic values.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     Statistic statistic;
 *     DefaultDemodulator demod;
 *
 *     // Set statistic range.
 *     error = Demodulator_getStatistic ((Demodulator*) &demod, &statistic);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 *     if (statistic.signalPresented == True)
 *         printf ("Signal Presented = True\n");
 *     else
 *         printf ("Signal Presented = False\n");
 *     if (statistic.signalLocked == True)
 *         printf ("Signal Locked = True\n");
 *     else
 *         printf ("Signal Locked = False\n");
 *     printf ("Signal Quality = %d\n", statistic.signalQuality);
 *     printf ("Signal Strength = %d\n", statistic.signalStrength);
 * </pre>
 */
Dword Demodulator_getStatistic (
    IN  Demodulator*    demodulator,
    OUT Statistic*      statistic
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @param contorl 1: Power up, 0: Power down;
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @
 * @Note:
 * @ The function should be called after calling Demodulator_enableControlPowerSaving
 * @ and the power saving function would be successful.
 * @
 * @example <pre>
 * </pre>
 */
Dword Demodulator_controlPowerSaving (
    IN  Demodulator*    demodulator,
    IN  Byte            control
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @param enable 1: enable GPIO Power Control, 0: disable GPIO Power Control;
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_enableControlPowerSaving (
    IN  Demodulator*    demodulator,
	IN  Byte            enable
);


/**
 * Control PID fileter
 *
 * @param demodulator the handle of demodulator.
 * @param contorl 0: Disable, 1: Enable.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Demodulator_controlPidFilter (
    IN  Demodulator*    demodulator,
    IN  Byte            control
);


/**
 * Reset PID filter.
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *
 *     error = Demodulator_resetPidFilter ((Demodulator*) &demod);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else
 *         printf ("Success");
 * </pre>
 */
Dword Demodulator_resetPidFilter (
    IN  Demodulator*    demodulator
);


/**
 * Add PID to PID filter.
 *
 * @param demodulator the handle of demodulator.
 * @param index the index of PID filter.
 * @param pid the PID that will be add to PID filter.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *     Word pid = 0x00;
 *
 *     // Add PID "0x00" to PID filter.
 *     error = Demodulator_addPidToFilter ((Demodulator*) &demod, 1, pid);
 *     if (error)
 *         printf ("Error Code = %X", error);
 *     else{
 *         //Pid counter should add 1 when adding a new pid successfully.
 *         ganymede->pidCounter++;
 *         printf ("Success");
 *	   }
 * </pre>
 */
Dword Demodulator_addPidToFilter (
    IN  Demodulator*    demodulator,
    IN  Byte            index,
    IN  Word            value
);

/**
 * get SNR data .
 *
 * @param demodulator the handle of demodulator.
 * @param SNR data(hex).
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 *
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *     
 *	error = Demodulator_getSNR (demod, &snr);
 *		if (error) {
 *			printf("Demodulator_getSNR  failed!Error = 0x%08x\n", error);
 *			break;
 *		} else {
 *			printf("Signal SNR = %d(DBm)\n", snr);
 *		}   
 *	   }
 * </pre>
 */
Dword Demodulator_getSNR_it9133 (
    IN  Demodulator*    demodulator,
    OUT Byte*           snr
) ;


/**
 * Set setMultiplier .
 *
 * @param demodulator the handle of demodulator.
 * @param  multiplier.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 *
 * @example <pre>
 *     Dword error = Error_NO_ERROR;
 *     DefaultDemodulator demod;
 *     
 *   start = GetTickCount();
 *   error = OMEGA_supportLNA(demod, OMEGA_LNA_Config_2); // 0: OMEGA ; 1: OMEGA_LNA_Config_1 ; 1: OMEGA_LNA_Config_2
 *   if (error){
 *		printf("Initialize LNA type failed.0x%08x\n", error);
 *		return;
 *	}
 *   error = Demodulator_initialize (demod, streamType);
 *   end = GetTickCount();
 *	if (error){
 *		printf("Initialize failed.0x%08x\n", error);
 *		return;
 *	}
 *   printf("Initialize Time: %.02fsec\n", ((double)(end-start))/1000);
 *   error = Demodulator_setMultiplier(demod,Multiplier_2X); //ste ADCx2
 *   printf("Initialize setMultiplier(demod,Multiplier_2X).\n");
 *   printf("Initialize successful.\n");
 * </pre>
 */

Dword Demodulator_setMultiplier (
    IN  Demodulator*    demodulator,
    IN  Multiplier      multiplier
)  ;
#endif
