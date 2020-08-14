#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

BLUETOOTH_DIR = $(COMPONENT_DIR)/bluetooth
BLUEZ_API_DIR = $(BLUETOOTH_DIR)/hi_api
BLUETOOTH_DRV_DIR = $(BLUETOOTH_DIR)/drv

#===============================================================================
# local variable
#===============================================================================

#===============================================================================
# rules
#===============================================================================
.PHONY : all clean

all :
	$(AT)cd $(BLUETOOTH_DIR)/src;tar -zxf bluez_install.tar.gz;chmod 777 -R install;cd -
	$(AT)echo "tar -zxf bluez_install.tar.gz  done"
	$(AT)mkdir -p $(BLUETOOTH_DIR)/include && cp -rf $(BLUETOOTH_DIR)/src/install/include/* $(BLUETOOTH_DIR)/include
	$(AT)mkdir -p $(BLUETOOTH_DIR)/lib && cp -rf $(BLUETOOTH_DIR)/src/install/usr/share/bluetooth/lib/* $(BLUETOOTH_DIR)/lib
	$(AT)make -C $(BLUEZ_API_DIR)
	$(AT)make -C $(BLUETOOTH_DRV_DIR)
	$(AT)make -C $(BLUEZ_API_DIR) install
	$(AT)make -C $(BLUETOOTH_DRV_DIR) install
	$(AT)mkdir -p $(HI_INCLUDE_DIR)/alsa_bluez
	$(AT)mkdir -p $(HI_SHARED_LIB_DIR)/alsa_bluez
	$(AT)cp -rf $(BLUETOOTH_DIR)/src/install/include/* $(HI_INCLUDE_DIR)/alsa_bluez
	$(AT)cp -rf $(BLUETOOTH_DIR)/src/install/usr/share/bluetooth/lib/* $(HI_SHARED_LIB_DIR)/alsa_bluez
	$(AT)mkdir -p $(HI_ROOTFS_DIR)/etc && cp -rf $(BLUETOOTH_DIR)/src/install/etc/* $(HI_ROOTFS_DIR)/etc
	$(AT)cp -rf $(BLUETOOTH_DIR)/src/install/kmod/* $(HI_MODULE_DIR)
	$(AT)for aaa in $$(find $(BLUETOOTH_DIR)/src/install/usr/share/bluetooth/lib -name "*.a");do rm -rf $$aaa;done
	$(AT)mkdir -p $(HI_ROOTFS_DIR)/usr && cp -rf $(BLUETOOTH_DIR)/src/install/usr/* $(HI_ROOTFS_DIR)/usr

clean:
	$(AT)make -C $(BLUEZ_API_DIR) clean
	$(AT)make -C $(BLUETOOTH_DRV_DIR) clean
	$(AT)rm -rf $(BLUETOOTH_DIR)/lib/*
	$(AT)rm -rf $(BLUETOOTH_DIR)/include/*
	$(AT)rm -rf $(BLUETOOTH_DIR)/src/install
	$(AT)make -C $(BLUEZ_API_DIR) uninstall
	$(AT)make -C $(BLUETOOTH_DRV_DIR) uninstall
	$(AT)rm -rf $(HI_INCLUDE_DIR)/alsa_bluez
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/alsa_bluez
	$(AT)rm -rf $(HI_MODULE_DIR)/bluetoothd_server $(HI_MODULE_DIR)/bluez_init
	$(AT)rm -rf $(HI_ROOTFS_DIR)/etc/asound.conf $(HI_ROOTFS_DIR)/etc/bluez_init \
                $(HI_ROOTFS_DIR)/etc/group $(HI_ROOTFS_DIR)/etc/passwd

