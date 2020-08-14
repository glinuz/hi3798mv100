#-------------------------------------------------------------------------------
# Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
#-------------------------------------------------------------------------------
CFLAGS += $(CFG_HI_CFLAGS) $(AUDIO_ADVCA_CFLAGS)

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)     \
			-I$(COMMON_API_INCLUDE)     \
			-I$(COMMON_DRV_INCLUDE)     \
			-I$(MSP_UNF_INCLUDE)        \
			-I$(MSP_API_INCLUDE)        \
			-I$(MSP_DRV_INCLUDE)        \
			-I$(CURDIR)                 \
			-I$(HA_CODEC_DIR)/include

ifneq ($(findstring -fPIC, $(CFLAGS)), -fPIC)
CFLAGS += -fPIC
endif

CFLAGS64 := $(filter-out $(FILTER),$(CFLAGS))

FILTER += -mcpu=cortex-a9 -mcpu=cortex-a7 -mcpu=cortex-a53
CFLAGS32 := $(filter-out $(FILTER),$(CFLAGS)) $(FLOAT_OPTIONS)
CFLAGS32 += -mcpu=cortex-a9

OBJS := $(C_SRCS:%.c=%.o) $(CPP_SRCS:%.cpp=%.o) $(ASM_SRCS:%.S=%.o)

OBJ_DIR32 := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(CURDIR))
OBJ_DIR64 := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(CURDIR))

ifeq ($(USE_VPATH),)
OBJS32 := $(addprefix $(OBJ_DIR32)/,$(OBJS))
OBJS64 := $(addprefix $(OBJ_DIR64)/,$(OBJS))
else
OBJS32 := $(addprefix $(OBJ_DIR32)/, $(notdir $(OBJS)))
OBJS64 := $(addprefix $(OBJ_DIR64)/, $(notdir $(OBJS)))
endif

# ha_codec release
RELEASE_DIR32 := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(HA_CODEC_DIR))/lib/
RELEASE_DIR64 := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(HA_CODEC_DIR))/lib/

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += $(if $(LIB_NAME),$(OBJ_DIR32)/$(LIB_NAME),$(OBJS32))
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += $(if $(LIB_NAME),$(OBJ_DIR64)/$(LIB_NAME),$(OBJS64))
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all : $(TARGETS)
ifneq ($(HEADER_FILES),)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -f $(HEADER_FILES) $(HI_INCLUDE_DIR)/
endif
ifneq ($(LIB_NAME),)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)cp -f $(OBJ_DIR32)/$(LIB_NAME).a $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_EXTERN_LIB_DIR) || mkdir -p $(HI_EXTERN_LIB_DIR)
	$(AT)cp -f $(OBJ_DIR32)/$(LIB_NAME).so $(HI_EXTERN_LIB_DIR)
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)cp -f $(OBJ_DIR64)/$(LIB_NAME).a $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_EXTERN_LIB64_DIR) || mkdir -p $(HI_EXTERN_LIB64_DIR)
	$(AT)cp -f $(OBJ_DIR64)/$(LIB_NAME).so $(HI_EXTERN_LIB64_DIR)
endif
endif

clean :
	$(AT)rm -rf $(OBJS32) $(OBJS64)
ifneq ($(LIB_NAME),)
	$(AT)rm -rf $(OBJ_DIR32)/$(LIB_NAME).a $(OBJ_DIR32)/$(LIB_NAME).so
	$(AT)rm -rf $(OBJ_DIR64)/$(LIB_NAME).a $(OBJ_DIR64)/$(LIB_NAME).so
endif
ifneq ($(HEADER_FILES),)
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))
endif
ifneq ($(LIB_NAME),)
	$(AT)rm -f $(HI_STATIC_LIB_DIR)/$(LIB_NAME).a $(HI_EXTERN_LIB_DIR)/$(LIB_NAME).so
	$(AT)rm -f $(HI_STATIC_LIB64_DIR)/$(LIB_NAME).a $(HI_EXTERN_LIB64_DIR)/$(LIB_NAME).so
endif

release : all
ifneq ($(LIB_NAME),)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(RELEASE_DIR32) || mkdir -p $(RELEASE_DIR32)
	$(AT)cp -f $(OBJ_DIR32)/$(LIB_NAME).so $(RELEASE_DIR32)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip -s -R .comment $(RELEASE_DIR32)/$(LIB_NAME).so
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip --strip-unneeded $(RELEASE_DIR32)/$(LIB_NAME).so
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(RELEASE_DIR64) || mkdir -p $(RELEASE_DIR64)
	$(AT)cp -f $(OBJ_DIR64)/$(LIB_NAME).so $(RELEASE_DIR64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip -s -R .comment $(RELEASE_DIR64)/$(LIB_NAME).so
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip --strip-unneeded $(RELEASE_DIR64)/$(LIB_NAME).so
endif
endif

ifeq ($(HI_USER_SPACE_LIB),y)
$(OBJ_DIR32)/%.o : %.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS32) -o $@ -c $^

$(OBJ_DIR32)/%.o : %.S
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS32) -o $@ -c $^

$(OBJ_DIR32)/%.o : %.cpp
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS32) -o $@ -c $^
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
$(OBJ_DIR64)/%.o : %.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS64) -o $@ -c $^

$(OBJ_DIR64)/%.o : %.S
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS64) -o $@ -c $^

$(OBJ_DIR64)/%.o : %.cpp
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS64) -o $@ -c $^
endif

ifneq ($(LIB_NAME),)
ifeq ($(HI_USER_SPACE_LIB),y)
$(OBJ_DIR32)/$(LIB_NAME): $(OBJS32)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar -rcs $@.a $(OBJS32)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc -shared -o $@.so $(OBJS32)
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
$(OBJ_DIR64)/$(LIB_NAME): $(OBJS64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar -rcs $@.a $(OBJS64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc -shared -o $@.so $(OBJS64)
endif
endif