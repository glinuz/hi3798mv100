LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_opensource_all

PKG_OPENSOURCE_ALL_NAME := pkg_opensource_all

ifeq ($(findstring y, $(CFG_ENABLE_GST_BAD_HLS)_$(CFG_ENABLE_GST_BAD_DASH)), y)
PKG_ALL_TARGETS := pkg_glib_all pkg_gst_core pkg_gst_base pkg_gst_good_all pkg_gst_bad_all pkg_gst_omx pkg_ffmpeg
PKG_ALL_TARGETS_DEPS := pkg_gst_soup-deps pkg_gst_bad-deps pkg_gst_omx-deps pkg_ffmpeg
else
ifeq ($(findstring p, $(CFG_ENABLE_GST_BAD_HLS)_$(CFG_ENABLE_GST_BAD_DASH)), p)
PKG_ALL_TARGETS := pkg_glib_all pkg_gst_core pkg_gst_base pkg_gst_good_all pkg_gmp pkg_nettle pkg_gnutls pkg_gst_omx pkg_ffmpeg
PKG_ALL_TARGETS_DEPS := pkg_gst_soup-deps pkg_gmp-deps pkg_nettle-deps pkg_gnutls-deps pkg_gst_omx-deps pkg_ffmpeg
else
PKG_ALL_TARGETS := pkg_glib_all pkg_gst_core pkg_gst_base pkg_gst_good_all pkg_gst_omx pkg_ffmpeg
PKG_ALL_TARGETS_DEPS := pkg_gst_soup-deps pkg_gst_omx-deps pkg_ffmpeg
endif
endif

$(LOCAL_MODULE)-deps:$(PKG_ALL_TARGETS_DEPS)
$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/.$(LOCAL_MODULE): $(PKG_ALL_TARGETS)
	$(NS)touch $@

$(LOCAL_MODULE)-clean:$(foreach pkg_all_targets_item, $(PKG_ALL_TARGETS), $(pkg_all_targets_item)-clean)
	$(NS)echo "begin make clean $(PKG_OPENSOURCE_ALL_NAME) ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_OPENSOURCE_ALL_NAME)"

$(LOCAL_MODULE)-cfg-clean:$(foreach pkg_all_targets_item, $(PKG_ALL_TARGETS), $(pkg_all_targets_item)-cfg-clean)
	$(NS)echo "begin make clean $(PKG_OPENSOURCE_ALL_NAME)-cfg ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_OPENSOURCE_ALL_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(foreach pkg_all_targets_item, $(PKG_ALL_TARGETS), $(pkg_all_targets_item)-prepare-clean)
	$(NS)echo "begin make clean $(PKG_OPENSOURCE_ALL_NAME)-prepare ..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/.$(LOCAL_MODULE)
	$(NS)echo "end make clean $(PKG_OPENSOURCE_ALL_NAME)-prepare ..."

#include $(BUILD_OPENSOURCE_PKG)
