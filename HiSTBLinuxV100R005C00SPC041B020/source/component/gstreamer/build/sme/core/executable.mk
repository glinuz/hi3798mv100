###########################################################
#Copyright (C) 2015-2020 Hisilicon
#executable.mk
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

ifeq ($(LOCAL_CC),)
LOCAL_CC := $(TARGET_CC)
endif

ifeq ($(LOCAL_CXX),)
LOCAL_CXX := $(TARGET_CXX)
endif

ifeq ($(LOCAL_MODULE_SUFFIX),)
LOCAL_MODULE_SUFFIX := $(TARGET_EXECUTABLE_SUFFIX)
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

#$(info all module checking :$(ALL_MODULES))

.PHONY:$(LOCAL_MODULE)


#output prefix set
mediaos_dir := $(shell cd $(MEDIAOSDIR);pwd)
#$(info mediaosdir=$(mediaos_dir))

ifeq ($(LOCAL_OBJ_EXTRADIR_NAME),)
obj_prefix :=$(TARGET_OUT_OBJ_DIR)/$(patsubst $(mediaos_dir)/%,%,$(LOCAL_PATH))
else
obj_prefix :=$(TARGET_OUT_OBJ_DIR)/$(patsubst $(mediaos_dir)/%,$(LOCAL_OBJ_EXTRADIR_NAME)/%,$(LOCAL_PATH))
endif

#obj_prefix :=$(TARGET_OUT_OBJ_DIR)/$(LOCAL_MODULE).EXE
ifeq ($(LOCAL_MODULE_PATH),)
bin_prefix :=$(TARGET_OUT_BIN_DIR)
else
bin_prefix :=$(LOCAL_MODULE_PATH)
endif

so_suffix :=$(TARGET_SHAREDLIB_SUFFIX)
a_suffix :=$(TARGET_STATICLIB_SUFFIX)
my_sharedlib_prefix :=$(TARGET_GLOBAL_LD_SHARED_DIRS)
my_staticlib_prefix :=$(TARGET_GLOBAL_LD_STATIC_DIRS)


#$(info obj_prefix ,$(obj_prefix))

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
	@echo "<<< compile c objs $@:"
	mkdir -p $(dir $@)
	$(NS)$(PRIVATE_CC) \
	$(addprefix -I , $(PRIVATE_C_INCLUDES)) \
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
$(target_cpp_objs):$(obj_prefix)/%.o:$(TOP_DIR)$(LOCAL_PATH)/%.cpp
	@echo "<<< compile cpp objs $@:"
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
$(target_s_objs):$(obj_prefix)/%.o:$(TOP_DIR)$(LOCAL_PATH)/%.S
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

target_all_objs := $(target_c_objs) $(target_cpp_objs) $(target_s_objs) $(LOCAL_PREBUILT_OBJ_FILES)
target_all_source := $(target_c_source) $(target_cpp_source) $(target_s_source)

#check all source and objs for module
#$(info checking source:$(target_all_source))
#$(info checking objs:$(target_all_objs))


#########################################################
##
## link libraries
##
#########################################################

built_shared_libraries := \
    $(addprefix $(my_sharedlib_prefix)/, \
      $(addsuffix $(so_suffix), \
        $(LOCAL_SHARED_LIBRARIES)))

built_static_libraries := \
    $(addprefix $(my_staticlib_prefix)/, \
      $(addsuffix $(a_suffix), \
        $(LOCAL_STATIC_LIBRARIES)))

#########################################################
## required modules
#########################################################
#all_required_modules := $(LOCAL_REQUIRED_MODULES)
all_required_modules := $(foreach req_tmp, $(LOCAL_REQUIRED_MODULES), $(req_tmp)-deps)


#define LOCAL_INTERMEDIATE_TARGETS for modules
LOCAL_INTERMEDIATE_TARGETS := $(target_all_objs) $(target_all_source) \
                              $(LOCAL_MODULE)-t-bin-check \
                              $(LOCAL_MODULE)-t-bin-clean \
                              $(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX) \
		  $(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)-deps

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
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_BIN_DIR := $(bin_prefix)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_C_INCLUDES := $(LOCAL_C_INCLUDES)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LOCAL_PATH := $(LOCAL_PATH)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_OBJS := $(target_all_objs)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_SOURCE := $(target_all_source)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_SHARED_LIBRARIES := $(built_shared_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_STATIC_LIBRARIES := $(built_static_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_TARGET_LD_SHARED_DIRS := $(TARGET_GLOBAL_LD_SHARED_DIRS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_TARGET_LD_STATIC_DIRS := $(TARGET_GLOBAL_LD_STATIC_DIRS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDLIBS := $(LOCAL_LDLIBS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_REQUIRED_MODULES := $(all_required_modules)

ifeq ($(CFG_ENABLE_NDK), y)
PRIVATE_STL_LDFLAGS := -L$(TOOLCHAIN_INSTALL_DIR)/arm-linux-androideabi/lib -lsupc++
else
PRIVATE_STL_LDFLAGS :=
endif

all_libraries := $(built_static_libraries) $(built_shared_libraries)

# module final target
$(LOCAL_MODULE)-deps:$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)-deps
$(LOCAL_MODULE):$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)
$(LOCAL_MODULE)-check:$(LOCAL_MODULE)-t-bin-check
$(LOCAL_MODULE)-clean:$(LOCAL_MODULE)-t-bin-clean
$(LOCAL_MODULE)-deps-clean:$(LOCAL_MODULE)-clean
$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-t-bin-clean
$(LOCAL_MODULE)-cfg-clean:$(LOCAL_MODULE)-t-bin-clean

$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)-deps:$(all_required_modules) $(target_all_objs) $(all_libraries)
	@echo "<<< compile bin $(bin_prefix)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX):"
	@mkdir -p $(dir $(bin_prefix)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX))
	$(NS)$(PRIVATE_CXX) \
	-L$(PRIVATE_TARGET_LD_STATIC_DIRS) \
	-Wl,-rpath-link=$(PRIVATE_TARGET_LD_SHARED_DIRS) \
	-o  $(bin_prefix)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX) \
	$(PRIVATE_ALL_OBJS) \
	-Wl,--whole-archive \
	$(call normalize-target-libraries,$(PRIVATE_STATIC_LIBRARIES)) \
	 -Wl,--no-whole-archive \
	-L$(PRIVATE_TARGET_LD_SHARED_DIRS) \
	$(call normalize-target-libraries,$(PRIVATE_SHARED_LIBRARIES)) \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_LDLIBS) $(PRIVATE_STL_LDFLAGS) -Wl,-pie

$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX):$(target_all_objs)
	@echo "<<< compile bin $@:"
	@mkdir -p $(dir $@)
	$(NS)$(PRIVATE_CXX) \
	-L$(PRIVATE_TARGET_LD_STATIC_DIRS) \
	-Wl,-rpath-link=$(PRIVATE_TARGET_LD_SHARED_DIRS) \
	-o $@ \
	$(PRIVATE_ALL_OBJS) \
	-Wl,--whole-archive \
	$(call normalize-target-libraries,$(PRIVATE_STATIC_LIBRARIES)) \
	 -Wl,--no-whole-archive \
	-L$(PRIVATE_TARGET_LD_SHARED_DIRS) \
	$(call normalize-target-libraries,$(PRIVATE_SHARED_LIBRARIES)) \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_LDLIBS) $(PRIVATE_STL_LDFLAGS) -Wl,-pie

$(LOCAL_MODULE)-t-bin-check:
	$(dump-module-variables)

$(LOCAL_MODULE)-t-bin-clean:
	$(NS)rm -rf $(PRIVATE_OBJ_DIR)
	$(NS)rm -rf $(PRIVATE_BIN_DIR)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX)
