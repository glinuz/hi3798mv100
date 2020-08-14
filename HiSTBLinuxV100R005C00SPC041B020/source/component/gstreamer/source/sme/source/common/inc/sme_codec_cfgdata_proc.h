/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_codec_cfgdata_proc.h
 * @brief   声明 各种codeccfgdata转换的函数
 * @author
 * @date    2014/4/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/4
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __SME_CODEC_CFGDATA_PROC_H__
#define __SME_CODEC_CFGDATA_PROC_H__

#include "sme_macro.h"
#include "sme_media_type.h"

#define SME_AVC_NAL_HEAD_LEN 4
#define SME_HEVC_NAL_HEAD_LEN 4
#define SME_MPEG4_HEAD_LEN 3

#define SME_HAS_AVC_NAL(pu8Data)\
(((0 == pu8Data[0]) && (0 == pu8Data[1])\
&& (0 == pu8Data[2]) && (1 == pu8Data[3])) ? ICS_TRUE : ICS_FALSE)

#define SME_GET_UINT_FOR_BGE(pu8Data)\
((((V_UINT32)pu8Data[0]) << 24) | (((V_UINT32)pu8Data[1]) << 16)\
| (((V_UINT32)pu8Data[2]) << 8) | ((V_UINT32)pu8Data[3]))

UTILS_EXTERN_C_BEGIN

//将文件层的avc cfgdata 转换成nal 的sps pps
V_BOOL SME_AvcRawCfgDataToNal(IN ST_SME_CFG_DATA stCfgData,
                        INOUT PST_AVCNALCFG_LIST pstAvcCfgSet,
                        INOUT V_UINT32* pu32Profile);

V_BOOL SME_HevcRawCfgDataToNal(IN ST_SME_CFG_DATA stCfgData,
                        INOUT PST_HEVCNALCFG_LIST pstHevcCfgSet);

V_VOID SME_AddNalHeaderToAvcData(INOUT V_UINT8* pu8Data,
                        IN V_UINT32 u32Len);

V_VOID SME_AddNalHeaderToHevcData(INOUT V_UINT8* pu8Data,
                        IN V_UINT32 u32Len);
UTILS_EXTERN_C_END


#endif //__GST_SMECODEC_CFGDATA_PROC_H__
