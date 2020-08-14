###########################################################
#Copyright (C) 2015-2020 Hisilicon
#host_executable.mk
###########################################################


LOCAL_MODULE := $(strip $(LOCAL_MODULE))
ifeq ($(LOCAL_MODULE),)
  $(error $(LOCAL_PATH): LOCAL_MODULE is not defined)
endif

ifeq ($(LOCAL_CFLAGS),)
LOCAL_CFLAGS := $(HOST_GLOBLE_CFLAGS)
endif

ifeq ($(LOCAL_LDFLAGS),)
LOCAL_LDFLAGS := $(HOST_GLOBLE_LDFLAGS)
endif

ifeq ($(LOCAL_CPPFLAGS),)
LOCAL_CPPFLAGS := $(HOST_GLOBLE_CPPFLAGS)
endif

ifeq ($(LOCAL_CC),)
LOCAL_CC := $(HOST_CC)
endif

ifeq ($(LOCAL_CXX),)
LOCAL_CXX := $(HOST_CXX)
endif

ifeq ($(LOCAL_MODULE_SUFFIX),)
LOCAL_MODULE_SUFFIX := $(HOST_EXECUTABLE_SUFFIX)
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
obj_prefix :=$(HOST_OBJ_DIR)/$(LOCAL_MODULE).EXE
bin_prefix :=$(HOST_BIN_DIR)
so_suffix :=$(HOST_SHAREDLIB_SUFFIX)
a_suffix :=$(HOST_STATICLIB_SUFFIX)
my_sharedlib_prefix :=$(HOST_GLOBAL_LD_SHARED_DIRS)
my_staticlib_prefix :=$(HOST_GLOBAL_LD_STATIC_DIRS)

#$(info obj_prefix ,$(obj_prefix))

###########################################################
##
## C: compile .c source files to .o
##
###########################################################

host_c_source := $(filter %.c, $(LOCAL_SRC_FILES))


host_c_objs   := $(addprefix $(obj_prefix)/,$(host_c_source:.c=.o))


#debug,checking the source files
#$(info checking c source:$(host_c_source))



ifneq ($(strip $(host_c_objs)),)
$(host_c_objs):$(obj_prefix)/%.o:$(LOCAL_PATH)/%.c
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

host_cpp_source := $(filter %.cpp, $(LOCAL_SRC_FILES))


host_cpp_objs   := $(addprefix $(obj_prefix)/,$(host_cpp_source:.cpp=.o))


#debug,checking the source files
#$(info checking cpp source:$(host_cpp_source))


ifneq ($(strip $(host_cpp_objs)),)
$(host_cpp_objs):$(obj_prefix)/%.o:$(TOP_DIR)$(LOCAL_PATH)/%.cpp
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

host_s_source := $(filter %.S, $(LOCAL_SRC_FILES))


host_s_objs   := $(addprefix $(obj_prefix)/,$(host_s_source:.S=.o))


#debug,checking the source files
#$(info checking S source:$(host_s_source))


ifneq ($(strip $(host_s_objs)),)
$(host_s_objs):$(obj_prefix)/%.o:$(TOP_DIR)$(LOCAL_PATH)/%.S
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

host_all_objs := $(host_c_objs) $(host_cpp_objs) $(host_s_objs)
host_all_source := $(host_c_source) $(host_cpp_source) $(host_s_source)

#check all source and objs for module
#$(info checking source:$(host_all_source))
#$(info checking objs:$(host_all_objs))


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
all_required_modules := $(LOCAL_REQUIRED_MODULES)


#define LOCAL_INTERMEDIATE_TARGETS for modules
LOCAL_INTERMEDIATE_TARGETS := $(host_all_objs) $(host_all_source) \
                              $(LOCAL_MODULE)-h-bin-check \
                              $(LOCAL_MODULE)-h-bin-clean \
                              $(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)

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
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_OBJS := $(host_all_objs)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_SOURCE := $(host_all_source)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_SHARED_LIBRARIES := $(built_shared_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_STATIC_LIBRARIES := $(built_static_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_TARGET_LD_SHARED_DIRS := $(HOST_GLOBAL_LD_SHARED_DIRS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_TARGET_LD_STATIC_DIRS := $(HOST_GLOBAL_LD_STATIC_DIRS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDLIBS := $(LOCAL_LDLIBS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_REQUIRED_MODULES := $(all_required_modules)

all_libraries := $(built_static_libraries) $(built_shared_libraries)


# module final target
$(LOCAL_MODULE):$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)
$(LOCAL_MODULE)-check:$(LOCAL_MODULE)-h-bin-check
$(LOCAL_MODULE)-clean:$(LOCAL_MODULE)-h-bin-clean


$(bin_prefix)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX):$(all_required_modules) $(host_all_objs) $(all_libraries)
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
	$(PRIVATE_LDLIBS)

$(LOCAL_MODULE)-h-bin-check:
	$(dump-module-variables)

$(LOCAL_MODULE)-h-bin-clean:
	$(NS)rm -rf $(PRIVATE_OBJ_DIR)
	$(NS)rm -rf $(PRIVATE_BIN_DIR)/$(PRIVATE_LOCAL_MODULE)$(PRIVATE_MODULE_SUFFIX)
