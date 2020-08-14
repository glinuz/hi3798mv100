
Purpose: Sample of playing the of the local ts file on the TVP.
Command: sample_tee_tsplay tsfile [vo_format]
      tsfile: TS file path
      vo_format: video output mode in high definition. Value range: 1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50. Default value: 1080i_50
                The standard definition mode is set automatically inside based on the high definition mode.
      After the program is started, the first program in the ts file is played automatically. Programs can be changed by pressing number keys.
  

Purpose: to demonstrate the picture-in-picture (PIP) play mode on the TVP.
Command: sample_tee_tsplay_pip tsfile_0 tsfile_1 [vo_format]
     tsfile_0: first TS file path
     tsfile_1: second TS file path
     vo_format: video output mode in high definition. Value range: 1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50. Default value: 1080i_50
                The standard definition mode is set automatically inside based on the high definition mode.

Purpose: to demonstrate the DVB playing scenario on the TVP.
Command: sample_tee_dvbplay freq srate [qamtype or polarization] [vo_format]
      Fields [qamtype or polarization] and [vo_format] are optional.
      qamtype or polarization:
          For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64]
          For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0]
      vo_format:
          The value of vo_format can be 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50. The default value is 
          1080i_50.   

