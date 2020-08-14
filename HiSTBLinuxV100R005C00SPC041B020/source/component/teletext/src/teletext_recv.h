/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

******************************************************************************
  File Name             :   teletext_recv.h
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

#ifndef __TELETEXT_RECV_H__
#define __TELETEXT_RECV_H__

#include "teletext_def.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MIN_TTX_PES_LEN (4)
#define TTX_PACKET_ADDR_POS1 (4)                   /* mag/1 bit line, hammed */
#define TTX_PACKET_ADDR_POS2 (5)                   /* 4 bits of line, hammed */
#define TTX_DC_POS           (6)                   /*designation code hammed8-4*/
#define TTX_MAGAZINE_MASK (0x07)
#define TTX_PAGE_NO_UNIT_POS (6)                   /* LO hex nybble, hammed */
#define TTX_PAGE_NO_TENS_POS (7)                   /* HO hex nybble, hammed */
#define TTX_NYBBLE_SHIFT_CNT (4)
#define TTX_PES_PACKET_LEN (2 * 1024)
#define TTX_BAD_INV_MAGZINE (0xff)
#define TTX_STREAM_ID_POS (3)                   /* PES header offsets */
#define TTX_PRIVATE_STREAM_1 (0xBD)
#define TTX_PCKT_LEN_MSB_POS (4)                   /*packet number position*/
#define TTX_PCKT_LEN_LSB_POS (5)
#define TTX_PCKT_HDR_LENGTH (6)
#define TTX_PES_DATA_START_POS (46)                  /* skip over header line */
#define TTX_BAD_INV_HAMMING (0xFF)
#define TTX_BAD_INV_PAGE (0xFF)
#define TTX_SUBPAGE_THOU_POS (11)                  /*subpage position*/
#define TTX_SUBPAGE_HUND_POS (10)
#define TTX_SUBPAGE_TENS_POS (9)
#define TTX_SUBPAGE_UNIT_POS (8)
#define TTX_C6_POS (7)
#define TTX_DEFAULT (8)

struct argTTX_CONTEXT_S
{
    TTX_HANDLE_FILTER_S_PTR  pstFiltersPtr;              /** pointer to Filter Queue */
    TTX_HANDLE_FILTER_S      stRequest;                  /** Request Queue */
    HI_HANDLE                hVBIHandle;                 /**the handle of VBI output*/
    HI_BOOL                  bVBIUsed;                   /**VBI output used or not*/
    HI_U32                   u32NumActiveRequests;       /**the num of active request*/
    HI_BOOL                  bOpen;                      /**OSD output used or not*/
    HI_BOOL                  bIsSubData;                 /**the output data is teletext subtitle or teletext non subtitle*/
    TTX_INIT_FILTER_S        stInitFilter;               /**the Init Filter*/
    TTX_OBJECTTYPE_E         enMode;                     /**the output mode*/
    TTX_PAGE_FILTER_STATUS_E enPageFilterStatus;         /**the status of Rec data*/
    HI_U8                    u8DataIdentifier;
    pthread_mutex_t          mutex_recv_lock;
    HI_U8                    u8szPesBuffer[TTX_PES_PACKET_LEN];
    HI_U8 *                  pu8WriteDataAddr;          /*The data address of includeing start code PES packet */
    HI_U32                   u32PESWritenLen;           /*The data length of recving PES packket, in byte */
    HI_BOOL                  bAutoPlay;
    HI_BOOL                  bHold;

    HI_UNF_TTX_PACKET_TYPE_E enPacketType;
};
typedef struct argTTX_CONTEXT_S   TTX_CONTEXT_S;
typedef struct argTTX_CONTEXT_S * TTX_CONTEXT_S_PTR;

#define TTX_RECV_CHECK(param) \
    do                                      \
    {                                       \
        if (param == HI_NULL)               \
        {                                   \
            return HI_FAILURE;              \
        }                                   \
    } while (0)


/*
 *@brief: Initializing the data receiving module,initializing the data of this modual
 *
 *@param[out] pphDataRecv - this module handle.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to initialize.
 */

HI_S32 TTX_Recv_Create(HI_VOID** pphDataRecv);

/*
 *@brief:Destroy all data of  data receiving moudle
 *
 *@param[in] hDataRecv - this module handle
 *@param[out] None.
 *
 *@retval ::HI_SUCCESS - upon successful
 *@retval ::HI_FAILURE - failed to destroy.
 */

HI_S32 TTX_Recv_Destroy(HI_VOID* hDataRecv);

/*
 *@brief: create one thread for receiving PES data.
 *
 *@param[in]hDataRecv - this data recv module handle.
 *@param[in]hDataParse - this data parse module handle.
 *@param[in]pstFilter - filter info.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to create thread.
 */

HI_S32 TTX_Recv_SetFilter(HI_VOID* hDataRecv, HI_VOID* hDataParse, TTX_Filter_S_PTR pstFilter );

/*
 *@brief: clear request
 *
 *@param[in]hDataRecv - this data recv module handle.
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed clear request.
 */
HI_S32 TTX_Recv_ClearFilter(HI_VOID* hDataRecv);

/*
 *@brief:set Hold flag
 *
 *@param[in]hDataRecv - this data recv module handle.
 *@param[in]u8Enable - HI_TRUE hold the display ,HI_FALSE not hold
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to set hold flag.
 */

HI_S32 TTX_Recv_Hold(HI_VOID* hDataRecv, HI_BOOL bHold);

/*
 *@brief:set Auto play subpage flag
 *
 *@param[in]hDataRecv - this data recv module handle.
 *@param[in]u8Enable - HI_TRUE open atuo play ,HI_FALSE close auto play
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to set auto play flag.
 */

HI_S32 TTX_Recv_AutoPlay(HI_VOID* hDataRecv, HI_U8 u8Enable);

/*
 *@brief: set init page number
 *
 *@param[in]hDataRecv - this data recv module handle.
 *@param[in]u32MagazineNum - magazine number
 *@param[in] u32PageNum - page number
 *@param[in]bSubtitle - HI_TRUE subtitle HI_FALSE non-subtitle
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to set init page number.
 */

HI_S32 TTX_Recv_SetInitpage(HI_VOID* hDataRecv, HI_U32 u32MagazineNum, HI_U32 u32PageNum, HI_BOOL bSubtitle);

/*
 *@brief: set output type
 *
 *@param[in]hDataRecv - this data recv module handle.
 *@param[in]enType - output mod
 *@param[in] bEnable - HI_TRUE open,HI_FALSE close
 *
 *@retval ::HI_SUCCESS - upon successful.
 *@retval ::HI_FAILURE - failed to store one page.
 */

HI_S32 TTX_Recv_SetOutputType(HI_VOID* hDataRecv, TTX_OBJECTTYPE_E enType, HI_BOOL bEnable);

HI_S32 TTX_Recv_ProcessData(HI_VOID* hDataRecv, HI_U8 *pu8Data, HI_U32 u32DataLength);

HI_S32  TTX_Recv_SetRequest(HI_VOID* hDataRecv, TTX_Filter_S_PTR pstFilter);

HI_S32 TTX_Recv_ClearRequest(HI_VOID* hDataRecv);

#ifdef __cplusplus
}
#endif

#endif
