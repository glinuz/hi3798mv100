#
# drivers/mtd/nand/hisfc400/Makefile
#

include $(TOPDIR)/config.mk


LIB	:= $(obj)libhisfc400.a

COBJS-$(CONFIG_NAND_FLASH_HISFC400) += hisfc400.o hisfc400_match.o \
				   hisfc400_ecc_info.o hisfc400_gen.o \
				   hisfc400_spi_general.o hisfc400_spi_giga.o


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
