LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsme_demux_impl_ffmpeg

LOCAL_SRC_FILES := gstsmeprotocol.c \
	sme_demux_impl_ffmpeg.cpp

LOCAL_CFLAGS += $(EXTRA_CFLAGS) -DHAVE_CONFIG_H
LOCAL_CPPFLAGS += $(EXTRA_CXXFLAGS) -DHAVE_CONFIG_H

LOCAL_C_INCLUDES := $(TOPDIR)/$(CFG_RT_SME_THIRDPARTY_MERGE_DIR)/gstreamer-1.2.3 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0/gst \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/gio \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib/glib-2.0/include \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib/gobject

LOCAL_C_INCLUDES += $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_FFMPEG_NAME)/ffmpeg \
	$(SME_THIRDPARTY_PATCH_DIR)/$(PKG_FFMPEG_NAME)/ffmpeg/libavcodec \
	$(SME_THIRDPARTY_PATCH_DIR)/$(PKG_FFMPEG_NAME)/ffmpeg/libavformat

ifeq ($(CFG_ENABLE_NDK), y)
LOCAL_C_INCLUDES += $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_FFMPEG_NAME)/ffmpeg/libavutil
endif

LOCAL_C_INCLUDES += $(TOPDIR)/source/sme/source/plugins/common \
	$(TOPDIR)/source/sme/source/common/inc \
	$(TOPDIR)/source/sme/source/osal/inc \
	$(TOPDIR)/source/sme/api

LOCAL_LDLIBS += $(EXTRA_LDLIBS)
LOCAL_LDLIBS += -L$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib \
	-lgstreamer-1.0  -lgstbase-1.0 \
	-lglib-2.0 -lgobject-2.0 \
	-lgmodule-2.0 -lgio-2.0 -lsme_ffmpeg

LOCAL_SHARED_LIBRARIES := libsme_osal libsme_common libsme_gst_common
LOCAL_REQUIRED_MODULES := libsme_osal libsme_common libsme_gst_common


include $(BUILD_SHARED_LIBRARY)
