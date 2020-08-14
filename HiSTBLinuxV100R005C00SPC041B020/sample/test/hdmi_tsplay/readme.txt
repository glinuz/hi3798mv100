Purpose: to test the commands of the hdmi. External transport streams (TSs) need to be read.
Command: sample_hdmi_tsplay TS_stream Videotming HDCPFlag
      TSs of TS_stream:MPEG/H.264 include ccitt_mux_32K.ts and ccitt_mux_ac3.ts.
      Videotming:1080P_60, 1080P_50, 1080P_30, 1080P_25, 1080P_24, 1080i_60, 1080i_50, 720P_60, 720p_50, 576p_50, 480P_60
      HDCPFlag: HDCP encryption transmission flag. The value 0 indicates non-encryption mode and 1 indicates encryption mode.
               Note: The function is valid only if a correct HDCP KEY is written to the one-time programmable (OTP) module. 
               Otherwise, the high-definition multimedia interface (HDMI) fails to work properly.
      
   When the program is running, enter h to query the HDMI items that can be tested.
   For example,
hdmi_cmd >h    
    help          list all command we provide
    q             quit sample test
    hdmi_debug    Display all relative status
    hdmi_deepcolor    set video deepcolor mode
    hdmi_xvycc        set video xvYCC output
    hdmi_video_timing set video output timing format
    hdmi_color_mode   set video color output(RGB/YCbCr)
    hdmi_a_freq       set audio output frequence
    hdmi_a_mute       set audio output mute/unmute
    hdmi_a_downsample set audio output downsample
    hdmi_av_mute      set video&audio mute/unmute
    hdmi_a_compress set audio output compress/uncompress type of PCM, AC3, DTS...
    hdmi_hdcp         set HDCP, to be define
    hdmi_cec          set CEC command, to be define
    
  Each of the preceding commands can be run individually.
  For example, you can run the hdmi_video_timing 0 command to set video timing to 1080p_60.
