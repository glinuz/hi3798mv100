#===============================================================================
# for apploader only
#===============================================================================

SRC_DIR := $(CURDIR)

IMAGE_NAME := loader

OBJ_DIR := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(SRC_DIR))
OBJ64_DIR := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(SRC_DIR))

RELEASE_DIR	:= $(OBJ_DIR)/release
RELEASE64_DIR := $(OBJ64_DIR)/release

HI_LIB_PATHS := -L$(HI_STATIC_LIB_DIR)
HI_LIB64_PATHS := -L$(HI_STATIC_LIB64_DIR)

ifeq ($(HI_USER_SPACE_LIB),y)
RELEASE := $(RELEASE_DIR)
OBJECT := $(OBJ_DIR)
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
RELEASE := $(RELEASE64_DIR)
OBJECT := $(OBJ64_DIR)
endif

C_FLAGS += -g -rdynamic -O2
C_FLAGS += -I$(COMMON_UNF_INCLUDE)
C_FLAGS += -I$(COMMON_API_INCLUDE)
C_FLAGS += -I$(COMMON_DRV_INCLUDE)
C_FLAGS += -I$(MSP_UNF_INCLUDE)
C_FLAGS += -I$(MSP_API_INCLUDE)
C_FLAGS += -I$(MSP_DRV_INCLUDE)
C_FLAGS += $(LOADER_INC)
C_FLAGS += $(LOADER_DEF)

LD_FLAGS := -Wl,--start-group
LD_FLAGS += -lrt -lpthread -ldl -lm -lstdc++ -lz
LD_FLAGS += -l:libfreetype.a -l:libhi_common.a -l:libhi_msp.a -l:libhigoadp.a -l:libhigo.a -l:libhiloader.a
LD_FLAGS += $(LOADER_LIB)

LD_FLAGS += -Wl,--end-group

FLOAT_OPTIONS := -mfloat-abi=softfp -mfpu=vfpv3-d16

OBJS := $(addprefix $(OBJ_DIR)/, $(LOADER_OBJS))
OBJS64 := $(addprefix $(OBJ64_DIR)/, $(LOADER_OBJS))
#===============================================================================
# rules
#===============================================================================
.PHONY: all clean $(RELEASE_DIR)/$(IMAGE_NAME) $(RELEASE64_DIR)/$(IMAGE_NAME)


all:  
	$(AT)test -d $(RELEASE) || mkdir -p $(RELEASE)

ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
	$(AT)make -C ca all
endif

	$(AT)make $(RELEASE)/$(IMAGE_NAME)
	
	-$(AT)cp -rvf $(CURDIR)/res $(RELEASE)/

clean: 
	$(AT)rm -rf $(OBJECT)
	
ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
	$(AT)make -C ca clean
endif

	
$(RELEASE_DIR)/$(IMAGE_NAME): $(OBJS)
	$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc -o $@ $^ $(C_FLAGS) $(HI_LIB_PATHS) $(LD_FLAGS) 
	$(STRIP) $(RELEASE_DIR)/$(IMAGE_NAME)

$(RELEASE64_DIR)/$(IMAGE_NAME): $(OBJS64)
	$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc -o $@ $^ $(C_FLAGS) $(HI_LIB64_PATHS) $(LD_FLAGS) 
	$(STRIP) $(RELEASE64_DIR)/$(IMAGE_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(C_FLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ64_DIR)/%.o: $(SRC_DIR)/%.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(C_FLAGS) -o $@ -c $^
