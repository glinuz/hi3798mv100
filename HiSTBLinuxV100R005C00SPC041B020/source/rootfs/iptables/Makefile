#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
ifneq ($(HI_OUT_DIR),)
TOP_DIR := $(HI_OUT_DIR)/obj/source/rootfs/iptables
else
TOP_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

BUILD_DIR      := $(TOP_DIR)/tmp
HOST           := $(CFG_HI_ARM_TOOLCHAINS_NAME)

IPTABLES       := iptables-1.4.21

TARGETS        := sbin/iptables sbin/xtables-multi
################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/sbin/xtables-multi: $(BUILD_DIR)/sbin/xtables-multi
	@test -d $(@D) || mkdir -p $(@D)
	@install -v -t $(@D) $<

$(PREFIX)/sbin/iptables: $(PREFIX)/sbin/xtables-multi
	@cd $(@D) && \
		ln -sf $(<F) $(@F) && \
	cd -

################################################################################

$(BUILD_DIR)/sbin/xtables-multi: prepare
	@cd $(TOP_DIR)/$(IPTABLES) && \
		./configure --host=$(HOST) --prefix=$(BUILD_DIR) \
			-enable-static -disable-shared \
			--datarootdir=$(BUILD_DIR)     \
			--includedir=$(BUILD_DIR)&& \
		make -j 16 && \
		make install && \
	cd -

################################################################################

prepare:
	$(AT)test -d $(TOP_DIR) || mkdir -p $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(IPTABLES) || tar -xjf $(OPEN_SOURCE_DIR)/$(IPTABLES).tar.bz2 -C $(TOP_DIR)

################################################################################

clean:
	$(AT)rm -rf $(TOP_DIR)/$(IPTABLES) $(BUILD_DIR)

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################
