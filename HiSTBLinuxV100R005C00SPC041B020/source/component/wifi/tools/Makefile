#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects := libnl wireless_tools 

ifeq ($(CFG_HI_WIFI_MODE_STA),y)
objects += wpa_supplicant
endif

ifeq ($(CFG_HI_WIFI_MODE_AP),y)
objects += hostapd
endif

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall
all: $(objects)
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
wpa_supplicant: libnl
endif
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
hostapd: libnl
endif

clean: $(objects_clean)

install: $(objects_install)
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
wpa_supplicant_install: libnl_install
endif
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
hostapd_install: libnl_install
endif

uninstall: $(objects_uninstall)

prepare:

unprepare:

#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

$(objects):
	make -C $@ all

$(objects_clean):
	make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	make -C $(patsubst %_install,%,$@) install

$(objects_uninstall):
	make -C $(patsubst %_uninstall,%,$@) uninstall

