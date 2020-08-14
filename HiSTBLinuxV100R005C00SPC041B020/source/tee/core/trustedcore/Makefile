#by fangjian00208632 2015-03-13

#====================================================================================
#                   export
#====================================================================================
.EXPORT_ALL_VARIABLES:
OS_PLATFORM_TYPE := LINUX

include trustedcore.mk

###########################################################
# platform definition
###########################################################
TARGET_IMG = $(OUT)/trustedcore.img
TARGET_IMG_INSTALL_DIR = $(HI_IMAGE_DIR)

##########################################################
# build all
##########################################################
.PHONY : all build packimg clean
.PHONY : drivers       platform       rtosck       globaltask       internal_tasks
.PHONY : drivers_clean platform_clean rtosck_clean globaltask_clean internal_tasks_clean

all: packimg
packimg: globaltask internal_tasks rtosck


ifneq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)),true)
rtosck:  drivers
rtosck_clean: drivers_clean
endif

rtosck: platform
rtosck_clean: platform_clean

clean: rtosck_clean globaltask_clean internal_tasks_clean
	$(RM) $(TARGET_IMG)
	$(RM) -rf $(OUT)
	$(RM) -rf $(TOOLCHAINS_DIR)/gcc
	$(RM) -rf $(TRUSTEDCORE_DIR)/tools/elf_extract

install: all
	@echo "Install $(TARGET_IMG_INSTALL_DIR)"
	$(COPY) $(TARGET_IMG) $(TARGET_IMG_INSTALL_DIR)

uninstall: clean
	@echo "Uninstall $(TARGET_IMG_INSTALL_DIR)/$(notdir $(TARGET_IMG))"
	$(RM) $(TARGET_IMG_INSTALL_DIR)/$(notdir $(TARGET_IMG))

$(TOOLCHAINS_DIR)/%/tar:
	@test -d $(@D) || mkdir -p $(@D)
	tar -xjf $(TOOLCHAINS_PACKAGE) -C $(TOOLCHAINS_DIR)
	@touch $@

##########################################################
# 1. build rtosck
##########################################################
rtosck:$(TOOLCHAINS_DIR)/gcc/tar
	$(MAKE) -C trustedcore_kernel/ all
rtosck_clean:
	$(MAKE) -C trustedcore_kernel/ clean

##########################################################
# 2. build globaltask
##########################################################
globaltask:$(TOOLCHAINS_DIR)/gcc/tar
	$(MAKE) -C TEE_ext/ all
globaltask_clean:
	$(MAKE) -C TEE_ext/ clean

##########################################################
# 3. build internal_tasks
##########################################################
internal_tasks:$(TOOLCHAINS_DIR)/gcc/tar
	$(MAKE) -C internal_tasks/ all
internal_tasks_clean:
	$(MAKE) -C internal_tasks/ clean

##########################################################
# 2. build drivers
##########################################################
drivers:$(TOOLCHAINS_DIR)/gcc/tar
	$(MAKE) -C drivers/ all
drivers_clean:
	$(MAKE) -C drivers/ clean

##########################################################
# 3. build platform
##########################################################
platform:$(TOOLCHAINS_DIR)/gcc/tar
	$(MAKE) -C platform/ all
platform_clean:
	$(MAKE) -C platform/ clean

##########################################################
# 6. packimg
##########################################################
PACKIMG_SHELL =${TRUSTEDCORE_DIR}/platform/${PLATFORM_DIR}/${PLATFORM_DIR_NAME}/packimg_linux.sh

packimg:
	chmod 777 ${PACKIMG_SHELL} 
	@${PACKIMG_SHELL} $(PACKING_CFG)
	@echo "Make $(TARGET_IMG) Success!"

