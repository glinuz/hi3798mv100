#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

JPEG_CURDIR = $(shell pwd)
SRC_CURDIR := .

#CFG_HI_ENC_CFLAGS = CONFIG_GFX_JPGE_ENC_ENABLE
CFG_HI_JPEG_SOFT_ENCODE_SUPPORT=y
#===============================================================================
# standard jpeg dir
#===============================================================================
SRC_9A       := $(SRC_CURDIR)/src_9a
SRC_SIMD     := $(SRC_CURDIR)/src_simd
SRC_HARD     := $(SRC_CURDIR)/src_hard
SRC_HARD_ENC := $(SRC_CURDIR)/src_hard_enc

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(JPEG_CURDIR)                       \
            -I$(SRC_9A)                            \
            -I$(SRC_SIMD)                          \
            -I$(SRC_HARD)                          \
            -I$(SRC_HARD_ENC)                      \
            -I$(JPEG_CURDIR)/../inc/inc_soft       \
            -I$(JPEG_CURDIR)/../inc/inc_hard       \
            -I$(JPEG_CURDIR)/../inc/inc_test       \
            -I$(JPEG_CURDIR)/../grc_cmm_inc        \
            -I$(MSP_DIR)/api/tde/include           \
            -I$(MSP_DIR)/api/jpge/include          \
            -I$(MSP_DIR)/drv/include/              \
            -I$(COMMON_DIR)/api/include            \
            -I$(MSP_DIR)/drv/jpeg/include

CFLAGS  +=  -DCFG_HI_JPEG6B_STREAMBUFFER_SIZE=$(CFG_HI_JPEG6B_STREAMBUFFER_SIZE)

#=====================================================================================
#the jpeg use version
#=====================================================================================
CFLAGS += -DCONFIG_HI_SDK_JPEG_VERSION
#CFLAGS += -DCONFIG_HI_FPGA_JPEG_VERSION

#=====================================================================================
#the platform version
#=====================================================================================
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3796mv200 hi3798mv200_a),)
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

ifeq ($(CFG_HI_JPEG_SOFT_ENCODE_SUPPORT),y)
CFLAGS += -DCONFIG_GFX_JPGE_SOFT_ENC_ENABLE
endif

ifdef CFG_HI_STATIC_LIB_ONLY
#directfb needed
CFLAGS += -fPIC
endif

#=====================================================================================
#use signal£¬not use mutex
#=====================================================================================
#CFLAGS += -DCONFIG_JPEG_USE_KERNEL_SIGNAL

#=====================================================================================
#the jpge enc var
#=====================================================================================
ifneq ($(findstring $(CFG_HI_ENC_CFLAGS), CONFIG_GFX_JPGE_ENC_ENABLE),)
CFLAGS += -D$(CFG_HI_ENC_CFLAGS)
endif

#=====================================================================================
#the chip env version
#=====================================================================================
LIBNAME :=
LIBPATH := -L$(COMMON_DIR)/api -L$(MSP_DIR)/api
DEPEND_LIB := $(LIBPATH) $(LIBNAME)

#===============================================================================
# whether debug and add googlefunction
#===============================================================================
#CFLAGS += -DCONFIG_JPEG_DEBUG_INFO
#CFLAGS += -DCONFIG_JPEG_ADD_GOOGLEFUNCTION

#===============================================================================
# complie
#===============================================================================
CPPFLAGS = 
LDFLAGS  =


#===============================================================================
#compile encode lib src
#===============================================================================
LIB_ENC_SOURCES = $(SRC_9A)/jcapimin.c   $(SRC_9A)/jcapistd.c   $(SRC_9A)/jcarith.c  \
	          $(SRC_9A)/jccoefct.c   $(SRC_9A)/jccolor.c    $(SRC_9A)/jcdctmgr.c \
	          $(SRC_9A)/jchuff.c     $(SRC_9A)/jcinit.c     $(SRC_9A)/jcmainct.c \
		  $(SRC_9A)/jcmarker.c   $(SRC_9A)/jcmaster.c   $(SRC_9A)/jcparam.c  \
		  $(SRC_9A)/jcprepct.c   $(SRC_9A)/jcsample.c   $(SRC_9A)/jctrans.c  \
		  $(SRC_9A)/jfdctflt.c   $(SRC_9A)/jfdctfst.c   $(SRC_9A)/jfdctint.c

ifneq ($(findstring $(CFG_HI_ENC_CFLAGS), CONFIG_GFX_JPGE_ENC_ENABLE),)
LIB_ENC_SOURCES += $(SRC_HARD_ENC)/jpge_henc_api.c
endif

#===============================================================================
#compile decode lib src
#===============================================================================
LIB_DEC_SOURCES = $(SRC_9A)/jdapimin.c   $(SRC_9A)/jdapistd.c  $(SRC_9A)/jdarith.c  \
		  $(SRC_9A)/jdatadst.c   $(SRC_9A)/jdatasrc.c  $(SRC_9A)/jdcoefct.c \
		  $(SRC_9A)/jdcolor.c    $(SRC_9A)/jddctmgr.c  $(SRC_9A)/jdhuff.c   \
		  $(SRC_9A)/jdinput.c    $(SRC_9A)/jdmainct.c  $(SRC_9A)/jdmarker.c \
		  $(SRC_9A)/jdmaster.c   $(SRC_9A)/jdmerge.c   $(SRC_9A)/jdpostct.c \
		  $(SRC_9A)/jdsample.c   $(SRC_9A)/jdtrans.c   $(SRC_9A)/jidctflt.c \
		  $(SRC_9A)/jidctfst.c   $(SRC_9A)/jidctint.c  $(SRC_9A)/jquant1.c  \
	          $(SRC_9A)/jquant2.c    $(SRC_9A)/jdcolor_userbuf.c

LIB_DEC_SOURCES += $(SRC_9A)/transupp.c

LIB_DEC_SOURCES += $(SRC_HARD)/hi_jpeg_hdec_api.c  $(SRC_HARD)/jpeg_hdec_parse.c      \
                   $(SRC_HARD)/jpeg_hdec_api.c     $(SRC_HARD)/jpeg_hdec_mem.c        \
                   $(SRC_HARD)/jpeg_hdec_rwreg.c   $(SRC_HARD)/jpeg_hdec_setpara.c    \
                   $(SRC_HARD)/jpeg_hdec_suspend.c $(SRC_HARD)/jpeg_hdec_table.c      \
                   $(SRC_HARD)/jpeg_hdec_csc.c     $(SRC_HARD)/jpeg_hdec_sentstream.c \
                   $(SRC_HARD)/hi_jpeg_hdec_test.c $(SRC_HARD)/jpeg_hdec_support.c    \
                   $(SRC_HARD)/jpeg_hdec_proc.c    $(SRC_HARD)/jpeg_hdec_savestream.c

#===============================================================================
#compile encode and decode lib src
#===============================================================================
LIB_COM_SOURCES = $(SRC_9A)/jaricom.c  $(SRC_9A)/jcomapi.c \
		  $(SRC_9A)/jerror.c   $(SRC_9A)/jutils.c  \
	          $(SRC_9A)/jmemmgr.c
#===============================================================================
#compile lib need mem src
#===============================================================================
LIB_SYSDEPMEMSRC = $(SRC_9A)/jmemnobs.c

#===============================================================================
#compile lib need simd src
#===============================================================================
#LIB_SIMD_SRC = $(SRC_SIMD)/jsimd_arm_neon.S    \
               $(SRC_SIMD)/jsimd_neon.c

#===============================================================================
#compile lib need all src
#===============================================================================
ifneq ($(CFG_HI_JPEG_SOFT_ENCODE_SUPPORT),y)
SOURCES = $(LIB_DEC_SOURCES) $(LIB_COM_SOURCES) $(LIB_SYSDEPMEMSRC) $(LIB_SIMD_SRC)
else
SOURCES = $(LIB_ENC_SOURCES) $(LIB_DEC_SOURCES) $(LIB_COM_SOURCES) $(LIB_SYSDEPMEMSRC) $(LIB_SIMD_SRC)
endif

SRCS := $(SOURCES)

#===============================================================================
#compile lib need include
#===============================================================================
INCLUDES = jpeglib.h jerror.h   jmorecfg.h  jconfig.h  transupp.h  \
	   jdct.h    jinclude.h jmemsys.h   jpegint.h  jversion.h

#===============================================================================
#compile
#===============================================================================
LIB_NAME := libjpeg9a

HEADER_FILES := $(MSP_DIR)/api/jpeg/inc/inc_soft/jpeglib.h
HEADER_FILES += $(MSP_DIR)/api/jpeg/inc/inc_soft/jmorecfg.h
HEADER_FILES += $(MSP_DIR)/api/jpeg/inc/inc_soft/jerror.h
HEADER_FILES += $(MSP_DIR)/api/jpeg/inc/inc_soft/jconfig.h
HEADER_FILES += $(MSP_DIR)/api/jpeg/inc/inc_hard/hi_jpeg_api.h

include $(SDK_DIR)/hi_rules.mak
