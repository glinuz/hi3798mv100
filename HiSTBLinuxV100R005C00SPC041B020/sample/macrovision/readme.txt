Purpose: 		to test Macrovision function.
Command: 		sample_macrovision vfile vtype afile atype [fmt]
Parameters: vfile: video es file path.
			vtype: video encoding type(h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sors)
			afile: audio es file path.
			atype: audio encoding type(aac/mp3/dts/dra/mlp/pcm/ddp)
			fmt  : display output format  (1080p_50/1080p_60/1080i_60/1080i_50/720p_50/720p_60/480p_60/576p_50/ntsc/pal_n)
Example:	sample_macrovision ./h264.es h264 ./mp3.es mp3  720p_50
Attention:	if only play audio￡?vfile and vtype should be null￡?if only play video, afile and atype should be null
Support format:
HI_UNF_ENC_FMT_NTSC
HI_UNF_ENC_FMT_PAL

How to test:
cmd   display 	interface    format	             mode
a     标清       CVBS        HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE0
b     标清       CVBS        HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE1                 
c     标清       CVBS        HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE2
d     标清       CVBS        HI_UNF_ENC_FMT_PAL      HI_UNF_DISP_MACROVISION_MODE_TYPE3                
e     标清       CVBS        HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE0
d     标清       CVBS        HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE1               
g     标清       CVBS        HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE2             
h     标清       CVBS        HI_UNF_ENC_FMT_NTSC     HI_UNF_DISP_MACROVISION_MODE_TYPE3 