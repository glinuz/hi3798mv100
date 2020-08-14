/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: hipng_accelerate.h
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/18
Description	: libpng Adaptation layer
Function List 	:

History       	:
Date				Author        		Modification
2010/10/18		z00141204		Created file      	
2012/03/26		zkf48693		        modify file
******************************************************************************/

#ifndef __HIPNG_ACCELERATE_H__
#define __HIPNG_ACCELERATE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"

#define PNG_NO_PEDANTIC_WARNINGS
#include "png.h"
#include "hi_png_type.h"
#include "hi_png_api.h"
#include "hipng.h"
#include "hi_gfx_comm.h"

#ifdef CONFIG_GFX_MEM_MMZ
#define HIPNG_COMMON_MMZ
#endif

#define HIPNG_NO_DEC 0x0
#define HIPNG_SW_DEC 0x1
#define HIPNG_HW_DEC 0x2
#define HIPNG_HWCOPY_DEC 0x4

    /* the  Control structure of hardware dec */
    typedef struct tag_hipng_struct_hwctl_s
    {
        HI_U8 u8DecMode;         /* dec mode*/
        HI_U8 u8UserDecMode; /* dec mode of user Specified */
	    HI_U8 u8SupportDecMode;	
        HI_BOOL bSyncDec;       /* whether  or not Synchronous decoding*/
        HI_PNG_HANDLE s32Handle;    /* handle of hardware dec*/
        HI_PNG_STATE_E eState;  /* status of Decoder*/
        png_rw_ptr read_data_fn;    /* libpng original IO function pointer */

        HI_PNG_READ_FN read_data_fn_hw; /* hardware IO function pointer */
        png_bytepp image;       /* goal dec memory */
        png_uint_32 idat_size;     /* current IDAT size for read */
        png_uint_32 crc;           /* current chunk CRC value */
        png_uint_32 chunk_name;
        png_uint_32 pallateaddr;
        HI_BOOL bPallateAlpha;
        HI_U32 u32TmpPhyaddr;
	    HI_U8 *pu8TmpImage;
        HI_U32 u32TmpStride;
        HI_U32 u32InflexionSize;
	    HI_U32 u32CopyRows;
	    HI_S32 s32MmzFd;
		HI_VOID *pTmpMemData;
		HI_VOID *pPallateMemData;
	}hipng_struct_hwctl_s;

    /*****************************************************************
    * func:  create the hardware information struct
    * in:	user_png_ver,error_ptr,png_error_ptr,warn_fn
    * out:	struct point
    * ret:   none
    * others:
    *****************************************************************/
    hipng_struct_hwctl_s *hipng_create_read_struct_hw(png_const_charp user_png_ver, png_voidp error_ptr,
            png_error_ptr error_fn, png_error_ptr warn_fn);

    /*****************************************************************
    * func: destroy hardware information struct
    * in:	pstHwctlStruct (hardware information struct)
    * out:	none
    * ret:   none
    * others:
    *****************************************************************/
    HI_VOID hipng_destroy_read_struct_hw(hipng_struct_hwctl_s *pstHwctlStruct);

    /*****************************************************************
    * func:	png hardware dec
    * in:	png_ptr png dec struct
    * in:    image target line struct
    * out:	image data
    * ret:	HI_SUCCESS	decode success
    *		HI_FAILURE	decode failed
    * others:
    *****************************************************************/
    HI_S32 hipng_read_rows(png_structp png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows);

    void hipng_finish_switch(png_structp png_ptr);

    HI_VOID hipng_read_transform_info(png_structp png_ptr, png_infop info_ptr);

#ifdef PNG_DO_PREMULTI_TO_ALPHA
    HI_VOID png_do_premulti_alpha PNGARG((png_row_infop row_info, png_bytep row,png_uint_32 flags));
#endif

    HI_VOID hipng_set_outfmt(png_structp png_ptr, int transforms);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HIPNG_ACCELERATE_H__ */

