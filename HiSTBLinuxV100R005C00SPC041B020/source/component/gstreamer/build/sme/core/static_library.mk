###########################################################
#Copyright (C) 2015-2020 Hisilicon
#static_library.mk
###########################################################


LOCAL_MODULE := $(strip $(LOCAL_MODULE))
ifeq ($(LOCAL_MODULE),)
  $(error $(LOCAL_PATH): LOCAL_MODULE is not defined)
endif

ifeq ($(LOCAL_CFLAGS),)
LOCAL_CFLAGS := $(TARGET_GLOBLE_CFLAGS)
endif

ifeq ($(LOCAL_LDFLAGS),)
LOCAL_LDFLAGS := $(TARGET_GLOBLE_LDFLAGS)
endif

ifeq ($(LOCAL_CPPFLAGS),)
LOCAL_CPPFLAGS := $(TARGET_GLOBLE_CPPFLAGS)
endif

ifeq ($(LOCAL_ARFLAGS),)
LOCAL_ARFLAGS := $(TARGET_GLOBLE_ARFLAGS)
endif

ifeq ($(LOCAL_CC),)
LOCAL_CC := $(TARGET_CC)
endif

ifeq ($(LOCAL_CXX),)
LOCAL_CXX := $(TARGET_CXX)
endif

ifeq ($(LOCAL_AR),)
LOCAL_AR := $(TARGET_AR)
endif

ifeq ($(LOCAL_MODULE_SUFFIX),)
LOCAL_MODULE_SUFFIX := $(TARGET_STATICLIB_SUFFIX)
endif

#LOCAL_C_INCLUDES
#LOCAL_SHARED_LIBRARIES
#LOCAL_EXPORT_C_INCLUDE_DIRS
#LOCAL_COPY_HEADERS
#LOCAL_REQUIRED_MODULES
#LOCAL_ADDITIONAL_DEPENDENCIES
#LOCAL_LDLIBS

ifneq ($(LOCAL_INTERMEDIATE_TARGETS),)
$(error LOCAL_INTERMEDIATE_TARGETS should not define in component makefiles !)
endif

#all_modules += $(LOCAL_MODULE)

ALL_MODULES +=$(LOCAL_MODULE)

.PHONY:$(LOCAL_MODULE)



#output prefix set

#output prefix set
mediaos_dir := $(shell cd $(MEDIAOSDIR);pwd)
#$(info mediaosdir=$(mediaos_dir))
obj_prefix :=$(TARGET_OUT_OBJ_DIR)/$(patsubst $(mediaos_dir)/%,%,$(LOCAL_PATH))
#obj_prefix :=$(TARGET_OUT_OBJ_DIR)/$(LOCAL_MODULE).STATIC
lib_prefix :=$(TARGET_OUT_LIB_DIR)/static


###########################################################
##
## C: compile .c source files to .o
##
###########################################################

target_c_source := $(filter %.c, $(LOCAL_SRC_FILES))


target_c_objs   := $(addprefix $(obj_prefix)/,$(target_c_source:.c=.o))


#debug,checking the source files
#$(info checking c source:$(target_c_source))


ifneq ($(strip $(target_c_objs)),)
$(target_c_objs):$(obj_prefix)/%.o:$(LOCAL_PATH)/%.c
	@mkdir -p $(dir $@)
	$(NS)$(PRIVATE_CC) \
	$(addprefix -I, $(PRIVATE_C_INCLUDES)) \
	-c $(PRIVATE_CFLAGS)  -o $@ $^

endif

###########################################################
##
## C++: compile .cpp source files to .o
##
###########################################################

target_cpp_source := $(filter %.cpp, $(LOCAL_SRC_FILES))


target_cpp_objs   := $(addprefix $(obj_prefix)/,$(target_cpp_source:.cpp=.o))


#debug,checking the source files
#$(info checking cpp source:$(target_cpp_source))


ifneq ($(strip $(target_cpp_objs)),)
$(target_cpp_objs):$(obj_prefix)/%.o:$(LOCAL_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(NS)$(PRIVATE_CXX) \
	$(addprefix -I , $(PRIVATE_C_INCLUDES)) \
	-c $(PRIVATE_CPPFLAGS)  -o $@ $^
endif

###########################################################
##
## S: compile S source files to .o
##
###########################################################

target_s_source := $(filter %.S, $(LOCAL_SRC_FILES))


target_s_objs   := $(addprefix $(obj_prefix)/,$(target_s_source:.S=.o))


#debug,checking the source files
#$(info checking S source:$(target_s_source))


ifneq ($(strip $(target_s_objs)),)
$(target_s_objs):$(obj_prefix)/%.o:$(LOCAL_PATH)/%.S
	@mkdir -p $(dir $@)
	$(NS)$(PRIVATE_CC) \
	$(addprefix -I , $(PRIVATE_C_INCLUDES)) \
	-c $(PRIVATE_CFLAGS)  -o $@ $^
endif


###########################################################
##
## To shared libs: compile .o  files to .so shared libs
##
###########################################################

target_all_objs := $(target_c_objs) $(target_cpp_objs) $(target_s_objs)
target_all_source := $(target_c_source) $(target_cpp_source) $(target_s_source)

#check all source and objs for module
#$(info checking source:$(target_all_source))
#$(info checking objs:$(target_all_objs))
#$(info LOCAL_AR,$(LOCAL_AR))
#$(info LOCAL_ARFLAGS,$(LOCAL_ARFLAGS))

#########################################################
## required modules
#########################################################
all_required_modules := $(LOCAL_REQUIRED_MODULES)

#define LOCAL_INTERMEDIATE_TARGETS for modules
LOCAL_INTERMEDIATE_TARGETS := $(target_all_objs) $(target_all_source) \
                              $(lib_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX) \
                              $(LOCAL_MODULE)-t-stlib-check \
                              $(LOCAL_MODULE)-t-stlib-clean

#$(info LOCAL_INTERMEDIATE_TARGETS ,$(LOCAL_INTERMEDIATE_TARGETS))

$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CC := $(LOCAL_CC)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CXX := $(LOCAL_CXX)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CFLAGS := $(LOCAL_CFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CPPFLAGS := $(LOCAL_CPPFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDFLAGS := $(LOCAL_LDFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LOCAL_MODULE := $(LOCAL_MODULE)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_MODULE_SUFFIX := $(LOCAL_MODULE_SUFFIX)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_AR := $(LOCAL_AR)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ARFLAGS := $(LOCAL_ARFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_OBJ_DIR := $(obj_prefix)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LIB_DIR := $(lib_prefix)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LOCAL_PATH := $(LOCAL_PATH)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_OBJS := $(target_all_objs)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_SOURCE := $(target_all_source)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_C_INCLUDES := $(LOCAL_C_INCLUDES)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_REQUIRED_MODULES := $(all_required_modules)

$(LOCAL_MODULE)-deps:$(lib_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)
$(LOCAL_MODULE):$(lib_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)
$(LOCAL_MODULE)-check:$(LOCAL_MODULE)-t-stlib-check
$(LOCAL_MODULE)-clean:$(LOCAL_MODULE)-t-stlib-clean
$(LOCAL_MODULE)-cfg-clean:$(LOCAL_MODULE)-t-stlib-clean
$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-t-stlib-clean
$(LOCAL_MODULE)-deps-clean:$(LOCAL_MODULE)-t-stlib-clean

$(lib_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX):$(all_required_modules) $(target_all_objs)
	@echo "compile static lib:$@"
	@mkdir -p $(dir $@)
	$(NS)$(PRIVATE_AR) $(PRIVATE_ARFLAGS)  $@ $(PRIVATE_ALL_OBJS)

$(LOCAL_MODULE)-t-stlib-check:
	$(dump-module-variables)

$(LOCAL_MODULE)-t-stlib-clean:
	$(NS)rm -rf $(PRIVATE_OBJ_DIR)
	$(NS)rm -rf $(PRIVATE_LIB_DIR)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX)
