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

SRCS-y := hinfc504.c \
	hinfc504_gen.c \
	hinfc504_ecc.c \
	hinfc504_read_retry.c \
	hinfc504_read_retry_hynix_bg_bdie.c \
	hinfc504_read_retry_hynix_bg_cdie.c \
	hinfc504_read_retry_hynix_cg_adie.c \
	hinfc504_read_retry_micron.c \
	hinfc504_read_retry_samsung.c \
	hinfc504_read_retry_toshiba.c

SRCS-$(CONFIG_CPU_HI3716CV200ES) += hinfc504_s40.c
SRCS-$(CONFIG_ARCH_GODBOX) += hinfc504_godbox.c
