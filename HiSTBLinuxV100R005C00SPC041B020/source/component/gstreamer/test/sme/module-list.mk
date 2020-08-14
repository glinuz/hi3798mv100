ifneq ($(ut), y)

ifeq ($(CFG_ENABLE_DVB),y)
ifneq ($(findstring -D__LINUX__, $(CFG_COMMON_MACRO)), -D__LINUX__)
-include $(TOPDIR)/test/sme/utils_native_surface/libsme_native_sf.mk
-include $(TOPDIR)/test/sme/test_dvb_player/test_dvb_player.mk
endif

-include $(TOPDIR)/test/sme/test_dvb_player/test_sme_dvbplayer.mk
endif

#test_sme_player
-include $(TOPDIR)/test/sme/test_sme_player/test_sme_player.mk

ifeq ($(CFG_ENABLE_NDK), y)
-include $(TOPDIR)/source/sme/api/android/jni/libsme_player_jni.mk
endif
endif

ifneq ($(demo), y)
#ut
-include $(TOPDIR)/test/sme/ut/pkg_gtest.mk
#-include $(TOPDIR)/test/sme/ut/test_sme_sdk/test_sme_sdk.mk
#-include $(TOPDIR)/test/sme/ut/test_sme_sdk/testcase/01.FunctionTest/DVB/test_sme_sdk_dvb.mk
#-include $(TOPDIR)/test/sme/ut/ut_smeplayer/ut_sme_msg/ut_sme_msg.mk
#-include $(TOPDIR)/test/sme/ut/ut_smeplayer/ut_sme_gst/ut_sme_gst.mk
#-include $(TOPDIR)/test/sme/ut/ut_smeproperty/ut_sme_property/ut_sme_property.mk
#-include $(TOPDIR)/test/sme/ut/ut_smeplugin/ut_sme_demux_ffmpeg/ut_sme_demux_ffmpeg.mk
############################## end sme self-RD compile #########################
endif
