#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
HI3G_DIR := $(CURDIR)

CFLAGS  +=  -I$(HI3G_DIR)/include \
            -I$(HI3G_DIR)/src

SRCS := src/hi_3g_intf.c \
        src/3g_intf.c \
        src/3g_ids.c

LIB_NAME := libhi_3g

HEADER_FILES := include/hi_3g_intf.h

include $(SDK_DIR)/hi_rules.mak
