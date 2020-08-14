#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects :=

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8188CUS),y)
objects += usb_rtl8188cu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8192CU),y)
objects += usb_rtl8188cu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8188EUS),y)
objects += usb_rtl8188eu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8192EU),y)
objects += usb_rtl8192eu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8192DU),y)
objects += usb_rtl8192du
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8812AU),y)
objects += usb_rtl8812au
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8822BU),y)
objects += usb_rtl8822bu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8723BU),y)
objects += usb_rtl8723bu
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8821AU),y)
objects += usb_rtl8821au
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RTL8192EE),y)
objects += pcie_rtl8192ee
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RT5370),y)
objects += usb_rt5370
endif

ifeq ($(CFG_HI_WIFI_DEVICE_RT5572),y)
objects += usb_rt5572
endif

ifeq ($(CFG_HI_WIFI_DEVICE_MT7601U),y)
objects += usb_mt7601u
endif

ifeq ($(CFG_HI_WIFI_DEVICE_AR9271),y)
objects += usb_ar9271
endif

ifeq ($(CFG_HI_WIFI_DEVICE_AR9374),y)
objects += usb_ar9374
endif

ifeq ($(CFG_HI_WIFI_DEVICE_QCA1021G),y)
objects += usb_ar9374
endif

ifeq ($(CFG_HI_WIFI_DEVICE_QCA1021X),y)
objects += usb_ar9374
endif

ifeq ($(CFG_HI_WIFI_DEVICE_BCM43236),y)
objects += usb_bcm43xx
endif

ifeq ($(CFG_HI_WIFI_DEVICE_MT76X2U),y)
objects += usb_mt76x2u
BIN_DIR := $(HI_ROOTFS_DIR)/etc/Wireless/RT2870STA
FW_DIR := $(HI_ROOTFS_DIR)/lib/firmware
BIN_SRC := $(COMPONENT_DIR)/wifi/drv/usb_mt76x2u/JEDI.MP2.mt76x2u.wifi.v3.3.1/conf/MT76x2STA.dat
FW_SRC := $(COMPONENT_DIR)/wifi/drv/usb_mt76x2u/JEDI.MP2.mt76x2u.wifi.v3.3.1/mcu/bin/mt7662_firmware_e3_tvbox.bin \
          $(COMPONENT_DIR)/wifi/drv/usb_mt76x2u/JEDI.MP2.mt76x2u.wifi.v3.3.1/mcu/bin/mt7662_patch_e3_hdr.bin \
          $(COMPONENT_DIR)/wifi/drv/usb_mt76x2u/JEDI.MP2.mt76x2u.wifi.v3.3.1/mcu/bin/mt7662t_firmware_e1.bin \
          $(COMPONENT_DIR)/wifi/drv/usb_mt76x2u/JEDI.MP2.mt76x2u.wifi.v3.3.1/mcu/bin/mt7662t_patch_e1_hdr.bin
endif

ifeq ($(CFG_HI_WIFI_DEVICE_MT7662TE),y)
objects += pcie_mt76x2te
BIN_DIR := $(HI_ROOTFS_DIR)/etc/Wireless/RT2870STA
FW_DIR := $(HI_ROOTFS_DIR)/lib/firmware
BIN_SRC := $(COMPONENT_DIR)/wifi/drv/pcie_mt76x2te/JEDI.MT76x2_WLDRV.MP2-PCIe/conf/MT76x2STA.dat
FW_SRC := $(COMPONENT_DIR)/wifi/drv/pcie_mt76x2te/JEDI.MT76x2_WLDRV.MP2-PCIe/mcu/bin/mt7662_firmware_e3_tvbox.bin \
          $(COMPONENT_DIR)/wifi/drv/pcie_mt76x2te/JEDI.MT76x2_WLDRV.MP2-PCIe/mcu/bin/mt7662_patch_e3_hdr.bin \
          $(COMPONENT_DIR)/wifi/drv/pcie_mt76x2te/JEDI.MT76x2_WLDRV.MP2-PCIe/mcu/bin/mt7662t_firmware_e1.bin \
          $(COMPONENT_DIR)/wifi/drv/pcie_mt76x2te/JEDI.MT76x2_WLDRV.MP2-PCIe/mcu/bin/mt7662t_patch_e1_hdr.bin
endif

ifeq ($(CFG_HI_WIFI_DEVICE_QCA6174),y)
objects += pcie_qca6174
endif

obj-y += $(addsuffix /,$(objects))

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean

all:
ifeq ($(CFG_HI_WIFI_DEVICE_QCA6174),y)
#	$(AT)make -C pcie_qca6174
endif

ifeq ($(CFG_HI_WIFI_DEVICE_MT76X2U),y)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)
	@cp -v $(BIN_SRC) $(BIN_DIR)/
	@test -d $(FW_DIR) || mkdir -p $(FW_DIR)
	@cp -v $(FW_SRC) $(FW_DIR)/
endif

ifeq ($(CFG_HI_WIFI_DEVICE_MT7662TE),y)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)
	@cp -v $(BIN_SRC) $(BIN_DIR)/
	@test -d $(FW_DIR) || mkdir -p $(FW_DIR)
	@cp -v $(FW_SRC) $(FW_DIR)/
endif

clean:
ifeq ($(CFG_HI_WIFI_DEVICE_QCA6174),y)
#	$(AT)make -C pcie_qca6174 clean
endif
