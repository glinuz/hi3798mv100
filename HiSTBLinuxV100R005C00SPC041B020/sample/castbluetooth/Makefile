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
ifeq ($(CFG_HI_BLUETOOTH_SUPPORT),y)
CFLAGS := -I$(HI_INCLUDE_DIR)/alsa_bluez

LIB_PATHS := -L$(HI_SHARED_LIB_DIR)/alsa_bluez
endif

SAMPLE_IMAGES  := sample_bluetooth sample_cast_bluetooth

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS) -lhi_bluetooth -lasound -lbluetooth -ldbus-1

include $(SAMPLE_DIR)/hi_sample_rules.mak
