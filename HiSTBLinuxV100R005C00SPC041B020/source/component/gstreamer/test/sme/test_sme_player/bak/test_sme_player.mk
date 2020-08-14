LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test_sme_player

ifeq ($(findstring -D__LINUX__, $(CFG_COMMON_MACRO)), -D__LINUX__)
#for linux
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(TOPDIR)/$(CFG_RT_HI_SDK_DIR)
#$(info ### SDK_DIR ###: $(SDK_DIR))
-include $(SDK_DIR)/base.mak
endif
#$(info ### SAMPLE_DIR ###: $(SAMPLE_DIR))
-include $(SAMPLE_DIR)/base.mak

endif

LOCAL_SRC_FILES := test_sme_player.cpp
LOCAL_CFLAGS += $(EXTRA_CFLAGS)

LOCAL_CFLAGS += -Wno-unused-parameter -Wno-unused-function -Wall -Wno-unknown-pragmas \
                  -Wno-write-strings -Wno-address -Wno-write-strings -Wno-shadow        \
                  -Wno-unused-variable -Wno-sign-compare -Wno-unused-but-set-variable   \
                  -Wno-format -Wno-unused-label -Wno-type-limits -Wno-missing-field-initializers \
				  -Wno-strict-prototypes -Wno-missing-prototypes -Wno-empty-body \
				  -Wno-cast-qual

LOCAL_CPPFLAGS += -Wno-unused-parameter -Wno-unused-function -Wall -Wno-unknown-pragmas \
                  -Wno-write-strings -Wno-address -Wno-write-strings -Wno-shadow        \
                  -Wno-unused-variable -Wno-sign-compare -Wno-unused-but-set-variable   \
                  -Wno-format -Wno-unused-label -Wno-type-limits -Wno-missing-field-initializers

LOCAL_CPPFLAGS += $(EXTRA_CXXFLAGS)
LOCAL_LDFLAGS += $(EXTRA_LDFLAGS)
LOCAL_C_INCLUDES += $(TOPDIR)/source/sme/source/osal/inc \
	$(TOPDIR)/source/sme/api \
	$(TOPDIR)/source/sme/source/common/inc \
	$(TOPDIR)/source/sme/source/plugins/common

LOCAL_LDLIBS += $(EXTRA_LDLIBS)

LOCAL_SHARED_LIBRARIES := libsme_osal libsme_common libsme_player
LOCAL_REQUIRED_MODULES := libsme_osal libsme_common libsme_player test_sme_player_res

ifeq ($(findstring -D__LINUX__, $(CFG_COMMON_MACRO)), -D__LINUX__)
#for linux
LOCAL_SRC_FILES += test_subtile_disp_linux_hi3798m.cpp \
	test_hdmi_init.c

ifneq ($(CFG_PROJECT_NAME), HiDolphin.Hi3798CV200)
#HI_SRCS := $(patsubst %.o, %.c, $(COMMON_FILES))
HI_SRCS := $(wildcard $(SAMPLE_DIR)/common/*.c)
#$(info ### 0 HI_SRCS ###: $(HI_SRCS), ### SAMPLE_DIR: $(SAMPLE_DIR))
HI_SRCS := $(patsubst $(TOPDIR)/%, ../../../%, $(HI_SRCS))
#$(info ### 1 HI_SRCS ###: $(HI_SRCS))
HI_SRCS := $(filter-out %/hi_adp_pvr.c, $(HI_SRCS))
HI_SRCS := $(filter-out %/hi_adp_audio.c, $(HI_SRCS))
#$(info ### 2 HI_SRCS ###: $(HI_SRCS))
LOCAL_SRC_FILES += $(HI_SRCS)
#$(info ==3 LOCAL_SRC_FILES==: $(LOCAL_SRC_FILES))
endif

LOCAL_CFLAGS += -I$(SDK_INC_DIR) -I$(SAMPLE_DIR)/common  $(CFG_HI_SAMPLE_CFLAGS)
LOCAL_CPPFLAGS += -I$(SDK_INC_DIR) -I$(SAMPLE_DIR)/common  $(CFG_HI_SAMPLE_CFLAGS)
LOCAL_C_INCLUDES += $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), $(TOPDIR)/$(h_tmp))
LOCAL_LDFLAGS += -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR)/../extern \
	-L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR)/alsa_bluez \
	-Wl,-rpath-link=$(TOPDIR)/$(CFG_RT_LIBHWSECURE_DIR)/out/lib,-rpath-link=$(TARGET_OUT_TO_PREBUILT_DIR)

#$(info COMMON_FILES=$(COMMON_FILES))
#LOCAL_PREBUILT_OBJ_FILES := $(COMMON_FILES)
#$(info HI_LIBS=$(HI_LIBS))
HI_LIBS_MIN=-lhi_common \
    -lpng -lhigo -lhigoadp \
    -ljpeg -lhi_msp -lz \
    -lfreetype

LOCAL_LDLIBS += -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR) $(SYS_LIBS) $(HI_LIBS_MIN)\
	-lOMX.hisi.audio.utils

ifeq ($(CFG_PROJECT_NAME), HiDolphin.Hi3798CV200)
ifneq ($(CFG_HI_AARCH64_TOOLCHAINS_NAME),)
LOCAL_LDLIBS += -L$(HI_EXTERN_LIB64_DIR)
else
LOCAL_LDLIBS += -L$(HI_EXTERN_LIB_DIR)
endif
endif

#$(info ##### TARGET_OUTDIR=$(TARGET_OUTDIR))
#$(info ##### ENV_SME_VERSION_TYPE=$(ENV_SME_VERSION_TYPE))
#$(info ##### TARGET_OUT_VERSION_TYPE=$(TARGET_OUT_VERSION_TYPE))

ifneq ($(CFG_PROJECT_NAME), linux_hi3798m_generic)
ifneq ($(CFG_PROJECT_NAME), HiDolphin.Hi3798CV200)
LOCAL_LDLIBS += -ldirectfb -ldirect -lfusion -llog
endif
endif

else
#for tvos-android
LOCAL_CFLAGS += -D __STDC_CONSTANT_MACROS
LOCAL_CPPFLAGS += -D __STDC_CONSTANT_MACROS

LOCAL_C_INCLUDES += $(TOPDIR)/test/sme/utils_native_surface/inc \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/include/android \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/bionic/libc/include \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/bionic/libc/kernel/uapi \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/bionic/libc/kernel/uapi \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/bionic/libc/kernel/uapi/asm-arm  \
	$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DIR)/platform/android/frameworks/native/include
LOCAL_LDLIBS += \
	-L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR) \
	-lcutils \
	-lbinder \
	-lutils \
	-llog \
	-lgui

LOCAL_C_INCLUDES += $(foreach h_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), $(TOPDIR)/$(h_tmp))

LOCAL_SHARED_LIBRARIES += libsme_native_sf
LOCAL_REQUIRED_MODULES += libsme_native_sf
endif

include $(BUILD_EXECUTABLE)

LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := test_sme_player_res

$(LOCAL_MODULE)-deps : $(LOCAL_MODULE)

$(LOCAL_MODULE) :
	cp $(MEDIAOSDIR)/test/sme/test_sme_player/res $(TARGET_OUT_BIN_DIR)/ -af

$(LOCAL_MODULE)-deps-clean:$(LOCAL_MODULE)-clean
$(LOCAL_MODULE)-cfg-clean:$(LOCAL_MODULE)-clean
$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean

$(LOCAL_MODULE)-clean :
	$(NC)rm -rf $(TARGET_OUT_BIN_DIR)/res

include $(BUILD_OPENSOURCE_PKG)
