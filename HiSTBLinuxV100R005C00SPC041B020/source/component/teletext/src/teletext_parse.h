/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

******************************************************************************
  File Name             :   teletext_parse.h
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

#ifndef __TELETEXT_PARSE_H__
#define __TELETEXT_PARSE_H__

#include "hi_unf_ttx.h"
#include "teletext_def.h"
#include "teletext_showpage.h"

#ifdef __cplusplus
extern "C" {
#endif
extern HI_BOOL g_bHasM29_0[8];
extern HI_BOOL g_bHasM29_1[8];
extern HI_BOOL g_bHasM29_4[8];
extern HI_U32 g_u32DefaultColorMap[32];
extern HI_U32 g_u32RedefColorMap[32];
extern HI_BOOL g_bHasM29_0[8];
extern HI_BOOL g_bHasM29_1[8];
extern HI_BOOL g_bHasM29_4[8];
extern TTX_P28_0_M29_0_INFO_S g_stM29_0_Info[8];
extern TTX_P28_1_M29_1_INFO_S g_stM29_1_Info[8];
extern TTX_P28_4_M29_4_INFO_S g_stM29_4_Info[8];

typedef enum tagTTX_DRCS_MODE_E
{
    TTX_DRCS_MODE_0 = 0,/* 12*10*1 */
    TTX_DRCS_MODE_1 = 1,/* 12*10*2 */
    TTX_DRCS_MODE_2 = 2,/* 12*10*4 */
    TTX_DRCS_MODE_3 = 3,/* 6*5*4     */
    TTX_DRCS_MODE_BUTT
}TTX_DRCS_MODE_E;

#define TTX_DEC_TO_HEX(value) ((value)=(((value)/10 *16)+((value)%10)))
#define TTX_HEX_TO_DEC(value) ((value)=(((value)>>4)*10+((value)&0xf)))

#define TTX_CHAR_TO_INT(value) ((value) - '0')
#define TTX_INT_TO_CHAR(value) ((value) + '0')

/*
 *@brief: Set the address of a page.
 *
 *@param[in] u8MagNum - the magazine number
 *@param[in] u8PgNum - the page number
 *@param[in] u16SubPgNum - the subpage number
 *@param[out] pstPgAddr - the point to page address struct.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to initialize.
 */
HI_S32       TTX_Parse_SetPgAddr(HI_UNF_TTX_PAGE_ADDR_S  * pstPgAddr, HI_U8 u8MagNum, HI_U8 u8PgNum, HI_U16 u16SubPgNum);

/*
 *@brief: Get the address of a page
 *
 *@param[in] pstPgAddr - the page address struct.
 *@param[out] u8MagNum - the point to magazine number
 *@param[out] u8PgNum - the point to page number
 *@param[out] u16SubPgNum - the point to subpage number
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to initialize.
 */
HI_S32       TTX_Parse_GetPgAddr(const HI_UNF_TTX_PAGE_ADDR_S pstPgAddr, HI_U8 * pu8MagNum, HI_U8 * pu8PgNum,
                                 HI_U16* pu16SubPgNum);

/*
 *@brief: Push  page data  into the queue
 *
 *@param[in] pstTtxPage - the handle of page context.
 *@param[in] pstOddPage - the point to odd page
 *@param[in] pstEvenPage - the point to even page
 *
 *@retval ::None
 */
HI_S32      TTX_Parse_GetPgData(const HI_VOID* hDataParse, TTX_SEGMENT_S_PTR pstSegment);

/*
 *@brief: The thread to update time and  flash
 *
 *@param[in] pvData - the handle of page context.
 *
 *@retval ::None
 */
HI_VOID      TTX_Parse_UpdateTask(HI_VOID* pvData);

HI_VOID      TTX_Parse_UpdateTime(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID      TTX_Parse_Flash(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID      TTX_Parse_InputNumber(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID      TTX_Parse_InvalidRequest(TTX_PAGE_CONTEXT_S * pstContextHead);

HI_VOID      TTX_Parse_UpdateSubpage(TTX_PAGE_CONTEXT_S * pstContextHead);
HI_S32       TTX_DecodeMOTPage(HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, TTX_MOT_INFO_S* pstMotInfo, HI_BOOL bHasP27);
HI_S32       TTX_DecodePOPPage(HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, TTX_OBJECT_POINTER_S* pstObj, HI_U32* pu32TripPos, HI_U8* pu8Line);
HI_BOOL      TTX_DecodeHam24_18  (HI_U8 * pu8Triplet, HI_U8 *pu8Address, HI_U8 *pu8Mode, HI_U8 *pu8Data);
HI_VOID      TTX_SetPgActPos (TTX_PAGE_CONTEXT_S  *   pstCurrentPoint, HI_U32 u32Row, HI_U32 u32Column,
                                 HI_U32 u32Code, HI_U32 u32Char,HI_U32 u32Masks);
HI_S32       TTX_DecodeDRCSPage(TTX_PAGE_CONTEXT_S  *   pstCurrentPoint, HI_UNF_TTX_PAGE_ADDR_S* pstPgAddr, HI_U8 u8PTUNum, TTX_DRCS_MODE_E* penDRCSMode,  HI_U32* pu32DRCSColorInfo );
HI_VOID      TTX_ConvertLine(HI_U8 * pu8Line, HI_U32 u32Length);
HI_BOOL      TTX_IsEraseFlagSet(TTX_PAGE_S* pstpage);
HI_S32       TTX_Show_Zoom(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_PAGEAREA_S * pstPageArea);
HI_S32       TTT_Show_DrawChar(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_DRAWCAHR_S * pstDrawCharParam);
HI_S32       TTT_Show_FillRect(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_FILLRECT_S * pstFillRectParam);

HI_S32       TTT_Show_Refresh(TTX_PAGE_CONTEXT_S *pstContextHead, HI_UNF_TTX_REFRESHLAYER_S* pstRefrshLayerParam);

#ifdef __cplusplus
}
#endif
#endif
