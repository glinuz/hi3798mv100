Introduction: Demonstrating the sample of the tuner lock frequency
Usage: sample_tuner
      After you running the sample_tuner, the operation guide is displayed. Functions of the sample_tuner include frequency setting, symbol rate setting, qam mode setting, audio or video program broadcasting, and obtainment of mosaic region quantity.
      Choose different qam ports and tuners. Currently 10 types of tuners are supported. Detailed tuner types are listed in the operation guide.

Here are the instructions for some frequently used commands:
1. setchnl 3840 27500000 0 (freqency/symbolrate/ploar),
	freqency unit MHz for cable and satellite, KHz for terrestrial. If test DVB S/S2, you should input downlink frequency here,
	symbolrate unit baud for satellite,
	polar 0:Horizontal 1:Vertical 2:Left-hand circular 3:Right-hand circular.
2. play VPID APID: VPID and APID are hex numbers, stand for video pid and audio pid.

For example, if you want to watch CCTV1 using this sample through STAR 6B, you can get the TP information from internet, for CCTV1, the frequency is 3840MHz, the symbol rate is 27.5MBaud, and H polarization, VPID is 512, APID is 650. So you can wantch
CCTV1 after setting the following commands:
setchnl 3840 27500000 0
play 512 650



