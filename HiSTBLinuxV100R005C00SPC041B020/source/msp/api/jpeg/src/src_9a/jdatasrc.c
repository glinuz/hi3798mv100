/*
 * jdatasrc.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * Modified 2009-2011 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains decompression data source routines for the case of
 * reading JPEG data from memory or from a file (or any stdio stream).
 * While these routines are sufficient for most applications,
 * some will want to use a different source manager.
 * IMPORTANT: we assume that fread() will correctly transcribe an array of
 * JOCTETs from 8-bit-wide elements on external storage.  If char is wider
 * than 8 bits on your machine, you may need to do some tweaking.
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"

/**
 ** add the include file about the functions that hard decode needed
 ** CNcomment: 增加硬件解码需要的头文件 CNend\n
 **/
 #include "jpeg_hdec_api.h"
 #include "hi_jpeg_config.h"
 /**
  ** this file is added
  ** CNcomment: 这个头文件是新增的 CNend\n
  **/
 #include "jdatasrc.h"

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}

/**
 ** added function to init the mem source
 ** CNcomment:新增处理初始化内存码流的函数 CNend\n
 **/
METHODDEF(void)
init_mem_source (j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	
	/* We reset the empty-input-file flag for each image,
	 * but we don't clear the input buffer.
	 * This is correct behavior for reading a series of images from one source.
	 */
	src->start_of_file = TRUE;

}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{

	  JPEG_HDEC_HANDLE_S_PTR	  pJpegHandle = NULL;
	  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
	  
	  my_src_ptr src = (my_src_ptr) cinfo->src;
	  size_t nbytes;
	  
	 if(HI_TRUE == pJpegHandle->stHDecDataBuf.bReadToDataBuf)
	 {   /**
		  ** read date from file to the stream buffer immediacy
		  ** CNcomment:直接将文件中的码流读到码流buffer中 CNend\n
		  **/
		   nbytes = JFREAD(src->infile, 						          \
						   pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,  \
						   pJpegHandle->stHDecDataBuf.u32ReadDataSize);
		   
		  if(nbytes <= 0)
		  {
			   if (src->start_of_file)
			   {/* Treat empty input file as fatal error */
				   ERREXIT(cinfo, JERR_INPUT_EMPTY);
			   }
			   pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[0] = (JOCTET) 0xFF;
			   pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[1] = (JOCTET) JPEG_EOI;
			   nbytes = 2;
		  }
	 }
	 else
	 {
		  nbytes = JFREAD(src->infile, src->buffer, INPUT_BUF_SIZE);
		  if (nbytes <= 0)
		  {
			   if (src->start_of_file)
			   {/* Treat empty input file as fatal error */
					 ERREXIT(cinfo, JERR_INPUT_EMPTY);
			   }
			   /* Insert a fake EOI marker */
			   src->buffer[0] = (JOCTET) 0xFF;
			   src->buffer[1] = (JOCTET) JPEG_EOI;
			   nbytes = 2;
		   }
		   src->pub.next_input_byte = src->buffer;
	 }
	 src->pub.bytes_in_buffer = nbytes;
	 src->start_of_file = FALSE;
	 
	 return TRUE;

}

/**
 ** added function to fill the physics memory input buffer
 ** CNcomment:新增从物理内存码流获取数据的函数 CNend\n
 **/
METHODDEF(boolean)
fill_mem_input_buffer (j_decompress_ptr cinfo)
{

	  JPEG_HDEC_HANDLE_S_PTR	  pJpegHandle = NULL;
	  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
	  
	  my_src_ptr src = (my_src_ptr) cinfo->src;
	  size_t nbytes;

	  if((NULL == src->stBufStream.img_buffer) || (src->stBufStream.pos >= (HI_U32)src->stBufStream.buffer_size))
	  {
		 nbytes = 0; /** the nbytes is unsigned type **/    
	  }
      else
      {

		    nbytes = (src->stBufStream.pos + INPUT_BUF_SIZE > (HI_U32)src->stBufStream.buffer_size ? \
				      src->stBufStream.buffer_size - src->stBufStream.pos : INPUT_BUF_SIZE);
            				
 		   /**
			** do not need copy the file data
			** CNcomment:软件解码不要使用memcpy，这样可以省了cpy的时间 CNend\n
			**/
		    src->buffer = (JOCTET *)(src->stBufStream.img_buffer+src->stBufStream.pos);
				    	
		    src->stBufStream.pos += nbytes;
			/**
			 ** consume the stream bytes
			 ** CNcomment:消耗了多少码流 CNend\n
			 **/
	        pJpegHandle->stHDecDataBuf.u32ConsumeDataSize += nbytes;
	  }
	  if(nbytes <=0)
	  {
		  
		    if (src->start_of_file)
			{/* Treat empty input file as fatal error */
			      ERREXIT(cinfo, JERR_INPUT_EMPTY);
		    }
		    /* Insert a fake EOI marker */
            src->buffer = (JOCTET *)src->stBufStream.EndMarkBuf;
		    nbytes = 2;
				
	   }
	   src->pub.next_input_byte = src->buffer;
	   src->pub.bytes_in_buffer = nbytes;
	   src->start_of_file       = FALSE;
	  
	   return TRUE;
  
}

METHODDEF(boolean)
fill_retrun_mem_input_buffer (j_decompress_ptr cinfo)
{

	  JPEG_HDEC_HANDLE_S_PTR	  pJpegHandle = NULL;
	  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
	  
	  my_src_ptr src = (my_src_ptr) cinfo->src;
	  size_t nbytes;

	  if(NULL == src->stBufStream.img_buffer)
	  {
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
	  }
	  if(pJpegHandle->stHDecDataBuf.u32ConsumeDataSize == (HI_U32)src->stBufStream.buffer_size)
	  {
			src->stBufStream.pos = 0; /** 第二块码流buffer的偏移 **/
	  }
	  if(pJpegHandle->stHDecDataBuf.u32ConsumeDataSize >= (HI_U32)src->stBufStream.buffer_size)
	  {/** 第一块码流已经用完，请使用第二块 **/
		 nbytes = (src->stBufStream.pos + INPUT_BUF_SIZE > (HI_U32)pJpegHandle->stHDecDataBuf.s32StreamReturnLen ? \
				   pJpegHandle->stHDecDataBuf.s32StreamReturnLen - src->stBufStream.pos : INPUT_BUF_SIZE);
	     src->buffer = (JOCTET *)(pJpegHandle->stHDecDataBuf.pDataVirBufReturn+src->stBufStream.pos);	    	
	     src->stBufStream.pos += nbytes;
         pJpegHandle->stHDecDataBuf.u32ConsumeDataSize += nbytes;
	  }
      else
      {
		    nbytes = (src->stBufStream.pos + INPUT_BUF_SIZE > (HI_U32)src->stBufStream.buffer_size ? \
				      src->stBufStream.buffer_size - src->stBufStream.pos : INPUT_BUF_SIZE);
		    src->buffer = (JOCTET *)(src->stBufStream.img_buffer+src->stBufStream.pos); 	
		    src->stBufStream.pos += nbytes;
	        pJpegHandle->stHDecDataBuf.u32ConsumeDataSize += nbytes;
	  }

	  src->pub.next_input_byte = src->buffer;
	  src->pub.bytes_in_buffer = nbytes;
	  src->start_of_file       = FALSE;
	  
	  return TRUE;
  
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
	  /**
	   ** use call back function, because we may want to use other 
	   ** type function.
	   ** CNcomment: 此处修改bug，有可能使用外部的回调函数 CNend\n
	   **/
	  (void) src->pub.fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

#ifdef CONFIG_JPEG_ADD_SRC_FUNCTION
METHODDEF(boolean)
seek_input_data(j_decompress_ptr cinfo, long byte_offset)
{

	 int ret = 0;
	 my_src_ptr src = (my_src_ptr) cinfo->src;
	 
	 ret = fseek(src->infile,byte_offset,SEEK_SET);
	 if(0 != ret)
	 {
		 ERREXIT(cinfo, JERR_STREAM_BACK_FAILURE);
	 }
	 
	 src->pub.next_input_byte = src->buffer;
	 src->pub.bytes_in_buffer = 0;
	   
	 return TRUE;
	
}
#endif


/*
 * Prepare for input from a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing decompression.
 */

GLOBAL(void)
jpeg_stdio_src (j_decompress_ptr cinfo, FILE * infile)
{

	  my_src_ptr src;
	  JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

	  /* The source object and input buffer are made permanent so that a series
	   * of JPEG images can be read from the same file by calling jpeg_stdio_src
	   * only before the first one.  (If we discarded the buffer at the end of
	   * one image, we'd likely lose the start of the next one.)
	   * This makes it unsafe to use this manager and a different source
	   * manager serially with the same JPEG object.  Caveat programmer.
	   */
	  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
	    cinfo->src = (struct jpeg_source_mgr *)
	      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
					  SIZEOF(my_source_mgr));
	    src = (my_src_ptr) cinfo->src;
	    src->buffer = (JOCTET *)
	      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
					  INPUT_BUF_SIZE * SIZEOF(JOCTET));
	  }

	  src = (my_src_ptr) cinfo->src;
	  src->pub.init_source       = init_source;
	  src->pub.fill_input_buffer = fill_input_buffer;
	  src->pub.skip_input_data   = skip_input_data;
	  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	  src->pub.term_source       = term_source;
#ifdef CONFIG_JPEG_ADD_SRC_FUNCTION
	  src->pub.seek_input_data   = seek_input_data;
#elif defined(CONFIG_JPEG_ADD_GOOGLEFUNCTION)
	  src->pub.seek_input_data   = NULL;
#endif
	  src->infile                = infile;
	  src->pub.bytes_in_buffer   = 0;    /* forces fill_input_buffer on first read */
	  src->pub.next_input_byte   = NULL; /* until buffer loaded */
	  /**
	   ** use the stream data from inside dispose method.
	   ** CNcomment: 使用内部处理的码流数据 CNend\n
	   **/
	   pJpegHandle->stHDecDataBuf.bUseInsideData = HI_TRUE;
	   pJpegHandle->stHDecDataBuf.bUseFileData   = HI_TRUE;
   
   
}


/*
 * Prepare for input from a supplied memory buffer.
 * The buffer must contain the whole JPEG data.
 */
GLOBAL(void)
jpeg_mem_src (j_decompress_ptr cinfo,
	      unsigned char * inbuffer, unsigned long insize)
{


	  my_src_ptr src;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

	  if(NULL == inbuffer ||  0 == insize)
	  {
	     ERREXIT(cinfo, JERR_INPUT_EMPTY);
	  }
	  
	  /* The source object is made permanent so that a series of JPEG images
	   * can be read from the same buffer by calling jpeg_mem_src only before
	   * the first one.
	   */
	  if (cinfo->src == NULL) 
	  {	  
			    cinfo->src = (struct jpeg_source_mgr *)
			      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
							  SIZEOF(my_source_mgr));

			src = (my_src_ptr) cinfo->src;

			src->buffer = (JOCTET *)src->stBufStream.EndMarkBuf;
			
	  }

	  if(HI_FALSE == pJpegHandle->stHDecDataBuf.bUserPhyMem)
	  {
	   /**
	    ** the mem may be physics mem though not call the set mem type function.
	    ** CNcomment: 没有调用设置内存接口也有可能是物理内存 CNend\n
	    **/
        JPEG_HDEC_CheckStreamMemType(cinfo,inbuffer);
	  }
	  pJpegHandle->stHDecDataBuf.u64DataSize = insize;
	  pJpegHandle->stHDecDataBuf.pDataVirBuf = (HI_CHAR*)inbuffer;
	 
	  src = (my_src_ptr) cinfo->src;
	  
	  src->stBufStream.img_buffer    = (char*)inbuffer;
	  src->stBufStream.buffer_size   = insize;
	  src->stBufStream.pos		     = 0;
	  src->stBufStream.EndMarkBuf[0] = (JOCTET)0xFF;
	  src->stBufStream.EndMarkBuf[1] = (JOCTET)JPEG_EOI;

	  src->pub.init_source           = init_mem_source;
	  if(HI_FALSE == pJpegHandle->stHDecDataBuf.bNeedStreamReturn)
	  {
	  		src->pub.fill_input_buffer     = fill_mem_input_buffer;
	  }
	  else
	  {/** 码流回绕的码流处理 **/
			src->pub.fill_input_buffer     = fill_retrun_mem_input_buffer;
	  }
	  src->pub.skip_input_data	     = skip_input_data;
	  src->pub.resync_to_restart     = jpeg_resync_to_restart;
	  src->pub.term_source		     = term_source;
#ifdef CONFIG_JPEG_ADD_SRC_FUNCTION
	  src->pub.seek_input_data       = seek_input_data;
#elif defined(CONFIG_JPEG_ADD_GOOGLEFUNCTION)
	  src->pub.seek_input_data       = NULL;
#endif
	  src->pub.bytes_in_buffer		 = 0;
	  src->pub.next_input_byte		 = NULL;
	  
	  /**
	   ** use the stream data from inside dispose method.
	   ** CNcomment: 使用内部处理的码流数据 CNend\n
	   **/
	  pJpegHandle->stHDecDataBuf.bUseInsideData = HI_TRUE;

}
