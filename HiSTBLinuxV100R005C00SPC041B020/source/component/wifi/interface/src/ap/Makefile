#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS += $(CFG_HI_CFLAGS)
CFLAGS += -I$(SDK_DIR)/source/common/include
CFLAGS += -I$(CURDIR)/../util
CFLAGS += -I$(CURDIR)/../../include

SRCS := hi_wlan_ap.c \
        ../util/wlan_util.c

ifneq ($(DEBUG),y)
LIB_NAME   := libwlanap
else
LIB_NAME   := libwlanap_dbg
endif

HEADER_FILES := $(SDK_DIR)/source/component/wifi/interface/include/hi_wlan_ap.h

include $(SDK_DIR)/hi_rules.mak