#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_rawplay

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

ifeq ($(CFG_HI_TEE_SUPPORT),y)
CFLAGS := -DHI_TEE_SUPPORT
CFLAGS += -I$(HI_TEE_OS_DIR)/libteec/inc

DEPEND_LIBS += -lteec -lc_sec
endif

include $(SAMPLE_DIR)/hi_sample_rules.mak
