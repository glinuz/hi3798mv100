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

#=======================================
#	local variable
#=======================================

ifeq ($(CFG_HI_GPU_MALI450_SUPPORT),y)
objects := mali4xx
else ifeq ($(CFG_HI_GPU_MALI400_SUPPORT),y)
objects := mali4xx
else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
objects := mali700
else
$(error error: mali unknown)
endif

objects_clean       := ${addsuffix _clean,${objects}}
objects_install     := ${addsuffix _install,${objects}}
objects_uninstall   := ${addsuffix _uninstall,${objects}}

EXTRA_CFLAGS += $(filter-out -Wformat-security,$(CFG_HI_KMOD_CFLAGS))
KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

obj-y += $(objects)/

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#=================================================================
#	  rules
#=================================================================
.PHONY: all clean install uninstall ${objects} ${objects_clean} ${objects_install} ${objects_uninstall}

all: ${objects}

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
ifeq ($(CFG_HI_GPU_MALI450_SUPPORT),y)
	@$(AT)cp -f $(CURDIR)/mali4xx/mali.ko $(HI_MODULE_DIR)
else ifeq ($(CFG_HI_GPU_MALI400_SUPPORT),y)
	@$(AT)cp -f $(CURDIR)/mali4xx/mali.ko $(HI_MODULE_DIR)
else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
	@$(AT)cp -f $(CURDIR)/mali700/drivers/base/kds/kds.ko $(HI_MODULE_DIR)
	@$(AT)cp -f $(CURDIR)/mali700/drivers/base/hi_dbe/hi_dbe.ko $(HI_MODULE_DIR)
	@$(AT)cp -f $(CURDIR)/mali700/drivers/gpu/arm/midgard/mali_kbase.ko $(HI_MODULE_DIR)
else
$(error error: mali unknown)
endif

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/mali.ko
	$(AT)rm -rf $(HI_MODULE_DIR)/kds.ko
	$(AT)rm -rf $(HI_MODULE_DIR)/hi_dbe.ko
	$(AT)rm -rf $(HI_MODULE_DIR)/mali_kbase.ko

${objects}:
	make -C $@

${objects_clean}:
	make -C ${patsubst %_clean,%,$@} clean

${objects_install}:
	make -C ${patsubst %_install,%,$@} install

${objects_uninstall}:
	make -C ${patsubst %_uninstall,%,$@} uninstall
