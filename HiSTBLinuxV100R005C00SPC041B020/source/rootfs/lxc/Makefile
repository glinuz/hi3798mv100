#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
ifneq ($(HI_OUT_DIR),)
TOP_DIR := $(HI_OUT_DIR)/obj/source/rootfs/lxc
else
TOP_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

BUILD_DIR      := $(TOP_DIR)/tmp
HOST           := $(CFG_HI_ARM_TOOLCHAINS_NAME)
LIBCAP         := libcap-2.24
ATTR           := attr-2.4.47
LXC            := lxc-1.0.8

################################################################################

all: $(LXC)

################################################################################

$(LXC): $(LIBCAP)
	cd $(TOP_DIR)/$(LXC) && \
	./configure --host=$(HOST) --prefix=/usr --with-log-path=/tmp --with-runtime-path=/tmp --disable-bash \
	CFLAGS="-I$(TOP_DIR)/$(LIBCAP)/libcap/include" LDFLAGS="-L$(TOP_DIR)/$(LIBCAP)/libcap" && \
	make && \
	make DESTDIR=$(BUILD_DIR)/install install
	@rm -rf $(BUILD_DIR)/install/usr/lib/pkgconfig && \
		rm -rf $(BUILD_DIR)/install/usr/etc && \
		rm -rf $(BUILD_DIR)/install/usr/share && \
		rm -rf $(BUILD_DIR)/install/usr/include 
	@cp -ar $(BUILD_DIR)/install/* $(PREFIX)/

$(LIBCAP): $(ATTR)
	cd $(TOP_DIR)/$(LIBCAP) && \
	make CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc BUILD_CC=gcc LDFLAGS+="-L$(TOP_DIR)/$(ATTR)/libattr/.libs/  -lattr"
	$(AT)test -d $(PREFIX)/usr/lib || mkdir -p $(PREFIX)/usr/lib
	cp -a $(TOP_DIR)/$(LIBCAP)/libcap/libcap.so* $(PREFIX)/usr/lib

$(ATTR): prepare
	cd $(TOP_DIR)/$(ATTR) && \
	./configure --host=$(HOST) --disable-shared && \
	make

prepare:
	$(AT)test -d $(TOP_DIR) || mkdir -p $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(LXC) || tar xf $(OPEN_SOURCE_DIR)/$(LXC).tar.gz -C $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(ATTR) || tar xf $(OPEN_SOURCE_DIR)/$(ATTR).tar.bz2 -C $(TOP_DIR)
	$(AT)test -d $(TOP_DIR)/$(LIBCAP) || tar xf $(OPEN_SOURCE_DIR)/$(LIBCAP).tar.gz -C $(TOP_DIR)

################################################################################

clean:
	$(AT)rm -rf $(TOP_DIR)/$(LXC) $(TOP_DIR)/$(ATTR) $(TOP_DIR)/$(LIBCAP) $(BUILD_DIR)

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean distclean install
################################################################################

