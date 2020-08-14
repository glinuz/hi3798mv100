LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_SUFFIX :=
LOCAL_FORCE_STATIC_EXECUTABLE := true

#modify by l00202565
# 0. default use android toolchain
# android 4.0/4.4 ARCH_PREFIX is arm; in android 5.0 & later ARCH_PREFIX can be arm/aarch64
$(info PLATFORM_VERSION: $(PLATFORM_VERSION))
$(info TRUSTEDCORE_ARCH_CHOOSE: $(TRUSTEDCORE_ARCH_CHOOSE))
ifeq ($(strip $(TRUSTEDCORE_ARCH_CHOOSE)), AARCH64)
# arm arch64, used when trustedcore support 64bit
TRUSTEDCORE_CC := $(TARGET_CC)
TRUSTEDCORE_LD := $(TARGET_LD)
TRUSTEDCORE_OBJCOPY := $(TARGET_OBJCOPY)
else
# arm arch32
ifneq ($(filter 4.% 4.%.%, $(PLATFORM_VERSION)),)
#android 4.0/4.4...
TRUSTEDCORE_CC := $(TARGET_CC)
TRUSTEDCORE_LD := $(TARGET_LD)
TRUSTEDCORE_OBJCOPY := $(TARGET_OBJCOPY)
else
# android5.0 and later version, 2nd arch for arm arch32
TRUSTEDCORE_CC := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_CC)
TRUSTEDCORE_LD := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_LD)
TRUSTEDCORE_OBJCOPY := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_OBJCOPY)
endif
endif

# 1. set arm-linux-androideabi-4.8 toolchain
#TRUSTEDCORE_CC := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-gcc
#TRUSTEDCORE_LD := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-ld
#TRUSTEDCORE_OBJCOPY := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-objcopy

# 2. arm-eabi toolchain
#TRUSTEDCORE_CC := arm-eabi-gcc
#TRUSTEDCORE_LD := arm-eabi-ld
#TRUSTEDCORE_OBJCOPY := arm-eabi-objcopy

# 3. set arm-linux-androideabi-4.6 toolchain
#TRUSTEDCORE_CC := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc
#TRUSTEDCORE_LD := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-ld
#TRUSTEDCORE_OBJCOPY := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-objcopy


#toolchain info
$(info TRUSTEDCORE_CC: $(TRUSTEDCORE_CC))
$(info TRUSTEDCORE_LD : $(TRUSTEDCORE_LD))
$(info TRUSTEDCORE_OBJCOPY : $(TRUSTEDCORE_OBJCOPY))

# set local compiler
LOCAL_CC := $(TRUSTEDCORE_CC)

# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables -fno-builtin-fwrite
#LOCAL_CFLAGS += -fstack-protector --param ssp-buffer-size=4 -Wstack-protector
LOCAL_CFLAGS += -fstack-protector-all -Wstack-protector
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables

#LOCAL_CFLAGS += -mno-apcs-float -fno-common -pipe -mthumb-interwork -mlong-calls

include $(BUILD_SYSTEM)/binary.mk

$(LOCAL_BUILT_MODULE) : PRIVATE_ELF_FILE := $(intermediates)/$(PRIVATE_MODULE).elf
$(LOCAL_BUILT_MODULE) : PRIVATE_LIBS := `$(TRUSTEDCORE_CC) -mthumb-interwork -print-libgcc-file-name`
$(all_objects) : PRIVATE_TARGET_PROJECT_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_C_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CFLAGS :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CPPFLAGS :=
$(all_objects) : PRIVATE_CFLAGS_NO_OVERRIDE := -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast

$(LOCAL_BUILT_MODULE): $(all_objects) $(all_libraries)
	@$(mkdir -p $(dir $@)
	@echo "target Linking: $(PRIVATE_MODULE)"
	$(hide) $(TRUSTEDCORE_LD) \
		$(addprefix --script ,$(PRIVATE_LINK_SCRIPT)) \
		$(PRIVATE_RAW_EXECUTABLE_LDFLAGS) \
		--whole-archive $(PRIVATE_ALL_WHOLE_STATIC_LIBRARIES)  --no-whole-archive \
		-o $(PRIVATE_ELF_FILE) \
		$(PRIVATE_ALL_OBJECTS) \
		--start-group $(PRIVATE_ALL_STATIC_LIBRARIES) --end-group \
		$(PRIVATE_LIBS)
	$(hide) $(TRUSTEDCORE_OBJCOPY) -O binary $(PRIVATE_ELF_FILE) $@
