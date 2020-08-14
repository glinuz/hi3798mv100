功能描述：测试Macrovison功能
命令格式：sample_macrovision vfile vtype afile atype [fmt]
参数说明：vfile: 视频文件路径
          vtype: 视频协议类型，可选h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor
          afile: 音频文件路径，如果只播视频
          atype：音频协议类型，可选aac/mp3/dts/dra/mlp/pcm/ddp
	  fmt  : 显示输出制式  可选1080p_50/1080p_60/1080i_60/1080i_50/720p_50/720p_60/480p_60/576p_50/ntsc/pal_n
命令参考：sample_macrovision ./h264.es h264 ./mp3.es mp3  720p_50
注意事项：如果只播音频，vfile和vtype需要置为null。
          如果只播视频，afile和atype需要置为null。
		  fmt未设置情况下，默认使用720p_50输出。
支持的制式:
HI_UNF_ENC_FMT_NTSC
HI_UNF_ENC_FMT_PAL
测试方法：
命令  高标清 接口   制式                    模式
a     标清   CVBS   HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE0
b     标清   CVBS   HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE1                 
c     标清   CVBS   HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE2
d     标清   CVBS   HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE3                
e     标清   CVBS   HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE0
d     标清   CVBS   HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE1               
g     标清   CVBS   HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE2             
h     标清   CVBS   HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE3             
