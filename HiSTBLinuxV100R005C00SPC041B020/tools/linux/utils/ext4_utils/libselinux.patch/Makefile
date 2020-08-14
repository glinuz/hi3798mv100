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

CC         ?= gcc -m32
RANLIB     ?= ranlib
PREFIX     ?= $(pwd)
LIBDIR     ?= $(PREFIX)/lib
CFLAGS      =
LDFLAGS     =

DEPDIR     := .deps
SRC        := $(shell pwd)
AM_CFLAGS  := -I$(SRC) -I$(SRC)/include -DHOST
TARGET     :=
LIBA       := libselinux.a
LIB        := $(LIBA)
SRC_FILES   = 

common_src_files := \
	src/booleans.c \
	src/canonicalize_context.c \
	src/disable.c \
	src/enabled.c \
	src/fgetfilecon.c \
	src/fsetfilecon.c \
	src/getenforce.c \
	src/getfilecon.c \
	src/getpeercon.c \
	src/lgetfilecon.c \
	src/load_policy.c \
	src/lsetfilecon.c \
	src/policyvers.c \
	src/procattr.c \
	src/setenforce.c \
	src/setfilecon.c \
	src/context.c \
	src/mapping.c \
	src/stringrep.c \
	src/compute_create.c \
	src/compute_av.c \
	src/avc.c \
	src/avc_internal.c \
	src/avc_sidtab.c \
	src/get_initial_context.c \
	src/checkAccess.c \
	src/sestatus.c \
	src/deny_unknown.c
libselinux_src_files += $(common_src_files)

common_host_src_files := \
	src/callbacks.c \
	src/check_context.c \
	src/freecon.c \
	src/init.c \
	src/label.c \
	src/label_file.c \
	src/label_android_property.c
libselinux_src_files += $(common_host_src_files)

libselinux_src_files += src/android.c
SRC_FILES += $(libselinux_src_files)
libselinux_obj := $(libselinux_src_files:.c=.o)

COBJS := $(SRC_FILES:.c=.o)
DEPS := $(addprefix $(DEPDIR)/,$(COBJS:.o=.Po))
################################################################################

all: $(TARGETS) $(LIBA)

libselinux.a: $(libselinux_obj)
	@echo "  AR      $@"
	@$(AR) rcs $@ $^
	@$(RANLIB) $@

.c.o:
	@echo "  CC      $@"
	@test -d "$(DEPDIR)/$(@D)" || mkdir -p "$(DEPDIR)/$(@D)"
	@if $(CC) $(AM_CFLAGS) $(CFLAGS) -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" -c -o $@ $<; \
	then mv -f "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; fi

distclean clean:
	@rm -rf $(COBJS) $(DEPDIR) $(TARGETS) $(LIB)

$(LIBDIR)/%.a: %.a
	test -d $(LIBDIR) || install -m 755 -d $(LIBDIR)
	install -m 644 $< $(@D)

$(PREFIX)/%.h: %.h
	test -d $(@D) || install -m 755 -d $(@D)
	install -m 644 $< $(@D)

install: \
    $(addprefix $(LIBDIR)/,$(LIBA)) \
    $(addprefix $(PREFIX)/,$(wildcard include/private/*.h) $(wildcard include/selinux/*.h))

sinclude $(DEPS)
