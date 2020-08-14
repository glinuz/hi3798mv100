#ifndef _MP3DEC_H
#define _MP3DEC_H

#ifdef __cplusplus
    #if __cplusplus
    extern "C" {
    #endif  /* __cpluscplus */
#endif  /* __cpluscplus */

#include "hi_type.h"


#define MP3_MAX_NCHANS		2	                      /* max number of channels， dont supoort multichannel  */

#define MP3_MAX_OUT_NSAMPS	1152                      /* max output samples per-frame, per-channel */
#define MP3_MAINBUF_SIZE	4096                      /* min. size. of input buffer. UNIT:bytes */

/* map to 0,1,2 to make table indexing easier */
typedef enum {
	MPEG1 =  0,
	MPEG2 =  1,
	MPEG25 = 2
} MPEGVersion;

typedef void *HMP3Decoder;

enum {
	ERR_MP3_NONE =                 0,   /* 解码正常，无错误 */
	ERR_MP3_INDATA_UNDERFLOW =    -1,   /* 检测到码流输入不足 */
	ERR_MP3_MAINDATA_UNDERFLOW =  -2,   /* 检测到输入码流main data不足 */
	ERR_MP3_FREE_BITRATE_SYNC =   -3,   /* 检测到输入码流free mode下出错 */
	ERR_MP3_OUT_OF_MEMORY =	      -4,   /* 解码器memory不足 */
	ERR_MP3_NULL_POINTER =        -5,   /* 检测到非法的输入指针 */
	ERR_MP3_INVALID_FRAMEHEADER = -6,   /* 检测到输入码流的帧头信息非法 */
	ERR_MP3_INVALID_SIDEINFO =    -7,   /* 检测到输入码流的side information非法 */
	ERR_MP3_INVALID_SCALEFACT =   -8,   /* 检测到输入码流的scale factors非法 */
	ERR_MP3_INVALID_HUFFCODES =   -9,   /* 解码器Huffman译码模块出错 */
	ERR_MP3_FAIL_SYNC =           -10,  /* 解码器检测不到帧头同步字 */

	ERR_MP3_UNKNOWN =             -9999 /* 保留扩展 */
};


typedef struct _MP3FrameInfo {
	int bitrate;
	int nChans;               /* 1 或 2 */
	int samprate;             /* 解码输出采样率 */ 
	int bitsPerSample;        /* 16，目前仅支持16比特位宽输出 */
	int outputSamps;          /* nChans*SamplePerFrame */ 
	int layer;                /* layer,   码流信息，不作为输出控制使用 */
	int version;              /* version, 码流信息，不作为输出控制使用 */
} MP3FrameInfo;

/*
*****************************************************************************
*                         ENCLARATION OF PROTOTYPES
*****************************************************************************
*/

/*****************************************************************************
  Function:    MP3InitDecoder
  Description: create and initial decoder device
  Calls: 
  Called By:
  Input:       
  Output:      
  Return:      HMP3Decoder --Mp3-Decoder handle
  Others:
*****************************************************************************/
HMP3Decoder MP3InitDecoder(HI_VOID);

/*****************************************************************************
  Function:    MP3FreeDecoder
  Description: destroy MP3-Decoder, free the memory.
  Calls: 
  Called By:
  Input:       hMP3Decoder --Mp3-Decoder handle
  Output:      
  Return:      
  Others:
*****************************************************************************/
HI_VOID MP3FreeDecoder(HMP3Decoder hMP3Decoder);

/*****************************************************************************
  Function:    MP3DecodeFindSyncHeader
  Description: look for valid MPEG sync header.
  Calls: 
  Called By:
  Input:       hMP3Decoder   --MP3-Decoder handle
               ppInbufPtr    --address of the pointer of start-point of the bitstream.
                           NOTE: bitstream should be in little endian format.
			   pBytesLeft --pointer to BytesLeft that indicates bitstream numbers at input buffer;
			               after FindSync, pInbufPtr pointer to current bitsrream header, BytesLeft
						   indicates the left bytes.

  Output:      updated bitstream buffer state(ppInbufPtr, pBytesLeft)
  
  Return:      <0    : ERR_MP3_INDATA_UNDERFLOW 
			   other : Success, return number bytes  of current frame.
*****************************************************************************/
HI_S32 MP3DecodeFindSyncHeader(HMP3Decoder hMP3Decoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft);

/*****************************************************************************
  Function:    MP3Decode
  Description: decoding MPEG frame and output 1152(L2/L3) OR 384(L1) 16bit PCM samples per channel.
  Calls: 
  Called By:
  Input:       hMP3Decoder   --MP3-Decoder handle
               ppInbufPtr    --address of the pointer of start-point of the bitstream.
                           NOTE: bitstream should be in little endian format.
			   pBytesLeft --pointer to BytesLeft that indicates bitstream numbers at input buffer;
			               after decoding, pInbufPtr pointer to current bitsrream header, BytesLeft
						   indicates the left bytes.

               pOutPcm    --the address of the out pcm buffer,
						   pcm data in noninterlaced fotmat: L/L/L/... R/R/R/...
                           NOTE: 1 caller must be sure buffer size is GREAT ENOUGH.
               nReserved  -- 0: reserved 
  Output:      output pcm data and updated bitstream buffer state(ppInbufPtr, pBytesLeft)

  Return:      SUCCESS/ERROR_CODE
  Others:
*****************************************************************************/
HI_S32  MP3Decode(HMP3Decoder hMP3Decoder, HI_U8 **ppInbufPtr, HI_S32 *pBytesLeft, HI_S16 *pOutPcm, HI_S32 nReserved);

/*****************************************************************************
  Function:    MP3GetLastFrameInfo
  Description: get the frame information
  Calls: 
  Called By:
  Input:       hMP3Decoder    --MP3-Decoder handle
  Output:      mp3FrameInfo   --frame information
  Return:      
  Others:
*****************************************************************************/
HI_VOID MP3GetLastFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo);



#ifdef __cplusplus
    #if __cplusplus
	}
    #endif  /* __cpluscplus */
#endif  /* __cpluscplus */

#endif	/* _MP3DEC_H */


