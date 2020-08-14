LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_glib_all

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GLIB_NAME := glib-2.46.2
PKG_GLIB_ALL_NAME := glib_all

PKG_GLIB_ALL_TARGETS := pkg_iconv pkg_gettext pkg_ffi pkg_glib
PKG_GLIB_ALL_TARGETS_DEPS := pkg_glib-deps

$(LOCAL_MODULE)-deps:$(PKG_GLIB_ALL_TARGETS_DEPS)
$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE): $(PKG_GLIB_ALL_TARGETS)
	$(NS)touch $@

$(LOCAL_MODULE)-clean:$(foreach pkg_glib_all_targets_item, $(PKG_GLIB_ALL_TARGETS), $(pkg_glib_all_targets_item)-clean)
	$(NS)echo "begin make clean $(PKG_GLIB_ALL_NAME) ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GLIB_ALL_NAME)"

$(LOCAL_MODULE)-cfg-clean:$(foreach pkg_glib_all_targets_item, $(PKG_GLIB_ALL_TARGETS), $(pkg_glib_all_targets_item)-cfg-clean)
	$(NS)echo "begin make clean $(PKG_GLIB_ALL_NAME)-cfg ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GLIB_ALL_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(foreach pkg_glib_all_targets_item, $(PKG_GLIB_ALL_TARGETS), $(pkg_glib_all_targets_item)-prepare-clean)
	$(NS)echo "begin make clean $(PKG_GLIB_ALL_NAME)-prepare ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GLIB_ALL_NAME)-prepare ..."

#include $(BUILD_OPENSOURCE_PKG)
