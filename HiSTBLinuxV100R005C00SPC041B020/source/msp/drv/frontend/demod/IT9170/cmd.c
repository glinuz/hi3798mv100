#include "cmd.h"
#include "variable.h"

Byte Cmd_sequence = 0;


Dword Cmd_addChecksum (
    IN  Demodulator*    demodulator,
    OUT Dword*          bufferLength,
    OUT Byte*           buffer
) {
    Dword error  = Error_NO_ERROR;
    Dword loop   = (*bufferLength - 1) / 2;
    Dword remain = (*bufferLength - 1) % 2;
    Dword i;
    Word  checksum = 0;

    for (i = 0; i < loop; i++)
        checksum += (Word) (buffer[2 * i + 1] << 8) + (Word) (buffer[2 * i + 2]);
    if (remain)
        checksum += (Word) (buffer[*bufferLength - 1] << 8);
    
    checksum = ~checksum;
    buffer[*bufferLength]     = (Byte) ((checksum & 0xFF00) >> 8);
    buffer[*bufferLength + 1] = (Byte) (checksum & 0x00FF);
    buffer[0]                 = (Byte) (*bufferLength + 1);
    *bufferLength            += 2;

    return (error);
}


Dword Cmd_removeChecksum (
    IN  Demodulator*    demodulator,
    OUT Dword*          bufferLength,
    OUT Byte*           buffer
) {
    Dword error    = Error_NO_ERROR;
    Dword loop     = (*bufferLength - 3) / 2;
    Dword remain   = (*bufferLength - 3) % 2;
    Dword i;
    Word  checksum = 0;

    for (i = 0; i < loop; i++)
        checksum += (Word) (buffer[2 * i + 1] << 8) + (Word) (buffer[2 * i + 2]);
    if (remain)
        checksum += (Word) (buffer[*bufferLength - 3] << 8);    
    
    checksum = ~checksum;
    if (((Word)(buffer[*bufferLength - 2] << 8) + (Word)(buffer[*bufferLength - 1])) != checksum) {
        error = Error_WRONG_CHECKSUM;
        goto exit;
    }
    if (buffer[2])
        error = Error_FIRMWARE_STATUS | buffer[2];
    
    buffer[0]      = (Byte) (*bufferLength - 3);
    *bufferLength -= 2;

exit :
    return (error);
}


Dword Cmd_busTx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    OUT Byte*           buffer
) {
    Dword     error = Error_NO_ERROR;
    Byte      i;

    for (i = 0; i < User_RETRY_MAX_LIMIT; i++) {
        error = User_busTx (demodulator, bufferLength, buffer);
        if (error == 0) goto exit;

        User_delay (demodulator, 1);
    }

exit:
    return (error);
}


Dword Cmd_busRx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    OUT Byte*           buffer
) {
    Dword     error = Error_NO_ERROR;
    Byte      i;
    User_delay (demodulator, 5);
    for (i = 0; i < User_RETRY_MAX_LIMIT; i++) {
        error = User_busRx (demodulator, bufferLength, buffer);
        if (error == 0) goto exit;

        User_delay (demodulator, /*1*/10);
    }

exit:
    return (error);
}


Dword Cmd_writeRegisters (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            registerAddressLength,
    IN  Dword           writeBufferLength,
    IN  Byte*           writeBuffer
) {
    Dword       error = Error_NO_ERROR;
    Word        command;
    Byte        buffer[255];
    Dword       bufferLength;
    Dword       remainLength;
    Dword       sendLength;
    Dword       i;
    DefaultDemodulator*   demod;
    Dword       maxFrameSize = 255;

    User_enterCriticalSection (demodulator);

    if (writeBufferLength == 0) goto exit;
    if (registerAddressLength > 4) {
        error  = Error_PROTOCOL_FORMAT_INVALID;
        goto exit;
    }

    demod     = (DefaultDemodulator*) demodulator;

    if ((writeBufferLength + 12) > maxFrameSize) {
        error = Error_INVALID_DATA_LENGTH;
        goto exit;
    }


    /** short command */
    if (User_USE_SHORT_CMD && demod->booted) {
        for (i = 0; i < writeBufferLength; i++) {
            command   = Command_SHORT_REG_DEMOD_WRITE + (Byte) (processor << 4);
            buffer[0] = (Byte) 4;
            buffer[1] = (Byte) command;
            buffer[2] = (Byte) ((registerAddress + i) >> 8);
            buffer[3] = (Byte) (registerAddress + i);
            buffer[4] = writeBuffer[i];

            bufferLength = 5;
            error = Cmd_busTx (demodulator, bufferLength, buffer);
            if (error) goto exit;
        }
        goto exit;
    }

    /** add frame header */
    command   = Cmd_buildCommand (Command_REG_DEMOD_WRITE, processor);
    buffer[1] = (Byte) (command >> 8);
    buffer[2] = (Byte) command;
    buffer[3] = (Byte) Cmd_sequence++;
    buffer[4] = (Byte) writeBufferLength;
    buffer[5] = (Byte) registerAddressLength;
    buffer[6] = (Byte) ((registerAddress) >> 24); /** Get first byte of reg. address  */
    buffer[7] = (Byte) ((registerAddress) >> 16); /** Get second byte of reg. address */
    buffer[8] = (Byte) ((registerAddress) >> 8);  /** Get third byte of reg. address  */
    buffer[9] = (Byte) (registerAddress );        /** Get fourth byte of reg. address */

    /** add frame data */
    for (i = 0; i < writeBufferLength; i++) {    
        buffer[10 + i] = writeBuffer[i];
    }

    /** add frame check-sum */
    bufferLength = 10 + writeBufferLength;
    error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;    

    /** send frame */
    i = 0;
    sendLength = 0;
    remainLength = bufferLength;
    while (remainLength > 0) {
        i     = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);
        //added by zhangtao
        if (Processor_OFDM == processor && registerAddress == trigger_ofsm)
            User_delay (demodulator, /*1*/100);
        error = Cmd_busTx (demodulator, i, &buffer[sendLength]);
        if (error) goto exit;

        //added by zhangtao
        if (Processor_OFDM == processor && registerAddress == trigger_ofsm)
            User_delay (demodulator, /*1*/100);
        sendLength   += i;
        remainLength -= i;
    }

    /** get reply frame */
    bufferLength = 5;
    error = Cmd_busRx (demodulator, bufferLength, buffer);
    if (error) goto exit;

    /** remove check-sum from reply frame */
    error = Cmd_removeChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;

exit :
    User_leaveCriticalSection (demodulator);
    return (error);
}


Dword Cmd_readRegisters (
    IN  Demodulator*    demodulator,
    IN  Processor       processor,
    IN  Dword           registerAddress,
    IN  Byte            registerAddressLength,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
) {
    Dword       error = Error_NO_ERROR;
    Word        command;
    Byte        buffer[255];
    Dword       bufferLength;
    Dword       sendLength;
    Dword       remainLength;
    Dword       i, k;
    DefaultDemodulator*   demod;
    Dword       maxFrameSize = 255;

    User_enterCriticalSection (demodulator);

    if (readBufferLength == 0) goto exit;
    if (registerAddressLength > 4) {
        error  = Error_PROTOCOL_FORMAT_INVALID;
        goto exit;
    }

    demod     = (DefaultDemodulator*) demodulator;

    if ((readBufferLength + 5) > User_MAX_PKT_SIZE) {
        error = Error_INVALID_DATA_LENGTH;
        goto exit;
    }

    if ((readBufferLength + 5) > maxFrameSize) {
        error = Error_INVALID_DATA_LENGTH;
        goto exit;
    }


    /** short command */
    if (User_USE_SHORT_CMD && demod->booted) {
        for (i = 0; i < readBufferLength; i++) {
            command   = Command_SHORT_REG_DEMOD_READ + (Byte) (processor << 4);
            buffer[0] = (Byte) 3;
            buffer[1] = (Byte) command;
            buffer[2] = (Byte) ((registerAddress + i) >> 8);
            buffer[3] = (Byte) (registerAddress + i);

            bufferLength = 4;
            error = Cmd_busTx (demodulator, bufferLength, buffer);
            if (error) goto exit;

            bufferLength = 2;
            error = Cmd_busRx (demodulator, bufferLength, buffer);
            if (error) goto exit;

            readBuffer[i] = buffer[1];
        }
        goto exit;
    }

    /** add frame header */
    command   = Cmd_buildCommand (Command_REG_DEMOD_READ, processor);
    buffer[1] = (Byte) (command >> 8);
    buffer[2] = (Byte) command;
    buffer[3] = (Byte) Cmd_sequence++;
    buffer[4] = (Byte) readBufferLength;
    buffer[5] = (Byte) registerAddressLength;
    buffer[6] = (Byte) (registerAddress >> 24); /** Get first byte of reg. address  */
    buffer[7] = (Byte) (registerAddress >> 16); /** Get second byte of reg. address */
    buffer[8] = (Byte) (registerAddress >> 8);  /** Get third byte of reg. address  */
    buffer[9] = (Byte) (registerAddress);       /** Get fourth byte of reg. address */

    /** add frame check-sum */
    bufferLength = 10;
    error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;


    /** send frame */
    i = 0;
    sendLength   = 0;
    remainLength = bufferLength;
    while (remainLength > 0) {
        i     = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);        
        error = Cmd_busTx (demodulator, i, &buffer[sendLength]);
        if (error) goto exit;

        sendLength   += i;
        remainLength -= i;
    }

    /** get reply frame */
    bufferLength = 5 + readBufferLength;

    //added by zhangtao
    User_delay (demodulator, /*1*/5);

    error = Cmd_busRx (demodulator, bufferLength, buffer);
    if (error) goto exit;

    /** remove check-sum from reply frame */
    error = Cmd_removeChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;

    for (k = 0; k < readBufferLength; k++) {
        readBuffer[k] = buffer[k + 3];
    }

exit :
    User_leaveCriticalSection (demodulator);
    return (error);
}


Dword Cmd_loadFirmware (
    IN  Demodulator*    demodulator,
    IN  Dword           length,
    IN  Byte*           firmware
) {
    Dword       error = Error_NO_ERROR;
    Word        command;
    Dword       loop;
    Dword       remain;
    Dword       i, j, k;
    Byte        buffer[255];
    Dword       payloadLength;
    Dword       bufferLength;
    Dword       remainLength;
    Dword       sendLength;
    Dword       maxFrameSize = 255;

    User_enterCriticalSection (demodulator);

    payloadLength = (maxFrameSize - 6);
    loop   = length / payloadLength;
    remain = length % payloadLength;

    k = 0;
    command = Cmd_buildCommand (Command_FW_DOWNLOAD, Processor_LINK);
    for (i = 0; i < loop; i++) {
        buffer[1] = (Byte) (command >> 8);
        buffer[2] = (Byte) command;
        buffer[3] = (Byte) Cmd_sequence++;

        for (j = 0; j < payloadLength; j++)
            buffer[4 + j] = firmware[j + i*payloadLength];

        bufferLength = 4 + payloadLength;
        error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
        if (error) goto exit;

        sendLength = 0;
        remainLength = maxFrameSize;
        while (remainLength > 0) {
            k     = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);        
            error = Cmd_busTx (demodulator, k, &buffer[sendLength]);
            if (error) goto exit;

            sendLength   += k;
            remainLength -= k;
        }
    }

    if (remain) {
        buffer[1] = (Byte) (command >> 8);
        buffer[2] = (Byte) command;
        buffer[3] = (Byte) Cmd_sequence++;

        for (j = 0; j < remain; j++)
            buffer[4 + j] = firmware[j + i*payloadLength];

        bufferLength = 4 + remain;
        error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
        if (error) goto exit;

        sendLength   = 0;
        remainLength = bufferLength;
        while (remainLength > 0)
        {
            k = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);        
            error = Cmd_busTx (demodulator, k, &buffer[sendLength]);
            if (error) goto exit;

            sendLength   += k;
            remainLength -= k;
        }
    }

exit :
    User_leaveCriticalSection (demodulator);
    return (error);
}


Dword Cmd_reboot (
    IN  Demodulator*    demodulator
) {
    Dword       error = Error_NO_ERROR;
    Word        command;
    Byte        buffer[255];
    Dword       bufferLength;
    

    User_enterCriticalSection (demodulator);

    command   = Cmd_buildCommand (Command_REBOOT, Processor_LINK);
    buffer[1] = (Byte) (command >> 8);
    buffer[2] = (Byte) command;
    buffer[3] = (Byte) Cmd_sequence++;
    bufferLength = 4;
    error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;

    error = Cmd_busTx (demodulator, bufferLength, buffer);
    if (error) goto exit;

exit :
    User_leaveCriticalSection (demodulator);
    return (error);
}


Dword Cmd_sendCommand (
    IN  Demodulator*    demodulator,
    IN  Word            command,
    IN  Processor       processor,
    IN  Dword           writeBufferLength,
    IN  Byte*           writeBuffer,
    IN  Dword           readBufferLength,
    OUT Byte*           readBuffer
) {
    Dword       error = Error_NO_ERROR;
    Byte        buffer[255];
    Dword       bufferLength;
    Dword       remainLength;
    Dword       sendLength;
    Dword       i, k;
    Dword       maxFrameSize = 255;

    User_enterCriticalSection (demodulator);

    if ((writeBufferLength + 6) > maxFrameSize) {
        error = Error_INVALID_DATA_LENGTH;
        goto exit;
    }

    if ((readBufferLength + 5) > User_MAX_PKT_SIZE) {
        error  = Error_INVALID_DATA_LENGTH;
        goto exit;
    }

    if ((readBufferLength + 5) > maxFrameSize) {
        error = Error_INVALID_DATA_LENGTH;
        goto exit;
    }


    if (writeBufferLength == 0) {
        command   = Cmd_buildCommand (command, processor);
        buffer[1] = (Byte) (command >> 8);
        buffer[2] = (Byte) command;
        buffer[3] = (Byte) Cmd_sequence++;
        bufferLength = 4;
        error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
        if (error) goto exit;

        // send command packet
        i = 0;
        sendLength = 0;
        remainLength = bufferLength;
        while (remainLength > 0) {
            i = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);        
            error = Cmd_busTx (demodulator, i, &buffer[sendLength]);
            if (error) goto exit;

            sendLength   += i;
            remainLength -= i;
        }
    } else {
        command   = Cmd_buildCommand (command, processor);
        buffer[1] = (Byte) (command >> 8);
        buffer[2] = (Byte) command;
        buffer[3] = (Byte) Cmd_sequence++;
        for (k = 0; k < writeBufferLength; k++)
            buffer[k + 4] = writeBuffer[k];
        
        
        bufferLength = 4 + writeBufferLength;
        error = Cmd_addChecksum (demodulator, &bufferLength, buffer);
        if (error) goto exit;

        
        /** send command */
        i = 0;
        sendLength = 0;
        remainLength = bufferLength;
        while (remainLength > 0) {
            i     = (remainLength > User_MAX_PKT_SIZE) ? (User_MAX_PKT_SIZE) : (remainLength);        
            error = Cmd_busTx (demodulator, i, &buffer[sendLength]);
            if (error) goto exit;

            sendLength   += i;
            remainLength -= i;
        }
    }

    bufferLength = 5 + readBufferLength;

    error = Cmd_busRx (demodulator, bufferLength, buffer);
    if (error) goto exit;

    error = Cmd_removeChecksum (demodulator, &bufferLength, buffer);
    if (error) goto exit;

    if (readBufferLength) {
        for (k = 0; k < readBufferLength; k++) {
            readBuffer[k] = buffer[k + 3];
        }
    }

exit :
    User_leaveCriticalSection (demodulator);
    return (error);
}

