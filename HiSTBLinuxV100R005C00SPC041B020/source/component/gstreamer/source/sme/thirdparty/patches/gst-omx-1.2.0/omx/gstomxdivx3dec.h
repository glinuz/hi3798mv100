#ifndef __GST_OMX_DIVX3_DEC_H__
#define __GST_OMX_DIVX3_DEC_H__

#include <gst/gst.h>
#include "gstomxvideodec.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_DIVX3_DEC \
  (gst_omx_divx3_dec_get_type())
#define GST_OMX_DIVX3_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_DIVX3_DEC,GstOMXDIVX3Dec))
#define GST_OMX_DIVX3_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_DIVX3_DEC,GstOMXDIVX3DecClass))
#define GST_OMX_DIVX3_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_DIVX3_DEC,GstOMXDIVX3DecClass))
#define GST_IS_OMX_DIVX3_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_DIVX3_DEC))
#define GST_IS_OMX_DIVX3_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_DIVX3_DEC))

typedef struct _GstOMXDIVX3Dec GstOMXDIVX3Dec;
typedef struct _GstOMXDIVX3DecClass GstOMXDIVX3DecClass;

struct _GstOMXDIVX3Dec
{
  GstOMXVideoDec parent;
};

struct _GstOMXDIVX3DecClass
{
  GstOMXVideoDecClass parent_class;
};

GType gst_omx_divx3_dec_get_type (void);

G_END_DECLS

#endif /* __GST_OMX_DIVX3_DEC_H__ */
