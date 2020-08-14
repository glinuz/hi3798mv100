#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../..
include $(SDK_DIR)/base.mak
endif

PREFIX         ?= $(shell pwd)

################################################################################
#
#  options below should be redefined here, don't use definitions in Makefile
#  in SDK top dir.
#
CC=gcc
AR=ar
CPP=cpp
LD=ld
RANLIB=ranlib
NM=nm
STRIP=strip

ATTR    := attr-2.4.47
LIBCAP  := libcap-ng-0.7.7

################################################################################
LIBATTR_PATH := $(PREFIX)/$(ATTR)
FILECAP_PATH := $(PREFIX)/$(LIBCAP)

################################################################################

all: filecap


filecap:
	$(AT)mkdir -p $(PREFIX)
	$(AT)test -d $(FILECAP_PATH) || tar xf $(OPEN_SOURCE_DIR)/$(LIBCAP).tar.bz2 -C $(PREFIX)
	$(AT)mkdir -p $(FILECAP_PATH)/INSTALL_DIR
	cd $(FILECAP_PATH) && \
	LDFLAGS=-static CFLAGS="-I$(LIBATTR_PATH)/INSTALL_DIR/include -m32 -g -O2" \
	    ./configure --prefix=/usr --disable-shared --without-python && \
	make && \
	./libtool  --tag=CC   --mode=link gcc -all-static -W -Wall -Wshadow -Wformat -Wundef -D_GNU_SOURCE -m32 -g -O2  -static -L./src -o utils/filecap utils/filecap.o  -lcap-ng && \
	make DESTDIR=$(FILECAP_PATH)/INSTALL_DIR install && \
	cd -

################################################################################

clean:
	$(AT)rm -rf $(LIBATTR_PATH) $(FILECAP_PATH)

distclean: clean

################################################################################
.PHONY: all clean distclean
################################################################################
