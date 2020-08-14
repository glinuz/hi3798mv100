#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ifeq ($(CFG_HI_64BIT_SYSTEM),y)
OBJ_NAME := obj64
else
OBJ_NAME := obj
endif

ifneq ($(HI_OUT_DIR),)
BUILD_DIR := $(HI_OUT_DIR)/$(OBJ_NAME)/source/component/wifi/tools/wpa_supplicant
LIBNL_DIR := $(HI_OUT_DIR)/$(OBJ_NAME)/source/component/wifi/tools/libnl/_install
else
BUILD_DIR := $(CURDIR)/tmp
LIBNL_DIR := $(COMPONENT_DIR)/wifi/tools/libnl/_install
endif

HOST := $(HI_KERNEL_TOOLCHAINS_NAME)
CROSS_COMPILE := $(HOST)-
INSTALL_DIR := $(HI_ROOTFS_DIR)/sbin

WPACONF_INSTALL_DIR := $(HI_ROOTFS_DIR)/etc/Wireless

MODULE_SRC := wpa_supplicant_8
MODULE := wpa_supplicant wpa_cli
MODULE_PATH := $(BUILD_DIR)/$(MODULE_SRC)/wpa_supplicant/wpa_supplicant \
	$(BUILD_DIR)/$(MODULE_SRC)/wpa_supplicant/wpa_cli

WPACONF := wpa_supplicant.conf
WPACONF_PATH := $(BUILD_DIR)/$(MODULE_SRC)/wpa_supplicant/wpa_supplicant.conf

BUILD_MODULES = $(patsubst %,%_build,${MODULE_SRC})
UNINSTALL_MODULES = $(patsubst %,%_uninstall,${MODULE})

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall
all: $(BUILD_MODULES)
	@test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	@cp -v $(MODULE_PATH) $(INSTALL_DIR)/
	@test -d $(WPACONF_INSTALL_DIR) || mkdir -p $(WPACONF_INSTALL_DIR)
	@cp -v $(WPACONF_PATH) $(WPACONF_INSTALL_DIR)/

clean:
	@rm -rf $(BUILD_DIR)

install: all

uninstall: $(UNINSTALL_MODULES)
	@rm -f $(WPACONF_INSTALL_DIR)/$(WPACONF)

#==================================================================
#                          modules
#==================================================================
%_build: $(BUILD_DIR)
	@echo -e "\033[31m""Build $(patsubst %_build,%,$@) ...""\033[00m"
	@test -d $(BUILD_DIR)/$(patsubst %_build,%,$@) \
		|| tar -zxf $(patsubst %_build,%,$@).tar.gz -C $(BUILD_DIR)
	cd $(BUILD_DIR)/$(patsubst %_build,%,$@)/wpa_supplicant && \
	cp -fv defconfig .config && \
	(echo ""; \
		echo "CONFIG_DRIVER_NL80211=y"; \
		echo "CONFIG_LIBNL20=y"; \
		echo "CONFIG_IEEE80211N=y"; \
		echo "CONFIG_TLS=internal"; \
		echo "CONFIG_INTERNAL_LIBTOMMATH=y"; \
		echo "CFLAGS += -I$(LIBNL_DIR)/include"; \
		echo "LIBS += -L$(LIBNL_DIR)/lib"; \
		echo "LIBS_p += -L$(LIBNL_DIR)/lib") >> .config && \
	make -j 16 CC=$(HOST)-gcc && \
	cd -

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)
