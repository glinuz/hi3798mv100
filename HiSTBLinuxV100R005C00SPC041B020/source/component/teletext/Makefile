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
CFLAGS := -I$(CURDIR)/include

CFLAGS += -DVER_V1R2

SRCS := src/hi_unf_ttx.c \
        src/teletext_data.c \
        src/teletext_mem.c \
        src/teletext_parse.c \
        src/teletext_recv.c \
        src/teletext_showpage.c \
        src/teletext_utils.c

LIB_NAME := libhi_ttx

HEADER_FILES := $(CURDIR)/include/hi_unf_ttx.h

include $(SDK_DIR)/hi_rules.mak

