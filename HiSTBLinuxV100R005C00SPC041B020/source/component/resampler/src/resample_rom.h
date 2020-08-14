
/*****************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_spdif.h
* Description:
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      2006-11-01   gz40717    NULL         Create this file.
*
*****************************************************************************/

#ifndef _RESAMPLERS_ROM_H_
#define _RESAMPLERS_ROM_H_

#include "resampler_v2.h"
#include "resample_config.h"
#include "resampler_statname.h"

 #ifdef __cplusplus
extern "C" {
 #endif


 #ifdef  CONFIG_RES_GENERATE_TRIGTABS_FLOAT

 #else

extern const signed short res_48_48_good_filttab[];
extern const signed int res_48_48_nextphasetab[];

  #ifdef ENA_QUALITY_LOW
extern const signed int res_32_48_low_nextphasetab[];
extern const signed short res_32_48_low_filttab[];
extern const signed int res_44_48_low_nextphasetab[];
extern const signed short res_44_48_low_filttab[];
  #endif

  #ifdef ENA_QUALITY_MEDIUM
extern const signed int res_32_48_mid_nextphasetab[];
extern const signed short res_32_48_mid_filttab[];
extern const signed int res_44_48_mid_nextphasetab[];
extern const signed short res_44_48_mid_filttab[];

/* res_8_11_mid_filttab = res_8_11_good_nextphasetab */
extern const signed short res_8_11_mid_filttab[];

/* res_8_22_mid_filttab = res_8_22_good_nextphasetab */
extern const signed short res_8_22_mid_filttab[];

/* res_8_44_mid_filttab = res_8_44_good_nextphasetab */
extern const signed short res_8_44_mid_filttab[];

/* res_11_12_mid_filttab = res_11_12_good_nextphasetab */
extern const signed short res_11_12_mid_filttab[];

/* res_11_16_mid_filttab = res_11_16_good_nextphasetab */
extern const signed short res_11_16_mid_filttab[];

/* res_11_24_mid_filttab = res_11_24_good_nextphasetab */
extern const signed short res_11_24_mid_filttab[];

/* res_11_32_mid_filttab = res_11_32_good_nextphasetab */
extern const signed short res_11_32_mid_filttab[];

/* res_11_48_mid_filttab = res_11_48_good_nextphasetab */
extern const signed short res_11_48_mid_filttab[];

/* res_12_22_mid_filttab = res_12_22_good_nextphasetab */
extern const signed short res_12_22_mid_filttab[];

/* res_12_44_mid_filttab = res_12_44_good_nextphasetab */
extern const signed short res_12_44_mid_filttab[];
  #endif

  #ifdef ENA_QUALITY_GOOD

/* 8_11 = 16_22 = 32_44*/
extern const signed int res_8_11_good_nextphasetab[];
extern const signed short res_8_11_good_filttab[];

/* 8_12 = 16_24 = 32_48 */
extern const signed int res_8_12_good_nextphasetab[];
extern const signed short res_8_12_good_filttab[];

/* 8_16 = 11_22 = 12_24 = 16_32 = 22_44 = 24_48 */
extern const signed int res_8_16_good_nextphasetab[];
extern const signed short res_8_16_good_filttab[];

/* 8_22 = 16_44 */
extern const signed int res_8_22_good_nextphasetab[];

//extern const signed short res_8_22_good_filttab[];

/* 8_24 = 16_48 */
extern const signed int res_8_24_good_nextphasetab[];
extern const signed short res_8_24_good_filttab[];

/* 8_32 = 11_44 = 12_48 */
extern const signed int res_8_32_good_nextphasetab[];
extern const signed short res_8_32_good_filttab[];

extern const signed int res_8_44_good_nextphasetab[];

//extern const signed short res_8_44_good_filttab[];

extern const signed int res_8_48_good_nextphasetab[];
extern const signed short res_8_48_good_filttab[];

/* 11_12 = 22_24 = 44_48 */
extern const signed int res_11_12_good_nextphasetab[];
extern const signed short res_11_12_good_filttab[];

/* 11_16 = 22_32 */
extern const signed int res_11_16_good_nextphasetab[];

//extern const signed short res_11_16_good_filttab[];

/*11_24 = 22_48*/
extern const signed int res_11_24_good_nextphasetab[];

//extern const signed short res_11_24_good_filttab[];

extern const signed int res_11_32_good_nextphasetab[];

//extern const signed short res_11_32_good_filttab[];

extern const signed int res_11_44_good_nextphasetab[];
extern const signed short res_11_44_good_filttab[];

extern const signed int res_11_48_good_nextphasetab[];

//extern const signed short res_11_48_good_filttab[];

/* 12_16 = 24_32 */
extern const signed int res_12_16_good_nextphasetab[];
extern const signed short res_12_16_good_filttab[];

/* 12_22 = 24_44 */
extern const signed int res_12_22_good_nextphasetab[];

//extern const signed short res_12_22_good_filttab[];

extern const signed int res_12_32_good_nextphasetab[];
extern const signed short res_12_32_good_filttab[];

extern const signed int res_12_44_good_nextphasetab[];

//extern const signed short res_12_44_good_filttab[];

  #endif

 #endif/* CONFIG_RES_GENERATE_TRIGTABS_FLOAT */

 #ifdef __cplusplus
}
 #endif

#endif /* _RESAMPLERS_ROM_H_ */
