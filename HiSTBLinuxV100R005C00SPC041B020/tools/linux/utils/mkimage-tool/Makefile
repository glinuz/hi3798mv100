#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../..
include $(SDK_DIR)/base.mak
endif

PREFIX         ?= $(shell pwd)

################################################################################
#
#  options below should be redefined here, don't use definitions in Makefile
#  in SDK top dir.
#
CC=gcc
OBJDUMP=objdump
HOST=
CPP=
LD=
NM=

###############################################################################
BUILD_DIR      := $(PREFIX)/tmp
STRIP          := strip
UBOOT_TOOLS    := fastboot
################################################################################

all: $(PREFIX)/mkimage

$(PREFIX)/mkimage: $(BUILD_DIR)/$(UBOOT_TOOLS)/tools/mkimage
	$(AT)cp -rf $(BUILD_DIR)/$(UBOOT_TOOLS)/tools/mkimage $(PREFIX)

$(BUILD_DIR)/$(UBOOT_TOOLS)/tools/mkimage: prepare
	$(AT)cd $(BUILD_DIR)/$(UBOOT_TOOLS) && make tools CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)- O= && cd -

################################################################################

clean:
	$(AT)rm -rf $(BUILD_DIR) $(PREFIX)/mkimage

distclean: clean

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(UBOOT_TOOLS) || tar -xjf $(OPEN_SOURCE_DIR)/$(UBOOT_TOOLS).tar.bz2 -C $(BUILD_DIR)

################################################################################
.PHONY: clean distclean all prepare
################################################################################
