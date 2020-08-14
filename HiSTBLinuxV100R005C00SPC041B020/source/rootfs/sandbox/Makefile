################################################################################
#
#  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
#
#  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
#  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
#  others, published or used, in whole or in part, without the express prior
#  written permission of Hisilicon.
#
#  Create By CaiZhiyong 2014.09.13
#
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifneq ($(HI_OUT_DIR),)
TOP_DIR := $(HI_OUT_DIR)/obj/source/rootfs/sandbox
else
TOP_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

################################################################################
BUILD_DIR      := $(TOP_DIR)/tmp
HOST           := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP          := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

COREUTILS      := coreutils-8.23

TARGETS        := bin/sandbox

################################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/%: $(BUILD_DIR)/%
	test -d $(@D) || mkdir -p $(@D)
	cp -fv $(BUILD_DIR)/bin/chroot $@

################################################################################
$(addprefix $(BUILD_DIR)/,$(TARGETS)): $(TOP_DIR)/$(COREUTILS)/build_finish

$(TOP_DIR)/$(COREUTILS)/build_finish: prepare
	$(AT)cd $(TOP_DIR)/$(COREUTILS) && ./configure --prefix=$(BUILD_DIR) --host=$(HOST) --enable-no-install-program=stdbuf LDFLAGS="-static -pthread"  --disable-nls
	$(AT)make -C $(TOP_DIR)/$(COREUTILS) -j
	$(AT)make -C $(TOP_DIR)/$(COREUTILS) install
	$(AT)touch $@

################################################################################

prepare:
	$(AT)test -d $(TOP_DIR) || mkdir -p $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(COREUTILS) || tar xf $(OPEN_SOURCE_DIR)/$(COREUTILS).tar.bz2 -C $(TOP_DIR)
ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE,$(CFG_HI_CFLAGS)),-DHI_ADVCA_FUNCTION_RELEASE)
	tar -xf sandbox_release_patch.tar.gz -C $(TOP_DIR)/ && cp -afr $(TOP_DIR)/sandbox_release_patch/* $(TOP_DIR)/$(COREUTILS)/ && \
		rm -fr $(TOP_DIR)/sandbox_release_patch/
endif

################################################################################

clean:
	$(AT)rm -rf $(TOP_DIR)/$(COREUTILS) $(BUILD_DIR)

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################
