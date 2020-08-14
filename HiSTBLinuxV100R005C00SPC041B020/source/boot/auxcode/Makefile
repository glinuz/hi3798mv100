################################################################################
#    Makefile for auxcode
################################################################################

PWD           = $(shell pwd)
TOPDIR        := $(PWD)
UNIBOOT_DIR   = $(PWD)/../
################################################################################

export CC       := $(CROSS_COMPILE)gcc
export AR       := $(CROSS_COMPILE)ar
export LD       := $(CROSS_COMPILE)ld
export OBJCOPY  := $(CROSS_COMPILE)objcopy
################################################################################

AUX_SIGN_TOOL  := $(PWD)/aux_sign_enc_tool
################################################################################
BOOT     := auxcode
OUTPUT   := auxcode_sign.img
TEXTBASE := 0x00000000

CFLAGS   := -g -Os -fno-strict-aliasing -fno-common \
	-D__KERNEL__ -DTEXT_BASE=$(TEXTBASE) -Wall \
	-fno-builtin -ffreestanding -fPIC\
	-pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux  \
	-mno-thumb-interwork -march=armv7-a

CFLAGS  += -I$(TOPDIR)/include
CFLAGS  += -I$(TOPDIR)/include/arch-$(MACH)
CFLAGS  += -I$(TOPDIR)/ddr

ALL_CONFIG   := $(notdir $(wildcard $(TOPDIR)/include/configs/$(MACH).h))
ALL_PLATFORM := $(subst .h,,$(ALL_CONFIG))

MKCONFIG := $(TOPDIR)/mkconfig

export CFLAGS TOPDIR

################################################################################
$(ALL_PLATFORM):
	cd $(TOPDIR)/include && $(MKCONFIG) $@ && cd -
	make $(BOOT).bin

sinclude $(TOPDIR)/include/config.mk

LIBS = boot/libboot.a
LIBS += common/libcommon.a
LIBS += ddr/libddr.a
LIBS += arch/$(MACH)/lib$(MACH).a

.PHONY : $(LIBS)
################################################################################
$(BOOT).bin: $(BOOT)
	$(OBJCOPY) -O binary $< $@
	@echo $(BOOT).bin is Ready.
	$(AUX_SIGN_TOOL) aux_sign.cfg -i $@ -o $(OUTPUT)
	@cp -fv $(OUTPUT) $(UNIBOOT_DIR)
	@echo $(OUTPUT) is Ready.

$(BOOT): $(LIBS) link.lds
	$(LD) -Bstatic -T link.lds -Ttext $(TEXTBASE) $(START) \
		--start-group $(LIBS) --end-group -Map $(BOOT).map -o $@
################################################################################

$(LIBS):
	$(MAKE) -C $(dir $(subst $(obj),,$@));
################################################################################

distclean: clean

clean:
	for f in ${LIBS}; do ( \
		make -C $$(dirname $${f}) clean \
	) done
	@rm -f $(TOPDIR)/include/config.h
	@rm -f $(TOPDIR)/include/config.mk
	@rm -f $(BOOT) $(BOOT).map $(BOOT).bin
	@rm -f hash.txt
	@rm -f $(OUTPUT)
	@rm -f $(UNIBOOT_DIR)/$(OUTPUT)


################################################################################
.PHONY: clean
################################################################################
