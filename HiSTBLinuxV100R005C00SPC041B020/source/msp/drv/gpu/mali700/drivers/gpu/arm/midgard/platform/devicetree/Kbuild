#
# (C) COPYRIGHT 2012-2013 ARM Limited. All rights reserved.
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


ifeq ($(CONFIG_MALI_MIDGARD),y)
obj-y += mali_kbase_runtime_pm.c
obj-y += mali_kbase_config_devicetree.c
obj-y += mali_kbase_proc_helper.c
obj-y += mali_kbase_proc.c
obj-y += mali_kbase_avs.c
else ifeq ($(CONFIG_MALI_MIDGARD),m)
SRC += platform/devicetree/mali_kbase_runtime_pm.c
SRC += platform/devicetree/mali_kbase_config_devicetree.c
SRC += platform/devicetree/mali_kbase_proc_helper.c
SRC += platform/devicetree/mali_kbase_proc.c
SRC += platform/devicetree/mali_kbase_avs.c
endif
