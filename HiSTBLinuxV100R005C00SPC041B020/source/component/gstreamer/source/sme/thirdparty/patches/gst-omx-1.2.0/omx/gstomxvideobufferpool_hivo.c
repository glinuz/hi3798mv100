#include "gstomxvideobufferpool_hivo.h"

G_DEFINE_TYPE (GstOMXVideoBufferPoolHiVO, gst_omx_video_buffer_pool_hivo,
    GST_TYPE_VIDEO_BUFFER_POOL);

static GstFlowReturn
gst_omx_video_buffer_pool_hivo_alloc_buffer (GstBufferPool * bpool,
    GstBuffer ** buffer, GstBufferPoolAcquireParams * params)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);
  GstBuffer *buf;

  g_return_val_if_fail (pool->allocating, GST_FLOW_ERROR);
{
    GstMemory *mem;
    GstMapInfo mapinfo;
    const guint nstride = pool->port->port_def.format.video.nStride;
    const guint nslice = pool->port->port_def.format.video.nSliceHeight;
    gsize offset[GST_VIDEO_MAX_PLANES] = { 0, };
    gint stride[GST_VIDEO_MAX_PLANES] = { nstride, 0, };
    gsize data_size = 0;
    guint video_stride = 0;
    video_stride = ((pool->video_info.width + 15) & ~15);
    data_size = video_stride * pool->video_info.height;
    GST_DEBUG("allocate outbuffer,stride:%d,width:%d,height:%d,data_size:%d", video_stride,
        pool->video_info.width, pool->video_info.height, data_size);

    mem = gst_allocator_alloc(pool->frame_allocator, data_size, NULL);
    buf = gst_buffer_new ();
    gst_buffer_append_memory (buf, mem);
    g_ptr_array_add (pool->buffers, buf);
    /* put omx_buf and gst_buf together */
    if(!gst_memory_map (mem, &mapinfo, GST_MEMORY_FLAG_LAST)){
        GST_WARNING("mem map failed");
        return GST_FLOW_ERROR;
    }

    pool->real_data = g_list_append(pool->real_data, mapinfo.data);
    gst_memory_unmap(mem, &mapinfo);

    switch (GST_VIDEO_INFO_FORMAT (&pool->video_info)) {
      case GST_VIDEO_FORMAT_ABGR:
      case GST_VIDEO_FORMAT_ARGB:
      case GST_VIDEO_FORMAT_RGB16:
      case GST_VIDEO_FORMAT_BGR16:
      case GST_VIDEO_FORMAT_YUY2:
      case GST_VIDEO_FORMAT_UYVY:
      case GST_VIDEO_FORMAT_YVYU:
      case GST_VIDEO_FORMAT_GRAY8:
        break;
      case GST_VIDEO_FORMAT_I420:
        stride[1] = nstride / 2;
        offset[1] = offset[0] + stride[0] * nslice;
        stride[2] = nstride / 2;
        offset[2] = offset[1] + (stride[1] * nslice / 2);
        break;
      case GST_VIDEO_FORMAT_NV12:
      case GST_VIDEO_FORMAT_NV16:
        stride[1] = nstride;
        offset[1] = offset[0] + stride[0] * nslice;
        break;
      default:
        g_assert_not_reached ();
        break;
    }



  *buffer = buf;

  if (pool->add_videometa) {
    /* We always add the videometa. It's the job of the user
     * to copy the buffer if pool->need_copy is TRUE
     */
    gst_buffer_add_video_meta_full (buf, GST_VIDEO_FRAME_FLAG_NONE,
        GST_VIDEO_INFO_FORMAT (&pool->video_info),
        GST_VIDEO_INFO_WIDTH (&pool->video_info),
        GST_VIDEO_INFO_HEIGHT (&pool->video_info),
        GST_VIDEO_INFO_N_PLANES (&pool->video_info), offset, stride);
  }

}

  return GST_FLOW_OK;

}

static GstFlowReturn
gst_omx_video_buffer_pool_hivo_acquire_buffer (GstBufferPool * bpool,
    GstBuffer ** buffer, GstBufferPoolAcquireParams * params)
{

  GstFlowReturn ret;


  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);

  if (pool->port->port_def.eDir == OMX_DirOutput) {

    *buffer = g_ptr_array_index (pool->buffers, pool->cur_frame_index);
    g_return_val_if_fail (*buffer != NULL, GST_FLOW_ERROR);
    ret = GST_FLOW_OK;
  } else {
    //not support now
    GST_WARNING("Not support now");
  }

  return ret;
}


static void
gst_omx_video_buffer_pool_hivo_finalize (GObject * object)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (object);

  if (pool->element)
    gst_object_unref (pool->element);
  pool->element = NULL;

  if (pool->frame_allocator)
    gst_object_unref (pool->frame_allocator);
  pool->frame_allocator = NULL;

  if(pool->real_data)
    g_list_free(pool->real_data);
  pool->real_data = NULL;

  if (pool->buffers)
    g_ptr_array_unref (pool->buffers);
  pool->buffers = NULL;

  G_OBJECT_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->finalize (object);
}

static gboolean
gst_omx_video_buffer_pool_hivo_start (GstBufferPool * bpool)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);

  /* Only allow to start the pool if we still are attached
   * to a component and port */
  GST_OBJECT_LOCK (pool);
  if (!pool->component || !pool->port) {
    GST_OBJECT_UNLOCK (pool);
    return FALSE;
  }
  GST_OBJECT_UNLOCK (pool);

  return
      GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->start (bpool);
}

static gboolean
gst_omx_video_buffer_pool_hivo_stop (GstBufferPool * bpool)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);
  gint i = 0;

  /* When not using the default GstBufferPool::GstAtomicQueue then
   * GstBufferPool::free_buffer is not called while stopping the pool
   * (because the queue is empty) */
  for (i = 0; i < pool->buffers->len; i++)
    GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->release_buffer
        (bpool, g_ptr_array_index (pool->buffers, i));

  /* Remove any buffers that are there */
  g_ptr_array_set_size (pool->buffers, 0);

  if (pool->caps)
    gst_caps_unref (pool->caps);
  pool->caps = NULL;

  pool->add_videometa = FALSE;

  return GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->stop (bpool);
}

static const gchar **
gst_omx_video_buffer_pool_hivo_get_options (GstBufferPool * bpool)
{
  static const gchar *raw_video_options[] =
      { GST_BUFFER_POOL_OPTION_VIDEO_META, NULL };
  static const gchar *options[] = { NULL };
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);

  GST_OBJECT_LOCK (pool);
  if (pool->port && pool->port->port_def.eDomain == OMX_PortDomainVideo
      && pool->port->port_def.format.video.eCompressionFormat ==
      OMX_VIDEO_CodingUnused) {
    GST_OBJECT_UNLOCK (pool);
    return raw_video_options;
  }
  GST_OBJECT_UNLOCK (pool);

  return options;
}

static gboolean
gst_omx_video_buffer_pool_hivo_set_config (GstBufferPool * bpool, GstStructure * config)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);
  GstCaps *caps;

  GST_OBJECT_LOCK (pool);

  if (!gst_buffer_pool_config_get_params (config, &caps, NULL, NULL, NULL))
    goto wrong_config;

  if (caps == NULL)
    goto no_caps;

  if (pool->port && pool->port->port_def.eDomain == OMX_PortDomainVideo
      && pool->port->port_def.format.video.eCompressionFormat ==
      OMX_VIDEO_CodingUnused) {
    GstVideoInfo info;

    /* now parse the caps from the config */
    if (!gst_video_info_from_caps (&info, caps))
      goto wrong_video_caps;

    /* enable metadata based on config of the pool */
    pool->add_videometa =
        gst_buffer_pool_config_has_option (config,
        GST_BUFFER_POOL_OPTION_VIDEO_META);

    pool->video_info = info;
  }

  if (pool->caps)
    gst_caps_unref (pool->caps);
  pool->caps = gst_caps_ref (caps);

  GST_OBJECT_UNLOCK (pool);

  return GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->set_config
      (bpool, config);

  /* ERRORS */
wrong_config:
  {
    GST_OBJECT_UNLOCK (pool);
    GST_WARNING_OBJECT (pool, "invalid config");
    return FALSE;
  }
no_caps:
  {
    GST_OBJECT_UNLOCK (pool);
    GST_WARNING_OBJECT (pool, "no caps in config");
    return FALSE;
  }
wrong_video_caps:
  {
    GST_OBJECT_UNLOCK (pool);
    GST_WARNING_OBJECT (pool,
        "failed getting geometry from caps %" GST_PTR_FORMAT, caps);
    return FALSE;
  }
}

static void
gst_omx_video_buffer_pool_hivo_free_buffer (GstBufferPool * bpool, GstBuffer * buffer)
{

  /* free HIMMZ memeory */

  GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->free_buffer (bpool,
      buffer);

  return ;
}

static void
gst_omx_video_buffer_pool_hivo_release_buffer (GstBufferPool * bpool, GstBuffer * buffer)
{

  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);
  OMX_ERRORTYPE err;
  GstOMXBuffer *omx_buf;
  guint ix;
  guint len;

  g_assert (pool->component && pool->port);

  if (!pool->allocating && !pool->deactivated) {

    len = pool->buffers->len;
    for(ix = 0; ix < len; ix++){
        if(buffer == g_ptr_array_index (pool->buffers, ix))
            break;
    }

    if(ix >= len){
        GST_ERROR("can not find omx_buf from gst_buf");
        return ;
    }

    omx_buf = g_ptr_array_index (pool->port->buffers, ix);

    if (pool->port->port_def.eDir == OMX_DirOutput && !omx_buf->used) {
      // Release back to the port, can be filled again
      err = gst_omx_port_release_buffer (pool->port, omx_buf);
      if (err != OMX_ErrorNone) {
        GST_ELEMENT_ERROR (pool->element, LIBRARY, SETTINGS, (NULL),
            ("Failed to relase output buffer to component: %s (0x%08x)",
                gst_omx_error_to_string (err), err));
      }
    } else if (!omx_buf->used) {

      g_assert_not_reached ();
      GST_BUFFER_POOL_CLASS (gst_omx_video_buffer_pool_hivo_parent_class)->release_buffer
          (bpool, buffer);
    }
  }


}


static void
gst_omx_video_buffer_pool_hivo_class_init (GstOMXVideoBufferPoolHiVOClass * klass)
{
  GObjectClass *gobject_class = (GObjectClass *) klass;
  GstBufferPoolClass *gstbufferpool_class = (GstBufferPoolClass *) klass;

  gobject_class->finalize = gst_omx_video_buffer_pool_hivo_finalize;
  gstbufferpool_class->start = gst_omx_video_buffer_pool_hivo_start;
  gstbufferpool_class->stop = gst_omx_video_buffer_pool_hivo_stop;
  gstbufferpool_class->get_options = gst_omx_video_buffer_pool_hivo_get_options;
  gstbufferpool_class->set_config = gst_omx_video_buffer_pool_hivo_set_config;
  gstbufferpool_class->alloc_buffer = gst_omx_video_buffer_pool_hivo_alloc_buffer;
  gstbufferpool_class->free_buffer = gst_omx_video_buffer_pool_hivo_free_buffer;
  gstbufferpool_class->acquire_buffer = gst_omx_video_buffer_pool_hivo_acquire_buffer;
  gstbufferpool_class->release_buffer = gst_omx_video_buffer_pool_hivo_release_buffer;
}

static void
gst_omx_video_buffer_pool_hivo_init (GstOMXVideoBufferPoolHiVO * pool)
{
  pool->buffers = g_ptr_array_new ();

  return;
}

GstBufferPool *
gst_omx_video_buffer_pool_hivo_new (GstElement * element, GstOMXComponent * component,
    GstOMXPort * port, GstAllocator* frame_allocator)
{
  GstOMXVideoBufferPoolHiVO *pool;

  pool = g_object_new (gst_omx_video_buffer_pool_hivo_get_type (), NULL);
  pool->frame_allocator = gst_object_ref(frame_allocator);
  pool->element = gst_object_ref (element);
  pool->component = component;
  pool->port = port;

  pool->real_data = g_list_alloc();
  pool->cur_frame_index = 0;
  //.....

  return GST_BUFFER_POOL (pool);
}

GList * gst_omx_video_buffer_pool_hivo_get_real_data(GstBufferPool * bpool)
{
  GstOMXVideoBufferPoolHiVO *pool = GST_OMX_VIDEO_BUFFER_POOL_HIVO (bpool);
  return pool->real_data;
}
