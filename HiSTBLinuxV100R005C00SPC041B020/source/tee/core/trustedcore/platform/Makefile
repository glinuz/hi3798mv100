
#by fangjian00208632 2015-03-13

###########################################################
# platform definition
###########################################################
ROOT      =.


##########################################################
# build
##########################################################
.PHONY : all clean

TZARCH_DIR = $(ROOT)/tzarch

all: $(ALL_TARGET)
	$(MAKE) -C $(ROOT)/$(PLATFORM_DIR)/ all
#	$(MAKE) -C $(ROOT)/$(TZARCH_DIR)/ all
	
clean: $(ALL_CLEAN)
	$(MAKE) -C $(ROOT)/$(PLATFORM_DIR)/ clean
#	$(MAKE) -C $(ROOT)/$(TZARCH_DIR) clean
	
