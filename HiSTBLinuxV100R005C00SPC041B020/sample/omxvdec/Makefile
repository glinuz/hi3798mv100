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
SAMPLE_IMAGES := sample_omxvdec

LOCAL_OBJS := sample_omxvdec.o sample_queue.o sample_tidx.o

DEPEND_LIBS := $(HI_LIBS) $(SYS_LIBS) -lOMX_Core

CFLAGS += -I$(MSP_DIR)/api/omx/include

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DHI_SMMU_SUPPORT
endif

ifeq ($(CFG_HI_TEE_SUPPORT), y)
CFLAGS += -DHI_OMX_TEE_SUPPORT
CFLAGS += -I$(HI_TEE_OS_DIR)/libteec/inc/
CFLAGS += -I$(COMMON_API_INCLUDE)/
DEPEND_LIBS += -lteec -lc_sec
endif

include $(SAMPLE_DIR)/hi_sample_rules.mak


