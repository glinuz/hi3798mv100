ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

all:
	$(AT)./CreateSignature.sh $(CFG_HI_ADVCA_TYPE) $(CFG_HI_CHIP_TYPE) $(SDK_DIR) $(HI_IMAGE_DIR) $(CFG_HI_BOOT_REG_NAME)
clean:
	-rm -f ./SignInput/*.bin
	-rm -f ./SignInput/*.cfg
	-rm -f ./SignInput/*.cfg
	-rm -f ./SignInput/rootfs*
	-rm -rf ./SignOutput
	-rm -rf ./CASignTool
	-rm -rf $(HI_IMAGE_DIR)/*.sig
	-rm -rf $(HI_IMAGE_DIR)/FinalBoot.bin
	-rm -rf $(HI_IMAGE_DIR)/*signature*
