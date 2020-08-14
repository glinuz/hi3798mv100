功能描述：展示本地es流播放功能
命令格式：sample_esplay vfile vtype afile atype
参数说明：vfile: 视频文件路径
          vtype: 视频协议类型，可选h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor
          afile: 音频文件路径，如果只播视频
          atype：音频协议类型，可选aac/mp3/dts/dra/mlp/pcm/ddp
命令参考：sample_esplay ./h264.es h264 ./mp3.es mp3    
注意事项：如果只播音频，vfile和vtype需要置为null，如果只播视频，afile和atype需要置为null