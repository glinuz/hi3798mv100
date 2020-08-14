#ifndef __GST_OMX_VIDEO_BUFFER_POOL_HIVO_H__
#define __GST_OMX_VIDEO_BUFFER_POOL_HIVO_H__

#include "gstomxbufferpool.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_VIDEO_BUFFER_POOL_HIVO \
  (gst_omx_video_buffer_pool_hivo_get_type())
#define GST_OMX_VIDEO_BUFFER_POOL_HIVO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_VIDEO_BUFFER_POOL_HIVO, GstOMXVideoBufferPoolHiVO))
#define GST_IS_OMX_VIDEO_BUFFER_POOL_HIVO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_VIDEO_BUFFER_POOL_HIVO))

#ifndef GST_HIMMZ_MEMORY
#define GST_HIMMZ_MEMORY
#endif

typedef struct _GstOMXVideoBufferPoolHiVO GstOMXVideoBufferPoolHiVO;
typedef struct _GstOMXVideoBufferPoolHiVOClass GstOMXVideoBufferPoolHiVOClass;

typedef struct _GstVideoFrameMap
{
    gpointer pvAddr;
    GstBuffer* gstvbuf;
}GstVideoFrameMap;

struct _GstOMXVideoBufferPoolHiVO
{
  GstVideoBufferPool parent;

  GstElement *element; //vdec element

  GstCaps *caps; //caps
  gboolean add_videometa;
  GstVideoInfo video_info;

  /* Owned by element, element has to stop this pool before
   * it destroys component or port */
  GstOMXComponent *component;//omx component
  GstOMXPort *port; //omx port

  /* Set from outside this pool */
  /* TRUE if we're currently allocating all our buffers */
  gboolean allocating;
  /* TRUE if the pool is not used anymore */
  gboolean deactivated;

  GPtrArray *buffers;//all buffer array

  /* For allocated frame memory */
  GstAllocator* frame_allocator;//query from vsink
  gpointer cur_frame_addr;//frame buffer addr, allcator by frame_allocator

  /* store real data for USE_OMX*/
  GList *real_data;

  /* for find gstbuf from gstomxbuf */
  guint cur_frame_index;
};

struct _GstOMXVideoBufferPoolHiVOClass
{
  GstVideoBufferPoolClass parent_class;
};

GType gst_omx_video_buffer_pool_hivo_get_type (void);

GstBufferPool *gst_omx_video_buffer_pool_hivo_new (GstElement * element,
  GstOMXComponent * component, GstOMXPort * port, GstAllocator* frame_allocator);

GList * gst_omx_video_buffer_pool_hivo_get_real_data(GstBufferPool * bpool);

G_END_DECLS

#endif /* __GST_OMX_VIDEO_BUFFER_POOL_HIVO_H__ */
