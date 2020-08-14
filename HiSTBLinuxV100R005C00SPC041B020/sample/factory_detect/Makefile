#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS  :=  $(CFG_HI_SAMPLE_CFLAGS)

CFLAGS  +=  -I$(HI_INCLUDE_DIR) \
            -I$(SAMPLE_DIR)/common \
            -I./include \
            -I./configs \
            -I./adapter \
            -I./ini \
            -I./src

# KEY_LED define
ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
CFLAGS += -DHI_BUILD_WITH_KEYLED
ifeq ($(CFG_HI_KEYLED_PT6964_SELECT),y)
CFLAGS += -DHI_KEYLED_PT6964
endif
ifeq ($(CFG_HI_KEYLED_PT6961_SELECT),y)
CFLAGS += -DHI_KEYLED_PT6961
endif
ifeq ($(CFG_HI_KEYLED_CT1642_SELECT),y)
CFLAGS += -DHI_KEYLED_CT1642
endif
ifeq ($(CFG_HI_KEYLED_74HC164_SELECT),y)
CFLAGS += -DHI_KEYLED_74HC164
endif
ifeq ($(CFG_HI_KEYLED_FD650_SELECT),y)
CFLAGS += -DHI_KEYLED_FD650
endif
ifeq ($(CFG_HI_KEYLED_GPIOKEY_SELECT),y)
CFLAGS += -DHI_KEYLED_GPIOKEY
endif
endif

VPATH := $(CURDIR)/src $(CURDIR)/adapter $(CURDIR)/configs $(CURDIR)/ini $(SAMPLE_DIR)/common

SRCS := fv_entry.o \
        fv_eth.o \
        fv_face.o \
        fv_misc.o \
        fv_storage.o \
        fv_sys.o \
        fv_tsplay.o \
        fv_tuner.o \
        fv_adapter.o \
        fv_configs.o \
        minIni.o \
        hi_adp_mpi.o \
        hi_adp_search.o \
        hi_adp_hdmi.o \
        hi_adp_data.o \
        hi_adp_demux.o

FV_TARGET_DIR := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(CURDIR))/
FV_TARGET_DIR64 := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(CURDIR))/

FV_EXE_NAME := $(FV_TARGET_DIR)/hifv
FV_EXE64_NAME := $(FV_TARGET_DIR64)/hifv

OBJS := $(addprefix $(FV_TARGET_DIR)/, $(patsubst %.c,%.o,$(notdir $(SRCS))))
OBJS64 := $(addprefix $(FV_TARGET_DIR64)/, $(patsubst %.c,%.o,$(notdir $(SRCS))))

LIB_PATHS := -L$(HI_SHARED_LIB_DIR) -L$(HI_EXTERN_LIB_DIR) -L$(HI_STATIC_LIB_DIR)
LIB64_PATHS := -L$(HI_SHARED_LIB64_DIR) -L$(HI_EXTERN_LIB64_DIR) -L$(HI_STATIC_LIB64_DIR)

LIBS := -Wl,--start-group $(SYS_LIBS) $(HI_LIBS) -Wl,--end-group

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += $(FV_EXE_NAME)
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += $(FV_EXE64_NAME)
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all: $(TARGETS)

clean:
	$(AT)rm -rf $(OBJS) $(OBJS64) $(FV_TARGET_DIR) $(FV_TARGET_DIR64)

$(FV_EXE_NAME): $(OBJS)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $^ $(CFLAGS) -o $@ $(LIB_PATHS) $(LIBS)

$(FV_EXE64_NAME): $(OBJS64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $^ $(CFLAGS) -o $@ $(LIB64_PATHS) $(LIBS)

$(FV_TARGET_DIR)/%.o : %.c
	$(AT)mkdir -p $(FV_TARGET_DIR)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) -c $< -o $@

$(FV_TARGET_DIR64)/%.o : %.c
	$(AT)mkdir -p $(FV_TARGET_DIR64)
	$(AT)$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc $(CFLAGS) -c $< -o $@

