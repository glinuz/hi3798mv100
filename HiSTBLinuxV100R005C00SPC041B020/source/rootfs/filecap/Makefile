#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
ifneq ($(HI_OUT_DIR),)
TOP_DIR := $(HI_OUT_DIR)/obj/source/rootfs/filecap
else
TOP_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

BUILD_DIR      := $(TOP_DIR)/tmp
HOST           := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP          := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip
LIBCAP         := libcap-ng-0.7.7
ATTR           := attr-2.4.47
TARGETS        := bin/filecap lib/libcap-ng.so.0

################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/bin/filecap: $(BUILD_DIR)/bin/filecap
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(dir $@) $<

$(PREFIX)/lib/libcap-ng.so.0: $(BUILD_DIR)/lib/libcap-ng.so.0.0.0
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)cp -fv $< $@
	$(STRIP) $@

################################################################################

$(BUILD_DIR)/bin/filecap \
$(BUILD_DIR)/lib/libcap-ng.so.0.0.0: $(BUILD_DIR)/lib/libattr.a
	cd $(TOP_DIR)/$(LIBCAP) && \
		CFLAGS="-I$(BUILD_DIR)/include -g -O2" LFLAGS="-L$(BUILD_DIR)/lib/" \
		./configure --host=$(HOST) --prefix=$(BUILD_DIR) \
			--without-python && \
		make && \
		make install && \
	cd -

$(BUILD_DIR)/lib/libattr.a: prepare
	cd $(TOP_DIR)/$(ATTR) && \
		./configure --host=$(HOST) --prefix=$(BUILD_DIR) && \
		make -j 16   && \
		make install-dev && \
	cd -

################################################################################

prepare:
	$(AT)test -d $(TOP_DIR) || mkdir -p $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(LIBCAP)  || tar -xjf $(OPEN_SOURCE_DIR)/$(LIBCAP).tar.bz2 -C $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(ATTR)    || tar -xjf $(OPEN_SOURCE_DIR)/$(ATTR).tar.bz2 -C $(TOP_DIR)

################################################################################

clean:
	rm -rf $(TOP_DIR)/$(LIBCAP) $(TOP_DIR)/$(ATTR) $(BUILD_DIR)

distclean: clean

install:
	$(AT)echo "not support"

################################################################################
.PHONY: clean distclean install
################################################################################

