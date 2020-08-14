#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../..
include $(SDK_DIR)/base.mak
endif

#CFLAGS := ARCH=arm CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)- LINUX_DIR=$(LINUX_DIR)
#CFLAGS += ROOTFS_DIR=$(HI_ROOTFS_DIR)
#CFLAGS += CFG_HI_KERNEL_VERSION=$(CFG_HI_KERNEL_VERSION)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects := 

ifeq ($(CFG_HI_BLUETOOTH_DEVICE_RTL8723BU),y)
objects += usb_rtl8723bu
endif

ifeq ($(CFG_HI_BLUETOOTH_DEVICE_RTL8761AU),y)
objects += usb_rtl8761au
endif

ifeq ($(CFG_HI_BLUETOOTH_DEVICE_RTL8821AU),y)
objects += usb_rtl8821au
endif

ifeq ($(CFG_HI_BLUETOOTH_DEVICE_RTL8822BU),y)
objects += usb_rtl8822bu
endif

ifeq ($(CFG_HI_BLUETOOTH_DEVICE_MT76X2),y)
objects += usb_mt76x2
endif

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

obj-y += $(addsuffix /,$(objects))
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: all clean install uninstall
all: 

clean: 

install: 

uninstall: 

