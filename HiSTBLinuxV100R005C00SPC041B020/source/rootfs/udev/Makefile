#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX ?= $(HI_ROOTFS_DIR)

ifneq ($(HI_OUT_DIR),)
UDEV_DIR := $(HI_OUT_DIR)/obj/source/rootfs/udev
else
UDEV_DIR := $(CURDIR)
endif

################################################################################
HOST    := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP   := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

UDEV    := udev-167
TARGETS := udevd udevadm

################################################################################
all: $(addprefix $(PREFIX)/bin/,$(TARGETS))

$(PREFIX)/bin/%: $(UDEV_DIR)/$(UDEV)/udev/%
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(@D) $<

$(addprefix $(UDEV_DIR)/$(UDEV)/udev/,$(TARGETS)): prepare
	$(AT)make -C $(UDEV_DIR)/$(UDEV) USE_STATIC=false

################################################################################
clean:
	$(AT)rm -rf $(UDEV_DIR)/$(UDEV)

################################################################################
prepare:
	$(AT)test -d $(UDEV_DIR) || mkdir -p $(UDEV_DIR)
	$(AT)test -d $(UDEV_DIR)/$(UDEV) || (tar xf $(OPEN_SOURCE_DIR)/$(UDEV).tar.bz2 -C $(UDEV_DIR); \
        cd $(UDEV_DIR)/$(UDEV); CC=$(HOST)-gcc; ./configure --prefix=/ --host=$(HOST) \
        --disable-logging --disable-extras --disable-introspection;)

################################################################################
.PHONY: install clean distclean
################################################################################
