/*
 * jcapistd.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * Modified 2013 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the compression half
 * of the JPEG library.  These are the "standard" API routines that are
 * used in the normal full-compression case.  They are not used by a
 * transcoding-only application.  Note that if an application links in
 * jpeg_start_compress, it will end up linking in the entire compressor.
 * We thus must separate this file from jcapimin.c to avoid linking the
 * whole compression library into a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
	#include "hi_jpeg_config.h"
	#include "jpge_henc_api.h"
	#include "jpge_henc_err.h"
	#include "jpeg_hdec_mem.h"
	#include "hi_type.h"
#endif

/*
 * Compression initialization.
 * Before calling this, all parameters and a data destination must be set up.
 *
 * We require a write_all_tables parameter as a failsafe check when writing
 * multiple datastreams from the same compression object.  Since prior runs
 * will have left all the tables marked sent_table=TRUE, a subsequent run
 * would emit an abbreviated stream (no tables) by default.  This may be what
 * is wanted, but for safety's sake it should not be the default behavior:
 * programmers should have to make a deliberate choice to emit abbreviated
 * images.  Therefore the documentation and examples should encourage people
 * to pass write_all_tables=TRUE; then it will take active thought to do the
 * wrong thing.
 */

GLOBAL(void)
jpeg_start_compress (j_compress_ptr cinfo, boolean write_all_tables)
{

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
	JPGE_HENC_HANDLE_S_PTR	pJpgeHandle = (JPGE_HENC_HANDLE_S_PTR)(cinfo->client_data);
	if(NULL == pJpgeHandle)
	{
		ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
	}
#endif


  if (cinfo->global_state != CSTATE_START)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  if (write_all_tables)
    jpeg_suppress_tables(cinfo, FALSE);	/* mark all tables to be written */

  /* (Re)initialize error mgr and destination modules */
  (*cinfo->err->reset_error_mgr) ((j_common_ptr) cinfo);
  (*cinfo->dest->init_destination) (cinfo);

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
   pJpgeHandle->s32FreeBufSize = cinfo->dest->free_in_buffer;
#endif

  /* Perform master selection of active modules */
  jinit_compress_master(cinfo);
  /* Set up for the first pass */
  (*cinfo->master->prepare_for_pass) (cinfo);
  /* Ready for application to drive first pass through jpeg_write_scanlines
   * or jpeg_write_raw_data.
   */
  cinfo->next_scanline = 0;
  cinfo->global_state = (cinfo->raw_data_in ? CSTATE_RAW_OK : CSTATE_SCANNING);
}


/*
 * Write some scanlines of data to the JPEG compressor.
 *
 * The return value will be the number of lines actually written.
 * This should be less than the supplied num_lines only in case that
 * the data destination module has requested suspension of the compressor,
 * or if more than image_height scanlines are passed in.
 *
 * Note: we warn about excess calls to jpeg_write_scanlines() since
 * this likely signals an application programmer error.  However,
 * excess scanlines passed in the last valid call are *silently* ignored,
 * so that the application need not adjust num_lines for end-of-image
 * when using a multiple-scanline buffer.
 */

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
static HI_U32 jpeg_henc_write_scanlines(j_compress_ptr cinfo, JSAMPARRAY scanlines,
		      JDIMENSION num_lines)
{	

	HI_CHAR* pTmpSrcBuf = NULL;
	HI_S32 s32Offset = 0;
	HI_S32 s32Cnt = 0;
	HI_S32 s32Ret = HI_FAILURE;
	JPGE_HENC_HANDLE_S_PTR  pJpgeHandle = (JPGE_HENC_HANDLE_S_PTR)(cinfo->client_data);

    /** yuv444package的需要填充一位,TDE转AYCbCr8888排布YUAV,编码需求 **/
	switch(cinfo->in_color_space)
	{
		case JCS_YCbCr:
			/** 需要重排，待逻辑优化 **/
			pTmpSrcBuf = (HI_CHAR*)(pJpgeHandle->stMiddleBuf.pInVir[0] + cinfo->next_scanline * pJpgeHandle->EncIn.ViYStride);
			for(s32Cnt = 0; s32Cnt < (HI_S32)(cinfo->image_width * cinfo->input_components);s32Cnt+=3)
			{
				pTmpSrcBuf[s32Offset]     = scanlines[0][s32Cnt];
				pTmpSrcBuf[s32Offset + 1] = scanlines[0][s32Cnt + 1];
				pTmpSrcBuf[s32Offset + 3] = scanlines[0][s32Cnt + 2];
				s32Offset+=4;
			}
			break;
		case JCS_YUV422_PACKAGE:
			if(HI_FALSE == pJpgeHandle->bHasSetStreamInfo)
			{
				pTmpSrcBuf = (HI_CHAR*)(pJpgeHandle->stMiddleBuf.pInVir[0] + cinfo->next_scanline * pJpgeHandle->EncIn.ViYStride);
				memcpy(pTmpSrcBuf,scanlines[0],pJpgeHandle->stStreamInfo.u32InStride[0]);
			}
			break;
		case JCS_YUV420_SP:
			if(HI_FALSE == pJpgeHandle->bHasSetStreamInfo)
			{
				ERREXIT(cinfo, JERR_HENC_COLOR_NOSUPPORT);
			}
		default:
			break;
	}
	if(cinfo->next_scanline == (cinfo->image_height - num_lines))
	{
		s32Ret = JPEG_HENC_Encode(cinfo);
		if(HI_SUCCESS != s32Ret)
		{
			return HI_FAILURE;
		}
		else
		{
			//JPEG_TRACE("hard compress success\n");
		}
	}
	cinfo->next_scanline += num_lines;

	return HI_SUCCESS;

}
#endif

static JDIMENSION jpeg_soft_write_scanlines (j_compress_ptr cinfo, JSAMPARRAY scanlines,JDIMENSION num_lines)
{
  JDIMENSION row_ctr, rows_left;

  if (cinfo->global_state != CSTATE_SCANNING)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->next_scanline >= cinfo->image_height)
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->next_scanline;
    cinfo->progress->pass_limit = (long) cinfo->image_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Give master control module another chance if this is first call to
   * jpeg_write_scanlines.  This lets output of the frame/scan headers be
   * delayed so that application can write COM, etc, markers between
   * jpeg_start_compress and jpeg_write_scanlines.
   */
  if (cinfo->master->call_pass_startup)
    (*cinfo->master->pass_startup) (cinfo);

  /* Ignore any extra scanlines at bottom of image. */
  rows_left = cinfo->image_height - cinfo->next_scanline;
  if (num_lines > rows_left)
    num_lines = rows_left;

  row_ctr = 0;
  (*cinfo->main->process_data) (cinfo, scanlines, &row_ctr, num_lines);
  cinfo->next_scanline += row_ctr;
  return row_ctr;
}

GLOBAL(JDIMENSION)
jpeg_write_scanlines (j_compress_ptr cinfo, JSAMPARRAY scanlines,
		      JDIMENSION num_lines)
{
#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
	HI_S32 s32Ret = HI_SUCCESS;
	JPGE_HENC_HANDLE_S_PTR	pJpgeHandle = (JPGE_HENC_HANDLE_S_PTR)(cinfo->client_data);
	if(NULL == pJpgeHandle)
	{
		ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
	}
	s32Ret = JPGE_HENC_IfSupport(cinfo);
	if(HI_SUCCESS == s32Ret)
	{
		s32Ret = jpeg_henc_write_scanlines(cinfo,scanlines,num_lines);
		if(HI_SUCCESS == s32Ret)
		{
			return num_lines;
		}
		ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
	}
	/**
	 ** if hard encode failure, return soft encode
	 ** 这个地方输入码流已经没有了，要想回退必须重新送一次码流
	 ** 这里不做码流回退处理了
	 **/

	if(  (JCS_YCbCr   != cinfo->in_color_space)
	   &&(JCS_RGB	  != cinfo->in_color_space)
	   &&(JCS_BGR	  != cinfo->in_color_space)
	   &&(JCS_RGB_565 != cinfo->in_color_space)
	   &&(JCS_BGR_565 != cinfo->in_color_space)
	   &&(JCS_ARGB_8888 != cinfo->in_color_space)
	   &&(JCS_ABGR_8888 != cinfo->in_color_space)
	   &&(JCS_ARGB_1555 != cinfo->in_color_space)
	   &&(JCS_ABGR_1555 != cinfo->in_color_space))
	{/** 如果是yuv420sp and yuv422package请使用MCU编码，这个和送码流参数有关系 **/
		ERREXIT(cinfo, JERR_HENC_COLOR_NOSUPPORT);
		return HI_FAILURE;
	}
	else
#endif
	{
		return jpeg_soft_write_scanlines(cinfo,scanlines,num_lines);
	}
}

/*
 * Alternate entry point to write raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
static JDIMENSION jpeg_hard_write_raw_data (j_compress_ptr cinfo, JSAMPIMAGE data, JDIMENSION num_lines)
{

		HI_CHAR* pTmpSrcBuf = NULL;
		HI_S32 s32Offset = 0;
		HI_S32 s32Row = 0;
		HI_S32 s32Cnt = 0;
		HI_S32 s32Ret = HI_FAILURE;
		JPGE_HENC_HANDLE_S_PTR  pJpgeHandle = (JPGE_HENC_HANDLE_S_PTR)(cinfo->client_data);

		switch(pJpgeHandle->EncCfg.YuvSampleType)
		{
			case JPGE_YUV422:
				if(HI_FALSE == pJpgeHandle->bHasSetStreamInfo)
				{
					for(s32Row = 0;s32Row < (HI_S32)(num_lines);s32Row++)
					{
						s32Offset = 0;
						pTmpSrcBuf = (HI_CHAR*)(pJpgeHandle->stMiddleBuf.pInVir[0] + pJpgeHandle->s32YRow * pJpgeHandle->EncIn.ViYStride);
						for(s32Cnt = 0; s32Cnt < (HI_S32)(cinfo->image_width); s32Cnt++)
						{/**UYVY排列编码**/
							pTmpSrcBuf[s32Offset]     = data[0][s32Row][2*s32Cnt];
							pTmpSrcBuf[s32Offset + 2] = data[0][s32Row][2*s32Cnt + 1];
							pTmpSrcBuf[s32Offset + 1] = data[1][s32Row][s32Cnt];
							pTmpSrcBuf[s32Offset + 3] = data[2][s32Row][s32Cnt];
							s32Offset += 4;
						}
						pJpgeHandle->s32YRow++;
					}
				}
				break;
			case JPGE_YUV420:
				if(HI_FALSE == pJpgeHandle->bHasSetStreamInfo)
				{
					for(s32Row = 0;s32Row < (HI_S32)num_lines;s32Row++)
					{
						pTmpSrcBuf = (HI_CHAR*)(pJpgeHandle->stMiddleBuf.pInVir[0] + pJpgeHandle->s32YRow * pJpgeHandle->EncIn.ViYStride);
						memcpy(pTmpSrcBuf,data[0][s32Row],cinfo->image_width);
						pJpgeHandle->s32YRow++;
					}
					for(s32Row = 0;s32Row < (HI_S32)(num_lines >> 1);s32Row++)
					{
						s32Offset = 0;
						pTmpSrcBuf = (HI_CHAR*)(pJpgeHandle->stMiddleBuf.pInVir[1] + pJpgeHandle->s32CRow * pJpgeHandle->EncIn.ViCStride);
						for(s32Cnt = 0; s32Cnt < (HI_S32)(cinfo->image_width >> 1); s32Cnt++)
						{
							pTmpSrcBuf[s32Offset]     = data[2][s32Row][s32Cnt];
							pTmpSrcBuf[s32Offset + 1] = data[1][s32Row][s32Cnt];
							s32Offset+=2;
						}
						pJpgeHandle->s32CRow++;
					}
				}
			default:
				break;
		}
		if(cinfo->next_scanline >= (cinfo->image_height - num_lines))
		{
			s32Ret = JPEG_HENC_Encode(cinfo);
			if(HI_SUCCESS != s32Ret)
			{
				return HI_FAILURE;
			}
			else
			{
				//JPEG_TRACE("hard compress success\n");
			}
		}
		cinfo->next_scanline += num_lines;

		return HI_SUCCESS;
				 
}
#endif

/*
 * Alternate entry point to write raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */
static JDIMENSION jpeg_soft_write_raw_data (j_compress_ptr cinfo, JSAMPIMAGE data, JDIMENSION num_lines)
{
  JDIMENSION lines_per_iMCU_row;

  if (cinfo->global_state != CSTATE_RAW_OK)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->next_scanline >= cinfo->image_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->next_scanline;
    cinfo->progress->pass_limit = (long) cinfo->image_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Give master control module another chance if this is first call to
   * jpeg_write_raw_data.  This lets output of the frame/scan headers be
   * delayed so that application can write COM, etc, markers between
   * jpeg_start_compress and jpeg_write_raw_data.
   */
  if (cinfo->master->call_pass_startup)
    (*cinfo->master->pass_startup) (cinfo);

  /* Verify that at least one iMCU row has been passed. */
  lines_per_iMCU_row = cinfo->max_v_samp_factor * cinfo->min_DCT_v_scaled_size;
  if (num_lines < lines_per_iMCU_row)
    ERREXIT(cinfo, JERR_BUFFER_SIZE);

  /* Directly compress the row. */
  if (! (*cinfo->coef->compress_data) (cinfo, data)) {
    /* If compressor did not consume the whole row, suspend processing. */
    return 0;
  }

  /* OK, we processed one iMCU row. */
  cinfo->next_scanline += lines_per_iMCU_row;
  return lines_per_iMCU_row;
}

GLOBAL(JDIMENSION)
jpeg_write_raw_data (j_compress_ptr cinfo, JSAMPIMAGE data,
		     JDIMENSION num_lines)
{
	#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
	    HI_S32 s32Ret = HI_SUCCESS;
		JPGE_HENC_HANDLE_S_PTR  pJpgeHandle = (JPGE_HENC_HANDLE_S_PTR)(cinfo->client_data);
		if(NULL == pJpgeHandle)
		{
			ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
		}
		pJpgeHandle->bEncMCU = HI_TRUE;
		s32Ret = JPGE_HENC_IfSupport(cinfo);
		if(HI_SUCCESS == s32Ret)
		{
			s32Ret = jpeg_hard_write_raw_data(cinfo,data,num_lines);
			if(HI_SUCCESS == s32Ret)
			{
				return num_lines;
			}
			ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
		}
		else if(HI_TRUE == pJpgeHandle->bHasSetStreamInfo)
		{/** 如果设置了setstream info，硬件编码又不支持，则编码不支持，因为码流不是按照iMCU送入的**/
			ERREXIT(cinfo, JERR_HENC_COLOR_NOSUPPORT);
		}
		else
		{
		}
	#endif
	return jpeg_soft_write_raw_data(cinfo,data,num_lines);
}

