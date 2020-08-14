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
CFLAGS := -I$(MSP_DRV_INCLUDE)

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o) hi_adp_hdmi_rx.o

include $(SAMPLE_DIR)/hi_sample_rules.mak