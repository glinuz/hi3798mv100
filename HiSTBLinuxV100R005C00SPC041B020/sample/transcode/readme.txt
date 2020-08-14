Purpose: to demonstrate the transcoding of audio and video with stream come from dvbplay
Command: sample_dvb_transcode freq srate [qamtype or polarization] [vo_format]
      Fields [qamtype or polarization] and [vo_format] are optional.
      qamtype or polarization:
          For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64]
          For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0]
      vo_format:
          The value of vo_format can be 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50. The default value is 
          1080i_50.

Command: sample_ts_transcode tsfile
      tsfile: ts file.

Note: 
    a) Using VENC and VO modules, video trancoding function helps user transcode DVB program from high definition to low definition with H.264 stream file, 
       which can be played on handle devices such as phone.
    b) Using AENC and AVPLAY modules, audio trancoding function helps user transcode DVB program from pcm data to AAC format file,
       audio transcoding supports 16-bit or 24-bit pcm, the samplerate can be 16000,22050,24000,32000,44100,48000Hz.   

      
