#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===================================================================
#prepare resource
#==================================================================
CFLAGS := $(subst -Werror,,$(CFG_HI_SAMPLE_CFLAGS))

ifeq (${CFG_HI_DIRECTFB_CA_SUPPORT},y)
CFLAGS += -DHI_ADVCA_FUNCTION_RELEASE
endif

ifeq ($(CFG_HI_DIRECTFB_SUPPORT),y)
CFLAGS += -DHI_DIRECTFB_SUPPORT
endif

CFLAGS +=   -I$(HI_INCLUDE_DIR)                 \
            -I$(HI_INCLUDE_DIR)/directfb        \
            -I$(SAMPLE_DIR)/common              \
            -I${DFB_EXAMPLE}                    \
            -I${DFB_EXAMPLE}/../include         


CFLAGS += -DFONT=\"./../share/directfb-examples/fonts/decker.ttf\"
CFLAGS += -DDATADIR=\"./../share/directfb-examples\"
CFLAGS += -DVERSION=\"1.6.0\"

DFB_LIB := -ldirectfb -ldirect -lfusion -lstdc++ -lrt

#===================================================================
#add *.c files
#==================================================================
SAMPLE_IMAGES := df_andi            \
                 df_blit_capa       \
                 df_bltload         \
                 df_cursor          \
                 df_dok             \
                 df_fire            \
                 df_flip            \
                 df_flip_capa       \
                 df_input           \
                 df_layer_surface   \
                 df_logo_to_app     \
                 df_multi_surface   \
                 df_neo             \
                 df_netload         \
                 df_palette         \
                 df_particle        \
                 df_porter          \
                 df_stress          \
                 df_window          \
                 df_window_surface  \
                 pss

SAMPLE_IMAGES += df_matrix    \
                 df_matrix_water

#SAMPLE_TMP_IMAGES := $(filter-out %df_layer.c, $(SAMPLE_IMAGES))
#SAMPLE_IMAGES := $(SAMPLE_TMP_IMAGES)

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o) hi_adp_gfx.o

DEPEND_LIBS := $(HI_LIBS) $(DFB_LIB)

include $(SAMPLE_DIR)/hi_sample_rules.mak
