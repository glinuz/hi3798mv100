#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../.. && /bin/pwd)
else
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
endif
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS += -I$(COMPONENT_DIR)/loader/app/common
CFLAGS += -I$(COMPONENT_DIR)/loader/app/ca/vmx/include
CFLAGS += -I$(COMPONENT_DIR)/loader/app/ca/vmx/vmx_loader_verify_lib

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
CFLAGS += -DHI_LOADER_BOOTLOADER
endif

ifeq ($(CFG_HI_LOADER_APPLOADER),y)
CFLAGS += -DHI_LOADER_APPLOADER
endif

SRCS := api/vmx_config.c
SRCS += api/vmx_ssd.c 

#=================================================================
#     rules
#=================================================================
LIB_NAME := libhi_ca_ssd

include $(SDK_DIR)/hi_rules.mak

ssd_install: prebuild all

ssd_clean: preclean clean

prebuild:
	$(AT)make -C vmx_loader_verify_lib all

preclean:
	$(AT)make -C vmx_loader_verify_lib clean
	
