#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifneq ($(HI_OUT_DIR),)
PPPD_DIR := $(HI_OUT_DIR)/obj/source/rootfs/pppd
else
PPPD_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

################################################################################
PPPSRC := ppp-2.4.7

#STATIC := "-static"

HOST    := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP   := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

TARGETS := \
    usr/local/sbin/chat \
    usr/local/sbin/pppd \
    usr/local/sbin/pppstats \
    usr/local/sbin/pppdump \
    usr/local/sbin/pppoe-discovery

install: all
	$(AT)cd $(PPPD_DIR)/$(PPPSRC) && \
		make CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc INSTROOT=$(PREFIX) install && \
	cd -

################################################################################
.PHONY: all clean
################################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))

clean:
	$(AT)rm -rf $(PPPD_DIR)/$(PPPSRC)

$(addprefix $(PREFIX)/,$(TARGETS)): $(PPPD_DIR)/$(PPPSRC)/tar
	$(AT)cd $(PPPD_DIR)/$(PPPSRC) && make CC=$(HOST)-gcc INSTROOT=$(PREFIX) STATIC=$(STATIC) all

$(PPPD_DIR)/$(PPPSRC)/tar:
	$(AT)test -d $(PPPD_DIR) || mkdir -p $(PPPD_DIR)
	$(AT)test -d $(PPPD_DIR)/$(PPPSRC) || tar -xf $(OPEN_SOURCE_DIR)/$(PPPSRC).tar.bz2 -C $(PPPD_DIR)
	$(AT)cd $(PPPD_DIR)/$(PPPSRC) && ./configure
	$(AT)touch $@
