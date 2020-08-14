Purpose: to demonstrate the function of audio encoding. The module of aenc only 
Input data(data before aenc):supports 1/2-channels and 16-bit 8000~192000Hz-samplerate pcm.
Output data(data after aenc):supports 1/2-channels and 16-bit 16000~48000Hz-samplerate, and can only support aac/g711/amrnb/amrwb audio encoding format.

Command: ./sample_aenc infileName inChannel inSamplerate outfileName outfileType
	Example:./sample_aenc ./test.wav 2 44100 ./test.aac aac
Note: 	
	1) The value of inChannel can be 1,2(The value of inChannel should be the same as the channel of infileName stream).
	2) inSamplerate supports 8000,11025,12000,16000,22050,24000,32000,44100,48000,88200,96000,176400,192000.(The parameter of inSamplerate should be the same as the samplerate of infileName stream.)
	3) outfileType only support aac/g711/amrnb/amrwb


Purpose: to demonstrate the function of audio encoding, which is attached with track.
Input data: supports aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3).
Output data: aac format
Command:./sample_aenctrack infile intype outfile outtype
        Example:./sample_aenc_track ./qgw.mp3 mp3 ./qgw.aac aac
