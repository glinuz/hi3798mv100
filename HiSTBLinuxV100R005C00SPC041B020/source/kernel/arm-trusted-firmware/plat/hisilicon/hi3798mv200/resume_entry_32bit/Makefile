#===============================================================================
# export variables
#===============================================================================

TARGET := resume_entry_32bit
LOAD_ADDR = 0x0

#
# Include the make variables (CC, etc...)
#
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
LDR	= $(CROSS_COMPILE)ldr
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB

PHONY: help

GREEN="\e[32;1m"
NORMAL="\e[39m"
RED="\e[31m"

LDSCRIPT := linker.lds
LDFLAGS := -Bstatic -T $(LDSCRIPT) -Ttext $(LOAD_ADDR)

CFLAGS += -march=armv7-a -Wall -g -c -fPIC -nostdinc -fno-builtin

SFLAGS := $(CFLAGS)

CSRC  = ${wildcard *.c lib/*.c}
ASRC  = $(wildcard *.S lib/*.S)

COBJS := $(patsubst %.c, %.o, $(CSRC))
AOBJS += $(patsubst %.S, %.o, $(ASRC))

COBJS:=$(COBJS:%.C=%.o)
AOBJS:=$(AOBJS:%.S=%.o)


all: $(AOBJS) $(COBJS)
	$(LD) $^ $(LDFLAGS) -Map $(TARGET).map -o $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

help:
	@echo -e ${GREEN}
	@echo "make CROSS_COMPILE=arm-xxx-linux- all    -- Build the bin"
	@echo "make CROSS_COMPILE=arm-xxx-linux- clean  -- Clean all"
	@echo -e ${NORMAL}

%.o:	%.S
	$(CC)  $(SFLAGS) -o $@ $<

%.o:	%.c
	$(CC)  $(CFLAGS) -o $@ $<

.PHONY : clean
clean:
	-rm -rf $(COBJS) $(AOBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map $(LIBS)


