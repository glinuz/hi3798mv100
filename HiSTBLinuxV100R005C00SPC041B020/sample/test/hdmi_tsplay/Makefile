#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP), y)
CFLAGS += -DHI_HDCP_SUPPORT
endif

SAMPLE_IMAGES := sample_hdmi_tsplay \
    sample_hdmi_subplug \
    sample_hdmi_cec

LOCAL_OBJS := hdmi_test_cmd.o

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
