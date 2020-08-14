# Copyright (c) 2008-2012 S00133955/HUAWEI  All rights reserved.
#  

OUTPUT_NAME_SO = libhwdec.so
OUTPUT_NAME_LIB = libhwdec.a
ifeq ($(ARCH),ARM)
CROSS_PREFIX = arm-hisi_vfpv3d16-linux-
OUTPUT_DIR = ../../bin/arm-hisi_linux/
else
OUTPUT_DIR = ../../bin/suse11_i386/
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
#CCFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_REENTRANT -D__USE_POSIX -D__linux__ -pipe

# ASM COMPILE FLAGS
ASFLAGS =

ifeq ($(DEST),debug)
CCFLAGS += -g -Wall -O
ASFLAGS += -W
else
CCFLAGS += -O2
#ASFLAGS += -O2
endif

CCFLAGS += -DHAVE_AV_CONFIG_H
CCFLAGS += -D_ISOC99_SOURCE -D_POSIX_C_SOURCE=200112 -std=c99 -fomit-frame-pointer -Wdeclaration-after-statement -Wno-switch -Wdisabled-optimization -Wpointer-arith -Wredundant-decls -Wno-pointer-sign -Wcast-qual -Wwrite-strings -Wundef -fno-math-errno 

#CCFLAGS += -std=c99 -D_GNUC_
ifeq ($(ARCH),ARM)
CCFLAGS += -DBUILD_ARM -mtune=cortex-a9 -march=armv7-a -fno-signed-zeros -DHAVE_IPP=0 -DARM_ASM_OPT -DASMIDCT
ifndef CFG_HI_STATIC_LIB_ONLY
CCFLAGS  += -fPIC
endif
#CCFLAGS += -DBUILD_ARM -mtune=cortex-a9 -march=armv7-a -fno-signed-zeros -DHAVE_IPP=0 -fPIC 
ASFLAGS += --defsym ARCHITECTURE=7 -march=armv7-a
AS = $(CROSS_PREFIX)as
#elifeq ($(ARCH),X86_64)
#CCFLAGS += -DBUILD_X86
#ASFLAGS += -f elf64
else
CCFLAGS += -DBUILD_X86 -m32 -DSYS_LINUX
ASFLAGS += -f elf
endif

# EACH DIRECTORY WITH HEADERS MUST BE APPENDED IN THIS MANNER TO THE CCFLAGS
CCFLAGS += -I./
CCFLAGS += -I./inc
CCFLAGS += -I./hwcodec
CCFLAGS += -I./hwcodec/x86
CCFLAGS += -I./hwcodec/x86/asm
CCFLAGS += -I./hwutil
CCFLAGS += -I./hwutil/x86
CCFLAGS += -I../../inc

ASFLAGS += -I../x86

# CPP COMPILE FLAGS
C++FLAGS = $(CCFLAGS)

# lib
LINK_OPTS += -L./

LINK_LIBS = 

CFILES = ./probe_header.c \
	./hwutil/avstring.c \
	./hwutil/log.c \
	./hwutil/mathematics.c \
	./hwutil/mem.c \
	./hwcodec/bitstream.c \
	./hwcodec/cabac.c \
	./hwcodec/dsputil.c \
	./hwcodec/error_resilience.c \
	./hwcodec/golomb.c \
	./hwcodec/h264.c \
	./hwcodec/h264_parser.c \
	./hwcodec/h264idct.c \
	./hwcodec/h264pred.c \
	./hwcodec/imgconvert.c \
	./hwcodec/jrevdct.c \
	./hwcodec/mpeg12.c \
	./hwcodec/mpeg12data.c \
	./hwcodec/mpegvideo.c \
	./hwcodec/mpegvideo_parser.c \
	./hwcodec/parser.c \
	./hwcodec/rv10.c \
	./hwcodec/simple_idct.c \
	./hwcodec/h263.c \
	./hwcodec/utils.c \
	./hwcodec/mdct.c \
	./hwcodec/options.c \
	./hwcodec/opt.c \
	./hwcodec/eval.c \
	./hwcodec/h263dec.c \
	./hwcodec/h263_parser.c \
	./hwcodec/mpeg4video_parser.c \
	./hwcodec/msmpeg4.c \
	./hwcodec/msmpeg4data.c \
	./hwcodec/vc1.c \
	./hwcodec/vc1_parser.c \
	./hwcodec/vc1data.c \
	./hwcodec/vc1dsp.c \
	./hwcodec/wmv2dec.c \
	./hwcodec/intrax8.c \
	./hwcodec/intrax8dsp.c \
	./hwcodec/huffman.c \
	./hwcodec/vp3.c \
	./hwcodec/vp3_parser.c \
	./hwcodec/vp3dsp.c \
	./hwcodec/vp5.c \
	./hwcodec/vp6.c \
	./hwcodec/vp6dsp.c \
	./hwcodec/vp56.c \
	./hwcodec/vp56data.c 
		

ifeq ($(ARCH),ARM)
CFILES += ./hwcodec/arm/dsputil_arm.c \
	./hwcodec/arm/float_arm_vfp.c \
	./hwcodec/arm/mpegvideo_arm.c \
	./hwcodec/arm/mpegvideo_armv5te.c \
	./hwcodec/arm/dsputil_init_armv6.c
	
SFILES += ./hwcodec/arm/dsputil_arm_s.S \
	./hwcodec/arm/dsputil_vfp.S \
	./hwcodec/arm/jrevdct_arm.S \
	./hwcodec/arm/mpegvideo_armv5te_s.S \
	./hwcodec/arm/simple_idct_arm.S \
	./hwcodec/arm/simple_idct_armv5te.S \
	./hwcodec/arm/simple_idct_armv6.S \
	./hwcodec/arm/h263_loop_armv6.S \
	./hwcodec/arm/dsputil_armv6.S \
	./hwcodec/arm/idct.S \
	./hwcodec/arm/add_idct.S

SFILES += ./hwcodec/arm/vp6_filter_hv4_armv6.S \
		    ./hwcodec/arm/vp6_filter_onetime_armv6.S \
		    ./hwcodec/arm/vp56_edge_filterarmv6.S \
		    ./hwcodec/arm/vp6_twotapfilter_2d_armv6.S \
		    ./hwcodec/arm/vp6_twotapfilter_1d_armv6.S

else
CFILES += ./hwcodec/x86/simple_idct_mmx.c \
	./hwcodec/x86/dsputil_mmx.c \
	./hwcodec/x86/idct_sse2_xvid.c \
	./hwcodec/x86/mpegvideo_mmx.c \
	./hwcodec/x86/idct_mmx.c \
	./hwcodec/x86/idct_mmx_xvid.c \
	./hwcodec/x86/fft_3dn2.c \
	./hwcodec/x86/fft_sse.c \
	./hwcodec/x86/fdct_mmx.c \
	./hwcodec/x86/cpuid.c \
	./hwcodec/x86/vp3dsp_sse2.c \
	./hwcodec/x86/vp6dsp_mmx.c \
	./hwcodec/x86/vp6dsp_sse2.c \
	./hwcodec/x86/vp3dsp_mmx.c \
	./hwcodec/x86/vc1dsp_mmx.c

SFILES = 	

ASMFILES = ./hwcodec/x86/asm/dsputil_yasm.asm \
	./hwcodec/x86/asm/fft_mmx.asm \
	./hwcodec/x86/asm/h264_deblock_sse2.asm \
	./hwcodec/x86/asm/h264_idct_sse2.asm \
	./hwcodec/x86/asm/x86inc.asm \
	./hwcodec/x86/asm/x86util.asm
endif

CPPFILES = 

LIBFILES =

all: $(OUTPUT_NAME_SO)

$(OUTPUT_NAME_SO): $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o) $(LIBFILES)
	$(LINKER) -o $@ -rdynamic -shared -Wl,-soname,$@ $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o) $(LINK_OPTS) $(LINK_LIBS) $(CORE_LINK_LIBS)
	cp $@ $(OUTPUT_DIR)
	
.PHONY: lib
lib: $(OUTPUT_NAME_LIB)

$(OUTPUT_NAME_LIB): $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	$(AR) -r $@ $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	$(RANLIB) $@
	#cp $@ $(OUTPUT_DIR)
	
.PHONY: clean
clean:
	rm -f $(CFILES:.c=.o) $(SFILES:.S=.o) $(CPPFILES:.cpp=.o) $(ASMFILES:.asm=.o)
	rm -f ./hwcodec/arm/*.o
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
