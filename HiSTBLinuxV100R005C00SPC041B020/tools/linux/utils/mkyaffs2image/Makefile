################################################################################
#    x86/src/mkyaffs2image
#    Create By
#    2011.01.16
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
# parameter
PREFIX      ?= $(shell pwd)
RULEATTR    := $(CURDIR)/../ruleattr
RULEATTR_LIB ?= $(PREFIX)/../ruleattr/libruleattr.a

################################################################################
#
#  options below should be redefined here, don't use definitions in Makefile
#  in SDK top dir.
#
CC=gcc
OBJDUMP=objdump
HOST=
CPP=
LD=
NM=

###############################################################################
BUILD_DIR      := $(PREFIX)/tmp
STRIP          := strip
MKYAFFS2IMAGE  := mkyaffs
MKYAFFS2SOURCES := mkyaffs2image
################################################################################

all: $(PREFIX)/$(MKYAFFS2IMAGE)

$(PREFIX)/$(MKYAFFS2IMAGE): $(BUILD_DIR)/$(MKYAFFS2SOURCES)/$(MKYAFFS2IMAGE)
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<

$(BUILD_DIR)/$(MKYAFFS2SOURCES)/$(MKYAFFS2IMAGE): $(BUILD_DIR)/$(MKYAFFS2SOURCES)/patch
	cd $(BUILD_DIR)/$(MKYAFFS2SOURCES) && \
		make CFLAGS="-I$(RULEATTR)" \
		LDFLAGS="-static -L$(dir $(RULEATTR_LIB))" && \
	cd -

$(BUILD_DIR)/$(MKYAFFS2SOURCES)/patch: \
    $(BUILD_DIR)/$(MKYAFFS2SOURCES)/tar
	cp -rf $(MKYAFFS2SOURCES).patch/. $(BUILD_DIR)/$(MKYAFFS2SOURCES)
	@touch $@
################################################################################

clean:
	@test ! -d $(BUILD_DIR) || rm -rf $(BUILD_DIR)
	@test ! -f $(MKYAFFS2IMAGE) || rm -rf $(MKYAFFS2IMAGE)

distclean: clean

$(BUILD_DIR)/$(MKYAFFS2SOURCES)/tar:
	@test -d $(@D) || mkdir -p $(@D)
	tar -xf $(MKYAFFS2SOURCES).tar.bz2 -C $(BUILD_DIR)
	@touch $@
################################################################################
.PHONY: clean distclean all
################################################################################
