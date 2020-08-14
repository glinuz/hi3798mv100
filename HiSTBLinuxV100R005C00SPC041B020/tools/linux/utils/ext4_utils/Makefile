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

# parameter
PREFIX      ?= $(shell pwd)
RULEATTR    := $(CURDIR)/../ruleattr
RULEATTR_LIB ?= $(PREFIX)/../ruleattr/libruleattr.a

BUILD_DIR   := $(PREFIX)/tmp
STRIP       := strip
ZLIB        ?= $(shell pwd)/../zlib/libz.a
LIBSPARSE   := libsparse
LIBSELINUX  := libselinux
EXT4_UTILS  := ext4_utils
PHONYS      :=
################################################################################

$(PREFIX)/make_ext4fs: $(BUILD_DIR)/bin/make_ext4fs
	@ test -d $(D) || install -m 755 -d $(@D)
	@ install -m 0755 $< $(@D)
	@ $(STRIP) $@

$(BUILD_DIR)/bin/make_ext4fs: \
    $(LIBSELINUX) $(LIBSPARSE) \
    $(ZLIB) prepare
	make -C $(BUILD_DIR)/$(EXT4_UTILS) \
		LDFLAGS="-L$(BUILD_DIR)/lib -L$(dir $(RULEATTR_LIB))  -L$(dir $(ZLIB)) -static" \
		CFLAGS="-I$(BUILD_DIR)/include -I$(RULEATTR) -m32" \
		PREFIX=$(BUILD_DIR) install

PHONYS += $(LIBSELINUX)
$(LIBSELINUX):
	@test -d $(BUILD_DIR)/ || mkdir -p $(BUILD_DIR)/
	test -d $(BUILD_DIR)/$(LIBSELINUX) || tar -xjf $(OPEN_SOURCE_DIR)/$(LIBSELINUX).tar.bz2 -C $(BUILD_DIR)
	cp $(LIBSELINUX).patch/* $(BUILD_DIR)/$(LIBSELINUX) -rf
	make -C $(BUILD_DIR)/$(LIBSELINUX) CC=gcc CFLAGS=-m32 LDFLAGS="-static -L$(BUILD_DIR)/lib"
	make -C $(BUILD_DIR)/$(LIBSELINUX) CC=gcc PREFIX=$(BUILD_DIR) install

PHONYS += $(LIBSPARSE)
$(LIBSPARSE): $(ZLIB)
	@test -d $(BUILD_DIR)/ || mkdir -p $(BUILD_DIR)/
	$(AT)tar -xjf $(OPEN_SOURCE_DIR)/$(LIBSPARSE).tar.bz2 -C $(BUILD_DIR)
	make -C $(BUILD_DIR)/$(LIBSPARSE) CFLAGS=-m32 LDFLAGS="-static -L$(BUILD_DIR)/lib -L$(dir $(ZLIB))"
	make -C $(BUILD_DIR)/$(LIBSPARSE) PREFIX=$(BUILD_DIR) install
	cp $(BUILD_DIR)/$(LIBSPARSE)/simg2img $(PREFIX)

################################################################################

clean:
	@rm -rf $(BUILD_DIR) $(PREFIX)/make_ext4fs $(PREFIX)/simg2img

distclean: clean

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(EXT4_UTILS) || tar -xjf $(OPEN_SOURCE_DIR)/$(EXT4_UTILS).tar.bz2 -C $(BUILD_DIR)
	$(AT)cp -rf $(EXT4_UTILS).patch/. $(BUILD_DIR)/$(EXT4_UTILS)/

################################################################################
.PHONY: clean $(PHONYS)
################################################################################
