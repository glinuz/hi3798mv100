LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsme_gstplg_tsdmx

LOCAL_SRC_FILES := gstsmetsdmx.c

ifeq ($(findstring -DENABLE_PCR_CLOCK, $(CFG_COMMON_MACRO)), -DENABLE_PCR_CLOCK)
LOCAL_SRC_FILES += gstsmetsdmxclock.c
endif

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

LOCAL_C_INCLUDES += $(TOPDIR)/source/sme/api \
    $(TOPDIR)/source/sme/source/osal/inc \
    $(TOPDIR)/source/sme/source/common/inc \
    $(TOPDIR)/source/sme/source/plugins/common

LOCAL_C_INCLUDES += $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), $(TOPDIR)/$(h_tmp))

LOCAL_LDLIBS += $(EXTRA_LDLIBS)
LOCAL_LDLIBS += -L$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib \
        $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR), -L$(TOPDIR)/$(h_tmp)) \
        -lgstreamer-1.0 -lgstbase-1.0 \
	-lglib-2.0 -lgobject-2.0 \
	-lgmodule-2.0 -lgio-2.0 \
	-lsme_ffmpeg

ifeq ($(findstring -DLINUX_OS, $(CFG_COMMON_MACRO)), -DLINUX_OS)
LOCAL_LDLIBS += -lhal -pthread
else
LOCAL_LDLIBS += -lhalhardware
endif

LOCAL_SHARED_LIBRARIES := libsme_osal
LOCAL_REQUIRED_MODULES :=

LOCAL_MODULE_PATH := $(TARGET_OUT_PLUGIN_DIR)

include $(BUILD_SHARED_LIBRARY)
