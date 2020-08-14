#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all: $(ALL_OUT_LIBS)

clean:
	rm -rf $(ALL_OUT_OBJ)
	rm -rf $(ALL_OUT_LIBS)
	
install: all

uninstall:

ifeq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)), true)
$(ALL_OUT_LIBS): 
	test  -d $@ | mkdir -p $(dir $@)
	#	cp $(PLATFORM_DIR_NAME)/$(ARMV8_POSIX_STR)/$(notdir $(ALL_OUT_LIBS)) $@
	cp $(TRUSTEDCORE_RELEASE_DIR)/$(HI_CHIP_TYPE)/$(CFG_HI_TVP_MEM_LAYOUT)/$(notdir $(ALL_OUT_LIBS)) $@
else
$(ALL_OUT_LIBS): $(ALL_OUT_OBJ)
	rm -rf $@
	$(AR2) crsP $@ $^	
endif

$(LOCAL_OUT)/%.o : %.c
	test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC2) $(CFLAGS) -o $@ -c $^	
