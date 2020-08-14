#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../../../../ && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

export HWMEDIA_SRC_DIR = $(HWMEDIA_DIR)/src

#libavutil
hwmedia-objs += ./src/hwdec/hwutil/avstring.o \
	./src/hwdec/hwutil/log.o \
	./src/hwdec/hwutil/mathematics.o \
	./src/hwdec/hwutil/mem.o \

#libavcodec	
hwmedia-objs += ./src/hwdec/hwcodec/bitstream.o \
	./src/hwdec/hwcodec/dsputil.o \
	./src/hwdec/hwcodec/error_resilience.o \
	./src/hwdec/hwcodec/imgconvert.o \
	./src/hwdec/hwcodec/h263.o \
	./src/hwdec/hwcodec/h263dec.o \
	./src/hwdec/hwcodec/h263_parser.o \
	./src/hwdec/hwcodec/mpegvideo.o \
	./src/hwdec/hwcodec/options.o  \
	./src/hwdec/hwcodec/utils.o  \
	./src/hwdec/hwcodec/golomb.o  \
	./src/hwdec/hwcodec/huffman.o  \
	./src/hwdec/hwcodec/intrax8dsp.o  \
	./src/hwdec/hwcodec/jrevdct.o  \
	./src/hwdec/hwcodec/mdct.o  \
	./src/hwdec/hwcodec/mpeg4video_parser.o  \
	./src/hwdec/hwcodec/parser.o  \
	./src/hwdec/hwcodec/simple_idct.o  \
	./src/hwdec/hwcodec/h264idct.o \

#libhwmedia
hwmedia-objs += ./src/imedia_viddec.o \
	./src/imedia_util.o \

#libdrv
#imedia-objs += ./src/imedia_kernel.o

ifeq ($(CFG_HI_CPU_ARCH),arm64)
hwmedia-objs += ./src/hwdec/hwcodec/arm/dsputil_arm.o \
	./src/hwdec/hwcodec/arm/float_arm_vfp.o \
	./src/hwdec/hwcodec/arm/mpegvideo_arm.o \
	./src/hwdec/hwcodec/arm/dsputil_init_armv6.o \
	./src/hwdec/hwcodec/arm/hpeldsp_neon.o \
	./src/hwdec/hwcodec/arm/float_dsp_neon.o \
	./src/hwdec/hwcodec/arm/dsputil_arm.o
else
hwmedia-objs += ./src/hwdec/hwcodec/arm/dsputil_arm.o \
	./src/hwdec/hwcodec/arm/float_arm_vfp.o \
	./src/hwdec/hwcodec/arm/mpegvideo_arm.o \
	./src/hwdec/hwcodec/arm/dsputil_init_armv6.o \
	./src/hwdec/hwcodec/arm/dsputil_arm.o \
	./src/hwdec/hwcodec/arm/simple_idct_armv6.o \
	./src/hwdec/hwcodec/arm/h263_loop_armv6.o \
	./src/hwdec/hwcodec/arm/dsputil_armv6.o \
	./src/hwdec/hwcodec/arm/dsputil_vfp.o
endif

OUTPUT_DIR = $(HWMEDIA_DIR)/bin/arm-hisi_kernel/

IN_DIR = -I$(HWMEDIA_DIR)/inc \
	-I$(HWMEDIA_SRC_DIR) \
	-I$(HWMEDIA_SRC_DIR)/hwdec/inc \
	-I$(HWMEDIA_SRC_DIR)/hwdec/inc/kernel \
	-I$(HWMEDIA_SRC_DIR)/hwdec/hwcodec \
	-I$(HWMEDIA_SRC_DIR)/hwdec/hwcodec/x86 \
	-I$(HWMEDIA_SRC_DIR)/hwdec/hwutil \
	-I$(HWMEDIA_SRC_DIR)/hwdec/hwutil/x86 \

EXTRA_CFLAGS += -DBUILD_ARM -DHAVE_IPP=0 -DARM_ASM_OPT -DIMEDIA_LINUX_KERNEL

ifeq ($(CFG_HI_CPU_ARCH),arm64)
EXTRA_CFLAGS += -DARMV8 -mcpu=cortex-a53 -fno-signed-zeros
else
EXTRA_CFLAGS += -mcpu=cortex-a9 -march=armv7-a -fno-signed-zeros
endif

ifneq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_KMOD_CFLAGS)), )
EXTRA_CFLAGS += -DHI_ADVCA_FUNCTION_RELEASE
endif

obj-y := hwmedia.o

imedia-objs += ./src/imedia_kernel.o hwmedia.o

obj-m := imedia.o

EXTRA_CFLAGS += -D__KERNEL__ -static 

EXTRA_CFLAGS += $(IN_DIR) -freg-struct-return -fno-omit-frame-pointer -Wdeclaration-after-statement -D_ISOC99_SOURCE -D_POSIX_C_SOURCE=200112 
#EXTRA_CFLAGS += -std=c99 -Wno-switch  -fPIC

EXTRA_AFLAGS = 

EXTRA_LDFLAGS += -static

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=`pwd` modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf


