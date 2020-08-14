#/******************************************************************************
# *    Copyright (c) 2009-2012 by Hisi.
# *    All rights reserved.
# * ***
# *    Create by Czyong. 2012-09-07
# *
#******************************************************************************/

sinclude $(TOPDIR)/arch/$(ARCH)/config.mk       # include architecture dependend rules
################################################################################

TOPDIR   :=
PWD      := $(shell pwd)
OPPDIR   := $(subst $(TOPDIR),,$(PWD))

CC       := $(CROSS_COMPILE)gcc
AR       := $(CROSS_COMPILE)ar
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy
OBJDUMP  := $(CROSS_COMPILE)objdump


################################################################################
DDR_CMD     := ddr_cmd

CMD_TEXT_BASE := $(shell grep '^\#define.*DDR_TRAINING_RUN_STACK' $(TOPDIR)/drivers/ddr/$(CPU)/ddr_training_custom.h|awk '{print $$3}')
STACK_POINT   := $(CMD_TEXT_BASE)
START       := cmd_entry.o
COBJS       := uart.o ddr_training_custom.o ddr_training_cmd.o ddr_training_impl.o ddr_training_ctl.o
DEPS        := $(COBJS:.o=.d) $(START:.o=.d)
SSRC        := ddr_training_impl.c ddr_training_ctl.c

CFLAGS   := -Os -pipe  \
	-DCMD_TEXT_BASE=$(CMD_TEXT_BASE) -DSTACK_POINT=$(STACK_POINT) \
	-fno-builtin -ffreestanding -I./ -I$(TOPDIR)/include -I../ \
	-DDDR_TRAINING_CMD -I$(TOPDIR)/drivers/ddr/$(CPU)/

CFLAGS += $(PLATFORM_RELFLAGS) $(PLATFORM_CPPFLAGS)

################################################################################

LINK_FILES = $(SSRC) uart.S ddr_training_custom.c

.PHONY: $(DDR_CMD).bin
all: $(DDR_CMD).bin
	#remove soft link files
	@rm -f $(LINK_FILES) *.o *.d *.elf *.map *.srec

$(DDR_CMD).bin: $(DDR_CMD).elf
	$(OBJCOPY) -O srec $(PWD)/$(DDR_CMD).elf $(DDR_CMD).srec
	$(OBJCOPY) --gap-fill=0xff -O binary $(PWD)/$(DDR_CMD).elf $@

$(DDR_CMD).elf: $(START) $(COBJS) ddr_cmd.lds
	#@echo CMD_TEXT_BASE=$(CMD_TEXT_BASE)
	$(LD) -Bstatic -T ddr_cmd.lds -Ttext $(CMD_TEXT_BASE) $(START) \
		$(COBJS) $(AOBJS) -Map $(DDR_CMD).map -o $@

$(SSRC):
	ln -sf ../$@ $@
	ln -sf $(TOPDIR)/arch/$(ARCH)/cpu/$(CPU)/uart.S uart.S
	ln -sf $(TOPDIR)/drivers/ddr/$(CPU)/ddr_training_custom.c ddr_training_custom.c

.PHONY: clean
clean:
	@rm -vf *.o *.d *.elf *.map *.srec $(LINK_FILES) $(DDR_CMD).bin

%.o : %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c $*.S

%.o : %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector \
		-o $@ -c $*.c

ifneq ("$(MAKECMDGOALS)","clean")
sinclude $(DEPS)
endif

%.d : %.c
	set -e; $(CC) $(CFLAGS) -MM $< | sed 's,$*.o:,$*.o $*.d:,g' > $@

%.d : %.S
	set -e; $(CC) $(CFLAGS) -MM $< | sed 's,$*.o:,$*.o $*.d:,g' > $@

################################################################################
