/**************************************************************************************
 * Hisilicon MP3 decoder
 * mp3common.h - implementation-independent API's, datatypes, and definitions
 **************************************************************************************/

#ifndef _MP3COMMON_H
#define _MP3COMMON_H

#include "pvmp3decoder_api.h"

#define MAX_SCFBD		4		/* max scalefactor bands per channel */
#define NGRANS_MPEG1	2
#define NGRANS_MPEG2	1

/* 11-bit syncword if MPEG 2.5 extensions are enabled */
#define	SYNCWORDH		0xff
#define	SYNCWORDL		0xe0

/* 12-bit syncword if MPEG 1,2 only are supported 
 * #define	SYNCWORDH		0xff
 * #define	SYNCWORDL		0xf0
 */

 /* determining MAINBUF_SIZE:
 *   max mainDataBegin = (2^9 - 1) bytes (since 9-bit offset) = 511
 *   max nSlots (concatenated with mainDataBegin bytes from before) = 1440 - 9 - 4 + 1 = 1428
 *   511 + 1428 = 1939, round up to 1940 (4-byte align)
 */
//#define MAINBUF_SIZE	1940
#define MAINBUF_SIZE	2000    /* 2000 > (511 + 1440) */

#define MAX_NGRAN		2		/* max granules */
#define MAX_NCHAN		2		/* max channels */
#define MAX_NSAMP		576		/* max samples per channel, per granule */

typedef struct _MP3DecInfo 
{
    /*-----------process with android open code -----------*/
	tPVMP3DecoderExternal  mConfig;
	void *mDecoderBuf;
} MP3DecInfo;


#endif	/* _MP3COMMON_H */
