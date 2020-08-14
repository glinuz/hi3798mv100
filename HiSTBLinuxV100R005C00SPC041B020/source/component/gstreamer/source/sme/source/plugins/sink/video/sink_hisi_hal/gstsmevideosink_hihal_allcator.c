//lint -e585
//lint -e717 //do..while
//lint -e838
//lint -e655
//lint -e438 //some Parameter of func not used
//lint -e826
//lint -e528
//lint -e551


#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include "gstsmevideosink_hihal_allcator.h"

#define GST_ALLOCATOR_HIVOMMZ_NAME "GstAllocatorHiVOMMZ"
extern GList* gToBeFree;

GType gst_allocator_sme_hivommz_get_type (void);
//lint -e19
G_DEFINE_TYPE (GstAllocatorHiVOMMZ, gst_allocator_sme_hivommz, GST_TYPE_ALLOCATOR);
//lint +e19
#define parent_class gst_allocator_sme_hivommz_parent_class


static inline void
_hivommz_init (GstAllocatorHiVOMMZ * allocator, HI_MMZ_BUF_S stMmzBuf, GstMemoryHiVOMMZ * mem,
    GstMemoryFlags flags,GstMemory * parent, gsize slice_size,
    gpointer data, gsize maxsize, gsize align, gsize offset, gsize size,
    gpointer user_data, GDestroyNotify notify)
{
  gst_memory_init (GST_MEMORY_CAST (mem),
      flags, GST_ALLOCATOR_CAST(allocator), parent, maxsize, align, offset, size);
  mem->slice_size = slice_size;
  mem->data = data;
  mem->stMmzBuf = stMmzBuf;
  mem->user_data = user_data;
  mem->notify = notify;
}

//lint -e715
static GstMemoryHiVOMMZ *
_hivommz_new_block (GstAllocatorHiVOMMZ * allocator, GstMemoryFlags flags,
    gsize maxsize, gsize align, gsize offset, gsize size)
{
  GstMemoryHiVOMMZ * hivommz_mem = NULL;
  gsize slice_size;
  HI_S32 l_return;
  HI_MMZ_BUF_S stMmzBuf;
  guint8 *data;
  static gsize g_buf_num = 0;
  E_SME_MEM_SECURE_RET eMemSecRet;
  V_INT32 flag_snp;

  slice_size = sizeof (GstMemoryHiVOMMZ) + (gsize)(maxsize * 1.5);
  hivommz_mem = (GstMemoryHiVOMMZ *)g_malloc(sizeof(GstMemoryHiVOMMZ));
  if(hivommz_mem == NULL)
  {
      GST_ERROR("malloc hivommz_mem failed");
      return NULL;
  }

  eMemSecRet = VOS_Memset_S(&stMmzBuf, sizeof(HI_MMZ_BUF_S), 0x0, sizeof(HI_MMZ_BUF_S));
  if(eMemSecRet != E_SME_MEM_OK){
      g_free(hivommz_mem);
      GST_ERROR("init stMmzBuf failed,ret:%d", eMemSecRet);
      return NULL;
  }

  stMmzBuf.bufsize = (gsize)(maxsize * 1.5);
//lint -e119
  flag_snp = VOS_Snprintf_S(stMmzBuf.bufname, sizeof(stMmzBuf.bufname) - 1, sizeof(stMmzBuf.bufname) - 1, "OMXVDEC_OUT%d", g_buf_num++);
  if(flag_snp == 0){
      g_free(hivommz_mem);
      GST_ERROR("init stMmzBuf.bufname failed");
      return NULL;
  }
//lint +e119

  l_return = HI_MMZ_Malloc(&stMmzBuf);

  GST_INFO("PHY ADDR IS %#x", stMmzBuf.phyaddr);
  GST_INFO("KERNEL ADDR IS %p", stMmzBuf.kernel_viraddr);
  GST_INFO("VIRTUAL ADDR IS %p", stMmzBuf.user_viraddr);
  GST_INFO("BUFNAME IS %s", stMmzBuf.bufname);

  if(l_return == HI_SUCCESS){
    data = (guint8 *) stMmzBuf.user_viraddr;
    GST_INFO("Allocate GstMemoryHiVOMMZ SUCCEED");
  }else{
    g_free(hivommz_mem);
    GST_ERROR("Allocate GstMemoryHiVOMMZ failed");
    return NULL;
  }

  /* init GstMMZVOQueue */
  GstMmzInfo* item = g_malloc(sizeof(GstMmzInfo));
  item->mmz_is_vo = FALSE;
  item->pVirAddr = (void*)data;
  item->pPhyAddr = (void*)stMmzBuf.phyaddr;
  item->stMmzBuf = stMmzBuf;

  g_mutex_lock(&allocator->vsink->mmz_is_vo_muxtex);
  g_queue_push_tail(&allocator->vsink->q_mmz_is_vo, item);
  g_mutex_unlock(&allocator->vsink->mmz_is_vo_muxtex);

  _hivommz_init(allocator, stMmzBuf, hivommz_mem, flags, NULL, slice_size, data,
    0, align, offset, (unsigned int)(maxsize * 1.5), NULL, NULL);

  return hivommz_mem;

}
//lint +e715

//lint -e818
static GstMemory *
gst_allocator_sme_hivommz_alloc (GstAllocator * allocator, gsize size,
GstAllocationParams * params)
{
  //sme_hivo_warp 中实现对HiVOMMZBuf的管理。

  gsize maxsize = size + params->prefix + params->padding;
  return  (GstMemory *)_hivommz_new_block ((GstAllocatorHiVOMMZ*)allocator,
    GST_MEMORY_FLAG_ZERO_PREFIXED | GST_MEMORY_FLAG_ZERO_PADDED, maxsize, params->align, params->prefix, size);

}
//lint +e818



static void
gst_allocator_sme_hivommz_free (GstAllocator * alloc, GstMemory * mem)
{
  //todo
  GstMemoryHiVOMMZ *hivo_mem = (GstMemoryHiVOMMZ *) mem;
  GstAllocatorHiVOMMZ *allocator = (GstAllocatorHiVOMMZ *)alloc;
  guint idx = 0;
  guint ilen = 0;
  gsize data_size;

  if(!hivo_mem)
    return;

  data_size = hivo_mem->slice_size;

  if (hivo_mem->notify)
    hivo_mem->notify (hivo_mem->user_data);

  g_mutex_lock(&allocator->vsink->mmz_is_vo_muxtex);

  ilen = g_queue_get_length(&allocator->vsink->q_mmz_is_vo);
  for(idx = 0; idx < ilen; idx++){
    GstMmzInfo* item = g_queue_peek_nth(&allocator->vsink->q_mmz_is_vo, idx);
    if(!item){
        GST_ERROR("peek GstVideoMmzVoFlag %d:is null, why?", idx);
        continue;
    }
    if(item->pVirAddr == hivo_mem->data){
      if(!item->mmz_is_vo){
        GST_INFO("FREE PHY ADDR IS %#x", hivo_mem->stMmzBuf.phyaddr);
        GST_INFO("VIRTUAL ADDR IS %p", hivo_mem->stMmzBuf.user_viraddr);
        GST_INFO("FREE BUFNAME IS %s", hivo_mem->stMmzBuf.bufname);
        HI_S32 l_return = HI_FAILURE;
        l_return = HI_MMZ_Free(&hivo_mem->stMmzBuf);
        if(l_return == HI_FAILURE)
          GST_WARNING("FREE MMZ FAILED");
        else if(l_return == HI_SUCCESS)
          GST_INFO("FREE MMZ SUCCEED");
        g_free(item);
      } else {
        gToBeFree = g_list_append(gToBeFree, item);
        GST_INFO("gToBeFree len is %d", g_list_length(gToBeFree));
        GST_INFO("TOBEFREE IS  %p", item->pVirAddr);
      }
      g_queue_pop_nth(&allocator->vsink->q_mmz_is_vo, idx);
      break;
    }
  }
  g_mutex_unlock(&allocator->vsink->mmz_is_vo_muxtex);

  hivo_mem->data = NULL;
  g_free(hivo_mem);

  GST_INFO("FREE SIZE: %d", data_size);

  return;
}

static void
gst_allocator_sme_hivommz_finalize (GObject * obj)
{
  GstAllocatorHiVOMMZ* allocator_hivommz = GST_ALLOCATOR_HIVOMMZ(obj);
  gst_object_unref(allocator_hivommz->vsink);
  G_OBJECT_CLASS(parent_class)->finalize (obj);
  return;
}

//lint -e64
static void
gst_allocator_sme_hivommz_class_init (GstAllocatorHiVOMMZClass * klass)
{
  GObjectClass *gobject_class;
  GstAllocatorClass *allocator_class;

  gobject_class = (GObjectClass *) klass;
  allocator_class = (GstAllocatorClass *) klass;
  gobject_class->finalize = gst_allocator_sme_hivommz_finalize;
  allocator_class->alloc = gst_allocator_sme_hivommz_alloc;
  allocator_class->free = gst_allocator_sme_hivommz_free;
  return;

}
//lint +e64

static gpointer
gst_memory_sme_hivommz_map (const GstMemoryHiVOMMZ * mem)
{
  return mem->data;
}

//lint -e715
static void
gst_memory_sme_hivommz_unmap (const GstMemoryHiVOMMZ * mem)
{
  return ;
}
//lint +e715

static void
gst_allocator_sme_hivommz_init  (GstAllocatorHiVOMMZ * allocator)
{
  GstAllocator *alloc = GST_ALLOCATOR_CAST (allocator);

  alloc->mem_type = GST_ALLOCATOR_HIVOMMZ_NAME;
  alloc->mem_map = (GstMemoryMapFunction) gst_memory_sme_hivommz_map;
  alloc->mem_unmap = (GstMemoryUnmapFunction) gst_memory_sme_hivommz_unmap;
  alloc->mem_copy = NULL;//(GstMemoryCopyFunction) gst_memory_sme_hivommz_copy;
  alloc->mem_share = NULL;//(GstMemoryShareFunction) gst_memory_sme_hivommz_share;
  alloc->mem_is_span = NULL;//(GstMemoryIsSpanFunction) gst_memory_sme_hivommz_is_span;
  return;

}


GstAllocator* gst_allcator_sme_hivommz_new(GstSmeVideoSinkHiHal* vsink)
{
  GstAllocatorHiVOMMZ* allocator_hivommz = NULL;

  allocator_hivommz = g_object_new (gst_allocator_sme_hivommz_get_type(), NULL);
  allocator_hivommz->vsink = gst_object_ref (vsink);
  return GST_ALLOCATOR(allocator_hivommz);

}
