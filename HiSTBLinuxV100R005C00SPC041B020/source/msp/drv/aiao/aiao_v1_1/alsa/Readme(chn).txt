dsp_mix文件夹：创建DSP ALSA声卡，DSP ALSA声卡可以和UNF Audio共存。
i2s_only文件夹：创建非DSP ALSA声卡只和I2S绑定，非DSP ALSA声卡不能和UNF Audio共存。即无法与UNF Audio混音。主要用于不使用UNF Audio项目。
hdmi_only文件夹：创建非DSP ALSA声卡只和HDMI Port绑定。非DSP ALSA声卡不能和UNF Audio共存。即无法与UNF Audio混音。主要用于不使用UNF Audio项目。