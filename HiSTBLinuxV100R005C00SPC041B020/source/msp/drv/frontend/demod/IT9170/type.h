/********************************************************************************************/
/**                                                                                         */
/**       These definiations in this file are used by API.                                  */
/**       Please not modify the content.                                                    */
/**                                                                                         */
/********************************************************************************************/
#ifndef __TYPE_H__
#define __TYPE_H__


#define IN
#define OUT
#define INOUT


/**
 * The type of handle.
 */
typedef void* Handle;


/**
 * The type defination of 8-bits unsigned type.
 */
typedef unsigned char Byte;


/**
 * The type defination of 16-bits unsigned type.
 */
typedef unsigned short Word;


/**
 * The type defination of 32-bits unsigned type.
 */
typedef unsigned long Dword;


/**
 * The type defination of 16-bits signed type.
 */
typedef short Short;


/**
 * The type defination of 32-bits signed type.
 */
typedef long Long;



/**
 * The type defination of Bool
 */
typedef enum {
    False = 0,
    True = 1
} Bool;


/**
 * The type defination of Segment
 */
typedef struct {
    Byte segmentType;           /** 0:Firmware download 1:Rom copy 2:Direct command */
    Dword segmentLength;
} Segment;


/**
 * The type defination of Bandwidth.
 */
typedef enum {
    Bandwidth_6M = 0,           /** Signal bandwidth is 6MHz */
    Bandwidth_7M,               /** Signal bandwidth is 7MHz */
    Bandwidth_8M,               /** Signal bandwidth is 8MHz */
    Bandwidth_5M                /** Signal bandwidth is 5MHz */
} Bandwidth;


/**
 * The type defination of TransmissionMode.
 */
typedef enum {
    TransmissionMode_2K = 0,    /** OFDM frame consists of 2048 different carriers (2K FFT mode) */
    TransmissionMode_8K = 1,    /** OFDM frame consists of 8192 different carriers (8K FFT mode) */
    TransmissionMode_4K = 2     /** OFDM frame consists of 4096 different carriers (4K FFT mode) */
} TransmissionModes;


/**
 * The type defination of Constellation.
 */

typedef enum {
	Constellation_DQPSK = 0,	/** Signal uses DQPSK constellation, for ISDB-T  */
	Constellation_QPSK,			/** Signal uses QPSK constellation  */
    Constellation_16QAM,        /** Signal uses 16QAM constellation */
    Constellation_64QAM         /** Signal uses 64QAM constellation */
} Constellation;

/**
 * The type defination of Interval.
 */
typedef enum {
    Interval_1_OVER_32 = 0,     /** Guard interval is 1/32 of symbol length */
    Interval_1_OVER_16,         /** Guard interval is 1/16 of symbol length */
    Interval_1_OVER_8,          /** Guard interval is 1/8 of symbol length  */
    Interval_1_OVER_4           /** Guard interval is 1/4 of symbol length  */
} Interval;


/**
 * The type defination of Priority.
 */
typedef enum {
    Priority_HIGH = 0,          /** DVB-T - identifies high-priority stream */
    Priority_LOW                /** DVB-T - identifies low-priority stream  */
} Priority;


/**
 * The type defination of CodeRate.
 */
typedef enum {
    CodeRate_1_OVER_2 = 0,      /** Signal uses FEC coding ratio of 1/2 */
    CodeRate_2_OVER_3,          /** Signal uses FEC coding ratio of 2/3 */
    CodeRate_3_OVER_4,          /** Signal uses FEC coding ratio of 3/4 */
    CodeRate_5_OVER_6,          /** Signal uses FEC coding ratio of 5/6 */
    CodeRate_7_OVER_8,          /** Signal uses FEC coding ratio of 7/8 */
    CodeRate_NONE               /** None, NXT doesn't have this one     */
} CodeRate;


/**
 * TPS Hierarchy and Alpha value.
 */
typedef enum {
    Hierarchy_NONE = 0,         /** Signal is non-hierarchical        */
    Hierarchy_ALPHA_1,          /** Signalling format uses alpha of 1 */
    Hierarchy_ALPHA_2,          /** Signalling format uses alpha of 2 */
    Hierarchy_ALPHA_4           /** Signalling format uses alpha of 4 */
} Hierarchy;


/**
 * The defination of ChannelInformation.
 */
typedef struct {
    Dword frequency;                    /** Channel frequency in KHz.                                */
    TransmissionModes transmissionMode; /** Number of carriers used for OFDM signal                  */
    Constellation constellation;        /** Constellation scheme (FFT mode) in use                   */
    Interval interval;                  /** Fraction of symbol length used as guard (Guard Interval) */
    Priority priority;                  /** The priority of stream                                   */
    CodeRate highCodeRate;              /** FEC coding ratio of high-priority stream                 */
    CodeRate lowCodeRate;               /** FEC coding ratio of low-priority stream                  */
    Hierarchy hierarchy;                /** Hierarchy levels of OFDM signal                          */
    Bandwidth bandwidth;
} ChannelModulation;


/**
 * The type defination of ValueSet.
 */
typedef struct {
    Dword address;      /** The address of target register */
    Byte value;         /** The value of target register   */
} ValueSet;


/**
 * The type defination of StreamType.
 */
typedef enum {
    StreamType_NONE = 0,            /** Invalid (Null) StreamType                */
    StreamType_DVBT_DATAGRAM = 3,   /** DVB-T mode, store data in device buffer  */
    StreamType_DVBT_PARALLEL,       /** DVB-T mode, output via paralle interface */
    StreamType_DVBT_SERIAL,         /** DVB-T mode, output via serial interface  */
} StreamType;


/**
 * The type defination of ClockTable.
 */
typedef struct {
    Dword crystalFrequency;     /** The frequency of crystal. */
    Dword adcFrequency;         /** The frequency of ADC.     */
} ClockTable;


/**
 * The type defination of BandTable.
 */
typedef struct {
    Dword minimum;          /** The minimum frequency of this band */
    Dword maximum;          /** The maximum frequency of this band */
} BandTable;


/**
 * The type defination of Processor.
 */
typedef enum {
    Processor_LINK = 0,
    Processor_OFDM = 8
} Processor;


/**
 * The type defination of Demodulator.
 */
typedef struct {
    Handle userData;
} Demodulator;


#include "user.h"


/**
 * The type defination of Statistic.
 */
typedef struct {
    Bool signalPresented;       /** Signal is presented.                                                                         */
    Bool signalLocked;          /** Signal is locked.                                                                            */
    Byte signalQuality;         /** Signal quality, from 0 (poor) to 100 (good).                                                 */
    Byte signalStrength;        /** Signal strength from 0 (weak) to 100 (strong).                                               */
} Statistic;


/**
 * General demodulator register-write function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be written.
 * @param bufferLength number, 1-8, of registers to be written.
 * @param buffer buffer used to store values to be written to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*WriteRegisters) (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            registerAddressLength,
    IN  Dword           writeBufferLength,
    IN  Byte*           writeBuffer
);


/**
 * General write EEPROM function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be read.
 * @param bufferLength number, 1-8, of registers to be written.
 * @param buffer buffer used to store values to be written to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*WriteEepromValues) (
    IN  Demodulator*    demodulator,
    IN  Byte            eepromAddress,
    IN  Word            registerAddress,
    IN  Byte            registerAddressLength,
    IN  Byte            writeBufferLength,
    IN  Byte*           writeBuffer
);


/**
 * General demodulator register-read function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be read.
 * @param bufferLength number, 1-8, of registers to be read.
 * @param buffer buffer used to store values to be read to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*ReadRegisters) (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            registerAddressLength,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
);


/**
 * General read EEPROM function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be read.
 * @param bufferLength number, 1-8, of registers to be read.
 * @param buffer buffer used to store values to be read to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*ReadEepromValues) (
    IN  Demodulator*    demodulator,
    IN  Byte            eepromAddress,
    IN  Word            registerAddress,
    IN  Byte            registerAddressLength,
    IN  Byte            readBufferLength,
    OUT Byte*           readBuffer
);


/**
 * General demodulator register-read function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be read.
 * @param bufferLength number, 1-8, of registers to be read.
 * @param buffer buffer used to store values to be read to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*ModifyRegister) (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            registerAddressLength,
    IN  Byte            position,
    IN  Byte            length,
    IN  Byte            value
);


/**
 * General load firmware function
 *
 * @param demodulator the handle of demodulator.
 * @param length The length of firmware.
 * @param firmware The byte array of firmware.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*LoadFirmware) (
    IN  Demodulator*    demodulator,
    IN  Dword           firmwareLength,
    IN  Byte*           firmware
);


/**
 * General reboot function
 *
 * @param demodulator the handle of demodulator.
 * @param length The length of firmware.
 * @param firmware The byte array of firmware.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*Reboot) (
    IN  Demodulator*    demodulator
);


/**
 * Find and Get bus handle used to control bus
 *
 * @param demodulator the handle of demodulator.
 * @param handle The bus handle.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*GetBus) (
    IN  Demodulator*    demodulator,
    OUT Handle*         handle
);


/**
 * Find and Get bus handle used to control bus
 *
 * @param demodulator the handle of demodulator.
 * @param bufferLength The length to transmit.
 * @param buffer The buffer which we store the data to send.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*BusTx) (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    IN  Byte*           buffer
);


/**
 * Find and Get bus handle used to control bus
 *
 * @param demodulator the handle of demodulator.
 * @param bufferLength The length to transmit.
 * @param buffer The buffer which we store the data to send.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*BusRx) (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    IN  Byte*           buffer
);


/**
 * Find and Get bus handle used to control bus
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress The starting address of memory to get.
 * @param readBufferLength The length of buffer to receive data.
 * @param readBuffer The buffer use to store received data
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*BusRxData) (
    IN  Demodulator*    demodulator,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
);

/**
 * General send command function
 *
 * @param demodulator the handle of demodulator.
 * @param command The command which you wan.
 * @param valueLength value length.
 * @param valueBuffer value buffer.
 * @param referenceLength reference length.
 * @param referenceBuffer reference buffer.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*SendCommand) (
    IN  Demodulator*    demodulator,
    IN  Word            command,
    IN  Processor       processor,
    IN  Dword           writeBufferLength,
    IN  Byte*           writeBuffer,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
);


/**
 * General read EEPROM function
 *
 * @param demodulator the handle of demodulator.
 * @param registerAddress address of register to be read.
 * @param bufferLength number, 1-8, of registers to be read.
 * @param buffer buffer used to store values to be read to specified
 *        registers.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*ReceiveData) (
    IN  Demodulator*    demodulator,
    IN  Dword           registerAddress,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
);


/**
 * General tuner opening function
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*OpenTuner) (
    IN  Demodulator*    demodulator
);


/**
 * General tuner closing function
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*CloseTuner) (
    IN  Demodulator*    demodulator
);


/**
 * General tuner setting function
 *
 * @param demodulator the handle of demodulator.
 * @return Error_NO_ERROR: successful, non-zero error code otherwise.
 */
typedef Dword (*SetTuner) (
    IN  Demodulator*    demodulator,
    IN  Word            bandwidth,
    IN  Dword           frequency
);


/**
 * The type defination of TunerDescription
 */
typedef struct {
    OpenTuner       openTunerFunc;
    CloseTuner      closeTunerFunc;
    SetTuner        setTunerFunc;
    ValueSet*       tunerScriptTable;
    Word*           tunerScriptSetsTable;
    Byte            tunerAddress;
    Byte            registerAddressLength;
    Dword           ifFrequency;
    Bool            inversion;
    Word            tunerId;
} TunerDescription;


/**
 * The data structure of Ganymede
 */
typedef struct {
    /** Basic structure */
    Handle userData;
    Byte* firmwareCodes;
    Segment* firmwareSegments;
    Byte* firmwarePartitions;
    Word* scriptSets;
    ValueSet* scripts;
    Dword crystalFrequency;
    Dword adcFrequency;
    StreamType streamType;
    Word bandwidth;
    Dword frequency;
    Dword fcw;
    Statistic statistic;
    Byte hostInterface;
    Bool booted;
    Bool initialized;
    Byte pidCounter;
    Byte demodAddr;       /** Demodulator I2C Address */
    Bool Clkout_en;       /** Clock output enable */
} DefaultDemodulator;

/**
 * The type defination of Multiplier.
 */
typedef enum {
    Multiplier_1X = 0,
    Multiplier_2X
} Multiplier;

extern const Byte Standard_bitMask[8];
#define REG_MASK(pos, len)                (Standard_bitMask[len-1] << pos)
#define REG_CLEAR(temp, pos, len)         (temp & (~REG_MASK(pos, len)))
#define REG_CREATE(val, temp, pos, len)   ((val << pos) | (REG_CLEAR(temp, pos, len)))
#define REG_GET(value, pos, len)          ((value & REG_MASK(pos, len)) >> pos)
#define LOWBYTE(w)      ((Byte)((w) & 0xff))
#define HIGHBYTE(w)     ((Byte)((w >> 8) & 0xff))

#define OMEGA_NORMAL                    0x00
#define OMEGA_LNA_Config_1              0x01
#define OMEGA_LNA_Config_2              0x02
#endif
