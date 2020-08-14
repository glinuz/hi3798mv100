

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).a

START   := start.o
COBJS   := chip.o
COBJS   += cpu.o
COBJS   += timer.o
COBJS   += usb.o
SOBJS   += uart.o
SOBJS   += arm_exception.o
SOBJS   += tail.o
SOBJS   += slave_bootup.o

SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
START	:= $(addprefix $(obj),$(START))

ifdef CONFIG_COMPRESS_BOOT
AFLAGS += -DCONFIG_NO_HEAD
endif

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
