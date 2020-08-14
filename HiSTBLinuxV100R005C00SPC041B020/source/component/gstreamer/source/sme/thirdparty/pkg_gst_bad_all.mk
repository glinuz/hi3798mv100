LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_gst_bad_all

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GST_BAD_NAME := gst-plugins-bad-1.2.3
PKG_GST_BAD_ALL_NAME := gst_bad_all

PKG_GST_BAD_ALL_TARGETS := pkg_gmp pkg_nettle pkg_gnutls pkg_gst_bad
PKG_GST_BAD_ALL_TARGETS_DEPS := pkg_gst_bad-deps

$(LOCAL_MODULE)-deps:$(PKG_GST_BAD_ALL_TARGETS_DEPS)
$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE): $(PKG_GST_BAD_ALL_TARGETS)
	$(NS)touch $@

$(LOCAL_MODULE)-clean:$(foreach pkg_gst_bad_all_targets_item, $(PKG_GST_BAD_ALL_TARGETS), $(pkg_gst_bad_all_targets_item)-clean)
	$(NS)echo "begin make clean $(PKG_GST_BAD_ALL_NAME) ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GST_BAD_ALL_NAME)"

$(LOCAL_MODULE)-cfg-clean:$(foreach pkg_gst_bad_all_targets_item, $(PKG_GST_BAD_ALL_TARGETS), $(pkg_gst_bad_all_targets_item)-cfg-clean)
	$(NS)echo "begin make clean $(PKG_GST_BAD_ALL_NAME)-cfg ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GST_BAD_ALL_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(foreach pkg_gst_bad_all_targets_item, $(PKG_GST_BAD_ALL_TARGETS), $(pkg_gst_bad_all_targets_item)-prepare-clean)
	$(NS)echo "begin make clean $(PKG_GST_BAD_ALL_NAME)-prepare ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_GST_BAD_ALL_NAME)-prepare ..."

#include $(BUILD_OPENSOURCE_PKG)
