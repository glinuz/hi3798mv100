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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstsmeaudiosinkclock.h"

//lint -e826
//lint -e717

static void gst_smeaudiosink_clock_class_init (GstSmeAudioSinkClockClass* klass);
static void gst_smeaudiosink_clock_init (GstSmeAudioSinkClock* clock);

static GstClockTime gst_smeaudiosink_clock_get_internal_time (GstClock* clock);

GType
gst_smeaudiosink_clock_get_type (void)
{
    static GType clock_type = 0;

    if (!clock_type)
    {
        static const GTypeInfo clock_info =
        {
            sizeof (GstSmeAudioSinkClockClass),
            NULL,
            NULL,
            (GClassInitFunc) gst_smeaudiosink_clock_class_init,
            NULL,
            NULL,
            sizeof (GstSmeAudioSinkClock),
            4,
            (GInstanceInitFunc) gst_smeaudiosink_clock_init,
            NULL
        };

        clock_type = g_type_register_static (GST_TYPE_SYSTEM_CLOCK,
          "GstSmeAudioSinkClock", &clock_info, (GTypeFlags)0);
    }

    return clock_type;
}


static void
gst_smeaudiosink_clock_class_init (GstSmeAudioSinkClockClass* klass)
{
    GstClockClass* gstclock_class;

    gstclock_class = GST_CLOCK_CLASS(klass);

    gstclock_class->get_internal_time = gst_smeaudiosink_clock_get_internal_time;
}

static void
gst_smeaudiosink_clock_init (GstSmeAudioSinkClock* audio_clock)
{
    GST_WARNING_OBJECT(audio_clock, "gst_smeaudiosink_clock_init in...");

    GST_WARNING_OBJECT(audio_clock, "gst_smeaudiosink_clock_init out");
}

GstClock*
gst_smeaudiosink_clock_new (const gchar* name, GstSmeAudioSinkClockGetTimeFunc func,
                        gpointer user_data)
{
    GstSmeAudioSinkClock* audiosink_clock =
        GST_SMEAUDIOSINK_CLOCK (g_object_new (GST_TYPE_SMEAUDIOSINK_CLOCK, NULL));

    (void)name;

    GST_WARNING_OBJECT(audiosink_clock, "gst_smeaudiosink_clock_new in audiosink_clock=%p, func=%p",
                    audiosink_clock, func);

    if (audiosink_clock)
    {
        audiosink_clock->func = func;
        audiosink_clock->user_data = user_data;
    }
    else
    {
        GST_ERROR_OBJECT(audiosink_clock, "gst_smeaudiosink_clock_new failed null !");
    }

    GST_WARNING_OBJECT(audiosink_clock, "gst_smeaudiosink_clock_new out");

    return GST_CLOCK (audiosink_clock);
}

static GstClockTime
gst_smeaudiosink_clock_get_internal_time (GstClock* audio_clock)
{
    GstSmeAudioSinkClock* audiosink_clock = GST_SMEAUDIOSINK_CLOCK (audio_clock);

    return audiosink_clock->func (audio_clock, audiosink_clock->user_data);
}

static GstClockTime
gst_smeaudiosink_clock_func_invalid(GstClock * self, gpointer user_data)
{
    (void)self;
    (void)user_data;
    return GST_CLOCK_TIME_NONE;
}

void
gst_smeaudiosink_clock_invalidate(GstClock* self)
{
    GstSmeAudioSinkClock *audiosink_clock;
    audiosink_clock = GST_SMEAUDIOSINK_CLOCK (self);
    audiosink_clock->func = gst_smeaudiosink_clock_func_invalid;
}

