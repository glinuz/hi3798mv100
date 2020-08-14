/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file		  -gstsmeplugin_hisimpi.c
 * @brief	  -sme plugin hisimpi entry
 * @author	  -liurongliang(l00180035)
 * @date		  -2015/1/21
 * @version      -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date 	 : 2015/1/21
 * Author	 : liurongliang(l00180035)
 * Desc 	 : Created file
 *
******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
//for dynamic load smeplugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif
#endif


#include "gstsme.h"
#include "gstsmevideosink_hihal_allcator.h"

//lint -e717 //do{}while(0)

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_ERROR("register sme plugin hisihal begin...");

    if (!gst_smevideosink_hihal_register (plugin))
        GST_WARNING("gst_smevideosink_hihal_register fail.");
//
//    if (!gst_smevideodec_himpi_register (plugin))
//        GST_WARNING("gst_smevideodec_himpi_register fail.");

    GST_ERROR("register sme plugin hisihal end");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    sme_hisihal,
    "sme plugin hisihal of mediaos. (info about sme: http://www.huawei.com)",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
