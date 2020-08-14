1.获取chipid
   描述：在屏幕上打印Chipid，如"chip id:xxxxxxx"。
   源文件：sample_ca_getchipid.c
   用法：./sample_ca_getchipid

2.设置jtag工作模式
	描述: 设置jtag的工作模式。
	源文件: sample_ca_setjtag.c
	用法: ./sample_ca_setjtagmode mode[0|1|2]
          mode:0-Opened 1-Password Protected 2-Closed

3.打开安全启动
   描述：打开安全启动。
   源文件：sample_ca_opensecboot.c
   用法：./sample_ca_opensecboot
          
4.ts流播放
   此demo有以下功能：
         1> 加载明文CW或加密的CWPK到芯片内部
         2> 处理加扰的ts流并输出显示
         视频数据可通过CVBS YPBPR或HDMI接口输出。
         如有需要更改CW或CWPK，可修改代码中对应的的CW和CWPK，重编译即可。
   源文件:sample_ca_tsplay.c 
   用法: ./sample_ca_tsplay tsfile cwtype[0|1]
          cwtype: 0-ClearCw 1-EncrytedCw
          如果有多个节目，可通过节目号指定要播放的节目。

5.DVB播放
   描述:此demo可包含如下功能
                1>获取chipid
                2>锁频
                3>加载明文的CW，加载密文CW或密文CWPK到芯片
                4>指定视频PID或音频PID播放节目
                5>指定节目号播放节目
   源文件:sample_ca_dvbplay.c
   用法: ./sample_ca_dvbplay freq
                    
6.数据加/解密
   描述:使用keyladder加解密数据
   源文件:sample_ca_crypto.c
   用法: ./sample_ca_crypto
   
7.PVR录制
   描述:演示高安PVR录制
   源代码:sample_ca_pvr_rec.c
   用法: ./sample_ca_pvrrec pvrrec file_path freq [SymbolRate] [qamtype] [vo_format]
      file_path: 可指定需要录制的文件名字，或指定路径，此demo会自动生成录制文件。
      SymbolRate, qamtype, 和 vo_format 为可选项，默认的SymbolRate是6875。
      qamtype的值是16, 32, 64, 128, 256, or 512，默认为 64QAM。
      vo_format的值是1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，默认是1080i_60。
      
      提示： 运行此demo时，可输入
      1. 'r' ：终止当前录制，并启动新录制。
      2. 'f' ：创建一个用于测试的fingerprint。
      2. 'm' ：创建一个测试的承认级别。
      
8.PVR播放
   描述:此demo演示高安pvr播放。
   源代码:sample_ca_pvr_play.c 
   用法: ./sample_ca_pvrplay file_name [vo_format]
      vo_format值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，默认是1080i_60。

9.PVR时移
   描述:此demo演示高安pvr播放。
   源代码:sample_ca_pvr_timeshift.c。
   用法: ./sample_ca_pvrtimeshift file_path freq [SymbolRate] [qamtype] [vo_format]
       详细用法请参考普通PVR时移。

10.设置STBSN
   描述: 设置STBSN。
   源代码:sample_ca_setSTBSN.c 
   用法: ./sample_ca_stbSTBSN

11.设置MarketID
   描述: 设置MarketID。
   源代码:sample_ca_setMarketID.c 
   用法: ./sample_ca_MarketID

12.SWPK Keyladder演示
   描述: 演示SWPK keyladder的使用。
   源代码:sample_ca_swpk_keyladder.c。
   用法: ./sample_ca_swpk_keyladder

13.SWPK加密
   描述: 演示怎样加密SWPK, 适用于MV200芯片。
   源代码:sample_ca_swpk.c。
   用法: ./sample_ca_swpk

14.CMAC计算
   描述: 演示CMAC计算用法。
   源代码:sample_ca_aes_cbc_mac.c
   用法: ./sample_ca_aes_cbc_mac