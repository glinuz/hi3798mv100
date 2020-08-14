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
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
VFMW_DIR = vfmw_v5.0
else
VFMW_DIR = vfmw_v4.0
endif

obj-y += $(VFMW_DIR)/

ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
obj-y += softlib/
endif

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(VFMW_DIR)/ all
ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
	$(AT)make -C softlib/ all
endif

clean:
	$(AT)make -C $(VFMW_DIR)/ clean
ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
	$(AT)make -C softlib/ clean
endif

install: all
	$(AT)make -C $(VFMW_DIR)/ install
ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
	$(AT)make -C softlib/ install
endif

uninstall:
	$(AT)make -C $(VFMW_DIR)/ uninstall
ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
	$(AT)make -C softlib/ uninstall
endif

