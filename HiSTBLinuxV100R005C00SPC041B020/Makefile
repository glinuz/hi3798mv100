#===============================================================================
#                   export
#===============================================================================
.EXPORT_ALL_VARIABLES:

SDK_DIR := $(CURDIR)

include base.mak

CFG_HI_EXPORT_FLAG=y

#===============================================================================
#                   build
#===============================================================================
.PHONY: build clean

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
PROGRAMMER := advca_programmer
endif

build: prebuilts hiboot $(PROGRAMMER) rootbox
ifeq ($(CFG_HI_FS_BUILDIN),y)
	$(AT)make -C $(KERNEL_DIR) fs_buildin
else
	$(AT)make $(IMAGES)
ifeq ($(CFG_HI_ADVCA_TYPE)_$(CFG_HI_LOADER_APPLOADER), CONAX_y)
	$(AT)test -e $(HI_IMAGE_DIR)/hi_kernel.bin && test -e $(HI_IMAGE_DIR)/rootfs.squashfs && $(MKBOOTIMG) --kernel $(HI_IMAGE_DIR)/hi_kernel.bin --ramdisk $(HI_IMAGE_DIR)/rootfs.squashfs --base $(CFG_HI_APPLOADER_INITRD_BASE_ADDR) -o $(HI_IMAGE_DIR)/apploader.bin
endif
endif
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(SAMPLE_DIR) all
endif
	$(AT)make signature

clean:
	$(AT)rm -rf $(HI_OUT_DIR)
	$(AT)test ! -d $(KERNEL_SRC_DIR).patch || rm -rf $(KERNEL_SRC_DIR)

#====================================================================================
#                   global targets
#====================================================================================
.PHONY: signature signature_clean
#++++++++++++++++++++++++++++++++++++++++++++++++++++
signature:
ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
	make -C $(SDK_DIR)/tools/linux/utils/advca/sig_image all
endif

signature_clean:
ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
	make -C $(SDK_DIR)/tools/linux/utils/advca/sig_image clean
endif

#====================================================================================
#                   tools
#====================================================================================
.PHONY: tools  tools_clean
#++++++++++++++++++++++++++++++++++++++++++++++++++++
tools:
	$(AT)make -C $(HI_TOOLS_DIR) all

tools_clean:
	$(AT)make -C $(HI_TOOLS_DIR) clean

#====================================================================================
#                   prebuilts
#====================================================================================
.PHONY:  prebuilts prebuilts_clean
#++++++++++++++++++++++++++++++++++++++++++++++++++++
#=============BOOTARGS_VAR======================================================================
BOOTARGS_INPUT  := bootargs.txt
BOOTARGS_OUTPUT := bootargs.bin

ifeq ($(CFG_HI_APPLOADER_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
BOOTARGS_INPUT := bootargs_apploader.txt
endif
endif

ifeq ($(CFG_HI_DONGLE_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
    BOOTARGS_INPUT := bootargs_dongle.txt
endif
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
        BOOTARGS_INPUT := bootargs_tee_DDR_$(CFG_HI_TVP_MEM_LAYOUT)B.txt
else
    ifeq ($(CFG_HI_ADVCA_TYPE),OTHER)
        BOOTARGS_INPUT := bootargs.txt
    else ifeq ($(CFG_HI_ADVCA_TYPE),VERIMATRIX)
        BOOTARGS_INPUT := bootargs_ca_vmx.txt
    else ifneq ($(CFG_HI_ADVCA_TYPE),)
        BOOTARGS_INPUT := bootargs_ca_$(shell echo $(CFG_HI_ADVCA_FUNCTION) | tr '[A-Z]' '[a-z]').txt
    endif
endif

prebuilts:
	$(AT)test -d $(HI_IMAGE_DIR) || mkdir -p $(HI_IMAGE_DIR)
	$(AT)(cd $(HI_IMAGE_DIR) && $(MKBOOTARGS) -s 64 -r $(HI_PREBUILTS_DIR)/$(BOOTARGS_INPUT) -o $(BOOTARGS_OUTPUT) > /dev/null)
	$(AT)cp -f $(HI_PREBUILTS_DIR)/logo.img $(HI_PREBUILTS_DIR)/baseparam.img $(HI_PREBUILTS_DIR)/pq_param.bin $(HI_IMAGE_DIR)/
	$(AT)cp -f $(HI_PREBUILTS_DIR)/*.xml $(HI_IMAGE_DIR)/

prebuilts_clean:
	$(AT)rm -rf $(MKBOOTARGS) $(HI_IMAGE_DIR)/$(BOOTARGS_OUTPUT)

#====================================================================================
#                   hiboot
#====================================================================================
.PHONY:  hiboot  hiboot_clean  advca_programmer
#++++++++++++++++++++++++++++++++++++++++++++++++++++
hiboot:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(BOOT_DIR) all $(if $(HI_OUT_DIR),O=$(HI_OUT_DIR)/obj/source/boot,)
endif

hiboot_clean:
	$(AT)make -C $(BOOT_DIR) clean $(if $(HI_OUT_DIR),O=$(HI_OUT_DIR)/obj/source/boot,)

advca_programmer:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(BOOT_DIR) advca_programmer_install BOOT_ADVCA_PROGRAMMER=y $(if $(HI_OUT_DIR),O=$(HI_OUT_DIR)/obj/source/programmer,)
endif

#====================================================================================
#                   linux
#====================================================================================
.PHONY:  linux linux_clean

linux:
	$(AT)make -C $(KERNEL_DIR) all

linux_clean:
	$(AT)make -C $(KERNEL_DIR) clean

#====================================================================================
#                   rootfs
#====================================================================================
.PHONY:  rootfs  rootfs_clean

rootfs:
	$(AT)make -C $(ROOTFS_SRC_DIR) all

rootfs_clean:
	$(AT)make -C $(ROOTFS_SRC_DIR) clean

#====================================================================================
#                   common
#====================================================================================
.PHONY:  common common_clean

common:
	$(AT)make -C $(COMMON_DIR)/api all

common_clean:
	$(AT)make -C $(COMMON_DIR)/api clean

#====================================================================================
#                   msp
#====================================================================================
.PHONY:  msp  msp_clean

msp:
	$(AT)make -C $(MSP_DIR)/api all

msp_clean:
	$(AT)make -C $(MSP_DIR)/api clean

#====================================================================================
#                   hal
#====================================================================================
.PHONY:  hal  hal_clean
#++++++++++++++++++++++++++++++++++++++++++++++++++++
hal:
ifeq ($(CFG_GSTREAMER_FULL_SUPPORT),y)
	$(AT)make -C $(SDK_DIR)/hal all
endif

hal_clean:
	$(AT)make -C $(SDK_DIR)/hal clean
#====================================================================================
#                   higo
#====================================================================================
.PHONY:  higo  higo_clean

higo:
	$(AT)make -C $(MSP_DIR)/api/higo all

higo_clean:
	$(AT)make -C $(MSP_DIR)/api/higo clean

#====================================================================================
#                   jpeg
#====================================================================================
.PHONY:  jpeg  jpeg_clean

jpeg:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(MSP_DIR)/api/jpeg all
endif

jpeg_clean:
	$(AT)make -C $(MSP_DIR)/api/jpeg clean

#====================================================================================
#                   png
#====================================================================================
.PHONY:  png  png_clean

png:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(MSP_DIR)/api/png all
endif

png_clean:
	$(AT)make -C $(MSP_DIR)/api/png clean

#====================================================================================
#                   omx
#====================================================================================
.PHONY:  omx  omx_clean

omx:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(MSP_DIR)/api/omx all
endif

omx_clean:
	$(AT)make -C $(MSP_DIR)/api/omx clean

#====================================================================================
#                   gpu
#====================================================================================
.PHONY:  gpu  gpu_clean

gpu:
ifneq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C $(MSP_DIR)/api/gpu all
endif

gpu_clean:
	$(AT)make -C $(MSP_DIR)/api/gpu clean

#====================================================================================
#                   component
#====================================================================================
.PHONY:  component  component_clean

component:
	$(AT)make -C $(COMPONENT_DIR) all

component_clean:
	$(AT)make -C $(COMPONENT_DIR) clean

#====================================================================================
#                   tee
#====================================================================================
.PHONY: tee tee_clean

tee:
ifeq ($(CFG_HI_TEE_SUPPORT),y)
	$(AT)[ ! -d $(HI_TEE_OS_DIR) ] || make -C $(HI_TEE_OS_DIR) all
endif

tee_clean:
	$(AT)[ ! -d $(HI_TEE_OS_DIR) ] || make -C $(HI_TEE_OS_DIR) clean

#====================================================================================
#                   sample
#====================================================================================
.PHONY:  sample  sample_clean

sample:
	$(AT)make -C $(SAMPLE_DIR) all

sample_clean:
	$(AT)make -C $(SAMPLE_DIR) clean

#====================================================================================
#                   rootbox
#====================================================================================
include rootbox.mak

.PHONY:  rootbox  rootbox_clean

rootbox: linux tee tools rootfs common msp higo jpeg png gpu omx hal component
ifeq ($(CFG_HI_DIRECTFB_SUPPORT),y)
	$(AT)make -C $(COMPONENT_DIR)/directfb all
endif
	$(AT)make rootbox_compose
	$(AT)echo "rootbox is ready"

rootbox_clean:
	$(AT)rm -rf $(HI_ROOTBOX_DIR)

#====================================================================================
#                   fs
#====================================================================================
.PHONY: fs cramfs squashfs jffs2 yaffs extfs ubiimg
#++++++++++++++++++++++++++++++++++++++++++++++++++++
IMAGES :=
ifeq ($(CFG_HI_ROOTFS_CRAMFS),y)
IMAGES += cramfs
endif
ifeq ($(CFG_HI_ROOTFS_SQUASHFS),y)
IMAGES += squashfs
endif
ifeq ($(CFG_HI_ROOTFS_JFFS2),y)
IMAGES += jffs2
endif
ifeq ($(CFG_HI_ROOTFS_YAFFS),y)
IMAGES += yaffs
endif
ifeq ($(CFG_HI_ROOTFS_EXT4),y)
IMAGES += extfs
endif
ifeq ($(CFG_HI_ROOTFS_UBIIMG),y)
IMAGES += ubiimg
endif

fs: $(IMAGES)

ifdef CFG_HI_SPI_BLOCK_SIZE
SPI_BLOCK_SIZE := $(CFG_HI_SPI_BLOCK_SIZE)
else
SPI_BLOCK_SIZE := 0x10000 0x20000 0x40000
endif

YAFFS_MODE := 2k-1bit 4k-1bit 4k-4byte 4k-24bit1k 8k-24bit1k 8k-40bit1k
ifdef CFG_HI_NAND_PAGE_SIZE
ifdef CFG_HI_NAND_ECC_TYPE
YAFFS_MODE :=$(CFG_HI_NAND_PAGE_SIZE)-$(CFG_HI_NAND_ECC_TYPE)
endif
endif

#++++++++++++++++++++++++++++++++++++++++++++++++++++
cramfs:
	$(MKCRAMFS) $(HI_ROOTBOX_DIR) $(HI_IMAGE_DIR)/rootfs.cramfs
#++++++++++++++++++++++++++++++++++++++++++++++++++++
squashfs:
	$(MKSQUASHFS) $(HI_ROOTBOX_DIR) $(HI_IMAGE_DIR)/rootfs.squashfs \
		-no-fragments -noappend -noI -comp xz -T $(RULEFILE) -G $(RULEDBG)
#++++++++++++++++++++++++++++++++++++++++++++++++++++
jffs2:
	$(AT)(for ix in $(SPI_BLOCK_SIZE) ; do ( \
		if [ "$$ix" == "0x10000" ]; then \
			NAME=64k; \
		elif [ "$$ix" == "0x20000" ];then \
			NAME=128k; \
		elif [ "$$ix" == "0x40000" ];then \
			NAME=256k; \
		fi; \
		$(MKJFFS2) -d $(HI_ROOTBOX_DIR) -l -e $${ix} \
		-o $(HI_IMAGE_DIR)/rootfs_$${NAME}.jffs2; \
	) done )
#++++++++++++++++++++++++++++++++++++++++++++++++++++
yaffs:
	$(AT)(for ix in $(YAFFS_MODE) ; do ( \
		NAME=`echo $$ix | sed -s 's/b.*[ket]/b/' | sed -s 's/-//'`; \
		PARAM=`echo $$ix | sed -s 's/-/ /'` ; \
		$(MKYAFFS) $(HI_ROOTBOX_DIR) \
			$(HI_IMAGE_DIR)/rootfs_$$NAME.yaffs $$PARAM -T $(RULEFILE) -G $(RULEDBG); \
		chmod a+r $(HI_IMAGE_DIR)/rootfs_$$NAME.yaffs; \
	) done )

ubiimg:
	$(MKUBIIMG) $(CFG_HI_NAND_PAGE_SIZE) $(CFG_HI_NAND_BLOCK_SIZE) $(HI_ROOTBOX_DIR) $(CFG_HI_UBI_PARTITION_SIZE) $(HI_IMAGE_DIR)/rootfs_$(CFG_HI_UBI_PARTITION_SIZE).ubiimg $(RULEFILE) $(RULEDBG)

EMMC_SIZE_STR := $(CFG_HI_EMMC_ROOTFS_SIZE)M
ROOTBOX_SIZE := $(word 1,$(shell du -sh $(HI_ROOTBOX_DIR) 2>/dev/null))

extfs:
	$(AT)echo -e "Attention:\n\trootbox size must be less than $(EMMC_SIZE_STR), rootbox size is $(ROOTBOX_SIZE).\n"
	$(MKEXT4FS) -l $(EMMC_SIZE_STR) -s $(HI_IMAGE_DIR)/rootfs_$(EMMC_SIZE_STR).ext4 $(HI_ROOTBOX_DIR)

#====================================================================================
#                   cfg ops
#====================================================================================
.PHONY: menuconfig

#++++++++++++++++++++++++++++++++++++++++++++++++++++
KCONFIG_SRC_DIR := $(SDK_DIR)/tools/linux/kconfig
KCONFIG_OUT_DIR := $(if $(HI_OUT_DIR),$(HI_OUT_DIR)/tools/linux/kconfig/,$(KCONFIG_SRC_DIR)/)
KCONFIG_EXE := $(KCONFIG_OUT_DIR)himconf
KCONFIG_CFG := scripts/kconfig/mainKconfig
#++++++++++++++++++++++++++++++++++++++++++++++++++++
$(KCONFIG_EXE):
	$(AT)mkdir -p $(KCONFIG_OUT_DIR)
	$(AT)make -C $(KCONFIG_SRC_DIR) O=$(KCONFIG_OUT_DIR)

#++++++++++++++++++++++++++++++++++++++++++++++++++++
menuconfig: $(KCONFIG_EXE)
	$(KCONFIG_EXE) $(KCONFIG_CFG)
	$(AT)echo "make $@ over"

#====================================================================================
#     build apploader.bin quickly just for the second time
#====================================================================================
loader_rebuild:
	$(AT)make -C $(COMPONENT_DIR) loader_clean
	$(AT)make -C $(COMPONENT_DIR) loader
ifeq ($(CFG_HI_ADVCA_TYPE), CONAX)
	$(AT)make rootbox_compose
	$(AT)make $(IMAGES)
	$(AT)test -e $(HI_IMAGE_DIR)/hi_kernel.bin && test -e $(HI_IMAGE_DIR)/rootfs.squashfs && $(MKBOOTIMG) --kernel $(HI_IMAGE_DIR)/hi_kernel.bin --ramdisk $(HI_IMAGE_DIR)/rootfs.squashfs --base $(CFG_HI_APPLOADER_INITRD_BASE_ADDR) -o $(HI_IMAGE_DIR)/apploader.bin
else
	$(AT)make -C $(KERNEL_DIR) fs_buildin
endif
#====================================================================================

#====================================================================================
#                   help
#====================================================================================
.PHONY:  help
#++++++++++++++++++++++++++++++++++++++++++++++++++++
GREEN="\e[32;1m"
NORMAL="\e[39m"
RED="\e[31m"

help:
	$(AT)echo -e $(GREEN)
	$(AT)cat $(HI_SCRIPTS_DIR)/MakeHelp | less
	$(AT)echo -e $(NORMAL)
