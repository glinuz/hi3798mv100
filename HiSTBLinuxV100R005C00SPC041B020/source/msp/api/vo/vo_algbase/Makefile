
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../../../..

include $(SDK_DIR)/base.mak
endif

#******	define PATH ***********************************************************
TARGET = libhi_algbase
CUR_DIR = .
TARGET_SRC_DIR = $(CUR_DIR)/src

TAR_DIR = $(CUR_DIR)/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)
OBJ_DIR = $(CUR_DIR)/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)/obj


#******	define CFLAG ***********************************************************
CFG_INC=-I$(TARGET_SRC_DIR) -I$(CUR_DIR)


#
# compiler flags (for normal, DEBUG, and WMOPS compilation)
#
CFLAGS_NORM  = $(USER_CFLAGS)
CFLAGS_NORM  += -O2 -DTARGET_ARM

#PREFIX = $(TARGET)_

CFLAGS = -Wall $(CFLAGS_NORM) 

ifndef CFG_HI_STATIC_LIB_ONLY
CFLAGS += -fPIC
endif

CFLAGS += $(CFG_INC)




COMPILE=$(CC) -c $(CFLAGS) -o "$(OBJ_DIR)/$(PREFIX)$(*F).o" "$<"  

TARGET_OBJS = $(OBJ_DIR)/bitstream.o \
	      $(OBJ_DIR)/compress.o

TARGET_LIB=$(TAR_DIR)/$(TARGET).so
TARGET_LIB_A=$(TAR_DIR)/$(TARGET).a
ALL_OBJS=$(TARGET_OBJS)

#
# default target: build standalone speech encoder and decoder
#
default: build_prepare target_lib



build_prepare:
	@rm -rf $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
#*** OBJs Define ***************************************************************
$(OBJ_DIR)/%.o : $(TARGET_SRC_DIR)/%.c
	@$(COMPILE)

#
# build the capture library
#

target_lib: $(ALL_OBJS)
	$(AR) -rsv $(TARGET_LIB_A)  $(OBJ_DIR)/*.o
#	$(CC) -o $(TARGET_LIB) -shared $(OBJ_DIR)/*.o

#
# remove object/executable files
#
clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f $(TAR_DIR)/*.so
	-rm -f $(TAR_DIR)/*.a
