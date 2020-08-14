#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

TARGET := gfx2d

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/$(TARGET)/include      \
                -I$(MSP_DIR)/drv/$(TARGET)              \
                -I$(MSP_DIR)/drv/$(TARGET)/hwc          \
                -I$(COMMON_DIR)/drv/mmz                 \
                -I$(MSP_DIR)/drv/jpeg/grc_cmm_inc

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)
EXTRA_CFLAGS += -DCONFIG_GFX_STB_SDK

ifeq ($(CFG_ANDROID_PRODUCT),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/staging/android
#EXTRA_CFLAGS += -DGFX2D_FENCE_SUPPORT
endif

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_GFX_MMU_SUPPORT
endif

ifeq ($(CFG_HI_GFX_DEBUG_LOG_SUPPORT), y)
EXTRA_CFLAGS += -DCFG_HI_GFX_DEBUG_LOG_SUPPORT
endif

GFX2D_CFLAGS = -DHWC_VERSION=0x100

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3719mv100 hi3718mv100), )
GFX2D_CFLAGS = -DHWC_VERSION=0x200
GFX2D_VER_DIR = v1
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
GFX2D_CFLAGS = -DHWC_VERSION=0x400
GFX2D_VER_DIR = v1
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200), )
GFX2D_CFLAGS = -DHWC_VERSION=0x500 -DGFX2D_SYNC_TDE -I$(MSP_DIR)/api/tde/include
GFX2D_VER_DIR = v2
endif

EXTRA_CFLAGS += $(GFX2D_CFLAGS)

MOD_NAME := hi_$(TARGET)

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y :=    gfx2d_os.o \
                    gfx2d_ctl.o \
                    gfx2d_list.o \
                    gfx2d_hal.o \
                    gfx2d_mem.o\
                    hwc/$(GFX2D_VER_DIR)/gfx2d_hal_hwc.o \
                    hwc/$(GFX2D_VER_DIR)/gfx2d_hal_hwc_adp.o \
                    gfx2d_config.o \
                    gfx2d_fence.o

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko
