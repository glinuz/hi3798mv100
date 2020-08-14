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

SRCS-$(CONFIG_CMDLINE) += cmd_system.c cmd_memory.c cmd_run.c cmd-words.c \
	cmd_env_set.c startcmd.c cmd_getinfo.c cmd_xmodem.c

SRCS-y += env_set.c run.c kern_tags.c reboot.c dump_hex.c xmodem.c bootstrap.c \
	init.c  kern_param.c mem_init.c

SRCS-$(CONFIG_BOOTARGS_MERGE) += params.c
SRCS-$(CONFIG_TEE_SUPPORT) += loadsecureimg.c
SRCS-$(CONFIG_ARM64_SUPPORT) += load_fip.c

SRCS-$(CONFIG_ARCH_S40) += board_s40.c
SRCS-$(CONFIG_ARCH_HI3798MX) += board_hi3798mx.c
SRCS-$(CONFIG_ARCH_HI3716MV410) += board_hi3716mv410.c
SRCS-$(CONFIG_ARCH_GODBOX) += board_godbox.c
SRCS-$(CONFIG_ARCH_HI3716MV310) += board_hi3716mv310.c
SRCS-$(CONFIG_ARCH_HI3798CV2X) += board_hi3798cv2x.c
SRCS-$(CONFIG_ARCH_HI3798MV2X) += board_hi3798mv2x.c
