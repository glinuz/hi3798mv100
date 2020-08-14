#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS := -I$(CURDIR)/include
CFLAGS += -I$(MSP_DIR)/api/higo/include

SRCS :=  src/loader_osal.c
SRCS +=  src/loader_crc32.c
SRCS +=  src/loader_dbinfo.c
SRCS +=  src/loader_dbharden.c
SRCS +=  src/hi_loader_info.c
SRCS +=  src/hi_loader_info_v2.c

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
SRCS +=  src/loader_cipher.c
endif

LIB_NAME := libhiloader

HEADER_FILES := $(CURDIR)/include/hi_loader_info.h
HEADER_FILES += $(CURDIR)/include/hi_loader_info_v2.h

include $(SDK_DIR)/hi_rules.mak
