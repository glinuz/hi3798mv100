#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hi_unf_common.h"
#include "hi_resampler.h"

#define IN_SMAPLE_NUMBER    80*6
#define OUT_BUFFER_SIZE  (IN_SMAPLE_NUMBER*8)

int main(int argc, char *argv[])
{
    HResampler hSRC = NULL;
    int inrate, outrate, chans;
	int nRead, loop, insamps, outsamps, nFrames;
	char *infileName, *outfileName;
    FILE *infile, *outfile;
    short *inbuf = NULL, *outbuf = NULL;
    HI_SYS_Init();    
    inbuf = (short *)malloc(IN_SMAPLE_NUMBER * 2 * sizeof(short));
    if (NULL == inbuf)
    {
        printf("\n inbuf malloc() error!\n");
        return -1;
    }
    outbuf = (short *)malloc(OUT_BUFFER_SIZE * 6 * sizeof(short));
    if (NULL == outbuf)
    {
        printf("\n outbuf malloc() error!\n");
        return -1;
    }

    if (argc < 6)
    {
        printf("Usage: ./sample_resampler inrate outrate channels infilename outfilename\n"
               "Notes:\n"   
               "    rate : 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 88200, 96000, 176400, 192000\n"     
               "    format : S16_LE, INTERLEAVED\n"   
               "    channels: 1-MONO, 2-STEREO \n"
               "Example:./sample_resampler 8000 16000 1 example_8kHz.pcm example_8kHz_to_16kHz.pcm\n"
               );               
        return -1;
    }

    inrate  = atoi(argv[1]);
    outrate = atoi(argv[2]);
	chans   = atoi(argv[3]);
    infileName   = argv[4];
    outfileName  = argv[5];

	/* open input file */
	infile = fopen(infileName, "rb");
	if (!infile) {
		printf(" *** Error opening input file %s ***\n", infileName);
		return -1;
	}

	/* open output file */
	outfile = fopen(outfileName, "wb");
	if (!outfile) {
		printf(" *** Error opening output file %s ***\n", outfileName);
        fclose(infile);
		return -1;
	}
    
   printf(" *** Begin ResamplerCreate ***\n");

   insamps = IN_SMAPLE_NUMBER;
   hSRC    = HI_Resampler_Create(inrate, outrate, chans);
   if(hSRC == NULL)
   {
	   printf(" *** ResamplerCreate Fail ***\n");
       fclose(infile);
       fclose(outfile);
	   return -1;
   }

   loop    = 4000;
   nFrames = 0;
   printf(" *** Beging resampling input file %s ***\n", infileName);
   while(loop--)
   {
      nRead = fread(inbuf, sizeof(short), insamps*chans, infile);
	  if(nRead != (insamps*chans))
		  break;
      
      outsamps =  HI_Resampler_Process(hSRC, inbuf, insamps, outbuf);
	  nFrames++;
//      if ( !(nFrames&0x003f) )
//          printf("    frame = %d\n", nFrames);

    if(outfile)	{
	  fwrite(outbuf, sizeof(short), outsamps*chans, outfile);
	}
   }

   printf(" *** Resampled output file: %s , frames: %d ***\n", outfileName, nFrames);

   HI_Resampler_Free(hSRC);
   if(infile)
   {
      fclose(infile);
      infile = NULL;
   }
   if(outfile)
   {
      fclose(outfile);
      outfile = NULL;
   }

   if(inbuf)
   {
      free(inbuf);
      inbuf = NULL;
   }
   if(outbuf)
   {
      free(outbuf);
      outbuf = NULL;
   }
   HI_SYS_DeInit();
   
   return 0;	
}
