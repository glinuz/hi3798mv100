#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(CURDIR)/../include/

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
CFLAGS  += -DHI_ADVCA_FUNCTION_RELEASE
endif

SRCS := omx_core.c

LIB_NAME := libOMX_Core

include $(SDK_DIR)/hi_rules.mak
