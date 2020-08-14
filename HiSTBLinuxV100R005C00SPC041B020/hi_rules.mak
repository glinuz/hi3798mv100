#-------------------------------------------------------------------------------
# Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
#-------------------------------------------------------------------------------
CFLAGS  +=  $(filter-out $(FILTER),$(CFG_HI_CFLAGS)) 

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)     \
            -I$(COMMON_API_INCLUDE)     \
            -I$(COMMON_DRV_INCLUDE)     \
            -I$(MSP_UNF_INCLUDE)        \
            -I$(MSP_API_INCLUDE)        \
            -I$(MSP_DRV_INCLUDE)        \
            -I$(CURDIR)

OBJS := $(SRCS:%.c=%.o) $(CPP_SRCS:%.cpp=%.o) $(ASM_SRCS:%.S=%.o)

ifneq ($(LOCAL_SRC_DIR),)
OBJS := $(notdir $(OBJS))
endif

OBJ_DIR32 := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(CURDIR))
OBJ_DIR64 := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(CURDIR))

OBJS32 += $(addprefix $(OBJ_DIR32)/,$(OBJS))
OBJS64 += $(addprefix $(OBJ_DIR64)/,$(OBJS))

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += $(OBJ_DIR32)/$(LIB_NAME)
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += $(OBJ_DIR64)/$(LIB_NAME)
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all : $(TARGETS)
ifneq ($(HEADER_FILES),)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -f $(HEADER_FILES) $(HI_INCLUDE_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)cp -f $(OBJ_DIR32)/$(LIB_NAME).a $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -f $(OBJ_DIR32)/$(LIB_NAME).so $(HI_SHARED_LIB_DIR)
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)cp -f $(OBJ_DIR64)/$(LIB_NAME).a $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -f $(OBJ_DIR64)/$(LIB_NAME).so $(HI_SHARED_LIB64_DIR)
endif

clean :
	$(AT)rm -rf $(OBJS32) $(OBJS64)
	$(AT)rm -rf $(OBJ_DIR32)/$(LIB_NAME).a $(OBJ_DIR32)/$(LIB_NAME).so
	$(AT)rm -rf $(OBJ_DIR64)/$(LIB_NAME).a $(OBJ_DIR64)/$(LIB_NAME).so
ifneq ($(HEADER_FILES),)
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))
endif
	$(AT)rm -f $(HI_STATIC_LIB_DIR)/$(LIB_NAME).a $(HI_SHARED_LIB_DIR)/$(LIB_NAME).so
	$(AT)rm -f $(HI_STATIC_LIB64_DIR)/$(LIB_NAME).a $(HI_SHARED_LIB64_DIR)/$(LIB_NAME).so

$(OBJ_DIR32)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.c,%.c)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR32)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.cpp,%.cpp)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR32)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.S,%.S)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR64)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.c,%.c)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS) -o $@ -c $^

$(OBJ_DIR64)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.cpp,%.cpp)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS) -o $@ -c $^

$(OBJ_DIR64)/%.o : $(if $(LOCAL_SRC_DIR),$(LOCAL_SRC_DIR)/%.S,%.S)
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS) -o $@ -c $^

$(OBJ_DIR32)/$(LIB_NAME): $(OBJS32)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar -rcs $@.a $(OBJS32)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc -shared -o $@.so $(OBJS32)

$(OBJ_DIR64)/$(LIB_NAME): $(OBJS64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar -rcs $@.a $(OBJS64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc -shared -o $@.so $(OBJS64)
