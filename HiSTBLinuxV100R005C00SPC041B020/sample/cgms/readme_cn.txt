功能描述：测试CGMS-A功能
命令格式：sample_cgms vfile vtype afile atype [fmt]
参数说明：vfile: 视频文件路径
          vtype: 视频协议类型，可选h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor
          afile: 音频文件路径，如果只播视频
          atype：音频协议类型，可选aac/mp3/dts/dra/mlp/pcm/ddp
	  fmt  : 显示输出制式  可选1080p_50/1080p_60/1080i_60/1080i_50/720p_50/720p_60/480p_60/576p_50/ntsc/pal_n
命令参考：sample_cgms ./h264.es h264 ./mp3.es mp3  720p_50
注意事项：如果只播音频，vfile和vtype需要置为null。
          如果只播视频，afile和atype需要置为null。
		  fmt未设置情况下，默认使用720p_50输出。
支持的制式:
HI_DRV_DISP_FMT_1080i_50
HI_DRV_DISP_FMT_1080i_60
HI_DRV_DISP_FMT_720P_50
HI_DRV_DISP_FMT_720P_60
HI_DRV_DISP_FMT_480P_60

HI_DRV_DISP_FMT_576P_50
HI_DRV_DISP_FMT_NTSC
HI_DRV_DISP_FMT_PAL_N
测试方法：
命令  高标清 接口   模式
a1    标清   CVBS   HI_UNF_DISP_CGMS_MODE_COPY_FREELY
a2    标清   CVBS   HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE
a3    标清   CVBS   HI_UNF_DISP_CGMS_MODE_COPY_ONCE
a4    标清   CVBS   HI_UNF_DISP_CGMS_MODE_COPY_NEVER
a0    标清   CVBS   关闭CGMS功能
b1    高清   YPBPR  HI_UNF_DISP_CGMS_MODE_COPY_FREELY
b2    高清   YPBPR  HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE
b3    高清   YPBPR  HI_UNF_DISP_CGMS_MODE_COPY_ONCE
b4    高清   YPBPR  HI_UNF_DISP_CGMS_MODE_COPY_NEVER
b0    高清   YPBPR  关闭CGMS功能