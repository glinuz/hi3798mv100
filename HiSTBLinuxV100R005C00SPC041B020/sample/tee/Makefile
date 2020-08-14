#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

CFLAGS += $(CFG_HI_CFLAGS)
CFLAGS += -I$(TEE_API_INCLUDE)
CFLAGS += -I$(COMMON_API_INCLUDE)
CFLAGS += -I$(COMMON_UNF_INCLUDE)

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_tee_tsplay \
                 sample_tee_tsplay_pip \
                 sample_tee_esplay

ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
    SAMPLE_IMAGES +=sample_tee_dvbplay
endif

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716dv100 hi3796mv100 hi3798mv100),)
all :
	@echo "$(CFG_HI_CHIP_TYPE) Not Support"
else
include $(SAMPLE_DIR)/hi_sample_rules.mak
endif

