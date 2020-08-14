#include "sre_syscalls_chip.h"
#include "tee_beidou_api.h"

uint32_t TEE_Beidou_GetSoftwareVersion(char* version, uint32_t length)
{
    if (NULL == version)
    {
        uart_printf_func("---[%s.%d]---invalid paramter\n", __FUNCTION__, __LINE__);
        return (uint32_t)-1;
    }

    return (uint32_t)__Beidou_GetSoftwareVersion(version, length);
}

uint32_t TEE_Beidou_GetPositionParameters(int* longitude, int* latitude, uint32_t* timestamp)
{
    if ((NULL == longitude) || (NULL == latitude) || (NULL == timestamp))
    {
        uart_printf_func("---[%s.%d]---invalid paramter\n", __FUNCTION__, __LINE__);
        return (uint32_t)-1;
    }

    return (uint32_t)__Beidou_GetPositionParameters(longitude, latitude, timestamp);
}

uint32_t TEE_Beidou_CalculateDistance(int longitudeA, int latitudeA, int longitudeB, int latitudeB)
{
    return __Beidou_CalculateDistance(longitudeA, latitudeA, longitudeB, latitudeB);
}


