Purpose:   This sample demonstrates the HDMI IN->HDMI OUT function.
Commmand:  sample_hdmi_rx [vo_format]
Parameter: vo_format: video display format, range: 1080P_60|1080P_50|1080i_60|1080i_50|720P_60, optional, default as 720P_50
Example:   sample_hdmi_rx 720P_50
Attention: 
HI3798 Series IC
1.sample needs to be loaded before running hi_hdmirx.ko.
HI3716CV200 Series IC:
1.insmod pub/kmod/hi_sil9293.ko
2.Make sure the HIMI-IN and SIL9293 are connected correctly.
3.audio revise£º
	source\msp\drv\aiao\aiao_v1_1\ao\drv_ao.c
      HI_S32 AOGetSndDefOpenAttr(HI_UNF_SND_ATTR_S *pstSndAttr)
      {
      ...
        #if 0   /*add*/                                       
      	#if defined(HI_UNF_SND_OUTPUTPORT_I2S0_SUPPORT)
      ...
      #endif                
      #endif   /*add*/                 
          return HI_SUCCESS;
      }
	sample\common\hi_adp_mpi.c 
			HI_S32 HIADP_AVPlay_SetAdecAttr(HI_HANDLE hAvplay, HI_U32 enADecType, HI_HA_DECODEMODE_E enMode, HI_S32 isCoreOnly)
      { 
      	...
       /* set pcm wav format here base on pcm file */
        stWavFormat.nChannels = 2;    /*1->2*/
        stWavFormat.nSamplesPerSec = 48000;
        stWavFormat.wBitsPerSample = 16;
        ...
       }
4.Refer to the attention part of sample/vi/readme.txt.