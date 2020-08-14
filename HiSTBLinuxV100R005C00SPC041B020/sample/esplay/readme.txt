Purpose: 		to demonstrate the playing of element stream (ES) files.
Command: 		sample_esplay vfile vtype afile atype
Parameters: vfile: video es file path.
						vtype: video encoding type(h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sors)
						afile: audio es file path.
						atype: audio encoding type(aac/mp3/dts/dra/mlp/pcm/ddp)
Example:		sample_esplay ./h264.es h264 ./mp3.es mp3
Attention:	if only play audio£¬vfile and vtype should be null£¬if only play video, afile and atype should be null
