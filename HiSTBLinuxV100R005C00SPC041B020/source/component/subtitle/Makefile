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

SRCS := src/hi_unf_subt.c \
        src/subtitle_data.c \
        src/subtitle_parse.c \
        src/scte_subt_data.c \
        src/scte_subt_display.c \
        src/scte_subt_parse.c

LIB_NAME := libhi_subtitle

HEADER_FILES := $(CURDIR)/include/hi_unf_subt.h

include $(SDK_DIR)/hi_rules.mak
