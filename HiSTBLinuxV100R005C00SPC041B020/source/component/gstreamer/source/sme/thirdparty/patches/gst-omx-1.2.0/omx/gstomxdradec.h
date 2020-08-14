#ifndef __GST_OMX_DRA_DEC_H__
#define __GST_OMX_DRA_DEC_H__

#include <gst/gst.h>
#include "gstomxaudiodec.h"

#if defined GST_OMX_DRA_DEC_ENABLE

G_BEGIN_DECLS
#define GST_TYPE_OMX_DRA_DEC                    \
  (gst_omx_dra_dec_get_type())
#define GST_OMX_DRA_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_DRA_DEC,GstOMXDRADec))
#define GST_OMX_DRA_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_DRA_DEC,GstOMXDRADecClass))
#define GST_OMX_DRA_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_DRA_DEC,GstOMXDRADecClass))
#define GST_IS_OMX_DRA_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_DRA_DEC))
#define GST_IS_OMX_DRA_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_DRA_DEC))

typedef struct _GstOMXDRADec GstOMXDRADec;
typedef struct _GstOMXDRADecClass GstOMXDRADecClass;

struct _GstOMXDRADec
{
  GstOMXAudioDec parent;
  gint spf;
};

struct _GstOMXDRADecClass
{
  GstOMXAudioDecClass parent_class;
};

GType gst_omx_dra_dec_get_type (void);

G_END_DECLS

#endif

#endif
