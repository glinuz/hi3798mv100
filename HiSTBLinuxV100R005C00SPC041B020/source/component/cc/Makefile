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
            -I$(CURDIR)/src/arib/include \
            -I$(CURDIR)/src/cc608/include \
            -I$(CURDIR)/src/cc708/include \
            -I$(CURDIR)/src/ccdisp/include \
            -I$(CURDIR)/src/common/include
# common
SRCS := src/common/src/cc_queue.c \
        src/common/src/cc_debug.c \
        src/common/src/cc_timer.c

# arib
SRCS += src/arib/src/aribcc_decode.c \
        src/arib/src/aribcc_obj.c \
        src/arib/src/aribcc_disp.c \
        src/arib/src/aribcc_graphic.c \
        src/arib/src/aribcc.c \
        src/arib/src/aribcc_osd.c \
        src/arib/src/aribcc_common.c \
        src/arib/src/aribcc_utils.c

# cc608
SRCS += src/cc608/src/cc608_obj.c \
        src/cc608/src/cc608_xds.c \
        src/cc608/src/cc608.c \
        src/cc608/src/cc608_data.c \
        src/cc608/src/cc608_dec.c

# cc708
SRCS += src/cc708/src/cc708_obj.c \
        src/cc708/src/cc708.c \
        src/cc708/src/cc708_dec.c \
        src/cc708/src/cc708_osd.c \
        src/cc708/src/cc708_pen.c \
        src/cc708/src/cc708_cursor.c

# ccdisp
SRCS += src/ccdisp/src/ccdisp_api.c

SRCS += src/hi_unf_cc.c

LIB_NAME := libhi_cc

HEADER_FILES := $(CURDIR)/include/hi_unf_cc.h

include $(SDK_DIR)/hi_rules.mak

