#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
GPU_DIR := $(MSP_DIR)/api/gpu

HEADER_FILES += $(GPU_DIR)/include/KHR
HEADER_FILES += $(GPU_DIR)/include/EGL
HEADER_FILES += $(GPU_DIR)/include/GLES
HEADER_FILES += $(GPU_DIR)/include/GLES2

ifeq ($(CFG_HI_GPU_MALI400_SUPPORT), y)
MALI_VER := mali400
LIB_NAME := libMali.so
else ifeq ($(CFG_HI_GPU_MALI450_SUPPORT), y)
MALI_VER := mali450
LIB_NAME := libMali.so
else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT), y)
MALI_VER := mali700
LIB_NAME := libmali.so
HEADER_FILES += $(GPU_DIR)/include/GLES3
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all: 
ifeq ($(CFG_HI_GPU_SUPPORT),y)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -rf $(HEADER_FILES) $(HI_INCLUDE_DIR)/
ifeq ($(CFG_HI_EGL_TYPE), fb)
	$(AT)cp -rf include/EGL/platform_fbdev/EGL/$(MALI_VER)/*.h $(HI_INCLUDE_DIR)/EGL/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -rf lib/platform_fbdev/$(CFG_HI_ARM_TOOLCHAINS_NAME)/$(MALI_VER)/*.so $(HI_SHARED_LIB_DIR)
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -rf lib/platform_fbdev/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)/$(MALI_VER)/*.so $(HI_SHARED_LIB64_DIR)
endif
endif
ifeq ($(CFG_HI_EGL_TYPE), directfb)
	$(AT)cp -rf include/EGL/platform_directfb/EGL/$(MALI_VER)/*.h $(HI_INCLUDE_DIR)/EGL/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -rf lib/platform_directfb/$(CFG_HI_ARM_TOOLCHAINS_NAME)/$(MALI_VER)/*.so $(HI_SHARED_LIB_DIR)
endif
endif
ifeq ($(HI_USER_SPACE_LIB),y)
	cd $(HI_SHARED_LIB_DIR); \
        ln -sf $(LIB_NAME)          libEGL.so.1.4;\
        ln -sf libEGL.so.1.4        libEGL.so.1;\
        ln -sf libEGL.so.1          libEGL.so;\
        ln -sf $(LIB_NAME)          libGLESv1_CM.so.1.1;\
        ln -sf libGLESv1_CM.so.1.1  libGLESv1_CM.so.1;\
        ln -sf libGLESv1_CM.so.1    libGLESv1_CM.so;\
        ln -sf $(LIB_NAME)          libGLESv2.so.2.0;\
        ln -sf libGLESv2.so.2.0     libGLESv2.so.2;\
        ln -sf libGLESv2.so.2       libGLESv2.so;\
        cd -;
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	cd $(HI_SHARED_LIB64_DIR); \
        ln -sf $(LIB_NAME)          libEGL.so.1.4;\
        ln -sf libEGL.so.1.4        libEGL.so.1;\
        ln -sf libEGL.so.1          libEGL.so;\
        ln -sf $(LIB_NAME)          libGLESv1_CM.so.1.1;\
        ln -sf libGLESv1_CM.so.1.1  libGLESv1_CM.so.1;\
        ln -sf libGLESv1_CM.so.1    libGLESv1_CM.so;\
        ln -sf $(LIB_NAME)          libGLESv2.so.2.0;\
        ln -sf libGLESv2.so.2.0     libGLESv2.so.2;\
        ln -sf libGLESv2.so.2       libGLESv2.so;\
        cd -;
endif
endif

clean:
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/$(LIB_NAME)
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libGLES* $(HI_SHARED_LIB_DIR)/libEGL*
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libOpenCL*
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/$(LIB_NAME)
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libGLES* $(HI_SHARED_LIB64_DIR)/libEGL*
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libOpenCL*
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))

