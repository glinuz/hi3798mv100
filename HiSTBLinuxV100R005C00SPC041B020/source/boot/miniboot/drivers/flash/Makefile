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

ifneq ($(CONFIG_GENERIC_HIFMC100_NAND)_$(CONFIG_GENERIC_HIFMC100_SPI_NAND)_$(CONFIG_GENERIC_HIFMC100_SPI_SPI_NOR),__)
CONFIG_GENERIC_HIFMC100 := y
endif

ifneq ($(CONFIG_GENERIC_NAND)_$(CONFIG_GENERIC_SPI_NAND),_)
SRCS-y                                        += nand/
endif

SRCS-$(CONFIG_GENERIC_SFC)                    += spiflash/
SRCS-$(CONFIG_GENERIC_MMC)                    += mmc/
SRCS-$(CONFIG_GENERIC_HIFMC100)               += hifmc100/

SRCS-y += flash.c flash_gen.c parts.c find_parts.c
