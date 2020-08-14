# add by l00202565

## this set trustedcore_src build toolchain
#0. default use android toolchain
# android 4.0/4.4 ARCH_PREFIX is arm; in android 5.0 ARCH_PREFIX can be arm/aarch64
$(info PLATFORM_VERSION: $(PLATFORM_VERSION))
$(info TRUSTEDCORE_ARCH_CHOOSE: $(TRUSTEDCORE_ARCH_CHOOSE))
ifeq ($(strip $(TRUSTEDCORE_ARCH_CHOOSE)), AARCH64)
#used when trustedcore support 64bit
LOCAL_CC := $(TARGET_CC)
TRUSTECORE_TARGET_AR := $(TARGET_AR)
else
ifneq ($(filter 4.% 4.%.%, $(PLATFORM_VERSION)),)
LOCAL_CC := $(TARGET_CC)
TRUSTECORE_TARGET_AR := $(TARGET_AR)
else
LOCAL_CC := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_CC)
TRUSTECORE_TARGET_AR := $($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_AR)
endif
endif

#1. set arm-linux-androideabi-4.8 toolchain
#LOCAL_CC := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-gcc
#TRUSTECORE_TARGET_AR := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-ar

#2. arm-eabi toolchain
#LOCAL_CC := arm-eabi-gcc
#TRUSTECORE_TARGET_AR := arm-eabi-ar

#3. set arm-linux-androideabi-4.6 toolchain
#LOCAL_CC := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc
#TRUSTECORE_TARGET_AR := prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-ar


#toolchain info
$(info LOCAL_CC: $(LOCAL_CC))
$(info TRUSTECORE_TARGET_AR: $(TRUSTECORE_TARGET_AR))

#add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables -fno-builtin-fwrite
#LOCAL_CFLAGS += -fstack-protector --param ssp-buffer-size=4 -Wstack-protector
LOCAL_CFLAGS += -fstack-protector-all -Wstack-protector
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables

#LOCAL_CFLAGS += -mno-apcs-float -fno-common -pipe -mthumb-interwork -mlong-calls

define trustedcore-transform-o-to-static-lib
@mkdir -p $(dir $@)
@rm -f $@
$(extract-and-include-target-whole-static-libs)
@echo "target StaticLib: $(PRIVATE_MODULE) ($@)"
$(call split-long-arguments,$(TRUSTECORE_TARGET_AR) \
    $(TARGET_GLOBAL_ARFLAGS) \
    $(PRIVATE_ARFLAGS) $@,$(filter %.o, $^))
endef

ifeq ($(strip $(LOCAL_MODULE_CLASS)),)
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
endif
ifeq ($(strip $(LOCAL_MODULE_SUFFIX)),)
LOCAL_MODULE_SUFFIX := .a
endif
LOCAL_UNINSTALLABLE_MODULE := true
ifneq ($(strip $(LOCAL_MODULE_STEM)$(LOCAL_BUILT_MODULE_STEM)),)
$(error $(LOCAL_PATH): Cannot set module stem for a library)
endif

include $(BUILD_SYSTEM)/binary.mk

LOCAL_RAW_STATIC_LIBRARY:=
$(all_objects) : PRIVATE_TARGET_PROJECT_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_C_INCLUDES :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CFLAGS :=
$(all_objects) : PRIVATE_TARGET_GLOBAL_CPPFLAGS :=
$(all_objects) : PRIVATE_CFLAGS_NO_OVERRIDE := -Werror=int-to-pointer-cast -Werror=pointer-to-int-cast

$(LOCAL_BUILT_MODULE) : $(built_whole_libraries)
$(LOCAL_BUILT_MODULE) : $(all_objects)
	$(trustedcore-transform-o-to-static-lib)
