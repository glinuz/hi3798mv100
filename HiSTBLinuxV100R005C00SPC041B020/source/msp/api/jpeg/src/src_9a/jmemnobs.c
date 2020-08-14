/*
 * jmemnobs.c
 *
 * Copyright (C) 1992-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides a really simple implementation of the system-
 * dependent portion of the JPEG memory manager.  This implementation
 * assumes that no backing-store files are needed: all required space
 * can be obtained from malloc().
 * This is very portable in the sense that it'll compile on almost anything,
 * but you'd better have lots of main memory (or virtual memory) if you want
 * to process big images.
 * Note that the max_memory_to_use option is ignored by this implementation.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */

/**
 ** add the include file about the init function
 ** CNcomment: 增加硬件解码需要的头文件 CNend\n
 **/
#include "jmemmgr.h"
#include "jpeg_hdec_api.h"

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
#include "jpge_henc_api.h"
#endif


#ifndef HAVE_STDLIB_H		/* <stdlib.h> should declare malloc(),free() */
extern void * malloc JPP((size_t size));
extern void free JPP((void *ptr));
#endif


#define MEM_ALIGN2MUL(x,a)             ( ((x) + ((a) - 1)) & (~((a) - 1)) )
/*
 * Memory allocation and freeing are controlled by the regular library
 * routines malloc() and free().
 */

GLOBAL(void *)
jpeg_get_small (j_common_ptr cinfo, size_t sizeofobject)
{

	JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
	if(cinfo->is_decompressor)
	{
		if(NULL == pJpegHandle)
		{/** 创建解码器的时候这个还没有初始化 **/
			return (void *) malloc(sizeofobject);
		}
		
		pJpegHandle->u32NeedMemSize = pJpegHandle->u32NeedMemSize + sizeofobject;

		if(0 == pJpegHandle->u32LeaveMemSize)
		{/** 用户态没有做限制 **/
			return (void *) malloc(sizeofobject);
		}
		if(pJpegHandle->u32NeedMemSize > pJpegHandle->u32LeaveMemSize)
		{
			return NULL;
		}
	}
  	return (void *) malloc(sizeofobject);
}

GLOBAL(void)
jpeg_free_small (j_common_ptr cinfo, void * object, size_t sizeofobject)
{
  free(object);
}


/*
 * "Large" objects are treated the same as "small" ones.
 * NB: although we include FAR keywords in the routine declarations,
 * this file won't actually work in 80x86 small/medium model; at least,
 * you probably won't be able to process useful-size images in only 64KB.
 */

GLOBAL(void FAR *)
jpeg_get_large (j_common_ptr cinfo, size_t sizeofobject)
{

#if 1
	   /**
	    ** this use malloc to alloc mem, because the capability is the best
		** 这个地方用malloc分配性能比较好
		** malloc默认不会返回NULL指针在分配不到内存的情况下，所以使用越界的时候会
		** come dump产生，提供了接口使用
		**/
		JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

/**
 **使用meminfo确认内存是否足够
 **/
#ifdef CONFIG_JPEG_USE_MEMINFO_TO_CHECK
		HI_U64 u64MemFree = 0;
		JPEG_HDEC_GetMemFromMemInfo(&u64MemFree);
		if(sizeofobject > (u64MemFree * 1024))
		{
			JPEG_TRACE("jpeg dec need memsize is %d,free memsize is = %lld,now not enough memory\n",sizeofobject,(u64MemFree * 1024));
			return NULL;
		}
#endif
		if(cinfo->is_decompressor)
		{
			if(NULL == pJpegHandle)
			{
				return (void *) malloc(sizeofobject);
			}

			pJpegHandle->u32NeedMemSize = pJpegHandle->u32NeedMemSize + sizeofobject;
			
			if(0 == pJpegHandle->u32LeaveMemSize)
			{/** 用户态没有做限制 **/
				return (void *) malloc(sizeofobject);
			}
			if(pJpegHandle->u32NeedMemSize > pJpegHandle->u32LeaveMemSize)
			{
				return NULL;
			}
		}
        return (void FAR *) malloc(sizeofobject);
#else
	    size_t tSize;
	    void FAR *  pVirtAddr = NULL;
	    size_t PhysAddr = 0;
		
	    large_pool_ptr hdr_ptr;
	    
	    tSize = MEM_ALIGN2MUL(sizeofobject, 64);

		/**
		 ** TC AND XBMC平台的MMZ支持多进程，自己封装必须改实现，所有使用公共模块的
		 **/
        PhysAddr = (size_t)HI_GFX_AllocMem((HI_U32)tSize, 64, (HI_CHAR*)"jpeg", "JPEG");
	    if(0 == PhysAddr)
		{  
	          return NULL;
	    }
#if 1
		/**
		 ** 这个地方有可能会导致解码错误，因为操作这个内存的时候没有对其进行刷cach
		 ** 但由于都是CPU操作，没有对这些物理内存进行操作，所以理论上不会有问题。
		 **/
         pVirtAddr = (void FAR *)HI_GFX_MapCached((HI_U32)PhysAddr);
		 if (NULL == pVirtAddr)
		 {
	          return NULL;
	     }
#else
		pVirtAddr = (void FAR *)HI_GFX_Map((HI_U32)PhysAddr);
		 if (NULL == pVirtAddr)
		 {
	          return NULL;
	     }
#endif
	     hdr_ptr = (large_pool_ptr)pVirtAddr;
	     hdr_ptr->hdr.PhysAddr = PhysAddr;
		 
	     return pVirtAddr;
#endif
}

GLOBAL(void)
jpeg_free_large (j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{
#if 1
        free(object);
#else
	    size_t PhysAddr = 0;
	    large_pool_ptr hdr_ptr;
		HI_S32 s32Ret = HI_SUCCESS;
	    hdr_ptr = (large_pool_ptr)object;
	    PhysAddr = hdr_ptr->hdr.PhysAddr;

		if (0 !=PhysAddr)
		{
		    s32Ret = HI_GFX_Unmap((HI_U32)PhysAddr);
			if(HI_SUCCESS != s32Ret)
			{
			   return;
			}
		    s32Ret = HI_GFX_FreeMem((HI_U32)PhysAddr);
			if(HI_SUCCESS != s32Ret)
			{
			   return;
			}
		}
#endif
}


/*
 * This routine computes the total memory space available for allocation.
 * Here we always say, "we got all you want bud!"
 */

GLOBAL(long)
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{
  return max_bytes_needed;
}


/*
 * Backing store (temporary file) management.
 * Since jpeg_mem_available always promised the moon,
 * this should never be called and we can just error out.
 */

GLOBAL(void)
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
  ERREXIT(cinfo, JERR_NO_BACKING_STORE);
}


/*
 * These routines take care of any system-dependent initialization and
 * cleanup required.  Here, there isn't any.
 */

GLOBAL(long)
jpeg_mem_init (j_common_ptr cinfo)
{
  return 0;			/* just set max_memory_to_use to 0 */
}

GLOBAL(void)
jpeg_mem_term (j_common_ptr cinfo)
{
#ifndef CONFIG_GFX_JPGE_ENC_ENABLE
		/* no work */
		if (cinfo->is_decompressor)
		{
			/** revise at 2011-11-4
			 ** only decompress use the hard device, at this release device
			 ** this fuction should realse at jpeg_finish_decompress and destroy_decompress.
			 ** because it maybe call create decompressor is one time, and finish
			 ** will maybe many times but not call destory decompressor fuction,so
			 ** the device is not closed but has opened, so the signal has not realsed
			 ** this function can call many times, because at this function has check the device
			 **/
			JPEG_HDEC_CloseDev(cinfo);
		}
		JPEG_HDEC_Destroy(cinfo);
#else
		/* no work */
		if (cinfo->is_decompressor)
		{
			/** revise at 2011-11-4
			 ** only decompress use the hard device, at this release device
			 ** this fuction should realse at jpeg_finish_decompress and destroy_decompress.
			 ** because it maybe call create decompressor is one time, and finish
			 ** will maybe many times but not call destory decompressor fuction,so
			 ** the device is not closed but has opened, so the signal has not realsed
			 ** this function can call many times, because at this function has check the device
			 **/
			JPEG_HDEC_CloseDev(cinfo);
			JPEG_HDEC_Destroy(cinfo);
		}
		else
		{
			JPEG_HENC_CloseDev(cinfo);
			JPGE_HENC_DInit(cinfo);
		}
#endif
}
