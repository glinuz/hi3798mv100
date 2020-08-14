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

#ifndef __GST_SMETSDMX_CLOCK_H__
#define __GST_SMETSDMX_CLOCK_H__

#include <gst/gst.h>


G_BEGIN_DECLS

#define GST_TYPE_SMETSDMX_CLOCK \
    (gst_smetsdmx_clock_get_type())
#define GST_SMETSDMX_CLOCK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SMETSDMX_CLOCK,GstSmeTSDMXClock))
#define GST_SMETSDMX_CLOCK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SMETSDMX_CLOCK,GstSmeTSDMXClockClass))
#define GST_IS_SMETSDMX_CLOCK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SMETSDMX_CLOCK))
#define GST_IS_SMETSDMX_CLOCK_CLASS(obj) \
    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SMETSDMX_CLOCK))

typedef struct _GstSmeTSDMXClock GstSmeTSDMXClock;
typedef struct _GstSmeTSDMXClockClass GstSmeTSDMXClockClass;

typedef GstClockTime (*GstSmeTSDMXClockGetTimeFunc) (GstClock* clock,
        gpointer user_data);

struct _GstSmeTSDMXClock
{
    GstSystemClock clock;
    GstSmeTSDMXClockGetTimeFunc func;
    gpointer user_data;
};

struct _GstSmeTSDMXClockClass
{
    GstSystemClockClass parent_class;
};

GType gst_smetsdmx_clock_get_type(void);
GstClock* gst_smetsdmx_clock_new(const gchar* name,
                                 GstSmeTSDMXClockGetTimeFunc func,
                                 gpointer user_data);
void
gst_smetsdmx_clock_invalidate(GstClock* self);

G_END_DECLS

#endif /* __GST_SMETSDMX_CLOCK_H__ */
