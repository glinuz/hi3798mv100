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
CFLAGS := -I$(SAMPLE_DIR)/openssl/adp

DEPEND_LIBS := $(HI_LIBS)

DEPEND_LIBS += -lssl -lcrypto

SAMPLE_IMAGES := openssl_server_test

LOCAL_OBJS := openssl_server_test.o openssl_server.o ../adp/openssl_adp.o

include $(SAMPLE_DIR)/hi_sample_rules.mak