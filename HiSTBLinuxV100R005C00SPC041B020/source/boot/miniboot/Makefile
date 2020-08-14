################################################################################
#    Copyright (C) 2014 Hisilicon STB Development Dept
#    All rights reserved.
#####
#    Create by Cai Zhiyong
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#    http://www.apache.org/licenses/LICENSE-2.0
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
################################################################################
ARCH ?= armv7-a
CROSS_COMPILE  ?= arm-histbv310-linux-
ifeq ($(CROSS_COMPILE),)
$(error You should define CROSS_COMPILE!)
endif
BOOT           = miniboot
V              = 0

################################################################################

TOPDIR     := .
DEFTARGET  := $(MAKECMDGOALS)
PLATFORM   :=
PHONYS     :=
TEMPS      :=
CC         := $(CROSS_COMPILE)gcc
AR         := $(CROSS_COMPILE)ar
LD         := $(CROSS_COMPILE)ld
OBJCOPY    := $(CROSS_COMPILE)objcopy
OBJDUMP    := $(CROSS_COMPILE)objdump
NM         := $(CROSS_COMPILE)nm
RM         := rm
DEFCONFIG  := $(TOPDIR)/configs/config

sinclude $(DEFCONFIG)

ARFLAGS    := rcs
LDFLAGS    :=
CFLAGS     := -Wall -nostdinc -isystem $(TOPDIR)/include \
	-I$(TOPDIR)/arm/$(PLATFORM)/include \
	-I$(TOPDIR)/arm/include \
	-Os -fno-strict-aliasing -mapcs-frame \
	-fno-common -fno-builtin -ffreestanding \
	-pipe -marm  -mabi=aapcs-linux \
	-mno-thumb-interwork -march=$(ARCH)

ASFLAGS    := $(CFLAGS)
ZBOOT      := $(BOOT)z
BUILDOBJ   := build-obj

ifeq ($(V),1)
    Q     :=
    QUIET :=
else
    Q     := @
    QUIET := -s
endif

AUXIMAGE =  $(TOPDIR)/auxcode_sign.img
ARCH_AUXIMAGE = $(TOPDIR)/arm/$(PLATFORM)/boot/auxcode_sign.img
AUXCODE  := $(shell if [ -d $(TOPDIR)/../auxcode ]; then \
			echo $(TOPDIR)/../auxcode; \
		else if [ -d $(TOPDIR)/auxcode ]; then \
			echo $(TOPDIR)/auxcode; \
		fi;fi;)

CFLAGS += -DCONFIG_AUXIMAGE=\"$(AUXIMAGE)\"

MKFLAGS    := $(QUIET) -f $(TOPDIR)/scripts/Makefile.build
CLFLAGS    := $(QUIET) -f $(TOPDIR)/scripts/Makefile.clean
export MKFLAGS CLFLAGS

SHELL      := $(shell if [ -x "$${BASH}" ]; then echo $${BASH}; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	     else echo sh; fi; \
	fi)

NETPATH    := $(TOPDIR)/libs/net

export CC AR LD OBJCOPY OBJDUMP RM ARFLAGS LDFLAGS CFLAGS ASFLAGS SHELL NETPATH
export PLATFORM TOPDIR BOOT ZBOOT V Q BUILDOBJ DEFCONFIG MAKE RM NM

EXPORTCONFIG := ${TOPDIR}/scripts/export.sh

INIT_REG := $(wildcard $(TOPDIR)/*.reg $(TOPDIR)/.reg)
export INIT_REG

ifdef CONFIG_UNIFIED_BOOT
BOOTREGS := $(wildcard $(CONFIG_BOOT_REG0) $(CONFIG_BOOT_REG1) $(CONFIG_BOOT_REG2) $(CONFIG_BOOT_REG3) \
                       $(CONFIG_BOOT_REG4) $(CONFIG_BOOT_REG5) $(CONFIG_BOOT_REG6) $(CONFIG_BOOT_REG7))

REGNUM  := $(words $(BOOTREGS))
ASFLAGS += -DREGNUM=$(REGNUM)
endif

################################################################################
SRCDIR      :=
CONFIGINC   := $(TOPDIR)/include/config.h
DEFMAKEFILE := $(TOPDIR)/configs/Makefile
BOOTPATH    := arm/$(PLATFORM)/boot/

ifdef CONFIG_UNIFIED_BOOT
LINKLDS     := arm/$(PLATFORM)/boot_unify.lds
else
LINKLDS     := arm/$(PLATFORM)/boot.lds
endif
PLATFORMLDS := arm/$(PLATFORM)/boot/platform.lds
export LINKLDS PLATFORMLDS

LIBS        := arm/ libs/ drivers/ common/ app/
ZLIBS       := arm/compress/ arm/lib/

LIBS  := $(addsuffix $(BUILDOBJ).o,$(addprefix $(TOPDIR)/,$(LIBS)))
ZLIBS := $(addsuffix $(BUILDOBJ).o,$(addprefix $(TOPDIR)/,$(ZLIBS)))

ifeq ($(CONFIG_DDR_TRAINING_V2),y)
DDR_TRAIN   := $(shell if [ -d $(TOPDIR)/drivers/ddr/$(PLATFORM) ]; then \
					echo drivers/ddr/$(PLATFORM); \
				else \
					echo drivers/ddr/default; \
				fi;)
DDR_CMD_BIN := $(DDR_TRAIN)/cmd_bin
DDRLIBS     := $(addsuffix $(BUILDOBJ).o,$(addprefix $(TOPDIR)/,$(DDR_TRAIN)/))
endif

TOOLPATH    := tools
DEFLATE     := $(TOPDIR)/$(TOOLPATH)/deflate
MKBOOTARGS  := $(TOPDIR)/$(TOOLPATH)/mkbootargs
MKSYM       := $(TOPDIR)/$(TOOLPATH)/mksym
export MKSYM

ifeq ($(CONFIG_PRODUCT_WITH_BOOT),y)
    LIBAPPS := $(TOPDIR)/../product/libproduct.a
else
    LIBAPPS :=
endif

INPUTFLAGS = ${MAKEFLAGS}
export INPUTFLAGS

ALL_CONFIG   := $(notdir $(wildcard $(TOPDIR)/configs/*_config))
ALL_PLATRORM := $(subst _config,,$(ALL_CONFIG))

include $(TOPDIR)/scripts/Makefile.define
################################################################################
sinclude $(DEFMAKEFILE)
TGTCONFIG := $(TOPDIR)/configs/$(DEFTARGET)_config

PHONYS += help
help:
	@echo "make [platform] [options]"
	@echo "platforms:"
	@for ix in $(ALL_PLATRORM); do ( \
		_MSG="    $${ix}                                        "; MSG=$${_MSG:0:40}; \
		echo -n "$${MSG}"; head -n 1 ./configs/$${ix}_config; \
	) done
	@echo ;

################################################################################
TEMPS += $(BOOT).bin $(BOOT).text $(BOOT).srec
PHONYS += $(BOOT).bin
$(BOOT).bin: $(BOOT).elf 
	$(call show_cmd,OBJCOPY,$@)
	$(Q)$(OBJCOPY) -O srec $(BOOT).elf $(BOOT).srec
	$(Q)$(OBJCOPY) -j .text -O binary $(BOOT).elf $(BOOT).text
	$(Q)$(OBJCOPY)  -R .comment --gap-fill=0xff -O binary $(BOOT).elf $@
	$(call show_cmd,BOOT,$@)
	@echo "Config File:  $(DEFTARGET)"
	@echo "Register:     $(notdir $(INIT_REG))"
	@echo "Entry Point:  $(TEXT_BASE)"
	@echo "  Image $@ is ready"

TEMPS += $(ZBOOT).elf $(ZBOOT).bin data.z $(ZBOOT).text \
	$(ZBOOT).srec $(BOOT).elf

ifeq ($(CONFIG_COMPRESS),y)
$(BOOT).elf: check_reg $(CONFIGINC) auxcode $(MKSYM) $(ZLIBS) $(LINKLDS) $(PLATFORMLDS) 
	$(Q)$(MAKE) $(MKFLAGS) SRCDIR="$(BOOTPATH)" LIBS="$(ZLIBS)" $(BOOT).elf

$(ZLIBS): data.z

data.z: $(LIBS) $(LINKLDS) $(PLATFORMLDS) $(DEFLATE) $(DDRLIBS) force
	$(Q)$(MAKE) $(MKFLAGS) SRCDIR="$(BOOTPATH)" LIBS="$(LIBS) $(LIBAPPS)" \
		$(ZBOOT).elf
	$(call show_cmd,OBJCOPY,$(ZBOOT).elf)
	$(Q)$(OBJCOPY) -O srec $(ZBOOT).elf $(ZBOOT).srec
	$(Q)$(OBJCOPY) -j .text -O binary $(ZBOOT).elf $(ZBOOT).text
	$(Q)$(OBJCOPY) -R .comment --gap-fill=0xff -O binary $(ZBOOT).elf $(ZBOOT).bin
	$(call show_cmd,DEFLATE,$@)
	$(Q)$(DEFLATE) $(ZBOOT).bin $@
else
$(BOOT).elf: check_reg $(CONFIGINC) auxcode $(MKSYM) $(LIBS) $(LINKLDS) $(PLATFORMLDS) $(DDRLIBS) force
	$(Q)$(MAKE) $(MKFLAGS) SRCDIR="$(BOOTPATH)" LIBS="$(LIBS) $(LIBAPPS)" CONFIG_AUXIMAGE=\"$(AUXIMAGE)\" \
		$(BOOT).elf
endif

$(TOPDIR)/$(TOOLPATH)/%:
	$(call show_cmd,TOOLS,$@)
	$(Q)$(MAKE) -s SRCDIR="$(TOPDIR)/$(TOOLPATH)" -C $(@D) $(@F)

%/$(BUILDOBJ).o: $(CONFIGINC) ddr_bin force
	$(Q)$(MAKE) $(MKFLAGS) SRCDIR="$(@D)/" $@

$(DEFCONFIG): $(TGTCONFIG)
	$(call show_cmd,GEN,$(DEFCONFIG))
	$(Q)$(SHELL) ${EXPORTCONFIG} export "$(TGTCONFIG)" "$(DEFCONFIG)" "$(CONFIGINC)" INPUTFLAGS

$(DEFMAKEFILE): $(TGTCONFIG)
	$(call show_cmd,GEN,$(DEFMAKEFILE))
	$(call export_make,$(DEFTARGET))

$(ALL_CONFIG):
	$(call show_cmd,GEN,$(DEFCONFIG))
	$(Q)$(SHELL) ${EXPORTCONFIG} export "$(TOPDIR)/configs/$(@)" "$(DEFCONFIG)" "$(CONFIGINC)" INPUTFLAGS
	$(call show_cmd,GEN,$(DEFMAKEFILE))
	$(call export_make,$(subst _config,,$@))

.PHONY: auxcode
auxcode:
ifdef CONFIG_UNIFIED_BOOT
ifeq ($(CONFIG_AUXCODE_COMPILE_SUPPORT),y)
	if [ -d $(AUXCODE) ]; then \
		make -C $(AUXCODE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) O=$(O); \
		cp -rf $(AUXCODE)/auxcode_sign.img $(ARCH_AUXIMAGE); \
	fi;
endif
	cp -rf $(ARCH_AUXIMAGE) $(AUXIMAGE);
endif

.PHONY: check_reg
check_reg:
	@if [ "$(words $(INIT_REG))" = "0" ]; then ( \
		echo '***' Need '.reg' or '*.reg' file in directory $(TOPDIR); \
		exit 1; \
	) fi
ifndef CONFIG_UNIFIED_BOOT
	@if [ "$(words $(INIT_REG))" != "1" ]; then ( \
		echo '***' Found multi '.reg' or '*.reg' file in directory $(TOPDIR); \
		echo '***' Files: $(notdir $(INIT_REG)); \
		exit 1; \
	) fi
endif
PHONYS += tools
tools:;

force:;

ddr_bin:
ifeq ($(CONFIG_DDR_TRAINING_V2),y)
	$(MAKE) -C $(DDR_CMD_BIN) PLATFORM=$(PLATFORM) TOPDIR=$(shell pwd) ARCH=$(ARCH) ddr_cmd.bin
endif

################################################################################
auxcode.clean:
ifdef CONFIG_UNIFIED_BOOT
ifeq ($(CONFIG_AUXCODE_COMPILE_SUPPORT),y)
	@test -d $(AUXCODE) && make -C $(AUXCODE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean
endif
	@rm -f $(AUXIMAGE)
endif

PHONYS += clean $(TOPDIR)/$(TOOLPATH)/clean
clean: $(addsuffix .clean,$(LIBS) $(ZLIBS) $(DDRLIBS)) $(TOPDIR)/$(TOOLPATH)/clean auxcode.clean
	$(Q)$(RM) -f $(TEMPS)
	$(Q)test ! -d "$(BOOTPATH)" || $(MAKE) $(CLFLAGS) SRCDIR="$(BOOTPATH)" clean
ifeq ($(CONFIG_DDR_TRAINING_V2),y)
	$(Q) $(MAKE) -C $(DDR_CMD_BIN) --no-print-directory clean
endif

$(TOPDIR)/$(TOOLPATH)/clean:
	$(call show_cmd,CLEAN,$(@D))
	$(Q)$(MAKE) -s SRCDIR="$(TOPDIR)/$(TOOLPATH)" -C $(TOOLPATH) clean

%/$(BUILDOBJ).o.clean:
	$(call show_cmd,CLEAN,$(@D))
	$(Q)$(MAKE) $(CLFLAGS) SRCDIR="$(@D)/" clean

PHONYS += distclean
distclean: clean
	$(Q)$(RM) -f $(DEFCONFIG) $(CONFIGINC) $(DEFMAKEFILE)

################################################################################

.PHONY: $(PHONYS)
################################################################################
