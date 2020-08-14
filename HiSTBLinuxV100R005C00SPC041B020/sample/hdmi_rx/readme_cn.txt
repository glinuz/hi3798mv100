功能描述：展示HDMI输入及：HDMI IN->HDMI OUT回环功能
命令格式：sample_hdmi_rx [vo_format]
参数说明：vo_format 视频输出制式 取值范围 1080P_60|1080P_50|1080i_60|1080i_50|720P_60 可选 默认值720P_50
例    子：sample_hdmi_rx 720P_50
注意事项：
HI3798系列IC
1.sample运行前需要加载hi_hdmirx.ko。
HI3716CV200系列IC：
1.手动加载SIL9293.ko，insmod pub/kmod/hi_sil9293.ko
2.确认HDMI输入信号与SIL9293芯片已正确连接
3.audio修改：
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
4.参见sample/vi/readme_cn.txt注意事项。