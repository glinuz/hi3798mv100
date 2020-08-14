Purpose: 		to test CGMS-A function.
Command: 		sample_cgms vfile vtype afile atype [fmt]
Parameters: vfile: video es file path.
			vtype: video encoding type(h264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sors)
			afile: audio es file path.
			atype: audio encoding type(aac/mp3/dts/dra/mlp/pcm/ddp)
			fmt  : display output format  (1080p_50/1080p_60/1080i_60/1080i_50/720p_50/720p_60/480p_60/576p_50/ntsc/pal_n)
Example:	sample_cgms ./h264.es h264 ./mp3.es mp3  720p_50
Attention:	if only play audio£¬vfile and vtype should be null£¬if only play video, afile and atype should be null
Support format:
HI_UNF_ENC_FMT_1080i_50
HI_UNF_ENC_FMT_1080i_60
HI_UNF_ENC_FMT_720P_50
HI_UNF_ENC_FMT_720P_60
HI_UNF_ENC_FMT_480P_60
HI_UNF_ENC_FMT_576P_50
HI_UNF_ENC_FMT_NTSC
HI_UNF_ENC_FMT_PAL_N

How to test:
cmd   display 	interface   	mode
a1    SD    	CVBS        	HI_UNF_DISP_CGMS_MODE_COPY_FREELY
a2 	  SD    	CVBS   	  		HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE
a3    SD    	CVBS   			HI_UNF_DISP_CGMS_MODE_COPY_ONCE
a4    SD    	CVBS   			HI_UNF_DISP_CGMS_MODE_COPY_NEVER
a0    SD    	CVBS   			turn off CGMS-A
b1    HD    	YPBPR  			HI_UNF_DISP_CGMS_MODE_COPY_FREELY
b2 	  HD    	YPBPR  			HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE
b3    HD    	YPBPR  			HI_UNF_DISP_CGMS_MODE_COPY_ONCE
b4    HD    	YPBPR  			HI_UNF_DISP_CGMS_MODE_COPY_NEVER
b0    HD    	YPBPR  			turn off CGMS-A