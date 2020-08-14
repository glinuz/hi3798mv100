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
# local variables
#===============================================================================

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS) -Wno-error=format-security

EXTRA_CFLAGS += \
	-I$(COMMON_UNF_INCLUDE) \
	-I$(COMMON_API_INCLUDE) \
	-I$(COMMON_DRV_INCLUDE) \
	-I$(MSP_UNF_INCLUDE)    \
	-I$(MSP_API_INCLUDE)    \
	-I$(MSP_DRV_INCLUDE)

asflags-y := -I$(COMMON_DRV_INCLUDE)

ifeq ($(CFG_HI_CPU_ARCH),arm)
HIFIDIR := ./hifi_32bit
else
HIFIDIR := ./hifi_64bit
endif

ADSP_DIR := $(MSP_DIR)/drv/adsp/adsp_v1_1

EXTRA_CFLAGS += \
	-I$(ADSP_DIR) \
	-I$(ADSP_DIR)/include \
	-I$(ADSP_DIR)/osal

ASRC_DIR := $(ADSP_DIR)/external/ASRC_ARM
ifeq ($(CFG_HI_CPU_ARCH),arm)
EXTRA_CFLAGS += -I$(ASRC_DIR)/inc
EXTRA_CFLAGS += -DHI_SND_IMEDIA_SRC_SUPPORT
endif

EXTRA_CFLAGS += -DHI_SOUND_SPDIF_COMPENSATION_SUPPORT

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_adsp

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   := \
	drv_adsp_intf.o \
	drv_adsp.o \
	osal/adsp_osal.o

$(MOD_NAME)-y += $(HIFIDIR)/common/linear/linear.o
$(MOD_NAME)-y += $(HIFIDIR)/common/mem/dsp_mem.o
$(MOD_NAME)-y += $(HIFIDIR)/common/resampler/resample_rom.o
$(MOD_NAME)-y += $(HIFIDIR)/common/resampler/resample.o
$(MOD_NAME)-y += $(HIFIDIR)/common/util/dsp_util.o
$(MOD_NAME)-y += $(HIFIDIR)/common/ringbuf/dsp_ringbuf.o
$(MOD_NAME)-y += $(HIFIDIR)/common/basicop/basicop.o
$(MOD_NAME)-y += $(HIFIDIR)/common/timer/system_timer.o

$(MOD_NAME)-y += $(HIFIDIR)/sw_main.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/aop/aoe_aop.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/aop/aoe_cast.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/aip/aoe_aip.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/engine/aoe_engine.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/aoe_main.o
$(MOD_NAME)-y += $(HIFIDIR)/aoe/src/aoe_reg.o

ifeq ($(O),)
ADSP_EXT_LIB_DIR := $(ASRC_DIR)/lib
else
ADSP_EXT_LIB_DIR := $(O)/drivers/msp/adsp/adsp_v1_1/external/ASRC_ARM/lib
endif

ifeq ($(CFG_HI_CPU_ARCH),arm)
$(shell mkdir -p $(ADSP_EXT_LIB_DIR))
$(shell cp $(ASRC_DIR)/lib/libimedia_asrc_arma9.lib $(ADSP_EXT_LIB_DIR)/libimedia_asrc_arma9.a)

$(MOD_NAME)-y += ./external/ASRC_ARM/lib/libimedia_asrc_arma9.a
endif

REMOVED_FILES := "*.o" "*.ko" "*.a" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

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
