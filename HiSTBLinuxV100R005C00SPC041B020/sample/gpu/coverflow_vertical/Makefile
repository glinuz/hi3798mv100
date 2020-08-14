#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS  :=  -I$(SAMPLE_DIR)/gpu/common/inc \
            -I$(SAMPLE_DIR)/gpu/common/egl/inc \
            -I$(SAMPLE_DIR)/gpu/common/key/inc \
            -I$(SAMPLE_DIR)/gpu/common/texture/inc

ifeq ($(CFG_HI_EGL_TYPE),fb)
CFLAGS += -DHI3D_DEBUG -DLINUX -DEGL_API_FB 
ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
CFLAGS += -DEGL_FBDEV
endif
endif

DEPEND_LIBS += $(HI_LIBS)

ifeq ($(CFG_HI_GPU_MALI400_SUPPORT),y)
DEPEND_LIBS += -lMali
else ifeq ($(CFG_HI_GPU_MALI450_SUPPORT),y)
DEPEND_LIBS += -lMali
else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
DEPEND_LIBS += -lmali -lstdc++
endif

DEPEND_LIBS += -lEGL -lGLESv1_CM

GPU_COMMON_OBJS :=  ../common/egl/src/hi_egl.o \
                    ../common/egl/src/egl_native.o \
                    ../common/texture/src/hi_texture.o \
                    ../common/key/src/hi_engine_key.o \
                    ../common/key/src/hi_input_dfb.o \
                    ../common/key/src/hi_ir_remote.o

SAMPLE_IMAGES := sample_coverflow_vertical

LOCAL_OBJS := coverflow_texture.o hi_coverflow.o $(GPU_COMMON_OBJS)

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

include $(SAMPLE_DIR)/hi_sample_rules.mak
