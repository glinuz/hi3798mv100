#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX ?= $(HI_ROOTFS_DIR)

ifneq ($(HI_OUT_DIR),)
SSH_DIR := $(HI_OUT_DIR)/obj/source/rootfs/dropbear
else
SSH_DIR := $(CURDIR)
endif

################################################################################
HOST    := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP   := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

SSH    := dropbear-2020.80
TARGETS := dropbear

################################################################################
all: $(addprefix $(PREFIX)/sbin/,$(TARGETS))

$(PREFIX)/sbin/%: $(SSH_DIR)/$(SSH)/sbin/%
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(@D) $<

$(addprefix $(SSH_DIR)/$(SSH)/sbin/,$(TARGETS)): prepare
	$(AT)make -C $(SSH_DIR)/$(SSH) USE_STATIC=false

################################################################################
clean:
	$(AT)rm -rf $(SSH_DIR)/$(SSH)

################################################################################
prepare:
	$(AT)test -d $(SSH_DIR) || mkdir -p $(SSH_DIR)
	$(AT)test -d $(SSH_DIR)/$(SSH) || (tar xf $(OPEN_SOURCE_DIR)/$(SSH).tar.bz2 -C $(SSH_DIR); \
        cd $(SSH_DIR)/$(SSH); CC=$(HOST)-gcc; ./configure --prefix=$(SSH_DIR) --host=$(HOST) \
        --with-zlib=$(HI_OUT_DIR)/obj/source/component/zlib/zlib-1.2.8 

################################################################################
.PHONY: install clean distclean
################################################################################
