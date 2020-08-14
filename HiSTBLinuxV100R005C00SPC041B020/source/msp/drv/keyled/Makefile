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
DRV_SRC_BASE=$(MSP_DIR)/drv/keyled

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(DRV_SRC_BASE)                       \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(COMMON_DIR)/drv/sys

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_keyled

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

ifeq ($(CFG_HI_KEYLED_CT1642_KERNEL_SUPPORT),y)
$(MOD_NAME)-y   :=  drv_keyled_init_ct1642.o
endif

$(MOD_NAME)-y   +=  drv_keyled_intf.o


ifeq ($(CFG_HI_KEYLED_74HC164_SUPPORT), y)
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_std
    EXTRA_CFLAGS += -DKEYLED_STANDARD
    $(MOD_NAME)-y  += keyled_std/drv_keyled_std.o
endif

ifeq ($(CFG_HI_KEYLED_GPIOKEY_SUPPORT), y)
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_gpiokey
    EXTRA_CFLAGS += -DKEYLED_GPIOKEY
    $(MOD_NAME)-y  += keyled_gpiokey/drv_keyled_gpiokey.o
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_POWER_GPIO=$(CFG_HI_KEYLED_GPIOKEY_POWER_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_OK_GPIO=$(CFG_HI_KEYLED_GPIOKEY_OK_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_MENU_GPIO=$(CFG_HI_KEYLED_GPIOKEY_MENU_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_UP_GPIO=$(CFG_HI_KEYLED_GPIOKEY_UP_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_DOWN_GPIO=$(CFG_HI_KEYLED_GPIOKEY_DOWN_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_LEFT_GPIO=$(CFG_HI_KEYLED_GPIOKEY_LEFT_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_GPIOKEY_RIGHT_GPIO=$(CFG_HI_KEYLED_GPIOKEY_RIGHT_GPIO)
endif

ifeq ($(CFG_HI_KEYLED_FD650_SUPPORT), y)
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_fd650
    EXTRA_CFLAGS += -DKEYLED_FD650
    $(MOD_NAME)-y  += keyled_fd650/drv_keyled_fd650.o
ifdef CFG_HI_KEYLED_FD650_CLOCK_GPIO
    EXTRA_CFLAGS += -DHI_KEYLED_FD650_CLOCK_GPIO=$(CFG_HI_KEYLED_FD650_CLOCK_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_FD650_DINOUT_GPIO=$(CFG_HI_KEYLED_FD650_DINOUT_GPIO)
endif
endif

ifeq ($(CFG_HI_KEYLED_PT6961_SUPPORT), y)
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_pt6961
    EXTRA_CFLAGS += -DKEYLED_PT6961
ifdef CFG_HI_KEYLED_PT6961_CLOCK_GPIO
    EXTRA_CFLAGS += -DHI_KEYLED_PT6961_CLOCK_GPIO=$(CFG_HI_KEYLED_PT6961_CLOCK_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_PT6961_STB_GPIO=$(CFG_HI_KEYLED_PT6961_STB_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_PT6961_DIN_GPIO=$(CFG_HI_KEYLED_PT6961_DIN_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_PT6961_DOUT_GPIO=$(CFG_HI_KEYLED_PT6961_DOUT_GPIO)
endif
    $(MOD_NAME)-y  += keyled_pt6961/drv_keyled_pt6961.o
endif

ifeq ($(CFG_HI_KEYLED_PT6964_SUPPORT), y)
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_pt6964
    $(MOD_NAME)-y += keyled_pt6964/drv_keyled_pt6964.o
    EXTRA_CFLAGS += -DKEYLED_PT6964
ifdef CFG_HI_KEYLED_PT6964_CLOCK_GPIO
    EXTRA_CFLAGS += -DHI_KEYLED_PT6964_CLOCK_GPIO=$(CFG_HI_KEYLED_PT6964_CLOCK_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_PT6964_STB_GPIO=$(CFG_HI_KEYLED_PT6964_STB_GPIO)
    EXTRA_CFLAGS += -DHI_KEYLED_PT6964_DINOUT_GPIO=$(CFG_HI_KEYLED_PT6964_DINOUT_GPIO)
endif
endif

ifeq ($(CFG_HI_KEYLED_CT1642_SUPPORT), y)
    EXTRA_CFLAGS += -DKEYLED_CT1642
    EXTRA_CFLAGS += -I$(DRV_SRC_BASE)/keyled_ct1642
    ifneq ($(CFG_HI_KEYLED_CT1642_GPIO_MODE),y)
        $(MOD_NAME)-y  += keyled_ct1642/drv_keyled_ct1642_inner.o
    else
    ifdef CFG_HI_KEYLED_CT1642_CLOCK_GPIO
        EXTRA_CFLAGS += -DHI_KEYLED_CT1642_CLOCK_GPIO=$(CFG_HI_KEYLED_CT1642_CLOCK_GPIO)
        EXTRA_CFLAGS += -DHI_KEYLED_CT1642_DAT_GPIO=$(CFG_HI_KEYLED_CT1642_DAT_GPIO)
        EXTRA_CFLAGS += -DHI_KEYLED_CT1642_KEY_GPIO=$(CFG_HI_KEYLED_CT1642_KEY_GPIO)
    endif
        $(MOD_NAME)-y  += keyled_ct1642/drv_keyled_ct1642.o
    endif
endif

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

