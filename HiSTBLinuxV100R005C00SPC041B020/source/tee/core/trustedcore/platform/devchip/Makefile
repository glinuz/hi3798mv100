
#by fangjian00208632 2015-03-13

###########################################################
# platform definition
###########################################################
ROOT      =.


##########################################################
# build
##########################################################
.PHONY : all clean 

all:
ifneq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)),true)
	$(MAKE) -C $(ROOT)/$(PLATFORM_DIR_NAME)/ all
endif
	$(MAKE) -C $(HI_TEE_DRV_DIR) all
	$(MAKE) -C $(HI_TEE_TA_DIR) all
	
clean:
ifneq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)),true)
	$(MAKE) -C $(ROOT)/$(PLATFORM_DIR_NAME)/ clean
endif
	$(MAKE) -C $(HI_TEE_DRV_DIR) clean
	$(MAKE) -C $(HI_TEE_TA_DIR) clean
