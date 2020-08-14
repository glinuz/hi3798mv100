#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../..

ifeq ($(wildcard $(SDK_DIR)/base.mak),$(SDK_DIR)/base.mak)
include $(SDK_DIR)/base.mak
endif
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS += $(CFG_HI_CFLAGS)

ifndef ADVCA_DIR
ADVCA_DIR = $(COMPONENT_DIR)/advca
endif

ifeq ($(CFG_HI_ADVCA_TYPE), NAGRA)
CFG_ADVCA_PATH_NAME = nagra
ADVCA_TYPE_DIR = nagra

ADVCA_COMMON_DIR = $(ADVCA_DIR)/common
ADVCA_COMMON_SRC_DIR = $(ADVCA_DIR)/common_src

export SDK_DIR ADVCA_DIR ADVCA_TYPE_DIR ADVCA_COMMON_SRC_DIR ADVCA_COMMON_DIR CFG_ADVCA_PATH_NAME
#===============================================================================
# rules
#===============================================================================
.PHONY : all clean install uninstall

all :
	make -C $(ADVCA_TYPE_DIR) all

clean:
	make -C $(ADVCA_TYPE_DIR) clean

install:
	make -C $(ADVCA_TYPE_DIR) install

uninstall: clean
	make -C $(ADVCA_TYPE_DIR) uninstall

else
all :
	echo "done"

clean:
	echo "done"   

install:
	echo "done" 

uninstall: clean
	echo "done" 
endif

