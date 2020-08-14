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

#include "gstsmetsdmxclock.h"

static void gst_smetsdmx_clock_class_init (GstSmeTSDMXClockClass* klass);
static void gst_smetsdmx_clock_init (GstSmeTSDMXClock* clock);

static GstClockTime gst_smetsdmx_clock_get_internal_time (GstClock* clock);

//static GstSystemClockClass* parent_class = NULL;

//lint -e717
//lint -e826

GType
gst_smetsdmx_clock_get_type (void)
{
    static GType clock_type = 0;

    if (!clock_type)
    {
        static const GTypeInfo clock_info =
        {
            sizeof (GstSmeTSDMXClockClass),
            NULL,
            NULL,
            (GClassInitFunc) gst_smetsdmx_clock_class_init,
            NULL,
            NULL,
            sizeof (GstSmeTSDMXClock),
            4,
            (GInstanceInitFunc) gst_smetsdmx_clock_init,
            NULL
        };

        clock_type = g_type_register_static (GST_TYPE_SYSTEM_CLOCK,
                                             "GstSmeTSDMXClock", &clock_info,
                                             (GTypeFlags)0);
    }

    return clock_type;
}


static void
gst_smetsdmx_clock_class_init (GstSmeTSDMXClockClass* klass)
{
    GstClockClass* gstclock_class;

    gstclock_class = GST_CLOCK_CLASS(klass);

    //parent_class = g_type_class_peek_parent (klass);

    gstclock_class->get_internal_time = gst_smetsdmx_clock_get_internal_time;
}

static void
gst_smetsdmx_clock_init (GstSmeTSDMXClock* self)
{
    GST_INFO_OBJECT(self, "gst_smetsdmx_clock_init in...");

    GST_INFO_OBJECT(self, "gst_smetsdmx_clock_init out");
}

GstClock*
gst_smetsdmx_clock_new (const gchar* name, GstSmeTSDMXClockGetTimeFunc func,
                        gpointer user_data)
{
    GstSmeTSDMXClock* tsdmx_clock =
        GST_SMETSDMX_CLOCK (g_object_new (GST_TYPE_SMETSDMX_CLOCK, NULL));

    GST_INFO_OBJECT(tsdmx_clock, "gst_smetsdmx_clock_new in tsdmx_clock=%p, func=%p",
                    tsdmx_clock, func);
    (void)name;
    if (tsdmx_clock)
    {
        tsdmx_clock->func = func;
        tsdmx_clock->user_data = user_data;
    }
    else
    {
        GST_ERROR_OBJECT(tsdmx_clock, "gst_smetsdmx_clock_new failed null !");
    }

    GST_INFO_OBJECT(tsdmx_clock, "gst_smetsdmx_clock_new out");

    return GST_CLOCK (tsdmx_clock);
}

static GstClockTime
gst_smetsdmx_clock_get_internal_time (GstClock* self)
{
    GstSmeTSDMXClock* tsdmx_clock = GST_SMETSDMX_CLOCK (self);

    return tsdmx_clock->func (self, tsdmx_clock->user_data);
}

static GstClockTime
gst_tsdmx_clock_func_invalid(GstClock * self, gpointer user_data)
{
    (void)self;
    (void)user_data;
    return GST_CLOCK_TIME_NONE;
}

void
gst_smetsdmx_clock_invalidate(GstClock* self)
{
    GstSmeTSDMXClock *tsdmx_clock;
    tsdmx_clock = GST_SMETSDMX_CLOCK (self);

    tsdmx_clock->func = gst_tsdmx_clock_func_invalid;
}
