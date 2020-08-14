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
EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/drv/adsp/adsp_v1_1/include \
                -I$(MSP_DIR)/api/jpge/include           \
                -I$(MSP_DIR)/api/tde/include            \
                -I$(MSP_DIR)/drv/pm			\
                -I$(COMPONENT_DIR)/pm/c51

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
EXTRA_CFLAGS += -DHI_KEYLED_SUPPORT
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_pmoc

$(MOD_NAME)-y   :=  drv_pmoc_intf.o drv_pmoc_smartstandby.o

ifeq ($(CFG_HI_DVFS_CPU_SUPPORT),y)
EXTRA_CFLAGS += -DHI_DVFS_CPU_SUPPORT

$(MOD_NAME)-y   +=  clock_mpu.o     \
                    hi_opp_data.o   \
                    opp.o           \
                    hi_dvfs.o       \
                    clock.o         \
                    hi_cpufreq.o
endif

ifeq ($(CFG_HI_PMU_DEVICE_SELECT),y)
EXTRA_CFLAGS += -DHI_PMU_DEVICE_SELECT
endif

ifeq ($(CFG_HI_DONGLE_MODE_NORMAL),y)
EXTRA_CFLAGS += -DHI_DONGLE_MODE_NORMAL
endif

ifeq ($(CFG_HI_DONGLE_MODE_LOW),y)
EXTRA_CFLAGS += -DHI_DONGLE_MODE_LOW
endif

ifeq ($(CFG_HI_DONGLE_MODE_CRITICAL),y)
EXTRA_CFLAGS += -DHI_DONGLE_MODE_CRITICAL
endif

ifeq ($(CFG_HI_AVS_SUPPORT),y)
EXTRA_CFLAGS += -DHI_AVS_SUPPORT
endif

ifeq ($(CFG_HI_AI_SUPPORT),y)
EXTRA_CFLAGS += -DHI_AUDIO_AI_SUPPORT
endif

ifeq ($(CFG_HI_GPIO_LIGHT_SUPPORT), y)
EXTRA_CFLAGS += -DHI_GPIO_LIGHT_SUPPORT
EXTRA_CFLAGS += -DHI_POSITIVE_GPIO_LIGHT_NUMBER=$(CFG_HI_POSITIVE_GPIO_LIGHT_NUMBER)
EXTRA_CFLAGS += -DHI_NEGATIVE_GPIO_LIGHT_NUMBER=$(CFG_HI_NEGATIVE_GPIO_LIGHT_NUMBER)

ifeq ($(CFG_HI_POSITIVE_GPIO_LIGHT_NUMBER_QFP),)
EXTRA_CFLAGS += -DHI_POSITIVE_GPIO_LIGHT_NUMBER_QFP=0x2A
else
EXTRA_CFLAGS += -DHI_POSITIVE_GPIO_LIGHT_NUMBER_QFP=$(CFG_HI_POSITIVE_GPIO_LIGHT_NUMBER_QFP)
endif

endif

ifeq ($(CFG_HI_PM_POWERUP_MODE1_SUPPORT), y)
EXTRA_CFLAGS += -DHI_PM_POWERUP_MODE1_SUPPORT
endif

ifeq ($(CFG_HI_TEMP_CTRL_CONFIG), y)
    EXTRA_CFLAGS += -DHI_TEMP_CTRL_CONFIG
    EXTRA_CFLAGS += -DHI_TEMP_CTRL_DOWN_THRESHOLD=$(CFG_HI_TEMP_CTRL_DOWN_THRESHOLD)
    EXTRA_CFLAGS += -DHI_TEMP_CTRL_UP_THRESHOLD=$(CFG_HI_TEMP_CTRL_UP_THRESHOLD)
    EXTRA_CFLAGS += -DHI_TEMP_CTRL_REBOOT_THRESHOLD=$(CFG_HI_TEMP_CTRL_REBOOT_THRESHOLD)
endif

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

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

