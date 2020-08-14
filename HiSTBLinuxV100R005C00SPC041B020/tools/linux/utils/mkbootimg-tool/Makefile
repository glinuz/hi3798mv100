#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX         ?= $(shell pwd)

###############################################################################
CC=gcc
AR=ar
CPP=cpp
LD=ld
RANLIB=ranlib
NM=nm
STRIP=strip

###############################################################################
all:
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	$(AT)tar xf $(OPEN_SOURCE_DIR)/mkbootimg-tool.tar.bz2 -C $(PREFIX)
	$(AT)cd $(PREFIX)/mkbootimg-tool; \
		make all; \
	cd -

clean:
	$(AT)rm -rf $(PREFIX)/mkbootimg-tool

distclean: clean
###############################################################################
.PHONY: clean distclean all
###############################################################################
