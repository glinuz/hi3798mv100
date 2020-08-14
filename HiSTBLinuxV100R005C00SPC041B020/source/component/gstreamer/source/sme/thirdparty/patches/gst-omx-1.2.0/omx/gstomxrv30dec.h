#ifndef __GST_OMX_RV30_DEC_H__
#define __GST_OMX_RV30_DEC_H__

#include <gst/gst.h>
#include "gstomxvideodec.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_RV30_DEC \
  (gst_omx_rv30_dec_get_type())
#define GST_OMX_RV30_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_RV30_DEC,GstOMXRV30Dec))
#define GST_OMX_RV30_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_RV30_DEC,GstOMXRV30DecClass))
#define GST_OMX_RV30_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_RV30_DEC,GstOMXRV30DecClass))
#define GST_IS_OMX_RV30_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_RV30_DEC))
#define GST_IS_OMX_RV30_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_RV30_DEC))

typedef struct _GstOMXRV30Dec GstOMXRV30Dec;
typedef struct _GstOMXRV30DecClass GstOMXRV30DecClass;

struct _GstOMXRV30Dec
{
  GstOMXVideoDec parent;
};

struct _GstOMXRV30DecClass
{
  GstOMXVideoDecClass parent_class;
};

GType gst_omx_rv30_dec_get_type (void);

G_END_DECLS

#endif /* __GST_OMX_RV30_DEC_H__ */
