/******************************************************************************
<Huawei Tec.>
Copyright (c) 2016 - 2020 Huawei Software, Inc. All rights reserved.
All software, firmware and related documentation herein ("Huawei Tec.") are
intellectual property of Huawei Tec. and protected by
law, including, but not limited to, copyright law and international treaties.
Any use, modification, reproduction, retransmission, or republication of all
or part of Huawei Tec. is expressly prohibited, unless prior written
permission has been granted by Huawei Tec.
*******************************************************************************/

#ifndef __GST_SMEAUDIOSINK_CLOCK_H__
#define __GST_SMEAUDIOSINK_CLOCK_H__

#include <gst/gst.h>


G_BEGIN_DECLS

#define GST_TYPE_SMEAUDIOSINK_CLOCK \
    (gst_smeaudiosink_clock_get_type())
#define GST_SMEAUDIOSINK_CLOCK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SMEAUDIOSINK_CLOCK,GstSmeAudioSinkClock))
#define GST_SMEAUDIOSINK_CLOCK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SMEAUDIOSINK_CLOCK,GstSmeAudioSinkClockClass))
#define GST_IS_SMEAUDIOSINK_CLOCK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SMEAUDIOSINK_CLOCK))
#define GST_IS_SMEAUDIOSINK_CLOCK_CLASS(obj) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_SMEAUDIOSINK_CLOCK))

typedef struct _GstSmeAudioSinkClock GstSmeAudioSinkClock;
typedef struct _GstSmeAudioSinkClockClass GstSmeAudioSinkClockClass;

typedef GstClockTime (*GstSmeAudioSinkClockGetTimeFunc) (GstClock* clock,
        gpointer user_data);

struct _GstSmeAudioSinkClock
{
    GstSystemClock clock;
    GstSmeAudioSinkClockGetTimeFunc func;
    gpointer user_data;
};

struct _GstSmeAudioSinkClockClass
{
    GstSystemClockClass parent_class;
};

GType gst_smeaudiosink_clock_get_type(void);
GstClock* gst_smeaudiosink_clock_new(const gchar* name,
                                 GstSmeAudioSinkClockGetTimeFunc func,
                                 gpointer user_data);

void
gst_smeaudiosink_clock_invalidate(GstClock* self);

G_END_DECLS

#endif /* __GST_SMEAUDIOSINK_CLOCK_H__ */
