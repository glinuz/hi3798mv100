# Copyright (c) 2008-2012 S00133955/HUAWEI  All rights reserved.
#  

OUTPUT_NAME_SO = libhwmedia.so
OUTPUT_NAME_LIB = libhwmedia.a
ifeq ($(ARCH),ARM)
CROSS_PREFIX = arm-hisi_vfpv3d16-linux-
OUTPUT_DIR = ../bin/arm-hisi_linux/
else
OUTPUT_DIR = ../bin/suse11_i386/
endif
CC = $(CROSS_PREFIX)gcc
LINKER = $(CROSS_PREFIX)gcc
AR = $(CROSS_PREFIX)ar
RANLIB = $(CROSS_PREFIX)ranlib
STRIP = $(CROSS_PREFIX)strip
AS = yasm
CORE_LINK_LIBS =
#C++ = g++
#LINKER = g++
#CORE_LINK_LIBS = -lpthread -ldl -lm -lcrypt -lstdc++

# C COMPILE FLAGS
CCFLAGS +=  -freg-struct-return
ifndef CFG_HI_STATIC_LIB_ONLY
CCFLAGS  += -fPIC
endif

#CCFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_REENTRANT -D__USE_POSIX -D__linux__ -pipe

# ASM COMPILE FLAGS
ASFLAGS =

ifeq ($(DEST),debug)
CCFLAGS += -g -Wall
ASFLAGS += -W
else
CCFLAGS += -O2
ASFLAGS += -O2
endif

#CCFLAGS += -std=c99 -D_GNUC_
CCFLAGS += -D_ISOC99_SOURCE -D_POSIX_C_SOURCE=200112 -std=c99 -fomit-frame-pointer

ifeq ($(ARCH),ARM)
CCFLAGS += -DBUILD_ARM -mtune=cortex-a9 -march=armv7-a -fno-signed-zeros -DHAVE_IPP=0
#elifeq ($(ARCH),X86_64)
#CCFLAGS += -DBUILD_X86
#ASFLAGS += -f elf64
else
CCFLAGS += -DBUILD_X86 -m32 -DSYS_LINUX
ASFLAGS += -f elf
endif

# EACH DIRECTORY WITH HEADERS MUST BE APPENDED IN THIS MANNER TO THE CCFLAGS
CCFLAGS += -I./
CCFLAGS += -I../inc
CCFLAGS += -I./hwdec/inc
CCFLAGS += -I./hwdec/hwcodec
CCFLAGS += -I./hwdec/hwutil
CCFLAGS += -I./hwimage/inc
CCFLAGS += -I./hwenc_h264

ASFLAGS += -I./x86

# CPP COMPILE FLAGS
C++FLAGS = $(CCFLAGS)

# lib
LINK_OPTS += -L./
LINK_OPTS += -L./hwimage
LINK_OPTS += -L./hwdec
LINK_OPTS += -L./hwenc_h264

LINK_LIBS = -lhwdec
LINK_LIBS += -Wl,-rpath=./

CFILES = ./imedia_util.c \
	./imedia_viddec.c

ifeq ($(ARCH),ARM)
ASMFILES = 
else
ASMFILES = ./x86/cpu-a.asm
endif

SFILES = 	

CPPFILES =

LIBFILES =

all: $(OUTPUT_NAME_SO)

$(OUTPUT_NAME_SO): $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o) $(LIBFILES)
	$(LINKER) -o $@ -rdynamic -shared -Wl,-soname,$@ $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o) $(LINK_OPTS) $(LINK_LIBS) $(CORE_LINK_LIBS)
	cp $@ $(OUTPUT_DIR)
	#cp $@ /lib/$@
	
.PHONY: lib
lib: $(OUTPUT_NAME_LIB)

$(OUTPUT_NAME_LIB): $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	$(AR) -r $@ $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	$(RANLIB) $@
	cp $@ $(OUTPUT_DIR)
	
.PHONY: clean
clean:
	rm -f $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	rm -f $(OUTPUT_NAME_SO)
	rm -f $(OUTPUT_NAME_LIB)

.SUFFIXES: .cpp .c .asm .o

.cpp.o:
	$(C++) $(C++FLAGS) -c -o $*.o $*.cpp

.c.o:
	$(CC) $(CCFLAGS) -c -o $*.o $*.c

.S.o:
	$(CC) $(CCFLAGS) -c -o $*.o $*.S
	
.asm.o:
	$(AS) $(ASFLAGS) -o $*.o $*.asm
