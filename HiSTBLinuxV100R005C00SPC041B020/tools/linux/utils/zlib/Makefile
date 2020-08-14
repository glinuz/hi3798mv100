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
ZLIB           := zlib-1.2.8
################################################################################

all: $(BUILD_DIR)/lib/libz.a

$(BUILD_DIR)/lib/libz.a: prepare
	cd $(BUILD_DIR)/$(ZLIB) && \
		./configure --prefix="$(BUILD_DIR)" && \
		make -k CC=gcc CFLAGS="-m32" LDFLAGS="-static" 2>&1>/dev/null && \
		make -k install CC=gcc 2>&1>/dev/null && \
	cd -

################################################################################

clean:
	$(AT)rm -rf $(BUILD_DIR)

distclean: clean

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(ZLIB) || tar -xf $(OPEN_SOURCE_DIR)/$(ZLIB).tar.gz -C $(BUILD_DIR)

################################################################################
.PHONY: clean distclean all prepare
################################################################################
