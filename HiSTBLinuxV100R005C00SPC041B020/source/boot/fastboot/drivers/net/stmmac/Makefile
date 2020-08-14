include $(TOPDIR)/config.mk

LIB	:= $(obj)libstmmac.a

COBJS-$(CONFIG_NET_STMMAC) \
		 += mdio.o ctrl.o stmmac.o

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend
#########################################################################
