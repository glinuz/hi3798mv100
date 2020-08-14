#ifndef __GST_OMX_DDP_DEC_H__
#define __GST_OMX_DDP_DEC_H__

#include <gst/gst.h>
#include "gstomxaudiodec.h"

#if defined GST_OMX_DDP_DEC_ENABLE

//#define GST_OMX_DDP_DEC_REQ_PAD_ENAEBL

G_BEGIN_DECLS
#define GST_TYPE_OMX_DDP_DEC                    \
  (gst_omx_ddp_dec_get_type())
#define GST_OMX_DDP_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_DDP_DEC,GstOMXDDPDec))
#define GST_OMX_DDP_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_DDP_DEC,GstOMXDDPDecClass))
#define GST_OMX_DDP_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_DDP_DEC,GstOMXDDPDecClass))
#define GST_IS_OMX_DDP_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_DDP_DEC))
#define GST_IS_OMX_DDP_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_DDP_DEC))

typedef struct _GstOMXDDPDec GstOMXDDPDec;
typedef struct _GstOMXDDPDecClass GstOMXDDPDecClass;

struct _GstOMXDDPDec
{
  GstOMXAudioDec parent;
  gint spf;

#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
  gboolean bitstream_output_enable;
#endif
};

struct _GstOMXDDPDecClass
{
  GstOMXAudioDecClass parent_class;
};

GType gst_omx_ddp_dec_get_type (void);

G_END_DECLS

#endif

#endif
