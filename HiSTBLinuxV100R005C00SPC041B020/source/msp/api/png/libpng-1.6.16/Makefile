# makefile for libpng.a and libpng14.so on Linux ELF with gcc
# Copyright (C) 1998, 1999, 2002, 2006, 2008, 2010 Greg Roelofs and
# Glenn Randers-Pehrson
# Copyright (C) 1996, 1997 Andreas Dilger
#
# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
PNG_CURDIR = $(shell pwd)
SRC_CURDIR :=

CFLAGS  := $(subst -Werror,,$(CFG_HI_CFLAGS))

#这个开关会增加库的编译大小
#CFLAGS +=  -funroll-loops

CFLAGS  +=  -I$(MSP_DIR)/api/png/include            \
            -I$(MSP_DIR)/drv/png/include            \
            -I$(MSP_DIR)/api/jpeg/grc_cmm_inc       \
            -I$(MSP_DIR)/drv/include                \
            -I$(COMMON_DIR)/api/include             \
            -I$(COMPONENT_DIR)/zlib/include         \
            -I$(COMPONENT_DIR)/freetype/include

CFLAGS  +=  -DHIGFX_COMMON

ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv310 hi3716mv320 hi3110ev500),)
CFLAGS += -DHIPNG_ACCELERATE
CFLAGS += -DPNG_WRITE_SUPPORTED -DPNG_WRITE_FILLER_SUPPORTED
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3796mv200),)
CFLAGS += -DCONFIG_GFX_MEM_ION
else
CFLAGS += -DCONFIG_GFX_MEM_MMZ
endif

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DCONFIG_GFX_MMU_SUPPORT
endif

ifeq ($(CFG_HI_GFX_DEBUG_LOG_SUPPORT), y)
CFLAGS += -DCFG_HI_GFX_DEBUG_LOG_SUPPORT
endif

ifneq ($(CFG_HI_PROC_SUPPORT),y)
CFLAGS += -DCONFIG_PNG_PROC_DISABLE
endif

ifdef CFG_HI_STATIC_LIB_ONLY
#directfb needed
CFLAGS += -fPIC
endif

ZLIBLIB = $(COMPONENT_DIR)/zlib/release/lib
ZLIBINC = $(COMPONENT_DIR)/zlib/include

#=====================================================================================
#the chip env version
#=====================================================================================
LIBNAME :=
LIBPATH := -L$(COMMON_DIR)/api -L$(MSP_DIR)/api
DEPEND_LIB := $(LIBPATH) $(LIBNAME)

#===============================================================================
# whether debug and add googlefunction
#===============================================================================

#===============================================================================
# complie
#===============================================================================
CPPFLAGS = 
LDFLAGS  =

#===============================================================================
#compile lib src
#===============================================================================
SOURCES :=  png.c pngset.c pngget.c pngrutil.c pngtrans.c pngwutil.c    \
	    pngread.c pngrio.c pngwio.c pngwrite.c pngrtran.c           \
	    pngwtran.c pngmem.c pngerror.c pngpread.c                   \
            png_accelerate.c hi_png_api.c

SRCS := $(SOURCES)

#===============================================================================
#compile
#===============================================================================
LIB_NAME := libpng1.6.16

HEADER_FILES := $(MSP_DIR)/api/png/include/hi_png_config.h
HEADER_FILES += $(MSP_DIR)/api/png/libpng-1.6.16/png.h
HEADER_FILES += $(MSP_DIR)/api/png/libpng-1.6.16/pngconf.h

include $(SDK_DIR)/hi_rules.mak

