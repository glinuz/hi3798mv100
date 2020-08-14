include $(TOPDIR)/config.mk

LIB	:= $(obj)libddr.a

SOBJS   := ddr_cmd_loc.o
COBJS   := ddr_cmd_ctl.o ddr_training_impl.o ddr_training_ctl.o ddr_training_boot.o ddr_training_custom.o cmd_ddr_training_v2.o

SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS    := $(addprefix $(obj),$(SOBJS) $(COBJS))

#########################################################################

all: $(LIB)

cmd:
	$(MAKE) -C cmd_bin TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE)

$(addprefix $(obj),ddr_cmd_loc.o):cmd

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(MAKE) -C cmd_bin TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE) clean
	rm -rf *.d *.o *.a

include $(SRCTREE)/rules.mk

#########################################################################
