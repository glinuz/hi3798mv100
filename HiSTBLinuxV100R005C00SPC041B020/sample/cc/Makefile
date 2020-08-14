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

CFLAGS += -DSTREAM_TYPE_CAPTURE_86

SAMPLE_IMAGES := sample_cc

LOCAL_OBJS := sample_cc_out.o sample_cc_data.o sample_cc_xds.o $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
