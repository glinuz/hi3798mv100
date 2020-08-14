Introduction: Demonstration of switch between playing local ES files and TS files.
Purpose: sample_es_ts_switch esvidfile vidtype tsfile [vo_format]
        esvidfile: video ES file path
        vidtype:   the video encoding format.
        tsfile:    TS file path
        vo_format: video output mode in high definition. Value range: 1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50. Default value: 1080i_50
                The standard definition mode is set automatically inside based on the high definition mode.
       After the program is started, input e to play esvidfile, t to play tsfile. 
       While the program is playing tsfile, programs can be changed by pressing number keys.
