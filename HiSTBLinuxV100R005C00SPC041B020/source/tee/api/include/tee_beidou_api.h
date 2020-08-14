#ifndef __TEE_BEIDOU_API_H
#define __TEE_BEIDOU_API_H

// reference GPD_TEE_Internal_Core_API_Specification_v1.1.pdf
#if 0 /* 2016/03/16-16:23:59 */
#ifndef TEE_Result
typedef unsigned int TEE_Result;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#endif

/**
 * @brief get Beidou software version
 *
 * @param version [OUT] TEE app will alocate memory space for it
 * @param length [IN] max bytes of version buffer
 *
 * @retval #TEE_SUCCESS version get successfully
 * @retval other Beidou module is not ready
 */
uint32_t TEE_Beidou_GetSoftwareVersion(char * version, 
                                         uint32_t length);


/**
 * @brief get Beidou position and timestamp
 *
 * @param longitude [OUT] longitude, degree * 1000000
 * @param latitude [OUT] latitude, degree * 1000000
 * @param timestamp [OUT] seconds since 1970-1-1 00:00:00
 *
 * @retval #TEE_SUCESS position and timestamp get successfully
 * @retval other Beidou module is not ready
 */
uint32_t TEE_Beidou_GetPositionParameters(int * longitude,
                                            int * latitude,
                                            uint32_t * timestamp);


/**
 * @brief calculate distance between two points
 *
 * maybe this API is not part of the GP API extention, and will be provided 
 * by secured OS in other ways such as common library for all TEE apps. 
 *
 * @param longitudeA [IN] longitude of point A, degree * 1000000
 * @param latitudeA [IN] latitude of point A, degree * 1000000
 * @param longitudeB [IN] longitude of point B, degree * 1000000
 * @param latitudeB [IN] latitude of point B, degree * 1000000
 *
 * @return distance in meter between two points
 */
uint32_t TEE_Beidou_CalculateDistance(int longitudeA,
                                      int latitudeA,
                                      int longitudeB,
                                      int latitudeB);
#endif 
