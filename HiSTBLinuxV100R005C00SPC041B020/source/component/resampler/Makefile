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
CFLAGS  :=  -I$(CURDIR)/include \
            -I$(CURDIR)/src

SRCS := src/resampler.c \
        src/resampler2X.c \
        src/resample_v2.c \
        src/resample_rom.c \
        src/linear_src.c

LIB_NAME := libhi_resampler

HEADER_FILES := $(CURDIR)/include/hi_resampler.h

include $(SDK_DIR)/hi_rules.mak

