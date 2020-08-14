#ifndef __GST_OMX_MPEG1_DEC_H__
#define __GST_OMX_MPEG1_DEC_H__

#include <gst/gst.h>
#include "gstomxvideodec.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_MPEG1_DEC \
  (gst_omx_mpeg1_dec_get_type())
#define GST_OMX_MPEG1_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_MPEG1_DEC,GstOMXMPEG1Dec))
#define GST_OMX_MPEG1_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_MPEG1_DEC,GstOMXMPEG1DecClass))
#define GST_OMX_MPEG1_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_MPEG1_DEC,GstOMXMPEG1DecClass))
#define GST_IS_MPEG1_AVS_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_MPEG1_DEC))
#define GST_IS_OMX_MPEG1_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_MPEG1_DEC))

typedef struct _GstOMXMPEG1Dec GstOMXMPEG1Dec;
typedef struct _GstOMXMPEG1DecClass GstOMXMPEG1DecClass;

struct _GstOMXMPEG1Dec
{
  GstOMXVideoDec parent;
};

struct _GstOMXMPEG1DecClass
{
  GstOMXVideoDecClass parent_class;
};

GType gst_omx_mpeg1_dec_get_type (void);

G_END_DECLS

#endif /* __GST_OMX_MPEG1_DEC_H__ */
