#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../..
include $(SDK_DIR)/base.mak
endif

PREFIX          ?= $(shell pwd)/rootfs

################################################################################
BUILD_DIR       := $(shell pwd)
HOST            := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP           := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

PROTO         := protobuf-2.5.0
TARGETS         := lib/libprotobuf.so.8

################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/%: $(BUILD_DIR)/%
	@test -d $(@D) || mkdir -p $(@D)
	@install -v -t $(dir $@) $<

$(BUILD_DIR)/lib/libprotobuf.so.8: prepare
	cd $(PROTO) && \
	CPPFLAGS="-P"\
		./configure --host=$(HOST) --without-ada --with-protoc=protoc --prefix=$(BUILD_DIR) && \
		make -j 16   && \
		make install && \
	cd -

################################################################################
prepare:
	#$(AT)test -d $(GDB) || tar -xjf $(OPEN_SOURCE_DIR)/$(GDB).tar.bz2
	$(AT)test -d $(PROTO) || tar -xzf $(OPEN_SOURCE_DIR)/$(PROTO).tar.gz

################################################################################

clean:
	$(AT)rm -rf $(PROTO)
	$(AT)rm -rf "lib"
	$(AT)rm -rf "bin"
#	@(for ix in $(TARGETS); do \
		test ! -e $(PREFIX)/$$ix || rm -rvf $(PREFIX)/$$ix; \
	done)

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################

