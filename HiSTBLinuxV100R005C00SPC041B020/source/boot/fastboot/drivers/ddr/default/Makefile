include $(TOPDIR)/config.mk

LIB	:= $(obj)libddr.a

SOBJS   := ddr_cmd_loc.o
COBJS   := ddr_cmd_ctl.o ddr_training_impl.o ddr_training_ctl.o ddr_training_boot.o ddr_training_custom.o cmd_ddr_training_v2.o

SSRC    := ddr_training_custom.h ddr_training_custom.c
SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS    := $(addprefix $(obj),$(SOBJS) $(COBJS))

LINK_FILES := $(SSRC)
#########################################################################

all: $(LIB)
	@rm -f $(LINK_FILES)

$(SSRC):
	ln -sf ../$(CPU)/$@ $@

$(addprefix $(obj),ddr_cmd_ctl.o):$(SSRC)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(OBJS): $(SSRC)

clean:
	rm -rf *.d *.o *.a

include $(SRCTREE)/rules.mk

#########################################################################
