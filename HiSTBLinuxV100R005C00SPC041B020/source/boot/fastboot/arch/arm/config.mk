#
# (C) Copyright 2000-2002
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CROSS_COMPILE ?=

ifeq ($(BOARD),omap2420h4)
STANDALONE_LOAD_ADDR = 0x80300000
else
ifeq ($(SOC),omap3)
STANDALONE_LOAD_ADDR = 0x80300000
else
STANDALONE_LOAD_ADDR = 0xc100000
endif
endif

PLATFORM_CPPFLAGS += -DCONFIG_ARM -D__ARM__

# Explicitly specifiy 32-bit ARM ISA since toolchain default can be -mthumb:

ifeq ($(CONFIG_SYS_THUMB_BUILD),y)
PLATFORM_CPPFLAGS += $(call cc-option, -mthumb -mthumb-interwork,\
		$(call cc-option,-marm,)\
		$(call cc-option,-mno-thumb-interwork,)\
	)
else
PLATFORM_CPPFLAGS += $(call cc-option,-marm,)\
	$(call cc-option,-mno-thumb-interwork,)
endif

# Try if EABI is supported, else fall back to old API,
# i. e. for example:
# - with ELDK 4.2 (EABI supported), use:
#	-mabi=aapcs-linux
# - with ELDK 4.1 (gcc 4.x, no EABI), use:
#	-mabi=apcs-gnu
# - with ELDK 3.1 (gcc 3.x), use:
#	-mapcs-32
PLATFORM_CPPFLAGS += $(call cc-option,\
				-mabi=aapcs-linux,\
				$(call cc-option,\
					-mapcs-32,\
					$(call cc-option,\
						-mabi=apcs-gnu,\
					)\
				)\
			)

# For EABI, make sure to provide raise()
ifneq (,$(findstring -mabi=aapcs-linux,$(PLATFORM_CPPFLAGS)))
# This file is parsed several times; make sure to add only once.
ifeq (,$(findstring arch/arm/lib/eabi_compat.o,$(PLATFORM_LIBS)))
PLATFORM_LIBS += $(OBJTREE)/arch/arm/lib/eabi_compat.o
endif
endif
LDSCRIPT := $(SRCTREE)/$(CPUDIR)/u-boot.lds
