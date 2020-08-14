LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsme_common

LOCAL_SRC_FILES := src/dlist/free_list.cpp \
	src/dlist/linked_list.cpp \
	src/dlist/sme_dlist.cpp \
	src/timer/sme_timer.cpp \
	src/log/sme_log.cpp \
	src/queue/sme_queue.cpp \
	src/msgqueue/sme_msg_queue.cpp \
	src/thread/sme_thread.cpp \
	src/logadpt/sme_log_adpt.cpp \
	src/sme_property.cpp

LOCAL_CFLAGS += $(EXTRA_CFLAGS)
LOCAL_CPPFLAGS += $(EXTRA_CXXFLAGS)

LOCAL_C_INCLUDES := $(TOPDIR)/source/sme/source/osal/inc \
	$(TOPDIR)/source/sme/source/common/inc \
	$(TOPDIR)/source/sme/api \
	$(TOPDIR)/$(CFG_RT_SME_THIRDPARTY_MERGE_DIR)/gstreamer-1.2.3 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0/gst \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/gstreamer-1.0/gst/app \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0 \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/gio \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/lib/glib-2.0/include \
	$(TOPDIR)/$(RT_THIRDPARTY_OBJ_DIR)/include/glib-2.0/glib/gobject

LOCAL_C_INCLUDES += $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), $(TOPDIR)/$(h_tmp))

LOCAL_LDLIBS += $(EXTRA_LDLIBS) $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR), -L$(TOPDIR)/$(h_tmp))

ifeq ($(findstring -DGST_SME_NO_LOGCAT, $(CFG_COMMON_MACRO)), -DGST_SME_NO_LOGCAT)
else
LOCAL_LDLIBS += -llog
endif

LOCAL_SHARED_LIBRARIES += libsme_osal
LOCAL_REQUIRED_MODULES += libsme_osal

include $(BUILD_SHARED_LIBRARY)
