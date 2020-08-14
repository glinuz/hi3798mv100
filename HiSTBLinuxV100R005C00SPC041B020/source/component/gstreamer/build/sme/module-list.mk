ifneq ($(findstring y, $(self-rd)_$(demo)_$(ut)), y)
#install ndk
ifeq ($(CFG_ENABLE_NDK), y)
-include $(SMEDIR)/thirdparty/pkg_ndk.mk
endif

############################## begin compile opensource #####################
#glib及其依赖包
include $(SMEDIR)/thirdparty/pkg_iconv.mk
include $(SMEDIR)/thirdparty/pkg_gettext.mk
include $(SMEDIR)/thirdparty/pkg_ffi.mk
include $(SMEDIR)/thirdparty/pkg_glib.mk
include $(SMEDIR)/thirdparty/pkg_glib_all.mk

#gstreamer core
include $(SMEDIR)/thirdparty/pkg_gst_core.mk

#gstreamer base插件包
include $(SMEDIR)/thirdparty/pkg_gst_base.mk

#gstreamer good依赖包，主要是soup及其依赖包
ifeq ($(findstring y, $(CFG_ENABLE_SME_HTTPSRC_SOUP)), y)
include $(SMEDIR)/thirdparty/pkg_xml2.mk
include $(SMEDIR)/thirdparty/pkg_sqlite3.mk
include $(SMEDIR)/thirdparty/pkg_soup.mk
include $(SMEDIR)/thirdparty/pkg_gst_good_all.mk
endif

#gstreamer bad及其依赖包,如果开了hls或者dash
ifeq ($(findstring y, $(CFG_ENABLE_GST_BAD_HLS)_$(CFG_ENABLE_GST_BAD_DASH)), y)
-include $(SMEDIR)/thirdparty/pkg_gmp.mk
-include $(SMEDIR)/thirdparty/pkg_nettle.mk
-include $(SMEDIR)/thirdparty/pkg_gnutls.mk
-include $(SMEDIR)/thirdparty/pkg_gst_bad.mk
-include $(SMEDIR)/thirdparty/pkg_gst_bad_all.mk
else
ifeq ($(findstring p, $(CFG_ENABLE_GST_BAD_HLS)_$(CFG_ENABLE_GST_BAD_DASH)), p)
-include $(SMEDIR)/thirdparty/pkg_gmp.mk
-include $(SMEDIR)/thirdparty/pkg_nettle.mk
-include $(SMEDIR)/thirdparty/pkg_gnutls.mk
endif
endif

#gst-omx插件包
ifeq ($(CFG_ENABLE_GSTOMX),y)
-include $(SMEDIR)/thirdparty/pkg_gst_omx.mk
endif

#ffmpeg
#ffmpeg2.8.x
ifeq ($(CFG_ENABLE_FFMPEG),y)
-include $(SMEDIR)/thirdparty/pkg_ffmpeg.mk
endif

#all pkg
include $(SMEDIR)/thirdparty/pkg_opensource_all.mk

#ffmpeg prebuilt
ifeq ($(CFG_ENABLE_FFMPEG), p)
-include $(TOPDIR)/source/sme/prebuilt/libsme_ffmpeg.mk
endif

ifeq ($(CFG_ENABLE_HWSECURE), y)
-include $(TOPDIR)/thirdparty/libhwsecurec/libhwsecurec.mk
endif

endif
############################## end compile opensource ########################

############################## begin sme self-RD compile  #########################

ifneq ($(findstring y, $(demo)_$(ut)), y)

#sme-prebuilt
-include $(TOPDIR)/source/sme/prebuilt/sme_prebuilt.mk

-include $(SMEDIR)/source/module-list.mk

endif

############################## end sme self-RD compile #########################

#test
ifneq ($(self-rd), y)
-include $(TOPDIR)/test/sme/module-list.mk
endif
