Purpose: to demonstrate the picture-in-picture (PIP) play mode.
Command: sample_dvb_pip freq [srate] [qamtype or polarization] [audmix]
      freq indicates frequency, srate indicates symbol rate, and audmix indicates whether the sounds from two players can be mixed. By default, audio mixing is not supported.
Command sample: sample_dvb_pip 618
      After the program starts, you can press the number key to switch the program channel.

Command: sample_ts_pip tsfile [audmix]
Command sample: sample_ts_pip ./a.ts
