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
 
CFLAGS += -I$(COMPONENT_DIR)/loader/app/ca/dcas/include

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
CFLAGS += -DHI_LOADER_BOOTLOADER
endif

ifeq ($(CFG_HI_LOADER_APPLOADER),y)
CFLAGS += -DHI_LOADER_APPLOADER
endif

SRCS := dcas_ssd.c
SRCS += dcas_flash.c
SRCS += dcas_cipher.c
SRCS += dcas_verify.c

LIB_NAME := libhi_ca_ssd

include $(SDK_DIR)/hi_rules.mak

ssd_install: prebuild all

ssd_clean: preclean clean

prebuild:
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/include/dcas_config.h $(COMPONENT_DIR)/loader/app/ca/dcas/include/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/include/dcas_flash.h $(COMPONENT_DIR)/loader/app/ca/dcas/include/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/include/dcas_debug.h $(COMPONENT_DIR)/loader/app/ca/dcas/include/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/include/dcas_verify.h $(COMPONENT_DIR)/loader/app/ca/dcas/include/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/include/dcas_cipher.h $(COMPONENT_DIR)/loader/app/ca/dcas/include/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/dcas_cipher.c $(COMPONENT_DIR)/loader/app/ca/dcas/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/dcas_verify.c $(COMPONENT_DIR)/loader/app/ca/dcas/
	$(AT)cp -rf $(SOURCE_DIR)/boot/product/ssa/dcas/dcas_flash.c $(COMPONENT_DIR)/loader/app/ca/dcas/
	
preclean:
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/include/dcas_config.h
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/include/dcas_flash.h
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/include/dcas_debug.h
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/include/dcas_verify.h
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/include/dcas_cipher.h
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/dcas_cipher.c
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/dcas_verify.c
	$(AT)rm $(COMPONENT_DIR)/loader/app/ca/dcas/dcas_flash.c


	