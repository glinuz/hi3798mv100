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
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/higo/include           \
                -I$(MSP_DIR)/api/vo

EXTRA_CFLAGS += -DHI_DISP_BUILD_FULL  -D__DISP_PLATFORM_SDK__ -D__DISP_D0_FOLLOW_D1__ -D__DISP_SELF_START__

ifeq ($(CFG_HI_DISP_USE_QPLL_SOLELY), y)
EXTRA_CFLAGS += -DHI_DISP_DOUBLE_HD_SUPPORT
endif

ifeq ($(CFG_HIFB_CURSOR_SUPPORT), y)
EXTRA_CFLAGS += -DHI_GFX_USE_G3
endif

ifeq ($(CFG_HI_DISP_TTX_SUPPORT), y)
EXTRA_CFLAGS += -DHI_DISP_TTX_SUPPORT
endif

#ifeq ($(CFG_HI_DISP_CC_SUPPORT), y)
#EXTRA_CFLAGS += -DHI_DISP_CC_SUPPORT
#endif

#ifeq ($(CFG_HI_DISP_WSS_SUPPORT), y)
#EXTRA_CFLAGS += -DHI_DISP_WSS_SUPPORT
#endif

ifeq ($(CFG_HI_DISP_G1_ATTACH_HD_SUPPORT), y)
EXTRA_CFLAGS += -DHI_DISP_ATTACH_OSD_SUPPORT
endif

ifeq ($(CFG_HI_DISP_CSC_MODE_RGB), y)
EXTRA_CFLAGS += -DHI_DISP_MODE_TC
endif

ifeq ($(CFG_HI_VO_WRAP_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_WRAP_SUPPORT
endif

ifeq ($(CFG_HI_DISP_CGMS_SUPPORT), y)
EXTRA_CFLAGS += -DHI_DISP_CGMS_SUPPORT
endif

ifeq ($(CFG_HI_DISP_LCD_SUPPORT), y)
EXTRA_CFLAGS += -DHI_DISP_LCD_SUPPORT
endif

ifeq ($(CFG_HI_VO_MOSAIC_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_MOSAIC_SUPPORT
endif

ifeq ($(CFG_HI_VO_SINGLE_VIDEO_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_SINGLE_VIDEO_SUPPORT
endif

ifeq ($(CFG_HI_VO_STILLFRAME_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_STILLFRAME_SUPPORT
endif

ifeq ($(CFG_HI_VO_DUMPFRAME_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_DUMPFRAME_SUPPORT
endif

ifeq ($(CFG_HI_VO_MOSAIC_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_MOSAIC_SUPPORT
endif

EXTRA_CFLAGS += -DHI_VO_SHARPNESS_SUPPORT

ifeq ($(CFG_HI_VDEC_DEINTERLACE_SUPPORT), y)
EXTRA_CFLAGS += -DHI_VO_HD_VIDEO_DO_DEI
endif

ifeq ($(CFG_HI_VIDEO_BUFF_TYPE0), y)
EXTRA_CFLAGS += -DHI_VIDEO_BUFF_TYPE0
endif

ifeq ($(CFG_HI_DISP_VIRT_OFFSET_EFFECT_WHEN_FULLSCREEN), y)
EXTRA_CFLAGS += -DHI_VO_OFFSET_EFFECTIVE_WHEN_WIN_FULL
endif

ifeq ($(CFG_HI_VDP_VDAC_FULL_DRIVE), y)
EXTRA_CFLAGS += -DHI_VO_VDAC_FULL_DRIVE
endif

ifeq ($(CFG_HI_VDP_VDAC_HALF_DRIVE), y)
EXTRA_CFLAGS += -DHI_VO_VDAC_HALF_DRIVE
endif

ifeq ($(CFG_HI_VDP_VDAC_QUARTER_DRIVE), y)
EXTRA_CFLAGS += -DHI_VO_VDAC_QUARTER_DRIVE
endif

ifeq ($(CFG_HI_HDR_SUPPORT), y)
EXTRA_CFLAGS += -DVDP_DOLBY_HDR_SUPPORT
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

#EXTRA_CFLAGS += -Os
STRIP=$(HI_KERNEL_TOOLCHAINS_NAME)-strip

MOD_NAME := hi_vou

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

VO_DIR := $(shell cd $(MSP_DIR)/drv/vo && pwd)

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
HAL_DIR := mv410
VO_VER := vdp_v3_0
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
HAL_DIR := 98cv200
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200)
HAL_DIR := 98mv200
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3796mv200)
EXTRA_CFLAGS += -DCHIP_TYPE_hi3798mv200
EXTRA_CFLAGS := $(filter-out %CHIP_TYPE_hi3796mv200, $(EXTRA_CFLAGS))
HAL_DIR := 98mv200
endif


ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200_a)
HAL_DIR := 98mv200_a
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
EXTRA_CFLAGS += -DFPGA_98CV200
#ifdef CFG_HI_SMMU_SUPPORT
ifeq ($(CFG_HI_SMMU_SUPPORT),y)
EXTRA_CFLAGS += -DCFG_VDP_MMU_SUPPORT
EXTRA_CFLAGS += -DCFG_VDP_MMU_DEBUG_SUPPORT
endif
VO_VER := vdp_v4_0
#EXTRA_CFLAGS += -DHI_VDP_WATERMARK
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
HAL_DIR := 3798m
VO_VER := vdp_v2_0
endif

ifeq ($(CFG_ANDROID_PRODUCT),y)
ifneq ($(CFG_HI_RECOVERY_SUPPORT),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/staging/android
EXTRA_CFLAGS += -DHI_VO_WIN_SYNC_SUPPORT
endif
endif

ifeq ($(CFG_ANDROID_PRODUCT),y)
EXTRA_CFLAGS += -DHI_VO_DISP_ISOLATED_FMT_SUPPORT
endif

EXTRA_CFLAGS += -I$(VO_DIR)/$(VO_VER)                   \
                -I$(VO_DIR)/$(VO_VER)/include           \
                -I$(VO_DIR)/$(VO_VER)/alg               \
                -I$(VO_DIR)/$(VO_VER)/alg/decompress    \
                -I$(VO_DIR)/$(VO_VER)/com               \
                -I$(VO_DIR)/$(VO_VER)/component         \
                -I$(VO_DIR)/$(VO_VER)/drv               \
                -I$(VO_DIR)/$(VO_VER)/hal/common        \
                -I$(VO_DIR)/$(VO_VER)/hal/$(HAL_DIR)    \
                -I$(VO_DIR)/$(VO_VER)/hal/$(HAL_DIR)/inc

VO_SRCS := $(wildcard $(VO_DIR)/$(VO_VER)/hal/common/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/hal/$(HAL_DIR)/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/alg/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/alg/decompress/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/com/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/drv/*.c)

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200),)
EXTRA_CFLAGS += -I$(VO_DIR)/$(VO_VER)/hal/$(HAL_DIR)/hal_specific
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/component/*.c)
VO_SRCS += $(wildcard $(VO_DIR)/$(VO_VER)/hal/$(HAL_DIR)/hal_specific/*.c)
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200_a)
VO_SRCS := $(filter-out %vdp_hal_hdr.c, $(VO_SRCS))
VO_SRCS := $(filter-out %vdp_drv_pq_hdr.c, $(VO_SRCS))
endif

VO_SRCS := $(filter-out %.mod.c, $(VO_SRCS))

ifneq ($(CFG_HI_HDR_SUPPORT), y)
VO_SRCS := $(filter-out %win_hdr.c, $(VO_SRCS))
VO_SRCS := $(filter-out %disp_hdr.c, $(VO_SRCS))
VO_SRCS := $(filter-out %_hdr_com.c, $(VO_SRCS))
endif

VO_OBJS := $(patsubst $(VO_DIR)/%.c, %.o, $(VO_SRCS))

$(MOD_NAME)-y = $(VO_OBJS)

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules
	$(AT)$(STRIP)  --strip-unneeded $(VO_DIR)/hi_vou.ko

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko

