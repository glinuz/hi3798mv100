/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-03-28 01:40:32 #$
  File Revision : $Revision:: 4945 $
------------------------------------------------------------------------------*/
/**
 @file    sony_i2c.h

          This file provides the I2C driver API definition.

*/

#ifndef SONY_I2C_H
#define SONY_I2C_H

#include "sony_common.h"

#define SONY_I2C_START_EN   (0x01)  /**< Output START (S) */
#define SONY_I2C_STOP_EN    (0x02)  /**< Output STOP  (P) */

/**
 @brief The I2C driver API defintion.

        The I2C API also allows for the remote device to be connected 
        through an "I2C gateway". The gateway relays requests/responses
        from/to the host. The gateway is configured by setting 
        ::sony_i2c_t::gwAddress and ::sony_i2c_t::gwSub, then the 
        ::sony_i2c_t::Read and ::sony_i2c_t::Write function pointers 
        would then use this information for read/writing through the gateway.

        <b>I2C Gateway Access</b>
        
        The Read gateway function pointer should:
        \verbatim {S} [Addr] [GW Sub] [TunerAddr+Read] {SR} [Addr+Read] [Read0] [Read1] ... [ReadN] {P} \endverbatim
        
        The Write gateway function pointer should:
        \verbatim {S} [Addr] [GW Sub] [TunerAddr] [Data0] [Data1] ... [DataN] {P} \endverbatim
        Where: \verbatim
        {S} : Start condition
        {P} : Stop condition
        {SR}: Start repeat condition \endverbatim

*/
typedef struct sony_i2c_t {
    /**
     @brief Read bytes from the I2C device.

     @param pI2c I2C driver instance.
     @param deviceAddress The device address.
     @param pData The buffer to store the read data into.
     @param size The number of bytes to read from the I2C device.
     @param mode The I2C mode (whether to output START/STOP conditions).
                 See ::SONY_I2C_START_EN and ::SONY_I2C_STOP_EN.

     @return SONY_RESULT_OK if successful.
    */
    sony_result_t (*Read)(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData, uint32_t size, uint8_t mode);
    /**
     @brief Write bytes to the I2C device.

     @param pI2c I2C driver instance.
     @param deviceAddress The device address.
     @param pData The data to write to the device.
     @param size The number of bytes to write to the I2C device.
     @param mode The I2C mode (whether to output START/STOP conditions).
                 See ::SONY_I2C_START_EN and ::SONY_I2C_STOP_EN.

     @return SONY_RESULT_OK if successful.
    */
    sony_result_t (*Write)(struct sony_i2c_t* pI2c, uint8_t deviceAddress, const uint8_t *pData, uint32_t size, uint8_t mode);
    /**
     @brief Read a sub-addressed register from the device.
            Multiple byte reads are stored into pData in ascending order.
            e.g pData[0] = Value of subAddr
                pData[1] = Value of subAddr+1

            Implement using ::sony_i2c_CommonReadRegister.
        
            I2C output/input:
            [S][Addr][SubAddr][Sr][Addr][Data0]....[DataN][P]

     @param pI2c I2C driver instance.
     @param deviceAddress The device address.
     @param subAddress The sub address.
     @param pData The buffer to store the register contents into.
     @param size The number of bytes to read from the device.

     @return SONY_RESULT_OK if successful.
    */
    sony_result_t (*ReadRegister)(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t* pData, uint32_t size);
    /**
     @brief Write a sub-addressed register of the device.
            Multiple byte writes are stored into the device register
            in ascending order.
            e.g pData[0] = Value to set in subAddr
                pData[1] = Value to set in subAddr+1

            Implement using ::sony_i2c_CommonWriteRegister.

            I2C output:
            [S][Addr][SubAddr][Data0]....[DataN][P]

     @param pI2c I2C driver instance.
     @param deviceAddress The device address.
     @param subAddress The sub address.
     @param pData The buffer to write into register contents.
     @param size The number of bytes to write to the  device.

     @return SONY_RESULT_OK if successful.
    */
    sony_result_t (*WriteRegister)(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, const uint8_t *pData, uint32_t size);
    /**
     @brief Write a single sub-addressed register of the device.

            Implement using ::sony_i2c_CommonWriteOneRegister.

            I2C output:
            [S][Addr][SubAddr][Data0][P]

     @param pI2c I2C driver instance.
     @param deviceAddress The device address.
     @param subAddress The sub address.
     @param data The byte to write into register contents.

     @return SONY_RESULT_OK if successful.
    */
    sony_result_t (*WriteOneRegister)(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data);

    uint8_t          gwAddress; /**< (Only for gateway access) Gateway address (e.g Demod device address). */
    uint8_t          gwSub;     /**< (Only for gateway access) Gateway sub address. */

    uint32_t         flags;     /**< Flags that can be used by i2c code */
    void*            user;      /**< User defined data. */
} sony_i2c_t;

/*------------------------------------------------------------------------------
  I2C help functions
------------------------------------------------------------------------------*/
/**
 @brief Provides a ::sony_i2c_t::ReadRegister function that uses the 
        ::sony_i2c_t::Read and ::sony_i2c_t::Write to perform a 
        sub-addressed I2C read.
        
        I2C output/input:
        [S][Addr][SubAddr][Sr][Addr][Data0]....[DataN][P]
 
 @param pI2c The I2C instance.
 @param deviceAddress The device address.
 @param subAddress The sub address.
 @param pData The buffer to store the register contents into.
 @param size The number of bytes to read from the device.

 @return SONY_RESULT_OK if successful.

*/
sony_result_t sony_i2c_CommonReadRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t* pData, uint32_t size);
/**
 @brief Provides a ::sony_i2c_t::WriteRegister function that uses 
        the ::sony_i2c_t::Write function enabling the STOP and START
        I2C conditions.

        I2C output:
        [S][Addr][SubAddr][Data0]....[DataN][P]

        The maximum burst write is 128 bytes.

 @param pI2c I2C driver instance.
 @param deviceAddress The device address.
 @param subAddress The sub address.
 @param pData The buffer to write into register contents.
 @param size The number of bytes to write to the  device.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_i2c_CommonWriteRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, const uint8_t* pData, uint32_t size);
/**
 @brief Provides a ::sony_i2c_t::WriteOneRegister implementation. 

        I2C output:
        [S][Addr][SubAddr][Data0][P]

 @param pI2c I2C driver instance.
 @param deviceAddress The device address.
 @param subAddress The sub address.
 @param data The byte to write into register contents.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_i2c_CommonWriteOneRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data);

/**
 @brief The driver uses this function to perform a read-modify-write 
        cycle on a single register contents.
 
 @param pI2c I2C driver instance.
 @param deviceAddress The device address.
 @param subAddress The sub address.
 @param data The byte to OR with the register contents.
 @param mask The mask to apply with the data.

 @return SONY_RESULT_OK if successful.
 
*/
sony_result_t sony_i2c_SetRegisterBits(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data, uint8_t mask);

#endif /* SONY_I2C_H */
