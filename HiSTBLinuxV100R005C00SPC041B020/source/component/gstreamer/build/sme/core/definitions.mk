###########################################################
#Copyright (C) 2016-2026 2012 Euler SME.
# definitions.mk
###########################################################

###########################################################
## Retrieve the directory of the current makefile
###########################################################

# Figure out where we are.
define my-dir
$(strip \
  $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
  $(if $(filter $(CLEAR_VARS),$(LOCAL_MODULE_MAKEFILE)), \
    $(error LOCAL_PATH must be set before including $$(CLEAR_VARS)) \
   , \
    $(patsubst %/,%,$(dir $(LOCAL_MODULE_MAKEFILE))) \
   ) \
 )
endef


###########################################################
## Retrieve a list of all makefiles immediately below your directory
###########################################################

define all-subdir-makefiles
$(call all-makefiles-under,$(call my-dir))
endef

###########################################################
## Look in the named list of directories for makefiles,
## relative to the current directory.
###########################################################

# $(1): List of directories to look for under this directory
define all-named-subdir-makefiles
$(wildcard $(addsuffix /*.mk, $(addprefix $(call my-dir)/,$(1))))
endef

###########################################################
## Find all of the c files under the named directories.
## Meant to be used like:
##    SRC_FILES := $(call all-c-files-under,src tests)
###########################################################

define all-c-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find -L $(1) -name "*.c" -and -not -name ".*") \
 )
endef

###########################################################
## Find all of the c files from here.  Meant to be used like:
##    SRC_FILES := $(call all-subdir-c-files)
###########################################################

define all-subdir-c-files
$(call all-c-files-under,.)
endef

###########################################################
## Find all of the cpp files under the named directories.
## Meant to be used like:
##    SRC_FILES := $(call all-cpp-files-under,src tests)
###########################################################

define all-cpp-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find -L $(1) -name "*.cpp" -and -not -name ".*") \
 )
endef

###########################################################
## Find all of the cpp files from here.  Meant to be used like:
##    SRC_FILES := $(call all-subdir-cpp-files)
###########################################################

define all-subdir-cpp-files
$(call all-cpp-files-under,.)
endef

###########################################################
## Function we can evaluate to introduce a dynamic dependency
###########################################################

define add-dependency
$(1): $(2)
endef

###########################################################
## Set up the dependencies for a prebuilt target
##  $(call add-prebuilt-file, srcfile, [targetclass])
###########################################################

define add-prebuilt-file
    $(eval $(include-prebuilt))
endef

define include-prebuilt
    include $$(CLEAR_VARS)
    LOCAL_SRC_FILES := $(1)
    LOCAL_BUILT_MODULE_STEM := $(1)
    LOCAL_MODULE_SUFFIX := $$(suffix $(1))
    LOCAL_MODULE := $$(basename $(1))
    LOCAL_MODULE_CLASS := $(2)
    include $$(BUILD_PREBUILT)
endef

###########################################################
## do multiple prebuilts
##  $(call target class, files ...)
###########################################################

define add-prebuilt-files
    $(foreach f,$(2),$(call add-prebuilt-file,$f,$(1)))
endef


###########################################################
## Convert "path/to/libXXX.so" to "-lXXX".
## Any "path/to/libXXX.a" elements pass through unchanged.
###########################################################

define normalize-libraries
$(foreach so,$(filter %.so,$(1)),-l$(patsubst lib%.so,%,$(notdir $(so))))\
$(filter-out %.so,$(1))
endef

# TODO: change users to call the common version.
define normalize-host-libraries
$(call normalize-libraries,$(1))
endef

define normalize-target-libraries
$(call normalize-libraries,$(1))
endef


###########################################################
## Returns true if $(1) and $(2) are equal.  Returns
## the empty string if they are not equal.
###########################################################
define streq
$(strip $(if $(strip $(1)),\
  $(if $(strip $(2)),\
    $(if $(filter-out __,_$(subst $(strip $(1)),,$(strip $(2)))$(subst $(strip $(2)),,$(strip $(1)))_),,true), \
    ),\
  $(if $(strip $(2)),\
    ,\
    true)\
 ))
endef

###########################################################
## Convert "a b c" into "a:b:c"
###########################################################
define normalize-path-list
$(subst $(space),:,$(strip $(1)))
endef

###########################################################
## Convert "a=b c= d e = f" into "a=b c=d e=f"
##
## $(1): list to collapse
## $(2): if set, separator word; usually "=", ":", or ":="
##       Defaults to "=" if not set.
###########################################################

define collapse-pairs
$(eval _cpSEP := $(strip $(if $(2),$(2),=)))\
$(subst $(space)$(_cpSEP)$(space),$(_cpSEP),$(strip \
    $(subst $(_cpSEP), $(_cpSEP) ,$(1))))
endef

###########################################################
## Append a leaf to a base path.  Properly deals with
## base paths ending in /.
##
## $(1): base path
## $(2): leaf path
###########################################################

define append-path
$(subst //,/,$(1)/$(2))
endef


define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find -L $(1) -name "*.*") \
 )
endef

##########################################################
##
## define compile functions
##
##########################################################

define compile_c_o

endef


define compile_o_to_shared

endef


define compile_o_to_static

endef

define compile_o_to_bin

endef


define host_compile_c_o

endef


define host_compile_o_to_shared

endef


define host_compile_o_to_static

endef

define host_compile_o_to_bin

endef



###########################################################
## Dump the variables that are associated with targets
###########################################################

define dump-module-variables
@echo check modules=$@
@echo LOCAL_CC=$(PRIVATE_CC)
@echo LOCAL_CXX=$(PRIVATE_CXX)
@echo LOCAL_CFLAGS=$(PRIVATE_CFLAGS)
@echo LOCAL_CPPFLAGS=$(PRIVATE_CPPFLAGS)
@echo LOCAL_LDFLAGS=$(PRIVATE_LDFLAGS)
@echo LOCAL_MODULE=$(PRIVATE_LOCAL_MODULE)
@echo LOCAL_MODULE_SUFFIX=$(PRIVATE_MODULE_SUFFIX)
@echo LOCAL_AR=$(PRIVATE_AR)
@echo LOCAL_ARFLAGS=$(PRIVATE_ARFLAGS)
@echo obj_prefix=$(PRIVATE_OBJ_DIR)
@echo bin_prefix=$(PRIVATE_BIN_DIR)
@echo LOCAL_C_INCLUDES=$(PRIVATE_C_INCLUDES)
@echo LOCAL_PATH=$(PRIVATE_LOCAL_PATH)
@echo target_all_objs=$(PRIVATE_ALL_OBJS)
@echo target_all_source=$(PRIVATE_ALL_SOURCE)
@echo built_shared_libraries=$(PRIVATE_SHARED_LIBRARIES)
@echo built_static_libraries=$(PRIVATE_STATIC_LIBRARIES)
@echo all_required_modules=$(PRIVATE_REQUIRED_MODULES)
@echo TARGET_LD_SHARED_DIRS=$(PRIVATE_TARGET_LD_SHARED_DIRS)
@echo TARGET_LD_STATIC_DIRS=$(PRIVATE_TARGET_LD_STATIC_DIRS)
@echo LOCAL_LDLIBS=$(PRIVATE_LDLIBS)
endef

##########################################################
##
## define copy funcs
##
##########################################################
