Purpose: to demonstrate the playing of audio element stream (ES) files(support one or multi sound card).
Command: 
usage: sample_audioplay afile1 atype1  (one sound card)
       sample_audioplay afile1 atype1 afile2 atype2   (two sound card)
       sample_audioplay afile1 atype1 afile2 atype2 afile3 atype3 (three sound card)
       atype: aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3)\n");
       examples: 
        ./sample_audioplay afile1 mp3 afile2 aac afile3 ddp;
Note: ddp means Dolby Digital Plus. Sure you got Dolby Digital Plus License before using ddp.