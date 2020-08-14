#ifndef __GST_OMX_AVS_DEC_H__
#define __GST_OMX_AVS_DEC_H__

#include <gst/gst.h>
#include "gstomxvideodec.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_AVS_DEC \
  (gst_omx_avs_dec_get_type())
#define GST_OMX_AVS_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_AVS_DEC,GstOMXAVSDec))
#define GST_OMX_AVS_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_AVS_DEC,GstOMXAVSDecClass))
#define GST_OMX_AVS_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_AVS_DEC,GstOMXAVSDecClass))
#define GST_IS_OMX_AVS_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_AVS_DEC))
#define GST_IS_OMX_AVS_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_AVS_DEC))

typedef struct _GstOMXAVSDec GstOMXAVSDec;
typedef struct _GstOMXAVSDecClass GstOMXAVSDecClass;

struct _GstOMXAVSDec
{
  GstOMXVideoDec parent;
};

struct _GstOMXAVSDecClass
{
  GstOMXVideoDecClass parent_class;
};

GType gst_omx_avs_dec_get_type (void);

G_END_DECLS

#endif /* __GST_OMX_AVS_DEC_H__ */
