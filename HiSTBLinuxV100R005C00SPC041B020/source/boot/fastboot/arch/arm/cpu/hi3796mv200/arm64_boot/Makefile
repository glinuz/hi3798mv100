#/******************************************************************************
# *    Copyright (c) 2009-2014 by Hisi.
# *    All rights reserved.
# * ***
# *    Create by zengzhl. 2014-07-28
# *
#******************************************************************************/

################################################################################
CROSS_COMPILE ?= "aarch64-linux-gnu-"

TOPDIR	:=

CC		:= $(CROSS_COMPILE)gcc
LD		:= $(CROSS_COMPILE)ld
OBJCOPY	:= $(CROSS_COMPILE)objcopy

################################################################################
BOOT		:= arm64_boot
#TEXTBASE	:= 0xffff0c00
TEXTBASE	:= 0xFFFF0000
#0xfff0000
AOBJS		+= start.o gic_64.o transition.o# exceptions.o
COBJS		+= arm64_main.o# interrupts_64.o

CFLAGS   := -Os -fno-strict-aliasing -fno-common -ffixed-r9 -fno-builtin -ffreestanding \
	-DTEXT_BASE=$(TEXTBASE) -D__ARM__ -I../../../include/ -I../../../../../include/ \
	-pipe -march=armv8-a

################################################################################
all:$(BOOT).bin

TMPS += $(BOOT).bin $(BOOT).srec
$(BOOT).bin: $(BOOT).elf
	$(OBJCOPY) -O srec $(BOOT).elf $(BOOT).srec
	$(OBJCOPY) --gap-fill=0xff -O binary $(BOOT).elf $@

TMPS += $(BOOT).elf $(BOOT).map
$(BOOT).elf: $(AOBJS) $(COBJS)
	$(LD) -Bstatic -T boot.lds -Ttext $(TEXTBASE) $(AOBJS) $(COBJS) -Map $(BOOT).map -o $@

.PHONY: clean
TMPS += $(AOBJS) $(COBJS)
clean:
	@rm -vf $(TMPS)

%.o : %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c $*.S

%.o : %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector -o $@ -c $*.c
################################################################################
