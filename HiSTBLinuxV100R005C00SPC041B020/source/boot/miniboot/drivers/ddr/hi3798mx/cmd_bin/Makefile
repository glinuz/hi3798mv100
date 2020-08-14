#/******************************************************************************
# *    Copyright (c) 2009-2012 by Hisi.
# *    All rights reserved.
# * ***
# *    Create by Czyong. 2012-09-07
# *
#******************************************************************************/
PWD=$(shell pwd)
TOPDIR=$(PWD)/../../../..
sinclude $(TOPDIR)/configs/config
sinclude $(TOPDIR)/scripts/Makefile.define
################################################################################

#PWD   := $(TOPDIR)/drivers/ddr/cmd_bin
#PLATFORM   := hi3716mv410
#CC         := $(CROSS_COMPILE)gcc
#AR         := $(CROSS_COMPILE)ar
#LD         := $(CROSS_COMPILE)ld
#OBJCOPY    := $(CROSS_COMPILE)objcopy
#OBJDUMP    := $(CROSS_COMPILE)objdump
#NM         := $(CROSS_COMPILE)nm
#RM         := rm
################################################################################
DDR_CMD     := ddr_cmd

CMD_TEXT_BASE := $(shell grep '^\#define.*DDR_TRAINING_RUN_STACK' $(TOPDIR)/drivers/ddr/$(PLATFORM)/ddr_training_custom.h|awk '{print $$3}')
STACK_POINT   := $(CMD_TEXT_BASE)

START       := cmd_entry.o
AOBJS       := early_uart.o
COBJS       := ddr_training_custom.o ddr_training_cmd.o ddr_training_impl.o ddr_training_ctl.o
DEPS        := $(COBJS:.o=.d) $(START:.o=.d) $(DOBJS:.o=.d)

SSRC        := ddr_training_impl.c ddr_training_ctl.c ddr_training_custom.c
LINK_FILES  := $(SSRC) early_uart.S 

AM_CFLAGS   := -Os -pipe  \
	-DCMD_TEXT_BASE=$(CMD_TEXT_BASE) -DSTACK_POINT=$(STACK_POINT) \
	-fno-builtin -ffreestanding -I./ -I$(TOPDIR)/include/  -I../ \
	-DDDR_TRAINING_CMD -I$(TOPDIR)/drivers/ddr/$(PLATFORM)/ \
	-I$(TOPDIR)/arm/$(PLATFORM)/include/
AM_CFLAGS += "-march=$(ARCH)"

################################################################################

$(DDR_CMD).bin: $(DDR_CMD).elf
	$(OBJCOPY) -O srec $(DDR_CMD).elf $(DDR_CMD).srec
	$(OBJCOPY) --gap-fill=0xff -O binary $(DDR_CMD).elf $@
	@rm -f $(LINK_FILES) *.o *.d *.elf *.map *.srec

$(DDR_CMD).elf:  $(COBJS) $(START) $(AOBJS) ddr_cmd.lds 
#	@echo CMD_TEXT_BASE=$(CMD_TEXT_BASE)
	$(LD) -Bstatic -T ddr_cmd.lds -Ttext $(CMD_TEXT_BASE) $(START) \
		$(COBJS) $(AOBJS) -Map $(DDR_CMD).map -o $@

$(COBJS) : $(SSRC)

$(SSRC):
	ln -sf $(PWD)/../$@  $(PWD)/$@
	ln -sf $(TOPDIR)/drivers/ddr/$(PLATFORM)/ddr_training_custom.c  $(PWD)/ddr_training_custom.c
	ln -sf $(TOPDIR)/arm/$(PLATFORM)/boot/early_uart.S $(PWD)/early_uart.S

$(START) $(AOBJS):
	@$(call show_cmd,CC,$(@:.o=.S))
	@$(CC) $(AM_CFLAGS) -MM -MT "$@ $(@:.o=.d)" $(@:.o=.S) > $*.d
	@$(CC) -D__ASSEMBLY__ $(AM_CFLAGS) -o $@ -c -c $(@:.o=.S)

$(COBJS):
	@$(call show_cmd,CC,$(@:.o=.c))
	@$(CC) $(AM_CFLAGS) -MM -MT "$@ $(@:.o=.d)" $(@:.o=.c) > $*.d
	@$(CC) $(AM_CFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector \
		-o $@ -c -c $(@:.o=.c)

#ifneq ("$(MAKECMDGOALS)","clean")
sinclude $(DEPS)
#endif

clean:
	@- rm -rf *.o *.d *.elf *.map *.srec $(LINK_FILES) $(DDR_CMD).bin

################################################################################
