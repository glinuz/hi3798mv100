#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#===============================================================================
# export variables
#===============================================================================
EXTRA_CFLAGS += -DENV_ARMLINUX_KERNEL

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
VFMW_DIR := $(MSP_DIR)/drv/vfmw/vfmw_v5.0
else
VFMW_DIR := $(MSP_DIR)/drv/vfmw/vfmw_v4.0
endif

################################################
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716cv200 hi3716mv400 hi3718cv100 hi3719cv100 hi3718mv100 hi3719mv100),)
include $(VFMW_DIR)/firmware/product/Hi3716CV200/HD_FULL/vfmw_make.cfg
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
include $(VFMW_DIR)/firmware/product/Hi3798MV100/HD_FULL/vfmw_make.cfg
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
include $(VFMW_DIR)/firmware/product/Hi3716MV410/HD_FULL/vfmw_make.cfg
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3751v100),)
include $(VFMW_DIR)/firmware/product/HiS5V100/HD_FULL/vfmw_make.cfg
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3751v200),)
include $(VFMW_DIR)/firmware/product/HiS5V200/HD_FULL/vfmw_make.cfg
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
include $(VFMW_DIR)/firmware/product/Hi3798CV200/HD_FULL/vfmw_make.cfg
endif

SOFTLIB_DIR := $(MSP_DIR)/drv/vfmw/softlib

HWMEDIA_DIR := $(SOFTLIB_DIR)/hwmedia_v1.1

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE) \
                -I$(COMMON_API_INCLUDE) \
                -I$(COMMON_DRV_INCLUDE) \
                -I$(SOFTLIB_DIR) \
                -I$(HWMEDIA_DIR)/inc \
                -I$(HWMEDIA_DIR)/src \
                -I$(HWMEDIA_DIR)/src/hwdec/inc \
                -I$(HWMEDIA_DIR)/src/hwdec/inc/kernel \
                -I$(HWMEDIA_DIR)/src/hwdec/hwcodec \
                -I$(HWMEDIA_DIR)/src/hwdec/hwcodec/x86 \
                -I$(HWMEDIA_DIR)/src/hwdec/hwutil \
                -I$(HWMEDIA_DIR)/src/hwdec/hwutil/x86

EXTRA_CFLAGS += -DBUILD_ARM -DHAVE_IPP=0 -DARM_ASM_OPT -DIMEDIA_LINUX_KERNEL

ifeq ($(CFG_HI_CPU_ARCH),arm64)
EXTRA_CFLAGS += -DARMV8 -mcpu=cortex-a53 -fno-signed-zeros
else
EXTRA_CFLAGS += -mcpu=cortex-a9 -march=armv7-a -fno-signed-zeros
endif

ifneq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_KMOD_CFLAGS)), )
EXTRA_CFLAGS += -DHI_ADVCA_FUNCTION_RELEASE
endif

EXTRA_CFLAGS += -D__KERNEL__ -static 

EXTRA_CFLAGS += -freg-struct-return -fno-omit-frame-pointer -Wdeclaration-after-statement -D_ISOC99_SOURCE -D_POSIX_C_SOURCE=200112 

EXTRA_AFLAGS =

EXTRA_LDFLAGS += -static

EXTRA_CFLAGS += $(VFMW_CFLAGS)

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

####################################### objects #####################################
MOD_NAME := hi_svdec

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

#1. ko interface
$(MOD_NAME)-y += sdec_imedia_intf.o

# 2. soft decoder
$(MOD_NAME)-y += sdec_imedia.o

#libavutil
$(MOD_NAME)-y += hwmedia_v1.1/src/hwdec/hwutil/avstring.o \
    hwmedia_v1.1/src/hwdec/hwutil/log.o \
    hwmedia_v1.1/src/hwdec/hwutil/mathematics.o \
    hwmedia_v1.1/src/hwdec/hwutil/mem.o \

#libavcodec 
$(MOD_NAME)-y += hwmedia_v1.1/src/hwdec/hwcodec/Bitstream.o \
    hwmedia_v1.1/src/hwdec/hwcodec/dsputil.o \
    hwmedia_v1.1/src/hwdec/hwcodec/error_resilience.o \
    hwmedia_v1.1/src/hwdec/hwcodec/imgconvert.o \
    hwmedia_v1.1/src/hwdec/hwcodec/h263.o \
    hwmedia_v1.1/src/hwdec/hwcodec/h263dec.o \
    hwmedia_v1.1/src/hwdec/hwcodec/h263_parser.o \
    hwmedia_v1.1/src/hwdec/hwcodec/mpegvideo.o \
    hwmedia_v1.1/src/hwdec/hwcodec/options.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/utils.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/golomb.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/huffman.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/intrax8dsp.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/jrevdct.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/mdct.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/mpeg4video_parser.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/parser.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/simple_idct.o  \
    hwmedia_v1.1/src/hwdec/hwcodec/h264idct.o \

#libhwmedia
$(MOD_NAME)-y += hwmedia_v1.1/src/imedia_viddec.o \
    hwmedia_v1.1/src/imedia_util.o \

ifeq ($(CFG_HI_CPU_ARCH),arm64)
$(MOD_NAME)-y += hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_arm.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/float_arm_vfp.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/mpegvideo_arm.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_init_armv6.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/hpeldsp_neon.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/float_dsp_neon.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_arm.o
else
$(MOD_NAME)-y += hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_arm.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/float_arm_vfp.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/mpegvideo_arm.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_init_armv6.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_arm.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/simple_idct_armv6.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/h263_loop_armv6.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_armv6.o \
    hwmedia_v1.1/src/hwdec/hwcodec/arm/dsputil_vfp.o
endif

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

all:
	$(AT)make -C $(HWMEDIA_DIR) ARCH=$(CFG_HI_CPU_ARCH)
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)make -C $(HWMEDIA_DIR) clean
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko
