/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmeffprosrc.h
 * @brief   fake source element for ffmpeg protocol
 * @author
 * @date    2016/6/12th
 * @version
 * History:
 *
 * Number : V1.00
 * Date   : 2016/6/12
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __GST_SME_FF_PRO_SRC_H__
#define __GST_SME_FF_PRO_SRC_H__

#include <gst/gst.h>
#include <glib.h>
#include <gst/base/gstbasesrc.h>
G_BEGIN_DECLS


#define GST_TYPE_SME_FF_PRO_SRC \
  (gst_sme_ffpro_src_get_type())
#define GST_SME_FF_PRO_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SME_FF_PRO_SRC,GstSmeFfProSrc))
#define GST_SME_FF_PRO_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), \
      GST_TYPE_SME_FF_PRO_SRC,GstSmeFfProSrcClass))
#define GST_IS_SME_FF_PRO_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_FF_PRO_SRC))
#define GST_IS_SME_FF_PRO_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_FF_PRO_SRC))

typedef struct _GstSmeFfProSrc GstSmeFfProSrc;
typedef struct _GstSmeFfProSrcClass GstSmeFfProSrcClass;

enum
{
    PROP_0,
    PROP_LOCATION,
};

struct _GstSmeFfProSrc {
    GstBaseSrc element;

    gchar *location;             /* Full URI. */
    GstCaps *src_caps;
};

struct _GstSmeFfProSrcClass {
    GstBaseSrcClass parent_class;
};

GType gst_sme_ffpro_src_get_type (void);

G_END_DECLS

#endif /* __GST_SME_FF_PRO_SRC_H__ */
