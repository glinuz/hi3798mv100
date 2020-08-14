################################################################################
#
#  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
#
#  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
#  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
#  others, published or used, in whole or in part, without the express prior
#  written permission of Hisilicon.
#
################################################################################

CC         = gcc -m32
LDFLAGS    =
CFLAGS     =
RANLIB     = ranlib
PREFIX     = $(shell pwd)
INCLUDEDIR = $(PREFIX)/include
LIBDIR     = $(PREFIX)/lib
BINDIR     = $(PREFIX)/bin

DEPDIR     := .deps
SRC        := $(shell pwd)
AM_CFLAGS  := -I$(SRC) -DANDROID
AM_LDFLAGS := -static
TARGETS    := make_ext4fs ext2simg ext4fixup_main
LIBA       := libext4_utils.a
AM_LIBA    := -lsparse -lselinux -lz -lruleattr
SRC_FILES  :=
PHONYS     :=

libext4_utils_src_files := \
	make_ext4fs.c \
	ext4fixup.c \
	ext4_utils.c \
	allocate.c \
	contents.c \
	extent.c \
	indirect.c \
	uuid.c \
	sha1.c \
	wipe.c \
	crc16.c
SRC_FILES += $(libext4_utils_src_files)
libext4_utils_objs := $(libext4_utils_src_files:.c=.o)

make_ext4fs_src_files := make_ext4fs_main.c
SRC_FILES += $(make_ext4fs_src_files)
make_ext4fs_objs := $(make_ext4fs_src_files:.c=.o)

ext2simg_src_files := ext2simg.c
SRC_FILES += $(ext2simg_src_files)
ext2simg_objs := $(ext2simg_src_files:.c=.o)

ext4fixup_main_src_files := ext4fixup_main.c
SRC_FILES += $(ext4fixup_main_src_files)
ext4fixup_main_objs := $(ext4fixup_main_src_files:.c=.o)

COBJS := $(SRC_FILES:.c=.o)
DEPS := $(addprefix $(DEPDIR)/,$(COBJS:.o=.Po))
################################################################################

all: $(TARGETS)

PHONYS += make_ext4fs
make_ext4fs: $(make_ext4fs_objs) $(LIBA)
	@echo "  LD      $@"
	@$(CC) $(AM_LDFLAGS) $(LDFLAGS) $^ $(AM_LIBA) -lpthread -o $@

PHONYS += ext2simg
ext2simg: $(ext2simg_objs) $(LIBA)
	@echo "  LD      $@"
	@$(CC) $(AM_LDFLAGS) $(LDFLAGS) $^ $(AM_LIBA) -o $@

PHONYS += ext4fixup_main
ext4fixup_main: $(ext4fixup_main_objs) $(LIBA)
	@echo "  LD      $@"
	@$(CC) $(AM_LDFLAGS) $(LDFLAGS) $^ $(AM_LIBA) -o $@

libext4_utils.a: $(libext4_utils_objs)
	@echo "  AR      $@"
	@$(AR) rcs $@ $^
	@$(RANLIB) $@

.c.o:
	@echo "  CC      $@"
	@test -d "$(DEPDIR)/$(@D)" || mkdir -p "$(DEPDIR)/$(@D)"
	@if $(CC) $(AM_CFLAGS) $(CFLAGS) -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" -c -o $@ $<; \
	then mv -f "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; fi

distclean clean:
	@rm -rf $(COBJS) $(DEPDIR) $(TARGETS) $(LIBA)

install: all
	@ test -d $(BINDIR) ||  install -m 755 -d $(BINDIR)
	@ install -m 755 $(TARGETS) $(BINDIR)

sinclude $(DEPS)

.PHONY: all install distclean clean $(PHONYS)
