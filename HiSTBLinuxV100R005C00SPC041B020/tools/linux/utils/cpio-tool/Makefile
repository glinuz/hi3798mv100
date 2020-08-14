################################################################################
#
#  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
#
#  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
#  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
#  others, published or used, in whole or in part, without the express prior
#  written permission of Hisilicon.
#
#  Create By CaiZhiyong 2014.07.11
#
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
# parameter
PREFIX      ?= $(shell pwd)
RULEATTR    := $(CURDIR)/../ruleattr
RULEATTR_LIB ?= $(PREFIX)/../ruleattr/libruleattr.a

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
CPIO           := cpio-2.12
################################################################################

all: $(PREFIX)/cpio

$(PREFIX)/cpio: $(BUILD_DIR)/$(CPIO)/src/cpio
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<

$(BUILD_DIR)/$(CPIO)/src/cpio: $(BUILD_DIR)/$(CPIO)/patch
	cd $(BUILD_DIR)/$(CPIO) && \
		./configure CFLAGS="-I$(RULEATTR) -static -m32" \
			LIBS="-lruleattr" \
			LDFLAGS="-L$(dir $(RULEATTR_LIB)) -static" && \
		make && \
	cd -

$(BUILD_DIR)/$(CPIO)/patch: \
    $(BUILD_DIR)/$(CPIO)/tar
	@cp -rf $(CPIO).patch/. $(BUILD_DIR)/$(CPIO)
	@touch $@
###############################################################################

clean:
	@test ! -d $(BUILD_DIR) || rm -rfv $(BUILD_DIR)
	@test ! -e $(PREFIX)/cpio || rm -rfv $(PREFIX)/cpio

distclean: clean

$(BUILD_DIR)/$(CPIO)/tar:
	@test -d $(@D) || mkdir -p $(@D)
	$(AT)tar -xjf $(OPEN_SOURCE_DIR)/$(CPIO).tar.bz2 -C $(BUILD_DIR)
	@touch $@

###############################################################################
.PHONY: clean distclean all
###############################################################################
