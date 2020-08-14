MYCC             = $(CC)-fPIC

MAKEFILENAME = makefile

#****** 定义目标输出名称的  ****************************************************
TARGET = libHA.AUDIO.MAD.decode

#****** 定义要编译目标文件的存放位置 *******************************************
ROOT_DIR = .
OBJ_DIR  = $(ROOT_DIR)/lib/obj
TAR_DIR  = $(ROOT_DIR)/lib

#****** 定义源代码及头文件的位置 ***********************************************
TARGET_INC_DIR = $(ROOT_DIR)/include
TARGET_SRC_DIR = $(ROOT_DIR)/src/libmad-0.15.1b
TARGET_WRAP_DIR= $(ROOT_DIR)/src

INC_DIR  = $(ROOT_DIR)/include
INC_SRC  = $(ROOT_DIR)/src/libmad-0.15.1b

#****** 定义头文件的搜索路径 ***************************************************
MYCFG_INC =-I$(TARGET_INC_DIR)  -I$(TARGET_SRC_DIR) -I$(INC_SRC) -I$(CFG_INC)
MYCFG_INC += $(CFG_INC)



COMPILE=$(MYCC) -c -I$(MYCFG_INC) -DFPM_ARM -o "$(OBJ_DIR)/$(PREFIX)$(*F).o" "$<"

TARGET_LIB=$(TAR_DIR)/$(TARGET).so

#
# source/object files
#

TARGET_SRCS=$(TARGET_SRC_DIR)/bit.c \
            $(TARGET_SRC_DIR)/decoder.c \
            $(TARGET_SRC_DIR)/fixed.c \
            $(TARGET_SRC_DIR)/frame.c \
            $(TARGET_SRC_DIR)/huffman.c \
            $(TARGET_SRC_DIR)/layer12.c \
            $(TARGET_SRC_DIR)/layer3.c \
            $(TARGET_SRC_DIR)/stream.c \
            $(TARGET_SRC_DIR)/synth.c \
            $(TARGET_SRC_DIR)/timer.c \
            $(TARGET_SRC_DIR)/version.c \
            $(TARGET_SRC_DIR)/imdct_l_arm.S \
            $(TARGET_WRAP_DIR)/ha_libmad_wrapper.c

TARGET_OBJS=$(OBJ_DIR)/bit.o \
            $(OBJ_DIR)/decoder.o \
            $(OBJ_DIR)/fixed.o \
            $(OBJ_DIR)/frame.o \
            $(OBJ_DIR)/huffman.o \
            $(OBJ_DIR)/layer12.o \
            $(OBJ_DIR)/layer3.o \
            $(OBJ_DIR)/stream.o \
            $(OBJ_DIR)/synth.o \
            $(OBJ_DIR)/timer.o \
            $(OBJ_DIR)/version.o \
            $(OBJ_DIR)/imdct_l_arm.o \
            $(OBJ_DIR)/ha_libmad_wrapper.o


ALL_SRCS=$(TARGET_SRCS) 
ALL_OBJS=$(TARGET_OBJS)


#
# default target: build standalone speech encoder and decoder
#
default: build_prepare target_lib 


#*** OBJs Define ***************************************************************
$(OBJ_DIR)/%.o : $(TARGET_SRC_DIR)/%.c 
	@$(COMPILE)
$(OBJ_DIR)/%.o : $(TARGET_WRAP_DIR)/%.c 
	@$(COMPILE)
$(OBJ_DIR)/%.o : $(TARGET_SRC_DIR)/%.S
	@$(COMPILE)

build_prepare:
	@mkdir -p $(OBJ_DIR)

target_lib: $(ALL_OBJS) 
	@$(MYCC) -o $(TARGET_LIB) -shared $(OBJ_DIR)/*.o



#
# make / clean dependency list
#
depend:
	@$(MAKE) -f $(MAKEFILENAME) $(MFLAGS) $(MAKEDEFS) clean_depend

clean_depend:
	chmod u+w $(MAKEFILENAME)
	(awk 'BEGIN{f=1}{if (f) print $0}/^\# DO NOT DELETE THIS LINE -- make depend depends on it./{f=0}'\
	    < $(MAKEFILENAME) > .depend && \
	mv .depend $(MAKEFILENAME)) || exit 1;


#
# remove object/executable files
#
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TAR_DIR)/*.a
	rm -f $(TAR_DIR)/*.so

	
