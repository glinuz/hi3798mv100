################################################################################
#    Copyright (C) 2014 Hisilicon STB Development Dept
#    All rights reserved.
#####
#    Create by Cai Zhiyong
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#    http://www.apache.org/licenses/LICENSE-2.0
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
################################################################################

CFLAGS += -I$(SRCDIR)../

SRCS-y = hifmc100_host.c hiflash.c

ifeq ($(CONFIG_GENERIC_HIFMC100_SPI_SPI_NOR),y)
SRCS-y +=  hifmc100_spinor.c \
	   spinor_ids.c \
	   spinor_driver.c
endif

ifneq ($(CONFIG_GENERIC_HIFMC100_NAND)_$(CONFIG_GENERIC_HIFMC100_SPI_NAND),_)
SRCS-y += spinand_ids.c \
	  spinand_driver.c \
	  hifmc100_xnand.c \
	  nand_ids.c  \
	  read_retry.c \
	  read_retry_hynix.c \
	  read_retry_micron.c \
	  read_retry_samsung.c \
	  read_retry_toshiba_v2012.c \
	  read_retry_toshiba_v2013.c
endif

SRCS-$(CONFIG_ARCH_HI3716MV410)  += hifmc100_clock_hi3716mv410.c
SRCS-$(CONFIG_ARCH_HI3798CV2X)   += hifmc100_clock_hi3798cv2x.c
SRCS-$(CONFIG_ARCH_HI3798MV2X)   += hifmc100_clock_hi3798mv2x.c

#########################################################################
