LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsme_player

LOCAL_SRC_FILES := sme_player_impl_gst.cpp \
	sme_player_msg.cpp \
	sme_player_gst_wrap.cpp \
	sme_init.cpp

ifeq ($(CFG_ENABLE_SUBTITLE_PATH), y)
LOCAL_SRC_FILES += sme_gstappsink_sub_wrap.cpp
LOCAL_CFLAGS += -DENABLE_SUBTITLE_PATH
LOCAL_CPPFLAGS += -DENABLE_SUBTITLE_PATH
endif

LOCAL_CFLAGS += $(EXTRA_CFLAGS) -D SME_VERSION=\"$(CFG_SME_VERSION)\"
LOCAL_CPPFLAGS += $(EXTRA_CXXFLAGS) -D SME_VERSION=\"$(CFG_SME_VERSION)\"

LOCAL_C_INCLUDES := $(TOPDIR)/$(CFG_RT_SME_THIRDPARTY_MERGE_DIR)/gstreamer-1.2.3 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0/gst \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0/gst/app \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/gio \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib/glib-2.0/include \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib/gobject

LOCAL_C_INCLUDES += $(TOPDIR)/source/sme/source/osal/inc \
	$(TOPDIR)/source/sme/api \
	$(TOPDIR)/source/sme/source/common/inc \
	$(TOPDIR)/source/sme/source/plugins/common

LOCAL_LDLIBS += $(EXTRA_LDLIBS)
LOCAL_LDLIBS += -L$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib \
	-lgstreamer-1.0  -lglib-2.0 -lgobject-2.0 \
	-lgmodule-2.0 -lgio-2.0 -lgstapp-1.0

LOCAL_LDLIBS += -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR) \
	-lhi_common

LOCAL_SHARED_LIBRARIES := libsme_osal libsme_common libsme_gst_common
LOCAL_REQUIRED_MODULES := libsme_osal libsme_common libsme_gst_common

ifneq ($(ENV_SME_VERSION_TYPE), rls)
ifeq ($(CFG_ENABLE_BENCHMARK), y)
LOCAL_SHARED_LIBRARIES += libsme_gst_benchmark
LOCAL_REQUIRED_MODULES += libsme_gst_benchmark
endif
endif

include $(BUILD_SHARED_LIBRARY)
