/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

******************************************************************************
  File Name             :   teletext_data.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2012/03/12
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2012/03/12
    Author              :
Modification            :   Created file
******************************************************************************/

#ifndef MPI_TELETEXT_DATA_H__
#define MPI_TELETEXT_DATA_H__

#include "hi_type.h"
#include "teletext_def.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

/*
 *@brief: Initializing the data store Memory
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to initialize.
 */

HI_S32 TTX_Data_Init(HI_VOID);

HI_S32 TTX_Data_DeInit(HI_VOID);

/*
 *@brief: Reset the data store Memory
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to Reset.
 */

HI_S32 TTX_Data_Reset(HI_VOID);

/*
 *@brief: store the 30 packet
 *
 *@param[in] pu8Data the data of 30 packet.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to store data of 30 packet.
 */

HI_S32 TTX_Data_M30In(HI_U8 *pu8Data);

/*
 *@brief: Store one page data
 *
 *@param[in]u32MagNum - magazine number.
 *@param[in]u32PageNum - page number
 *@param[in] u32SubpageNum - sub-code number
 *@param[in]stpage - the date of one page
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to store one page.
 */

HI_S32 TTX_Data_In(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S* pstpage);

/*
 *@brief: search one page which wanted
 *
 *@param[in] u32MagNum- magazine number.
 *@param[in] u32PageNum -  page number
 *@param[in] u32SubpageNum - sub-code number
 *@param[out] pstTmppage - the wanted page data.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to get one page we wanted.
 */

//HI_S32 TTX_Data_Search(HI_U32 u32MagNum, HI_U32 u32PageNum, HI_U32 u32SubpageNum, TTX_PAGE_S_PTR pstTmppage);
HI_S32 TTX_Data_Search(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S_PTR pstTmppage);

HI_S32 TTX_Data_SetFilter(HI_UNF_TTX_PAGE_ADDR_S stReqPgAddr);

/*
 *@brief: bases the search diraction,find the wanted page's magazine number ,page number,subpage number
 *
 *@param[in] pu32MagNum -magazine number.
 *@param[in] pu32PageNum -  page number
 *@param[in] pu32SubpageNum - sub-code number
 *@param[in] enSearchDir - search diraction
 *@param[out] pu32MagNum - magazine number.
 *@param[out] pu32PageNum -  page number.
 *@param[out] pu32SubpageNum - sub-code number
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to get the wanted page
 */

HI_S32 TTX_Data_GetValidPage(HI_U8 *pu8MagNum, HI_U8 *pu8PageNum, HI_U16 *pu16SubpageNum,
                             TTX_SEARCH_DIRECTION_E enSearchDir);

/*
 *@brief: get page's subpage number
 *
 *@param[in] u32MagNum- magazine number.
 *@param[in] u32PageNum -page number
 *@param[in] pu8Length -the max subpage number stored
 *@param[out] pSubpageNum - all subpage
 *@param[out] pu8Length - the reality subpage number.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to get number.
 */

HI_S32 TTX_Data_GetSubpageNum(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U8 *pSubpageNum, HI_U8 *pu8Length);

HI_S32 TTX_Data_StoreInitPage(const TTX_PAGE_S* pstPage);
HI_S32 TTX_Data_Get_TimeCode(HI_U8* pu8Data);
HI_S32 TTX_Data_Set_TimeCode(HI_U8* pu8Data);

HI_S32 TTX_Data_FindPacket(TTX_PAGE_S *pstPage, HI_U8 u8PacketNo, HI_U8 u8DesignCode, HI_U8* pu8DataLine);
HI_S32 TTX_Data_FindPage(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S *pstPage);
HI_S32 TTX_Data_FindFirstValidPage(HI_U8* pu8MagNum, HI_U8* pu8PageNum, HI_U16* pu16SubpageNum);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif
