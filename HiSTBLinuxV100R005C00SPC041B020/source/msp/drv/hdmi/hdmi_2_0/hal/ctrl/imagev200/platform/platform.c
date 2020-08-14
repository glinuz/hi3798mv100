#include "platform_api.h"
#include "sii_time.h"
#include "si_lib_log_api.h"
#include "si_drv_tx_regs.h"
#include "hdmi_platform.h"

SiiPlatformStatus_t SiiPlatformRead(SiiPlatformInterface_t *pInterfaceInfo, uint8_t devId, uint16_t address, uint8_t *data_in, uint16_t length, uint32_t *pu32VirtualBaseAddr)
{
    uint16_t count = 0;

    volatile uint32_t *addr = (uint32_t *)((((address - BASE_ADDRESS)*4) + (uint8_t *)pu32VirtualBaseAddr));
    for (; count < length; count++)  
    {
        *(data_in + count) = (uint8_t)(*(addr + count));
    }
 
    return SII_PLATFORM_STATUS__SUCCESS;
}

SiiPlatformStatus_t SiiPlatformWrite(SiiPlatformInterface_t *pInterfaceInfo, uint8_t devId, uint16_t address, uint8_t *data_out, uint16_t length, uint32_t *pu32VirtualBaseAddr)
{
    uint16_t count = 0;

    volatile uint32_t *addr = (uint32_t *)((((address - BASE_ADDRESS)*4) + (uint8_t *)pu32VirtualBaseAddr));
    for (; count < length; count++)  
    {
        *(addr + count) = *(data_out + count);
    }  

    return SII_PLATFORM_STATUS__SUCCESS;
}

#if (SII_ENV_BUILD_ASSERT != 0)
void SiiPlatformDebugAssert( const char *pFileName, uint32_t lineNumber,
        uint32_t expressionEvaluation, const char *pConditionText )
{
    // The following two parameters are currently not used
    expressionEvaluation = expressionEvaluation; //to avoid compiler warnings about unused variable
    pConditionText       = pConditionText; // to avoid compiler warnings about unused variable

#if 1
    pFileName = pFileName; // to avoid compiler warnings about unused variable
    lineNumber = lineNumber; // to avoid compiler warnings about unused variable
#else
    // Example how to print the place where error happened:
    // Print file name and line number caused an assertion
    SII_PRINTF("\n\nAssertion failure: File %s, Line %d\n\n", pFileName, (int) lineNumber);
#endif
        SII_PRINTF("Error In %s,Line %d\n",pFileName,lineNumber);       

    // Stop SW execution to make the issue noticeable
    // while(true) {}; // Note: just an example
}
#endif// SII_ENV_BUILD_ASSERT


