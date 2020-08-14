/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: hi_png_api.h
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/09
Description	: PNG application interface
Function List 	: HI_PNG_Open
			  HI_PNG_Close
			  HI_PNG_CreateDecoder
			  HI_PNG_DestroyDecoder
			  HI_PNG_GetBuf
			  HI_PNG_SetStreamLen
			  HI_PNG_Decode
			  HI_PNG_GetResult
			  HI_PNG_GetReadPtr

History       	:
Date				Author        		Modification
2010/10/09		z00141204		Created file
******************************************************************************/
#ifndef __HI_PNG_API_H__
#define __HI_PNG_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif	/* __cplusplus */
#endif	/* __cplusplus */

#include "hi_png_type.h"

    /********************************************************************************************
    * func:	Open PNG device
    * in:	none
    * out:	none
    * ret:	HI_SUCCESS	Open device successfully
    *		HI_ERR_PNG_DEV_NOEXIST	device is not exist
    * others:	when using PNG module, open device at first: support multi_course and multi-thread
    ********************************************************************************************/
    HI_S32 HI_PNG_Open(HI_VOID);

    /********************************************************************************************
    * func:	close PNG device
    * in:	none
    * out:	none
    * ret:	none
    * others:	pair wirh HI_PNG_Open
    ********************************************************************************************/
    HI_VOID HI_PNG_Close(HI_VOID);

    /********************************************************************************************
    * func:	create decoder
    * in:	none
    * out:	ps32Handle decoder handle pointer
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_NOHANDLE	no decoder resource
    * ret:       HI_ERR_PNG_NULLPTR  Null pointer
    * others:	support create 32 decoder at most
    *********************************************************************************************/
    HI_S32 HI_PNG_CreateDecoder(HI_PNG_HANDLE *ps32Handle);

    /********************************************************************************************
    * func:	destroy decoder
    * in:	s32Handle	decoder handle
    * out:	none
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
    * others:	pair with HI_PNG_CreateDecoder
    ********************************************************************************************/
    HI_S32 HI_PNG_DestroyDecoder(HI_PNG_HANDLE s32Handle);

    /********************************************************************************************
    * func:	Alloc code stream buffer
    * in:	s32Handle	decoder handle
    * in:	pstBuf->u32Len	apply the size of buffer
    * out:	pstBuf	return buf information,include  basic physical addr and the size of buffer
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
    * ret:	HI_ERR_PNG_NOMEM	memory is not enough
    * ret:	HI_ERR_PNG_NULLPTR	pointer is null
    * others:
    Get code stream buffer, write into code steam data;
    if pstBuf->u32Len = 0, alloc default size in memory,
    otherwise alloc by the size of pstBuf->u32Len.
    while output, the value of pstBuf->u32Len equal to its used
    ********************************************************************************************/
    HI_S32 HI_PNG_AllocBuf(HI_PNG_HANDLE s32Handle, HI_PNG_BUF_S *pstBuf);

    /********************************************************************************************
    * func:	release code stream buffer
    * in:	s32Handle	decoder handle
    * out:      none
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
    * others:
    *********************************************************************************************/
    HI_S32 HI_PNG_ReleaseBuf(HI_PNG_HANDLE s32Handle);


    /********************************************************************************************
    * func:	set code stream size
    * in:	        s32Handle	decoder handle
    * in:	        pstBuf	 info of code stream buffer
    * out:
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
    * ret:	HI_ERR_PNG_INVALID_PARAM	invalid code stream size
    * ret:  HI_ERR_PNG_NULLPTR  point is null
    * others:
    set the size of code stream, after get code stream buffer and write code stream data
    the size of code stream is not more than the size of buffer
    *********************************************************************************************/
    HI_S32 HI_PNG_SetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 u32Len);

    /********************************************************************************************
    * func:	Get code stream size
    * in:	        s32Handle	decoder handle
    * in:	        u32Phyaddr	physical size of buffer
    * out:	pu32Len length of code stream
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
    * ret:  HI_ERR_PNG_NULLPTR  pointer is null
    * others:
    *********************************************************************************************/
    HI_S32 HI_PNG_GetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 *pu32Len);

    /********************************************************************************************
    * func:	start decode
    * in:	        s32Handle	decoder handle
    * in:	        pstDecInfo	decoder setting information
    * out:
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
    * ret:  HI_ERR_PNG_NULLPTR pointer is null
    * ret:  HI_ERR_PNG_NOSTREAM  no code stream
    * ret:  HI_PNG_ERR_INTERNAL  decode error
    * others:
    *********************************************************************************************/
    HI_S32 HI_PNG_Decode(HI_PNG_HANDLE s32Handle, const HI_PNG_DECINFO_S *pstDecInfo);

    /********************************************************************************************
    * func:	Get decode result
    * in:	s32Handle	decoder handle
    * in:	bBlock		if block
    * out:	pstDecResult	decode result
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
    * ret:	HI_ERR_PNG_NULLPTR	pointer is null
    * others:
    ********************************************************************************************/
    HI_S32 HI_PNG_GetResult(HI_PNG_HANDLE s32Handle, HI_BOOL bBlock, HI_PNG_STATE_E * peDecState);

    /********************************************************************************************
    * func:	Get read function pointer
    * in:	s32Handle	decoder handle
    * out:	pReadFunc 	read fuction
    * ret:	HI_SUCCESS
    * ret:	HI_ERR_PNG_NOOPEN	device is not open
    * ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
    * ret:	HI_ERR_PNG_NULLPTR	pointer is null
    * others:
    *********************************************************************************************/
    HI_S32 HI_PNG_GetReadPtr(HI_PNG_HANDLE s32Handle, HI_PNG_READ_FN *ppReadFunc);

#ifdef __cplusplus
#if __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __cplusplus */

#endif	/* __HI_PNG_API_H__ */
