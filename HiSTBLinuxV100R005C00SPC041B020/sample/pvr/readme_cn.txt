sample_pvr_play:
功能描述: sample_pvr_play 用于回放通过pvr录制相关的sample录制下来的码流.
    功能包括正常播放、暂停、快进、快退、慢放、跳转到指定位置、逐帧播放等.

命令格式: sample_pvr_play file_name [vo_format]
      file_name: 播放的文件名，需要包含文件所在路径
      vo_format：可以设置为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, 720P_50. 默认值为1080P_60.
      
      启动播放后如下接口菜单将会显示，可根据菜单提示完成相应的功能:
         n: 正常播放
         p: 暂停
         f: 快进(2x/4x/8x/16x/32x)
         s: 慢放(2x/4x/8x/16x/32x)
         r: 快退(2x/4x/8x/16x/32x)
         t: 逐帧播放
         k: 跳转到文件头
         e: 跳转到文件尾
         l: 向后跳转5秒
         j: 向前跳转5秒
         x: 销毁当前播放通道，然后重新启动
         h: 帮助
         q: 退出

//////////////////////////////////////////////////////////////////////////////////
sample_pvr_multifile_play：
功能描述: sample_pvr_multifile_play 播放按特定命名方式录制的多个文件.
		示例程序能够播放的是按如下命名方式录制的多个文件，文件名的前面几个字符相同，后面三个字符按数字递增:
		rec_xxx_001.ts
	  rec_xxx_002.ts
	  rec_xxx_003.ts

命令格式: sample_pvr_multifile_play file_name [vo_format]
      file_name: 播放的文件名，需要包含文件所在路径
      vo_format：可以设置为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, 720P_50. 默认值为1080P_60.

      启动播放后如下接口菜单将会显示，可根据菜单提示完成相应的功能:
         n: 正常播放
         p: 暂停
         f: 快进(2x/4x/8x/16x/32x)
         s: 慢放(2x/4x/8x/16x/32x)
         r: 快退(2x/4x/8x/16x/32x)
         t: 逐帧播放
         k: 跳转到文件头
         e: 跳转到文件尾
         l: 向后跳转5秒
         j: 向前跳转5秒
         x: 销毁当前播放通道，然后重新启动
         h: 帮助
         q: 退出


//////////////////////////////////////////////////////////////////////////////////
sample_pvr_rec:
功能描述: sample_pvr_rec 用于PVR录制码流.
    示例程序在录制前会扫描节目列表并由用户选择对多路节目(最多支持8路)进行录制

命令格式: sample_pvr_rec file_path freq [SymbolRate] [qamtype or polarization] [vo_format]
      file_path: 录制码流文件的路径，不需要输入文件名，示例程序会根据音视频PID自动生成录制码流的文件名.
      [SymbolRate], [qamtype or polarization], [vo_format]为可选配置，如果不设置将使用默认值.
      SymbolRate: 
          针对有线信号, 默认值为 6875
          针对卫星信号, 默认值为 27500
      qamtype or polarization:
          针对有线信号, 该参数被用作QAM类型, 取值范围为 16|32|64|128|256|512， 默认值为 64
          针对卫星信号, 该参数被用作极化标志, 取值范围为 [0] 水平极化 | [1] 垂直极化，默认值为 0
      vo_format:
          取值范围为 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, 720P_50. 默认值为 1080P_60.
      
      
//////////////////////////////////////////////////////////////////////////////////
sample_pvr_rec_allts:
功能描述: sample_pvr_rec_allts 用于录制一个频点中的所有节目.
    由于录制的数据量较大，使用示例程序前需要检查录制的存储设备速度是否满足要求.

命令格式: sample_pvr_rec_allts file_path freq [SymbolRate] [qamtype or polarization] 
      File_path: 录制码流文件的路径，不需要输入文件名，示例程序会根据频点自动生成录制码流的文件名.
      [SymbolRate], [qamtype or polarization]为可选配置，如果不设置将使用默认值.
      SymbolRate:
          针对有线信号, 默认值为 6875
          针对卫星信号, 默认值为 27500
      qamtype or polarization:
          针对有线信号, 该参数被用作QAM类型, 取值范围为 16|32|64|128|256|512， 默认值为 64
          针对卫星信号, 该参数被用作极化标志, 取值范围为 [0] 水平极化 | [1] 垂直极化，默认值为 0


//////////////////////////////////////////////////////////////////////////////////
sample_pvr_timeshift:
功能描述: sample_pvr_timeshift 用于展示PVR的时移功能.
    示例程序支持直播和回放的相互切换，在回放状态下，支持暂停、快进、快退、慢放、跳转到指定位置、逐帧播放等/

命令格式: sample_pvr_timeshift file_path freq [SymbolRate] [qamtype or polarization] [vo_format]
      file_path: 录制码流文件的路径，不需要输入文件名，示例程序会根据音视频PID自动生成录制码流的文件名.
      [SymbolRate], [qamtype or polarization], [vo_format] 为可选配置，如果不设置将使用默认值.
      SymbolRate:
          针对有线信号, 默认值为 6875
          针对卫星信号, 默认值为 27500
      qamtype or polarization:
          针对有线信号, 该参数被用作QAM类型, 取值范围为 16|32|64|128|256|512， 默认值为 64
          针对卫星信号, 该参数被用作极化标志, 取值范围为 [0] 水平极化 | [1] 垂直极化，默认值为 0
      vo_format:
          取值范围为 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, 720P_50. 默认值为 1080P_60.

      启动示例程序后如下接口菜单将会显示，可根据菜单提示完成相应的功能:
         l: live play
         n: normal play
         p: pause/play
         f: fast forward (4x)
         s: slow forward (4x)
         r: fast rewind (4x)
         h: help
         q: quit
         
         l: 切换到直播状态
         p: 暂停或者切换到回放状态
         f: 快进
         r: 快退
         n: 切换到正常播放
         s: 慢放
         k: 跳转到文件头
         e: 跳转到文件尾
         a: 向前跳转5秒
         d: 向后跳转5秒
         x: 销毁当前播放通道，然后重新启动
         h: 帮助
         q: 退出

注意:
     直播状态下按'p'会暂停当前直播，如果想从当前直播暂停的位置启动回放，则再按一次'p'即可.
     在回放状态下按'l',会从回放状态恢复到直播状态.
     按'p'或者'n'可以从暂停状态下恢复到正常播放. 从特技播放模式恢复到正常播放需要按'n'.

//////////////////////////////////////////////////////////////////////////////////
sample_pvr_eth_timeshift:
功能描述: sample_pvr_eth_timeshift 用于展示在IPTV场景下PVR的时移功能.
    示例程序支持直播和回放的相互切换，在回放状态下，支持暂停、快进、快退、慢放、跳转到指定位置、逐帧播放等/

命令格式: sample_pvr_timeshift file_path multiaddr udpport [vo_format]
      file_path: 录制码流文件的路径，不需要输入文件名，示例程序会根据音视频PID自动生成录制码流的文件名.
      multiaddr: 发送码流的组播地址
      udpport: UDP端口号
      vo_format:
          取值范围为 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, 720P_50. 默认值为 1080P_60.

      启动示例程序后如下接口菜单将会显示，可根据菜单提示完成相应的功能:
         l: live play
         n: normal play
         p: pause/play
         f: fast forward (4x)
         s: slow forward (4x)
         r: fast rewind (4x)
         h: help
         q: quit
         
         l: 切换到直播状态
         p: 暂停或者切换到回放状态
         f: 快进
         r: 快退
         n: 切换到正常播放
         s: 慢放
         k: 跳转到文件头
         e: 跳转到文件尾
         a: 向前跳转5秒
         d: 向后跳转5秒
         x: 销毁当前播放通道，然后重新启动
         h: 帮助
         q: 退出

注意:
     直播状态下按'p'会暂停当前直播，如果想从当前直播暂停的位置启动回放，则再按一次'p'即可.
     在回放状态下按'l',会从回放状态恢复到直播状态.
     按'p'或者'n'可以从暂停状态下恢复到正常播放. 从特技播放模式恢复到正常播放需要按'n'.




