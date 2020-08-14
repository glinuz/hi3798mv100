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
# local variable
#===============================================================================
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)             \
                -I$(COMMON_API_INCLUDE)             \
                -I$(COMMON_DRV_INCLUDE)             \
                -I$(MSP_UNF_INCLUDE)                \
                -I$(MSP_API_INCLUDE)                \
                -I$(MSP_DRV_INCLUDE)                \
                -I$(MSP_DIR)/drv/advca              \
                -I$(MSP_DIR)/drv/advca/runtime      \
                -I$(MSP_DIR)/drv/advca/V300         \
                -I$(MSP_DIR)/drv/advca/ca_otp       \
                -I$(MSP_DIR)/drv/advca/ca_otp/ca_otpv200

DDR_WAKE_UP_CHECK_BIN = $(MSP_DIR)/drv/advca/ddr_wakeup_check/ddr_wakeup_check.bin
EXTRA_AFLAGS += -DWAKE_UP_BIN=\"$(DDR_WAKE_UP_CHECK_BIN)\"

ifeq ($(CFG_HI_64BIT_SYSTEM),y)
EXTRA_CFLAGS += -DDDR_WAKEUP_SRC=\"ddr_wakeup_check/output_64.txt\"
else
EXTRA_CFLAGS += -DDDR_WAKEUP_SRC=\"ddr_wakeup_check/output_32.txt\"
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
EXTRA_AFLAGS += -DHI_TEE_SUPPORT
endif

MOD_NAME := hi_advca

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y :=    drv_advca_intf.o \
                    drv_advca.o \
                    drv_advca_common.o \
                    V300/hal_advca_v300.o \
                    V300/drv_advca_v300.o \
                    ca_otp/ca_otpv200/drv_ca_otp_v200.o \
                    runtime/sha1.o \
                    runtime/runtime_module.o

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
# rules
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
