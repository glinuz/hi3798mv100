/******************************************************************************
 *
 * Copyright 2014, Silicon Image, Inc.  All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of
 * Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
 *
 *****************************************************************************/
/**
 * @file platform_api.h
 *
 * @brief Platform API functions to initialize a hardware interface and access registers using it
 *
 *****************************************************************************/
#ifndef __PLATFORM_API_H__
#define __PLATFORM_API_H__

#include "si_datatypes.h"
//#include "aardvark.h"
//#include "BlackBox.h"
//#include "cheetah.h"
  
/***** public macro definitions ***********************************************/

#define SII_ENV_BUILD_ASSERT 1 //!< Macro to enable/Disable assering on error
#define SII_ENV_BUILD_LOG_PRINT 0

/***** public type definitions ***********************************************/

/**
* @brief Platfrom operation (read/write/platform initialization) satus
*/
typedef enum
{
    SII_PLATFORM_STATUS__SUCCESS, //!< Platform operation successful
    SII_PLATFORM_STATUS__FAILED //!< platoform opeartion failed
}SiiPlatformStatus_t;

/**
* @brief Platfrom Interface type being used 
*/
typedef enum
{
    SII_PLATFORM_TYPE__GPIO, //!< All pins for GPIOs only (only in Aardvark and Cheetah)
    SII_PLATFORM_TYPE__I2C, //!< I2C interface
    SII_PLATFORM_TYPE__PRIF_OVER_I2C, //!< Parallel interface over I2C interface
    SII_PLATFORM_TYPE__SPI, //!< SPI interface
    SII_PLATFORM_TYPE__APB, //!< APB BUS interface
}SiiPlatformInterfaceType_t;

/**
* @brief Hardware being used for the interface
*/
typedef enum
{
    SII_PLATFORM_HARDWARE__BB, //!< BlackBox (Only I2C interface is allowed)
    SII_PLATFORM_HARDWARE__AARDVARK, //!< Aardvark
    SII_PLATFORM_HARDWARE__CHEETAH, //!< Cheetah
    SII_PLATFORM_HARDWARE__HISI, //!< Hisilicon
}SiiPlatformInterfaceHardware_t;

/**
* @brief Details of interface being used
* @see @ref SiiPlatformInterfaceType_t, @ref SiiPlatformInterfaceHardware_t
*/
typedef struct
{
    SiiPlatformInterfaceType_t interfaceType; //!< Interface type being used
    SiiPlatformInterfaceHardware_t interfaceHardware; //!< Hardware being used for interface
    uint32_t handle; //!< Handle to the hardware interface device opened
}SiiPlatformInterface_t;

/**
* @brief GPIO pins enumeration
*/
typedef enum
{
    SII_PLATFORM_GPIO__0, //!< GPIO pin 0
    SII_PLATFORM_GPIO__1, //!< GPIO pin 1
    SII_PLATFORM_GPIO__2, //!< GPIO pin 2
    SII_PLATFORM_GPIO__3, //!< GPIO pin 3
    SII_PLATFORM_GPIO__4, //!< GPIO pin 4
    SII_PLATFORM_GPIO__5, //!< GPIO pin 5
    SII_PLATFORM_GPIO__6, //!< GPIO pin 6
    SII_PLATFORM_GPIO__7, //!< GPIO pin 7
}SiiPlatformGPIO_t;

/**
* @brief GPIO direction
*/
typedef enum
{
    SII_PLATFORM_GPIO_DIRECTION__INPUT, //!< GPIO pin as Input
    SII_PLATFORM_GPIO_DIRECTION__OUTPUT, //!< GPIO pin as output
    SII_PLATFORM_GPIO_DIRECTION__INOUT, //!< GPIO pin as bi-directional
}SiiPlatformGPIODirection_t;

/**
* brief GPIO signal level
*/
typedef enum
{
    SII_PLATFORM_GPIO_LEVEL__LOW, //!< GPIO pin level is low
    SII_PLATFORM_GPIO_LEVEL__HIGH  //!< GPIO pin level is high
}SiiPlatformGPIOLevel_t;

/**
* @brief Platform layer functions error code
*/
typedef enum
{
	SII_OS_STATUS_SUCCESS,           //!< No error
	SII_OS_STATUS_ERR_INVALID_PARAM, //!< One of parameters of called OSAL function is invalid
	SII_OS_STATUS_ERR_NOT_AVAIL,     //!< OSAL function does not support requested operation
	SII_OS_STATUS_ERR_FAILED         //!< Other error types
} SiiOsStatus_t;

/**
* @brief I2C transaction details for @ref SiiHalMasterI2cTransfer() function
*
* Similar to Linux's \c struct \c i2c_msg structure.
*/
typedef struct
{
	uint8_t addr;      //!< Slave I2C address
	uint16_t cmdFlags;  //!< Flags defining message actions
	uint16_t len;       //!< Buffer length in bytes
	uint8_t *pBuf;      //!< Pointer to input (for write operations) or
	//!< Pointer to output (for read operations) buffer
} SiiI2cMsg_t;

#ifndef SII_MI2C_TEN
/*****************************************************************************/
/** @defgroup SII_I2C_TRANSACTION_BIT_MASK I2C transaction bit mask
* @brief I2C transaction bit mask
*
* Specifies type of I2C transaction in \c cmdFlags member of @ref SiiI2cMsg_t
* used in a parameter of @ref SiiHalMasterI2cTransfer() function.
*
* @see SiiI2cMsg_t
* @see SiiHalMasterI2cTransfer()
*
******************************************************************************/
/* @{ */
#define SII_MI2C_TEN        0x0010  //!< Set for ten bit chip address; cleared otherwise
#define SII_MI2C_RD         0x0001  //!< Set for read data operation; cleared for write operation
#define SII_MI2C_WR         0x0000  //!< Absence of read == write
/* @} */
#endif // SII_MI2C_TEN

#if (SII_ENV_BUILD_LOG_PRINT != 0)
#ifndef DEBUG_PRINT
/**
* @brief Macro for log messages printing
*
* This macro is called from the inside of Silicon Image software and can be
* used during the driver or system debugging.
*
* The first parameter is the logging channel defined in @ref SII_LOG_MSG_VAL.
* The remaining parameters are similar to ones used in C standard \c print() function.
*/
#define DEBUG_PRINT(x,...)  SiiPlatformLoggerPrintf(x,__VA_ARGS__)
#endif // DEBUG_PRINT
#else // SII_ENV_BUILD_LOG_PRINT
#undef DEBUG_PRINT
/**
* @brief Dummy log print macro
*
* Used in cases when no debug logging is needed.
*/
#define DEBUG_PRINT(...)
#endif // SII_ENV_BUILD_LOG_PRINT
/***** public functions ******************************************************/

/*****************************************************************************/
/**
* @brief Initialize the platform of interest. Multiple flatform initialization is allowed.
*        Registers can be accessed using multiple hardware by providing the @ref SiiPlatformInterface_t 
*        to each API.
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
*
* @retval Returns the Success/Failure status of initialization
* @see @ref SiiPlatformInterface_t
*/
//bool_t SiiPlatformInit(SiiPlatformInterface_t *pInterfaceInfo);

/**
* @brief API to read registers.
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
* @param[in] devId - Device Id of the register page.  This parameter has no significance is SPI interface.
* @param[in] address - address of the register in the page.
* @param[out] data_in - pointer to the data.
* @param[in] length - number of registers to read.
*
* @retval Returns the Success/Failure status of read operartion
* @see @ref SiiPlatformInterface_t
*/
SiiPlatformStatus_t SiiPlatformRead(SiiPlatformInterface_t *pInterfaceInfo, uint8_t devId, uint16_t address, uint8_t *data_in, uint16_t length, uint32_t *pu32VirtualBaseAddr);

/**
* @brief API to write into registers.
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
* @param[in] devId - Device Id of the register page. This parameter has no significance is SPI interface.
* @param[in] address - address of the register in the page.
* @param[in] data_out - pointer to the data.
* @param[in] length - number of registers to read.
*
* @retval Returns the Success/Failure status of write operartion
* @see @ref SiiPlatformInterface_t
*/
SiiPlatformStatus_t SiiPlatformWrite(SiiPlatformInterface_t *pInterfaceInfo, uint8_t devId, uint16_t address, uint8_t *data_out, uint16_t length, uint32_t *pu32VirtualBaseAddr);

/**
* @brief API to configure the GPIO pin as input or output are bidirectional.
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
* @param[in] gpioPin - GPIO pin 
*                      In Aardvark and Cheetah hardware interface - SPI pins shall act as GPIOs in I2C configuration
*                                                                 - I2C pins shall act as GPIOs in SPI configuration
* @param[in] direction - direction of GPIO pin operation
* @retval returns the status of operation
* @see @ref SiiPlatformInterface_t, SiiPlatfromGPIO_t
*/
//SiiPlatformStatus_t SiiPlatformGPIOConfig(SiiPlatformInterface_t *pInterfaceInfo, SiiPlatformGPIO_t gpioPin, SiiPlatformGPIODirection_t direction);

/**
* @brief API to drive the GPIO pin.
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
* @param[in] gpioPin - Output GPIO pin
* @param[in] level - Output level GPIO pin
*
* @retval returns the status of operation
* @see @ref SiiPlatformInterface_t, SiiPlatfromGPIO_t
*/
//SiiPlatformStatus_t SiiPlatformGPIOSet(SiiPlatformInterface_t *pInterfaceInfo, SiiPlatformGPIO_t gpioPin, SiiPlatformGPIOLevel_t level);

/**
* @brief API to read the status of GPIO
* 
* @param[in] pInterfaceInfo - pointer to the datastructure containing interface type and hardware being used
* @param[in] gpioPin - input GPIO pin
*
* @retval level level on the GPIO pin

* @see @ref SiiPlatformInterface_t, SiiPlatfromGPIO_t
*/
//SiiPlatformGPIOLevel_t SiiPlatformGPIOStatusGet(SiiPlatformInterface_t *pInterfaceInfo, SiiPlatformGPIO_t gpioPin);

//-------------------------------------------------------------------------------------------------
//! @brief      Returns number of passed milli seconds since TimeInit().
//!
//! @return     Number of milli seconds.
//-------------------------------------------------------------------------------------------------
//uint32_t SiiPlatformTimeMilliGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Blocks execution for x number of milli seconds.
//!
//! @param[in]  milliDelay - Number of milli seconds.
//-------------------------------------------------------------------------------------------------
//void SiiPlatformTimeMilliDelay( uint32_t milliDelay );

//#if(SII_ENV_BUILD_ASSERT)
#if(0)
/**
* @brief Assertion macro to check for internal error conditions
*/
#define SII_PLATFORM_DEBUG_ASSERT( expr ) \
      ( (void)( (/*lint -e{506}*/!(expr)) ? SiiPlatformDebugAssert(__FILE__, __LINE__, (uint32_t)(expr), NULL) : ((void)0) ) )
#else // SII_ENV_BUILD_ASSERT
/**
* @brief Dummy assertion macro
*/
#define SII_PLATFORM_DEBUG_ASSERT( expr ) ( (void)0 )
#endif // SII_ENV_BUILD_ASSERT
#if (SII_ENV_BUILD_ASSERT != 0)
// Assertion handler
// Not needed if SII_ENV_BUILD_ASSERT is set to 0
/*****************************************************************************/
/**
* @brief Debug assertion handler
*
* This function is called when the @ref SII_PLATFORM_DEBUG_ASSERT() macro is called
* and the expression in the parameter is \c 0.
*
* @param[in] pFileName    String with the file name where the assertion happened
* @param[in] lineNumber   String with the line number where the assertion happened
* @param[in] expressionEvaluation  Reserved for the future; do not use
* @param[in] pConditionText        Reserved for the future; do not use
*
* @see SII_PLATFORM_DEBUG_ASSERT()
* @see SII_ENV_BUILD_ASSERT
*
*****************************************************************************/
void SiiPlatformDebugAssert (const char *pFileName, uint32_t lineNumber, uint32_t expressionEvaluation, const char *pConditionText);
#endif // SII_ENV_BUILD_ASSERT

/*****************************************************************************/
/**
* @brief I2C transaction request
*
* The function is similar to Linux's \c i2c_transfer():
* @code
* int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msg, int num);
* @endcode
*
* @param[in] busId   Selects I2C bus
* @param[in] pMsgs   Pointer to array of @ref SiiI2cMsg_t structures
* @param[in] msgNum  Number of @ref SiiI2cMsg_t structures
*
* Typical write operation is described by one SiiI2cMsg_t structure.
* \c pMsgs->pBuf points to a buffer where the first byte
* specifies Silicon Image's register and the rest is the block of data
* to write.
* \c pMsgs->cmdFlags does not have @ref SII_MI2C_RD bit set
* to mark the write transaction.
*
* Typical read operation is described by two SiiI2cMsg_t structures.
* The first \c pMsgs->pBuf points to a buffer where the first byte
* specifies Silicon Image's register.
* \c pMsgs->cmdFlags does not have @ref SII_MI2C_RD bit set
* to mark the write transaction.
* The second \c pMsgs->pBuf points to an input buffer where read data is to be copied over.
* \c pMsgs->cmdFlags has @ref SII_MI2C_RD bit set to mark the read transaction.
*
* @retval SII_OS_STATUS_SUCCESS     No error
* @retval SII_OS_STATUS_ERR_FAILED  Error occurred during the I2C operation
*
* @see SII_I2C_TRANSACTION_BIT_MASK
*
*****************************************************************************/
SiiOsStatus_t SiiHalMasterI2cTransfer (int busId, SiiI2cMsg_t *pMsgs, uint8_t msgNum);


#if (SII_ENV_BUILD_LOG_PRINT != 0)
/*****************************************************************************/
/**
* @brief Log printing
*
* Prints given message into a log (e.g. on a terminal).
* The messages can be filtered (some of them are printed and some not)
* depending on \c printFlags parameter.
*
* Not needed if @ref SII_ENV_BUILD_LOG_PRINT is set to \c 0.
*
* @param[in] printFlags @ref SII_LOG_MSG_VAL "Log channel" selection
* @param[in] ...        \c print() - like parameters
*
* @see SII_LOG_MSG_VAL
* @see DEBUG_PRINT()
* @see SII_ENV_BUILD_LOG_PRINT
*
*****************************************************************************/
void SiiPlatformLoggerPrintf (uint_t printFlags, ...);
#endif // SII_ENV_BUILD_LOG_PRINT

#endif //__PLATFROM_API_H__
