LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_SUFFIX :=
LOCAL_FORCE_STATIC_EXECUTABLE := true


include $(BUILD_SYSTEM)/binary.mk

$(LOCAL_BUILT_MODULE) : PRIVATE_ELF_FILE := $(intermediates)/$(PRIVATE_MODULE).elf
$(LOCAL_BUILT_MODULE) : PRIVATE_LIBS := `$(TRUSTEDCORE_CC) -mthumb-interwork -print-libgcc-file-name`
$(LOCAL_BUILT_MODULE) : PRIVATE_IMG := $(intermediates)/$(PRIVATE_MODULE)

$(all_objects) : PRIVATE_TARGET_PROJECT_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_C_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CFLAGS :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CPPFLAGS :=

$(LOCAL_BUILT_MODULE): $(all_objects) $(all_libraries)
	@$(mkdir -p $(dir $@)
	$(hide) touch $(PRIVATE_IMG)
	@echo "target Linking: $(PRIVATE_MODULE)"
