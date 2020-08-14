Purpose: to demonstrate the function of decoding I frames
      Switching between dvbplay and I frame decoding is supported.
      Decoded I frames are directly displayed over the video output (VO) device.
      Decoded I frames are displayed over the on-screen display (OSD). The sample can be displayed only at high-definition (HD) graphics layers.
      Decoded I frames can be saved as .jpeg files. (not supported now)
Command: ./iframe_dec file protocol type [freq] [srate]
      file: save path of I frame streams
      protocol: protocol used by I frame streams (0-mpeg2 1-mpeg4 2-avs 3-h264)
      type processing mode for decoded I frames (0-display on vo 1-display on osd 3-save as jpeg)
      freq: frequency used by the dvbplay
      srate: symbol rate used by the dvbplay
      The test case shows only I frame decoding if fields freq and srate are left blank. If the two fields are set, the test case also shows the switching between dvbplay and I frame decoding.
