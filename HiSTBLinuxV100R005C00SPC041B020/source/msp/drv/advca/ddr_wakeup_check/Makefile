#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(KERNEL_DIR)/../../../..
    else
    SDK_DIR := $(CURDIR)/../../../../..
    endif

include $(SDK_DIR)/base.mak
endif

TARGET := ddr_wakeup_check


CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)-

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
gccincdir := $(shell $(CC) -print-file-name=include)
PLATFORM_LIBGCC = -L$(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

LDSCRIPT := linker.lds
LDFLAGS := -Bstatic -T $(LDSCRIPT) $(PLATFORM_LIBGCC)
LDFLAGS += -L$(shell dirname `$(CC) -print-libgcc-file-name`) -lgcc 

CFLAGS := -fno-builtin -fno-common
CFLAGS += -Wall -g -c -nostdinc -Os
#CFLAGS += -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
CFLAGS += -isystem $(gccincdir)
CFLAGS += -DCHIP_TYPE_$(CFG_HI_CHIP_TYPE)
CFLAGS += -I$(PWD)

SFLAGS := $(CFLAGS)

CSRC  = ${wildcard *.c lib/*.c}
ASRC  = $(wildcard *.S lib/*.S)

COBJS := $(patsubst %.c, %.o, $(CSRC))
AOBJS += $(patsubst %.S, %.o, $(ASRC))

COBJS:=$(COBJS:%.C=%.o)
AOBJS:=$(AOBJS:%.S=%.o)

#$(warning COBJS $(COBJS) )
#$(warning AOBJS $(AOBJS) )

all: $(AOBJS) $(COBJS)
	$(LD) $^ $(LDFLAGS) -Map $(TARGET).map -o $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(TARGET).bin
	
%.o:	%.S
	$(CC)  $(SFLAGS) -o $@ $<
	
%.o:	%.c
	$(CC)  $(CFLAGS) -o $@ $<
	
.PHONY : clean
clean:
	-rm -f $(COBJS) $(AOBJS) $(TARGET) $(TARGET).bin $(TARGET).map
	

