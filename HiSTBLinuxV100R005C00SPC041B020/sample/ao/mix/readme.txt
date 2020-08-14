Purpose: to demonstrate multiple track of audio mixing. A maximum of 8-channel pulse code modulations (PCMs) are supported.
Command: 
./sample_mixengine file0 TrackWeight0 [SampleRate0] [Channel0] file1 TrackWeight1 [SampleRate1] [Channel1] ......
For example:
./sample_mixengine a.wav 100 48000 1 b.wav 100 48000 1


Purpose: to demonstrate multiple track of audio mixing(tracks attach with avplays).
Command: 
./sample_audiomix freq srate inputfile0 type0 inputfile1 type1 ...
For example:
./sample_audiomix 618 6187 a.mp3 mp3 b.aac aac
