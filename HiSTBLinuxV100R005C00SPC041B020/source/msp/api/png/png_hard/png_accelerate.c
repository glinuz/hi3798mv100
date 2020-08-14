/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	        : png_accelerate.c
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/18
Description	: libpng Adaptation
Function List 	:

History       	:
Date				Author        		Modification
2010/10/18		z00141204		Created file
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "hipng_accelerate.h"
#include "pngpriv.h"

#define PNG_CHUNK_HEAD_LEN 8
#define PNG_CHUNK_TAIL_LEN 4

#define PNG_INFLEXION_SIZE (40*40)

#define HIPNG_MAX_WIDTH 4096
#define HIPNG_MAX_HEIGHT 4096
#define HIPNG_MIN_HEIGHT 1
#define HIPNG_MAX_MEM (10*1024*1024)

    /*****************************************************************
    * func:create the struct of hardWare dec
    * in:	        user_png_ver,error_ptr,png_error_ptr,warn_fn Consistent with libpng
    * out:	stuct PT
    * ret:	HI_SUCCESS
    *		HI_FAILURE
    * others:
    *****************************************************************/
    hipng_struct_hwctl_s *hipng_create_read_struct_hw(png_const_charp user_png_ver, png_voidp error_ptr,
    	png_error_ptr error_fn, png_error_ptr warn_fn)
    {
        HI_PNG_HANDLE s32Handle = -1;
        HI_S32 s32Ret = HI_SUCCESS;
        hipng_struct_hwctl_s *pstPngStruct = NULL;
		HI_S32 s32MemFd;

        /* Remove the alarm */
        (HI_VOID)user_png_ver;
        (HI_VOID)error_ptr;
        (HI_VOID)error_fn;
        (HI_VOID)warn_fn;

#ifdef HIPNG_COMMON_MMZ
		s32MemFd = 0;
#else
		s32MemFd = gfx_mem_open();
		if (s32MemFd <= 0)
		{
			ALOGE("ion_open failed!\n");
			return NULL;
		}
#endif

        s32Ret = HI_PNG_Open();
        if (s32Ret < 0)
        {
#ifndef HIPNG_COMMON_MMZ
			gfx_mem_close(s32MemFd);
#endif
            return NULL;
        }

        s32Ret = HI_PNG_CreateDecoder(&s32Handle);
        if (s32Ret < 0)
        {
            HI_PNG_Close();
#ifndef HIPNG_COMMON_MMZ
			gfx_mem_close(s32MemFd);
#endif
            return NULL;
        }
    
        pstPngStruct = (hipng_struct_hwctl_s *)malloc(sizeof(hipng_struct_hwctl_s));
        if (NULL == pstPngStruct)
        {
            HI_PNG_DestroyDecoder(s32Handle);
            HI_PNG_Close();
#ifndef HIPNG_COMMON_MMZ
			gfx_mem_close(s32MemFd);
#endif
            return NULL;
        }

        memset(pstPngStruct, 0, sizeof(hipng_struct_hwctl_s));

	    pstPngStruct->u8DecMode = HIPNG_NO_DEC;
		pstPngStruct->u8SupportDecMode = (HIPNG_SW_DEC | HIPNG_HW_DEC | HIPNG_HWCOPY_DEC);
  	    pstPngStruct->u8UserDecMode = (HIPNG_SW_DEC | HIPNG_HW_DEC | HIPNG_HWCOPY_DEC);
        pstPngStruct->s32Handle = s32Handle;
        pstPngStruct->u32InflexionSize = PNG_INFLEXION_SIZE;
        pstPngStruct->bSyncDec = HI_TRUE;
		pstPngStruct->s32MmzFd = s32MemFd;

        return pstPngStruct;
    }

    /*****************************************************************
    * func:	destroy the struct of hardWare dec
    * in:	pstHwctlStruct the struct of hardWare dec
    * out:	none
    * ret:   none
    * others:
    *****************************************************************/
    HI_VOID hipng_destroy_read_struct_hw(hipng_struct_hwctl_s *pstHwctlStruct)
    {
#ifdef CONFIG_GFX_MMU_SUPPORT
		HI_BOOL bMmu = HI_TRUE;
#else
		HI_BOOL bMmu = HI_FALSE;
#endif

        if (NULL == pstHwctlStruct)
        {
            return;
        }

        HI_PNG_DestroyDecoder(pstHwctlStruct->s32Handle);

        if (pstHwctlStruct->pallateaddr != 0)
        {
#ifdef HIPNG_COMMON_MMZ
            HI_GFX_FreeMem(pstHwctlStruct->pallateaddr, bMmu);
#else
            HI_GFX_FreeMem(pstHwctlStruct->s32MmzFd, pstHwctlStruct->pallateaddr, pstHwctlStruct->pPallateMemData, bMmu);
#endif
            pstHwctlStruct->pallateaddr = 0;
        }

        HI_PNG_Close();

        if (pstHwctlStruct->s32MmzFd)
        {
#ifndef HIPNG_COMMON_MMZ
			gfx_mem_close(pstHwctlStruct->s32MmzFd);
#endif		
        }

        free(pstHwctlStruct);

        return;
    }

    /* fill to stream */
    HI_S32 hipng_read_stream(png_structp png_ptr)
    {
        HI_PNG_BUF_S stBuf = {0, 0, 0};
        HI_S32 s32Ret = HI_SUCCESS;
        hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
        HI_UCHAR ucChunkHead[PNG_CHUNK_HEAD_LEN] = {0, 0, 0, 0, 'I', 'D', 'A', 'T'};
        HI_UCHAR *pBuf = NULL;      /* stream buf head point*/
        HI_UCHAR *pWrite = NULL;    /* stream buf write point*/
        HI_BOOL bNewChunk = HI_TRUE;    /* head data*/
        HI_U8 u8ReadHeadCount = 0;      /* length of has write to data trunk */
        HI_U32 u32IdatSize = png_ptr->idat_size + PNG_CHUNK_HEAD_LEN + PNG_CHUNK_TAIL_LEN;
        HI_U32 u32Len = 0;

        HI_PNG_BUF_S stTempBuf;
        HI_BOOL bEnd = HI_FALSE;

        memset(&stTempBuf,0,sizeof(HI_PNG_BUF_S));
        
        ucChunkHead[0] = (png_ptr->idat_size >> 24) & 0xff;
        ucChunkHead[1] = (png_ptr->idat_size >> 16) & 0xff;
        ucChunkHead[2] = (png_ptr->idat_size >> 8) & 0xff;
        ucChunkHead[3] = png_ptr->idat_size & 0xff;


        while (!bEnd)
        {
            if ((0 == stBuf.u32Size) && (0 == stTempBuf.u32Size))
            {
                stBuf.u32Size = u32IdatSize;
                s32Ret = HI_PNG_AllocBuf(pstHwStruct->s32Handle, &stBuf);
                if (s32Ret < 0)
                {
                    return HI_FAILURE;
                }
                pBuf = (HI_UCHAR *)stBuf.pVir;
                pWrite = pBuf;
            }
            else if (0 == stBuf.u32Size)
            {
                memcpy(&stBuf, &stTempBuf, sizeof(HI_PNG_BUF_S));
                memset(&stTempBuf, 0, sizeof(HI_PNG_BUF_S));
                pBuf = (HI_UCHAR *)stBuf.pVir;
                pWrite = pBuf;
            }

            /* fill IDAT head data*/
            if (bNewChunk)
            {
                u32Len = (HI_U32)(PNG_CHUNK_HEAD_LEN - u8ReadHeadCount) < stBuf.u32Size ? (HI_U32)(PNG_CHUNK_HEAD_LEN - u8ReadHeadCount):stBuf.u32Size;
                memcpy(pWrite, &ucChunkHead[u8ReadHeadCount], u32Len);
                pWrite += u32Len;
                u32IdatSize -= u32Len;
                stBuf.u32Size -= u32Len;
                u8ReadHeadCount += u32Len;
                if (u8ReadHeadCount == PNG_CHUNK_HEAD_LEN)
                {
                    bNewChunk = HI_FALSE;
                }
            }

            if (u32IdatSize)
            {
                u32Len = (u32IdatSize < stBuf.u32Size)?u32IdatSize:stBuf.u32Size;
                png_read_data(png_ptr, pWrite, u32Len);
                u32IdatSize -= u32Len;
                stBuf.u32Size -= u32Len;
                pWrite += u32Len;
            }
            else if ((stBuf.u32Size < PNG_CHUNK_HEAD_LEN) && (HI_NULL == stTempBuf.pVir))
            {
                s32Ret = HI_PNG_AllocBuf(pstHwStruct->s32Handle, &stTempBuf);
                if (s32Ret < 0)
                {
                    s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);
                    return HI_FAILURE;
                }
            }
            /* read next chunk */
            else
            {
                png_ptr->idat_size = png_read_chunk_header(png_ptr);
                if (png_IDAT == png_ptr->chunk_name)
                {
                    bNewChunk = HI_TRUE;
                    u8ReadHeadCount = 0;
                    u32IdatSize = png_ptr->idat_size + PNG_CHUNK_HEAD_LEN + PNG_CHUNK_TAIL_LEN;

                    ucChunkHead[0] = (png_ptr->idat_size >> 24) & 0xff;
                    ucChunkHead[1] = (png_ptr->idat_size >> 16) & 0xff;
                    ucChunkHead[2] = (png_ptr->idat_size >> 8) & 0xff;
                    ucChunkHead[3] = png_ptr->idat_size & 0xff;
                }
                else
                {
                    bEnd = HI_TRUE;
                    s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);

                    pstHwStruct->idat_size = png_ptr->idat_size;
                    pstHwStruct->crc = png_ptr->crc;
					pstHwStruct->chunk_name = png_ptr->chunk_name;
                }
            }

            /* set stream length*/
            if (0 == stBuf.u32Size)
            {
                s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);
                memset(&stBuf, 0, sizeof(HI_PNG_BUF_S));
            }
        }
        return HI_SUCCESS;
    }

    HI_S32 hipng_set_transform(png_structp png_ptr, HI_PNG_TRANSFORM_S *pstTransform)
    {
        HI_U32 *pu32Clut = HI_NULL;
        HI_U32 i = 0;
        HI_U8 u8Alpha = 0;
        hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);

        pstTransform->e124To8Mode = HI_PNG_124TO8_ACCURATE;
        pstTransform->e16To8Mode = HI_PNG_16TO8_BRIEF;

        /* 1.if 1/2/4bit gray image,expand 8bit; alpha deal with
            2.if palette to RGB*/
        if (png_ptr->transformations & PNG_EXPAND)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADJUSTPIXELORDER;
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_GRAY124TO8;
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_CLUT2RGB;
            if (png_ptr->color_type & PNG_COLOR_MASK_PALETTE)
            {
            	HI_BOOL bMmu = HI_TRUE;
#ifdef CONFIG_GFX_MMU_SUPPORT
				HI_CHAR *pZoneName = "iommu";
#else
				HI_CHAR *pZoneName = NULL;
#endif
				
                /* alloc palette*/
#ifdef HIPNG_COMMON_MMZ
                pstTransform->u32ClutPhyaddr = HI_GFX_AllocMem(256 * 4, 16, pZoneName, "PNG_PALETTE", &bMmu);
#else
                pstTransform->u32ClutPhyaddr = HI_GFX_AllocMem(pstHwStruct->s32MmzFd, 256 * 4, 16, pZoneName, "PNG_PALETTE", &(pstHwStruct->pPallateMemData), &bMmu);
#endif
				if (0 == pstTransform->u32ClutPhyaddr)
                {
                    return HI_FAILURE;
                }

#ifdef HIPNG_COMMON_MMZ
                pu32Clut = (HI_U32 *)HI_GFX_MapCached(pstTransform->u32ClutPhyaddr, bMmu);
#else
                pu32Clut = (HI_U32 *)HI_GFX_MapCached(pstHwStruct->s32MmzFd, pstTransform->u32ClutPhyaddr, pstHwStruct->pPallateMemData);
#endif
                if (HI_NULL == pu32Clut)
                {
#ifdef HIPNG_COMMON_MMZ
                    HI_GFX_FreeMem(pstTransform->u32ClutPhyaddr, bMmu);
#else
                    HI_GFX_FreeMem(pstHwStruct->s32MmzFd, pstTransform->u32ClutPhyaddr, pstHwStruct->pPallateMemData, bMmu);
#endif
					return HI_FAILURE;
                }

                pstHwStruct->pallateaddr = pstTransform->u32ClutPhyaddr;

                if (png_ptr->num_trans || (png_ptr->transformations & PNG_ADD_ALPHA))
                {
                    pstHwStruct->bPallateAlpha = HI_TRUE;
                }
                else
                {
                    pstHwStruct->bPallateAlpha = HI_FALSE;
                }

                pstTransform->bClutAlpha = pstHwStruct->bPallateAlpha;

                for (i = 0; i < png_ptr->num_palette; i++)
                {
                    if (png_ptr->num_trans)
                    {
                        if (i < png_ptr->num_trans)
                        {
                            u8Alpha = png_ptr->trans_alpha[i];
                        }
                        else
                        {
                            u8Alpha = 0xff;
                        }
                    }
                    else
                    {
                        if (png_ptr->transformations & PNG_ADD_ALPHA)
                        {
                            u8Alpha = png_ptr->filler;
                        }
                        else
                        {
                            u8Alpha = 0xff;
                        }
                    }
                    *(pu32Clut + i) = (u8Alpha << 24) | (png_ptr->palette[i].red << 16)
                                      | (png_ptr->palette[i].green << 8) | png_ptr->palette[i].blue;
                }

#ifdef HIPNG_COMMON_MMZ
				HI_GFX_Flush(0, bMmu);
                HI_GFX_Unmap(pstTransform->u32ClutPhyaddr, bMmu);
#else
				HI_GFX_Flush(pstHwStruct->s32MmzFd, 0, pstHwStruct->pPallateMemData);
                HI_GFX_Unmap(pstHwStruct->s32MmzFd, pstTransform->u32ClutPhyaddr, pstHwStruct->pPallateMemData);
#endif
            }
        }

        /* swap pixels order*/
        if (png_ptr->transformations & PNG_PACKSWAP)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADJUSTPIXELORDER;
        }

        if (png_ptr->transformations & PNG_GRAY_TO_RGB)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_GRAY2RGB;
        }

        if ((png_ptr->transformations & PNG_EXPAND_tRNS) && png_ptr->num_trans)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADDALPHA;
            pstTransform->eAddAlphaMode = HI_PNG_ADDALPHA_TRNS;
            if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
            {
                pstTransform->sTrnsInfo.u16Red = png_ptr->trans_color.red;
                pstTransform->sTrnsInfo.u16Green = png_ptr->trans_color.green;
                pstTransform->sTrnsInfo.u16Blue = png_ptr->trans_color.blue;
            }
            else
            {
                pstTransform->sTrnsInfo.u16Blue = png_ptr->trans_color.gray;
            }
            pstTransform->u16Filler = 0xffff;
        }
        else if (png_ptr->transformations & PNG_ADD_ALPHA
		    || png_ptr->transformations & PNG_FILLER)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADDALPHA;
            pstTransform->eAddAlphaMode = HI_PNG_ADDALPHA_BRIEF;
            pstTransform->u16Filler = png_ptr->filler;
        }

        if (png_ptr->flags & PNG_STRIP_ALPHA)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_STRIPALPHA;
        }

        if (png_ptr->transformations & PNG_BGR)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_BGR2RGB;
        }

        if (png_ptr->transformations & PNG_SWAP_ALPHA)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_SWAPALPHA;
        }

        if (png_ptr->transformations & PNG_16_TO_8)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_16TO8;
			if (16 == png_ptr->bit_depth)
			{
				pstTransform->u16Filler <<= 8;
			}
        }

        if (png_ptr->transformations & HIPNG_ARGB1555)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
            pstTransform->eOutFmt = HI_PNG_COLORFMT_ARGB1555;
        }

        if (png_ptr->transformations & HIPNG_ARGB4444)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
            pstTransform->eOutFmt = HI_PNG_COLORFMT_ARGB4444;
        }

        if (png_ptr->transformations & HIPNG_RGB565)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
            pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB565;
        }

        if (png_ptr->transformations & HIPNG_RGB555)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
            pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB555;
        }

        if (png_ptr->transformations & HIPNG_RGB444)
        {
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
            pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB444;
        }
        if(png_ptr->transformations & HIPNG_PREMULTI_ALPHA)
        {   
#ifdef CONFIG_PNG_PREMULTIALPHA_ENABLE
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_PREMULTI_ALPHA;
#else
            return HI_FAILURE;
#endif
        }
    
        return HI_SUCCESS;
}

	HI_U8 hipng_select_decmode(png_structp png_ptr, png_bytepp image, png_uint_32 num_rows)
	{
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    HI_U32 u32Phyaddr = 0;
	    HI_U32 u32Size;
	    HI_U32 u32Stride = 0;
	    HI_S32 s32Ret;
	    HI_U32 i;
		HI_U8 u8DecMode;

		if (pstHwStruct->u8DecMode != HIPNG_NO_DEC)
		{
			return pstHwStruct->u8DecMode;
		}

	    if (png_ptr->interlaced || (png_ptr->transformations & PNG_PACK)
	        || (png_ptr->transformations & PNG_SHIFT)
	        || (png_ptr->transformations & PNG_SWAP_BYTES)
	        || (png_ptr->transformations & PNG_INVERT_MONO)
	        || (png_ptr->transformations & PNG_QUANTIZE)
	        || (png_ptr->transformations & PNG_COMPOSE)
	        || (png_ptr->transformations & PNG_BACKGROUND_EXPAND)
	        || (png_ptr->transformations & PNG_GAMMA)
	        || (png_ptr->transformations & PNG_INVERT_ALPHA)
	        || (png_ptr->transformations & PNG_USER_TRANSFORM)
	        || (png_ptr->transformations & PNG_RGB_TO_GRAY_ERR)
	        || (png_ptr->transformations & PNG_RGB_TO_GRAY_WARN)
	        || (png_ptr->transformations & PNG_RGB_TO_GRAY) 
	        || ((png_ptr->transformations & PNG_FILLER) && !(png_ptr->flags & PNG_FLAG_FILLER_AFTER))
	        || (png_ptr->width >= HIPNG_MAX_WIDTH)
	        || (png_ptr->height >= HIPNG_MAX_HEIGHT || png_ptr->height <= HIPNG_MIN_HEIGHT)
	        || (png_ptr->read_row_fn)
#ifdef CONFIG_GFX_MMU_SUPPORT
			|| (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#endif
			)
	    {
	        pstHwStruct->u8SupportDecMode &= ~HIPNG_HW_DEC;
			pstHwStruct->u8SupportDecMode &= ~HIPNG_HWCOPY_DEC;
	        goto CHECKSOFT;
	    }

		if (image && (num_rows == png_ptr->height))
		{
			HI_BOOL bMmu;
#ifdef HIPNG_COMMON_MMZ
		    s32Ret = HI_GFX_GetPhyaddr(image[0], &u32Phyaddr, &u32Size, &bMmu);
#else
		    s32Ret = HI_GFX_GetPhyaddr(pstHwStruct->s32MmzFd, image[0], &u32Phyaddr, &u32Size, NULL);
#endif
			if (s32Ret < 0)
		    {
		        pstHwStruct->u8SupportDecMode &= ~HIPNG_HW_DEC;
		        goto CHECKSOFT;
		    }

		    if ( (image[1] - image[0] < 0) ||((HI_U32)(image[1] - image[0]) > u32Size) ||  (((image[1] - image[0]) & 0xf) != 0))
		    {
		        pstHwStruct->u8SupportDecMode &= ~HIPNG_HW_DEC;
		        goto CHECKSOFT;
		    }

		    u32Stride = image[1] - image[0];

		    for (i = 2; i < png_ptr->height; i++)
		    {
		        if (((HI_U32)(image[i] - image[0]) > u32Size) || ((HI_U32)(image[i] - image[i - 1]) != u32Stride)
		            || (image[i] - image[0]  < 0) || (image[i] - image[i - 1] < 0))
		        {
		            pstHwStruct->u8SupportDecMode |= ~HIPNG_HW_DEC;
		        }
		    }
		}
		else
		{
			pstHwStruct->u8SupportDecMode &= ~HIPNG_HW_DEC;
		}
		
CHECKSOFT:
	    if ((png_ptr->transformations & HIPNG_ARGB1555)
	        || (png_ptr->transformations & HIPNG_ARGB4444)
	        || (png_ptr->transformations & HIPNG_RGB565)
	        || (png_ptr->transformations & HIPNG_RGB555)
	        || (png_ptr->transformations & HIPNG_RGB444))
	    {
	        pstHwStruct->u8SupportDecMode &= ~HIPNG_SW_DEC;
	    }

		u8DecMode = (pstHwStruct->u8SupportDecMode & pstHwStruct->u8UserDecMode);

	    if ((u8DecMode & HIPNG_SW_DEC) 
			&& ((u8DecMode & HIPNG_HW_DEC) || (u8DecMode & HIPNG_HWCOPY_DEC)))
	    {
	        if (png_ptr->height * png_ptr->width < pstHwStruct->u32InflexionSize)
			{
	            u8DecMode &= ~HIPNG_HW_DEC;
				u8DecMode &= ~HIPNG_HWCOPY_DEC;
			}
	    }

	    if (!(u8DecMode & HIPNG_HW_DEC) && (u8DecMode & HIPNG_HWCOPY_DEC))
	    {
			HI_U32 u32MemSize = ((pstHwStruct->u32TmpStride + 0xf)&0xfffffff0) * png_ptr->height;
			if (u32MemSize >= HIPNG_MAX_MEM)
			{
				u8DecMode &= ~HIPNG_HWCOPY_DEC;
			}
			else
			{

			 	HI_BOOL bMmu = HI_TRUE;
#ifdef CONFIG_GFX_MMU_SUPPORT
	 			HI_CHAR *pZoneName = "iommu";
#else
		 		HI_CHAR *pZoneName = NULL;
#endif

#ifdef HIPNG_COMMON_MMZ
		        pstHwStruct->u32TmpPhyaddr = HI_GFX_AllocMem(u32MemSize, 
		            16, pZoneName, "HIPNG_IMAGE_BUF", &bMmu);
#else
		        pstHwStruct->u32TmpPhyaddr = HI_GFX_AllocMem(pstHwStruct->s32MmzFd, u32MemSize, 
		            16, pZoneName, "HIPNG_IMAGE_BUF", &(pstHwStruct->pTmpMemData), &bMmu);
#endif
				if (0 == pstHwStruct->u32TmpPhyaddr)
		        {
		        	ALOGE("[HIPNG]%s:%d", __FUNCTION__, __LINE__);
		            u8DecMode &= ~HIPNG_HWCOPY_DEC;
		        }
				else
				{
#ifdef HIPNG_COMMON_MMZ
					pstHwStruct->pu8TmpImage = (HI_U8 *)HI_GFX_MapCached(pstHwStruct->u32TmpPhyaddr, bMmu);
#else
					pstHwStruct->pu8TmpImage = (HI_U8 *)HI_GFX_MapCached(pstHwStruct->s32MmzFd, pstHwStruct->u32TmpPhyaddr, pstHwStruct->pTmpMemData);
#endif
					if (HI_NULL == pstHwStruct->pu8TmpImage)
					{
#ifdef HIPNG_COMMON_MMZ
						HI_GFX_FreeMem(pstHwStruct->u32TmpPhyaddr, bMmu);
#else
						HI_GFX_FreeMem(pstHwStruct->s32MmzFd, pstHwStruct->u32TmpPhyaddr, pstHwStruct->pTmpMemData, bMmu);
#endif
						pstHwStruct->u32TmpPhyaddr = 0;
						u8DecMode &= ~HIPNG_HWCOPY_DEC;
					}
					else
					{
#ifdef HIPNG_COMMON_MMZ
						HI_GFX_Flush(0, bMmu);
#else
						HI_GFX_Flush(pstHwStruct->s32MmzFd, 0, pstHwStruct->pTmpMemData);
#endif
					}
    			}
			}
    	}
	    else
	    {
		    pstHwStruct->u32TmpPhyaddr = u32Phyaddr;
		    pstHwStruct->u32TmpStride = u32Stride;
	    }


	    if (!(u8DecMode & HIPNG_SW_DEC) && !(u8DecMode & HIPNG_HW_DEC)
			&& !(u8DecMode & HIPNG_HWCOPY_DEC))
	    {
	        png_error(png_ptr, "Unsupport!\n");
	    }

		pstHwStruct->u8DecMode = u8DecMode;

	    return pstHwStruct->u8DecMode;
	}

	void hipng_read_fn(png_structp png_ptr, png_bytep pbuf, png_size_t size)
	{
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    png_size_t read = 0;

	    read = pstHwStruct->read_data_fn_hw(pbuf, size, pstHwStruct->s32Handle);
	    if (read != size)
	    {
	        size -= read;
	        pstHwStruct->read_data_fn(png_ptr, pbuf + read, size);
	    }

	    return;
	}

	HI_S32 hipng_get_readfn(png_structp png_ptr)
	{
	    HI_S32 s32Ret = HI_SUCCESS;
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    
	    s32Ret = HI_PNG_GetReadPtr(pstHwStruct->s32Handle, &pstHwStruct->read_data_fn_hw);
	    if (s32Ret < 0)
	    {
	        return HI_FAILURE;
	    }

	    return HI_SUCCESS;
	}

	void hipng_prepare_switch(png_structp png_ptr)
	{
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    HI_S32 s32Ret = 0;
	    pstHwStruct->read_data_fn = png_ptr->read_data_fn;
	    s32Ret = hipng_get_readfn(png_ptr);
	    if (s32Ret < 0)
	    {
	        png_error(png_ptr, "Internal Err!\n");
	    }

	    if (pstHwStruct->read_data_fn_hw != NULL)
	    {
	        png_ptr->read_data_fn = hipng_read_fn;
	        png_ptr->idat_size = png_read_chunk_header(png_ptr);
	    }

	    return;
	}

	void hipng_finish_switch(png_structp png_ptr)
	{
		hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);

		if (HI_NULL == pstHwStruct)
		{
			return;
		}

	    /*Resume original IO funciton after finishing decode*/
		if (png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED)
		{
			pstHwStruct->eState = HI_PNG_STATE_FINISH;
			if (pstHwStruct->read_data_fn)
	    		png_ptr->read_data_fn = pstHwStruct->read_data_fn;
		}

		return;
	}

	void hipng_copy_rows(png_structp png_ptr, png_bytepp row,
	   png_bytepp display_row, png_uint_32 num_rows)
	{
	    HI_U32 i;
	    hipng_struct_hwctl_s *pstHwStruct = HI_NULL;
		HI_U32 u32CopyRows = num_rows;

		if ((NULL == row) && (NULL == display_row))
		{
			return;
		}

	    pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);

		if (pstHwStruct->u32CopyRows + u32CopyRows > png_ptr->height)
		{
			u32CopyRows = png_ptr->height - pstHwStruct->u32CopyRows;
		}

		if (u32CopyRows > 0)
		{
		    for (i = 0; i < u32CopyRows; i++)
		    {
		    	if (row)
		        	memcpy(row[i], pstHwStruct->pu8TmpImage + ((pstHwStruct->u32TmpStride + 0xf)&0xfffffff0) * (i+pstHwStruct->u32CopyRows), pstHwStruct->u32TmpStride);
				if (display_row)
					memcpy(display_row[i], pstHwStruct->pu8TmpImage + ((pstHwStruct->u32TmpStride + 0xf)&0xfffffff0) * (i+pstHwStruct->u32CopyRows), pstHwStruct->u32TmpStride);
		    }

			pstHwStruct->u32CopyRows += u32CopyRows;
		}

		if (pstHwStruct->u32CopyRows >= png_ptr->height)
			pstHwStruct->eState = HI_PNG_STATE_FINISH;

	    return;
	}

    /*****************************************************************
    * func:	png hardware dec
    * in:	png_ptr png dec struct
    * in:    image target line point
    * out:	image data after dec
    * ret:	HI_SUCCESS
    *		HI_FAILURE
    * others:
    *****************************************************************/
	HI_S32 hipng_read_rows(png_structp png_ptr, png_bytepp row,
	   png_bytepp display_row, png_uint_32 num_rows)
	{
	    HI_S32 s32Ret = HI_SUCCESS;
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    HI_PNG_DECINFO_S stInfo;
		HI_U8 u8DecMode;

        memset(&stInfo,0,sizeof(HI_PNG_DECINFO_S));
		if ((row == NULL) && (display_row == NULL))
		{
			return HI_SUCCESS;
		}

		if (HI_NULL == pstHwStruct)
	    {
		    if ((png_ptr->transformations & HIPNG_ARGB1555)
	        	|| (png_ptr->transformations & HIPNG_ARGB4444)
	        	|| (png_ptr->transformations & HIPNG_RGB565)
	        	|| (png_ptr->transformations & HIPNG_RGB555)
	        	|| (png_ptr->transformations & HIPNG_RGB444))
	    	{
	    		png_error(png_ptr, "Unsupport!\n");
	    	}
			return HI_FAILURE;
	    }

		u8DecMode = hipng_select_decmode(png_ptr, row, num_rows);
		
		if (!(u8DecMode & HIPNG_HW_DEC) && !(u8DecMode & HIPNG_HWCOPY_DEC))
		{
			return HI_FAILURE;
		}
		else if (u8DecMode & HIPNG_HW_DEC)
		{
			if (pstHwStruct->eState == HI_PNG_STATE_ERR)
				return HI_FAILURE;
			else if (pstHwStruct->eState == HI_PNG_STATE_FINISH
				|| pstHwStruct->eState == HI_PNG_STATE_DECING)
				return HI_SUCCESS;
		}
		else if (u8DecMode & HIPNG_HWCOPY_DEC)
		{
			if (pstHwStruct->eState == HI_PNG_STATE_ERR)
				return HI_FAILURE;
			else if (pstHwStruct->eState == HI_PNG_STATE_FINISH)
			{
				hipng_copy_rows(png_ptr, row, display_row, num_rows);
				return HI_SUCCESS;
			}
			else if (pstHwStruct->eState == HI_PNG_STATE_DECING)
				return HI_SUCCESS;
		}
        
        /* read stream*/
        s32Ret = hipng_read_stream(png_ptr);
        if (s32Ret < 0)
        {
    	    goto ERR;
        }
        
        s32Ret = hipng_set_transform(png_ptr, &stInfo.stTransform);
        if (s32Ret < 0)
        {
            goto ERR;
        }

        stInfo.bSync = pstHwStruct->bSyncDec;
        stInfo.stPngInfo.eColorFmt = png_ptr->color_type;
        stInfo.stPngInfo.u32Width = png_ptr->width;
        stInfo.stPngInfo.u32Height = png_ptr->height;
        stInfo.stPngInfo.u8BitDepth = png_ptr->bit_depth;
        stInfo.u32Phyaddr = pstHwStruct->u32TmpPhyaddr;
        stInfo.u32Stride = (pstHwStruct->u32TmpStride + 0xf) & 0xfffffff0;

        s32Ret = HI_PNG_Decode(pstHwStruct->s32Handle, &stInfo);
        if (s32Ret < 0)
        {
            goto ERR;
        }

		if (stInfo.bSync)
		{
			pstHwStruct->eState = HI_PNG_STATE_FINISH;
			if (!(u8DecMode & HIPNG_HW_DEC))
				hipng_copy_rows(png_ptr, row, display_row, num_rows);			
		}
		else
		{
			pstHwStruct->image = row;
			pstHwStruct->eState = HI_PNG_STATE_DECING;
		}

			
	    return HI_SUCCESS;

ERR:
		pstHwStruct->eState = HI_PNG_STATE_ERR;
		
		if (!(u8DecMode & HIPNG_SW_DEC))
		{
			png_error(png_ptr, "Internel err!\n");
		}

		hipng_prepare_switch(png_ptr);

		return HI_FAILURE;
	}

	HI_S32 hipng_get_result(png_structp png_ptr, HI_BOOL bBlock)
	{
	    HI_S32 s32Ret = HI_SUCCESS;
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    
	    s32Ret = HI_PNG_GetResult(pstHwStruct->s32Handle, bBlock, &pstHwStruct->eState);
	    if (s32Ret < 0)
	    {
	        return HI_FAILURE;
	    }

	    return HI_SUCCESS;
	}

	void PNGAPI
	png_set_inflexion(png_structp png_ptr, png_uint_32 u32InflexionSize)
	{
	    hipng_struct_hwctl_s *pstHwStruct = NULL;
	    if (NULL == png_ptr)
	    {
	        return;
	    }
	    
	    pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    if (NULL == pstHwStruct)
	    {
	        return;    
	    }
	    
	    pstHwStruct->u32InflexionSize = u32InflexionSize;

	    return;
	}

	void PNGAPI 
	png_read_image_async(png_structp png_ptr, png_bytepp image)
	{    
	    hipng_struct_hwctl_s *pstHwStruct = NULL;
	    
	    if (NULL == png_ptr)
	    {
	        return;
	    }
	    
	    pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    if (pstHwStruct)
	        pstHwStruct->bSyncDec = HI_FALSE;

	    png_read_image(png_ptr, image);

	    return;
	}

	void PNGAPI
	png_get_state(png_structp png_ptr, PNG_BOOL bBlock,PNG_STATE_E *pState)
	{
	    HI_S32 s32Ret = 0;
	    hipng_struct_hwctl_s *pstHwStruct = NULL;

	    if ((NULL == png_ptr) || (NULL == pState))
	    {
	        return;
	    }

	    pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);

	    if (NULL == pstHwStruct)
	    {
	        if (png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED)
	        {
	            *pState = PNG_STATE_FINISH;
	        }
	        else
	        {
	            *pState = PNG_STATE_NOSTART;
	        }
	        return;
	    }

	    if (pstHwStruct->bSyncDec)
	    {
	        *pState = (PNG_STATE_E)pstHwStruct->eState;
	    }
	    else
	    {
	        if (HI_PNG_STATE_DECING == pstHwStruct->eState)
	        {
	            s32Ret = hipng_get_result(png_ptr, (HI_BOOL)bBlock);
	            if (s32Ret < 0)
	            {
	                png_error(png_ptr, "Internal Err!\n");
	            }
	        }
			
	        if (HI_PNG_STATE_ERR == pstHwStruct->eState)
	        {   
	        	png_read_rows(png_ptr, pstHwStruct->image, NULL, png_ptr->height);
	            pstHwStruct->eState = HI_PNG_STATE_FINISH;
	            *pState = PNG_STATE_FINISH;
	        }
	        else
	        {
	            *pState = (PNG_STATE_E)pstHwStruct->eState;
	            if (*pState == PNG_STATE_FINISH)
	            {
	                if (!(pstHwStruct->u8DecMode & HIPNG_HW_DEC)
						&& (pstHwStruct->u8DecMode & HIPNG_HWCOPY_DEC)
	                    && !(png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED))
	                {
	                    hipng_copy_rows(png_ptr, pstHwStruct->image, NULL, png_ptr->height);
	                }
	            }
	        }
	    }

	    return;
	}

	void PNGAPI
	png_set_outfmt(png_structp png_ptr, HIPNG_FMT_E eOutFmt)
	{
	    switch(eOutFmt)
	    {
	        case HIPNG_FMT_ARGB4444:
	        {
	            png_ptr->transformations |= HIPNG_ARGB4444;
	            return;
	        }
	        case HIPNG_FMT_ARGB1555:
	        {
	            png_ptr->transformations |= HIPNG_ARGB1555;
	            return;
	        }
	        case HIPNG_FMT_RGB565:
	        {
	            png_ptr->transformations |= HIPNG_RGB565;
	            return;
	        }
	        case HIPNG_FMT_RGB555:
	        {
	            png_ptr->transformations |= HIPNG_RGB555;
	            return;
	        }
	        case HIPNG_FMT_RGB444:
	        {
	            png_ptr->transformations |= HIPNG_RGB444;
	            return;
	        }
	        default:
	            return;
	    }
	}

	void PNGAPI
	png_set_swdec(png_structp png_ptr)
	{
	    hipng_struct_hwctl_s *pstHwStruct = NULL;
	    
	    if (NULL == png_ptr)
	    {
	        return;
	    }
	    
	    pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    if (pstHwStruct == NULL)
	    {
	        return;
	    }

	    pstHwStruct->u8UserDecMode = HIPNG_SW_DEC;

	    return;
	}


	HI_VOID hipng_read_transform_info(png_structp png_ptr, png_infop info_ptr)
	{
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
	    if (HI_NULL == pstHwStruct)
			return;

	    if ((png_ptr->transformations & HIPNG_ARGB1555)
	    	|| (png_ptr->transformations & HIPNG_ARGB4444)
	    	|| (png_ptr->transformations & HIPNG_RGB565)
	    	|| (png_ptr->transformations & HIPNG_RGB555)
	    	|| (png_ptr->transformations & HIPNG_RGB444))
	    {
	        if (info_ptr->color_type == PNG_COLOR_TYPE_RGBA && info_ptr->bit_depth == 8)
	        {
	            info_ptr->rowbytes /= 2;
	        }
	    }

	    pstHwStruct->u32TmpStride = info_ptr->rowbytes;

	    return;
	}

	void PNGAPI
	png_read_png_async(png_structp png_ptr, png_infop info_ptr,
                       int transforms, png_voidp params)
	{
	    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);

	    pstHwStruct->bSyncDec = HI_FALSE;

	    png_read_png(png_ptr, info_ptr, transforms, params);
	}

	HI_VOID hipng_set_outfmt(png_structp png_ptr, int transforms)
	{
		if (transforms & HIPNG_TRANSFORM_ARGB1555)
			png_set_outfmt(png_ptr, HIPNG_FMT_ARGB1555);
		else if (transforms & HIPNG_TRANSFORM_ARGB4444)
			png_set_outfmt(png_ptr, HIPNG_FMT_ARGB4444);
		else if (transforms & HIPNG_TRANSFORM_RGB565)
			png_set_outfmt(png_ptr, HIPNG_FMT_RGB565);
		else if (transforms & HIPNG_TRANSFORM_RGB555)
			png_set_outfmt(png_ptr, HIPNG_FMT_RGB555);
		else if (transforms & HIPNG_TRANSFORM_RGB444)
			png_set_outfmt(png_ptr, HIPNG_FMT_RGB444);

		return;
	}

#ifdef PNG_DO_PREMULTI_TO_ALPHA
	void PNGAPI
	png_set_premulti_alpha(png_structp png_ptr)
	{
	   png_debug(1, "in png_set_premulti_alpha");

	      if (png_ptr == NULL)
	      return;
	   png_ptr->transformations |= HIPNG_PREMULTI_ALPHA;
	}

	HI_VOID /* PRIVATE */
	png_do_premulti_alpha(png_row_infop row_info, png_bytep row,png_uint_32 flags)
	{
	    png_debug(1, "in png_do_premulti_alpha");
		{
		  png_uint_32 row_width=row_info->width;
		  png_uint_32 i;
		  char r,b,g;
		  if((row_info->color_type == PNG_COLOR_TYPE_RGB&&row_info->bit_depth == 8&&
		    row_info->channels == 4)||(row_info->color_type==PNG_COLOR_TYPE_RGB_ALPHA&&row_info->bit_depth == 8))
		    {
		        if(flags & PNG_FLAG_ALPHA_SWAP)
		        {
		            for(i=0;i<row_width; i++)
		            {
		                  r=((*(row + 1))*(*(row)+1)+128)>>8;
		                  g=((*(row + 2))*(*(row)+1)+128)>>8;
		                  b=((*(row + 3))*(*(row)+1)+128)>>8;
		                  *(row+1)=r;
		                  *(row+2)=g;
		                  *(row+3)=b;
		                    row +=4;
		            }
		        }
		        else
		       {
		            for(i=0;i<row_width; i++)
		            {
		                  r=((*row)*(*(row+3)+1)+128)>>8;
		                  g=((*(row+1))*(*(row+3)+1)+128)>>8;
		                  b=((*(row+2))*(*(row+3)+1)+128)>>8;
		                  *(row)=r;
		                  *(row+1)=g;
		                  *(row+2)=b;
		                    row +=4;
		            } 
		        }    
		    }
		  
		}    
	}
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
