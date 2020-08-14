#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
        KERNEL_DIR := $(srctree)

        SDK_DIR := $(KERNEL_DIR)/../../..
    else
        SDK_DIR := $(CURDIR)/../../../../..
    endif

    include $(SDK_DIR)/base.mak
endif

##########################################################################################
#	prepare param
##########################################################################################
export CUR_DIR=$(shell pwd)
export ARCH=$(CFG_HI_CPU_ARCH)
export KDIR=$(LINUX_DIR)
export CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)-

include kbuild_flags

##########################################################################################
#	set task
##########################################################################################
all: 
	@rm -f $(CUR_DIR)/drivers/base/kds/kds.ko
	@rm -f $(CUR_DIR)/drivers/base/hi_dbe/hi_dbe.ko
	@rm -f $(CUR_DIR)/drivers/gpu/arm/midgard/mali_kbase.ko
	make -C $(CUR_DIR)/drivers/base/kds
	make -C $(CUR_DIR)/drivers/base/hi_dbe
	make -C $(CUR_DIR)/drivers/gpu/arm/midgard
	

clean: 
	make -C $(CUR_DIR)/drivers/base/kds/ clean
	make -C $(CUR_DIR)/drivers/base/hi_dbe/ clean
	make -C $(CUR_DIR)/drivers/gpu/arm/midgard/ clean

