#
# (C) COPYRIGHT 2010-2015 ARM Limited. All rights reserved.
#
# This program is free software and is provided to you under the terms of the
# GNU General Public License version 2 as published by the Free Software
# Foundation, and any use by you of this program is subject to the terms
# of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained
# from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA  02110-1301, USA.
#
#


ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
        KERNEL_DIR := $(srctree)

        SDK_DIR := $(KERNEL_DIR)/../../..
    else
        SDK_DIR := $(CURDIR)/../../../../../../../../..
    endif

    include $(SDK_DIR)/base.mak
endif

include $(SDK_DIR)/source/msp/drv/gpu/mali700/kbuild_flags


KDIR ?= /lib/modules/$(shell uname -r)/build

BUSLOG_PATH_RELATIVE = $(CURDIR)/../../../..
KBASE_PATH_RELATIVE = $(CURDIR)
KDS_PATH_RELATIVE = $(CURDIR)/../../../..

SCONS_CFLAGS=-DMALI_GCC_WORKAROUND_MIDCOM_4598=0 -DCONFIG_KDS -DCONFIG_MALI_DEBUG -DCONFIG_MALI_TRACE_TIMELINE -DCONFIG_MALI_PLATFORM_DEVICETREE -DCONFIG_MALI_GATOR_SUPPORT 
SCONS_CONFIGS=MALI_GCC_WORKAROUND_MIDCOM_4598=0 CONFIG_KDS=m CONFIG_MALI_MIDGARD=m CONFIG_MALI_DEBUG=y CONFIG_MALI_PLATFORM_DEVICETREE=y CONFIG_MALI_TRACE_TIMELINE=y CONFIG_MALI_GATOR_SUPPORT=y

ifeq ($(MALI_UNIT_TEST), 1)
	EXTRA_SYMBOLS += $(KBASE_PATH_RELATIVE)/tests/internal/src/kernel_assert_module/linux/Module.symvers
endif

ifneq ($(wildcard $(CURDIR)/internal/Makefile.in),)
include $(CURDIR)/internal/Makefile.in
endif

ifeq ($(MALI_BUS_LOG), 1)
#Add bus logger symbols
EXTRA_SYMBOLS += $(BUSLOG_PATH_RELATIVE)/drivers/base/bus_logger/Module.symvers
endif

# GPL driver supports KDS
EXTRA_SYMBOLS += $(KDS_PATH_RELATIVE)/drivers/base/kds/Module.symvers

# we get the symbols from modules using KBUILD_EXTRA_SYMBOLS to prevent warnings about unknown functions
all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) EXTRA_CFLAGS="-I$(CURDIR)/../../../../include $(SCONS_CFLAGS)" $(SCONS_CONFIGS) KBUILD_EXTRA_SYMBOLS="$(EXTRA_SYMBOLS)" modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
