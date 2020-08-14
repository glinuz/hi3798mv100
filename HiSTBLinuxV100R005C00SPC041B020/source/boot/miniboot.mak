################################################################################
#    source/boot/Makefile
#    Create By Lyj
#    2014.03.17
################################################################################
ifneq ($(BOOT_ADVCA_PROGRAMMER),y) 
CONFIG_PRODUCT_WITH_BOOT := $(CFG_HI_BUILD_WITH_PRODUCT)
endif

################################################################################
#CONFIG_AUXCODE_COMPILE_SUPPORT=y
################################################################################


TARGET := miniboot.bin

#
# local variable.
#
CURRDIR        = $(shell pwd)

ifneq ($(O),)
OBJ_DIR := $(O)
BUILD_DIR := $(OBJ_DIR)
OBJ_TARGET := $(OBJ_DIR)/miniboot.bin
BOOT_DIR := $(OBJ_DIR)
export BOOT_DIR
PREFIX         = $(OBJ_DIR)
else
BUILD_DIR := $(CURRDIR)
PREFIX         = $(HI_IMAGE_DIR)
endif

FASTBOOT       = $(BUILD_DIR)/fastboot
PRODUCT        = $(BUILD_DIR)/product
MINIBOOT       = $(BUILD_DIR)/miniboot
AUXCODE        = $(BUILD_DIR)/auxcode
SYSREGDIR      = $(CURRDIR)/sysreg
LIBSPRODUCT    = $(PRODUCT)/libproduct.a
AUXIMG         = $(FASTBOOT)/auxcode_sign.img

BOOTCONFIG     = s40

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
BOOTCONFIG     = hi3798mx
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
BOOTCONFIG     = hi3716mv410
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100),)
BOOTCONFIG     = hi3798cv2x
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3796mv200),)
BOOTCONFIG     = hi3796mv200
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
BOOTCONFIG     = hi3798mv2x
endif

export BOOTCONFIG

CONFIG_SYSREG  := $(CFG_HI_BOOT_REG_NAME)
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT), y)
CONFIG_SYSREG1  := $(CFG_HI_BOOT_REG1_NAME)
CONFIG_SYSREG2  := $(CFG_HI_BOOT_REG2_NAME)
CONFIG_SYSREG3  := $(CFG_HI_BOOT_REG3_NAME)
CONFIG_SYSREG4  := $(CFG_HI_BOOT_REG4_NAME)
CONFIG_SYSREG5  := $(CFG_HI_BOOT_REG5_NAME)
CONFIG_SYSREG6  := $(CFG_HI_BOOT_REG6_NAME)
CONFIG_SYSREG7  := $(CFG_HI_BOOT_REG7_NAME)
CONFIG_UNIFIED_BOOT := $(CFG_HI_UNIFIED_BOOT_SUPPORT)
endif

CONFIG_BOOTREG_DEFAULT := $(firstword $(CONFIG_SYSREG) $(CONFIG_SYSREG1) \
	$(CONFIG_SYSREG2) $(CONFIG_SYSREG3) $(CONFIG_SYSREG4) \
	$(CONFIG_SYSREG5) $(CONFIG_SYSREG6) $(CONFIG_SYSREG7))
$(warning CONFIG_BOOTREG_DEFAULT=$(CONFIG_BOOTREG_DEFAULT))
SYSREG         = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CFG_HI_BOOT_REG_NAME)
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT), y)
SYSREG1        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG1)
SYSREG2        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG2)
SYSREG3        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG3)
SYSREG4        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG4)
SYSREG5        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG5)
SYSREG6        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG6)
SYSREG7        = $(SYSREGDIR)/$(CFG_HI_CHIP_TYPE)/$(CONFIG_SYSREG7)
endif

export PRODUCT MINIBOOT

ifeq ($(CFG_HI_CPU_ARCH),arm64)
CONFIG_ARM64_SUPPORT = y
endif

################################################################################
CONFIG_EXTERN_FLAGS := \
                $(if $(CFG_HI_SDK_VERSION),SDK_VERSION_STRING=\"$(CFG_HI_SDK_VERSION)\",) \
                $(if $(CFG_HI_NAND_SUPPORT),CONFIG_GENERIC_NAND=$(CFG_HI_NAND_SUPPORT),) \
                $(if $(CFG_HI_SPI_NAND_SUPPORT),CONFIG_GENERIC_SPI_NAND=$(CFG_HI_SPI_NAND_SUPPORT),) \
                $(if $(CFG_HI_SPI_SUPPORT),CONFIG_GENERIC_SFC=$(CFG_HI_SPI_SUPPORT),) \
                $(if $(CFG_HI_ETH_SUPPORT),CONFIG_NET=$(CFG_HI_ETH_SUPPORT),) \
                $(if $(CFG_HI_ETH_UP_PHYADDR),HISFV_PHY_U=$(CFG_HI_ETH_UP_PHYADDR),) \
                $(if $(CFG_HI_ETH_DOWN_PHYADDR),HISFV_PHY_D=$(CFG_HI_ETH_DOWN_PHYADDR),) \
                $(if $(CFG_HI_EMMC_SUPPORT),CONFIG_GENERIC_MMC=$(CFG_HI_EMMC_SUPPORT),) \
                $(if $(CFG_HI_BOOT_ENV_STARTADDR),CONFIG_ENV_FROM=$(CFG_HI_BOOT_ENV_STARTADDR),) \
                $(if $(CFG_HI_BOOT_ENV_SIZE),CONFIG_ENV_SIZE=$(CFG_HI_BOOT_ENV_SIZE),) \
                $(if $(CFG_HI_BOOT_ENV_RANGE),CONFIG_ENV_RANGE=$(CFG_HI_BOOT_ENV_RANGE),) \
                $(if $(CFG_HI_BOOT_ENV_BAK_SUPPORT),CONFIG_ENV_BACKUP=$(CFG_HI_BOOT_ENV_BAK_SUPPORT),) \
                $(if $(CFG_HI_BOOT_ENV_STARTADDR_BAKUP),CONFIG_ENV_BACKUP_FROM=$(CFG_HI_BOOT_ENV_STARTADDR_BAKUP),)\
                $(if $(CONFIG_PRODUCT_WITH_BOOT),CONFIG_PRODUCT_WITH_BOOT=$(CONFIG_PRODUCT_WITH_BOOT),) \
                $(if $(CFG_HI_CHIP_TYPE),CONFIG_CHIP_TYPE=$(CFG_HI_CHIP_TYPE),) \
                $(if $(CFG_HI_BOOT_REG_NAME),CONFIG_BOOT_REG_NAME=$(CFG_HI_BOOT_REG_NAME),) \
                $(if $(CFG_HI_CHIP_TYPE),CHIP_TYPE_$(CFG_HI_CHIP_TYPE)=1,) \
                $(if $(CFG_HI_AVS_SUPPORT),CONFIG_AVS_SUPPORT=$(CFG_HI_AVS_SUPPORT),) \
                $(if $(CFG_HI_PMU_DEVICE_SELECT),CONFIG_PMU_DEVICE=$(CFG_HI_PMU_DEVICE_SELECT),) \
                $(if $(CFG_HI_BOOT_USB_SUPPORT),CONFIG_USB_SUPPORT=$(CFG_HI_BOOT_USB_SUPPORT),) \
                $(if $(CFG_HI_USER_MODE),CONFIG_DISABLE_CONSOLE_INPUT=$(CFG_HI_USER_MODE),) \
		$(if $(CFG_HI_BOOT_COMPRESSED),CONFIG_COMPRESS=$(CFG_HI_BOOT_COMPRESSED),) \
		$(if $(CFG_HI_MINIBOOT_SUPPORT),CFG_HI_MINIBOOT_SUPPORT=$(CFG_HI_MINIBOOT_SUPPORT),) \
		$(if $(CFG_HI_BOOT_LOG_SUPPORT),CONFIG_PRINT=y,CONFIG_PRINT=) \
		$(if $(CFG_HI_UNIFIED_BOOT_SUPPORT),CONFIG_UNIFIED_BOOT=$(CFG_HI_UNIFIED_BOOT_SUPPORT)) \
		$(if $(CONFIG_AUXCODE_COMPILE_SUPPORT),CONFIG_AUXCODE_COMPILE_SUPPORT=$(CONFIG_AUXCODE_COMPILE_SUPPORT)) \
		$(if $(CFG_HI_TEE_SUPPORT),CONFIG_TEE_SUPPORT=$(CFG_HI_TEE_SUPPORT)) \
		$(if $(CFG_HI_TVP_MEM_LAYOUT),CONFIG_TEE_MEM_LAYOUT_$(CFG_HI_TVP_MEM_LAYOUT)=1) \
		$(if $(CONFIG_ARM64_SUPPORT),CONFIG_ARM64_SUPPORT=$(CONFIG_ARM64_SUPPORT)) \
		$(if $(CONFIG_BOOTREG_DEFAULT),CONFIG_BOOTREG_DEFAULT=\"$(MINIBOOT)/$(CONFIG_BOOTREG_DEFAULT)\") \
		$(if $(CFG_HI_BOOT_REG_NAME),BOOT_REG0=\"$(MINIBOOT)/$(CONFIG_SYSREG)\") \
		$(if $(CFG_HI_BOOT_REG1_NAME),BOOT_REG1=\"$(MINIBOOT)/$(CONFIG_SYSREG1)\") \
		$(if $(CFG_HI_BOOT_REG2_NAME),BOOT_REG2=\"$(MINIBOOT)/$(CONFIG_SYSREG2)\") \
		$(if $(CFG_HI_BOOT_REG3_NAME),BOOT_REG3=\"$(MINIBOOT)/$(CONFIG_SYSREG3)\") \
		$(if $(CFG_HI_BOOT_REG4_NAME),BOOT_REG4=\"$(MINIBOOT)/$(CONFIG_SYSREG4)\") \
		$(if $(CFG_HI_BOOT_REG5_NAME),BOOT_REG5=\"$(MINIBOOT)/$(CONFIG_SYSREG5)\") \
		$(if $(CFG_HI_BOOT_REG6_NAME),BOOT_REG6=\"$(MINIBOOT)/$(CONFIG_SYSREG6)\") \
		$(if $(CFG_HI_BOOT_REG7_NAME),BOOT_REG7=\"$(MINIBOOT)/$(CONFIG_SYSREG7)\")

ifdef CONFIG_UNIFIED_BOOT
    CONFIG_EXTERN_FLAGS += CONFIG_BOOT_REG0=$(CFG_HI_BOOT_REG_NAME) \
			   CONFIG_BOOT_REG1=$(CFG_HI_BOOT_REG1_NAME) \
			   CONFIG_BOOT_REG2=$(CFG_HI_BOOT_REG2_NAME) \
			   CONFIG_BOOT_REG3=$(CFG_HI_BOOT_REG3_NAME) \
			   CONFIG_BOOT_REG4=$(CFG_HI_BOOT_REG4_NAME) \
			   CONFIG_BOOT_REG5=$(CFG_HI_BOOT_REG5_NAME) \
			   CONFIG_BOOT_REG6=$(CFG_HI_BOOT_REG6_NAME) \
			   CONFIG_BOOT_REG7=$(CFG_HI_BOOT_REG7_NAME)
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420 hi3798cv200 hi3798hv100 hi3798mv200 hi3798mv200_a),)
CONFIG_EXTERN_FLAGS     += $(if $(CFG_HI_NAND_SUPPORT),CONFIG_GENERIC_HIFMC100_NAND=$(CFG_HI_NAND_SUPPORT),)
CONFIG_EXTERN_FLAGS     += $(if $(CFG_HI_SPI_NAND_SUPPORT),CONFIG_GENERIC_HIFMC100_SPI_NAND=$(CFG_HI_SPI_NAND_SUPPORT),)
CONFIG_EXTERN_FLAGS     += $(if $(CFG_HI_SPI_SUPPORT),CONFIG_GENERIC_HIFMC100_SPI_SPI_NOR=$(CFG_HI_SPI_SUPPORT),)
endif

ifeq ($(CFG_HI_FORCE_ENV_TO_MEDIA), y)
CONFIG_EXTERN_FLAGS += $(if $(CFG_FORCE_ENV_IN_NAND),CONFIG_FORCE_ENV_IN_NAND=$(CFG_FORCE_ENV_IN_NAND),)
CONFIG_EXTERN_FLAGS += $(if $(CFG_FORCE_ENV_IN_SPI),CONFIG_FORCE_ENV_IN_SPI=$(CFG_FORCE_ENV_IN_SPI),)
CONFIG_EXTERN_FLAGS += $(if $(CFG_FORCE_ENV_IN_EMMC),CONFIG_FORCE_ENV_IN_EMMC=$(CFG_FORCE_ENV_IN_EMMC),)
endif

BOOTCONFIG_FILE     := s40
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
BOOTCONFIG_FILE     := hi3798mx
endif
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
BOOTCONFIG_FILE     := hi3716mv410
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100),)
BOOTCONFIG_FILE     := hi3798cv2x
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
ifeq ($(findstring skb,$(CFG_HI_BOOT_REG_NAME)), skb)
CONFIG_NET_HIGMACV300 := 1
else
ifeq ($(findstring ts,$(CFG_HI_BOOT_REG_NAME)), ts)
CONFIG_NET_HIGMACV300 := 1
else
CONFIG_USB_HOST_ETHER := 1
CONFIG_NET_HISFV300 := 1
endif
endif
BOOTCONFIG_FILE     := hi3798mv2x
endif
#+++++++++++++++++++++ADVCA compile options+++++++++++++++++++++++++++++++++++++++++++++
ifneq ($(BOOT_ADVCA_PROGRAMMER),y)
    ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
CONFIG_EXTERN_FLAGS +=  CONFIG_SUPPORT_CA=$(CFG_HI_ADVCA_TYPE)
CONFIG_EXTERN_FLAGS +=  CONFIG_SUPPORT_CA_$(CFG_HI_ADVCA_TYPE)=y
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE,$(CFG_HI_CFLAGS)),-DHI_ADVCA_FUNCTION_RELEASE)
CONFIG_EXTERN_FLAGS +=  CONFIG_SUPPORT_CA_RELEASE=y
BOOTCONFIG_FILE     := s40_ca_release
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
BOOTCONFIG_FILE     := hi3716mv410_ca_release
endif
    endif
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_DEBUG,$(CFG_HI_CFLAGS)),-DHI_ADVCA_FUNCTION_DEBUG)
CONFIG_EXTERN_FLAGS +=  CONFIG_SUPPORT_CA_DEBUG=y
    endif
    endif
endif
################################################################################

MKFLAGS    := CROSS_COMPILE=$(CROSS_COMPILE) \
              LIBSPRODUCT=$(LIBSPRODUCT) \
              $(CONFIG_EXTERN_FLAGS) \
              O=
ifeq (y,$(CFG_HI_BENCH_SUPPORT))
    MKFLAGS += CONFIG_MERGE_IMAGE=$(FASTBOOT)/merge.img
else
    MKFLAGS += CONFIG_MERGE_IMAGE=
endif

ifeq ($(BOOT_ADVCA_PROGRAMMER),y)
    MKFLAGS += CONFIG_BOOTROM_CA_SUPPORT=y
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
    MKFLAGS += MACH=hi3798cv2x
endif
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
    MKFLAGS += MACH=hi3798mv2x
endif
################################################################################
PHONYS += all product prepare miniboot $(AUXIMG)

all: \
   $(if $(OBJ_DIR), $(OBJ_DIR)/copy_code,) \
   $(if $(CONFIG_PRODUCT_WITH_BOOT),product,) \
   miniboot advca_programmer_install
ifneq ($(OBJ_DIR),)
	$(AT)cp -f $(MINIBOOT)/$(TARGET) $(OBJ_TARGET)
endif
ifeq ($(CFG_PRODUCT_TYPE),linux)
	$(AT)test -d $(HI_IMAGE_DIR) || mkdir -p $(HI_IMAGE_DIR)
	$(AT)cp -f $(MINIBOOT)/$(TARGET) $(HI_IMAGE_DIR)/miniboot.bin
else
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	$(AT)cp -f $(MINIBOOT)/$(TARGET) $(PREFIX)/miniboot.bin
endif
ifeq ($(CFG_HI_ADVCA_TYPE),NAGRA)
	$(AT)cp -f $(MINIBOOT)/miniboot.text $(PREFIX)/boot-ca.code
	$(AT)cp -f $(PRODUCT)/driver/advca/nagra/bep/bep.bin $(PREFIX)/bep.bin
endif
ifeq ($(CFG_HI_BOOTLOADER_SUPPORT),y)
	$(AT)cp -f $(MINIBOOT)/$(TARGET) $(SDK_DIR)/tools/windows/HiPro-serial/HiPro-serial_cn/program.bin
	$(AT)cp -f $(MINIBOOT)/$(TARGET) $(SDK_DIR)/tools/windows/HiPro-serial/HiPro-serial_en/program.bin
endif

$(OBJ_DIR)/copy_code:
	$(AT)test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	$(AT)cp -rf $(CURRDIR)/miniboot $(OBJ_DIR)
	$(AT)cp -rf $(CURRDIR)/product $(OBJ_DIR)
	$(AT)cp -rf $(CURRDIR)/fastboot $(OBJ_DIR)
	$(AT)cp -rf $(CURRDIR)/auxcode $(OBJ_DIR)
	touch $@

product: $(if $(OBJ_DIR), $(OBJ_DIR)/copy_code,)
	make -C $(MINIBOOT) $(MKFLAGS) $(BOOTCONFIG_FILE)_config
	make -C $(PRODUCT) $(MKFLAGS) O=$(O)

auxcode: $(AUXIMG)
ifdef CONFIG_UNIFIED_BOOT
ifeq (y,$(CONFIG_AUXCODE_COMPILE_SUPPORT))
	make -C $(AUXCODE) $(MKFLAGS) O=$(O) \
	&& cp -rf $(AUXCODE)/auxcode_sign.img $(AUXIMG)
endif
endif

miniboot: prepare $(if $(CONFIG_PRODUCT_WITH_BOOT),product,)
	make -C $(MINIBOOT) $(MKFLAGS) $(BOOTCONFIG_FILE) -j1

################################### programmer #################################
FASTBOOT       =  $(BUILD_DIR)/fastboot
CONFIG_FILE    = $(FASTBOOT)/include/configs/$(BOOTCONFIG)_config.h
CONFIG_EXPORT  = $(FASTBOOT)/include/configs/export.sh
PROGRAMMER_TARGET := mini-boot.bin


PROGRAM_MKFLAGS    := CROSS_COMPILE=$(CROSS_COMPILE) O=

ifdef CONFIG_UNIFIED_BOOT
    PROGRAM_MKFLAGS += CONFIG_BOOT_REG0=$(CFG_HI_BOOT_REG_NAME) \
			CONFIG_BOOT_REG1=$(CFG_HI_BOOT_REG1_NAME) \
			CONFIG_BOOT_REG2=$(CFG_HI_BOOT_REG2_NAME) \
			CONFIG_BOOT_REG3=$(CFG_HI_BOOT_REG3_NAME) \
			CONFIG_BOOT_REG4=$(CFG_HI_BOOT_REG4_NAME) \
			CONFIG_BOOT_REG5=$(CFG_HI_BOOT_REG5_NAME) \
			CONFIG_BOOT_REG6=$(CFG_HI_BOOT_REG6_NAME) \
			CONFIG_BOOT_REG7=$(CFG_HI_BOOT_REG7_NAME)
endif

advca_programmer_install: programmer_prepare
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) CONFIG_BOOTROM_CA_SUPPORT=y $(BOOTCONFIG) -j 16
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) mini-boot.bin
ifeq ($(CFG_PRODUCT_TYPE),linux)
	$(AT)test -d $(HI_IMAGE_DIR) || mkdir -p $(HI_IMAGE_DIR)
	$(AT)cp -f $(FASTBOOT)/$(PROGRAMMER_TARGET) $(HI_IMAGE_DIR)/advca_programmer.bin
else
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	$(AT)cp -f $(FASTBOOT)/$(PROGRAMMER_TARGET) $(PREFIX)/advca_programmer.bin
endif
	make -C $(FASTBOOT)  $(PROGRAM_MKFLAGS) distclean
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) mini-boot.clean
	$(AT)rm -f $(FASTBOOT)/*.reg

$(PREFIX)/non-advca_programmer.bin: programmer_prepare
	$(AT)test -d $(PREFIX)/ || mkdir -p $(PREFIX)/
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) $(BOOTCONFIG) -j 16
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) mini-boot.bin
	$(AT)cp -fv $(FASTBOOT)/$(PROGRAMMER_TARGET) $(PREFIX)/non-advca_programmer.bin
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) distclean
	make -C $(FASTBOOT) $(PROGRAM_MKFLAGS) mini-boot.clean
	$(AT)rm -f $(FASTBOOT)/*.reg

$(CONFIG_FILE): $(if $(OBJ_DIR), $(OBJ_DIR)/copy_code,) force
	$(AT)test ! -f $(CONFIG_FILE) || rm -rvf $(CONFIG_FILE)
	chmod 0755 $(CONFIG_EXPORT)
	@$(CONFIG_EXPORT) "CONFIG_NAND_SUPPORT=$(CFG_HI_NAND_SUPPORT)" \
		"nand flash support" $@
	@$(CONFIG_EXPORT) "CONFIG_SPI_NAND_SUPPORT=$(CFG_HI_SPI_NAND_SUPPORT)" \
		"spi-nand flash support" $@
	@$(CONFIG_EXPORT) "CONFIG_SPIFLASH_SUPPORT=$(CFG_HI_SPI_SUPPORT)" \
		"spi flash support" $@
	@$(CONFIG_EXPORT) "CONFIG_NET_SUPPORT=y" \
		"eth support" $@
	@$(CONFIG_EXPORT) "HISFV_PHY_U=$(CFG_HI_ETH_UP_PHYADDR)" \
		"eth upport phy address" $@
	@$(CONFIG_EXPORT) "HISFV_PHY_D=$(CFG_HI_ETH_DOWN_PHYADDR)" \
		"eth downport phy address" $@
	@$(CONFIG_EXPORT) "CONFIG_EMMC_SUPPORT=$(CFG_HI_EMMC_SUPPORT)" \
		"emmc flash support" $@
	@$(CONFIG_EXPORT) "CONFIG_PRODUCT_WITH_BOOT=" \
		"build product with boot" $@
	@$(CONFIG_EXPORT) "CONFIG_CHIP_TYPE=$(CFG_HI_CHIP_TYPE)" \
		"chip type" $@
	@$(CONFIG_EXPORT) "CONFIG_BOOT_REG_NAME=$(CFG_HI_BOOT_REG_NAME)" \
		"boot regfile name" $@
	@$(CONFIG_EXPORT) "CONFIG_BOOT_SIMULATE=$(CONFIG_BOOT_SIMULATE)" \
		"Simulate boot" $@
	@$(CONFIG_EXPORT) "CHIP_TYPE_$(CFG_HI_CHIP_TYPE)=1" \
		"Chip type" $@
	@$(CONFIG_EXPORT) "CONFIG_BOOTREG_DEFAULT=\"$(FASTBOOT)/$(CONFIG_BOOTREG_DEFAULT)\"" \
		"default boot reg" $@
	@$(CONFIG_EXPORT) "CONFIG_NET_HIGMACV300=$(CONFIG_NET_HIGMACV300)" \
		"build higmacv300" $@
	@$(CONFIG_EXPORT) "CONFIG_NET_HISFV300=$(CONFIG_NET_HISFV300)" \
		"build hisfv300" $@
	@$(CONFIG_EXPORT) "CONFIG_USB_HOST_ETHER=$(CONFIG_USB_HOST_ETHER)" \
		"build usb2eth" $@
	@$(CONFIG_EXPORT) "CONFIG_DISABLE_CONSOLE_INPUT=$(CFG_HI_USER_MODE)" \
		"Disable console input" $@
#+++++++++++++++++++++ unified boot +++++++++++++++++++++++++++++++++++++++++++++
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT),y)
	$(CONFIG_EXPORT) "CONFIG_UNIFIED_BOOT=$(CFG_HI_UNIFIED_BOOT_SUPPORT)" \
		"Unified boot" $@
ifneq ($(strip $(CONFIG_SYSREG)),)
	@( printf '#define BOOT_REG0	"$(FASTBOOT)/$(CONFIG_SYSREG)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG1)),)
	@( printf '#define BOOT_REG1	"$(FASTBOOT)/$(CONFIG_SYSREG1)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG2)),)
	@( printf '#define BOOT_REG2	"$(FASTBOOT)/$(CONFIG_SYSREG2)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG3)),)
	@( printf '#define BOOT_REG3	"$(FASTBOOT)/$(CONFIG_SYSREG3)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG4)),)
	@( printf '#define BOOT_REG4	"$(FASTBOOT)/$(CONFIG_SYSREG4)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG5)),)
	@( printf '#define BOOT_REG5	"$(FASTBOOT)/$(CONFIG_SYSREG5)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG6)),)
	@( printf '#define BOOT_REG6	"$(FASTBOOT)/$(CONFIG_SYSREG6)" \n') >> $@
endif
ifneq ($(strip $(CONFIG_SYSREG7)),)
	@( printf '#define BOOT_REG7	"$(FASTBOOT)/$(CONFIG_SYSREG7)" \n') >> $@
endif
endif

programmer_prepare: $(SYSREG) $(CONFIG_FILE)
	$(AT)rm -f $(FASTBOOT)/*.reg
	$(AT)test -d $(FASTBOOT)/ || mkdir -p $(FASTBOOT)
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT), y)
ifneq ($(strip $(CONFIG_SYSREG)),)
	$(AT)cp -fv $(SYSREG) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG1)),)
	$(AT)cp -fv $(SYSREG1) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG2)),)
	$(AT)cp -fv $(SYSREG2) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG3)),)
	$(AT)cp -fv $(SYSREG3) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG4)),)
	$(AT)cp -fv $(SYSREG4) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG5)),)
	$(AT)cp -fv $(SYSREG5) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG6)),)
	$(AT)cp -fv $(SYSREG6) $(FASTBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG7)),)
	$(AT)cp -fv $(SYSREG7) $(FASTBOOT)
endif
else
	$(AT)cp -fv $(SYSREG) $(FASTBOOT)
endif

###################################################################################
prepare: $(SYSREG)
	$(AT)rm -f $(MINIBOOT)/*.reg
	$(AT)test -d $(MINIBOOT)/ || mkdir -p $(MINIBOOT)
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT), y)
ifneq ($(strip $(CONFIG_SYSREG)),)
	$(AT)cp -fv $(SYSREG) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG1)),)
	$(AT)cp -fv $(SYSREG1) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG2)),)
	$(AT)cp -fv $(SYSREG2) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG3)),)
	$(AT)cp -fv $(SYSREG3) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG4)),)
	$(AT)cp -fv $(SYSREG4) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG5)),)
	$(AT)cp -fv $(SYSREG5) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG6)),)
	$(AT)cp -fv $(SYSREG6) $(MINIBOOT)
endif
ifneq ($(strip $(CONFIG_SYSREG7)),)
	$(AT)cp -fv $(SYSREG7) $(MINIBOOT)
endif
else
	$(AT)cp -fv $(SYSREG) $(MINIBOOT)
endif

force: ;

################################################################################
PHONYS += clean distclean

distclean: clean

clean :
	$(AT)rm -f $(PREFIX)/miniboot.bin
ifeq ($(OBJ_DIR),)
	make -C $(PRODUCT)  $(MKFLAGS) clean
	make -C $(MINIBOOT) distclean
ifeq ($(CFG_HI_UNIFIED_BOOT_SUPPORT), y)
ifeq (y, $(CONFIG_AUXCODE_COMPILE_SUPPORT))
	make -C $(AUXCODE) $(MKFLAGS) clean
endif
endif
	$(AT)rm -f $(MINIBOOT)/*.reg
	$(AT)rm -f $(FASTBOOT)/*.reg
else
	$(AT)rm -rf $(OBJ_DIR)
endif

################################################################################
.PHONY: $(PHONYS)
################################################################################
