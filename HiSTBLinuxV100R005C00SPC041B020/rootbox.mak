#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_ADVCA_TYPE),VERIMATRIX)
OUTPUT_FILE := > /dev/null 2>&1
endif

ifeq ($(CFG_HI_ADVCA_SANDBOX_SUPPORT),y)
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
SANDBOX_TYPE := chroot
else
SANDBOX_TYPE := $(shell echo $(CFG_HI_ADVCA_SANDBOX_TYPE) | tr "[A-Z]" "[a-z]")
endif
CA_ROOTBOX_BUILD := $(ROOTFS_SRC_DIR)/scripts/hica_build/$(SANDBOX_TYPE)
else
CA_ROOTBOX_BUILD := $(ROOTFS_SRC_DIR)/scripts/hica_build/none
endif

#===============================================================================
# rule
#===============================================================================
.PHONY: rootbox_compose

rootbox_compose:
	$(AT)make -C $(HI_SCRIPTS_DIR) all
	$(AT)rm -rf $(HI_ROOTBOX_DIR)
	$(AT)cp -arf $(HI_ROOTFS_DIR) $(HI_ROOTBOX_DIR)
	$(AT)cp -arf $(HI_MODULE_DIR) $(HI_ROOTBOX_DIR)
ifeq ($(CFG_HI_ROOTFS_INSTALL_SHARED_LIBS),y)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -arf $(HI_SHARED_LIB_DIR)/*.so* $(HI_ROOTBOX_DIR)/usr/lib
	$(AT)cp -arf $(HI_EXTERN_LIB_DIR)/* $(HI_ROOTBOX_DIR)/usr/lib/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_ROOTBOX_DIR)/usr/lib64 || mkdir -p $(HI_ROOTBOX_DIR)/usr/lib64
	$(AT)cp -arf $(HI_SHARED_LIB64_DIR)/*.so* $(HI_ROOTBOX_DIR)/usr/lib64/
	$(AT)cp -arf $(HI_EXTERN_LIB64_DIR)/* $(HI_ROOTBOX_DIR)/usr/lib64/
endif
endif
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
ifeq ($(HI_USER_SPACE_LIB),y)
	-$(AT)cp -arf $(HI_INSTALL_DIR)/obj/source/component/loader/app/release/*   $(HI_ROOTBOX_DIR)/home
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	-$(AT)cp -arf $(HI_INSTALL_DIR)/obj64/source/component/loader/app/release/*   $(HI_ROOTBOX_DIR)/home
endif
	$(AT)if [ -f $(HI_ROOTBOX_DIR)/etc/profile ]; then \
		echo "sleep 3" >> $(HI_ROOTBOX_DIR)/etc/profile; \
		echo "cd /home && ./loader $(OUTPUT_FILE)" >> $(HI_ROOTBOX_DIR)/etc/profile; \
		echo "reboot $(OUTPUT_FILE)" >> $(HI_ROOTBOX_DIR)/etc/profile; \
	fi
	-$(AT)mknod $(HI_ROOTBOX_DIR)/dev/console c 5 1
	-$(AT)mknod $(HI_ROOTBOX_DIR)/dev/ttyAMA0 c 204 64
	-$(AT)mknod $(HI_ROOTBOX_DIR)/dev/ttyAMA1 c 204 65
	-$(AT)mknod $(HI_ROOTBOX_DIR)/dev/ttyS000 c 204 64
else
ifeq ($(HI_USER_SPACE_LIB),y) 
	$(AT)cp -arf $(HI_SHARED_LIB_DIR)/*freetype.so* $(HI_ROOTBOX_DIR)/usr/lib
	$(AT)cp -arf $(HI_SHARED_LIB_DIR)/libz.so* $(HI_ROOTBOX_DIR)/usr/lib
	$(AT)cp -arf $(HI_EXTERN_LIB_DIR)/*AUDIO* $(HI_ROOTBOX_DIR)/usr/lib/
	$(AT)cp -arf $(HI_EXTERN_LIB_DIR)/higo-adp $(HI_ROOTBOX_DIR)/usr/lib/
endif
endif

ifdef CFG_HI_ADVCA_FUNCTION
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)test -d $(CA_ROOTBOX_BUILD)/loader_build || tar xzf $(CA_ROOTBOX_BUILD)/loader_build.tar.gz -C $(CA_ROOTBOX_BUILD)/
	$(AT)$(CA_ROOTBOX_BUILD)/loader_build/MakeCALoaderRootBox.sh $(HI_ROOTBOX_DIR) $(CFG_HI_ADVCA_FUNCTION) $(CFG_HI_ARM_TOOLCHAINS_NAME)
else
	$(AT)test -d $(CA_ROOTBOX_BUILD)/system_build || tar xzf $(CA_ROOTBOX_BUILD)/system_build.tar.gz -C $(CA_ROOTBOX_BUILD)/
	$(AT)$(CA_ROOTBOX_BUILD)/system_build/MakeCASystemRootBox.sh $(HI_ROOTBOX_DIR) $(CFG_HI_ADVCA_FUNCTION) $(CFG_HI_ARM_TOOLCHAINS_NAME)
endif ####ifeq ($(CFG_HI_LOADER_APPLOADER),y)
endif
ifeq ($(CFG_HI_ROOTFS_STRIP), y)
	-$(AT)find $(HI_ROOTBOX_DIR)/bin/ $(HI_ROOTBOX_DIR)/lib/ $(HI_ROOTBOX_DIR)/sbin/ $(HI_ROOTBOX_DIR)/usr/ | xargs $(STRIP) 2> /dev/null
	-$(AT)find $(HI_ROOTBOX_DIR)/bin/ $(HI_ROOTBOX_DIR)/lib/ $(HI_ROOTBOX_DIR)/sbin/ $(HI_ROOTBOX_DIR)/usr/ | xargs $(STRIP) -R .note -R .comment 2> /dev/null
endif
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)rm -rf $(HI_ROOTBOX_DIR)/opt
	$(AT)rm -rf $(HI_ROOTBOX_DIR)/share
endif
