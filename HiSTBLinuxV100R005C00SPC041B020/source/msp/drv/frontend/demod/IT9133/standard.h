/********************************************************************************************/
/**                                                                                         */
/**       These functions in this file are used by API.                                     */
/**       Please not modify the content.                                                    */
/**                                                                                         */
/********************************************************************************************/
#ifndef __STANDARD_H__
#define __STANDARD_H__

#ifndef __OMAGADEMOD_H__
  #error "standard.h can't be included driectly. Please use function in demodulator.h"
#endif


#include "type.h"
#include "user.h"
#include "error.h"
#include "register.h"
#include "variable.h"
#include "version.h"

/**
 * Send the command to device.
 *
 * @param demodulator the handle of demodulator.
 * @param command the command to be send.
 * @param processor The processor of specified register. Because each chip
 *        has two processor so user have to specify the processor. The
 *        possible values are Processor_LINK and Processor_OFDM.
 * @param writeBufferLength the number of registers to be write.
 * @param writeBuffer a byte array which is used to store values to be write.
 * @param readBufferLength the number of registers to be read.
 * @param readBuffer a byte array which is used to store values to be read.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_sendCommand (
    IN  Demodulator*    demodulator,
    OUT Word            command,
    IN  Processor       processor,
    IN  Dword           writeBufferLength,
    IN  Byte*           writeBuffer,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
);


/**
 * Program the bandwidth related parameters to demodulator.
 *
 * @param demodulator the handle of demodulator.
 * @param bandwidth DVB channel bandwidth in MHz. The possible values
 *        are 5, 6, 7, and 8 (MHz).
 * @param adcFrequency The value of desire internal ADC frequency (Hz).
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_selectBandwidth (
    IN  Demodulator*    demodulator,
    IN  Word            bandwidth,          /** KHz                 */
    IN  Dword           adcFrequency        /** Hz, ex: 20480000    */
);


/**
 * Set frequency.
 *
 * @param demodulator the handle of demodulator.
 * @param frequency The desired frequency.
 * @return Error_NO_ERROR: successful, other non-zero error code otherwise.
 */
Dword Standard_setFrequency (
    IN  Demodulator*    demodulator,
    IN  Dword           frequency
);


/**
 * Load firmware to device
 *
 * @param demodulator the handle of demodulator.
 * @firmwareCodes pointer to fw binary.
 * @firmwareSegments pointer to fw segments.
 * @firmwarePartitions pointer to fw partition.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_loadFirmware (
    IN  Demodulator*    demodulator,
    IN  Byte*           firmwareCodes,
    IN  Segment*        firmwareSegments,
    IN  Byte*           firmwarePartitions
);


/**
 * Load initial script to device
 *
 * @param demodulator the handle of demodulator.
 * @scriptSets pointer to scriptSets.
 * @scripts pointer to fw scripts.
 * @tunerScriptSets pointer to tunerScriptSets.
 * @tunerScripts pointer to tunerScripts.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_loadScript (
    IN  Demodulator*    demodulator,
    IN  Word*           scriptSets,
    IN  ValueSet*       scripts,
    IN  Word*           tunerScriptSets,
    IN  ValueSet*       tunerScripts
);


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
 */
Dword Standard_writeRegister (
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
 */
Dword Standard_writeRegisters (
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
 */
Dword Standard_writeRegisterBits (
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
 */
Dword Standard_readRegister (
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
 */
Dword Standard_readRegisters (
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
 */
Dword Standard_readRegisterBits (
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
 */
Dword Standard_getFirmwareVersion (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    OUT Dword*          version
);


/**
 * Set the counting range for Post-Viterbi and Post-Viterbi.
 *
 * @param demodulator the handle of demodulator.
 * @param postErrorCount the number of super frame for Pre-Viterbi.
 * @param postBitCount the number of packet unit for Post-Viterbi.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getPostVitBer (
    IN  Demodulator*    demodulator,
    OUT Dword*          postErrorCount, /** 24 bits */
    OUT Dword*          postBitCount,   /** 16 bits */
    OUT Word*           abortCount
);


/**
 * Get RF AGC gain.
 *
 * @param demodulator the handle of demodulator.
 * @param rfAgc the value of RF AGC.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getRfAgcGain (
    IN  Demodulator*    demodulator,
    OUT Byte*           rfAgc
);


/**
 * Get IF AGC gain.
 *
 * @param demodulator the handle of demodulator.
 * @param ifAgc the value of IF AGC.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getIfAgcGain (
    IN  Demodulator*    demodulator,
    OUT Byte*           ifAgc
);


/**
 * Get siganl quality.
 *
 * @param demodulator the handle of demodulator.
 * @param quality The value of signal quality.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSignalQuality (
    IN  Demodulator*    demodulator,
    OUT Byte*           quality
);

/**
 * Get siganl quality Indication.
 *
 * @param demodulator the handle of demodulator.
 * @param quality The value of signal quality.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSignalQualityIndication (
    IN  Demodulator*    demodulator,
    OUT Byte*           quality
);



/**
 * Get siganl strength Indication.
 *
 * @param demodulator the handle of demodulator.
 * @param strength The value of signal strength that calculations of ¡§score mapping¡¨ from the signal strength (dBm) to the ¡§0-100¡¨ scoring.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSignalStrengthIndication (
    IN  Demodulator*    demodulator,
    OUT Byte*           strength
);


/**
 * Get siganl strength.
 *
 * @param demodulator the handle of demodulator.
 * @param strength The value of signal strength.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSignalStrength (
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
 */
Dword Standard_getSignalStrengthDbm (
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
 */
Dword Standard_initialize (
    IN  Demodulator*    demodulator,
    IN  StreamType      streamType
);


/**
 * Power off the demodulators.
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_finalize (
    IN  Demodulator*    demodulator
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Standard_isTpsLocked (
    IN  Demodulator*    demodulator,
    OUT Bool*           locked
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Standard_isMpeg2Locked (
    IN  Demodulator*    demodulator,
    OUT Bool*           locked
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @param locked the result of frequency tuning. True if there is
 *        demodulator can lock signal, False otherwise.
 * @example <pre>
 * </pre>
 */
Dword Standard_isLocked (
    IN  Demodulator*    demodulator,
    OUT Bool*           locked
);


/**
 * Get channel modulation related information.
 *
 * @param demodulator the handle of demodulator.
 * @param channelModulation The modulation of channel.
 * @return Error_NO_ERROR: successful, other non-zero error code otherwise.
 */
Dword Standard_getChannelModulation (
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
 */
Dword Standard_acquireChannel (
    IN  Demodulator*    demodulator,
    IN  Word            bandwidth,
    IN  Dword           frequency
);


/**
 * Set the output stream type of chip. Because the device could output in
 * many stream type, therefore host have to choose one type before receive
 * data.
 *
 * Note: Please refer to the example of Standard_acquireChannel when host want
 *       to detect the available channels.
 * Note: After host know all the available channels, and want to change to
 *       specific channel, host have to choose output mode before receive
 *       data. Please refer the example of Standard_setStreamType.
 *
 * @param demodulator the handle of demodulator.
 * @param streamType the possible values are
 *        DVB-T:    StreamType_DVBT_DATAGRAM
 *                  StreamType_DVBT_PARALLEL
 *                  StreamType_DVBT_SERIAL
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_setStreamType (
    IN  Demodulator*    demodulator,
    IN  StreamType      streamType
);


/**
 * Set the architecture of chip. When two of our device are using. Therefore,
 * host could decide which mode to be operated.
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_setArchitecture (
    IN  Demodulator*    demodulator
);


/**
 * Set the counting range for Pre-Viterbi and Post-Viterbi.
 *
 * @param demodulator the handle of demodulator.
 * @param frameCount the number of super frame for Pre-Viterbi.
 * @param packetUnit the number of packet unit for Post-Viterbi.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_setViterbiRange (
    IN  Demodulator*    demodulator,
    IN  Byte            superFrameCount,
    IN  Word            packetUnit
);


/**
 * Get the counting range for Pre-Viterbi and Post-Viterbi.
 *
 * @param demodulator the handle of demodulator.
 * @param frameCount the number of super frame for Pre-Viterbi.
 * @param packetUnit the number of packet unit for Post-Viterbi.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getViterbiRange (
    IN  Demodulator*    demodulator,
    IN  Byte*           frameCount,
    IN  Word*           packetUnit
);


/**
 * Get the statistic values of demodulator, it includes Pre-Viterbi BER,
 * Post-Viterbi BER, Abort Count, Signal Presented Flag, Signal Locked Flag,
 * Signal Quality, Signal Strength.
 *
 * @param demodulator the handle of demodulator.
 * @param statistic the structure that store all statistic values.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getStatistic (
    IN  Demodulator*    demodulator,
    OUT Statistic*      statistic
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @param enable 1: enable GPIO Power Control, 0: disable GPIO Power Control;
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Standard_enableControlPowerSaving (
    IN  Demodulator*    demodulator,
    IN  Byte            enable
);


/**
 *
 * @param demodulator the handle of demodulator.
 * @param contorl 1: Power up, 0: Power down;
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 * @example <pre>
 * </pre>
 */
Dword Standard_controlPowerSaving (
    IN  Demodulator*    demodulator,
    IN  Byte            control
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
Dword Standard_controlPidFilter (
    IN  Demodulator*    demodulator,
    IN  Byte            control
);


/**
 * Reset PID filter.
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_resetPidFilter (
    IN  Demodulator*    demodulator
);


/**
 * Add PID to PID filter.
 *
 * @param demodulator the handle of demodulator.
 * @param pid the PID that will be add to PID filter.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_addPidToFilter (
    IN  Demodulator*    demodulator,
    IN  Byte            index,
    IN  Word            value
);


/**
 * get SNR .
 *
 * @param demodulator the handle of demodulator.
 * @param SNR (db).
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSNR (
    IN  Demodulator*    demodulator,
    OUT Byte*           snr
); 

/**
 * get SNR data .
 *
 * @param demodulator the handle of demodulator.
 * @param SNR data(hex).
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_getSNRValue(
      IN  Demodulator*  demodulator,
      OUT Dword*        snr_value
);

/**
 * Set setMultiplier .
 *
 * @param demodulator the handle of demodulator.
 * @param  multiplier.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
Dword Standard_setMultiplier (
    IN  Demodulator*    demodulator,
    IN  Multiplier      multiplier
);

#endif
