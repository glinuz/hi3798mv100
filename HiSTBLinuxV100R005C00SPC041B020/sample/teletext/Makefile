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
ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
CFLAGS := -DHI_FRONTEND_SUPPORT
endif

SAMPLE_IMAGES := sample_ts_ttx

LOCAL_OBJS := sample_teletext_font.o sample_teletext_msg.o sample_teletext_out.o sample_ttx_port.o

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
