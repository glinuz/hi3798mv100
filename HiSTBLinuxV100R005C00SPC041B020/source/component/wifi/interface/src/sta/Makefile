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

SRCS := hi_wlan_sta.c \
        wlan_sta.c \
        ../util/wlan_util.c

ifneq ($(DEBUG),y)
LIB_NAME   := libwlansta
else
LIB_NAME   := libwlansta_dbg
endif

HEADER_FILES := $(SDK_DIR)/source/component/wifi/interface/include/hi_wlan_sta.h

include $(SDK_DIR)/hi_rules.mak