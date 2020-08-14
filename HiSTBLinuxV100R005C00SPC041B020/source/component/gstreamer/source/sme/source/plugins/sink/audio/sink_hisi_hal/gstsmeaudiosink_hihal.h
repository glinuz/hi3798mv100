#ifndef __GST_SME_AUDIOSINK_HIHAL_H__
#define __GST_SME_AUDIOSINK_HIHAL_H__

#include "gstsmeaudiosink2.h"

G_BEGIN_DECLS

//#define GST_SME_SINK_PASS_THROUGH    (1)

#define GST_TYPE_SME_AUDIOSINK_HIHAL (gst_smeaudiosink_hihal_get_type())
#define GST_SME_AUDIOSINK_HIHAL(obj)                                         \
    (G_TYPE_CHECK_INSTANCE_CAST(                                        \
        (obj),GST_TYPE_SME_AUDIOSINK_HIHAL, GstSmeAudioSinkHiHal))
#define GST_SME_AUDIOSINK_HIHAL_CLASS(klass)                                 \
    (G_TYPE_CHECK_CLASS_CAST(                                           \
        (klass),GST_TYPE_SME_AUDIOSINK_HIHAL, GstSmeAudioSinkHiHalClass))
#define GST_SME_AUDIOSINK_HIHAL_GET_CLASS(obj)                               \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                                  \
                                GST_TYPE_SME_AUDIOSINK_HIHAL,                \
                                GstSmeAudioSinkHiHalClass))
#define GST_IS_SME_AUDIOSINK_HIHAL(obj)                          \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_AUDIOSINK_HIHAL))
#define GST_IS_SME_AUDIOSINK_HIHAL_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_AUDIOSINK_HIHAL))
#define GST_SME_AUDIOSINK_HIHAL_CAST(obj)      \
    ((GstSmeAudioSinkHiHal*) (obj))

typedef struct _GstSmeAudioSinkHiHal GstSmeAudioSinkHiHal;
typedef struct _GstSmeAudioSinkHiHalClass GstSmeAudioSinkHiHalClass;


/**
 * GstSmeAudioSinkHiHal:
 *
 * sme audio sink element based on hisi hal api
 */
struct _GstSmeAudioSinkHiHal
{
    GstSmeAudiosink2 element;

    void *aout_handle;
};

struct _GstSmeAudioSinkHiHalClass
{
    GstSmeAudiosink2Class parent_class;
};

GType gst_sme_audiosink_hihal_get_type (void);

G_END_DECLS

#endif /* end of __GST_SME_AUDIOSINK_HIHAL_H__ */
