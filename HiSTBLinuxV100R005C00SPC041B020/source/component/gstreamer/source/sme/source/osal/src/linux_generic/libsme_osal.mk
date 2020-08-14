LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsme_osal


LOCAL_SRC_FILES := osal_str_linux_generic.cpp \
        osal_mem_linux_generic.cpp \
        osal_thread_linux_generic.cpp

LOCAL_CFLAGS += $(EXTRA_CFLAGS)
LOCAL_CPPFLAGS += $(EXTRA_CXXFLAGS)

LOCAL_C_INCLUDES := $(TOPDIR)/$(CFG_RT_LIBHWSECURE_DIR)/include \
        $(TOPDIR)/source/sme/source/osal/inc \
        $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), $(TOPDIR)/$(h_tmp))

LOCAL_LDLIBS += $(EXTRA_LDLIBS)

ifneq ($(CFG_ENABLE_NDK), y)
LOCAL_LDLIBS += -lpthread
else
LOCAL_LDLIBS += -llog
endif

LOCAL_SHARED_LIBRARIES :=

include $(BUILD_SHARED_LIBRARY)
