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

ifeq ($(wildcard $(MSP_DIR)/drv/otp/otp_cfg.mak), $(MSP_DIR)/drv/otp/otp_cfg.mak)
include $(MSP_DIR)/drv/otp/otp_cfg.mak
endif

#===============================================================================
# local variables
#===============================================================================
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(MSP_UNF_INCLUDE)            \
                -I$(MSP_DRV_INCLUDE)            \
                -I$(COMMON_UNF_INCLUDE)         \
                -I$(COMMON_DRV_INCLUDE)         \
                -I$(MSP_DIR)/drv/otp            \
                -I$(MSP_DIR)/drv/otp/otpv200

ifeq ($(SDK_OTP_ARCH_VERSION), SDK_OTP_ARCH_VERSION_V3)
EXTRA_CFLAGS += -DSDK_OTP_ARCH_VERSION_V3
endif

#EXTRA_CFLAGS += -DSDK_OTP_VIRTUAL_DEBUG
KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_otp

obj-$(HI_DRV_BUILDTYPE) := $(MOD_NAME).o

$(MOD_NAME)-y := ./drv_otp.o ./drv_otp_intf.o ./drv_otp_common.o ./otpv200/hal_otp_v200.o
$(MOD_NAME)-y += ./otpv200/drv_otp_v200.o

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY:clean all install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/hi_otp.ko $(HI_MODULE_DIR)/hi_otp.ko

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/hi_otp.ko

