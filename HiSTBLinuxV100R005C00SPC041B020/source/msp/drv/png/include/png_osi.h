/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_hal.h
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/11
Description    : PNG osi interface
Function List     :

History           :
Date                Author                Modification
2010/10/11        z00141204        Created file
******************************************************************************/

#ifndef __PNG_OSI_H__
#define __PNG_OSI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_png_type.h"

/*****************************************************************
* func:    open PNG device
* in:    none
* out:    none
* ret:    HI_SUCCESS
* others:    when using png mode,at first open device:
        support multi-course and multi-thread open.
*****************************************************************/
HI_S32 PngOsiOpen(HI_VOID);

/*****************************************************************
* func:    close PNG device
* in:    none
* out:    none
* ret:    none
* others:    pair with PngOsiClose
*****************************************************************/
HI_VOID PngOsiClose(HI_VOID);

/********************************************************************************************
* func:    create decoder
* in:            none
* out:    ps32Handle decoder pointer
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_NOHANDLE    no decoder resource
* ret:       HI_ERR_PNG_NULLPTR pointer is null
* others:    support 32 decoder at most
*********************************************************************************************/
HI_S32 PngOsiCreateDecoder(HI_PNG_HANDLE *ps32Handle);

/*****************************************************************
* func:    destroy decoder
* in:    s32Handle    decoder handle
* out:    none
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decode handle
* others:    pair with HI_PNG_CreateDecoder
*****************************************************************/
HI_S32 PngOsiDestroyDecoder(HI_PNG_HANDLE s32Handle);

/*****************************************************************
* func:    get code stream buffer
* in:    s32Handle    decoder handle
* in:    pstBuf->u32Len    applied buffer size
* out:    pstBuf    return buffer info, include start physical addr and the size of buffer
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decode handle
* ret:    HI_ERR_PNG_NOMEM    memory is not enough
* ret:    HI_ERR_PNG_NULLPTR    parameter pointer is null
* others:
        Get code stream buffer and write data of code stream;
        if pstBuf->u32Len = 0,alloc default size of memory,
        otherwise alloc the size by pstBuf->u32Len.
        pstBuf->u32Len equal to the size assigned by memory in fact,when output.
*****************************************************************/
HI_S32 PngOsiAllocBuf(HI_PNG_HANDLE s32Handle, HI_PNG_BUF_IOC_S *pstBuf);

/********************************************************************************************
* func:    release code stream buffer
* in:    s32Handle    decoder handle
* out:      none
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_NOOPEN    device is not open
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decoder handle
* others:
*********************************************************************************************/
HI_S32 PngOsiReleaseBuf(HI_PNG_HANDLE s32Handle);

/*****************************************************************
* func:    set code stream lenth
* in:    s32Handle    decoder handle
* in:    u32StreamLen     the length of code stream
* out:
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decoder handle
* ret:    HI_ERR_PNG_INVALID_STREAMLEN    invalid code stream length
* ret:  HI_ERR_PNG_NULLPTR pointer is null
* others:
        pair with PngOsiAllocBuf.After getting code stream buffer and write into data,
        set the length of code stream. This length is the lastest length of get by buffer.
        The length of code stream is not more than buff's.
*****************************************************************/
HI_S32 PngOsiSetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 u32Len);

/********************************************************************************************
* func:    Get code stream length
* in:            s32Handle    decoder handle
* in:            u32Phyaddr    buffer physical address
* out:    pu32Len length of code stream
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_NOOPEN    device is not open
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decoder handle
* ret:  HI_ERR_PNG_NULLPTR pointer is null
* others:
*********************************************************************************************/
HI_S32 PngOsiGetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 *pu32Len);

/*****************************************************************
* func:    start decoder
* in:    s32Handle    decoder handle
* in:    stDecInfo    decoder setting information
* out:
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    pointer is null
* ret:  HI_ERR_PNG_NULL_PTR pointer is null
* others:
*****************************************************************/
HI_S32 PngOsiDecode(HI_PNG_HANDLE s32Handle, HI_PNG_DECINFO_S *pstDecInfo);

/*****************************************************************
* func:    Get decode result
* in:    s32Handle    decoder handle
* in:    bBlock if block
* out:    peDecResult decode result
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decoder handle
* others:
*****************************************************************/
HI_S32 PngOsiGetResult(HI_PNG_HANDLE s32Handle, HI_BOOL bBlock, HI_PNG_STATE_E *peDecResult);

/*****************************************************************
* func:    Get physical address of first code stream buffer
* in:    s32Handle    decoder handle
* out:    pu32Phyaddr buf physical address
* ret:    HI_SUCCESS
* ret:    HI_ERR_PNG_INVALID_HANDLE    invalid decoder handle
* others:
*****************************************************************/
HI_S32 PngOsiGetBufParam(HI_PNG_HANDLE s32Handle, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif    /* __PNG_OSI_H__ */
