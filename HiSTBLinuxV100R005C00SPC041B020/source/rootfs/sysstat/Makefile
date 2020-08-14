#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifneq ($(HI_OUT_DIR),)
TOP_DIR := $(HI_OUT_DIR)/obj/source/rootfs/sysstat
else
TOP_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

################################################################################
BUILD_DIR      := $(TOP_DIR)/tmp
HOST           := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP          := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip
SYSSTAT        := sysstat-10.0.5
TARGETS        := sbin/mpstat sbin/sar sbin/iostat sbin/sadc sbin/sadf

################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/sbin/%: $(TOP_DIR)/$(SYSSTAT)/%
	@test -d $(@D) || mkdir -p $(@D)
	@install -vs -t $(@D) --strip-program=$(STRIP) $<

$(addprefix $(TOP_DIR)/$(SYSSTAT)/,$(notdir $(TARGETS))): prepare
	cd $(TOP_DIR)/$(SYSSTAT) && \
		./configure --host=$(HOST) --prefix=$(BUILD_DIR) \
			--exec-prefix=$(BUILD_DIR) CFLAGS="-static" && \
		make -j 16 && \
	cd -

################################################################################

prepare:
	$(AT)test -d $(TOP_DIR) || mkdir -p $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(SYSSTAT) || tar -xf $(OPEN_SOURCE_DIR)/$(SYSSTAT).tar.gz -C $(TOP_DIR)/

################################################################################

clean:
	$(AT)rm -rf $(TOP_DIR)/$(SYSSTAT)

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################

