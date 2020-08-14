/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_algchange.h
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/12/23
 Description   :
******************************************************************************/
#ifndef __PQ_HAL_ALGCHANGE_H__
#define __PQ_HAL_ALGCHANGE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum hiPQ_LAYER_WBC_E
{
    PQ_LAYER_WBC_HD0  = 0,
    PQ_LAYER_WBC_GP0  = 1,
    PQ_LAYER_WBC_G0   = 2,
    PQ_LAYER_WBC_G4   = 3,
    PQ_LAYER_WBC_ME   = 4,
    PQ_LAYER_WBC_FI   = 5,
    PQ_LAYER_WBC_BMP  = 6,

    PQ_LAYER_WBC_BUTT
} PQ_LAYER_WBC_E;

/*TNR*/
HI_S32 PQ_HAL_InitVdpTnr(HI_VOID);
HI_S32 PQ_HAL_SetVdpTnrParaUpd (HI_U32 u32Data);
HI_S32 PQ_HAL_SetVdpTnrLutUpd (HI_U32 u32Data);
HI_S32 PQ_HAL_SetTnrEnable(HI_U32 u32Data, HI_U32 u32En);
HI_S32 PQ_HAL_SetTnrWbcEnable(HI_U32 u32Data, HI_U32 u32Enable);
HI_S32 PQ_HAL_SetTnrWbcCEnable(HI_U32 u32Data, HI_U32 u32Enable);
HI_S32 PQ_HAL_SetTnrWbcRegUp(HI_U32 u32Data);
HI_S32 PQ_HAL_SetTnrWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr, HI_U32 u32CAddr);
HI_S32 PQ_HAL_SetTnrWbcStride(HI_U32 u32Data, HI_U32 u32YStride, HI_U32 u32CStride);
HI_S32 PQ_HAL_SetMadWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr);
HI_S32 PQ_HAL_SetMadWbcStride(HI_U32 u32Data, HI_U32 u32YStride);
HI_S32 PQ_HAL_SetTnrCEnable(HI_U32 u32Data, HI_U32 tnr_c_en);
HI_S32 PQ_HAL_SetTnrRefDataWidth(HI_U32 u32Data, HI_U32 u32Data1);
HI_S32 PQ_HAL_SetTnrRefReqCtrl(HI_U32 u32Data, HI_U32 u32Data1);
HI_S32 PQ_HAL_SetTnrMadDbmReqCtrl(HI_U32 u32Data, HI_U32 u32Data1);
HI_S32 PQ_HAL_SetTnrMadDataMode(HI_U32 u32Data, HI_U32 mad_data_mode);
HI_S32 PQ_HAL_SetTnrWbcCmpEnable(HI_U32 u32Data, HI_U32 u32bEnable);
HI_S32 PQ_HAL_SetTnrWbcDataWidth(HI_U32 u32Data, HI_U32 u32DataWidth);
HI_S32 PQ_HAL_SetMadWbcDataMode(HI_U32 u32Data, HI_U32 mad_data_mode);
HI_S32 PQ_HAL_SetRefDcmpEnable(HI_U32 u32Data, HI_U32 u32bEnable );
HI_S32 PQ_HAL_SetTnrPreAddr(HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32CAddr, HI_U32 u32LStr, HI_U32 u32CStr);
HI_S32 PQ_HAL_SetTnrMadAddr(HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32LStr);
HI_VOID PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID PQ_HAL_SetSmmuCBypass(PQ_LAYER_WBC_E enLayer, HI_U32 u32Enable);
HI_VOID PQ_HAL_Set16RegionSmmuLumBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);
HI_VOID PQ_HAL_Set16RegionSmmuChromBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);

/*SNR*/
HI_S32 PQ_HAL_SetVdpSnrParaUpd (HI_U32 u32Data);
HI_S32 PQ_HAL_SetVdpSnrEnable(HI_U32 u32Data, HI_U32 u32Data1);
HI_S32 PQ_HAL_InitVdpSnr(HI_VOID);

/*DB*/
HI_S32 PQ_HAL_SetVdpDbmEnable(HI_U32 u32Data, HI_BOOL bEn);
HI_S32 PQ_HAL_SetVdpDbmParaUpd (HI_U32 u32Data);
HI_S32 PQ_HAL_InitVdpDbm(HI_VOID);
HI_S32 PQ_HAL_SetSttWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr);
HI_S32 PQ_HAL_SetMadWbcEnable(HI_U32 u32Data, HI_U32 u32Enable);
HI_S32 PQ_HAL_SetMadWbcRegUp(HI_U32 u32Data);
HI_S32 PQ_HAL_SetDbmDetEnable(HI_U32 u32Data, HI_U32 u32Data1);
HI_S32 PQ_HAL_SetDbmInfoReso(HI_U32 u32Data, HI_U32 u32InfoWidth, HI_U32 u32InfoHeight);
HI_S32 PQ_HAL_SetDbmInfoEnable(HI_U32 u32Data, HI_U32  dbm_info_en);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

