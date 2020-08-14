/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : gstsmevideosink_hivo_allocator.h
  版 本 号   : 初稿
  作    者   : liurongliang(l00180035)
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : video sink & vdec 共享内存分配器
  函数列表   :
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : liurongliang(l00180035)
    修改内容   : 创建文件

******************************************************************************/
#ifndef __GSTSMEVIDEOSINK_HIHAL_ALLCATOR_H__
#define __GSTSMEVIDEOSINK_HIHAL_ALLCATOR_H__

#include <gst/gstallocator.h>
#include "gstsmevideosink_hihal.h"
#include <hi_common.h>
#include "osal_mem.h"

typedef struct
{
  GstMemory mem;
  gsize slice_size;//sizeof(GstMemoryHiVOMMZ) + data size
  guint8 *data;//ture frame memory
  HI_MMZ_BUF_S stMmzBuf;//for free MMZ
  gpointer user_data;
  GDestroyNotify notify;

} GstMemoryHiVOMMZ;


#define GST_TYPE_ALLOCATOR_HIVOMMZ            (gst_allocator_sme_hivommz_get_type())
#define GST_IS_ALLOCATOR_HIVOMMZ(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_ALLOCATOR_HIVOMMZ))
#define GST_IS_ALLOCATOR_HIVOMMZ_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_ALLOCATOR_HIVOMMZ))
#define GST_ALLOCATOR_HIVOMMZ_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_ALLOCATOR_HIVOMMZ, GstAllocatorHiVOMMZClass))
#define GST_ALLOCATOR_HIVOMMZ(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_ALLOCATOR_HIVOMMZ, GstAllocatorHiVOMMZ))
#define GST_ALLOCATOR_HIVOMMZ_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_ALLOCATOR_HIVOMMZ, GstAllocatorHiVOMMZClass))
#define GST_ALLOCATOR_HIVOMMZ_CAST(obj)       ((GstAllocatorHiVOMMZ*)(obj))


typedef struct _GstAllocatorHiVOMMZ GstAllocatorHiVOMMZ;
typedef struct _GstAllocatorHiVOMMZClass GstAllocatorHiVOMMZClass;

//typedef struct _GstSmeVideoSinkHiVO GstSmeVideoSinkHiVO;
struct _GstAllocatorHiVOMMZ
{
  GstAllocator parent;
  GstSmeVideoSinkHiHal* vsink;//vsink element指针
};

struct _GstAllocatorHiVOMMZClass {
  GstAllocatorClass p_class;
};

//@vsink:video sink element 指针
//return: GstAllocatorHiVOMMZ对象指针
GstAllocator* gst_allcator_sme_hivommz_new(GstSmeVideoSinkHiHal* vsink);

#endif
