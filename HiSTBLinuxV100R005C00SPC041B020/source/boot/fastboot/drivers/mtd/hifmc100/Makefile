#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk


LIB	:= $(obj)libhifmc100.a

COBJS-y = hifmc100_host.o hiflash.o

COBJS-$(CONFIG_HIFMC100_SPI_NOR_SUPPORT) += hifmc100_spinor.o \
					    spinor_ids.o \
					    spinor_driver.o

ifneq ($(CONFIG_HIFMC100_NAND_SUPPORT)_$(CONFIG_HIFMC100_SPI_NAND_SUPPORT),_)
COBJS-y += spinand_driver.o \
	   spinand_ids.o \
	   hifmc100_xnand.o \
	   nand_ids.o \
	   hifmc100_xnand.o \
	   read_retry.o \
	   read_retry_hynix.o \
	   read_retry_micron.o \
	   read_retry_samsung.o \
	   read_retry_toshiba_v2012.o \
	   read_retry_toshiba_v2013.o
endif

COBJS-$(CONFIG_ARCH_HI3716MV410)  += hifmc100_clock_hi3716mv410.o
COBJS-$(CONFIG_ARCH_HI3798CV2X)   += hifmc100_clock_hi3798cv2x.o
COBJS-$(CONFIG_ARCH_HI3798MV2X)   += hifmc100_clock_hi3798mv2x.o
COBJS-$(CONFIG_ARCH_HI3796MV2X)   += hifmc100_clock_hi3796mv2x.o

################################################################################

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
