################################################################################
#    ./Makefile
#    Create By CaiZhiYong
#    2011.06.19
#
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

CROSS_COMPILE   := $(HI_KERNEL_TOOLCHAINS_NAME)-
OUTPUT_DIR      := $(HI_IMAGE_DIR)
KERNEL          := $(CFG_HI_KERNEL_VERSION)
KERNEL_CONFIG   := $(CFG_HI_KERNEL_CFG)
CFG_FS_BUILDIN  := $(CFG_HI_FS_BUILDIN)

################################################################################
KERNEL_OUT_DIR := $(LINUX_DIR)

ifeq ($(KERNEL_SRC_DIR),$(KERNEL_OUT_DIR))
BUILD_DIR :=
else
BUILD_DIR := $(KERNEL_OUT_DIR)

$(shell mkdir -p $(KERNEL_OUT_DIR))
endif

################################################################################

KERNEL_PATCH_DIR ?= $(KERNEL_DIR)/$(KERNEL).patch

ifeq ($(findstring fpga_umap5, $(CFG_HI_BOOT_REG_NAME)), fpga_umap5)
CONFIG_HI3796MV2X_FPGA = 1
endif

ifeq ($(findstring fpga_umap6, $(CFG_HI_BOOT_REG_NAME)), fpga_umap6)
CONFIG_HI3796MV2X_FPGA = 1
endif

ifdef CFG_HI_ADVCA_FUNCTION
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
INITRAMFS_SOURCE = $(KERNEL_DIR)/initramfs.cpio.gz
endif
else
INITRAMFS_SOURCE = $(HI_ROOTBOX_DIR)
endif

ifeq ($(CFG_HI_CPU_ARCH),arm64)
FIP_SRC_DIR = $(KERNEL_DIR)/arm-trusted-firmware
FIP_IMG = $(FIP_OUT_DIR)/build/$(CFG_HI_CHIP_TYPE)/debug/fip.bin
BUILD_TGT_IMG = $(FIP_IMG)

else
BUILD_TGT_IMG = $(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage
endif

################################################################################
CONFIG_TOOL=$(KERNEL_SRC_DIR)/scripts/config

################################################################################
# TVP Config                                                                   #
################################################################################
ifeq ($(CFG_HI_TEE_SUPPORT),y)

tee_config:
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_SUPPORT_CA
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_CA_WARKUP_CHECK
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --disable CONFIG_CA_SUPPORT_ADVCA
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_TEE
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_TEE_HISI
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_SECURE_EXTENSION
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_TZDRIVER
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_TEE_LIBHWSECUREC
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --enable CONFIG_DEVCHIP_PLATFORM
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --disable CONFIG_TEELOG
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --disable CONFIG_TC_DEBUG
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --disable CONFIG_TEE_TUI
	$(CONFIG_TOOL) --file $(KERNEL_OUT_DIR)/.config --disable CONFIG_HISI_SPI_SMC

else
DISABLE_TEE = 1
endif
################################################################################

all: $(BUILD_TGT_IMG)
	$(AT)test -d $(OUTPUT_DIR) || mkdir -p $(OUTPUT_DIR)
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)cp -f $(BUILD_TGT_IMG) $(OUTPUT_DIR)/hi_kernel.bin
endif
	$(AT)mkdir -p $(HI_MODULE_DIR)/usb
	$(AT)-cp -f $(KERNEL_OUT_DIR)/drivers/usb/host/*.ko $(HI_MODULE_DIR)/usb/
	$(AT)(for file in $(wildcard $(KERNEL_OUT_DIR)/drivers/ata/*.ko) ; do ( \
			mkdir -p $(HI_MODULE_DIR)/sata && cp $$file $(HI_MODULE_DIR)/sata/ \
		) done )
ifeq ($(HI_DRV_BUILDTYPE),m)
	$(AT)(find $(KERNEL_OUT_DIR)/drivers/common -name "*.ko" | xargs cp -f -t $(HI_MODULE_DIR)/)
	$(AT)(find $(KERNEL_OUT_DIR)/drivers/msp -name "*.ko" | xargs cp -f -t $(HI_MODULE_DIR)/)
endif
ifeq ($(CFG_HI_WIFI_SUPPORT),y)
	$(AT)-cp -f $(KERNEL_OUT_DIR)/net/wireless/cfg80211.ko $(HI_MODULE_DIR)/
	$(AT)(find $(KERNEL_OUT_DIR)/drivers/wifi -name "*.ko" | xargs cp -f -t $(HI_MODULE_DIR)/)
endif
ifeq ($(CFG_HI_BLUETOOTH_SUPPORT),y)
	$(AT)test -d $(HI_ROOTFS_DIR)/lib/firmware || mkdir -p $(HI_ROOTFS_DIR)/lib/firmware
	$(AT)(find $(KERNEL_OUT_DIR)/drivers/bluetooth_usb -name "*.ko" | xargs cp -f -t $(HI_MODULE_DIR)/)
	$(AT)(find $(SDK_DIR)/source/component/bluetooth -name "*_fw" | xargs cp -f -t $(HI_ROOTFS_DIR)/lib/firmware)
	$(AT)(find $(SDK_DIR)/source/component/bluetooth -name "*_config" | xargs cp -f -t $(HI_ROOTFS_DIR)/lib/firmware)
	$(AT)(find $(SDK_DIR)/source/component/bluetooth -name "*.bin" | xargs cp -f -t $(HI_ROOTFS_DIR)/lib/firmware)
endif


ifeq ($(CFG_HI_CPU_ARCH),arm64)

ifeq ($(CFG_HI_TEE_SUPPORT),y)
ATF_EXTRA_FLAGS = "-DCONFIG_TEE_SUPPORT -DCONFIG_TEE_MEM_LAYOUT_$(CFG_HI_TVP_MEM_LAYOUT)"
endif

ifeq ($(DISABLE_TEE),1)
ifeq ($(CONFIG_HI3796MV2X_FPGA),1)
$(FIP_IMG): $(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) SPD=none CONFIG_HI3796MV2X_FPGA=$(CONFIG_HI3796MV2X_FPGA) BL33=$< DEBUG=1 DISABLE_TEE=$(DISABLE_TEE) fip O=$(FIP_OUT_DIR) ATF_EXT_FLAGS=$(ATF_EXTRA_FLAGS)
else
$(FIP_IMG): $(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) SPD=none BL33=$< DEBUG=1 DISABLE_TEE=$(DISABLE_TEE) fip O=$(FIP_OUT_DIR) ATF_EXT_FLAGS=$(ATF_EXTRA_FLAGS)
endif
else
ifeq ($(CONFIG_HI3796MV2X_FPGA),1)
$(FIP_IMG): $(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) CONFIG_HI3796MV2X_FPGA=$(CONFIG_HI3796MV2X_FPGA) NEED_BL32=no BL33=$< DEBUG=1 fip O=$(FIP_OUT_DIR) ATF_EXT_FLAGS=$(ATF_EXTRA_FLAGS)
else
$(FIP_IMG): $(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) NEED_BL32=no BL33=$< DEBUG=1 fip O=$(FIP_OUT_DIR) ATF_EXT_FLAGS=$(ATF_EXTRA_FLAGS)
endif
endif
endif

$(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage: prepare
	make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		CONFIG_MSP=y \
		HI_CONFIG_WIFI=$(CFG_HI_WIFI_SUPPORT) \
		HI_CONFIG_BLUETOOTH=$(CFG_HI_BLUETOOTH_SUPPORT) \
		CONFIG_ADVCA=$(CFG_ADVCA) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) AFLAGS_KERNEL=$(AFLAGS_KERNEL) uImage modules

menuconfig: prepare
	make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) AFLAGS_KERNEL=$(AFLAGS_KERNEL) menuconfig

prepare:
	$(AT)test -d $(KERNEL) || (tar -xjf $(OPEN_SOURCE_DIR)/$(KERNEL).tar.bz2 \
			&& cp -rf $(KERNEL_PATCH_DIR)/* $(KERNEL_SRC_DIR)/ \
			&& cp -f $(KERNEL_SRC_DIR)/include/linux/vinput.h $(MSP_DIR)/drv/include/hi_drv_vinput.h \
		)
	$(AT)(cd $(KERNEL_SRC_DIR)/drivers && rm -rf common && ln -s $(COMMON_DIR)/drv common)
	$(AT)(cd $(KERNEL_SRC_DIR)/drivers && rm -rf msp && ln -s $(MSP_DIR)/drv msp)
	$(AT)(cd $(KERNEL_SRC_DIR)/drivers && rm -rf wifi && ln -s $(COMPONENT_DIR)/wifi/drv wifi)
	$(AT)(cd $(KERNEL_SRC_DIR)/drivers && rm -rf bluetooth_usb && ln -s $(COMPONENT_DIR)/bluetooth/drv bluetooth_usb)
	$(AT)make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) AFLAGS_KERNEL=$(AFLAGS_KERNEL) $(KERNEL_CONFIG)
	$(if $(CFG_HI_TEE_SUPPORT),make tee_config,)
	$(AT)make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) AFLAGS_KERNEL=$(AFLAGS_KERNEL) oldconfig

ifdef CFG_HI_ADVCA_FUNCTION
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
$(INITRAMFS_SOURCE): force
	@rm -f $(INITRAMFS_SOURCE)
	@cd $(HI_ROOTFS_DIR) && \
	    find . | $(CPIO) -o -H newc -T $(RULEFILE) -G $(RULEDBG) | gzip > $(INITRAMFS_SOURCE) && \
        cd -

force: ;
endif
endif

fs_buildin: prepare $(INITRAMFS_SOURCE)
	make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		CONFIG_MSP=y \
		HI_CONFIG_WIFI=$(CFG_HI_WIFI_SUPPORT) \
		HI_CONFIG_BLUETOOTH=$(CFG_HI_BLUETOOTH_SUPPORT) \
		CONFIG_ADVCA=$(CFG_ADVCA) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) \
		AFLAGS_KERNEL=$(AFLAGS_KERNEL) \
		CONFIG_CC_OPTIMIZE_FOR_SIZE=$(CONFIG_CC_OPTIMIZE_FOR_SIZE) \
		CONFIG_INITRAMFS_SOURCE=$(INITRAMFS_SOURCE) \
		uImage
ifeq ($(CFG_HI_CPU_ARCH),arm64)
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) $(if $(DISABLE_TEE), SPD=none,) BL33=$(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage DEBUG=1 DISABLE_TEE=$(DISABLE_TEE) CONFIG_HI3796MV2X_FPGA=$(CONFIG_HI3796MV2X_FPGA) fip O=$(FIP_OUT_DIR)
endif
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)cp -f $(BUILD_TGT_IMG) $(OUTPUT_DIR)/apploader.bin
else
	$(AT)cp -f $(BUILD_TGT_IMG) $(OUTPUT_DIR)/hi_kernel.bin
endif

################################################################################

distclean: clean
	@test ! -d $(KERNEL_SRC_DIR) || make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) \
		AFLAGS_KERNEL=$(AFLAGS_KERNEL) distclean
ifeq ($(CFG_HI_CPU_ARCH),arm64)
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) $(if $(DISABLE_TEE), SPD=none,) BL33=$(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage DEBUG=1 DISABLE_TEE=$(DISABLE_TEE) O=$(FIP_OUT_DIR) distclean
endif

clean:
	$(AT)test ! -f $(OPEN_SOURCE_DIR)/$(KERNEL).tar.bz2 || rm -rf $(KERNEL_SRC_DIR)
	$(AT)test ! -d $(KERNEL_SRC_DIR) || make -C $(KERNEL_SRC_DIR) ARCH=$(CFG_HI_CPU_ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		O=$(BUILD_DIR) \
		$(EXTRA_FLAGS_KERNEL) \
		CFLAGS_KERNEL=$(CFLAGS_KERNEL) \
		AFLAGS_KERNEL=$(AFLAGS_KERNEL) clean
ifeq ($(CFG_HI_CPU_ARCH),arm64)
	make -C $(FIP_SRC_DIR) CROSS_COMPILE=$(CROSS_COMPILE) PLAT=$(CFG_HI_CHIP_TYPE) $(if $(DISABLE_TEE), SPD=none,) BL33=$(KERNEL_OUT_DIR)/arch/$(CFG_HI_CPU_ARCH)/boot/uImage DEBUG=1 DISABLE_TEE=$(DISABLE_TEE) O=$(FIP_OUT_DIR) distclean
endif
ifdef CFG_HI_ADVCA_FUNCTION
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	@test ! -f $(INITRAMFS_SOURCE) || rm -f $(INITRAMFS_SOURCE)
endif
endif
	$(AT)rm -rf $(OUTPUT_DIR)/hi_kernel.bin $(OUTPUT_DIR)/apploader.bin
	$(AT)rm -rf $(HI_MODULE_DIR)

#### for android ####
linux_prepare:
	$(AT)(cd $(LINUX_DIR)/drivers && rm -rf common && ln -s $(COMMON_DIR)/drv common)
	$(AT)(cd $(LINUX_DIR)/drivers && rm -rf msp && ln -s $(MSP_DIR)/drv msp)
	$(AT)(cd $(LINUX_DIR)/drivers && rm -rf wifi && ln -s $(COMPONENT_DIR)/wifi/drv wifi)
	$(AT)(cd $(LINUX_DIR)/drivers && rm -rf bluetooth_usb && ln -s $(COMPONENT_DIR)/bluetooth/drv bluetooth_usb)

linux_unprepare:
	$(AT)rm -rf $(LINUX_DIR)/drivers/common $(LINUX_DIR)/drivers/msp $(LINUX_DIR)/drivers/wifi $(LINUX_DIR)/drivers/bluetooth_usb

################################################################################
.PHONY: all clean distclean prepare linux_prepare linux_unprepare
################################################################################
