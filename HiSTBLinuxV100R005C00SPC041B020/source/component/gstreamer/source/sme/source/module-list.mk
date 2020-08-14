############################## begin sme self-RD compile #########################
#osal
ifeq ($(CFG_ENABLE_HWSECURE), y)
include $(SMEDIR)/source/osal/src/linux_generic/hwsecurec/libsme_osal.mk
else
include $(SMEDIR)/source/osal/src/linux_generic/libsme_osal.mk
endif

#common
include $(SMEDIR)/source/common/libsme_common.mk

ifneq ($(ENV_SME_VERSION_TYPE), rls)
ifeq ($(CFG_ENABLE_BENCHMARK), y)
-include $(SMEDIR)/source/plugins/common/libsme_gst_benchmark.mk
endif
endif

-include $(SMEDIR)/source/plugins/common/libsme_gst_ptscalibrate.mk

#plugins/common
include $(SMEDIR)/source/plugins/common/libsme_gst_common.mk

#plugins/source
ifeq ($(CFG_ENABLE_SME_HTTPSRC_SOUP), y)
include $(SMEDIR)/source/plugins/source/httpsrc_soup/libsme_gstplg_httpsrc_soup.mk
endif

#plugins/demux
include $(SMEDIR)/source/plugins/demux/demux_impl_ffmpeg/libsme_demux_impl_ffmpeg.mk
include $(SMEDIR)/source/plugins/demux/demux_ffmpeg/libsme_gstplg_demux_ffmpeg.mk

#use ffmpeg protocol and demux
ifeq ($(CFG_ENABLE_FFMPEG_PROTOCOL), y)
include $(SMEDIR)/source/plugins/source/ff_pro_src/libsme_gstplg_ffpro_src.mk
include $(SMEDIR)/source/plugins/demux/demux_ffmpeg_pro/libsme_gstplg_demux_ffpro.mk
endif

ifeq ($(CFG_ENABLE_DVB),y)
-include $(SMEDIR)/source/plugins/demux/demux_dvb2/libsme_gstplg_tsdmx.mk
endif

#plugins/decoder
ifeq ($(CFG_ENABLE_ADEC_FFMPEG), y)
-include $(SMEDIR)/source/plugins/decoder/audio/dec_ffmpeg/libsme_gstplg_adec_ffmpeg.mk
endif

#sink
include $(SMEDIR)/source/plugins/sink/audio/basesink2/libsme_gst_audiobasesink2.mk
ifeq ($(CFG_ENABLE_ASINK_HISI_UNF), y)
-include $(SMEDIR)/source/plugins/sink/audio/sink_linux_hiunf/libsme_gstplg_asink_hiunf.mk
endif

ifneq ($(CFG_ENABLE_ALSASINK), y)
ifeq ($(CFG_ENABLE_ASINK_AF), y)
-include $(SMEDIR)/source/plugins/sink/audio/sink_tvos2.0_af/libsme_gstplg_asink_af.mk
endif

ifeq ($(CFG_ENABLE_ASINK_HISI_HAL), y)
-include $(SMEDIR)/source/plugins/sink/audio/sink_hisi_hal/libsme_gstplg_asink_hihal.mk
endif
endif

ifeq ($(CFG_ENABLE_VSINKHIHAL), y)
-include $(SMEDIR)/source/plugins/sink/video/sink_hisi_hal/libsme_gstplg_vsink_hihal.mk
endif

ifeq ($(CFG_ENABLE_HISI_MPI_VSINK), y)
-include $(SMEDIR)/source/plugins/sink/video/sink_linux_himpi/libsme_gstplg_vsink_himpi.mk
endif

ifeq ($(CFG_ENABLE_DVBPLAYBIN),y)
-include $(SMEDIR)/source/plugins/playbin/dvbplaybin/libsme_gstplg_dvbplaybin.mk
endif

#player
include $(SMEDIR)/source/player/libsme_player.mk
include $(SMEDIR)/source/dvbplayer/libsme_dvbplayer.mk
############################## end sme self-RD compile #########################
